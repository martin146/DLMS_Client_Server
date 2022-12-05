/**
 * @file ApplicationConfigParser.cpp
 * @author Filip Grepl
 * @brief Implementation of 'ApplicationConfigParser' class
 * @date 2021-10-08
 *
 * @copyright Copyright (c) 2020
 *
 */

#include "ApplicationConfigParser/ApplicationConfigParser.h"
#include <algorithm>

#include <PugiXML/pugixml.hpp>
#include <ServerConfigParser/ConfigParserServer.h>
#include <Exceptions/InitException.h>
#include <GURUX_DLMS/Objects/TimeAndEventBoundControl/GXDLMSClock.h>
#include <GURUX_DLMS/ExtendedObjects/GXDLMSRegisterE.h>
#include <GURUX_DLMS/ExtendedObjects/GXDLMSAssociationLogicalNameE.h>
#include <GURUX_DLMS/Objects/ParametersAndMeasurementData/GXDLMSData.h>
#include <GURUX_DLMS/Objects/SettingUpDataExchangeInternet/GXDLMSMacAddressSetup.h>
#include <GURUX_DLMS/Objects/SettingUpDataExchangeInternet/GXDLMSIp4Setup.h>
#include <GURUX_DLMS/ExtendedObjects/GXDLMSSecuritySetupE.h>

void ApplicationConfigParser::parseHatelConfiguration(pugi::xml_node &hatelNode) {
    for (auto &&opt : hatelNode.children()) {
        if (opt.empty())
            throw ParseException("Element: " + std::string(opt.name()) + " of Hatel Configuration is empty!");
        if (opt.name() == std::string("OutQueue"))
            loadedUserConfiguration.getHatelConfiguration().setOutQueue(opt.text().as_string());
        else if (opt.name() == std::string("InQueue"))
            loadedUserConfiguration.getHatelConfiguration().setInQueue(opt.text().as_string());
        else if (opt.name() == std::string("MaxMsgLen"))
            loadedUserConfiguration.getHatelConfiguration().setMaxMsgLen(configParserServer.GetNumberValue<unsigned int>(opt.text().as_string()));
        else if (opt.name() == std::string("Timeout"))
            loadedUserConfiguration.getHatelConfiguration().setTimeout(configParserServer.GetNumberValue<unsigned int>(opt.text().as_string()));
        else
            throw ParseException("Configuration node: " + std::string(opt.name()) + " of Hatel Configuration is not known!");
    }
}

void ApplicationConfigParser::parseDlmsPathsConfiguration(pugi::xml_node &dlmsServerPathsConfigurationNode) {
    for (auto &&opt : dlmsServerPathsConfigurationNode.children()) {
        if (opt.empty())
            throw ParseException("Element: " + std::string(opt.name()) + " of DLMS paths configuration is empty!");
        if (opt.name() == std::string("PathToSaveProfileGenericData")) {
            loadedUserConfiguration.getDlmsServerPathsConfiguration().setPathToSaveProfileGenericData(opt.text().as_string());
        } else  if (opt.name() == std::string("PathToLogFile")) {
            loadedUserConfiguration.getDlmsServerPathsConfiguration().setPathToLogFile(opt.text().as_string());
        } else
            throw ParseException("Configuration node: " + std::string(opt.name()) + " of DLMS paths configuration is not known!");
    }
}

void ApplicationConfigParser::parseDlmsServerConfiguration(pugi::xml_node &dlmsServerConfiguration) {
    for (auto ait = dlmsServerConfiguration.attributes_begin(); ait != dlmsServerConfiguration.attributes_end(); ++ait) {
        if (ait->empty())
            throw ParseException("Attribute: " + std::string(ait->name()) + " of Dlms server configuration is empty!");
        else if (ait->name() == std::string("name"))
            loadedUserConfiguration.getDlmsServerConfiguration().setName(ait->as_string());
        else if (ait->name() == std::string("logicalAddress"))
            loadedUserConfiguration.getDlmsServerConfiguration().setLogicalAddress(configParserServer.GetNumberValue<unsigned long>(ait->as_string()));
        else if (ait->name() == std::string("physicalAddress"))
            loadedUserConfiguration.getDlmsServerConfiguration().setPhysicalAddress(configParserServer.GetNumberValue<unsigned long>(ait->as_string()));
        else if (ait->name() == std::string("addressSize"))
            loadedUserConfiguration.getDlmsServerConfiguration().setAddressSize(configParserServer.GetNumberValue<unsigned int >(ait->as_string()));
        else if (ait->name() == std::string("interfaceType"))
            loadedUserConfiguration.getDlmsServerConfiguration().setInterfaceType(configParserServer.GetInterfaceType(ait->as_string()));
        else if (ait->name() == std::string("useLogicalNameReferencing"))
            loadedUserConfiguration.getDlmsServerConfiguration().setUseLogicalNameReferencing(configParserServer.GetBoolValue(ait->as_string()));
        else if (ait->name() == std::string("traceLevel"))
            loadedUserConfiguration.getDlmsServerConfiguration().setTraceLevel(configParserServer.GetTraceLevel(ait->as_string()));
        else
            throw ParseException("Attribute: " + std::string(ait->name()) + " of Dlms server configuration is not known!");
    }
}

