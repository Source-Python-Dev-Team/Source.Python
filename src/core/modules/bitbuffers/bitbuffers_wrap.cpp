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

//-----------------------------------------------------------------------------
// Includes.
//-----------------------------------------------------------------------------
#include "export_main.h"
#include "utilities/wrap_macros.h"
#include "bitbuffers.h"
#include "modules/memory/memory_tools.h"


//-----------------------------------------------------------------------------
// Forward declarations.
//-----------------------------------------------------------------------------
void export_bf_write(scope);
void export_bf_read(scope);


//-----------------------------------------------------------------------------
// Declare the _bitbuffers module.
//-----------------------------------------------------------------------------
DECLARE_SP_MODULE(_bitbuffers)
{
	export_bf_write(_bitbuffers);
	export_bf_read(_bitbuffers);
}


//-----------------------------------------------------------------------------
// Expose bf_write.
//-----------------------------------------------------------------------------
void export_bf_write(scope _bitbuffers)
{
	class_<bf_write>("BitBufferWrite", init<void*, int, optional<int> >())
		.def("__init__",
			make_constructor(&BitBufferWriteExt::__init__)
		)

		.def("seek_to_bit",
			&bf_write::SeekToBit,
			"Seeks to a specific position."
		)

		.def("write_one_bit",
			&bf_write::WriteOneBit
		)

		.def("write_one_bit_no_check",
			&bf_write::WriteOneBitNoCheck
		)

		.def("write_one_bit_at",
			&bf_write::WriteOneBitAt
		)

		.def("write_ubit_long",
			&bf_write::WriteUBitLong,
			("data", "num_bits", arg("check_range")=true)
		)

		.def("write_sbit_long",
			&bf_write::WriteSBitLong
		)

		.def("write_ubit_var",
			&bf_write::WriteUBitVar,
			"Writes an unsigned integer with variable bit length."
		)

#ifdef ENGINE_CSGO
		.def("write_var_int32",
			&bf_write::WriteVarInt32
		)
#endif

		.def("write_bits_from_buffer",
			&bf_write::WriteBitsFromBuffer,
			"Copy the bits straight out of <pIn>. This seeks <pIn> forward by <nBits>. Returns an error if this buffer or the read buffer overflows."
		)

		.def("write_bit_angle",
			&bf_write::WriteBitAngle
		)

		.def("write_bit_coord",
			&bf_write::WriteBitCoord
		)

		.def("write_bit_coord_mp",
			&bf_write::WriteBitCoordMP
		)

		.def("write_bit_float",
			&bf_write::WriteBitFloat
		)

		.def("write_bit_vec3_coord",
			&bf_write::WriteBitVec3Coord
		)

		.def("write_bit_normal",
			&bf_write::WriteBitNormal
		)

		.def("write_bit_vec3_normal",
			&bf_write::WriteBitVec3Normal
		)

		.def("write_bit_angles",
			&bf_write::WriteBitAngles
		)

		.def("write_char",
			&bf_write::WriteChar
		)

		.def("write_byte",
			&bf_write::WriteByte
		)

		.def("write_short",
			&bf_write::WriteShort
		)

		.def("write_word",
			&bf_write::WriteWord
		)

		.def("write_long",
			&bf_write::WriteLong
		)

		.def("write_long_long",
			&bf_write::WriteLongLong
		)

		.def("write_float",
			&bf_write::WriteFloat
		)

		.def("write_string",
			// Required for CS:GO
			GET_METHOD(bool, bf_write, WriteString, const char*),
			"Returns false if it overflows the buffer."
		)

		.add_property("num_bytes_written",
			&bf_write::GetNumBytesWritten
		)

		.add_property("num_bits_written",
			&bf_write::GetNumBitsWritten
		)

		.add_property("max_num_bits",
			&bf_write::GetMaxNumBits
		)

		.add_property("num_bits_left",
			&bf_write::GetNumBitsLeft
		)

		.add_property("num_bytes_left",
			&bf_write::GetNumBytesLeft
		)

		.add_property("data",
			make_function(&bf_write::GetBasePointer, return_by_value_policy())
		)

		.def("check_for_overflow",
			&bf_write::CheckForOverflow
		)

		.def("is_overflowed",
			&bf_write::IsOverflowed
		)

		.def("set_overflow_flag",
			&bf_write::SetOverflowFlag
		)
		
		.def_readwrite("data_bytes_count",
			&bf_write::m_nDataBytes
		)

		.def_readwrite("data_bits_count",
			&bf_write::m_nDataBits
		)

		.def_readwrite("current_bit",
			&bf_write::m_iCurBit
		)

		ADD_MEM_TOOLS(bf_write)
	;
}


