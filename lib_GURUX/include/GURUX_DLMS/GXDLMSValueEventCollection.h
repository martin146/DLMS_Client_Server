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
// and/or modify it under the terms of the GNU General License
// as published by the Free Software Foundation; version 2 of the License.
// Gurux ServerConfiguration Framework is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// See the GNU General License for more details.
//
// More information of Gurux products: http://www.gurux.org
//
// This code is licensed under the GNU General License v2.
// Full text may be retrieved at http://www.gnu.org/licenses/gpl-2.0.txt
//---------------------------------------------------------------------------

#ifndef GXDLMSVALUEEVENT_COLLECTION_H
#define GXDLMSVALUEEVENT_COLLECTION_H

#include "GXDLMSValueEventArg.h"
#include <algorithm>

class CGXDLMSValueEventCollection : public std::vector<CGXDLMSValueEventArg*>
{
public:
    /**
    * Destructor.
    */
    ~CGXDLMSValueEventCollection()
    {
        std::vector<CGXDLMSValueEventArg*> freed;
        for(std::vector<CGXDLMSValueEventArg*>::iterator it = begin(); it != end(); ++it)
        {
            if (std::find(begin(), end(), *it) == end()) {
                delete *it;
                freed.push_back(*it);
            }
        }
        clear();
    }
};
#endif //GXDLMSVALUEEVENT_COLLECTION_H
