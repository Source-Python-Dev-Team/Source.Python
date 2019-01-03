/**
* =============================================================================
* Source Python
* Copyright (C) 2012-2016 Source Python Development Team.  All rights reserved.
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

// ============================================================================
// >> INCLUDES
// ============================================================================
// Boost
#include "boost/unordered_map.hpp"

// Source.Python
#include "modules/memory/memory_pointer.h"
#include "entities_props.h"
#include ENGINE_INCLUDE_PATH(entities_props.h)


// ============================================================================
// >> TYPEDEFS
// ============================================================================
typedef boost::unordered_map<std::string, int> OffsetsMap;
typedef boost::unordered_map<std::string, OffsetsMap > SendTableMap;


// ============================================================================
// >> GLOBAL VARIABLES
// ============================================================================
SendTableMap g_SendTableCache;


// ============================================================================
// >> HELPER FUNCTIONS
// ============================================================================
SendTable* GetNextSendTable(SendTable* pTable)
{
	for (int i=0; i < pTable->GetNumProps(); ++i)
	{
		SendProp* pProp = pTable->GetProp(i);
		if (strcmp(pProp->GetName(), "baseclass") != 0)
			continue;

		return pProp->GetDataTable();
	}
	return NULL;
}

void AddSendTable(SendTable* pTable, OffsetsMap& offsets, int offset=0, const char* baseName=NULL)
{
	for (int i=0; i < pTable->GetNumProps(); ++i)
	{
		SendProp* pProp = pTable->GetProp(i);
		if (strcmp(pProp->GetName(), "baseclass") == 0)
			continue;

		int currentOffset = offset + pProp->GetOffset();

		char* currentName = NULL;
		if (baseName == NULL) {
			currentName = (char*) pProp->GetName();
		}
		else {
			char tempName[256];
			sprintf(tempName, "%s.%s", baseName, pProp->GetName());
			currentName = strdup(tempName);
		}

		if (pProp->GetType() == DPT_DataTable)
		{
			AddSendTable(pProp->GetDataTable(), offsets, currentOffset, currentName);
		}
		else
		{
			offsets.insert(std::make_pair(currentName, currentOffset));
		}
	}
}


// ============================================================================
// >> SendTableSharedExt
// ============================================================================
SendProp* SendTableSharedExt::__getitem__(SendTable *pSendTable, int iIndex)
{
	if (iIndex < 0 || iIndex > (pSendTable->m_nProps - 1))
	{
		BOOST_RAISE_EXCEPTION(PyExc_IndexError, "Invalid index.");
	}
	return pSendTable->GetProp(iIndex);
}

int SendTableSharedExt::find_offset(SendTable* pTable, const char* name)
{
	while(pTable)
	{
		SendTableMap::iterator offsets = g_SendTableCache.find(pTable->GetName());
		if (offsets == g_SendTableCache.end())
		{
			offsets = g_SendTableCache.insert(std::make_pair(pTable->GetName(), OffsetsMap())).first;
			AddSendTable(pTable, offsets->second);
		}
		
		OffsetsMap::iterator result = offsets->second.find(name);
		if (result == offsets->second.end())
		{
			pTable = GetNextSendTable(pTable);
		}
		else {
			return result->second;
		}
	}
	return -1;
}


// ============================================================================
// >> SendPropSharedExt
// ============================================================================
BoostSendVarProxyFn SendPropSharedExt::get_proxy_function(SendProp *pSendProp)
{
	if (pSendProp->IsExcludeProp())
		BOOST_RAISE_EXCEPTION(PyExc_TypeError, "%s is excluded.", pSendProp->GetName());

	if (pSendProp->GetType() == DPT_DataTable)
		BOOST_RAISE_EXCEPTION(PyExc_TypeError, "%s is a DataTable.", pSendProp->GetName());

	return BoostSendVarProxyFn(pSendProp->GetProxyFn());
}

BoostDataTableProxyFn SendPropSharedExt::get_data_table_proxy_function(SendProp *pSendProp)
{
	if (pSendProp->IsExcludeProp())
		BOOST_RAISE_EXCEPTION(PyExc_TypeError, "%s is excluded.", pSendProp->GetName());

	if (pSendProp->GetType() != DPT_DataTable)
		BOOST_RAISE_EXCEPTION(PyExc_TypeError, "%s is not a DataTable.", pSendProp->GetName());

	return BoostDataTableProxyFn(pSendProp->GetDataTableProxyFn());
}


// ============================================================================
// >> SendPropVariantExt
// ============================================================================
const char* SendPropVariantExt::get_string(DVariant *pVariant)
{
	if (pVariant->m_Type != DPT_String)
	{
		BOOST_RAISE_EXCEPTION(PyExc_TypeError, "Unable to cast to the specified type.");
	}
	return pVariant->m_pString;
}

void SendPropVariantExt::set_string(DVariant *pVariant, const char *pString)
{
	pVariant->m_Type = DPT_String;
	*(char**)&pVariant->m_pString = (char*)pString;
}

int SendPropVariantExt::get_int(DVariant* pVariant)
{
	if (pVariant->m_Type != DPT_Int)
	{
		BOOST_RAISE_EXCEPTION(PyExc_TypeError, "Unable to cast to the specified type.");
	}

	return pVariant->m_Int;
}

void SendPropVariantExt::set_int(DVariant* pVariant, int iValue)
{
	if (pVariant->m_Type != DPT_Int)
	{
		BOOST_RAISE_EXCEPTION(PyExc_TypeError, "Unable to cast to the specified type.");
	}

	pVariant->m_Int = iValue;
}

CPointer* SendPropVariantExt::get_data(DVariant *pVariant)
{
	return new CPointer((unsigned long)get_typed_value<DPT_DataTable, void *, &DVariant::m_pData>(pVariant));
}

void SendPropVariantExt::set_data(DVariant *pVariant, CPointer *pData)
{
	set_typed_value<DPT_DataTable, void *, &DVariant::m_pData>(pVariant, (void *)pData->m_ulAddr);
}

Vector* SendPropVariantExt::get_vector(DVariant *pVariant)
{
	if (pVariant->m_Type != DPT_Vector)
	{
		BOOST_RAISE_EXCEPTION(PyExc_TypeError, "Unable to cast to the specified type.");
	}
	return new Vector(pVariant->m_Vector[0], pVariant->m_Vector[1], pVariant->m_Vector[2]);
}

void SendPropVariantExt::set_vector(DVariant *pVariant, Vector *pVector)
{
	pVariant->m_Type = DPT_Vector;
	pVariant->m_Vector[0] = pVector->x;
	pVariant->m_Vector[1] = pVector->y;
	pVariant->m_Vector[2] = pVector->z;
}
