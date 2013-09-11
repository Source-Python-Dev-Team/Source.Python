// =======================================================================
// File: arg_class.h
// Purpose: Defines a class which represents a function argument.
// =======================================================================
#ifndef _ARG_CLASS_H
#define _ARG_CLASS_H

// =======================================================================
// Includes
// =======================================================================
#include "arg_types.h"

// =======================================================================
// The CFuncArg class. This class allows one to build argument information
// and then parent that information to a function object.
// =======================================================================
class CFuncArg
{
	private:
		eArgType		m_ArgType; 
		eArgPassType	m_PassType;
		unsigned int	m_nSize;
		unsigned int	m_nOffset;

	public:
		// ------------------------------------
		// @brief Constructor.
		// ------------------------------------
		CFuncArg( void );

		// ------------------------------------
		// Setter / Getter for arg type.
		// ------------------------------------
		void SetType( eArgType type );
		eArgType GetType( void ) { return m_ArgType; }

		// ------------------------------------
		// Setter / Getter for arg passtype.
		// ------------------------------------
		void SetPassType( eArgPassType passType ) { m_PassType = passType; }
		eArgPassType GetPassType( void ) { return m_PassType; }

		// ------------------------------------
		// Setter / Getter for arg size.
		// ------------------------------------
		unsigned int GetSize( void ) { return m_nSize; }
		void SetSize( unsigned int size ) { m_nSize = size; }

		// ------------------------------------
		// Setter / Getter for stack offset.
		// ------------------------------------
		unsigned int GetOffset( void ) { return m_nOffset; }
		void SetOffset( unsigned int offset ) { m_nOffset = offset; }
};

#endif // _ARG_CLASS_H
