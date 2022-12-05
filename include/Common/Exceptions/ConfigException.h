/**
 * @file ConfigException.h
 * @author Filip Grepl
 * @brief Definition of 'ConfigException' class
 * @version 1.0
 * @date 2021-10-08
 *
 * @copyright Copyright (c) 2021
 *
 */

#ifndef DLMS_CLIENT_SERVER_CONFIGEXCEPTION_H
#define DLMS_CLIENT_SERVER_CONFIGEXCEPTION_H


#include "ParseException.h"

/**
 * @brief Exception that is thrown when the error occurs during configuration loading.
 */
class ConfigException : public RuntimeException {
private:
public:

    /**
     * @brief Constructor
     * @param parseException    Exception, that occurs.
     * @param serverConfigPath  Path to configuration file.
     * @param row               A row number on which the exception occured.
     */
    ConfigException(const ParseException &parseException, const std::string &serverConfigPath, int row) :
    RuntimeException("CONFIG_EXCEPTION",serverConfigPath + " line " + std::to_string(row) + ": " + parseException.what()) {
    }
};

#endif //DLMS_CLIENT_SERVER_CONFIGEXCEPTION_H
