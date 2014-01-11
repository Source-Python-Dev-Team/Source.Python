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
// Includes we need
//-----------------------------------------------------------------------------
#include "modules/entities/entities_wrap.h"
#include "convar.h"

//-----------------------------------------------------------------------------
// Externals
//-----------------------------------------------------------------------------
extern CGlobalVars* gpGlobals;

//-----------------------------------------------------------------------------
// Purpose: Abstract class for all the specific engine implementation calls
//-----------------------------------------------------------------------------
abstract_class CGlobalServerImplementationBase
{
	// Most likely most of these base functions will be stubs unless we can
	// achieve what they do with other engine calls
public:
	// Default functionality - returns false
	virtual bool is_remote_client();

	// Default functionality - ""
	virtual const char* get_map_group_name();

	// Default functionality - return 0
	virtual int get_server_count();

	// Default functionality - return NULL
	virtual edict_t* get_edicts();
};
