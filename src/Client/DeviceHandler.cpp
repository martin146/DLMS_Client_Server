/**
 * @file DeviceHandler.cpp
 * @author Dominik Majer
 * @brief Implementation of 'DeviceHandler' class
 * @date 2020-01-01
 *
 * @copyright Copyright (c) 2020
 *
 */

#include "DeviceHandler.h"
#include "Logger.h"

#ifdef _WIN32
//#include <filesystem>
#endif
#include <fstream>
#include <boost/format.hpp>
#include "GURUX_DLMS/Client/GXDLMSSecureClient.h"


DeviceHandler::DeviceHandler() :
	m_configParser()
{
	if (!LoadDevicesList())
	{
		Logger::LogEvent(
			Logger::INFO,
			(boost::format("%s %s") % GET_VAR_NAME(DEVICE_LIST_NOT_FOUND) % DEVICE_LIST_FILE_PATH).str()
		);
		throw(0);
	}
}

DeviceHandler::~DeviceHandler(void)
{
	Logger::LogEvent(Logger::ERROR, "DeviceHandler::~DeviceHandler() Calling destructor");

	for (auto const& dev : devices)
		delete dev;
}

int DeviceHandler::LoadDevicesList(void)
{
	Logger::LogEvent(
		Logger::ERROR,
		(boost::format("Loading Device (SERVER) list from %s") % DEVICE_LIST_FILE_PATH).str()
	);

	std::ifstream input;

	input.open(DEVICE_LIST_FILE_PATH);
	if (!input.is_open())
		return 0;

	std::string line;
	Device* dev;
	int row = 0;
	do
	{
		row++;
		dev = nullptr;

		getline(input, line);

		if (line.at(0) == COMMENT_CHAR && line.at(1) == COMMENT_CHAR)
			continue;

		Logger::LogEvent(
			Logger::ERROR,
			(boost::format("[%s] Start adding device") % line).str()
		);
		if ((dev = LoadDevice(line)) != nullptr)
		{
			if (!LinkDeviceToGurux(dev))
			{
				Logger::LogEvent(
					Logger::INFO,
					(boost::format("[%s] DEVICE_COULD_NOT_BE_LINKED_TO_GURUX") % dev->devConfig.name).str()
				);
				continue;
			}

			devices.push_back(dev);
			Logger::LogEvent(
				Logger::ERROR,
				(boost::format("[%s] Device (SERVER) added") % dev->devConfig.name).str()
			);
			continue;
		}
		else
		{
			Logger::LogEvent(
				Logger::INFO,
				(boost::format("[%s] DEVICE_FILE_NOT_FOUND") % line).str()
			);
			continue;
		}

	} while (!input.eof());


	return 1;
}

Device* DeviceHandler::LoadDevice(std::string devName)
{
	std::ifstream input;

	input.open(DEVICE_FOLDER_PATH + devName);
	if (!input.is_open()) 
		return nullptr;

	std::string line = "";

	Device* dev = new Device();
	int row = 0;

	do
	{
		row++;
		getline(input, line);
		try {
            m_configParser.ParseConfigLine(dev, line);
        } catch (ParseException &e) {
            //ErrorHandler::ConfigError("../ConfigClient/Devices/" + devName, e.what(), row);
            throw ("Parsing config file is finished with error");
        }
    } while (!input.eof());

	return dev;
}

