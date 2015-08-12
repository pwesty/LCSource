#include "StdH.h"
#include <Engine/Interface/UIInternalClasses.h>
#include "UIHardCoreWarning.h"
#include <Engine/Interface/UIManager.h>

class CmdWarningClose : public Command
{
public:
	CmdWarningClose() : m_pWnd(NULL) {}
	void setData( CUIHardCoreWarning* pWnd ) { m_pWnd = pWnd; }
	void execute()
	{
		if( m_pWnd )
			m_pWnd->CloseUI();
	}
private:
	CUIHardCoreWarning* m_pWnd;
};

CUIHardCoreWarning::CUIHardCoreWarning(void)
	: m_pimgWarning(NULL)
	, m_pimgCharacterDie(NULL)
	, m_pbtnOK(NULL)
{
	setInherit(false);
}

CUIHardCoreWarning::~CUIHardCoreWarning(void)
{
	Destroy();
}

void CUIHardCoreWarning::OpenUI( eWarningType WarningType )
{
	if (IsVisible() == TRUE)
		return;

	BOOL bDie = TRUE;
	BOOL bWarning = TRUE;

	switch (WarningType)
	{
	case WARNING_TYPE_HARDCORE_NOTICE:
		bWarning = FALSE;
		break;
	case WARNING_TYPE_CHARACTER_DIE:
		bDie = FALSE;
		break;
	default:
		return;
	}

	if (m_pimgWarning != NULL)
		m_pimgWarning->Hide(bWarning);

	if (m_pimgCharacterDie != NULL)
		m_pimgCharacterDie->Hide(bDie);

	SetVisible(TRUE);
	Hide(FALSE);
	
	CUIManager::getSingleton()->RearrangeOrder(UI_HARDCORE_WARNING, TRUE);
}

void CUIHardCoreWarning::CloseUI()
{
	if (m_pimgWarning != NULL)
		m_pimgWarning->Hide(TRUE);

	if (m_pimgCharacterDie != NULL)
		m_pimgCharacterDie->Hide(TRUE);
	
	SetVisible(FALSE);
	Hide(TRUE);

	CUIManager::getSingleton()->RearrangeOrder(UI_HARDCORE_WARNING, FALSE);
}

void CUIHardCoreWarning::initialize()
{
	m_pbtnOK = (CUIButton*)findUI("btn_ok");

	if (m_pbtnOK != NULL)
	{
		CmdWarningClose* pCmd = new CmdWarningClose;
		pCmd->setData(this);
		m_pbtnOK->SetCommandUp(pCmd);
	}

	m_pimgCharacterDie = (CUIImage*)findUI("img_charDie");

	if (m_pimgCharacterDie != NULL)
	{
		m_pimgCharacterDie->Hide(TRUE);
	}

	m_pimgWarning = (CUIImage*)findUI("img_warning");

	if (m_pimgWarning != NULL)
	{
		m_pimgWarning->Hide(TRUE);
	}
}

void CUIHardCoreWarning::OnUpdatePosition()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	int x = 0, y = 0;
	int w, h;

	w = pUIManager->GetWidth();
	h = pUIManager->GetHeight();

	SetSize(w, h);
}

WMSG_RESULT CUIHardCoreWarning::OnKeyMessage( MSG* pMsg )
{
	if ( pMsg->wParam == VK_RETURN )
	{
		CloseUI();
		return WMSG_SUCCESS;
	}
	else if( pMsg->wParam == VK_ESCAPE )
	{
		CloseUI();
		return WMSG_SUCCESS;
	}

	return WMSG_FAIL;
}
