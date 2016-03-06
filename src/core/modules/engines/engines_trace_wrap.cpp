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

//---------------------------------------------------------------------------------
// Includes.
//---------------------------------------------------------------------------------
// Source.Python
#include "export_main.h"
#include "utilities/conversions.h"
#include "engines.h"
#include ENGINE_INCLUDE_PATH(engines_wrap.h)

// SDK
#include "engine/IEngineTrace.h"
#include "public/worldsize.h"


//---------------------------------------------------------------------------------
// External variables.
//---------------------------------------------------------------------------------
extern IEngineTrace* enginetrace;


//---------------------------------------------------------------------------------
// Forward declarations.
//---------------------------------------------------------------------------------
void export_ray_t(scope);
void export_engine_trace(scope);
void export_base_trace(scope);
void export_worldsize(scope);
void export_displacement_flags(scope);
void export_game_trace(scope);
void export_surface_t(scope);
void export_trace_filter(scope);
void export_entity_enumerator(scope);
void export_trace_type_t(scope);
void export_content_flags(scope);
void export_content_masks(scope);
void export_surface_flags(scope);


//---------------------------------------------------------------------------------
// Declare the _trace module.
//---------------------------------------------------------------------------------
DECLARE_SP_SUBMODULE(_engines, _trace)
{
	export_ray_t(_trace);
	export_engine_trace(_trace);
	export_base_trace(_trace);
	export_worldsize(_trace);
	export_displacement_flags(_trace);
	export_game_trace(_trace);
	export_surface_t(_trace);
	export_trace_filter(_trace);
	export_entity_enumerator(_trace);
	export_trace_type_t(_trace);

	// Sucks that we can't use enum for content flags and masks. They are too big
	// and crash the server
	export_content_flags(_trace);
	export_content_masks(_trace);
	export_surface_flags(_trace);
}


//---------------------------------------------------------------------------------
// Exports Ray_t.
//---------------------------------------------------------------------------------
void export_ray_t(scope _trace)
{
	// Since Ray_t has members of the type AlignedVector that uses ALIGN16, we have
	// to declare this class as noncopyable.
	class_<Ray_t, boost::noncopyable>("Ray", no_init)
		.def("__init__", make_constructor(&Ray_tExt::CreateRay1))
		.def("__init__", make_constructor(&Ray_tExt::CreateRay2))

		ADD_MEM_TOOLS(Ray_t)
	;
}


//---------------------------------------------------------------------------------
// Exports IEngineTrace.
//---------------------------------------------------------------------------------
void export_engine_trace(scope _trace)
{
	class_<IEngineTrace, boost::noncopyable>("_EngineTrace", no_init)
		.def("get_point_contents",
			&IEngineTraceExt::GetPointContents,
			"Returns the contents mask and the entities at the given position.",
			args("position")
		)

		.def("get_point_contents_of_collideable",
			&IEngineTrace::GetPointContents_Collideable,
			"Returns the content mask, but only tests the given entity.",
			args("entity", "position")
		)

		.def("clip_ray_to_entity",
			&IEngineTrace::ClipRayToEntity,
			"Traces a ray against a particular entity.",
			args("ray", "mask", "entity", "trace")
		)

		.def("clip_ray_to_collideable",
			&IEngineTrace::ClipRayToCollideable,
			"Traces a ray against a particular entity.",
			args("ray", "mask", "entity", "trace")
		)

		.def("trace_ray",
			&IEngineTrace::TraceRay,
			"A version that simply accepts a ray (can work as a traceline or tracehull).",
			args("ray", "mask", "filter", "trace")
		)

		.def("enumerate_entities",
			GET_METHOD(void, IEngineTrace, EnumerateEntities, const Ray_t&, bool, IEntityEnumerator*),
			"Enumerates over all entities along a ray.",
			args("ray", "triggers", "enumerator")
		)

		.def("enumerate_entities_in_box",
			&IEngineTraceExt::EnumerateEntitiesInBox,
			"Enumerates over all entities within a box.",
			args("p1", "p2", "enumerator")
		)

		.def("get_collideable",
			&IEngineTrace::GetCollideable,
			"Convert a handle entity to a collideable.",
			args("entity"),
			reference_existing_object_policy()
		)

		.def("is_point_outside_of_world",
			&IEngineTrace::PointOutsideWorld,
			"Tests a point to see if it's outside any playable area.",
			args("position")
		)

		.def("sweep_collideable",
			&IEngineTrace::SweepCollideable,
			"Sweeps a collideable through the world.",
			args("collideable", "start", "end", "angles", "mask", "filter", "trace")
		)

		/*
		//finds brushes in an AABB, prone to some false positives
		virtual void GetBrushesInAABB( const Vector &vMins, const Vector &vMaxs, CUtlVector<int> *pOutput, int iContentsMask = 0xFFFFFFFF ) = 0;

		//retrieve brush planes and contents, returns true if data is being returned in the output pointers, false if the brush doesn't exist
		virtual bool GetBrushInfo( int iBrush, CUtlVector<Vector4D> *pPlanesOut, int *pContentsOut ) = 0;
		*/

		ADD_MEM_TOOLS(IEngineTrace)
	;

	_trace.attr("engine_trace") = object(ptr(enginetrace));
}


