#include "stdh.h"

// 헤더 정리. [12/1/2009 rumist]
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Contents/Base/UIChangeWeaponNew.h>
#include <Engine/Contents/Base/UIChangeEquipment.h>
#include <Engine/Entities/InternalClasses.h>
#include <Engine/Interface/UIInventory.h>
#include <Engine/Interface/UIHelp.h>
#include <Engine/Interface/UIMixNew.h>
#include <Engine/Contents/Base/UIQuestNew.h>
#include <Engine/Contents/Base/UIQuestBookNew.h>
#include <Engine/GameDataManager/GameDataManager.h>
#include <Engine/Contents/Base/ChangeWeapon.h>

#define DEF_LIST_INFO_COUNT 5

enum eERROR_MSG
{
	eERROR_NOT_TRADE_ITEM = 0,
	eERROR_NOT_TRADE_LEVEL,
	eERROR_NOT_ENOUGH_NAS,
	eERROR_NO_TRADE_ITEM,
	eERROR_NO_SELECT_TRADE_ITEM,
	eERROR_SEALED_ITEM,
	eERROR_ONLY_WEAPON,
	eERROR_ONLY_ARMOR,
	eERROR_WEARING_EQUIPMENT,
	eERROR_USE_SKILL,
	eERROR_USE_TELEPORT
};

//--------------------------------------------------------------
// 창 닫기
class CmdChangeWeaponClose : public Command
{
public:
	CmdChangeWeaponClose() : pUI_(NULL) {}
	void setData(CUIChangeWeapon* pUI)	{ pUI_ = pUI; }
	void execute() {
		if (pUI_)
			pUI_->close();
	}
private:
	CUIChangeWeapon* pUI_;
};

//--------------------------------------------------------------
// 확인 클릭
class CmdChangeWeaponOk : public Command
{
public:
	CmdChangeWeaponOk() : pUI_(NULL) {}
	void setData(CUIChangeWeapon* pUI)	{ pUI_ = pUI; }
	void execute() {
		if (pUI_)
			pUI_->PressOK();
	}
private:
	CUIChangeWeapon* pUI_;
};

//--------------------------------------------------------------
// 리스트 클릭
class CmdChangeWeaponClick : public Command
{
public:
	CmdChangeWeaponClick() : pUI_(NULL) {}
	void setData(CUIChangeWeapon* pUI)	{ pUI_ = pUI; }
	void execute() {
		if (pUI_)
			pUI_->ClickListItem();
	}
private:
	CUIChangeWeapon* pUI_;
};

// [KH_07044] 3차 도움말 관련 추가
extern INDEX g_iShowHelp1Icon;

enum eSelection
{
	CHANGEWEAPON_OK,
	CHANGEWEAPON_TALK,
	CHANGEWEAPON_EVENT,
	CHANGESHIELD_OK,
	CHANGEWEAPON_MASTERSTONE,
	CHANGEEQUIP_WEAPON_OPEN,
	CHANGEEQUIP_ARMOR_OPEN,
};

#define CRITERION_LEVEL					(28)
#define EXCHANGE_LIMIT_LEVEL			(29)
#define EXCHANGE_LIMIT_LEVEL_MAX		(72)

CUIChangeWeapon::CUIChangeWeapon()
	: m_piconItem(NULL)
	, m_psbList(NULL)
	, m_bDrag(false)
	, m_nOriX(0), m_nOriY(0)
	, m_llNas(0)
{
	m_eChangeWeaponState	= CHANGEWEAPON_WEAPON;
	m_bCashItemChk			= FALSE;

	int i;	
	for (i = 0; i < eBUTTON_MAX; i++)
	{
		m_pbtn[i] = NULL;
	}

	for (i = 0; i < eTEXT_MAX; i++)
	{
		m_pText[i] = NULL;
	}

	for (i = 0; i < eLIST_MAX; i++)
	{
		m_plist[i] = NULL;
	}

	setInherit(false);
}

