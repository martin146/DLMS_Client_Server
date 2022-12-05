/**
 * @file main.cpp
 * @author Dominik Majer
 * @brief ClientApp entry point
 * @date 2020-01-01
 * 
 * @copyright Copyright (c) 2020
 * 
 */

// Disable some warnings
#pragma warning( disable : 6031)
#pragma warning( disable : 26812)

// Memory leak detection for WIN
#ifdef _WIN32
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

#include "ClientDirector.h"
#include "Logger.h"

#include <iostream>
#include <boost/format.hpp>

/**
 * @brief Program abort procedura.
 * 
 */
void Aborting();

/// Local main pointers
ClientDirector* client;
DeviceHandler* devHandler;


/**
 * @brief Entry point.
 * 
 */
int main(int argc, char* argv[])
{
    Logger::InitLogger(CLIENT_LOG_PATH);
    Logger::LogEvent(
            Logger::INFO,
            (boost::format("%s %s") % GET_VAR_NAME(DEVICE_LIST_NOT_FOUND) % DEVICE_LIST_FILE_PATH).str()
    );
#ifdef _WIN32
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	std::cout << "Client Begin..." << std::endl << std::endl;

	// Make instance of Client
	// Try-catch because of non-possibility of return value from constructor
	try
	{
		client = new ClientDirector();
	}
	catch (...)
	{
		Logger::LogEvent(
			Logger::INFO,
			"Could not make instance of ClientDirector"
		);
		Aborting();
	}

	// Make instance of DeviceHandler
	try
	{
		devHandler = new DeviceHandler();
		if (argc > 3)
			devHandler->MakeDeviceFromParams(argc, argv);
	}
	catch (std::exception &e)
	{
	    std::cout << "ERROR IS: " << e.what() << std::endl;
		Logger::LogEvent(
			Logger::INFO,
			"Could not make instance of DeviceHandler"
		);
		Aborting();
	}
	
	Device* dev;
	CGXCommunication* devGuruxHandle;

	long long startReading = GetNow();
	long long now = 0;

	int ret = 0;
	char c = ' ';
	client->LinkDeviceHandler(devHandler);

	// Main loop
	while (true)
	{
		printf("----------------------------------------------------------------\n");
		printf("1. Show all devices\n");
		printf("2. ...\n");
		printf("3. End App\n");

		std::cin >> c;
		switch (c)
		{
		case '1':
			while(client->ShowAllDevices_1() != -2);
			break;

		case '2':

			break;

		case '3':
			Aborting();


		}







	}




	/*
	printVector(devHandler->devices);
	try
	{
		// Main Loop
		// Check if reading from servers should continue or client->clientConfig.readFor time was exceeded
		while ( (now = GetNow()) < (startReading + client->clientConfig.readFor) )
		{

			// Check if some new Device(s) was(were) added to ConfigClient folder
			if (now > (devHandler->GetLastUpdate() + client->clientConfig.updateCheckTimer))
				devHandler->UpdateDeviceList();

			// Loop through vector of Devices and check if Device should be readed
			for (auto const& dev : devHandler->devices)
			{

				if (now > (dev->autoReadConfig.lastReading + dev->autoReadConfig.readTimingLoop))
				{
					ErrorHandler::InfoShow("Start reading device: ", dev->devConfig.name);

					devGuruxHandle = dev->guruxLinker;

					// Establish connection and reading
					devGuruxHandle->Connect(dev->tcpConnConfig.hostName.c_str(), dev->tcpConnConfig.port);
					devGuruxHandle->ReadAll();
					devGuruxHandle->Close();

					dev->autoReadConfig.lastReading = now;



					CGXDLMSSecureClient* c = devGuruxHandle->GetGuruxCore_Client();
					CGXDLMSObjectCollection objs = c->GetObjects();
					//obj->


					devGuruxHandle = nullptr;
				}

			}

			// Loop timing
#ifdef _WIN32
			Sleep(MAIN_LOOP_TIMING);
#elif __linux__
			sleep(MAIN_LOOP_TIMING);
#endif
		}
	}
	catch (...)
	{
		ErrorHandler::GeneralError("Loop failed", "");
		Aborting();
	}
	*/

	//printVector(devHandler->devices);
	Aborting();

	return 0;
}

void Aborting()
{
	delete devHandler;
	delete client;

	printf("\n\n\t ABORTNIG\n");

#ifdef _WIN32
	if (_CrtDumpMemoryLeaks() == TRUE)
		Logger::LogEvent(
			Logger::INFO,
			"MEMORY LEAK!!!"
		);
#endif

	exit(0);
}

/*

void printVector(std::vector<Device*> const& vec)
{
	int it = 0;
	CGXDLMSSecureClient* guruxLink;
	for (auto const& i : vec) 
	{
		guruxLink = i->guruxLinker->GetGuruxCore_Client();

		std::cout	<< "\n" << it++ << ": " << i->devConfig.name		<< " - " 
					<< i->devConfig.manufacturer				<< " - " 
					<< i->autoReadConfig.readTimingLoop	<< " - " 
					<< i->devConfig.password					<< " - " 
					<< i->tcpConnConfig.ipTransportProtocol		<< " - " 
					<< i->devConfig.clientAddress << " - "
					<< i->tcpConnConfig.hostName << "\n" << "\n"
			
			<< std::endl;


		std::cout 
			
			<< guruxLink->GetUseLogicalNameReferencing() << "\n"
			<< guruxLink->GetAuthentication() << "\n"
			<< guruxLink->GetCiphering()->GetSecurity() << "\n"
			<< guruxLink->GetClientAddress() << "\n"
			<< " PASSWORD ?? " << "\n"
			<< " INVO COUNTER ?? " << "\n"
			<< guruxLink->GetInterfaceType() << "\n"
			<< static_cast<int>(guruxLink->GetServerAddress()) << "\n"
			<< static_cast<int>(guruxLink->GetLimits().GetWindowSizeRX()) << "\n"
			<< static_cast<int>(guruxLink->GetLimits().GetWindowSizeTX()) << "\n"
			<< " SERVER ADDRESS SIZE ?? " << "\n"
			<< guruxLink->GetPriority() << "\n"
			<< guruxLink->GetServiceClass() << "\n"
			<< guruxLink->GetMaxPduSize() << "\n"
			<< guruxLink->GetMaxReceivePDUSize() << "\n"
			<< std::endl;

		printf("%x", guruxLink->GetUserID());

		for(int y = 0; y < guruxLink->GetCiphering()->GetSystemTitle().GetSize(); y++)
			printf("%x", guruxLink->GetCiphering()->GetSystemTitle().GetData()[y]);
		printf("\n");

		for (int y = 0; y < guruxLink->GetCiphering()->GetAuthenticationKey().GetSize(); y++)
			printf("%x", guruxLink->GetCiphering()->GetAuthenticationKey().GetData()[y]);
		printf("\n");

		for (int y = 0; y < guruxLink->GetCiphering()->GetBlockCipherKey().GetSize(); y++)
			printf("%x", guruxLink->GetCiphering()->GetBlockCipherKey().GetData()[y]);
		printf("\n");

		for (int y = 0; y < guruxLink->GetCiphering()->GetDedicatedKey().GetSize(); y++)
			printf("%x", guruxLink->GetCiphering()->GetDedicatedKey().GetData()[y]);
		printf("\n");

	}
}
*/
