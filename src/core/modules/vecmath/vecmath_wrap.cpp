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

#include "vecmath_wrap.h"
#include "utility/wrap_macros.h"
#include "utility/sp_util.h"

CVector::CVector(float x, float y, float z)
{
	m_Vector = Vector(x, y, z);
}

CVector::CVector()
{
	m_Vector = Vector();
}

CVector::CVector(Vector vec)
{
	m_Vector = vec;
}

CVector::operator Vector()
{
	return m_Vector;
}

CVector::operator Vector() const
{
	return m_Vector;
}

float CVector::get_x()
{
	return m_Vector.x;
}

void CVector::set_x(float fValue)
{
	m_Vector.x = fValue;
}

float CVector::get_y()
{
	return m_Vector.y;
}

void CVector::set_y(float fValue)
{
	m_Vector.y = fValue;
}

float CVector::get_z()
{
	return m_Vector.z;
}

void CVector::set_z(float fValue)
{
	m_Vector.z = fValue;
}

float CVector::get_item(int iIndex)
{
	if (iIndex == 0)
		return get_x();
	else if (iIndex == 1)
		return get_y();
	else if (iIndex == 2)
		return get_z();
	else
		BOOST_RAISE_EXCEPTION(PyExc_IndexError, "Index out of range.")

	return NULL;
}

void CVector::set_item(int iIndex, float fValue)
{
	if (iIndex == 0)
		return set_x(fValue);
	else if (iIndex == 1)
		return set_y(fValue);
	else if (iIndex == 2)
		return set_z(fValue);

	BOOST_RAISE_EXCEPTION(PyExc_IndexError, "Index out of range.")
}

bool CVector::is_valid()
{
	return m_Vector.IsValid();
}

void CVector::invalidate()
{
	m_Vector.Invalidate();
}

void CVector::random(float fMin, float fMax)
{
	m_Vector.Random(fMin, fMax);
}

void CVector::zero()
{
	m_Vector.Zero();
}

bool CVector::operator==(CVector* pOther)
{
	return m_Vector == *pOther;
}

bool CVector::operator!=(CVector* pOther)
{
	return m_Vector != *pOther;
}

CVector* CVector::operator+(object value)
{
	if (CheckClassname(value, "CVector"))
	{
		CVector other = extract<CVector>(value);
		return new CVector(m_Vector + other);
	}

	float fValue = extract<float>(value);

	// CS:GO SDK does not define Vector::operator+(float fValue), so we make it easy
	// and create a new vector
	return new CVector(m_Vector + CVector(fValue, fValue, fValue));
}

CVector* CVector::operator-(object value)
{
	if (CheckClassname(value, "CVector"))
	{
		CVector other = extract<CVector>(value);
		return new CVector(m_Vector - other);
	}

	float fValue = extract<float>(value);

	// CS:GO SDK does not define Vector::operator+(float fValue), so we make it easy
	// and create a new vector
	return new CVector(m_Vector - CVector(fValue, fValue, fValue));
}

CVector* CVector::operator*(object value)
{
	if (CheckClassname(value, "CVector"))
	{
		CVector other = extract<CVector>(value);
		return new CVector(m_Vector * other);
	}

	float fValue = extract<float>(value);
	return new CVector(m_Vector * fValue);
}

CVector* CVector::operator/(object value)
{
	if (CheckClassname(value, "CVector"))
	{
		CVector other = extract<CVector>(value);
		return new CVector(m_Vector / other);
	}

	float fValue = extract<float>(value);
	return new CVector(m_Vector / fValue);
}

void CVector::negate()
{
	m_Vector.Negate();
}

float CVector::get_length()
{
	return m_Vector.Length();
}

float CVector::get_length_sqr()
{
	return m_Vector.LengthSqr();
}

bool CVector::is_zero(float fTolerance /* = 0.01f */)
{
	return m_Vector.IsZero(fTolerance);
}

float CVector::normalize()
{
	return m_Vector.NormalizeInPlace();
}

bool CVector::is_within_box(CVector* pMins, CVector* pMaxs)
{
	return m_Vector.WithinAABox(*pMins, *pMaxs);
}

float CVector::get_distance(CVector* pOther)
{
	return m_Vector.DistTo(*pOther);
}

float CVector::get_distance_sqr(CVector* pOther)
{
	return m_Vector.DistToSqr(*pOther);
}

void CVector::mul_add(CVector* pA, CVector* pB, float fScalar)
{
	m_Vector.MulAdd(*pA, *pB, fScalar);
}

float CVector::dot(CVector* pOther)
{
	return m_Vector.Dot(*pOther);
}

float CVector::get_length_2D()
{
	return m_Vector.Length2D();
}

float CVector::get_length_2D_sqr()
{
	return m_Vector.Length2DSqr();
}

CVector* CVector::cross(CVector* pOther)
{
	return new CVector(m_Vector.Cross(*pOther));
}

CVector* CVector::get_min(CVector* pOther)
{
	return new CVector(m_Vector.Min(*pOther));
}

CVector* CVector::get_max(CVector* pOther)
{
	return new CVector(m_Vector.Max(*pOther));
}