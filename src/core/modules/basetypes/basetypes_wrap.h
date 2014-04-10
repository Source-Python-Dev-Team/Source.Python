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
#include "game/shared/ehandle.h"
#include "isaverestore.h"
#include "datamap.h"
#include "game/shared/takedamageinfo.h"
#include "modules/memory/memory_tools.h"
#include "modules/conversions/conversions_wrap.h"


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


//-----------------------------------------------------------------------------
// CTakeDamageInfo extension class.
//-----------------------------------------------------------------------------
class TakeDamageInfoSharedExt: public CTakeDamageInfo
{
public:
	CPointer *get_inflictor()
	{
		return PointerFromIndex(m_hInflictor.GetEntryIndex());
	}
	
	void set_inflictor(CPointer *pInflictor)
	{
		m_hInflictor = (CBaseEntity *)pInflictor->m_ulAddr;
	}
	
	CPointer *get_attacker()
	{
		return PointerFromIndex(m_hAttacker.GetEntryIndex());
	}
	
	void set_attacker(CPointer *pAttacker)
	{
		m_hAttacker = (CBaseEntity *)pAttacker->m_ulAddr;
	}
	
	CPointer *get_weapon()
	{
		return PointerFromIndex(m_hWeapon.GetEntryIndex());
	}
	
	void set_weapon(CPointer *pWeapon)
	{
		m_hWeapon = (CBaseEntity *)pWeapon->m_ulAddr;
	}
	
	void set_base_damage(float flBaseDamage)
	{
		m_flBaseDamage = flBaseDamage;
	}
	
	int get_damaged_other_players()
	{
		return m_iDamagedOtherPlayers;
	}
	
	void set_damaged_other_players(int iDamagedOtherPlayers)
	{
		m_iDamagedOtherPlayers = iDamagedOtherPlayers;
	}
};


#endif // _BASETYPES_WRAP_H
