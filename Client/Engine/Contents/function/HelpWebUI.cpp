
#include "StdH.h"
#include <Engine/Interface/UIInternalClasses.h>
#include "HelpWebUI.h"

#include <Engine/Network/Web.h>

extern cWeb	g_web;

class CmdHelp_Web_Close : public Command
{
public:
	CmdHelp_Web_Close() : m_pWnd(NULL) {}
	void setData(CHelpWebUI* pWnd)	{ m_pWnd = pWnd;	}
	void execute()
	{
		if (m_pWnd != NULL)
			m_pWnd->CloseUI();
	}

private:
	CHelpWebUI* m_pWnd;
};

CHelpWebUI::CHelpWebUI()
	: m_pWebBase(NULL)
	, m_pDragArea(NULL)
	, m_nOriX(0)
	, m_nOriY(0)
	, m_bDrag(false)
{
	setInherit(false);
}

CHelpWebUI::~CHelpWebUI()
{

}

void CHelpWebUI::initialize()
{
	CUIButton* pBtn = (CUIButton*)findUI("btn_close");

	if (pBtn != NULL)
	{
		CmdHelp_Web_Close* pCmd = new CmdHelp_Web_Close;
		pCmd->setData(this);
		pBtn->SetCommandUp(pCmd);
	}

	CUIText* pText = (CUIText*)findUI("text_title");

	if (pText != NULL)
	{
		pText->SetText(_S(284, "µµ¿ò¸»"));
	}

	m_pWebBase = findUI("base_web");
	m_pDragArea = findUI("base_drag_area");
}

void CHelpWebUI::OpenUI()
{
	if (IsVisible() == TRUE || GetHide() == FALSE)
		return;

	SetVisible(TRUE);
	Hide(FALSE);

	UIMGR()->RearrangeOrder(UI_HELP_WEB, TRUE);

	if (m_pWebBase != NULL)
	{
		int nAbsX, nAbsY;
		m_pWebBase->GetAbsPos(nAbsX, nAbsY);
		g_web.SetSize(m_pWebBase->GetWidth(), m_pWebBase->GetHeight());
		g_web.SetPos(nAbsX, nAbsY);
		g_web.SendWebPageOpenMsg(TRUE);

		std::string strPath = "file:///" + _fnmApplicationPath.FileDir();
		strPath += "Data/Interface/Help/index.htm";
		g_web.SetWebUrl(strPath);
	}
}

void CHelpWebUI::CloseUI()
{
	if (g_web.GetWebHandle())
		g_web.CloseWebPage(g_web.GetWebHandle());

	SetVisible(FALSE);
	Hide(TRUE);
	UIMGR()->RearrangeOrder(UI_HELP_WEB, FALSE);
}

WMSG_RESULT CHelpWebUI::OnLButtonUp( UINT16 x, UINT16 y )
{
	if (m_bHide)
		return WMSG_FAIL;

	m_bDrag = false;

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	return CUIBase::OnLButtonUp(x, y);
}

WMSG_RESULT CHelpWebUI::OnLButtonDown( UINT16 x, UINT16 y )
{
	if (m_bHide)
		return WMSG_FAIL;

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	if( m_pDragArea && m_pDragArea->IsInside(x, y))
	{
		m_bDrag = true;
		m_nOriX = x;
		m_nOriY = y;
	}

	CUIManager::getSingleton()->RearrangeOrder(UI_HELP_WEB, TRUE);

	return CUIBase::OnLButtonDown(x, y);
}

WMSG_RESULT CHelpWebUI::OnMouseMove( UINT16 x, UINT16 y, MSG* pMsg )
{
	if (m_bHide)
		return WMSG_FAIL;

	if( m_bDrag && ( pMsg->wParam & MK_LBUTTON ) )
	{
		int ndX = x - m_nOriX;
		int ndY = y - m_nOriY;

		m_nOriX = x;
		m_nOriY = y;
		
		if (g_web.IsWebHandle() == TRUE)
		{
			int nMoveX = m_nPosX + ndX;
			int nMoveY = m_nPosY + ndY;

			if (nMoveX <= 0)
				nMoveX = 0;
			if (nMoveY <= 0)
				nMoveY = 0;
			if (nMoveX + m_nWidth >= UIMGR()->GetWidth())
				nMoveX = UIMGR()->GetWidth() - m_nWidth;
			if (nMoveY + m_nHeight >= UIMGR()->GetHeight())
				nMoveY = UIMGR()->GetHeight() - m_nHeight;

			m_nPosX = nMoveX;
			m_nPosY = nMoveY;

			int nAbsX, nAbsY;
			m_pWebBase->GetAbsPos(nAbsX, nAbsY);
			
			g_web.SetPos(nAbsX, nAbsY);
		}

		return WMSG_SUCCESS;
	}

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	return CUIBase::OnMouseMove(x, y, pMsg);
}

BOOL CHelpWebUI::CloseWindowByEsc()
{
	CloseUI();
	
	if (g_web.IsWebHandle())
	{
		g_web.OpenWebPage(FALSE);
	}

	return TRUE;
}
