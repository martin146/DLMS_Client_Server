/**
 * @file HatelCommunication.h
 * @author Filip Grepl
 * @brief Definition of 'HatelCommunication' class
 * @version 1.0
 * @date 2021-10-08
 *
 * @copyright Copyright (c) 2021
 *
 */

#ifndef DLMS_CLIENT_SERVER_HATELCOMMUNICATION_H
#define DLMS_CLIENT_SERVER_HATELCOMMUNICATION_H

#include "MqConnector.hpp"
#include <GURUX_DLMS/GXDLMSVariant.h>
#include <GURUX_DLMS/GXDLMSObjectCollection.h>
#include <ApplicationConfigParser/Configurations/LoadedUserConfiguration.h>

/**
 * @brief Class that provides communication with Hatel server.
 */
class HatelCommunication
{
private:

    /**
     * @brief Mutex for waiting to response from Hatel.
     */
    static pthread_mutex_t condition_mutex;
    /**
     * @brief Condition for waiting to response from Hatel.
     */
    static pthread_cond_t condition;
    /**
     * @brief Confirm message from Hatel.
     */
    static HATEL_NOTIFICATION_CONFIRM *itemConfirm;
    /**
     * @brief Data message from Hatel.
     */
    static HATEL_NOTIFICATION_DATA *itemData;
    /**
     * @brief Cid (Hatel identifier) for getting value of object.
     */
    static std::string *cidToGet;
    /**
     * @brief Loaded user configuration.
     */
    static LoadedUserConfiguration *loadedUserConfiguration;
    /**
     * @brief DLMS objects of the DLMS server.
     */
    static CGXDLMSObjectCollection *dlmsServerObjects;
    /**
     * @brief Pointer to mutex for modification of DLMS server objects.
     */
    static pthread_mutex_t *objectAccessMutex;

    // Identifikace příkazu
    /**
     * @brief Acknowledge number of request to be sent to the Hatel.
     */
    unsigned short AckNum;

    /**
     * @brief Method, that creates and returns next acknowledge number of request.
     * @return Next acknowledge number.
     */
    unsigned short GetNextAckNum();

    /**
     * @brief Not implemented yet.
     */
    void Log(const char *cmd);

    /**
     * @brief Method for waiting to response from Hatel when some request has been sent.
     */
    void WaitForResponse() const;


public:

    /**
     * @brief Constructor.
     * @param loadedUserConfiguration Loaded user configuration.
     * @param dlmsServerObjects  Collection of dlms server object.
     * @param objectAccessMutex Mutex for modification of DLMS server objects.
     */
    HatelCommunication(LoadedUserConfiguration &loadedUserConfiguration,
                       CGXDLMSObjectCollection &dlmsServerObjects,
                       pthread_mutex_t &objectAccessMutex);

    /**
     * @brief Method for initializing of communication with Hatel.
     */
    void Initialize();

    /**
     * @brief Method for obtaining value of object specified by cid identifier.
     * @param cid Id of object for which the value to be read.
     */
    void Get(std::string &cid);
    /**
     * @brief Method for writing new value of object specified by cid identifier.
     * @param cid Id of object for which the new value to be written.
     * @param value The new value of object.
     * @param dataType Type of data object.
     */
    void Put(const std::string &cid, const CGXDLMSVariant &value, DLMS_DATA_TYPE dataType);

    /**
     * @brief Method to stop communication with Hatel.
     * @return Stop code.
     */
    int StopHatelCommunication() const;

    /**
     * @brief Method for receiving messages from Hatel, that is called as callback function by Hatel library.
     * @param item Parsed data.
     */
    static void Receive(const HATEL_NOTIFICATION_HDR *item);
};

#endif //DLMS_CLIENT_SERVER_HATELCOMMUNICATION_H
