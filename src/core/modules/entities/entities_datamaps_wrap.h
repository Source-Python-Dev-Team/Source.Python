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

#ifndef _ENTITIES_DATAMAP_WRAP_H
#define _ENTITIES_DATAMAP_WRAP_H

//-----------------------------------------------------------------------------
// Includes.
//-----------------------------------------------------------------------------
#include "modules/memory/memory_tools.h"
#include "edict.h"
#include "isaverestore.h"
#include "datamap.h"
#include "game/server/variant_t.h"
#include "Color.h"
#include "utilities/conversions.h"
#include "tier0/basetypes.h"
#include "utilities/baseentity.h"


//-----------------------------------------------------------------------------
// datamap_t extension class.
//-----------------------------------------------------------------------------
class DataMapSharedExt
{
public:
	static typedescription_t __getitem__(const datamap_t& pDataMap, int iIndex)
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
	static object get_input(const typedescription_t& pTypeDesc)
	{
		if (!(pTypeDesc.flags & FTYPEDESC_INPUT || pTypeDesc.flags & FTYPEDESC_FUNCTIONTABLE))
			BOOST_RAISE_EXCEPTION(PyExc_TypeError, "\"%s\" is not an input.", pTypeDesc.fieldName);

		CPointer *pInputFunction = new CPointer((unsigned long)(void *&)pTypeDesc.inputFunc);

		object oInputFunction = make_function(pTypeDesc.inputFunc);
		PyDict_SetItemString(oInputFunction.ptr()->ob_type->tp_dict, "_ptr", object(ptr(pInputFunction)).ptr());

		return oInputFunction;
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
		return IndexFromBaseHandle(pVariant->Entity());
	}

	static void set_entity(variant_t *pVariant, unsigned int uiEntity)
	{
		pVariant->SetEntity(BaseEntityFromIndex(uiEntity, true));
	}
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

	static int get_activator(const inputdata_t& pInputData)
	{
		return IndexFromBaseEntity(pInputData.pActivator);
	}

	static void set_activator(inputdata_t *pInputData, unsigned int uiActivator)
	{
		pInputData->pActivator = BaseEntityFromIndex(uiActivator, true);
	}

	static int get_caller(const inputdata_t& pInputData)
	{
		return IndexFromBaseEntity(pInputData.pCaller);
	}
	
	static void set_caller(inputdata_t *pInputData, unsigned int uiCaller)
	{
		pInputData->pCaller = BaseEntityFromIndex(uiCaller, true);
	}
};


#endif // _ENTITIES_DATAMAP_WRAP_H
