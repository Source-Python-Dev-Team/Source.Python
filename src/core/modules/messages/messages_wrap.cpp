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

#include "messages.h"


//-----------------------------------------------------------------------------
// Exposes the engine module.
//-----------------------------------------------------------------------------
void export_usermessages(scope);
void export_message_functions(scope);
void export_dialog_enum(scope);
void export_shake_command(scope);

DECLARE_SP_MODULE(_messages)
{
	export_usermessages(_messages);
	export_message_functions(_messages);
	export_dialog_enum(_messages);
	export_shake_command(_messages);

	/*
	TODO: Expose these constants
	#define HUD_PRINTNOTIFY		1
	#define HUD_PRINTCONSOLE	2
	#define HUD_PRINTTALK		3
	#define HUD_PRINTCENTER		4
	*/
}


//-----------------------------------------------------------------------------
// Exposes the UserMessage class
//-----------------------------------------------------------------------------
void export_usermessages(scope _messages)
{
	def("send_say_text2",
		&SendSayText2,
		("recipients", "message", arg("index")=0, arg("chat")=false,
			arg("param1")="", arg("param2")="", arg("param3")="", arg("param4")="")
	);

	def("send_show_menu",
		&SendShowMenu
	);

	def("send_hint_text",
		&SendHintText
	);

	def("send_say_text",
		&SendSayText,
		("recipients", "text", arg("index")=0, arg("chat")=false)
	);

	def("send_shake",
		&SendShake
	);

	def("send_reset_hud",
		&SendResetHUD
	);

	def("send_vgui_menu",
		&SendVGUIMenu
	);

	def("send_text_msg",
		&SendTextMsg,
		("recipients", "destination", "name", arg("param1")="", arg("param2")="", arg("param3")="", arg("param4")="")
	);
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
