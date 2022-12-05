/**
 * @file DlmsServerConfiguration.h
 * @author Filip Grepl
 * @brief Declaration of 'DlmsServerConfiguration' class
 * @version 1.0
 * @date 2021-11-02
 *
 * @copyright Copyright (c) 2021
 *
 */
#ifndef DLMS_CLIENT_SERVER_DLMSSERVERCONFIGURATION_H
#define DLMS_CLIENT_SERVER_DLMSSERVERCONFIGURATION_H

#include <GURUX_DLMS/enums/enums.h>
#include <string>

class DlmsServerConfiguration {

    std::string name;
    unsigned long logicalAddress;
    unsigned long physicalAddress;
    unsigned int addressSize;
    DLMS_INTERFACE_TYPE interfaceType;
    bool useLogicalNameReferencing;
    GX_TRACE_LEVEL traceLevel;

public:

    const std::string &getName() const;

    void setName(const std::string &name);

    unsigned long getLogicalAddress() const;

    void setLogicalAddress(unsigned long logicalAddress);

    unsigned long getPhysicalAddress() const;

    void setPhysicalAddress(unsigned long physicalAddress);

    unsigned int getAddressSize() const;

    void setAddressSize(unsigned int addressSize);

    const DLMS_INTERFACE_TYPE &getInterfaceType() const;

    void setInterfaceType(const DLMS_INTERFACE_TYPE &interfaceType);

    bool isUseLogicalNameReferencing() const;

    void setUseLogicalNameReferencing(bool useLogicalNameReferencing);

    GX_TRACE_LEVEL getTraceLevel() const;

    void setTraceLevel(GX_TRACE_LEVEL traceLevel);

};

#endif //DLMS_CLIENT_SERVER_DLMSSERVERCONFIGURATION_H
