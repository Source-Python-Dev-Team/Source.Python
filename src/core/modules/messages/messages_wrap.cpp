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
using namespace boost::python;

#include "modules/memory/memory_tools.h"

#include "utilities/sp_util.h"
#include "messages.h"

//-----------------------------------------------------------------------------
// Exposes the engine module.
//-----------------------------------------------------------------------------
void export_usermessage_interface(scope);
void export_message_functions(scope);
void export_dialog_enum(scope);

DECLARE_SP_MODULE(_messages)
{
	export_usermessage_interface(_messages);
	export_message_functions(_messages);
	export_dialog_enum(_messages);
}

//-----------------------------------------------------------------------------
// Exposes the UserMessage class
//-----------------------------------------------------------------------------
void export_usermessage_interface(scope _messages)
{
	class_<CUserMessage>("UserMessage", init<const MRecipientFilter &, const char *>())
		.def("send_message",
			&CUserMessage::send_message,
			"Sends the usermessage to the client recipients."
		)

		.def("get_message_name",
			&CUserMessage::get_message_name,
			"Retrieves the string name of the message."
		)

		.def("get_message_index",
			&CUserMessage::get_message_index,
			"Retrieves the integer index of the message. -1 means the message is not valid."
		)

		.def("get_recipient_filter",
			&CUserMessage::get_recipient_filter,
			"Retrieves the clients this message will be sent to.",
			reference_existing_object_policy()
		)

		.def("has_been_sent",
			&CUserMessage::has_been_sent,
			"Whether the message has been sent to the clients."
		)

		.def("set_char",
			&CUserMessage::set_char,
			"Sets a field parameter to the specified value.",
			("field_name", "field_value", arg("index")=-1)
		)

		.def("set_byte",
			&CUserMessage::set_byte,
			"Sets a field parameter to the specified value.",
			("field_name", "field_value", arg("index")=-1)
		)

		.def("set_short",
			&CUserMessage::set_short,
			"Sets a field parameter to the specified value.",
			("field_name", "field_value", arg("index")=-1)
		)

		.def("set_long",
			&CUserMessage::set_long,
			"Sets a field parameter to the specified value.",
			("field_name", "field_value", arg("index")=-1)
		)

		.def("set_float",
			&CUserMessage::set_float,
			"Sets a field parameter to the specified value.",
			("field_name", "field_value", arg("index")=-1)
		)

		.def("set_bool",
			&CUserMessage::set_bool,
			"Sets a field parameter to the specified value.",
			("field_name", "field_value", arg("index")=-1)
		)

		.def("set_color",
			&CUserMessage::set_color,
			"Sets a field parameter to the specified value.",
			("field_name", "field_value", arg("index")=-1)
		)
		
		.def("set_buffer",
			&CUserMessage::set_buffer,
			"Sets a field parameter to the specified value.",
			("field_name", "buffer", "num_bytes", arg("index")=-1)
		)

		.def("set_string",
			&CUserMessage::set_string,
			"Sets a field parameter to the specified value.",
			("field_name", "field_value", arg("index")=-1)
		)
	;
}

void export_message_functions(scope _messages)
{
	def("create_message",
		CreateMessage,
		"Creates an onscreen menu with various option buttons",
		args("edict", "message_type", "data")
	);
}

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
