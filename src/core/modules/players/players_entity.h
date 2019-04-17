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

#ifndef _PLAYERS_ENTITY_H
#define _PLAYERS_ENTITY_H

//-----------------------------------------------------------------------------
// Includes.
//-----------------------------------------------------------------------------
#include "boost/python.hpp"
using namespace boost::python;

#include "modules/entities/entities_entity.h"



//-----------------------------------------------------------------------------
// CBaseEntity extension class for players.
//-----------------------------------------------------------------------------
class PlayerWrapper: public CBaseEntityWrapper
{
public:
	static boost::shared_ptr<PlayerWrapper> __init__(unsigned int uiEntityIndex);
	static boost::shared_ptr<PlayerWrapper> wrap(CBaseEntity* pEntity);
	
	// CBasePlayer
	// TODO: Return for some of these the proper entity class instead of a handle/index
	// E. g. BaseEntity, Entity, Weapon, Player, etc.
	float GetSpeed();
	void SetSpeed(float value);

	bool GetIsDucked();
	void SetIsDucked(bool value);

	bool GetIsDucking();
	void SetIsDucking(bool value);

	int GetFlags();
	void SetFlags(int value);

	int GetLastWeapon();
	void SetLastWeapon(int value);

	int GetObserverTarget();
	void SetObserverTarget(int value);

	int GetDeaths();
	void SetDeaths(int value);

	int GetKills();
	void SetKills(int value);

	// TODO: Return ObserverMode enum
	int GetObserverMode();
	void SetObserverMode(int value);

	// TODO: Return LifeState enum
	int GetLifeState();
	void SetLifeState(int value);

	str GetPlace();
	void SetPlace(const char* value);

	bool GetDead();
	void SetDead(bool value);

	float GetFallVelocity();
	void SetFallVelocity(float value);

	int GetButtons();
	void SetButtons(int value);

	int GetHiddenHUDs();
	void SetHiddenHUDs(int value);

	int GetDrawViewModel();
	void SetDrawViewModel(int value);

	int GetFOV();
	void SetFOV(int value);

	int GetFOVStart();
	void SetFOVStart(int value);

	float GetFOVTime();
	void SetFOVTime(float value);

	int GetDefaultFOV();
	void SetDefaultFOV(int value);

	float GetFOVRate();
	void SetFOVRate(float value);

	// CBaseCombatCharacter
	Vector GetGunOffset();
	void SetGunOffset(Vector& value);

	int GetLastHitgroup();
	void SetLastHitgroup(int value);

	int GetActiveWeaponHandle();
	void SetActiveWeaponHandle(int value);

	str GetRelationship();
	void SetRelationship(const char* value);

	float GetPhysDamageScale();
	void SetPhysDamageScale(float value);
};


#endif // _PLAYERS_ENTITY_H
