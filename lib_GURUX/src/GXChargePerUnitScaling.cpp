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

#include "GURUX_DLMS/GXChargePerUnitScaling.h"
#include "GURUX_DLMS/GXHelpers.h"

 // Constructor.
CGXChargePerUnitScaling::CGXChargePerUnitScaling()
{
}

/**
 * Online help:<br>
 * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSCharge
 *
 * @return Commodity scale.
 */
signed char CGXChargePerUnitScaling::GetCommodityScale()
{
    return m_CommodityScale;
}

/**
 * Online help:<br>
 * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSCharge
 *
 * @param value
 *            Commodity scale.
 */
void CGXChargePerUnitScaling::SetCommodityScale(signed char value)
{
    m_CommodityScale = value;
}

/**
 * Online help:<br>
 * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSCharge
 *
 * @return Price scale.
 */
signed char CGXChargePerUnitScaling::GetPriceScale()
{
    return m_PriceScale;
}

/**
 * Online help:<br>
 * http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSCharge
 *
 * @param value
 *            Price scale.
 */
void CGXChargePerUnitScaling::SetPriceScale(signed char value)
{
    m_PriceScale = value;
}

std::string CGXChargePerUnitScaling::ToString()
{
    std::string str = GXHelpers::IntToString(m_CommodityScale);
    str.append(", ");
    str = GXHelpers::IntToString(m_PriceScale);
    return str;
}