#ifndef _ACCESS_CONTROL_H_
#define _ACCESS_CONTROL_H_
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>
//※ 시스템 그룹에 속한 객체를 인증된 사용자들이 접근할 수 있는 SD생성
typedef BOOL (WINAPI fn_ConvertStringSecurityDescriptorToSecurityDescriptorW) (
																			   LPCWSTR, // security descriptor string
																			   DWORD,	// revision level
																			   PSECURITY_DESCRIPTOR*, // SD
																			   PULONG // SD size
																			   );
typedef BOOL (fnVisitFunc)(const char* strFile, void* para);

class XAccessControl
{
public:
	static BOOL mbSecLowSAInit;
	static BOOL mbSecSAInit;
	static BOOL mbSecSDInit;
	static BOOL mbSecNameEnabled;

	static SECURITY_ATTRIBUTES* getHighPrivSA();
	static SECURITY_DESCRIPTOR* getHighPrivSD();

	// need user to free it outside by LocalFree
	static SECURITY_ATTRIBUTES* getLowPrivSA();

	static void GoThroughDir(const char* strPath, fnVisitFunc* visit, void* para);
	static BOOL SetPrivilege(HANDLE hToken, // access token handle
		LPCTSTR lpszPrivilege, // name of privilege to enable/disable
		BOOL bEnablePrivilege // to enable or disable privilege
		);
	static BOOL EnableSecurityPriv();
	static fn_ConvertStringSecurityDescriptorToSecurityDescriptorW* GetSecurityFuncAddress();
	static BOOL SetFileSecurityInDir(const char* szDirFileName, BOOL bRecursive=TRUE);
	static BOOL SetSingleFileSecurity(const char* szFileName, void* psd=NULL);
};

#endif