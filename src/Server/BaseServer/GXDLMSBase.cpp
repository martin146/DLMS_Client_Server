/**
 * @file GXDLMSBase.cpp
 * @author Filip Grepl
 * @brief Implementation of 'GXDLMSBase' class
 * @date 2021-10-08
 *
 * @copyright Copyright (c) 2020
 *
 */


//
// --------------------------------------------------------------------------
//  Gurux Ltd
//
//
//
// Filename:        $HeadURL$
//
// Version:         $Revision$,
//                  $Date$
//                  $Author$
//
// Copyright (c) Gurux Ltd
//
//---------------------------------------------------------------------------
//
//  DESCRIPTION
//
// This file is a part of Gurux ServerConfiguration Framework.
//
// Gurux ServerConfiguration Framework is Open Source software; you can redistribute it
// and/or modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; version 2 of the License.
// Gurux ServerConfiguration Framework is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// See the GNU General Public License for more details.
//
// More information of Gurux products: http://www.gurux.org
//
// This code is licensed under the GNU General Public License v2.
// Full text may be retrieved at http://www.gnu.org/licenses/gpl-2.0.txt
//---------------------------------------------------------------------------

#include <cstdio>
#if defined(_WIN32) || defined(_WIN64)//Windows includes
#include <tchar.h>
#include <conio.h>
#include <Winsock.h> //Add support for sockets
#include <time.h>
#include <process.h>//Add support for threads
#else //Linux includes.
#define closesocket close
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h> //Add support for sockets
#include <unistd.h> //Add support for sockets
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <Exceptions/RuntimeException.h>
#include <Exceptions/InitException.h>
#include <Exceptions/HandleException.h>
#include <Logger.h>
#include <GURUX_DLMS/ExtendedObjects/GXDLMSRegisterE.h>
#include <GURUX_DLMS/ExtendedObjects/GXDLMSSecuritySetupE.h>

#endif

#include "BaseServer/GXDLMSBase.h"

#include "GURUX_DLMS/GXTime.h"
#include "GURUX_DLMS/GXDate.h"
#include "GURUX_DLMS/Client/GXDLMSClient.h"
#include "GURUX_DLMS/Objects/ParametersAndMeasurementData/GXDLMSData.h"
#include "GURUX_DLMS/Objects/ParametersAndMeasurementData/GXDLMSRegister.h"
#include "GURUX_DLMS/Objects/TimeAndEventBoundControl/GXDLMSClock.h"
#include "GURUX_DLMS/Objects/SettingUpDataExchangeInternet/GXDLMSTcpUdpSetup.h"
#include "GURUX_DLMS/Objects/ParametersAndMeasurementData/GXDLMSProfileGeneric.h"
#include "GURUX_DLMS/Objects/SettingUpDataExchangeLocalPortsModems/GXDLMSAutoConnect.h"
#include "GURUX_DLMS/Objects/SettingUpDataExchangeLocalPortsModems/GXDLMSIECOpticalPortSetup.h"
#include "GURUX_DLMS/Objects/TimeAndEventBoundControl/GXDLMSActivityCalendar.h"
#include "GURUX_DLMS/Objects/ParametersAndMeasurementData/GXDLMSDemandRegister.h"
#include "GURUX_DLMS/Objects/TimeAndEventBoundControl/GXDLMSRegisterMonitor.h"
#include "GURUX_DLMS/Objects/TimeAndEventBoundControl/GXDLMSActionSchedule.h"
#include "GURUX_DLMS/Objects/AccessControlAndManagement/GXDLMSSapAssignment.h"
#include "GURUX_DLMS/Objects/SettingUpDataExchangeLocalPortsModems/GXDLMSAutoAnswer.h"
#include "GURUX_DLMS/Objects/SettingUpDataExchangeLocalPortsModems/GXDLMSModemConfiguration.h"
#include "GURUX_DLMS/Objects/SettingUpDataExchangeInternet/GXDLMSMacAddressSetup.h"
#include "GURUX_DLMS/GXDLMSModemInitialisation.h"
#include "GURUX_DLMS/Objects/TimeAndEventBoundControl/GXDLMSActionSet.h"
#include "GURUX_DLMS/Objects/SettingUpDataExchangeInternet/GXDLMSIp4Setup.h"
#include "GURUX_DLMS/Objects/AccessControlAndManagement/GXDLMSPushSetup.h"
#include "GURUX_DLMS/Objects/AccessControlAndManagement/GXDLMSAssociationLogicalName.h"
#include "GURUX_DLMS/Objects/AccessControlAndManagement/GXDLMSAssociationShortName.h"
#include "GURUX_DLMS/Objects/TimeAndEventBoundControl/GXDLMSSchedule.h"

using namespace std;

/////////////////////////////////////////////////////////////////////////
// Constructor.
/////////////////////////////////////////////////////////////////////////
GXDLMSBase::GXDLMSBase(
        bool isUseLogicalNameReferencing,
        DLMS_INTERFACE_TYPE interfaceType,
        LoadedUserConfiguration &loadedUserConfiguration) :
        CGXDLMSSecureServer(isUseLogicalNameReferencing, interfaceType), loadedUserConfiguration(loadedUserConfiguration)//,
        //hatelCommunication(loadedUserConfiguration, this->GetItems(), objectAccessMutex)
{
#if defined(_WIN32) || defined(_WIN64)//If Windows
    m_ReceiverThread = INVALID_HANDLE_VALUE;
        m_ServerSocket = INVALID_SOCKET;
#else //If Linux.
    m_ServerSocket = -1;
#endif
    //SetMaxReceivePDUSize(1024);
}

/////////////////////////////////////////////////////////////////////////
// Constructor.
/////////////////////////////////////////////////////////////////////////
GXDLMSBase::GXDLMSBase(
        CGXDLMSIecHdlcSetup* hdlc,
        LoadedUserConfiguration &loadedUserConfiguration) :
    CGXDLMSSecureServer(hdlc), loadedUserConfiguration(loadedUserConfiguration)//,
    //hatelCommunication(loadedUserConfiguration, this->GetItems(), objectAccessMutex)
{
#if defined(_WIN32) || defined(_WIN64)//If Windows
    m_ReceiverThread = INVALID_HANDLE_VALUE;
        m_ServerSocket = INVALID_SOCKET;
#else //If Linux.
    m_ServerSocket = -1;
#endif
    //SetMaxReceivePDUSize(1024);
}

/////////////////////////////////////////////////////////////////////////
// Constructor.
/////////////////////////////////////////////////////////////////////////
GXDLMSBase::GXDLMSBase(
        CGXDLMSTcpUdpSetup* wrapper,
        LoadedUserConfiguration &loadedUserConfiguration) :
    CGXDLMSSecureServer(wrapper), loadedUserConfiguration(loadedUserConfiguration)//,
//    hatelCommunication(loadedUserConfiguration, this->GetItems(), objectAccessMutex)
{
#if defined(_WIN32) || defined(_WIN64)//If Windows
    m_ReceiverThread = INVALID_HANDLE_VALUE;
        m_ServerSocket = INVALID_SOCKET;
#else //If Linux.
    m_ServerSocket = -1;
#endif
    //SetMaxReceivePDUSize(1024);
}

/////////////////////////////////////////////////////////////////////////
// Constructor.
/////////////////////////////////////////////////////////////////////////
GXDLMSBase::GXDLMSBase(
        CGXDLMSAssociationShortName* sn,
        CGXDLMSIecHdlcSetup* hdlc,
        LoadedUserConfiguration &loadedUserConfiguration) :
    CGXDLMSSecureServer(sn, hdlc), loadedUserConfiguration(loadedUserConfiguration)//,
//    hatelCommunication(loadedUserConfiguration, this->GetItems(), objectAccessMutex)
{
#if defined(_WIN32) || defined(_WIN64)//If Windows
    m_ReceiverThread = INVALID_HANDLE_VALUE;
        m_ServerSocket = INVALID_SOCKET;
#else //If Linux.
    m_ServerSocket = -1;
#endif
    //SetMaxReceivePDUSize(1024);
}

/////////////////////////////////////////////////////////////////////////
// Constructor.
/////////////////////////////////////////////////////////////////////////
GXDLMSBase::GXDLMSBase(
        CGXDLMSAssociationShortName* sn,
        CGXDLMSTcpUdpSetup* wrapper,
        LoadedUserConfiguration &loadedUserConfiguration) :
    CGXDLMSSecureServer(sn, wrapper), loadedUserConfiguration(loadedUserConfiguration)//,
//    hatelCommunication(loadedUserConfiguration, this->GetItems(), objectAccessMutex)
{
#if defined(_WIN32) || defined(_WIN64)//If Windows
    m_ReceiverThread = INVALID_HANDLE_VALUE;
        m_ServerSocket = INVALID_SOCKET;
#else //If Linux.
    m_ServerSocket = -1;
#endif
    //SetMaxReceivePDUSize(1024);
}

/////////////////////////////////////////////////////////////////////////
// Destructor.
/////////////////////////////////////////////////////////////////////////
GXDLMSBase::~GXDLMSBase() {
    StopServer();
    pthread_mutex_destroy(&objectAccessMutex);
    pthread_mutex_destroy(&endMutex);
    pthread_cond_destroy(&endCondition);
}


