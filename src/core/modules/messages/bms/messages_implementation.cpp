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

#include "messages_implementation.h"

CUserMessageImplementation::CUserMessageImplementation(const MRecipientFilter &recipient_filter, const char *message_name ) :
IUsermessageImplementationBase(recipient_filter, message_name)
{
	// Set the message index.
	set_message_index();
	
	// TODO - Check if we should pass this as a non-const so we don't have to const-cast it. I'm sure I saw a constructor
	// somewhere requiring this to be passed as a const-ref somewhere
	m_buffer = engine->UserMessageBegin(&const_cast<MRecipientFilter&>(recipient_filter), m_message_index);
}

void CUserMessageImplementation::send_message_internal()
{
	engine->MessageEnd();
}

void CUserMessageImplementation::set_bool( const char *field_name, bool field_value, int index/*=-1*/ )
{
	m_buffer->WriteByte(static_cast<bool>(field_value));
}

void CUserMessageImplementation::set_char( const char *field_name, char field_value, int index/*=-1*/ )
{
	m_buffer->WriteChar(static_cast<int>(field_value));
}

void CUserMessageImplementation::set_byte( const char *field_name, unsigned char field_value, int index/*=-1*/ )
{
	m_buffer->WriteByte(static_cast<unsigned int>(field_value));
}

void CUserMessageImplementation::set_short( const char *field_name, int field_value, int index/*=-1*/ )
{
	m_buffer->WriteShort(static_cast<int>(field_value));
}

void CUserMessageImplementation::set_long( const char *field_name, signed long field_value, int index/*=-1*/ )
{
	m_buffer->WriteLong(static_cast<long>(field_value));
}

void CUserMessageImplementation::set_float( const char *field_name, float field_value, int index/*=-1*/ )
{
	m_buffer->WriteFloat(field_value);
}

void CUserMessageImplementation::set_color( const char *field_name, Color field_value, int index/*=-1*/ )
{
	m_buffer->WriteByte(field_value[0]);
	m_buffer->WriteByte(field_value[1]);
	m_buffer->WriteByte(field_value[2]);
	m_buffer->WriteByte(field_value[3]);
}

void CUserMessageImplementation::set_buffer( const char *field_name, void *buffer, unsigned int num_bytes, int index/*=-1*/ )
{
	m_buffer->WriteBytes(buffer, static_cast<int>(num_bytes));
}

void CUserMessageImplementation::set_string( const char *field_name, const char *field_value, int index/*=-1*/ )
{
	m_buffer->WriteString(field_value);
}

void CUserMessageImplementation::set_message_index()
{
	char sz_mname[256];
	int sizereturn;
	int index = 0;
	while (servergamedll->GetUserMessageInfo(index, sz_mname, 255, sizereturn))
	{
		if (strcmp(m_message_name, sz_mname) == 0)
		{
			m_message_index = index;
			break;
		}
		index++;
	}
}
