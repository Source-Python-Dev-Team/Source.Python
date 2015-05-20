/**
* =============================================================================
* Source Python
* Copyright (C) 2015 Source Python Development Team.  All rights reserved.
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

#ifndef _ENTITIES_CONSTANTS_BMS_WRAP_PYTHON_H
#define _ENTITIES_CONSTANTS_BMS_WRAP_PYTHON_H

//-----------------------------------------------------------------------------
// Includes.
//-----------------------------------------------------------------------------
#include "utilities/wrap_macros.h"


//-----------------------------------------------------------------------------
// Exports damage types.
//-----------------------------------------------------------------------------
template<class T>
void export_engine_specific_damage_types(T _constants)
{
	// Nothing specific to BMS...
}


//-----------------------------------------------------------------------------
// Exports SolidFlags_t.
//-----------------------------------------------------------------------------
template<class T, class U>
void export_engine_specific_solid_flags(T _constants, U SolidFlags)
{
	// Nothing specific to BMS...
}


//-----------------------------------------------------------------------------
// Exports entity effects.
//-----------------------------------------------------------------------------
template<class T>
void export_engine_specific_entity_effects(T _constants)
{
	// Nothing specific to BMS...
}


//-----------------------------------------------------------------------------
// Exports RenderFx_t.
//-----------------------------------------------------------------------------
template<class T, class U>
void export_engine_specific_render_effects(T _constants, U RenderEffects)
{
	RenderEffects.value("DISTORT", kRenderFxDistort);
	RenderEffects.value("HOLOGRAM", kRenderFxHologram);
	RenderEffects.value("EXPLODE", kRenderFxExplode);
	RenderEffects.value("CLAMP_MIN_SCALE", kRenderFxClampMinScale);
	RenderEffects.value("ENV_RAIN", kRenderFxEnvRain);
	RenderEffects.value("ENV_SNOW", kRenderFxEnvSnow);
	RenderEffects.value("SPOTLIGHT", kRenderFxSpotlight);
	RenderEffects.value("RAGDOLL", kRenderFxRagdoll);
	RenderEffects.value("PULSE_FAST_WIDER", kRenderFxPulseFastWider);
}


//-----------------------------------------------------------------------------
// Exports Collision_Group_t.
//-----------------------------------------------------------------------------
template<class T, class U>
void export_engine_specific_collision_group(T _constants, U CollisionGroup)
{
	// Nothing specific to BMS...
}


#endif // _ENTITIES_CONSTANTS_BMS_WRAP_PYTHON_H
