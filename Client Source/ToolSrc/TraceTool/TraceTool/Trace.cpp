#if defined(_MSC_VER) && (_MSC_VER < 1400)
	#pragma warning(disable : 4786)		// identifier was truncated to '255' characters in the debug information
#endif

#include <windows.h>
#include <mmsystem.h>
#include "Trace.h"
#include "TraceTool.h"

//#define LOG_FULL_FILE_PATH  // 주석처리하면 소스 파일 이름만 나온다.
#define LOG_STRING_SIZE 1024

static LONG g_threadId = 0;
class Lock
{
public:
	// set lock to gain exclusive access to trace
	// functions
	Lock()
		{
		LONG threadId = ::GetCurrentThreadId();
		while (g_threadId != threadId)
			{
			// keep trying until successfully completed the operation
			::InterlockedCompareExchange(&g_threadId, threadId, 0);
			if (g_threadId == threadId)
				break;
			::Sleep(25);
			}
		}

	// release lock so that other threads can access 
	// trace functions
	~Lock()
		{
		// only the thread that set the lock can release it
		::InterlockedCompareExchange(&g_threadId, 0, ::GetCurrentThreadId());
		}
};

////////////////////////////////////////////////////////////////////////////////
Trace::Trace(const char *pFile, int nLine)
{
#ifdef LOG_FULL_FILE_PATH
	m_pFileName   = pFile;
#else
	const char *pFind = strrchr(pFile, '\\');
	if (pFind)
		{
		m_pFileName = pFind+1;
		}
	else
		{
		pFind = strrchr(pFile, '/');
		if (pFind)
			m_pFileName = pFind+1;
		else
			m_pFileName = pFile;
		}
#endif
	m_nLineNumber = nLine;
}

void Trace::Watch(int line, char* pData, ...)
{
	Lock lock;

	//INT3( pData != 0 );
	if (pData == 0)
		return;

	static char szString[LOG_STRING_SIZE];
	static char name[256];

	va_list vParams;
	va_start(vParams, pData);
	#if defined(_MSC_VER) && (_MSC_VER >= 1400) && (!UNDER_CE)
	vsprintf_s(szString, LOG_STRING_SIZE, pData, vParams);
	#else
	_vsnprintf(szString, LOG_STRING_SIZE, pData, vParams);
	#endif
	va_end(vParams);

	#if defined(_MSC_VER) && (_MSC_VER >= 1400) && (!UNDER_CE)
	sprintf_s(name, sizeof(name), "[%d] %s:%d", line, m_pFileName, m_nLineNumber);
	#else
	_snprintf(name, sizeof(name), "[%d] %s:%d", line, m_pFileName, m_nLineNumber);
	#endif
	TTrace::SendToWinWatch("Watch", name, szString);
}

void Trace::Watch(int level, int line, char* pData, ...)
{
	Lock lock;

	//INT3( pData != 0 );
	if (pData == 0)
		return;

	static char szString[LOG_STRING_SIZE];
	static char watchId[32];
	static char name[256];

	va_list vParams;
	va_start(vParams, pData);
	#if defined(_MSC_VER) && (_MSC_VER >= 1400) && (!UNDER_CE)
	vsprintf_s(szString, LOG_STRING_SIZE, pData, vParams);
	#else
	_vsnprintf(szString, LOG_STRING_SIZE, pData, vParams);
	#endif
	va_end(vParams);

	#if defined(_MSC_VER) && (_MSC_VER >= 1400) && (!UNDER_CE)
	sprintf_s(watchId, sizeof(watchId), "Watch-%d", level);
	sprintf_s(name, sizeof(name), "[%d] %s:%d", line, m_pFileName, m_nLineNumber);
	#else
	_snprintf(watchId, sizeof(watchId), "Watch-%d", level);
	_snprintf(name, sizeof(name), "[%d] %s:%d", line, m_pFileName, m_nLineNumber);
	#endif
	TTrace::SendToWinWatch(watchId, name, szString);
}