void ApplicationConfigParser::parseObjectsConfiguration(pugi::xml_node &objectsNode) {
    for (auto &&object : objectsNode.children()) {
        if (object.name() == std::string("Register")) {
            parseRegisterConfiguration(object);
        } else if (object.name() == std::string("Data")) {
            parseDataConfiguration(object);
        } else if (object.name() == std::string("ProfileGeneric")) {
            parseProfileGenericConfiguration(object);
        } else if(object.name() == std::string("Clock")) {
            parseClockConfiguration(object);
        } else if (object.name() == std::string("IecHdlcSetup")) {
            if (loadedUserConfiguration.getDlmsServerConfiguration().getInterfaceType() == DLMS_INTERFACE_TYPE_HDLC)
                parseIecHdlcSetupConfiguration(object);
            else
                throw ParseException(
                        "The unexpected " + std::string(object.name()) + " object is loaded. The entered interface type is " +
                        std::to_string(loadedUserConfiguration.getDlmsServerConfiguration().getInterfaceType()) +
                        ". " + std::string(object.name()) + " object is expected only with HDLC( " + std::to_string(DLMS_INTERFACE_TYPE_HDLC) + ") interface.");
        } else if (object.name() == std::string("TcpUdpSetup"))
            parseTcpUdpSetup(object);
        else if (object.name() == std::string("IPv4Setup"))
            parseIPv4Setup(object);
        else if (object.name() == std::string("MacAddressSetup"))
            parseMacAddressSetup(object);
        else if (object.name() == std::string("AssociationLogicalName")) {
            parseAssociationConfiguration(object);
        } else if (object.name() == std::string("SecuritySetup"))
            parseSecuritySetup(object);
        else
            throw ParseException("Object of type " + std::string(object.name()) + " is not supported!");
    }

    // add all association objects to object list - must be there due to cyclic dependency between association objects
    for (auto &&object : loadedUserConfiguration.getLoadedObjects()) {
        if (object->GetObjectType() == DLMS_OBJECT_TYPE_ASSOCIATION_LOGICAL_NAME) {
            CGXDLMSAssociationLogicalNameE *ln = (CGXDLMSAssociationLogicalNameE*) object;
            for (auto &&assocObject : ln->getObjectsAttributeAccessRights()) {
                std::string logicalName = assocObject.first;
                CGXDLMSObject *o = loadedUserConfiguration.getLoadedObjects().CGXDLMSObjectCollection::FindByLN(DLMS_OBJECT_TYPE_ALL, logicalName);
                if (o == NULL)
                    throw InitException("Object with obis " + logicalName + " to association object " + ln->GetLogicalName() + " is not found!");
                else
                    ln->GetObjectList().push_back(o);
            }
            // check if IecHdlcSetup object has been specified if HDLC interface has been entered
            auto findHdlcObject = std::find_if(ln->GetObjectList().begin(), ln->GetObjectList().end(),
                                               [&](CGXDLMSObject *o) {
                                                   return o->GetObjectType() == DLMS_OBJECT_TYPE_IEC_HDLC_SETUP;
                                               });

            if (loadedUserConfiguration.getDlmsServerConfiguration().getInterfaceType() == DLMS_INTERFACE_TYPE_HDLC) {
                if (ln->GetLogicalName() != CGXDLMSObjectCollectionE::currentAssociationLn &&
                    findHdlcObject == ln->GetObjectList().end()) {
                    throw InitException("Association object " + ln->GetLogicalName() +
                                        " doesn't contain IecHdlSetup object, but a specified interface is HDLC!");
                }
            }
            std::sort(ln->GetObjectList().begin(), ln->GetObjectList().end(), [](CGXDLMSObject *object1, CGXDLMSObject *object2) -> bool {
                if (object1->GetObjectType() == DLMS_OBJECT_TYPE_ASSOCIATION_LOGICAL_NAME && object2->GetObjectType() != DLMS_OBJECT_TYPE_ASSOCIATION_LOGICAL_NAME)
                    return true;
                else
                    return object1->GetLogicalName() < object2->GetLogicalName();
            });
        }
    }
}

void ApplicationConfigParser::parseRegisterConfiguration(pugi::xml_node &registerNode) {
    CGXDLMSRegisterE *r = new CGXDLMSRegisterE();
    r->GetValue() = (int64_t)0;
    for (auto ait = registerNode.attributes_begin(); ait != registerNode.attributes_end(); ++ait) {
        if (ait->empty())
            throw ParseException("Attribute: " + std::string(ait->name()) + " of Register is empty!");
        else if (ait->name() == std::string("valueAccessMode"))
            r->SetAccess(2, configParserServer.GetValueAccessMode(ait->as_string()));
        else if (ait->name() == std::string("resetAccessMode"))
            r->SetMethodAccess(1, configParserServer.GetMethodAccessMode(ait->as_string()));
        else if (ait->name() == std::string("cid"))
            r->SetCid(ait->as_string());
        else if (ait->name() == std::string("type"))
            r->SetDataType(2, configParserServer.GetDataType(ait->as_string()));
        else if (ait->name() == std::string("obis")) {
            CGXDLMSVariant v = ait->as_string();
            r->SetName(v);
        }
        else if (ait->name() == std::string("unit"))
            r->SetUnit(configParserServer.GetUnit(ait->as_string()));
        else if (ait->name() == std::string("scaler"))
            r->SetScaler(configParserServer.GetNumberValue<unsigned int>(ait->as_string()));
        else if (ait->name() == std::string("description")) {
            std::string text = ait->as_string();
            r->SetDescription(text);
        } else
            throw ParseException("Attribute: " + std::string(ait->name()) + " of Register object is not known!");
    }
    loadedUserConfiguration.getLoadedObjects().push_back(r);
}

