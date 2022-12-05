#ifndef DATASTRUCTURETEMPLATESCLIENT_H
#define DATASTRUCTURETEMPLATESCLIENT_H

/**
 * @file DataStructureTemplates.h
 * @author Dominik Majer
 * @brief Basic data structures for client app with default values
 * @date 2020-01-01
 *
 * @copyright Copyright (c) 2020
 *
 */

// Use GuruxCore enumerators
#include "GURUX_DLMS/enums/enums.h"
#include "GURUX_DLMS/GXBytebuffer.h"

#include <CommonDataStructureTemplates.h>

#include <string>
#include <chrono>


/**
 * @brief Cesta kam ukládat logy.
 *
 */
static const std::string DEV_VALUE_LOG_FOLDER = "../Logs/Server_ReadValues/";

static const std::string CLIENT_LOG_PATH = "../Logs/ClientGeneralLog.txt";

static const std::string DEVICE_LIST_FILE_PATH = "/home/student/Documents/DLMS_AIS_Server_Application-master/DLMS_Client_Server/ConfigClient/Devices.txt";
static const std::string DEVICE_FOLDER_PATH = "/home/student/Documents/DLMS_AIS_Server_Application-master/DLMS_Client_Server/ConfigClient/Devices/";

/**
 * @brief Server config structure 
 * 
 */
struct DeviceConfig
{
	std::string name = "N/A";											// Just server name
	std::string manufacturer = "N/A";									// Manufacturer
	std::string serialNumber = "N/A";
	DLMS_INTERFACE_TYPE interfaceType = DLMS_INTERFACE_TYPE_HDLC;       // HDLC vs TCP WRAPPER
	bool useLogicalNameReferencing = true;								// Logical/short name referencing
    BYTE clientAddress = 0x11;											// Client DLMS/COSEM address
	
	unsigned int serverAddressSize = 2;
	unsigned int serverAddress = 0x01;
    //BYTE logicalServer[2] = { 0x00 , 0x00 };							// In txt config must be written as 2B
    //BYTE physicalServer[2] = { 0x01 , 0x00 };							// In txt config must be written as 2B

    unsigned int logicalServerM = 0x00;
    unsigned int physicalServerM = 0x01;

	unsigned int invocationCounter = 1;

	DLMS_IEC_PROTOCOL startProtocol = DLMS;								// IEC vs DLMS; It is always DLMS now

    MaxPDUSize_HDLC maxPayloadSize;
	WindowSize_HDLC maxWindowSize;
	unsigned int maxPDUsize = 65535;

	unsigned int inactivityTimeout = 60;
	DLMS_PRIORITY priority = DLMS_PRIORITY_NORMAL;
	DLMS_SERVICE_CLASS serviceClass = DLMS_SERVICE_CLASS_UN_CONFIRMED;

	DLMS_CONFORMANCE conformance = (DLMS_CONFORMANCE) (
		DLMS_CONFORMANCE_GENERAL_BLOCK_TRANSFER |
		DLMS_CONFORMANCE_READ |
		DLMS_CONFORMANCE_WRITE
		);

};

/**
 * @brief ConfigClient of TCP socket
 * 
 */
struct NetConfig
{
	std::string hostName = "127.0.0.1";					// IPv4 address
	unsigned short port = 4059;							// TPC/UDP port
	IP_TRANSPORT_PROTOCOL ipTransportProtocol = TCP;	// Transport protocol
	std::string macAddress = "N/A";						// not sure exact usage; I guess MAC address	// not listed in sent xls
	unsigned short waitTime = 60;
};




#endif // DATASTRUCTURETEMPLATESCLIENT_H
