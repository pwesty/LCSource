#include "StdH.h"
#ifdef KALYDO
#include "TPlayerInterface.h"
#include "PlayerInterfaceDef.h"
#include "Exception.h"

#include <stdio.h>

TPlayerInterface::TPlayerInterface()
:	m_IsInitialized(false)
{
	m_RequestBalance = NULL;
	m_GetPPMState = NULL;
	m_StartPPM = NULL;
	m_StopPPM = NULL;
	m_GetPPMCost = NULL;
	m_GetUserInfo = NULL;
	m_GetApplicationInfo = NULL;
	m_GetSessionInfo = NULL;
	m_SendAppStatistics = NULL;
	m_SendXmlData = NULL;
	m_RequestXmlData = NULL;
	m_RequestAppQuantityRatio = NULL;
	m_RequestUserInfo = NULL;
	m_AddCallback = NULL;
	m_CallBrowser = NULL;
	m_SetPackagePriority = NULL;
	m_RequestPackage = NULL;
	m_RequestPackageCheck = NULL;
	m_HasPackage = NULL;
	m_AddLogFileName = NULL;
	m_RequestCredentials = NULL;
	m_RequestCredentialsFrom = NULL;
	m_RequestAdvertisement = NULL;
	m_SetPackagePriority = NULL;
	m_SetDefaultPackagePriority = NULL;
	m_SetReservedAppBandwidth = NULL;
	m_CancelPackageRequest = NULL;
	m_PausePackageRequest = NULL;
	m_ResumePackageRequest = NULL;
	m_DownloadKCPFile = NULL;
	m_GetRequestedPackagesSize = NULL;
	m_CallOnAppThread = NULL;
}

TPlayerInterface::~TPlayerInterface()
{
}

