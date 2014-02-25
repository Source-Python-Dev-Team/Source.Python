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
#include "mathlib/mathlib.h"
#include "utility/sp_util.h"


//-----------------------------------------------------------------------------
// Exposes the mathlib_c module.
//-----------------------------------------------------------------------------
void export_vector();
void export_qangle();
void export_quaternion();
void export_cplane_t();

DECLARE_SP_MODULE(mathlib_c)
{
	export_vector();
	export_qangle();
	export_quaternion();
	export_cplane_t();
}

//-----------------------------------------------------------------------------
// Exposes Vector
//-----------------------------------------------------------------------------
class VectorExt
{
public:
	static Vector* CreateNullVector()
	{
		return new Vector(0, 0, 0);
	}
};

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(is_zero_overload, IsZero, 0, 1)

void export_vector()
{
	class_<Vector>("Vector", init<float, float, float>())
		.def("__init__", make_constructor(&VectorExt::CreateNullVector))

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
			&Vector::Init,
			args("x", "y", "z")
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
			&GetItemIndexer<Vector, float, 0, 2>
		)

		.def("__setitem__",
			&SetItemIndexer<Vector, float, 0, 2>
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
			&Vector::IsLengthGreaterThan,
			args("value")
		)

		.def("is_length_less_than",
			&Vector::IsLengthLessThan,
			args("value")
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
			"Returns the dot product.",
			args("other")
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
			"Returns the cross product between two vectors.",
			args("other")
		)

		.def("min",
			&Vector::Min,
			"Returns a new vector containing the lowest values of both vectors.",
			args("other")
		)

		.def("max",
			&Vector::Max,
			"Returns a new vector containing the biggest values of both vectors.",
			args("other")
		)
	;
}

//-----------------------------------------------------------------------------
// Exposes QAngle
//-----------------------------------------------------------------------------
void export_qangle()
{
	// TODO: Documentation
	class_<QAngle>("QAngle")
		.def(init<float, float, float>())

		.def_readwrite("x",
			&QAngle::x
		)

		.def_readwrite("y",
			&QAngle::y
		)
		
		.def_readwrite("z",
			&QAngle::z
		)

		.def("init",
			&QAngle::Init,
			"Initializes the QAngle instance with the given values.",
			args("x", "y", "z")
		)

		.def("random",
			&QAngle::Random,
			args("min_val", "max_val")
		)

		.def("is_valid",
			&QAngle::IsValid
		)

		.def("invalidate",
			&QAngle::Invalidate
		)

		.def("__getitem__",
			&GetItemIndexer<QAngle, float, 0, 2>
		)

		.def("__setitem__",
			&SetItemIndexer<QAngle, float, 0, 2>
		)

		.def(self == self)
		.def(self != self)
		
		.def(self += self)
		.def(self -= self)
		.def(self *= other<float>())
		.def(self /= other<float>())

		.def("get_length",
			&QAngle::Length
		)

		.def("get_length_sqr",
			&QAngle::LengthSqr
		)

		.def(self + self)
		.def(self - self)
		.def(self * other<float>())
		.def(self / other<float>())
	;
}

//-----------------------------------------------------------------------------
// Exposes Quaternion
//-----------------------------------------------------------------------------
void export_quaternion()
{
	// TODO: Documentation
	class_<Quaternion>("Quaternion")
		.def(init<float, float, float, float>())
		.def(init<RadianEuler>())

		.def("init",
			&Quaternion::Init,
			args("x", "y", "z", "w")
		)

		.def("is_valid",
			&Quaternion::IsValid
		)

		.def("invalidate",
			&Quaternion::Invalidate
		)

		.def(self == self)
		.def(self != self)

		.def("__getitem__",
			&GetItemIndexer<Quaternion, float, 0, 3>
		)

		.def("__setitem__",
			&SetItemIndexer<Quaternion, float, 0, 3>
		)

		.def_readwrite("x",
			&Quaternion::x
		)

		.def_readwrite("y",
			&Quaternion::y
		)

		.def_readwrite("z",
			&Quaternion::z
		)

		.def_readwrite("w",
			&Quaternion::w
		)
	;
}

//-----------------------------------------------------------------------------
// Exposes cplane_t
//-----------------------------------------------------------------------------
void export_cplane_t()
{
	// TODO: Documentation
	class_<cplane_t>("cplane")
		.def_readwrite("normal",
			&cplane_t::normal
		)

		.def_readwrite("dist",
			&cplane_t::dist
		)

		.def_readwrite("type",
			&cplane_t::type
		)

		.def_readwrite("signbits",
			&cplane_t::signbits
		)

		// TODO: byte	pad[2];
	;
}

//-----------------------------------------------------------------------------
// Exposes RadianEuler
//-----------------------------------------------------------------------------
void export_radian_euler()
{
	// TODO: Documentation
	class_<RadianEuler>("RadianEuler")
		.def(init<float, float, float>())
		.def(init<Quaternion>())
		.def(init<QAngle>())

		.def("init",
			&RadianEuler::Init
		)

		.def("to_qangle",
			&RadianEuler::ToQAngle
		)

		.def("is_valid",
			&RadianEuler::IsValid
		)

		.def("invalidate",
			&RadianEuler::Invalidate
		)

		.def("__getitem__",
			&GetItemIndexer<RadianEuler, float, 0, 2>
		)

		.def("__setitem__",
			&SetItemIndexer<RadianEuler, float, 0, 2>
		)

		.def_readwrite("x",
			&RadianEuler::x
		)

		.def_readwrite("y",
			&RadianEuler::y
		)

		.def_readwrite("z",
			&RadianEuler::z
		)
	;
}