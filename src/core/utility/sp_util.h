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
#ifndef _SP_UTIL_H
#define _SP_UTIL_H

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "eiface.h"
#include "public/game/server/iplayerinfo.h"
#include "basehandle.h"
#include "utility/wrap_macros.h"
#include "boost/python.hpp"

//-----------------------------------------------------------------------------
// Externals
//-----------------------------------------------------------------------------
extern IVEngineServer* engine;
extern CGlobalVars* gpGlobals;
extern IPlayerInfoManager* playerinfomanager;

//-----------------------------------------------------------------------------
// Namespaces to use
//-----------------------------------------------------------------------------
using namespace boost::python;

//-----------------------------------------------------------------------------
// Returns True if the class name of the given object equals the given string.
//-----------------------------------------------------------------------------
inline bool CheckClassname(object obj, char* name)
{
	return strcmp(extract<char *>(obj.attr("__class__").attr("__name__")), name) == 0;
}

//-----------------------------------------------------------------------------
// Helper template methods for __getitem__ and __setitem__
//-----------------------------------------------------------------------------
template<class cls, class return_type, int iMin, int iMax>
return_type GetItemIndexer(cls* self, int iIndex)
{
	if ((iIndex < iMin) || (iIndex > iMax))
		BOOST_RAISE_EXCEPTION(PyExc_IndexError, "Index out of range.");
		
	return (*self)[iIndex];
}

template<class T, class U>
U GetItemIndexer(const T* self, const int i)
{
	return (*self)[i];
}

template<class cls, class value_type, int iMin, int iMax>
void SetItemIndexer(cls* self, int iIndex, value_type value)
{
	if ((iIndex < iMin) || (iIndex > iMax))
		BOOST_RAISE_EXCEPTION(PyExc_IndexError, "Index out of range.");
		
	(*self)[iIndex] = value;
}

template<class T, class U>
void SetItemIndexer(T* self, const int i, const U& value)
{
	(*self)[i] = value;
}

template<class T, class U, int iIndex>
void IndexSetter(T *pSelf, U pValue)
{
	(*pSelf)[iIndex] = pValue;
}

#endif // _SP_UTIL_H
