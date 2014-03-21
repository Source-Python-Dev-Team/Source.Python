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

#ifndef _CONSTANTS_CONST_WRAP_PYTHON_H
#define _CONSTANTS_CONST_WRAP_PYTHON_H

//-----------------------------------------------------------------------------
// Includes.
//-----------------------------------------------------------------------------
#include "constants_const_wrap.h"
#include ENGINE_INCLUDE_PATH(constants_const_wrap_python.h)


//-----------------------------------------------------------------------------
// Forward declarations.
//-----------------------------------------------------------------------------
void export_player_flags();
void export_entity_flags();
void export_movement_types();
void export_movement_collide();
void export_solid_types();
void export_solid_flags();
void export_life_states();
void export_entity_effects();
void export_render_modes();
void export_render_effects();
void export_collision_groups();


//-----------------------------------------------------------------------------
// Expose ../public/const.h.
//-----------------------------------------------------------------------------
void export_const(scope constants_c)
{
	// Global variables...
	constants_c.attr("STEAM_PARAM") = STEAM_PARM;
	constants_c.attr("AUTO_RESTART") = AUTO_RESTART;
	constants_c.attr("INVALID_STEAM_TICKET") = INVALID_STEAM_TICKET;
	constants_c.attr("INVALID_STEAM_LOGON") = INVALID_STEAM_LOGON;
	constants_c.attr("INVALID_STEAM_VACBANSTATE") = INVALID_STEAM_VACBANSTATE;
	constants_c.attr("INVALID_STEAM_LOGGED_IN_ELSEWHERE") = INVALID_STEAM_LOGGED_IN_ELSEWHERE;
	constants_c.attr("DEFAULT_TICK_INTERVAL") = DEFAULT_TICK_INTERVAL;
	constants_c.attr("MINIMUM_TICK_INTERVAL") = MINIMUM_TICK_INTERVAL;
	constants_c.attr("MAXIMUM_TICK_INTERVAL") = MAXIMUM_TICK_INTERVAL;
	constants_c.attr("ABSOLUTE_PLAYER_LIMIT") = ABSOLUTE_PLAYER_LIMIT;
	constants_c.attr("ABSOLUTE_PLAYER_LIMIT_DW") = ABSOLUTE_PLAYER_LIMIT_DW;
	constants_c.attr("MAX_PLAYER_NAME_LENGTH") = MAX_PLAYER_NAME_LENGTH;
	constants_c.attr("MAX_PLAYERS_PER_CLIENT") = MAX_PLAYERS_PER_CLIENT;
	constants_c.attr("MAX_MAP_NAME") = MAX_MAP_NAME;
	constants_c.attr("MAX_NETWORKID_LENGTH") = MAX_NETWORKID_LENGTH;
	constants_c.attr("SP_MODEL_INDEX_BITS") = SP_MODEL_INDEX_BITS;
	constants_c.attr("MAX_EDICT_BITS") = MAX_EDICT_BITS;
	constants_c.attr("MAX_EDICTS") = MAX_EDICTS;
	constants_c.attr("MAX_SERVER_CLASS_BITS") = MAX_SERVER_CLASS_BITS;
	constants_c.attr("MAX_SERVER_CLASSES") = MAX_SERVER_CLASSES;
	constants_c.attr("SIGNED_GUID_LEN") = SIGNED_GUID_LEN;
	constants_c.attr("NUM_ENT_ENTRY_BITS") = NUM_ENT_ENTRY_BITS;
	constants_c.attr("NUM_ENT_ENTRIES") = NUM_ENT_ENTRIES;
	constants_c.attr("ENT_ENTRY_MASK") = ENT_ENTRY_MASK;
	constants_c.attr("INVALID_EHANDLE_INDEX") = INVALID_EHANDLE_INDEX;
	constants_c.attr("NUM_SERIAL_NUM_BITS") = NUM_SERIAL_NUM_BITS;
	constants_c.attr("NUM_NETWORKED_EHANDLE_SERIAL_NUMBER_BITS") = NUM_NETWORKED_EHANDLE_SERIAL_NUMBER_BITS;
	constants_c.attr("NUM_NETWORKED_EHANDLE_BITS") = NUM_NETWORKED_EHANDLE_BITS;
	constants_c.attr("INVALID_NETWORKED_EHANDLE_VALUE") = INVALID_NETWORKED_EHANDLE_VALUE;
	constants_c.attr("MAX_PACKEDENTITY_DATA") = MAX_PACKEDENTITY_DATA;
	constants_c.attr("MAX_PACKEDENTITY_PROPS") = MAX_PACKEDENTITY_PROPS;
	constants_c.attr("MAX_CUSTOM_FILES") = MAX_CUSTOM_FILES;
	constants_c.attr("MAX_CUSTOM_FILE_SIZE") = MAX_CUSTOM_FILE_SIZE;
	constants_c.attr("EF_PARITY_BITS") = EF_PARITY_BITS;
	constants_c.attr("EF_PARITY_MASK") = EF_PARITY_MASK;
	constants_c.attr("EF_MUZZLEFLASH_BITS") = EF_MUZZLEFLASH_BITS;
	constants_c.attr("PLAT_LOW_TRIGGER") = PLAT_LOW_TRIGGER;
	constants_c.attr("SF_TRAIN_WAIT_RETRIGGER") = SF_TRAIN_WAIT_RETRIGGER;
	constants_c.attr("SF_TRAIN_PASSABLE") = SF_TRAIN_PASSABLE;
	constants_c.attr("FIXANGLE_NONE") = FIXANGLE_NONE;
	constants_c.attr("FIXANGLE_ABSOLUTE") = FIXANGLE_ABSOLUTE;
	constants_c.attr("FIXANGLE_RELATIVE") = FIXANGLE_RELATIVE;
	
	// TODO: Wrap those?
	constants_c.attr("BREAK_GLASS") = BREAK_GLASS;
	constants_c.attr("BREAK_METAL") = BREAK_METAL;
	constants_c.attr("BREAK_FLESH") = BREAK_FLESH;
	constants_c.attr("BREAK_WOOD") = BREAK_WOOD;
	constants_c.attr("BREAK_SMOKE") = BREAK_SMOKE;
	constants_c.attr("BREAK_TRANS") = BREAK_TRANS;
	constants_c.attr("BREAK_CONCRETE") = BREAK_CONCRETE;
	constants_c.attr("BREAK_SLAVE") = BREAK_SLAVE;
	
	constants_c.attr("BOUNCE_GLASS") = BOUNCE_GLASS;
	constants_c.attr("BOUNCE_METAL") = BOUNCE_METAL;
	constants_c.attr("BOUNCE_FLESH") = BOUNCE_FLESH;
	constants_c.attr("BOUNCE_WOOD") = BOUNCE_WOOD;
	constants_c.attr("BOUNCE_SHRAP") = BOUNCE_SHRAP;
	constants_c.attr("BOUNCE_SHELL") = BOUNCE_SHELL;
	constants_c.attr("BOUNCE_CONCRETE") = BOUNCE_CONCRETE;
	constants_c.attr("BOUNCE_SHOTSHELL") = BOUNCE_SHOTSHELL;
	
	constants_c.attr("TE_BOUNCE_NULL") = TE_BOUNCE_NULL;
	constants_c.attr("TE_BOUNCE_SHELL") = TE_BOUNCE_SHELL;
	constants_c.attr("TE_BOUNCE_SHOTSHELL") = TE_BOUNCE_SHOTSHELL;
	
	constants_c.attr("MAX_USER_MSG_DATA") = MAX_USER_MSG_DATA;
	
	// Enumerators...
	export_player_flags();
	export_entity_flags();
	export_movement_types();
	export_movement_collide();
	export_solid_types();
	export_solid_flags();
	export_life_states();
	export_entity_effects();
	export_render_modes();
	export_render_effects();
	export_collision_groups();
}