//-----------------------------------------------------------------------------
// Exports CBaseTrace.
//-----------------------------------------------------------------------------
void export_base_trace(scope _trace)
{
	class_<CBaseTrace, boost::noncopyable>("BaseTrace")
		.def_readwrite("start_position",
			&CBaseTrace::startpos
		)

		.def_readwrite("end_position",
			&CBaseTrace::endpos
		)

		.def_readwrite("plane",
			&CBaseTrace::plane
		)

		.def_readwrite("fraction",
			&CBaseTrace::fraction
		)

		.def_readwrite("contents",
			&CBaseTrace::contents
		)

		.def_readwrite("displacement_flags",
			&CBaseTrace::dispFlags
		)

		.def_readwrite("all_solid",
			&CBaseTrace::allsolid
		)

		.def_readwrite("start_solid",
			&CBaseTrace::startsolid
		)

		ADD_MEM_TOOLS(CBaseTrace)
	;
}


//-----------------------------------------------------------------------------
// Exports world size constants.
//-----------------------------------------------------------------------------
void export_worldsize(scope _trace)
{
	_trace.attr("MAX_COORD_INTEGER") = MAX_COORD_INTEGER;
	_trace.attr("MIN_COORD_INTEGER") = MIN_COORD_INTEGER;

	_trace.attr("MAX_COORD_FRACTION") = MAX_COORD_FRACTION;
	_trace.attr("MIN_COORD_FRACTION") = MIN_COORD_FRACTION;

	_trace.attr("MAX_COORD_FLOAT") = MAX_COORD_FLOAT;
	_trace.attr("MIN_COORD_FLOAT") = MIN_COORD_FLOAT;

	_trace.attr("COORD_EXTENT") = COORD_EXTENT;

	_trace.attr("MAX_TRACE_LENGTH") = MAX_TRACE_LENGTH;

	_trace.attr("MAX_COORD_RANGE") = MAX_COORD_RANGE;
}


//-----------------------------------------------------------------------------
// Exports displacement flags
//-----------------------------------------------------------------------------
void export_displacement_flags(scope _trace)
{	
	enum DisplacementFlags {};

	enum_<DisplacementFlags> _DisplacementFlags("DisplacementFlags");
	
	_DisplacementFlags.value("SURFACE", (DisplacementFlags) DISPSURF_FLAG_SURFACE);
	_DisplacementFlags.value("WALKABLE", (DisplacementFlags) DISPSURF_FLAG_WALKABLE);
	_DisplacementFlags.value("BUILDABLE", (DisplacementFlags) DISPSURF_FLAG_BUILDABLE);
	_DisplacementFlags.value("SURFPROP1", (DisplacementFlags) DISPSURF_FLAG_SURFPROP1);
	_DisplacementFlags.value("SURFPROP2", (DisplacementFlags) DISPSURF_FLAG_SURFPROP2);
}


//-----------------------------------------------------------------------------
// Exports CGameTrace
//-----------------------------------------------------------------------------
void export_game_trace(scope _trace)
{
	class_<CGameTrace, bases<CBaseTrace>, boost::noncopyable>("GameTrace")
		.def("did_hit_world",
			&CGameTrace::DidHitWorld,
			"Returns True if the ray hit the world entity."
		)

		.def("did_hit",
			&CGameTrace::DidHit,
			"Returns true if there was any kind of impact at all"
		)

		.add_property("entity_index",
			&CGameTrace::GetEntityIndex,
			"Returns the index of the entity that was hit."
		)

		.def_readwrite("fraction_left_solid",
			&CGameTrace::fractionleftsolid
		)

		.def_readwrite("surface",
			&CGameTrace::surface
		)

		.def_readwrite("hitgroup",
			&CGameTrace::hitgroup
		)

		.def_readwrite("physicsbone",
			&CGameTrace::physicsbone
		)

		.def_readwrite("entity",
			&CGameTrace::m_pEnt
		)

		.def_readwrite("hitbox",
			&CGameTrace::hitbox
		)

		ADD_MEM_TOOLS(CGameTrace)
	;
}


