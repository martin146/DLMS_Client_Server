/**
 * @file HandleException.h
 * @author Filip Grepl
 * @brief Definition of 'HandleException' class
 * @version 1.0
 * @date 2021-10-08
 *
 * @copyright Copyright (c) 2021
 *
 */

#ifndef DLMS_CLIENT_SERVER_HANDLEEXCEPTION_H
#define DLMS_CLIENT_SERVER_HANDLEEXCEPTION_H

#include "Exceptions/RuntimeException.h"

/**
 * @brief Exception that is thrown when the obis code of saved object is not found.
 */
class HandleException : public RuntimeException {

public:
    /**
     * @brief Constructor.
     * @param description Error description.
     */
    HandleException(const std::string &description) : RuntimeException("HANDLE_EXCEPTION", description) {
    }
};

#endif //DLMS_CLIENT_SERVER_HANDLEEXCEPTION_H
