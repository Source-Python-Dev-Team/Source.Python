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

template<class T>
void IVEngineServer_Visitor(T cls)
{
	/*
    CS:GO:
    // get arbitrary launch options
    virtual KeyValues* GetLaunchOptions( void ) = 0;
    virtual bool                IsUserIDInUse( int userID ) = 0;        // TERROR: used for transitioning
    virtual int                        GetLoadingProgressForUserID( int userID ) = 0;        // TERROR: used for transitioning
    virtual void                SendUserMessage( IRecipientFilter &filter, int message, const google::protobuf::Message &msg );
    virtual bool                IsLogEnabled() = 0;
    // What is the game timescale multiplied with the host_timescale?
    virtual float GetTimescale( void ) const = 0;
    // Is the engine running a background map?
    virtual bool                IsLevelMainMenuBackground( void ) = 0;
    virtual bool IsAnyClientLowViolence() = 0;
    virtual bool IsSplitScreenPlayer( int ent_num ) = 0;
    virtual edict_t *GetSplitScreenPlayerAttachToEdict( int ent_num ) = 0;
    virtual int        GetNumSplitScreenUsersAttachedToEdict( int ent_num ) = 0;
    virtual edict_t *GetSplitScreenPlayerForEdict( int ent_num, int nSlot ) = 0;

    // Used by Foundry to hook into the loadgame process and override the entities that are getting loaded.
    virtual bool IsOverrideLoadGameEntsOn() = 0;

    // Used by Foundry when it changes an entity (and possibly its class) but preserves its serial number.
    virtual void ForceFlushEntity( int iEntity ) = 0;

    //Finds or Creates a shared memory space, the returned pointer will automatically be AddRef()ed
    virtual ISPSharedMemory *GetSinglePlayerSharedMemorySpace( const char *szName, int ent_num = MAX_EDICTS ) = 0;

    // Allocate hunk memory
    virtual void *AllocLevelStaticData( size_t bytes ) = 0;
    virtual bool IsCreatingReslist() = 0;
    virtual bool IsCreatingXboxReslist() = 0;
    virtual bool IsDedicatedServerForXbox() = 0;
    virtual bool IsDedicatedServerForPS3() = 0;

    virtual void Pause( bool bPause, bool bForce = false ) = 0;

    virtual void SetTimescale( float flTimescale ) = 0;
    // Validate session
    virtual void HostValidateSession() = 0;

    // Update the 360 pacifier/spinner
    virtual void RefreshScreenIfNecessary() = 0;

    // Tells the engine to allocate paint surfaces
    virtual bool HasPaintmap() = 0;

    // Calls ShootPaintSphere
    virtual bool SpherePaintSurface( const model_t *pModel, const Vector &, unsigned char, float, float ) = 0;

    virtual void SphereTracePaintSurface( const model_t *pModel, const Vector &, const Vector &, float, CUtlVector<unsigned char> & ) = 0;

    virtual void RemoveAllPaint() = 0;

    virtual void PaintAllSurfaces( unsigned char ) = 0;
    virtual void RemovePaint( const model_t *pModel ) = 0;
    virtual uint64 GetClientXUID( edict_t *pPlayerEdict ) = 0;
    virtual bool IsActiveApp() = 0;

    virtual void SetNoClipEnabled( bool bEnabled ) = 0;

    virtual void GetPaintmapDataRLE( CUtlVector<unsigned int> &mapdata ) = 0;
    virtual void LoadPaintmapDataRLE( CUtlVector<unsigned int> &mapdata ) = 0;
    virtual void SendPaintmapDataToClient( edict_t *pEdict ) = 0;

    virtual float GetLatencyForChoreoSounds() = 0;

    virtual CrossPlayPlatform_t GetClientCrossPlayPlatform( int ent_num ) = 0;

    virtual void EnsureInstanceBaseline( int ent_num ) = 0;

    virtual bool ReserveServerForQueuedGame( const char *szReservationPayload ) = 0;

    virtual bool GetEngineHltvInfo( CEngineHltvInfo_t &out ) = 0;
    */
}