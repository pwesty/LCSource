/** @class TPlayerInterface "TPlayerInterface.h"
 *	@author M.G.T.Titulaer
 *
 *	This class encapsulates the interface of the Kalydo player. The player can
 *	be accessed through an instance of this class.
 */

#ifdef KALYDO
#ifndef TWIN32PLAYERINTERFACE_H
#define TWIN32PLAYERINTERFACE_H

#include "Defines.h"
#include "PlayerInterfaceDef.h"

class TPlayerInterface
{
public:
	/**
	 * Performs a request to retrieve the ratio that the currently loaded 
	 * application is owned by the user. The ratio can be retrieved in 
	 * the engine by implementing and exporting the sendRequestResult function.
	 * 
	 * id is a user defined id and helps to identify the request.
	 */
	void requestAppQuantityRatio(void* id);

	/**
	 * Performs a request to retrieve the balance of the current user.
	 * The balance can be retrieved in the engine by implementing and exporting 
	 * the sendRequestResult function.
	 * 
	 * id is a user defined id and helps to identify the balance request.
	 */
	void requestBalance(void* id);

	/**
	 * Returns the current state of the pay-per-minute payment loop.
	 * 
	 * Possible values are:
	 * PS_STARTED - pay-per-minute is running
	 * PS_STOPPED - pay-per-minute was stopped (default)
	 * PS_NOTAPPLICABLE - used offline for testing purposes.
	 */
	TPPMState getPPMState();

	/**
	 * Starts the pay-per-minute payment loop. Pay-per-minute is post paid, 
	 * in this case meaning that the first pay-per-minute request will be 
	 * about half a minute after the call to startPPM. Each next 
	 * pay-per-minute request will be about one minute apart. 
	 * The pay-per-minute state will be PS_STARTED.
	 *
	 * id is a user defined id and helps to identify the request.
	 */
	void startPPM(void* id);

	/**
	 * Stops the pay-per-minute loop. When a pay-per-minute loop is
	 * stopped, the time remaining until the next pay-per-minute request 
	 * is stored. Then, when the loop is started again the next 
	 * pay-per-minute request is performed after the stored time has 	
	 * elapsed. Each next pay-per-minute request will be about one minute 
	 * apart. 
	 * The pay-per-minute state will be PS_STOPPED.
	 */
	void stopPPM();

	/**
	 * returns the number of eCoins that pay-per-minute of the loaded 
	 * application costs.
	 */
	unsigned int getPPMCost();

	/**
	 * Can be used to retrieve user info in the engine, like the user name
	 * of the current player and other profile information. 
	 *
	 * type specifies the type of user info that is being requested.
	 * Possible values: 
	 * - UI_USERNAME: info is a character buffer that receives the current user name.
	 * - UI_ALL: info is a character buffer that receives an xml with all user info.
	 *
	 * info is a void pointer to a buffer which contains the user info and size 
	 * specifies the size of the buffer pointed to by info.
	 * Returns the required buffer size if the buffer is too small, otherwise 
	 * returns 0.
	 */
	unsigned int getUserInfo(unsigned int type, void* info, unsigned int size);

	/**
	 * Can be used to retrieve application info in the engine, like the application name
	 * of the current application.
	 *
	 * type specifies the type of application info that is being requested.
	 * Possible values: 
	 * - AI_APPNAME: info is a character buffer that receives the current application name.
	 * - AI_ALL: info is a character buffer that receives an xml with all 
	 * information on the current application in the current session.
	 *
	 * info is a void pointer to a buffer which contains the user info and size 
	 * specifies the size of the buffer pointed to by info.
	 * Returns the required buffer size if the buffer is too small, otherwise 
	 * returns 0.
	 */
	unsigned int getApplicationInfo(unsigned int type, void* info, unsigned int size);

	/**
	 * Can be used to retrieve session info in the engine, like the initiation ticket.
	 *
	 * type specifies the type of user info that is being requested.
	 * Possible values: 
	 * - SI_INITTICKET: info is a character buffer that receives the current initiation ticket.
	 *
	 * info is a void pointer to a buffer which contains the user info and size 
	 * specifies the size of the buffer pointed to by info.
	 * Returns the required buffer size if the buffer is too small, otherwise 
	 * returns 0.
	 */
	unsigned int getSessionInfo(unsigned int type, void* info, unsigned int size);

	/**
	 * Can be used to retrieve user info in the engine, like the language 
	 * settings of the user or other profile information. For now only 
	 * username NULL is allowed, which means profile information for the 
	 * current user is requested.
	 *
	 * username is a pointer to a null terminated string that specifies 
	 * the name of the user of which the user information is requested. 
	 * If username is NULL the user info of the current player is requested.
	 * id is a user defined id that helps to identify the request.
	 * Note: this is a server request!
	 */
	void requestUserInfo(char* username, void* id);

	/**
	 * Can be used to send application specific statistics to be stored onto 
	 * the server (persistent data).
	 * This application specific data consists of a name/value pair.
	 * pairs is a continues array of appStatisticsPair structs.
	 * noPairs indicates the number of arguments that args consists of.
	 *
	 * id is a user defined id that helps to identify the request.
	 * Note: this is a server request!
	 */
	void sendAppStatistics(const TAppStatisticsPair* pairs, unsigned int noPairs, void* id);