//-----------------------------------------------------------------------------
// Exports csurface_t
//-----------------------------------------------------------------------------
void export_surface_t(scope _trace)
{	
	class_<csurface_t>("Surface")
		.def_readwrite("name",
			&csurface_t::name
		)

		.def_readwrite("surface_props",
			&csurface_t::surfaceProps
		)

		.def_readwrite("flags",
			&csurface_t::flags
		)

		ADD_MEM_TOOLS(csurface_t)
	;
}


//-----------------------------------------------------------------------------
// Exports csurface_t
//-----------------------------------------------------------------------------
void export_trace_filter(scope _trace)
{	
	// Trace filter baseclass
	class_<ITraceFilterWrap, boost::noncopyable>("TraceFilter")
		.def("should_hit_entity",
			pure_virtual(&ITraceFilterWrap::ShouldHitEntity),
			"Returns True if the a should hit the entity."
		)

		.def("get_trace_type",
			pure_virtual(&ITraceFilterWrap::GetTraceType),
			"Returns the trace type."
		)

		ADD_MEM_TOOLS_WRAPPER(ITraceFilterWrap, ITraceFilter)
	;
}


//-----------------------------------------------------------------------------
// Exports csurface_t
//-----------------------------------------------------------------------------
void export_entity_enumerator(scope _trace)
{	
	// Enumerator baseclass
	class_<IEntityEnumeratorWrap, boost::noncopyable>("EntityEnumerator")
		.def("enum_entity",
			pure_virtual(&IEntityEnumeratorWrap::EnumEntity),
			"Gets called with each handle."
		)

		ADD_MEM_TOOLS_WRAPPER(IEntityEnumeratorWrap, IEntityEnumerator)
	;
}


//-----------------------------------------------------------------------------
// Exports TraceType_t
//-----------------------------------------------------------------------------
void export_trace_type_t(scope _trace)
{	
	// Trace types
	enum_<TraceType_t>("TraceType")
		.value("EVERYTHING", TRACE_EVERYTHING)
		.value("WORLD_ONLY", TRACE_WORLD_ONLY)
		.value("ENTITIES_ONLY", TRACE_ENTITIES_ONLY)
		.value("EVERYTHING_FILTER_PROPS", TRACE_EVERYTHING_FILTER_PROPS)
	;
}


//-----------------------------------------------------------------------------
// Exports content flags
//-----------------------------------------------------------------------------
void export_content_flags(scope _trace)
{
	_trace.attr("CONTENTS_EMPTY") = CONTENTS_EMPTY;
	_trace.attr("CONTENTS_SOLID") = CONTENTS_SOLID;
	_trace.attr("CONTENTS_WINDOW") = CONTENTS_WINDOW;
	_trace.attr("CONTENTS_AUX") = CONTENTS_AUX;
	_trace.attr("CONTENTS_GRATE") = CONTENTS_GRATE;
	_trace.attr("CONTENTS_SLIME") = CONTENTS_SLIME;
	_trace.attr("CONTENTS_WATER") = CONTENTS_WATER;
	_trace.attr("CONTENTS_BLOCKLOS") = CONTENTS_BLOCKLOS;
	_trace.attr("CONTENTS_OPAQUE") = CONTENTS_OPAQUE;
	_trace.attr("LAST_VISIBLE_CONTENTS") = LAST_VISIBLE_CONTENTS;
	_trace.attr("ALL_VISIBLE_CONTENTS") = ALL_VISIBLE_CONTENTS;
	_trace.attr("CONTENTS_TESTFOGVOLUME") = CONTENTS_TESTFOGVOLUME;
	_trace.attr("CONTENTS_UNUSED") = CONTENTS_UNUSED;
	_trace.attr("CONTENTS_TEAM1") = CONTENTS_TEAM1;
	_trace.attr("CONTENTS_TEAM2") = CONTENTS_TEAM2;
	_trace.attr("CONTENTS_IGNORE_NODRAW_OPAQUE") = CONTENTS_IGNORE_NODRAW_OPAQUE;
	_trace.attr("CONTENTS_MOVEABLE") = CONTENTS_MOVEABLE;
	_trace.attr("CONTENTS_AREAPORTAL") = CONTENTS_AREAPORTAL;
	_trace.attr("CONTENTS_PLAYERCLIP") = CONTENTS_PLAYERCLIP;
	_trace.attr("CONTENTS_MONSTERCLIP") = CONTENTS_MONSTERCLIP;
	_trace.attr("CONTENTS_CURRENT_0") = CONTENTS_CURRENT_0;
	_trace.attr("CONTENTS_CURRENT_90") = CONTENTS_CURRENT_90;
	_trace.attr("CONTENTS_CURRENT_180") = CONTENTS_CURRENT_180;
	_trace.attr("CONTENTS_CURRENT_270") = CONTENTS_CURRENT_270;
	_trace.attr("CONTENTS_CURRENT_UP") = CONTENTS_CURRENT_UP;
	_trace.attr("CONTENTS_CURRENT_DOWN") = CONTENTS_CURRENT_DOWN;
	_trace.attr("CONTENTS_ORIGIN") = CONTENTS_ORIGIN;
	_trace.attr("CONTENTS_MONSTER") = CONTENTS_MONSTER;
	_trace.attr("CONTENTS_DEBRIS") = CONTENTS_DEBRIS;
	_trace.attr("CONTENTS_DETAIL") = CONTENTS_DETAIL;
	_trace.attr("CONTENTS_TRANSLUCENT") = CONTENTS_TRANSLUCENT;
	_trace.attr("CONTENTS_LADDER") = CONTENTS_LADDER;
	_trace.attr("CONTENTS_HITBOX") = CONTENTS_HITBOX;
}


