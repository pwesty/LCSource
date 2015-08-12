
#include "stdafx.h"
#include "PropertiesWnd.h"
#include "UITool.h"
#include "Resource.h"
#include "MainFrm.h"

#include "ChildFrm.h"
#include "UIToolDoc.h"
#include "UIToolView.h"

#include <direct.h>
#include <Engine/Interface/UIInternalClasses.h>


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

class CmdChangeTexString : public Command
{
	// win7 64bit 에서 MsgProc() 에서 발생 시키는 exception 처리 문제로 인한 변경
	// MFC 메시지 프로시저에서 exception을 무시, 파일을 열지 못하고 assult가 일어나서 변경.
	// 메시지 큐에서 메시지 처리 하는 방식으로 변경함.
public:
	CmdChangeTexString() : m_pBase(NULL) {}
	void setData(CUIBase* pBase, const char* _strTexString) {  m_pBase = pBase; m_strTexString = _strTexString; }
	void execute()
	{
		m_pBase->setTexString(m_strTexString.c_str());
	}
private:
	CUIBase* m_pBase;
	std::string m_strTexString;
};

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar

const char* szAlign_H[3] = { "TEXT_LEFT", "TEXT_CENTER", "TEXT_RIGHT" };
const char* szAlign_V[3] = { "TEXT_TOP", "TEXT_CENTER", "TEXT_BOTTOM" };
const char* szSplit[6] = { "SPLIT_NONE", "SPLIT_SPACE", "SPLIT_HYPHEN", "SPLIT_MIXED", "SPLIT_CHAR_COUNT", "SPLIT_ONELINE" };
const char* szImageSplit[3] = { "3Horizontal", "3Vertical", "9Plane" };


CPropertiesWnd::CPropertiesWnd()
	: m_pTmpProp(NULL)
	, m_pSelectedUI(NULL)
	, m_nImageArrayOldCount(0)
	, m_nSpriteAniOldCount(0)
{
}

CPropertiesWnd::~CPropertiesWnd()
{
}

BEGIN_MESSAGE_MAP(CPropertiesWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_EXPAND_ALL, OnExpandAllProperties)
	ON_UPDATE_COMMAND_UI(ID_EXPAND_ALL, OnUpdateExpandAllProperties)
	ON_COMMAND(ID_SORTPROPERTIES, OnSortProperties)
	ON_UPDATE_COMMAND_UI(ID_SORTPROPERTIES, OnUpdateSortProperties)
	ON_COMMAND(ID_PROPERTIES1, OnProperties1)
	ON_UPDATE_COMMAND_UI(ID_PROPERTIES1, OnUpdateProperties1)
	ON_COMMAND(ID_PROPERTIES2, OnProperties2)
	ON_UPDATE_COMMAND_UI(ID_PROPERTIES2, OnUpdateProperties2)
	ON_WM_SETFOCUS()
	ON_WM_SETTINGCHANGE()
	ON_REGISTERED_MESSAGE(AFX_WM_PROPERTY_CHANGED, OnPropertyChanged)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar 메시지 처리기

void CPropertiesWnd::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient,rectCombo;
	GetClientRect(rectClient);

	//m_wndObjectCombo.GetWindowRect(&rectCombo);

	int cyCmb = rectCombo.Size().cy;
	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	//m_wndObjectCombo.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), 200, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top + cyCmb, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndPropListEx.SetWindowPos(NULL, rectClient.left, rectClient.top + cyCmb + cyTlb, rectClient.Width(), rectClient.Height() -(cyCmb+cyTlb), SWP_NOACTIVATE | SWP_NOZORDER);
}

int CPropertiesWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// 콤보 상자를 만듭니다.
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_BORDER | CBS_SORT | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

// 	if (!m_wndObjectCombo.Create(dwViewStyle, rectDummy, this, 1))
// 	{
// 		TRACE0("속성 콤보 상자를 만들지 못했습니다. \n");
// 		return -1;      // 만들지 못했습니다.
// 	}

//	m_wndObjectCombo.AddString(_T("응용 프로그램"));
//	m_wndObjectCombo.AddString(_T("속성 창"));
//	m_wndObjectCombo.SetCurSel(0);

	if (!m_wndPropListEx.Create(WS_VISIBLE | WS_CHILD, rectDummy, this, 2))
	{
		TRACE0("속성 표를 만들지 못했습니다. \n");
		return -1;      // 만들지 못했습니다.
	}

	//InitPropList();

	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_PROPERTIES);
	m_wndToolBar.LoadToolBar(IDR_PROPERTIES, 0, 0, TRUE /* 잠금 */);
	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_PROPERTIES_HC : IDR_PROPERTIES, 0, 0, TRUE /* 잠금 */);

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
	m_wndToolBar.SetOwner(this);

	// 모든 명령은 부모 프레임이 아닌 이 컨트롤을 통해 라우팅됩니다.
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	AdjustLayout();
	return 0;
}

void CPropertiesWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CPropertiesWnd::OnExpandAllProperties()
{
	m_wndPropListEx.ExpandAll();
}

void CPropertiesWnd::OnUpdateExpandAllProperties(CCmdUI* /* pCmdUI */)
{
}

void CPropertiesWnd::OnSortProperties()
{
	m_wndPropListEx.SetAlphabeticMode(!m_wndPropListEx.IsAlphabeticMode());
}

void CPropertiesWnd::OnUpdateSortProperties(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndPropListEx.IsAlphabeticMode());
}

void CPropertiesWnd::OnProperties1()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}

void CPropertiesWnd::OnUpdateProperties1(CCmdUI* /*pCmdUI*/)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
}

void CPropertiesWnd::OnProperties2()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}

void CPropertiesWnd::OnUpdateProperties2(CCmdUI* /*pCmdUI*/)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
}

void CPropertiesWnd::InitPropList()
{
	SetPropListFont();

	m_wndPropListEx.EnableHeaderCtrl(FALSE);
	m_wndPropListEx.EnableDescriptionArea();
	m_wndPropListEx.SetVSDotNetLook();
	m_wndPropListEx.MarkModifiedProperties();

	CMFCPropertyGridPropertyEx* pGroup1 = new CMFCPropertyGridPropertyEx(_T("모양"));

	pGroup1->AddSubItem(new CMFCPropertyGridPropertyEx(_T("3D 모양"), (_variant_t) false, _T("창에 굵은 글꼴이 아닌 글꼴을 지정하고, 컨트롤에 3D 테두리를 지정합니다.")));

	CMFCPropertyGridPropertyEx* pProp = new CMFCPropertyGridPropertyEx(_T("테두리"), _T("Dialog Frame"), _T("None, Thin, Resizable 또는 Dialog Frame 중 하나를 지정합니다."));
	pProp->AddOption(_T("None"));
	pProp->AddOption(_T("Thin"));
	pProp->AddOption(_T("Resizable"));
	pProp->AddOption(_T("Dialog Frame"));
	pProp->AllowEdit(FALSE);

	pGroup1->AddSubItem(pProp);
	pGroup1->AddSubItem(new CMFCPropertyGridPropertyEx(_T("캡션"), (_variant_t) _T("정보"), _T("창의 제목 표시줄에 표시되는 텍스트를 지정합니다.")));

	m_wndPropListEx.AddProperty(pGroup1);

	CMFCPropertyGridPropertyEx* pSize = new CMFCPropertyGridPropertyEx(_T("창 크기"), 0, TRUE);

	pProp = new CMFCPropertyGridPropertyEx(_T("높이"), (_variant_t) 250l, _T("창의 높이를 지정합니다."));
	pProp->EnableSpinControl(TRUE, 50, 300);
	pSize->AddSubItem(pProp);

	pProp = new CMFCPropertyGridPropertyEx( _T("너비"), (_variant_t) 150l, _T("창의 너비를 지정합니다."));
	pProp->EnableSpinControl(TRUE, 50, 200);
	pSize->AddSubItem(pProp);

	m_wndPropListEx.AddProperty(pSize);

	CMFCPropertyGridPropertyEx* pGroup2 = new CMFCPropertyGridPropertyEx(_T("글꼴"));

	LOGFONT lf;
	CFont* font = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	font->GetLogFont(&lf);

	lstrcpy(lf.lfFaceName, _T("맑은 고딕"));

	pGroup2->AddSubItem(new CMFCPropertyGridFontProperty(_T("글꼴"), lf, CF_EFFECTS | CF_SCREENFONTS, _T("창의 기본 글꼴을 지정합니다.")));
	pGroup2->AddSubItem(new CMFCPropertyGridPropertyEx(_T("시스템 글꼴을 사용합니다."), (_variant_t) true, _T("창에서 MS Shell Dlg 글꼴을 사용하도록 지정합니다.")));

	m_wndPropListEx.AddProperty(pGroup2);

	CMFCPropertyGridPropertyEx* pGroup3 = new CMFCPropertyGridPropertyEx(_T("기타"));
	pProp = new CMFCPropertyGridPropertyEx(_T("(이름)"), _T("응용 프로그램"));
	pProp->Enable(FALSE);
	pGroup3->AddSubItem(pProp);

	CMFCPropertyGridColorProperty* pColorProp = new CMFCPropertyGridColorProperty(_T("창 색상"), RGB(210, 192, 254), NULL, _T("창의 기본 색상을 지정합니다."));
	pColorProp->EnableOtherButton(_T("기타..."));
	pColorProp->EnableAutomaticButton(_T("기본값"), ::GetSysColor(COLOR_3DFACE));
	pGroup3->AddSubItem(pColorProp);

	static const TCHAR szFilter[] = _T("아이콘 파일(*.ico)|*.ico|모든 파일(*.*)|*.*||");
	pGroup3->AddSubItem(new CMFCPropertyGridFileProperty(_T("아이콘"), TRUE, _T(""), _T("ico"), 0, szFilter, _T("창 아이콘을 지정합니다.")));

	pGroup3->AddSubItem(new CMFCPropertyGridFileProperty(_T("폴더"), _T("c:\\")));

	m_wndPropListEx.AddProperty(pGroup3);

	CMFCPropertyGridPropertyEx* pGroup4 = new CMFCPropertyGridPropertyEx(_T("계층"));

	CMFCPropertyGridPropertyEx* pGroup41 = new CMFCPropertyGridPropertyEx(_T("첫번째 하위 수준"));
	pGroup4->AddSubItem(pGroup41);

	CMFCPropertyGridPropertyEx* pGroup411 = new CMFCPropertyGridPropertyEx(_T("두 번째 하위 수준"));
	pGroup41->AddSubItem(pGroup411);

	pGroup411->AddSubItem(new CMFCPropertyGridPropertyEx(_T("항목 1"), (_variant_t) _T("값 1"), _T("설명입니다.")));
	pGroup411->AddSubItem(new CMFCPropertyGridPropertyEx(_T("항목 2"), (_variant_t) _T("값 2"), _T("설명입니다.")));
	pGroup411->AddSubItem(new CMFCPropertyGridPropertyEx(_T("항목 3"), (_variant_t) _T("값 3"), _T("설명입니다.")));

	pGroup4->Expand(FALSE);
	m_wndPropListEx.AddProperty(pGroup4);
}

void CPropertiesWnd::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);
	m_wndPropListEx.SetFocus();
}

void CPropertiesWnd::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CDockablePane::OnSettingChange(uFlags, lpszSection);
	SetPropListFont();
}

void CPropertiesWnd::SetPropListFont()
{
	::DeleteObject(m_fntPropList.Detach());

	LOGFONT lf;
	afxGlobalData.fontRegular.GetLogFont(&lf);

	NONCLIENTMETRICS info;
	info.cbSize = sizeof(info);

	afxGlobalData.GetNonClientMetrics(info);

	lf.lfHeight = info.lfMenuFont.lfHeight;
	lf.lfWeight = info.lfMenuFont.lfWeight;
	lf.lfItalic = info.lfMenuFont.lfItalic;

	m_fntPropList.CreateFontIndirect(&lf);

	m_wndPropListEx.SetFont(&m_fntPropList);
	//m_wndObjectCombo.SetFont(&m_fntPropList);

	//initData( DEF_PROPERTY_BASE | DEF_PROPERTY_UV );
}

void CPropertiesWnd::parseUI(CUIBase* pUI)
{
	if( pUI == NULL )
	{
		CMainFrame* pMain = (CMainFrame *)AfxGetMainWnd();
		pMain->Message("parseUI(...) : 해당 컨트롤의 Pointer가 없습니다.");
		return;
	}

	m_wndPropListEx.RemoveAll();
	Refresh();

	ShowPropBase(pUI);
	m_pSelectedUI = pUI;

	switch( pUI->getType() )
	{
	case eUI_CONTROL_BASE:
		{

		}
		break;
	case eUI_CONTROL_WINDOW:
		{
		}
		break;
	case eUI_CONTROL_IMAGE:
		{
			CUIImage* pImg = dynamic_cast<CUIImage*>(pUI);
			if( pImg )
				ShowPropImage( pImg );
		}
		break;
	case eUI_CONTROL_BUTTON:
		{
			CUIButton* pBtn= dynamic_cast<CUIButton*>(pUI);
			if( pBtn )
				ShowPropButton( pBtn );
		}
		break;

	case eUI_CONTROL_TEXT:
		{
			CUIText* pTxt= dynamic_cast<CUIText*>(pUI);
			if( pTxt )
				ShowPropText( pTxt );
		}
		break;
	case eUI_CONTROL_TEXTBOX:
		{
			CUITextBox* pTextBox = dynamic_cast<CUITextBox*>(pUI);
			if( pTextBox )
				ShowPropTextBox( pTextBox );
		}
		break;
	case eUI_CONTROL_TEXTBOX_EX:
		{
			CUITextBoxEx* pTBEx = dynamic_cast<CUITextBoxEx*>(pUI);
			if(pTBEx)
				ShowPropTextBoxEx(pTBEx);
		}
		break;
	case eUI_CONTROL_EDIT:
		{
			CUIEditBox* pEdit= dynamic_cast<CUIEditBox*>(pUI);
			if( pEdit )
				ShowPropEdit( pEdit );
		}
		break;
	case eUI_CONTROL_SCROLL:
		{
			CUIScrollBar* pScroll= dynamic_cast<CUIScrollBar*>(pUI);
			if( pScroll )
				ShowPropScrollBar( pScroll );
		}
		break;
	case eUI_CONTROL_CHECKBOX:
		{
			CUICheckButton* pCheckBox= dynamic_cast<CUICheckButton*>(pUI);
			if( pCheckBox )
				ShowPropCheckButton(pCheckBox);
		}
		break;
	case eUI_CONTROL_COMBOBOX:
		{
			CUIComboBox* pCombo = dynamic_cast<CUIComboBox*>(pUI);
			if( pCombo )
				ShowPropComboBox(pCombo);
		}
		break;
	case eUI_CONTROL_SLIDEBAR:
		{
			CUISlideBar* pSlide = dynamic_cast<CUISlideBar*>(pUI);
			if( pSlide )
				ShowPropSlideBar(pSlide);
		}
		break;
	case eUI_CONTROL_PROGRESSBAR:
		{
			CUIProgressBar* pProgressBar = dynamic_cast<CUIProgressBar*>(pUI);
			if( pProgressBar )
				ShowPropProgressBar(pProgressBar);
		}
		break;
	case eUI_CONTROL_LIST:
		{
			CUIList* pList = dynamic_cast<CUIList*>(pUI);
			if( pList )
				ShowPropList(pList);
		}
		break;
	case eUI_CONTROL_ICON:
		{
			CUIIcon* pIcon	= dynamic_cast<CUIIcon*>(pUI);
			if( pIcon )
				ShowPropIcon(pIcon);
		}
		break;
	case eUI_CONTROL_SPINBUTTON:
		{
			CUISpinButton* pSpinBtn = dynamic_cast<CUISpinButton*>(pUI);
			if( pSpinBtn )
				ShowPropSpinButton(pSpinBtn);
		}
		break;
	case eUI_CONTROL_ARRAY:
		{
			CUIArray* pArray = dynamic_cast<CUIArray*>(pUI);
			if( pArray )
				ShowPropArray(pArray);
		}
		break;
	case eUI_CONTROL_IMAGEFONT:
		{
			CUIImageFont* pImgFont = dynamic_cast<CUIImageFont*>(pUI);
			if( pImgFont )
				ShowPropImageFont(pImgFont);
		}
	case eUI_CONTROL_SPRITEANI:
		{
			CUISpriteAni* pSpriteAni = dynamic_cast<CUISpriteAni*>(pUI);
			if( pSpriteAni )
				ShowPropSpriteAni(pSpriteAni);
		}
	case eUI_CONTROL_IMAGE_ARRAY:
		{
			CUIImageArray* pImageArray = dynamic_cast<CUIImageArray*>(pUI);
			if (pImageArray)
				ShowPropImageArray(pImageArray);
		}
	case eUI_CONTROL_IMAGE_SPLIT:
		{
			CUIImageSplit* pImageSplit = dynamic_cast<CUIImageSplit*>(pUI);
			if (pImageSplit)
				ShowPropImageSplit(pImageSplit);
		}
	case eUI_CONTROL_SPIN_CONTROL:
		{
			CUISpinControl* pSpinControl = dynamic_cast<CUISpinControl*>(pUI);
			if (pSpinControl)
				ShowPropSpinControl(pSpinControl);
		}
		break;

	case eUI_CONTROL_TREE:
		{
			CUITree* pTree = dynamic_cast<CUITree*>(pUI);
			if (pTree)
				ShowPropTree(pTree);
		}
		break;
	}
}

void CPropertiesWnd::Refresh()
{
	if( CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd() )
	{
		pMain->udpateRect();
	}
}

void CPropertiesWnd::SetUV( int nMsgID_L, int nMsgID_T, int nMsgID_R, int nMsgID_B, UIRectUV uv, CMFCPropertyGridProperty* pGroup, std::string strName /*= " UV "*/ )
{
	CMFCPropertyGridPropertyEx* pPropUV = new CMFCPropertyGridPropertyEx(_T(strName.c_str()));
	
	char buff[16];

	itoa(int(uv.U0), buff, 10);
	CMFCPropertyGridPropertyEx* pProp = new CMFCPropertyGridPropertyEx(_T("Left"), (_variant_t) buff, _T("Left"), (ePROP_TYPE)nMsgID_L);
	pPropUV->AddSubItem(pProp);

	itoa(int(uv.V0), buff, 10);
	pProp = new CMFCPropertyGridPropertyEx( _T("Top"), (_variant_t) buff, _T("Top"), (ePROP_TYPE)nMsgID_T);
	pPropUV->AddSubItem(pProp);

	itoa(int(uv.U1), buff, 10);
	pProp = new CMFCPropertyGridPropertyEx( _T("Right"), (_variant_t) buff, _T("Right"), (ePROP_TYPE)nMsgID_R);
	pPropUV->AddSubItem(pProp);

	itoa(int(uv.V1), buff, 10);
	pProp = new CMFCPropertyGridPropertyEx( _T("Bottom"), (_variant_t) buff, _T("Bottom"), (ePROP_TYPE)nMsgID_B);
	pPropUV->AddSubItem(pProp);

	pPropUV->Expand(TRUE);
	pGroup->AddSubItem(pPropUV);
}

void CPropertiesWnd::SetRect( int nMsgID_L, int nMsgID_T, int nMsgID_R, int nMsgID_B , UIRect rc, CMFCPropertyGridProperty* pGroup )
{
	CMFCPropertyGridPropertyEx* pPos = new CMFCPropertyGridPropertyEx(_T("위치             ( Position )"));
	CMFCPropertyGridPropertyEx* pProp = new CMFCPropertyGridPropertyEx(_T("X"), (_variant_t) rc.Left, _T("X"), (ePROP_TYPE)nMsgID_L);
	pProp->EnableSpinControl(TRUE, -3000, 3000);
	pPos->AddSubItem(pProp);

	pProp = new CMFCPropertyGridPropertyEx( _T("Y"), (_variant_t) (int)rc.Top, _T("Y"), (ePROP_TYPE)nMsgID_T);
	pProp->EnableSpinControl(TRUE, -3000, 3000);
	pPos->AddSubItem(pProp);
	pPos->Expand(TRUE);
	pGroup->AddSubItem(pPos);

	CMFCPropertyGridPropertyEx* pSize = new CMFCPropertyGridPropertyEx(_T("사이즈           ( Position )"));
	pProp = new CMFCPropertyGridPropertyEx( _T("Width"), (_variant_t) rc.Right, _T("Width"), (ePROP_TYPE)nMsgID_R);
	pProp->EnableSpinControl(TRUE, -3000, 3000);
	pSize->AddSubItem(pProp);

	pProp = new CMFCPropertyGridPropertyEx( _T("Height"), (_variant_t) rc.Bottom, _T("Height"), (ePROP_TYPE)nMsgID_B);
	pProp->EnableSpinControl(TRUE, -3000, 3000);
	pSize->AddSubItem(pProp);
	pSize->Expand(TRUE);
	pGroup->AddSubItem(pSize);
}


// void CPropertiesWnd::SetComboUV( ePROP_TYPE eMsg_L, UIRectUV uv, CMFCPropertyGridPropertyEx* pGroup, char* szPos )
// {
// 	CMFCPropertyGridPropertyEx* pName;
// 	if( szPos != NULL )
// 		 pName = new CMFCPropertyGridPropertyEx(_T(szPos));
// 
// 	CMFCPropertyGridPropertyEx* pPos  = new CMFCPropertyGridPropertyEx(_T("위치"));
// 	CMFCPropertyGridPropertyEx* pProp = new CMFCPropertyGridPropertyEx(_T("Left"), (_variant_t) (int)uv.U0, _T("Left"), (int)eMsg_L);;
// 	pProp->EnableSpinControl(TRUE, 0, 3000);
// 	pPos->AddSubItem(pProp);
// 
// 	pProp = new CMFCPropertyGridPropertyEx( _T("Top"), (_variant_t) (int)uv.V0, _T("Top"), (int)eMsg_L+1);
// 	pProp->EnableSpinControl(TRUE, 0, 3000);
// 	pPos->AddSubItem(pProp);
// 	pPos->Expand(TRUE);
// 	pName->AddSubItem(pPos);
// 	//pGroup->AddSubItem(pName);
// 
// 	CMFCPropertyGridPropertyEx* pSize = new CMFCPropertyGridPropertyEx(_T("사이즈"));
// 	pProp = new CMFCPropertyGridPropertyEx( _T("Right"), (_variant_t) (int)uv.U1, _T("Right"), (int)eMsg_L+2);
// 	pProp->EnableSpinControl(TRUE, 0, 3000);
// 	pSize->AddSubItem(pProp);
// 
// 	pProp = new CMFCPropertyGridPropertyEx( _T("Bottom"), (_variant_t) (int)uv.V1, _T("Bottom"), (int)eMsg_L+3);
// 	pProp->EnableSpinControl(TRUE, 0, 3000);
// 	pSize->AddSubItem(pProp);
// 	pSize->Expand(TRUE);
// 	pName->AddSubItem(pSize);
// 	pGroup->AddSubItem(pName);
// }

void CPropertiesWnd::SetTextString(ePROP_TYPE eMsg_type, CMFCPropertyGridProperty* pGroup, std::string str, std::string name, std::string desc /*= ""*/ )
{
	CMFCPropertyGridPropertyEx* pProp;

	if( str.empty() == false )
		pProp = new CMFCPropertyGridPropertyEx( _T(name.c_str()), (_variant_t) _T(str.c_str()), _T(desc.c_str()), eMsg_type);
	else
		pProp = new CMFCPropertyGridPropertyEx( _T(name.c_str()), (_variant_t) _T(""), _T(name.c_str()), eMsg_type);
	pGroup->AddSubItem(pProp);
}

