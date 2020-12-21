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

#ifndef _ENTITIES_PROPS_H
#define _ENTITIES_PROPS_H

//-----------------------------------------------------------------------------
// Includes.
//-----------------------------------------------------------------------------
#include "utilities/wrap_macros.h"
#include "string_t.h"
#include "dt_send.h"
#include "game/shared/ehandle.h"
#include "isaverestore.h"


//-----------------------------------------------------------------------------
// typedefs
//-----------------------------------------------------------------------------
BOOST_FUNCTION_TYPEDEF(void (const SendProp*, const void*, const void*, DVariant*, int, int), BoostSendVarProxyFn)
BOOST_FUNCTION_TYPEDEF(void* (const SendProp*, const void*, const void*, CSendProxyRecipients*, int), BoostDataTableProxyFn)
BOOST_FUNCTION_TYPEDEF(int (const void *pStruct, int objectID), BoostArrayLengthProxyFn)


//-----------------------------------------------------------------------------
// Forward declarations
//-----------------------------------------------------------------------------
class CPointer;
class ServerClass;


//-----------------------------------------------------------------------------
// ServerClass extension class.
//-----------------------------------------------------------------------------
class ServerClassExt
{
public:
	static ServerClass* find_server_class(const char* name);
};


//-----------------------------------------------------------------------------
// SendTable extension class.
//-----------------------------------------------------------------------------
class SendTableSharedExt
{
public:
	static SendProp *__getitem__(SendTable *pSendTable, int iIndex);
	static int find_offset(SendTable* pTable, const char* name);
};


//-----------------------------------------------------------------------------
// SendProp extension class.
//-----------------------------------------------------------------------------
class SendPropSharedExt
{
public:
	static BoostSendVarProxyFn get_proxy_function(SendProp *pSendProp);
	static BoostDataTableProxyFn get_data_table_proxy_function(SendProp *pSendProp);
	static BoostArrayLengthProxyFn get_array_length_proxy_function(SendProp *pSendProp);
};


//-----------------------------------------------------------------------------
// DVariant extension class.
//-----------------------------------------------------------------------------
class SendPropVariantExt
{
public:
	// TODO:
	// I guess we can remove the get_typed_value() method and use a more
	// descriptive exception message in the specific methods.
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

	static const char *get_string(DVariant *pVariant);
	static void set_string(DVariant *pVariant, const char *pString);
	static int get_int(DVariant* pVariant);
	static void set_int(DVariant* pVariant, int iValue);
	static CPointer *get_data(DVariant *pVariant);
	static void set_data(DVariant *pVariant, CPointer *pData);
	static Vector *get_vector(DVariant *pVariant);
	static void set_vector(DVariant *pVariant, Vector *pVector);
};


#endif // _ENTITIES_PROPS_H
