
//
// --------------------------------------------------------------------------
//  Gurux Ltd
//
//
//
// Filename:        $HeadURL$
//
// Version:         $Revision$,
//                  $Date$
//                  $Author$
//
// Copyright (c) Gurux Ltd
//
//---------------------------------------------------------------------------
//
//  DESCRIPTION
//
// This file is a part of Gurux ServerConfiguration Framework.
//
// Gurux ServerConfiguration Framework is Open Source software; you can redistribute it
// and/or modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; version 2 of the License.
// Gurux ServerConfiguration Framework is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// See the GNU General Public License for more details.
//
// More information of Gurux products: http://www.gurux.org
//
// This code is licensed under the GNU General Public License v2.
// Full text may be retrieved at http://www.gnu.org/licenses/gpl-2.0.txt
//---------------------------------------------------------------------------

/**
 * @file GXDLMSBase.h
 * @author Filip Grepl
 * @brief Definition of 'GXDLMSBase' class
 * @version 1.0
 * @date 2021-10-08
 *
 * @copyright Copyright (c) 2021
 *
 */

#pragma once

#if defined(_WIN32) || defined(_WIN64)//Windows includes
#include <Winsock.h> //Add support for sockets
#endif

//#include <HatelCommunication/HatelCommunication.h>
#include <ApplicationConfigParser/Configurations/HatelConfiguration.h>
#include <GURUX_DLMS/Objects/TimeAndEventBoundControl/GXDLMSClock.h>
#include <ApplicationConfigParser/Configurations/LoadedUserConfiguration.h>
#include "GURUX_DLMS/Servers/GXDLMSSecureServer.h"
#include "GURUX_DLMS/Objects/AccessControlAndManagement/GXDLMSSecuritySetup.h"
#include <PugiXML/pugixml.hpp>
#include <DataStructureTemplatesServer.h>


/**
 * @brief Class that represents the DLMS Server.
 */
class GXDLMSBase : public CGXDLMSSecureServer
{
    long SendPush(CGXDLMSPushSetup* target);

private:
#if defined(_WIN32) || defined(_WIN64)//If Windows
    SOCKET m_ServerSocket;
    HANDLE m_ReceiverThread;
#else //If Linux.
    /**
     * @brief Server socket for receiving messages from the clients.
     */
    int m_ServerSocket;

    /**
     * @brief Vector of all profile generic threads and all necessary information for saving profile generic data.
     */
    std::vector<std::shared_ptr<ProfileGenericWriterStruct>> m_ProfileGenericThreads;
#endif

    /**
     * @brief  Loaded user configuration.
     */
    LoadedUserConfiguration loadedUserConfiguration;

//    /**
//     * @brief Class for communication with Hatel server.
//     */
//    HatelCommunication hatelCommunication;

    /**
     * @brief Mutex to multi thread access object of the DLMS server.
     */
    pthread_mutex_t objectAccessMutex{};

    /**
     * @brief Mutex for correct termination of all running threads.
     */
    pthread_mutex_t endMutex{};
    /**
     * @brief Waiting condition for correct termination of all running threads.
     */
    pthread_cond_t endCondition{};
    /**
     * @brief Flag for signalizing the end request.
     */
    bool endFlag{};

    /**
     * @brief Method, that adds the security setup to the server.
     */
    void AddSecuritySetup();

    /**
     * @brief Method, that returns count of saved data, i.e. number of lines in profile generic file.
     * @param profileGeneric Profile generic object for which the number of saved data to be found out.
     * @return Count of saved data for specified profile generic object.
     */
    long GetProfileGenericDataCount(CGXDLMSProfileGeneric &profileGeneric);

    /**
     * @brief Method, that loads profile generic data of specified profile generic object.
     * @param profileGeneric Profile generic object for which the saved data to be loaded.
     * @param index Start index from which the saved data to be loaded.
     * @param count Number of items to be loaded from the start index.
     */
    void GetProfileGenericDataByEntry(CGXDLMSProfileGeneric &profileGeneric, long index, long count);

