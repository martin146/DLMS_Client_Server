/**
 * @file ServerDirector.h
 * @author Filip Grepl
 * @brief Definition of 'ServerDirector' class
 * @version 1.0
 * @date 2021-10-08
 *
 * @copyright Copyright (c) 2021
 *
 */

#ifndef ServerDirector_H
#define ServerDirector_H

#include <ApplicationConfigParser/Configurations/LoadedUserConfiguration.h>
#include "BaseServer/GXDLMSBase.h"

#if defined(_WIN32) || defined(_WIN64)
#include <winsock.h>
#endif

class ServerDirector
{
private:

    void CheckCorrectServerAddress();

#if defined(_WIN32) || defined(_WIN64)
	WSADATA wsaData;
#endif

	LoadedUserConfiguration loadedUserConfiguration;

    GXDLMSBase* server;
protected:


public:
	~ServerDirector();

    void CreateServer(const char *pathToConfigFile);

	void StartServer() const;

	void PrintConfig();
};

#endif

