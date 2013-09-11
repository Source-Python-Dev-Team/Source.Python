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

//---------------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------------
#include "eiface.h"

#include "../eiface_engine_base.h"

//---------------------------------------------------------------------------------
// Global externs we need.
//---------------------------------------------------------------------------------
extern IVEngineServer * engine;

//---------------------------------------------------------------------------------
// Purpose: Source Engine 1 Specific engine implementation calls
//---------------------------------------------------------------------------------
// Engine 1 does not define bbox_t. There are only forward declarations.
struct bbox_t
{
	Vector mins;
	Vector maxs;
};

// Engine 1 uses CBitVec<ABSOLUTE_PLAYER_LIMIT> all the time as CPlayerBitVec
// is not a define in the OB SDK
class CPlayerBitVecWrapperImplementation : public CBitVec<ABSOLUTE_PLAYER_LIMIT>
{
};

// Single Player shared memory class doesn't exist in OB engine. We only need to 
// forward declare to allow us to stub a NULL ptr into it.
abstract_class ISPSharedMemory {};

class CEngineServerImplementation : public CEngineServerImplementationBase
{
public:
};