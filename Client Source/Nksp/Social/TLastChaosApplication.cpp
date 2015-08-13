#include "StdH.h"
#ifdef KALYDO
#include <nksp/Social/TLastChaosApplication.h>
#include "Exception.h"
#include "TRequestId.h"
#include <ctime>
#include <Engine/Kalydo/PackageManager.h>

#include <Engine/Interface/UIManager.h>
#include <Engine/Interface/UIOption.h>

class CmdSendLogin : public Command
{
public:
	void execute() 
	{
		extern TApplication* application;
		TLastChaosApplication* app = dynamic_cast<TLastChaosApplication*>(application);

		if (app != NULL)
			app->sendLoggedIn();
	}
};

class CmdSendSelServer : public Command
{
public:
	void execute() 
	{
		extern TApplication* application;
		TLastChaosApplication* app = dynamic_cast<TLastChaosApplication*>(application);

		if (app != NULL)
			app->sendServerSelected();
	}
};

class CmdSendCreateChar : public Command
{
public:
	void execute() 
	{
		extern TApplication* application;
		TLastChaosApplication* app = dynamic_cast<TLastChaosApplication*>(application);

		if (app != NULL)
			app->sendCharCreated();
	}
};

class CmdSendEnterWorld : public Command
{
public:
	void execute() 
	{
		extern TApplication* application;
		TLastChaosApplication* app = dynamic_cast<TLastChaosApplication*>(application);

		if (app != NULL)
			app->sendGameWorldEntered();
	}
};

//--------------------------------------------------------------


TLastChaosApplication::TLastChaosApplication()
{
	// The application is being created!
	// This is a good place to initialize the members

	// The handlers below are used as callbacks for application->player requests
	// that are handled asynchronously 
	m_SendDataHandler = MAKECLASSHANDLER(this, &TLastChaosApplication::handleSendData, NULL);
	m_RequestDataHandler = MAKECLASSHANDLER(this, &TLastChaosApplication::handleRequestData, NULL);
	m_PackageHandler = MAKECLASSHANDLER(this, &TLastChaosApplication::handlePackages, NULL);
	m_CredentialsHandler = MAKECLASSHANDLER(this, &TLastChaosApplication::handleCredentials, NULL);
	m_CustomHandler = MAKECLASSHANDLER(this, &TLastChaosApplication::handleZonePack, NULL );
	
	m_Fullscreen = m_Focus = false;
	m_DataStoreTest = -1;
	m_LogFile = NULL;
// 	m_szID.clear();
// 	m_szPWD.clear();
#ifdef	USE_KALYDO_AUTO_LOGIN
	m_bSSOEnabled = true;
#endif	// USE_KALYDO_AUTO_LOGIN
}

TLastChaosApplication::~TLastChaosApplication()
{
	delete m_SendDataHandler;
	delete m_RequestDataHandler;
	delete m_PackageHandler;
	delete m_CredentialsHandler;
	delete m_CustomHandler;
}

void TLastChaosApplication::init()
{
	// The application is being initialized!

	// We need to set the current directory so we use relative paths in the rest of the code
	// However, we need to convert it to unicode first, to make sure it works on all languages
	StackMultiByteToWideChar(CP_UTF8, m_ApplicationDirectory, -1, wAppDir);
	SetCurrentDirectoryW(wAppDir);
	SetCurrentDirectoryW(L"..");

#ifdef	_DEBUG
	// 디버그 모드에서는 한 Depth 차이가 난다.
	SetCurrentDirectoryW(L"..");
#endif	// _DEBUG

	TKRFResult res = krfReadInit("");

	KASSERT(KRF_SUCCESS(res), "unable to initialize KRF lib", "check return value", ET_INPUT, SL_CRITICAL);

	// Load the required packages (the results are checked when the
	// appropriate menu options are selected that uses these files)
	TKRFResult packChar_r;
	TKRFResult packChar_o;
	TKRFResult publisher_r;
	m_ReqPackageRes = krfLoadPackage("base_r.krf");
	m_DLCPackageRes = krfLoadPackage("base_o.krf");
	m_ReqGameDataPackRes = krfLoadPackage("gamedata_r.krf");
	packChar_r = krfLoadPackage("char_r.krf");
	packChar_o = krfLoadPackage("char_o.krf");
	publisher_r = krfLoadPackage("publisher_r.krf");

	// without replacing all the fopen and fread functions in the game files
	res = krfEnableHooks();
	
	res = krfSetPlayerHandle(m_KalydoPlayerInterface.getPlayerHandle());
	// request item packages. [11/30/2011 rumist]
 	requestItemKRF();
 	requestNPCKRF();
 	requestMobKRF();
	requestCommonKRF();

	// Add the file names for your log files, so the error reporter can attach those files
	// to the error reports to provide the support team with additional information.
	addLogFileName("example.log");	

	// This is where we initialize the application itself (and pass it the Kalydo window handle)
	// KalydoInit() is an example function call

	KalydoInit(m_MyWindowHandle);

	// The application would like to display the user's name, so we retrieve it from the player
	unsigned int size = getUserInfo(UI_USERNAME, NULL, 0);
	char* userName = new char[size];
	getUserInfo(UI_USERNAME, (void*)userName, size);
	
	// KalydoGame() is an example function call that in this case returns a pointer to the game instance
//	KalydoGame()->m_UserName = userName;
#ifdef	KALYDO_SEQ_FILE
	krfEnableSequenceFile();
#endif	// KALYDO_SEQ_FILE

}

