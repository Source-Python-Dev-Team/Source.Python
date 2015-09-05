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
// SayText2
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


//-----------------------------------------------------------------------------
// ShowMenu
//-----------------------------------------------------------------------------
#define MENU_MSG_CHUNK_SIZE 250

void SendShowMenu(IRecipientFilter& recipients, int valid_slots, int display_time, const char* menu_string)
{
#ifdef USE_PROTOBUF
	CCSUsrMsg_ShowMenu buffer = CCSUsrMsg_ShowMenu();
	buffer.set_bits_valid_slots(valid_slots);
	buffer.set_display_time(display_time);
	buffer.set_menu_string(menu_string);
	SendProtobufMessage(recipients, "ShowMenu", buffer);
#else
	int length = strlen(menu_string);
	char* data = (char *) menu_string;
	char saved_char = 0;

	// This code is taken from here:
	// https://github.com/alliedmodders/sourcemod/blob/3291e3a38f8a458c7aebc233811e9514a2ec5f11/core/MenuStyle_Radio.cpp#L503
	while (true)
	{
		// If the menu string is bigger than the chunk size, we need to split
		// the data and send it in several messages.
		if (length > MENU_MSG_CHUNK_SIZE)
		{
			// Save the char at the split position, so we can restore it later
			saved_char = data[MENU_MSG_CHUNK_SIZE];
			data[MENU_MSG_CHUNK_SIZE] = '\0';
		}

		// Send the chunked data
		bf_write* buffer = StartBitbufMessage(recipients, "ShowMenu");
		buffer->WriteWord(valid_slots);
		buffer->WriteChar(display_time);
		buffer->WriteByte(length > MENU_MSG_CHUNK_SIZE ? 1 : 0);
		buffer->WriteString(data);
		engine->MessageEnd();

		if (length > MENU_MSG_CHUNK_SIZE)
		{
			// Restore the saved char and set the data variable to the remaining data
			data[MENU_MSG_CHUNK_SIZE] = saved_char;
			data = &data[MENU_MSG_CHUNK_SIZE];
			length -= MENU_MSG_CHUNK_SIZE;
		}
		else
		{
			// If we didn't split the data in this loop, there is nothing left to send.
			break;
		}
	}
#endif
}


//-----------------------------------------------------------------------------
// HintText
//-----------------------------------------------------------------------------
void SendHintText(IRecipientFilter& recipients, const char* message)
{
#ifdef USE_PROTOBUF
	CCSUsrMsg_HintText buffer = CCSUsrMsg_HintText();
	buffer.set_text(message);
	SendProtobufMessage(recipients, "HintText", buffer);
#else
	bf_write* buffer = StartBitbufMessage(recipients, "HintText");
	buffer->WriteString(message);
	engine->MessageEnd();
#endif
}


//-----------------------------------------------------------------------------
// SayText
//-----------------------------------------------------------------------------
void SendSayText(IRecipientFilter& recipients, const char* message, int index, bool chat)
{
#ifdef USE_PROTOBUF
	// TODO: Add textallchat
	CCSUsrMsg_SayText buffer = CCSUsrMsg_SayText();
	buffer.set_ent_idx(index);
	buffer.set_text(message);
	buffer.set_chat(message);
	SendProtobufMessage(recipients, "SayText", buffer);
#else
	bf_write* buffer = StartBitbufMessage(recipients, "SayText");
	buffer->WriteByte(index);
	buffer->WriteString(message);
	buffer->WriteByte(chat);
	engine->MessageEnd();
#endif
}


//-----------------------------------------------------------------------------
// Shake
//-----------------------------------------------------------------------------
void SendShake(IRecipientFilter& recipients, ShakeCommand_t shake_command,
	float amplitude, float frequency, float duration)
{
#ifdef USE_PROTOBUF
	CCSUsrMsg_Shake buffer = CCSUsrMsg_Shake();
	buffer.set_command(shake_command);
	buffer.set_local_amplitude(amplitude);
	buffer.set_frequency(frequency);
	buffer.set_duration(duration);
	SendProtobufMessage(recipients, "Shake", buffer);
#else
	bf_write* buffer = StartBitbufMessage(recipients, "Shake");
	buffer->WriteByte(shake_command);
	buffer->WriteFloat(amplitude);
	buffer->WriteFloat(frequency);
	buffer->WriteFloat(duration);
	engine->MessageEnd();
#endif
}


