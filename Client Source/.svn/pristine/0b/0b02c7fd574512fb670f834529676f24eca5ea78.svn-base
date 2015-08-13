#include "StdH.h"
#include <Engine/Interface/UIInternalClasses.h>
#include "UIRankingSystem.h"
#include <Engine/Interface/UICommand.h>
#include "RankingSystemCmd.h"

#define DEF_SHOW_LIST_ITEM_MAX 10

CUIRankingSystem::CUIRankingSystem()
	: m_pbtnClose(NULL)
	, m_pbtnSearch(NULL)
	, m_pbtnReset(NULL)
	, m_pTopRankList(NULL)
	, m_pBottomRankList(NULL)
	, m_pTopRankListScroll(NULL)
	, m_pBottomRankListScroll(NULL)
	, m_pProgressBox(NULL)
	, m_pProgressBar(NULL)
	, m_pLankType(NULL)
	, m_pEbSearch(NULL)
	, m_pTitle(NULL)
	, m_pMoveTitle(NULL)
	, m_pTopRankMsg(NULL)
	, m_pBottomRankMsg(NULL)
	, m_bTitleClick(false)
	, m_nOriX(0), m_nOriY(0)
	, m_nLastCurSel(-1)

{
	setInherit(false);
	m_eSubType = _ERANKINGEX_SUB_TYPE_END;

	m_nRecvRankTotalCount	= 0;
	m_nRecvRankCount		= 0;
	m_bRecvRank				= FALSE;
}

CUIRankingSystem::~CUIRankingSystem()
{
	Destroy();
}

void CUIRankingSystem::SearchName(CTString cstrName)
{
	if ( m_bRecvRank == TRUE)
		return;

	if(cstrName.Length() > 0)
	{
		if (UIMGR()->checkName(cstrName, 0) == FALSE)
			return;

		m_cstrSearch			= cstrName;
		SetBottomListErrorMsgString();

		if ( m_pBottomRankList )
		{
			m_pBottomRankList->DeleteAllListItem();
		}
		
		if ( m_pEbSearch )
		{
			m_pEbSearch->SetEnable(FALSE);
		}
		
		CheckRecvRankingListAndReq();
	}
}

CTString CUIRankingSystem::GetJobStringByType(UBYTE ubClass)
{
	CTString		cstrTemp;

	switch(ubClass)
	{
	case TITAN:
		cstrTemp	= _S(43, "타이탄");
		break;

	case KNIGHT:
		cstrTemp	= _S(44, "기사");
		break;

	case HEALER:
		cstrTemp	= _S(45, "힐러");
		break;

	case MAGE:
		cstrTemp	= _S(46, "메이지");
		break;

	case ROGUE:
		cstrTemp	= _S(47, "로그");
		break;

	case SORCERER:
		cstrTemp	= _S(48, "소서러");
		break;

	case NIGHTSHADOW:
		cstrTemp	= _S(4410, "나이트셰도우");
		break;

#ifdef CHAR_EX_ROGUE
	case EX_ROGUE:	// [2012/08/27 : Sora] EX로그 추가
		cstrTemp	= _S( 5732, "EX로그" );
		break;
#endif
#ifdef  CHAR_EX_MAGE
	case EX_MAGE:	//2013/01/08 jeil EX메이지 추가 스트링 받으면 추가 수정 필요 
		cstrTemp	= _S( 5820, "아크메이지" );
		break;
#endif

	default:
		break;
	}

	return cstrTemp;
}

CTString CUIRankingSystem::GetRankStringByType(ERANKINGEX_SUB_TYPE eSubType)
{
	switch(eSubType)
	{
	case ERANKINGEX_SUB_TYPE_LEVELMASTER:
		return _S(5189, "랭킹 마스터");

	case ERANKINGEX_SUB_TYPE_LEVELCLASS_TITAN:
		return _S(5190, "타이탄 랭킹");

	case ERANKINGEX_SUB_TYPE_LEVELCLASS_KNIGHT:
		return _S(5191, "나이트 랭킹");

	case ERANKINGEX_SUB_TYPE_LEVELCLASS_HEALER:
		return _S(5192, "힐러 랭킹");

	case ERANKINGEX_SUB_TYPE_LEVELCLASS_MAGE:
		return _S(5193, "메이지 랭킹");

	case ERANKINGEX_SUB_TYPE_LEVELCLASS_ROGUE:
		return _S(5194, "로그 랭킹");

	case ERANKINGEX_SUB_TYPE_LEVELCLASS_SORCERER:
		return _S(5195, "소서러 랭킹");

	case ERANKINGEX_SUB_TYPE_LEVELCLASS_NIGHTSHADOW:
		return _S(5196, "나이트셰도우 랭킹");

#ifdef CHAR_EX_ROGUE
	case ERANKINGEX_SUB_TYPE_LEVELCLASS_EX_ROGUE:	// [2012/08/27 : Sora] EX로그 추가
		return _S( 5737,"EX로그 랭킹");
#endif
#ifdef CHAR_EX_MAGE
	case ERANKINGEX_SUB_TYPE_LEVELCLASS_EX_MAGE:	// 2013/01/08 jeil EX메이지 추가 수정 스트링 받으면 추가 수정 필요 
		return _S( 5825,"아크메이지 랭킹");
#endif
	}

	return "";
}

