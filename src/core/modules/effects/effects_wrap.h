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

#ifndef _EFFECTS_H
#define _EFFECTS_H

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// Classes
//-----------------------------------------------------------------------------
class CEffects
{
public:
	void beam(const CVector &Start, const CVector &End, int nModelIndex, 
		int nHaloIndex, unsigned char frameStart, unsigned char frameRate,
		float flLife, unsigned char width, unsigned char endWidth, unsigned char fadeLength, 
		unsigned char noise, unsigned char red, unsigned char green,
		unsigned char blue, unsigned char brightness, unsigned char speed);

	void smoke(const CVector &origin, int modelIndex, float scale, float framerate);
	void sparks(const CVector &position, int nMagnitude = 1, int nTrailLength = 1, const CVector *pvecDir = NULL);
	void dust(const CVector &pos, const CVector &dir, float size, float speed);
	void muzzle_flash(const CVector &vecOrigin, const QAngle &vecAngles, float flScale, int iType);
	void metal_sparks(const CVector &position, const CVector &direction);
	void energy_splash(const CVector &position, const CVector &direction, bool bExplosive = false);
	void ricochet(const CVector &position, const CVector &direction);
	float get_time();
	bool is_server();
	void suppress_effects_sounds(bool bSuppress);
};

inline CEffects* get_effects()
{
	static CEffects* s_pEffects = new CEffects();
	return s_pEffects;
}

#endif // _EFFECTS_H