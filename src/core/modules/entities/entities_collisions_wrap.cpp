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
void export_collision_manager(scope);
void export_collision_mode(scope);
void export_base_collision_rules(scope);
void export_collision_hash(scope);
void export_collision_set(scope);
void export_collision_map(scope);


//-----------------------------------------------------------------------------
// Declare the _entities._collisions module.
//-----------------------------------------------------------------------------
DECLARE_SP_SUBMODULE(_entities, _collisions)
{
	export_collision_manager(_collisions);
	export_collision_mode(_collisions);
	export_base_collision_rules(_collisions);
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
		"Registers a collision hook.\n"
		"\n"
		":param function callback:\n"
		"	Function to register as a collision hook callback.\n"
		"\n"
		":raises ValueError:\n"
		"	If the given callback is already registered.",
		args("self", "callback")
	);

	CollisionManager.def(
		"unregister_hook",
		&CCollisionManager::UnregisterCollisionHook,
		"Unregisters a collision hook.\n"
		"\n"
		":param function callback:\n"
		"	Function to unregister as a collision hook callback.\n"
		"\n"
		":raises ValueError:\n"
		"	If the given callback was not registered.",
		args("self", "callback")
	);

	// Properties...
	CollisionManager.add_property(
		"solid_masks",
		&CCollisionManager::GetSolidMasks,
		"Returns a list containing the masks that are currently considered solid.\n"
		"\n"
		":rtype:\n"
		"	list"
	);

	// Singleton...
	_collisions.attr("collision_manager") = object(ptr(GetCollisionManager()));

	// Add memory tools...
	CollisionManager ADD_MEM_TOOLS(CCollisionManager);
}


//-----------------------------------------------------------------------------
// Exports ECollisionMode.
//-----------------------------------------------------------------------------
void export_collision_mode(scope _collisions)
{
	enum_<ECollisionMode> CollisionMode("CollisionMode");

	// Values...
	CollisionMode.value("ALLOW", COLLISION_MODE_ALLOW);
	CollisionMode.value("PREVENT", COLLISION_MODE_PREVENT);
}


//-----------------------------------------------------------------------------
// Exports ICollisionRules.
//-----------------------------------------------------------------------------
void export_base_collision_rules(scope _collisions)
{
	class_<ICollisionRules, boost::noncopyable> BaseCollisionRules("BaseCollisionRules", no_init);

	// Properties...
	BaseCollisionRules.add_property(
		"mode",
		&ICollisionRules::GetMode,
		&ICollisionRules::SetMode,
		"Returns the collision mode for these rules.\n"
		"\n"
		":rtype:\n"
		"	CollisionMode"
	);

	BaseCollisionRules.add_property(
		"solid_only",
		&ICollisionRules::GetSolidOnly,
		&ICollisionRules::SetSolidOnly,
		"Returns whether these rules affect solid contents only.\n"
		"\n"
		":rtype:\n"
		"	bool"
	);

	// Methods...
	BaseCollisionRules.def(
		"should_collide",
		&ICollisionRules::ShouldCollide,
		"Returns whether the given entities should collide with each other.\n"
		"\n"
		":rtype:\n"
		"	bool",
		args("self", "entity", "other")
	);

	// AutoUnload...
	BaseCollisionRules.def(
		"_unload_instance",
		&ICollisionRules::UnloadInstance,
		"Called when an instance is being unloaded.",
		args("self")
	);

	// Add memory tools...
	BaseCollisionRules ADD_MEM_TOOLS(ICollisionRules);
}


