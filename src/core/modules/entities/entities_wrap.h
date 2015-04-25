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
// IServerUnknown extension class.
//-----------------------------------------------------------------------------
class IServerUnknownExt
{
public:
	static CPointer *GetBaseEntity( IServerUnknown *pServerUnknown )
	{
		return new CPointer((unsigned long)pServerUnknown->GetBaseEntity());
	}
	
};


//-----------------------------------------------------------------------------
// edict_t extension class.
//-----------------------------------------------------------------------------
class CEdictExt
{
public:
	static str GetKeyValueString(edict_t* pEdict, const char* szName)
	{
		char szResult[1024];
		CBaseEntity* pEntity = pEdict->GetUnknown()->GetBaseEntity();
		servertools->GetKeyValue(pEntity, szName, szResult, 1024);

		// Fix for field name "model". I think a string_t object is copied to szResult.
		if (strcmp(szName, "model") == 0)
			return *(char **) szResult;

		return str(szResult);
	}

	static int GetKeyValueInt(edict_t* pEdict, const char* szName)
	{
		return extract<int>(eval("lambda x: int(x)")(GetKeyValueString(pEdict, szName)));
	}

	static float GetKeyValueFloat(edict_t* pEdict, const char* szName)
	{
		return extract<float>(eval("lambda x: float(x)")(GetKeyValueString(pEdict, szName)));
	}

	static Vector GetKeyValueVector(edict_t* pEdict, const char* szName)
	{
		object vec = eval("lambda x: tuple(map(float, x.split(' ')))")(GetKeyValueString(pEdict, szName));
		return Vector(extract<float>(vec[0]), extract<float>(vec[1]), extract<float>(vec[2]));
	}

	static bool GetKeyValueBool(edict_t* pEdict, const char* szName)
	{
		return strcmp(extract<const char *>(GetKeyValueString(pEdict, szName)), "1") == 0;
	}

	static Color GetKeyValueColor(edict_t* pEdict, const char* szName)
	{
		object color = eval("lambda x: tuple(map(int, x.split(' ')))")(GetKeyValueString(pEdict, szName));
		return Color(extract<int>(color[0]), extract<int>(color[1]), extract<int>(color[2]), extract<int>(color[3]));
	}

	static void SetKeyValueColor(edict_t* pEdict, const char* szName, Color color)
	{
		char string[16];
		Q_snprintf(string, sizeof(string), "%i %i %i %i", color.r(), color.g(), color.b(), color.a());
		SetKeyValue(pEdict, szName, string);
	}

	template<class T>
	static void SetKeyValue(edict_t* pEdict, const char* szName, T value)
	{
		servertools->SetKeyValue(BaseEntityFromEdict(pEdict, true), szName, value);
	}
};


//-----------------------------------------------------------------------------
// If these aren't defined, we get linker errors about CBaseEdict.
//-----------------------------------------------------------------------------
IChangeInfoAccessor *CBaseEdict::GetChangeAccessor()
{
	return engine->GetChangeAccessor( (const edict_t *)this );
}

const IChangeInfoAccessor *CBaseEdict::GetChangeAccessor() const
{
	return engine->GetChangeAccessor( (const edict_t *)this );
}


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
		return IndexFromBaseHandle(m_hInflictor);
	}
	
	void set_inflictor(unsigned int uiInflictor)
	{
		m_hInflictor = BaseHandleFromIndex(uiInflictor, true);
	}
	
	int get_attacker()
	{
		return IndexFromBaseHandle(m_hAttacker);
	}
	
	void set_attacker(unsigned int uiAttacker)
	{
		m_hAttacker = BaseHandleFromIndex(uiAttacker, true);
	}
	
	int get_weapon()
	{
		return IndexFromBaseHandle(m_hWeapon);
	}
	
	void set_weapon(unsigned int uiWeapon)
	{
		m_hWeapon = BaseHandleFromIndex(uiWeapon, true);
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
