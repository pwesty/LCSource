#include "stdh.h"

#include <Engine/Base/Shell.h>
#include <Engine/Base/Shell_internal.h>
#include "ParsingSymbols.h"

#include <Engine/Templates/DynamicStackArray.h>
#include <Engine/Base/Console.h>
#include <Engine/Base/Stream.h>
#include <Engine/Base/MemoryTracking.h>
#include <Engine/Base/SimpleHashTable.h>

#include <Engine/Templates/AllocationArray.cpp>
#include <Engine/Templates/DynamicArray.cpp>
#include <Engine/Templates/DynamicStackArray.cpp>

//안태훈 수정 시작	//(Taiwan Closed beta)(0.2)
#include <Engine/Base/PersistentSymbolsEncode.h>
CTString OpenPersistentSymbolFile(const CTFileName &fnmPS)
{
	CTFileStream fs;
	fs.Open_t(fnmPS, CTStream::OM_READBINARY);
	INDEX len = -1;
	fs >> len;
	UBYTE *bufferSrc = new UBYTE[len];
	char *bufferDecoded = new char[len + 1];
	memset(bufferDecoded, 0, len+1);
	fs.Read_t(bufferSrc, len);

	DecodePersistentSymbols((UBYTE*)bufferDecoded, len*2, bufferSrc, len);

	CTString strContents = bufferDecoded;
	delete[] bufferSrc;
	delete[] bufferDecoded;
	return strContents;
}

void SavePersistentSymbolFile(const CTFileName &fnmPS, CTString strContents)
{
	CTFileStream fs;
	//fs.Create_t(fnmPS, CTStream::CM_BINARY);
	fs.Open_t(fnmPS, CTStream::OM_WRITEBINARY);
	INDEX len = strContents.Length();
	UBYTE *bufferEncoded = new UBYTE[len];
	UBYTE *bufferSrc = new UBYTE[len];
	memset(bufferEncoded, 0, len);
	memset(bufferSrc, 0, len);
	for(INDEX i=0; i<len; ++i) bufferSrc[i] = UBYTE(strContents[i]);

	EncodePersistentSymbols(bufferEncoded, len*2, bufferSrc, len);

	fs << len;
	fs.Write_t(bufferEncoded, len);
	delete[] bufferEncoded;
	delete[] bufferSrc;
}
//안태훈 수정 끝	//(Taiwan Closed beta)(0.2)

template CDynamicArray<CShellSymbol>;

// shell type used for undeclared symbols
extern INDEX _shell_istUndeclared = -1;

// pointer to global shell object
CShell *_pShell = NULL;
void *_pvNextToDeclare=NULL; // != NULL if declaring external symbol defined in exe code

// Symbol hash table
static BOOL CompareSymbols(INDEX iSymbol, const void *pSymbol);
static ULONG CalculateHash(const void *pSymbol);
static void RecreateHashTable(void);
static CSimpleHashTable _shtHashTable(CompareSymbols,CalculateHash,RecreateHashTable,10000);

// define console variable for number of last console lines
	extern INDEX con_iLastLines = 5;

extern void yy_switch_to_buffer(YY_BUFFER_STATE);

// declarations for recursive shell script parsing
struct BufferStackEntry {
	YY_BUFFER_STATE bse_bs;
	char *bse_strName;
	char *bse_strContents;
	int bse_iLineCt;
	BOOL bse_bParserEnd;
};

static BufferStackEntry _abseBufferStack[SHELL_MAX_INCLUDE_LEVEL];
static int _ibsBufferStackTop = -1;

BOOL _bExecNextBlock = 1;

void ShellPushBuffer(const char *strName, const char *strBuffer, BOOL bParserEnd)
{
	TRACKMEM(Mem, "Shell");
	_ibsBufferStackTop++;

	_abseBufferStack[_ibsBufferStackTop].bse_strContents = StringDuplicate(strBuffer);
	_abseBufferStack[_ibsBufferStackTop].bse_strName = StringDuplicate(strName);
	_abseBufferStack[_ibsBufferStackTop].bse_iLineCt = 1;
	_abseBufferStack[_ibsBufferStackTop].bse_bParserEnd = bParserEnd;

	_abseBufferStack[_ibsBufferStackTop].bse_bs = yy_scan_string((char*)(const char*)strBuffer);

	yy_switch_to_buffer(_abseBufferStack[_ibsBufferStackTop].bse_bs);
}
BOOL ShellPopBuffer(void)
{
	TRACKMEM(Mem, "Shell");
	yy_delete_buffer( _abseBufferStack[_ibsBufferStackTop].bse_bs);
	StringFree(_abseBufferStack[_ibsBufferStackTop].bse_strName);
	StringFree(_abseBufferStack[_ibsBufferStackTop].bse_strContents);
	BOOL bParserEnd = _abseBufferStack[_ibsBufferStackTop].bse_bParserEnd;

	_ibsBufferStackTop--;

	if (_ibsBufferStackTop>=0) {
		yy_switch_to_buffer(_abseBufferStack[_ibsBufferStackTop].bse_bs);
	}
	return bParserEnd;
}
const char *ShellGetBufferName(void)
{
	return _abseBufferStack[_ibsBufferStackTop].bse_strName;
}
int ShellGetBufferLineNumber(void)
{
	return _abseBufferStack[_ibsBufferStackTop].bse_iLineCt;
}
int ShellGetBufferStackDepth(void)
{
	return _ibsBufferStackTop;
}
const char *ShellGetBufferContents(void)
{
	return _abseBufferStack[_ibsBufferStackTop].bse_strContents;
}
void ShellCountOneLine(void)
{
	_abseBufferStack[_ibsBufferStackTop].bse_iLineCt++;
}


