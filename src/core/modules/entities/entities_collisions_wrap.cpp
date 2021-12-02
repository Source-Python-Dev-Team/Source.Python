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
static void export_base_collision_hash(scope);
static void export_collision_hash(scope);


//-----------------------------------------------------------------------------
// Declare the _entities._collisions module.
//-----------------------------------------------------------------------------
DECLARE_SP_SUBMODULE(_entities, _collisions)
{
	export_collision_manager(_collisions);
	export_base_collision_hash(_collisions);
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

	// Singleton...
	_collisions.attr("collision_manager") = object(ptr(GetCollisionManager()));
}


//-----------------------------------------------------------------------------
// Exports ICollisionHash.
//-----------------------------------------------------------------------------
void export_base_collision_hash(scope _collisions)
{
	class_<ICollisionHash, boost::noncopyable> BaseCollisionHash("BaseCollisionHash", no_init);

	// Methods...
	BaseCollisionHash.def(
		"add_pair",
		&ICollisionHash::AddPair,
		"Adds the given entity pair to the hash."
	);

	BaseCollisionHash.def(
		"remove_pair",
		&ICollisionHash::RemovePair,
		"Removes the given pair from the hash."
	);

	BaseCollisionHash.def(
		"remove_pairs",
		&ICollisionHash::RemovePairs,
		"Removes all pairs associated with the given entity."
	);

	BaseCollisionHash.def(
		"has_pair",
		&ICollisionHash::HasPair,
		"Returns whether the given pair is in the hash."
	);

	BaseCollisionHash.def(
		"get_count",
		&ICollisionHash::GetCount,
		"Returns the amount of pairs associated with the given entity."
	);

	BaseCollisionHash.def(
		"get_pairs",
		&ICollisionHash::GetPairs,
		"Returns a list of all entities associated with the given entity."
	);

	// Special methods...
	BaseCollisionHash.def(
		"__contains__",
		&ICollisionHash::Contains,
		"Returns whether the given entity is in the hash."
	);

	// AutoUnload...
	BaseCollisionHash.def(
		"_unload_instance",
		&ICollisionHash::UnloadInstance,
		"Called when an instance is being unloaded."
	);
}


//-----------------------------------------------------------------------------
// Exports CCollisionHash.
//-----------------------------------------------------------------------------
void export_collision_hash(scope _collisions)
{
	class_<CCollisionHash, bases<ICollisionHash> >("CollisionHash");
}