    /**
     * @brief Method, that adds profile generic data from file to the profile generic buffer using start and end date time.
     * @param profileGeneric Profile generic object for which the saved data to be loaded.
     * @param e Information about request
     */
    void GetProfileGenericDataByRange(CGXDLMSProfileGeneric &profileGeneric, CGXDLMSValueEventArg &e);

public:
    GX_TRACE_LEVEL m_Trace = GX_TRACE_LEVEL_ERROR;

    /**
     * Constructor
     * @param isUseLogicalNameReferencing True i logical name referencing is used.
     *                                    False if short name referencing is used.
     * @param interfaceType Type of interface.
     * @param device Device configuration.
     * @param loadedUserConfiguration Loaded user configuration.
     */
    GXDLMSBase(
            bool isUseLogicalNameReferencing,
            DLMS_INTERFACE_TYPE interfaceType,
            LoadedUserConfiguration &loadedUserConfiguration);

    /**
     * @brief Constructor
     * @param ln Association logical name object.
     * @param hdlc Hdlc setup object.
     * @param device Device configuration.
     * @param loadedUserConfiguration Loaded user configuration.
     */
    GXDLMSBase(
            CGXDLMSIecHdlcSetup* hdlc,
            LoadedUserConfiguration &loadedUserConfiguration);

    /**
     * @brief Constructor.
     * @param ln Association logical name object.
     * @param wrapper Wrapper configuration.
     * @param device Device configuration.
     * @param loadedUserConfiguration Loaded user configuration.
     */
    GXDLMSBase(
            CGXDLMSTcpUdpSetup* wrapper,
            LoadedUserConfiguration &loadedUserConfiguration);

    /**
     * @brief Constructor
     * @param sn Association short name object.
     * @param hdlc Hdlc setup object.
     * @param device Device configuration.
     * @param loadedUserConfiguration Loaded user configuration.
     */
    GXDLMSBase(
            CGXDLMSAssociationShortName* sn,
            CGXDLMSIecHdlcSetup* hdlc,
            LoadedUserConfiguration &loadedUserConfiguration);

    /**
     * @brief Constructor
     * @param sn Association short name object.
     * @param wrapper Wrapper configuration.
     * @param device Device configuration.
     * @param loadedUserConfiguration Loaded user configuration.
     */
    GXDLMSBase(
            CGXDLMSAssociationShortName* sn,
            CGXDLMSTcpUdpSetup* wrapper,
            LoadedUserConfiguration &loadedUserConfiguration);

    /**
     * @brief Destructor
     */
    ~GXDLMSBase(void);

    /**
     * @brief Check if server socket is still open.
     * @return True if server socket is open. False otherwise.
     */
    bool IsOpen() const;

    /**
     * @brief Method, that returns server socket.
     * @return Server socket.
     */
    int GetSocket() const;

    /**
     * @brief Method, that starts the server for receiving requests from the clients.
     */
    void StartServer();

    /**
     * @brief Method, that stops the server for receiving requests from the clients.
     */
    void StopServer();

    /**
     * @brief Method for initializing DLMS server.
     * @param trace Trace level of logging information.
     */
    void Init(GX_TRACE_LEVEL trace);

    /**
     * @brief Method, that is called when the object is not found by "FindByLN" method. Not implemented yet.
     * @param objectType Type of object.
     * @param sn Short name.
     * @param ln Logical name.
     * @return NULL
     */
    CGXDLMSObject* FindObject(
        DLMS_OBJECT_TYPE objectType,
        int sn,
        std::string& ln);

    /**
     * @brief Method, that is called before some object is read.
     * @param args Request parameters
     */
    void PreRead(
        std::vector<CGXDLMSValueEventArg*>& args);

    /**
     * @brief  Method, that is called before some value of object is written.
     * @param args Request parameters
     */
    void PreWrite(
        std::vector<CGXDLMSValueEventArg*>& args);

    /**
     * @brief Method, that is called before some every action event.
     * @param args Request parameters
     */
    void PreAction(
        std::vector<CGXDLMSValueEventArg*>& args);

    /**
     * @brief Method, that is called after some object is read.
     * @param args Request parameters
     */
    void PostRead(
        std::vector<CGXDLMSValueEventArg*>& args);

