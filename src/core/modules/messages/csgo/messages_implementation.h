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

#ifndef __USERMESSAGE_IMPLEMENTATION_H__
#define __USERMESSAGE_IMPLEMENTATION_H__

#include "../messages_implementation_base.h"

#include "google/protobuf/message.h"
#include "google/protobuf/descriptor.h"

class CUserMessageImplementation : public IUsermessageImplementationBase
{
public:
	CUserMessageImplementation( const MRecipientFilter &recipient_filter, const char *message_name);

	// Pure-virtual methods which must be inherited and overwritten in the inherited
	// classes
	virtual void set_char(const char *field_name, char field_value, int index=-1);
	virtual void set_byte(const char *field_name, unsigned char field_value, int index=-1);
	virtual void set_short(const char *field_name, signed short field_value, int index=-1);
	virtual void set_long(const char *field_name, signed long field_value, int index=-1);
	virtual void set_float(const char *field_name, float field_value, int index=-1);
	virtual void set_bool(const char *field_name, bool field_value, int index=-1);
	virtual void set_color(const char *field_name, Color field_value, int index=-1);
	
	// Unknown sized buffers
	virtual void set_buffer(const char *field_name, void *buffer, unsigned int num_bytes, int index=-1);
	virtual void set_string(const char *field_name, const char *field_value, int index=-1);

protected:
	
	/** Set the message index of the base class to the index of the message name */
	virtual void set_message_index();

	/** Send the message to the clients */
	virtual void send_message_internal();

private:
	template < typename _T_INPUT, typename _T_CAST >
	void set_typed_value(
		void (google::protobuf::Reflection::*setter_delegate)(google::protobuf::Message* message,const google::protobuf::FieldDescriptor* field, _T_CAST value) const, 
		const char *field_name, 
		_T_INPUT value)
	{
		if (m_message != NULL)
		{
			if (m_message->GetDescriptor() != NULL && m_message->GetReflection() != NULL)
			{
				const google::protobuf::FieldDescriptor *descriptor(m_message->GetDescriptor()->FindFieldByName(field_name));
				if (descriptor != NULL)
				{
					(*m_message->GetReflection().*setter_delegate)(m_message, descriptor, static_cast< _T_CAST >(value));
				}
				else
				{
					DevMsg(1, "SP-ERROR: Could not find field name '%s' for usermessage '%s'\n", field_name, m_message_name);
				}
			}
			else
			{
				DevMsg(1, "SP-ERROR: Could not find descriptor or reflection for usermessage '%s'\n", m_message_name);
			}
		}
		else
		{
			DevMsg(1, "SP-ERROR: Could not find message name '%s'\n", m_message_name);
		}
	}

	template < typename _T_INPUT, typename _T_CAST >
	void set_typed_value(
		void (google::protobuf::Reflection::*setter_delegate)(google::protobuf::Message* message,const google::protobuf::FieldDescriptor* field, int idx, _T_CAST value) const,
		void (google::protobuf::Reflection::*adder_delegate)(google::protobuf::Message* message,const google::protobuf::FieldDescriptor* field, _T_CAST value) const,
		const char *field_name,
		_T_INPUT value,
		_T_CAST default_value,
		int idx)
	{
		if (m_message != NULL)
		{
			if (m_message->GetDescriptor() != NULL && m_message->GetReflection() != NULL)
			{
				const google::protobuf::FieldDescriptor *descriptor(m_message->GetDescriptor()->FindFieldByName(field_name));
				if (descriptor != NULL)
				{
					
					for (int field_size(m_message->GetReflection()->FieldSize(*m_message, descriptor));
						 field_size <= idx; ++field_size)
					{
						// Resize the array
						(*m_message->GetReflection().*adder_delegate)(m_message, descriptor, default_value);
					}
					(*m_message->GetReflection().*setter_delegate)(m_message, descriptor, idx, static_cast< _T_CAST >(value));
				}
				else
				{
					DevMsg(1, "SP-ERROR: Could not find field name '%s' for usermessage '%s'\n", field_name, m_message_name);
				}
			}
			else
			{
				DevMsg(1, "SP-ERROR: Could not find descriptor or reflection for usermessage '%s'\n", m_message_name);
			}
		}
		else
		{
			DevMsg(1, "SP-ERROR: Could not find message name '%s'\n", m_message_name);
		}
	}

private:
	/** The protobuf message that this implementation represents. Possible to be NULL for invalid names. */
	google::protobuf::Message *m_message;
};

#endif // __USERMESSAGE_IMPLEMENTATION_H__
