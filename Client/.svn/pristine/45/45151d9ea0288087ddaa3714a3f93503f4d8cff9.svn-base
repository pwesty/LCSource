//#include "stdH.h"
#include "Access_control.h"
#include <Shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

//////////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////////

/*static*/
BOOL XAccessControl::mbSecLowSAInit = FALSE;
BOOL XAccessControl::mbSecSAInit = FALSE;
BOOL XAccessControl::mbSecSDInit = FALSE;
BOOL XAccessControl::mbSecNameEnabled = FALSE;
//fn_ConvertStringSecurityDescriptorToSecurityDescriptorW* pfnConvertStringSecurityDescriptorToSecurityDescriptorW = NULL;

#define SDDL_REVISION_1		1

SECURITY_ATTRIBUTES* XAccessControl::getLowPrivSA()
{
	static SECURITY_ATTRIBUTES gsLowSA;

	if (!mbSecLowSAInit)
	{
		gsLowSA.nLength = sizeof(SECURITY_ATTRIBUTES);
		gsLowSA.bInheritHandle = FALSE;

		/* Example
		TCHAR* szSD = TEXT("D:") // Discretionary ACL
			TEXT("(D;OICI;GA;;;BG)")	// Deny access to built-in guests
			TEXT("(D;OICI;GA;;;AN)")	// Deny access to anonymous logon
			TEXT("(A;OICI;GRGWGX;;;AU)")	// Allow read/write/execute to authenticated users
			TEXT("(A;OICI;GA;;;BA)");		// Allow full control to administrators
		*/

		wchar_t* wszSD = L"O:"	// Discretionary ACL
			L"(A;OICI;GAFA;;;BA)"	// allow full control to administrators
			L"(A;OICI;GAFA;;;SY)";	// Allow full control to SYSTEM

		fn_ConvertStringSecurityDescriptorToSecurityDescriptorW* pfn = GetSecurityFuncAddress(); // 함수 포인터를 가져온다.

		if (pfn)
		{
			mbSecLowSAInit = pfn(wszSD, SDDL_REVISION_1, &gsLowSA.lpSecurityDescriptor, NULL);
		}
		else
		{
			gsLowSA.lpSecurityDescriptor = NULL;
		}
	}
	return &gsLowSA;
}

SECURITY_ATTRIBUTES* XAccessControl::getHighPrivSA()
{
	static SECURITY_ATTRIBUTES gsHighSA;

	if (!mbSecSAInit)
	{
		gsHighSA.nLength = sizeof(SECURITY_ATTRIBUTES);
		gsHighSA.bInheritHandle = FALSE;

		/*
		The format is a null-terminated string with tokens to indicate each of the four main components of a security
		descriptor: owner (0:), primary group (G:), DACL (D:), and SACL (S:)
		*/

		wchar_t* wszSD = L"D:"		// Discretionary ACL
			L"(A;OICI;GAFA;;;BA)"	// Allow full control to administrators
			L"(A;OICI;GAFA;;;SY)";	// Allow full control to SYSTEM

		fn_ConvertStringSecurityDescriptorToSecurityDescriptorW* pfn = GetSecurityFuncAddress();

		if (pfn)
		{
			mbSecSAInit = pfn(wszSD, SDDL_REVISION_1, &gsHighSA.lpSecurityDescriptor, NULL);
		}
		else
		{
			gsHighSA.lpSecurityDescriptor = NULL;
		}
	}
	return &gsHighSA;
}

SECURITY_DESCRIPTOR* XAccessControl::getHighPrivSD()
{
	static LPVOID psd = NULL;
	wchar_t* wszSD = L"D:"	// Discretionary ACL
		L"(A;OICI;GAFA;;;BA)"	// Allow full control to administrators
		L"(A;OICI;GAFA;;;SY)";	// Allow full control to SYSTEM

	if (!mbSecSDInit)
	{
		DWORD nSize;

		fn_ConvertStringSecurityDescriptorToSecurityDescriptorW* pfn = GetSecurityFuncAddress();

		if (pfn)
		{
			mbSecSDInit = pfn(wszSD, SDDL_REVISION_1, &psd, &nSize);
		}
		else
		{
			return NULL;
		}
	}
	return (SECURITY_DESCRIPTOR*)psd;
}

BOOL XAccessControl::SetPrivilege(
								  HANDLE hToken,	// access token handle
								  LPCTSTR lpszPrivilege,	// name of privilege to enable/disable
								  BOOL bEnablePrivilege		// to enable or disable privilege
								  )
{
	TOKEN_PRIVILEGES tp;
	LUID luid;

	if (!LookupPrivilegeValue(
		NULL,	// lookup privilege on local system
		lpszPrivilege,	// privilege to lookup
		&luid	// receives LUID of privilege
		))
	{
		return FALSE;
	}

	tp.PrivilegeCount = 1;
	tp.Privileges[0].Luid = luid;

	if (bEnablePrivilege)
		tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;		// 특권을 부여하는 속성 설정
	else
		tp.Privileges[0].Attributes = 0;

	// Enable the privilege or disable all privileges

	BOOL bRet = AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(TOKEN_PRIVILEGES), (PTOKEN_PRIVILEGES)NULL, (PDWORD)NULL);

	// Call GatLastError to determine whether the function succeeded.
	if (!bRet || GetLastError() != ERROR_SUCCESS)
	{
		return FALSE;
	}

	return TRUE;
}

