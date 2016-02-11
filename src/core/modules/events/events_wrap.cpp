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
#include "modules/memory/memory_tools.h"
#include "events.h"
#include "igameevents.h"


//-----------------------------------------------------------------------------
// External variables.
//-----------------------------------------------------------------------------
extern IGameEventManager2* gameeventmanager;


//-----------------------------------------------------------------------------
// Forward declarations.
//-----------------------------------------------------------------------------
void export_igameevent(scope);
void export_igameeventlistener(scope);
void export_igameeventmanager(scope);


//-----------------------------------------------------------------------------
// Declare the _events module.
//-----------------------------------------------------------------------------
DECLARE_SP_MODULE(_events)
{
	export_igameevent(_events);
	export_igameeventlistener(_events);
	export_igameeventmanager(_events);
}


//-----------------------------------------------------------------------------
// Exports IGameEvent.
//-----------------------------------------------------------------------------
void export_igameevent(scope _events)
{
	class_<IGameEvent, boost::noncopyable>("GameEvent", no_init)
		.add_property("name",
			&IGameEvent::GetName,
			"Returns the event name."
		)

		.def("is_reliable",
			&IGameEvent::IsReliable,
			"Returns true if the event handled reliably."
		)

		.def("is_local",
			&IGameEvent::IsLocal,
			"Returns true if the event never networked."
		)

		.def("is_empty",
			&IGameEvent::IsEmpty,
			"Returns true if the given key exists. If no key name was passed, it will check if the whole event is empty.",
			(arg("key_name")=((const char*) NULL))
		)

		.def("get_bool",
			&IGameEvent::GetBool,
			("key_name", arg("default_value")=false),
			"Returns the value of the key name as a bool."
		)

		.def("get_int",
			&IGameEvent::GetInt,
			("key_name", arg("default_value")=0),
			"Returns the value of the key name as an int."
		)

		.def("get_float",
			&IGameEvent::GetFloat,
			("key_name", arg("default_value")=0.0f),
			"Returns the value of the key name as a float."
		)

		.def("get_string",
			&IGameEventExt::GetString,
			("key_name", arg("default_value")=""),
			"Returns the value of the key name as a string."
		)

		.def("set_bool",
			&IGameEvent::SetBool,
			"Sets the given key name.",
			args("key_name", "value")
		)

		.def("set_int",
			&IGameEvent::SetInt,
			"Sets the given key name.",
			args("key_name", "value")
		)

		.def("set_float",
			&IGameEvent::SetFloat,
			"Sets the given key name.",
			args("key_name", "value")
		)

		.def("set_string",
			&IGameEvent::SetString,
			"Sets the given key name.",
			args("key_name", "value")
		)

		.add_property("variables",
			make_function(IGameEventExt::GetVariables, reference_existing_object_policy())
		)

		.def("__getitem__",
			&IGameEventExt::__getitem__
		)

		.def("__setitem__",
			&IGameEventExt::__setitem__
		)

		ADD_MEM_TOOLS(IGameEvent)
	;

	BEGIN_CLASS_INFO(IGameEvent)
		FUNCTION_INFO(GetName)
		FUNCTION_INFO(IsReliable)
		FUNCTION_INFO(IsLocal)
		FUNCTION_INFO(IsEmpty)
		FUNCTION_INFO(GetBool)
		FUNCTION_INFO(GetInt)
		FUNCTION_INFO(GetFloat)
		FUNCTION_INFO(GetString)
		FUNCTION_INFO(SetBool)
		FUNCTION_INFO(SetInt)
		FUNCTION_INFO(SetFloat)
		FUNCTION_INFO(SetString)
	END_CLASS_INFO()
}


//-----------------------------------------------------------------------------
// Exports CGameEventListener2.
//-----------------------------------------------------------------------------
void export_igameeventlistener(scope _events)
{
	class_<CGameEventListener2, boost::noncopyable>("GameEventListener")
		.def("fire_game_event",
			pure_virtual(&CGameEventListener2::FireGameEvent),
			"Fires the given game event.",
			args("event")
		)

		.add_property("event_debug_id",
			&CGameEventListener2::GetEventDebugID
		)

		ADD_MEM_TOOLS_WRAPPER(CGameEventListener2, IGameEventListener2)
	;

	BEGIN_CLASS_INFO(IGameEventListener2)
		FUNCTION_INFO(FireGameEvent)
		// TODO: GetEventDebugID
	END_CLASS_INFO()
}


//-----------------------------------------------------------------------------
// Exports IGameEventManager2.
//-----------------------------------------------------------------------------
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(create_event_overload, CreateEvent, 2, 2);

void export_igameeventmanager(scope _events)
{
	class_<IGameEventManager2, boost::noncopyable>("_GameEventManager", no_init)
		.def("load_events_from_file",
			&IGameEventManager2::LoadEventsFromFile,
			"Loads game event descriptions from a file eg resource/gameevents.res",
			args("filename")
		)

		.def("reset",
			&IGameEventManager2::Reset,
			"Removes all and anything."
		)

		.def("add_listener",
			&IGameEventManager2::AddListener,
			"Adds a listener for a particular event. Returns true on success.",
			args("listener", "event_name", "server_side")
		)

		.def("find_listener",
			&IGameEventManager2::FindListener,
			"Returns true if the given listener is listening to the given event.",
			args("listener", "event_name")
		)

		.def("remove_listener",
			&IGameEventManager2::RemoveListener,
			"Stops a listener from receiving event notifications.",
			args("listener")
		)

		.def("create_event",
			&IGameEventManager2::CreateEvent,
			create_event_overload(
				"Creates an event by name but doesn't fire it. Returns NULL if the event\
				is not known or no listener is registered for it. Setting <force> to True\
				forces the creation of the event even if no listener for it is active.",
				("event_name", arg("force")=false)
			)[reference_existing_object_policy()]
		)

		.def("fire_event",
			&IGameEventManager2::FireEvent,
			"Fires a server event created by create_event. If dont_broadcast is set\
			 to False (which it is by default), the event is not sent to clients.\
			 Returns true if the event is fired off successfully.",
			("game_event", arg("dont_broadcast")=false)
		)

		.def("fire_event_client_side",
			&IGameEventManager2::FireEventClientSide,
			"Fires an event for the local client only. This should only be used by\
			 client code!",
			args("game_event")
		)

		.def("duplicate_event",
			&IGameEventManager2::DuplicateEvent,
			"Creates a new copy of the given CGameEvent instance. This must be freed\
			 by free_event later.",
			args("game_event"),
			reference_existing_object_policy()
		)

		.def("free_event",
			&IGameEventManager2::FreeEvent,
			"If an event is created but not fired, it must be freed. This method will\
			 do that for you.",
			args("game_event")
		)

		ADD_MEM_TOOLS(IGameEventManager2)
	;

	BEGIN_CLASS_INFO(IGameEventManager2)
		FUNCTION_INFO(LoadEventsFromFile)
		FUNCTION_INFO(Reset)
		FUNCTION_INFO(AddListener)
		FUNCTION_INFO(FindListener)
		FUNCTION_INFO(RemoveListener)

		// TODO: This method is overloaded in some SDKs
		// FUNCTION_INFO(CreateEvent)
		
		FUNCTION_INFO(FireEvent)
		FUNCTION_INFO(FireEventClientSide)
		FUNCTION_INFO(DuplicateEvent)
		FUNCTION_INFO(FreeEvent)
		FUNCTION_INFO(SerializeEvent)
		FUNCTION_INFO(UnserializeEvent)
	END_CLASS_INFO()

	_events.attr("game_event_manager") = object(ptr(gameeventmanager));
}
