/**
 * @file DlmsServerPathsConfiguration.cpp
 * @author Filip Grepl
 * @brief Implementation of 'DlmsServerPathsConfiguration' class
 * @date 2021-10-08
 *
 * @copyright Copyright (c) 2020
 *
 */

#include <string>
#include <ApplicationConfigParser/Configurations/DlmsServerPathsConfiguration.h>

const std::string &DlmsServerPathsConfiguration::getPathToSaveProfileGenericData() const {
    return pathToSaveProfileGenericData;
}

void DlmsServerPathsConfiguration::setPathToSaveProfileGenericData(const std::string &pathToSaveProfileGenericData) {
    DlmsServerPathsConfiguration::pathToSaveProfileGenericData = pathToSaveProfileGenericData;
}

const std::string &DlmsServerPathsConfiguration::getPathToLogFile() const {
    return pathToLogFile;
}

void DlmsServerPathsConfiguration::setPathToLogFile(const std::string &pathToLogFile) {
    DlmsServerPathsConfiguration::pathToLogFile = pathToLogFile;
}
