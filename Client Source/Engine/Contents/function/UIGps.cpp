
#include "StdH.h"
#include "UIGps.h"
#include <Engine/Interface/UIButton.h>
#include <Engine/Interface/UIEditBox.h>
#include <Engine/Interface/UITextBox.h>
#include <Engine/Interface/UIText.h>
#include <Engine/Interface/UIImage.h>
#include <Engine/GameDataManager/GameDataManager.h>
#include <Engine/Contents/function/gps.h>
#include <Engine/Interface/UICommand.h>
#include <Engine/Contents/Base/UIMsgBoxMgr.h>
#include <Engine/Interface/UIInventory.h>

// --------------------------------------------------------------

CUICompass::CUICompass()
	: m_pImgQuestion(NULL)
	, m_pImgCompass(NULL)
	, m_bClick(false)
{
	setInherit(false);

	for (int i = 0; i < eCOVER_MAX; ++i)
		m_pImgCover[i] = NULL;
}

void CUICompass::initialize()
{
	m_pImgQuestion = (CUIImage*)findUI("question");
	m_pImgCompass = (CUIImage*)findUI("compass");

	m_pImgCover[eCOVER_NORMAL] = (CUIImage*)findUI("cover");
	m_pImgCover[eCOVER_RED] = (CUIImage*)findUI("cover1");
	m_pImgCover[eCOVER_GREEN] = (CUIImage*)findUI("cover2");

	if (m_pImgCompass != NULL)
	{
		m_pImgCompass->Hide(TRUE);
		m_pImgCompass->EnableRotate(true);
	}
}

void CUICompass::showCompass( bool bShow )
{
	if (bShow == true)
	{
		if (m_pImgQuestion != NULL)
			m_pImgQuestion->Hide(TRUE);
		if (m_pImgCompass != NULL)
			m_pImgCompass->Hide(FALSE);

		setCover(eCOVER_GREEN);
	}
	else
	{
		if (m_pImgQuestion != NULL)
			m_pImgQuestion->Hide(FALSE);
		if (m_pImgCompass != NULL)
			m_pImgCompass->Hide(TRUE);

		setCover(eCOVER_NORMAL);
	}
}

void CUICompass::setAngle( float fAngle )
{
	if (m_pImgCompass != NULL)
		m_pImgCompass->setAngle(fAngle);
}

void CUICompass::Open()
{
	m_bHide = FALSE;
	SetVisible(TRUE);

	UIMGR()->RearrangeOrder(UI_COMPASS, TRUE);
}

void CUICompass::Close()
{
	m_bHide = TRUE;
	SetVisible(FALSE);

	UIMGR()->RearrangeOrder(UI_COMPASS, FALSE);
}

WMSG_RESULT CUICompass::OnLButtonDown( UINT16 x, UINT16 y )
{
	if (m_bHide)
		return WMSG_FAIL;

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	m_bClick = true;

	ox = x;
	oy = y;

	CUIManager::getSingleton()->RearrangeOrder(UI_COMPASS, TRUE);	

	return CUIBase::OnLButtonDown(x, y);
}

WMSG_RESULT CUICompass::OnLButtonUp( UINT16 x, UINT16 y )
{
	if (m_bHide)
		return WMSG_FAIL;

	m_bClick = false;

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	return CUIBase::OnLButtonUp( x, y );
}

WMSG_RESULT CUICompass::OnMouseMove( UINT16 x, UINT16 y, MSG* pMsg )
{
	if (m_bHide)
		return WMSG_FAIL;

	if( m_bClick && ( pMsg->wParam & MK_LBUTTON ) )
	{
		int	ndX = x - ox;
		int	ndY = y - oy;

		ox = x;
		oy = y;

		Move( ndX, ndY );

		return WMSG_SUCCESS;
	}

	if (IsInside(x, y) == FALSE)
	{
		m_bClick = false;
		return WMSG_FAIL;
	}

	CUIManager::getSingleton()->SetMouseCursorInsideUIs();

	return WMSG_FAIL;
}

void CUICompass::setCover( eCOVER eType )
{
	int		i;

	for (i = 0; i < eCOVER_MAX; ++i)
	{
		if (m_pImgCover[i] != NULL)
		{
			if ((eCOVER)i == eType)
				m_pImgCover[i]->Hide(FALSE);
			else
				m_pImgCover[i]->Hide(TRUE);
		}
	}
}

// --------------------------------------------------------------

#define		DEF_UPDATE_GAP		0.1f
#define		DEF_ROTATE_SPEED	35.f	// per 0.1 sec

class CmdGPSFind : public Command
{
public:
	CmdGPSFind() : _pUI(NULL) {}
	void setData(CUIGPS* pUI)	{ _pUI = pUI; }
	void execute() {
		if (_pUI != NULL)
			_pUI->FindUser();
	}

private:
	CUIGPS* _pUI;
};

class CmdGPSClose : public Command
{
public:
	CmdGPSClose() : _pUI(NULL) {}
	void setData(CUIGPS* pUI)	{ _pUI = pUI; }
	void execute() {
		if (_pUI != NULL)
			_pUI->Close();
	}

private:
	CUIGPS* _pUI;
};

