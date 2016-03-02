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

//-----------------------------------------------------------------------------
// Includes.
//-----------------------------------------------------------------------------
#include "export_main.h"
#include "utilities/wrap_macros.h"
#include "entities_datamaps.h"
#include "modules/memory/memory_tools.h"

#include ENGINE_INCLUDE_PATH(entities_datamaps_wrap.h)


//-----------------------------------------------------------------------------
// Forward declarations.
//-----------------------------------------------------------------------------
void export_interval(scope);
void export_datamap(scope);
void export_type_description(scope);
void export_input_data(scope);
void export_variant(scope);
void export_field_types(scope);
void export_type_description_flags(scope);


//-----------------------------------------------------------------------------
// Declare the _datamaps module.
//-----------------------------------------------------------------------------
DECLARE_SP_SUBMODULE(_entities, _datamaps)
{
	export_interval(_datamaps);
	export_datamap(_datamaps);
	export_type_description(_datamaps);
	export_input_data(_datamaps);
	export_variant(_datamaps);
	export_field_types(_datamaps);
	export_type_description_flags(_datamaps);
}


//-----------------------------------------------------------------------------
// Expose interval_t.
//-----------------------------------------------------------------------------
void export_interval(scope _datamaps)
{
	class_<interval_t, interval_t *> Interval("Interval");
	
	// Properties...
	Interval.def_readwrite("start", &interval_t::start);
	Interval.def_readwrite("range", &interval_t::range);
	
	// Add memory tools...
	Interval ADD_MEM_TOOLS(interval_t);
}


//-----------------------------------------------------------------------------
// Expose datamap_t.
//-----------------------------------------------------------------------------
void export_datamap(scope _datamaps)
{
	class_<datamap_t, datamap_t *> DataMap("DataMap", no_init);

	// Properties...
	DataMap.def_readonly("description", &datamap_t::dataDesc);
	DataMap.def_readonly("length", &datamap_t::dataNumFields);
	DataMap.def_readonly("class_name", &datamap_t::dataClassName);
	DataMap.def_readonly("base", &datamap_t::baseMap);

	// CS:GO properties...
	DataMap.NOT_IMPLEMENTED_ATTR("optimized_datamap");

	// OrangeBox properties...
	DataMap.NOT_IMPLEMENTED_ATTR("chains_validated");
	DataMap.NOT_IMPLEMENTED_ATTR("packed_offsets_computed");

	// Special methods...
	DataMap.def("__len__", make_getter(&datamap_t::dataNumFields));
	DataMap.def("__getitem__", &DataMapSharedExt::__getitem__, reference_existing_object_policy());

	// Helper methods...
	DataMap.def("find",
		&DataMapSharedExt::find,
		reference_existing_object_policy()
	);

	// Engine specific stuff...
	export_engine_specific_datamap(_datamaps, DataMap);

	// Add memory tools...
	DataMap ADD_MEM_TOOLS(datamap_t);
}


//-----------------------------------------------------------------------------
// Expose typedescription_t.
//-----------------------------------------------------------------------------
void export_type_description(scope _datamaps)
{
	class_<typedescription_t, typedescription_t *> TypeDescription("TypeDescription", no_init);

	// Properties...
	TypeDescription.add_property("offset", &TypeDescriptionExt::get_offset);
	TypeDescription.def_readonly("type", &typedescription_t::fieldType);
	TypeDescription.def_readonly("name", &typedescription_t::fieldName);
	TypeDescription.def_readonly("size", &typedescription_t::fieldSize);
	TypeDescription.def_readonly("flags", &typedescription_t::flags);
	TypeDescription.def_readonly("external_name", &typedescription_t::externalName);

	// CS:GO properties...
	TypeDescription.NOT_IMPLEMENTED_ATTR("flat_offset");
	TypeDescription.NOT_IMPLEMENTED_ATTR("flat_group");

	// TODO: Expose ISaveRestoreOps...
	TypeDescription.def_readonly("save_restore", &typedescription_t::pSaveRestoreOps);

	TypeDescription.def("get_input_function", &TypeDescriptionSharedExt::get_input_function, manage_new_object_policy());

	TypeDescription.def_readonly("embedded_datamap", &typedescription_t::td);
	TypeDescription.def_readonly("size_in_bytes", &typedescription_t::fieldSizeInBytes);
	TypeDescription.def_readonly("override_field", &typedescription_t::override_field);
	TypeDescription.def_readonly("override_count", &typedescription_t::override_count);
	TypeDescription.def_readonly("tolerance", &typedescription_t::fieldTolerance);

	// Engine specific stuff...
	export_engine_specific_type_description(_datamaps, TypeDescription);

	// Add memory tools...
	TypeDescription ADD_MEM_TOOLS(typedescription_t);
}