void ApplicationConfigParser::parseDataConfiguration(pugi::xml_node &dataNode) {
    CGXDLMSData* data = new CGXDLMSData();
    for (auto ait = dataNode.attributes_begin(); ait != dataNode.attributes_end(); ++ait) {
        if (ait->empty())
            throw ParseException("Attribute: " + std::string(ait->name()) + " of Data object is empty!");
        else if (ait->name() == std::string("valueAccessMode"))
            data->SetAccess(2, configParserServer.GetValueAccessMode(ait->as_string()));
        else if (ait->name() == std::string("obis")) {
            CGXDLMSVariant v = ait->as_string();
            data->SetName(v);
        } else if (ait->name() == std::string("value")) {
            CGXDLMSVariant var;
            try {
                var = configParserServer.GetNumberValue<unsigned int>(ait->as_string());
            } catch (std::invalid_argument &e) {
                std::string valueStr = ait->as_string();
                var.Add(valueStr);
            }
            data->SetValue(var);
        } else if (ait->name() == std::string("description")) {
            std::string text = ait->as_string();
            data->SetDescription(text);
        } else
            throw ParseException("Attribute: " + std::string(ait->name()) + " of Data object is not known!");
    }
    loadedUserConfiguration.getLoadedObjects().push_back(data);
}

void ApplicationConfigParser::parseProfileGenericConfiguration(pugi::xml_node &profileGenericNode) {
    CGXDLMSProfileGeneric *pg = new CGXDLMSProfileGeneric();
    for (auto ait = profileGenericNode.attributes_begin(); ait != profileGenericNode.attributes_end(); ++ait) {
        if (ait->empty())
            throw ParseException("Attribute: " + std::string(ait->name()) + " of Profile Generic Configuration is empty!");
        else if (ait->name() == std::string("resetAccessMode"))
            pg->SetMethodAccess(1, configParserServer.GetMethodAccessMode(ait->as_string()));
        else if (ait->name() == std::string("captureMethod"))
            pg->SetMethodAccess(2, configParserServer.GetMethodAccessMode(ait->as_string()));
        else if (ait->name() == std::string("obis")){
            CGXDLMSVariant v = ait->as_string();
            pg->SetName(v);
        } else if (ait->name() == std::string("obisSavedData")) {
            CGXDLMSCaptureObject *capture = new CGXDLMSCaptureObject(2, 0);
            std::string obisCodes = ait->as_string();
            for (auto &&obisToCapture : configParserServer.SplitStringByComma(obisCodes)) {
                CGXDLMSRegister *regToCapture = (CGXDLMSRegister*) loadedUserConfiguration.getLoadedObjects().FindByLN(DLMS_OBJECT_TYPE_ALL, (std::string&) obisToCapture);
                if (regToCapture == NULL) {
                    std::string obis;
                    pg->GetLogicalName(obis);
                    throw InitException("The object to capture " + obisToCapture + " to profile generic " + obis + " was not found.");
                }
                pg->GetCaptureObjects().emplace_back(regToCapture, capture);
            }
        } else if (ait->name() == std::string("capturePeriod"))
            pg->SetCapturePeriod(configParserServer.GetNumberValue<unsigned int>(ait->as_string()));
        else if (ait->name() == std::string("sortMethod"))
            pg->SetSortMethod(configParserServer.GetSortMethodType(ait->as_string()));
        else if (ait->name() == std::string("sortObjectObis")) {

            auto foundSortObject = std::find_if(pg->GetCaptureObjects().begin(), pg->GetCaptureObjects().end(),
                                                [&](std::pair<CGXDLMSObject *, CGXDLMSCaptureObject *> captureObjectPair) {
                                                    return captureObjectPair.first->GetLogicalName()  == ait->as_string();
                                                }) ;
            if (foundSortObject == pg->GetCaptureObjects().end())
                throw ParseException("The sort object " + std::string(ait->as_string()) + " is not included in obis saved data list!");
            pg->SetSortObject(foundSortObject->first);
        } else if (ait->name() == std::string("profileEntries"))
            pg->SetProfileEntries(configParserServer.GetNumberValue<unsigned int>(ait->as_string()));
        else if (ait->name() == std::string("description")){
            std::string text = ait->as_string();
            pg->SetDescription(text);
        } else
            throw ParseException("Attribute: " + std::string(ait->name()) + " of Profile generic confguration is not known!");
    }
    loadedUserConfiguration.getLoadedObjects().push_back(pg);
}

void ApplicationConfigParser::parseClockConfiguration(pugi::xml_node &clockNode) {
    CGXDLMSClock* pClock = new CGXDLMSClock();
    for (auto ait = clockNode.attributes_begin(); ait != clockNode.attributes_end(); ++ait) {
        if (ait->empty())
            throw ParseException("Attribute: " + std::string(ait->name()) + " of Clock configuration is empty!");
        else if (ait->name() == std::string("obis")) {
            CGXDLMSVariant v = ait->as_string();
            pClock->SetName(v);
        } else if (ait->name() == std::string("timeZone"))
            pClock->SetTimeZone(configParserServer.GetNumberValue<short>(ait->as_string()));
        else if (ait->name() == std::string("status")) {
            std::string statuses = ait->as_string();
            pClock->SetStatus(DLMS_CLOCK_STATUS_OK);
            for (auto &&status : configParserServer.SplitStringByComma(statuses)) {
                pClock->SetStatus((DLMS_CLOCK_STATUS) (pClock->GetStatus() | configParserServer.GetClockStatus(status)));
            }
        } else if (ait->name() == std::string("daylightSavingsBegin")) {
            CGXDateTime beginTime;
            beginTime.FromString(ait->as_string());
            pClock->SetBegin(beginTime);
        } else if (ait->name() == std::string("daylightSavingsEnd")) {
            CGXDateTime endTime;
            endTime.FromString(ait->as_string());
            pClock->SetEnd(endTime);
        } else if (ait->name() == std::string("daylightSavingsDeviation")) {
            pClock->SetDeviation(configParserServer.GetNumberValue<unsigned int>(ait->as_string()));
        } else if (ait->name() == std::string("daylightSavingsEnabled")) {
            pClock->SetEnabled(configParserServer.GetBoolValue(ait->as_string()));
        } else if (ait->name() == std::string("clockBase")) {
            pClock->SetClockBase(configParserServer.GetClockBaseValue(ait->as_string()));
        }  else if (ait->name() == std::string("description")){
            std::string text = ait->as_string();
            pClock->SetDescription(text);
        } else
            throw ParseException("Attribute: " + std::string(ait->name()) + " of Clock object is not known!");
    }
    loadedUserConfiguration.getLoadedObjects().push_back(pClock);
}