/////////////////////////////////////////////////////////////////////////
// thread that listens to the clients connection.
/////////////////////////////////////////////////////////////////////////
void Listener(void* pVoid)
{
    CGXByteBuffer reply;
    auto* server = (GXDLMSBase*)pVoid;
    sockaddr_in add = { 0 };
    long ret;
    char tmp[10];
    CGXByteBuffer bb;
    bb.Capacity(2048);
#if defined(_WIN32) || defined(_WIN64)//If Windows
    int len;
    int AddrLen = sizeof(add);
    SOCKET socket;
#else //If Linux
    socklen_t len;
    socklen_t AddrLen = sizeof(add);
    int socket;
#endif
    struct sockaddr_in client;
    memset(&client, 0, sizeof(client));
    //Get buffer data
    basic_string<char> senderInfo;
    while (server->IsOpen())
    {
        len = sizeof(client);
        senderInfo.clear();

        socket = accept(server->GetSocket(), (struct sockaddr*) & client, &len);

        server->Reset();
        if (server->IsOpen())
        {
            server->Reset();
            if ((ret = getpeername(socket, (sockaddr*)&add, &AddrLen)) == -1)
            {
                closesocket(socket);
#if defined(_WIN32) || defined(_WIN64)//If Windows
                socket = INVALID_SOCKET;
#else //If Linux
                socket = -1;
#endif
                continue;
                //Notify error.
            }
            senderInfo = inet_ntoa(add.sin_addr);
            senderInfo.append(":");
#if _MSC_VER > 1000
            _ltoa_s(add.sin_port, tmp, 10, 10);
#else
            sprintf(tmp, "%d", add.sin_port);
#endif
            senderInfo.append(tmp);
            while (server->IsOpen())
            {
                //If client is left wait for next client.
                if ((ret = recv(socket, (char*)
                    bb.GetData() + bb.GetSize(),
                    bb.Capacity() - bb.GetSize(), 0)) == -1)
                {
                    //Notify error.
                    server->Reset();
#if defined(_WIN32) || defined(_WIN64)//If Windows
                    closesocket(socket);
                    socket = INVALID_SOCKET;
#else //If Linux
                    close(socket);
                    socket = -1;
#endif
                    break;
                }
                //If client is closed the connection.
                if (ret == 0)
                {
                    server->Reset();
#if defined(_WIN32) || defined(_WIN64)//If Windows
                    closesocket(socket);
                    socket = INVALID_SOCKET;
#else //If Linux
                    close(socket);
                    socket = -1;
#endif
                    break;
                }
                bb.SetSize(bb.GetSize() + ret);
                if (server->m_Trace == GX_TRACE_LEVEL_VERBOSE)
                {
                    printf("RX:\t%s\r\n", bb.ToHexString().c_str());
                }
                try {
                    if (server->HandleRequest(bb, reply) != 0) {
#if defined(_WIN32) || defined(_WIN64)//If Windows
                        closesocket(socket);
                        socket = INVALID_SOCKET;
#else //If Linux
                        close(socket);
                        socket = -1;
#endif
                    }
                    bb.SetSize(0);
                    if (reply.GetSize() != 0) {
                        if (server->m_Trace == GX_TRACE_LEVEL_VERBOSE) {
                            printf("TX:\t%s\r\n", reply.ToHexString().c_str());
                        }
                        if (send(socket, (const char *) reply.GetData(), reply.GetSize() - reply.GetPosition(), 0) ==
                            -1) {
                            //If error has occured
                            server->Reset();
#if defined(_WIN32) || defined(_WIN64)//If Windows
                            closesocket(socket);
                            socket = INVALID_SOCKET;
#else //If Linux
                            close(socket);
                            socket = -1;
#endif
                        }
                        reply.Clear();
                    }
                } catch (RuntimeException &e) {
                    Logger::LogEvent(Logger::ERROR, e.what());
                    close(socket);
                    socket = -1;
                    reply.Clear();
                }
            }
            server->Reset();
        }
    }
}

void endListening(ProfileGenericWriterStruct *writerStruct, const std::string& errDescription) {
    Logger::LogEvent(Logger::ERROR, errDescription);
    shutdown(*writerStruct->m_ServerSocket, SHUT_RDWR);
    *writerStruct->m_ServerSocket = -1;
}

void* ProfileGenericWriterThread(void* pVoid) {
    auto *writerStruct = (ProfileGenericWriterStruct*) pVoid;
    std::string pathToFile =  GXDLMSBase::CreatePathToProfileGenericFile(
            writerStruct->loadedUserConfiguration->getDlmsServerPathsConfiguration().getPathToSaveProfileGenericData(), *writerStruct->profileGeneric);

    int ret;
    struct timespec waitTime = {0, 0};

    while (true) {
        pthread_mutex_lock(writerStruct->objectAccessMutex);
        if (*writerStruct->endFlag)  {
            pthread_mutex_unlock(writerStruct->objectAccessMutex);
            return nullptr;
        }

        try {
            GXDLMSBase::Capture(*writerStruct);
        } catch (HandleException &e) {
            endListening(writerStruct, e.what());
            pthread_mutex_unlock(writerStruct->objectAccessMutex);
            return nullptr;
        }
        pthread_mutex_unlock(writerStruct->objectAccessMutex);

        if ((ret = clock_gettime(CLOCK_REALTIME, &waitTime))) {
            endListening(writerStruct, "Error in getting system time with return value: " + std::to_string(ret) + ".");
            return nullptr;
        }

        waitTime.tv_sec += writerStruct->profileGeneric->GetCapturePeriod();

        pthread_mutex_lock(writerStruct->endMutex);
        ret = pthread_cond_timedwait(writerStruct->endCondition, writerStruct->endMutex, &waitTime); // return value does not matter
        if (ret != ETIMEDOUT && ret != 0) {
            endListening(writerStruct, "Error in waiting in profile generic thread. Pthread_cond_timedwait return value is : " + std::to_string(ret));
            return nullptr;
        }
        pthread_mutex_unlock(writerStruct->endMutex);
    }
    return nullptr;
}

bool GXDLMSBase::IsOpen() const
{
#if defined(_WIN32) || defined(_WIN64)//If Windows
    return m_ServerSocket != INVALID_SOCKET;
#else //If Linux
    return m_ServerSocket != -1;
#endif
}

int GXDLMSBase::GetSocket() const
{
    return (int)m_ServerSocket;
}

void GXDLMSBase::StartServer()
{
    Logger::LogEvent(Logger::INFO, "Starting server");
    SetPushClientAddress(60);
    int ret = 0;

    m_ServerSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (!IsOpen())
    {
        //socket creation.
        throw InitException("Cannot create server socket");
    }
    int fFlag = 1;
    if (setsockopt(m_ServerSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&fFlag, sizeof(fFlag)) == -1)
    {
        //setsockopt.
        throw InitException("Cannot set reused flag on socket.");
    }
    sockaddr_in add = { 0 };
    add.sin_port = htons(loadedUserConfiguration.getLoadedObjects().getTcpUdpSetup()->GetPort());

    std::string ipv4SetupIpAddress = loadedUserConfiguration.getLoadedObjects().getTcpUdpSetup()->GetIPReference();
    std::string IPaddress;
    ((CGXDLMSIp4Setup *) this->GetItems().FindByLN(DLMS_OBJECT_TYPE_IP4_SETUP, ipv4SetupIpAddress))->GetIPAddress(IPaddress);

    add.sin_addr.s_addr = inet_addr(IPaddress.c_str());
    add.sin_family = AF_INET;

    if ((ret = ::bind(m_ServerSocket, (sockaddr*)&add, sizeof(add))) == -1)
    {
        //bind;
        throw InitException("Cannot bind listening socket to port " + std::to_string(loadedUserConfiguration.getLoadedObjects().getTcpUdpSetup()->GetPort()));
    }
    if ((ret = listen(m_ServerSocket, 1)) == -1) {
        //socket listen failed.
        throw InitException("Cannot start listening on port " + std::to_string(loadedUserConfiguration.getLoadedObjects().getTcpUdpSetup()->GetPort()));
    }


    // Start saving data for profile generic objects
    for (auto &&object : this->GetItems()) {
        if (object->GetObjectType() == DLMS_OBJECT_TYPE_PROFILE_GENERIC) {
            std::shared_ptr<ProfileGenericWriterStruct> writerStruct = std::make_shared<ProfileGenericWriterStruct>();
            writerStruct->serverObjects = &this->GetItems();
            writerStruct->loadedUserConfiguration = &this->loadedUserConfiguration;
            writerStruct->profileGeneric = (CGXDLMSProfileGeneric *) object;
            writerStruct->objectAccessMutex = &this->objectAccessMutex;
            writerStruct->endMutex = &this->endMutex;
            writerStruct->endCondition = &this->endCondition;
            writerStruct->endFlag = &this->endFlag;
            writerStruct->m_ServerSocket = &this->m_ServerSocket;

            pthread_create(&writerStruct->writerThread, nullptr, ProfileGenericWriterThread,
                           (void *) writerStruct.get());
            this->m_ProfileGenericThreads.push_back(writerStruct);
        }
    }

    //ret = pthread_create(&m_ReceiverThread, NULL, Listener, (void*)this);

    Listener((void*)this);

    StopServer(); // stop generic threads and hatel communinication thread
}

void GXDLMSBase::StopServer()
{
    Logger::LogEvent(Logger::INFO, "Stopping server");
    // stop hatel communication
    int ret;
//    if ((ret = hatelCommunication.StopHatelCommunication()) != 0) {
//        Logger::LogEvent(Logger::ERROR, "Error in stopping hatel communication. Error code is: " + std::to_string(ret));
//    }
    // stop listening to connections
    if (IsOpen())
    {
        shutdown(m_ServerSocket, SHUT_RDWR);
        m_ServerSocket = -1;
    }
    // stop generic threads
    pthread_mutex_lock(&objectAccessMutex);
    this->endFlag = true;
    pthread_mutex_unlock(&objectAccessMutex);
    pthread_mutex_lock(&this->endMutex);
    pthread_cond_broadcast(&this->endCondition);
    pthread_mutex_unlock(&this->endMutex);
    for (auto &&writerThreadStruct : this->m_ProfileGenericThreads)
        pthread_join(writerThreadStruct->writerThread, nullptr);
}