void CUIChangeWeapon::OpenChangeWeapon(int iMobIndex, BOOL bHasQuest, FLOAT fX, FLOAT fZ )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if( pUIManager->GetInventory()->IsLocked() == TRUE ||
		pUIManager->GetInventory()->IsLockedArrange() == TRUE)
	{
		pUIManager->GetInventory()->ShowLockErrorMessage();
		return;
	}

	// If this is already exist
	if( pUIManager->DoesMessageBoxLExist( MSGLCMD_CHANGEWEAPON_REQ ) || IsVisible() )
		return;

	// 전직 캐릭터인지를 체크함!!!
	pUIManager->CloseMessageBox( MSGCMD_CHANGEWEAPON_REP );
	pUIManager->CloseMessageBox( MSGCMD_CHANGEWEAPON_NOTIFY );

	// Set position of target npc
	m_fNpcX = fX;
	m_fNpcZ = fZ;

	CMobData* MD = CMobData::getData(iMobIndex);

	CTString strMessage;	

	if(iMobIndex == 1261)		// 인챈트 마스터 닐리 ; 마스터 스톤용
	{
		pUIManager->CreateMessageBoxL( _S(5435, "마스터 스톤으로 업그레이드하기" ), UI_CHANGEWEAPON, MSGLCMD_CHANGEWEAPON_REQ );		

		CTString	strNpcName = CMobData::getData(iMobIndex)->GetName();
		pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEWEAPON_REQ, TRUE, strNpcName, -1, 0xE18600FF );

		pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEWEAPON_REQ, TRUE, _S(5443,"일반인이 아이템을 업그레이드하는 데는 한계가 있습니다. 저는 그 한계를 뛰어 넘는 업그레이드하는 방법을 알고 있으며 또한 저만 업그레이드 한계 수치 이상 +26까지 업그레이드 할 수 있습니다."), -1, 0xA3A1A3FF );
		pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEWEAPON_REQ, TRUE, _S(5444,"그 방법은 마스터 스톤입니다. 마스터 스톤은 크게 무기마스터 스톤과 방어 구 마스터 스톤 2가지가 있으며 무기 마스터 스톤은 등급별로 6단계가 존재합니다."), -1, 0xA3A1A3FF );
		pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEWEAPON_REQ, TRUE, _S(5445,"(0~16, 0~18, 0~20, 0~22, 0~24, 0~25) 방어 구 마스터 스톤은 등급 없이 사용 가능합니다. 제가하는 업그레이드는 실패 없이 100%성공을 자랑합니다."), -1, 0xA3A1A3FF );
		pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEWEAPON_REQ, TRUE, _S(6451,"단, 마스터스톤을 한번이라도 사용한 아이템은 그 즉시 귀속되어 다른 사람은 사용할 수 없다는점을 반드시 명심하시기 바랍니다."), -1, 0xA3A1A3FF );

		strMessage= _S(5435, "마스터 스톤으로 업그레이드 하기" );			
		pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEWEAPON_REQ, FALSE, strMessage, CHANGEWEAPON_MASTERSTONE );	

		strMessage.PrintF( _S( 1220, "취소한다." ) );
		pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEWEAPON_REQ, FALSE, strMessage );	

		return;
	}

	// Create refine message box
	pUIManager->CreateMessageBoxL( _S(2308, "무기및 방어구 교체" ), UI_CHANGEWEAPON, MSGLCMD_CHANGEWEAPON_REQ );		

	CTString	strNpcName = CMobData::getData(iMobIndex)->GetName();
	pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEWEAPON_REQ, TRUE, strNpcName, -1, 0xE18600FF );
	pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEWEAPON_REQ, TRUE, _S( 1050, "지금 사용하고 있는 무기나 방어구가 마음에 들지 않는겐가?" ), -1, 0xA3A1A3FF );		
	pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEWEAPON_REQ, TRUE, _S( 1051, "그럼 내가 자네를 도와줄 수 있겠구만." ), -1, 0xA3A1A3FF );
	pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEWEAPON_REQ, TRUE, _S(2309,"하지만 주의해야할 사항이 있네"), -1, 0xA3A1A3FF );			
	pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEWEAPON_REQ, TRUE, _S(2310,"교체되는 장비는 제련된 수치만 남고 블러드 옵션값이 삭제되니까 잘 생각해 보시고 교체게나."), -1, 0xF3BA0CFF );
	pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEWEAPON_REQ, TRUE, _S(2311,"그래도 이렇게 쉽게 새로운 장비를 구할 수 있는 곳이 어디 있겠나..."), -1, 0xA3A1A3FF );
	pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEWEAPON_REQ, TRUE, _S(2312,"자 빨리 교환하게나...  사람들이 더 몰리기 전에..."), -1, 0xA3A1A3FF );

	strMessage.PrintF( _S( 1049, "무기 교체" ) );			
	pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEWEAPON_REQ, FALSE, strMessage, CHANGEWEAPON_OK );	

	// wooss add shield item exchange
	strMessage= _S(2313, "방어구 교체" );			
	pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEWEAPON_REQ, FALSE, strMessage, CHANGESHIELD_OK );

	strMessage= _S(7025, "은밀한 무기 교체" );			
	pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEWEAPON_REQ, FALSE, strMessage, CHANGEEQUIP_WEAPON_OPEN );

	strMessage= _S(7026, "은밀한 방어구 교체" );			
	pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEWEAPON_REQ, FALSE, strMessage, CHANGEEQUIP_ARMOR_OPEN );

	if( bHasQuest )
	{
		CUIQuestBook::AddQuestListToMessageBoxL(MSGLCMD_CHANGEWEAPON_REQ);				
	}

	if( MD->IsEvent() )
	{
		strMessage.PrintF( _S( 100, "이벤트." ) );			
		pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEWEAPON_REQ, FALSE, strMessage, CHANGEWEAPON_EVENT );
	}

	strMessage.PrintF( _S( 1220, "취소한다." ) );
	pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEWEAPON_REQ, FALSE, strMessage );	

}

