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
// External variables.
//-----------------------------------------------------------------------------
extern CGlobalVars* gpGlobals;

//-----------------------------------------------------------------------------
// Global accessor.
//-----------------------------------------------------------------------------
typedef boost::unordered_map<std::string, CSendPropHashTable*> SendPropMap;
SendPropMap g_SendPropMap;

//-----------------------------------------------------------------------------
// CEdict methods.
//-----------------------------------------------------------------------------
CEdict::CEdict( edict_t* edict_ptr )
{
	m_edict_ptr = edict_ptr;
	m_is_valid = (m_edict_ptr != NULL);
	m_index = IndexOfEdict(m_edict_ptr);
}

CEdict::CEdict( int index )
{
	m_edict_ptr = PEntityOfEntIndex(index);
	m_is_valid = (m_edict_ptr != NULL);
	m_index = index;
}

CEdict::CEdict( const char* name, bool bExact /* = true */ )
{
	const int max_entities = gpGlobals->maxEntities;
	for( int i = 0; i < max_entities; i++ )
	{
		edict_t* edict = PEntityOfEntIndex(i); // &gpGlobals->pEdicts[i];
		
		// Rule out empty edicts.
		if( !edict || edict->IsFree() ) {
			continue;
		}

		// Check the name if we want a partial match.
		if( !bExact ) 
		{
			if( V_stristr(edict->GetClassName(), name) ) 
			{
				m_edict_ptr = edict;
				m_is_valid = true;
				m_index = i;
				return;
			}
		}
		else 
		{
			// Compare names otherwise.
			if( V_strcmp(edict->GetClassName(), name) == 0 )
			{
				m_edict_ptr = edict;
				m_is_valid = true;
				m_index = i;
				return;
			}
		}
	}

	m_edict_ptr = NULL;
	m_is_valid = false;
	m_index = -1;
}

int CEdict::area_num() const
{
	return m_edict_ptr->AreaNum();
}

const char* CEdict::get_class_name() const
{
	return m_edict_ptr->GetClassName();
}

bool CEdict::is_free() const
{
	return m_edict_ptr->IsFree();
}

void CEdict::set_free()
{
	return m_edict_ptr->SetFree();
}

void CEdict::clear_free()
{
	m_edict_ptr->ClearFree();
}

bool CEdict::is_valid() const
{
	return m_is_valid;
}

int CEdict::get_index() const
{
	return m_index;
}

CServerNetworkable* CEdict::get_networkable() const
{
	IServerNetworkable* networkable = m_edict_ptr->GetNetworkable();
	CServerNetworkable* new_networkable = new CServerNetworkable(networkable);
	return new_networkable;
}

CServerUnknown* CEdict::get_unknown() const
{
	IServerUnknown* unknown = m_edict_ptr->GetUnknown();
	CServerUnknown* new_unknown = new CServerUnknown(unknown);
	return new_unknown;
}

CServerEntity* CEdict::get_server_entity() const
{
	IServerEntity* server_entity = m_edict_ptr->GetIServerEntity();
	CServerEntity* new_server_entity = new CServerEntity(server_entity);
	return new_server_entity;
}

int CEdict::get_prop_int( const char* prop_name )
{
	CSendProp* send_prop = new CSendProp(m_edict_ptr, prop_name);
	if( !send_prop )
	{
		return NULL;
	}
	return send_prop->get_int();
}

float CEdict::get_prop_float( const char* prop_name )
{
	CSendProp* send_prop = new CSendProp(m_edict_ptr, prop_name);
	if( !send_prop )
	{
		return NULL;
	}
	return send_prop->get_float();
}

const char* CEdict::get_prop_string( const char* prop_name )
{
	CSendProp* send_prop = new CSendProp(m_edict_ptr, prop_name);
	if( !send_prop )
	{
		return NULL;
	}
	return send_prop->get_string();
}

CVector* CEdict::get_prop_vector( const char* prop_name )
{
	CSendProp* send_prop = new CSendProp(m_edict_ptr, prop_name);
	if( !send_prop )
	{
		return NULL;
	}
	return send_prop->get_vector();
}

void CEdict::set_prop_int( const char* prop_name, int iValue )
{
	CSendProp* send_prop = new CSendProp(m_edict_ptr, prop_name);
	if( send_prop )
	{
		send_prop->set_int(iValue);
	}
}

