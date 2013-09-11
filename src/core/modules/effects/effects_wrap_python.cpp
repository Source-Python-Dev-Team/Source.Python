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
#include "modules/export_main.h"
#include "effects_wrap.h"

void export_effects();

//-----------------------------------------------------------------------------
// Exposes the effects_c module.
//-----------------------------------------------------------------------------
DECLARE_SP_MODULE(effects_c)
{
	export_effects();
}

DECLARE_CLASS_METHOD_OVERLOAD(CEffects, sparks, 1, 4);
DECLARE_CLASS_METHOD_OVERLOAD(CEffects, energy_splash, 2, 3);

void export_effects()
{
	BOOST_ABSTRACT_CLASS(CEffects)
		
		CLASS_METHOD(CEffects,
			beam,
			"Creates a beam particle effect.",
			args("vecStart", "vecEnd", "iModelIndex", "iHaloIndex", "frameStart", "frameRate", "fLife", "width", "endWidth", "fadeLength", "noise", "red", "green", "blue", "brightness", "speed")
		)
		
		CLASS_METHOD(CEffects,
			smoke,
			"Creates a smoke effect.",
			args("vecOrigin", "iModelIndex", "fScale", "fFrameRate")
		)
		
		CLASS_METHOD_OVERLOAD(CEffects,
			sparks,
			"Creates a sparks effect.",
			args("vecOrigin", "iMagnitude", "iTrailLength", "vecDirection")
		)
		
		CLASS_METHOD(CEffects,
			dust,
			"Creates a dust effect.",
			args("vecOrigin", "vecDirection", "fSize", "fSpeed")
		)
		
		CLASS_METHOD(CEffects,
			muzzle_flash,
			"Creates a muzzle flash effect.",
			args("vecOrigin", "vecAngles", "fScale", "iType")
		)
		
		CLASS_METHOD(CEffects,
			metal_sparks,
			"Creates a muzzle flash effect.",
			args("vecOrigin", "vecDirection")
		)

		CLASS_METHOD_OVERLOAD(CEffects,
			energy_splash,
			"Creates a metal sparks effect.",
			args("vecOrigin", "vecDirection", "bExplosive")
		)

		CLASS_METHOD(CEffects,
			ricochet,
			"Creates a ricochet effect.",
			args("vecOrigin", "vecDirection")
		)
		
		CLASS_METHOD(CEffects,
			ricochet,
			"Creates a ricochet.",
			args("vecOrigin", "vecDirection")
		)
		
		CLASS_METHOD(CEffects,
			get_time,
			"Returns the current time."
		)
		
		CLASS_METHOD(CEffects,
			is_server,
			"Returns True if this is a server."
		)
		
		CLASS_METHOD(CEffects,
			suppress_effects_sounds,
			"Set to True to suppress effect sounds.",
			args("bSuppress")
		)

	BOOST_END_CLASS()

	BOOST_FUNCTION(get_effects, reference_existing_object_policy());
}