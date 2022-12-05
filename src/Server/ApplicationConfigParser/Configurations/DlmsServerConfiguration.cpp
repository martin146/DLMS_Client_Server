//
// Created by filip on 13.11.2021.
//

#include "ApplicationConfigParser/Configurations/DlmsServerConfiguration.h"

const std::string &DlmsServerConfiguration::getName() const {
    return name;
}

void DlmsServerConfiguration::setName(const std::string &name) {
    DlmsServerConfiguration::name = name;
}

unsigned long DlmsServerConfiguration::getLogicalAddress() const {
    return logicalAddress;
}

void DlmsServerConfiguration::setLogicalAddress(unsigned long logicalAddress) {
    DlmsServerConfiguration::logicalAddress = logicalAddress;
}

unsigned long DlmsServerConfiguration::getPhysicalAddress() const {
    return physicalAddress;
}

void DlmsServerConfiguration::setPhysicalAddress(unsigned long physicalAddress) {
    DlmsServerConfiguration::physicalAddress = physicalAddress;
}

unsigned int DlmsServerConfiguration::getAddressSize() const {
    return addressSize;
}

void DlmsServerConfiguration::setAddressSize(unsigned int addressSize) {
    DlmsServerConfiguration::addressSize = addressSize;
}

const DLMS_INTERFACE_TYPE &DlmsServerConfiguration::getInterfaceType() const {
    return interfaceType;
}

void DlmsServerConfiguration::setInterfaceType(const DLMS_INTERFACE_TYPE &interfaceType) {
    DlmsServerConfiguration::interfaceType = interfaceType;
}

bool DlmsServerConfiguration::isUseLogicalNameReferencing() const {
    return useLogicalNameReferencing;
}

void DlmsServerConfiguration::setUseLogicalNameReferencing(bool useLogicalNameReferencing) {
    DlmsServerConfiguration::useLogicalNameReferencing = useLogicalNameReferencing;
}

GX_TRACE_LEVEL DlmsServerConfiguration::getTraceLevel() const {
    return traceLevel;
}

void DlmsServerConfiguration::setTraceLevel(GX_TRACE_LEVEL traceLevel) {
    DlmsServerConfiguration::traceLevel = traceLevel;
}
