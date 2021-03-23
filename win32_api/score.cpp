#include <algorithm>
#include <windows.h>
#include "score.h"
#include "win32_api.h"

void checkUserScores(std::vector<EntityScore> & scores, INT16 & points)
{

	int index;
	std::vector<Entity> users;

	if (!helper::testForDuplicateNames<EntityScore>(scores, helper::compareName)) {
		return;
	}

	GetUsers(users);
	std::sort(users.begin(), users.end(), helper::compareEntity<Entity>);

	for (std::vector<EntityScore>::iterator it = scores.begin(); it != scores.end(); ++it) {

		if ((it->getActionId() < 0 || it->getActionId() > 3) ||
			(it->getActionId() == 0 && !it->getStringSid()) ||
			(it->getActionId() == 3 && !it->getOptionalName())) {
			continue;
		}

		switch (it->getActionId()) {
			case 0: 
				// Don't score user / Don't remove user from system
				// Points for this score will be used to subtract from the total amount of points
				index = helper::searchForEntityName<Entity>(users, it->getName());
				if (index == -1 || lstrcmpi(users[index].getStringSid(),it->getStringSid()) != 0) {
					points -= it->getPoints();
				}
				break;
			case 1: 
				// Add user
				if (helper::searchForEntityName<Entity>(users, it->getName()) != -1) {
					points += it->getPoints();
				}
				break;
			case 2:
				// Delete user
				if (helper::searchForEntityName<Entity>(users, it->getName()) == -1) {
					points += it->getPoints();
				}
				break;
			case 3:
				// Rename user
				index = helper::searchForEntityName<Entity>(users, it->getOptionalName());
				if (index != -1 && lstrcmpi(users[index].getStringSid(), it->getStringSid()) == 0) {
					points += it->getPoints();
				}
				break;
		}
	}
}

void checkGroupScores(std::vector<EntityScore> & scores, INT16 & points) {


	int index;
	std::vector<Entity> groups;

	if (!helper::testForDuplicateNames<EntityScore>(scores, helper::compareName)) {
		return;
	}

	GetGroups(groups);
	std::sort(groups.begin(), groups.end(), helper::compareEntity<Entity>);

	for (std::vector<EntityScore>::iterator it = scores.begin(); it != scores.end(); ++it) {

		if ((it->getActionId() < 0 || it->getActionId() > 3) ||
			(it->getActionId() == 0 && !it->getStringSid()) ||
			(it->getActionId() == 3 && !it->getOptionalName())) {
			continue;
		}

		switch (it->getActionId()) {
			case 0: // Don't score group / Don't remove group from the system
				index = helper::searchForEntityName<Entity>(groups, it->getName());
				if (index == -1 || lstrcmpi(groups[index].getStringSid(), it->getStringSid()) != 0) {
					points -= it->getPoints();
				}
				break;
			case 1: // Add group
				if (helper::searchForEntityName<Entity>(groups, it->getName()) != -1) {
					points += it->getPoints();
				}
				break;
			case 2: // Delete group
				if (helper::searchForEntityName<Entity>(groups, it->getName()) == -1) {
					points += it->getPoints();
				}
				break;
			case 3: // Rename group
				index = helper::searchForEntityName<Entity>(groups, it->getOptionalName());
				if (index != -1 && lstrcmpi(groups[index].getStringSid(), it->getStringSid()) == 0) {
					points += it->getPoints();
				}
				break;
		}
	}

}

void checkGroupMembershipScores(std::vector<EntityScore>& scores, INT16& points) {
	int groupIndex;
	int memberIndex;
	std::vector<Entity> groups;
	std::vector<Entity> members;
	LPCTSTR prevGroupName{ nullptr };

	if (!helper::testForDuplicateNames<EntityScore>(scores, helper::compareName)) {
		return;
	}

	GetGroups(groups);
	std::sort(groups.begin(), groups.end(), helper::compareEntity<Entity>);

	for (std::vector<EntityScore>::iterator it = scores.begin(); it != scores.end(); ++it) {
		if ((it->getActionId() < 0 || it->getActionId() > 2) ||
			(it->getActionId() == 0 && !it->getStringSid()) ||
			(!it->getOptionalName())) {
			continue;
		}

		if (lstrcmpi(prevGroupName, it->getName()) != 0) {
			prevGroupName = it->getName();
			members.clear();
			GetGroupMemberships(members, it->getName());
			std::sort(members.begin(), members.end(), helper::compareEntity<Entity>);
		}

		memberIndex = helper::searchForEntityName<Entity>(members, it->getOptionalName());

		switch (it->getActionId()) {
			case 0: // Don't remove member from group
				if (memberIndex == -1) {
					points -= it->getPoints();
				}
				break;
			case 1: // Add member to group
				if (memberIndex != 1) {
					points += it->getPoints();
				}
				break;
			case 2: // Remove member from group
				if (memberIndex == -1) {
					points += it->getPoints();
				}
				break;
		}
	}
}

int main() {
	INT16 points{ 0 };
	std::vector<EntityScore> test;
	std::vector<Entity> test1;

	Entity t1 = Entity(L"test", nullptr, 0);
	test1.push_back(t1);

	EntityScore t2 = EntityScore(L"test", nullptr, 0, nullptr, 2, 10);
	test.push_back(t2);

	checkGroupScores(test, points);
	wprintf(L"%d\n", points);

	return 0;
}