bool CPropertiesWnd::SetBtnUV( ePROP_TYPE eType, CUIButton* pBtn, std::string& strVal )
{	
	UIBtnState state;
	ePROP_TYPE start_type;

	if( eType < ePROP_BTN_IDLE_L && eType > ePROP_BTN_DISABLE_B )
		return false;

	// 타입별로 구분
	if( eType >= ePROP_BTN_IDLE_L && eType <= ePROP_BTN_IDLE_B )
	{
		start_type = ePROP_BTN_IDLE_L;
		state = UBS_IDLE;
	}
	else if( eType >= ePROP_BTN_ON_L && eType <= ePROP_BTN_ON_B)
	{
		start_type = ePROP_BTN_ON_L;
		state = UBS_ON;
	}
	else if( eType >= ePROP_BTN_CLICK_L && eType <= ePROP_BTN_CLICK_B)
	{
		start_type = ePROP_BTN_CLICK_L;
		state = UBS_CLICK;
	}
	else if( eType >= ePROP_BTN_DISABLE_L && eType <= ePROP_BTN_DISABLE_B)
	{
		start_type = ePROP_BTN_DISABLE_L;
		state = UBS_DISABLE;
	}

	UIRectUV uv = pBtn->GetUV(state);

	ExtractionUV((ePROP_TYPE)eType, start_type, uv, strVal);

	pBtn->SetUV( state, uv );
	return true;
}

void CPropertiesWnd::ExtractionUV( ePROP_TYPE eType, ePROP_TYPE eStart, UIRectUV& uv, std::string& val )
{
	int nVal;
	int findPos = 0;
	char buff[64];

	if( val.empty() == true )
		return;

	if( (findPos = val.find('+')) >= 0 )
	{
		string::iterator pos = remove(val.begin(), val.end(), '+');
		val.erase(pos, val.end());
	}

	nVal = atoi(val.c_str());

	if( eType == eStart )
		uv.U0 = nVal;
	else if( eType == (ePROP_TYPE)(eStart + 1) )
		uv.V0 = nVal;

	if( findPos >= 0 )
	{
		if( eType == (ePROP_TYPE)(eStart + 2) )
		{
			uv.U1 = uv.U0 + nVal;
			val = itoa(int(uv.U1), buff, 10);
		}
		else if( eType == (ePROP_TYPE)(eStart + 3) )
		{
			uv.V1 = uv.V0 + nVal;
			val = itoa(int(uv.V1), buff, 10);
		}
	}
	else
	{
		if( eType == (ePROP_TYPE)(eStart + 2) )
			uv.U1 = nVal;
		else if( eType == (ePROP_TYPE)(eStart + 3) )
			uv.V1 = nVal;
	}
}

void CPropertiesWnd::ExtractionRect( ePROP_TYPE eType, ePROP_TYPE eStart, UIRect& rc, int nVal )
{
	if( eType == eStart )
		rc.Left		 = nVal;
	else if( eType == (ePROP_TYPE)(eStart + 1) )
		rc.Top		 = nVal;
	else if( eType == (ePROP_TYPE)(eStart + 2) )
		rc.Right	 = nVal;
	else if( eType == (ePROP_TYPE)(eStart + 3) )
		rc.Bottom	 = nVal;
}

void CPropertiesWnd::SetImage( int nMsgID, std::string strImage, CMFCPropertyGridProperty* pGroup )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if( strImage.empty() == TRUE )
		CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();

	static const TCHAR szFilter[] = _T("File(*.tex)|*.tex|File(*.png)|*.png||");

	if( strImage.empty() == true )
		strImage = "Texture를 지정해 주세요";
		
	pGroup->AddSubItem(new CMFCPropertyGridFileProperty(_T("Tex File"), TRUE, 
		_T(strImage.c_str()), _T(""), 0, szFilter, _T("Texture 파일 지정"), (ePROP_TYPE)nMsgID));
}

void CPropertiesWnd::ShowPropBase( CUIBase* pBase )
{
	CUIManager* pUIMgr = CUIManager::getSingleton();

	if( pUIMgr == NULL )
		return;

	CUIWindow* pWnd = (CUIWindow*)pUIMgr->getChildAt(0);

	SetPropListFont();

	m_wndPropListEx.EnableHeaderCtrl(FALSE);
	m_wndPropListEx.EnableDescriptionArea();
	m_wndPropListEx.SetVSDotNetLook();
	m_wndPropListEx.MarkModifiedProperties();

	std::string strId = pBase->getID();
	CMFCPropertyGridPropertyEx* pGroup = new CMFCPropertyGridPropertyEx(_T("Base"), (DWORD_PTR)pWnd);

	if( strId.empty() == FALSE )
		pGroup->AddSubItem(new CMFCPropertyGridPropertyEx(_T("ID"), (_variant_t) strId.c_str(), 0, ePROP_TYPE_ID, _T("ID")));
	else
		pGroup->AddSubItem(new CMFCPropertyGridPropertyEx(_T("ID"), (_variant_t) "NULL", 0, ePROP_TYPE_ID, _T("NULL")));

	strId = pBase->getPID();
	if( strId.empty() == FALSE )
		pGroup->AddSubItem(new CMFCPropertyGridPropertyEx(_T("Parent ID"), (_variant_t) strId.c_str(), 0, ePROP_TYPE_PID, _T("부모의 ID를 입력 넣어줄 시 부모 컨트롤의 자식이됨")));
	else
		pGroup->AddSubItem(new CMFCPropertyGridPropertyEx(_T("Parent ID"), (_variant_t) "NULL", 0, ePROP_TYPE_PID, _T("부모의 ID를 입력 넣어줄 시 부모 컨트롤의 자식이됨")));

	bool bHide = pBase->GetHide() ? true : false;
	pGroup->AddSubItem(new CMFCPropertyGridPropertyEx(_T("Hide"), (_variant_t) bHide ,0 , ePROP_TYPE_HIDE, _T("보여줄지 말지 선택")));

	CMFCPropertyGridPropertyEx* pPos		= new CMFCPropertyGridPropertyEx(_T("위치") );
	CMFCPropertyGridPropertyEx* pProp		= new CMFCPropertyGridPropertyEx(_T("X"), (_variant_t) pBase->GetOrigX(), 0, ePROP_TYPE_X, _T("X"));
	pProp->EnableSpinControl(TRUE, -3000, 3000);
	pPos->AddSubItem(pProp);

	pProp = new CMFCPropertyGridPropertyEx( _T("Y"), (_variant_t) pBase->GetOrigY(), 0, ePROP_TYPE_Y, _T("Y"));
	pProp->EnableSpinControl(TRUE, -3000, 3000);
	pPos->AddSubItem(pProp);
	pPos->Expand(TRUE);
	pGroup->AddSubItem(pPos);

	CMFCPropertyGridPropertyEx* pSize		 = new CMFCPropertyGridPropertyEx(_T("사이즈"));
	pProp = new CMFCPropertyGridPropertyEx( _T("Width"), (_variant_t) pBase->GetWidth(), 0, ePROP_TYPE_W, _T("Width"));
	pProp->EnableSpinControl(TRUE, -3000, 3000);
	pSize->AddSubItem(pProp);

	pProp = new CMFCPropertyGridPropertyEx( _T("Height"), (_variant_t) pBase->GetHeight(), 0, ePROP_TYPE_H, _T("Height"));
	pProp->EnableSpinControl(TRUE, -3000, 3000);
	pSize->AddSubItem(pProp);
	pGroup->AddSubItem(pSize);
	pSize->Expand(TRUE);

	int nAlignH = (int)pBase->getAlignH();
	int nAlignV = (int)pBase->getAlignV();

	pProp = new CMFCPropertyGridPropertyEx( _T("Align_H"), (_variant_t) szAlign_H[nAlignH], _T("Left, Center, Right 정렬"), ePROP_TYPE_ALIGN_H);
	for(int i = 0; i < 3; i++)
		pProp->AddOption(_T(szAlign_H[i]));
	pProp->AllowEdit(FALSE);
	pGroup->AddSubItem(pProp);

	pProp = new CMFCPropertyGridPropertyEx( _T("Align_V"), (_variant_t) szAlign_V[nAlignV], _T("Top, Center, Bottom 정렬"), ePROP_TYPE_ALIGN_V);
	for(int i = 0; i < 3; i++)
		pProp->AddOption(_T(szAlign_V[i]));
	pProp->AllowEdit(FALSE);
	pGroup->AddSubItem(pProp);

	pProp = new CMFCPropertyGridPropertyEx( _T("Tooltip Index"), (_variant_t) pBase->GetTooltipIndex(), _T("툴팁 인덱스 입력"), ePROP_TYPE_TOOLTIP);
	pGroup->AddSubItem(pProp);

	pProp = new CMFCPropertyGridPropertyEx( _T("Tooltip Width"), (_variant_t) pBase->getTooltipWidth(), _T("툴팁 Width 입력 (미입력시 1줄로 출력)"), ePROP_TYPE_TOOLTIP_WIDTH);
	pGroup->AddSubItem(pProp);

	pGroup->Expand(TRUE);
	//pGroup->Enable(FALSE);
	m_wndPropListEx.AddProperty(pGroup);
}

void CPropertiesWnd::ShowPropImage( CUIImage* pUI )
{
	CMFCPropertyGridPropertyEx* pGroup = new CMFCPropertyGridPropertyEx(_T("이미지"));

	UIRectUV uv = pUI->GetAbsUV();
	SetUV( (int)ePROP_IMG_TYPE_L, (int)ePROP_IMG_TYPE_T
		, (int)ePROP_IMG_TYPE_R, (int)ePROP_IMG_TYPE_B
		, uv, pGroup );

	std::string strImage = pUI->getTexString();
	SetImage( (int)ePROP_IMG_TYPE_TEX, strImage, pGroup );

	pGroup->Expand(TRUE);
	m_wndPropListEx.AddProperty(pGroup);
}

void CPropertiesWnd::ShowPropButton( CUIButton* pUI )
{
	CMFCPropertyGridPropertyEx* pGroup = new CMFCPropertyGridPropertyEx(_T("Button"));
	
	int nStrIdx = pUI->getStringIdx();
	CMFCPropertyGridPropertyEx* pProp = new CMFCPropertyGridPropertyEx(_T("String Index"), (_variant_t) nStrIdx, _T("string Index"), (int)ePROP_BTN_STR_IDX );
	pGroup->AddSubItem( pProp );

	std::string str = pUI->GetText();
	SetTextString(ePROP_BTN_STR, pGroup, str, "String");

	int i, nBtnMax = 4;

	bool bEdge = pUI->GetEdge();
	pGroup->AddSubItem(new CMFCPropertyGridPropertyEx(_T("Edge"), (_variant_t) bEdge ,0 , ePROP_BTN_EDGE, _T("외곽선 표시")));
	
	COLORREF col = ByteSwap(pUI->GetTextColor((UIBtnState)0));
	CMFCPropertyGridColorProperty* pColorProp = new CMFCPropertyGridColorProperty(_T("Text Color"), col, NULL, _T("Text Color"), ePROP_BTN_TEXT_COLOR);
	pColorProp->EnableOtherButton(_T("기타..."));
	pColorProp->EnableAutomaticButton(_T("기본값"), ::GetSysColor(COLOR_3DFACE));
	pGroup->AddSubItem(pColorProp);

	const char* type[] = { "Idle", "On", "Click", "Disable" };

 	std::string strImg = pUI->getTexString();

	for( i = 0; i < nBtnMax; i++ )
	{
		UIRectUV uv;
		uv = pUI->GetUV( (UIBtnState)i );
		CMFCPropertyGridPropertyEx* pType = new CMFCPropertyGridPropertyEx(_T(type[i]) );
		SetUV( (int)ePROP_BTN_IDLE_L + i*nBtnMax, (int)ePROP_BTN_IDLE_T + i*nBtnMax
			,(int)ePROP_BTN_IDLE_R + i*nBtnMax, (int)ePROP_BTN_IDLE_B + i*nBtnMax
			,uv, pType );
		pType->Expand(FALSE);
		pGroup->AddSubItem(pType);
	}
	SetImage( (int)ePROP_BTN_TEX, strImg, pGroup );
	pGroup->Expand(TRUE);
	m_wndPropListEx.AddProperty(pGroup);
}


void CPropertiesWnd::ShowPropText( CUIText* pUI )
{
	CMFCPropertyGridPropertyEx* pGroup = new CMFCPropertyGridPropertyEx(_T("TEXT"));

	int nStrIdx = pUI->getStringIdx();
	CMFCPropertyGridPropertyEx* pProp = new CMFCPropertyGridPropertyEx(_T("String Index"), (_variant_t) nStrIdx, _T("String Index"), ePROP_TEXT_STR_IDX);
	pGroup->AddSubItem(pProp);

	std::string str = pUI->getText();
	SetTextString(ePROP_TEXT_STR, pGroup, str, "String");

	str = pUI->getEllipsisText();
	SetTextString(ePROP_TEXT_STR_ELLIPSIS, pGroup, str, "Ellipsis String");
	
	bool bShadow = pUI->getShadow();
	COLORREF col = ByteSwap(pUI->getFontColor());
	CMFCPropertyGridColorProperty* pColorProp = new CMFCPropertyGridColorProperty(_T("Color"), col, NULL, _T("Color"), ePROP_TEXT_COLOR);
	pColorProp->EnableOtherButton(_T("기타..."));
	pColorProp->EnableAutomaticButton(_T("기본값"), ::GetSysColor(COLOR_3DFACE));
	pGroup->AddSubItem(pColorProp);

	pProp = new CMFCPropertyGridPropertyEx( _T("Shadow"), (_variant_t) bShadow, _T("Shadow"), ePROP_TEXT_SHADOW);
	pGroup->AddSubItem(pProp);

	pProp = new CMFCPropertyGridPropertyEx( _T("Align_H"), (_variant_t) szAlign_H[pUI->getAlignTextH()], _T("Left, Center, Right 정렬"), ePROP_TEXT_ALIGN);
	for(int i = 0; i < 3; i++)
		pProp->AddOption(_T(szAlign_H[i]));
	pProp->AllowEdit(FALSE);
	pGroup->AddSubItem(pProp);

	bool bEdge = pUI->getEdge() ? true : false;
	pGroup->AddSubItem(new CMFCPropertyGridPropertyEx(_T("Edge"), (_variant_t) bEdge ,0 , ePROP_TEXT_EDGE, _T("외곽선 표시")));

	col = ByteSwap(pUI->getFontShadow());
	pColorProp = new CMFCPropertyGridColorProperty(_T("Shadow Color"), ByteSwap(pUI->getFontShadow()), NULL, _T("Shadow Color"), ePROP_TEXT_SHADOW_COLOR);
	pColorProp->EnableOtherButton(_T("기타..."));
	pColorProp->EnableAutomaticButton(_T("기본값"), ::GetSysColor(COLOR_3DFACE));
	pGroup->AddSubItem(pColorProp);

	//pProp = new CMFCPropertyGridPropertyEx( _T("Depth"), (_variant_t) pUI->getDepth(), _T("Depth"), ePROP_TEXT_DEPTH);
	//pGroup->AddSubItem(pProp);

	UIRectUV uv;
	UIRect	 rc;
	pUI->getBackGround(rc, uv);
	CMFCPropertyGridPropertyEx* pBack = new CMFCPropertyGridPropertyEx(_T("BackGround Image"));
	SetRect(ePROP_TEXT_BACK_RECT_X, ePROP_TEXT_BACK_RECT_Y, ePROP_TEXT_BACK_RECT_W, ePROP_TEXT_BACK_RECT_H, rc, pBack);
	SetUV(ePROP_TEXT_BACK_UV_L, ePROP_TEXT_BACK_UV_T, ePROP_TEXT_BACK_UV_R, ePROP_TEXT_BACK_UV_B, uv, pBack);

	std::string strImg = pUI->getTexString();
	SetImage( (int)ePROP_TEXT_TEX, strImg.c_str(), pBack );

	pBack->Expand(FALSE);
	pGroup->AddSubItem(pBack);

	pGroup->Expand(TRUE);
	m_wndPropListEx.AddProperty(pGroup);
}

void CPropertiesWnd::ShowPropEdit( CUIEditBox* pUI )
{
	int nMaxChar = pUI->GetMaxChar();
	CMFCPropertyGridPropertyEx* pGroup = new CMFCPropertyGridPropertyEx(_T("Edit Control"));

	CMFCPropertyGridPropertyEx* pProp = new CMFCPropertyGridPropertyEx( _T("최대글자 수"), (_variant_t) nMaxChar, _T("최대글자 수"), ePROP_EDIT_MAX_CHAR);
	pProp->EnableSpinControl(TRUE, 0, 128);
	pGroup->AddSubItem(pProp);

	BOOL bPassword, bMsgInput;
	bool bFocus			= pUI->IsFocused() ? true : false;
	bool bCursorMove	= pUI->GetCursorMove() ? true : false;
	pUI->GetPassWord( bPassword, bMsgInput );
	bool bPasswordMode = bPassword ? true : false;

	pProp = new CMFCPropertyGridPropertyEx( _T("Password모드"), (_variant_t) bPasswordMode, _T("Password모드"), ePROP_EDIT_PASSWORD_SET);
	pGroup->AddSubItem(pProp);
	pProp = new CMFCPropertyGridPropertyEx( _T("커서이동"), (_variant_t) bCursorMove, _T("커서이동"), ePROP_EDIT_CURSOR_MOVER);
	pGroup->AddSubItem(pProp);

	pProp = new CMFCPropertyGridPropertyEx( _T("ReadWindow UV"));
	UIRectUV uvReadWnd = pUI->getReadingWindowUV(), uvCandi = pUI->getCandidateUV();
	SetUV( (int)ePROP_EDIT_READWINDOW_L, (int)ePROP_EDIT_READWINDOW_T
		, (int)ePROP_EDIT_READWINDOW_R, (int)ePROP_EDIT_READWINDOW_B
		, uvReadWnd, pProp );
	pGroup->AddSubItem(pProp);

	pProp = new CMFCPropertyGridPropertyEx( _T("Candidate UV"));
	SetUV( (int)ePROP_EDIT_CANDIDATE_UV_L, (int)ePROP_EDIT_CANDIDATE_UV_T
		, (int)ePROP_EDIT_CANDIDATE_UV_R, (int)ePROP_EDIT_CANDIDATE_UV_B
		, uvCandi, pProp );
	pGroup->AddSubItem(pProp);

	UIRectUV uvBack = pUI->getBackGroundUV();
	UIRect	 rcBack = pUI->getBackGroundRect();
	
	CMFCPropertyGridPropertyEx* pBack = new CMFCPropertyGridPropertyEx( _T("배경 이미지") );
	SetUV(ePROP_EDIT_BACK_UV_L, ePROP_EDIT_BACK_UV_T, ePROP_EDIT_BACK_UV_R, ePROP_EDIT_BACK_UV_B,uvBack, pBack);
	SetRect(ePROP_EDIT_BACK_RECT_X, ePROP_EDIT_BACK_RECT_Y, ePROP_EDIT_BACK_RECT_W, ePROP_EDIT_BACK_RECT_H, rcBack, pBack);
	pBack->Expand(FALSE);
	pGroup->AddSubItem(pBack);

	std::string strImg = pUI->getTexString();
	SetImage( (int)ePROP_EDIT_TEX, strImg.c_str(), pGroup );

	pGroup->Expand(TRUE);
	m_wndPropListEx.AddProperty(pGroup);
}

void CPropertiesWnd::ShowPropCheckButton( CUICheckButton* pUI )
{
	CMFCPropertyGridPropertyEx* pGroup = new CMFCPropertyGridPropertyEx(_T("CheckButton Control"));

	bool bLeft	 = pUI->GetLeft() ? true : false;
	int  nText_X = pUI->GetTextSX();
	int  nRegion = pUI->GetCheckRegion();

	int nStrIdx = pUI->getStringIndex();
	CMFCPropertyGridPropertyEx* pProp = new CMFCPropertyGridPropertyEx(_T("String Index"), (_variant_t) nStrIdx, _T("String Index"), ePROP_CHECK_STR_IDX);
	pProp->EnableSpinControl(TRUE, 0, 3000);
	pGroup->AddSubItem(pProp);

	std::string str = pUI->getText();
	SetTextString(ePROP_CHECK_STR, pGroup, str, "String");

	pProp = new CMFCPropertyGridPropertyEx(_T("Pos Left"), (_variant_t) bLeft, _T("Pos Left"), ePROP_CHECK_POS_LEFT);
	pGroup->AddSubItem(pProp);

	pProp = new CMFCPropertyGridPropertyEx(_T("Text X"), (_variant_t) nText_X, _T("Text X"), ePROP_CHECK_TEXT_X);
	pGroup->AddSubItem(pProp);

	pProp = new CMFCPropertyGridPropertyEx(_T("Check Region"), (_variant_t) nRegion, _T("Check Region"), ePROP_CHECK_REGION);
	pGroup->AddSubItem(pProp);

	int nTextArea = pUI->getTextArea();
	int	nAlign	  = pUI->getAlignTextH();

	pProp = new CMFCPropertyGridPropertyEx(_T("Text Area"), (_variant_t) nTextArea, _T("텍스트 영역"), ePROP_CHECK_TEXT_AREA);
	pGroup->AddSubItem(pProp);

	pProp = new CMFCPropertyGridPropertyEx(_T("Align"), (_variant_t) szAlign_H[nAlign], _T("정렬"), ePROP_CHECK_ALIGN);
	for(int i = 0; i < 3; i++)
		pProp->AddOption(_T(szAlign_H[i]));
	pProp->AllowEdit(FALSE);
	pGroup->AddSubItem(pProp);

	bool bEdge = pUI->getEdge();
	pGroup->AddSubItem(new CMFCPropertyGridPropertyEx(_T("Edge"), (_variant_t) bEdge ,0 , ePROP_CHECK_EDGE, _T("외곽선 표시")));

	COLOR colOn, colOff;
	pUI->GetTextColor(colOn, colOff);

	CMFCPropertyGridColorProperty* pColorProp = new CMFCPropertyGridColorProperty(_T("Color On"), ByteSwap(colOn), NULL, _T("Color On"), ePROP_CHECK_COLOR_ON);
	pColorProp->EnableOtherButton(_T("기타..."));
	pColorProp->EnableAutomaticButton(_T("기본값"), ::GetSysColor(COLOR_3DFACE));
	pGroup->AddSubItem(pColorProp);

	pColorProp = new CMFCPropertyGridColorProperty(_T("Color Off"), ByteSwap(colOff), NULL, _T("Color Off"), ePROP_CHECK_COLOR_OFF);
	pColorProp->EnableOtherButton(_T("기타..."));
	pColorProp->EnableAutomaticButton(_T("기본값"), ::GetSysColor(COLOR_3DFACE));
	pGroup->AddSubItem(pColorProp);

	UIRectUV uvNone = pUI->GetUV(UCBS_NONE), uvCheck = pUI->GetUV(UCBS_CHECK);
	pProp = new CMFCPropertyGridPropertyEx( _T("Button None"));
	SetUV( (int)ePROP_CHECK_NONE_L, (int)ePROP_CHECK_NONE_T
		, (int)ePROP_CHECK_NONE_R, (int)ePROP_CHECK_NONE_B
		, uvNone, pProp );
	pGroup->AddSubItem(pProp);

	pProp = new CMFCPropertyGridPropertyEx( _T("Button Check"));
	SetUV( (int)ePROP_CHECK_CHECK_L, (int)ePROP_CHECK_CHECK_T
		, (int)ePROP_CHECK_CHECK_R, (int)ePROP_CHECK_CHECK_B
		, uvCheck, pProp );
	pGroup->AddSubItem(pProp);

	std::string strImg = pUI->getTexString();
	SetImage( (int)ePROP_CHECK_TEX, strImg.c_str(), pGroup );

	pGroup->Expand(TRUE);
	m_wndPropListEx.AddProperty(pGroup);
}