CUIGPS::CUIGPS()
	: m_pEdit(NULL)
	, m_pTxtBox(NULL)
	, m_pTxtTitle(NULL)
	, m_bTitleClick(false)
	, m_pImgIn(NULL)
	, m_fDeltaIn(0.f)
{
	setInherit(false);
}

CUIGPS::~CUIGPS()
{
	Destroy();
}

void CUIGPS::initialize()
{
	m_pEdit = (CUIEditBox*)findUI("input");
	m_pTxtBox = (CUITextBox*)findUI("desc");
	m_pTxtTitle = (CUIText*)findUI("title");
	m_pImgIn = (CUIImage*)findUI("in_pattern");

	if (m_pImgIn != NULL)
		m_pImgIn->EnableRotate(true);

	CUIButton* pBtn = (CUIButton*)findUI("btn_ok");
	if (pBtn != NULL)
	{
		CmdGPSFind* pCmd = new CmdGPSFind;
		pCmd->setData(this);
		pBtn->SetCommand(pCmd);
	}

	pBtn = (CUIButton*)findUI("btn_close");
	if (pBtn != NULL)
	{
		CmdGPSClose* pCmd = new CmdGPSClose;
		pCmd->setData(this);
		pBtn->SetCommand(pCmd);
	}

	pBtn = (CUIButton*)findUI("btn_cancel");
	if (pBtn != NULL)
	{
		CmdGPSClose* pCmd = new CmdGPSClose;
		pCmd->setData(this);
		pBtn->SetCommand(pCmd);
	}

	
}

void CUIGPS::Open()
{
	m_bHide = FALSE;
	SetVisible(TRUE);

	UIMGR()->RearrangeOrder(UI_GPS, TRUE);

	UIMGR()->GetInventory()->Lock(TRUE, TRUE, LOCK_GPS);
}

void CUIGPS::Close()
{
	m_bHide = TRUE;
	SetVisible(FALSE);

	UIMGR()->RearrangeOrder(UI_GPS, FALSE);

	if (m_pEdit != NULL)
		m_pEdit->ClearInputBuffer();

	CUIBase::CloseProc();

	UIMGR()->GetInventory()->Lock(FALSE, FALSE, LOCK_GPS);
}

void CUIGPS::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
}

BOOL CUIGPS::IsEditBoxFocused()
{
	if (m_pEdit != NULL)
	{
		return m_pEdit->IsFocused();
	}

	return FALSE;
}

void CUIGPS::FindUser()
{
	if (m_pEdit != NULL)
	{
		char* pName = m_pEdit->GetString();
		if (pName != NULL)
			GAMEDATAMGR()->GetGPS()->findUser(pName);

		m_pEdit->ClearInputBuffer();
	}
}

void CUIGPS::OnUpdate( float fDeltaTime, ULONG ElapsedTime )
{
	if (m_bHide == FALSE)
	{
		if (m_pImgIn != NULL)
		{
			m_fDeltaIn += fDeltaTime;

			if (m_fDeltaIn > DEF_UPDATE_GAP)
			{				
				float fAngle = m_pImgIn->getAngle();

				fAngle -= (DEF_ROTATE_SPEED * m_fDeltaIn);

				m_fDeltaIn -= floor(m_fDeltaIn / DEF_UPDATE_GAP) * DEF_UPDATE_GAP;
				
				m_pImgIn->setAngle(fAngle);				
			}			
		}
	}
}

WMSG_RESULT CUIGPS::OnLButtonDown( UINT16 x, UINT16 y )
{
	if (m_bHide)
		return WMSG_FAIL;

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	if (m_pTxtTitle && m_pTxtTitle->IsInside(x, y))
	{
		m_bTitleClick = true;

		ox = x;
		oy = y;
	}

	CUIManager::getSingleton()->RearrangeOrder(UI_GPS, TRUE);	

	return CUIBase::OnLButtonDown(x, y);
}

WMSG_RESULT CUIGPS::OnLButtonUp( UINT16 x, UINT16 y )
{
	if (m_bHide)
		return WMSG_FAIL;

	m_bTitleClick = false;

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	CUIManager* pUIManager = CUIManager::getSingleton();

	return CUIBase::OnLButtonUp( x, y );
}

WMSG_RESULT CUIGPS::OnMouseMove( UINT16 x, UINT16 y, MSG* pMsg )
{
	if (m_bHide)
		return WMSG_FAIL;

	if( m_bTitleClick && ( pMsg->wParam & MK_LBUTTON ) )
	{
		int	ndX = x - ox;
		int	ndY = y - oy;

		ox = x;
		oy = y;

		Move( ndX, ndY );

		return WMSG_SUCCESS;
	}

	if (IsInside(x, y) == FALSE)
	{
		m_bTitleClick = false;
		return WMSG_FAIL;
	}

	CUIManager::getSingleton()->SetMouseCursorInsideUIs();

	return WMSG_FAIL;
}

WMSG_RESULT CUIGPS::OnKeyMessage( MSG* pMsg )
{
	if ( pMsg->wParam == VK_RETURN )
	{
		if (m_pEdit != NULL && 
			m_pEdit->GetString() != NULL)
		{
			FindUser();
		}
	}

	return WMSG_FAIL;
}
