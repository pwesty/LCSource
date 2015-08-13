#include "StdH.h"
#include <Engine/Interface/UIInternalClasses.h>
#include "UIPetStash.h"
#include <Engine/Contents/Base/PetStash.h>
#include <Engine/GameDataManager/GameDataManager.h>
#include <Engine/Network/CNetwork.h>
#include <Engine/Help/Util_Help.h>
#include <Engine/Interface/UICommand.h>
#include <Engine/Interface/UIInventory.h>
#include <string.h>
#include "PetStashCmd.h"

#define DEF_MAX_ITEM_SHOWCOUNT 10
#define DEF_PETSTASH_MAX_COUNT 20
#define DEF_MAX_PET_NAME_UI_SIZE 98

CUIPetStash::CUIPetStash()
	: m_pTab(NULL)
	, m_pTxtTitle(NULL)
	, m_pMoveTitle(NULL)
	, m_pTxtReuseTime(NULL)
	, m_pbtnClose(NULL)
	, m_pbtnInStash(NULL)
	, m_pbtnOutStash(NULL)
	, m_pTxtPetCount(NULL)
	, m_pListPetInfo(NULL)
	, m_pbtnSelect(NULL)
	, m_pIconSelectPet(NULL)
	, m_pTxtPetName(NULL)
	, m_pTxtPetLevel(NULL)
	, m_pTxtPetExp(NULL)
	, m_pTxtPetLoyalty(NULL)
	, m_pTxtPetHungry(NULL)
	, m_pPetArray(NULL)
	, m_pbtnPetstash(NULL)
	, m_pbtnPetDeputy(NULL)
	, m_bTitleClick(false)
	, m_nOriX(0), m_nOriY(0)
	, m_pListScroll(NULL)
{
	setInherit(false);
}

CUIPetStash::~CUIPetStash()
{
	Destroy();
}

//----------------------------------------------------------------------------
// interface

void CUIPetStash::UpdateUI()
{
	UpdateReuseTime();

	if(_UpdatePetListStash() == FALSE)
	{
		LOG_DEBUG("펫창고 리스트 업데이트 실패");
	}

	if(_UpdateDeputyList() == FALSE)
	{
		LOG_DEBUG("대리육성 리스트 업데이트 실패");
	}
}

void CUIPetStash::UpdateReuseTime()
{
	GameDataManager* pGame = GameDataManager::getSingleton();

	if (pGame == NULL)
		return;

	PetStash* pPetStash = pGame->GetStashData();

	if (pPetStash == NULL)
		return;

	CTString strTooltip;
	COLOR col;

	if ( _pUIBuff->GetMyTimerItemBuffExist(TIMERITEM_TYPE_PET_STASH) == true )
	{
		int	nTime	= _pUIBuff->GetMyTimerItemBuffExpireTime(TIMERITEM_TYPE_PET_STASH);

		tm*	pTimeEnd = NULL;
		pTimeEnd = localtime((time_t*)&nTime);
		col = 0xFFFFFFFF;

		if( pTimeEnd != NULL )
		{		
			strTooltip.PrintF( _S( 6070, "만료 : %d년%d월%d일%d시%d분"), pTimeEnd->tm_year + 1900, pTimeEnd->tm_mon + 1, pTimeEnd->tm_mday, pTimeEnd->tm_hour, pTimeEnd->tm_min );
		}

		int nMax = pPetStash->GetPetCount();

		if ( nMax >= DEF_PETSTASH_MAX_COUNT )
		{
			m_pbtnInStash->SetEnable(FALSE);			
		}
		else
		{		
			m_pbtnInStash->SetEnable(TRUE);
		}
	}
	else
	{
		strTooltip = _S( 5956, "펫 창고 이용권을 사용하여 주시길 바랍니다." );
		col = 0xFA0000FF;

		m_pbtnInStash->SetEnable(FALSE);
		m_pbtnSelect->SetEnable(FALSE);
	}		

	m_pTxtReuseTime->SetText( CTString(strTooltip) );
	m_pTxtReuseTime->setFontColor( col );
}

void CUIPetStash::ClosePetStash()
{
	SetVisible(FALSE);
	Hide(TRUE);

	ClearUI();

	CUIManager* pUIManager = CUIManager::getSingleton();
	pUIManager->RearrangeOrder(UI_PET_STASH, FALSE);
	pUIManager->GetInventory()->Lock(FALSE, FALSE, LOCK_PET_STASH);
}

