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
// C++
#if defined(_WIN32)
	#include <direct.h>
	#define RemoveEmptyDir _rmdir
#else
	#define RemoveEmptyDir rmdir
#endif

// SDK
#include "filesystem.h"
#include "eiface.h"

// Source.Python
#include "updater.h"
#include "definitions.h"


//---------------------------------------------------------------------------------
// Declarations
//---------------------------------------------------------------------------------
extern IFileSystem* filesystem;
extern IVEngineServer* engine;


//---------------------------------------------------------------------------------
// Functions
//---------------------------------------------------------------------------------
void DeleteDirectory(const char* szDirectory)
{
	char szFilePattern[MAX_PATH_LENGTH];
	V_snprintf(szFilePattern, sizeof(szFilePattern), "%s/*", szDirectory);

	FileFindHandle_t findHandle;
	const char *pRelativeFileName = filesystem->FindFirst(szFilePattern, &findHandle);
	while( pRelativeFileName )
	{
		// Skip . and ..
		if (strcmp(pRelativeFileName, ".") == 0 || strcmp(pRelativeFileName, "..") == 0)
		{
			pRelativeFileName = filesystem->FindNext( findHandle );
			continue;
		}

		// Construct an absolute path
		char tmpFile[MAX_PATH_LENGTH];
		V_snprintf(tmpFile, sizeof(tmpFile), "%s/%s", szDirectory, pRelativeFileName);

		if( filesystem->FindIsDirectory( findHandle ) )
		{
			DeleteDirectory(tmpFile);
		}
		else
		{
			DevMsg(5, MSG_PREFIX "Deleting '%s'...\n", tmpFile);
			filesystem->RemoveFile(tmpFile);
		}

		pRelativeFileName = filesystem->FindNext( findHandle );
	}

	filesystem->FindClose(findHandle);
	
	DevMsg(5, MSG_PREFIX "Deleting '%s'...\n", szDirectory);
	RemoveEmptyDir(szDirectory);
}

bool UpdateAvailable()
{
	char szGameDir[MAX_PATH_LENGTH];
	engine->GetGameDir(szGameDir, MAX_PATH_LENGTH);

	char szFilePattern[MAX_PATH_LENGTH];
	V_snprintf(szFilePattern, sizeof(szFilePattern), "%s/%s/*", szGameDir, SP_UPDATE_PATH);

	bool found = false;
	FileFindHandle_t findHandle;
	const char *pRelativeFileName = filesystem->FindFirst(szFilePattern, &findHandle);
	while( pRelativeFileName )
	{
		// Skip . and ..
		if (strcmp(pRelativeFileName, ".") == 0 || strcmp(pRelativeFileName, "..") == 0)
		{
			pRelativeFileName = filesystem->FindNext( findHandle );
			continue;
		}

		found = true;
		break;
	}

	filesystem->FindClose(findHandle);
	return found;
}

void ApplyUpdateStage2()
{
	Msg(MSG_PREFIX "Applying Source.Python update stage 2...\n");

	char szGameDir[MAX_PATH_LENGTH];
	engine->GetGameDir(szGameDir, MAX_PATH_LENGTH);

	// Delete SP package dir
	char szSPPackageDir[MAX_PATH_LENGTH];
	V_snprintf(szSPPackageDir, sizeof(szSPPackageDir), "%s/%s", szGameDir, SP_PACKAGE_PATH);

	DevMsg(1, MSG_PREFIX "Deleting %s...\n", szSPPackageDir);
	DeleteDirectory(szSPPackageDir);

	// Delete SP data dir
	char szSPDataDir[MAX_PATH_LENGTH];
	V_snprintf(szSPDataDir, sizeof(szSPDataDir), "%s/%s", szGameDir, SP_DATA_PATH);

	DevMsg(1, MSG_PREFIX "Deleting %s...\n", szSPDataDir);
	DeleteDirectory(szSPPackageDir);

	// Move files from update dir to real dir
	char szUpdateDir[MAX_PATH_LENGTH];
	V_snprintf(szUpdateDir, sizeof(szUpdateDir), "%s/%s", szGameDir, SP_UPDATE_PATH);

	DevMsg(1, MSG_PREFIX "Merging '%s' into '%s'...\n", szUpdateDir, szGameDir);
	MergeDirectories(szUpdateDir, szGameDir);

	// Delete update dir, because it now contains a bunch of empty directories
	DevMsg(1, MSG_PREFIX "Deleting %s...\n", szUpdateDir);
	DeleteDirectory(szUpdateDir);

	Msg(MSG_PREFIX "Stage 2 has been applied.\n");
}

void MergeDirectories(const char* szSourceDir, const char* szDestDir)
{
	char szFilePattern[MAX_PATH_LENGTH];
	V_snprintf(szFilePattern, sizeof(szFilePattern), "%s/*", szSourceDir);

	FileFindHandle_t findHandle;
	const char *pRelativeFileName = filesystem->FindFirst(szFilePattern, &findHandle);
	while( pRelativeFileName )
	{
		// Skip . and ..
		if (strcmp(pRelativeFileName, ".") == 0 || strcmp(pRelativeFileName, "..") == 0)
		{
			pRelativeFileName = filesystem->FindNext( findHandle );
			continue;
		}

		// Construct an absolute path
		char tmpUpdateFile[MAX_PATH_LENGTH];
		V_snprintf(tmpUpdateFile, sizeof(tmpUpdateFile), "%s/%s", szSourceDir, pRelativeFileName);

		char tmpRealFile[MAX_PATH_LENGTH];
		V_snprintf(tmpRealFile, sizeof(tmpRealFile), "%s/%s", szDestDir, pRelativeFileName);

		if( filesystem->FindIsDirectory( findHandle ) )
		{
			MergeDirectories(tmpUpdateFile, tmpRealFile);
		}
		else
		{
			DevMsg(5, MSG_PREFIX "Moving '%s' to '%s'...\n", tmpUpdateFile, tmpRealFile);
			if (filesystem->FileExists(tmpRealFile))
			{
				filesystem->RemoveFile(tmpRealFile);
			}

			if (!filesystem->RenameFile(tmpUpdateFile, tmpRealFile))
			{
				Msg(MSG_PREFIX "Failed to move file '%s' to '%s'!\n", tmpUpdateFile, tmpRealFile);
			}
		}

		pRelativeFileName = filesystem->FindNext( findHandle );
	}

	filesystem->FindClose(findHandle);
}