void CPropertiesWnd::ShowPropComboBox( CUIComboBox* pUI )
{
	CMFCPropertyGridPropertyEx* pGroup = new CMFCPropertyGridPropertyEx(_T("ComboBox"));

	int nMaxLine = pUI->GetMaxLine();
	int nLineHeight = pUI->getlineHeight();
	int nSpace_x, nSpace_y, nShowLine, nDropAreaUnit;
	UIRectUV uvBase, uvComboBtn0, uvComboBtn1, uvDropArea;
	UIRect	 rcBase, rcComboBtn, rcDropArea;
	pUI->getBackGround(rcBase, uvBase);
	pUI->getComboButton(rcComboBtn, uvComboBtn0, uvComboBtn1);
	pUI->getDropArea(rcDropArea, uvDropArea, nDropAreaUnit);
	pUI->getSpace(nSpace_x, nSpace_y);
	nShowLine = pUI->getShowLine();

	CMFCPropertyGridPropertyEx* pProp = new CMFCPropertyGridPropertyEx(_T("MaxLine"), (_variant_t) nMaxLine, _T("MaxLine"), ePROP_COMBO_MAXLINE);
	pGroup->AddSubItem(pProp);

	pProp = new CMFCPropertyGridPropertyEx(_T("Show Line"), (_variant_t) nShowLine, _T("보여줄 라인"), ePROP_COMBO_SHOW_LINE);
	pGroup->AddSubItem(pProp);

	pProp = new CMFCPropertyGridPropertyEx(_T("LineHeight"), (_variant_t) nLineHeight, _T("LineHeight"), ePROP_COMBO_LINE_HEIGHT);
	pGroup->AddSubItem(pProp);

	pProp = new CMFCPropertyGridPropertyEx(_T("Space_X"), (_variant_t) nSpace_x, _T("Space_X"), ePROP_COMBO_SPACE_X);
	pGroup->AddSubItem(pProp);

	pProp = new CMFCPropertyGridPropertyEx(_T("Space_Y"), (_variant_t) nSpace_y, _T("Space_Y"), ePROP_COMBO_SPACE_Y);
	pGroup->AddSubItem(pProp);

	CMFCPropertyGridPropertyEx* pBack = new CMFCPropertyGridPropertyEx(_T("BackGround"));
	CMFCPropertyGridPropertyEx* pSize = new CMFCPropertyGridPropertyEx(_T("Size"));
	CMFCPropertyGridPropertyEx* pUV0 = new CMFCPropertyGridPropertyEx(_T("이미지"));
	SetRect(ePROP_COMBO_BACK_X, ePROP_COMBO_BACK_Y, ePROP_COMBO_BACK_W, ePROP_COMBO_BACK_H, rcBase, pSize);
	pBack->AddSubItem(pSize);

	SetUV(ePROP_COMBO_BACK_L, ePROP_COMBO_BACK_T, ePROP_COMBO_BACK_R, ePROP_COMBO_BACK_B, uvBase, pUV0);
	pBack->AddSubItem(pUV0);
	pGroup->AddSubItem(pBack);

	CMFCPropertyGridPropertyEx* pButton = new CMFCPropertyGridPropertyEx(_T("Combo Button"));
	pSize = new CMFCPropertyGridPropertyEx(_T("Size"));
	SetRect(ePROP_COMBO_BTN_X, ePROP_COMBO_BTN_Y, ePROP_COMBO_BTN_W, ePROP_COMBO_BTN_H, rcComboBtn, pSize);
	pButton->AddSubItem(pSize);

	pUV0 = new CMFCPropertyGridPropertyEx(_T("이미지1"));
	SetUV(ePROP_COMBO_BTN0_L, ePROP_COMBO_BTN0_T, ePROP_COMBO_BTN0_R, ePROP_COMBO_BTN0_B, uvComboBtn0, pUV0);
	pButton->AddSubItem(pUV0);

	CMFCPropertyGridPropertyEx* pUV1 = new CMFCPropertyGridPropertyEx(_T("이미지2"));
	SetUV(ePROP_COMBO_BTN1_L, ePROP_COMBO_BTN1_T, ePROP_COMBO_BTN1_R, ePROP_COMBO_BTN1_B, uvComboBtn1, pUV1);
	pButton->AddSubItem(pUV1);
	pButton->Expand(FALSE);
	pGroup->AddSubItem(pButton);

	CMFCPropertyGridPropertyEx* pDropBG = new CMFCPropertyGridPropertyEx("Drop List BackGround");
	pSize = new CMFCPropertyGridPropertyEx(_T("Size"));
	SetRect(ePROP_COMBO_DROP_AREA_X, ePROP_COMBO_DROP_AREA_Y, ePROP_COMBO_DROP_AREA_W, ePROP_COMBO_DROP_AREA_H, rcDropArea, pSize);
	pDropBG->AddSubItem(pSize);
	pUV0 = new CMFCPropertyGridPropertyEx(_T("이미지"));
	SetUV(ePROP_COMBO_DROP_AREA_L, ePROP_COMBO_DROP_AREA_T, ePROP_COMBO_DROP_AREA_R, ePROP_COMBO_DROP_AREA_B, uvDropArea, pUV0);
	pDropBG->AddSubItem(pUV0);
	pDropBG->Expand(FALSE);
	pGroup->AddSubItem(pDropBG);

	std::string strImg = pUI->getTexString();
	SetImage( (int)ePROP_COMBO_TEX, strImg.c_str(), pGroup );
	pGroup->Expand(TRUE);
	m_wndPropListEx.AddProperty(pGroup);
}

void CPropertiesWnd::ShowPropSlideBar( CUISlideBar* pUI )
{
	CMFCPropertyGridPropertyEx* pGroup = new CMFCPropertyGridPropertyEx(_T("SlideBar"));

	int nBarW	 = pUI->getBarWidth();
	int nBarH	 = pUI->getBarHeight();
	int nCurPos	 = pUI->GetCurPos();
	int nRange	 = pUI->GetRange();
	int nMin	 = pUI->GetMinPos();
	int nMax	 = pUI->GetMaxPos();
	
	CMFCPropertyGridPropertyEx* pSize = new CMFCPropertyGridPropertyEx(_T("Bar Size"));
	CMFCPropertyGridPropertyEx* pProp = new CMFCPropertyGridPropertyEx(_T("Bar Width"), (_variant_t) nBarW, _T("Bar Width"), ePROP_SLIDEBAR_BAR_W);
	pSize->AddSubItem(pProp);
	pProp = new CMFCPropertyGridPropertyEx(_T("Bar Height"), (_variant_t) nBarW, _T("Bar Height"), ePROP_SLIDEBAR_BAR_H);
	pSize->AddSubItem(pProp);
	pGroup->AddSubItem(pSize);

	pProp = new CMFCPropertyGridPropertyEx(_T("Current Pos"), (_variant_t) nCurPos, _T("Current Pos"), ePROP_SLIDEBAR_CURPOS);
	pGroup->AddSubItem(pProp);

	pProp = new CMFCPropertyGridPropertyEx(_T("Range"), (_variant_t) nRange, _T("Range"), ePROP_SLIDEBAR_RANGE);
	pGroup->AddSubItem(pProp);

	CMFCPropertyGridPropertyEx* pMinMax = new CMFCPropertyGridPropertyEx(_T("Min, Max"));
	pProp = new CMFCPropertyGridPropertyEx(_T("Min Pos"), (_variant_t) nMin, _T("Min Pos"), ePROP_SLIDEBAR_MIN);
	pMinMax->AddSubItem(pProp);
	pProp = new CMFCPropertyGridPropertyEx(_T("Max Pos"), (_variant_t) nMax, _T("Max Pos"), ePROP_SLIDEBAR_MAX);
	pMinMax->AddSubItem(pProp);
	pGroup->AddSubItem(pMinMax);

	UIRectUV uv[2];
	uv[0] = pUI->getBackUV();
	uv[1] = pUI->getBarUV();
	CMFCPropertyGridPropertyEx* pUV = new CMFCPropertyGridPropertyEx(_T("BackGround UV"));
	SetUV((int)ePROP_SLIDEBAR_BACK_L, (int)ePROP_SLIDEBAR_BACK_T, (int)ePROP_SLIDEBAR_BACK_R, (int)ePROP_SLIDEBAR_BACK_B, uv[0], pUV);
	pGroup->AddSubItem(pUV);
	pUV = new CMFCPropertyGridPropertyEx(_T("Bar UV"));
	SetUV((int)ePROP_SLIDEBAR_BAR_L, (int)ePROP_SLIDEBAR_BAR_T, (int)ePROP_SLIDEBAR_BAR_R, (int)ePROP_SLIDEBAR_BAR_B, uv[1], pUV);
	pGroup->AddSubItem(pUV);

	std::string strImg = pUI->getTexString();
	SetImage( (int)ePROP_SLIDEBAR_TEX, strImg.c_str(), pGroup );
	pGroup->Expand(TRUE);
	m_wndPropListEx.AddProperty(pGroup);
}

void CPropertiesWnd::ShowPropProgressBar( CUIProgressBar* pUI )
{
	CMFCPropertyGridPropertyEx* pGroup = new CMFCPropertyGridPropertyEx(_T("ProgressBar"));

	int nLower, nUpper, nStep, nOffsetPos, nBarSX, nBarSY;
	bool bAutoComplete;

	pUI->GetProgressRange(nLower, nUpper);
	nStep			= pUI->getStep();
	nOffsetPos		= pUI->GetProgressPos();
	bAutoComplete	= pUI->getAutoComplete() ? true : false;

	UIRectUV uv;
	uv = pUI->getUVBack();
	CMFCPropertyGridPropertyEx* pUV = new CMFCPropertyGridPropertyEx(_T("BackGround UV"));
	SetUV((int)ePROP_PROGRESS_UV_BACK_L, (int)ePROP_PROGRESS_UV_BACK_T
		, (int)ePROP_PROGRESS_UV_BACK_R, (int)ePROP_PROGRESS_UV_BACK_B, uv, pUV);
	pGroup->AddSubItem(pUV);

	CMFCPropertyGridPropertyEx* pRange = new CMFCPropertyGridPropertyEx(_T("Progress Range"));
	CMFCPropertyGridPropertyEx* pProp = new CMFCPropertyGridPropertyEx(_T("Lower"), (_variant_t) nLower, _T("Lower"), ePROP_PROGRESS_LOWER);
	pRange->AddSubItem(pProp);
	pProp = new CMFCPropertyGridPropertyEx(_T("Upper"), (_variant_t) nUpper, _T("Upper"), ePROP_PROGRESS_UPPER);
	pRange->AddSubItem(pProp);
	pGroup->AddSubItem(pRange);

	pProp = new CMFCPropertyGridPropertyEx(_T("Step"), (_variant_t) nStep, _T("Step"), ePROP_PROGRESS_STEP);
	pGroup->AddSubItem(pProp);

	pProp = new CMFCPropertyGridPropertyEx(_T("OffsetPos"), (_variant_t) nOffsetPos, _T("OffsetPos"), ePROP_PROGRESS_OFFSETPOS);
	pGroup->AddSubItem(pProp);

	pProp = new CMFCPropertyGridPropertyEx(_T("AutoComplete"), (_variant_t) bAutoComplete, _T("AutoComplete"), ePROP_PROGRESS_AUTO_COMPLETE);
	pGroup->AddSubItem(pProp);

	CMFCPropertyGridPropertyEx* pBar = new CMFCPropertyGridPropertyEx(_T("Bar"));
	pUI->getBarStartPos(nBarSX, nBarSY);
	pProp = new CMFCPropertyGridPropertyEx(_T("Bar Start X"), (_variant_t) nBarSX, _T("시작 포지션 X"), ePROP_PROGRESS_BAR_SX);
	pBar->AddSubItem(pProp);

	pProp = new CMFCPropertyGridPropertyEx(_T("Bar Start Y"), (_variant_t) nBarSY, _T("시작 포지션 Y"), ePROP_PROGRESS_BAR_SY);
	pBar->AddSubItem(pProp);
	pGroup->AddSubItem(pBar);

	uv = pUI->getUVBar();
	SetUV((int)ePROP_PROGRESS_UV_BAR_L, (int)ePROP_PROGRESS_UV_BAR_T
		, (int)ePROP_PROGRESS_UV_BAR_R, (int)ePROP_PROGRESS_UV_BAR_B, uv, pBar);



	
	//---------------------------Bar 세팅--------------------------------//
	COLOR		color;
	std::string str;
	int			nStringIdx;
	bool		bShowPercent;
	const char* szDesc[] = { "Bar Text ( Idle )", "Bar Text ( Doing )", "Bar Text ( Complete )" };
	int i;
	for( i = 0; i < PR_STAT_TAG_END; i++ )
	{
		nStringIdx = pUI->getStringIdx((_tagProgressStatus)i);
		CMFCPropertyGridPropertyEx* pBarText = new CMFCPropertyGridPropertyEx(_T(szDesc[i]));
		pProp = new CMFCPropertyGridPropertyEx(_T("String Index"), (_variant_t) nStringIdx, _T("String Index")
			, ePROP_PROGRESS_IDLE_STR_IDX + ( i * 4 ) );
		pBarText->AddSubItem(pProp);

		str	 = pUI->getProgressText((_tagProgressStatus)i);
		SetTextString((ePROP_TYPE)(ePROP_PROGRESS_IDLE_STR + ( i * 4 )), pBarText, str, "String", "Bar Text");

		bShowPercent = pUI->getShowPercentText((_tagProgressStatus)i) ? true : false;
		pProp = new CMFCPropertyGridPropertyEx(_T("Show Percent"),
			(_variant_t) bShowPercent, _T("Show Percent"), ePROP_PROGRESS_IDLE_SHOW_PERCENT + ( i * 4 ));
		pBarText->AddSubItem(pProp);

		color = pUI->getColProgressText((_tagProgressStatus)i);
		CMFCPropertyGridColorProperty* pColorProp = new CMFCPropertyGridColorProperty(_T("Color"),
								 ByteSwap(color), NULL, _T("Color"), ePROP_PROGRESS_IDLE_COLOR + ( i * 4 ));

		pColorProp->EnableOtherButton(_T("기타..."));
		pColorProp->EnableAutomaticButton(_T("기본값"), ::GetSysColor(COLOR_3DFACE));
		pBarText->AddSubItem(pColorProp);

		pGroup->AddSubItem(pBarText);
	}

	std::string strImg = pUI->getTexString();
	SetImage( (int)ePROP_PROGRESS_TEX, strImg.c_str(), pGroup );

	pGroup->Expand(TRUE);
	m_wndPropListEx.AddProperty(pGroup);
}

void CPropertiesWnd::ShowPropList( CUIList* pUI )
{
	int nX, nY, nGap = pUI->GetGap(), nShowCnt = pUI->GetItemShowNum();
	int nItemCount	 = pUI->GetItemCount();
	bool bPreCreate	 = pUI->GetPreCreate();
	pUI->GetItemStart(nX, nY);

	CMFCPropertyGridPropertyEx* pGroup = new CMFCPropertyGridPropertyEx(_T("List"));

	CMFCPropertyGridPropertyEx* pPos = new CMFCPropertyGridPropertyEx(_T("시작 위치"));
	CMFCPropertyGridPropertyEx* pProp = new CMFCPropertyGridPropertyEx(_T("Start X"), (_variant_t) nX, _T("Start X"), ePROP_LIST_START_X);
	pPos->AddSubItem(pProp);
	pProp = new CMFCPropertyGridPropertyEx(_T("Start Y"), (_variant_t) nY, _T("Start Y"), ePROP_LIST_START_Y);
	pPos->AddSubItem(pProp);
	pGroup->AddSubItem(pPos);

	pProp = new CMFCPropertyGridPropertyEx(_T("간격"), (_variant_t) nGap, _T("간격"), ePROP_LIST_GAP);
	pGroup->AddSubItem(pProp);

	pProp = new CMFCPropertyGridPropertyEx(_T("Show Item Count"), (_variant_t) nShowCnt, _T("화면에 보여줄 아이템 갯수"), ePROP_LIST_SHOW_COUNT);
	pGroup->AddSubItem(pProp);

	pProp = new CMFCPropertyGridPropertyEx(_T("Item Count"), (_variant_t) nItemCount, _T("아이템의 갯수"), ePROP_LIST_ITEM_COUNT);
	pGroup->AddSubItem(pProp);

	pProp = new CMFCPropertyGridPropertyEx(_T("PreCreate"), (_variant_t) bPreCreate, _T("PreCreate"), ePROP_LIST_PRE_CREATE);
	pGroup->AddSubItem(pProp);

	
	UIRect rc;
	UIRectUV uv;

	CMFCPropertyGridPropertyEx* pSelect = new CMFCPropertyGridPropertyEx("Select Image");
	pUI->GetEventImg(rc, uv, CUIList::eTYPE_SELECT);

	SetRect(ePROP_LIST_SELECT_RECT_X, ePROP_LIST_SELECT_RECT_Y, ePROP_LIST_SELECT_RECT_W, ePROP_LIST_SELECT_RECT_H, rc, pSelect);
	SetUV(ePROP_LIST_SELECT_UV_L, ePROP_LIST_SELECT_UV_T, ePROP_LIST_SELECT_UV_R, ePROP_LIST_SELECT_UV_B, uv, pSelect);
	pGroup->AddSubItem(pSelect);

	CMFCPropertyGridPropertyEx* pOver = new CMFCPropertyGridPropertyEx("Over Image");
	pUI->GetEventImg(rc, uv, CUIList::eTYPE_OVER);

	SetRect(ePROP_LIST_OVER_RECT_X, ePROP_LIST_OVER_RECT_Y, ePROP_LIST_OVER_RECT_W, ePROP_LIST_OVER_RECT_H, rc, pOver);
	SetUV(ePROP_LIST_OVER_UV_L, ePROP_LIST_OVER_UV_T, ePROP_LIST_OVER_UV_R, ePROP_LIST_OVER_UV_B, uv, pOver);
	pGroup->AddSubItem(pOver);

	std::string strImg = pUI->getTexString();
	SetImage( (int)ePROP_LIST_TEX, strImg.c_str(), pGroup );

	pGroup->Expand(TRUE);
	m_wndPropListEx.AddProperty(pGroup);
}

void CPropertiesWnd::ShowPropScrollBar( CUIScrollBar* pUI )
{
	CMFCPropertyGridPropertyEx* pGroup = new CMFCPropertyGridPropertyEx(_T("Scroll Bar"));
	bool bVertical    = pUI->getVertical() ? true : false;
	int  nScroll_Pos  = pUI->GetScrollPos();
	bool bRight		  = pUI->getRight();
	int  nItemSize	  = pUI->getItemSize();
	int	 nItemPerPage = pUI->GetItemsPerPage();

	CMFCPropertyGridPropertyEx* pProp = new CMFCPropertyGridPropertyEx(_T("Vertical Mode"), (_variant_t) bVertical, _T("vertical or horizon"), ePROP_SB_VERTICAL);
	pGroup->AddSubItem(pProp);

	pProp = new CMFCPropertyGridPropertyEx(_T("Right Mode"), (_variant_t) bRight, _T(""), ePROP_SB_RIGHT);
	pGroup->AddSubItem(pProp);

	pProp = new CMFCPropertyGridPropertyEx(_T("Scroll Position"), (_variant_t) nScroll_Pos, _T(""), ePROP_SB_SCROLL_POS);
	pGroup->AddSubItem(pProp);

	pProp = new CMFCPropertyGridPropertyEx(_T("Move Unit"), (_variant_t) nItemSize, _T(""), ePROP_SB_MOVE_UNIT);
	pGroup->AddSubItem(pProp);

	pProp = new CMFCPropertyGridPropertyEx(_T("Item Per Page"), (_variant_t) nItemPerPage, _T(""), ePROP_SB_ITEM_PER_PAGE);
	pGroup->AddSubItem(pProp);

	UIRectUV uv0, uv1;
	UIRect	 rc;
	pUI->getBackGround(rc, uv0);
	CMFCPropertyGridPropertyEx* pBack = new CMFCPropertyGridPropertyEx(_T("BackGround Image"));
	SetRect(ePROP_SB_BACK_RECT_X, ePROP_SB_BACK_RECT_Y, ePROP_SB_BACK_RECT_W, ePROP_SB_BACK_RECT_H
		, rc, pBack);
	SetUV(ePROP_SB_BACK_UV_L, ePROP_SB_BACK_UV_T, ePROP_SB_BACK_UV_R, ePROP_SB_BACK_UV_B
		, uv0, pBack );
	pGroup->AddSubItem(pBack);

	CMFCPropertyGridPropertyEx* pScrollBtn = new CMFCPropertyGridPropertyEx(_T("Scroll UP Button"));
	pUI->getScrollBtn(0, rc, uv0, uv1);
	SetRect(ePROP_SB_BUTTON_1_RECT_X, ePROP_SB_BUTTON_1_RECT_Y, ePROP_SB_BUTTON_1_RECT_W, ePROP_SB_BUTTON_1_RECT_H
		, rc, pScrollBtn);
	SetUV(ePROP_SB_BUTTON_1_UP_UV_L, ePROP_SB_BUTTON_1_UP_UV_T, ePROP_SB_BUTTON_1_UP_UV_R, ePROP_SB_BUTTON_1_UP_UV_B
		, uv0, pScrollBtn, " UV (Idle) ");
	SetUV(ePROP_SB_BUTTON_1_DOWN_UV_L, ePROP_SB_BUTTON_1_DOWN_UV_T, ePROP_SB_BUTTON_1_DOWN_UV_R, ePROP_SB_BUTTON_1_DOWN_UV_B
		, uv1, pScrollBtn, " UV (Click) ");
	pScrollBtn->Expand(FALSE);
	pGroup->AddSubItem(pScrollBtn);

	pScrollBtn = new CMFCPropertyGridPropertyEx(_T("Scroll DOWN Button"));
	pUI->getScrollBtn(1, rc, uv0, uv1);
	SetRect(ePROP_SB_BUTTON_2_RECT_X, ePROP_SB_BUTTON_2_RECT_Y, ePROP_SB_BUTTON_2_RECT_W, ePROP_SB_BUTTON_2_RECT_H
		, rc, pScrollBtn);
	SetUV(ePROP_SB_BUTTON_2_UP_UV_L, ePROP_SB_BUTTON_2_UP_UV_T, ePROP_SB_BUTTON_2_UP_UV_R, ePROP_SB_BUTTON_2_UP_UV_B
		, uv0, pScrollBtn, " UV (Idle) ");
	SetUV(ePROP_SB_BUTTON_2_DOWN_UV_L, ePROP_SB_BUTTON_2_DOWN_UV_T, ePROP_SB_BUTTON_2_DOWN_UV_R, ePROP_SB_BUTTON_2_DOWN_UV_B
		, uv1, pScrollBtn, " UV (Click) ");
	pScrollBtn->Expand(FALSE);
	pGroup->AddSubItem(pScrollBtn);

	CMFCPropertyGridPropertyEx* pThumb = new CMFCPropertyGridPropertyEx(_T("Thumb"));
	int unit;
	pUI->getThumb(rc, uv0, unit);
	pProp	= new CMFCPropertyGridPropertyEx(_T("Thumb Unit"), (_variant_t) unit, _T(""), ePROP_SB_THUMB_UNIT);
	pThumb->AddSubItem(pProp);
	SetRect(ePROP_SB_THUMB_RECT_X, ePROP_SB_THUMB_RECT_Y, ePROP_SB_THUMB_RECT_W, ePROP_SB_THUMB_RECT_H, rc, pThumb);
	SetUV(ePROP_SB_THUMB_UV_L, ePROP_SB_THUMB_UV_T, ePROP_SB_THUMB_UV_R, ePROP_SB_THUMB_UV_B, uv0, pThumb);
	pThumb->Expand(FALSE);
	pGroup->AddSubItem(pThumb);

	std::string strImg = pUI->getTexString();
	SetImage( (int)ePROP_SB_TEX, strImg.c_str(), pGroup );

	pGroup->Expand(TRUE);
	m_wndPropListEx.AddProperty(pGroup);
}