//-----------------------------------------------------------------------------
// Expose PlayerFlags.
//-----------------------------------------------------------------------------
void export_player_flags()
{
	enum_<PlayerFlags> PlayerFlags("PlayerFlags");
	
	// Values...
	PlayerFlags.value("ONGROUND", ONGROUND);
	PlayerFlags.value("DUCKING", DUCKING);
	PlayerFlags.value("WATERJUMP", WATERJUMP);
	PlayerFlags.value("ONTRAIN", ONTRAIN);
	PlayerFlags.value("INRAIN", INRAIN);
	PlayerFlags.value("FROZEN", FROZEN);
	PlayerFlags.value("ATCONTROLS", ATCONTROLS);
	PlayerFlags.value("CLIENT", CLIENT);
	PlayerFlags.value("FAKECLIENT", FAKECLIENT);
	PlayerFlags.value("INWATER", INWATER);
}


//-----------------------------------------------------------------------------
// Expose EntityFlags.
//-----------------------------------------------------------------------------
void export_entity_flags()
{
	enum_<EntityFlags> EntityFlags("EntityFlags");
	
	// Values...
	EntityFlags.value("FLY", FLY);
	EntityFlags.value("SWIM", SWIM);
	EntityFlags.value("CONVEYOR", CONVEYOR);
	EntityFlags.value("NPC", NPC);
	EntityFlags.value("GODMODE", GODMODE);
	EntityFlags.value("NOTARGET", NOTARGET);
	EntityFlags.value("AIMTARGET", AIMTARGET);
	EntityFlags.value("PARTIALGROUND", PARTIALGROUND);
	EntityFlags.value("STATICPROP", STATICPROP);
	EntityFlags.value("GRAPHED", GRAPHED);
	EntityFlags.value("GRENADE", GRENADE);
	EntityFlags.value("STEPMOVEMENT", STEPMOVEMENT);
	EntityFlags.value("DONTTOUCH", DONTTOUCH);
	EntityFlags.value("BASEVELOCITY", BASEVELOCITY);
	EntityFlags.value("WORLDBRUSH", WORLDBRUSH);
	EntityFlags.value("OBJECT", OBJECT);
	EntityFlags.value("KILLME", KILLME);
	EntityFlags.value("ONFIRE", ONFIRE);
	EntityFlags.value("DISSOLVING", DISSOLVING);
	
	/* TODO: The server crashes if there is any value larger than (1 << 30) in a wrapped enumerator...
	EntityFlags.value("TRANSRAGDOLL", TRANSRAGDOLL);
	EntityFlags.value("UNBLOCKABLE_BY_PLAYER", UNBLOCKABLE_BY_PLAYER);*/
}


