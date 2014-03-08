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
#include "dt_common.h"
#include "dt_send.h"
#include "eiface.h"
#include "server_class.h"
#include "utility/call_python.h"

//-----------------------------------------------------------------------------
// Externals
//-----------------------------------------------------------------------------
extern IServerGameDLL* servergamedll;

//-----------------------------------------------------------------------------
// Dump Server Classes
//-----------------------------------------------------------------------------
void DumpServerClassProps(SendTable* pTable, int iTabLevel)
{
	if (iTabLevel > 16)
	{
		return;
	}

	static const char* szTabs = "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t";

	int iNumProps = pTable->GetNumProps();
	for(int i=0; i < iNumProps; ++i)
	{
		char* szType;
		SendProp* pProp = pTable->GetProp(i);
		switch (pProp->GetType())
		{
		case DPT_Int:
			szType = "int";
			break;
		case DPT_Float:
			szType = "float";
			break;
		case DPT_Vector:
			szType = "vector";
			break;
		case DPT_VectorXY:
			szType = "vectorXY";
			break;
		case DPT_String:
			szType = "string";
			break;
		case DPT_Array:
			szType = "array";
			break;
		case DPT_DataTable:
			szType = "datatable";
			break;
#ifdef ENGINE_CSGO
		case DPT_Int64:
			szType = "int64";
			break;
#endif
		default:
			szType = "**Unknown**";
			break;
		}

		if (pProp->GetType() == DPT_DataTable && strcmp(pProp->GetName(), "baseclass") != 0)
		{
			SendTable* pTable = pProp->GetDataTable();
			int iNumProps = pTable->GetNumProps();
			PythonLog("dump", "%*.*s%s %s (offset %d) [%d properties]:", iTabLevel, iTabLevel, szTabs, szType, pProp->GetName(), pProp->GetOffset(), iNumProps);
			DumpServerClassProps(pProp->GetDataTable(), iTabLevel + 1);
		}
		else
		{
			PythonLog("dump", "%*.*s%s %s (offset %d)", iTabLevel, iTabLevel, szTabs, szType, pProp->GetName(), pProp->GetOffset());
		}
	}
}

void DumpServerClasses()
{
	SendTable* pSendTable = NULL;
	ServerClass* pCurrentServerClass = servergamedll->GetAllServerClasses();
	while (pCurrentServerClass)
	{
		SendTable* pTable = pCurrentServerClass->m_pTable;
		int iNumProps = pTable->GetNumProps();
		PythonLog("dump", "%s %d [%d properties]:", pCurrentServerClass->GetName(), pCurrentServerClass->m_InstanceBaselineIndex, iNumProps);
		DumpServerClassProps(pTable, 1);
		pCurrentServerClass = pCurrentServerClass->m_pNext;
	}
}