void CPropertiesWnd::ShowPropSpinButton( CUISpinButton* pUI )
{
	std::string strTitle = pUI->getTitleText();
	UIRectUV uv = pUI->getUVDataBack();
	bool	bAutoInfo	= pUI->getBtnAutoInfo();
	int		nAdjust		= pUI->getAdjust();
	int		nBtnW, nBtnH, nBtnGap;
	int		nDataBackWidth = pUI->getDataBackWidth();
	UIRect	rcWheel		= pUI->getWheelRect();

	pUI->getSpinButton(nBtnW, nBtnH, nBtnGap);

	CMFCPropertyGridPropertyEx* pGroup = new CMFCPropertyGridPropertyEx(_T("SpinButton"));

	CMFCPropertyGridPropertyEx* pProp = new CMFCPropertyGridPropertyEx(_T("Data Back Image"));
	SetUV(ePROP_SPIN_BACK_UV_L, ePROP_SPIN_BACK_UV_T, ePROP_SPIN_BACK_UV_R, ePROP_SPIN_BACK_UV_B, uv, pProp);
	pGroup->AddSubItem(pProp);

	pProp = new CMFCPropertyGridPropertyEx(_T("Title"), (_variant_t) strTitle.c_str(), _T("Title"), ePROP_SPIN_TITLE_STR);
	pGroup->AddSubItem(pProp);

	pProp = new CMFCPropertyGridPropertyEx(_T("Data Back Ground Width"), (_variant_t) nDataBackWidth, _T("뒷 배경 이미지 가로 사이즈."), ePROP_SPIN_DATA_BACK_WIDTH);
	pGroup->AddSubItem(pProp);

	pProp = new CMFCPropertyGridPropertyEx(_T("AutoInfo"), (_variant_t) bAutoInfo, _T("TRUE로 해놓으면 버튼 위치 자동 세팅."), ePROP_SPIN_AUTO_INFO);
	pGroup->AddSubItem(pProp);

	pProp = new CMFCPropertyGridPropertyEx(_T("Spin Button Width"), (_variant_t) nBtnW, _T("Auto Info가 True가 아니라면 해줄 필요 없음."), ePROP_SPIN_BTN_W);
	pGroup->AddSubItem(pProp);

	pProp = new CMFCPropertyGridPropertyEx(_T("Spin Button Height"), (_variant_t) nBtnH, _T("Auto Info가 True가 아니라면 해줄 필요 없음."), ePROP_SPIN_BTN_H);
	pGroup->AddSubItem(pProp);

	pProp = new CMFCPropertyGridPropertyEx(_T("Spin Button Gap"), (_variant_t) nBtnGap, _T("Auto Info가 True가 아니라면 해줄 필요 없음."), ePROP_SPIN_BTN_GAP);
	pGroup->AddSubItem(pProp);

	CMFCPropertyGridPropertyEx* pChild	= new CMFCPropertyGridPropertyEx(_T("Mouse Wheel Position"));
	SetUV(ePROP_SPIN_WHEEL_L, ePROP_SPIN_WHEEL_T, ePROP_SPIN_WHEEL_R, ePROP_SPIN_WHEEL_B
	, UIRectUV(rcWheel.Left, rcWheel.Top, rcWheel.Right, rcWheel.Bottom, 1.0f, 1.0f), pChild);
	pGroup->AddSubItem(pChild);

	std::string strImg = pUI->getTexString();
	SetImage( (int)ePROP_SPIN_TEX, strImg.c_str(), pGroup );

	pGroup->Expand(TRUE);
	m_wndPropListEx.AddProperty(pGroup);
}

void CPropertiesWnd::ShowPropIcon( CUIIcon* pUI )
{
	int			nRow, nCol, nType;
	SBYTE		sbTexID;

	pUI->getInfo(nType, sbTexID, nRow, nCol);

	CMFCPropertyGridPropertyEx* pGroup = new CMFCPropertyGridPropertyEx(_T("Icon"));

	CMFCPropertyGridPropertyEx* pProp = new CMFCPropertyGridPropertyEx(_T("Button Type"), (_variant_t) nType, _T(""), ePROP_ICON_TYPE);
	pGroup->AddSubItem(pProp);

	pProp = new CMFCPropertyGridPropertyEx(_T("Tex ID"), (_variant_t) (int)sbTexID, _T(""), ePROP_ICON_TEX_ID);
	pGroup->AddSubItem(pProp);

	pProp = new CMFCPropertyGridPropertyEx(_T("Row"), (_variant_t) nRow, _T(""), ePROP_ICON_ROW);
	pGroup->AddSubItem(pProp);

	pProp = new CMFCPropertyGridPropertyEx(_T("Col"), (_variant_t) nCol, _T(""), ePROP_ICON_COL);
	pGroup->AddSubItem(pProp);

	pGroup->Expand(TRUE);
	m_wndPropListEx.AddProperty(pGroup);
}

void CPropertiesWnd::ShowPropArray( CUIArray* pUI )
{
	int			row, col, sx, sy, gapx, gapy, nShowCnt;
	
	pUI->GetRowCol(row, col);
	pUI->GetStartItem(sx, sy);
	pUI->GetGap(gapx, gapy);
	nShowCnt = pUI->GetShowRowCount();	

	CMFCPropertyGridPropertyEx* pGroup = new CMFCPropertyGridPropertyEx(_T("Array"));

	CMFCPropertyGridPropertyEx* pProp = new CMFCPropertyGridPropertyEx(_T("Row"), (_variant_t) row, _T("가로 갯수"), ePROP_ARRAY_ROW);
	pGroup->AddSubItem(pProp);

	pProp = new CMFCPropertyGridPropertyEx(_T("Col"), (_variant_t) col, _T("세로 갯수"), ePROP_ARRAY_COL);
	pGroup->AddSubItem(pProp);

	pProp = new CMFCPropertyGridPropertyEx(_T("Start Pos X"), (_variant_t) sx, _T("시작 위치 X"), ePROP_ARRAY_SX);
	pGroup->AddSubItem(pProp);

	pProp = new CMFCPropertyGridPropertyEx(_T("Start Pos Y"), (_variant_t) sy, _T("시작 위치 Y"), ePROP_ARRAY_SY);
	pGroup->AddSubItem(pProp);

	pProp = new CMFCPropertyGridPropertyEx(_T("Gap X"), (_variant_t) gapx, _T("가로 Gap"), ePROP_ARRAY_GAPX);
	pGroup->AddSubItem(pProp);

	pProp = new CMFCPropertyGridPropertyEx(_T("Gap Y"), (_variant_t) gapy, _T("세로 Gap"), ePROP_ARRAY_GAPY);
	pGroup->AddSubItem(pProp);

	pProp = new CMFCPropertyGridPropertyEx(_T("Show Item Row"), (_variant_t) nShowCnt, _T("화면에 세로 갯수"), ePROP_ARRAY_SHOW_ROW);
	pGroup->AddSubItem(pProp);

	UIRect rc;
	UIRectUV uv;
	pUI->GetMouseEventImage(rc, uv, 0);
	CMFCPropertyGridPropertyEx* pSelect = new CMFCPropertyGridPropertyEx(_T("Select Image"));
	SetRect(ePROP_ARRAY_IMG_SELECT_RECT_X, ePROP_ARRAY_IMG_SELECT_RECT_Y, ePROP_ARRAY_IMG_SELECT_RECT_W, ePROP_ARRAY_IMG_SELECT_RECT_H
		, rc, pSelect );
	SetUV(ePROP_ARRAY_IMG_SELECT_UV_L, ePROP_ARRAY_IMG_SELECT_UV_T, ePROP_ARRAY_IMG_SELECT_UV_R, ePROP_ARRAY_IMG_SELECT_UV_B
		, uv, pSelect );
	pGroup->AddSubItem(pSelect);

	pUI->GetMouseEventImage(rc, uv, 1);
	CMFCPropertyGridPropertyEx* pOver = new CMFCPropertyGridPropertyEx(_T("Mouse Over"));
	SetRect(ePROP_ARRAY_IMG_OVER_RECT_X, ePROP_ARRAY_IMG_OVER_RECT_Y, ePROP_ARRAY_IMG_OVER_RECT_W, ePROP_ARRAY_IMG_OVER_RECT_H
		, rc, pOver );
	SetUV(ePROP_ARRAY_IMG_OVER_UV_L, ePROP_ARRAY_IMG_OVER_UV_T, ePROP_ARRAY_IMG_OVER_UV_R, ePROP_ARRAY_IMG_OVER_UV_B
		, uv, pOver );
	pGroup->AddSubItem(pOver);

	std::string strImg = pUI->getTexString();
	SetImage(ePROP_ARRAY_IMG, strImg.c_str(), pGroup);

	pGroup->Expand(TRUE);
	m_wndPropListEx.AddProperty(pGroup);
}

void CPropertiesWnd::ShowPropTextBox( CUITextBox* pUI )
{
	int nSX, nSY;
	pUI->GetStartXY(nSX, nSY);
	int nH_Align = pUI->GetAlignTextH();
	int strIdx = pUI->getStringIdx();
	int nGapY	= pUI->GetGapY();
	int nSplit	= pUI->GetSplitMode();
	std::string str = pUI->GetString();
	COLOR color = pUI->GetTextColor();

	UIRect rc;
	UIRectUV uv;
	pUI->getBackGround(rc, uv);

	CMFCPropertyGridPropertyEx* pGroup = new CMFCPropertyGridPropertyEx(_T("TextBox"));
	CMFCPropertyGridPropertyEx* pSpace = new CMFCPropertyGridPropertyEx(_T("Start Pos"));
	CMFCPropertyGridPropertyEx* pProp = new CMFCPropertyGridPropertyEx(_T("Start X"), (_variant_t) nSX, _T(""), ePROP_TB_START_X);
	pProp->EnableSpinControl(TRUE, -3000, 3000);
	pSpace->AddSubItem(pProp);

	pProp = new CMFCPropertyGridPropertyEx(_T("Start Y"), (_variant_t) nSY, _T(""), ePROP_TB_START_Y);
	pProp->EnableSpinControl(TRUE, -3000, 3000);
	pSpace->AddSubItem(pProp);
	pGroup->AddSubItem(pSpace);

	pProp = new CMFCPropertyGridPropertyEx(_T("Gap Y"), (_variant_t)nGapY, _T(""), ePROP_TB_GAP_Y);
	pProp->EnableSpinControl(TRUE, -3000, 3000);
	pGroup->AddSubItem(pProp);

	pProp = new CMFCPropertyGridPropertyEx(_T("Align"), (_variant_t) szAlign_H[nH_Align], _T(""), ePROP_TB_ALIGN);
	for(int i = 0; i < 3; i++)
		pProp->AddOption(_T(szAlign_H[i]));
	pProp->AllowEdit(FALSE);
	pGroup->AddSubItem(pProp);

	pProp = new CMFCPropertyGridPropertyEx(_T("SplitMode"), (_variant_t) szSplit[nSplit], _T(""), ePROP_TB_SPLIT);
	for(int i = 0; i < 6; i++)
		pProp->AddOption(_T(szSplit[i]));
	pProp->AllowEdit(FALSE);
	pGroup->AddSubItem(pProp);

	pProp = new CMFCPropertyGridPropertyEx(_T("Text Idx"), (_variant_t) strIdx, _T(""), ePROP_TB_STR_IDX);
	pGroup->AddSubItem(pProp);

	pProp = new CMFCPropertyGridPropertyEx(_T("Text"), (_variant_t) str.c_str(), _T(""), ePROP_TB_STR);
	pGroup->AddSubItem(pProp);

	CMFCPropertyGridColorProperty* pColorProp = new CMFCPropertyGridColorProperty(_T("Color"),ByteSwap(pUI->GetTextColor()), NULL, _T("Color"), ePROP_TB_STR_COLOR);
	pColorProp->EnableOtherButton(_T("기타..."));
	pColorProp->EnableAutomaticButton(_T("기본값"), ::GetSysColor(COLOR_3DFACE));
	pGroup->AddSubItem(pColorProp);

	CMFCPropertyGridPropertyEx* pBack = new CMFCPropertyGridPropertyEx(_T("Back Ground"));
	SetRect(ePROP_TB_BACK_RECT_X, ePROP_TB_BACK_RECT_Y, ePROP_TB_BACK_RECT_W, ePROP_TB_BACK_RECT_H, rc, pBack);
	SetUV(ePROP_TB_BACK_UV_L, ePROP_TB_BACK_UV_T, ePROP_TB_BACK_UV_R, ePROP_TB_BACK_UV_B, uv, pBack);
	pGroup->AddSubItem(pBack);

	std::string strImg = pUI->getTexString();
	SetImage( (int)ePROP_TB_TEX, strImg.c_str(), pGroup );

	pGroup->Expand(TRUE);
	m_wndPropListEx.AddProperty(pGroup);
}

void CPropertiesWnd::ShowPropImageFont( CUIImageFont* pUI )
{
	int sx, sy, gap, ogap;
	pUI->getFontSizeGap(sx, sy, gap);
	ogap = pUI->getOutputGap();
	std::string strOString = pUI->getOrigString();
	std::string strString  = pUI->getString();
	UIRectUV uv			   = pUI->getSourceImageUV();

	CMFCPropertyGridPropertyEx* pGroup = new CMFCPropertyGridPropertyEx(_T("Image Font"));
	CMFCPropertyGridPropertyEx* pProp = new CMFCPropertyGridPropertyEx(_T("Font Size X"), (_variant_t) sx, _T(""), ePROP_IMGFONT_FSIZE_X);
	pGroup->AddSubItem(pProp);

	pProp = new CMFCPropertyGridPropertyEx(_T("Font Size Y"), (_variant_t) sy, _T(""), ePROP_IMGFONT_FSIZE_Y);
	pGroup->AddSubItem(pProp);

	pProp = new CMFCPropertyGridPropertyEx(_T("Gap"), (_variant_t) gap, _T(""), ePROP_IMGFONT_FGAP);
	pGroup->AddSubItem(pProp);

	pProp = new CMFCPropertyGridPropertyEx(_T("Output Gap"), (_variant_t) ogap, _T(""), ePROP_IMGFONT_OGAP);
	pGroup->AddSubItem(pProp);

	pProp = new CMFCPropertyGridPropertyEx(_T("String"), (_variant_t) strString.c_str(), _T(""), ePROP_IMGFONT_STR);
	pGroup->AddSubItem(pProp);

	pProp = new CMFCPropertyGridPropertyEx(_T("Original String"), (_variant_t) strOString.c_str(), _T(""), ePROP_IMGFONT_OSTR);
	pGroup->AddSubItem(pProp);

	CMFCPropertyGridPropertyEx* pImg = new CMFCPropertyGridPropertyEx(_T("Source Image"));
	SetUV(ePROP_IMGFONT_SRC_UV_L, ePROP_IMGFONT_SRC_UV_T, ePROP_IMGFONT_SRC_UV_R, ePROP_IMGFONT_SRC_UV_B, uv,  pImg);
	pGroup->AddSubItem(pImg);

	std::string strImg = pUI->getTexString();
	SetImage( (int)ePROP_IMGFONT_SRC_TEX, strImg.c_str(), pGroup );

	pGroup->Expand(TRUE);
	m_wndPropListEx.AddProperty(pGroup);
}

void CPropertiesWnd::ShowPropSpriteAni( CUISpriteAni* pUI )
{
	m_nSpriteAniOldCount = pUI->GetAniCount();

	CMFCPropertyGridPropertyEx* pGroup = new CMFCPropertyGridPropertyEx(_T("Sprite Ani"));

	UIRectUV uv;

	int nCurIdx = pUI->GetRenderIdx();

	CMFCPropertyGridPropertyEx* pProp = new CMFCPropertyGridPropertyEx(_T("Render Idx"), (_variant_t) nCurIdx, _T(""), ePROP_SPRITE_CUR_IDX);
	pGroup->AddSubItem(pProp);

	std::string strImage = pUI->getTexString();
	SetImage( (int)ePROP_SPRITE_TEX, strImage, pGroup );

	int i, max = pUI->GetAniCount();

	pProp = new CMFCPropertyGridPropertyEx( _T("Count"), (_variant_t) max, _T("추가할 이미지 개수"), ePROP_SPRITE_ADD);
	pProp->EnableSpinControl(TRUE, 0, 1000);
	pGroup->AddSubItem(pProp);

	pProp = new CMFCPropertyGridPropertyEx( _T("Frame Loop"), (_variant_t) pUI->GetLoopState(), _T("애니메이션 스타트 & 스탑"), ePROP_SPRITE_LOOP);
	pGroup->AddSubItem(pProp);

	for (i = 0; i < max; ++i)
	{
		CMFCPropertyGridPropertyEx* pSurface = new CMFCPropertyGridPropertyEx(_T("Surface"));

		pUI->DataAt(i, uv);

		SetUV( (int)ePROP_SPRITE_UV_L+ (i*4), (int)ePROP_SPRITE_UV_T+ (i*4)
		, (int)ePROP_SPRITE_UV_R + (i*4), (int)ePROP_SPRITE_UV_B + (i*4)
		, uv, pSurface );

		if (i < max - 1)
			pSurface->Expand(FALSE);
		else
			pSurface->Expand(TRUE);

		pGroup->AddSubItem(pSurface);
	}

	pGroup->Expand(TRUE);
	m_wndPropListEx.AddProperty(pGroup);
}

void CPropertiesWnd::ShowPropImageArray( CUIImageArray* pUI )
{
	m_nImageArrayOldCount = pUI->GetImageCount();

	CMFCPropertyGridPropertyEx* pGroup = new CMFCPropertyGridPropertyEx(_T("이미지 배열"));

	UIRectUV uv;
	UIRect	 rc;
	std::string desc;

	int nCurIdx = pUI->GetRenderIdx();

	CMFCPropertyGridPropertyEx* pProp = new CMFCPropertyGridPropertyEx(_T("Render Idx"), (_variant_t) nCurIdx, _T(""), ePROP_IMAGE_ARRAY_CUR_IDX);
	pProp->EnableSpinControl(TRUE, 0, 1000);
	pGroup->AddSubItem(pProp);

	std::string strImage = pUI->getTexString();
	SetImage( (int)ePROP_IMAGE_ARRAY_TEX, strImage, pGroup );

	int i, max = pUI->GetImageCount();

	pProp = new CMFCPropertyGridPropertyEx( _T("Count"), (_variant_t) max, _T("추가할 이미지 개수"), ePROP_IMAGE_ARRAY_ADD);
	pProp->EnableSpinControl(TRUE, 0, 1000);
	pGroup->AddSubItem(pProp);

	for (i = 0; i < max; ++i)
	{
		CMFCPropertyGridPropertyEx* pSurface = new CMFCPropertyGridPropertyEx(_T("Surface"));
		pUI->DataAt(i, rc, uv, desc);

		SetRect(ePROP_IMAGE_ARRAY_RECT_X + (i*4), ePROP_IMAGE_ARRAY_RECT_Y + (i*4), ePROP_IMAGE_ARRAY_RECT_W + (i*4), ePROP_IMAGE_ARRAY_RECT_H + (i*4), rc, pSurface);\

			SetUV( (int)ePROP_IMAGE_ARRAY_UV_L + (i*4), (int)ePROP_IMAGE_ARRAY_UV_T + (i*4)
			, (int)ePROP_IMAGE_ARRAY_UV_R + (i*4), (int)ePROP_IMAGE_ARRAY_UV_B + (i*4)
			, uv, pSurface );

		CMFCPropertyGridPropertyEx* pDesc = new CMFCPropertyGridPropertyEx(_T("Desc"), (_variant_t) desc.c_str(), _T(""), ePROP_IMAGE_ARRAY_DESC+i);
		pSurface->AddSubItem(pDesc);
		if (i < max - 1)
			pSurface->Expand(FALSE);
		else
			pSurface->Expand(TRUE);

		pGroup->AddSubItem(pSurface);
	}

	pGroup->Expand(TRUE);
	m_wndPropListEx.AddProperty(pGroup);
}

void CPropertiesWnd::ShowPropTree( CUITree* pUI )
{

}

void CPropertiesWnd::ShowPropTextBoxEx( CUITextBoxEx* pUI )
{
	int nSX = pUI->GetStartX();
	int nSY = pUI->GetStartY();
	int nGapY	= pUI->GetGapY();
	int nSplit	= pUI->GetSplitMode();
	std::string str = pUI->GetText();
	COLOR color = pUI->GetBaseColor();

	CMFCPropertyGridPropertyEx* pGroup = new CMFCPropertyGridPropertyEx(_T("TextBoxEx"));
	CMFCPropertyGridPropertyEx* pSpace = new CMFCPropertyGridPropertyEx(_T("Start Pos"));
	CMFCPropertyGridPropertyEx* pProp = new CMFCPropertyGridPropertyEx(_T("Start X"), (_variant_t) nSX, _T(""), ePROP_TB_EX_SX);
	pProp->EnableSpinControl(TRUE, -3000, 3000);
	pSpace->AddSubItem(pProp);

	pProp = new CMFCPropertyGridPropertyEx(_T("Start Y"), (_variant_t) nSY, _T(""), ePROP_TB_EX_SY);
	pProp->EnableSpinControl(TRUE, -3000, 3000);
	pSpace->AddSubItem(pProp);
	pGroup->AddSubItem(pSpace);

	pProp = new CMFCPropertyGridPropertyEx(_T("Gap Y"), (_variant_t)nGapY, _T(""), ePROP_TB_EX_GAP_Y);
	pProp->EnableSpinControl(TRUE, -3000, 3000);
	pGroup->AddSubItem(pProp);

	pProp = new CMFCPropertyGridPropertyEx(_T("SplitMode"), (_variant_t) szSplit[nSplit], _T(""), ePROP_TB_EX_SPLIT_MODE);
	for(int i = 0; i < 6; i++)
		pProp->AddOption(_T(szSplit[i]));
	pProp->AllowEdit(FALSE);
	pGroup->AddSubItem(pProp);

	CMFCPropertyGridColorProperty* pColorProp = new CMFCPropertyGridColorProperty(_T("Color"),ByteSwap(pUI->GetBaseColor()), NULL, _T("Color"), ePROP_TB_EX_COL);
	pColorProp->EnableOtherButton(_T("기타..."));
	pColorProp->EnableAutomaticButton(_T("기본값"), ::GetSysColor(COLOR_3DFACE));
	pGroup->AddSubItem(pColorProp);

	pProp = new CMFCPropertyGridPropertyEx(_T("Text"), (_variant_t) str.c_str(), _T(""), ePROP_TB_EX_STR);
	pGroup->AddSubItem(pProp);

	m_wndPropListEx.AddProperty(pGroup);
}