void CUIChangeWeapon::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
}

void CUIChangeWeapon::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

void CUIChangeWeapon::PressOK()
{
	SendChangeWeaponReq();
}

void CUIChangeWeapon::ClickListItem()
{
	refreshListInfo();
}

void CUIChangeWeapon::MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput )
{
	switch( nCommandCode )
	{
	case MSGCMD_CHANGEWEAPON_REP:		// 무기 교체 확인.
		{
			if( bOK )
			{
				ChangeWeapon* pData = GAMEDATAMGR()->GetChangeWeapon();

				if (pData != NULL)
					pData->SendChangeEquipment(m_bCashItemChk);
			}
		}
		break;
	}
}

void CUIChangeWeapon::MsgBoxLCommand( int nCommandCode, int nResult )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	switch( nCommandCode )
	{
	case MSGLCMD_CHANGEWEAPON_REQ:
		if( nResult == CHANGEWEAPON_OK )
		{
			open(CHANGEWEAPON_WEAPON);

			if( !pUIManager->GetInventory()->IsVisible() )
				pUIManager->GetInventory()->ToggleVisible();			
			// [KH_07044] 3차 도움말 관련 추가
			if(g_iShowHelp1Icon)
			{
				pUIManager->GetHelp3()->ClearHelpString();
				pUIManager->GetHelp3()->AddHelpString(_S(3288, "교체하고자 하는 무기나 방어구를 왼쪽 상단의 빈 칸에 올려 놓으면 교체비용이 계산되며, 교체하려는 종류를 선택하고 확인을 누르면 비용이 지불되고 제련된 수치 그대로 선택한 종류의 장비로 교환이 이루어 집니다."));
				pUIManager->GetHelp3()->AddHelpString(_S(3289, "※ 교체되는 무기 및 방어구의 블러드 옵션은 옮겨지지 않습니다."));
				pUIManager->GetHelp3()->AddHelpString(_S(3290, "※ 29레벨 이상의 장비부터 교체가 가능합니다."));
				pUIManager->GetHelp3()->AddHelpString(_S(3291, "※ 착용 중인 장비나 레어옵션 장비(73레벨 이상)는 교체할 수 없습니다."));
				pUIManager->GetHelp3()->OpenHelp(this);
			}
		}		
		else if( nResult == CHANGEWEAPON_TALK )				// 이야기하기.
		{
			CUIQuestBook::TalkWithNPC();
		}
		else if( nResult == CHANGEWEAPON_EVENT )			// Event
		{
		}
		else if(nResult == CHANGESHIELD_OK)
		{
			open(CHANGEWEAPON_SHIELD);

			if( !pUIManager->GetInventory()->IsVisible() )
				pUIManager->GetInventory()->ToggleVisible();
			// [KH_07044] 3차 도움말 관련 추가
			if(g_iShowHelp1Icon)
			{
				pUIManager->GetHelp3()->ClearHelpString();
				pUIManager->GetHelp3()->AddHelpString(_S(3288, "교체하고자 하는 무기나 방어구를 왼쪽 상단의 빈 칸에 올려 놓으면 교체비용이 계산되며, 교체하려는 종류를 선택하고 확인을 누르면 비용이 지불되고 제련된 수치 그대로 선택한 종류의 장비로 교환이 이루어 집니다."));
				pUIManager->GetHelp3()->AddHelpString(_S(3289, "※ 교체되는 무기 및 방어구의 블러드 옵션은 옮겨지지 않습니다."));
				pUIManager->GetHelp3()->AddHelpString(_S(3290, "※ 29레벨 이상의 장비부터 교체가 가능합니다."));
				pUIManager->GetHelp3()->AddHelpString(_S(3291, "※ 착용 중인 장비나 레어옵션 장비(73레벨 이상)는 교체할 수 없습니다."));
				pUIManager->GetHelp3()->OpenHelp(this);
			}
		}
		else if (nResult == CHANGEEQUIP_WEAPON_OPEN)
		{
			if( !pUIManager->GetInventory()->IsVisible() )
				pUIManager->GetInventory()->ToggleVisible();

			pUIManager->GetChangeEquipment()->open(CHANGEEQUIP_WEAPON, m_fNpcX, m_fNpcZ);
		}
		else if (nResult == CHANGEEQUIP_ARMOR_OPEN)
		{
			if( !pUIManager->GetInventory()->IsVisible() )
				pUIManager->GetInventory()->ToggleVisible();

			pUIManager->GetChangeEquipment()->open(CHANGEEQUIP_ARMOR, m_fNpcX, m_fNpcZ);
		}
		else if(nResult == CHANGEWEAPON_MASTERSTONE)
		{
			pUIManager->GetMixNew()->OpenMixNewMasterStone();
		}
		// [090527: selo] 확장팩 퀘스트 수정
		else if( ciQuestClassifier < nResult )	
		{
			// 선택한 퀘스트에 대해 수락 또는 보상 창을 연다.
			CUIQuestBook::SelectQuestFromMessageBox( nResult );
		}
		else
		{
			close();
		}
		break;
	
	}
}