UCHAR CUIRankingSystem::GetRankingNetworkType()
{
	switch(m_eSubType)
	{
	case ERANKINGEX_SUB_TYPE_LEVELMASTER:
		return 0;

	case ERANKINGEX_SUB_TYPE_LEVELCLASS_TITAN:
	case ERANKINGEX_SUB_TYPE_LEVELCLASS_KNIGHT:
	case ERANKINGEX_SUB_TYPE_LEVELCLASS_HEALER:
	case ERANKINGEX_SUB_TYPE_LEVELCLASS_MAGE:
	case ERANKINGEX_SUB_TYPE_LEVELCLASS_ROGUE:
	case ERANKINGEX_SUB_TYPE_LEVELCLASS_SORCERER:
	case ERANKINGEX_SUB_TYPE_LEVELCLASS_NIGHTSHADOW:
#ifdef CHAR_EX_ROGUE
	case ERANKINGEX_SUB_TYPE_LEVELCLASS_EX_ROGUE:	// [2012/08/27 : Sora] EX로그 추가
#endif
#ifdef CHAR_EX_MAGE
	case ERANKINGEX_SUB_TYPE_LEVELCLASS_EX_MAGE:	// 2013/01/08 jeil EX메이지 추가 
#endif
		return 1;
	}

	return 0;
}

UCHAR CUIRankingSystem::GetRankingNetworkSubType()
{
	switch(m_eSubType)
	{
	case ERANKINGEX_SUB_TYPE_LEVELCLASS_TITAN:
		return TITAN;

	case ERANKINGEX_SUB_TYPE_LEVELCLASS_KNIGHT:
		return KNIGHT;

	case ERANKINGEX_SUB_TYPE_LEVELCLASS_HEALER:
		return HEALER;

	case ERANKINGEX_SUB_TYPE_LEVELCLASS_MAGE:
		return MAGE;

	case ERANKINGEX_SUB_TYPE_LEVELCLASS_ROGUE:
		return ROGUE;

	case ERANKINGEX_SUB_TYPE_LEVELCLASS_SORCERER:
		return SORCERER;

	case ERANKINGEX_SUB_TYPE_LEVELCLASS_NIGHTSHADOW:
		return NIGHTSHADOW;

#ifdef CHAR_EX_ROGUE
	case ERANKINGEX_SUB_TYPE_LEVELCLASS_EX_ROGUE:	// [2012/08/27 : Sora] EX로그 추가
		return EX_ROGUE;
#endif
#ifdef CHAR_EX_MAGE
	case ERANKINGEX_SUB_TYPE_LEVELCLASS_EX_MAGE:	// 2013/01/08 jeil EX메이지 추가 
		return EX_MAGE;
#endif
	}

	return TITAN;
}

void CUIRankingSystem::ChangeViewType(ERANKINGEX_SUB_TYPE eSubType)
{
	m_eSubType	= eSubType;
	if(m_eSubType >= _ERANKINGEX_SUB_TYPE_END)
		m_eSubType	= (ERANKINGEX_SUB_TYPE)(_ERANKINGEX_SUB_TYPE_END - 1);

	if (m_pLankType)
	{
		m_pLankType->SetCurSel((int)m_eSubType);
	}
	
	SetTitle(GetRankStringByType(eSubType));

	// 기본 검색대상은 본인임
	m_cstrSearch	= _pNetwork->MyCharacterInfo.name;

	// 선택된 타입의 랭킹 정보가 들어있는지 체크하고
	// 없다면 랭킹 요청 패킷 전송.
	CheckRecvRankingListAndReq();
}

void CUIRankingSystem::ToggleReset()
{
	if ( m_bRecvRank == TRUE)
		return;

	if (m_pEbSearch)
		m_pEbSearch->ResetString();
		
	if (m_pTopRankList)
		m_pTopRankList->DeleteAllListItem();

	if (m_pBottomRankList)
		m_pBottomRankList->DeleteAllListItem();
	
	m_cstrSearch			= _pNetwork->MyCharacterInfo.name;

	SetTopListErrorMsgString();
	SetBottomListErrorMsgString();
		
	ChangeViewType(m_eSubType);
}

