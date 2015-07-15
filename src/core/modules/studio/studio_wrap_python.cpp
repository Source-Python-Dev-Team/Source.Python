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
#include "export_main.h"
#include "utilities/wrap_macros.h"
#include "studio.h"
#include "modules/memory/memory_tools.h"
#include "studio_wrap.h"


//-----------------------------------------------------------------------------
// Forward declarations.
//-----------------------------------------------------------------------------
void export_model_header(scope);
void export_bone(scope);
void export_bone_controller(scope);
void export_hitbox_set(scope);
void export_hitbox(scope);
void export_animation(scope);
void export_movement(scope);
void export_hierarchy(scope);
void export_anim_sections(scope);
void export_sequence(scope);
void export_sequence_event(scope);
void export_attachment(scope);


//-----------------------------------------------------------------------------
// Declare the _studio module.
//-----------------------------------------------------------------------------
DECLARE_SP_MODULE(_studio)
{
	export_model_header(_studio);
	export_bone(_studio);
	export_bone_controller(_studio);
	export_hitbox_set(_studio);
	export_hitbox(_studio);
	export_animation(_studio);
	export_movement(_studio);
	export_hierarchy(_studio);
	export_anim_sections(_studio);
	export_sequence(_studio);
	export_sequence_event(_studio);
	export_attachment(_studio);
}


//-----------------------------------------------------------------------------
// Exports studiohdr_t.
//-----------------------------------------------------------------------------
void export_model_header(scope _studio)
{
	class_<studiohdr_t, boost::shared_ptr<studiohdr_t>, boost::noncopyable> ModelHeader("ModelHeader", no_init);

	// Initializer...
	ModelHeader.def("__init__", make_constructor(&ModelHeaderExt::__init__,
			default_call_policies(),
			args("model_name")
		)
	);

	// Properties...
	ModelHeader.def_readwrite("id", &studiohdr_t::id); // Looks like a pointer?
	ModelHeader.def_readwrite("version", &studiohdr_t::version);
	ModelHeader.def_readwrite("checksum", &studiohdr_t::checksum);

	ModelHeader.add_property("name", &studiohdr_t::pszName);

	ModelHeader.add_property("eye_position", make_getter(&studiohdr_t::eyeposition, reference_existing_object_policy()));
	ModelHeader.add_property("illumination_center", make_getter(&studiohdr_t::illumposition, reference_existing_object_policy()));
	ModelHeader.add_property("hull_min", make_getter(&studiohdr_t::hull_min, reference_existing_object_policy()));
	ModelHeader.add_property("hull_max", make_getter(&studiohdr_t::hull_max, reference_existing_object_policy()));
	ModelHeader.add_property("view_min", make_getter(&studiohdr_t::view_bbmin, reference_existing_object_policy()));
	ModelHeader.add_property("view_max", make_getter(&studiohdr_t::view_bbmax, reference_existing_object_policy()));

	ModelHeader.def_readwrite("flags", &studiohdr_t::flags);
	ModelHeader.def_readwrite("bones_count", &studiohdr_t::numbones);
	ModelHeader.def_readwrite("bones_offset", &studiohdr_t::boneindex);
	ModelHeader.def_readwrite("bone_controllers_count", &studiohdr_t::numbonecontrollers);
	ModelHeader.def_readwrite("bone_controllers_offset", &studiohdr_t::bonecontrollerindex);
	ModelHeader.def_readwrite("hitbox_sets_count", &studiohdr_t::numhitboxsets);
	ModelHeader.def_readwrite("hitbox_sets_offset", &studiohdr_t::hitboxsetindex);
	ModelHeader.def_readwrite("local_animations_count", &studiohdr_t::numlocalanim);
	ModelHeader.def_readwrite("animations_offset", &studiohdr_t::localanimindex);
	ModelHeader.def_readwrite("attachments_count", &studiohdr_t::numlocalattachments);
	ModelHeader.def_readwrite("local_sequences_count", &studiohdr_t::numlocalseq);
	ModelHeader.def_readwrite("local_sequence_offset", &studiohdr_t::localseqindex);
	ModelHeader.def_readwrite("mass", &studiohdr_t::mass);
	ModelHeader.def_readwrite("contents", &studiohdr_t::contents);

	// Methods...
	ModelHeader.def("get_bone", &studiohdr_t::pBone, reference_existing_object_policy());
	// ModelHeader.def("remap_sequence_bone", &studiohdr_t::RemapSeqBone);
	// ModelHeader.def("remap_animation_bone", &studiohdr_t::RemapAnimBone);
	ModelHeader.def("get_bone_controller", &studiohdr_t::pBonecontroller, reference_existing_object_policy());
	ModelHeader.def("get_hitbox_set", &studiohdr_t::pHitboxSet, reference_existing_object_policy());
	ModelHeader.def("get_local_animation", &studiohdr_t::pLocalAnimdesc, reference_existing_object_policy());
	// ModelHeader.def("has_sequences_available", &studiohdr_t::SequencesAvailable);
	// ModelHeader.def("get_animation", &studiohdr_t::pAnimdesc, reference_existing_object_policy());
	// ModelHeader.def("get_sequences_count", &studiohdr_t::GetNumSeq);
	// ModelHeader.def("get_sequence", &studiohdr_t::pSeqdesc, reference_existing_object_policy());
	// ModelHeader.def("get_activity_list_version", &studiohdr_t::GetActivityListVersion);
	// ModelHeader.def("set_activity_list_version", &studiohdr_t::SetActivityListVersion);
	ModelHeader.def("get_attachment", &studiohdr_t::pLocalAttachment, reference_existing_object_policy());

	// Special methods...
	ModelHeader.def("__len__", make_getter(&studiohdr_t::length));

	// Add memory tools...
	ModelHeader ADD_MEM_TOOLS(studiohdr_t);
}


