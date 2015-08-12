#include "StdH.h"
#ifdef KALYDO
#include "TLastChaosApplication.h"
#include "KalydoExports.h"

HINSTANCE inst;

TApplication* TApplication::create()
{
	// define your application here!
	TApplication* app = new TLastChaosApplication();
	app->setApplicationInstance(inst);
	return app;
} 

BOOL WINAPI DllMain(
  HINSTANCE hInst,
  DWORD fdwReason,
  LPVOID)
{
	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		inst = hInst;
		break;
	case DLL_PROCESS_DETACH:
		destroyEngine();
		break;
	}

	return TRUE;
}

#endif // KALYDO