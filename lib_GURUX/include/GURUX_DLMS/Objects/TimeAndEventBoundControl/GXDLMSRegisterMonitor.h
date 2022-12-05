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

#ifndef GXDLMSREGISTERMONITOR_H
#define GXDLMSREGISTERMONITOR_H

#include "GURUX_DLMS/GXIgnore.h"
#ifndef DLMS_IGNORE_REGISTER_MONITOR
#include "GURUX_DLMS/GXDLMSMonitoredValue.h"
#include "GXDLMSActionSet.h"

/**
Online help:
http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSRegisterMonitor
*/
class CGXDLMSRegisterMonitor : public CGXDLMSObject
{
    std::vector<CGXDLMSActionSet*> m_Actions;
    CGXDLMSMonitoredValue m_MonitoredValue;
    std::vector<CGXDLMSVariant> m_Thresholds;

public:
    /**
     Constructor.
    */
    CGXDLMSRegisterMonitor();

    /**
     destructor.
    */
    virtual ~CGXDLMSRegisterMonitor();

    /**
     Constructor.

     @param ln Logical Name of the object.
    */
    CGXDLMSRegisterMonitor(std::string ln);

    /**
     Constructor.

     @param ln Logical Name of the object.
     @param sn Short Name of the object.
    */
    CGXDLMSRegisterMonitor(std::string ln, unsigned short sn);

    std::vector<CGXDLMSVariant>& GetThresholds();

    void SetThresholds(std::vector<CGXDLMSVariant>& value);


    CGXDLMSMonitoredValue& GetMonitoredValue();

    void SetMonitoredValue(CGXDLMSMonitoredValue& value);

    std::vector<CGXDLMSActionSet*>& GetActions();

    // Returns amount of attributes.
    int GetAttributeCount();

    // Returns amount of methods.
    int GetMethodCount();

    //Get attribute values of object.
    void GetValues(std::vector<std::string>& values);

    /////////////////////////////////////////////////////////////////////////
    // Returns collection of attributes to read.
    //
    // If attribute is static and already read or device is returned
    // HW error it is not returned.
    //
    // all: All items are returned even if they are read already.
    // attributes: Collection of attributes to read.
    void GetAttributeIndexToRead(bool all, std::vector<int>& attributes);

    int GetDataType(int index, DLMS_DATA_TYPE& type);

    /*
     * Returns value of given attribute.
     */
    int GetValue(CGXDLMSSettings& settings, CGXDLMSValueEventArg& e);

    /*
     * Set value of given attribute.
     */
    int SetValue(CGXDLMSSettings& settings, CGXDLMSValueEventArg& e);
};
#endif //DLMS_IGNORE_REGISTER_MONITOR

#endif //GXDLMSREGISTERMONITOR_H
