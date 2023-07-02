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
#include "modules/entities/entities_transmit.h"


//-----------------------------------------------------------------------------
// Forward declarations.
//-----------------------------------------------------------------------------
void export_transmit_manager(scope);
void export_transmit_mode(scope);
void export_base_transmit_rules(scope);
void export_transmit_hash(scope);
void export_transmit_set(scope);
void export_transmit_map(scope);
void export_transmit_states(scope);


//-----------------------------------------------------------------------------
// Declare the _entities._transmit module.
//-----------------------------------------------------------------------------
DECLARE_SP_SUBMODULE(_entities, _transmit)
{
	export_transmit_manager(_transmit);
	export_transmit_mode(_transmit);
	export_base_transmit_rules(_transmit);
	export_transmit_hash(_transmit);
	export_transmit_set(_transmit);
	export_transmit_map(_transmit);
	export_transmit_states(_transmit);
}


//-----------------------------------------------------------------------------
// Exports CTransmitManager.
//-----------------------------------------------------------------------------
void export_transmit_manager(scope _transmit)
{
	class_<CTransmitManager, boost::noncopyable> TransmitManager("TransmitManager", no_init);

	// Methods...
	TransmitManager.def(
		"register_hook",
		&CTransmitManager::RegisterTransmitHook,
		"Registers a transmit hook.\n"
		"\n"
		":param function callback:\n"
		"	Function to register as a transmit hook callback.\n"
		"\n"
		":raises ValueError:\n"
		"	If the given callback is already registered.",
		args("self", "callback")
	);

	TransmitManager.def(
		"unregister_hook",
		&CTransmitManager::UnregisterTransmitHook,
		"Unregisters a transmit hook.\n"
		"\n"
		":param function callback:\n"
		"	Function to unregister as a transmit hook callback.\n"
		"\n"
		":raises ValueError:\n"
		"	If the given callback was not registered.",
		args("self", "callback")
	);

	// Singleton...
	_transmit.attr("transmit_manager") = object(ptr(GetTransmitManager()));

	// Add memory tools...
	TransmitManager ADD_MEM_TOOLS(CTransmitManager);
}


//-----------------------------------------------------------------------------
// Exports ETransmitMode.
//-----------------------------------------------------------------------------
void export_transmit_mode(scope _transmit)
{
	enum_<ETransmitMode> TransmitMode("TransmitMode");

	// Values...
	TransmitMode.value("ALLOW", TRANSMIT_MODE_ALLOW);
	TransmitMode.value("PREVENT", TRANSMIT_MODE_PREVENT);
}


//-----------------------------------------------------------------------------
// Exports CTransmitStates.
//-----------------------------------------------------------------------------
void export_transmit_states(scope _transmit)
{
	class_<CTransmitStates> TransmitStates("TransmitStates");

	// Special methods...
	TransmitStates.def("__delitem__", GET_METHOD(void, CTransmitStates, Clear, int));
	TransmitStates.def("__getitem__", &CTransmitStates::__getitem__);
	TransmitStates.def("__setitem__", &CTransmitStates::__setitem__);

	// Add memory tools...
	TransmitStates ADD_MEM_TOOLS(CTransmitStates);
}


//-----------------------------------------------------------------------------
// Exports ITransmitRules.
//-----------------------------------------------------------------------------
void export_base_transmit_rules(scope _transmit)
{
	class_<ITransmitRules, boost::noncopyable> BaseTransmitRules("BaseTransmitRules", no_init);

	// Properties...
	BaseTransmitRules.add_property(
		"mode",
		&ITransmitRules::GetMode,
		&ITransmitRules::SetMode,
		"Returns the transmit mode for these rules.\n"
		"\n"
		":rtype:\n"
		"	TransmitMode"
	);

	// Methods...
	BaseTransmitRules.def(
		"should_transmit",
		GET_METHOD(bool, ITransmitRules, ShouldTransmit, CBaseEntityWrapper *, CBaseEntityWrapper *),
		"Returns whether the given entity should be transmitted to the given player.\n"
		"\n"
		":rtype:\n"
		"	bool",
		args("self", "player", "entity")
	);

	// AutoUnload...
	BaseTransmitRules.def(
		"_unload_instance",
		&ITransmitRules::UnloadInstance,
		"Called when an instance is being unloaded.",
		args("self")
	);

	// Add memory tools...
	BaseTransmitRules ADD_MEM_TOOLS(ITransmitRules);
}


