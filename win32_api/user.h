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
	NameSid(LPCTSTR name, LPCTSTR stringSid);
	BOOL getName(LPTSTR buf, DWORD & size);
	BOOL getStringSid(LPTSTR buf, DWORD & size);
	void cleanUp();
};

class UserScore : public NameSid {
	LPTSTR newName{ nullptr };
	INT8 actionId;
public:
	UserScore(LPCTSTR name, LPCTSTR stringSid, LPCTSTR newName, INT8 actionId);
	BOOL getNewName(LPTSTR buf, DWORD & size);
	INT8 getActionId();
	void cleanUp();
};
