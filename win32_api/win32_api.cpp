#pragma comment(lib, "netapi32.lib")

#include <iostream>
#include <windows.h>
#include <lm.h>
#include <sddl.h>
#include <tchar.h>
#include <strsafe.h>
#include "win32_api.h"

/*
	References
	code		: https://docs.microsoft.com/en-us/windows/win32/api/lmaccess/nf-lmaccess-netquerydisplayinformation
	datatypes	: https://docs.microsoft.com/en-us/windows/win32/winprog/windows-data-types
	naming		: https://docs.microsoft.com/en-us/windows/win32/stg/coding-style-conventions
*/

BOOL GetAccountSidFromName(LPCTSTR accountName, PSID sid, const DWORD sidSize) {
	// https://stackoverflow.com/questions/48103823/lookupaccountname-error-when-referenceddomainname-is-null

	// variables used for the LookupAccountName function
	SID_NAME_USE snu;
	DWORD cbSid = sidSize, cchRefDomainName = 0;
	LPTSTR refDomainName{ nullptr };
	BOOL bResult;

	bResult = LookupAccountName(nullptr, accountName, sid, &cbSid, refDomainName, &cchRefDomainName, &snu);
	if (!bResult) {

		// If the domain name is not passed, then the last error returned will be ERROR_INSUFFICIENT_BUFFER
		// Also, the function call will store the size of the domain name in cchRefDomainName
		if (GetLastError() != ERROR_INSUFFICIENT_BUFFER) {
			return FALSE;
		}

		// Allocate memory for the domain name
		refDomainName = (LPTSTR)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, cchRefDomainName * sizeof(*refDomainName));
		// Call LookupAccountName again but with space allocated for the domain name
		bResult = LookupAccountName(nullptr, accountName, sid, &cbSid, refDomainName, &cchRefDomainName, &snu);
		HeapFree(GetProcessHeap(), 0, refDomainName);
	}
	return bResult;
}

BOOL GetUsers(std::vector<Entity> & users) {
	// variables used for querying users
	PNET_DISPLAY_USER pBuf, pTmpBuf;
	DWORD result, dwEntriesRead, index = 0;

	// variables used for querying a user's SID
	BYTE sidbuf[SECURITY_MAX_SID_SIZE];
	LPTSTR pszStringSid;

	do {

		/*--------------------------------------------------------------------
		  nullptr	-> specifies DNS/NetBios of remote server or NULL for local computer
		  1			-> information level of requested data (1 = users, 2 = machine, 3 = groups)
		  i			-> current index of requested user
		  100		-> how many requested entries
		  MAX...	-> how many bytes allocated to buffer
		  &dw...	-> returns number of entries
		  pBuf		-> pointer to a buffer which is used to store the requested data
		--------------------------------------------------------------------*/
		result = NetQueryDisplayInformation(nullptr, 1, index, 100, MAX_PREFERRED_LENGTH, &dwEntriesRead, (PVOID*)&pBuf);
		if (result == ERROR_SUCCESS || result == ERROR_MORE_DATA) {
			pTmpBuf = pBuf;
			for (; dwEntriesRead > 0; --dwEntriesRead) {

				// Get SID of a user and stores it in a SID struct
				if (!GetAccountSidFromName(pTmpBuf->usri1_name, (PSID)sidbuf, sizeof(sidbuf))) {
					return FALSE;
				}

				// Converts the SID struct to a string SID
				if (!ConvertSidToStringSid((PSID)sidbuf, &pszStringSid)) {
					return FALSE;
				}

				users.push_back(Entity(pTmpBuf->usri1_name, pszStringSid, pTmpBuf->usri1_flags));

				// i is the next index of the user; used for the netquerydisplayinformation function call
				index = pTmpBuf->usri1_next_index;
				// moves onto the next address of the next net_display_user
				++pTmpBuf;
				// have to free up memory as per documentation for ConvertSidToStringSid
				LocalFree(pszStringSid);
			}
			// have to free up memory as per documentation for NetQueryDisplayInformation
			NetApiBufferFree(pBuf);
		}
		else {
			printf("Error: %u\n", result);
			return FALSE;
		}
	} while (result == ERROR_MORE_DATA);
	return TRUE;
}

BOOL GetGroups(std::vector<Entity> & groups) {
	PLOCALGROUP_INFO_1 pBuf, pTmpBuf;
	DWORD dwEntriesRead, totalEntries, resumeHandle = 0;
	NET_API_STATUS result;

	BYTE sidBuf[SECURITY_MAX_SID_SIZE];
	LPTSTR pszStringSid;

	do {
		result = NetLocalGroupEnum(nullptr, 1, (LPBYTE*)&pBuf, MAX_PREFERRED_LENGTH,
			&dwEntriesRead, &totalEntries, &resumeHandle);
		if (result == NERR_Success || result == ERROR_MORE_DATA) {
			pTmpBuf = pBuf;
			for (; dwEntriesRead > 0; --dwEntriesRead) {
				if (!GetAccountSidFromName(pTmpBuf->lgrpi1_name, (PSID)sidBuf, sizeof(sidBuf))) {
					return FALSE;
				}
				if (!ConvertSidToStringSid((PSID)sidBuf, &pszStringSid)) {
					return FALSE;
				}
				groups.push_back(Entity(pTmpBuf->lgrpi1_name, pszStringSid, 0));
				++pTmpBuf;
				LocalFree(pszStringSid);
			}
			NetApiBufferFree(pBuf);
		}
		else {
			printf("Error: %u\n", result);
			return FALSE;
		}

	} while (result == ERROR_MORE_DATA);
	return TRUE;
}

int amain() {
	std::vector<Entity> test;
	GetGroups(test);

	for (std::vector<Entity>::iterator it = test.begin(); it != test.end(); ++it) {
		wprintf(L"%s + %s\n", it->getName(), it->getStringSid());
		it->cleanUp();
	}

	return 0;
}
