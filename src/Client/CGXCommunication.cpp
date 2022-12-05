/**
 * @file CGXCommunication.cpp
 * @author Gurux, Dominik Majer
 * @brief Implementation of 'CGXCommunication' class
 * 
 * @copyright Copyright (c) 2020
 * 
 */

// Memory leak detection for WIN
#ifdef _WIN32
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#endif

#include "CGXCommunication.h"
#include "Logger.h"

#include "boost/format.hpp"

// GuruxCore includes
#include "GURUX_DLMS/GXDLMSConverter.h"
#include "GURUX_DLMS/Objects/ParametersAndMeasurementData/GXDLMSProfileGeneric.h"
#include "GURUX_DLMS/Objects/ParametersAndMeasurementData/GXDLMSDemandRegister.h"
#include "GURUX_DLMS/GXDLMSTranslator.h"
#include "GURUX_DLMS/Objects/ParametersAndMeasurementData/GXDLMSData.h"


CGXCommunication::CGXCommunication(CGXDLMSSecureClient *pParser, int wt, char *invocationCounter, std::string devName) :
        m_WaitTime(wt), m_Parser(pParser),
        m_socket(-1), m_InvocationCounter(invocationCounter) {
#if defined(_WIN32) || defined(_WIN64)//Windows includes
    ZeroMemory(&m_osReader, sizeof(OVERLAPPED));
    ZeroMemory(&m_osWrite, sizeof(OVERLAPPED));
    m_osReader.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    m_osWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
#endif
    m_hComPort = INVALID_HANDLE_VALUE;
    parentDevName.assign(devName);
}

CGXCommunication::~CGXCommunication(void) {
    Logger::LogEvent(Logger::ERROR, (boost::format("[%s] -> CGXCommunication::~CGXCommunication(void) Calling destructor") %
                             parentDevName).str());
    Close();
    delete m_Parser;
}

int CGXCommunication::Disconnect(void) {
    int ret;
    std::vector<CGXByteBuffer> data;
    CGXReplyData reply;
    if (m_hComPort != INVALID_HANDLE_VALUE || m_socket != -1) {
        if ((ret = m_Parser->DisconnectRequest(data)) != 0 || (ret = ReadDataBlock(data, reply)) != 0)
            Logger::LogEvent(
                    Logger::INFO,
                    (boost::format("[%s] -> DisconnectRequest failed (%d) %s.") % parentDevName % ret %
                     CGXDLMSConverter::GetErrorMessage(ret)).str()
            );
    }
    return 0;
}

int CGXCommunication::Close(void) {
    int ret;
    std::vector<CGXByteBuffer> data;
    CGXReplyData reply;

    if (m_hComPort != INVALID_HANDLE_VALUE || m_socket != -1) {
        if ((ret = m_Parser->ReleaseRequest(data)) != 0 || (ret = ReadDataBlock(data, reply)) != 0)
            Logger::LogEvent(
                    Logger::INFO,
                    (boost::format("[%s] -> ReleaseRequest failed (%d) %s.") % parentDevName % ret %
                     CGXDLMSConverter::GetErrorMessage(ret)).str()
            );
    }

    if (m_hComPort != INVALID_HANDLE_VALUE || m_socket != -1) {
        if ((ret = m_Parser->DisconnectRequest(data)) != 0 || (ret = ReadDataBlock(data, reply)) != 0)
            Logger::LogEvent(
                    Logger::INFO,
                    (boost::format("[%s] -> DisconnectRequest failed (%d) %s.") % parentDevName % ret %
                     CGXDLMSConverter::GetErrorMessage(ret)).str()
            );
    }

    if (m_hComPort != INVALID_HANDLE_VALUE) {
#if defined(_WIN32) || defined(_WIN64)//Windows includes
        CloseHandle(m_hComPort);
        CloseHandle(m_osReader.hEvent);
        CloseHandle(m_osWrite.hEvent);
#else
        close(m_hComPort);
#endif
        m_hComPort = INVALID_HANDLE_VALUE;
    }

    if (m_socket != -1) {
#if defined(_WIN32) || defined(_WIN64)//Windows includes
        closesocket(m_socket);
#else
        close(m_socket);
#endif
        m_socket = -1;
    }

    return 0;
}

int CGXCommunication::Connect(const char *pAddress, unsigned short Port) {
    //Close();

    Logger::LogEvent(
            Logger::ERROR,
            (boost::format("[%s] -> Connecting to [%s]") % parentDevName % pAddress).str()
    );

    //create socket.
    m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    if (m_socket == -1) {
        Logger::LogEvent(
                Logger::INFO,
                (boost::format("[%s] -> DLMS_ERROR_CODE_INVALID_PARAMETER") % parentDevName).str()
        );
        assert(0);
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }

    sockaddr_in add;
    add.sin_port = htons(Port);
    add.sin_family = AF_INET;
    add.sin_addr.s_addr = inet_addr(pAddress);

    //If address is give as name
    if (add.sin_addr.s_addr == INADDR_NONE) {
        hostent *Hostent = gethostbyname(pAddress);
        if (Hostent == NULL) {
#if defined(_WIN32) || defined(_WIN64)//If Windows
            int err = WSAGetLastError();
#elif __linux__
            int err = errno;
#endif
            Logger::LogEvent(
                    Logger::INFO,
                    (boost::format("[%s] -> Error number: %d") % parentDevName % err).str()
            );
            Close();
            return err;

        };

        add.sin_addr = *(in_addr *) (void *) Hostent->h_addr_list[0];
    };

    //Connect to the meter.
    int ret = connect(m_socket, (sockaddr *) &add, sizeof(sockaddr_in));
    if (ret == -1) {
        Logger::LogEvent(
                Logger::INFO,
                (boost::format("[%s] -> DLMS_ERROR_CODE_INVALID_PARAMETER") % parentDevName).str()
        );
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    };

    Logger::LogEvent(
            Logger::ERROR,
            (boost::format("[%s] -> Connected") % parentDevName).str()
    );

    return DLMS_ERROR_CODE_OK;
}

