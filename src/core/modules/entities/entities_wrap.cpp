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
// Includes
//-----------------------------------------------------------------------------
#include "utilities/sp_util.h"
#include "edict.h"
#include "utilities/baseentity.h"
#include "game/shared/entitylist_base.h"
#include "game/server/entitylist.h"

#include "modules/memory/memory_tools.h"
#include "export_main.h"
#include "entities.h"
#include "entities_generator.h"

#include ENGINE_INCLUDE_PATH(entities_wrap.h)


//-----------------------------------------------------------------------------
// Forward declarations.
//-----------------------------------------------------------------------------
void export_base_entity_handle(scope);
void export_handle_entity(scope);
void export_server_unknown(scope);
void export_server_entity(scope);
void export_server_networkable(scope);
void export_edict(scope);
void export_entity_generator(scope);
void export_take_damage_info(scope);
void export_global_entity_list(scope);
void export_entity_listener(scope);
void export_check_transmit_info(scope);
void export_baseentity_generator(scope);
void export_server_class_generator(scope);


//-----------------------------------------------------------------------------
// Declare the _entities module.
//-----------------------------------------------------------------------------
DECLARE_SP_MODULE(_entities)
{
	export_base_entity_handle(_entities);
	export_handle_entity(_entities);
	export_server_unknown(_entities);
	export_server_entity(_entities);
	export_server_networkable(_entities);
	export_edict(_entities);
	export_entity_generator(_entities);
	export_take_damage_info(_entities);
	export_global_entity_list(_entities);
	export_entity_listener(_entities);
	export_check_transmit_info(_entities);
	export_baseentity_generator(_entities);
	export_server_class_generator(_entities);
}


//-----------------------------------------------------------------------------
// Exports CBaseEntityHandle.
//-----------------------------------------------------------------------------
void export_base_entity_handle(scope _entities)
{
	class_<CBaseHandle>("BaseEntityHandle")
		.def(init<CBaseHandle&>())
		.def(init<unsigned long>())
		.def(init<int, int>())

		.def("init",
			&CBaseHandle::Init,
			args("entry", "serial_number")
		)

		.def("term",
			&CBaseHandle::Term
		)

		.def("is_valid",
			&CBaseHandle::IsValid,
			"Returns whether the handle has been initted with any values."
		)

		.def("get_entry_index",
			&CBaseHandle::GetEntryIndex
		)

		.def("get_serial_number",
			&CBaseHandle::GetSerialNumber
		)

		.def("to_int",
			&CBaseHandle::ToInt
		)

		.def(self != self)
		.def(self == self)
		.def(self < self)

		.def("set",
			&CBaseHandle::Set,
			args("entity"),
			"Assigns a value to the handle.",
			reference_existing_object_policy()
		)

		ADD_MEM_TOOLS(CBaseHandle)
	;
}

//-----------------------------------------------------------------------------
// Exports CHandleEntity.
//-----------------------------------------------------------------------------
void export_handle_entity(scope _entities)
{
	class_<IHandleEntity, boost::noncopyable>("HandleEntity", no_init)
		.add_property("basehandle",
			make_function(&IHandleEntity::GetRefEHandle, reference_existing_object_policy()),
			&IHandleEntity::SetRefEHandle
		)

		ADD_MEM_TOOLS(IHandleEntity)
	;
}

//-----------------------------------------------------------------------------
// Exports CServerUnknown.
//-----------------------------------------------------------------------------
void export_server_unknown(scope _entities)
{
	class_< IServerUnknown, bases<IHandleEntity>, boost::noncopyable >("ServerUnknown", no_init)
		.def("get_collideable",
			&IServerUnknown::GetCollideable,
			"Returns the Collideable object for this entity.",
			reference_existing_object_policy()
		)

		.def("get_networkable",
			&IServerUnknown::GetNetworkable,
			"Returns the ServerNetworkable object for this entity.",
			reference_existing_object_policy()
		)

		.def("get_base_entity",
			&IServerUnknown::GetBaseEntity,
			"Returns the CBasEntity pointer for this entity.",
			return_by_value_policy()
		)

		.add_property("classname",
			&IServerUnknownExt::GetClassname,
			"Return the entity's classname."
		)

		.def("is_networked",
			&IServerUnknownExt::IsNetworked,
			"Return True if the entity is networked."
		)

		ADD_MEM_TOOLS(IServerUnknown)
	;
}

