/**
 * @file ConfigParser.cpp
 * @author Filip Grepl
 * @brief Implementation of 'ConfigParser' class
 * @date 2021-10-08
 *
 * @copyright Copyright (c) 2020
 *
 */

#include <algorithm>
#include <ConfigParser.h>

bool ConfigParser::InsensitiveCaseCompare(const std::string &a, const std::string &b) const {
    return std::equal(a.begin(), a.end(),
                      b.begin(), b.end(),
                      [](char a, char b) {
                          return tolower(a) == tolower(b);
                      });
}

void ConfigParser::EraseWhiteSpaces(std::string &line) const {
    line.erase(std::remove_if(line.begin(),
                              line.end(),
                              [](unsigned char x) { return std::isspace(x); }),
               line.end());
}

DLMS_BAUD_RATE ConfigParser::GetCommSpeed(const std::string &value) const {
    if (InsensitiveCaseCompare(value, "DLMS_BAUD_RATE_300"))
        return DLMS_BAUD_RATE_300;
    else if (InsensitiveCaseCompare(value, "DLMS_BAUD_RATE_600"))
        return DLMS_BAUD_RATE_600;
    else if (InsensitiveCaseCompare(value, "DLMS_BAUD_RATE_1200"))
        return DLMS_BAUD_RATE_1200;
    else if (InsensitiveCaseCompare(value, "DLMS_BAUD_RATE_2400"))
        return DLMS_BAUD_RATE_2400;
    else if (InsensitiveCaseCompare(value, "DLMS_BAUD_RATE_4800"))
        return DLMS_BAUD_RATE_4800;
    else if (InsensitiveCaseCompare(value, "DLMS_BAUD_RATE_9600"))
        return DLMS_BAUD_RATE_9600;
    else if (InsensitiveCaseCompare(value, "DLMS_BAUD_RATE_19200"))
        return DLMS_BAUD_RATE_19200;
    else if (InsensitiveCaseCompare(value, "DLMS_BAUD_RATE_38400"))
        return DLMS_BAUD_RATE_38400;
    else if (InsensitiveCaseCompare(value, "DLMS_BAUD_RATE_57600"))
        return DLMS_BAUD_RATE_57600;
    else if (InsensitiveCaseCompare(value, "DLMS_BAUD_RATE_115200"))
        return DLMS_BAUD_RATE_115200;
    else
        throw ParseException("The baud rate value: " + value +
                             " is invalid. Valid values are: 300, 600, 1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200");
}

void ConfigParser::RemoveComment(std::string &line) const {
    int startCommentPos = line.find("//");
    if (startCommentPos != std::string::npos) {
        line = line.substr(0, startCommentPos);
    }
}


DLMS_AUTHENTICATION ConfigParser::GetAuthenticationMechanismType(const std::string &value) const {
    if (InsensitiveCaseCompare(value, "NO_SECURITY"))
        return DLMS_AUTHENTICATION_NONE;
    else if (InsensitiveCaseCompare(value, "DLMS_AUTHENTICATION_LOW"))
        return DLMS_AUTHENTICATION_LOW;
    else if (InsensitiveCaseCompare(value, "DLMS_AUTHENTICATION_HIGH"))
        return DLMS_AUTHENTICATION_HIGH;
    else if (InsensitiveCaseCompare(value, "DLMS_AUTHENTICATION_HIGH_MD5"))
        return DLMS_AUTHENTICATION_HIGH_MD5;
    else if (InsensitiveCaseCompare(value, "DLMS_AUTHENTICATION_HIGH_SHA1"))
        return DLMS_AUTHENTICATION_HIGH_SHA1;
    else if (InsensitiveCaseCompare(value, "DLMS_AUTHENTICATION_HIGH_GMAC"))
        return DLMS_AUTHENTICATION_HIGH_GMAC;
    else if (InsensitiveCaseCompare(value, "DLMS_AUTHENTICATION_HIGH_SHA256"))
        return DLMS_AUTHENTICATION_HIGH_SHA256;
    else if (InsensitiveCaseCompare(value, "DLMS_AUTHENTICATION_HIGH_ECDSA"))
        return DLMS_AUTHENTICATION_HIGH_ECDSA;
    else
        throw ParseException("Cannot convert: " + value +
                             " to Authentication mechanism. Valid values are NO_SECURITY, DLMS_AUTHENTICATION_LOW, "
                             "DLMS_AUTHENTICATION_HIGH, DLMS_AUTHENTICATION_HIGH_MD5, DLMS_AUTHENTICATION_HIGH_SHA1, "
                             "DLMS_AUTHENTICATION_HIGH_GMAC, DLMS_AUTHENTICATION_HIGH_SHA256, "
                             "DLMS_AUTHENTICATION_HIGH_ECDSA.");
}

DLMS_INTERFACE_TYPE ConfigParser::GetInterfaceType(const std::string &value) const {
    if (InsensitiveCaseCompare(value, "DLMS_INTERFACE_TYPE_HDLC"))
        return DLMS_INTERFACE_TYPE_HDLC;
    else if (InsensitiveCaseCompare(value, "DLMS_INTERFACE_TYPE_WRAPPER"))
        return DLMS_INTERFACE_TYPE_WRAPPER;
    else if (InsensitiveCaseCompare(value, "DLMS_INTERFACE_TYPE_PDU"))
        return DLMS_INTERFACE_TYPE_PDU;
    else if (InsensitiveCaseCompare(value, "DLMS_INTERFACE_TYPE_WIRELESS_MBUS"))
        return DLMS_INTERFACE_TYPE_WIRELESS_MBUS;
    else if (InsensitiveCaseCompare(value, "DLMS_INTERFACE_TYPE_HDLC_WITH_MODE_E"))
        return DLMS_INTERFACE_TYPE_HDLC_WITH_MODE_E;
    else if (InsensitiveCaseCompare(value, "DLMS_INTERFACE_TYPE_PLC"))
        return DLMS_INTERFACE_TYPE_PLC;
    else if (InsensitiveCaseCompare(value, "DLMS_INTERFACE_TYPE_PLC_HDLC"))
        return DLMS_INTERFACE_TYPE_PLC_HDLC;
    else if (InsensitiveCaseCompare(value, "DLMS_INTERFACE_TYPE_LPWAN"))
        return DLMS_INTERFACE_TYPE_LPWAN;
    else if (InsensitiveCaseCompare(value, "DLMS_INTERFACE_TYPE_WI_SUN"))
        return DLMS_INTERFACE_TYPE_WI_SUN;
    else if (InsensitiveCaseCompare(value, "DLMS_INTERFACE_TYPE_PLC_PRIME"))
        return DLMS_INTERFACE_TYPE_PLC_PRIME;
    else
        throw ParseException("Cannot convert: " + value +
                             " to interface type. Valid values are DLMS_INTERFACE_TYPE_HDLC, DLMS_INTERFACE_TYPE_WRAPPER, "
                             "DLMS_INTERFACE_TYPE_PDU, DLMS_INTERFACE_TYPE_WIRELESS_MBUS, DLMS_INTERFACE_TYPE_HDLC_WITH_MODE_E"
                             ", DLMS_INTERFACE_TYPE_PLC, DLMS_INTERFACE_TYPE_PLC_HDLC, DLMS_INTERFACE_TYPE_LPWAN"
                             ", DLMS_INTERFACE_TYPE_WI_SUN, DLMS_INTERFACE_TYPE_PLC_PRIME.");
}

