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

// This code is taken from here:
// https://github.com/bcserv/masterhook.ext
// It's included in multiple project, so I'm not sure what's the original source.

#ifndef _MEMORY_RTTI_H
#define _MEMORY_RTTI_H


#include <typeinfo>
#include <stddef.h>
#include <cstring>

class IBaseType
{
public:
	/* Offset from the original IType* */
	virtual ptrdiff_t GetOffset() =0;

	/* Gets the basic type info for this type */
	virtual const std::type_info &GetTypeInfo() =0;

	/* Returns the number of types this type inherits from */
	virtual size_t GetNumBaseClasses() =0;

	/* Gets the first base class */
	virtual IBaseType *GetBaseClass(size_t num) =0;
	
	/* Returns the classname for a given type - Removes platform specific formatting */
	virtual const char* GetName() = 0;

	/* Returns true if the type is derived from the given class name. */
	virtual bool IsDerivedFrom(const char* name) = 0;
	virtual bool IsDerivedFrom(IBaseType* pType) = 0;

	/* Dumps the class hierachy. */
	virtual void Dump() = 0;
	virtual void Dump(int level) = 0;
};

/* Get type information for a class pointer */
IBaseType *GetType(const void *ptr);

/* Returns the classname for a given type - Removes platform specific formatting */
const char *GetTypeName(const std::type_info &type);

inline bool IsClassDerivedFrom(IBaseType* pBaseType, const char* name)
{
	for (size_t i = 0; i < pBaseType->GetNumBaseClasses(); i++)
	{
		if (strcmp(GetTypeName(pBaseType->GetTypeInfo()), name) == 0) {
			return true;
		}

		if (IsClassDerivedFrom(pBaseType->GetBaseClass(i), name)) {
			return true;
		}
	}

	return false;
}

#endif // _MEMORY_RTTI_H
