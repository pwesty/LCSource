
#include "StdH.h"
#include <Engine/Network/CNetwork.h>
#include <Engine/Interface/UIManager.h>
#include "SessionState.h"
#include <Engine/Interface/UISelectWord.h>
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Interface/UIInventory.h>
#include <Engine/Interface/UIProcess.h>
#include <Engine/Interface/UIGamble.h>
#include <Engine/Contents/Base/UIChangeWeaponNew.h>
#include <Engine/Contents/Base/UIQuestNew.h>
#include <Engine/Interface/UISelectList.h>
#include <Engine/Interface/UICollectBox.h>
#include <Engine/Interface/UIOXQuizEvent.h>
#include <Engine/Interface/UIAutoHelp.h>
#include <Engine/Interface/UIBingoBox.h>
#include <Engine/Interface/UIMinigame.h>
#include <Engine/Interface/UIFlowerTree.h>
#include <Engine/Ska/Render.h>
#include <Engine/Entities/InternalClasses.h>
#include <Engine/GameDataManager/GameDataManager.h>
#include <Engine/Contents/Base/UINoticeNew.h>
#include <Engine/Contents/Base/Notice.h>
#include <Engine/Contents/Login/ServerSelect.h>
#include <Engine/Contents/Login/UIServerSelect.h>

// 보물 상자에서 지급하는 아이템 갯수
#define TREASUREBOX_ITEM_COUNT	3

int GuildMasterErrChk(SBYTE errType)
{
	CTString strMessage;
	BOOL nResult = FALSE;
	switch(errType)
	{
	case MSG_ERROR_PARENTSDAY_2007_FULLINVEN:
		strMessage =_S( 265, "인벤토리 공간이 부족합니다.");
		break;
	case MSG_ERROR_PARENTSDAY_2007_NO_EXCHAGNE_ITEM:
		strMessage =_S( 2236, "이벤트 아이템이 존재하지 않습니다.");
		break;		
	case MSG_ERROR_PARENTSDAY_2007_NOT_GUILD_OWNER:
		strMessage =_S(3470,"길드 마스터가 아닙니다.");
		break;
	case MSG_ERROR_ACCUMULATEPOINT_NOT_RANKING:
		strMessage =_S(3471, "랭킹 데이터가 없습니다.");
		break;

	case MSG_ERROR_ADD_POINT:					
		strMessage =_S(3472,"포인트를 적립하지 못했습니다.");
		break;
	case MSG_ERROR_NOT_ENOUGH_POINT:
		strMessage =_S(3473, "포인트가 부족 합니다.");
		break;
	case MSG_ERROR_NOT_FOUND_ACCUMULATEPOINT_LIST:
		strMessage =_S(3474, "누적 포인트 정보가 없습니다.");
		break;
	case MSG_ERROR_PARENTSDAY_2007_ERROR_NOITEM :
		strMessage =_S(3475,"교환권이 존재하지 않습니다.");
		break;
	case MSG_PARENTSDAY_2007_SUCCESS:
		return TRUE;
	default :
		nResult = TRUE;
		break;
	}
	if(strMessage.str_String!=NULL)
		_pNetwork->ClientSystemMessage(strMessage,SYSMSG_ERROR);

	return nResult;
}

int EasterEggs2007ErrChk(ULONG errType)
{
	CTString strMessage;
	BOOL nResult = FALSE;
	switch(errType)
	{
	case MSG_EVENT_EGGS_HUNT_2007_GIFT_REP_OK:
		nResult = TRUE;
		break;
	case MSG_EVENT_EGGS_HUNT_2007_ERROR_ALREADY_EXIST:
		strMessage =_S( 3265, "1개 이상의 부활절 달걀을 소유할 수 없습니다.");
		break;
	case MSG_EVENT_EGGS_HUNT_2007_ERROR_GIFT_FULLINVEN:
		strMessage =_S( 265, "인벤토리 공간이 부족합니다.");
		break;
	case MSG_EVENT_EGGS_HUNT_2007_ERROR_NOITEM:
		strMessage =_S( 3257, "요청한 작업이 실패 하였습니다.");
		break;
	}

	_pNetwork->ClientSystemMessage(strMessage);
	return nResult;

}

int Whiteday2007ErrChk(ULONG errType)
{
	CTString strMessage;
	BOOL nResult = FALSE;
	switch(errType)
	{
	case MSG_EVENT_WHITEDAY_2007_EXCHANGE_OK:
		//  성공시 메시지 출력이 필요 없을거 같음...
		//	strMessage =_s( "요청한 작업이 완료 되었습니다.");
		nResult = TRUE;
		break;
	case MSG_EVENT_WHITEDAY_2007_EXCHANGE_FAILED:
		strMessage =_S( 3257, "요청한 작업이 실패 하였습니다.");
		break;
	case MSG_EVENT_WHITEDAY_2007_EXCHANGE_NOITEM:
		strMessage =_S( 3258, "보상을 받기 위한 사탕의 개수가 부족합니다.");
		break;		
	case MSG_EVENT_WHITEDAY_2007_EXCHANGE_FULLINVEN:
		strMessage =_S( 265, "인벤토리 공간이 부족합니다.");
		break;
	case MSG_EVENT_WHITEDAY_2007_EXCHANGE_MAXITEM:
		strMessage =_S( 3260, "편지지는 최대 5개까지 소유할 수 있습니다.");
		break;
	}

	_pNetwork->ClientSystemMessage(strMessage);
	return nResult;

}

int WorldCupErrChk(ULONG errType) 		
{
	CTString strMessage;
	BOOL nResult = FALSE;
	switch(errType)
	{
	case MSG_EVENT_WORLDCUP_ERROR_VOTE_OK :			// 승리 국가 응모 : 성공
		strMessage =_S( 2835, "우승컵이 선택한 국가의 국기로 교환되었습니다.");
		nResult = TRUE;
		break;
	case MSG_EVENT_WORLDCUP_ERROR_VOTE_NOITEM :		// 승리 국가 응모 : FIFA 컵 부족
		strMessage =_S( 2836, "우승컵이 부족합니다.");
		break;
	case MSG_EVENT_WORLDCUP_ERROR_VOTE_NOTCHANGE :	// 승리 국가 응모 : 응모 기간 아님
		strMessage =_S( 2837, "우승컵 교환기간이 아닙니다.");
		break;
	case MSG_EVENT_WORLDCUP_ERROR_VOTE_FULLINVEN :	// 승리 국가 응모 : 인벤 공간 부족
		strMessage =_S( 2838, "승리 국가 응모 : 인벤 공간이 부족합니다.");
		break;
	case MSG_EVENT_WORLDCUP_ERROR_VOTE_ALREADY :		// 승리 국가 응모 : 이미 응모한 국가
		strMessage =_S( 2839, "승리 국가 응모 당첨 확인 : 이미 응모한 국가입니다.");
		break;
	case MSG_EVENT_WORLDCUP_ERROR_GIFT_OK :			// 승리 국가 응모 당첨 확인 : 지급 성공
		strMessage =_S( 2840, "우승국가 맞추기 이벤트 보상품이 모두 지급되었습니다. 참여해 주셔서 감사합니다.");
		break;
	case MSG_EVENT_WORLDCUP_ERROR_GIFT_NOTCHANGE :	// 승리 국가 응모 당첨 확인 : 교환 기간 아님
		strMessage =_S( 2841, "보상기간이 아닙니다. 월드컵이 종료된 이후에 확인하시기 바랍니다.");
		break;
	case MSG_EVENT_WORLDCUP_ERROR_GIFT_NOITEM :		// 승리 국가 응모 당첨 확인 : 국기 아이템 없음
		strMessage =_S( 2842, "해당 순위의 국기를 가지고 있지 않습니다. 다시 확인해 주세요");
		break;
	case MSG_EVENT_WORLDCUP_ERROR_GIFT_FULLINVEN :	// 승리 국가 응모 당첨 확인 : 인벤 공간 부족
		strMessage =_S( 2843, "승리 국가 응모 당첨 확인 : 인벤공간이 부족합니다.");
		break;

	}
	_pNetwork->ClientSystemMessage(strMessage);
	return nResult;
}

//wooss 060201 inner Function
void FindFriendErrChk1(ULONG errType) 		
{
	CTString strMessage;

	switch(errType)
	{
	case MSG_EVENT_SEARCHFRIEND_ERROR_OK :				 //정상.(친구등록OK, 삭제 OK)
		strMessage=_S( 2495, "친구등록에 성공하였습니다.");
		CUIManager::getSingleton()->GetSelectList()->ResetSelectList();
		break;
	case MSG_EVENT_SEARCHFRIEND_ERROR_PACKET :			 //패킷오류
		strMessage =_S(854,"패킷 오류가 발생하였습니다.");
		break;
	case MSG_EVENT_SEARCHFRIEND_ERROR_NOT_EXIST :			 //존재하지 않는다.
		strMessage =_S(11, "존재하지 않는 캐릭터 입니다.");
		break;
	case MSG_EVENT_SEARCHFRIEND_ERROR_FULLMEMBER :		 //친구 정원 초과.
		strMessage =_S( 2496, "등록인원이 초과 되었습니다.");
		break;
	case MSG_EVENT_SEARCHFRIEND_ERROR_ALREADY_EXIST :		 //이미 친구리스트에 존재.
		strMessage =_S( 2497, "이미 등록된 친구입니다.");
		break;
	case MSG_EVENT_SEARCHFRIEND_ERROR_NOT_MATCH_INDEX :	 //서로간  요청,승인 아이디가 일치하면 안됨.
		strMessage = _S( 2498, "같은 계정의 캐릭터는 등록할 수 없습니다.  ");
		break;
	case MSG_EVENT_SEARCHFRIEND_ERROR_HELPER_SERVER :		 //헬퍼서버와의 접속오류.
		strMessage = _S(2133,"헬퍼서버 오류 입니다.");
		break;
	case MSG_EVENT_SEARCHFRIEND_ERROR_GAMESERVER :		 //기간이 지났음.
		strMessage = _S( 2499, "한달 이상 게임에 접속을 안 한 캐릭터만 등록이 가능합니다.");
		break;
	case MSG_EVENT_SEARCHFRIEND_ERROR_FRIENDNAME :		 //이름이 잘못되었다.
		strMessage = _S( 2500, "잘못된 이름입니다.");
		break;							
	}
	_pNetwork->ClientSystemMessage(strMessage);
}

void FindFriendErrChk2(ULONG errType) 		
{
	CTString strMessage;

	switch(errType)
	{
	case MSG_EVENT_SEARCHFRIEND_GOODS_ERROR_OK :				// 지급 성공
		{
			CUIManager* pUIManager = CUIManager::getSingleton();

			pUIManager->CloseMessageBox( MSGCMD_NULL );
			CTString strSysMessage;
			strSysMessage = _S(1840,"아이템이 성공적으로 지급되었습니다.");
			CUIMsgBox_Info	MsgBoxInfo;
			MsgBoxInfo.SetMsgBoxInfo(   _S( 169, "이벤트" ), UMBS_OK, UI_NONE, MSGCMD_NULL);
			MsgBoxInfo.AddString( strSysMessage );
			pUIManager->CreateMessageBox( MsgBoxInfo );

			strMessage = CTString("");
			pUIManager->GetSelectList()->ResetSelectList();
		}
		break;
	case MSG_EVENT_SEARCHFRIEND_GOODS_ERROR_TIME :			// 지급 실패 : 시간 부족
		strMessage = _S( 2501, "시간이 부족합니다.");
		break;
	case MSG_EVENT_SEARCHFRIEND_GOODS_ERROR_END :				// 지급 실패 : 기간 아님
		strMessage = _S(945,"지금은 이벤트 기간이 아닙니다.");
		break;
	case MSG_EVENT_SEARCHFRIEND_GOODS_ERROR_SOMEREMAIN :		// 지급 성공, 일부 아이템 미지급
		strMessage = _S(2235,"이벤트 아이템을 일부 지급 받았습니다.");
		break;
	case MSG_EVENT_SEARCHFRIEND_GOODS_ERROR_NOGOODS :			// 지급 실패, 받을 아이템 없음
		strMessage = _S( 2502,"지급 받을 아이템이 없습니다." );
		break;
	case MSG_EVENT_SEARCHFRIEND_GOODS_ERROR_NOSPACE :			// 지급 실패 : 아이템 칸이 모자름.
		strMessage = _S(116,"인벤토리 공간이 부족하여 보상을 받을 수 없습니다.");
		break;
	}
	_pNetwork->ClientSystemMessage(strMessage);
}