void CUIPetStash::OpenPetStash()
{
	if(IsVisible() == TRUE )
		return;
	CUIManager* pUIManager = CUIManager::getSingleton();

	if (pUIManager->GetInventory()->IsLocked() == TRUE ||
		pUIManager->GetInventory()->IsLockedArrange() == TRUE)
	{
		pUIManager->GetInventory()->ShowLockErrorMessage();
		return;
	}

	UpdateUI();

	SetVisible(TRUE);
	Hide(FALSE);

	pUIManager->RearrangeOrder(UI_PET_STASH, TRUE);
	pUIManager->GetInventory()->Lock(TRUE, TRUE, LOCK_PET_STASH);

	SetPos( m_rcOriginPos.Left, m_rcOriginPos.Top );
}

void CUIPetStash::ClearUI()
{
	GameDataManager* pGame = GameDataManager::getSingleton();

	if (pGame == NULL)
		return;

	PetStash* pPetStash = pGame->GetStashData();

	if ( pPetStash == NULL )
		return;

	// 탭페이지 1 펫리스트 클리어
	CUIListItem* plistItem = NULL;

	int nMax = pPetStash->GetPetCount();
	int i = 0;

	for ( i = 0; i < nMax; i++ )
	{
		if ( m_pListPetInfo == NULL )
			break;

		plistItem = (CUIListItem*)m_pListPetInfo->GetListItem( i );

		if (plistItem != NULL)
		{
			_ClearPetStashList( plistItem );
		}
	}

	// 탭페이지 2 대리육성 클리어
	CUIArrayItem* pArrayItem = NULL;

	for ( i = 0; i < nMax; i++ )
	{
		if ( m_pPetArray == NULL )
			break;

		pArrayItem = m_pPetArray->GetArrayItem( i );

		if (pArrayItem != NULL)
		{
			_ClearDeputyList( pArrayItem );
		}
	}

	CUITooltipMgr::getSingleton()->setData(NULL);

	_ClearSelectPet();
}

//----------------------------------------------------------------------------
// 커맨드

void CUIPetStash::ChangeTab()
{
	int nSel = m_pTab->getCurSelTab();
	CTString strTemp = CTString("");

	switch( nSel )
	{
	case 0:
		strTemp.PrintF(_S(5954, "펫 창고"));
		break;
	case 1:
		strTemp.PrintF(_S(5955,"펫 대리육성"));
		break;
	}

	m_pTxtTitle->SetText(CTString(strTemp));
}

void CUIPetStash::SelectPet()
{
	GameDataManager* pGame = GameDataManager::getSingleton();

	if (pGame == NULL)
		return;

	PetStash* pPetStash = pGame->GetStashData();

	if (pPetStash == NULL)
		return;

	if ( m_pPetArray != NULL )
	{
		PetStashData* pPetData = pPetStash->GetStashListAt(m_pPetArray->GetSelectIdx());

		if( pPetData != FALSE )
		{
			if (pPetData->virtualIndex == pPetStash->GetProxyPetIndex())
				pPetStash->SendProxyCancelReq( pPetData->virtualIndex );
			else
				pPetStash->SendProxyReq( pPetData->virtualIndex );
		}
	}

	m_pbtnSelect->SetText( _S(5959, "등록"));
	m_pbtnSelect->SetEnable(FALSE);
}

void CUIPetStash::SetListItemClick()
{
	if ( m_pListPetInfo == NULL)
		return;

	int nCurSel = m_pListPetInfo->getCurSel();

	if ( nCurSel >= 0 )
		m_pbtnOutStash->SetEnable(TRUE);
	else
		m_pbtnOutStash->SetEnable(FALSE);
}

void CUIPetStash::SetArrayItemClick()
{
	if ( m_pPetArray == NULL )
		return;

	int nSelectIdx = m_pPetArray->GetSelectIdx();

	// 펫아이콘이 선택 된게 아니라면
	if ( nSelectIdx < 0 )
	{
		m_pbtnSelect->SetText( _S(5959, "등록"));
		m_pbtnSelect->SetEnable(FALSE);
		return;
	}

	PetStash* pPetStash = GameDataManager::getSingleton()->GetStashData();

	if (pPetStash == NULL)
		return;

	PetStashData* pPetData = pPetStash->GetStashListAt( m_pPetArray->GetSelectIdx() );

	// 펫 데이타가 없을 경우
	if (pPetData == NULL)
	{
		m_pbtnSelect->SetText( _S(5959, "등록"));
		m_pbtnSelect->SetEnable(FALSE);
		return;
	}		

	// 대리육성 중인 펫을 클릭 할 경우
	if (pPetStash->GetProxyPetIndex() == pPetData->virtualIndex)
		m_pbtnSelect->SetText( _S(5960, "등록 취소"));
	else
		m_pbtnSelect->SetText( _S(5959, "등록"));

	// 펫 이용권 사용 중 이라면
	if (_pUIBuff->GetMyTimerItemBuffExist(TIMERITEM_TYPE_PET_STASH) == true)
		m_pbtnSelect->SetEnable(TRUE);
	else
		m_pbtnSelect->SetEnable(FALSE);		

}

