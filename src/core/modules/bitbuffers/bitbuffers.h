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

#ifndef _BITBUFFERS_H
#define _BITBUFFERS_H

//-----------------------------------------------------------------------------
// Includes.
//-----------------------------------------------------------------------------
#include "tier1/bitbuf.h"


//-----------------------------------------------------------------------------
// bf_write extension class.
//-----------------------------------------------------------------------------
class BitBufferWriteExt
{
public:
	static boost::shared_ptr<bf_write> __init__(int buffer_size)
	{
		return boost::shared_ptr<bf_write>(new bf_write(new unsigned char[buffer_size], buffer_size), &__del__);
	}

	static void __del__(bf_write* buffer)
	{
		delete buffer->GetData();
	}
};


//-----------------------------------------------------------------------------
// bf_read extension class.
//-----------------------------------------------------------------------------
class BitBufferReadExt
{
public:
	static boost::shared_ptr<bf_read> __init__(bf_write& buffer)
	{
		int size = buffer.GetNumBytesWritten();
		void* pData = new unsigned char[size];
		memcpy(pData, buffer.GetData(), size);
		return boost::shared_ptr<bf_read>(new bf_read(pData, size), &__del__);
	}

	static void __del__(bf_read* buffer)
	{
		delete buffer->GetBasePointer();
	}

	static int GetNumBytesRead(bf_read& buffer)
	{
		return BitByte(buffer.GetNumBitsRead());
	}

	static str ReadString(bf_read& buffer)
	{
		char* pStr = new char[buffer.m_nDataBytes];
		buffer.ReadString(pStr, buffer.m_nDataBytes);

		// Let Boost handle deallocating the string
		str result = str((const char *) pStr);
		delete pStr;
		return result;
	}
};


#endif // _BITBUFFERS_H
