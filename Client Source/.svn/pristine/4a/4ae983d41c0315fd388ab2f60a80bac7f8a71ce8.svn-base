#include "StdH.h"
#include "PremiumChar.h"
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/GameDataManager/GameDataManager.h>
#include <Engine/Contents/Base/Auction.h>
#include <Engine/Interface/UIInventory.h>
#include <Engine/Interface/UIInvenCashBag.h>
#include <Engine/Contents/Base/ExpressSystem.h>
#include <Engine/Interface/UIPetFree.h>
#include <Engine/Interface/UIMessenger.h>
#include <Engine/Interface/UIRadar.h>
#include <Engine/Contents/function/SimplePlayerInfoUI.h>

CPremiumChar::CPremiumChar()
	: m_sbPmType(0)
	, m_nTeleportCount(0)
	, m_nExpireTime(0)
{
}

CPremiumChar::~CPremiumChar()
{

}

void CPremiumChar::Clear()
{
	m_sbPmType = PREMIUM_CHAR_TYPE_NONE;
	m_nTeleportCount = 0;
	m_nExpireTime = 0;

	// 프리미엄 캐릭터 해택 업데이트
	PremiumBenefit(false);
	UIMGR()->GetSimplePlayerInfo()->SetPremiumChar(m_sbPmType);
}

void CPremiumChar::updateInfo( char type, int nCount, int nTime )
{
	m_sbPmType = type;
	m_nTeleportCount = nCount;
	m_nExpireTime = nTime;

	UIMGR()->GetSimplePlayerInfo()->SetPremiumChar(type);

	bool bUse = false;

	switch(m_sbPmType)
	{
	case PREMIUM_CHAR_TYPE_NONE:
		bUse = false;
		break;
	case PREMIUM_CHAR_TYPE_FIRST:
		bUse = true;
		break;
	}

	// 프리미엄 캐릭터 해택 업데이트
	PremiumBenefit(bUse);	
}

void CPremiumChar::RecvPremiumCharMsg( CNetworkMessage* istr )
{
#ifdef PREMIUM_CHAR
	pTypeBase* pPacket = reinterpret_cast<pTypeBase*>(istr->GetBuffer());
	int errorType = 0;

	switch(pPacket->subType)
	{
	case MSG_SUB_PREMIUM_CHAR_USE_ITEM:
		{
			ResponseClient::premiumChar_UseItem* pErr = reinterpret_cast<ResponseClient::premiumChar_UseItem*>(istr->GetBuffer());
			errorType = pErr->result;
		}		
		break;

	case MSG_SUB_PREMIUM_CHAR_JUMP_TO_CHAR:
		{
			ResponseClient::premiumChar_JumpToChar* pErr = reinterpret_cast<ResponseClient::premiumChar_JumpToChar*>(istr->GetBuffer());
			errorType = pErr->result;
		}		
		break;

	default:
		return;
	}

	CTString strErrorMsg = CTString("");

	switch(errorType)
	{
	case ERR_PREMIUM_CHAR_NONE:					// 성공
		{
			if (pPacket->subType == MSG_SUB_PREMIUM_CHAR_USE_ITEM)
				strErrorMsg.PrintF( _S( 6324, "%s 에게 프리미엄 혜택이 적용 되었습니다.") , _pNetwork->MyCharacterInfo.name);
			else if (pPacket->subType == MSG_SUB_PREMIUM_CHAR_JUMP_TO_CHAR)
				strErrorMsg.PrintF( _S( 395, "이동이 완료되었습니다."));
		}		
		break;
	case ERR_PREMIUM_CHAR_ALREADY_USED:			// 이미 사용중
		strErrorMsg.PrintF(_S( 6322, "프리미엄 혜택이 이미 적용되어 있어 사용 할 수 없습니다."));
		break;
	case ERR_PREMIUM_CHAR_NOT_FOUNC_CHAR:		// 점핑 대상을 찾을 수 없음
		strErrorMsg.PrintF(_S( 4523, "대상을 찾을 수 없습니다."));
		break;
	case ERR_PREMIUM_CHAR_INVALID_JUMPING_POSITION:		// 점핑할 수 없는 위치
		strErrorMsg.PrintF(_S( 2127,  "순간 이동 가능 지역이 아닙니다.(다른 존이거나 퍼스널던전에서는 사용할 수 없습니다.)" )); 
		break;
	case ERR_PREMIUM_CHAR_MAX_JUMP_COUNT:		// 점핑수를 초과함
		strErrorMsg.PrintF(_S( 6345, "오늘 이동 가능한 횟수를 모두 사용하였습니다."));
		break;
	case ERR_PREMIUM_CHAR_NOT_PREMIUM_CHAR:		// 프리미엄 캐릭터 상태가 아님
		strErrorMsg.PrintF(_S( 6327, "프리미엄 캐릭터 혜택 없음"));
		break;
	case ERR_PREMIUM_CHAR_FRIEND_IS_OFFLINE:
		strErrorMsg.PrintF(_S( 1628, "해당 케릭터가 존재 하지 않거나 [오프라인] 상태입니다."));
		break;
	case ERR_PREMIUM_CHAR_NOT_JUMP_STATUS:
		strErrorMsg.PrintF( _S( 415, "이동할 수 없는 상태입니다." ));
		break;
	}

	_pNetwork->ClientSystemMessage(strErrorMsg, SYSMSG_ERROR);
#endif	//	PREMIUM_CHAR
}

void CPremiumChar::PremiumBenefit( bool bUse )
{
	CUIManager* pUImgr = CUIManager::getSingleton();

	// 레이더 업데이트
	pUImgr->GetRadar()->SetPremiumCharBenefit(bUse);

	// 펫 부활 원격 사용 가능
	pUImgr->GetPetFree()->SetPremiumBenefit(bUse);

	// 아이리스 가방 1 사용 가능
	CUIInvenCashBag* pCash = ((CUIInvenCashBag*)pUImgr->GetUI(UI_INVEN_CASH1));

	if (pCash != NULL)
		pCash->SetPremiumBenefit(bUse);

	// 거래대행 수수료 0
	Auction* pAuction = GAMEDATAMGR()->GetAuction();

	if (pAuction != NULL)
		pAuction->SetPremiumBenefit(bUse);

	// 신비한 석상 원격 사용 가능
	ExpressSystem* pExpress = GAMEDATAMGR()->GetExpressData();

	if (pExpress != NULL)
		pExpress->SetPremiumBenefit(bUse);

	// 친구에게 텔레포트 사용 가능
	pUImgr->GetMessenger()->SetPremiumBenefit(bUse);

	// 장비레벨 3 감소는 CUITooltip::SetLevelInfo() 안에서 처리
	// 외형변경의 경우 서버 단독
	// 경험치 및 SP 증가의 경우 서버 단독
}
