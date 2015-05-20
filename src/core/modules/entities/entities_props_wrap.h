/**
* =============================================================================
* Source Python
* Copyright (C) 2015 Source Python Development Team.  All rights reserved.
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

#ifndef _ENTITIES_PROPS_WRAP_H
#define _ENTITIES_PROPS_WRAP_H

//-----------------------------------------------------------------------------
// Includes.
//-----------------------------------------------------------------------------
#include "dt_send.h"
#include "game/shared/ehandle.h"
#include "isaverestore.h"
#include "modules/memory/memory_tools.h"
#include "utilities/conversions.h"


//-----------------------------------------------------------------------------
// SendTable extension class.
//-----------------------------------------------------------------------------
class SendTableSharedExt
{
public:
	static SendProp *__getitem__(SendTable *pSendTable, int iIndex)
	{
		if (iIndex < 0 || iIndex > (pSendTable->m_nProps - 1))
		{
			BOOST_RAISE_EXCEPTION(PyExc_IndexError, "Invalid index.");
		}
		return pSendTable->GetProp(iIndex);
	}
};


//-----------------------------------------------------------------------------
// SendProp extension class.
//-----------------------------------------------------------------------------
class SendPropSharedExt
{
public:
	static CFunction *get_proxy_function(SendProp *pSendProp, object oCallingConvention, object args, object oReturnType)
	{
		if (pSendProp->IsExcludeProp())
			BOOST_RAISE_EXCEPTION(PyExc_TypeError, "%s is excluded.", pSendProp->GetName());

		if (pSendProp->GetType() == DPT_DataTable)
			BOOST_RAISE_EXCEPTION(PyExc_TypeError, "%s is a DataTable.", pSendProp->GetName());

		return new CFunction((unsigned long)pSendProp->GetProxyFn(), oCallingConvention, args, oReturnType);
	}
};


//-----------------------------------------------------------------------------
// DVariant extension class.
//-----------------------------------------------------------------------------
class SendPropVariantExt
{
public:
	template<SendPropType prop_type, typename return_type, return_type DVariant::*member>
	static return_type get_typed_value(DVariant *pVariant)
	{
		if (pVariant->m_Type != prop_type)
		{
			BOOST_RAISE_EXCEPTION(PyExc_TypeError, "Unable to cast to the specified type.");
		}
		return pVariant->*member;
	}

	template<SendPropType prop_type, typename member_type, member_type DVariant::*member>
	static void set_typed_value(DVariant *pVariant, member_type value)
	{
		pVariant->m_Type = prop_type;
		pVariant->*member = value;
	}

	static const char *get_string(DVariant *pVariant)
	{
		if (pVariant->m_Type != DPT_String)
		{
			BOOST_RAISE_EXCEPTION(PyExc_TypeError, "Unable to cast to the specified type.");
		}
		return pVariant->m_pString;
	}

	static void set_string(DVariant *pVariant, const char *pString)
	{
		pVariant->m_Type = DPT_String;
		*(char**)&pVariant->m_pString = (char*)pString;
	}

	static CPointer *get_data(DVariant *pVariant)
	{
		return new CPointer((unsigned long)get_typed_value<DPT_DataTable, void *, &DVariant::m_pData>(pVariant));
	}

	static void set_data(DVariant *pVariant, CPointer *pData)
	{
		set_typed_value<DPT_DataTable, void *, &DVariant::m_pData>(pVariant, (void *)pData->m_ulAddr);
	}

	static Vector *get_vector(DVariant *pVariant)
	{
		if (pVariant->m_Type != DPT_Vector)
		{
			BOOST_RAISE_EXCEPTION(PyExc_TypeError, "Unable to cast to the specified type.");
		}
		return new Vector(pVariant->m_Vector[0], pVariant->m_Vector[1], pVariant->m_Vector[2]);
	}

	static void set_vector(DVariant *pVariant, Vector *pVector)
	{
		pVariant->m_Type = DPT_Vector;
		pVariant->m_Vector[0] = pVector->x;
		pVariant->m_Vector[1] = pVector->y;
		pVariant->m_Vector[2] = pVector->z;
	}
};


#endif // _ENTITIES_PROPS_WRAP_H
