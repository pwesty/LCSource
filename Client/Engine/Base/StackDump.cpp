#include "stdh.h"

#include <DbgHelp.h>
#include <tchar.h>
#include <Engine/Build.h>
#include <Engine/Base/Stream.h>
#include <Engine/Base/StackDump.h>
#include <Engine/Base/Console_Internal.h>
#include <Engine/Base/MemoryTracking.h>
#include <Engine/Base/ErrorReporting.h>
#include <Engine/Network/Web.h>
#include <wininet.h>
#include <Engine/Interface/UIManager.h>
#include <Engine/Contents/Login/UILoginNew.h>

static LPTOP_LEVEL_EXCEPTION_FILTER _efPreviousFilter; // Previous filter
static char _fnReportFileName[MAX_PATH] = ""; // Report file name
static CTString strSendErrorData = "";  // 전송할 에러 데이터 
//extern ULONG _ulEngineBuildMajor;
//extern ULONG _ulEngineBuildMinor;
extern void FCPrintF(CTFileStream *pstrm,const char *strFormat, ...);
extern UINT g_uiEngineVersion;
extern int g_iLocalVersion;
extern SLONG	g_slZone;

// <-- ErrorLog.txt에 디스플레이 정보를 기록하기 위한 부분
extern CTString _strDisplayDriver;
extern CTString _strDisplayDriverVersion;
extern CTString	_strSoundDriver;
extern CTString _strTotalMemory;
extern INDEX g_iCountry;
// -->

#define DUMP_LOGICAL_ADDR 1

// dummy function for converting abs addressess to logical ones
extern volatile ULONG __RefFuncAtAbsAddr(void)
{
	return -1;
}

static void _tprintf(HANDLE hFile, const char *format, ...)
{
	char achBuffer[1024];
	
	va_list argptr;
	va_start(argptr, format);
	wvsprintf(achBuffer, format, argptr);
	va_end(argptr);
	
	INDEX iLength = strlen(achBuffer);
	ULONG ulBytesWritten;
	WriteFile(hFile,achBuffer,iLength, &ulBytesWritten, 0);

	strSendErrorData += achBuffer;// 웹 전송 데이터
}

static LPTSTR GetExceptionString(ULONG ulCode)
{
#define EXCEPTION( x ) case EXCEPTION_##x: return #x;
	
	switch(ulCode) {
		EXCEPTION( ACCESS_VIOLATION )
			EXCEPTION( DATATYPE_MISALIGNMENT )
			EXCEPTION( BREAKPOINT )
			EXCEPTION( SINGLE_STEP )
			EXCEPTION( ARRAY_BOUNDS_EXCEEDED )
			EXCEPTION( FLT_DENORMAL_OPERAND )
			EXCEPTION( FLT_DIVIDE_BY_ZERO )
			EXCEPTION( FLT_INEXACT_RESULT )
			EXCEPTION( FLT_INVALID_OPERATION )
			EXCEPTION( FLT_OVERFLOW )
			EXCEPTION( FLT_STACK_CHECK )
			EXCEPTION( FLT_UNDERFLOW )
			EXCEPTION( INT_DIVIDE_BY_ZERO )
			EXCEPTION( INT_OVERFLOW )
			EXCEPTION( PRIV_INSTRUCTION )
			EXCEPTION( IN_PAGE_ERROR )
			EXCEPTION( ILLEGAL_INSTRUCTION )
			EXCEPTION( NONCONTINUABLE_EXCEPTION )
			EXCEPTION( STACK_OVERFLOW )
			EXCEPTION( INVALID_DISPOSITION )
			EXCEPTION( GUARD_PAGE )
			EXCEPTION( INVALID_HANDLE )
	}
	// If not one of the "known" exceptions, try to get the string
	// from NTDLL.DLL's message table.
	
	static TCHAR szBuffer[512] = { 0 };
	FormatMessage(FORMAT_MESSAGE_IGNORE_INSERTS|FORMAT_MESSAGE_FROM_HMODULE,
		GetModuleHandle("NTDLL.DLL"),
		ulCode, 0, szBuffer, sizeof( szBuffer ), 0 );
	
	return szBuffer;
}

