/**
 * @file Device.cpp
 * @author Dominik Majer
 * @brief Implementation of 'ServerConfiguration' class
 * @date 2020-01-01
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include "Device.h"
#include "Logger.h"

#include "GURUX_DLMS/Client/GXDLMSSecureClient.h"
#include <boost/format.hpp>
#include <iostream>

Device::Device(void)
{

}

Device::~Device(void)
{
	Logger::LogEvent(Logger::ERROR, "ServerConfiguration::ServerConfigurationConfiguration()\tCalling destructor");
    delete secureClient;
	delete guruxLinker;
}

int Device::ListAllObjects(void) const
{
    if (secureClient->GetObjects().size() == 0)
    {
        Logger::LogEvent(Logger::ERROR, "No objects uploaded...");
        return N_OK;
    }

    std::vector<std::string> attribs;
    for (std::vector<CGXDLMSObject*>::iterator it = secureClient->GetObjects().begin(); it != secureClient->GetObjects().end(); ++it)
    {
        (*it)->GetValues(attribs);
        printf("[ID:%d]: [%s] -> [%s :: (%s)]\n",
            it - secureClient->GetObjects().begin(),
            (*it)->GetName().ToString().c_str(),
            CGXDLMSClient::ObjectTypeToString((*it)->GetObjectType()).c_str(),
            (*it)->GetDescription().c_str());

        for (std::vector<std::string>::iterator itA = attribs.begin(); itA != attribs.end(); ++itA) {
            printf("\t[%d.%d] :: { %s } ACCESS RIGHTS: ", it - secureClient->GetObjects().begin(),
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

    return OK;
}

int Device::ReadObject(void)
{
    std::string value = "";
    std::string option;

    CGXDLMSObject* obj;
    std::vector<std::string> attribs;

    std::string::size_type sz;

    while (true)
    {
        std::cin >> option;

        try
        {
            if (secureClient->GetUseLogicalNameReferencing())
                obj = secureClient->GetObjects().FindByLN(DLMS_OBJECT_TYPE_ALL, option);
            else
                obj = secureClient->GetObjects().FindBySN(static_cast<unsigned short>(std::stoi(option)));

            if (obj == NULL)
            {
                Logger::LogEvent(
                    Logger::INFO,
                    (boost::format("[%s] -> Object [%s] not found in current Association View") % devConfig.name % option).str()
                );
                continue;
            }


            break;
        }
        catch (...)
        {
            Logger::LogEvent(Logger::INFO, "Invalid Object Number! ");
        }
    }

    obj->GetValues(attribs);

    Logger::LogEvent(
        Logger::ERROR,
        (boost::format("[%s] -> Start Reading Object [%s] ") % devConfig.name % option).str()
    );

    guruxLinker->Connect(netConfig.hostName.c_str(), netConfig.port);
    guruxLinker->InitializeConnection();

    for (auto it = attribs.begin(); it != attribs.end(); ++it) {
        // !! Must be +1 because in GURUX attributes are numbered from 1
        guruxLinker->Read(obj, std::distance(attribs.begin(), it)+1, value);
    }
    Logger::LogEvent(
        Logger::ERROR,
        (boost::format("[%s] -> Object [%s] has been read.") % devConfig.name % option).str()
    );

    guruxLinker->Close();


    return OK;
}

const DeviceConfig &Device::getDevConfig() const {
    return devConfig;
}

void Device::setDevConfig(const DeviceConfig &devConfig) {
    Device::devConfig = devConfig;
}

const Security &Device::getSecurity() const {
    return security;
}

void Device::setSecurity(const Security &security) {
    Device::security = security;
}

const NetConfig &Device::getNetConfig() const {
    return netConfig;
}

void Device::setNetConfig(const NetConfig &netConfig) {
    Device::netConfig = netConfig;
}

CGXCommunication *Device::getGuruxLinker() const {
    return guruxLinker;
}

void Device::setGuruxLinker(CGXCommunication *guruxLinker) {
    Device::guruxLinker = guruxLinker;
}

CGXDLMSSecureClient *Device::getSecureClient() const {
    return secureClient;
}

void Device::setSecureClient(CGXDLMSSecureClient *secureClient) {
    Device::secureClient = secureClient;
}



