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

//---------------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------------
#include "keyvalues_wrap.h"
#include "filesystem.h"

//---------------------------------------------------------------------------------
// Defined by sp_main.cpp
//---------------------------------------------------------------------------------
extern IFileSystem* filesystem;

//---------------------------------------------------------------------------------
// CKeyValues code.
//---------------------------------------------------------------------------------
CKeyValues::CKeyValues( KeyValues* child )
{
	m_key_values = child;
	m_is_child = true;

	if( !m_key_values )
	{
		DevMsg(1, "[SP] Child keyvalues not valid!\n");
	}
}

CKeyValues::CKeyValues( const char* name )
{
	m_key_values = new KeyValues(name);
	m_is_child = false;
}

CKeyValues::~CKeyValues()
{
	if( !m_is_child && m_key_values )
	{
		// Super lame but KeyValues calls delete on itself...
		m_key_values->deleteThis();
	}
}

const char* CKeyValues::get_name() const
{
	if( m_key_values )
	{
		return m_key_values->GetName();
	}
	
	return "";
}

void CKeyValues::set_name( const char* name )
{
	if( m_key_values ) 
	{
		m_key_values->SetName(name);
	}
}

int CKeyValues::get_name_symbol() const
{
	if( m_key_values ) 
	{
		return m_key_values->GetNameSymbol();
	}

	return -1;
}

void CKeyValues::uses_escape_sequences( bool state )
{
	if( m_key_values ) 
	{
		m_key_values->UsesEscapeSequences(state);
	}
}

bool CKeyValues::load_from_file( const char* file_name )
{
	if( m_key_values ) 
	{
		return m_key_values->LoadFromFile(filesystem, file_name);
	}

	return false;
}

bool CKeyValues::save_to_file( const char* file_name )
{
	if( m_key_values ) 
	{
		return m_key_values->SaveToFile(filesystem, file_name);
	}
	
	return false;
}

CKeyValues* CKeyValues::find_key( const char* key_name, bool create_key /*= false */ )
{
	if( m_key_values ) 
	{
		return new CKeyValues(m_key_values->FindKey(key_name, create_key));
	}

	return NULL;
}

CKeyValues* CKeyValues::find_key_by_symbol( int key )
{
	if( m_key_values )
	{
		return new CKeyValues(m_key_values->FindKey(key));
	}

	return NULL;
}

CKeyValues* CKeyValues::create_new_key()
{
	if( m_key_values )
	{
		return new CKeyValues(m_key_values->CreateNewKey());
	}

	return NULL;
}

void CKeyValues::add_sub_key( CKeyValues* sub_key )
{
	if( m_key_values && sub_key )
	{
		m_key_values->AddSubKey(sub_key->m_key_values);
	}
}

void CKeyValues::remove_sub_key( CKeyValues* sub_key )
{
	if( m_key_values )
	{
		m_key_values->RemoveSubKey(sub_key->m_key_values);
	}
}

CKeyValues* CKeyValues::get_first_sub_key()
{
	if( m_key_values )
	{
		return new CKeyValues(m_key_values->GetFirstSubKey());
	}

	return NULL;
}

CKeyValues* CKeyValues::get_next_key()
{
	if( m_key_values )
	{
		return new CKeyValues(m_key_values->GetNextKey());
	}

	return NULL;
}

CKeyValues* CKeyValues::get_first_true_sub_key()
{
	if( m_key_values )
	{
		return new CKeyValues(m_key_values->GetFirstTrueSubKey());
	}

	return NULL;
}

CKeyValues* CKeyValues::get_next_true_sub_key()
{
	if( m_key_values )
	{
		return new CKeyValues(m_key_values->GetNextTrueSubKey());
	}

	return NULL;
}

CKeyValues* CKeyValues::get_first_value()
{
	if( m_key_values )
	{
		return new CKeyValues(m_key_values->GetFirstValue());
	}

	return NULL;
}

CKeyValues* CKeyValues::get_next_value()
{
	if( m_key_values )
	{
		return new CKeyValues(m_key_values->GetNextValue());
	}

	return NULL;
}

int CKeyValues::get_int( const char* key_name, int default_value /*= 0 */ )
{
	if( m_key_values )
	{
		return m_key_values->GetInt(key_name, default_value);
	}

	return default_value;
}

uint64 CKeyValues::get_uint64( const char* key_name, uint64 default_value /*= 0 */ )
{
	if( m_key_values )
	{
		return m_key_values->GetUint64(key_name, default_value);
	}

	return default_value;
}

float CKeyValues::get_float( const char* key_name, float default_value /*= 0.0f */ )
{
	if( m_key_values )
	{
		return m_key_values->GetFloat(key_name, default_value);
	}

	return default_value;
}

const char* CKeyValues::get_string( const char* key_name, const char* default_value /*= "" */ )
{
	if( m_key_values )
	{
		return m_key_values->GetString(key_name, default_value);
	}

	return default_value;
}

bool CKeyValues::get_bool( const char* key_name, bool default_value /*= false */ )
{
	if( m_key_values )
	{
		return false; // m_key_values->GetBool(key_name, default_value);
	}

	return default_value;
}

bool CKeyValues::is_empty( const char* key_name /*= NULL */ )
{
	if( m_key_values )
	{
		return m_key_values->IsEmpty(key_name);
	}

	return true;
}

void CKeyValues::set_string( const char* key_name, const char* value )
{
	if( m_key_values )
	{
		m_key_values->SetString(key_name, value);
	}
}

void CKeyValues::set_int( const char* key_name, int value )
{
	if( m_key_values )
	{
		m_key_values->SetInt(key_name, value);
	}
}

void CKeyValues::set_uint64( const char* key_name, uint64 value )
{
	if( m_key_values )
	{
		m_key_values->SetUint64(key_name, value);
	}
}

void CKeyValues::set_float( const char* key_name, float value )
{
	if( m_key_values )
	{
		m_key_values->SetFloat(key_name, value);
	}
}

void CKeyValues::set_bool( const char* key_name, bool value )
{
	if( m_key_values )
	{
		// m_key_values->SetBool(key_name, value);
	}
}