static void IntelStackWalk(HANDLE hFile, PCONTEXT pContext)
{
	_tprintf(hFile, "\nmanual stack frame walk begin:\r\n");
	_tprintf(hFile, "\r\n");
	_tprintf(hFile, "Address   Frame     Logical addr  Module\r\n");
	
	DWORD pc = pContext->Eip;
	PDWORD pFrame, pPrevFrame;
	pFrame = (PDWORD)pContext->Ebp;
	
	for(INDEX iDepth=0; iDepth<100; iDepth++) {
		char szModule[MAX_PATH] = "";
		DWORD section = 0, offset = 0;
		_tprintf(hFile, "%08X %08X ", pc, pFrame);
		DWORD symDisplacement = 0;  // Displacement of the input address, relative to the start of the symbol
		
#if DUMP_LOGICAL_ADDR
		BOOL bSuccess = GetLogicalAddress((PVOID)pc, szModule,sizeof(szModule),section,offset );
		ASSERT(bSuccess);
		_tprintf(hFile, "$adr: %s %04X:%08X", szModule, section, offset);
#else
		// :(
#if _DEBUG
		strcpy(szModule,"SeriousSamXD.xbe");
#else
		strcpy(szModule,"SeriousSamX.xbe");
#endif
		
		_tprintf(hFile, "@adr: %s 0x%08X", szModule, (ULONG*)pc);
#endif
		
		_tprintf(hFile, "\r\n");
		pc = pFrame[1];
		pPrevFrame = pFrame;
		pFrame = (PDWORD)pFrame[0]; // proceed to next higher frame on stack
		if((DWORD)pFrame&3) { // Frame pointer must be aligned on a
			break;            // DWORD boundary.  Bail if not so.
		}
		
		if(pFrame<=pPrevFrame) {
			break;
		}
		
		// Can two DWORDs be read from the supposed frame address?
		if(IsBadWritePtr(pFrame, sizeof(PVOID)*2)) {
			break;
		}
	}
	_tprintf(hFile, "\nmanual stack frame walk end:\r\n" );
}

static void ConvertStringToWebParameter(const char *szParam, std::string& strOutput)
{
	if( szParam == NULL )
		return;

	int nLength = strlen(szParam);
	if( nLength == 0 )
		return;

	for( int i=0; i<nLength; ++i )
	{
		if(szParam[i] == '\n')		strOutput += "%0a";
		else if(szParam[i] == '\r')	strOutput += "%0d";
		else if(szParam[i] == '+')	strOutput += "%2b";
		else if(szParam[i] == '&')	strOutput += "error_andchar";
		else if(szParam[i] == ' ')	strOutput += "%20";
		else strOutput += szParam[i];
	}
}

static void SendErrorInfo( HANDLE hFile, CTString &csContents )
{
	HINTERNET hOpen, hConnect, hReq;

	hOpen = InternetOpen( "Web Error Data Send",
							INTERNET_OPEN_TYPE_PRECONFIG,
							NULL,
							NULL,
							0 );

	if( hOpen == NULL ) 
	{
		_tprintf( hFile, "Not InternetOpen\r\n" );
		return;
	}

	hConnect = InternetConnect( hOpen, "lastchaos2.2pan4pan.com", INTERNET_DEFAULT_HTTP_PORT,
		"", "", INTERNET_SERVICE_HTTP, 0, 0 );

	if( !hConnect )
	{
		_tprintf( hFile, "Not InternetConnect \r\n" );
		InternetCloseHandle( hOpen );
		return;
	}

	hReq = HttpOpenRequest( hConnect, "POST", "/client/report_insert.asp", HTTP_VERSION, "", NULL,
		INTERNET_FLAG_CACHE_IF_NET_FAIL | INTERNET_FLAG_MAKE_PERSISTENT | INTERNET_FLAG_KEEP_CONNECTION, 0 );
	
	if( !hReq )
	{
		_tprintf( hFile, "Not HttpOpenRequest\r\n" );
		InternetCloseHandle( hConnect );
		InternetCloseHandle( hOpen );
		return;
	}

	if( !InternetCheckConnection( "http://lastchaos2.2pan4pan.com/client/report_insert.asp",
		FLAG_ICC_FORCE_CONNECTION, 0 ) )
	{
		_tprintf( hFile, "Not InternetCheckConnection\r\n" );
		InternetCloseHandle( hConnect );
		InternetCloseHandle( hOpen );
		return;
	}

	//post header
	const TCHAR *pszHeader = TEXT("Content-Type: application/x-www-form-urlencoded");
	std::string csConvert = "";
	ConvertStringToWebParameter(csContents.str_String, csConvert);
	CTString csSendData = "";
	csSendData.PrintF( "err_version=%d&err_log=%s&err_country=%d", g_uiEngineVersion, csConvert.c_str(), g_iCountry );
	DWORD dwSize = csSendData.Length();

	if( !HttpSendRequest( hReq, pszHeader, _tcslen(pszHeader), csSendData.str_String, dwSize ) )
	{
		_tprintf( hFile, "Not HttpSendRequest\r\n" );
		InternetCloseHandle( hReq );
		InternetCloseHandle( hConnect );
		InternetCloseHandle( hOpen );
		return;
	}

	InternetCloseHandle( hReq );
	InternetCloseHandle( hConnect );
	InternetCloseHandle( hOpen );
}

