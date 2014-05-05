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

#ifndef _DATAMAP_WRAP_H
#define _DATAMAP_WRAP_H

//-----------------------------------------------------------------------------
// Includes.
//-----------------------------------------------------------------------------
#include "modules/memory/memory_tools.h"
#include "edict.h"
#include "isaverestore.h"
#include "datamap.h"
#include "game/server/variant_t.h"
#include "Color.h"
#include "modules/conversions/conversions_wrap.h"


//-----------------------------------------------------------------------------
// datamap_t extension class.
//-----------------------------------------------------------------------------
class DataMapSharedExt
{
public:
	static typedescription_t __getitem__(datamap_t pDataMap, int iIndex)
	{
		if (iIndex < 0 || iIndex > (pDataMap.dataNumFields - 1))
		{
			BOOST_RAISE_EXCEPTION(PyExc_IndexError, "Invalid index.");
		}
		return pDataMap.dataDesc[iIndex];
	}
	
	static typedescription_t *find(datamap_t *pDataMap, const char *szName)
	{
		while (pDataMap)
		{
			for (int iCurrentIndex=0; iCurrentIndex < pDataMap->dataNumFields; iCurrentIndex++)
			{
				typedescription_t pCurrentDataDesc = pDataMap->dataDesc[iCurrentIndex];
				if ((pCurrentDataDesc.fieldName && strcmp(szName, pCurrentDataDesc.fieldName) == 0) ||
					(pCurrentDataDesc.externalName && strcmp(szName, pCurrentDataDesc.externalName) == 0))
				{
					return &(pDataMap->dataDesc[iCurrentIndex]);
				}
				else if (pCurrentDataDesc.fieldType == FIELD_EMBEDDED)
				{
					typedescription_t *pReturnValue = find(pCurrentDataDesc.td, szName);
					if (pReturnValue)
					{
						return pReturnValue;
					}
				}
			}
			pDataMap = pDataMap->baseMap;
		}
		return NULL;
	}
};


//-----------------------------------------------------------------------------
// typedescription_t extension class.
//-----------------------------------------------------------------------------
class TypeDescriptionSharedExt
{
public:
	enum TypeDescriptionFlags
	{
		GLOBAL = FTYPEDESC_GLOBAL,
		SAVE = FTYPEDESC_SAVE,
		KEY = FTYPEDESC_KEY,
		INPUT = FTYPEDESC_INPUT,
		OUTPUT = FTYPEDESC_OUTPUT,
		FUNCTIONTABLE = FTYPEDESC_FUNCTIONTABLE,
		PTR = FTYPEDESC_PTR,
		OVERRIDE = FTYPEDESC_OVERRIDE,
		INSENDTABLE = FTYPEDESC_INSENDTABLE,
		PRIVATE = FTYPEDESC_PRIVATE,
		NOERRORCHECK = FTYPEDESC_NOERRORCHECK,
		MODELINDEX = FTYPEDESC_MODELINDEX,
		INDEX = FTYPEDESC_INDEX,
		VIEW_OTHER_PLAYER = FTYPEDESC_VIEW_OTHER_PLAYER,
		VIEW_OWN_TEAM = FTYPEDESC_VIEW_OWN_TEAM,
		VIEW_NEVER = FTYPEDESC_VIEW_NEVER
	};
	
	static CPointer *get_input(typedescription_t pTypeDesc)
	{
		if (pTypeDesc.flags & INPUT || pTypeDesc.flags & FUNCTIONTABLE)
		{
			// HACK!
			return new CPointer((unsigned long)(void *&)pTypeDesc.inputFunc);
		}
		return NULL;
	}
};


//-----------------------------------------------------------------------------
// varian_t extension class.
//-----------------------------------------------------------------------------
class VariantSharedExt
{
public:
	static const char *get_string(variant_t *pVariant)
	{
		return STRING(pVariant->StringID());
	}
	
	static void set_string(variant_t *pVariant, const char *szValue)
	{
		return pVariant->SetString(MAKE_STRING(szValue));
	}
	
	static Color *get_color(variant_t *pVariant)
	{
		color32 pColor32 = pVariant->Color32();
		return new Color((int)pColor32.r, (int)pColor32.g, (int)pColor32.b, (int)pColor32.a);
	}
	
	static void set_color(variant_t *pVariant, Color *pColor)
	{
		pVariant->SetColor32(pColor->r(), pColor->g(), pColor->b(), pColor->a());
	}
	
	static Vector get_vector(variant_t *pVariant)
	{
		Vector pVector;
		pVariant->Vector3D(pVector);
		return pVector;
	}
	
	static int get_entity(variant_t *pVariant)
	{
		CHandle<CBaseEntity> pHandle = pVariant->Entity();
		if (pHandle.IsValid())
		{
			return pVariant->Entity().GetEntryIndex();
		}
		return -1;
	}
	
	static void set_entity(variant_t *pVariant, unsigned int uiEntity)
	{
		pVariant->SetEntity((CBaseEntity *)PointerFromIndex(uiEntity)->m_ulAddr);
	}
};


//-----------------------------------------------------------------------------
// Declared here to fix undefined symbol issues.
//-----------------------------------------------------------------------------
void variant_t::SetEntity(CBaseEntity *pValue) 
{ 
	eVal = pValue;
	fieldType = FIELD_EHANDLE; 
}


//-----------------------------------------------------------------------------
// Structure passed to input handlers.
//-----------------------------------------------------------------------------
struct inputdata_t
{
	CBaseEntity *pActivator;
	CBaseEntity *pCaller;
	variant_t value;
	int nOutputID;
};


//-----------------------------------------------------------------------------
// inputdata_t extension class.
//-----------------------------------------------------------------------------
class InputDataSharedExt
{
public:
	static inputdata_t *__init__()
	{
		inputdata_t *pInputData = new inputdata_t;
		pInputData->pActivator = NULL;
		pInputData->pCaller = NULL;
		pInputData->nOutputID = 0;
		return pInputData;
	}
	
	static int get_activator(inputdata_t pInputData)
	{
		if (pInputData.pActivator)
		{
			CPointer ptr = CPoiner((unsigned long)pInputData.pActivator);
			return IndexFromPointer(&ptr);
		}
		return -1;
	}
	
	static void set_activator(inputdata_t *pInputData, unsigned int uiActivator)
	{
		pInputData->pActivator = (CBaseEntity *)PointerFromIndex(uiActivator)->m_ulAddr;
	}
	
	static int get_caller(inputdata_t pInputData)
	{
		if (pInputData.pCaller)
		{
			CPointer ptr = CPointer((unsigned long)pInputData.pCaller);
			return IndexFromPointer(&ptr);
		}
		return -1;
	}
	
	static void set_caller(inputdata_t *pInputData, unsigned int uiCaller)
	{
		pInputData->pCaller = (CBaseEntity *)PointerFromIndex(uiCaller)->m_ulAddr;
	}
};


#endif // _DATAMAP_WRAP_H