SQUAD CUIChangeWeapon::CalculateNeedNas( int iWeaponLevel )
{
	SQUAD llPrice	= 0;
	if( iWeaponLevel <= CRITERION_LEVEL )
		return llPrice;

	int nPlus = 0;
	if (m_piconItem != NULL)
	{
		CItems* pItem = m_piconItem->getItems();

		if (pItem != NULL)
			nPlus = pItem->GetItemPlus();
	}

	llPrice = 5000 * (nPlus * 2 + 10) * (iWeaponLevel - CRITERION_LEVEL) / 10;
	return llPrice;
}

SQUAD CUIChangeWeapon::CalculateNeedNasS( int iWeaponLevel )
{
	SQUAD llPrice	= 0;
	if( iWeaponLevel <= CRITERION_LEVEL )
		return llPrice;

	int nPlus = 0;
	if (m_piconItem != NULL)
	{
		CItems* pItem = m_piconItem->getItems();

		if (pItem != NULL)
			nPlus = pItem->GetItemPlus();
	}

	llPrice = 3000 * (nPlus * 2 + 10) * (iWeaponLevel - CRITERION_LEVEL) / 10;
	return llPrice;
}

void CUIChangeWeapon::SendChangeWeaponReq()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if (m_piconItem == NULL)
	{
		ShowSystemMsg(eERROR_NO_TRADE_ITEM);
		return;
	}

	if (m_piconItem->IsEmpty() != false)
	{
		ShowSystemMsg(eERROR_NO_TRADE_ITEM);
		return;
	}

	if( ( (CPlayerEntity*)CEntity::GetPlayerEntity(0) )->IsSkilling() )
	{
		ShowSystemMsg(eERROR_USE_SKILL);
		return;
	}
	
	if( pUIManager->IsCSFlagOn( CSF_TELEPORT ) )
	{
		ShowSystemMsg(eERROR_USE_TELEPORT);
		return;
	}

	if( m_llNas > _pNetwork->MyCharacterInfo.money && !m_bCashItemChk )
	{
		ShowSystemMsg(eERROR_NOT_ENOUGH_NAS);
		return;
	}

	if (m_plist[eLIST_ITEM] == NULL)
	{
		ShowSystemMsg(eERROR_NO_SELECT_TRADE_ITEM);
		return;
	}

	int nSelIdx = m_plist[eLIST_ITEM]->getCurSel();

	if( nSelIdx < 0 )
	{
		ShowSystemMsg(eERROR_NO_SELECT_TRADE_ITEM);
		return;
	}

	ChangeWeapon* pData = GAMEDATAMGR()->GetChangeWeapon();

	if (pData == NULL)
		return;

	pData->SetSelectIdx(getSelectIdx(nSelIdx));
	pUIManager->CloseMessageBox( MSGCMD_CHANGEWEAPON_REP );
	
	// Create message box of remission
	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( _S(2308, "무기및 방어구 교체" ), UMBS_OKCANCEL,UI_CHANGEWEAPON, MSGCMD_CHANGEWEAPON_REP );
	MsgBoxInfo.AddString( _S(2323,"교체 무기및 방어구에 제련된 기록은 옮겨지지만 블러드 옵션은 옮겨지지 않습니다. 무기및 방어구를 교체하시겠습니까?" ) );
	pUIManager->CreateMessageBox( MsgBoxInfo );
}

void CUIChangeWeapon::initialize()
{
	m_piconItem = (CUIIcon*)findUI("icon_item");
	m_psbList = (CUIScrollBar*)findUI("list_scroll");
	CTString strBtn[eBUTTON_MAX] = { "btn_close", "btn_ok", "btn_no" };
	CTString strText[eTEXT_MAX] = { "move_title", "str_title", "str_nas", "str_listType" };
	CTString strList[eLIST_MAX] = { "list_item", "list_itemInfo" };
	int i;	
	for (i = 0; i < eBUTTON_MAX; i++)
	{
		m_pbtn[i] = (CUIButton*)findUI(strBtn[i]);
	}

	for (i = 0; i < eTEXT_MAX; i++)
	{
		m_pText[i] = (CUIText*)findUI(strText[i]);
	}

	for (i = 0; i < eLIST_MAX; i++)
	{
		m_plist[i] = (CUIList*)findUI(strList[i]);
	}

	// command 셋팅
	if (m_pbtn[eBUTTON_CLOSE] != NULL)
	{
		CmdChangeWeaponClose* pCmd = new CmdChangeWeaponClose;
		pCmd->setData(this);
		m_pbtn[eBUTTON_CLOSE]->SetCommandUp(pCmd);
	}

	if (m_pbtn[eBUTTON_NO] != NULL)
	{
		CmdChangeWeaponClose* pCmd = new CmdChangeWeaponClose;
		pCmd->setData(this);
		m_pbtn[eBUTTON_NO]->SetCommandUp(pCmd);
	}

	if (m_pbtn[eBUTTON_OK] != NULL)
	{
		CmdChangeWeaponOk* pCmd = new CmdChangeWeaponOk;
		pCmd->setData(this);
		m_pbtn[eBUTTON_OK]->SetCommandUp(pCmd);
	}
}