void CUIRankingSystem::SetRankingListSelect( RANK_LIST_TYPE ListType, COLOR col )
{
	int nCurSel = GetListCurSel(ListType);
	
	if ( nCurSel < 0 )
		return;

	CUIListItem* pItem;
	
	switch (ListType)
	{
	case RANKING_LIST_TOP:
		{
			pItem = (CUIListItem*)m_pTopRankList->GetListItem( nCurSel );
		}
		break;

	case RANKING_LIST_BOTTOM:
		{
			pItem = (CUIListItem*)m_pBottomRankList->GetListItem( nCurSel );
		}
		break;
	}

	if ( pItem == NULL)
		return;

	SetRankingListStringCol(pItem, col);
}

void CUIRankingSystem::SetRankingListUnSelect( RANK_LIST_TYPE ListType, COLOR col )
{
	int nCurSel = GetListCurSel(ListType);
	COLOR colorText = col;

	if ( nCurSel < 0 )
		return;

	CUIListItem* pItem;
	SRankingPointData	oRankData;

	switch (ListType)
	{
	case RANKING_LIST_TOP:
		{
			pItem = (CUIListItem*)m_pTopRankList->GetListItem( nCurSel );

			if(m_oRankingList.GetRankPoint(&oRankData, nCurSel, m_eSubType) == FALSE)
				return;
		}
		break;

	case RANKING_LIST_BOTTOM:
		{
			pItem = (CUIListItem*)m_pBottomRankList->GetListItem( nCurSel );

			if(m_oRankingList.GetRankPointSearch(&oRankData, m_cstrSearch, nCurSel, m_eSubType) == FALSE)
				return;
		}
		break;
	}

	if ( pItem == NULL)
		return;

	if(oRankData.cstrCharacterName == _pNetwork->MyCharacterInfo.name) // 자신의 아이디와 같다면 하이라이트
		colorText	= 0xF22020FF;
	else if(oRankData.cstrCharacterName == m_cstrSearch && ListType == RANKING_LIST_BOTTOM)	// 검색 아이디와 같다면 하이라이트
		colorText	= 0x20F220FF;

	SetRankingListStringCol(pItem, colorText);
}

void CUIRankingSystem::SetRankingListMouseEvent( RANK_LIST_TYPE ListType, CUIListItem* pListItem, COLOR col, MOUSE_EVENT_TYPE EventType )
{
	if ( pListItem == NULL )
		return;

	int nCurSel = GetListCurSel(ListType);

	switch( EventType )
	{
	case RANKING_MOUSE_NONE:
		{
			if ( nCurSel == pListItem->getControlIndex() )
				return;
		}
		break;
	}

	SetRankingListStringCol(pListItem, col);
}

int CUIRankingSystem::GetListCurSel( RANK_LIST_TYPE ListType )
{
	switch ( ListType )
	{
	case RANKING_LIST_TOP:
		{
			if (m_pTopRankList)
			{
				return m_pTopRankList->getCurSel();
			}
		}
		break;

	case RANKING_LIST_BOTTOM:
		{
			if (m_pBottomRankList)
			{
				return m_pBottomRankList->getCurSel();
			}
		}
		break;
	}

	return -1;
}

void CUIRankingSystem::SetRankingListStringCol( CUIListItem* pListItem, COLOR col )
{
	if ( pListItem == NULL )
		return;

	int nMax = pListItem->getChildCount();
	CUIText* pTex;

	for (int i = 0; i < nMax; i++)
	{
		pTex = (CUIText*)pListItem->getChildAt(i);

		if ( pTex != NULL)
		{
			pTex->setFontColor(col);
		}
	}
}

void CUIRankingSystem::ClearRankingList()
{
	m_oRankingList.ClearRankingPoint();
	m_oRankingList.ClearRankingPointSearch();
	m_oRankingList.ClearRankingGuild();
	m_oRankingList.ClearRankingGuildSearch();
}

void CUIRankingSystem::ToggleVisible()
{
	if(IsVisible() == FALSE)
	{
		OpenUI(ERANKINGEX_SUB_TYPE_LEVELMASTER);
	}
	else
	{
		CloseUI();
	}
}

void CUIRankingSystem::OpenUI()
{
	CUIManager::getSingleton()->RearrangeOrder(UI_RANKINGVIEW_RENEWAL, TRUE);

	ChangeViewType(m_eSubType);
}

void CUIRankingSystem::OpenUI(ERANKINGEX_SUB_TYPE eSubType)
{
	m_eSubType		= eSubType;
	OpenUI();
}

void CUIRankingSystem::CloseUI()
{
	if ( m_pEbSearch )
	{
		m_pEbSearch->ResetString();
		m_pEbSearch->SetFocus(FALSE);
	}
	m_cstrSearch			= _pNetwork->MyCharacterInfo.name;

	CUIFocus::getSingleton()->setUI(NULL);	// 창을 닫을 때 포커스가 있다면, 초기화

	SetTopListErrorMsgString();
	SetBottomListErrorMsgString();
	
	if ( m_pTopRankList )
		m_pTopRankList->DeleteAllListItem();

	if ( m_pBottomRankList )
		m_pBottomRankList->DeleteAllListItem();

	m_bRecvRank				= FALSE;

	if ( m_pLankType )
		m_pLankType->SetCurSel(0);

	if ( m_pProgressBox )
		m_pProgressBox->Hide( !m_bRecvRank );

	if ( m_pProgressBar )
		m_pProgressBar->Hide( !m_bRecvRank );

	CUIManager::getSingleton()->RearrangeOrder(UI_RANKINGVIEW_RENEWAL, FALSE);
}

