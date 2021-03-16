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
	NameSid(LPCTSTR username, LPCTSTR stringSid);
	BOOL getName(LPTSTR buf, DWORD size);
	BOOL getStringSid(LPTSTR buf, DWORD size);
	void cleanUp();
};

class UserScore : public NameSid {
	LPTSTR newName{ NULL };
	INT8 actionId;
public:
};
