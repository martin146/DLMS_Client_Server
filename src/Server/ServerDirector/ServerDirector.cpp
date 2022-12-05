/**
 * @file ServerDirector.cpp
 * @author Filip Grepl
 * @brief Implementation of 'ServerDirector' class
 * @date 2021-10-08
 *
 * @copyright Copyright (c) 2020
 *
 */

#include "ServerDirector/ServerDirector.h"
#include <iostream>
#include <fstream>
#include <ServerConfigParser/ConfigParserServer.h>
#include <Logger.h>
#include <Exceptions/ConfigException.h>
#include <Exceptions/InitException.h>
#include "GURUX_DLMS/Client/GXDLMSClient.h"
#include "ApplicationConfigParser/ApplicationConfigParser.h"


ServerDirector::~ServerDirector() {
    Logger::LogEvent(Logger::INFO, "ServerDirector::~ServerDirector() - Calling destructor");
}

void ServerDirector::CreateServer(const char *pathToConfigFile) {

    ApplicationConfigParser applicationConfigParser(pathToConfigFile, loadedUserConfiguration);
    applicationConfigParser.LoadApplicationConfiguration();

    Logger::InitLogger(loadedUserConfiguration.getDlmsServerPathsConfiguration().getPathToLogFile());


#if defined(_WIN32) || defined(_WIN64)
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        throw ("Could not find a usable WinSock DLL/LIB");
#endif

    ///////////////////////////////////////////////////////////////////////

    server = new GXDLMSBase(loadedUserConfiguration.getDlmsServerConfiguration().isUseLogicalNameReferencing(),
                            loadedUserConfiguration.getDlmsServerConfiguration().getInterfaceType(), loadedUserConfiguration);
    if (loadedUserConfiguration.getDlmsServerConfiguration().getInterfaceType() == DLMS_INTERFACE_TYPE_WRAPPER) {
        server->SetWrapper(loadedUserConfiguration.getLoadedObjects().getTcpUdpSetup());
        server->SetMaxReceivePDUSize(loadedUserConfiguration.getLoadedObjects().getTcpUdpSetup()->GetMaximumSegmentSize());
    } else if (loadedUserConfiguration.getDlmsServerConfiguration().getInterfaceType() == DLMS_INTERFACE_TYPE_HDLC) {
        server->SetHdlc(loadedUserConfiguration.getLoadedObjects().getIecHdlcSetup());
        server->SetMaxReceivePDUSize(loadedUserConfiguration.getLoadedObjects().getIecHdlcSetup()->GetMaximumInfoLengthReceive());

        server->GetLimits().SetWindowSizeRX(loadedUserConfiguration.getLoadedObjects().getIecHdlcSetup()->GetWindowSizeReceive());
        server->GetLimits().SetWindowSizeTX(loadedUserConfiguration.getLoadedObjects().getIecHdlcSetup()->GetWindowSizeTransmit());
        server->GetLimits().SetMaxInfoRX(loadedUserConfiguration.getLoadedObjects().getIecHdlcSetup()->GetMaximumInfoLengthReceive());
        server->GetLimits().SetMaxInfoTX(loadedUserConfiguration.getLoadedObjects().getIecHdlcSetup()->GetMaximumInfoLengthTransmit());
    } else
        throw InitException("Interface type " + std::to_string(loadedUserConfiguration.getDlmsServerConfiguration().getInterfaceType()) + " is not supported!");

    /*auto *sn = new CGXDLMSAssociationShortName();
    sn->SetSecret(tmp);
    if (m_thisDevice.getDevConfig().interfaceType == DLMS_INTERFACE_TYPE_WRAPPER) {
        //Create Gurux DLMS server component for Logical Name and start listen events.
        server = new GXDLMSBase(sn, &m_thisDevice.getTcpUdpSetup(), m_thisDevice, loadedUserConfiguration);
    } else if (m_thisDevice.getDevConfig().interfaceType == DLMS_INTERFACE_TYPE_HDLC) {
        //Create Gurux DLMS server component for Short Name and start listen events.
        server = new GXDLMSBase(sn, &m_thisDevice.getHdlcSetup(), m_thisDevice, loadedUserConfiguration);*/

    // note: useLogicalNameReferencing and Interface type is set by CGXDLMSBase constructor

    CGXByteBuffer tmp;

    // authentication is validated in ValidateAuthentication method

    // security settings
    // http://www.gurux.fi/Gurux.DLMS.Secure


    /*GXHelpers::HexToBytes(m_thisDevice.getSecurity().blockCipherKey, tmp);
    server->GetCiphering()->SetBlockCipherKey(tmp);
    GXHelpers::HexToBytes(m_thisDevice.getSecurity().authenticationKey, tmp);
    server->GetCiphering()->SetAuthenticationKey(tmp);
    GXHelpers::HexToBytes(m_thisDevice.getSecurity().dedicatedKey, tmp);
    server->GetCiphering()->SetDedicatedKey(tmp);*/

    server->Init(loadedUserConfiguration.getDlmsServerConfiguration().getTraceLevel());

}