/*
The ConvertStringSecurityDescriptorToSecurityDescriptor function converts a string-format security descriptor into
a valid, functional security descriptor. This function retrieves a security descriptor that
the ConvertSecurityDescriporToStringSecurityDescriptor function converted to string format.
*/
fn_ConvertStringSecurityDescriptorToSecurityDescriptorW* XAccessControl::GetSecurityFuncAddress()
{
	HMODULE hModule = NULL;
	fn_ConvertStringSecurityDescriptorToSecurityDescriptorW* pfnConvertStringSecurityDescriptorToSecurityDescriptorW = NULL;

	if (pfnConvertStringSecurityDescriptorToSecurityDescriptorW==NULL)
	{
		hModule = LoadLibrary("ADVAPI32.DLL");

		if (hModule == NULL)
		{
			return NULL;
		}

		pfnConvertStringSecurityDescriptorToSecurityDescriptorW = (fn_ConvertStringSecurityDescriptorToSecurityDescriptorW*)
			GetProcAddress(hModule, "ConvertStringSecurityDescriptorToSecurityDescriptorW");
	}

	if (hModule) FreeLibrary(hModule);
	return pfnConvertStringSecurityDescriptorToSecurityDescriptorW;
}

BOOL XAccessControl::EnableSecurityPriv()
{
	HANDLE hTokenSelf = NULL;
	BOOL bRet = TRUE;
//	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, GetCurrentProcessId());

//	if (!hProcess)
//	{
//		bRet = FALSE;
//	}

	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ALL_ACCESS, &hTokenSelf))
	{
		bRet = FALSE;
	}

	if (!SetPrivilege(hTokenSelf, SE_SECURITY_NAME, TRUE))
	{
		bRet = FALSE;
	}

//	if (hProcess) CloseHandle(hProcess);
	if (hTokenSelf) CloseHandle(hTokenSelf);
	return bRet;
}

void XAccessControl::GoThroughDir(const char* strPath, fnVisitFunc* visit, void* para)
{
	if (!strPath) return;

	HANDLE hSearch;
	BOOL bFinish = FALSE;
	WIN32_FIND_DATA FindData;
	char strSearchPath[MAX_PATH];

	memset(strSearchPath, 0, MAX_PATH);
	strncpy(strSearchPath, strPath, MAX_PATH-1);

	if (FALSE == PathFileExists(strPath)) return;

	visit(strPath, para);
	PathAppend(strSearchPath, "*.*"); // strSearchPath + "*.*"

	hSearch = FindFirstFile(strSearchPath, &FindData);

	if (hSearch != INVALID_HANDLE_VALUE)
	{
		while (!bFinish)
		{// 경로상의 파일 데이터 검색
			if (FindNextFile(hSearch, &FindData))
			{
				if ((FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
				{
					char strFullPathName[MAX_PATH];

					memset(strFullPathName, 0, MAX_PATH);

					if (!strcmp(FindData.cFileName, ".")) continue;
					if (!strcmp(FindData.cFileName, "..")) continue;
					
					strncpy(strFullPathName, strPath, MAX_PATH-1);

					PathAppend(strFullPathName, FindData.cFileName);
					(*visit)(strFullPathName, para);
					GoThroughDir(strFullPathName, visit, para);
				}
				else
				{
					char strFullFileName[MAX_PATH];

					memset(strFullFileName, 0, MAX_PATH);
					strncpy(strFullFileName, strPath, MAX_PATH-1);
					PathAppend(strFullFileName, FindData.cFileName);
					(*visit)(strFullFileName, para);
				}
			}
			else if (GetLastError() == ERROR_NO_MORE_FILES)
			{
				bFinish = TRUE;
			}
			else
			{
				FindClose(hSearch);
				return;
			}
		}
	}
	FindClose(hSearch);
}

BOOL XAccessControl::SetFileSecurityInDir(const char* szDirFileName, BOOL bRecursive/* =TRUE */)
{
	if (!szDirFileName) return FALSE;

	if (!mbSecNameEnabled)
	{
		mbSecNameEnabled = EnableSecurityPriv();
		
		if (!mbSecNameEnabled) return FALSE;
	}

	BOOL bRet = TRUE;

	if (bRecursive == FALSE)
	{
		bRet = SetSingleFileSecurity(szDirFileName, getHighPrivSD());
	}
	else
	{
		char szFullName[MAX_PATH] = {0};
		char szCurrentPath[MAX_PATH] = {0};

		GetFullPathName(szDirFileName, MAX_PATH, szFullName, NULL);
		// save current directory
		DWORD nChar = GetCurrentDirectory(MAX_PATH, szCurrentPath);
		GoThroughDir(szFullName, SetSingleFileSecurity, getHighPrivSD());
		// restore current directory
		if (nChar) SetCurrentDirectory(szCurrentPath);
	}

	return bRet;
}

BOOL XAccessControl::SetSingleFileSecurity(const char* szFileName, void* psd/* =NULL */)
{
	if (!szFileName) return FALSE;

	if (!mbSecNameEnabled)
	{
		mbSecNameEnabled = EnableSecurityPriv();

		if (!mbSecNameEnabled) return FALSE;
	}

	if (!PathFileExists(szFileName)) return TRUE;

	if (psd==NULL)
	{
		psd = getHighPrivSD();
	}

	BOOL bRet = ::SetFileSecurity(szFileName, DACL_SECURITY_INFORMATION, (SECURITY_DESCRIPTOR*)psd);

	return bRet;
}