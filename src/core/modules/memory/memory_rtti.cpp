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

#include "memory_rtti.h"
#include "utilities/call_python.h"

#include <assert.h>
#include <ctype.h>
#include <new>
#include <stdlib.h>
#include <string>

#ifdef WIN32
struct _s_RTTIBaseClassDescriptor;
typedef _s_RTTIBaseClassDescriptor ClassDescriptor;
#else
class __class_type_info;
typedef __class_type_info ClassDescriptor;
#endif

class CBaseType : public IBaseType
{
public:
	CBaseType(const ClassDescriptor *descriptor, size_t offset);

	~CBaseType()
	{
		for (size_t i=0; i<m_BaseTypes; i++)
		{
			m_Bases[i].~CBaseType();
		}

		free(m_Bases);
	}

	ptrdiff_t GetOffset()
	{
		return m_Offset;
	}

	const std::type_info &GetTypeInfo()
	{
		return *m_TypeInfo;
	}

	size_t GetNumBaseClasses()
	{
		return m_BaseTypes;
	}

	IBaseType *GetBaseClass(size_t num)
	{
		assert(num < GetNumBaseClasses());
		return &m_Bases[num];
	}
	
	const char* GetName()
	{
		return GetTypeName(GetTypeInfo());
	}

	bool IsDerivedFrom(const char* name)
	{
		return IsClassDerivedFrom(this, name);
	}

	bool IsDerivedFrom(IBaseType* pType)
	{
		return IsDerivedFrom(pType->GetName());
	}

	void Dump()
	{
		Dump(0);
	}

	void Dump(int level)
	{
		std::string x = "";
		for (int i=0; i < level; ++i)
			x += "-";

		PythonLog(-1, "%s%s", x.c_str(), GetName());
		for (unsigned int i=0; i < GetNumBaseClasses(); ++i) {
			GetBaseClass(i)->Dump(level + 1);
		}
	}

private:
	ptrdiff_t m_Offset;
	size_t m_BaseTypes;
	CBaseType *m_Bases;
	const std::type_info *m_TypeInfo;
};

/* OS Specific Implementations */

const char *GetTypeName(const std::type_info &type)
{
	const char *temp = type.name();
#ifdef WIN32
	/* WIN32 uses 'class <classname>' */
	while (*temp != ' ')
	{
		temp++;
	}
	return ++temp;
#else
	/* GCC uses '<length><classname>' */
	while (isdigit(*temp))
	{
		temp++;
	}

	return temp;
#endif
}

#ifdef WIN32
#include <windows.h>

using namespace std;

using std::type_info;

typedef std::type_info TypeDescriptor;

struct PMD
{
	ptrdiff_t mdisp; //vftable offset
	ptrdiff_t pdisp; //vbtable offset
	ptrdiff_t vdisp; //vftable offset (for virtual base class)
};

struct _s_RTTICompleteObjectLocator;
struct _s_RTTIClassHierarchyDescriptor;
struct _s_RTTIBaseClassDescriptor;

struct _s_RTTICompleteObjectLocator
{
	DWORD signature;
	DWORD offset; //vftable offset to this
	DWORD cdOffset;
	TypeDescriptor *pTypeDescriptor;
	_s_RTTIClassHierarchyDescriptor *pClassHierarchyDescriptor;
};

struct _s_RTTIClassHierarchyDescriptor
{
	DWORD signature;
	DWORD attributes; //bit 0 multiple inheritance, bit 1 virtual inheritance
	size_t numBaseClasses; //at least 1 (all base classes, including itself)
	_s_RTTIBaseClassDescriptor **pBaseClassArray;
};

struct _s_RTTIBaseClassDescriptor
{
	TypeDescriptor *pTypeDescriptor;
	size_t numBaseClasses; //direct base classes
	PMD pmd; //Len=0xC
	DWORD attributes;
	_s_RTTIClassHierarchyDescriptor *pClassHierarchyDescriptor; //of this base class
};

template <typename R> // only ptrdiff_t and size_t allowed
R ReadData(size_t ptr)
{
	return *(( const R *)ptr);
}

template < typename R, typename T>
R ReadData( const T *ptr)
{
	return ReadData<R>((size_t)ptr);
}

//We assume the vftable is at offset 0
//If it is not the case, we assume the vbtable should be at offset 0, and with its first entry equal to 0 (point to itself).
//Then its second entry should contain the offset of vftable
template < typename T>
_s_RTTICompleteObjectLocator *GetCompleteObjectLocator( const T *ptr) //not have vbtable
{
	ptrdiff_t offset=0;
	if (ReadData<ptrdiff_t>(ReadData<size_t>(ptr))==0) offset=ReadData<ptrdiff_t>(ReadData<size_t>(ptr)+4);
	return (_s_RTTICompleteObjectLocator *)(ReadData<size_t>(ReadData<size_t>((size_t)ptr+offset)-4));
}

