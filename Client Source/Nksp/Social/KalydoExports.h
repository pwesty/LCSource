/** @file "KalydoExports.h"
 *	@author M.F.A.Schroders, R.Binnendijk, M.G.T.Titulaer, M.W.A.Roza
 *
 *	This file defines the interface between the engine and the player. 
 *
 *	It contains the dll exports to be compiled into the application.
 *
 *  These functions can be changed directly, or the convenience class TApplication
 *  can be used.
 */

#ifdef KALYDO
#ifndef KALYDOEXPORTS_H
#define KALYDOEXPORTS_H

#include "Defines.h"

extern "C"
{
	/** @name kalydo player - engine interface
	* This list contains methods that defines the interface between the
	* kalydo player and the kalydo engine.
	*/
	//@{
	KALYDOEXPORT bool initEngine(TPlayerHandle playerModuleHandle, TWindowHandle hWnd, 
		const char* appDir);
	KALYDOEXPORT void startApplication();
	KALYDOEXPORT void endApplication();
	KALYDOEXPORT bool update();
	KALYDOEXPORT void sendRequestResult(unsigned int errorType, 
		unsigned int errorCode, void* id, void* message, unsigned int length);
	KALYDOEXPORT bool injectMessage(HWND hWnd, UINT type, 
		WPARAM wParam, LPARAM lParam);
	KALYDOEXPORT void destroyEngine();
	KALYDOEXPORT unsigned int getLogFileName(char* name, unsigned int size); // DEPRECATED
	KALYDOEXPORT unsigned int getCrashInfo(char* name, unsigned int size);
	KALYDOEXPORT bool call(const char* method, const TBrowserData* args, unsigned int noArgs);
	KALYDOEXPORT void sendProgressUpdate(const char* progressText, float progress, void* id);
	KALYDOEXPORT int getProtocolVersion();
	//@}
}

#endif //KALYDOEXPORTS_H
#endif //KALYDO