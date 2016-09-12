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
#ifndef _SHARED_UTILS_H
#define _SHARED_UTILS_H

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "strtools.h"


//-----------------------------------------------------------------------------
// Generates a symlink without the colon symbol (":").
//
// Thanks to Hedgehog for this!
// http://forums.sourcepython.com/viewtopic.php?p=2967#p2967
//-----------------------------------------------------------------------------
inline void _GenerateSymlink(char* path)
{
	DevMsg(1, MSG_PREFIX "Generating symlink: %s\n", path);
#if __linux__
	int pathLen = V_strlen(path) + 1;
	char symlinkOrigin[pathLen];
	char symlinkPath[pathLen];

	V_strncpy(symlinkOrigin, path, pathLen);
	V_strncpy(symlinkPath, path, pathLen);

	int i = 0;
	bool flag = false;

	for (i = 0; i < pathLen; i++)
	{
		if (path[i] == ':')
		{
			path[i] = '-';
			symlinkPath[i] = '-';
			flag = true;
		}
		if (flag && path[i] == '/')
		{
			break;
		}
	}
	symlinkOrigin[i] = '\0';
	symlinkPath[i] = '\0';

	if (symlink(symlinkOrigin, symlinkPath) != 0 && (errno != EEXIST))
	{
		Msg(MSG_PREFIX "Unable to create a symbolic link (%s).\n", strerror(errno));
		Msg(MSG_PREFIX "Original path: %s\n", symlinkOrigin);
		Msg(MSG_PREFIX "Symbolic link: %s\n", symlinkPath);
	}
#endif
}

inline void GenerateSymlink(char* path)
{
#if __linux__
	while (strchr(path, ':') != NULL)
	{
		_GenerateSymlink(path);
	}
#endif
}

#endif // _SHARED_UTILS_H
