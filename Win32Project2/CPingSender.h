#ifndef HG_CPINGSENDER
#define HG_CPINGSENDER
#include "common.h"
#include "CPacket.h"
#include <queue>
using namespace std;


class CPingSender
{
public:
	static void Initialize();
	static void Setup(int interval);
	static CPacket* GetFPPacket();

};

#endif

DWORD ProcessSrWindowMsg();
