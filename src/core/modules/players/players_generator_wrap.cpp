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

// ----------------------------------------------------------------------------
// Includes
// ----------------------------------------------------------------------------
#include "players_generator_wrap.h"
#include "utility/sp_util.h"
#include "game/server/iplayerinfo.h"
#include "boost/python/iterator.hpp"
#include "modules/conversions/conversions_wrap.h"

// ----------------------------------------------------------------------------
// External variables.
// ----------------------------------------------------------------------------
extern IPlayerInfoManager* playerinfomanager;

// ----------------------------------------------------------------------------
// PlayerGenerator Constructor.
// ----------------------------------------------------------------------------
PlayerGenerator::PlayerGenerator( PyObject* self ):
	IPythonGenerator<IPlayerInfo>(self),
	m_iEntityIndex(0)
{
}

// ----------------------------------------------------------------------------
// PlayerGenerator Copy-Constructor.
// ----------------------------------------------------------------------------
PlayerGenerator::PlayerGenerator( PyObject* self, const PlayerGenerator& rhs ):
	IPythonGenerator<IPlayerInfo>(self),
	m_iEntityIndex(rhs.m_iEntityIndex)
{
}

// ----------------------------------------------------------------------------
// PlayerGenerator Destructor.
// ----------------------------------------------------------------------------
PlayerGenerator::~PlayerGenerator()
{
}

// ----------------------------------------------------------------------------
// Returns the next valid IPlayerInfo instance.
// ----------------------------------------------------------------------------
IPlayerInfo* PlayerGenerator::getNext()
{
	IPlayerInfo* pPlayerInfo = NULL;
	while(m_iEntityIndex < gpGlobals->maxClients)
	{
		m_iEntityIndex++;
		edict_t* pEdict = EdictFromIndex(m_iEntityIndex);
		pPlayerInfo = playerinfomanager->GetPlayerInfo(pEdict);
		if ( pPlayerInfo )
		{
			return pPlayerInfo;
		}
	}
	return NULL;
}
