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

// ----------------------------------------------------------------------------
// Includes
// ----------------------------------------------------------------------------
#include "players_wrap.h"
#include "mathlib/vector.h"
#include "modules/entities/entities_wrap.h"
#include "utility/sp_util.h"

// ----------------------------------------------------------------------------
// External variables.
// ----------------------------------------------------------------------------
extern IPlayerInfoManager* playerinfomanager;

// ----------------------------------------------------------------------------
// CPlayerInfo methods.
// ----------------------------------------------------------------------------
CPlayerInfo::CPlayerInfo( IPlayerInfo* iplayerinfo )
{
	m_iplayerinfo_ptr = iplayerinfo;
	edict_t* edict = EdictOfPlayer(m_iplayerinfo_ptr);
	CEdict* m_edict = new CEdict(edict);
	m_edict_ptr = m_edict;
}

CPlayerInfo::CPlayerInfo( CEdict* edict_ptr )
{
	m_edict_ptr = edict_ptr;
	int index = m_edict_ptr->get_index();
	edict_t* edict = PEntityOfEntIndex(index);
	m_iplayerinfo_ptr = playerinfomanager->GetPlayerInfo(edict);
}

CPlayerInfo::CPlayerInfo( int userid )
{
	m_iplayerinfo_ptr = PlayerOfUserid(userid);
	edict_t* edict = EdictOfPlayer(m_iplayerinfo_ptr);
	CEdict* m_edict = new CEdict(edict);
	m_edict_ptr = m_edict;
}

const char* CPlayerInfo::get_name() const
{
	return m_iplayerinfo_ptr->GetName();
}

int CPlayerInfo::get_userid() const
{
	return m_iplayerinfo_ptr->GetUserID();
}

const char* CPlayerInfo::get_networkid_string() const
{
	return m_iplayerinfo_ptr->GetNetworkIDString();
}

int CPlayerInfo::get_team_index() const
{
	return m_iplayerinfo_ptr->GetTeamIndex();
}

void CPlayerInfo::change_team( int iTeamNum )
{
	m_iplayerinfo_ptr->ChangeTeam(iTeamNum);
}

int CPlayerInfo::get_frag_count() const
{
	return m_iplayerinfo_ptr->GetFragCount();
}

int CPlayerInfo::get_death_count() const
{
	return m_iplayerinfo_ptr->GetDeathCount();
}

bool CPlayerInfo::is_connected() const
{
	return m_iplayerinfo_ptr->IsConnected();
}

int CPlayerInfo::get_armor_value() const
{
	return m_iplayerinfo_ptr->GetArmorValue();
}

bool CPlayerInfo::is_hltv() const
{
	return m_iplayerinfo_ptr->IsHLTV();
}

bool CPlayerInfo::is_player() const
{
	return m_iplayerinfo_ptr->IsPlayer();
}

bool CPlayerInfo::is_fake_client() const
{
	return m_iplayerinfo_ptr->IsFakeClient();
}

bool CPlayerInfo::is_dead() const
{
	return m_iplayerinfo_ptr->IsDead();
}

bool CPlayerInfo::is_in_a_vehicle() const
{
	return m_iplayerinfo_ptr->IsInAVehicle();
}

bool CPlayerInfo::is_observer() const
{
	return m_iplayerinfo_ptr->IsObserver();
}

const CVector CPlayerInfo::get_abs_origin() const
{
	return CVector(m_iplayerinfo_ptr->GetAbsOrigin());
}

const QAngle CPlayerInfo::get_abs_angles() const
{
	return m_iplayerinfo_ptr->GetAbsAngles();
}

const CVector CPlayerInfo::get_player_mins() const
{
	return CVector(m_iplayerinfo_ptr->GetPlayerMins());
}

const CVector CPlayerInfo::get_player_maxs() const
{
	return CVector(m_iplayerinfo_ptr->GetPlayerMaxs());
}

const char* CPlayerInfo::get_weapon_name() const
{
	return m_iplayerinfo_ptr->GetWeaponName();
}

const char* CPlayerInfo::get_model_name() const
{
	return m_iplayerinfo_ptr->GetModelName();
}

int CPlayerInfo::get_health() const
{
	return m_iplayerinfo_ptr->GetHealth();
}

int CPlayerInfo::get_max_health() const
{
	return m_iplayerinfo_ptr->GetMaxHealth();
}

CEdict* CPlayerInfo::get_edict() const
{
	return new CEdict(m_edict_ptr->get_edict());
}

//-----------------------------------------------------------------------------
// CNetChannelInfo Methods
//-----------------------------------------------------------------------------
CNetChannelInfo::CNetChannelInfo( INetChannelInfo* netinfo )
{
	m_netinfo_ptr = netinfo;
}

const char* CNetChannelInfo::get_address()
{
	return m_netinfo_ptr->GetAddress();
}

float CNetChannelInfo::get_time_connected()
{
	return m_netinfo_ptr->GetTimeConnected();
}
