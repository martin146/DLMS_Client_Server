/**
 * @file ApplicationConfigParser.h
 * @author Filip Grepl
 * @brief Definition of 'ApplicationConfigParser' class
 * @version 1.0
 * @date 2021-10-08
 *
 * @copyright Copyright (c) 2021
 *
 */

#ifndef DLMS_CLIENT_SERVER_APPLICATIONCONFIGPARSER_H
#define DLMS_CLIENT_SERVER_APPLICATIONCONFIGPARSER_H

#include "ApplicationConfigParser/Configurations/LoadedUserConfiguration.h"
#include <vector>
#include <memory>
#include <PugiXML/pugixml.hpp>
#include <ServerConfigParser/ConfigParserServer.h>
#include <GURUX_DLMS/ExtendedObjects/GXDLMSAssociationLogicalNameE.h>
#include <GURUX_DLMS/Objects/SettingUpDataExchangeInternet/GXDLMSIp4Setup.h>
#include <GURUX_DLMS/Objects/AccessControlAndManagement/GXDLMSSecuritySetup.h>

/**
 * @brief Class for loading Hatel configuration.
 */
class ApplicationConfigParser {
private:
    ConfigParserServer configParserServer;


    LoadedUserConfiguration &loadedUserConfiguration;
    const char *pathToApplicationConfigFile;

    void parseHatelConfiguration(pugi::xml_node &hatelNode);

    void parseDlmsPathsConfiguration(pugi::xml_node &dlmsServerPathsConfigurationNode);

    void parseDlmsServerConfiguration(pugi::xml_node &dlmsServerConfiguration);

    void parseObjectsConfiguration(pugi::xml_node &rootNode);

    // Register parsing
    void parseRegisterConfiguration(pugi::xml_node &registerNode);

    // Data parsing
    void parseDataConfiguration(pugi::xml_node &dataNode);

    // Profile generic parsing
    void parseProfileGenericConfiguration(pugi::xml_node &profileGenericNode);

    // Clock parsing
    void parseClockConfiguration(pugi::xml_node &clockNode);

    // IecHdlcSetup parsing
    void parseIecHdlcSetupConfiguration(pugi::xml_node &iecHdlcSetupNode);

    // TcpUdpSetup parsing
    void parseTcpUdpSetup(pugi::xml_node &tcpUdpSetupNode);

    // IPv4Setup parsing
    void parseIPv4Setup(pugi::xml_node &ipv4setupNode);

    // IP options of IPv4Setup parsing
    void parseIpOptions(pugi::xml_node &ipOptionNode, CGXDLMSIp4Setup &ipv4setup);

    // MacAddressSetup parsing
    void parseMacAddressSetup(pugi::xml_node &macAddressSetupNode);

    // Association logical name parsing
    void parseAssociationConfiguration(pugi::xml_node &associationNode);

    void parseAssociationAttributes(pugi::xml_node &associationNode, CGXDLMSAssociationLogicalNameE &ln);

    void parseObjectList(pugi::xml_node &objectListNode, CGXDLMSAssociationLogicalNameE &ln);

    void parsePartnersId(pugi::xml_node &partnersIdNode, CGXDLMSAssociationLogicalNameE &ln);

    void parseApplicationContextName(pugi::xml_node &applicationContextNameNode, CGXDLMSAssociationLogicalNameE &ln);

    void parseXDlmsContextInfo(pugi::xml_node &xDlmsContextInfoNode, CGXDLMSAssociationLogicalNameE &ln);

    void parseAuthenticationMechanismName(pugi::xml_node &autMechNameNode, CGXDLMSAssociationLogicalNameE &ln);

    void parseSecuritySetup(pugi::xml_node &securitySetupNode);

    void parseCertificate(pugi::xml_node certificateNode, CGXDLMSSecuritySetup &securitySetup);

public:

    ApplicationConfigParser(const char *pathToApplicationConfigFile, LoadedUserConfiguration &loadedUserConfiguration);

    /**
     * @brief Method, that loads and parses the Hatel configuration
     * @param pathToApplicationConfigFile Path to Hatel configuration file.
     * @param loadedUserConfiguration Reference to object where loaded configuration to be saved.
     */
    void LoadApplicationConfiguration();
};

#endif //DLMS_CLIENT_SERVER_APPLICATIONCONFIGPARSER_H
