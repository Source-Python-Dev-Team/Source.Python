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

// ============================================================================
// >> INCLUDES
// ============================================================================
// Source.Python
#include "weapons_entity.h"


// ============================================================================
// >> PlayerMixin
// ============================================================================
boost::shared_ptr<WeaponMixin> WeaponMixin::__init__(unsigned int uiEntityIndex)
{
	CBaseEntityWrapper* pEntity = (CBaseEntityWrapper*) ExcBaseEntityFromIndex(uiEntityIndex);

	if (!pEntity->IsWeapon())
		BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Index '%d' is not a valid weapon.", uiEntityIndex);

	return WeaponMixin::wrap(pEntity->GetBaseEntity());
}

boost::shared_ptr<WeaponMixin> WeaponMixin::wrap(CBaseEntity* pEntity)
{
	return boost::shared_ptr<WeaponMixin>(
		(WeaponMixin *) pEntity,
		&NeverDeleteDeleter<WeaponMixin *>
	);
}

object WeaponMixin::_obj(object cls, CPointer *pPtr)
{
	return cls(object(ExcIndexFromPointer(pPtr)));
}

bool WeaponMixin::IsNetworked()
{
	return true;
}

bool WeaponMixin::IsPlayer()
{
	return false;
}

bool WeaponMixin::IsWeapon()
{
	return true;
}

// CBaseCombatWeapon
float WeaponMixin::GetNextAttack()
{
	static int offset = FindNetworkPropertyOffset("LocalActiveWeaponData.m_flNextPrimaryAttack");
	return GetNetworkPropertyByOffset<float>(offset);
}

void WeaponMixin::SetNextAttack(float value)
{
	static int offset = FindNetworkPropertyOffset("LocalActiveWeaponData.m_flNextPrimaryAttack");
	SetNetworkPropertyByOffset<float>(offset, value);
}


float WeaponMixin::GetNextSecondaryFireAttack()
{
	static int offset = FindNetworkPropertyOffset("LocalActiveWeaponData.m_flNextSecondaryAttack");
	return GetNetworkPropertyByOffset<float>(offset);
}

void WeaponMixin::SetNextSecondaryFireAttack(float value)
{
	static int offset = FindNetworkPropertyOffset("LocalActiveWeaponData.m_flNextSecondaryAttack");
	SetNetworkPropertyByOffset<float>(offset, value);
}


short WeaponMixin::GetAmmoProp()
{
	static int offset = FindNetworkPropertyOffset("LocalWeaponData.m_iPrimaryAmmoType");
	return GetNetworkPropertyByOffset<char>(offset);
}

void WeaponMixin::SetAmmoProp(short value)
{
	static int offset = FindNetworkPropertyOffset("LocalWeaponData.m_iPrimaryAmmoType");
	SetNetworkPropertyByOffset<char>(offset, value);
}


short WeaponMixin::GetSecondaryFireAmmoProp()
{
	static int offset = FindNetworkPropertyOffset("LocalWeaponData.m_iSecondaryAmmoType");
	return GetNetworkPropertyByOffset<char>(offset);
}

void WeaponMixin::SetSecondaryFireAmmoProp(short value)
{
	static int offset = FindNetworkPropertyOffset("LocalWeaponData.m_iSecondaryAmmoType");
	SetNetworkPropertyByOffset<char>(offset, value);
}


unsigned char WeaponMixin::GetClip()
{
	static int offset = FindDatamapPropertyOffset("m_iClip1");
	return GetNetworkPropertyByOffset<unsigned char>(offset);
}

void WeaponMixin::SetClip(unsigned char value)
{
	static int offset = FindDatamapPropertyOffset("m_iClip1");
	SetNetworkPropertyByOffset<unsigned char>(offset, value);
}


unsigned char WeaponMixin::GetSecondaryFireClip()
{
	static int offset = FindDatamapPropertyOffset("m_iClip2");
	return GetNetworkPropertyByOffset<unsigned char>(offset);
}

void WeaponMixin::SetSecondaryFireClip(unsigned char value)
{
	static int offset = FindDatamapPropertyOffset("m_iClip2");
	SetNetworkPropertyByOffset<unsigned char>(offset, value);
}


short WeaponMixin::GetFlipViewModel()
{
	static int offset = FindNetworkPropertyOffset("LocalWeaponData.m_bFlipViewModel");
	return GetNetworkPropertyByOffset<char>(offset);
}

void WeaponMixin::SetFlipViewModel(short value)
{
	static int offset = FindNetworkPropertyOffset("LocalWeaponData.m_bFlipViewModel");
	SetNetworkPropertyByOffset<char>(offset, value);
}


short WeaponMixin::GetWorldModelIndex()
{
	static int offset = FindNetworkPropertyOffset("m_iWorldModelIndex");
	return GetNetworkPropertyByOffset<short>(offset);
}

void WeaponMixin::SetWorldModelIndex(short value)
{
	static int offset = FindNetworkPropertyOffset("m_iWorldModelIndex");
	SetNetworkPropertyByOffset<short>(offset, value);
}


// CS:GO
short WeaponMixin::GetPrimaryAmmoCount()
{
	static int offset = FindNetworkPropertyOffset("m_iPrimaryReserveAmmoCount");
	return GetNetworkPropertyByOffset<short>(offset);
}

void WeaponMixin::SetPrimaryAmmoCount(short value)
{
	static int offset = FindNetworkPropertyOffset("m_iPrimaryReserveAmmoCount");
	SetNetworkPropertyByOffset<short>(offset, value);
}


short WeaponMixin::GetSecondaryAmmoCount()
{
	static int offset = FindNetworkPropertyOffset("m_iSecondaryReserveAmmoCount");
	return GetNetworkPropertyByOffset<short>(offset);
}

void WeaponMixin::SetSecondaryAmmoCount(short value)
{
	static int offset = FindNetworkPropertyOffset("m_iSecondaryReserveAmmoCount");
	SetNetworkPropertyByOffset<short>(offset, value);
}