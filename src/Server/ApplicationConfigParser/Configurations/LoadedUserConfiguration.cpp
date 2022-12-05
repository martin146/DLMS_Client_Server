/**
 * @file LoadedUserConfiguration.cpp
 * @author Filip Grepl
 * @brief Implementation of 'LoadedUserConfiguration' class
 * @date 2021-10-08
 *
 * @copyright Copyright (c) 2020
 *
 */

#include <ApplicationConfigParser/Configurations/LoadedUserConfiguration.h>


DlmsServerPathsConfiguration &LoadedUserConfiguration::getDlmsServerPathsConfiguration() {
    return dlmsServerPathsConfiguration;
}

HatelConfiguration &LoadedUserConfiguration::getHatelConfiguration() {
    return hatelConfiguration;
}

CGXDLMSObjectCollectionE &LoadedUserConfiguration::getLoadedObjects() {
    return loadedObjects;
}

DlmsServerConfiguration &LoadedUserConfiguration::getDlmsServerConfiguration() {
    return dlmsServerConfiguration;
}
