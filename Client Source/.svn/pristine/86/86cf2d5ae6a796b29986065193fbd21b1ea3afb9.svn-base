#include "stdh.h"
#include <Engine/Entities/InternalClasses.h>
#include <Engine/Network/CNetwork.h>
#include <Engine/Network/Server.h>
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Network/SessionState.h>
#include <Engine/Network/MessageDefine.h>
#include <Engine/GlobalDefinition.h>
#include <Engine/GameState.h>
#include <Engine/SlaveInfo.h>
#include <Engine/Interface/UIManager.h>
#include <Engine/Interface/UIAutoHelp.h>
#include <Engine/Interface/UISiegeWarfareDoc.h>
#include <Engine/Interface/UIPetTraining.h>
#include <Engine/Interface/UIPetInfo.h>
#include <Engine/Interface/UISummon.h>
#include <Engine/Interface/UIShop.h>
#include <Engine/Contents/Base/UINoticeNew.h>
#include <Engine/Templates/DynamicContainer.cpp>
#include <Engine/Contents/Base/UIPetStash.h>
#include <Engine/Interface/UICashShopEX.h>
#include <Engine/Interface/UIInventory.h>
#include <Engine/Interface/UIRadar.h>
#include <Engine/Interface/UIGamble.h>
#include <Engine/Interface/UIMessenger.h>
#include <Engine/Interface/UIGuild.h>
#include <Engine/Contents/Base/UIPartyNew.h>
#include <Engine/Interface/UIMap.h>
#include <Engine/Interface/UIPartyAuto.h>
#include <Engine/Interface/UISiegeWarfareNew.h>
#include <Engine/Interface/UIPlayerInfo.h>
#include <Engine/Interface/UIGuildWarPortal.h>
#include <Engine/Interface/UIMonsterCombo.h>
#include <Engine/Contents/function/UIPortalNew.h>
#include <Engine/Interface/UIHelp.h>
#include <Engine/Interface/UIRanking.h>
#include <Engine/Interface/UIOption.h>
#include <Engine/Interface/UIWildPetInfo.h>
#include <Engine/Interface/UIReformSystem.h>
#include <Engine/Interface/UINickName.h>
#include <Engine/Interface/UINpcScroll.h>
#include <Engine/Interface/UIPetFree.h>
#include <Engine/Interface/UISocketSystem.h>
#include <Engine/Interface/UIPetItemMix.h>
#include <Engine/Interface/UIAffinity.h>
#include <Engine/Interface/UIAffinityInfo.h>
#include <Engine/Interface/UIMonsterMercenary.h>
#include <Engine/Interface/UIInitJob.h>
#include <Engine/Interface/UIQuickSlot.h>
#include <Engine/Contents/Base/UIRankingSystem.h>
#include <Engine/Interface/UIMixNew.h>
#include <Engine/LoginJobInfo.h>
#include <Engine/Contents/Login/BackImageManager.h>
#include <Engine/Object/ActorMgr.h>
#include <Engine/GameDataManager/GameDataManager.h>
#include <Engine/Contents/Base/Notice.h>
#include <Engine/Contents/Base/Quest.h>
#include <Engine/Info/MyInfo.h>
#include <Engine/Contents/Base/Party.h>
#include <Engine/Contents/function/SimplePlayerInfoUI.h>
#include <Engine/Info/ServerInfo.h>
#include <Engine/Contents/Base/ChattingUI.h>
#include <Common/Packet/ptype_old_do_item.h>

// socket system. [5/11/2010 rumist]
//#include <Engine/Interface/UISocketSystem.h>
#define MODEL_TREASURE	("Data\\Item\\Common\\ITEM_treasure02.smc")
// #include <map>
// #include <algorithm>
//#define XTRAP_LOG_ENABLE
// ----------------------------------------------------------------------------
// Name : CheckDratanSiegewarfareError()
// Desc : 
// ----------------------------------------------------------------------------
void CheckDratanSiegewarfareError(UBYTE errcode)
{
	CTString tStr;
	switch(errcode)
	{
		case MSG_EX_CASTLE_ERROR_NOT_JOIN:    // 공성 참가주이 아님
				tStr = _S( 3739,"공성 참가중이 아닙니다.");
				break;
		case MSG_EX_CASTLE_ERROR_NOT_WAR:    // 공성전 중이 아님
				tStr = _S( 3740,"공성중이 아닙니다.");
				break;
		case MSG_EX_CASTLE_ERROR_POS_FAIL:    // 위치 이상
				tStr = _S( 3741,"공성 지역이 아닙니다.");
				break;
		case MSG_EX_CASTLE_ERROR_MEM_5_FAIL:    // 교감 인원 자리 없음
				tStr = _S( 3742,"교감 인원이 초과되었습니다.");
				break;
		case MSG_EX_CASTLE_ERROR_NOT_OWNER:    // 성주 길드 아님
				tStr = _S( 3743,"성주 길드가 아닙니다.");
				break;
		case MSG_EX_CASTLE_ERROR_NOT_RESPOND:   // 교감 중이 아님
				tStr = _S( 3744,"교감중이 아닙니다.");
				break;
		case MSG_EX_CASTLE_ERROR_NOT_RESPOND_TIME:  // 교감 시간 부족
				tStr = _S( 3745,"교감 시간이 부족합니다.");
				break;
		case MSG_EX_CASTLE_ERROR_NO_MONEY:    // 돈 부족
				tStr = _S(306,"나스가 부족합니다.");
				break;
		case MSG_EX_CASTLE_ERROR_NOT_ATTACK:    // 공성 길드 아님
				tStr = _S( 3746,"공성 길드가 아닙니다.");
				break;
		case MSG_EX_CASTLE_ERROR_NO_DATA:    // 데이터 이상
				tStr.PrintF("Error %d",errcode);
				break;
		case MSG_EX_CASTLE_ERROR_NO_DIFFER:    // 이전 데이터와 동일				
				tStr = _S( 3747,"이미 강화된 단계입니다.");				
				break;
		case MSG_EX_CASTLE_ERROR_NOT_BUY:    // 구매하지 않았음				
				tStr.PrintF("Error %d",errcode);
				break;
		case MSG_EX_CASTLE_ERROR_NO_NPC:     // 없는 타워
				tStr.PrintF("Error %d",errcode);
				break;
		case MSG_EX_CASTLE_ERROR_DEAD_NPC:    // 죽은 타워 
				tStr.PrintF("Error %d",errcode);
				break;
		case MSG_EX_CASTLE_ERROR_HAVE_NO_GUILD:   // 길드 원이 아님
				tStr = _S(985,"해당 길드원을 찾을 수 없습니다.");
				break;				
		case MSG_EX_CASTLE_ERROR_USE_NPC:    // 사용중 부활 진지
				tStr = _S(3749,"사용중인 부활진지입니다.");
				break;
		case MSG_EX_CASTLE_ERROR_USE_NPC_OTHER:   // 이미 다른 부활진지 사용중
				tStr = _S(3750,"다른 부활진지를 사용중입니다.");
				break;
		case MSG_EX_CASTLE_ERROR_NO_NPC2:     // 없는 부활 진지
				tStr.PrintF("Error %d",errcode);
				break;
		case MSG_EX_CASTLE_ERROR_DEAD_NPC2:    // 죽은 부활진지
				tStr.PrintF("Error %d",errcode);
				break;				
		case MSG_EX_CASTLE_ERROR_SAME_DATA:	   // 같은 데이터 - 타입과 단계가 안맞을때...
				tStr.PrintF("Error %d",errcode);
				break;
		case MSG_EX_CASTLE_ERROR_NOT_BUY_NPC:  // 더이상 NPC를 구매 할수 없습니다,
				tStr = _S(3812, "더이상 구입할 수 없습니다.");
				break;
		case MSG_EX_CASTLE_ERROR_PERMISSION_DENIED: // 권한이 없습니다.
				tStr = _S( 973, "권한이 없습니다.");
				break;
	}
	
	if(tStr.Length()>0 )
		CUIManager::getSingleton()->GetChattingUI()->AddSysMessage( tStr , SYSMSG_ERROR );	
	
}

// ----------------------------------------------------------------------------
// Name : CheckCashMoonStoneMessage()
// Desc : 
// ----------------------------------------------------------------------------
BOOL CheckCashMoonStone(UBYTE errcode)
{
	if( errcode == MSG_EX_CASHITEM_MOONSTONE_ERROR_SUCCESS )	// 성공
		return TRUE;

	CUIManager* pUIManager = CUIManager::getSingleton();

	switch(errcode)
	{
		case MSG_EX_CASHITEM_MOONSTONE_ERROR_NOITEM:		
			pUIManager->GetChattingUI()->AddSysMessage( _S(2908, "교환할 아이템이 없습니다." ), SYSMSG_ERROR );	
			break;
		case MSG_EX_CASHITEM_MOONSTONE_ERROR_INVEN:		
			pUIManager->GetChattingUI()->AddSysMessage( _S(265, "인벤토리가 부족합니다." ), SYSMSG_ERROR );	
			break;
		case MSG_EX_CASHITEM_MOONSTONE_ERROR_CANTUSE_CASHMOON:			
			pUIManager->GetChattingUI()->AddSysMessage( _S(2907, "이용 가능한 아이템이 아닙니다." ), SYSMSG_ERROR );	
			break;
	}

	return FALSE;

}

// ----------------------------------------------------------------------------
// Name : CheckCashItemMessage()
// Desc : 
// ----------------------------------------------------------------------------
BOOL CheckCashItemMessage(UBYTE errcode)
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	switch(errcode)
	{
		case MSG_EX_CASHITEM_ERROR_SUCCESS:				// 성공
			pUIManager->GetChattingUI()->AddSysMessage( _S(2408,  "요청한 작업이 성공하였습니다." ), SYSMSG_ERROR );	
			return TRUE;
			
		case MSG_EX_CASHITEM_ERROR_LACKCASH:			// 보유캐쉬 부족
			pUIManager->GetCashShopEX()->Message(MSGCMD_CASH_EX_ERROR, _S(191, "확인"), _S(2409, "보유 캐쉬가 부족 합니다."), UMBS_OK);
			break;
		case MSG_EX_CASHITEM_ERROR_NOTUSER:				// 해당유저 없음
			pUIManager->GetCashShopEX()->Message(MSGCMD_CASH_EX_ERROR, _S(191, "확인"), _S(2410, "해당 유저가 존재하지 않습니다."), UMBS_OK);
			break;
		case MSG_EX_CASHITEM_ERROR_NOTITEM:				// 존재하지 않는 상품입니다
			pUIManager->GetCashShopEX()->Message(MSGCMD_CASH_EX_ERROR, _S(191, "확인"), _S(2411, "존재하지 않는 상품입니다."), UMBS_OK);
			break;
		case MSG_EX_CASHITEM_ERROR_DB:					// DB오류
			pUIManager->GetCashShopEX()->Message(MSGCMD_CASH_EX_ERROR, _S(191, "확인"), _S(1843, "서버에 연결할 수 없습니다.(B)"), UMBS_OK);
			break;
		case MSG_EX_CASHITEM_ERROR_PACKET:				// 패킷 오류
			pUIManager->GetCashShopEX()->Message(MSGCMD_CASH_EX_ERROR, _S(191, "확인"), _S(854, "패킷 오류가 발생하였습니다."), UMBS_OK);
			break;
		case MSG_EX_CASHITEM_ERROR_ETC:					// 기타 오류
			pUIManager->GetCashShopEX()->Message(MSGCMD_CASH_EX_ERROR, _S(191, "확인"), _S(1094, "알 수 없는 오류가 발생하였습니다."), UMBS_OK);
			break;
		case MSG_EX_CASHITEM_ERROR_OVERITEM:				// 구입 상품 개수 초과(10개)
			pUIManager->GetCashShopEX()->Message(MSGCMD_CASH_EX_ERROR, _S(191, "확인"), _S(2413, "구입 상품이 10개를 초과하였습니다."), UMBS_OK);
			break;
		case MSG_EX_CASHITEM_ERROR_NOSPACE:				// 구매물품 보관소의 공간 부족
			pUIManager->GetCashShopEX()->Message(MSGCMD_CASH_EX_ERROR, _S(191, "확인"), _S(2414, "구매물품 보관소의 공간이 부족합니다."), UMBS_OK);
			break;
		case MSG_EX_CASHITEM_ERROR_LACKINVEN:			// 케릭터 인벤 부족
			pUIManager->GetCashShopEX()->Message(MSGCMD_CASH_EX_ERROR, _S(191, "확인"), _S(265, "캐릭터 인벤토리 공간이 부족합니다."), UMBS_OK);
			break;
		case MSG_EX_CASHITEM_ERROR_CONN:					// 커넥터 서버 에러
			pUIManager->GetCashShopEX()->Message(MSGCMD_CASH_EX_ERROR, _S(191, "확인"), _S(2415, "커넥터 서버 에러입니다."), UMBS_OK);
			break;
		case MSG_EX_CASHITEM_ERROR_BILL:					// 빌링 서버 에러
			pUIManager->GetCashShopEX()->Message(MSGCMD_CASH_EX_ERROR, _S(191, "확인"), _S(2416, "빌링 서버 에러입니다."), UMBS_OK);
			break;
		case MSG_EX_CASHITEM_ERROR_NOTHAVECT :
			pUIManager->GetCashShopEX()->Message(MSGCMD_CASH_EX_ERROR, _S(191, "확인"), _S(2769, "품절된 상품 입니다."), UMBS_OK);
			break;


		// 선물 관련 :Su-won	|---------->
		case MSG_EX_CASHITEM_ERROR_GIFT_SUCCESS:
			pUIManager->GetCashShopEX()->Message(MSGCMD_CASH_EX_ERROR, _S(3102, "선물 보내기"), _S(3106, "선물 보내기가 완료되었습니다."), UMBS_OK);
			return TRUE;
		case MSG_EX_CASHITEM_ERROR_GIFT_WRONGCHAR:
			pUIManager->GetCashShopEX()->Message(MSGCMD_CASH_EX_ERROR, _S(3107, "선물 보내기 에러"), _S(3108, "해당 캐릭터에게로 선물을 보낼 수 없습니다. 캐릭터 명을 정확하게 입력해 주세요."), UMBS_OK);
			break;
		case MSG_EX_CASHITEM_ERROR_CANT_GIFT:
			pUIManager->GetCashShopEX()->Message(MSGCMD_CASH_EX_ERROR, _S(3107, "선물 보내기 에러"), _S(5366, "이벤트 상품은 선물이 불가능합니다."), UMBS_OK);
			break;
		case MSG_EX_CASHITEM_ERROR_GIFTRECV_SUCCESS:
			pUIManager->GetCashShopEX()->Message(MSGCMD_CASH_EX_ERROR, _S(3109, "선물 받기"), _S(3110, "선물 받은 물품들이 캐릭터 아이템 인벤토리로 옮겨졌습니다."), UMBS_OK);
			pUIManager->GetCashShopEX()->SetLoadDataState(FALSE);
			return TRUE;
		// 선물 관련 :Su-won	<----------|
	}

	pUIManager->GetCashShopEX()->SetLoadDataState(FALSE);
	return FALSE;

}
void CheckSocketSystemError(UBYTE errcode);

#ifdef XTRAP_SECURE_CKBANG_2010_07_20
    #include <Engine/XTrapInterface/XTrapInterface.h>
#endif

void CheckTimerEvent()
{
	if (IS_EVENT_ON(A_EVENT_SONGKRAN))
	{
		_pNetwork->ga_World.loadTradeItem();
	}

	if (IS_EVENT_ON(A_EVENT_HOLLOWEEN))
	{
		g_fGWTime = 30*3600; // 할로윈데이2007이벤트에서는 계속 밤을 유지 한다.
		g_fGWTimeMul = 0.0f;
	}
	else
	{
		g_fGWTime = CUIManager::getSingleton()->GetRadar()->GetHour() * 3600;
		g_fGWTimeMul = 1.0f;
	}

	FOREACHINDYNAMICCONTAINER( _pNetwork->ga_World.wo_cenEntities, CEntity, iten) 
	{
		if( iten->GetName() == CTString("Color controller") || iten->GetName() == CTString("SunMoon") )
		{
			iten->Initialize();
		}
	}

	if (!_pUIBuff->IsBuffBySkill(564))
	{
		if (CUIManager::getSingleton()->GetInventory()->GetWearingBtn(WEAR_HELMET)->IsEmpty() == false)
		{
			int iItemIndex = CUIManager::getSingleton()->GetInventory()->GetWearingBtn(WEAR_HELMET)->getIndex();
			if ((( ( iItemIndex >= 4927 && iItemIndex <= 4932 ) || iItemIndex == 6228) && IS_EVENT_ON(A_EVENT_HOLLOWEEN))// 호박탈 기간제가 추가되었다.
				|| (iItemIndex >= 7253 && iItemIndex <= 7259)) // [ldy1978220 2011/5/31] 가미고 10주년 이벤트 관련 추가 아이템
			{ // 호박탈은 hidden 속성의 npc를 볼 수 있다.
				((CPlayerEntity*)CEntity::GetPlayerEntity(0))->SetFlagOn(ENF_SHOWHIDDEN);
			}
			else
			{
				((CPlayerEntity*)CEntity::GetPlayerEntity(0))->SetFlagOff(ENF_SHOWHIDDEN);
			}
		}
	}
}

