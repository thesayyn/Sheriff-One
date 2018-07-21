#include "CMem.h"

//Please, note that in some cases you have to write extra nops
void CMem::SetDetour(E_DetourType type, DWORD dwFrom, DWORD dwTo)
{
	//Built detour instruction
	BYTE instruction[5];
	
	if(type == E_DetourType::Jmp)
		instruction[0] = ASM_JMP;

	if(type == E_DetourType::Call)
		instruction[0] = ASM_CALL;

	*(DWORD*) (instruction + 1) = (dwTo - (dwFrom + 5));

	//Write instruction
	DWORD dwOldProtect = 0;
	VirtualProtect((LPVOID)dwFrom, 5, PAGE_EXECUTE_READWRITE, &dwOldProtect);
	memcpy((LPVOID)dwFrom, instruction, 5);
	VirtualProtect((LPVOID)dwFrom, 5, dwOldProtect, &dwOldProtect);
}

void CMem::SetBytes(DWORD dwDest, BYTE value, int count)
{
	//Write instruction
	DWORD dwOldProtect = 0;
	VirtualProtect((LPVOID)dwDest, 5, PAGE_EXECUTE_READWRITE, &dwOldProtect);
	memset((LPVOID)dwDest, value, count);
	VirtualProtect((LPVOID)dwDest, 5, dwOldProtect, &dwOldProtect);
}


void CMem::WriteBytes(DWORD dwDest, BYTE* data, int count)
{
	//Write instruction
	DWORD dwOldProtect = 0;
	VirtualProtect((LPVOID)dwDest, 5, PAGE_EXECUTE_READWRITE, &dwOldProtect);
	memcpy((void*) dwDest, data, count);
	VirtualProtect((LPVOID)dwDest, 5, dwOldProtect, &dwOldProtect);
}

void CMem::SetNops(DWORD dwDest, int count)
{
	SetBytes(dwDest, ASM_NOP, count);
}

BYTE* CMem::ReadBytes(DWORD dwFrom, int count)
{
	BYTE* buffer = new BYTE[count];

	DWORD dwOldProtect = 0;
	VirtualProtect((LPVOID)dwFrom, 5, PAGE_EXECUTE_READWRITE, &dwOldProtect);
	memcpy(buffer, (LPVOID)dwFrom, count);
	VirtualProtect((LPVOID)dwFrom, 5, dwOldProtect, &dwOldProtect);

	return buffer;
}

