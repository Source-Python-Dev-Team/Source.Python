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
#include "messages.h"
#include "public/engine/iserverplugin.h"
#include "eiface.h"
#include "sp_main.h"


//-----------------------------------------------------------------------------
// Externals.
//-----------------------------------------------------------------------------
extern IServerPluginHelpers *helpers;
extern CSourcePython g_SourcePythonPlugin;
extern IServerGameDLL *servergamedll;


//-----------------------------------------------------------------------------
// Forward declarations.
//-----------------------------------------------------------------------------
int GetUserMessageIndex(const char* message_name);


//-----------------------------------------------------------------------------
// Helpers.
//-----------------------------------------------------------------------------
#ifdef USE_PROTOBUF
	#include "game/shared/csgo/protobuf/cstrike15_usermessages.pb.h"
	#include "public/game/shared/csgo/protobuf/cstrike15_usermessage_helpers.h"

	static void SendProtobufMessage(IRecipientFilter& recipients, const char* message_name, google::protobuf::Message& buffer)
	{
		engine->SendUserMessage(recipients, GetUserMessageIndex(message_name), buffer);
	}
#else
	static bf_write* StartBitbufMessage(IRecipientFilter& recipients, const char* message_name)
	{
	#ifdef ENGINE_LEFT4DEAD2
		return engine->UserMessageBegin(&recipients, GetUserMessageIndex(message_name), message_name);
	#else
		return engine->UserMessageBegin(&recipients, GetUserMessageIndex(message_name));
	#endif
	}
#endif

static int GetUserMessageIndex(const char* message_name)
{
#ifdef USE_PROTOBUF
	return g_Cstrike15UsermessageHelpers.GetIndex(message_name);
#else
	char sz_mname[256];
	int sizereturn;
	int index = 0;
	while (servergamedll->GetUserMessageInfo(index, sz_mname, 255, sizereturn))
	{
		if (strcmp(message_name, sz_mname) == 0)
		{
			return index;
		}
		index++;
	}
	return 0;
#endif
}


//-----------------------------------------------------------------------------
// Functions.
//-----------------------------------------------------------------------------
void CreateMessage( edict_t *pEdict, DIALOG_TYPE type, KeyValues *data )
{
	helpers->CreateMessage(pEdict, type, data, &g_SourcePythonPlugin);
}


//-----------------------------------------------------------------------------
// Usermessages
//-----------------------------------------------------------------------------
void SendSayText2(IRecipientFilter& recipients, const char* message,
	int index, bool chat, const char* param1, const char* param2,
	const char* param3, const char* param4)
{
#ifdef USE_PROTOBUF
	// TODO: Add textchatall
	CCSUsrMsg_SayText2 buffer = CCSUsrMsg_SayText2();
	buffer.set_msg_name(message);
	buffer.set_chat(chat);
	buffer.set_ent_idx(index);
	buffer.add_params(param1);
	buffer.add_params(param2);
	buffer.add_params(param3);
	buffer.add_params(param4);
	SendProtobufMessage(recipients, "SayText2", buffer);

#else
	bf_write* buffer = StartBitbufMessage(recipients, "SayText2");
	buffer->WriteByte(index);
	buffer->WriteByte(chat);
	buffer->WriteString(message);
	buffer->WriteString(param1);
	buffer->WriteString(param2);
	buffer->WriteString(param3);
	buffer->WriteString(param4);
	engine->MessageEnd();
#endif
}