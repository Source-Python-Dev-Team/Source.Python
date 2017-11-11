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
	DataMap.def_readonly(
		"length", 
		&datamap_t::dataNumFields,
		"Return the number of type descriptions hold by this data map.\n\n"
		":rtype: int");

	DataMap.def_readonly(
		"class_name", 
		&datamap_t::dataClassName,
		"Return the name of the class described by this data map.\n\n"
		":rtype: str");

	DataMap.def_readonly(
		"base", 
		&datamap_t::baseMap,
		"Return the next data map in the class hierarchy.\n\n"
		":rtype: DataMap");

	// CS:GO properties...
	DataMap.NOT_IMPLEMENTED_ATTR("optimized_datamap");

	// OrangeBox properties...
	DataMap.NOT_IMPLEMENTED_ATTR("chains_validated");
	DataMap.NOT_IMPLEMENTED_ATTR("packed_offsets_computed");

	// Special methods...
	DataMap.def(
		"__len__", 
		make_getter(&datamap_t::dataNumFields),
		"Return the number of type descriptions hold by this data map.\n\n"
		"rtype: int");

	DataMap.def(
		"__getitem__", 
		&DataMapSharedExt::__getitem__,
		reference_existing_object_policy(),
		"Return the type description at the given index.\n\n"
		":rtype: TypeDescription");

	// Helper methods...
	DataMap.def("find",
		&DataMapSharedExt::find,
		reference_existing_object_policy(),
		"Find a type description based on its field name or external name.\n\n"
		":rtype: TypeDescription");

	DataMap.def("find_offset",
		&DataMapSharedExt::find_offset,
		args("name"),
		"Return the offset of a type description by searching for its name. Return -1 if the property wasn't found.\n\n"
		":rtype: int"
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
	EXPOSE_FUNCTION_TYPEDEF(BoostInputFn, "InputFn")

	class_<typedescription_t, typedescription_t *> TypeDescription("TypeDescription", no_init);

	// Properties...
	TypeDescription.add_property(
		"offset", 
		&TypeDescriptionExt::get_offset,
		"Return the offset of the field.\n\n"
		":rtype: int");

	TypeDescription.def_readonly(
		"type", 
		&typedescription_t::fieldType,
		"Return the type of the field.\n\n"
		":rtype: FieldType");

	TypeDescription.def_readonly(
		"name",
		&typedescription_t::fieldName,
		"Return the name of the field.\n\n"
		":rtype: str");

	TypeDescription.def_readonly(
		"size", 
		&typedescription_t::fieldSize,
		"Return the size of the field.\n\n"
		":rtype: int");

	TypeDescription.def_readonly(
		"flags",
		&typedescription_t::flags,
		"Return the flags of the field.\n\n"
		":rtype: int");

	TypeDescription.def_readonly(
		"external_name", 
		&typedescription_t::externalName,
		"Return the external name of the field.\n\n"
		":rtype: str");

	TypeDescription.add_property(
		"input_function", 
		&TypeDescriptionSharedExt::get_input_function,
		"Return a callable function if the field is an input function.");

	TypeDescription.add_property(
		"function",
		make_function(&TypeDescriptionSharedExt::get_function, return_by_value_policy()),
		"Return the input function as a pointer.\n\n"
		":rtype: Pointer");

	// CS:GO properties...
	TypeDescription.NOT_IMPLEMENTED_ATTR("flat_offset");
	TypeDescription.NOT_IMPLEMENTED_ATTR("flat_group");

	// TODO: Expose ISaveRestoreOps...
	TypeDescription.def_readonly("save_restore", &typedescription_t::pSaveRestoreOps);

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
	InputData.def("__init__", make_constructor(&InputDataExt::__init__));

	// Properties...
	InputData.def_readwrite(
		"activator",
		&inputdata_t::pActivator,
		"Return the activator.\n\n"
		":rtype: BaseEntity");

	InputData.def_readwrite(
		"caller",
		&inputdata_t::pCaller,
		"Return the caller.\n\n"
		":rtype: BaseEntity");

	InputData.def_readwrite(
		"value",
		&inputdata_t::value,
		"Return the value.\n\n"
		":rtype: Variant");

	InputData.def_readwrite(
		"output_index", 
		&inputdata_t::nOutputID,
		"Return the output index.\n\n"
		":rtype: int");

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
	Variant.add_property(
		"type",
		&variant_t::FieldType,
		"Return the type of the value.\n\n"
		":rtype: FieldType");

	// Getters...
	Variant.def(
		"get_bool", 
		&variant_t::Bool,
		"Return the value as a boolean.\n\n"
		":rtype: bool");

	Variant.def(
		"get_string",
		&VariantExt::get_string,
		"Return the value as a string.\n\n"
		":rtype: str");

	Variant.def(
		"get_int", 
		&variant_t::Int,
		"Return the value as an integer.\n\n"
		":rtype: int");

	Variant.def(
		"get_float", 
		&variant_t::Float,
		"Return the value as a floating value.\n\n"
		":rtype: float");

	Variant.def("get_color",
		&VariantExt::get_color,
		manage_new_object_policy(),
		"Return the value as a color.\n\n"
		":rtype: Color");

	Variant.def(
		"get_vector", 
		&VariantExt::get_vector,
		"Return the value as a vector.\n\n"
		":rtype: Value");

	Variant.def(
		"get_entity", 
		&variant_t::Entity, 
		reference_existing_object_policy(),
		"Return the value as a handle.\n\n"
		":rtype: BaseHandle");

	// Setters...
	Variant.def(
		"set_bool", 
		&variant_t::SetBool,
		"Set the value as a boolean.");

	Variant.def(
		"set_string", 
		&VariantExt::set_string,
		"Set the value as a string.");

	Variant.def(
		"set_int", 
		&variant_t::SetInt,
		"Set the value as an integer.");

	Variant.def(
		"set_float",
		&variant_t::SetFloat,
		"Set the value as a floating value.");

	Variant.def(
		"set_color",
		&VariantExt::set_color,
		"Set the value as a color.");

	Variant.def(
		"set_vector", 
		&variant_t::SetVector3D,
		"Set the value as a Vector.");

	Variant.def(
		"set_entity", 
		&variant_t::SetEntity,
		"Set the value as an entity.");

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
