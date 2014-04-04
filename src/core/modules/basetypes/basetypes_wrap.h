/**
* =============================================================================
* Source Python
* Copyright (C) 2014 Source Python Development Team.  All rights reserved.
* =============================================================================
*
* This program is free software; you can redistribute it and/or modify it under
* the terms of the GNU General Public License, version 3.0, as published by the
* Free Software Foundation.
*
* This program is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
* FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
* details.
*
* You should have received a copy of the GNU General Public License along with
* this program.  If not, see <http://www.gnu.org/licenses/>.
*
* As a special exception, the Source Python Team gives you permission
* to link the code of this program (as well as its derivative works) to
* "Half-Life 2," the "Source Engine," and any Game MODs that run on software
* by the Valve Corporation.  You must obey the GNU General Public License in
* all respects for all other code used.  Additionally, the Source.Python
* Development Team grants this exception to all derivative works.
*/

#ifndef _BASETYPES_WRAP_H
#define _BASETYPES_WRAP_H

//-----------------------------------------------------------------------------
// Includes.
//-----------------------------------------------------------------------------
#include "dt_send.h"


//-----------------------------------------------------------------------------
// SendTable extension class.
//-----------------------------------------------------------------------------
class SendTableSharedExt
{
public:
	static SendProp *__getitem__(SendTable *pSendTable, int iIndex)
	{
		if (iIndex < 0 || iIndex > (pSendTable->m_nProps - 1))
		{
			BOOST_RAISE_EXCEPTION(PyExc_IndexError, "Invalid index.");
		}
		return pSendTable->GetProp(iIndex);
	}
};


//-----------------------------------------------------------------------------
// SendProp extension class.
//-----------------------------------------------------------------------------
class SendPropSharedExt
{
public:
	enum SendPropFlags
	{
		UNSIGNED = SPROP_UNSIGNED,
		COORD = SPROP_COORD,
		NO_SCALE = SPROP_NOSCALE,
		ROUND_DOWN = SPROP_ROUNDDOWN,
		ROUND_UP = SPROP_ROUNDUP,
		NORMAL = SPROP_NORMAL,
		EXCLUDE = SPROP_EXCLUDE,
		XYZE = SPROP_XYZE,
		INSIDE_ARRAY = SPROP_INSIDEARRAY,
		PROXY_ALWAYS_YES = SPROP_PROXY_ALWAYS_YES,
		IS_A_VECTOR_ELEM = SPROP_IS_A_VECTOR_ELEM,
		COLLAPSIBLE = SPROP_COLLAPSIBLE,
		COORD_MP = SPROP_COORD_MP,
		COORD_MP_LOW_PRECISION = SPROP_COORD_MP_LOWPRECISION,
		COORD_MP_INTEGRAL = SPROP_COORD_MP_INTEGRAL,
		CHANGE_OFTEN = SPROP_CHANGES_OFTEN
	};
};


//-----------------------------------------------------------------------------
// SendProp destructor declaration.
//-----------------------------------------------------------------------------
SendProp::~SendProp()
{
	// Declared here to fix undefined symbol issues...
}


#endif // _BASETYPES_WRAP_H
