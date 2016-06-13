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

#ifndef _ENTITIES_DATAMAPS_H
#define _ENTITIES_DATAMAPS_H

//-----------------------------------------------------------------------------
// Includes.
//-----------------------------------------------------------------------------
#include "edict.h"
#include "isaverestore.h"
#include "datamap.h"
#include "game/server/variant_t.h"
#include "Color.h"
#include "tier0/basetypes.h"
#include "utilities/baseentity.h"


//-----------------------------------------------------------------------------
// Forward declarations
//-----------------------------------------------------------------------------
class CFunction;


//-----------------------------------------------------------------------------
// datamap_t extension class.
//-----------------------------------------------------------------------------
class DataMapSharedExt
{
public:
	static typedescription_t& __getitem__(const datamap_t& pDataMap, int iIndex);
	static typedescription_t* find(datamap_t* pDataMap, const char *szName);
	static int find_offset(datamap_t* pDataMap, const char* name);
};


//-----------------------------------------------------------------------------
// typedescription_t extension class.
//-----------------------------------------------------------------------------
class TypeDescriptionSharedExt
{
public:
	static CFunction* get_input_function(const typedescription_t& pTypeDesc, object oCallingConvention, object args, object oReturnType);
};


//-----------------------------------------------------------------------------
// varian_t extension class.
//-----------------------------------------------------------------------------
class VariantSharedExt
{
public:
	static const char* get_string(variant_t *pVariant);
	static void set_string(variant_t *pVariant, const char *szValue);
	static Color* get_color(variant_t *pVariant);
	static void set_color(variant_t *pVariant, Color *pColor);
	static Vector get_vector(variant_t *pVariant);
	static unsigned int get_entity(variant_t *pVariant);
	static void set_entity(variant_t *pVariant, unsigned int uiEntity);
};


//-----------------------------------------------------------------------------
// inputdata_t extension class.
//-----------------------------------------------------------------------------
class InputDataSharedExt
{
public:
	static inputdata_t* __init__();
	static unsigned int get_activator(const inputdata_t& pInputData);
	static void set_activator(inputdata_t *pInputData, unsigned int uiActivator);
	static unsigned int get_caller(const inputdata_t& pInputData);
	static void set_caller(inputdata_t *pInputData, unsigned int uiCaller);
};


#endif // _ENTITIES_DATAMAPS_H