void TLastChaosApplication::start()
{
	// To allow the website where the game is loaded have buttons to mute the game or make it fullscreen,
	// we need to register those events so the Kalydo Player can let the application know when they occur
	// The first argument is the event the java script will send to the player
	// The second argument is the message the application will receive when the event occurs
	// The function that is called with this message is callFromBrowser, which is defined in this .cpp

	addCallback("app.toggleFullscreen", "toggleFullscreen");
	addCallback("app.toggleMute", "toggleMute");

#ifdef USE_KALYDO_AUTO_LOGIN
	addCallback("app.setSSOURL", "setSSOURL");
	if (!callBrowser("app.getSSOURL", NULL, 0))
	{
		// show normal login screen
		KalydoSetUserAccount( std::string(""), std::string("") );
		m_bSSOEnabled = false;
	}
	else
	{
		// don't show any login screen
	}
#endif

	// At last we can let the game know everything is ready to go
	// KalydoStart() is an example function call
	KalydoStart();

	if (_pNetwork != NULL)
	{
		_pNetwork->ga_sesSessionState.SetSendCommand(CSessionState::eKST_LOGIN, new CmdSendLogin);
		_pNetwork->ga_sesSessionState.SetSendCommand(CSessionState::eKST_SEL_SERVER, new CmdSendSelServer);
		_pNetwork->ga_sesSessionState.SetSendCommand(CSessionState::eKST_CREATE_CHAR, new CmdSendCreateChar);
		_pNetwork->ga_sesSessionState.SetSendCommand(CSessionState::eKST_ENTER_GAME, new CmdSendEnterWorld);
	}
}

void TLastChaosApplication::end()
{
	// KalydoEnd() is an example function call
	KalydoEnd();
}

bool TLastChaosApplication::update()
{
	bool retVal = false;
	
	// If we are in fullscreen and we don't have focus anymore after all messages are processed,
	// deactivate fullscreen because alt-tab has been used or another application has stolen focus.
	if (!m_Focus && m_Fullscreen)
		toggleFullscreen();

	// Make the game perform one step.
	// KalydoUpdate() is an example function call
	retVal = KalydoUpdate();
	
	// Just sleep a bit to avoid 100% cpu usage.
#ifdef WIN32
	Sleep(1);
#else
	usleep(10);
#endif

	// False means the application does NOT want to exit.
	return retVal;
}

bool TLastChaosApplication::injectMessage(injectMessageParamsDef)
{ 
	// This function is called by the Kalydo Player every time the window receives a message.
	// Please note this function will always be called in a new thread.
	// The return value of this function tells the player if the game will handle the message or not.
	
#ifdef WIN32
	switch (type)
	{
	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYDOWN:
	case WM_SYSKEYUP:
	case WM_MOUSEWHEEL:
	case WM_SETFOCUS:
	case WM_KILLFOCUS:
	// ime support code. [11/29/2011 rumist]
	case WM_IME_STARTCOMPOSITION:
	case WM_IME_CHAR:
	case WM_IME_COMPOSITION:
	case WM_INPUTLANGCHANGE:
	case WM_INPUTLANGCHANGEREQUEST:
	case WM_IME_ENDCOMPOSITION:
	case WM_IME_NOTIFY:
	{
		if (GetFocus() != getGameWindow())
			PostMessage(getGameWindow(), type, wParam, lParam);
		return true;
	}
	case WM_SIZE:
	{
		SetWindowPos(getGameWindow(), NULL, 0, 0, LOWORD(lParam), HIWORD(lParam), 0);
	}
	default:
		break;
	}
#else
	// TODO: other OSes
#endif
	// Return that we haven't handled the message
	return false;
}

