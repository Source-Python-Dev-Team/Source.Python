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

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "conversions_wrap.h"
#include "modules/export_main.h"

//-----------------------------------------------------------------------------
// Conversions module definition.
//-----------------------------------------------------------------------------
void export_functions();

DECLARE_SP_MODULE(conversions_c)
{
	export_functions();
}

//-----------------------------------------------------------------------------
// Exports conversion functions.
//-----------------------------------------------------------------------------
void export_functions()
{
	def("index_from_edict",
		index_from_edict,
		"Returns the index of the given Edict"
	);

	def("index_from_basehandle",
		index_from_basehandle,
		"Returns the index of the given BaseEntityHandle"
	);

	def("index_from_inthandle",
		index_from_inthandle,
		"Returns the index of the given integer handle"
	);

	def("index_from_pointer",
		index_from_pointer,
		"Returns the index of the given BaseEntity pointer"
	);

	def("index_from_userid",
		index_from_userid,
		"Returns the index of the given userid"
	);

	def("index_from_playerinfo",
		index_from_playerinfo,
		"Returns the index of the given PlayerInfo"
	);

	def("edict_from_index",
		edict_from_index,
		"Returns the Edict of the given index",
		reference_existing_object_policy()
	);

	def("edict_from_basehandle",
		edict_from_basehandle,
		"Returns the Edict of the given BaseEntityHandle",
		reference_existing_object_policy()
	);

	def("edict_from_inthandle",
		edict_from_inthandle,
		"Returns the Edict of the given integer handle",
		reference_existing_object_policy()
	);

	def("edict_from_pointer",
		edict_from_pointer,
		"Returns the Edict of the given BaseEntity pointer",
		reference_existing_object_policy()
	);

	def("edict_from_userid",
		edict_from_userid,
		"Returns the Edict of the given userid",
		reference_existing_object_policy()
	);

	def("edict_from_playerinfo",
		edict_from_playerinfo,
		"Returns the Edict of the given PlayerInfo",
		reference_existing_object_policy()
	);

	def("basehandle_from_index",
		basehandle_from_index,
		"Returns the BaseEntityHandle of the given index"
	);

	def("basehandle_from_edict",
		basehandle_from_edict,
		"Returns the BaseEntityHandle of the given Edict"
	);

	def("basehandle_from_inthandle",
		basehandle_from_inthandle,
		"Returns the BaseEntityHandle of the given integer handle"
	);

	def("basehandle_from_pointer",
		basehandle_from_pointer,
		"Returns the BaseEntityHandle of the given BaseEntity pointer"
	);

	def("basehandle_from_userid",
		basehandle_from_userid,
		"Returns the BaseEntityHandle of the given userid"
	);

	def("basehandle_from_playerinfo",
		basehandle_from_playerinfo,
		"Returns the BaseEntityHandle of the given PlayerInfo"
	);

	def("inthandle_from_index",
		inthandle_from_index,
		"Returns the integer handle of the given index"
	);

	def("inthandle_from_edict",
		inthandle_from_edict,
		"Returns the integer handle of the given Edict"
	);

	def("inthandle_from_basehandle",
		inthandle_from_basehandle,
		"Returns the integer handle of the given BaseEntityHandle"
	);

	def("inthandle_from_pointer",
		inthandle_from_pointer,
		"Returns the integer handle of the given BaseEntity pointer"
	);

	def("inthandle_from_userid",
		inthandle_from_userid,
		"Returns the integer handle of the given userid"
	);

	def("inthandle_from_playerinfo",
		inthandle_from_playerinfo,
		"Returns the integer handle of the given PlayerInfo"
	);

	def("pointer_from_index",
		pointer_from_index,
		"Returns the BaseEntity pointer of the given index"
	);

	def("pointer_from_edict",
		pointer_from_edict,
		"Returns the BaseEntity pointer of the given Edict"
	);

	def("pointer_from_basehandle",
		pointer_from_basehandle,
		"Returns the BaseEntity pointer of the given BaseEntityHandle"
	);

	def("pointer_from_inthandle",
		pointer_from_inthandle,
		"Returns the BaseEntity pointer of the given integer handle"
	);

	def("pointer_from_userid",
		pointer_from_userid,
		"Returns the BaseEntity pointer of the given userid"
	);

	def("pointer_from_playerinfo",
		pointer_from_playerinfo,
		"Returns the BaseEntity pointer of the given PlayerInfo"
	);

	def("userid_from_index",
		userid_from_index,
		"Returns the userid of the given index"
	);

	def("userid_from_edict",
		userid_from_edict,
		"Returns the userid of the given Edict"
	);

	def("userid_from_basehandle",
		userid_from_basehandle,
		"Returns the userid of the given BaseEntityHandle"
	);

	def("userid_from_inthandle",
		userid_from_inthandle,
		"Returns the userid of the given integer handle"
	);

	def("userid_from_pointer",
		userid_from_pointer,
		"Returns the userid of the given BaseEntity pointer"
	);

	def("userid_from_playerinfo",
		userid_from_playerinfo,
		"Returns the userid of the given PlayerInfo"
	);

	def("playerinfo_from_index",
		playerinfo_from_index,
		"Returns the PlayerInfo of the given index",
		reference_existing_object_policy()
	);

	def("playerinfo_from_edict",
		playerinfo_from_edict,
		"Returns the PlayerInfo of the given Edict",
		reference_existing_object_policy()
	);

	def("playerinfo_from_basehandle",
		playerinfo_from_basehandle,
		"Returns the PlayerInfo of the given BaseEntityHandle",
		reference_existing_object_policy()
	);

	def("playerinfo_from_inthandle",
		playerinfo_from_inthandle,
		"Returns the PlayerInfo of the given integer handle",
		reference_existing_object_policy()
	);

	def("playerinfo_from_pointer",
		playerinfo_from_pointer,
		"Returns the PlayerInfo of the given BaseEntity pointer",
		reference_existing_object_policy()
	);

	def("playerinfo_from_userid",
		playerinfo_from_userid,
		"Returns the PlayerInfo of the given userid",
		reference_existing_object_policy()
	);
}
