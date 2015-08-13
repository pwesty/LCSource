/** @file "Defines.h"
 *  @author M.F.A.Schroders, R.Binnendijk, M.G.T.Titulaer, M.W.A.Roza
 *
 *  This file contains the defines for all Kalydo applications.
 */

#ifdef KALYDO
#ifndef DEFINES_H
#define DEFINES_H

#define KALYDOPROTOCOL_VERSION 1

#ifdef WIN32
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
	#include <stdlib.h>

	#define KALYDOEXPORT __declspec(dllexport)
	#define injectMessageParamsDef HWND hWnd, UINT type, WPARAM wParam, LPARAM lParam
	#define injectMessageParams hWnd, type, wParam, lParam
	#define TPlayerHandle HINSTANCE
	#define TWindowHandle HWND
	#define TGameHandle HINSTANCE
	#define openLib(dllname) LoadLibrary(dllname)
	#define getProcedure(handle, function) GetProcAddress(handle, function)
	#define getLastError() GetLastError()

#else
	// for all other OSes, we use SDL
	union SDL_Event;
	#define KALYDOEXPORT extern
	#define injectMessageParamsDef SDL_Event* event
	#define injectMessageParams event
	#define TPlayerHandle void*
	#define TWindowHandle void*
	#define openLib(dllname) dlopen(dllname, RTLD_LAZY)
	#define getProcedure(handle, function) dlsym(handle, function)
	#define getLastError() errno
#endif

#ifndef LLONG
#if _MSC_VER <= 1200
	#define LLONG __int64
#else
	#define LLONG long long
#endif
#endif

#ifndef NULL
#define NULL (0)
#endif

#ifndef kStrCpy
#if _MSC_VER >= 1400
	#define kStrCpy(dest, size, source) strcpy_s(dest, size, source)
	#define kItoa(value, buffer, size, radix) _itoa_s(value, buffer, size, radix)
#else
	#define kStrCpy(dest, size, source) strcpy(dest, source)
	#define kItoa(value, buffer, size, radix) itoa(value, buffer, radix)
#endif
#endif

#endif //DEFINES_H
#endif //KALYDO