#ifndef DLMS_CLIENT_SERVER_CONFIGPARSERCLIENT_H
#define DLMS_CLIENT_SERVER_CONFIGPARSERCLIENT_H

/**
 * @file ConfigParserClient.h
 * @author Dominik Majer
 * @brief Definition of 'ConfigParserClient' class
 * @version 0.1
 * @date 2020-02-19
 *
 * @copyright Copyright (c) 2020
 * 
 */

#include "ConfigParser.h"
#include "Device.h"

/**
 * @brief Tato třída parsuje všechny druhy config souborů v naší aplikaci.
 * 
 */
class ConfigParserClient : public ConfigParser
{
private:

	bool m_confFlag = false;


protected:



public:
	/**
	 * @brief Constructor.
	 * 
	 */
	ConfigParserClient(void);
	/**
	 * @brief Destructor.
	 */
	~ConfigParserClient(void);

	

	/**
	 * @brief Tato funkce rozparsuje jednu line z config souboru.
	 * 
	 */
	void ParseConfigLine(Device*, std::string);

};

#endif // DLMS_CLIENT_SERVER_CONFIGPARSERCLIENT_H
