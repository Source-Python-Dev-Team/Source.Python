/**
* =============================================================================
* Source Python
* Copyright (C) 2014 Source Python Development Team.  All rights reserved.
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
#include "modules/export_main.h"
#include "utility/wrap_macros.h"
#include "mathlib/vector.h"
#include "tier1/utlvector.h"
#include "dt_send.h"
#include "game/shared/ehandle.h"
#include "isaverestore.h"
#include "datamap.h"
#include "game/shared/takedamageinfo.h"
#include "game/shared/shareddefs.h"

#include ENGINE_INCLUDE_PATH(entities_constants_wrap_python.h)


//-----------------------------------------------------------------------------
// Forward declarations.
//-----------------------------------------------------------------------------
void export_damage_types(scope);


//-----------------------------------------------------------------------------
// Declare the _entities._constants module.
//-----------------------------------------------------------------------------
DECLARE_SP_SUBMODULE(_entities, _constants)
{
    export_damage_types(_constants);
}


//-----------------------------------------------------------------------------
// Expose damage types.
//-----------------------------------------------------------------------------
void export_damage_types(scope _constants)
{
    // Damage types constants...
    _constants.attr("DMG_BULLET") = DMG_BULLET;
    _constants.attr("DMG_SLASH") = DMG_SLASH;
    _constants.attr("DMG_BURN") = DMG_BURN;
    _constants.attr("DMG_VEHICLE") = DMG_VEHICLE;
    _constants.attr("DMG_FALL") = DMG_FALL;
    _constants.attr("DMG_BLAST") = DMG_BLAST;
    _constants.attr("DMG_CLUB") = DMG_CLUB;
    _constants.attr("DMG_SHOCK") = DMG_SHOCK;
    _constants.attr("DMG_SONIC") = DMG_SONIC;
    _constants.attr("DMG_ENERGYBEAM") = DMG_ENERGYBEAM;
    _constants.attr("DMG_PREVENT_PHYSICS_FORCE") = DMG_PREVENT_PHYSICS_FORCE;
    _constants.attr("DMG_NEVERGIB") = DMG_NEVERGIB;
    _constants.attr("DMG_ALWAYSGIB") = DMG_ALWAYSGIB;
    _constants.attr("DMG_DROWN") = DMG_DROWN;
    _constants.attr("DMG_PARALYZE") = DMG_PARALYZE;
    _constants.attr("DMG_NERVEGAS") = DMG_NERVEGAS;
    _constants.attr("DMG_POISON") = DMG_POISON;
    _constants.attr("DMG_RADIATION") = DMG_RADIATION;
    _constants.attr("DMG_DROWNRECOVER") = DMG_DROWNRECOVER;
    _constants.attr("DMG_ACID") = DMG_ACID;
    _constants.attr("DMG_SLOWBURN") = DMG_SLOWBURN;
    _constants.attr("DMG_REMOVENORAGDOLL") = DMG_REMOVENORAGDOLL;
    _constants.attr("DMG_PHYSGUN") = DMG_PHYSGUN;
    _constants.attr("DMG_PLASMA") = DMG_PLASMA;
    _constants.attr("DMG_AIRBOAT") = DMG_AIRBOAT;
    _constants.attr("DMG_DISSOLVE") = DMG_DISSOLVE;
    _constants.attr("DMG_BLAST_SURFACE") = DMG_BLAST_SURFACE;
    _constants.attr("DMG_DIRECT") = DMG_DIRECT;
    _constants.attr("DMG_BUCKSHOT") = DMG_BUCKSHOT;
    _constants.attr("DMG_LASTGENERICFLAG") = DMG_LASTGENERICFLAG;
    _constants.attr("BASEDAMAGE_NOT_SPECIFIED") = BASEDAMAGE_NOT_SPECIFIED;

    // CS:GO/CS:S specific constants...
    _constants.attr("DMG_HEADSHOT") = object();

    // Engine specific stuff..
    export_engine_specific_damage_types(_constants);
}
