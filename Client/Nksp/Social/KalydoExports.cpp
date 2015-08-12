// these functions can be changed directly, or the convenience class TWin32Application
// can be used. Your choice!

#include "StdH.h"
#ifdef KALYDO
#include "TApplication.h"
#include "KalydoExports.h"

extern "C"
{

bool initEngine(TPlayerHandle playerModuleHandle, TWindowHandle hWnd, const char* appDir)
{
	// create and initialize the application
	application = TApplication::create();
	return application->init(playerModuleHandle, hWnd, appDir);
}

void destroyEngine()
{
	// destroy the application, if it is still there.
	if (application != NULL)
	{
		delete application;
		application = NULL;
	}
}

void startApplication()
{
	// start the application
	if (application != NULL)
		application->start();
}

void endApplication()
{
	// end the application
	if (application != NULL)
		application->end();
}

bool update()
{
	// Do one step of the application
	if (application != NULL)
		return application->update();
	return false;
}

unsigned int getLogFileName(char* name, unsigned int size)
{
	// DEPRECATED
	return 0;
}

unsigned int getCrashInfo(char* name, unsigned int size)
{
	if (application != NULL)
		return application->getCrashInfo(name, size);
	return 0;
}

bool call(const char* method, const TBrowserData* args, unsigned int noArgs)
{
	if (application != NULL)
		return application->callFromBrowser(method, args, noArgs);
	return false;
}
void sendRequestResult(unsigned int errorType, unsigned int errorCode, void* id, 
					   void* message, unsigned int length)
{
	// Send a request result
	if (application != NULL)
		application->requestResult(errorType, errorCode, id, message, length);
}

void sendProgressUpdate(const char* progressText, float progress, void* id)
{
	//progressText is DEPRECATED, will be removed

	// Send progress of request
	if (application != NULL)
		application->progressUpdate(progress, id);
}

bool injectMessage(injectMessageParamsDef)
{
	// Inject a windows message.
	if (application != NULL)
		return application->injectMessage(injectMessageParams);
	return false;
}

int getProtocolVersion()
{
	return KALYDOPROTOCOL_VERSION;
}

}

#endif //KALYDO