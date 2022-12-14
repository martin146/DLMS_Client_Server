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

#ifndef GXDLMSACTIONSET_H
#define GXDLMSACTIONSET_H

#include "GURUX_DLMS/Objects/TimeAndEventBoundControl/GXDLMSActionItem.h"

class CGXDLMSActionSet
{
    CGXDLMSActionItem m_ActionUp;
    CGXDLMSActionItem m_ActionDown;
public:
    /**
     Constructor.
    */
    CGXDLMSActionSet();

    /**
     Destructor.
    */
    ~CGXDLMSActionSet()
    {
    }

    CGXDLMSActionItem& GetActionUp();

    CGXDLMSActionItem& GetActionDown();

    std::string ToString();
};

#endif //GXDLMSACTIONSET_H