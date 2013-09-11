// =======================================================================
// File: register_class.h
// Purpose: This class encapsulates common X86 registers used when
//	functions are called. Each calling convention interface should have
//	an instance of this class.
// =======================================================================
#ifndef _REGISTER_CLASS_H
#define _REGISTER_CLASS_H

// =======================================================================
// The register object.
// =======================================================================
class CRegisterObj
{
	// ------------------------------------
	// One thing I need to consider is
	// what happens if a function returns
	// something that is more than 8 bytes.
	// Perhaps I could allocate a retbuf in
	// the detour class. I could grab the
	// size of the retval from the CFuncArg
	// class which represents the retval.
	// ------------------------------------

	public:
		// ------------------------------------
		// Constructors.
		// ------------------------------------
		CRegisterObj( void )
		{
			// ------------------------------------
			// Initialize everything to 0.
			// ------------------------------------
			r_esp = 0;
			r_ebp = 0;
			r_ecx = 0;
			r_edx = 0;
			r_retreg = NULL;
		}

		// ------------------------------------
		// These are public because we want
		// the calling convention classes to
		// grab the addresses of these for
		// inline ASM placement.
		// ------------------------------------
		unsigned long r_esp; // Stack pointer.
		unsigned long r_ebp; // Base pointer.
		unsigned long r_ecx; // Counter. Used for thiscalls on windows.	
		unsigned long r_edx;
		void* r_retreg; // eax and st0 at once
};


#endif // _REGISTER_CLASS_H
