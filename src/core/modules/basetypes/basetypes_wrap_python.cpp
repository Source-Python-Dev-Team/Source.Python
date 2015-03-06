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

//-----------------------------------------------------------------------------
// Includes.
//-----------------------------------------------------------------------------
#include "modules/export_main.h"
#include "utility/wrap_macros.h"
#include "tier0/basetypes.h"
#include "dt_common.h"
#include "dt_send.h"
#include "server_class.h"
#include "modules/memory/memory_tools.h"
#include "utility/sp_util.h"
#include "basetypes_wrap.h"

#include ENGINE_INCLUDE_PATH(basetypes_wrap_python.h)


//-----------------------------------------------------------------------------
// Forward declarations.
//-----------------------------------------------------------------------------
void export_take_damage_info();


//-----------------------------------------------------------------------------
// Declare the _basetypes module.
//-----------------------------------------------------------------------------
DECLARE_SP_MODULE(_basetypes)
{
	export_take_damage_info();
}


//-----------------------------------------------------------------------------
// Expose CTakeDamageInfo.
//-----------------------------------------------------------------------------
void export_take_damage_info()
{
	class_<CTakeDamageInfo, CTakeDamageInfo *> TakeDamageInfo("TakeDamageInfo");
	
	// Initializers...
	TakeDamageInfo.def("__init__", make_constructor(&TakeDamageInfoSharedExt::__init__));
	
	// Properties...
	TakeDamageInfo.add_property("force", &CTakeDamageInfo::GetDamageForce, &CTakeDamageInfo::SetDamageForce);
	TakeDamageInfo.add_property("position", &CTakeDamageInfo::GetDamagePosition, &CTakeDamageInfo::SetDamagePosition);
	
	TakeDamageInfo.add_property("reported_position", &CTakeDamageInfo::GetReportedPosition,
		&CTakeDamageInfo::SetReportedPosition
	);
	
	TakeDamageInfo.add_property("damage", &CTakeDamageInfo::GetDamage, &CTakeDamageInfo::SetDamage);
	TakeDamageInfo.add_property("base_damage", &CTakeDamageInfo::GetBaseDamage, &TakeDamageInfoSharedExt::set_base_damage);
	TakeDamageInfo.add_property("type", &CTakeDamageInfo::GetDamageType, &CTakeDamageInfo::SetDamageType);
	TakeDamageInfo.add_property("stats", &CTakeDamageInfo::GetDamageStats, &CTakeDamageInfo::SetDamageStats);
	TakeDamageInfo.add_property("ammo", &CTakeDamageInfo::GetAmmoType, &CTakeDamageInfo::SetAmmoType);
	
	TakeDamageInfo.add_property("damaged_other_players", &TakeDamageInfoSharedExt::get_damaged_other_players,
		&TakeDamageInfoSharedExt::set_damaged_other_players
	);

	TakeDamageInfo.add_property("inflictor", &TakeDamageInfoSharedExt::get_inflictor, &TakeDamageInfoSharedExt::set_inflictor);
	TakeDamageInfo.add_property("attacker", &TakeDamageInfoSharedExt::get_attacker, &TakeDamageInfoSharedExt::set_attacker);
	TakeDamageInfo.add_property("weapon", &TakeDamageInfoSharedExt::get_weapon, &TakeDamageInfoSharedExt::set_weapon);
	
	// CS:GO properties...
	TakeDamageInfo.NOT_IMPLEMENTED_ATTR("radius");
	TakeDamageInfo.NOT_IMPLEMENTED_ATTR("bullet");
	TakeDamageInfo.NOT_IMPLEMENTED_ATTR("recoil");
	
	// Engine specific stuff...
	export_engine_specific_take_damage_info(TakeDamageInfo);
	
	// Add memory tools...
	TakeDamageInfo ADD_MEM_TOOLS(CTakeDamageInfo, "TakeDamageInfo");
}
