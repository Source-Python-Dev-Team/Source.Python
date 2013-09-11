/**
* =============================================================================
* Source Python
* Copyright (C) 2012 Source Python Development Team.  All rights reserved.
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

//---------------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------------
#include "modules/export_main.h"
#include "igameevents.h"

//---------------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------------
#include "events_wrap.h"

//---------------------------------------------------------------------------------
// Method overloads.
//---------------------------------------------------------------------------------
DECLARE_CLASS_METHOD_OVERLOAD(CGameEvent, is_empty, 0, 1)
DECLARE_CLASS_METHOD_OVERLOAD(CGameEvent, get_bool, 0, 2)
DECLARE_CLASS_METHOD_OVERLOAD(CGameEvent, get_int, 0, 2)
DECLARE_CLASS_METHOD_OVERLOAD(CGameEvent, get_float, 0, 2)
DECLARE_CLASS_METHOD_OVERLOAD(CGameEvent, get_string, 0, 2)

//---------------------------------------------------------------------------------
// Functions that expose event functionality to us.
//---------------------------------------------------------------------------------
void export_igameevent();
void export_igameeventlistener();
void export_igameeventmanager();

//---------------------------------------------------------------------------------
// Exposes the Game Event module.
//---------------------------------------------------------------------------------
DECLARE_SP_MODULE(event_c)
{
	export_igameevent();
	export_igameeventlistener();
	export_igameeventmanager();
}

//---------------------------------------------------------------------------------
// Exposes IGameEvent.
//---------------------------------------------------------------------------------
void export_igameevent()
{
	BOOST_CLASS_CONSTRUCTOR(CGameEvent, const char*, bool)

		CLASS_METHOD(CGameEvent,
			get_name,
			"Returns the event name."
		)

		CLASS_METHOD(CGameEvent,
			is_reliable,
			"Returns true if the event handled reliably."
		)

		CLASS_METHOD(CGameEvent,
			is_local,
			"Returns true if the event never networked."
		)

		CLASS_METHOD_OVERLOAD(CGameEvent,
			is_empty,
			"Returns true if the given key exists.",
			args("keyName")
		)

		CLASS_METHOD_OVERLOAD(CGameEvent,
			get_bool,
			args("keyName", "defaultValue")
		)

		CLASS_METHOD_OVERLOAD(CGameEvent,
			get_int,
			args("keyName", "defaultValue")
		)

		CLASS_METHOD_OVERLOAD(CGameEvent,
			get_float,
			args("keyName", "defaultValue")
		)

		CLASS_METHOD_OVERLOAD(CGameEvent,
			get_string,
			args("keyName", "defaultValue")
		)

		CLASS_METHOD(CGameEvent,
			set_bool,
			args("keyName", "value")
		)

		CLASS_METHOD(CGameEvent,
			set_int,
			args("keyName", "value")
		)

		CLASS_METHOD(CGameEvent,
			set_float,
			args("keyName", "value")
		)

		CLASS_METHOD(CGameEvent,
			set_string,
			args("keyName", "value")
		)

		CLASS_METHOD(CGameEvent,
			fire_event,
			"Fires this event."
		)
	BOOST_END_CLASS()
}

//---------------------------------------------------------------------------------
// Exposes the game event listener.
//---------------------------------------------------------------------------------
void export_igameeventlistener()
{
	BOOST_CLASS_NOCOPY(CGameEventListener)

		CLASS_METHOD_PURE_VIRTUAL(CGameEventListener,
			fire_game_event,
			"Fires off a game event. NOTE: You must override this!",
			args("game_event")
		)

		CLASS_METHOD_PURE_VIRTUAL(CGameEventListener,
			get_event_debug_id,
			"Returns debug ID for the event listener."
		)

	BOOST_END_CLASS()
}

//---------------------------------------------------------------------------------
// Exposes the game event manager.
//---------------------------------------------------------------------------------
void export_igameeventmanager()
{
	BOOST_ABSTRACT_CLASS(CGameEventManager)

		CLASS_METHOD(CGameEventManager,
			load_events_from_file,
			"Loads game event descriptions from a file eg resource/gameevents.res",
			args("filename")
		)

		CLASS_METHOD(CGameEventManager,
			reset,
			"Removes all and anything."
		)

		CLASS_METHOD(CGameEventManager,
			add_listener,
			"Adds a listener for a particular event. Returns true on success.",
			args("listener", "event_name")
		)

		CLASS_METHOD(CGameEventManager,
			find_listener,
			"Returns true if the given listener is listening to the given event.",
			args("listener", "event_name")
		)

		CLASS_METHOD(CGameEventManager,
			remove_listener,
			"Stops a listener from receiving event notifications.",
			args("listener")
		)

		CLASS_METHOD(CGameEventManager,
			create_event,
			"Creates an event by name but doesn't fire it. Returns NULL if the event\
			is not known or no listener is registered for it. Setting should_force to\
			True forces the creation of the event even if no listener for it is active.",
			args("event_name", "bForce"),
			reference_existing_object_policy()
		)

		CLASS_METHOD(CGameEventManager,
			fire_event,
			"Fires a server event created by create_event. If dont_broadcast is set\
			to False (which it is by default), the event is not sent to clients.\
			Returns true if the event is fired off successfully.",
			args("game_event", "dont_broadcast")
		)

		CLASS_METHOD(CGameEventManager,
			fire_event_client_side,
			"Fires an event for the local client only. This should only be used by\
			client code!",
			args("game_event")
		)

		CLASS_METHOD(CGameEventManager,
			duplicate_event,
			"Creates a new copy of the given CGameEvent instance. This must be freed\
			by free_event later.",
			args("game_event"),
			reference_existing_object_policy()
		)

		CLASS_METHOD(CGameEventManager,
			free_event,
			"If an event is created but not fired, it must be freed. This method will\
			do that for you.",
			args("game_event")
		)

	BOOST_END_CLASS()

	BOOST_FUNCTION(get_game_event_manager,
		"Returns the CGameEventManager singleton.",
		reference_existing_object_policy()
	);
}