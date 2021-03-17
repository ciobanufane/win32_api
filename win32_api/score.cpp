#include <algorithm>
#include <windows.h>
#include <iostream>
#include "score.h"
#include "win32_api.h"

bool compareUser(User u1, User u2) {
	return lstrcmpi(u1.getName(), u2.getName()) < 0 ? true : false;
}

int binary_search(std::vector<User> users, LPCTSTR username) {
	unsigned int length{ users.size() };
	unsigned int left{ 0 };
	unsigned int right{ length-1 };
	unsigned int midpoint;
	int res;

	if (length == 0) {
		return -1;
	}

	while (left <= right) {
		midpoint = left + (right - left) / 2;
		res = lstrcmpi(users[midpoint].getName(), username);
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
//std::vector<UserScore> scores, INT16 & points
void CHECK_SCORES()
{
	std::vector<User> users;
	GetUsers(users);
	std::sort(users.begin(), users.end(), compareUser);

	for (std::vector<User>::iterator it = users.begin(); it != users.end(); ++it) {
		wprintf(L"%s\n", it->getName());
	}
	/*
	for (std::vector<UserScore>::iterator it = scores.begin(); it != scores.end(); ++it) {
		switch (it->getActionId()) {
			case 0:
				break;
			case 1:
				break;
			case 2:
				break;
			case 3:
				break;
		}
	}
	*/
}

int main() {
	CHECK_SCORES();
}