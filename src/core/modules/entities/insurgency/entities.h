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

#ifndef _ENTITIES_INSURGENCY_H
#define _ENTITIES_INSURGENCY_H

//-----------------------------------------------------------------------------
// Includes.
//-----------------------------------------------------------------------------
#include "dt_send.h"
#include "server_class.h"
#include "game/shared/ehandle.h"
#include "isaverestore.h"
#include "datamap.h"
#include "game/shared/takedamageinfo.h"


//-----------------------------------------------------------------------------
// CTakeDamageInfo wrapper class.
//-----------------------------------------------------------------------------
class TakeDamageInfoWrapper: public TakeDamageInfoBaseWrapper
{
public:
	int get_penetrated()
	{
		return m_iObjectsPenetrated;
	}
	
	void set_penetrated(int iPenetrated)
	{
		m_iObjectsPenetrated = iPenetrated;
	}
	
	uint32 get_bullet()
	{
		return m_uiBulletID;
	}
	
	void set_bullet(uint32 uiBullet)
	{
		m_uiBulletID = uiBullet;
	}
	
	uint8 get_recoil()
	{
		return m_uiRecoilIndex;
	}
	
	void set_recoil(uint8 uiRecoil)
	{
		m_uiRecoilIndex = uiRecoil;
	}
};


//-----------------------------------------------------------------------------
// CTakeDamageInfo extension class.
//-----------------------------------------------------------------------------
class TakeDamageInfoExt
{
public:
	static int get_penetrated(CTakeDamageInfo *pTakeDamageInfo)
	{
		return ((TakeDamageInfoWrapper *)pTakeDamageInfo)->get_penetrated();
	}
	
	static void set_penetrated(CTakeDamageInfo *pTakeDamageInfo, int iPenetrated)
	{
		((TakeDamageInfoWrapper *)pTakeDamageInfo)->set_penetrated(iPenetrated);
	}
	
	static uint32 get_bullet(CTakeDamageInfo *pTakeDamageInfo)
	{
		return ((TakeDamageInfoWrapper *)pTakeDamageInfo)->get_bullet();
	}
	
	static void set_bullet(CTakeDamageInfo *pTakeDamageInfo, uint32 uiBullet)
	{
		((TakeDamageInfoWrapper *)pTakeDamageInfo)->set_bullet(uiBullet);
	}
	
	static uint8 get_recoil(CTakeDamageInfo *pTakeDamageInfo)
	{
		return ((TakeDamageInfoWrapper *)pTakeDamageInfo)->get_recoil();
	}
	
	static void set_recoil(CTakeDamageInfo *pTakeDamageInfo, uint8 uiRecoil)
	{
		((TakeDamageInfoWrapper *)pTakeDamageInfo)->set_recoil(uiRecoil);
	}
};


#endif // _ENTITIES_INSURGENCY_H
