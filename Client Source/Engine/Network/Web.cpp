//안태훈 수정 시작	//(5th Closed beta)(0.2)
#include "StdH.h"
#include "Web.h"
#include <WinInet.h>
#include <assert.h>
#include "ThreadWrapper.h"
#include <Engine/Interface/UIManager.h>
#include <Engine/Interface/UIMouseCursor.h>

extern HWND	_hDlgWeb;


/**
리턴값
0 : 이상 없음
1 : 자체 복구가 가능한 수준의 오류
2 : 유저에게 알려야할 오류
3 : 개발자 수준의 오류
**/
UINT WINAPI WebThreadFunction(void *parameter)
{
	ASSERT(parameter != NULL);
	if(parameter == NULL) return 3;
	CSharedWebData &shared = *(CSharedWebData*)parameter;

	///internet 연결을 초기화한다.
	HINTERNET hInternet = InternetOpen( "Web Board Browser", 
										INTERNET_OPEN_TYPE_PRECONFIG, // Use registry settings. 
										NULL, // Proxy name. NULL indicates use default.
										NULL, // List of local servers. NULL indicates default. 
										0 );
	if(hInternet == NULL)	//internet 연결을 여는데 실패했다. IE가 없나?
	{
		return 3;
	}
	DWORD exitCode = 0;

	for(;;)
	{
		///입력이 들어오기를 무한정 기다림.
		if(WAIT_OBJECT_0 != WaitForSingleObject(shared.GetSendReadEventHandle(), INFINITE)) {exitCode = 3; break;}
		if(shared.GetExit()) break;

		///URL을 저장하고 Send Buffer를 지우고 Send Write Event를 Set한다.
		if(shared.GetSendMsgBuffer() == NULL) continue;
		std::string strURL = shared.GetSendMsgBuffer();
		shared.FreeSendMsgBuffer();
		SetEvent(shared.GetSendWriteEventHandle());

		///http연결을 만든다.
		HINTERNET hHttpFile = InternetOpenUrl(	hInternet
												, strURL.c_str()
												, NULL
												, 0
												, 0
												, 0 );
		if(hHttpFile == NULL) //url을 찾을 수 없습니다.
		{
			DWORD dwError = GetLastError();
			//exitCode = 2;
			//break;
			continue;
		}


		///http file의 사이즈를 얻어온다.
		char szSizeBuffer[128] = {0};
		DWORD dwLengthSizeBuffer = sizeof(szSizeBuffer);
		BOOL bQuery = HttpQueryInfo( hHttpFile
									, HTTP_QUERY_CONTENT_LENGTH
									, szSizeBuffer
									, &dwLengthSizeBuffer
									, NULL );
		DWORD dwFileSize = 0;
		if(bQuery)
		{
			//http file을 저장할 적당한 크기의 메모리를 할당한다.
			dwFileSize = atol(szSizeBuffer);
		}
		else
		{
			//적당한 크기를 잡는다.
			dwFileSize = 1024*1024; //1 mega bytes
		}
		ResetEvent(shared.GetReceiveWriteEventHandle());
		ResetEvent(shared.GetReceiveReadEventHandle());
		shared.AllocReceiveMsgBuffer(dwFileSize+1);
		char *szReceiveBuffer = shared.GetReceiveMsgBuffer();

		///http file을 web에서 읽어온다.
		DWORD dwBytesRead = 0;	//읽힌 파일의 크기
		BOOL bRead = InternetReadFile( hHttpFile
									, szReceiveBuffer
									, dwFileSize
									, &dwBytesRead );
    
		if(bRead)
		{
			szReceiveBuffer[dwBytesRead] = '\0';
			SetEvent(shared.GetReceiveReadEventHandle());
		}
		else	//web page를 읽는데 실패했다.
		{
			shared.FreeReceiveMsgBuffer();
			//exitCode = 3;
			//break;
			InternetCloseHandle(hHttpFile);
			continue;
		}

		///http 연결을 닫는다.
		InternetCloseHandle(hHttpFile);
	}

	///internet 연결을 종료한다.
	InternetCloseHandle(hInternet);

	return exitCode;
}

cWeb::cWeb()
: m_eStatus( WS_PREBEGIN )
{
	m_sharedData.ResetAll();
	m_pThread = new cThreadWrapper(WebThreadFunction);
	m_hWebPage = ::LoadLibrary("cwebpage.dll");

	m_hWebWnd = NULL;

	// It's linking web embed functions
	EmbedBrowserObject = (long (WINAPI *)(HWND hwnd))GetProcAddress(m_hWebPage, "EmbedBrowserObject");
	UnEmbedBrowserObject = (void (WINAPI *)(HWND hwnd))GetProcAddress(m_hWebPage, "UnEmbedBrowserObject");
	DisplayHTMLPage = (long (WINAPI *)(HWND hwnd, const char *webPageName))GetProcAddress(m_hWebPage, "DisplayHTMLPage");
	WebDialogProc = NULL;
	m_fnWebCallBack = NULL;
}

cWeb::~cWeb()
{
	if(m_pThread) delete m_pThread;

	if (m_hWebPage)
	{
		::FreeLibrary(m_hWebPage);
	}
}

BOOL cWeb::Begin()
{
	if(!IsPreBegin()) return FALSE;
	if(m_pThread->Start(&m_sharedData))
	{
		m_eStatus = WS_READYREQUEST;
		return TRUE;
	}
	return FALSE;
}

