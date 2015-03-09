/**
* =============================================================================
* Source Python
* Copyright (C) 2015 Source Python Development Team.  All rights reserved.
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

#ifndef _WEAPONS_SCRIPTS_ORANGEBOX_WRAP_PYTHON_H
#define _WEAPONS_SCRIPTS_ORANGEBOX_WRAP_PYTHON_H

//-----------------------------------------------------------------------------
// Includes.
//-----------------------------------------------------------------------------
#include ENGINE_BRANCH_INCLUDE_PATH(weapons_scripts_wrap_python.h)


//-----------------------------------------------------------------------------
// Expose FileWeaponInfo_t.
//-----------------------------------------------------------------------------
template<class T>
void export_engine_specific_base_weapon_info(T BaseWeaponInfo)
{
	// Nothing specific to OrangeBox...
}


//-----------------------------------------------------------------------------
// Expose WeaponInfo.
//-----------------------------------------------------------------------------
template<class T>
void export_weapon_info(T _scripts)
{
	class_<WeaponInfo, WeaponInfo *, bases<FileWeaponInfo_t>, boost::noncopyable> WeaponInfo_("WeaponInfo", no_init);

	// Game specific stuff...
	export_game_specific_weapon_info(WeaponInfo_);

	// Add memory tools...
	WeaponInfo_ ADD_MEM_TOOLS(WeaponInfo, "WeaponInfo");
}


#endif // _WEAPONS_SCRIPTS_ORANGEBOX_WRAP_PYTHON_H
