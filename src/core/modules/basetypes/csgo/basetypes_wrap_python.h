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

#ifndef _BASETYPES_WRAP_PYTHON_CSGO_H
#define _BASETYPES_WRAP_PYTHON_CSGO_H

//-----------------------------------------------------------------------------
// Includes.
//-----------------------------------------------------------------------------
#include "dt_common.h"
#include "dt_send.h"
#include "basetypes_wrap.h"


//-----------------------------------------------------------------------------
// Expose ServerClass.
//-----------------------------------------------------------------------------
template<class T>
void export_engine_specific_server_class(T ServerClass_)
{
	ServerClass_.add_property("name", &ServerClassExt::get_name);
}


//-----------------------------------------------------------------------------
// Expose SendProp.
//-----------------------------------------------------------------------------
template<class T>
void export_engine_specific_send_prop(T SendProp_)
{
	SendProp_.add_property("priority", &SendProp::GetPriority);
	SendProp_.def("get_priority", &SendProp::GetPriority);
}


//-----------------------------------------------------------------------------
// Expose SendPropType.
//-----------------------------------------------------------------------------
template<class T>
void export_engine_specific_send_prop_types(T SendPropTypes)
{
	SendPropTypes.value("INT64", DPT_Int64);
}


//-----------------------------------------------------------------------------
// Expose SendPropVariant.
//-----------------------------------------------------------------------------
template<class T>
void export_engine_specific_send_prop_variant(T SendPropVariant)
{
	SendPropVariant.def("get_int64", &SendPropVariantExt::get_typed_value<DPT_Int64, int64, &DVariant::m_Int64>);
	SendPropVariant.def("get_int64", &SendPropVariantExt::get_typed_value<DPT_Int64, int64, &DVariant::m_Int64>);
}


//-----------------------------------------------------------------------------
// Expose CTakeDamageInfo.
//-----------------------------------------------------------------------------
template<class T>
void export_engine_specific_take_damage_info(T TakeDamageInfo)
{
	TakeDamageInfo.add_property("radius", &CTakeDamageInfo::GetRadius, &CTakeDamageInfo::SetRadius);
	TakeDamageInfo.add_property("penetrated", &TakeDamageInfoExt::get_penetrated, &TakeDamageInfoExt::set_penetrated);
	TakeDamageInfo.add_property("bullet", &TakeDamageInfoExt::get_bullet, &TakeDamageInfoExt::set_bullet);
	TakeDamageInfo.add_property("recoil", &TakeDamageInfoExt::get_recoil, &TakeDamageInfoExt::set_recoil);
}


#endif // _BASETYPES_WRAP_PYTHON_CSGO_H
