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

#ifndef _VECMATH_WRAP_H
#define _VECMATH_WRAP_H

#include "mathlib/vector.h"
#include "boost/python.hpp"
using namespace boost::python;


class CVector
{
public:
	CVector(float x, float y, float z);
	CVector();
	CVector(Vector vec);

	operator Vector();
	operator Vector() const;

	float		get_x();
	void		set_x(float fValue);

	float		get_y();
	void		set_y(float fValue);

	float		get_z();
	void		set_z(float fValue);

	float		get_item(int iIndex);
	void		set_item(int iIndex, float fValue);

	bool		is_valid();
	void		invalidate();
	void		random(float fMin, float fMax);
	void		zero();

	bool		operator==(CVector* pOther);
	bool		operator!=(CVector* pOther);

	CVector*	operator+(object value);
	CVector*	operator-(object value);
	CVector*	operator*(object value);
	CVector*	operator/(object value);

	void		negate();
	float		get_length();
	float		get_length_sqr();
	bool		is_zero(float fTolerance = 0.01f);
	float		normalize();
	bool		is_within_box(CVector* pMins, CVector* pMaxs);
	float		get_distance(CVector* pOther);
	float		get_distance_sqr(CVector* pOther);
	void		mul_add(CVector* pA, CVector* pB, float fScalar);
	float		dot(CVector* pOther);
	float		get_length_2D();
	float		get_length_2D_sqr();
	CVector*	cross(CVector* pOther);
	CVector*	get_min(CVector* pOther);
	CVector*	get_max(CVector* pOther);

private:
	Vector m_Vector;
};

#endif // _VECMATH_WRAP_H