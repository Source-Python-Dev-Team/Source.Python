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
#include "utility/wrap_macros.h"
#include "datamaps_wrap.h"
#include "modules/memory/memory_tools.h"

#include ENGINE_INCLUDE_PATH(datamaps_wrap_python.h)


//-----------------------------------------------------------------------------
// Forward declarations.
//-----------------------------------------------------------------------------
void export_datamap();
void export_type_description();
void export_input_data();
void export_variant();
void export_field_types();
void export_type_description_flags();


//-----------------------------------------------------------------------------
// Declare the _datamaps module.
//-----------------------------------------------------------------------------
DECLARE_SP_MODULE(_datamaps)
{
	export_datamap();
	export_type_description();
	export_input_data();
	export_variant();
	export_field_types();
	export_type_description_flags();
}


//-----------------------------------------------------------------------------
// Expose datamap_t.
//-----------------------------------------------------------------------------
void export_datamap()
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
	DataMap.def("__getitem__", &DataMapSharedExt::__getitem__);

	// Helper methods...
	DataMap.def("find",
		&DataMapSharedExt::find,
		reference_existing_object_policy()
	);

	// Engine specific stuff...
	export_engine_specific_datamap(DataMap);

	// Add memory tools...
	DataMap ADD_MEM_TOOLS(datamap_t, "DataMap");
}


//-----------------------------------------------------------------------------
// Expose typedescription_t.
//-----------------------------------------------------------------------------
void export_type_description()
{
	class_<typedescription_t, typedescription_t *> TypeDescription("TypeDescription", no_init);

	// Properties...
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

	TypeDescription.add_property("input",
		make_function(
			&TypeDescriptionSharedExt::get_input,
			manage_new_object_policy()
		)
	);
	
	TypeDescription.def_readonly("embedded_datamap", &typedescription_t::td);
	TypeDescription.def_readonly("size_in_bytes", &typedescription_t::fieldSizeInBytes);
	TypeDescription.def_readonly("override_field", &typedescription_t::override_field);
	TypeDescription.def_readonly("override_count", &typedescription_t::override_count);
	TypeDescription.def_readonly("tolerance", &typedescription_t::fieldTolerance);

	// Engine specific stuff...
	export_engine_specific_type_description(TypeDescription);

	// Add memory tools...
	TypeDescription ADD_MEM_TOOLS(typedescription_t, "TypeDescription");
}


//-----------------------------------------------------------------------------
// Expose inputdata_t.
//-----------------------------------------------------------------------------
void export_input_data()
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
	InputData ADD_MEM_TOOLS(inputdata_t, "InputData");
}


//-----------------------------------------------------------------------------
// Expose variant_t.
//-----------------------------------------------------------------------------
void export_variant()
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
	Variant ADD_MEM_TOOLS(variant_t, "Variant");
}