void CUIRankingSystem::UpdateTopRanking()
{
	if (m_pTopRankList)
	{
		m_pTopRankList->DeleteAllListItem();
	}

	CUIListItem* pListItem;
	CUIListItem* pListTempItem;
	
	// 길드는 아직 적용하지 않음
	if(m_eSubType != ERANKINGEX_SUB_TYPE_GUILD)
	{	
		int		nTotal	= m_oRankingList.GetCountRankPoint(m_eSubType);

		if(nTotal <= 0)
			return;

		if (m_pTopRankList == NULL)
			return;

		pListItem = m_pTopRankList->GetListItemTemplate();

		if ( pListItem == NULL )
			return;

		for(int i = 0; i < nTotal; i++)
		{
			SRankingPointData	oRankData;

			if(m_oRankingList.GetRankPoint(&oRankData, i, m_eSubType) == FALSE)
				return;

			pListTempItem = (CUIListItem*)pListItem->Clone();
			m_pTopRankList->AddListItem((CUIBase*)pListTempItem);

			pListTempItem = (CUIListItem*)m_pTopRankList->GetListItem(i);

			if ( pListTempItem == NULL )
				return;
			COLOR			colorText	= DEF_UI_FONT_COLOR;

			if(oRankData.cstrCharacterName == _pNetwork->MyCharacterInfo.name)		// 자신의 아이디와 같다면 하이라이트
				colorText	= 0xF22020FF;

			CmdRankingMouseEvent* pCmdOver = new CmdRankingMouseEvent;
			pCmdOver->setData( this, RANKING_LIST_TOP, pListTempItem, 0xF8E1B5FF, RANKING_MOUSE_OVER);
			pListTempItem->SetCommandEnter(pCmdOver);

			CmdRankingMouseEvent* pCmdLeave = new CmdRankingMouseEvent;
			pCmdLeave->setData( this, RANKING_LIST_TOP, pListTempItem, colorText, RANKING_MOUSE_NONE);
			pListTempItem->SetCommandLeave(pCmdLeave);

			SetListString(&oRankData, pListTempItem, colorText);

		}

		m_pTopRankList->UpdateList();
		m_pTopRankList->UpdateScroll(nTotal);
	}	
}

void CUIRankingSystem::UpdateBottomRanking()
{
	static ERANKINGEX_SUB_TYPE	msgLastSubType	= _ERANKINGEX_SUB_TYPE_END;

	if (m_pBottomRankList)
	{
		m_pBottomRankList->DeleteAllListItem();
	}

	CUIListItem* pListItem;
	CUIListItem* pListTempItem;
		
	msgLastSubType	= m_eSubType;

	if(m_eSubType != ERANKINGEX_SUB_TYPE_GUILD)
	{
		int		nTotal	= m_oRankingList.GetCountRankPointSearch(m_cstrSearch, m_eSubType);

		if(nTotal <= 0)
			return;

		if (m_pBottomRankList == NULL)
			return;

		pListItem = m_pBottomRankList->GetListItemTemplate();

		if ( pListItem == NULL )
			return;

		for(int i = 0; i < nTotal; i++)
		{
			SRankingPointData	oRankData;

			if(m_oRankingList.GetRankPointSearch(&oRankData, m_cstrSearch, i, m_eSubType) == FALSE)
				return;

			pListTempItem = (CUIListItem*)pListItem->Clone();
			m_pBottomRankList->AddListItem(pListTempItem);

			pListTempItem = (CUIListItem*)m_pBottomRankList->GetListItem(i);

			if ( pListTempItem == NULL )
				return;

			COLOR			colorText	= 0xF2F2F2FF;

			if(oRankData.cstrCharacterName == _pNetwork->MyCharacterInfo.name)		// 자신의 아이디와 같다면 하이라이트
				colorText	= 0xF22020FF;
			else if(oRankData.cstrCharacterName == m_cstrSearch)					// 검색 아이디와 같다면 하이라이트
				colorText	= 0x20F220FF;

			CmdRankingMouseEvent* pCmdOver = new CmdRankingMouseEvent;
			pCmdOver->setData( this, RANKING_LIST_BOTTOM, pListTempItem, 0xF8E1B5FF, RANKING_MOUSE_OVER);
			pListTempItem->SetCommandEnter(pCmdOver);

			CmdRankingMouseEvent* pCmdLeave = new CmdRankingMouseEvent;
			pCmdLeave->setData( this, RANKING_LIST_BOTTOM, pListTempItem, colorText, RANKING_MOUSE_NONE);
			pListTempItem->SetCommandLeave(pCmdLeave);

			SetListString(&oRankData, pListTempItem, colorText);
		}

		m_pBottomRankList->UpdateList();
		m_pBottomRankList->UpdateScroll(nTotal);
	}
}