void ApplicationConfigParser::parseIecHdlcSetupConfiguration(pugi::xml_node &iecHdlcSetupNode) {
    CGXDLMSIecHdlcSetup *hdlcSetup = new CGXDLMSIecHdlcSetup();
    for (auto ait = iecHdlcSetupNode.attributes_begin(); ait != iecHdlcSetupNode.attributes_end(); ++ait) {
        if (ait->empty())
            throw ParseException("Attribute: " + std::string(ait->name()) + " of IecHdlc object is empty!");
        else if (ait->name() == std::string("obis")) {
            CGXDLMSVariant v = ait->as_string();
            hdlcSetup->SetName(v);
        } else if (ait->name() == std::string("comm_speed"))
            hdlcSetup->SetCommunicationSpeed(configParserServer.GetCommSpeed(ait->as_string()));
        else if (ait->name() == std::string("windowSizeTransmit"))
            hdlcSetup->SetWindowSizeTransmit(configParserServer.GetNumberValue<int>(ait->as_string()));
        else if (ait->name() == std::string("windowSizeReceive"))
            hdlcSetup->SetWindowSizeReceive(configParserServer.GetNumberValue<int>(ait->as_string()));
        else if (ait->name() == std::string("maxInfoLengthTransmit"))
            hdlcSetup->SetMaximumInfoLengthTransmit(configParserServer.GetNumberValue<int>(ait->as_string()));
        else if (ait->name() == std::string("maxInfoLengthReceive"))
            hdlcSetup->SetMaximumInfoLengthReceive(configParserServer.GetNumberValue<int>(ait->as_string()));
        else if (ait->name() == std::string("interOctetTimeout"))
            hdlcSetup->SetInterCharachterTimeout(configParserServer.GetNumberValue<int>(ait->as_string()));
        else if (ait->name() == std::string("inactivityTimeout"))
            hdlcSetup->SetInactivityTimeout(configParserServer.GetNumberValue<int>(ait->as_string()));
        else if (ait->name() == std::string("description")){
            std::string text = ait->as_string();
            hdlcSetup->SetDescription(text);
        } else
            throw ParseException("Attribute: " + std::string(ait->name()) + " of IecHdlc object is not known!");
    }
    hdlcSetup->SetDeviceAddress(loadedUserConfiguration.getDlmsServerConfiguration().getPhysicalAddress());
    loadedUserConfiguration.getLoadedObjects().push_back(hdlcSetup);
    loadedUserConfiguration.getLoadedObjects().setIecHdlcSetup(hdlcSetup);
}

void ApplicationConfigParser::parseTcpUdpSetup(pugi::xml_node &tcpUdpSetupNode) {
    CGXDLMSTcpUdpSetup *tcpUdpSetup = new CGXDLMSTcpUdpSetup();
    for (auto ait = tcpUdpSetupNode.attributes_begin(); ait != tcpUdpSetupNode.attributes_end(); ++ait) {
        if (ait->empty())
            throw ParseException("Attribute: " + std::string(ait->name()) + " of TcpUdpSetup is empty!");
        else if (ait->name() == std::string("obis")) {
            CGXDLMSVariant v = ait->as_string();
            tcpUdpSetup->SetName(v);
        } else if (ait->name() == std::string("port"))
            tcpUdpSetup->SetPort(configParserServer.GetNumberValue<int>(ait->as_string()));
        else if (ait->name() == std::string("ipReference")) {
            tcpUdpSetup->SetIPReference(ait->as_string());
        } else if (ait->name() == std::string("maximumSegmentSize"))
            tcpUdpSetup->SetMaximumSegmentSize(configParserServer.GetNumberValue<int>(ait->as_string()));
        else if (ait->name() == std::string("maximumSimultaneousConnections"))
            tcpUdpSetup->SetMaximumSimultaneousConnections(configParserServer.GetNumberValue<int>(ait->as_string()));
        else if (ait->name() == std::string("inactivityTimeout"))
            tcpUdpSetup->SetInactivityTimeout(configParserServer.GetNumberValue<int>(ait->as_string()));
        else if (ait->name() == std::string("description")){
            std::string text = ait->as_string();
            tcpUdpSetup->SetDescription(text);
        } else
            throw ParseException("Attribute: " + std::string(ait->name()) + " of TcpUdpSetup object is not known!");
    }
    loadedUserConfiguration.getLoadedObjects().push_back(tcpUdpSetup);
    loadedUserConfiguration.getLoadedObjects().setTcpUdpSetup(tcpUdpSetup);
}