BOOL cWeb::End()
{
	if(!IsBegin()) return TRUE;
	m_sharedData.SetExit(true);
	SetEvent(m_sharedData.GetSendReadEventHandle());
	Sleep(0);
	m_sharedData.ResetAll();
	m_eStatus = WS_PREBEGIN;
	return TRUE;
}

void cWeb::Request(const char *szURL)	//web page 요청
{
	ResetEvent(m_sharedData.GetSendWriteEventHandle());
	int lenURL = strlen(szURL);
	m_sharedData.AllocSendMsgBuffer(lenURL+1);
	char *sendBuf = m_sharedData.GetSendMsgBuffer();
	memcpy(sendBuf, szURL, lenURL+1);
	sendBuf[lenURL] = '\0';
	SetEvent(m_sharedData.GetSendReadEventHandle());
}

BOOL cWeb::Read(std::string &strContent, std::string &strError)		//요청했던 web page 읽기
{
	DWORD waitRet = WaitForSingleObject(m_sharedData.GetReceiveReadEventHandle(), 1000/*INFINITE*/);
	if(WAIT_OBJECT_0 == waitRet)
	{
		if(m_sharedData.GetReceiveMsgBuffer() != NULL)
		{
			strContent = m_sharedData.GetReceiveMsgBuffer();
			m_sharedData.FreeReceiveMsgBuffer();
		}
		if(m_sharedData.GetErrorMsgBuffer() != NULL)
		{
			strError = m_sharedData.GetErrorMsgBuffer();
			m_sharedData.FreeErrorMsgBuffer();
		}
		SetEvent(m_sharedData.GetReceiveWriteEventHandle());
		return TRUE;
	}
	return FALSE;
}

BOOL cWeb::OpenWebPage(HWND hDlg)
{
	m_hWebWnd = hDlg;
	EmbedBrowserObject(hDlg);
	CUIManager::getSingleton()->GetMouseCursor()->SetCursorNULL();
	
	return TRUE;
}

void cWeb::SetWebMoveWindow(void)
{
	if (_hDlgWeb != NULL)
	{
		SetWebPosition(500, 400);
		BOOL bSuccess = MoveWindow(_hDlgWeb, m_nPosX, m_nPosY, m_nWidth, m_nHeight, FALSE);
	}
}

BOOL cWeb::SendWebPageOpenMsg(BOOL bShow)
{
	if (_hDlgWeb!=NULL)
	{
		if (bShow)	{
			ShowWindow(_hDlgWeb, SW_SHOWNORMAL);
		} else {
			ShowWindow(_hDlgWeb, SW_HIDE);
		}

		UpdateWindow(_hDlgWeb);
		return TRUE;
	}

	return FALSE;
}

BOOL cWeb::CloseWebPage(HWND hDlg)
{
	if (m_hWebWnd)
	{
		UnEmbedBrowserObject(m_hWebWnd);
		m_hWebWnd = NULL;
		
		CUIManager::getSingleton()->GetMouseCursor()->SetCursorType();

		return TRUE;
	}
	return FALSE;
}

void cWeb::SetWebPosition(INDEX nWidth, INDEX nHeight)
{
	extern ENGINE_API HWND	_hwndMain;
	extern INDEX	sam_bFullScreenActive;
	RECT	rtMain;
	GetWindowRect(_hwndMain, &rtMain);

	if (IsFullScreen(sam_bFullScreenActive))
	{
		m_nPosX = (m_pixMaxI+m_pixMinI - nWidth) / 2; m_nPosY = ( m_pixMaxJ + m_pixMinJ - nHeight ) / 2;
	}
	else
	{
		m_nPosX = (rtMain.right+rtMain.left - nWidth) / 2; m_nPosY = ( rtMain.bottom+rtMain.top - nHeight ) / 2;
	}
	//m_nPosX = (m_pixMaxI+m_pixMinI - nWidth) / 2; m_nPosY = ( m_pixMaxJ + m_pixMinJ - nHeight ) / 2;

	m_nWidth = nWidth; m_nHeight = nHeight;
}

void cWeb::SetPos( int x, int y )
{
	m_nPosX = x;
	m_nPosY = y;

	UpdatePos();
}

void cWeb::SetSize( int width, int height )
{
	m_nWidth = width;
	m_nHeight = height;
}

void cWeb::UpdatePos()
{
	if (_hDlgWeb == NULL)
		return;

	extern ENGINE_API HWND	_hwndMain;
	RECT	rcMain;
	int nOffY = GetSystemMetrics(SM_CYCAPTION);
	nOffY += GetSystemMetrics(SM_CYDLGFRAME);
	int nOffX = GetSystemMetrics(SM_CXDLGFRAME);

	GetWindowRect(_hwndMain, &rcMain);

	MoveWindow(_hDlgWeb, rcMain.left + nOffX + m_nPosX, rcMain.top + nOffY + m_nPosY, m_nWidth, m_nHeight, FALSE);
	UpdateWindow(_hDlgWeb);
}

void cWeb::SetWebUrl( std::string& url )
{
	if (m_hWebWnd == NULL)
		return;

	DisplayHTMLPage(m_hWebWnd, CTString(url.c_str()));
}