void CUIPetStash::CreateMSGBox( eMSG_PetStash_Type type )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	switch (type)
	{
	case MSG_TYPE_INSTASH:
		{			
			if( pUIManager->DoesMessageBoxExist( MSGCMD_PET_STASH_INSTASH ) ) return;

			CUIMsgBox_Info	MsgBoxInfo;
			CTString		strMessage[MAX_MSGSTRING];
			strMessage[0] = "";
			strMessage[1] = _S(5958, "보관할 펫을 인벤토리에서 선택해 주시길 바랍니다. (단, 장착중인 펫은 보관 하실 수 없습니다.)");
			MsgBoxInfo.SetMsgBoxInfo( strMessage[0], UMBS_YESNO | UMBS_BUTTONEX, UI_PET_STASH, MSGCMD_PET_STASH_INSTASH );
			MsgBoxInfo.SetBtnType( UBET_ITEM, CItemData::ITEM_ACCESSORY, CItemData::ACCESSORY_WILDPET );
			MsgBoxInfo.AddString( strMessage[1] );

			pUIManager->CreateMessageBox( MsgBoxInfo );
			m_pbtnInStash->SetEnable(FALSE);
		}
		break;

	case MSG_TYPE_OUTSTASH:
		{
			if( pUIManager->DoesMessageBoxExist( MSGCMD_PET_STASH_OUTSTASH ) ) return;

			CUIMsgBox_Info	MsgBoxInfo;
			CTString		strMessage[MAX_MSGSTRING];
			strMessage[0] = "";
			strMessage[1] = _S(5950, "보관 중인 펫을 찾으시겠습니까?");
			MsgBoxInfo.SetMsgBoxInfo( strMessage[0], UMBS_YESNO, UI_PET_STASH, MSGCMD_PET_STASH_OUTSTASH );
			MsgBoxInfo.AddString( strMessage[1] );

			pUIManager->CreateMessageBox( MsgBoxInfo );
			m_pbtnOutStash->SetEnable(FALSE);
		}
		break;
	}
}

//----------------------------------------------------------------------------

