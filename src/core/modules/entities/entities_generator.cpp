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

// ----------------------------------------------------------------------------
// Includes
// ----------------------------------------------------------------------------
#include "entities_generator.h"
#include "utilities/sp_util.h"
#include "boost/python/iterator.hpp"
#include "utilities/conversions.h"
#include "toolframework/itoolentity.h"
#include "eiface.h"
#include "game/server/entityoutput.h"


//-----------------------------------------------------------------------------
// External variables.
//-----------------------------------------------------------------------------
extern IServerTools *servertools;
extern IServerGameDLL* servergamedll;


// ----------------------------------------------------------------------------
// CEntityGenerator
// ----------------------------------------------------------------------------
CEntityGenerator::CEntityGenerator( PyObject* self ):
	IPythonGenerator<edict_t>(self),
	m_pCurrentEntity((CBaseEntity *)servertools->FirstEntity()),
	m_szClassName(NULL),
	m_uiClassNameLen(0),
	m_bExactMatch(false)
{
}

CEntityGenerator::CEntityGenerator( PyObject* self, const CEntityGenerator& rhs ):
	IPythonGenerator<edict_t>(self),
	m_pCurrentEntity(rhs.m_pCurrentEntity),
	m_uiClassNameLen(rhs.m_uiClassNameLen),
	m_bExactMatch(rhs.m_bExactMatch)
{
	makeStringCopy(rhs.m_szClassName, m_uiClassNameLen);
}

CEntityGenerator::CEntityGenerator(PyObject* self, const char* szClassName):
	IPythonGenerator<edict_t>(self),
	m_pCurrentEntity((CBaseEntity *)servertools->FirstEntity()),
	m_uiClassNameLen(strlen(szClassName)),
	m_bExactMatch(false)
{
	makeStringCopy(szClassName, m_uiClassNameLen);
}

CEntityGenerator::CEntityGenerator(PyObject* self, const char* szClassName, bool bExactMatch):
	IPythonGenerator<edict_t>(self),
	m_pCurrentEntity((CBaseEntity *)servertools->FirstEntity()),
	m_uiClassNameLen(strlen(szClassName)),
	m_bExactMatch(bExactMatch)
{
	makeStringCopy(szClassName, m_uiClassNameLen);
}

CEntityGenerator::~CEntityGenerator()
{
	delete[] m_szClassName;
}

edict_t* CEntityGenerator::getNext()
{
	while (m_pCurrentEntity)
	{
		edict_t *pEdict;
		if (!EdictFromBaseEntity(m_pCurrentEntity, pEdict))
			pEdict = NULL;

		m_pCurrentEntity = (CBaseEntity *)servertools->NextEntity(m_pCurrentEntity);
		if (pEdict)
		{
			if (m_uiClassNameLen && m_szClassName)
			{
				if (!m_bExactMatch && strncmp(pEdict->GetClassName(), m_szClassName, m_uiClassNameLen) != 0)
					continue;

				else if (m_bExactMatch && strcmp(pEdict->GetClassName(), m_szClassName) != 0)
					continue;
			}
			return pEdict;
		}
	}
	return NULL;
}

void CEntityGenerator::makeStringCopy(const char* szClassName, unsigned int uiClassNameLen)
{
	if (uiClassNameLen > 0)
	{
		char* szClassNameCopy = new char[uiClassNameLen + 1];
		memcpy(szClassNameCopy, szClassName, uiClassNameLen);
		szClassNameCopy[uiClassNameLen] = 0;
		m_szClassName = szClassNameCopy;
	}
	else
	{
		m_szClassName = NULL;
	}
}


// ----------------------------------------------------------------------------
// CBaseEntityGenerator
// ----------------------------------------------------------------------------
CBaseEntityGenerator::CBaseEntityGenerator( PyObject* self ):
	IPythonGenerator<CBaseEntityWrapper>(self),
	m_pCurrentEntity((CBaseEntity *)servertools->FirstEntity()),
	m_szClassName(NULL),
	m_uiClassNameLen(0),
	m_bExactMatch(false)
{
}

