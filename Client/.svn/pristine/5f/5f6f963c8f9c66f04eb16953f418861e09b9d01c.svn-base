#include "StdH.h"

#include "UIBase.h"
#include "UITextEx.h"
#include "UIText.h"
#include "UIButton.h"
#include "UITextureManager.h"

#include <Engine/Help/Util_Help.h>

#if		defined(G_RUSSIA)
extern CFontData *_pfdDefaultFont;
#endif

CUITextEx::CUITextEx()
	: m_strOrigin("")
	, m_colBase(DEF_UI_COLOR_WHITE)
	, m_nNextPosX(0)
{
	setType(eUI_CONTROL_TEXT_EX);
	DeleteUICont();
}

CUITextEx::~CUITextEx()
{
	Destroy();

	DeleteUICont();
}

CUIBase* CUITextEx::Clone()
{
	CUITextEx* pTextEx = new CUITextEx(*this);

	vec_uinode_iter		iter = m_VecChild.begin();
	vec_uinode_iter		eiter = m_VecChild.end();

	pTextEx->ClearUI();

	for(; iter != eiter; ++iter )
	{
		pTextEx->addUI((*iter)->Clone());
	}

	return pTextEx;
}

void CUITextEx::AddText(const char* str, DWORD dwCol /*=DEF_UI_COLOR_WHITE*/, int pos_x /*=-1*/)
{
 	CUIText* pText = new CUIText;
 	
 	pText->setFontColor(dwCol);
 	pText->SetText((CTString)str);
  
	int nStrWidth = 0;
	
#if		defined(G_RUSSIA)
	nStrWidth = UTIL_HELP()->GetNoFixedWidth(_pfdDefaultFont, ((CTString)str).str_String);
#else
 	nStrWidth = UIMGR()->GetDrawPort()->GetTextWidth2((CTString)str);
#endif // G_RUSSIA

	int nHeight = _pUIFontTexMgr->GetFontHeight();

	if (pos_x < 0)
		pos_x = m_nNextPosX;

	pText->SetPosX(pos_x);
	pText->SetSize(nStrWidth, nHeight);

	addChild((CUIBase*)pText);
	m_vecUICont.push_back((CUIBase*)pText);

	m_nNextPosX = pos_x + nStrWidth;
	m_nWidth = m_nNextPosX;
	m_strOrigin += str;
	SetSize(m_nNextPosX, nHeight);
}

void CUITextEx::AddBtn( const char* str, Command* pCmd, 
	DWORD dwColIdle /*= DEF_UI_COLOR_WHITE*/,
	DWORD dwColOver /*= DEF_UI_COLOR_YELLOW*/, 
	DWORD dwColClick /*= DEF_UI_COLOR_WHITE*/, 
	DWORD dwColDisable /*= DEF_UI_COLOR_YELLOW*/ )
{
	CUIButton* pBtn = new CUIButton;
	
	pBtn->SetText(CTString(str), FALSE, TRUE);
	pBtn->SetTextColor(UBS_IDLE, dwColIdle);
	pBtn->SetTextColor(UBS_ON, dwColOver);
	pBtn->SetTextColor(UBS_CLICK, dwColClick);
	pBtn->SetTextColor(UBS_DISABLE, dwColDisable);

	int nStrWidth = 0;

#if		defined(G_RUSSIA)
	nStrWidth = UTIL_HELP()->GetNoFixedWidth(_pfdDefaultFont, ((CTString)str).str_String);
#else
	nStrWidth = UIMGR()->GetDrawPort()->GetTextWidth2((CTString)str);
#endif // G_RUSSIA

	int nHeight = _pUIFontTexMgr->GetFontHeight();

	pBtn->SetPosX(m_nNextPosX);
	pBtn->SetSize(nStrWidth, nHeight);

	addChild((CUIBase*)pBtn);
	m_vecUICont.push_back((CUIBase*)pBtn);

	m_nNextPosX += nStrWidth;
	m_nWidth = m_nNextPosX;
	m_strOrigin += str;
	SetSize(m_nNextPosX, nHeight);

	if (pCmd != NULL)
	{
		pBtn->SetCommandUp(pCmd);
	}
}

void CUITextEx::DeleteUICont()
{
	vec_UICont_iter		iter = m_vecUICont.begin();
	vec_UICont_iter		eiter = m_vecUICont.end();

	for(; iter != eiter; ++iter )
	{
		deleteChild( (*iter) );
	}

	m_vecUICont.clear();
	m_strOrigin = "";
	m_nNextPosX = 0;
	m_colBase = DEF_UI_COLOR_WHITE;
}

void CUITextEx::addUI( CUIBase* pBase )
{
	m_vecUICont.push_back(pBase);
	addChild(pBase);
}

void CUITextEx::ClearUI()
{
	m_vecUICont.clear();
	m_VecChild.clear();
}