///////////////////////////////////////////////////////////////////////
// Method, that returns IP address of the server.
///////////////////////////////////////////////////////////////////////
int GetIpAddress(std::string& address)
{
    int ret;
    struct hostent* phe;
    char ac[80];
    if ((ret = gethostname(ac, sizeof(ac))) == 0)
    {
        phe = gethostbyname(ac);
        if (phe == nullptr)
        {
            ret = -1;
        }
        else
        {
            auto* addr = (struct in_addr*)phe->h_addr_list[0];
            address = inet_ntoa(*addr);
        }
    }
    return ret;
}


///////////////////////////////////////////////////////////////////////
// Method, that initializes the server.
///////////////////////////////////////////////////////////////////////
void GXDLMSBase::Init(GX_TRACE_LEVEL trace)
{
    int ret;
    m_Trace = trace;

    /*CGXByteBuffer kek;
    kek.AddString("12345678");
    SetKek(kek);*/


    unsigned long sn = 123456;

    //AddElectricityID1(GetItems(), sn);
    //AddElectricityID2(GetItems(), sn);

    //Add default clock. Clock's Logical Name is 0.0.1.0.0.255.
    /* CGXDLMSClock* pClock = new CGXDLMSClock();
    *CGXDateTime begin(-1, 9, 1, -1, -1, -1, -1);
     pClock->SetBegin(begin);
     CGXDateTime end(-1, 3, 1, -1, -1, -1, -1);
     pClock->SetEnd(end);
     pClock->SetTimeZone(CGXDateTime::GetCurrentTimeZone());
     pClock->SetDeviation(CGXDateTime::GetCurrentDeviation());
     GetItems().push_back(pClock);*/

/*
    ///////////////////////////////////////////////////////////////////////
    //Add Auto connect object.
    AddAutoConnect(GetItems());

    ///////////////////////////////////////////////////////////////////////
    //Add Activity Calendar object.
    AddActivityCalendar(GetItems());

    ///////////////////////////////////////////////////////////////////////
    //Add Optical Port Setup object.
    AddOpticalPortSetup(GetItems());
    ///////////////////////////////////////////////////////////////////////
    //Add Demand Register object.
    AddDemandRegister(GetItems());

    ///////////////////////////////////////////////////////////////////////
    //Add Register Monitor object.
    AddRegisterMonitor(GetItems(), pRegister);

    ///////////////////////////////////////////////////////////////////////
    //Add action schedule object.
    AddActionSchedule(GetItems());

    ///////////////////////////////////////////////////////////////////////
    //Add SAP Assignment object.
    AddSapAssignment(GetItems());

    ///////////////////////////////////////////////////////////////////////
    //Add Auto Answer object.
    AddAutoAnswer(GetItems());

    ///////////////////////////////////////////////////////////////////////
    //Add Modem Configuration object.
    AddModemConfiguration(GetItems());

    ///////////////////////////////////////////////////////////////////////
    //Add Mac Address Setup object.
    AddMacAddressSetup(GetItems());
    ///////////////////////////////////////////////////////////////////////
    //Add IP4 Setup object.
    CGXDLMSIp4Setup* pIp4 = AddIp4Setup(GetItems(), address);

    ///////////////////////////////////////////////////////////////////////
    //Add Push Setup object.
    CGXDLMSPushSetup* pPush = new CGXDLMSPushSetup();
    address = "192.168.50.24:4059";
    pPush->SetDestination(address);
    GetItems().push_back(pPush);

    // Add push object itself. This is needed to tell structure of data to
    // the Push listener.
    pPush->GetPushObjectList().push_back(std::pair<CGXDLMSObject*, CGXDLMSCaptureObject>(pPush, CGXDLMSCaptureObject(2, 0)));
    //Add logical device name.
    pPush->GetPushObjectList().push_back(std::pair<CGXDLMSObject*, CGXDLMSCaptureObject>(ldn, CGXDLMSCaptureObject(2, 0)));
    // Add 0.0.25.1.0.255 Ch. 0 IPv4 setup IP address.
    pPush->GetPushObjectList().push_back(std::pair<CGXDLMSObject*, CGXDLMSCaptureObject>(pIp4, CGXDLMSCaptureObject(3, 0)));

    ///////////////////////////////////////////////////////////////////////
    //Add image transfer object.
    CGXDLMSImageTransfer* image = new CGXDLMSImageTransfer();
    GetItems().push_back(image);
    ///////////////////////////////////////////////////////////////////////
    //Add script table object.
    CGXDLMSScriptTable* st = new CGXDLMSScriptTable();
    GetItems().push_back(st);

    ///////////////////////////////////////////////////////////////////////
    //Add Schedule object.
    CGXDLMSSchedule* schedule = new CGXDLMSSchedule();
    GetItems().push_back(schedule);
    ///////////////////////////////////////////////////////////////////////
     */

    for (auto &&o : this->loadedUserConfiguration.getLoadedObjects()) this->GetItems().push_back(o);

    this->endFlag = false;
    pthread_mutex_init(&objectAccessMutex, nullptr);
    pthread_mutex_init(&endMutex, nullptr);
    pthread_cond_init(&endCondition, nullptr);

//    try {
//        hatelCommunication.Initialize();
//    } catch (HatelCommunicationException &e) {
//        hatelCommunication.StopHatelCommunication();
//        throw HatelCommunicationException(e.what());
//    }

    //Server must initialized after all objects are added.
    ret = Initialize();
    if (ret != DLMS_ERROR_CODE_OK)
    {
        throw InitException("Initializing of server is failed. Error code is " + std::to_string(ret) + ".");
    }
}

///////////////////////////////////////////////////////////////////////
// Method, that is called when the object is not found by "FindByLN" method.
///////////////////////////////////////////////////////////////////////
CGXDLMSObject* GXDLMSBase::FindObject(
    DLMS_OBJECT_TYPE objectType,
    int sn,
    std::string& ln)
{
    return nullptr;
}

///////////////////////////////////////////////////////////////////////
// Method, that adds profile generic data from file to the profile generic buffer using start index and number of items.
///////////////////////////////////////////////////////////////////////
void GXDLMSBase::GetProfileGenericDataByEntry(CGXDLMSProfileGeneric &profileGeneric, const long index, const long count)
{
    profileGeneric.GetBuffer().clear();
    pugi::xml_document doc = GXDLMSBase::LoadProfileGenericData(
            this->loadedUserConfiguration.getDlmsServerPathsConfiguration().getPathToSaveProfileGenericData(), profileGeneric);
    pugi::xml_node rootNode = doc.child("Root");
    if (!rootNode) return;

    int i = 0;
    for (auto &&record : rootNode.children()) {
        if (profileGeneric.GetBuffer().size() == count) break;
        if (i >= index) {
            std::vector<CGXDLMSVariant> values;

            for (auto &&item : record.children()) {
                if (item.name() == std::string("Register"))
                    values.emplace_back(stoll(item.text().get()));
                else if (item.name() == std::string("Clock")) {
                    int month = 0, day = 0, year = 0, hour = 0, minute = 0, second = 0;
                    sscanf(item.text().get(), "%d/%d/%d %d:%d:%d", &month, &day, &year, &hour, &minute, &second);
                    CGXDateTime tm(2000 + year, month, day, hour, minute, second, 0, 0x8000);
                    values.emplace_back(tm);
                } else
                    throw HandleException("Profile generic file " + doc.path() + " contains unsupported type of object " + item.name() + " !");
            }
            profileGeneric.GetBuffer().push_back(values);
        }
        i++;
    }
}