void TPlayerInterface::init(TPlayerHandle playerModuleHandle)
{
	if(m_IsInitialized) return;
	KASSERT(playerModuleHandle != NULL, "Player module handle was not set.", "Set a valid player module handle.", ET_PROGENGINE, SL_ERROR);

	m_PlayerHandle = playerModuleHandle;

	m_RequestBalance = (TRequestBalance)getProcedure(m_PlayerHandle, "requestBalance");
	if(m_RequestBalance == NULL) handleError("requestBalance");
	m_GetPPMState = (TGetPPMState)getProcedure(m_PlayerHandle, "getPPMState");
	if(m_GetPPMState == NULL) handleError("getPPMState");
	m_StartPPM = (TStartPPM)getProcedure(m_PlayerHandle, "startPPM");
	if(m_StartPPM == NULL) handleError("startPPM");
	m_StopPPM = (TStopPPM)getProcedure(m_PlayerHandle, "stopPPM");
	if(m_StopPPM == NULL) handleError("stopPPM");
	m_GetPPMCost = (TGetPPMCost)getProcedure(m_PlayerHandle, "getPPMCost");
	if(m_GetPPMCost == NULL) handleError("getPPMCost");
	m_GetUserInfo = (TGetUserInfo)getProcedure(m_PlayerHandle, "getUserInfo");
	if(m_GetUserInfo == NULL) handleError("getUserInfo");
	m_GetApplicationInfo = (TGetApplicationInfo)getProcedure(m_PlayerHandle, "getApplicationInfo");
	if(m_GetApplicationInfo == NULL) handleError("getApplicationInfo");
	m_GetSessionInfo = (TGetSessionInfo)getProcedure(m_PlayerHandle, "getSessionInfo");
	if(m_GetSessionInfo == NULL) handleError("getSessionInfo");
	m_SendAppStatistics = (TSendAppStatistics)getProcedure(m_PlayerHandle, "sendAppStatistics");
	if(m_SendAppStatistics == NULL) handleError("sendAppStatistics");
	m_SendXmlData = (TSendXmlData)getProcedure(m_PlayerHandle, "sendXmlData");
	if(m_SendXmlData == NULL) handleError("sendXmlData");
	m_RequestXmlData = (TRequestXmlData)getProcedure(m_PlayerHandle, "requestXmlData");
	if(m_RequestXmlData == NULL) handleError("requestXmlData");
	m_RequestAppQuantityRatio = (TRequestAppQuantityRatio)getProcedure(m_PlayerHandle, "requestAppQuantityRatio");
	if(m_RequestAppQuantityRatio == NULL) handleError("requestAppQuantityRatio");
	m_RequestUserInfo = (TRequestUserInfo)getProcedure(m_PlayerHandle, "requestUserInfo");
	if(m_RequestUserInfo == NULL) handleError("requestUserInfo");
	m_AddCallback = (TAddCallback)getProcedure(m_PlayerHandle, "addCallback");
	if(m_AddCallback == NULL) handleError("addCallback");
	m_CallBrowser = (TCallBrowser)getProcedure(m_PlayerHandle, "call");
	if(m_CallBrowser == NULL) handleError("call");
	m_RequestPackage = (TRequestPackage)getProcedure(m_PlayerHandle, "requestPackage");
	if(m_RequestPackage == NULL) handleError("requestPackage");
	m_RequestPackageCheck = (TRequestPackageCheck)getProcedure(m_PlayerHandle, "requestPackageCheck");
	if(m_RequestPackageCheck == NULL) handleError("requestPackageCheck");
	m_HasPackage = (THasPackage)getProcedure(m_PlayerHandle, "hasPackage");
	if(m_HasPackage == NULL) handleError("hasPackage");
	m_GetRequestedPackagesSize = (TGetRequestedPackagesSize)getProcedure(m_PlayerHandle, "getRequestedPackagesSize");
	if(m_GetRequestedPackagesSize == NULL) handleError("getRequestedPackagesSize");
	m_AddLogFileName = (TAddLogFileName)getProcedure(m_PlayerHandle, "addLogFileName");
	if(m_AddLogFileName == NULL) handleError("addLogFileName");
	m_RequestCredentials = (TRequestCredentials)getProcedure(m_PlayerHandle, "requestCredentials");
	if(m_RequestCredentials == NULL) handleError("requestCredentials");	
	m_RequestCredentialsFrom = (TRequestCredentialsFrom)getProcedure(m_PlayerHandle, "requestCredentialsFrom");
	if(m_RequestCredentialsFrom == NULL) handleError("requestCredentialsFrom");
	m_RequestAdvertisement = (TRequestAdvertisement)getProcedure(m_PlayerHandle, "requestAdvertisement");
	if(m_RequestAdvertisement == NULL) handleError("requestAdvertisement");	
	m_DownloadKCPFile = (TDownloadKCPFilePtr)getProcedure(m_PlayerHandle, "downloadKCPFile");
	if(m_DownloadKCPFile == NULL) handleError("downloadKCPFile");	
	m_SetPackagePriority = (TSetPackagePriority)getProcedure(m_PlayerHandle, "setPackagePriority");
	if(m_SetPackagePriority == NULL) handleError("setPackagePriority");
	m_SetDefaultPackagePriority = (TSetDefaultPackagePriority)getProcedure(m_PlayerHandle, "setDefaultPackagePriority");
	if(m_SetDefaultPackagePriority == NULL) handleError("setDefaultPackagePriority");
	m_SetReservedAppBandwidth = (TSetReservedAppBandwidth)getProcedure(m_PlayerHandle, "setReservedAppBandwidth");
	if(m_SetReservedAppBandwidth == NULL) handleError("setReservedAppBandwidth");
	m_CancelPackageRequest = (TCancelPackageRequest)getProcedure(m_PlayerHandle, "cancelPackageRequest");
	if(m_CancelPackageRequest == NULL) handleError("cancelPackageRequest");
	m_PausePackageRequest = (TPausePackageRequest)getProcedure(m_PlayerHandle, "pausePackageRequest");
	if(m_PausePackageRequest == NULL) handleError("pausePackageRequest");
	m_ResumePackageRequest = (TResumePackageRequest)getProcedure(m_PlayerHandle, "resumePackageRequest");
	if(m_ResumePackageRequest == NULL) handleError("resumePackageRequest");
	m_CallOnAppThread = (TCallOnAppThreadPtr)getProcedure(m_PlayerHandle, "callOnAppThread");
	if(m_CallOnAppThread == NULL) handleError("callOnAppThread");

	m_IsInitialized = true;
}