void CUIRankingSystem::initialize()
{
	m_pbtnClose = (CUIButton*)findUI("btn_close");

	if ( m_pbtnClose != NULL )
	{
		CmdRankingClose* pCmd = new CmdRankingClose;
		pCmd->setData(this);
		m_pbtnClose->SetCommandUp( pCmd );
	}

	m_pbtnReset = (CUIButton*)findUI("btn_reset");
	
	if ( m_pbtnReset != NULL )
	{
		CmdRankingReset* pCmd = new CmdRankingReset;
		pCmd->setData(this);
		m_pbtnReset->SetCommandUp( pCmd );
	}

	m_pTopRankList = (CUIList*)findUI("list_toprank");
	
	if ( m_pTopRankList != NULL )
	{
		CmdRankingListUnSelect* pCmdUnClick = new CmdRankingListUnSelect;
		pCmdUnClick->setData( this, RANKING_LIST_TOP, DEF_UI_FONT_COLOR);
		m_pTopRankList->SetCommandUn(pCmdUnClick);

		CmdRankingListSelect* pCmdClick = new CmdRankingListSelect;
		pCmdClick->setData( this, RANKING_LIST_TOP, 0xF8E1B5FF);
		m_pTopRankList->SetCommand(pCmdClick);
	}

	m_pBottomRankList = (CUIList*)findUI("list_bottomrank");

	if ( m_pBottomRankList != NULL )
	{
		CmdRankingListUnSelect* pCmdUnClick = new CmdRankingListUnSelect;
		pCmdUnClick->setData( this, RANKING_LIST_BOTTOM, DEF_UI_FONT_COLOR);
		m_pBottomRankList->SetCommandUn(pCmdUnClick);

		CmdRankingListSelect* pCmdClick = new CmdRankingListSelect;
		pCmdClick->setData( this, RANKING_LIST_BOTTOM, 0xF8E1B5FF);
		m_pBottomRankList->SetCommand(pCmdClick);
	}

	m_pTopRankListScroll = (CUIScrollBar*)findUI("list_scroll");
	m_pBottomRankListScroll = (CUIScrollBar*)findUI("list_scroll");

	m_pProgressBox = (CUIImage*)findUI("im_progressBox");
	m_pProgressBar = (CUIProgressBar*)findUI("pb_progress");

	m_pLankType = (CUIComboBox*)findUI("cb_ranktype");

	if ( m_pLankType != NULL )
	{
		for(int eType = ERANKINGEX_SUB_TYPE_LEVELMASTER; eType < _ERANKINGEX_SUB_TYPE_END; eType++)
		{
			m_pLankType->AddString(GetRankStringByType((ERANKINGEX_SUB_TYPE)eType));
		}
	}

	m_pEbSearch = (CUIEditBox*)findUI("eb_search");

	m_pbtnSearch = (CUIButton*)findUI("btn_serch");

	if ( m_pbtnSearch != NULL )
	{
		CmdRankingSearch* pCmd = new CmdRankingSearch;

		if (m_pEbSearch)
			pCmd->setData(this, m_pEbSearch);
		else
			pCmd->setData(this, NULL);

		m_pbtnSearch->SetCommandUp( pCmd );
	}

	m_pTitle = (CUIText*)findUI("str_title");
	m_pMoveTitle = (CUIText*)findUI("str_movetitle");
	m_pTopRankMsg = (CUIText*)findUI("strTopRankingMsg");
	m_pBottomRankMsg = (CUIText*)findUI("strBottomRankingMsg");
}

void CUIRankingSystem::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
}

void CUIRankingSystem::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

//////////private function //////////////
void CUIRankingSystem::SetTitle( CTString cstrTitle )
{
	if (m_pTitle)
	{
		m_pTitle->SetText(cstrTitle);
	}
}

void CUIRankingSystem::ChangeComboType()
{
	if ( m_pEbSearch )
		m_pEbSearch->ResetString();

	m_cstrSearch			= _pNetwork->MyCharacterInfo.name;
	m_nLastCurSel			= m_pLankType->GetCurSel();
	SetTopListErrorMsgString();
	SetBottomListErrorMsgString();

	if ( m_pTopRankList )
		m_pTopRankList->DeleteAllListItem();

	if ( m_pBottomRankList )
		m_pBottomRankList->DeleteAllListItem();			

	ChangeViewType((ERANKINGEX_SUB_TYPE)m_pLankType->GetCurSel());
}