int CGXCommunication::Read(unsigned char eop, CGXByteBuffer &reply) {
#if defined(_WIN32) || defined(_WIN64)//Windows
    unsigned long RecieveErrors;
    COMSTAT comstat;
    DWORD bytesRead = 0;
#else //If Linux.
    unsigned short bytesRead = 0;
    int ret, readTime = 0;
#endif

    int pos;
    unsigned long cnt = 1;
    bool bFound = false;
    int lastReadIndex = 0;

    do {
#if defined(_WIN32) || defined(_WIN64)//Windows
        //We do not want to read byte at the time.
        if (!ClearCommError(m_hComPort, &RecieveErrors, &comstat)) {
            Logger::LogEvent(
                    Logger::INFO,
                    (boost::format("[%s] -> DLMS_ERROR_CODE_SEND_FAILED") % parentDevName).str()
            );
            return DLMS_ERROR_CODE_SEND_FAILED;
        }

        bytesRead = 0;
        cnt = 1;
        //Try to read at least one byte.
        if (comstat.cbInQue > 0)
            cnt = comstat.cbInQue;

        //If there is more data than can fit to buffer.
        if (cnt > RECEIVE_BUFFER_SIZE)
            cnt = RECEIVE_BUFFER_SIZE;

        if (!ReadFile(m_hComPort, m_Receivebuff, cnt, &bytesRead, &m_osReader)) {
            DWORD nErr = GetLastError();
            if (nErr != ERROR_IO_PENDING) {
                Logger::LogEvent(
                        Logger::INFO,
                        (boost::format("[%s] -> DLMS_ERROR_CODE_RECEIVE_FAILED") % parentDevName).str()
                );
                return DLMS_ERROR_CODE_RECEIVE_FAILED;
            }

            //Wait until data is actually read
            if (::WaitForSingleObject(m_osReader.hEvent, m_WaitTime) != WAIT_OBJECT_0) {
                Logger::LogEvent(
                        Logger::INFO,
                        (boost::format("[%s] -> DLMS_ERROR_CODE_RECEIVE_FAILED") % parentDevName).str()
                );
                return DLMS_ERROR_CODE_RECEIVE_FAILED;
            }

            if (!GetOverlappedResult(m_hComPort, &m_osReader, &bytesRead, TRUE)) {
                Logger::LogEvent(
                        Logger::INFO,
                        (boost::format("[%s] -> DLMS_ERROR_CODE_RECEIVE_FAILED") % parentDevName).str()
                );
                return DLMS_ERROR_CODE_RECEIVE_FAILED;
            }
        }
#else
        //Get bytes available.
        ret = ioctl(m_hComPort, FIONREAD, &cnt);
        //If driver is not supporting this functionality.
        if (ret < 0)
        {
            cnt = RECEIVE_BUFFER_SIZE;
        }
        else if (cnt == 0)
        {
            //Try to read at least one byte.
            cnt = 1;
        }
        //If there is more data than can fit to buffer.
        if (cnt > RECEIVE_BUFFER_SIZE)
        {
            cnt = RECEIVE_BUFFER_SIZE;
        }
        bytesRead = read(m_hComPort, m_Receivebuff, cnt);
        if (bytesRead == 0xFFFF)
        {
            //If wait time has elapsed.
            if (errno == EAGAIN)
            {
                if (readTime > m_WaitTime)
                {
                    printf("Read failed. Timeout occurred.\r\n");
                    return DLMS_ERROR_CODE_RECEIVE_FAILED;
                }
                readTime += 100;
                bytesRead = 0;
            }
            //If connection is closed.
            else if (errno == EBADF)
            {
                printf("Read failed. Connection closed.\r\n");
                return DLMS_ERROR_CODE_RECEIVE_FAILED;
            }
            else
            {
                printf("Read failed. %d.\r\n", errno);
                return DLMS_ERROR_CODE_RECEIVE_FAILED;
            }
        }
#endif
        reply.Set(m_Receivebuff, bytesRead);
        //Note! Some USB converters can return true for ReadFile and Zero as bytesRead.
        //In that case wait for a while and read again.
        if (bytesRead == 0) {
#if defined(_WIN32) || defined(_WIN64)//Windows
            Sleep(100);
#else
            usleep(100000);
#endif
            continue;
        }
        if (reply.GetSize() > 5) {
            //Some optical strobes can return extra bytes.
            for (pos = reply.GetSize() - 1; pos != lastReadIndex; --pos) {
                if (reply.GetData()[pos] == eop) {
                    bFound = true;
                    break;
                }
            }
            lastReadIndex = pos;
        }
    } while (!bFound);

    return DLMS_ERROR_CODE_OK;
}

int CGXCommunication::UpdateFrameCounter(void) {
    int ret = 0;

    //Read frame counter if GeneralProtection is used.
    if (m_InvocationCounter != NULL && m_Parser->GetCiphering() != NULL &&
        m_Parser->GetCiphering()->GetSecurity() != DLMS_SECURITY_NONE) {
        Logger::LogEvent(
                Logger::ERROR,
                (boost::format("[%s] -> UpdateFrameCounter") % parentDevName).str()
        );

        m_Parser->SetProposedConformance(
                (DLMS_CONFORMANCE) (m_Parser->GetProposedConformance() | DLMS_CONFORMANCE_GENERAL_PROTECTION));

        unsigned long add = m_Parser->GetClientAddress();
        DLMS_AUTHENTICATION auth = m_Parser->GetAuthentication();
        DLMS_SECURITY security = m_Parser->GetCiphering()->GetSecurity();
        CGXByteBuffer &challenge = m_Parser->GetCtoSChallenge();

        std::vector<CGXByteBuffer> data;
        CGXReplyData reply;

        m_Parser->SetClientAddress(16);
        m_Parser->SetAuthentication(DLMS_AUTHENTICATION_NONE);
        m_Parser->GetCiphering()->SetSecurity(DLMS_SECURITY_NONE);

        //Get meter's send and receive buffers size.
        if ((ret = m_Parser->SNRMRequest(data)) != 0 ||
            (ret = ReadDataBlock(data, reply)) != 0 ||
            (ret = m_Parser->ParseUAResponse(reply.GetData())) != 0) {
            Logger::LogEvent(
                    Logger::INFO,
                    (boost::format("[%s] -> SNRMRequest failed %d") % parentDevName % ret).str()
            );
            return ret;
        }

        reply.Clear();

        if ((ret = m_Parser->AARQRequest(data)) != 0 ||
            (ret = ReadDataBlock(data, reply)) != 0 ||
            (ret = m_Parser->ParseAAREResponse(reply.GetData())) != 0) {
            if (ret == DLMS_ERROR_CODE_APPLICATION_CONTEXT_NAME_NOT_SUPPORTED) {
                Logger::LogEvent(
                        Logger::INFO,
                        (boost::format("[%s] -> Use Logical Name referencing is wrong. Change it!") %
                         parentDevName).str()
                );
                return ret;
            }

            Logger::LogEvent(
                    Logger::INFO,
                    (boost::format("[%s] -> AARQRequest failed (%d) %s") % parentDevName % ret %
                     CGXDLMSConverter::GetErrorMessage(ret)).str()
            );
            return ret;
        }

        reply.Clear();

        std::string ln;
        ln.append(m_InvocationCounter);
        CGXDLMSData d(ln);
        std::string str;

        if ((ret = Read(&d, 2, str)) == 0)
            m_Parser->GetCiphering()->SetInvocationCounter(1 + d.GetValue().ToInteger());

        reply.Clear();
        Disconnect();

        m_Parser->SetClientAddress(add);
        m_Parser->SetAuthentication(auth);
        m_Parser->GetCiphering()->SetSecurity(security);
        m_Parser->SetCtoSChallenge(challenge);
    }

    return ret;
}