// temporary values for parsing
CDynamicStackArray<CTString> _shell_astrTempStrings;
// values for extern declarations
CDynamicStackArray<CTString> _shell_astrExtStrings;
CDynamicStackArray<FLOAT> _shell_afExtFloats;

static const char *strCommandLine = "";

ENGINE_API extern FLOAT tmp_af[20] = { 0 };
ENGINE_API extern INDEX tmp_ai[10] = { 0 };
ENGINE_API extern INDEX tmp_fAdd   = 0.0f;
ENGINE_API extern INDEX tmp_i      = 0;
#if MEMORY_TRACKING
extern ULONG _ulCurrentAllocationAge;
#endif
extern INDEX mem_ulStackTraceBegin = -1;
extern INDEX mem_ulStackTraceEnd = -1;

void CShellSymbol::Clear(void)
{
	ss_istType = -1;
	ss_strName.Clear();
	ss_ulFlags = 0;
};
BOOL CShellSymbol::IsDeclared(void)
{
	return ss_istType>=0 && ss_istType!=_shell_istUndeclared;
}

CTString CShellSymbol::GetCompletionString(void) const
{
	// get its type
	ShellType &st = _shell_ast[ss_istType];

	// get its name
	if (st.st_sttType==STT_FUNCTION) {
		return ss_strName + "()";
	} else if (st.st_sttType==STT_ARRAY) {
		return ss_strName + "[]";
	} else {
		return ss_strName;
	}
}

// Constructor.
CShell::CShell(void)
{
	TRACKMEM(Mem, "Shell");
	// allocate undefined symbol
	_shell_istUndeclared = _shell_ast.Allocate();
};
CShell::~CShell(void)
{
	_shell_astrExtStrings.Clear();
	_shell_afExtFloats.Clear();
};

static const INDEX _bTRUE  = TRUE;
static const INDEX _bFALSE = FALSE;

CTString ScriptEsc(const CTString &str)
{
	CTString strResult = "";

	const char *pchSrc = (const char *)str;
	char buf[2];
	buf[1] = 0;

	while (*pchSrc!=0) {
		switch(*pchSrc) {
		case  10: strResult+="\\n"; break;
		case  13: strResult+="\\r"; break;
		case '\\': strResult+="\\\\"; break;
		case '"': strResult+="\\\""; break;
		default: buf[0] = *pchSrc; strResult+=buf; break;
		}
		pchSrc++;
	}
	return strResult;
}

#pragma inline_depth(0)
void MakeAccessViolation(INDEX bDont)
{
	if( bDont) return;
	char *p=NULL;
	*p=1;
}

extern int _a=123;
void MakeStackOverflow(INDEX bDont)
{
	if( bDont) return;
	int a[1000];
	a[999] = _a;
	MakeStackOverflow(0);
	_a=a[999];
}

void MakeFatalError(INDEX bDont)
{
	if( bDont) return;
	FatalError( "MakeFatalError()");
}

// Print to file or to console
extern void FCPrintF(CTFileStream *pstrm,const char *strFormat, ...)
{
	// format the message in buffer
	va_list arg;
	va_start(arg, strFormat);
	CTString strBuffer;
	strBuffer.VPrintF(strFormat, arg);

	if(pstrm==NULL) {
		CPrintF(strBuffer);
	} else {
		pstrm->FPrintF_t(strBuffer);
	}
}

// <-- ErrorLog.txt에 디스플레이 정보를 기록하기 위한 부분
extern ENGINE_API CTString _strTotalMemory;
// -->

extern void ReportGlobalMemoryStatus(CTFileStream *pstrm)
{
	MEMTRACK_SETFLAGS(mem,MTF_NOSTACKTRACE);
	FCPrintF(pstrm,TRANS("Global memory status...\n"));

	MEMORYSTATUS ms;
	GlobalMemoryStatus(&ms);
#define KB (1024.0f)
#define MB (KB*KB)
	 FCPrintF(pstrm,TRANS("  Physical memory used: %6.2f/%6.2fMB\t(%d B)\t(%6.2f KB)\n"), (ms.dwTotalPhys-ms.dwAvailPhys )/MB, ms.dwTotalPhys/MB,ms.dwTotalPhys-ms.dwAvailPhys, (ms.dwTotalPhys-ms.dwAvailPhys)/KB);
	 // <-- ErrorLog.txt에 디스플레이 정보를 기록하기 위한 부분
	 _strTotalMemory.PrintF( TRANS("%6.2fMB\n"), ms.dwTotalPhys/MB );
	 // -->
	 FCPrintF(pstrm,TRANS("  Page file used:       %6.2f/%6.2fMB\n"), (ms.dwTotalPageFile-ms.dwAvailPageFile)/MB, ms.dwTotalPageFile/MB);
	 FCPrintF(pstrm,TRANS("  Virtual memory used:  %6.2f/%6.2fMB\n"), (ms.dwTotalVirtual -ms.dwAvailVirtual )/MB, ms.dwTotalVirtual /MB);
	 FCPrintF(pstrm,TRANS("  Memory load: %3d%%\n"), ms.dwMemoryLoad);

	 DWORD dwMin;
	 DWORD dwMax;
	 GetProcessWorkingSetSize(GetCurrentProcess(), &dwMin, &dwMax);
	 FCPrintF(pstrm,TRANS("  Process working set: %dMB-%dMB\n\n"), dwMin/(1024*1024), dwMax/(1024*1024));

	MEMTRACK_DUMP_INSTREAM(pstrm);
}

