/**
 * @file HatelCommunicationException.h
 * @author Filip Grepl
 * @brief Definition of 'HatelCommunicationException' class
 * @version 1.0
 * @date 2021-10-08
 *
 * @copyright Copyright (c) 2021
 *
 */

#ifndef DLMS_CLIENT_SERVER_HATELCOMMUNICATIONEXCEPTION_H
#define DLMS_CLIENT_SERVER_HATELCOMMUNICATIONEXCEPTION_H

#include "Exceptions/RuntimeException.h"

/**
 * @brief Exception that is thrown when the exception occurs in communication with Hatel.
 */
class HatelCommunicationException : public RuntimeException {

public:
    /**
    * @brief Constructor.
    * @param description Error description.
    */
    HatelCommunicationException(const std::string &description) : RuntimeException("RUNTIME_EXCEPTION", description) {
    }
};

#endif //DLMS_CLIENT_SERVER_HATELCOMMUNICATIONEXCEPTION_H
