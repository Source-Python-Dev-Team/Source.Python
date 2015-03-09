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

#ifndef _WEAPONS_SCRIPTS_CSGO_WRAP_H
#define _WEAPONS_SCRIPTS_CSGO_WRAP_H

//-----------------------------------------------------------------------------
// Includes.
//-----------------------------------------------------------------------------
#include "tier1/KeyValues.h"
#include "game/shared/weapon_parse.h"


//-----------------------------------------------------------------------------
// FileWeaponInfo_t extension class.
//-----------------------------------------------------------------------------
class BaseWeaponInfoExt
{
public:
	static const char *get_addon(FileWeaponInfo_t *pWeaponInfo)
	{
		return (const char *)pWeaponInfo->szAIAddOn;
	}

	static void set_addon(FileWeaponInfo_t *pWeaponInfo, const char *szAIAddOn)
	{
		strncpy(pWeaponInfo->szAIAddOn, szAIAddOn, MAX_WEAPON_STRING);
	}
};


#endif // _WEAPONS_SCRIPTS_CSGO_WRAP_H
