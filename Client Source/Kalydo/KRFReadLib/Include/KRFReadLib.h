/** @file "KRFReadLib.h"
 *  @author M.W.A.Roza
 *
 * This file contains export functions for the KRF library.
 * All strings must be supplied in UTF-8 format.
 *
 * Note that this library is thread safe.
 */

#ifndef KRFLIB_H
#define KRFLIB_H

#ifdef __cplusplus 
	extern "C" { 
#endif 

#ifndef KALYDOEXPORT
	#ifdef WIN32
		#define KALYDOEXPORT __declspec(dllimport)
	#else
		#define KALYDOEXPORT extern
	#endif
#endif

#include "KRFDefines.h"

/** @name Generic lib functions */
//@{
/**
 * Initializes the KRFReadLib.
 * @param applicationDirectory  The root directory of your application. All files contained in KRF's are 
 *								virtually added to the filesystem in this directory.
 */
KALYDOEXPORT TKRFResult krfReadInit(const char* applicationDirectory);

/**
 * Quits the KRFReadLib. No functions can be called afterwards.
 */
KALYDOEXPORT TKRFResult krfReadQuit();
//@}

/**
 * Enables file I/O hooking; so no additional code changes are needed.
 *
 * Returns:
 *			KR_OK when successful
 *			KR_FILE_NOT_FOUND if EasyHook32.dll does not exist
 *			KR_FILE_CORRUPT if EasyHook32.dll is corrupted or has the wrong exports
 *			KR_UNSUPPORTED_FORMAT if a function could not be hooked. (this is not necessarily a failure,
 *								 however results are undefined in this case).
 *
 * NOTE: This is currently only implemented in Win32!
 */
KALYDOEXPORT TKRFResult krfEnableHooks();

/** @name Sequence file functions */
//@{
/**
 * Sets the sequence file. Every time a file is read from disk regardless of whether they were
 * already contained in a KRF, its name is appended to this file.
 * 
 * The sequence file can be used to determine preload sequence and optimize packages. 
 * Note that this should only be used on local test machines before packaging, 
 * and not online on client machines.
 *
 * The sequence file is written in the following format:
 * 
 * file1<enter>
 * file2<enter>
 *
 * and denotes the sequence of files that are preloaded, so better optimized packages can be created. can be NULL.
 */
KALYDOEXPORT TKRFResult krfEnableSequenceFile();

/** 
 * Do the actual writing of the sequence file.
 */
KALYDOEXPORT TKRFResult krfWriteSequenceFile(const char* sequenceFile);
//@}

/** @name Package file functions */
//@{
/**
 * Loads a package, and adds it contents to the local index of files.
 */
KALYDOEXPORT TKRFResult krfLoadPackage(const char* krfFileName);

/**
 * Unloads a package. Note that no files included can be read afterwards; and currently open files may be truncated.
 */
KALYDOEXPORT TKRFResult krfUnloadPackage(const char* krfFileName);

/**
 * Writes the (null-terminated) hash code of the index in character array 'hash'.
 */
KALYDOEXPORT TKRFResult krfGetIndexHash(const char* krfFileName, char hash[65]);

/** 
 * Sets the function that downloads a file from a KCP 
 *
 * Returns KR_OK
 * NOTE: If using Kalydo Player, use krfSetPlayerHandle instead.
 */
KALYDOEXPORT TKRFResult krfSetKCPDownloadCallback(TDownloadKCPFile* kcpDownloadCallback);

/** 
 * Sets the function that handles callbacks back to the application
 * NOTE: if this function is not set, all actions from KRFLib are done synchronously which may cause framedrops!
 *
 * Returns KR_OK
 * NOTE: If using Kalydo Player, use krfSetPlayerHandle instead.
 */
KALYDOEXPORT TKRFResult krfSetCallOnAppThread(TCallOnAppThread* callOnAppThread);

/** 
 * Sets the handle to the Kalydo Player so the KRF Lib can communicate with the Player
 * NOTE: this call can replace the above two functions that relay function pointers from the Player to the KRF lib.
 *
 * Returns KR_OK
 */
KALYDOEXPORT TKRFResult krfSetPlayerHandle(TPlayerHandle playerHandle);

/** 
 * Requests a file from a KCP, calls the callback when downloading is complete or when something goes wrong
 *
 * Returns:
 *
 *			KR_OK if the file is already available (the callback will be called directly)
 *			KR_IO_PENDING if the file is being downloaded or checked (the callback will be called after downloading)
 *			KR_MISSING_CALLBACK if krfSetPlayerHandle wasn't called (no callback will be called)
 *			KR_FILE_NOT_FOUND if the file is unavailable for download. (no callback will be called)
 *			KR_FILE_NOT_AVAILABLE if there is no callback supplied and the file is not available.
 *
 * The callback is called with one of the following results:
 *
 *			KR_OK if the file is available and ready to use (can be called synchronously or asynchronously)
 *			KR_OUT_OF_MEMORY if there is not enough memory available to download the file
 *			KR_FILE_CORRUPT if the file downloaded was different than expected
 *			KR_OUTPUT_FAILED if the file could not be written to disk
 *			KR_DOWNLOAD_FAILED if there was a server error and the file could not be downloaded
 *			KR_DISK_FULL if the file was not written because the disk is full
 */
KALYDOEXPORT TKRFResult krfRequestKCPFile(const char* fileName, TKCPRequestResultCallback* callback, void* id);

/** 
 * Requests a range from a KCP file, calls the callback when downloading is complete or when something goes wrong
 *
 * Returns:
 *
 *			KR_OK if the file is already available (the callback will be called directly)
 *			KR_IO_PENDING if the file is being downloaded or checked (the callback will be called after downloading)
 *			KR_MISSING_CALLBACK if krfSetPlayerHandle wasn't called (no callback will be called)
 *			KR_FILE_NOT_FOUND if the file is unavailable for download. (no callback will be called)
 *			KR_FILE_NOT_AVAILABLE if there is no callback supplied and the file is not available.
 *			KR_INVALID_PARAMETER if the requested range could not be satisfied.
 *
 * The callback is called with one of the following results:
 *
 *			KR_OK if the file is available and ready to use (can be called synchronously or asynchronously)
 *			KR_OUT_OF_MEMORY if there is not enough memory available to download the file
 *			KR_FILE_CORRUPT if the file downloaded was different than expected
 *			KR_OUTPUT_FAILED if the file could not be written to disk
 *			KR_DOWNLOAD_FAILED if there was a server error and the file could not be downloaded
 *			KR_DISK_FULL if the file was not written because the disk is full
 */
KALYDOEXPORT TKRFResult krfRequestKCPRange(const char* fileName, LLONG start, LLONG size, TKCPRequestResultCallback* callback, void* id);

/** 
 * Sets the priority of a single KCP file download. If a file with a higher priority is requested while another is being downloaded,
 * the lower priority download will be paused until the high priority file is finished.
 * NOTE: The value -2147483648 (INT_MIN) is reserved for internal use.
 *
 * Returns:
 *
 *			KR_OK if the priority is changed succesfully.
 *			KR_FILE_NOT_FOUND if the file was not found in the virtual file system.
 *			KR_INVALID_PARAMETER if a reserved value is used.
 */
KALYDOEXPORT TKRFResult krfSetKCPFilePriority(const char* fileName, int priority);

/**
 * Sets a variable.
 *
 * Possible values:
 *
 *			KV_SYNCHRONOUS set to non-zero if kopen/hooked file open calls to files in KCP packages should be downloaded synchronous (default 0)
 *							WARNING: if you open a KRF file in the thread where the player calls the callback, deadlock may occur!
 *			KV_CODEPAGE is the codepage used for ascii-based file functions, default is system-default.
 *			KV_KCPDEFAULTPRIO Sets the priority of all subsequent KCP requests. If a file with a higher priority is requested while another is being downloaded,
 *							the lower priority download will be paused until the high priority file is finished.
 *			KV_BACKGROUND set to non-zero to have the KRF lib download all streaming files in the background at low priority.
 */
KALYDOEXPORT TKResult krfSetVariableInt(int variable, int value);

//@}

/**
 * Opens a file. Access is ignored, just added so you can replace fopen by kopen without problems
 */
KALYDOEXPORT KFILE* kopen(const char* fileName, const char* access);

/** @name File functions */
//@{
/**
 * Reads an array of count elements, each one with a size of size bytes, from the stream 
 * and stores them in the buffer. The position indicator of the stream is advanced by the 
 * total amount of bytes read. The total amount of bytes read if successful is (size * count)
 */
KALYDOEXPORT size_t kread(void* buffer, size_t size, size_t count, KFILE* kfile);

/**
 * closes the file.
 */
KALYDOEXPORT int kclose(KFILE* kfile);

/**
 * returns the current position.
 */
KALYDOEXPORT __int64 ktell(KFILE* kfile);

/**
 * Get a string from a stream
 */
KALYDOEXPORT char* kgets(char* string, int n, KFILE* kfile);

/**
 * Get a unicode string from a stream
 * NOTE: only supports UTF-16 LE!
 */
KALYDOEXPORT wchar_t* kgetws(wchar_t* string, int n, KFILE* kfile);

/**
 * Returns whether the file is completely read
 */
KALYDOEXPORT int keof(KFILE* kfile);

/**
 * Returns the file size
 */
KALYDOEXPORT __int64 kgetSize(KFILE* kfile);

/**
 * Returns the file pointer to the beginning of the file
 */
KALYDOEXPORT void kreset(KFILE* kfile);

/**
 * Skips 'count' number of bytes (a.k.a. forwards the file pointer
 * the given number of characters (from the current))
 * Returns the current position from the start, or -1 if kfile == NULL or count is negative.
 */
KALYDOEXPORT __int64 kskip(__int64 count, KFILE* kfile);

/**
 * returns whether the file is contained in the KRF.
 */
KALYDOEXPORT int kfileExists(const char* fileName);

//@}

/** @name Directory functions */
//@{
/**
 * Does a directory listing by calling the given callback function for every file.
 * Returns the number of files (and/or directories) found.
 * Optionally, an extension can be given (without the '.'); if all files are to be listed, return NULL.
 * set outsideKRF to non-zero if you also want to receive files outside KRF files
 * set recursive to non-zero to enable recursive dir listing.
 * (e.g. for debugging or user generated content).
 * Userdata is passed onto the callback function to identify this call.
 * NOTE: will only work inside KRF's where the flag 'Allow directory listings' was set!
 */
KALYDOEXPORT int krfGetDirectoryListing(
						TDirectoryListingCallback* callback, const char* directory, 
						const char* extension, int outsideKRF, int recursive, void* userData);
//@}
#ifdef __cplusplus 
	}
#endif 

#endif // KRFLIB_H
