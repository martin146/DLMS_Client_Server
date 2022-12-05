/**
 * @file HatelConfiguration.h
 * @author Filip Grepl
 * @brief Definition of 'HatelConfiguration' class
 * @version 1.0
 * @date 2021-10-08
 *
 * @copyright Copyright (c) 2021
 *
 */

#ifndef DLMS_CLIENT_SERVER_HATELCONFIGURATION_H
#define DLMS_CLIENT_SERVER_HATELCONFIGURATION_H

#include <string>

/**
 * @brief Class with data of Hatel configuration.
 */
class HatelConfiguration {
    std::string outQueue;
    std::string inQueue;
    unsigned int maxMsgLen;
    unsigned int timeout; // ms

public:

    /** GETTERS AND SETTERS **/

    const std::string &getOutQueue() const;

    void setOutQueue(const std::string &outQueue);

    const std::string &getInQueue() const;

    void setInQueue(const std::string &inQueue);

    unsigned int getMaxMsgLen() const;

    void setMaxMsgLen(unsigned int maxMsgLen);

    unsigned int getTimeout() const;

    void setTimeout(unsigned int timeout);
};

#endif //DLMS_CLIENT_SERVER_HATELCONFIGURATION_H
