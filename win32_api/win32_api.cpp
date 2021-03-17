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
	BOOL success;

	success = LookupAccountName(nullptr, accountName, sid, &cbSid, refDomainName, &cchRefDomainName, &snu);
	if (!success) {

		// If the domain name is not passed, then the last error returned will be ERROR_INSUFFICIENT_BUFFER
		// Also, the function call will store the size of the domain name in cchRefDomainName
		if (GetLastError() != ERROR_INSUFFICIENT_BUFFER) {
			return FALSE;
		}

		// Allocate memory for the domain name
		refDomainName = (LPTSTR)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, cchRefDomainName * sizeof(*refDomainName));
		// Call LookupAccountName again but with space allocated for the domain name
		success = LookupAccountName(nullptr, accountName, sid, &cbSid, refDomainName, &cchRefDomainName, &snu);
		HeapFree(GetProcessHeap(), 0, refDomainName);
	}
	return success;
}

BOOL GetUsers(std::vector<User>& users) {
	// variables used for querying users
	PNET_DISPLAY_USER pBuf, pTmpBuf;
	DWORD res, dwEntryCnt, i = 0;

	// variables used for querying a user's SID
	BYTE sidbuf[SECURITY_MAX_SID_SIZE];
	LPTSTR stringSid;

	do {

		// Function call to query for all users on the local computer
		res = NetQueryDisplayInformation(nullptr, 1, i, 100, MAX_PREFERRED_LENGTH, &dwEntryCnt, (PVOID*)&pBuf);
		if (res == ERROR_SUCCESS || res == ERROR_MORE_DATA) {
			pTmpBuf = pBuf;
			for (; dwEntryCnt > 0; --dwEntryCnt) {

				// Get SID of a user and stores it in a SID struct
				if (!GetAccountSidFromName(pTmpBuf->usri1_name, (PSID)sidbuf, sizeof(sidbuf))) {
					return FALSE;
				}

				// Converts the SID struct to a string SID
				if (!ConvertSidToStringSid((PSID)sidbuf, &stringSid)) {
					return FALSE;
				}

				users.push_back(User(pTmpBuf->usri1_name, stringSid, pTmpBuf->usri1_flags));

				// i is the next index of the user; used for the netquerydisplayinformation function call
				i = pTmpBuf->usri1_next_index;
				// moves onto the next address of the next net_display_user
				pTmpBuf++;
				// have to free up memory as per documentation for ConvertSidToStringSid
				LocalFree(stringSid);
			}
			// have to free up memory as per documentation for NetQueryDisplayInformation
			NetApiBufferFree(pBuf);
		}
		else {
			printf("Error: %u\n", res);
		}
	} while (res == ERROR_MORE_DATA);
	return TRUE;
}

int bmain() {
	std::vector<User> test;
	GetUsers(test);

	for (std::vector<User>::iterator it = test.begin(); it != test.end(); ++it) {
		wprintf(L"%s + %s\n", it->getName(), it->getStringSid());
		it->cleanUp();
	}

	return 0;
}