void TPlayerInterface::requestAppQuantityRatio(void* id)
{
	KASSERT(m_IsInitialized, "Player interface was not initialized.", "Select a correct player dll", ET_PROGGEN, SL_WARNING);
	if(!m_IsInitialized || m_RequestAppQuantityRatio == NULL) return;
	m_RequestAppQuantityRatio(id);
}

void TPlayerInterface::requestBalance(void* id)
{
	KASSERT(m_IsInitialized, "Player interface was not initialized.", "Select a correct player dll", ET_PROGGEN, SL_WARNING);
	if(!m_IsInitialized || m_RequestBalance == NULL) return;
	m_RequestBalance(id);
}

unsigned int TPlayerInterface::getPPMState()
{
	KASSERT(m_IsInitialized, "Player interface was not initialized.", "Select a correct player dll", ET_PROGGEN, SL_WARNING);
	if(!m_IsInitialized || m_GetPPMState == NULL) return PS_STOPPED;
	return m_GetPPMState();
}

void TPlayerInterface::startPPM(void* id)
{
	KASSERT(m_IsInitialized, "Player interface was not initialized.", "Select a correct player dll", ET_PROGGEN, SL_WARNING);
	if(!m_IsInitialized || m_StartPPM == NULL) return;
	m_StartPPM(id);
}

void TPlayerInterface::stopPPM()
{
	KASSERT(m_IsInitialized, "Player interface was not initialized.", "Select a correct player dll", ET_PROGGEN, SL_WARNING);
	if(!m_IsInitialized || m_StopPPM == NULL) return;
	m_StopPPM();
}

unsigned int TPlayerInterface::getPPMCost()
{
	KASSERT(m_IsInitialized, "Player interface was not initialized.", "Select a correct player dll", ET_PROGGEN, SL_WARNING);
	if(!m_IsInitialized || m_GetPPMCost == NULL) return 0;
	return m_GetPPMCost();
}

unsigned int TPlayerInterface::getUserInfo(unsigned int type, void* info, unsigned int size)
{
	KASSERT(m_IsInitialized, "Player interface was not initialized.", "Select a correct player dll", ET_PROGGEN, SL_WARNING);
	if(!m_IsInitialized || m_GetUserInfo == NULL) return 0;
	return m_GetUserInfo(type, info, size);
}

unsigned int TPlayerInterface::getApplicationInfo(unsigned int type, void* info, unsigned int size)
{
	KASSERT(m_IsInitialized, "Player interface was not initialized.", "Select a correct player dll", ET_PROGGEN, SL_WARNING);
	if(!m_IsInitialized || m_GetApplicationInfo == NULL) return 0;
	return m_GetApplicationInfo(type, info, size);
}

unsigned int TPlayerInterface::getSessionInfo(unsigned int type, void* info, unsigned int size)
{
	KASSERT(m_IsInitialized, "Player interface was not initialized.", "Select a correct player dll", ET_PROGGEN, SL_WARNING);
	if(!m_IsInitialized || m_GetSessionInfo == NULL) return 0;
	return m_GetSessionInfo(type, info, size);
}

void TPlayerInterface::sendAppStatistics(const TAppStatisticsPair* pairs, unsigned int noPairs, void* id)
{
	KASSERT(m_IsInitialized, "Player interface was not initialized.", "Select a correct player dll", ET_PROGGEN, SL_WARNING);
	if(!m_IsInitialized || m_SendAppStatistics == NULL) return;
	m_SendAppStatistics(pairs, noPairs, id);
}

