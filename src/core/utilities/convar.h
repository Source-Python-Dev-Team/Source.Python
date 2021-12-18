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

#ifndef _UTILITIES_CONVAR_H
#define _UTILITIES_CONVAR_H

//-----------------------------------------------------------------------------
// Includes.
//-----------------------------------------------------------------------------
#ifndef CONVAR_H
	#include "public/tier1/convar.h"
#endif // CONVAR_H

//-----------------------------------------------------------------------------
// Classes.
//-----------------------------------------------------------------------------
class CCvar : public ConCommandBase
{
public:
	int GetFlags()
	{
		return m_nFlags;
	}

	void SetFlags(int nFlags)
	{
		m_nFlags = nFlags;
	}

	void AddFlags(int nFlags)
	{
		m_nFlags |= nFlags;
	}

	void RemoveFlags(int nFlags)
	{
		m_nFlags &= ~nFlags;
	}
};


//-----------------------------------------------------------------------------
// Returns the flags of a ConVar/ConCommand.
//-----------------------------------------------------------------------------
inline int GetConCommandFlags(ConCommandBase *pBase)
{
	return reinterpret_cast<CCvar *>(pBase)->GetFlags();
}


//-----------------------------------------------------------------------------
// Assigns the flags of a ConVar/ConCommand.
//-----------------------------------------------------------------------------
inline void SetConCommandFlags(ConCommandBase *pBase, int nFlags)
{
	reinterpret_cast<CCvar *>(pBase)->SetFlags(nFlags);
}


//-----------------------------------------------------------------------------
// Adds the given flags to a ConVar/ConCommand.
//-----------------------------------------------------------------------------
inline void AddConCommandFlags(ConCommandBase *pBase, int nFlags)
{
	reinterpret_cast<CCvar *>(pBase)->AddFlags(nFlags);
}


//-----------------------------------------------------------------------------
// Removes the given flags to a ConVar/ConCommand.
//-----------------------------------------------------------------------------
inline void RemoveConCommandFlags(ConCommandBase *pBase, int nFlags)
{
	reinterpret_cast<CCvar *>(pBase)->RemoveFlags(nFlags);
}


//-----------------------------------------------------------------------------
// Returns Source.Python's DLL identifier.
//-----------------------------------------------------------------------------
inline CVarDLLIdentifier_t CVarDLLIdentifier()
{
	static CVarDLLIdentifier_t s_nDLLIdentifier = ConCommandBase().GetDLLIdentifier();
	return s_nDLLIdentifier;
}


#endif // _UTILITIES_CONVAR_H
