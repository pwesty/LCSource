#include "StdH.h"
#include <Engine/Interface/UIInternalClasses.h>
#include "UIMySyndicateInfo.h"
#include <Engine/Interface/UIManager.h>
#include <Engine/GameDataManager/GameDataManager.h>
#include <Engine/Contents/Base/Syndicate.h>

class CmdSyndiClose : public Command
{
public:
	CmdSyndiClose() : m_pWnd(NULL) {}
	void setData( CUIMySyndicateInfo* pWnd ) { m_pWnd = pWnd; }
	void execute()
	{
		if( m_pWnd )
			m_pWnd->CloseUI();
	}
private:
	CUIMySyndicateInfo* m_pWnd;
};

CUIMySyndicateInfo::CUIMySyndicateInfo(void)
	: m_ptxtKingName(NULL)
	, m_ptxtMyPos(NULL)
	, m_ptxtMyBuff(NULL)
	, m_pbtnClose(NULL)
	, m_pMoveTitle(NULL)
	, m_bDrag(false)
	, m_nOriX(0), m_nOriY(0)
{
	for (int i = 0; i < eSYNDICATE_TYPE_MAX; i++)
	{
		m_pimgSyndicate[i] = NULL;
	}
	setInherit(false);
}

CUIMySyndicateInfo::~CUIMySyndicateInfo(void)
{
	Destroy();
}

void CUIMySyndicateInfo::OpenUI()
{
	if (IsVisible() != FALSE)
		return;

	int nType = _pNetwork->MyCharacterInfo.iSyndicateType;
	if ( nType <= CSyndicate::eTYPE_NONE || nType >= CSyndicate::eTYPE_MAX)
		return;

	CSyndicate* pData = GameDataManager::getSingleton()->GetSyndicate();

	if (pData != NULL)
	{
		pData->SendSyndicateInfo();
	}
#ifdef	VER_TEST
	else
	{
		LOG_DEBUG("CUIMySyndicateInfo::OpenUI() - (pData != NULL)");
	}
#endif	// VER_TEST
	
	UpdateMyInfo();

	SetVisible(TRUE);
	Hide(FALSE);

	SetPos( m_rcOriginPos.Left, m_rcOriginPos.Top );
	
	CUIManager::getSingleton()->RearrangeOrder(UI_MYSYNDICATE_INFO, TRUE);
}

void CUIMySyndicateInfo::CloseUI()
{
	SetVisible(FALSE);
	Hide(TRUE);

	CUIManager::getSingleton()->RearrangeOrder(UI_MYSYNDICATE_INFO, FALSE);
}

void CUIMySyndicateInfo::initialize()
{
	m_pbtnClose = (CUIButton*)findUI("btn_close");

	if (m_pbtnClose != NULL)
	{
		CmdSyndiClose* pCmd = new CmdSyndiClose;
		pCmd->setData(this);
		m_pbtnClose->SetCommandUp(pCmd);
	}

	m_pimgSyndicate[eSYNDICATE_TYPE_KAY] = (CUIImage*)findUI("img_kayinfo");

	if (m_pimgSyndicate[eSYNDICATE_TYPE_KAY] != NULL)
	{
		m_pimgSyndicate[eSYNDICATE_TYPE_KAY]->Hide(TRUE);
	}

	m_pimgSyndicate[eSYNDICATE_TYPE_DEAL] = (CUIImage*)findUI("img_dealinfo");

	if (m_pimgSyndicate[eSYNDICATE_TYPE_DEAL] != NULL)
	{
		m_pimgSyndicate[eSYNDICATE_TYPE_DEAL]->Hide(TRUE);
	}

	CTString strString = CTString("");

	m_ptxtKingName = (CUIText*)findUI("str_king_name");

	if (m_ptxtKingName != NULL)
		m_ptxtKingName->SetText(strString);

	m_ptxtMyPos = (CUIText*)findUI("str_myrvrpos");

	if (m_ptxtMyPos != NULL)
		m_ptxtMyPos->SetText(strString);

	m_ptxtMyBuff = (CUIText*)findUI("str_myrvrbuff");

	if (m_ptxtMyBuff != NULL)
		m_ptxtMyBuff->SetText(strString);

	m_pMoveTitle = (CUIText*)findUI("move_title");
	
}

