#include "CPingSender.h"
#include "CMem.h"
#include "stream_utility.h"
#include "VTDetector.h"
#include "Utility.h"
#include "AES.h"

#define SEND_HOOK		0x8416D2
#define SEND_ECX		0xEECBF4
#define SEND_CALL		0x81E750
#define WNDPROC_HOOK	0x8311C4

const int wndHookTimerId = 0x1337;
int wndHookTimerInterval = 0;


BYTE* bpOriginalSendCode;

struct tWindowProc
{
	HWND hWnd;
	UINT uMsg;
	WPARAM wParam;
	LPARAM lParam;
} srWndProc;

DWORD dwWindowProc_Ret = 0;
DWORD dwWindowProc_Esp = 0;
DWORD dwWindowProc_Res = 0;
DWORD dwSendHook_RetAddr = 0;

BYTE* packetBytes		 = 0;
int packetBytesLength	 = 0;
int lifepacketsSend = 0;
bool started = true;


void WritePacketData(LPBYTE _edi)
{
	CPacket* pck = new CPacket(0x2002);
	if (started) {
		pck = CPingSender::GetFPPacket();
		started = false;
	}
	else {
		if (lifepacketsSend > 10) {
			lifepacketsSend = 0;
			pck = CPingSender::GetFPPacket();
		}
		else {
			lifepacketsSend++;
		}
	}


	packetBytes = pck->GetRawBytes();
	packetBytesLength = pck->GetRawSize();
	LPBYTE pData = (_edi + 0x34);
	//RAW
	memcpy(pData, packetBytes, packetBytesLength);

}


__declspec(naked) void SendHook_Codecave()
{
	__asm
	{
		pop dwSendHook_RetAddr;
		mov eax, [edx + 0x2C];

		pushad;					
		push edi;				
		call WritePacketData;	
		add esp, 4;
		popad;					

		push edi;
		call eax;
		push dwSendHook_RetAddr;
		ret;
	}
}

void Send()
{
	CMem::SetNops(SEND_HOOK, 6);
	CMem::SetDetour(E_DetourType::Call, SEND_HOOK, (DWORD)SendHook_Codecave);
	__asm
	{
		push 4;
		mov ecx, SEND_ECX;
		mov eax, SEND_CALL;
		call eax;
	}

	
	CMem::WriteBytes(SEND_HOOK, bpOriginalSendCode, 6);
}

DWORD ProcessSrWindowMsg()
{

	if(srWndProc.uMsg == WM_CREATE)
	{
		SetTimer(srWndProc.hWnd, wndHookTimerId, wndHookTimerInterval, NULL);
	}

	if(srWndProc.uMsg == WM_TIMER && srWndProc.wParam == wndHookTimerId)
	{
		Send();
		return 0;
	}

	return 1;
}

void CallMyWindowProcessor()
{
	memcpy(&srWndProc, ((LPBYTE)ULongToPtr(dwWindowProc_Esp)) + 4, 16);
	dwWindowProc_Res = ProcessSrWindowMsg();
}



__declspec(naked) void WindowProcHook_Codecave()
{
	__asm
	{
		pop dwWindowProc_Ret;
		mov dwWindowProc_Esp, esp;
		pushad;
		call CallMyWindowProcessor;
		//Ours
		cmp dwWindowProc_Res, 0;

		//dwWindowProc_Res == 0 ?
		jne LABEL_1;

		popad;
		//eax = 0
		xor eax, eax;
		ret 0x10;

LABEL_1:
		popad;
		cmp eax, 0x496;
		push dwWindowProc_Ret;
		ret;
	}
}



void CPingSender::Initialize()
{
	
	bpOriginalSendCode = CMem::ReadBytes(SEND_HOOK, 6); /* Original bytes */

	CMem::SetDetour(E_DetourType::Call, WNDPROC_HOOK, (DWORD)WindowProcHook_Codecave);
}




void CPingSender::Setup(int interval)
{
	wndHookTimerInterval = interval;
}

CPacket* CPingSender::GetFPPacket()
{
	CPacket* hwid = new CPacket(0x133D);
	hwid->WriteUInt16(1);
	hwid->WriteAscii("Hello son of bitch!");


	StreamUtility stream = StreamUtility();
	stream.WriteAscii(Utility::GetHwid());
	stream.Write<byte>(VTDetector::IsInsideVMWare());
	stream.Write<byte>(VTDetector::IsInsideVPC());
	stream.WriteAscii("B=!?(Su4m!(d7q859RSdYAQEb.(b(((#");


	std::vector<byte> rawbytes = stream.GetStreamVector();

	std::vector<byte> bytes = Sheriff::AES::encrypt(rawbytes);


	hwid->WriteBytesCR(bytes.data(), bytes.size());
	return hwid;
}