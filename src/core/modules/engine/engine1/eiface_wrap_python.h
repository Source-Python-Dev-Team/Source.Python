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

#include "eiface.h"

template<class T>
void IVEngineServer_Visitor(T cls)
{
	cls
		.def("index_of_edict",
			&IVEngineServer::IndexOfEdict,
			"Returns the edict's index",
			args("edict")
		)

		.def("edict_of_index",
			&IVEngineServer::PEntityOfEntIndex,
			"Returns the corresponding edict for the given index.",
			args("index"),
			reference_existing_object_policy()
		)

		.def("user_message_begin",
			&IVEngineServer::UserMessageBegin,
			"Begins a user message from the server to the client .dll.",
			args("filter", "message_type"),
			reference_existing_object_policy()
		)

		.def("get_time",
			&IVEngineServer::Time,
			"Returns a high precision timer for doing profiling work"
		)

		.def("multiplayer_end_game",
			&IVEngineServer::MultiplayerEndGame,
			"Matchmaking"
		)

		.def("change_team",
			&IVEngineServer::ChangeTeam
		)

		.def("create_fake_client_ex",
			&IVEngineServer::CreateFakeClientEx,
			reference_existing_object_policy()
		)

		.def("get_server_version",
			&IVEngineServer::GetServerVersion
		)

		/*
		TODO: void*
		.def("get_replay",
			&IVEngineServer::GetReplay
		)
		*/
	;
}
