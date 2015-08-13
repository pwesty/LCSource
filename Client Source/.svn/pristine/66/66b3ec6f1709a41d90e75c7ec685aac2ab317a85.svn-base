
#include "StdH.h"
#include <Engine/Interface/UIInternalClasses.h>
#include "UITab.h"
#include <Engine/Base/Command.h>




CUITabPage::CUITabPage()
{
	m_eType = eUI_CONTROL_TABPAGE;
}

void CUITabPage::OnRender( CDrawPort* pDraw )
{
#ifdef	UI_TOOL
	RenderBorder(pDraw);
#endif	// UI_TOOL
}

CUIBase* CUITabPage::Clone()
{
	CUITabPage* pPage = new CUITabPage(*this);

	if( pPage == NULL )
		return NULL;

	pPage->setTexString( getTexString() );

	CUIBase::CloneChild(pPage);

	return (CUIBase*)pPage;
}

//----------------------------------------------------------------

CUITab::CUITab()
	: m_pCurTabBt(NULL)
	, m_pCurTabPage(NULL)
	, m_nCurSel(-1)
{
	m_eType = eUI_CONTROL_TAB;
}


CUIBase* CUITab::Clone()
{
	CUITab* pTab = new CUITab(*this);

	if( pTab == NULL )
		return NULL;

	pTab->setTexString( getTexString() );

	pTab->initialize();
	pTab->deleteChildList();

	vec_uinode_iter iter = m_VecChild.begin();

	while (iter != m_VecChild.end())
	{
		if ((*iter) == NULL)
			continue;

		CUITabPage* pPage = NULL;
		CUICheckButton* pBtn = NULL;

		if ((*iter)->getType() == eUI_CONTROL_TABPAGE)
		{
			pPage = (CUITabPage*)(*iter)->Clone();
			pTab->addChild(pPage);
			pTab->addTabPage( pPage );
		}
		else if((*iter)->getType() == eUI_CONTROL_CHECKBOX)
		{
			pBtn = (CUICheckButton*)(*iter)->Clone();
			pTab->addChild(pBtn);
			pTab->addTabButton( pBtn );

		}
		else
			pTab->addChild((*iter)->Clone());
		
		++iter;
	}

	return pTab;
}


void CUITab::deleteChildList()
{
	m_vecTabBt.clear();
	m_vecTabPage.clear();

	m_VecChild.clear();
}


void CUITab::initialize()
{
	OnChangeTab(0);
}

void CUITab::addTabButton( CUICheckButton* pButton )
{
	if (pButton == NULL)
		return;
	
	pButton->SetCheck(FALSE);
	
	CmdSelectTab* pCmd = new CmdSelectTab;
	pCmd->setData(this, m_vecTabBt.size());
	pButton->SetCommand(pCmd);

	m_vecTabBt.push_back(pButton);
}

void CUITab::addTabPage( CUITabPage* pPage )
{
	if (pPage == NULL)
		return;

	pPage->Hide(TRUE);
	m_vecTabPage.push_back(pPage);
}

void CUITab::OnRender( CDrawPort* pDraw )
{
#ifdef	UI_TOOL
	RenderBorder(pDraw);
#endif	// UI_TOOL
}

void CUITab::OnChangeTab( int idx )
{
	if (m_vecTabBt.size() == 0 || m_vecTabPage.size() == 0)
		return;

	if (m_nCurSel == idx)
	{
		// 같은 버튼을 누를 경우 상태를 유지하게 한다.
		m_vecTabBt[m_nCurSel]->SetCheck(TRUE);
		return;
	}

	if (m_nCurSel >= 0)
	{
		m_vecTabBt[m_nCurSel]->SetCheck(FALSE);
		m_vecTabPage[m_nCurSel]->Hide(TRUE);
	}

	m_nCurSel = idx;

	m_vecTabBt[m_nCurSel]->SetCheck(TRUE);
	m_vecTabPage[m_nCurSel]->Hide(FALSE);

	// 탭이 바뀌면, 포커스를 빼주자.
	CUIFocus::getSingleton()->setUI(NULL);

	// 창이 바뀌었을 경우 명령 실행
	if (m_pCmd != NULL)
		m_pCmd->execute();
}

CUICheckButton* CUITab::getCheckBtn( int nIdx )
{
	if (nIdx < 0 || nIdx >= m_vecTabBt.size())
		return NULL;

	return m_vecTabBt[nIdx];
}




