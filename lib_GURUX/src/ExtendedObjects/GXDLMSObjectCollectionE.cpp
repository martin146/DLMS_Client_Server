//
// Created by filip on 27.10.2021.
//

#include "GURUX_DLMS/ExtendedObjects/GXDLMSObjectCollectionE.h"

const std::string CGXDLMSObjectCollectionE::currentAssociationLn = "0.0.40.0.0.255";
const unsigned char CGXDLMSObjectCollectionE::currentAssociationLnArr[6] = {0,0,40,0,0,255};
const unsigned short CGXDLMSObjectCollectionE::currentAssociationSn = 0xFA00;

CGXDLMSObjectCollectionE::CGXDLMSObjectCollectionE() : tcpUdpSetup(NULL), iecHdlcSetup(NULL) {

}

std::string &CGXDLMSObjectCollectionE::getActAssociationLn() {
    return actAssociationLN;
}

void CGXDLMSObjectCollectionE::setActAssociationLn(const std::string &actAssociationLn) {
    actAssociationLN = actAssociationLn;
}

unsigned short CGXDLMSObjectCollectionE::getActAssociationSn() const {
    return actAssociationSN;
}

void CGXDLMSObjectCollectionE::setActAssociationSn(unsigned short actAssociationSn) {
    actAssociationSN = actAssociationSn;
}

CGXDLMSObject *CGXDLMSObjectCollectionE::FindByLN(DLMS_OBJECT_TYPE type, std::string &ln) {
    if (ln == CGXDLMSObjectCollectionE::currentAssociationLn)
        return CGXDLMSObjectCollection::FindByLN(type, this->actAssociationLN);
    else
        return CGXDLMSObjectCollection::FindByLN(type, ln);
}

CGXDLMSObject *CGXDLMSObjectCollectionE::FindByLN(DLMS_OBJECT_TYPE type, unsigned char *ln) {
    unsigned char actLN[6] = {0};
    if (memcmp(ln, currentAssociationLnArr, 6) == 0) {
        std::string octet;
        int octetIndex = 0;
        for (int index = 0; index < this->actAssociationLN.size(); index++) {
            if (this->actAssociationLN[index] != '.')
                octet.push_back(this->actAssociationLN[index]);
            if (this->actAssociationLN[index] == '.' || index == this->actAssociationLN.size()-1) {
                actLN[octetIndex] = std::stoi(octet);
                octetIndex++;
                octet.clear();
            }

        }
        return CGXDLMSObjectCollection::FindByLN(type, actLN);
    } else
        return CGXDLMSObjectCollection::FindByLN(type, ln);
}

CGXDLMSObject *CGXDLMSObjectCollectionE::FindBySN(unsigned short sn) {
    if (sn == currentAssociationSn)
        return CGXDLMSObjectCollection::FindBySN(this->actAssociationSN);
    else
        return CGXDLMSObjectCollection::FindBySN(sn);
}

CGXDLMSTcpUdpSetup *CGXDLMSObjectCollectionE::getTcpUdpSetup() const {
    return tcpUdpSetup;
}

void CGXDLMSObjectCollectionE::setTcpUdpSetup(CGXDLMSTcpUdpSetup *tcpUdpSetup) {
    CGXDLMSObjectCollectionE::tcpUdpSetup = tcpUdpSetup;
}

CGXDLMSIecHdlcSetup *CGXDLMSObjectCollectionE::getIecHdlcSetup() const {
    return iecHdlcSetup;
}

void CGXDLMSObjectCollectionE::setIecHdlcSetup(CGXDLMSIecHdlcSetup *iecHdlcSetup) {
    CGXDLMSObjectCollectionE::iecHdlcSetup = iecHdlcSetup;
}
