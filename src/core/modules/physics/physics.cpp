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
#include "utilities/conversions.h"
#include "physics.h"

using namespace boost::python;


//-----------------------------------------------------------------------------
// IPhysicsObject
//-----------------------------------------------------------------------------
bool IPhysicsObjectWrapper::IsStatic()
{ return GetWrapped()->IsStatic(); }

bool IPhysicsObjectWrapper::IsAsleep()
{ return GetWrapped()->IsAsleep(); }

bool IPhysicsObjectWrapper::IsTrigger()
{ return GetWrapped()->IsTrigger(); }

bool IPhysicsObjectWrapper::IsFluid()
{ return GetWrapped()->IsFluid(); }

bool IPhysicsObjectWrapper::IsHinged()
{ return GetWrapped()->IsHinged(); }

bool IPhysicsObjectWrapper::IsCollisionEnabled()
{ return GetWrapped()->IsCollisionEnabled(); }

bool IPhysicsObjectWrapper::IsGravityEnabled()
{ return GetWrapped()->IsGravityEnabled(); }

bool IPhysicsObjectWrapper::IsDragEnabled()
{ return GetWrapped()->IsDragEnabled(); }

bool IPhysicsObjectWrapper::IsMotionEnabled()
{ return GetWrapped()->IsMotionEnabled(); }

bool IPhysicsObjectWrapper::IsMoveable()
{ return GetWrapped()->IsMoveable(); }

bool IPhysicsObjectWrapper::IsAttachedToConstraint(bool external_only)
{ return GetWrapped()->IsAttachedToConstraint(external_only); }


void IPhysicsObjectWrapper::EnableCollisions(bool enable)
{ GetWrapped()->EnableCollisions(enable); }

void IPhysicsObjectWrapper::EnableGravity(bool enable)
{ GetWrapped()->EnableGravity(enable); }

void IPhysicsObjectWrapper::EnableDrag(bool enable)
{ GetWrapped()->EnableDrag(enable); }

void IPhysicsObjectWrapper::EnableMotion(bool enable)
{ GetWrapped()->EnableMotion(enable); }

void IPhysicsObjectWrapper::EnableSleep(bool enable)
{ 
	if (enable)
		GetWrapped()->Sleep();
	else
		GetWrapped()->Wake();
}


void* IPhysicsObjectWrapper::GetGameData()
{ return GetWrapped()->GetGameData(); }

void IPhysicsObjectWrapper::SetGameData(void* game_data)
{ GetWrapped()->SetGameData(game_data); }


unsigned short IPhysicsObjectWrapper::GetGameFlags()
{ return GetWrapped()->GetGameFlags(); }

void IPhysicsObjectWrapper::SetGameFlags(unsigned short game_flags)
{ GetWrapped()->SetGameFlags(game_flags); }


unsigned short IPhysicsObjectWrapper::GetGameIndex()
{ return GetWrapped()->GetGameIndex(); }

void IPhysicsObjectWrapper::SetGameIndex(unsigned short game_index)
{ GetWrapped()->SetGameIndex(game_index); }


unsigned short IPhysicsObjectWrapper::GetCallbackFlags()
{ return GetWrapped()->GetCallbackFlags(); }

void IPhysicsObjectWrapper::SetCallbackFlags(unsigned short callback_flags)
{ GetWrapped()->SetCallbackFlags(callback_flags); }


float IPhysicsObjectWrapper::GetMass()
{ return GetWrapped()->GetMass(); }

void IPhysicsObjectWrapper::SetMass(float mass)
{ GetWrapped()->SetMass(mass); }


void IPhysicsObjectWrapper::RecheckCollisionFilter()
{ GetWrapped()->RecheckCollisionFilter(); }

void IPhysicsObjectWrapper::RecheckContactPoints()
{ GetWrapped()->RecheckContactPoints(); }


Vector IPhysicsObjectWrapper::GetInertia()
{ return GetWrapped()->GetInertia(); }