static void MemoryInfo(void)
{
	ReportGlobalMemoryStatus(NULL);

	_HEAPINFO hinfo;
	 int heapstatus;
	 hinfo._pentry = NULL;
	 SLONG slTotalUsed = 0;
	 SLONG slTotalFree = 0;
	 INDEX ctUsed = 0;
	 INDEX ctFree = 0;

	 CPrintF( "Walking heap...\n");
	 while( ( heapstatus = _heapwalk( &hinfo ) ) == _HEAPOK )
	 {
		 if (hinfo._useflag == _USEDENTRY ) {
			 slTotalUsed+=hinfo._size;
			 ctUsed++;
		 } else {
			 slTotalFree+=hinfo._size;
			 ctFree++;
		 }
	 }
	 switch( heapstatus )   {
		 case _HEAPEMPTY:     CPrintF( "Heap empty?!?\n" );                break;
		 case _HEAPEND:       CPrintF( "Heap ok.\n" );                     break;
		 case _HEAPBADPTR:    CPrintF( "ERROR - bad pointer to heap\n" );  break;
		 case _HEAPBADBEGIN:  CPrintF( "ERROR - bad start of heap\n" );    break;
		 case _HEAPBADNODE:   CPrintF( "ERROR - bad node in heap\n" );     break;
	 }
	 CPrintF( "Total used: %d bytes (%.2f MB) in %d blocks\n", slTotalUsed, slTotalUsed/1024.0f/1024.0f, ctUsed);
	 CPrintF( "Total free: %d bytes (%.2f MB) in %d blocks\n", slTotalFree, slTotalFree/1024.0f/1024.0f, ctFree);
}

static INDEX _ctReports = 1;
static void MemoryReport(void)
{
	MEMTRACK_SETFLAGS(mem,MTF_NOSTACKTRACE);
	TRACKMEM(Mem, "Shell");
	#define KB (1024.0f)
	#define MB (KB*KB)
	try {
		CTFileStream strm;
		CTFileName fnm;
		// Find unused report file
		do {
			fnm = CTString(0,"Temp\\MemReport_%03d.txt",_ctReports++);
		} while(FileExists(fnm));

		strm.Create_t(fnm);
		extern void FreeUnusedStock(void);
		extern void StockInfoToStream(CTFileStream *pstrm);
		extern void StockDumpToStream(CTFileStream *pstrm);
		FreeUnusedStock();
		char strDate[12];
		char strTime[12];
		_strtime(strTime);
		_strdate(strDate);
		FCPrintF(&strm,"%s\n", strTime);
		FCPrintF(&strm,"%s\n", strDate);
#if MEMORY_TRACKING
		FCPrintF(&strm,"Current memory age is 0x%08X\n",_ulCurrentAllocationAge);
#endif
		ReportGlobalMemoryStatus(&strm);
		StockInfoToStream(&strm);
		StockDumpToStream(&strm);
		CPrintF("Reported to '%s'\n", (const char*)fnm);
	} catch(char *strErr) {
		CPrintF("Error:%s\n",(const char*)strErr);
	}
}

static void StackTraceBegin(void)
{
#if MEMORY_TRACKING
	mem_ulStackTraceBegin = _ulCurrentAllocationAge;
	CPrintF("Stack tracing begin time is 0x%08X\n",mem_ulStackTraceBegin);
#else
	CPrintF("Memory tracking is not enabled!\n");
#endif
}

static void StackTraceEnd(void)
{
#if MEMORY_TRACKING
	mem_ulStackTraceEnd = _ulCurrentAllocationAge;
	CPrintF("Stack tracing end time is 0x%08X\n",mem_ulStackTraceEnd);
#else
	CPrintF("Memory tracking is not enabled!\n");
#endif
}

static INDEX _ctStackTraces = 0;
static void StackTraceDump(void)
{
#if MEMORY_TRACKING
	CTFileStream strm;
	CTFileName fnm;
	do {
		fnm = CTString(0,"Temp\\StackTrace_%03d.txt",_ctStackTraces++);
	} while(FileExists(fnm));

	strm.Create_t(fnm);
	MemoryStackTraceDump(&strm);
	CPrintF("Stack trace dumped to '%s'\n", (const char*)fnm);

#else
	CPrintF("Memory tracking is not enabled!\n");
#endif
}

// get help for a shell symbol
extern CTString GetShellSymbolHelp_t(const CTString &strSymbol)
{
	CTString strPattern = strSymbol+"*";
	// open the symbol help file
	CTFileStream strm;
	strm.Open_t(CTString("Help\\ShellSymbols.txt"));

	// while not at the end of file
	while (!strm.AtEOF()) {
		// read the symbol name and its help
		CTString strSymbolInFile;
		strm.GetLine_t(strSymbolInFile, ':');
		strSymbolInFile.TrimSpacesLeft();
		strSymbolInFile.TrimSpacesRight();
		CTString strHelpInFile;
		strm.GetLine_t(strHelpInFile, '$');
		strHelpInFile.TrimSpacesLeft();
		strHelpInFile.TrimSpacesRight();
		// if that is the one
		if( strSymbolInFile.Matches(strPattern)) {
			// print the help
			return strHelpInFile;
		}
	}
	return "";
}

