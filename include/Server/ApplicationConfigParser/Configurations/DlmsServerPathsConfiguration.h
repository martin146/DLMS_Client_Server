//
// Created by filip on 19.09.2021.
//

#ifndef DLMS_CLIENT_SERVER_DLMSSERVERPATHSCONFIGURATION_H
#define DLMS_CLIENT_SERVER_DLMSSERVERPATHSCONFIGURATION_H

#include <string>

/**
 * @brief Class with data of paths for DLMS server.
 */
class DlmsServerPathsConfiguration {
private:
    std::string pathToSaveProfileGenericData;
    std::string pathToLogFile;

public:

    /** GETTERS AND SETTERS **/

    const std::string &getPathToSaveProfileGenericData() const;

    void setPathToSaveProfileGenericData(const std::string &pathToSaveProfileGenericData);

    const std::string &getPathToLogFile() const;

    void setPathToLogFile(const std::string &pathToLogFile);

};

#endif //DLMS_CLIENT_SERVER_DLMSSERVERPATHSCONFIGURATION_H
