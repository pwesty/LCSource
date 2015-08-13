
// UIToolDoc.cpp : CUIToolDoc 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "UITool.h"
#endif

#include "UIToolDoc.h"
#include "MainFrm.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CUIToolDoc

IMPLEMENT_DYNCREATE(CUIToolDoc, CDocument)

BEGIN_MESSAGE_MAP(CUIToolDoc, CDocument)
	ON_COMMAND(ID_FILE_SAVE, &CUIToolDoc::OnFileSave)
	ON_COMMAND(ID_FILE_SAVE_AS, &CUIToolDoc::OnFileSaveAs)
END_MESSAGE_MAP()


// CUIToolDoc 생성/소멸

CUIToolDoc::CUIToolDoc()
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.

}

CUIToolDoc::~CUIToolDoc()
{
}

BOOL CUIToolDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 재초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.

	return TRUE;
}

// CUIToolDoc serialization

void CUIToolDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 여기에 저장 코드를 추가합니다.
	}
	else
	{
		// TODO: 여기에 로딩 코드를 추가합니다.
	}
}

#ifdef SHARED_HANDLERS

// 축소판 그림을 지원합니다.
void CUIToolDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 문서의 데이터를 그리려면 이 코드를 수정하십시오.
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 검색 처리기를 지원합니다.
void CUIToolDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 문서의 데이터에서 검색 콘텐츠를 설정합니다.
	// 콘텐츠 부분은 ";"로 구분되어야 합니다.

	// 예: strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CUIToolDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CUIToolDoc 진단

#ifdef _DEBUG
void CUIToolDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CUIToolDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CUIToolDoc 명령


void CUIToolDoc::OnFileSave()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	if( CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd() )
	{
		if (pMain->GetPath().empty() == false)
		{
			pMain->OnSaveFile( (LPSTR)(LPCSTR)pMain->GetPath().c_str() );
			return;
		}
	}

	CFileDialog dlgFile(FALSE, NULL, (LPCTSTR)theApp.getLoadFileName().c_str(), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL, 0);

	CString strFilter;
	strFilter += "UI Files(*.xml)";
	strFilter += (TCHAR)'\0';   // next string please
	strFilter += "*.xml";
	strFilter += (TCHAR)'\0';   // last string

	dlgFile.m_ofn.lpstrTitle = "UI 파일 저장";
	dlgFile.m_ofn.lpstrFilter = strFilter;
	//dlgFile.m_ofn.lpstrInitialDir = "";

	if( dlgFile.DoModal() == IDOK )
	{
		if( CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd() )
		{
			CString strPath = dlgFile.GetPathName();
			
			//char szfilePath[_MAX_EXT];
			//_splitpath_s(szfilePath, 0, NULL, szfilePath, sizeof(szfilePath));
			pMain->OnSaveFile( (LPSTR)(LPCSTR)strPath );

			if( CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd() )
			{
				std::string strTemp = strPath;
				std::size_t pos = strTemp.rfind(".xml");

				if (pos == std::string::npos)
					strTemp += ".xml";

				pMain->SetPath(strTemp);
			}

			theApp.setSaveFileName(dlgFile.GetFileTitle().GetString());	// 파일 이름 저장.
		}			
	}	
}


void CUIToolDoc::OnFileSaveAs()
{
	CFileDialog dlgFile(FALSE, NULL, (LPCTSTR)theApp.getLoadFileName().c_str(), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL, 0);

	CString strFilter;
	strFilter += "UI Files(*.xml)";
	strFilter += (TCHAR)'\0';   // next string please
	strFilter += "*.xml";
	strFilter += (TCHAR)'\0';   // last string

	dlgFile.m_ofn.lpstrTitle = "UI 파일 저장";
	dlgFile.m_ofn.lpstrFilter = strFilter;
	//dlgFile.m_ofn.lpstrInitialDir = "";

	if( dlgFile.DoModal() == IDOK )
	{
		if( CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd() )
		{
			CString strPath = dlgFile.GetPathName();

			pMain->OnSaveFile( (LPSTR)(LPCSTR)strPath );
			
			if( CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd() )
			{
				std::string strTemp = strPath;
				std::size_t pos = strTemp.rfind(".xml");

				if (pos == std::string::npos)
					strTemp += ".xml";

				pMain->SetPath(strTemp);
			}

			theApp.setSaveFileName(dlgFile.GetFileTitle().GetString());	// 파일 이름 저장.
		}			
	}
}