void CUIRankingSystem::SetListString( SRankingPointData* pRankData, CUIListItem* pListItem, COLOR color )
{
	CUIText*	pText;
	char		szBuff[64]	= { 0, };
	CTString	cstrTemp;

	// 1 : 이름
	pText = (CUIText*)pListItem->findUI("strName");

	if (pText != NULL)
	{
		pText->SetText(pRankData->cstrCharacterName);
		pText->setFontColor(color);
	}

	// 2 : 전주 랭킹
	_itoa(pRankData->nLastRank, szBuff, 10);
	cstrTemp	= szBuff;

	pText = (CUIText*)pListItem->findUI("strlastrank");

	if (pText != NULL)
	{
		pText->SetText(cstrTemp);
		pText->setFontColor(color);
	}

	// 3 : 금주 랭킹
	_itoa(pRankData->nThisRank, szBuff, 10);
	cstrTemp	= szBuff;

	pText = (CUIText*)pListItem->findUI("strthisrank");

	if (pText != NULL)
	{
		pText->SetText(cstrTemp);
		pText->setFontColor(color);
	}

	// 4 : 전주 달성
	_itoa(pRankData->nLastPoint, szBuff, 10);
	cstrTemp	= szBuff;

	pText = (CUIText*)pListItem->findUI("strlastpoint");

	if (pText != NULL)
	{
		pText->SetText(cstrTemp);
		pText->setFontColor(color);
	}

	// 5 : 금주 달성
	_itoa(pRankData->nThisPoint, szBuff, 10);
	cstrTemp	= szBuff;

	pText = (CUIText*)pListItem->findUI("strthispoint");

	if (pText != NULL)
	{
		pText->SetText(cstrTemp);
		pText->setFontColor(color);
	}

	// 6 : 레벨
	_itoa(pRankData->nLevel, szBuff, 10);
	cstrTemp	= szBuff;

	pText = (CUIText*)pListItem->findUI("strlevel");

	if (pText != NULL)
	{
		pText->SetText(cstrTemp);
		pText->setFontColor(color);
	}

	// 7 : 직업
	pText = (CUIText*)pListItem->findUI("strjob");

	if (pText != NULL)
	{
		pText->SetText(GetJobStringByType(pRankData->ubClass));
		pText->setFontColor(color);
	}

	// 8 : 달성날짜
	pText = (CUIText*)pListItem->findUI("strdate");

	if (pText != NULL)
	{
		pText->SetText(pRankData->cstrFixedDate);
		pText->setFontColor(color);
	}
}

void CUIRankingSystem::SetTopListErrorMsgString( CTString strString /*= ""*/, BOOL bHide /*= TRUE*/)
{
	if ( m_pTopRankMsg )
	{
		m_pTopRankMsg->SetText( strString );
		m_pTopRankMsg->Hide(bHide);
	}
}

void CUIRankingSystem::SetBottomListErrorMsgString( CTString strString /*= ""*/, BOOL bHide /*= TRUE*/)
{
	if ( m_pBottomRankMsg )
	{
		m_pBottomRankMsg->SetText( strString );
		m_pBottomRankMsg->Hide(bHide);
	}
}

///////////// Msg Proc //////////////
WMSG_RESULT CUIRankingSystem::OnLButtonDown( UINT16 x, UINT16 y )
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

	if(m_bRecvRank == TRUE)
		return WMSG_SUCCESS;

	if ( m_pEbSearch && m_pEbSearch->OnLButtonDown(x, y) )
	{
		m_pEbSearch->SetFocus(TRUE);
		return WMSG_SUCCESS;
	}

	CUIManager::getSingleton()->RearrangeOrder(UI_RANKINGVIEW_RENEWAL, TRUE);

	return WMSG_FAIL;
}

WMSG_RESULT CUIRankingSystem::OnLButtonUp( UINT16 x, UINT16 y )
{
	if (m_bHide)
		return WMSG_FAIL;

	m_bDrag = false;

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	if ( m_pLankType && m_pLankType->IsInside(x, y) )
	{
		if ( m_nLastCurSel != m_pLankType->GetCurSel() )
		{
			ChangeComboType();
		}
	}
	return WMSG_FAIL;
}

WMSG_RESULT CUIRankingSystem::OnMouseMove( UINT16 x, UINT16 y, MSG* pMsg )
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

	CUIManager::getSingleton()->SetMouseCursorInsideUIs();

	if ( m_bRecvRank == TRUE )
		return WMSG_SUCCESS;

	return WMSG_FAIL;
}

WMSG_RESULT CUIRankingSystem::OnKeyMessage( MSG* pMsg )
{
	if(m_bRecvRank == TRUE)
		return WMSG_SUCCESS;

	if( m_pEbSearch == NULL )
		return WMSG_FAIL;

	if ( m_pEbSearch->IsFocused() == FALSE )
		return WMSG_FAIL;

	switch( pMsg->wParam )
	{
	case VK_RETURN:
		{
			SearchName( m_pEbSearch->GetString() );
		}
		break;
	}
	return WMSG_FAIL;
}