//-----------------------------------------------------------------------------
// Exports mstudiobone_t.
//-----------------------------------------------------------------------------
void export_bone(scope _studio)
{
	class_<mstudiobone_t, mstudiobone_t *, boost::noncopyable> ModelBone("ModelBone");

	// Properties...
	ModelBone.add_property("name", &ModelBoneExt::get_name);
	ModelBone.def_readwrite("parent", &mstudiobone_t::parent);

	ModelBone.add_property("position", make_getter(&mstudiobone_t::pos, reference_existing_object_policy()));
	ModelBone.add_property("quaternion", make_getter(&mstudiobone_t::quat, reference_existing_object_policy()));
	ModelBone.add_property("radiant_euler", make_getter(&mstudiobone_t::rot, reference_existing_object_policy()));
	ModelBone.add_property("position_scale", make_getter(&mstudiobone_t::posscale, reference_existing_object_policy()));
	ModelBone.add_property("radiant_euler_scale", make_getter(&mstudiobone_t::rotscale, reference_existing_object_policy()));

	ModelBone.add_property("position_to_bone", make_getter(&mstudiobone_t::poseToBone, reference_existing_object_policy())); // TODO: Export matrix3x4_t...

	ModelBone.add_property("alignment", make_getter(&mstudiobone_t::qAlignment, reference_existing_object_policy()));
	ModelBone.add_property("surface_name", &ModelBoneExt::get_surface_name);

	ModelBone.def_readwrite("flags", &mstudiobone_t::flags);
	ModelBone.def_readwrite("procedural_type", &mstudiobone_t::proctype);
	ModelBone.def_readwrite("physics_bone", &mstudiobone_t::physicsbone);
	ModelBone.def_readwrite("flags", &mstudiobone_t::flags);
	ModelBone.def_readwrite("contents", &mstudiobone_t::contents);

	// Methods...
	ModelBone.def("get_procedure", &ModelBoneExt::get_procedure, manage_new_object_policy());

	// Add memory tools...
	ModelBone ADD_MEM_TOOLS(mstudiobone_t);
}


