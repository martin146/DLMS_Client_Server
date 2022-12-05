/**
 * @file HatelConfiguration.cpp
 * @author Filip Grepl
 * @brief Implementation of 'HatelConfiguration' class
 * @date 2021-10-08
 *
 * @copyright Copyright (c) 2020
 *
 */

#include "ApplicationConfigParser/Configurations/HatelConfiguration.h"

const std::string &HatelConfiguration::getOutQueue() const {
    return outQueue;
}

void HatelConfiguration::setOutQueue(const std::string &outQueue) {
    HatelConfiguration::outQueue = outQueue;
}

const std::string &HatelConfiguration::getInQueue() const {
    return inQueue;
}

void HatelConfiguration::setInQueue(const std::string &inQueue) {
    HatelConfiguration::inQueue = inQueue;
}

unsigned int HatelConfiguration::getMaxMsgLen() const {
    return maxMsgLen;
}

void HatelConfiguration::setMaxMsgLen(unsigned int maxMsgLen) {
    HatelConfiguration::maxMsgLen = maxMsgLen;
}

unsigned int HatelConfiguration::getTimeout() const {
    return timeout;
}

void HatelConfiguration::setTimeout(unsigned int timeout) {
    HatelConfiguration::timeout = timeout;
}