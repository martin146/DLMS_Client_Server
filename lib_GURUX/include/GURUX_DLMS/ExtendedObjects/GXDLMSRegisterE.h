//
// Created by filip on 23.10.2021.
//

#ifndef DLMS_CLIENT_SERVER_GXDLMSREGISTERE_H
#define DLMS_CLIENT_SERVER_GXDLMSREGISTERE_H


#include <string>
#include <GURUX_DLMS/Objects/ParametersAndMeasurementData/GXDLMSRegister.h>

class CGXDLMSRegisterE : public CGXDLMSRegister {
private:
    std::string cid;

public:
    const std::string &GetCid() const;

    void SetCid(const std::string &cid);

};


#endif //DLMS_CLIENT_SERVER_GXDLMSREGISTERE_H
