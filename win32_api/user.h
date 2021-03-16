#pragma once
#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif
#define _WIN32_WINNT 0x0500
#include <WTypesbase.h>

class NameSid {
	LPTSTR name{ NULL };
	LPTSTR stringSid{ NULL };
public:
	NameSid(LPTSTR username, LPTSTR stringSid);
	BOOL getName(TCHAR buf[], DWORD size);
	BOOL getStringSid(LPTSTR& buf, DWORD size);
	void cleanUp();
};