// =======================================================================
// File: asmbridge_class.h
// Purpose: This file declares the CASMBridge class. This class will be
//	used to bridge the hooked function to the DynDetours library.
// =======================================================================
#ifndef _ASMBRIDGE_CLASS_H
#define _ASMBRIDGE_CLASS_H

// =======================================================================
// Includes
// =======================================================================
#include "AsmJit/AsmJit.h"
#include "conv_main.h"

// =======================================================================
// Forward declarations.
// =======================================================================
class CDetour;

// =======================================================================
// The ASMBridge class.
// =======================================================================
class CASMBridge
{
	private:
		// ------------------------------------
		// Needed to generate ASM op codes.
		// ------------------------------------
		AsmJit::Assembler m_Assembler; 

		// ------------------------------------
		// Instructions to execute before calling
		// the callback.
		// ------------------------------------
		AsmJit::Label	  m_PreCall;   
		
		// ------------------------------------
		// Instructions to execute after calling
		// the callback.
		// ------------------------------------
		AsmJit::Label	  m_PostCall;  

		// ------------------------------------
		// Instructions to override the return
		// value of the function.
		// ------------------------------------
		AsmJit::Label	  m_Override;  

		// ------------------------------------
		// The calling convention instance
		// used to generate ASM for pre and
		// post calls.
		// ------------------------------------
		ICallConvention* m_pCallConvention; 

		// ------------------------------------
		// True if we're initialized.
		// ------------------------------------
		bool m_bInitialized;

	public:

		// ------------------------------------
		// @brief Constructor
		// @param pDetour - The detour instance
		//	responsible for pTarget.
		// ------------------------------------
		CASMBridge( CDetour* pDetour );
		
		// ------------------------------------
		// @brief Destructor.
		// ------------------------------------
		~CASMBridge( void );

		// ------------------------------------
		// Returns the calling convention for
		// this detour.
		// ------------------------------------
		ICallConvention* GetConv( void ) 
			{ return m_pCallConvention; }
		
		// ------------------------------------
		// Returns the base address of the
		// ASM instruction blob.
		// ------------------------------------
		void* GetBase( void ) { return m_Assembler.make(); }

		// ------------------------------------
		// Returns true if we are initialized.
		// ------------------------------------
		bool GetInitialized( void ) { return m_bInitialized; }
};


#endif // _ASMBRIDGE_CLASS_H

