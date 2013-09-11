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

#ifndef _KEYVALUES_WRAP_H
#define _KEYVALUES_WRAP_H

//---------------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------------
#include "KeyValues.h"

//---------------------------------------------------------------------------------
// Keyvalues wrapper class.
//---------------------------------------------------------------------------------
class CKeyValues
{
public:
	CKeyValues( KeyValues* child );
	CKeyValues( const char* name );
	~CKeyValues();

public:
	const char* get_name() const;
	void		set_name( const char* name );

	int			get_name_symbol() const;

	void		uses_escape_sequences( bool state );
	
	bool		load_from_file( const char* file_name );
	bool		save_to_file( const char* file_name );
	
	CKeyValues* find_key( const char* key_name, bool create_key = false );
	CKeyValues* find_key_by_symbol( int key );
	
	CKeyValues*	create_new_key();

	void		add_sub_key( CKeyValues* sub_key );
	void		remove_sub_key( CKeyValues* sub_key );
	
	CKeyValues*	get_first_sub_key();
	CKeyValues* get_next_key();

	CKeyValues* get_first_true_sub_key();
	CKeyValues* get_next_true_sub_key();

	CKeyValues*	get_first_value();
	CKeyValues* get_next_value();

	int			get_int( const char* key_name, int default_value = 0 );
	uint64		get_uint64( const char* key_name, uint64 default_value = 0 );
	float		get_float( const char* key_name, float default_value = 0.0f );
	const char* get_string( const char* key_name, const char* default_value = "" );
	bool		get_bool( const char* key_name, bool default_value = false );

	bool		is_empty( const char* key_name = NULL );

	void		set_string( const char* key_name, const char* value );
	void		set_int( const char* key_name, int value );
	void		set_uint64( const char* key_name, uint64 value );
	void		set_float( const char* key_name, float value );
	void		set_bool( const char* key_name, bool value );

private:
	KeyValues*	m_key_values;

	// This is set to true if we're an instance returned by
	// find_key(..) from a parent class. If this is set,
	// we will not free the KeyValues instance when this
	// object destructs.
	bool		m_is_child;
};


#endif // _KEYVALUES_WRAP_H