///////////////////////////////////////////////////////////////////////
// Method, that adds profile generic data from file to the profile generic buffer using start and end date time.
///////////////////////////////////////////////////////////////////////
void GXDLMSBase::GetProfileGenericDataByRange(CGXDLMSProfileGeneric &profileGeneric, CGXDLMSValueEventArg &e)
{

    CGXDLMSVariant start, end;
    CGXByteBuffer bb;
    bb.Set(e.GetParameters().Arr[1].byteArr, e.GetParameters().Arr[1].size);
    CGXDLMSClient::ChangeType(bb, DLMS_DATA_TYPE_DATETIME, start);

    bb.Clear();
    bb.Set(e.GetParameters().Arr[2].byteArr, e.GetParameters().Arr[2].size);
    CGXDLMSClient::ChangeType(bb, DLMS_DATA_TYPE_DATETIME, end);

    start.dateTime.GetValue().tm_isdst = 0;
    end.dateTime.GetValue().tm_isdst = 0;

    Logger::LogEvent(Logger::INFO, "Loading profile generic data from date: " + start.ToString() + " to " + end.ToString());


    pugi::xml_document doc = GXDLMSBase::LoadProfileGenericData(
            this->loadedUserConfiguration.getDlmsServerPathsConfiguration().getPathToSaveProfileGenericData(), profileGeneric);
    pugi::xml_node rootNode = doc.child("Root");

    if (!rootNode) return;

    for (auto &&record : rootNode.children()) {
        for (auto &&item : record.children()) {
            if (item.name() == std::string("Clock")) {
                int month = 0, day = 0, year = 0, hour = 0, minute = 0, second = 0;
                sscanf(item.text().get(), "%d/%d/%d %d:%d:%d", &month, &day, &year, &hour, &minute,
                       &second);
                CGXDateTime tm(2000 + year, month, day, hour, minute, second, 0, 0x8000);

                // If all data is read -> return
                if (tm.CompareTo(end.dateTime) > 0) return;

                // Finding first item
                if (tm.CompareTo(start.dateTime) < 0) e.SetRowBeginIndex(e.GetRowBeginIndex() + 1);

                e.SetRowEndIndex(e.GetRowEndIndex() + 1);
                break;
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////
// Method, that returns number of records in file with profile generic data.
///////////////////////////////////////////////////////////////////////
long GXDLMSBase::GetProfileGenericDataCount(CGXDLMSProfileGeneric &profileGeneric) {

    pugi::xml_document doc = GXDLMSBase::LoadProfileGenericData(
            this->loadedUserConfiguration.getDlmsServerPathsConfiguration().getPathToSaveProfileGenericData(), profileGeneric);
    pugi::xml_node rootNode = doc.child("Root");

    if (!rootNode)  return 0;
    else            return std::distance(rootNode.children("Record").begin(), rootNode.children("Record").end());
}


/////////////////////////////////////////////////////////////////////////////
// Method, that is called before some object is read.
/////////////////////////////////////////////////////////////////////////////
void GXDLMSBase::PreRead(std::vector<CGXDLMSValueEventArg*>& args)
{
    CGXDLMSVariant value;
    CGXDLMSObject* pObj;
    int ret, index;
    DLMS_OBJECT_TYPE type;
    pthread_mutex_lock(&objectAccessMutex);
    for (auto &&it : args)
    {
        //Let framework handle Logical Name read.
        if (it->GetIndex() == 1)
        {
            continue;
        }
        //Get attribute index.
        index = it->GetIndex();
        pObj = it->GetTarget();
        Logger::LogEvent(Logger::INFO, "Reading object: " + pObj->GetLogicalName());
        //Get target type.
        type = pObj->GetObjectType();
        if (type == DLMS_OBJECT_TYPE_PROFILE_GENERIC)
        {
            auto* p = (CGXDLMSProfileGeneric*) pObj;
            // If buffer is read and we want to save memory.
            if (index == 7)
            {
                // If client wants to know EntriesInUse.
                p->SetEntriesInUse(this->GetProfileGenericDataCount(*p));
            }
            else if (index == 2)
            {
                long count;
                // If reading first time.
                if (it->GetRowEndIndex() == 0)
                {
                    if (it->GetSelector() == 0)
                    {
                        it->SetRowBeginIndex(0);

                        // the end index must be higher by one than the last index that is read
                        it->SetRowEndIndex(this->GetProfileGenericDataCount(*p));
                    }
                    else if (it->GetSelector() == 1)
                    {
                        // Read by entry.
                        GetProfileGenericDataByRange(*p, *it);
                    }
                    else if (it->GetSelector() == 2)
                    {
                        // Read by range.
                        it->SetRowBeginIndex(it->GetParameters().Arr[0].ulVal - 1);
                        it->SetRowEndIndex( it->GetParameters().Arr[1].ulVal);

                        // If client wants to read more data what we have.
                        if (it->GetRowEndIndex() > this->GetProfileGenericDataCount(*p)) it->SetRowEndIndex( this->GetProfileGenericDataCount(*p));
                    }
                }

                if (it->GetRowEndIndex() == 0)   count = 0;
                else                                count = it->GetRowEndIndex() - it->GetRowBeginIndex() + 1;

                // Read only rows that can fit to one PDU.
                if (count > 0 && it->GetRowEndIndex() - it->GetRowBeginIndex() > it->GetRowToPdu()) count = it->GetRowToPdu();
                Logger::LogEvent(Logger::INFO, "Loading profile generic data from index: " + std::to_string(it->GetRowBeginIndex()) + ". Number of items is " + std::to_string(count));
                GetProfileGenericDataByEntry(*p, it->GetRowBeginIndex(), count);
            }
            continue;
        }
        //Framework will handle Association objects automatically.
        else if (type == DLMS_OBJECT_TYPE_ASSOCIATION_LOGICAL_NAME ||
            type == DLMS_OBJECT_TYPE_ASSOCIATION_SHORT_NAME ||
            //Framework will handle profile generic automatically.
            type == DLMS_OBJECT_TYPE_PROFILE_GENERIC)
        {
            continue;
        }
        DLMS_DATA_TYPE ui, dt;
        it->GetTarget()->GetUIDataType(index, ui);
        it->GetTarget()->GetDataType(index, dt);
        //Update date and time of clock object.
        if (type == DLMS_OBJECT_TYPE_CLOCK && index == 2)
        {
            CGXDateTime tm = CGXDateTime::Now();
            ((CGXDLMSClock*)pObj)->SetTime(tm);
            continue;
        }
        else if (type == DLMS_OBJECT_TYPE_REGISTER_MONITOR)
        {
            auto* pRm = (CGXDLMSRegisterMonitor*)pObj;
            if (index == 2)
            {
                //Initialize random seed.
                srand((unsigned int)time(nullptr));
                pRm->GetThresholds().clear();
                pRm->GetThresholds().emplace_back(rand() % 100 + 1);
                continue;
            }
        }
        else if (type == DLMS_OBJECT_TYPE_REGISTER) {
            continue;
        }
        else
        {

            CGXDLMSVariant null;
            CGXDLMSValueEventArg e(pObj, index);
            ret = ((IGXDLMSBase*)pObj)->GetValue(m_Settings, e);
            if (ret != DLMS_ERROR_CODE_OK)
            {
                //TODO: Show error.
                continue;
            }
            //If data is not assigned and value type is unknown return number.
            DLMS_DATA_TYPE tp = e.GetValue().vt;
            if (tp == DLMS_DATA_TYPE_INT8 ||
                tp == DLMS_DATA_TYPE_INT16 ||
                tp == DLMS_DATA_TYPE_INT32 ||
                tp == DLMS_DATA_TYPE_INT64 ||
                tp == DLMS_DATA_TYPE_UINT8 ||
                tp == DLMS_DATA_TYPE_UINT16 ||
                tp == DLMS_DATA_TYPE_UINT32 ||
                tp == DLMS_DATA_TYPE_UINT64)
            {
                //Initialize random seed.
                srand((unsigned int)time(nullptr));
                value = rand() % 100 + 1;
                value.vt = tp;
                e.SetValue(value);
            }
        }
    }
}

/////////////////////////////////////////////////////////////////////////////
// Method, that is called after some object is read.
/////////////////////////////////////////////////////////////////////////////
void GXDLMSBase::PostRead(std::vector<CGXDLMSValueEventArg*>& args)
{
    pthread_mutex_unlock(&objectAccessMutex);
    Logger::LogEvent(Logger::INFO, "Reading object is finished");
}

/////////////////////////////////////////////////////////////////////////////
// Method, that is called before some value of object is written.
/////////////////////////////////////////////////////////////////////////////
void GXDLMSBase::PreWrite(std::vector<CGXDLMSValueEventArg*>& args)
{
    pthread_mutex_lock(&objectAccessMutex);
    for (auto & arg : args)
    {
        if (m_Trace > GX_TRACE_LEVEL_WARNING)
        {
            Logger::LogEvent(Logger::INFO, "Writing to object: " + arg->GetTarget()->GetLogicalName());
        }
    }
}

/////////////////////////////////////////////////////////////////////////////
// Method, that is called after some value of object is written.
/////////////////////////////////////////////////////////////////////////////
void GXDLMSBase::PostWrite(std::vector<CGXDLMSValueEventArg*>& args)
{
    pthread_mutex_unlock(&objectAccessMutex);
    DLMS_OBJECT_TYPE type;
    for (auto &&arg : args)
    {
        if (arg->GetTarget()->GetObjectType() == DLMS_OBJECT_TYPE_REGISTER) {
            auto *pRegister = (CGXDLMSRegisterE*) arg->GetTarget();
            DLMS_DATA_TYPE valueType;
            arg->GetTarget()->GetDataType(2, valueType);
            cerr << "Mrdat prdelky" << endl;

//            this->hatelCommunication.Put(pRegister->GetCid(), pRegister->GetValue(), valueType);
        }
    }
    Logger::LogEvent(Logger::INFO, "Writing object is finished");
}

/////////////////////////////////////////////////////////////////////////////
// Method that returns value of object.
/////////////////////////////////////////////////////////////////////////////
std::string getObjectValue(
        ProfileGenericWriterStruct  &writerStruct,
        DLMS_OBJECT_TYPE type,
        const std::string &obis) {

    CGXDLMSObject *object = writerStruct.serverObjects->FindByLN(DLMS_OBJECT_TYPE_ALL,(std::string &) obis);

    if (object == nullptr) {
        pthread_mutex_unlock(writerStruct.objectAccessMutex);
        throw HandleException("Object with obis " + obis + " was not found!");
    }
    else if (object->GetObjectType() == DLMS_OBJECT_TYPE_REGISTER) {
        auto *reg = (CGXDLMSRegister *) object;
        return reg->GetValue().ToString();
    } else if (object->GetObjectType() == DLMS_OBJECT_TYPE_CLOCK) {
        auto *clock = (CGXDLMSClock*) object;
        CGXDateTime tm = CGXDateTime::Now();
        clock->SetTime(tm);
        return clock->GetTime().ToString();
    } else {
        pthread_mutex_unlock(writerStruct.objectAccessMutex);
        throw HandleException("Object of type " + std::to_string(object->GetObjectType()) + " is not supported!");
    }
}


/////////////////////////////////////////////////////////////////////////////
// Method that returns true if object can be inserted to the saved values of profile generic object
// that depends on actual value, number of saved values and on sort method. False otherwise.
/////////////////////////////////////////////////////////////////////////////
bool canBeInserted(long actRegisterValue, long savedSortRegisterValue, GX_SORT_METHOD sortMethod) {
    switch(sortMethod) {
        case DLMS_SORT_METHOD_LARGEST:
            return actRegisterValue > savedSortRegisterValue;
        case DLMS_SORT_METHOD_SMALLEST:
            return actRegisterValue < savedSortRegisterValue;
        case DLMS_SORT_METHOD_NEAREST_TO_ZERO:
            return std::abs(actRegisterValue) < std::abs(savedSortRegisterValue);
        case DLMS_SORT_METHOD_FAREST_FROM_ZERO:
            return std::abs(actRegisterValue) > std::abs(savedSortRegisterValue);
        default:
            return false;
    }
}

///////////////////////////////////////////////////////////////////////
// Method, that saves new record that contains actual values of specified objects to the profile generic file.
///////////////////////////////////////////////////////////////////////
void GXDLMSBase::Capture(ProfileGenericWriterStruct &writerStruct)
{
    // access mutex must be locked by the caller
    CGXDateTime tm = CGXDateTime::Now();
    pugi::xml_document doc = GXDLMSBase::LoadProfileGenericData(
            writerStruct.loadedUserConfiguration->getDlmsServerPathsConfiguration().getPathToSaveProfileGenericData(), *writerStruct.profileGeneric);
    pugi::xml_node rootNode = doc.child("Root");

    pugi::xml_node recordNode;
    bool isInserted = false;
    long actSortRegisterValue = std::stoll(getObjectValue(writerStruct, DLMS_OBJECT_TYPE_ALL,  writerStruct.profileGeneric->GetSortObject()->GetLogicalName()));
    switch(writerStruct.profileGeneric->GetSortMethod()) {
        case DLMS_SORT_METHOD_LARGEST:
        case DLMS_SORT_METHOD_SMALLEST:
        case DLMS_SORT_METHOD_NEAREST_TO_ZERO:
        case DLMS_SORT_METHOD_FAREST_FROM_ZERO:
            for (auto &&record : rootNode.children("Record")) {
                long savedSortRegisterValue = std::stoll(record.find_child_by_attribute("Register", "obis", writerStruct.profileGeneric->GetSortObject()->GetLogicalName().c_str()).text().get());
                if (canBeInserted(actSortRegisterValue, savedSortRegisterValue, writerStruct.profileGeneric->GetSortMethod())) {
                    recordNode = rootNode.insert_child_before("Record", record);
                    isInserted = true;
                    break;
                }
            }
            // if the new record is not inserted, insert it at the last position
            if (!isInserted) recordNode = rootNode.append_child("Record");

            if (std::distance(rootNode.children("Record").begin(), rootNode.children("Record").end()) > writerStruct.profileGeneric->GetProfileEntries())
                rootNode.remove_child(rootNode.last_child());
            break;
        case DLMS_SORT_METHOD_LIFO:
            if (std::distance(rootNode.children("Record").begin(), rootNode.children("Record").end()) == writerStruct.profileGeneric->GetProfileEntries())
                rootNode.remove_child(rootNode.last_child());
            recordNode = rootNode.append_child("Record");
            break;
        case DLMS_SORT_METHOD_FIFO:
        default:
            if (std::distance(rootNode.children("Record").begin(), rootNode.children("Record").end()) == writerStruct.profileGeneric->GetProfileEntries())
                rootNode.remove_child(rootNode.first_child());
            recordNode = rootNode.append_child("Record");
            break;
    }

    for (auto &&captureObject : writerStruct.profileGeneric->GetCaptureObjects()) {
        pugi::xml_node node;
        switch (captureObject.first->GetObjectType()) {
            case DLMS_OBJECT_TYPE_REGISTER:
                node = recordNode.append_child("Register");
                break;
            case DLMS_OBJECT_TYPE_CLOCK:
                node = recordNode.append_child("Clock");
                break;
            default:
                throw HandleException("Capturing of object with type " + std::to_string(captureObject.first->GetObjectType()) + " is not supported!");
        }
        node.append_attribute("obis").set_value(captureObject.first->GetLogicalName().c_str());
        if (captureObject.first->GetLogicalName() == writerStruct.profileGeneric->GetSortObject()->GetLogicalName())
            node.text().set(actSortRegisterValue);
        else
            node.text().set(getObjectValue(writerStruct, DLMS_OBJECT_TYPE_ALL, captureObject.first->GetLogicalName()).c_str());
    }

    GXDLMSBase::SaveProfileGenericData(doc, writerStruct.loadedUserConfiguration->getDlmsServerPathsConfiguration().getPathToSaveProfileGenericData(), *writerStruct.profileGeneric);
}

/////////////////////////////////////////////////////////////////////////////
// Called after every action
/////////////////////////////////////////////////////////////////////////////
void GXDLMSBase::PostAction(std::vector<CGXDLMSValueEventArg*>& args)
{
    for (auto & arg : args)
    {
        if (arg->GetTarget()->GetObjectType() == DLMS_OBJECT_TYPE_ASSOCIATION_LOGICAL_NAME) {
            auto* ln = (CGXDLMSAssociationLogicalName*)  arg->GetTarget();


            /*ln->GetObjectList().erase(std::remove_if(ln->GetObjectList().begin(), ln->GetObjectList().end(), [](CGXDLMSObject *o) -> bool {
                return o->GetObjectType() == DLMS_OBJECT_TYPE_REGISTER;
            }), ln->GetObjectList().end());*/
        }
        else if (arg->GetTarget()->GetObjectType() == DLMS_OBJECT_TYPE_PROFILE_GENERIC)
        {
            auto* pg = (CGXDLMSProfileGeneric*) arg->GetTarget();

            if (arg->GetIndex() == 1) // Reset method is called
            {
                pugi::xml_document doc = GXDLMSBase::LoadProfileGenericData(
                        this->loadedUserConfiguration.getDlmsServerPathsConfiguration().getPathToSaveProfileGenericData(), *pg);
                pugi::xml_node rootNode = doc.child("Root");
                rootNode.remove_children();
                GXDLMSBase::SaveProfileGenericData(doc, this->loadedUserConfiguration.getDlmsServerPathsConfiguration().getPathToSaveProfileGenericData(), *pg);
            }
            else if (arg->GetIndex() == 2) // Profile generic Capture is called.
            {
            }
        }

        else if (arg->GetTarget()->GetObjectType() == DLMS_OBJECT_TYPE_SECURITY_SETUP)
        {
            printf("----------------------------------------------------------\r\n");
            printf("Updated keys :\r\n");
            printf("System Title: %s\r\n", m_Settings.GetCipher()->GetSystemTitle().ToHexString().c_str());
            printf("Authentication key: %s\r\n", m_Settings.GetCipher()->GetAuthenticationKey().ToHexString().c_str());
            printf("Block cipher key: %s\r\n", m_Settings.GetCipher()->GetBlockCipherKey().ToHexString().c_str());
            printf("Master key (KEK) title: %s\r\n", m_Settings.GetKek().ToHexString().c_str());
        }
        else if (arg->GetTarget()->GetObjectType() == DLMS_OBJECT_TYPE_REGISTER) {
            auto *pRegister = (CGXDLMSRegisterE*) arg->GetTarget();
            DLMS_DATA_TYPE valueType;
            arg->GetTarget()->GetDataType(2, valueType);


            pthread_mutex_unlock(&objectAccessMutex);
            // Value of register is cleared (set to 0) in Invoke method
//            this->hatelCommunication.Put(pRegister->GetCid(), pRegister->GetValue(), valueType);
            pthread_mutex_lock(&objectAccessMutex);
        }
    }
    pthread_mutex_unlock(&objectAccessMutex);
    Logger::LogEvent(Logger::INFO, "Post action is finished");
}

/////////////////////////////////////////////////////////////////////////////
// Called before every action
/////////////////////////////////////////////////////////////////////////////
void GXDLMSBase::PreAction(std::vector<CGXDLMSValueEventArg*>& args)
{
    pthread_mutex_lock(&objectAccessMutex);
    for (auto & arg : args)
    {
        Logger::LogEvent(Logger::INFO, "Pre action of object: " + arg->GetTarget()->GetLogicalName());
        if (arg->GetTarget()->GetObjectType() == DLMS_OBJECT_TYPE_IMAGE_TRANSFER)
        {
            HandleImageTransfer(arg);
        }
        if (arg->GetTarget()->GetObjectType() == DLMS_OBJECT_TYPE_PUSH_SETUP)
        {
            if (SendPush((CGXDLMSPushSetup*)arg->GetTarget()) != 0)
            {

            }
            arg->SetHandled(true);
        }
    }
}

/////////////////////////////////////////////////////////////////////////////
//  Method, that verifies if this server is corresponds with given address.
/////////////////////////////////////////////////////////////////////////////
bool GXDLMSBase::IsTarget(
    unsigned long int serverAddress, // address is without force zeros and ones
    unsigned long clientAddress)
{
    auto findObject = std::find_if(this->GetItems().begin(), this->GetItems().end(), [&] (CGXDLMSObject *o) {
        if (o->GetObjectType() != DLMS_OBJECT_TYPE_ASSOCIATION_LOGICAL_NAME)
            return false;
        else {
            auto *ln = (CGXDLMSAssociationLogicalName*) o;
            return ln->GetClientSAP() == clientAddress;
        }
    });

    if (findObject == this->GetItems().end()) {
        this->GetItems().setActAssociationLn("");
        return false;
    } else {
        this->GetItems().setActAssociationLn((*findObject)->GetLogicalName());
        auto *ln = (CGXDLMSAssociationLogicalName*) (*findObject);

        std::string securitySetupLn = ln->GetSecuritySetupReference();
        auto *securitySetup = (CGXDLMSSecuritySetupE*) this->GetItems().FindByLN(DLMS_OBJECT_TYPE_SECURITY_SETUP, securitySetupLn);
        std::string invocationCounterReferenceLn = securitySetup->getInvocationCounterReference();
        auto *invocationCounterData = (CGXDLMSData*) this->GetItems().FindByLN(DLMS_OBJECT_TYPE_DATA, invocationCounterReferenceLn);

        this->SetExpectedSecurityPolicy(securitySetup->GetSecurityPolicy());
        this->SetExpectedSecuritySuite(securitySetup->GetSecuritySuite());
        this->SetExpectedInvocationCounter((uint64_t) invocationCounterData->GetValue().ToInteger());
        this->SetConformance(ln->GetXDLMSContextInfo().GetConformance());

        CGXByteBuffer buffer = securitySetup->GetServerSystemTitle();
        this->GetCiphering()->SetSystemTitle(buffer);
    }

    if (loadedUserConfiguration.getDlmsServerConfiguration().getInterfaceType() == DLMS_INTERFACE_TYPE_WRAPPER)
        return loadedUserConfiguration.getDlmsServerConfiguration().getPhysicalAddress() == serverAddress;
    else
        return CGXDLMSClient::GetServerAddress(
                loadedUserConfiguration.getDlmsServerConfiguration().getLogicalAddress(),
                loadedUserConfiguration.getDlmsServerConfiguration().getPhysicalAddress(),
                loadedUserConfiguration.getDlmsServerConfiguration().getAddressSize()) == serverAddress;
}

/////////////////////////////////////////////////////////////////////////////
//  Method, that validates client NONE and LOW authentication.
/////////////////////////////////////////////////////////////////////////////
DLMS_SOURCE_DIAGNOSTIC GXDLMSBase::ValidateAuthentication(
    DLMS_AUTHENTICATION authentication,
    CGXByteBuffer& password)
{
    CGXByteBuffer associationSecret;
    DLMS_AUTHENTICATION associationAuthentication = DLMS_AUTHENTICATION_HIGH_SHA1;
    if (m_Settings.GetUseLogicalNameReferencing()) {
        auto *ln = (CGXDLMSAssociationLogicalName*) this->GetItems().FindByLN(DLMS_OBJECT_TYPE_ASSOCIATION_LOGICAL_NAME, this->GetItems().getActAssociationLn());
        associationSecret = ln->GetSecret();
        associationAuthentication = ln->GetAuthenticationMechanismName().GetMechanismId();
    } else {
        auto* sn = (CGXDLMSAssociationShortName*) this->GetItems().FindBySN(this->GetItems().getActAssociationSn());
        associationSecret = sn->GetSecret();
        // TODO authentication mechanism for SN
        //associationAuthentication = sn->GetSecuritySetupReference()
    }

    // Check if client address corresponds with type of client
    if (m_Settings.GetAuthentication() != associationAuthentication) {
        return DLMS_SOURCE_DIAGNOSTIC_AUTHENTICATION_FAILURE;
    }

    if (authentication == DLMS_AUTHENTICATION_NONE)
    {
        //Uncomment this if authentication is always required.
        //return DLMS_SOURCE_DIAGNOSTIC_AUTHENTICATION_MECHANISM_NAME_REQUIRED;
    }

    if (authentication == DLMS_AUTHENTICATION_LOW)
    {
        if (associationSecret.GetSize() == password.GetSize() && associationSecret.Compare(password.GetData(), password.GetSize()))
        {
            return DLMS_SOURCE_DIAGNOSTIC_NONE;
        }
        return DLMS_SOURCE_DIAGNOSTIC_AUTHENTICATION_FAILURE;
    }
    // Other authentication levels are check on phase two.
    return DLMS_SOURCE_DIAGNOSTIC_NONE;
}

/////////////////////////////////////////////////////////////////////////////
//  Method, that returns access mode to given object attribute.
/////////////////////////////////////////////////////////////////////////////
DLMS_ACCESS_MODE GXDLMSBase::GetAttributeAccess(CGXDLMSValueEventArg* arg)
{
    try {
        auto *ln = (CGXDLMSAssociationLogicalNameE*) this->GetItems().FindByLN(DLMS_OBJECT_TYPE_ASSOCIATION_LOGICAL_NAME, this->GetItems().getActAssociationLn());
        return ln->getObjectsAttributeAccessRights().at(arg->GetTarget()->GetLogicalName()).at(arg->GetIndex());
    } catch (const std::out_of_range &err) {
        // if attribute access for association is not found, the default value is returned
        switch(arg->GetTarget()->GetAccess(arg->GetIndex())) {
            case DLMS_ACCESS_MODE_AUTHENTICATED_READ:
            case DLMS_ACCESS_MODE_AUTHENTICATED_WRITE:
            case DLMS_ACCESS_MODE_AUTHENTICATED_READ_WRITE:
                if (arg->GetSettings()->GetAuthentication() == DLMS_AUTHENTICATION_NONE)
                    return DLMS_ACCESS_MODE_NONE;
                else
                    return arg->GetTarget()->GetAccess(arg->GetIndex());
            case DLMS_ACCESS_MODE_NONE:
            case DLMS_ACCESS_MODE_READ:
            case DLMS_ACCESS_MODE_WRITE:
            case DLMS_ACCESS_MODE_READ_WRITE:
            default:
                return arg->GetTarget()->GetAccess(arg->GetIndex());
        }
    }

    /*
    // Only read is allowed
    if (arg->GetSettings()->GetAuthentication() == DLMS_AUTHENTICATION_NONE) {
        return DLMS_ACCESS_MODE_READ;
    }
    // Only clock write is allowed.
    if (arg->GetSettings()->GetAuthentication() == DLMS_AUTHENTICATION_LOW)
    {
        if (arg->GetTarget()->GetObjectType() == DLMS_OBJECT_TYPE_CLOCK)
        {
            return DLMS_ACCESS_MODE_READ_WRITE;
        }
        return DLMS_ACCESS_MODE_READ;
    }
    // All writes are allowed.
    return DLMS_ACCESS_MODE_READ_WRITE;*/
}

/////////////////////////////////////////////////////////////////////////////
//  Method, that returns access mode to given object method.
/////////////////////////////////////////////////////////////////////////////
DLMS_METHOD_ACCESS_MODE GXDLMSBase::GetMethodAccess(CGXDLMSValueEventArg* arg)
{
    try {
        auto *ln = (CGXDLMSAssociationLogicalNameE*) this->GetItems().FindByLN(DLMS_OBJECT_TYPE_ASSOCIATION_LOGICAL_NAME, this->GetItems().getActAssociationLn());
        return ln->getObjectsMethodAccessRights().at(arg->GetTarget()->GetLogicalName()).at(arg->GetIndex());
    } catch (const std::out_of_range &err) {
        switch (arg->GetTarget()->GetMethodAccess(arg->GetIndex())) {
            case DLMS_METHOD_ACCESS_MODE_AUTHENTICATED_ACCESS:
                if (arg->GetSettings()->GetAuthentication() == DLMS_AUTHENTICATION_NONE)
                    return DLMS_METHOD_ACCESS_MODE_NONE;
                else
                    return arg->GetTarget()->GetMethodAccess(arg->GetIndex());
            case DLMS_METHOD_ACCESS_MODE_NONE:
            case DLMS_METHOD_ACCESS_MODE_ACCESS:
            default:
                return arg->GetTarget()->GetMethodAccess(arg->GetIndex());
        }
    }
    /*
    // Methods are not allowed.
    if (arg->GetSettings()->GetAuthentication() == DLMS_AUTHENTICATION_NONE)
    {
        return DLMS_METHOD_ACCESS_MODE_NONE;
    }
    // Only clock and profile generic methods are allowed.
    if (arg->GetSettings()->GetAuthentication() == DLMS_AUTHENTICATION_LOW)
    {
        if (arg->GetTarget()->GetObjectType() == DLMS_OBJECT_TYPE_CLOCK ||
            arg->GetTarget()->GetObjectType() == DLMS_OBJECT_TYPE_PROFILE_GENERIC)
        {
            return DLMS_METHOD_ACCESS_MODE_ACCESS;
        }
        return DLMS_METHOD_ACCESS_MODE_NONE;
    }
    return DLMS_METHOD_ACCESS_MODE_ACCESS;*/
}

/////////////////////////////////////////////////////////////////////////////
    //  Method, that is called when the new connection was accepted an was made for the server. All initialization is done.
/////////////////////////////////////////////////////////////////////////////
void GXDLMSBase::Connected(
    CGXDLMSConnectionEventArgs& connectionInfo)
{
    if (m_Trace > GX_TRACE_LEVEL_WARNING)
    {
        Logger::LogEvent(Logger::INFO, "Connected");
    }
}

/////////////////////////////////////////////////////////////////////////////
// Method that is called when a client has try to made invalid connection. Password is incorrect.
/////////////////////////////////////////////////////////////////////////////
void GXDLMSBase::InvalidConnection(
    CGXDLMSConnectionEventArgs& connectionInfo)
{
    if (m_Trace > GX_TRACE_LEVEL_WARNING)
    {
        Logger::LogEvent(Logger::INFO, "Invalid connection");
    }
}
/////////////////////////////////////////////////////////////////////////////
// Method that is called after the connection is closed.
/////////////////////////////////////////////////////////////////////////////
void GXDLMSBase::Disconnected(
    CGXDLMSConnectionEventArgs& connectionInfo)
{
    if (m_Trace > GX_TRACE_LEVEL_WARNING)
    {
        Logger::LogEvent(Logger::INFO, "Disconnected");
    }
}

///////////////////////////////////////////////////////////////////////
// Method, that is called before Capture method is called on profile Generic object.
///////////////////////////////////////////////////////////////////////
void GXDLMSBase::PreGet(
    std::vector<CGXDLMSValueEventArg*>& args)
{
    // mutex is already locked in preAction method
    for (auto &&arg : args)
    {
        Logger::LogEvent(Logger::INFO, "Pre get of object: " + arg->GetTarget()->GetLogicalName());
        if (arg->GetTarget()->GetObjectType() == DLMS_OBJECT_TYPE_PROFILE_GENERIC)
        {
            auto* pg = (CGXDLMSProfileGeneric*)arg->GetTarget();
            for (auto &&writerStruct : this->m_ProfileGenericThreads) {
                if (writerStruct->profileGeneric == pg) {
                    Logger::LogEvent(Logger::INFO, "Capturing value of object: " + writerStruct->profileGeneric->GetLogicalName());
                    GXDLMSBase::Capture(*writerStruct);
                }
            }
            arg->SetHandled(true);
        }
    }
}

///////////////////////////////////////////////////////////////////////
// Method, that is called after Capture method is called on profile Generic object.
///////////////////////////////////////////////////////////////////////
void GXDLMSBase::PostGet(
    std::vector<CGXDLMSValueEventArg*>& args)
{
    // mutex will be freed in PostAction method
    Logger::LogEvent(Logger::INFO, "Post get is finsihed");
}

///////////////////////////////////////////////////////////////////////
// Method, that loads content of file with profile generic data.
///////////////////////////////////////////////////////////////////////
pugi::xml_document GXDLMSBase::LoadProfileGenericData(const std::string &pathToFolder, CGXDLMSProfileGeneric &profileGeneric) {
    pugi::xml_document doc;

    doc.load_file(GXDLMSBase::CreatePathToProfileGenericFile(pathToFolder, profileGeneric).c_str());

    pugi::xml_node rootNode = doc.child("Root");

    if (!rootNode)   rootNode = doc.append_child("Root");

    return doc;
}


///////////////////////////////////////////////////////////////////////
// Method, that saves content of file with profile generic data.
///////////////////////////////////////////////////////////////////////
void GXDLMSBase::SaveProfileGenericData(const pugi::xml_document& doc, const std::string &pathToFolder, CGXDLMSProfileGeneric &profileGeneric) {
    doc.save_file(GXDLMSBase::CreatePathToProfileGenericFile(pathToFolder, profileGeneric).c_str());
}


///////////////////////////////////////////////////////////////////////
// Method, that creates path to file with profile generic data.
///////////////////////////////////////////////////////////////////////
std::string GXDLMSBase::CreatePathToProfileGenericFile(const std::string &pathToFolder, CGXDLMSProfileGeneric &profileGeneric) {
    std::string pathToFile = pathToFolder;

    if (pathToFile.back() != '/') pathToFile.append("/");

    return pathToFile + profileGeneric.GetLogicalName() + ".xml";
}


/****************************************************
 * Methods for future implementations
 ****************************************************/

void GXDLMSBase::HandleImageTransfer(CGXDLMSValueEventArg* e)
{
/*    CGXDLMSImageTransfer* i = (CGXDLMSImageTransfer*)e->GetTarget();
    //Image name and size to transfer
    FILE* f;
    if (e->GetIndex() == 1)
    {
        i->SetImageTransferStatus(DLMS_IMAGE_TRANSFER_STATUS_NOT_INITIATED);
        if (e->GetParameters().Arr.size() != 2)
        {
            e->SetError(DLMS_ERROR_CODE_UNMATCH_TYPE);
            return;
        }
        imageSize = e->GetParameters().Arr[1].ToInteger();
        char* p = strrchr(IMAGEFILE, '\\');
        ++p;
        *p = '\0';
#if defined(_WIN32) || defined(_WIN64)//If Windows
        strncat_s(IMAGEFILE, (char*)e->GetParameters().Arr[0].byteArr, (int)e->GetParameters().Arr[0].GetSize());
        strcat_s(IMAGEFILE, ".bin");
#else
        strncat(IMAGEFILE, (char*)e->GetParameters().Arr[0].byteArr, (int)e->GetParameters().Arr[0].GetSize());
        strcat(IMAGEFILE, ".bin");
#endif
#if defined(_WIN32) || defined(_WIN64) || defined(__linux__)//If Windows or Linux
        printf("Updating image %s Size: %d\n", IMAGEFILE, imageSize);
#endif
#if defined(_WIN32) || defined(_WIN64)//If Windows
        fopen_s(&f, IMAGEFILE, "wb");
#else
        f = fopen(IMAGEFILE, "wb");
#endif
        if (!f)
        {
#if defined(_WIN32) || defined(_WIN64) || defined(__linux__)//If Windows or Linux
            printf("Unable to open file %s\n", IMAGEFILE);
#endif
            e->SetError(DLMS_ERROR_CODE_HARDWARE_FAULT);
            return;
        }
        fclose(f);
    }
    //Transfers one block of the Image to the server
    else if (e->GetIndex() == 2)
    {
        if (e->GetParameters().Arr.size() != 2)
        {
            e->SetError(DLMS_ERROR_CODE_UNMATCH_TYPE);
            return;
        }
        i->SetImageTransferStatus(DLMS_IMAGE_TRANSFER_STATUS_INITIATED);
#if defined(_WIN32) || defined(_WIN64)//If Windows
        fopen_s(&f, IMAGEFILE, "ab");
#else
        f = fopen(IMAGEFILE, "ab");
#endif
        if (!f)
        {
#if defined(_WIN32) || defined(_WIN64) || defined(__linux__)//If Windows or Linux
            printf("Unable to open file %s\n", IMAGEFILE);
#endif
            e->SetError(DLMS_ERROR_CODE_HARDWARE_FAULT);
            return;
        }

        int ret = fwrite(e->GetParameters().Arr[1].byteArr, 1, (int)e->GetParameters().Arr[1].GetSize(), f);
        fclose(f);
        if (ret != e->GetParameters().Arr[1].GetSize())
        {
            e->SetError(DLMS_ERROR_CODE_UNMATCH_TYPE);
        }
        imageActionStartTime = time(NULL);
        return;
    }
    //Verifies the integrity of the Image before activation.
    else if (e->GetIndex() == 3)
    {
        i->SetImageTransferStatus(DLMS_IMAGE_TRANSFER_STATUS_VERIFICATION_INITIATED);
#if defined(_WIN32) || defined(_WIN64)//If Windows
        fopen_s(&f, IMAGEFILE, "rb");
#else
        f = fopen(IMAGEFILE, "rb");
#endif
        if (!f)
        {
#if defined(_WIN32) || defined(_WIN64) || defined(__linux__)//If Windows or Linux
            printf("Unable to open file %s\n", IMAGEFILE);
#endif
            e->SetError(DLMS_ERROR_CODE_HARDWARE_FAULT);
            return;
        }
        fseek(f, 0L, SEEK_END);
        int size = (int)ftell(f);
        fclose(f);
        if (size != imageSize)
        {
            i->SetImageTransferStatus(DLMS_IMAGE_TRANSFER_STATUS_VERIFICATION_FAILED);
            e->SetError(DLMS_ERROR_CODE_OTHER_REASON);
        }
        else
        {
            //Wait 5 seconds before image is verified.
            if (time(NULL) - imageActionStartTime < 5)
            {
#if defined(_WIN32) || defined(_WIN64) || defined(__linux__)//If Windows or Linux
                printf("Image verification is on progress.\n");
#endif
                e->SetError(DLMS_ERROR_CODE_TEMPORARY_FAILURE);
            }
            else
            {
#if defined(_WIN32) || defined(_WIN64) || defined(__linux__)//If Windows or Linux
                printf("Image is verificated");
#endif
                i->SetImageTransferStatus(DLMS_IMAGE_TRANSFER_STATUS_VERIFICATION_SUCCESSFUL);
                imageActionStartTime = time(NULL);
            }
        }
    }
    //Activates the Image.
    else if (e->GetIndex() == 4)
    {
        i->SetImageTransferStatus(DLMS_IMAGE_TRANSFER_STATUS_ACTIVATION_INITIATED);
        //Wait 5 seconds before image is activated.
        if (time(NULL) - imageActionStartTime < 5)
        {
#if defined(_WIN32) || defined(_WIN64) || defined(__linux__)//If Windows or Linux
            printf("Image activation is on progress.\n");
#endif
            e->SetError(DLMS_ERROR_CODE_TEMPORARY_FAILURE);
        }
        else
        {
#if defined(_WIN32) || defined(_WIN64) || defined(__linux__)//If Windows or Linux
            printf("Image is activated.");
#endif
            i->SetImageTransferStatus(DLMS_IMAGE_TRANSFER_STATUS_ACTIVATION_SUCCESSFUL);
            imageActionStartTime = time(NULL);
        }
    }*/
}


/**
* Connect to Push listener.
*/
int Connect(const char* address, int port, int& s)
{
    //create socket.
    s = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    if (s == -1)
    {
        assert(0);
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }
    sockaddr_in add{};
    add.sin_port = htons(port);
    add.sin_family = AF_INET;
    add.sin_addr.s_addr = inet_addr(address);
    //If address is give as name
    if (add.sin_addr.s_addr == INADDR_NONE)
    {
        hostent* Hostent = gethostbyname(address);
        if (Hostent == nullptr)
        {
#if defined(_WIN32) || defined(_WIN64)//If Windows
            int err = WSAGetLastError();
#else
            int err = errno;
#endif
            closesocket(s);
            return err;
        };
        add.sin_addr = *(in_addr*)(void*)Hostent->h_addr_list[0];
    };

    //Connect to the meter.
    int ret = connect(s, (sockaddr*)&add, sizeof(sockaddr_in));
    if (ret == -1)
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    };
    return DLMS_ERROR_CODE_OK;
}

long GXDLMSBase::SendPush(CGXDLMSPushSetup* target)
{
    long ret;
    char host[20];
    int port;
    if (sscanf(target->GetDestination().c_str(), "%[^:]:%d", host, &port) != 2)
    {
        return DLMS_ERROR_CODE_INVALID_PARAMETER;
    }

    int socket = -1;
    std::vector<CGXByteBuffer> reply;
    if ((ret = GeneratePushSetupMessages(nullptr, target, reply)) == 0)
    {
        if ((ret = Connect(host, port, socket)) != 0)
        {
            return ret;
        }
        for (auto & it : reply)
        {
            if ((ret = send(socket, (const char*)it.GetData(), it.GetSize(), 0)) == -1)
            {
                break;
            }
        }
        closesocket(socket);
    }
    return ret;
}


/*****************************************
 * Default methods for creating objects
 ****************************************/

/*
* Add Electricity ID 1.
*/
void AddElectricityID1(CGXDLMSObjectCollection& items, unsigned long sn)
{
    char buff[17];
#if defined(_WIN32) || defined(_WIN64)//Windows
    sprintf_s(buff, "GRX%.13d", sn);
#else
    sprintf(buff, "GRX%.13lu", sn);
#endif
    CGXDLMSVariant id;
    id.Add((const char*)buff, 16);
    auto* d = new CGXDLMSData("1.1.0.0.0.255");
    d->SetValue(id);
    d->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_STRING));
    items.push_back(d);
}

/*
* Add Electricity ID 2.
*/
void AddElectricityID2(CGXDLMSObjectCollection& items, unsigned long sn)
{
    CGXDLMSVariant id2(sn);
    auto* d = new CGXDLMSData("1.1.0.0.1.255");
    d->SetValue(id2);
    d->GetAttributes().push_back(CGXDLMSAttribute(2, DLMS_DATA_TYPE_UINT32));
    items.push_back(d);
}

/*
* Add Auto connect object.
*/
void AddAutoConnect(CGXDLMSObjectCollection& items)
{
    auto* pAC = new CGXDLMSAutoConnect();
    pAC->SetMode(DLMS_AUTO_CONNECT_MODE_NO_AUTO_CONNECT);
    pAC->SetRepetitions(10);
    pAC->SetRepetitionDelay(60);
    //Calling is allowed between 1am to 6am.
    pAC->GetCallingWindow().emplace_back(CGXTime(1, 0, 0, -1), CGXTime(6, 0, 0, -1));
    pAC->GetDestinations().emplace_back("www.gurux.org");
    items.push_back(pAC);
}

/*
* Add Activity Calendar object.
*/
void AddActivityCalendar(CGXDLMSObjectCollection& items)
{
    auto* pActivity = new CGXDLMSActivityCalendar();
    pActivity->SetCalendarNameActive("Active");
    CGXDateTime c0(-1, 3, 31, 0, 0, 0, 0);
    pActivity->GetSeasonProfileActive().push_back(new CGXDLMSSeasonProfile("Summer time", c0, "sss"));
    pActivity->GetWeekProfileTableActive().push_back(new CGXDLMSWeekProfile("Monday", 1, 1, 1, 1, 1, 1, 1));
    auto* aDp = new CGXDLMSDayProfile();
    aDp->SetDayId(1);
    CGXDateTime now = CGXDateTime::Now();
    CGXTime time = now;
    aDp->GetDaySchedules().push_back(new CGXDLMSDayProfileAction(time, "test", 1));
    pActivity->GetDayProfileTableActive().push_back(aDp);
    pActivity->SetCalendarNamePassive("Passive");
    CGXDateTime c1(-1, 10, 30, 0, 0, 0, 0);
    pActivity->GetSeasonProfilePassive().push_back(new CGXDLMSSeasonProfile("Winter time", c1, ""));
    pActivity->GetWeekProfileTablePassive().push_back(new CGXDLMSWeekProfile("Tuesday", 1, 1, 1, 1, 1, 1, 1));

    auto* passive = new CGXDLMSDayProfile();
    passive->SetDayId(1);
    passive->GetDaySchedules().push_back(new CGXDLMSDayProfileAction(time, "0.0.1.0.0.255", 1));
    pActivity->GetDayProfileTablePassive().push_back(passive);
    CGXDateTime dt(CGXDateTime::Now());
    pActivity->SetTime(dt);
    items.push_back(pActivity);
}

/*
* Add Optical Port Setup object.
*/
void AddOpticalPortSetup(CGXDLMSObjectCollection& items)
{
    auto* pOptical = new CGXDLMSIECOpticalPortSetup();
    pOptical->SetDefaultMode(DLMS_OPTICAL_PROTOCOL_MODE_DEFAULT);
    pOptical->SetProposedBaudrate(DLMS_BAUD_RATE_9600);
    pOptical->SetDefaultBaudrate(DLMS_BAUD_RATE_300);
    pOptical->SetResponseTime(DLMS_LOCAL_PORT_RESPONSE_TIME_200_MS);
    pOptical->SetDeviceAddress("Gurux");
    pOptical->SetPassword1("Gurux1");
    pOptical->SetPassword2("Gurux2");
    pOptical->SetPassword5("Gurux5");
    items.push_back(pOptical);
}

/*
* Add Demand Register object.
*/
void AddDemandRegister(CGXDLMSObjectCollection& items)
{
    auto* pDr = new CGXDLMSDemandRegister("1.0.31.4.0.255");
    pDr->SetCurrentAverageValue(10);
    pDr->SetLastAverageValue(20);
    pDr->SetStatus(1);
    CGXDateTime c = CGXDateTime::Now();
    pDr->SetStartTimeCurrent(c);
    pDr->SetCaptureTime(CGXDateTime::Now());
    pDr->SetPeriod(10);
    pDr->SetNumberOfPeriods(1);
    items.push_back(pDr);
}

/*
* Add Register Monitor object.
*/
void AddRegisterMonitor(CGXDLMSObjectCollection& items, CGXDLMSRegister* pRegister)
{
    auto* pRm = new CGXDLMSRegisterMonitor("0.0.16.1.0.255");
    CGXDLMSVariant threshold;
    vector<CGXDLMSVariant> thresholds;
    threshold.Add("Gurux1", 6);
    thresholds.push_back(threshold);
    threshold.Clear();
    threshold.Add("Gurux2", 6);
    thresholds.push_back(threshold);
    pRm->SetThresholds(thresholds);
    CGXDLMSMonitoredValue mv;
    mv.Update(pRegister, 2);
    pRm->SetMonitoredValue(mv);
    auto* action = new CGXDLMSActionSet();
    string ln;
    pRm->GetLogicalName(ln);
    action->GetActionDown().SetLogicalName(ln);
    action->GetActionDown().SetScriptSelector(1);
    pRm->GetLogicalName(ln);
    action->GetActionUp().SetLogicalName(ln);
    action->GetActionUp().SetScriptSelector(1);
    pRm->GetActions().push_back(action);
    items.push_back(pRm);
}

/*
* Add action schedule object.
*/
void AddActionSchedule(CGXDLMSObjectCollection& items)
{
    auto* pActionS = new CGXDLMSActionSchedule();
    pActionS->SetExecutedScriptLogicalName("0.1.10.1.101.255");
    pActionS->SetExecutedScriptSelector(1);
    pActionS->SetType(DLMS_SINGLE_ACTION_SCHEDULE_TYPE1);
    pActionS->GetExecutionTime().push_back(CGXDateTime::Now());
    items.push_back(pActionS);
}

/*
* Add SAP Assignment object.
*/
void AddSapAssignment(CGXDLMSObjectCollection& items)
{
    auto* pSap = new CGXDLMSSapAssignment();
    std::map<int, basic_string<char> > list;
    list[1] = "Gurux";
    list[16] = "Gurux-2";
    pSap->SetSapAssignmentList(list);
    items.push_back(pSap);
}

/**
* Add Auto Answer object.
*/
void AddAutoAnswer(CGXDLMSObjectCollection& items)
{
    auto* pAa = new CGXDLMSAutoAnswer();
    pAa->SetMode(DLMS_AUTO_ANSWER_MODE_NONE);
    pAa->GetListeningWindow().emplace_back(CGXDateTime(-1, -1, -1, 6, -1, -1, -1), CGXDateTime(-1, -1, -1, 8, -1, -1, -1));
    pAa->SetStatus(AUTO_ANSWER_STATUS_INACTIVE);
    pAa->SetNumberOfCalls(0);
    pAa->SetNumberOfRingsInListeningWindow(1);
    pAa->SetNumberOfRingsOutListeningWindow(2);
    items.push_back(pAa);
}

/*
* Add Modem Configuration object.
*/
void AddModemConfiguration(CGXDLMSObjectCollection& items)
{
    auto* pMc = new CGXDLMSModemConfiguration();
    pMc->SetCommunicationSpeed(DLMS_BAUD_RATE_38400);
    CGXDLMSModemInitialisation init;
    vector<CGXDLMSModemInitialisation> initialisationStrings;
    init.SetRequest("AT");
    init.SetResponse("OK");
    init.SetDelay(0);
    initialisationStrings.push_back(init);
    pMc->SetInitialisationStrings(initialisationStrings);
    items.push_back(pMc);
}

/**
* Add MAC Address Setup object.
*/
void AddMacAddressSetup(CGXDLMSObjectCollection& items)
{
    auto* pMac = new CGXDLMSMacAddressSetup();
    pMac->SetMacAddress("00:11:22:33:44:55:66");
    items.push_back(pMac);
}

/**
* Add IP4 setup object.
*/
CGXDLMSIp4Setup* AddIp4Setup(CGXDLMSObjectCollection& items, std::string& address)
{
    auto* pIp4 = new CGXDLMSIp4Setup();
    pIp4->SetIPAddress(address);
    items.push_back(pIp4);
    return pIp4;
}