void CSessionState::ReceiveEventMessage(CNetworkMessage *istr)
{
	UBYTE	ubType;
	(*istr) >> ubType;	

	CUIManager* pUIManager = CUIManager::getSingleton();

	switch(ubType)
	{
	case MSG_EVENT_2PAN4PAN_LETTER:
		{
			LONG lError;
			(*istr) >> lError;

			pUIManager->GetSelectWord()->ResultProcess( lError );

		}
		break;
	case MSG_EVENT_ERROR:
		{
			SBYTE	sbError;
			(*istr) >> sbError;

			_pNetwork->m_iLastError = sbError;
			CTString strMessage;
			switch(sbError)
			{
			case MSG_EVENT_ERROR_ALREADY_GIVE:
				strMessage.PrintF( _S( 673, "이벤트 아이템이 이미 지급되었습니다." ) );		
				_pNetwork->ClientSystemMessage( strMessage, SYSMSG_ERROR );
				break;
			case MSG_EVENT_ERROR_INSUFF_CONDITION:
				strMessage.PrintF( _S( 674, "이벤트 아이템을 받을 수 없습니다." ) );			
				_pNetwork->ClientSystemMessage( strMessage, SYSMSG_ERROR );
				break;
			case MSG_EVENT_ERROR_FULLINVENTORY:
				strMessage.PrintF( _S( 675, "인벤토리 공간이 부족해서 이벤트 아이템을 받을 수 없습니다." ) );		
				_pNetwork->ClientSystemMessage( strMessage, SYSMSG_ERROR );
				break;
			case MSG_EVENT_ERROR_WRONG_PACKET:
				strMessage.PrintF( _S( 1341, "잘못된 패킷입니다." ) );				
				_pNetwork->ClientSystemMessage( strMessage, SYSMSG_ERROR );
				break;
			case MSG_EVENT_ERROR_NO_MATCH:
				strMessage.PrintF( _S( 1342, "같은 레벨의 아이템이 없습니다." ) );		
				_pNetwork->ClientSystemMessage( strMessage, SYSMSG_ERROR );
				break;
			case MSG_EVENT_ERROR_CANT_OPTION:
				strMessage.PrintF( _S( 1343, "아이템 옵션을 옮길 수 없습니다." ) );	
				_pNetwork->ClientSystemMessage( strMessage, SYSMSG_ERROR );
				break;
			case MSG_EVENT_ERROR_CHUSEOK_NO_CROP:
				strMessage.PrintF( _S( 1879, "작물이 없습니다." ) );	
				_pNetwork->ClientSystemMessage( strMessage, SYSMSG_ERROR );
				break;

			case MSG_EVENT_ERROR_CHUSEOK_NO_SOIL:
#if defined (G_THAI)
				ULONG	tv_index;
				(*istr) >> tv_index;
				strMessage.PrintF( _S( 1880, "작물을 업그레이드 하기 위해서는 [%s] 10개가 필요합니다." ), 
					CItemData::getData(tv_index)->GetName());	
#else	// else about (G_THAI)
				strMessage.PrintF( _S( 1880, "작물을 업그레이드 하기 위해서는 '배양토'가 필요합니다." ) );	
#endif	// end (G_THAI)
				_pNetwork->ClientSystemMessage( strMessage, SYSMSG_ERROR );
				break;

			case MSG_EVENT_ERROR_CHUSEOK_ALREADY_UPGRADE:
				strMessage.PrintF( _S( 1881, "이미 작물의 업그레이드 최종단계인 열매단계까지 도달하셨습니다. 보상품으로 교환하십시오." ) );	
				_pNetwork->ClientSystemMessage( strMessage, SYSMSG_ERROR );
				break;
			case MSG_EVENT_ERROR_CHUSEOK_UPGRADE_OK:
				strMessage.PrintF( _S( 1882, "작물 업그레이드에 성공하였습니다." ) );	
				break;
			case MSG_EVENT_ERROR_CHUSEOK_UPGRADE_FAIL:
				strMessage.PrintF( _S( 1883, "작물 업그레이드에 실패하였습니다." ) );	
				_pNetwork->ClientSystemMessage( strMessage, SYSMSG_ERROR );
				break;
			case MSG_EVENT_ERROR_CHUSEOK_EXCHANGE_OK:
				strMessage.PrintF( _S( 1884, "추석 이벤트 보상품이 지급되었습니다. 이벤트에 참여해 주셔서 감사합니다." ) );	
				//	_pNetwork->ClientSystemMessage( strMessage );
				break;

				// 060118 wooss japan package item...
			case MSG_EVENT_ERROR_NOT_COUPON :					// 패키지 아이템 지급, 없는 시리얼
				strMessage.PrintF( _S( 2405, "요청한 쿠폰 번호가 존재하지 않습니다." ) );	
				break;

			case MSG_EVENT_ERROR_ALREADY_USE_COUNPON :		// 패키지 아이템 지급, 이미 지급 받은 쿠폰
				strMessage.PrintF( _S( 2406, "이미 지급받은 쿠폰 번호 입니다." ) );	
				break;

			case MSG_EVENT_ERROR_COUPON_SUCCESS :				// 패키지 아이템 지급, 지급 성공
				strMessage.PrintF( _S( 1840, "아이템이 성공적으로 지급 되었습니다" ) );	
				break;		
			case MSG_EVENT_ERROR_SAKURABUD_SUCCESS: // 060322 eons 일본 전통의상 교환 이벤트
				strMessage.PrintF( _S( 2545, "벚꽃 봉우리 교환 아이템이 성공적으로 지급 되었습니다." ) );
				break;
			case MSG_EVENT_ERROR_SAKURABUD_LEAKITEM: // 060322 eons
				strMessage.PrintF( _S( 2546, "벚꽃 봉우리가 부족합니다. 확이 후 다시 시도해 주세요." ) );
				break;

			case MSG_EVENT_ERROR_SEPTEMBER_NAS:
				{
					strMessage.PrintF( _S( 2073, "나스 당첨(사운드)" ) );	
					//_pNetwork->ClientSystemMessage( strMessage );

					CEntity* penEntity = CEntity::GetPlayerEntity(0);
					StartEffectGroup( "UpSkill", &penEntity->en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
				}
				return;
			case MSG_EVENT_ERROR_SEPTEMBER_DROP:
				{
					strMessage.PrintF( _S( 2074, "드롭율 당첨(사운드)" ) );	
					//	_pNetwork->ClientSystemMessage( strMessage );

					CEntity* penEntity = CEntity::GetPlayerEntity(0);
					StartEffectGroup( "UpSkill", &penEntity->en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
				}
				return;
			case MSG_EVENT_ERROR_SEPTEMBER_EXP:
				{	
					strMessage.PrintF( _S( 2075, "경험치 당첨(사운드)" ) );	
					//	_pNetwork->ClientSystemMessage( strMessage );

					CEntity* penEntity = CEntity::GetPlayerEntity(0);
					StartEffectGroup( "UpSkill", &penEntity->en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
				}
				return;
			case MSG_EVENT_ERROR_SEPTEMBER_SP:
				{
					strMessage.PrintF( _S( 2076, "숙련도 당첨(사운드)" ) );	
					//	_pNetwork->ClientSystemMessage( strMessage );

					CEntity* penEntity = CEntity::GetPlayerEntity(0);
					StartEffectGroup( "UpSkill", &penEntity->en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
				}
				return;
			case MSG_EVENT_AKAN_TEMPLE_ERROR_NO_ITEM:
				{
					strMessage.PrintF(_S(5099, "교환 가능한 아이템이 없습니다."));
				}
				break;
			case MSG_EVENT_AKAN_TEMPLE_ERROR_LACK_ITEM:
				{
					strMessage.PrintF(_S(5100, "교환 가능한 아이템이 부족합니다."));
				}
				break;

			case MSG_EVENT_ERROR_LOI_KRATHONG_NO_ITEM:
				{
					strMessage.PrintF(_S(5100, "교환 가능한 아이템이 부족합니다."));
				}
				break;

			case MSG_EVENT_ERROR_KB_NO_ITEM:	// [2011/11/14 : Sora] 태국 국왕 탄생일 이벤트
				{
					UCHAR	ucCommand;
					(*istr) >> ucCommand;
					if( ucCommand == 0 )	// 하트 부족함
						strMessage.PrintF( _S(5606, "하트가 없습니다. 확인 후 다시 시도하여 주십시오." ) );
					else					// ucCommand == 1 Father Flag 부족함
						strMessage.PrintF( _S(5607, "Father Flag 아이템이 없습니다. 확인 후 다시 시도하여 주십시오." ) );
				}
				break;
			}			

			CUIMsgBox_Info	MsgBoxInfo;	
			MsgBoxInfo.SetMsgBoxInfo( _S( 585, "이벤트" ), UMBS_OK, UI_NONE, MSGCMD_EVENT_PRIZE);		
			MsgBoxInfo.AddString( strMessage );
			pUIManager->CreateMessageBox( MsgBoxInfo );
		}
		break;
	case MSG_EVENT_PREOPEN_GIFT:
		{
			SLONG slIndex;
			(*istr) >> slIndex;

			pUIManager->CloseMessageBox(MSGCMD_EVENT_PRIZE);
			CTString		strMessage;
			CUIMsgBox_Info	MsgBoxInfo;	
			MsgBoxInfo.SetMsgBoxInfo( _S( 585, "이벤트" ), UMBS_OK, UI_NONE, MSGCMD_EVENT_PRIZE);		
			//CItemData& ID = _pNetwork->GetItemData(slIndex);
			strMessage.PrintF( _S( 676, "[%s] 이벤트 아이템을 얻었습니다." ), _pNetwork->GetItemName(slIndex) );	
			MsgBoxInfo.AddString( strMessage );
			pUIManager->CreateMessageBox( MsgBoxInfo );
		}
		break;
		// Date : 2005-01-28,   By Lee Ki-hwan
	case MSG_EVENT_LATTO:
		{
			unsigned char ucIndex;
			(*istr) >> ucIndex;

			CTString		strMessage;

			switch ( ucIndex )
			{
			case MSG_EVENT_LATTO_UPGRADE_SUCCESS: // "인첸트 역전 라또 아이템 +2에 당첨 되었습니다"
				strMessage = "인첸트 역전 라또 아이템 +2에 당첨 되었습니다";
				pUIManager->GetInventory()->Lock( FALSE, FALSE, LOCK_UPGRADE_ITEM  );
				break;
			case MSG_EVENT_LATTO_BLOOD_SUCCESS:	// "블러드 역전 라또 아이템 옵션에 당첨 되었습니다"
				strMessage = "블러드 역전 라또 아이템 옵션에 당첨 되었습니다";
				pUIManager->GetInventory()->Lock( FALSE, FALSE, LOCK_BLOOD_OPTION  );
				break;
			case MSG_EVENT_LATTO_PROCESS_SUCCESS:// "가공 역전 라또 아이템에 당첨 되었습니다"
				strMessage = "가공 역전 라또 아이템에 당첨 되었습니다";
				pUIManager->GetInventory()->Lock( FALSE, FALSE, LOCK_PROCESS  );
				// 가공창 닫기 
				pUIManager->GetProcess()->CloseProcess();
				break;	
			case MSG_EVENT_LATTO_EXPSP_SUCCESS:	// "레벨 역전 라또 (경험치 *%: 숙련도 *)에 당첨 되었습니다" : exp(n) sp(n)
				{
					SLONG Exp; //1013
					SLONG Sp;

					(*istr) >> Exp;
					(*istr) >> Sp;
					strMessage.PrintF ( "레벨 역전 라또 (경험치 %ld%%, 숙련도 %ld)에 당첨 되었습니다", Exp, Sp );
				}
				break;

			case MSG_EVENT_LATTO_PARTY_SUCCESS:	// "파티 역전 라또 아이템 복주머니에 당첨 되었습니다" : 시스템 창으로 msg 처리
				{
					strMessage = "파티 역전 라또 아이템 복주머니에 당첨 되었습니다";
					pUIManager->GetChattingUI()->AddSysMessage( strMessage );
					return;
				}
				break;


			case MSG_EVENT_LATTO_CHANGE_LUCKYBAG_REP : 	// 복주머니 교환 응답		: type (c)
				{
					UBYTE	ubType;
					(*istr) >> ubType;

					switch ( ubType )
					{
					case MSG_CHANGE_26LEVEL_GENERAL_STONE:			// 26레벨 일반 제련석
						strMessage = "26레벨 일반 제련석을 얻었습니다.";
						break;
					case MSG_CHANGE_SPECIAL_STONE:					// 고급제련석
						strMessage = "고급 제련석을 얻었습니다.";	
						break;
					case MSG_CHANGE_22LEVEL_EVENT_WEAPON:			// 22레벨 이벤트 무기 (종족별)
						strMessage = "22레벨 이벤트 무기을 얻었습니다.";
						break;
					case MSG_CHANGE_31LEVEL_ARMOR:					// 31레벨 방어구 상의 (종족별)
						strMessage = "31레벨 방어구 상의을 얻었습니다.";
						break;
					case MSG_CHANGE_ATTACK_UP:						// 공격력 향상제
						strMessage = "공격력 향상제를 얻었습니다.";
						break;
					case MSG_CHANGE_DEFENSE_UP:						// 방어력 향상제
						strMessage = "방어력 향상제를 얻었습니다.";
						break;
					case MSG_CHANGE_50000NAS:						// 5만 나스
						strMessage = "5만 나스를 얻었습니다.";
						break;
					case MSG_CHANGE_NO_LUCKYBAG:					// 교환할 복주머니가 없다
						strMessage = "교환할 복 주머니가 없습니다.";
						break;
					}

				}
				break;
			}

			pUIManager->CloseMessageBox(MSGCMD_EVENT_PRIZE);

			CUIMsgBox_Info	MsgBoxInfo;	
			MsgBoxInfo.SetMsgBoxInfo( _S( 585, "이벤트" ), UMBS_OK, UI_NONE, MSGCMD_EVENT_PRIZE);		
			MsgBoxInfo.AddString( strMessage );
			pUIManager->CreateMessageBox( MsgBoxInfo );

		}
		break;
	case MSG_EVENT_CHANGEWEAPON:		// 무기 교체 이벤트
		{
			LONG lOldIndex;
			LONG lNewIndex;

			(*istr) >> lOldIndex;
			(*istr) >> lNewIndex;

			CTString strOldItem = _pNetwork->GetItemName(lOldIndex);
			CTString strNewItem = _pNetwork->GetItemName(lNewIndex);

			CTString strMessage;
			strMessage.PrintF( _S( 1344, "[%s] 무기가 [%s] 무기로 교체되었습니다." ), strOldItem, strNewItem );		
			pUIManager->GetChattingUI()->AddSysMessage( strMessage );			
		}
		break;
	case MSG_EVENT_MOONSTONE:				// 문스톤 이벤트...
		{
			unsigned char ucIndex;
			(*istr) >> ucIndex;

			switch ( ucIndex )
			{
			case MSG_EVENT_MOONSTONE_START_REP:
				{
					SBYTE sbType;
					LONG lAccumulate;
					(*istr) >> sbType;
					(*istr) >> lAccumulate;
					pUIManager->GetGamble()->SetAccumulate( lAccumulate );
					pUIManager->GetGamble()->ShowGamble();
					pUIManager->SetCSFlagOn( CSF_MOONSTONE );
				}
				break;

			case MSG_EVENT_MOONSTONE_TRY_REP:		// 문스톤 복권 응답
				{
					unsigned char ucSubType;
					(*istr) >> ucSubType;
					switch( ucSubType )
					{
					case MSG_CHANGE_NO_MOONSTONE:
						{
							CTString strMessage;
							strMessage.PrintF( _S( 1439, "문스톤이 없습니다." ) );		
							_pNetwork->ClientSystemMessage( strMessage, SYSMSG_ERROR );
							pUIManager->GetGamble()->ResetGamble();
						}
						break;					
					case MSG_CHANGE_MOONSTONE_RULLET:
						{
							LONG lSelectedMarker;
							LONG lSelectedCount;
							LONG lAccumulate;
							(*istr) >> lSelectedMarker;
							(*istr) >> lSelectedCount;
							(*istr) >> lAccumulate;

							pUIManager->GetGamble()->SetSelectedMarker( lSelectedMarker, lSelectedCount );
							pUIManager->GetGamble()->Start();
							pUIManager->GetGamble()->SetAccumulate( lAccumulate );
							//pUIManager->SetCSFlagOn( CSF_MOONSTONE );
							//pUIManager->GetGamble()->Stop();
						}
						break;
					}
				}
				break;
			case MSG_EVENT_MOONSTONE_RESULT_REP:	// 문스톤 복권 결과 응답.ㄴ
				{
					SBYTE	sbPrizeType;					
					(*istr) >> sbPrizeType;
					pUIManager->GetGamble()->Stop();
					switch( sbPrizeType )
					{										
					case MSG_CHANGE_150000_EXP:					    // 경험치 150,000
						{
							CTString strMessage;
							strMessage.PrintF( _S( 1442, "경험치 150,000을 얻었습니다." ) );		
							//	_pNetwork->ClientSystemMessage( strMessage );
							pUIManager->GetGamble()->ShowPrizeEffect();
						}
						break;
					case MSG_CHANGE_70000_EXP:					    // 경험치 70,000	
						{
							CTString strMessage;
							strMessage.PrintF(_S( 1443, "경험치 70,000을 얻었습니다." ) );
							//	_pNetwork->ClientSystemMessage( strMessage );
							pUIManager->GetGamble()->ShowPrizeEffect();
						}
						break;
					case MSG_CHANGE_25000_EXP:					    // 경험치 25,000							
						{
							CTString strMessage;
							strMessage.PrintF(_S( 1444, "경험치 25,000을 얻었습니다." ));		
							//	_pNetwork->ClientSystemMessage( strMessage );
							pUIManager->GetGamble()->ShowPrizeEffect();
						}
						break;

					case MSG_CHANGE_JACK_POT:						// 잭팟.
					case MSG_CHANGE_5PLUS_41LEVEL_WEAPON:			// 41레벨 +5 무기(종족별)	
					case MSG_CHANGE_3PLUS_41LEVEL_WEAPON:			// 41레벨 +3 무기(종족별)	
					case MSG_CHANGE_41LEVEL_WEAPON:					// 41레벨 무기 (종족별)
					case MSG_CHANGE_3_SPECIAL_STONE:				// 고급제련석 3개
					case MSG_CHANGE_2_SPECIAL_STONE:				// 고급제련석 2개
					case MSG_CHANGE_1_SPECIAL_STONE:				// 고급제련석 1개						
					case MSG_CHANGE_CRYSTAL_OF_EFFORT:				// 노력의 결정.
					case MSG_CHANGE_HIGH_CRYSTAL_OF_EXPERIMENCE:	// 상급 경험의 결정.					
					case MSG_CHANGE_MIDDLE_CRYSTAL_OF_EXPERIMENCE:	// 중급 경험의 결정
					case MSG_CHANGE_10_PURE_STONE:
					case MSG_CHANGE_3_MIDDLE_CRYSTAL_OF_EXPERIMENCE:
					case MSG_CHANGE_5_12GRADE_RED_KER:				//12레벨 붉은 케르 5개
					case MSG_CHANGE_5_12GRADE_RED_NEN:				//12레벨 붉은 넨 5개
					case MSG_CHANGE_5_12GRADE_BLUE_KER:				//12레벨 푸른 케르 5개
					case MSG_CHANGE_5_BIG_MP_POTION:				//대형 MP물약 5개
					case MSG_CHANGE_5_12GRADE_BLUE_NEN:				//12레벨 푸른 넨 5개
					case MSG_CHANGE_5_MOONSTONE:					//문스톤 5개
					case MSG_CHANGE_3_6GRADE_RED_KER:				//6레벨 붉은 케르 3개 
					case MSG_CHANGE_3_6GRADE_RED_NEN:				//6레벨 붉은 넨 3개
					case MSG_CHANGE_3_6GRADE_BLUE_KER:				//6레벨 푸른 케르 3개
					case MSG_CHANGE_3_BIG_HEAL_POTION:				//대형 HP물약 3개
					case MSG_CHANGE_3_6GRADE_BLUE_NEN:				//6레벨 푸른넨 3개
					case MSG_CHANGE_3_MIDDLE_MP_POTION:				//중형 MP물약 3개
					case MSG_CHANGE_3_MEMORY_SCROLL:				//메모리 스크롤 3개
					case MSG_CHANGE_3_C_GRADE_ELEMENTAL:			//c등급 원소 3개
					case MSG_CHANGE_3_CRAK_BLUE_LEAF:			 	//크락 파란잎 3개
					case MSG_CHANGE_3_STONE_WEAVER:					//스톤 웨버 3개
					case MSG_CHANGE_3_D_GRADE_ELEMENTAL:			//D등급 원소 3개
					case MSG_CHANGE_3_MIDIUM_HEAL_POTION:			//중형회복물약 5개
					case MSG_CHANGE_3_CRAK_BRANCH:					//크락 줄기 3개
					case MSG_CHANGE_3_STONELISE:					//스톤 리스
						{
							pUIManager->GetGamble()->ShowPrizeEffect();
							//CTString strMessage;
							//strMessage.PrintF( _S( 1452, "당첨 되었습니다!!!" ));		
							//_pNetwork->ClientSystemMessage( strMessage );							
						}
						break;
					}					
				}
				break;
			}
		}
		break;

	case MSG_EVENT_TREASUREBOX:
		{
			unsigned char ucSubType;
			(*istr) >> ucSubType;
			switch( ucSubType )
			{
			case MSG_EVENT_TREASUREBOX_TRY_REP:
				{
					int iTreasureBoxLevel	= -1;
					CTString strMessage;
					LONG lTreasureBoxType;
					(*istr) >> lTreasureBoxType;

					switch( lTreasureBoxType )
					{
					case MSG_EVENT_TREASUREBOX_ERROR_NOT_LEVEL:
						{
							strMessage = _S( 1757, "레벨이 부족하여 보물 상자를 열수 없습니다." );		
							_pNetwork->ClientSystemMessage( strMessage, SYSMSG_ERROR );
							return;
						}
						break;
					case MSG_EVENT_TREASUREBOX_ERROR_NO_BOX:
						{
							strMessage = _S( 1758, "보물 상자가 없습니다." );		
							_pNetwork->ClientSystemMessage( strMessage, SYSMSG_ERROR );
							return;
						}
						break;
					case MSG_EVENT_TREASUREBOX_10LEVEL:
						{
							iTreasureBoxLevel	= 10;
						}
						break;
					case MSG_EVENT_TREASUREBOX_14LEVEL:
						{
							iTreasureBoxLevel	= 14;
						}
						break;
					case MSG_EVENT_TREASUREBOX_18LEVEL:
						{
							iTreasureBoxLevel	= 18;
						}
						break;
					case MSG_EVENT_TREASUREBOX_22LEVEL:
						{
							iTreasureBoxLevel	= 22;
						}
						break;
					case MSG_EVENT_TREASUREBOX_26LEVEL:
						{
							iTreasureBoxLevel	= 26;
						}
						break;
					case MSG_EVENT_TREASUREBOX_30LEVEL:
						{
							iTreasureBoxLevel	= 30;
						}
						break;
					}

					ASSERT( iTreasureBoxLevel != -1 && "Invalid Treasure Box Level!" );

					CTString	strItemName[TREASUREBOX_ITEM_COUNT+1];
					LONG		lItemCount[TREASUREBOX_ITEM_COUNT+1];

					int nTreasureBoxCount = TREASUREBOX_ITEM_COUNT;
					int nGiftCount = 1;

					if (pUIManager->m_JapanTempEvent == 1)
					{
						nTreasureBoxCount = TREASUREBOX_ITEM_COUNT+1;
						nGiftCount = 2;
					}

					int		i;
					for( i = 0; i < nTreasureBoxCount ; ++i )
					{
						SBYTE sbTreasureItem;
						(*istr) >> sbTreasureItem;
						(*istr) >> lItemCount[i];

						switch( sbTreasureItem )
						{
						case MSG_EVENT_CHANGE_13LEVEL_WEAPON:		//+4 13레벨 무기
							strItemName[i] = _S( 1759, "+4 13레벨 무기" );	
							break;
						case MSG_EVENT_CHANGE_21LEVEL_WEAPON:		//+4 21레벨 무기
							strItemName[i] = _S( 1760, "+4 21레벨 무기" );	
							break;
						case MSG_EVENT_CHANGE_29LEVEL_WEAPON:		//+4 29레벨 무기
							strItemName[i] = _S( 1761, "+4 29레벨 무기" );	
							break;
						case MSG_EVENT_CHANGE_NAS:					//나스
							strItemName[i] =_S( 1762,  "나스" );	
							break;
						case MSG_EVENT_CHANGE_CANDY:				//사탕
							strItemName[i] = _S( 1763, "사탕" );	
							break;
						case MSG_EVENT_CHANGE_ATTACK_POTION:		//공격력향상물약
							strItemName[i] = _S( 1764, "공격력 향상 물약" );	
							break;
						case MSG_EVENT_CHANGE_DEFENSE_POTION:		//방어력향상물약
							strItemName[i] = _S( 1765, "방어력 향상 물약" );	
							break;
						case MSG_EVENT_CHANGE_HP_POTION:   // 소형  HP 확장 포션
							strItemName[i] = _pNetwork->GetItemName(1289);
							break;
						case MSG_EVENT_CHANGE_MP_POTION:   // 소형  MP 확장 포션
							strItemName[i] = _pNetwork->GetItemName(1292);
							break;
						case MSG_EVENT_CHANGE_EX_MEMORY:   // 메모리 스크롤 확장 카드
							strItemName[i] = _pNetwork->GetItemName(840);
							break;
						case MSG_EVENT_CHANGE_HPMP_POTION:  // 소형  HP/MP 확장 포션
							strItemName[i] = _pNetwork->GetItemName(1295);
							break;
						case MSG_EVEVNT_CHAGE_EX_STAT:  // 개인 창고 확장 카드
							strItemName[i] = _pNetwork->GetItemName(841);
							break;
						case MSG_EVENT_CHANGE_PINK_DRAGON:		//핑크 드레이크 알
							strItemName[i] = _pNetwork->GetItemName(1706);
							break;
						case MSG_EVENT_CHANGE_BLUE_HORSE:		//파란 판의 피리
							strItemName[i] = _pNetwork->GetItemName(1707);
							break;
						case MSG_EVENT_CHANGE_NO_TREASUREBOX:		//보물상자가 없음
							break;
						}
					}
					pUIManager->CloseMessageBoxL( MSGLCMD_TREASUREBOX_EVENT );

					pUIManager->CreateMessageBoxL( _S( 1766, "보물 상자" ), UI_QUEST, MSGLCMD_TREASUREBOX_EVENT );			

					pUIManager->AddMessageBoxLString( MSGLCMD_TREASUREBOX_EVENT, TRUE, _S( 1766, "보물 상자" ), -1, 0xE18600FF );	
					strMessage.PrintF( _S( 1767, "%d Lv 보물상자를 열면 아래 아이템 항목중 %d가지가 지급됩니다.  %d Lv 보물상자를 열어보시겠습니까?" ), iTreasureBoxLevel, nGiftCount, iTreasureBoxLevel );	
					pUIManager->AddMessageBoxLString( MSGLCMD_TREASUREBOX_EVENT, TRUE, strMessage, -1, 0xA3A1A3FF );
					pUIManager->AddMessageBoxLString( MSGLCMD_TREASUREBOX_EVENT, TRUE, CTString("  "), -1, 0xA3A1A3FF );
					pUIManager->AddMessageBoxLString( MSGLCMD_TREASUREBOX_EVENT, TRUE, _S( 1768, "아이템 지급 품목" ), -1, 0xE18600FF );			

					for( i = 0; i < nTreasureBoxCount; ++i )
					{
						strMessage.PrintF( _S( 61, "%s %d개" ), strItemName[i], lItemCount[i] );
						pUIManager->AddMessageBoxLString( MSGLCMD_TREASUREBOX_EVENT, TRUE, strMessage, -1, 0xA3A1A3FF );
					}

					pUIManager->AddMessageBoxLString( MSGLCMD_TREASUREBOX_EVENT, FALSE, _S( 1769, "보물상자를 연다." ), 1 );
					pUIManager->AddMessageBoxLString( MSGLCMD_TREASUREBOX_EVENT, FALSE, _S( 1220, "취소한다." ) );	
				}
				break;
			case MSG_EVENT_TREASUREBOX_OPEN_REP:
				{
					LONG lTreasureItem;
					(*istr) >> lTreasureItem;
				}
				break;
			}
		}
		break;
	case MSG_EVENT_SUPERSTONE:
		{
			unsigned char ucSubType;
			(*istr) >> ucSubType;
			switch(ucSubType)
			{
			case MSG_EVENT_SUPERSTONE_REP:
				{
				} break;
			}
		} break;

	case MSG_EVENT_CHANGE_WITHOUT_OPTION:
		{			
			// MSG_EVENT_CHANGE_WITHOUT_OPTION,			// 2판4판 리뉴얼 무기 교체 이벤트 : old_itemindex(n) new_subtype(n)	
			LONG lOldItemIndex;
			LONG lNewSubType;
			(*istr) >> lOldItemIndex;
			(*istr) >> lNewSubType;
			pUIManager->GetChangeWeapon()->close();
		}
		break;

	case MSG_EVENT_2PAN4PAN_GOODS_CHECK:
		{
			ULONG count;
			CTString strMessage;
			(*istr) >> count;
			if(count>0){
				strMessage.PrintF( _S(2232, "보상품을 요청 중입니다." ));
				_pNetwork->ClientSystemMessage( strMessage, SYSMSG_ERROR );				
				_pNetwork->SendEvent24(0,MSG_EVENT_2PAN4PAN_GOODS_REQ);

			} else {				
				strMessage.PrintF( _S(2233, "요청한 보상품이 존재하지 않습니다" ));
				_pNetwork->ClientSystemMessage( strMessage, SYSMSG_ERROR );
			}

		}
		break;

	case MSG_EVENT_2PAN4PAN_GOODS_REP:
		{
			ULONG errType,count;
			CTString strMessage;
			(*istr) >> errType;
			(*istr) >> count;

			switch(errType)
			{
			case MSG_EVENT_2PAN4PAN_GOODS_ERROR_OK :
				strMessage.PrintF( _S(2234, "이벤트 아이템을 지급 받았습니다." ));
				_pNetwork->ClientSystemMessage( strMessage, SYSMSG_ERROR );	
				break;
			case MSG_EVENT_2PAN4PAN_GOODS_ERROR_SOMEREMAIN:
				strMessage.PrintF( _S(2235, "이벤트 아이템을 일부 지급 받았습니다." ));
				_pNetwork->ClientSystemMessage( strMessage, SYSMSG_ERROR );	
				break;
			case MSG_EVENT_2PAN4PAN_GOODS_ERROR_NOGOODS:
				strMessage.PrintF( _S(2236, "이벤트 아이템이 존재하지 않습니다." ));
				_pNetwork->ClientSystemMessage( strMessage, SYSMSG_ERROR );	
				break;
			case MSG_EVENT_2PAN4PAN_GOODS_ERROR_NOSPACE:
				strMessage.PrintF( _S(2237, "지급받을 공간이 부족합니다." ));
				_pNetwork->ClientSystemMessage( strMessage, SYSMSG_ERROR );	
				break;
			}
		}
		break;
	case MSG_EVENT_2PAN4PAN_MONEY_REP:
		{
			ULONG success;
			CTString strMessage;
			(*istr) >> success;

			if(success==1 ){ //1 맞고 
				strMessage.PrintF( _S(2238, "맞고머니를 지급 받았습니다." ));
				_pNetwork->ClientSystemMessage( strMessage, SYSMSG_ERROR );				

			} 
			else if(success==2 ){ //2 포커 
				strMessage.PrintF( _S(2239, "포커머니를 지급 받았습니다." ));
				_pNetwork->ClientSystemMessage( strMessage, SYSMSG_ERROR );				

			} 

			else {				
				strMessage.PrintF( _S(2240, "이벤트 아이템이 존재하지 않습니다." ));
				_pNetwork->ClientSystemMessage( strMessage, SYSMSG_ERROR );
			}

		}
		break;
	case MSG_EVENT_2PAN4PAN_BOX_CHECK:
		{
			ULONG flag;
			CTString strMessage;
			(*istr) >> flag;
			if(flag>0){
				pUIManager->CloseMessageBoxL( MSGLCMD_EVENT_2PAN4PAN_TWO );
				pUIManager->CreateMessageBoxL( _S( 1875, "이벤트 선택창" ),UI_QUEST, MSGLCMD_EVENT_2PAN4PAN_TWO);						
				pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_2PAN4PAN_TWO, TRUE, 
					_S(2241,"2pan4pan 보물상자를 열어보면 아래의 3가지 물품 중 한가지가 지급됩니다."), -1, 0xA3A1A3FF );
				pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_2PAN4PAN_TWO, TRUE, CTString("\n\n"), -1,0xA3A1A3FF );		
				pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_2PAN4PAN_TWO, TRUE, _S(2242, "지급 물품" ), -1, 0xFFA41CFF );		
				pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_2PAN4PAN_TWO, TRUE, _S(2243, "부스터 5개" ), -1, 0xA3A1A3FF );
				pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_2PAN4PAN_TWO, TRUE, _S(2244, "문스톤 5개" ), -1, 0xA3A1A3FF );
				pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_2PAN4PAN_TWO, TRUE, _S(2245, "2pan4pan 물약 10개" ), -1, 0xA3A1A3FF );

				pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_2PAN4PAN_TWO, FALSE, _S(2246,"2pan4pan 보물상자 열기"),OPEN_TREA);
				pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_2PAN4PAN_TWO, FALSE,_S( 1220, "취소한다." ));

			} else {				
				strMessage.PrintF( _S(2247, "보물상자가 존재하지 않습니다" ));
				_pNetwork->ClientSystemMessage( strMessage, SYSMSG_ERROR );
			}

		}
		break;
	case MSG_EVENT_2PAN4PAN_BOX_REP:
		{
			ULONG errType,count;
			CTString strMessage;
			(*istr) >> errType;
			(*istr) >> count;

			switch(errType)
			{
			case MSG_EVENT_2PAN4PAN_BOX_ERROR_OK :
				strMessage.PrintF( _S(2234, "이벤트 아이템을 지급 받았습니다." ));
				_pNetwork->ClientSystemMessage( strMessage, SYSMSG_ERROR );	
				break;
			case MSG_EVENT_2PAN4PAN_BOX_ERROR_SOMEREMAIN:
				strMessage.PrintF( _S(2235, "이벤트 아이템을 일부 지급 받았습니다." ));
				_pNetwork->ClientSystemMessage( strMessage, SYSMSG_ERROR );	
				break;
			case MSG_EVENT_2PAN4PAN_BOX_ERROR_NOBOX:
				strMessage.PrintF( _S(2236, "이벤트 아이템이 존재하지 않습니다." ));
				_pNetwork->ClientSystemMessage( strMessage, SYSMSG_ERROR );	
				break;
			case MSG_EVENT_2PAN4PAN_BOX_ERROR_NOSPACE:
				strMessage.PrintF( _S(2237, "지급받을 공간이 부족합니다." ));
				_pNetwork->ClientSystemMessage( strMessage, SYSMSG_ERROR );	
				break;
			}

		}
		break;
	case MSG_EVENT_XMAS_2005 :
		LONG errType1;
		(*istr)>>errType1;
		if(errType1==MSG_EVENT_XMAS_2005_ERROR_OK){
			pUIManager->CloseMessageBox( MSGCMD_NULL );
			CTString strSysMessage = _S(2282,"축하합니다. 2005 크리스마스 이벤트 보상품을 획득 하셨습니다.") ; // 번역 
			CUIMsgBox_Info	MsgBoxInfo;
			MsgBoxInfo.SetMsgBoxInfo(   _S( 191, "확인" ), UMBS_OK, UI_NONE, MSGCMD_NULL);
			MsgBoxInfo.AddString( strSysMessage );
			pUIManager->CreateMessageBox( MsgBoxInfo );
		}
		else if(errType1==MSG_EVENT_XMAS_2005_ERROR_NO_ITEM){
			pUIManager->CloseMessageBox( MSGCMD_NULL );
			CTString strSysMessage = _S(2283,"조합할 꿈과 희망 아이템이 부족합니다.") ; //번역 
			CUIMsgBox_Info	MsgBoxInfo;
			MsgBoxInfo.SetMsgBoxInfo(   _S( 191, "확인" ), UMBS_OK, UI_NONE, MSGCMD_NULL);
			MsgBoxInfo.AddString( strSysMessage );
			pUIManager->CreateMessageBox( MsgBoxInfo );

		}
		break;

	case MSG_EVENT_NEWYEAR_2006_CHECK:
		{
			LONG tv_time,hour,min;
			(*istr)>>tv_time;
			hour=tv_time/3600;
			min=(tv_time%3600)/60;


			pUIManager->CloseMessageBox( MSGCMD_NULL );
			CTString strSysMessage;
			strSysMessage.PrintF(_S(2304,"현재 접속 시간은 %d시간 %d분 입니다."),hour,min) ; 
			CUIMsgBox_Info	MsgBoxInfo;
			MsgBoxInfo.SetMsgBoxInfo(   _S( 2287, "2006 인내의 신년 이벤트" ), UMBS_OK, UI_NONE, MSGCMD_NULL);
			MsgBoxInfo.AddString( strSysMessage );
			pUIManager->CreateMessageBox( MsgBoxInfo );
		}		
		break;


	case MSG_EVENT_NEWYEAR_2006_GOODS:
		LONG errType2;
		(*istr)>>errType2;
		if(errType2==MSG_EVENT_NEWYEAR_2006_GOODS_ERROR_OK){
			pUIManager->CloseMessageBox( MSGCMD_NULL );
			CTString strSysMessage = _S(2284,"축하합니다. 2006 신년 이벤트 보상품을 획득 하셨습니다.") ;
			CUIMsgBox_Info	MsgBoxInfo;
			MsgBoxInfo.SetMsgBoxInfo(   _S( 191, "확인" ), UMBS_OK, UI_NONE, MSGCMD_NULL);
			MsgBoxInfo.AddString( strSysMessage );
			pUIManager->CreateMessageBox( MsgBoxInfo );

		}
		else if(errType2==MSG_EVENT_NEWYEAR_2006_GOODS_ERROR_TIME){
			pUIManager->CloseMessageBox( MSGCMD_NULL );
			CTString strSysMessage = _S(2285,"이벤트 시간이 부족 합니다.(10시간 이상)") ;
			CUIMsgBox_Info	MsgBoxInfo;
			MsgBoxInfo.SetMsgBoxInfo(   _S( 191, "확인" ), UMBS_OK, UI_NONE, MSGCMD_NULL);
			MsgBoxInfo.AddString( strSysMessage );
			pUIManager->CreateMessageBox( MsgBoxInfo );


		}
		else if(errType2==MSG_EVENT_NEWYEAR_2006_GOODS_ERROR_END){
			pUIManager->CloseMessageBox( MSGCMD_NULL );
			CTString strSysMessage = _S(2285,"인내의 열매 이벤트 보상품은 2006 신년 이벤트가 종료 되어야 지급 받으실 수 있습니다.") ;
			CUIMsgBox_Info	MsgBoxInfo;
			MsgBoxInfo.SetMsgBoxInfo(   _S( 191, "확인" ), UMBS_OK, UI_NONE, MSGCMD_NULL);
			MsgBoxInfo.AddString( strSysMessage );
			pUIManager->CreateMessageBox( MsgBoxInfo );

		}
		else if(errType2==MSG_EVENT_NEWYEAR_2006_GOODS_ERROR_ALREADY){
			pUIManager->CloseMessageBox( MSGCMD_NULL );
			CTString strSysMessage = _S(673,"이벤트 아이템이 이미 지급 되었습니다.") ;
			CUIMsgBox_Info	MsgBoxInfo;
			MsgBoxInfo.SetMsgBoxInfo(   _S( 191, "확인" ), UMBS_OK, UI_NONE, MSGCMD_NULL);
			MsgBoxInfo.AddString( strSysMessage );
			pUIManager->CreateMessageBox( MsgBoxInfo );

		}

		break;

	case MSG_EVENT_SEARCHFRIEND_ADD_CHECK :
	case MSG_EVENT_SEARCHFRIEND_ADDSELECT_CHECK :
		{
			ULONG errType;
			(*istr) >> errType;
			FindFriendErrChk1(errType);
		}
		break;

	case MSG_EVENT_SEARCHFRIEND_ADDSELECT_LIST :
		{
			ULONG listCnt,startIdx,listAll;
			SEL_LIST tv_char;
			LONG tv_tmp;

			(*istr) >> listCnt;
			(*istr) >> startIdx;
			(*istr) >> listAll;

			if( startIdx==0) 
				pUIManager->GetSelectList()->ResetSelectList();

			tv_tmp = startIdx+listCnt;
			for( int i=startIdx; i<tv_tmp; i++){
				(*istr) >> tv_char.m_nIndex;
				(*istr) >> tv_char.m_sName;

				tv_char.m_nState = LS_WAIT ;

				pUIManager->GetSelectList()->GetList()->push_back(tv_char);
			}
			if(tv_tmp>=listAll)
				pUIManager->GetSelectList()->CreateSelectList(SLS_REGIST|SLS_CANCEL|SLS_PREV|SLS_NEXT ,10, _S( 2503, "나를 찾기 원하는 친구"));
		}		
		break;
	case MSG_EVENT_SEARCHFRIEND_TIME_CHECK :
		{
			CTString strMessage;
			ULONG tv_time;
			(*istr) >> tv_time;
			pUIManager->CloseMessageBox( MSGCMD_NULL );
			strMessage.PrintF(_S( 2504, "현재 %d시간 %d분 사냥하였습니다."),tv_time/3600,(tv_time%3600)/60 );
			CUIMsgBox_Info	MsgBoxInfo;
			MsgBoxInfo.SetMsgBoxInfo(   _S( 2433, "친구 찾기 이벤트" ), UMBS_OK, UI_NONE, MSGCMD_NULL);
			MsgBoxInfo.AddString( strMessage );
			pUIManager->CreateMessageBox( MsgBoxInfo );
		}
		break;
	case MSG_EVENT_SEARCHFRIEND_ONETIME_CHECK :
		{
			CTString strMessage;
			ULONG tv_time;
			int tv_sum;
			(*istr) >> tv_time;
			tv_sum = tv_time/3600;
			strMessage.PrintF(_S( 2505, "현재 %d시간 사냥하였습니다."),tv_sum);
			_pNetwork->ClientSystemMessage(strMessage);
			if(tv_sum == 30)  
			{
				strMessage.PrintF(_S( 2506, "이제 랜디에게서 보상품을 지급 받을 수 있습니다."));
				_pNetwork->ClientSystemMessage(strMessage);
			}
			if(tv_sum == 60)  
			{
				strMessage.PrintF(_S( 2507, "경험치 두배 이벤트가 종료 되었습니다."));
				_pNetwork->ClientSystemMessage(strMessage);
			}
		}
		break;
	case MSG_EVENT_SEARCHFRIEND_LIST_GOODS :
		{
			ULONG listCnt,startIdx,listAll;
			SEL_LIST tv_char;
			LONG tv_tmp,listState;

			(*istr) >> listCnt;
			(*istr) >> startIdx;
			(*istr) >> listAll;

			if( startIdx==0) 
				pUIManager->GetSelectList()->ResetSelectList();

			if(listAll <= 0){
				pUIManager->CloseMessageBox( MSGCMD_NULL );
				CTString strSysMessage = _S( 2493, "등록된 친구가 없습니다.") ;
				CUIMsgBox_Info	MsgBoxInfo;
				MsgBoxInfo.SetMsgBoxInfo(   _S( 2433, "친구 찾기 이벤트" ), UMBS_OK, UI_NONE, MSGCMD_NULL);
				MsgBoxInfo.AddString( strSysMessage );
				pUIManager->CreateMessageBox( MsgBoxInfo );
				break;
			}
			tv_tmp = startIdx+listCnt;
			for( int i=startIdx; i<tv_tmp; i++){
				(*istr) >> tv_char.m_nIndex;
				(*istr) >> tv_char.m_sName;
				//	(*istr) >> requestTime;
				(*istr) >> listState;
				if(listState < 0) tv_char.m_nState = LS_WAIT;
				else if(listState == 0) tv_char.m_nState = LS_REWARDX;
				else if(listState == 3) tv_char.m_nState = LS_RECEIVED;
				else if(listState > 0 ) tv_char.m_nState = LS_REWARD;

				pUIManager->GetSelectList()->GetList()->push_back(tv_char);
			}
			if(tv_tmp == listAll)
				pUIManager->GetSelectList()->CreateSelectList(SLS_REWARD|SLS_PREV|SLS_NEXT|SLS_CANCEL,10,_S( 2503, "내가 찾기 원하는 친구"));
		}
		break;
	case MSG_EVENT_SEARCHFRIEND_GOODS_CHECK :
		{
			ULONG errType;
			(*istr) >> errType;
			FindFriendErrChk2(errType);
		}
		break;
	case MSG_EVENT_OXQUIZ:
		{
			ULONG QuizType;
			ULONG QuizNo;
			ULONG Limitsec;
			CTString strQuizMessage;
			UBYTE isAnswer;

			(*istr) >> QuizType;

			if( QuizType == MSG_EVENT_OXQUIZ_INVALID_TIME )
			{
				pUIManager->CloseMessageBox( MSGCMD_NULL );
				CTString strSysMessage = _S(2626, "O.X 이벤트 입장 가능 시간이 아닙니다.") ;
				CUIMsgBox_Info	MsgBoxInfo;
				MsgBoxInfo.SetMsgBoxInfo(   _S(2627, "O.X 이벤트" ), UMBS_OK, UI_NONE, MSGCMD_NULL);
				MsgBoxInfo.AddString( strSysMessage );
				pUIManager->CreateMessageBox( MsgBoxInfo );

				pUIManager->SetCSFlagOff(CSF_TELEPORT);
				break;				
			}
			else if( QuizType == MSG_EVENT_OXQUIZ_START )
			{
				CTString strStart = _S(2628, "O.X 퀴즈를 시작 합니다." );
				_pNetwork->ClientSystemMessage( strStart );
			}
			else if( QuizType == MSG_EVENT_OXQUIZ_QUIZ )
			{
				(*istr) >> QuizNo;
				(*istr) >> Limitsec;
				(*istr) >> strQuizMessage;

				pUIManager->GetOXQuizEvent()->ReceiveQuiz( QuizNo, Limitsec, strQuizMessage );
			}
			else if( QuizType == MSG_EVENT_OXQUIZ_ANSWER )
			{

				(*istr) >> QuizNo;
				(*istr) >> isAnswer;

				CTString tv_str;
				tv_str.PrintF(_S(2629,"%d번 문제의 정답은"), QuizNo );

				if( isAnswer == 1 )
				{
					tv_str += _S(2630, "O 였습니다." );	
				}
				else
				{
					tv_str += _S(2631, "X 였습니다." );	
				}

				_pNetwork->ClientSystemMessage( tv_str );
			}
		}
		break;
	case MSG_EVENT_RECOMMEND :
		{
			ULONG subType;
			(*istr) >> subType;

			switch(subType){
			case MSG_EVENT_RECOMMEND_SET :

				break;
			case MSG_EVENT_RECOMMEND_POTION :

				break;
			case MSG_EVENT_RECOMMEND_MOONSTONE :
				ULONG tv_idx;
				CTString tv_str,tv_str2;
				(*istr) >> tv_idx;
				(*istr) >> tv_str;
				tv_str2.PrintF(_S(2632,"%s님께서 추천서버 이벤트에 당첨 되셨습니다."),tv_str.str_String);
				PCStartEffectGroup("server_event_using", tv_idx);
				_UIAutoHelp->SetGMNotice ( tv_str2, 0xFFAA33FF );
				break;

			}
		}
		break;
	case MSG_EVENT_PCBANG:	// NetCafe 보물상자 지급, (태국, 일본)NetCafe 캠페인 이벤트
		{
			ULONG ItemIndex;
			(*istr) >> ItemIndex;



			if( ItemIndex != -1 )
			{
				if( ItemIndex == 1394 || ItemIndex == 1395 || ItemIndex == 1396 || ItemIndex == 1397 )
				{// 보물 상자를 받을 때만
					Notice* pNotice = GAMEDATAMGR()->GetNotice();

					if (pNotice != NULL)
						pNotice->AddToNoticeList(4002, Notice::NOTICE_EVENT);
				}
				else
				{
					pUIManager->CloseMessageBox( MSGCMD_NULL );
					CTString strSysMessage = _S( 1840, "아이템이 성공적으로 지급 되었습니다." );
					CUIMsgBox_Info	MsgBoxInfo;
					MsgBoxInfo.SetMsgBoxInfo(   _S( 169, "이벤트" ), UMBS_OK, UI_NONE, MSGCMD_NULL);
					MsgBoxInfo.AddString( strSysMessage );
					pUIManager->CreateMessageBox( MsgBoxInfo );
				}
			}
			else
			{
				_pNetwork->ClientSystemMessage( _S( 1758, "보물 상자가 없습니다." ) );
			}
		}
		break;
	case MSG_EVENT_WORLDCUP :
		{
			ULONG subType,nIdx,errCode;
			(*istr) >> subType;
			(*istr) >> nIdx;
			(*istr) >> errCode;

			WorldCupErrChk( errCode) ;

		}
		break;
	case MSG_EVENT_GOLDENBALL:
		{
			ULONG subType, errType;
			CTString strTeamA, strTeamB, strMessage;
			ULONG ulTeamAScore, ulTeamBScore;
			ULONG ulYear, ulMonth, ulDay, ulHour, ulMinute;
			SYSTEMTIME GoldenTime;

			ZeroMemory( &GoldenTime, sizeof( GoldenTime ) );

			Notice* pNotice = GAMEDATAMGR()->GetNotice();

			CUIMsgBox_Info	MsgBoxInfo;

			(*istr) >>subType;

			switch( subType )
			{
			case MSG_EVENT_GOLDENBALL_VOTE_START:
				{
					(*istr) >> strTeamA;
					(*istr) >> strTeamB;
					(*istr) >> ulYear >> ulMonth >> ulDay >> ulHour >> ulMinute;

					GoldenTime.wYear = static_cast<WORD>(ulYear);
					GoldenTime.wMonth = static_cast<WORD>(ulMonth);
					GoldenTime.wDay = static_cast<WORD>(ulDay);
					GoldenTime.wHour = static_cast<WORD>(ulHour);
					GoldenTime.wMinute = static_cast<WORD>(ulMinute);

					pUIManager->GetQuest()->SetGoldenBallEntry( TRUE );
					pUIManager->GetQuest()->SetStrTeamName( strTeamA, strTeamB, GoldenTime, TRUE );

					if (pNotice != NULL)
						pNotice->AddToNoticeList(2000+100, Notice::NOTICE_EVENT);
				}
				break;
			case MSG_EVENT_GOLDENBALL_VOTE_END:
				{
					(*istr) >> strTeamA;
					(*istr) >> strTeamB;				

					pUIManager->GetQuest()->SetStrTeamName( strTeamA, strTeamB, GoldenTime, FALSE );
					pUIManager->GetQuest()->SetGoldenBallEntry( FALSE );
					_pNetwork->ClientSystemMessage( _S( 2844, "골든 볼 이벤트의 응모를 마감합니다." ) );
				}
				break;
			case MSG_EVENT_GOLDENBALL_GIFT_START:
				{
					(*istr) >> strTeamA;
					(*istr) >> ulTeamAScore;
					(*istr) >> strTeamB;
					(*istr) >> ulTeamBScore;
					(*istr) >> ulYear >> ulMonth >> ulDay >> ulHour >> ulMinute;

					GoldenTime.wYear = static_cast<WORD>(ulYear);
					GoldenTime.wMonth = static_cast<WORD>(ulMonth);
					GoldenTime.wDay = static_cast<WORD>(ulDay);
					GoldenTime.wHour = static_cast<WORD>(ulHour);
					GoldenTime.wMinute = static_cast<WORD>(ulMinute);

					pUIManager->GetQuest()->SetGoldenBallRequital( TRUE );
					pUIManager->GetQuest()->SetStrTeamName( strTeamA, strTeamB, GoldenTime, TRUE );
					pUIManager->GetQuest()->SetTeamScore( static_cast<int>( ulTeamAScore ), static_cast<int>( ulTeamBScore ) );

					if (pNotice != NULL)
						pNotice->AddToNoticeList(2000+101, Notice::NOTICE_EVENT);

					pUIManager->GetQuest()->SetGoldenBallRequital( TRUE );

				}	
				break;
			case MSG_EVENT_GOLDENBALL_GIFT_END:
				{
					pUIManager->GetQuest()->SetGoldenBallRequital( FALSE );
					_pNetwork->ClientSystemMessage( _S( 2845, "골든 볼 이벤트의 보상시간이 끝났습니다." ), SYSMSG_NOTIFY );
				}
				break;
			case MSG_EVENT_GOLDENBALL_VOTE:
				{
					(*istr) >> ulTeamAScore;
					(*istr) >> ulTeamBScore;

					pUIManager->GetQuest()->SetTeamScore( static_cast<int>( ulTeamAScore ), static_cast<int>( ulTeamBScore ) );

					(*istr) >> errType;

					switch( errType )
					{
					case MSG_EVENT_GOLDENBALL_ERROR_VOTE_OK:
						{
							strMessage = _S( 2846, "골든볼 이벤트에 응모되었습니다" );
							_pNetwork->ClientSystemMessage( _S( 2847, "골든 볼 이벤트 보상이 시작되면 랜디를 다시 찾아오셔서 보상을 받으시기 바랍니다." ), SYSMSG_NOTIFY );
						}
						break;
					case MSG_EVENT_GOLDENBALL_ERROR_VOTE_ALREADY:
						{
							strMessage = _S( 2848, "골든 볼 이벤트에 이미 응모 하셨습니다." );
						}
						break;
					case MSG_EVENT_GOLDENBALL_ERROR_VOTE_NOITEM:
						{
							strMessage = _S( 2849, "이벤트 응모를 위해서는 팀가이스트 축구공 10개가 필요합니다.\n확인 후 다시 시도해 주세요" );
						}
						break;
					case MSG_EVENT_GOLDENBALL_ERROR_VOTE_FULLINVEN:
						{
							strMessage = _S( 2850, "인벤토리가 가득 찼습니다." );
						}
						break;
					case MSG_EVENT_GOLDENBALL_ERROR_VOTE_TIME:
						{
							strMessage = _S( 2851, "골든 볼 이벤트 응모 시간이 아닙니다." );
						}
						break;
					}

					MsgBoxInfo.SetMsgBoxInfo( _S( 191, "확인" ), UMBS_OK, UI_NONE, MSGCMD_NULL );		
					MsgBoxInfo.AddString( strMessage );
					pUIManager->CreateMessageBox( MsgBoxInfo );
				}
				break;
			case MSG_EVENT_GOLDENBALL_GIFT:
				{
					(*istr) >> errType;

					switch( errType )
					{
					case MSG_EVENT_GOLDENBALL_ERROR_GIFT_OK_EXACT:
						{
							_pNetwork->ClientSystemMessage( _S( 2852, "골든 볼 이벤트 승리국 및 점수를 맞추어 보상을 획득 하셨습니다." ), SYSMSG_NOTIFY );
						}
						break;
					case MSG_EVENT_GOLDENBALL_ERROR_GIFT_OK_WINNER:
						{
							_pNetwork->ClientSystemMessage( _S( 2853, "골든 볼 이벤트 승리국 적중하여 보상을 획득하셨습니다." ), SYSMSG_NOTIFY );
						}
						break;
					case MSG_EVENT_GOLDENBALL_ERROR_GIFT_OK_NONE:
						{
							_pNetwork->ClientSystemMessage( _S( 2854, "골든 볼 이벤트 참가상 획득하셨습니다." ), SYSMSG_NOTIFY );
						}
						break;
					case MSG_EVENT_GOLDENBALL_ERROR_GIFT_FULLINVEN:
						{
							_pNetwork->ClientSystemMessage( _S( 2855, "인벤토리가 가득 찼습니다." ), SYSMSG_ERROR );
						}
						break;
					case MSG_EVENT_GOLDENBALL_ERROR_GIFT_NOITEM:
						{
							_pNetwork->ClientSystemMessage( _S( 2856, "골든 볼 이벤트 응모 카드가 없습니다." ), SYSMSG_ERROR );
						}
						break;
					case MSG_EVENT_GOLDENBALL_ERROR_GIFT_TIME:
						{
							_pNetwork->ClientSystemMessage( _S( 2857, "골든 볼 이벤트 보상 기간이 아닙니다." ), SYSMSG_ERROR );
						}
						break;
					}
				}
				break;
			}
			break;
		}
		break;
	case MSG_EVENT_RAIN_2006 : 
		{
			ULONG errType;
			(*istr) >> errType;
			CTString tv_str;
			switch(errType)
			{
			case MSG_EVENT_RAIN_2006_ERROR_OK :				// 보상 성공
				tv_str=_S( 159, "보상이 성공적으로 지급되었습니다.");
				break;
			case MSG_EVENT_RAIN_2006_ERROR_NOITEM :			// 빗방울/빗물병 없음
				tv_str=_S(2767, "쟈스민꽃이 부족하여 보상을 받을 수 없습니다. 확인 후 다시 시도하여 주세요.");
				break;
			case MSG_EVENT_RAIN_2006_ERROR_FULLINVEN :		// 인벤토리 가득참
				tv_str=_S( 2768,"인벤토리가 가득차서 보상을 받을 수 없습니다.");
				break;
			}
			_pNetwork->ClientSystemMessage(tv_str);
		}
		break;
	case MSG_EVENT_TLD_BUDDHIST:
		{
			BYTE ItemType;
			ULONG errType;
			(*istr) >> ItemType;
			(*istr) >> errType;

			switch( errType )
			{
			case MSG_EVENT_TLD_BUDDHIST_ERROR_OK:
				_pNetwork->ClientSystemMessage( _S( 2926, "성공적으로 보상품을 수령하였습니다." ), SYSMSG_NOTIFY );
				break;
			case MSG_EVENT_TLD_BUDDHIST_ERROR_FULLINVEN:
				_pNetwork->ClientSystemMessage( _S( 2768, "인벤토리가 가득차서 보상을 받을 수 없습니다."), SYSMSG_ERROR );
				break;
			case MSG_EVENT_TLD_BUDDHIST_ERROR_NOITEM:
				_pNetwork->ClientSystemMessage( _S( 2927, "모래 봉투가 부족하여 보상을 받을 수 없습니다. 확인 후 다시 시도하여 주세요." ), SYSMSG_ERROR );
				break;
			}
		}
		break;
	case MSG_EVENT_COLLECT_BUG:
		{
			ULONG subType;
			ULONG ulTemp;
			CTString strMessage;

			CUIMsgBox_Info	MsgBoxInfo;

			(*istr) >> subType;

			switch( subType )
			{
			case MSG_EVENT_COLLECT_BUG_BUY_BOX_REP_OK:
				{
					(*istr) >> ulTemp;
					if( ulTemp == 1 )
					{
						strMessage = _S( 2972, "축하합니다! 30포인트가 추가된 골든 곤충 채집상자를 구입하셨습니다" );
					}
					else if( ulTemp == 0 )
					{ 
						strMessage = _S( 2973, "곤충 채집상자 1개를 구입하였습니다." );
					}
				}
				break;
			case MSG_EVENT_COLLECT_BUG_BUY_BOX_REP_ERROR_DUPLICATE:
				{
					_pNetwork->ClientSystemMessage( _S( 2974, "이미 곤충 채집상자를 가지고 있어 더 이상 구입할 수 없습니다." ), SYSMSG_ERROR );
					return;
				}
				break;
			case MSG_EVENT_COLLECT_BUG_BUY_BOX_REP_ERROR_FULLINVEN:
				{
					_pNetwork->ClientSystemMessage( _S( 2855, "인벤토리가 가득 찼습니다." ), SYSMSG_ERROR );
					return;
				}
				break;
			case MSG_EVENT_COLLECT_BUG_BUY_BOX_REP_ERROR_MONEY:
				{
					_pNetwork->ClientSystemMessage( _S( 266, "소지금이 부족합니다." ), SYSMSG_ERROR );
					return;
				}
				break;
			case MSG_EVENT_COLLECT_BUG_GIFT_REP_OK:
				{
					(*istr) >> ulTemp;

					strMessage = _S( 2975, "곤충 채집 이벤트 보상이 완료 되었습니다." );
					if ( pUIManager->GetCollectBox()->IsVisible() )
						pUIManager->RearrangeOrder(UI_COLLECTBOX, FALSE);
				}
				break;
			case MSG_EVENT_COLLECT_BUG_GIFT_REP_ERROR_FULLINVEN:
				{
					_pNetwork->ClientSystemMessage( _S( 116, "인벤토리 공간이 부족하여 보상을 받을 수 없습니다." ), SYSMSG_ERROR );
					return;
				}
				break;
			case MSG_EVENT_COLLECT_BUG_GIFT_REP_ERROR_NOITEM:
				{
					_pNetwork->ClientSystemMessage( _S( 2976, "곤충 채집 상자를 가지고 있지 않습니다." ), SYSMSG_ERROR );
					return;
				}
				break;
			case MSG_EVENT_COLLECT_BUG_GIFT_REP_ERROR_NOTFULL:
				{
					_pNetwork->ClientSystemMessage( _S( 2977, "곤충 채집상자가 전부 채워지지 않아 보상 받을 수 없습니다." ), SYSMSG_ERROR );
					return;
				}
				break;
			case MSG_EVENT_COLLECT_BUG_PICK_OK:
				{
					(*istr) >> ulTemp;

					strMessage = pUIManager->GetCollectBox()->GetInsectName( ulTemp );
					_pNetwork->ClientSystemMessage( strMessage, SYSMSG_NOTIFY );					
					return;
				}
				break;
			case MSG_EVENT_COLLECT_BUG_PICK_NOITEM:
				{
					_pNetwork->ClientSystemMessage( _S( 2978, "곤충 채집 상자를 가지고 있지 않습니다." ), SYSMSG_ERROR );
					return;
				}
				break;
			case MSG_EVENT_COLLECT_BUG_PICK_FULL:
				{
					_pNetwork->ClientSystemMessage( _S( 2979, "채집상자가 가득 찼습니다." ), SYSMSG_ERROR );
					return;
				}
				break;
			case MSG_EVENT_COLLECT_BUG_DROP:
				{// 메세지 처리 안함 (eons)
					return;
				}
			}

			MsgBoxInfo.SetMsgBoxInfo( _S( 191, "확인" ), UMBS_OK, UI_NONE, MSGCMD_NULL );		
			MsgBoxInfo.AddString( strMessage );
			pUIManager->CreateMessageBox( MsgBoxInfo );
		}
		break;

	case MSG_EVENT_CHUSEOK_2006:
		{
			ULONG subType;
			CTString strMessage;

			CUIMsgBox_Info	MsgBoxInfo;

			(*istr) >> subType;
			switch(subType)
			{
			case MSG_EVENT_CHUSEOK_2006_MAKE_RICECAKE_OK:
				strMessage.PrintF( _S(3138, "송편 제작에 성공하였습니다." ) );	
				break;
			case MSG_EVENT_CHUSEOK_2006_MAKE_RICECAKE_FAIL:
				strMessage.PrintF( _S(3139, "송편 제작에 실패하였습니다." ) );	
				break;
			case MSG_EVENT_CHUSEOK_2006_MAKE_RICECAKE_NOITEM:
				strMessage.PrintF( _S(3140, "송편 재료가 부족합니다." ) );
				break;

			case MSG_EVENT_CHUSEOK_2006_MAKE_RAINBOW_CAKE_OK:			// 오색 송편 만들기 성공
				strMessage.PrintF( _S(3141, "오색송편 제작에 성공하였습니다." ) );	
				break;
			case MSG_EVENT_CHUSEOK_2006_MAKE_RAINBOW_CAKE_FAIL:			// 오색 송편 만들기 실패
				strMessage.PrintF( _S(3142, "오색송편 제작에 실패하였습니다." ) );	
				break;
			case MSG_EVENT_CHUSEOK_2006_MAKE_RAINBOW_CAKE_NOITEM:		// 오색 송편 재료 부족
				strMessage.PrintF( _S(3143, "오색송편 재료가 부족합니다." ) );
				break;

			case MSG_EVENT_CHUSEOK_2006_GIFT_OK:							// 오색 송편 교환 성공
				strMessage.PrintF( _S( 1884, "추석 이벤트 보상품이 지급되었습니다. 이벤트에 참여해 주셔서 감사합니다." ) );	
				break;
			case MSG_EVENT_CHUSEOK_2006_GIFT_OK_NOITEM:					// 오색 송편 부족
				strMessage.PrintF( _S(3144, "오색송편이 없어 보상을 받을 수 없습니다." ) );	
				break;

			case MSG_EVENT_CHUSEOK_2006_MAKE_RICECAKE_FULLINVEN:
			case MSG_EVENT_CHUSEOK_2006_MAKE_RAINBOW_CAKE_FULLINVEN:		// 인벤토리 부족
				strMessage.PrintF( _S(265, "인벤토리 공간이 부족합니다." ) );
				break;
			case MSG_EVENT_CHUSEOK_2006_GIFT_OK_FULLINVEN:				// 인벤토리 부족
				strMessage.PrintF( _S(116, "인벤토리 공간이 부족하여 보상을 받을 수 없습니다." ) );
				break;
			}
			MsgBoxInfo.SetMsgBoxInfo( _S( 191, "확인" ), UMBS_OK, UI_NONE, MSGCMD_NULL );		
			MsgBoxInfo.AddString( strMessage );
			pUIManager->CreateMessageBox( MsgBoxInfo );
		}
		break;
	case MSG_EVENT_XMAS_2006:		// 2006 X-Mas event [12/12/2006 Theodoric]
		{
			SLONG nCakeCount;
			ULONG subType;

			CTString strMessage;
			CUIMsgBox_Info	MsgBoxInfo;

			(*istr) >> nCakeCount;
			(*istr) >> subType;	

			switch(subType)
			{
			case MSG_EVENT_XMAS_2006_ERROR_OK:	// 성공
				strMessage.PrintF( _S(159, "교환이 성공적으로 이루어졌습니다." ) );
				break;
			case MSG_EVENT_XMAS_2006_ERROR_FULLINVEN:	// 인벤 가득 참
				strMessage.PrintF( _S(265, "인벤토리 공간이 부족합니다." ) );
				break;
			case MSG_EVENT_XMAS_2006_ERROR_NOITEM:	// 아이템 없음
				strMessage.PrintF( _S(2908, "교환할 아이템이 없습니다." ) );
				break;
			}

			MsgBoxInfo.SetMsgBoxInfo( _S( 191, "확인" ), UMBS_OK, UI_NONE, MSGCMD_NULL, MSGBOX_WIDTH + 20 );		
			MsgBoxInfo.AddString( strMessage );
			pUIManager->CreateMessageBox( MsgBoxInfo );
		}
		break;
	case MSG_EVENT_OPEN_BETA_GIFT:
		{
			CUIMsgBox_Info	MsgBoxInfo;
			CTString strMessage;

			strMessage.PrintF(_S(1840, "아이템이 성공적으로 지급 되었습니다."));
			MsgBoxInfo.SetMsgBoxInfo(_S(191, "확인"), UMBS_OK, UI_NONE, MSGCMD_NULL);
			MsgBoxInfo.AddString(strMessage);
			pUIManager->CreateMessageBox(MsgBoxInfo);
		}
		break;
	case MSG_EVENT_VALENTINE_2007: // 러브러브 이벤트
		{
			pUIManager->GetBingoBox()->ErrorMessage(&(*istr));
		}
		break;

		// wooss 070307 ---------------------------------------------------->>
		// kw : WSS_WHITEDAY_2007 
	case MSG_EVENT_WHITEDAY_2007:
		{
			ULONG subType;
			(*istr) >> subType;
			if( Whiteday2007ErrChk(subType) )
			{
				pUIManager->CloseMessageBox(MSGCMD_NULL);
				CUIMsgBox_Info	MsgBoxInfo;
				MsgBoxInfo.SetMsgBoxInfo( _S(191, "확인" ), UMBS_OK, UI_NONE, MSGCMD_NULL );
				CTString tMsg,tStr;

				UBYTE exType;
				(*istr) >> exType;
				switch(exType)
				{
				case 0:
					{
						ULONG iIdx;
						(*istr) >> iIdx;						
						tStr = CTString(_pNetwork->ga_World.wo_aSkillData[iIdx].GetName());
						tMsg.PrintF(_S(3261, "로레인으로부터 %s 주문을 받았습니다."), tStr.str_String );
						MsgBoxInfo.AddString( tMsg );
						pUIManager->CreateMessageBox( MsgBoxInfo );
					}
					break;
				case 1:
					{
						tMsg = _S(3262,"로레인으로부터 하늘색 편지지 1장을 받았습니다.");
						MsgBoxInfo.AddString( tMsg );
						pUIManager->CreateMessageBox( MsgBoxInfo );					
					}					
					break;
				case 2:
					{
						tMsg =_S(3263,"로레인으로부터 분홍색 편지지 1장을 받았습니다.");
						MsgBoxInfo.AddString( tMsg );
						pUIManager->CreateMessageBox( MsgBoxInfo );					
					}
					break;
				}
			}

			if( subType ==  MSG_EVENT_WHITEDAY_2007_LETTER_REP )
			{
				COLOR tCol;
				CTString tStr;
				(*istr) >> tCol;
				(*istr) >> tStr;
				pUIManager->GetChattingUI()->AddSysMessage( tStr, SYSMSG_USER,tCol );
			}				
		}
		break;
		// [KH_070417] 스승의날 이벤트 관련 추가
	case MSG_EVENT_TEACH_2007:
		ReceiveEventMaster(istr);
		break;
		// ------------------------------------------------------------------<<
	case MSG_EVENT_CHILDRENSDAY_2007:
		{
			ULONG subType;
			(*istr) >> subType;
			if(subType == MSG_EVENT_CHILDRENSDAY_2007_REP)
			{				
				UBYTE exType;
				(*istr) >> exType;

				CTString tMsg;
				CUIMsgBox_Info	MsgBoxInfo;
				MsgBoxInfo.SetMsgBoxInfo( _S(191, "확인" ), UMBS_OK, UI_NONE, MSGCMD_NULL );

				switch(exType)
				{
				case 0:
					{
						ULONG iItemidx;
						(*istr) >> iItemidx;
						CItemData* pID = _pNetwork->GetItemData(iItemidx);
						tMsg.PrintF(_S(3405, "%s 아이템을 획득 하였습니다."), pID->GetName());
						MsgBoxInfo.AddString(tMsg);
						pUIManager->CreateMessageBox( MsgBoxInfo );

					}break;
				case 1:
					{
						tMsg = _S(3406, "보유하고 있는 노란 개나리 꽃의 개수가 부족합니다. 확인하고 다시 시도하여 주십시오");
						MsgBoxInfo.AddString(tMsg);
						pUIManager->CreateMessageBox( MsgBoxInfo );

					}break;
				case 2:
					{
						tMsg = _S(3407, "한번 받은 장비는 두 번 받을 수 없습니다. 확인하고 다시 시도 하여 주십시오");
						MsgBoxInfo.AddString(tMsg);
						pUIManager->CreateMessageBox(MsgBoxInfo);
					}break;
				default:
					{
						tMsg.PrintF(_S(1844,"지급된 아이템을 정상적으로 처리하지 못하였습니다. 다시 시도하여 주십시오."));
						MsgBoxInfo.AddString(tMsg);
						pUIManager->CreateMessageBox(MsgBoxInfo);
					}break;

				}

			}
		}break;
		// [KH_070326] 부활절 이벤트 관련
	case MSG_EVENT_EGGS_HUNT_2007:
		{
			ULONG subType;
			(*istr) >> subType;
			if( EasterEggs2007ErrChk(subType) )
			{
				pUIManager->CloseMessageBox(MSGCMD_NULL);
				CUIMsgBox_Info	MsgBoxInfo;
				MsgBoxInfo.SetMsgBoxInfo( _S(191, "확인" ), UMBS_OK, UI_NONE, MSGCMD_NULL );
				CTString tMsg,tStr;

				INDEX exItemType;	// [KH_070329] 아이템 서버 인덱스
				INDEX exItemNum;	// [KH_070329] 아이템 갯수
				(*istr) >> exItemType;
				(*istr) >> exItemNum;

				if(exItemNum != 1) // [KH_070329] 한개가 아니면 숫자 표시
					tStr.PrintF("%d", exItemNum);
				else // [KH_070329] 하나가 아니면 표시 안함
				{
					tStr = CTString("");
				}

				tMsg.PrintF( _S( 3266, "부활절 달걀 보상품 %s%s를 획득하였습니다."), tStr, _pNetwork->GetItemName( exItemType ) );

				MsgBoxInfo.AddString( tMsg );

				pUIManager->CreateMessageBox( MsgBoxInfo );
			}
		}
		break;

		// ------------------------------------------------------------------<<
		// WSS_GUILDMASTER -------------------------------------------------->>
	case MSG_EVENT_PARENTSDAY_2007:
		{
			UBYTE subType;
			SBYTE errCode;  
			(*istr) >> subType;
			switch(subType)
			{
			case MSG_EVENT_PARENTSDAY_2007_ACCUMULATEPOINT_RANKING_LIST:
				{
					(*istr)>>errCode;
					if(GuildMasterErrChk(errCode))
					{						
						ULONG tCnt;
						ULONG tRank,tPoint;
						CTString gName,strRank,strPoint;
						CUIListBox* tBox = &pUIManager->GetMessageBox(MSGCMD_GUILDMASTER_1)->GetListBox();

						// Init string
						ASSERT(tBox!=NULL);
						tBox->ResetAllStrings();
						tBox->AddString( 0, _S(1714, "순위"),0xFFF244FF, FALSE );
						tBox->AddString( 1, _S(3450, "포인트"),0xFFF244FF, FALSE );
						tBox->AddString( 2, _S(3451, "길드명"),0xFFF244FF, FALSE );	

						(*istr) >> tCnt;
						for(int i=0; i<tCnt;i++)
						{
							(*istr) >> tRank >> tPoint >> gName;
							strRank.PrintF("%d",tRank);
							strPoint.PrintF("%d",tPoint);
							tBox->AddString(0,strRank,0xF2F2F2FF,FALSE);
							tBox->AddString(1,strPoint,0xF2F2F2FF,FALSE);
							tBox->AddString(2,gName,0xF2F2F2FF,FALSE);							
						}									
					}						
				}	
				break;
			case MSG_EVENT_PARENTSDAY_2007_ADD_POINT:			// s->c : errorcode(c) point(n) totalpoint(n)
				{
					(*istr)>>errCode;
					if( GuildMasterErrChk(errCode))
					{
						CTString tMsg;
						CUIMsgBox_Info	MsgBoxInfo;
						ULONG tInc,tPnt;
						(*istr) >> tInc >> tPnt;
						MsgBoxInfo.SetMsgBoxInfo( _S(191, "확인" ), UMBS_OK, UI_NONE, MSGCMD_NULL );
						tMsg.PrintF(_S(3476,"길드의 포인트가 %d만큼 증가하여 %d점이 되었습니다."),tInc,tPnt);
						MsgBoxInfo.AddString(tMsg);
						pUIManager->CreateMessageBox( MsgBoxInfo );										
					}
				}
				break;
			case MSG_EVENT_PARENTSDAY_2007_EXCHANGE_TICKET:		// s->c : errorcode(c) changeTicketCount(n)
				{
					(*istr)>>errCode;
					if(GuildMasterErrChk(errCode))
					{
						CTString tMsg;
						CUIMsgBox_Info	MsgBoxInfo;
						ULONG tCnt;
						(*istr) >> tCnt;
						MsgBoxInfo.SetMsgBoxInfo( _S(191, "확인" ), UMBS_OK, UI_NONE, MSGCMD_NULL );
						tMsg.PrintF(_S(3477,"누적된 교환권 %d장을 지급 받았습니다."),tCnt);
						MsgBoxInfo.AddString(tMsg);
						pUIManager->CreateMessageBox( MsgBoxInfo );										
					}
				}
				break;
			case MSG_EVENT_PARENTSDAY_2007_EXCHANGE_ITEM:		// s->c: errcode(c)					
				(*istr)>>errCode;
				GuildMasterErrChk(errCode);
				break;
			case MSG_EVENT_PARENTSDAY_2007_NOTICE:
				{
					CTString strMessage,tGuild,tItem;
					(*istr)>>tGuild>>tItem;
					strMessage.PrintF(_S(3478,"[%s]길드의 길드 마스터가 [%s]아이템에 당첨 되었습니다."),tGuild,tItem);
					_UIAutoHelp->SetGMNotice ( strMessage );
				}
				break;
			}
		}
		break;
		// ------------------------------------------------------------------<<

		// WSS_MINIGAME 070420 ---------------------------------------------->>
	case MSG_EVENT_GOMDORI_2007 :
		{
			UBYTE subType;
			SBYTE wincount; 
			SBYTE usersel;  
			SBYTE npcsel;  
			CUIMsgBox_Info msgInfo;
			CTString strMessage=_S(191,"확인");
			CUIMinigame* pMinigame=((CUIMinigame*)pUIManager->GetUI(UI_MINIGAME));
			(*istr) >> subType;
			switch(subType)
			{
			case MSG_EVENT_GOMDORI_2007_CHECKITEM:
				// 에코시 버튼을 풀어 준다.
				pMinigame->OpenMinigame(MSG_EVENT_GOMDORI_2007);
				break;
			case MSG_EVENT_GOMDORI_2007_START:
				// 에코시 버튼을 풀어 준다.
				pMinigame->SetButtonState(BTN_STATE_START_CONTINUE);
				break;
			case MSG_EVENT_GOMDORI_2007_CONTINUE:
				// 에코시 버튼을 풀어 준다.
				pMinigame->SetButtonState(BTN_STATE_START_CONTINUE);
				break;

			case MSG_EVENT_GOMDORI_2007_RESULT_WIN:
				// TODO : 이겼을 경우 처리(UIMinigame내 함수 호출)
				(*istr)>>wincount>>usersel>>npcsel;
				pMinigame->SetMinigameResult(wincount,usersel,npcsel,0);
				break;
			case MSG_EVENT_GOMDORI_2007_RESULT_LOSE:
				// TODO : 졌을 경우 처리(UIMinigame내 함수 호출)
				(*istr)>>wincount>>usersel>>npcsel;
				pMinigame->SetMinigameResult(wincount,usersel,npcsel,2);
				break;
			case MSG_EVENT_GOMDORI_2007_RESULT_DRAW:
				// TODO : 비겼을 경우 처리(UIMinigame내 함수 호출)
				(*istr)>>wincount>>usersel>>npcsel;
				pMinigame->SetMinigameResult(wincount,usersel,npcsel,1);
				break;
			case MSG_EVENT_GOMDORI_2007_END:
				{
					UBYTE wincount,prompttype;
					ULONG extra1,extra2;

					(*istr)>>wincount>>prompttype>>extra1>>extra2;
					if(prompttype== 0)
					{
						msgInfo.SetMsgBoxInfo(strMessage,UMBS_OK,UI_NONE,MSGCMD_NULL);
						//	strMessage.PrintF( _s( "보상아이템[%s] %d개가 지급 되었습니다." ), _pNetwork->GetItemName(extra1), extra2);
						strMessage.PrintF( _S(1840, "아이템이 성공적으로 지급 되었습니다" ));
						msgInfo.AddString(strMessage);
						pUIManager->CreateMessageBox(msgInfo);
					}
					else if(prompttype== 1)
					{
						msgInfo.SetMsgBoxInfo(strMessage,UMBS_OK,UI_NONE,MSGCMD_NULL);
						strMessage.PrintF( _S(3479, "체험 교복이 지급되었습니다." ));
						msgInfo.AddString(strMessage);
						pUIManager->CreateMessageBox(msgInfo);
					}
					else if(prompttype== 2)
					{
						CTString strName1,strName2;
						strName1.PrintF("%s",_pNetwork->GetItemName(extra1));
						strName2.PrintF("%s",_pNetwork->GetItemName(extra2));

						// 저장
						pMinigame->m_extra1 = extra1;
						pMinigame->m_extra2 = extra2;

						strMessage= _S(3480, "보상 무기 선택" );
						msgInfo.SetMsgBoxInfo(strMessage,UMBS_SELECTBOX,UI_MINIGAME,MSGCMD_MINIGAME_SELECT_ITEM);
						msgInfo.m_nHeight =200;
						pUIManager->CreateMessageBox(msgInfo);
						pUIManager->GetMessageBox(MSGCMD_MINIGAME_SELECT_ITEM)->addSelectContext(strName1);
						if (_pNetwork->MyCharacterInfo.job != NIGHTSHADOW)
						{
							pUIManager->GetMessageBox(MSGCMD_MINIGAME_SELECT_ITEM)->addSelectContext(strName2);
						}
					}

					// 게임 창 닫기 
					pMinigame->Close();
				}
				break;
			case MSG_EVENT_GOMDORI_2007_VIEW_STATUS:
				{
					ULONG serverCnt,itemCnt;
					ULONG itemAll=0;
					(*istr)>>serverCnt;
					strMessage= _S(3481, "곰돌이 인형 웬디 찾기 현황");

					msgInfo.SetMsgBoxInfo(strMessage,UMBS_OK,UI_NONE,MSGCMD_NULL,300);

					strMessage=_S(3482, "♣완성된 곰돌이 웬디의 전체 누적 수에 따라 실제 곰 인형과 학용품 셋트를 어린이날 천사의 집 어린이들에게 선물하게 됩니다." );
					msgInfo.AddString(strMessage);

					for(int i=0;i<serverCnt;i++)
					{
						(*istr)>>itemCnt;
						strMessage.PrintF( _S(576, "%s : %d개" ),pUIManager->GetServerSelect()->GetServerName(i+1),itemCnt);
						msgInfo.AddString(strMessage);

						itemAll+=itemCnt;
					}
					msgInfo.AddString(CTString(" "));
					strMessage.PrintF( _S(3483, "전체 서버에서 찾은 완성품 수 : %d개" ),itemAll);
					msgInfo.AddString(strMessage);

					pUIManager->CreateMessageBox(msgInfo);
				}
				break;

			case MSG_EVENT_GOMDORI_2007_ERROR_NOITEM:
				{
					if(pUIManager->DoesMessageBoxExist(MSGCMD_NULL)) return;
					msgInfo.SetMsgBoxInfo(strMessage,UMBS_OK,UI_NONE,MSGCMD_NULL);
					strMessage.PrintF( _S(3484, "어라, 곰돌이가 없네요? 곰돌이가 없으면 가위바위보를 할 수 없어요. 게임을 하고 싶다면, 곰돌이조각 1번부터 9번까지를 모두 모아 완성된 곰돌이를 가져와야 해요." ));
					msgInfo.AddString(strMessage);
					pUIManager->CreateMessageBox(msgInfo);
				}
				break;

			case MSG_EVENT_GOMDORI_2007_ERROR_FULLINVENTORY:
				strMessage=_S(675, "인벤토리가 가득 차서 아이템 지급이 불가능합니다." );
				pUIManager->GetChattingUI()->AddSysMessage( strMessage, SYSMSG_NORMAL );
				break;
			}
		}
		break;
		// ------------------------------------------------------------------<<
	case MSG_EVENT_FLOWERTREE_2007: // 2007 꽃놀이 이벤트
		{
			UBYTE subtype, errortype;
			SLONG SvCount, SvGroup;
			ULONG SvPoint, SvPoint1;

			CUIMsgBox_Info MsgBoxInfo;

			(*istr) >> subtype;

			switch (subtype)
			{
			case MSG_EVENT_FLOWERTREE_2007_POINT:
				{
					(*istr) >> SvPoint;
					(*istr) >> SvGroup;

					pUIManager->GetFlowerTree()->AddServerNameList(SvPoint, SvGroup);
				}
				break;
			case MSG_EVENT_FLOWERTREE_2007_POINT_RANK:
				{
					(*istr) >> SvCount; // 서버군 갯수

					SLONG i;

					for (i=0; i<SvCount; i++)
					{
						(*istr) >> SvPoint; // 포인트
						(*istr) >> SvGroup; // 서버군 번호

						pUIManager->GetFlowerTree()->AddServerNameList(SvPoint, SvGroup);
					}
				}
				break;
			case MSG_EVENT_FLOWERTREE_2007_GROWUP:
				{
					(*istr) >> SvPoint; // 증가 포인트

					CTString strMessage;

					if (SvPoint > 0)
					{
						strMessage.PrintF(_S(3494, "이 나무의 개화를 도와주어 기여도가 %d증가 하였습니다."), SvPoint);
					}
					else
					{
						strMessage.PrintF(_S(3495, "나무에게 줄 퇴비와 정화수가 없습니다."));						
					}

					MsgBoxInfo.SetMsgBoxInfo(_S(191, "확인"), UMBS_OK, UI_NONE, MSGCMD_NULL);
					MsgBoxInfo.AddString(strMessage);
					pUIManager->CreateMessageBox(MsgBoxInfo);
				}
				break;
			case MSG_EVENT_FLOWERTREE_2007_MY_POINT:
				{
					(*istr) >> SvPoint; // 유저가 기여한 포인트
					(*istr) >> SvPoint1;

					if (pUIManager->DoesMessageBoxExist(MSGCMD_FLOWERTREE_SUPPORT))
						pUIManager->CloseMessageBox(MSGCMD_FLOWERTREE_SUPPORT);

					if (pUIManager->GetFlowerTree()->IsVisible())
						pUIManager->RearrangeOrder(UI_FLOWERTREE, FALSE);

					MsgBoxInfo.SetMsgBoxInfo(_S(169, "이벤트"), UMBS_USER_12, UI_FLOWERTREE, MSGCMD_FLOWERTREE_TICKET);
					MsgBoxInfo.SetUserBtnName(_S(3498, "교환권 받기"), _S(3499, "교환권 확인"));
					MsgBoxInfo.AddString( _S(3500, "꽃나무 기여포인트"), 0xF3F3F3FF, TEXT_CENTER );
					CTString strPoint, strPoint1;
					strPoint.PrintF(_S(3511, "나의 총 기여포인트: %d"), SvPoint);
					strPoint1.PrintF(_S(3512, "현재 교환권을 받을 갯수: %d"), SvPoint1);
					MsgBoxInfo.AddString(CTString(" "));
					MsgBoxInfo.AddString( strPoint, 0x6BD2FFFF, TEXT_CENTER );
					MsgBoxInfo.AddString( strPoint1, 0x6BD2FFFF, TEXT_CENTER );
					pUIManager->CreateMessageBox( MsgBoxInfo );
				}
				break;
			case MSG_EVENT_FLOWERTREE_2007_RECV_TICKET:
				{
					(*istr) >> SvCount; // 교환권 개수

					CTString strTicket;					

					MsgBoxInfo.SetMsgBoxInfo(_S(191, "확인"), UMBS_OK, UI_NONE, MSGCMD_NULL);

					if (SvCount > 0)
						strTicket.PrintF(_S(3496, "누적된 교환권 %d장을 지급 받았습니다"), SvCount);
					else
						strTicket.PrintF(_S(3497, "지급받을 교환권이 없습니다."));

					MsgBoxInfo.AddString(strTicket);
					pUIManager->CreateMessageBox(MsgBoxInfo);
				}
				break;
			case MSG_EVENT_FLOWERTREE_2007_OPEN_TICKET:
				{
					(*istr) >> errortype;

					switch (errortype)
					{
					case 0: // 실패
						{
							MsgBoxInfo.SetMsgBoxInfo(_S(191, "확인"), UMBS_OK, UI_NONE, MSGCMD_NULL);
							MsgBoxInfo.AddString(_S(3502, "현재 보유하고 있는 교환권이 없습니다. 확인하고 다시 시도하여 주십시요"));
							pUIManager->CreateMessageBox(MsgBoxInfo);
						}
						break;
					}
				}
				break;
			case MSG_EVENT_FLOWERTREE_2007_PRICE_MSG:
				{ // 보상 메세지
					CTString CharName, strMessage;
					SLONG nItemIndex;

					(*istr) >> CharName;
					(*istr) >> nItemIndex;

					strMessage.PrintF(_S(3503, "%s님이 %s 아이템에 당첨되었습니다."), CharName, _pNetwork->GetItemData(nItemIndex)->GetName());
					_UIAutoHelp->SetGMNotice ( strMessage );
				}
				break;
			}
		}
		break;
		////////////////////////////////////////////////////////////////
		// [070705: Su-won] EVENT_SUMMER_2007						|---------->
	case MSG_EVENT_SUMMER_VACATION_2007:
		{
			LONG subtype;
			(*istr) >> subtype;

			CTString strMessage;
			CUIMsgBox_Info MsgBoxInfo;

			switch (subtype)
			{
			case MSG_EVENT_SUMMER_VACATION_2007_ERROR_MAKE_OK:
				{
					LONG nResult;
					LONG nReward;
					LONG nCount;
					(*istr) >> nResult;
					(*istr) >> nReward;
					(*istr) >> nCount;

					MsgBoxInfo.SetMsgBoxInfo(_S(191, "확인"), UMBS_OK, UI_NONE, MSGCMD_NULL);

					CTString strColor;
					if( nResult == 1 )
						strColor = _S(3576, "그린");
					else if( nResult<=4)
						strColor = _S(3577, "레드");
					else if( nResult<=7)
						strColor = _S(3578, "실버");
					else if( nResult<=10)
						strColor = _S(3579, "골드");

					if( nResult == 1 )
						strMessage.PrintF( _S(3580, "색종이로 %s 꽃을 접으셨네요! 축하합니다!"), strColor);
					else if( nResult%3 ==2) 
						strMessage.PrintF( _S(3581, "색종이로 %s 개구리를 접으셨네요! 축하합니다!"), strColor);
					else if( nResult%3 ==0) 
						strMessage.PrintF( _S(3582, "색종이로 %s 종이학을 접으셨네요! 축하합니다!"), strColor);
					else if( nResult%3 ==1) 
						strMessage.PrintF( _S(3583, "색종이로 %s 드래곤을 접으셨네요! 축하합니다!"), strColor);
					MsgBoxInfo.AddString(strMessage);

					if( nReward == 19 )
						strMessage.PrintF( _S(3584, "보상으로 %d 나스를 드리겠습니다."), nCount );
					else
						strMessage.PrintF( _S(3585, "보상으로 %s %d개를 드리겠습니다."), _pNetwork->GetItemName(nReward), nCount );
					MsgBoxInfo.AddString(strMessage);

					pUIManager->CreateMessageBox(MsgBoxInfo);
				}
				break;
			case MSG_EVENT_SUMMER_VACATION_2007_ERROR_INCHEN_OK:
				{
					LONG nResult;
					(*istr) >> nResult;
					MsgBoxInfo.SetMsgBoxInfo(_S(191, "확인"), UMBS_OK, UI_NONE, MSGCMD_NULL);
					strMessage.PrintF( _S(558, "조합에 성공하였습니다."));
					MsgBoxInfo.AddString(strMessage);
					strMessage.PrintF( _S(3586, "%s 1장이 생성되었습니다."), _pNetwork->GetItemName(nResult));
					MsgBoxInfo.AddString(strMessage);
					pUIManager->CreateMessageBox(MsgBoxInfo);
				}
				break;
			case MSG_EVENT_SUMMER_VACATION_2007_ERROR_MAKE_FAIL:
				{
					MsgBoxInfo.SetMsgBoxInfo(_S(191, "확인"), UMBS_OK, UI_NONE, MSGCMD_NULL);
					strMessage.PrintF( _S(3588, "종이 접기에 가능한 조합이 아닙니다."));
					MsgBoxInfo.AddString(strMessage);
					pUIManager->CreateMessageBox(MsgBoxInfo);
				}
				break;
			case MSG_EVENT_SUMMER_VACATION_2007_ERROR_INCHEN_FALIL:
				{
					LONG nResult;
					(*istr) >> nResult;
					MsgBoxInfo.SetMsgBoxInfo(_S(191, "확인"), UMBS_OK, UI_NONE, MSGCMD_NULL);
					strMessage.PrintF( _S(557, "조합에 실패하였습니다."));
					MsgBoxInfo.AddString(strMessage);
					if( nResult != 2370 )
					{
						strMessage.PrintF( _S(3587, "%s 4장이 사라졌습니다."), _pNetwork->GetItemName(nResult));
						MsgBoxInfo.AddString(strMessage);
					}
					pUIManager->CreateMessageBox(MsgBoxInfo);
				}
				break;
			case MSG_EVENT_SUMMER_VACATION_2007_ERROR_NOITEM:
				{
					strMessage.PrintF(_S( 1330 ,"아이템이 부족합니다." ));
					_pNetwork->ClientSystemMessage( strMessage, SYSMSG_ERROR );
				}
				break;
			case MSG_EVENT_SUMMER_VACATION_2007_ERROR_FULLINVENTORY:
				{
					strMessage.PrintF(_S( 265 ,"인벤토리 공간이 부족합니다." ));
					_pNetwork->ClientSystemMessage( strMessage, SYSMSG_ERROR );
				}
				break;
			case MSG_EVENT_SUMMER_VACATION_2007_ERROR_NOMONEY:
				{
					strMessage.PrintF(_S( 266 ,"소지금이 부족합니다." ));
					_pNetwork->ClientSystemMessage( strMessage, SYSMSG_ERROR );
				}
				break;
			}
		}
		break;
		// [070703: Su-won] EVENT_SUMMER_2007						<----------|
		////////////////////////////////////////////////////////////////

		//////////////////////////////////////////////////////////////////////////////////////////////////
		// [070807: Su-won] EVENT_ADULT_OPEN
	case MSG_EVENT_OPEN_ADULT_SERVER:
		{
			LONG subtype;
			(*istr) >> subtype;

			CTString strMessage;
			CUIMsgBox_Info MsgBoxInfo;

			pUIManager->CloseMessageBox( MSGCMD_NULL );

			switch(subtype)
			{
				//////////////////////////////////////////////
				// 매직카드 이벤트
			case MSG_EVENT_OPEN_ADULT_SERVER_MAGIC_OK:
				{
					UBYTE ubBuffType;
					(*istr) >> ubBuffType;

					CTString strBuff;
					switch(ubBuffType)
					{
					case 0:
						strBuff = CTString(_pNetwork->GetSkillData(423).GetName());	//바람의 러브매직
						break;
					case 1:
						strBuff = CTString(_pNetwork->GetSkillData(424).GetName()); //힘의 러브매직
						break;
					case 2:
						strBuff = CTString(_pNetwork->GetSkillData(425).GetName()); //민첩의 러브매직
						break;
					case 3:
						strBuff = CTString(_pNetwork->GetSkillData(426).GetName()); //지혜의 러브매직
						break;
					case 4:
						strBuff = CTString(_pNetwork->GetSkillData(427).GetName()); //체질의 러브매직
						break;
					case 5:
						strBuff = CTString(_pNetwork->GetSkillData(428).GetName()); //로레인의 러브매직
						break;
					}

					strMessage.PrintF( _S(3637, "[성인서버 오픈 이벤트] 이벤트 진행요원으로부터 %s 마법주문을 받았습니다."), strBuff);

					MsgBoxInfo.SetMsgBoxInfo(_S(191, "확인"), UMBS_OK, UI_NONE, MSGCMD_NULL);
					MsgBoxInfo.AddString( strMessage );
					pUIManager->CreateMessageBox(MsgBoxInfo);
				}
				break;
			case MSG_EVENT_OPEN_ADULT_SERVER_MAGIC_FAILED:
				{
				}
				break;
			case MSG_EVENT_OPEN_ADULT_SERVER_MAGIC_NOITEM:
				{
					strMessage.PrintF(_S( 1330 ,"아이템이 부족합니다." ));
					_pNetwork->ClientSystemMessage( strMessage, SYSMSG_ERROR );
				}
				break;
				// 매직카드 이벤트
				//////////////////////////////////////////////


				//////////////////////////////////////////////
				// 전직 이벤트
			case MSG_EVENT_OPEN_ADULT_SERVER_CHABGEJOB_EXCHANGE_OK:
				{
					LONG lRank, lCount;
					(*istr) >> lRank;
					(*istr) >> lCount;

					if( lRank <=100 )
						strMessage.PrintF( _S(3638, "%d등! 축하합니다. 상품으로 절망의 부적 %d개를 지급받으셨습니다."), lRank, lCount);
					else
						strMessage.PrintF( _S(3639, "축하합니다. 상품으로 절망의 부적 %d개를 지급받으셨습니다."), lCount);

					MsgBoxInfo.SetMsgBoxInfo(_S(191, "확인"), UMBS_OK, UI_NONE, MSGCMD_NULL);
					MsgBoxInfo.AddString( strMessage );
					pUIManager->CreateMessageBox(MsgBoxInfo);
				}
				break;
			case MSG_EVENT_OPEN_ADULT_SERVER_CHABGEJOB_ERROR_ALREADY:
				{
					MsgBoxInfo.SetMsgBoxInfo(_S(191, "확인"), UMBS_OK, UI_NONE, MSGCMD_NULL);
					MsgBoxInfo.AddString( _S(3640, "이미 전직 달성 상품을 받으셨습니다.") );
					pUIManager->CreateMessageBox(MsgBoxInfo);

				}
				break;
			case MSG_EVENT_OPEN_ADULT_SERVER_CHABGEJOB_ERROR_FAILED:
				{
					MsgBoxInfo.SetMsgBoxInfo(_S(191, "확인"), UMBS_OK, UI_NONE, MSGCMD_NULL);
					MsgBoxInfo.AddString( _S(3641, "전직을 하지 않아 상품을 받을 수 없습니다.") );
					pUIManager->CreateMessageBox(MsgBoxInfo);
				}
				break;
			case MSG_EVENT_OPEN_ADULT_SERVER_CHABGEJOB_ERROR_FULLINVEN:
				{
					strMessage.PrintF(_S( 265 ,"인벤토리 공간이 부족합니다." ));
					_pNetwork->ClientSystemMessage( strMessage, SYSMSG_ERROR );
				}
				break;
				// 전직 이벤트
				//////////////////////////////////////////////

				//////////////////////////////////////////////
				// 연금술 이벤트
			case MSG_EVENT_OPEN_ADULT_SERVER_UPGRADE_OK:
				{
					MsgBoxInfo.SetMsgBoxInfo(_S(191, "확인"), UMBS_OK, UI_NONE, MSGCMD_NULL);
					MsgBoxInfo.AddString( _S(3642, "축하합니다! 장비조합 연금술이 성공하였습니다.") );
					pUIManager->CreateMessageBox(MsgBoxInfo);
				}
				break;
			case MSG_EVENT_OPEN_ADULT_SERVER_UPGRADE_ERROR_NOT_FOUND:
				{
					MsgBoxInfo.SetMsgBoxInfo(_S(191, "확인"), UMBS_OK, UI_NONE, MSGCMD_NULL);
					MsgBoxInfo.AddString( _S(3643, "연금술을 시도하기 위해 필요한 재료가 부족합니다.") );
					pUIManager->CreateMessageBox(MsgBoxInfo);
				}
				break;
			case MSG_EVENT_OPEN_ADULT_SERVER_UPGRADE_ERROR_FAILED:
				{
					MsgBoxInfo.SetMsgBoxInfo(_S(191, "확인"), UMBS_OK, UI_NONE, MSGCMD_NULL);
					MsgBoxInfo.AddString( _S(3644, "이럴수가...장비조합 연금술이 실패하여 사용한 재료가 사라졌습니다.") );
					pUIManager->CreateMessageBox(MsgBoxInfo);
				}
				break;
			case MSG_EVENT_OPEN_ADULT_SERVER_UPGRADE_ERROR_FULLINVEN:
				{
					strMessage.PrintF(_S( 265 ,"인벤토리 공간이 부족합니다." ));
					_pNetwork->ClientSystemMessage( strMessage, SYSMSG_ERROR );
				}
				break;
				// 연금술 이벤트
				//////////////////////////////////////////////
			}
		}
		break;
		// '거침없이 쏜다!' 이벤트
	case MSG_EVENT_GIFT_2007:
		{
			BYTE subtype;
			(*istr) >> subtype;

			CTString strMessage;
			CUIMsgBox_Info MsgBoxInfo;

			pUIManager->CloseMessageBox( MSGCMD_NULL );

			switch(subtype)
			{
				//성공
			case MSG_CONN_EVENT_GIFT_2007_GIVE:
				{
					MsgBoxInfo.SetMsgBoxInfo(_S(191, "확인"), UMBS_OK, UI_NONE, MSGCMD_NULL);
					MsgBoxInfo.AddString( _S(2234, "이벤트 아이템을 지급받았습니다.") );
					pUIManager->CreateMessageBox(MsgBoxInfo);
				}
				break;
				//이벤트 DB 에러(?)
			case MSG_CONN_EVENT_GIFT_2007_CHECK_DBERROR:
				break;
				//이미 보상 받았음
			case MSG_CONN_EVENT_GIFT_2007_CHECK_DENY:
				{
					MsgBoxInfo.SetMsgBoxInfo(_S(191, "확인"), UMBS_OK, UI_NONE, MSGCMD_NULL);
					MsgBoxInfo.AddString( _S(673, "이벤트 아이템이 이미 지급되었습니다.") );
					pUIManager->CreateMessageBox(MsgBoxInfo);
				}
				break;
				//DB 없음(?)
			case MSG_CONN_EVENT_GIFT_2007_GIVE_DBERROR:
				break;
				// 인벤 만땅
			case MSG_CONN_EVENT_GIFT_2007_GIVE_FULL:
				{
					MsgBoxInfo.SetMsgBoxInfo(_S(191, "확인"), UMBS_OK, UI_NONE, MSGCMD_NULL);
					MsgBoxInfo.AddString( _S(116, "인벤토리 공간이 부족하여 보상을 받을 수 없습니다.") );
					pUIManager->CreateMessageBox(MsgBoxInfo);
				}
				break;
			}
		}
		break;
		// [070807: Su-won] EVENT_ADULT_OPEN
		//////////////////////////////////////////////////////////////////////////////////////////////////
	case MSG_EVENT_BJMONO_2007:
		{
			BYTE subtype;

			(*istr) >> subtype;

			CUIMsgBox_Info MsgBoxInfo;
			CTString strMessage;
			ULONG ulCount;

			switch (subtype)
			{
			case MSG_EVENT_BJMONO_2007_TICKET_REQ:
				{
					(*istr) >> ulCount; // 응모권 갯수

					MsgBoxInfo.SetMsgBoxInfo(_S(191,"확인"), UMBS_OK, UI_NONE, MSGCMD_NULL);
					strMessage.PrintF(_s("%d장의 응모권을 받았습니다."), ulCount);
					MsgBoxInfo.AddString(strMessage);
					pUIManager->CreateMessageBox(MsgBoxInfo);
				}
				break;
			case MSG_EVENT_BJMONO_2007_CHANGE_TICKET_REQ:
				{
					pUIManager->CloseMessageBox(MSGCMD_BJMONO_2007_CHANGE_REQ);

					strMessage.PrintF(_s("응모권 교환 성공"));
					MsgBoxInfo.SetMsgBoxInfo(_S(191,"확인"), UMBS_OK, UI_NONE, MSGCMD_NULL);
					MsgBoxInfo.AddString(strMessage);
					pUIManager->CreateMessageBox(MsgBoxInfo);
				}
				break;
			case MSG_EVENT_BJMONO_2007_NO_TICKET:
				{
					_pNetwork->ClientSystemMessage(_s("받으실 응모권이 없습니다."), SYSMSG_ERROR);					
				}
				break;
			case MSG_EVENT_BJMONO_2007_DONT_CHANGE_TICKET:
				{
					_pNetwork->ClientSystemMessage( _s("교환 받을 응모권이 없습니다."), SYSMSG_ERROR );
				}
				break;
			case MSG_EVENT_BJMONO_2007_PRICE_FULLINVEN:
				{
					_pNetwork->ClientSystemMessage(_s("인벤토리가 가득차서 보상을 받을 수 없습니다."), SYSMSG_ERROR);
				}
			}
		}
		break;		
	case MSG_EVENT_SSHOT_2007 : // WSS_TG2007 2007/09/14
		{
			UBYTE subtype;
			(*istr) >> subtype;

			CUIMsgBox_Info MsgBoxInfo;
			CTString strMessage;			

			switch (subtype)
			{
			case MSG_EVENT_SSHOT_2007_RECIVE:
				{
					MsgBoxInfo.SetMsgBoxInfo(_S(191,"확인"), UMBS_OK, UI_NONE, MSGCMD_NULL);
					strMessage.PrintF(_s("송편 10개를 받았습니다."));
					MsgBoxInfo.AddString(strMessage);
					pUIManager->CreateMessageBox(MsgBoxInfo);
				}
				break;
			case MSG_EVENT_SSHOT_2007_ALREADY:
				{
					MsgBoxInfo.SetMsgBoxInfo(_S(191,"확인"), UMBS_OK, UI_NONE, MSGCMD_NULL);
					strMessage.PrintF(_S( 673,"이벤트 아이템이 이미 지급되었습니다."));
					MsgBoxInfo.AddString(strMessage);
					pUIManager->CreateMessageBox(MsgBoxInfo);
				}
				break;
			case MSG_EVENT_SSHOT_2007_NOSSHOT:					// 응모한적 없음
				{
					MsgBoxInfo.SetMsgBoxInfo(_S(191,"확인"), UMBS_OK, UI_NONE, MSGCMD_NULL);
					strMessage.PrintF(_s("송편받기 이벤트에 해당되지 않습니다. 확인하고 다시 시도하여 주십시오."));
					MsgBoxInfo.AddString(strMessage);
					pUIManager->CreateMessageBox(MsgBoxInfo);
				}
				break;
			case MSG_EVENT_SSHOT_2007_FULLINVEN:				// 인벤이 가득 참
				{
					MsgBoxInfo.SetMsgBoxInfo(_S(191,"확인"), UMBS_OK, UI_NONE, MSGCMD_NULL);
					strMessage.PrintF(_S( 116,"인벤토리 공간이 부족하여 보상을 받을 수 없습니다."));
					MsgBoxInfo.AddString(strMessage);
					pUIManager->CreateMessageBox(MsgBoxInfo);
				}
				break;
			}

		}
		break;
	case MSG_EVENT_RICHYEAR : // WSS_TG2007 2007/09/14
		{
			UBYTE subtype;
			(*istr) >> subtype;

			CUIMsgBox_Info MsgBoxInfo;
			CTString strMessage;			
			MsgBoxInfo.SetMsgBoxInfo(_S(191,"확인"), UMBS_OK, UI_NONE, MSGCMD_NULL);

			switch (subtype)
			{				
			case MSG_EVENT_RICHYEAR_ERROR_UPGRADE_NOITEM:
				strMessage.PrintF(_S(1799,"재료가 부족합니다. 확인하고 다시 시도하여 주십시오."));
				break;
			case MSG_EVENT_RICHYEAR_ERROR_UPGRADE_FAILED:
				strMessage.PrintF(_S(1883,"작물 업그레이드에 실패하였습니다."));
				break;
			case MSG_EVENT_RICHYEAR_ERROR_NOCROP:
				strMessage.PrintF(_S(1879,"작물이 없습니다."));
				break;
			case MSG_EVENT_RICHYEAR_ERROR_ALREADY_UPGRADE:
				strMessage.PrintF(_S(2642,"더 이상 업그레이드 할 수 없습니다."));					
				break;
			case MSG_EVENT_RICHYEAR_ERROR_FULLINVEN:
				strMessage.PrintF(_S( 265 ,"인벤토리 공간이 부족합니다." ));					
				break;
			case MSG_EVENT_RICHYEAR_UPGRADE_OK:
				strMessage.PrintF(_S(1882,"작물 업그레이드에 성공하였습니다."));										
				break;
			case MSG_EVENT_RICHYEAR_EXCHAGE_OK:
				strMessage.PrintF(_S(1840,"아이템이 성공적으로 지급 되었습니다"));										
				break;

			}		

			MsgBoxInfo.AddString(strMessage);
			pUIManager->CreateMessageBox(MsgBoxInfo);
		}
		break;
	case MSG_EVENT_LC_1000DAY: // 라스트 카오스 1000일 이벤트
		{
			BYTE subType, noItem;

			(*istr) >> subType;
			CUIMsgBox_Info MsgBoxInfo;
			CTString strMessage;

			switch (subType)
			{
			case MSG_EVENT_LC_1000DAY_HAT_UPDATE_SUCCESS: // 모자 옵션 업데이트 성공
				{
					_pNetwork->ClientSystemMessage(_S(3833, "업그레이드를 성공하였습니다."), SYSMSG_ERROR);
				}
				break;
			case MSG_EVENT_LC_1000DAY_HAT_UPDATE_FAIL: // 모자 옵션 업데이트 실패
				{
					(*istr) >> noItem;

					if (noItem == 1)
					{
						_pNetwork->ClientSystemMessage(_S(3834, "업그레이드할 기념모자 아이템이 없습니다." ), SYSMSG_ERROR);
					}
					else
					{
						_pNetwork->ClientSystemMessage(_S(3835, "업그레이드를 실패하였습니다." ), SYSMSG_ERROR);
						_pNetwork->ClientSystemMessage(_S(3836, "기념모자가 파괴되었습니다." ), SYSMSG_ERROR);
					}
				}
				break;
			case MSG_EVENT_LC_1000DAY_FIRECRACKER_DUPLICATION: // 폭죽 아이템이 인벤에 있음
				{
					MsgBoxInfo.SetMsgBoxInfo(_S(191,"확인"), UMBS_OK, UI_NONE, MSGCMD_NULL);
					MsgBoxInfo.AddString(_S(3837, "폭죽을 다 사용하고 나서 다시 시도하여 주십시오."));
					pUIManager->CreateMessageBox(MsgBoxInfo);
				}
				break;
			case MSG_EVENT_LC_1000DAY_NOITEM: // 아이템 부족
				{
					MsgBoxInfo.SetMsgBoxInfo(_S(191,"확인"), UMBS_OK, UI_NONE, MSGCMD_NULL);
					MsgBoxInfo.AddString(_S(3838, "이벤트 아이템이 부족하여 기념모자를 받을 수 없습니다."));
					pUIManager->CreateMessageBox(MsgBoxInfo);
				}
				break;
			case MSG_EVENT_LC_1000DAY_FULLINVEN: // 인벤이 가득 참
				{
					_pNetwork->ClientSystemMessage(_S( 265 ,"인벤토리 공간이 부족합니다." ), SYSMSG_ERROR);
				}
				break;
			}
		}
		break;
	case MSG_EVENT_HALLOWEEN_2007:	// 할로윈 이벤트 2007
		{
			BYTE subType, subResult;

			(*istr) >> subType;
			(*istr) >> subResult;
			CUIMsgBox_Info MsgBoxInfo;
			CTString strMessage;
			MsgBoxInfo.SetMsgBoxInfo(_S(191, "확인"), UMBS_OK, UI_NONE, MSGCMD_NULL);

			switch (subType)
			{
			case MSG_EVENT_TAKE_DEVILHEAIR:
				{
					switch (subResult)
					{
					case 0: MsgBoxInfo.AddString(_S(3781, "악마 날개 머리띠를 지급 받았습니다!")); break;
					case 1: MsgBoxInfo.AddString(_S(3782, "악마 날개 머리띠를 더이상 받을 수 없습니다!")); break;
					}
				}
				break;
			case MSG_EVENT_CHANGE_CANDYBASKET:
				{
					switch (subResult)
					{
					case 0: MsgBoxInfo.AddString(_S(3783, "사탕을 받았습니다!")); break;
					case 1: MsgBoxInfo.AddString(_S(3784, "바구니가 없어 사탕을 받지 못했습니다!")); break;
					}
				}
				break;
			case MSG_EVENT_CHANGE_PUMKINHEAD:
				{
					switch (subResult)
					{
					case 0: MsgBoxInfo.AddString(_S(3785, "호박 머리탈을 교환 받았습니다!")); break;
					case 1: MsgBoxInfo.AddString(_S(3786, "사탕의 종류가 모자라 교환 받지 못했습니다!")); break;
					}
				}
				break;
			case MSG_EVENT_CHANGE_WITCHHAT:
				{
					switch (subResult)
					{
					case 0: 
						{
							MsgBoxInfo.AddString(_S(5111, "할로윈 스크림 가면을 지급 받았습니다.!"));
						}
						break;
					case 1: MsgBoxInfo.AddString(_S(3788, "증표가 없어 교환을 받지 못했습니다!")); break;
					}
				}
				break;
			case MSG_EVENT_CHANGE_PUMKIN:
				{
					switch (subResult)
					{
					case 0:
						{
							SLONG slItemIndex;
							(*istr) >> slItemIndex;
							strMessage.PrintF(_S(3789, "호박을 쪼개었더니 %s이(가) 나왔습니다!"), _pNetwork->GetItemName(slItemIndex));
							MsgBoxInfo.AddString(strMessage);
						}
						break;
					case 1: MsgBoxInfo.AddString(_S(3790, "너에게 볼일은 없어 썩 꺼져!")); break;
					}
				}
				break;
			case MSG_EVENT_TASTE_CANDY:
				{
					switch (subResult)
					{
					case 0: MsgBoxInfo.AddString(_S(3791, "절망적인 끔직한 맛! 잠시 지옥을 본 것 같아!!")); break;
					case 1: MsgBoxInfo.AddString(_S(3792, "우~웩! 정말! 토 나오는 맛이군!!")); break;
					case 2: MsgBoxInfo.AddString(_S(3793, "그럭저럭 먹을 만 하군!!")); break;
					case 3: MsgBoxInfo.AddString(_S(3794, "오! 의외로 괜찮은 맛 이군!!")); break;
					case 4: MsgBoxInfo.AddString(_S(3795, "아! 천상의 맛이야! 아 날개 달린 당신은...천..사!!")); break;
					}
				}
				break;
			case MSG_EVENT_FULL_INVEN:
				{
					_pNetwork->ClientSystemMessage(_S(3796, "인벤토리가 가득차서 아이템을 받을 수 없습니다."), SYSMSG_ERROR);
					return;
				}
				break;
			}

			pUIManager->CreateMessageBox(MsgBoxInfo);
		}
		break;

		/////////////////////////////////////////////////////////////////////////
		// [071122: Su-won] NEW_MOONSTONE
	case MSG_EVENT_NEW_MOONSTONE:
		{
			CUIMsgBox_Info MsgBoxInfo;
			CTString strMessage;
			MsgBoxInfo.SetMsgBoxInfo(_S(191, "확인"), UMBS_OK, UI_NONE, MSGCMD_NULL);

			unsigned char ucIndex;
			(*istr) >> ucIndex;

			switch ( ucIndex )
			{
			case MSG_EVENT_NEW_MOONSTONE_START_REP:
				{
					LONG lAccumulate;
					(*istr) >> lAccumulate;
					pUIManager->GetGamble()->SetAccumulate( lAccumulate );
					pUIManager->GetGamble()->ShowGamble();
					pUIManager->SetCSFlagOn( CSF_MOONSTONE );
				}
				break;

			case MSG_EVENT_NEW_MOONSTONE_TRY_REP:		// 문스톤 복권 응답
				{
					LONG lRewardItem;
					LONG lAccumulate;
					(*istr) >> lRewardItem;
					(*istr) >> lAccumulate;

					pUIManager->GetGamble()->SetSelectedItem( lRewardItem );
					pUIManager->GetGamble()->Start();
					pUIManager->GetGamble()->SetAccumulate( lAccumulate );
				}
				break;
			case MSG_EVENT_NEW_MOONSTONE_RESULT_REP:	// 문스톤 복권 결과 응답.
				{
					LONG	lRewardItem;					
					(*istr) >> lRewardItem;
					pUIManager->GetGamble()->Stop();

					pUIManager->GetGamble()->ShowPrizeEffect();
				}
				break;

			case MSG_EVENT_NEW_MOONSTONE_JACK_POT:
				{
					pUIManager->GetGamble()->ShowPrizeEffect();
				}
				break;
			case MSG_EVENT_NEW_MOONSTONE_OK:
				{
				}
				break;
			case MSG_EVENT_NEW_MOONSTONE_NO_MOONSTONE:
				{
					MsgBoxInfo.AddString(_S(1439, "문스톤이 없습니다."));
					pUIManager->CreateMessageBox(MsgBoxInfo);

					pUIManager->GetGamble()->ResetGamble();
				}
				break;
			case MSG_EVENT_NEW_MOONSTONE_FULLINVEN:
				{
					MsgBoxInfo.AddString(_S(2850, "인벤토리가 가득 찼습니다."));
					pUIManager->CreateMessageBox(MsgBoxInfo);

					//pUIManager->GetGamble()->SetEnable( FALSE );
					pUIManager->GetGamble()->ResetGamble();
				}
				break;
			case MSG_EVENT_NEW_MOONSTONE_MIX_SUCCESS:
				{
					MsgBoxInfo.AddString(_S(558, "조합에 성공하였습니다."));
					pUIManager->CreateMessageBox(MsgBoxInfo);
				}
				break;
			case MSG_EVENT_NEW_MOONSTONE_MIX_FAILED:
				{
					MsgBoxInfo.AddString(_S(557, "조합에 실패하였습니다."));
					pUIManager->CreateMessageBox(MsgBoxInfo);
				}
				break;
			case MSG_EVENT_NEW_MOONSTONE_MIX_MISS:
				{
					MsgBoxInfo.AddString(_S(2466, "제작에 적합하지 않은 조합입니다."));
					pUIManager->CreateMessageBox(MsgBoxInfo);
				}
				break;
			}
		}
		break;
		// [071122: Su-won] NEW_MOONSTONE
		/////////////////////////////////////////////////////////////////////////
	case MSG_EVENT_XMAS_2007: // X-MAS 2007 eons 071204
		{
			UBYTE ubSucess;
			UBYTE ubType;
			SLONG slRemain;

			CUIMsgBox_Info MsgBoxInfo;
			CTString strMessage;
			MsgBoxInfo.SetMsgBoxInfo(_S(191, "확인"), UMBS_OK, UI_NONE, MSGCMD_NULL);

			(*istr) >> ubType;

			switch (ubType)
			{
			case MSG_EVENT_XMASTREE_ADD_POINT: // 장식
				{
					(*istr) >> ubSucess;

					if (ubSucess == 1)
					{
						MsgBoxInfo.AddString(_S(3954, "크리스마스 트리를 장식할 아이템이 없습니다"));
						pUIManager->CreateMessageBox(MsgBoxInfo);
					}
				}
				break;
			case MSG_EVENT_XMASTREE_GET_BLESS: // 축복
				{
					(*istr) >> ubSucess;

					if (ubSucess == 1)
					{
						MsgBoxInfo.AddString(_S(3955, "지금은 크리스마스 축복을 받을 수 없습니다. 잠시 후 다시 시도하여주십시오"));
						pUIManager->CreateMessageBox(MsgBoxInfo);
					}
				}
				break;
			case MSG_EVENT_XMASTREE_DROP_ITEM: // 선물 공지
				{
					(*istr) >> slRemain;

					strMessage.PrintF(_S(3956, "%d분 뒤 크리스마스 트리주위에 크리스마스 선물이 놓여지게 됩니다"), slRemain);
					_UIAutoHelp->SetGMNotice(strMessage, 0xFFAA33FF);
				}
				break;
			case MSG_EVENT_XMAS_PUZZLE_CHECKITEM_REP:
				{
					(*istr) >> ubSucess;
					if (ubSucess == 0)
					{
						pUIManager->GetMinigame()->OpenMinigame(MSG_EVENT_XMAS_2007);
					}
					else
					{
						if (pUIManager->DoesMessageBoxExist(MSGCMD_NULL))
							return;
						CUIMsgBox_Info msgInfo;
						CTString strMessage = _S(191, "확인");
						msgInfo.SetMsgBoxInfo(strMessage, UMBS_OK, UI_NONE, MSGCMD_NULL);
						strMessage.PrintF(_S(5813, "어라, 산타클로스 인형이 없네요? 산타클로스 인형이 없으면 가위바위보를 할 수 없어요. 게임을 하고 싶다면, 조각을 1번부터 9번까지를 모두 모아 완성된 산타클로스를 가져와야 해요."));
						msgInfo.AddString(strMessage);
						pUIManager->CreateMessageBox(msgInfo);
					}
				}
				break;
			case MSG_EVENT_XMAS_PUZZLE_START_REP:
				{
					(*istr) >> ubSucess;
					if (ubSucess == 0)
					{
						pUIManager->GetMinigame()->SetButtonState(BTN_STATE_START_CONTINUE);
					}
					else
					{
						if (pUIManager->DoesMessageBoxExist(MSGCMD_NULL))
							return;
						CUIMsgBox_Info msgInfo;
						CTString strMessage = _S(191, "확인");
						msgInfo.SetMsgBoxInfo(strMessage, UMBS_OK, UI_NONE, MSGCMD_NULL);
						strMessage.PrintF(_S(5813, "어라, 산타클로스 인형이 없네요? 산타클로스 인형이 없으면 가위바위보를 할 수 없어요. 게임을 하고 싶다면, 조각을 1번부터 9번까지를 모두 모아 완성된 산타클로스를 가져와야 해요."));
						msgInfo.AddString(strMessage);
						pUIManager->CreateMessageBox(msgInfo);
					}
				}
				break;
			case MSG_EVENT_XMAS_PUZZLE_SELECT_REP:
				{
					UBYTE result, winCount, userSelect, npcSelect;
					(*istr) >> result >> winCount >> userSelect >> npcSelect;
					pUIManager->GetMinigame()->SetMinigameResult(winCount, userSelect, npcSelect, result);
				}
				break;
			case MSG_EVENT_XMAS_PUZZLE_CONTINUE_REP:
				{
					pUIManager->GetMinigame()->SetButtonState(BTN_STATE_START_CONTINUE);
				}
				break;
			case MSG_EVENT_XMAS_PUZZLE_END_REP:
				{
					UBYTE ubWinCount;
					SLONG slItemIndex;
					LONGLONG llItemCount;
					(*istr) >> ubWinCount >> slItemIndex >> llItemCount;
					if (slItemIndex != -1 && llItemCount != -1)
					{
						CUIMsgBox_Info msgInfo;
						CTString strMessage = _S(191, "확인");
						msgInfo.SetMsgBoxInfo(strMessage, UMBS_OK, UI_NONE, MSGCMD_NULL);
						strMessage.PrintF(_S(1840, "아이템이 성공적으로 지급 되었습니다"));
						msgInfo.AddString(strMessage);
						pUIManager->CreateMessageBox(msgInfo);
					}
					pUIManager->GetMinigame()->Close();
				}
				break;
			}
		}
		break;
		// [071211: Su-won] EVENT_NEWYEAR_2008
	case MSG_EVENT_NEWYEAR_2008:
		{
			INDEX iChar;
			CTString strCharName;

			(*istr) >> iChar;
			(*istr) >> strCharName;

			CTString strMsg;
			strMsg.PrintF(_S(3965, "%s님으로부터 새해복 많이 받으세요 새해인사가 도착하였습니다."), strCharName);
			_pNetwork->ClientSystemMessage( strMsg, SYSMSG_ERROR );

			CUIMsgBox_Info MsgBoxInfo;
			MsgBoxInfo.SetMsgBoxInfo(_S(191, "확인"), UMBS_OKCANCEL, UI_NONE, MSGCMD_EVENT_NEWYEAR_2008);
			MsgBoxInfo.AddString(_S(3966, "해맞이 버프가 시전되었습니다. 수락하시겠습니까?"));
			pUIManager->CreateMessageBox(MsgBoxInfo);
		}
		break;
	case MSG_EVENT_SAKURA_2008:
		{
			UBYTE ubType;
			(*istr) >> ubType;

			switch (ubType)
			{
			case MSG_EVENT_SAKURAEVENT_2008_ERROR:
				{
					UBYTE ubError;
					(*istr) >> ubError;

					if( ubError == 0 )
					{

					}
					else if( ubError == 1 )
					{
						CUIMsgBox_Info MsgBoxInfo;
						MsgBoxInfo.SetMsgBoxInfo(_S(191, "확인"), UMBS_OK, UI_NONE, MSGCMD_NULL);

						MsgBoxInfo.AddString(_S(4114, "퇴비를 가지고 있지 않습니다."));
						pUIManager->CreateMessageBox(MsgBoxInfo);

						return;
					}
				}
				break;
			case MSG_EVENT_SAKURAEVENT_2008_ITEMCHANGE:
				{
					CTString strName;
					INDEX iItemIndex;

					(*istr) >> strName;
					(*istr) >> iItemIndex;

					CTString strNotice;
					strNotice.PrintF( _S(4115, "%s님이 벚꽃선물 상자를 열어 %s을 획득하였습니다."), strName, _pNetwork->GetItemName(iItemIndex));

					_UIAutoHelp->SetGMNotice( strNotice );

					return;
				}
				break;
			}
		}
		break;
	case MSG_EVENT_NOM:
		{
			UBYTE ubType;
			(*istr) >> ubType;

			switch(ubType)
			{
			case MSG_EVENT_NOM_OK:						// 응모 성공
				{
					CUIMsgBox_Info MsgBoxInfo;
					MsgBoxInfo.SetMsgBoxInfo(_S(191, "확인"), UMBS_OK, UI_NONE, MSGCMD_NULL);

					MsgBoxInfo.AddString(_S(4150, "놈놈놈 영화티켓을 잡아라! 이벤트에 응모되었습니다."));
					pUIManager->CreateMessageBox(MsgBoxInfo);

					return;
				}
				break;
			case MSG_EVENT_NOM_ERROR_ALREADY_APPLY:		// 이미 응모 함
				break;
			case MSG_EVENT_NOM_ERROR_FULL_INVEN:		// 인벤토리가 꽉참
				{
					CUIMsgBox_Info MsgBoxInfo;
					MsgBoxInfo.SetMsgBoxInfo(_S(191, "확인"), UMBS_OK, UI_NONE, MSGCMD_NULL);

					MsgBoxInfo.AddString(_S(4151, "인벤토리 공간이 부족하여 복권을 지급받지 못하였습니다. 공간을 확보하고 접속을 다시 시도하여 주십시오."));
					pUIManager->CreateMessageBox(MsgBoxInfo);

					return;
				}
				break;
			case MSG_EVENT_NOM_ERROR_ITEM_FAIL:			// 아이템 오류
				break;
			case MSG_EVENT_NOM_LOTTERY_USE_OK:			// 복권 사용 성공
				break;
			case MSG_EVENT_NOM_MOVIE_TICKET_OK:
				{
					CUIMsgBox_Info MsgBoxInfo;
					MsgBoxInfo.SetMsgBoxInfo(_S(191, "확인"), UMBS_OK, UI_NONE, MSGCMD_NULL);

					MsgBoxInfo.AddString(_S(4152, "놈놈놈 영화 티켓이 등록되었습니다."));
					pUIManager->CreateMessageBox(MsgBoxInfo);

					return;
				}
				break;
			}
		}
		break;
	case MSG_EVENT_PHOENIX:
		{
			UBYTE ubType;
			(*istr) >> ubType;
			switch(ubType)
			{
			case MSG_EVENT_PHOENIX_ERROR_ALREADY_APPLY:
				{
					CUIMsgBox_Info MsgBoxInfo;
					MsgBoxInfo.SetMsgBoxInfo(_S(191, "확인"), UMBS_OK, UI_NONE, MSGCMD_NULL);

					MsgBoxInfo.AddString(_S(4235, "이미 피닉스로 등록되어 있습니다."));
					pUIManager->CreateMessageBox(MsgBoxInfo);

					return;
				}
				break;
			case MSG_EVENT_PHOENIX_ERROR_REQUIREMENT:
				{
					CUIMsgBox_Info MsgBoxInfo;
					MsgBoxInfo.SetMsgBoxInfo(_S(191, "확인"), UMBS_OK, UI_NONE, MSGCMD_NULL);

					MsgBoxInfo.AddString(_S(4236, "피닉스가 될 자격이 되지 않습니다."));
					pUIManager->CreateMessageBox(MsgBoxInfo);

					return;
				}
				break;
			case MSG_EVENT_PHOENIX_ERROR_FULL_INVENTORY:
				{
					CUIMsgBox_Info MsgBoxInfo;
					MsgBoxInfo.SetMsgBoxInfo(_S(191, "확인"), UMBS_OK, UI_NONE, MSGCMD_NULL);

					MsgBoxInfo.AddString(_S(4237, "인벤토리의 공간이 부족 합니다. 인벤토리를 비운 후 다시 시도해주시기 바랍니다."));
					pUIManager->CreateMessageBox(MsgBoxInfo);

					return;
				}
				break;
			case MSG_EVENT_PHOENIX_OK:
				{
					if(pUIManager->GetQuest()->GetNpcIndex() == 834)
					{
						FLOAT fNPCPosX = pUIManager->GetQuest()->GetNpcPosX();
						FLOAT fNPCPosZ = pUIManager->GetQuest()->GetNpcPosZ();

						CUIMsgBox_Info	MsgBoxInfo;
						MsgBoxInfo.SetMsgBoxInfo(_S(191, "확인"), UMBS_OKCANCEL, UI_NONE, MSGCMD_EVENT_PHOENIX, -1, fNPCPosX, fNPCPosZ);
						MsgBoxInfo.AddString(_S(4238, "피닉스가 될 자격이 충분합니다. 가입하시겠습니까?"));
						pUIManager->CreateMessageBox( MsgBoxInfo );		

					}
					return;
				}
				break;
			}
		}
		break;
	case MSG_EVENT_PHOENIX_SUCCESS:
		{
			CUIMsgBox_Info	MsgBoxInfo;
			MsgBoxInfo.SetMsgBoxInfo(_S(191, "확인"), UMBS_OK, UI_NONE, MSGCMD_NULL);
			MsgBoxInfo.AddString(_S(4239, "피닉스로 등록되었습니다."));
			pUIManager->CreateMessageBox( MsgBoxInfo );

			return;			
		}
		break;
	case MSG_EVENT_REQUITAL:
		{
			UBYTE ubType;
			(*istr) >> ubType;

			CUIMsgBox_Info	MsgBoxInfo;
			MsgBoxInfo.SetMsgBoxInfo(_S(191, "확인"), UMBS_OK, UI_NONE, MSGCMD_NULL);

			switch(ubType)
			{
			case MSG_EVENT_REQUITALM_OK:						// 보상 성공
				{
					MsgBoxInfo.AddString(_S(1840,"아이템이 성공적으로 지급 되었습니다"));

				}break;
			case MSG_EVENT_REQUITAL_FAIL:					// 보상 실패
			case MSG_EVENT_REQUITAL_FAIL_TIME:				// 보상 시간이 지나감
			case MSG_EVENT_REQUITAL_FAIL_TIME_NOPLAY:		// 해당 날짜에 게임을 하지 않아서 실패
			case MSG_EVENT_REQUITAL_FAIL_CREATE_TIME:		// 유저가 게임을 시작한 날짜가 너무 이름
				{
					MsgBoxInfo.AddString(_S(1722, "보상을 받으실 수 있는 조건이 아닙니다."));
				}break;
			case MSG_EVENT_REQUITAL_ERROR_ALREADY_APPLY:		// 이미 응모 함
				{
					MsgBoxInfo.AddString(_S(673, "이벤트 아이템이 이미 지급되었습니다."));
				}break;
			case MSG_EVENT_REQUITAL_ERROR_FULL_INVEN:		// 인벤토리가 꽉참				
				{
					MsgBoxInfo.AddString(_S(4237, "인벤토리의 공간이 부족 합니다. 인벤토리를 비운 후 다시 시도해주시기 바랍니다."));
				}break;
			}		
			pUIManager->CreateMessageBox( MsgBoxInfo );

			return;
		}
		break;
	case MSG_EVENT_NEW_AFRON_2009:
		{
			UBYTE ubType;
			(*istr) >> ubType;

			if (ubType == MSG_EVENT_GIVE_ITEM_SUCCESS)
			{
				CUIMsgBox_Info	MsgBoxInfo;
				MsgBoxInfo.SetMsgBoxInfo(_S(191, "확인"), UMBS_OK, UI_NONE, MSGCMD_NULL);
				MsgBoxInfo.AddString(_S(1840,"아이템이 성공적으로 지급 되었습니다"));
				pUIManager->CreateMessageBox(MsgBoxInfo);
			}
		}
		break;

		// ($E_WC2010) [100511: selo] 2010 남아공 월드컵
	case MSG_EVENT_WORLDCUP_2010:
		{
			UBYTE ubType;
			UBYTE ubError;
			(*istr) >> ubType;

			switch(ubType)
			{
			case MSG_EVENT_WORLDCUP2010_TRADE_REP:				// 축구공 교환 답변
				{					
					(*istr) >> ubError;					

					switch(ubError)
					{
					case MSG_EVENT_WORLDCUP2010_TRADE_ERROR_SUC:	// 축구공 교환 성공
						{
							INDEX ubTradeType;
							(*istr) >> ubTradeType;

							if( ubTradeType ==  1)
							{
								if( pUIManager->DoesMessageBoxLExist(MSGLCMD_EVENT_WORLDCUP_EVENT1) )
									pUIManager->CloseMessageBoxL(MSGLCMD_EVENT_WORLDCUP_EVENT1);

								pUIManager->CreateMessageBoxL( _S(4879, "황금 축구공으로 교환"), UI_SHOP, MSGLCMD_EVENT_WORLDCUP_EVENT1);

								pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_WORLDCUP_EVENT1, TRUE, _S(4880, "계산은 정확하오. 다음에 또 들러주시오."),-1,0xa3a1a3ff);
								pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_WORLDCUP_EVENT1, FALSE, _S( 191, "확인" ) );
							}
							else
							{
								if( pUIManager->DoesMessageBoxLExist(MSGLCMD_EVENT_WORLDCUP_EVENT1) )
									pUIManager->CloseMessageBoxL(MSGLCMD_EVENT_WORLDCUP_EVENT1);

								pUIManager->CreateMessageBoxL( _S(4881, "월드 축제 상자로 교환"), UI_SHOP, MSGLCMD_EVENT_WORLDCUP_EVENT1);

								pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_WORLDCUP_EVENT1, TRUE, _S(4882, "축하하오. 이벤트 기간 동안에는 언제든지 찾아 주시구려."),-1,0xa3a1a3ff);
								pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_WORLDCUP_EVENT1, FALSE, _S( 191, "확인" ) );
							}
						}
						break;
					case MSG_EVENT_WORLDCUP2010_TRADE_ERROR_BALL:	// 축구공 갯수 부족 
						{
							INDEX nTradeType;
							(*istr) >> nTradeType;

							if( nTradeType ==  1)
							{
								if( pUIManager->DoesMessageBoxLExist(MSGLCMD_EVENT_WORLDCUP_EVENT1) )
									pUIManager->CloseMessageBoxL(MSGLCMD_EVENT_WORLDCUP_EVENT1);

								pUIManager->CreateMessageBoxL( _S(4879, "황금 축구공으로 교환"), UI_SHOP, MSGLCMD_EVENT_WORLDCUP_EVENT1);

								pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_WORLDCUP_EVENT1, TRUE, _S(4898, "축구공 수량이 부족하여 황금 축구공으로 바꿔 드리긴 힘들 것 같소. 좀 더 구해오시오."),-1,0xa3a1a3ff);
								pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_WORLDCUP_EVENT1, FALSE, _S( 191, "확인" ) );
							}
							else
							{
								if( pUIManager->DoesMessageBoxLExist(MSGLCMD_EVENT_WORLDCUP_EVENT1) )
									pUIManager->CloseMessageBoxL(MSGLCMD_EVENT_WORLDCUP_EVENT1);

								pUIManager->CreateMessageBoxL( _S(4881, "월드 축제 상자로 교환"), UI_SHOP, MSGLCMD_EVENT_WORLDCUP_EVENT1);

								pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_WORLDCUP_EVENT1, TRUE, _S(4899, "에헴~ 난 자선 사업가가 아니오. 50개! 50개! 정도는 내게 주어야 하오."),-1,0xa3a1a3ff);
								pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_WORLDCUP_EVENT1, FALSE, _S( 191, "확인" ) );
							}
						}
						break;
					case MSG_EVENT_WORLDCUP2010_TRADE_ERROR_FAIL:	// 축구공 교환 실패
						{
							// Do Nothing
						}
						break;
					}

				}
				break;

			case MSG_EVENT_WORLDCUP2010_TOTO_REP:				// 승리국가 TOTO 등록 답변
				{
					(*istr) >> ubError;
					switch( ubError )
					{
					case MSG_EVENT_WORLDCUP2010_TOTO_ERROR_SUC:
						{
							if( pUIManager->DoesMessageBoxExist(MSGCMD_EVENT_WORLDCUP_EVENT2_STATUS) )
								pUIManager->CloseMessageBox(MSGCMD_EVENT_WORLDCUP_EVENT2_STATUS);

							CUIMsgBox_Info	MsgBoxInfo;
							MsgBoxInfo.SetMsgBoxInfo(_S(4921, "이벤트 응모"), UMBS_OK, UI_QUEST, MSGCMD_EVENT_WORLDCUP_EVENT2_STATUS);
							MsgBoxInfo.AddString(_S(4922, "이벤트 응모 성공"));
							pUIManager->CreateMessageBox(MsgBoxInfo);
						}
						break;
					case MSG_EVENT_WORLDCUP2010_TOTO_ERROR_FAIL:
						{
							if( pUIManager->DoesMessageBoxExist(MSGCMD_EVENT_WORLDCUP_EVENT2_STATUS) )
								pUIManager->CloseMessageBox(MSGCMD_EVENT_WORLDCUP_EVENT2_STATUS);

							CUIMsgBox_Info	MsgBoxInfo;
							MsgBoxInfo.SetMsgBoxInfo(_S(4921, "이벤트 응모"), UMBS_OK, UI_QUEST, MSGCMD_EVENT_WORLDCUP_EVENT2_STATUS);
							MsgBoxInfo.AddString(_S(4923, "이벤트 응모 실패"));
							pUIManager->CreateMessageBox(MsgBoxInfo);
						}
						break;
					}
				}
				break;

			case MSG_EVENT_WORLDCUP2010_TOTO_STATUS_REP:		// TOTO 응모 현황 답변
				{
					(*istr) >> ubError;
					switch( ubError )
					{
					case MSG_EVENT_WORLDCUP2010_TOTO_STATUS_ERROR_SUC:
						{
							INDEX nResultType = 0;
							INDEX nTotoType = 0;
							INDEX nCountryItem1 = 0;
							INDEX nCountryItem2 = 0;

							(*istr) >> nResultType;							
							(*istr) >> nCountryItem1;							

							switch( nResultType )
							{
							case EVENT_WORLDCUP_2010_EVENT2_SEL2:
								{
									if( pUIManager->DoesMessageBoxExist(MSGCMD_EVENT_WORLDCUP_EVENT2_STATUS) )
										pUIManager->CloseMessageBox(MSGCMD_EVENT_WORLDCUP_EVENT2_STATUS);

									CTString strDesc;
									CTString strDesc2;
									int nBtnCnt = 1;
									CUIMsgBox_Info	MsgBoxInfo;
									MsgBoxInfo.SetMsgBoxInfo(_S(4911, "내 응모 현황"), UMBS_OK | UMBS_BUTTONEX, UI_QUEST, MSGCMD_EVENT_WORLDCUP_EVENT2_STATUS);
									strDesc.PrintF(_S(4924, "응모 방식 : %s"), _S(4908, "우승 국가 응모"));
									strDesc2.PrintF(_S(4925, "등록 국가 : %s"), _pNetwork->GetItemName(nCountryItem1));

									MsgBoxInfo.SetBtnType(UBET_ITEM, CItemData::ITEM_ETC, CItemData::ITEM_ETC_EVENT, nBtnCnt);
									MsgBoxInfo.AddString(strDesc);
									MsgBoxInfo.AddString(strDesc2);
									pUIManager->CreateMessageBox(MsgBoxInfo);								

									CUIMessageBox* pMsgBox = pUIManager->GetMessageBox(MSGCMD_EVENT_WORLDCUP_EVENT2_STATUS);

									if( pMsgBox )
									{
										//pMsgBox->SetBtnEx(0, btn);
										//pMsgBox->GetBtnEx(0).SetEnable(FALSE);
										pMsgBox->GetBtnEx()->setData(UBET_ITEM, nCountryItem1);
									}									
								}
								break;
							default:
								{
									CTString strTitle;
									CTString strDesc;
									int nCommand;

									strTitle = _S(4908, "우승 국가 응모");
									strDesc.PrintF(_S(4927, "이미 우승 국가 응모 방법으로 %s를 등록하셨습니다. 변경하시겠습니까?"), _pNetwork->GetItemName(nCountryItem1));
									nCommand = MSGCMD_EVENT_WORLDCUP_EVENT2_REREQ;

									if( pUIManager->DoesMessageBoxExist(nCommand) )
										pUIManager->CloseMessageBox(nCommand);

									CUIMsgBox_Info	MsgBoxInfo;
									MsgBoxInfo.SetMsgBoxInfo(strTitle, UMBS_OKCANCEL, UI_QUEST, nCommand);
									MsgBoxInfo.AddString(strDesc);
									pUIManager->CreateMessageBox(MsgBoxInfo);
								}
								break;
							}
						}
						break;
					case MSG_EVENT_WORLDCUP2010_TOTO_STATUS_ERROR_FAIL:
						{
							INDEX nResultType;
							(*istr) >> nResultType;

							switch( nResultType )
							{							
							case EVENT_WORLDCUP_2010_EVENT2_SEL2:		// 응모 현황
								{
									if( pUIManager->DoesMessageBoxExist(MSGCMD_EVENT_WORLDCUP_EVENT2_STATUS) )
										pUIManager->CloseMessageBox(MSGCMD_EVENT_WORLDCUP_EVENT2_STATUS);

									CUIMsgBox_Info MsgBoxInfo;
									MsgBoxInfo.SetMsgBoxInfo(_S(4911, "내 응모 현황"), UMBS_OK, UI_QUEST, MSGCMD_EVENT_WORLDCUP_EVENT2_STATUS);

									MsgBoxInfo.AddString(_S(4930, "현재 이벤트 참여 정보가 존재하지 않습니다."));
									pUIManager->CreateMessageBox(MsgBoxInfo);									
								}
								break;
							default:
								{
									pUIManager->GetQuest()->CreateMessageBox_WorldCup2010_Event2(nResultType);
								}
								break;
							}
						}
						break;
					}
				}
				break;

			case MSG_EVENT_WORLDCUP2010_TOTO_GIFT_REP:			// TOTO 상품 요청 답변
				{
					CTString strMessage;

					(*istr) >> ubError;
					switch( ubError )
					{
					case MSG_EVENT_WORLDCUP2010_TOTO_GIFT_ERROR_SUC:
						{
							strMessage = _S(2926, "성공적으로 보상품을 수령하였습니다.");
						}
						break;
					case MSG_EVENT_WORLDCUP2010_TOTO_GIFT_ERROR_INVEN:
						{
							strMessage = _S(265, "인벤토리 공간이 부족합니다.");
						}
						break;
					case MSG_EVENT_WORLDCUP2010_TOTO_GIFT_ERROR_FAIL:
						{
							strMessage = _S(1845, "아이템 지급이 실패 하였습니다.");
						}
						break;
					}

					if( pUIManager->DoesMessageBoxExist(MSGCMD_EVENT_WORLDCUP_EVENT2_STATUS) )
						pUIManager->CloseMessageBox(MSGCMD_EVENT_WORLDCUP_EVENT2_STATUS);

					CUIMsgBox_Info MsgBoxInfo;
					MsgBoxInfo.SetMsgBoxInfo(_S(4912, "당첨 보상 받기" ), UMBS_OK, UI_QUEST, MSGCMD_EVENT_WORLDCUP_EVENT2_STATUS);					
					MsgBoxInfo.AddString(strMessage);
					pUIManager->CreateMessageBox(MsgBoxInfo);
				}
				break;

			case MSG_EVENT_WORLDCUP2010_ATTENDANCE_REP:			// 출석 이벤트 답변
				{
					CTString strMessage;

					(*istr) >> ubError;
					switch( ubError )
					{
					case MSG_EVENT_WORLDCUP2010_ATTENDANCE_ERROR_SUC:		// 출석 체크 성공
						{							
							strMessage = _S(4943, "응원 감사합니다. 또 봐요~!");							
						}
						break;
					case MSG_EVENT_WORLDCUP2010_ATTENDANCE_ERROR_ALREADY:	// 이미 출석 됨	
						{
							strMessage = _S(4944, "이미 받으셨네요. 오늘은 더 이상 드릴 수 없어요~!");							
						}
						break;
					case MSG_EVENT_WORLDCUP2010_ATTENDANCE_ERROR_FAIL:		// 출석 체크 실패
						{
							strMessage = _S(4945, "응원카드 받기에 실패 하였습니다.");							
						}
						break;
					}

					if( pUIManager->DoesMessageBoxExist(MSGCMD_EVENT_WORLDCUP_EVENT3) )
						pUIManager->CloseMessageBox(MSGCMD_EVENT_WORLDCUP_EVENT3);

					CUIMsgBox_Info MsgBoxInfo;
					MsgBoxInfo.SetMsgBoxInfo(_S(4937, "응원카드 받기" ), UMBS_OK, UI_QUEST, MSGCMD_EVENT_WORLDCUP_EVENT3);
					MsgBoxInfo.AddString(strMessage);
					pUIManager->CreateMessageBox(MsgBoxInfo);
				}
				break;

			case MSG_EVENT_WORLDCUP2010_SUPPORTCARD_REP:		// 응원 카드 교환 답변
				{
					CTString strMessage;

					(*istr) >> ubError;
					switch( ubError )
					{
					case MSG_EVENT_WORLDCUP2010_SUPPORTCARD_ERROR_SUC:		// 응원 카드 교환 성공
						{
							strMessage = _S(2926, "성공적으로 보상품을 수령하였습니다.");
						}
						break;
					case MSG_EVENT_WORLDCUP2010_SUPPORTCARD_ERROR_CARD:		// 응원 카드 개수 부족
						{
							strMessage = _S(2912, "아이템 개수가 부족합니다.");
						}
						break;
					case MSG_EVENT_WORLDCUP2010_SUPPORTCARD_ERROR_FAIL:		// 응원 카드 교환 실패
						{
							strMessage = _S(1845, "아이템 지급이 실패 하였습니다.");
						}
						break;
					}

					CUIMsgBox_Info MsgBoxInfo;
					MsgBoxInfo.SetMsgBoxInfo(_S(4937, "응원카드 받기" ), UMBS_OK, UI_QUEST, MSGCMD_EVENT_WORLDCUP_EVENT3);
					MsgBoxInfo.AddString(strMessage);
					pUIManager->CreateMessageBox(MsgBoxInfo);
				}
				break;
			}
		}
		break;
	case MSG_EVENT_PROMOTION2:
		{	// 쿠폰 프로모션 이벤트
			UBYTE ubType;
			(*istr) >> ubType;

			switch( ubType )
			{
			case MSG_EVENT_PROMOTION2_COUPON_USE_REP:
				{
					RecievePromotionEventMessage( istr );
				}
				break;
			}
		}
		break;

		// [2011/01/18 : Sora] 출석 이벤트
	case MSG_EVENT_ATTENDANCE_2011:
		{
			UBYTE ubType;
			(*istr) >> ubType;

			switch( ubType )
			{
			case MSG_EVENT_ATTENDANCE_2011_NOTICE:
				{
					SLONG attendanceRate;

					(*istr) >> attendanceRate;

					pUIManager->GetNotice()->ShowAttendanceMessage( attendanceRate );
				}
				break;

			case MSG_EVENT_ATTENDANCE_2011_CALC_RATE:
				{
					SLONG attendanceRate, enchantRate, expSpRate;

					(*istr) >> attendanceRate;
					(*istr) >> enchantRate;
					(*istr) >> expSpRate;

					pUIManager->GetNotice()->ShowAttendanceNotice( attendanceRate, enchantRate, expSpRate );
				}
				break;

			case MSG_EVENT_ATTENDANCE_2011_INDIVIDUAL_REWARD_REP:
				{
					UBYTE ubResult;
					CTString strTemp;

					(*istr) >> ubResult;

					switch( ubResult )
					{
					case 0: // 지급 성공
						{
							strTemp = _S( 1840, "아이템이 성공적으로 지급 되었습니다." );
						}
						break;
					case 1: // 출석 조건 부족
						{
							strTemp = _S( 5346, "출석 대기 버프가 사라진 후 지급 받을 수 있습니다."  );
						}
						break;
					case 2: // 이미 지급되었음
						{
							strTemp = _S(673, "이벤트 아이템이 이미 지급되었습니다.");
						}
						break;
					case 3: // 인벤 공간부족
						{
							strTemp = _S(4237, "인벤토리의 공간이 부족 합니다. 인벤토리를 비운 후 다시 시도해주시기 바랍니다.");
						}
						break;
					case 4: // 보상 받을 날짜가 아님
						{
							strTemp = _S(5347, "보상 받을 수 있는 연속일 수가 아닙니다.");
						}
						break;
					}

					CUIMsgBox_Info	MsgBoxInfo;
					MsgBoxInfo.SetMsgBoxInfo(_S(191, "확인"), UMBS_OK, UI_NONE, MSGCMD_NULL);
					MsgBoxInfo.AddString(strTemp);
					pUIManager->CreateMessageBox(MsgBoxInfo);
				}
				break;
			}
		}
		break;

		// [2011/01/18 : Sora] 출석 이벤트
	case MSG_EVENT_ENCHANT_RATE:
		{
			UBYTE ubIsStart;
			SLONG attendanceRate;

			(*istr) >> ubIsStart;
			(*istr) >> attendanceRate;

			pUIManager->GetInventory()->GetEventPopUp()->ClearNoticePopUp();

			if( ubIsStart )
			{
				pUIManager->GetInventory()->GetEventPopUp()->SetPercent( attendanceRate );

				pUIManager->GetInventory()->GetEventPopUp()->SetTitle(_S( 100, "이벤트" ));
				pUIManager->GetInventory()->GetEventPopUp()->SetNoticeMessage( _S(5344, "오늘의\n인첸트 증가확률"), 0xFFFF40FF );
			}
		}
		break;
	case MSG_EVENT_USER_COMEBACK:
		{	// 휴면 계정 보상 이벤트
			CTString strMessage;

			UBYTE ubResult;
			(*istr) >> ubResult;

			switch ( ubResult )
			{
			case MSG_EVENT_USER_COMEBACK_ERROR_SUC:
				{	// 지급 성공
					strMessage = _S( 2234, "이벤트 아이템을 지급 받았습니다." );
				}
				break;
			case MSG_EVENT_USER_COMEBACK_ERROR_NO_MORE:
				{	// 이미 받은 상태
					strMessage = _S( 673, "이벤트 아이템이 이미 지급되었습니다." );
				}
				break;
			case MSG_EVENT_USER_COMEBACK_ERROR_NOT_USER:
				{	// 보상 대상이 아님
					strMessage = _S( 5357, "이벤트 대상이 아닙니다." );
				}
				break;
			}

			if ( strMessage.Length() > 0 )
			{
				CUIMsgBox_Info MsgBoxInfo;
				MsgBoxInfo.SetMsgBoxInfo(_S(100, "이벤트" ), UMBS_OK, UI_NONE, MSGCMD_NULL);
				MsgBoxInfo.AddString(strMessage);
				pUIManager->CreateMessageBox(MsgBoxInfo);
			}
		}
		break;
	case MSG_EVENT_CHAR_BIRTHDAY:
		{
			CTString strMessage, strMessageAdd;
			UBYTE ubResult;
			(*istr) >> ubResult;

			switch( ubResult )
			{
			case MSG_EVENT_CHAR_BIRTHDAY_GIFT_REP:
				{
					UBYTE ubErrorType;
					(*istr) >> ubErrorType;
					switch( ubErrorType )
					{
					case MSG_EVENT_CHAR_BIRTHDAY_ERROR_SUC:
						{
							strMessage = _S( 2234, "이벤트 아이템을 지급 받았습니다." );
						}
						break;
					case MSG_EVENT_CHAR_BIRTHDAY_ERROR_FAIL:
						{
							strMessage = _S( 674, "이벤트 아이템을 받을 수 없습니다." );
						}
						break;
					}
				}
				break;
			case MSG_EVENT_CHAR_BIRTHDAY_BDAY_REP:
				{
					INDEX iYear;
					UBYTE ubMonth, ubDay;
					(*istr) >> iYear;

					if ( iYear > 0)
					{
						(*istr) >> ubMonth;
						(*istr) >> ubDay;
						strMessage.PrintF( _S( 5356, "캐릭터 생성일은 %d년 %d월 %d일 입니다."), iYear, (int)ubMonth, (int)ubDay );
					}
				}
				break;
			}
			if ( strMessage.Length() > 0 )
			{
				CUIMsgBox_Info MsgBoxInfo;
				MsgBoxInfo.SetMsgBoxInfo(_S(100, "이벤트" ), UMBS_OK, UI_NONE, MSGCMD_NULL);
				MsgBoxInfo.AddString(strMessage);
				pUIManager->CreateMessageBox(MsgBoxInfo);
			}
		}
		break;
		// [2011/03/04 : Sora] 유럽2국 프로모션 이벤트
	case MSG_EVENT_EUR2_PROMOTION:
		{
			CTString strMessage;
			UBYTE ubResult;

			(*istr) >> ubResult;

			switch( ubResult )
			{
			case MSG_EVENT_EUR2_PROMOTION_SUCCESS:
				{
					strMessage = _S( 1840, "아이템이 성공적으로 지급 되었습니다" );
				}
				break;
			case MSG_EVENT_EUR2_PROMOTION_WRONG_KEY:
				{
					strMessage = _S( 2405, "요청한 쿠폰 번호가 존재하지 않습니다." );
				}
				break;
			case MSG_EVENT_EUR2_PROMOTION_ALREADY_USED:
				{
					strMessage = _S( 2406, "이미 지급받은 쿠폰 번호 입니다." );
				}
				break;
			case MSG_EVENT_EUR2_PROMOTION_LACK_INVENTORY:
				{
					strMessage = _S( 675, "인벤토리 공간이 부족해서 이벤트 아이템을 받을 수 없습니다." );
				}
				break;
			case MSG_EVENT_EUR2_PROMOTION_SYS_ERROR_NOT_USED:
				{
					strMessage = _S(1845, "아이템 지급이 실패 하였습니다.");
				}
				break;
			case MSG_EVENT_EUR2_PROMOTION_CANT_USE_USER:
				{
					strMessage = _S( 673, "이벤트 아이템이 이미 지급되었습니다." );
				}
				break;
			}
			if ( strMessage.Length() > 0 )
			{
				CUIMsgBox_Info MsgBoxInfo;
				MsgBoxInfo.SetMsgBoxInfo(_S(100, "이벤트" ), UMBS_OK, UI_NONE, MSGCMD_NULL);
				MsgBoxInfo.AddString(strMessage);
				pUIManager->CreateMessageBox(MsgBoxInfo);
			}
		}
		break;
	case MSG_EVENT_MUSICAL_ZORRO:
		{
			CUIMsgBox_Info MsgBoxInfo;
			MsgBoxInfo.SetMsgBoxInfo(_S(100, "이벤트" ), UMBS_OK, UI_NONE, MSGCMD_NULL);
			MsgBoxInfo.AddString(_S( 5591, "뮤지컬 조로관람권 응모이벤트에 정상적으로 응모하였습니다."));
			pUIManager->CreateMessageBox(MsgBoxInfo);
		}
		break;
	}
}