void CEdict::set_prop_float( const char* prop_name, float flValue )
{
	CSendProp* send_prop = new CSendProp(m_edict_ptr, prop_name);
	if( send_prop )
	{
		send_prop->set_float(flValue);
	}
}

void CEdict::set_prop_string( const char* prop_name, const char* szValue )
{
	CSendProp* send_prop = new CSendProp(m_edict_ptr, prop_name);
	if( send_prop )
	{
		send_prop->set_string(szValue);
	}
}

void CEdict::set_prop_vector( const char* prop_name, CVector* vecValue )
{
	CSendProp* send_prop = new CSendProp(m_edict_ptr, prop_name);
	if( send_prop )
	{
		send_prop->set_vector(vecValue);
	}
}

edict_t* CEdict::get_edict()
{
	return m_edict_ptr;
}

//-----------------------------------------------------------------------------
// CBaseEntityHandle code.
//-----------------------------------------------------------------------------
CBaseEntityHandle::CBaseEntityHandle( int handle ) : m_base_handle(handle)
{
}

CBaseEntityHandle::CBaseEntityHandle( const CBaseHandle& other ) : m_base_handle(other)
{
}

bool CBaseEntityHandle::is_valid()
{
	return m_base_handle.IsValid();
}

int CBaseEntityHandle::get_entry_index() const
{
	return m_base_handle.GetEntryIndex();
}

int CBaseEntityHandle::get_serial_number() const
{
	return m_base_handle.GetSerialNumber();
}

int CBaseEntityHandle::to_int() const
{
	return m_base_handle.ToInt();
}

//-----------------------------------------------------------------------------
// CServerNetworkable code.
//-----------------------------------------------------------------------------
CServerNetworkable::CServerNetworkable( IServerNetworkable* server_networkable )
{
	m_server_networkable = server_networkable;
}

CHandleEntity* CServerNetworkable::get_entity_handle()
{
	IHandleEntity* handle = m_server_networkable->GetEntityHandle();
	CHandleEntity* new_handle = new CHandleEntity(handle);
	return new_handle;
}

CEdict* CServerNetworkable::get_edict()
{
	edict_t* edict = m_server_networkable->GetEdict();
	CEdict* new_edict = new CEdict(edict);
	return new_edict;
}

const char* CServerNetworkable::get_class_name()
{
	return m_server_networkable->GetClassName();
}

//-----------------------------------------------------------------------------
// CHandleEntity code.
//-----------------------------------------------------------------------------
CHandleEntity::CHandleEntity( IHandleEntity* handle_entity )
{
	m_handle_entity = handle_entity;
}

const CBaseEntityHandle* CHandleEntity::get_ref_ehandle() const
{
	const CBaseHandle& handle = m_handle_entity->GetRefEHandle();
	const CBaseEntityHandle* new_entity_handle = new CBaseEntityHandle(handle);
	return new_entity_handle;
}

//-----------------------------------------------------------------------------
// CServerUnknown code.
//-----------------------------------------------------------------------------
CServerUnknown::CServerUnknown( IServerUnknown* server_unknown )
{
	m_server_unknown = server_unknown;
}

/*ICollideable* CServerUnknown::get_collideable()
{
	return m_server_unknown->GetCollideable();
}*/

CServerNetworkable* CServerUnknown::get_networkable()
{
	IServerNetworkable* networkable = m_server_unknown->GetNetworkable();
	CServerNetworkable* new_networkable = new CServerNetworkable(networkable);
	return new_networkable;
}

unsigned long CServerUnknown::get_base_entity()
{
	return (unsigned long)(m_server_unknown->GetBaseEntity());
}

//-----------------------------------------------------------------------------
// CServerEntity code.
//-----------------------------------------------------------------------------
CServerEntity::CServerEntity( IServerEntity* server_entity )
{
	m_server_entity = server_entity;
}

int CServerEntity::get_model_index() const
{
	return m_server_entity->GetModelIndex();
}

void CServerEntity::set_model_index( int index )
{
	m_server_entity->SetModelIndex(index);
}

const char* CServerEntity::get_model_name()
{
	return m_server_entity->GetModelName().ToCStr();
}

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