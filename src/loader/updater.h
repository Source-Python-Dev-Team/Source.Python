/**
* =============================================================================
* Source Python
* Copyright (C) 2012-2018 Source Python Development Team.  All rights reserved.
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

#ifndef _UPDATER_H
#define _UPDATER_H

//---------------------------------------------------------------------------------
// Defines
//---------------------------------------------------------------------------------
#define SP_UPDATE_PATH "addons/source-python/update"
#define SP_PACKAGE_PATH "addons/source-python/packages/source-python"
#define SP_DATA_PATH "addons/source-python/data/source-python"
#define SP_DOCS_PATH "addons/source-python/docs/source-python"
#define PYTHON3_PATH "addons/source-python/Python3"


//---------------------------------------------------------------------------------
// Functions
//---------------------------------------------------------------------------------
// Returns true if there is at least one file in the update directory
bool UpdateAvailable();

// Applies stage 2 of the update process
void ApplyUpdateStage2();

#endif // _UPDATER_H
