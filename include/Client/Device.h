#ifndef DEVICE_H
#define DEVICE_H

/**
 * @file ServerConfiguration.h
 * @author Dominik Majer
 * @brief Definition of 'ServerConfiguration' class
 * @date 2020-01-01
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include "DataStructureTemplatesClient.h"
#include "CGXCommunication.h"
#include "Logger.h"

/**
 * @brief Tato třída drží veškerá data vztažená k jednomu danému SM a obsahuje link do Gurux driveru
 * 
 */


class Device
{

private:

protected:

public:

	DeviceConfig devConfig;
	Security security;
	NetConfig netConfig;

	// Linkage to GuruxCore
	CGXCommunication* guruxLinker;
	CGXDLMSSecureClient* secureClient;

	/**
	 * @brief Constructor.
	 *
	 */
	Device(void);
	/**
	 * @brief Destructor.
	 */
	~Device(void);

	/**
	 * @brief Zobrazí všechny objekty.
	 */
	int ListAllObjects(void) const;
	/**
	 * @brief Zobrazí menu k vyčtení objektu.
	 */
	int ReadObject(void);

	/**
	 * @brief Vrátí pointer na SM instanci.
	 */
	inline Device* GetThisDevice() { return this; }


    const DeviceConfig &getDevConfig() const;

    void setDevConfig(const DeviceConfig &devConfig);

    const Security &getSecurity() const;

    void setSecurity(const Security &security);

    const NetConfig &getNetConfig() const;

    void setNetConfig(const NetConfig &netConfig);

    CGXCommunication *getGuruxLinker() const;

    void setGuruxLinker(CGXCommunication *guruxLinker);

    CGXDLMSSecureClient *getSecureClient() const;

    void setSecureClient(CGXDLMSSecureClient *secureClient);
};


#endif // DEVICE_H
