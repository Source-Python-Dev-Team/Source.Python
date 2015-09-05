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

#ifndef _MESSAGES_H
#define _MESSAGES_H

//-----------------------------------------------------------------------------
// Includes.
//-----------------------------------------------------------------------------
#include "irecipientfilter.h"
#include "utilities/wrap_macros.h"

#include "public/engine/iserverplugin.h"


//-----------------------------------------------------------------------------
// Usermessages
//-----------------------------------------------------------------------------
void SendSayText2(IRecipientFilter& recipients, const char* message,
	int index=0, bool chat=false, const char* param1="", const char* param2="",
	const char* param3="", const char* param4="");

void SendShowMenu(IRecipientFilter& recipients, int valid_slots,
	int display_time, const char* menu_string);

void SendHintText(IRecipientFilter& recipients, const char* message);
void SendSayText(IRecipientFilter& recipients, const char* message, int index=0, bool chat=false);


//-----------------------------------------------------------------------------
// Functions.
//-----------------------------------------------------------------------------
void CreateMessage( edict_t *pEdict, DIALOG_TYPE type, KeyValues *data );

#endif // _MESSAGES_H
