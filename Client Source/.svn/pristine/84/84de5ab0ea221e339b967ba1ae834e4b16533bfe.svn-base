/** @class Win32PlayerInterfaceDef "Win32PlayerInterfaceDef.h"
*	@author M.G.T.Titulaer
*
*	This file defines the dll interface definitions of the player.
*/

#ifdef KALYDO
#ifndef PLAYERINTERFACEDEF_H
#define PLAYERINTERFACEDEF_H

#define BROWSERDATAVERSION 1
#define MAX_STRING_LENGTH 10240 //For now: Arbitrary max string param length of 10K! 10K Should be enough for everyone!

#include "Defines.h"

//used in the interface between plugin and loader
typedef unsigned int TBrowserDataType;
static const TBrowserDataType BDT_NULL	 = 0;
static const TBrowserDataType BDT_STRING = 1;
static const TBrowserDataType BDT_BOOL	 = 2;
static const TBrowserDataType BDT_INT	 = 3;
static const TBrowserDataType BDT_DOUBLE = 4;

struct TBrowserData
{
	unsigned int version;

	TBrowserDataType type;

	char stringValue[MAX_STRING_LENGTH];
	bool boolValue;
	int intValue;
	double doubleValue;

	TBrowserData()
	{
		version = BROWSERDATAVERSION;
		type = BDT_NULL;
		memset(stringValue, 0, MAX_STRING_LENGTH);
		boolValue = false;
		intValue = 0;
		doubleValue = 0.0;
	}
};

struct TAppStatsPair
{
	char name[MAX_STRING_LENGTH];
	int value;
	TAppStatsPair()
	{
		memset(name, 0, MAX_STRING_LENGTH);
		value = 0;
	}
	
	TAppStatsPair(const char* _name, int _value)
	{
		kStrCpy(name, MAX_STRING_LENGTH, _name);
		value = _value;
	}
};

/** for internal use */
struct TAppStatisticsPair
{
	char name[MAX_STRING_LENGTH];
	char value[MAX_STRING_LENGTH];
	TAppStatisticsPair()
	{
		memset(name, 0, MAX_STRING_LENGTH);
		memset(value, 0, MAX_STRING_LENGTH);
	}
};

struct TAdvertisement
{
	char* adContent;
	unsigned int contentType;
	unsigned int contentSize;
	char clickurl[MAX_STRING_LENGTH];
	float showSecs;
	TAdvertisement()
	{
		showSecs = 0.0f;
		contentSize = 0;
		adContent = NULL;
		contentType = 0;
		memset(clickurl, 0, MAX_STRING_LENGTH);
	}
};

//used by getSessionInfo
#define SI_INITTICKET		   3

//used by getApplicationInfo
#define AI_APPNAME			   2

//used by getUserInfo
#define UI_USERNAME			   0
#define UI_ALL				   1

//used by requestXmlData
#define RD_USERNAME			   0

// The status value returned by payment requests.
//do not change these value, they are in the server protocol!!
typedef unsigned int TPaymentStatus;
const TPaymentStatus SR_INSUFFICIENTFUNDS  = 0; 
const TPaymentStatus SR_SUCCESS			   = 1;
const TPaymentStatus SR_PENDING			   = 2;
const TPaymentStatus SR_BALANCE			   = 3;
const TPaymentStatus SR_QUANTITYRATIO	   = 4;
const TPaymentStatus SR_NOSTATUS		   = 5;

typedef unsigned int TPPMState;
const TPPMState PS_STARTED  = 0;
const TPPMState PS_STOPPED = 1;
const TPPMState PS_NOTAPPLICABLE = 2; // used in the exe game loader

struct TPaymentResult
{
	TPaymentStatus status;
	unsigned int balance;
	float quantityRatio;
};

typedef unsigned int TPackageError;
const TPackageError PE_EXTRACT	= 1;
const TPackageError PE_HASH		= 2;

struct TPackageResult
{
	const char* name;
	const char* installPath;
};

struct TPackagePartResult
{
	const char* name;
	char* buffer;
	unsigned LLONG bufferSize;
};

struct TCredentials
{
	const char* name;
	const char* password;
};

typedef void (TKCPResultCallback)(unsigned int errorType, unsigned int errorCode, 
								  void* id, void* buffer, unsigned LLONG bufferSize);

typedef unsigned int TConnectionErrorType;
const TConnectionErrorType CE_PROGRESS = -1;
const TConnectionErrorType CE_NOERROR = 0;
const TConnectionErrorType CE_HTTP	  = 1;
const TConnectionErrorType CE_SERVER  = 2;
const TConnectionErrorType CE_PLAYER  = 3;

typedef void (*TRequestBalance)(void* id);
typedef unsigned int (*TGetPPMState)();
typedef void (*TStartPPM)(void* id);
typedef void (*TStopPPM)();
typedef unsigned int (*TGetPPMCost)();
typedef void (*TRequestUserInfo)(const char* username, void* id);
typedef unsigned int (*TGetUserInfo)(unsigned int type, void* info, 
										unsigned int size);
typedef unsigned int (*TGetApplicationInfo)(unsigned int type, void* info, 
										unsigned int size);
typedef unsigned int (*TGetSessionInfo)(unsigned int type, void* info, 
										unsigned int size);
typedef void (*TSendAppStatistics)(const TAppStatisticsPair* pairs, unsigned int noPairs, void* id);
typedef void (*TSendXmlData)(const char* xmlData, void* id);
typedef void (*TRequestXmlData)(unsigned int type, const void* requestType, 
								unsigned int size, void* id);
typedef void (*TRequestAppQuantityRatio)(void* id);
typedef bool (*TAddCallback)(const char* _event, const char* method);
typedef bool (*TCallBrowser)(const char* _event, const TBrowserData* args, unsigned int noArgs);
typedef void (*TRequestPackage)(const char* packageName, void* id);
typedef void (*TRequestPackageCheck)(const char* packageName, void* id);
typedef unsigned LLONG (*TGetRequestedPackagesSize)();
typedef void (*TSetPackagePriority)(const char* packageName, int priority);
typedef void (*TSetDefaultPackagePriority)(int priority);
typedef void (*TSetReservedAppBandwidth)(float bandwidth);
typedef void (*TDownloadKCPFilePtr)(const char* packageName, unsigned int index, 
								 unsigned LLONG start, unsigned LLONG size, 
								 TKCPResultCallback* callback, void* buffer, void* id);
typedef void (*TCancelPackageRequest)(const char* packageName);
typedef void (*TPausePackageRequest)(const char* packageName);
typedef void (*TResumePackageRequest)(const char* packageName);
typedef bool (*THasPackage)(const char* packageName);
typedef void (*TAddLogFileName)(const char* fileName);
typedef void (*TRequestCredentials)(void* id);
typedef void (*TRequestCredentialsFrom)(const char* url, void* id);
typedef void (*TRequestAdvertisement)(unsigned int contentType, void* id);
typedef void (*TCallOnAppThreadPtr)(void (*function)(void*), void* id);

#endif //PLAYERINTERFACEDEF_H
#endif //KALYDO