void CUIPetStash::initialize()
{
	CTString strTooltip = CTString("");

	// 닫기 버튼
	m_pbtnClose = (CUIButton*)findUI("btn_close");

	if( m_pbtnClose != NULL )
	{
		CmdPetStashClose* pCmd = new CmdPetStashClose;
		pCmd->setData(this);
		m_pbtnClose->SetCommandUp( pCmd );
	}

	// 타이틀
	m_pTxtTitle = (CUIText*)findUI("str_title");
	m_pMoveTitle = (CUIText*)findUI("move_title");

	// 펫 이용권 시간
	m_pTxtReuseTime = (CUIText*)findUI("str_reusetime");

	// 탭
	m_pTab = (CUITab*)findUI( "tab_petstash" );

	if( m_pTab != NULL )
	{
		CmdChangeTab* pCmd = new CmdChangeTab;
		pCmd->setData(this);
		m_pTab->SetCommand( pCmd );
	}
		
	//---------------------------------------------------
	// 탭 페이지 1 펫 리스트

	// 체크 버튼
	m_pbtnPetstash = (CUICheckButton*)findUI("btn_tab_stash");

	if ( m_pbtnPetstash != NULL )
	{		
		strTooltip.PrintF(_S(5954,"펫 창고"));
		m_pbtnPetstash->setTooltip(CTString(strTooltip));
	}

	// 맡기기 버튼
	m_pbtnInStash = (CUIButton*)findUI("btn_InStash");

	if ( m_pbtnInStash != NULL )
	{
		CmdPetMsgStash* pCmd = new CmdPetMsgStash;
		pCmd->setData( this, MSG_TYPE_INSTASH );
		m_pbtnInStash->SetCommandUp( pCmd );
		m_pbtnInStash->SetEnable(FALSE);
	}

	// 찾기 버튼
	m_pbtnOutStash = (CUIButton*)findUI("btn_OutStash");

	if ( m_pbtnOutStash != NULL )
	{
		CmdPetMsgStash* pCmd = new CmdPetMsgStash;
		pCmd->setData( this, MSG_TYPE_OUTSTASH );
		m_pbtnOutStash->SetCommandUp( pCmd );
		m_pbtnOutStash->SetEnable(FALSE);
	}

	// 보관 펫 숫자
	m_pTxtPetCount = (CUIText*)findUI("str_petcount");

	// 펫 리스트
	m_pListPetInfo = (CUIList*)findUI("list_PetInfo");

	if ( m_pListPetInfo != NULL )
	{
		CmdPetListItemClick* pCmd = new CmdPetListItemClick;
		pCmd->setData( this );
		m_pListPetInfo->SetCommand( pCmd );
	}

	m_pListScroll = (CUIScrollBar*)findUI("list_scroll");

	//----------------------------------------------------
	// 탭 페이지 2 대리육성
	m_pbtnPetDeputy = (CUICheckButton*)findUI("btn_tab_deputypromote");

	if (m_pbtnPetDeputy != NULL)
	{		
		strTooltip.PrintF(_S(5955,"펫 대리육성"));
		m_pbtnPetDeputy->setTooltip(CTString(strTooltip));
	}
		
	// 대리육성 등록 버튼
	m_pbtnSelect = (CUIButton*)findUI("btn_select");

	if ( m_pbtnSelect != NULL )
	{
		CmdPetSelect* pCmd = new CmdPetSelect;
		pCmd->setData(this);
		m_pbtnSelect->SetCommandUp( pCmd );
		m_pbtnSelect->SetEnable(FALSE);
	}

	// 등록된 펫 아이콘
	m_pIconSelectPet = (CUIIcon*)findUI("icon_select_pet");

	m_pTxtPetName = (CUIText*)findUI("str_petname");
	m_pTxtPetLevel = (CUIText*)findUI("str_petlevel");
	m_pTxtPetExp = (CUIText*)findUI("str_exp");
	m_pTxtPetLoyalty = (CUIText*)findUI("str_loyalty");
	m_pTxtPetHungry = (CUIText*)findUI("str_hungry");

	// 대리육성 펫 어레이 리스트
	m_pPetArray = (CUIArray*)findUI("array_deputy");

	if ( m_pPetArray != NULL )
	{
		CmdPetArrayItemClick* pCmd = new CmdPetArrayItemClick;
		pCmd->setData( this );
		m_pPetArray->SetCommand( pCmd );
	}
}

void CUIPetStash::MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput )
{
	GameDataManager* pGame = GameDataManager::getSingleton();
	CUIManager* pUIManager = CUIManager::getSingleton();

	if (pGame == NULL || pUIManager == NULL)
		return;

	PetStash* pPetStash = pGame->GetStashData();

	if (pPetStash == NULL)
		return;

	int CharIndex = _pNetwork->MyCharacterInfo.index;

	switch( nCommandCode )
	{
	case MSGCMD_PET_STASH_INSTASH:
		{
			if( bOK )
			{
				CUIIcon* pIcon = pUIManager->GetMessageBox(MSGCMD_PET_STASH_INSTASH)->GetBtnEx();

				if (pIcon != NULL)
				{
					CItems* pItems = pIcon->getItems();

					if (pItems != NULL)
					{
						int	nPetVirtualIndex = pItems->Item_UniIndex;
						pPetStash->SendKeepReq(CharIndex, nPetVirtualIndex);
					}
				}
			}
			else
			{
				int nMax = pPetStash->GetPetCount();

				if ( nMax >= DEF_PETSTASH_MAX_COUNT ||
					_pUIBuff->GetMyTimerItemBuffExist(TIMERITEM_TYPE_PET_STASH) == false )
				{
					m_pbtnInStash->SetEnable(FALSE);
				}
				else
				{		
					m_pbtnInStash->SetEnable(TRUE);
				}
			}
			pUIManager->CloseMessageBox(MSGCMD_PET_STASH_INSTASH);
		}
		break;

	case MSGCMD_PET_STASH_OUTSTASH:
		{
			if( bOK )
			{
				PetStashData* pPetData = pPetStash->GetStashListAt( m_pListPetInfo->getCurSel() );

				if( pPetData != FALSE )
					pPetStash->SendTakeReq( CharIndex, pPetData->virtualIndex );
			}
			else
			{
				m_pbtnOutStash->SetEnable(FALSE);
				m_pListPetInfo->setCurSel(-1);
			}
			pUIManager->CloseMessageBox(MSGCMD_PET_STASH_OUTSTASH);
		}
		break;
	}
}

void CUIPetStash::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
	m_rcOriginPos.SetRect(m_nPosX, m_nPosY, m_nWidth, m_nHeight);
}

