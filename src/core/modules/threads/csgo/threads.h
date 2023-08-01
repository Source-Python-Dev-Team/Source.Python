/**
* =============================================================================
* Source Python
* Copyright (C) 2012-2023 Source Python Development Team.  All rights reserved.
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

#ifndef _THREADS_CSGO_H
#define _THREADS_CSGO_H

#if defined(_WIN32)
	#include <windows.h>
	#define TY_Unit ms
	#define TY_Hook ThreadSleep
	#define TY_Sleep Sleep
	#define TY_Yield if (!SwitchToThread()) YieldProcessor
#elif defined(__linux__)
	#include <time.h>
	#include <unistd.h>
	#define TY_Unit ns
	extern "C" void ThreadNanoSleep(unsigned ns);
	#define TY_Hook ThreadNanoSleep
	#define TY_Sleep(ns) \
		struct timespec ts = { \
			.tv_sec = 0, \
			.tv_nsec = ns \
		}; \
		nanosleep(&ts, NULL);
	#define TY_Yield ThreadPause
#else
	#define TY_NotImplemented
#endif

#endif // _THREADS_CSGO_H
