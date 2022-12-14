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

#ifndef GXDLMSSPECIALDAY_H
#define GXDLMSSPECIALDAY_H

#include "GURUX_DLMS/GXDate.h"

class CGXDLMSSpecialDay
{
    int m_Index;
    CGXDate m_Date;
    int m_DayId;

public:
    int GetIndex();
    void SetIndex(int value);

    CGXDate& GetDate();
    void SetDate(CGXDate& value);
    void SetDate(CGXDateTime& value);

    int GetDayId();
    void SetDayId(int value);

    std::string ToString();
};
#endif //GXDLMSSPECIALDAY_H