//-----------------------------------------------------------------------------
// Exports CTransmitHash.
//-----------------------------------------------------------------------------
void export_transmit_hash(scope _transmit)
{
	class_<CTransmitHash, boost::shared_ptr<CTransmitHash>, bases<ITransmitRules> > TransmitHash(
		"TransmitHash",
		"Transmissions rules where contained pairs are never transmitted to each other.\n"
		"\n"
		"Example:\n"
		"\n"
		".. code:: python\n"
		"\n"
		"	from entities.transmit import TransmitHash\n"
		"	from events import Event\n"
		"	from players.entity import Player\n"
		"\n"
		"	h = TransmitHash()\n"
		"\n"
		"	@Event('player_say')\n"
		"	def player_say(game_event):\n"
		"	    player = Player.from_userid(game_event['userid'])\n"
		"	    entity = player.view_entity\n"
		"	    if entity is None:\n"
		"	        return\n"
		"	    # Toggle transmissions with aimed entity\n"
		"	    if h.has_pair(player, entity):\n"
		"	        h.remove_pair(player, entity)\n"
		"	    else:\n"
		"	        h.add_pair(player, entity)",
		no_init
	);

	// Constructor...
	TransmitHash.def("__init__",
		make_constructor(
			&ITransmitRulesExt::Construct<CTransmitHash>,
			post_constructor_policies<default_call_policies>(
				make_function(
					&ITransmitRulesExt::Initialize<CTransmitHash>,
					default_call_policies(),
					args("self", "mode")
				)
			),
			("self", arg("mode")=TRANSMIT_MODE_PREVENT)
		),
		"Constructs and initializes the transmit hash.\n"
		"\n"
		":param TransmitMode mode:\n"
		"	The transmit mode for these rules."
	);

	// Methods...
	TransmitHash.def(
		"add_pair",
		&CTransmitHash::AddPair,
		"Adds the given entity pair to the hash.\n"
		"\n"
		":raises ValueError:\n"
		"	If none of the given entities is a player.",
		args("self", "entity", "other")
	);

	TransmitHash.def(
		"remove_pair",
		&CTransmitHash::RemovePair,
		"Removes the given pair from the hash.",
		args("self", "entity", "other")
	);

	TransmitHash.def(
		"remove_pairs",
		&CTransmitHash::RemovePairs,
		"Removes all pairs associated with the given entity.",
		args("self", "entity")
	);

	TransmitHash.def(
		"has_pair",
		GET_METHOD(bool, CTransmitHash, HasPair, CBaseEntityWrapper *, CBaseEntityWrapper *),
		"Returns whether the given pair is in the hash.\n"
		"\n"
		":rtype:\n"
		"	bool",
		args("self", "entity", "other")
	);

	TransmitHash.def(
		"get_count",
		&CTransmitHash::GetCount,
		"Returns the amount of pairs associated with the given entity.\n"
		"\n"
		":rtype:\n"
		"	int",
		args("self", "entity")
	);

	TransmitHash.def(
		"get_pairs",
		&CTransmitHash::GetPairs,
		"Returns a list of all entities associated with the given entity.\n"
		"\n"
		":rtype:\n"
		"	list",
		args("self", "entity")
	);

	TransmitHash.def(
		"clear",
		&CTransmitHash::Clear,
		"Removes all entities from the hash.",
		args("self")
	);

	// Special methods...
	TransmitHash.def(
		"__bool__",
		&CTransmitHash::HasElements,
		"Returns whether the hash is empty or not.\n"
		"\n"
		":rtype:\n"
		"	bool",
		args("self")
	);

	TransmitHash.def(
		"__contains__",
		&CTransmitHash::Contains,
		"Returns whether the given entity is in the hash.\n"
		"\n"
		":rtype:\n"
		"	bool",
		args("self", "entity")
	);

	TransmitHash.def(
		"__len__",
		&CTransmitHash::GetSize,
		"Returns the size of the transmission hash.\n"
		"\n"
		":rtype:\n"
		"	int",
		args("self")
	);

	TransmitHash.def(
		"__iter__",
		&CTransmitHash::Iterate,
		"Iterates over all entities contained in the hash.\n"
		"\n"
		":rtype:\n"
		"	iterator",
		args("self")
	);

	// Add memory tools...
	TransmitHash ADD_MEM_TOOLS(CTransmitHash);
}


