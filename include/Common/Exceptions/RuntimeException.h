/**
 * @file RuntimeException.h
 * @author Filip Grepl
 * @brief Definition of 'RuntimeException' class
 * @version 1.0
 * @date 2021-10-08
 *
 * @copyright Copyright (c) 2021
 *
 */
#ifndef DLMS_CLIENT_SERVER_RUNTIMEEXCEPTION_H
#define DLMS_CLIENT_SERVER_RUNTIMEEXCEPTION_H

#include <string>

/**
 * @brief Exception that is thrown when the error occurs during runtime.
 */
class RuntimeException : public std::exception {

private:
    const std::string description;

protected:
    /**
     * @brief Construtor
     * @param exceptionType Type of exception.
     * @param description Description of error.
     */
    RuntimeException(const std::string& exceptionType, const std::string& description) : description("[" + exceptionType + "] : " + description) {
    }

public:

    /**
     * @brief Constructor
     * @param description Description of error.
     */
    RuntimeException(const std::string& description) : RuntimeException("RUNTIME", description) {
    }

    const char * what () const noexcept
    {
        return description.c_str();
    }
};

#endif //DLMS_CLIENT_SERVER_RUNTIMEEXCEPTION_H
