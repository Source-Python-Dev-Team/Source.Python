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
#ifndef _GLOBALS_H
#define _GLOBALS_H

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "edict.h"
#include "../entities/entities_wrap.h"
#include "../../utility/wrap_macros.h"

// Include the engine specific include
#include ENGINE_INCLUDE_PATH(globals_engine_implementation.h)

//-----------------------------------------------------------------------------
// Static singletons.
//-----------------------------------------------------------------------------
static CGlobalServerImplementationBase s_GlobalServerImplementation;

//-----------------------------------------------------------------------------
// CGlobals class
//-----------------------------------------------------------------------------
class CGlobals
{
public:
	bool is_client();
	bool is_remote_client();
	int get_network_base( int nTick, int nEntity );

	// Getter methods
	float				get_real_time();
	int				  get_frame_count();
	int				  get_absolute_frame_time();
	float				get_current_time();
	float				get_frame_time();
	int				  get_max_clients();
	int				  get_tick_count();
	float				get_interval_per_tick();
	float				get_interpolation_amount();
	int				  get_simulation_ticks_this_frame();
	int				  get_network_protocol();
	//CSaveRestoreData*	get_saved_data();
	const char*		  get_map_name();
	int				  get_map_version();
	const char*		  get_start_spot();
	MapLoadType_t		get_load_type();
	bool				 get_map_load_failed();
	bool				 get_deathmatch();
	bool				 get_coop();
	bool				 get_teamplay();
	int				  get_max_entities();
	const char*		  get_map_group_name();
	int				  get_server_count();
	CEdict*			  get_edicts();
};

//-----------------------------------------------------------------------------
// Signleton accessor function
//-----------------------------------------------------------------------------
CGlobals* get_globals();

#endif // _GLOBALS_H
