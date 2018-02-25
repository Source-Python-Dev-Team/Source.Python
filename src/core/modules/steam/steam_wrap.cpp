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
#include "export_main.h"
#include "modules/memory/memory_tools.h"
#include "steam/steamclientpublic.h"
#include "steam_wrap.h"

#include ENGINE_INCLUDE_PATH(steam_wrap.h)

//-----------------------------------------------------------------------------
// Forward declarations.
//-----------------------------------------------------------------------------
void export_steamid(scope);
void export_universe(scope);
void export_account_type(scope);


//-----------------------------------------------------------------------------
// Declare the _steam module.
//-----------------------------------------------------------------------------
DECLARE_SP_MODULE(_steam)
{
	export_steamid(_steam);
	export_universe(_steam);
	export_account_type(_steam);
}


//-----------------------------------------------------------------------------
// Exports CSteamID.
//-----------------------------------------------------------------------------
void export_steamid(scope _steam)
{
	struct steamid_pickle_suite: pickle_suite
	{
		static tuple getinitargs(const CSteamID& steamid)
		{
			return make_tuple(steamid.ConvertToUint64());
		}
	};

	class_<CSteamID, CSteamID*> SteamID("SteamID");

	SteamID.def_pickle(steamid_pickle_suite());
	
	// Constructors

	//-----------------------------------------------------------------------------
	// Purpose: Constructor
	// Input  : unAccountID -	32-bit account ID
	//			eUniverse -		Universe this account belongs to
	//			eAccountType -	Type of account
	//-----------------------------------------------------------------------------
	SteamID.def(init<uint32, EUniverse, EAccountType>());

	//-----------------------------------------------------------------------------
	// Purpose: Constructor
	// Input  : unAccountID -	32-bit account ID
	//			unAccountInstance - instance 
	//			eUniverse -		Universe this account belongs to
	//			eAccountType -	Type of account
	//-----------------------------------------------------------------------------
	SteamID.def(init<uint32, unsigned int, EUniverse, EAccountType>());

	//-----------------------------------------------------------------------------
	// Purpose: Constructor
	// Input  : ulSteamID -		64-bit representation of a Steam ID
	// Note:	Will not accept a uint32 or int32 as input, as that is a probable mistake.
	//			See the stubbed out overloads in the private: section for more info.
	//-----------------------------------------------------------------------------
	SteamID.def(init<uint64>());

	SteamID.def(
		"parse",
		&CSteamIDExt::Parse, manage_new_object_policy(),
		"Parse a SteamID2, SteamID3 or SteamID64 string representation and create a new SteamID object."
	).staticmethod("parse");

	// Methods
	SteamID.def(
		"set",
		&CSteamID::Set,
		"Set parameters for SteamID.",
		("account_id", "universe", "account_type")
	);

	SteamID.def(
		"instanced_set", 
		&CSteamID::InstancedSet,
		"Set parameters for SteamID.",
		("account_id", "instance", "universe", "account_type")
	);

	SteamID.def(
		"full_set", 
		&CSteamID::FullSet,
		"Initialize a Steam ID from its 52-bit parts and universe/type.",
		("identifier", "universe", "account_type")
	);

	SteamID.def(
		"set_from_uint64",
		&CSteamID::SetFromUint64,
		"Initialize a Steam ID from its 64-bit representation.",
		("steam_id")
	);

	SteamID.def(
		"to_uint64", 
		&CSteamID::ConvertToUint64,
		"Convert a Steam ID to its 64-bit representation."
	);

	SteamID.def("to_steamid2",
		&CSteamIDExt::ToSteamID2,
		"Convert the Steam ID to its SteamID2 string representation."
	);

	SteamID.def("to_steamid3",
		&CSteamIDExt::ToSteamID3,
		"Convert the Steam ID to its SteamID3 string representation."
	);

	SteamID.add_property(
		"static_account_key", 
		&CSteamID::GetStaticAccountKey,
		"Convert the static parts of a Steam ID to a 64-bit representation. For " \
		"multiseat accounts, all instances of that account will have the same " \
		"static account key, so they can be grouped together by the static account key."
	);

	SteamID.def(
		"create_blank_anonymous_logon",
		&CSteamID::CreateBlankAnonLogon,
		"Create an anonymous game server login to be filled in by the AM.",
		("universe")
	);

	SteamID.def(
		"is_blank_anonymous_account",
		&CSteamID::BBlankAnonAccount,
		"Return True if this is an anonymous game server login that will be filled in."
	);

	SteamID.def(
		"is_game_server_account",
		&CSteamID::BGameServerAccount,
		"Return True if this is a game server account ID."
	);

	SteamID.def(
		"is_content_server_account", 
		&CSteamID::BContentServerAccount,
		"Return True if this is a content server account ID."
	);

	SteamID.def(
		"is_clan_account",
		&CSteamID::BClanAccount,
		"Return True if this is a clan account ID."
	);

	SteamID.def(
		"is_chat_account",
		&CSteamID::BChatAccount,
		"Return True if this a chat account ID."
	);

	SteamID.def(
		"is_individual_account", 
		&CSteamID::BIndividualAccount,
		"Return True if this is an individual user account ID."
	);

	SteamID.add_property("account_instance", &CSteamID::GetUnAccountInstance);
	SteamID.add_property("account_type", &CSteamID::GetEAccountType);
	SteamID.def("is_valid", &CSteamID::IsValid);
	SteamID.def(self == self);
	SteamID.def(self != self);
	SteamID.def(self < self);
	// TODO: Add self > self. Missing in Orangebox

	// Properties
	SteamID.add_property(
		"account_id", 
		&CSteamID::GetAccountID,
		&CSteamID::SetAccountID,
		"A property to get/set the account ID."
	);

	SteamID.add_property(
		"universe", 
		&CSteamID::GetEUniverse, 
		&CSteamID::SetEUniverse,
		"A property to get/set the universe."
	);

	// Game/engine specific methods
	// Available in BM:S, CS:GO
	SteamID.NOT_IMPLEMENTED("create_blank_anonymous_user_account");
	SteamID.NOT_IMPLEMENTED("is_lobby");
	SteamID.NOT_IMPLEMENTED("is_anonymous_account");
	SteamID.NOT_IMPLEMENTED("is_anonymous_user_account");

	// Available in BM:S
	SteamID.NOT_IMPLEMENTED("clear"); // Easy to implement on our own
	SteamID.NOT_IMPLEMENTED("is_persistent_game_server_account");
	SteamID.NOT_IMPLEMENTED("is_anonymous_game_server_account");
	SteamID.NOT_IMPLEMENTED("is_console_user_account");
	SteamID.NOT_IMPLEMENTED("clear_individual_instance");
	SteamID.NOT_IMPLEMENTED("has_no_individual_instance");

	export_engine_specific_steamid(_steam, SteamID);

	// Add memory tools
	SteamID ADD_MEM_TOOLS(CSteamID);
}