void TLastChaosApplication::checkRequiredKRF()
{
	stringstream msg;
	
	// KRF 패키지 로드시에 파일이 없다면 거기에 대한 에러를 처리한다. [11/2/2011 rumist]
	if (KRF_FAILURE(m_ReqPackageRes)) {
		msg << "failed to load required KRF package! error code: ";
		msg << m_ReqPackageRes;
//		KalydoGame()->Notice((char*)msg.str().c_str(), 1);
		return;
	}
}

void TLastChaosApplication::requestKRF()
{
	// This function will attempt to download the optional KRF package and load a file from it when it's available.
	// However, the download will happen as the game continues to run, so to keep track of the download, we assign a handler.

	// This handler can store a void pointer to identify the package in all future callbacks.
	int packageId = 1;
	m_PackageHandler->setId((void*)packageId);

	// Please note the name of the package is the name used for the package entry in the KMC, NOT the filename
	// optional 패키지 추가에 대한 부분. [11/2/2011 rumist]
	// 옵션 패키지가 있으면(KRF) 이 함수로 호출 할 수 있다.
	// 대부분은 이렇게 호출하지 않고 내부 코드를 수정한다고 한다.
	requestPackage("optional", m_PackageHandler);

	stringstream msg;
	msg << "Optional package requested";
//	KalydoGame()->Notice((char*)msg.str().c_str(), 1);
}

void TLastChaosApplication::requestItemKRF()
{
	std::string strItemPackageName = "item_";
	TKRFResult result;
	// kcp package.
	result = krfLoadPackage((
	strItemPackageName+"o.krf").c_str());
	if( KRF_FAILURE( result ) )
	{
		m_CustomHandler->setId( (void*)(PACID_ITEM_O) );
		requestPackage((strItemPackageName+"o").c_str(), m_PackageHandler );
	}
	// krf package.
	result = krfLoadPackage( (
	strItemPackageName+"r.krf").c_str() );
	if( KRF_FAILURE( result ) )
	{
		m_CustomHandler->setId( (void*)(PACID_ITEM_R) );
		requestPackage((strItemPackageName+"r").c_str(), m_PackageHandler );
	}
}

void TLastChaosApplication::requestMobKRF()
{
	std::string strMobPackageName = "mob_";

	TKRFResult result;
	// kcp package.
	result = krfLoadPackage((
	strMobPackageName+"o.krf").c_str());
	if( KRF_FAILURE( result ) )
	{
		m_CustomHandler->setId( (void*)(PACID_MOB_O) );
		requestPackage((strMobPackageName+"o").c_str(), m_PackageHandler );
	}
	// krf package.
	result = krfLoadPackage( (
	strMobPackageName+"r.krf").c_str() );
	if( KRF_FAILURE( result ) )
	{
		m_CustomHandler->setId( (void*)(PACID_MOB_R) );
		requestPackage((strMobPackageName+"r").c_str(), m_PackageHandler );
	}
}

void TLastChaosApplication::requestNPCKRF()
{
	std::string strNPCPackageName = "npc_";

	TKRFResult result;
	// kcp package.
	result = krfLoadPackage((
	strNPCPackageName+"o.krf").c_str());
	if( KRF_FAILURE( result ) )
	{
		m_CustomHandler->setId( (void*)(PACID_NPC_O) );
		requestPackage((strNPCPackageName+"o").c_str(), m_PackageHandler );
	}
	// krf package.
	result = krfLoadPackage( (
	strNPCPackageName+"r.krf").c_str() );
	if( KRF_FAILURE( result ) )
	{
		m_CustomHandler->setId( (void*)(PACID_NPC_R) );
		requestPackage((strNPCPackageName+"r").c_str(), m_PackageHandler );
	}
}

