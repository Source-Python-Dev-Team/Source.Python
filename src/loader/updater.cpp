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

//---------------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------------
// Source.Python
#include "updater.h"
#include "definitions.h"

// Boost
#include "boost/filesystem.hpp"
namespace bfs = boost::filesystem;

// SDK
#include "eiface.h"


//---------------------------------------------------------------------------------
// Declarations
//---------------------------------------------------------------------------------
extern IVEngineServer* engine;


//---------------------------------------------------------------------------------
// Functions
//---------------------------------------------------------------------------------
bool UpdateAvailable()
{
	DevMsg(1, MSG_PREFIX "Checking if update stage 2 can be applied... ");

	char szGameDir[MAX_PATH_LENGTH];
	engine->GetGameDir(szGameDir, MAX_PATH_LENGTH);

	bfs::path updateDir = bfs::path(szGameDir) / SP_UPDATE_PATH;
	bool result = bfs::is_directory(updateDir) && !bfs::is_empty(updateDir);

	DevMsg(1, "%s.\n", result ? "Yes" : "No");
	return result;
}

static void DeleteDir(const bfs::path& dir)
{
	DevMsg(1, MSG_PREFIX "Deleting %s...\n", dir.string().c_str());
	bfs::remove_all(dir);
}

static void MergeDirectories(const bfs::path& src, const bfs::path& dst)
{
	if (bfs::is_directory(src)) {
		bfs::create_directories(dst);

		bfs::directory_iterator end_iter;
		for (bfs::directory_iterator iter(src); iter != end_iter; ++iter)
		{
			MergeDirectories(iter->path(), dst/iter->path().filename());
		}
	} 
	else if (bfs::is_regular_file(src))
	{
		DevMsg(5, MSG_PREFIX "Merging %s into %s...\n", src.string().c_str(), dst.string().c_str());
		bfs::rename(src, dst);
	} 
	else
	{
		Msg(MSG_PREFIX "%s is not a file or directory. Doing nothing...\n", dst.string().c_str());
	}
}

void ApplyUpdateStage2()
{
	Msg(MSG_PREFIX "Applying update stage 2...\n");

	char szGameDir[MAX_PATH_LENGTH];
	engine->GetGameDir(szGameDir, MAX_PATH_LENGTH);

	// Delete old directories
	DeleteDir(bfs::path(szGameDir) / SP_PACKAGE_PATH);
	DeleteDir(bfs::path(szGameDir) / SP_DATA_PATH);
	DeleteDir(bfs::path(szGameDir) / SP_DOCS_PATH);
	DeleteDir(bfs::path(szGameDir) / PYTHON3_PATH);

	// Move files from update dir to real dir
	bfs::path updateDir = bfs::path(szGameDir) / SP_UPDATE_PATH;
	DevMsg(1, MSG_PREFIX "Merging \"%s\" into \"%s\"...\n", updateDir.string().c_str(), szGameDir);
	MergeDirectories(updateDir, szGameDir);

	// Delete update dir, because it now contains a bunch of empty directories
	DeleteDir(updateDir);

	Msg(MSG_PREFIX "Stage 2 has been applied.\n");
}