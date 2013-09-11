// =======================================================================
// File: conv_main.h
// Purpose: Links in all calling convention types. This file will
//	disappear as soon as I write dynamic calling convention instantiation
//	code.
// 
// Ideas: Creating a linked list of convention names (stored as strings),
//	paired to a function to generate new instances of calling convention
//	classes.
// =======================================================================
#ifndef _CONV_MAIN_H
#define _CONV_MAIN_H

// =======================================================================
// Includes
// =======================================================================
#include "conv_interface.h"
#include "func_types.h"

// =======================================================================
// Returns a new instance of a calling convention.
// =======================================================================
ICallConvention* EnumToConvention(eCallConv conv);

#endif // _CONV_MAIN_H