void TLastChaosApplication::requestCommonKRF()
{
	std::string strCommonPackageName = "zone_common_";

	TKRFResult result;
	// kcp package.
	result = krfLoadPackage((
	strCommonPackageName+"o.krf").c_str());
	if( KRF_FAILURE( result ) )
	{
		m_CustomHandler->setId( (void*)(PACID_COM_O) );
		requestPackage((strCommonPackageName+"o").c_str(), m_PackageHandler );
	}
	// krf package.
	result = krfLoadPackage( (
	strCommonPackageName+"r.krf").c_str() );
	if( KRF_FAILURE( result ) )
	{
		m_CustomHandler->setId( (void*)(PACID_COM_R) );
		requestPackage((strCommonPackageName+"r").c_str(), m_PackageHandler );
	}
}

void TLastChaosApplication::requestZoneKRF( unsigned int zoneNo )
{
	std::string strNPCPackageName = "zone";
	stringstream strTemp;
	strTemp << "zone";
	strTemp << zoneNo;
	strNPCPackageName = strTemp.str();

	TKRFResult result;
	// krf package.
	result = krfLoadPackage( (
	strNPCPackageName+"_r.krf").c_str() );
	if( KRF_FAILURE( result ) )
	{
		int pacID = (PACID_ZON_R+zoneNo);
		m_CustomHandler->setId( (void*)(PACID_ZON_R+zoneNo) );
		requestPackage((strNPCPackageName+"_r").c_str(), m_CustomHandler );
		m_enLastestID = PACID_ZON_R;
	}

	// kcp package.
 	result = krfLoadPackage((
	strNPCPackageName+"_o.krf").c_str());
	if( KRF_FAILURE( result ) )
	{
		int pacID = (PACID_ZON_O+zoneNo);
		m_CustomHandler->setId( (void*)(PACID_ZON_O+zoneNo) );
		requestPackage((strNPCPackageName+"_o").c_str(), m_CustomHandler );
		m_enLastestID = PACID_ZON_O;
	}
}


void TLastChaosApplication::handlePackages(unsigned int errorType, unsigned int errorCode, void* id, 
					   void* message, unsigned int mLength)
{
	// The message pointer will only contain the result when the package has finished downloading.
	// Don't use this pointer in a progress update.
	TPackageResult& pResult = *(TPackageResult*)message;
	stringstream msg1, msg2;

	switch(errorType)
	{
	case CE_PROGRESS:
	{
		// This "errorType" is used to relay progress updates.
		// The id pointer is used to identify the handler that was passed to the request.
		// The progress is in errorCode and in the 0-100 range.
		int packageId = reinterpret_cast<int>(id);

		msg2 << "Optional KRF package download progress: ";
		msg2 << errorCode;
		msg2 << "%";
		break;
	}
	case CE_NOERROR:
	{
		KASSERT(message == NULL || (sizeof(TPackageResult) == mLength), "Unknown package result!", 
			"Make sure the player has the same protocol version as this application", 
			ET_INPUT, SL_ERROR);

		// The package has been downloaded and checked without problems
		// We can use the packageId to identify which package has been downloaded

		// NOTE: When packages are downloaded, they will NOT have the name they had when they were uploaded!
		//		 Instead they are renamed to the package name that was used to request the package.
		//		 That name is stored in pResult.name

		// Don't forget to load the downloaded package with the KRF lib
		TKRFResult res = krfLoadPackage(pResult.installPath);

		// A simple check of the load result
		msg1 << "package received: " << pResult.name;
		msg1 << ", load check: ";
		if (res == KR_OK)
			msg1 << "PASS";
		else
			msg1 << "FAIL: " << res;

		// A check to see if one of the files in the optional package is present in the KRF virtual file system
// 		msg1 << ", content check: ";
// 		if (kfileExists("assets/chars/bird.blend"))
// 			msg1 << "PASS";
// 		else
// 			msg1 << "FAIL";
		break;
	}
	case CE_HTTP:
	case CE_SERVER:
	case CE_PLAYER:
	default:
		msg1 << "An error occurred in packages: " << errorType << " ";
		
		switch(errorCode)
		{
		case PE_EXTRACT:
			msg1 << "Extracting failed!";
			break;
		case PE_HASH:
			msg1 << "Hashcheck failed!";
			break;
		default:
			msg1 << "Unknown error: " << errorCode;
			break;
		}

		break;
	}
}

//********************************
// kcp file 요청시 콜백 및 기타 처리 루틴의 흐름임.
// 실제로 쓰지 않음. 삭제예정.
void TLastChaosApplication::requestKCP()
{
}

void TLastChaosApplication::KCPCallback(const char* fileName, TKResult result, void* id)
{
}
//*********************************************************//

