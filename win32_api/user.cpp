
#include "user.h"
#include <strsafe.h>
#include <tchar.h>

NameSid::NameSid(LPCTSTR name, LPCTSTR userStringSid) {
	this->name = (LPTSTR)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(*name) * (_tcslen(name) + 1));
	this->stringSid = (LPTSTR)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(*userStringSid) * (_tcslen(userStringSid) + 1));
	StringCchCopy(this->name, _tcslen(name) + 1, name);
	StringCchCopy(this->stringSid, _tcslen(userStringSid) + 1, userStringSid);
}

BOOL NameSid::getName(LPTSTR buf, DWORD size) {
	StringCchCopy(buf, size, this->name);
	return TRUE;
}

BOOL NameSid::getStringSid(LPTSTR buf, DWORD size) {
	StringCchCopy(buf, size, this->stringSid);
	return TRUE;
}

void NameSid::cleanUp() {
	HeapFree(GetProcessHeap(), 0, this->name);
	HeapFree(GetProcessHeap(), 0, this->stringSid);
}