void Trace::Log(int level, char* pData, ...)
{
	Lock lock;

	//INT3( pData != 0 );
	if (pData == 0)
		return;

	static char szString[LOG_STRING_SIZE];
	static char traceId[32];
	static char cmt[256];

	va_list vParams;
	va_start(vParams, pData);
	#if defined(_MSC_VER) && (_MSC_VER >= 1400) && (!UNDER_CE)
	vsprintf_s(szString, LOG_STRING_SIZE, pData, vParams);
	#else
	_vsnprintf(szString, LOG_STRING_SIZE, pData, vParams);
	#endif
	va_end(vParams);

	#if defined(_MSC_VER) && (_MSC_VER >= 1400) && (!UNDER_CE)
	sprintf_s(traceId, sizeof(traceId), "Trace-%d", level);
	sprintf_s(cmt, sizeof(cmt), "%s:%d", m_pFileName, m_nLineNumber);
	#else
	_snprintf(traceId, sizeof(traceId), "Trace-%d", level);
	_snprintf(cmt, sizeof(cmt), "%s:%d", m_pFileName, m_nLineNumber);
	#endif
	TTrace::SendToWinTrace(traceId, szString, cmt);
}

////////////////////////////////////////////////////////////////////////////////
void Trace::Log(char* pData, ...)
{
	Lock lock;

	//INT3( pData != 0 );
	if (pData == 0)
		return;

	static char szString[LOG_STRING_SIZE];
	static char cmt[256];

	va_list vParams;
	va_start(vParams, pData);
	#if defined(_MSC_VER) && (_MSC_VER >= 1400) && (!UNDER_CE)
	vsprintf_s(szString, LOG_STRING_SIZE, pData, vParams);
	#else
	_vsnprintf(szString, LOG_STRING_SIZE, pData, vParams);
	#endif
	va_end(vParams);

	#if defined(_MSC_VER) && (_MSC_VER >= 1400) && (!UNDER_CE)
	sprintf_s(cmt, sizeof(cmt), "%s:%d", m_pFileName, m_nLineNumber);
	#else
	_snprintf(cmt, sizeof(cmt), "%s:%d", m_pFileName, m_nLineNumber);
	#endif
	TTrace::SendToWinTrace(NULL, szString, cmt);
}

////////////////////////////////////////////////////////////////////////////////
void Trace::Assert(BOOL bFlag, char *pStrF, ...)
{
ASSERT_RETRY:
	if (bFlag == 1)
		return;

	static char szCaption[LOG_STRING_SIZE];
	static char szString[LOG_STRING_SIZE];
	static char	szUserInfo[LOG_STRING_SIZE];

	va_list vParams;
	va_start(vParams, pStrF);
	#if defined(_MSC_VER) && (_MSC_VER >= 1400) && (!UNDER_CE)
	vsprintf_s(szString, LOG_STRING_SIZE, pStrF, vParams);
	#else
	_vsnprintf(szString, LOG_STRING_SIZE, pStrF, vParams);
	#endif
	va_end(vParams);

//	 AttribMessage(ATTRIB_MESSAGE_FROM_SYSTEM, NULL, dwLastErr, 0, sLastMsg, 1024, NULL);

	#if defined(_MSC_VER) && (_MSC_VER >= 1400) && (!UNDER_CE)
	sprintf_s(szCaption, sizeof(szCaption), "POWER ASSERT 입력값 : %d", bFlag);
	sprintf_s(szUserInfo, sizeof(szUserInfo), "[source file   : %s]\n[line number : %d]\n\n%s", m_pFileName, m_nLineNumber, szString);
	#else
	_snprintf(szCaption, sizeof(szCaption), "POWER ASSERT 입력값 : %d", bFlag);
	_snprintf(szUserInfo, sizeof(szUserInfo), "[source file   : %s]\n[line number : %d]\n\n%s", m_pFileName, m_nLineNumber, szString);
	#endif
	switch (MessageBoxA(NULL, szUserInfo, szCaption, MB_ABORTRETRYIGNORE|MB_ICONQUESTION))
		{
		case IDABORT: // 중단
			ExitProcess(0);
			break;

		case IDIGNORE: // 무시
			bFlag = 1;
			goto ASSERT_RETRY;
			break;

		case IDRETRY:  // 다시시도
			goto ASSERT_RETRY;
		}
}

