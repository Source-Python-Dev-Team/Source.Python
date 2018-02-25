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
#include "utilities/conversions.h"
#include "utilities/wrap_macros.h"
#include "modules/entities/entities_entity.h"


//-----------------------------------------------------------------------------
// Forward declarations.
//-----------------------------------------------------------------------------
void export_player_conversion_functions(scope);


//-----------------------------------------------------------------------------
// Declare the _players._helpers module.
//-----------------------------------------------------------------------------
DECLARE_SP_SUBMODULE(_players, _helpers)
{
	// Conversion functions...
	export_player_conversion_functions(_helpers);
}


//-----------------------------------------------------------------------------
// Exports conversion functions.
//-----------------------------------------------------------------------------
void export_player_conversion_functions(scope _helpers)
{
	// To Index conversions...
	EXPORT_CONVERSION_FUNCTION(unsigned int, Index, unsigned int, Userid);
	EXPORT_CONVERSION_FUNCTION(unsigned int, Index, IPlayerInfo *, PlayerInfo);
	EXPORT_CONVERSION_FUNCTION(unsigned int, Index, const char *, Name);
	EXPORT_CONVERSION_FUNCTION(unsigned int, Index, const char *, SteamID);
	EXPORT_CONVERSION_FUNCTION(unsigned int, Index, const char *, UniqueID);

	// To Edict conversions...
	EXPORT_CONVERSION_FUNCTION(edict_t *, Edict, unsigned int, Userid, reference_existing_object_policy());
	EXPORT_CONVERSION_FUNCTION(edict_t *, Edict, IPlayerInfo *, PlayerInfo, reference_existing_object_policy());

	// To BaseHandle conversions...
	EXPORT_CONVERSION_FUNCTION(CBaseHandle, BaseHandle, unsigned int, Userid);
	EXPORT_CONVERSION_FUNCTION(CBaseHandle, BaseHandle, IPlayerInfo *, PlayerInfo);

	// To IntHandle conversions...
	EXPORT_CONVERSION_FUNCTION(unsigned int, IntHandle, unsigned int, Userid);
	EXPORT_CONVERSION_FUNCTION(unsigned int, IntHandle, IPlayerInfo *, PlayerInfo);

	// To Pointer conversions...
	EXPORT_CONVERSION_FUNCTION(CPointer, Pointer, unsigned int, Userid);
	EXPORT_CONVERSION_FUNCTION(CPointer, Pointer, IPlayerInfo *, PlayerInfo);

	// To UserID conversions...
	EXPORT_CONVERSION_FUNCTION(unsigned int, Userid, unsigned int, Index);
	EXPORT_CONVERSION_FUNCTION(unsigned int, Userid, edict_t *, Edict);
	EXPORT_CONVERSION_FUNCTION(unsigned int, Userid, CBaseHandle, BaseHandle);
	EXPORT_CONVERSION_FUNCTION(unsigned int, Userid, unsigned int, IntHandle);
	EXPORT_CONVERSION_FUNCTION(unsigned int, Userid, CPointer *, Pointer);
	EXPORT_CONVERSION_FUNCTION(unsigned int, Userid, IPlayerInfo *, PlayerInfo);
	EXPORT_CONVERSION_FUNCTION(unsigned int, Userid, CBaseEntity *, BaseEntity);
	
	// To PlayerInfo conversions...
	EXPORT_CONVERSION_FUNCTION(IPlayerInfo *, PlayerInfo, unsigned int, Index, reference_existing_object_policy());
	EXPORT_CONVERSION_FUNCTION(IPlayerInfo *, PlayerInfo, edict_t *, Edict, reference_existing_object_policy());
	EXPORT_CONVERSION_FUNCTION(IPlayerInfo *, PlayerInfo, CBaseHandle, BaseHandle, reference_existing_object_policy());
	EXPORT_CONVERSION_FUNCTION(IPlayerInfo *, PlayerInfo, unsigned int, IntHandle, reference_existing_object_policy());
	EXPORT_CONVERSION_FUNCTION(IPlayerInfo *, PlayerInfo, CPointer *, Pointer, reference_existing_object_policy());
	EXPORT_CONVERSION_FUNCTION(IPlayerInfo *, PlayerInfo, unsigned int, Userid, reference_existing_object_policy());
	EXPORT_CONVERSION_FUNCTION(IPlayerInfo *, PlayerInfo, CBaseEntity *, BaseEntity, reference_existing_object_policy());

	// To BaseEntity conversions...
	EXPORT_CONVERSION_FUNCTION(CBaseEntity *, BaseEntity, unsigned int, Userid, return_by_value_policy());
	EXPORT_CONVERSION_FUNCTION(CBaseEntity *, BaseEntity, IPlayerInfo *, PlayerInfo, return_by_value_policy());

	// To Address conversions...
	EXPORT_CONVERSION_FUNCTION(str, Address, IPlayerInfo *, PlayerInfo);

	// To UniqueID conversions...
	EXPORT_CONVERSION_FUNCTION(str, UniqueID, IPlayerInfo *, PlayerInfo);
	EXPORT_CONVERSION_FUNCTION(str, UniqueID, unsigned int, Index);
}