WMSG_RESULT CUIRankingSystem::OnCharMessage( MSG* pMsg )
{
	if(m_bRecvRank == TRUE)
		return WMSG_SUCCESS;

	if( m_pEbSearch == NULL )
		return WMSG_FAIL;

	if ( m_pEbSearch->IsFocused() == FALSE )
		return WMSG_FAIL;

	return WMSG_FAIL;
}

WMSG_RESULT CUIRankingSystem::OnIMEMessage( MSG* pMsg )
{
	if(m_bRecvRank == TRUE)
		return WMSG_SUCCESS;

	if( m_pEbSearch == NULL )
		return WMSG_FAIL;

	if ( m_pEbSearch->IsFocused() == FALSE )
		return WMSG_FAIL;

	return WMSG_FAIL;
}
//////////// NetWork ////////////////
BOOL CUIRankingSystem::CheckRecvRankingListAndReq()
{
	if(m_oRankingList.GetCountRankPoint(m_eSubType) <= 0)
	{
		m_bRecvRank				= TRUE;
		m_nRecvRankCount		= 0;
		m_nRecvRankTotalCount	= 0;
		if (m_pProgressBar)
		{
			m_pProgressBar->SetProgressRange(0, 100);
			m_pProgressBar->OffsetProgressPos(100);
		}

		if ( m_pProgressBox )
			m_pProgressBox->Hide( !m_bRecvRank );

		if ( m_pProgressBar )
			m_pProgressBar->Hide( !m_bRecvRank );

		_pNetwork->SendRankingListEx(GetRankingNetworkType(), GetRankingNetworkSubType());
		return FALSE;
	}
	else if(m_oRankingList.GetCountRankPoint(m_eSubType) > 0)
	{
		UpdateTopRanking();
	}

	if(m_cstrSearch.Length() > 0 && m_oRankingList.GetCountRankPointSearch(m_cstrSearch, m_eSubType) <= 0)
	{
		m_bRecvRank				= TRUE;
		m_nRecvRankCount		= 0;
		m_nRecvRankTotalCount	= 0;
		if (m_pProgressBar)
		{
			m_pProgressBar->SetProgressRange(0, 100);
			m_pProgressBar->OffsetProgressPos(100);
		}

		if ( m_pProgressBox )
			m_pProgressBox->Hide( !m_bRecvRank );

		if ( m_pProgressBar )
			m_pProgressBar->Hide( !m_bRecvRank );

		_pNetwork->SendRankingSearchListEx(GetRankingNetworkType(), GetRankingNetworkSubType(), m_cstrSearch);
		return FALSE;
	}
	else if(m_oRankingList.GetCountRankPointSearch(m_cstrSearch, m_eSubType) > 0)
	{
		UpdateBottomRanking();
		if (m_pEbSearch)
		{
			m_pEbSearch->SetEnable(TRUE);	
		}
	}

	m_bRecvRank			= FALSE;

	if ( m_pProgressBox )
		m_pProgressBox->Hide( !m_bRecvRank );

	if ( m_pProgressBar )
		m_pProgressBar->Hide( !m_bRecvRank );

	return TRUE;
}

