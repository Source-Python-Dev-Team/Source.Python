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

#ifndef _ENTITIES_ENTITY_H
#define _ENTITIES_ENTITY_H

//-----------------------------------------------------------------------------
// Includes.
//-----------------------------------------------------------------------------
#include "utilities/baseentity.h"
#include "toolframework/itoolentity.h"
#include "modules/memory/memory_pointer.h"


//-----------------------------------------------------------------------------
// Definitions.
//-----------------------------------------------------------------------------
#define MAX_KEY_VALUE_LENGTH 1024


//-----------------------------------------------------------------------------
// External variables.
//-----------------------------------------------------------------------------
extern IServerTools* servertools;
extern CGlobalVars *gpGlobals;


//-----------------------------------------------------------------------------
// IServerUnknown extension class.
//-----------------------------------------------------------------------------
class IServerUnknownExt
{
public:
	static const char* GetClassname(IServerUnknown* pUnknown)
	{
		IServerNetworkable* pNetworkable = pUnknown->GetNetworkable();
		if (!pNetworkable)
			BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Failed to get the IServerNetworkable pointer.");

		return pNetworkable->GetClassName();
	}

	static bool IsNetworked(IServerUnknown* pUnknown)
	{
		IServerNetworkable *pServerNetworkable = pUnknown->GetNetworkable();
		if (pServerNetworkable)
			return pServerNetworkable->GetEdict() != NULL;

		return false;
	}
};


//-----------------------------------------------------------------------------
// CBaseEntity extension class.
//-----------------------------------------------------------------------------
class CBaseEntityWrapper: public IServerEntity
{
private:
	// Make sure that nobody can call the constructor/destructor
	CBaseEntityWrapper() {}
	~CBaseEntityWrapper() {}

public:
	// We need to keep the order of these methods up-to-date and maybe we need
	// to add new methods for other games.
	// TODO: Do we want to do this? Or do we want to dynamically call the methods from Python?
	virtual ServerClass* GetServerClass() = 0;
	virtual int YouForgotToImplementOrDeclareServerClass() = 0;
	virtual datamap_t* GetDataDescMap() = 0;

public:
	static boost::shared_ptr<CBaseEntityWrapper> __init__(unsigned int uiEntityIndex);
	static boost::shared_ptr<CBaseEntityWrapper> wrap(CBaseEntity* pEntity);
	static CBaseEntity* create(const char* name);
	static CBaseEntity* find(const char* name);
	static CBaseEntity* find_or_create(const char* name);

	CBaseEntity* GetThis();

	// Datamap methods
	int FindDataMapOffset(const char* name);

	template<class T>
	T GetDatamapProperty(const char* name)
	{
		return *(T*) (((unsigned long) this) + FindDataMapOffset(name));
	}

	const char* GetDatamapPropertyStringArray(const char* name)
	{
		return (const char*) (((unsigned long) this) + FindDataMapOffset(name));
	}

	template<class T>
	void SetDatamapProperty(const char* name, T value)
	{
		*(T*) (((unsigned long) this) + FindDataMapOffset(name)) = value;
	}

	void SetDatamapPropertyStringArray(const char* name, const char* value)
	{
		strcpy((char*) (((unsigned long) this) + FindDataMapOffset(name)), value);
	}

	// Network property methods
	int FindNetworkPropertyOffset(const char* name);

	template<class T>
	T GetNetworkProperty(const char* name)
	{
		return *(T *) (((unsigned long) this) + FindNetworkPropertyOffset(name));
	}

	const char* GetNetworkPropertyStringArray(const char* name)
	{
		return (const char*) (((unsigned long) this) + FindNetworkPropertyOffset(name));
	}

	template<class T>
	void SetNetworkProperty(const char* name, T value)
	{
		*(T *) (((unsigned long) this) + FindNetworkPropertyOffset(name)) = value;
		GetEdict()->StateChanged();
	}

	void SetNetworkPropertyStringArray(const char* name, const char* value)
	{
		strcpy((char*) (((unsigned long) this) + FindNetworkPropertyOffset(name)), value);
		GetEdict()->StateChanged();
	}

	// KeyValue methods
	void GetKeyValueStringRaw(const char* szName, char* szOut, int iLength);
	str GetKeyValueString(const char* szName);
	long GetKeyValueInt(const char* szName);
	double GetKeyValueFloat(const char* szName);
	Vector GetKeyValueVector(const char* szName);
	bool GetKeyValueBool(const char* szName);
	Color GetKeyValueColor(const char* szName);
	void SetKeyValueColor(const char* szName, Color color);

	template<class T>
	void SetKeyValue(const char* szName, T value)
	{
		/*
			Removing this conditional/exception as it causes issues with certain entity keyvalues
		*/
		//	if (!servertools->SetKeyValue(GetThis(), szName, value))
		//		BOOST_RAISE_EXCEPTION(PyExc_NameError, "\"%s\" is not a valid KeyValue for entity class \"%s\".",
		//			szName, GetDataDescMap()->dataClassName);

		servertools->SetKeyValue(GetThis(), szName, value);
	}

	// Conversion methods
	edict_t* GetEdict();
	unsigned int GetIndex();
	CPointer GetPointer();
	unsigned int GetIntHandle();

	// Other methods
	bool IsPlayer();
};


#endif // _ENTITIES_ENTITY_H
