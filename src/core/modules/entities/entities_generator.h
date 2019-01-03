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

#ifndef _ENTITIES_GENERATOR_H
#define _ENTITIES_GENERATOR_H

// ----------------------------------------------------------------------------
// Includes.
// ----------------------------------------------------------------------------
#include "utilities/wrap_macros.h"
#include "utilities/ipythongenerator.h"
#include "utilities/baseentity.h"
#include "entities_entity.h"
#include "eiface.h"
#include "game/server/entityoutput.h"

// ----------------------------------------------------------------------------
// Forward declaration.
// ----------------------------------------------------------------------------
struct edict_t;

// ----------------------------------------------------------------------------
// Declare the generator class.
// ----------------------------------------------------------------------------
class CEntityGenerator: public IPythonGenerator<edict_t>
{
public:
	CEntityGenerator(PyObject* self);
	CEntityGenerator(PyObject* self, const CEntityGenerator& rhs);
	CEntityGenerator(PyObject* self, const char* szClassName);
	CEntityGenerator(PyObject* self, const char* szClassName, bool exactMatch);
	virtual ~CEntityGenerator();

protected:
	virtual edict_t* getNext();

private:
	CBaseEntity *m_pCurrentEntity;
	void makeStringCopy(const char* szClassName, unsigned int uiClassNameLen);
	const char* m_szClassName;
	unsigned int m_uiClassNameLen;
	bool m_bExactMatch;
};

BOOST_SPECIALIZE_HAS_BACK_REFERENCE(CEntityGenerator)


// ----------------------------------------------------------------------------
// CBaseEntityGenerator
// ----------------------------------------------------------------------------
class CBaseEntityGenerator: public IPythonGenerator<CBaseEntityWrapper>
{
public:
	CBaseEntityGenerator(PyObject* self);
	CBaseEntityGenerator(PyObject* self, const CBaseEntityGenerator& rhs);
	CBaseEntityGenerator(PyObject* self, const char* szClassName);
	CBaseEntityGenerator(PyObject* self, const char* szClassName, bool exactMatch);
	virtual ~CBaseEntityGenerator();

protected:
	virtual CBaseEntityWrapper* getNext();

private:
	CBaseEntity* m_pCurrentEntity;
	void makeStringCopy(const char* szClassName, unsigned int uiClassNameLen);
	const char* m_szClassName;
	unsigned int m_uiClassNameLen;
	bool m_bExactMatch;
};

BOOST_SPECIALIZE_HAS_BACK_REFERENCE(CBaseEntityGenerator)


// ----------------------------------------------------------------------------
// CServerClassGenerator
// ----------------------------------------------------------------------------
class CServerClassGenerator: public IPythonGenerator<ServerClass>
{
public:
	CServerClassGenerator(PyObject* self);
	CServerClassGenerator(PyObject* self, const CServerClassGenerator& rhs);
	virtual ~CServerClassGenerator() {}

protected:
	virtual ServerClass* getNext();

private:
	ServerClass* m_pCurrentServerClass;
};

BOOST_SPECIALIZE_HAS_BACK_REFERENCE(CServerClassGenerator)


// ----------------------------------------------------------------------------
// CEventActionGenerator
// ----------------------------------------------------------------------------
class CEventActionGenerator: public IPythonGenerator<CEventAction>
{
public:
	CEventActionGenerator(PyObject* self, CEventAction *pFirstEventAction);
	CEventActionGenerator(PyObject* self, const CEventActionGenerator& rhs);
	virtual ~CEventActionGenerator() {}

protected:
	virtual CEventAction* getNext();

private:
	CEventAction* m_pCurrentEventAction;
};

BOOST_SPECIALIZE_HAS_BACK_REFERENCE(CEventActionGenerator)

#endif // _ENTITIES_GENERATOR_H
