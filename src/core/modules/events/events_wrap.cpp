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
// Includes.
//---------------------------------------------------------------------------------
#include "events_wrap.h"

//---------------------------------------------------------------------------------
// External interfaces we need.
//---------------------------------------------------------------------------------
extern IGameEventManager2* gameeventmanager;

//---------------------------------------------------------------------------------
// Game event manager singleton.
//---------------------------------------------------------------------------------
CGameEventManager* SP_GameEventManager = NULL;

//---------------------------------------------------------------------------------
// Returns the game event manager singleton.
//---------------------------------------------------------------------------------
CGameEventManager* get_game_event_manager()
{
	if( !SP_GameEventManager ) {
		SP_GameEventManager = new CGameEventManager(gameeventmanager);
	}

	return SP_GameEventManager;
}

//---------------------------------------------------------------------------------
// Methods for CGameEvent.
//---------------------------------------------------------------------------------
CGameEvent::CGameEvent( IGameEvent* game_event )
{
	m_game_event = game_event;
	m_created = false;
	m_fired = false;
}

CGameEvent::CGameEvent( const char* name, bool force )
{
	// Have the game event manager create the event.
	m_game_event = get_game_event_manager()->create_event(name, force)->get_event();

	// If false at the time of this object's destruction, we must free the event
	// manually.
	m_fired = false;

	// Event was created manually.
	m_created = true;
}

CGameEvent::~CGameEvent()
{
	// Per the documentation in igameevents.h, if the event is not fired,
	// it needs to be freed by IGameEventManager.
	if( !m_fired && m_created )
	{
		get_game_event_manager()->free_event(this);
	}
}

const char* CGameEvent::get_name() const 
{
	return m_game_event->GetName();
}

bool CGameEvent::is_reliable() const
{
	return m_game_event->IsReliable();
}

bool CGameEvent::is_local() const
{
	return m_game_event->IsLocal();
}

bool CGameEvent::is_empty( const char* keyName )
{
	return m_game_event->IsEmpty(keyName);
}

bool CGameEvent::get_bool( const char *keyName /*= NULL*/, bool defaultValue /*= false */ )
{
	return m_game_event->GetBool(keyName, defaultValue);
}

int CGameEvent::get_int( const char *keyName /*= NULL*/, int defaultValue /*= 0 */ )
{
	return m_game_event->GetInt(keyName, defaultValue);
}

float CGameEvent::get_float( const char *keyName /*= NULL*/, float defaultValue /*= 0.0f */ )
{
	return m_game_event->GetFloat(keyName, defaultValue);
}

const char* CGameEvent::get_string( const char *keyName /*= NULL*/, const char *defaultValue /*= "" */ )
{
	return m_game_event->GetString(keyName, defaultValue);
}

void CGameEvent::set_bool( const char *keyName, bool value )
{
	m_game_event->SetBool(keyName, value);
}

void CGameEvent::set_int( const char *keyName, int value )
{
	m_game_event->SetInt(keyName, value);
}

void CGameEvent::set_float( const char *keyName, float value )
{
	m_game_event->SetFloat(keyName, value);
}

void CGameEvent::set_string( const char *keyName, const char *value )
{
	m_game_event->SetString(keyName, value);
}

IGameEvent* CGameEvent::get_event() const
{
	return m_game_event;
}

void CGameEvent::fire_event()
{
	m_fired = get_game_event_manager()->fire_event(this);
}

//---------------------------------------------------------------------------------
// Methods for CGameEventListener.
//---------------------------------------------------------------------------------
void CGameEventListener::fire_game_event( CGameEvent* game_event )
{
	get_override("fire_game_event")(game_event);
}

int CGameEventListener::get_event_debug_id()
{
#ifdef EVENT_DEBUG_ID_INIT
	return EVENT_DEBUG_ID_INIT;
#else
    return 0;
#endif
}

void CGameEventListener::FireGameEvent( IGameEvent *event )
{
	CGameEvent game_event(event);
	fire_game_event(&game_event);
}

int CGameEventListener::GetEventDebugID()
{
	return get_event_debug_id();
}

//---------------------------------------------------------------------------------
// Methods for CGameEventManager.
//---------------------------------------------------------------------------------
CGameEventManager::CGameEventManager( IGameEventManager2* game_event_manager )
{
	m_game_event_manager = game_event_manager;
}

int CGameEventManager::load_events_from_file( const char* filename )
{
	return m_game_event_manager->LoadEventsFromFile(filename);	
}

void CGameEventManager::reset()
{
	m_game_event_manager->Reset();
}

bool CGameEventManager::add_listener( CGameEventListener* listener, const char* event_name )
{
	return m_game_event_manager->AddListener(listener, event_name, true);
}

bool CGameEventManager::find_listener( CGameEventListener* listener, const char* event_name )
{
	return m_game_event_manager->FindListener(listener, event_name);
}

void CGameEventManager::remove_listener( CGameEventListener* listener )
{
	m_game_event_manager->RemoveListener(listener);
}

CGameEvent* CGameEventManager::create_event( const char* event_name, bool bForce /*= false*/, int* pCookie /*= NULL */ )
{
	IGameEvent* game_event = m_game_event_manager->CreateEvent(event_name, bForce); // , pCookie);
	return new CGameEvent(game_event);
}

bool CGameEventManager::fire_event( CGameEvent* game_event, bool dont_broadcast )
{
	return m_game_event_manager->FireEvent(game_event->get_event(), dont_broadcast);
}

bool CGameEventManager::fire_event_client_side( CGameEvent* game_event )
{
	return m_game_event_manager->FireEventClientSide( game_event->get_event() );
}

CGameEvent* CGameEventManager::duplicate_event( CGameEvent* game_event )
{
	IGameEvent* game_event_2 = m_game_event_manager->DuplicateEvent(game_event->get_event());
	return new CGameEvent(game_event_2);
}

void CGameEventManager::free_event( CGameEvent* game_event )
{
	if( game_event ) 
	{
		m_game_event_manager->FreeEvent(game_event->get_event());
	}
}