// ----------------------------------------------------------------------------
// Name : ReceiveExtendMessage()
// Desc : 
// ----------------------------------------------------------------------------
void CSessionState::ReceiveExtendMessage( CNetworkMessage *istr )
{
	int		i;
	LONG	lType;
	UBYTE	errcode;
	UBYTE	exType;
	SLONG	nServerTime;
	(*istr) >> lType;
	CTString		strTitle,strMessage,newName;
	ULONG			nIndex;
	UBYTE			nJob;
	BYTE			bIsGuildName;
	CUIMsgBox_Info	MsgBoxInfo;

	CUIManager* pUIManager = CUIManager::getSingleton();

	switch(lType)
	{
	case MSG_EX_PET_STATUS:
		ReceivePetStatusMessage(istr);
		break;
	case MSG_EX_PET_MOUNT:
		ReceivePetMountMessage(istr);
		break;
	case MSG_EX_PET_LEARN:			// 펫 교육			: skillindex(n) level(c:server) errorcode(n:server)
		ReceivePetLearnMessage(istr);
		break;
	case MSG_EX_PET_SKILLLIST:		// 펫 스킬 리스트	: index(n) count(n) [skillindex(n) skilllevel(c)] ...
		ReceivePetSkillListMessage(istr);
		break;
	case MSG_EX_PET_RESET_SKILL:	// 펫 스킬 초기화
		ReceivePetResetSkillMessage(istr);
		break;
	case MSG_EX_PET_SELL_INFO:		// 펫 교환/판매 정보: ownerindex(n) petindex(n) petTypeGrade(c) level(n) exp(ll) needexp(ll) hp(n) maxhp(n) ability(n) hungry(n) maxhugry(n) sympathy(n) maxsympathy(n)
		ReceivePetSellInfoMessage(istr);
		break;
	case MSG_EX_PET_CHANGE_MOUNT:	// 펫 타입 변경		: errorcode(n:server)
		ReceivePetChangeMountMessage(istr);
		break;	
	
	case MSG_EX_PET_COMMAND:		// 펫 사교동작		: pet_index(n) command_skill_index(n) targettype(c) targetindex(n)
		{
			((CPlayerEntity*)CEntity::GetPlayerEntity(0))->Read_net_Pet(MSG_EX_PET_COMMAND, &(*istr));			
		}
		break;
	case MSG_EX_PET_LEVELUP: // 펫 레벨업 petindex(n) typegrade(c) level(n)
		ReceivePetLevelUpMessage(istr);
		break;
	case MSG_EX_PET_MIX_ITEM:		// 펫 이아템 조합	: 
		ReceviePetItemMixMessage(istr);
		break;
	case MSG_EX_PET_CHANGE_ITEM:	// 펫 아이템 교환  
		ReceviePetItemChangeMessage(istr);
		break;
	case MSG_EX_PET_REBIRTH:
		ReceviePetRebirthMessage(istr); // 펫 봉인 해제
		break;
	
	case MSG_EX_PARTY_RECALL:
		ReceivePartyRecall(istr);	// CashItem 060306
		break;

	case MSG_EX_NAMECHANGE :
		(*istr) >> errcode;
		switch(errcode)
		{
			case MSG_EX_NAMECHANGE_ERROR_SUCCESS:
				(*istr) >> newName;
				(*istr) >> bIsGuildName;
				if(bIsGuildName)
				{
					_pNetwork->MyCharacterInfo.strGuildName=newName;
					if(pUIManager->DoesMessageBoxExist(MSG_EX_NAMECHANGE_ERROR_SUCCESS)) return;
					strTitle	=	_S(191,"확인");
					strMessage	=_S( 2131, 	"길드장에 의해 길드 이름이 변경되었습니다" ); 
					MsgBoxInfo.SetMsgBoxInfo(strTitle,UMBS_OK,UI_NONE,MSGCMD_NULL);
					MsgBoxInfo.AddString(strMessage);
					pUIManager->CreateMessageBox(MsgBoxInfo);
					
				}
				else 
				{
					_pNetwork->MyCharacterInfo.name=newName;
					if(pUIManager->DoesMessageBoxExist(MSG_EX_NAMECHANGE_ERROR_SUCCESS)) return;
					strTitle	=	_S(191,"확인");
					strMessage	=_S( 2132, 	"이름이 성공적으로 변경되었습니다" ); 
					MsgBoxInfo.SetMsgBoxInfo(strTitle,UMBS_OK,UI_NONE,MSGCMD_NULL);
					MsgBoxInfo.AddString(strMessage);
					pUIManager->CreateMessageBox(MsgBoxInfo);
					
					//wooss 051004 개명시 메신저상 이름도 변경
					pUIManager->GetMessenger()->SetMyInfo(-1,newName);
					
				}
				break;
				
			case MSG_EX_NAMECHANGE_ERROR_DUPLICATE:
				if(pUIManager->DoesMessageBoxExist(MSG_EX_NAMECHANGE_ERROR_DUPLICATE)) return;
				strTitle	=	_S(191,"확인");
				strMessage	=_S(12,"중복된 이름 입니다."); 
				MsgBoxInfo.SetMsgBoxInfo(strTitle,UMBS_OK,UI_NONE,MSGCMD_NULL);
				MsgBoxInfo.AddString(strMessage);
				pUIManager->CreateMessageBox(MsgBoxInfo);
				break;
				
			case MSG_EX_NAMECHANGE_ERROR_HELPER:
				if(pUIManager->DoesMessageBoxExist(MSG_EX_NAMECHANGE_ERROR_HELPER)) return;
				strTitle	=	_S(191,"확인");
				strMessage	=	_S( 2133, "헬퍼서버 오류 입니다" ); 
				MsgBoxInfo.SetMsgBoxInfo(strTitle,UMBS_OK,UI_NONE,MSGCMD_NULL);
				MsgBoxInfo.AddString(strMessage);
				pUIManager->CreateMessageBox(MsgBoxInfo);
				break;
				
			case MSG_EX_NAMECHANGE_ERROR_INVALID:
				if(pUIManager->DoesMessageBoxExist(MSG_EX_NAMECHANGE_ERROR_INVALID)) return;
				strTitle	=	_S(191,"확인");
				strMessage	=_S( 2134, 	"올바른 형식의 이름이 아닙니다" ); 
				MsgBoxInfo.SetMsgBoxInfo(strTitle,UMBS_OK,UI_NONE,MSGCMD_NULL);
				MsgBoxInfo.AddString(strMessage);
				pUIManager->CreateMessageBox(MsgBoxInfo);
				break;
				
			case MSG_EX_NAMECHANGE_ERROR_NOTGUILDBOSS:
				if(pUIManager->DoesMessageBoxExist(MSG_EX_NAMECHANGE_ERROR_NOTGUILDBOSS)) return;
				strTitle	=	_S(191,"확인");
				strMessage	=_S( 2136, 	"길드이름은 길드장만이 변경할 수 있습니다" ); 
				MsgBoxInfo.SetMsgBoxInfo(strTitle,UMBS_OK,UI_NONE,MSGCMD_NULL);
				MsgBoxInfo.AddString(strMessage);
				pUIManager->CreateMessageBox(MsgBoxInfo);
				break;	

			case MSG_EX_NAMECHANGE_ERROR_PARTY:
				if(pUIManager->DoesMessageBoxExist(MSG_EX_NAMECHANGE_ERROR_PARTY)) return;
				strTitle	=	_S(191,"확인");
				strMessage	=_S(4718, "파티중에는 이름을 변경할 수 없습니다." ); 
				MsgBoxInfo.SetMsgBoxInfo(strTitle,UMBS_OK,UI_NONE,MSGCMD_NULL);
				MsgBoxInfo.AddString(strMessage);
				pUIManager->CreateMessageBox(MsgBoxInfo);
				break;
			}
			break;

		case MSG_EX_CASHITEM:			// 아이템 구매		: subtype(uc) ...
			{
				ULONG tv_cash, /*nCtid,*/ nCount;
				(*istr) >> exType;
				
				switch(exType)
				{
				case MSG_EX_CASHITEM_TYPE_LIST_REP:
					{ // max type count를 기준으로 사용하는 index와 사용 않는 index를 분별한다.
						pUIManager->GetCashShopEX()->SetCashType(istr);
					}
					break;
				case MSG_EX_CASHITEM_LIST_REP:
					{
						pUIManager->GetCashShopEX()->SetGoodsList(istr);
					}
					break;
				case MSG_EX_CASHITEM_RECOMMAND_REP:
					{
						pUIManager->GetCashShopEX()->SetRecommandList(istr);
					}
					break;
				case MSG_EX_CASHITEM_WISHLIST_REP:
					{
						pUIManager->GetCashShopEX()->SetWishList(istr);
					}
					break;

					case MSG_EX_CASHITEM_BALANCE_REP :		// 캐쉬				: errorCode(uc) cashBalance(n)
						{
							(*istr) >> errcode;
							if(CheckCashItemMessage(errcode))
							{
								(*istr) >> 	tv_cash;
								pUIManager->GetCashShopEX()->SetMyCash(static_cast<SQUAD>(tv_cash));
							}
						}
						break;			
					case MSG_EX_CASHITEM_PURCHASE_REP :		// 응답				: errorCode(uc) cashBalance(n)
					case MSG_EX_CASHITEM_PURCHASE_WITH_COUPON_REP : // 쿠폰 구매 응답 : errorCode(uc) cashBalance(n)
						{							
							(*istr) >> errcode;
							if(CheckCashItemMessage(errcode))
							{
								(*istr) >> 	tv_cash;
								pUIManager->GetCashShopEX()->SetMyCash(static_cast<SQUAD>(tv_cash));
								pUIManager->GetCashShopEX()->CompletePurchase();
							}
						}
						break;
					case MSG_EX_CASHITEM_BRING_REP:			// 응답				: errorCode(uc) 
						{
							(*istr) >> errcode;
							if(CheckCashItemMessage(errcode)){
								pUIManager->GetCashShopEX()->CompleteBringItems();
							}
						}
						break;	
					case MSG_EX_CASHITEM_PURCHASELIST_REP:   // 응답				: count(n) idx(n) ctid(n)
						{
							(*istr) >> errcode;
							if(CheckCashItemMessage(errcode))
							{
								pUIManager->GetCashShopEX()->SetPurchaseItemList(istr);
							}
						}
						break;
					case MSG_EX_CASHITEM_PURCHASEHISTORY_REP:// 응답				: errorcode(uc) count(n) ctid(n)
						{
							(*istr) >> errcode;
							if(CheckCashItemMessage(errcode)){
								(*istr >> nCount);
								if( nCount > 0){
// 									pUIManager->GetCashShop()->m_abtnHistoryItems.New(nCount);
// 									pUIManager->GetCashShop()->m_sbHistory.SetScrollPos( 0 );
// 									pUIManager->GetCashShop()->m_sbHistory.SetScrollRange(nCount);
// 									pUIManager->GetCashShop()->m_sbHistory.SetCurItemCount(nCount);
// 									for( int i=0; i < nCount ;i++){
// 										CUIButtonEx tv_btn;
// 										ULONG nItemCnt;
// 										(*istr) >> nItemCnt;
// 										(*istr) >> nCtid;
// 										pUIManager->GetCashShop()->m_abtnHistoryItems[i].Create(NULL,-1,-1,BTN_SIZE,BTN_SIZE,UI_CASH_SHOP);
// 										if(pUIManager->GetCashShop()->SetCashIndexToBtn(nCtid,-1,tv_btn)){
// 											pUIManager->GetCashShop()->m_abtnHistoryItems[i] = tv_btn;
// 											pUIManager->GetCashShop()->m_abtnHistoryItems[i].Copy(tv_btn);
// 											pUIManager->GetCashShop()->m_abtnHistoryItems[i].SetItemCount(nItemCnt);
// 										}
// 									}
								}
							}						
						}
						break;
					case MSG_EX_CASHITEM_MOONSTONE_START :
						{
							(*istr) >> errcode;
							if(CheckCashMoonStone(errcode)){
								UBYTE grade;
								(*istr) >> grade;
								grade =5-grade;
								pUIManager->GetGamble()->SetSelectedMarker( grade, grade );
								pUIManager->GetGamble()->Start();
							}						
						}
						break;

					// 선물 관련 :Su-won	|--------------------------------------------------->
					case MSG_EX_CASHITEM_GIFT_REP:					//응답	: errcode(uc) 
						{
							(*istr) >> errcode;
							if(CheckCashItemMessage(errcode))
							{
								pUIManager->GetCashShopEX()->CompleteBringItems();
							}
						}
						break;
					case MSG_EX_CASHITEM_WISHLIST_SAVE_REP: // 소망상자 아이템 등록 결과
						{
							//(*istr) >> errcode;
							pUIManager->GetCashShopEX()->RevWishList(istr, TRUE);
						}
						break;
					case MSG_EX_CASHITEM_WISHLIST_DEL_REP:
						{
							pUIManager->GetCashShopEX()->RevWishList(istr, FALSE);
						}
						break;
					case MSG_EX_CASHITEM_GIFT_SENDHISTORY_REP:		// 응답	: errorcode(uc) count(n) ctid(n) recvcharName(str)
						{
							(*istr) >> errcode;
							if(CheckCashItemMessage(errcode))
							{
								(*istr >> nCount);
								if( nCount > 0)
								{
// 									pUIManager->GetCashShop()->m_abtnSendHistoryItems.New(nCount);
// 									pUIManager->GetCashShop()->m_sbGift.SetScrollPos( 0 );
// 									pUIManager->GetCashShop()->m_sbGift.SetScrollRange(nCount);
// 									pUIManager->GetCashShop()->m_sbGift.SetCurItemCount(nCount);
// 
// 									pUIManager->GetCashShop()->m_astrSendChar.New(nCount);
// 									for( int i=0; i < nCount ;i++)
// 									{
// 										CUIButtonEx tv_btn;
// 										//ULONG nItemCnt;
// 										(*istr) >> nCtid;
// 										(*istr) >> newName;
// 										pUIManager->GetCashShop()->m_abtnSendHistoryItems[i].Create(NULL,-1,-1,BTN_SIZE,BTN_SIZE,UI_CASH_SHOP);
// 										if(pUIManager->GetCashShop()->SetCashIndexToBtn(nCtid,-1,tv_btn))
// 										{
// 											pUIManager->GetCashShop()->m_abtnSendHistoryItems[i] = tv_btn;
// 											pUIManager->GetCashShop()->m_abtnSendHistoryItems[i].Copy(tv_btn);
// 											//pUIManager->GetCashShop()->m_abtnSendHistoryItems[i].SetItemCount(nItemCnt);
// 											pUIManager->GetCashShop()->m_astrSendChar[i] =newName;
// 										}
// 									}
								}
							}
						}
						break;
					case MSG_EX_CASHITEM_GIFT_RECVHISTORY_REP:// 응답				: errorcode(uc) count(n) ctid(n) recvcharName(str)
						{
							(*istr) >> errcode;
							if(CheckCashItemMessage(errcode))
							{
								(*istr >> nCount);
								if( nCount > 0)
								{
// 									pUIManager->GetCashShop()->m_abtnRecvHistoryItems.New(nCount);
// 									pUIManager->GetCashShop()->m_sbGift.SetScrollPos( 0 );
// 									pUIManager->GetCashShop()->m_sbGift.SetScrollRange(nCount);
// 									pUIManager->GetCashShop()->m_sbGift.SetCurItemCount(nCount);
// 
// 									pUIManager->GetCashShop()->m_astrReceiveChar.New(nCount);
// 									for( int i=0; i < nCount ;i++)
// 									{
// 										CUIButtonEx tv_btn;
// 										//ULONG nItemCnt;
// 										//(*istr) >> nItemCnt;
// 										(*istr) >> nCtid;
// 										(*istr) >> newName;
// 										pUIManager->GetCashShop()->m_abtnRecvHistoryItems[i].Create(NULL,-1,-1,BTN_SIZE,BTN_SIZE,UI_CASH_SHOP);
// 										if(pUIManager->GetCashShop()->SetCashIndexToBtn(nCtid,-1,tv_btn))
// 										{
// 											pUIManager->GetCashShop()->m_abtnRecvHistoryItems[i] = tv_btn;
// 											pUIManager->GetCashShop()->m_abtnRecvHistoryItems[i].Copy(tv_btn);
// 											//pUIManager->GetCashShop()->m_abtnReceiveHistoryItems[i].SetItemCount(nItemCnt);
// 											pUIManager->GetCashShop()->m_astrReceiveChar[i] =newName;
// 										}										
// 									}
								}
							}
						}
						break;
					case MSG_EX_CASHITEM_GIFT_RECVLIST_REP:// 응답
						{
							pUIManager->GetCashShopEX()->RevGiftList(istr);
						}
						break;
					case MSG_EX_CASHITEM_GIFT_RECV_REP:		// 응답				: errCode(uc)
						{
							(*istr) >> errcode;
							if (CheckCashItemMessage(errcode))
							{
								pUIManager->GetCashShopEX()->RevResultGetGift();
							}
						}
						break;
					case MSG_EX_CASHITEM_GIFT_NOTICE: // 캐쉬 아이템 선물 공지 메세지
						{
							BYTE bGift;

							(*istr) >> bGift;
							// MSG_EX_CASHITEM_GIFT_NOTICE가 Party 및 원정대 정보 다음으로 항상 받게 되는 메세지라는 조건으로
							// 존 이동 중 파티 및 원정대가 해체 되었을 때 서버로 부터 정보가 없는 것으로 판단하여 해체 처리
							Party* pParty = GAMEDATAMGR()->GetPartyInfo();

							if (pParty == NULL)
								return;

							if( pParty->GetIsPartyPlay() == FALSE)
							{
								if( pUIManager->IsCSFlagOn(CSF_EXPEDITION) )
								{
									pParty->ExpeditionEnd();
									pUIManager->GetChattingUI()->AddSysMessage( _S( 4665, "원정대가 해체되었습니다." ) );
								}
								
								if( pUIManager->IsCSFlagOn(CSF_PARTY) )
									pParty->PartyEnd();
							}
							//////////////////////////////////////////////////////////////////////////
							pUIManager->GetQuickSlot()->SetGiftRecv(bGift);
						}
						break;
					// 선물 관련 :Su-won	<---------------------------------------------------|
					case MSG_EX_CASHITEM_CUPON_REP: // 보유 쿠폰 리스트 응답
						{
							(*istr) >> errcode;

							if (CheckCashItemMessage(errcode))
							{ // pUIManager->GetCashShop()->m_pConfirmInfo NULL 체크 안함 
								// 이하 리스트를 저장
								SLONG slCuponCout;
								(*istr) >> slCuponCout;

								//pUIManager->GetCashShop()->m_pConfirmInfo->ClearCuponData(); // 무조건 리스트 클리어

								if (slCuponCout > 0) // 쿠폰 존재시
								{
									int i;

									for (i=0; i<slCuponCout; i++)
									{
										CTString strCuponName;
										SLONG slCuponID;
										SLONG slDCPoint;
										SLONG slLimitPoint = 0;

										(*istr) >> strCuponName; // 쿠폰 이름
										(*istr) >> slCuponID; // 쿠폰 아이디
										(*istr) >> slDCPoint; // 할인 금액
										(*istr) >> slLimitPoint; // 할인 제한 금액

										//pUIManager->GetCashShop()->m_pConfirmInfo->AddCuponData(strCuponName, slCuponID, slDCPoint, slLimitPoint);
									}
								}

								//pUIManager->GetCashShop()->m_pConfirmInfo->SetEnable(TRUE);
								//pUIManager->GetCashShop()->m_pConfirmInfo->SetVisible(TRUE);
							}
						}
					case MSG_EX_CASHITEM_SHOP_LOCK:
						{
							pUIManager->GetCashShopEX()->ForcedExit();
						}
						break;
					case MSG_EX_CASHITEM_SHOP_UNLOCK:
						{
							pUIManager->GetCashShopEX()->SetCashShopLock(FALSE);
						}
						break;
				}
			}
			break;

		// wooss 051004 개명후 메신저 등록
		case MSG_EX_FRIENDNAMECHANGE :
			(*istr) >> nIndex;
			(*istr) >> newName;
			(*istr) >> nJob;
			pUIManager->GetMessenger()->DeleteMember(nIndex);
			pUIManager->GetMessenger()->AddFriendList(nIndex, 0, newName,(eJob)nJob);
			break;

		case MSG_EX_CASTLE_MAP_RECENT:
			{
				FLOAT	fLordX, fLordZ;
				FLOAT	nLordHP, nLordMaxHP;
				
				LONG	lTowerCount;
				
				LONG	lTowerIndex;
				FLOAT	fTowerX, fTowerZ;
				LONG	nTowerHP, nTowerMaxHP;

				FLOAT	fRegenX, fRegenY;
				LONG	nSenderFlag;
				LONG	nSenderIndex;
				FLOAT	fSignalX, fSignalY;
				
				extern SLONG	g_slZone;

				if( _pUISWDoc->IsWar() ) 
				{
					pUIManager->GetMap()->SetCurrentWorldMap( _pNetwork->MyCharacterInfo.zoneNo, 1 );
					pUIManager->GetMap()->ReSetData();
				}
				else
				{
					pUIManager->GetMap()->SetCurrentWorldMap( _pNetwork->MyCharacterInfo.zoneNo, 0 );
					return;
				}

				pUIManager->GetMap()->InitCastleData();
				// 권좌
				(*istr) >> fLordX;
				(*istr) >> fLordZ;
				(*istr) >> nLordHP;
				(*istr) >> nLordMaxHP;
				
				pUIManager->GetMap()->AddCastleData( CASTLE_LORD, fLordX, fLordZ, -1, nLordHP, nLordMaxHP );

				// 공성 탑 
				(*istr) >> lTowerCount;

				for( int i = 0; i < lTowerCount; i++ )
				{
					(*istr) >> lTowerIndex;
					(*istr) >> fTowerX;
					(*istr) >> fTowerZ;
					(*istr) >> nTowerHP;
					(*istr) >> nTowerMaxHP;

					pUIManager->GetMap()->AddCastleData( CASTLE_TOWER, fTowerX, fTowerZ, lTowerIndex, nTowerHP, nTowerMaxHP );
				}

				// 리젠 포인트
				(*istr) >> fRegenX;
				(*istr) >> fRegenY;
				
				pUIManager->GetMap()->AddCastleData( CASTLE_REGEN, fRegenX, fRegenY );
				
				// Final Signal... 
				(*istr) >> nSenderFlag;
				(*istr) >> nSenderIndex;
				(*istr) >> fSignalX;
				(*istr) >> fSignalY;
					
				pUIManager->GetMap()->AddSignal( fSignalX, fSignalY, nSenderFlag, nSenderIndex );

			}
			break;

		case MSG_EX_CASTLE_MAP_SIGNAL:
			{
				LONG	nSenderFlag;
				LONG	nSenderIndex;
				FLOAT	fSignalX, fSignalY;
								 
				(*istr) >> fSignalX;
				(*istr) >> fSignalY;
				(*istr) >> nSenderFlag;
				(*istr) >> nSenderIndex;
				
				pUIManager->GetMap()->AddSignal( fSignalX, fSignalY, nSenderFlag, nSenderIndex );
			}
			break;

		case MSG_EX_ELEMENTAL_DELETE:
			{
				ReceiveElementalDeleteMessage( istr );
			}
			break;

		case MSG_EX_EVOCATION_START:
			{
				ReceiveEvocationStart( istr );
			}
			break;

		case MSG_EX_EVOCATION_STOP:
			{
				ReceiveEvocationStop( istr );
			}
			break;
			
		case MSG_EX_GUILDMEMBERNAMECHANGE :
			{
				ULONG	 charIndex; 
				(*istr) >> charIndex;
				(*istr) >> newName;
			//	if(_pNetwork->MyCharacterInfo.index == charIndex)
				pUIManager->GetGuild()->SetMemberName(charIndex,newName);

			}
			break;
		case MSG_EX_PARTY_MATCH :		// Date : 2006-05-09(오후 5:43:02), By eons
			{
				ULONG	ErrorType;
				ULONG	MatchType;
				(*istr) >> MatchType;

				switch( MatchType )
				{
				case MSG_EX_PARTY_MATCH_REG_MEMBER_REP:
					{
						(*istr) >> ErrorType;
						
						pUIManager->GetPartyAuto()->MatchRegMemberRep( ErrorType );
					}
					break;
				case MSG_EX_PARTY_MATCH_REG_PARTY_REP:
					{
						(*istr) >> ErrorType;

						pUIManager->GetPartyAuto()->MatchRegPartyRep( ErrorType );
					}
					break;
				case MSG_EX_PARTY_MATCH_MEMBER_LIST_REP:
					{
						pUIManager->GetPartyAuto()->ReceiveMemberData( istr );
					}
					break;
				case MSG_EX_PARTY_MATCH_PARTY_LIST_REP:
					{
						pUIManager->GetPartyAuto()->ReceivePartyData( istr );
					}
					break;
				case MSG_EX_PARTY_MATCH_DEL_REP:
					{
						strMessage = _S( 2732, "등록이 삭제 되었습니다." );
						MsgBoxInfo.SetMsgBoxInfo( _S( 2733, "등록 삭제" ), UMBS_OK, UI_NONE, MSGCMD_NULL );
						MsgBoxInfo.AddString( strMessage );
						pUIManager->CreateMessageBox( MsgBoxInfo );						
					}
					break;
				case MSG_EX_PARTY_MATCH_INVITE_REP:
					{
						(*istr) >> ErrorType;

						pUIManager->GetPartyAuto()->ReceivePartyInviteMessage( ErrorType, istr );
					}
					break;
				case MSG_EX_PARTY_MATCH_JOIN_REP:
					{
						(*istr) >> ErrorType;

						pUIManager->GetPartyAuto()->ReceivePartyJoinMessage( ErrorType, istr );
					}
					break;
				}
			}
			break;
		case MSG_EX_STRING :
			{
				BYTE	outType;
				LONG	strIdx,argCnt,argType;
				(*istr) >> outType >> strIdx >> argCnt;

				if(argCnt>0)
				{
					for(int i=0;i<argCnt;i++)
					{
						(*istr) >> argType;

						// arg 1
						if(argCnt==1){
							switch(argType)
							{
								case MSG_EX_STRING_CHAR : 		// char
									{
										BYTE arg1;
										(*istr) >> arg1;
										strMessage.PrintF(_S(strIdx,""),arg1);
									}
									break;
								case MSG_EX_STRING_INT :			// int
									{
										LONG arg1;
										(*istr) >> arg1;
										strMessage.PrintF(_S(strIdx,""),arg1);
									}
									break;
								case MSG_EX_STRING_LONGLONG :	// LONGLONG
									{
										LONGLONG arg1;
										(*istr) >> arg1;
										strMessage.PrintF(_S(strIdx,""),arg1);
									}
									break;
								case MSG_EX_STRING_STR :			// str(50)
									{
										CTString arg1;
										(*istr) >> arg1;
										strMessage.PrintF(_S(strIdx,""),arg1);
									}
									break;
							}

						}
				/*		// arg 2
						if(argCnt==2){
							if(i==1)
							switch(argType)
							{
								case MSG_EX_STRING_CHAR : 		// char
									{
										BYTE arg1;
										arg1 = new BYTE;
										(*istr) >> ((BYTE)(*arg1));
									}
									break;
								case MSG_EX_STRING_INT :			// int
									{
										static LONG arg1;
										(*istr) >> arg1;
									}
									break;
								case MSG_EX_STRING_LONGLONG :	// LONGLONG
									{
										static LONGLONG arg1;
										(*istr) >> arg1;
									}
									break;
								case MSG_EX_STRING_STR :			// str(50)
									{
										static CTString arg1;
										(*istr) >> arg1;
									}
									break;
							}
							if(i==2)
							switch(argType)
							{
								case MSG_EX_STRING_CHAR : 		// char
									{
										BYTE arg2;
										(*istr) >> arg2;
										strMessage.PrintF(_S(strIdx,""),arg1,arg2);
									}
									break;
								case MSG_EX_STRING_INT :			// int
									{
										LONG arg2;
										(*istr) >> arg2;
										strMessage.PrintF(_S(strIdx,""),arg1,arg2);
									}
									break;
								case MSG_EX_STRING_LONGLONG :	// LONGLONG
									{
										LONGLONG arg2;
										(*istr) >> arg2;
										strMessage.PrintF(_S(strIdx,""),arg1,arg2);
									}
									break;
								case MSG_EX_STRING_STR :			// str(50)
									{
										CTString arg2;
										(*istr) >> arg2;
										strMessage.PrintF(_S(strIdx,""),arg1,arg2);
									}
									break;
							}

							
						}*/
						
					}
				}
				else strMessage=_S(strIdx,"서버 호출 스트링");

				switch(outType)
				{
					case MSG_EX_STRING_OUTPUT_WINDOW :		// 창
						{
							MsgBoxInfo.SetMsgBoxInfo( _S(191,"확인" ), UMBS_OK, UI_NONE, MSGCMD_NULL );
							MsgBoxInfo.AddString(strMessage);
							pUIManager->CreateMessageBox(MsgBoxInfo);

						}
						break;
					case MSG_EX_STRING_OUTPUT_SYS :			// 시스템창
						pUIManager->GetChattingUI()->AddSysMessage(strMessage);
						break;
					case MSG_EX_STRING_OUTPUT_NOTICE :		// 공지
						_UIAutoHelp->SetGMNotice ( strMessage );
						break;
					case MSG_EX_STRING_OUTPUT_CHAT :		// 챗창
						 pUIManager->GetChattingUI()->AddChatMessage(MSG_CHAT_SAY,
							 _pNetwork->MyCharacterInfo.index,_pNetwork->MyCharacterInfo.name,strMessage);
						break;
				}
				
			}
			break;

		case MSG_EX_MESSENGER:
				ReceiveMessengerExMessage( istr );
				break;
		
		case MSG_EX_HAIR_CHANGE:
			{
				ReceiveExHairChange(istr);
			}
			break;
		case MSG_EX_PLAYER_STATE_CHANGE:
			{
				ReceiveExPlayerStateChange(istr);
			}
			break;
		case MSG_EX_NPROTECT:
			{
#ifndef NO_GAMEGUARD
				ReceiveExnProtect(istr);
#endif
			}
			
			break;
		case MSG_EX_PET_CHANGE_NAME:
			{
				ReceiveExPetNameChange(istr);
			}
			break;
			
		case MSG_EX_CASTLE_WAR: // WSS_DRATAN_SEIGEWARFARE 2007/07/30
		{
			UBYTE bType;
			ULONG charIdx;
			CTString tStr, tStrtmp;
			(*istr) >> bType;
			switch (bType)
			{
				case MSG_CASTLE_CRISTAL_RESPOND_START:
					(*istr) >> charIdx;
					pUIManager->GetSiegeWarfareNew()->StartConsensus(charIdx);
					break;
				case MSG_CASTLE_CRISTAL_RESPOND_END:	// 교감 완료								
					(*istr) >> charIdx;
					(*istr) >> tStrtmp;
					tStr.PrintF(_S(3898,"[%s]님이 크리스탈 교감에 성공하였습니다."), tStrtmp);
					pUIManager->GetChattingUI()->AddSysMessage( tStr, SYSMSG_ERROR );
					pUIManager->GetSiegeWarfareNew()->CompleteConsensus(charIdx);
					break;
				case MSG_CASTLE_CRISTAL_RESPOND_FAIL:	// 교감 실패
					(*istr) >> charIdx;
					if( _pNetwork->MyCharacterInfo.index == charIdx)
					{
						//pUIManager->GetChattingUI()->AddSysMessage( _S( 3751,"교감에 실패하였습니다."), SYSMSG_ERROR );
						//CUIMsgBox_Info	MsgBoxInfo;
						MsgBoxInfo.SetMsgBoxInfo( _S(191,"확인"), UMBS_OK, UI_NONE, MSGCMD_NULL);
						MsgBoxInfo.AddString(_S(3751,"교감에 실패하였습니다."), 0xE28769FF);
						pUIManager->CreateMessageBox( MsgBoxInfo );
					}
					pUIManager->GetSiegeWarfareNew()->StopConsensus(charIdx);					
					break;
				case MSG_CASTLE_TOWER_CONTRAL:
					{
						pUIManager->GetChattingUI()->AddSysMessage( _S( 3752,"정상적으로 가동되었습니다." ), SYSMSG_ERROR );
					}
					break;
				case MSG_CASTLE_TOWER_CONTRAL_LIST:		// 마스트 타워 설정 리스트
					{
						UBYTE tSet[DRATAN_TOWER_KIND_MAX];
						for(int i=0;i<DRATAN_TOWER_KIND_MAX;i++)
						{
							(*istr) >> tSet[i];
							pUIManager->GetSiegeWarfareNew()->SetTowerSet(i,tSet[i]);
						}
						
						pUIManager->GetSiegeWarfareNew()->SetCBFromTowerSet();
						pUIManager->GetSiegeWarfareNew()->OpenCheckTower();						
						
					}
					break;
				case MSG_CASTLE_TOWER_REINFORCE:
					{
						CTString tStr,tStr2;
						UBYTE tType,tLevel;
						(*istr) >> tType >> tLevel;
						switch(tType)
						{
						case 0:
							tStr = _S( 3753,"공격형 타워");
							break;
						case 1:
							tStr = _S( 3754,"가드형 타워");
							break;
						case 2:
							tStr = _S( 3755,"성문");
							break;
						}						
						tStr2.PrintF("%d %s",(int)tLevel, _S( 3756,"단계가 정상적으로 강화되었습니다." ));
						tStr += tStr2;
						pUIManager->GetChattingUI()->AddSysMessage( tStr, SYSMSG_ERROR );
					}
					break;
				case MSG_CASTLE_TOWER_REINFORCE_LIST:
					{					
						UBYTE tType,tLevel;
						(*istr) >> tType >> tLevel;		
						if( (int)tType == pUIManager->GetSiegeWarfareNew()->GetUpgradeType())
						{								
							
							pUIManager->GetSiegeWarfareNew()->SetUpgradedLevel(int(tLevel));
							pUIManager->GetSiegeWarfareNew()->SetUpgradePos(int(tLevel));
							pUIManager->GetSiegeWarfareNew()->SetUpgradeLevel((int)tLevel );
							pUIManager->GetSiegeWarfareNew()->OpenUpgradeTower();
						}
						else 
							pUIManager->GetChattingUI()->AddSysMessage( _S( 3757,"강화할 타워가 올바르지 않습니다." ), SYSMSG_ERROR );	
					}
					break;
				case MSG_CASTLE_TOWER_REPAIRE:			// 공성 타워 수리    cl->gs idx(n) | gs->cl idx(n)
					{
						ULONG tIdx;
						(*istr) >> tIdx;
						pUIManager->GetChattingUI()->AddSysMessage( _S( 3758,"정상적으로 수리되었습니다." ), SYSMSG_ERROR );	
						
					}
					break;
				case MSG_CASTLE_TOWER_REPAIRE_LIST:		// 공성 타워 수리 상태   cl->gs idx(n) | gs->cl money(n)
					{
						ULONG tNpcIdx;
						SQUAD tMoney;

						(*istr) >> tNpcIdx >> tMoney;
						pUIManager->GetSiegeWarfareNew()->SetRepairMoney( tNpcIdx,tMoney );
						pUIManager->GetSiegeWarfareNew()->OpenRepairTower();
					}
					break;
				case MSG_CASTLE_TOWER_WARP_LIST:
					{
						UBYTE tCnt;
						ULONG tNpcIdx;

						(*istr) >> tCnt;
						for( int i=0;i<tCnt ;i++)
						{
							(*istr) >> tNpcIdx;
							pUIManager->GetGuildWarPortal()->SetDratanPortalFlag((int)tNpcIdx);
						}
						pUIManager->GetGuildWarPortal()->OpenGuildWarPortalDratan();
					}
					break;				
				case MSG_CASTLE_QUARTERS_INSTALL:		// 부활진지 설치 cl->gs idx(n) | gs->cl  idx(n), gidx(n)
					{
						CTString tStr,tGuildName;
						ULONG tNpcIdx,tGuildIdx;
						SLONG tNpcUniIndex;
						(*istr) >> tNpcIdx >> tGuildIdx >> tGuildName >> tNpcUniIndex;
						tStr.PrintF(_S( 3759,"[%s]길드가 [%d]번 부활진지를 점령했습니다." ),tGuildName,tNpcIdx-389); // TEMP
						_pNetwork->MyCharacterInfo.mQuarter[tNpcIdx] = tGuildIdx;
						_pNetwork->MyCharacterInfo.mQuarterName[tNpcIdx] = tGuildName;						
						pUIManager->GetChattingUI()->AddSysMessage( tStr, SYSMSG_ERROR );

						// 몹 이름 변경
						CMobData* MD = CMobData::getData(tNpcIdx);
						if(MD->GetMobIndex()>0 && tGuildName.Length() >0 && tGuildIdx != -1)
						{							
							tStr = tGuildName + CTString(" ") + _S( 3760,"공성 부활진지");;
							MD->SetName(tStr);

							tStr = MD->GetName();
							MD = CMobData::getData(tNpcIdx);
							tStr = MD->GetName();
						}

						if (!MD->IsNPC())
						{
							CEntity* pEntity = NULL;

							int		i;

							for (i = 0; i < eOBJ_MAX; ++i)
							{
								pEntity = ACTORMGR()->GetEntityByIndexServer((eOBJ_TYPE)i, tNpcUniIndex);
								// 일반 npc에서 몬스터로 설정 변경(수성 측이 공격 가능한 npc로 )
								if (pEntity != NULL)
								{
									pEntity->SetFirstExtraFlagOff(ENF_EX1_NPC);
									break;
								}
							}
						}
					}
					break;
				case MSG_CASTLE_QUARTERS_CRUSH:			// 부활 진지 파괴 gs->cl idx(n)
					{
						CTString tStr;
						ULONG tNpcIdx;
						(*istr) >> tNpcIdx;
						tStr.PrintF(_S( 3705,"%d번 부활진지가 파괴되었습니다." ),tNpcIdx-389); // TEMP
						_pNetwork->MyCharacterInfo.mQuarter[tNpcIdx] = -1;
						_pNetwork->MyCharacterInfo.mQuarterName[tNpcIdx] = CTString("");
						pUIManager->GetChattingUI()->AddSysMessage( tStr, SYSMSG_ERROR );

						// Hard Cording ^^;; 어쩔수 없이....
						// 몹 이름 변경
						CMobData* MD = CMobData::getData(tNpcIdx);
						if(MD->GetMobIndex()>0)
						{
							tStr.PrintF(_S( 3685,"공성 부활진지%d"),tNpcIdx-389);
							MD->SetName(tStr);
						}						
					}
					break;
				case MSG_CASTLE_QUARTERS_LIST:			// 부활 진지 리스트 gs->cl 7*(nindex(n), gindex(n))
					{		
						CTString tGuildName;
						LONG tNpcIdx,tGuildIdx;

						for(int i=0;i<7;i++)
						{	
							(*istr) >> tNpcIdx >> tGuildIdx >> tGuildName;						
							// 점령 길드가 존재하면...
							if (tGuildIdx!=-1)
							{
								_pNetwork->MyCharacterInfo.mQuarter[tNpcIdx] = tGuildIdx;
								_pNetwork->MyCharacterInfo.mQuarterName[tNpcIdx] = tGuildName;

								// 몹 이름 변경
								CMobData* MD = CMobData::getData(tNpcIdx);
								if(MD->GetMobIndex()>0 && tGuildName.Length() >0 && tGuildIdx != -1)
								{
									tStr = tGuildName +CTString(" ") +_S( 3760,"공성 부활진지");
									MD->SetName(tStr);
								}
							}
							// 점령 길드가 없다면...
							else 
							{
								// Hard Cording ^^;; 어쩔수 없이....
								// 몹 이름 변경
								CMobData* MD = CMobData::getData(tNpcIdx);
								if(MD->GetMobIndex()>0)
								{
									tStr.PrintF(_S( 3685,"공성 부활진지%d"),tNpcIdx-389);
									MD->SetName(tStr);
								}						
							}
						}
					}
					break;

				case MSG_CASTLE_WAIT_TIME:				// 부활 대기 시간
					{
						LONG tWaitTime;
						(*istr) >> tWaitTime;						
						pUIManager->GetSiegeWarfareNew()->SetWaitTime(tWaitTime);
						pUIManager->GetSiegeWarfareNew()->SetTimeReply(TRUE);

					}
					break;
				case MSG_EX_CASTLE_ERROR:				// 공성 에러
					{
						UBYTE errcode;
						(*istr) >> errcode;
						CheckDratanSiegewarfareError(errcode);
					}

			}
		}
		break;
	
		case MSG_EX_PET_CHANGE_COLOR:
			{
				ReceiveExPetColorChange(istr);
			}
			break;
// EDIT : BS : 070413 : 신규 패킷 암호화			
#ifdef CRYPT_NET_MSG
#ifdef CRYPT_NET_MSG_MANUAL
	case MSG_EX_KEYCHANGE:		// EDIT : BS : 패킷 암호화 : 키 변경
		{
			ULONG nKey;
			(*istr) >> nKey;
			CNM_CopyKey(&_pNetwork->cnmKey, (CNM_KEY_TYPE*)&nKey);
		}
		break;
#endif // CRYPT_NET_MSG_MANUAL
#else
	case MSG_EX_KEYCHANGE:
		{
			ULONG nKey;
			(*istr) >> nKey;
			_pNetwork->cnmKey = nKey;
		}
#endif // #ifdef CRYPT_NET_MSG

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//[071123: Su-won] DRATAN_SIEGE_DUNGEON
	case MSG_EX_DVD:
		{
			UBYTE sbSubType;
			
			(*istr) >> sbSubType;

			switch(sbSubType)
			{
			case MSG_DUNGEON_INFO:
				{
					ULONG ulEnvRate, ulMonRate;
					
					(*istr) >> ulEnvRate;
					(*istr) >> ulMonRate;

					pUIManager->GetPortal()->Create_SiegeDungeon_State_MsgBox( ulEnvRate, ulMonRate);
				}
				break;
			case MSG_MANAGEMENT:
				{
					UBYTE sbSubSubType, ubError;

					(*istr) >> sbSubSubType;
					switch( sbSubSubType )
					{
					case MSG_MANAGEMENT_MANAGER_CONFIRM:				//관리 요청
						{
							(*istr) >> ubError;

							if( ubError == MSG_DVD_ERROR_OK )
							{
								pUIManager->CreateMessageBoxL( _S(3908, "던전 관리"),UI_PORTAL, MSGLCMD_DRATAN_SIEGE_DUNGEON_CONTROL);
								pUIManager->AddMessageBoxLString(MSGLCMD_DRATAN_SIEGE_DUNGEON_CONTROL,TRUE, _S(3940, "안녕하십니까? 성주님."),-1,0xa3a1a3ff);
								pUIManager->AddMessageBoxLString(MSGLCMD_DRATAN_SIEGE_DUNGEON_CONTROL,TRUE, _S(3941, "던전의 어떤 부분을 관리하시겠습니까?"),-1,0xa3a1a3ff);

								pUIManager->AddMessageBoxLString( MSGLCMD_DRATAN_SIEGE_DUNGEON_CONTROL, FALSE, _S(3921, "던전 내부 환경 제어" ), DRATAN_SIEGE_DUNGEON_CONTROL_ENVIRONMENT);
								pUIManager->AddMessageBoxLString( MSGLCMD_DRATAN_SIEGE_DUNGEON_CONTROL, FALSE, _S(3926, "던전 내부 몬스터 제어" ), DRATAN_SIEGE_DUNGEON_CONTROL_MONSTER);
								pUIManager->AddMessageBoxLString( MSGLCMD_DRATAN_SIEGE_DUNGEON_CONTROL, FALSE, _S(3931, "던전 입장료 조정" ), DRATAN_SIEGE_DUNGEON_CONTROL_ADMISSION_FEE);
								pUIManager->AddMessageBoxLString( MSGLCMD_DRATAN_SIEGE_DUNGEON_CONTROL, FALSE, _S(3935, "던전 수렵세율 조정" ), DRATAN_SIEGE_DUNGEON_CONTROL_HUNTING_FEE);
								pUIManager->AddMessageBoxLString( MSGLCMD_DRATAN_SIEGE_DUNGEON_CONTROL, FALSE, _S( 1220, "취소한다." ) );

								SERVER_INFO()->SetDG_Mode(eDUNGEON_MODE_LORD);
							}
							else if( ubError == MSG_DVD_ERROR_CONFIRM )
							{
								pUIManager->CloseMessageBox(MSGCMD_NULL);
								MsgBoxInfo.SetMsgBoxInfo( _S(191,"확인"), UMBS_OK, UI_NONE, MSGCMD_NULL);
								MsgBoxInfo.AddString( _S(3942, "성주 길드의 마스터, 또는 부길드마스터만 접근할 수 있습니다.") );
								pUIManager->CreateMessageBox( MsgBoxInfo );
							}
							else if (ubError == MSG_DVD_ERROR_ZONE)
							{
								pUIManager->CloseMessageBox(MSGCMD_NULL);
								MsgBoxInfo.SetMsgBoxInfo( _S(191,"확인"), UMBS_OK, UI_NONE, MSGCMD_NULL);
								MsgBoxInfo.AddString( _S(6275, "공성 채널에서만 이용이 가능합니다.") );
								pUIManager->CreateMessageBox( MsgBoxInfo );
							}
							else if (ubError == MSG_DVD_ERROR_NORMAL_MODE)
							{
								LONGLONG llNas = 0;
								(*istr) >> llNas;
								CTString strString;
								strString.PrintF(_S(7065, "죄송하지만 성주님 지금은 일반 관리 모드 입니다. %I64d 나스를 저에게"), llNas);
								pUIManager->CreateMessageBoxL( _S(3908, "던전 관리"),UI_PORTAL, MSGLCMD_DRATAN_SIEGE_DUNGEON_CONTROL);
								pUIManager->AddMessageBoxLString(MSGLCMD_DRATAN_SIEGE_DUNGEON_CONTROL,TRUE, strString, -1, 0xa3a1a3ff);
								pUIManager->AddMessageBoxLString( MSGLCMD_DRATAN_SIEGE_DUNGEON_CONTROL, FALSE, _S(191, "확인" ), DRATAN_SIEGE_DUNGEON_CHANGE_LORDMODE);
								pUIManager->AddMessageBoxLString( MSGLCMD_DRATAN_SIEGE_DUNGEON_CONTROL, FALSE, _S( 139, "취소" ) );
							}
						}
						break;
					case MSG_MANAGEMENT_MANAGER_ENV_INFO:				//던전 내부 환경 정보 요청
						{
							ULONG ulCurRate;
							LONGLONG llNas;
							(*istr) >> ulCurRate;
							(*istr) >> llNas;

							if( ulCurRate >90)
							{
								MsgBoxInfo.SetMsgBoxInfo( _S(191,"확인"), UMBS_OK, UI_NONE, MSGCMD_NULL);
								MsgBoxInfo.AddString( _S(3943, "더이상 상승시킬 수 없습니다.") );
								pUIManager->CreateMessageBox( MsgBoxInfo );

								return;
							}
														
							SERVER_INFO()->SetDG_EnvNas(llNas / 10); // 10% 상승시 지불하는 금액이 오기에 나눔
							pUIManager->GetPortal()->Create_SiegeDungeon_EnvCtr_MsgBox(ulCurRate);
						}
						break;
					case MSG_MANAGEMENT_MANAGER_MONSTER_INFO:			//던전 내부 몬스터 정보 요청
						{
							ULONG ulCurRate;
							LONGLONG llNas;
							(*istr) >> ulCurRate;
							(*istr) >> llNas;

							SERVER_INFO()->SetDG_MonsterNas(llNas / 10); // 10% 상승시 지불하는 금액이 오기에 나눔

							if( ulCurRate >90)
							{
								MsgBoxInfo.SetMsgBoxInfo( _S(191,"확인"), UMBS_OK, UI_NONE, MSGCMD_NULL);
								MsgBoxInfo.AddString( _S(3943, "더이상 상승시킬 수 없습니다.") );
								pUIManager->CreateMessageBox( MsgBoxInfo );

								return;
							}

							pUIManager->GetPortal()->Create_SiegeDungeon_MonCtr_MsgBox(ulCurRate);
						}
						break;
					case MSG_MANAGEMENT_MANAGER_TAX_INFO:				//입장료 세금 정보 요청
						{
							ULONG ulCurFee;
							(*istr) >> ulCurFee;

							pUIManager->GetPortal()->Create_SiegeDungeon_AdmissionCtr_MsgBox(ulCurFee);
						}
						break;
					case MSG_MANAGEMENT_MANAGER_HUNTER_TAX_INFO:		//수렵세 정보 요청
						{
							ULONG ulCurFee;
							(*istr) >> ulCurFee;

							pUIManager->GetPortal()->Create_SiegeDungeon_HuntingCtr_MsgBox(ulCurFee);
						}
						break;
					case MSG_MANAGEMENT_MANAGER_ENV_CHANGE:				//던전 내부환경 변경
					case MSG_MANAGEMENT_MANAGER_MONSTER_CHANGE:			//던전 내부 몬스터 상태 변경
					case MSG_MANAGEMENT_MANAGER_TAX_CHANGE:				//입장료 세금 변경
					case MSG_MANAGEMENT_MANAGER_HUNTER_TAX_CHANGE:		//수렵세 변경
						{
							(*istr) >> ubError;
							if( ubError == MSG_DVD_ERROR_OK )
							{
								MsgBoxInfo.SetMsgBoxInfo( _S(191,"확인"), UMBS_OK, UI_NONE, MSGCMD_NULL);
								MsgBoxInfo.AddString( _S(3944, "던전의 상태가 변경되었습니다.") );
								pUIManager->CreateMessageBox( MsgBoxInfo );
							}
							else if( ubError == MSG_DVD_ERROR_MONEY)
							{
								MsgBoxInfo.SetMsgBoxInfo( _S(191,"확인"), UMBS_OK, UI_NONE, MSGCMD_NULL);
								MsgBoxInfo.AddString( _S(967, "나스가 부족합니다.") );
								pUIManager->CreateMessageBox( MsgBoxInfo );
							}
						}
						break;
					case MSG_MANAGEMENT_MANAGER_CONFIRM_NORMAL:
						{
							(*istr) >> ubError;

							if( ubError == MSG_DVD_ERROR_OK )
							{
								pUIManager->CreateMessageBoxL( _S(3908, "던전 관리"),UI_PORTAL, MSGLCMD_DRATAN_SIEGE_DUNGEON_CONTROL);
								pUIManager->AddMessageBoxLString(MSGLCMD_DRATAN_SIEGE_DUNGEON_CONTROL,TRUE, _S(7060, "현재 테오스 무덤은 ... 성주 대신 던전을 관리 할수 있는 권한을 드리겠습니다."),-1,0xa3a1a3ff);
								pUIManager->AddMessageBoxLString( MSGLCMD_DRATAN_SIEGE_DUNGEON_CONTROL, FALSE, _S(3921, "던전 내부 환경 제어" ), DRATAN_SIEGE_DUNGEON_CONTROL_ENVIRONMENT);
								pUIManager->AddMessageBoxLString( MSGLCMD_DRATAN_SIEGE_DUNGEON_CONTROL, FALSE, _S(3926, "던전 내부 몬스터 제어" ), DRATAN_SIEGE_DUNGEON_CONTROL_MONSTER);
								pUIManager->AddMessageBoxLString( MSGLCMD_DRATAN_SIEGE_DUNGEON_CONTROL, FALSE, _S( 1220, "취소한다." ) );

								SERVER_INFO()->SetDG_Mode(eDUNGEON_MODE_NOMAL);
							}
							else if( ubError == MSG_DVD_ERROR_CONFIRM )
							{
								pUIManager->CloseMessageBox(MSGCMD_NULL);
								MsgBoxInfo.SetMsgBoxInfo( _S(191,"확인"), UMBS_OK, UI_NONE, MSGCMD_NULL);
								MsgBoxInfo.AddString( _S(7074, "지금은 던전 관리가 잘 이루어지고 있는 상태로, 던전 관리 권한이 성주에게 있습니다.") );
								pUIManager->CreateMessageBox( MsgBoxInfo );
							}
							else if( ubError == MSG_DVD_ERROR_NORMAL_MODE )
							{
								pUIManager->CloseMessageBox(MSGCMD_NULL);
								MsgBoxInfo.SetMsgBoxInfo( _S(191,"확인"), UMBS_OK, UI_NONE, MSGCMD_NULL);
								MsgBoxInfo.AddString( _S(7071, "지금은 일반 관리 모드로 현재 던전은 성주님이 아닌 무덤을 이용하는 플레이어들이 직접 관리를 하고 있습니다.") );
								pUIManager->CreateMessageBox( MsgBoxInfo );
							}							
							else if (ubError == MSG_DVD_ERROR_ZONE)
							{
								pUIManager->CloseMessageBox(MSGCMD_NULL);
								MsgBoxInfo.SetMsgBoxInfo( _S(191,"확인"), UMBS_OK, UI_NONE, MSGCMD_NULL);
								MsgBoxInfo.AddString( _S(6275, "공성 채널에서만 이용이 가능합니다.") );
								pUIManager->CreateMessageBox( MsgBoxInfo );
							}
						}
						break;
					case MSG_NAMAGEMENT_CHANGE_OWNER_MODE:
						{
							(*istr) >> ubError;

							if( ubError == MSG_DVD_ERROR_OK )
							{
								pUIManager->CloseMessageBox(MSGCMD_NULL);
								MsgBoxInfo.SetMsgBoxInfo( _S(191,"확인"), UMBS_OK, UI_NONE, MSGCMD_NULL);
								MsgBoxInfo.AddString( _S(7069, "정상적으로 나스를 지급하셔서 지금부터 성주님이 관리 하실 수 있으십니다.") );
								pUIManager->CreateMessageBox( MsgBoxInfo );
							}
							else if( ubError == MSG_DVD_ERROR_CONFIRM )
							{
								pUIManager->CloseMessageBox(MSGCMD_NULL);
								MsgBoxInfo.SetMsgBoxInfo( _S(191,"확인"), UMBS_OK, UI_NONE, MSGCMD_NULL);
								MsgBoxInfo.AddString( _S(3942, "성주 길드의 마스터, 또는 부길드마스터만 접근할 수 있습니다.") );
								pUIManager->CreateMessageBox( MsgBoxInfo );
							}
							else if (ubError == MSG_DVD_ERROR_MONEY)
							{
								pUIManager->CloseMessageBox(MSGCMD_NULL);
								MsgBoxInfo.SetMsgBoxInfo( _S(191,"확인"), UMBS_OK, UI_NONE, MSGCMD_NULL);
								MsgBoxInfo.AddString( _S(7070, "나스가 부족 하여 관리를 하실 수 없습니다.") );
								pUIManager->CreateMessageBox( MsgBoxInfo );
							}
						}
						break;
					}
				}
				break;
			case MSG_DUNGEON_ENTER:
				{
					UBYTE ubSubSubType;
					(*istr) >> ubSubSubType;

					switch( ubSubSubType )
					{
					case MSG_DUNGEON_ENTER_FEE:
						{
							__int64 ulNeedNas;
							(*istr) >> ulNeedNas;

							if( ulNeedNas <= 0 )
							{
								_pNetwork->SendDratanSiegeDungeonMSG(MSG_DUNGEON_ENTER, MSG_DUNGEON_GO);
								return;
							}

							if( pUIManager->DoesMessageBoxExist(MSGCMD_SIEGE_DUNGEON_ENTER) )
								pUIManager->CloseMessageBox(MSGCMD_SIEGE_DUNGEON_ENTER);
							
							strMessage.PrintF( _S( 191, "확인" ) );
							MsgBoxInfo.SetMsgBoxInfo(strMessage,UMBS_YESNO,UI_PORTAL,MSGCMD_SIEGE_DUNGEON_ENTER);
							strMessage.PrintF( _S(3945, "던전으로 이동하기 위해서는 %d나스가 필요합니다. 이동 하시겠습니까?" ), ulNeedNas );
							MsgBoxInfo.AddString(strMessage);

							pUIManager->CreateMessageBox(MsgBoxInfo);
						}
						break;
					case MSG_DUNGEON_GO:
						{
							UBYTE ubError;
							(*istr) >> ubError;

							if( ubError == MSG_DVD_ERROR_MONEY )
							{
								MsgBoxInfo.SetMsgBoxInfo( _S(191,"확인"), UMBS_OK, UI_NONE, MSGCMD_NULL);
								MsgBoxInfo.AddString( _S(967, "나스가 부족합니다.") );
								pUIManager->CreateMessageBox( MsgBoxInfo );

								pUIManager->SetCSFlagOff(CSF_TELEPORT);
							}
						}
					}
				}
				break;
			case MSG_GUILD_NAME_COLOR:
				{
					ULONG ulCharIndex;
					UBYTE ubCol;

					(*istr) >> ulCharIndex;
					(*istr) >> ubCol;

					if( ulCharIndex == _pNetwork->MyCharacterInfo.index)
					{				
						_pNetwork->MyCharacterInfo.ubGuildNameColor = ubCol;		
					}
					else
					{
						ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_CHARACTER, ulCharIndex);

						if (pObject != NULL)
						{
							CCharacterTarget* pTarget = static_cast< CCharacterTarget* >(pObject);

							pTarget->cha_ubGuildNameColor = ubCol;
						}
					}
				}
				break;

			case MSG_CHANGE_NORMAL_NOTICE: // 일반모드 전환
				{
					strMessage = _S(7067, "지금부터 테오스의 무덤이 성주 관리에서 일반 관리로 전환되어 성주를 제외 한 누구나 테오스의 무덤을 관리 하실 수 있습니다. ");
					_UIAutoHelp->SetGMNotice(strMessage);
					_pNetwork->ClientSystemMessage(strMessage);
				}
				break;

			case MSG_CHANGE_NORMAL_NOTICE_TO_OWNER:
				{
					MsgBoxInfo.SetMsgBoxInfo( _S(191,"확인"), UMBS_OK, UI_NONE, MSGCMD_NULL);
					MsgBoxInfo.AddString( _S(7068, "지금부터 테오스의 무덤이 일반 관리로 전환되어 던전 관리 보상 아이템과 던전에 대한 던전 입장료, 수렵세를 지급 받지 못합니다. 성주의 권한을 다시 되찾으시리면 던전 관리인을 찾아가 현재까지의 관리 나스를 지급해야 권한을 찾을 수 있습니다.") );
					pUIManager->CreateMessageBox( MsgBoxInfo );
				}
				break;
			}
		}
		break;
		//[071123: Su-won] DRATAN_SIEGE_DUNGEON
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	case MSG_EX_PET_TURNTO_NPC: // 펫 변신
		{
			SLONG slPetIndex;
			SLONG slNpcIndex;
			SLONG slCharIndex;
			SLONG slNpcSize;

			(*istr) >> slPetIndex >> slNpcIndex >> slCharIndex >> slNpcSize;

			CEntity* penEntity = NULL;

			ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_PET, slPetIndex);
			CPetTargetInfom* pPetInfo = INFO()->GetMyPetInfo();

			if (pObject != NULL)
			{
				penEntity = pObject->GetEntity();
			}

			if (penEntity != NULL)
			{
				if (penEntity == pPetInfo->pen_pEntity) // 자신이 장착한 펫일 경우
					pPetInfo->TransformIndex = slNpcIndex;

				((CPlayerEntity*)CEntity::GetPlayerEntity(0))->TransfromPet(penEntity, slNpcIndex, slNpcSize); // 10은 사이즈 1배
			}
		}
		break;
	
	// [080422: Su-won] EVENT_CHAOSBALL
	case MSG_EX_CHAOSBALL:
		{
			(*istr) >> errcode;

			switch(errcode)
			{
			case MSG_CHAOSBALL_ERROR_SUCCESS:
				break;
			case MSG_CHAOSBALL_ERROR_ITEM_COUNT:
				break;
			case MSG_CHAOSBALL_ERROR_NOSPACE:
				pUIManager->GetChattingUI()->AddSysMessage( _S( 265,  "캐릭터 인벤토리 공간이 부족합니다." ), SYSMSG_ERROR );
				break;
			case MSG_CHAOSBALL_ERROR_ITEM_FAULT:
				break;
			}			
		}
	case MSG_EX_ATTACK_PET:
		{
			UBYTE ubSubType;
			(*istr) >> ubSubType;

			ReceiveExWildPetMessage(ubSubType, &(*istr));

		}break;

	///////////////////////////////////////////////////////////////////////////////////	
	// Cube
	case MSG_EX_EXTREME_CUBE:
		{
			UBYTE ubSubType;
			(*istr) >> ubSubType;

			switch( ubSubType )
			{
			case MSG_EX_EXTREME_CUBE_STATE_REP:
				{
					pUIManager->GetCubeRank()->CreateCubeState(istr, TRUE);
				}
				break;
			case MSG_EX_EXTREME_CUBE_STATE_PERSONAL_REP:
				{
					pUIManager->GetCubeRank()->CreateCubeState(istr, FALSE);
				}
				break;
			case MSG_EX_EXTREME_CUBE_COMPLETE:
				{
					INDEX ubCnt;
					(*istr) >> ubCnt;

					//pUIManager->GetCombo()->SetComboCount(ubCnt);
					pUIManager->GetCombo()->StageComplete(ubCnt,1);
					pUIManager->GetCombo()->SetSysImage(SYS_STAGE, TRUE, CUBE_TYPE);
				}
				break;
			case MSG_EX_EXTREME_CUBE_ERROR:
				{
					UBYTE ubErrorType;
					(*istr) >> ubErrorType;

					BOOL bShowBox = TRUE;

					switch( ubErrorType )
					{
					case MSG_EX_EXTREME_CUBE_ERROR_NOITEM:
						{
							strMessage = _S(1330, "아이템이 부족합니다.");
							pUIManager->SetCSFlagOff(CSF_TELEPORT);
						}
						break;
					case MSG_EX_EXTREME_CUBE_ERROR_OVERCOUNT:
						{
							strMessage = _S(4374, "현재 들어갈 수 있는 방이 존재하지 않습니다. 잠시 후 다시 시도하여 주십시오.");
							pUIManager->SetCSFlagOff(CSF_TELEPORT);
						}
						break;
					case MSG_EX_EXTREME_CUBE_ERROR_CANNOT_MOVE:
						{
							strMessage = _S(4375, "현재 큐브에 들어갈 수 없습니다. 잠시 후 다시 시도하여 주십시오.");
							pUIManager->SetCSFlagOff(CSF_TELEPORT);
						}
						break;
					case MSG_EX_EXTREME_CUBE_ERROR_CANNOT_CAHNGE_RATE:
						{
							strMessage = _S(4376, "입장큐브 세율을 변경할 수 없습니다.");
						}
						break;
					case MSG_EX_EXTREME_CUBE_ERROR_NOT_GUILD_CUBE_TIME:
						{
							strMessage = _S(4377, "길드 큐브 입장 시간이 아닙니다.");
							pUIManager->SetCSFlagOff(CSF_TELEPORT);
						}
						break;
					case MSG_EX_EXTREME_CUBE_ERROR_NOT_PARTY_CUBE_TIME:
						{
							strMessage = _S(4397, "파티 큐브 입장 시간이 아닙니다.");
							pUIManager->SetCSFlagOff(CSF_TELEPORT);
						}
						break;
					case MSG_EX_EXTREME_CUBE_ERROR_PARTYCUBE:
						{ // 파티 큐브 입장
							strMessage = _S(4398, "파티 큐브에 입장하였습니다.");
							_pNetwork->MyCharacterInfo.EntranceType = CURRENT_ENTER_PARTYCUBE;
							bShowBox = FALSE;
						}
						break;
					case MSG_EX_EXTREME_CUBE_ERROR_GUILDCUBE:
						{ // 길드 큐브 입장
							strMessage = _S(4399, "길드 큐브에 입장하였습니다.");
							_pNetwork->MyCharacterInfo.EntranceType = CURRENT_ENTER_GUILDCUBE;
							bShowBox = FALSE;
						}
						break;
					case MSG_EX_EXTREME_CUBE_ERROR_START_NOTICE: // 길드큐브 시작메시지
						{
							strMessage = _S(4401, "길드큐브가 시작되고 파티큐브가 종료되었습니다.");
							_UIAutoHelp->SetGMNotice(strMessage);
							bShowBox = FALSE;
						}
						break;
					case MSG_EX_EXTREME_CUBE_ERROR_END_NOTICE: // 길드큐브 종료메시지
						{
							strMessage = _S(4402, "길드큐브가 종료되고 파티큐브가 시작되었습니다.");
							_UIAutoHelp->SetGMNotice(strMessage);
							bShowBox = FALSE;
						}
						break;
					case MSG_EX_EXTREME_CUBE_ERROR_START_REMAINTIME: // 길드큐브 시작 1분전 메시지 
						{
							strMessage = _S(4403, "1분 후 길드큐브가 시작되고 1분 후 파티큐브가 종료됩니다.");
							_UIAutoHelp->SetGMNotice(strMessage);
							bShowBox = FALSE;
						}
						break;
					case MSG_EX_EXTREME_CUBE_ERROR_END_REMAINTIME: // 길드큐브 종료 1분전 메시지
						{
							strMessage = _S(4404, "1분 후 파티큐브가 시작되고 1분 후 길드큐브가 종료됩니다.");
							_UIAutoHelp->SetGMNotice(strMessage);
							bShowBox = FALSE;
						}
						break;
					case MSG_EX_EXTREME_CUBE_ERROR_INVEN_NOSPACE:		// 인벤 공간부족
						{
							strMessage = _S(265, "인벤토리 공간이 부족합니다.");
						}break;
					case MSG_EX_EXTREME_CUBE_ERROR_REWARD_PERSONAL_CANNOT:	// 개인보상 조건 안맞음
						{
							strMessage = _S(1722, "보상을 받으실 수 있는 조건이 아닙니다");
						}break;
					case MSG_EX_EXTREME_CUBE_ERROR_REWARD_PERSONAL_ALREADY:	// 개인보상 이미 받았음	
						{
							strMessage = _S(1755, "이미 보상을 받았습니다.");
						}break;
					}
					
					if (bShowBox)
					{
						MsgBoxInfo.SetMsgBoxInfo( _S(191,"확인"), UMBS_OK, UI_NONE, MSGCMD_NULL);
						MsgBoxInfo.AddString(strMessage);
						pUIManager->CreateMessageBox(MsgBoxInfo);
					}
					else
					{
						_pNetwork->ClientSystemMessage(strMessage);
					}
				}
				break;
			}
		}
		break;
	// Cube
	///////////////////////////////////////////////////////////////////////////////////	
	case MSG_EX_INIT_SSKILL:
		{
			UBYTE ubSubType;
			(*istr) >> ubSubType;

			MsgBoxInfo.SetMsgBoxInfo( _S(191,"확인"), UMBS_OK, UI_NONE, MSGCMD_NULL);

			switch(ubSubType)
			{
			case MSG_EX_INIT_SSkill_ERROR_SUCCESS:
				{
					MsgBoxInfo.AddString(_S(4247, "스페셜 스킬 초기화에 성공하였습니다."));
				}
				break;
			case MSG_EX_INIT_SSkill_ERROR_FAIL:
				{
					MsgBoxInfo.AddString(_S(4248, "스페셜 스킬 초기화에 실패하였습니다."));
				}
				break;
			}

			pUIManager->CreateMessageBox( MsgBoxInfo );
		}
		break;
	case MSG_EX_TRIGGER_EVENT:
		{
			UBYTE ubSubType;
			INDEX iTouchID, iPlayID, iContinued;
			(*istr) >> ubSubType;
			(*istr) >> iTouchID >> iPlayID >> iContinued;

			switch(ubSubType)
			{
			case MSG_EX_TRIGGER_EVENT_ERROR: // 서버 에러
				break;
			case MSG_EX_TRIGGER_EVENT_ALREADY_PLAY: // 이미 활성화 됨
				break;
			case MSG_EX_TRIGGER_EVENT_NO_CONDITION: // 조건이 성립이 안됨 (어떤 조건이 없는지는 클라가 검색해야 하나?)
				break;
			case MSG_EX_TRIGGER_EVENT_TRIGER_EXIST_AREA: // 존 이동시 이미 동작된 환경 오브젝트의 정보를 업데이트 한다.(Sub Info : 1. Obj count, 2. Objects ID
				((CPlayerEntity*)CEntity::GetPlayerEntity(0))->ReceiveTriggerEvent(iTouchID, iPlayID, iContinued, istr);
				break;
			case MSG_EX_TRIGGER_EVENT_ITEM_COMPOSITION: // 아이템 결합 완료
				pUIManager->GetChattingUI()->AddSysMessage(_S(4669, "아이템 결합 완료"));
				break;
			case MSG_EX_TRIGGER_EVENT_ITEM_DROP: // 아이템이 드랍됨( 어떤 아이템이 드랍이 되는지 클라가 알아야 알려주나?)
				pUIManager->GetChattingUI()->AddSysMessage(_S(4670, "아이템 드랍 완료"));
				break;
			case MSG_EX_TRIGGER_EVENT_OPEN_THE_DOOR: // Door object의 문을 연다.(충돌해제)
				((CPlayerEntity*)CEntity::GetPlayerEntity(0))->ReceiveTriggerEvent(iTouchID, iPlayID, iContinued, NULL);
				break;
			case MSG_EX_TRIGGER_EVENT_START_RAID: // 레이드 던전의 플레이 시작
				{				
					//pUIManager->GetChattingUI()->AddSysMessage(_s("레이드 플레이 시작"));					
				}
				break;
			case MSG_EX_TRIGGER_EVENT_MESSAGE_PRINT: // 게임 메세지를 출력(어떤 메세지를 출력할지 클라가 알아서해야 함)
				{				
					// [090709: selo] 스트링 인덱스를 받아 퀘스트북에 레이드 메시지로 추가한다
					INDEX iStringIndex = 0;
					(*istr) >> iStringIndex;
					
					if (iStringIndex > 0)
					{
						GAMEDATAMGR()->GetQuest()->AddRaidMessage(iStringIndex);
					}
				}
				break;
			case MSG_EX_TRIGGER_EVENT_SUMMON_NPC: // npc를 소환한다.(어떤 npc가 소환될지 클라가 알아서 해야 한다.)
				//pUIManager->GetChattingUI()->AddSysMessage(_s("npc소환 완료"));
				break;
			case MSG_EX_TRIGGER_EVENT_USE_DEBUF: // 캐릭터가 디버프에 걸렸다.(자신의 캐릭터만)
				//pUIManager->GetChattingUI()->AddSysMessage(_s("앗 디버프에 걸렸다~"));
				break;
			case MSG_EX_TRIGGER_EVENT_ACTIVE_OBJECT: // 오브젝트의 활성화(애니메이션이 동작한다.)
				((CPlayerEntity*)CEntity::GetPlayerEntity(0))->ReceiveTriggerEvent(iTouchID, iPlayID, iContinued, NULL);
				break;
			case MSG_EX_TRIGGER_EVENT_REMOVE_NPC: // npc 자동 제거 (npc를 삭제한다. 왜 필요할까?)
				//pUIManager->GetChattingUI()->AddSysMessage(_s("npc 제거~"));
				break;
			case MSG_EX_TRIGGER_EVENT_TIME_LIMIT:
				{
					// [090707: selo] 
					// 남은 시간을 받아서 0 이상이면 남은 시간을 설정한다.
					INDEX iRemainTime = 0;
					(*istr) >> iRemainTime;

					// 0 이면 시간이 다되서 끝난 것
					if( 0 == iRemainTime )
					{
						GAMEDATAMGR()->GetQuest()->RaidEnd();
					}
					// -1 이면 네임드 몬스터를 잡아서 끝난 것
					else if( -1 == iRemainTime )
					{
						GAMEDATAMGR()->GetQuest()->RaidEnd();
					}
					else
					{
						// [090907: selo] 레이드 남은 시간을 알린다.
						GAMEDATAMGR()->GetQuest()->SetRaidRemainTime(iRemainTime);	
				}	}
				break;
			case MSG_EX_TRIGGER_EVENT_RESTORE_OBJECT:
				{
					CEntity *tmpEntity = NULL;

					tmpEntity = _pNetwork->ga_World.EntityFromID(iPlayID);

					if (tmpEntity != NULL)
					{
						tmpEntity->Precache();
					}
				}
				break;
			}
		}
		break;
	case MSG_EX_AFFINITY:
		{
			ReceiveAffinityMessage( istr );
		}
		break;
		
	case MSG_EX_TUTORIAL:
		{
			pUIManager->GetHelp()->OpenTutorial();
		}	
		break;
		
	case MSG_EX_LOADING_END:
		{
			ReceiveExLoadingEndMessage( istr );
		}
		break;

	case MSG_EX_NPC_PORTAL_SCROLL:
		{
			ReceiveNPCPortalMessage( istr);
		}
		break;
		
	case MSG_EX_RAID_INFO:
		{
			SLONG nRaidCount = 0;
			SLONG nZoneNum = 0;
			SLONG nRoomNum = 0;

			(*istr) >> nRaidCount;

			if(pUIManager->DoesMessageBoxExist(MSGCMD_RAID_INFO))
				pUIManager->CloseMessageBox(MSGCMD_RAID_INFO);

			MsgBoxInfo.SetMsgBoxInfo( _S(4754, "귀속된 레이드 존 정보"), UMBS_OK, UI_NONE, MSGCMD_RAID_INFO);

			if(nRaidCount > 0)
			{
				for( i = 0; i < nRaidCount; i++ )
				{
					(*istr) >> nZoneNum;
					(*istr) >> nRoomNum;

					strMessage.PrintF(_S(4755, "귀속된 레이드 존 이름 : %s"),CZoneInfo::getSingleton()->GetZoneName(nZoneNum));
					MsgBoxInfo.AddString(strMessage);
					strMessage.PrintF(_S(4756, "귀속된 레이드 존 ID : %d"), nRoomNum);
					MsgBoxInfo.AddString(strMessage);
				}
			}
			else
			{
				MsgBoxInfo.AddString(_S(4757, "귀속된 존이 없습니다."));
			}

			pUIManager->CreateMessageBox( MsgBoxInfo );
		}
		break;	

	case MSG_EX_TITLE_SYSTEM:	// 호칭
		{
			ReceiveNickNameMessage( istr );
		}
		break;
		
	case MSG_EX_TAKE_AGAIN_QUEST_ITEM:
		{
			ReceiveTakeAgainQuestItem( istr);						
		}
		break;
	case MSG_EX_COSTUME2:
		{
			UCHAR ucType;
			(*istr) >> ucType;
			if ( ucType == MSG_EX_COSTUME2_SYSTEM_ERROR )
			{
				RecieveCostume2Message( istr );
			}
			else if ( ucType == MSG_EX_COSTUEM2_WEARING )
			{
				((CPlayerEntity*)CEntity::GetPlayerEntity(0))->WearingCostumeArmor( istr );
			}
			// added by sam 11/02/01 [SAM]
			else if ( ucType == MSG_EX_COSTUME2_WEARING_SUIT )
			{
				((CPlayerEntity*)CEntity::GetPlayerEntity(0))->WearingCostumeArmorOneSuit( istr );
			}
			else if ( ucType == MSG_EX_COSTUME2_PREINVENINFO )
			{
				SBYTE	wear_type;	
				INDEX	uni_index;
				INDEX	item_index;
				CEntity			*penPlEntity;
				CPlayerEntity	*penPlayerEntity;

				penPlEntity = CEntity::GetPlayerEntity(0);		// 캐릭터 자기 자신
				penPlayerEntity = static_cast<CPlayerEntity *>(penPlEntity);
				
				CModelInstance *pMI	= penPlayerEntity->GetModelInstance();

				if( _pNetwork->MyCharacterInfo.bPetRide || _pNetwork->MyCharacterInfo.bWildPetRide)
				{
					CModelInstance *pMITemp = penPlayerEntity->GetModelInstance();
					
					INDEX ctmi = pMITemp->mi_cmiChildren.Count();
					if( ctmi > 0 )
					{
						pMI = &pMITemp->mi_cmiChildren[0];
					}
					else
					{
						pMI	= penPlayerEntity->GetModelInstance();
					}
				}
				
				for (int i=0; i<WEAR_COSTUME_TOTAL; ++i)
				{
					(*istr) >> wear_type; // 0 ~ 6, 11
					(*istr) >> uni_index;
					(*istr) >> item_index;
					
					if ( item_index >= 0 )
					{	// 코스튬 입기
						_pNetwork->MyWearCostItem[i].Item_Index = item_index;
						_pNetwork->MyWearCostItem[i].Item_UniIndex = uni_index;
						
						// BUGFIX : ITS(#0002472) 코스튬 2 착용 후 강신 -> 존이동시 모델 깨짐 현상 수정. [6/9/2011 rumist]
						if( _pNetwork->MyCharacterInfo.nEvocationIndex == 0 )
						{
							if ( !( wear_type == WEAR_HELMET && (CTString)_pNetwork->GetItemData(item_index)->GetItemSmcFileName() == MODEL_TREASURE) )
							{
								((CPlayerEntity*)CEntity::GetPlayerEntity(0))->DeleteDefaultArmor(wear_type);
								_pGameState->WearingArmor(pMI, *_pNetwork->GetItemData(item_index));
							}
							if(!((static_cast<CPlayerEntity*>(CEntity::GetPlayerEntity(0)))->IsSitting() && _pNetwork->MyCharacterInfo.job == NIGHTSHADOW))
								_pNetwork->MyCharacterInfo.itemEffect.Change(_pNetwork->MyCharacterInfo.job, _pNetwork->GetItemData(item_index), wear_type,
								0, &pMI->m_tmSkaTagManager, 1, _pNetwork->GetItemData(item_index)->GetSubType());
						}
					}
				}
			}
		}
		break;
	case MSG_EX_DUNGEONTIME:
		{
			UCHAR ucType;
			SLONG slStart, slEnd, slAddExp;
			CTString strMessage;
			(*istr) >> ucType;

			if (ucType == MSG_EX_DUNGEONTIME_NOTICE)
			{
				(*istr) >> slStart >> slEnd >> slAddExp;
				strMessage.PrintF(_S(4956, "던전 타임이 시작되었습니다. (%d시 ~ %d시, 던전 경험치 %d%%추가 획득)"), slStart, slEnd, slAddExp);
				_UIAutoHelp->SetGMNotice(strMessage);
				pUIManager->GetSimplePlayerInfo()->StartDungeonTime();
				pUIManager->GetSimplePlayerInfo()->SetDungeonList(slAddExp);
			}
			else if (ucType == MSG_EX_DUNGEONTIME_START)
			{
				(*istr) >> slStart >> slEnd >> slAddExp;
			//TO.DO 던전 타임
				strMessage.PrintF(_S(4957, "던전 타임이 진행 중입니다. (%d시 ~ %d시, 던전 경험치 %d%%추가 획득)"), slStart, slEnd, slAddExp);
				_UIAutoHelp->SetGMNotice(strMessage);
				pUIManager->GetSimplePlayerInfo()->StartDungeonTime();
				pUIManager->GetSimplePlayerInfo()->SetDungeonList(slAddExp);
			}
			else if (ucType == MSG_EX_DUNGEONTIME_END)
			{
				_UIAutoHelp->SetGMNotice( _S(4958, "던전타임이 종료 되었습니다.") );
				pUIManager->GetSimplePlayerInfo()->StopDungeonTime();
				pUIManager->GetSimplePlayerInfo()->SetDungeonList(0);
			}
		}
		break;
	case MSG_EX_SOCKET:
		{
			RecieveSocketSystemMessage( istr );
		}
		break;
