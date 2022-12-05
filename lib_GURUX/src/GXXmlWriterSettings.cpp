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

#include "GURUX_DLMS/GXXmlWriterSettings.h"

CGXXmlWriterSettings::CGXXmlWriterSettings()
{
    m_Values = true;
    m_SkipDefaults = true;
}

bool CGXXmlWriterSettings::GetValues()
{
    return m_Values;
}

void CGXXmlWriterSettings::SetValues(bool value)
{
    m_Values = value;
}

//Are default values serialized.
bool CGXXmlWriterSettings::GetSkipDefaults()
{
    return m_SkipDefaults;
}

//Are default values serialized.
void CGXXmlWriterSettings::SetSkipDefaults(bool value)
{
    m_SkipDefaults = value;
}