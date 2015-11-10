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
#include "modules/memory/memory_utilities.h"
using namespace boost::python;

#include "messages.h"
#include "shake.h"
#include "shareddefs.h"


//-----------------------------------------------------------------------------
// Exposes the engine module.
//-----------------------------------------------------------------------------
void export_message_functions(scope);
void export_dialog_enum(scope);
void export_user_message(scope);
void export_protobuf_message(scope);
void export_shake_command(scope);
void export_hud_destination(scope);
void export_fade_flags(scope);
void export_constants(scope);

DECLARE_SP_MODULE(_messages)
{
	export_message_functions(_messages);
	export_dialog_enum(_messages);
	export_user_message(_messages);
	export_protobuf_message(_messages);
	export_shake_command(_messages);
	export_hud_destination(_messages);
	export_fade_flags(_messages);
	export_constants(_messages);
}


//-----------------------------------------------------------------------------
// Exposes the UserMessage class
//-----------------------------------------------------------------------------
void export_user_message(scope _message)
{
	class_<CUserMessage> UserMessage("UserMessage", init<IRecipientFilter&, const char*>());

	UserMessage.add_property("recipients",
		make_function(&CUserMessage::GetRecipients, reference_existing_object_policy())
	);

	UserMessage.add_property("message_name",
		&CUserMessage::GetMessageName
	);

	UserMessage.add_property("message_index",
		&CUserMessage::GetMessageIndex
	);

	UserMessage.add_property("buffer",
		make_function(&CUserMessage::GetBuffer, reference_existing_object_policy())
	);

	UserMessage.def("send",
		&CUserMessage::Send
	);

	UserMessage.def("is_protobuf",
		&CUserMessage::IsProtobuf
	).staticmethod("is_protobuf");

	UserMessage ADD_MEM_TOOLS(CUserMessage);
}


//-----------------------------------------------------------------------------
// Exposes CProtobufMessage
//-----------------------------------------------------------------------------
void export_protobuf_message(scope _messages)
{
#ifdef USE_PROTOBUF
	class_<CProtobufMessage> ProtobufMessage("ProtobufMessage", no_init);
	
	ProtobufMessage.def("get_int32", &CProtobufMessage::GetInt32);
	ProtobufMessage.def("get_int64", &CProtobufMessage::GetInt64);
	ProtobufMessage.def("get_uint32", &CProtobufMessage::GetUInt32);
	ProtobufMessage.def("get_uint64", &CProtobufMessage::GetUInt64);
	ProtobufMessage.def("get_float", &CProtobufMessage::GetFloat);
	ProtobufMessage.def("get_double", &CProtobufMessage::GetDouble);
	ProtobufMessage.def("get_bool", &CProtobufMessage::GetBool);
	ProtobufMessage.def("get_string", &CProtobufMessage::GetString);
	ProtobufMessage.def("get_enum", &CProtobufMessage::GetEnum);
	ProtobufMessage.def("get_message", &CProtobufMessage::GetMessage);
	
	ProtobufMessage.def("get_repeated_int32", &CProtobufMessage::GetRepeatedInt32);
	ProtobufMessage.def("get_repeated_int64", &CProtobufMessage::GetRepeatedInt64);
	ProtobufMessage.def("get_repeated_uint32", &CProtobufMessage::GetRepeatedUInt32);
	ProtobufMessage.def("get_repeated_uint64", &CProtobufMessage::GetRepeatedUInt64);
	ProtobufMessage.def("get_repeated_float", &CProtobufMessage::GetRepeatedFloat);
	ProtobufMessage.def("get_repeated_double", &CProtobufMessage::GetRepeatedDouble);
	ProtobufMessage.def("get_repeated_bool", &CProtobufMessage::GetRepeatedBool);
	ProtobufMessage.def("get_repeated_string", &CProtobufMessage::GetRepeatedString);
	ProtobufMessage.def("get_repeated_enum", &CProtobufMessage::GetRepeatedEnum);
	ProtobufMessage.def("get_repeated_message", &CProtobufMessage::GetRepeatedMessage);
	
	ProtobufMessage.def("set_int32", &CProtobufMessage::SetInt32);
	ProtobufMessage.def("set_int64", &CProtobufMessage::SetInt64);
	ProtobufMessage.def("set_uint32", &CProtobufMessage::SetUInt32);
	ProtobufMessage.def("set_uint64", &CProtobufMessage::SetUInt64);
	ProtobufMessage.def("set_float", &CProtobufMessage::SetFloat);
	ProtobufMessage.def("set_double", &CProtobufMessage::SetDouble);
	ProtobufMessage.def("set_bool", &CProtobufMessage::SetBool);
	ProtobufMessage.def("set_string", &CProtobufMessage::SetString);
	ProtobufMessage.def("set_enum", &CProtobufMessage::SetEnum);
	ProtobufMessage.def("mutable_message", &CProtobufMessage::MutableMessage);
	
	ProtobufMessage.def("set_repeated_int32", &CProtobufMessage::SetRepeatedInt32);
	ProtobufMessage.def("set_repeated_int64", &CProtobufMessage::SetRepeatedInt64);
	ProtobufMessage.def("set_repeated_uint32", &CProtobufMessage::SetRepeatedUInt32);
	ProtobufMessage.def("set_repeated_uint64", &CProtobufMessage::SetRepeatedUInt64);
	ProtobufMessage.def("set_repeated_float", &CProtobufMessage::SetRepeatedFloat);
	ProtobufMessage.def("set_repeated_double", &CProtobufMessage::SetRepeatedDouble);
	ProtobufMessage.def("set_repeated_bool", &CProtobufMessage::SetRepeatedBool);
	ProtobufMessage.def("set_repeated_string", &CProtobufMessage::SetRepeatedString);
	ProtobufMessage.def("set_repeated_enum", &CProtobufMessage::SetRepeatedEnum);
	ProtobufMessage.def("mutable_repeated_message", &CProtobufMessage::MutableRepeatedMessage);
	
	ProtobufMessage.def("add_int32", &CProtobufMessage::AddInt32);
	ProtobufMessage.def("add_int64", &CProtobufMessage::AddInt64);
	ProtobufMessage.def("add_uint32", &CProtobufMessage::AddUInt32);
	ProtobufMessage.def("add_uint64", &CProtobufMessage::AddUInt64);
	ProtobufMessage.def("add_float", &CProtobufMessage::AddFloat);
	ProtobufMessage.def("add_double", &CProtobufMessage::AddDouble);
	ProtobufMessage.def("add_bool", &CProtobufMessage::AddBool);
	ProtobufMessage.def("add_string", &CProtobufMessage::AddString);
	ProtobufMessage.def("add_enum", &CProtobufMessage::AddEnum);
	ProtobufMessage.def("add_message", &CProtobufMessage::AddMessage);

	ProtobufMessage ADD_MEM_TOOLS(CProtobufMessage);
#endif
}