#ifdef XTRAP_SECURE_CKBANG_2010_07_20
    case MSG_EX_XTRAP:
        {
            if (g_pXTrap_CS_Step2Func)
            {
                CNetworkMessage nmMessageTest(MSG_EXTEND);
                #define XTRAP_CMD_STEP_TWO  0x002
                nmMessageTest << (ULONG)MSG_EX_XTRAP;
                nmMessageTest << (BYTE)XTRAP_CMD_STEP_TWO;
                char szBuf[128] , szSendBuf[128];
                
				istr->Read(szBuf, 128);
#ifdef XTRAP_LOG_ENABLE
				g_pHexaDump4XTrap_V1("TestXtrapLog.txt", szBuf, 128, "XTrap_stap1_RecBufData");
#endif
                g_pXTrap_CS_Step2Func(szBuf, szSendBuf);

                nmMessageTest.Write(szSendBuf , 128 );
#ifdef XTRAP_LOG_ENABLE
				g_pHexaDump4XTrap_V1("TestXtrapLog.txt", szSendBuf, 128, "XTrap_stap2_SendBufData");
#endif
                _pNetwork->SendToServerNew( nmMessageTest );
            }
        }
        break;
#endif
		case MSG_EX_TUTORIAL_RENEWER_2010:
		{
			pUIManager->GetHelp()->RepBeginnerTitle( istr );
		}
		break;
	case MSG_EX_RAID_SCENE: // 아칸사원 레이드
		{
			((CPlayerEntity*)CEntity::GetPlayerEntity(0))->ReceiveRaidScene(istr);
		}
		break;

	case MSG_EX_CASTLLAN:
		{
			(*istr) >> errcode;
			if ( errcode == MSG_EX_CASTLLAN_ERROR )
			{
				RecieveLordCostumeMessage( istr );
			}
		}
		break;

	case MSG_EX_SUBJOB:	// [2010/08/25 : Sora] ADD_SUBJOB 보조 직업 메시지
		{
			RecieveSubJobMessage(istr);
		}
		break;
	case MSG_EX_RANKING_SYSTEM_EX:		// 랭킹 시스템 개편 [trylord : 110825]
		{
			RecieveRankingListExMessage(istr);
		}
	// [2010/10/20 : Sora] 몬스터 용병 카드
	case MSG_EX_MONSTER_MERCENARY:
		{
			RecieveMonsterMercenaryCardMessage( istr );
		}
		break;
	case MSG_EX_FACEOFF:
		{
			(*istr) >> errcode;
			if ( errcode == MSG_EX_FACEOFF_REP )
			{
				RecieveFaceOffMessage( istr );
			}
		}
		break;
	case MSG_EX_LUCKYDRAW_BOX:
		{
			RecieveLuckyDrawBoxMessage( istr );
		}
		break;
		// 메시지 박스 출력 메시지 추가 added by sam 11/01/04
	case MSG_EX_MSGBOX:
		{	
			RecieveMsgBoxShow( istr );					
		}
		break;

	case MSG_EX_LCBALL:
		{
			pUIManager->GetLacaBall()->ReceiveLcBallMessage( istr );
		}
		break;
	// royal rumble [4/19/2011 rumist]
	case MSG_EX_ROYAL_RUMBLE:
		{
			RecieveRoyalrumbleMessage( istr );
		}
		break;
	case MSG_EX_CASH_AUTHENTICATION_CODE:
		{
			CTString strUserID;
			CTString strCode;
			UBYTE nGrupID,nAuthCode;
			(*istr) >> nAuthCode;
			

			if (nAuthCode)
			{
				(*istr) >> strUserID;
				(*istr) >> nGrupID;
				(*istr) >> strCode;

				CTString temURL;
				extern ENGINE_API INDEX sam_bFullScreenActive;				
				
				if ( IsFullScreen( sam_bFullScreenActive))
				{												 
					 pUIManager->GetOption()->ChangeWindowMode();
					 pUIManager->DestroyRenderTarget();
					 pUIManager->InitRenderTarget();
				}
				
				temURL.PrintF("http://shop.gamigo.com/game_%d/login/user/%s/hash/%s/", nGrupID, strUserID, strCode);
				ShellExecute( NULL, "open", temURL, NULL,NULL, SW_SHOWNORMAL );
			}else
			{
				MsgBoxInfo.SetMsgBoxInfo( _S( 191, "확인" ), UMBS_OK, UI_NONE, MSGCMD_NULL );
				MsgBoxInfo.AddString( _S( 3257, "요청한 작업이 실패 하였습니다." ) );
				pUIManager->CreateMessageBox( MsgBoxInfo );
			}
			
		}break;
	case MSG_EX_TREASURE_MAP_SYSTEM_KEY_EXCHANGE:
		{
			UBYTE ubErrorcode;
			(*istr) >> ubErrorcode;
			
			if (ubErrorcode == MSG_EX_TRS_MAP_KEY_EXCHANGE_ERROR_SUC) // 성공
			{
				MsgBoxInfo.SetMsgBoxInfo( _S(191, "확인"), UMBS_OK, UI_NONE, MSGCMD_NULL );
				MsgBoxInfo.AddString( _S( 159, "교환이 성공적으로 이루어졌습니다." ) );
				pUIManager->CreateMessageBox( MsgBoxInfo );

			}else if (ubErrorcode == MSG_EX_TRS_MAP_KEY_EXCHANGE_ERROR_FAIL) 
			{
				MsgBoxInfo.SetMsgBoxInfo( _S(191, "확인"), UMBS_OK, UI_NONE, MSGCMD_NULL );
				MsgBoxInfo.AddString( _S( 3257, "요청한 작업이 실패 하였습니다." ) );
				pUIManager->CreateMessageBox( MsgBoxInfo );

			}else if (ubErrorcode == MSG_EX_TRS_MAP_KEY_EXCHANGE_ERROR_ITEM) 
			{
				MsgBoxInfo.SetMsgBoxInfo( _S(191, "확인"), UMBS_OK, UI_NONE, MSGCMD_NULL );
				MsgBoxInfo.AddString( _S( 5099, "교환 가능한 아이템이 없습니다" ) );
				pUIManager->CreateMessageBox( MsgBoxInfo );

			}else if (ubErrorcode == MSG_EX_TRS_MAP_KEY_EXCHANGE_ERROR_COUNT) 
			{
				MsgBoxInfo.SetMsgBoxInfo( _S(191, "확인"), UMBS_OK, UI_NONE, MSGCMD_NULL );
				MsgBoxInfo.AddString( _S( 5100, "교환 가능한 아이템이 부족합니다" ) );
				pUIManager->CreateMessageBox( MsgBoxInfo );

			}

		}break;
	case MSG_EX_PKPENALTY_REFORM:
		{
			UBYTE ubinx;
			CTString strSysMessage;
			CUIMsgBox_Info	MsgBoxInfo;

			(*istr) >> ubinx;
			if(ubinx == MSG_EX_PKPENALTY_REFORM_REWARD_INFO)
			{
				ULONG pkSysRewardFlag;
				(*istr) >> pkSysRewardFlag;
				_pNetwork->MyCharacterInfo.pkSysRewardFlag = pkSysRewardFlag;
				_pNetwork->MyCharacterInfo.bpkSysRewardLate= FALSE;
			}
			else if(ubinx == MSG_EX_PKPENALTY_REFORM_REWARD_REP)
			{
				INDEX pkSysRewardFlag;
				UBYTE errcode;
				(*istr) >> pkSysRewardFlag;
				(*istr) >> errcode;
				switch(errcode)
				{
				case MSG_EX_PKPENALTY_REFORM_REWARD_ERROR_SUC:
					_pNetwork->MyCharacterInfo.bpkSysRewardLate= FALSE;
					break;
				case MSG_EX_PKPENALTY_REFORM_REWARD_ERROR_COUNT:
					strSysMessage = _S(2237, "지급받을 공간이 부족합니다.");
					break;
				case MSG_EX_PKPENALTY_REFORM_REWARD_ERROR_ONCE:
					//strSysMessage = _S(4827, "이미 등록되어 있는 호칭입니다.");
				    break;
				case MSG_EX_PKPENALTY_REFORM_REWARD_ERROR_FAIL:
					strSysMessage = _S(1745, "조건이 맞지 않습니다.");
				    break;
				default:
				    break;
				}
				
			}
			if (strSysMessage != CTString(""))
			{
				MsgBoxInfo.SetMsgBoxInfo(strTitle,UMBS_OK,UI_NONE,MSGCMD_NULL);
				MsgBoxInfo.AddString(strSysMessage);
				pUIManager->CreateMessageBox(MsgBoxInfo);
			}
		}break;

	case MSG_EX_SHUTDOWN:
		{
			UCHAR type;
			SLONG arg;
			CTString strMessage;
			(*istr) >> type;
			(*istr) >> arg;
			
			switch( type )
			{
				case MSG_EX_SHUTDOWN_REMAIN:
					{
						strMessage.PrintF( _s("Shut Down 시간까지 %d분 남았습니다."), arg );
						pUIManager->GetChattingUI()->AddSysMessage( strMessage, SYSMSG_USER, 0xFFFF00FF );
					}
					break;
				case MSG_EX_SHUTDOWN_COUNTDOWN:
					{
						strMessage.PrintF( _s("Shut Down 시작까지 %d초 남았습니다. 해당되시는 유저 분들은 캐릭터를 안전한 지역으로 옮겨주시기 바랍니다."), arg );
						pUIManager->GetChattingUI()->AddSysMessage( strMessage, SYSMSG_USER, 0xFFFF00FF );
					}
					break;
				case MSG_EX_SHUTDOWN_START:
					{
						strMessage.PrintF( _s("Shut Down이 시작 되었습니다.") );
						pUIManager->GetChattingUI()->AddSysMessage( strMessage, SYSMSG_USER, 0xFFFF00FF );
					}
					break;
				case MSG_EX_SHUTDOWN_END:
					{
						strMessage.PrintF( _s("Shut Down이 종료 되었습니다.") );
						pUIManager->GetChattingUI()->AddSysMessage( strMessage, SYSMSG_USER, 0xFFFF00FF );
					}
					break;
				case MSG_EX_SHUTDOWN_DISCONNECT:
					{
						CUIMsgBox_Info	MsgBoxInfo;
						
						strMessage.PrintF( _s("셧다운이 시작되었습니다. 셧다운 종료 시각 이후에 다시 접속해주세요. 확인을 눌러 클라이언트를 종료하시기 바랍니다. (셧다운 적용시간 : 24:00-06:00)") );
						MsgBoxInfo.SetMsgBoxInfo( _S(191, "확인"), UMBS_OK, UI_NONE, MSGCMD_DISCONNECT );
						MsgBoxInfo.AddString( strMessage );
						pUIManager->CreateMessageBox(MsgBoxInfo);
					}
					break;
			}

		}break;