LRESULT CPropertiesWnd::OnPropertyChanged( WPARAM wparam, LPARAM lparam )
{
	CMFCPropertyGridPropertyEx* pProp = ( CMFCPropertyGridPropertyEx* ) lparam;
	ePROP_TYPE eType = (ePROP_TYPE)pProp->GetData();

	CMainFrame* pMain = (CMainFrame *)AfxGetMainWnd();
	if( m_pSelectedUI == NULL )
	{
		pMain->Message("OnPropertyChanged(...) : 해당 컨트롤의 Pointer가 없습니다.");
		return TRUE;
	}

	static ePROP_TYPE eOldType = ePROP_TYPE_NONE;

	// 값이 변경 되기 전에 저장.
	if (eOldType != eType)
	{
		eOldType = eType;

		pMain->OnSaveFile("auto_save");
	}

	switch( eType )
	{
	case ePROP_TYPE_ID:
		{
			std::string strID = (std::string)pProp->FormatProperty();
			m_pSelectedUI->setID( strID.c_str() );
		}
		break;
	case ePROP_TYPE_PID:
		{
			std::string strPID = (std::string)pProp->FormatProperty();
			m_pSelectedUI->setPID( strPID.c_str() );
		}
	case ePROP_TYPE_HIDE:
		m_pSelectedUI->Hide( pProp->GetValue().boolVal ? TRUE : FALSE);
		break;
	case ePROP_TYPE_X:		
		m_pSelectedUI->SetPosX( pProp->GetValue().intVal );
		m_pSelectedUI->updatePosition(true);
		break;
	case ePROP_TYPE_Y:
		m_pSelectedUI->SetPosY( pProp->GetValue().intVal );
		m_pSelectedUI->updatePosition(true);
		break;
	case ePROP_TYPE_W:
		m_pSelectedUI->SetWidth( pProp->GetValue().intVal );
		m_pSelectedUI->updatePosition(true);
		break;
	case ePROP_TYPE_H:
		m_pSelectedUI->SetHeight( pProp->GetValue().intVal );
		m_pSelectedUI->updatePosition(true);
		break;
	case ePROP_TYPE_ALIGN_H:
		{
			int nAlign;
			std::string strTmp = pProp->FormatProperty();
			for( nAlign = 0; nAlign < 3; nAlign++ )
			{
				if( strcmpi(szAlign_H[nAlign], strTmp.c_str()) == 0 )
					m_pSelectedUI->setAlignH((eALIGN_H)nAlign);
			}

			m_pSelectedUI->updatePosition(true);
		}
		break;
	case ePROP_TYPE_ALIGN_V:
		{
			int nAlign;
			std::string strTmp = pProp->FormatProperty();
			for( nAlign = 0; nAlign < 3; nAlign++ )
			{
				if( strcmpi(szAlign_V[nAlign], strTmp.c_str()) == 0 )
					m_pSelectedUI->setAlignV((eALIGN_V)nAlign);
			}

			m_pSelectedUI->updatePosition(true);
		}
		break;
	case ePROP_TYPE_TOOLTIP:
		{
			int nIdx = pProp->GetValue().intVal;
			m_pSelectedUI->SetTooltipIndex(nIdx);

			if (nIdx > 0)
				m_pSelectedUI->setTooltip( CUIBase::getText( (INDEX)nIdx) );
		}
		break;
	case ePROP_TYPE_TOOLTIP_WIDTH:
		{
			int nWidth = pProp->GetValue().intVal;
			m_pSelectedUI->setTooltipWidth(nWidth);
		}
		break;
	}

	switch( m_pSelectedUI->getType() )
	{
	case eUI_CONTROL_IMAGE:
		ChangeImageControlProp( pProp );
		break;
	case eUI_CONTROL_BUTTON:
		ChangeButtonControlProp( lparam );
		break;
	case eUI_CONTROL_TEXT:
		ChangeTextControlProp( lparam );
		break;
	case eUI_CONTROL_TEXTBOX:
		ChangeTextBoxControlProp( lparam );
		break;
	case eUI_CONTROL_TEXTBOX_EX:
		ChangeTextBoxExControlProp( lparam );
		break;
	case eUI_CONTROL_EDIT:
		ChangeEditControlProp( pProp );
		break;
	case eUI_CONTROL_SCROLL:
		ChangeScrollBarControlProp( pProp );
		break;
	case eUI_CONTROL_CHECKBOX:
		ChangeCheckControlProp( lparam );
		break;
	case eUI_CONTROL_COMBOBOX:
		ChangeComboBoxControlProp( pProp );
		break;
	case eUI_CONTROL_SLIDEBAR:
		ChangeSlideBarControlProp( pProp );
		break;
	case eUI_CONTROL_PROGRESSBAR:
		ChangeProgressBarControlProp( lparam );
		break;
	case eUI_CONTROL_LIST:
		ChangeListControlProp( pProp );
		break;
	case eUI_CONTROL_SPINBUTTON:
		ChangeSpinButtonControlProp( pProp );
		break;
	case eUI_CONTROL_ICON:
		ChangeIconControlProp( pProp );
		break;
	case eUI_CONTROL_ARRAY:
		ChangeArrayControlProp( pProp );
		break;
	case eUI_CONTROL_IMAGEFONT:
		ChangeImageFontControlProp( pProp );
		break;
	case eUI_CONTROL_SPRITEANI:
		ChangeSpriteAniControlProp( pProp );
		break;
	case eUI_CONTROL_IMAGE_ARRAY:
		ChangeImageArrayControlProp( pProp );
		break;
	case eUI_CONTROL_IMAGE_SPLIT:
		ChangeImageSplitControlProp( pProp );
		break;
	case eUI_CONTROL_SPIN_CONTROL:
		ChangeSpinControlProp( pProp );
		break;
	case eUI_CONTROL_TREE:
		ChangeTreeControlProp( pProp );
		break;
	}

	Refresh();

	return TRUE;
}

void CPropertiesWnd::ChangeImageControlProp( CMFCPropertyGridProperty* p )
{
	ePROP_TYPE eType = (ePROP_TYPE)p->GetData();
	CUIImage* pUI = dynamic_cast<CUIImage*>(m_pSelectedUI);	
	UIRectUV uv = pUI->GetAbsUV();
	switch( eType )
	{
	case ePROP_IMG_TYPE_L:	case ePROP_IMG_TYPE_T:	case ePROP_IMG_TYPE_R:	case ePROP_IMG_TYPE_B:
		{
			std::string strVal = p->FormatProperty();
			ExtractionUV(eType, ePROP_IMG_TYPE_L, uv, strVal);
			pUI->SetUV( uv );

			p->SetValue(strVal.c_str());
		}
		break;
	case ePROP_IMG_TYPE_TEX:
		{
			std::string strFullPath = p->FormatProperty();
			std::string strFileName = ExtracFileName((char*)strFullPath.c_str());
			CmdChangeTexString* pCmd = new CmdChangeTexString();
			pCmd->setData((CUIBase*)pUI, (LPSTR)(LPCSTR)strFileName.c_str());
			theApp.addQueue(pCmd);
			p->SetValue(strFileName.c_str());
			pUI->SetUV( uv );
		}
		break;
	}
}

void CPropertiesWnd::ChangeButtonControlProp( LPARAM lparam )
{
	CMFCPropertyGridPropertyEx* p = ( CMFCPropertyGridPropertyEx* ) lparam;

	ePROP_TYPE eType = (ePROP_TYPE)p->GetData();
	CUIButton* pUI = dynamic_cast<CUIButton*>(m_pSelectedUI);	

	switch( eType )
	{
	case ePROP_BTN_STR_IDX:
		{
			int nIdx = p->GetValue().intVal;
			pUI->setStringIdx((INDEX)nIdx);
		}
		break;
	case ePROP_BTN_STR:
		{
			CTString str = p->FormatProperty();
			pUI->SetText(str);
		}
		break;
	case ePROP_BTN_TEX:
		{
			int i;
			UIRectUV uv[UCBS_TOTAL];
			for( i = 0; i < UCBS_TOTAL; i++ )
				uv[i] = pUI->GetUV((UIBtnState)i);

			std::string strFullPath = p->FormatProperty();
			std::string strFileName = ExtracFileName((char*)strFullPath.c_str());
			CmdChangeTexString* pCmd = new CmdChangeTexString();
			pCmd->setData((CUIBase*)pUI, (LPSTR)(LPCSTR)strFileName.c_str());
			theApp.addQueue(pCmd);
			p->SetValue(strFileName.c_str());

			for( i = 0; i < UCBS_TOTAL; i++ )
				pUI->SetUV((UIBtnState)i, uv[i]);
		}
		break;
	case ePROP_BTN_EDGE:
		{
			pUI->SetEdge( p->GetValue().boolVal ? true : false );
		}
		break;
	case ePROP_BTN_TEXT_COLOR:
		{
			CMFCPropertyGridColorProperty* pColor = (CMFCPropertyGridColorProperty*) lparam;
			COLORREF col = pColor->GetColor();
			int i;

			for (i = 0 ; i < UBS_TOTAL; ++i)
				pUI->SetTextColor((UIBtnState)i, ConvertToColor(col));
		}
		break;
	case ePROP_BTN_IDLE_L: 		case ePROP_BTN_IDLE_T:		case ePROP_BTN_IDLE_R:		case ePROP_BTN_IDLE_B:
	case ePROP_BTN_ON_L:		case ePROP_BTN_ON_T:		case ePROP_BTN_ON_R:		case ePROP_BTN_ON_B:
	case ePROP_BTN_CLICK_L:		case ePROP_BTN_CLICK_T:		case ePROP_BTN_CLICK_R:		case ePROP_BTN_CLICK_B:
	case ePROP_BTN_DISABLE_L:	case ePROP_BTN_DISABLE_T:	case ePROP_BTN_DISABLE_R:	case ePROP_BTN_DISABLE_B:
		{
			if (pUI->getTexWidth() == 0)
			{
				MessageBox("텍스쳐를 먼저 설정하세요", "Error", MB_OK);
				p->SetValue("0");
				return;
			}

			std::string strVal = p->FormatProperty();
			bool bRet = SetBtnUV( eType, pUI, strVal );

			p->SetValue(strVal.c_str());
			ASSERT(bRet);
		}
		break;	
	}
}

void CPropertiesWnd::ChangeTextControlProp( LPARAM lparam )
{
	CMFCPropertyGridPropertyEx* pProp = ( CMFCPropertyGridPropertyEx* ) lparam;
	ePROP_TYPE eType = (ePROP_TYPE)pProp->GetData();
	CUIText* pUI = dynamic_cast<CUIText*>(m_pSelectedUI);	

	switch( eType )
	{
	case ePROP_TEXT_TEX:
		{
			std::string strFullPath = pProp->FormatProperty();
			std::string strFileName = ExtracFileName((char*)strFullPath.c_str());
			CmdChangeTexString* pCmd = new CmdChangeTexString();
			pCmd->setData((CUIBase*)pUI, (LPSTR)(LPCSTR)strFileName.c_str());
			theApp.addQueue(pCmd);
			pProp->SetValue(strFileName.c_str());
		}
		break;
	case ePROP_TEXT_STR_IDX:
		{
			int nIdx = pProp->GetValue().intVal;
			pUI->setStringIdx((INDEX)nIdx);
		}
		break;
	case ePROP_TEXT_STR: case ePROP_TEXT_STR_ELLIPSIS:
		{
			CTString str = pProp->FormatProperty();
			CTString strTemp;

			if (eType == ePROP_TEXT_STR)
				pUI->SetText(str, pUI->getEllipsisText());
			else
				pUI->SetText((CTString)pUI->getText(), str);
		}
		break;
	case ePROP_TEXT_COLOR:
		{
			CMFCPropertyGridColorProperty* pColor = (CMFCPropertyGridColorProperty*) lparam;
			COLORREF col = pColor->GetColor();
			pUI->setFontColor(ConvertToColor(col));
			//pUI->setFontColor(ConvertToColor(col));
		}
		break;
	case ePROP_TEXT_EDGE:
		{
			pUI->setEdge( pProp->GetValue().boolVal );
		}
		break;
	case ePROP_TEXT_SHADOW_COLOR: case ePROP_TEXT_SHADOW:
		{
			BOOL bShadow = pUI->getShadow() ? TRUE : FALSE;
			COLORREF col = pUI->getFontShadow();

			CMFCPropertyGridColorProperty* pColor = (CMFCPropertyGridColorProperty*) lparam;
			if( eType == ePROP_TEXT_SHADOW )
				bShadow = pProp->GetValue().boolVal;
			else
				col = ConvertToColor(pColor->GetColor());

			pUI->setFontShadow( col, bShadow );
		}
		break;
	case ePROP_TEXT_ALIGN:
		{
			int nAlign;
			std::string strTmp = pProp->FormatProperty();
			for( nAlign = 0; nAlign < 3; nAlign++ )
			{
				if( strcmpi(szAlign_H[nAlign], strTmp.c_str()) == 0 )
					pUI->setAlignTextH((eALIGN_H)nAlign);
			}
		}
		break;
	case ePROP_TEXT_BACK_RECT_X: case ePROP_TEXT_BACK_RECT_Y:	case ePROP_TEXT_BACK_RECT_W:	case ePROP_TEXT_BACK_RECT_H:
	case ePROP_TEXT_BACK_UV_L:	 case ePROP_TEXT_BACK_UV_T:		case ePROP_TEXT_BACK_UV_R:		case ePROP_TEXT_BACK_UV_B:
		{
			UIRectUV uv;
			UIRect	 rc;
			pUI->getBackGround(rc, uv);

			if( eType >= ePROP_TEXT_BACK_RECT_X && eType <= ePROP_TEXT_BACK_RECT_H )
			{
				int	nVal = pProp->GetValue().intVal;
				ExtractionRect(eType, ePROP_TEXT_BACK_RECT_X, rc, nVal);
			}
			else
			{
				std::string strVal = pProp->FormatProperty();
				ExtractionUV(eType, ePROP_TEXT_BACK_UV_L, uv, strVal);
				pProp->SetValue(strVal.c_str());
			}
			pUI->setBackGround(rc, uv);
		}
		break;
// 	case ePROP_TEXT_DEPTH:
// 		{
// 			float fDepth = pProp->GetValue().fltVal;
// 			pUI->setDepth(fDepth);
// 		}
// 		break;
	}
}

void CPropertiesWnd::ChangeEditControlProp( CMFCPropertyGridProperty* p )
{
	ePROP_TYPE eType = (ePROP_TYPE)p->GetData();
	CUIEditBox* pUI = dynamic_cast<CUIEditBox*>(m_pSelectedUI);	

	float fTexW = 1.f;
	float fTexH = 1.f;

	if (pUI->getTexString() != NULL)
	{
		fTexW = pUI->getTexWidth();
		fTexH = pUI->getTexHeight();
	}

	switch( eType )
	{
	case ePROP_EDIT_TEX:
		{
			std::string strFullPath = p->FormatProperty();
			std::string strFileName = ExtracFileName((char*)strFullPath.c_str());
			CmdChangeTexString* pCmd = new CmdChangeTexString();
			pCmd->setData((CUIBase*)pUI, (LPSTR)(LPCSTR)strFileName.c_str());
			theApp.addQueue(pCmd);
			p->SetValue(strFileName.c_str());
			//pUI->SetUV( uv );
		}
		break;
	case ePROP_EDIT_MAX_CHAR:
		{
			int nMaxChar = p->GetValue().intVal;
			pUI->SetMaxChar(nMaxChar);
		}
		break;
	case ePROP_EDIT_PASSWORD_SET:
		{
			bool bVal = p->GetValue().fltVal ? true : false;
			pUI->SetPassWord(bVal);
		}
		break;
	case ePROP_EDIT_CURSOR_MOVER:
		{
			bool bVal = p->GetValue().fltVal ? true : false;
			pUI->SetCursorMove(bVal);
		}
		break;
	case ePROP_EDIT_READWINDOW_L:	case ePROP_EDIT_READWINDOW_T:	case ePROP_EDIT_READWINDOW_R:	case ePROP_EDIT_READWINDOW_B:
		{
			UIRectUV uv		= pUI->getReadingWindowUV();
			std::string	strVal = p->FormatProperty();

			ExtractionUV(eType, ePROP_EDIT_READWINDOW_L, uv, strVal);

			pUI->SetReadingWindowUV(uv.U0, uv.V0, uv.U1, uv.V1, fTexW, fTexH);

			p->SetValue(strVal.c_str());
		}
		break;
	case ePROP_EDIT_CANDIDATE_UV_L:	case ePROP_EDIT_CANDIDATE_UV_T:	case ePROP_EDIT_CANDIDATE_UV_R:	case ePROP_EDIT_CANDIDATE_UV_B:
		{
			UIRectUV uv		= pUI->getCandidateUV();
			std::string	strVal = p->FormatProperty();

			ExtractionUV(eType, ePROP_EDIT_CANDIDATE_UV_L, uv, strVal);
			
			pUI->SetCandidateUV(uv.U0, uv.V0, uv.U1, uv.V1, fTexW, fTexH);

			p->SetValue(strVal.c_str());
		}
		break;
	case ePROP_EDIT_BACK_UV_L:	case ePROP_EDIT_BACK_UV_T:	case ePROP_EDIT_BACK_UV_R:	case ePROP_EDIT_BACK_UV_B:
		{
			UIRectUV uv		= pUI->getBackGroundUV();
			std::string	strVal = p->FormatProperty();

			ExtractionUV(eType, ePROP_EDIT_BACK_UV_L, uv, strVal);

			pUI->setBackGroundUV(uv);

			p->SetValue(strVal.c_str());
		}
		break;
	case ePROP_EDIT_BACK_RECT_X:	case ePROP_EDIT_BACK_RECT_Y:	case ePROP_EDIT_BACK_RECT_W:	case ePROP_EDIT_BACK_RECT_H:
		{
			UIRect rc	= pUI->getBackGroundRect();
			float	fVal	= p->GetValue().intVal;
			ExtractionRect(eType, ePROP_EDIT_BACK_RECT_X, rc, fVal);

			pUI->setBackGroundRect(rc);
		}
		break;
	}
}

void CPropertiesWnd::ChangeCheckControlProp( LPARAM lparam )
{
	CMFCPropertyGridPropertyEx* pProp = ( CMFCPropertyGridPropertyEx* ) lparam;
	ePROP_TYPE eType = (ePROP_TYPE)pProp->GetData();
	CUICheckButton* pUI = dynamic_cast<CUICheckButton*>(m_pSelectedUI);	

	switch( eType )
	{
	case ePROP_CHECK_TEX:
		{
			std::string strFullPath = pProp->FormatProperty();
			std::string strFileName = ExtracFileName((char*)strFullPath.c_str());
			CmdChangeTexString* pCmd = new CmdChangeTexString();
			pCmd->setData((CUIBase*)pUI, (LPSTR)(LPCSTR)strFileName.c_str());
			theApp.addQueue(pCmd);
			pProp->SetValue(strFileName.c_str());
		}
		break;
	case ePROP_CHECK_STR_IDX:
		{
			int nIdx = pProp->GetValue().intVal;
			pUI->setStringIndex((INDEX)nIdx);
		}
		break;
	case ePROP_CHECK_STR:
		{
			CTString str = pProp->FormatProperty();
			if( str.IsEmpty() == false )
				pUI->SetText(str);
		}
		break;
	case ePROP_CHECK_POS_LEFT:	case ePROP_CHECK_TEXT_X:	case ePROP_CHECK_REGION:
		{
			BOOL bLeft			= pUI->GetLeft();
			int  nTextSX		= pUI->GetTextSX();
			int	 nCheckRegion	= pUI->GetCheckRegion();
			if( eType == ePROP_CHECK_POS_LEFT )
			{
				bLeft = pProp->GetValue().boolVal ? 1 : 0;
			}
			else if( eType == ePROP_CHECK_TEXT_X )
			{
				nTextSX = pProp->GetValue().intVal;
			}
			else if( eType == ePROP_CHECK_REGION )
			{
				nCheckRegion = pProp->GetValue().intVal;
			}
			pUI->SetCheckRegion(bLeft, nTextSX, nCheckRegion);
		}
		break;
	case ePROP_CHECK_TEXT_AREA:
		{
			int val = pProp->GetValue().intVal;
			pUI->setTextArea(val);
		}
		break;
	case ePROP_CHECK_ALIGN:
		{
			int nAlign;
			std::string strTmp = pProp->FormatProperty();
			for( nAlign = 0; nAlign < 3; nAlign++ )
			{
				if( strcmpi(szAlign_H[nAlign], strTmp.c_str()) == 0 )
					pUI->setAlignTextH((eALIGN_H)nAlign);
			}
		}
		break;
	case ePROP_CHECK_EDGE:
		{
			pUI->setEdge( pProp->GetValue().boolVal ? true : false );
		}
		break;
	case ePROP_CHECK_COLOR_ON:
		{
			CMFCPropertyGridColorProperty* pColor = (CMFCPropertyGridColorProperty*) lparam;
			COLORREF col = pColor->GetColor();
			pUI->SetTextColor(TRUE, ConvertToColor(col));
		}
		break;
	case ePROP_CHECK_COLOR_OFF:
		{
			CMFCPropertyGridColorProperty* pColor = (CMFCPropertyGridColorProperty*) lparam;
			COLORREF col = pColor->GetColor();
			
			pUI->SetTextColor(FALSE, ConvertToColor(col));
		}
		break;
	case ePROP_CHECK_NONE_L:	case ePROP_CHECK_NONE_T:	case ePROP_CHECK_NONE_R:	case ePROP_CHECK_NONE_B:
	case ePROP_CHECK_CHECK_L:	case ePROP_CHECK_CHECK_T:	case ePROP_CHECK_CHECK_R:	case ePROP_CHECK_CHECK_B:
		{
			UICheckBtnState state;
			ePROP_TYPE		start;
			if( ePROP_CHECK_NONE_L <= eType && eType <= ePROP_CHECK_NONE_B )
			{
				state = UCBS_NONE;
				start = ePROP_CHECK_NONE_L;
			}
			else if( ePROP_CHECK_CHECK_L <= eType && eType <= ePROP_CHECK_CHECK_B )
			{
				state = UCBS_CHECK;
				start = ePROP_CHECK_CHECK_L;
			}

			UIRectUV uv		= pUI->GetUV(state);
			std::string strVal = pProp->FormatProperty();

			ExtractionUV(eType, start, uv, strVal);

			pUI->SetUVTex(state, uv);

			pProp->SetValue(strVal.c_str());
		}
		break;
	}
}

