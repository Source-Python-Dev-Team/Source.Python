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
#ifndef _SP_UTIL_H
#define _SP_UTIL_H

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "utilities/wrap_macros.h"
#include "boost/python.hpp"

// SDK
#include "strtools.h"
#include "dbg.h"

//-----------------------------------------------------------------------------
// Namespaces to use
//-----------------------------------------------------------------------------
using namespace boost::python;

//-----------------------------------------------------------------------------
// Send a message in chunks
//-----------------------------------------------------------------------------
#define MAX_CHUNK 1024

inline void ChunkedMsg(const char* msg)
{
	char* pMsg = (char*) msg;
	int iLen = strlen(msg);

	while(iLen > 0) {
		Msg(pMsg);
		pMsg += MAX_CHUNK-1;
		iLen -= MAX_CHUNK-1;
	}
}

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


//-----------------------------------------------------------------------------
// Dummy deleter function for shared_ptr that we are not owning.
//-----------------------------------------------------------------------------
template<class T>
void NeverDeleteDeleter(T pSelf)
{
}

namespace sputils {

//-----------------------------------------------------------------------------
// Convert a string into a float array.
// Copied from util_shared.cpp, adapted to return true on success
//-----------------------------------------------------------------------------
inline bool UTIL_StringToFloatArray( float *pVector, int count, const char *pString )
{
	char *pstr, *pfront, tempString[128];
	int	j;

	Q_strncpy( tempString, pString, sizeof(tempString) );
	pstr = pfront = tempString;

	for ( j = 0; j < count; j++ )			// lifted from pr_edict.c
	{
		pVector[j] = atof( pfront );

		// skip any leading whitespace
		while ( *pstr && *pstr <= ' ' )
			pstr++;

		// skip to next whitespace
		while ( *pstr && *pstr > ' ' )
			pstr++;

		if (!*pstr)
			break;

		pstr++;
		pfront = pstr;
	}

	return j == count - 1;
}

//-----------------------------------------------------------------------------
// Convert a string into an int array.
// Copied from util_shared.cpp, adapted to return true on success
//-----------------------------------------------------------------------------
inline bool UTIL_StringToIntArray( int *pVector, int count, const char *pString )
{
	char *pstr, *pfront, tempString[128];
	int	j;

	Q_strncpy( tempString, pString, sizeof(tempString) );
	pstr = pfront = tempString;

	for ( j = 0; j < count; j++ )			// lifted from pr_edict.c
	{
		pVector[j] = atoi( pfront );

		while ( *pstr && *pstr != ' ' )
			pstr++;
		if (!*pstr)
			break;
		pstr++;
		pfront = pstr;
	}

	return j == count - 1;
}

//-----------------------------------------------------------------------------
// Convert the given string into a long. Return true on success.
//-----------------------------------------------------------------------------
inline bool UTIL_StringToLong(long* pOut, const char* szString)
{
	char* p;
	*pOut = strtol(szString, &p, 10);
	return !*p;
}

//-----------------------------------------------------------------------------
// Convert the given string into a double. Return true on success.
//-----------------------------------------------------------------------------
inline bool UTIL_StringToDouble(double* pOut, const char* szString)
{
	char* p;
	*pOut = strtod(szString, &p);
	return !*p;
}

} // namespace sputils

#endif // _SP_UTIL_H
