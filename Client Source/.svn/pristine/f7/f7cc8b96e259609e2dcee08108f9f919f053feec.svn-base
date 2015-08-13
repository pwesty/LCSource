#include "stdh.h"

#include <Engine/Interface/UIEditBox.h>

#include "UISpinControl.h"

#include <Engine/Interface/UIButton.h>
#include <Engine/Interface/UICommand.h>
#include <Engine/Interface/UITextureManager.h>
#include <Engine/Interface/UIImageSplit.h>

class SpinCmdOnChange : public Command
{
public:
	SpinCmdOnChange() : parent_(NULL) {}

	void setData(CUISpinControl* parent, bool bUp)	{ parent_ = parent; bUp_ = bUp; }
	void execute() 
	{
		if (parent_)
		{
			parent_->MoveData(bUp_);
		}
	}

private:
	CUISpinControl* parent_;
	bool bUp_;
};

CUISpinControl::CUISpinControl()
	: m_nMin(0)
	, m_llMax(0)
	, m_pBackImage(NULL)
	, m_pCmdReturn(NULL)
{
	setInherit(false);

	setType(eUI_CONTROL_SPIN_CONTROL);

	m_nMaxCharCount = 16;
	SetOnlyIntegerMode(TRUE);

	for (int i = 0; i < 2; ++i)
	{
		m_pSpinBtn[i] = NULL;
	}
}

CUISpinControl::~CUISpinControl()
{
	SAFE_DELETE(m_pBackImage);
	SAFE_DELETE(m_pCmdReturn);
}

void CUISpinControl::initialize()
{
	if (m_pSpinBtn[0] = (CUIButton*)findUI("btn_up"))
	{
		SpinCmdOnChange* pCmd = new SpinCmdOnChange;
		pCmd->setData(this, true);
		m_pSpinBtn[0]->SetCommandUp(pCmd);
	}

	if (m_pSpinBtn[1] = (CUIButton*)findUI("btn_down"))
	{
		SpinCmdOnChange* pCmd = new SpinCmdOnChange;
		pCmd->setData(this, false);
		m_pSpinBtn[1]->SetCommandUp(pCmd);
	}

	CUIEditBox::initialize();
}

void CUISpinControl::MoveData( bool bUp )
{
	SQUAD llCurData = _atoi64(GetString());

	if (bUp)
	{
		++llCurData;

		if (llCurData > m_llMax)
			llCurData = m_nMin;
	}
	else
	{
		--llCurData;

		if (llCurData < m_nMin)
			llCurData = m_llMax;
	}

	CTString strTmp;
	strTmp.PrintF("%I64d", llCurData);

	SetString(strTmp.str_String);
}

WMSG_RESULT CUISpinControl::OnMouseWheel( UINT16 x, UINT16 y, int wheel )
{
	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	MoveData(wheel > 0 ? true : false);
	
	return WMSG_SUCCESS;
}

void CUISpinControl::OnRender( CDrawPort* pDraw )
{
	
	if (m_pBackImage != NULL)
	{
		int nX, nY;
		GetAbsPos(nX, nY);

		m_pBackImage->SetPos(nX + m_rcBackPos.Left, nY + m_rcBackPos.Top);

		m_pBackImage->OnRender(pDraw);
	}

	CUIEditBox::OnRender(pDraw);
}

WMSG_RESULT CUISpinControl::OnCharMessage( MSG* pMsg )
{
	extern UBYTE	_abKeysPressed[256];

	// If editbox is not focused
	if( !IsFocused() || m_InValidEditBox )
		return WMSG_FAIL;

	if(m_bOnlyInteger && (pMsg->wParam < 48 || pMsg->wParam > 57))
	{
		return WMSG_SUCCESS;
	}

	InsertChar(m_nCursorIndex, pMsg->wParam);

	if (_atoi64(GetString()) > m_llMax)
	{
		CTString strTmp;
		strTmp.PrintF("%I64d", m_llMax);

		SetString(strTmp.str_String);
	}

	return WMSG_SUCCESS;
}

WMSG_RESULT CUISpinControl::OnKeyMessage( MSG* pMsg )
{
	if( !IsFocused() || m_InValidEditBox )
		return WMSG_FAIL;

	if (pMsg->wParam == VK_RETURN)
	{
		if (m_pCmdReturn != NULL)
		{
			m_pCmdReturn->execute();
			return WMSG_SUCCESS;
		}
	}

	return CUIEditBox::OnKeyMessage(pMsg);
}

void CUISpinControl::SetBackGround( UIRect rc, UIRectUV uv, int nUnit )
{
	if (m_pBackImage == NULL)
		m_pBackImage = new CUIImageSplit;

	if (m_pTexData != NULL)
		m_pBackImage->setTexData(m_pTexData);

	m_rcBackPos = rc;

	m_pBackImage->SetUnit(nUnit);
	m_pBackImage->SetSplitMode(CUIImageSplit::SPLIT_3H);

	m_pBackImage->SetPos(rc.Left,  rc.Top);
	m_pBackImage->SetSize(rc.Right, rc.Bottom);
	m_pBackImage->SetUV(uv);

	m_pBackImage->UpdateSplit();
}

void CUISpinControl::GetBackGround( UIRect& rc, UIRectUV& uv, int& nUnit )
{
	if (m_pBackImage == NULL)
		return;

	rc = m_rcBackPos;
	uv = m_pBackImage->GetAbsUV();
	nUnit = m_pBackImage->GetUnit();
}