////////////////////////////////////////////////////////////////////////////////
void Trace::Interrupt(BOOL bFlag)
{
#ifdef _DEBUG
	if (bFlag == 1)
		return;

	static char szCaption[LOG_STRING_SIZE];
	static char	szUserInfo[LOG_STRING_SIZE];

	#if defined(_MSC_VER) && (_MSC_VER >= 1400) && (!UNDER_CE)
	sprintf_s(szCaption, sizeof(szCaption), "INT3");
	sprintf_s(szUserInfo, sizeof(szCaption), "[source file   : %s]\n[line number : %d]", m_pFileName, m_nLineNumber);
	#else
	_snprintf(szCaption, sizeof(szCaption), "INT3");
	_snprintf(szUserInfo, sizeof(szCaption), "[source file   : %s]\n[line number : %d]", m_pFileName, m_nLineNumber);
	#endif
	MessageBoxA(NULL, szUserInfo, szCaption, MB_OK);

	__asm int 3
#endif
}

////////////////////////////////////////////////////////////////////////////////
void Trace::ValueInterrupt(float fmin, float fmax, float fvalue)
{
	if (fvalue >= fmin && fvalue <= fmax)
		return;

	static char szCaption[LOG_STRING_SIZE];
	static char	szUserInfo[LOG_STRING_SIZE];

	#if defined(_MSC_VER) && (_MSC_VER >= 1400) && (!UNDER_CE)
	sprintf_s(szCaption, sizeof(szCaption), "VINT3");
	sprintf_s(szUserInfo, sizeof(szUserInfo), "[source file   : %s]\n[line number : %d]", m_pFileName, m_nLineNumber);
	#else
	_snprintf(szCaption, sizeof(szCaption), "VINT3");
	_snprintf(szUserInfo, sizeof(szUserInfo), "[source file   : %s]\n[line number : %d]", m_pFileName, m_nLineNumber);
	#endif
	MessageBoxA(NULL, szUserInfo, szCaption, MB_OK);

	__asm int 3
}

////////////////////////////////////////////////////////////////////////////////
void Trace::Dump(const char* name, const char* buf, unsigned int bufLen)
	{
	Lock lock;

	if (name == 0 || buf == 0 || bufLen == 0)
		return;

	static char cmt[256];
	#if defined(_MSC_VER) && (_MSC_VER >= 1400) && (!UNDER_CE)
	sprintf_s(cmt, sizeof(cmt), "%s:%d", m_pFileName, m_nLineNumber);
	#else
	_snprintf(cmt, sizeof(cmt), "%s:%d", m_pFileName, m_nLineNumber);
	#endif
	TTrace::Debug()->SendDump(name, cmt, name, buf, bufLen);
	}

void Trace::Dump(int level, const char* name, const char* buf, unsigned int bufLen)
	{
	Lock lock;

	if (name == 0 || buf == 0 || bufLen == 0)
		return;

	static char traceId[32];
	static char cmt[256];

	#if defined(_MSC_VER) && (_MSC_VER >= 1400) && (!UNDER_CE)
	sprintf_s(traceId, sizeof(traceId), "Trace-%d", level);
	sprintf_s(cmt, sizeof(cmt), "%s:%d", m_pFileName, m_nLineNumber);
	#else
	_snprintf(traceId, sizeof(traceId), "Trace-%d", level);
	_snprintf(cmt, sizeof(cmt), "%s:%d", m_pFileName, m_nLineNumber);
	#endif

	TTrace::SendToWinTraceDump(traceId, name, buf, bufLen, cmt);
	}