//-----------------------------------------------------------------------------
// Exports CServerEntity.
//-----------------------------------------------------------------------------
void export_server_entity(scope _entities)
{
	class_< IServerEntity, bases<IServerUnknown>, boost::noncopyable >("ServerEntity", no_init)
		.def("get_model_index",
			&IServerEntity::GetModelIndex,
			"Returns the model index for this entity."
		)

		.def("get_model_name",
			&IServerEntity::GetModelName,
			"Returns the name of the model this entity is using."
		)

		.def("set_model_index",
			&IServerEntity::SetModelIndex,
			"Sets the model of this entity.",
			args("index")
		)

		ADD_MEM_TOOLS(IServerEntity)
	;
}


//-----------------------------------------------------------------------------
// Exports CServerNetworkable.
//-----------------------------------------------------------------------------
void export_server_networkable(scope _entities)
{
	class_< IServerNetworkable, IServerNetworkable *, boost::noncopyable >("ServerNetworkable", no_init)
		.def("get_entity_handle",
			&IServerNetworkable::GetEntityHandle,
			"Returns the HandleEntity instance of this entity.",
			reference_existing_object_policy()
		)

		.def("get_server_class",
			&IServerNetworkable::GetServerClass,
			"Returns the ServerClass instance of this entity.",
			reference_existing_object_policy()
		)

		.def("get_edict",
			&IServerNetworkable::GetEdict,
			"Returns the edict_t instance of this entity.",
			reference_existing_object_policy()
		)

		.def("get_class_name",
			&IServerNetworkable::GetClassName,
			"Returns the class name of this entity."
		)

		.def("release",
			&IServerNetworkable::Release
		)

		.def("area_num",
			&IServerNetworkable::AreaNum
		)

		/*
		.def("get_base_networkable",
			&IServerNetworkable::GetBaseNetworkable,
			reference_existing_object_policy()
		)
		*/

		.def("get_base_entity",
			&IServerNetworkable::GetBaseEntity,
			return_by_value_policy()
		)

		.def("get_pvs_info",
			&IServerNetworkable::GetPVSInfo,
			"Returns the current visible data.",
			reference_existing_object_policy()
		)

		ADD_MEM_TOOLS(IServerNetworkable)
	;
}

//-----------------------------------------------------------------------------
// Exports edict_t.
//-----------------------------------------------------------------------------
void export_edict(scope _entities)
{
	class_< CBaseEdict >("_BaseEdict")
		.def("get_server_entity",
			GET_METHOD(IServerEntity*, CBaseEdict, GetIServerEntity),
			"Returns its ServerEntity instance.",
			reference_existing_object_policy()
		)

		.def("get_networkable",
			&CBaseEdict::GetNetworkable,
			"Returns its ServerNetworkable instance.",
			reference_existing_object_policy()
		)

		.def("get_unknown",
			&CBaseEdict::GetUnknown,
			"Returns its ServerUnknown instance.",
			reference_existing_object_policy()
		)

		.def("set_edict",
			&CBaseEdict::SetEdict,
			"Set when initting an entity. If it's only a networkable, this is false.",
			args("unknown", "full_edict")
		)

		.def("area_num",
			&CBaseEdict::AreaNum
		)

		.def("get_class_name",
			&CBaseEdict::GetClassName,
			"Returns the edict's class name."
		)

		.def("is_free",
			&CBaseEdict::IsFree
		)

		.def("set_free",
			&CBaseEdict::SetFree
		)

		.def("clear_free",
			&CBaseEdict::ClearFree
		)

		.def("has_state_changed",
			&CBaseEdict::HasStateChanged
		)

		.def("clear_state_changed",
			&CBaseEdict::ClearStateChanged
		)

		.def("state_changed",
			GET_METHOD(void, CBaseEdict, StateChanged)
		)

		/*
		.def("state_changed",
			static_cast< void(CBaseEdict::*)(unsigned short) >(&CBaseEdict::StateChanged),
			args("offset")
		)
		*/

		.def("clear_transmit_state",
			&CBaseEdict::ClearTransmitState
		)

		.def("set_change_info",
			&CBaseEdict::SetChangeInfo,
			args("info")
		)

		.def("set_change_info_serial_number",
			&CBaseEdict::SetChangeInfoSerialNumber,
			args("serial_number")
		)

		.def("get_change_info",
			&CBaseEdict::GetChangeInfo
		)

		.def("get_change_info_serial_number",
			&CBaseEdict::GetChangeInfoSerialNumber
		)

		.def("get_change_accessor",
			GET_METHOD(IChangeInfoAccessor*, CBaseEdict, GetChangeAccessor),
			reference_existing_object_policy()
		)

		// Class attributes
		.def_readwrite("state_flags",
			&CBaseEdict::m_fStateFlags
		)

		.def_readwrite("serial_number",
			&CBaseEdict::m_NetworkSerialNumber
		)

		.def_readwrite("networkable",
			&CBaseEdict::m_pNetworkable
		)

		ADD_MEM_TOOLS(CBaseEdict)
	;

	class_< edict_t, edict_t*, bases<CBaseEdict> >("Edict")
		.def("get_collidable",
			&edict_t::GetCollideable,
			"Returns its Collideable instance.",
			reference_existing_object_policy()
		)

		// Class attributes
		/*
		// TODO: Missing on CS:GO
		.def_readwrite("free_time",
			&edict_t::freetime,
			"The server timestampe at which the edict was freed."
		)
		*/

		ADD_MEM_TOOLS(edict_t)
	;
}


