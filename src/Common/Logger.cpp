/**
 * @file Logger.cpp
 * @author Filip Grepl
 * @brief Implementation of 'Logger' class
 * @date 2021-10-08
 *
 * @copyright Copyright (c) 2020
 *
 */

#include "Logger.h"
#include <sstream>
#include <iostream>
#include <Exceptions/ConfigException.h>
#include <fstream>
#include <GURUX_DLMS/GXDLMSTranslator.h>

std::string Logger::pathToLogFile = "";
pthread_mutex_t Logger::accessMutex = PTHREAD_MUTEX_INITIALIZER;

Logger::~Logger(void)
{

}

Logger::Logger()
{
}

void Logger::LogEvent(LOG_LEVEL logLevel, const std::string& message)
{
    pthread_mutex_lock(&accessMutex);
    // create log line
    std::stringstream logLine;
    logLine << "[" << GetNow() << "] ";
    switch(logLevel) {
        case ERROR:
            logLine << "\t[ERROR] ";
            break;
        case INFO:
            logLine << "[INFO] ";
            break;
        default:
            logLine << "[UNKNOWN] ";
            break;
    }
    logLine << message << std::endl;

    std::cout << logLine.str();

    if (!Logger::pathToLogFile.empty()) {
        // create log folder
        std::string logFolder = Logger::pathToLogFile.substr(0, Logger::pathToLogFile.find_last_of('/'));

        std::ofstream f(Logger::pathToLogFile, std::fstream::out | std::fstream::app);
        if (!f.is_open()) {
            pthread_mutex_unlock(&accessMutex);
            throw RuntimeException("\t[ERROR] " + Logger::pathToLogFile + " cannot be open!");
        } else {
            f << logLine.str();
            f.close();
        }
    }
    pthread_mutex_unlock(&accessMutex);
}


void Logger::InitLogger(const std::string &pathToLogFile) {
    Logger::pathToLogFile = pathToLogFile;
}