static void GenerateExceptionReport(HANDLE hFile, PEXCEPTION_POINTERS pExceptionInfo)
{
	char strTime[80];
	char strDate[80];
	_strtime(strTime);
	_strdate(strDate);

	PEXCEPTION_RECORD pExceptionRecord = pExceptionInfo->ExceptionRecord;

#if DUMP_LOGICAL_ADDR
	// Now print information about where the fault occured
	TCHAR szFaultingModule[MAX_PATH];
	DWORD section, offset;
	BOOL bSuccess = GetLogicalAddress(pExceptionRecord->ExceptionAddress, szFaultingModule,
		sizeof(szFaultingModule), section, offset);
	ASSERT(bSuccess);
//	_tprintf(hFile, "[%08X][v.%d-%d][%d]\r\n", pExceptionRecord->ExceptionAddress, g_uiEngineVersion, g_iLocalVersion, g_slZone );
	_tprintf(hFile, "[Ver %s]\r\n", CUIManager::getSingleton()->GetLogin()->getVersion());
#else 
#define GETVARNAME(x) #x
	_tprintf(hFile, "Referent variable: %s 0x%08X\r\n",GETVARNAME(__RefFuncAtAbsAddr),&__RefFuncAtAbsAddr);
#endif
	
	/*
	// Start out with a banner
	_tprintf( hFile, "//===== %s %s Bug Report[v. %d - %d] Zone : %d=====\r\n", strDate,
														strTime, g_uiEngineVersion, g_iLocalVersion, g_slZone );														
	
	  */
	//--------------------OS Version----------------------------------
	OSVERSIONINFO osv;
	memset(&osv, 0, sizeof(osv));
	osv.dwOSVersionInfoSize = sizeof(osv);
	if (GetVersionEx(&osv)) 
	{
		switch (osv.dwPlatformId) 
		{
		case VER_PLATFORM_WIN32s:         _tprintf(hFile, "Type : Win32s\r\n");  break;
		case VER_PLATFORM_WIN32_WINDOWS:  _tprintf(hFile, "Type : Win9x\r\n"); break;
		case VER_PLATFORM_WIN32_NT:       _tprintf(hFile, "Type : WinNT\r\n"); break;
		default: _tprintf(hFile, "Type : Unknown\r\n"); break;
		}
		_tprintf(hFile, "  Version: %d.%d, build %d\r\n", 
			osv.dwMajorVersion, osv.dwMinorVersion, osv.dwBuildNumber & 0xFFFF);
		_tprintf(hFile, "  Misc: %s\r\n", osv.szCSDVersion);
	} 
	else 
	{
		_tprintf(hFile, "Error getting OS info: %s\r\n", GetWindowsError(GetLastError()) );
	}
	_tprintf(hFile, "Total Memory : %s\r\n", _strTotalMemory);			// 물리적 메모리 크기
	_tprintf(hFile, "\r\n");
	
	_tprintf(hFile, "Display : %s\r\n", _strDisplayDriver);				// 그래픽 카드 종류
	_tprintf(hFile, "Display Version : %s\r\n", _strDisplayDriverVersion);// 그래픽 드라이버 버젼
	_tprintf(hFile, "Sound : %s\r\n", _strSoundDriver);					// 사운드 카드 종류
	//_tprintf(hFile, "National-Code : %d\r\n", g_iCountry);				// 국가 코드
	_tprintf(hFile, "\r\n");
	
	_tprintf(hFile, "Crashed at: %s %s\r\n", strDate, strTime);
	//_tprintf(hFile, "Version: %d.%d%s%s\n", _ulEngineBuildMajor, _ulEngineBuildMinor, _SE_BUILD_EXTRA, _SE_DEMO?"-demo":"");
	
	// First print information about the type of fault
	_tprintf(hFile, "Exception code: %08X %s\r\n", pExceptionRecord->ExceptionCode,
		GetExceptionString(pExceptionRecord->ExceptionCode));
	
	
#if DUMP_LOGICAL_ADDR
	// Now print information about where the fault occured
//	TCHAR szFaultingModule[MAX_PATH];
//	DWORD section, offset;
//	BOOL bSuccess = GetLogicalAddress(pExceptionRecord->ExceptionAddress, szFaultingModule,
//		sizeof(szFaultingModule), section, offset);
	ASSERT(bSuccess);
	_tprintf(hFile, "Fault address:  %08X %02X:%08X %s\r\n", pExceptionRecord->ExceptionAddress,
		section, offset, szFaultingModule);
#else 
#define GETVARNAME(x) #x
	_tprintf(hFile, "Referent variable: %s 0x%08X\r\n",GETVARNAME(__RefFuncAtAbsAddr),&__RefFuncAtAbsAddr);
#endif	
	
	PCONTEXT pCtx = pExceptionInfo->ContextRecord;
	
	// Show the registers
#ifdef _M_IX86  // Intel Only!
	_tprintf(hFile, "\nRegisters:\r\n");
	
	_tprintf(hFile, "EAX:%08X\nEBX:%08X\nECX:%08X\nEDX:%08X\nESI:%08X\nEDI:%08X\r\n",
		pCtx->Eax, pCtx->Ebx, pCtx->Ecx, pCtx->Edx, pCtx->Esi, pCtx->Edi );
	
	_tprintf(hFile, "CS:EIP:%04X:%08X\r\n", pCtx->SegCs, pCtx->Eip );
	_tprintf(hFile, "SS:ESP:%04X:%08X  EBP:%08X\r\n", pCtx->SegSs, pCtx->Esp, pCtx->Ebp );
	_tprintf(hFile, "DS:%04X  ES:%04X  FS:%04X  GS:%04X\r\n", pCtx->SegDs, pCtx->SegEs, pCtx->SegFs, pCtx->SegGs );
	_tprintf(hFile, "Flags:%08X\r\n", pCtx->EFlags );
	
#endif // #ifdef _M_IX86
	
#ifdef _M_IX86  // Intel Only!
	// Walk the stack using x86 specific code
	IntelStackWalk(hFile, pCtx);
#endif // #ifdef _M_IX86
	
	// Done
	_tprintf(hFile, "\r\n" );

	if( YesNoMessage( "Error log information saved in ErrorLog.txt" ) )
	{	// 웹으로 에러데이터 전송
		//SendErrorInfo( hFile, strSendErrorData );
		// 전송후 초기화
		//strSendErrorData.Clear();
	}
}

