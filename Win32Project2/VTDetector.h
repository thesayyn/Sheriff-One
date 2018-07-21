#pragma once
#include <windows.h>

class VTDetector
{
public:
	static bool IsInsideVPC();
	static bool IsInsideVMWare();

private :
	static DWORD IsInsideVPC_exceptionFilter(LPEXCEPTION_POINTERS ep);
};


