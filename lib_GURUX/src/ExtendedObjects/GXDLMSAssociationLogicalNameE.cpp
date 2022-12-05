//
// Created by filip on 24.10.2021.
//

#include <GURUX_DLMS/ExtendedObjects/GXDLMSAssociationLogicalNameE.h>

std::map<std::string, std::map<unsigned int, DLMS_ACCESS_MODE>> &
CGXDLMSAssociationLogicalNameE::getObjectsAttributeAccessRights() {
    return objectsAttributeAccessRights;
}

std::map<std::string, std::map<unsigned int, DLMS_METHOD_ACCESS_MODE>> &
CGXDLMSAssociationLogicalNameE::getObjectsMethodAccessRights() {
    return objectsMethodAccessRights;
}

DLMS_ACCESS_MODE CGXDLMSAssociationLogicalNameE::GetAttributeAccess(CGXDLMSServer *server, CGXDLMSValueEventArg &e) {
   try {
        return this->objectsAttributeAccessRights.at(e.GetTarget()->GetLogicalName()).at(e.GetIndex());
    } catch (const std::out_of_range &err) {
        // if attribute access for association is not found, the default value is returned
        return CGXDLMSAssociationLogicalName::GetAttributeAccess(server, e);
    }

}

DLMS_METHOD_ACCESS_MODE CGXDLMSAssociationLogicalNameE::GetMethodAccess(CGXDLMSServer *server, CGXDLMSValueEventArg &e) {
    try {
        return this->objectsMethodAccessRights.at(e.GetTarget()->GetLogicalName()).at(e.GetIndex());
    } catch (const std::out_of_range &err) {
        return CGXDLMSAssociationLogicalName::GetMethodAccess(server, e);
    }
}
