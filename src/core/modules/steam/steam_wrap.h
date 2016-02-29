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

#ifndef _STEAM_WRAP_H
#define _STEAM_WRAP_H

//-----------------------------------------------------------------------------
// CLASSES
//-----------------------------------------------------------------------------
class CSteamIDExt
{
public:
	static CSteamID* Parse(const char* input)
	{
		if (!input || input[0] == '\0')
			BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Input is empty.")

		uint32 universe;
		uint32 auth_server;
		uint32 account_id;

		// Try SteamID2
		if (sscanf(input, "STEAM_%u:%u:%u", &universe, &auth_server, &account_id) == 3) {
			account_id = account_id * 2 | auth_server;

			// Some games represent the public universe as 0, others as 1
			if (universe == 0)
				universe++;

			return new CSteamID(account_id, (EUniverse) universe, k_EAccountTypeIndividual);
		}
		
		char account_type_identifier;
		uint32 account_instance;
		EAccountType account_type;
		EChatSteamIDInstanceFlags default_account_instance;

		// Try SteamID3 with account instance
		if (sscanf(input, "[%c:%u:%u:%u]", &account_type_identifier, &universe, &account_id, &account_instance) == 4) {
			if (!ParseAccountTypeIdentifier(account_type_identifier, account_type, default_account_instance))
				BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Invalid account type identifier for SteamID3 value '%s'.", input)

			return new CSteamID(account_id, account_instance, (EUniverse) universe, account_type);
		}
		
		// Try SteamID3 without account instance
		if (sscanf(input, "[%c:%u:%u]", &account_type_identifier, &universe, &account_id) == 3) {
			if (!ParseAccountTypeIdentifier(account_type_identifier, account_type, default_account_instance))
				BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Invalid account type identifier for SteamID3 value '%s'.", input)

			return new CSteamID(account_id, default_account_instance, (EUniverse) universe, account_type);
		}

		// Try SteamID64
		else {
			uint64 ulSteamid64;
			try
			{
				ulSteamid64 = boost::lexical_cast<uint64>(input);
			}
			catch(const boost::bad_lexical_cast &)
			{
				BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Input '%s' is not a SteamID2, SteamID3 or SteamID64 value.", input)
			}

			return new CSteamID(ulSteamid64);
		}
	
		return NULL;
	}

	static bool ParseAccountTypeIdentifier(char identifier, EAccountType& account_type, EChatSteamIDInstanceFlags& account_instance)
	{
		account_instance = (EChatSteamIDInstanceFlags) 1;
		switch (identifier)
		{
			case 'I':
				account_type = k_EAccountTypeInvalid;
				break;
			case 'U':
				account_type = k_EAccountTypeIndividual;
				break;
			case 'M':
				account_type = k_EAccountTypeMultiseat;
				break;
			case 'G':
				account_type = k_EAccountTypeGameServer;
				break;
			case 'A':
				account_type = k_EAccountTypeAnonGameServer;
				break;
			case 'P':
				account_type = k_EAccountTypePending;
				break;
			case 'C':
				account_type = k_EAccountTypeContentServer;
				break;
			case 'g':
				account_type = k_EAccountTypeClan;
				account_instance = (EChatSteamIDInstanceFlags) 0;
				break;
			case 'T':
				account_type = k_EAccountTypeChat;
				break;
			case 'c':
				account_type = k_EAccountTypeChat;
				account_instance = k_EChatInstanceFlagClan;
				break;
			case 'L':
				account_type = k_EAccountTypeChat;
				account_instance = k_EChatInstanceFlagLobby;
				break;
			case 'a':
				account_type = (EAccountType) 10;
				break;
			default:
				return false;
		}
		return true;
	}
};

#endif // _STEAM_WRAP_H