int CGXCommunication::InitializeConnection(void) {
    int ret = 0;
    if ((ret = UpdateFrameCounter()) != 0)
        return ret;

    Logger::LogEvent(
            Logger::ERROR,
            (boost::format("[%s] -> InitializeConnection") % parentDevName).str()
    );

    std::vector<CGXByteBuffer> data;
    CGXReplyData reply;

    //Get meter's send and receive buffers size.
    if ((ret = m_Parser->SNRMRequest(data)) != 0 ||
        (ret = ReadDataBlock(data, reply)) != 0 ||
        (ret = m_Parser->ParseUAResponse(reply.GetData())) != 0) {
        Logger::LogEvent(
                Logger::INFO,
                (boost::format("[%s] -> SNRMRequest failed %d") % parentDevName % ret).str()
        );
        return ret;
    }

    reply.Clear();
    if ((ret = m_Parser->AARQRequest(data)) != 0 ||
        (ret = ReadDataBlock(data, reply)) != 0 ||
        (ret = m_Parser->ParseAAREResponse(reply.GetData())) != 0) {
        if (ret == DLMS_ERROR_CODE_APPLICATION_CONTEXT_NAME_NOT_SUPPORTED) {
            Logger::LogEvent(
                    Logger::INFO,
                    (boost::format("[%s] -> Use Logical Name referencing is wrong. Change it!") % parentDevName).str()
            );
            return ret;
        }

        Logger::LogEvent(
                Logger::INFO,
                (boost::format("[%s] -> AARQRequest failed (%d) %s") % parentDevName % ret %
                 CGXDLMSConverter::GetErrorMessage(ret)).str()
        );
        return ret;
    }

    reply.Clear();

    // Get challenge Is HLS authentication is used.
    if (m_Parser->IsAuthenticationRequired()) {
        if ((ret = m_Parser->GetApplicationAssociationRequest(data)) != 0 ||
            (ret = ReadDataBlock(data, reply)) != 0 ||
            (ret = m_Parser->ParseApplicationAssociationResponse(reply.GetData())) != 0) {
            Logger::LogEvent(
                    Logger::INFO,
                    (boost::format("[%s] -> Authentication failed (%d) %s") % parentDevName % ret %
                     CGXDLMSConverter::GetErrorMessage(ret)).str()
            );
            return ret;
        }
    }

    return DLMS_ERROR_CODE_OK;
}

