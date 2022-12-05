/**
 * @file GXDLMSObjectCollection.h
 * @author Filip Grepl
 * @brief Declaration of 'GXDLMSObjectCollection' class
 * @version 1.0
 * @date 2021-10-27
 *
 * @copyright Copyright (c) 2021
 *
 */
#ifndef DLMS_CLIENT_SERVER_GXDLMSOBJECTCOLLECTIONE_H
#define DLMS_CLIENT_SERVER_GXDLMSOBJECTCOLLECTIONE_H

#include <GURUX_DLMS/GXDLMSObjectCollection.h>
#include <GURUX_DLMS/Objects/SettingUpDataExchangeInternet/GXDLMSTcpUdpSetup.h>
#include <GURUX_DLMS/Objects/SettingUpDataExchangeLocalPortsModems/GXDLMSHdlcSetup.h>

class CGXDLMSObjectCollectionE : public CGXDLMSObjectCollection {

private:
    std::string actAssociationLN;
    unsigned short actAssociationSN;
    CGXDLMSTcpUdpSetup *tcpUdpSetup;
    CGXDLMSIecHdlcSetup *iecHdlcSetup;
public:

    static const std::string currentAssociationLn;
    static const unsigned char currentAssociationLnArr[6];
    static const unsigned short currentAssociationSn;

    CGXDLMSObjectCollectionE();

    CGXDLMSObject *FindByLN(DLMS_OBJECT_TYPE type, std::string &ln) override;

    CGXDLMSObject *FindByLN(DLMS_OBJECT_TYPE type, unsigned char *ln) override;

    CGXDLMSObject *FindBySN(unsigned short sn) override;

    std::string &getActAssociationLn();

    void setActAssociationLn(const std::string &actAssociationLn);

    unsigned short getActAssociationSn() const;

    void setActAssociationSn(unsigned short actAssociationSn);

    CGXDLMSTcpUdpSetup *getTcpUdpSetup() const;

    void setTcpUdpSetup(CGXDLMSTcpUdpSetup *tcpUdpSetup);

    CGXDLMSIecHdlcSetup *getIecHdlcSetup() const;

    void setIecHdlcSetup(CGXDLMSIecHdlcSetup *iecHdlcSetup);
};

#endif //DLMS_CLIENT_SERVER_GXDLMSOBJECTCOLLECTIONE_H