void CUIChangeWeapon::open( eChangeWeaponState eType )
{
	m_eChangeWeaponState = eType;
	CUIManager* pUIManager = CUIManager::getSingleton();
	pUIManager->CloseMessageBox( MSGCMD_CHANGEWEAPON_REP );
	SetVisible(TRUE);
	Hide(FALSE);

	ChangeWeapon* pData = GAMEDATAMGR()->GetChangeWeapon();

	if (pData != NULL)
		pData->Clear();
	
	UpdateUI();

	pUIManager->RearrangeOrder( UI_CHANGEWEAPON, TRUE );
	pUIManager->GetInventory()->Lock( TRUE, TRUE, LOCK_CHANGEWEAPON );
}

void CUIChangeWeapon::close()
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	CUITooltipMgr::getSingleton()->setData(NULL);
	SetVisible(FALSE);
	Hide(TRUE);
	
	pUIManager->CloseMessageBox( MSGCMD_CHANGEWEAPON_REP );
	pUIManager->RearrangeOrder( UI_CHANGEWEAPON, FALSE );
	pUIManager->GetInventory()->Lock( FALSE, FALSE, LOCK_CHANGEWEAPON );

	if(m_bCashItemChk)
		m_bCashItemChk = FALSE;
}

void CUIChangeWeapon::UpdateUI()
{	
	if (refreshString() == false)
		close();

	if (refreshListItem() == false)
		close();

	refreshListInfo();

	clearIcon();
}

void CUIChangeWeapon::OnUpdate( float fDeltaTime, ULONG ElapsedTime )
{
	if (IsVisible() == FALSE)
		return;

	// Check distance
	FLOAT	fDiffX = _pNetwork->MyCharacterInfo.x - m_fNpcX;
	FLOAT	fDiffZ = _pNetwork->MyCharacterInfo.z - m_fNpcZ;

	if( m_bCashItemChk == FALSE )
	{
		if( fDiffX * fDiffX + fDiffZ * fDiffZ > UI_VALID_SQRDIST )
		{
			close();
		}
	}
}

WMSG_RESULT CUIChangeWeapon::OnMouseMove( UINT16 x, UINT16 y, MSG* pMsg )
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

WMSG_RESULT CUIChangeWeapon::OnLButtonDown( UINT16 x, UINT16 y )
{
	if (m_bHide)
		return WMSG_FAIL;

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	if( m_pText[eTEXT_MOVE] && m_pText[eTEXT_MOVE]->IsInside(x, y))
	{
		m_bDrag = true;
		m_nOriX = x;
		m_nOriY = y;
	}

	CUIManager::getSingleton()->RearrangeOrder( UI_CHANGEWEAPON, TRUE );

	return WMSG_FAIL;
}

