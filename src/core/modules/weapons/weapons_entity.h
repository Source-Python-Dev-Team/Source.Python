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
	static object _obj(object cls, CPointer *pPtr);

	bool IsNetworked();
	bool IsPlayer();
	bool IsWeapon();

	float GetNextAttack();
	void SetNextAttack(float value);

	float GetNextSecondaryFireAttack();
	void SetNextSecondaryFireAttack(float value);

	short GetAmmoProp();
	void SetAmmoProp(short value);

	short GetSecondaryFireAmmoProp();
	void SetSecondaryFireAmmoProp(short value);

	unsigned char GetClip();
	void SetClip(unsigned char value);

	unsigned char GetSecondaryFireClip();
	void SetSecondaryFireClip(unsigned char value);

	short GetFlipViewModel();
	void SetFlipViewModel(short value);

	short GetWorldModelIndex();
	void SetWorldModelIndex(short value);

	// CS:GO
	short GetPrimaryAmmoCount();
	void SetPrimaryAmmoCount(short value);

	short GetSecondaryAmmoCount();
	void SetSecondaryAmmoCount(short value);

};


#endif // _WEAPONS_ENTITY_H
