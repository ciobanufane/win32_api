#ifndef ENTITY_H
#define ENTITY_H

#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif

#include <vector>
#include <WTypesbase.h>

class Entity {
	LPTSTR name{ nullptr };
	LPTSTR stringSid{ nullptr };
	DWORD flags;
	std::vector<Entity> memberList;

	void initialize(LPCTSTR name, LPCTSTR stringSid, DWORD flags);

public:
	Entity(LPCTSTR name, LPCTSTR stringSid, DWORD flags);
	Entity(const Entity& e);
	Entity& operator=(const Entity& e);
	~Entity();

	LPCTSTR getName() const;
	LPCTSTR getStringSid() const;
	DWORD getFlags() const;

	void addMember(Entity member);
};

class EntityScore : public Entity {
	LPTSTR newName{ nullptr };
	INT8 actionId;
	INT16 points;

	void initialize(LPCTSTR name, LPCTSTR stringSid, DWORD flags, LPCTSTR newName, INT8 actionId, INT16 points);

public:
	EntityScore(LPCTSTR name, LPCTSTR stringSid, DWORD flags, LPCTSTR newName, INT8 actionId, INT16 points);
	EntityScore(const EntityScore& es);
	EntityScore& operator=(const EntityScore& es);
	~EntityScore();
	LPCTSTR getNewName() const;
	INT8 getActionId() const;
	INT16 getPoints() const;
};

#endif