// check if there is help for a shell symbol
extern BOOL CheckShellSymbolHelp(const CTString &strSymbol)
{
	try {
		return GetShellSymbolHelp_t(strSymbol)!="";
	} catch(char *strError) {
		(void)strError;
		return FALSE;
	}
}

// print help for a shell symbol
extern void PrintShellSymbolHelp(const CTString &strSymbol)
{
	// try to
	try {
		CTString strHelp = GetShellSymbolHelp_t(strSymbol);
		if (strHelp!="") {
			CPrintF("%s\n", strHelp);
		} else {
			CPrintF( TRANS("No help found for '%s'.\n"), strSymbol);
		}
	// if failed
	} catch(char *strError) {
		// just print the error
		CPrintF( TRANS("Cannot print help for '%s': %s\n"), strSymbol, strError);
	}
}

extern void ListSymbolsByPattern(CTString strPattern)
{
	// synchronize access to global shell
	CTSingleLock slShell(&_pShell->sh_csShell, TRUE);

	// for each of symbols in the shell
	FOREACHINDYNAMICARRAY(_pShell->sh_assSymbols, CShellSymbol, itss) {
		CShellSymbol &ss = *itss;

		// if it is not visible to user, or not matching
		if (!(ss.ss_ulFlags&SSF_USER) || !ss.ss_strName.Matches(strPattern)) {
			// skip it
			continue;
		}

		// get its type
		ShellType &st = _shell_ast[ss.ss_istType];

		if (ss.ss_ulFlags & SSF_CONSTANT) {
			CPrintF("const ");
		}
		if (ss.ss_ulFlags & SSF_PERSISTENT) {
			CPrintF("persistent ");
		}

		// print its declaration to the console
		if (st.st_sttType == STT_FUNCTION) {
			CPrintF("void %s(void)", ss.ss_strName);

		} else if (st.st_sttType == STT_STRING) {
			CPrintF("CTString %s = \"%s\"", ss.ss_strName, *(CTString*)ss.ss_pvValue);
		} else if (st.st_sttType == STT_FLOAT) {
			CPrintF("FLOAT %s = %g", ss.ss_strName, *(FLOAT*)ss.ss_pvValue);
		} else if (st.st_sttType == STT_INDEX) {
			CPrintF("INDEX %s = %d (0x%08x)", ss.ss_strName, *(INDEX*)ss.ss_pvValue, *(INDEX*)ss.ss_pvValue);
		} else if (st.st_sttType == STT_ARRAY) {
			// get base type
			ShellType &stBase = _shell_ast[st.st_istBaseType];
			if (stBase.st_sttType == STT_FLOAT) {
				CPrintF("FLOAT %s[%d]", ss.ss_strName, st.st_ctArraySize);
			} else if (stBase.st_sttType == STT_INDEX) {
				CPrintF("INDEX %s[%d]", ss.ss_strName, st.st_ctArraySize);
			} else if (stBase.st_sttType == STT_STRING) {
				CPrintF("CTString %s[%d]", ss.ss_strName, st.st_ctArraySize);
			} else {
				ASSERT(FALSE);
			}
		} else {
			ASSERT(FALSE);
		}

		if (!CheckShellSymbolHelp(ss.ss_strName)) {
			CPrintF( TRANS(" help N/A"));
		}
		CPrintF("\n");
	}
}

// Print a list of all symbols in global shell to console.
static void ListSymbols(void)
{
	// print header
	CPrintF( TRANS("Useful symbols:\n"));

	// list all symbols
	ListSymbolsByPattern("*");
}


// output any string to console
void Echo(const CTString &str)
{
	CPrintF( "%s", str); 
}



CTString UndecorateString(const CTString &strString)
{
	return strString.Undecorated();
}
BOOL MatchStrings(const CTString &strString, const CTString &strPattern)
{
	return strString.Matches(strPattern);
}
CTString MyLoadString(const CTString &strFileName)
{
	try {
		CTString strString;
		strString.Load_t(strFileName);
		return strString;
	} catch (char *strError) {
		(void)strError;
		return "";
	}
}
void MySaveString(const CTString &strFileName, /*const */CTString &strString)
{
	try {
		strString.Save_t(strFileName);
	} catch (char *strError) {
		(void)strError;
	}
}

// load command batch files
void LoadCommands(void)
{
	TRACKMEM(Mem, "Shell");
	// list all command files
	CDynamicStackArray<CTFileName> afnmCmds;
	MakeDirList( afnmCmds, CTString("Scripts\\Commands\\"), "*.ini", DLI_RECURSIVE);
	// for each file
	for(INDEX i=0; i<afnmCmds.Count(); i++) {
		CTFileName &fnm = afnmCmds[i];
		// load the file
		CTString strCmd;
		try {
			strCmd.Load_t(fnm);
		} catch (char *strError) {
			CPrintF("%s\n", strError);
			continue;
		}
		CTString strName = fnm.FileName();
		// declare it
		extern void Declaration(
			ULONG ulQualifiers, INDEX istType, CShellSymbol &ssNew,
			INDEX (*pPreFunc)(INDEX), void (*pPostFunc)(INDEX));

		INDEX iType = ShellTypeNewString();
		CShellSymbol &ssNew = *_pShell->GetSymbol(strName, FALSE);
		Declaration(SSF_EXTERNAL|SSF_USER, iType, ssNew, NULL, NULL);
		ShellTypeDelete(iType);

		// get symbol type
		ShellTypeType stt = _shell_ast[ssNew.ss_istType].st_sttType;

		// if the symbol is ok
		if (stt == STT_STRING && !(ssNew.ss_ulFlags&SSF_CONSTANT)) {
			// assign value
			*(CTString*)ssNew.ss_pvValue = "!command "+strCmd;
		} else {
			_pShell->ErrorF("Symbol '%s' is not suitable to be a command", ssNew.ss_strName);
		}
	}
}

