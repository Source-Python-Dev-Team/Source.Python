// =======================================================================
// File: conv_interface.h
// Purpose: This file declares the ICallConvention interface. This
//	interface is used providing uniformity accross calling conventions.
//	The ASMBridge class uses this interface in order to generate the
//	correct ASM without having it all stuffed into a single function.
// =======================================================================
#ifndef _CONV_INTERFACE_H
#define _CONV_INTERFACE_H

// =======================================================================
// Includes
// =======================================================================
#include "AsmJit/AsmJit.h"
#include "register_class.h"

// =======================================================================
// The ICallConvention interface.
// =======================================================================
class ICallConvention
{
	public:

		/* @brief Generates code to save convention specific registers.
		 *	Called by CASMBridge when generating the ASM code for the
		 *	detour.
		 * @param pAssembler - The assembler to generate the code with.
		 */
		virtual void Stage_1( AsmJit::Assembler* pAssembler ) = 0;

		/* @brief Generates code to clean up the stack. Called by the
		 *	CASMBridge class when generating ASM code to clean up the
		 *	stack.
		 * @param pAssembler - The assembler to generate the code with.
		 */
		virtual void Stage_2( AsmJit::Assembler* pAssembler ) = 0;

		/* @brief Generates code which will execute before we do the
		 *	override.
		 * @param pAssembler - The assembler to generate code with.
		 */
		virtual void Stage_3( AsmJit::Assembler* pAssembler ) = 0;

		/* @brief Returns the register object containing the values
		 *	of x86 registers pertinent to this call.
		 */
		virtual CRegisterObj* GetRegisters( void ) = 0;
};

#endif // _CONV_INTERFACE_H