#ifdef	IMPROV1107_NOTICESYSTEM
	case MSG_EX_USER_NOTICE:
		{
			LONG	retCode;

			(*istr) >> retCode;
			switch(retCode)
			{
			case MSG_EX_USER_NOTICE_REG_REP:		// 성공
				{
					pUIManager->GetChattingUI()->AddSysMessage(_S( 5611 ,"정상적으로 등록되었습니다."), SYSMSG_NORMAL);
				}
				break;

			case MSG_EX_USER_NOTICE_PRINT:
				{
					CTString	strName;
					CTString	strMessage;

					(*istr) >> strName;
					(*istr) >> strMessage;
					pUIManager->GetChattingUI()->AddUserNotice(strName, strMessage);
				}
				break;

			case MSG_EX_USER_NOTICE_ERROR:
				{
					LONG		errcode;
					CTString	strSysMessage;

					(*istr) >> errcode;
					if(errcode == MSG_EX_USER_NOTICE_ERROR_NOITEM)
					{
						strSysMessage	= _S( 1330 ,"아이템이 부족합니다." );
					}
					else if(errcode == MSG_EX_USER_NOTICE_ERROR_LENGTH)
					{
						strSysMessage	= _S( 5609 ,"공지할 내용을 입력하세요." );
					}

					if (strSysMessage.Length() > 0)
					{
						MsgBoxInfo.SetMsgBoxInfo( _S(191, "확인"), UMBS_OK, UI_NONE, MSGCMD_NULL );
						MsgBoxInfo.AddString(strSysMessage);
						pUIManager->CreateMessageBox( MsgBoxInfo );
					}

				}
				break;
			}
		}
		break;
