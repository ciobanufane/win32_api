#pragma once
#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif
#undef _WIN32_WINNT
#define _WIN32_WINNT 0x0500

#include <WTypesbase.h>
#include <vector>
#include "user.h"

BOOL GetAccountSidFromName(LPCTSTR accountName, PSID sid, const DWORD sidSize);
BOOL GetUsers(std::vector<User>& users);