//-----------------------------------------------------------------------------
// Exports CEntityGenerator.
//-----------------------------------------------------------------------------
void export_entity_generator(scope _entities)
{
	class_<CEntityGenerator>("EntityGenerator")
		.def(init<const char*>())
		.def(init<const char*, bool>())

		.def("__iter__",
			&CEntityGenerator::iter,
			"Returns the iterable object."
		)

		.def("__next__",
			&CEntityGenerator::next,
			"Returns the next valid instance.",
			reference_existing_object_policy()
		)
	;
}


//-----------------------------------------------------------------------------
// Exports CTakeDamageInfo.
//-----------------------------------------------------------------------------
void export_take_damage_info(scope _entities)
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
	export_engine_specific_take_damage_info(_entities, TakeDamageInfo);
	
	// Add memory tools...
	TakeDamageInfo ADD_MEM_TOOLS(CTakeDamageInfo);
}


//-----------------------------------------------------------------------------
// Exports CGlobalEntityList.
//-----------------------------------------------------------------------------
void export_global_entity_list(scope _entities)
{
	class_<CGlobalEntityList, boost::noncopyable>("GlobalEntityList", no_init)
		.def("add_entity_listener",
			&CGlobalEntityList::AddListenerEntity
		)

		.def("remove_entity_listener",
			&CGlobalEntityList::AddListenerEntity
		)

		ADD_MEM_TOOLS(CGlobalEntityList);
	;
}


//-----------------------------------------------------------------------------
// Exports IEntityListener.
//-----------------------------------------------------------------------------
void export_entity_listener(scope _entities)
{
	class_<IEntityListener> EntityListener("EntityListener");

	// Add memory tools...
	EntityListener ADD_MEM_TOOLS(IEntityListener);
}


//-----------------------------------------------------------------------------
// Exports CCheckTransmitInfo.
//-----------------------------------------------------------------------------
void export_check_transmit_info(scope _entities)
{
	class_<CCheckTransmitInfo, CCheckTransmitInfo*> CheckTransmitInfo("CheckTransmitInfo");

	CheckTransmitInfo.def_readwrite("client", &CCheckTransmitInfo::m_pClientEnt);
	//byte	m_PVS[PAD_NUMBER( MAX_MAP_CLUSTERS,8 ) / 8];
	CheckTransmitInfo.def_readwrite("pvs_size", &CCheckTransmitInfo::m_nPVSSize, "PVS size in bytes.");

	//CBitVec<MAX_EDICTS>	*m_pTransmitEdict;	// entity n is already marked for transmission
	//CBitVec<MAX_EDICTS>	*m_pTransmitAlways; // entity n is always sent even if not in PVS (HLTV and Replay only)
	
	CheckTransmitInfo.def_readwrite("areas_networked", &CCheckTransmitInfo::m_AreasNetworked, "Number of networked areas.");
	//int		m_Areas[MAX_WORLD_AREAS]; // the areas
	
	// This is used to determine visibility, so if the previous state
	// is the same as the current state (along with pvs and areas networked),
	// then the parts of the map that the player can see haven't changed.
	//byte	m_AreaFloodNums[MAX_MAP_AREAS];
	CheckTransmitInfo.def_readwrite("map_areas", &CCheckTransmitInfo::m_nMapAreas, "Number of map areas.");

	CheckTransmitInfo ADD_MEM_TOOLS(CCheckTransmitInfo);
}


//-----------------------------------------------------------------------------
// Exports CBaseEntityGenerator.
//-----------------------------------------------------------------------------
void export_baseentity_generator(scope _entities)
{
	class_<CBaseEntityGenerator>("BaseEntityGenerator")
		.def("__iter__", &CBaseEntityGenerator::iter)
		.def("__next__", &CBaseEntityGenerator::next, reference_existing_object_policy())
	;
}


//-----------------------------------------------------------------------------
// Exports CServerClassGenerator.
//-----------------------------------------------------------------------------
void export_server_class_generator(scope _entities)
{
	class_<CServerClassGenerator>("ServerClassGenerator")
		.def("__iter__", &CServerClassGenerator::iter)
		.def("__next__", &CServerClassGenerator::next, reference_existing_object_policy())
	;
}