//-----------------------------------------------------------------------------
// Exposes functions
//-----------------------------------------------------------------------------
void export_message_functions(scope _messages)
{
	def("create_message",
		CreateMessage,
		"Creates an onscreen menu with various option buttons",
		args("edict", "message_type", "data")
	);
}


//-----------------------------------------------------------------------------
// Exposes the DIALOG_TYPE enum
//-----------------------------------------------------------------------------
void export_dialog_enum(scope _messages)
{
	enum_<DIALOG_TYPE>("DialogType")
		.value("MSG", DIALOG_MSG)
		.value("MENU", DIALOG_MENU)
		.value("TEXT", DIALOG_TEXT)
		.value("ENTRY", DIALOG_ENTRY)
		.value("ASKCONNECT", DIALOG_ASKCONNECT)
	;
}


//-----------------------------------------------------------------------------
// Exposes the ShakeCommand_t enum
//-----------------------------------------------------------------------------
void export_shake_command(scope _messages)
{
	enum_<ShakeCommand_t>("ShakeCommand")
		.value("START", SHAKE_START)
		.value("STOP", SHAKE_STOP)
		.value("AMPLITUDE", SHAKE_AMPLITUDE)
		.value("FREQUENCY", SHAKE_FREQUENCY)
		.value("START_RUMBLEONLY", SHAKE_START_RUMBLEONLY)
		.value("START_NORUMBLE", SHAKE_START_NORUMBLE)
	;
}


//-----------------------------------------------------------------------------
// Exposes the hud destination constants
//-----------------------------------------------------------------------------
void export_hud_destination(scope _messages)
{
	enum HudDestination {};

	enum_<HudDestination> _HudDestination("HudDestination");
	_HudDestination.value("NOTIFY", (HudDestination) HUD_PRINTNOTIFY);
	_HudDestination.value("CONSOLE", (HudDestination) HUD_PRINTCONSOLE);
	_HudDestination.value("TALK", (HudDestination) HUD_PRINTTALK);
	_HudDestination.value("CENTER", (HudDestination) HUD_PRINTCENTER);
}


//-----------------------------------------------------------------------------
// Exposes the fade flags
//-----------------------------------------------------------------------------
void export_fade_flags(scope _messages)
{
	enum FadeFlags{};

	enum_<FadeFlags> _FadeFlags("FadeFlags");
	_FadeFlags.value("IN", (FadeFlags) FFADE_IN);
	_FadeFlags.value("OUT", (FadeFlags) FFADE_OUT);
	_FadeFlags.value("MODULATE", (FadeFlags) FFADE_MODULATE);
	_FadeFlags.value("STAYOUT", (FadeFlags) FFADE_STAYOUT);
	_FadeFlags.value("PURGE", (FadeFlags) FFADE_PURGE);
}


//-----------------------------------------------------------------------------
// Exposes the messages specific constants
//-----------------------------------------------------------------------------
void export_constants(scope _messages)
{
	_messages.attr("SCREENFADE_FRACBITS") = SCREENFADE_FRACBITS;
}