//-----------------------------------------------------------------------------
// Exports mstudiobonecontroller_t.
//-----------------------------------------------------------------------------
void export_bone_controller(scope _studio)
{
	class_<mstudiobonecontroller_t, mstudiobonecontroller_t *> BoneController("BoneController");

	// Properties...
	BoneController.def_readwrite("bone", &mstudiobonecontroller_t::bone);
	BoneController.def_readwrite("type", &mstudiobonecontroller_t::type);
	BoneController.def_readwrite("start", &mstudiobonecontroller_t::start);
	BoneController.def_readwrite("end", &mstudiobonecontroller_t::end);
	BoneController.def_readwrite("rest", &mstudiobonecontroller_t::rest);
	BoneController.def_readwrite("input_field", &mstudiobonecontroller_t::inputfield);

	// Add memory tools...
	BoneController ADD_MEM_TOOLS(mstudiobonecontroller_t);
}


//-----------------------------------------------------------------------------
// Exports mstudiohitboxset_t.
//-----------------------------------------------------------------------------
void export_hitbox_set(scope _studio)
{
	class_<mstudiohitboxset_t, mstudiohitboxset_t *> HitboxSet("HitboxSet");

	// Properties...
	HitboxSet.add_property("name", &HitboxSetExt::get_name);
	HitboxSet.def_readwrite("hitboxes_count", &mstudiohitboxset_t::numhitboxes);

	// Methods...
	HitboxSet.def("get_hitbox", &mstudiohitboxset_t::pHitbox, reference_existing_object_policy());

	// Add memory tools...
	HitboxSet ADD_MEM_TOOLS(mstudiohitboxset_t);
}


//-----------------------------------------------------------------------------
// Exports mstudiobbox_t.
//-----------------------------------------------------------------------------
void export_hitbox(scope _studio)
{
	class_<mstudiobbox_t, mstudiobbox_t *, boost::noncopyable> Hitbox("Hitbox");

	// Properties...
	Hitbox.add_property("name", &mstudiobbox_t::pszHitboxName);

	Hitbox.def_readwrite("bone", &mstudiobbox_t::bone);
	Hitbox.def_readwrite("group", &mstudiobbox_t::group);

	Hitbox.add_property("min", make_getter(&mstudiobbox_t::bbmin, reference_existing_object_policy()));
	Hitbox.add_property("max", make_getter(&mstudiobbox_t::bbmax, reference_existing_object_policy()));

	// Add memory tools...
	Hitbox ADD_MEM_TOOLS(mstudiobbox_t);
}


//-----------------------------------------------------------------------------
// Exports mstudioanimdesc_t.
//-----------------------------------------------------------------------------
void export_animation(scope _studio)
{
	class_<mstudioanimdesc_t, mstudioanimdesc_t *, boost::noncopyable> Animation("Animation");

	// Properties...
	Animation.add_property("model_header", make_function(&mstudioanimdesc_t::pStudiohdr, reference_existing_object_policy()));
	Animation.add_property("name", &AnimationExt::get_name);

	Animation.def_readwrite("flags", &mstudioanimdesc_t::flags);
	Animation.def_readwrite("frames_count", &mstudioanimdesc_t::numframes);
	Animation.def_readwrite("movements_count", &mstudioanimdesc_t::nummovements);
	Animation.def_readwrite("movements_offset", &mstudioanimdesc_t::movementindex);
	Animation.def_readwrite("anim_block", &mstudioanimdesc_t::animblock);
	Animation.def_readwrite("anim_offset", &mstudioanimdesc_t::animindex);
	Animation.def_readwrite("local_hierarchy_count", &mstudioanimdesc_t::numlocalhierarchy);
	Animation.def_readwrite("local_hierarchy_offset", &mstudioanimdesc_t::localhierarchyindex);
	Animation.def_readwrite("section_offset", &mstudioanimdesc_t::sectionindex);
	Animation.def_readwrite("section_frames", &mstudioanimdesc_t::sectionframes);
	Animation.def_readwrite("zero_frame_span", &mstudioanimdesc_t::zeroframespan);
	Animation.def_readwrite("zero_frame_count", &mstudioanimdesc_t::zeroframecount);
	Animation.def_readwrite("zero_frame_index", &mstudioanimdesc_t::zeroframeindex);
	Animation.def_readwrite("zero_frame_stall_time", &mstudioanimdesc_t::zeroframestalltime);

	// Methods...
	Animation.def("get_movement", &AnimationExt::get_movement, reference_existing_object_policy());
	// Animation.def("get_hierarchy", &mstudioanimdesc_t::pHierarchy, reference_existing_object_policy());

	// Add memory tools...
	Animation ADD_MEM_TOOLS(mstudioanimdesc_t);
}