//-----------------------------------------------------------------------------
// Expose MoveType_t.
//-----------------------------------------------------------------------------
void export_movement_types()
{
	enum_<MoveType_t> MoveTypes("MoveTypes");
	
	// Values...
	MoveTypes.value("NONE", MOVETYPE_NONE);
	MoveTypes.value("ISOMETRIC", MOVETYPE_ISOMETRIC);
	MoveTypes.value("WALK", MOVETYPE_WALK);
	MoveTypes.value("STEP", MOVETYPE_STEP);
	MoveTypes.value("FLY", MOVETYPE_FLY);
	MoveTypes.value("FLYGRAVITY", MOVETYPE_FLYGRAVITY);
	MoveTypes.value("VPHYSICS", MOVETYPE_VPHYSICS); 
	MoveTypes.value("PUSH", MOVETYPE_PUSH);
	MoveTypes.value("NOCLIP", MOVETYPE_NOCLIP);
	MoveTypes.value("LADDER", MOVETYPE_LADDER);
	MoveTypes.value("OBSERVER", MOVETYPE_OBSERVER);
	MoveTypes.value("CUSTOM", MOVETYPE_CUSTOM);
}


//-----------------------------------------------------------------------------
// Expose MoveCollide_t.
//-----------------------------------------------------------------------------
void export_movement_collide()
{
	enum_<MoveCollide_t> MoveCollide("MoveCollide");
	
	// Values...
	MoveCollide.value("DEFAULT", MOVECOLLIDE_DEFAULT);
	MoveCollide.value("FLY_BOUNCE", MOVECOLLIDE_FLY_BOUNCE);
	MoveCollide.value("FLY_CUSTOM", MOVECOLLIDE_FLY_CUSTOM);
	MoveCollide.value("FLY_SLIDE", MOVECOLLIDE_FLY_SLIDE);
}


//-----------------------------------------------------------------------------
// Expose SolidType_t.
//-----------------------------------------------------------------------------
void export_solid_types()
{
	enum_<SolidType_t> SolidTypes("SolidTypes");
	
	// Values...
	SolidTypes.value("NONE", SOLID_NONE);
	SolidTypes.value("BSP", SOLID_BSP);
	SolidTypes.value("BBOX", SOLID_BBOX);
	SolidTypes.value("OBB", SOLID_OBB);
	SolidTypes.value("OBB_YAW", SOLID_OBB_YAW);
	SolidTypes.value("CUSTOM", SOLID_CUSTOM);
	SolidTypes.value("VPHYSICS", SOLID_VPHYSICS);
}


//-----------------------------------------------------------------------------
// Expose SolidFlags_t.
//-----------------------------------------------------------------------------
void export_solid_flags()
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
	
	// Engine specific stuff...
	export_engine_specific_solid_flags(SolidFlags);
}


//-----------------------------------------------------------------------------
// Expose LifeStates.
//-----------------------------------------------------------------------------
void export_life_states()
{
	enum_<LifeStates> LifeStates("LifeStates");
	
	// Values...
	LifeStates.value("ALIVE", ALIVE);
	LifeStates.value("DYING", DYING);
	LifeStates.value("DEAD", DEAD);
	LifeStates.value("RESPAWNABLE", RESPAWNABLE);
	LifeStates.value("DISCARD_BODY", DISCARD_BODY);
}


