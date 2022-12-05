/**
 * @file GXDLMSSecuritySetup.h
 * @author Filip Grepl
 * @brief Declaration of 'GXDLMSSecuritySetup' class
 * @version 1.0
 * @date 2021-11-14
 *
 * @copyright Copyright (c) 2021
 *
 */
#ifndef DLMS_CLIENT_SERVER_GXDLMSSECURITYSETUPE_H
#define DLMS_CLIENT_SERVER_GXDLMSSECURITYSETUPE_H

#include <GURUX_DLMS/Objects/AccessControlAndManagement/GXDLMSSecuritySetup.h>
#include <string>

class CGXDLMSSecuritySetupE : public CGXDLMSSecuritySetup {

private:
    std::string invocationCounterReference;
public:
    const std::string &getInvocationCounterReference() const;

    void setInvocationCounterReference(const std::string &invocationCounterReference);
};

#endif //DLMS_CLIENT_SERVER_GXDLMSSECURITYSETUPE_H
