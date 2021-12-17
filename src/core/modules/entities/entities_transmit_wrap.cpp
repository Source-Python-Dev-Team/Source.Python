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
		"Registers a transmit hook."
	);

	TransmitManager.def(
		"unregister_hook",
		&CTransmitManager::UnregisterTransmitHook,
		"Unregisters a transmit hook."
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
		"Returns the transmission mode for these rules."
	);

	// Methods...
	BaseTransmitRules.def(
		"should_transmit",
		GET_METHOD(bool, ITransmitRules, ShouldTransmit, CBaseEntityWrapper *, CBaseEntityWrapper *),
		"Returns whether the given entity should be transmitted to the given player."
	);

	// AutoUnload...
	BaseTransmitRules.def(
		"_unload_instance",
		&ITransmitRules::UnloadInstance,
		"Called when an instance is being unloaded."
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
		"Transmissions rules where contained pairs are never transmitted to each other.",
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
			(arg("mode")=TRANSMIT_MODE_PREVENT)
		)
	);

	// Methods...
	TransmitHash.def(
		"add_pair",
		&CTransmitHash::AddPair,
		"Adds the given entity pair to the hash."
	);

	TransmitHash.def(
		"remove_pair",
		&CTransmitHash::RemovePair,
		"Removes the given pair from the hash."
	);

	TransmitHash.def(
		"remove_pairs",
		&CTransmitHash::RemovePairs,
		"Removes all pairs associated with the given entity."
	);

	TransmitHash.def(
		"has_pair",
		GET_METHOD(bool, CTransmitHash, HasPair, CBaseEntityWrapper *, CBaseEntityWrapper *),
		"Returns whether the given pair is in the hash."
	);

	TransmitHash.def(
		"get_count",
		&CTransmitHash::GetCount,
		"Returns the amount of pairs associated with the given entity."
	);

	TransmitHash.def(
		"get_pairs",
		&CTransmitHash::GetPairs,
		"Returns a list of all entities associated with the given entity."
	);

	TransmitHash.def(
		"clear",
		&CTransmitHash::Clear,
		"Removes all entities from the hash."
	);

	// Special methods...
	TransmitHash.def(
		"__bool__",
		&CTransmitHash::HasElements,
		"Returns whether the hash is empty or not."
	);

	TransmitHash.def(
		"__contains__",
		&CTransmitHash::Contains,
		"Returns whether the given entity is in the hash."
	);

	TransmitHash.def(
		"__len__",
		&CTransmitHash::GetSize,
		"Returns the size of the transmission hash."
	);

	TransmitHash.def(
		"__iter__",
		&CTransmitHash::Iterate,
		"Iterates over all entities contained in the hash."
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
		"Transmission rules where contained entities are never transmitted to any player.",
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
		)
	);

	// Methods...
	TransmitSet.def(
		"add",
		&CTransmitSet::Add,
		"Adds the given entity to the set."
	);

	TransmitSet.def(
		"remove",
		&CTransmitSet::Remove,
		"Removes the given entity from the set."
	);

	TransmitSet.def(
		"clear",
		&CTransmitSet::Clear,
		"Removes all entities from the set."
	);

	// Special methods...
	TransmitSet.def(
		"__bool__",
		&CTransmitSet::HasElements,
		"Returns whether the set is empty or not."
	);

	TransmitSet.def(
		"__contains__",
		GET_METHOD(bool, CTransmitSet, Contains, CBaseEntityWrapper *),
		"Returns whether the given entity is in the set or not."
	);

	TransmitSet.def(
		"__iter__",
		&CTransmitSet::Iterate,
		"Iterates over all entities contained in the set."
	);

	TransmitSet.def(
		"__len__",
		&CTransmitSet::GetSize,
		"Returns the amount of entities contained in the set."
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
		"Transmission rules that overrides per-player transmission.",
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
		)
	);

	// Methods...
	TransmitMap.def(
		"clear",
		&CTransmitMap::Clear,
		"Removes all players from the map."
	);

	// Special methods...
	TransmitMap.def(
		"__getitem__",
		GET_METHOD(boost::shared_ptr<CTransmitSet>, CTransmitMap, Find, CBaseEntityWrapper *),
		"Returns the transmission set associated with the given player."
	);

	TransmitMap.def(
		"__delitem__",
		&CTransmitMap::Remove,
		"Removes the transmission set associated with the given player."
	);

	TransmitMap.def(
		"__bool__",
		&CTransmitMap::HasElements,
		"Returns whether the map is empty or not."
	);

	TransmitMap.def(
		"__contains__",
		GET_METHOD(bool, CTransmitMap, Contains, CBaseEntityWrapper *),
		"Returns whether the given entities is in the map or not."
	);

	TransmitMap.def(
		"__len__",
		&CTransmitMap::GetSize,
		"Returns the amount of entities contained in the map."
	);

	TransmitMap.def(
		"__iter__",
		&CTransmitMap::Iterate,
		"Iterates over all entities contained in the map."
	);

	// Add memory tools...
	TransmitMap ADD_MEM_TOOLS(CTransmitMap);
}