	/**
	 * Can be used to store information of the current user onto 
	 * the server (persistent data). The xml data that can be send, can be of any 
	 * form as long it is in valid xml format. The function can not be used in 
	 * combination with existing save structures, in the future a method like 
	 * sendBinaryData can be introduced for this purpose.
	 * Note: this is a server request!
	 *
	 * xmlData is the data to be send 
	 * (for example <highscore>50</highscore><score>8</score>)
	 * id is a user defined id that helps to identify the request.
	 */
	void sendXmlData(const char* xmlData, void* id);

	/**
	 * Can be used to retrieve information stored onto the server by a 
	 * sendXmlData request (persistent data). The function can not be used 
	 * in combination with existing save structures, in the future a 
	 * method like requestBinaryData can be introduced for 	this purpose.
	 * Currently requestXmlData only does a request for xml data for the 
	 * current user!
	 * Note: this is a server request!
	 *
	 * type specifies the type of request. Possible values: 
	 *
	 * - RD_USERNAME: requestType is a null terminated character string to a user name
	 *
	 * requestType the type of request.
	 * size is the size of requestType.
	 * id is a user defined id that helps to identify the request.
	 */
	void requestXmlData(unsigned int type, const void* requestType, unsigned int size, void* id);

	/**
	 * The addCallback function registers an engine/application method as 
	 * callable from within a browser. After successful invocation of 
	 * addCallback, the registered method can be called.
	 *
	 * event is the name by which the browser can invoke a specified method.
	 * method is the method that should be invoked.
	 *
	 * Returns true on success, otherwise false.
	 */
	bool addCallback(const char* _event, const char* method);

	/**
	 * Call a function in the browser.
	 *
	 * event must always be an event string to a callback function that was 
	 * registered from within the browser.
	 * args is a continues array of browserData structs.
	 * noArgs indicates the number of arguments that args consists of.
	 *
	 * Returns true if the call is valid, otherwise returns false.
	 */
	bool call(const char* _event, const TBrowserData* args, unsigned int noArgs);

	/**
	 * Requests that a package is installed by the player, including all of its dependencies. 
	 * The request is handled asynchronously.
	 * If the package or its dependencies are already installed, this function will perform a content check to 
	 * see if the content has been modified or not.
	 * If the package or one of its dependencies is not yet installed, this function will download the package or take it 
	 * from the cache if available, and extract it. After this, the package content is checked to 
	 * see if the content has been modified or not.
	 *
	 * packageName specifies the package to check.
	 * id is a user defined id that helps to identify the request.
	 *
	 * Note: this is a server request!
	 */
	void requestPackage(const char* packageName, void* id);

	/**
	 * Checks if the content for a specified package, including all of its dependencies, is modified or not. 
	 * The request is handled asynchronously.
	 *
	 * packageName specifies the package to check.
	 * id is a user defined id that helps to identify the request.
	 */
	void requestPackageCheck(const char* packageName, void* id);

	/**
	 * Stops any previous call to the requestPackage function where a package download was required. 
	 * If no call was placed or no download was required this function does nothing.
	 * 
	 * packageName specifies the package to cancel.
	 */
	void cancelPackageRequest(const char* packageName);

	/**
	 * Pauses any previous call to the requestPackage function where a package download was required. 
	 * If no call was placed or no download was required this function does nothing.
	 * 
	 * packageName specifies the package to pause.
	 */
	void pausePackageRequest(const char* packageName);

	/**
	 * Resumes any previously paused call to the requestPackage function where a package download was required. 
	 * If no call was placed or no download was required, or the request was not paused, this function does nothing.
	 * 
	 * packageName specifies the package to resume.
	 */
	void resumePackageRequest(const char* packageName);

	/**
	 * Returns the total size of all packages that have currently been requested in MB.
	 */
	unsigned LLONG getRequestedPackagesSize();

	/**
	 * Sets the priority of the package with the given name. 
	 * By default KCP file requests have priority over package requests and all requested 
	 * packages have a priority of 0, unless otherwise specified by setDefaultPackagePriority.
	 * 
	 * A requested package or KCP file with a higher priority is downloaded and installed 
	 * before a requested package or KCP file with a lower priority.
	 */
	void setPackagePriority(const char* packageName, int priority);

	/**
	 * Sets the default package priority.
	 * By default KCP file requests have priority over package requests and all requested 
	 * packages have a priority of 0. If this method is called all newly requested packages 
	 * will have the given priority by default.
	 *
	 * A requested package with a higher priority is downloaded and installed 
	 * before a requested package with a lower priority.
	 */
	void setDefaultPackagePriority(int priority);

	/**
	 * Sets the bandwidth usage (MB/s) that is reserved for the application.
	 * The Kalydo Player will try not to use all available bandwidth and keep the 
	 * given reserved bandwidth unused.
	 *
	 * If the reserved bandwidth is set to more than 50% of the available bandwidth, the 
	 * actual reserved bandwidth is 50% of the available bandwidth.
	 */
	void setReservedAppBandwidth(float bandwidth);