    /**
     * @brief Method, that is called after some value of object is written.
     * @param args Request parameters
     */
    void PostWrite(
        std::vector<CGXDLMSValueEventArg*>& args);

    /**
     * @brief Method, that is called after some action is finished.
     * @param args Request parameters
     */
    void PostAction(
        std::vector<CGXDLMSValueEventArg*>& args);

    /**
     * @brief Method that checks if this server is requested server.
     * @param serverAddress Address of server.
     * @param clientAddress Address of client.
     * @return True if requested device address corresponds with address of this server.
     */
    bool IsTarget(
        unsigned long int serverAddress,
        unsigned long clientAddress);

    /**
     * @brief  Method, that validates first phrase of client authentication - NONE and LOW authentication.
     * @param authentication Type of client authentication.
     * @param password Received password from client.
     * @return Enum type which depends on result of validation.
     */
    DLMS_SOURCE_DIAGNOSTIC ValidateAuthentication(
        DLMS_AUTHENTICATION authentication,
        CGXByteBuffer& password);

    /**
    * @brief  Method, that returns access mode to given object attribute.
    * @param arg Information about requested object.
    * @return Access mode to the specified attribute of object.
    */
    DLMS_ACCESS_MODE GetAttributeAccess(CGXDLMSValueEventArg* arg);

    /**
    * @brief Method, that returns access mode to given object method.
    * @param arg Information about requested object.
    * @return Access mode to the specified attribute of object.
    */
    DLMS_METHOD_ACCESS_MODE GetMethodAccess(CGXDLMSValueEventArg* arg);

    /**
    * @brief Accepted connection is made for the server. All initialization is done here.
    * @param arg Information about connection.
    */
    void Connected(
        CGXDLMSConnectionEventArgs& connectionInfo);

    /**
     * @brief Client has try to made invalid connection. Password is incorrect.
     * @param connectionInfo Connection information.
     */
    void InvalidConnection(
        CGXDLMSConnectionEventArgs& connectionInfo);
    /**
     * @brief Server has close the connection. All clean up is made here.
     * @param connectionInfo Connection information.
     */
    void Disconnected(
        CGXDLMSConnectionEventArgs& connectionInfo);

    /**
    * @brief Method, that is called before Capture method is called on profile Generic object.
    * @param args Request parameters
    */
    void PreGet(std::vector<CGXDLMSValueEventArg*>& args);

    /**
    * @brief Method, that is called after Capture method is called on profile Generic object.
    * @param args Value event arguments.
    */
    void PostGet(std::vector<CGXDLMSValueEventArg*>& args);

    /**
     * @brief Method, that loads content of file with profile generic data.
     * @param pathToFolder Path to folder where profile generic data to be saved.
     * @param profileGeneric Profile generic object for which the saved data to be loaded.
     * @return Loaded document.
     */
    static pugi::xml_document LoadProfileGenericData(const std::string &pathToFolder, CGXDLMSProfileGeneric &profileGeneric);

    /**
     * @brief Method, that saves content of file with profile generic data.
     * @param doc Document to be saved.
     * @param pathToFolder Path to folder where profile generic data to be saved.
     * @param profileGeneric Profile generic object for which the new data to be saved.
     */
    static void SaveProfileGenericData(const pugi::xml_document& doc, const std::string &pathToFolder, CGXDLMSProfileGeneric &profileGeneric);

    /**
     * @brief Method, that creates path to file with profile generic data.
     * @param pathToFolder Path to folder where profile generic data to be saved.
     * @param profileGeneric Profile generic object for which the path to file to be created.
     * @return Path to file where data for specified profile generic file to be saved.
     */
    static std::string CreatePathToProfileGenericFile(const std::string &pathToFolder, CGXDLMSProfileGeneric &profileGeneric);

    /**
     * @brief Method, that saves new record that contains actual values of specified objects to the profile generic file.
     * @param writerStruct Structure that contains data about capturing object.
     */
    static void Capture(ProfileGenericWriterStruct &writerStruct);

    /** Not used yet.
     */
    void HandleImageTransfer(CGXDLMSValueEventArg* e);
};