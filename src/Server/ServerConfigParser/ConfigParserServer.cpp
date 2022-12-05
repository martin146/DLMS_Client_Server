/**
 * @file ConfigParserClient.cpp
 * @author Filip Grepl
 * @brief Implementation of 'ConfigParserServer' class
 * @date 2020-01-01
 *
 * @copyright Copyright (c) 2020
 *
 */

#include <arpa/inet.h> //Add support for sockets
#include "ServerConfigParser/ConfigParserServer.h"
#include <algorithm>
#include <fstream>
#include <Logger.h>

ConfigParserServer::ConfigParserServer() {
    parserState = ONE_LINE_PARSING;
}

ConfigParserServer::~ConfigParserServer() {
    Logger::LogEvent(Logger::INFO, "ConfigParserServer::ConfigParserServer() Calling destructor");
}

GX_TRACE_LEVEL ConfigParserServer::GetTraceValue(std::string &value) const {
    if (!value.compare("OFF"))
        return GX_TRACE_LEVEL_OFF;
    else if (!value.compare("ERROR"))
        return GX_TRACE_LEVEL_ERROR;
    else if (!value.compare("WARNING"))
        return GX_TRACE_LEVEL_WARNING;
    else if (!value.compare("INFO"))
        return GX_TRACE_LEVEL_INFO;
    else if (!value.compare("VERBOSE"))
        return GX_TRACE_LEVEL_VERBOSE;
    else
        throw ParseException("The trace value: " + value + " is invalid. Valid values are OFF, ERROR, WARNING, INFO or VERBOSE.");
}

DLMS_DATA_TYPE ConfigParserServer::GetDataType(const std::string &value) {
    if (InsensitiveCaseCompare(value, "INT64"))
        return DLMS_DATA_TYPE_INT64;
    if (InsensitiveCaseCompare(value, "DOUBLE") || InsensitiveCaseCompare(value, "FLOAT64"))
        return DLMS_DATA_TYPE_FLOAT64;
    else
        throw ParseException("Cannot convert: " + value + " to DLMS_DATA_TYPE enum.");
}

std::vector<std::string> ConfigParserServer::SplitStringByComma(std::string &value) {
    std::vector<std::string> obisCodes;
    std::string obis;
    this->EraseWhiteSpaces(value);
    std::stringstream ss(value);
    while(std::getline(ss, obis, ','))   obisCodes.push_back(obis);
    return obisCodes;
}

unsigned int ConfigParserServer::ConvertStringIpToUInt(const std::string &value) {
    unsigned int ip = 0;
    std::stringstream ss(value);
    std::string octet;
    while(std::getline(ss, octet, '.'))   {
        ip += this->GetNumberValue<unsigned int>(octet);
        if (ss.rdbuf()->in_avail() > 0)
            ip = ip << 8;
    }
    return ip;
}

DLMS_CLOCK_BASE ConfigParserServer::GetClockBaseValue(const std::string value) const {
    if (InsensitiveCaseCompare(value, "DLMS_CLOCK_BASE_NONE"))
        return DLMS_CLOCK_BASE_NONE;
    else if (InsensitiveCaseCompare(value, "DLMS_CLOCK_BASE_CRYSTAL"))
        return DLMS_CLOCK_BASE_CRYSTAL;
    else if (InsensitiveCaseCompare(value, "DLMS_CLOCK_BASE_FREQUENCY_50"))
        return DLMS_CLOCK_BASE_FREQUENCY_50;
    else if (InsensitiveCaseCompare(value, "DLMS_CLOCK_BASE_FREQUENCY_60"))
        return DLMS_CLOCK_BASE_FREQUENCY_60;
    else if (InsensitiveCaseCompare(value, "DLMS_CLOCK_BASE_GPS"))
        return DLMS_CLOCK_BASE_GPS;
    else if (InsensitiveCaseCompare(value, "DLMS_CLOCK_BASE_RADIO"))
        return DLMS_CLOCK_BASE_RADIO;
    else
        throw ParseException("Cannot convert: " + value +
                             " to Clock Base value. Valid values are DLMS_CLOCK_BASE_NONE, DLMS_CLOCK_BASE_CRYSTAL, "
                             "DLMS_CLOCK_BASE_FREQUENCY_50, DLMS_CLOCK_BASE_FREQUENCY_60, DLMS_CLOCK_BASE_GPS, "
                             "DLMS_CLOCK_BASE_RADIO.");
}