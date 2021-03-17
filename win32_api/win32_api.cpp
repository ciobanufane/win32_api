#pragma comment(lib, "netapi32.lib")

#include <iostream>
#include <vector>
#include <windows.h>
#include <lm.h>
#include <sddl.h>
#include <tchar.h>
#include <strsafe.h>
#include "user.h"

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

BOOL GetUsers(std::vector<NameSid>& users) {
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

				users.push_back(NameSid(pTmpBuf->usri1_name, stringSid));

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

int main() {
	std::vector<NameSid> test;
	TCHAR buf1[256] = L"";
	TCHAR buf2[256] = L"";
	GetUsers(test);

	for (std::vector<NameSid>::iterator it = test.begin(); it != test.end(); ++it) {

		it->getName(buf1, 256);
		it->getStringSid(buf2, 256);
		wprintf(L"%s + %s\n", buf1, buf2);
		it->cleanUp();

		ZeroMemory(buf1, sizeof(buf1));
		ZeroMemory(buf2, sizeof(buf2));
	}

	return 0;
}
