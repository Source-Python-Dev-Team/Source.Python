/**
* =============================================================================
* Source Python
* Copyright (C) 2012-2020 Source Python Development Team.  All rights reserved.
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
#include "modules/entities/entities_transmit.h"
#include "modules/memory/memory_tools.h"


//-----------------------------------------------------------------------------
// Forward declarations.
//-----------------------------------------------------------------------------
static void export_transmit_states(scope);
static void export_transmit_target(scope);
static void export_transmit_type(scope);
static void export_base_transmit_criteria(scope);
static void export_base_transmit_filter(scope);
static void export_transmit_manager(scope);


//-----------------------------------------------------------------------------
// Declare the _entities._transmit module.
//-----------------------------------------------------------------------------
DECLARE_SP_SUBMODULE(_entities, _transmit)
{
	export_transmit_states(_transmit);
	export_transmit_target(_transmit);
	export_transmit_type(_transmit);
	export_base_transmit_criteria(_transmit);
	export_base_transmit_filter(_transmit);
	export_transmit_manager(_transmit);
}


//-----------------------------------------------------------------------------
// Exports TransmitStates_t.
//-----------------------------------------------------------------------------
void export_transmit_states(scope _transmit)
{
	class_<TransmitStates_t> TransmitStates("TransmitStates");

	// Special methods...
	TransmitStates.def("__delitem__", GET_METHOD(void, TransmitStates_t, Clear, int));
	TransmitStates.def("__getitem__", &TransmitStates_t::IsBitSet);
	TransmitStates.def("__setitem__", GET_METHOD(void, TransmitStates_t, Set, int, bool));

	// Add memory tools...
	TransmitStates ADD_MEM_TOOLS(TransmitStates_t);
}


//-----------------------------------------------------------------------------
// Exports ETransmitTarget.
//-----------------------------------------------------------------------------
void export_transmit_target(scope _transmit)
{
	enum_<ETransmitTarget> TransmitTarget("TransmitTarget");

	// Values...
	TransmitTarget.value("ENTITY", TRANSMIT_TARGET_ENTITY);
	TransmitTarget.value("PLAYER", TRANSMIT_TARGET_PLAYER);
}


//-----------------------------------------------------------------------------
// Exports ETransmitType.
//-----------------------------------------------------------------------------
void export_transmit_type(scope _transmit)
{
	enum_<ETransmitType> TransmitType("TransmitType");

	// Values...
	TransmitType.value("IN", TRANSMIT_IN);
	TransmitType.value("OUT", TRANSMIT_OUT);
}


//-----------------------------------------------------------------------------
// Exports CBaseTransmitCriteria.
//-----------------------------------------------------------------------------
void export_base_transmit_criteria(scope _transmit)
{
	class_<CBaseTransmitCriteria, CBaseTransmitCriteria *, bases<TransmitStates_t> > BaseTransmitCriteria(
		"BaseTransmitCriteria",
		init<ETransmitTarget>(
			(
				arg("self"), arg("target")=TRANSMIT_TARGET_ENTITY
			)
		)
	);

	// Properties...
	BaseTransmitCriteria.def_readwrite("target", &CBaseTransmitCriteria::m_eTarget);

	// Add memory tools...
	BaseTransmitCriteria ADD_MEM_TOOLS(CBaseTransmitCriteria);
}


//-----------------------------------------------------------------------------
// Exports CBaseTransmitFilter.
//-----------------------------------------------------------------------------
void export_base_transmit_filter(scope _transmit)
{
	class_<CBaseTransmitFilter, CBaseTransmitFilter *> BaseTransmitFilter(
		"BaseTransmitFilter",
		init<ETransmitType, CBaseTransmitCriteria *, object>(
			(
				arg("self"), arg("type")=TRANSMIT_OUT, arg("criteria")=object(), arg("override")=object()
			)
		)
	);

	// Properties...
	BaseTransmitFilter.def_readwrite("type", &CBaseTransmitFilter::m_eType);
	BaseTransmitFilter.def_readwrite("criteria", &CBaseTransmitFilter::m_pCriteria);
	BaseTransmitFilter.add_property("override", &CBaseTransmitFilter::get_override, &CBaseTransmitFilter::set_override);
	BaseTransmitFilter.add_property("callback", &CBaseTransmitFilter::get_callback, &CBaseTransmitFilter::set_callback);

	// Add memory tools...
	BaseTransmitFilter ADD_MEM_TOOLS(CBaseTransmitFilter);
}


//-----------------------------------------------------------------------------
// Exports CTransmitManager.
//-----------------------------------------------------------------------------
void export_transmit_manager(scope _transmit)
{
	class_<CTransmitManager, CTransmitManager *, boost::noncopyable> TransmitManager("TransmitManager", no_init);

	// Methods...
	TransmitManager.def("register_filter", &CTransmitManager::register_filter);
	TransmitManager.def("unregister_filter", &CTransmitManager::unregister_filter);

	// Add memory tools...
	TransmitManager ADD_MEM_TOOLS(CTransmitManager);

	// Singleton...
	_transmit.attr("transmit_manager") = object(ptr(GetTransmitManager()));
}

