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

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "edict.h"
#include "globals_wrap.h"

//-----------------------------------------------------------------------------
// Signleton accessor function
//-----------------------------------------------------------------------------
CGlobals* get_globals()
{
	static CGlobals* s_Globals = new CGlobals();
	return s_Globals;
}

bool CGlobals::is_client()
{
	return gpGlobals->IsClient();
}

bool CGlobals::is_remote_client()
{
	return s_GlobalServerImplementation.is_remote_client();
}

int CGlobals::get_network_base( int nTick, int nEntity )
{
	return gpGlobals->GetNetworkBase(nTick, nEntity);
}

// Getter methods
float CGlobals::get_real_time()
{
	return gpGlobals->realtime;
}

int CGlobals::get_frame_count()
{
	return gpGlobals->framecount;
}

int CGlobals::get_absolute_frame_time()
{
	return gpGlobals->absoluteframetime;
}

float CGlobals::get_current_time()
{
	return gpGlobals->curtime;
}

float CGlobals::get_frame_time()
{
	return gpGlobals->frametime;
}

int CGlobals::get_max_clients()
{
	return gpGlobals->maxClients;
}

int CGlobals::get_tick_count()
{
	return gpGlobals->tickcount;
}

float CGlobals::get_interval_per_tick()
{
	return gpGlobals->interval_per_tick;
}

float CGlobals::get_interpolation_amount()
{
	return gpGlobals->interpolation_amount;
}

int CGlobals::get_simulation_ticks_this_frame()
{
	return gpGlobals->simTicksThisFrame;
}

int CGlobals::get_network_protocol()
{
	return gpGlobals->network_protocol;
}

/*
CSaveRestoreData* CGlobals::get_saved_data()
{
	return NULL;
}
*/

const char* CGlobals::get_map_name()
{
	return gpGlobals->mapname.ToCStr();
}

int CGlobals::get_map_version()
{
	return gpGlobals->mapversion;
}

const char* CGlobals::get_start_spot()
{
	return gpGlobals->startspot.ToCStr();
}

MapLoadType_t CGlobals::get_load_type()
{
	return gpGlobals->eLoadType;
}

bool CGlobals::get_map_load_failed()
{
	return gpGlobals->bMapLoadFailed;
}

bool CGlobals::get_deathmatch()
{
	return gpGlobals->deathmatch;
}

bool CGlobals::get_coop()
{
	return gpGlobals->coop;
}

bool CGlobals::get_teamplay()
{
	return gpGlobals->teamplay;
}

int CGlobals::get_max_entities()
{
	return gpGlobals->maxEntities;
}

const char* CGlobals::get_map_group_name()
{
	return s_GlobalServerImplementation.get_map_group_name();
}

int CGlobals::get_server_count()
{
	return s_GlobalServerImplementation.get_server_count();
}

CEdict* CGlobals::get_edicts()
{
	return s_GlobalServerImplementation.get_edicts();
}
