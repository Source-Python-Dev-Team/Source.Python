/**
* =============================================================================
* Source Python
* Copyright (C) 2012-2021 Source Python Development Team.  All rights reserved.
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
// Source.Python
#include "export_main.h"
#include "modules/entities/entities_collisions.h"


//-----------------------------------------------------------------------------
// Forward declarations.
//-----------------------------------------------------------------------------
static void export_collision_manager(scope);
static void export_collision_table(scope);
static void export_collision_hash(scope);


//-----------------------------------------------------------------------------
// Declare the _entities._collisions module.
//-----------------------------------------------------------------------------
DECLARE_SP_SUBMODULE(_entities, _collisions)
{
	export_collision_manager(_collisions);
	export_collision_table(_collisions);
	export_collision_hash(_collisions);
}


//-----------------------------------------------------------------------------
// Exports ICollisionHash.
//-----------------------------------------------------------------------------
void export_collision_manager(scope _collisions)
{
	class_<CCollisionManager, boost::noncopyable> CollisionManager("CollisionManager", no_init);

	// Methods...
	CollisionManager.def(
		"register_hook",
		&CCollisionManager::RegisterCollisionHook,
		"Registers a collision hook."
	);

	CollisionManager.def(
		"unregister_hook",
		&CCollisionManager::UnregisterCollisionHook,
		"Unregisters a collision hook."
	);

	// Properties...
	CollisionManager.add_property(
		"solid_masks",
		&CCollisionManager::GetSolidMasks,
		"Returns a list containing the masks that are currently considered solid."
	);

	// Singleton...
	_collisions.attr("collision_manager") = object(ptr(GetCollisionManager()));
}


//-----------------------------------------------------------------------------
// Exports CCollisionTable.
//-----------------------------------------------------------------------------
void export_collision_table(scope _collisions)
{
	class_<CCollisionTable, boost::shared_ptr<CCollisionTable>, boost::noncopyable> CollisionTable("CollisionTable", no_init);

	// Methods...
	CollisionTable.def(
		"disable",
		&CCollisionTable::Disable,
		"Disables collision with the given entity."
	);

	CollisionTable.def(
		"reset",
		&CCollisionTable::Reset,
		"Resets collision with the given entity."
	);

	CollisionTable.def(
		"is_disabled",
		&CCollisionTable::IsDisabled,
		"Returns whether collision with the given entity is disabled."
	);

	CollisionTable.def(
		"disable_all",
		&CCollisionTable::DisableAll,
		"Disables collision with all entities."
	);

	CollisionTable.def(
		"clear",
		&CCollisionTable::ClearAll,
		"Resets collision with all entities."
	);

	CollisionTable.def(
		"get_disabled",
		&CCollisionTable::GetDisabled,
		"Returns a list of entities with collision disabled."
	);

	CollisionTable.add_property(
		"count",
		&CCollisionTable::Count,
		"Returns the amount of entities with collision disabled."
	);

	CollisionTable.def_readonly(
		"activator",
		&CCollisionTable::m_oActivator,
		"Returns the activator entity that will disable collision."
	);

	// Special methods...
	CollisionTable.def(
		"__getitem__",
		&CCollisionTable::__getitem__,
		"Returns whether collision is disabled by an entity index."
	);

	CollisionTable.def(
		"__setitem__",
		&CCollisionTable::__setitem__,
		"Set collision by entity index."
	);

	CollisionTable.def(
		"__contains__",
		&CCollisionTable::IsDisabled,
		"Returns whether collision with the given entity is disabled."
	);
}


//-----------------------------------------------------------------------------
// Exports CCollisionHash.
//-----------------------------------------------------------------------------
void export_collision_hash(scope _collisions)
{
	class_<CCollisionHash, boost::noncopyable> CollisionHash("CollisionHash");

	// Methods...
	CollisionHash.def(
		"get_collision_table",
		GET_METHOD(boost::shared_ptr<CCollisionTable>, CCollisionHash, GetCollisionTable, CBaseEntity *),
		"Returns the CollisionTable for the given activator entity."
	);

	CollisionHash.def(
		"disable_from",
		&CCollisionHash::DisableFrom,
		"Disables collision of the given entity from the activator entity."
	);

	CollisionHash.def(
		"reset_from",
		&CCollisionHash::ResetFrom,
		"Resets collision of the given entity from the activator entity."
	);

	CollisionHash.def(
		"is_disabled_from",
		&CCollisionHash::IsDisabledFrom,
		"Returns whether collision of the given entity is disabled from the activator entity."
	);

	CollisionHash.def(
		"disable",
		&CCollisionHash::Disable,
		"Disables collision of the given entity in all activator entities."
	);

	CollisionHash.def(
		"reset",
		&CCollisionHash::Reset,
		"Resets collision of the given entity in all activator entities."
	);

	CollisionHash.def(
		"is_disabled",
		&CCollisionHash::IsDisabled,
		"Returns whether collision of the given entity has been disabled for any activator entity."
	);

	CollisionHash.def(
		"get_disabled_activator",
		&CCollisionHash::GetDisabledActivator,
		"Returns a list of activator entities with collision disabled for the given entity."
	);

	CollisionHash.def(
		"add_pair",
		&CCollisionHash::AddPair,
		"Adds the given entity pair to the hash."
	);

	CollisionHash.def(
		"remove_pair",
		&CCollisionHash::RemovePair,
		"Removes the given pair from the hash."
	);

	CollisionHash.def(
		"has_pair",
		&CCollisionHash::HasPair,
		"Returns whether the given pair is in the hash."
	);

	CollisionHash.def(
		"remove_pairs",
		&CCollisionHash::RemovePairs,
		"Removes all pairs associated with the given entity."
	);

	CollisionHash.def(
		"count_pairs",
		&CCollisionHash::CountPairs,
		"Returns the amount of pairs associated with the given entity."
	);

	CollisionHash.def(
		"get_pairs",
		&CCollisionHash::GetPairs,
		"Returns a list of all entities associated with the given entity."
	);

	CollisionHash.def(
		"clear",
		&CCollisionHash::Clear,
		"Resets collision across all entities in all activator entities."
	);

	// Special methods...
	CollisionHash.def(
		"__getitem__",
		GET_METHOD(boost::shared_ptr<CCollisionTable>, CCollisionHash, GetCollisionTable, unsigned int),
		"Returns the CollisionTable for the given activator entity index."
	);

	CollisionHash.def(
		"__contains__",
		&CCollisionHash::IsDisabled,
		"Returns whether collision of the given entity has been disabled for any activator entity."
	);

	// AutoUnload...
	CollisionHash.def(
		"_unload_instance",
		&CCollisionHash::UnloadInstance,
		"Called when an instance is being unloaded."
	);
}