#endif	// #ifdef	IMPROV1107_NOTICESYSTEM

//////////////////////////////////////////////////////////////////////////
// 이벤트 시스템 개선
//////////////////////////////////////////////////////////////////////////
	case MSG_EX_EVENT_AUTOMATION_SYSTEM:
		{
			extern ENGINE_API std::map<int,int> g_mapEvent;

			UCHAR type;
			SLONG nEventIndex;
			(*istr) >> type;

			switch( type )
			{
			case MSG_EX_EVENT_AUTOMATION_LIST:
				{
					SLONG nCount;
					(*istr) >> nCount;

					for( int i=0; i<nCount; i++ )
					{
						(*istr) >> nEventIndex;
						g_mapEvent[nEventIndex] = 1;
					}

					CheckTimerEvent();
				}
				break;
			case MSG_EX_EVENT_AUTOMATION_ADD:
				(*istr) >> nEventIndex;
				g_mapEvent[nEventIndex] = 1;

				CheckTimerEvent();
				break;
			case MSG_EX_EVENT_AUTOMATION_REMOVE:
				(*istr) >> nEventIndex;
				g_mapEvent.erase(nEventIndex);

				CheckTimerEvent();
				break;
			}
		}
		break;
	case MSG_EX_JUMPING_SYSTEM:
		{
			UCHAR type;
			CTString	strSysMessage;
			INDEX nJumpingLevel;

			(*istr) >> type;

			switch( type )
			{
			case MSG_EX_JUMPING_SUCCESS:
				{
					strSysMessage.PrintF(_S(5689, "성공적으로 적용 되었습니다."));
				}break;
			case MSG_EX_JUMPING_ERROR_LEVEL:
				{
					strSysMessage.PrintF(_S(5685, "레벨이 맞지 않아 사용하실 수 없습니다."));
				}break;
			case MSG_EX_JUMPING_ERROR_NOT_EXIST_CHAR:
				{
					(*istr) >> nJumpingLevel;
					strSysMessage.PrintF(_S(5690, "해당 계정 혹은 해당 서버 내에 %d 레벨 이상의 캐릭터가 존재하지 않습니다."), nJumpingLevel);
				}break;
			}

			MsgBoxInfo.SetMsgBoxInfo( _S(191, "확인"), UMBS_OK, UI_NONE, MSGCMD_NULL );
			MsgBoxInfo.AddString(strSysMessage);
			pUIManager->CreateMessageBox( MsgBoxInfo );

		}break;

	//REFORM_SYSTEM
	case MSG_EX_REFORMER_SYSTEM:
		{
			pUIManager->GetReformSystem()->RecvReformSystemResult(istr);
		}break;

	case MSG_EX_SERVER_TIME:
		{
			(*istr) >> nServerTime;

			_pNetwork->slServerTime = nServerTime;
			_pNetwork->slServerTimeGap = (SLONG)time(NULL) - nServerTime;
		}break;
	}
}

void CSessionState::ReceiveExWildPetMessage(UBYTE index, CNetworkMessage *istr)
{	
//	CEntity* penPlEntity;
	CPlayerEntity* penPlayerEntity;
// 	penPlEntity = CEntity::GetPlayerEntity(0); //캐릭터 자기 자신
// 	penPlayerEntity = (CPlayerEntity*) penPlEntity;
	
	penPlayerEntity = static_cast<CPlayerEntity*>( CEntity::GetPlayerEntity(0) );

	CUIManager* pUIManager = CUIManager::getSingleton();
	ObjInfo* pInfo = ObjInfo::getSingleton();

	switch(index)	
	{
	case MSG_SUB_ITEM_WEAR:
		{
			SBYTE wear_pos;
			INDEX wear_item,takeoff_item;

			(*istr) >> wear_pos;
			(*istr) >> wear_item;
			(*istr) >> takeoff_item;

			// 펫 장착 정보가 없을 경우이다.
			if (pInfo->GetMyApetInfo() == NULL)
				return;

			CWildPetTarget* pTarget = pInfo->GetMyApetInfo();
			
			if (pTarget->bIsActive == FALSE)
			{
				break;
			}

			if(pTarget->m_nPetWearIndex[wear_pos] == takeoff_item)
			{
				if (pTarget->GetEntity() != NULL)
					_pGameState->TakeOffArmorTest(pTarget->GetEntity()->GetModelInstance(), takeoff_item);
				
				pTarget->m_nPetWearIndex[wear_pos] = -1;
			}

			if (pTarget->m_nPetWearIndex[wear_pos] > 0)
			{
				if (pTarget->GetEntity() != NULL)
					_pGameState->TakeOffArmorTest(pTarget->GetEntity()->GetModelInstance(), pTarget->m_nPetWearIndex[wear_pos]);
			}			
						
			pTarget->m_nPetWearIndex[wear_pos] = wear_item;

			if (pTarget->GetEntity() != NULL)
				_pGameState->WearingArmorTest(pTarget->GetEntity()->GetModelInstance(), wear_item);
			
			pUIManager->GetWildPetInfo()->PetWearItemReSet();

		}
		break;
	case MSG_SUB_ITEM_WEARING:
		{
			INDEX OwenerIndex,petID;
			INDEX wearIndex[WILDPET_WEAR_TOTAL] = {0,};
			INDEX wearPlus[WILDPET_WEAR_TOTAL] = {0,};

			(*istr) >> OwenerIndex;
			(*istr) >> petID;

			for (int i = 0; i < WILDPET_WEAR_TOTAL; i++)
			{
				(*istr) >> wearIndex[i];
				(*istr) >> wearPlus[i];
			}

			ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_WILDPET, petID);

			if (pObject != NULL)
			{
				CWildPetTarget* pTarget = static_cast< CWildPetTarget* >(pObject);

				for (int i = 0; i < WILDPET_WEAR_TOTAL; i++)
				{
					if (pTarget->GetEntity() != NULL)
						_pGameState->TakeOffArmorTest(pTarget->GetEntity()->GetModelInstance(), pTarget->m_nPetWearIndex[i]);

					pTarget->m_nPetWearIndex[i] = wearIndex[i];
					pTarget->m_nPetWearPlus[i] = wearPlus[i];

					if (pTarget->GetEntity() != NULL)
						_pGameState->WearingArmorTest(pTarget->GetEntity()->GetModelInstance(), pTarget->m_nPetWearIndex[i]);
				}

				// 펫 정보갱신 (&_WildPetInfo != &_pNetwork->ga_srvServer.srv_actWildPet)
				if (pTarget->m_nOwnerIndex == _pNetwork->MyCharacterInfo.index)
				{
					pInfo->SetMyApet(pTarget);
					pUIManager->GetWildPetInfo()->PetWearItemReSet();
				}
			}
		}
		break;

	case MSG_SUB_SKILLLEAN:
		{
			CTString	strMessage;
			UBYTE code;
			(*istr) >> code;

			switch(code)
			{
			case 0:
				{	LONG nSkillIndex;
					UBYTE nSkillLevel;
				
					(*istr) >> nSkillIndex;
					(*istr) >> nSkillLevel;
					
					pUIManager->GetPetTraining()->LearnSkill( 0, nSkillIndex, nSkillLevel );

					CSkill		&rSkill = _pNetwork->GetSkillData( nSkillIndex );
					strMessage.PrintF( _S( 277, "%s 스킬을 습득하였습니다" ), rSkill.GetName() );

				}break;
			case 1:
				{
					//착용 펫 없음
					strMessage = _S(2203, "애완동물을 착용하지 않아 스킬을 습득할 수 없습니다." );
				}break;
			case 2:
				{
					//레벨이 아님
					strMessage = _S( 278, "레벨이 부족하여 스킬을 습득할 수 없습니다." );
				}break;
			case 3:
				{
					//스킬 부족
					strMessage = _S( 281, "스킬 조건이 맞지 않아 스킬을 습득할 수 없습니다." );
				}break;
			case 4:
				{
					//필요 아이템 없음
					strMessage = _S( 280, "아이템이 존재하지 않아 스킬을 습득할 수 없습니다." );
				}break;
			case 5:
				{
					//필요 아이템 부족
					strMessage = _S(4214, "필요 아이템의 부족으로 스킬을 습득할 수 없습니다. ");
					
				}break;

			}
			// Close message box of skill learn
		pUIManager->CloseMessageBox( MSGCMD_PETTRAINING_NOTIFY );

		// Create message box of skill learn
		CUIMsgBox_Info	MsgBoxInfo;
		MsgBoxInfo.SetMsgBoxInfo( _S( 270, "스킬" ), UMBS_OK,
									UI_PETTRAINING, MSGCMD_PETTRAINING_NOTIFY );
		MsgBoxInfo.AddString( strMessage );
		pUIManager->CreateMessageBox( MsgBoxInfo );


		}break;
	case MSG_SUB_SKILLLIST:
		{
			INDEX nSkillCont;
			INDEX nSkillIndex;
			UBYTE nSkillLevel;
			(*istr) >> nSkillCont;
			
			pUIManager->GetWildPetInfo()->SkillClear();

			for(int con = 0; con < nSkillCont; con++)
			{
				(*istr) >> nSkillIndex;
				(*istr) >> nSkillLevel;

				pUIManager->GetPetTraining()->LearnSkill(0, nSkillIndex, nSkillLevel, FALSE, false);
			}

		}break;
	case MSG_SUB_REBIRTH:
		{
			CTString	strMessage;
			CUIMsgBox_Info	MsgBoxInfo;
			UBYTE code;
			(*istr) >> code;
			switch(code)
			{
			case 0:
				{
					pUIManager->GetPetFree()->ClosePetFree(); //성공
					MsgBoxInfo.SetMsgBoxInfo( _S(191,"확인"), UMBS_OK, UI_PETFREE, UI_NONE );
					strMessage.PrintF( _S( 2450, "펫의 봉인을 해제하는데 성공하였습니다.") );
					MsgBoxInfo.AddString( strMessage );
					pUIManager->CreateMessageBox( MsgBoxInfo );
				}break;
			case 1:
				{
					//알맞지 않은 아이템
					pUIManager->GetChattingUI()->AddSysMessage( 
						_S( 2452, "봉인을 해제할 펫이 존재하지 않습니다." ), SYSMSG_ERROR );
				}break;
			case 2:
				{
					//나스가 모자람
					pUIManager->GetChattingUI()->AddSysMessage( 
						_S( 2451, "펫의 봉인을 해제하기 위한 비용이 부족합니다." ), SYSMSG_ERROR );
				}break;
			case 3:
				{
					//봉인되지 않은 아이템
					pUIManager->GetChattingUI()->AddSysMessage( 
						_S( 2452, "봉인을 해제할 펫이 존재하지 않습니다." ), SYSMSG_ERROR );
				}break;
			}
		}break;
	case MSG_SUB_LEVELUP:
		{
			pUIManager->GetChattingUI()->AddSysMessage( 
						_S( 2253, "펫이 레벨업 하였습니다." ), SYSMSG_NORMAL );
			
			if (pInfo->GetMyApetInfo() != NULL)
				WildPetStartEffectGroup("pet_levelup", pInfo->GetMyApetInfo()->m_nIdxServer, pInfo->GetMyApetInfo()->m_pEntity);

		}break;
	case MSG_SUB_DELETE_EQUIP:
		{
			CTString strPetname,strmassage;
			INDEX nItem_Index;
			SBYTE wear_pos;

			(*istr) >> strPetname;
			(*istr) >> nItem_Index;
			(*istr) >> wear_pos;
	
			strmassage.PrintF(_S(4213, "%s 의 %s 아이템이 기간 만료로 사라집니다." ),strPetname,_pNetwork->GetItemName(nItem_Index));
			pUIManager->GetChattingUI()->AddSysMessage( strmassage, SYSMSG_NORMAL );

			if (pInfo->GetMyApetInfo() != NULL)
			{
				if (pInfo->GetMyApetInfo()->bIsActive == FALSE)
				{
					break;
				}
				if (pInfo->GetMyApetInfo()->GetEntity() != NULL)
					_pGameState->TakeOffArmorTest(pInfo->GetMyApetInfo()->GetEntity()->GetModelInstance(),nItem_Index);
				pInfo->GetMyApetInfo()->m_nPetWearIndex[wear_pos] = -1;
			
				// _WildPetInfo는 실제 와일드 펫정보가 아니다.(_pNetwork->ga_srvServer.srv_actWildPet의 펫정보를 직접 바꾸어야 한다.)
				// _WildPetInfo변수가 포인터 변수가 아니다. &_WildPetInfo != &_pNetwork->ga_srvServer.srv_actWildPet
				if (pInfo->GetMyApetInfo()->GetEntity() != NULL)
					pInfo->GetMyApetInfo()->GetEntity()->en_pWildPetTarget->m_nPetWearIndex[wear_pos] = -1;
			}
			//////////////////////////////////////////////////////////////////////////
			pUIManager->GetWildPetInfo()->PetWearItemReSet();

			

		}break;

	case MSG_SUB_ADDITEM_MSG: // 이것 또한 실제 펫 정보의 갱신을 통해서 UI Update가 되어야 한다.
		{
			pUIManager->GetWildPetInfo()->ReceiveWearItemData(istr);	
		}break;

	case MSG_SUB_AI_LIST_NTF:
		{
			INDEX nPetAICount;
			INDEX nAIMaxCount;
			INDEX nSlotnum;
			UBYTE bActive;
			sWildPet_AI_Slot temAI;

			pUIManager->GetWildPetInfo()->AIClear();

			(*istr) >> nAIMaxCount;

			pUIManager->GetWildPetInfo()->SetPetAISlotCount(nAIMaxCount);

			(*istr) >> nPetAICount;

			for (int ncnt = 0; ncnt < nPetAICount; ncnt++)
			{
				(*istr) >> nSlotnum;
				(*istr)	>> temAI.m_nTargetSlot;
				(*istr)	>> temAI.m_nActionSlot;
				(*istr)	>> temAI.m_nSillType;
				(*istr)	>> bActive;

				temAI.m_bActive = bActive;

				pUIManager->GetWildPetInfo()->AddAIData(nSlotnum, temAI);

			}

			pUIManager->GetWildPetInfo()->PetAIReSet();
			pUIManager->GetWildPetInfo()->AIPopupClose();


		}break;
	case MSG_SUB_AI_UPDATE_REP:
		{
			UBYTE ubErrorcode;
			(*istr) >> ubErrorcode;

			pUIManager->GetWildPetInfo()->ErrorMassage(ubErrorcode);

		}break;
	case MSG_SUB_AI_ONOFF_REP:
		{
			UBYTE bActive;
			UBYTE ubErrorCode;

			(*istr) >> bActive;
			(*istr) >> ubErrorCode;

			pUIManager->GetWildPetInfo()->SetPetAIActive(bActive);
			pUIManager->GetWildPetInfo()->ErrorMassage(ubErrorCode);			

		}break;
	case MSG_SUB_EVOLUTION: // 단지 에러 및 성공 메세지 처리
		{
			UBYTE ubErrorCode;

			(*istr) >> ubErrorCode;

			pUIManager->GetPetTraining()->EvolutionError(ubErrorCode);
		}break;
	case MSG_SUB_MOUNT_REP:
		{
			UBYTE ubErrorCode;	// 0: 성공, 1: 실패

			(*istr) >> ubErrorCode;

			pUIManager->SetCSFlagOff(CSF_PETRIDING);
		}
		break;
	case MSG_SUB_SUMMON_ERROR:
		{
			UBYTE ubErrorCode;
			CTString strMessage;

			(*istr) >> ubErrorCode;

			strMessage.PrintF( _S(5212, "소환가능시간 %d초 남았습니다." ), ubErrorCode );						
			pUIManager->GetChattingUI()->AddSysMessage( strMessage, SYSMSG_ERROR );
		}break;
		// cash item 추가 : p2용. [1/19/2011 rumist]
	case MSG_SUB_STATINIT:
		{
			UBYTE ubErrorCode;
			CTString strMessage;
			CUIMsgBox_Info MsgBoxInfo;
			(*istr) >> ubErrorCode;
			
			MsgBoxInfo.SetMsgBoxInfo( _S( 191, "확인" ), UMBS_OK, UI_NONE, MSGCMD_NULL );
			switch( ubErrorCode )
			{
				case 0:
					MsgBoxInfo.AddString( _S( 5322, "능력치 초기화가 완료 되었습니다." ) );
				break;
				case 1:
					MsgBoxInfo.AddString( _S( 2189,"애완동물을 슬롯에 장착하여야만 합니다.") );
				break;
				case 2:
					MsgBoxInfo.AddString( _S( 2444, "펫 봉인 해제." ) );
				break;
				case 3:
					MsgBoxInfo.AddString( _S( 5320, "초기화 될 포인트가 존재하지 않습니다." ) );
				break;
			}
			
			pUIManager->CreateMessageBox( MsgBoxInfo );
		}break;
	case MSG_SUB_EXPUSE:
		{
			UBYTE ubErrorCode;
			CTString strMessage;
			CUIMsgBox_Info MsgBoxInfo;
			(*istr) >> ubErrorCode;

			MsgBoxInfo.SetMsgBoxInfo( _S( 191, "확인" ), UMBS_OK, UI_NONE, MSGCMD_NULL );
			switch( ubErrorCode )
			{
			case 0:
				MsgBoxInfo.AddString( _S( 5651, "축적된 경험치가 성공적으로 적용되었습니다." ) );
				break;
			case 1:
				MsgBoxInfo.AddString( _S( 5673,"축적 된 경험치를 사용 할 수 없는 상태 입니다.") );
				break;
			case 2:
				MsgBoxInfo.AddString( _S( 4981, "착용중인 아이템이거나, 올바른 아이템이 아닙니다. 확인 후 다시 시도해 주시기 바랍니다." ) );
				break;
			case 3:
				MsgBoxInfo.AddString( _S( 2445, "펫 아이템만 가능합니다." ) );
				break;
			}
			pUIManager->CreateMessageBox( MsgBoxInfo );

		}break;
	case MSG_SUB_ERROR_NOT_EXIST_NPC:
		{
			CTString strMessage;
			strMessage.PrintF( _S( 5913, "수행가능한 NPC가 주위에 존재하지 않습니다.") );
			_pNetwork->ClientSystemMessage(strMessage, SYSMSG_ERROR);
		}
		break;
	case MSG_SUB_INIT_COOLTIME:
		{
			UBYTE ubErrorCode;
			CTString strMessage;
			CUIMsgBox_Info MsgBoxInfo;
			(*istr) >> ubErrorCode;

			MsgBoxInfo.SetMsgBoxInfo(_S(191, "확인"), UMBS_OK, UI_NONE, MSGCMD_NULL);

			switch(ubErrorCode)
			{
			case 0:
				MsgBoxInfo.AddString(_S(6288, "장착 중인 경험치 펫의 쿨타임이 삭제되어 경험치 축적이 시작됩니다."));
				break;
			case 1:
			case 2:
				MsgBoxInfo.AddString(_S(6289, "쿨타임을 제거하고자 하는 경험치 펫을 장착하여 주시기 바랍니다."));
				break;
			default:
				return;
			}

			pUIManager->CreateMessageBox(MsgBoxInfo);
		}
		break;
	}
}

// ----------------------------------------------------------------------------
// Name : ReceivePetStatusMessage()
// Desc : 애완동물 상태.
// ----------------------------------------------------------------------------
void CSessionState::ReceivePetStatusMessage( CNetworkMessage *istr )
{
	// 애완동물 상태	: index(n) petTypeGrade(c) level(n) exp(ll) needexp(ll) hp(n) maxhp(n) hungry(n) maxhugry(n) sympathy(n) maxsympathy(n) x(f) z(f) h(f) r(f) y(c) mapatt(uc)
	LONG		lIndex;
	SBYTE		sbPetTypeGrade;
	LONG		lLevel;
	__int64		llExp;
	__int64		llNeedExp;
	LONG		lHP;
	LONG		lMaxHP;
	LONG		lAbility;
	LONG		lHungry;
	LONG		lMaxHungry;
	LONG		lSympathy;
	LONG		lMaxSympathy;
	FLOAT		fX;
	FLOAT		fZ;
	FLOAT		fH;
	FLOAT		fR;	
	SBYTE		sbYLayer;
	UWORD		sbMapAttribute;
	LONG		remainRebirth;
	CTString	strNameCard;

	(*istr) >> lIndex;
	(*istr) >> sbPetTypeGrade;
	(*istr) >> lLevel;
	(*istr) >> llExp;
	(*istr) >> llNeedExp;
	(*istr) >> lHP;
	(*istr) >> lMaxHP;
	(*istr) >> lAbility;
	(*istr) >> lHungry;
	(*istr) >> lMaxHungry;
	(*istr) >> lSympathy;
	(*istr) >> lMaxSympathy;
	(*istr) >> fX;
	(*istr) >> fZ;
	(*istr) >> fH;
	(*istr) >> fR;
	(*istr) >> sbYLayer;
	(*istr) >> sbMapAttribute;
#ifdef PET_SEAL_TIME
	(*istr) >> remainRebirth;
#endif

	(*istr) >> strNameCard;

	CNetworkLibrary::sPetInfo	TempPet;
	TempPet.lIndex				= lIndex;
	TempPet.sbPetTypeGrade		= sbPetTypeGrade;
	TempPet.lLevel				= lLevel;
	TempPet.llExp				= llExp;
	TempPet.llNeedExp			= llNeedExp;
	TempPet.lHP					= lHP;
	TempPet.lMaxHP				= lMaxHP;
	TempPet.lAbility			= lAbility;
	TempPet.lHungry				= lHungry;
	TempPet.lMaxHungry			= lMaxHungry;
	TempPet.lSympathy			= lSympathy;
	TempPet.lMaxSympathy		= lMaxSympathy;
	TempPet.fX					= fX;
	TempPet.fZ					= fZ;
	TempPet.fH					= fH;
	TempPet.fR					= fR;	
	TempPet.sbMapAttribute		= sbMapAttribute;
#ifdef PET_SEAL_TIME
	TempPet.lRemainRebirth		= remainRebirth;
#endif
	TempPet.strNameCard			= strNameCard;

	std::vector<CNetworkLibrary::sPetInfo>::iterator iter = 
		std::find_if(_pNetwork->m_vectorPetList.begin(), _pNetwork->m_vectorPetList.end(), CNetworkLibrary::FindPet(TempPet) );
	
	// 펫 정보를 못 찾으면 추가함.
	if( iter == _pNetwork->m_vectorPetList.end() )
	{
		_pNetwork->m_vectorPetList.push_back(TempPet);
	}	
	// 펫 정보를 갱신함.
	else
	{		
		(*iter).sbPetTypeGrade	= TempPet.sbPetTypeGrade;
		(*iter).lLevel			= TempPet.lLevel;
		(*iter).llExp			= TempPet.llExp;
		(*iter).llNeedExp		= TempPet.llNeedExp;
		(*iter).lHP				= TempPet.lHP;
		(*iter).lMaxHP			= TempPet.lMaxHP;
		(*iter).lHungry			= TempPet.lHungry;
		(*iter).lMaxHungry		= TempPet.lMaxHungry;
		(*iter).lSympathy		= TempPet.lSympathy;
		(*iter).lMaxSympathy	= TempPet.lMaxSympathy;
		(*iter).fX				= TempPet.fX;
		(*iter).fZ				= TempPet.fZ;
		(*iter).fH				= TempPet.fH;
		(*iter).fR				= TempPet.fR;		
		(*iter).sbMapAttribute	= TempPet.sbMapAttribute;
		(*iter).lAbility		= TempPet.lAbility;
#ifdef PET_SEAL_TIME
		(*iter).lRemainRebirth	= TempPet.lRemainRebirth;
#endif
		(*iter).strNameCard		= TempPet.strNameCard;
	}
	_pNetwork->UpdatePetTargetInfo( TempPet.lIndex );
	CUIManager::getSingleton()->GetPetInfo()->GetPetDesc();	
}