int DeviceHandler::LinkDeviceToGurux(Device* dev)
{
    // create server address
    unsigned int sA_tmp = dev->devConfig.physicalServerM;

    if (dev->devConfig.serverAddressSize > 1) {
        sA_tmp |= dev->devConfig.logicalServerM << 8*((dev->devConfig.serverAddressSize)/2);
    }

	dev->devConfig.serverAddress = static_cast<unsigned int>(sA_tmp);

	dev->secureClient = new CGXDLMSSecureClient(dev->devConfig.useLogicalNameReferencing, //ok
		static_cast<int>(dev->devConfig.clientAddress),	//ok
		dev->devConfig.serverAddress,
		dev->security.authentication,	//ok
		dev->security.password.c_str(),	//ok
		dev->devConfig.interfaceType);

	dev->guruxLinker = new CGXCommunication(dev->secureClient,
		dev->netConfig.waitTime,
		NULL,
		dev->devConfig.name);


	//CGXDLMSSettings* guruxSettings = dev->secureClient->GetGuruxSettings();
	CGXByteBuffer tmp;


	// NAME
	/* Non-DLMS -> Device class */

	// MANUFACTURER
	/* Non-DLMS -> Device class */

	// USE LOGICAL NAME REFERENCING
	/* Redundant now -> SERVER is created as LN or SN (Line Zero in config files) */

	// AUTHENTICATION
	/* Init in CGXDLMSSecureClient contructor */

	// SECURITY
	//dev->secureClient->GetCiphering()->SetSecurity(dev->security.securityPolicy1);

	// CLIENT ADDRESS
	dev->secureClient->SetClientAddress(static_cast<unsigned long>(dev->devConfig.clientAddress));

	// PASSWORD
	/* Init in CGXDLMSSecureClient contructor */
	unsigned int addr;
    if (dev->devConfig.serverAddressSize == 1 || dev->devConfig.serverAddressSize == 2) {
        addr =  (dev->devConfig.logicalServerM << 7) | dev->devConfig.physicalServerM;
    } else { // device.devConfig.serverAddressSize == 4
        addr =  (dev->devConfig.logicalServerM << 14) | dev->devConfig.physicalServerM ;
    }
	dev->secureClient->SetServerAddress(addr);

	// INVOCATION COUNTER
	dev->secureClient->GetCiphering()->SetInvocationCounter(static_cast<unsigned long>(dev->devConfig.invocationCounter));

	// INTERFACE TYPE
	/* Redundant now -> SERVER is created as LN or SN (Line Zero in config files) */

	// SERVER ADDRESS
	/* Init in CGXDLMSSecureClient contructor */

	// HOSTNAME
	/* Non-DLMS */

	// PORT
	/* Non-DLMS */

	// TRANSPORT PROTOCOL
	/* Non-DLMS */

	// CONFORMANCE (all conformance...)
	dev->secureClient->SetProposedConformance(dev->devConfig.conformance);


	// WINDOIW SIZE IN TRANSIT
    dev->secureClient->GetLimits().SetWindowSizeTX(static_cast<unsigned char>(dev->devConfig.maxWindowSize.transmit));

	// WINDOIW SIZE IN RECEIVE
    dev->secureClient->GetLimits().SetWindowSizeRX(static_cast<unsigned char>(dev->devConfig.maxWindowSize.receive));

	// SERVER ADDRESS SIZE
	/* Non-DLMS */

	// INACTIVITY TIMEOUT
	/* CGXCommunication constructor */

	// PRIORITY
    dev->secureClient->SetPriority(dev->devConfig.priority);

	// SERVICE CLASS
    dev->secureClient->SetServiceClass(dev->devConfig.serviceClass);

	// MAX PAYLOAD SIZE IN TRANSIT
	dev->secureClient->GetLimits().SetMaxInfoTX(dev->devConfig.maxPayloadSize.transmit);

	// MAX PAYLOAD SIZE IN RECEIVE
	dev->secureClient->GetLimits().SetMaxInfoRX(dev->devConfig.maxPayloadSize.transmit);

	// MAX PDU SIZE 
	//dev->secureClient->SetMaxServerPDUSize(static_cast<unsigned short>(dev->devConfig.maxPDUsize));
	dev->secureClient->SetMaxReceivePDUSize(static_cast<unsigned short>(dev->devConfig.maxPDUsize));

	// USER ID
	//guruxSettings->SetUserID(static_cast<unsigned char>(dev->devConfig.userID));
	dev->secureClient->SetUserID(static_cast<unsigned char>(dev->security.userID));

	// SYSTEM TITLE
	GXHelpers::HexToBytes(dev->security.sysTitle, tmp);
	dev->secureClient->GetCiphering()->SetSystemTitle(tmp);

	// AUTHENTICATION KEY
	GXHelpers::HexToBytes(dev->security.authenticationKey, tmp);
	dev->secureClient->GetCiphering()->SetAuthenticationKey(tmp);

	// BLOCK CIPHER KEY
	GXHelpers::HexToBytes(dev->security.blockCipherKey, tmp);
	dev->secureClient->GetCiphering()->SetBlockCipherKey(tmp);

	// DEDICATED KEY
	GXHelpers::HexToBytes(dev->security.dedicatedKey, tmp);
	dev->secureClient->GetCiphering()->SetDedicatedKey(tmp);


	return 1;
}