void TLastChaosApplication::testDataStore()
{
	// This function will store a piece of data in the online data store provided by KMC.
	// After storage, it will retrieve the same information again to check if the value was stored correctly.

	// Because we are sending random values, we need to cancel any tests if another one is still in progress.
	if (m_DataStoreTest > -1) {
//		KalydoGame()->Notice("data store test still in progress, please wait...", 3);
		return;
	}

	// Generate the value and put it in a xml format.
	m_DataStoreTest = rand() % 1000;
	stringstream msg;
	msg << "<testdata>" << m_DataStoreTest << "</testdata>";

	// Again, we are using a THandler for the callback so we can provide a void pointer for identification.
	int msgType = 1;
	m_SendDataHandler->setId((void*)msgType);

	// The sendXmlData function REPLACES everything in the datastore, so make sure the xmlData argument contains ALL the data for the current user
	sendXmlData(msg.str().c_str(), m_SendDataHandler);
}

void TLastChaosApplication::handleSendData(unsigned int errorType, unsigned int errorCode, void* id, void* message, unsigned int mLength)
{
	// This is the callback that is registered to the m_SendDataHandler.
	// It is used for both the datastore test as well as the statistics test.
	// We can use the void pointer identification to know which test was called.
	int msgType = reinterpret_cast<int>(id);

	stringstream msg;

	switch(errorType)
	{
	case CE_NOERROR:
	{
		switch (msgType)
		{
		// msgType 1 means we are performing the data store test, so request the data again so we can verify it.
		// Currently the only relevant argument in the requestXmlData function is the THandler pointer.
		// The rest is reserved for future use.
		case 1:
			msg << "Data send to datastore";
			requestXmlData(RD_USERNAME, NULL, 0, m_RequestDataHandler);
			break;

		// msgType 2 means we posted statistics
		case 2:
			msg << "Statistic posted";
		}
		break;
	}
	case CE_HTTP:
	case CE_SERVER:
	case CE_PLAYER:
	default:
		msg << "An error occurred in data storing! ";
		msg << "Error type: " << errorType << ", error code: " << errorCode;

		// NOTE: errorCode 500 means an internal server error. Check the KMC for a more detailed error description.
		break;
	}

//	KalydoGame()->Notice((char*)msg.str().c_str(), 1);
}

void TLastChaosApplication::handleRequestData(unsigned int errorType, unsigned int errorCode, void* id, void* message, unsigned int mLength)
{
	// This is the callback that is registered to the m_RequestDataHandler.
	// It is used for the datastore check to varify the received data.

	// The message pointer contains the entire datastore XML
	string xmlData;
	if (message)
		xmlData = (char*)message;
	stringstream msg;

	switch(errorType)
	{
	case CE_NOERROR:
	{
		// To extract the value we need from the XML, we search for the desired tag.
		int start = xmlData.find("<testdata>");
		start = xmlData.find(">", start) + 1;
		int end = xmlData.find("</testdata>");
		int data;
		istringstream (xmlData.substr(start, end)) >> data;

		if (m_DataStoreTest == data)
			msg << "Value in datastore verified!";
		else
			msg << "Value in datastore incorrect! original: " << m_DataStoreTest << ", received: " << data;

		break;
	}
	case CE_HTTP:
	case CE_SERVER:
	case CE_PLAYER:
	default:
		msg << "An error occurred in data retrieving! ";
		msg << "Error type: " << errorType << ", error code: " << errorCode;

		break;
	}

	// The test has finished, allow new tests
	m_DataStoreTest = -1;
}

