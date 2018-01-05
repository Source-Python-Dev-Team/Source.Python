/**
* =============================================================================
* Source Python
* Copyright (C) 2012-2015 Source Python Development Team.  All rights reserved.
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
#ifndef _ENTITIES_H
#define _ENTITIES_H

//-----------------------------------------------------------------------------
// Includes.
//-----------------------------------------------------------------------------
// Source.Python
#include "modules/memory/memory_tools.h"
#include "utilities/conversions.h"
#include "entities_entity.h"

// SDK
#include "edict.h"
#include "server_class.h"
#include <cstdint>
#include "string_t.h"
#include "dt_send.h"
#include "game/shared/ehandle.h"
#include "isaverestore.h"
#include "datamap.h"
#include "game/shared/takedamageinfo.h"


//-----------------------------------------------------------------------------
// IServerEntity extension class.
//-----------------------------------------------------------------------------
class ServerEntityExt
{
public:
	static void SetModelName(IServerEntity *pServerEntity, const char *szModelName);
	static MDLHandle_t get_model_handle(IServerEntity *pServerEntity);
	static studiohdr_t* get_model_header(IServerEntity *pServerEntity);
	static int lookup_attachment(IServerEntity* pEntity, const char* name);
	static int lookup_bone(IServerEntity* pEntity, const char* name);
};


//-----------------------------------------------------------------------------
// CBaseEdict extension class.
//-----------------------------------------------------------------------------
class _BaseEdictExt
{
public:
	static void set_free(CBaseEdict *pEdict, bool bState)
	{
		if (bState)
			pEdict->SetFree();
		else
			pEdict->ClearFree();
	}
};


//-----------------------------------------------------------------------------
// CTakeDamageInfo wrapper class.
//-----------------------------------------------------------------------------
class TakeDamageInfoBaseWrapper: public CTakeDamageInfo
{
public:
	unsigned int get_inflictor()
	{
		return ExcIndexFromBaseHandle(m_hInflictor);
	}
	
	void set_inflictor(unsigned int uiInflictor)
	{
		m_hInflictor = ExcBaseHandleFromIndex(uiInflictor);
	}
	
	unsigned int get_attacker()
	{
		return ExcIndexFromBaseHandle(m_hAttacker);
	}
	
	void set_attacker(unsigned int uiAttacker)
	{
		m_hAttacker = ExcBaseHandleFromIndex(uiAttacker);
	}
	
	unsigned int get_weapon()
	{
		unsigned int index;
		if (IndexFromBaseHandle(m_hWeapon, index))
			return index;

		if (m_hAttacker.ToInt() == m_hInflictor.ToInt())
		{
			CBaseEntity* _attacker;
			if (!BaseEntityFromBaseHandle(m_hAttacker, _attacker))
				BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Failed to retrieve the attacker")

			CBaseEntityWrapper* attacker = (CBaseEntityWrapper*) _attacker;
			if (!attacker->IsPlayer())
				BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Attacker is not a player.");

			// TODO: Don't hardcode m_hActiveWeapon
			return ExcIndexFromIntHandle(attacker->GetDatamapProperty<unsigned int>("m_hActiveWeapon"));
		}
		return get_inflictor();
	}
	
	void set_weapon(unsigned int uiWeapon)
	{
		m_hWeapon = ExcBaseHandleFromIndex(uiWeapon);
	}
	
	void set_base_damage(float flBaseDamage)
	{
		m_flBaseDamage = flBaseDamage;
	}
	
	int get_damaged_other_players()
	{
		return -1;
		// return m_iDamagedOtherPlayers;
	}
	
	void set_damaged_other_players(int iDamagedOtherPlayers)
	{
		// m_iDamagedOtherPlayers = iDamagedOtherPlayers;
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
	
	static unsigned int get_inflictor(CTakeDamageInfo *pTakeDamageInfo)
	{
		return ((TakeDamageInfoBaseWrapper *)pTakeDamageInfo)->get_inflictor();
	}
	
	static void set_inflictor(CTakeDamageInfo *pTakeDamageInfo, unsigned int iInflictor)
	{
		((TakeDamageInfoBaseWrapper *)pTakeDamageInfo)->set_inflictor(iInflictor);
	}
	
	static unsigned int get_attacker(CTakeDamageInfo *pTakeDamageInfo)
	{
		return ((TakeDamageInfoBaseWrapper *)pTakeDamageInfo)->get_attacker();
	}
	
	static void set_attacker(CTakeDamageInfo *pTakeDamageInfo, unsigned int iAttacker)
	{
		((TakeDamageInfoBaseWrapper *)pTakeDamageInfo)->set_attacker(iAttacker);
	}
	
	static unsigned int get_weapon(CTakeDamageInfo *pTakeDamageInfo)
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


#endif // _ENTITIES_H
