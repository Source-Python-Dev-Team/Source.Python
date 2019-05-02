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

#ifndef _WEAPONS_ENTITY_H
#define _WEAPONS_ENTITY_H

//-----------------------------------------------------------------------------
// Includes.
//-----------------------------------------------------------------------------
#include "boost/python.hpp"
using namespace boost::python;

#include "modules/entities/entities_entity.h"


//-----------------------------------------------------------------------------
// CBaseEntity extension class for players.
//-----------------------------------------------------------------------------
class WeaponMixin: public CBaseEntityWrapper
{
public:
	static boost::shared_ptr<WeaponMixin> __init__(unsigned int uiEntityIndex);
	static boost::shared_ptr<WeaponMixin> wrap(CBaseEntity* pEntity);


	float GetNextAttack();
	void SetNextAttack(float value);

	float GetNextSecondaryFireAttack();
	void SetNextSecondaryFireAttack(float value);

	int GetAmmoProp();
	void SetAmmoProp(int value);

	int GetSecondaryFireAmmoProp();
	void SetSecondaryFireAmmoProp(int value);

	int GetOwnerHandle();
	void SetOwnerHandle(int value);

	int GetClip();
	void SetClip(int value);

	int GetSecondaryFireClip();
	void SetSecondaryFireClip(int value);

	int GetFlipViewModel();
	void SetFlipViewModel(int value);

	int GetWorldModelIndex();
	void SetWorldModelIndex(int value);

	// CS:GO
	int GetPrimaryAmmoCount();
	void SetPrimaryAmmoCount(int value);

	int GetSecondaryAmmoCount();
	void SetSecondaryAmmoCount(int value);

};


#endif // _WEAPONS_ENTITY_H