CTString ToUpper(const CTString &str)
{
	CTString strResult = str;
	char *pch = (char*)(const char *)strResult;
	for(INDEX i=0; i<strlen(pch); i++) {
		pch[i]=toupper(pch[i]);
	}
	return strResult;
}
CTString ToLower(const CTString &str)
{
	CTString strResult = str;
	char *pch = (char*)(const char *)strResult;
	for(INDEX i=0; i<strlen(pch); i++) {
		pch[i]=tolower(pch[i]);
	}
	return strResult;
}

CTString RemoveSubstring(const CTString &strFull, const CTString &strSub)
{
	CTString strFullL = ToLower(strFull);
	CTString strSubL = ToLower(strSub);
	const char *pchFullL = strFullL;
	const char *pchSubL = strSubL;
	const char *pchFound = strstr(pchFullL, pchSubL);
	if (pchFound==NULL || strlen(strSub)==0) {
		return strFull;
	}
	INDEX iOffset = pchFound-pchFullL;
	INDEX iLenFull = strlen(strFull);
	INDEX iLenSub = strlen(strSub);

	CTString strLeft = strFull;
	strLeft.TrimRight(iOffset);
	CTString strRight = strFull;
	strRight.TrimLeft(iLenFull-iOffset-iLenSub);
	return strLeft+strRight;
}

// Initialize the shell.
void CShell::Initialize(void)
{
	TRACKMEM(Mem, "Shell");
	sh_csShell.cs_iIndex = -1;

	// synchronize access to shell
	CTSingleLock slShell(&sh_csShell, TRUE);
	// add built in commands and constants
	DeclareSymbol("const INDEX TRUE;",  (void*)&_bTRUE);
	DeclareSymbol("const INDEX FALSE;", (void*)&_bFALSE);
	DeclareSymbol("const INDEX ON;",    (void*)&_bTRUE);
	DeclareSymbol("const INDEX OFF;",   (void*)&_bFALSE);
	DeclareSymbol("const INDEX YES;",   (void*)&_bTRUE);
	DeclareSymbol("const INDEX NO;",    (void*)&_bFALSE);

	DeclareSymbol("user void LoadCommands(void);", &LoadCommands);
	DeclareSymbol("user void ListSymbols(void);", &ListSymbols);

	DeclareSymbol("user void StackTraceBegin(void);",&StackTraceBegin);
	DeclareSymbol("user void StackTraceDump(void);", &StackTraceDump);
	DeclareSymbol("user void StackTraceEnd(void);",  &StackTraceEnd);
	DeclareSymbol("user void MemoryReport(void);",  &MemoryReport);
	DeclareSymbol("user void MemoryInfo(void);",  &MemoryInfo);
	DeclareSymbol("user void MakeAccessViolation(INDEX);", &MakeAccessViolation);
	DeclareSymbol("user void MakeStackOverflow(INDEX);",   &MakeStackOverflow);
	DeclareSymbol("user void MakeFatalError(INDEX);",      &MakeFatalError);
	DeclareSymbol("persistent user INDEX con_iLastLines;", &con_iLastLines);
	DeclareSymbol("persistent user FLOAT tmp_af[20];", &tmp_af);
	DeclareSymbol("persistent user INDEX tmp_ai[10];", &tmp_ai);
	DeclareSymbol("persistent user INDEX tmp_i;", &tmp_i);
	DeclareSymbol("persistent user FLOAT tmp_fAdd;", &tmp_fAdd);
	DeclareSymbol("user INDEX mem_ulStackTraceBegin;", &mem_ulStackTraceBegin);
	DeclareSymbol("user INDEX mem_ulStackTraceEnd;", &mem_ulStackTraceEnd);


	DeclareSymbol("user void Echo(CTString);", &Echo);
	DeclareSymbol("user CTString UndecorateString(CTString);", &UndecorateString);
	DeclareSymbol("user INDEX Matches(CTString, CTString);", &MatchStrings);
	DeclareSymbol("user CTString LoadString(CTString);", &MyLoadString);
	DeclareSymbol("user void SaveString(CTString, CTString);", &MySaveString);
	DeclareSymbol("user CTString RemoveSubstring(CTString, CTString);", &RemoveSubstring);
	DeclareSymbol("user CTString ToUpper(CTString);", &ToUpper);
	DeclareSymbol("user CTString ToLower(CTString);", &ToLower);
}

static BOOL _iParsing = 0;

// Declare a symbol in the shell.
void CShell::DeclareSymbol(const CTString &strDeclaration, void *pvValue)
{
	TRACKMEM(Mem, "Shell");
	// synchronize access to shell
	CTSingleLock slShell(&sh_csShell, TRUE);

	_pvNextToDeclare = pvValue;

	_iParsing++;

	// parse the string
	const BOOL old_bExecNextBlock = _bExecNextBlock;
	_bExecNextBlock = 1;

	ShellPushBuffer("<declaration>", strDeclaration, TRUE);
	yyparse();
//  ShellPopBuffer();

	_bExecNextBlock = old_bExecNextBlock;

	_iParsing--;
	if (_iParsing<=0) {
		_shell_astrTempStrings.PopAll();
	}

	// don't use that value for parsing any more
	_pvNextToDeclare = NULL;
};

