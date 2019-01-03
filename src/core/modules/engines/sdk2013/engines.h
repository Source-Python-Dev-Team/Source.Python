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

#ifndef _ENGINES_SDK2013_H
#define _ENGINES_SDK2013_H

//-----------------------------------------------------------------------------
// Includes.
//-----------------------------------------------------------------------------
#include "engine/IEngineSound.h"
#include "eiface.h"


//-----------------------------------------------------------------------------
// IVEngineServer extension class.
//-----------------------------------------------------------------------------
class GameIVEngineServerExt
{
public:
	static bool IsMapValid(IVEngineServer* pServer, const char* map_name)
	{
		int iLength = strlen(map_name) + 10;
		char* szMapName = new char[iLength];
		V_snprintf(szMapName, iLength, "maps/%s.bsp", map_name);
		bool result = pServer->IsMapValid(szMapName);
		delete szMapName;
		return result;
	}
};


//-----------------------------------------------------------------------------
// IEngineSound extension class.
//-----------------------------------------------------------------------------
class IEngineSoundExt
{
public:
	static void EmitSound(IEngineSound* pEngineSound, IRecipientFilter& filter, int iEntIndex, int iChannel, const char *pSample, 
		float flVolume, float flAttenuation, int iFlags, int iPitch, const Vector *pOrigin, const Vector *pDirection,
		tuple origins, bool bUpdatePositions, float soundtime, int speakerentity)
	{
		CUtlVector<Vector> *pUtlVecOrigins = NULL;
		CUtlVector<Vector> vecOrigins;
		if (len(origins) > 0)
		{
			pUtlVecOrigins = &vecOrigins;
			for(int i=0; i < len(origins); i++)
			{
				vecOrigins.AddToTail(extract<Vector>(origins[i]));
			}
		}
		
		pEngineSound->EmitSound(filter, iEntIndex, iChannel, pSample, flVolume, flAttenuation, iFlags, iPitch, 0, pOrigin,
			pDirection, pUtlVecOrigins, bUpdatePositions, soundtime, speakerentity);
	}

	static void StopSound(IEngineSound* pEngineSound, int iEntIndex, int iChannel, const char *pSample)
	{
		pEngineSound->StopSound(iEntIndex, iChannel, pSample);
	}
};


//---------------------------------------------------------------------------------
// IEngineTrace
//---------------------------------------------------------------------------------
inline int GetPointContents(const Vector &vecAbsPosition, IHandleEntity** ppEntity)
{
	return enginetrace->GetPointContents(vecAbsPosition, ppEntity);
}


#endif // _ENGINES_SDK2013_H