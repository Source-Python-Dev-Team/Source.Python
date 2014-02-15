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

//-----------------------------------------------------------------------------
// Includes.
//-----------------------------------------------------------------------------
#include "modules/export_main.h"
#include "utility/sp_util.h"
#include "usermessage.h"
#include "Color.h"

//-----------------------------------------------------------------------------
// Namespaces to use
//-----------------------------------------------------------------------------
using namespace boost::python;

//-----------------------------------------------------------------------------
// Exposer functions.
//-----------------------------------------------------------------------------
void export_usermessage_interface();
void export_message_functions();
void export_dialog_enum();
void export_color();

//-----------------------------------------------------------------------------
// Method overloads
//-----------------------------------------------------------------------------
DECLARE_CLASS_METHOD_OVERLOAD(CUserMessage, set_char,	2, 3);
DECLARE_CLASS_METHOD_OVERLOAD(CUserMessage, set_byte,	2, 3);
DECLARE_CLASS_METHOD_OVERLOAD(CUserMessage, set_short,	2, 3);
DECLARE_CLASS_METHOD_OVERLOAD(CUserMessage, set_long,	2, 3);
DECLARE_CLASS_METHOD_OVERLOAD(CUserMessage, set_float,	2, 3);
DECLARE_CLASS_METHOD_OVERLOAD(CUserMessage, set_bool,	2, 3);
DECLARE_CLASS_METHOD_OVERLOAD(CUserMessage, set_string,	2, 3);
DECLARE_CLASS_METHOD_OVERLOAD(CUserMessage, set_buffer,	3, 4);

//-----------------------------------------------------------------------------
// Exposes the engine module.
//-----------------------------------------------------------------------------
DECLARE_SP_MODULE(usermessage_c)
{
	export_usermessage_interface();
	export_message_functions();
	export_dialog_enum();
	export_color();
}

void export_usermessage_interface()
{
	// ----------------------------------------------------------
	// The Usermessage interface.
	// ----------------------------------------------------------
	BOOST_CLASS_CONSTRUCTOR( CUserMessage, const MRecipientFilter &, const char * )

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
	;
}

void export_message_functions()
{
	def("create_message",
		CreateMessage,
		"Creates an onscreen menu with various option buttons"
	);
}

void export_dialog_enum()
{
	enum_<DIALOG_TYPE>("DialogType")
		.value("MSG", DIALOG_MSG)
		.value("MENU", DIALOG_MENU)
		.value("TEXT", DIALOG_TEXT)
		.value("ENTRY", DIALOG_ENTRY)
		.value("ASKCONNECT", DIALOG_ASKCONNECT)
	;
}

//---------------------------------------------------------------------------------
// Exposes the Color class
//---------------------------------------------------------------------------------
class ColorExt
{
public:
	static tuple GetColor(Color* pColor)
	{
		list color;
		int r, g, b, a;
		pColor->GetColor(r, g, b, a);
		color.append(r);
		color.append(g);
		color.append(b);
		color.append(a);
		return tuple(color);
	}
};

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(set_color_overload, SetColor, 3, 4);

void export_color()
{
	class_<Color>("Color")
		.def(init<int, int, int>())
		.def(init<int, int, int, int>())

		.def("set_color",
			&Color::SetColor,
			set_color_overload(
				"Sets the color in a RGB(A) format (0-255).",
				args("r", "g", "b", "a")
			)
		)

		.def("get_color",
			&ColorExt::GetColor,
			"Returns the color as a tuple containing the RGBA values."
		)

		.def("__getitem__",
			&GetItemIndexer<Color, unsigned char>,
			"Returns the color at the given index (0-3)."
		)

		.def("__setitem__",
			&SetItemIndexer<Color, unsigned char>,
			"Sets the color at the given index (0-3)."
		)

		.def(self == self)
		.def(self != self)
	;
}