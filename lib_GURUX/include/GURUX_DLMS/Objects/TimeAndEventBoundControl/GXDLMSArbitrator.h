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

#ifndef GXDLMSArbitrator_H
#define GXDLMSArbitrator_H

#include "GURUX_DLMS/GXIgnore.h"
#ifndef DLMS_IGNORE_ARBITRATOR
#include "GURUX_DLMS/Objects/GXDLMSObject.h"
#include "GXDLMSActionItem.h"

/**
Online help:
http://www.gurux.fi/Gurux.DLMS.Objects.GXDLMSArbitrator
*/
class CGXDLMSArbitrator : public CGXDLMSObject
{
    /**
    * Requested actions.
    */
    std::vector<CGXDLMSActionItem> m_Actions;

    /**
     * Permissions for each actor to request actions.
     */
    std::vector<std::string> m_PermissionsTable;
    /**
     * Weight allocated for each actor and to each possible action of that
     * actor.
     */
    std::vector< std::vector<uint16_t> > m_WeightingsTable;
    /**
     * The most recent requests of each actor.
     */
    std::vector<std::string> m_MostRecentRequestsTable;
    /**
     * The number identifies a bit in the Actions.
     */
    unsigned char m_LastOutcome;
public:
    //Constructor.
    CGXDLMSArbitrator();

    //SN Constructor.
    CGXDLMSArbitrator(std::string ln, unsigned short sn);

    //LN Constructor.
    CGXDLMSArbitrator(std::string ln);

    /**
     * Requested actions.
     */
    std::vector<CGXDLMSActionItem>& GetActions()
    {
        return m_Actions;
    }

    /**
     * Permissions for each actor to request actions.
     */
    std::vector<std::string>& GetPermissionsTable()
    {
        return m_PermissionsTable;
    }

    /**
     * Weight allocated for each actor and to each possible action of
     *         that actor.
     */
    std::vector< std::vector<uint16_t> >& GetWeightingsTable()
    {
        return m_WeightingsTable;
    }

    /**
     * The most recent requests of each actor.
     */
    std::vector<std::string>& GetMostRecentRequestsTable()
    {
        return m_MostRecentRequestsTable;
    }

    /**
     * The number identifies a bit in the Actions.
     */
    unsigned char GetLastOutcome()
    {
        return m_LastOutcome;
    }
    void SetLastOutcome(unsigned char value)
    {
        m_LastOutcome = value;
    }

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

    // Returns value of given attribute.
    int GetValue(CGXDLMSSettings& settings, CGXDLMSValueEventArg& e);

    // Set value of given attribute.
    int SetValue(CGXDLMSSettings& settings, CGXDLMSValueEventArg& e);
};
#endif //DLMS_IGNORE_ARBITRATOR
#endif //GXDLMSArbitrator_H