DLMS_SECURITY_POLICY1 ConfigParser::GetSecPolicy1Value(const std::string value) const {
    if (InsensitiveCaseCompare(value, "DLMS_SECURITY_POLICY1_NOTHING"))
        return DLMS_SECURITY_POLICY1_NOTHING;
    else if (InsensitiveCaseCompare(value, "DLMS_SECURITY_POLICY1_AUTHENTICATED_REQUEST"))
        return DLMS_SECURITY_POLICY1_AUTHENTICATED_REQUEST;
    else if (InsensitiveCaseCompare(value, "DLMS_SECURITY_POLICY1_ENCRYPTED_REQUEST"))
        return DLMS_SECURITY_POLICY1_ENCRYPTED_REQUEST;
    else if (InsensitiveCaseCompare(value, "DLMS_SECURITY_POLICY1_DIGITALLY_SIGNED_REQUEST"))
        return DLMS_SECURITY_POLICY1_DIGITALLY_SIGNED_REQUEST;
    else if (InsensitiveCaseCompare(value, "DLMS_SECURITY_POLICY1_AUTHENTICATED_RESPONSE"))
        return DLMS_SECURITY_POLICY1_AUTHENTICATED_RESPONSE;
    else if (InsensitiveCaseCompare(value, "DLMS_SECURITY_POLICY1_ENCRYPTED_RESPONSE"))
        return DLMS_SECURITY_POLICY1_ENCRYPTED_RESPONSE;
    else if (InsensitiveCaseCompare(value, "DLMS_SECURITY_POLICY1_DIGITALLY_SIGNED_RESPONSE"))
        return DLMS_SECURITY_POLICY1_DIGITALLY_SIGNED_RESPONSE;
    else
        throw ParseException("Cannot convert: " + value +
                                    " to Security Policy value of version 1. Please enter DLMS_SECURITY_POLICY1_NOTHING, DLMS_SECURITY_POLICY1_AUTHENTICATED_REQUEST, "
                                    " DLMS_SECURITY_POLICY1_ENCRYPTED_REQUEST, DLMS_SECURITY_POLICY1_DIGITALLY_SIGNED_REQUEST,"
                                    " DLMS_SECURITY_POLICY1_AUTHENTICATED_RESPONSE, DLMS_SECURITY_POLICY1_ENCRYPTED_RESPONSE, "
                                    "or DLMS_SECURITY_POLICY1_DIGITALLY_SIGNED_RESPONSE.");
}

DLMS_APPLICATION_CONTEXT_NAME ConfigParser::GetContextId(const std::string value) const {
    if (InsensitiveCaseCompare(value, "DLMS_APPLICATION_CONTEXT_NAME_LOGICAL_NAME"))
        return DLMS_APPLICATION_CONTEXT_NAME_LOGICAL_NAME;
    else if (InsensitiveCaseCompare(value, "DLMS_APPLICATION_CONTEXT_NAME_SHORT_NAME"))
        return DLMS_APPLICATION_CONTEXT_NAME_SHORT_NAME;
    else if (InsensitiveCaseCompare(value, "DLMS_APPLICATION_CONTEXT_NAME_LOGICAL_NAME_WITH_CIPHERING"))
        return DLMS_APPLICATION_CONTEXT_NAME_LOGICAL_NAME_WITH_CIPHERING;
    else if (InsensitiveCaseCompare(value, "DLMS_APPLICATION_CONTEXT_NAME_SHORT_NAME_WITH_CIPHERING"))
        return DLMS_APPLICATION_CONTEXT_NAME_SHORT_NAME_WITH_CIPHERING;
    else
        throw ParseException("Cannot convert: " + value +
                             " to Context id value of version 1. Please enter DLMS_APPLICATION_CONTEXT_NAME_LOGICAL_NAME, "
                             " DLMS_APPLICATION_CONTEXT_NAME_SHORT_NAME, DLMS_APPLICATION_CONTEXT_NAME_LOGICAL_NAME_WITH_CIPHERING, "
                             " DLMS_APPLICATION_CONTEXT_NAME_SHORT_NAME_WITH_CIPHERING.");
}

DLMS_SECURITY_SUITE ConfigParser::GetSecuritySuite(const std::string value) const {
    if (InsensitiveCaseCompare(value, "DLMS_SECURITY_SUITE_V0"))
        return DLMS_SECURITY_SUITE_V0;
    else if (InsensitiveCaseCompare(value, "DLMS_SECURITY_SUITE_V1"))
        return DLMS_SECURITY_SUITE_V1;
    else
        throw ParseException("Cannot convert: " + value +
                             " to Security Suite. Please enter DLMS_SECURITY_SUITE_V0, "
                             " DLMS_SECURITY_SUITE_V1.");
}

DLMS_CERTIFICATE_ENTITY ConfigParser::GetCertificateEntity(const std::string value) const {
    if (InsensitiveCaseCompare(value, "DLMS_CERTIFICATE_ENTITY_SERVER"))
        return DLMS_CERTIFICATE_ENTITY_SERVER;
    else if (InsensitiveCaseCompare(value, "DLMS_CERTIFICATE_ENTITY_CLIENT"))
        return DLMS_CERTIFICATE_ENTITY_CLIENT;
    else if (InsensitiveCaseCompare(value, "DLMS_CERTIFICATE_ENTITY_CERTIFICATION_AUTHORITY"))
        return DLMS_CERTIFICATE_ENTITY_CERTIFICATION_AUTHORITY;
    else if (InsensitiveCaseCompare(value, "OTHER"))
        return OTHER;
    else
        throw ParseException("Cannot convert: " + value +
                             " to Certificate Authority. Please enter DLMS_CERTIFICATE_ENTITY_SERVER, "
                             " DLMS_CERTIFICATE_ENTITY_CLIENT, DLMS_CERTIFICATE_ENTITY_CERTIFICATION_AUTHORITY,"
                             " OTHER.");
}

DLMS_CERTIFICATE_TYPE ConfigParser::GetCertificateType(const std::string value) const {
    if (InsensitiveCaseCompare(value, "DLMS_CERTIFICATE_TYPE_DIGITAL_SIGNATURE"))
        return DLMS_CERTIFICATE_TYPE_DIGITAL_SIGNATURE;
    else if (InsensitiveCaseCompare(value, "DLMS_CERTIFICATE_TYPE_KEY_AGREEMENT"))
        return DLMS_CERTIFICATE_TYPE_KEY_AGREEMENT;
    else if (InsensitiveCaseCompare(value, "DLMS_CERTIFICATE_TYPE_TLS"))
        return DLMS_CERTIFICATE_TYPE_TLS;
    else if (InsensitiveCaseCompare(value, "DLMS_CERTIFICATE_TYPE_OTHER"))
        return DLMS_CERTIFICATE_TYPE_OTHER;
    else
        throw ParseException("Cannot convert: " + value +
                             " to Certificate type. Please enter DLMS_CERTIFICATE_TYPE_DIGITAL_SIGNATURE, "
                             " DLMS_CERTIFICATE_TYPE_KEY_AGREEMENT, DLMS_CERTIFICATE_TYPE_TLS,"
                             " DLMS_CERTIFICATE_TYPE_OTHER.");
}

