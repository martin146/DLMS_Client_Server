/**
 * @file Logger.h
 * @author Filip Grepl
 * @brief Definition of 'Logger' class
 * @version 1.0
 * @date 2021-10-08
 *
 * @copyright Copyright (c) 2021
 *
 */

#ifndef DLMS_CLIENT_SERVER_LOGGER_H
#define DLMS_CLIENT_SERVER_LOGGER_H

#include <string>
#include <ctime>
#include <cstring>

/**
 * @brief Class, that provides logging of all events.
 */
class Logger {
private:
    static std::string pathToLogFile;

    /**
     * @brief Access mutex for multi thread access to logging.
     */
    static pthread_mutex_t accessMutex;

protected:

public:

    /**
     * @brief Level of logging message
     */
    enum LOG_LEVEL {
        INFO,
        ERROR
    };


    /**
     * @brief Constructor.
    */
    Logger();

    /**
     * @brief Destructor.
    */
    ~Logger(void);

    /**
     * @brief Logger initializer.
     * @param pathToLogFile Path to log file.
     */
    static void InitLogger(const std::string &pathToLogFile);


    /**
     * @brief Method, that saves event to the log file.
     * @param logLevel Level of logged message.
     * @param message Message to be logged.
     */
    static void LogEvent(LOG_LEVEL logLevel, const std::string& message);

    /**
     * @brief Method, that returns current time.
     * @return Current time in %Y-%m-%d|%H:%M:%S format.
     */
    inline static std::string GetNow() {
        char buffer[30];
        memset(buffer, '\0', sizeof(buffer));

        time_t now = time(0);
        struct tm ltm;
#ifdef _WIN32
        localtime_s(&ltm, &now);
#elif __linux__
        ltm = *localtime(&now);
#endif
        if (std::strftime(buffer, sizeof(buffer), "%Y-%m-%d|%H:%M:%S", &ltm))
            return buffer;

        return nullptr;
    }
};

#endif // DLMS_CLIENT_SERVER_LOGGER_H