//-----------------------------------------------------------------------------
// Exports CTransmitSet.
//-----------------------------------------------------------------------------
void export_transmit_set(scope _transmit)
{
	class_<CTransmitSet, boost::shared_ptr<CTransmitSet>, bases<ITransmitRules> > TransmitSet(
		"TransmitSet",
		"Transmission rules where contained entities are never transmitted to any player.\n"
		"\n"
		"Example:\n"
		"\n"
		".. code:: python\n"
		"\n"
		"	from entities.transmit import TransmitSet\n"
		"	from events import Event\n"
		"	from players.entity import Player\n"
		"\n"
		"	s = TransmitSet()\n"
		"\n"
		"	@Event('player_say')\n"
		"	def player_say(game_event):\n"
		"	    player = Player.from_userid(game_event['userid'])\n"
		"	    entity = player.view_entity\n"
		"	    if entity is None:\n"
		"	        return\n"
		"	    # Toggle transmissions with everything\n"
		"	    if entity in s:\n"
		"	        s.remove(entity)\n"
		"	    else:\n"
		"	        s.add(entity)",
		no_init
	);

	// Constructor...
	TransmitSet.def("__init__",
		make_constructor(
			&ITransmitRulesExt::Construct<CTransmitSet>,
			post_constructor_policies<default_call_policies>(
				make_function(
					&ITransmitRulesExt::Initialize<CTransmitSet>,
					default_call_policies(),
					args("self", "mode")
				)
			),
			(arg("mode")=TRANSMIT_MODE_PREVENT)
		),
		"Constructs and initializes the transmit set.\n"
		"\n"
		":param TransmitMode mode:\n"
		"	The transmit mode for these rules."
	);

	// Methods...
	TransmitSet.def(
		"add",
		&CTransmitSet::Add,
		"Adds the given entity to the set.\n"
		"\n"
		":raises ValueError:\n"
		"	If the given entity is not networked.",
		args("self", "entity")
	);

	TransmitSet.def(
		"remove",
		&CTransmitSet::Remove,
		"Removes the given entity from the set.",
		args("self", "entity")
	);

	TransmitSet.def(
		"clear",
		&CTransmitSet::Clear,
		"Removes all entities from the set.",
		args("self")
	);

	// Special methods...
	TransmitSet.def(
		"__bool__",
		&CTransmitSet::HasElements,
		"Returns whether the set is empty or not.\n"
		"\n"
		":rtype:\n"
		"	bool",
		args("self")
	);

	TransmitSet.def(
		"__contains__",
		GET_METHOD(bool, CTransmitSet, Contains, CBaseEntityWrapper *),
		"Returns whether the given entity is in the set or not.\n"
		"\n"
		":rtype:\n"
		"	bool",
		args("self", "entity")
	);

	TransmitSet.def(
		"__iter__",
		&CTransmitSet::Iterate,
		"Iterates over all entities contained in the set.\n"
		"\n"
		":rtype:\n"
		"	iterator",
		args("self")
	);

	TransmitSet.def(
		"__len__",
		&CTransmitSet::GetSize,
		"Returns the amount of entities contained in the set.\n"
		"\n"
		":rtype:\n"
		"	int",
		args("self")
	);

	// Add memory tools...
	TransmitSet ADD_MEM_TOOLS(CTransmitSet);
}