IP_OPTION_TYPE ConfigParser::GetIpOptionType(const std::string value) const {
    if (InsensitiveCaseCompare(value, "IP_OPTION_TYPE_DLMS_SECURITY"))
        return IP_OPTION_TYPE_DLMS_SECURITY;
    else if (InsensitiveCaseCompare(value, "IP_OPTION_TYPE_LOOSE_SOURCE_AND_RECORD_ROUTE"))
        return IP_OPTION_TYPE_LOOSE_SOURCE_AND_RECORD_ROUTE;
    else if (InsensitiveCaseCompare(value, "IP_OPTION_TYPE_STRICT_SOURCE_AND_RECORD_ROUTE"))
        return IP_OPTION_TYPE_STRICT_SOURCE_AND_RECORD_ROUTE;
    else if (InsensitiveCaseCompare(value, "IP_OPTION_TYPE_RECORD_ROUTE"))
        return IP_OPTION_TYPE_RECORD_ROUTE;
    else if (InsensitiveCaseCompare(value, "IP_OPTION_TYPE_INTERNET_TIMESTAMP"))
        return IP_OPTION_TYPE_INTERNET_TIMESTAMP;
    else
        throw ParseException("Cannot convert: " + value +
                             " to Ip option type. Please enter IP_OPTION_TYPE_DLMS_SECURITY, "
                             " IP_OPTION_TYPE_LOOSE_SOURCE_AND_RECORD_ROUTE, IP_OPTION_TYPE_STRICT_SOURCE_AND_RECORD_ROUTE,"
                             " IP_OPTION_TYPE_RECORD_ROUTE, IP_OPTION_TYPE_INTERNET_TIMESTAMP.");
}

GX_TRACE_LEVEL ConfigParser::GetTraceLevel(const std::string value) const {
    if (InsensitiveCaseCompare(value, "OFF"))
        return GX_TRACE_LEVEL_OFF;
    else if (InsensitiveCaseCompare(value, "ERROR"))
        return GX_TRACE_LEVEL_ERROR;
    else if (InsensitiveCaseCompare(value, "WARNING"))
        return GX_TRACE_LEVEL_WARNING;
    else if (InsensitiveCaseCompare(value, "INFO"))
        return GX_TRACE_LEVEL_INFO;
    else if (InsensitiveCaseCompare(value, "VERBOSE"))
        return GX_TRACE_LEVEL_VERBOSE;
    else
        throw ParseException("Cannot convert: " + value +
                             " to trace level. Please enter OFF, ERROR, WARNING, INFO, VERBOSE.");
}

bool ConfigParser::GetBoolValue(const std::string &value) const {
    if (InsensitiveCaseCompare(value, "true"))
        return true;
    else if (InsensitiveCaseCompare(value, "false"))
        return false;
    else
        throw ParseException("Cannot convert: " + value + " to boolean. Please enter 'true' or 'false'");
}

DLMS_CONFORMANCE ConfigParser::GetConformance(const std::string &value) const {
    if (InsensitiveCaseCompare(value, "GENERAL_PROTECTION"))
        return DLMS_CONFORMANCE_GENERAL_PROTECTION;
    else if (InsensitiveCaseCompare(value, "GENERAL_BLOCK_TRANSFER"))
        return DLMS_CONFORMANCE_GENERAL_BLOCK_TRANSFER;
    else if (InsensitiveCaseCompare(value, "READ"))
        return DLMS_CONFORMANCE_READ;
    else if (InsensitiveCaseCompare(value, "WRITE"))
        return DLMS_CONFORMANCE_WRITE;
    else if (InsensitiveCaseCompare(value, "UN_CONFIRMED_WRITE"))
        return DLMS_CONFORMANCE_UN_CONFIRMED_WRITE;
    else if (InsensitiveCaseCompare(value, "ATTRIBUTE_0_SUPPORTED_WITH_SET"))
        return DLMS_CONFORMANCE_ATTRIBUTE_0_SUPPORTED_WITH_SET;
    else if (InsensitiveCaseCompare(value, "PRIORITY_MGMT_SUPPORTED"))
        return DLMS_CONFORMANCE_PRIORITY_MGMT_SUPPORTED;
    else if (InsensitiveCaseCompare(value, "ATTRIBUTE_0_SUPPORTED_WITH_GET"))
        return DLMS_CONFORMANCE_ATTRIBUTE_0_SUPPORTED_WITH_GET;
    else if (InsensitiveCaseCompare(value, "BLOCK_TRANSFER_WITH_GET_OR_READ"))
        return DLMS_CONFORMANCE_BLOCK_TRANSFER_WITH_GET_OR_READ;
    else if (InsensitiveCaseCompare(value, "BLOCK_TRANSFER_WITH_SET_OR_WRITE"))
        return DLMS_CONFORMANCE_BLOCK_TRANSFER_WITH_SET_OR_WRITE;
    else if (InsensitiveCaseCompare(value, "BLOCK_TRANSFER_WITH_ACTION"))
        return DLMS_CONFORMANCE_BLOCK_TRANSFER_WITH_ACTION;
    else if (InsensitiveCaseCompare(value, "MULTIPLE_REFERENCES"))
        return DLMS_CONFORMANCE_MULTIPLE_REFERENCES;
    else if (InsensitiveCaseCompare(value, "INFORMATION_REPORT"))
        return DLMS_CONFORMANCE_INFORMATION_REPORT;
    else if (InsensitiveCaseCompare(value, "DATA_NOTIFICATION"))
        return DLMS_CONFORMANCE_DATA_NOTIFICATION;
    else if (InsensitiveCaseCompare(value, "ACCESS"))
        return DLMS_CONFORMANCE_ACCESS;
    else if (InsensitiveCaseCompare(value, "PARAMETERIZED_ACCESS"))
        return DLMS_CONFORMANCE_PARAMETERIZED_ACCESS;
    else if (InsensitiveCaseCompare(value, "GET"))
        return DLMS_CONFORMANCE_GET;
    else if (InsensitiveCaseCompare(value, "SET"))
        return DLMS_CONFORMANCE_SET;
    else if (InsensitiveCaseCompare(value, "SELECTIVE_ACCESS"))
        return DLMS_CONFORMANCE_SELECTIVE_ACCESS;
    else if (InsensitiveCaseCompare(value, "EVENT_NOTIFICATION"))
        return DLMS_CONFORMANCE_EVENT_NOTIFICATION;
    else if (InsensitiveCaseCompare(value, "ACTION"))
        return DLMS_CONFORMANCE_ACTION;
    else
        throw ParseException("Cannot convert: " + value +
                                    " to Conformance value. Please enter GENERAL_PROTECTION, GENERAL_BLOCK_TRANSFER, READ,"
                                    " WRITE, UN_CONFIRMED_WRITE, ATTRIBUTE_0_SUPPORTED_WITH_SET, PRIORITY_MGMT_SUPPORTED, ATTRIBUTE_0_SUPPORTED_WITH_GET,"
                                    " BLOCK_TRANSFER_WITH_GET_OR_READ, BLOCK_TRANSFER_WITH_SET_OR_WRITE, BLOCK_TRANSFER_WITH_ACTION, MULTIPLE_REFERENCES,"
                                    " INFORMATION_REPORT, INFORMATION_REPORT, DATA_NOTIFICATION, ACCESS, PARAMETERIZED_ACCESS, GET, SET, SELECTIVE_ACCESS,"
                                    " EVENT_NOTIFICATION, ACTION");
}