	/**
	 * Downloads a new file from a KCP package.
	 *
	 * packageName specifies the name of the package from which a part is 
	 * downloaded.
	 * start is the start offset in bytes of the part to download.
	 * size is the size in bytes of the part to download.
	 * buffer is a buffer in which the result of the request is placed.
	 * id is a user defined id that helps to identify the request.
	 * 
	 * The result of the request is returned on the given callback.
	 */
	void downloadKCPFile(const char* packageName, unsigned int index, 
		unsigned LLONG start, unsigned LLONG size, 
		TKCPResultCallback* callback, void* buffer, void* id);

	/**
	 * Checks if the latest version (at the time of application start) of the
	 * specified package is available. 
	 * NOTE: DOES NOT CHECK WHETHER THE PACKAGE CONTENT IS MODIFIED!
	 * 
	 * packageName specifies the package to check.
	 *
	 * The function returns true if the required version of the specified 
	 * package and the required version of its dependencies are installed and exist in the root directory. 
	 * If one of the packages does not exist or is not the correct version, then false is returned.
	 */
	bool hasPackage(const char* packageName);
	
	/**
	 * Adds a log file name to be sent when a crash occurs.
	 */
	void addLogFileName(const char* fileName);	

	/**
	 * Requests for credentials of the current user.
	 * The credentials can be used to log into the application's servers, 
	 * and thus create a single sign-on solution between the application's 
	 * servers and kalydo.
	 * The request is handled asynchronously.
	 *
	 * id is a user defined id that helps to identify the request.
	 */
	void requestCredentials(void* id);

	/**
	 * Requests for credentials of the current user on the given url.
	 * The credentials can be used to log into the application's servers, 
	 * and thus create a single sign-on solution between the application's 
	 * servers and kalydo.
	 * The request is handled asynchronously.
	 *
	 * url is a url on which the credentials are requested. This can
	 * have the following format: https://<ip-addres>:<portnumber>
	 * Where the portnumber is optional, the ip addres can be a domain name
	 * and the protocol can also be http://
	 * If url is NULL, this method behaves as requestCredentials.
	 * id is a user defined id that helps to identify the request.
	 */
	void requestCredentialsFrom(const char* url, void* id);

	/**
	 * Requests for a new advertisement, targeted to the current user.
	 * The application is responsible for displaying and handling this 
	 * advertisement.
	 *
	 * contentType specifies the type of advertisement to receive.
	 * Possible values: RA_IMAGE (the ad is provided in an image format)
	 * id is a user defined id that helps to identify the request.
	 * 
	 * The result of the request is returned in the form of 
	 * a TAdvertisement struct on the sendRequestResult function.
	 */
	void requestAdvertisement(unsigned int contentType, void* id);

	TPlayerHandle getPlayerHandle();

	/**
	 * Initialize the interface.
	 */
	void init(TPlayerHandle playerModuleHandle);

	TPlayerInterface();
	virtual ~TPlayerInterface();

private:

	void handleError(const char* methodName);

	/** The handle to the player. */
	TPlayerHandle	m_PlayerHandle;

	/** Indicates if the interface is initialized or not. */
	bool m_IsInitialized;

	TRequestBalance				m_RequestBalance;
	TGetPPMState				m_GetPPMState;
	TStartPPM					m_StartPPM;
	TStopPPM					m_StopPPM;
	TGetPPMCost					m_GetPPMCost;
	TSendXmlData				m_SendXmlData;
	TRequestXmlData				m_RequestXmlData;
	TRequestAppQuantityRatio	m_RequestAppQuantityRatio;
	TRequestUserInfo			m_RequestUserInfo;
	TGetUserInfo				m_GetUserInfo;
	TGetApplicationInfo			m_GetApplicationInfo;
	TGetSessionInfo				m_GetSessionInfo;
	TSendAppStatistics			m_SendAppStatistics;
	TAddCallback				m_AddCallback;
	TCallBrowser				m_CallBrowser;
	TRequestPackage				m_RequestPackage;
	TRequestPackageCheck		m_RequestPackageCheck;
	THasPackage					m_HasPackage;
	TAddLogFileName				m_AddLogFileName;
	TRequestCredentials			m_RequestCredentials;
	TRequestCredentialsFrom		m_RequestCredentialsFrom;
	TRequestAdvertisement		m_RequestAdvertisement;
	TDownloadKCPFilePtr			m_DownloadKCPFile;
	TSetPackagePriority			m_SetPackagePriority;
	TSetDefaultPackagePriority	m_SetDefaultPackagePriority;
	TSetReservedAppBandwidth	m_SetReservedAppBandwidth;
	TCancelPackageRequest		m_CancelPackageRequest;
	TPausePackageRequest		m_PausePackageRequest;
	TResumePackageRequest		m_ResumePackageRequest;
	TGetRequestedPackagesSize	m_GetRequestedPackagesSize;
	TCallOnAppThreadPtr			m_CallOnAppThread;
};

#endif

#endif // KALYDO