/**
* =============================================================================
* Source Python
* Copyright (C) 2012 Source Python Development Team.  All rights reserved.
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
#ifndef _ENTITIES_WRAP_H
#define _ENTITIES_WRAP_H

//-----------------------------------------------------------------------------
// Includes.
//-----------------------------------------------------------------------------
#include "edict.h"
#include "server_class.h"
#include <cstdint>
#include "toolframework/itoolentity.h"
#include "modules/memory/memory_tools.h"
#include "string_t.h"
#include "dt_send.h"
#include "game/shared/ehandle.h"
#include "isaverestore.h"
#include "datamap.h"
#include "game/shared/takedamageinfo.h"
#include "utilities/conversions.h"


//-----------------------------------------------------------------------------
// External variables.
//-----------------------------------------------------------------------------
extern IServerTools* servertools;


//-----------------------------------------------------------------------------
// IServerUnknown extension
//-----------------------------------------------------------------------------
class IServerUnknownExt
{
public:
	static unsigned long		GetBaseEntity( IServerUnknown* unknown );
};

//-----------------------------------------------------------------------------
// edict_t extension
//-----------------------------------------------------------------------------
class CEdictExt
{
public:
	static const char* GetKeyValueString(edict_t* pEdict, const char* szName);
	static int         GetKeyValueInt(edict_t* pEdict, const char* szName);
	static float       GetKeyValueFloat(edict_t* pEdict, const char* szName);
	static Vector      GetKeyValueVector(edict_t* pEdict, const char* szName);
	static bool        GetKeyValueBool(edict_t* pEdict, const char* szName);
	static Color       GetKeyValueColor(edict_t* pEdict, const char* szName);

	static void        SetKeyValueColor(edict_t* pEdict, const char* szName, Color color);

	template<class T>
	static void        SetKeyValue(edict_t* pEdict, const char* szName, T value)
	{
		CBaseEntity* pEntity = pEdict->GetUnknown()->GetBaseEntity();
		servertools->SetKeyValue(pEntity, szName, value);
	}
};


//-----------------------------------------------------------------------------
// IServerNetworkable extension class.
//-----------------------------------------------------------------------------
class ServerNetworkableSharedExt
{
public:
	static CPointer *get_base_entity(IServerNetworkable *pNetworkable)
	{
		return new CPointer((unsigned long)pNetworkable->GetBaseEntity());
	}
};


//-----------------------------------------------------------------------------
// CTakeDamageInfo wrapper class.
//-----------------------------------------------------------------------------
class TakeDamageInfoBaseWrapper: public CTakeDamageInfo
{
public:
	int get_inflictor()
	{
		if (m_hInflictor.IsValid())
		{
			return m_hInflictor.GetEntryIndex();
		}
		return -1;
	}
	
	void set_inflictor(unsigned int iInflictor)
	{
		m_hInflictor = BaseHandleFromIndex(iInflictor);
	}
	
	int get_attacker()
	{
		if (m_hAttacker.IsValid())
		{
			return m_hAttacker.GetEntryIndex();
		}
		return -1;
	}
	
	void set_attacker(unsigned int iAttacker)
	{
		m_hAttacker = BaseHandleFromIndex(iAttacker);
	}
	
	int get_weapon()
	{
		if (m_hWeapon.IsValid())
		{
			return m_hWeapon.GetEntryIndex();
		}
		return -1;
	}
	
	void set_weapon(unsigned int iWeapon)
	{
		m_hWeapon = BaseHandleFromIndex(iWeapon);
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


//-----------------------------------------------------------------------------
// CTakeDamageInfo extension class.
//-----------------------------------------------------------------------------
class TakeDamageInfoSharedExt
{
public:
	static CTakeDamageInfo *__init__()
	{
		CTakeDamageInfo *pTakeDamageInfo = new CTakeDamageInfo();
		set_inflictor(pTakeDamageInfo, 0);
		set_attacker(pTakeDamageInfo, 0);
		return pTakeDamageInfo;
	}
	
	static int get_inflictor(CTakeDamageInfo *pTakeDamageInfo)
	{
		return ((TakeDamageInfoBaseWrapper *)pTakeDamageInfo)->get_inflictor();
	}
	
	static void set_inflictor(CTakeDamageInfo *pTakeDamageInfo, unsigned int iInflictor)
	{
		((TakeDamageInfoBaseWrapper *)pTakeDamageInfo)->set_inflictor(iInflictor);
	}
	
	static int get_attacker(CTakeDamageInfo *pTakeDamageInfo)
	{
		return ((TakeDamageInfoBaseWrapper *)pTakeDamageInfo)->get_attacker();
	}
	
	static void set_attacker(CTakeDamageInfo *pTakeDamageInfo, unsigned int iAttacker)
	{
		((TakeDamageInfoBaseWrapper *)pTakeDamageInfo)->set_attacker(iAttacker);
	}
	
	static int get_weapon(CTakeDamageInfo *pTakeDamageInfo)
	{
		return ((TakeDamageInfoBaseWrapper *)pTakeDamageInfo)->get_weapon();
	}
	
	static void set_weapon(CTakeDamageInfo *pTakeDamageInfo, unsigned int iWeapon)
	{
		((TakeDamageInfoBaseWrapper *)pTakeDamageInfo)->set_weapon(iWeapon);
	}
	
	static void set_base_damage(CTakeDamageInfo *pTakeDamageInfo, float flBaseDamage)
	{
		((TakeDamageInfoBaseWrapper *)pTakeDamageInfo)->set_base_damage(flBaseDamage);
	}
	
	static int get_damaged_other_players(CTakeDamageInfo *pTakeDamageInfo)
	{
		return ((TakeDamageInfoBaseWrapper *)pTakeDamageInfo)->get_damaged_other_players();
	}
	
	static void set_damaged_other_players(CTakeDamageInfo *pTakeDamageInfo, int iDamagedOtherPlayers)
	{
		((TakeDamageInfoBaseWrapper *)pTakeDamageInfo)->set_damaged_other_players(iDamagedOtherPlayers);
	}
};


#endif // _ENTITIES_WRAP_H
