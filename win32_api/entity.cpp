#include <strsafe.h>
#include <tchar.h>
#include "entity.h" // algorithm, vector, and WTypesBase

void Entity::initialize(LPCTSTR name, LPCTSTR stringSid, DWORD flags) {

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

Entity::Entity(LPCTSTR name, LPCTSTR stringSid, DWORD flags) {
	initialize(name, stringSid, flags);
}

Entity::Entity(const Entity& e){
	initialize(e.getName(), e.getStringSid(), e.getFlags());
}

Entity& Entity::operator=(const Entity& e){
	initialize(e.getName(), e.getStringSid(), e.getFlags());
	return *this;
}

LPCTSTR Entity::getName() const {
	return (LPCTSTR)name;
}

LPCTSTR Entity::getStringSid() const {
	return (LPCTSTR)stringSid;
}

DWORD Entity::getFlags() const {
	return flags;
}

Entity::~Entity() {
	HeapFree(GetProcessHeap(), 0, name);
	if (stringSid) {
		HeapFree(GetProcessHeap(), 0, stringSid);
	}
}

void EntityScore::initialize(LPCTSTR optionalName, INT8 actionId, INT16 points) {

	if (optionalName) {
		this->optionalName = (LPTSTR)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(*optionalName) * (_tcslen(optionalName) + 1));
		StringCchCopy(this->optionalName, _tcslen(optionalName) + 1, optionalName);
	}

	this->actionId = actionId;
	this->points = points;
}

EntityScore::EntityScore(LPCTSTR name, LPCTSTR stringSid, DWORD flags, LPCTSTR optionalName, INT8 actionId, INT16 points)
	: Entity(name, stringSid, flags){
	initialize(optionalName, actionId, points);
}

EntityScore::EntityScore(const EntityScore& es) : Entity(es.getName(), es.getStringSid(), es.getFlags()) {
	initialize(es.getOptionalName(), es.getActionId(), es.getPoints());
}

EntityScore& EntityScore::operator=(const EntityScore& es) {
	initialize(es.getOptionalName(), es.getActionId(), es.getPoints());
	return *this;
}

LPCTSTR EntityScore::getOptionalName() const {
	return (LPCTSTR)optionalName;
}

INT8 EntityScore::getActionId() const {
	return actionId;
}

INT16 EntityScore::getPoints() const {
	return points;
}

EntityScore::~EntityScore() {
	if (optionalName) {
		HeapFree(GetProcessHeap(), 0, optionalName);
	}
}