//-----------------------------------------------------------------------------
// Exports mstudioanimdesc_t.
//-----------------------------------------------------------------------------
void export_movement(scope _studio)
{
	class_<mstudiomovement_t, mstudiomovement_t *, boost::noncopyable> Movement("Movement");

	// Properties...
	Movement.def_readwrite("end_frame", &mstudiomovement_t::endframe);
	Movement.def_readwrite("motion_flags", &mstudiomovement_t::motionflags);
	Movement.def_readwrite("velocity_start", &mstudiomovement_t::v0);
	Movement.def_readwrite("velocity_end", &mstudiomovement_t::v1);
	Movement.def_readwrite("angle", &mstudiomovement_t::angle);

	Movement.add_property("vector", make_getter(&mstudiomovement_t::vector, reference_existing_object_policy()));
	Movement.add_property("position", make_getter(&mstudiomovement_t::position, reference_existing_object_policy()));

	// Add memory tools...
	Movement ADD_MEM_TOOLS(mstudiomovement_t);
}


//-----------------------------------------------------------------------------
// Exports mstudiolocalhierarchy_t.
//-----------------------------------------------------------------------------
void export_hierarchy(scope _studio)
{
	class_<mstudiolocalhierarchy_t, mstudiolocalhierarchy_t *, boost::noncopyable> Hierarchy("Hierarchy");

	// Properties...
	Hierarchy.def_readwrite("bone", &mstudiolocalhierarchy_t::iBone);
	Hierarchy.def_readwrite("new_parent", &mstudiolocalhierarchy_t::iNewParent);
	Hierarchy.def_readwrite("start", &mstudiolocalhierarchy_t::start);
	Hierarchy.def_readwrite("peak", &mstudiolocalhierarchy_t::peak);
	Hierarchy.def_readwrite("tail", &mstudiolocalhierarchy_t::tail);
	Hierarchy.def_readwrite("end", &mstudiolocalhierarchy_t::end);
	Hierarchy.def_readwrite("start", &mstudiolocalhierarchy_t::iStart);

	// Add memory tools...
	Hierarchy ADD_MEM_TOOLS(mstudiolocalhierarchy_t);
}


//-----------------------------------------------------------------------------
// Exports mstudioanimsections_t.
//-----------------------------------------------------------------------------
void export_anim_sections(scope _studio)
{
	class_<mstudioanimsections_t, mstudioanimsections_t *, boost::noncopyable> AnimSections("AnimSections");

	// Properties...
	AnimSections.def_readwrite("block", &mstudioanimsections_t::animblock);
	AnimSections.def_readwrite("index", &mstudioanimsections_t::animindex);

	// Add memory tools...
	AnimSections ADD_MEM_TOOLS(mstudioanimsections_t);
}


