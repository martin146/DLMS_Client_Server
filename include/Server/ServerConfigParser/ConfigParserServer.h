/**
 * @file ConfigParserServer.h
 * @author Filip Grepl
 * @brief Definition of 'ConfigParserServer' class
 * @version 1.0
 * @date 2021-10-08
 *
 * @copyright Copyright (c) 2021
 *
 */


#ifndef DLMS_CLIENT_SERVER_CONFIGPARSERSERVER_H
#define DLMS_CLIENT_SERVER_CONFIGPARSERSERVER_H

#include "ConfigParser.h"

 /**
  * @brief This class parses data of DLMS server configuration.
  */
class ConfigParserServer : public ConfigParser
{
private:

    /**
     * @brief States of parser
     */
    enum ParserState {
        ONE_LINE_PARSING,
        CONFORMANCE_PARSING,
        POLICY_PARSING
    };

    /**
     * @brief State of parser.
     */
    ParserState parserState;

	/**
	 * @brief Method, that parses the trace level of target server from config file.
	 * @param value Loaded value.
	 * @return Enum value of trace level.
	 */
    GX_TRACE_LEVEL GetTraceValue(std::string &value) const;

protected:

public:

    /**
     * @brief Constructor.
     */
	ConfigParserServer();
	/**
	 * @brief Destructor.
	 */
	~ConfigParserServer();

    /**
     * @brief Method, that converts hatel data type to DLMS data type.
     * @param value Hatel data type to be converted to DLMS data type.
     * @return DLMS data type that corresponds with input hatel data type.
     * @throws ParseException if conversion failed.
     */
    DLMS_DATA_TYPE GetDataType (const std::string &value);


    /**
     * @brief Method, that splits string by comma character.
     * @param value String, where the items are delimited by comma.
     * @return Vector of items.
     */
    std::vector<std::string> SplitStringByComma(std::string &value);

    /**
     * @brief Method, taht coverts ip address from string to unsigned int value.
     * @param value Ip address in string to be converted to unsigned char.
     * @return Ip address represented by unsigned int value.
     */
    unsigned int ConvertStringIpToUInt(const std::string &value);

    /**
     * @brief Method, that converts string to DLMS security policy type of version 1.
     * @param value String to be converted to DLMS security policy type of version 1.
     * @return Enum value of DLMS security policy type of version 1.
     * @throws ParseException if conversion failed.
     */
    DLMS_CLOCK_BASE GetClockBaseValue(const std::string value) const;

};

#endif // DLMS_CLIENT_SERVER_CONFIGPARSERSERVER_H
