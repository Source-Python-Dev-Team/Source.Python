/**
* =============================================================================
* Source Python
* Copyright (C) 2012-2016 Source Python Development Team.  All rights reserved.
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
// Source.Python
#include "export_main.h"
#include "modules/memory/memory_utilities.h"
#include "physics.h"


//-----------------------------------------------------------------------------
// Forward declarations.
//-----------------------------------------------------------------------------
extern IPhysics* physics;

void export_physics(scope);
void export_physics_environment(scope);
void export_physics_object(scope);


//-----------------------------------------------------------------------------
// Declare the _physics module.
//-----------------------------------------------------------------------------
DECLARE_SP_MODULE(_physics)
{
	export_physics(_physics);
	export_physics_environment(_physics);
	export_physics_object(_physics);
}


//-----------------------------------------------------------------------------
// Exports IPhysics.
//-----------------------------------------------------------------------------
void export_physics(scope _physics)
{
	class_<IPhysicsWrapper, boost::noncopyable> Physics("Physics", no_init);

	Physics.def(
		"get_active_environment_by_index",
		&IPhysicsWrapper::GetActiveEnvironmentByIndex,
		manage_new_object_policy()
	);

	//Physics ADD_MEM_TOOLS_WRAPPER(IPhysicsWrapper, IPhysics);

	_physics.attr("physics") = object(ptr(Wrap<IPhysicsWrapper>(physics)));
}


//-----------------------------------------------------------------------------
// Exports IPhysicsEnvironment.
//-----------------------------------------------------------------------------
void export_physics_environment(scope _physics)
{
	class_<IPhysicsEnvironmentWrapper, boost::noncopyable> PhysicsEnvironment("PhysicsEnvironment", no_init);

	PhysicsEnvironment.add_property(
		"gravity",
		make_function(&IPhysicsEnvironmentWrapper::GetGravity, manage_new_object_policy()),
		&IPhysicsEnvironmentWrapper::SetGravity
	);

	PhysicsEnvironment.add_property(
		"air_density",
		&IPhysicsEnvironmentWrapper::GetAirDensity,
		&IPhysicsEnvironmentWrapper::SetAirDensity
	);

	PhysicsEnvironment.def(
		"get_active_object_by_index",
		&IPhysicsEnvironmentWrapper::GetActiveObjectByIndex,
		manage_new_object_policy()
	);

	//PhysicsEnvironment ADD_MEM_TOOLS_WRAPPER(IPhysicsEnvironmentWrapper, IPhysicsEnvironment);
}


//-----------------------------------------------------------------------------
// Exports IPhysicsObject.
//-----------------------------------------------------------------------------
void export_physics_object(scope _physics)
{
	class_<IPhysicsObjectWrapper, boost::noncopyable> PhysicsObject("PhysicsObject", no_init);

	PhysicsObject.def(
		"is_static",
		&IPhysicsObjectWrapper::IsStatic,
		"Return True if the object is static."
	);

	PhysicsObject.add_property(
		"asleep",
		&IPhysicsObjectWrapper::IsAsleep,
		&IPhysicsObjectWrapper::EnableSleep,
		"Return True if the object is asleep."
	);

	PhysicsObject.def(
		"is_trigger",
		&IPhysicsObjectWrapper::IsTrigger,
		"Return True if the object is a trigger."
	);

	PhysicsObject.def(
		"is_fluid",
		&IPhysicsObjectWrapper::IsFluid,
		"Return True if the object is fluid."
	);

	PhysicsObject.def(
		"is_hinged",
		&IPhysicsObjectWrapper::IsHinged,
		"Return True if the object is hinged."
	);

	PhysicsObject.add_property(
		"collision_enabled",
		&IPhysicsObjectWrapper::IsCollisionEnabled,
		&IPhysicsObjectWrapper::EnableCollisions,
		"Return True if collisions are enabled."
	);

	PhysicsObject.add_property(
		"gravity_enabled",
		&IPhysicsObjectWrapper::IsGravityEnabled,
		&IPhysicsObjectWrapper::EnableGravity,
		"Return True if gravity is enabled."

	);

	PhysicsObject.add_property(
		"drag_enabled",
		&IPhysicsObjectWrapper::IsDragEnabled,
		&IPhysicsObjectWrapper::EnableDrag,
		"Return True if air fraction/drag is enabled."

	);

	PhysicsObject.add_property(
		"motion_enabled",
		&IPhysicsObjectWrapper::IsMotionEnabled,
		&IPhysicsObjectWrapper::EnableMotion,
		"Return True if motion is enabled."

	);

	PhysicsObject.def(
		"is_moveable",
		&IPhysicsObjectWrapper::IsMoveable
	);

	PhysicsObject.def(
		"is_attached_to_constraint",
		&IPhysicsObjectWrapper::IsAttachedToConstraint
	);


	PhysicsObject.add_property(
		"game_data",
		make_function(&IPhysicsObjectWrapper::GetGameData, return_by_value_policy()),
		&IPhysicsObjectWrapper::SetGameData
	);

	PhysicsObject.add_property(
		"game_flags",
		&IPhysicsObjectWrapper::GetGameFlags,
		&IPhysicsObjectWrapper::SetGameFlags
	);

	PhysicsObject.add_property(
		"game_index",
		&IPhysicsObjectWrapper::GetGameIndex,
		&IPhysicsObjectWrapper::SetGameIndex
	);

	PhysicsObject.add_property(
		"callback_flags",
		&IPhysicsObjectWrapper::GetCallbackFlags,
		&IPhysicsObjectWrapper::SetCallbackFlags
	);

	PhysicsObject.add_property(
		"mass",
		&IPhysicsObjectWrapper::GetMass,
		&IPhysicsObjectWrapper::SetMass
	);
	
	
	PhysicsObject.def(
		"recheck_collision_filter",
		&IPhysicsObjectWrapper::RecheckCollisionFilter
	);
	
	PhysicsObject.def(
		"recheck_contact_points",
		&IPhysicsObjectWrapper::RecheckContactPoints
	);


	PhysicsObject.add_property(
		"inertia",
		&IPhysicsObjectWrapper::GetInertia,
		&IPhysicsObjectWrapper::SetInertia
	);

	PhysicsObject.add_property(
		"material_index",
		&IPhysicsObjectWrapper::GetMaterialIndex,
		&IPhysicsObjectWrapper::SetMaterialIndex
	);

	PhysicsObject.add_property(
		"contents",
		&IPhysicsObjectWrapper::GetContents,
		&IPhysicsObjectWrapper::SetContents
	);

	PhysicsObject.add_property(
		"sphere_radius",
		&IPhysicsObjectWrapper::GetSphereRadius,
		"Return the radius if this is a sphere object (zero if this is a polygonal mesh)."
	);

	PhysicsObject.add_property(
		"energy",
		&IPhysicsObjectWrapper::GetEnergy
	);

	PhysicsObject.add_property(
		"mass_center_local_space",
		&IPhysicsObjectWrapper::GetMassCenterLocalSpace
	);

	PhysicsObject.add_property(
		"position",
		&IPhysicsObjectWrapper::GetPosition,
		"Return a tuple that contains the position and the angle of the object."
	);

	PhysicsObject.def(
		"set_position",
		&IPhysicsObjectWrapper::SetPosition
	);

	PhysicsObject.add_property(
		"velocity",
		&IPhysicsObjectWrapper::GetVelocity,
		"Return a tuple that contains the velocity in worldspace and relative to the object."
	);

	PhysicsObject.def(
		"set_velocity",
		&IPhysicsObjectWrapper::SetVelocity
	);

	PhysicsObject.def(
		"set_velocity_instantaneous",
		&IPhysicsObjectWrapper::SetVelocityInstantaneous
	);

	PhysicsObject.def(
		"apply_force_center",
		&IPhysicsObjectWrapper::ApplyForceCenter
	);



	PhysicsObject.add_property(
		"name",
		&IPhysicsObjectWrapper::GetName
	);



	PhysicsObject.def(
		"output_debug_info",
		&IPhysicsObjectWrapper::OutputDebugInfo
	);


	// Add memory tools...
	// TODO: Add a macro to wrap these.
	PhysicsObject.def(
		GET_OBJ_NAME,
		&IPhysicsObjectWrapper::_obj<IPhysicsObjectWrapper>,
		manage_new_object_policy()
	);

	PhysicsObject.def(
		GET_PTR_NAME,
		&IPhysicsObjectWrapper::_ptr<IPhysicsObjectWrapper>,
		manage_new_object_policy()
	);

	PhysicsObject ADD_SIZE(IPhysicsObject)
	STORE_CLASS(IPhysicsObject, "PhysicsObject")
}
