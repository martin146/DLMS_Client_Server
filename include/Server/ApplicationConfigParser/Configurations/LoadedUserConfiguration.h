/**
 * @file LoadedUserConfiguration.h
 * @author Filip Grepl
 * @brief Definition of 'LoadedUserConfiguration' class
 * @version 1.0
 * @date 2021-10-08
 *
 * @copyright Copyright (c) 2021
 *
 */

#ifndef DLMS_CLIENT_SERVER_LOADEDUSERCONFIGURATION_H
#define DLMS_CLIENT_SERVER_LOADEDUSERCONFIGURATION_H

#include "DlmsServerPathsConfiguration.h"
#include "HatelConfiguration.h"
#include <vector>
#include <memory>
#include <GURUX_DLMS/ExtendedObjects/GXDLMSObjectCollectionE.h>
#include "GURUX_DLMS/Objects/AccessControlAndManagement/GXDLMSAssociationLogicalName.h"
#include "DlmsServerConfiguration.h"

/**
 * @brief Class with data about loaded user configuration.
 */
class LoadedUserConfiguration {
private:
    DlmsServerPathsConfiguration dlmsServerPathsConfiguration;
    DlmsServerConfiguration dlmsServerConfiguration;
    HatelConfiguration hatelConfiguration;
    CGXDLMSObjectCollectionE loadedObjects;

public:

    /** GETTERS AND SETTERS **/

    DlmsServerPathsConfiguration &getDlmsServerPathsConfiguration();

    DlmsServerConfiguration &getDlmsServerConfiguration();

    HatelConfiguration &getHatelConfiguration();

    CGXDLMSObjectCollectionE &getLoadedObjects();


};

#endif //DLMS_CLIENT_SERVER_LOADEDUSERCONFIGURATION_H
