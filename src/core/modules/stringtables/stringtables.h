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

#ifndef _STRINGTABLES_H
#define _STRINGTABLES_H

//-----------------------------------------------------------------------------
// Includes.
//-----------------------------------------------------------------------------
#include "utilities/wrap_macros.h"
#include "networkstringtabledefs.h"
#include "eiface.h"


//-----------------------------------------------------------------------------
// INetworkStringTableExt
//-----------------------------------------------------------------------------
class INetworkStringTableExt
{
public:
	static const char* GetString(INetworkStringTable& table, int index);
	static bool __contains__( INetworkStringTable *pTable, const char *string );
	static int AddString( INetworkStringTable *pTable, const char *string, const char *user_data, int length, bool is_server, bool auto_unlock );
	static void SetStringIndexUserData( INetworkStringTable *pTable, int string_index, const char *user_data, int length );
	static void SetStringUserData( INetworkStringTable *pTable, const char *string, const char *user_data, int length );
	static const char *GetStringIndexUserData( INetworkStringTable *pTable, int string_index );
	static const char *GetStringUserData( INetworkStringTable *pTable, const char *string );
	static int GetStringIndexUserDataLength( INetworkStringTable *pTable, int string_index );
	static int GetStringUserDataLength( INetworkStringTable *pTable, const char *string );
};

//---------------------------------------------------------------------------------
// INetworkStringTableContainerExt
//---------------------------------------------------------------------------------
class INetworkStringTableContainerExt
{
public:
	static INetworkStringTable* GetTable(INetworkStringTableContainer& table_container, TABLEID table_id);
};

#endif // _STRINGTABLES_H
