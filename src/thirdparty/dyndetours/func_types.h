// =======================================================================
// File: func_types.h
// Purpose: This file defines some useful constants with regard to
//	function attributes.
// =======================================================================
#ifndef _FUNC_TYPES_H
#define _FUNC_TYPES_H

// =======================================================================
// Calling conventions.
// =======================================================================
enum eCallConv
{
	CONV_CDECL = 0,
#ifdef _WIN32
	CONV_STDCALL = 2,
	CONV_THISCALL = 5,
#else
	CONV_THISCALL = 6,
#endif
};

#endif // _FUNC_TYPES_H