void CUIPetStash::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

//----------------------------------------------------------------------------
// MSG Proc
WMSG_RESULT CUIPetStash::OnLButtonDown( UINT16 x, UINT16 y )
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

	CUIManager::getSingleton()->RearrangeOrder(UI_PET_STASH, TRUE);

	return WMSG_FAIL;
}

WMSG_RESULT CUIPetStash::OnLButtonUp( UINT16 x, UINT16 y )
{
	if (m_bHide)
		return WMSG_FAIL;

	m_bDrag = false;

	return WMSG_FAIL;
}

WMSG_RESULT CUIPetStash::OnMouseMove( UINT16 x, UINT16 y, MSG* pMsg )
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

//----------------------------------------------------------------------------
// 탭 페이지 1 펫 리스트

BOOL CUIPetStash::_UpdatePetListStash()
{
	GameDataManager* pGame = GameDataManager::getSingleton();

	if (pGame == NULL)
	{
		LOG_DEBUG("_UpdatePetListStash 펫 창고 데이타메니져 접근 실패");
		return FALSE;
	}

	PetStash* pPetStash = pGame->GetStashData();

	if (pPetStash == NULL)
	{
		LOG_DEBUG("_UpdatePetListStash 펫 창고 데이타 접근 실패");
		return FALSE;
	}

	CUIListItem* pTempItem = NULL;
	CUIListItem* pItem;

	// 리스트 클리어
	m_pListPetInfo->DeleteAllListItem();

	// 펫 갯수 얻기
	int nMax = pPetStash->GetPetCount();
	int listcount = m_pListPetInfo->getListItemCount();

	pTempItem = m_pListPetInfo->GetListItemTemplate();

	// 펫 리스트 아이템 셋팅
	for ( int i = 0; i < nMax; i++ )
	{
		if ( i >= listcount)
		{
			m_pListPetInfo->AddListItem(pTempItem->Clone());
			listcount = m_pListPetInfo->getListItemCount();
		}

		pItem = (CUIListItem*)m_pListPetInfo->GetListItem( i );

		if (pItem == NULL)
			continue;

		if ( _SetPetStashList( pItem, i ) == FALSE )
		{
			LOG_DEBUG(" 펫 셋팅 실패 _UpdatePetListStash %d", i);
		}		
	}

	// 펫 리스트 스크롤 셋팅
	m_pListPetInfo->UpdateList();
	m_pListPetInfo->UpdateScroll(nMax);
	m_pListPetInfo->setCurSel(-1);

	// 펫 저장 갯수 셋팅
	CTString strPetCount;
	strPetCount.PrintF("%d/20", nMax);

	if ( m_pTxtPetCount != NULL )
		m_pTxtPetCount->SetText(strPetCount);

	// 버튼 셋팅
	if (nMax >= DEF_PETSTASH_MAX_COUNT || 
		_pUIBuff->GetMyTimerItemBuffExist(TIMERITEM_TYPE_PET_STASH) == false)
	{
		m_pbtnInStash->SetEnable(FALSE);
	}
	else
	{		
		m_pbtnInStash->SetEnable(TRUE);
	}

	return TRUE;
}