void ServerDirector::StartServer() const {
    this->server->StartServer();
}

void ServerDirector::CheckCorrectServerAddress() {
    if ((loadedUserConfiguration.getDlmsServerConfiguration().getAddressSize() == 1 || loadedUserConfiguration.getDlmsServerConfiguration().getAddressSize() == 2) &&
        loadedUserConfiguration.getDlmsServerConfiguration().getLogicalAddress() > 0x7F) {
        throw RuntimeException( "Server address size is set to " + std::to_string( loadedUserConfiguration.getDlmsServerConfiguration().getAddressSize())
                                + ", but length of the logical address is higher than 0x7F.");
    }
    if (loadedUserConfiguration.getDlmsServerConfiguration().getAddressSize() == 1 && loadedUserConfiguration.getDlmsServerConfiguration().getPhysicalAddress() != 0x0) {
        throw RuntimeException("Server address size is set to 1, but length of the physical address is higher than 0.");
    }
    if (loadedUserConfiguration.getDlmsServerConfiguration().getAddressSize() == 2 && loadedUserConfiguration.getDlmsServerConfiguration().getPhysicalAddress() > 0x7F) {
        throw RuntimeException("Server address size is set to 2, but length of the physical address is higher than 0x7F.");
    }
    if (loadedUserConfiguration.getDlmsServerConfiguration().getAddressSize() == 4) {
        if (loadedUserConfiguration.getDlmsServerConfiguration().getPhysicalAddress() > 0x3FFF) {
            throw RuntimeException("Server address size is set to 4, but length of the physical address is higher than 0x3FFF.");
        }
        if (loadedUserConfiguration.getDlmsServerConfiguration().getLogicalAddress() > 0x3FFF) {
            throw RuntimeException( "Server address size is set to 1, but length of the logical address size is higher than 0x3FFF.");
        }
    }
}


void ServerDirector::PrintConfig() {
    std::cout << "*********************** OBJECTS ***********************" << std::endl;

    std::vector<std::string> attribs;
    for (auto it = server->GetItems().begin(); it != server->GetItems().end(); ++it) {
        (*it)->GetValues(attribs);
        printf("[ID:%ld]: [%s] -> [%s :: (%s)]\n",
               it - server->GetItems().begin(),
               (*it)->GetName().ToString().c_str(),
               CGXDLMSConverter::ToString((*it)->GetObjectType()),
               (*it)->GetDescription().c_str());

        for (auto itA = attribs.begin(); itA != attribs.end(); ++itA) {
            printf("\t[%d.%d] :: { %s } ACCESS RIGHTS: ", it - server->GetItems().begin(),
                   itA - attribs.begin(), (*itA).c_str());
            switch ((*it)->GetAccess(itA - attribs.begin() + 1)) {
                case DLMS_ACCESS_MODE_NONE:
                    printf("%s \n", "DLMS_ACCESS_MODE_NONE");
                    break;
                case DLMS_ACCESS_MODE_READ:
                    printf("%s \n", "DLMS_ACCESS_MODE_READ");
                    break;
                case DLMS_ACCESS_MODE_WRITE:
                    printf("%s \n", "DLMS_ACCESS_MODE_WRITE");
                    break;
                case DLMS_ACCESS_MODE_READ_WRITE:
                    printf("%s \n", "DLMS_ACCESS_MODE_READ_WRITE");
                    break;
            }
        }
    }

    std::cout << "*********************** END ***********************" << std::endl;

    std::string IPv4SetupObis = loadedUserConfiguration.getLoadedObjects().getTcpUdpSetup()->GetIPReference();

    std::cout << loadedUserConfiguration.getDlmsServerConfiguration().getName() << " - "
              << loadedUserConfiguration.getDlmsServerConfiguration().isUseLogicalNameReferencing() << " - "
              << loadedUserConfiguration.getLoadedObjects().getTcpUdpSetup()->GetPort() << " - "
              << ((CGXDLMSIp4Setup*)loadedUserConfiguration.getLoadedObjects().FindByLN(DLMS_OBJECT_TYPE_ALL,
                          IPv4SetupObis))->GetIPAddress()
             << std::endl;
}