// ----------------------------------------------------------------------------
// Name : ReceivePetMountMessage()
// Desc : 애완동물 타기.
// ----------------------------------------------------------------------------
// FIXME : ReceivePetAppearMessage, ReceiveStatusMessage, ReceivePetMountMessage 중복되는 코드 많음 정리 필요.
void CSessionState::ReceivePetMountMessage( CNetworkMessage *istr )
{
	// 애완동물에 타기	: owner_index(n) mountPetTypeGrade(c)
	LONG	lOwnerIndex;
	SBYTE	sbMountPetTypeGrade;

	(*istr) >> lOwnerIndex;
	(*istr) >> sbMountPetTypeGrade;

	// [070824: Su-won] PET_COLOR_CHANGE
	SBYTE	sbPetColor =0;
	(*istr) >> sbPetColor;

	INDEX iPetType;
	INDEX iPetAge;
	_pNetwork->CheckPetType( sbMountPetTypeGrade, iPetType, iPetAge );
	
	const BOOL bPetRide = PetInfo().IsRide(iPetType, iPetAge);
	
	// [070824: Su-won] PET_COLOR_CHANGE
	INDEX iPetColoredType = iPetType | (sbPetColor<<8);
	
	// 내 펫인경우...
	if( lOwnerIndex == _pNetwork->MyCharacterInfo.index )
	{			
		// 펫을 타도록 설정해야 하는 경우...
		if( iPetType != -1 && 
			iPetAge != -1 && 
			bPetRide )
		{
			
			if( _pNetwork->MyWearItem[WEAR_PET].IsEmptyItem() == FALSE )
			{
				const INDEX iPetIndex = _pNetwork->MyWearItem[WEAR_PET].Item_Plus;
				CNetworkLibrary::sPetInfo	TempPet;
				TempPet.lIndex				= iPetIndex;

				ObjInfo* pInfo = ObjInfo::getSingleton();
				CPetTargetInfom* pPetInfo = pInfo->GetMyPetInfo();
				
				std::vector<CNetworkLibrary::sPetInfo>::iterator iter = 
					std::find_if(_pNetwork->m_vectorPetList.begin(), _pNetwork->m_vectorPetList.end(), CNetworkLibrary::FindPet(TempPet) );
				if( iter != _pNetwork->m_vectorPetList.end() )
				{
					pPetInfo->iLevel		= (*iter).lLevel;
					pPetInfo->fHealth		= (*iter).lHP;
					pPetInfo->fMaxHealth	= (*iter).lMaxHP;
					pPetInfo->fMaxHungry	= (*iter).lMaxHungry;
					pPetInfo->fHungry		= (*iter).lHungry;
					pPetInfo->lAbility		= (*iter).lAbility;
					pPetInfo->bIsActive		= TRUE;				
					pPetInfo->iType			= iPetType;
					pPetInfo->iAge			= iPetAge;
					pPetInfo->lIndex		= iPetIndex;
					pPetInfo->strNameCard	= (*iter).strNameCard;
					
					// 펫 타겟 제거. [11/18/2010 rumist]
					pPetInfo->pen_pEntity	= NULL;
					
					CUIManager::getSingleton()->GetPetInfo()->GetPetDesc();
				}
			}

			// 기존의 정보랑 비교를 해서, 펫을 제거해야 하는지? 아니면 생성해야 하는지 판단...
			_pNetwork->RidePet( (CPlayerEntity*)CEntity::GetPlayerEntity(0), NULL, iPetColoredType );
		}
		// 펫에서 내리도록 설정해야 하는 경우...
		else
		{
			_pNetwork->LeavePet( (CPlayerEntity*)CEntity::GetPlayerEntity(0) );
		}
		//		_pNetwork->MyCharacterInfo.bPetRide = bPetRide;		
	}
	// 타 캐릭터의 펫인 경우...
	else
	{
		ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_CHARACTER, lOwnerIndex);

		if (pObject != NULL)
		{
			CCharacterTarget* pTarget = static_cast< CCharacterTarget* >(pObject);

			pTarget->cha_iPetType = iPetType;
			pTarget->cha_iPetAge	= iPetAge;
			if( pTarget->cha_bPetRide != bPetRide )
			{
				// 펫을 타도록 설정해야 하는 경우...
				if( iPetType != -1 && 
					iPetAge != -1 && 
					bPetRide )
				{	
					_pNetwork->RidePet( pTarget->GetEntity(), NULL, iPetColoredType );
				}
				// 펫에서 내리도록 설정해야 하는 경우...
				else
				{
					_pNetwork->LeavePet( pTarget->GetEntity() );
				}
				pTarget->cha_bPetRide = bPetRide;
			}
		}
	}	
}

// ----------------------------------------------------------------------------
// Name : ReceivePetLearnMessage()
// Desc : 
// ----------------------------------------------------------------------------
void CSessionState::ReceivePetLearnMessage( CNetworkMessage *istr )
{
	LONG nSkillIndex;
	BYTE nSkillLevel;
	LONG nErrorcode;

	(*istr) >> nSkillIndex;
	(*istr) >> nSkillLevel;
	(*istr) >> nErrorcode;

	CUIManager* pUIManager = CUIManager::getSingleton();
	ObjInfo* pInfo = ObjInfo::getSingleton();
	CPetTargetInfom* pPetInfo = pInfo->GetMyPetInfo();

	if( nErrorcode == MSG_EX_PET_LEARN_ERROR_OK || nErrorcode == MSG_EX_PET_LEARN_ERROR_AUTO  )
	{	
		pUIManager->GetPetTraining()->LearnSkill( pPetInfo->lIndex, nSkillIndex, nSkillLevel );
	}
	else if( nErrorcode == MSG_EX_PET_LEARN_ERROR_AUTO ) // 다종 스킬 습득 
	{
		pUIManager->GetPetTraining()->LearnSkill( pPetInfo->lIndex, nSkillIndex, nSkillLevel, TRUE );
	}
	else 
	{
		pUIManager->GetPetTraining()->LearnSkillError( nErrorcode );
	}

}

// ----------------------------------------------------------------------------
// Name : ReceivePetSkillListMessage()
// Desc : 
// ----------------------------------------------------------------------------
void CSessionState::ReceivePetSkillListMessage( CNetworkMessage *istr )
{
	LONG nPetIndex, nCount;
	LONG nSkillIndex;
	BYTE nSkillLevel;

	(*istr) >> nPetIndex;
	(*istr) >> nCount;

	CUIManager* pUIManager = CUIManager::getSingleton();

	pUIManager->GetPetInfo()->ClearSkills( nPetIndex );
	
	for( int i = 0; i < nCount; i++ )
	{
		(*istr) >> nSkillIndex;
		(*istr) >> nSkillLevel;
	
		if (MY_PET_INFO()->lIndex < 0)
		{
			MY_INFO()->SetPetSkill(nPetIndex, nSkillIndex, nSkillLevel);
			pUIManager->GetPetInfo()->AddSkill(nPetIndex, nSkillIndex, nSkillLevel);
		}
		else
		{
			pUIManager->GetPetTraining()->LearnSkill(nPetIndex, nSkillIndex, nSkillLevel, FALSE, false);
		}
	}
}

// ----------------------------------------------------------------------------
// Name : ReceivePetResetSkillMessage()
// Desc : 현재 착용하고 있는 펫의 스킬 초기화
// ----------------------------------------------------------------------------
void CSessionState::ReceivePetResetSkillMessage( CNetworkMessage *istr )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	pUIManager->GetPetInfo()->ClearSkills( MY_PET_INFO()->lIndex );
	
	CTString	strMessage;
	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( _S(191,"확인" ), UMBS_OK, UI_NONE, MSGCMD_NULL );
	strMessage.PrintF( _S(2565,"애완동물 스킬 초기화가 완료 되었습니다." ) );
	MsgBoxInfo.AddString( strMessage );
	pUIManager->CreateMessageBox( MsgBoxInfo );
}

// ----------------------------------------------------------------------------
// Name : ReceivePetSellInfoMessage()
// Desc : 
// ----------------------------------------------------------------------------
void CSessionState::ReceivePetSellInfoMessage( CNetworkMessage *istr )
{
	SPetExchangInfo temp;

	(*istr) >> temp.lOwnerIndex;
	(*istr) >> temp.lPetIndex;
	(*istr) >> temp.sbPetTypeGrade;
	(*istr) >> temp.lLevel;
	(*istr) >> temp.llExp;
	(*istr) >> temp.llNeedExp;
	(*istr) >> temp.lHP;
	(*istr) >> temp.lMaxHP;
	(*istr) >> temp.lAbility;
	(*istr) >> temp.lHungry;
	(*istr) >> temp.lMaxHungry;
	(*istr) >> temp.lSympathy;
	(*istr) >> temp.lMaxSympathy;
#ifdef PET_SEAL_TIME
	(*istr) >> temp.lRemainRebirth;	
#endif

	(*istr) >> temp.strNameCard;

	CUIManager::getSingleton()->GetPetInfo()->AddPetExchangeInfo( temp );
	
}

// ----------------------------------------------------------------------------
// Name : ReceivePetChangeMountMessage()
// Desc : 
// ----------------------------------------------------------------------------
void CSessionState::ReceivePetChangeMountMessage( CNetworkMessage *istr )
{
	LONG	lResult;
	(*istr) >> lResult;
	CUIManager::getSingleton()->GetPetInfo()->PetExchangeMount( lResult );
}
//------------------------------------------------------------------------------
// CSessionState::ReceivePetLevelUpMessage
// Explain:  
// Date : 2005-10-07,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CSessionState::ReceivePetLevelUpMessage( CNetworkMessage *istr )
{
	LONG	lPetIndex;
	SBYTE	sbTypeGrade;
	LONG	lLevel;

	(*istr) >> lPetIndex;
	(*istr) >> sbTypeGrade;
	(*istr) >> lLevel;

	// [070824: Su-won] PET_COLOR_CHANGE
	SBYTE	sbPetColor =0;
	(*istr) >> sbPetColor;

	INDEX iPetType;
	INDEX iPetAge;
	_pNetwork->CheckPetType( sbTypeGrade, iPetType, iPetAge );

	ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_PET, lPetIndex);

	if (pObject != NULL)
	{
		CPetTarget* pTarget = static_cast< CPetTarget* >(pObject);

		if (pTarget->m_nIdxServer == MY_PET_INFO()->lIndex )
		{
			_pNetwork->ClientSystemMessage( _S(2253, "펫이 레벨업 하였습니다." ) );
		}

		// 펫의 나이가 변했을때...
		if (pTarget->pet_iAge != iPetAge )
		{
			CEntity	*penEntity = pTarget->GetEntity();

			if (penEntity != NULL)
			{
				pTarget->m_nType	= iPetType;
				pTarget->pet_iAge	= iPetAge;

				// Date : 2005-11-08(오후 4:04:21), By Lee Ki-hwan
				// _s 로 처리가 안되나요?ㅋㅋ 
				CTString strPetName = pTarget->pet_OwnerName + _S( 2228, "의" ) + CTString(" ") + PetInfo().GetName(iPetType, iPetAge);	
				pTarget->m_strName	= strPetName;
				penEntity->SetSkaModel(PetInfo().GetFileName(iPetType, iPetAge));					
				//((CPlayerEntity*)CEntity::GetPlayerEntity(0))->PetIdleAnim( penEntity );
				PetInfo().SetPetDataToEntity(iPetType, iPetAge, penEntity, FALSE);
				penEntity->GetModelInstance()->RefreshTagManager();
				//penEntity->GetModelInstance()->StopAllAnimations(0.1f);

				if( sbPetColor > 0 )
				{
					CTString strTexFile =PetInfo().GetColoredTexFileName(iPetType, iPetAge, sbPetColor);
					penEntity->GetModelInstance()->mi_aMeshInst[0].mi_tiTextures[0].ti_toTexture.SetData_t( strTexFile );
				}
			}
		}
	}

	CTString strEffect;
	strEffect = "pet_levelup";
	
	if( iPetAge >= 2 )
		strEffect = "pet_levelup_mount";
	
	// 펫 모양 변경 ...?? 
	PetStartEffectGroup( strEffect, lPetIndex );
}

// ----------------------------------------------------------------------------
// Name : ReceviePetItemMixMessage()
// Desc : 유니크 아이템 조합 eons
// ----------------------------------------------------------------------------
void CSessionState::ReceviePetItemMixMessage( CNetworkMessage *istr )
{
	LONG	lResult;
	(*istr) >> lResult;
	CUIManager::getSingleton()->GetPetItemMix()->PetItemMixRep( lResult );	
}

// ----------------------------------------------------------------------------
// Name : ReceviePetItemChangeMessage()
// Desc : 펫 유니크 재료 아이템으로 교환 eons
// ----------------------------------------------------------------------------
void CSessionState::ReceviePetItemChangeMessage( CNetworkMessage *istr )
{
	LONG	lResult;
	(*istr) >> lResult;
	CUIManager::getSingleton()->GetPetTraining()->PetChangeItemError( lResult );
}

// ----------------------------------------------------------------------------
// Name : ReceviePetRebirthMessage()
// Desc : eons( 봉인 해제 )
// ----------------------------------------------------------------------------
void CSessionState::ReceviePetRebirthMessage( CNetworkMessage *istr )
{
	LONG	lPetIndex;
	LONG	lResult;
	(*istr) >> lPetIndex;
	(*istr) >> lResult;

	CUIManager::getSingleton()->GetPetFree()->PetFreeError( lPetIndex, lResult );
}


// ----------------------------------------------------------------------------
// Name : ReceivePartyRecall()
// Desc : 
// ----------------------------------------------------------------------------
void CSessionState::ReceivePartyRecall( CNetworkMessage *istr )
{
	CUIMsgBox_Info	MsgBoxInfo;
	LONG subType;
	(*istr) >> subType;

	CUIManager* pUIManager = CUIManager::getSingleton();

	switch(subType)
	{

		case MSG_EX_PARTY_RECALL_NOTICE :
			pUIManager->CloseMessageBox(MSGCMD_NULL);
			MsgBoxInfo.SetMsgBoxInfo( _S(191, "확인" ), UMBS_OK, UI_NONE, MSGCMD_NULL );
			MsgBoxInfo.AddString( _S(2516, "파티원에게 소환 메시지를 보냈습니다." ) );
			pUIManager->CreateMessageBox( MsgBoxInfo );
			break;
		case  MSG_EX_PARTY_RECALL_PROMPT :
			{
				CTString tv_str;
				(*istr) >> _pNetwork->m_tmp_idx;
				(*istr) >> _pNetwork->m_tmp_str;

				if( pUIManager->DoesMessageBoxExist(MSGCMD_EX_PARTY_RECALL_PROMPT) )
					pUIManager->CloseMessageBox(MSGCMD_EX_PARTY_RECALL_PROMPT); // 기존의 메시지 박스는 지우고 새로운 메시지로 갱신
				
				MsgBoxInfo.SetMsgBoxInfo( _S(191, "확인" ), UMBS_YESNO, UI_NONE, MSGCMD_EX_PARTY_RECALL_PROMPT );
				tv_str.PrintF(_S(2517,"%s파티원님께서 소환메시지를 보내 왔습니다. 소환에 응하시겠습니까?"),_pNetwork->m_tmp_str.str_String);
				MsgBoxInfo.AddString(tv_str);
				pUIManager->CreateMessageBox( MsgBoxInfo );
			}
			break;
	
		case MSG_EX_PARTY_RECALL_CANCEL :
			{
			   LONG nReq_char,nTgt_char;
			   CTString strReq_char,strTgt_char,tv_str;
			   (*istr) >> nReq_char;
			   (*istr) >> strReq_char;
			   (*istr) >> nTgt_char;
			   (*istr) >> strTgt_char;
			   if(nReq_char == _pNetwork->MyCharacterInfo.index)
			   {
			   		pUIManager->CloseMessageBox(MSGCMD_NULL);
					MsgBoxInfo.SetMsgBoxInfo( _S(191, "확인" ), UMBS_OK, UI_NONE, MSGCMD_NULL );
					tv_str.PrintF(_S(2518, "%s파티원님이 소환을 거절하였습니다."), strTgt_char);
					MsgBoxInfo.AddString(tv_str);
					pUIManager->CreateMessageBox( MsgBoxInfo );
			   }
			   else {
				   pUIManager->CloseMessageBox(MSGCMD_NULL);
					MsgBoxInfo.SetMsgBoxInfo( _S(191, "확인" ), UMBS_OK, UI_NONE, MSGCMD_NULL );
					tv_str.PrintF(_S(2519, "%s파티원님의 소환을 거절하였습니다."), strReq_char);
					MsgBoxInfo.AddString( tv_str );
					pUIManager->CreateMessageBox( MsgBoxInfo );				   
			   }
			}
			break;
		case MSG_EX_PARTY_RECALL_ERROR_NOT_FOUND_PARTY_PLAYER :
			{
				_pNetwork->ClientSystemMessage(_S(4802, "같은 대륙에 있는 파티 멤버가 없습니다."), SYSMSG_ERROR);
			}
			break;
		case MSG_EX_PARTY_RECALL_ERROR_CANT_USE_INSTANT_DUNGEON :
			{
				_pNetwork->ClientSystemMessage(_S(4504, "인스턴트존 내부에서는 사용할 수 없습니다."), SYSMSG_ERROR);
			}
			break;
		case MSG_EX_PARTY_RECALL_ERROR_CANT_USE_THIS_CONTINENT :
			{
				_pNetwork->ClientSystemMessage(_S(4803, "현재 대륙에선 사용할 수 없습니다."), SYSMSG_ERROR);
			}
			break;
		case MSG_EX_PARTY_RECALL_ERROR_NOT_PARTY:
			{
				_pNetwork->ClientSystemMessage(_S(3408, "소환할 파티원들이 없습니다."), SYSMSG_ERROR);
			}
			break;
		case MSG_EX_PARTY_RECALL_ERROR_CANT_USE_EXPEND:
			{
				_pNetwork->ClientSystemMessage(_S(4752, "원정대중에는 사용할 수 없습니다."), SYSMSG_ERROR);
			}
			break;
		case MSG_EX_PARTY_RECALL_CONFIRM_FAIL:
			{// 리콜 수락 시 이동할 수 없는 경우 클라 락을 풀기 위한 메시지
				pUIManager->CloseMessageBox(MSGCMD_NULL);
				MsgBoxInfo.SetMsgBoxInfo( _S(191, "확인" ), UMBS_OK, UI_NONE, MSGCMD_NULL );
				MsgBoxInfo.AddString( _S(340, "이동이 취소되었습니다." ) );
				pUIManager->CreateMessageBox( MsgBoxInfo );

				if(pUIManager->IsCSFlagOn(CSF_TELEPORT))
				{
					pUIManager->SetCSFlagOff( CSF_TELEPORT );
				}			
			}
			break;
		case MSG_EX_PARTY_RECALL_AREADY:
			{
				_pNetwork->ClientSystemMessage(_S(556, "이미 사용중인 아이템 입니다."), SYSMSG_ERROR);
			}
			break;

	}
}

// ----------------------------------------------------------------------------
// Name : ReceiveElementalDeleteMessage()
// Desc : 
// ----------------------------------------------------------------------------
void CSessionState::ReceiveElementalDeleteMessage( CNetworkMessage *istr )
{
	// MSG_EX_ELEMENTAL_DELETE,	// 소환수 제거		: index(n)
	LONG lIndex;
	(*istr) >> lIndex;

	CUIManager* pUIManager = CUIManager::getSingleton();

	ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_SLAVE, lIndex);

	if (pObject != NULL)
	{
		CSlaveTarget* pTarget = static_cast< CSlaveTarget* >(pObject);

		// 내 소환수인 경우...
		if( pTarget->slave_OwnerIndex == _pNetwork->MyCharacterInfo.index )
		{
			for (int i = UI_SUMMON_START; i <= UI_SUMMON_END; ++i)
			{
				CUISummon* pUISummon = (CUISummon*)pUIManager->GetUI(i);
				
				if( pUISummon->GetSummonEntity() && pUISummon->GetSummonIndex() == lIndex )
				{
					pUISummon->ResetSummon();						
					break;
				}
			}			
		}

		SlaveInfo().StopIdleEffect( lIndex );

		//해당 소환수의 타겟 이펙트를 없앰...
		pUIManager->StopTargetEffect( lIndex );

		((CPlayerEntity*)CEntity::GetPlayerEntity(0))->ClearTargetInfo(pTarget->GetEntity());

		ACTORMGR()->RemoveObject(eOBJ_SLAVE, lIndex);
	}
}

// ----------------------------------------------------------------------------
// Name : ReceiveEvocationStart()
// Desc : 
// ----------------------------------------------------------------------------
void CSessionState::ReceiveEvocationStart( CNetworkMessage *istr )
{
	// charindex(n) type(c)
	LONG	lCharIndex;
	INDEX	iType;

	(*istr) >> lCharIndex;
	(*istr) >> iType;

	((CPlayerEntity*)CEntity::GetPlayerEntity(0))->EvocationStart( lCharIndex, iType);	
}

// ----------------------------------------------------------------------------
// Name : ReceiveEvocationStop()
// Desc : 
// ----------------------------------------------------------------------------
void CSessionState::ReceiveEvocationStop( CNetworkMessage *istr )
{
	// charindex(n)
	LONG	lCharIndex;	
	(*istr) >> lCharIndex;	
	((CPlayerEntity*)CEntity::GetPlayerEntity(0))->EvocationStop( lCharIndex );	
}


//////////////////////////////////////////////////////////////////////////

// ----------------------------------------------------------------------------
//  Name 	: 
//  Desc 	: 
// ----------------------------------------------------------------------------
void AffinityErrChk( UBYTE errType )
{
	CUIMsgBox_Info MsgBoxInfo;
	CTString		MsgBoxMsg;
	MsgBoxInfo.SetMsgBoxInfo( _S( 191, "확인" ), UMBS_OK, UI_NONE, MSGCMD_NULL );

	CUIManager* pUIManager = CUIManager::getSingleton();

	switch( errType )
	{
		case MSG_EX_AFFINITY_ERROR_CONNECT_SUCCESS:
		{
			MsgBoxMsg.PrintF( _S( 4671, "성공적으로 친구를 맺었습니다." ) );
		}
			break;
		case MSG_EX_AFFINITY_ERROR_CONNECT_NOMONEY:			// 나스부족
		{
			MsgBoxMsg.PrintF( _S(306, "나스가 부족합니다.") );
		}
			break;
		case MSG_EX_AFFINITY_ERROR_CONNECT_ALREADY:			// 이미세력 친구맺음
		{
			MsgBoxMsg.PrintF( _S( 4672, "이미 세력과 친구를 맺었습니다." ) );
		}
			break;
		case MSG_EX_AFFINITY_ERROR_ITEM_CONTRIBUTE_MAX:		// 친화도 포인트 최대로 찼음
		{
			MsgBoxMsg.PrintF( _S( 4673, "친화도가 가득 찼습니다. 더이상 친화도를 쌓을 수 없습니다." ) );
		}
			break;
		case MSG_EX_AFFINITY_ERROR_ITEM_CONTRIBUTE_MISSMATCH:	// 기부 부적합 아이템
		{
			MsgBoxMsg.PrintF( _S( 4674, "기부할 수 없는 아이템입니다. 확인하고 다시 시도하여 주십시오." ) );
		}
			break;
		case MSG_EX_AFFINITY_ERROR_USESHOP_POINT:			// 상점이용 포인트 부족
		{
			MsgBoxMsg.PrintF( _S( 4675, "아직 당신에게는 팔 물건이 없습니다. 우리 %s 을 위해서 조금더 노력을 해 주십시오." ), 
			_pNetwork->GetAffinityData()->GetAffinityName( pUIManager->GetAffinity()->GetNPCIndex() ) );	// [2010/12/07 : Sora] 친화도 개선 2차
		}
			break;
		case MSG_EX_AFFINITY_ERROR_GIFTINFO_NOITEM:			// 보상받을 아이템이 없다
		{
			MsgBoxMsg.PrintF( _S( 4676, "모든 보상을 받았습니다. 더이상 보상받을 아이템이 없습니다." ) );
		}
			break;
		case MSG_EX_AFFINITY_ERROR_GIFTINFO_NOPOINT:			// 포인트부족으로 보상 못받음
		{
			MsgBoxMsg.PrintF( _S( 4677, "친화도가 부족합니다." ) );
		}
			break;
		case MSG_EX_AFFINITY_ERROR_GIFT_NOSPACE:			// 인벤에 아이템 받을 공간이 없다.
		{
			MsgBoxMsg = _S(2850, "인벤토리가 가득 찼습니다.");
		}
			break;
		case MSG_EX_AFFINITY_ERROR_NOT_CONNECTED:
		{
			MsgBoxMsg = _S( 4678, "아직 친구를 맺지 않았습니다. 먼저 친구를 맺어주세요." );
		}
			break;
		case MSG_EX_AFFINITY_ERROR_CONNECT_PCLEVEL:
		{
			MsgBoxMsg = _S( 965, "레벨이 부족합니다." );
		}
			break;
		case MSG_EX_AFFINITY_ERROR_CONNECT_NOITEM:
		{
			MsgBoxMsg = _S( 1330 ,"아이템이 부족합니다." );
		}
			break;
	}
	MsgBoxInfo.AddString( MsgBoxMsg );
	pUIManager->CreateMessageBox( MsgBoxInfo );
}