void CPropertiesWnd::ChangeComboBoxControlProp( CMFCPropertyGridProperty* p )
{
	ePROP_TYPE eType = (ePROP_TYPE)p->GetData();
	CUIComboBox* pUI = dynamic_cast<CUIComboBox*>(m_pSelectedUI);	

	int nValue = 0;
	switch(eType)
	{
	case ePROP_COMBO_TEX :
		{
			std::string strFullPath = p->FormatProperty();
			std::string strFileName = ExtracFileName((char*)strFullPath.c_str());
			CmdChangeTexString* pCmd = new CmdChangeTexString();
			pCmd->setData((CUIBase*)pUI, (LPSTR)(LPCSTR)strFileName.c_str());
			theApp.addQueue(pCmd);

			p->SetValue(strFileName.c_str());
		}
		break;
	case ePROP_COMBO_MAXLINE :
		{
			nValue = p->GetValue().intVal;
			pUI->setMaxLine(nValue);
		}
		break;
	case ePROP_COMBO_LINE_HEIGHT :
		{
			nValue = p->GetValue().intVal;
			pUI->setLineHeight(nValue);
		}
		break;
	case ePROP_COMBO_SPACE_X :	case ePROP_COMBO_SPACE_Y :
		{
			nValue = p->GetValue().intVal;
			int x, y;
			pUI->getSpace(x,y);
			if( eType == ePROP_COMBO_SPACE_X )
				x = nValue;
			else if( eType == ePROP_COMBO_SPACE_Y)
				y = nValue;

			pUI->setSpace(x, y);
		}
		break;
	case ePROP_COMBO_SHOW_LINE:
		{
			nValue = p->GetValue().intVal;
			pUI->setShowLine(nValue);
		}
		break;
	case ePROP_COMBO_BACK_X: case ePROP_COMBO_BACK_Y: case ePROP_COMBO_BACK_W: case ePROP_COMBO_BACK_H:
	case ePROP_COMBO_BACK_L: case ePROP_COMBO_BACK_T: case ePROP_COMBO_BACK_R: case ePROP_COMBO_BACK_B:
		{
			UIRect		rc;
			UIRectUV	uv;
			
			pUI->getBackGround(rc, uv);
			if( eType >= ePROP_COMBO_BACK_X && eType <= ePROP_COMBO_BACK_H )
			{
				int			nVal = p->GetValue().intVal;
				ExtractionRect(eType, ePROP_COMBO_BACK_X, rc, nVal);
				pUI->setBaseRect(rc);
			}
			else if( eType >= ePROP_COMBO_BACK_L && eType <= ePROP_COMBO_BACK_B )
			{
				std::string strVal = p->FormatProperty();

				ExtractionUV(eType, ePROP_COMBO_BACK_L, uv, strVal);

				pUI->setBaseUV(uv);

				p->SetValue(strVal.c_str());
			}
		}
		break;
	case ePROP_COMBO_BTN_X:	  case ePROP_COMBO_BTN_Y:  case ePROP_COMBO_BTN_W:  case ePROP_COMBO_BTN_H:
	case ePROP_COMBO_BTN0_L:  case ePROP_COMBO_BTN0_T: case ePROP_COMBO_BTN0_R: case ePROP_COMBO_BTN0_B:
	case ePROP_COMBO_BTN1_L:  case ePROP_COMBO_BTN1_T: case ePROP_COMBO_BTN1_R: case ePROP_COMBO_BTN1_B:
		{
			UIRect		rc;
			UIRectUV	uv0, uv1;
			
			pUI->getComboButton(rc, uv0, uv1);

			if( eType >= ePROP_COMBO_BTN_X && eType <= ePROP_COMBO_BTN_H )
			{
				int nVal = p->GetValue().intVal;
				ExtractionRect(eType, ePROP_COMBO_BTN_X, rc, nVal);
			}
			else
			{
				std::string strVal = p->FormatProperty();

				if( eType >= ePROP_COMBO_BTN0_L && eType <= ePROP_COMBO_BTN0_B )
					ExtractionUV(eType, ePROP_COMBO_BTN0_L, uv0, strVal);
				else if( eType >= ePROP_COMBO_BTN1_L && eType <= ePROP_COMBO_BTN1_B )
					ExtractionUV(eType, ePROP_COMBO_BTN1_L, uv1, strVal);

				p->SetValue(strVal.c_str());
			}

			pUI->setComboButton(rc, uv0, uv1);
		}
		break;
	case ePROP_COMBO_DROP_AREA_X:	case ePROP_COMBO_DROP_AREA_Y:	case ePROP_COMBO_DROP_AREA_W:	case ePROP_COMBO_DROP_AREA_H:
	case ePROP_COMBO_DROP_AREA_L:	case ePROP_COMBO_DROP_AREA_T:	case ePROP_COMBO_DROP_AREA_R:	case ePROP_COMBO_DROP_AREA_B:
		{
			UIRect		rc;
			UIRectUV	uv;
			int			nTmp;
			
			pUI->getDropArea(rc, uv, nTmp);

			if( eType >= ePROP_COMBO_DROP_AREA_X && eType <= ePROP_COMBO_DROP_AREA_H )
			{
				int			nVal = p->GetValue().intVal;
				ExtractionRect(eType, ePROP_COMBO_DROP_AREA_X, rc, nVal);
			}
			else if( eType >= ePROP_COMBO_DROP_AREA_L && eType <= ePROP_COMBO_DROP_AREA_B )
			{
				std::string strVal = p->FormatProperty();
				ExtractionUV(eType, ePROP_COMBO_DROP_AREA_L, uv, strVal);

				p->SetValue(strVal.c_str());
			}

			pUI->setDropBG(rc, uv);
		}
		break;
	}
}

void CPropertiesWnd::ChangeSlideBarControlProp( CMFCPropertyGridProperty* p )
{
	ePROP_TYPE eType = (ePROP_TYPE)p->GetData();
	CUISlideBar* pUI = dynamic_cast<CUISlideBar*>(m_pSelectedUI);	

	std::string strTex = pUI->getTexString();
	float fTexW = 1.0f;
	float fTexH = 1.0f;
	if( strTex.empty() == false )
	{
		std::string strPath = "Data\\Interface\\";
		strPath += strTex;
		CTextureData* pTex = _pTextureStock->Obtain_t( strPath.c_str() );
		fTexW = pTex->GetPixWidth();
		fTexH = pTex->GetPixHeight();
	}

	switch(eType)
	{
	case ePROP_SLIDEBAR_TEX :
		{
			std::string strFullPath = p->FormatProperty();
			std::string strFileName = ExtracFileName((char*)strFullPath.c_str());
			CmdChangeTexString* pCmd = new CmdChangeTexString();
			pCmd->setData((CUIBase*)pUI, (LPSTR)(LPCSTR)strFileName.c_str());
			theApp.addQueue(pCmd);
			p->SetValue(strFileName.c_str());
		}
		break;
	case ePROP_SLIDEBAR_BAR_W:
		{
			int nVal = p->GetValue().intVal;			
			pUI->setBar(nVal, pUI->getBarHeight());
		}
		break;
	case ePROP_SLIDEBAR_BAR_H:
		{
			int nVal = p->GetValue().intVal;			
			pUI->setBar(pUI->getBarWidth(), nVal);
		}
		break;
	case ePROP_SLIDEBAR_CURPOS:
		{
			int nVal = p->GetValue().intVal;
			pUI->SetCurPos(nVal);
		}
		break;
	case ePROP_SLIDEBAR_RANGE:
		{
			int nVal = p->GetValue().intVal;
			pUI->SetRange(nVal);
		}
		break;
	case ePROP_SLIDEBAR_MIN:
		{
			int nVal = p->GetValue().intVal;
			pUI->SetMinPos(nVal);
		}
		break;
	case ePROP_SLIDEBAR_MAX:
		{
			int nVal = p->GetValue().intVal;
			pUI->SetMaxPos(nVal);
		}
		break;
	case ePROP_SLIDEBAR_BACK_L:	case ePROP_SLIDEBAR_BACK_T:	case ePROP_SLIDEBAR_BACK_R:	case ePROP_SLIDEBAR_BACK_B:
		{
			UIRectUV uv		= pUI->getBackUV();
			std::string strVal = p->FormatProperty();

			ExtractionUV(eType, ePROP_SLIDEBAR_BACK_L, uv, strVal);

			pUI->SetBackgroundUV(uv.U0, uv.V0, uv.U1, uv.V1, fTexW, fTexH);

			p->SetValue(strVal.c_str());
		}
		break;
	case ePROP_SLIDEBAR_BAR_L:	case ePROP_SLIDEBAR_BAR_T:	case ePROP_SLIDEBAR_BAR_R:	case ePROP_SLIDEBAR_BAR_B:
		{
			UIRectUV uv		= pUI->getBarUV();
			std::string strVal = p->FormatProperty();

			ExtractionUV(eType, ePROP_SLIDEBAR_BAR_L, uv, strVal);

			pUI->SetBarUV(uv.U0, uv.V0, uv.U1, uv.V1, fTexW, fTexH);

			p->SetValue(strVal.c_str());
		}
		break;
	}
}

void CPropertiesWnd::ChangeProgressBarControlProp( LPARAM lparam )
{
	CMFCPropertyGridPropertyEx* pProp = ( CMFCPropertyGridPropertyEx* ) lparam;
	ePROP_TYPE eType = (ePROP_TYPE)pProp->GetData();
	CUIProgressBar* pUI = dynamic_cast<CUIProgressBar*>(m_pSelectedUI);	

	switch( eType )
	{
	case ePROP_PROGRESS_TEX:
		{
			std::string strFullPath = pProp->FormatProperty();
			std::string strFileName = ExtracFileName((char*)strFullPath.c_str());

			CmdChangeTexString* pCmd = new CmdChangeTexString();
			pCmd->setData((CUIBase*)pUI, (LPSTR)(LPCSTR)strFileName.c_str());
			theApp.addQueue(pCmd);
			pProp->SetValue(strFileName.c_str());
		}
	case ePROP_PROGRESS_LOWER:	case ePROP_PROGRESS_UPPER:
		{
			int nLower, nUpper;
			pUI->GetProgressRange(nLower, nUpper);
			if( eType == ePROP_PROGRESS_LOWER )
				nLower = pProp->GetValue().intVal;
			else if( eType == ePROP_PROGRESS_UPPER )
				nUpper = pProp->GetValue().intVal;

			pUI->SetProgressRange(nLower, nUpper);
		}
		break;
	case ePROP_PROGRESS_STEP:
		{
			int nVal = pProp->GetValue().intVal;
			pUI->SetProgressStep(nVal);
		}
		break;
	case ePROP_PROGRESS_OFFSETPOS:
		{
			int nVal = pProp->GetValue().intVal;
			pUI->OffsetProgressPos(nVal);
		}
		break;
	case ePROP_PROGRESS_AUTO_COMPLETE:
		{
			int nVal = pProp->GetValue().boolVal;
			pUI->SetAutoComplete( nVal ? TRUE : FALSE );
		}
		break;
	case ePROP_PROGRESS_UV_BACK_L:	case ePROP_PROGRESS_UV_BACK_T:	case ePROP_PROGRESS_UV_BACK_R:	case ePROP_PROGRESS_UV_BACK_B:
		{
			UIRectUV uv = pUI->getUVBack();
			std::string strVal = pProp->FormatProperty();

			ExtractionUV(eType, ePROP_PROGRESS_UV_BACK_L, uv, strVal);

			pUI->setUVBack(uv);

			pProp->SetValue(strVal.c_str());
		}
		break;
	case ePROP_PROGRESS_BAR_SX:		case ePROP_PROGRESS_BAR_SY:
		{
			int nSX, nSY;
			pUI->getBarStartPos(nSX, nSY);
			if( eType == ePROP_PROGRESS_BAR_SX )
				nSX = pProp->GetValue().intVal;
			else if( eType == ePROP_PROGRESS_BAR_SY )
				nSY = pProp->GetValue().intVal;
			pUI->setBarStartPos(nSX, nSY);
		}
		break;
	case ePROP_PROGRESS_UV_BAR_L:	case ePROP_PROGRESS_UV_BAR_T:	case ePROP_PROGRESS_UV_BAR_R:	case ePROP_PROGRESS_UV_BAR_B:
		{
			UIRectUV uv = pUI->getUVBar();
			std::string strVal = pProp->FormatProperty();

			ExtractionUV(eType, ePROP_PROGRESS_UV_BAR_L, uv, strVal);

			pUI->setUVBar(uv);

			pProp->SetValue(strVal.c_str());
		}
		break;
	case ePROP_PROGRESS_IDLE_STR_IDX:	case ePROP_PROGRESS_DOING_STR_IDX:			case ePROP_PROGRESS_COMPLETE_STR_IDX:
		{
			int nVal = pProp->GetValue().intVal;
			_tagProgressStatus eState;
			if( eType == ePROP_PROGRESS_IDLE_STR_IDX )
				eState = PR_STAT_IDLE;
			else if( eType == ePROP_PROGRESS_DOING_STR_IDX )
				eState = PR_STAT_DOING;
			else if( eType == ePROP_PROGRESS_COMPLETE_STR_IDX )
				eState = PR_STAT_COMPLETE;

			pUI->setStringIdx(eState, nVal);
		}
		break;
	case ePROP_PROGRESS_IDLE_STR:		case ePROP_PROGRESS_IDLE_SHOW_PERCENT:		case ePROP_PROGRESS_IDLE_COLOR:
	case ePROP_PROGRESS_DOING_STR:		case ePROP_PROGRESS_DOING_SHOW_PERCENT:		case ePROP_PROGRESS_DOING_COLOR:
	case ePROP_PROGRESS_COMPLETE_STR:	case ePROP_PROGRESS_COMPLETE_SHOW_PERCENT:	case ePROP_PROGRESS_COMPLETE_COLOR:
		{
			int i, j;
			for( i = 0 ; i < PR_STAT_TAG_END; i++ )
			{	// 타입을 구분하기 위해 idle_str_idx 부터 complete_color로 계산. ( progress Bar 처음과 마지막 )
				if( ePROP_PROGRESS_IDLE_STR_IDX + (i * 4) <= eType &&
					ePROP_PROGRESS_IDLE_COLOR + (i * 4) >= eType )
					break;
			}
			CTString	str				= pUI->getProgressText((_tagProgressStatus)i);
			COLOR		col				= pUI->getColProgressText((_tagProgressStatus)i);
			bool		bShowPercent	= pUI->getShowPercentText((_tagProgressStatus)i) ? true : false; 

			int nUVType = (eType - ePROP_PROGRESS_IDLE_STR_IDX) % 4;

			CMFCPropertyGridColorProperty* pColor = (CMFCPropertyGridColorProperty*) lparam;
			
			for( j = 0; j < PR_STAT_TAG_END; j++ )
			{
				// i 는 위에서 구별했던 타입
				if( eType == ePROP_PROGRESS_IDLE_STR + ( i * 4 ) )
					str = pProp->FormatProperty();
				else if( eType == ePROP_PROGRESS_IDLE_SHOW_PERCENT + ( i * 4 ) )
					bShowPercent = pProp->GetValue().boolVal ? true : false;
				else if( eType == ePROP_PROGRESS_IDLE_COLOR + ( i * 4 ) )
				{
					col = ConvertToColor(pColor->GetColor());
				}
			}
			pUI->setProgressText((_tagProgressStatus)i, str, bShowPercent ? TRUE : FALSE, col);
		}
		break;
	}
}

void CPropertiesWnd::ChangeListControlProp( CMFCPropertyGridProperty* p )
{
	ePROP_TYPE eType = (ePROP_TYPE)p->GetData();
	CUIList* pUI = dynamic_cast<CUIList*>(m_pSelectedUI);

	switch( eType )
	{
	case ePROP_LIST_TEX:
		{
			std::string strFullPath = p->FormatProperty();
			std::string strFileName = ExtracFileName((char*)strFullPath.c_str());

			CmdChangeTexString* pCmd = new CmdChangeTexString();
			pCmd->setData((CUIBase*)pUI, (LPSTR)(LPCSTR)strFileName.c_str());
			theApp.addQueue(pCmd);
			p->SetValue(strFileName.c_str());
		}
	case ePROP_LIST_START_X : case ePROP_LIST_START_Y :
		{
			int nValX, nValY;
			pUI->GetItemStart(nValX, nValY);
			if( eType == ePROP_LIST_START_X)
				nValX = p->GetValue().intVal;
			else
				nValY = p->GetValue().intVal;

			pUI->SetItemStart(nValX, nValY);
		}
		break;
	case ePROP_LIST_GAP:
		{
			int nVal = p->GetValue().intVal;
			pUI->SetGap(nVal);
		}
		break;
	case ePROP_LIST_SHOW_COUNT:
		{
			int nVal = p->GetValue().intVal;
			pUI->SetItemShowNum(nVal);
		}
		break;
	case ePROP_LIST_ITEM_COUNT:
		{
			int nVal = p->GetValue().intVal;
			pUI->SetItemCount(nVal);
		}
		break;
	case ePROP_LIST_PRE_CREATE:
		{
			BOOL bVal = p->GetValue().boolVal;
			pUI->SetPreCreate(bVal ? true : false);
		}
		break;
	case ePROP_LIST_SELECT_RECT_X:	case ePROP_LIST_SELECT_RECT_Y:	case ePROP_LIST_SELECT_RECT_W:	case ePROP_LIST_SELECT_RECT_H:
	case ePROP_LIST_SELECT_UV_L:	case ePROP_LIST_SELECT_UV_T:	case ePROP_LIST_SELECT_UV_R:	case ePROP_LIST_SELECT_UV_B:
		{
			UIRect rc;
			UIRectUV uv;
			pUI->GetEventImg(rc, uv, CUIList::eTYPE_SELECT);
			if( eType >= ePROP_LIST_SELECT_RECT_X && eType <= ePROP_LIST_SELECT_RECT_H )
				ExtractionRect(eType, ePROP_LIST_SELECT_RECT_X, rc, p->GetValue().intVal);
			else
			{
				std::string strVal = p->FormatProperty();

				ExtractionUV(eType, ePROP_LIST_SELECT_UV_L, uv, strVal);

				p->SetValue(strVal.c_str());
			}
			pUI->addEventImage(rc, uv, CUIList::eTYPE_SELECT);
		}
		break;
	case ePROP_LIST_OVER_RECT_X:	case ePROP_LIST_OVER_RECT_Y:	case ePROP_LIST_OVER_RECT_W:	case ePROP_LIST_OVER_RECT_H:
	case ePROP_LIST_OVER_UV_L:	case ePROP_LIST_OVER_UV_T:	case ePROP_LIST_OVER_UV_R:	case ePROP_LIST_OVER_UV_B:
		{
			UIRect rc;
			UIRectUV uv;
			pUI->GetEventImg(rc, uv, CUIList::eTYPE_OVER);
			if( eType >= ePROP_LIST_OVER_RECT_X && eType <= ePROP_LIST_OVER_RECT_H )
				ExtractionRect(eType, ePROP_LIST_OVER_RECT_X, rc, p->GetValue().intVal);
			else
			{
				std::string strVal = p->FormatProperty();

				ExtractionUV(eType, ePROP_LIST_OVER_UV_L, uv, strVal);

				p->SetValue(strVal.c_str());
			}
			pUI->addEventImage(rc, uv, CUIList::eTYPE_OVER);
		}
		break;
	}
	pUI->UpdateList();
	pUI->UpdateScroll(pUI->getListItemCount());
}

void CPropertiesWnd::ChangeScrollBarControlProp( CMFCPropertyGridProperty* p )
{
	ePROP_TYPE eType = (ePROP_TYPE)p->GetData();
	CUIScrollBar* pUI = dynamic_cast<CUIScrollBar*>(m_pSelectedUI);

	UIRect   rc;
	UIRectUV uv0, uv1;

	switch( eType )
	{
	case ePROP_SB_TEX:
		{
			std::string strFullPath = p->FormatProperty();
			std::string strFileName = ExtracFileName((char*)strFullPath.c_str());

			CmdChangeTexString* pCmd = new CmdChangeTexString();
			pCmd->setData((CUIBase*)pUI, (LPSTR)(LPCSTR)strFileName.c_str());
			theApp.addQueue(pCmd);
			p->SetValue(strFileName.c_str());
		}
		break;
	case ePROP_SB_VERTICAL:
		{
			BOOL bVertical = p->GetValue().boolVal;
			pUI->setVertical(bVertical);
		}
		break;
	case ePROP_SB_RIGHT:
		{
			BOOL bRight = p->GetValue().boolVal;
			pUI->setRight(bRight ? true : false);
		}
		break;
	case ePROP_SB_SCROLL_POS:
		{
			int nPos = p->GetValue().intVal;
			pUI->SetScrollPos(nPos);
		}
		break;
	case ePROP_SB_MOVE_UNIT:
		{
			int nUnit = p->GetValue().intVal;
			pUI->setItemSize(nUnit);
		}
		break;
	case ePROP_SB_CUR_ITEM_CNT:
		{
			int nCnt = p->GetValue().intVal;
			pUI->SetCurItemCount(nCnt);
		}
		break;
	case ePROP_SB_ITEM_PER_PAGE:
		{
			int nPage = p->GetValue().intVal;
			pUI->SetItemsPerPage(nPage);
		}
		break;
	case ePROP_SB_BACK_RECT_X:	case ePROP_SB_BACK_RECT_Y:	case ePROP_SB_BACK_RECT_W:	case ePROP_SB_BACK_RECT_H:
	case ePROP_SB_BACK_UV_L:	case ePROP_SB_BACK_UV_T:	case ePROP_SB_BACK_UV_R:	case ePROP_SB_BACK_UV_B:
		{
			pUI->getBackGround(rc, uv0);
			if( eType >= ePROP_SB_BACK_RECT_X && eType <= ePROP_SB_BACK_RECT_H )
				ExtractionRect(eType, ePROP_SB_BACK_RECT_X, rc, p->GetValue().intVal);
			else
			{
				std::string strVal = p->FormatProperty();

				ExtractionUV(eType, ePROP_SB_BACK_UV_L, uv0, strVal);

				p->SetValue(strVal.c_str());
			}
			pUI->setBackGround(rc, uv0);
		}
		break;
	case ePROP_SB_BUTTON_1_RECT_X:		case ePROP_SB_BUTTON_1_RECT_Y:		case ePROP_SB_BUTTON_1_RECT_W:		case ePROP_SB_BUTTON_1_RECT_H:
	case ePROP_SB_BUTTON_1_UP_UV_L:		case ePROP_SB_BUTTON_1_UP_UV_T:		case ePROP_SB_BUTTON_1_UP_UV_R:		case ePROP_SB_BUTTON_1_UP_UV_B:
	case ePROP_SB_BUTTON_1_DOWN_UV_L:	case ePROP_SB_BUTTON_1_DOWN_UV_T:	case ePROP_SB_BUTTON_1_DOWN_UV_R:	case ePROP_SB_BUTTON_1_DOWN_UV_B:
		{
			pUI->getScrollBtn(0, rc, uv0, uv1);
			if( eType >= ePROP_SB_BUTTON_1_RECT_X && eType <= ePROP_SB_BUTTON_1_RECT_H )
				ExtractionRect(eType, ePROP_SB_BUTTON_1_RECT_X, rc, p->GetValue().intVal);
			else
			{
				std::string strVal = p->FormatProperty();

				if (eType >= ePROP_SB_BUTTON_1_DOWN_UV_L)
					ExtractionUV(eType, ePROP_SB_BUTTON_1_DOWN_UV_L, uv1, strVal);
				else
					ExtractionUV(eType, ePROP_SB_BUTTON_1_UP_UV_L, uv0, strVal);

				p->SetValue(strVal.c_str());
			}
			pUI->setButton(0, rc, uv0, uv1);
		}
		break;
	case ePROP_SB_BUTTON_2_RECT_X:		case ePROP_SB_BUTTON_2_RECT_Y:		case ePROP_SB_BUTTON_2_RECT_W:		case ePROP_SB_BUTTON_2_RECT_H:
	case ePROP_SB_BUTTON_2_UP_UV_L:		case ePROP_SB_BUTTON_2_UP_UV_T:		case ePROP_SB_BUTTON_2_UP_UV_R:		case ePROP_SB_BUTTON_2_UP_UV_B:
	case ePROP_SB_BUTTON_2_DOWN_UV_L:	case ePROP_SB_BUTTON_2_DOWN_UV_T:	case ePROP_SB_BUTTON_2_DOWN_UV_R:	case ePROP_SB_BUTTON_2_DOWN_UV_B:
		{
			pUI->getScrollBtn(1, rc, uv0, uv1);
			if( eType >= ePROP_SB_BUTTON_2_RECT_X && eType <= ePROP_SB_BUTTON_2_RECT_H )
				ExtractionRect(eType, ePROP_SB_BUTTON_2_RECT_X, rc, p->GetValue().intVal);
			else
			{
				std::string strVal = p->FormatProperty();

				if (eType >= ePROP_SB_BUTTON_2_DOWN_UV_L)
					ExtractionUV(eType, ePROP_SB_BUTTON_2_DOWN_UV_L, uv1, strVal);
				else
					ExtractionUV(eType, ePROP_SB_BUTTON_2_UP_UV_L, uv0, strVal);

				p->SetValue(strVal.c_str());
			}
			pUI->setButton(1, rc, uv0, uv1);
		}
		break;
	case ePROP_SB_THUMB_UNIT:
	case ePROP_SB_THUMB_RECT_X:	case ePROP_SB_THUMB_RECT_Y:	case ePROP_SB_THUMB_RECT_W:	case ePROP_SB_THUMB_RECT_H:
	case ePROP_SB_THUMB_UV_L:	case ePROP_SB_THUMB_UV_T:	case ePROP_SB_THUMB_UV_R:	case ePROP_SB_THUMB_UV_B:
		{
			int nUnit;
			pUI->getThumb(rc, uv0, nUnit);

			if( eType >= ePROP_SB_THUMB_UNIT && eType <= ePROP_SB_THUMB_RECT_H )
			{
				int nVal = p->GetValue().intVal;
				if( eType == ePROP_SB_THUMB_UNIT )
					nUnit = nVal;
				else
					ExtractionRect(eType, ePROP_SB_THUMB_RECT_X, rc, nVal);
			}
			else
			{
				std::string strVal = p->FormatProperty();

				ExtractionUV(eType, ePROP_SB_THUMB_UV_L, uv0, strVal);

				p->SetValue(strVal.c_str());
			}
			pUI->setThumb(rc, uv0, nUnit);
		}
		break;
	}
}

