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
#include "string_t.h"
#include "game/shared/itempents.h"
#include "game/shared/effect_dispatch_data.h"
#include "tools_wrap.h"
#include "modules/memory/memory_tools.h"

#include ENGINE_INCLUDE_PATH(tools_wrap_python.h)


//-----------------------------------------------------------------------------
// External variables.
//-----------------------------------------------------------------------------
extern IServerTools* servertools;


//-----------------------------------------------------------------------------
// Forward declarations.
//-----------------------------------------------------------------------------
void export_server_tools(scope tools_c);
void export_temp_entities_system(scope tools_c);


//-----------------------------------------------------------------------------
// Declare the tools_c module.
//-----------------------------------------------------------------------------
DECLARE_SP_MODULE(tools_c)
{
	scope tools_c = scope();
	export_server_tools(tools_c);
	export_temp_entities_system(tools_c);
}


//-----------------------------------------------------------------------------
// Expose IServerTools.
//-----------------------------------------------------------------------------
void export_server_tools(scope tools_c)
{
	class_<IServerTools, boost::noncopyable> ServerTools("_ServerTools", no_init);
	
	// Methods...
	ServerTools.def("create_entity", &ServerToolsSharedExt::create_entity);
	ServerTools.def("spawn_entity", &ServerToolsSharedExt::spawn_entity);
	
	// Engine specific stuff...
	export_engine_specific_server_tools(ServerTools);
	
	// Add memory tools...
	ServerTools ADD_MEM_TOOLS(IServerTools, "_ServerTools");
	
	// Singleton...
	tools_c.attr("ServerTools") = object(ptr(servertools));
}


//-----------------------------------------------------------------------------
// Expose ITempEntsSystem.
//-----------------------------------------------------------------------------
void export_temp_entities_system(scope tools_c)
{
	class_<ITempEntsSystem, boost::noncopyable> TempEntities("TempEntities", no_init);
	
	// Methods...
	TempEntities.def("armor_ricochet", &ITempEntsSystem::ArmorRicochet);
	TempEntities.def("beam_ent_point", &ITempEntsSystem::BeamEntPoint);
	TempEntities.def("beam_ents", &ITempEntsSystem::BeamEnts);
	TempEntities.def("beam_follow", &ITempEntsSystem::BeamFollow);
	TempEntities.def("beam_points", &ITempEntsSystem::BeamPoints);
	TempEntities.def("beam_laser", &ITempEntsSystem::BeamLaser);
	TempEntities.def("beam_ring", &ITempEntsSystem::BeamRing);
	TempEntities.def("beam_ring_point", &ITempEntsSystem::BeamRingPoint);
	TempEntities.def("beam_spline", &ITempEntsSystem::BeamSpline);
	TempEntities.def("blood_sprite", &ITempEntsSystem::BloodSprite);
	TempEntities.def("break_model", &ITempEntsSystem::BreakModel);
	TempEntities.def("bsp_decal", &ITempEntsSystem::BSPDecal);
	TempEntities.def("project_decal", &ITempEntsSystem::ProjectDecal);
	TempEntities.def("bubbles", &ITempEntsSystem::Bubbles);
	TempEntities.def("bubble_trail", &ITempEntsSystem::BubbleTrail);
	TempEntities.def("decal", &ITempEntsSystem::Decal);
	TempEntities.def("dynamic_light", &ITempEntsSystem::DynamicLight);
	TempEntities.def("explosion", &ITempEntsSystem::Explosion);
	TempEntities.def("shatter_surface", &ITempEntsSystem::ShatterSurface);
	TempEntities.def("glow_sprite", &ITempEntsSystem::GlowSprite);
	TempEntities.def("foot_print_decal", &ITempEntsSystem::FootprintDecal);
	TempEntities.def("kill_player_attachments", &ITempEntsSystem::KillPlayerAttachments);
	TempEntities.def("large_funnel", &ITempEntsSystem::LargeFunnel);
	TempEntities.def("metal_sparks", &ITempEntsSystem::MetalSparks);
	TempEntities.def("energy_splash", &ITempEntsSystem::EnergySplash);
	TempEntities.def("player_decal", &ITempEntsSystem::PlayerDecal);
	TempEntities.def("show_line", &ITempEntsSystem::ShowLine);
	TempEntities.def("smoke", &ITempEntsSystem::Smoke);
	TempEntities.def("sparks", &ITempEntsSystem::Sparks);
	TempEntities.def("sprite", &ITempEntsSystem::Sprite);
	TempEntities.def("sprite_spray", &ITempEntsSystem::SpriteSpray);
	TempEntities.def("world_decal", &ITempEntsSystem::WorldDecal);
	TempEntities.def("muzzle_flash", &ITempEntsSystem::MuzzleFlash);
	TempEntities.def("dust", &ITempEntsSystem::Dust);
	TempEntities.def("gauss_explosion", &ITempEntsSystem::GaussExplosion);
	TempEntities.def("physics_prop", &ITempEntsSystem::PhysicsProp);
	TempEntities.def("trigger_temp_entity", &ITempEntsSystem::TriggerTempEntity);
	
	TempEntities.def("fizz", &TempEntitiesSharedExt::fizz);
	TempEntities.def("client_projectile", &TempEntitiesSharedExt::client_projectile);

	// Engine specific stuff...
	export_engine_specific_temp_entities_system(TempEntities);
	
	// Add memory tools...
	TempEntities ADD_MEM_TOOLS(ITempEntsSystem, "TempEntities");
}