BOOL CUIPetStash::_SetPetStashList(CUIListItem* pListItem, int PetNum)
{
	if ( pListItem == NULL )
	{
		LOG_DEBUG("펫 창고 리스트아이템이 추가가 안됐음");
		return FALSE;
	}

	GameDataManager* pGame = GameDataManager::getSingleton();
	UtilHelp* pHelp = UtilHelp::getSingleton();

	if (pGame == NULL)
	{
		LOG_DEBUG("_SetPetStashList 게임메니져 생성 안됨 ");
		return FALSE;
	}

	if (pHelp == NULL)
	{
		LOG_DEBUG("_SetPetStashList UtilHelp메니져 생성 안됨");
		return FALSE;
	}

	PetStash* pPetStash = pGame->GetStashData();

	if (pPetStash == NULL)
	{
		LOG_DEBUG("_SetPetStashList 펫 창고 데이타를 얻어오지 못함 ");
		return FALSE;
	}

	PetStashData* pPetData = pPetStash->GetStashListAt( PetNum );

	if (pPetData == NULL)
	{
		LOG_DEBUG("_SetPetStashList 펫 데이타가 없음");
		return FALSE;
	}

	CItemData* pItemData = _pNetwork->GetItemData(pPetData->dbIndex);

	if ( pItemData == NULL )
	{
		LOG_DEBUG("_SetPetStashList 펫 아이템 정보가 없음");
		return FALSE;
	}

	CTString strTooltip;
	CUIIcon* pIcon;
	CUIText* pText;
	CUIImage* pImage;
	CUIProgressBar* pProgressBar;

	// 펫 아이콘
	pIcon = (CUIIcon*)pListItem->findUI("icon_pet");

	if ( pIcon != NULL )
	{
		pIcon->clearIconData();		

		pIcon->setData(UBET_ITEM, pPetData->dbIndex);
		CItems* pItems = pIcon->getItems();

		if (pItems != NULL)
			pItems->Item_Plus = pPetData->plus;
	}

	// 펫 이름
	pText = (CUIText*)pListItem->findUI("str_name");

	if ( pText != NULL )
	{
		CTString strName;
		strName.PrintF("%s", pPetData->petName);

		if ( pItemData->GetSubType() == CItemData::ACCESSORY_PET && strcmp(strName, CTString("")) == 0 )
		{
			strName = pHelp->GetP1PetName( pPetData->dbIndex, pPetData->plus );
		}

		strName = pHelp->GetCalcStringEllipsis(strName, DEF_MAX_PET_NAME_UI_SIZE, "..");

		pText->SetText( strName );
	}

	// 펫 레벨
	pText = (CUIText*)pListItem->findUI("str_level");

	if ( pText != NULL )
	{
		CTString strlevel;
		strlevel.PrintF("%d", pPetData->petLevel);
		pText->SetText( strlevel );
	}

	// 펫 충성도 아이콘 셋팅
	_ClearImage(pListItem);
	
	int nPetFaith = ( (float)(pPetData->petFaith) / (float)(pPetData->petFaithMax) ) * 100;

	if (nPetFaith <= 20)
	{
		pImage = (CUIImage*)pListItem->findUI("img_faith3");
	}
	else if(nPetFaith > 20 && nPetFaith <= 60)
	{
		pImage = (CUIImage*)pListItem->findUI("img_faith2");
	}
	else
	{
		pImage = (CUIImage*)pListItem->findUI("img_faith1");
	}

	if ( pImage != NULL )
	{
		strTooltip.PrintF( _S(5957,"충성도 %d"), pPetData->petFaith );
		pImage->setTooltip(CTString(strTooltip));
		pImage->Hide(FALSE);
	}

	// 배고픔 아이콘 셋팅
	int nPetHungry = ( (float)(pPetData->petHungry) / (float)(pPetData->petHungryMax) ) * 100;

	if (nPetHungry <= 20)
	{
		pImage = (CUIImage*)pListItem->findUI("img_hungry3");
	}
	else if(nPetHungry > 20 && nPetHungry <= 60)
	{
		pImage = (CUIImage*)pListItem->findUI("img_hungry2");
	}
	else
	{
		pImage = (CUIImage*)pListItem->findUI("img_hungry1");
	}

	if ( pImage != NULL )
	{
		strTooltip.PrintF( _S(2257,"배고픔 %d"), pPetData->petHungry );
		pImage->setTooltip(CTString(strTooltip));
		pImage->Hide(FALSE);
	}

	// 대리육성 마크
	pImage = (CUIImage*)pListItem->findUI("stash_curr_deputy");

	if ( pImage != NULL )
	{
		if (pPetData->virtualIndex == pPetStash->GetProxyPetIndex())
		{
			pImage->Hide(FALSE);
		}
		else
		{
			pImage->Hide(TRUE);
		}
	}

	// 경험치
	pProgressBar = (CUIProgressBar*)pListItem->findUI("list_progress");

	if ( pProgressBar != NULL )
	{
		CTString strExp;
		int nExp = 0;

		if (pPetData->petExp > 0 && pPetData->petLevelupExp > 0)
		{
			nExp = ( (double)(pPetData->petExp) / (double)(pPetData->petLevelupExp) ) * 100;
		}

		strExp.PrintF( "%d%%", nExp );
		pProgressBar->OffsetProgressPos( nExp );

		strTooltip.PrintF( _S(62,"경험치 %d"), pPetData->petExp );
		pProgressBar->setTooltip(CTString(strTooltip));

		for( int i = 0; i < PR_STAT_TAG_END; i++ )
		{
			pProgressBar->setProgressText( (_tagProgressStatus)i, strExp );
		}
	}

	return TRUE;
}

