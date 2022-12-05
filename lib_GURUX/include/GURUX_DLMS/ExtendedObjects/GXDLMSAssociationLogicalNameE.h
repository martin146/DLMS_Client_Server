/**
 * @file GXDLMSAssociationLogicalNameO.h
 * @author Filip Grepl
 * @brief Declaration of 'GXDLMSAssociationLogicalNameO' class
 * @version 1.0
 * @date 2021-10-24
 *
 * @copyright Copyright (c) 2021
 *
 */
#ifndef DLMS_CLIENT_SERVER_GXDLMSASSOCIATIONLOGICALNAMEE_H
#define DLMS_CLIENT_SERVER_GXDLMSASSOCIATIONLOGICALNAMEE_H

#include <GURUX_DLMS/Objects/AccessControlAndManagement/GXDLMSAssociationLogicalName.h>
#include <GURUX_DLMS/Objects/GXDLMSObject.h>

class CGXDLMSAssociationLogicalNameE : public CGXDLMSAssociationLogicalName {
private:
    std::map<std::string, std::map<unsigned int, DLMS_ACCESS_MODE>> objectsAttributeAccessRights;
    std::map<std::string, std::map<unsigned int, DLMS_METHOD_ACCESS_MODE>> objectsMethodAccessRights;

public:

protected:
    DLMS_ACCESS_MODE GetAttributeAccess(CGXDLMSServer *server, CGXDLMSValueEventArg &e) override;

    DLMS_METHOD_ACCESS_MODE GetMethodAccess(CGXDLMSServer *server, CGXDLMSValueEventArg &e) override;

public:

    std::map<std::string, std::map<unsigned int, DLMS_ACCESS_MODE>> &getObjectsAttributeAccessRights();

    std::map<std::string, std::map<unsigned int, DLMS_METHOD_ACCESS_MODE>> &getObjectsMethodAccessRights();


};

#endif //DLMS_CLIENT_SERVER_GXDLMSASSOCIATIONLOGICALNAMEE_H
