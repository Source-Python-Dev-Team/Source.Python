// =======================================================================
// File: arg_types.h
// Purpose: This file defines a number of constants which represent
//  attributes of a function argument (be it type, or size).
// =======================================================================
#ifndef _ARG_TYPES_H
#define _ARG_TYPES_H

#define DC_SIGCHAR_VOID         'v'
#define DC_SIGCHAR_BOOL         'B'
#define DC_SIGCHAR_CHAR         'c'
#define DC_SIGCHAR_UCHAR        'C'
#define DC_SIGCHAR_SHORT        's'
#define DC_SIGCHAR_USHORT       'S'
#define DC_SIGCHAR_INT          'i'
#define DC_SIGCHAR_UINT         'I'
#define DC_SIGCHAR_LONG         'j'
#define DC_SIGCHAR_ULONG        'J'
#define DC_SIGCHAR_LONGLONG     'l'
#define DC_SIGCHAR_ULONGLONG    'L'
#define DC_SIGCHAR_FLOAT        'f'
#define DC_SIGCHAR_DOUBLE       'd'
#define DC_SIGCHAR_POINTER      'p'
#define DC_SIGCHAR_STRING       'Z'

// =======================================================================
// Types of arguments we can have.
// =======================================================================
enum eArgType 
{
	TYPE_UNKNOWN,   // Type is not a default type.
	TYPE_VOID,		
	TYPE_BOOL,		
    TYPE_CHAR,
    TYPE_UCHAR,
    TYPE_SHORT,
    TYPE_USHORT,
    TYPE_INT,
    TYPE_UINT,
    TYPE_LONG,
    TYPE_ULONG,
    TYPE_LONGLONG,
    TYPE_ULONGLONG,
    TYPE_FLOAT,
    TYPE_DOUBLE,
    TYPE_POINTER,
    TYPE_STRING
};

// =======================================================================
// If we're passed by reference or not.
// =======================================================================
enum eArgPassType
{
	PASS_UNKNOWN, // Unknown passing convention.
	PASS_BYREF,   // Argument passed by reference to function.
	PASS_BYVAL,   // Argument passed by value to the function.
};

#endif // _ARG_TYPES_H