//-----------------------------------------------------------------------------
// Exports CTransmitMap.
//-----------------------------------------------------------------------------
void export_transmit_map(scope _transmit)
{
	class_<CTransmitMap, boost::shared_ptr<CTransmitMap>, bases<ITransmitRules> > TransmitMap(
		"TransmitMap",
		"Transmission rules that overrides per-player transmission.\n"
		"\n"
		"Example:\n"
		"\n"
		".. code:: python\n"
		"\n"
		"	from entities.transmit import TransmitMap\n"
		"	from events import Event\n"
		"	from players.entity import Player\n"
		"\n"
		"	m = TransmitMap()\n"
		"\n"
		"	@Event('player_say')\n"
		"	def player_say(game_event):\n"
		"	    player = Player.from_userid(game_event['userid'])\n"
		"	    entity = player.view_entity\n"
		"	    if entity is None:\n"
		"	        return\n"
		"	    # Toggle one-way transmissions with aimed entity\n"
		"	    s = m[entity]\n"
		"	    if player in s:\n"
		"	        s.remove(player)\n"
		"	    else:\n"
		"	        s.add(player)",
		no_init
	);

	// Constructor...
	TransmitMap.def("__init__",
		make_constructor(
			&ITransmitRulesExt::Construct<CTransmitMap>,
			post_constructor_policies<default_call_policies>(
				make_function(
					&ITransmitRulesExt::Initialize<CTransmitMap>,
					default_call_policies(),
					args("self", "mode")
				)
			),
			(arg("mode")=TRANSMIT_MODE_PREVENT)
		),
		"Constructs and initializes the transmit map.\n"
		"\n"
		":param TransmitMode mode:\n"
		"	The transmit mode for these rules."
	);

	// Methods...
	TransmitMap.def(
		"clear",
		&CTransmitMap::Clear,
		"Removes all players from the map.",
		args("self")
	);

	// Special methods...
	TransmitMap.def(
		"__getitem__",
		GET_METHOD(boost::shared_ptr<CTransmitSet>, CTransmitMap, Find, CBaseEntityWrapper *),
		"Returns the transmission set associated with the given entity.\n"
		"\n"
		":rtype:\n"
		"	TransmitSet",
		args("self", "entity")
	);

	TransmitMap.def(
		"__delitem__",
		&CTransmitMap::Remove,
		"Removes the transmission set associated with the given entity.",
		args("self", "entity")
	);

	TransmitMap.def(
		"__bool__",
		&CTransmitMap::HasElements,
		"Returns whether the map is empty or not.\n"
		"\n"
		":rtype:\n"
		"	bool",
		args("self")
	);

	TransmitMap.def(
		"__contains__",
		GET_METHOD(bool, CTransmitMap, Contains, CBaseEntityWrapper *),
		"Returns whether the given entity is in the map or not.\n"
		"\n"
		":rtype:\n"
		"	bool",
		args("self", "entity")
	);

	TransmitMap.def(
		"__len__",
		&CTransmitMap::GetSize,
		"Returns the amount of entities contained in the map.\n"
		"\n"
		":rtype:\n"
		"	int",
		args("self")
	);

	TransmitMap.def(
		"__iter__",
		&CTransmitMap::Iterate,
		"Iterates over all entities contained in the map.\n"
		"\n"
		":rtype:\n"
		"	iterator",
		args("self")
	);

	// Add memory tools...
	TransmitMap ADD_MEM_TOOLS(CTransmitMap);
}
