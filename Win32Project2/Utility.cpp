#include "utility.h"
#include "md5.h"


std::string Utility::GetHwid()
{

	HW_PROFILE_INFO hwProfileInfo;

	if (GetCurrentHwProfile(&hwProfileInfo) != NULL)
	{
		std::wstring ws(hwProfileInfo.szHwProfileGuid);
		std::string hwid(ws.begin(), ws.end());
		std::stringstream strm;
		strm << hwid << (2.2/4) <<hwid;
		return md5(strm.str());

	}
	return std::string("");
}

