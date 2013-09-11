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
#ifndef _PLAYERS_WRAP_H
#define _PLAYERS_WRAP_H

// ----------------------------------------------------------------------------
// Includes.
// ----------------------------------------------------------------------------
#include "edict.h"
#include "game/server/iplayerinfo.h"
#include "mathlib/vector.h"
#include "modules/entities/entities_wrap.h"
#include "inetchannelinfo.h"
#include "modules/vecmath/vecmath_wrap.h"

// ----------------------------------------------------------------------------
// CPlayerInfo class.
// ----------------------------------------------------------------------------
class CPlayerInfo
{
public:

	// Get CPlayerInfo instances by index.
	CPlayerInfo( int userid );

	// Get CPlayerInfo instances by CEdict
	CPlayerInfo( CEdict* edict_ptr );

	// For engine use.
	CPlayerInfo( IPlayerInfo* playerinfo );

	virtual const char*		get_name() const;
	virtual int 				get_userid() const;
	const char* 				get_networkid_string() const;
	virtual int 				get_team_index() const;
	virtual void 				change_team( int iTeamNum );
	virtual int 				get_frag_count() const;
	virtual int 				get_death_count() const;
	virtual bool 				is_connected() const;
	virtual int 				get_armor_value() const;
	virtual bool 				is_hltv() const;
	virtual bool 				is_player() const;
	virtual bool 				is_fake_client() const;
	virtual bool 				is_dead() const;
	virtual bool 				is_in_a_vehicle() const;
	virtual bool 				is_observer() const;
	virtual const CVector 		get_abs_origin() const;
	virtual const QAngle 		get_abs_angles() const;
	virtual const CVector 		get_player_mins() const;
	virtual const CVector 		get_player_maxs() const;
	virtual const char* 		get_weapon_name() const;
	virtual const char* 		get_model_name() const;
	virtual int 				get_health() const;
	virtual int 				get_max_health() const;
	virtual CEdict*			get_edict() const;

private:
	CEdict*			m_edict_ptr;
	IPlayerInfo*	m_iplayerinfo_ptr;
};

class CNetChannelInfo
{
public:
	// Constructor
	CNetChannelInfo( INetChannelInfo* netinfo );

	// Methods
	virtual const char*		get_address();
	virtual float				get_time_connected();

private:
	INetChannelInfo* m_netinfo_ptr;
};

#endif
