//
// Created by filip on 23.10.2021.
//

#include "GURUX_DLMS/ExtendedObjects/GXDLMSRegisterE.h"

const std::string &CGXDLMSRegisterE::GetCid() const {
    return cid;
}

void CGXDLMSRegisterE::SetCid(const std::string &cid) {
    CGXDLMSRegisterE::cid = cid;
}