//-----------------------------------------------------------------------------
// Exports content masks
//-----------------------------------------------------------------------------
void export_content_masks(scope _trace)
{	
	_trace.attr("MASK_ALL") = MASK_ALL;
	_trace.attr("MASK_SOLID") = MASK_SOLID;
	_trace.attr("MASK_PLAYERSOLID") = MASK_PLAYERSOLID;
	_trace.attr("MASK_NPCSOLID") = MASK_NPCSOLID;
	_trace.attr("MASK_WATER") = MASK_WATER;
	_trace.attr("MASK_OPAQUE") = MASK_OPAQUE;
	_trace.attr("MASK_OPAQUE_AND_NPCS") = MASK_OPAQUE_AND_NPCS;
	_trace.attr("MASK_BLOCKLOS") = MASK_BLOCKLOS;
	_trace.attr("MASK_BLOCKLOS_AND_NPCS") = MASK_BLOCKLOS_AND_NPCS;
	_trace.attr("MASK_VISIBLE") = MASK_VISIBLE;
	_trace.attr("MASK_VISIBLE_AND_NPCS") = MASK_VISIBLE_AND_NPCS;
	_trace.attr("MASK_SHOT") = MASK_SHOT;
	_trace.attr("MASK_SHOT_HULL") = MASK_SHOT_HULL;
	_trace.attr("MASK_SHOT_PORTAL") = MASK_SHOT_PORTAL;
	_trace.attr("MASK_SOLID_BRUSHONLY") = MASK_SOLID_BRUSHONLY;
	_trace.attr("MASK_PLAYERSOLID_BRUSHONLY") = MASK_PLAYERSOLID_BRUSHONLY;
	_trace.attr("MASK_NPCWORLDSTATIC") = MASK_NPCWORLDSTATIC;
	_trace.attr("MASK_SPLITAREAPORTAL") = MASK_SPLITAREAPORTAL;
	_trace.attr("MASK_CURRENT") = MASK_CURRENT;
	_trace.attr("MASK_DEADSOLID") = MASK_DEADSOLID;
}



//-----------------------------------------------------------------------------
// Exports surface flags
//-----------------------------------------------------------------------------
void export_surface_flags(scope _trace)
{	
	enum SurfaceFlags {};

	enum_<SurfaceFlags> _SurfaceFlags("SurfaceFlags");
	
	_SurfaceFlags.value("LIGHT", (SurfaceFlags) SURF_LIGHT);
	_SurfaceFlags.value("SKY2D", (SurfaceFlags) SURF_SKY2D);
	_SurfaceFlags.value("SKY", (SurfaceFlags) SURF_SKY);
	_SurfaceFlags.value("WARP", (SurfaceFlags) SURF_WARP);
	_SurfaceFlags.value("TRANS", (SurfaceFlags) SURF_TRANS);
	_SurfaceFlags.value("NOPORTAL", (SurfaceFlags) SURF_NOPORTAL);
	_SurfaceFlags.value("TRIGGER", (SurfaceFlags) SURF_TRIGGER);
	_SurfaceFlags.value("NODRAW", (SurfaceFlags) SURF_NODRAW);
	_SurfaceFlags.value("HINT", (SurfaceFlags) SURF_HINT);
	_SurfaceFlags.value("SKIP", (SurfaceFlags) SURF_SKIP);
	_SurfaceFlags.value("NOLIGHT", (SurfaceFlags) SURF_NOLIGHT);
	_SurfaceFlags.value("BUMPLIGHT", (SurfaceFlags) SURF_BUMPLIGHT);
	_SurfaceFlags.value("NOSHADOWS", (SurfaceFlags) SURF_NOSHADOWS);
	_SurfaceFlags.value("NODECALS", (SurfaceFlags) SURF_NODECALS);
	_SurfaceFlags.value("NOCHOP", (SurfaceFlags) SURF_NOCHOP);
	_SurfaceFlags.value("HITBOX", (SurfaceFlags) SURF_HITBOX);
}