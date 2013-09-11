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
#ifndef _EVENTS_WRAP_H
#define _EVENTS_WRAP_H

//---------------------------------------------------------------------------------
// Includes.
//---------------------------------------------------------------------------------
#include "igameevents.h"
#include "modules/export_main.h"

//---------------------------------------------------------------------------------
// The GameEvent base class.
//---------------------------------------------------------------------------------
class CGameEvent
{
public:
	CGameEvent( IGameEvent* game_event );
	CGameEvent( const char* name, bool force );
	~CGameEvent();

	virtual const char*		get_name() const;
	
	virtual bool			is_reliable() const;
	virtual bool			is_local() const;
	virtual bool			is_empty( const char* keyName = NULL );

	virtual bool			get_bool( const char *keyName = NULL, bool defaultValue = false );
	virtual int				get_int( const char *keyName = NULL, int defaultValue = 0 );
	virtual float			get_float( const char *keyName = NULL, float defaultValue = 0.0f );
	virtual const char*		get_string( const char *keyName = NULL, const char *defaultValue = "" );

	virtual void			set_bool( const char *keyName, bool value );
	virtual void			set_int( const char *keyName, int value );
	virtual void			set_float( const char *keyName, float value );
	virtual void			set_string( const char *keyName, const char *value );

	// Custom methods.
	IGameEvent*				get_event() const;
	void					fire_event();

private:
	IGameEvent* m_game_event;
	bool		m_created;
	bool		m_fired;
};

//---------------------------------------------------------------------------------
// This is the IGameEventListener2 callback class. It allows python to subclass
// IGameEventListener2 then pass an instance of that to CGameEventManager.
//---------------------------------------------------------------------------------
class CGameEventListener : public IGameEventListener2, 
	public wrapper<IGameEventListener2>
{
public:
	virtual void	fire_game_event( CGameEvent* game_event );
	virtual int		get_event_debug_id( void );

	// From IGameEventListener2.
	virtual void	FireGameEvent( IGameEvent *event );
	virtual int		GetEventDebugID();
};

//---------------------------------------------------------------------------------
// The game event manager.
//---------------------------------------------------------------------------------
class CGameEventManager
{
public:
	CGameEventManager( IGameEventManager2* game_event_manager );

	virtual int			load_events_from_file( const char* filename );
	virtual void		reset();

	virtual bool		add_listener( CGameEventListener* listener, const char* event_name );
	virtual bool		find_listener( CGameEventListener* listener, const char* event_name );
	virtual void		remove_listener( CGameEventListener* listener );

	virtual CGameEvent* create_event( const char* event_name, bool bForce = false, int* pCookie = NULL );
	
	virtual bool		fire_event( CGameEvent* game_event, bool dont_broadcast = false );
	virtual bool		fire_event_client_side( CGameEvent* game_event );

	virtual CGameEvent*	duplicate_event( CGameEvent* game_event );
	virtual void		free_event( CGameEvent* game_event );

	// virtual bool serialize_event( CGameEvent* game_Event, CBfWrite bit_buf );
	// virtual CGameEvent* unserialize_event( CBfWrite bit_buf );

private:
	IGameEventManager2* m_game_event_manager;
};

//---------------------------------------------------------------------------------
// Accessors for singletons.
//---------------------------------------------------------------------------------
extern CGameEventManager* get_game_event_manager();

#endif