int DeviceHandler::UpdateDeviceConfig(Device* dev)
{
	Logger::LogEvent(
		Logger::ERROR,
		(boost::format("[%s] Start updating config from file") % dev->devConfig.name).str()
	);

	std::ifstream input;
	input.open(DEVICE_FOLDER_PATH + dev->devConfig.name);
	if (!input.is_open())
	{
		Logger::LogEvent(
			Logger::INFO,
			(boost::format("[%s] DEVICE_FILE_NOT_FOUND") % dev->devConfig.name).str()
		);
		return N_OK;
	}

	std::string line = "";
	int row = 0;

	do
	{
		row++;
		getline(input, line);
        try {
            m_configParser.ParseConfigLine(dev, line);
        } catch (ParseException &e) {
            //ErrorHandler::ConfigError("../ConfigClient/Devices/" + dev->devConfig.name, e.what(), row);
            throw ("Parsing config file is finished with error");
        }
	} while (!input.eof());

	Logger::LogEvent(
		Logger::ERROR,
		(boost::format("[%s] Device config successfully updated") % dev->devConfig.name).str()
	);

	return OK;
}

void DeviceHandler::MakeDeviceFromParams(int argc, char* argv[])
{
	Logger::LogEvent(
		Logger::ERROR,
		(boost::format("Start adding device from parameters...")).str()
	);

	std::string line = "";
	Device* dev = new Device();
	std::string devName;
	int retP = 0;
	char buffer[100];

	int argPos = 2;

	unsigned int numberDevToAdd = argv[1][0] - '0'; // only 0-9 for now

	std::string param;
	std::string value;

	/*
	Format: app.exe {numberOfSERVERs} {SERVER01:Param01}={SERVER01:Value01} {SERVER01:ParamN}={SERVER01:ValueN} {?}
									  {SERVER02:Param01}={SERVER02:Value01} {SERVER02:ParamN}={SERVER02:ValueN} {?} 
									  {...}
	*/

	for (unsigned int devNmb = 0; devNmb < numberDevToAdd; devNmb++)
	{
		dev = new Device();
		devName.assign((boost::format("ParamDev-%d") % (devNmb +1)).str());

		Logger::LogEvent(
			Logger::ERROR,
			(boost::format("Start adding device: %s") % devName).str()
		);

		for ( ; argPos < argc ; )
		{
			if (*argv[argPos] == '?')
			{
				argPos++;
				break;
			}

            try {
                line.assign(argv[argPos]);
                m_configParser.ParseConfigLine(dev, line);
            } catch (ParseException &e) {
                //ErrorHandler::ConfigError("From arguments", e.what(), argPos);
                throw ("Parsing config file is finished with error");
            }

			argPos++;

		}

		if (dev->devConfig.name == "N/A")
			dev->devConfig.name.assign(devName);


		if (!LinkDeviceToGurux(dev))
		{
			Logger::LogEvent(
				Logger::INFO,
				(boost::format("DEVICE_COULD_NOT_BE_LINKED_TO_GURUX: %s") % dev->devConfig.name).str()
			);
		}
		else
		{
			devices.push_back(dev);
			Logger::LogEvent(
				Logger::ERROR,
				(boost::format("Device (SERVER) added: %s") % dev->devConfig.name).str()
			);
		}

		dev = nullptr;
	}




}


