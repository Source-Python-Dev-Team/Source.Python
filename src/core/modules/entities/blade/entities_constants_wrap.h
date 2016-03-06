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

#ifndef _ENTITIES_CONSTANTS_BLADE_WRAP_H
#define _ENTITIES_CONSTANTS_BLADE_WRAP_H

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
	// Nothing specific to Blade...
}


//-----------------------------------------------------------------------------
// Exports SolidFlags_t.
//-----------------------------------------------------------------------------
template<class T, class U>
void export_engine_specific_solid_flags(T _constants, U SolidFlags)
{
	SolidFlags.value("TRIGGER_TOUCH_PLAYER", FSOLID_TRIGGER_TOUCH_PLAYER);
	SolidFlags.value("NOT_MOVEABLE", FSOLID_NOT_MOVEABLE);
}


//-----------------------------------------------------------------------------
// Exports entity effects.
//-----------------------------------------------------------------------------
template<class T>
void export_engine_specific_entity_effects(T _constants)
{
	// Nothing specific to Blade...
}


//-----------------------------------------------------------------------------
// Exports RenderFx_t.
//-----------------------------------------------------------------------------
template<class T, class U>
void export_engine_specific_render_effects(T _constants, U RenderEffects)
{
	RenderEffects.value("FADE_OUT", kRenderFxFadeOut);
	RenderEffects.value("FADE_IN", kRenderFxFadeIn);
	RenderEffects.value("PULSE_FAST_WIDER", kRenderFxPulseFastWider);
}


//-----------------------------------------------------------------------------
// Exports Collision_Group_t.
//-----------------------------------------------------------------------------
template<class T, class U>
void export_engine_specific_collision_group(T _constants, U CollisionGroup)
{
	CollisionGroup.value("PZ_CLIP", COLLISION_GROUP_PZ_CLIP);
	CollisionGroup.value("DEBRIS_BLOCK_PROJECTILE", COLLISION_GROUP_DEBRIS_BLOCK_PROJECTILE);
}


#endif // _ENTITIES_CONSTANTS_BLADE_WRAP_H
