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
#include "messages.h"
#include "public/engine/iserverplugin.h"
#include "core/sp_main.h"

//-----------------------------------------------------------------------------
// Externals.
//-----------------------------------------------------------------------------
extern IServerPluginHelpers *helpers;
extern CSourcePython g_SourcePythonPlugin;

//-----------------------------------------------------------------------------
// CUserMessage implementation.
//-----------------------------------------------------------------------------
CUserMessage::CUserMessage(const MRecipientFilter &recipient_filter, const char *message_name ) :
	CUserMessageImplementation(recipient_filter, message_name),
	m_sent(false)
{
}

CUserMessage::~CUserMessage()
{
	send_message();
}

void CUserMessage::send_message()
{
	if (m_sent == false)
	{
		send_message_internal();
		m_sent = true;
	}
}

bool CUserMessage::has_been_sent() const
{
	return m_sent;
}

const char * CUserMessage::get_message_name() const
{
	return m_message_name;
}

const int CUserMessage::get_message_index() const
{
	return m_message_index;
}

const MRecipientFilter & CUserMessage::get_recipient_filter() const
{
	return m_recipient_filter;
}

void CUserMessage::set_char( const char *field_name, char field_value, int index/*=-1*/ )
{
	CUserMessageImplementation::set_char(field_name, field_value, index);
}

void CUserMessage::set_byte( const char *field_name, unsigned char field_value, int index/*=-1*/ )
{
	CUserMessageImplementation::set_byte(field_name, field_value, index);
}

void CUserMessage::set_short( const char *field_name, signed short field_value, int index/*=-1*/ )
{
	CUserMessageImplementation::set_short(field_name, field_value, index);
}

void CUserMessage::set_long( const char *field_name, signed long field_value, int index/*=-1*/ )
{
	CUserMessageImplementation::set_long(field_name, field_value, index);
}

void CUserMessage::set_float( const char *field_name, float field_value, int index/*=-1*/ )
{
	CUserMessageImplementation::set_float(field_name, field_value, index);
}

void CUserMessage::set_bool( const char *field_name, bool field_value, int index/*=-1*/ )
{
	CUserMessageImplementation::set_bool(field_name, field_value, index);
}

void CUserMessage::set_buffer( const char *field_name, void *buffer, unsigned int num_bytes, int index/*=-1*/ )
{
	CUserMessageImplementation::set_buffer(field_name, buffer, num_bytes, index);
}

void CUserMessage::set_string( const char *field_name, const char *field_value, int index/*=-1*/ )
{
	CUserMessageImplementation::set_string(field_name, field_value, index);
}

//-----------------------------------------------------------------------------
// Functions.
//-----------------------------------------------------------------------------
void CreateMessage( edict_t *pEdict, DIALOG_TYPE type, KeyValues *data )
{
	helpers->CreateMessage(pEdict, type, data, &g_SourcePythonPlugin);
}