CBaseEntityGenerator::CBaseEntityGenerator( PyObject* self, const CBaseEntityGenerator& rhs ):
	IPythonGenerator<CBaseEntityWrapper>(self),
	m_pCurrentEntity(rhs.m_pCurrentEntity),
	m_uiClassNameLen(rhs.m_uiClassNameLen),
	m_bExactMatch(rhs.m_bExactMatch)
{
	makeStringCopy(rhs.m_szClassName, m_uiClassNameLen);
}

CBaseEntityGenerator::CBaseEntityGenerator(PyObject* self, const char* szClassName):
	IPythonGenerator<CBaseEntityWrapper>(self),
	m_pCurrentEntity((CBaseEntity *)servertools->FirstEntity()),
	m_uiClassNameLen(strlen(szClassName)),
	m_bExactMatch(false)
{
	makeStringCopy(szClassName, m_uiClassNameLen);
}

CBaseEntityGenerator::CBaseEntityGenerator(PyObject* self, const char* szClassName, bool bExactMatch):
	IPythonGenerator<CBaseEntityWrapper>(self),
	m_pCurrentEntity((CBaseEntity *)servertools->FirstEntity()),
	m_uiClassNameLen(strlen(szClassName)),
	m_bExactMatch(bExactMatch)
{
	makeStringCopy(szClassName, m_uiClassNameLen);
}

void CBaseEntityGenerator::makeStringCopy(const char* szClassName, unsigned int uiClassNameLen)
{
	if (uiClassNameLen > 0)
	{
		char* szClassNameCopy = new char[uiClassNameLen + 1];
		memcpy(szClassNameCopy, szClassName, uiClassNameLen);
		szClassNameCopy[uiClassNameLen] = 0;
		m_szClassName = szClassNameCopy;
	}
	else
	{
		m_szClassName = NULL;
	}
}

CBaseEntityGenerator::~CBaseEntityGenerator()
{
	delete[] m_szClassName;
}

CBaseEntityWrapper* CBaseEntityGenerator::getNext()
{
	CBaseEntity* result = NULL;
	while (m_pCurrentEntity)
	{
		result = m_pCurrentEntity;
		m_pCurrentEntity = (CBaseEntity *)servertools->NextEntity(m_pCurrentEntity);
		if (result && m_uiClassNameLen && m_szClassName)
		{
			const char* szClassname = IServerUnknownExt::GetClassname(result);
			if (!m_bExactMatch && strncmp(szClassname, m_szClassName, m_uiClassNameLen) != 0)
				continue;

			else if (m_bExactMatch && strcmp(szClassname, m_szClassName) != 0)
				continue;
		}
		return (CBaseEntityWrapper*) result;
	}
	return NULL;
}


// ----------------------------------------------------------------------------
// CServerClassGenerator
// ----------------------------------------------------------------------------
CServerClassGenerator::CServerClassGenerator( PyObject* self ):
	IPythonGenerator<ServerClass>(self)
{
	m_pCurrentServerClass = servergamedll->GetAllServerClasses();
}

CServerClassGenerator::CServerClassGenerator( PyObject* self, const CServerClassGenerator& rhs ):
	IPythonGenerator<ServerClass>(self)
{
	m_pCurrentServerClass = rhs.m_pCurrentServerClass;
}

ServerClass* CServerClassGenerator::getNext()
{
	if (!m_pCurrentServerClass)
		return NULL;

	ServerClass* result = m_pCurrentServerClass;
	m_pCurrentServerClass = m_pCurrentServerClass->m_pNext;
	return result;
}


// ----------------------------------------------------------------------------
// CEventActionGenerator
// ----------------------------------------------------------------------------
CEventActionGenerator::CEventActionGenerator( PyObject* self, CEventAction *pFirstEventAction ):
	IPythonGenerator<CEventAction>(self)
{
	m_pCurrentEventAction = pFirstEventAction;
}

CEventActionGenerator::CEventActionGenerator( PyObject* self, const CEventActionGenerator& rhs ):
	IPythonGenerator<CEventAction>(self)
{
	m_pCurrentEventAction = rhs.m_pCurrentEventAction;
}

CEventAction* CEventActionGenerator::getNext()
{
	if (!m_pCurrentEventAction)
		return NULL;

	CEventAction* result = m_pCurrentEventAction;
	m_pCurrentEventAction = m_pCurrentEventAction->m_pNext;
	return result;
}
