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
static void export_entity_collision_hash(scope);


//-----------------------------------------------------------------------------
// Declare the _entities._collisions module.
//-----------------------------------------------------------------------------
DECLARE_SP_SUBMODULE(_entities, _collisions)
{
	export_entity_collision_hash(_collisions);
}


//-----------------------------------------------------------------------------
// Exports CCollisionHash.
//-----------------------------------------------------------------------------
void export_entity_collision_hash(scope _collisions)
{
	class_<CCollisionHash> CollisionHash("CollisionHash");

	// Methods...
	CollisionHash.def(
		"add_pair",
		&CCollisionHash::AddPair,
		"Adds the given pair to the hash."
	);

	CollisionHash.def(
		"remove_pair",
		&CCollisionHash::RemovePair,
		"Removes the given pair from the hash."
	);

	CollisionHash.def(
		"remove_pairs",
		&CCollisionHash::RemovePairs,
		"Removes all pair associated with the given object."
	);

	CollisionHash.def(
		"has_pair",
		&CCollisionHash::HasPair,
		"Returns whether the given pairs is in the hash."
	);

	CollisionHash.def(
		"get_count",
		&CCollisionHash::GetCount,
		"Returns the amount of pairs associated with the given object."
	);

	CollisionHash.def(
		"get_pairs",
		&CCollisionHash::GetPairs,
		"Returns a list of all objects associated with the given object."
	);

	// Special methods...
	CollisionHash.def(
		"__contains__",
		&CCollisionHash::IsInHash,
		"Returns whether the given object is in the hash."
	);

	// AutoUnload...
	CollisionHash.def(
		"_unload_instance",
		&CCollisionHash::UnloadInstance,
		"Called when an instance is being unloaded."
	);
}
