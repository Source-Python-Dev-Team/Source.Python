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
#include "bitbuf.h"


#ifdef USE_PROTOBUF
	#include "game/shared/csgo/protobuf/cstrike15_usermessages.pb.h"
	#include "public/game/shared/csgo/protobuf/cstrike15_usermessage_helpers.h"
	#define MESSAGE_BUFFER google::protobuf::Message
#else
	#define MESSAGE_BUFFER bf_write
#endif


//-----------------------------------------------------------------------------
// Classes.
//-----------------------------------------------------------------------------
#ifdef USE_PROTOBUF
	class CProtobufMessageExt
	{
	public:
		static boost::shared_ptr<google::protobuf::Message> __init__(const char* szMessage)
		{
			const google::protobuf::DescriptorPool* pool = google::protobuf::DescriptorPool::generated_pool();
			if (!pool) {
				BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Pool is NULL");
			}

			const google::protobuf::Descriptor* descriptor = pool->FindMessageTypeByName(szMessage);
			if (!descriptor) {
				BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to find '%s'.", szMessage);
			}

			google::protobuf::MessageFactory* factory = google::protobuf::MessageFactory::generated_factory();
			if (!factory) {
				BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Factory is NULL");
			}

			const google::protobuf::Message* message = factory->GetPrototype(descriptor);
			if (!message) {
				BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Failed to get prototype of '%s'.", szMessage);
			}

			return boost::shared_ptr<google::protobuf::Message>(message->New());
		}

		static const google::protobuf::FieldDescriptor* GetFieldDescriptor(google::protobuf::Message* pMessage, const char* field_name)
		{
			const google::protobuf::Descriptor* descriptor = pMessage->GetDescriptor();

			// For some reasons, FindFieldByName is causing a crash if the message has been initialized
			//	by the server so let's look it up ourself...
			for (int iCurrentIndex=0; iCurrentIndex < descriptor->field_count(); iCurrentIndex++)
			{
				const google::protobuf::FieldDescriptor *field_descriptor = descriptor->field(iCurrentIndex);
				if (field_descriptor && strcmp(field_descriptor->name().c_str(), field_name) == 0)
					return field_descriptor;
			}

			BOOST_RAISE_EXCEPTION(PyExc_NameError, "Unable to find field '%s'.", field_name);
			return NULL;
		}
	
		static const google::protobuf::EnumValueDescriptor* GetEnumValueDescriptor(google::protobuf::Message* pMessage, const char* field_name, int value)
		{
			const google::protobuf::EnumValueDescriptor* enum_value = GetFieldDescriptor(pMessage, field_name)->enum_type()->FindValueByNumber(value);
			if (!enum_value) {
				BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to find enum value '%i'.", value);
			}
			return enum_value;
		}

		static int GetFieldCount(google::protobuf::Message* pMessage, const char* field_name)
		{
			return pMessage->GetReflection()->FieldSize(*pMessage, GetFieldDescriptor(pMessage, field_name));
		}
		

		// ====================================================================
		// >> GetField
		// ====================================================================
		template<class T>
		static T GetField(
			google::protobuf::Message* pMessage,
			T (google::protobuf::Reflection::*get_field_delegate)(const google::protobuf::Message& message, const google::protobuf::FieldDescriptor* field) const,
			const char* field_name)
		{ return (*pMessage->GetReflection().*get_field_delegate)(*pMessage, GetFieldDescriptor(pMessage, field_name)); }

		static int32 GetInt32(google::protobuf::Message* pMessage, const char* field_name)
		{ return GetField<int32>(pMessage, &google::protobuf::Reflection::GetInt32, field_name); }

		static int64 GetInt64(google::protobuf::Message* pMessage, const char* field_name)
		{ return GetField<int64>(pMessage, &google::protobuf::Reflection::GetInt64, field_name); }

		static uint32 GetUInt32(google::protobuf::Message* pMessage, const char* field_name)
		{ return GetField<uint32>(pMessage, &google::protobuf::Reflection::GetUInt32, field_name); }

		static uint64 GetUInt64(google::protobuf::Message* pMessage, const char* field_name)
		{ return GetField<uint64>(pMessage, &google::protobuf::Reflection::GetUInt64, field_name); }

		static float GetFloat(google::protobuf::Message* pMessage, const char* field_name)
		{ return GetField<float>(pMessage, &google::protobuf::Reflection::GetFloat, field_name); }

		static double GetDouble(google::protobuf::Message* pMessage, const char* field_name)
		{ return GetField<double>(pMessage, &google::protobuf::Reflection::GetDouble, field_name); }

		static bool GetBool(google::protobuf::Message* pMessage, const char* field_name)
		{ return GetField<bool>(pMessage, &google::protobuf::Reflection::GetBool, field_name); }

		static std::string GetString(google::protobuf::Message* pMessage, const char* field_name)
		{
			std::string return_value;
			return_value = pMessage->GetReflection()->GetStringReference(*pMessage, GetFieldDescriptor(pMessage, field_name), &return_value);
			return return_value;
		}

		static int GetEnum(google::protobuf::Message* pMessage, const char* field_name)
		{ return GetField<const google::protobuf::EnumValueDescriptor*>(pMessage, &google::protobuf::Reflection::GetEnum, field_name)->number(); }

		static const google::protobuf::Message& GetMessage(google::protobuf::Message* pMessage, const char* field_name)
		{ return pMessage->GetReflection()->GetMessage(*pMessage, GetFieldDescriptor(pMessage, field_name)); }
		
		
		// ====================================================================
		// >> GetRepeatedField
		// ====================================================================
		template<class T>
		static T GetRepeatedField(
			google::protobuf::Message* pMessage,
			T (google::protobuf::Reflection::*get_repeated_field_delegate)(const google::protobuf::Message& message, const google::protobuf::FieldDescriptor* field, int index) const,
			const char* field_name,
			int index)
		{
			
			const google::protobuf::FieldDescriptor* descriptor = GetFieldDescriptor(pMessage, field_name);
			if (index >= pMessage->GetReflection()->FieldSize(*pMessage, descriptor))
			{
				BOOST_RAISE_EXCEPTION(PyExc_IndexError, "Index (%d) out of range.", index)
			}

			return (*pMessage->GetReflection().*get_repeated_field_delegate)(*pMessage, GetFieldDescriptor(pMessage, field_name), index);
		}

		static int32 GetRepeatedInt32(google::protobuf::Message* pMessage, const char* field_name, int index)
		{ return GetRepeatedField<int32>(pMessage, &google::protobuf::Reflection::GetRepeatedInt32, field_name, index); }

		static int64 GetRepeatedInt64(google::protobuf::Message* pMessage, const char* field_name, int index)
		{ return GetRepeatedField<int64>(pMessage, &google::protobuf::Reflection::GetRepeatedInt64, field_name, index); }

		static uint32 GetRepeatedUInt32(google::protobuf::Message* pMessage, const char* field_name, int index)
		{ return GetRepeatedField<uint32>(pMessage, &google::protobuf::Reflection::GetRepeatedUInt32, field_name, index); }

		static uint64 GetRepeatedUInt64(google::protobuf::Message* pMessage, const char* field_name, int index)
		{ return GetRepeatedField<uint64>(pMessage, &google::protobuf::Reflection::GetRepeatedUInt64, field_name, index); }

		static float GetRepeatedFloat(google::protobuf::Message* pMessage, const char* field_name, int index)
		{ return GetRepeatedField<float>(pMessage, &google::protobuf::Reflection::GetRepeatedFloat, field_name, index); }

		static double GetRepeatedDouble(google::protobuf::Message* pMessage, const char* field_name, int index)
		{ return GetRepeatedField<double>(pMessage, &google::protobuf::Reflection::GetRepeatedDouble, field_name, index); }

		static bool GetRepeatedBool(google::protobuf::Message* pMessage, const char* field_name, int index)
		{ return GetRepeatedField<bool>(pMessage, &google::protobuf::Reflection::GetRepeatedBool, field_name, index); }

		static std::string GetRepeatedString(google::protobuf::Message* pMessage, const char* field_name, int index)
		{
			std::string return_value;
			return_value = pMessage->GetReflection()->GetRepeatedStringReference(*pMessage, GetFieldDescriptor(pMessage, field_name), index, &return_value);
			return return_value;
		}

		static int GetRepeatedEnum(google::protobuf::Message* pMessage, const char* field_name, int index)
		{ return GetRepeatedField<const google::protobuf::EnumValueDescriptor*>(pMessage, &google::protobuf::Reflection::GetRepeatedEnum, field_name, index)->number(); }

		static const google::protobuf::Message& GetRepeatedMessage(google::protobuf::Message* pMessage, const char* field_name, int index)
		{ return pMessage->GetReflection()->GetRepeatedMessage(*pMessage, GetFieldDescriptor(pMessage, field_name), index); }
		
		
		// ====================================================================
		// >> SetField
		// ====================================================================
		template<class T>
		static void SetField(
			google::protobuf::Message* pMessage,
			void (google::protobuf::Reflection::*set_field_delegate)(google::protobuf::Message* message, const google::protobuf::FieldDescriptor* field, T value) const,
			const char* field_name,
			T value)
		{ (*pMessage->GetReflection().*set_field_delegate)(pMessage, GetFieldDescriptor(pMessage, field_name), value); }

		static void SetInt32(google::protobuf::Message* pMessage, const char* field_name, int64 value)
		{ SetField<int32>(pMessage, &google::protobuf::Reflection::SetInt32, field_name, value); }

		static void SetInt64(google::protobuf::Message* pMessage, const char* field_name, int64 value)
		{ SetField<int64>(pMessage, &google::protobuf::Reflection::SetInt64, field_name, value); }

		static void SetUInt32(google::protobuf::Message* pMessage, const char* field_name, uint32 value)
		{ SetField<uint32>(pMessage, &google::protobuf::Reflection::SetUInt32, field_name, value); }

		static void SetUInt64(google::protobuf::Message* pMessage, const char* field_name, uint64 value)
		{ SetField<uint64>(pMessage, &google::protobuf::Reflection::SetUInt64, field_name, value); }

		static void SetFloat(google::protobuf::Message* pMessage, const char* field_name, float value)
		{ SetField<float>(pMessage, &google::protobuf::Reflection::SetFloat, field_name, value); }

		static void SetDouble(google::protobuf::Message* pMessage, const char* field_name, double value)
		{ SetField<double>(pMessage, &google::protobuf::Reflection::SetDouble, field_name, value); }

		static void SetBool(google::protobuf::Message* pMessage, const char* field_name, bool value)
		{ SetField<bool>(pMessage, &google::protobuf::Reflection::SetBool, field_name, value); }

		static void SetEnum(google::protobuf::Message* pMessage, const char* field_name, int value)
		{ SetField<const google::protobuf::EnumValueDescriptor*>(pMessage, &google::protobuf::Reflection::SetEnum, field_name, GetEnumValueDescriptor(pMessage, field_name, value)); }

		static void SetString(google::protobuf::Message* pMessage, const char* field_name, const char* value)
		{ SetField<const std::string&>(pMessage, &google::protobuf::Reflection::SetString, field_name, value); }

		static google::protobuf::Message* MutableMessage(google::protobuf::Message* pMessage, const char* field_name)
		{ return pMessage->GetReflection()->MutableMessage(pMessage, GetFieldDescriptor(pMessage, field_name)); }
		
		
		// ====================================================================
		// >> SetRepeatedField
		// ====================================================================
		template<class T>
		static void SetRepeatedField(
			google::protobuf::Message* pMessage,
			void (google::protobuf::Reflection::*set_repeated_field_delegate)(google::protobuf::Message* message, const google::protobuf::FieldDescriptor* field, int index, T value) const,
			const char* field_name,
			int index,
			T value)
		{
			const google::protobuf::FieldDescriptor* descriptor = GetFieldDescriptor(pMessage, field_name);
			if (index >= pMessage->GetReflection()->FieldSize(*pMessage, descriptor))
			{
				BOOST_RAISE_EXCEPTION(PyExc_IndexError, "Index (%d) out of range.", index)
			}

			(*pMessage->GetReflection().*set_repeated_field_delegate)(pMessage, descriptor, index, value); 
		}

		static void SetRepeatedInt32(google::protobuf::Message* pMessage, const char* field_name, int index, int32 value)
		{ SetRepeatedField<int32>(pMessage, &google::protobuf::Reflection::SetRepeatedInt32, field_name, index, value); }

		static void SetRepeatedInt64(google::protobuf::Message* pMessage, const char* field_name, int index, int64 value)
		{ SetRepeatedField<int64>(pMessage, &google::protobuf::Reflection::SetRepeatedInt64, field_name, index, value); }

		static void SetRepeatedUInt32(google::protobuf::Message* pMessage, const char* field_name, int index, uint32 value)
		{ SetRepeatedField<uint32>(pMessage, &google::protobuf::Reflection::SetRepeatedUInt32, field_name, index, value); }

		static void SetRepeatedUInt64(google::protobuf::Message* pMessage, const char* field_name, int index, uint64 value)
		{ SetRepeatedField<uint64>(pMessage, &google::protobuf::Reflection::SetRepeatedUInt64, field_name, index, value); }

		static void SetRepeatedFloat(google::protobuf::Message* pMessage, const char* field_name, int index, float value)
		{ SetRepeatedField<float>(pMessage, &google::protobuf::Reflection::SetRepeatedFloat, field_name, index, value); }

		static void SetRepeatedDouble(google::protobuf::Message* pMessage, const char* field_name, int index, double value)
		{ SetRepeatedField<double>(pMessage, &google::protobuf::Reflection::SetRepeatedDouble, field_name, index, value); }

		static void SetRepeatedBool(google::protobuf::Message* pMessage, const char* field_name, int index, bool value)
		{ SetRepeatedField<bool>(pMessage, &google::protobuf::Reflection::SetRepeatedBool, field_name, index, value); }

		static void SetRepeatedString(google::protobuf::Message* pMessage, const char* field_name, int index, const char* value)
		{ SetRepeatedField<const std::string&>(pMessage, &google::protobuf::Reflection::SetRepeatedString, field_name, index, value); }

		static void SetRepeatedEnum(google::protobuf::Message* pMessage, const char* field_name, int index, int value)
		{  SetRepeatedField<const google::protobuf::EnumValueDescriptor*>(pMessage, &google::protobuf::Reflection::SetRepeatedEnum, field_name, index, GetEnumValueDescriptor(pMessage, field_name, value)); }

		static google::protobuf::Message* MutableRepeatedMessage(google::protobuf::Message* pMessage, const char* field_name, int index)
		{ return pMessage->GetReflection()->MutableRepeatedMessage(pMessage, GetFieldDescriptor(pMessage, field_name), index); }

		
		// ====================================================================
		// >> AddField
		// ====================================================================
		template<class T>
		static void AddField(
			google::protobuf::Message* pMessage,
			void (google::protobuf::Reflection::*add_field_delegate)(google::protobuf::Message* message, const google::protobuf::FieldDescriptor* field, T value) const,
			const char* field_name,
			T value)
		{ (*pMessage->GetReflection().*add_field_delegate)(pMessage, GetFieldDescriptor(pMessage, field_name), value); }

		static void AddInt32(google::protobuf::Message* pMessage, const char* field_name, int32 value)
		{ AddField<int32>(pMessage, &google::protobuf::Reflection::AddInt32, field_name, value); }

		static void AddInt64(google::protobuf::Message* pMessage, const char* field_name, int64 value)
		{ AddField<int64>(pMessage, &google::protobuf::Reflection::AddInt64, field_name, value); }

		static void AddUInt32(google::protobuf::Message* pMessage, const char* field_name, uint32 value)
		{ AddField<uint32>(pMessage, &google::protobuf::Reflection::AddUInt32, field_name, value); }

		static void AddUInt64(google::protobuf::Message* pMessage, const char* field_name, uint64 value)
		{ AddField<uint64>(pMessage, &google::protobuf::Reflection::AddUInt64, field_name, value); }

		static void AddFloat(google::protobuf::Message* pMessage, const char* field_name, float value)
		{ AddField<float>(pMessage, &google::protobuf::Reflection::AddFloat, field_name, value); }

		static void AddDouble(google::protobuf::Message* pMessage, const char* field_name, double value)
		{ AddField<double>(pMessage, &google::protobuf::Reflection::AddDouble, field_name, value); }

		static void AddBool(google::protobuf::Message* pMessage, const char* field_name, bool value)
		{ AddField<bool>(pMessage, &google::protobuf::Reflection::AddBool, field_name, value); }

		static void AddString(google::protobuf::Message* pMessage, const char* field_name, const char* value)
		{ AddField<const std::string&>(pMessage, &google::protobuf::Reflection::AddString, field_name, value); }

		static void AddEnum(google::protobuf::Message* pMessage, const char* field_name, int value)
		{ AddField<const google::protobuf::EnumValueDescriptor*>(pMessage, &google::protobuf::Reflection::AddEnum, field_name, GetEnumValueDescriptor(pMessage, field_name, value)); }

		static google::protobuf::Message* AddMessage(google::protobuf::Message* pMessage, const char* field_name)
		{ return pMessage->GetReflection()->AddMessage(pMessage, GetFieldDescriptor(pMessage, field_name)); }
	};
#endif


class CUserMessage
{
public:
	CUserMessage(IRecipientFilter& recipients, const char* message_name);
	~CUserMessage();

public:
	// Attribute accessors
	IRecipientFilter& GetRecipients()
	{ return m_recipients; }

	const char* GetMessageName()
	{ return m_message_name; }

	MESSAGE_BUFFER* GetBuffer()
	{ return m_buffer; }

public:
	void Send();
	int GetMessageIndex()
	{ return m_index; }

	static bool IsProtobuf();

private:
	IRecipientFilter& m_recipients;
	const char* m_message_name;
	MESSAGE_BUFFER* m_buffer;
	int m_index;
};


//-----------------------------------------------------------------------------
// Functions.
//-----------------------------------------------------------------------------
void	CreateMessage( edict_t *pEdict, DIALOG_TYPE type, KeyValues *data );
int		GetMessageIndex(const char* name);
object	GetMessageName(int index);
object	GetMessageSize(int index);

#endif // _MESSAGES_H
