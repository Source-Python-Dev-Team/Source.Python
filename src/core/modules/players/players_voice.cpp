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

//-----------------------------------------------------------------------------
// Includes.
//-----------------------------------------------------------------------------
#include "ivoiceserver.h"
#include "export_main.h"
#include "modules/memory/memory_utilities.h"


//-----------------------------------------------------------------------------
// Externals
//-----------------------------------------------------------------------------
extern IVoiceServer* voiceserver;


//-----------------------------------------------------------------------------
// Forward declarations.
//-----------------------------------------------------------------------------
void export_voice_server(scope);


//-----------------------------------------------------------------------------
// Declare the _players._voice module.
//-----------------------------------------------------------------------------
DECLARE_SP_SUBMODULE(_players, _voice)
{
	export_voice_server(_voice);
}


//-----------------------------------------------------------------------------
// Exports IVoiceServer.
//-----------------------------------------------------------------------------
void export_voice_server(scope _voice)
{
	class_<IVoiceServer, IVoiceServer*, boost::noncopyable> _VoiceServer("_VoiceServer", no_init);

	_VoiceServer.def("get_client_listening", &IVoiceServer::GetClientListening);
	_VoiceServer.def("set_client_listening", &IVoiceServer::SetClientListening);
	_VoiceServer.def("set_client_proximity", &IVoiceServer::SetClientProximity);

	_VoiceServer ADD_MEM_TOOLS(IVoiceServer);

	_voice.attr("voice_server") = object(ptr(voiceserver));

	BEGIN_CLASS_INFO(IVoiceServer)
		FUNCTION_INFO(GetClientListening)
		FUNCTION_INFO(SetClientListening)
		FUNCTION_INFO(SetClientProximity)
	END_CLASS_INFO()
}