#pragma once
#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif

#include <WTypesbase.h>

class Entity {
	LPTSTR name{ nullptr };
	LPTSTR stringSid{ nullptr };
	DWORD flags;

public:
	Entity(LPCTSTR name, LPCTSTR stringSid, DWORD flags);
	LPCTSTR getName();
	LPCTSTR getStringSid();
	DWORD getFlags();
	void cleanUp();
};

class EntityScore : public Entity {
	LPTSTR newName{ nullptr };
	INT8 actionId;
	INT16 points;

public:
	EntityScore(LPCTSTR name, LPCTSTR stringSid, DWORD flags, LPCTSTR newName, INT8 actionId, INT16 points);
	LPCTSTR getNewName();
	INT8 getActionId();
	INT16 getPoints();
	void cleanUp();
};
