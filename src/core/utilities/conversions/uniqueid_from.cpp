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
#include "strtools.h"


//-----------------------------------------------------------------------------
// Returns a unique ID from the given PlayerInfo instance.
//-----------------------------------------------------------------------------
bool UniqueIDFromPlayerInfo2( IPlayerInfo* pInfo, char*& output )
{
	if (!pInfo)
		return false;

	const char* szSteamID = pInfo->GetNetworkIDString();
	if (pInfo->IsFakeClient() || V_strstr(szSteamID, "BOT"))
	{
		sprintf(output, "BOT_%s", pInfo->GetName());
		return true;
	}

	if (!V_strstr(szSteamID, "LAN")) 
	{
		V_strcpy(output, szSteamID);
		return true;
	}

	const char* szAddr = NULL;
	if (!AddressFromPlayerInfo2(pInfo, szAddr))
		return false;

	sprintf(output, "LAN_%s", szAddr);

	// Replace dots with underscores and terminate the string before the colon
	char* pResult = (char*) output;
	while (*pResult != '\0')
	{
		if (*pResult == '.')
		{
			*pResult = '_';
		}
		else if (*pResult == ':')
		{
			*pResult = '\0';
			break;
		}

		++pResult;
	}

	return true;
}

bool UniqueIDFromPlayerInfo( IPlayerInfo* pInfo, str& output )
{
	char result[UNIQUE_ID_SIZE] = "";
	char* pResult = (char*) result;
	if (!UniqueIDFromPlayerInfo2(pInfo, pResult))
		return false;

	output = str(result);
	return true;
}


//-----------------------------------------------------------------------------
// Returns a unique ID from the given PlayerInfo instance.
//-----------------------------------------------------------------------------
bool UniqueIDFromIndex( unsigned int iIndex, str& output )
{
	IPlayerInfo* pInfo = NULL;
	if (!PlayerInfoFromIndex(iIndex, pInfo))
		return false;

	return UniqueIDFromPlayerInfo(pInfo, output);
}