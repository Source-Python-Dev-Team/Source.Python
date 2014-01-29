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

#include "usermessage_implementation.h"

// Game specific protobuf includes
#include "public/game/shared/csgo/protobuf/cstrike15_usermessages.pb.h"
#include "public/game/shared/csgo/protobuf/cstrike15_usermessage_helpers.h"

CUserMessageImplementation::CUserMessageImplementation( const CMRecipientFilter &recipient_filter, const char *message_name ) :
	IUsermessageImplementationBase(recipient_filter, message_name),
	m_message(NULL)
{
	// Set the message index.
	set_message_index();
	
	// Eugh - I need to go take a shower after const casting...
	m_message = const_cast<google::protobuf::Message*>(g_Cstrike15UsermessageHelpers.GetPrototype(message_name));
}

void CUserMessageImplementation::set_message_index()
{
	m_message_index = g_Cstrike15UsermessageHelpers.GetIndex(m_message_name);
}

void CUserMessageImplementation::send_message_internal()
{
	if (m_message != NULL  && m_message_index != -1)
	{
		engine->SendUserMessage(const_cast<CMRecipientFilter&>(m_recipient_filter), m_message_index, *m_message);
	}
	else
	{
		DevMsg(1, "SP-ERROR: Could not send message '%s', invalid message name\n", m_message_name);
	}
}


void CUserMessageImplementation::set_bool( const char *field_name, bool field_value, int index/*=-1*/ )
{
	if (index == -1)
	{
		set_typed_value<bool, bool>(&google::protobuf::Reflection::SetBool, field_name, field_value);
	}
	else
	{
		set_typed_value<bool, bool>(
			&google::protobuf::Reflection::SetRepeatedBool, 
			&google::protobuf::Reflection::AddBool, 
			field_name, field_value, false, index);
	}
}

void CUserMessageImplementation::set_char( const char *field_name, char field_value, int index/*=-1*/ )
{
	if (index == -1)
	{
		set_typed_value<char, google::protobuf::int32>(&google::protobuf::Reflection::SetInt32, field_name, field_value);
	}
	else
	{
		set_typed_value<char, google::protobuf::int32>(
			&google::protobuf::Reflection::SetRepeatedInt32, 
			&google::protobuf::Reflection::AddInt32, 
			field_name, field_value, 0, index);
	}
}

void CUserMessageImplementation::set_byte( const char *field_name, unsigned char field_value, int index/*=-1*/ )
{
	if (index == -1)
	{
		set_typed_value<unsigned char, google::protobuf::int32>(&google::protobuf::Reflection::SetInt32, field_name, field_value);
	}
	else
	{
		set_typed_value<unsigned char, google::protobuf::int32>(
			&google::protobuf::Reflection::SetRepeatedInt32, 
			&google::protobuf::Reflection::AddInt32, 
			field_name, field_value, 0, index);
	}
}

void CUserMessageImplementation::set_short( const char *field_name, signed short field_value, int index/*=-1*/ )
{
	if (index == -1)
	{
		set_typed_value<signed short, google::protobuf::int32>(&google::protobuf::Reflection::SetInt32, field_name, field_value);
	}
	else
	{
		set_typed_value<signed short, google::protobuf::int32>(
			&google::protobuf::Reflection::SetRepeatedInt32, 
			&google::protobuf::Reflection::AddInt32, 
			field_name, field_value, 0, index);
	}
}

void CUserMessageImplementation::set_long( const char *field_name, signed long field_value, int index/*=-1*/ )
{
	if (index == -1)
	{
		set_typed_value<signed long, google::protobuf::int32>(&google::protobuf::Reflection::SetInt32, field_name, field_value);
	}
	else
	{
		set_typed_value<signed long, google::protobuf::int32>(
			&google::protobuf::Reflection::SetRepeatedInt32, 
			&google::protobuf::Reflection::AddInt32, 
			field_name, field_value, 0, index);
	}
}

void CUserMessageImplementation::set_float( const char *field_name, float field_value, int index/*=-1*/ )
{
	if (index == -1)
	{
		set_typed_value<float, float>(&google::protobuf::Reflection::SetFloat, field_name, field_value);
	}
	else
	{
		set_typed_value<float, float>(
			&google::protobuf::Reflection::SetRepeatedFloat, 
			&google::protobuf::Reflection::AddFloat, 
			field_name, field_value, 0.0f, index);
	}
}

void CUserMessageImplementation::set_buffer( const char *field_name, void *buffer, unsigned int num_bytes, int index/*=-1*/ )
{
	
}

void CUserMessageImplementation::set_string( const char *field_name, const char *field_value, int index/*=-1*/ )
{
	if (index == -1)
	{
		set_typed_value<const char *, const std::string &>(&google::protobuf::Reflection::SetString, field_name, field_value);
	}
	else
	{
		set_typed_value<const char *, const std::string &>(
			&google::protobuf::Reflection::SetRepeatedString, 
			&google::protobuf::Reflection::AddString, 
			field_name, field_value, "", index);
	}
}
