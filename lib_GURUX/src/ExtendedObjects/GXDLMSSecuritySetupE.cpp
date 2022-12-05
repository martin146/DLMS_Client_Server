//
// Created by filip on 14.11.2021.
//

#include <GURUX_DLMS/ExtendedObjects/GXDLMSSecuritySetupE.h>

const std::string &CGXDLMSSecuritySetupE::getInvocationCounterReference() const {
    return invocationCounterReference;
}

void CGXDLMSSecuritySetupE::setInvocationCounterReference(const std::string &invocationCounterReference) {
    CGXDLMSSecuritySetupE::invocationCounterReference = invocationCounterReference;
}