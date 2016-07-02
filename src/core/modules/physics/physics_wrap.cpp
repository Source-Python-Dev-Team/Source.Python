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


//-----------------------------------------------------------------------------
// Declare the _physics module.
//-----------------------------------------------------------------------------
DECLARE_SP_MODULE(_physics)
{
	export_physics(_physics);
	export_physics_environment(_physics);
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

	//PhysicsEnvironment ADD_MEM_TOOLS_WRAPPER(IPhysicsEnvironmentWrapper, IPhysicsEnvironment);
}