void TLastChaosApplication::handleZonePack(unsigned int errorType, unsigned int errorCode, void* id, 
					   void* message, unsigned int mLength)
{
	// The message pointer will only contain the result when the package has finished downloading.
	// Don't use this pointer in a progress update.
	TPackageResult& pResult = *(TPackageResult*)message;
	stringstream msg1, msg2;

	switch(errorType)
	{
	case CE_PROGRESS:
	{
		// This "errorType" is used to relay progress updates.
		// The id pointer is used to identify the handler that was passed to the request.
		// The progress is in errorCode and in the 0-100 range.
		int packageId = reinterpret_cast<int>(id);

		msg2 << "Optional KRF package download progress: ";
		msg2 << errorCode;
		msg2 << "%";
		KPackManager::GetInstance()->SetProgressPercent( errorCode );
		break;
	}
	case CE_NOERROR:
	{
		KASSERT(message == NULL || (sizeof(TPackageResult) == mLength), "Unknown package result!", 
			"Make sure the player has the same protocol version as this application", 
			ET_INPUT, SL_ERROR);

		// The package has been downloaded and checked without problems
		// We can use the packageId to identify which package has been downloaded

		// NOTE: When packages are downloaded, they will NOT have the name they had when they were uploaded!
		//		 Instead they are renamed to the package name that was used to request the package.
		//		 That name is stored in pResult.name

		// Don't forget to load the downloaded package with the KRF lib
		TKRFResult res = krfLoadPackage(pResult.installPath);
		int packageId = reinterpret_cast<int>(id);
		// A simple check of the load result
// 		msg1 << "package received: " << pResult.name;
// 		msg1 << ", load check: ";
		std::string strName = pResult.name;
		if (res == KR_OK)
		{
			msg1 << "PASS";
			if( packageId >= PACID_ZON_O )
			{
				if( strName.rfind( "_o" ) != std::string::npos )
				{
					KPackManager::GetInstance()->SetProgressPercent( 100 );
					KPackManager::GetInstance()->SetKPMStatus( KPackManager::KPMS_COMPLETE );
				}
			}
			else
			{
				KPackManager::GetInstance()->SetProgressPercent( 100 );	
				KPackManager::GetInstance()->SetKPMStatus( KPackManager::KPMS_COMPLETE );
			}
		}
		else
			msg1 << "FAIL: " << res;

		// A check to see if one of the files in the optional package is present in the KRF virtual file system
// 		msg1 << ", content check: ";
// 		if (kfileExists("assets/chars/bird.blend"))
// 			msg1 << "PASS";
// 		else
// 			msg1 << "FAIL";
		break;
	}
	case CE_HTTP:
	case CE_SERVER:
	case CE_PLAYER:
	default:
		msg1 << "An error occurred in packages: " << errorType << " ";
		
		switch(errorCode)
		{
		case PE_EXTRACT:
			msg1 << "Extracting failed!";
			break;
		case PE_HASH:
			msg1 << "Hashcheck failed!";
			break;
		default:
			msg1 << "Unknown error: " << errorCode;
			break;
		}

		break;
	}
}


//*****************************************
// single sign on 관련 부분.
// 계정 및 pwd를 얻어올 수 있다.
// 아래는 샘플코드로서 실제로는 다른 방식으로 호출할 예정.
void TLastChaosApplication::testCredentials()
{
	// This function will approach the login server of the client that has a kalgus script running.
	// The url for this server can be set in the application configuration in the KMC.
	// If the url has not been set in the KMC, the application will crash on this request.
	// Or you can use requestCredentialsFrom and provide the url in the code.
	requestCredentials(m_CredentialsHandler);
}

void TLastChaosApplication::handleCredentials(unsigned int errorType, unsigned int errorCode, void* id, 
											void* message, unsigned int mLength)
{
#ifdef USE_KALYDO_AUTO_LOGIN
	// This is the callback that is registered to the m_CredentialsHandler.
	// It is used for retrieving the results of the credentials request.
	if (message == NULL)
	{
		m_bSSOEnabled = false;
		return;
	}

	KASSERT((sizeof(TCredentials) == mLength), "Unknown credentials result!", 
		"Make sure the player has the same protocol version as this application", 
		ET_INPUT, SL_ERROR);

	TCredentials& pResult = *(TCredentials*)message;
	
	if (NULL != pResult.name)
		m_szID = pResult.name;

	if (NULL != pResult.password)
		m_szPWD = pResult.password;

	KalydoSetUserAccount( m_szID, m_szPWD );
	if (m_szID == "" && m_szPWD == "")
		m_bSSOEnabled = false;
#endif
}
//*****************************************//

void TLastChaosApplication::sendStats()
{
	// This function will post a statistic to the KMC.
	// Application statistics are send in string pairs.
	// Usually the first string is used to identify the application, while the second string describes the event.
	// NOTE: the second string MUST contain an integer, otherwise the handler will return with errorCode 500.
	// The backend system can perform various calculations on the received events and display the results in multiple ways.
	int EVENT_TEST = 1337;

	// Using the same handler as the data storage function.
	// We can use the Id to identify which data was send to the server.
	int msgType = 2;
	m_SendDataHandler->setId((void*)msgType);
	sendAppStatistics("KalydoTestApp", EVENT_TEST, m_SendDataHandler);
}

