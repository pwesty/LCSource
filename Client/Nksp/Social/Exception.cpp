#include "StdH.h"
#ifdef KALYDO
#include <assert.h>
#include <stdio.h>

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <WTypes.h>
#else
#include <errno.h>
#endif

#include "Exception.h"
#include "TException.h"

void KALYDOERROR(const char* err, const char* solution, int type,
					int severityLevel, const char* file, int line)
{
	// type is code
	const char* t;
	switch (type)
	{
	case ET_PROGGEN:
		t = "General programming";
		break;
	case ET_PROGSCRIPT:
		t = "Script programming";
		break;
	case ET_PROGENGINE:
		t = "GDK programming (please report to Eximion)";
		break;
	case ET_INPUT:
		t = "Input (file or other input is incorrect)";
		break;
	case ET_HARDWARE:
		t = "Hardware error";
		break;
	case ET_UNKNOWN:
	default:
		t = "Unknown";
	}

#ifdef WIN32
#ifdef _DEBUG
	char buf[4096];
#if _MSC_VER >= 1400
	sprintf_s(buf, 4096, "%s(%d) : error : %s => %s", file, line, err, solution);
#else
	sprintf(buf, "%s(%d) : error : %s => %s", file, line, err, solution);
#endif
	wchar_t wbuf[4096];
	MultiByteToWideChar(CP_UTF8, 0, buf, -1, wbuf, 4096);
	OutputDebugStringW(wbuf);
	MessageBoxW(NULL, wbuf, L"error", MB_ICONEXCLAMATION | MB_OK | MB_TOPMOST);
	assert(false);
#endif

#ifdef REPORTERROR
	if (severityLevel > SL_WARNING)
	{
		ULONG_PTR args[1];
		args[0] = reinterpret_cast<ULONG_PTR>( 
			new TException(err, solution, t, type, severityLevel, 
			GetLastError(), file, line) );
		RaiseException(KALYDOASSERT, 0, 1, args);
	}
#endif
#else
	fprintf(stderr, "%s\n", err);
	if (severityLevel > SL_WARNING)
	{
		TException* exception = new TException(err, solution, t, type, severityLevel, 
			errno, file, line);
		// TODO: raise this exception!!
	}
#endif

}

#endif //KALYDO
