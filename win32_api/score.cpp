#include <algorithm>
#include <windows.h>
#include "score.h"
#include "win32_api.h"

template <class T>
bool compareEntity(T u1, T u2) {
	return lstrcmpi(u1.getName(), u2.getName()) < 0 ? true : false;
}

bool compareName(LPCTSTR n1, LPCTSTR n2) {
	return lstrcmpi(n1, n2) < 0 ? true : false;
}

static int searchForEntityName(std::vector<Entity> & entities, LPCTSTR name) {
	unsigned int length{ entities.size() };
	unsigned int left{ 0 };
	unsigned int right{ length-1 };
	unsigned int midpoint;
	int res;

	if (length == 0) {
		return -1;
	}

	while (left <= right) {
		midpoint = left + (right - left) / 2;
		res = lstrcmpi(entities[midpoint].getName(), name);
		if (res == 0) {
			return midpoint;
		}
		else if (res > 0) {
			right = midpoint-1;
		}
		else {
			left = midpoint+1;
		}
	}
	return -1;
}

static bool testEntityScoresForDuplicateName(std::vector<EntityScore> & scores) {
	unsigned int index{ 0 };
	std::vector<LPCTSTR> copy;

	for (std::vector<EntityScore>::iterator it = scores.begin(); it != scores.end(); ++it) {
		copy.push_back(it->getName());
		if (it->getNewName()) {
			copy.push_back(it->getNewName());
		}
	}

	std::sort(copy.begin(), copy.end(), compareName);
	
	// Don't need to check the last element
	for (; index < copy.size() - 1; ++index) {
		if (lstrcmpi(copy[index], copy[index + 1]) == 0) {
			return false;
		}
	}

	return true;
	
}

void checkUserScores(std::vector<EntityScore> & scores, INT16 & points)
{
	/*
		need to add messages to each case
	*/

	int index;
	std::vector<Entity> users;

	if (!testEntityScoresForDuplicateName(scores)) {
		return;
	}

	GetUsers(users);
	std::sort(users.begin(), users.end(), compareEntity<Entity>);

	for (std::vector<EntityScore>::iterator it = scores.begin(); it != scores.end(); ++it) {
		switch (it->getActionId()) {
			case 0: 
				// Don't score user / Don't remove user from system
				// Points for this score will be used to subtract from the total amount of points
				index = searchForEntityName(users, it->getName());
				if (index == -1 || lstrcmpi(users[index].getStringSid(),it->getStringSid()) != 0) {
					points -= it->getPoints();
				}
				break;
			case 1: 
				// Add user
				if (searchForEntityName(users, it->getName()) != -1) {
					points += it->getPoints();
				}
				break;
			case 2:
				// Delete user
				if (searchForEntityName(users, it->getName()) == -1) {
					points += it->getPoints();
				}
				break;
			case 3:
				// Rename user
				index = searchForEntityName(users, it->getNewName());
				if (index != -1 && lstrcmpi(users[index].getStringSid(), it->getStringSid()) == 0) {
					points += it->getPoints();
				}
				break;
		}
		it->cleanUp();
	}
}

void checkGroupScores(std::vector<EntityScore> & scores, INT16 & points) {

	/*
		need to add messages to each case
	*/

	int index;
	std::vector<Entity> groups;

	if (!testEntityScoresForDuplicateName(scores)) {
		return;
	}

	GetGroups(groups);
	std::sort(groups.begin(), groups.end(), compareEntity<Entity>);

	for (std::vector<EntityScore>::iterator it = scores.begin(); it != scores.end(); ++it) {
		switch (it->getActionId()) {
			case 0: // Don't score group / Don't remove group from the system
				index = searchForEntityName(groups, it->getName());
				if (index == -1 || lstrcmpi(groups[index].getStringSid(), it->getStringSid()) != 0) {
					points -= it->getPoints();
				}
				break;
			case 1: // Add group
				if (searchForEntityName(groups, it->getName()) != -1) {
					points += it->getPoints();
				}
				break;
			case 2: // Delete group
				if (searchForEntityName(groups, it->getName()) == -1) {
					points += it->getPoints();
				}
				break;
			case 3: // Rename group
				index = searchForEntityName(groups, it->getNewName());
				if (index != -1 && lstrcmpi(groups[index].getStringSid(), it->getStringSid()) == 0) {
					points += it->getPoints();
				}
				break;
		}
		it->cleanUp();
	}

}

int main() {
	INT16 points{ 0 };
	std::vector<EntityScore> test;

	test.push_back(EntityScore(L"test", nullptr, 0, nullptr, 2, 10));

	checkGroupScores(test, points);
	wprintf(L"%d\n", points);

	return 0;
}