void TPlayerInterface::sendXmlData(const char* xmlData, void* id)
{
	KASSERT(m_IsInitialized, "Player interface was not initialized.", "Select a correct player dll", ET_PROGGEN, SL_WARNING);
	if(!m_IsInitialized || m_SendXmlData == NULL) return;
	m_SendXmlData(xmlData, id);
}

void TPlayerInterface::requestXmlData(unsigned int type, const void* requestType, unsigned int size, void* id)
{
	KASSERT(m_IsInitialized, "Player interface was not initialized.", "Select a correct player dll", ET_PROGGEN, SL_WARNING);
	if(!m_IsInitialized || m_RequestXmlData == NULL) return;
	m_RequestXmlData(type, requestType, size, id);
}

void TPlayerInterface::requestUserInfo(char* username, void* id)
{
	KASSERT(m_IsInitialized, "Player interface was not initialized.", "Select a correct player dll", ET_PROGGEN, SL_WARNING);
	if(!m_IsInitialized || m_RequestUserInfo == NULL) return;
	m_RequestUserInfo(username, id);
}

bool TPlayerInterface::addCallback(const char* _event, const char* method)
{
	KASSERT(m_IsInitialized, "Player interface was not initialized.", "Select a correct player dll", ET_PROGGEN, SL_WARNING);
	if(!m_IsInitialized || m_AddCallback == NULL) return false;
	return m_AddCallback(_event, method);
}

bool TPlayerInterface::call(const char* _event, const TBrowserData* args, unsigned int noArgs)
{
	KASSERT(m_IsInitialized, "Player interface was not initialized.", "Select a correct player dll", ET_PROGGEN, SL_WARNING);
	if(!m_IsInitialized || m_CallBrowser == NULL) return false;
	return m_CallBrowser(_event, args, noArgs);
}

void TPlayerInterface::requestPackage(const char* packageName, void* id)
{
	KASSERT(m_IsInitialized, "Player interface was not initialized.", "Select a correct player dll", ET_PROGGEN, SL_WARNING);
	if(!m_IsInitialized || m_RequestPackage == NULL) return;
	m_RequestPackage(packageName, id);
}

void TPlayerInterface::requestPackageCheck(const char* packageName, void* id)
{
	KASSERT(m_IsInitialized, "Player interface was not initialized.", "Select a correct player dll", ET_PROGGEN, SL_WARNING);
	if(!m_IsInitialized || m_RequestPackageCheck == NULL) return;
	m_RequestPackageCheck(packageName, id);
}

void TPlayerInterface::cancelPackageRequest(const char* packageName)
{
	KASSERT(m_IsInitialized, "Player interface was not initialized.", "Select a correct player dll", ET_PROGGEN, SL_WARNING);
	if(!m_IsInitialized || m_CancelPackageRequest == NULL) return;
	m_CancelPackageRequest(packageName);
}

void TPlayerInterface::pausePackageRequest(const char* packageName)
{
	KASSERT(m_IsInitialized, "Player interface was not initialized.", "Select a correct player dll", ET_PROGGEN, SL_WARNING);
	if(!m_IsInitialized || m_PausePackageRequest == NULL) return;
	m_PausePackageRequest(packageName);
}

void TPlayerInterface::resumePackageRequest(const char* packageName)
{
	KASSERT(m_IsInitialized, "Player interface was not initialized.", "Select a correct player dll", ET_PROGGEN, SL_WARNING);
	if(!m_IsInitialized || m_ResumePackageRequest == NULL) return;
	m_ResumePackageRequest(packageName);
}

unsigned LLONG TPlayerInterface::getRequestedPackagesSize()
{
	KASSERT(m_IsInitialized, "Player interface was not initialized.", "Select a correct player dll", ET_PROGGEN, SL_WARNING);
	if(!m_IsInitialized || m_GetRequestedPackagesSize == NULL) return 0;
	return m_GetRequestedPackagesSize();
}

