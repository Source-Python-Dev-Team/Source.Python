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
// HELPERS
//-----------------------------------------------------------------------------
#ifndef USE_PROTOBUF
static int GetUsermessageIndex(const char* message_name)
{
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
}
#endif


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
	
#else
	bf_write* buffer = engine->UserMessageBegin(&recipients, GetUsermessageIndex("SayText2"));
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