#pragma once
#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif

#include <WTypesbase.h>

class User {
	LPTSTR name{ nullptr };
	LPTSTR stringSid{ nullptr };
	DWORD flags{ };
public:
	User(LPCTSTR name, LPCTSTR stringSid, DWORD flags);
	LPCTSTR getName();
	LPCTSTR getStringSid();
	void cleanUp();
};

class UserScore : public User {
	LPTSTR newName{ nullptr };
	INT8 actionId;
	INT16 points;
public:
	UserScore(LPCTSTR name, LPCTSTR stringSid, DWORD flags, LPCTSTR newName, INT8 actionId, INT16 points);
	LPCTSTR getNewName();
	INT8 getActionId();
	void cleanUp();
};
