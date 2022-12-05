/**
 * @file ParseException.h
 * @author Filip Grepl
 * @brief Definition of 'ParseException' class
 * @version 1.0
 * @date 2021-10-08
 *
 * @copyright Copyright (c) 2021
 *
 */


#ifndef DLMS_CLIENT_SERVER_PARSEEXCEPTION_H
#define DLMS_CLIENT_SERVER_PARSEEXCEPTION_H


#include "RuntimeException.h"

/**
 * @brief Exception that is thrown when the error occurs during parsing specific line of input configuration file.
 */
class ParseException : public RuntimeException {

public:

    /**
     * @brief Constructor
     * @param description Description of error.
     */
    ParseException(const std::string &description) : RuntimeException("PARSE_CONFIG_EXCEPTION", description) {
    }
};


#endif //DLMS_CLIENT_SERVER_PARSEEXCEPTION_H
