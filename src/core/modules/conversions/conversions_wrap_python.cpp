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
		IndexFromEdict,
		"Returns the index of the given Edict",
		args("edict")
	);

	def("index_from_basehandle",
		IndexFromBaseHandle,
		"Returns the index of the given BaseEntityHandle",
		args("basehandle")
	);

	def("index_from_inthandle",
		IndexFromIntHandle,
		"Returns the index of the given integer handle",
		args("inthandle")
	);

	def("index_from_pointer",
		IndexFromPointer,
		"Returns the index of the given BaseEntity pointer",
		args("pointer")
	);

	def("index_from_userid",
		IndexFromUserid,
		"Returns the index of the given userid",
		args("userid")
	);

	def("index_from_playerinfo",
		IndexFromPlayerInfo,
		"Returns the index of the given PlayerInfo",
		args("playerinfo")
	);

	def("edict_from_index",
		EdictFromIndex,
		"Returns the Edict of the given index",
		args("index"),
		reference_existing_object_policy()
	);

	def("edict_from_basehandle",
		EdictFromBaseHandle,
		"Returns the Edict of the given BaseEntityHandle",
		args("basehandle"),
		reference_existing_object_policy()
	);

	def("edict_from_inthandle",
		EdictFromIntHandle,
		"Returns the Edict of the given integer handle",
		args("inthandle"),
		reference_existing_object_policy()
	);

	def("edict_from_pointer",
		EdictFromPointer,
		"Returns the Edict of the given BaseEntity pointer",
		args("pointer"),
		reference_existing_object_policy()
	);

	def("edict_from_userid",
		EdictFromUserid,
		"Returns the Edict of the given userid",
		args("userid"),
		reference_existing_object_policy()
	);

	def("edict_from_playerinfo",
		EdictFromPlayerInfo,
		"Returns the Edict of the given PlayerInfo",
		args("playerinfo"),
		reference_existing_object_policy()
	);

	def("basehandle_from_index",
		BaseHandleFromIndex,
		"Returns the BaseEntityHandle of the given index",
		args("index")
	);

	def("basehandle_from_edict",
		BaseHandleFromEdict,
		"Returns the BaseEntityHandle of the given Edict",
		args("edict")
	);

	def("basehandle_from_inthandle",
		BaseHandleFromIntHandle,
		"Returns the BaseEntityHandle of the given integer handle",
		args("inthandle")
	);

	def("basehandle_from_pointer",
		BaseHandleFromPointer,
		"Returns the BaseEntityHandle of the given BaseEntity pointer",
		args("pointer")
	);

	def("basehandle_from_userid",
		BaseHandleFromUserid,
		"Returns the BaseEntityHandle of the given userid",
		args("userid")
	);

	def("basehandle_from_playerinfo",
		BaseHandleFromPlayerInfo,
		"Returns the BaseEntityHandle of the given PlayerInfo",
		args("playerinfo")
	);

	def("inthandle_from_index",
		IntHandleFromIndex,
		"Returns the integer handle of the given index",
		args("index")
	);

	def("inthandle_from_edict",
		IntHandleFromEdict,
		"Returns the integer handle of the given Edict",
		args("edict")
	);

	def("inthandle_from_basehandle",
		IntHandleFromBaseHandle,
		"Returns the integer handle of the given BaseEntityHandle",
		args("basehandle")
	);

	def("inthandle_from_pointer",
		IntHandleFromPointer,
		"Returns the integer handle of the given BaseEntity pointer",
		args("pointer")
	);

	def("inthandle_from_userid",
		IntHandleFromUserid,
		"Returns the integer handle of the given userid",
		args("userid")
	);

	def("inthandle_from_playerinfo",
		IntHandleFromPlayerInfo,
		"Returns the integer handle of the given PlayerInfo",
		args("playerinfo")
	);

	def("pointer_from_index",
		PointerFromIndex,
		"Returns the BaseEntity pointer of the given index",
		args("index"),
		manage_new_object_policy()
	);

	def("pointer_from_edict",
		PointerFromEdict,
		"Returns the BaseEntity pointer of the given Edict",
		args("edict"),
		manage_new_object_policy()
	);

	def("pointer_from_basehandle",
		PointerFromBaseHandle,
		"Returns the BaseEntity pointer of the given BaseEntityHandle",
		args("basehandle"),
		manage_new_object_policy()
	);

	def("pointer_from_inthandle",
		PointerFromIntHandle,
		"Returns the BaseEntity pointer of the given integer handle",
		args("inthandle"),
		manage_new_object_policy()
	);

	def("pointer_from_userid",
		PointerFromUserid,
		"Returns the BaseEntity pointer of the given userid",
		args("userid"),
		manage_new_object_policy()
	);

	def("pointer_from_playerinfo",
		PointerFromPlayerInfo,
		"Returns the BaseEntity pointer of the given PlayerInfo",
		args("playerinfo"),
		manage_new_object_policy()
	);

	def("userid_from_index",
		UseridFromIndex,
		"Returns the userid of the given index",
		args("index")
	);

	def("userid_from_edict",
		UseridFromEdict,
		"Returns the userid of the given Edict",
		args("edict")
	);

	def("userid_from_basehandle",
		UseridFromBaseHandle,
		"Returns the userid of the given BaseEntityHandle",
		args("basehandle")
	);

	def("userid_from_inthandle",
		UseridFromIntHandle,
		"Returns the userid of the given integer handle",
		args("inthandle")
	);

	def("userid_from_pointer",
		UseridFromPointer,
		"Returns the userid of the given BaseEntity pointer",
		args("pointer")
	);

	def("userid_from_playerinfo",
		UseridFromPlayerInfo,
		"Returns the userid of the given PlayerInfo",
		args("playerinfo")
	);

	def("playerinfo_from_index",
		PlayerInfoFromIndex,
		"Returns the PlayerInfo of the given index",
		args("index"),
		reference_existing_object_policy()
	);

	def("playerinfo_from_edict",
		PlayerInfoFromEdict,
		"Returns the PlayerInfo of the given Edict",
		args("edict"),
		reference_existing_object_policy()
	);

	def("playerinfo_from_basehandle",
		PlayerInfoFromBaseHandle,
		"Returns the PlayerInfo of the given BaseEntityHandle",
		args("basehandle"),
		reference_existing_object_policy()
	);

	def("playerinfo_from_inthandle",
		PlayerInfoFromIntHandle,
		"Returns the PlayerInfo of the given integer handle",
		args("inthandle"),
		reference_existing_object_policy()
	);

	def("playerinfo_from_pointer",
		PlayerInfoFromPointer,
		"Returns the PlayerInfo of the given BaseEntity pointer",
		args("pointer"),
		reference_existing_object_policy()
	);

	def("playerinfo_from_userid",
		PlayerInfoFromUserid,
		"Returns the PlayerInfo of the given userid",
		args("userid"),
		reference_existing_object_policy()
	);
}
