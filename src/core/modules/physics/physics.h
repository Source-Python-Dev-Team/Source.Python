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
// SDK
#include "vphysics_interface.h"


//-----------------------------------------------------------------------------
// WRAPPER BASE CLASS
//-----------------------------------------------------------------------------
// TODO: _obj, _ptr and _size need to be implemented differently for wrapped classes.
// TODO: Move this to a different file.
// TODO: Add the possibility to use policies (reference_existing..., etc.).
template<class T>
class Wrapper
{
public:
	typedef T wrapped_type;

	T* GetWrapped()
	{ return m_pWrapped; }

	void SetWrapped(T* pWrapped)
	{ m_pWrapped = pWrapped; }

	operator T*()
	{ return GetWrapped(); }

private:
	T* m_pWrapped;
};

template<class WrapperType>
WrapperType* Wrap(typename WrapperType::wrapped_type* pWrapped)
{
	if (!pWrapped)
		return NULL;

	WrapperType* result = new WrapperType;
	result->SetWrapped(pWrapped);
	return result;
}

//-----------------------------------------------------------------------------
// IPhysicsObject
//-----------------------------------------------------------------------------
class IPhysicsObjectWrapper: public Wrapper<IPhysicsObject>
{
public:
	bool IsStatic();
	bool IsAsleep();
	bool IsTrigger();
	bool IsFluid();
	bool IsHinged();
	bool IsCollisionEnabled();
	bool IsGravityEnabled();
	bool IsDragEnabled();
	bool IsMotionEnabled();
	bool IsMoveable();
	bool IsAttachedToConstraint(bool external_only);

	void EnableCollisions(bool enable);
	void EnableGravity(bool enable);
	void EnableDrag(bool enable);
	void EnableMotion(bool enable);
	void EnableSleep(bool enable);
	
	void* GetGameData();
	void SetGameData(void* game_data);
	
	unsigned short GetGameFlags();
	void SetGameFlags(unsigned short flags);

	unsigned short GetGameIndex();
	void SetGameIndex(unsigned short game_index);
	
	unsigned short GetCallbackFlags();
	void SetCallbackFlags(unsigned short callback_flags);

	void SetMass(float mass);
	float GetMass();