void TPlayerInterface::setPackagePriority(const char* packageName, int priority)
{
	KASSERT(m_IsInitialized, "Player interface was not initialized.", "Select a correct player dll", ET_PROGGEN, SL_WARNING);
	if(!m_IsInitialized || m_SetPackagePriority == NULL) return;
	m_SetPackagePriority(packageName, priority);
}

void TPlayerInterface::setDefaultPackagePriority(int priority)
{
	KASSERT(m_IsInitialized, "Player interface was not initialized.", "Select a correct player dll", ET_PROGGEN, SL_WARNING);
	if(!m_IsInitialized || m_SetDefaultPackagePriority == NULL) return;
	m_SetDefaultPackagePriority(priority);
}

void TPlayerInterface::setReservedAppBandwidth(float bandwidth)
{
	KASSERT(m_IsInitialized, "Player interface was not initialized.", "Select a correct player dll", ET_PROGGEN, SL_WARNING);
	if(!m_IsInitialized || m_SetReservedAppBandwidth == NULL) return;
	m_SetReservedAppBandwidth(bandwidth);
}

void TPlayerInterface::downloadKCPFile(const char* packageName, unsigned int index, unsigned LLONG start, unsigned LLONG size, 
									   TKCPResultCallback* callback, void* buffer, void* id)
{
	KASSERT(m_IsInitialized, "Player interface was not initialized.", "Select a correct player dll", ET_PROGGEN, SL_WARNING);
	if(!m_IsInitialized || m_DownloadKCPFile == NULL) return;
	m_DownloadKCPFile(packageName, index, start, size, callback, buffer, id);
}

bool TPlayerInterface::hasPackage(const char* packageName)
{
	KASSERT(m_IsInitialized, "Player interface was not initialized.", "Select a correct player dll", ET_PROGGEN, SL_WARNING);
	if(!m_IsInitialized || m_HasPackage == NULL) return false;
	return m_HasPackage(packageName);
}

void TPlayerInterface::addLogFileName(const char* fileName)
{
	KASSERT(m_IsInitialized, "Player interface was not initialized.", "Select a correct player dll", ET_PROGGEN, SL_WARNING);
	if(!m_IsInitialized || m_AddLogFileName == NULL) return;
	m_AddLogFileName(fileName);
}

void TPlayerInterface::requestCredentials(void* id)
{
	KASSERT(m_IsInitialized, "Player interface was not initialized.", "Select a correct player dll", ET_PROGGEN, SL_WARNING);
	if(!m_IsInitialized || m_RequestCredentials == NULL) return;
	m_RequestCredentials(id);
}

void TPlayerInterface::requestCredentialsFrom(const char* url, void* id)
{
	KASSERT(m_IsInitialized, "Player interface was not initialized.", "Select a correct player dll", ET_PROGGEN, SL_WARNING);
	if(!m_IsInitialized || m_RequestCredentialsFrom == NULL) return;
	m_RequestCredentialsFrom(url, id);
}

void TPlayerInterface::requestAdvertisement(unsigned int contentType, void* id)
{
	KASSERT(m_IsInitialized, "Player interface was not initialized.", "Select a correct player dll", ET_PROGGEN, SL_WARNING);
	if(!m_IsInitialized || m_RequestAdvertisement == NULL) return;
	m_RequestAdvertisement(contentType, id);
}

void TPlayerInterface::handleError(const char* methodName)
{
	char error[10240];
#if _MSC_VER >= 1400
	sprintf_s(error, "Failed getting '%s' address.\nError code: %d; The method could be DEPRECATED.\n", methodName, getLastError());
#else
	sprintf(error, "Failed getting '%s' address.\nError code: %d; The method could be DEPRECATED.\n", methodName, getLastError());
#endif
	KASSERT(false, error, "Select correct dll", ET_PROGGEN, SL_WARNING);
}

TPlayerHandle TPlayerInterface::getPlayerHandle()
{
	return m_PlayerHandle;
}
#endif KALYDO