void CPropertiesWnd::ChangeSpinButtonControlProp( CMFCPropertyGridProperty* p )
{
	ePROP_TYPE eType = (ePROP_TYPE)p->GetData();
	CUISpinButton* pUI = dynamic_cast<CUISpinButton*>(m_pSelectedUI);

	switch( eType )
	{
	case ePROP_SPIN_TEX:
		{
			std::string strFullPath = p->FormatProperty();
			std::string strFileName = ExtracFileName((char*)strFullPath.c_str());

			CmdChangeTexString* pCmd = new CmdChangeTexString();
			pCmd->setData((CUIBase*)pUI, (LPSTR)(LPCSTR)strFileName.c_str());
			theApp.addQueue(pCmd);
			p->SetValue(strFileName.c_str());
		}
		break;
	case ePROP_SPIN_TITLE_STR:
		{
			std::string strVal = p->FormatProperty();
			pUI->SetTitle(strVal.c_str());
		}
		break;
	case ePROP_SPIN_AUTO_INFO:
		{
			bool	bVal	= p->GetValue().boolVal ? true : false;
			pUI->setBtnAutoInfo(bVal);
		}
		break;
	case ePROP_SPIN_BTN_W:	case ePROP_SPIN_BTN_H:	case ePROP_SPIN_BTN_GAP:
		{
			int nW, nH, nGap;
			pUI->getSpinButton(nW, nH, nGap);
			if( eType == ePROP_SPIN_BTN_W )
				nW = p->GetValue().intVal;
			else if( eType == ePROP_SPIN_BTN_H )
				nH = p->GetValue().intVal;
			else if( eType == ePROP_SPIN_BTN_GAP )
				nGap = p->GetValue().intVal;

			pUI->setSpinButton(nW, nH, nGap);
		}
		break;
	case ePROP_SPIN_DATA_BACK_WIDTH:
		{
			int nVal = p->GetValue().intVal;
			pUI->setDataBackWidth(nVal);
		}
		break;
	case ePROP_SPIN_BACK_UV_L:	case ePROP_SPIN_BACK_UV_T:	case ePROP_SPIN_BACK_UV_R:	case ePROP_SPIN_BACK_UV_B:
		{
			UIRectUV uv		 = pUI->getUVDataBack();
			std::string strVal = p->FormatProperty();

			ExtractionUV(eType, ePROP_SPIN_BACK_UV_L, uv, strVal);

			pUI->setUVDataBack(uv);

			p->SetValue(strVal.c_str());
		}
		break;
	case ePROP_SPIN_WHEEL_L:	case ePROP_SPIN_WHEEL_T:	case ePROP_SPIN_WHEEL_R:	case ePROP_SPIN_WHEEL_B:
		{
			UIRect	rc	= pUI->getWheelRect();
			ExtractionRect(eType, ePROP_SPIN_WHEEL_L, rc, p->GetValue().intVal);

			pUI->SetWheelRect(rc.Left, rc.Top, rc.Right, rc.Bottom);
		}
		break;
	}
}

void CPropertiesWnd::ChangeTreeControlProp( CMFCPropertyGridProperty* p )
{

}

void CPropertiesWnd::ChangeIconControlProp( CMFCPropertyGridProperty* p )
{
	ePROP_TYPE eType = (ePROP_TYPE)p->GetData();
	CUIIcon* pUI = dynamic_cast<CUIIcon*>(m_pSelectedUI);
	int			nRow, nCol, nType;
	SBYTE		sbTexID;

	pUI->getInfo(nType, sbTexID, nRow, nCol);

	switch( eType )
	{
	case ePROP_ICON_TYPE:
			nType = p->GetValue().intVal;
		break;
	case ePROP_ICON_TEX_ID:
			sbTexID = p->GetValue().intVal;
		break;
	case ePROP_ICON_ROW:
			nRow = p->GetValue().intVal;
		break;
	case ePROP_ICON_COL:
			nCol = p->GetValue().intVal;
		break;
	}

	if ((UIBtnExType)nType == UBET_TYPE_MAX)
	{
		nType = UBET_ITEM;
		CMFCPropertyGridProperty* pPropType = m_wndPropListEx.FindItemByData(ePROP_ICON_TYPE);

		if (pPropType != NULL)
			pPropType->SetValue((_variant_t)nType);
	}

	pUI->setInfo((UIBtnExType)nType, sbTexID, nRow, nCol);
}

void CPropertiesWnd::ChangeArrayControlProp( CMFCPropertyGridProperty* p )
{
	ePROP_TYPE eType = (ePROP_TYPE)p->GetData();
	CUIArray* pUI = dynamic_cast<CUIArray*>(m_pSelectedUI);
	int tmp1, tmp2;

	switch( eType )
	{
	case ePROP_ARRAY_IMG:
		{
			std::string strFullPath = p->FormatProperty();
			std::string strFileName = ExtracFileName((char*)strFullPath.c_str());

			CmdChangeTexString* pCmd = new CmdChangeTexString();
			pCmd->setData((CUIBase*)pUI, (LPSTR)(LPCSTR)strFileName.c_str());
			theApp.addQueue(pCmd);
			p->SetValue(strFileName.c_str());
		}
		break;
	case ePROP_ARRAY_ROW:	case ePROP_ARRAY_COL:
		{
			int val = p->GetValue().intVal;
			pUI->GetRowCol(tmp1, tmp2);

			if(eType == ePROP_ARRAY_ROW)
				tmp1 = val;
			else
				tmp2 = val;

			pUI->SetRowCol(tmp1, tmp2);
		}
		break;
	case ePROP_ARRAY_SX:	case ePROP_ARRAY_SY:
		{
			int val = p->GetValue().intVal;
			pUI->GetStartItem(tmp1, tmp2);

			if(eType == ePROP_ARRAY_SX)
				tmp1 = val;
			else
				tmp2 = val;

			pUI->SetStartItem(tmp1, tmp2);
		}
		break;
	case ePROP_ARRAY_GAPX:	case ePROP_ARRAY_GAPY:
		{
			int val = p->GetValue().intVal;
			pUI->GetGap(tmp1, tmp2);

			if(eType == ePROP_ARRAY_GAPX)
				tmp1 = val;
			else
				tmp2 = val;

			pUI->SetGap(tmp1, tmp2);
		}
		break;
	case ePROP_ARRAY_SHOW_ROW:
		{
			int val = p->GetValue().intVal;
			pUI->SetShowRowCount(val);
		}
		break;
	case ePROP_ARRAY_IMG_SELECT_RECT_X :	case ePROP_ARRAY_IMG_SELECT_RECT_Y :	case ePROP_ARRAY_IMG_SELECT_RECT_W :	case ePROP_ARRAY_IMG_SELECT_RECT_H :
	case ePROP_ARRAY_IMG_SELECT_UV_L :		case ePROP_ARRAY_IMG_SELECT_UV_T :		case ePROP_ARRAY_IMG_SELECT_UV_R :		case ePROP_ARRAY_IMG_SELECT_UV_B :
		{
			UIRect rc;
			UIRectUV uv;

			pUI->GetMouseEventImage(rc, uv, 0);

			if( eType >= ePROP_ARRAY_IMG_SELECT_RECT_X && eType <= ePROP_ARRAY_IMG_SELECT_RECT_H )
				ExtractionRect(eType, ePROP_ARRAY_IMG_SELECT_RECT_X, rc, p->GetValue().intVal);
			else
			{
				std::string strVal = p->FormatProperty();

				ExtractionUV(eType, ePROP_ARRAY_IMG_SELECT_UV_L, uv, strVal);

				p->SetValue(strVal.c_str());
			}
			pUI->SetMouseEventImage(rc, uv, 0);
		}
		break;
	case ePROP_ARRAY_IMG_OVER_RECT_X :	case ePROP_ARRAY_IMG_OVER_RECT_Y :	case ePROP_ARRAY_IMG_OVER_RECT_W :	case ePROP_ARRAY_IMG_OVER_RECT_H :
	case ePROP_ARRAY_IMG_OVER_UV_L :	case ePROP_ARRAY_IMG_OVER_UV_T :	case ePROP_ARRAY_IMG_OVER_UV_R :	case ePROP_ARRAY_IMG_OVER_UV_B :
		{
			UIRect rc;
			UIRectUV uv;

			pUI->GetMouseEventImage(rc, uv, 1);

			if( eType >= ePROP_ARRAY_IMG_OVER_RECT_X && eType <= ePROP_ARRAY_IMG_OVER_RECT_H )
				ExtractionRect(eType, ePROP_ARRAY_IMG_OVER_RECT_X, rc, p->GetValue().intVal);
			else
			{
				std::string strVal = p->FormatProperty();

				ExtractionUV(eType, ePROP_ARRAY_IMG_OVER_UV_L, uv, strVal);

				p->SetValue(strVal.c_str());
			}
			pUI->SetMouseEventImage(rc, uv, 1);
		}
		break;
	}
	pUI->UpdateItem();
}

void CPropertiesWnd::ChangeTextBoxControlProp( LPARAM lparam )
{
	CMFCPropertyGridPropertyEx* pProp = ( CMFCPropertyGridPropertyEx* ) lparam;
	ePROP_TYPE eType = (ePROP_TYPE)pProp->GetData();
	CUITextBox* pUI = dynamic_cast<CUITextBox*>(m_pSelectedUI);

	switch( eType )
	{
	case ePROP_TB_TEX:
		{
			std::string strFullPath = pProp->FormatProperty();
			std::string strFileName = ExtracFileName((char*)strFullPath.c_str());

			CmdChangeTexString* pCmd = new CmdChangeTexString();
			pCmd->setData((CUIBase*)pUI, (LPSTR)(LPCSTR)strFileName.c_str());
			theApp.addQueue(pCmd);
			pProp->SetValue(strFileName.c_str());
		}
		break;
	case ePROP_TB_STR_IDX:
		{
			int val = pProp->GetValue().intVal;
			pUI->setStringIdx((INDEX)val);
		}
		break;
	case ePROP_TB_STR:	case ePROP_TB_STR_COLOR:
		{
			CMFCPropertyGridColorProperty* pColor = (CMFCPropertyGridColorProperty*) lparam;
			COLOR col = pUI->GetTextColor();
			std::string str = pUI->GetString();

			if( eType == ePROP_TB_STR_COLOR )
			{
				col = ConvertToColor(pColor->GetColor());
			}
			else if( eType == ePROP_TB_STR )
				str = pProp->FormatProperty();

			pUI->SetText( CTString(str.c_str()), col );
		}
		break;
	case ePROP_TB_START_X:    case ePROP_TB_START_Y:
		{
			int x, y;
			pUI->GetStartXY(x, y);

			if( eType == ePROP_TB_START_X )
				x = pProp->GetValue().intVal;
			else if( eType == ePROP_TB_START_Y )
				y = pProp->GetValue().intVal;

			pUI->SetStartXY(x, y);
		}
		break;
	case ePROP_TB_GAP_Y:
		{
			int gap = pProp->GetValue().intVal;
			pUI->SetGapY(gap);
			
		}
	case ePROP_TB_ALIGN:
		{
			int nAlign;
			std::string strTmp = pProp->FormatProperty();
			for( nAlign = 0; nAlign < 3; nAlign++ )
			{
				if( strcmpi(szAlign_H[nAlign], strTmp.c_str()) == 0 )
					pUI->SetAlignTextH((eALIGN_H)nAlign);
			}
		}
		break;
	case ePROP_TB_SPLIT:
		{
			int nSplit;
			std::string strTmp = pProp->FormatProperty();
			for( nSplit = 0; nSplit < 6; nSplit++ )
			{
				if( strcmpi(szSplit[nSplit], strTmp.c_str()) == 0 )
					pUI->SetSplitMode((eSPLIT_TYPE)nSplit);
			}
		}
		break;
	case ePROP_TB_BACK_RECT_X:	case ePROP_TB_BACK_RECT_Y:	case ePROP_TB_BACK_RECT_W:	case ePROP_TB_BACK_RECT_H:
	case ePROP_TB_BACK_UV_L:	case ePROP_TB_BACK_UV_T:	case ePROP_TB_BACK_UV_R:	case ePROP_TB_BACK_UV_B:
		{
			UIRect rc;
			UIRectUV uv;

			pUI->getBackGround(rc, uv);
			if( eType >= ePROP_TB_BACK_RECT_X && eType <= ePROP_TB_BACK_RECT_H )
				ExtractionRect(eType, ePROP_TB_BACK_RECT_X, rc, pProp->GetValue().intVal);
			else
			{
				std::string strVal = pProp->FormatProperty();

				ExtractionUV(eType, ePROP_TB_BACK_UV_L, uv, strVal);

				pProp->SetValue(strVal.c_str());
			}

			pUI->setBackGround(rc, uv);
		}
		break;
	}
	pUI->OnUpdateBoxInfo();
}

void CPropertiesWnd::ChangeImageFontControlProp( CMFCPropertyGridProperty* p )
{
	ePROP_TYPE eType = (ePROP_TYPE)p->GetData();
	CUIImageFont* pUI = dynamic_cast<CUIImageFont*>(m_pSelectedUI);

	switch( eType )
	{
	case ePROP_IMGFONT_SRC_TEX:
		{
			std::string strFullPath = p->FormatProperty();
			std::string strFileName = ExtracFileName((char*)strFullPath.c_str());
			CmdChangeTexString* pCmd = new CmdChangeTexString();
			pCmd->setData((CUIBase*)pUI, (LPSTR)(LPCSTR)strFileName.c_str());
			theApp.addQueue(pCmd);
			p->SetValue(strFileName.c_str());
		}
		break;
	case ePROP_IMGFONT_FSIZE_X:		case ePROP_IMGFONT_FSIZE_Y:		case ePROP_IMGFONT_FGAP:
		{
			int sx, sy, gap;
			pUI->getFontSizeGap(sx, sy, gap);
			if( eType == ePROP_IMGFONT_FSIZE_X )
				sx = p->GetValue().intVal;
			else if( eType == ePROP_IMGFONT_FSIZE_Y )
				sy = p->GetValue().intVal;
			else
				gap = p->GetValue().intVal;

			pUI->setFontSizeGap(sx, sy, gap);
		}
		break;
	case ePROP_IMGFONT_OGAP:
			pUI->setOutputGap( p->GetValue().intVal );
		break;
	case ePROP_IMGFONT_STR:
		{
			std::string str = p->FormatProperty();
			pUI->setString(str.c_str());
		}
		break;
	case ePROP_IMGFONT_OSTR:
		{
			std::string str = p->FormatProperty();
			pUI->setOrigString(str.c_str());
		}
		break;
	case ePROP_IMGFONT_SRC_UV_L:	case ePROP_IMGFONT_SRC_UV_T:	case ePROP_IMGFONT_SRC_UV_R:	case ePROP_IMGFONT_SRC_UV_B:
		{
			UIRectUV uv = pUI->getSourceImageUV();
			std::string strVal = p->FormatProperty();

			ExtractionUV(eType, ePROP_IMGFONT_SRC_UV_L, uv, strVal);

			pUI->setSourceImage(uv);

			p->SetValue(strVal.c_str());
		}
		break;
	}
}

void CPropertiesWnd::ChangeSpriteAniControlProp( CMFCPropertyGridProperty* p )
{
	ePROP_SPRITE_ANI eType = (ePROP_SPRITE_ANI)p->GetData();
	CUISpriteAni* pUI = dynamic_cast<CUISpriteAni*>(m_pSelectedUI);

	UIRectUV uv;
	std::string strDesc;

	switch( eType )
	{
	case ePROP_SPRITE_TEX:
		{
			std::string strFullPath = p->FormatProperty();
			std::string strFileName = ExtracFileName((char*)strFullPath.c_str());
			CmdChangeTexString* pCmd = new CmdChangeTexString();
			pCmd->setData((CUIBase*)pUI, (LPSTR)(LPCSTR)strFileName.c_str());
			theApp.addQueue(pCmd);
			p->SetValue(strFileName.c_str());
		}
		break;
	case ePROP_SPRITE_CUR_IDX:
		{
			int nVal = p->GetValue().intVal;

			pUI->SetRenderIdx(nVal);
		}
		break;
	case ePROP_SPRITE_ADD:
		{
			int nVal = p->GetValue().intVal;
			int i, count = 0;

			if (nVal > m_nSpriteAniOldCount)
			{
				count = nVal - m_nSpriteAniOldCount;
				for (i = 0; i < count; ++i)
					pUI->PushUV(uv);
			}
			else
			{
				count = m_nSpriteAniOldCount - nVal;
				for (i = 0; i < count; ++i)
					pUI->RemoveUV(nVal);
			}

			parseUI(pUI);
		}
		break;
	case ePROP_SPRITE_LOOP:
		{
			BOOL bVal = p->GetValue().boolVal;

			if (bVal == TRUE)
				pUI->PlayAni();
			else
				pUI->StopAni();
		}
		break;
	default:
		{
			if (eType < ePROP_SPRITE_UV_L || eType >= ePROP_SPRITE_ADD)
				break;

			int nIdx = 0;
			int nTmpType = 0;

			if (eType >= ePROP_SPRITE_UV_L && eType < ePROP_SPRITE_ADD)
				nIdx = (eType - ePROP_SPRITE_UV_L) / 4;

			std::string strVal = p->FormatProperty();

			nTmpType = ePROP_SPRITE_UV_L + ( nIdx * 4);

			if (nTmpType >= 0)
			{
				pUI->DataAt(nIdx, uv);

				ExtractionUV((ePROP_TYPE)eType, (ePROP_TYPE)nTmpType, uv, strVal);
				p->SetValue(strVal.c_str());

				pUI->PushUV(uv, nIdx);
			}
		}
		break;
	}
}

void CPropertiesWnd::ChangeImageSplitControlProp( CMFCPropertyGridProperty* p )
{
	ePROP_TYPE eType = (ePROP_TYPE)p->GetData();
	CUIImageSplit* pUI = dynamic_cast<CUIImageSplit*>(m_pSelectedUI);
	
	UIRectUV uv = pUI->GetAbsUV();
	UIRect	rc;

	switch( eType )
	{
	case ePROP_IMGSPLIT_TYPE_L:	case ePROP_IMGSPLIT_TYPE_T:	case ePROP_IMGSPLIT_TYPE_R:	case ePROP_IMGSPLIT_TYPE_B:
		{
			std::string strVal = p->FormatProperty();
			ExtractionUV(eType, ePROP_IMGSPLIT_TYPE_L, uv, strVal);
			pUI->SetUV( uv );

			p->SetValue(strVal.c_str());
		}
		break;
	case ePROP_IMGSPLIT_TYPE_TEX:
		{
			std::string strFullPath = p->FormatProperty();
			std::string strFileName = ExtracFileName((char*)strFullPath.c_str());
			CmdChangeTexString* pCmd = new CmdChangeTexString();
			pCmd->setData((CUIBase*)pUI, (LPSTR)(LPCSTR)strFileName.c_str());
			theApp.addQueue(pCmd);
			p->SetValue(strFileName.c_str());
			pUI->SetUV( uv );
		}
		break;
	case ePROP_IMGSPLIT_SPLIT_MODE:
		{
			int nSplit;
			std::string strTmp = p->FormatProperty();
			for( nSplit = 0; nSplit < 3; nSplit++ )
			{
				if( strcmpi(szImageSplit[nSplit], strTmp.c_str()) == 0 )
					pUI->SetSplitMode((CUIImageSplit::eSPLIT_TYPE)nSplit);
			}
		}
		break;
	case ePROP_IMGSPLIT_SPLIT_UNIT:
		{
			int nUnit = p->GetValue().intVal;
			pUI->SetUnit(nUnit);
		}
		break;
	}

	pUI->UpdateSplit();

}

void CPropertiesWnd::ChangeImageArrayControlProp( CMFCPropertyGridProperty* p )
{
	ePROP_IMAGE_ARRAY_TYPE eType = (ePROP_IMAGE_ARRAY_TYPE)p->GetData();
	CUIImageArray* pUI = dynamic_cast<CUIImageArray*>(m_pSelectedUI);

	UIRectUV uv;
	UIRect	rc;
	std::string strDesc;

	switch( eType )
	{
	case ePROP_IMAGE_ARRAY_TEX:
		{
			std::string strFullPath = p->FormatProperty();
			std::string strFileName = ExtracFileName((char*)strFullPath.c_str());
			CmdChangeTexString* pCmd = new CmdChangeTexString();
			pCmd->setData((CUIBase*)pUI, (LPSTR)(LPCSTR)strFileName.c_str());
			theApp.addQueue(pCmd);
			p->SetValue(strFileName.c_str());
		}
		break;
	case ePROP_IMAGE_ARRAY_CUR_IDX:
		{
			int nVal = p->GetValue().intVal;

			pUI->SetRenderIdx(nVal);
		}
		break;
	case ePROP_IMAGE_ARRAY_ADD:
		{
			int nVal = p->GetValue().intVal;
			int i, count = 0;

			if (nVal > m_nImageArrayOldCount)
			{
				count = nVal - m_nImageArrayOldCount;
				for (i = 0; i < count; ++i)
					pUI->AddImage(rc, uv, -1, strDesc);
			}
			else
			{
				count = m_nImageArrayOldCount - nVal;
				for (i = 0; i < count; ++i)
					pUI->RemoveImage(nVal);
			}

			parseUI(pUI);
		}
		break;
	default:
		SetPropImageArray(pUI, p);
		break;
	}
}

