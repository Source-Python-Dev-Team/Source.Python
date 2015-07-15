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
#include "entities_generator_wrap.h"
#include "utilities/sp_util.h"
#include "boost/python/iterator.hpp"
#include "utilities/conversions.h"
#include "toolframework/itoolentity.h"


//-----------------------------------------------------------------------------
// External variables.
//-----------------------------------------------------------------------------
extern IServerTools *servertools;


// ----------------------------------------------------------------------------
// CEntityGenerator Constructor.
// ----------------------------------------------------------------------------
CEntityGenerator::CEntityGenerator( PyObject* self ):
	IPythonGenerator<edict_t>(self),
	m_pCurrentEntity((CBaseEntity *)servertools->FirstEntity()),
	m_szClassName(NULL),
	m_uiClassNameLen(0),
	m_bExactMatch(false)
{
}

// ----------------------------------------------------------------------------
// CEntityGenerator Copy-Constructor.
// ----------------------------------------------------------------------------
CEntityGenerator::CEntityGenerator( PyObject* self, const CEntityGenerator& rhs ):
	IPythonGenerator<edict_t>(self),
	m_pCurrentEntity(rhs.m_pCurrentEntity),
	m_uiClassNameLen(rhs.m_uiClassNameLen),
	m_bExactMatch(rhs.m_bExactMatch)
{
	makeStringCopy(rhs.m_szClassName, m_uiClassNameLen);
}

// ----------------------------------------------------------------------------
// CEntityGenerator Constructor (takes a filter string).
// ----------------------------------------------------------------------------
CEntityGenerator::CEntityGenerator(PyObject* self, const char* szClassName):
	IPythonGenerator<edict_t>(self),
	m_pCurrentEntity((CBaseEntity *)servertools->FirstEntity()),
	m_uiClassNameLen(strlen(szClassName)),
	m_bExactMatch(false)
{
	makeStringCopy(szClassName, m_uiClassNameLen);
}

// ----------------------------------------------------------------------------
// CEntityGenerator Constructor (takes a filter string and a boolean flag).
// ----------------------------------------------------------------------------
CEntityGenerator::CEntityGenerator(PyObject* self, const char* szClassName, bool bExactMatch):
	IPythonGenerator<edict_t>(self),
	m_pCurrentEntity((CBaseEntity *)servertools->FirstEntity()),
	m_uiClassNameLen(strlen(szClassName)),
	m_bExactMatch(bExactMatch)
{
	makeStringCopy(szClassName, m_uiClassNameLen);
}

// ----------------------------------------------------------------------------
// CEntityGenerator Destructor.
// ----------------------------------------------------------------------------
CEntityGenerator::~CEntityGenerator()
{
	delete[] m_szClassName;
}

// ----------------------------------------------------------------------------
// Returns the next valid edict_t instance.
// ----------------------------------------------------------------------------
edict_t* CEntityGenerator::getNext()
{
	while (m_pCurrentEntity)
	{
		edict_t *pEdict = EdictFromBaseEntity(m_pCurrentEntity);
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

//---------------------------------------------------------------------------------
// Private function, creates a copy of the class name string.
//---------------------------------------------------------------------------------
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