	void RecheckCollisionFilter();
	void RecheckContactPoints();
	

/*
	// mass accessors
	// get 1/mass (it's cached)
	virtual float			GetInvMass( void ) const = 0;
	virtual Vector			GetInertia( void ) const = 0;
	virtual Vector			GetInvInertia( void ) const = 0;
	virtual void			SetInertia( const Vector &inertia ) = 0;

	virtual void			SetDamping( const float *speed, const float *rot ) = 0;
	virtual void			GetDamping( float *speed, float *rot ) const = 0;

	// coefficients are optional, pass either
	virtual void			SetDragCoefficient( float *pDrag, float *pAngularDrag ) = 0;
	virtual void			SetBuoyancyRatio( float ratio ) = 0;			// Override bouyancy

	// material index
	virtual int				GetMaterialIndex() const = 0;
	virtual void			SetMaterialIndex( int materialIndex ) = 0;

	// contents bits
	virtual unsigned int	GetContents() const = 0;
	virtual void			SetContents( unsigned int contents ) = 0;

	// Get the radius if this is a sphere object (zero if this is a polygonal mesh)
	virtual float			GetSphereRadius() const = 0;
	virtual float			GetEnergy() const = 0;
	virtual Vector			GetMassCenterLocalSpace() const = 0;

	// NOTE: This will teleport the object
	virtual void			SetPosition( const Vector &worldPosition, const QAngle &angles, bool isTeleport ) = 0;
	virtual void			SetPositionMatrix( const matrix3x4_t&matrix, bool isTeleport ) = 0;

	virtual void			GetPosition( Vector *worldPosition, QAngle *angles ) const = 0;
	virtual void			GetPositionMatrix( matrix3x4_t *positionMatrix ) const = 0;
	// force the velocity to a new value
	// NOTE: velocity is in worldspace, angularVelocity is relative to the object's 
	// local axes (just like pev->velocity, pev->avelocity)
	virtual void			SetVelocity( const Vector *velocity, const AngularImpulse *angularVelocity ) = 0;

	// like the above, but force the change into the simulator immediately
	virtual void			SetVelocityInstantaneous( const Vector *velocity, const AngularImpulse *angularVelocity ) = 0;

	// NOTE: velocity is in worldspace, angularVelocity is relative to the object's 
	// local axes (just like pev->velocity, pev->avelocity)
	virtual void			GetVelocity( Vector *velocity, AngularImpulse *angularVelocity ) const = 0;

	// NOTE: These are velocities, not forces.  i.e. They will have the same effect regardless of
	// the object's mass or inertia
	virtual void			AddVelocity( const Vector *velocity, const AngularImpulse *angularVelocity ) = 0;
	// gets a velocity in the object's local frame of reference at a specific point
	virtual void			GetVelocityAtPoint( const Vector &worldPosition, Vector *pVelocity ) const = 0;
	// gets the velocity actually moved by the object in the last simulation update
	virtual void			GetImplicitVelocity( Vector *velocity, AngularImpulse *angularVelocity ) const = 0;
	// NOTE:	These are here for convenience, but you can do them yourself by using the matrix
	//			returned from GetPositionMatrix()
	// convenient coordinate system transformations (params - dest, src)
	virtual void			LocalToWorld( Vector *worldPosition, const Vector &localPosition ) const = 0;
	virtual void			WorldToLocal( Vector *localPosition, const Vector &worldPosition ) const = 0;

	// transforms a vector (no translation) from object-local to world space
	virtual void			LocalToWorldVector( Vector *worldVector, const Vector &localVector ) const = 0;
	// transforms a vector (no translation) from world to object-local space
	virtual void			WorldToLocalVector( Vector *localVector, const Vector &worldVector ) const = 0;
	
	// push on an object
	// force vector is direction & magnitude of impulse kg in / s
	virtual void			ApplyForceCenter( const Vector &forceVector ) = 0;
	virtual void			ApplyForceOffset( const Vector &forceVector, const Vector &worldPosition ) = 0;
	// apply torque impulse.  This will change the angular velocity on the object.
	// HL Axes, kg degrees / s
	virtual void			ApplyTorqueCenter( const AngularImpulse &torque ) = 0;

	// Calculates the force/torque on the center of mass for an offset force impulse (pass output to ApplyForceCenter / ApplyTorqueCenter)
	virtual void			CalculateForceOffset( const Vector &forceVector, const Vector &worldPosition, Vector *centerForce, AngularImpulse *centerTorque ) const = 0;
	// Calculates the linear/angular velocities on the center of mass for an offset force impulse (pass output to AddVelocity)
	virtual void			CalculateVelocityOffset( const Vector &forceVector, const Vector &worldPosition, Vector *centerVelocity, AngularImpulse *centerAngularVelocity ) const = 0;
	// calculate drag scale
	virtual float			CalculateLinearDrag( const Vector &unitDirection ) const = 0;
	virtual float			CalculateAngularDrag( const Vector &objectSpaceRotationAxis ) const = 0;

	// returns true if the object is in contact with another object
	// if true, puts a point on the contact surface in contactPoint, and
	// a pointer to the object in contactObject
	// NOTE: You can pass NULL for either to avoid computations
	// BUGBUG: Use CreateFrictionSnapshot instead of this - this is a simple hack
	virtual bool			GetContactPoint( Vector *contactPoint, IPhysicsObject **contactObject ) const = 0;

	// refactor this a bit - move some of this to IPhysicsShadowController
	virtual void			SetShadow( float maxSpeed, float maxAngularSpeed, bool allowPhysicsMovement, bool allowPhysicsRotation ) = 0;
	virtual void			UpdateShadow( const Vector &targetPosition, const QAngle &targetAngles, bool tempDisableGravity, float timeOffset ) = 0;
	
	// returns number of ticks since last Update() call
	virtual int				GetShadowPosition( Vector *position, QAngle *angles ) const = 0;
	virtual IPhysicsShadowController *GetShadowController( void ) const = 0;
	virtual void			RemoveShadowController() = 0;
	// applies the math of the shadow controller to this object.
	// for use in your own controllers
	// returns the new value of secondsToArrival with dt time elapsed
	virtual float			ComputeShadowControl( const hlshadowcontrol_params_t &params, float secondsToArrival, float dt ) = 0;


	virtual const CPhysCollide	*GetCollide( void ) const = 0;
*/
	const char* GetName();

/*
	virtual void			BecomeTrigger() = 0;
	virtual void			RemoveTrigger() = 0;

	// sets the object to be hinged.  Fixed it place, but able to rotate around one axis.
	virtual void			BecomeHinged( int localAxis ) = 0;
	// resets the object to original state
	virtual void			RemoveHinged() = 0;

	// used to iterate the contact points of an object
	virtual IPhysicsFrictionSnapshot *CreateFrictionSnapshot() = 0;
	virtual void DestroyFrictionSnapshot( IPhysicsFrictionSnapshot *pSnapshot ) = 0;

	// dumps info about the object to Msg()
	virtual void			OutputDebugInfo() const = 0;
*/
	void OutputDebugInfo();
};


//-----------------------------------------------------------------------------
// IPhysicsEnvironment
//-----------------------------------------------------------------------------
class IPhysicsEnvironmentWrapper: public Wrapper<IPhysicsEnvironment>
{
public:
	Vector* GetGravity();
	void SetGravity(Vector& vec);

	void SetAirDensity(float density);
	float GetAirDensity();

	IPhysicsObjectWrapper* GetActiveObjectByIndex(int index);
};


//-----------------------------------------------------------------------------
// IPhysics
//-----------------------------------------------------------------------------
class IPhysicsWrapper: public Wrapper<IPhysics>
{
public:
	IPhysicsEnvironmentWrapper* GetActiveEnvironmentByIndex(int index);
};