//-----------------------------------------------------------------------------
// Exports CCollisionHash.
//-----------------------------------------------------------------------------
void export_collision_hash(scope _collisions)
{
	class_<CCollisionHash, boost::shared_ptr<CCollisionHash>, bases<ICollisionRules> > CollisionHash(
		"CollisionHash",
		"Collision rules where contained pairs never collide with each other.\n"
		"\n"
		"Example:\n"
		"\n"
		".. code:: python\n"
		"\n"
		"	from entities.collisions import CollisionHash\n"
		"	from events import Event\n"
		"	from players.entity import Player\n"
		"\n"
		"	h = CollisionHash()\n"
		"\n"
		"	@Event('player_say')\n"
		"	def player_say(game_event):\n"
		"	    player = Player.from_userid(game_event['userid'])\n"
		"	    entity = player.view_entity\n"
		"	    if entity is None:\n"
		"	        return\n"
		"	    # Toggle collisions with aimed entity\n"
		"	    if h.has_pair(player, entity):\n"
		"	        h.remove_pair(player, entity)\n"
		"	    else:\n"
		"	        h.add_pair(player, entity)",
		no_init
	);

	// Constructor...
	CollisionHash.def("__init__",
		make_constructor(
			&ICollisionRulesExt::Construct<CCollisionHash>,
			post_constructor_policies<default_call_policies>(
				make_function(
					&ICollisionRulesExt::Initialize<CCollisionHash>,
					default_call_policies(),
					args("self", "mode", "solid_only")
				)
			),
			("self", arg("mode")=COLLISION_MODE_PREVENT, arg("solid_only")=true)
		),
		"Constructs and initializes the collision hash.\n"
		"\n"
		":param CollisionMode mode:\n"
		"	The collision mode for these rules.\n"
		":param bool solid_only:\n"
		"	Whether these rules should affect solid contents only."
	);

	// Methods...
	CollisionHash.def(
		"add_pair",
		&CCollisionHash::AddPair,
		"Adds the given entity pair to the hash.\n"
		"\n"
		":raises ValueError:\n"
		"	If any of the given entities is not networked.",
		args("self", "entity", "other")
	);

	CollisionHash.def(
		"remove_pair",
		&CCollisionHash::RemovePair,
		"Removes the given pair from the hash.",
		args("self", "entity", "other")
	);

	CollisionHash.def(
		"remove_pairs",
		&CCollisionHash::RemovePairs,
		"Removes all pairs associated with the given entity.",
		args("self", "entity")
	);

	CollisionHash.def(
		"has_pair",
		&CCollisionHash::HasPair,
		"Returns whether the given pair is in the hash.\n"
		"\n"
		":rtype:\n"
		"	bool",
		args("self", "entity", "other")
	);

	CollisionHash.def(
		"get_count",
		&CCollisionHash::GetCount,
		"Returns the amount of pairs associated with the given entity.\n"
		"\n"
		":rtype:\n"
		"	int",
		args("self", "entity")
	);

	CollisionHash.def(
		"get_pairs",
		&CCollisionHash::GetPairs,
		"Returns a list of all entities associated with the given entity.\n"
		"\n"
		":rtype:\n"
		"	list",
		args("self", "entity")
	);

	CollisionHash.def(
		"clear",
		&CCollisionHash::Clear,
		"Removes all entities from the hash.",
		args("self")
	);

	// Special methods...
	CollisionHash.def(
		"__bool__",
		&CCollisionHash::HasElements,
		"Returns whether the hash is empty or not.\n"
		"\n"
		":rtype:\n"
		"	bool",
		args("self")
	);

	CollisionHash.def(
		"__contains__",
		&CCollisionHash::Contains,
		"Returns whether the given entity is in the hash.\n"
		"\n"
		":rtype:\n"
		"	bool",
		args("self", "entity")
	);

	CollisionHash.def(
		"__len__",
		&CCollisionHash::GetSize,
		"Returns the size of the collision hash.\n"
		"\n"
		":rtype:\n"
		"	int",
		args("self")
	);

	CollisionHash.def(
		"__iter__",
		&CCollisionHash::Iterate,
		"Iterates over all entities contained in the hash.\n"
		"\n"
		":rtype:\n"
		"	iterator",
		args("self")
	);

	// Add memory tools...
	CollisionHash ADD_MEM_TOOLS(CCollisionHash);
}


//-----------------------------------------------------------------------------
// Exports CCollisionSet.
//-----------------------------------------------------------------------------
void export_collision_set(scope _collisions)
{
	class_<CCollisionSet, boost::shared_ptr<CCollisionSet>, bases<ICollisionRules> > CollisionSet(
		"CollisionSet",
		"Collision rules where contained entities never collide with anything.\n"
		"\n"
		"Example:\n"
		"\n"
		".. code:: python\n"
		"\n"
		"	from entities.collisions import CollisionSet\n"
		"	from events import Event\n"
		"	from players.entity import Player\n"
		"\n"
		"	s = CollisionSet()\n"
		"\n"
		"	@Event('player_say')\n"
		"	def player_say(game_event):\n"
		"	    player = Player.from_userid(game_event['userid'])\n"
		"	    # Toggle collisions with everything\n"
		"	    if player in s:\n"
		"	        s.remove(player)\n"
		"	    else:\n"
		"	        s.add(player)"
		,
		no_init
	);

	// Constructor...
	CollisionSet.def("__init__",
		make_constructor(
			&ICollisionRulesExt::Construct<CCollisionSet>,
			post_constructor_policies<default_call_policies>(
				make_function(
					&ICollisionRulesExt::Initialize<CCollisionSet>,
					default_call_policies(),
					args("self", "mode", "solid_only")
				)
			),
			("self", arg("mode")=COLLISION_MODE_PREVENT, arg("solid_only")=true)
		),
		"Constructs and initializes the collision set.\n"
		"\n"
		":param CollisionMode mode:\n"
		"	The collision mode for these rules.\n"
		":param bool solid_only:\n"
		"	Whether these rules should affect solid contents only."
	);

	// Methods...
	CollisionSet.def(
		"add",
		&CCollisionSet::Add,
		"Adds the given entity to the set.\n"
		"\n"
		":raises ValueError:\n"
		"	If the given entity is not networked.",
		args("self", "entity")
	);

	CollisionSet.def(
		"remove",
		&CCollisionSet::Remove,
		"Removes the given entity from the set.",
		args("self", "entity")
	);

	CollisionSet.def(
		"clear",
		&CCollisionSet::Clear,
		"Removes all entities from the set.",
		args("self")
	);

	// Special methods...
	CollisionSet.def(
		"__bool__",
		&CCollisionSet::HasElements,
		"Returns whether the set is empty or not.\n"
		"\n"
		":rtype:\n"
		"	bool",
		args("self")
	);

	CollisionSet.def(
		"__contains__",
		&CCollisionSet::Contains,
		"Returns whether the given entity is in the set or not.\n"
		"\n"
		":rtype:\n"
		"	bool",
		args("self", "entity")
	);

	CollisionSet.def(
		"__iter__",
		&CCollisionSet::Iterate,
		"Iterates over all entities contained in the set.\n"
		"\n"
		":rtype:\n"
		"	iterator",
		args("self")
	);

	CollisionSet.def(
		"__len__",
		&CCollisionSet::GetSize,
		"Returns the amount of entities contained in the set.\n"
		"\n"
		":rtype:\n"
		"	int",
		args("self")
	);

	// Add memory tools...
	CollisionSet ADD_MEM_TOOLS(CCollisionSet);
}


