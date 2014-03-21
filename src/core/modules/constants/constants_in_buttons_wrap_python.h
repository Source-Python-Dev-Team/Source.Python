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

#ifndef _CONSTANTS_IN_BUTTONS_WRAP_PYTHON_H
#define _CONSTANTS_IN_BUTTONS_WRAP_PYTHON_H

//-----------------------------------------------------------------------------
// Includes.
//-----------------------------------------------------------------------------
#include "constants_in_buttons_wrap.h"


//-----------------------------------------------------------------------------
// Forward declarations.
//-----------------------------------------------------------------------------
void export_in_buttons(scope constants_c)
{
	enum_<InButtons> InButtons("InButtons");
	
	// Values...
	InButtons.value("ATTACK", ATTACK);
	InButtons.value("JUMP", JUMP);
	InButtons.value("DUCK", DUCK);
	InButtons.value("FORWARD", FORWARD);
	InButtons.value("BACK", BACK);
	InButtons.value("USE", USE);
	InButtons.value("CANCEL", CANCEL);
	InButtons.value("LEFT", LEFT);
	InButtons.value("RIGHT", RIGHT);
	InButtons.value("MOVELEFT", MOVELEFT);
	InButtons.value("MOVERIGHT", MOVERIGHT);
	InButtons.value("ATTACK2", ATTACK2);
	InButtons.value("RUN", RUN);
	InButtons.value("RELOAD", RELOAD);
	InButtons.value("ALT1", ALT1);
	InButtons.value("ALT2", ALT2);
	InButtons.value("SCORE", SCORE);
	InButtons.value("SPEED", SPEED);
	InButtons.value("WALK", WALK);
	InButtons.value("ZOOM", ZOOM);
	InButtons.value("WEAPON1", WEAPON1);
	InButtons.value("WEAPON2", WEAPON2);
	InButtons.value("BULLRUSH", BULLRUSH);
	InButtons.value("GRENADE1", GRENADE1);
	InButtons.value("GRENADE2", GRENADE2);
	
	// TODO: Engine specific stuff...
}


#endif // _CONSTANTS_IN_BUTTONS_WRAP_PYTHON_H