//-----------------------------------------------------------------------------
// Expose EntityEffects.
//-----------------------------------------------------------------------------
void export_entity_effects()
{
	enum_<EntityEffects> EntityEffects("EntityEffects");
	
	// Values...
	EntityEffects.value("BONEMERGE", BONEMERGE);
	EntityEffects.value("BRIGHTLIGHT", BRIGHTLIGHT);
	EntityEffects.value("DIMLIGHT", DIMLIGHT);
	EntityEffects.value("NOINTERP", NOINTERP);
	EntityEffects.value("NOSHADOW", NOSHADOW); 
	EntityEffects.value("NODRAW", NODRAW);
	EntityEffects.value("NORECEIVESHADOW", NORECEIVESHADOW);
	EntityEffects.value("BONEMERGE_FASTCULL", BONEMERGE_FASTCULL);
	EntityEffects.value("ITEM_BLINK", ITEM_BLINK);
	EntityEffects.value("PARENT_ANIMATES", PARENT_ANIMATES);
	
	// Engine specific stuff...
	export_engine_specific_entity_effects(EntityEffects);
}


//-----------------------------------------------------------------------------
// Expose RenderMode_t.
//-----------------------------------------------------------------------------
void export_render_modes()
{
	enum_<RenderMode_t> RenderModes("RenderModes");
	
	// Values...
	RenderModes.value("NORMAL", kRenderNormal);
	RenderModes.value("TRANS_COLOR", kRenderTransColor);
	RenderModes.value("TRANS_TEXTURE", kRenderTransTexture);
	RenderModes.value("GLOW", kRenderGlow);
	RenderModes.value("TRANS_ALPHA", kRenderTransAlpha);
	RenderModes.value("TRANS_ADD", kRenderTransAdd);
	RenderModes.value("ENVIRONMENTAL", kRenderEnvironmental);
	RenderModes.value("TRANS_ADD_FRAME_BLEND", kRenderTransAddFrameBlend);
	RenderModes.value("WORLD_GLOW", kRenderWorldGlow);
	RenderModes.value("NONE", kRenderNone);
}


//-----------------------------------------------------------------------------
// Expose RenderFx_t.
//-----------------------------------------------------------------------------
void export_render_effects()
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
	
	// Engine specific stuff...
	export_engine_specific_render_effects(RenderEffects);
}


//-----------------------------------------------------------------------------
// Expose Collision_Group_t.
//-----------------------------------------------------------------------------
void export_collision_groups()
{
	enum_<Collision_Group_t> CollisionGroups("CollisionGroups");
	
	// Values...
	CollisionGroups.value("NONE", COLLISION_GROUP_NONE);
	CollisionGroups.value("DEBRIS", COLLISION_GROUP_DEBRIS);
	CollisionGroups.value("DEBRIS_TRIGGER", COLLISION_GROUP_DEBRIS_TRIGGER);
	CollisionGroups.value("INTERACTIVE_DEBRIS", COLLISION_GROUP_INTERACTIVE_DEBRIS);
	CollisionGroups.value("INTERACTIVE", COLLISION_GROUP_INTERACTIVE);
	CollisionGroups.value("PLAYER", COLLISION_GROUP_PLAYER);
	CollisionGroups.value("BREAKABLE_GLASS", COLLISION_GROUP_BREAKABLE_GLASS);
	CollisionGroups.value("VEHICLE", COLLISION_GROUP_VEHICLE);
	CollisionGroups.value("PLAYER_MOVEMENT", COLLISION_GROUP_PLAYER_MOVEMENT);
	CollisionGroups.value("NPC", COLLISION_GROUP_NPC);
	CollisionGroups.value("IN_VEHICLE", COLLISION_GROUP_IN_VEHICLE);
	CollisionGroups.value("WEAPON", COLLISION_GROUP_WEAPON);
	CollisionGroups.value("VEHICLE_CLIP", COLLISION_GROUP_VEHICLE_CLIP);
	CollisionGroups.value("PROJECTILE", COLLISION_GROUP_PROJECTILE);
	CollisionGroups.value("DOOR_BLOCKER", COLLISION_GROUP_DOOR_BLOCKER);
	CollisionGroups.value("PASSABLE_DOOR", COLLISION_GROUP_PASSABLE_DOOR);
	CollisionGroups.value("DISSOLVING", COLLISION_GROUP_DISSOLVING);
	CollisionGroups.value("PUSHAWAY", COLLISION_GROUP_PUSHAWAY);
	CollisionGroups.value("NPC_ACTOR", COLLISION_GROUP_NPC_ACTOR);
	CollisionGroups.value("NPC_SCRIPTED", COLLISION_GROUP_NPC_SCRIPTED);
	
	// Engine specific stuff...
	export_engine_specific_collision_groups(CollisionGroups);
}


#endif // _CONSTANTS_CONST_WRAP_PYTHON_H
