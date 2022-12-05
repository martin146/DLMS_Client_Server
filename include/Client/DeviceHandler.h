#ifndef DEVICEHANDLER_H
#define DEVICEHANDLER_H

/**
 * @file DeviceHandler.h
 * @author Dominik Majer
 * @brief Definition of 'DeviceHandler' class
 * @date 2020-01-01
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include <vector>


#include "Device.h"
#include "ConfigParserClient.h"


/**
 * @brief Tato třída obsahuje vektor Device tříd - všechny aktuálně nahrané zařízení
 */
class DeviceHandler
{
private:

	ConfigParserClient m_configParser;

	/**
	 * @brief Nahraje list zařízení z DevicesFilePath lokace.
	 * 
	 */
	int LoadDevicesList(void);
	/**
	 * @brief Nahraje zařízení z DevicesFilePath lokace.
	 * 
	 */
	Device* LoadDevice(std::string devName);
	/**
	 * @brief Linkuje Device třídu s Gurux driverem.
	 * 
	 */
	int LinkDeviceToGurux(Device* dev);


protected:

public:
	// Vector of pointers to 'Device' heap instances
	std::vector<Device*> devices;

	/**
	 * @brief Constructor.
	 * 
	 */
	DeviceHandler(void);
	/**
	 * @brief Destructor
	 */
	~DeviceHandler(void);
	/**
	 * @brief Update konfigurace zařízení.
	*/
	int UpdateDeviceConfig(Device* dev);

	
	/**
	* @brief Vytvoření zařízení ze zadaných parametrů.
	*/
	void MakeDeviceFromParams(int argc, char* argv[]);

};

#endif // DEVICEHANDLER_H