void CUIPetStash::_ClearImage(CUIListItem* pListItem)
{
	CUIImage* pImage;

	pImage = (CUIImage*)pListItem->findUI("img_faith1");

	if( pImage != NULL )
		pImage->Hide(TRUE);

	pImage = (CUIImage*)pListItem->findUI("img_faith2");

	if( pImage != NULL )
		pImage->Hide(TRUE);

	pImage = (CUIImage*)pListItem->findUI("img_faith3");

	if( pImage != NULL )
		pImage->Hide(TRUE);

	pImage = (CUIImage*)pListItem->findUI("img_hungry1");

	if( pImage != NULL )
		pImage->Hide(TRUE);

	pImage = (CUIImage*)pListItem->findUI("img_hungry2");

	if( pImage != NULL )
		pImage->Hide(TRUE);

	pImage = (CUIImage*)pListItem->findUI("img_hungry3");

	if( pImage != NULL )
		pImage->Hide(TRUE);

}

void CUIPetStash::_ClearPetStashList( CUIListItem* pListItem)
{
	if ( pListItem == NULL )
		return;

	CUIIcon* pIcon;
	CUIText* pText;
	CUIImage* pImage;
	CTString strInit = "";

	// 펫 아이콘 리셋
	pIcon = (CUIIcon*)pListItem->findUI("icon_pet");

	if ( pIcon != NULL )
		pIcon->clearIconData();

	// 펫 네임 리셋
	pText = (CUIText*)pListItem->findUI("str_name");

	if ( pText != NULL )
	{
		pText->SetText( strInit );
	}

	// 펫 레벨 리셋
	pText = (CUIText*)pListItem->findUI("str_level");

	if ( pText != NULL )
	{
		pText->SetText( strInit );
	}
	
	// 펫 충성도 아이콘 리셋
	_ClearImage(pListItem);

	// 펫 대리육성 마크 리셋
	pImage = (CUIImage*)pListItem->findUI("stash_curr_deputy");

	if ( pImage != NULL )
	{
		pImage->Hide(TRUE);
	}
}

//----------------------------------------------------------------------------
// 탭 페이지 2 대리육성

BOOL CUIPetStash::_UpdateDeputyList()
{
	GameDataManager* pGame = GameDataManager::getSingleton();

	if (pGame == NULL)
	{
		LOG_DEBUG("_UpdateDeputyList 대리육성 데이타메니져 접근 실패");
		return FALSE;
	}

	PetStash* pPetStash = pGame->GetStashData();

	if (pPetStash == NULL)
	{
		LOG_DEBUG("_UpdateDeputyList 대리육성 데이타 접근 실패");
		return FALSE;
	}

	CUIArrayItem* ptempItem = NULL;

	// 펫 갯수 얻기
	int nMax = pPetStash->GetPetCount();
	int Arraycount = m_pPetArray->GetArrayChildCount();

	// 펫 대리육성 아이콘 초기화
	_ClearSelectPet();

	// 펫 대리육성 어레이 리스트 셋팅
	for ( int i = 0; i < Arraycount; i++ )
	{
		ptempItem = m_pPetArray->GetArrayItem( i );

		if ( ptempItem == NULL )
			continue;

		if ( i >= nMax )
		{
			ptempItem->Hide(TRUE);
			continue;
		}
		
		if ( _SetDeputyList( ptempItem, i ) == FALSE )
		{
			LOG_DEBUG( "대리육성 셋팅 실패 _UpdateDeputyList %d", i );
			continue;
		}

		ptempItem->Hide(FALSE);
	}

	return TRUE;
}