int CGXCommunication::ReadDLMSPacket(CGXByteBuffer &data, CGXReplyData &reply) {
    int ret;
    CGXByteBuffer bb;
    std::string strTmp;
    CGXReplyData notify;
    if (data.GetSize() == 0)
        return DLMS_ERROR_CODE_OK;

    strTmp = "[TX]  ";
    strTmp += data.ToHexString();
    Logger::LogEvent(
            Logger::ERROR,
            (boost::format("[%s] -> %s") % parentDevName % strTmp).str()
    );

    int len = data.GetSize();
    if (m_hComPort != INVALID_HANDLE_VALUE) {
#if defined(_WIN32) || defined(_WIN64)//If Windows
        DWORD sendSize = 0;
        BOOL bRes = ::WriteFile(m_hComPort, data.GetData(), len, &sendSize, &m_osWrite);

        if (!bRes) {
            COMSTAT comstat;
            unsigned long RecieveErrors;
            DWORD err = GetLastError();

            //If error occurs...
            if (err != ERROR_IO_PENDING) {
                Logger::LogEvent(
                        Logger::INFO,
                        (boost::format("[%s] -> DLMS_ERROR_CODE_SEND_FAILED") % parentDevName).str()
                );
                return DLMS_ERROR_CODE_SEND_FAILED;
            }

            //Wait until data is actually sent
            ret = WaitForSingleObject(m_osWrite.hEvent, m_WaitTime);
            if (ret != 0) {
                DWORD err = GetLastError();
                Logger::LogEvent(
                        Logger::INFO,
                        (boost::format("[%s] -> DLMS_ERROR_CODE_SEND_FAILED (%s)") % parentDevName % err).str()
                );
                return DLMS_ERROR_CODE_SEND_FAILED;
            }

            //Read bytes in output buffer. Some USB converts require this.
            if (!ClearCommError(m_hComPort, &RecieveErrors, &comstat)) {
                Logger::LogEvent(
                        Logger::INFO,
                        (boost::format("[%s] -> DLMS_ERROR_CODE_SEND_FAILED") % parentDevName).str()
                );
                return DLMS_ERROR_CODE_SEND_FAILED;
            }
        }
#else //If Linux
        ret = write(m_hComPort, data.GetData(), len);
        if (ret != len)
        {
            printf("write failed %d\n", errno);
            return DLMS_ERROR_CODE_SEND_FAILED;
        }
#endif
    } else if ((ret = send(m_socket, (const char *) data.GetData(), len, 0)) == -1) {
        Logger::LogEvent(
                Logger::INFO,
                (boost::format("[%s] -> DLMS_ERROR_CODE_SEND_FAILED: %d") % parentDevName %
                 #if defined(_WIN32) || defined (_WIN64)
                 WSAGetLastError()
#else
                        errno
#endif
                ).str()
        );
        return DLMS_ERROR_CODE_SEND_FAILED;
    }

    // Loop until whole DLMS packet is received.
    strTmp = "";
    do {
        if (notify.GetData().GetSize() != 0) {
            //Handle notify.
            if (!notify.IsMoreData()) {
                //Show received push message as XML.
                std::string xml;
                CGXDLMSTranslator t(DLMS_TRANSLATOR_OUTPUT_TYPE_SIMPLE_XML);

                if ((ret = t.DataToXml(notify.GetData(), xml)) != 0)
                    Logger::LogEvent(
                            Logger::INFO,
                            (boost::format("[%s] -> ERROR! DataToXml failed.") % parentDevName).str()
                    );
                else
                    Logger::LogEvent(
                            Logger::INFO,
                            (boost::format("[%s] -> %s") % parentDevName % xml.c_str()).str()
                    );

                notify.Clear();
            }
            continue;
        }

        if (m_hComPort != INVALID_HANDLE_VALUE) {
            unsigned short pos = (unsigned short) bb.GetSize();
            if (Read(0x7E, bb) != 0) {
                Logger::LogEvent(
                        Logger::INFO,
                        (boost::format("[%s] -> DLMS_ERROR_CODE_SEND_FAILED: Read failed") % parentDevName).str()
                );
                return DLMS_ERROR_CODE_SEND_FAILED;
            }
            if (strTmp.size() == 0) {
                //Now(tmp);
                strTmp = "[RX]  " + strTmp;
            } else {
                strTmp += " ";
            }
            strTmp += bb.ToHexString(pos, bb.GetSize() - pos, true);
        } else {
            len = RECEIVE_BUFFER_SIZE;
            if ((ret = recv(m_socket, (char *) m_Receivebuff, len, 0)) == -1) {
#if defined(_WIN32) || defined(_WIN64)//If Windows
                Logger::LogEvent(
                        Logger::INFO,
                        (boost::format("[%s] -> DLMS_ERROR_CODE_RECEIVE_FAILED: %d") % parentDevName %
                         WSAGetLastError()).str()
                );
#else
                printf("recv failed %d\n", errno);
#endif
                return DLMS_ERROR_CODE_RECEIVE_FAILED;
            }
            bb.Set(m_Receivebuff, ret);
            if (strTmp.size() == 0) {
                strTmp = "[RX]  " + strTmp;
            } else {
                strTmp += " ";
            }
            strTmp += GXHelpers::BytesToHex(m_Receivebuff, ret);
        }
    } while ((ret = m_Parser->GetData(bb, reply, notify)) == DLMS_ERROR_CODE_FALSE);


    Logger::LogEvent(
            Logger::ERROR,
            (boost::format("[%s] -> %s") % parentDevName % strTmp).str()
    );

    if (ret == DLMS_ERROR_CODE_REJECTED) {
#if defined(_WIN32) || defined(_WIN64)//Windows
        Sleep(1000);
#else
        usleep(1000000);
#endif
        ret = ReadDLMSPacket(data, reply);
    }

    return ret;
}

int CGXCommunication::ReadDataBlock(CGXByteBuffer &data, CGXReplyData &reply) {
    //If ther is no data to send.
    if (data.GetSize() == 0)
        return DLMS_ERROR_CODE_OK;

    int ret;
    CGXByteBuffer bb;

    //Send data.
    if ((ret = ReadDLMSPacket(data, reply)) != DLMS_ERROR_CODE_OK)
        return ret;

    while (reply.IsMoreData()) {
        bb.Clear();

        if ((ret = m_Parser->ReceiverReady(reply.GetMoreData(), bb)) != 0)
            return ret;

        if ((ret = ReadDLMSPacket(bb, reply)) != DLMS_ERROR_CODE_OK)
            return ret;

    }

    return DLMS_ERROR_CODE_OK;
}

int CGXCommunication::ReadDataBlock(std::vector<CGXByteBuffer> &data, CGXReplyData &reply) {
    //If ther is no data to send.
    if (data.size() == 0)
        return DLMS_ERROR_CODE_OK;

    int ret;
    CGXByteBuffer bb;

    //Send data.
    for (std::vector<CGXByteBuffer>::iterator it = data.begin(); it != data.end(); ++it) {
        //Send data.
        if ((ret = ReadDLMSPacket(*it, reply)) != DLMS_ERROR_CODE_OK)
            return ret;

        while (reply.IsMoreData()) {
            bb.Clear();

            if ((ret = m_Parser->ReceiverReady(reply.GetMoreData(), bb)) != 0)
                return ret;

            if ((ret = ReadDLMSPacket(bb, reply)) != DLMS_ERROR_CODE_OK)
                return ret;
        }
    }

    return DLMS_ERROR_CODE_OK;
}

int CGXCommunication::GetAssociationView() {
    Logger::LogEvent(
            Logger::ERROR,
            (boost::format("[%s] -> GetAssociationView") % parentDevName).str()
    );

    int ret;
    std::vector<CGXByteBuffer> data;
    CGXReplyData reply;

    if ((ret = m_Parser->GetObjectsRequest(data)) != 0 ||
        (ret = ReadDataBlock(data, reply)) != 0 ||
        (ret = m_Parser->ParseObjects(reply.GetData(), true)) != 0) {
        Logger::LogEvent(
                Logger::INFO,
                (boost::format("[%s] -> GetObjects failed %d") % parentDevName % ret).str()
        );
        return ret;
    }

    return DLMS_ERROR_CODE_OK;
}