void ApplicationConfigParser::parseIPv4Setup(pugi::xml_node &ipv4setupNode) {
    CGXDLMSIp4Setup *ipv4setup = new CGXDLMSIp4Setup();
    for (auto ait = ipv4setupNode.attributes_begin(); ait != ipv4setupNode.attributes_end(); ++ait) {
        if (ait->empty())
            throw ParseException("Attribute: " + std::string(ait->name()) + " of TcpUdpSetup object is empty!");
        else if (ait->name() == std::string("obis")) {
            CGXDLMSVariant v = ait->as_string();
            ipv4setup->SetName(v);
        } else if (ait->name() == std::string("DL_reference"))
            ipv4setup->SetDataLinkLayerReference(ait->as_string());
        else if (ait->name() == std::string("IP_address")) {
            std::string ipAddress = ait->as_string();
            ipv4setup->SetIPAddress(configParserServer.ConvertStringIpToUInt(ipAddress));
        } else if (ait->name() == std::string("multicastIPaddresses")) {
            std::string multicastAddresses = ait->as_string();
            for (auto &&ipAddress : configParserServer.SplitStringByComma(multicastAddresses)) {
                ipv4setup->GetMulticastIPAddress().push_back(configParserServer.ConvertStringIpToUInt(ipAddress));
            }
        } else if (ait->name() == std::string("subnet_mask"))
            ipv4setup->SetSubnetMask(configParserServer.ConvertStringIpToUInt(ait->as_string()));
        else if (ait->name() == std::string("gateway_IP_address"))
            ipv4setup->SetGatewayIPAddress(configParserServer.ConvertStringIpToUInt(ait->as_string()));
        else if (ait->name() == std::string("use_DHCP_flag"))
            ipv4setup->SetUseDHCP(ait->as_bool());
        else if (ait->name() == std::string("primary_DNS_address"))
            ipv4setup->SetPrimaryDNSAddress(configParserServer.ConvertStringIpToUInt(ait->as_string()));
        else if (ait->name() == std::string("secondary_DNS_address"))
            ipv4setup->SetSecondaryDNSAddress(configParserServer.ConvertStringIpToUInt(ait->as_string()));
        else if (ait->name() == std::string("description")){
            std::string text = ait->as_string();
            ipv4setup->SetDescription(text);
        } else
            throw ParseException("Attribute: " + std::string(ait->name()) + " of TcpUdpSetup object is not known!");
    }

    for (auto &&node : ipv4setupNode.children()) {
        if (node.empty())
            throw ParseException("IPv4 node contains empty element!");
        if (node.name() == std::string("IP_options")) {
            for (auto &&child : node.children()) {
                if (child.name() == std::string("IP_option"))
                    parseIpOptions(child, *ipv4setup);
                else
                    throw ParseException("Configuration node: " + std::string(node.name()) + " of IPv4 IP_options configuration is not known!");
            }
        } else
            throw ParseException("Configuration node: " + std::string(node.name()) + " of IPv4 options is not known!");
    }

    loadedUserConfiguration.getLoadedObjects().push_back(ipv4setup);
}

void ApplicationConfigParser::parseIpOptions(pugi::xml_node &ipOptionNode, CGXDLMSIp4Setup &ipv4setup) {
    CGXDLMSIp4SetupIpOption ipv4setupOption;
    for (auto ait = ipOptionNode.attributes_begin(); ait != ipOptionNode.attributes_end(); ++ait) {
        if (ait->empty())
            throw ParseException("Attribute: " + std::string(ait->name()) + " of IP option is empty!");
        else if (ait->name() == std::string("ipOptionType"))
            ipv4setupOption.SetType(configParserServer.GetIpOptionType(ait->as_string()));
        else if (ait->name() == std::string("ipOptionData")) {
            CGXByteBuffer buffer;
            buffer.AddString(ait->as_string());
            ipv4setupOption.SetData(buffer);
        } else
            throw ParseException("Attribute: " + std::string(ait->name()) + " of IP option is not known!");
    }
    ipv4setupOption.SetLength(ipv4setupOption.GetData().GetSize() + sizeof(ipv4setupOption.GetType()) + sizeof(ipv4setupOption.GetLength()));
    ipv4setup.GetIPOptions().push_back(ipv4setupOption);

}

void ApplicationConfigParser::parseMacAddressSetup(pugi::xml_node &macAddressSetupNode) {
    CGXDLMSMacAddressSetup *macAddressSetup = new CGXDLMSMacAddressSetup();
    for (auto ait = macAddressSetupNode.attributes_begin(); ait != macAddressSetupNode.attributes_end(); ++ait) {
        if (ait->empty())
            throw ParseException("Attribute: " + std::string(ait->name()) + " of MacAddressSetup object is empty!");
        else if (ait->name() == std::string("obis")) {
            CGXDLMSVariant v = ait->as_string();
            macAddressSetup->SetName(v);
        } else if (ait->name() == std::string("MAC_address"))
            macAddressSetup->SetMacAddress(ait->as_string());
        else if (ait->name() == std::string("description")) {
            std::string text = ait->as_string();
            macAddressSetup->SetDescription(text);
        } else
            throw ParseException("Attribute: " + std::string(ait->name()) + " of MacAddressSetup object is not known!");
    }
    loadedUserConfiguration.getLoadedObjects().push_back(macAddressSetup);
}

void ApplicationConfigParser::parseAssociationConfiguration(pugi::xml_node &associationNode) {
    CGXDLMSAssociationLogicalNameE *ln = new CGXDLMSAssociationLogicalNameE();

    parseAssociationAttributes(associationNode, *ln);
    for (auto &&opt : associationNode.children()) {
        if (opt.name() == std::string("ObjectList")) {
            parseObjectList(opt, *ln);
        } else if (opt.name() == std::string("AssociatedPartnersId")) {
            parsePartnersId(opt, *ln);
        } else if (opt.name() == std::string("ApplicationContextName")) {
            parseApplicationContextName(opt, *ln);
        } else if (opt.name() == std::string("XDlmsContextInfo")) {
            parseXDlmsContextInfo(opt, *ln);
        }else if (opt.name() == std::string("AuthenticationMechanismName")) {
            parseAuthenticationMechanismName(opt, *ln);
        }  else
            throw ParseException("Configuration node: " + std::string(opt.name()) + " of Association logical name is not known!");
    }
    loadedUserConfiguration.getLoadedObjects().push_back(ln);
}

