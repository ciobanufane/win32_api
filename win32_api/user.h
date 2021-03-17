#pragma once
#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif
#undef _WIN32_WINNT
#define _WIN32_WINNT 0x0500

#include <WTypesbase.h>

class NameSid {
	LPTSTR name{ nullptr };
	LPTSTR stringSid{ nullptr };
public:
	NameSid(LPCTSTR username, LPCTSTR userStringSid);
	BOOL getName(LPTSTR buf, DWORD size);
	BOOL getStringSid(LPTSTR buf, DWORD size);
	void cleanUp();
};

class UserScore : public NameSid {
	LPTSTR newName{ NULL };
	INT8 actionId;
public:
	UserScore(LPCTSTR username, LPCTSTR userStringSid, LPCTSTR newName, INT8 actionId);
	BOOL getNewName();
	INT8 getActionId();
	void cleanUp();
};
