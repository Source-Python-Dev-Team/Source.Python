/**
* =============================================================================
* Source Python
* Copyright (C) 2012 Source Python Development Team.  All rights reserved.
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

#include "effects_wrap.h"

extern IEffects* effects;

void CEffects::beam(const CVector &Start, const CVector &End, int nModelIndex, 
	int nHaloIndex, unsigned char frameStart, unsigned char frameRate,
	float flLife, unsigned char width, unsigned char endWidth, unsigned char fadeLength, 
	unsigned char noise, unsigned char red, unsigned char green,
	unsigned char blue, unsigned char brightness, unsigned char speed)
{
	effects->SetSuppressHost(NULL);
	effects->Beam(Start, End, nModelIndex, nHaloIndex, frameStart, frameRate, flLife,
			width, endWidth, fadeLength, noise, red, green, blue, brightness, speed);
}

void CEffects::smoke(const CVector &origin, int modelIndex, float scale, float framerate)
{
	effects->Smoke(origin, modelIndex, scale, framerate);
}

void CEffects::sparks(const CVector &position, int nMagnitude /* = 1 */, int nTrailLength /* = 1 */, const CVector *pvecDir /* = NULL */)
{
	effects->Sparks(position, nMagnitude, nTrailLength, (Vector *) pvecDir);
}

void CEffects::dust(const CVector &pos, const CVector &dir, float size, float speed)
{
	effects->Dust(pos, dir, size, speed);
}

void CEffects::muzzle_flash(const CVector &vecOrigin, const QAngle &vecAngles, float flScale, int iType)
{
	effects->MuzzleFlash(vecOrigin, vecAngles, flScale, iType);
}

void CEffects::metal_sparks(const CVector &position, const CVector &direction)
{
	effects->MetalSparks(position, direction);
}

void CEffects::energy_splash(const CVector &position, const CVector &direction, bool bExplosive /* = false */)
{
	effects->EnergySplash(position, direction, bExplosive);
}

void CEffects::ricochet(const CVector &position, const CVector &direction)
{
	effects->Ricochet(position, direction);
}

float CEffects::get_time()
{
	return effects->Time();
}

bool CEffects::is_server()
{
	return effects->IsServer();
}

void CEffects::suppress_effects_sounds(bool bSuppress)
{
	effects->SuppressEffectsSounds(bSuppress);
}