// based on dbghelp.h
typedef BOOL (WINAPI *MINIDUMPWRITEDUMP)(HANDLE hProcess, DWORD dwPid, HANDLE hFile, MINIDUMP_TYPE DumpType,
	CONST PMINIDUMP_EXCEPTION_INFORMATION ExceptionParam,
	CONST PMINIDUMP_USER_STREAM_INFORMATION UserStreamParam,
	CONST PMINIDUMP_CALLBACK_INFORMATION CallbackParam
	);

int WriteDump( PEXCEPTION_POINTERS pExceptionInfo )
{
	//반드시 로딩
	HMODULE DllHandle = LoadLibrary(_T("DBGHELP.DLL"));
	if (DllHandle == NULL)
	{
		return EXCEPTION_CONTINUE_SEARCH;
	}

	//DBGHELP.DLL에서 MiniDumpWriteDump를 불러와 Dump라고 정의하며 이걸로 덤프 파일을 생성합니다.
	MINIDUMPWRITEDUMP Dump = (MINIDUMPWRITEDUMP)GetProcAddress(DllHandle, "MiniDumpWriteDump");
	if (Dump == NULL)
	{
		return EXCEPTION_CONTINUE_SEARCH;
	}


	SYSTEMTIME SystemTime;
	GetLocalTime(&SystemTime);	//현재시간 획득

	TCHAR DumpPath[MAX_PATH] = {0,};
	//덤프 파일 이름 설정
	sprintf(DumpPath, _T("%d-%d-%d %d_%d_%d.dmp"), 
		SystemTime.wYear,
		SystemTime.wMonth,
		SystemTime.wDay,
		SystemTime.wHour,
		SystemTime.wMinute,
		SystemTime.wSecond);

	//덤프 파일 생성
	HANDLE FileHandle = CreateFile(DumpPath, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (FileHandle == INVALID_HANDLE_VALUE)
	{
//		printLastError();
		return EXCEPTION_CONTINUE_SEARCH;
	}

	//MiniDump 예외 정보 저장 구조체
	_MINIDUMP_EXCEPTION_INFORMATION MiniDumpExceptionInfo;
	MiniDumpExceptionInfo.ThreadId			= GetCurrentThreadId();
	MiniDumpExceptionInfo.ExceptionPointers	= (PEXCEPTION_POINTERS)pExceptionInfo;
	MiniDumpExceptionInfo.ClientPointers		= NULL;

	//현재 프로세스에 대한 덤프 기록을 실행합니다.
	BOOL Success = Dump(
		GetCurrentProcess(),
		GetCurrentProcessId(),
		FileHandle,				//덤프를 기록할 파일 핸들
		MiniDumpNormal,
		&MiniDumpExceptionInfo,	//MiniDump 예외 정보
		NULL,
		NULL);

	//덤프 기록 설공시 수행
	if (Success)
	{
		CloseHandle(FileHandle);
		return EXCEPTION_EXECUTE_HANDLER;
	}
	CloseHandle(FileHandle);

	return EXCEPTION_CONTINUE_SEARCH;
}

static LONG WINAPI _UnhandledExceptionFilter(PEXCEPTION_POINTERS pExceptionInfo)
{
#if 0
	_asm int 3; // for debugging
#endif
	
	extern INDEX dbg_bAtHome;
	BOOL bDumpReportFile = TRUE;
	
	if(bDumpReportFile) 
	{
		HANDLE hFile = CreateFile(_fnReportFileName, GENERIC_WRITE, 0, 0, OPEN_ALWAYS, FILE_FLAG_WRITE_THROUGH, 0);
		if(hFile!=INVALID_HANDLE_VALUE) 
		{
			// Dump report at eof
			SetFilePointer(hFile, 0, 0, FILE_END);
			// Dump report
			GenerateExceptionReport(hFile, pExceptionInfo);
			// Done
			CloseHandle(hFile);
		}
	}

	WriteDump(pExceptionInfo);

	// 튕겼을때 소켓을 닫는다.
	extern SOCKET g_hSocket;
	if(g_hSocket != INVALID_SOCKET)
	{
		closesocket(g_hSocket);
		int iResult = WSACleanup();
		ASSERT(iResult==0);
	}
	
	// Make sure the console log was written safely
	if (_pConsole!=NULL) {
		_pConsole->CloseLog();
	}
	extern void EnableWindowsKeys(void);
	EnableWindowsKeys();
	
	if(_efPreviousFilter) {
		return _efPreviousFilter(pExceptionInfo);
	} else {
		return EXCEPTION_CONTINUE_SEARCH;
	}
}

extern void InitExceptionHandler(const char* FileName)
{
	// Set current exception filter function
	_efPreviousFilter = SetUnhandledExceptionFilter(_UnhandledExceptionFilter);
	
    // Get name for report file
    //GetModuleFileName(0, _fnReportFileName, MAX_PATH);
	strcpy(_fnReportFileName, FileName);
	
	/*
    // Add .txt extension
    char *pchDot = strrchr(_fnReportFileName,'.');
    if(pchDot!=NULL) {
	// Skip dot
	pchDot++;
	if(strlen(pchDot)>=3) {
	strcpy(pchDot,"TXT");
	}
    }
	*/
}

extern void CloseExceptionHandler(void)
{
	// Return old exception filter
	SetUnhandledExceptionFilter(_efPreviousFilter);
}

// Convert absolute address from logical address
extern BOOL GetLogicalAddress(void *pAddr, char *szModule, ULONG ulLen, ULONG &ulSection, ULONG &ulOffset)
{
	MEMORY_BASIC_INFORMATION mbi;
	
	if(!VirtualQuery(pAddr, &mbi, sizeof(mbi))) {
		return FALSE;
	}
	
	DWORD hMod = (DWORD)mbi.AllocationBase;
	if(!GetModuleFileName((HMODULE)hMod, szModule, ulLen)) {
		return FALSE;
	}
	
	// Point to the DOS header in memory
	PIMAGE_DOS_HEADER pDosHdr = (PIMAGE_DOS_HEADER)hMod;
	
	// From the DOS header, find the NT (PE) header
	PIMAGE_NT_HEADERS pNtHdr = (PIMAGE_NT_HEADERS)(hMod + pDosHdr->e_lfanew);
	PIMAGE_SECTION_HEADER pSection = IMAGE_FIRST_SECTION( pNtHdr );
	DWORD rva = (DWORD)pAddr - hMod; // RVA is offset from module load address
	
	// Iterate through the section table, looking for the one that encompasses
	// the linear address.
	for ( unsigned int i=0;i<pNtHdr->FileHeader.NumberOfSections;i++,pSection++) {
		DWORD sectionStart = pSection->VirtualAddress;
		DWORD sectionEnd = sectionStart + max(pSection->SizeOfRawData, pSection->Misc.VirtualSize);
		
		// Is the address in this section???
		if((rva>=sectionStart) && (rva<=sectionEnd)) {
			// Yes, address is in the section.  Calculate section and offset,
			// and store in the "section" & "offset" params, which were
			// passed by reference.
			ulSection = i+1;
			ulOffset = rva - sectionStart;
			return TRUE;
		}
	}
	return FALSE;   // Should never get here!
}

extern void DumpCurrentStack(CTFileStream *pstrm/*=NULL*/)
{
#define STACKTRACKDEPTH 32
	
	char strTime[12];
	_strtime(strTime);
	
	FCPrintF(pstrm,"\n");
	FCPrintF(pstrm,"Dumping stack at %s\n",strTime);
	
	ULONG aulStack[STACKTRACKDEPTH];
	
	ULONG	ulFrame;
	int		isd;
	// Get current frame (ebp)
	__asm mov dword ptr ulFrame, ebp;
	// walk all frames
	for( isd = 0; isd < STACKTRACKDEPTH; isd++ ) 
	{
		// Get previous frame
		ULONG ulPrevFrame = *(ULONG*)ulFrame;
		// Get calling address
		ULONG ulRetAddr = *(ULONG*)(ulFrame+4);
		aulStack[isd] = ulRetAddr;
		
		// Frame pointer must be aligned on a ULONG boundary
		if(ulFrame&3) {
			break;
		}
		if(ulPrevFrame<=ulFrame) {
			break;
		}
		
		if(IsBadWritePtr((ULONG*)ulFrame,sizeof(ULONG)) || IsBadWritePtr((ULONG*)ulPrevFrame,sizeof(ULONG))) {
			break;
		}
		ulFrame = ulPrevFrame; // go to next frame
	}
	
	for( isd = 0; isd < STACKTRACKDEPTH; isd++) {
		ULONG ulAddress = aulStack[isd];
		// if done
		if(ulAddress==0) {
			// bail out
			break;
		}
		
		// Get module for calling address
		char strModule[MAX_PATH];
		ULONG ulSection = 0;
		ULONG ulOffset = 0;
		
#if DUMP_LOGICAL_ADDR
		BOOL bSuccess = GetLogicalAddress((PVOID)ulAddress, strModule,sizeof(strModule),ulSection,ulOffset);
		if(bSuccess) {
			FCPrintF(pstrm, "$adr: %s %04X:%08X\n", strModule, ulSection, ulOffset);
		}
#else
		// :(
#if _DEBUG
		strcpy(strModule,"SeriousSamXD.xbe");
#else
		strcpy(strModule,"SeriousSamX.xbe");
#endif
		
		FCPrintF(pstrm,"@adr: %s 0x%08X\n", strModule, (ULONG*)ulAddress);
#endif
	}
	FCPrintF(pstrm,"\n");
}







