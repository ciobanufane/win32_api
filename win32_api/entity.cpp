#include <strsafe.h>
#include <tchar.h>
#include "entity.h"

Entity::Entity(LPCTSTR name, LPCTSTR stringSid, DWORD flags) {

	if (!name) {
		throw "Name cannot be null";
	}

	this->name = (LPTSTR)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(*name) * (_tcslen(name) + 1));
	StringCchCopy(this->name, _tcslen(name) + 1, name);

	if (stringSid) {
		this->stringSid = (LPTSTR)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(*stringSid) * (_tcslen(stringSid) + 1));
		StringCchCopy(this->stringSid, _tcslen(stringSid) + 1, stringSid);
	}

	this->flags = flags;
}

LPCTSTR Entity::getName() {
	return (LPCTSTR)name;
}

LPCTSTR Entity::getStringSid() {
	return (LPCTSTR)stringSid;
}

DWORD Entity::getFlags() {
	return flags;
}

void Entity::cleanUp() {
	HeapFree(GetProcessHeap(), 0, name);
	if (!stringSid) {
		HeapFree(GetProcessHeap(), 0, stringSid);
	}
}

EntityScore::EntityScore(LPCTSTR name, LPCTSTR stringSid, DWORD flags, LPCTSTR newName, INT8 actionId, INT16 points) 
	: Entity(name, stringSid, flags){

	if (actionId < 0 || actionId > 3) {
		throw "Not a valid action";
	}
	if (actionId == 0 && !stringSid) {
		throw "Need to enter SID of user";
	}
	if (actionId == 3 && !newName) {
		throw "Need to define a new name";
	}

	if (newName) {
		this->newName = (LPTSTR)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(*newName) * (_tcslen(newName) + 1));
		StringCchCopy(this->newName, _tcslen(newName) + 1, newName);
	}
	this->actionId = actionId;
	this->points = points;
}

LPCTSTR EntityScore::getNewName() {
	return (LPCTSTR)newName;
}

INT8 EntityScore::getActionId() {
	return actionId;
}

INT16 EntityScore::getPoints() {
	return points;
}

void EntityScore::cleanUp() {
	Entity::cleanUp();
	if (!newName) {
		HeapFree(GetProcessHeap(), 0, newName);
	}
}