int CGXCommunication::Read(CGXDLMSObject *pObject, int attributeIndex, std::string &value) {
    value.clear();
    int ret;
    std::vector<CGXByteBuffer> data;
    CGXReplyData reply;

    //Read data from the meter.
    if ((ret = m_Parser->Read(pObject, attributeIndex, data)) != 0 ||
        (ret = ReadDataBlock(data, reply)) != 0 ||
        (ret = m_Parser->UpdateValue(*pObject, attributeIndex, reply.GetValue())) != 0) {
        return ret;
    }

    //Update data type.
    DLMS_DATA_TYPE type;
    if ((ret = pObject->GetDataType(attributeIndex, type)) != 0)
        return ret;

    if (type == DLMS_DATA_TYPE_NONE) {
        type = reply.GetValue().vt;
        if ((ret = pObject->SetDataType(attributeIndex, type)) != 0)
            return ret;

    }

    //Get read value as string.
    //Note! This is for example. It's faster if you handle read COSEM object directly.
    std::vector<std::string> values;
    pObject->GetValues(values);
    value = values[attributeIndex - 1];

    return DLMS_ERROR_CODE_OK;
}

int CGXCommunication::Read(CGXDLMSObject *pObject, int attributeIndex, std::string &value, DLMS_DATA_TYPE &value_unit) {
    value.clear();
    int ret;
    std::vector<CGXByteBuffer> data;
    CGXReplyData reply;

    //Read data from the meter.
    if ((ret = m_Parser->Read(pObject, attributeIndex, data)) != 0 ||
        (ret = ReadDataBlock(data, reply)) != 0 ||
        (ret = m_Parser->UpdateValue(*pObject, attributeIndex, reply.GetValue())) != 0) {
        return ret;
    }

    //Update data type.
    DLMS_DATA_TYPE type;
    if ((ret = pObject->GetDataType(attributeIndex, type)) != 0)
        return ret;

    if (type == DLMS_DATA_TYPE_NONE) {
        type = reply.GetValue().vt;
        if ((ret = pObject->SetDataType(attributeIndex, type)) != 0)
            return ret;

    }

    //Get read value as string.
    //Note! This is for example. It's faster if you handle read COSEM object directly.
    std::vector<std::string> values;
    pObject->GetValues(values);
    value = values[attributeIndex - 1];

    // DM
    if ((attributeIndex-1) < pObject->GetAttributes().size())
        value_unit = pObject->GetAttributes().at(attributeIndex - 1).GetDataType();

    return DLMS_ERROR_CODE_OK;
}

int CGXCommunication::ReadList(std::vector<std::pair<CGXDLMSObject *, unsigned char> > &list) {
    int ret;
    CGXReplyData reply;
    std::vector<CGXByteBuffer> data;
    if (list.size() == 0)
        return 0;

    //Get values from the meter.
    if ((ret = m_Parser->ReadList(list, data)) != 0)
        return ret;

    std::vector<CGXDLMSVariant> values;
    for (std::vector<CGXByteBuffer>::iterator it = data.begin(); it != data.end(); ++it) {
        if ((ret = ReadDataBlock(*it, reply)) != 0)
            return ret;

        if (reply.GetValue().vt == DLMS_DATA_TYPE_ARRAY)
            values.insert(values.end(), reply.GetValue().Arr.begin(), reply.GetValue().Arr.end());

        reply.Clear();
    }
    if (values.size() != list.size())
        //Invalid reply. Read items count do not match.
    {
        Logger::LogEvent(
                Logger::INFO,
                (boost::format("[%s] -> DLMS_ERROR_CODE_INVALID_PARAMETER") % parentDevName).str()
        );
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }

    return m_Parser->UpdateValues(list, values);
}

int CGXCommunication::Write(CGXDLMSObject *pObject, int attributeIndex, CGXDLMSVariant &value) {
    int ret;
    std::vector<CGXByteBuffer> data;
    CGXReplyData reply;
    //Get meter's send and receive buffers size.
    if ((ret = m_Parser->Write(pObject, attributeIndex, value, data)) != 0 || (ret = ReadDataBlock(data, reply)) != 0)
        return ret;

    return DLMS_ERROR_CODE_OK;
}

int CGXCommunication::Write(CGXDLMSObject *pObject, int attributeIndex) {
    int ret;
    std::vector<CGXByteBuffer> data;
    CGXReplyData reply;

    //Get meter's send and receive buffers size.
    if ((ret = m_Parser->Write(pObject, attributeIndex, data)) != 0 || (ret = ReadDataBlock(data, reply)) != 0)
        return ret;

    return DLMS_ERROR_CODE_OK;
}

int CGXCommunication::Method(CGXDLMSObject *pObject, int attributeIndex, CGXDLMSVariant &value) {
    int ret;
    std::vector<CGXByteBuffer> data;
    CGXReplyData reply;

    //Get meter's send and receive buffers size.
    if ((ret = m_Parser->Method(pObject, attributeIndex, value, data)) != 0 || (ret = ReadDataBlock(data, reply)) != 0)
        return ret;

    return DLMS_ERROR_CODE_OK;
}

int CGXCommunication::ReadRowsByRange(CGXDLMSProfileGeneric *pObject, CGXDateTime &start, CGXDateTime &end,
                                      CGXDLMSVariant &rows) {
    return ReadRowsByRange(pObject, &start.GetValue(), &end.GetValue(), rows);
}

int CGXCommunication::ReadRowsByRange(CGXDLMSProfileGeneric *pObject, struct tm *start, struct tm *end,
                                      CGXDLMSVariant &rows) {
    rows.Clear();
    int ret;
    std::vector<CGXByteBuffer> data;
    CGXReplyData reply;

    if ((ret = m_Parser->ReadRowsByRange(pObject, start, end, data)) != 0 ||
        (ret = ReadDataBlock(data, reply)) != 0 ||
        (ret = m_Parser->UpdateValue(*pObject, 2, reply.GetValue())) != 0) {
        return ret;
    }

    //Get rows value as string.
    //Note! This is for example. It's faster if you handle read COSEM object directly.
    std::vector<std::string> values;
    pObject->GetValues(values);
    rows = values[2 - 1];

    return DLMS_ERROR_CODE_OK;
}

