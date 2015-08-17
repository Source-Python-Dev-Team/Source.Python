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

#ifndef UNDEFINED_SYMBOLS_BLADE_H
#define UNDEFINED_SYMBOLS_BLADE_H

//-----------------------------------------------------------------------------
// Includes.
//-----------------------------------------------------------------------------
#include "dt_send.h"
#include "game/shared/ehandle.h"
#include "isaverestore.h"
#include "datamap.h"
#include "game/shared/takedamageinfo.h"


//-----------------------------------------------------------------------------
// CTakeDamageInfo constructor declaration.
//-----------------------------------------------------------------------------
CTakeDamageInfo::CTakeDamageInfo()
{
	m_hInflictor = NULL;
	m_hAttacker = NULL;
	m_hWeapon = NULL;
	m_flDamage = 0.0f;
	m_flBaseDamage = BASEDAMAGE_NOT_SPECIFIED;
	m_bitsDamageType = 0;
	m_iDamageCustom = 0;
	m_flMaxDamage = 0.0f;
	m_vecDamageForce = vec3_origin;
	m_vecDamagePosition = vec3_origin;
	m_vecReportedPosition = vec3_origin;
	m_iAmmoType = -1;
}

#endif // _UNDEFINED_SYMBOLS_BLADE_H
