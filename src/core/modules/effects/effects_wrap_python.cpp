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
// These includes are really important. Be careful if you try to change the
// order or remove an include!
#include "mathlib/vector.h"
#include <stddef.h>
#include "wchartypes.h"
#include "string_t.h"
#include "shake.h"
#include "IEffects.h"
#include "utility/wrap_macros.h"
#include "modules/export_main.h"


//-----------------------------------------------------------------------------
// Externals
//-----------------------------------------------------------------------------
extern IEffects* effects;

// This is required to fix a linker error
IPredictionSystem* IPredictionSystem::g_pPredictionSystems = NULL;

//-----------------------------------------------------------------------------
// Exposes the effects_c module.
//-----------------------------------------------------------------------------
void export_effects();

DECLARE_SP_MODULE(effects_c)
{
	export_effects();
}

//-----------------------------------------------------------------------------
// Expose effects
//-----------------------------------------------------------------------------
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(sparks_overload, Sparks, 1, 4)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(energy_splash_overload, EnergySplash, 2, 3)

void export_effects()
{
	class_<IPredictionSystem>("PredictionSystem")
		.def("get_next",
			&IPredictionSystem::GetNext,
			"Returns the next prediction system.",
			reference_existing_object_policy()
		)

		.def("set_suppress_event",
			&IPredictionSystem::SetSuppressEvent
		)

		/*
		TODO: CBaseEntity
		.def("set_suppress_host",
			&IPredictionSystem::SetSuppressHost
		)
		
		TODO: CBaseEntity
		.def("get_suppress_host",
			&IPredictionSystem::GetSuppressHost,
			reference_existing_object_policy()
		)
		*/
		
		.def("can_predict",
			&IPredictionSystem::CanPredict
		)

		.def_readwrite("prediction_systems",
			&IPredictionSystem::g_pPredictionSystems
		)

		.def("suppress_events",
			&IPredictionSystem::SuppressEvents
		)
		.staticmethod("suppress_events")

		/*
		.def("suppress_host_events",
			&IPredictionSystem::SuppressHostEvents
		)
		.staticmethod("suppress_host_events")
		*/
	;

	class_<IEffects, bases<IPredictionSystem>, boost::noncopyable>("Effects", no_init)
		.def("beam",
			&IEffects::Beam,
			"Creates a beam particle effect.",
			args("start_vec", "end_vec", "model_index", "halo_index", "start_frame", "frame_rate", "life", "width", "end_width", "fade_length", "noise", "red", "green", "blue", "brightness", "speed")
		)
		
		.def("smoke",
			&IEffects::Smoke,
			"Creates a smoke effect.",
			args("origin", "model_index", "scale", "frame_rate")
		)
		
		.def("sparks",
			&IEffects::Sparks,
			sparks_overload(
				"Creates a sparks effect.",
				args("origin", "magnitude", "trail_length", "direction")
			)
		)
		
		.def("dust",
			&IEffects::Dust,
			"Creates a dust effect.",
			args("origin", "direction", "size", "speed")
		)
		
		.def("muzzle_flash",
			&IEffects::MuzzleFlash,
			"Creates a muzzle flash effect.",
			args("origin", "angles", "scale", "type")
		)
		
		.def("metal_sparks",
			&IEffects::MetalSparks,
			"Creates a muzzle flash effect.",
			args("origin", "direction")
		)

		.def("energy_splash",
			&IEffects::EnergySplash,
			energy_splash_overload(
				"Creates a metal sparks effect.",
				args("origin", "direction", "explosive")
			)
		)

		.def("ricochet",
			&IEffects::Ricochet,
			"Creates a ricochet effect.",
			args("origin", "direction")
		)
		
		.def("get_time",
			&IEffects::Time,
			"Returns the current time."
		)
		
		.def("is_server",
			&IEffects::IsServer,
			"Returns True if this is a server."
		)
		
		.def("suppress_effects_sounds",
			&IEffects::SuppressEffectsSounds,
			"Set to True to suppress effect sounds.",
			args("suppress")
		)
	;

	scope().attr("Effects") = object(ptr(effects));
}