int CGXCommunication::ReadRowsByEntry(CGXDLMSProfileGeneric *pObject, unsigned int index, unsigned int count,
                                      CGXDLMSVariant &rows) {
    rows.Clear();
    int ret;
    std::vector<CGXByteBuffer> data;
    CGXReplyData reply;

    if ((ret = m_Parser->ReadRowsByEntry(pObject, index, count, data)) != 0 ||
        (ret = ReadDataBlock(data, reply)) != 0 ||
        (ret = m_Parser->UpdateValue(*pObject, 2, reply.GetValue())) != 0) {
        return ret;
    }

    //Get rows value as string.
    //Note! This is for example. It's faster if you handle read COSEM object directly.
    std::vector<std::string> values;
    pObject->GetValues(values);
    rows = values[2 - 1];

    return DLMS_ERROR_CODE_OK;
}

int CGXCommunication::ReadScalerAndUnits() {
    Logger::LogEvent(
            Logger::ERROR,
            (boost::format("[%s] -> ReadScalerAndUnits") % parentDevName).str()
    );

    int ret = 0;
    std::string str;
    std::string ln;
    std::vector<std::pair<CGXDLMSObject *, unsigned char> > list;

    if ((m_Parser->GetNegotiatedConformance() & DLMS_CONFORMANCE_MULTIPLE_REFERENCES) != 0) {
        // Read scalers and units from the device.
        for (std::vector<CGXDLMSObject *>::iterator it = m_Parser->GetObjects().begin();
             it != m_Parser->GetObjects().end(); ++it) {
            if ((*it)->GetObjectType() == DLMS_OBJECT_TYPE_REGISTER ||
                (*it)->GetObjectType() == DLMS_OBJECT_TYPE_EXTENDED_REGISTER)
                list.push_back(std::make_pair(*it, 3));
            else if ((*it)->GetObjectType() == DLMS_OBJECT_TYPE_DEMAND_REGISTER)
                list.push_back(std::make_pair(*it, 4));
        }

        if ((ret = ReadList(list)) != 0) {
            Logger::LogEvent(
                    Logger::INFO,
                    (boost::format("[%s] -> Err! Failed to read register: %s (%s)") % parentDevName % ln.c_str() %
                     CGXDLMSConverter::GetErrorMessage(ret)).str()
            );
            m_Parser->SetNegotiatedConformance(
                    (DLMS_CONFORMANCE) (m_Parser->GetNegotiatedConformance() & ~DLMS_CONFORMANCE_MULTIPLE_REFERENCES));
        }
    }

    if ((m_Parser->GetNegotiatedConformance() & DLMS_CONFORMANCE_MULTIPLE_REFERENCES) == 0) {
        //If readlist is not supported read one value at the time.
        for (std::vector<CGXDLMSObject *>::iterator it = m_Parser->GetObjects().begin();
             it != m_Parser->GetObjects().end(); ++it) {
            if ((*it)->GetObjectType() == DLMS_OBJECT_TYPE_REGISTER ||
                (*it)->GetObjectType() == DLMS_OBJECT_TYPE_EXTENDED_REGISTER) {
                (*it)->GetLogicalName(ln);
                //WriteValue(m_Trace, ln.c_str());
                if ((ret = Read(*it, 3, str)) != 0) {
                    Logger::LogEvent(
                            Logger::INFO,
                            (boost::format("[%s] -> Err! Failed to read register: %s (%s)") % parentDevName %
                             ln.c_str() % CGXDLMSConverter::GetErrorMessage(ret)).str()
                    );

                    //Continue reading.
                    continue;
                }
            } else if ((*it)->GetObjectType() == DLMS_OBJECT_TYPE_DEMAND_REGISTER) {
                (*it)->GetLogicalName(ln);
                //WriteValue(m_Trace, ln.c_str());
                if ((ret = Read(*it, 4, str)) != 0) {
                    Logger::LogEvent(
                            Logger::INFO,
                            (boost::format("[%s] -> Err! Failed to read register: %s (%s)") % parentDevName %
                             ln.c_str() % CGXDLMSConverter::GetErrorMessage(ret)).str()
                    );

                    //Continue reading.
                    continue;
                }
            }
        }
    }

    return ret;
}

int CGXCommunication::GetProfileGenericColumns() {
    Logger::LogEvent(
            Logger::ERROR,
            (boost::format("[%s] -> GetProfileGenericColumns") % parentDevName).str()
    );

    int ret = 0;
    std::string ln;
    std::string value;

    //Read columns.
    CGXDLMSObjectCollection profileGenerics;
    m_Parser->GetObjects().GetObjects(DLMS_OBJECT_TYPE_PROFILE_GENERIC, profileGenerics);

    for (std::vector<CGXDLMSObject *>::iterator it = profileGenerics.begin(); it != profileGenerics.end(); ++it) {
        //Read Profile Generic columns first.
        CGXDLMSProfileGeneric *pg = (CGXDLMSProfileGeneric *) *it;
        if ((ret = Read(pg, 3, value)) != 0) {
            Logger::LogEvent(
                    Logger::INFO,
                    (boost::format("[%s] -> Err! Failed to read columns: %s") % parentDevName %
                     CGXDLMSConverter::GetErrorMessage(ret)).str()
            );

            //Continue reading.
            continue;
        }

        //Update columns scalers.
        DLMS_OBJECT_TYPE ot;
        CGXDLMSObject *obj;

        for (std::vector<std::pair<CGXDLMSObject *, CGXDLMSCaptureObject *> >::iterator it2 = pg->GetCaptureObjects().begin();
             it2 != pg->GetCaptureObjects().end(); ++it2) {
            ot = it2->first->GetObjectType();
            if (ot == DLMS_OBJECT_TYPE_REGISTER || ot == DLMS_OBJECT_TYPE_EXTENDED_REGISTER ||
                ot == DLMS_OBJECT_TYPE_DEMAND_REGISTER) {
                it2->first->GetLogicalName(ln);
                obj = m_Parser->GetObjects().FindByLN(ot, ln);
                if (obj != NULL) {
                    if (ot == DLMS_OBJECT_TYPE_REGISTER || ot == DLMS_OBJECT_TYPE_EXTENDED_REGISTER) {
                        ((CGXDLMSRegister *) it2->first)->SetScaler(((CGXDLMSRegister *) obj)->GetScaler());
                        ((CGXDLMSRegister *) it2->first)->SetUnit(((CGXDLMSRegister *) obj)->GetUnit());
                    } else if (ot == DLMS_OBJECT_TYPE_DEMAND_REGISTER) {
                        ((CGXDLMSDemandRegister *) it2->first)->SetScaler(((CGXDLMSDemandRegister *) obj)->GetScaler());
                        ((CGXDLMSDemandRegister *) it2->first)->SetUnit(((CGXDLMSDemandRegister *) obj)->GetUnit());
                    }
                }
            }
        }

        Logger::LogEvent(
                Logger::ERROR,
                (boost::format("[%s] -> Profile Generic %s Columns:") % parentDevName %
                 (*it)->GetName().ToString()).str()
        );

        std::string str;
        for (std::vector<std::pair<CGXDLMSObject *, CGXDLMSCaptureObject *> >::iterator it2 = pg->GetCaptureObjects().begin();
             it2 != pg->GetCaptureObjects().end(); ++it2) {
            if (str.size() != 0)
                str.append(" | ");

            str.append((*it2).first->GetName().ToString());
            str.append(" ");
            str.append((*it2).first->GetDescription());
        }
        Logger::LogEvent(
                Logger::ERROR,
                (boost::format("[%s] -> %s") % parentDevName % str).str()
        );
    }


    return ret;
}