//-----------------------------------------------------------------------------
// Expose bf_read.
//-----------------------------------------------------------------------------
void export_bf_read(scope _bitbuffers)
{
	class_<bf_read>("BitBufferRead", init<void*, int, optional<int> >())
		.def("__init__",
			make_constructor(&BitBufferReadExt::__init__)
		)

		.def("read_one_bit",
			&bf_read::ReadOneBit
		)

		.def("read_bit_angle",
			&bf_read::ReadBitAngle
		)

		.def("read_ubit_long",
			&bf_read::ReadUBitLong
		)

		.def("peek_ubit_long",
			&bf_read::PeekUBitLong
		)

		.def("read_sbit_long",
			&bf_read::ReadSBitLong
		)

		.def("read_ubit_var",
			&bf_read::ReadUBitVar
		)

		.def("read_bit_coord",
			&bf_read::ReadBitCoord
		)

		.def("read_bit_coord_mp",
			&bf_read::ReadBitCoordMP
		)

		.def("read_bit_float",
			&bf_read::ReadBitFloat
		)

		.def("read_bit_normal",
			&bf_read::ReadBitNormal
		)

		.def("read_bit_vec3_coord",
			&bf_read::ReadBitVec3Coord
		)

		.def("read_bit_vec3_normal",
			&bf_read::ReadBitVec3Normal
		)

		.def("read_bit_angles",
			&bf_read::ReadBitAngles
		)

		.def("read_char",
			&bf_read::ReadChar
		)

		.def("read_byte",
			&bf_read::ReadByte
		)

		.def("read_short",
			&bf_read::ReadShort
		)

		.def("read_word",
			&bf_read::ReadWord
		)

		.def("read_long",
			&bf_read::ReadLong
		)

		.def("read_long_long",
			&bf_read::ReadLongLong
		)

		.def("read_float",
			&bf_read::ReadFloat
		)
		
		.def("read_string",
			&BitBufferReadExt::ReadString
		)

		.add_property("num_bytes_left",
			&bf_read::GetNumBytesLeft
		)

		.add_property("num_bytes_read",
			&BitBufferReadExt::GetNumBytesRead
		)

		.add_property("num_bits_left",
			&bf_read::GetNumBitsLeft
		)

		.add_property("num_bits_read",
			&bf_read::GetNumBitsRead
		)

		.def("is_overflowed",
			&bf_read::IsOverflowed
		)

		.def("seek",
			&bf_read::Seek
		)

		.def("seek_relative",
			&bf_read::SeekRelative
		)

		.def("set_overflow_flag",
			&bf_read::SetOverflowFlag
		)

		.add_property("data",
			make_function(&bf_read::GetBasePointer, return_by_value_policy())
		)

		.def_readwrite("data_bytes_count",
			&bf_read::m_nDataBytes
		)

		.def_readwrite("data_bits_count",
			&bf_read::m_nDataBits
		)
		
		.add_property("current_bit",
			&bf_read::Seek,
			&bf_read::GetNumBitsRead
		)

		ADD_MEM_TOOLS(bf_read)
	;
}