// Execute command(s).
void CShell::Execute(const CTString &strCommands)
{
	TRACKMEM(Mem, "Shell");
	// synchronize access to shell
	CTSingleLock slShell(&sh_csShell, TRUE);

//  ASSERT(_iParsing==0);
	_iParsing++;

	// parse the string
	const BOOL old_bExecNextBlock = _bExecNextBlock;
	_bExecNextBlock = 1;

	ShellPushBuffer("<command>", strCommands, TRUE);
	yyparse();
	//ShellPopBuffer();

	_bExecNextBlock = old_bExecNextBlock;

	_iParsing--;
	if (_iParsing<=0) {
		_shell_astrTempStrings.PopAll();
	}
};

// Get a shell symbol by its name.
CShellSymbol *CShell::GetSymbol(const CTString &strName, BOOL bDeclaredOnly)
{
	TRACKMEM(Mem, "Shell");
	// synchronize access to shell
	CTSingleLock slShell(&sh_csShell, TRUE);

	// find shell symbol in hash table
	INDEX iSymbol = _shtHashTable.FindObject(&strName);
	// if symbol is found
	if(iSymbol!=(-1)) {
		return &sh_assSymbols[iSymbol];
	}

	// if only declared symbols are allowed
	if (bDeclaredOnly) {
		// return nothing
		return NULL;

	// if undeclared symbols are allowed
	} else {
		// Add string to hash table
		INDEX iStringIndex = sh_assSymbols.Count();
		_shtHashTable.AddObject(&strName,iStringIndex);

		// create a new one with that name and undefined type
		CShellSymbol &ssNew = *sh_assSymbols.New(1);
		ssNew.ss_strName = strName;
		ssNew.ss_istType = _shell_istUndeclared;
		ssNew.ss_pvValue = NULL;
		ssNew.ss_ulFlags = 0;
		ssNew.ss_pPreFunc = NULL;
		ssNew.ss_pPostFunc = NULL;
		return &ssNew;
	}
};

FLOAT CShell::GetFLOAT(const CTString &strName)
{
	// get the symbol
	CShellSymbol *pss = GetSymbol(strName, TRUE);

	// if it doesn't exist or is not of given type
	if (pss==NULL || _shell_ast[pss->ss_istType].st_sttType!=STT_FLOAT) {
		// error
		ASSERT(FALSE);
		return -666.0f;
	} 
	// get it
	return *(FLOAT*)pss->ss_pvValue;
}

void CShell::SetFLOAT(const CTString &strName, FLOAT fValue)
{
	CShellSymbol *pss = GetSymbol(strName, TRUE);

	// if it doesn't exist or is not of given type
	if (pss==NULL || _shell_ast[pss->ss_istType].st_sttType!=STT_FLOAT) {
		// error
		ASSERT(FALSE);
		return;
	} 
	// set it
	*(FLOAT*)pss->ss_pvValue = fValue;
}

INDEX CShell::GetINDEX(const CTString &strName)
{
	// get the symbol
	CShellSymbol *pss = GetSymbol(strName, TRUE);

	// if it doesn't exist or is not of given type
	if (pss==NULL || _shell_ast[pss->ss_istType].st_sttType!=STT_INDEX) {
		// error
		ASSERT(FALSE);
		return -666;
	} 
	// get it
	return *(INDEX*)pss->ss_pvValue;
}

void CShell::SetINDEX(const CTString &strName, INDEX iValue)
{
	CShellSymbol *pss = GetSymbol(strName, TRUE);

	// if it doesn't exist or is not of given type
	if (pss==NULL || _shell_ast[pss->ss_istType].st_sttType!=STT_INDEX) {
		// error
		ASSERT(FALSE);
		return;
	} 
	// set it
	*(INDEX*)pss->ss_pvValue = iValue;
}

CTString CShell::GetString(const CTString &strName)
{
	// get the symbol
	CShellSymbol *pss = GetSymbol(strName, TRUE);

	// if it doesn't exist or is not of given type
	if (pss==NULL || _shell_ast[pss->ss_istType].st_sttType!=STT_STRING) {
		// error
		ASSERT(FALSE);
		return "<invalid>";
	} 
	// get it
	return *(CTString*)pss->ss_pvValue;
}

void CShell::SetString(const CTString &strName, const CTString &strValue)
{
	TRACKMEM(Mem, "Shell");
	CShellSymbol *pss = GetSymbol(strName, TRUE);

	// if it doesn't exist or is not of given type
	if (pss==NULL || _shell_ast[pss->ss_istType].st_sttType!=STT_STRING) {
		// error
		ASSERT(FALSE);
		return;
	} 
	// set it
	*(CTString*)pss->ss_pvValue = strValue;
}


