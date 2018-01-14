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
#include "engine/IEngineSound.h"


//---------------------------------------------------------------------------------
// External variables.
//---------------------------------------------------------------------------------
extern IEngineSound* enginesound;


//---------------------------------------------------------------------------------
// Forward declarations.
//---------------------------------------------------------------------------------
void export_engine_sound(scope);
void export_channel(scope);
void export_global_sound_variables(scope);
void export_attenuation(scope);
void export_sound_flags_t(scope);
void export_pitch(scope);


//---------------------------------------------------------------------------------
// Declare the _sound module.
//---------------------------------------------------------------------------------
DECLARE_SP_SUBMODULE(_engines, _sound)
{
	export_engine_sound(_sound);
	export_channel(_sound);
	export_global_sound_variables(_sound);
	export_attenuation(_sound);
	export_sound_flags_t(_sound);
	export_pitch(_sound);
}


//---------------------------------------------------------------------------------
// Exports IEngineSound.
//---------------------------------------------------------------------------------
void export_engine_sound(scope _sound)
{
	// Call engine specific implementation function
	IEngineSound_Visitor(

	class_<IEngineSound, boost::noncopyable>("_EngineSound", no_init)

		.def("precache_sound",
			&IEngineSound::PrecacheSound,
			("sample", arg("preload")=false, arg("is_ui_sound")=false),
			"Precaches a particular sample."
		)

		.def("is_sound_precached",
			&IEngineSound::IsSoundPrecached,
			args("sample"),
			"Returns True if the given sound is precached."
		)

		.def("prefetch_sound",
			&IEngineSound::PrefetchSound,
			args("sample"),
			"Prefetches a sample."
		)

		.def("emit_sound",
			&IEngineSoundExt::EmitSound,
			("filter", "entity_index", "channel", "sample",
				"volume", "attenuation",
				arg("flags")=0, arg("pitch")=PITCH_NORM, arg("origin")=object(), arg("direction")=object(),
				arg("origins")=tuple(), arg("update_positions")=true, arg("sound_time")=0.0f, arg("speaker_entity")=INVALID_ENTITY_INDEX),
			"Emits a sound from an entity."
		)

		// TODO: Can we use IVEngineServer::SentenceNameFromIndex() and then call IEngineSound::EmitSound()?
		.NOT_IMPLEMENTED("emit_sentence_by_index")

		.def("stop_sound",
			&IEngineSoundExt::StopSound,
			(arg("entity_index"), arg("channel"), arg("sample")),
			"Stops a sound from being emitted from an entity."
		)

		// TODO: StopAllSounds, SetRoomType, SetPlayerDSP. Are they really just client only?

		.def("get_dist_gain_from_sound_level",
			&IEngineSound::GetDistGainFromSoundLevel,
			args("sound_level", "distance"),
			"Returns the distance gain value from a sound level"
		)

		// Only available for CS:GO
		.NOT_IMPLEMENTED("is_looping_sound")

	) ADD_MEM_TOOLS(IEngineSound); // IEngineSound_Visitor

	_sound.attr("engine_sound") = object(ptr(enginesound));
}


//---------------------------------------------------------------------------------
// Exports Channel.
//---------------------------------------------------------------------------------
void export_channel(scope _sound)
{
	enum_<int>("Channel")
		.value("REPLACE", CHAN_REPLACE)
		.value("AUTO", CHAN_AUTO)
		.value("WEAPON", CHAN_WEAPON)
		.value("VOICE", CHAN_VOICE)
		.value("ITEM", CHAN_ITEM)
		.value("BODY", CHAN_BODY)
		.value("STREAM", CHAN_STREAM)
		.value("STATIC", CHAN_STATIC)
		.value("VOICE_BASE", CHAN_VOICE_BASE)
		.value("USER_BASE", CHAN_USER_BASE)
	;
}


//---------------------------------------------------------------------------------
// Exports volume values.
//---------------------------------------------------------------------------------
void export_global_sound_variables(scope _sound)
{
	// Common volume values
	_sound.attr("VOL_NORM") = VOL_NORM;

	_sound.attr("SOUND_FROM_LOCAL_PLAYER") = SOUND_FROM_LOCAL_PLAYER;
	_sound.attr("SOUND_FROM_WORLD") = SOUND_FROM_WORLD;
}


//---------------------------------------------------------------------------------
// Exports attenuation values.
//---------------------------------------------------------------------------------
void export_attenuation(scope _sound)
{
	_sound.attr("ATTN_NONE") = ATTN_NONE;
	_sound.attr("ATTN_NORM") = ATTN_NORM;
	_sound.attr("ATTN_IDLE") = ATTN_IDLE;
	_sound.attr("ATTN_STATIC") = ATTN_STATIC;
	_sound.attr("ATTN_RICOCHET") = ATTN_RICOCHET;
	_sound.attr("ATTN_GUNFIRE") = ATTN_GUNFIRE;
	_sound.attr("MAX_ATTENUATION") = MAX_ATTENUATION;
}


//---------------------------------------------------------------------------------
// Exports SoundFlags_t.
//---------------------------------------------------------------------------------
void export_sound_flags_t(scope _sound)
{
	enum_<SoundFlags_t>("SoundFlags")
		.value("NO_FLAGS", SND_NOFLAGS)
		.value("CHANGE_VOL", SND_CHANGE_VOL)
		.value("CHANGE_PITCH", SND_CHANGE_PITCH)
		.value("STOP", SND_STOP)
		.value("SPAWNING", SND_SPAWNING)
		.value("DELAY", SND_DELAY)
		.value("STOP_LOOPING", SND_STOP_LOOPING)
		.value("SPEAKER", SND_SPEAKER)
		.value("SHOULD_PAUSE", SND_SHOULDPAUSE)
		.value("IGNORE_PHONEMES", SND_IGNORE_PHONEMES)
		.value("IGNORE_NAME", SND_IGNORE_NAME)
	;
}


//---------------------------------------------------------------------------------
// Exports pitch values.
//---------------------------------------------------------------------------------
void export_pitch(scope _sound)
{
	enum Pitch {};

	enum_<Pitch> _Pitch("Pitch");

	_Pitch.value("NORMAL", (Pitch) PITCH_NORM);
	_Pitch.value("LOW", (Pitch) PITCH_LOW);
	_Pitch.value("HIGH", (Pitch) PITCH_HIGH);
}
