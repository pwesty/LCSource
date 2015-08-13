
#include "stdh.h"
#include <Engine/Interface/UIInternalClasses.h>
#include "UIMsgBoxNumeric_only.h"

class CmdMsgNumericOnlyClose : public Command
{
public:
	CmdMsgNumericOnlyClose() : m_pWnd(NULL), _pCmd(NULL) {}
	void setData(CUIMsgBoxNumericOnly* pWnd, Command* pCmd = NULL)	
	{ 
		m_pWnd = pWnd;
		_pCmd = pCmd;
	}
	void execute() 
	{
		if (m_pWnd != NULL)
		{
			m_pWnd->CloseBox();

			if (_pCmd != NULL)
				m_pWnd->addQueue(_pCmd);
		}
	}
private:
	CUIMsgBoxNumericOnly* m_pWnd;
	Command* _pCmd;
};


CUIMsgBoxNumericOnly::CUIMsgBoxNumericOnly()
	: m_pSpinControl(NULL)
	, m_pTitle(NULL)
	, m_pDesc(NULL)
	, m_pCmdOk(NULL)
	, m_pCmdReturn(NULL)
	, m_pBtnOk(NULL)
	, m_bDrag(false)
	, m_nOriX(0)
	, m_nOriY(0)
	, m_nOldPosX(0)
	, m_nOldPosY(0)
{
	setInherit(false);

	m_csLock.cs_iIndex = -1;
}

CUIMsgBoxNumericOnly::~CUIMsgBoxNumericOnly()
{

}

void CUIMsgBoxNumericOnly::initialize()
{
	m_pSpinControl = (CUISpinControl*)findUI("sp_input");
	m_pTitle = (CUIText*)findUI("text_title");
	m_pDesc	= (CUITextBox*)findUI("tb_string");
	m_pBtnOk = (CUIButton*)findUI("btn_ok");
	m_pIsDrag = (CUIImageSplit*)findUI("is_title");

	m_pBtnClose = (CUIButton*)findUI("btn_close");
	m_pBtnCancel = (CUIButton*)findUI("btn_cancel");

	m_pCmdClose = new CmdMsgNumericOnlyClose;
	m_pBtnClose->SetCommandUp(m_pCmdClose);
	
	m_pCmdCancel = new CmdMsgNumericOnlyClose;
	m_pBtnCancel->SetCommandUp(m_pCmdCancel);

	m_pCmdOk = new CmdMsgNumericOnlyClose;
	m_pBtnOk->SetCommandUp(m_pCmdOk);

	m_pCmdReturn = new CmdMsgNumericOnlyClose;
	m_pSpinControl->SetCmdReturn(m_pCmdReturn);

	m_nOldPosX = (UIMGR()->GetWidth() / 2) - (GetWidth() / 2);
	m_nOldPosY = (UIMGR()->GetHeight() * 0.3) - (GetHeight() / 2);
	SetPos(m_nOldPosX, m_nOldPosY);
}

void CUIMsgBoxNumericOnly::SetInfo(Command* pCmdOk, CTString title, CTString desc, int nMin, SQUAD llMax, Command* pCmdCancel /*= NULL*/)
{
	if (m_pSpinControl == NULL || m_pTitle == NULL || m_pDesc == NULL)
		return;

	SetVisible(TRUE);
	Hide(FALSE);

	m_pSpinControl->SetMinMax(nMin, llMax);
	
	CTString strTmp;
	strTmp.PrintF("%d", nMin);

	m_pSpinControl->SetString(strTmp.str_String);
	m_pSpinControl->SetFocus(TRUE);

	m_pTitle->SetText(title);
	m_pDesc->SetText(desc);

	UIMGR()->RearrangeOrder(UI_MSGBOX_NUMERIC_ONLY, TRUE);
	SetPos(m_nOldPosX, m_nOldPosY);

	ConnectCmd(pCmdOk, pCmdCancel);
}

void CUIMsgBoxNumericOnly::InitData( SQUAD llData )
{
	if (m_pSpinControl == NULL)
		return;

	CTString strTmp;
	strTmp.PrintF("%d", llData);

	m_pSpinControl->SetString(strTmp.str_String);
}

 SQUAD CUIMsgBoxNumericOnly::GetData()
 {
	if (m_pSpinControl == NULL)
		return 0;

	return _atoi64(m_pSpinControl->GetString());
 }

WMSG_RESULT CUIMsgBoxNumericOnly::OnLButtonDown( UINT16 x, UINT16 y )
{
	if (m_bHide == TRUE)
		return WMSG_FAIL;

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	if( m_pIsDrag && m_pIsDrag->IsInside(x, y))
	{
		m_bDrag = true;
		m_nOriX = x;
		m_nOriY = y;
	}

	CUIManager::getSingleton()->RearrangeOrder(UI_MSGBOX_NUMERIC_ONLY, TRUE);
	return WMSG_FAIL;
}

WMSG_RESULT CUIMsgBoxNumericOnly::OnLButtonUp( UINT16 x, UINT16 y )
{
	if (m_bHide)
		return WMSG_FAIL;

	m_bDrag = false;

	return WMSG_FAIL;
}

WMSG_RESULT CUIMsgBoxNumericOnly::OnMouseMove( UINT16 x, UINT16 y, MSG* pMsg )
{
	if (m_bHide)
		return WMSG_FAIL;

	if( m_bDrag && ( pMsg->wParam & MK_LBUTTON ) )
	{
		int ndX = x - m_nOriX;
		int ndY = y - m_nOriY;

		m_nOriX = x;
		m_nOriY = y;

		Move( ndX, ndY );
		return WMSG_SUCCESS;
	}

	return WMSG_FAIL;
}

void CUIMsgBoxNumericOnly::CloseBox()
{
	Hide(TRUE);
	SetVisible(FALSE);

	UIMGR()->RearrangeOrder(UI_MSGBOX_NUMERIC_ONLY, FALSE);

	CUIBase::CloseProc();
}

void CUIMsgBoxNumericOnly::OnUpdate( float fDeltaTime, ULONG ElapsedTime )
{
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

void CUIMsgBoxNumericOnly::addQueue( Command* pCmd )
{
	CTSingleLock sl(&m_csLock, TRUE);
	m_queue.push(pCmd);
}

void CUIMsgBoxNumericOnly::ConnectCmd( Command* pCmdOk, Command* pCmdCancel /*= NULL*/ )
{
	if (CmdMsgNumericOnlyClose* _pCmdOk = dynamic_cast<CmdMsgNumericOnlyClose*>(m_pCmdOk))
		_pCmdOk->setData(this, pCmdOk);

	if (CmdMsgNumericOnlyClose* _pCmdEnter = dynamic_cast<CmdMsgNumericOnlyClose*>(m_pCmdReturn))
		_pCmdEnter->setData(this, pCmdOk);

	if (CmdMsgNumericOnlyClose* _pCmdClose = dynamic_cast<CmdMsgNumericOnlyClose*>(m_pCmdClose))
		_pCmdClose->setData(this, pCmdCancel);

	if (CmdMsgNumericOnlyClose* _pCmdCancel = dynamic_cast<CmdMsgNumericOnlyClose*>(m_pCmdCancel))
		_pCmdCancel->setData(this, pCmdCancel);
}
