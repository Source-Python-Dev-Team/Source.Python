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
*
* Original source:
* https://developer.valvesoftware.com/wiki/Ingame_menu_for_server_plugins_(CS:S_only)
*/

//---------------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------------
#include "filters_recipients_wrap.h"
#include "interface.h"
#include "filesystem.h"
#include "engine/iserverplugin.h"
#include "game/server/iplayerinfo.h"
#include "eiface.h"
#include "igameevents.h"
#include "convar.h"
#include "Color.h"

#include "shake.h"
#include "game/shared/IEffects.h"
#include "engine/IEngineSound.h"
#include "utilities/conversions.h"

extern IVEngineServer		*engine;
extern IPlayerInfoManager	*playerinfomanager;
extern IServerPluginHelpers	*helpers;
extern CGlobalVars		*gpGlobals;

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

MRecipientFilter::MRecipientFilter()
{
	m_bReliable			= false;
	m_bInitMessage		= false;
	m_bUsingPredictionRules = false;
	m_bIgnorePredictionCull = true;
}

MRecipientFilter::~MRecipientFilter()
{
}

bool MRecipientFilter::IsReliable( void ) const
{
	return m_bReliable;
}

bool MRecipientFilter::IsInitMessage( void ) const
{
	return m_bInitMessage;
}

int MRecipientFilter::GetRecipientCount() const
{
	return m_Recipients.Count();
}

int MRecipientFilter::GetRecipientIndex(int slot) const
{
	if(slot < 0 || slot >= GetRecipientCount())
		return -1;

	return m_Recipients[slot];
}

void MRecipientFilter::AddAllPlayers()
{
	m_Recipients.RemoveAll();

	for(int i = 1; i <= gpGlobals->maxClients; i++)
	{
		edict_t *pPlayer = EdictFromIndex(i);

		// Skip invalid entities.
		if( !pPlayer ) {
			continue;
		}

		// Get and compare the classnames. Skip non-player
		// entities.
		const char* classname = pPlayer->GetClassName();
		if( strcmp(classname, "player") != 0 ) {
			continue;
		}

		m_Recipients.AddToTail(i);
	}
}
void MRecipientFilter::AddRecipient(int iPlayer)
{
	// Return if the recipient is already in the vector
	if(m_Recipients.Find(iPlayer) != m_Recipients.InvalidIndex())
		return;

	// Make sure the player is valid
	edict_t* pPlayer = EdictFromIndex(iPlayer);
	if(!pPlayer)
		return;

	// Get and compare the classnames. Skip non-player
	// entities.
	const char* classname = pPlayer->GetClassName();
	if( strcmp(classname, "player") != 0 ) {
		return;
	}

	m_Recipients.AddToTail(iPlayer);
}

void MRecipientFilter::RemoveRecipient( int iPlayer )
{
	m_Recipients.FindAndRemove(iPlayer);
}

void MRecipientFilter::RemoveAllPlayers()
{
	m_Recipients.RemoveAll();
}

bool MRecipientFilter::HasRecipient( int iPlayer )
{
	return m_Recipients.HasElement(iPlayer);
}
