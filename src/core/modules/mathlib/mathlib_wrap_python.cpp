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
#include "mathlib/vector.h"


//-----------------------------------------------------------------------------
// Exposes the mathlib_c module.
//-----------------------------------------------------------------------------
void export_vector();

DECLARE_SP_MODULE(mathlib_c)
{
	export_vector();
}

//-----------------------------------------------------------------------------
// Exposes Vector
//-----------------------------------------------------------------------------
class VectorExt
{
public:
	static void SetItem(Vector* pVec, int iIndex, float fValue)
	{
		pVec[iIndex] = fValue;
	}
};

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(is_zero_overload, IsZero, 0, 1)

void export_vector()
{
	// TODO: Rename
	class_<Vector>("CVector")
		.def(init<float, float, float>())
		.def(init<float>())

		// Members
		.def_readwrite("x",
			&Vector::x
		)

		.def_readwrite("y",
			&Vector::y
		)

		.def_readwrite("z",
			&Vector::z
		)

		// Methods
		.def("init",
			&Vector::Init
		)

		.def("is_valid",
			&Vector::IsValid,
			"Returns True if the vector is valid."
		)

		.def("invalidated",
			&Vector::Invalidate,
			"Invalidates the vector."
		)

		.def("__getitem__",
			static_cast< float(Vector::*)( int ) const >(&Vector::operator[])
		)

		.def("__setitem__",
			&VectorExt::SetItem
		)

		.def("as_vector_2D",
			GET_METHOD(Vector2D&, Vector, AsVector2D),
			reference_existing_object_policy()
		)

		.def("random",
			&Vector::Random,
			args("min", "max"),
			"Fills the vector with random values within the given range."
		)

		.def("zero",
			&Vector::Zero,
			"Zeros out the vector."
		)

		.def(self == self)
		.def(self != self)
		.def(self += self)
		.def(self -= self)
		.def(self *= self)
		.def(self *= float())
		.def(self /= self)
		.def(self /= float())
		.def(self += float())
		.def(self -= float())

		.def("negate",
			&Vector::Negate,
			"Negates the vector."
		)

		.def("get_length",
			&Vector::Length,
			"Returns the vector's 3D length."
		)

		.def("get_length_sqr",
			&Vector::LengthSqr,
			"Returns the vector's 3D length as a square product."
		)

		.def("is_zero",
			&Vector::IsZero,
			is_zero_overload(
				args("tolerance"),
				"Returns True if x, y and z are zero or within the tolerance."
			)
		)

		.def("normalize",
			&Vector::NormalizeInPlace,
			"Normalizes the vector."
		)

		.def("is_length_greater_than",
			&Vector::IsLengthGreaterThan
		)

		.def("is_length_less_than",
			&Vector::IsLengthLessThan
		)

		.def("is_within_box",
			&Vector::WithinAABox,
			args("mins", "maxs"),
			"Returns True if the vector is within the given box coordinates."
		)

		.def("get_distance",
			&Vector::DistTo,
			args("other"),
			"Returns the distance to the other vector."
		)

		.def("get_distance_sqr",
			&Vector::DistToSqr,
			args("other"),
			"Returns the distance to the other vector as a square product."
		)

		.def("mul_add",
			&Vector::MulAdd,
			args("a", "b", "scalar"),
			"Multiply and add. this = a + b * scalar."
		)

		.def("dot",
			&Vector::Dot,
			"Returns the dot product."
		)

		.def("get_length_2D",
			&Vector::Length2D,
			"Returns the vector's 2D length."
		)

		.def("get_length_2D_sqr",
			&Vector::Length2DSqr,
			"Returns the vector's 2D length as a square product."
		)

		.def(self + self)
		.def(self - self)
		.def(self * self)
		.def(self / self)
		.def(self * float())
		.def(self / float())

		.def("cross",
			&Vector::Cross,
			"Returns the cross product between two vectors."
		)

		.def("min",
			&Vector::Min,
			"Returns a new vector containing the lowest values of both vectors."
		)

		.def("max",
			&Vector::Max,
			"Returns a new vector containing the biggest values of both vectors."
		)
	;
}