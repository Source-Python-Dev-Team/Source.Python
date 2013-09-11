// =======================================================================
// File: trampoline_class.h
// Purpose: This file contains the CTrampoline class. This class is
//	responsible for saving the first few bytes of the target function that
//	is to be hooked. The trampoline is also used to call the original
//	function after hooks are executed.
// =======================================================================
#ifndef _TRAMPOLINE_CLASS_H
#define _TRAMPOLINE_CLASS_H

// =======================================================================
// The trampoline class.
// =======================================================================
class CTrampoline
{
	private:
		// ------------------------------------
		// Points to the array of bytes we
		// have saved from the original function.
		// ------------------------------------
		unsigned char* m_pSavedBytes;

		// ------------------------------------
		// The number of bytes we saved. Used
		// to allocate space for the saved bytes.
		// ------------------------------------
		int m_iSavedBytes;

		// ------------------------------------
		// The target function.
		// ------------------------------------
		unsigned char* m_pTarget;

	public:

		// ------------------------------------
		// @brief Constructor.
		// @param pTarget - The target function
		//	to create the trampoline from.
		// ------------------------------------
		CTrampoline( void* pTarget );

		// ------------------------------------
		// @brief Destructor.
		// ------------------------------------
		~CTrampoline( void );

		// ------------------------------------
		// @brief Returns a pointer to the
		//	array of saved bytes.
		// ------------------------------------
		void* GetBase( void ) { return m_pSavedBytes; }

		// ------------------------------------
		// @brief Returns the number of bytes
		//	we saved from the original function.
		// ------------------------------------
		int GetNumSavedBytes( void ) { return m_iSavedBytes; }

		// ------------------------------------
		// @brief Restores the original bytes
		//	of the target function.
		// ------------------------------------
		bool Restore( void );
};

#endif // _TRAMPOLINE_CLASS_H
