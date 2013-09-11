// =======================================================================
// File: func_stack.h
// Purpose: This file represents the stack frame of a function. It stores
//	CFuncArg instances and calculates the offsets they are from a given
//	ESP value.
// =======================================================================
#ifndef _FUNC_STACK_H
#define _FUNC_STACK_H

// =======================================================================
// Includes
// =======================================================================
#include "arg_class.h"

// =======================================================================
// A node representing an argument. To be used in our linked list of
// function arguments.
// =======================================================================
struct ArgNode_t
{
	CFuncArg*  m_pArg;		// Argument instance.
	int		   m_nOffset;	// Offset of the argument from ESP.
	ArgNode_t* m_pNext;		// Next argument in the list.
};

// =======================================================================
// The stack class.
// =======================================================================
class CFuncStack
{
	private:
		// ------------------------------------
		// Linked list of arguments.
		// ------------------------------------
		ArgNode_t* m_pArgListHead;

		// ------------------------------------
		// Number of arguments registered to
		// this stack instance.
		// ------------------------------------
		int m_nNumArgs;

		// ------------------------------------
		// Total stack size.
		// ------------------------------------
		int m_nTotalSize;

	public:

		// ------------------------------------
		// Constructor / Destructor.
		// ------------------------------------
		CFuncStack( void );
		~CFuncStack( void );

		// ------------------------------------
		// @brief Adds an argument to the
		//	linked list.
		// @return The CFuncArg instance that
		//	was just created.
		// ------------------------------------
		bool AddArgument( CFuncArg* pArg );

		// ------------------------------------
		// @brief Returns an ArgNode_t by its
		//	position in the stack.
		// @param pos - Position of the
		//	desired argument on the stack.
		// @return The ArgNode_t instance at the
		//	given index.
		// ------------------------------------
		ArgNode_t* GetArgument( int pos );

		// ------------------------------------
		// Stack size accessor.
		// ------------------------------------
		int GetStackSize( void ) { return m_nTotalSize; }

		// ------------------------------------
		// NumArgs accessor.
		// ------------------------------------
		int GetNumArgs( void ) { return m_nNumArgs; }

};

#endif // _FUNC_STACK_H
