
// UITool.cpp : 응용 프로그램에 대한 클래스 동작을 정의합니다.
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "UITool.h"
#include "MainFrm.h"

#include "ChildFrm.h"
#include "UIToolDoc.h"
#include "UIToolView.h"

#include <Engine/Templates/Stock_CFontData.h>
#include <Engine/Interface/UIManager.h>
#include <Engine/GameState.h>

#include "CmdUITool.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define REG_LOAD_INDEX(s,d) (AfxGetApp()->GetProfileInt(_T("Settings"),_T(s),d));

// CUIToolApp

BEGIN_MESSAGE_MAP(CUIToolApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CUIToolApp::OnAppAbout)
	// 표준 파일을 기초로 하는 문서 명령입니다.
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	//ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	ON_COMMAND(ID_FILE_OPEN, &CUIToolApp::OnFileOpen)
	ON_COMMAND(ID_REFRESH, &CUIToolApp::OnRefresh)
	ON_COMMAND(ID_FILE_SAVE, &CUIToolApp::OnFileSave)
	ON_COMMAND(ID_FILE_SAVE_AS, &CUIToolApp::OnFileSave)
END_MESSAGE_MAP()

// CUIToolApp 생성

CUIToolApp::CUIToolApp()
	: m_pDrawPort(NULL)
	, m_pViewPort(NULL)
	, m_pUIMgr(NULL)
	, m_oldTime(0.f)
{
	m_bHiColorIcons = TRUE;

	// 다시 시작 관리자 지원
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// 응용 프로그램을 공용 언어 런타임 지원을 사용하여 빌드한 경우(/clr):
	//     1) 이 추가 설정은 다시 시작 관리자 지원이 제대로 작동하는 데 필요합니다.
	//     2) 프로젝트에서 빌드하려면 System.Windows.Forms에 대한 참조를 추가해야 합니다.
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: 아래 응용 프로그램 ID 문자열을 고유 ID 문자열로 바꾸십시오(권장).
	// 문자열에 대한 서식: CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("UITool.AppID.NoVersion"));

	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.

	m_csLock.cs_iIndex = -1;
}

// 유일한 CUIToolApp 개체입니다.

CUIToolApp theApp;


// CUIToolApp 초기화

BOOL CUIToolApp::InitInstance()
{
	BOOL bResult;
	CTSTREAM_BEGIN {
		bResult = SubInitInstance();
	} CTSTREAM_END;

	return bResult;
}

int CUIToolApp::Run()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	MSG		msg;

	while(TRUE)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
		{
			if (GetMessage(&msg, NULL, 0, 0) == FALSE)
			{
				ExitInstance();
				return msg.wParam;
			}			

			if (PreTranslateMessage(&msg) == FALSE)
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			CTSTREAM_BEGIN {
				Update();
				Draw();
			} CTSTREAM_END;
		}
	}

	return CWinAppEx::Run();
}