int CGXCommunication::GetReadOut() {
    Logger::LogEvent(
            Logger::ERROR,
            (boost::format("[%s] -> GetReadOut") % parentDevName).str()
    );

    int ret = 0;            // ??
    std::string value;
    DLMS_DATA_TYPE value_unit;

    for (std::vector<CGXDLMSObject *>::iterator it = m_Parser->GetObjects().begin();
         it != m_Parser->GetObjects().end(); ++it) {
        // Profile generics are read later because they are special cases.
        // (There might be so lots of data and we so not want waste time to read all the data.)
        if ((*it)->GetObjectType() == DLMS_OBJECT_TYPE_PROFILE_GENERIC)
            continue;

        if (dynamic_cast<CGXDLMSCustomObject *>((*it)) != NULL) {
            //If interface is not implemented.
            //Example manufacturer specific interface.
            Logger::LogEvent(
                    Logger::ERROR,
                    (boost::format("[%s] -> Unknown Interface: %d") % parentDevName % (*it)->GetObjectType()).str()
            );

            continue;
        }

        Logger::LogEvent(
                Logger::ERROR,
                (boost::format("[%s] -> --------Reading %s %s %s") % parentDevName %
                 CGXDLMSClient::ObjectTypeToString((*it)->GetObjectType()).c_str() %
                 (*it)->GetName().ToString().c_str() % (*it)->GetDescription().c_str()).str()
        );

        std::vector<int> attributes;
        (*it)->GetAttributeIndexToRead(attributes);

        for (std::vector<int>::iterator pos = attributes.begin(); pos != attributes.end(); ++pos) {
            value.clear();
            if ((ret = Read(*it, *pos, value, value_unit)) != DLMS_ERROR_CODE_OK) {
                Logger::LogEvent(
                        Logger::INFO,
                        (boost::format("[%s] -> Error! Index: %d %s") % parentDevName % *pos %
                         CGXDLMSConverter::GetErrorMessage(ret)).str()
                );
                //Continue reading.
            } else {

                std::string s_valueUnit;
                switch (value_unit) {
                    case 0:
                        s_valueUnit.assign("NONE");
                        break;
                    case 3:
                        s_valueUnit.assign("BOOLEAN");
                        break;
                    case 4:
                        s_valueUnit.assign("BIT_STRING");
                        break;
                    case 5:
                        s_valueUnit.assign("INT32");
                        break;
                    case 6:
                        s_valueUnit.assign("UINT32");
                        break;
                    case 9:
                        s_valueUnit.assign("OCTET_STRING");
                        break;
                    case 10:
                        s_valueUnit.assign("STRING");
                        break;
                    case 13:
                        s_valueUnit.assign("BINARY_CODED_DECIMAL");
                        break;
                    case 12:
                        s_valueUnit.assign("STRING_UTF8");
                        break;
                    case 15:
                        s_valueUnit.assign("INT8");
                        break;
                    case 16:
                        s_valueUnit.assign("INT16");
                        break;
                    case 17:
                        s_valueUnit.assign("UINT8");
                        break;
                    case 18:
                        s_valueUnit.assign("UINT16");
                        break;
                    case 20:
                        s_valueUnit.assign("INT64");
                        break;
                    case 21:
                        s_valueUnit.assign("UINT64");
                        break;
                    case 22:
                        s_valueUnit.assign("ENUM");
                        break;
                    case 23:
                        s_valueUnit.assign("FLOAT32");
                        break;
                    case 24:
                        s_valueUnit.assign("FLOAT64");
                        break;
                    case 25:
                        s_valueUnit.assign("DATETIME");
                        break;
                    case 26:
                        s_valueUnit.assign("DATE");
                        break;
                    case 27:
                        s_valueUnit.assign("TIME");
                        break;
                    case 1:
                        s_valueUnit.assign("ARRAY");
                        break;
                    case 2:
                        s_valueUnit.assign("STRUCTURE");
                        break;
                    case 19:
                        s_valueUnit.assign("COMPACT_ARRAY");
                        break;

                }

                Logger::LogEvent(
                        Logger::ERROR,
                        (boost::format("[%s] -> Index: %d Value: %s [%s(%d)]") % parentDevName % *pos % value.c_str() %
                         s_valueUnit.c_str() % value_unit).str()
                );
            }
        }

    }

    return ret;
}