DLMS_CLOCK_STATUS ConfigParser::GetClockStatus(const std::string &value) const {
    if (InsensitiveCaseCompare(value, "DLMS_CLOCK_STATUS_OK"))
        return DLMS_CLOCK_STATUS_OK;
    else if (InsensitiveCaseCompare(value, "DLMS_CLOCK_STATUS_INVALID_VALUE"))
        return DLMS_CLOCK_STATUS_INVALID_VALUE;
    else if (InsensitiveCaseCompare(value, "DLMS_CLOCK_STATUS_DOUBTFUL_VALUE"))
        return DLMS_CLOCK_STATUS_DOUBTFUL_VALUE;
    else if (InsensitiveCaseCompare(value, "DLMS_CLOCK_STATUS_DIFFERENT_CLOCK_BASE"))
        return DLMS_CLOCK_STATUS_DIFFERENT_CLOCK_BASE;
    else if (InsensitiveCaseCompare(value, "DLMS_CLOCK_STATUS_DAYLIGHT_SAVE_ACTIVE"))
        return DLMS_CLOCK_STATUS_DAYLIGHT_SAVE_ACTIVE;
    else if (InsensitiveCaseCompare(value, "DLMS_CLOCK_STATUS_SKIP"))
        return DLMS_CLOCK_STATUS_SKIP;
    else
        throw ParseException("Cannot convert: " + value + " to DLMS_CLOCK_STATUS enum.");
}

