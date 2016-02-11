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

#ifndef _MATHLIB_H
#define _MATHLIB_H

//-----------------------------------------------------------------------------
// Includes.
//-----------------------------------------------------------------------------
#include "mathlib/vector.h"


//-----------------------------------------------------------------------------
// Vector extension class.
//-----------------------------------------------------------------------------
class VectorExt
{
public:
	static bool IsWithinBox(Vector& point, Vector& corner1, Vector& corner2)
	{
		return point.WithinAABox(corner1.Min(corner2), corner2.Max(corner1));
	}

	static void SetLength(Vector& vec, int iLength)
	{
		vec.NormalizeInPlace();
		vec *= iLength;
	}

	static str __repr__(Vector* pVector)
	{ return str("Vector" + str(tuple(pVector))); }
};


//-----------------------------------------------------------------------------
// QAngle extension class.
//-----------------------------------------------------------------------------
class QAngleExt
{
public:
	static str __repr__(QAngle* pQAngle)
	{ return str("QAngle" + str(tuple(pQAngle))); }
};


//-----------------------------------------------------------------------------
// Quaternion extension class.
//-----------------------------------------------------------------------------
class QuaternionExt
{
public:
	static str __repr__(Quaternion* pQuaternion)
	{ return str("Quaternion" + str(tuple(pQuaternion))); }
};


//-----------------------------------------------------------------------------
// RadianEuler extension class.
//-----------------------------------------------------------------------------
class RadianEulerExt
{
public:
	static str __repr__(RadianEuler* pRadianEuler)
	{ return str("RadianEuler" + str(tuple(pRadianEuler))); }
};

#endif // _MATHLIB_H
