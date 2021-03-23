#ifndef HELPER_FUNCTIONS_H
#define HELPER_FUNCTIONS_H

#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif

#include <algorithm>
#include <vector>
#include <WTypesbase.h>

namespace helper {

	inline bool compareName(LPCTSTR psz1, LPCTSTR psz2) {
		return lstrcmpi(psz1, psz2) < 0 ? true : false;
	}

	template <typename T>
	bool compareEntity(T e1, T e2) {
		return lstrcmpi(e1.getName(), e2.getName()) < 0 ? true : false;
	}

	template <typename T>
	int searchForEntityName(std::vector<T>& entities, LPCTSTR name) {
		unsigned int length{ entities.size() };
		unsigned int left{ 0 };
		unsigned int right{ length - 1 };
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
				right = midpoint - 1;
			}
			else {
				left = midpoint + 1;
			}
		}
		return -1;
	}

	template <typename T, typename Compare>
	bool testForDuplicateNames(std::vector<T>& scores, Compare comparator) {
		unsigned int index{ 0 };
		std::vector<LPCTSTR> copy;

		for (typename std::vector<T>::iterator it = scores.begin(); it != scores.end(); ++it) {
			copy.push_back(it->getOptionalName());
			if (it->getOptionalName()) {
				copy.push_back(it->getOptionalName());
			}
		}

		std::sort(copy.begin(), copy.end(), comparator);

		// Don't need to check the last element
		for (; index < copy.size() - 1; ++index) {
			if (lstrcmpi(copy[index], copy[index + 1]) == 0) {
				return false;
			}
		}

		return true;
	}
}
#endif