void ApplicationConfigParser::parseAssociationAttributes(pugi::xml_node &associationNode, CGXDLMSAssociationLogicalNameE &ln) {
    for (auto ait = associationNode.attributes_begin(); ait != associationNode.attributes_end(); ++ait) {
        if (ait->empty())
            throw ParseException("Attribute: " + std::string(ait->name()) + "of Association logical name object is empty!");
        if (ait->name() == std::string("obis")) {
            CGXDLMSVariant v = ait->as_string();
            ln.SetName(v);
        } else if (ait->name() == std::string("secret")) {
            CGXByteBuffer buffer;
            buffer.AddString(ait->as_string());
            ln.SetSecret(buffer);
        } else if (ait->name() == std::string("securitySetupReference")) {
            std::string referenceObis = ait->as_string();
            CGXDLMSObject *o = loadedUserConfiguration.getLoadedObjects().FindByLN(DLMS_OBJECT_TYPE_SECURITY_SETUP, referenceObis);
            if (o == NULL)
                throw InitException("Security setup reference object with obis " + std::string(ait->as_string()) + " to association object " + ln.GetLogicalName() + " is not found!");
            else
                ln.SetSecuritySetupReference(ait->as_string());
        } else if (ait->name() == std::string("userList")) {
            continue;
        } else if (ait->name() == std::string("description")){
            std::string text = ait->as_string();
            ln.SetDescription(text);
        } else
            throw ParseException("Configuration attribute: " + std::string(ait->name()) + " of Association logical name is not known!");
    }
}

void ApplicationConfigParser::parseObjectList(pugi::xml_node &objectListNode, CGXDLMSAssociationLogicalNameE &ln) {

    for (auto &&object : objectListNode.children()) {
        std::map<unsigned int, DLMS_ACCESS_MODE> attributesAccess;
        std::map<unsigned int, DLMS_METHOD_ACCESS_MODE> methodsAccess;
        std::string associationLN;
        for (auto ait = object.attributes_begin(); ait != object.attributes_end(); ++ait) {
            if (ait->empty())
                throw ParseException("Attribute: " + std::string(ait->name()) + " is empty!");
            if (ait->name() == std::string("obis")) {
                associationLN = ait->as_string();
            } else if (ait->name() == std::string("description"))
                continue;
            else
                throw ParseException("Configuration attribute: " + std::string(ait->name()) + " of Association object list is not known!");
        }

        for (auto &&attribute : object.child("Attributes").children()) {
            unsigned int index;
            DLMS_ACCESS_MODE accessMode;
            for (auto ait = attribute.attributes_begin(); ait != attribute.attributes_end(); ++ait) {
                if (ait->empty())
                    throw ParseException("Attribute: " + std::string(ait->name()) + " is empty!");
                if (ait->name() == std::string("index")) {
                    index = configParserServer.GetNumberValue<unsigned int>(ait->as_string());
                } else if (ait->name() == std::string("accessMode")) {
                    accessMode = configParserServer.GetValueAccessMode(ait->as_string());
                } else if (ait->name() == std::string("description"))
                    continue;
                else
                    throw ParseException("Configuration attribute: " + std::string(ait->name()) + " of Attribute node is not known!");
            }
            attributesAccess.insert(std::pair<unsigned int, DLMS_ACCESS_MODE>(index, accessMode));
        }
        ln.getObjectsAttributeAccessRights().insert(std::pair<std::string, std::map<unsigned int, DLMS_ACCESS_MODE>>(associationLN, attributesAccess));

        for (auto &&method : object.child("Methods").children()) {
            unsigned int index;
            DLMS_METHOD_ACCESS_MODE accessMode;
            for (auto ait = method.attributes_begin(); ait != method.attributes_end(); ++ait) {
                if (ait->empty())
                    throw ParseException("Attribute: " + std::string(ait->name()) + " is empty!");
                if (ait->name() == std::string("index")) {
                    index = configParserServer.GetNumberValue<unsigned int>(ait->as_string());
                } else if (ait->name() == std::string("accessMode")) {
                    accessMode = configParserServer.GetMethodAccessMode(ait->as_string());
                } else if (ait->name() == std::string("description"))
                    continue;
                else
                    throw ParseException("Configuration attribute: " + std::string(ait->name()) + " of Method node is not known!");
            }
            methodsAccess.insert(std::pair<unsigned int, DLMS_METHOD_ACCESS_MODE>(index, accessMode));
        }
        ln.getObjectsMethodAccessRights().insert(std::pair<std::string, std::map<unsigned int, DLMS_METHOD_ACCESS_MODE>>(associationLN, methodsAccess));
    }
}

void ApplicationConfigParser::parsePartnersId(pugi::xml_node &partnersIdNode, CGXDLMSAssociationLogicalNameE &ln) {
    for (auto ait = partnersIdNode.attributes_begin(); ait != partnersIdNode.attributes_end(); ++ait) {
        if (ait->empty())
            throw ParseException("Attribute: " + std::string(ait->name()) + " of Association partners id is empty!");
        if (ait->name() == std::string("clientSAP")) {
            ln.SetClientSAP(configParserServer.GetNumberValue<unsigned short>(ait->as_string()));
        } else
            throw ParseException("Configuration attribute: " + std::string(ait->name()) + " of Association partners id is not known!");
    }
   ln.SetServerSAP(loadedUserConfiguration.getDlmsServerConfiguration().getLogicalAddress());

}

