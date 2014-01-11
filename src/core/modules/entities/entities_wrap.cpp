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
// Includes
//-----------------------------------------------------------------------------
#include <vector>
#include "entities_props.h"
#include "entities_wrap.h"
#include "dt_common.h"
#include "utility/sp_util.h"
#include "edict.h"
#include "boost/unordered_map.hpp"
#include "boost/algorithm/string.hpp"
#include "boost/foreach.hpp"
#include "utility/wrap_macros.h"

//-----------------------------------------------------------------------------
// If these aren't defined, we get linker errors about CBaseEdict.
//-----------------------------------------------------------------------------
IChangeInfoAccessor *CBaseEdict::GetChangeAccessor()
{
	return engine->GetChangeAccessor( (const edict_t *)this );
}

const IChangeInfoAccessor *CBaseEdict::GetChangeAccessor() const
{
	return engine->GetChangeAccessor( (const edict_t *)this );
}


//-----------------------------------------------------------------------------
// Global accessor.
//-----------------------------------------------------------------------------
typedef boost::unordered_map<std::string, CSendPropHashTable*> SendPropMap;
SendPropMap g_SendPropMap;


//-----------------------------------------------------------------------------
// CSendProp code.
//-----------------------------------------------------------------------------
CSendProp::CSendProp( edict_t* edict, const char* prop_name )
{
	// Set default values.
	m_send_prop = NULL;
	m_prop_offset = 0;
	m_edict = edict;

	if( !m_edict )
	{
		DevMsg(1, "[SP]: edict was not valid!\n");
		return;
	}

	// Get the base entity. This saves us from having to call
	// this code repeatedly.
	IServerUnknown* entity_unknown = m_edict->GetUnknown();
	m_base_entity = entity_unknown->GetBaseEntity();

	// Get the entity's classname
	const char* szClassName = m_edict->GetClassName();

	// Get the classname's prop table
	SendPropMap::iterator sendPropIter = g_SendPropMap.find(szClassName);
	if( sendPropIter == g_SendPropMap.end() )
	{
		// If the classname isn't mapped, map it
		g_SendPropMap.insert(std::make_pair(szClassName, new CSendPropHashTable()));

		// Get the classname's prop table
		sendPropIter = g_SendPropMap.find(szClassName);
	}

	// Create an offset value to be set by the hash table
	int iOffset = 0;

	// Get the SendProp from the hash table
	m_send_prop = sendPropIter->second->get_prop(prop_name, iOffset);

	// Was the SendProp not found in the hash table?
	if( !m_send_prop )
	{

		// Get the send table for this entity.
		ServerClass* server_class = m_edict->GetNetworkable()->GetServerClass();
		SendTable* send_table = server_class->m_pTable;

		// Split the prop_name by "."
		std::vector<std::string> tokens;
		boost::algorithm::split(tokens, prop_name, boost::is_any_of("."));

		// Create base variables to use in the foreach loop
		unsigned int i = 0;
		SendProp* send_prop;

		// Create a loop to cycle through each token in the given prop_name
		BOOST_FOREACH(std::string token, tokens)
		{
			// Find the SendProp instance for the current token
			const char* str_token = token.c_str();
			send_prop = UTIL_FindSendProp(send_table, str_token, iOffset);

			// Does the SendProp exist?
			if( !send_prop )
			{
				// If not, raise an exception
				std::string szMessage = "prop_name '";
				szMessage += prop_name;
				szMessage += "' was not found!\n";
				BOOST_RAISE_EXCEPTION(PyExc_ValueError, szMessage.c_str());
				return;
			}
			// Increment the counter
			i ++;

			// Is this the end of the loop?
			if( i < tokens.size() )
			{
				// If not, is the current SendProp a datatable?
				if( send_prop->GetType() != DPT_DataTable )
				{
					// If not, raise an exception
					std::string szMessage = "prop_name '";
					szMessage += prop_name;
					szMessage += "' was not found!\n";
					BOOST_RAISE_EXCEPTION(PyExc_ValueError, szMessage.c_str());
					return;
				}
				// Set the current datatable
				send_table = send_prop->GetDataTable();
			}
			else
			{
				// Store the SendProp
				m_send_prop = send_prop;

				// Store the offset
				m_prop_offset = iOffset;

				// Insert the prop into the hash table
				sendPropIter->second->insert_offset(prop_name, m_send_prop, m_prop_offset);
			}
		}
	}
	else
	{
		// Prop was valid. Store off the offset.
		m_prop_offset = iOffset;
	}
}

