#ifndef CLIENTDIRECTOR_H
#define CLIENTDIRECTOR_H

/**
 * @file ClientDirector.h
 * @author Dominik Majer
 * @brief Definition of 'ClientDirector' class
 * @version 0.1
 * @date 2020-01-01
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include "DeviceHandler.h"
#include "CGXCommunication.h"

// GuruxCore include
#include "GURUX_DLMS/Client/GXDLMSSecureClient.h"

/**
 * @brief Tato třída uchovává CLIENT config, instanci TCP/IP socketu a DeviceHandler třídu, která obsahuje všechny data vztažená k SM.
 * 
 */
class ClientDirector
{
private:
#if defined(_WIN32)
	WSADATA wsaData;
#endif

	DeviceHandler* m_devHandler;

	ConfigParserClient cp;

	/**
	* @brief Tato funkce zobrazí menu 2. úrovně - konkrétní zařízení.
	*/
	int ShowDeviceMenu_2(int devNumber);



protected:


public:
	/**
	 * @brief Constructor.
	 */
	ClientDirector(void);
	/**
	 * @brief Provede prolinkovaní mezi ClientDirector a DeviceHandler třídou.
	 */
	void LinkDeviceHandler(DeviceHandler* devHandler) { m_devHandler = devHandler; }
	/**
	 * @brief Tato funkce vyčte předdefinovaný list objektů (definováno v txt souboru).
	 */
	int ReadObjectList(Device* dev);

	/**
	 * @brief Destructor.
	 */
	~ClientDirector(void);

	/**
	 * @brief Tato funkce zobrazí menu 1. úrovně.
	 *
	 */
	int ShowAllDevices_1();



};

#endif // CLIENTDIRECTOR_H
 