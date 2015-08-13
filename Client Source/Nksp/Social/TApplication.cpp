#include "StdH.h"
#ifdef KALYDO
#include "TApplication.h"
#include <string.h>
// the application
TApplication* application = NULL;



TApplication::TApplication()
:	m_ApplicationDirectory(NULL)
,	m_MyWindowHandle(NULL)
,	m_ApplicationInstance(NULL)
{

}

TApplication::~TApplication()
{
	// cleanup
	if (m_ApplicationDirectory != NULL)
		delete [] m_ApplicationDirectory;
}

bool TApplication::init(TPlayerHandle playerModuleHandle, TWindowHandle hWnd, 
		const char* appDir)
{
	// set my members
	m_KalydoPlayerInterface.init(playerModuleHandle);
	m_MyWindowHandle = hWnd;

	// the application directory is dir/app ...
	m_ApplicationDirectory = new char[strlen(appDir) + 1];
	kStrCpy(m_ApplicationDirectory, strlen(appDir) + 1, appDir);

	init();
	return true;
}

void TApplication::init()
{
	// do nothing by default
}

void TApplication::start()
{
	// do nothing by default
}

void TApplication::end()
{
	// do nothing by default
}

bool TApplication::update()
{
	// avoid 100% cpu usage
#ifdef WIN32
	Sleep(1);
#else
	usleep(10);
#endif
	return false;
}

bool TApplication::injectMessage(injectMessageParamsDef)
{
	// NOTE: this function is only called when building the game as a DLL!
	// nothing is handled by default
	return false;
}

void TApplication::requestResult(unsigned int errorType, 
		unsigned int errorCode, void* id, void* message, unsigned int mLength)
{
	if (id == NULL)
		// no callback function ...
		return;

	THandler* handler = reinterpret_cast<THandler*>(id);
	(*handler)(errorType, errorCode, message, mLength);
}

void TApplication::progressUpdate(float progress, void* id)
{
 // Called by player to give proggress updates on package downloads.
	if (id == NULL)
		// no callback function ...
		return;

	THandler* handler = reinterpret_cast<THandler*>(id);
	(*handler)(CE_PROGRESS, (int)progress, NULL, 0);
}

void TApplication::requestAppQuantityRatio(THandler* id)
{
	m_KalydoPlayerInterface.requestAppQuantityRatio(id);
}

void TApplication::requestBalance(THandler* id)
{
	m_KalydoPlayerInterface.requestBalance(id);
}

TPPMState TApplication::getPPMState()
{
	return m_KalydoPlayerInterface.getPPMState();
}

void TApplication::startPPM(THandler* id)
{
	m_KalydoPlayerInterface.startPPM(id);
}

void TApplication::stopPPM()
{
	m_KalydoPlayerInterface.stopPPM();
}

unsigned int TApplication::getPPMCost()
{
	return m_KalydoPlayerInterface.getPPMCost();
}

unsigned int TApplication::getUserInfo(unsigned int type, void* info, unsigned int size)
{
	return m_KalydoPlayerInterface.getUserInfo(type, info, size);
}

unsigned int TApplication::getApplicationInfo(unsigned int type, void* info, unsigned int size)
{
	return m_KalydoPlayerInterface.getApplicationInfo(type, info, size);
}

unsigned int TApplication::getSessionInfo(unsigned int type, void* info, unsigned int size)
{
	return m_KalydoPlayerInterface.getSessionInfo(type, info, size);
}

void TApplication::sendAppStatistics(const TAppStatsPair* pairs, unsigned int noPairs, THandler* id)
{
	TAppStatisticsPair* playerPairs = new TAppStatisticsPair[noPairs];
	
	for (unsigned int i = 0; i < noPairs; ++i)
	{
		kStrCpy(playerPairs[i].name, MAX_STRING_LENGTH, pairs[i].name);
		kItoa(pairs[i].value, playerPairs[i].value, MAX_STRING_LENGTH, 10);
	}
	m_KalydoPlayerInterface.sendAppStatistics(playerPairs, noPairs, id);

	delete[] playerPairs;
}

void TApplication::sendAppStatistics(const char* name, int value, THandler* id)
{
	TAppStatsPair pair(name, value);
	
	sendAppStatistics(&pair, 1, id);
}