int CGXCommunication::GetProfileGenerics() {
    Logger::LogEvent(
            Logger::ERROR,
            (boost::format("[%s] -> GetProfileGenerics") % parentDevName).str()
    );

    char buff[200];
    int ret = 0;
    std::string str;
    std::string value;

    //Find profile generics and read them.
    CGXDLMSObjectCollection pgs;
    m_Parser->GetObjects().GetObjects(DLMS_OBJECT_TYPE_PROFILE_GENERIC, pgs);
    for (std::vector<CGXDLMSObject *>::iterator it = pgs.begin(); it != pgs.end(); ++it) {

        Logger::LogEvent(
                Logger::ERROR,
                (boost::format("[%s] -> --------Reading %s %s %s") % parentDevName %
                 CGXDLMSClient::ObjectTypeToString((*it)->GetObjectType()).c_str() %
                 (*it)->GetName().ToString().c_str() % (*it)->GetDescription().c_str()).str()
        );

        if ((ret = Read(*it, 7, value)) != DLMS_ERROR_CODE_OK) {
            Logger::LogEvent(
                    Logger::INFO,
                    (boost::format("[%s] -> Error! Index: %d %s") % parentDevName % 7 %
                     CGXDLMSConverter::GetErrorMessage(ret)).str()
            );

            //WriteValue(GX_TRACE_LEVEL_ERROR, buff);
            //Continue reading.
        }

        std::string entriesInUse = value;
        if ((ret = Read(*it, 8, value)) != DLMS_ERROR_CODE_OK) {

            Logger::LogEvent(
                    Logger::INFO,
                    (boost::format("[%s] -> Error! Index: %d %s") % parentDevName % 8 %
                     CGXDLMSConverter::GetErrorMessage(ret)).str()
            );
            //WriteValue(GX_TRACE_LEVEL_ERROR, buff);
            //Continue reading.
        }

        std::string entries = value;
        str = "Entries: ";
        str += entriesInUse;
        str += "/";
        str += entries;
        Logger::LogEvent(
                Logger::ERROR,
                (boost::format("[%s] -> %s") % parentDevName % str).str()
        );

        //If there are no columns or rows.
        if (((CGXDLMSProfileGeneric *) *it)->GetEntriesInUse() == 0 ||
            ((CGXDLMSProfileGeneric *) *it)->GetCaptureObjects().size() == 0)
            continue;

        //All meters are not supporting parameterized read.
        CGXDLMSVariant rows;
        if ((m_Parser->GetNegotiatedConformance() &
             (DLMS_CONFORMANCE_PARAMETERIZED_ACCESS | DLMS_CONFORMANCE_SELECTIVE_ACCESS)) != 0) {
            //Read first row from Profile Generic.
            if ((ret = ReadRowsByEntry((CGXDLMSProfileGeneric *) *it, 1, 1, rows)) != 0) {
                str = "Error! Failed to read first row:";
                str += CGXDLMSConverter::GetErrorMessage(ret);
                Logger::LogEvent(
                        Logger::INFO,
                        (boost::format("[%s] -> %s") % parentDevName % str).str()
                );
                //Continue reading.
            } else {
                //////////////////////////////////////////////////////////////////////////////
                //Show rows.
                Logger::LogEvent(
                        Logger::ERROR,
                        (boost::format("[%s] -> %s") % parentDevName % rows.ToString()).str()
                );
            }
        }

        //All meters are not supporting parameterized read.
        if ((m_Parser->GetNegotiatedConformance() &
             (DLMS_CONFORMANCE_PARAMETERIZED_ACCESS | DLMS_CONFORMANCE_SELECTIVE_ACCESS)) != 0) {
            CGXDateTime start = CGXDateTime::Now();
            start.ResetTime();
            CGXDateTime end = CGXDateTime::Now();

            if ((ret = ReadRowsByRange((CGXDLMSProfileGeneric *) (*it), start, end, rows)) != 0) {
                str = "Error! Failed to read last day:";
                str += CGXDLMSConverter::GetErrorMessage(ret);
                Logger::LogEvent(
                        Logger::INFO,
                        (boost::format("[%s] -> %s") % parentDevName % str).str()
                );
                //Continue reading.
            } else {
                //////////////////////////////////////////////////////////////////////////////
                //Show rows.
                Logger::LogEvent(
                        Logger::ERROR,
                        (boost::format("[%s] -> %s") % parentDevName % rows.ToString()).str()
                );
            }
        }
    }

    return ret;
}

int CGXCommunication::ReadAll(void) {
    int ret;
    if ((ret = InitializeConnection()) == 0) {
        // Get list of objects that meter supports.
        if ((ret = GetAssociationView()) != 0) {
            Logger::LogEvent(
                    Logger::INFO,
                    (boost::format("[%s] -> GetAssociationView failed (%d) %s") % parentDevName % ret %
                     CGXDLMSConverter::GetErrorMessage(ret)).str()
            );
        }
        //return 1;
        // Read Scalers and units from the register objects.
        if (ret == 0 && (ret = ReadScalerAndUnits()) != 0)\
 {
            Logger::LogEvent(
                    Logger::INFO,
                    (boost::format("[%s] -> ReadScalerAndUnits failed (%d) %s") % parentDevName % ret %
                     CGXDLMSConverter::GetErrorMessage(ret)).str()
            );
        }

        // Read Profile Generic columns.
        if (ret == 0 && (ret = GetProfileGenericColumns()) != 0) {
            Logger::LogEvent(
                    Logger::INFO,
                    (boost::format("[%s] -> GetProfileGenericColumns failed (%d) %s") % parentDevName % ret %
                     CGXDLMSConverter::GetErrorMessage(ret)).str()
            );
        }

        if (ret == 0 && (ret = GetReadOut()) != 0) {
            Logger::LogEvent(
                    Logger::INFO,
                    (boost::format("[%s] -> GetReadOut failed (%d) %s") % parentDevName % ret %
                     CGXDLMSConverter::GetErrorMessage(ret)).str()
            );
        }

        // Read historical data.
        if (ret == 0 && (ret = GetProfileGenerics()) != 0) {
            Logger::LogEvent(
                    Logger::INFO,
                    (boost::format("[%s] -> GetProfileGenerics failed (%d) %s") % parentDevName % ret %
                     CGXDLMSConverter::GetErrorMessage(ret)).str()
            );
        }


    }

    return ret;
}

