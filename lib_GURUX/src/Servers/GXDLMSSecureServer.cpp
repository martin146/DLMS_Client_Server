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

#include "GURUX_DLMS/Servers/GXDLMSSecureServer.h"

CGXDLMSSecureServer::CGXDLMSSecureServer(bool UseLogicalNameReferencing,
    DLMS_INTERFACE_TYPE intefaceType) :
    CGXDLMSServer(
        UseLogicalNameReferencing,
        intefaceType), m_Cipher("ABCDEFGH")
{
    m_Settings.SetCipher(&m_Cipher);
}

CGXDLMSSecureServer::CGXDLMSSecureServer(
        CGXDLMSIecHdlcSetup* hdlc) :
        CGXDLMSServer(
                hdlc), m_Cipher("ABCDEFGH")
{
    m_Settings.SetCipher(&m_Cipher);
}

CGXDLMSSecureServer::CGXDLMSSecureServer(
        CGXDLMSTcpUdpSetup* wrapper) :
        CGXDLMSServer(
                wrapper), m_Cipher("ABCDEFGH")
{
    m_Settings.SetCipher(&m_Cipher);
}

#ifndef DLMS_IGNORE_ASSOCIATION_LOGICAL_NAME
CGXDLMSSecureServer::CGXDLMSSecureServer(
    CGXDLMSAssociationLogicalName* ln,
    CGXDLMSIecHdlcSetup* hdlc) :
    CGXDLMSServer(
        ln,
        hdlc), m_Cipher("ABCDEFGH")
{
    m_Settings.SetCipher(&m_Cipher);
}

CGXDLMSSecureServer::CGXDLMSSecureServer(
    CGXDLMSAssociationLogicalName* ln,
    CGXDLMSTcpUdpSetup* wrapper) :
    CGXDLMSServer(
        ln,
        wrapper), m_Cipher("ABCDEFGH")
{
    m_Settings.SetCipher(&m_Cipher);
}
#endif //DLMS_IGNORE_ASSOCIATION_LOGICAL_NAME

#ifndef DLMS_IGNORE_ASSOCIATION_SHORT_NAME
CGXDLMSSecureServer::CGXDLMSSecureServer(
    CGXDLMSAssociationShortName* sn,
    CGXDLMSIecHdlcSetup* hdlc) :
    CGXDLMSServer(
        sn,
        hdlc), m_Cipher("ABCDEFGH")
{
    m_Settings.SetCipher(&m_Cipher);
}

CGXDLMSSecureServer::CGXDLMSSecureServer(
    CGXDLMSAssociationShortName* sn,
    CGXDLMSTcpUdpSetup* wrapper) :
    CGXDLMSServer(
        sn,
        wrapper), m_Cipher("ABCDEFGH")
{
    m_Settings.SetCipher(&m_Cipher);
}

#endif //DLMS_IGNORE_ASSOCIATION_SHORT_NAME

CGXDLMSSecureServer::~CGXDLMSSecureServer()
{
}

CGXCipher* CGXDLMSSecureServer::GetCiphering()
{
    return m_Settings.GetCipher();
}

CGXByteBuffer& CGXDLMSSecureServer::GetKek() {
    return GetSettings().GetKek();
}

int CGXDLMSSecureServer::SetKek(CGXByteBuffer& value) {
    GetSettings().SetKek(value);
    return 0;
}