WMSG_RESULT CUIMySyndicateInfo::OnKeyMessage( MSG* pMsg )
{
	if( pMsg->wParam == VK_ESCAPE )
	{
		CloseUI();
		return WMSG_SUCCESS;
	}

	return WMSG_FAIL;
}

void CUIMySyndicateInfo::UpdateMyInfo()
{
	BOOL bKay = FALSE;

	int nSyndiType = _pNetwork->MyCharacterInfo.iSyndicateType;
	
	if (nSyndiType == CSyndicate::eTYPE_KAILUX)
		bKay = FALSE;
	else if (nSyndiType == CSyndicate::eTYPE_DEALERMOON)
		bKay = TRUE;		
	else
		return;

	if (m_pimgSyndicate[eSYNDICATE_TYPE_KAY] != NULL)
		m_pimgSyndicate[eSYNDICATE_TYPE_KAY]->Hide(bKay);

	if (m_pimgSyndicate[eSYNDICATE_TYPE_DEAL] != NULL)
		m_pimgSyndicate[eSYNDICATE_TYPE_DEAL]->Hide(!bKay);

	GameDataManager* pGameDataManager = GameDataManager::getSingleton();
	CSyndicate* pSyndiInfo = pGameDataManager->GetSyndicate();

	if (pSyndiInfo == NULL)
		return;

	UpdateClient::RVRSyndicateInfoOfServer* pData = pSyndiInfo->GetSyndicateInfo();

	if (pData == NULL)
		return;

	CTString strString = pData->kingName;

	if (m_ptxtKingName != NULL)
		m_ptxtKingName->SetText(strString);
	
	strString = CTString("");
	CTString strTemp1 = CTString("");
	CTString strTemp2 = CTString("");

	int nGrade = _pNetwork->MyCharacterInfo.iSyndicateGrade;

	strTemp1.PrintF(_S(6165, "나의 직위:"));
	strTemp2 = pSyndiInfo->GetGradeName(nSyndiType, nGrade);
	strString = strTemp1 + " " + strTemp2;

	if (m_ptxtMyPos != NULL)
		m_ptxtMyPos->SetText(strString);

	int		nLevel = pData->skillLevel;
	int		nSkillIdx = pData->skillIndex;
	
	CSkill	&rSkill = _pNetwork->GetSkillData( nSkillIdx );

	strTemp1.PrintF( _S(6166,"직위 해택:"));

	if (nSkillIdx <= 0 || nSkillIdx >= _pNetwork->wo_iNumOfSkill)
		strTemp2.PrintF( _S(3865,"없음"));
	else
		strTemp2.PrintF( _S(6258,"%s %d 레벨"), rSkill.GetName(), nLevel );
	
	strString = strTemp1 + " " + strTemp2;

	if (m_ptxtMyBuff != NULL)
		m_ptxtMyBuff->SetText(strString);
}

WMSG_RESULT CUIMySyndicateInfo::OnLButtonDown( UINT16 x, UINT16 y )
{
	if (m_bHide)
		return WMSG_FAIL;

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	if( m_pMoveTitle && m_pMoveTitle->IsInside(x, y))
	{
		m_bDrag = true;
		m_nOriX = x;
		m_nOriY = y;
	}

	CUIManager::getSingleton()->RearrangeOrder(UI_MYSYNDICATE_INFO, TRUE);

	return WMSG_FAIL;
}

WMSG_RESULT CUIMySyndicateInfo::OnLButtonUp( UINT16 x, UINT16 y )
{
	if (m_bHide)
		return WMSG_FAIL;

	m_bDrag = false;

	CUIManager::getSingleton()->ResetHoldBtn();
	return WMSG_FAIL;
}

WMSG_RESULT CUIMySyndicateInfo::OnMouseMove( UINT16 x, UINT16 y, MSG* pMsg )
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

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	CUIManager::getSingleton()->SetMouseCursorInsideUIs();

	return WMSG_FAIL;
}

void CUIMySyndicateInfo::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
	m_rcOriginPos.SetRect(m_nPosX, m_nPosY, m_nWidth, m_nHeight);
}

void CUIMySyndicateInfo::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}
