/**
 * @file ConfigParser.h
 * @author Filip Grepl
 * @brief Definition of 'ConfigParser' class
 * @version 1.0
 * @date 2021-10-08
 *
 * @copyright Copyright (c) 2021
 *
 */

#ifndef DLMS_CLIENT_SERVER_CONFIGPARSER_H
#define DLMS_CLIENT_SERVER_CONFIGPARSER_H

#include <string>
#include "CommonDataStructureTemplates.h"
#include "Exceptions/ParseException.h"
#include <typeinfo>
#include <iostream>
#include <unordered_map>
#include <GURUX_DLMS/Objects/ParametersAndMeasurementData/GXDLMSProfileGeneric.h>

/**
 * @brief Class that provides loading of configuration for both - client and server.
 */
class ConfigParser {

public:
    /**
     * @brief Method, that converts string to DLMS unit type.
     * @param value String to be converted to DLMS unit type.
     * @return Enum value of DLMS unit type.
     * @throws ParseException if conversion failed.
     */
    DLMS_UNIT GetUnit(const std::string &value) const;

    /**
     * @brief Method, that converts string to DLMS value access mode.
     * @param value String to be converted to DLMS value access mode.
     * @return Enum value of DLMS value access mode.
     * @throws ParseException if conversion failed.
     */
    DLMS_ACCESS_MODE GetValueAccessMode (const std::string &value) const;

    /**
     * @brief Method, that converts string to DLMS method access mode.
     * @param value String to be converted to DLMS method access mode.
     * @return Enum value of DLMS method access mode.
     * @throws ParseException if conversion failed.
     */
    DLMS_METHOD_ACCESS_MODE GetMethodAccessMode(const std::string &value) const;

    /**
     * @brief Method, that converts string to DLMS object type.
     * @param value String to be converted to DLMS object type.
     * @return Enum value of DLMS object type.
     * @throws ParseException if conversion failed.
     */
    DLMS_OBJECT_TYPE GetObjectType (const std::string &value) const;

    /**
     * @brief Method, that converts string to DLMS sort method type.
     * @param value String to be converted to DLMS sort method type.
     * @return Enum value of DLMS sort method type.
     * @throws ParseException if conversion failed.
     */
    GX_SORT_METHOD GetSortMethodType(const std::string &value) const;

    /**
     * @brief Method, that converts string to DLMS authentication type.
     * @param value String to be converted to DLMS authentication type.
     * @return Enum value of DLMS authentication type.
     * @throws ParseException if conversion failed.
     */
    DLMS_AUTHENTICATION GetAuthenticationMechanismType(const std::string &value) const;

    /**
     * @brief Method, that converts string to DLMS interface type.
     * @param value String to be converted to DLMS interface type.
     * @return Enum value of DLMS interface type.
     * @throws ParseException if conversion failed.
     */
    DLMS_INTERFACE_TYPE GetInterfaceType(const std::string &value) const;

    /**
    * @brief Method, that converts string to DLMS baud rate type.
    * @param value String to be converted to DLMS baud rate type.
    * @return Enum value of DLMS baud rate type.
    * @throws ParseException if conversion failed.
    */
    DLMS_BAUD_RATE GetCommSpeed(const std::string &value) const;

    /**
     * @brief Method, that converts string to DLMS conformance type.
     * @param value String to be converted to DLMS conformance type.
     * @return Enum value of DLMS conformance type.
     * @throws ParseException if conversion failed.
     */
    DLMS_CONFORMANCE GetConformance(const std::string &value) const;

    /**
     * @brief Method, taht converts string to DLMS clock status type.
     * @param value String to be converted to DLMS clock status type.
     * @return Enum value of DLMS clock status.
     * @throws ParseException if convertsion failed.
     */
    DLMS_CLOCK_STATUS GetClockStatus(const std::string &value) const;

    /**
     * @brief Method that converts string to number.
     * @tparam T Type of number to which the input String to be converted.
     * @param value String to be converted to T number.
     * @return Number of T type.
     */
    template<typename T>
    T GetNumberValue(const std::string &value) const {
        unsigned long long tmp = std::stoull(value, nullptr, 0);

        if (typeid(T) == typeid(BYTE))
            return static_cast<BYTE>(tmp);
        else if (typeid(T) == typeid(short))
            return static_cast<short>(tmp);
        else if (typeid(T) == typeid(unsigned short))
            return static_cast<unsigned short>(tmp);
        else if (typeid(T) == typeid(int))
            return static_cast<int>(tmp);
        else if (typeid(T) == typeid(unsigned int))
            return static_cast<unsigned int>(tmp);
        else if (typeid(T) == typeid(long))
            return static_cast<long>(tmp);
        else if (typeid(T) == typeid(unsigned long))
            return tmp;
        else if (typeid(T) == typeid(long long))
            return static_cast<long long>(tmp);
        else if (typeid(T) == typeid(unsigned long long))
            return static_cast<unsigned long long>(tmp);
        else
            throw ParseException("Uknown typeid of target: " + value);
    };

    /**
    * @brief Method, that converts string to bool value.
    * @param value String to be converted to bool value.
    * @return Bool value
    * @throws ParseException if conversion failed.
    */
    bool GetBoolValue(const std::string &value) const;

    /**
     * @brief Method, that converts string to DLMS security policy type of version 1.
     * @param value String to be converted to DLMS security policy type of version 1.
     * @return Enum value of DLMS security policy type of version 1.
     * @throws ParseException if conversion failed.
     */
    DLMS_SECURITY_POLICY1 GetSecPolicy1Value(const std::string value) const;

    /**
     * @brief Method, that converts string to DLMS context id.
     * @param value String to be converted to DLMS context id.
     * @return Enum value of DLMS security context id.
     * @throws ParseException if conversion failed.
     */
    DLMS_APPLICATION_CONTEXT_NAME GetContextId(const std::string value) const;

    /**
     * @brief Method, that converts string to DLMS security suite.
     * @param value String to be converted to DLMS security suite.
     * @return Enum value of DLMS security suite.
     * @throws ParseException if conversion failed.
     */
    DLMS_SECURITY_SUITE GetSecuritySuite(const std::string value) const;

    DLMS_CERTIFICATE_ENTITY GetCertificateEntity(const std::string value) const;

    DLMS_CERTIFICATE_TYPE GetCertificateType(const std::string value) const;

    IP_OPTION_TYPE GetIpOptionType(const std::string value) const;

    GX_TRACE_LEVEL GetTraceLevel(const std::string value) const;

protected:

    /**
     * @brief Method that compares insensitive two input strings.
     * @param a First input String.
     * @param b Second input String.
     * @return True if strings are equal (insensitive case). False otherwise.
     */
    bool InsensitiveCaseCompare(const std::string &a, const std::string &b) const;

    /**
     * @brief Method that removes all spaces from input string.
     * @param line String from which the whitespaces to be removed.
     */
    void EraseWhiteSpaces(std::string &line) const;

     /**
      * @brief Method, that removes comment from input string, i.e. all after // sequence.
      * @param line String from which the comment to be removed.
      */
    void RemoveComment(std::string &line) const;
};

#endif //DLMS_CLIENT_SERVER_CONFIGPARSER_H
