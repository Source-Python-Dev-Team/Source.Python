/**
* =============================================================================
* Source Python
* Copyright (C) 2015 Source Python Development Team.  All rights reserved.
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

#ifndef _WEAPONS_SCRIPTS_BMS_WRAP_H
#define _WEAPONS_SCRIPTS_BMS_WRAP_H

//-----------------------------------------------------------------------------
// Includes.
//-----------------------------------------------------------------------------
#include "datamap.h"
#include "game/shared/weapon_parse.h"
#include "tier1/utldict.h"
#include "utilities/wrap_macros.h"


//-----------------------------------------------------------------------------
// WeaponDataBase_t declaration.
//-----------------------------------------------------------------------------
typedef CUtlDict<FileWeaponInfo_t *, unsigned short> WeaponDataBase_t;


//-----------------------------------------------------------------------------
// WeaponDataBase_t extension class.
//-----------------------------------------------------------------------------
class WeaponDataBaseExt
{
public:
	static CPointer *_find(WeaponDataBase_t *pWeaponDataBase, WEAPON_FILE_INFO_HANDLE uiIndex)
	{
		if (uiIndex >= pWeaponDataBase->Count())
			BOOST_RAISE_EXCEPTION(PyExc_IndexError, "Index out of range.");

		return new CPointer((unsigned long)(void *)pWeaponDataBase->Element(uiIndex));
	}
};


#endif // _WEAPONS_SCRIPTS_BMS_WRAP_H
