/**
 * @file ClientDirector.cpp
 * @author Dominik Majer
 * @brief Implementation of 'ClientDirector' class
 * @date 2020-01-01
 *
 * @copyright Copyright (c) 2020
 *
 */

#include "ClientDirector.h"
#include "Logger.h"
//#include <windows.h>
//#include <filesystem>
#include <fstream>
#include <iostream>
#include "boost/format.hpp"

ClientDirector::~ClientDirector()
{
	Logger::LogEvent(Logger::ERROR, "ClientDirector::~ClientDirector() Calling destructor");
#ifdef _WIN32
	WSACleanup();
#endif
}

ClientDirector::ClientDirector() :
	cp()
{
#if defined(_WIN32) || defined(_WIN64)
try
	{

		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		{
			Logger::LogEvent(Logger::INFO, GET_VAR_NAME(GENERAL_ERROR) + std::string().assign("Could not find a usable WinSock DLL/LIB"));
			throw(0);
		}
	}
	catch (...)
	{
		throw(0);
	}
#endif
}

int ClientDirector::ShowAllDevices_1()
{
	short it = 0;
	printf("----------------------------------------------------------------\n");
	printf("___\n");
	printf(" |-0. Back...\n");

	for (auto const& dev : m_devHandler->devices)
	{
		printf(" |-%d. %s\n", ++it, dev->devConfig.name.c_str());
	}

	int devNumber = 0;
	std::cin >> devNumber;

	if (devNumber == 0)
		return -2;

	try
	{
		m_devHandler->devices.at(devNumber - 1);
		ShowDeviceMenu_2(devNumber - 1);
		return 1;
	}
	catch (...)
	{
		return 0;
	}


}

int ClientDirector::ReadObjectList(Device* dev)
{

	std::ifstream inputFile;
	std::string inputFile_path;
	std::string s;

	std::cin >> inputFile_path;
	inputFile.open(inputFile_path);

	if (!inputFile.is_open())
	{
		Logger::LogEvent(Logger::INFO, std::string().append("Cannot open: " + inputFile_path));
		Logger::LogEvent(Logger::ERROR, std::string().append("Opening default file ../AutoRead/ar01.txt"));
		inputFile.open("../AutoRead/ar01.txt");
		if (!inputFile.is_open())
		{
			Logger::LogEvent(Logger::INFO, std::string().append("Cannot open: ../AutoRead/ar01.txt"));
			return N_OK;
		}
	}

	std::string line = "";
	std::string value = "";
	CGXDLMSObject* obj;
	std::vector<std::string> attribs;

	dev->guruxLinker->Connect(dev->netConfig.hostName.c_str(), dev->netConfig.port);
	dev->guruxLinker->InitializeConnection();
	dev->guruxLinker->GetAssociationView();

	do
	{
		getline(inputFile, line);

		if (dev->secureClient->GetUseLogicalNameReferencing())
			obj = dev->secureClient->GetObjects().FindByLN(DLMS_OBJECT_TYPE_ALL, line);
		else
			obj = dev->secureClient->GetObjects().FindBySN(static_cast<unsigned short>(std::stoi(line)));

		if (obj == NULL)
		{
			Logger::LogEvent(
				Logger::INFO,
				(boost::format("[%s] -> Object [%s] not found in current Association View") % dev->devConfig.name % line).str()
			);
			continue;
		}
		obj->GetValues(attribs);

		Logger::LogEvent(
			Logger::ERROR,
			(boost::format("[%s] -> Start Reading Object [%s] ") % dev->devConfig.name % line).str()
		);

		for (std::vector<std::string>::iterator it = attribs.begin(); it != attribs.end(); ++it)
			dev->guruxLinker->Read(obj, it - attribs.begin(), value);

		Logger::LogEvent(
			Logger::ERROR,
			(boost::format("[%s] -> Object [%s] has been read") % dev->devConfig.name % line).str()
		);

	} while (!inputFile.eof());

	dev->guruxLinker->Close();

	return OK;
}

int ClientDirector::ShowDeviceMenu_2(int devNumber)
{
	Device* dev = m_devHandler->devices.at(devNumber);
	int option = 0;

	CGXDLMSConverter* conv = nullptr;

	while (true)
	{
		printf("----------------------------------------------------------------\n");
		printf("______\n");
		printf("  |-0. Back...\n");
		printf("  |-1. Show All Objects\n");
		printf("  |-2. Update Association View\n");
		printf("  |-3. Read All Objects\n");
		printf("  |-4. Read Specific Object\n");
		printf("  |-5. Read Object Collection From File...\n");
		printf("  |-6. Update Device ConfigClient\n");
		std::cin >> option;
		switch (option)
		{
		case 0:
			return -2;

		case 1:
			dev->ListAllObjects();
			break;

		case 2:
			dev->guruxLinker->Connect(dev->netConfig.hostName.c_str(), dev->netConfig.port);
			dev->guruxLinker->InitializeConnection();
			dev->guruxLinker->GetAssociationView();
			dev->guruxLinker->Close();
			conv = new CGXDLMSConverter;
			conv->UpdateOBISCodeInformation(dev->secureClient->GetObjects());
			delete conv;
			conv = nullptr;
			break;

		case 3:
			dev->guruxLinker->Connect(dev->netConfig.hostName.c_str(), dev->netConfig.port);
			dev->guruxLinker->ReadAll();
			dev->guruxLinker->Close();
			break;

		case 4:
			if (dev->ListAllObjects() == N_OK)
				break;
			else
				dev->ReadObject();
			break;

		case 5:
			ReadObjectList(dev);
			break;

		case 6:
			m_devHandler->UpdateDeviceConfig(dev);
			break;
		}
	}

	return OK;
}

