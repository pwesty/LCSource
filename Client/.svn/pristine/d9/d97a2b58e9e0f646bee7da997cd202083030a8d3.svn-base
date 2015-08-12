#ifndef _UTIL_H_
#define _UTIL_H_
#ifdef PRAGMA_ONCE
#pragma once
#endif

#pragma warning(disable:4002)
#define		SAFE_DELETE(x)			{ if(x) { delete x; x = NULL; } }
#ifndef SAFE_ARRAY_DELETE
#	define		SAFE_ARRAY_DELETE(x)	if(x) { delete[] x; x = NULL; }
#endif

extern void ENGINE_API LogOutput( const char* format, ... );
extern void ENGINE_API LogOutputChat( const char* format, ... );

#define StackMultiByteToWideChar(codePage, input, inputSize, output) \
	int output##Size = 0; \
	wchar_t* output = NULL; \
	if ((input) != NULL) \
{ \
	output##Size = MultiByteToWideChar(codePage, 0, (input), (inputSize), NULL, 0); \
	output = (wchar_t*)alloca((##output##Size) * sizeof(wchar_t)); \
	MultiByteToWideChar(codePage, 0, (input), (inputSize), (output), (##output##Size)); \
}

#define StackWideCharToMultiByte(codePage, input, inputSize, output) \
	int output##Size = 0; \
	char* output = NULL; \
	if ((input) != NULL) \
{ \
	output##Size = WideCharToMultiByte(codePage, 0, (input), (inputSize), NULL, 0, NULL, NULL); \
	output = (char*)alloca((##output##Size) * sizeof(char)); \
	WideCharToMultiByte(codePage, 0, (input), (inputSize), (output), (##output##Size), NULL, NULL); \
}

#if		defined(_MSC_VER) && (_MSC_VER >= 1600)
#	ifdef	_DEBUG
#		define		LOG_DEBUG(x, ...)			LogOutput( x, __VA_ARGS__ )
#	else	// _DEBUG
#		if	defined(VER_TEST)
#			define		LOG_DEBUG(x, ...)				LogOutputChat( x, __VA_ARGS__ )
#		else	// VER_TEST
#			define		LOG_DEBUG(x)
#		endif	// VER_TEST
#	endif	// _DEBUG
#else
#	define		LOG_DEBUG(x)
#endif

#endif	// _UTIL_H_