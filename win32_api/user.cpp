#include <strsafe.h>
#include <tchar.h>
#include "user.h"

User::User(LPCTSTR name, LPCTSTR stringSid, DWORD flags) {
	this->name = (LPTSTR)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(*name) * (_tcslen(name) + 1));
	this->stringSid = (LPTSTR)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(*stringSid) * (_tcslen(stringSid) + 1));
	StringCchCopy(this->name, _tcslen(name) + 1, name);
	StringCchCopy(this->stringSid, _tcslen(stringSid) + 1, stringSid);

	this->flags = flags;
}

LPCTSTR User::getName() {
	return (LPCTSTR)this->name;
}

LPCTSTR User::getStringSid() {
	return (LPCTSTR)this->stringSid;
}

void User::cleanUp() {
	HeapFree(GetProcessHeap(), 0, name);
	HeapFree(GetProcessHeap(), 0, stringSid);
}

UserScore::UserScore(LPCTSTR name, LPCTSTR stringSid, DWORD flags, LPCTSTR newName, INT8 actionId, INT16 points) 
	: User(name, stringSid, flags){
	if (!newName) {
		this->newName = (LPTSTR)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(*newName) * (_tcslen(newName) + 1));
		StringCchCopy(this->newName, _tcslen(newName) + 1, newName);
	}
	this->actionId = actionId;
	this->points = points;
}

LPCTSTR UserScore::getNewName() {
	return (LPCTSTR)this->newName;
}

INT8 UserScore::getActionId() {
	return actionId;
}

void UserScore::cleanUp() {
	User::cleanUp();
	if (!newName) {
		HeapFree(GetProcessHeap(), 0, newName);
	}
}
