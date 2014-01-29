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

#include "modules/export_main.h"
#include "usermessage.h"

//---------------------------------------------------------------------------------
// Namespaces to use
//---------------------------------------------------------------------------------
using namespace boost::python;

//---------------------------------------------------------------------------------
// Exposer functions.
//---------------------------------------------------------------------------------
void export_usermessage_interface();

//---------------------------------------------------------------------------------
// Method overloads
//---------------------------------------------------------------------------------
DECLARE_CLASS_METHOD_OVERLOAD(CUserMessage, set_char,	2, 3);
DECLARE_CLASS_METHOD_OVERLOAD(CUserMessage, set_byte,	2, 3);
DECLARE_CLASS_METHOD_OVERLOAD(CUserMessage, set_short,	2, 3);
DECLARE_CLASS_METHOD_OVERLOAD(CUserMessage, set_long,	2, 3);
DECLARE_CLASS_METHOD_OVERLOAD(CUserMessage, set_float,	2, 3);
DECLARE_CLASS_METHOD_OVERLOAD(CUserMessage, set_bool,	2, 3);
DECLARE_CLASS_METHOD_OVERLOAD(CUserMessage, set_string,	2, 3);
DECLARE_CLASS_METHOD_OVERLOAD(CUserMessage, set_buffer,	3, 4);

//---------------------------------------------------------------------------------
// Exposes the engine module.
//---------------------------------------------------------------------------------
DECLARE_SP_MODULE(usermessage_c)
{
	export_usermessage_interface();
}

void export_usermessage_interface()
{
	// ----------------------------------------------------------
	// The Usermessage interface.
	// ----------------------------------------------------------
	BOOST_CLASS_CONSTRUCTOR( CUserMessage, const CMRecipientFilter &, const char * )

		CLASS_METHOD(CUserMessage,
			send_message,
			"Sends the usermessage to the client recipients."
		)

		CLASS_METHOD(CUserMessage,
			get_message_name,
			"Retrieves the string name of the message."
		)

		CLASS_METHOD(CUserMessage,
			get_message_index,
			"Retrieves the integer index of the message. -1 means the message is not valid."
		)

		CLASS_METHOD(CUserMessage,
			get_recipient_filter,
			"Retrieves the clients this message will be sent to.",
			reference_existing_object_policy()
		)

		CLASS_METHOD(CUserMessage,
			has_been_sent,
			"Whether the message has been sent to the clients."
		)

		CLASS_METHOD_OVERLOAD(CUserMessage,
			set_char,
			"Sets a field parameter to the specified value.",
			args("field_name", "field_value", "index")
		)

		CLASS_METHOD_OVERLOAD(CUserMessage,
			set_byte,
			"Sets a field parameter to the specified value.",
			args("field_name", "field_value", "index")
		)

		CLASS_METHOD_OVERLOAD(CUserMessage,
			set_short,
			"Sets a field parameter to the specified value.",
			args("field_name", "field_value", "index")
		)

		CLASS_METHOD_OVERLOAD(CUserMessage,
			set_long,
			"Sets a field parameter to the specified value.",
			args("field_name", "field_value", "index")
		)

		CLASS_METHOD_OVERLOAD(CUserMessage,
			set_float,
			"Sets a field parameter to the specified value.",
			args("field_name", "field_value", "index")
		)

		CLASS_METHOD_OVERLOAD(CUserMessage,
			set_bool,
			"Sets a field parameter to the specified value.",
			args("field_name", "field_value", "index")
		)

		CLASS_METHOD_OVERLOAD(CUserMessage,
			set_buffer,
			"Sets a field parameter to the specified value.",
			args("field_name", "field_value", "index")
		)

		CLASS_METHOD_OVERLOAD(CUserMessage,
			set_string,
			"Sets a field parameter to the specified value.",
			args("field_name", "field_value", "index")
		)

	BOOST_END_CLASS()
}