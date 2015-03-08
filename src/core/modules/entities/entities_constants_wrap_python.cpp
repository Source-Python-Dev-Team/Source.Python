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
#include "string_t.h"
#include "dt_send.h"
#include "game/shared/ehandle.h"
#include "isaverestore.h"
#include "datamap.h"
#include "game/shared/takedamageinfo.h"
#include "game/shared/shareddefs.h"
#include "const.h"

#include ENGINE_INCLUDE_PATH(entities_constants_wrap_python.h)


//-----------------------------------------------------------------------------
// Forward declarations.
//-----------------------------------------------------------------------------
void export_damage_types(scope);
void export_entity_states(scope);
void export_move_type(scope);
void export_move_collide(scope);
void export_solid_type(scope);
void export_solid_flags(scope);
void export_entity_effects(scope);
void export_render_mode(scope);
void export_render_effects(scope);
void export_collision_group(scope);
void export_dissolve_types(scope);
void export_take_damage(scope);
void export_entity_flags(scope);


//-----------------------------------------------------------------------------
// Declare the _entities._constants module.
//-----------------------------------------------------------------------------
DECLARE_SP_SUBMODULE(_entities, _constants)
{
	export_damage_types(_constants);
	export_entity_states(_constants);
	export_move_type(_constants);
	export_move_collide(_constants);
	export_solid_type(_constants);
	export_solid_flags(_constants);
	export_entity_effects(_constants);
	export_render_mode(_constants);
	export_render_effects(_constants);
	export_collision_group(_constants);
	export_dissolve_types(_constants);
	export_take_damage(_constants);
	export_entity_flags(_constants);
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


//-----------------------------------------------------------------------------
// Expose entity state flags.
//-----------------------------------------------------------------------------
void export_entity_states(scope _constants)
{
	// Entity flags constants...
	_constants.attr("FL_FLY") = FL_FLY;
	_constants.attr("FL_SWIM") = FL_SWIM;
	_constants.attr("FL_CONVEYOR") = FL_CONVEYOR;
	_constants.attr("FL_NPC") = FL_NPC;
	_constants.attr("FL_GODMODE") = FL_GODMODE;
	_constants.attr("FL_NOTARGET") = FL_NOTARGET;
	_constants.attr("FL_AIMTARGET") = FL_AIMTARGET;
	_constants.attr("FL_PARTIALGROUND") = FL_PARTIALGROUND;
	_constants.attr("FL_STATICPROP") = FL_STATICPROP;
	_constants.attr("FL_GRAPHED") = FL_GRAPHED;
	_constants.attr("FL_GRENADE") = FL_GRENADE;
	_constants.attr("FL_STEPMOVEMENT") = FL_STEPMOVEMENT;
	_constants.attr("FL_DONTTOUCH") = FL_DONTTOUCH;
	_constants.attr("FL_BASEVELOCITY") = FL_BASEVELOCITY;
	_constants.attr("FL_WORLDBRUSH") = FL_WORLDBRUSH;
	_constants.attr("FL_OBJECT") = FL_OBJECT;
	_constants.attr("FL_KILLME") = FL_KILLME;
	_constants.attr("FL_ONFIRE") = FL_ONFIRE;
	_constants.attr("FL_DISSOLVING") = FL_DISSOLVING;
	_constants.attr("FL_TRANSRAGDOLL") = FL_TRANSRAGDOLL;
	_constants.attr("FL_UNBLOCKABLE_BY_PLAYER") = FL_UNBLOCKABLE_BY_PLAYER;
}


//-----------------------------------------------------------------------------
// Expose MoveType_t.
//-----------------------------------------------------------------------------
void export_move_type(scope _constants)
{
	enum_<MoveType_t> MoveType("MoveType");

	// Values...
	MoveType.value("NONE", MOVETYPE_NONE);
	MoveType.value("ISOMETRIC", MOVETYPE_ISOMETRIC);
	MoveType.value("WALK", MOVETYPE_WALK);
	MoveType.value("STEP", MOVETYPE_STEP);
	MoveType.value("FLY", MOVETYPE_FLY);
	MoveType.value("FLYGRAVITY", MOVETYPE_FLYGRAVITY);
	MoveType.value("VPHYSICS", MOVETYPE_VPHYSICS); 
	MoveType.value("PUSH", MOVETYPE_PUSH);
	MoveType.value("NOCLIP", MOVETYPE_NOCLIP);
	MoveType.value("LADDER", MOVETYPE_LADDER);
	MoveType.value("OBSERVER", MOVETYPE_OBSERVER);
	MoveType.value("CUSTOM", MOVETYPE_CUSTOM);
}


//-----------------------------------------------------------------------------
// Expose MoveCollide_t.
//-----------------------------------------------------------------------------
void export_move_collide(scope _constants)
{
	enum_<MoveCollide_t> MoveCollide("MoveCollide");

	// Values...
	MoveCollide.value("DEFAULT", MOVECOLLIDE_DEFAULT);
	MoveCollide.value("FLY_BOUNCE", MOVECOLLIDE_FLY_BOUNCE);
	MoveCollide.value("FLY_CUSTOM", MOVECOLLIDE_FLY_CUSTOM);
	MoveCollide.value("FLY_SLIDE", MOVECOLLIDE_FLY_SLIDE);
	MoveCollide.value("COUNT", MOVECOLLIDE_COUNT);
}


//-----------------------------------------------------------------------------
// Expose SolidType_t.
//-----------------------------------------------------------------------------
void export_solid_type(scope _constants)
{
	enum_<SolidType_t> SolidType("SolidType");

	// Values...
	SolidType.value("NONE", SOLID_NONE);
	SolidType.value("BSP", SOLID_BSP);
	SolidType.value("BBOX", SOLID_BBOX);
	SolidType.value("OBB", SOLID_OBB);
	SolidType.value("OBB_YAW", SOLID_OBB_YAW);
	SolidType.value("CUSTOM", SOLID_CUSTOM);
	SolidType.value("VPHYSICS", SOLID_VPHYSICS);
}


//-----------------------------------------------------------------------------
// Expose SolidFlags_t.
//-----------------------------------------------------------------------------
void export_solid_flags(scope _constants)
{
	enum_<SolidFlags_t> SolidFlags("SolidFlags");

	// Values...
	SolidFlags.value("CUSTOMRAYTEST", FSOLID_CUSTOMRAYTEST);
	SolidFlags.value("CUSTOMBOXTEST", FSOLID_CUSTOMBOXTEST);
	SolidFlags.value("NOT_SOLID", FSOLID_NOT_SOLID);
	SolidFlags.value("TRIGGER", FSOLID_TRIGGER);
	SolidFlags.value("NOT_STANDABLE", FSOLID_NOT_STANDABLE);
	SolidFlags.value("VOLUME_CONTENTS", FSOLID_VOLUME_CONTENTS);
	SolidFlags.value("FORCE_WORLD_ALIGNED", FSOLID_FORCE_WORLD_ALIGNED);
	SolidFlags.value("USE_TRIGGER_BOUNDS", FSOLID_USE_TRIGGER_BOUNDS);
	SolidFlags.value("ROOT_PARENT_ALIGNED", FSOLID_ROOT_PARENT_ALIGNED);
	SolidFlags.value("TRIGGER_TOUCH_DEBRIS", FSOLID_TRIGGER_TOUCH_DEBRIS);

	// CS:GO specific values...
	NOT_IMPLEMENTED_VALUE(SolidFlags_t, "TRIGGER_TOUCH_PLAYER");
	NOT_IMPLEMENTED_VALUE(SolidFlags_t, "NOT_MOVEABLE");

	// Engine specific stuff...
	export_engine_specific_solid_flags(SolidFlags);
}


//-----------------------------------------------------------------------------
// Expose entity effects.
//-----------------------------------------------------------------------------
void export_entity_effects(scope _constants)
{
	_constants.attr("EF_BONEMERGE") = (long)EF_BONEMERGE;
	_constants.attr("EF_BRIGHTLIGHT") = (long)EF_BRIGHTLIGHT;
	_constants.attr("EF_DIMLIGHT") = (long)EF_DIMLIGHT;
	_constants.attr("EF_NOINTERP") = (long)EF_NOINTERP;
	_constants.attr("EF_NOSHADOW") = (long)EF_NOSHADOW;
	_constants.attr("EF_NODRAW") = (long)EF_NODRAW;
	_constants.attr("EF_NORECEIVESHADOW") = (long)EF_NORECEIVESHADOW;
	_constants.attr("EF_BONEMERGE_FASTCULL") = (long)EF_BONEMERGE_FASTCULL;
	_constants.attr("EF_ITEM_BLINK") = (long)EF_ITEM_BLINK;
	_constants.attr("EF_PARENT_ANIMATES") = (long)EF_PARENT_ANIMATES;

	// CS:GO specific constants...
	_constants.attr("EF_MARKED_FOR_FAST_REFLECTION") = object();
	_constants.attr("EF_NOSHADOWDEPTH") = object();
	_constants.attr("EF_NOFLASHLIGHT") = object();

	// Engine specific stuff...
	export_engine_specific_entity_effects(_constants);
}


//-----------------------------------------------------------------------------
// Expose RenderMode_t.
//-----------------------------------------------------------------------------
void export_render_mode(scope _constants)
{
	enum_<RenderMode_t> RenderMode("RenderMode");

	// Values...
	RenderMode.value("NORMAL", kRenderNormal);
	RenderMode.value("TRANS_COLOR", kRenderTransColor);
	RenderMode.value("TRANS_TEXTURE", kRenderTransTexture);
	RenderMode.value("GLOW", kRenderGlow);
	RenderMode.value("TRANS_ALPHA", kRenderTransAlpha);
	RenderMode.value("TRANS_ADD", kRenderTransAdd);
	RenderMode.value("ENVIRONMENTAL", kRenderEnvironmental);
	RenderMode.value("TRANS_ADD_FRAME_BLEND", kRenderTransAddFrameBlend);
	RenderMode.value("WORLD_GLOW", kRenderWorldGlow);
	RenderMode.value("NONE", kRenderNone);
}


//-----------------------------------------------------------------------------
// Expose RenderFx_t.
//-----------------------------------------------------------------------------
void export_render_effects(scope _constants)
{
	enum_<RenderFx_t> RenderEffects("RenderEffects");

	// Values...
	RenderEffects.value("NONE", kRenderFxNone);
	RenderEffects.value("PULSE_SLOW", kRenderFxPulseSlow);
	RenderEffects.value("PULSE_FAST", kRenderFxPulseFast);
	RenderEffects.value("PULSE_SLOW_WIDE", kRenderFxPulseSlowWide);
	RenderEffects.value("PULSE_FAST_WIDE", kRenderFxPulseFastWide);
	RenderEffects.value("FADE_SLOW", kRenderFxFadeSlow);
	RenderEffects.value("FADE_FAST", kRenderFxFadeFast);
	RenderEffects.value("SOLID_SLOW", kRenderFxSolidSlow);
	RenderEffects.value("SOLID_FAST", kRenderFxSolidFast);
	RenderEffects.value("STROBE_SLOW", kRenderFxStrobeSlow);
	RenderEffects.value("STROBE_FAST", kRenderFxStrobeFast);
	RenderEffects.value("STROBE_FASTER", kRenderFxStrobeFaster);
	RenderEffects.value("FLICKER_SLOW", kRenderFxFlickerSlow);
	RenderEffects.value("FLICKER_FAST", kRenderFxFlickerFast);
	RenderEffects.value("NO_DISSIPATION", kRenderFxNoDissipation);
	RenderEffects.value("GLOW_SHELL", kRenderFxGlowShell);

	// CS:GO specific values...
	NOT_IMPLEMENTED_VALUE(RenderFx_t, "FADE_OUT");
	NOT_IMPLEMENTED_VALUE(RenderFx_t, "FADE_IN");
	NOT_IMPLEMENTED_VALUE(RenderFx_t, "PULSE_FAST_WIDER");

	// OrangeBox specific values...
	NOT_IMPLEMENTED_VALUE(RenderFx_t, "DISTORT");
	NOT_IMPLEMENTED_VALUE(RenderFx_t, "HOLOGRAM");
	NOT_IMPLEMENTED_VALUE(RenderFx_t, "EXPLODE");
	NOT_IMPLEMENTED_VALUE(RenderFx_t, "CLAMP_MIN_SCALE");
	NOT_IMPLEMENTED_VALUE(RenderFx_t, "ENV_RAIN");
	NOT_IMPLEMENTED_VALUE(RenderFx_t, "ENV_SNOW");
	NOT_IMPLEMENTED_VALUE(RenderFx_t, "SPOTLIGHT");
	NOT_IMPLEMENTED_VALUE(RenderFx_t, "RAGDOLL");
	NOT_IMPLEMENTED_VALUE(RenderFx_t, "PULSE_FAST_WIDER");

	// Engine specific stuff...
	export_engine_specific_render_effects(RenderEffects);
}


//-----------------------------------------------------------------------------
// Expose Collision_Group_t.
//-----------------------------------------------------------------------------
void export_collision_group(scope _constants)
{
	enum_<Collision_Group_t> CollisionGroup("CollisionGroup");

	// Values...
	CollisionGroup.value("NONE", COLLISION_GROUP_NONE);
	CollisionGroup.value("DEBRIS", COLLISION_GROUP_DEBRIS);
	CollisionGroup.value("DEBRIS_TRIGGER", COLLISION_GROUP_DEBRIS_TRIGGER);
	CollisionGroup.value("INTERACTIVE_DEBRIS", COLLISION_GROUP_INTERACTIVE_DEBRIS);
	CollisionGroup.value("INTERACTIVE", COLLISION_GROUP_INTERACTIVE);
	CollisionGroup.value("PLAYER", COLLISION_GROUP_PLAYER);
	CollisionGroup.value("BREAKABLE_GLASS", COLLISION_GROUP_BREAKABLE_GLASS);
	CollisionGroup.value("VEHICLE", COLLISION_GROUP_VEHICLE);
	CollisionGroup.value("PLAYER_MOVEMENT", COLLISION_GROUP_PLAYER_MOVEMENT);
	CollisionGroup.value("NPC", COLLISION_GROUP_NPC);
	CollisionGroup.value("IN_VEHICLE", COLLISION_GROUP_IN_VEHICLE);
	CollisionGroup.value("WEAPON", COLLISION_GROUP_WEAPON);
	CollisionGroup.value("VEHICLE_CLIP", COLLISION_GROUP_VEHICLE_CLIP);
	CollisionGroup.value("PROJECTILE", COLLISION_GROUP_PROJECTILE);
	CollisionGroup.value("DOOR_BLOCKER", COLLISION_GROUP_DOOR_BLOCKER);
	CollisionGroup.value("PASSABLE_DOOR", COLLISION_GROUP_PASSABLE_DOOR);
	CollisionGroup.value("DISSOLVING", COLLISION_GROUP_DISSOLVING);
	CollisionGroup.value("PUSHAWAY", COLLISION_GROUP_PUSHAWAY);
	CollisionGroup.value("NPC_ACTOR", COLLISION_GROUP_NPC_ACTOR);
	CollisionGroup.value("NPC_SCRIPTED", COLLISION_GROUP_NPC_SCRIPTED);

	// CS:GO specific values...
	NOT_IMPLEMENTED_VALUE(Collision_Group_t, "PZ_CLIP");
	NOT_IMPLEMENTED_VALUE(Collision_Group_t, "DEBRIS_BLOCK_PROJECTILE");

	// Engine specific stuff...
	export_engine_specific_collision_group(CollisionGroup);
}


//-----------------------------------------------------------------------------
// Expose dissolve types.
//-----------------------------------------------------------------------------
void export_dissolve_types(scope _constants)
{
	_constants.attr("ENTITY_DISSOLVE_NORMAL") = (int)ENTITY_DISSOLVE_NORMAL;
	_constants.attr("ENTITY_DISSOLVE_ELECTRICAL") = (int)ENTITY_DISSOLVE_ELECTRICAL;
	_constants.attr("ENTITY_DISSOLVE_ELECTRICAL_LIGHT") = (int)ENTITY_DISSOLVE_ELECTRICAL_LIGHT;
	_constants.attr("ENTITY_DISSOLVE_CORE") = (int)ENTITY_DISSOLVE_CORE;
}


//-----------------------------------------------------------------------------
// Expose take damage constants.
//-----------------------------------------------------------------------------
void export_take_damage(scope _constants)
{
	_constants.attr("DAMAGE_NO") = DAMAGE_NO;
	_constants.attr("DAMAGE_EVENTS_ONLY") = DAMAGE_EVENTS_ONLY;
	_constants.attr("DAMAGE_YES") = DAMAGE_YES;
	_constants.attr("DAMAGE_AIM") = DAMAGE_AIM;
}


//-----------------------------------------------------------------------------
// Expose entity flags.
//-----------------------------------------------------------------------------
void export_entity_flags(scope _constants)
{
	_constants.attr("EFL_KILLME") = (long)EFL_KILLME;
	_constants.attr("EFL_DORMANT") = (long)EFL_DORMANT;
	_constants.attr("EFL_NOCLIP_ACTIVE") = (long)EFL_NOCLIP_ACTIVE;
	_constants.attr("EFL_SETTING_UP_BONES") = (long)EFL_SETTING_UP_BONES;
	_constants.attr("EFL_KEEP_ON_RECREATE_ENTITIES") = (long)EFL_KEEP_ON_RECREATE_ENTITIES;
	_constants.attr("EFL_DIRTY_SHADOWUPDATE") = (long)EFL_DIRTY_SHADOWUPDATE;
	_constants.attr("EFL_NOTIFY") = (long)EFL_NOTIFY;
	_constants.attr("EFL_FORCE_CHECK_TRANSMIT") = (long)EFL_FORCE_CHECK_TRANSMIT;
	_constants.attr("EFL_BOT_FROZEN") = (long)EFL_BOT_FROZEN;
	_constants.attr("EFL_SERVER_ONLY") = (long)EFL_SERVER_ONLY;
	_constants.attr("EFL_NO_AUTO_EDICT_ATTACH") = (long)EFL_NO_AUTO_EDICT_ATTACH;
	_constants.attr("EFL_DIRTY_ABSTRANSFORM") = (long)EFL_DIRTY_ABSTRANSFORM;
	_constants.attr("EFL_DIRTY_ABSVELOCITY") = (long)EFL_DIRTY_ABSVELOCITY;
	_constants.attr("EFL_DIRTY_ABSANGVELOCITY") = (long)EFL_DIRTY_ABSANGVELOCITY;
	_constants.attr("EFL_DIRTY_SURROUNDING_COLLISION_BOUNDS") = (long)EFL_DIRTY_SURROUNDING_COLLISION_BOUNDS;
	_constants.attr("EFL_DIRTY_SPATIAL_PARTITION") = (long)EFL_DIRTY_SPATIAL_PARTITION;
	_constants.attr("EFL_HAS_PLAYER_CHILD") = (long)EFL_HAS_PLAYER_CHILD;
	_constants.attr("EFL_IN_SKYBOX") = (long)EFL_IN_SKYBOX;
	_constants.attr("EFL_USE_PARTITION_WHEN_NOT_SOLID") = (long)EFL_USE_PARTITION_WHEN_NOT_SOLID;
	_constants.attr("EFL_TOUCHING_FLUID") = (long)EFL_TOUCHING_FLUID;
	_constants.attr("EFL_IS_BEING_LIFTED_BY_BARNACLE") = (long)EFL_IS_BEING_LIFTED_BY_BARNACLE;
	_constants.attr("EFL_NO_ROTORWASH_PUSH") = (long)EFL_NO_ROTORWASH_PUSH;
	_constants.attr("EFL_NO_THINK_FUNCTION") = (long)EFL_NO_THINK_FUNCTION;
	_constants.attr("EFL_NO_GAME_PHYSICS_SIMULATION") = (long)EFL_NO_GAME_PHYSICS_SIMULATION;
	_constants.attr("EFL_CHECK_UNTOUCH") = (long)EFL_CHECK_UNTOUCH;
	_constants.attr("EFL_DONTBLOCKLOS") = (long)EFL_DONTBLOCKLOS;
	_constants.attr("EFL_DONTWALKON") = (long)EFL_DONTWALKON;
	_constants.attr("EFL_NO_DISSOLVE") = (long)EFL_NO_DISSOLVE;
	_constants.attr("EFL_NO_MEGAPHYSCANNON_RAGDOLL") =(long)EFL_NO_MEGAPHYSCANNON_RAGDOLL;
	_constants.attr("EFL_NO_WATER_VELOCITY_CHANGE") = (long)EFL_NO_WATER_VELOCITY_CHANGE;
	_constants.attr("EFL_NO_PHYSCANNON_INTERACTION") = (long)EFL_NO_PHYSCANNON_INTERACTION;
	_constants.attr("EFL_NO_DAMAGE_FORCES") = (long)EFL_NO_DAMAGE_FORCES;
}
