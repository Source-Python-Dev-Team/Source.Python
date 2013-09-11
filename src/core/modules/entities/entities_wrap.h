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
// Forward declarations
//---------------------------------------------------------------------------------
class CServerEntity;
class CServerNetworkable;
class CServerUnknown;
class CServerClass;
class CSendProp;

//---------------------------------------------------------------------------------
// The base class for all entities.
// Note: This class may not be instantiated directly.
//---------------------------------------------------------------------------------
class CEdict
{
public:
	// This awful hack allows us to convert pointers returned by
	// dyncall to the edict class.
	// CEdict( uintptr_t edict_ptr );

	// Get edict instances by index.
	CEdict( int index );

	// Get edict instances by name.
	CEdict( const char* name, bool bExact = true );

	// For engine use.
	CEdict( edict_t* edict_ptr );

	virtual int						area_num() const;
	virtual const char*				get_class_name() const;

	virtual bool						is_free() const;
	virtual void						set_free();
	virtual void						clear_free();

	// Helper methods.
	virtual bool						is_valid() const;
	virtual int						get_index() const;
	virtual CServerNetworkable*		get_networkable() const;
	virtual CServerUnknown*			get_unknown() const;
	virtual CServerEntity*				get_server_entity() const;

	// Send property methods.
	int									get_prop_int( const char* prop_name );
	float								get_prop_float( const char* prop_name );
	const char*						get_prop_string( const char* prop_name );
	CVector*							get_prop_vector( const char* prop_name );
	void								set_prop_int( const char* prop_name, int iValue );
	void								set_prop_float( const char* prop_name, float flValue );
	void								set_prop_string( const char* prop_name, const char* szValue );
	void								set_prop_vector( const char* prop_name, CVector* vecValue );

	virtual edict_t*					get_edict();

private:
	edict_t*	m_edict_ptr;
	bool		m_is_valid;
	int			m_index;
};


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

//---------------------------------------------------------------------------------
// CBaseHandleEntity wrapper class.
//---------------------------------------------------------------------------------
class CBaseEntityHandle
{
public:
	CBaseEntityHandle( int handle );
	CBaseEntityHandle( const CBaseHandle& other );

	bool	is_valid();
	
	int		get_entry_index() const;
	int		get_serial_number() const;
	
	int		to_int() const;

private:
	CBaseHandle m_base_handle;
};

//---------------------------------------------------------------------------------
// IHandleEntity wrapper class.
//---------------------------------------------------------------------------------
class CHandleEntity
{
public:
	CHandleEntity( IHandleEntity* handle_entity );

	const CBaseEntityHandle* get_ref_ehandle() const;

private:
	IHandleEntity* m_handle_entity;
};

//---------------------------------------------------------------------------------
// IServerNetworkable wrapper class.
//---------------------------------------------------------------------------------
class CServerNetworkable
{
public:
	CServerNetworkable( IServerNetworkable* server_networkable );

	CHandleEntity* get_entity_handle();
	CEdict* get_edict();
	virtual const char* get_class_name();

private:
	IServerNetworkable* m_server_networkable;
};

//---------------------------------------------------------------------------------
// IServerUnknown wrapper class.
//---------------------------------------------------------------------------------
class CServerUnknown
{
public:
	CServerUnknown( IServerUnknown* server_unknown );

	// ICollideable* get_collideable();
	CServerNetworkable* get_networkable();
	unsigned long get_base_entity();

private:
	IServerUnknown* m_server_unknown;
};

//---------------------------------------------------------------------------------
// IServerEntity wrapper class.
//---------------------------------------------------------------------------------
class CServerEntity
{
public:
	CServerEntity( IServerEntity* server_entity );

	virtual int			get_model_index() const;
	virtual void			set_model_index( int index );
	virtual const char*	get_model_name();

private:
	IServerEntity* m_server_entity;
};

#endif
