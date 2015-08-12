/** @class TApplication "TApplication.h"
 *	@author M.F.A.Schroders, R.Binnendijk, M.G.T.Titulaer, M.W.A.Roza
 *
 *	This class is an convenience class for integrating your application
 *  into the Kalydo technology.
 *
 *  Every character array that is communicated between the Kalydo Player
 *  and the application is formatted using the UTF-8 format. 
 */

#ifdef KALYDO
#ifndef TAPPLICATION_H
#define TAPPLICATION_H

#include "Defines.h"
#include "TPlayerInterface.h"
#include "THandler.h"

class TApplication
{
public:
	/**
	 * Default constructor.
	 */
	TApplication();

	/**
	 * Destructor, virtual so it can be overloaded.
	 */
	virtual ~TApplication();

	/** @name Functions that are called by the Kalydo Player
	 *
	 * The functions in this group are called by the Kalydo Player if necessary.
	 */
	//@{

	/**
	 * Create an application. You should define this function in your main.cpp
	 * which returns your application, derived from this class.
	 */
	static TApplication* create();

	/**
	 * Initializes the application. 
	 * 
	 * The playerModuleHandle is a handle to the 
	 * Kalydo Player that runs the application.
	 *
	 * The hWnd is the window on to which the application is allowed to render
	 * and appDir is a fully qualified working directory from where the
	 * application is started. Note that the message loop of hWnd is done
	 * inside the Kalydo player.
	 *
	 * Returns false when initialization fails and true on success.
	 */
	bool init(TPlayerHandle playerModuleHandle, TWindowHandle hWnd, 
		const char* appDir);

	/**
	 * Initializes the application. You can overload this function.
	 */
	virtual void init();

	/**
	 * Inject a message into the application. 
	 * Returns true if the message is handled, false otherwise.
	 * 
	 * Note that setting focus on your application is not allowed if the application
	 * is parented in the browser. Setting the focus can actually create the opposite
	 * effect; in that the application will lose focus. This also means that the 
	 * set focus message may not be dispatched.
	 *
	 * Also note that injectMessage may be called from several different threads.
	 */
	virtual bool injectMessage(injectMessageParamsDef);

	/**
	 * The application start function. This function is called as soon as 
	 * your application was successfully loaded and initialized
	 * by a Kalydo Player. The website has stopped showing the loading screen
	 * and the application window is shown.
	 * It's a marker function that signals the start of the update loop.
	 *
	 * This function may or may not return, both are allowed.
	 * For example, it is okay to insert the main game loop here.
	 * Otherwise, the update function can be used to step the application.
	 */
	virtual void start();

	/**
	 * The application end function. This function is called when the Kalydo Player
	 * is being shutdown and only when your application was successfully loaded and initialized. 
	 * It's a marker function that signals the end of the update loop.
	 *
	 * Note that the Kalydo Player will not call the destructor of this class; the process will
	 * simply be killed (for quick navigation in the website). So, any cleanup that is truly
	 * necessary needs to be placed here (e.g. a logoff message to a game server).
	 */
	virtual void end();

	/**
	 * Steps the application once.
	 * The control/message loop is in the Kalydo Player running the application.
	 *
	 * Returns false if the control loop should continue to update and returns 
	 * true if the control loop should be terminated.
	 */
	virtual bool update();

	/**
	 * Sends the result of a http(s) request to the application.
	 * ErrorType can be:
	 * CE_NOERROR - No error has occurred, the result is stored in message
	 * CE_HTTP	  - An http error has occurred, the errorCode contains an 
	 *				http error code (404, 500 etc.) 
	 * CE_SERVER  - A server error has occurred, the errorCode contains the
	 *			    network api error code.
	 * Id is a user defined id that was passed with a request.
	 * Message is a pointer to a buffer, with length mLength, containing 
	 * the result of the request. For instance if the application requested 
	 * for saved data, this message is the data xml. NULL on error.
	 */
	virtual void requestResult(unsigned int errorType, 
		unsigned int errorCode, void* id, void* message, unsigned int mLength);

	/**
	 * Sends the updated progress of a package request to the engine.
	 * Progress is the progress of the request in the form of a float.
	 * Id is a user defined id that was passed with a request.
	 */
	virtual void progressUpdate(float progress, void* id);
	
	/**
	 * This method is called by the Kalydo Player whenever a crash occurs
	 * (e.g. an exception is thrown by an assert, or an access violation occurs)
	 * and the data returned is put in the Kalydo Error Reporter.
	 * An example of useful information this could contain is the call stack of a
	 * scripting language. A word of warning though; this information may be corrupt
	 * if your application's memory is corrupted by e.g. a buffer overflow!
	 *
	 * name A buffer that can be filled by the application.
	 * size The size of the buffer.
	 * 
	 * Should returns the size of the needed buffer if size is too small; otherwise returns 0.
	 * If no additional information is available, always returns 0.
	 */
	virtual unsigned int getCrashInfo(char* name, unsigned int size);