CBaseType::CBaseType(const ClassDescriptor *descriptor, size_t offset)
{
	m_BaseTypes = 0;
	int basecount = descriptor->numBaseClasses;
	int current = 1;
	while (current <= basecount)
	{
		_s_RTTIBaseClassDescriptor *temp = descriptor->pClassHierarchyDescriptor->pBaseClassArray[current];
		current+= temp->numBaseClasses;
		current++;
		m_BaseTypes++;
	}

	m_Offset = descriptor->pmd.mdisp + offset;
	m_TypeInfo = descriptor->pTypeDescriptor;
	m_Bases = (CBaseType *)malloc(sizeof(CBaseType) * m_BaseTypes);

	current = 1;
	int count = 0;
	while (current <= basecount)
	{
		_s_RTTIBaseClassDescriptor *temp = descriptor->pClassHierarchyDescriptor->pBaseClassArray[current];
		current+= temp->numBaseClasses;
		current++;
		new (&m_Bases[count]) CBaseType(temp, m_Offset);
		count++;
	}
}

#else

class __class_type_info;

/* helper class for __vmi_class_type */
class __base_class_info
{
	/* abi defined member variables */
public:
	const __class_type_info *__base;    /* base class type */
	long __offset_flags;            /* offset and info */

	/* implementation defined types */
public:
	enum __offset_flags_masks {
		__virtual_mask = 0x1,
		__public_mask = 0x2,
		hwm_bit = 2,
		offset_shift = 8          /* bits to shift offset by */
	};

	/* implementation defined member functions */
public:
	bool __is_virtual_p () const
	{
		return __offset_flags & __virtual_mask;
	}

	bool __is_public_p () const
	{
		return __offset_flags & __public_mask;
	}

	__PTRDIFF_TYPE__ __offset () const
	{
		// This shift, being of a signed type, is implementation defined. GCC
		// implements such shifts as arithmetic, which is what we want.
		return static_cast<__PTRDIFF_TYPE__> (__offset_flags) >> offset_shift;
	}
};

class __class_type_info
	: public std::type_info
{
};

class __si_class_type_info
	: public __class_type_info
{
public:
	const __class_type_info *__base_type;
};

class __vmi_class_type_info : public __class_type_info
{
	/* abi defined member variables */
public:
	unsigned int __flags;         /* details about the class hierarchy */
	unsigned int __base_count;    /* number of direct bases */
	__base_class_info const __base_info[1]; /* array of bases */
};


struct vtable_prefix {
	ptrdiff_t whole_object;           // offset to most derived object
	const __class_type_info *whole_type;  // pointer to most derived type_info
	const void *origin;               // what a class's vptr points to
};

template <typename T>
inline const T *
adjust_pointer (const void *base, ptrdiff_t offset)
{
	return reinterpret_cast <const T *>
		(reinterpret_cast <const char *> (base) + offset);
}

const __class_type_info *typeid2(const void *ptr)
{
	const void *pVtable = *(const void **)(ptr);
	const vtable_prefix *prefix = adjust_pointer <vtable_prefix> (pVtable, -offsetof (vtable_prefix, origin));
	return prefix->whole_type;
}

enum InheritanceType
{
	Inheritance_None, /* Doubles as unknown */
	Inheritance_Single,
	Inheritance_VirtualMultiple,
};

InheritanceType GetInheritanceType(const __class_type_info *ptr)
{
	const __class_type_info *typeinfo = typeid2(ptr);

	if (strcmp(typeinfo->name(), "N10__cxxabiv120__si_class_type_infoE") == 0)
	{
		return Inheritance_Single;
	}
	else if (strcmp(typeinfo->name(), "N10__cxxabiv121__vmi_class_type_infoE") == 0)
	{
		return Inheritance_VirtualMultiple;
	}

	return Inheritance_None;
}

CBaseType::CBaseType(const ClassDescriptor *descriptor, size_t offset)
{
	InheritanceType type = GetInheritanceType(descriptor);

	switch (type)
	{
	case Inheritance_Single:
		{
			m_BaseTypes = 1;
			break;
		}
	case Inheritance_VirtualMultiple:
		{
			__vmi_class_type_info *vmiObject = (__vmi_class_type_info *)descriptor;
			m_BaseTypes = vmiObject->__base_count;
			break;
		}
	default:
		{
			m_BaseTypes = 0;
		}
	}

	m_Offset = offset;
	m_TypeInfo = descriptor;

	if (type == Inheritance_None)
	{
		m_Bases = NULL;
		return;
	}

	m_Bases = (CBaseType *)malloc(sizeof(CBaseType) * m_BaseTypes);

	if (type == Inheritance_Single)
	{
		__si_class_type_info *siObject = (__si_class_type_info *)descriptor;
		new (m_Bases) CBaseType(siObject->__base_type, offset);
		return;
	}

	__vmi_class_type_info *vmiObject = (__vmi_class_type_info *)descriptor;

	for (size_t i=0; i<m_BaseTypes; i++)
	{
		new (&m_Bases[i]) CBaseType(vmiObject->__base_info[i].__base, offset + vmiObject->__base_info[i].__offset());
	}
}
#endif

IBaseType *GetType(const void *ptr)
{
#ifdef _WIN32
	_s_RTTICompleteObjectLocator *locator = GetCompleteObjectLocator(ptr);
	return new CBaseType(locator->pClassHierarchyDescriptor->pBaseClassArray[0], 0);
#elif __linux__
	return new CBaseType(typeid2(ptr), 0);
#else
	#error Unsupported platform.
#endif
}