WMSG_RESULT CUIChangeWeapon::OnLButtonUp( UINT16 x, UINT16 y )
{
	if (m_bHide)
		return WMSG_FAIL;

	m_bDrag = false;

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	CUIManager* pUIManager = CUIManager::getSingleton();

	if (m_piconItem == NULL)
	{
		CUIManager::getSingleton()->ResetHoldBtn();
		return WMSG_FAIL;
	}
	// 아이템 검사

	CUIIcon* pDrag = pUIManager->GetDragIcon();

	if (m_piconItem->IsInside(x, y) == TRUE && pDrag != NULL)
	{
		bool bError = false;
		// Error check
		if (pDrag->IsWearTab() == true)
		{
			ShowSystemMsg(eERROR_WEARING_EQUIPMENT);
			bError = true;
		}

		CItems*	pItems = pDrag->getItems();

		if (pItems == NULL)
			return WMSG_FAIL;

		CItemData*	pItemData = pItems->ItemData;

		if (pItemData == NULL)
			return WMSG_FAIL;

		if (pItems->Item_Flag & FLAG_ITEM_SEALED)
		{
			ShowSystemMsg(eERROR_SEALED_ITEM);
			bError = true;
		}

		if (bError == true)
		{
			CUIManager::getSingleton()->ResetHoldBtn();
			return WMSG_FAIL;
		}

		if (pItemData != NULL)
		{
			switch(m_eChangeWeaponState)
			{
			case CHANGEWEAPON_WEAPON:
				{
					if (pItemData->GetType() != CItemData::ITEM_WEAPON)
					{
						ShowSystemMsg(eERROR_ONLY_WEAPON);
						bError = true;
					}
				}
				break;
			case CHANGEWEAPON_SHIELD:
				{
					if (pItemData->GetType() != CItemData::ITEM_SHIELD)
					{
						ShowSystemMsg(eERROR_ONLY_ARMOR);
						bError = true;
					}
				}
				break;
			}

			if (checkValidItem(pItemData->getindex()) == true && bError == false)
			{
				ChangeWeapon* pData = GAMEDATAMGR()->GetChangeWeapon();
				if (pData != NULL)
				{
					pData->SetInvenTab(pItems->Item_Tab);
					pData->SetInvenIdx(pItems->InvenIndex);
					pData->SetVirtualIndex(pItems->Item_UniIndex);
				}
				m_piconItem->setData(pItems);
				updateNas();
				refreshListInfo();
			}
		}
	}

	CUIManager::getSingleton()->ResetHoldBtn();
	return WMSG_FAIL;
}

bool CUIChangeWeapon::refreshString()
{
	CTString strString[eTEXT_MAX];
	m_llNas = 0;
	strString[eTEXT_NAS].PrintF("%d", m_llNas);

	switch(m_eChangeWeaponState)
	{
	case CHANGEWEAPON_WEAPON:
		{
			strString[eTEXT_TITLE].PrintF( _S(1049, "무기 교체"));
			strString[eTEXT_LISTNAME].PrintF( _S(1059, "무기 리스트"));
		}
		break;
	case CHANGEWEAPON_SHIELD:
		{
			strString[eTEXT_TITLE].PrintF( _S(2313, "방어구 교체"));
			strString[eTEXT_LISTNAME].PrintF( _S(2316, "클래스 리스트"));
		}
		break;
	default:
		return false;
	}
	
	for (int i = 1; i < eTEXT_MAX; i++)
	{
		if (m_pText[i] == NULL)
		{
			LOG_DEBUG("CUIChangeWeapon::refreshString() ERROR %d String Null ptr", i);
			return false;
		}

		m_pText[i]->SetText(strString[i]);
	}

	return true;
}

bool CUIChangeWeapon::refreshListItem()
{
	if (m_plist[eLIST_ITEM] == NULL)
		return false;

	m_plist[eLIST_ITEM]->DeleteAllListItem();
	m_plist[eLIST_ITEM]->setCurSel(-1);

	if (m_psbList != NULL)
		m_psbList->SetScrollPos(0);

	int i, nMax = 0;
	CUIListItem* pTempItem = m_plist[eLIST_ITEM]->GetListItemTemplate();
	CUIListItem* pItem = NULL;

	switch(m_eChangeWeaponState)
	{
	case CHANGEWEAPON_WEAPON:
		{
			nMax = DEF_TRADE_WEAPON_MAX - 1; // (총갯수 - 교환가능 탭)
		}
		break;
	case CHANGEWEAPON_SHIELD:
		{
			nMax = TOTAL_JOB - 1; // (나이트 쉐도우 빼줌)
		}
		break;
	default:
		return false;
	}

	int nListString[CHANGEWEAPON_MAX][DEF_TRADE_WEAPON_MAX - 1] = { \
	{1038, 1039, 1040, 1041, 1042, 1043, 1044, 1045, 1046, 1047, 2306, 2307}, \
	{43, 44, 45, 46, 47, 48, 5732, 5820, 0, 0, 0, 0}		\
	};

	CTString strTemp;

	for (i = 0; i < nMax; i++)
	{
		m_plist[eLIST_ITEM]->AddListItem(pTempItem->Clone());
		pItem = (CUIListItem*)m_plist[eLIST_ITEM]->GetListItem( i );

		if (pItem == NULL)
			continue;

		CUIText* pText = (CUIText*)pItem->findUI("str_itemType");

		if (pText != NULL)
		{
			strTemp.PrintF(_S( nListString[m_eChangeWeaponState][i], "리스트 스트링"));
			pText->SetText(strTemp);
		}

		CmdChangeWeaponClick* pCmd = new CmdChangeWeaponClick;
		pCmd->setData(this);
		pItem->SetCommandUp(pCmd);	
	}

	m_plist[eLIST_ITEM]->UpdateList();
	m_plist[eLIST_ITEM]->UpdateScroll(nMax);
	return true;
}