	/**
	 * This method is called by the Kalydo Player whenever a call is received from the browser.
	 * 
	 * method must always be an event string to a callback function that was 
	 * registered from within the application by using the addCallback function.
	 * args is a continues array of browserData structs.
	 * noArgs indicates the number of arguments that args consists of.
	 *
	 * Returns true if the call is valid, otherwise returns false.
	 */
	virtual bool callFromBrowser(const char* method, const TBrowserData* args, unsigned int noArgs);

	//@}

	/** @name Functions that are used to talk to the Kalydo Player
	 */
	//@{

	/**
	 * Performs a request to retrieve the ratio that the currently loaded 
	 * application is owned by the user. The ratio can be retrieved in 
	 * the engine by implementing and exporting the sendRequestResult function.
	 * 
	 * id is a user defined id and helps to identify the request.
	 */
	void requestAppQuantityRatio(THandler* id);

	/**
	 * Performs a request to retrieve the balance of the current user.
	 * The balance can be retrieved in the engine by implementing and exporting 
	 * the sendRequestResult function.
	 * 
	 * id is a user defined id and helps to identify the balance request.
	 */
	void requestBalance(THandler* id);

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
	void startPPM(THandler* id);

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
	void requestUserInfo(char* username, THandler* id);

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
	void sendAppStatistics(const TAppStatsPair* pairs, unsigned int noPairs, THandler* id = NULL);

	/**
	 * Can be used to send one application specific statistics to be stored onto 
	 * the server (persistent data).
	 * This application specific data consists of a name/value pair.
	 *
	 * id is a user defined id that helps to identify the request.
	 * Note: this is a server request!
	 */
	void sendAppStatistics(const char* name, int value = 1, THandler* id = NULL);

	/**
	 * Convenience functions for creating statistics on the conversion funnel.
	 */
	//@{
	/**
	 * Send "Logged in" statistic to the Kalydo Platform.
	 *
	 * id can be used to determine whether the statistic was sucessfully sent.
	 */
	void sendLoggedIn(THandler* id = NULL);
	/**
	 * Send "Server selected" statistic to the Kalydo Platform.
	 *
	 * id can be used to determine whether the statistic was sucessfully sent.
	 */
	void sendServerSelected(THandler* id = NULL);
	/**
	 * Send "Character created" statistic to the Kalydo Platform.
	 *
	 * id can be used to determine whether the statistic was sucessfully sent.
	 */
	void sendCharCreated(THandler* id = NULL);
	/**
	 * Send "Game world entered" statistic to the Kalydo Platform.
	 *
	 * id can be used to determine whether the statistic was sucessfully sent.
	 */
	void sendGameWorldEntered(THandler* id = NULL);
	//@}

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
	void sendXmlData(const char* xmlData, THandler* id);

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
	void requestXmlData(unsigned int type, const void* requestType, unsigned int size, THandler* id);

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
	 * NOTE: the event "player.getBrowserInterface" will make the Player compile a list of all callbacks registered
	 * by the website and send that list to callFromBrowser with the "sendBrowserInterface" method string.
	 *
	 * Returns true if the call is valid and registered by the website, otherwise returns false.
	 */
	bool callBrowser(const char* _event, const TBrowserData* args, unsigned int noArgs);

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
	void requestPackage(const char* packageName, THandler* id);

	/**
	 * Checks if the content for a specified package, including all of its dependencies, is modified or not. 
	 * The request is handled asynchronously.
	 *
	 * packageName specifies the package to check.
	 * id is a user defined id that helps to identify the request.
	 */
	void requestPackageCheck(const char* packageName, THandler* id);

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
	 * Stops any previous call to the requestPackage function where a package download was required. 
	 * If no call was placed or no download was required this function does nothing.
	 * 
	 * packageName specifies the package to cancel.
	 */
	void cancelPackageRequest(const char* packageName);

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
		TKCPResultCallback* callback, void* buffer, THandler* id);

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
	 *
	 * The result of the request is returned in the form of 
	 * a TCredentials struct on the requestResult function.
	 */
	void requestCredentials(THandler* id);

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
	 *
	 * The result of the request is returned in the form of 
	 * a TCredentials struct on the requestResult function.
	 */
	void requestCredentialsFrom(const char* url, THandler* id);

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
	 * a TAdvertisement struct on the requestResult function.
	 */
	void requestAdvertisement(unsigned int contentType, THandler* id);

	/**
	 * Returns the working directory of the game.
	 * This is NOT the location of the game dll, but the default location all packages are placed
	 */
	char* getApplicationDirectory();

	/**
	 * Returns the handle to the Kalydo Loader window in the browser.
	 * This could be needed for message handling in the game.
	 */
	TWindowHandle getKalydoWindow();

	/**
	 * Some calls require the application instance as an argument.
	 * These members can be used to set and retrieve it.
	 */
	void setApplicationInstance(TGameHandle inst);
	TGameHandle getApplicationInstance();

	//@}
protected:
	TPlayerInterface m_KalydoPlayerInterface;
	/**
	 * The handle to the window to which we can render.
	 */
	TWindowHandle m_MyWindowHandle;
	/**
	 * The application directory, in UTF-8.
	 */
	char* m_ApplicationDirectory;
	/**
	 * The instance handle
	 */
	TGameHandle m_ApplicationInstance;
};

// the application
extern TApplication* application;

#endif //TAPPLICATION_H
#endif //KALYDO