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

	static void SetLength(Vector& vec, float fLength)
	{
		vec.NormalizeInPlace();
		vec *= fLength;
	}

	static inline float GetLength(Vector& vec)
	{
		// Workaround for https://github.com/Source-Python-Dev-Team/Source.Python/issues/150
		#ifdef __linux__
			return sqrt(vec.LengthSqr());
		#else
			return vec.Length();
		#endif
	}

	static inline float GetLength2D(Vector& vec)
	{
		// Workaround for https://github.com/Source-Python-Dev-Team/Source.Python/issues/150
		#ifdef __linux__
			return sqrt(vec.Length2DSqr());
		#else
			return vec.Length2D();
		#endif
	}

	static str __repr__(Vector* pVector)
	{ return str("Vector" + str(tuple(pVector))); }

	static Vector Copy(Vector* pVec)
	{ return *pVec; }

	static Vector Normalized(Vector* pVec)
	{
		Vector copy = *pVec;
		copy.NormalizeInPlace();
		return copy;
	}

	static inline Vector __add__(Vector vecCopy, float val)
	{
		vecCopy += val;
		return vecCopy;
	}

	static inline Vector __sub__(Vector vecCopy, float val)
	{
		vecCopy -= val;
		return vecCopy;
	}
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


//-----------------------------------------------------------------------------
// matrix3x4_t extension class.
//-----------------------------------------------------------------------------
class Matrix3x4Row
{
public:
	Matrix3x4Row(float* row)
	{
		m_row = row;
	}

	float& operator[](unsigned int index)
	{
		return m_row[index];
	}

	str __repr__()
	{
		return str(tuple(*this)); 
	}

public:
	float* m_row;
};


class matrix3x4_tExt
{
public:
	static Matrix3x4Row* __getitem__(matrix3x4_t& matrix, unsigned int index)
	{
		if (index >= 3) {
			BOOST_RAISE_EXCEPTION(PyExc_IndexError, "Index out of range (%d)", index)
		}

		return new Matrix3x4Row(matrix[index]);
	}

	static str __repr__(matrix3x4_t& matrix)
	{
		return str("["
			+ str(__getitem__(matrix, 0)) + ",\n"
			+ str(__getitem__(matrix, 1)) + ",\n"
			+ str(__getitem__(matrix, 2)) + "]");
	}

	static Vector* get_position(matrix3x4_t& matrix)
	{
		Vector* result = new Vector();
		MatrixPosition(matrix, *result);
		return result;
	}

	static QAngle* get_angles(matrix3x4_t& matrix)
	{
		QAngle* result = new QAngle();
		MatrixAngles(matrix, *result);
		return result;
	}
};

#endif // _MATHLIB_H