//-----------------------------------------------------------------------------
// Exports EUniverse.
//-----------------------------------------------------------------------------
void export_universe(scope _steam)
{
	enum_<EUniverse> Universe("Universe");
	
	Universe.value("INVALID", k_EUniverseInvalid);
	Universe.value("PUBLIC", k_EUniversePublic);
	Universe.value("BETA", k_EUniverseBeta);
	Universe.value("INTERNAL", k_EUniverseInternal);
	Universe.value("DEV", k_EUniverseDev);
	Universe.value("MAX", k_EUniverseMax);

	// Game/engine specific values
	// Available in Orangebox, CS:GO, L4D2
	NOT_IMPLEMENTED_VALUE(EUniverse, "RC");
	
	export_engine_specific_universe(_steam, Universe);
}


//-----------------------------------------------------------------------------
// Exports EAccountType.
//-----------------------------------------------------------------------------
void export_account_type(scope _steam)
{
	enum_<EAccountType> AccountType("AccountType");

	AccountType.value("INVALID", k_EAccountTypeInvalid);
	AccountType.value("INDIVIDUAL", k_EAccountTypeIndividual);
	AccountType.value("MULTISEAT", k_EAccountTypeMultiseat);
	AccountType.value("GAME_SERVER", k_EAccountTypeGameServer);
	AccountType.value("ANONYMOUS_GAME_SERVER", k_EAccountTypeAnonGameServer);
	AccountType.value("PENDING", k_EAccountTypePending);
	AccountType.value("CONTENT_SERVER", k_EAccountTypeContentServer);
	AccountType.value("CLAN", k_EAccountTypeClan);
	AccountType.value("CHAT", k_EAccountTypeChat);
	AccountType.value("ANONYMOUS_USER", (EAccountType) 10);

	// Game/engine specific values
	// Available in Orangebox, L4D2
	NOT_IMPLEMENTED_VALUE(EAccountType, "P2P_SUPER_SEEDER");

	// Available in BM:S
	NOT_IMPLEMENTED_VALUE(EAccountType, "CONSOLE_USER");
	
	export_engine_specific_account_type(_steam, AccountType);
}