void CUIRankingSystem::RecvRankingList(CNetworkMessage& nmMessage)
{
	ERANKINGEX_SUB_TYPE	eSubType	= ERANKINGEX_SUB_TYPE_LEVELMASTER;
	SLONG	iListType				= 0;
	UCHAR	ucType					= 0;
	UCHAR	ucClass					= 0;
	SLONG	nTotalCount				= 0;
	SLONG	nRecvListCount			= 0;

	nmMessage >> (SLONG)iListType;		// (INT)MSG_EX_RANKING_SYSTEM_EX_TYPE
	switch(iListType)
	{
	case MSG_EX_RANKING_LIST:		// (s->c) (uc)type, (uc)subtype, (int)total_count, (int)list_count, (str)name, (int)ranking, (int)ranking_pre, (int)exp, (int)exp_pre, (int)level, (uc)job, (str)date.....
		{
			nmMessage >> (UCHAR)ucType;			// (uc)type
			nmMessage >> (UCHAR)ucClass;		// (uc)subtype
			nmMessage >> (SLONG)nTotalCount;	// (int)total_count
			nmMessage >> (SLONG)nRecvListCount;	// (int)list_count
		}
		break;

	case MSG_EX_RANKING_SEARCH:		// (s->c) (uc)type, (uc)subtype, (int)list_count, (str)name, (int)ranking, (int)ranking_pre, (int)exp, (int)exp_pre, (int)level, (uc)job, (str)date.....
		{
			nmMessage >> (UCHAR)ucType;			// (uc)type
			nmMessage >> (UCHAR)ucClass;		// (uc)subtype
			nmMessage >> (SLONG)nRecvListCount;	// (int)list_count

			nTotalCount		= nRecvListCount;
		}
		break;

	case MSG_EX_RANKING_REFRESH:	// (s->c)
		{
			ClearRankingList();
			return;
		}
		break;

	case MSG_EX_RANKING_ERROR:		// (s->c) (uc)type
		{
			nmMessage >> (UCHAR)ucType;			// (uc)type

			switch(ucType)
			{
			case MSG_EX_RANKING_SYSTEM_ERROR:
				{
					SetTopListErrorMsgString( _S(5581, "랭킹 서버 오류입니다."), FALSE );
					
					if ( m_pTopRankList )
						m_pTopRankList->DeleteAllListItem();

					if ( m_pBottomRankList )
						m_pBottomRankList->DeleteAllListItem();
				}
				break;

			case MSG_EX_RANKING_SYSTEM_ERROR_NOT_EXIST:
				{
					SetBottomListErrorMsgString( _S(5574, "해당 캐릭터에 대한 랭킹 정보가 없습니다."), FALSE );
				}				
				break;

			case MSG_EX_RANKING_SYSTEM_ERROR_REFRESH_TIME:
				{
					SetTopListErrorMsgString( _S(5575, "랭킹이 갱신된 이후에 다시 확인 해주십시오."), FALSE );
					
					if ( m_pTopRankList )
						m_pTopRankList->DeleteAllListItem();

					if ( m_pBottomRankList )
						m_pBottomRankList->DeleteAllListItem();
				}
				break;
			}

			if ( m_pEbSearch)
				m_pEbSearch->SetEnable(TRUE);

			m_bRecvRank		= FALSE;

			if ( m_pProgressBox )
				m_pProgressBox->Hide( !m_bRecvRank );

			if ( m_pProgressBar )
				m_pProgressBar->Hide( !m_bRecvRank );

			return;
		}
		break;
	}

	if(nTotalCount <= 0 || nRecvListCount <= 0)
		return;

	// 요청받은 리스트의 서브 카테고리 (레벨마스터, 각 클래스)
	if(ucType == 0)
	{	// 레벨마스터
		eSubType	= ERANKINGEX_SUB_TYPE_LEVELMASTER;
	}
	else if(ucType == 1)
	{	// 각 클래스 랭크
		eSubType	= (ERANKINGEX_SUB_TYPE)(ucClass + 1);
	}

	for(int i = 0; i < nRecvListCount; i++)
	{
		CTString	cstrName;
		SLONG		nRanking;
		SLONG		nRankingLast;
		SLONG		nPoint;
		SLONG		nPointLast;
		SLONG		nLevel;
		UCHAR		ucJob;
		CTString	cstrDate;

		nmMessage >> cstrName;			// (str)name
		nmMessage >> (SLONG)nRanking;		// (int)ranking
		nmMessage >> (SLONG)nRankingLast;	// (int)ranking_pre
		nmMessage >> (SLONG)nPoint;		// (int)exp
		nmMessage >> (SLONG)nPointLast;	// (int)exp_pre
		nmMessage >> (SLONG)nLevel;		// (int)level
		nmMessage >> (UCHAR)ucJob;		// (uc)job
		nmMessage >> cstrDate;			// (str)date


		SRankingPointData	oData;

		oData.cstrCharacterName		= cstrName;
		oData.nThisRank				= nRanking;
		oData.nLastRank				= nRankingLast;
		oData.nThisPoint			= nPoint;
		oData.nLastPoint			= nPointLast;
		oData.nLevel				= nLevel;
		oData.ubClass				= ucJob;
		// 날짜 포멧 변환
		if(cstrDate.Length() > 0 && cstrDate.str_String[0] != NULL)
		{
			char	szBuff[32]		= { 0, };
			int		iYear, iMonth, iDay, iHour, iMin, iSec;
			sscanf(cstrDate, "%d-%d-%d %d:%d:%d", &iYear, &iMonth, &iDay, &iHour, &iMin, &iSec);
			sprintf(szBuff, "%02d-%02d-%04d", iMonth, iDay, iYear);

			oData.cstrFixedDate		= szBuff;
		}

		if(iListType == MSG_EX_RANKING_LIST)
		{
			m_oRankingList.AddRankingPoint(eSubType, oData);
		}
		else if(iListType == MSG_EX_RANKING_SEARCH)
		{
			m_oRankingList.AddRankingPointSearch(eSubType, oData, m_cstrSearch);
		}
	}

	m_nRecvRankCount		+= nRecvListCount;
	m_nRecvRankTotalCount	= nTotalCount;

	if ( m_pProgressBar )
	{
		m_pProgressBar->SetProgressRange(0, m_nRecvRankTotalCount);
		m_pProgressBar->OffsetProgressPos(m_nRecvRankCount);
	}


	if(m_nRecvRankCount >= m_nRecvRankTotalCount)
	{
		CheckRecvRankingListAndReq();
	}
}












