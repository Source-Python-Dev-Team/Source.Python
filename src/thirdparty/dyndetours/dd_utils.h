// =======================================================================
// File: dd_utils.h
// Purpose: Useful functions used accross dyndetours.
// =======================================================================
#ifndef _DD_UTILS_H
#define _DD_UTILS_H

// =======================================================================
// Includes
// =======================================================================
#include "arg_types.h"

// =======================================================================
// Returns the eArgType enum corresponding to the given char. Returns
// TYPE_UNKNOWN if there is no mapping.
// =======================================================================
eArgType CharToTypeEnum( char c );
char     TypeEnumToChar(eArgType);

#endif // _DD_UTILS_H
