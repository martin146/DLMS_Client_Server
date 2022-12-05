/**
 * @file ConfigParserClient.cpp
 * @author Dominik Majer
 * @brief Implementation of 'ConfigParserClient' class
 * @date 2020-01-01
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#if defined(_WIN32) || defined(_WIN64)
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WS2tcpip.h>
#endif

#include <algorithm>

#include "ConfigParserClient.h"
#include "Logger.h"

ConfigParserClient::ConfigParserClient() {

}

ConfigParserClient::~ConfigParserClient() {
    Logger::LogEvent(Logger::ERROR, "ConfigParserClient::ConfigParserClientClient() Calling destructor");
}


void ConfigParserClient::ParseConfigLine(Device *dev, std::string line) {
    std::string param;
    std::string value;
    EraseWhiteSpaces(line);
    RemoveComment(line);
    int paramDelimiter;

    if (line.empty())
        return;

    paramDelimiter = line.find(CONFIG_DELIMITER);

    if (paramDelimiter == std::string::npos &&!m_confFlag) // not valid line -> skip it
        throw ParseException("Unknown line in config file: " + line);
    else if (paramDelimiter != std::string::npos && m_confFlag) {
        m_confFlag = false;
    } else if (m_confFlag) {
        dev->devConfig.conformance = (DLMS_CONFORMANCE) (dev->devConfig.conformance | GetConformance(line));
        return;
    }

    param.assign(line.substr(0, paramDelimiter));

    if (!m_confFlag && InsensitiveCaseCompare(param, "conformance") && (paramDelimiter == param.length())) {
        m_confFlag = true;
        return;
    }



    value.assign(line.substr(paramDelimiter + 1));
    m_confFlag = false;

    if (InsensitiveCaseCompare("name", param)) {
        dev->devConfig.name.assign(value);
    } else if (InsensitiveCaseCompare("manufacturer", param)) {
        dev->devConfig.manufacturer.assign(value);
    } else if (InsensitiveCaseCompare("serialNumber", param)) {
        dev->devConfig.serialNumber.assign(value);
    } else if (InsensitiveCaseCompare("useLogicalNameReferencing", param)) {
        dev->devConfig.useLogicalNameReferencing = GetBoolValue(value);
    } else if (InsensitiveCaseCompare("authentication", param)) {
        dev->security.authentication = GetAuthenticationMechanismType(value);
    } else if (InsensitiveCaseCompare("security", param)) {
        dev->security.securityPolicy1 = GetSecPolicy1Value(value);
    } else if (InsensitiveCaseCompare("clientAddress", param)) {
        dev->devConfig.clientAddress = GetNumberValue<BYTE>(value);
            //return INVALID_VALUE;
    } else if (InsensitiveCaseCompare("password", param)) {
        dev->security.password.assign(value);
    } else if (InsensitiveCaseCompare("invocationCounter", param)) {
        dev->devConfig.invocationCounter = GetNumberValue<int>(value);
            //return INVALID_VALUE;
    } else if (InsensitiveCaseCompare("interfaceType", param)) {
        if (InsensitiveCaseCompare(value, "HDLC"))
            dev->devConfig.interfaceType = DLMS_INTERFACE_TYPE_HDLC;
        else if (InsensitiveCaseCompare(value, "WRAPPER"))
            dev->devConfig.interfaceType = DLMS_INTERFACE_TYPE_WRAPPER;
        else
            throw ParseException("Invalid value for interfaceType: " + value);
    } else if (InsensitiveCaseCompare("logicalServer", param)) {
        dev->devConfig.logicalServerM = GetNumberValue<unsigned int>(value);
           // return INVALID_VALUE;
    } else if (InsensitiveCaseCompare("physicalServer", param)) {
        dev->devConfig.physicalServerM = GetNumberValue<unsigned int>(value);
           // return INVALID_VALUE;
    } else if (InsensitiveCaseCompare("hostName", param)) {
        struct sockaddr_in sa;
        if (inet_pton(AF_INET, value.c_str(), &(sa.sin_addr)))
            dev->netConfig.hostName.assign(value);
        else
            throw ParseException("Invalid value for hostname: " + value);
    } else if (InsensitiveCaseCompare("port", param)) {
        dev->netConfig.port = GetNumberValue<unsigned short>(value);
    } else if (InsensitiveCaseCompare("ipTransportProtocol", param)) {
        if (!value.compare("TCP"))
            dev->netConfig.ipTransportProtocol = TCP;
        else if (!value.compare("UDP"))
            dev->netConfig.ipTransportProtocol = UDP;
        else
            throw ParseException("The ip protocol type: " + value + "is invalid. The value must be between TCP or UDP.");
    } else if (InsensitiveCaseCompare("phyDevAddress", param)) {
        dev->netConfig.macAddress.assign(value);
    } else if (InsensitiveCaseCompare("waitTime", param)) {
        dev->netConfig.waitTime = GetNumberValue<unsigned short >(value);
            //return INVALID_VALUE;
    } else if (InsensitiveCaseCompare("windowsizeintransit", param)) {
        dev->devConfig.maxWindowSize.transmit = GetNumberValue<unsigned int>(value);
            if (dev->devConfig.maxWindowSize.transmit > 7) {
                throw ParseException("The size of windowSizeTransmit is out of range: " + std::to_string(dev->devConfig.maxWindowSize.transmit) +
                                     ". The value must be between 1 and 7.");
            }
    } else if (InsensitiveCaseCompare("windowsizeinreceive", param)) {
        dev->devConfig.maxWindowSize.receive = GetNumberValue<unsigned short>(value);
        if (dev->devConfig.maxWindowSize.receive > 7) {
            throw ParseException("The size of windowSizeReceive is out of range: " + std::to_string(dev->devConfig.maxWindowSize.transmit) +
                                 ". The value must be between 1 and 7.");
        }
    } else if (InsensitiveCaseCompare("serverAddressSize", param)) {
        dev->devConfig.serverAddressSize = GetNumberValue<unsigned int>(value);
        if (dev->devConfig.serverAddressSize > 4 || dev->devConfig.serverAddressSize == 0 || dev->devConfig.serverAddressSize == 3) {
            throw ParseException("Invalid value of server address size: " + std::to_string(dev->devConfig.serverAddressSize) + ". Possible values are 1,2 or 4.");
        }
    } else if (InsensitiveCaseCompare("inactivityTimeout", param)) {
        dev->devConfig.inactivityTimeout = GetNumberValue<unsigned int>(value);
            //return INVALID_VALUE;
    } else if (InsensitiveCaseCompare("priority", param)) {
        if (InsensitiveCaseCompare(value, "NORMAL"))
            dev->devConfig.priority = DLMS_PRIORITY_NORMAL;
        else if (InsensitiveCaseCompare(value, "HIGH"))
            dev->devConfig.priority = DLMS_PRIORITY_HIGH;
        else
            throw ParseException("The priority value: " + param + " is invalid. Please enter 'NORMAL' or 'HIGH' priority.");
    } else if (InsensitiveCaseCompare("serviceClass", param)) {
        if (InsensitiveCaseCompare(value, "UNCONFIRMED"))
            dev->devConfig.serviceClass = DLMS_SERVICE_CLASS_UN_CONFIRMED;
        else if (InsensitiveCaseCompare(value, "CONFIRMED"))
            dev->devConfig.serviceClass = DLMS_SERVICE_CLASS_CONFIRMED;
        else
            throw ParseException("The serviceClass value: " + param + " is invalid. Please enter 'UNCONFIRMED' or 'CONFIRMED' service class.");
    } else if (InsensitiveCaseCompare("maxPayloadsizeInTransit", param)) {
        dev->devConfig.maxPayloadSize.transmit = GetNumberValue<unsigned int>(value);
            //return INVALID_VALUE;
    } else if (InsensitiveCaseCompare("maxPayloadsizeInReceive", param)) {
        dev->devConfig.maxPayloadSize.receive = GetNumberValue<unsigned int>(value);
            //return INVALID_VALUE;
    } else if (InsensitiveCaseCompare("userID", param)) {
        dev->security.userID = GetNumberValue<BYTE>(value);
            //return INVALID_VALUE;

    } else if (InsensitiveCaseCompare("sysTitle", param)) {
        dev->security.sysTitle.assign(value.substr(2, value.length() - 2));
    } else if (InsensitiveCaseCompare("maxPDUsize", param)) {
        dev->devConfig.maxPDUsize = GetNumberValue<unsigned int>(value);
            //return INVALID_VALUE;
    } else if (InsensitiveCaseCompare("authenticationKey", param)) {
        dev->security.authenticationKey.assign(value.substr(2, value.length() - 2));
    } else if (InsensitiveCaseCompare("blockCipherKey", param)) {
        dev->security.blockCipherKey.assign(value.substr(2, value.length() - 2));
    } else if (InsensitiveCaseCompare("dedicatedKey", param)) {
        dev->security.dedicatedKey.assign(value.substr(2, value.length() - 2));
    } else {
        throw ParseException("The parameter: " + param + " is unknown.");
    }
}
