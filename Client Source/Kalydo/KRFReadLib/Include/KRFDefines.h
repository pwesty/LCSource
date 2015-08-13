#ifndef KRFDEFINES_H
#define KRFDEFINES_H

// Current KRF version information
#define KRF_VERSION_MAJOR 0
#define KRF_VERSION_MINOR 2
#define KRF_VERSION_STRING "\0\2"
#define KRF_VERSION_MAJOR_CHAR '\0'
#define KRF_VERSION_MINOR_CHAR '\2'

// possible results from KRF functions
enum TKResult {
	KR_IO_PENDING			=1,
	KR_OK					=0,
	KR_FILE_NOT_FOUND		=-1,
	KR_FILE_CORRUPT			=-2,
	KR_OUTPUT_FAILED		=-3,
	KR_NOT_IMPLEMENTED		=-4,
	KR_INVALID_PARAMETER	=-5,
	KR_BUFFER_TOO_SMALL		=-6,
	KR_UNSUPPORTED_FORMAT	=-7,
	KR_OUT_OF_MEMORY		=-8,
	KR_FAILED_READING		=-9,
	KR_NOTHING_TO_WRITE		=-10,
	KR_FAILED_WRITING		=-11,
	KR_ABORTED				=-12,
	KR_FILE_TOO_LARGE		=-13,
	KR_MISSING_CALLBACK		=-14,
	KR_DOWNLOAD_FAILED		=-15,
	KR_FILE_NOT_AVAILABLE	=-16,
	KR_UNKNOWN_EXCEPTION	=-17,
	KR_PLUGIN_NOT_FOUND		=-18,
	KR_PLUGIN_FAILURE		=-19,
	KR_DISK_FULL			=-20,

	// backwards compatible
	KR_FILENOTFOUND			=-1,	
	KR_FILECORRUPT			=-2,
	KR_OUTPUTFAILED			=-3,
	KR_NOTIMPLEMENTED		=-4,
	KR_INVALIDPARAMETER		=-5,
	KR_BUFFERTOOSMALL		=-6,
	KR_UNSUPPORTEDFORMAT	=-7,
	KR_OUTOFMEMORY			=-8,
	KR_FAILEDREADING		=-9,
	KR_NOTHINGTOWRITE		=-10,
	KR_FAILEDWRITING		=-11,
	KR_FILETOOLARGE			=-13
};

enum TKVarType {
	KV_SYNCHRONOUS			=1, // whether we want to use synchronous downloads.
	KV_CODEPAGE				=2, // The codepage for ascii-based file functions (only used when hooking)
	KV_KCPDEFAULTPRIO		=3, // Default priority for KCP downloads
	KV_BACKGROUND			=4  // to manually turn on the background downloader.
};

typedef TKResult TKRFResult;

// Use these macros to determine success or failure of KRF functions
#undef KRF_SUCCESS
#define KRF_SUCCESS(result) ((result) >= KR_OK)
#undef KRF_FAILURE
#define KRF_FAILURE(result) ((result) < KR_OK)

typedef void (TDirectoryListingCallback)(const char* directory, const char* filename, int isDirectory, int insideKRF, void* userData);
typedef void (TKCPDownloadResultCallback)(unsigned int errorType, 
								   unsigned int errorCode,void* id, 
								   void* buffer, unsigned __int64 bufSize);
typedef void (TDownloadKCPFile)(const char* packageName, unsigned int index, unsigned __int64 start, unsigned __int64 size, 
								TKCPDownloadResultCallback* callback, void* buffer, void* id);

typedef void (TCallOnAppThread)(void (*function)(void*), void* id);
typedef void (TKCPRequestResultCallback)(const char* fileName, TKResult result, void* id);

#define KFILE void

#ifdef WIN32
#include <windows.h>
#define TPlayerHandle HINSTANCE
#else
#define TPlayerHandle void*
#endif

#ifndef LLONG
#if _MSC_VER <= 1200
	#define LLONG __int64
#else
	#define LLONG long long
#endif
#endif

#endif //KRFDEFINES_H
