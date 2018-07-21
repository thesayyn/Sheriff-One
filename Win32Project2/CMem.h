#ifndef HG_CMEM
#define HG_CMEM
#include "common.h"

enum E_DetourType
{
	Call,
	Jmp
};

//Internal
class CMem
{
public:
	static void SetDetour(E_DetourType type, DWORD dwFrom, DWORD dwTo);
	static void SetBytes(DWORD dwDest, BYTE value, int count);
	static void WriteBytes(DWORD dwDest, BYTE* data, int count);

	//SetBytes(dwDest, ASM_NOP, count); 
	static void SetNops(DWORD dwDest, int count);

	static BYTE* ReadBytes(DWORD dwFrom, int count);

};
#endif