DLMS_UNIT ConfigParser::GetUnit(const std::string &value) const {
    if (InsensitiveCaseCompare(value, "DLMS_UNIT_NONE"))
        return DLMS_UNIT_NONE;
    else if (InsensitiveCaseCompare(value, "DLMS_UNIT_YEAR"))
        return DLMS_UNIT_YEAR;
    else if (InsensitiveCaseCompare(value, "DLMS_UNIT_MONTH"))
        return DLMS_UNIT_MONTH;
    else if (InsensitiveCaseCompare(value, "DLMS_UNIT_WEEK"))
        return DLMS_UNIT_WEEK;
    else if (InsensitiveCaseCompare(value, "DLMS_UNIT_DAY"))
        return DLMS_UNIT_DAY;
    else if (InsensitiveCaseCompare(value, "DLMS_UNIT_HOUR"))
        return DLMS_UNIT_HOUR;
    else if (InsensitiveCaseCompare(value, "DLMS_UNIT_MINUTE"))
        return DLMS_UNIT_MINUTE;
    else if (InsensitiveCaseCompare(value, "DLMS_UNIT_SECOND"))
        return DLMS_UNIT_SECOND;
    else if (InsensitiveCaseCompare(value, "DLMS_UNIT_PHASE_ANGLE_DEGREE"))
        return DLMS_UNIT_PHASE_ANGLE_DEGREE;
    else if (InsensitiveCaseCompare(value, "DLMS_UNIT_TEMPERATURE"))
        return DLMS_UNIT_TEMPERATURE;
    else if (InsensitiveCaseCompare(value, "DLMS_UNIT_LOCAL_CURRENCY"))
        return DLMS_UNIT_LOCAL_CURRENCY;
    else if (InsensitiveCaseCompare(value, "DLMS_UNIT_LENGTH"))
        return DLMS_UNIT_LENGTH;
    else if (InsensitiveCaseCompare(value, "DLMS_UNIT_SPEED"))
        return DLMS_UNIT_SPEED;
    else if (InsensitiveCaseCompare(value, "DLMS_UNIT_VOLUME_CUBIC_METER"))
        return DLMS_UNIT_VOLUME_CUBIC_METER;
    else if (InsensitiveCaseCompare(value, "DLMS_UNIT_CORRECTED_VOLUME"))
        return DLMS_UNIT_CORRECTED_VOLUME;
    else if (InsensitiveCaseCompare(value, "DLMS_UNIT_VOLUME_FLUX_HOUR"))
        return DLMS_UNIT_VOLUME_FLUX_HOUR;
    else if (InsensitiveCaseCompare(value, "DLMS_UNIT_CORRECTED_VOLUME_FLUX_HOUR"))
        return DLMS_UNIT_CORRECTED_VOLUME_FLUX_HOUR;
    else if (InsensitiveCaseCompare(value, "DLMS_UNIT_VOLUME_FLUXDAY"))
        return DLMS_UNIT_VOLUME_FLUXDAY;
    else if (InsensitiveCaseCompare(value, "DLMS_UNIT_CORRECTE_VOLUME_FLUX_DAY"))
        return DLMS_UNIT_CORRECTE_VOLUME_FLUX_DAY;
    else if (InsensitiveCaseCompare(value, "DLMS_UNIT_VOLUME_LITER"))
        return DLMS_UNIT_VOLUME_LITER;
    else if (InsensitiveCaseCompare(value, "DLMS_UNIT_MASS_KG"))
        return DLMS_UNIT_MASS_KG;
    else if (InsensitiveCaseCompare(value, "DLMS_UNIT_FORCE"))
        return DLMS_UNIT_FORCE;
    else if (InsensitiveCaseCompare(value, "DLMS_UNIT_ENERGY"))
        return DLMS_UNIT_ENERGY;
    else if (InsensitiveCaseCompare(value, "PRESSURE_PASCAL"))
        return PRESSURE_PASCAL;
    else if (InsensitiveCaseCompare(value, "DLMS_UNIT_PRESSURE_BAR"))
        return DLMS_UNIT_PRESSURE_BAR;
    else if (InsensitiveCaseCompare(value, "DLMS_UNIT_ENERGY_JOULE"))
        return DLMS_UNIT_ENERGY_JOULE;
    else if (InsensitiveCaseCompare(value, "DLMS_UNIT_THERMAL_POWER"))
        return DLMS_UNIT_THERMAL_POWER;
    else if (InsensitiveCaseCompare(value, "DLMS_UNIT_ACTIVE_POWER"))
        return DLMS_UNIT_ACTIVE_POWER;
    else if (InsensitiveCaseCompare(value, "DLMS_UNIT_APPARENT_POWER"))
        return DLMS_UNIT_APPARENT_POWER;
    else if (InsensitiveCaseCompare(value, "DLMS_UNIT_REACTIVE_POWER"))
        return DLMS_UNIT_REACTIVE_POWER;
    else if (InsensitiveCaseCompare(value, "DLMS_UNIT_ACTIVE_ENERGY"))
        return DLMS_UNIT_ACTIVE_ENERGY;
    else if (InsensitiveCaseCompare(value, "DLMS_UNIT_APPARENT_ENERGY"))
        return DLMS_UNIT_APPARENT_ENERGY;
    else if (InsensitiveCaseCompare(value, "REACTIVE_ENERGY"))
        return REACTIVE_ENERGY;
    else if (InsensitiveCaseCompare(value, "DLMS_UNIT_CURRENT"))
        return DLMS_UNIT_CURRENT;
    else if (InsensitiveCaseCompare(value, "DLMS_UNIT_ELECTRICAL_CHARGE"))
        return DLMS_UNIT_ELECTRICAL_CHARGE;
    else if (InsensitiveCaseCompare(value, "DLMS_UNIT_VOLTAGE"))
        return DLMS_UNIT_VOLTAGE;
    else if (InsensitiveCaseCompare(value, "DLMS_UNIT_ELECTRICAL_FIELD_STRENGTH"))
        return DLMS_UNIT_ELECTRICAL_FIELD_STRENGTH;
    else if (InsensitiveCaseCompare(value, "DLMS_UNIT_CAPACITY"))
        return DLMS_UNIT_CAPACITY;
    else if (InsensitiveCaseCompare(value, "DLMS_UNIT_RESISTANCE"))
        return DLMS_UNIT_RESISTANCE;
    else if (InsensitiveCaseCompare(value, "DLMS_UNIT_RESISTIVITY"))
        return DLMS_UNIT_RESISTIVITY;
    else if (InsensitiveCaseCompare(value, "DLMS_UNIT_MAGNETIC_FLUX"))
        return DLMS_UNIT_MAGNETIC_FLUX;
    else if (InsensitiveCaseCompare(value, "DLMS_UNIT_INDUCTION"))
        return DLMS_UNIT_INDUCTION;
    else if (InsensitiveCaseCompare(value, "DLMS_UNIT_MAGNETIC"))
        return DLMS_UNIT_MAGNETIC;
    else if (InsensitiveCaseCompare(value, "DLMS_UNIT_INDUCTIVITY"))
        return DLMS_UNIT_INDUCTIVITY;
    else if (InsensitiveCaseCompare(value, "DLMS_UNIT_FREQUENCY"))
        return DLMS_UNIT_FREQUENCY;
    else if (InsensitiveCaseCompare(value, "DLMS_UNIT_ACTIVE"))
        return DLMS_UNIT_ACTIVE;
    else if (InsensitiveCaseCompare(value, "DLMS_UNIT_REACTIVE"))
        return DLMS_UNIT_REACTIVE;
    else if (InsensitiveCaseCompare(value, "DLMS_UNIT_APPARENT"))
        return DLMS_UNIT_APPARENT;
    else if (InsensitiveCaseCompare(value, "DLMS_UNIT_V260"))
        return DLMS_UNIT_V260;
    else if (InsensitiveCaseCompare(value, "DLMS_UNIT_A260"))
        return DLMS_UNIT_A260;
    else if (InsensitiveCaseCompare(value, "DLMS_UNIT_MASS_KG_PER_SECOND"))
        return DLMS_UNIT_MASS_KG_PER_SECOND;
    else if (InsensitiveCaseCompare(value, "DLMS_UNIT_CONDUCTANCE"))
        return DLMS_UNIT_CONDUCTANCE;
    else if (InsensitiveCaseCompare(value, "DLMS_UNIT_KELVIN"))
        return DLMS_UNIT_KELVIN;
    else if (InsensitiveCaseCompare(value, "DLMS_UNIT_V2H"))
        return DLMS_UNIT_V2H;
    else if (InsensitiveCaseCompare(value, "DLMS_UNIT_A2H"))
        return DLMS_UNIT_A2H;
    else if (InsensitiveCaseCompare(value, "DLMS_UNIT_CUBIC_METER_RV"))
        return DLMS_UNIT_CUBIC_METER_RV;
    else if (InsensitiveCaseCompare(value, "DLMS_UNIT_PERCENTAGE"))
        return DLMS_UNIT_PERCENTAGE;
    else if (InsensitiveCaseCompare(value, "DLMS_UNIT_AMPERE_HOURS"))
        return DLMS_UNIT_AMPERE_HOURS;
    else if (InsensitiveCaseCompare(value, "DLMS_UNIT_ENERGY_PER_VOLUME"))
        return DLMS_UNIT_ENERGY_PER_VOLUME;
    else if (InsensitiveCaseCompare(value, "DLMS_UNIT_WOBBE"))
        return DLMS_UNIT_WOBBE;
    else if (InsensitiveCaseCompare(value, "DLMS_UNIT_MOLE_PERCENT"))
        return DLMS_UNIT_MOLE_PERCENT;
    else if (InsensitiveCaseCompare(value, "DLMS_UNIT_MASS_DENSITY"))
        return DLMS_UNIT_MASS_DENSITY;
    else if (InsensitiveCaseCompare(value, "DLMS_UNIT_PASCAL_SECOND"))
        return DLMS_UNIT_PASCAL_SECOND;
    else if (InsensitiveCaseCompare(value, "DLMS_UNIT_JOULE_KILOGRAM"))
        return DLMS_UNIT_JOULE_KILOGRAM;
    else if (InsensitiveCaseCompare(value, "DLMS_UNIT_SIGNAL_STRENGTH"))
        return DLMS_UNIT_SIGNAL_STRENGTH;
    else if (InsensitiveCaseCompare(value, "DLMS_UNIT_OTHER"))
        return DLMS_UNIT_OTHER;
    else if (InsensitiveCaseCompare(value, "DLMS_UNIT_NO_UNIT"))
        return DLMS_UNIT_NO_UNIT;
    else
        throw ParseException("Cannot convert: " + value + " to DLMS_UNIT enum.");
}

DLMS_ACCESS_MODE ConfigParser::GetValueAccessMode(const std::string &value) const {
    if (InsensitiveCaseCompare(value, "DLMS_ACCESS_MODE_NONE"))
        return DLMS_ACCESS_MODE_NONE;
    else if (InsensitiveCaseCompare(value, "DLMS_ACCESS_MODE_READ"))
        return DLMS_ACCESS_MODE_READ;
    else if (InsensitiveCaseCompare(value, "DLMS_ACCESS_MODE_WRITE"))
        return DLMS_ACCESS_MODE_WRITE;
    else if (InsensitiveCaseCompare(value, "DLMS_ACCESS_MODE_READ_WRITE"))
        return DLMS_ACCESS_MODE_READ_WRITE;
    else if (InsensitiveCaseCompare(value, "DLMS_ACCESS_MODE_AUTHENTICATED_READ"))
        return DLMS_ACCESS_MODE_AUTHENTICATED_READ;
    else if (InsensitiveCaseCompare(value, "DLMS_ACCESS_MODE_AUTHENTICATED_WRITE"))
        return DLMS_ACCESS_MODE_AUTHENTICATED_WRITE;
    else if (InsensitiveCaseCompare(value, "DLMS_ACCESS_MODE_AUTHENTICATED_READ_WRITE"))
        return DLMS_ACCESS_MODE_AUTHENTICATED_READ_WRITE;
    else
        throw ParseException("Cannot convert: " + value + " to DLMS_ACCESS_MODE enum.");
}