//-----------------------------------------------------------------------------
// Expose inputdata_t.
//-----------------------------------------------------------------------------
void export_input_data(scope _datamaps)
{
	class_<inputdata_t, inputdata_t *> InputData("InputData");

	// Initializers...
	InputData.def("__init__", make_constructor(&InputDataSharedExt::__init__));

	// Properties...
	InputData.add_property("activator", &InputDataSharedExt::get_activator, &InputDataSharedExt::set_activator);
	InputData.add_property("caller", &InputDataSharedExt::get_caller, &InputDataSharedExt::set_caller);

	InputData.def_readwrite("value", &inputdata_t::value);
	InputData.def_readwrite("output_index", &inputdata_t::nOutputID);

	// Add memory tools...
	InputData ADD_MEM_TOOLS(inputdata_t);
}


//-----------------------------------------------------------------------------
// Expose variant_t.
//-----------------------------------------------------------------------------
void export_variant(scope _datamaps)
{
	class_<variant_t, variant_t *> Variant("Variant");

	// Properties...
	Variant.add_property("type", &variant_t::FieldType);

	// Getters...
	Variant.def("get_bool", &variant_t::Bool);
	Variant.def("get_string", &VariantSharedExt::get_string);
	Variant.def("get_int", &variant_t::Int);
	Variant.def("get_float", &variant_t::Float);

	Variant.def("get_color",
		&VariantSharedExt::get_color,
		manage_new_object_policy()
	);

	Variant.def("get_vector", &VariantSharedExt::get_vector);
	Variant.def("get_entity", &VariantSharedExt::get_entity);

	// Setters...
	Variant.def("set_bool", &variant_t::SetBool);
	Variant.def("set_string", &VariantSharedExt::set_string);
	Variant.def("set_int", &variant_t::SetInt);
	Variant.def("set_float", &variant_t::SetFloat);
	Variant.def("set_color", &VariantSharedExt::set_color);
	Variant.def("set_vector", &variant_t::SetVector3D);
	Variant.def("set_entity", &VariantSharedExt::set_entity);

	// Add memory tools...
	Variant ADD_MEM_TOOLS(variant_t);
}