CTString CShell::GetValue(const CTString &strName)
{
	// get the symbol
	CShellSymbol *pss = GetSymbol(strName, TRUE);

	// if it doesn't exist
	if (pss==NULL) {
		// error
		ASSERT(FALSE);
		return "<invalid>";
	} 

	// get it
	ShellTypeType stt = _shell_ast[pss->ss_istType].st_sttType;
	CTString strValue;
	switch(stt) {
	case STT_STRING:
		strValue = *(CTString*)pss->ss_pvValue;
		break;
	case STT_INDEX:
		strValue.PrintF("%d", *(INDEX*)pss->ss_pvValue);
		break;
	case STT_FLOAT:
		strValue.PrintF("%g", *(FLOAT*)pss->ss_pvValue);
		break;
	default:
		ASSERT(FALSE);
		return "";
	}

	return strValue;
}

void CShell::SetValue(const CTString &strName, const CTString &strValue)
{
	TRACKMEM(Mem, "Shell");
	// get the symbol
	CShellSymbol *pss = GetSymbol(strName, TRUE);

	// if it doesn't exist
	if (pss==NULL) {
		// error
		ASSERT(FALSE);
		return;
	} 
	// get it
	ShellTypeType stt = _shell_ast[pss->ss_istType].st_sttType;
	switch(stt) {
	case STT_STRING:
		*(CTString*)pss->ss_pvValue = strValue;
		break;
	case STT_INDEX:
		((CTString&)strValue).ScanF("%d", (INDEX*)pss->ss_pvValue);
		break;
	case STT_FLOAT:
		((CTString&)strValue).ScanF("%g", (FLOAT*)pss->ss_pvValue);
		break;
	default:
		ASSERT(FALSE);
	}

	return;
}

// Get symbol type
enum ShellTypeType CShell::GetType(const CTString &strName)
{
	// get symbol
	const CShellSymbol *pss = GetSymbol(strName,TRUE);
	ASSERT(pss!=NULL); // must be valid
	const ShellTypeType stt = _shell_ast[pss->ss_istType].st_sttType;
	return stt;
}

/*
 * Report error in shell script processing.
 */
void CShell::ErrorF(const char *strFormat, ...)
{
	// synchronize access to shell
	CTSingleLock slShell(&sh_csShell, TRUE);

	// print the error file and line
	const char *strName = ShellGetBufferName();
	int iLine = ShellGetBufferLineNumber();
	if (strName[0] == '<') {
		CPrintF("%s\n%s(%d): ", ShellGetBufferContents(), strName, iLine);
	} else {
		CPrintF("%s(%d): ", strName, iLine);
	}

	// format the message in buffer
	va_list arg;
	va_start(arg, strFormat);
	CTString strBuffer;
	strBuffer.VPrintF(strFormat, arg);

	// print it to the main console
	CPrintF(strBuffer);
	// go to new line
	CPrintF("\n");
}

// Save shell commands to restore persistent symbols to a script file
void CShell::StorePersistentSymbols(const CTFileName &fnScript)
{
	// synchronize access to global shell
	CTSingleLock slShell(&sh_csShell, TRUE);

	try {
		// open the file
		CTFileStream fScript;
		fScript.Create_t(fnScript);

		// print header
		fScript.FPrintF_t("// automatically saved persistent symbols:\n");
		// for each of symbols in the shell
		FOREACHINDYNAMICARRAY(sh_assSymbols, CShellSymbol, itss) {
			CShellSymbol &ss = *itss;

			// if it is not persistent
			if (! (ss.ss_ulFlags & SSF_PERSISTENT)) {
				// skip it
				continue;
			}

			char *strUser = (ss.ss_ulFlags & SSF_USER)?"user ":"";

			// get its type
			ShellType &st = _shell_ast[ss.ss_istType];
			// if array
			if (st.st_sttType==STT_ARRAY) {
				// get base type
				ShellType &stBase = _shell_ast[st.st_istBaseType];
				CTString strType;
				// if float
				if (stBase.st_sttType==STT_FLOAT) {
					// dump all members as floats
					for(INDEX i=0; i<st.st_ctArraySize; i++) {
						fScript.FPrintF_t("%s[%d]=(FLOAT)%g;\n", ss.ss_strName, i, ((FLOAT*)ss.ss_pvValue)[i]);
					}
				// if index
				} else if (stBase.st_sttType==STT_INDEX) {
					// dump all members as indices
					for(INDEX i=0; i<st.st_ctArraySize; i++) {
						fScript.FPrintF_t("%s[%d]=(INDEX)%d;\n", ss.ss_strName, i, ((INDEX*)ss.ss_pvValue)[i]);
					}
				// if string
				} else if (stBase.st_sttType==STT_STRING) {
					// dump all members
					for(INDEX i=0; i<st.st_ctArraySize; i++) {
						fScript.FPrintF_t("%s[%d]=\"%s\";\n", ss.ss_strName, i, (const char*)(ScriptEsc(*(CTString*)ss.ss_pvValue)[i]) );
					}
				// otherwise
				} else {
					ThrowF_t("%s is an array of wrong type", ss.ss_strName);
				}
			// if float
			} else if (st.st_sttType==STT_FLOAT) {
				// dump as float
				fScript.FPrintF_t("persistent extern %sFLOAT %s=(FLOAT)%g;\n", strUser, ss.ss_strName, *(FLOAT*)ss.ss_pvValue);
			// if index
			} else if (st.st_sttType==STT_INDEX) {
				// dump as index
				fScript.FPrintF_t("persistent extern %sINDEX %s=(INDEX)%d;\n", strUser, ss.ss_strName, *(INDEX*)ss.ss_pvValue);
			// if string
			} else if (st.st_sttType==STT_STRING) {
				// dump as index
				fScript.FPrintF_t("persistent extern %sCTString %s=\"%s\";\n", strUser, ss.ss_strName, (const char*)ScriptEsc(*(CTString*)ss.ss_pvValue) );
			// otherwise
			} else {
				ThrowF_t("%s of wrong type", ss.ss_strName);
			}
		}
	} catch (char *strError) {
		WarningMessage(TRANS("Cannot save persistent symbols:\n%s"), strError);
	}
}