bool CUIChangeWeapon::refreshListInfo()
{
	if (m_plist[eLIST_INFO] == NULL)
		return false;
	// 인포 초기화
	m_plist[eLIST_INFO]->DeleteAllListItem();

	// 인포 셋팅 검사
	if (m_piconItem == NULL)
		return false;

	if (m_piconItem->IsEmpty() != false)
		return false;

	if (m_plist[eLIST_ITEM] == NULL)
		return false;

	// 교환할 아이템 인덱스 얻기
	int nSelectItemIdx = m_piconItem->getIndex();
	int nSelectListIdx = m_plist[eLIST_ITEM]->getCurSel();

	if (nSelectListIdx < 0)
		return false;

	// 인포 셋팅
	int i;
	int nTradeItemIdx = 0;
	CUIListItem* pTempItem = m_plist[eLIST_INFO]->GetListItemTemplate();
	CUIListItem* pItem = NULL;

	// 교활될 아이템 정보 얻기
	switch(m_eChangeWeaponState)
	{
	case CHANGEWEAPON_WEAPON:
		{
			nTradeItemIdx = CChangeEquipment::getTradeWeaponIndex(nSelectItemIdx, nSelectListIdx);
		}
		break;
	case CHANGEWEAPON_SHIELD:
		{
			nTradeItemIdx = CChangeEquipment::getTradeArmorIndex(nSelectItemIdx, nSelectListIdx);
		}
		break;
	default:
		return false;
	}

	if (nTradeItemIdx <= 0)
		return false;

	if (GAMEDATAMGR()->GetChangeWeapon() != NULL)
		GAMEDATAMGR()->GetChangeWeapon()->SetTradeItemIndex(nTradeItemIdx);

	// 인포 타이틀 스트링 인덱스
	int nListString[CHANGEWEAPON_MAX][DEF_LIST_INFO_COUNT] = { \
	{1065, 1066, 1067, 1068, 1069}, \
	{2317, 2318, 1067, 87, 0}		\
	};

	// String XML ID
	CTString strTitleID[DEF_LIST_INFO_COUNT] = {"str_nameTitle", "str_levelTitle", "str_classTitle", \
		"str_point1Title", "str_point2Title" };
	// String XML ID
	CTString strValueID[DEF_LIST_INFO_COUNT] = {"str_name", "str_level", "str_class", "str_point1", "str_point2" };

	CTString strTemp;
	m_plist[eLIST_INFO]->AddListItem(pTempItem->Clone());
	pItem = (CUIListItem*)m_plist[eLIST_INFO]->GetListItem(0);

	if (pItem == NULL)
	{
		m_plist[eLIST_INFO]->DeleteAllListItem();
		return false;
	}

	CUIText* pTextString = NULL;
	CUIText* pTextValue = NULL;

	// 타이틀 셋팅
	for (i = 0; i < DEF_LIST_INFO_COUNT; i++)
	{
		pTextString = (CUIText*)pItem->findUI(strTitleID[i]);

		if (pTextString != NULL)
		{
			strTemp.PrintF(_S( nListString[m_eChangeWeaponState][i], "타이틀 스트링"));
			pTextString->SetText(strTemp);
		}
	}

	pTextString = (CUIText*)pItem->findUI(strTitleID[4]);

	if (pTextString != NULL)
	{
		if (m_eChangeWeaponState == CHANGEWEAPON_SHIELD)
			pTextString->Hide(TRUE);
		else
			pTextString->Hide(FALSE);
	}

	// 아이템 정보 셋팅
	CTString strValue[DEF_LIST_INFO_COUNT];

	CItemData* pItemData = CItemData::getData(nTradeItemIdx);

	if (pItemData == NULL)
	{
		m_plist[eLIST_INFO]->DeleteAllListItem();
		return false;
	}	
	
	// 아이템 이름
	strValue[0] = pItemData->GetName();
	// 아이템 레벨
	strValue[1].PrintF("%d", pItemData->GetLevel());
	// 아이템 사용 클래스
	CUIManager::getSingleton()->GetClassOfItem(pItemData, strValue[2]);
	
	int nPoint1 = 0, nPoint2 = 0;
	switch(m_eChangeWeaponState)
	{
	case CHANGEWEAPON_WEAPON:
		{
			nPoint1 = pItemData->GetPhysicalAttack();
			nPoint2 = pItemData->GetMagicAttack();
		}
		break;
	case CHANGEWEAPON_SHIELD:
		{
			nPoint1 = pItemData->GetPhysicalDefence();
		}
		break;
	}
	// 공격력, 방어력
	strValue[3].PrintF("%d", nPoint1);
	// 마법 공격력, 없음
	strValue[4].PrintF("%d", nPoint2);

	for (i = 0; i < DEF_LIST_INFO_COUNT; i++)
	{
		pTextValue = (CUIText*)pItem->findUI(strValueID[i]);

		if (pTextValue != NULL)
		{
			pTextValue->SetText(strValue[i]);
		}
	}

	pTextValue = (CUIText*)pItem->findUI(strValueID[4]);

	if (pTextValue != NULL)
	{
		if (m_eChangeWeaponState == CHANGEWEAPON_SHIELD)
			pTextValue->Hide(TRUE);
		else
			pTextValue->Hide(FALSE);
	}

	m_plist[eLIST_INFO]->UpdateList();
	m_plist[eLIST_INFO]->UpdateScroll(1);
	return true;
}

