/**
* =============================================================================
* Source Python
* Copyright (C) 2012-2015 Source Python Development Team.  All rights reserved.
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
// These includes are really important. Be careful if you try to change the
// order or remove an include!
#include "utilities/wrap_macros.h"
#include "export_main.h"
#include "modules/memory/memory_tools.h"
#include "utilities/baseentity.h"
#include "mathlib/vector.h"
#include <stddef.h>
#include "wchartypes.h"
#include "string_t.h"
#include "shake.h"
#include "IEffects.h"
#include "KeyValues.h"
#include "game/shared/itempents.h"
#include "game/shared/effect_dispatch_data.h"

#include ENGINE_INCLUDE_PATH(effects_wrap.h)

BOOST_PYTHON_OPAQUE_SPECIALIZED_TYPE_ID(CBaseEntity)


//-----------------------------------------------------------------------------
// Forward declarations.
//-----------------------------------------------------------------------------
void export_shatter_surface(scope);
void export_prediction_system(scope);
void export_temp_entities_system(scope);
void export_dispatch_effect_data(scope);


//-----------------------------------------------------------------------------
// Declare the _effects module.
//-----------------------------------------------------------------------------
DECLARE_SP_MODULE(_effects)
{
	export_shatter_surface(_effects);
	export_prediction_system(_effects);
	export_temp_entities_system(_effects);
	export_dispatch_effect_data(_effects);
}


//-----------------------------------------------------------------------------
// Exports ShatterSurface_t.
//-----------------------------------------------------------------------------
void export_shatter_surface(scope _effects)
{
	enum_<ShatterSurface_t>("ShatterSurface")
		.value("GLASS", SHATTERSURFACE_GLASS)
		.value("TILE", SHATTERSURFACE_TILE)
	;
}


//-----------------------------------------------------------------------------
// Exports IPredictionSystem.
//-----------------------------------------------------------------------------
void export_prediction_system(scope _effects)
{
	class_<IPredictionSystem>("PredictionSystem")
		.add_property("next",
			make_function(
				&IPredictionSystem::GetNext,
				reference_existing_object_policy()
			),
			"Returns the next prediction system."
		)

		.def("set_suppress_event",
			&IPredictionSystem::SetSuppressEvent,
			"Set the SuppressEvent state.",
			args("state")
		)
		
		.def("can_predict",
			&IPredictionSystem::CanPredict
		)

		.def("suppress_events",
			&IPredictionSystem::SuppressEvents
		)
		.staticmethod("suppress_events")

		ADD_MEM_TOOLS(IPredictionSystem)
	;

	BEGIN_CLASS_INFO(IPredictionSystem)
		FUNCTION_INFO(GetNext)
		FUNCTION_INFO(SetSuppressEvent)
		FUNCTION_INFO(CanPredict)
	END_CLASS_INFO()
}


//-----------------------------------------------------------------------------
// Exports ITempEntsSystem.
//-----------------------------------------------------------------------------
void export_temp_entities_system(scope _effects)
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
	TempEntities.def("footprint_decal", &ITempEntsSystem::FootprintDecal);
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
	TempEntities.def("fizz", &ITempEntsSystem::Fizz);
	TempEntities.def("client_projectile", &ITempEntsSystem::ClientProjectile);

	// OrangeBox methods...
	TempEntities.NOT_IMPLEMENTED("dispatch_effect");

	// Engine specific stuff...
	export_engine_specific_temp_entities_system(_effects, TempEntities);

	// Add memory tools...
	TempEntities ADD_MEM_TOOLS(ITempEntsSystem);

	BEGIN_CLASS_INFO(ITempEntsSystem)
		FUNCTION_INFO(ArmorRicochet)
		FUNCTION_INFO(BeamEntPoint)
		FUNCTION_INFO(BeamEnts)
		FUNCTION_INFO(BeamFollow)
		FUNCTION_INFO(BeamPoints)
		FUNCTION_INFO(BeamLaser)
		FUNCTION_INFO(BeamRing)
		FUNCTION_INFO(BeamRingPoint)
		FUNCTION_INFO(BeamSpline)
		FUNCTION_INFO(BloodStream)
		FUNCTION_INFO(BloodSprite)
		FUNCTION_INFO(BreakModel)
		FUNCTION_INFO(BSPDecal)
		FUNCTION_INFO(ProjectDecal)
		FUNCTION_INFO(Bubbles)
		FUNCTION_INFO(BubbleTrail)
		FUNCTION_INFO(Decal)
		FUNCTION_INFO(DynamicLight)
		FUNCTION_INFO(Explosion)
		FUNCTION_INFO(ShatterSurface)
		FUNCTION_INFO(GlowSprite)
		FUNCTION_INFO(FootprintDecal)
		FUNCTION_INFO(KillPlayerAttachments)
		FUNCTION_INFO(LargeFunnel)
		FUNCTION_INFO(MetalSparks)
		FUNCTION_INFO(EnergySplash)
		FUNCTION_INFO(PlayerDecal)
		FUNCTION_INFO(ShowLine)
		FUNCTION_INFO(Smoke)
		FUNCTION_INFO(Sparks)
		FUNCTION_INFO(Sprite)
		FUNCTION_INFO(SpriteSpray)
		FUNCTION_INFO(WorldDecal)
		FUNCTION_INFO(MuzzleFlash)
		FUNCTION_INFO(Dust)
		FUNCTION_INFO(GaussExplosion)
		FUNCTION_INFO(PhysicsProp)
		FUNCTION_INFO(TriggerTempEntity)
		FUNCTION_INFO(Fizz)
		FUNCTION_INFO(ClientProjectile)
	END_CLASS_INFO()
}


//-----------------------------------------------------------------------------
// Exports CEffectData.
//-----------------------------------------------------------------------------
void export_dispatch_effect_data(scope _effects)
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
	DispatchEffectData.add_property("effect_name_index", &CEffectData::GetEffectNameIndex);
	
	// Engine specific stuff...
	export_engine_specific_dispatch_effect_data(_effects, DispatchEffectData);
	
	// Add memory tools...
	DispatchEffectData ADD_MEM_TOOLS(CEffectData);
}