//-----------------------------------------------------------------------------
// ResetHUD
//-----------------------------------------------------------------------------
void SendResetHUD(IRecipientFilter& recipients, bool reset)
{
#ifdef USE_PROTOBUF
	CCSUsrMsg_ResetHud buffer = CCSUsrMsg_ResetHud();
	buffer.set_reset(reset);
	SendProtobufMessage(recipients, "ResetHUD", buffer);
#else
	bf_write* buffer = StartBitbufMessage(recipients, "ResetHUD");
	buffer->WriteByte(reset);
	engine->MessageEnd();
#endif
}


//-----------------------------------------------------------------------------
// VGUIMenu
//-----------------------------------------------------------------------------
void SendVGUIMenu(IRecipientFilter& recipients, const char* name, bool show, dict data)
{
#ifdef USE_PROTOBUF
	CCSUsrMsg_VGUIMenu buffer = CCSUsrMsg_VGUIMenu();
	buffer.set_name(name);
	buffer.set_show(show);

	for(int i=0; i < len(data); i++)
    {
        const char* key = extract<const char*>(data.keys()[i]);
        const char* value = extract<const char*>(data[key]);

        CCSUsrMsg_VGUIMenu_Subkey* subkey = buffer.add_subkeys();
        subkey->set_name(key);
        subkey->set_str(value);
    }

	SendProtobufMessage(recipients, "VGUIMenu", buffer);
#else
	bf_write* buffer = StartBitbufMessage(recipients, "VGUIMenu");
	buffer->WriteString(name);
	buffer->WriteByte(show);
	buffer->WriteByte(len(data));

	for(int i=0; i < len(data); i++)
    {
        const char* key = extract<const char*>(data.keys()[i]);
        const char* value = extract<const char*>(data[key]);
		
		buffer->WriteString(key);
		buffer->WriteString(value);
    }

	engine->MessageEnd();
#endif
}


//-----------------------------------------------------------------------------
// TextMsg
//-----------------------------------------------------------------------------
void SendTextMsg(IRecipientFilter& recipients, int destination, const char* name,
	const char* param1, const char* param2, const char* param3, const char* param4)
{
#ifdef USE_PROTOBUF
	CCSUsrMsg_TextMsg buffer = CCSUsrMsg_TextMsg();
	buffer.set_msg_dst(destination);
	// TODO: What should we do with the "name" parameter
	buffer.add_params(param1);
	buffer.add_params(param2);
	buffer.add_params(param3);
	buffer.add_params(param4);
	SendProtobufMessage(recipients, "TextMsg", buffer);
#else
	bf_write* buffer = StartBitbufMessage(recipients, "TextMsg");
	buffer->WriteByte(destination);
	buffer->WriteString(name);
	buffer->WriteString(param1);
	buffer->WriteString(param2);
	buffer->WriteString(param3);
	buffer->WriteString(param4);
	engine->MessageEnd();
#endif
}


//-----------------------------------------------------------------------------
// KeyHintText
//-----------------------------------------------------------------------------
void SendKeyHintText(IRecipientFilter& recipients, const char* message)
{
	// TODO: How do multiple hints look like?
#ifdef USE_PROTOBUF
	CCSUsrMsg_KeyHintText buffer = CCSUsrMsg_KeyHintText();
	buffer.add_hints(message);
	SendProtobufMessage(recipients, "KeyHintText", buffer);
#else
	bf_write* buffer = StartBitbufMessage(recipients, "KeyHintText");
	buffer->WriteByte(1);
	buffer->WriteString(message);
	engine->MessageEnd();
#endif
}


//-----------------------------------------------------------------------------
// Fade
//-----------------------------------------------------------------------------
void SendFade(IRecipientFilter& recipients, int duration, int hold_time, int flags, Color color)
{
#ifdef USE_PROTOBUF
	CCSUsrMsg_Fade buffer = CCSUsrMsg_Fade();
	buffer.set_duration(duration);
	buffer.set_hold_time(hold_time);
	buffer.set_flags(flags);
	CMsgRGBA* mutable_color = buffer.mutable_clr();
	mutable_color->set_r(color.r());
	mutable_color->set_g(color.g());
	mutable_color->set_b(color.b());
	mutable_color->set_a(color.a());
	SendProtobufMessage(recipients, "Fade", buffer);
#else
	bf_write* buffer = StartBitbufMessage(recipients, "Fade");
	buffer->WriteShort(duration);
	buffer->WriteShort(hold_time);
	buffer->WriteShort(flags);
	buffer->WriteByte(color.r());
	buffer->WriteByte(color.g());
	buffer->WriteByte(color.b());
	buffer->WriteByte(color.a());
	engine->MessageEnd();
#endif
}
