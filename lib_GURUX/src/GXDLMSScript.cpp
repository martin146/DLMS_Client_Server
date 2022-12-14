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

#include "GURUX_DLMS/GXDLMSScript.h"

CGXDLMSScript::CGXDLMSScript()
{
}

CGXDLMSScript::~CGXDLMSScript()
{
    for (std::vector<CGXDLMSScriptAction*>::iterator it = m_Actions.begin(); it != m_Actions.end(); ++it)
    {
        delete *it;
    }
    m_Actions.clear();
}

int CGXDLMSScript::GetID()
{
    return m_ID;
}

void CGXDLMSScript::SetID(int value)
{
    m_ID = value;
}

std::vector<CGXDLMSScriptAction*>& CGXDLMSScript::GetActions()
{
    return m_Actions;
}