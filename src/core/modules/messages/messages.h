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
	#define MESSAGE_BUFFER CProtobufMessage
#else
	#define MESSAGE_BUFFER bf_write
#endif


//-----------------------------------------------------------------------------
// Classes.
//-----------------------------------------------------------------------------
#ifdef USE_PROTOBUF
	class CProtobufMessage
	{
	public:
		CProtobufMessage(google::protobuf::Message* message);

	public:
		google::protobuf::Message* GetProtobufMessage()
		{ return m_message; }

	public:
		const google::protobuf::FieldDescriptor* GetFieldDescriptor(const char* field_name)
		{
			const google::protobuf::FieldDescriptor* descriptor = m_message->GetDescriptor()->FindFieldByName(field_name);
			if (!descriptor) {
				BOOST_RAISE_EXCEPTION(PyExc_NameError, "Unable to find field '%s'.", field_name);
			}
			return descriptor;
		}
	
		const google::protobuf::EnumValueDescriptor* GetEnumValueDescriptor(const char* field_name, int value)
		{
			const google::protobuf::EnumValueDescriptor* enum_value = GetFieldDescriptor(field_name)->enum_type()->FindValueByNumber(value);
			if (!enum_value) {
				BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Unable to find enum value '%i'.", value);
			}
			return enum_value;
		}
		

		// ====================================================================
		// >> GetField
		// ====================================================================
		template<class T>
		T GetField(
			T (google::protobuf::Reflection::*get_field_delegate)(const google::protobuf::Message& message, const google::protobuf::FieldDescriptor* field) const,
			const char* field_name)
		{ return (*m_message->GetReflection().*get_field_delegate)(*m_message, GetFieldDescriptor(field_name)); }

		int32 GetInt32(const char* field_name)
		{ return GetField<int32>(&google::protobuf::Reflection::GetInt32, field_name); }

		int64 GetInt64(const char* field_name)
		{ return GetField<int64>(&google::protobuf::Reflection::GetInt64, field_name); }

		uint32 GetUInt32(const char* field_name)
		{ return GetField<uint32>(&google::protobuf::Reflection::GetUInt32, field_name); }

		uint64 GetUInt64(const char* field_name)
		{ return GetField<uint64>(&google::protobuf::Reflection::GetUInt64, field_name); }

		float GetFloat(const char* field_name)
		{ return GetField<float>(&google::protobuf::Reflection::GetFloat, field_name); }

		double GetDouble(const char* field_name)
		{ return GetField<double>(&google::protobuf::Reflection::GetDouble, field_name); }

		bool GetBool(const char* field_name)
		{ return GetField<bool>(&google::protobuf::Reflection::GetBool, field_name); }

		std::string GetString(const char* field_name)
		{ return GetField<std::string>(&google::protobuf::Reflection::GetString, field_name); }

		int GetEnum(const char* field_name)
		{ return GetField<const google::protobuf::EnumValueDescriptor*>(&google::protobuf::Reflection::GetEnum, field_name)->number(); }

		CProtobufMessage GetMessage(const char* field_name)
		{ return CProtobufMessage(&const_cast<google::protobuf::Message&>(m_message->GetReflection()->GetMessage(*m_message, GetFieldDescriptor(field_name)))); }
		
		
		// ====================================================================
		// >> GetRepeatedField
		// ====================================================================
		template<class T>
		T GetRepeatedField(
			T (google::protobuf::Reflection::*get_repeated_field_delegate)(const google::protobuf::Message& message, const google::protobuf::FieldDescriptor* field, int index) const,
			const char* field_name,
			int index)
		{ return (*m_message->GetReflection().*get_repeated_field_delegate)(*m_message, GetFieldDescriptor(field_name), index); }

		int32 GetRepeatedInt32(const char* field_name, int index)
		{ return GetRepeatedField<int32>(&google::protobuf::Reflection::GetRepeatedInt32, field_name, index); }

		int64 GetRepeatedInt64(const char* field_name, int index)
		{ return GetRepeatedField<int64>(&google::protobuf::Reflection::GetRepeatedInt64, field_name, index); }

		uint32 GetRepeatedUInt32(const char* field_name, int index)
		{ return GetRepeatedField<uint32>(&google::protobuf::Reflection::GetRepeatedUInt32, field_name, index); }

		uint64 GetRepeatedUInt64(const char* field_name, int index)
		{ return GetRepeatedField<uint64>(&google::protobuf::Reflection::GetRepeatedUInt64, field_name, index); }

		float GetRepeatedFloat(const char* field_name, int index)
		{ return GetRepeatedField<float>(&google::protobuf::Reflection::GetRepeatedFloat, field_name, index); }

		double GetRepeatedDouble(const char* field_name, int index)
		{ return GetRepeatedField<double>(&google::protobuf::Reflection::GetRepeatedDouble, field_name, index); }

		bool GetRepeatedBool(const char* field_name, int index)
		{ return GetRepeatedField<bool>(&google::protobuf::Reflection::GetRepeatedBool, field_name, index); }

		std::string GetRepeatedString(const char* field_name, int index)
		{ return GetRepeatedField<std::string>(&google::protobuf::Reflection::GetRepeatedString, field_name, index); }

		int GetRepeatedEnum(const char* field_name, int index)
		{ return GetRepeatedField<const google::protobuf::EnumValueDescriptor*>(&google::protobuf::Reflection::GetRepeatedEnum, field_name, index)->number(); }

		CProtobufMessage GetRepeatedMessage(const char* field_name, int index)
		{ return CProtobufMessage(&const_cast<google::protobuf::Message&>(m_message->GetReflection()->GetRepeatedMessage(*m_message, GetFieldDescriptor(field_name), index))); }
		
		
		// ====================================================================
		// >> SetField
		// ====================================================================
		template<class T>
		void SetField(
			void (google::protobuf::Reflection::*set_field_delegate)(google::protobuf::Message* message, const google::protobuf::FieldDescriptor* field, T value) const,
			const char* field_name,
			T value)
		{ (*m_message->GetReflection().*set_field_delegate)(m_message, GetFieldDescriptor(field_name), value); }

		void SetInt32(const char* field_name, int64 value)
		{ SetField<int32>(&google::protobuf::Reflection::SetInt32, field_name, value); }

		void SetInt64(const char* field_name, int64 value)
		{ SetField<int64>(&google::protobuf::Reflection::SetInt64, field_name, value); }

		void SetUInt32(const char* field_name, uint32 value)
		{ SetField<uint32>(&google::protobuf::Reflection::SetUInt32, field_name, value); }

		void SetUInt64(const char* field_name, uint64 value)
		{ SetField<uint64>(&google::protobuf::Reflection::SetUInt64, field_name, value); }

		void SetFloat(const char* field_name, float value)
		{ SetField<float>(&google::protobuf::Reflection::SetFloat, field_name, value); }

		void SetDouble(const char* field_name, double value)
		{ SetField<double>(&google::protobuf::Reflection::SetDouble, field_name, value); }

		void SetBool(const char* field_name, bool value)
		{ SetField<bool>(&google::protobuf::Reflection::SetBool, field_name, value); }

		void SetEnum(const char* field_name, int value)
		{ SetField<const google::protobuf::EnumValueDescriptor*>(&google::protobuf::Reflection::SetEnum, field_name, GetEnumValueDescriptor(field_name, value)); }

		void SetString(const char* field_name, const char* value)
		{ SetField<const std::string&>(&google::protobuf::Reflection::SetString, field_name, value); }

		CProtobufMessage MutableMessage(const char* field_name)
		{ return CProtobufMessage(m_message->GetReflection()->MutableMessage(m_message, GetFieldDescriptor(field_name))); }
		
		
		// ====================================================================
		// >> SetRepeatedField
		// ====================================================================
		template<class T>
		void SetRepeatedField(
			void (google::protobuf::Reflection::*set_repeated_field_delegate)(google::protobuf::Message* message, const google::protobuf::FieldDescriptor* field, int index, T value) const,
			const char* field_name,
			int index,
			T value)
		{ (*m_message->GetReflection().*set_repeated_field_delegate)(m_message, GetFieldDescriptor(field_name), index, value); }

		void SetRepeatedInt32(const char* field_name, int index, int32 value)
		{ SetRepeatedField<int32>(&google::protobuf::Reflection::SetRepeatedInt32, field_name, index, value); }

		void SetRepeatedInt64(const char* field_name, int index, int64 value)
		{ SetRepeatedField<int64>(&google::protobuf::Reflection::SetRepeatedInt64, field_name, index, value); }

		void SetRepeatedUInt32(const char* field_name, int index, uint32 value)
		{ SetRepeatedField<uint32>(&google::protobuf::Reflection::SetRepeatedUInt32, field_name, index, value); }

		void SetRepeatedUInt64(const char* field_name, int index, uint64 value)
		{ SetRepeatedField<uint64>(&google::protobuf::Reflection::SetRepeatedUInt64, field_name, index, value); }

		void SetRepeatedFloat(const char* field_name, int index, float value)
		{ SetRepeatedField<float>(&google::protobuf::Reflection::SetRepeatedFloat, field_name, index, value); }

		void SetRepeatedDouble(const char* field_name, int index, double value)
		{ SetRepeatedField<double>(&google::protobuf::Reflection::SetRepeatedDouble, field_name, index, value); }

		void SetRepeatedBool(const char* field_name, int index, bool value)
		{ SetRepeatedField<bool>(&google::protobuf::Reflection::SetRepeatedBool, field_name, index, value); }

		void SetRepeatedString(const char* field_name, int index, const char* value)
		{ SetRepeatedField<const std::string&>(&google::protobuf::Reflection::SetRepeatedString, field_name, index, value); }

		void SetRepeatedEnum(const char* field_name, int index, int value)
		{  SetRepeatedField<const google::protobuf::EnumValueDescriptor*>(&google::protobuf::Reflection::SetRepeatedEnum, field_name, index, GetEnumValueDescriptor(field_name, value)); }

		CProtobufMessage MutableRepeatedMessage(const char* field_name, int index)
		{ return CProtobufMessage(m_message->GetReflection()->MutableRepeatedMessage(m_message, GetFieldDescriptor(field_name), index)); }

		
		// ====================================================================
		// >> AddField
		// ====================================================================
		template<class T>
		void AddField(
			void (google::protobuf::Reflection::*add_field_delegate)(google::protobuf::Message* message, const google::protobuf::FieldDescriptor* field, T value) const,
			const char* field_name,
			T value)
		{ (*m_message->GetReflection().*add_field_delegate)(m_message, GetFieldDescriptor(field_name), value); }

		void AddInt32(const char* field_name, int32 value)
		{ AddField<int32>(&google::protobuf::Reflection::AddInt32, field_name, value); }

		void AddInt64(const char* field_name, int64 value)
		{ AddField<int64>(&google::protobuf::Reflection::AddInt64, field_name, value); }

		void AddUInt32(const char* field_name, uint32 value)
		{ AddField<uint32>(&google::protobuf::Reflection::AddUInt32, field_name, value); }

		void AddUInt64(const char* field_name, uint64 value)
		{ AddField<uint64>(&google::protobuf::Reflection::AddUInt64, field_name, value); }

		void AddFloat(const char* field_name, float value)
		{ AddField<float>(&google::protobuf::Reflection::AddFloat, field_name, value); }

		void AddDouble(const char* field_name, double value)
		{ AddField<double>(&google::protobuf::Reflection::AddDouble, field_name, value); }

		void AddBool(const char* field_name, bool value)
		{ AddField<bool>(&google::protobuf::Reflection::AddBool, field_name, value); }

		void AddString(const char* field_name, const char* value)
		{ AddField<const std::string&>(&google::protobuf::Reflection::AddString, field_name, value); }

		void AddEnum(const char* field_name, int value)
		{ AddField<const google::protobuf::EnumValueDescriptor*>(&google::protobuf::Reflection::AddEnum, field_name, GetEnumValueDescriptor(field_name, value)); }

		CProtobufMessage AddMessage(const char* field_name)
		{ return CProtobufMessage(m_message->GetReflection()->AddMessage(m_message, GetFieldDescriptor(field_name))); }

	private:
		google::protobuf::Message* m_message;
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
	int GetMessageIndex();
	static bool IsProtobuf();

private:
	IRecipientFilter& m_recipients;
	const char* m_message_name;
	MESSAGE_BUFFER* m_buffer;
};


//-----------------------------------------------------------------------------
// Functions.
//-----------------------------------------------------------------------------
void CreateMessage( edict_t *pEdict, DIALOG_TYPE type, KeyValues *data );

#endif // _MESSAGES_H