DLMS_METHOD_ACCESS_MODE ConfigParser::GetMethodAccessMode(const std::string &value) const {
    if (InsensitiveCaseCompare(value, "DLMS_METHOD_ACCESS_MODE_NONE"))
        return DLMS_METHOD_ACCESS_MODE_NONE;
    else if (InsensitiveCaseCompare(value, "DLMS_METHOD_ACCESS_MODE_ACCESS"))
        return DLMS_METHOD_ACCESS_MODE_ACCESS;
    else if (InsensitiveCaseCompare(value, "DLMS_METHOD_ACCESS_MODE_AUTHENTICATED_ACCESS"))
        return DLMS_METHOD_ACCESS_MODE_AUTHENTICATED_ACCESS;
    else
        throw ParseException("Cannot convert: " + value + " to DLMS_METHOD_ACCESS_MODE enum.");
}

DLMS_OBJECT_TYPE ConfigParser::GetObjectType (const std::string &value) const {
    if (InsensitiveCaseCompare(value, "DLMS_OBJECT_TYPE_ALL"))
        return DLMS_OBJECT_TYPE_ALL;
    else if (InsensitiveCaseCompare(value, "DLMS_OBJECT_TYPE_NONE"))
        return DLMS_OBJECT_TYPE_NONE;
    else if (InsensitiveCaseCompare(value, "DLMS_OBJECT_TYPE_DATA"))
        return DLMS_OBJECT_TYPE_DATA;
    else if (InsensitiveCaseCompare(value, "DLMS_OBJECT_TYPE_REGISTER"))
        return DLMS_OBJECT_TYPE_REGISTER;
    else if (InsensitiveCaseCompare(value, "DLMS_OBJECT_TYPE_EXTENDED_REGISTER"))
        return DLMS_OBJECT_TYPE_EXTENDED_REGISTER;
    else if (InsensitiveCaseCompare(value, "DLMS_OBJECT_TYPE_DEMAND_REGISTER"))
        return DLMS_OBJECT_TYPE_DEMAND_REGISTER;
    else if (InsensitiveCaseCompare(value, "DLMS_OBJECT_TYPE_REGISTER_ACTIVATION"))
        return DLMS_OBJECT_TYPE_REGISTER_ACTIVATION;
    else if (InsensitiveCaseCompare(value, "DLMS_OBJECT_TYPE_PROFILE_GENERIC"))
        return DLMS_OBJECT_TYPE_PROFILE_GENERIC;
    else if (InsensitiveCaseCompare(value, "DLMS_OBJECT_TYPE_CLOCK"))
        return DLMS_OBJECT_TYPE_CLOCK;
    else if (InsensitiveCaseCompare(value, "DLMS_OBJECT_TYPE_SCRIPT_TABLE"))
        return DLMS_OBJECT_TYPE_SCRIPT_TABLE;
    else if (InsensitiveCaseCompare(value, "DLMS_OBJECT_TYPE_SCHEDULE"))
        return DLMS_OBJECT_TYPE_SCHEDULE;
    else if (InsensitiveCaseCompare(value, "DLMS_OBJECT_TYPE_SPECIAL_DAYS_TABLE"))
        return DLMS_OBJECT_TYPE_SPECIAL_DAYS_TABLE;
    else if (InsensitiveCaseCompare(value, "DLMS_OBJECT_TYPE_ASSOCIATION_SHORT_NAME"))
        return DLMS_OBJECT_TYPE_ASSOCIATION_SHORT_NAME;
    else if (InsensitiveCaseCompare(value, "DLMS_OBJECT_TYPE_ASSOCIATION_LOGICAL_NAME"))
        return DLMS_OBJECT_TYPE_ASSOCIATION_LOGICAL_NAME;
    else if (InsensitiveCaseCompare(value, "DLMS_OBJECT_TYPE_SAP_ASSIGNMENT"))
        return DLMS_OBJECT_TYPE_SAP_ASSIGNMENT;
    else if (InsensitiveCaseCompare(value, "DLMS_OBJECT_TYPE_IMAGE_TRANSFER"))
        return DLMS_OBJECT_TYPE_IMAGE_TRANSFER;
    else if (InsensitiveCaseCompare(value, "DLMS_OBJECT_TYPE_IEC_LOCAL_PORT_SETUP"))
        return DLMS_OBJECT_TYPE_IEC_LOCAL_PORT_SETUP;
    else if (InsensitiveCaseCompare(value, "DLMS_OBJECT_TYPE_ACTIVITY_CALENDAR"))
        return DLMS_OBJECT_TYPE_ACTIVITY_CALENDAR;
    else if (InsensitiveCaseCompare(value, "DLMS_OBJECT_TYPE_REGISTER_MONITOR"))
        return DLMS_OBJECT_TYPE_REGISTER_MONITOR;
    else if (InsensitiveCaseCompare(value, "DLMS_OBJECT_TYPE_ACTION_SCHEDULE"))
        return DLMS_OBJECT_TYPE_ACTION_SCHEDULE;
    else if (InsensitiveCaseCompare(value, "DLMS_OBJECT_TYPE_IEC_HDLC_SETUP"))
        return DLMS_OBJECT_TYPE_IEC_HDLC_SETUP;
    else if (InsensitiveCaseCompare(value, "DLMS_OBJECT_TYPE_IEC_TWISTED_PAIR_SETUP"))
        return DLMS_OBJECT_TYPE_IEC_TWISTED_PAIR_SETUP;
    else if (InsensitiveCaseCompare(value, "DLMS_OBJECT_TYPE_MBUS_SLAVE_PORT_SETUP"))
        return DLMS_OBJECT_TYPE_MBUS_SLAVE_PORT_SETUP;
    else if (InsensitiveCaseCompare(value, "DLMS_OBJECT_TYPE_UTILITY_TABLES"))
        return DLMS_OBJECT_TYPE_UTILITY_TABLES;
    else if (InsensitiveCaseCompare(value, "DLMS_OBJECT_TYPE_MODEM_CONFIGURATION"))
        return DLMS_OBJECT_TYPE_MODEM_CONFIGURATION;
    else if (InsensitiveCaseCompare(value, "DLMS_OBJECT_TYPE_AUTO_ANSWER"))
        return DLMS_OBJECT_TYPE_AUTO_ANSWER;
    else if (InsensitiveCaseCompare(value, "DLMS_OBJECT_TYPE_AUTO_CONNECT"))
        return DLMS_OBJECT_TYPE_AUTO_CONNECT;
    else if (InsensitiveCaseCompare(value, "DLMS_OBJECT_TYPE_TCP_UDP_SETUP"))
        return DLMS_OBJECT_TYPE_TCP_UDP_SETUP;
    else if (InsensitiveCaseCompare(value, "DLMS_OBJECT_TYPE_IP4_SETUP"))
        return DLMS_OBJECT_TYPE_IP4_SETUP;
    else if (InsensitiveCaseCompare(value, "DLMS_OBJECT_TYPE_MAC_ADDRESS_SETUP"))
        return DLMS_OBJECT_TYPE_MAC_ADDRESS_SETUP;
    else if (InsensitiveCaseCompare(value, "DLMS_OBJECT_TYPE_PPP_SETUP"))
        return DLMS_OBJECT_TYPE_PPP_SETUP;
    else if (InsensitiveCaseCompare(value, "DLMS_OBJECT_TYPE_GPRS_SETUP"))
        return DLMS_OBJECT_TYPE_GPRS_SETUP;
    else if (InsensitiveCaseCompare(value, "DLMS_OBJECT_TYPE_SMTP_SETUP"))
        return DLMS_OBJECT_TYPE_SMTP_SETUP;
    else if (InsensitiveCaseCompare(value, "DLMS_OBJECT_TYPE_GSM_DIAGNOSTIC"))
        return DLMS_OBJECT_TYPE_GSM_DIAGNOSTIC;
    else if (InsensitiveCaseCompare(value, "DLMS_OBJECT_TYPE_IP6_SETUP"))
        return DLMS_OBJECT_TYPE_IP6_SETUP;
    else if (InsensitiveCaseCompare(value, "DLMS_OBJECT_TYPE_REGISTER_TABLE"))
        return DLMS_OBJECT_TYPE_REGISTER_TABLE;
    else if (InsensitiveCaseCompare(value, "DLMS_OBJECT_TYPE_STATUS_MAPPING"))
        return DLMS_OBJECT_TYPE_STATUS_MAPPING;
    else if (InsensitiveCaseCompare(value, "DLMS_OBJECT_TYPE_SECURITY_SETUP"))
        return DLMS_OBJECT_TYPE_SECURITY_SETUP;
    else if (InsensitiveCaseCompare(value, "DLMS_OBJECT_TYPE_DISCONNECT_CONTROL"))
        return DLMS_OBJECT_TYPE_DISCONNECT_CONTROL;
    else if (InsensitiveCaseCompare(value, "DLMS_OBJECT_TYPE_LIMITER"))
        return DLMS_OBJECT_TYPE_LIMITER;
    else if (InsensitiveCaseCompare(value, "DLMS_OBJECT_TYPE_MBUS_CLIENT"))
        return DLMS_OBJECT_TYPE_MBUS_CLIENT;
    else if (InsensitiveCaseCompare(value, "DLMS_OBJECT_TYPE_PUSH_SETUP"))
        return DLMS_OBJECT_TYPE_PUSH_SETUP;

        /**
        * S-FSK Phy MAC Setup
        */
    else if (InsensitiveCaseCompare(value, "DLMS_OBJECT_TYPE_SFSK_PHY_MAC_SETUP"))
        return DLMS_OBJECT_TYPE_SFSK_PHY_MAC_SETUP;

        /*
        * S-FSK Active initiator.
        */
    else if (InsensitiveCaseCompare(value, "DLMS_OBJECT_TYPE_SFSK_ACTIVE_INITIATOR"))
        return DLMS_OBJECT_TYPE_SFSK_ACTIVE_INITIATOR;
        /*
        * S-FSK MAC synchronization timeouts
        */
    else if (InsensitiveCaseCompare(value, "DLMS_OBJECT_TYPE_SFSK_MAC_SYNCHRONIZATION_TIMEOUTS"))
        return DLMS_OBJECT_TYPE_SFSK_MAC_SYNCHRONIZATION_TIMEOUTS;

        /*
        * S-FSK MAC Counters.
        */
    else if (InsensitiveCaseCompare(value, "DLMS_OBJECT_TYPE_SFSK_MAC_COUNTERS"))
        return DLMS_OBJECT_TYPE_SFSK_MAC_COUNTERS;

        /*
        * IEC 61334-4-32 LLC setup.
        */
    else if (InsensitiveCaseCompare(value, "DLMS_OBJECT_TYPE_IEC_61334_4_32_LLC_SETUP"))
        return DLMS_OBJECT_TYPE_IEC_61334_4_32_LLC_SETUP;

        /*
        * S-FSK Reporting system list.
        */
    else if (InsensitiveCaseCompare(value, "DLMS_OBJECT_TYPE_SFSK_REPORTING_SYSTEM_LIST"))
        return DLMS_OBJECT_TYPE_SFSK_REPORTING_SYSTEM_LIST;

        /*
        * ISO/IEC 8802-2 LLC Type 1 setup.
        */
    else if (InsensitiveCaseCompare(value, "DLMS_OBJECT_TYPE_IEC_8802_LLC_TYPE1_SETUP"))
        return DLMS_OBJECT_TYPE_IEC_8802_LLC_TYPE1_SETUP;

        /*
        * ISO/IEC 8802-2 LLC Type 2 setup.
        */
    else if (InsensitiveCaseCompare(value, "DLMS_OBJECT_TYPE_IEC_8802_LLC_TYPE2_SETUP"))
        return DLMS_OBJECT_TYPE_IEC_8802_LLC_TYPE2_SETUP;
        /*
        * ISO/IEC 8802-2 LLC Type 3 setup.
        */
    else if (InsensitiveCaseCompare(value, "DLMS_OBJECT_TYPE_IEC_8802_LLC_TYPE3_SETUP"))
        return DLMS_OBJECT_TYPE_IEC_8802_LLC_TYPE3_SETUP;

    else if (InsensitiveCaseCompare(value, "DLMS_OBJECT_TYPE_WIRELESS_MODE_Q_CHANNEL"))
        return DLMS_OBJECT_TYPE_WIRELESS_MODE_Q_CHANNEL;
    else if (InsensitiveCaseCompare(value, "DLMS_OBJECT_TYPE_MBUS_MASTER_PORT_SETUP"))
        return DLMS_OBJECT_TYPE_MBUS_MASTER_PORT_SETUP;
    else if (InsensitiveCaseCompare(value, "DLMS_OBJECT_TYPE_MESSAGE_HANDLER"))
        return DLMS_OBJECT_TYPE_MESSAGE_HANDLER;
    else if (InsensitiveCaseCompare(value, "DLMS_OBJECT_TYPE_PARAMETER_MONITOR"))
        return DLMS_OBJECT_TYPE_PARAMETER_MONITOR;
        /*
        * Arbitrator
        */
    else if (InsensitiveCaseCompare(value, "DLMS_OBJECT_TYPE_ARBITRATOR"))
        return DLMS_OBJECT_TYPE_ARBITRATOR;

        /*
        * Addresses that are provided by the base node during the opening of the
        * convergence layer.
        */
    else if (InsensitiveCaseCompare(value, "DLMS_OBJECT_TYPE_LLC_SSCS_SETUP"))
        return DLMS_OBJECT_TYPE_LLC_SSCS_SETUP;

        /*
        * Counters related to the physical layers exchanges.
        */
    else if (InsensitiveCaseCompare(value, "DLMS_OBJECT_TYPE_PRIME_NB_OFDM_PLC_PHYSICAL_LAYER_COUNTERS"))
        return DLMS_OBJECT_TYPE_PRIME_NB_OFDM_PLC_PHYSICAL_LAYER_COUNTERS;

        /*
        * A necessary parameters to set up and manage the PRIME NB OFDM PLC MAC
        * layer.
        */
    else if (InsensitiveCaseCompare(value, "DLMS_OBJECT_TYPE_PRIME_NB_OFDM_PLC_MAC_SETUP"))
        return DLMS_OBJECT_TYPE_PRIME_NB_OFDM_PLC_MAC_SETUP;

        /*
        * Functional behaviour of MAC.
        */
    else if (InsensitiveCaseCompare(value, "DLMS_OBJECT_TYPE_PRIME_NB_OFDM_PLC_MAC_FUNCTIONAL_PARAMETERS"))
        return DLMS_OBJECT_TYPE_PRIME_NB_OFDM_PLC_MAC_FUNCTIONAL_PARAMETERS;

        /*
        * Statistical information on the operation of the MAC layer for management
        * purposes.
        */
    else if (InsensitiveCaseCompare(value, "DLMS_OBJECT_TYPE_PRIME_NB_OFDM_PLC_MAC_COUNTERS"))
        return DLMS_OBJECT_TYPE_PRIME_NB_OFDM_PLC_MAC_COUNTERS;

        /*
        * Parameters related to the management of the devices connected to the
        * network.
        */
    else if (InsensitiveCaseCompare(value, "DLMS_OBJECT_TYPE_PRIME_NB_OFDM_PLC_MAC_NETWORK_ADMINISTRATION_DATA"))
        return DLMS_OBJECT_TYPE_PRIME_NB_OFDM_PLC_MAC_NETWORK_ADMINISTRATION_DATA;

        /*
        * Identification information related to administration and maintenance of
        * PRIME NB OFDM PLC devices.
        */
    else if (InsensitiveCaseCompare(value, "DLMS_OBJECT_TYPE_PRIME_NB_OFDM_PLC_APPLICATIONS_IDENTIFICATION"))
        return DLMS_OBJECT_TYPE_PRIME_NB_OFDM_PLC_APPLICATIONS_IDENTIFICATION;

        /*
        * G3-PLC MAC layer counters
        */
    else if (InsensitiveCaseCompare(value, "DLMS_OBJECT_TYPE_G3_PLC_MAC_LAYER_COUNTERS"))
        return DLMS_OBJECT_TYPE_G3_PLC_MAC_LAYER_COUNTERS;

        /*
        * G3-PLC MAC setup.
        */
    else if (InsensitiveCaseCompare(value, "DLMS_OBJECT_TYPE_G3_PLC_MAC_SETUP"))
        return DLMS_OBJECT_TYPE_G3_PLC_MAC_SETUP;

        /*
        * G3-PLC 6LoWPAN.
        */
    else if (InsensitiveCaseCompare(value, "DLMS_OBJECT_TYPE_G3_PLC6_LO_WPAN"))
        return DLMS_OBJECT_TYPE_G3_PLC6_LO_WPAN;

        /*
        * Configure a ZigBee PRO device with information necessary to create or
        * join the network.
        */
    else if (InsensitiveCaseCompare(value, "DLMS_OBJECT_TYPE_ZIG_BEE_SAS_STARTUP"))
        return DLMS_OBJECT_TYPE_ZIG_BEE_SAS_STARTUP;

        /*
        * Configure the behavior of a ZigBee PRO device on joining or loss of
        * connection to the network.
        */
    else if (InsensitiveCaseCompare(value, "DLMS_OBJECT_TYPE_ZIG_BEE_SAS_JOIN"))
        return DLMS_OBJECT_TYPE_ZIG_BEE_SAS_JOIN;

        /*
        * Configure the fragmentation feature of ZigBee PRO transport layer.
        */
    else if (InsensitiveCaseCompare(value, "DLMS_OBJECT_TYPE_ZIG_BEE_SAS_APS_FRAGMENTATION"))
        return DLMS_OBJECT_TYPE_ZIG_BEE_SAS_APS_FRAGMENTATION;


    else if (InsensitiveCaseCompare(value, "DLMS_OBJECT_TYPE_ZIG_BEE_NETWORK_CONTROL"))
        return DLMS_OBJECT_TYPE_ZIG_BEE_NETWORK_CONTROL;

    else if (InsensitiveCaseCompare(value, "DLMS_OBJECT_TYPE_ACCOUNT"))
        return DLMS_OBJECT_TYPE_ACCOUNT;
    else if (InsensitiveCaseCompare(value, "DLMS_OBJECT_TYPE_CREDIT"))
        return DLMS_OBJECT_TYPE_CREDIT;
    else if (InsensitiveCaseCompare(value, "DLMS_OBJECT_TYPE_CHARGE"))
        return DLMS_OBJECT_TYPE_CHARGE;
    else if (InsensitiveCaseCompare(value, "DLMS_OBJECT_TYPE_TOKEN_GATEWAY"))
        return DLMS_OBJECT_TYPE_TOKEN_GATEWAY;
    else if (InsensitiveCaseCompare(value, "DLMS_OBJECT_TYPE_COMPACT_DATA"))
        return DLMS_OBJECT_TYPE_COMPACT_DATA;
    else
        throw ParseException("Cannot convert: " + value + " to DLMS_OBJECT_TYPE enum.");
}

GX_SORT_METHOD ConfigParser::GetSortMethodType(const std::string &value) const {
    if (InsensitiveCaseCompare(value, "DLMS_SORT_METHOD_FIFO"))
        return DLMS_SORT_METHOD_FIFO;
    else if (InsensitiveCaseCompare(value, "DLMS_SORT_METHOD_LIFO"))
        return DLMS_SORT_METHOD_LIFO;
    else if (InsensitiveCaseCompare(value, "DLMS_SORT_METHOD_LARGEST"))
        return DLMS_SORT_METHOD_LARGEST;
    else if (InsensitiveCaseCompare(value, "DLMS_SORT_METHOD_SMALLEST"))
        return DLMS_SORT_METHOD_SMALLEST;
    else if (InsensitiveCaseCompare(value, "DLMS_SORT_METHOD_NEAREST_TO_ZERO"))
        return DLMS_SORT_METHOD_NEAREST_TO_ZERO;
    else if (InsensitiveCaseCompare(value, "DLMS_SORT_METHOD_FAREST_FROM_ZERO"))
        return DLMS_SORT_METHOD_FAREST_FROM_ZERO;
    else
        throw ParseException("Cannot convert: " + value + " to DLMS_DATA_TYPE enum.");
}