void CPropertiesWnd::SetPropImageArray(CUIImageArray* pUI, CMFCPropertyGridProperty* pProp)
{
	if (pUI == NULL || pProp == NULL)
		return;

	ePROP_IMAGE_ARRAY_TYPE eType = (ePROP_IMAGE_ARRAY_TYPE)pProp->GetData();

	UIRectUV uv;
	UIRect	rc;
	std::string strDesc;

	int nIdx = 0;
	int nTmpType = 0;
	int nDataType = -1;	// 0 = Rect, 1 == uv, 2 == desc

	// type 인덱스 찾기.
	if (eType >= ePROP_IMAGE_ARRAY_RECT_X && eType < ePROP_IMAGE_ARRAY_UV_L)
	{
		nIdx = (eType - ePROP_IMAGE_ARRAY_RECT_X) / 4;
		nDataType = 0;
	}
	else if (eType >= ePROP_IMAGE_ARRAY_UV_L && eType < ePROP_IMAGE_ARRAY_DESC)
	{
		nIdx = (eType - ePROP_IMAGE_ARRAY_UV_L) / 4;
		nDataType = 1;
	}
	else if (eType >= ePROP_IMAGE_ARRAY_DESC)
	{
		nIdx = eType - ePROP_IMAGE_ARRAY_DESC;
		nDataType = 2;
	}
	else	// 아무것도 찾지 못했음..
		return;

	pUI->DataAt(nIdx, rc, uv, strDesc);

	switch( nDataType )
	{
	case 0:	// RECT
		{
			int nVal = pProp->GetValue().intVal;

			nTmpType = ePROP_IMAGE_ARRAY_RECT_X + ( nIdx * 4);
			ExtractionRect((ePROP_TYPE)eType, (ePROP_TYPE)nTmpType, rc, nVal);
		}
		break;
	case 1: // UV
		{
			std::string strVal = pProp->FormatProperty();

			nTmpType = ePROP_IMAGE_ARRAY_UV_L + ( nIdx * 4);
			ExtractionUV((ePROP_TYPE)eType, (ePROP_TYPE)nTmpType, uv, strVal);
			pProp->SetValue(strVal.c_str());
		}
		break;
	case 2: // DESC
		{
			std::string strVal = pProp->FormatProperty();

			strDesc = strVal;
		}
		break;
	}

	if (nDataType < 0)
		return;

	pUI->AddImage(rc, uv, nIdx, strDesc);
}

std::string CPropertiesWnd::ExtracFileName( char* FilePath )
{
	std::string strFileName = FilePath;

	std::size_t pos = strFileName.rfind('\\');

	if (pos != std::string::npos)
		strFileName = strFileName.substr(pos+1);

	return strFileName;
}

COLOR CPropertiesWnd::ConvertToColor( COLORREF col)
{
	BYTE r = GetRValue(col);
	BYTE g = GetGValue(col);
	BYTE b = GetBValue(col);
	BYTE a = (BYTE((col)>>24));
	
	return RGBAToColor(r,g,b,BYTE(0xFF));
}

void CPropertiesWnd::ShowPropImageSplit( CUIImageSplit* pUI )
{
	CMFCPropertyGridPropertyEx* pGroup = new CMFCPropertyGridPropertyEx(_T("이미지"));
	
	// Img UV
	UIRectUV uv = pUI->GetAbsUV();
	SetUV( (int)ePROP_IMGSPLIT_TYPE_L, (int)ePROP_IMGSPLIT_TYPE_T
		, (int)ePROP_IMGSPLIT_TYPE_R, (int)ePROP_IMGSPLIT_TYPE_B
		, uv, pGroup );
		
	// split mode
	int nSplit = pUI->GetMode();

	CMFCPropertyGridPropertyEx* pProp = new CMFCPropertyGridPropertyEx(_T("SplitMode"), (_variant_t) szImageSplit[nSplit], _T(""), ePROP_IMGSPLIT_SPLIT_MODE);

	for(int i = 0; i < 3; i++)
		pProp->AddOption(_T(szImageSplit[i]));

	pProp->AllowEdit(FALSE);
	pGroup->AddSubItem(pProp);

	// split unit
	int nUnit = pUI->GetUnit();

	pProp = new CMFCPropertyGridPropertyEx(_T("SplitUnit"), (_variant_t) nUnit, _T(""), ePROP_IMGSPLIT_SPLIT_UNIT);
	pProp->EnableSpinControl(TRUE, -3000, 3000);
	pGroup->AddSubItem(pProp);
			
	// Tex
	std::string strImage = pUI->getTexString();
	SetImage( (int)ePROP_IMGSPLIT_TYPE_TEX, strImage, pGroup );

	pGroup->Expand(TRUE);
	m_wndPropListEx.AddProperty(pGroup);
}

void CPropertiesWnd::ShowPropSpinControl( CUISpinControl* pUI )
{
	int _min;
	__int64 _llmax;

	pUI->GetMinMax(_min, _llmax);
	CMFCPropertyGridPropertyEx* pGroup = new CMFCPropertyGridPropertyEx(_T("Edit Control"));

	CMFCPropertyGridPropertyEx* pProp = new CMFCPropertyGridPropertyEx( _T("최소 값"), (_variant_t) _min, _T(""), ePROP_SP_MIN);
	pProp->EnableSpinControl(TRUE, 0, 1000);
	pGroup->AddSubItem(pProp);

	int nTmpMax = (int)_llmax;

	pProp = new CMFCPropertyGridPropertyEx( _T("최대 값"), (_variant_t) nTmpMax, _T(""), ePROP_SP_MAX);
	pProp->EnableSpinControl(TRUE, 0, 1000);
	pGroup->AddSubItem(pProp);

	bool bFocus			= pUI->IsFocused() ? true : false;
	bool bCursorMove	= pUI->GetCursorMove() ? true : false;
	int nCursorSX		= pUI->GetCursorSX();

	pProp = new CMFCPropertyGridPropertyEx( _T("커서이동"), (_variant_t) bCursorMove, _T("커서이동"), ePROP_SP_CURSOR_MOVER);
	pGroup->AddSubItem(pProp);

	pProp = new CMFCPropertyGridPropertyEx( _T("커서 Start PosX"), (_variant_t) nCursorSX, _T("커서 시작 위치."), ePROP_SP_CURSOR_SX);
	pGroup->AddSubItem(pProp);

	UIRectUV uvBack;
	UIRect	 rcBack;
	int nUnit = 0;

	pUI->GetBackGround(rcBack, uvBack, nUnit);

	CMFCPropertyGridPropertyEx* pBack = new CMFCPropertyGridPropertyEx( _T("배경 이미지") );
	SetUV(ePROP_SP_BACK_UV_L, ePROP_SP_BACK_UV_T, ePROP_SP_BACK_UV_R, ePROP_SP_BACK_UV_B,uvBack, pBack);
	SetRect(ePROP_SP_BACK_RECT_X, ePROP_SP_BACK_RECT_Y, ePROP_SP_BACK_RECT_W, ePROP_SP_BACK_RECT_H, rcBack, pBack);
	
	pProp = new CMFCPropertyGridPropertyEx( _T("Unit"), (_variant_t) nUnit, _T("Unit"), ePROP_SP_BACK_UNIT);
	pBack->AddSubItem(pProp);

	pBack->Expand(FALSE);
	pGroup->AddSubItem(pBack);

	std::string strImg = pUI->getTexString();
	SetImage( (int)ePROP_SP_TEX, strImg.c_str(), pGroup );

	pGroup->Expand(TRUE);
	m_wndPropListEx.AddProperty(pGroup);
}

void CPropertiesWnd::ChangeSpinControlProp( CMFCPropertyGridProperty* p )
{
	ePROP_TYPE eType = (ePROP_TYPE)p->GetData();
	CUISpinControl* pUI = dynamic_cast<CUISpinControl*>(m_pSelectedUI);	

	float fTexW = 1.f;
	float fTexH = 1.f;

	if (pUI->getTexString() != NULL)
	{
		fTexW = pUI->getTexWidth();
		fTexH = pUI->getTexHeight();
	}

	switch( eType )
	{
	case ePROP_SP_TEX:
		{
			std::string strFullPath = p->FormatProperty();
			std::string strFileName = ExtracFileName((char*)strFullPath.c_str());
			CmdChangeTexString* pCmd = new CmdChangeTexString();
			pCmd->setData((CUIBase*)pUI, (LPSTR)(LPCSTR)strFileName.c_str());
			theApp.addQueue(pCmd);
			p->SetValue(strFileName.c_str());
			//pUI->SetUV( uv );
		}
		break;
	case ePROP_SP_MIN:	case ePROP_SP_MAX:
		{
			int _min;
			SQUAD _llmax;

			pUI->GetMinMax(_min, _llmax);

			if (eType == ePROP_SP_MIN)
				_min = p->GetValue().intVal;
			else
				_llmax = p->GetValue().llVal;

			pUI->SetMinMax(_min, _llmax);
		}
		break;
	case ePROP_SP_CURSOR_MOVER:
		{
			bool bVal = p->GetValue().fltVal ? true : false;
			pUI->SetCursorMove(bVal);
		}
		break;
	case ePROP_SP_CURSOR_SX:
		{
			int val = p->GetValue().intVal;
			pUI->SetCursorSX(val);
		}
		break;
	case ePROP_SP_BACK_UV_L:	case ePROP_SP_BACK_UV_T:	case ePROP_SP_BACK_UV_R:	case ePROP_SP_BACK_UV_B:
		{
			UIRect rc;
			UIRectUV uv;
			int unit;

			std::string	strVal = p->FormatProperty();

			pUI->GetBackGround(rc, uv, unit);

			ExtractionUV(eType, ePROP_SP_BACK_UV_L, uv, strVal);

			pUI->SetBackGround(rc, uv, unit);

			p->SetValue(strVal.c_str());
		}
		break;
	case ePROP_SP_BACK_RECT_X:	case ePROP_SP_BACK_RECT_Y:	case ePROP_SP_BACK_RECT_W:	case ePROP_SP_BACK_RECT_H:
		{
			UIRect rc;
			UIRectUV uv;
			int unit;
			
			float	fVal	= p->GetValue().intVal;

			pUI->GetBackGround(rc, uv, unit);

			ExtractionRect(eType, ePROP_SP_BACK_RECT_X, rc, fVal);

			pUI->SetBackGround(rc, uv, unit);
		}
		break;
	case ePROP_SP_BACK_UNIT:
		{
			UIRect rc;
			UIRectUV uv;
			int unit;

			pUI->GetBackGround(rc, uv, unit);

			unit	= p->GetValue().intVal;

			pUI->SetBackGround(rc, uv, unit);
		}
		break;
	}
}

void CPropertiesWnd::ChangeTextBoxExControlProp( LPARAM lparam )
{
	CMFCPropertyGridPropertyEx* pProp = ( CMFCPropertyGridPropertyEx* ) lparam;
	ePROP_TYPE eType = (ePROP_TYPE)pProp->GetData();
	CUITextBoxEx* pUI = dynamic_cast<CUITextBoxEx*>(m_pSelectedUI);

	COLOR col = pUI->GetBaseColor();
	std::string str = pUI->GetText();

	switch( eType )
	{
	case ePROP_TB_EX_STR:	case ePROP_TB_EX_COL:
		{
			if( eType == ePROP_TB_EX_COL )
			{
				CMFCPropertyGridColorProperty* pColor = (CMFCPropertyGridColorProperty*) lparam;
				col = ConvertToColor(pColor->GetColor());
			}
			else if( eType == ePROP_TB_EX_STR )
			{
				str = pProp->FormatProperty();
			}
		}
		break;
	case ePROP_TB_EX_SX:    case ePROP_TB_EX_SY:
		{
			int x, y;
			x = pUI->GetStartX();
			y = pUI->GetStartY();

			if( eType == ePROP_TB_EX_SX )
				x = pProp->GetValue().intVal;
			else if( eType == ePROP_TB_EX_SY )
				y = pProp->GetValue().intVal;

			pUI->SetStartX(x);
			pUI->SetStartY(y);
		}
		break;
	case ePROP_TB_EX_GAP_Y:
		{
			int gap = pProp->GetValue().intVal;
			pUI->SetGapY(gap);
		}
		break;
	case ePROP_TB_EX_SPLIT_MODE:
		{
			int nSplit;
			std::string strTmp = pProp->FormatProperty();
			for( nSplit = 0; nSplit < 6; nSplit++ )
			{
				if( strcmpi(szSplit[nSplit], strTmp.c_str()) == 0 )
					pUI->SetSplitMode((eSPLIT_TYPE)nSplit);
			}
		}
		break;
	}

	pUI->Release();
	pUI->AddText( str, col );
}

void CPropertiesWnd::UpdatePos( int x, int y )
{
	CMFCPropertyGridProperty* pPropPosX = m_wndPropListEx.FindItemByData(ePROP_TYPE_X);
	CMFCPropertyGridProperty* pPropPosY = m_wndPropListEx.FindItemByData(ePROP_TYPE_Y);

	if (pPropPosX != NULL && pPropPosY != NULL)
	{
		pPropPosX->SetValue((_variant_t)x);
		pPropPosY->SetValue((_variant_t)y);
	}
}

////////////////////////// 탭 기능 추가를 위해 만들어짐. ////////////////////////////////

CMFCPropertyGridPropertyEx::CMFCPropertyGridPropertyEx(const CString& strName, const COleVariant& varValue, LPCTSTR lpszDescr, DWORD_PTR dwData, LPCTSTR lpszEditMask, LPCTSTR lpszEditTemplate, LPCTSTR lpszValidChars)
	: CMFCPropertyGridProperty(strName, varValue, lpszDescr, dwData, lpszEditMask, lpszEditTemplate, lpszValidChars)
{
}

CMFCPropertyGridPropertyEx::CMFCPropertyGridPropertyEx(const CString& strGroupName, DWORD_PTR dwData, BOOL bIsValueList)
	: CMFCPropertyGridProperty(strGroupName, dwData, bIsValueList)
{
}

BOOL CMFCPropertyGridPropertyEx::OnActivateByTab()
{
	ASSERT_VALID(this);
	ASSERT_VALID(m_pWndList);

	if (m_bInPlaceEdit &&(m_dwFlags & 0x0002))
	{
		m_bButtonIsFocused = !m_bButtonIsFocused;
		m_pWndList->RedrawWindow(m_rectButton);
		return TRUE;
	}

	if (CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd())
	{
		CMFCPropertyGridProperty* pItem = pMain->GetPropWnd()->m_wndPropListEx.GetTabItem();

		if (pItem == NULL)
			return FALSE;
		
		if (m_pWndList->EditItem(pItem))
		{
			if (!m_bInPlaceEdit && m_bEnabled)
			{
				CWnd* pEditPtr = ((CMFCPropertyGridPropertyEx*)pItem)->GetInPlaceEditPtr();

				CEdit* pEdit = DYNAMIC_DOWNCAST(CEdit, pEditPtr);
				if (::IsWindow(pEdit->GetSafeHwnd()))
				{
					pEdit->SetSel(0, -1);
				}
			}
		
			return TRUE;
		}
	}

	return FALSE;
}

CMFCPropertyGridProperty* CMFCPropertyGridPropertyEx::GetNextChildProp()
{
	int nChildMax = GetSubItemsCount();

	if (nChildMax <= 0)
		return this;

	CMFCPropertyGridPropertyEx* pChild = NULL;

	for (int i = 0; i < nChildMax; ++i)
	{
		pChild = (CMFCPropertyGridPropertyEx*)GetSubItem(i);

		if (pChild != this)
			continue;

		if (pChild->m_bInPlaceEdit == FALSE && pChild->m_varValue.vt != VT_EMPTY)
			return pChild;

		if (i + 1 < nChildMax)
		{
			pChild = (CMFCPropertyGridPropertyEx*)GetSubItem(i + 1);

			if (pChild == NULL)
				return this;

			if (pChild->GetSubItemsCount() > 0)
				return pChild->GetNextChildProp();

			return pChild;
		}

		break;
	}
	
	pChild = (CMFCPropertyGridPropertyEx*)GetSubItem(0);

	if (pChild->GetSubItemsCount() > 0)
		return pChild->GetNextChildProp();

	return pChild;
}

CMFCPropertyGridProperty* CMFCPropertyGridPropertyEx::GetNextProp(CMFCPropertyGridPropertyEx* pParent /*= NULL*/)
{
	if (pParent == NULL)
		return this;

	int nChildMax = pParent->GetSubItemsCount();

	for (int i = 0; i < nChildMax; ++i)
	{
		CMFCPropertyGridPropertyEx* pNext = (CMFCPropertyGridPropertyEx*)pParent->GetSubItem(i);

		if (pNext != this)
			continue;

		if (pNext->m_bInPlaceEdit == FALSE && pNext->m_varValue.vt != VT_EMPTY)
			return pNext;

		if (i + 1 < nChildMax)
		{
			pNext = (CMFCPropertyGridPropertyEx*)pParent->GetSubItem(i + 1);

			if (pNext == NULL)
				return this;

			if (pNext->GetSubItemsCount() > 0)
				return pNext->GetNextChildProp();

			return pNext;
		}
		else
		{
			if (pParent->GetParent() == NULL)
				return this;
			return pParent->GetNextProp((CMFCPropertyGridPropertyEx*)pParent->GetParent());
		}
	}

	return this;
}

CMFCPropertyGridProperty* CMFCPropertyGridPropertyEx::GetPrevChildProp()
{
	int nChildMax = GetSubItemsCount();

	if (nChildMax <= 0)
		return this;

	CMFCPropertyGridPropertyEx* pChild = NULL;

	for (int i = nChildMax - 1; i >= 0; --i)
	{
		pChild = (CMFCPropertyGridPropertyEx*)GetSubItem(i);

		if (pChild != this)
			continue;

		if (pChild->m_bInPlaceEdit == FALSE && pChild->m_varValue.vt != VT_EMPTY)
			return pChild;

		if (i - 1 >= 0)
		{
			pChild = (CMFCPropertyGridPropertyEx*)GetSubItem(i - 1);

			if (pChild == NULL)
				return this;

			if (pChild->GetSubItemsCount() > 0)
				return pChild->GetPrevChildProp();

			return pChild;
		}

		break;
	}

	pChild = (CMFCPropertyGridPropertyEx*)GetSubItem(nChildMax - 1);

	if (pChild->GetSubItemsCount() > 0)
		return pChild->GetPrevChildProp();

	return pChild;
}

CMFCPropertyGridProperty* CMFCPropertyGridPropertyEx::GetPrevProp( CMFCPropertyGridPropertyEx* pParent /*= NULL*/ )
{
	if (pParent == NULL)
		return this;

	int nChildMax = pParent->GetSubItemsCount();

	for (int i = nChildMax - 1; i >= 0; --i)
	{
		CMFCPropertyGridPropertyEx* pPrev = (CMFCPropertyGridPropertyEx*)pParent->GetSubItem(i);

		if (pPrev != this)
			continue;

		if (pPrev->m_bInPlaceEdit == FALSE && pPrev->m_varValue.vt != VT_EMPTY)
			return pPrev;

		if (i - 1 >= 0)
		{
			pPrev = (CMFCPropertyGridPropertyEx*)pParent->GetSubItem(i - 1);

			if (pPrev == NULL)
				return this;

			if (pPrev->GetSubItemsCount() > 0)
				return pPrev->GetPrevChildProp();

			return pPrev;
		}
		else
		{
			if (pParent->GetParent() == NULL)
				return NULL;
			return pParent->GetPrevProp((CMFCPropertyGridPropertyEx*)pParent->GetParent());
		}
	}

	return this;
}

CMFCPropertyGridProperty* CMFCPropertyGridPropertyEx::GetTopParent()
{
	if (GetParent() != NULL)
		return ((CMFCPropertyGridPropertyEx*)GetParent())->GetTopParent();

	return this;
}

CMFCPropertyGridCtrlEx::CMFCPropertyGridCtrlEx()
	: m_bShift(false)
{

}

CMFCPropertyGridProperty* CMFCPropertyGridCtrlEx::GetTabItem()
{
	if (m_bShift == true)
		return GetPrevItem();
	return GetNextItem();
}

CMFCPropertyGridProperty* CMFCPropertyGridCtrlEx::GetNextItem()
{
	CMFCPropertyGridPropertyEx* pSel = (CMFCPropertyGridPropertyEx*)GetCurSel();
	CMFCPropertyGridPropertyEx* pFind = NULL;

	if (pSel == NULL)
		return NULL;

	if (pSel->GetSubItemsCount() > 0)
		pFind = (CMFCPropertyGridPropertyEx*)pSel->GetNextChildProp();
	else
		pFind = (CMFCPropertyGridPropertyEx*)pSel->GetNextProp((CMFCPropertyGridPropertyEx*)pSel->GetParent());

	if (pFind == pSel && pFind->GetInPlaceEdit() == TRUE)
	{
		CMFCPropertyGridPropertyEx* pParent = (CMFCPropertyGridPropertyEx*)pSel->GetParent();

		for (POSITION pos = m_lstProps.GetHeadPosition(); pos != NULL;)
		{
			CMFCPropertyGridPropertyEx* pListProp = (CMFCPropertyGridPropertyEx*)m_lstProps.GetNext(pos);
	
			if (pos == NULL)
			{
				POSITION headPos = m_lstProps.GetHeadPosition();
				pListProp = (CMFCPropertyGridPropertyEx*)m_lstProps.GetNext(headPos);

				return pListProp->GetSubItem(0);
			}

			if (pParent == pListProp)
			{
				CMFCPropertyGridPropertyEx* pNext = (CMFCPropertyGridPropertyEx*)m_lstProps.GetNext(pos);

				if (pNext != NULL)
					pFind = (CMFCPropertyGridPropertyEx*)pNext->GetNextChildProp();

				if (pFind != NULL && pFind != pSel)
					return pFind;
			}
		}
	}

	return pFind;
}

CMFCPropertyGridProperty* CMFCPropertyGridCtrlEx::GetPrevItem()
{
	CMFCPropertyGridPropertyEx* pSel = (CMFCPropertyGridPropertyEx*)GetCurSel();
	CMFCPropertyGridPropertyEx* pFind = NULL;

	pFind = (CMFCPropertyGridPropertyEx*)pSel->GetPrevProp((CMFCPropertyGridPropertyEx*)pSel->GetParent());

	if (pFind == NULL)
	{
		CMFCPropertyGridPropertyEx* pParent = (CMFCPropertyGridPropertyEx*)pSel->GetTopParent();

		for (POSITION pos = m_lstProps.GetTailPosition(); pos != NULL;)
		{
			CMFCPropertyGridPropertyEx* pListProp = (CMFCPropertyGridPropertyEx*)m_lstProps.GetPrev(pos);

			if (pos == NULL)
			{
				POSITION tailPos = m_lstProps.GetTailPosition();
				pListProp = (CMFCPropertyGridPropertyEx*)m_lstProps.GetPrev(tailPos);

				return pListProp->GetSubItem(pListProp->GetSubItemsCount() - 1);
			}

			if (pParent == pListProp)
			{
				CMFCPropertyGridPropertyEx* pPrev = (CMFCPropertyGridPropertyEx*)m_lstProps.GetPrev(pos);
	
				if (pPrev != NULL)
					pFind = (CMFCPropertyGridPropertyEx*)pPrev->GetPrevChildProp();

				if (pFind != NULL && pFind != pSel)
					return pFind;
			}
		}
	}

	return pFind;
}

BOOL CMFCPropertyGridCtrlEx::PreTranslateMessage( MSG* pMsg )
{
	switch(pMsg->message)
	{
	case WM_KEYDOWN:
		{
			if (pMsg->wParam == VK_SHIFT)
				m_bShift = true;
		}
		break;
	case WM_KEYUP:
		{
			if (pMsg->wParam == VK_SHIFT)
				m_bShift = false;
		}
		break;
	}

	return CMFCPropertyGridCtrl::PreTranslateMessage(pMsg);
}

////////////////////////// 탭 기능 추가를 위해 만들어짐. ////////////////////////////////