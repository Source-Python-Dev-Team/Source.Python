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
#include "KeyValues.h"
#include "game/shared/itempents.h"
#include "game/shared/effect_dispatch_data.h"
#include "utility/wrap_macros.h"
#include "modules/export_main.h"
#include "modules/memory/memory_tools.h"
#include "effects_wrap.h"

#include ENGINE_INCLUDE_PATH(effects_wrap_python.h)


//-----------------------------------------------------------------------------
// Externals
//-----------------------------------------------------------------------------
extern IEffects* effects;

// This is required to fix a linker error
IPredictionSystem* IPredictionSystem::g_pPredictionSystems = NULL;

//-----------------------------------------------------------------------------
// Exposes the effects_c module.
//-----------------------------------------------------------------------------
void export_prediction_system();
void export_effects();
void export_temp_entities_system();
void export_dispatch_effect_data();

DECLARE_SP_MODULE(_effects)
{
	export_prediction_system();
	export_effects();
	export_temp_entities_system();
	export_dispatch_effect_data();
}


//-----------------------------------------------------------------------------
// Expose IPredictionSystem
//-----------------------------------------------------------------------------
void export_prediction_system()
{
	class_<IPredictionSystem>("PredictionSystem")
		.def("get_next",
			&IPredictionSystem::GetNext,
			"Returns the next prediction system.",
			reference_existing_object_policy()
		)

		.def("set_suppress_event",
			&IPredictionSystem::SetSuppressEvent,
			"Set the SuppressEvent state.",
			args("state")
		)
		
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

		ADD_MEM_TOOLS(IPredictionSystem, "PredictionSystem")
	;
}

//-----------------------------------------------------------------------------
// Expose IEffects
//-----------------------------------------------------------------------------
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(sparks_overload, Sparks, 1, 4)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(energy_splash_overload, EnergySplash, 2, 3)

void export_effects()
{
	class_<IEffects, bases<IPredictionSystem>, boost::noncopyable>("_Effects", no_init)
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

		ADD_MEM_TOOLS(IEffects, "_Effects")
	;

	scope().attr("effects") = object(ptr(effects));
}


//-----------------------------------------------------------------------------
// Expose ITempEntsSystem.
//-----------------------------------------------------------------------------
void export_temp_entities_system()
{
	class_<ITempEntsSystem, bases<IPredictionSystem>, boost::noncopyable> TempEntities("_TempEntities", no_init);

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
	TempEntities.def("blood_stream", &ITempEntsSystem::BloodStream);
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

	// OrangeBox methods...
	TempEntities.NOT_IMPLEMENTED("dispatch_effect");

	// Engine specific stuff...
	export_engine_specific_temp_entities_system(TempEntities);

	// Add memory tools...
	TempEntities ADD_MEM_TOOLS(ITempEntsSystem, "_TempEntities");
}


//-----------------------------------------------------------------------------
// Expose CEffectData.
//-----------------------------------------------------------------------------
void export_dispatch_effect_data()
{
	class_<CEffectData> DispatchEffectData("DispatchEffectData");
	
	// Properties...
	DispatchEffectData.def_readwrite("origin", &CEffectData::m_vOrigin);
	DispatchEffectData.def_readwrite("start", &CEffectData::m_vStart);
	DispatchEffectData.def_readwrite("normal", &CEffectData::m_vNormal);
	DispatchEffectData.def_readwrite("angles", &CEffectData::m_vAngles);
	DispatchEffectData.def_readwrite("flags", &CEffectData::m_fFlags);
	DispatchEffectData.def_readwrite("entity_index", &CEffectData::m_nEntIndex);
	DispatchEffectData.def_readwrite("scale", &CEffectData::m_flScale);
	DispatchEffectData.def_readwrite("magnitude", &CEffectData::m_flMagnitude);
	DispatchEffectData.def_readwrite("radius", &CEffectData::m_flRadius);
	DispatchEffectData.def_readwrite("attachement_index", &CEffectData::m_nAttachmentIndex);
	DispatchEffectData.def_readwrite("surface_prop", &CEffectData::m_nSurfaceProp);
	DispatchEffectData.def_readwrite("material", &CEffectData::m_nMaterial);
	DispatchEffectData.def_readwrite("damage_type", &CEffectData::m_nDamageType);
	DispatchEffectData.def_readwrite("hitbox", &CEffectData::m_nHitBox);
	DispatchEffectData.def_readwrite("color", &CEffectData::m_nColor);
	
	// CS:GO properties...
	DispatchEffectData.NOT_IMPLEMENTED_ATTR("other_entity_index");
	
	// Methods...
	DispatchEffectData.def("get_effect_name_index", &CEffectData::GetEffectNameIndex);
	
	// Engine specific stuff...
	export_engine_specific_dispatch_effect_data(DispatchEffectData);
	
	// Add memory tools...
	DispatchEffectData ADD_MEM_TOOLS(CEffectData, "DispatchEffectData");
}