bool CUIChangeWeapon::checkValidItem( int nItemIdx )
{
	bool bValid = false;

	switch(m_eChangeWeaponState)
	{
	case CHANGEWEAPON_WEAPON:
		bValid = CChangeEquipment::IsValidWeapon(nItemIdx);
		break;
	case CHANGEWEAPON_SHIELD:
		bValid = CChangeEquipment::IsValidArmor(nItemIdx);
		break;
	}
	
	if (bValid == false)
	{
		ShowSystemMsg(eERROR_NOT_TRADE_ITEM);
		return false;
	}
	
	int nlevel = CItemData::getData(nItemIdx)->GetLevel();

	if (nlevel < EXCHANGE_LIMIT_LEVEL || nlevel > EXCHANGE_LIMIT_LEVEL_MAX)
	{
		ShowSystemMsg(eERROR_NOT_TRADE_LEVEL);
		return false;
	}
	
	return true;
}

void CUIChangeWeapon::updateNas()
{
	if (m_pText[eTEXT_NAS] == NULL)
		return;

	CTString strNas;
	m_llNas = 0;

	if (m_piconItem != NULL)
	{
		int nlevel = CItemData::getData(m_piconItem->getIndex())->GetLevel();

		switch(m_eChangeWeaponState)
		{
		case CHANGEWEAPON_WEAPON:
			m_llNas = CalculateNeedNas(nlevel);
			break;
		case CHANGEWEAPON_SHIELD:
			m_llNas = CalculateNeedNasS(nlevel);
			break;
		}
	}
	
	strNas.PrintF("%lli", m_llNas);
	m_pText[eTEXT_NAS]->SetText(strNas);
}

void CUIChangeWeapon::ShowSystemMsg( int errorType )
{
	CTString strMessage;

	switch (errorType)
	{
	case eERROR_NOT_TRADE_ITEM:
		strMessage.PrintF( _S( 2158,"교체할 수 없는 아이템입니다." ) );
		break;
	case eERROR_NOT_TRADE_LEVEL:
		strMessage.PrintF( _S( 1073, "교체할 수 있는 아이템 레벨이 아닙니다." ) );
		break;
	case eERROR_NOT_ENOUGH_NAS:
		strMessage.PrintF( _S( 1080, "교체 비용이 부족합니다." ) );
		break;
	case eERROR_NO_TRADE_ITEM:
		strMessage.PrintF( _S( 1076, "교체할 아이템이 없습니다." ) );
		break;
	case eERROR_NO_SELECT_TRADE_ITEM:
		strMessage.PrintF( _S( 2321,"새로운 무기및 방어구를 선택해주십시요." ) );
		break;
	case eERROR_SEALED_ITEM:
		strMessage.PrintF( _S( 1079, "봉인된 아이템은 교체할 수 없습니다." ) );
		break;
	case eERROR_ONLY_WEAPON:
		strMessage.PrintF( _S( 1072, "무기만 교체 할 수 있습니다.") );
		break;
	case eERROR_ONLY_ARMOR:
		strMessage.PrintF( _S( 2320, "방어구만 교체 할 수 있습니다." ) );
		break;
	case eERROR_WEARING_EQUIPMENT:
		strMessage.PrintF( _S( 1071, "착용된 아이템은 교체할 수 없습니다." ) );
		break;
	case eERROR_USE_SKILL:
		strMessage.PrintF( _S( 1074, "스킬 사용중에는 아이템을 교체할 수 없습니다." ) );
		break;
	case eERROR_USE_TELEPORT:
		strMessage.PrintF( _S( 1075, "순간 이동중에는 아이템를 교체할 수 없습니다." ) );
		break;
	default:
		return;
	}

	_pNetwork->ClientSystemMessage(strMessage, SYSMSG_ERROR);
}

int CUIChangeWeapon::getSelectIdx( int nSelIdx )
{
	int ret = -1;

	switch(m_eChangeWeaponState)
	{
	case CHANGEWEAPON_WEAPON:
		ret = nWeaponSelType[nSelIdx];
		break;
	case CHANGEWEAPON_SHIELD:
		ret = nClassSelType[nSelIdx];
		break;
	}
	return ret;
}

void CUIChangeWeapon::clearIcon()
{
	CTString strToolTip;

	if (m_piconItem != NULL)
	{
		m_piconItem->clearIconData();
		strToolTip.PrintF(_S(7033, "장비 슬롯"));
		m_piconItem->setTooltip(strToolTip);
	}
}