BOOL CUIToolApp::SubInitInstance()
{
	// InitInstance 내용 이동


	// 응용 프로그램 매니페스트가 ComCtl32.dll 버전 6 이상을 사용하여 비주얼 스타일을
	// 사용하도록 지정하는 경우, Windows XP 상에서 반드시 InitCommonControlsEx()가 필요합니다. 
	// InitCommonControlsEx()를 사용하지 않으면 창을 만들 수 없습니다.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 응용 프로그램에서 사용할 모든 공용 컨트롤 클래스를 포함하도록
	// 이 항목을 설정하십시오.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();


	EnableTaskbarInteraction(FALSE);

	// RichEdit 컨트롤을 사용하려면  AfxInitRichEdit2()가 있어야 합니다.	
	// AfxInitRichEdit2();

	// 표준 초기화
	// 이들 기능을 사용하지 않고 최종 실행 파일의 크기를 줄이려면
	// 아래에서 필요 없는 특정 초기화
	// 루틴을 제거해야 합니다.
	// 해당 설정이 저장된 레지스트리 키를 변경하십시오.
	// TODO: 이 문자열을 회사 또는 조직의 이름과 같은
	// 적절한 내용으로 수정해야 합니다.
	SetRegistryKey(_T("로컬 응용 프로그램 마법사에서 생성된 응용 프로그램"));
	LoadStdProfileSettings(4);  // MRU를 포함하여 표준 INI 파일 옵션을 로드합니다.


	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// 응용 프로그램의 문서 템플릿을 등록합니다. 문서 템플릿은
	//  문서, 프레임 창 및 뷰 사이의 연결 역할을 합니다.
	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(IDR_UIToolTYPE,
		RUNTIME_CLASS(CUIToolDoc),
		RUNTIME_CLASS(CChildFrame), // 사용자 지정 MDI 자식 프레임입니다.
		RUNTIME_CLASS(CUIToolView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	// 주 MDI 프레임 창을 만듭니다.
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;
	// 접미사가 있을 경우에만 DragAcceptFiles를 호출합니다.
	//  MDI 응용 프로그램에서는 m_pMainWnd를 설정한 후 바로 이러한 호출이 발생해야 합니다.

	// Engine 초기화
	{
		// initialize entire engine
		SE_InitEngine("UITool");
		SE_LoadDefaultFonts();
		// ska studio must use smc files instead of bmc files
		_pShell->SetINDEX("ska_bReplaceSmcWithBmc", FALSE);

		try {
			m_pfdDefaultFont = _pFontStock->Obtain_t( CTFILENAME( "Fonts\\Console1.fnt"));
			m_pfdDefaultFont->fd_pixCharSpacing = 0;
			m_pfdDefaultFont->fd_pixLineSpacing = 0;
		} catch (char *strErr) {
			WarningMessage("Error loading font: %s.", strErr);
			delete m_pfdDefaultFont;
			m_pfdDefaultFont = _pfdConsoleFont;
		}

		// set main window for engine
		SE_UpdateWindowHandle( m_pMainWnd->m_hWnd);

		GfxAPIType eNewAPI = (GfxAPIType)REG_LOAD_INDEX(_T("GAPI"),GAT_D3D);
		// if invalid api
		if(eNewAPI!=GAT_D3D && eNewAPI!=GAT_OGL) {
			// d3d is default
			eNewAPI = GAT_D3D;
		}

		_pGfx->ResetDisplayMode(eNewAPI);

		CUIManager* pUIMgr = CUIManager::getSingleton();

		if( pUIMgr )
		{
			pUIMgr->InitUIString();
		}

		CUITooltipMgr::getSingleton()->initialize();
	}

	// 표준 셸 명령, DDE, 파일 열기에 대한 명령줄을 구문 분석합니다.
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);



	// 명령줄에 지정된 명령을 디스패치합니다.
	// 응용 프로그램이 /RegServer, /Register, /Unregserver 또는 /Unregister로 시작된 경우 FALSE를 반환합니다.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	// 주 창이 초기화되었으므로 이를 표시하고 업데이트합니다.
	pMainFrame->ShowWindow(m_nCmdShow);
	//pMainFrame->UpdateWindow();

	return TRUE;
}

int CUIToolApp::ExitInstance()
{
	//TODO: 추가한 추가 리소스를 처리합니다.
	_pGfx->DestroyWindowCanvas( m_pViewPort);
	m_pViewPort = NULL;

	if (m_pfdDefaultFont != NULL)
	{
		m_pfdDefaultFont->Clear();
		FONT_STOCK_RELEASE(m_pfdDefaultFont);
	}

	SE_EndEngine();
	CTStream::ClearStreamHandling();

	return CWinAppEx::ExitInstance();
}

void CUIToolApp::initDraw(HWND hWnd)
{
	_pGfx->CreateWindowCanvas(hWnd, &m_pViewPort, &m_pDrawPort);

	if( m_pDrawPort )
		m_pDrawPort->SetFont(m_pfdDefaultFont);

	if( m_pUIMgr == NULL )
	{
		m_pUIMgr = CUIManager::getSingleton();
		//m_pUIMgr->SetDrawPort( m_pDrawPort );
		if (m_pUIMgr)
		{
			SE_UpdateWindowHandle( hWnd );

			RECT rect;		
			::GetClientRect(hWnd, &rect);

			m_pUIMgr->SetDrawPort(m_pDrawPort);

			m_pUIMgr->InitPos(0, 0, rect.right - rect.left, rect.bottom - rect.top);
		}
	}
}

// CUIToolApp 메시지 처리기


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// 대화 상자를 실행하기 위한 응용 프로그램 명령입니다.
void CUIToolApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CUIToolApp 사용자 지정 로드/저장 메서드

void CUIToolApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
	bNameValid = strName.LoadString(IDS_EXPLORER);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EXPLORER);
}