void TLastChaosApplication::postNotification()
{
	// This function will send a notification to the website.
	// In this example we are sending information to make a wall post on Facebook.
	string title("Test successful!");
	string desc("This user has succesfully used the Kalydo Test Application.");
	string fun("Kalydo - Browser Game Platform");
	string icon("testSuccessful");
	
	TBrowserData* data = new TBrowserData[4];

	// These notifications can contain any amount of any type of data,
	// so we need to define what we are going to send.
	data[0].type = BDT_STRING;
	data[1].type = BDT_STRING;
	data[2].type = BDT_STRING;
	data[3].type = BDT_STRING;

	kStrCpy(data[0].stringValue, title.length()+1, title.c_str());
	kStrCpy(data[1].stringValue, desc.length()+1, desc.c_str());
	kStrCpy(data[2].stringValue, fun.length()+1, fun.c_str());
	kStrCpy(data[3].stringValue, icon.length()+1, icon.c_str());

	bool res = callBrowser("site.doWallPost", data, 4);

	// The player cannot check if the website takes any action based on the notification.
	// Therefor this result is only a check on the input.
	if (res)
//		KalydoGame()->Notice("wall notification posted", 1);
		;	
	else
//		KalydoGame()->Notice("unable to send notification, invalid input", 1);
		;

	delete [] data;
}

bool TLastChaosApplication::callFromBrowser(const char* method, const TBrowserData* args, unsigned int noArgs)
{
	// This function is called by the Kalydo Player when the website sends a notification to the player.
	// A notification must be registered before this function is called.
	// In this application this registration is done in the start() function.
	// NOTE: This function is called in the main thread of the game, between two update() calls.
	stringstream message;

	message << "The site called method \"";
	message << method;
	message << "\" with arguments: ";

	for (unsigned int i = 0; i < noArgs; ++i)
	{
		const TBrowserData& arg = args[i];

		switch(arg.type)
		{
		case BDT_STRING:
			message << args[i].stringValue;
			break;
		case BDT_BOOL:
			message << args[i].boolValue;
			break;
		case BDT_INT:
			message << args[i].intValue;
			break;
		case BDT_DOUBLE:
			message << args[i].doubleValue;
			break;
		default:
			message << "NULL";
			break;
		}
		if (i < noArgs)
			message << ", ";
	}

	// We registered methods for the fullscreen and mute buttons on the website,
	// so we need to handle those notifications when we get them.
	if (!strcmp(method, "toggleFullscreen"))
		toggleFullscreen();
#ifdef	KALYDO_SEQ_FILE
	else if (!strcmp(method, "toggleMute"))
		krfWriteSequenceFile("sequencefile.xml");
#endif	// KALYDO_SEQ_FILE
		
#ifdef USE_KALYDO_AUTO_LOGIN
	else if (!strcmp(method, "setSSOURL"))
		setSSOURL(args, noArgs);
#endif
	else
		showMessage(message.str().c_str());

	return true;
}