void TApplication::sendLoggedIn(THandler* id)
{
	sendAppStatistics("LoggedIn", 1, id);
}

void TApplication::sendServerSelected(THandler* id)
{
	sendAppStatistics("ServerSelected", 1, id);
}

void TApplication::sendCharCreated(THandler* id)
{
	sendAppStatistics("CharCreated", 1, id);
}

void TApplication::sendGameWorldEntered(THandler* id)
{
	sendAppStatistics("GameWorldEntered", 1, id);
}

void TApplication::sendXmlData(const char* xmlData, THandler* id)
{
	m_KalydoPlayerInterface.sendXmlData(xmlData, id);
}

void TApplication::requestXmlData(unsigned int type, const void* requestType, unsigned int size, THandler* id)
{
	m_KalydoPlayerInterface.requestXmlData(type, requestType, size, id);
}

bool TApplication::addCallback(const char* _event, const char* method)
{
	return m_KalydoPlayerInterface.addCallback(_event, method);
}

bool TApplication::callBrowser(const char* _event, const TBrowserData* args, unsigned int noArgs)
{
	return m_KalydoPlayerInterface.call(_event, args, noArgs);
}

void TApplication::requestPackage(const char* packageName, THandler* id)
{
	m_KalydoPlayerInterface.requestPackage(packageName, id);
}

void TApplication::requestPackageCheck(const char* packageName, THandler* id)
{
	m_KalydoPlayerInterface.requestPackageCheck(packageName, id);
}

void TApplication::cancelPackageRequest(const char* packageName)
{
	m_KalydoPlayerInterface.cancelPackageRequest(packageName);
}

void TApplication::pausePackageRequest(const char* packageName)
{
	m_KalydoPlayerInterface.pausePackageRequest(packageName);
}

void TApplication::resumePackageRequest(const char* packageName)
{
	m_KalydoPlayerInterface.resumePackageRequest(packageName);
}

unsigned LLONG TApplication::getRequestedPackagesSize()
{
	return m_KalydoPlayerInterface.getRequestedPackagesSize();
}

void TApplication::setPackagePriority(const char* packageName, int priority)
{
	m_KalydoPlayerInterface.setPackagePriority(packageName, priority);
}

void TApplication::setDefaultPackagePriority(int priority)
{
	m_KalydoPlayerInterface.setDefaultPackagePriority(priority);
}

void TApplication::setReservedAppBandwidth(float bandwidth)
{
	m_KalydoPlayerInterface.setReservedAppBandwidth(bandwidth);
}

void TApplication::downloadKCPFile(const char* packageName, unsigned int index, 
								   unsigned LLONG start, unsigned LLONG size, 
								   TKCPResultCallback* callback, void* buffer, THandler* id)
{
	m_KalydoPlayerInterface.downloadKCPFile(packageName, index, start, size, callback, buffer, id);
}

bool TApplication::hasPackage(const char* packageName)
{
	return m_KalydoPlayerInterface.hasPackage(packageName);
}

void TApplication::addLogFileName(const char* fileName)
{
	m_KalydoPlayerInterface.addLogFileName(fileName);
}

void TApplication::requestCredentials(THandler* id)
{
	m_KalydoPlayerInterface.requestCredentials(id);
}

void TApplication::requestCredentialsFrom(const char* url, THandler* id)
{
	m_KalydoPlayerInterface.requestCredentialsFrom(url, id);
}

void TApplication::requestAdvertisement(unsigned int contentType, THandler* id)
{
	m_KalydoPlayerInterface.requestAdvertisement(contentType, id);
}

unsigned int TApplication::getCrashInfo(char* name, unsigned int size)
{
	// default behavior: no crash info
	return 0;
}

bool TApplication::callFromBrowser(const char* method , const TBrowserData* args, unsigned int noArgs)
{
	//default behavior: nothing is done with the calls from the browser.
	return false;
}

char* TApplication::getApplicationDirectory()
{
	return m_ApplicationDirectory;
}

TWindowHandle TApplication::getKalydoWindow()
{
	return m_MyWindowHandle;
}

void TApplication::setApplicationInstance(TGameHandle inst)
{
	m_ApplicationInstance = inst;
}

TGameHandle TApplication::getApplicationInstance()
{
	return m_ApplicationInstance;
}
#endif //KALYDO