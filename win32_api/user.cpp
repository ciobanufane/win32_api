
#include "user.h"
#include <strsafe.h>
#include <tchar.h>

NameSid::NameSid(LPCTSTR name, LPCTSTR stringSid) {
	this->name = (LPTSTR)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(*name) * (_tcslen(name) + 1));
	this->stringSid = (LPTSTR)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(*stringSid) * (_tcslen(stringSid) + 1));
	StringCchCopy(this->name, _tcslen(name) + 1, name);
	StringCchCopy(this->stringSid, _tcslen(stringSid) + 1, stringSid);
}

BOOL NameSid::getName(LPTSTR buf, DWORD & size) {
	if (SUCCEEDED(StringCchCopy(buf, size, name)) == S_OK) {
		return TRUE;
	}
	else {
		size = _tcslen(name) + 1;
		return FALSE;
	}
}

BOOL NameSid::getStringSid(LPTSTR buf, DWORD & size) {
	if (SUCCEEDED(StringCchCopy(buf, size, stringSid)) == S_OK) {
		return TRUE;
	}
	else {
		size = _tcslen(stringSid) + 1;
		return FALSE;
	}
}

void NameSid::cleanUp() {
	HeapFree(GetProcessHeap(), 0, name);
	HeapFree(GetProcessHeap(), 0, stringSid);
}

UserScore::UserScore(LPCTSTR name, LPCTSTR stringSid, LPCTSTR newName, INT8 actionId) : NameSid(name, stringSid){
	if (!newName) {
		this->newName = (LPTSTR)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(*newName) * (_tcslen(newName) + 1));
		StringCchCopy(this->newName, _tcslen(newName) + 1, newName);
	}
	this->actionId = actionId;
}

BOOL UserScore::getNewName(LPTSTR buf, DWORD & size) {
	if (SUCCEEDED(StringCchCopy(buf, size, newName)) == S_OK) {
		return TRUE;
	}
	else {
		size = _tcslen(newName) + 1;
		return FALSE;
	}
}

INT8 UserScore::getActionId() {
	return actionId;
}

void UserScore::cleanUp() {
	NameSid::cleanUp();
	if (!newName) {
		HeapFree(GetProcessHeap(), 0, newName);
	}
}
