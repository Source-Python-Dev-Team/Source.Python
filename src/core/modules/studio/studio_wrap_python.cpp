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
#include "export_main.h"
#include "utilities/wrap_macros.h"
#include "studio.h"
#include "modules/memory/memory_tools.h"
#include "studio_wrap.h"


//-----------------------------------------------------------------------------
// Forward declarations.
//-----------------------------------------------------------------------------
void export_studio_header(scope);
void export_studio_bone(scope);
void export_studio_bone_controller(scope);
void export_studio_hitbox_set(scope);


//-----------------------------------------------------------------------------
// Declare the _studio module.
//-----------------------------------------------------------------------------
DECLARE_SP_MODULE(_studio)
{
	export_studio_header(_studio);
	export_studio_bone(_studio);
	export_studio_bone_controller(_studio);
	export_studio_hitbox_set(_studio);
}


//-----------------------------------------------------------------------------
// Exports studiohdr_t.
//-----------------------------------------------------------------------------
void export_studio_header(scope _studio)
{
	class_<studiohdr_t, studiohdr_t *, boost::noncopyable> StudioHeader("StudioHeader", no_init);

	// Properties...
	StudioHeader.def_readwrite("id", &studiohdr_t::id); // Looks like a pointer?
	StudioHeader.def_readwrite("version", &studiohdr_t::version);
	StudioHeader.def_readwrite("checksum", &studiohdr_t::checksum);

	StudioHeader.add_property("name", &studiohdr_t::pszName); // TODO: Mutator...

	StudioHeader.add_property("eye_position", make_getter(&studiohdr_t::eyeposition, reference_existing_object_policy()));
	StudioHeader.add_property("illumination_center", make_getter(&studiohdr_t::illumposition, reference_existing_object_policy()));
	StudioHeader.add_property("hull_min", make_getter(&studiohdr_t::hull_min, reference_existing_object_policy()));
	StudioHeader.add_property("hull_max", make_getter(&studiohdr_t::hull_max, reference_existing_object_policy()));
	StudioHeader.add_property("view_min", make_getter(&studiohdr_t::view_bbmin, reference_existing_object_policy()));
	StudioHeader.add_property("view_max", make_getter(&studiohdr_t::view_bbmax, reference_existing_object_policy()));

	StudioHeader.def_readwrite("flags", &studiohdr_t::flags);
	StudioHeader.def_readwrite("num_bones", &studiohdr_t::numbones);
	StudioHeader.def_readwrite("num_controllers", &studiohdr_t::numbonecontrollers);
	StudioHeader.def_readwrite("num_hitbox_sets", &studiohdr_t::numhitboxsets);

	// Methods...
	StudioHeader.def("get_bone", &studiohdr_t::pBone, reference_existing_object_policy());
	StudioHeader.def("remap_sequence_bone", &studiohdr_t::RemapSeqBone);
	StudioHeader.def("remap_animation_bone", &studiohdr_t::RemapAnimBone);
	StudioHeader.def("get_bone_controller", &studiohdr_t::pBonecontroller, reference_existing_object_policy());
	StudioHeader.def("get_hitbox_set", &studiohdr_t::pHitboxSet, reference_existing_object_policy());

	// Special methods...
	StudioHeader.def("__init__", make_getter(&studiohdr_t::length));

	// Add memory tools...
	StudioHeader ADD_MEM_TOOLS(studiohdr_t, "StudioHeader");
}


//-----------------------------------------------------------------------------
// Exports mstudiobone_t.
//-----------------------------------------------------------------------------
void export_studio_bone(scope _studio)
{
	class_<mstudiobone_t, mstudiobone_t *, boost::noncopyable> StudioBone("StudioBone", no_init);

	// Properties...
	StudioBone.add_property("name", &StudioBoneExt::get_name); // TODO: Mutator...
	StudioBone.def_readwrite("parent", &mstudiobone_t::parent);

	StudioBone.add_property("position", make_getter(&mstudiobone_t::pos, reference_existing_object_policy()));
	StudioBone.add_property("quaternion", make_getter(&mstudiobone_t::quat, reference_existing_object_policy()));
	StudioBone.add_property("radiant_euler", make_getter(&mstudiobone_t::rot, reference_existing_object_policy()));
	StudioBone.add_property("position_scale", make_getter(&mstudiobone_t::posscale, reference_existing_object_policy()));
	StudioBone.add_property("radiant_euler_scale", make_getter(&mstudiobone_t::rotscale, reference_existing_object_policy()));

	StudioBone.add_property("position_to_bone", make_getter(&mstudiobone_t::poseToBone, reference_existing_object_policy())); // TODO: Export matrix3x4_t...

	StudioBone.add_property("alignment", make_getter(&mstudiobone_t::qAlignment, reference_existing_object_policy()));

	StudioBone.def_readwrite("flags", &mstudiobone_t::flags);
	StudioBone.def_readwrite("procedural_type", &mstudiobone_t::proctype);
	StudioBone.def_readwrite("physics_bone", &mstudiobone_t::physicsbone);
	StudioBone.def_readwrite("flags", &mstudiobone_t::flags);
	StudioBone.def_readwrite("contents", &mstudiobone_t::contents);

	// Methods...
	StudioBone.def("get_procedure", &StudioBoneExt::get_procedure, manage_new_object_policy());
	StudioBone.def("get_surface_name", &StudioBoneExt::get_surface_name);

	// Add memory tools...
	StudioBone ADD_MEM_TOOLS(mstudiobone_t, "StudioBone");
}


//-----------------------------------------------------------------------------
// Exports mstudiobonecontroller_t.
//-----------------------------------------------------------------------------
void export_studio_bone_controller(scope _studio)
{
	class_<mstudiobonecontroller_t, mstudiobonecontroller_t *> StudioBoneController("StudioBoneController");

	// Properties...
	StudioBoneController.def_readwrite("bone", &mstudiobonecontroller_t::bone);
	StudioBoneController.def_readwrite("type", &mstudiobonecontroller_t::type);
	StudioBoneController.def_readwrite("start", &mstudiobonecontroller_t::start);
	StudioBoneController.def_readwrite("end", &mstudiobonecontroller_t::end);
	StudioBoneController.def_readwrite("rest", &mstudiobonecontroller_t::rest);
	StudioBoneController.def_readwrite("input_field", &mstudiobonecontroller_t::inputfield);

	// Add memory tools...
	StudioBoneController ADD_MEM_TOOLS(mstudiobonecontroller_t, "StudioBoneController");
}


//-----------------------------------------------------------------------------
// Exports mstudiobonecontroller_t.
//-----------------------------------------------------------------------------
void export_studio_hitbox_set(scope _studio)
{
	class_<mstudiohitboxset_t, mstudiohitboxset_t *> StudioHitboxSet("StudioHitboxSet");

	// Properties...
	StudioHitboxSet.add_property("name", &StudioHitboxSetExt::get_name); // TODO: Mutator...
	StudioHitboxSet.def_readwrite("num_hitboxes", &mstudiohitboxset_t::numhitboxes);

	// Methods...
	StudioHitboxSet.def("get_hitbox", &mstudiohitboxset_t::pHitbox, reference_existing_object_policy());

	// Add memory tools...
	StudioHitboxSet ADD_MEM_TOOLS(mstudiohitboxset_t, "StudioHitboxSet");
}
