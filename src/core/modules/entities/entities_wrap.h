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
class CEdictExt
{
public:
	int         GetPropInt( edict_t* pEdict, const char* prop_name );
	float       GetPropFloat( edict_t* pEdict, const char* prop_name );
	const char* GetPropString( edict_t* pEdict, const char* prop_name );
	Vector      GetPropVector( edict_t* pEdict, const char* prop_name );

	void        SetPropInt( edict_t* pEdict, const char* prop_name, int iValue );
	void        SetPropFloat( edict_t* pEdict, const char* prop_name, float flValue );
	void        SetPropString( edict_t* pEdict, const char* prop_name, const char* szValue );
	void        SetPropVector( edict_t* pEdict, const char* prop_name, Vector vecValue );
};

//---------------------------------------------------------------------------------
// Custom SendProp wrapper.
//---------------------------------------------------------------------------------
class CSendProp
{
public:
	CSendProp( edict_t* edict, const char* prop_name );

	SendPropType GetType();

	template<class T>
	T Get()
	{ return *(T *) ((char *) m_base_entity + m_prop_offset); }

	template<class T>
	void Set(T value)
	{
		*(T *)((char *) m_base_entity + m_prop_offset) = value;
		
		// Force a network update.
		m_edict->StateChanged();
	}

	template<>
	void Set(const char* szValue)
	{
		// Get the address of the string buffer.
		char* data_buffer = (char *)((char *)m_base_entity + m_prop_offset);

		// Write the string to the buffer.
		V_strncpy(data_buffer, szValue, DT_MAX_STRING_BUFFERSIZE);
		
		// Force a network update.
		m_edict->StateChanged();
	}

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
