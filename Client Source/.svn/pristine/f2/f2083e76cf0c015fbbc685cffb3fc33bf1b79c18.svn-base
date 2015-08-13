/** @class TLastChaosApplication "TLastChaosApplication.h"
 *	@author M.F.A.Schroders, R.Binnendijk, M.G.T.Titulaer, M.W.A.Rozam, M.J.Tops
 *
 *	This class is an example class for integrating your application
 *  into the Kalydo technology.
 *
 *  Some samples of how the communication with the Kalydo Player
 *  works are included in the injectMessage function.
 */

#ifndef TMYAPPLICATION_H
#define TMYAPPLICATION_H

#include "TApplication.h"
//#include "KRFReadLib.h"
#include "../Kalydo/KRFReadLib/Include/KRFReadLib.h"
#include "TException.h"
#include <sstream>
#include <queue>
#include <malloc.h>
#ifndef WIN32
#include <iostream>
#include <unistd.h>
#endif

using namespace std;

#define USE_KALYDO_AUTO_LOGIN

// this macro is used to convert UTF8 strings to unicode to provide support for non-english OSes
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

class TLastChaosApplication: public TApplication
{
public:
	typedef enum __tag_Package_ID
	{
		PACID_ITEM_O = 0,
		PACID_ITEM_R,
		PACID_NPC_O,
		PACID_NPC_R,
		PACID_MOB_O,
		PACID_MOB_R,
		PACID_COM_O,
		PACID_COM_R,
		PACID_ZON_R = 100,
		PACID_ZON_O = 200,
	}PACID;

	TLastChaosApplication();
	virtual ~TLastChaosApplication();

	virtual void init();
	virtual void start();
	virtual void end();

	virtual bool update();
	virtual bool injectMessage(injectMessageParamsDef);

	virtual bool callFromBrowser(const char* method, const TBrowserData* args, unsigned int noArgs);
	virtual unsigned int getCrashInfo(char* name, unsigned int size);

	static void KCPCallback(const char* fileName, TKResult result, void* id);

	void checkRequiredKRF();
	void requestKRF();
	void requestZoneKRF( unsigned int zoneNo );
	void requestItemKRF();
	void requestMobKRF();
	void requestNPCKRF();
	void requestCommonKRF();
	void requestKCP();
	void testDataStore();
	void testCredentials();
	void sendStats();
	void toggleFullscreen();
#ifdef USE_KALYDO_AUTO_LOGIN
	void setSSOURL(const TBrowserData* args, unsigned int noArgs);
#endif
	void postNotification();
	void writeLogEntry(char* msg);

	bool getFullscreen() { return m_Fullscreen; }
	bool getFocus() { return m_Focus; }
	void setFocus(bool focus) { m_Focus = focus; }

	const std::string getID() const		{ return m_szID;	}
	const std::string getPWD() const	{ return m_szPWD;	}

#ifdef USE_KALYDO_AUTO_LOGIN
	bool IsSSOEnabled() { return m_bSSOEnabled; }
#endif

protected:
	THandler* m_PackageHandler;
	THandler* m_CredentialsHandler;
	THandler* m_SendDataHandler;
	THandler* m_RequestDataHandler;

	THandler* m_CustomHandler;

	void handlePackages(unsigned int errorType, unsigned int errorCode, void* id, 
					   void* message, unsigned int mLength);
	void handleCredentials(unsigned int errorType, unsigned int errorCode, void* id, 
						void* message, unsigned int mLength);
	void handleSendData(unsigned int errorType, unsigned int errorCode, void* id, 
					   void* message, unsigned int mLength);
	void handleRequestData(unsigned int errorType, unsigned int errorCode, void* id, 
					   void* message, unsigned int mLength);

	void handleZonePack(unsigned int errorType, unsigned int errorCode, void* id,
						void* message, unsigned int mLength );

	void showMessage(const char* message);
	
	bool m_Fullscreen, m_Focus;
	LONG m_OldWidth, m_OldHeight, m_OldPosX, m_OldPosY;
	LONG m_DWStyle, m_EXDWStyle;
	HWND m_MyParentHandle;

	TKRFResult m_ReqPackageRes;
	TKRFResult m_DLCPackageRes;
	TKRFResult m_ReqGameDataPackRes;
	int m_DataStoreTest;
	FILE* m_LogFile;
	std::string			m_szID;
	std::string			m_szPWD;
	PACID				m_enLastestID;				// only used in zone packages.

#ifdef USE_KALYDO_AUTO_LOGIN
	bool m_bSSOEnabled;
#endif

#ifdef _DEBUG
	std::string			m_szPackagePath;
#endif
};

#endif //TMYAPPLICATION_H