//-----------------------------------------------------------------------------
// Exports mstudioseqdesc_t.
//-----------------------------------------------------------------------------
void export_sequence(scope _studio)
{
	class_<mstudioseqdesc_t, mstudioseqdesc_t *, boost::noncopyable> Sequence("Sequence");

	// Properties...
	Sequence.add_property("model_header", make_function(&mstudioseqdesc_t::pStudiohdr, reference_existing_object_policy()));
	Sequence.def_readwrite("label_offset", &mstudioseqdesc_t::szlabelindex);
	Sequence.add_property("label", &SequenceExt::get_label);
	Sequence.def_readwrite("activity_offset", &mstudioseqdesc_t::szactivitynameindex);
	Sequence.add_property("activity", &SequenceExt::get_activity);
	Sequence.def_readwrite("flags", &mstudioseqdesc_t::flags);
	Sequence.def_readwrite("activity_index", &mstudioseqdesc_t::activity);
	Sequence.def_readwrite("act_weight", &mstudioseqdesc_t::actweight);
	Sequence.def_readwrite("events_count", &mstudioseqdesc_t::numevents);
	Sequence.def_readwrite("events_offset", &mstudioseqdesc_t::eventindex);
	Sequence.add_property("min", make_getter(&mstudioseqdesc_t::bbmin, reference_existing_object_policy()));
	Sequence.add_property("max", make_getter(&mstudioseqdesc_t::bbmax, reference_existing_object_policy()));
	Sequence.def_readwrite("blends_count", &mstudioseqdesc_t::numblends);
	Sequence.def_readwrite("animation_offset", &mstudioseqdesc_t::animindexindex);
	Sequence.def_readwrite("movement_index", &mstudioseqdesc_t::movementindex);
	Sequence.def_readwrite("fade_in_time", &mstudioseqdesc_t::fadeintime);
	Sequence.def_readwrite("fade_out_time", &mstudioseqdesc_t::fadeouttime);
	Sequence.def_readwrite("local_entry_node", &mstudioseqdesc_t::localentrynode);
	Sequence.def_readwrite("local_exit_node", &mstudioseqdesc_t::localexitnode);
	Sequence.def_readwrite("node_flags", &mstudioseqdesc_t::nodeflags);
	Sequence.def_readwrite("entry_phase", &mstudioseqdesc_t::entryphase);
	Sequence.def_readwrite("exit_phase", &mstudioseqdesc_t::exitphase);
	Sequence.def_readwrite("lastframe", &mstudioseqdesc_t::lastframe);
	Sequence.def_readwrite("next_sequence", &mstudioseqdesc_t::nextseq);
	Sequence.def_readwrite("pose", &mstudioseqdesc_t::pose);

	// Methods...
	Sequence.def("get_animation_index", &mstudioseqdesc_t::anim);
	Sequence.def("get_weight", &mstudioseqdesc_t::weight);

	// Add memory tools...
	Sequence ADD_MEM_TOOLS(mstudioseqdesc_t);
}


//-----------------------------------------------------------------------------
// Exports mstudioevent_t.
//-----------------------------------------------------------------------------
void export_sequence_event(scope _studio)
{
	class_<mstudioevent_t, mstudioevent_t *, boost::noncopyable> SequenceEvent("SequenceEvent");

	// Properties...
	SequenceEvent.def_readwrite("cycle", &mstudioevent_t::cycle);
	SequenceEvent.def_readwrite("event", &mstudioevent_t::event);
	SequenceEvent.def_readwrite("type", &mstudioevent_t::type);
	SequenceEvent.add_property("options", &mstudioevent_t::pszOptions);
	SequenceEvent.def_readwrite("event_offset", &mstudioevent_t::szeventindex);

	// Methods...
	SequenceEvent.def("get_name", &SequenceEventExt::get_name);

	// Add memory tools...
	SequenceEvent ADD_MEM_TOOLS(mstudioevent_t);
}


//-----------------------------------------------------------------------------
// Exports mstudioattachment_t.
//-----------------------------------------------------------------------------
void export_attachment(scope _studio)
{
	class_<mstudioattachment_t, mstudioattachment_t *, boost::noncopyable> ModelAttachment("ModelAttachment");

	// Properties...
	ModelAttachment.def_readwrite("name_offset", &mstudioattachment_t::sznameindex);
	ModelAttachment.add_property("name", &ModelAttachmentExt::get_name);
	ModelAttachment.def_readwrite("flags", &mstudioattachment_t::flags);
	ModelAttachment.def_readwrite("local_bone", &mstudioattachment_t::localbone);
	ModelAttachment.def_readwrite("local", &mstudioattachment_t::local); // TODO: Export matrix3x4_t...

	// Add memory tools...
	ModelAttachment ADD_MEM_TOOLS(mstudioattachment_t);
}