//-----------------------------------------------------------------------------
// Expose fieldtype_t.
//-----------------------------------------------------------------------------
void export_field_types(scope _datamaps)
{
	enum_<fieldtype_t> FieldType("FieldType");
	
	// Values...
	FieldType.value("VOID", FIELD_VOID);
	FieldType.value("FLOAT", FIELD_FLOAT);
	FieldType.value("STRING", FIELD_STRING);
	FieldType.value("VECTOR", FIELD_VECTOR);
	FieldType.value("QUATERNION", FIELD_QUATERNION);
	FieldType.value("INTEGER", FIELD_INTEGER);
	FieldType.value("BOOLEAN", FIELD_BOOLEAN);
	FieldType.value("SHORT", FIELD_SHORT);
	FieldType.value("CHARACTER", FIELD_CHARACTER);
	FieldType.value("COLOR32", FIELD_COLOR32);
	FieldType.value("EMBEDDED", FIELD_EMBEDDED);
	FieldType.value("CUSTOM", FIELD_CUSTOM);
	FieldType.value("CLASSPTR", FIELD_CLASSPTR);
	FieldType.value("EHANDLE", FIELD_EHANDLE);
	FieldType.value("EDICT", FIELD_EDICT);
	FieldType.value("POSITION_VECTOR", FIELD_POSITION_VECTOR);
	FieldType.value("TIME", FIELD_TIME);
	FieldType.value("TICK", FIELD_TICK);
	FieldType.value("MODELNAME", FIELD_MODELNAME);
	FieldType.value("SOUNDNAME", FIELD_SOUNDNAME);
	FieldType.value("INPUT", FIELD_INPUT);
	FieldType.value("FUNCTION", FIELD_FUNCTION);
	FieldType.value("VMATRIX", FIELD_VMATRIX);
	FieldType.value("VMATRIX_WORLDSPACE", FIELD_VMATRIX_WORLDSPACE);
	FieldType.value("MATRIX3X4_WORLDSPACE", FIELD_MATRIX3X4_WORLDSPACE);
	FieldType.value("INTERVAL", FIELD_INTERVAL);
	FieldType.value("MODELINDEX", FIELD_MODELINDEX);
	FieldType.value("MATERIALINDEX", FIELD_MATERIALINDEX);
	FieldType.value("VECTOR2D", FIELD_VECTOR2D);
	FieldType.value("TYPECOUNT", FIELD_TYPECOUNT);

	// CS:GO values...
	NOT_IMPLEMENTED_VALUE(fieldtype_t, "INTEGER64");
	NOT_IMPLEMENTED_VALUE(fieldtype_t, "VECTOR4D");

	// Engine specific stuff...
	export_engine_specific_field_types(_datamaps, FieldType);
}


//-----------------------------------------------------------------------------
// Expose flags.
//-----------------------------------------------------------------------------
void export_type_description_flags(scope _datamaps)
{
	_datamaps.attr("FTYPEDESC_GLOBAL") = FTYPEDESC_GLOBAL;
	_datamaps.attr("FTYPEDESC_SAVE") = FTYPEDESC_SAVE;
	_datamaps.attr("FTYPEDESC_KEY") = FTYPEDESC_KEY;
	_datamaps.attr("FTYPEDESC_INPUT") = FTYPEDESC_INPUT;
	_datamaps.attr("FTYPEDESC_OUTPUT") = FTYPEDESC_OUTPUT;
	_datamaps.attr("FTYPEDESC_FUNCTIONTABLE") = FTYPEDESC_FUNCTIONTABLE;
	_datamaps.attr("FTYPEDESC_PTR") = FTYPEDESC_PTR;
	_datamaps.attr("FTYPEDESC_OVERRIDE") = FTYPEDESC_OVERRIDE;
	_datamaps.attr("FTYPEDESC_INSENDTABLE") = FTYPEDESC_INSENDTABLE;
	_datamaps.attr("FTYPEDESC_PRIVATE") = FTYPEDESC_PRIVATE;
	_datamaps.attr("FTYPEDESC_NOERRORCHECK") = FTYPEDESC_NOERRORCHECK;
	_datamaps.attr("FTYPEDESC_MODELINDEX") = FTYPEDESC_MODELINDEX;
	_datamaps.attr("FTYPEDESC_INDEX") = FTYPEDESC_INDEX;
	_datamaps.attr("FTYPEDESC_VIEW_OTHER_PLAYER") = FTYPEDESC_VIEW_OTHER_PLAYER;
	_datamaps.attr("FTYPEDESC_VIEW_OWN_TEAM") = FTYPEDESC_VIEW_OWN_TEAM;
	_datamaps.attr("FTYPEDESC_VIEW_NEVER") = FTYPEDESC_VIEW_NEVER;
}
