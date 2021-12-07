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
static void export_collision_rules(scope);
static void export_collision_hash(scope);
static void export_collision_set(scope);
static void export_collision_map(scope);


//-----------------------------------------------------------------------------
// Declare the _entities._collisions module.
//-----------------------------------------------------------------------------
DECLARE_SP_SUBMODULE(_entities, _collisions)
{
	export_collision_manager(_collisions);
	export_collision_rules(_collisions);
	export_collision_hash(_collisions);
	export_collision_set(_collisions);
	export_collision_map(_collisions);
}


//-----------------------------------------------------------------------------
// Exports CCollisionManager.
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
// Exports ICollisionRules.
//-----------------------------------------------------------------------------
void export_collision_rules(scope _collisions)
{
	class_<ICollisionRules, boost::noncopyable> CollisionRules("CollisionRules", no_init);

	// Methods...
	CollisionRules.def(
		"should_hit_entity",
		&ICollisionRules::ShouldHitEntity,
		"Returns whether the given entities should collide with each other."
	);

	// AutoUnload...
	CollisionRules.def(
		"_unload_instance",
		&ICollisionRules::UnloadInstance,
		"Called when an instance is being unloaded."
	);
}


//-----------------------------------------------------------------------------
// Exports CCollisionHash.
//-----------------------------------------------------------------------------
void export_collision_hash(scope _collisions)
{
	class_<CCollisionHash, bases<ICollisionRules> > CollisionHash(
		"CollisionHash",
		"Collision rules where contained pairs never collide with each other."
	);

	// Methods...
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
		"remove_pairs",
		&CCollisionHash::RemovePairs,
		"Removes all pairs associated with the given entity."
	);

	CollisionHash.def(
		"has_pair",
		&CCollisionHash::HasPair,
		"Returns whether the given pair is in the hash."
	);

	CollisionHash.def(
		"get_count",
		&CCollisionHash::GetCount,
		"Returns the amount of pairs associated with the given entity."
	);

	CollisionHash.def(
		"get_pairs",
		&CCollisionHash::GetPairs,
		"Returns a list of all entities associated with the given entity."
	);

	// Special methods...
	CollisionHash.def(
		"__contains__",
		&CCollisionHash::Contains,
		"Returns whether the given entity is in the hash."
	);
}


//-----------------------------------------------------------------------------
// Exports CCollisionSet.
//-----------------------------------------------------------------------------
void export_collision_set(scope _collisions)
{
	class_<CCollisionSet, boost::shared_ptr<CCollisionSet>, bases<ICollisionRules> > CollisionSet(
		"CollisionSet",
		"Collision rules where contained elements never collide with anything."
	);

	// Methods...
	CollisionSet.def(
		"add",
		&CCollisionSet::Add,
		"Adds the given element to the set."
	);

	CollisionSet.def(
		"remove",
		&CCollisionSet::Remove,
		"Removes the given element from the set."
	);

	CollisionSet.def(
		"clear",
		&CCollisionSet::Clear,
		"Removes all elements from the set."
	);

	// Special methods...
	CollisionSet.def(
		"__bool__",
		&CCollisionSet::HasElements,
		"Returns whether the set is empty or not."
	);

	CollisionSet.def(
		"__contains__",
		&CCollisionSet::Contains,
		"Returns whether the given element is in the set or not."
	);

	CollisionSet.def(
		"__iter__",
		&CCollisionSet::Iterate,
		"Iterates over all elements contained in the set."
	);

	CollisionSet.def(
		"__len__",
		&CCollisionSet::GetSize,
		"Returns the amount of elements contained in the set."
	);
}


//-----------------------------------------------------------------------------
// Exports CCollisionMap.
//-----------------------------------------------------------------------------
void export_collision_map(scope _collisions)
{
	class_<CCollisionMap, bases<ICollisionRules> > CollisionMap(
		"CollisionMap",
		"Collision rules that overrides one-way collisions."
	);

	// Methods...
	CollisionMap.def(
		"clear",
		&CCollisionMap::Clear,
		"Removes all elements from the map."
	);

	// Special methods...
	CollisionMap.def(
		"__getitem__",
		&CCollisionMap::Find,
		"Returns the collision set associated with the given entity."
	);

	CollisionMap.def(
		"__delitem__",
		&CCollisionMap::Remove,
		"Removes the collision set associated with the given entity."
	);

	CollisionMap.def(
		"__bool__",
		&CCollisionMap::HasElements,
		"Returns whether the map is empty or not."
	);

	CollisionMap.def(
		"__contains__",
		&CCollisionMap::Contains,
		"Returns whether the given element is in the map or not."
	);

	CollisionMap.def(
		"__len__",
		&CCollisionMap::GetSize,
		"Returns the amount of elements contained in the map."
	);

	CollisionMap.def(
		"__iter__",
		&CCollisionMap::Iterate,
		"Iterates over all elements contained in the map."
	);
}
