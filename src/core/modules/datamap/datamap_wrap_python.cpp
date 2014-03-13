/**
* =============================================================================
* Source Python
* Copyright (C) 2014 Source Python Development Team.  All rights reserved.
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

//-----------------------------------------------------------------------------
// Includes.
//-----------------------------------------------------------------------------
#include "modules/export_main.h"
#include "utility/sp_util.h"
#include "datamap.h"
#include "game/server/variant_t.h"
#include "datamap_wrap.h"
#include "modules/memory/memory_tools.h"
#include "modules/conversions/conversions_wrap.h"

#include ENGINE_INCLUDE_PATH(datamap_wrap_python.h)

//-----------------------------------------------------------------------------
// Forward declarations.
//-----------------------------------------------------------------------------
void export_datamap();
void export_typedescription();
void export_inputdata();
void export_variant();
void export_fieldtypes();
void export_typedescription_flags();

//-----------------------------------------------------------------------------
// Declares the datamap_c module.
//-----------------------------------------------------------------------------
DECLARE_SP_MODULE(datamap_c)
{
	export_datamap();
	export_typedescription();
	export_inputdata();
	export_variant();
	export_fieldtypes();
	export_typedescription_flags();
}

//-----------------------------------------------------------------------------
// Expose datamap_t.
//-----------------------------------------------------------------------------
class DataMapSharedExt
{
public:
	static typedescription_t __getitem__(datamap_t pDataMap, int iIndex)
	{
		return pDataMap.dataDesc[iIndex];
	}
};

void export_datamap()
{
	DataMap_Visitor(
	class_<datamap_t, datamap_t *>("DataMap", no_init)

		// Properties...
		.def_readonly("desc", &datamap_t::dataDesc)
		.def_readonly("length", &datamap_t::dataNumFields)
		.def_readonly("classname", &datamap_t::dataClassName)
		.def_readonly("base", &datamap_t::baseMap)

		// Special methods...
		.def("__getitem__", &DataMapSharedExt::__getitem__)
		
	) // DataMap_Visitor...

		// Add memory tools...
		ADD_MEM_TOOLS(datamap_t, "DataMap")
	;
}

//-----------------------------------------------------------------------------
// Expose typedescription_t.
//-----------------------------------------------------------------------------
void export_typedescription()
{
	TypeDesc_Visitor(
	class_<typedescription_t, typedescription_t *>("TypeDesc", no_init)
		
		// Properties...
		.def_readonly("type", &typedescription_t::fieldType)
		.def_readonly("name", &typedescription_t::fieldName)
		.def_readonly("size", &typedescription_t::fieldSize)
		.def_readonly("flags", &typedescription_t::flags)
		.def_readonly("external_name", &typedescription_t::externalName)
		
		// TODO: Expose ISaveRestoreOps...
		.def_readonly("save_restore", &typedescription_t::pSaveRestoreOps)
		
		// TODO: Wrap and return a CFunction...
		.def_readonly("input", &typedescription_t::inputFunc)
		
		.def_readonly("td", &typedescription_t::td)
		.def_readonly("size_in_bytes", &typedescription_t::fieldSizeInBytes)
		.def_readonly("override_field", &typedescription_t::override_field)
		.def_readonly("override_count", &typedescription_t::override_count)
		.def_readonly("tolerance", &typedescription_t::fieldTolerance)
		
	) // TypeDesc_Visitor

		// Add memory tools...
		ADD_MEM_TOOLS(typedescription_t, "TypeDesc")
	;
}

//-----------------------------------------------------------------------------
// Expose inputdata_t.
//-----------------------------------------------------------------------------
void export_inputdata()
{
	class_<inputdata_t, inputdata_t *>("InputData")
		
		// Properties...
		// TODO: Add a CBaseEntity converter and return CPointer instances...
		.def_readonly("activator", &inputdata_t::pActivator)
		.def_readonly("caller", &inputdata_t::pCaller)
		
		.def_readwrite("value", &inputdata_t::value)
		.def_readwrite("output_index", &inputdata_t::nOutputID)
		
		// Add memory tools...
		ADD_MEM_TOOLS(inputdata_t, "InputData")
	;
}

//-----------------------------------------------------------------------------
// Expose variant_t.
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
};

void export_variant()
{
	// TODO: Get/Set Entity, Color and Vector...
	class_<variant_t>("Variant")

		// Properties...
		.add_property("type", &variant_t::FieldType)

		// Getters...
		.def("get_bool", &variant_t::Bool)
		.def("get_string", &VariantSharedExt::get_string)
		.def("get_int", &variant_t::Int)
		.def("get_float", &variant_t::Float)

		// Setters...
		.def("set_bool", &variant_t::SetBool)
		.def("set_string", &VariantSharedExt::set_string)
		.def("set_int", &variant_t::SetInt)
		.def("set_float", &variant_t::SetFloat)

		// Add memory tools...
		ADD_MEM_TOOLS(variant_t, "Variant")
	;
}

//-----------------------------------------------------------------------------
// Expose fieldtype_t.
//-----------------------------------------------------------------------------
void export_fieldtypes()
{
	FieldTypes_Visitor(
	enum_<fieldtype_t>("FieldTypes")
		.value("VOID", FIELD_VOID)
		.value("FLOAT", FIELD_FLOAT)
		.value("STRING", FIELD_STRING)
		.value("VECTOR", FIELD_VECTOR)
		.value("QUATERNION", FIELD_QUATERNION)
		.value("INTEGER", FIELD_INTEGER)
		.value("BOOLEAN", FIELD_BOOLEAN)
		.value("SHORT", FIELD_SHORT)
		.value("CHARACTER", FIELD_CHARACTER)
		.value("COLOR32", FIELD_COLOR32)
		.value("EMBEDDED", FIELD_EMBEDDED)
		.value("CUSTOM", FIELD_CUSTOM)
		.value("CLASSPTR", FIELD_CLASSPTR)
		.value("EHANDLE", FIELD_EHANDLE)
		.value("EDICT", FIELD_EDICT)
		.value("POSITION_VECTOR", FIELD_POSITION_VECTOR)
		.value("TIME", FIELD_TIME)
		.value("TICK", FIELD_TICK)
		.value("MODELNAME", FIELD_MODELNAME)
		.value("SOUNDNAME", FIELD_SOUNDNAME)
		.value("INPUT", FIELD_INPUT)
		.value("FUNCTION", FIELD_FUNCTION)
		.value("VMATRIX", FIELD_VMATRIX)
		.value("VMATRIX_WORLDSPACE", FIELD_VMATRIX_WORLDSPACE)
		.value("MATRIX3X4_WORLDSPACE", FIELD_MATRIX3X4_WORLDSPACE)
		.value("INTERVAL", FIELD_INTERVAL)
		.value("MODELINDEX", FIELD_MODELINDEX)
		.value("VECTOR2D", FIELD_VECTOR2D)
		.value("TYPECOUNT", FIELD_TYPECOUNT)
		
	) // FieldTypes_Visitor
	
	;
}

//-----------------------------------------------------------------------------
// Expose flags.
//-----------------------------------------------------------------------------
void export_typedescription_flags()
{	
	scope().attr("FTYPEDESC_GLOBAL") = FTYPEDESC_GLOBAL;
	scope().attr("FTYPEDESC_SAVE") = FTYPEDESC_SAVE;
	scope().attr("FTYPEDESC_KEY") = FTYPEDESC_KEY;
	scope().attr("FTYPEDESC_INPUT") = FTYPEDESC_INPUT;
	scope().attr("FTYPEDESC_OUTPUT") = FTYPEDESC_OUTPUT;
	scope().attr("FTYPEDESC_FUNCTIONTABLE") = FTYPEDESC_FUNCTIONTABLE;
	scope().attr("FTYPEDESC_PTR") = FTYPEDESC_PTR;
	scope().attr("FTYPEDESC_OVERRIDE") = FTYPEDESC_OVERRIDE;
	scope().attr("FTYPEDESC_INSENDTABLE") = FTYPEDESC_INSENDTABLE;
	scope().attr("FTYPEDESC_PRIVATE") = FTYPEDESC_PRIVATE;
	scope().attr("FTYPEDESC_NOERRORCHECK") = FTYPEDESC_NOERRORCHECK;
	scope().attr("FTYPEDESC_MODELINDEX") = FTYPEDESC_MODELINDEX;
	scope().attr("FTYPEDESC_INDEX") = FTYPEDESC_INDEX;
	scope().attr("FTYPEDESC_VIEW_OTHER_PLAYER") = FTYPEDESC_VIEW_OTHER_PLAYER;
	scope().attr("FTYPEDESC_VIEW_OWN_TEAM") = FTYPEDESC_VIEW_OWN_TEAM;
	scope().attr("FTYPEDESC_VIEW_NEVER") = FTYPEDESC_VIEW_NEVER;
}