void TLastChaosApplication::toggleFullscreen()
{
	// This function contains example code on how to switch between windowed mode and fullscreen.
	// Switching will produce a WM_SIZE message so the game will be notified of the change through there.

	if (!m_Fullscreen)
	{
		// Find out the name of the device this window is on (this is for multi-monitor setups)
		HMONITOR hMonitor = MonitorFromWindow(m_MyWindowHandle, MONITOR_DEFAULTTONEAREST);
		MONITORINFOEXW monInfo;
		memset(&monInfo, 0, sizeof(MONITORINFOEXW));
		monInfo.cbSize = sizeof(MONITORINFOEXW);
		GetMonitorInfoW(hMonitor, &monInfo);

		// Find the requested device mode
		DEVMODEW dmode;
		bool foundMode = false;
		memset(&dmode, 0, sizeof(DEVMODEW));
		dmode.dmSize = sizeof(DEVMODEW);
		EnumDisplaySettingsW(monInfo.szDevice, ENUM_CURRENT_SETTINGS, &dmode);

		// store the old settings
		RECT windowRect;
		GetWindowRect(m_MyWindowHandle, &windowRect);
		m_OldWidth  = windowRect.right - windowRect.left;
		m_OldHeight = windowRect.bottom - windowRect.top;
		m_OldPosX = windowRect.left;
		m_OldPosY = windowRect.top;
		m_DWStyle = GetWindowLongW(m_MyWindowHandle, GWL_STYLE);
		m_EXDWStyle = GetWindowLongW(m_MyWindowHandle, GWL_EXSTYLE);

		// check and set parent window
		m_MyParentHandle = GetParent(m_MyWindowHandle);
		if( m_MyParentHandle != NULL)
			SetParent(m_MyWindowHandle, NULL);

		//SetWindowPos(m_MyWindowHandle, NULL, dmode.dmPosition.x, dmode.dmPosition.y, sam_iScreenSizeI, sam_iScreenSizeJ, SWP_NOZORDER);
		MoveWindow(m_MyWindowHandle, dmode.dmPosition.x, dmode.dmPosition.y, sam_iScreenSizeI, sam_iScreenSizeJ, TRUE);		
				
		m_Fullscreen = true;
	}
	else {
		// return default size.
		ChangeDisplaySettings( NULL, 0 );
		//reset window style and reset size and position
		ShowWindow(m_MyWindowHandle, SW_HIDE);
		SetWindowLongW(m_MyWindowHandle, GWL_STYLE, m_DWStyle);
		SetWindowLongW(m_MyWindowHandle, GWL_EXSTYLE, m_EXDWStyle);
		ShowWindow(m_MyWindowHandle, SW_SHOW);

		//set back parent like before full screen
		if(m_MyParentHandle != NULL)
		{
			SetParent(m_MyWindowHandle, m_MyParentHandle);
			if (m_Focus) SetFocus(m_MyParentHandle);
		}

		//reset window and calculate new dimensions
		SetWindowPos(m_MyWindowHandle, HWND_TOP, 0, 0, m_OldWidth, m_OldHeight, SWP_FRAMECHANGED);
		if (m_Focus) SetFocus(m_MyWindowHandle);

		m_Fullscreen = false;
	}

	if( CUIManager::isCreated() == true )
	{		
		CUIManager::getSingleton()->GetOption()->ResetDisplay();		
	}
}

#ifdef USE_KALYDO_AUTO_LOGIN
void TLastChaosApplication::setSSOURL(const TBrowserData* args, unsigned int noArgs)
{
	if (noArgs > 0)
	{
		const TBrowserData& arg = args[0];
		if (arg.type == BDT_STRING)
		{
			requestCredentialsFrom(args[0].stringValue, m_CredentialsHandler);
			return;
		}
	}
	// show normal login screen
	m_bSSOEnabled = false;
}
#endif

void TLastChaosApplication::showMessage(const char* message)
{
	// This function will generate a simple message box and can be used for debug purposes.
	// It is not used in the current version of this application example.

#ifdef WIN32
	// we need to convert our UTF8 strings to wide chars to 
	// show them properly in windows.
	StackMultiByteToWideChar(CP_UTF8, message, -1, wideMessage);
	MessageBoxW(NULL, wideMessage, L"Your Kalydo Application", MB_OK);
#else
	// TODO: other OSes
	std::cout << message << std::endl;
#endif
}

unsigned int TLastChaosApplication::getCrashInfo(char* name, unsigned int size)
{
	// This function will be called by the Kalydo Player when an error report is generated.
	// If there is any additional information you would like to include in the report, like a script file/line.

	// An example crash info message; a fictive script with a line number.
	const char* message = "example.lua, line 1337";

	unsigned int messageLen = (unsigned int)(strlen(message) + 1);
	// check size
	if (size < messageLen)
		// return correct size
		return messageLen;

	// buffer is large enough, copy my crash message into the buffer.
	kStrCpy(name, messageLen, message);

	return messageLen;
}

void TLastChaosApplication::writeLogEntry(char* msg)
{
	if (!m_LogFile) return;

	time_t rawtime;
	//struct tm* timeinfo = new tm;
	struct tm* timeinfo;

	time(&rawtime);
	//localtime_s(timeinfo, &rawtime);
	timeinfo = localtime( &rawtime );


	char timeEntry[32];
	//asctime_s(timeEntry, 32, timeinfo);
	strcpy( timeEntry, asctime(  timeinfo ) );;

	fwrite(timeEntry, 1, strlen(timeEntry) - 1, m_LogFile);
	fwrite(" - ", 1, 3, m_LogFile);
	fwrite(msg, 1, strlen(msg), m_LogFile);
	fwrite("\n", 1, 1, m_LogFile);
	delete timeinfo;
}
#endif // KALYDO