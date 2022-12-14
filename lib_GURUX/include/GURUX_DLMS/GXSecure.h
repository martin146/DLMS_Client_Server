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

#ifndef GXSECURE_H
#define GXSECURE_H

#include "GURUX_DLMS/enums/enums.h"
#include "GXBytebuffer.h"
#include "GURUX_DLMS/CoreSettings/GXDLMSSettings.h"

class CGXSecure
{
public:
    /**
    * Generates challenge.
    *
    * @param authentication
    *            Used authentication.
    * @return Generated challenge.
    */
    static int GenerateChallenge(
        DLMS_AUTHENTICATION authentication,
        CGXByteBuffer& challenge);

    /**
    * Chipher text.
    *
    * @param auth
    *            Authentication level.
    * @param data
    *            Text to chipher.
    * @param secret
    *            Secret.
    * @return Chiphered text.
    */
    static int Secure(
        CGXDLMSSettings& settings,
        CGXCipher* cipher,
        unsigned long ic,
        CGXByteBuffer& data,
        CGXByteBuffer& secret,
        CGXByteBuffer& reply);

    static int EncryptAesKeyWrapping(CGXByteBuffer& data, CGXByteBuffer& kek, CGXByteBuffer& reply);
    static int DecryptAesKeyWrapping(CGXByteBuffer& data, CGXByteBuffer& kek, CGXByteBuffer& reply);
};

#endif //GXSECURE_H