void CUIToolApp::LoadCustomState()
{
}

void CUIToolApp::SaveCustomState()
{
}

// CUIToolApp 메시지 처리기

void CUIToolApp::OnFileOpen()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CFileDialog dlgFile(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL, 0);

	CString strFilter;
	strFilter += "UI Files(*.xml)";
	strFilter += (TCHAR)'\0';   // next string please
	strFilter += "*.xml";
	strFilter += (TCHAR)'\0';   // last string

	dlgFile.m_ofn.lpstrTitle = "UI 파일 열기";
	dlgFile.m_ofn.lpstrFilter = strFilter;
	//dlgFile.m_ofn.lpstrInitialDir = "";

	if( dlgFile.DoModal() == IDOK )
	{
		 CString strPath = dlgFile.GetPathName();
		 m_strLoadFileName = dlgFile.GetFileTitle();	// 파일 이름 저장.
		 
		 if( CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd() )
			 pMain->SetPath(strPath.GetString());

		 CmdOpenFile* pCmd = new CmdOpenFile;
		 pCmd->setData((LPSTR)(LPCSTR)strPath);
		 addQueue(pCmd);
	}	
}


void CUIToolApp::OnRefresh()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if( CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd() )
	{
		pMain->Refresh();

		if( CChildFrame *pChild = (CChildFrame *)pMain->GetActiveFrame() )
		{
			if( CUIToolView* pView = (CUIToolView*)pChild->GetActiveView() )
			{
				pView->Invalidate();
				pView->UpdateWindow();
			}
		}
	}
}


void CUIToolApp::OnFileSave()
{
}

void CUIToolApp::Draw()
{
	if( m_pDrawPort )
	{
		m_pDrawPort->SetAsCurrent();

		m_pDrawPort->Fill( 0x7F7F7FFF | CT_OPAQUE);
		m_pDrawPort->FillZBuffer(ZBUF_BACK);


		m_pDrawPort->SetOrtho();
		shaBlendFunc(GFX_SRC_ALPHA, GFX_INV_SRC_ALPHA);

		if( m_pUIMgr )
			m_pUIMgr->Render( m_pDrawPort );

		if( m_pViewPort != NULL )
			m_pViewPort->SwapBuffers();
	}
}

void CUIToolApp::Update()
{
	// Update
	if (_pTimer != NULL)
	{
		ULONG	cur = (ULONG)_pTimer->GetHighPrecisionTimer().GetMilliseconds();
		float	delta = (float)(cur - m_oldTime) * 0.001f;

		if (m_pUIMgr != NULL)
			m_pUIMgr->Update(delta, cur);

		m_oldTime = cur;
	}

	{
		CTSingleLock sl(&m_csLock, TRUE);

		while (m_queue.size())
		{
			Command* pCmd = m_queue.front();
			if (pCmd != NULL)
				pCmd->execute();

			SAFE_DELETE(pCmd);

			m_queue.pop();
		}
	}
}

void CUIToolApp::addQueue( Command* pCmd )
{
	CTSingleLock sl(&m_csLock, TRUE);

	m_queue.push(pCmd);
}