void IPhysicsObjectWrapper::SetInertia(Vector& inertia)
{ GetWrapped()->SetInertia(inertia); }


int IPhysicsObjectWrapper::GetMaterialIndex()
{ return GetWrapped()->GetMaterialIndex(); }

void IPhysicsObjectWrapper::SetMaterialIndex(int index)
{ GetWrapped()->SetMaterialIndex(index); }


unsigned int IPhysicsObjectWrapper::GetContents()
{ return GetWrapped()->GetContents(); }

void IPhysicsObjectWrapper::SetContents(unsigned int contents)
{ GetWrapped()->SetContents(contents); }


float IPhysicsObjectWrapper::GetSphereRadius()
{ return GetWrapped()->GetSphereRadius(); }

float IPhysicsObjectWrapper::GetEnergy()
{ return GetWrapped()->GetEnergy(); }

Vector IPhysicsObjectWrapper::GetMassCenterLocalSpace()
{ return GetWrapped()->GetMassCenterLocalSpace(); }

tuple IPhysicsObjectWrapper::GetPosition()
{
	Vector position;
	QAngle angles;
	GetWrapped()->GetPosition(&position, &angles);
	return make_tuple(position, angles);
}

void IPhysicsObjectWrapper::SetPosition(Vector& position, QAngle& angles, bool teleport)
{ GetWrapped()->SetPosition(position, angles, teleport); }

tuple IPhysicsObjectWrapper::GetVelocity()
{
	Vector velocity;
	AngularImpulse angular_velocity;
	GetWrapped()->GetVelocity(&velocity, &angular_velocity);
	return make_tuple(velocity, angular_velocity);
}

void IPhysicsObjectWrapper::SetVelocity(Vector* velocity, AngularImpulse* angular_velocity)
{ GetWrapped()->SetVelocity(velocity, angular_velocity); }



const char* IPhysicsObjectWrapper::GetName()
{ return GetWrapped()->GetName(); }



void IPhysicsObjectWrapper::OutputDebugInfo()
{ GetWrapped()->OutputDebugInfo(); }


//-----------------------------------------------------------------------------
// IPhysicsEnvironment
//-----------------------------------------------------------------------------
Vector* IPhysicsEnvironmentWrapper::GetGravity()
{
	Vector* result = new Vector;
	GetWrapped()->GetGravity(result);
	return result;
}

void IPhysicsEnvironmentWrapper::SetGravity(Vector& vec)
{
	GetWrapped()->SetGravity(vec);
}

void IPhysicsEnvironmentWrapper::SetAirDensity(float density)
{
	GetWrapped()->SetAirDensity(density);
}

float IPhysicsEnvironmentWrapper::GetAirDensity()
{
	return GetWrapped()->GetAirDensity();
}

IPhysicsObjectWrapper* IPhysicsEnvironmentWrapper::GetActiveObjectByIndex(int index)
{
	int count = GetWrapped()->GetActiveObjectCount();
	if (count == 0)
		return NULL;

	IPhysicsObject** ppObjects = new IPhysicsObject*[count];
	GetWrapped()->GetActiveObjects(ppObjects);

	for (int i=0; i < count; ++i)
	{
		IPhysicsObject* current_object = ppObjects[i];
		unsigned int current_index;
		if (!IndexFromBaseEntity((CBaseEntity*) current_object->GetGameData(), current_index))
			continue;

		if (current_index == index)
		{
			delete[] ppObjects;
			return Wrap<IPhysicsObjectWrapper>(current_object);
		}
	}

	delete[] ppObjects;
	return NULL;
}


//-----------------------------------------------------------------------------
// IPhysics
//-----------------------------------------------------------------------------
IPhysicsEnvironmentWrapper* IPhysicsWrapper::GetActiveEnvironmentByIndex(int index)
{
	return Wrap<IPhysicsEnvironmentWrapper>(GetWrapped()->GetActiveEnvironmentByIndex(index));
}