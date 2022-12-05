/**
 * @file InitException.h
 * @author Filip Grepl
 * @brief Definition of 'InitException' class
 * @version 1.0
 * @date 2021-10-08
 *
 * @copyright Copyright (c) 2021
 *
 */

#ifndef DLMS_CLIENT_SERVER_INITEXCEPTION_H
#define DLMS_CLIENT_SERVER_INITEXCEPTION_H

/**
 * @brief Exception that is thrown when the exception occurs in initialization of whole server application.
 */
class InitException : public RuntimeException {

public:
    /**
    * @brief Constructor.
    * @param description Error description.
    */
    InitException(const std::string &description) : RuntimeException("INIT_EXCEPTION", description) {
    }
};

#endif //DLMS_CLIENT_SERVER_INITEXCEPTION_H