// ----------------------------------------------------------------------------
//  Name 	: ReceiveAffinityMessage()
//  Desc 	: 
// ----------------------------------------------------------------------------
void CSessionState::ReceiveAffinityMessage( CNetworkMessage *istr )
{
	UBYTE	ubType;
	UBYTE	errCode;
	(*istr) >> ubType;

	CUIManager* pUIManager = CUIManager::getSingleton();

	switch( ubType )
	{
		case MSG_EX_AFFINITY_ITEM_CONTRIBUTE_REP:
		{
			CPrintF("Affinity Contribute success!!\n" );
		}
			break;
		case MSG_EX_AFFINITY_SHOPUSE_REP:
		{
			SLONG		npc_idx = 0;
			(*istr) >> npc_idx;
			pUIManager->GetAffinity()->OpenShop( npc_idx );
		}
			break;
		case MSG_EX_AFFINITY_GIFT_REP:
		{
			// [100324: selo] 선물 받은 npc 는 알림 항목에서 제거 한다.
			SLONG npc_idx = 0;
			(*istr) >> npc_idx;

			_pNetwork->RemoveAffinityRewardNPC(npc_idx);

			if( npc_idx > 0)
			{
				Notice* pNotice = GAMEDATAMGR()->GetNotice();

				if (pNotice != NULL)
					pNotice->DelFromNoticeList(npc_idx, Notice::NOTICE_AFFINITY);
			}
		}
			break;
		case MSG_EX_AFFINITY_GIFTINFO_REP:
		{
			SLONG		gift_point = 0;
			SLONG		gift_item_index = 0;
			SLONG		gift_item_count = 0;
			CTString	MsgTemp;
			CUIMsgBox_Info	Msg_Info;

			(*istr) >> gift_point;
			(*istr) >> gift_item_index;
			(*istr) >> gift_item_count;

			MsgTemp.PrintF( _S( 5302, "%s 세력을 위해 많은 노력을 해주셔서 친화도가 %d점이 되어 작은 선물을 준비했습니다. \n%s  %d개를 받으시겠습니까?" ),
				_pNetwork->GetAffinityData()->GetAffinityName( pUIManager->GetAffinity()->GetNPCIndex() ), gift_point, _pNetwork->GetItemData( gift_item_index )->GetName(), gift_item_count );
			
			Msg_Info.SetMsgBoxInfo( _S(4680, "선물 받기"), UMBS_OKCANCEL, UI_NPC_AFFINITY, MSGCMD_AFFINITY_TAKEPRESENT );
			Msg_Info.AddString( MsgTemp );
			pUIManager->CreateMessageBox( Msg_Info );
		}
			break;
		case MSG_EX_AFFINITY_UPDATE_POINT:
		{
			SLONG	affinity_index = 0;
			SLONG	affinity_point = 0;
			SLONG	affinity_point_bonus = 0;
			
			(*istr) >> affinity_index;
			(*istr) >> affinity_point; // total (길드스킬로 증가된양이 포함된값)
			(*istr) >> affinity_point_bonus; // 길드스킬로 증가된 양

			CNetworkLibrary::_AffinityInfo::mapAffIter iter;

			iter = _pNetwork->AffinityInfo.mapAffinityList.find( affinity_index );
			if( iter != _pNetwork->AffinityInfo.mapAffinityList.end() )
			{
				CTString MsgTemp;
				int ndelta = affinity_point - (*iter).second.current; // 기존 친화도에서 증가값 얻기
				// [2010/12/07 : Sora] 친화도 개선 2차
				if (affinity_point_bonus > 0)
				{
					MsgTemp.PrintF( _S(6445, "%d(+%d)만큼 %s친화도가 상승되었습니다."), ndelta - affinity_point_bonus, affinity_point_bonus,
						_pNetwork->GetAffinityData()->GetAffinityNameByIndex(affinity_index) );
				}
				else
				{
					MsgTemp.PrintF( _S(4681, "%d만큼 %s친화도가 상승되었습니다."), ndelta,
						_pNetwork->GetAffinityData()->GetAffinityNameByIndex(affinity_index) );
				}				

				((*iter).second).current = affinity_point;
				pUIManager->GetChattingUI()->AddSysMessage( MsgTemp );

				//2012/11/09 jeil 친화도 부스터 아이탬관련 체크 하여 메시지 출력 추가 
				if(_pUIBuff->IsBuff(9189)){	//버프 번호 체크 
					MsgTemp.PrintF(_S(5815, "(친화도 부스터 적용)"));	//스트링에 메시지 담기
					pUIManager->GetChattingUI()->AddSysMessage( MsgTemp );	//메시지 채팅창에 출력
				}
				//2012/11/09 jeil 임시 친화도 아이템 플레그 체크하여 메시지 출력 
				//친화도 부스터 아이탬 적용중인지 체크 필요 -> 물어보자
				//친화도 부스터 적용 스트링 필요 ->불어보자 
//				MsgTemp.PrintF("(친화도 부스터 적용)");	//스트링에 메시지 담기
//				pUIManager->GetChattingUI()->AddSysMessage( MsgTemp );	//메시지 채팅창에 출력

			}
			else
				CPrintF("Affinity Index Error !!\n" );
		}
			break;
		case MSG_EX_AFFINITY_LISTINFO:
		{
			SLONG	affinity_cnt = 0;
			SLONG	affinity_index = 0;
			SLONG	affinity_point = 0;
			SLONG	affinity_max = 0;
			CNetworkLibrary::_AffinityInfo::_AffinityPoint AfPoint;
			(*istr) >> affinity_cnt;

			_pNetwork->AffinityInfo.count = affinity_cnt;

			for( int i = 0; i < affinity_cnt; ++i )
			{
				(*istr) >> affinity_index;
				(*istr) >> affinity_point;
				(*istr) >> affinity_max;

				AfPoint.current = affinity_point;
				AfPoint.max		= affinity_max;

				_pNetwork->AffinityInfo.mapAffinityList.insert( std::make_pair( affinity_index, AfPoint ) );
			}
		}
			break;
		case MSG_EX_AFFINITY_ADDINFO:
		{
			// [6/8/2009 rumist] if server was accept that connect to npc forces, called this message.
			SLONG affinity_index;
			SLONG affinity_max = 0;
			CNetworkLibrary::_AffinityInfo::_AffinityPoint AfPoint;
			AfPoint.current = 0;
			(*istr) >> affinity_index;

			(*istr) >> affinity_max;
			AfPoint.max = affinity_max;

			// 친화도 개편2 친화도 세력 맺을때 리스트 업데이트 [2/14/2013 Ranma]
			CNetworkLibrary::_AffinityInfo::mapAffIter iter = _pNetwork->AffinityInfo.mapAffinityList.find( affinity_index );
		
			if ( iter == _pNetwork->AffinityInfo.mapAffinityList.end() )
			{
				_pNetwork->AffinityInfo.mapAffinityList.insert( std::make_pair( affinity_index, AfPoint ) );
			}
			else
			{
				((*iter).second).max = affinity_max;
			}
			
			// [2013/02/05] sykim70 친화도 기부 UI 개선
			_pNetwork->AffinityInfo.count = _pNetwork->AffinityInfo.mapAffinityList.size();
		}
			break;
		case MSG_EX_AFFINITY_ERROR:
		{
			(*istr) >> errCode;
			AffinityErrChk( errCode );
		}	
			break;
			
		// [100322: selo] 친화도 보상 알람		
		case MSG_EX_AFFINITY_REWARD_NOTICE:
		{					 
			INDEX noticecount = 0;
			INDEX npcidx = 0;

			(*istr) >> noticecount;

			if( noticecount > 0 )
			{
				Notice* pNotice = GAMEDATAMGR()->GetNotice();

				for( int i = 0; i < noticecount; i++ )
				{
					(*istr) >> npcidx;
					_pNetwork->AddAffinityRewardNPC(npcidx);

					if (pNotice != NULL && npcidx > 0)
						pNotice->AddToNoticeList(npcidx, Notice::NOTICE_AFFINITY);
				}
			}
		}
		break;

		// 친화도 개편2 보상 정보 받음 [2/13/2013 Ranma]		
		case MSG_EX_AFFINITY_INFOTAB_REP:
			{					 
				pUIManager->GetAffinityInfo()->RecvAffinity_RewardInfo(istr);
			}
		break;
	}
}

// ----------------------------------------------------------------------------
//  Name 	: ReceiveExLoadingEndMessage()
//  Desc 	: 
// ----------------------------------------------------------------------------
void CSessionState::ReceiveExLoadingEndMessage(CNetworkMessage* istr)
{
	UBYTE	ubType;
	(*istr) >> ubType;

	switch( ubType )
	{
		case MSG_EX_LODING_END_NPC_PORTAL_USE:
		{
			CUIManager::getSingleton()->GetNpcScroll()->UseNPCScroll();
		}
			break;
	}
}

// ----------------------------------------------------------------------------
//  Name 	: ReceiveNickNameMessage()
//  Desc 	: 
// ----------------------------------------------------------------------------
void CSessionState::ReceiveNickNameMessage(CNetworkMessage* istr)
{
	SLONG	slType;
	INDEX	iCount, iNickIndex, iDate, iCharIndex, iItemIndex;
	CTString strSysMessage, strTitle;
	CUIMsgBox_Info	MsgBoxInfo;
	
	strTitle = _S(4826, "호칭");

	(*istr) >> slType;

	CUIManager* pUIManager = CUIManager::getSingleton();

	switch( slType )
	{
	case MSG_EX_TITLE_SYSTEM_LIST_REP_NO_LIST:	// 호칭 리스트 없음
		{
			pUIManager->GetNickName()->Open();	
		}
		break;
	case MSG_EX_TITLE_SYSTEM_LIST_REP_EXIST_LIST:	// 호칭 리스트 전송
		{
			(*istr) >> iCount;
			
			pUIManager->GetNickName()->Open();	
			
			for (int i = 0 ; i < iCount ; ++i)
			{
				(*istr) >> iNickIndex;
				(*istr) >> iDate;

				pUIManager->GetNickName()->AddNickNameList( iNickIndex, iDate);
			}

			pUIManager->GetNickName()->SortNickNameData();
		}
		break;
	case MSG_EX_TITLE_SYSTEM_ALREADY_HAVE_TITLE:	// 아이템 사용 시 이미 가지고 있는 호칭
		{
			(*istr) >> iNickIndex;

			strSysMessage	=_S(4827, "이미 등록되어 있는 호칭입니다.");
		}
		break;
	case MSG_EX_TITLE_SYSTEM_INSERT_SUCCESS:		// 아이템 사용 시 호칭 목록에 추가 성공
		{
			(*istr) >> iNickIndex;
			(*istr) >> iDate;

			pUIManager->GetNickName()->AddNickNameList( iNickIndex, iDate);
			pUIManager->GetNickName()->SortNickNameData();
			//  [3/25/2010 kiny8216] 호칭 이름을 아이템 lod에서 불러오도록 변경
			iItemIndex = CNickNameData::getData(iNickIndex)->GetItemIndex();
#if !(defined (G_GERMAN) || defined (G_EUROPE2) || defined (G_EUROPE3) || defined (G_NETHERLANDS))
		if( pUIManager->GetHelp()->IsBeginner() )
		{
			_pNetwork->SendNickNameMessage( MSG_EX_TITLE_SYSTEM_TITLE_SELECT_REQ, iNickIndex );	
		}
		else
#endif
				strSysMessage.PrintF(_S(4828, "%s 호칭을 추가하였습니다."), pUIManager->GetNickName()->GetName(iItemIndex));
		}
		break;
	case MSG_EX_TITLE_SYSTEM_NOT_HAVE_TITLE:		// 호칭 선택 요청하였으나 서버에 목록이 없을 때
		{
			strSysMessage	=_S(4829, "존재하지 않는 호칭 입니다.");
		}
		break;
	case MSG_EX_TITLE_SYSTEM_TITLE_SELECT_SUCCESS:	// 타이틀 선택 성공
		{
			(*istr) >> iNickIndex;
			(*istr) >> iCharIndex;
			
			if ( iCharIndex == _pNetwork->MyCharacterInfo.index )
			{
				_pNetwork->MyCharacterInfo.iNickType = iNickIndex;
				_pNetwork->MyCharacterInfo.itemEffect.DeleteNickEffect();
				((CPlayerEntity*)CEntity::GetPlayerEntity(0))->SetNickNameDamageEffect(_pNetwork->MyCharacterInfo.iNickType, CEntity::NICKNAME_ATTACH_EFFECT);
				//  [3/25/2010 kiny8216] 호칭 이름을 아이템 lod에서 불러오도록 변경
				iItemIndex = CNickNameData::getData(iNickIndex)->GetItemIndex();
		
				if( !pUIManager->GetHelp()->IsBeginner() )
					strSysMessage.PrintF(_S(4830,"%s 호칭으로 설정 완료하였습니다."), pUIManager->GetNickName()->GetName(iItemIndex));
				else
					pUIManager->GetHelp()->SetBeginner( FALSE );
				
				pUIManager->GetNickName()->SetCancelButtonState( TRUE );
			}
			else
			{
				ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_CHARACTER, iCharIndex);

				if (pObject != NULL)
				{
					CCharacterTarget* pTarget = static_cast< CCharacterTarget* >(pObject);

					pTarget->cha_NickType = iNickIndex;
					pTarget->cha_itemEffect.DeleteNickEffect();
					pTarget->m_pEntity->SetNickNameDamageEffect(pTarget->cha_NickType, CEntity::NICKNAME_ATTACH_EFFECT);
				}
			}		
		}
		break;
	case MSG_EX_TITLE_SYSTEM_TITLE_CANCEL_SUCCESS:	// 타이틀 취소 성공
		{
			(*istr) >> iNickIndex;
			(*istr) >> iCharIndex;
			
			if ( iCharIndex == _pNetwork->MyCharacterInfo.index )
			{
				_pNetwork->MyCharacterInfo.iNickType = 0;
				_pNetwork->MyCharacterInfo.itemEffect.DeleteNickEffect();

				strSysMessage = _S(4831, "호칭이 정상적으로 취소되었습니다.");
				pUIManager->GetNickName()->SetCancelButtonState( FALSE );
			}
			else
			{
				ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_CHARACTER, iCharIndex);

				if (pObject != NULL)
				{
					CCharacterTarget* pTarget = static_cast< CCharacterTarget* >(pObject);

					pTarget->cha_NickType = 0;
					pTarget->cha_itemEffect.DeleteNickEffect();
				}
			}
			break;
		}
	case MSG_EX_TITLE_SYSTEM_TITLE_DELETE_SUCCESS:	// 타이틀 삭제 성공
		{
			(*istr) >> iNickIndex;
			
			pUIManager->GetNickName()->DelNickNameList( iNickIndex );
			//  [3/25/2010 kiny8216] 호칭 이름을 아이템 lod에서 불러오도록 변경
			iItemIndex = CNickNameData::getData(iNickIndex)->GetItemIndex();
			strSysMessage.PrintF(_S(4832, "정상적으로 %s 호칭을 삭제하였습니다."), pUIManager->GetNickName()->GetName(iItemIndex));
		}
		break;
	case MSG_EX_TITLE_SYSTEM_TITLE_EXPIRED:			// 타이틀 기간 만료
		{
			(*istr) >> iNickIndex;
			(*istr) >> iCharIndex;
			if ( iCharIndex == _pNetwork->MyCharacterInfo.index )
			{
				_pNetwork->MyCharacterInfo.iNickType = 0;
				_pNetwork->MyCharacterInfo.itemEffect.DeleteNickEffect();
				pUIManager->GetNickName()->DelNickNameList( iNickIndex );
			}
			else
			{
				ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_CHARACTER, iCharIndex);

				if (pObject != NULL)
				{
					CCharacterTarget* pTarget = static_cast< CCharacterTarget* >(pObject);

					pTarget->cha_NickType = 0;
					pTarget->cha_itemEffect.DeleteNickEffect();
				}
			}
		}
		break;
	case MSG_EX_TITLE_SYSTEM_TITLE_CHANGE_TIME:
		{
			(*istr) >> iNickIndex;
			(*istr) >> iDate;
			
			pUIManager->GetNickName()->ChangeTitleTime( iNickIndex, iDate );
			break;
		}
	}
	if (strSysMessage != CTString(""))
	{
		MsgBoxInfo.SetMsgBoxInfo(strTitle,UMBS_OK,UI_NONE,MSGCMD_NULL);
		MsgBoxInfo.AddString(strSysMessage);
		pUIManager->CreateMessageBox(MsgBoxInfo);
	}
}

// ----------------------------------------------------------------------------
// [100208: selo] 
//  Name 	: ReceiveTakeAgainQuestItem()
//  Desc 	: 퀘스트 아이템 다시 받기 ( 그림자 문의 열쇠 )
// ----------------------------------------------------------------------------
void CSessionState::ReceiveTakeAgainQuestItem(CNetworkMessage* istr)
{
	CTString	strMessage;
	CUIMsgBox_Info	MsgBoxInfo;

	SLONG	slType;
	(*istr) >> slType;

	switch( slType )
	{
	case MSG_EX_TAKE_AGAIN_QUEST_ITEM_SUCCESS:
		{
			strMessage.PrintF( _S( 4820, "정상적으로 아이템이 지급되었습니다.") );
		}
		break;
	case MSG_EX_TAKE_AGAIN_QUEST_ITEM_ERROR_ALREADY_EXIST:
		{
			strMessage.PrintF( _S( 4821, "인벤토리에 아이템이 존재합니다.") );
		}
		break;
	case MSG_EX_TAKE_AGAIN_QUEST_ITEM_ERROR_FULL_INVENTORY:
		{
			strMessage.PrintF( _S( 3796, "인벤토리가 가득차서 아이템을 받을 수 없습니다.") );
		}
		break;
	}

	CUIManager* pUIManager = CUIManager::getSingleton();

	if (pUIManager->DoesMessageBoxExist(MSGCMD_QUEST_RESTORE_ITEM_RESULT))
		pUIManager->CloseMessageBox(MSGCMD_QUEST_RESTORE_ITEM_RESULT);

	MsgBoxInfo.SetMsgBoxInfo( _S(191,"확인"), UMBS_OK, UI_QUEST, MSGCMD_QUEST_RESTORE_ITEM_RESULT );
	MsgBoxInfo.AddString( strMessage );
	pUIManager->CreateMessageBox( MsgBoxInfo );	
}

// ----------------------------------------------------------------------------
//  [4/15/2010 kiny8216] 코스튬 시스템2 메시지
//  Name 	: RecieveCostume2Message()
//  Desc 	: 코스튬 시스템2 메시지
// ----------------------------------------------------------------------------
void CSessionState::RecieveCostume2Message(CNetworkMessage* istr)
{
	UCHAR ucType;
	CTString	strMessage;

	(*istr) >> ucType;

	switch( ucType )
	{
	case 0:
		strMessage.PrintF( _S( 4868, "착용하려는 무기가 코스튬 무기와 타입이 틀립니다.") );
		break;
	case 1:
	case 3:
		strMessage.PrintF( _S( 4869, "방패를 착용한 상태에서는 착용할 수 없습니다.") );
		break;
	case 2:
		strMessage.PrintF( _S( 4870, "착용하려는 코스튬 무기가 장비와 타입이 틀립니다.") );
		break;
	case 4:
		strMessage.PrintF( _S( 4871, "코스튬 듀얼소드를 착용한 상태에서는 방패를 착용할 수 없습니다.") );
		break;
	case 5:
		strMessage.PrintF( _S( 4872, "듀얼소드를 착용한 상태에서는 코스튬 방패를 착용할 수 없습니다.") );
		break;
	}

	if(strMessage.Length()>0 )
		CUIManager::getSingleton()->GetChattingUI()->AddSysMessage( strMessage , SYSMSG_ERROR );	

	// 장비 착용 시도 상태 해제
	CUIManager::getSingleton()->SetCSFlagOffElapsed( CSF_ITEMWEARING );
}

// SOCKET_SYSTEM_S2 소켓 시스템 관련 메시지 받는 부분 [4/3/2013 Ranma]
void CSessionState::RecieveSocketSystemMessage(CNetworkMessage* istr )
{
	UBYTE	slType;
	(*istr) >> slType;

	CUIManager* pUIManager = CUIManager::getSingleton();

	switch( slType )
	{
		case MSG_EX_SOCKET_MAKE_REP:
			{
				SLONG idx;
				SBYTE socketCount;
				(*istr) >> idx;
				(*istr) >> socketCount;

				// 성공시 ui 업데이트가 필요함.
				// 무결성 유지를 위해서 서버로부터 idx를 받는다.
				pUIManager->GetSocketSystem()->CompleteProgress();
				pUIManager->GetSocketSystem()->UpdateUI( 2, idx );
				//pUIManager->GetSocketSystem()->UpdateCreateUI();
			}
			break;
		case MSG_EX_SOCKET_COMBINE_JEWEL_REP:
			{
				SLONG idx;
				(*istr) >> idx;
				pUIManager->GetSocketSystem()->CompleteProgress();
				pUIManager->GetSocketSystem()->UpdateUI( 0, idx );
				pUIManager->GetSocketSystem()->UpdateCombineUI(TRUE, true);
			}
			break;

		case MSG_EX_SOCKET_UPGRADE_JEWEL_REP:
		case MSG_EX_SOCKET_UPGRADE_JEWEL_CHAOS_REP:
			{
				SLONG jewelIdx;
				(*istr) >> jewelIdx;
				pUIManager->GetSocketSystem()->CompleteProgress();
				pUIManager->GetSocketSystem()->UpdateJewelComposUI(CUISocketSystem::GENARAL_JEWEL_COMPOS, jewelIdx);
			}
			break;

		case MSG_EX_SOCKET_INFO_REP:
			{
			}
			break;

		case MSG_EX_SOCKET_CLEAN_JEWEL_REP:
			{
				SLONG idx;
				(*istr) >> idx;
				CUIMsgBox_Info MsgInfo;
				MsgInfo.SetMsgBoxInfo( _S( 191, "확인" ), UMBS_OK, UI_NONE, MSGCMD_NULL );
				MsgInfo.AddString( _S( 5000, "소켓 비우기 작업을 정상적으로 완료하였습니다.") );
				pUIManager->CreateMessageBox( MsgInfo );
				pUIManager->GetSocketSystem()->CloseSocketSystem();
			}
			break;

		case MSG_EX_SOCKET_ERROR:
			{
				UBYTE errcode;
				(*istr) >> errcode;
				CheckSocketSystemError( errcode );
			}
			break;
	}
}	

// 에러 출력은 메시지로 할건지 박스로 처리할건지 논의 필요. [5/4/2010 rumist]	
void CheckSocketSystemError(UBYTE errcode)
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	CTString tStr;

	switch(errcode)
	{
		case MSG_EX_SOCKET_ERROR_ITEM_MISSMATCH:    // 올바른 아이템이 아닙니다.
				tStr = _S( 4981, "올바른 아이템이 아닙니다. 확인 후 다시 시도해 주시기 바랍니다." );
				pUIManager->GetSocketSystem()->CompleteProgress();
				pUIManager->GetSocketSystem()->ClearProcessing();
				pUIManager->GetSocketSystem()->CloseSocketSystem();
				break;
		case MSG_EX_SOCKET_ERROR_FAILED_MAKE:		// 소켓 생성 실패
				tStr = _S( 5001, "소켓 생성 실패." );
				// 실패시에도 스크롤이 제거되어야 하므로 update가 필요.
				pUIManager->GetSocketSystem()->CompleteProgress();
				pUIManager->GetSocketSystem()->UpdateCreateUI(FALSE);
				break;
		case MSG_EX_SOCKET_ERROR_NOMONEY:			// 돈 없음	
				tStr = _S( 306, "나스가 부족합니다." );
				pUIManager->GetSocketSystem()->CompleteProgress();
				pUIManager->GetSocketSystem()->ClearProcessing();
				pUIManager->GetSocketSystem()->CloseSocketSystem();
				break;
		case MSG_EX_SOCKET_ERROR_MAX_SOCKET:		// 보석 결합시 소켓 개수 초과
				tStr = _S( 5002, "소켓 개수가 부족합니다." );
				pUIManager->GetSocketSystem()->CompleteProgress();
				pUIManager->GetSocketSystem()->ClearProcessing();
				pUIManager->GetSocketSystem()->CloseSocketSystem();
				break;
		case MSG_EX_SOCKET_ERROR_INVEN_NOSPACE:		// 인벤공간 부족
				tStr = _S( 265, "인벤토리 공간이 부족합니다." );
				pUIManager->GetSocketSystem()->CompleteProgress();
				pUIManager->GetSocketSystem()->ClearProcessing();
				pUIManager->GetSocketSystem()->CloseSocketSystem();
				break;
		case MSG_EX_SOCKET_ERROR_NOWEARITEM:		// 무기 방어구만할 수 있는 작업입니다.
				tStr = _S( 520, "무기나 방어구만 가능합니다." );
				pUIManager->GetSocketSystem()->CompleteProgress();
				pUIManager->GetSocketSystem()->ClearProcessing();
				pUIManager->GetSocketSystem()->CloseSocketSystem();
				break;
		case MSG_EX_SOCKET_ERROR_NOSPACE:			// 소켓이 없는 아이템으로는 작업을 진행할 수 없습니다
				tStr = _S( 4998, "소켓이 생성되지 않은 아이템입니다." );
				pUIManager->GetSocketSystem()->CompleteProgress();
				pUIManager->GetSocketSystem()->ClearProcessing();
				pUIManager->GetSocketSystem()->CloseSocketSystem();
				break;
		case MSG_EX_SOCKET_ERROR_NOJEWEL:			// 소켓이 비워있는 아이템은 작업을 진행할 수 없습니다
				tStr = _S( 5003, "이미 보석이 결합되어 있는 아이템입니다." );
				pUIManager->GetSocketSystem()->CompleteProgress();
				pUIManager->GetSocketSystem()->ClearProcessing();
				pUIManager->GetSocketSystem()->CloseSocketSystem();
				break;

		case MSG_EX_SOCKET_ERROR_FAILED_COMBINE: // 보석 장착 실패
				pUIManager->GetSocketSystem()->CompleteProgress();
				pUIManager->GetSocketSystem()->UpdateCombineUI(FALSE);
				break;
	}
	
	if(tStr.Length()>0 )
		pUIManager->GetChattingUI()->AddSysMessage( tStr , SYSMSG_ERROR );	
}


// ----------------------------------------------------------------------------
// [9/29/2010 kiny8216] 성주 코스튬
//  Name 	: RecieveCostume2Message()
//  Desc 	: 성주 코스튬 메시지
// ----------------------------------------------------------------------------
void CSessionState::RecieveLordCostumeMessage( CNetworkMessage* istr )
{
	UBYTE ubType;
	CTString strMessage;
	CUIMsgBox_Info MsgBoxInfo;

	(*istr) >> ubType;
	switch( ubType )
	{
	case MSG_EX_CASTLLAN_ERROR_GIVE_ITEM_ERROR:
		{	// 지급할 아이템이 없음
			strMessage = _S(2502, "지급 받을 아이템이 없습니다.");
		}
		break;
	case MSG_EX_CASTLLAN_ERROR_GIVE_ITEM_SUC:
		{	// 지급 성공
			strMessage = _S(1840, "아이템이 성공적으로 지급 되었습니다");
		}
		break;
	case MSG_EX_CASTLLAN_ERROR_NOT_CASTLLAN:
		{	// 성주가 아님
			strMessage = _S(1722, "보상을 받으실 수 있는 조건이 아닙니다");
		}
		break;
	case MSG_EX_CASTLLAN_ERROR_NOT_CASTLLAN_GUILD:
		{
			// 성주길드가 아님
			strMessage = _S( 3743, "성주 길드가 아닙니다." );
		}break;
	}

	if ( strMessage.Length() > 0)
	{
		MsgBoxInfo.SetMsgBoxInfo( _S(191,"확인"), UMBS_OK, UI_NONE, MSGCMD_NULL);
		MsgBoxInfo.AddString( strMessage );
		CUIManager::getSingleton()->CreateMessageBox( MsgBoxInfo );
	}
}

