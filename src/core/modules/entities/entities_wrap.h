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
#ifndef _ENTITIES_WRAP_H
#define _ENTITIES_WRAP_H

//---------------------------------------------------------------------------------
// Includes.
//---------------------------------------------------------------------------------
#include "edict.h"
#include "server_class.h"
#include <cstdint>
#include "modules/vecmath/vecmath_wrap.h"


//---------------------------------------------------------------------------------
// edict_t extension
//---------------------------------------------------------------------------------
/*
class edict_tExt
{
public:
	int         GetPropInt( const char* prop_name );
	float       GetPropFloat( const char* prop_name );
	const char* GetPropString( const char* prop_name );
	Vector*     GetPropVector( const char* prop_name );
	object      GetProp( const char* prop_name );

	void        SetPropInt( const char* prop_name, int iValue );
	void        SetPropFloat( const char* prop_name, float flValue );
	void        SetPropString( const char* prop_name, const char* szValue );
	void        SetPropVector( const char* prop_name, Vector* vecValue );
	void        SetProp( const char* prop_name, object value);
};
*/

//---------------------------------------------------------------------------------
// Custom SendProp wrapper.
//---------------------------------------------------------------------------------
class CSendProp
{
public:
	CSendProp( edict_t* edict, const char* prop_name );

	SendPropType	get_type();

	void			set_int( int value );
	void			set_float( float value );
	void			set_string( const char* value );
	void			set_vector( CVector* pVec );

	int				get_int();
	float			get_float();
	const char*		get_string();
	CVector*		get_vector();

private:
	// Offset from the beginning of the network table that
	// this prop is located at.
	unsigned int	m_prop_offset;

	// Base entity instance.
	CBaseEntity*	m_base_entity;

	// Edict instance.
	edict_t*		m_edict;

	// The actual send prop object.
	SendProp*		m_send_prop;
};

#endif
