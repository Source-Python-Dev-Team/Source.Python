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

//-----------------------------------------------------------------------------
// Includes.
//-----------------------------------------------------------------------------
#include "export_main.h"
#include "utilities/wrap_macros.h"
#include "modules/memory/memory_tools.h"

#include "weapons_entity.h"


//-----------------------------------------------------------------------------
// Forward declarations.
//-----------------------------------------------------------------------------
void export_weapon_entity(scope);


//-----------------------------------------------------------------------------
// Declare the _weapons._scripts module.
//-----------------------------------------------------------------------------
DECLARE_SP_SUBMODULE(_weapons, _entity)
{
	export_weapon_entity(_entity);
}


//-----------------------------------------------------------------------------
// Exports WeaponDataBase_t.
//-----------------------------------------------------------------------------
void export_weapon_entity(scope _entity)
{
	class_<WeaponMixin, bases<CBaseEntityWrapper>, boost::noncopyable> _WeaponMixin("WeaponMixin", no_init);

	_WeaponMixin.def("__init__",
		make_constructor(
			&WeaponMixin::__init__,
			default_call_policies(),
			args("entity_index")
		)
	);

	_WeaponMixin.def(
		"is_networked",
		&WeaponMixin::IsNetworked,
		"Return True if the entity is networked.\n\n"
		":rtype: bool"
	);

	_WeaponMixin.def(
		"is_player",
		&WeaponMixin::IsPlayer,
		"Return True if the entity is a player.\n\n"
		":rtype: bool"
	);

	_WeaponMixin.def(
		"is_weapon",
		&WeaponMixin::IsWeapon,
		"Return True if the entity is a weapon.\n\n"
		":rtype: bool"
	);

	_WeaponMixin.add_property(
		"next_attack",
		&WeaponMixin::GetNextAttack,
		&WeaponMixin::SetNextAttack,
		"Get/set the weapons's next primary fire attack time.\n\n"
		":rtype: float");

	_WeaponMixin.add_property(
		"next_secondary_fire_attack",
		&WeaponMixin::GetNextSecondaryFireAttack,
		&WeaponMixin::SetNextSecondaryFireAttack,
		"Get/set the weapons's next secondary fire attack time.\n\n"
		":rtype: float");

	_WeaponMixin.add_property(
		"ammoprop",
		&WeaponMixin::GetAmmoProp,
		&WeaponMixin::SetAmmoProp,
		"Get/set the weapons's primary fire ammo prop type.\n\n"
		":rtype: int");

	_WeaponMixin.add_property(
		"secondary_fire_ammoprop",
		&WeaponMixin::GetSecondaryFireAmmoProp,
		&WeaponMixin::SetSecondaryFireAmmoProp,
		"Get/set the weapons's secondary fire ammo prop type.\n\n"
		":rtype: int");

	_WeaponMixin.add_property(
		"owner_handle",
		&WeaponMixin::GetOwnerHandle,
		&WeaponMixin::SetOwnerHandle,
		"Get/set the weapons's owner handle.\n\n"
		":rtype: int");

	_WeaponMixin.add_property(
		"_clip",
		&WeaponMixin::GetClip,
		&WeaponMixin::SetClip,
		"Get/set the weapons's primary fire clip.\n\n"
		":rtype: int");

	_WeaponMixin.add_property(
		"_secondary_fire_clip",
		&WeaponMixin::GetSecondaryFireClip,
		&WeaponMixin::SetSecondaryFireClip,
		"Get/set the weapons's secondary fire clip.\n\n"
		":rtype: int");

	_WeaponMixin.add_property(
		"flip_view_model",
		&WeaponMixin::GetFlipViewModel,
		&WeaponMixin::SetFlipViewModel,
		"Get/set the weapons's flip view model.\n\n"
		":rtype: int");

	_WeaponMixin.add_property(
		"world_model_index",
		&WeaponMixin::GetWorldModelIndex,
		&WeaponMixin::SetWorldModelIndex,
		"Get/set the weapons's world model index.\n\n"
		":rtype: int");

	// CS:GO
	_WeaponMixin.add_property(
		"primary_ammo_count",
		&WeaponMixin::GetPrimaryAmmoCount,
		&WeaponMixin::SetPrimaryAmmoCount,
		"Get/set the weapons's primary ammo count.\n\n"
		".. note:: Only available in CS:GO.\n\n"
		":rtype: int");

	_WeaponMixin.add_property(
		"secondary_ammo_count",
		&WeaponMixin::GetSecondaryAmmoCount,
		&WeaponMixin::SetSecondaryAmmoCount,
		"Get/set the weapons's secondary ammo count.\n\n"
		".. note:: Only available in CS:GO.\n\n"
		":rtype: int");


	CLASSMETHOD(_WeaponMixin, GET_OBJ_NAME, &WeaponMixin::_obj)
	ADD_PTR(WeaponMixin)
	ADD_SIZE(WeaponMixin)
	STORE_CLASS(WeaponMixin, "WeaponMixin")
}