//안태훈 수정 시작	//(Taiwan Closed beta)(0.2)
void CShell::StoreEncodePersistentSymbols(const CTFileName &fnScript)
{
	// synchronize access to global shell
	CTSingleLock slShell(&sh_csShell, TRUE);

	try {
		CTString strContents, strUser, strTemp;

		FOREACHINDYNAMICARRAY(sh_assSymbols, CShellSymbol, itss) {
			CShellSymbol &ss = *itss;

			// if it is not persistent
			if (! (ss.ss_ulFlags & SSF_PERSISTENT)) {
				// skip it
				continue;
			}

			strUser = (ss.ss_ulFlags & SSF_USER)?"user ":"";

			// get its type
			ShellType &st = _shell_ast[ss.ss_istType];
			// if array
			if (st.st_sttType==STT_ARRAY) {
				// get base type
				ShellType &stBase = _shell_ast[st.st_istBaseType];
				CTString strType;
				// if float
				if (stBase.st_sttType==STT_FLOAT) {
					// dump all members as floats
					for(INDEX i=0; i<st.st_ctArraySize; i++) {
						strTemp.PrintF("%s[%d]=(FLOAT)%g;\n", ss.ss_strName, i, ((FLOAT*)ss.ss_pvValue)[i]);
						strContents += strTemp;
					}
				// if index
				} else if (stBase.st_sttType==STT_INDEX) {
					// dump all members as indices
					for(INDEX i=0; i<st.st_ctArraySize; i++) {
						strTemp.PrintF("%s[%d]=(INDEX)%d;\n", ss.ss_strName, i, ((INDEX*)ss.ss_pvValue)[i]);
						strContents += strTemp;
					}
				// if string
				} else if (stBase.st_sttType==STT_STRING) {
					// dump all members
					for(INDEX i=0; i<st.st_ctArraySize; i++) {
						strTemp.PrintF("%s[%d]=\"%s\";\n", ss.ss_strName, i, (const char*)(ScriptEsc(*(CTString*)ss.ss_pvValue)[i]) );
						strContents += strTemp;
					}
				// otherwise
				} else {
					ThrowF_t("%s is an array of wrong type", ss.ss_strName);
				}
			// if float
			} else if (st.st_sttType==STT_FLOAT) {
				// dump as float
				strTemp.PrintF("persistent extern %sFLOAT %s=(FLOAT)%g;\n", strUser, ss.ss_strName, *(FLOAT*)ss.ss_pvValue);
				strContents += strTemp;
			// if index
			} else if (st.st_sttType==STT_INDEX) {
				// dump as index
				strTemp.PrintF("persistent extern %sINDEX %s=(INDEX)%d;\n", strUser, ss.ss_strName, *(INDEX*)ss.ss_pvValue);
				strContents += strTemp;
			// if string
			} else if (st.st_sttType==STT_STRING) {
				// dump as index
				strTemp.PrintF("persistent extern %sCTString %s=\"%s\";\n", strUser, ss.ss_strName, (const char*)ScriptEsc(*(CTString*)ss.ss_pvValue) );
				strContents += strTemp;
			// otherwise
			} else {
				ThrowF_t("%s of wrong type", ss.ss_strName);
			}
		}
		SavePersistentSymbolFile(fnScript, strContents);
	} catch (char *strError) {
		WarningMessage(TRANS("Cannot save persistent symbols:\n%s"), strError);
	}
}
//안태훈 수정 끝	//(Taiwan Closed beta)(0.2)

// Callback function for symbol comparing in hash table
static BOOL CompareSymbols(INDEX iSymbol, const void *pSymbol)
{
	const CTString &strSymbol = *(const CTString*)pSymbol;
	const CShellSymbol &shs = _pShell->sh_assSymbols[iSymbol];
	const CTString &str = shs.ss_strName;

	return str==strSymbol;
}
// Callback function for symbol hash calculation
static ULONG CalculateHash(const void *pSymbol)
{
	const CTString &strSymbol = *(const CTString*)pSymbol;
	return strSymbol.GetHash();
}
// Callback function for hash table recreation
static void RecreateHashTable(void)
{
	// Add all symbols to hash table again
	INDEX ctshs = _pShell->sh_assSymbols.Count();
	for(INDEX ishs=0;ishs<ctshs;ishs++) {
		const CShellSymbol &ss = _pShell->sh_assSymbols[ishs];
		_shtHashTable.AddObject(&ss.ss_strName,ishs);
	}
}

extern void DumpShellHashTable(void)
{
	INDEX ctshs = _pShell->sh_assSymbols.Count();
	CStaticArray<CTString> astrSymbols;
	astrSymbols.New(ctshs);

	// for each shell symbol
	for(INDEX ishs=0;ishs<ctshs;ishs++) {
		const CShellSymbol &ss = _pShell->sh_assSymbols[ishs];
		astrSymbols[ishs] = ss.ss_strName;
	}

	_shtHashTable.DumpHashTableUsage(&astrSymbols[0],sizeof(CTString),ctshs);
}