void CSendProp::set_int( int iValue )
{
	// Is the property of "integer" type?
	if( m_send_prop->GetType() == DPT_Int )
	{
		// Set the value.
		*(int *)((char *)m_base_entity + m_prop_offset) = iValue;

		// Force a network update.
		m_edict->StateChanged();
	}
	else
	{
		BOOST_RAISE_EXCEPTION(PyExc_TypeError, "Property is not an integer.");
	}
}

void CSendProp::set_float( float flValue )
{
	// Is the property of "float" type?
	if( m_send_prop->GetType() == DPT_Float )
	{
		// Set the value.
		*(float *)((char *)m_base_entity + m_prop_offset) = flValue;

		// Force a network update.
		m_edict->StateChanged();
	}
	else
	{
		BOOST_RAISE_EXCEPTION(PyExc_TypeError, "Property is not a float.");
	}
}

void CSendProp::set_string( const char* szValue )
{
	// Is the property of "string" type?
	if( m_send_prop->GetType() == DPT_String )
	{
		// Get the address of the string buffer.
		char* data_buffer = (char *)((char *)m_base_entity + m_prop_offset);

		// Write the string to the buffer.
		V_strncpy(data_buffer, szValue, DT_MAX_STRING_BUFFERSIZE);

		// Force a network update.
		m_edict->StateChanged();
	}
	else
	{
		BOOST_RAISE_EXCEPTION(PyExc_TypeError, "Property is not a string.");
	}
}

void CSendProp::set_vector( CVector* vecValue )
{
	// Is the property of "vector" type?
	if( m_send_prop->GetType() == DPT_Vector )
	{
		*(Vector *)((char *)m_base_entity + m_prop_offset) = *(Vector *) vecValue;
		
		// Force a network update.
		m_edict->StateChanged();
	}
	else
	{
		BOOST_RAISE_EXCEPTION(PyExc_TypeError, "Property is not a vector.");
	}
}

int CSendProp::get_int()
{
	// Is the property of "integer" type?
	if( m_send_prop->GetType() == DPT_Int )
	{
		return *(int *)((char *)m_base_entity + m_prop_offset);
	}

	BOOST_RAISE_EXCEPTION(PyExc_TypeError, "Property is not an integer.");
	return NULL;
}

float CSendProp::get_float()
{
	// Is the property of "float" type?
	if( m_send_prop->GetType() == DPT_Float )
	{
		return *(float *)((char *)m_base_entity + m_prop_offset);
	}

	BOOST_RAISE_EXCEPTION(PyExc_TypeError, "Property is not a float.");
	return NULL;
}

const char* CSendProp::get_string()
{
	// Is the property of "string" type?
	if( m_send_prop->GetType() == DPT_String )
	{
		return (const char *)((char *)m_base_entity + m_prop_offset);
	}

	BOOST_RAISE_EXCEPTION(PyExc_TypeError, "Property is not a string.");
	return NULL;
}

CVector* CSendProp::get_vector()
{
	// Is the property of "vector" type?
	if( m_send_prop->GetType() == DPT_Vector )
	{
		return new CVector(*(Vector *) ((char *)m_base_entity + m_prop_offset));
	}

	BOOST_RAISE_EXCEPTION(PyExc_TypeError, "Property is not a vector.");
	return NULL;
}