//-----------------------------------------------------------------------------
// Expose fieldtype_t.
//-----------------------------------------------------------------------------
void export_field_types()
{
	enum_<fieldtype_t> FieldTypes("FieldTypes");
	
	// Values...
	FieldTypes.value("VOID", FIELD_VOID);
	FieldTypes.value("FLOAT", FIELD_FLOAT);
	FieldTypes.value("STRING", FIELD_STRING);
	FieldTypes.value("VECTOR", FIELD_VECTOR);
	FieldTypes.value("QUATERNION", FIELD_QUATERNION);
	FieldTypes.value("INTEGER", FIELD_INTEGER);
	FieldTypes.value("BOOLEAN", FIELD_BOOLEAN);
	FieldTypes.value("SHORT", FIELD_SHORT);
	FieldTypes.value("CHARACTER", FIELD_CHARACTER);
	FieldTypes.value("COLOR32", FIELD_COLOR32);
	FieldTypes.value("EMBEDDED", FIELD_EMBEDDED);
	FieldTypes.value("CUSTOM", FIELD_CUSTOM);
	FieldTypes.value("CLASSPTR", FIELD_CLASSPTR);
	FieldTypes.value("EHANDLE", FIELD_EHANDLE);
	FieldTypes.value("EDICT", FIELD_EDICT);
	FieldTypes.value("POSITION_VECTOR", FIELD_POSITION_VECTOR);
	FieldTypes.value("TIME", FIELD_TIME);
	FieldTypes.value("TICK", FIELD_TICK);
	FieldTypes.value("MODELNAME", FIELD_MODELNAME);
	FieldTypes.value("SOUNDNAME", FIELD_SOUNDNAME);
	FieldTypes.value("INPUT", FIELD_INPUT);
	FieldTypes.value("FUNCTION", FIELD_FUNCTION);
	FieldTypes.value("VMATRIX", FIELD_VMATRIX);
	FieldTypes.value("VMATRIX_WORLDSPACE", FIELD_VMATRIX_WORLDSPACE);
	FieldTypes.value("MATRIX3X4_WORLDSPACE", FIELD_MATRIX3X4_WORLDSPACE);
	FieldTypes.value("INTERVAL", FIELD_INTERVAL);
	FieldTypes.value("MODELINDEX", FIELD_MODELINDEX);
	FieldTypes.value("MATERIALINDEX", FIELD_MATERIALINDEX);
	FieldTypes.value("VECTOR2D", FIELD_VECTOR2D);
	FieldTypes.value("TYPECOUNT", FIELD_TYPECOUNT);

	// CS:GO values...
	NOT_IMPLEMENTED_VALUE(fieldtype_t, "INTEGER64");
	NOT_IMPLEMENTED_VALUE(fieldtype_t, "VECTOR4D");

	// Engine specific stuff...
	export_engine_specific_field_types(FieldTypes);
}

//-----------------------------------------------------------------------------
// Expose flags.
//-----------------------------------------------------------------------------
void export_type_description_flags()
{
	enum_<TypeDescriptionSharedExt::TypeDescriptionFlags> TypeDescriptionFlags("TypeDescriptionFlags");
	
	// Values...
	TypeDescriptionFlags.value("GLOBAL", TypeDescriptionSharedExt::GLOBAL);
	TypeDescriptionFlags.value("SAVE", TypeDescriptionSharedExt::SAVE);
	TypeDescriptionFlags.value("KEY", TypeDescriptionSharedExt::KEY);
	TypeDescriptionFlags.value("INPUT", TypeDescriptionSharedExt::INPUT);
	TypeDescriptionFlags.value("OUTPUT", TypeDescriptionSharedExt::OUTPUT);
	TypeDescriptionFlags.value("FUNCTIONTABLE", TypeDescriptionSharedExt::FUNCTIONTABLE);
	TypeDescriptionFlags.value("PTR", TypeDescriptionSharedExt::PTR);
	TypeDescriptionFlags.value("OVERRIDE", TypeDescriptionSharedExt::OVERRIDE);
	TypeDescriptionFlags.value("INSENDTABLE", TypeDescriptionSharedExt::INSENDTABLE);
	TypeDescriptionFlags.value("PRIVATE", TypeDescriptionSharedExt::PRIVATE);
	TypeDescriptionFlags.value("NOERRORCHECK", TypeDescriptionSharedExt::NOERRORCHECK);
	TypeDescriptionFlags.value("MODELINDEX", TypeDescriptionSharedExt::MODELINDEX);
	TypeDescriptionFlags.value("INDEX", TypeDescriptionSharedExt::INDEX);
	TypeDescriptionFlags.value("VIEW_OTHER_PLAYER", TypeDescriptionSharedExt::VIEW_OTHER_PLAYER);
	TypeDescriptionFlags.value("VIEW_OWN_TEAM", TypeDescriptionSharedExt::VIEW_OWN_TEAM);
	TypeDescriptionFlags.value("VIEW_NEVER", TypeDescriptionSharedExt::VIEW_NEVER);
}
