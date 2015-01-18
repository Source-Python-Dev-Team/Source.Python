/**
* =============================================================================
* Source Python
* Copyright (C) 2014 Source Python Development Team.  All rights reserved.
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

#ifndef _ENTITIES_HELPERS_WRAP_CSGO_H
#define _ENTITIES_HELPERS_WRAP_CSGO_H

//-----------------------------------------------------------------------------
// Includes.
//-----------------------------------------------------------------------------
#include "utility/wrap_macros.h"
#include "toolframework/itoolentity.h"
#include "modules/conversions/conversions_wrap.h"
#include "modules/memory/memory_tools.h"


//-----------------------------------------------------------------------------
// External variables.
//-----------------------------------------------------------------------------
extern IServerTools *servertools;


//-----------------------------------------------------------------------------
// Removes the given entity index...
//-----------------------------------------------------------------------------
void remove_entity(unsigned int uiEntityIndex)
{
	CPointer *pEntity = PointerFromIndex(uiEntityIndex);
	if (!pEntity)
	{
		BOOST_RAISE_EXCEPTION(PyExc_IndexError, "Unable to find an entity " \
			"matching the given index \"%u\".", uiEntityIndex);
	}
	CBaseEntity *pBaseEntity = (CBaseEntity *)pEntity->m_ulAddr;
	int iHammerID = 0;
	while (servertools->FindEntityByHammerID(iHammerID))
	{
		iHammerID++;
	}
	servertools->SetKeyValue(pBaseEntity, "hammerid", iHammerID);
	servertools->RemoveEntity(iHammerID);
}


#endif // _ENTITIES_HELPERS_WRAP_CSGO_H