// ----------------------------------------------------------------------------
//  [2010/08/25 : Sora] ADD_SUBJOB
//  Name 	: RecieveSubJobMessage()
//  Desc 	: 보조직업 메시지 처리
// ----------------------------------------------------------------------------
void CSessionState::RecieveSubJobMessage(CNetworkMessage* istr)
{
	SLONG	slType;
	(*istr) >> slType;

	switch( slType )
	{
		case MSG_EX_SUBJOB_ERROR_SUCESS:
			{
				SLONG subJobCode;
				(*istr) >> subJobCode;
				CTString strMessage;
				strMessage.PrintF( _S( 5083, "[%s]으로 등록 되었습니다." ), CUIManager::getSingleton()->GetSubJobName(subJobCode) );
				_pNetwork->ClientSystemMessage(strMessage, SYSMSG_NORMAL);
				_pNetwork->MyCharacterInfo.slSubJob = subJobCode;
			}
			break;
		case MSG_EX_SUBJOB_ERROR_FAIL:
			{
				SLONG errcode;
				(*istr) >> errcode;
				switch( errcode )
				{
					case 1:
						{
							_pNetwork->ClientSystemMessage(_S( 965, "레벨이 부족합니다." ), SYSMSG_ERROR);
						}
						break;
					case 2:
						{
							_pNetwork->ClientSystemMessage(_S( 1217, "명성치가 부족합니다." ), SYSMSG_ERROR);
						}
						break;
					case 3:
						{
							_pNetwork->ClientSystemMessage(_S( 966, "SP가 부족합니다." ), SYSMSG_ERROR);
						}
						break;
					case 4:
						{
							_pNetwork->ClientSystemMessage(_S( 967, "나스가 부족합니다." ), SYSMSG_ERROR);
						}
						break;
				}
			}		
			break;
		case MSG_EX_SUBJOB_ERROR_NOT_TRADER:
			{
				CUIMsgBox_Info MsgInfo;
				MsgInfo.SetMsgBoxInfo( _S( 1748, "안내" ), UMBS_OK, UI_NONE, MSGCMD_NULL );
				MsgInfo.AddString( _S( 5050, "상인으로 등록되어 있지 않습니다.") );
				CUIManager::getSingleton()->CreateMessageBox( MsgInfo );			
			}
			break;
	}
}

// ----------------------------------------------------------------------------
// [2010/10/20 : Sora] 몬스터 용병 카드
//  Name 	: RecieveMonsterMercenaryCardMessage()
//  Desc 	: 
// ----------------------------------------------------------------------------
void CSessionState::RecieveMonsterMercenaryCardMessage(CNetworkMessage* istr)
{
	UBYTE ubType;
	CTString strMessage;
	CUIMsgBox_Info MsgBoxInfo;

	CUIManager* pUIManager = CUIManager::getSingleton();
	pUIManager->CloseMessageBox(MSGCMD_MONSTER_MERCENARY_CARD_ERROR);

	(*istr) >> ubType;

	switch( ubType )
	{
		case MSG_EX_MERCENARY_SUMMON:		// 소환/ 해제 성공
			{
				SLONG uniqItemIndex;
				SBYTE toggle;
				//용병창 토글
				(*istr) >> uniqItemIndex;
				(*istr) >> toggle;

				if( toggle > 0 )
				{
					CUIIcon* pIcon = pUIManager->GetInventory()->GetItemIcon(uniqItemIndex);

					if (pIcon != NULL)
						pUIManager->GetMonsterMercenary()->SetUseItemData(pIcon->getIndex());
				}
				pUIManager->GetMonsterMercenary()->ToggleMonsterMercenary( toggle );
			}
			break;
		case MSG_EX_MERCENARY_LIMIT_TIME:	// 남은 시간 정보
			{
				SLONG time;
				SLONG tab, nInvenIdx;

				(*istr) >> tab;
				(*istr) >> nInvenIdx;
				(*istr) >> time;

				CItems	&rItems = _pNetwork->MySlotItem[tab][nInvenIdx];

				strMessage.PrintF(_S( 5158, "[%s]%s용병 사용 기간 만료까지 %d분 남았습니다."), 
									pUIManager->GetMonsterMercenary()->GetMonsterGrade(rItems.Item_Used2),
									CMobData::getData(rItems.Item_Plus)->GetName(), time);

				pUIManager->GetChattingUI()->AddSysMessage( strMessage, SYSMSG_ERROR );
				return;
			}
			break;
		case MSG_EX_MERCENARY_ERROR:
			{
				UBYTE errcode;
				
				(*istr) >> errcode;

				switch( errcode )
				{
					case MSG_EX_MERCENARY_ERROR_USE_FAIL:		// 사용 실패
						{
							strMessage = _S( 5159, "용병을 소환 할 수 없습니다. 확인 후 다시 사용하십시오.");
						}
						break;
					case MSG_EX_MERCENARY_ERROR_SUMMON_ALREADY:	// 이미 다른넘이 소환되어 있어 사용할 수 없음
						{
							strMessage = _S( 5160, "이미 용병이 소환되어 있습니다.");
						}
						break;
					case MSG_EX_MERCENARY_ERROR_DONT_SUMMON:	// 소환 불가
						{
							strMessage = strMessage = _S( 5159, "용병을 소환 할 수 없습니다. 확인 후 다시 사용하십시오.");
						}
						break;
					case MSG_EX_MERCENARY_ERROR_MON_AUTO_DEL:	// 몬스터 자동 삭제(거리가 멀어졌거나 시간이 다되어서)
						{
							pUIManager->GetMonsterMercenary()->ToggleMonsterMercenary( -1 );

							strMessage = _S( 5161, "용병 소환이 해제되었습니다.");

							pUIManager->GetChattingUI()->AddSysMessage( strMessage, SYSMSG_ERROR );
							return;
						}
						break;
					case MSG_EX_MERCENARY_ERROR_AREA:	// 이 지역에서는 용병을 소환 할 수 없습니다. 
						{
							// 용병창 닫자
							strMessage = _S( 5162, "해당 지역에서는 용병을 소환 할 수 없습니다.");
						}
						break;
					default:
						{
							strMessage = _S( 5159, "용병을 소환 할 수 없습니다. 확인 후 다시 사용하십시오.");
						}
						break;
				}
			}
			break;
	}

	if ( strMessage.Length() > 0)
	{
		MsgBoxInfo.SetMsgBoxInfo( _S(191,"확인"), UMBS_OK, UI_NONE, MSGCMD_MONSTER_MERCENARY_CARD_ERROR);
		MsgBoxInfo.AddString( strMessage );
		pUIManager->CreateMessageBox( MsgBoxInfo );
	}
}

void CheckFaceOffError(UBYTE ubError)
{
	CTString strMessage;
	CUIMsgBox_Info	MsgBoxInfo;

	switch( ubError )
	{
	case MSG_EX_FACEOFF_ERROR_FAIL:
		{
			strMessage.PrintF( _S( 5186, "성형카드가 없거나 성형에 실패하였습니다. 잠시 후 다시 시도해 주세요."));
		}
		break;
	case MSG_EX_FACEOFF_ERROR_PLZ_CHOISE:
		{
			strMessage.PrintF( _S( 5187, "선택하신 외모는 현재 외모와 동일합니다."));
		}
		break;
	default:
		{
			strMessage.PrintF( _S( 5188, "외형 변경에 실패하였습니다."));
		}
		break;
	}

	CUIManager* pUIManager = CUIManager::getSingleton();

	pUIManager->GetInitJob()->CloseInitJob();

	if ( strMessage.Length() > 0 )
	{
		MsgBoxInfo.SetMsgBoxInfo( _S(191,"확인"), UMBS_OK, UI_NONE, MSGCMD_NULL);
		MsgBoxInfo.AddString( strMessage );
		pUIManager->CreateMessageBox( MsgBoxInfo );
	}
}
void CSessionState::RecieveFaceOffMessage( CNetworkMessage* istr )
{
	UBYTE ubType;
	INDEX iCharIndex;
	UBYTE ubFaceStyle, ubHairStyle;
	CUIMsgBox_Info	MsgBoxInfo;
	CTString strMessage;

	(*istr) >> ubType;

	CUIManager* pUIManager = CUIManager::getSingleton();

	switch( ubType )
	{
	case MSG_EX_FACEOFF_ERROR_SUC:
		{
			(*istr) >> ubHairStyle;
			(*istr) >> ubFaceStyle;
			(*istr) >> iCharIndex;

			if ( iCharIndex == _pNetwork->MyCharacterInfo.index )
			{	// 내 캐릭터가 face, hair change를 성공한 경우,
				_pNetwork->MyCharacterInfo.faceStyle = ubFaceStyle;
				_pNetwork->MyCharacterInfo.hairStyle = ubHairStyle;
				pUIManager->GetInitJob()->CloseInitJob();
				strMessage.PrintF( _S( 5185, "외형 변경에 성공했습니다."));
			}
			else
			{	// 다른 캐릭터가 face, hair change를 성공한 경우,
				ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_CHARACTER, iCharIndex);

				if (pObject != NULL)
				{
					if (((CPlayerEntity*)CEntity::GetPlayerEntity(0))->IsHelmet(iCharIndex) == TRUE)
						return;

					CCharacterTarget* pTarget = static_cast< CCharacterTarget* >(pObject);

					CModelInstance* pMI = pTarget->GetEntity()->en_pmiModelInstance;

					INDEX iJob = pTarget->GetType();
					((CPlayerEntity*)CEntity::GetPlayerEntity(0))->SetAppearanceData( pTarget->GetEntity()->en_ulID, ubHairStyle, ubFaceStyle);
					((CPlayerEntity*)CEntity::GetPlayerEntity(0))->ChangeHairMesh( pMI, iJob, ubHairStyle - 1);
					((CPlayerEntity*)CEntity::GetPlayerEntity(0))->ChangeFaceMesh( pMI, iJob, ubFaceStyle - 1);
					pTarget->cha_itemEffect.Refresh(&pMI->m_tmSkaTagManager, 1);
					pTarget->cha_statusEffect.Refresh(&pMI->m_tmSkaTagManager, CStatusEffect::R_NONE);
				}
			}
			PCStartEffectGroup("squid", iCharIndex);
		}
		break;
	default :
		{
			CheckFaceOffError( ubType );
		}
		break;
	}
	if ( strMessage.Length() > 0 )
	{
		MsgBoxInfo.SetMsgBoxInfo( _S(191,"확인"), UMBS_OK, UI_NONE, MSGCMD_NULL);
		MsgBoxInfo.AddString( strMessage );
		pUIManager->CreateMessageBox( MsgBoxInfo );
	}
}

void CSessionState::RecieveLuckyDrawBoxMessage( CNetworkMessage* istr )
{
	CUIMsgBox_Info	MsgBoxInfo;
	CTString strMessage;
	SLONG slErrorType;

	(*istr) >> slErrorType;

	switch( slErrorType )
	{
		case MSG_LUCKYDRAWBOX_ERROR_SUCESS:
			return;
		case MSG_LUCKYDRAWBOX_ERROR_NEED:
			{
				strMessage.PrintF( _S( 5311, "상자를 열기 위해서는 아이템이 필요합니다 확인 후 다시 시도하여 주십시오."));
			}
			break;
		case MSG_LUCKYDRAWBOX_ERROR_INVEN:
			{
				strMessage.PrintF( _S(4237, "인벤토리의 공간이 부족 합니다. 인벤토리를 비운 후 다시 시도해주시기 바랍니다."));
			}
			break;
		case MSG_LUCKYDRAWBOX_ERROR_USE:
			{
				strMessage.PrintF( _S(5312, "아이템을 사용할 수 없습니다."));
			}
			break;
		case MSG_LUCKYDRAWBOX_ERROR_OVER_WEIGHT:
			{
				strMessage.PrintF( _S(4237, "인벤토리의 공간이 부족 합니다. 인벤토리를 비운 후 다시 시도해주시기 바랍니다."));
			}
			break;
		default:
			{
				strMessage.PrintF( _S(5312, "아이템을 사용할 수 없습니다."));
			}
	}

	if ( strMessage.Length() > 0 )
	{
		MsgBoxInfo.SetMsgBoxInfo( _S(191,"확인"), UMBS_OK, UI_NONE, MSGCMD_NULL);
		MsgBoxInfo.AddString( strMessage );
		CUIManager::getSingleton()->CreateMessageBox( MsgBoxInfo );
	}
}


//added by sam 11/01/04 메시지 박스를 보여 줄 메시지를 서버에서 보내준다. 
void CSessionState::RecieveMsgBoxShow ( CNetworkMessage* istr )
{
	CUIMsgBox_Info	MsgBoxInfo;
	CTString strMessage;
	UCHAR chrErrorType;
	(*istr) >> chrErrorType;

	INDEX nTitle = 191;
	switch ( chrErrorType )
	{
		case MSG_EX_MSGBOX_INVEN_FULL:				// 인벤토리 가득참
			{
				strMessage.PrintF( _S( 265, "인벤토리 공간이 부족합니다." ));			
				
				break;			
			}
		case MSG_EX_MSGBOX_CHANGE_START_POINT:
			{
				MsgBoxInfo.SetMsgBoxInfo( _S(nTitle,"확인"), UMBS_YESNO, UI_NONE, MSGCMD_CHANGE_START_POINT, 200);
				MsgBoxInfo.AddString( _S(5376, "라스트 카오스에 오신 것을 환영합니다.") );
				MsgBoxInfo.AddString( _S(5377, "이곳은 쥬노 지역의 란돌 성 입니다.") );
				MsgBoxInfo.AddString( _S(5378, "모험가들의 휴식 공간으로써, 교류가 가장 활발한 곳입니다.") );
				MsgBoxInfo.AddString( _S(5379, "천천히 둘러 보시고 초보지역으로 워프를 원하시면 확인 버튼을 눌러주세요.") );
				MsgBoxInfo.AddString( _S(5380, "(단, 취소를 누를 시 초보지역으로 워프 할 수 없습니다.)") );
				MsgBoxInfo.AddString( _S(5381, "초보지역으로 이동 하시겠습니까?") );
				CUIManager::getSingleton()->CreateMessageBox(MsgBoxInfo);
				return;
			}
	}	

	if ( strMessage.Length() > 0 )
	{
		MsgBoxInfo.SetMsgBoxInfo( _S(nTitle,"확인"), UMBS_OK, UI_NONE, MSGCMD_NULL);
		MsgBoxInfo.AddString( strMessage );
		CUIManager::getSingleton()->CreateMessageBox( MsgBoxInfo );
	}
}

void	CSessionState::RecieveUsedPartyItemMessage(CNetworkMessage* istr )
{
	INDEX		iItemIdx = -1;
	CTString	strNickName;

	(*istr) >> iItemIdx;
	(*istr) >> strNickName;

	CUIManager::getSingleton()->GetNotice()->ShowUsedPartyItemNotice( iItemIdx, strNickName );
}


void	CSessionState::RecieveRoyalrumbleMessage(CNetworkMessage* istr )
{
	CUIMsgBox_Info	MsgBoxInfo;
	CTString strMessage;
	UBYTE msgIdx				= 0;
	(*istr) >> msgIdx;

	CUIManager* pUIManager = CUIManager::getSingleton();

	switch( msgIdx )
	{
		case MSG_EX_ROYAL_RUMBLE_NOTICE:						// 공지
			{
				UBYTE uResult = 0;
				(*istr) >> uResult;
				pUIManager->GetNotice()->ShowRoyalRumbleNotice( uResult );
			}
			break;
		case MSG_EX_ROYAL_RUMBLE_REG_MENU:						// 신청메뉴 활성화.
			{
				UBYTE uType = 0;
				(*istr) >> uType;
				pUIManager->GetRadar()->SetRoyalRumbleStatus( uType );
			}
			break;
		case MSG_EX_ROYAL_RUMBLE_PLAYER_REP:					// 등록 신청 결과.
			{
				MsgBoxInfo.SetMsgBoxInfo( _s(""), UMBS_OK, UI_NONE, MSGCMD_NULL);
				UBYTE uResult = 0;
				(*istr) >> uResult;
				if( uResult == 0 )
				{
					MsgBoxInfo.AddString( _S(5414, "로얄 럼블에 참가신청을 완료하였습니다.") );
				}
				else if( uResult == 1 )
				{
					MsgBoxInfo.AddString( _S(5415, "나스가 부족하여 로얄럼블에 참가하실 수 없습니다.") );
				}
				else if( uResult == 2 )
				{
					MsgBoxInfo.AddString( _S(5416, "선수등록이 불가능합니다.") );
				}
				else if( uResult == 3 )
				{
					MsgBoxInfo.AddString( _S(5417, "이미 신청하였습니다.") );
				}
				pUIManager->CreateMessageBox(MsgBoxInfo);
			}
			break;
		case MSG_EX_ROYAL_RUMBLE_PLAYER_UNREGIST_REP:			// 등록 취소.
			{
				MsgBoxInfo.SetMsgBoxInfo( _S(5405, "로얄럼블 신청 취소"), UMBS_OK, UI_NONE, MSGCMD_NULL);
				UBYTE uResult = 0;
				(*istr) >> uResult;
				if( uResult == 0 )
				{
					MsgBoxInfo.AddString( _S(5418, "성공적으로 취소되었습니다.") );
				}
				else if( uResult == 1 )
				{
					MsgBoxInfo.AddString( _S(5419, "취소할 수 있는 시간이 아닙니다.") );
				}
				else if( uResult == 2 )
				{
					MsgBoxInfo.AddString( _S(5420, "등록자 명단에 없습니다.") );
				}
				pUIManager->CreateMessageBox(MsgBoxInfo);
			}
			break;
		case MSG_EX_ROYAL_RUMBLE_PLAYER_REG_RESULT:				// 선수 선발 결과.
			{
				MsgBoxInfo.SetMsgBoxInfo( _S(5407, "로얄럼블 참가신청"), UMBS_OK, UI_NONE, MSGCMD_NULL);
				UBYTE uResult = 0;
				(*istr) >> uResult;
				if( uResult == 0 )
				{
					MsgBoxInfo.AddString( _s("신청만 된 상태") );
				}
				else if( uResult == 1 )
				{
					MsgBoxInfo.AddString( _S(5421, "축하드립니다. 로얄럼블에 선발되셨습니다. 장비를 정비하여 차례를 기다려 주세요.") );
				}
				else if( uResult == 2 )
				{
					MsgBoxInfo.AddString( _S(5422, "안타깝게도 로얄럼블에 선발되지 못하셨습니다. 다음 로얄럼블에 도전해 주세요.") );
				}
				else if( uResult == 3 )
				{
					MsgBoxInfo.AddString( _S(5423, "죄송합니다. 경기가 취소되었습니다.") );
				}
				pUIManager->CreateMessageBox(MsgBoxInfo);
			}
			break;
		case MSG_EX_ROYAL_RUMBLE_GO_ZONE:						// 이동하기
			{
				MsgBoxInfo.SetMsgBoxInfo( _S(5424, "대기실 이동"), UMBS_OK|UMBS_USE_TIMER, UI_RADAR, MSGCMD_ROYALRUMBLE_GO_ZONE);
								
				MsgBoxInfo.AddString( _S(5425,	"로얄럼블 대기실로 이동합니다."
												"이동하려면 확인 버튼을 누르세요."
												"(제한시간이 지나면 실격처리 됩니다.)" ) );
// 				MsgBoxInfo.AddString( _s("이동하려면 확인 버튼을 누르세요.") );
// 				MsgBoxInfo.AddString( _s("(제한시간이 지나면 실격처리 됩니다.)") );
				MsgBoxInfo.SetMsgBoxTimer( 30, FALSE );			// 30 seconds.
				pUIManager->CreateMessageBox(MsgBoxInfo);
			}
			break;
		case MSG_EX_ROYAL_RUMBLE_ENTER_NEXT_PLAYER:				// 다음 선수 입장 남은 시간
			{
				INDEX iLeftTime = 0;
				(*istr) >> iLeftTime;
				pUIManager->GetNotice()->ShowRoyalRumbleNextPlayerTime( iLeftTime );
			}
			break;
		case MSG_EX_ROYAL_RUMBLE_HOLD:							// 경기 시작 
			{
				;
			}
			break;
		case MSG_EX_ROYAL_RUMBLE_WINNER:						// 우승자 전체 공지
			{
				INDEX iLevelType = 0;
				INDEX iCharIdx = 0;
				CTString strNickName;
				(*istr) >> iLevelType;
				(*istr) >> iCharIdx;
				(*istr) >> strNickName;
				pUIManager->GetNotice()->ShowRoyalRumbleWinner( iLevelType, strNickName );
				if( _pNetwork->MyCharacterInfo.index == iCharIdx )
				{
					pUIManager->GetRadar()->ShowRoyalRumbleTrophy();
				}
			}
			break;
		case MSG_EX_ROYAL_RUMBLE_REWARD_REP:					// 보상 응답
			{
				UBYTE uResult = 0;
				(*istr) >> uResult;
				MsgBoxInfo.SetMsgBoxInfo( _s(""), UMBS_OK, UI_NONE, MSGCMD_NULL);
				if( uResult == 0 )
				{
					MsgBoxInfo.AddString( _S(5427, "보상 대상자가 아닙니다.") );
				}
				else if( uResult == 1 )
				{
					MsgBoxInfo.AddString( _S(1755, "이미 보상을 받았습니다.") );
				}
				pUIManager->CreateMessageBox(MsgBoxInfo);
			}
			break;
		case MSG_EX_ROYAL_RUMBLE_POINT_REP:						// 전장포인트 조회 응답.
			{
				INDEX iWarPoint = 0, iWarAccPoint = 0;
				(*istr) >> iWarPoint;
				(*istr) >> iWarAccPoint;
				MsgBoxInfo.SetMsgBoxInfo( _S(5412, "전장 포인트 조회"), UMBS_OK, UI_NONE, MSGCMD_NULL);
				CTString strTemp;
				MsgBoxInfo.AddString( _S(5428, "보유 전장 포인트"), 0xF2F2F2FF, TEXT_CENTER );
				MsgBoxInfo.AddString( _s(" ") );
				strTemp.PrintF( "%d", iWarPoint );
				MsgBoxInfo.AddString( strTemp, 0xF2F2F2FF, TEXT_CENTER );
				MsgBoxInfo.AddString( _s(" ") );
				MsgBoxInfo.AddString( _s(" ") );
				MsgBoxInfo.AddString( _S(5429, "누적 전장 포인트"), 0xF2F2F2FF, TEXT_CENTER );
				MsgBoxInfo.AddString( _s(" ") );
				strTemp.PrintF( "%d", iWarAccPoint );
				MsgBoxInfo.AddString( strTemp, 0xF2F2F2FF, TEXT_CENTER );
				MsgBoxInfo.AddString( _s(" ") );
				pUIManager->CreateMessageBox(MsgBoxInfo);
			}
			break;
		case MSG_EX_ROYAL_RUMBLE_TIME_REP:
			{
				INDEX iTime = -1;
				(*istr) >> iTime;
				MsgBoxInfo.SetMsgBoxInfo( _S(5430, "로얄럼블 경기 일정"), UMBS_OK, UI_NONE, MSGCMD_NULL );
				CTString strTemp;
				if( iTime < 0 )
				{
					strTemp.PrintF( _S(5431, "현재 경기가 진행중입니다.") );
				}
				else
				{
					strTemp.PrintF( _S(5432, "다음 경기는 %d시 00분에 진행됩니다."), iTime );
				}
				MsgBoxInfo.AddString( strTemp, 0xF2F2F2FF, TEXT_CENTER );
				pUIManager->CreateMessageBox( MsgBoxInfo );
			}
			break;
		case MSG_EX_ROYAL_RUMBLE_LEFT_COUNT:
			{
				INDEX iLeftCount = 0;
				(*istr) >> iLeftCount;
				pUIManager->GetRoyalRumbleIcon()->SetLeftCount( iLeftCount );
			}
			break;
		case MSG_EX_ROYAL_RUMBLE_TIME_NOTICE:
			{
				UBYTE uType = 0;
				INDEX iLeftTime = -1;
				(*istr) >> uType;
				(*istr) >> iLeftTime;

				pUIManager->GetRadar()->SetRoyalRumbleSandglass(uType, iLeftTime );
			}
			break;
		case MSG_EX_ROYAL_RUMBLE_NOTFOUND_ITEM:
			{	
				MsgBoxInfo.SetMsgBoxInfo( _S( 5576, "로얄럼블 입장권"), UMBS_OK, UI_NONE, MSGCMD_NULL );
				CTString strTemp;
				strTemp.PrintF( _S( 5577, "해당 로얄럼블 구간의 입장권이 존재하지 않습니다.") );
				MsgBoxInfo.AddString( strTemp, 0xF2F2F2FF, TEXT_CENTER );
				pUIManager->CreateMessageBox( MsgBoxInfo );
			}
			break;
		case MSG_EX_ROYAL_RUMBLE_KILL_PLAYER:
			{
				CTString strAttacker;
				CTString strVictim;
				CTString strTemp;
				(*istr) >> strAttacker >> strVictim;
				// ADD-Contents : ITS#3620 공격자 스스로 사망시 메시지 추가.  [8/22/2011 rumist]
				if( strAttacker == strVictim )
					strTemp.PrintF( _S( 5580, "%님이 아웃되었습니다."), strAttacker );
				else
					strTemp.PrintF( _S( 5578, "%s님이 %s님을 아웃시켰습니다."), strAttacker, strVictim );
				pUIManager->GetChattingUI()->AddSysMessage( strTemp, SYSMSG_ERROR );
			}
			break;
	}
}

void CSessionState::RecieveMasterStoneMessage( CNetworkMessage* istr )
{
	ResponseClient::doItemUseMasterStoneUSA* pPacket = reinterpret_cast<ResponseClient::doItemUseMasterStoneUSA*>(istr->GetBuffer());

	CUIManager* pUIManager = CUIManager::getSingleton();

	pUIManager->GetMixNew()->MixNewMasterStoneRep(pPacket->error, pPacket->success_type);
}

void CSessionState::RecieveRankingListExMessage( CNetworkMessage* istr )
{
	CUIManager::getSingleton()->GetRankingViewEx()->RecvRankingList(*istr);
}