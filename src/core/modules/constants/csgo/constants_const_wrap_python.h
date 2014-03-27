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

#ifndef _CONSTANTS_CONST_WRAP_PYTHON_CSGO_H
#define _CONSTANTS_CONST_WRAP_PYTHON_CSGO_H

//-----------------------------------------------------------------------------
// Includes.
//-----------------------------------------------------------------------------
#include "const.h"


//-----------------------------------------------------------------------------
// Expose SolidFlags_t.
//-----------------------------------------------------------------------------
template<class T>
void export_engine_specific_solid_flags(T SolidFlags)
{
	SolidFlags.value("TRIGGER_TOUCH_PLAYER", FSOLID_TRIGGER_TOUCH_PLAYER);
	SolidFlags.value("NOT_MOVEABLE", FSOLID_NOT_MOVEABLE);
}


//-----------------------------------------------------------------------------
// Expose RenderFx_t.
//-----------------------------------------------------------------------------
template<class T>
void export_engine_specific_render_effects(T RenderEffects)
{
	RenderEffects.value("FADE_OUT", kRenderFxFadeOut);
	RenderEffects.value("FADE_IN", kRenderFxFadeIn);
	RenderEffects.value("PULSE_FAST_WIDER", kRenderFxPulseFastWider);
}


//-----------------------------------------------------------------------------
// Expose Collision_Group_t.
//-----------------------------------------------------------------------------
template<class T>
void export_engine_specific_collision_groups(T CollisionGroups)
{
	CollisionGroups.value("PZ_CLIP", COLLISION_GROUP_PZ_CLIP);
	CollisionGroups.value("DEBRIS_BLOCK_PROJECTILE", COLLISION_GROUP_DEBRIS_BLOCK_PROJECTILE);
}


//-----------------------------------------------------------------------------
// Expose EntityEffects.
//-----------------------------------------------------------------------------
template<class T>
void export_engine_specific_entity_effects(T EntityEffects)
{
	/* TODO...
	EF_MARKED_FOR_FAST_REFLECTION = 0x400,
 	EF_NOSHADOWDEPTH = 0x800,
 	EF_NOFLASHLIGHT = = 0x1000*/
}


#endif // _CONSTANTS_CONST_WRAP_PYTHON_CSGO_H
