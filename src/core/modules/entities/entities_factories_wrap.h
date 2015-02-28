/**
* =============================================================================
* Source Python
* Copyright (C) 2012 Source Python Development Team.  All rights reserved.
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

#ifndef _ENTITIES_FACTORIES_WRAP_H
#define _ENTITIES_FACTORIES_WRAP_H

//-----------------------------------------------------------------------------
// Includes.
//-----------------------------------------------------------------------------
#include "toolframework/itoolentity.h"
#include "game/shared/shareddefs.h"
#include "game/server/util.h"
#include "tier1/utldict.h"
#include "utility/conversions.h"


//-----------------------------------------------------------------------------
// Entity creation factory.
//-----------------------------------------------------------------------------
class CEntityFactoryDictionary : public IEntityFactoryDictionary
{
public:
	CUtlDict<IEntityFactory *, unsigned short> m_Factories;
};


//-----------------------------------------------------------------------------
// CEntityFactoryDictionary extension class.
//-----------------------------------------------------------------------------
class EntityFactoryDictionarySharedExt
{
public:
	static const char *__getitem__(CEntityFactoryDictionary *pEntityFactoryDictionary, int iIndex)
	{
		if (!pEntityFactoryDictionary->m_Factories.IsValidIndex(iIndex))
		{
			BOOST_RAISE_EXCEPTION(PyExc_IndexError, "Invalid index.");
		}
		return pEntityFactoryDictionary->m_Factories.GetElementName(iIndex);
	}
};


#endif // _ENTITIES_FACTORIES_WRAP_H