BOOL CUIPetStash::_SetDeputyList( CUIArrayItem* pArrayItem, int PetNum )
{
	if ( pArrayItem == NULL )
	{
		LOG_DEBUG("대리육성 리스트아이템이 추가가 안됐음");
		return FALSE;
	}

	GameDataManager* pGame = GameDataManager::getSingleton();
	UtilHelp* pHelp = UtilHelp::getSingleton();

	if (pGame == NULL)
	{
		LOG_DEBUG("_SetDeputyList 대리육성 게임메니져 생성 안됨 ");
		return FALSE;
	}

	if (pHelp == NULL)
	{
		LOG_DEBUG("_SetDeputyList 유틸메니져 생성 안됨 ");
		return FALSE;
	}

	PetStash* pPetStash = pGame->GetStashData();

	if (pPetStash == NULL)
	{
		LOG_DEBUG("_SetDeputyList 대리육성 데이타를 얻어오지 못함 ");
		return FALSE;
	}

	PetStashData* pPetData = pPetStash->GetStashListAt( PetNum );

	if (pPetData == NULL)
	{
		LOG_DEBUG("_SetDeputyList 펫 데이타가 없음");
		return FALSE;
	}

	CItemData* pItemData = _pNetwork->GetItemData(pPetData->dbIndex);

	if ( pItemData == NULL )
	{
		LOG_DEBUG("_SetDeputyList 펫 아이템 정보가 없음");
		return FALSE;
	}

	CUIIcon* pIcon;
	CUIImage* pImage;

	// 펫 아이콘
	pIcon = (CUIIcon*)pArrayItem->findUI("icon_pet");

	if ( pIcon != NULL )
	{
		pIcon->clearIconData();
		
		pIcon->setData(UBET_ITEM, pPetData->dbIndex);

		CItems* pItems = pIcon->getItems();

		if (pItems != NULL)
			pItems->Item_Plus = pPetData->plus;
	}

	// 대리육성 마크
	pImage = (CUIImage*)pArrayItem->findUI("curr_deputy");

	if ( pImage != NULL )
	{
		if (pPetData->virtualIndex != pPetStash->GetProxyPetIndex())
		{
			pImage->Hide(TRUE);
			return TRUE;
		}
		else
		{
			// 대리육성중인 펫일 경우 대리육성 펫 아이콘으로 설정
			pImage->Hide(FALSE);
		}
	}

	// 대리육성 펫 아이콘
	if ( m_pIconSelectPet != NULL )
	{
		m_pIconSelectPet->Hide(FALSE);

		m_pIconSelectPet->clearIconData();

		m_pIconSelectPet->setData(UBET_ITEM, pPetData->dbIndex);
		CItems* pItems = m_pIconSelectPet->getItems();

		if (pItems != NULL)
			pItems->Item_Plus = pPetData->plus;
	}
	
	CTString strTemp;

	// 펫 이름
	if ( m_pTxtPetName != NULL )
	{
		strTemp.PrintF("%s", pPetData->petName);

		if ( pItemData->GetSubType() == CItemData::ACCESSORY_PET &&
			strcmp(strTemp, CTString("")) == 0 )
		{
			strTemp = pHelp->GetP1PetName( pPetData->dbIndex, pPetData->plus );						
		}

		m_pTxtPetName->SetText(CTString(strTemp));
	}

	// 펫 레벨
	if ( m_pTxtPetLevel != NULL )
	{
		strTemp.PrintF("%d", pPetData->petLevel);
		m_pTxtPetLevel->SetText(CTString(strTemp));
	}
	
	// 펫 경험치
	if ( m_pTxtPetExp != NULL )
	{
		strTemp.PrintF("%lli/%lli", pPetData->petExp, pPetData->petLevelupExp);
		m_pTxtPetExp->SetText(CTString(strTemp));
	}

	// 펫 충성도
	if ( m_pTxtPetLoyalty != NULL )
	{
		strTemp.PrintF("%d/%d", pPetData->petFaith, pPetData->petFaithMax);
		m_pTxtPetLoyalty->SetText(CTString(strTemp));
	}

	// 펫 배고픔
	if ( m_pTxtPetHungry != NULL )
	{
		strTemp.PrintF("%d/%d", pPetData->petHungry, pPetData->petHungryMax);
		m_pTxtPetHungry->SetText(CTString(strTemp));
	}

	return TRUE;
}

void CUIPetStash::_ClearDeputyList( CUIArrayItem* pArrayItem )
{
	if ( pArrayItem == NULL )
		return;

	CUIIcon* pIcon;
	CUIImage* pImage;

	pIcon = (CUIIcon*)pArrayItem->findUI("icon_pet");

	if ( pIcon != NULL )
	{
		pIcon->clearIconData();

	}

	pImage = (CUIImage*)pArrayItem->findUI("curr_deputy");

	if ( pImage != NULL )
	{
		pImage->Hide(TRUE);
	}
}

void CUIPetStash::_ClearSelectPet()
{
	if ( m_pPetArray != NULL )
		m_pPetArray->SetSelectIdx(-1);

	if ( m_pIconSelectPet != NULL )
	{
		m_pIconSelectPet->Hide(TRUE);
		m_pIconSelectPet->clearIconData();
	}

	if ( m_pTxtPetName != NULL )
		m_pTxtPetName->SetText(CTString(""));

	if ( m_pTxtPetLevel != NULL )
		m_pTxtPetLevel->SetText(CTString(""));

	if ( m_pTxtPetExp != NULL )
		m_pTxtPetExp->SetText(CTString(""));

	if ( m_pTxtPetLoyalty != NULL )
		m_pTxtPetLoyalty->SetText(CTString(""));

	if ( m_pTxtPetHungry != NULL )
		m_pTxtPetHungry->SetText(CTString(""));
}
