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
// CUserMessage.
//-----------------------------------------------------------------------------
CUserMessage::CUserMessage(IRecipientFilter& recipients, const char* message_name):
	m_recipients(recipients)
{
	m_message_name = message_name;
	m_index = ::GetMessageIndex(message_name);

	// Initialize buffer
#ifdef USE_PROTOBUF
	const google::protobuf::Message* message = g_Cstrike15UsermessageHelpers.GetPrototype(message_name);
	if (!message) {
		BOOST_RAISE_EXCEPTION(PyExc_NameError, "Invalid message name: '%s'.", message_name);
	}

	m_buffer = message->New();
#else
	if (m_index == -1) {
		BOOST_RAISE_EXCEPTION(PyExc_NameError, "Invalid message name: '%s'.", message_name);
	}

	#if defined(ENGINE_LEFT4DEAD2) || defined(ENGINE_BLADE)
		m_buffer = engine->UserMessageBegin(&recipients, m_index, message_name);
	#else
		m_buffer = engine->UserMessageBegin(&recipients, m_index);
	#endif
#endif
}

CUserMessage::~CUserMessage()
{
#ifdef USE_PROTOBUF
	delete m_buffer;
#endif
}

void CUserMessage::Send()
{
#ifdef USE_PROTOBUF
	engine->SendUserMessage(m_recipients, GetMessageIndex(), *m_buffer);
#else
	engine->MessageEnd();
#endif
}

bool CUserMessage::IsProtobuf()
{
#ifdef USE_PROTOBUF
	return true;
#else
	return false;
#endif
}

//-----------------------------------------------------------------------------
// Functions.
//-----------------------------------------------------------------------------
void CreateMessage( edict_t *pEdict, DIALOG_TYPE type, KeyValues *data )
{
	helpers->CreateMessage(pEdict, type, data, &g_SourcePythonPlugin);
}

int GetMessageIndex(const char* name)
{
#ifdef USE_PROTOBUF
	return g_Cstrike15UsermessageHelpers.GetIndex(name);
#else
	char sz_mname[256];
	int sizereturn;
	int index = 0;
	while (servergamedll->GetUserMessageInfo(index, sz_mname, 255, sizereturn))
	{
		if (V_strcasecmp(name, sz_mname) == 0)
		{
			return index;
		}
		index++;
	}
	return -1;
#endif
}

object GetMessageName(int index)
{
#ifdef USE_PROTOBUF
	const char* name = g_Cstrike15UsermessageHelpers.GetName(index);
	if (!name)
		return object();

	return str(name);
#else
	char result[256] = "";
	int size = 0;
	if (!servergamedll->GetUserMessageInfo(index, result, 255, size))
		return object();

	return str(result);
#endif
}

object GetMessageSize(int index)
{
#ifdef USE_PROTOBUF
	BOOST_RAISE_EXCEPTION(PyExc_NotImplementedError, "")
	return object();
#else
	char result[256];
	int size;
	if (!servergamedll->GetUserMessageInfo(index, result, 255, size))
		return object();

	return object(size);
#endif
}
