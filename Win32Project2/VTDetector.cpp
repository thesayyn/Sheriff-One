#include "stdafx.h"
#include "VTDetector.h"

DWORD __forceinline VTDetector::IsInsideVPC_exceptionFilter(LPEXCEPTION_POINTERS ep)
{
	PCONTEXT ctx = ep->ContextRecord;
	ctx->Ebx = -1; 
	ctx->Eip += 4; 
	return EXCEPTION_CONTINUE_EXECUTION;
}

bool VTDetector::IsInsideVPC()
{
	bool rc = false;

	__try
	{
		_asm push ebx
		_asm mov  ebx, 0 
		_asm mov  eax, 1 

						 
		_asm __emit 0Fh
		_asm __emit 3Fh
		_asm __emit 07h
		_asm __emit 0Bh

		_asm test ebx, ebx
		_asm setz[rc]
			_asm pop ebx
	}
	__except (VTDetector::IsInsideVPC_exceptionFilter(GetExceptionInformation()))
	{
	}

	return rc;
}

bool VTDetector::IsInsideVMWare()
{
	bool rc = true;

	__try
	{
		__asm
		{
			push   edx
			push   ecx
			push   ebx

			mov    eax, 'VMXh'
			mov    ebx, 0 
			mov    ecx, 10 
			mov    edx, 'VX' 

			in     eax, dx 
			cmp    ebx, 'VMXh' 
			setz[rc]

			pop    ebx
			pop    ecx
			pop    edx
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		rc = false;
	}



	return rc;
}