void ApplicationConfigParser::parseApplicationContextName(pugi::xml_node &applicationContextNameNode, CGXDLMSAssociationLogicalNameE &ln) {
    for (auto ait = applicationContextNameNode.attributes_begin(); ait != applicationContextNameNode.attributes_end(); ++ait) {
        if (ait->empty())
            throw ParseException("Attribute: " + std::string(ait->name()) + " of Association application context name is empty!");
        else if (ait->name() == std::string("jointIsoCtt"))
            ln.GetApplicationContextName().SetJointIsoCtt(configParserServer.GetNumberValue<unsigned int>(ait->as_string()));
        else if (ait->name() == std::string("country"))
            ln.GetApplicationContextName().SetCountry(configParserServer.GetNumberValue<unsigned short>(ait->as_string()));
        else if (ait->name() == std::string("countryName"))
            ln.GetApplicationContextName().SetCountryName(configParserServer.GetNumberValue<unsigned short>(ait->as_string()));
        else if (ait->name() == std::string("identifiedOrganization"))
            ln.GetApplicationContextName().SetIdentifiedOrganization(configParserServer.GetNumberValue<unsigned short>(ait->as_string()));
        else if (ait->name() == std::string("dlmsUA"))
            ln.GetApplicationContextName().SetDlmsUA(configParserServer.GetNumberValue<unsigned short>(ait->as_string()));
        else if (ait->name() == std::string("applicationContext"))
            ln.GetApplicationContextName().SetApplicationContext(configParserServer.GetNumberValue<unsigned short>(ait->as_string()));
        else if (ait->name() == std::string("contextId"))
            ln.GetApplicationContextName().SetContextId(configParserServer.GetContextId(ait->as_string()));
        else
            throw ParseException("Configuration attribute: " + std::string(ait->name()) + " of Association application context name is not known!");
    }
}

void ApplicationConfigParser::parseXDlmsContextInfo(pugi::xml_node &xDlmsContextInfoNode, CGXDLMSAssociationLogicalNameE &ln) {
    for (auto ait = xDlmsContextInfoNode.attributes_begin(); ait != xDlmsContextInfoNode.attributes_end(); ++ait) {
        if (ait->empty())
            throw ParseException("Attribute: " + std::string(ait->name()) + " of Association application xDlms context info is empty!");
        else if (ait->name() == std::string("conformance")) {
            std::string conformanceStr = ait->as_string();
            ln.GetXDLMSContextInfo().SetConformance(DLMS_CONFORMANCE_NONE);
            for (auto conformanceStr : configParserServer.SplitStringByComma(conformanceStr)) {
                ln.GetXDLMSContextInfo().SetConformance(
                (DLMS_CONFORMANCE)
                        (ln.GetXDLMSContextInfo().GetConformance() | configParserServer.GetConformance(conformanceStr))
                );
            }
        } else if (ait->name() == std::string("maxReceivePduSize"))
            ln.GetXDLMSContextInfo().SetMaxReceivePduSize(configParserServer.GetNumberValue<int>(ait->as_string()));
        else if (ait->name() == std::string("maxSendPduSize"))
            ln.GetXDLMSContextInfo().SetMaxSendPduSize(configParserServer.GetNumberValue<int>(ait->as_string()));
        else if (ait->name() == std::string("dlmsVersionNumber"))
            ln.GetXDLMSContextInfo().SetDlmsVersionNumber(configParserServer.GetNumberValue<int>(ait->as_string()));
       else if (ait->name() == std::string("qualityOfService"))
            ln.GetXDLMSContextInfo().SetQualityOfService(configParserServer.GetNumberValue<int>(ait->as_string()));
        else if (ait->name() == std::string("cypheringInfo")) {
            CGXByteBuffer buffer;
            buffer.AddString(ait->as_string());
            ln.GetXDLMSContextInfo().SetCypheringInfo(buffer);
        } else
            throw ParseException("Configuration attribute: " + std::string(ait->name()) + " of Association application xDlms context info is not known!");
    }
}


void ApplicationConfigParser::parseAuthenticationMechanismName(pugi::xml_node &autMechNameNode, CGXDLMSAssociationLogicalNameE &ln) {
    for (auto ait = autMechNameNode.attributes_begin(); ait != autMechNameNode.attributes_end(); ++ait) {
        if (ait->empty())
            throw ParseException("Attribute: " + std::string(ait->name()) + " of Association authentication mechanism name is empty!");
        else if (ait->name() == std::string("jointIsoCtt"))
            ln.GetAuthenticationMechanismName().SetJointIsoCtt(configParserServer.GetNumberValue<unsigned int>(ait->as_string()));
        else if (ait->name() == std::string("country"))
            ln.GetAuthenticationMechanismName().SetCountry(configParserServer.GetNumberValue<unsigned short>(ait->as_string()));
        else if (ait->name() == std::string("countryName"))
            ln.GetAuthenticationMechanismName().SetCountryName(configParserServer.GetNumberValue<unsigned short>(ait->as_string()));
        else if (ait->name() == std::string("identifiedOrganization"))
            ln.GetAuthenticationMechanismName().SetIdentifiedOrganization(configParserServer.GetNumberValue<unsigned short>(ait->as_string()));
        else if (ait->name() == std::string("dlmsUA"))
            ln.GetAuthenticationMechanismName().SetDlmsUA(configParserServer.GetNumberValue<unsigned short>(ait->as_string()));
        else if (ait->name() == std::string("authenticationMechanismName"))
            ln.GetAuthenticationMechanismName().SetAuthenticationMechanismName(configParserServer.GetNumberValue<int>(ait->as_string()));
        else if (ait->name() == std::string("mechanismId"))
            ln.GetAuthenticationMechanismName().SetMechanismId(configParserServer.GetAuthenticationMechanismType(ait->as_string()));
        else
            throw ParseException("Configuration attribute: " + std::string(ait->name()) + " of Association authentication mechanism name is not known!");
    }
}

