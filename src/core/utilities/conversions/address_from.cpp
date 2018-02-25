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

//-----------------------------------------------------------------------------
// Includes.
//-----------------------------------------------------------------------------
#include "../conversions.h"
#include "inetchannelinfo.h"


//-----------------------------------------------------------------------------
// Returns an address from the given PlayerInfo instance.
//-----------------------------------------------------------------------------
bool AddressFromPlayerInfo2( IPlayerInfo* pInfo, const char*& output )
{
	if (!pInfo)
		return false;

	if (pInfo->IsFakeClient() || V_strstr(pInfo->GetNetworkIDString(), "BOT"))
	{
		output = "";
		return true;
	}

	unsigned int index;
	if (!IndexFromPlayerInfo(pInfo, index))
		return false;

	INetChannelInfo* netinfo = engine->GetPlayerNetInfo(index);
	if (!netinfo)
		return false;

	output = netinfo->GetAddress();
	return true;
}

bool AddressFromPlayerInfo( IPlayerInfo* pInfo, str& output )
{
	const char* result = NULL;
	if (!AddressFromPlayerInfo2(pInfo, result))
		return false;

	output = str(result);
	return true;
}
