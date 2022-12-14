//
// --------------------------------------------------------------------------
//  Gurux Ltd
//
//
//
// Filename:        $HeadURL$
//
// Version:         $Revision$,
//                  $Date$
//                  $Author$
//
// Copyright (c) Gurux Ltd
//
//---------------------------------------------------------------------------
//
//  DESCRIPTION
//
// This file is a part of Gurux ServerConfiguration Framework.
//
// Gurux ServerConfiguration Framework is Open Source software; you can redistribute it
// and/or modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; version 2 of the License.
// Gurux ServerConfiguration Framework is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// See the GNU General Public License for more details.
//
// More information of Gurux products: http://www.gurux.org
//
// This code is licensed under the GNU General Public License v2.
// Full text may be retrieved at http://www.gnu.org/licenses/gpl-2.0.txt
//---------------------------------------------------------------------------

#ifndef GXDLMSSECURESERVER_H
#define GXDLMSSECURESERVER_H

#include "GXDLMSServer.h"

class CGXDLMSSecureServer : public CGXDLMSServer
{
private:
    CGXCipher m_Cipher;
public:
    /////////////////////////////////////////////////////////////////////////////
    //Constructor
    /////////////////////////////////////////////////////////////////////////////
    CGXDLMSSecureServer(
        bool UseLogicalNameReferencing = true,
        DLMS_INTERFACE_TYPE intefaceType = DLMS_INTERFACE_TYPE_HDLC);

#ifndef DLMS_IGNORE_ASSOCIATION_LOGICAL_NAME
    /**
    * Constructor.
    *
    * @param ln
    *           Logical name settings..
    * @param hdlc
    *            HDLC settings.
    */
    CGXDLMSSecureServer(
        CGXDLMSAssociationLogicalName* ln, CGXDLMSIecHdlcSetup* hdlc);

    /**
    * Constructor.
    *
    * @param ln
    *           Logical name settings..
    * @param wrapper
    *            WRAPPER settings.
    */
    CGXDLMSSecureServer(
        CGXDLMSAssociationLogicalName* ln, CGXDLMSTcpUdpSetup* wrapper);

#endif //DLMS_IGNORE_ASSOCIATION_LOGICAL_NAME

    /**
    * Constructor.
    *
    * @param hdlc
    *            HDLC settings.
    */
    CGXDLMSSecureServer(
            CGXDLMSIecHdlcSetup* hdlc);

    /**
    * Constructor.
    *
    * @param wrapper
    *            WRAPPER settings.
    */
    CGXDLMSSecureServer(
            CGXDLMSTcpUdpSetup* wrapper);

#ifndef DLMS_IGNORE_ASSOCIATION_SHORT_NAME
    /**
    * Constructor.
    *
    * @param sn
    *           Short name settings..
    * @param hdlc
    *            HDLC settings.
    */
    CGXDLMSSecureServer(
        CGXDLMSAssociationShortName* sn, CGXDLMSIecHdlcSetup* hdlc);

    /**
    * Constructor.
    *
    * @param sn
    *           Short name settings..
    * @param wrapper
    *            WRAPPER settings.
    */
    CGXDLMSSecureServer(
        CGXDLMSAssociationShortName* sn, CGXDLMSTcpUdpSetup* wrapper);

#endif //DLMS_IGNORE_ASSOCIATION_SHORT_NAME
    /////////////////////////////////////////////////////////////////////////////
    //Destructor.
    /////////////////////////////////////////////////////////////////////////////
    ~CGXDLMSSecureServer();

    /**
     * @return Ciphering settings.
     */
    CGXCipher* GetCiphering();


    /////////////////////////////////////////////////////////////////////////////
    // Returns Key Encrypting Key, also known as Master key.
    /////////////////////////////////////////////////////////////////////////////
    CGXByteBuffer& GetKek();

    /////////////////////////////////////////////////////////////////////////////
    // value: Key Encrypting Key, also known as Master key.
    /////////////////////////////////////////////////////////////////////////////
    int SetKek(CGXByteBuffer& value);
};
#endif //GXDLMSSECURESERVER_H