void ApplicationConfigParser::parseSecuritySetup(pugi::xml_node &securitySetupNode) {
    CGXDLMSSecuritySetupE *securitySetup = new CGXDLMSSecuritySetupE();
    for (auto ait = securitySetupNode.attributes_begin(); ait != securitySetupNode.attributes_end(); ++ait) {
        if (ait->empty())
            throw ParseException("Attribute: " + std::string(ait->name()) + " of Security setup is empty!");
        else  if (ait->name() == std::string("obis")) {
            CGXDLMSVariant v = ait->as_string();
            securitySetup->SetName(v);
        } else if (ait->name() == std::string("version"))
            securitySetup->SetVersion(configParserServer.GetNumberValue<unsigned short>(ait->as_string()));
        else if (ait->name() == std::string("securityPolicy")) {
            std::string securityPolicyStr = ait->as_string();
            securitySetup->SetSecurityPolicy(DLMS_SECURITY_POLICY1_NOTHING);
            for (auto securityPolicyStr : configParserServer.SplitStringByComma(securityPolicyStr))
                securitySetup->SetSecurityPolicy((DLMS_SECURITY_POLICY1) (securitySetup->GetSecurityPolicy() | configParserServer.GetSecPolicy1Value(securityPolicyStr)));
        } else if (ait->name() == std::string("securitySuite"))
            securitySetup->SetSecuritySuite(configParserServer.GetSecuritySuite(ait->as_string()));
        else if (ait->name() == std::string("serverSystemTitle")) {
            CGXByteBuffer buffer;
            buffer.AddString(ait->as_string());
            securitySetup->SetServerSystemTitle(buffer);
        } else if (ait->name() == std::string("invocationCounterReference")) {
            std::string referenceObis = ait->as_string();
            CGXDLMSObject *o = loadedUserConfiguration.getLoadedObjects().FindByLN(DLMS_OBJECT_TYPE_DATA, referenceObis);
            if (o == NULL)
                throw InitException("Invocation counter reference object with obis " + std::string(ait->as_string()) +
                                    " to security setup object " + securitySetup->GetLogicalName() + " is not found!");
            else
                securitySetup->setInvocationCounterReference(ait->as_string());
        } else if (ait->name() == std::string("description")){
            std::string text = ait->as_string();
            securitySetup->SetDescription(text);
        } else
            throw ParseException("Configuration attribute: " + std::string(ait->name()) + " of Security setup is not known!");
    }

    for (auto &&node : securitySetupNode.children()) {
        if (node.empty())
            throw ParseException("Security setup node contains empty element!");
        if (node.name() == std::string("Certificates")) {
            for (auto &&certificate : node.children()) {
                if (certificate.name() == std::string("Certificate"))
                    parseCertificate(certificate, *securitySetup);
                else
                    throw ParseException("Configuration node: " + std::string(node.name()) + " of Security setup Certificates configuration is not known!");
            }
        } else
            throw ParseException("Configuration node: " + std::string(node.name()) + " of Security setup configuration is not known!");
    }
    loadedUserConfiguration.getLoadedObjects().push_back(securitySetup);
}

void ApplicationConfigParser::parseCertificate(pugi::xml_node certificateNode, CGXDLMSSecuritySetup &securitySetup) {
    CGXDLMSCertificateInfo *certificateInfo = new CGXDLMSCertificateInfo();
    for (auto ait = certificateNode.attributes_begin(); ait != certificateNode.attributes_end(); ++ait) {
        if (ait->empty())
            throw ParseException("Attribute: " + std::string(ait->name()) + " of Certificate setup is empty!");
        else if (ait->name() == std::string("entity"))
            certificateInfo->SetEntity(configParserServer.GetCertificateEntity(ait->as_string()));
        else if (ait->name() == std::string("type"))
            certificateInfo->SetType(configParserServer.GetCertificateType(ait->as_string()));
        else if (ait->name() == std::string("serialNumber")) {
            std::string serialNumber = ait->as_string();
            certificateInfo->SetSerialNumber(serialNumber);
        } else if (ait->name() == std::string("issuer")) {
            std::string issuer = ait->as_string();
            certificateInfo->SetIssuer(issuer);
        } else if (ait->name() == std::string("subject")) {
            std::string subject = ait->as_string();
            certificateInfo->SetSubject(subject);
        } else if (ait->name() == std::string("subjectAltName")) {
            std::string subjectAltName = ait->as_string();
            certificateInfo->SetSubjectAltName(subjectAltName);
        } else
            throw ParseException("Configuration attribute: " + std::string(ait->name()) + " of Certificate setup is not known!");
    }
    securitySetup.GetCertificates().push_back(certificateInfo);
}

ApplicationConfigParser::ApplicationConfigParser(const char *pathToApplicationConfigFile, LoadedUserConfiguration &loadedUserConfiguration) :
        pathToApplicationConfigFile(pathToApplicationConfigFile), loadedUserConfiguration(loadedUserConfiguration) {
}

void ApplicationConfigParser::LoadApplicationConfiguration() {
    pugi::xml_document doc;

    // load the XML file
    if (!doc.load_file(pathToApplicationConfigFile))
        throw ParseException(std::string("Path to obis object mapping xml file doesn't exist!"));
    pugi::xml_node rootNode = doc.child("Root");

    for (auto &&node : rootNode.children()) {
        if (node.name() == std::string("HatelConfiguration"))                   parseHatelConfiguration(node);
        else if (node.name() == std::string("DlmsServerPathsConfiguration"))    parseDlmsPathsConfiguration(node);
        else if (node.name() == std::string("DlmsServerConfiguration"))         parseDlmsServerConfiguration(node);
        else if (node.name() == std::string("Objects"))                         parseObjectsConfiguration(node);
        else
            throw ParseException("Configuration node: " + std::string(node.name()) + " is not known!");
    }





}