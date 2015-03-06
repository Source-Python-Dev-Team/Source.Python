/**
* =============================================================================
* Source Python
* Copyright (C) 2015 Source Python Development Team.  All rights reserved.
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

#ifndef _ENTITIES_PROPS_WRAP_PYTHON_ORANGEBOX_H
#define _ENTITIES_PROPS_WRAP_PYTHON_ORANGEBOX_H

//-----------------------------------------------------------------------------
// Includes.
//-----------------------------------------------------------------------------
#include "dt_send.h"
#include "server_class.h"
#include "entities_props_wrap.h"


//-----------------------------------------------------------------------------
// Expose ServerClass.
//-----------------------------------------------------------------------------
template<class T>
void export_engine_specific_server_class(T ServerClass_)
{
	// Properties...
	ServerClass_.def_readonly("name", &ServerClass::m_pNetworkName);
}


//-----------------------------------------------------------------------------
// Expose SendProp.
//-----------------------------------------------------------------------------
template<class T>
void export_engine_specific_send_prop(T SendProp_)
{
	// Nothing specific to OrangeBox...
}


//-----------------------------------------------------------------------------
// Expose SendPropType.
//-----------------------------------------------------------------------------
template<class T>
void export_engine_specific_send_prop_types(T SendPropTypes)
{
	// Nothing specific to OrangeBox...
}


//-----------------------------------------------------------------------------
// Expose SendPropVariant.
//-----------------------------------------------------------------------------
template<class T>
void export_engine_specific_send_prop_variant(T SendPropVariant)
{
	// Nothing specific to OrangeBox...
}


#endif // _ENTITIES_PROPS_WRAP_PYTHON_ORANGEBOX_H
