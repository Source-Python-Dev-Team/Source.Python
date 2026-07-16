/**
* =============================================================================
* DynamicHooks
* Linux x86-64 System V calling convention support.
* =============================================================================
*/

#ifndef _X64_GCC_SYSTEM_V_H
#define _X64_GCC_SYSTEM_V_H

#include "convention.h"

class x64GccSystemV : public ICallingConvention
{
public:
	x64GccSystemV(
		std::vector<DataType_t> vecArgTypes,
		DataType_t returnType,
		int iAlignment=8
	) : ICallingConvention(vecArgTypes, returnType, iAlignment) {}

	std::list<Register_t> GetRegisters();
	int GetPopSize();
	void* GetArgumentPtr(int iIndex, CRegisters* pRegisters);
	void ArgumentPtrChanged(int iIndex, CRegisters* pRegisters, void* pArgumentPtr);
	void* GetReturnPtr(CRegisters* pRegisters);
	void ReturnPtrChanged(CRegisters* pRegisters, void* pReturnPtr);

	CRegister* GetRegister(Register_t reg, CRegisters* pRegisters);

private:
	bool IsSseType(DataType_t type) const;
};

#endif // _X64_GCC_SYSTEM_V_H
