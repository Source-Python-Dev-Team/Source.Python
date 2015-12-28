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
#include "utilities/sp_util.h"
#include "utilities/conversions.h"
#include "utilities/wrap_macros.h"
#include "toolframework/itoolentity.h"
#include "entities.h"


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
	static boost::shared_ptr<CBaseEntityWrapper> __init__(unsigned int uiEntityIndex)
	{
		return CBaseEntityWrapper::wrap(ExcBaseEntityFromIndex(uiEntityIndex));
	}

	static boost::shared_ptr<CBaseEntityWrapper> wrap(CBaseEntity* pEntity)
	{
		return boost::shared_ptr<CBaseEntityWrapper>(
			(CBaseEntityWrapper *) pEntity,
			&NeverDeleteDeleter<CBaseEntityWrapper *>
		);
	}

	// TODO: Why doesn't implicit conversion work?
	// operaton CBaseEntity* () { return (CBaseEntity *) this; }
	CBaseEntity* GetThis()
	{ return (CBaseEntity *) this; }

	void GetKeyValueStringRaw(const char* szName, char* szOut, int iLength)
	{
		if (!servertools->GetKeyValue(GetThis(), szName, szOut, iLength))
			BOOST_RAISE_EXCEPTION(PyExc_NameError, "\"%s\" is not a valid KeyValue for entity class \"%s\".",
				szName, GetDataDescMap()->dataClassName);
	}

	str GetKeyValueString(const char* szName)
	{
		char szResult[MAX_KEY_VALUE_LENGTH];
		GetKeyValueStringRaw(szName, szResult, MAX_KEY_VALUE_LENGTH);

		// Fix for field name "model". I think a string_t object is copied to szResult.
		if (strcmp(szName, "model") == 0)
			return *(char **) szResult;

		return str(szResult);
	}

	long GetKeyValueInt(const char* szName)
	{
		char szResult[128];
		GetKeyValueStringRaw(szName, szResult, 128);
		
		long iResult;
		if (!sputils::UTIL_StringToLong(&iResult, szResult))
			BOOST_RAISE_EXCEPTION(PyExc_ValueError, "KeyValue does not seem to be an integer: '%s'.", szResult);

		return iResult;
	}

	double GetKeyValueFloat(const char* szName)
	{
		char szResult[128];
		GetKeyValueStringRaw(szName, szResult, 128);
		
		double dResult;
		if (!sputils::UTIL_StringToDouble(&dResult, szResult))
			BOOST_RAISE_EXCEPTION(PyExc_ValueError, "KeyValue does not seem to be a float: '%s'.", szResult);

		return dResult;
	}

	Vector GetKeyValueVector(const char* szName)
	{
		char szResult[128];
		GetKeyValueStringRaw(szName, szResult, 128);

		float fResult[3];
		if (!sputils::UTIL_StringToFloatArray(fResult, 3, szResult))
			BOOST_RAISE_EXCEPTION(PyExc_ValueError, "KeyValue does not seem to be a vector: '%s'.", szResult);

		return Vector(fResult[0], fResult[1], fResult[2]);
	}

	bool GetKeyValueBool(const char* szName)
	{
		char szResult[3];
		GetKeyValueStringRaw(szName, szResult, 3);
		if (szResult[1] != '\0')
			BOOST_RAISE_EXCEPTION(PyExc_ValueError, "KeyValue does not seem to be a boolean: '%s'.", szResult);

		if (szResult[0] == '1')
			return true;
		else if (szResult[0] == '0')
			return false;
		else
			BOOST_RAISE_EXCEPTION(PyExc_ValueError, "Invalid boolean value: '%c'.", szResult[0]);

		return false; // to fix a warning.
	}

	Color GetKeyValueColor(const char* szName)
	{
		char szResult[128];
		GetKeyValueStringRaw(szName, szResult, 128);

		int iResult[4];
		if (!sputils::UTIL_StringToIntArray(iResult, 4, szResult))
			BOOST_RAISE_EXCEPTION(PyExc_ValueError, "KeyValue does not seem to be a color: '%s'.", szResult);

		return Color(iResult[0], iResult[1], iResult[2], iResult[3]);
	}

	void SetKeyValueColor(const char* szName, Color color)
	{
		char string[16];
		Q_snprintf(string, sizeof(string), "%i %i %i %i", color.r(), color.g(), color.b(), color.a());
		SetKeyValue(szName, string);
	}

	template<class T>
	void SetKeyValue(const char* szName, T value)
	{
		if (!servertools->SetKeyValue(GetThis(), szName, value))
			BOOST_RAISE_EXCEPTION(PyExc_NameError, "\"%s\" is not a valid KeyValue for entity class \"%s\".",
				szName, GetDataDescMap()->dataClassName);
	}

	edict_t* GetEdict()
	{
		return ExcEdictFromBaseEntity(GetThis());
	}
	
	unsigned int GetIndex()
	{
		return ExcIndexFromBaseEntity(GetThis());
	}
	
	CPointer GetPointer()
	{
		return ExcPointerFromBaseEntity(GetThis());
	}
	
	unsigned int GetIntHandle()
	{
		return ExcIntHandleFromBaseEntity(GetThis());
	}

	bool IsPlayer()
	{
		if (!IServerUnknownExt::IsNetworked(GetThis()))
			return false;

		unsigned int iEntityIndex;
		if (!IndexFromBaseEntity(GetThis(), iEntityIndex)) 
			return false;

		return iEntityIndex > WORLD_ENTITY_INDEX && iEntityIndex <= (unsigned int) gpGlobals->maxClients;
	}
};


#endif // _ENTITIES_ENTITY_H
