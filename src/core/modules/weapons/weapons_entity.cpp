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

	// TODO
	//if (!pEntity->IsPlayer())
	//	BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Index '%d' is not a valid weapon.", uiEntityIndex);

	return WeaponMixin::wrap(pEntity->GetBaseEntity());
}

boost::shared_ptr<WeaponMixin> WeaponMixin::wrap(CBaseEntity* pEntity)
{
	return boost::shared_ptr<WeaponMixin>(
		(WeaponMixin *) pEntity,
		&NeverDeleteDeleter<WeaponMixin *>
	);
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


int WeaponMixin::GetAmmoProp()
{
	static int offset = FindNetworkPropertyOffset("LocalWeaponData.m_iPrimaryAmmoType");
	return GetNetworkPropertyByOffset<int>(offset);
}

void WeaponMixin::SetAmmoProp(int value)
{
	static int offset = FindNetworkPropertyOffset("LocalWeaponData.m_iPrimaryAmmoType");
	SetNetworkPropertyByOffset<int>(offset, value);
}


int WeaponMixin::GetSecondaryFireAmmoProp()
{
	static int offset = FindNetworkPropertyOffset("LocalWeaponData.m_iSecondaryAmmoType");
	return GetNetworkPropertyByOffset<int>(offset);
}

void WeaponMixin::SetSecondaryFireAmmoProp(int value)
{
	static int offset = FindNetworkPropertyOffset("LocalWeaponData.m_iSecondaryAmmoType");
	SetNetworkPropertyByOffset<int>(offset, value);
}


int WeaponMixin::GetOwnerHandle()
{
	static int offset = FindNetworkPropertyOffset("m_hOwner");
	return GetNetworkPropertyByOffset<int>(offset);
}

void WeaponMixin::SetOwnerHandle(int value)
{
	static int offset = FindNetworkPropertyOffset("m_hOwner");
	SetNetworkPropertyByOffset<int>(offset, value);
}


int WeaponMixin::GetClip()
{
	static int offset = FindDatamapPropertyOffset("m_iClip1");
	return GetNetworkPropertyByOffset<int>(offset);
}

void WeaponMixin::SetClip(int value)
{
	static int offset = FindDatamapPropertyOffset("m_iClip1");
	SetNetworkPropertyByOffset<int>(offset, value);
}


int WeaponMixin::GetSecondaryFireClip()
{
	static int offset = FindDatamapPropertyOffset("m_iClip2");
	return GetNetworkPropertyByOffset<int>(offset);
}

void WeaponMixin::SetSecondaryFireClip(int value)
{
	static int offset = FindDatamapPropertyOffset("m_iClip2");
	SetNetworkPropertyByOffset<int>(offset, value);
}


int WeaponMixin::GetFlipViewModel()
{
	static int offset = FindNetworkPropertyOffset("LocalWeaponData.m_bFlipViewModel");
	return GetNetworkPropertyByOffset<int>(offset);
}

void WeaponMixin::SetFlipViewModel(int value)
{
	static int offset = FindNetworkPropertyOffset("LocalWeaponData.m_bFlipViewModel");
	SetNetworkPropertyByOffset<int>(offset, value);
}


int WeaponMixin::GetWorldModelIndex()
{
	static int offset = FindNetworkPropertyOffset("m_iWorldModelIndex");
	return GetNetworkPropertyByOffset<int>(offset);
}

void WeaponMixin::SetWorldModelIndex(int value)
{
	static int offset = FindNetworkPropertyOffset("m_iWorldModelIndex");
	SetNetworkPropertyByOffset<int>(offset, value);
}


// CS:GO
int WeaponMixin::GetPrimaryAmmoCount()
{
	static int offset = FindNetworkPropertyOffset("m_iPrimaryReserveAmmoCount");
	return GetNetworkPropertyByOffset<int>(offset);
}

void WeaponMixin::SetPrimaryAmmoCount(int value)
{
	static int offset = FindNetworkPropertyOffset("m_iPrimaryReserveAmmoCount");
	SetNetworkPropertyByOffset<int>(offset, value);
}


int WeaponMixin::GetSecondaryAmmoCount()
{
	static int offset = FindNetworkPropertyOffset("m_iSecondaryReserveAmmoCount");
	return GetNetworkPropertyByOffset<int>(offset);
}

void WeaponMixin::SetSecondaryAmmoCount(int value)
{
	static int offset = FindNetworkPropertyOffset("m_iSecondaryReserveAmmoCount");
	SetNetworkPropertyByOffset<int>(offset, value);
}