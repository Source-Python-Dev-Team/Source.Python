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

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "modules/export_main.h"
#include "vecmath_wrap.h"

void export_vector();

//-----------------------------------------------------------------------------
// Exposes the vecmath_c module.
//-----------------------------------------------------------------------------
DECLARE_SP_MODULE(vecmath_c)
{
	export_vector();
}

//-----------------------------------------------------------------------------
// Exposes CVector
//-----------------------------------------------------------------------------
// Overloads
DECLARE_CLASS_METHOD_OVERLOAD(CVector, is_zero, 0, 1)

void export_vector()
{
	BOOST_CLASS(CVector)
		CLASS_CONSTRUCTOR(float, float, float)

		// Methods
		CLASS_METHOD(CVector,
			is_valid,
			"Returns True if the vector is valid."
		)

		CLASS_METHOD(CVector,
			invalidate,
			"Invalidates the vector."
		)

		CLASS_METHOD(CVector,
			random,
			args("fMin", "fMax"),
			"Fills the vector with random values within the given range."
		)

		CLASS_METHOD(CVector,
			zero,
			"Zeros out the vector."
		)

		CLASS_METHOD(CVector,
			negate,
			"Negates the vector."
		)

		CLASS_METHOD(CVector,
			get_length,
			"Returns the vector's 3D length."
		)

		CLASS_METHOD(CVector,
			get_length_sqr,
			"Returns the vector's 3D length as a square product."
		)

		CLASS_METHOD_OVERLOAD(CVector,
			is_zero,
			args("fTolerance"),
			"Returns True if x, y and z are zero or within the tolerance."
		)

		CLASS_METHOD(CVector,
			normalize,
			"Normalizes the vector."
		)

		CLASS_METHOD(CVector,
			is_within_box,
			args("pMins", "pMaxs"),
			"Returns True if the vector is within the given box coordinates."
		)

		CLASS_METHOD(CVector,
			get_distance,
			args("pOther"),
			"Returns the distance to the other vector."
		)

		CLASS_METHOD(CVector,
			get_distance_sqr,
			args("pOther"),
			"Returns the distance to the other vector as a square product."
		)

		CLASS_METHOD(CVector,
			mul_add,
			args("pA", "pB", "fScalar"),
			"Multiply and add. this = a + b * scalar."
		)

		CLASS_METHOD(CVector,
			dot,
			args("pOther"),
			"Returns the dot product."
		)

		CLASS_METHOD(CVector,
			get_length_2D,
			"Returns the vector's 2D length."
		)

		CLASS_METHOD(CVector,
			get_length_2D_sqr,
			"Returns the vector's 2D length as a square product."
		)

		CLASS_METHOD(CVector,
			cross,
			args("pOther"),
			"Returns the cross product between two vectors.",
			manage_new_object_policy()
		)

		CLASS_METHOD(CVector,
			get_min,
			args("pOther"),
			"Returns a new vector containing the min values of both vectors.",
			manage_new_object_policy()
		)

		CLASS_METHOD(CVector,
			get_max,
			args("pOther"),
			"Returns a new vector containing the max values of both vectors.",
			manage_new_object_policy()
		)

		// Properties
		CLASS_PROPERTY_READWRITE(CVector,
			"x",
			get_x,
			set_x,
			"X coordinate."
		)

		CLASS_PROPERTY_READWRITE(CVector,
			"y",
			get_y,
			set_y,
			"Y coordinate."
		)

		CLASS_PROPERTY_READWRITE(CVector,
			"z",
			get_z,
			set_z,
			"Z coordinate."
		)

		// Special methods
		CLASS_METHOD_SPECIAL(CVector,
			"__getitem__",
			get_item,
			args("iIndex"),
			"my_vec.x -> my_vec[0]"
		)

		CLASS_METHOD_SPECIAL(CVector,
			"__setitem__",
			set_item,
			args("iIndex", "fValue"),
			"my_vec.x = 10.6 -> my_vec[0] = 10.6"
		)

		CLASS_METHOD_SPECIAL(CVector,
			"__eq__",
			operator==,
			args("pOther"),
			"Returns True if both vectors are the same."
		)

		CLASS_METHOD_SPECIAL(CVector,
			"__ne__",
			operator!=,
			args("pOther"),
			"Returns True if both vectors are not the same."
		)

		CLASS_METHOD_SPECIAL(CVector,
			"__add__",
			operator+,
			args("value"),
			"Adds a single value or a vector to the vector.",
			manage_new_object_policy()
		)

		CLASS_METHOD_SPECIAL(CVector,
			"__sub__",
			operator-,
			args("value"),
			"Subtracts a single value or a vector from the vector.",
			manage_new_object_policy()
		)

		CLASS_METHOD_SPECIAL(CVector,
			"__mul__",
			operator*,
			args("value"),
			"Multiplies a single value or a vector with the vector.",
			manage_new_object_policy()
		)

		CLASS_METHOD_SPECIAL(CVector,
			"__div__",
			operator/,
			args("value"),
			"Divides a single value or a vector by the vector.",
			manage_new_object_policy()
		)

	BOOST_END_CLASS()
}