//-----------------------------------------------------------------------------
// Exports CCollisionMap.
//-----------------------------------------------------------------------------
void export_collision_map(scope _collisions)
{
	class_<CCollisionMap, boost::shared_ptr<CCollisionMap>, bases<ICollisionRules> > CollisionMap(
		"CollisionMap",
		"Collision rules that overrides one-way collisions.\n"
		"\n"
		"Example:\n"
		"\n"
		".. code:: python\n"
		"\n"
		"	from entities.collisions import CollisionMap\n"
		"	from events import Event\n"
		"	from players.entity import Player\n"
		"\n"
		"	m = CollisionMap()\n"
		"\n"
		"	@Event('player_say')\n"
		"	def player_say(game_event):\n"
		"	    player = Player.from_userid(game_event['userid'])\n"
		"	    entity = player.view_entity\n"
		"	    if entity is None:\n"
		"	        return\n"
		"	    # Toggle one-way collisions with aimed entity\n"
		"	    s = m[player]\n"
		"	    if entity in s:\n"
		"	        s.remove(entity)\n"
		"	    else:\n"
		"	        s.add(entity)",
		no_init
	);

	// Constructor...
	CollisionMap.def("__init__",
		make_constructor(
			&ICollisionRulesExt::Construct<CCollisionMap>,
			post_constructor_policies<default_call_policies>(
				make_function(
					&ICollisionRulesExt::Initialize<CCollisionMap>,
					default_call_policies(),
					args("self", "mode", "solid_only")
				)
			),
			("self", args("mode")=COLLISION_MODE_PREVENT, arg("solid_only")=true)
		),
		"Constructs and initializes the collision map.\n"
		"\n"
		":param CollisionMode mode:\n"
		"	The collision mode for these rules.\n"
		":param bool solid_only:\n"
		"	Whether these rules should affect solid contents only."
	);

	// Methods...
	CollisionMap.def(
		"clear",
		&CCollisionMap::Clear,
		"Removes all entities from the map.",
		args("self")
	);

	// Special methods...
	CollisionMap.def(
		"__getitem__",
		&CCollisionMap::Find,
		"Returns the collision set associated with the given entity.\n"
		"\n"
		":rtype:\n"
		"	CollisionSet",
		args("self", "entity")
	);

	CollisionMap.def(
		"__delitem__",
		&CCollisionMap::Remove,
		"Removes the collision set associated with the given entity.",
		args("self", "entity")
	);

	CollisionMap.def(
		"__bool__",
		&CCollisionMap::HasElements,
		"Returns whether the map is empty or not.\n"
		"\n"
		":rtype:\n"
		"	bool",
		args("self")
	);

	CollisionMap.def(
		"__contains__",
		&CCollisionMap::Contains,
		"Returns whether the given entity is in the map or not.\n"
		"\n"
		":rtype:\n"
		"	bool",
		args("self", "entity")
	);

	CollisionMap.def(
		"__len__",
		&CCollisionMap::GetSize,
		"Returns the amount of entities contained in the map.\n"
		"\n"
		":rtype:\n"
		"	int",
		args("self")
	);

	CollisionMap.def(
		"__iter__",
		&CCollisionMap::Iterate,
		"Iterates over all entities contained in the map.\n"
		"\n"
		":rtype:\n"
		"	iterator",
		args("self")
	);

	// Add memory tools...
	CollisionMap ADD_MEM_TOOLS(CCollisionMap);
}
