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

//---------------------------------------------------------------------------------
// Includes.
//---------------------------------------------------------------------------------
// Source.Python
#include "export_main.h"
#include "utilities/conversions.h"
#include "engines.h"
#include ENGINE_INCLUDE_PATH(engines_wrap.h)

// SDK
#include "toolframework/ienginetool.h"
#include "engine/IEngineTrace.h"

//---------------------------------------------------------------------------------
// External variables
//---------------------------------------------------------------------------------
extern IEngineTool* enginetool;

//---------------------------------------------------------------------------------
// Forward declarations.
//---------------------------------------------------------------------------------
void export_engine_tool(scope);

//---------------------------------------------------------------------------------
// Declare the _tool module
//---------------------------------------------------------------------------------
DECLARE_SP_SUBMODULE(_engines, _tool)
{
	export_engine_tool(_tool);
}

//---------------------------------------------------------------------------------
// Exports IEngineTool
//---------------------------------------------------------------------------------
void export_engine_tool(scope _tool)
{
	class_<IEngineTool, boost::noncopyable>("_EngineTool", no_init)
		.def("get_sound_duration_by_path",
			GET_METHOD(float, IEngineTool, GetSoundDuration, const char*),
			"Returns the duration of the given sound sample.",
			args("sampleName")
		)

		.def("get_sound_duration_by_guid",
			GET_METHOD(float, IEngineTool, GetSoundDuration, int),
			"Returns the duration of the sound tied to the given GUID.",
			args("guid")
		)

		.def("is_sound_still_playing",
			&IEngineTool::IsSoundStillPlaying,
			"Returns true if the given sound is still playing",
			args("sound_guid")
		)

		.def("stop_sound_by_guid",
			&IEngineTool::StopSoundByGuid,
			"Stops a sound by GUID.",
			args("guid")
		)

		.def("is_looping_sound",
			&IEngineTool::IsLoopingSound,
			"Returns true if the sound is looping.",
			args("guid")
		)

		.def("reload_sound",
			&IEngineTool::ReloadSound,
			"Reloads the given sound from the disk.",
			args("sound_path")
		)

		.def("stop_all_sounds",
			&IEngineTool::StopAllSounds,
			"Stops all sounds that are currently playing."
		)

		ADD_MEM_TOOLS(IEngineTool)
	;

	_tool.attr("engine_tool") = object(ptr(enginetool));
}