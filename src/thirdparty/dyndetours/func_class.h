// =======================================================================
// File: func_class.h
// Purpose: This file declares the CFuncObj class which encapsulates and 
//	represents the attributes of a C/C++ function in memory.
// =======================================================================
#ifndef _FUNC_CLASS_H
#define _FUNC_CLASS_H

// =======================================================================
// Includes
// =======================================================================
#include "arg_types.h"
#include "func_stack.h"
#include "func_types.h"
#include <vector>

// =======================================================================
// Namespaces to use
// =======================================================================
using namespace std;

// =======================================================================
// The CFuncObj class.
// =======================================================================
class CFuncObj
{
	private:
		// ------------------------------------
		// Each function has the following
		// attributes:
		//	- Calling Convention.
		//	- Parameters.
		//	- Return value + type.
		//	- Address.
		// ------------------------------------
		
		CFuncArg		   m_RetArg;
		CFuncStack		   m_Stack;
		// vector<CFuncArg *> m_ArgList;
		eCallConv		   m_eConv;
		void*			   m_pAddr;

	public:

		// ------------------------------------
		// @brief Constructor.
		// @param pAddr - The address of the
		//	function.
		// @param conv - Calling convention.
		// ------------------------------------
		CFuncObj( void* pAddr, eCallConv conv );

		// ------------------------------------
		// @brief Constructor. Sets up the
		//	function object via string.
		// @param szParamList - Parameter
		//	format string.
		// @param eConv - Calling convention of
		//	the function.
		// ------------------------------------
		CFuncObj( void* pAddr, const char* szParamList,
					eCallConv eConv );

		// ------------------------------------
		// @brief Destructor
		// ------------------------------------
		~CFuncObj( void );

		// ------------------------------------
		// @brief Adds an argument to this
		//	function's argument stack.
		// @param type - Type of argument.
		// @param passType - How the argument
		//	is passed to the function.
		// @param size - Optional unless type
		//	is TYPE_CUSTOM. The size of the
		//	argument in bytes.
		// ------------------------------------
		void AddArg(eArgType type, eArgPassType passType, int size=0);

		// ------------------------------------
		// Returns the number of arguments
		// ------------------------------------
		unsigned int GetNumArgs( void );

		// ------------------------------------
		// Retrieves an argument at the
		// specified index. Returns NULL if
		// there is no argument at this index.
		// ------------------------------------
		CFuncArg* GetArg( int iArgNum );

		// ------------------------------------
		// Stacj accessor.
		// ------------------------------------
		CFuncStack* GetStack( void );

		// ------------------------------------
		// @brief Returns return value
		//	information.
		// ------------------------------------
		CFuncArg* GetRetType( void ) { return &m_RetArg; }

		// ------------------------------------
		// @brief Sets the return type. 
		//	NOTE: If you used a parameter
		//	format string to create this funcobj,
		//	you do not need to use this method.
		// ------------------------------------
		void SetRetType( eArgType retType, int size=0 );

		// ------------------------------------
		// @brief Accessor for calling
		//	convention.
		// ------------------------------------
		eCallConv GetConvention( void ) { return m_eConv; }

		// ------------------------------------
		// @brief Accessor for function
		//	address.
		// ------------------------------------
		void* GetAddress( void ) { return m_pAddr; }
};

#endif // _FUNC_CLASS_H

