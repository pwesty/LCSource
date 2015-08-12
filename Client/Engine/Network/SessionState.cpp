#include "stdh.h"
#include <Engine/Base/ErrorTable.h>
#include <Engine/Base/MemoryTracking.h>
#include <Engine/Base/Shell.h>
#include <Engine/Base/ListIterator.inl>
#include <Engine/Base/Synchronization.h>
#include <Engine/Math/Float.h>
#include <Engine/Network/CNetwork.h>
#include <Engine/Network/Server.h>
#include <Engine/Network/NetworkProfile.h>
#include <Engine/Network/ClientInterface.h>
#include <Engine/Network/SessionState.h>
#include <Engine/Ska/Render.h>
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Network/LevelChange.h>
#include <Engine/Network/tcpipconnection.h>
#include <Engine/Entities/InternalClasses.h>
#include <Engine/Sound/SoundObject.h>
#include <Engine/Sound/SoundLibrary.h>
#include <Engine/Templates/Stock_CEntityClass.h>
#include <Engine/Templates/DynamicContainer.cpp>
#include <Engine/GlobalDefinition.h>
#include <Engine/GameState.h>
#include <Engine/SlaveInfo.h>
#include <Engine/Interface/UIManager.h>
#include <Engine/Interface/UIPetTraining.h>
#include <Engine/Interface/UIPetInfo.h>
#include <Engine/Interface/UISelectWord.h>
#include <Engine/Interface/UISingleBattle.h>
#include <Engine/Interface/UIAutoHelp.h>
#include <Engine/Interface/UISiegeWarfareDoc.h>
#include <Engine/Contents/Base/UINoticeNew.h>
#include <Engine/Interface/UISummon.h>
#include <Engine/Interface/UITeleport.h>
#include <Engine/Interface/UIShop.h>
#include <include/F_CSocket_Event.h>
#include <Engine/Interface/UICharServerMove.h>
#include <Engine/Interface/UIQuickSlot.h>
#include <Engine/Interface/UIChildQuickSlot.h>
#include <Engine/Interface/UIFortune.h>
#include <Engine/Interface/UISelectList.h>
#include <Engine/Interface/UISystemMenu.h>
#include <Engine/Interface/UIFlowerTree.h>
#include <Engine/Interface/UIGuildBattle.h>
#include <Engine/Interface/UIMap.h>
#include <Engine/Interface/UISkillLearn.h>
#include <Engine/Interface/UIInventory.h>
#include <Engine/Interface/UIProduct.h>
#include <Engine/Interface/UIExchange.h>
#include <Engine/Interface/UIGWMix.h>
#include <Engine/Interface/UIWareHouse.h>
#include <Engine/Contents/Base/UIPartyNew.h>
#include <Engine/Contents/Base/UICharacterInfoNew.h>
#include <Engine/Interface/UIHelper.h>
#include <Engine/Interface/UIPlayerInfo.h>
#include <Engine/Contents/Base/UIChangeWeaponNew.h>
#include <Engine/Contents/Base/UIChangeEquipment.h>
#include <Engine/Interface/UIGuildWarPortal.h>
#include <Engine/Interface/UIRefine.h>
#include <Engine/Contents/function/TargetInfoNewUI.h>
#include <Engine/Interface/UIGamble.h>
#include <Engine/Contents/Base/UIQuestNew.h>
#include <Engine/Interface/UIMinigame.h>
#include <Engine/Interface/UIRanking.h>
#include <Engine/Interface/UICollectBox.h>
#include <Engine/Interface/UIGuild.h>
#include <Engine/Interface/UIGuildStash.h>
#include <Engine/Interface/UIInitJob.h>
#include <Engine/Interface/UIMessenger.h>
#include <Engine/Interface/UIPersonalShop.h>
#include <Engine/Interface/UIMixNew.h>
#include <Engine/Interface/UIWildPetInfo.h>
#include <Engine/Interface/UISiegeWarfareNew.h>
#include <Engine/Interface/UINpcScroll.h>
#include <Engine/Interface/UINickName.h>
#include <Engine/Interface/UIRadar.h>
#include <Engine/Interface/UISocketSystem.h>
#include <Engine/Interface/UIMix.h>
#include <Engine/Interface/UICompound.h>
#include <Engine/Interface/UIProcessNPC.h>
#include <Engine/Interface/UIOXQuizEvent.h>
#include <Engine/Interface/UIBingoBox.h>
#include <Engine/Interface/UISelectResource.h>
#include <Engine/Interface/UIInventory.h>
#include <Engine/GameDataManager/GameDataManager.h>
#include <Engine/Effect/CEffectGroupManager.h>
#include <Engine/Effect/CEffectGroup.h>
#include <Engine/Contents/Base/Auction.h>
#include <Engine/Contents/Base/UISkillNew.h>
#include <Engine/Contents/Base/ExpressSystem.h>
#include <Engine/Contents/Base/PetStash.h>
#include <Engine/Contents/Base/Calendar.h>
#include <Engine/Contents/Base/Syndicate.h>
#include <Engine/Contents/Base/Durability.h>
#include <Engine/Contents/Base/Notice.h>
#include <Engine/Contents/Base/Quest.h>
#include <Engine/Contents/Base/UIQuestBookNew.h>
#include <Engine/Contents/Base/UIQuestView.h>
#include <Engine/Contents/Base/UIQuestAccept.h>
#include <Engine/Contents/Base/UIQuestComplete.h>
#include <Engine/Contents/Base/UIQuestRestore.h>
#include <Engine/Contents/function/ItemCollection.h>
#include <Engine/Contents/function/PremiumChar.h>
#include <tlhelp32.h>

#include <Engine/Contents/Login/ServerSelect.h>
#include <Engine/Contents/Login/UIServerSelect.h>
#include <Engine/Contents/Login/UICharacterSelect.h>
#include <Engine/Contents/Login/CharacterSelect.h>
#include <Engine/Contents/Login/UICharacterSelect.h>
#include <Engine/Contents/Login/UILoginNew.h>

#include <Engine/LoginJobInfo.h>
#include <Engine/GameStageManager/StageMgr.h>
#include <Engine/Loading.h>
#include <Engine/Contents/Base/Quest.h>
#include <Engine/Contents/Base/Party.h>

#include <Engine/Contents/function/SimplePlayerInfoUI.h>
#include <Engine/Contents/function/ItemComposeUI.h>

// WSS_NPROTECT 070402 ------------------------------->>
#ifndef NO_GAMEGUARD
	//#include <NPGameLib.h>
	#include <Engine/GameGuardInterface.h>
#endif
// ---------------------------------------------------<<

#ifdef KALYDO
#include <Engine/Kalydo/PackageManager.h>
#endif

#include <Engine/Help/ItemHelp.h>
#include <Engine/Interface/UISecurity.h>

#ifdef	STASH_PASSWORD
#include <Common/Packet/ptype_old_do_stash.h>
#endif	// STASH_PASSWORD

#include <Engine/Contents/Login/UICharacterCreateNew.h>
#include <Engine/Object/ActorMgr.h>

#include <Common/Packet/ptype_old_mempos.h>
#include <Common/Packet/ptype_old_do_changejob.h>
#include <Common/Packet/ptype_old_do_move.h>
#include <Common/Packet/ptype_old_do_skill.h>
#include <Common/Packet/ptype_old_do_sskill.h>
#include <Common/Packet/ptype_old_extend.h>
#include <Engine/Info/MyInfo.h>
#include <Engine/Contents/function/gps.h>
#include <Engine/Contents/function/ItemCompose.h>
#include "Entities/TargetInfo.h"

#define SESSIONSTATEVERSION_OLD 1
#define SESSIONSTATEVERSION_WITHBULLETTIME 2
#define SESSIONSTATEVERSION_CURRENT SESSIONSTATEVERSION_WITHBULLETTIME



//---------------------------------------------------------------------

#define MODEL_MONEY		("Data\\Item\\Common\\item_money_01.smc")
#define MODEL_GIFT		("Data\\Item\\Common\\item_gift.smc")


#define MODEL_DROPIEM_TI_BU		("Data\\Item\\Common\\ti_bu_drop.smc")
#define MODEL_DROPIEM_TI_BD		("Data\\Item\\Common\\ti_bd_dorp.smc")
#define MODEL_DROPIEM_TI_FT		("Data\\Item\\Common\\ti_ft_drop.smc")
#define MODEL_DROPIEM_TI_HN		("Data\\Item\\Common\\ti_hn_drop.smc")

#define MODEL_DROPIEM_NI_BU		("Data\\Item\\Common\\ni_bu_drop.smc")
#define MODEL_DROPIEM_NI_BD		("Data\\Item\\Common\\ni_bd_drop.smc")
#define MODEL_DROPIEM_NI_FT		("Data\\Item\\Common\\ni_ft_drop.smc")
#define MODEL_DROPIEM_NI_HN		("Data\\Item\\Common\\ni_hn_drop.smc")

#define MODEL_DROPIEM_HW_BU		("Data\\Item\\Common\\hw_bu_drop.smc")
#define MODEL_DROPIEM_HW_BD		("Data\\Item\\Common\\hw_bd_drop.smc")
#define MODEL_DROPIEM_HW_FT		("Data\\Item\\Common\\hw_ft_drop.smc")
#define MODEL_DROPIEM_HW_HN		("Data\\Item\\Common\\hw_hn_drop.smc")

#define MODEL_DROPIEM_MA_BU		("Data\\Item\\Common\\ma_bu_drop.smc")
#define MODEL_DROPIEM_MA_BD		("Data\\Item\\Common\\ma_bd_drop.smc")
#define MODEL_DROPIEM_MA_FT		("Data\\Item\\Common\\ma_ft_drop.smc")
#define MODEL_DROPIEM_MA_HN		("Data\\Item\\Common\\ma_hn_drop.smc")

#define MODEL_DROPIEM_RO_BU		("Data\\Item\\Common\\ro_bu_drop.smc")
#define MODEL_DROPIEM_RO_BD		("Data\\Item\\Common\\ro_bd_drop.smc")
#define MODEL_DROPIEM_RO_FT		("Data\\Item\\Common\\ro_ft_drop.smc")
#define MODEL_DROPIEM_RO_HN		("Data\\Item\\Common\\ro_hn_drop.smc")

#define MODEL_DROPIEM_SO_BU		("Data\\Item\\Common\\so_bu_drop.smc")
#define MODEL_DROPIEM_SO_BD		("Data\\Item\\Common\\so_bd_drop.smc")
#define MODEL_DROPIEM_SO_FT		("Data\\Item\\Common\\so_ft_drop.smc")
#define MODEL_DROPIEM_SO_HN		("Data\\Item\\Common\\so_hn_drop.smc")

#define MODEL_DROPIEM_NS_HD		("Data\\Item\\Common\\ns_hood_drop.smc")
#define MODEL_DROPIEM_NS_BU		("Data\\Item\\Common\\ns_bu_drop.smc")
#define MODEL_DROPIEM_NS_BD		("Data\\Item\\Common\\ns_bd_drop.smc")
#define MODEL_DROPIEM_NS_FT		("Data\\Item\\Common\\ns_ft_drop.smc")
#define MODEL_DROPIEM_NS_HN		("Data\\Item\\Common\\ns_hn_drop.smc")
#define MODEL_DROPIEM_NS_WG		("Data\\Item\\Common\\ns_wing_drop.smc")

#define MODEL_DROP_WEAPON_NS	("Data\\Item\\Weapon\\Drop\\ns_weapon_drop.smc")

#define MODEL_DROPIEM_HW_BOW		("Data\\Item\\Common\\hw_bow_drop.smc")
#define MODEL_DROPIEM_HW_STAFF		("Data\\Item\\Common\\hw_staff_drop.smc")
#define MODEL_DROPIEM_MA_WAND		("Data\\Item\\Common\\ma_wand_drop.smc")
#define MODEL_DROPIEM_MA_STAFF		("Data\\Item\\Common\\ma_staff_drop.smc")
#define MODEL_DROPIEM_NI_SHIELD		("Data\\Item\\Common\\ni_shield_drop.smc")
#define MODEL_DROPIEM_NI_SWORD		("Data\\Item\\Common\\ni_sword_drop.smc")
#define MODEL_DROPIEM_RO_CBOW		("Data\\Item\\Common\\ro_crossbow_drop.smc")
#define MODEL_DROPIEM_RO_DAGGER		("Data\\Item\\Common\\ro_dagger_drop.smc")
#define MODEL_DROPIEM_TI_GSWORD		("Data\\Item\\Common\\ti_gsword_drop.smc")
#define MODEL_DROPIEM_TI_AXE		("Data\\Item\\Common\\ti_axe_drop.smc")
#define MODEL_DROPIEM_SO_SCYTHE		("Data\\Item\\Common\\so_scythe_001_drop.smc")
#define MODEL_DROPIEM_SO_STAFF		("Data\\Item\\Common\\so_staff_001_drop.smc")

//---------------------------------------------------------------------

#ifndef PROC_MESSAGE_COUNT_MAX
#define PROC_MESSAGE_COUNT_MAX 5
#endif

#define	ISNPCLIVE	1
//#define TEST_SERVER

extern INDEX net_bLerping;
extern FLOAT net_tmConnectionTimeout;
extern FLOAT net_tmProblemsTimeOut;
extern FLOAT net_tmDisconnectTimeout;
extern INDEX net_bReportClientTraffic;

extern CClientInterface cm_ciLocalClient;
extern TIME _tmLocalTick = -1.0f;
extern INDEX g_iCountry;

extern INDEX g_iQuestBookOpen;

FLOAT3D _vBasePosition;
BOOL    _bBasePositionSet;

#if DEBUG_LERPING

FLOAT avfStats[1000][4];
INDEX ctCounter=0;
INDEX ctMax = sizeof(avfStats)/sizeof(avfStats[0]);

#endif // DEBUG_LERPING

// 플레이 타임 연산 관련 /////////////////////////////////////////////////
DWORD	_SetPlayStartTime = 0;
const DWORD	_IntervalTime = 60; // 60분 간격
const DWORD	_IntervalPlayCountTime = 1000 * 60 * _IntervalTime;
static DWORD _PlayTimeCount = 0;
//////////////////////////////////////////////////////////////////////////

#if defined(G_GERMAN) || defined(G_EUROPE3) || defined(G_EUROPE2)//독일 로컬은 정산 알림 사용 안함
	BOOL bCalcNotChecked = FALSE;
#else
	BOOL bCalcNotChecked = TRUE;
#endif

// [100122: selo] 윈도우 핸들을 찾아 프로세스 찾기 루틴
ULONG ProcIDFromWnd(HWND hwnd) // 윈도우 핸들로 프로세스 아이디 얻기  
{  
  ULONG idProc;  
  GetWindowThreadProcessId( hwnd, &idProc );  
  return idProc;  
}  
// [100122: selo] 위 함수와 연계되는 함수 
HWND GetWinHandle(ULONG pid) // 프로세스 아이디로 윈도우 핸들 얻기  
{  
  HWND tempHwnd = FindWindow(NULL,NULL); // 최상위 윈도우 핸들 찾기  
  
  while( tempHwnd != NULL )  
  {  
    if( GetParent(tempHwnd) == NULL ) // 최상위 핸들인지 체크, 버튼 등도 핸들을 가질 수 있으므로 무시하기 위해  
      if( pid == ProcIDFromWnd(tempHwnd) )  
        return tempHwnd;  
    tempHwnd = GetWindow(tempHwnd, GW_HWNDNEXT); // 다음 윈도우 핸들 찾기  
  }  
  return NULL;  
}
// [100122: selo] 해킹 프로세스 찾기
BOOL FindProcess(void)
{
	HANDLE      hProcessSnap = NULL;
	BOOL		Return       = FALSE;
	
	PROCESSENTRY32 pe32         = {0};
	
	char buf[256];
	
	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);    
	
	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}
	
	pe32.dwSize = sizeof(PROCESSENTRY32);    
	
	if (Process32First(hProcessSnap, &pe32))
	{   
		do
		{
			HANDLE hProcess;                        
			// Get the actual priority class.            
			hProcess = OpenProcess (PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID);
			
			if( hProcess != NULL )
			{
				HWND hWnd = GetWinHandle(pe32.th32ProcessID);

				GetClassName((HWND)hWnd, buf, 256);	

				if( 0 == strncmp(buf, "AutoIt", 6) )
				{
					Return = TRUE;	
					CPrintF("나는 행유져");
				}				

				CloseHandle (hProcess);
			}
		}
		while (Process32Next(hProcessSnap, &pe32));		
	}	
	
	CloseHandle (hProcessSnap);

	return Return;
}

int LeaseWeaponErrChk(ULONG errType) 		
{
	CTString strMessage;
	BOOL nResult = FALSE;
	switch(errType)
	{
		case MSG_ITEM_LEND_WEAPON_ERROR_OK:
			strMessage =_S(3082, "선택한 장비가 대여 되었습니다.");
			nResult = TRUE;
			break;
		case MSG_ITEM_LEND_WEAPON_ERROR_FULLINVEN:
			strMessage =_S( 265, "인벤토리 공간이 부족합니다.");
			break;
		case MSG_ITEM_LEND_WEAPON_ERROR_NOMONEY:
			strMessage =_S( 306,"나스가 부족합니다.");
			break;
		case MSG_ITEM_LEND_WEAPON_ERROR_FAIL:
			strMessage =_S( 16,"알수 없는 오류가 발생했습니다.");
			break;
		case MSG_ITEM_LEND_TIMEOUT:
			strMessage =_S( 3064,"대여 기간이 종료되었습니다.");
			break;
	}
	
	_pNetwork->ClientSystemMessage(strMessage);
	return nResult;
}

//wooss 050808 inner function
void ConfirmMsgProc(UBYTE subType,UBYTE errorType = NULL,CTString charName = CTString("PLAYER"))
{
	CTString		strTitle= _S(191,"확인");
	CTString		strMsg;
	CUIMsgBox_Info	msgInfo;
	
	switch(subType)
	{
	case MSG_WARP_TO_REQ:
		{
			CUIManager* pUIManager = CUIManager::getSingleton();

			if ( pUIManager->DoesMessageBoxExist(MSGCMD_WARP_TO_REQ_CALL) == TRUE )
			{
				// 메시지를 취소 요청 한후 닫는다
				CTString PreCallName = pUIManager->GetMessageBox(MSGCMD_WARP_TO_REQ_CALL)->GetInputBox().GetString();
				_pNetwork->SendWarpItemMessage( MSG_WARP_TO, PreCallName, FALSE);
				pUIManager->CloseMessageBox(MSGCMD_WARP_TO_REQ_CALL);
			}

			strMsg.PrintF(_S( 2124, "%s님께서 당신에게 순간이동을 하려고 합니다. 승인 하시겠습니까?" ),charName); 
			msgInfo.SetMsgBoxInfo(strTitle,UMBS_YESNO,UI_NONE,MSGCMD_WARP_TO_REQ_CALL);
			msgInfo.AddString(strMsg);
			pUIManager->CreateMessageBox(msgInfo);
			pUIManager->GetMessageBox(MSGCMD_WARP_TO_REQ_CALL)->GetInputBox().SetString(charName.str_String);
		}
		break;
	case MSG_WARP_TAKE_REQ:
		{
			CUIManager* pUIManager = CUIManager::getSingleton();

			if ( pUIManager->DoesMessageBoxExist(MSGCMD_WARP_TAKE_REQ_CALL) == TRUE )
			{
				// 메시지를 취소 요청 한후 닫는다
				CTString PreCallName = pUIManager->GetMessageBox(MSGCMD_WARP_TAKE_REQ_CALL)->GetInputBox().GetString();
				_pNetwork->SendWarpItemMessage( MSG_WARP_TAKE, PreCallName, FALSE);
				pUIManager->CloseMessageBox(MSGCMD_WARP_TAKE_REQ_CALL);
			}

			strMsg.PrintF(_S( 2125, "%s님께서 당신의 소환을 요청하셨습니다. 승인 하시겠습니까?" ),charName); 
			msgInfo.SetMsgBoxInfo(strTitle,UMBS_YESNO,UI_NONE,MSGCMD_WARP_TAKE_REQ_CALL);
			msgInfo.AddString(strMsg);
			pUIManager->CreateMessageBox(msgInfo);
			pUIManager->GetMessageBox(MSGCMD_WARP_TAKE_REQ_CALL)->GetInputBox().SetString(charName.str_String);
		}
		break;
	case MSG_WARP_ERROR:
		{
			switch(errorType)
			{
			case MSG_WARP_ERROR_NOTALLOW:
				{
					strMsg.PrintF(_S( 2126, "%s님께서 이동 요청을 거절하였습니다. 순간 이동이 취소됩니다." ),charName); 
					msgInfo.SetMsgBoxInfo(strTitle,UMBS_OK,UI_NONE,MSGCMD_NULL);
					msgInfo.AddString(strMsg);
					CUIManager::getSingleton()->CreateMessageBox(msgInfo);
					_pNetwork->SendWarpCancel();
				}
				break;
				
			case MSG_WARP_ERROR_NOTALLOWZONE:
				{
					strMsg =_S( 2127,  "순간 이동 가능 지역이 아닙니다.(다른 존이거나 퍼스널던전에서는 사용할 수 없습니다.)" ); 
					msgInfo.SetMsgBoxInfo(strTitle,UMBS_OK,UI_NONE,MSGCMD_NULL);
					msgInfo.AddString(strMsg);
					CUIManager::getSingleton()->CreateMessageBox(msgInfo);
				}
				break;
				
			case MSG_WARP_ERROR_NOTCHAR:
				{
					strMsg = _S(1628,"해당 케릭터가 존재 하지 않거나 [오프라인] 상태입니다."); 
					msgInfo.SetMsgBoxInfo(strTitle,UMBS_OK,UI_NONE,MSGCMD_NULL);
					msgInfo.AddString(strMsg);
					CUIManager::getSingleton()->CreateMessageBox(msgInfo);
				}
				break;
				
			case MSG_WARP_ERROR_CANCEL:
				{
					CUIManager* pUIManager = CUIManager::getSingleton();

					if(pUIManager->DoesMessageBoxExist(MSGCMD_WARP_TO_REQ_CALL)) 
						pUIManager->CloseMessageBox(MSGCMD_WARP_TO_REQ_CALL);
					if(pUIManager->DoesMessageBoxExist(MSGCMD_WARP_TAKE_REQ_CALL)) 		
						pUIManager->CloseMessageBox(MSGCMD_WARP_TAKE_REQ_CALL);
					pUIManager->GetChattingUI()->AddSysMessage( _S( 340, "이동이 취소되었습니다." ) );
				}
				break;
			}
			UIMGR()->SetCSFlagOff(CSF_TELEPORT);
		}
		break;
	}
}

// FIXME : 드롭될 SMC 명이 하드 코딩되어있는데...
ENGINE_API void SetDropItemModel(CEntity *penEntity, CItemData* pItemData, CItemTarget* pTarget)
{
	penEntity->InitAsSkaModel();
	char *szEffectName = NULL;
	INDEX animID = -1;

	if(pItemData->GetType() == CItemData::ITEM_ETC)
	{
		if(pItemData->GetSubType() == CItemData::ITEM_ETC_EVENT)		// 이벤트 아이템이라면,
		{
			if(pItemData->GetItemIndex() == 1393){
				penEntity->SetSkaModel(pItemData->GetItemSmcFileName());
				szEffectName = "server_event_drop";				
			}
			else penEntity->SetSkaModel(MODEL_GIFT);
		}	
		else if (pItemData->GetSubType() != CItemData::ITEM_ETC_MONEY)
		{
			if(pItemData->GetItemIndex() == 723)//문스톤
			{
				penEntity->SetSkaModel("data\\item\\common\\item_moonstone.smc");
				animID = penEntity->GetModelInstance()->FindFirstAnimationID();
				szEffectName = "Item MoonStone";
			}
			else if (pItemData->GetItemIndex() == 2844) // 카오스 제련석
			{
				penEntity->SetSkaModel("data\\item\\common\\item_goje_twn.smc");
				szEffectName = "Item GOJE twn";
			}
			else if(pItemData->GetItemIndex() == 85)//고급제련석
			{
				penEntity->SetSkaModel("data\\item\\common\\item_goje.smc");
				szEffectName = "Item GOJE";
			}
			else if(pItemData->GetItemIndex() == 779)//초고급제련석
			{
				penEntity->SetSkaModel("Data\\Item\\Common\\item_lightstone.smc");
				szEffectName = "Item SGOJE";
			}
			else if(pItemData->GetItemIndex() == 974)//행운의 제련석 wooss 051026 -> 소스 머지 대만 공개(060320)
			{
				penEntity->SetSkaModel("Data\\Item\\Common\\item_goje_lucky.smc");
				szEffectName = "Item LGOJE";
			}
			else if (pItemData->GetItemIndex() == 3888 || pItemData->GetItemIndex() == 6097 || pItemData->GetItemIndex() == 6650 )
			{
				// 정화의 룬 아르 이펙트 추가 ITS 15517
				penEntity->SetSkaModel(pItemData->GetItemSmcFileName());
				szEffectName = "Item GOJE_blue";
			}
			else if (pItemData->GetItemIndex() == 3889 || pItemData->GetItemIndex() == 4968 ||
				pItemData->GetItemIndex() == 6098 || pItemData->GetItemIndex() == 6649 )
			{
				// 악마의 룬 아르 이펙트 추가 ITS 15517
				penEntity->SetSkaModel(pItemData->GetItemSmcFileName());
				szEffectName = "Item GOJE_red";
			}
			// 1482 : 축구공 1484 :우승컵
			else if(pItemData->GetItemIndex() == 1482 || pItemData->GetItemIndex() == 1484 ){
				penEntity->SetSkaModel(pItemData->GetItemSmcFileName());
			}
			else if( pItemData->GetItemIndex() >= 1579 && pItemData->GetItemIndex() <= 1588 )
			{// 곤충 박스
				penEntity->SetSkaModel( pItemData->GetItemSmcFileName() );
			}
			// WSS_MINIGAME 070423 -------------------------------------------->>
			// 2329 아이템부터 smc 파일대로 드랍시킴
			else if( pItemData->GetItemIndex() >= 2329 )
			{
				penEntity->SetSkaModel( pItemData->GetItemSmcFileName() );
			}
			// ----------------------------------------------------------------<<
			else
			{
				penEntity->SetSkaModel(MODEL_TREASURE);
			}
		}
	}
	else if(pItemData->GetType() == CItemData::ITEM_WEAPON)
	{
#ifdef NEW_WEAPONDROP
		if( pItemData->GetSubType() == CItemData::ITEM_WEAPON_TWOSWORD || pItemData->GetSubType() == CItemData::ITEM_WEAPON_DAGGER )
		{
			CTString tStr;
			tStr.PrintF("%s",pItemData->GetItemSmcFileName());

			if ( tStr.ReplaceSubstr( CTString("Weapon"), CTString("Weapon\\Drop") ) == FALSE)
				tStr.ReplaceSubstr( CTString("weapon"), CTString("Weapon\\Drop") );

			penEntity->SetSkaModel( tStr );
		}
		else if (pItemData->GetSubType() == CItemData::ITEM_WEAPON_SOUL)
		{
			penEntity->SetSkaModel(MODEL_DROP_WEAPON_NS);
			animID = penEntity->GetModelInstance()->FindFirstAnimationID();
		}
		else 
			penEntity->SetSkaModel( pItemData->GetItemSmcFileName() );
#else
		switch(pItemData->GetSubType()) 
		{
		case CItemData::ITEM_WEAPON_KNIFE :
		case CItemData::ITEM_WEAPON_TWOSWORD :
			penEntity->SetSkaModel(MODEL_DROPIEM_NI_SWORD);
			break;
		case CItemData::ITEM_WEAPON_CROSSBOW :
			penEntity->SetSkaModel(MODEL_DROPIEM_RO_CBOW);
			break;
		case CItemData::ITEM_WEAPON_STAFF :
			penEntity->SetSkaModel(MODEL_DROPIEM_HW_STAFF);
			break;
		case CItemData::ITEM_WEAPON_BIGSWORD :
			penEntity->SetSkaModel(MODEL_DROPIEM_TI_GSWORD);
			break;
		case CItemData::ITEM_WEAPON_AXE	:
			penEntity->SetSkaModel(MODEL_DROPIEM_TI_AXE);
			break;		
		case CItemData::ITEM_WEAPON_WAND:
			//penEntity->SetSkaModel(MODEL_DROPIEM_MA_WAND);
			penEntity->SetSkaModel(MODEL_DROPIEM_MA_STAFF);
			break;
		case CItemData::ITEM_WEAPON_SSTAFF :
			//penEntity->SetSkaModel(MODEL_DROPIEM_MA_STAFF);
			penEntity->SetSkaModel(MODEL_DROPIEM_MA_WAND);
			break;
		case CItemData::ITEM_WEAPON_BOW	:
			penEntity->SetSkaModel(MODEL_DROPIEM_HW_BOW);
			break;
		case CItemData::ITEM_WEAPON_DAGGER :
			penEntity->SetSkaModel(MODEL_DROPIEM_RO_DAGGER);
			break;
		case CItemData::ITEM_WEAPON_SCYTHE:
			penEntity->SetSkaModel(MODEL_DROPIEM_SO_SCYTHE);
			break;
		case CItemData::ITEM_WEAPON_POLEARM	:
			penEntity->SetSkaModel(MODEL_DROPIEM_SO_STAFF);
			break;
		default:
			penEntity->SetSkaModel(MODEL_TREASURE);
			break;
		}
#endif
	}
	else if	(pItemData->GetType() == CItemData::ITEM_SHIELD)
	{
		switch(pItemData->GetSubType()) 
		{
		case CItemData::ITEM_SHIELD_COAT :
			if(pItemData->GetJob()& MASK_TITAN)
			{
				penEntity->SetSkaModel(MODEL_DROPIEM_TI_BU);
			}
			else if(pItemData->GetJob()& MASK_HEALER)
			{
				penEntity->SetSkaModel(MODEL_DROPIEM_HW_BU);
			}
			else if(pItemData->GetJob()& MASK_KNIGHT)
			{
				penEntity->SetSkaModel(MODEL_DROPIEM_NI_BU);
			}
			else if(pItemData->GetJob()& MASK_MAGE)
			{
				penEntity->SetSkaModel(MODEL_DROPIEM_MA_BU);
			}
			else if(pItemData->GetJob()& MASK_ROGUE)
			{
				penEntity->SetSkaModel(MODEL_DROPIEM_RO_BU);
			}
			else if(pItemData->GetJob()& MASK_SOCERER)
			{
				penEntity->SetSkaModel(MODEL_DROPIEM_SO_BU);
			}
			else if (pItemData->GetJob() & MASK_NIGHTSHADOW)
			{
				penEntity->SetSkaModel(MODEL_DROPIEM_NS_BU);
			}
#ifdef CHAR_EX_ROGUE
			else if(pItemData->GetJob()& MASK_EX_ROGUE)	// [2012/08/27 : Sora] EX로그 추가
			{
				penEntity->SetSkaModel(MODEL_DROPIEM_RO_BU);
			}
#endif
#ifdef CHAR_EX_MAGE
			else if(pItemData->GetJob()& MASK_EX_MAGE)	// 2013/01/08 jeil EX메이지 추가
			{
				penEntity->SetSkaModel(MODEL_DROPIEM_MA_BU);
			}
#endif
			else if(pItemData->GetJob()&(1<<WILDPET_JOB))
			{
				penEntity->SetSkaModel(pItemData->GetItemSmcFileName());
			}
			break;
		case CItemData::ITEM_SHIELD_PANTS :
			if(pItemData->GetJob()& MASK_TITAN)
			{
				penEntity->SetSkaModel(MODEL_DROPIEM_TI_BD);
			}
			else if(pItemData->GetJob()& MASK_HEALER)
			{
				penEntity->SetSkaModel(MODEL_DROPIEM_HW_BD);
			}
			else if(pItemData->GetJob()& MASK_KNIGHT)
			{
				penEntity->SetSkaModel(MODEL_DROPIEM_NI_BD);
			}
			else if(pItemData->GetJob()& MASK_MAGE)
			{
				penEntity->SetSkaModel(MODEL_DROPIEM_MA_BD);
			}
			else if(pItemData->GetJob()& MASK_ROGUE)
			{
				penEntity->SetSkaModel(MODEL_DROPIEM_RO_BD);
			}
			else if(pItemData->GetJob()& MASK_SOCERER)
			{
				penEntity->SetSkaModel(MODEL_DROPIEM_SO_BD);
			}
			else if (pItemData->GetJob() & MASK_NIGHTSHADOW)
			{
				penEntity->SetSkaModel(MODEL_DROPIEM_NS_BD);
			}
#ifdef CHAR_EX_ROGUE
			else if(pItemData->GetJob()& MASK_EX_ROGUE)	// [2012/08/27 : Sora] EX로그 추가
			{
				penEntity->SetSkaModel(MODEL_DROPIEM_RO_BD);
			}
#endif
#ifdef CHAR_EX_MAGE
			else if(pItemData->GetJob()& MASK_EX_MAGE)	// 2013/01/08 jeil EX메이지 추가 
			{
				penEntity->SetSkaModel(MODEL_DROPIEM_MA_BD);
			}
#endif
			break;
		case CItemData::ITEM_SHIELD_GLOVE :
			if(pItemData->GetJob()& MASK_TITAN)
			{
				penEntity->SetSkaModel(MODEL_DROPIEM_TI_HN);
			}
			else if(pItemData->GetJob()& MASK_HEALER)
			{
				penEntity->SetSkaModel(MODEL_DROPIEM_HW_HN);
			}
			else if(pItemData->GetJob()& MASK_KNIGHT)
			{
				penEntity->SetSkaModel(MODEL_DROPIEM_NI_HN);
			}
			else if(pItemData->GetJob()& MASK_MAGE)
			{
				penEntity->SetSkaModel(MODEL_DROPIEM_MA_HN);
			}
			else if(pItemData->GetJob()& MASK_ROGUE)
			{
				penEntity->SetSkaModel(MODEL_DROPIEM_RO_HN);
			}
			else if(pItemData->GetJob()& MASK_SOCERER)
			{
				penEntity->SetSkaModel(MODEL_DROPIEM_SO_HN);
			}
			else if (pItemData->GetJob() & MASK_NIGHTSHADOW)
			{
				penEntity->SetSkaModel(MODEL_DROPIEM_NS_HN);
			}
#ifdef CHAR_EX_ROGUE
			else if(pItemData->GetJob()& MASK_EX_ROGUE)	// [2012/08/27 : Sora] EX로그 추가
			{
				penEntity->SetSkaModel(MODEL_DROPIEM_RO_HN);
			}
#endif
#ifdef CHAR_EX_MAGE
			else if(pItemData->GetJob()& MASK_EX_MAGE)	// 2013/01/08 jeil EX메이지 추가 
			{
				penEntity->SetSkaModel(MODEL_DROPIEM_MA_HN);
			}
#endif
			break;
		case CItemData::ITEM_SHIELD_SHOES :
			if(pItemData->GetJob()& MASK_TITAN)
			{
				penEntity->SetSkaModel(MODEL_DROPIEM_TI_FT);
			}
			else if(pItemData->GetJob()& MASK_HEALER)
			{
				penEntity->SetSkaModel(MODEL_DROPIEM_HW_FT);
			}
			else if(pItemData->GetJob()& MASK_KNIGHT)
			{
				penEntity->SetSkaModel(MODEL_DROPIEM_NI_FT);
			}
			else if(pItemData->GetJob()& MASK_MAGE)
			{
				penEntity->SetSkaModel(MODEL_DROPIEM_MA_FT);
			}
			else if(pItemData->GetJob()& MASK_ROGUE)
			{
				penEntity->SetSkaModel(MODEL_DROPIEM_RO_FT);
			}
			else if(pItemData->GetJob()& MASK_SOCERER)
			{
				penEntity->SetSkaModel(MODEL_DROPIEM_SO_FT);
			}
			else if (pItemData->GetJob() & MASK_NIGHTSHADOW)
			{
				penEntity->SetSkaModel(MODEL_DROPIEM_NS_FT);
			}
#ifdef CHAR_EX_ROGUE
			else if(pItemData->GetJob()& MASK_EX_ROGUE)	// [2012/08/27 : Sora] EX로그 추가
			{
				penEntity->SetSkaModel(MODEL_DROPIEM_RO_FT);
			}
#endif
#ifdef CHAR_EX_MAGE
			else if(pItemData->GetJob()& MASK_EX_MAGE)	// 2013/01/08 jeil EX메이지 추가
			{
				penEntity->SetSkaModel(MODEL_DROPIEM_MA_FT);
			}
#endif
			break;
		case CItemData::ITEM_SHIELD_HEAD:
			{
				if (pItemData->GetJob() & MASK_NIGHTSHADOW)
				{
					penEntity->SetSkaModel(MODEL_DROPIEM_NS_HD);
				}
				else
				{
					penEntity->SetSkaModel(MODEL_TREASURE);
				}
			}
			break;
		case CItemData::ITEM_SHIELD_BACKWING:
			{
				if (pItemData->GetJob() & MASK_NIGHTSHADOW)
				{
					penEntity->SetSkaModel(MODEL_DROPIEM_NS_WG);
				}
				else
				{
					penEntity->SetSkaModel(MODEL_TREASURE);
				}
			}
			break;
		case CItemData::ITEM_SHIELD_SHIELD :
			penEntity->SetSkaModel(MODEL_DROPIEM_NI_SHIELD);
			break;
		default:
			penEntity->SetSkaModel(MODEL_TREASURE);
			break;
		}
	}
	else if(pItemData->GetType() == CItemData::ITEM_ONCEUSE)
	{
// [KH_070328] 달걀 이벤트 관련 추가
		if(pItemData->GetItemIndex() == EASTER_EGGS || pItemData->GetItemIndex() == 6231 )	// 일본 1주년 기념 달걀 추가
		{
			penEntity->SetSkaModel( pItemData->GetItemSmcFileName() );
			szEffectName = "Item LGOJE";
		}else if (pItemData->GetItemIndex() == 6593) // 웜의 알 [4/15/2011 ldy1978220]
		{
			penEntity->SetSkaModel( pItemData->GetItemSmcFileName() );
			szEffectName = "egg";
		}
		else
		{
			penEntity->SetSkaModel(MODEL_TREASURE);
		}
	}
	else
	{
		penEntity->SetSkaModel(MODEL_TREASURE);
	}

	penEntity->Initialize();
	penEntity->FallDownToFloor();
	
	if(penEntity->GetModelInstance())
	{
		if(szEffectName)
		{
			CSkaTag tag;
			tag.SetName("__ROOT");
			penEntity->GetModelInstance()->m_tmSkaTagManager.Register(&tag);
			pTarget->item_pDropItemEffect = StartEffectGroup(szEffectName, 
				&penEntity->GetModelInstance()->m_tmSkaTagManager, 
				_pTimer->GetLerpedCurrentTick());
		}
		if(animID != -1)
		{
			penEntity->GetModelInstance()->AddAnimation(
				animID, AN_LOOPING|AN_CLEAR|AN_NORESTART, 1.0f, 0);
		}
	}
}

//0105
//CEntity* m_penEntity;

#define TOTAL_TAB	3
#define TOTAL_ROW	20
#define TOTAL_COL	5

#define ADDSERVERVALUE	1999

#define		ITEM_ARRANGE_START			(1 << 0)	// 아이템 정렬의 시작.
#define		ITEM_ARRANGE_END			(1 << 1)	// 아이템 정렬의 끝.

#define TEACHER_PRIZE_EVENT_2PAN4PAN				// 후견인 명성 보상 이벤트

//#define RESTRICT_SOUND

// get a pseudo-random number (safe for network gaming)
ULONG CSessionState::Rnd(void) {
//  ASSERTMSG(ses_bAllowRandom, "Use RND only in entity AI!");
	// NOTE: 
	// using multiplicative congruent method with Greanberger's lambda = 2^18+3
	ses_ulRandomSeed = ses_ulRandomSeed*262147;
	ASSERT(ses_ulRandomSeed!=0);
	return ses_ulRandomSeed;
}

// reset random number generator (always randomizes to same sequence!)
void CSessionState::ResetRND(void)
{
	BOOL bOldAllow = ses_bAllowRandom;
	ses_bAllowRandom = TRUE;
	// random must start at a number different than zero!
	ses_ulRandomSeed = 0x87654321;
	// run rnd a few times to make it go random
	for(INDEX i=0; i<32; i++) {
		Rnd();
	}
	ses_bAllowRandom = bOldAllow;
}

/*
 * Constructor.
 */
CSessionState::CSessionState(void)
{
	TRACKMEM(Mem,"Network");
	ses_bKeepingUpWithTime = TRUE;
	ses_tmLastUpdated = -100;
	ses_bAllowRandom = TRUE;  // random allowed when not in game

	ses_bPause = FALSE;
	ses_bWantPause = FALSE;
	ses_bGameFinished = FALSE;
	ses_bWaitingForServer = FALSE;
	ses_strDisconnected = "";
	ses_ctMaxPlayers = 1;
	ses_bWaitAllPlayers = FALSE;
	ses_fRealTimeFactor = 1.0f;
	ses_ubNumLevelChanges = 0;

	// reset random number generator
	ResetRND();

	ses_tmLastTickTime = -1;
	ses_ulMsgId=0;
	ses_bRestartLocalTime = FALSE;

	_tmLocalTick = -1.0f;
//0105
	m_commIn[0] = NULL;
	m_commOut[0] = NULL;
	m_commInTmp[0] = NULL;
	m_pCmd = new CCmd(this);

	m_iRecentServer = -1;
	m_iRecentGroup	= -1;

#ifdef	KALYDO
	int		i;
	for (i = 0; i < eKST_MAX; ++i)
	{
		m_pCmdKalydo[i] = NULL;
	}
#endif	// KALYDO	
	reg_packet();
	reg_packet_info();
	reg_packet_comm();
}

/*
 * Destructor.
 */
CSessionState::~CSessionState()
{
//0105
	delete m_pCmd;
#ifdef	KALYDO
	int		i;
	for (i = 0; i < eKST_MAX; ++i)
	{		
		if (m_pCmdKalydo[i] != NULL)
			delete m_pCmdKalydo[i];
		m_pCmdKalydo[i] = NULL;
	}
#endif	// KALYDO	
}

/*
 * Clear the object.
 */
void CSessionState::Stop(void)
{

	ses_bKeepingUpWithTime = TRUE;
	ses_tmLastUpdated = -100;
	ses_bAllowRandom = TRUE;  // random allowed when not in game
	ses_bPause = FALSE;
	ses_bWantPause = FALSE;
	ses_bGameFinished = FALSE;
	ses_bWaitingForServer = FALSE;
	ses_strDisconnected = "";
	ses_ctMaxPlayers = 1;
	ses_fRealTimeFactor = 1.0f;
	ses_bWaitAllPlayers = FALSE;
	ses_tmLastTickTime = -1;
	ses_ulMsgId=0;

	// disable lerping
	_pTimer->DisableLerp();

#if DEBUG_LERPING

	CTFileStream f;
	f.Create_t(CTFILENAME("Temp\\Lerp.stats"), CTStream::CM_TEXT);
	for (INDEX i=0; i<ctCounter; i++) {
		char strBuffer[256];
		sprintf(strBuffer, "%6.2f %6.2f %6.2f %6.2f",
			avfStats[i][0],
			avfStats[i][1],
			avfStats[i][2],
			avfStats[i][3]);
		f.PutLine_t(strBuffer);
	}
	f.Close();

#endif // DEBUG_LERPING
/* //0522 kwon 삭제.
	CNetworkMessage nmConfirmDisconnect(MSG_REP_DISCONNECTED);
	if (_cmiComm.cci_bClientInitialized) {
	  _pNetwork->SendToServerReliable(nmConfirmDisconnect);
	}
*/
	_cmiComm.Client_Close();

	// clear all old levels
	ForgetOldLevels();

	_pNetwork->ga_srvServer.srv_apltPlayers.Clear();
	_pNetwork->ga_srvServer.srv_apltPlayers.New(NET_MAXGAMEPLAYERS);
}

/*
 * Initialize session state and wait for game to be started.
 */
void CSessionState::Start_t(INDEX ctLocalPlayers) 
{
	ses_bKeepingUpWithTime = TRUE;
	ses_tmLastUpdated = -100;
	// clear message stream
	ses_nsGameStream.Clear();
	ses_bAllowRandom = FALSE;  // random not allowed in game
	ses_bPause = FALSE;
	ses_bWantPause = FALSE;
	ses_bWaitingForServer = FALSE;
	ses_bGameFinished = FALSE;
	ses_strDisconnected = "";
	ses_ctMaxPlayers = 1;
	ses_fRealTimeFactor = 1.0f;
	ses_bWaitAllPlayers = FALSE;
	ses_iMissingSequence = -1;
	ses_tvMessageReceived.Clear();
	_pNetwork->ga_strRequiredMod = "";
	ses_tmLastTickTime = -1;
	ses_ulMsgId=0;

	// reset random number generator
	ResetRND();

	// clear all old levels
	ForgetOldLevels();

#if DEBUG_LERPING
	// clear lerp stats
	ctCounter = 0;
#endif // DEBUG_LERPING

	// ses_LastProcessedTick and ses_LastReceivedTick tick counters are
	// irrelevant now, will be initialized when initialization message
	// from server is received, no need to set them here

	// if this computer is server
	if (_pNetwork->IsServer()) {
		// initialize local client
		_cmiComm.Client_Init_t(0UL);
		// connect as main session state
		try {
			Start_AtServer_t();
		} catch(char *) {
			_cmiComm.Client_Close();
			throw;
		}

	// if this computer is client
	} else {
		// connect client to server computer
		_cmiComm.Client_Init_t((char*)(const char*)_pNetwork->ga_strServerAddress);
	}
	ses_tvMessageReceived = _pTimer->GetHighPrecisionTimer();
}
//! 서버 스타트.
void CSessionState::Start_AtServer_t(void)     // throw char *
{
	//! 로컬 클라이언트가 접속을 요청한다.
	// send registration request
	CNetworkMessage nmRegisterMainSessionState((UBYTE)MSG_REQ_CONNECTLOCALSESSIONSTATE);
	ses_sspParams.Update();
	nmRegisterMainSessionState<<ses_sspParams;
	//! 이함수 안에서 서버 업데이트를 하지..
	_pNetwork->SendToServerReliable(nmRegisterMainSessionState);

	TIME tmTimeout = net_tmConnectionTimeout;
	if (_pNetwork->IsServer()) {
		tmTimeout = 5.0f;
	}
//! 5초동안 0.05초 간격으로 
	for(TIME tmWait=0; tmWait<tmTimeout*1000; 
	Sleep(NET_WAITMESSAGE_DELAY), tmWait+=NET_WAITMESSAGE_DELAY) { 
	  
	  //! 서버 업데이트.
	  // handle server messages
	  _cmiComm.Server_Update();
	  //! 서버가 스타트 되었다면,
	  if (_pNetwork->ga_srvServer.srv_bActive) {
		  // handle all incoming messages
		  _pNetwork->ga_srvServer.HandleAll();
	  }
	  //! 서버 메인 loop
	  _pNetwork->ga_srvServer.ServerLoop();
	  //! 또 서버 업데이트
	  _cmiComm.Server_Update();
	  //! 로컬 클라이언트 업데이트.
	  if (_cmiComm.Client_Update() == FALSE) {
		  break;
	  }
	  
	  //! 로컬클라이언트가 서버로 부터의 릴라이어블 메시지를 기다린다.
	  // wait for message to come
	  CNetworkMessage nmReceived;
	  if (!_pNetwork->ReceiveFromServerReliable(nmReceived)) {
		  continue;
	  }
	  //! 그 메시지가 초기화 메시지라면,
	  // if this is the init message
	  if (nmReceived.GetType() == MSG_REP_CONNECTLOCALSESSIONSTATE) {
		  // just adjust your tick counters
		  nmReceived>>ses_tmLastProcessedTick;
		  nmReceived>>ses_iLastProcessedSequence;
		  ses_tmInitializationTick  = -1.0f;
		  ses_tmInitializationTick2 = -1.0f;
		  ses_tvMessageReceived = _pTimer->GetHighPrecisionTimer();
		  //! for문 빠져나감.
		  // finish waiting
		  return;
		  // otherwise
	  } else {
		  // it is invalid message
		  ThrowF_t(TRANS("Invalid message while waiting for server session registration"));
	  }
	  
	  // if client is disconnected
	  if (!_cmiComm.Client_IsConnected()) {
		  // quit
		  ThrowF_t(TRANS("Client disconnected"));
	  }
	}
	ThrowF_t(TRANS("Timeout while waiting for server session registration"));
}

//안태훈 수정 시작	//(5th Closed beta)(0.2)
BOOL PCStartEffectGroup(const char *szEffectGroupName, SLONG slPCIndex, CEntity *penPC, float fDelayTime )
{
	float fStartTime = _pTimer->GetLerpedCurrentTick() +fDelayTime;
	
	//내 캐릭이면
	if(_pNetwork->MyCharacterInfo.index == slPCIndex)
	{
		if(CEntity::GetPlayerEntity(0) != NULL
			&& CEntity::GetPlayerEntity(0)->en_pmiModelInstance != NULL)
		{
			StartEffectGroup(szEffectGroupName
				, &CEntity::GetPlayerEntity(0)->en_pmiModelInstance->m_tmSkaTagManager
				//, _pTimer->GetLerpedCurrentTick());
				, fStartTime);
			return TRUE;
		}
	}
	//내 캐릭이 아니면
	else
	{
		if( penPC == NULL )
		{
			ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_CHARACTER, slPCIndex);

			if (pObject != NULL)
			{
				if (pObject->GetEntity() != NULL)
				{
						StartEffectGroup(szEffectGroupName
						, &(pObject->GetEntity()->en_pmiModelInstance->m_tmSkaTagManager)
										, fStartTime);
					}
				return TRUE;				
				}
			}
		else
		{
			if( penPC != NULL && penPC->en_pmiModelInstance != NULL && penPC->en_pmiModelInstance->GetName() != "" )
			{
				StartEffectGroup( szEffectGroupName, &(penPC->en_pmiModelInstance->m_tmSkaTagManager)
									//, _pTimer->GetLerpedCurrentTick() );
									, fStartTime);
				return TRUE;
			}
		}
	}
	return FALSE;
}
//안태훈 수정 끝	//(5th Closed beta)(0.2)
BOOL ENGINE_API WildPetStartEffectGroup(const char *szEffectGroupName, SLONG slPetIndex, CEntity *penPet )
{
	if( penPet == NULL )
	{
		ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_WILDPET, slPetIndex);

		if (pObject != NULL)
		{
			CWildPetTarget* pTarget = static_cast< CWildPetTarget* >(pObject);

			if (pTarget->GetEntity() != NULL && 
				pTarget->GetEntity()->en_pmiModelInstance != NULL && 
				pTarget->GetEntity()->en_pmiModelInstance->GetName() != "")
			{
				StartEffectGroup(szEffectGroupName, 
					&(pTarget->GetEntity()->en_pmiModelInstance->m_tmSkaTagManager),
					_pTimer->GetLerpedCurrentTick());
				
					return TRUE;
				}
			}
		}
	else
	{
		if( penPet != NULL && penPet->en_pmiModelInstance != NULL && penPet->en_pmiModelInstance->GetName() != "" )
		{
			StartEffectGroup( szEffectGroupName, &(penPet->en_pmiModelInstance->m_tmSkaTagManager),
								_pTimer->GetLerpedCurrentTick() );
			return TRUE;
		}
	}
	
	return FALSE;
}

// notify entities of level change
void CSessionState::SendLevelChangeNotification(CEntityEvent &ee)
{
	// for each entity in the world
	{FOREACHINDYNAMICCONTAINER(_pNetwork->ga_World.wo_cenEntities, CEntity, iten) {
		// if it should be notified
		if (iten->en_ulFlags&ENF_NOTIFYLEVELCHANGE) {
			// send the event
			iten->SendEvent(ee);
		}
	}}
}
//! 서버로부터 오는 스트림을 기다린다.
// wait for a stream to come from server
void CSessionState::WaitStream_t(CTMemoryStream &strmMessage, const CTString &strName, 
																 INDEX iMsgCode)
{

	// start waiting for server's response
	CallProgressHook_t(0.0f);
	SLONG slReceivedLast = 0;

	// repeat until timed out
	for(TIME tmWait=0; tmWait<net_tmConnectionTimeout*1000/3;
		Sleep(NET_WAITMESSAGE_DELAY), tmWait+=NET_WAITMESSAGE_DELAY) {
		//!서버에서 답변이 올때까지 보낸다.
		// update network connection sockets
		if (_cmiComm.Client_Update() == FALSE) {
			break;
		}



		// check how much is received so far
		SLONG slExpectedSize; 
		SLONG slReceivedSize;
		//! 패킷에 적혀있는 전송사이즈(slExpectedSize)와 
	//! 실제로 버퍼에 존재하는 릴라이어블 패킷의 사이즈(slReceivedSize)를 얻어낸다.	
		_cmiComm.Client_PeekSize_Reliable(slExpectedSize,slReceivedSize);
	//! 아직 패킷이 버퍼에 안들어왔다면,
		// if nothing received yet
		if (slExpectedSize==0) {
			// progress with waiting
	  //! 네트워크 업데이트 함수였다!!!
			CallProgressHook_t(tmWait/(net_tmConnectionTimeout*1000));
	  //! 무언가 패킷이 들어왔는데,
			// if something received
		} else {
	  //! 새로운 데이타를 받았다면,
			// if some new data received
			if (slReceivedSize!=slReceivedLast) {
				slReceivedLast = slReceivedSize;
				// reset timeout
				tmWait=0;
			}
			// progress with receiving
			CallProgressHook_t((float)slReceivedSize/slExpectedSize);
		}

	//! 만약에 제대로 다 받지 못했다면, continue.
		// if not everything received yet
		if (!_pNetwork->ReceiveFromServerReliable(strmMessage)) {
			// continue waiting
			continue;
		}
	//! 이제 메시지를 받았다.메시지의 첫번째 위치로 이동하여 아이디를 얻어낸다.
		// read message identifier
		strmMessage.SetPos_t(0);
		INDEX iID;
		strmMessage>>iID;
	//! 우리가 기다리면 메시지 코드이냐...처음엔 MSG_REP_CONNECTREMOTESESSIONSTATE
		// if this is the message
		if (iID == iMsgCode) {
			// all ok
			CallProgressHook_t(1.0f);
			return;
		// if disconnected
		}
/* //0522 kwon 삭제.		
		else if (iID == MSG_INF_DISCONNECTED) {
		//! 디스커넥트 확인 메시지 보냄.
		// confirm disconnect
		CNetworkMessage nmConfirmDisconnect(MSG_REP_DISCONNECTED);			
		_pNetwork->SendToServerReliable(nmConfirmDisconnect);
		//! 디스커넥트 이유
		// report the reason
		CTString strReason;
		strmMessage>>strReason;
		ses_strDisconnected = strReason;
		ThrowF_t(TRANS("Disconnected: %s\n"), strReason);
		// otherwise
		} */else {
			 //! 이상한 메시지가 날라온것임.
			// it is invalid message
			ThrowF_t(TRANS("Invalid stream while waiting for %s"), strName);
		}

		// if client is disconnected
		if (!_cmiComm.Client_IsConnected()) {
			// quit
			ThrowF_t(TRANS("Client disconnected"));
		}
	}

//	CNetworkMessage nmConfirmDisconnect(MSG_REP_DISCONNECTED);			
//	_pNetwork->SendToServerReliable(nmConfirmDisconnect);

	
	ThrowF_t(TRANS("Timeout while waiting for %s"), strName);
}

// check if disconnected
BOOL CSessionState::IsDisconnected(void)
{
	return ses_strDisconnected!="";
}

// print an incoming chat message to console
void CSessionState::PrintChatMessage(ULONG ulFrom, const CTString &strFrom, const CTString &strMessage)
{
	CTString strSender;
	// if no sender players
	if (ulFrom==0) {
		// take symbolic sender string
		strSender = strFrom;
	// if there are sender players
	} else {
		// for each sender player
		for(INDEX ipl=0; ipl<_pNetwork->ga_srvServer.srv_apltPlayers.Count(); ipl++) {
			CPlayerTarget &plt = _pNetwork->ga_srvServer.srv_apltPlayers[ipl];
			if (plt.IsActive() && (ulFrom & (1UL<<ipl)) ) {
				// add its name to the sender list
				if (strSender!="") {
					strSender+=", ";
				}
				strSender+=plt.plt_penPlayerEntity->GetPlayerName();
			}
		}
	}

	// let eventual script addon process the message
	extern CTString cmd_strChatSender ;
	extern CTString cmd_strChatMessage;
	extern CTString cmd_cmdOnChat;
	cmd_strChatSender = strSender;
	cmd_strChatMessage = strMessage;
	if (cmd_cmdOnChat!="") {
		_pShell->Execute(cmd_cmdOnChat);
	}

	// if proccessing didn't kill it
	if (cmd_strChatSender!="" && cmd_strChatMessage!="") {
		// print the message
		CPrintF("%s: ^o^cFFFFFF%s^r\n", (const char*)RemoveSpecialCodes(cmd_strChatSender), (const char*)RemoveSpecialCodes(cmd_strChatMessage));
	}
	extern INDEX net_ctChatMessages;
	net_ctChatMessages++;
}

/* NOTES:
1) New thinkers might be added by current ones, but it doesn't matter,
since they must be added forward in time and the list is sorted, so they
cannot be processed in this tick.
2) Thinkers/Movers can be removed during iteration, but the CEntityPointer
guarantee that they are not freed from memory.
*/

// do physics for a game tick
void CSessionState::HandleMovers(void)
{
//  CPrintF("---- tick %g\n", _pTimer->CurrentTick());
	// put all movers in active list, pushing ones first
	CListHead lhActiveMovers, lhDoneMovers, lhDummyMovers;
	{FORDELETELIST(CMovableEntity, en_lnInMovers, _pNetwork->ga_World.wo_lhMovers, itenMover) {
		CMovableEntity *pen = itenMover;
		pen->en_lnInMovers.Remove();
		if (!(pen->en_ulFlags&ENF_DELETED)) {
			if ((pen->en_ulPhysicsFlags&EPF_ONBLOCK_MASK)==EPF_ONBLOCK_PUSH) {
				lhActiveMovers.AddHead(pen->en_lnInMovers);
			} else {
				lhActiveMovers.AddTail(pen->en_lnInMovers);
			}
		}
	}}


	// for each active mover
	{FORDELETELIST(CMovableEntity, en_lnInMovers, lhActiveMovers, itenMover) {
		// let it clear its temporary variables    

		if (itenMover->IsPlayer() && !itenMover->IsActionApplied()) {
			continue;
		}
		itenMover->ClearMovingTemp();
	}}

	// for each active mover
	{FORDELETELIST(CMovableEntity, en_lnInMovers, lhActiveMovers, itenMover) {
		// let it calculate its wanted parameters for this tick
		if (itenMover->IsPlayer() && !itenMover->IsActionApplied()) {
			continue;
		}
		itenMover->PreMoving();
	}}

 

	// while there are some active movers
	while(!lhActiveMovers.IsEmpty()) {
		// get first one
		CMovableEntity *penMoving = LIST_HEAD(lhActiveMovers, CMovableEntity, en_lnInMovers);
		CEntityPointer penCurrent = penMoving;  // just to keep it alive around the loop
		// first move it to done list (if not done, it will move back to world's movers)
		penMoving->en_lnInMovers.Remove();
		lhDoneMovers.AddTail(penMoving->en_lnInMovers);

		// let it do its own physics
		if (penMoving->IsPlayer() && !penMoving->IsActionApplied()) {
//      lhActiveMovers.MoveList(_pNetwork->ga_World.wo_lhMovers);
			continue;
		}

		penMoving->DoMoving();

		// if any mover is re-added, put it to the end of active list
		lhActiveMovers.MoveList(_pNetwork->ga_World.wo_lhMovers);
	}


	// for each done mover
	{FORDELETELIST(CMovableEntity, en_lnInMovers, lhDoneMovers, itenMover) {
		// let it calculate its parameters after all movement has been resolved
		if (itenMover->IsPlayer() && !itenMover->IsActionApplied()) {
			continue;
		}
		itenMover->PostMoving();
	}}

 

	// for each done mover

	{FORDELETELIST(CMovableEntity, en_lnInMovers, lhDoneMovers, itenMover) {
		CMovableEntity *pen = itenMover;
		// if marked for removing from list of movers
		if (pen->en_ulFlags&ENF_INRENDERING) {
			// remove it
			pen->en_ulFlags&=~ENF_INRENDERING;
			pen->en_lnInMovers.Remove();

			// this means that the entity has stopped moving, so 
			// force sending an update to the client
			if (pen->IsSentOverNet() && !pen->IsPlayer()) {
				ses_emEntityMessage.WritePlacementNotify(pen->en_ulID,TRUE);      
				_pNetwork->ga_srvServer.SendMessage(ses_emEntityMessage);
			}

		}
		// let it clear its temporary variables
		if (itenMover->IsPlayer() && !itenMover->IsActionApplied()) {
			continue;
		}
		pen->ClearMovingTemp();
	}}
	
	// return all done movers to the world's list
	_pNetwork->ga_World.wo_lhMovers.MoveList(lhDummyMovers);
	_pNetwork->ga_World.wo_lhMovers.MoveList(lhDoneMovers);

	// handle all the sent events
	CEntity::HandleSentEvents();
}


// let the client move the entities around
void CSessionState::HandleMovers_client(void)
{
	ASSERT(!_pNetwork->IsServer());

//  CPrintF("---- tick %g\n", _pTimer->CurrentTick());
	// put all movers in active list, pushing ones first
	CListHead lhActiveMovers, lhDoneMovers, lhDummyMovers;
	{FORDELETELIST(CMovableEntity, en_lnInMovers, _pNetwork->ga_World.wo_lhMovers, itenMover) {
		CMovableEntity *pen = itenMover;
		pen->en_lnInMovers.Remove();
		if (!(pen->en_ulFlags&ENF_DELETED)) {
			if ((pen->en_ulPhysicsFlags&EPF_ONBLOCK_MASK)==EPF_ONBLOCK_PUSH) {
				lhActiveMovers.AddHead(pen->en_lnInMovers);
			} else {
				lhActiveMovers.AddTail(pen->en_lnInMovers);
			}
		}
	}}
	
	// for each active mover
	{FORDELETELIST(CMovableEntity, en_lnInMovers, lhActiveMovers, itenMover) {
		// let it clear its temporary variables
		itenMover->ClearMovingTemp();
	}}

	// for each active mover
	{FORDELETELIST(CMovableEntity, en_lnInMovers, lhActiveMovers, itenMover) {
		// if this is the client and this is a locally created entity
		if (itenMover->IsSentOverNet()) {
			continue;
		}
		itenMover->PreMoving();
	}}

	// while there are some active movers
	while(!lhActiveMovers.IsEmpty()) {
		// get first one
		CMovableEntity *penMoving = LIST_HEAD(lhActiveMovers, CMovableEntity, en_lnInMovers);
		CEntityPointer penCurrent = penMoving;  // just to keep it alive around the loop
		// first move it to done list (if not done, it will move back to world's movers)
		penMoving->en_lnInMovers.Remove();
		lhDoneMovers.AddTail(penMoving->en_lnInMovers);
		
		//lhActiveMovers.MoveList(_pNetwork->ga_World.wo_lhMovers);

		// let it do its own physics                            
		if (penMoving->IsSentOverNet()) {
			penMoving->ApplyDeadReckoning();     
			continue;
		}

		penMoving->DoMoving();

		// if any mover is re-added, put it to the end of active list
		lhActiveMovers.MoveList(_pNetwork->ga_World.wo_lhMovers);
	}

	// for each done mover
	{FORDELETELIST(CMovableEntity, en_lnInMovers, lhDoneMovers, itenMover) {
		// let it calculate its parameters after all movement has been resolved
		if (itenMover->IsSentOverNet()) {
			continue;
		}
		itenMover->PostMoving();
	}}


	// for each done mover
	{FORDELETELIST(CMovableEntity, en_lnInMovers, lhDoneMovers, itenMover) {
		CMovableEntity *pen = itenMover;
		// if marked for removing from list of movers
		if (pen->en_ulFlags&ENF_INRENDERING) {
			// remove it
			pen->en_ulFlags&=~ENF_INRENDERING;
			pen->en_lnInMovers.Remove();
		}
		// let it clear its temporary variables
		pen->ClearMovingTemp();
	}}
	
	// return all done movers to the world's list
	_pNetwork->ga_World.wo_lhMovers.MoveList(lhDummyMovers);
	_pNetwork->ga_World.wo_lhMovers.MoveList(lhDoneMovers);
}


// do thinking for a game tick
void CSessionState::HandleTimers(TIME tmCurrentTick)
{
#define TIME_EPSILON 0.0001f
	IFDEBUG(TIME tmLast = 0.0f);

	// repeat
	CListHead &lhTimers = _pNetwork->ga_World.wo_lhTimers;
	FOREVER {
		// no entity found initially
		CRationalEntity *penTimer = NULL;
		// for each entity in list of timers
		FOREACHINLIST(CRationalEntity, en_lnInTimers, lhTimers, iten) {
			// if due after current time
			if(iten->en_timeTimer>tmCurrentTick+TIME_EPSILON) {
				// stop searching
				break;
			}
			// remember found entity, and stop searching
			penTimer = iten;
			break;
		}

		// if no entity is found
		if (penTimer==NULL) {
			// stop
			break;
		}
		
		// if this entity isn't locally created, remove it from the timer list and skip it
		if (!cm_ciLocalClient.ci_bClientLocal && cm_ciLocalClient.ci_bUsed && penTimer->IsSentOverNet() && penTimer->IsFlagOff(ENF_CLIENTHANDLING)) {
			// remove the timer from the list
			penTimer->en_timeTimer = THINKTIME_NEVER;
			penTimer->en_lnInTimers.Remove();
			continue;
		}
		 
		extern INDEX net_bReportMiscErrors;
		// check that timers are propertly handled - client doesn't do thinking, so it doesn't check
//안태훈 수정 시작	//(Zone Change System)(0.1)
		//if (_pNetwork->IsServer()) {
			//ASSERT(penTimer->en_timeTimer>tmCurrentTick-_pTimer->TickQuantum-TIME_EPSILON);
		//}
//안태훈 수정 끝	//(Zone Change System)(0.1)
		//ASSERT(penTimer->en_timeTimer>=tmLast);
		IFDEBUG(tmLast=penTimer->en_timeTimer);

		// remove the timer from the list
		penTimer->en_timeTimer = THINKTIME_NEVER;
		penTimer->en_lnInTimers.Remove();
		// send timer event to the entity
		penTimer->SendEvent(ETimer());
	}

	// handle all the sent events
	CEntity::HandleSentEvents();
}


// do a warm-up run of the world for a few ticks
void CSessionState::WarmUpWorld(void)
{
#define WORLD_WORMUP_COUNT 20   // run 20 ticks to get stable
	ses_tmLastProcessedTick = _pNetwork->ga_srvServer.srv_tmLastProcessedTick = 0;
	ses_iLastProcessedSequence = _pNetwork->ga_srvServer.srv_iLastProcessedSequence = -1;
	// add a few empty all-action messages to the game stream
	for (INDEX iTick=0; iTick<WORLD_WORMUP_COUNT; iTick++) {
		_pNetwork->ga_srvServer.srv_tmLastProcessedTick += _pTimer->TickQuantum;
		_pNetwork->ga_srvServer.srv_iLastProcessedSequence++;
		CNetworkStreamBlock nsbAllActions(MSG_SEQ_ALLACTIONS, _pNetwork->ga_srvServer.srv_iLastProcessedSequence);
		nsbAllActions<<_pNetwork->ga_srvServer.srv_tmLastProcessedTick;
		nsbAllActions.Rewind();
		ses_nsGameStream.AddBlock(nsbAllActions);
	}

	// process the blocks
	ProcessGameStream();
}


/*
 * Process a game tick.
 */

void CSessionState::ProcessGameTick(CNetworkMessage &nmMessage, TIME tmCurrentTick)
{
	ASSERT(this!=NULL);

	// copy the tick to process into tick used for all tasks
	_pTimer->SetCurrentTick(tmCurrentTick);
	_pfNetworkProfile.IncrementAveragingCounter();

	// random is allowed only here, during entity ai
	ses_bAllowRandom = TRUE;

	// clear the applied action flag for all players
	for (INDEX ctPlayers=0;ctPlayers<NET_MAXGAMEPLAYERS;ctPlayers++) {
		if (_pNetwork->ga_srvServer.srv_apltPlayers[ctPlayers].plt_penPlayerEntity != NULL) {
			_pNetwork->ga_srvServer.srv_apltPlayers[ctPlayers].plt_penPlayerEntity->ClearActionApplied();
		}
	}

	// for all players in game
	INDEX iPlayer = 0;
	CPlayerAction paAction;
	FOREACHINSTATICARRAY(_pNetwork->ga_srvServer.srv_apltPlayers, CPlayerTarget, itplt) 
	{
		// if player is active
		if (itplt->IsActive()) 
		{
			// while there are actions in this player's buffer
			while(itplt->HasBufferedActions()) 
			{
				itplt->GetActionPacket(paAction);

				CPlayerTarget *pptTarget = itplt;
				// if this is action number two for a certian player, perform it's moving operations      
				if (pptTarget->plt_penPlayerEntity->IsActionApplied()) 
				{
					pptTarget->plt_penPlayerEntity->ClearMovingTemp();
					pptTarget->plt_penPlayerEntity->PreMoving();
					pptTarget->plt_penPlayerEntity->DoMoving(); 
					pptTarget->plt_penPlayerEntity->PostMoving();
					pptTarget->plt_penPlayerEntity->ClearMovingTemp();
					CEntity::HandleSentEvents();
				}

				//Sleep(1);
				// apply the new action and mark it
				pptTarget->ApplyActionPacket(paAction);
				pptTarget->plt_penPlayerEntity->SetActionApplied();

				ses_emEntityMessage.WritePlayerAction(pptTarget->plt_penPlayerEntity->en_ulID,paAction);
				_pNetwork->ga_srvServer.SendMessage(ses_emEntityMessage);
			}
		}
		iPlayer++;
	}

	// handle all the sent events
	CEntity::HandleSentEvents();

	if (_pNetwork->IsServer() || _pNetwork->ga_bDemoPlay) {
		// do thinking
		HandleTimers(tmCurrentTick);

		// do physics
		HandleMovers();
	} 

	// notify all entities of level change as needed
	if (_lphCurrent==LCP_INITIATED) {
		EPreLevelChange ePreChange;
		ePreChange.iUserData = _pNetwork->ga_iNextLevelUserData;
		SendLevelChangeNotification(ePreChange);
		CEntity::HandleSentEvents();
		_lphCurrent=LCP_SIGNALLED;
	}
	if (_lphCurrent==LCP_CHANGED) {
		EPostLevelChange ePostChange;
		ePostChange.iUserData = _pNetwork->ga_iNextLevelUserData;
		SendLevelChangeNotification(ePostChange);
		CEntity::HandleSentEvents();
		_lphCurrent=LCP_NOCHANGE;
	}

	// let the worldbase execute its tick function
	if (_pNetwork->ga_World.wo_pecWorldBaseClass!=NULL
		&&_pNetwork->ga_World.wo_pecWorldBaseClass->ec_pdecDLLClass!=NULL
		&&_pNetwork->ga_World.wo_pecWorldBaseClass->ec_pdecDLLClass->dec_OnWorldTick!=NULL) {
		_pNetwork->ga_World.wo_pecWorldBaseClass->ec_pdecDLLClass->
			dec_OnWorldTick(&_pNetwork->ga_World);
	}
	// handle all the sent events
	CEntity::HandleSentEvents();

	ses_bAllowRandom = FALSE;

	// let eventual script do something on each tick
	extern FLOAT cmd_tmTick;
	extern CTString cmd_cmdOnTick;
	if (cmd_cmdOnTick!="") {
		cmd_tmTick = tmCurrentTick;
		_pShell->Execute(cmd_cmdOnTick);
	}
}

//! 게임스트림을 처리한다.
/*
 * Process all eventual available gamestream blocks.
 */
void CSessionState::ProcessGameStream(void)
{

// this is actually a compiler kludge - it messes up the stack pointer when compiling debug code 
// this kludge gets rid of it 
#ifndef NDEBUG
	CSetFPUPrecision FPUPrecision(FPT_64BIT);  
#endif 

	// repeat
	FOREVER {
		// calculate index of next expected sequence
		INDEX iSequence = ses_iLastProcessedSequence+1;
		// get the stream block with that sequence
		CNetworkStreamBlock *pnsbBlock;
	//서버로부터 온 게임스트림에서 순서대로 블록을 읽어들인다.
		CNetworkStream::Result res = ses_nsGameStream.GetBlockBySequence(iSequence, pnsbBlock);
		// if it is found
		if (res==CNetworkStream::R_OK) 
		{
			// remember the message type
			int iMsgType=pnsbBlock->GetType();
			// remember the processed sequence
			ses_iLastProcessedSequence = iSequence;
			// process the stream block
			ProcessGameStreamBlock(*pnsbBlock);
			// remove the block from the stream
			pnsbBlock->RemoveFromStream();
			// delete network stream block
			DeleteNetworkStreamBlock(pnsbBlock);
			// remove eventual resent blocks that have already been processed
			ses_nsGameStream.RemoveOlderBlocksBySequence(ses_iLastProcessedSequence-2);

			// if the message is all actions
			if (iMsgType==MSG_SEQ_ALLACTIONS) {
				// if playing a demo
				if (_pNetwork->ga_bDemoPlay) {
					// step demo sequence
					ses_tmLastDemoSequence+=_pTimer->TickQuantum;
				}
			}

		// if it is not avaliable yet
		} if (res==CNetworkStream::R_BLOCKNOTRECEIVEDYET) {
			// finish
			return;
	//! 여긴 클라이언트만 들어온다.
		// if it is missing
		} else if (res==CNetworkStream::R_BLOCKMISSING) {
			
			// if it is a new sequence
			if (iSequence>ses_iMissingSequence) {
				ses_iMissingSequence = iSequence;
				// setup timeout
				ses_tvResendTime = _pTimer->GetHighPrecisionTimer();
				ses_tmResendTimeout = 0.1f;
			}

			// if timeout occured
			if (_pTimer->GetHighPrecisionTimer()>ses_tvResendTime+CTimerValue(ses_tmResendTimeout)) {

				_pNetwork->AddNetGraphValue(NGET_MISSING, 1.0f); // missing sequence

				// find how many are missing
				INDEX iNextValid = ses_nsGameStream.GetOldestSequenceAfter(iSequence);
				INDEX ctSequences = Max(iNextValid-iSequence, INDEX(1));

					// create a request for resending the missing packet
				CNetworkMessage nmResendRequest((UBYTE)MSG_REQUESTGAMESTREAMRESEND);
				nmResendRequest<<iSequence;
				nmResendRequest<<ctSequences;
				// send the request to the server
				_pNetwork->SendToServer(nmResendRequest);

				extern INDEX net_bReportMiscErrors;
				if (net_bReportMiscErrors) {
					CPrintF(TRANS("Session State: Missing sequences %d-%d(%d) timeout %g\n"), 
						iSequence, iSequence+ctSequences-1, ctSequences, ses_tmResendTimeout);
				}

				// increase the timeout
				ses_tvResendTime = _pTimer->GetHighPrecisionTimer();
				ses_tmResendTimeout *= 2.0f;
			}

			// finish
			return;
		}
	}

}

//! 게임스트림 블록을 처리.
/*
 * Process a gamestream block.
 */
void CSessionState::ProcessGameStreamBlock(CNetworkMessage &nmMessage)
{
	// copy the tick to process into tick used for all tasks
	_pTimer->SetCurrentTick(ses_tmLastProcessedTick);

	// check message type
	switch (nmMessage.GetType()) 
	{
/* //0522 kwon 삭제.
	// if removing a player
	case MSG_SEQ_REMPLAYER: {
			_pNetwork->AddNetGraphValue(NGET_NONACTION, 1.0f); // non-action sequence
			INDEX iPlayer;
			nmMessage>>iPlayer;      // player index

			if (_pNetwork->ga_srvServer.srv_apltPlayers[iPlayer].plt_penPlayerEntity == NULL) {
				extern INDEX net_bReportMiscErrors;
				if (net_bReportMiscErrors) {
					CPrintF(TRANS("Warning: trying to remove a non-existing player, index: %d\n"),iPlayer);
				}
				break;
			}

			// inform entity of disconnnection
			CPrintF(TRANS("%s left\n"), _pNetwork->ga_srvServer.srv_apltPlayers[iPlayer].plt_penPlayerEntity->GetPlayerName());
			_pNetwork->ga_srvServer.srv_apltPlayers[iPlayer].plt_penPlayerEntity->Disconnect();
			// deactivate the player
			_pNetwork->ga_srvServer.srv_apltPlayers[iPlayer].Deactivate();
			// handle all the sent events
			ses_bAllowRandom = TRUE;
			CEntity::HandleSentEvents();
			ses_bAllowRandom = FALSE;

		} break;
*/
	// if changing character
	case MSG_SEQ_CHARACTERCHANGE: 
		{
			_pNetwork->AddNetGraphValue(NGET_NONACTION, 1.0f); // non-action sequence
			INDEX iPlayer;
			CPlayerCharacter pcCharacter;
			nmMessage>>iPlayer>>pcCharacter;

			// change the character
			_pNetwork->ga_srvServer.srv_apltPlayers[iPlayer].plt_penPlayerEntity->CharacterChanged(pcCharacter);

			// handle all the sent events
			ses_bAllowRandom = TRUE;
			CEntity::HandleSentEvents();
			ses_bAllowRandom = FALSE;
			
			ses_emEntityMessage.WriteCharacterChange(_pNetwork->ga_srvServer.srv_apltPlayers[iPlayer].plt_penPlayerEntity->en_ulID,pcCharacter);
			_pNetwork->ga_srvServer.SendMessage(ses_emEntityMessage);
			
		} break;

	  //! 클라이언트 액션을 받았다면,
	// if receiving client actions
	case MSG_SEQ_ALLACTIONS: 
		{
			// read time from packet
			TIME tmPacket;
			nmMessage>>tmPacket;    // packet time

			// time must be greater by one than that on the last packet received
			TIME tmTickQuantum = _pTimer->TickQuantum;
			TIME tmPacketDelta = tmPacket-ses_tmLastProcessedTick;
			if(! (Abs(tmPacketDelta-tmTickQuantum) < (tmTickQuantum/10.0f)) ) {
				// report debug info
				
				extern INDEX net_bReportMiscErrors;
				if (net_bReportMiscErrors) {
					CPrintF(TRANS("Session state: Mistimed MSG_ALLACTIONS: Last received tick %g, this tick %g\n"),
					ses_tmLastProcessedTick, tmPacket);
				}
			}
			// remember the received tick
			ses_tmLastProcessedTick = tmPacket;
			
/* //0522 kwon 삭제.
			if (_pNetwork->IsServer() && _pNetwork->ga_srvServer.srv_bSendPause && _pNetwork->ga_srvServer.srv_bPauseValue) {
				ses_emEntityMessage.WritePause(_pNetwork->ga_srvServer.srv_bPauseValue,_pNetwork->ga_srvServer.srv_strPauser);
				_pNetwork->ga_srvServer.SendMessage(ses_emEntityMessage);
				_pNetwork->ga_srvServer.srv_bSendPause = FALSE;
				_pNetwork->ga_srvServer.srv_bPause = _pNetwork->ga_srvServer.srv_bPauseValue;
				CNetworkStreamBlock nsbPause(MSG_SEQ_PAUSE, ++_pNetwork->ga_srvServer.srv_iLastProcessedSequence);
				nsbPause<<(INDEX&)_pNetwork->ga_srvServer.srv_bPause;
				nsbPause<<_pNetwork->ga_srvServer.srv_strPauser;        
				// put the message in buffer
				_pNetwork->ga_srvServer.srv_assoSessions[0].sso_nsBuffer.AddBlock(nsbPause);
			}
*/
			INDEX iActiveSessions = 0;
			for (INDEX iSession=1;iSession < _pNetwork->ga_srvServer.srv_assoSessions.Count();iSession++) {
				if (_pNetwork->ga_srvServer.srv_assoSessions[iSession].sso_bTrackData) {
					iActiveSessions++;
				}
			}

			_pNetwork->ga_srvServer.StartNewTick(ses_tmLastProcessedTick);
			if (_pNetwork->ga_srvServer.srv_bRunning) {       
				_pNetwork->ga_srvServer.srv_embSendBuffer.RequestTickAcknowledge(ses_tmLastProcessedTick,iActiveSessions);
			}

			// NOTE: if we got a tick, it means that all players have joined
			// don't wait for new players any more
			ses_bWaitAllPlayers = FALSE;

			// process the tick
			ProcessGameTick(nmMessage, tmPacket);
	
		} break;
/* //0522 kwon 삭제.
	// if receiving pause message
	case MSG_SEQ_PAUSE: 
	{
		_pNetwork->AddNetGraphValue(NGET_NONACTION, 1.0f); // non-action sequence

		BOOL bPauseBefore = ses_bPause;
		// read the pause state and pauser from it
		nmMessage>>(INDEX&)ses_bPause;
		CTString strPauser;
		nmMessage>>strPauser;
		// if paused by some other machine
		if (strPauser!=TRANS("Local machine")) {
			// report who paused
			if (ses_bPause!=bPauseBefore) {
				if (ses_bPause) {
					CPrintF(TRANS("Paused by '%s'\n"), strPauser);
				} else {
					CPrintF(TRANS("Unpaused by '%s'\n"), strPauser);
				}
			}
		}
		// must keep wanting current state
		ses_bWantPause = ses_bPause;
													} break;
*/
	// otherwise
	default:
		// error
		ASSERT(FALSE);
	}
}

//-----------------------------------------------------------------------------
// Purpose: attach a player entity to this session 
// Input  : iPlayerIndex - 
//			&pcCharacter - 
//			&ulEntityID - 
//			&ulWeaponsID - 
//			&ulAnimatorID - 
//			iClient - 
//-----------------------------------------------------------------------------
void CSessionState::AddPlayer(
							  INDEX iPlayerIndex, 
							  CPlayerCharacter &pcCharacter, 
							  ULONG &ulEntityID, 
							  ULONG &ulWeaponsID, 
							  ULONG &ulAnimatorID, 
							  INDEX iClient) 
{
	ASSERT(iPlayerIndex >=0);

	INDEX iPlayerWithName = 0;
	BOOL bFound = FALSE;
	CPlayerEntity *penNewPlayer = NULL;
	

	// if this is single player
	if (ses_ctMaxPlayers == 1) 
	{
		// get the first player entity in the world
		penNewPlayer = _pNetwork->GetNthPlayerEntity(0);
		_pNetwork->ga_srvServer.srv_apltPlayers[0].plt_bActive = FALSE;
	} 
		// if multiplayer
	else 
	{
		// try to find a player entity with the same name as this player character
		// 같은 이름의 플레이어 엔티티가 있는지 찾아보기.
		do 
		{
			penNewPlayer = _pNetwork->GetNthPlayerEntityWithName(pcCharacter.pc_strName, iPlayerWithName);
			bFound = FALSE;
			if (penNewPlayer != NULL) 
			{
				// if the names match, check if it's already active
				for (INDEX iTarget=0;iTarget<NET_MAXGAMEPLAYERS;iTarget++) 
				{
					// and if it is, skip it (someone is already playing with it)
					if (_pNetwork->ga_srvServer.srv_apltPlayers[iTarget].IsActive() &&
							_pNetwork->ga_srvServer.srv_apltPlayers[iTarget].plt_penPlayerEntity == penNewPlayer)
					{
						bFound = TRUE;
						break;
					}
				}
			}
			iPlayerWithName++;
		} while (bFound);
	}

	// activate the player target
	_pNetwork->ga_srvServer.srv_apltPlayers[iPlayerIndex].Activate(iClient);
	pcCharacter.pc_iPlayerIndex = iPlayerIndex;

	// if there is no entity with that character in the world
	// 월드내에 캐릭터 엔티티가 존재하지 않는 경우.
	if (penNewPlayer==NULL) 
	{
		// create an entity for it
		CPlacement3D pl(FLOAT3D(0.0f,0.0f,0.0f), ANGLE3D(0,0,0));
		try 
		{
			CTFileName fnmPlayer = CTString("Classes\\Player.ecl"); // this must not be a dependency!
			penNewPlayer = (CPlayerEntity*)(_pNetwork->ga_World.CreateEntity_t(pl, fnmPlayer,ulEntityID,FALSE));
			// attach entity to client data
			_pNetwork->ga_srvServer.srv_apltPlayers[iPlayerIndex].AttachEntity(penNewPlayer);
			// attach the character to it
			penNewPlayer->en_pcCharacter = pcCharacter;
			// initialize the player      
			penNewPlayer->InitializePlayer(ulWeaponsID,ulAnimatorID);      
		} 
		catch (char *strError) 
		{
			(void)strError;
			FatalError(TRANS("Cannot load Player class:\n%s"), strError);
		}
		if (!_pNetwork->IsPlayerLocal(penNewPlayer)) 
		{
			CPrintF(TRANS("%s joined\n"), (const char*) RemoveSpecialCodes(penNewPlayer->en_pcCharacter.GetNameForPrinting()));
		}
	} 
	else 
	{
		// attach entity to client data
		_pNetwork->ga_srvServer.srv_apltPlayers[iPlayerIndex].AttachEntity(penNewPlayer);
		// make it update its character
		penNewPlayer->CharacterChanged(pcCharacter);

		if (!_pNetwork->IsPlayerLocal(penNewPlayer)) 
		{
			CPrintF(TRANS("%s rejoined\n"), (const char*) RemoveSpecialCodes(penNewPlayer->en_pcCharacter.GetNameForPrinting()));
		}
	}

	ulEntityID = penNewPlayer->en_ulID;

}




// Set lerping factor for current frame.
void CSessionState::SetLerpFactor(CTimerValue tvNow)
{
	TIME tmLastTick;
	if (_pNetwork->IsServer()) 
	{
		tmLastTick = ses_tmLastProcessedTick;
	} 
	else 
	{
		tmLastTick = _tmLocalTick;
	} 
	
//강동민 수정 시작 로그인 처리 작업	07.13
#ifndef FINALVERSION
	if (_pNetwork->IsPlayingDemo()) 
	{
		tvNow = CTimerValue(_pNetwork->ga_fDemoTimer);
		tmLastTick = _pNetwork->ga_tmNextDemoTick - _pTimer->TickQuantum;
	}
#endif	// FINALVERSION
//강동민 수정 끝 로그인 처리 작업		07.13
	
	// if no lerping
	if (!net_bLerping) 
	{
		// set lerping factor without lerping
		_pTimer->SetLerp(1.0f);
		_pTimer->SetCurrentTick(tmLastTick);
		return;
	}
	
	FLOAT fFactor  = 0.0f;
	FLOAT fFactor2 = 0.0f;
	
	// ---- primary factor - used for prediction
	{
		// if lerping was never set before
		if (ses_tmInitializationTick<0) {
			// initialize it
			ses_tvInitialization = tvNow;
			ses_tmInitializationTick = tmLastTick;
		}

		// get passed time from session state starting in precise time and in ticks
		FLOAT tmRealDelta = FLOAT((tvNow-ses_tvInitialization).GetSeconds())
			*_pNetwork->ga_fGameRealTimeFactor*_pNetwork->ga_sesSessionState.ses_fRealTimeFactor;
		FLOAT tmTickDelta = tmLastTick-ses_tmInitializationTick;
		// calculate factor
		fFactor = 1.0f-(tmTickDelta-tmRealDelta)/_pTimer->TickQuantum;

		// if the factor starts getting below zero
		if (fFactor<0) {
			//CPrintF("Lerp=%.2f <0 @ %.2fs\n", fFactor, tmLastTick);
			// clamp it
			fFactor = 0.0f;
			// readjust timers so that it gets better
			ses_tvInitialization = tvNow;
			ses_tmInitializationTick = tmLastTick-_pTimer->TickQuantum;
		}
		if (fFactor>1) {
			//CPrintF("Lerp=%.2f >1 @ %.2fs\n", fFactor, tmLastTick);
			// clamp it
			fFactor = 1.0f;
			// readjust timers so that it gets better
			ses_tvInitialization = tvNow;
			ses_tmInitializationTick = tmLastTick;
		}

		#if DEBUG_LERPING

			avfStats[ctCounter][0] = tmRealDelta/_pTimer->TickQuantum;
			avfStats[ctCounter][1] = tmTickDelta/_pTimer->TickQuantum;
			avfStats[ctCounter][2] = fFactor;
			avfStats[ctCounter][3] = (tmLastTick/_pTimer->TickQuantum-1.0f)+fFactor;
			ctCounter++;
			if (ctCounter>=ctMax) {
				ctCounter = 0;
			}
		#endif // DEBUG_LERPING
	}

	// set lerping
	_pTimer->SetLerp(fFactor);
	_pTimer->SetLerp2(fFactor);
	_pTimer->SetCurrentTick(tmLastTick);

	static TIME tmOld = -1.0f;
	if (tmOld>0 && fabs(tmLastTick - tmOld)>0.01) {
//    CPrintF("New tick: %7.4f\n",tmLastTick);
	}
	tmOld = tmLastTick;

}




/*
 * Read session state state from a stream.
 */
void CSessionState::Read_t(CTStream *pstr,BOOL bNetwork)  // throw char *
{
	// read time information and random seed

	//! 버전 읽기
	INDEX iVersion = SESSIONSTATEVERSION_OLD;
	if (pstr->PeekID_t()==CChunkID("SESV")) {
		pstr->ExpectID_t("SESV");
		(*pstr)>>iVersion;
	}
	(*pstr)>>ses_tmLastProcessedTick;
	(*pstr)>>ses_iLastProcessedSequence;
	(*pstr)>>ses_ulRandomSeed;
	(*pstr)>>ses_ulSpawnFlags;
	(*pstr)>>ses_ctMaxPlayers;
	(*pstr)>>ses_bWaitAllPlayers;
	(*pstr)>>ses_bPause;
	(*pstr)>>ses_bGameFinished;
	if (iVersion>=SESSIONSTATEVERSION_WITHBULLETTIME) {
		(*pstr)>>ses_fRealTimeFactor;
	}
	ses_bWaitingForServer = FALSE;
	ses_bWantPause = ses_bPause;
	ses_strDisconnected = "";
	_pTimer->SetCurrentTick(ses_tmLastProcessedTick);

	// read session properties from stream
	(*pstr)>>_pNetwork->ga_strSessionName;
	pstr->Read_t(_pNetwork->ga_aubProperties, NET_MAXSESSIONPROPERTIES);

	//! bNetwork에 FALSE는 안들어오는데? ㅡㅡ?
	if (!bNetwork) {
		// read world and its state
		ReadWorldAndState_t(pstr);
	} else {
//0130 //0203 ska스트링 테이블땜에 잠시 다시 씀.
		ReadState_t(pstr);

	}

}


void CSessionState::ReadState_t(CTStream *pstr)   // throw char *
{

	//! 모든 엔티티에 lock건다.
	// prepare the world for loading 
	_pNetwork->ga_World.LockAll();
	//! 현재의 월드 스테이트를 읽어들인다.
	// read world situation
	_pNetwork->ga_World.ReadState_net_t(pstr);
	//! 플레이어 수를 읽는다.
		// read number of players
	INDEX ctPlayers;
	(*pstr)>>ctPlayers;
	ASSERT(ctPlayers==_pNetwork->ga_srvServer.srv_apltPlayers.Count());
	//! 플레이어 수만큼 스트림에서 플레이어타겟 정보를 읽는다.
	// for all clients
	FOREACHINSTATICARRAY(_pNetwork->ga_srvServer.srv_apltPlayers, CPlayerTarget, itclt) {    
		// read from stream
		itclt->Read_t(pstr);
	}
 //! lock을 해제한다.
	_pNetwork->ga_World.UnlockAll();

};

void CSessionState::ReadWorldAndState_t(CTStream *pstr)   // throw char *
{
	// check engine build disallowing reinit
	BOOL bNeedsReinit;
	_pNetwork->CheckVersion_t(*pstr, FALSE, bNeedsReinit);
	ASSERT(!bNeedsReinit);

	// read world filename from stream
	(*pstr)>>_pNetwork->ga_fnmWorld;

	if (CTFileName(pstr->GetDescription()).FileExt()==".dem" &&
		GetFileTimeStamp_t(pstr->GetDescription())<=GetFileTimeStamp_t(_pNetwork->ga_fnmWorld)) {
		ThrowF_t(
			TRANS("Cannot play demo because file '%s'\n"
			"is older than file '%s'!\n"),
			CTString(pstr->GetDescription()),
			CTString(_pNetwork->ga_fnmWorld));
	}

	// prepare the world for loading
	_pNetwork->ga_World.Clear();
	_pNetwork->ga_World.LockAll();
	// load the world brushes from the world file
	_pNetwork->ga_World.LoadBrushes_t(_pNetwork->ga_fnmWorld);
	// read world situation
	_pNetwork->ga_World.ReadState_t(pstr);

	// create an empty list for relinking timers
	CListHead lhNewTimers;
	// read number of entities in timer list
	pstr->ExpectID_t("TMRS");   // timers
	INDEX ctTimers;
	*pstr>>ctTimers;
//  ASSERT(ctTimers == _pNetwork->ga_World.wo_lhTimers.Count());
	// for each entity in the timer list
	{for(INDEX ienTimer=0; ienTimer<ctTimers; ienTimer++) {
		// read its index in container of all entities
		INDEX ien;
		*pstr>>ien;
		// get the entity
		CRationalEntity *pen = (CRationalEntity*)_pNetwork->ga_World.EntityFromID(ien);
		// remove it from the timer list and add it at the end of the new timer list
		if (pen->en_lnInTimers.IsLinked()) {
			pen->en_lnInTimers.Remove();
			lhNewTimers.AddTail(pen->en_lnInTimers);
		}
	}}
	// use the new timer list instead the old one
	ASSERT(_pNetwork->ga_World.wo_lhTimers.IsEmpty());
	_pNetwork->ga_World.wo_lhTimers.MoveList(lhNewTimers);

	// create an empty list for relinking movers
	CListHead lhNewMovers;
	// read number of entities in mover list
	pstr->ExpectID_t("MVRS");   // movers
	INDEX ctMovers;
	*pstr>>ctMovers;
	ASSERT(ctMovers == _pNetwork->ga_World.wo_lhMovers.Count());
	// for each entity in the mover list
	{for(INDEX ienMover=0; ienMover<ctMovers; ienMover++) {
		// read its index in container of all entities
		INDEX ien;
		*pstr>>ien;
		// get the entity
		CMovableEntity *pen = (CMovableEntity*)_pNetwork->ga_World.EntityFromID(ien);
		// remove it from the mover list and add it at the end of the new mover list
		if (pen->en_lnInMovers.IsLinked()) {
			pen->en_lnInMovers.Remove();
		}
		lhNewMovers.AddTail(pen->en_lnInMovers);
	}}
	// use the new mover list instead the old one
	ASSERT(_pNetwork->ga_World.wo_lhMovers.IsEmpty());
	_pNetwork->ga_World.wo_lhMovers.MoveList(lhNewMovers);

	// read number of players
	INDEX ctPlayers;
	(*pstr)>>ctPlayers;
	ASSERT(ctPlayers==_pNetwork->ga_srvServer.srv_apltPlayers.Count());
	// for all clients
	FOREACHINSTATICARRAY(_pNetwork->ga_srvServer.srv_apltPlayers, CPlayerTarget, itclt) {
		// read from stream
		itclt->Read_t(pstr);
	}

	_pNetwork->ga_World.UnlockAll();
}

void CSessionState::ReadRememberedLevels_t(CTStream *pstr)
{
	pstr->ExpectID_t("RLEV"); // remembered levels
	// read count of remembered levels
	INDEX ctLevels;
	(*pstr)>>ctLevels;
	// for each level
	for(INDEX iLevel=0; iLevel<ctLevels; iLevel++) {
		// create it
		CRememberedLevel *prl = new CRememberedLevel;
		// read it
		(*pstr)>>prl->rl_strFileName;
		//prl->rl_strmSessionState.
		// use readstream() !!!! @@@@

	}
	
};

//! 스트림에 세션 스테이트를 쓴다.
/*
 * Write session state state into a stream.
 */
void CSessionState::Write_t(CTStream *pstr,BOOL bNetwork)  // throw char *
{
	pstr->WriteID_t("SESV");
	(*pstr)<<INDEX(SESSIONSTATEVERSION_WITHBULLETTIME);
	// write time information and random seed
	(*pstr)<<ses_tmLastProcessedTick;
	(*pstr)<<ses_iLastProcessedSequence;
	(*pstr)<<ses_ulRandomSeed;
	(*pstr)<<ses_ulSpawnFlags;
	(*pstr)<<ses_ctMaxPlayers;
	(*pstr)<<ses_bWaitAllPlayers;
	(*pstr)<<ses_bPause;
	(*pstr)<<ses_bGameFinished;
	(*pstr)<<ses_fRealTimeFactor;
	// write session properties to stream
	(*pstr)<<_pNetwork->ga_strSessionName;
	pstr->Write_t(_pNetwork->ga_aubProperties, NET_MAXSESSIONPROPERTIES);


	if (!bNetwork) {
		// read world and its state
		WriteWorldAndState_t(pstr);
	} else {
		WriteState_t(pstr);
	}

}

//! 클라이언트로 보낼 게임 스테이트를 쓴다.
void CSessionState::WriteState_t(CTStream *pstr)   // throw char *
{
	// write world situation
	_pNetwork->ga_World.LockAll();
	//! 월드 스테이트를 쓴다.
	_pNetwork->ga_World.WriteState_net_t(pstr);

	//! 플레이어 수를 쓴다.
	// write players - their number and data for each one
	// write number of clients
	(*pstr)<<_pNetwork->ga_srvServer.srv_apltPlayers.Count();
	// for all clients
	FOREACHINSTATICARRAY(_pNetwork->ga_srvServer.srv_apltPlayers, CPlayerTarget, itclt) {
		// write to stream
		itclt->Write_t(pstr);
	}

	_pNetwork->ga_World.UnlockAll();
}

void CSessionState::WriteWorldAndState_t(CTStream *pstr)   // throw char *
{
	// save engine build
	_pNetwork->WriteVersion_t(*pstr);

	// write world filename to stream
	(*pstr)<<_pNetwork->ga_fnmWorld;

	// write world situation
	_pNetwork->ga_World.LockAll();
	_pNetwork->ga_World.WriteState_t(pstr, TRUE);

	// write number of entities in timer list
	pstr->WriteID_t("TMRS");   // timers
	CListHead &lhTimers = _pNetwork->ga_World.wo_lhTimers;
	*pstr<<lhTimers.Count();
	// for each entity in the timer list
	{FOREACHINLIST(CRationalEntity, en_lnInTimers, lhTimers, iten) {
		// save its index in container
		*pstr<<iten->en_ulID;
	}}

	// write number of entities in mover list
	pstr->WriteID_t("MVRS");   // movers
	CListHead &lhMovers = _pNetwork->ga_World.wo_lhMovers;
	*pstr<<lhMovers.Count();
	// for each entity in the mover list
	{FOREACHINLIST(CMovableEntity, en_lnInMovers, lhMovers, iten) {
		// save its index in container
		*pstr<<iten->en_ulID;
	}}

	// write number of clients
	(*pstr)<<_pNetwork->ga_srvServer.srv_apltPlayers.Count();
	// for all clients
	FOREACHINSTATICARRAY(_pNetwork->ga_srvServer.srv_apltPlayers, CPlayerTarget, itclt) {
		// write to stream
		itclt->Write_t(pstr);
	}

	_pNetwork->ga_World.UnlockAll();
}

void CSessionState::WriteRememberedLevels_t(CTStream *pstr)
{
		// use writestream() !!!! @@@@

};

// remember current level
void CSessionState::RememberCurrentLevel(const CTString &strFileName)
{
	// if level is already remembered
	for(;;) {
		CRememberedLevel *prlOld = FindRememberedLevel(strFileName);
		if (prlOld==NULL) {
			break;
		}
		// remove it
		prlOld->rl_lnInSessionState.Remove();
		delete prlOld;
	}

	// create new remembered level
	CRememberedLevel *prlNew = new CRememberedLevel;
	ses_lhRememberedLevels.AddTail(prlNew->rl_lnInSessionState);
	// remember it
	prlNew->rl_strFileName = strFileName;
	WriteWorldAndState_t(&prlNew->rl_strmSessionState);
}

// find a level if it is remembered
CRememberedLevel *CSessionState::FindRememberedLevel(const CTString &strFileName)
{
	{FOREACHINLIST(CRememberedLevel, rl_lnInSessionState, ses_lhRememberedLevels, itrl) {
		CRememberedLevel &rl = *itrl;
		if (rl.rl_strFileName==strFileName) {
			return &rl;
		}
	}}
	return NULL;
}

// restore some old level
void CSessionState::RestoreOldLevel(const CTString &strFileName)
{
	// find the level
	CRememberedLevel *prlOld = FindRememberedLevel(strFileName);
	// it must exist
	ASSERT(prlOld!=NULL);
	// restore it
	try {
		prlOld->rl_strmSessionState.SetPos_t(0);
		_pTimer->SetCurrentTick(0.0f);
		ReadWorldAndState_t(&prlOld->rl_strmSessionState);
		_pTimer->SetCurrentTick(ses_tmLastProcessedTick);
	} catch (char *strError) {
		FatalError(TRANS("Cannot restore old level '%s':\n%s"), prlOld->rl_strFileName, strError);
	}
	// delete it
	delete prlOld;
}


// forget all remembered levels
void CSessionState::ForgetOldLevels(void)
{
	{FORDELETELIST(CRememberedLevel, rl_lnInSessionState, ses_lhRememberedLevels, itrl) {
		delete &*itrl;
	}}
}


//! 클라이언트에 로컬 타임을 적용한다.그리고 클라이언트 업데이트를 한다. 사용안함.
// this is used on client side of network game
// adjust the client's local game time and run client updates
void CSessionState::AdjustLocalTime() {
	static CTimerValue tvOld = _pTimer->GetHighPrecisionTimer(); 
	BOOL bPaused = _pNetwork->IsWaitingForPlayers() || ses_bPause;   
	CTimerValue tvNow = _pTimer->GetHighPrecisionTimer();

	//! 데모 플레이 때만 TRUE
	if (ses_bRestartLocalTime) {
		ses_bRestartLocalTime = FALSE;
		tvOld = tvNow;
	}

	if (_pNetwork->IsGameFinished()) {
		return;
	}
	// how many ticks we have to run?
	ULONG ulNumTicks = (tvNow.GetSeconds() - tvOld.GetSeconds())/_pTimer->TickQuantum;
	//! ClampUp:높은것을 압박하다.즉, 둘중에 작은것을 선택한다.
	ULONG ulClampedTicks = ClampUp(ulNumTicks,(ULONG)10);
	//! 새로운 시간은 기억.
	// remember the new time
	tvOld += _pTimer->TickQuantum*ulNumTicks;

	if (ulClampedTicks>0 && !bPaused) {
		// advance local time for that many ticks
		_tmLocalTick += _pTimer->TickQuantum*ulNumTicks;
		// update the current tick
		if (_pNetwork->IsPlayingDemo()) {
			_pTimer->SetCurrentTick(_pNetwork->ga_tmNextDemoTick - _pTimer->TickQuantum);
		} else {
			_pTimer->SetCurrentTick(_tmLocalTick);
		}
		
		if (_cmiComm.cci_bClientInitialized) {
			INDEX ctTicks = 0;
			// for each tick that has passed
			while (ctTicks < ulClampedTicks && !_pNetwork->IsPlayingDemo()) {
				 //! 모든 로컬 플레이어에 대한 액션 패킷을 만들고 서버로 보낸다.
				// make actions packet for all local players and send to server
				_pNetwork->SendActionsToServer();
				ctTicks++;
			}
		}
		extern INDEX cli_iFPSLimit;
		extern INDEX  cli_iCurrentFPS;
		if (cli_iFPSLimit && (cli_iCurrentFPS < cli_iFPSLimit)) {
			SetLerpFactor(_pTimer->GetHighPrecisionTimer());
		}
		// do time events
		if (!_pNetwork->IsPlayingDemo()) {
			HandleTimers(_tmLocalTick);
			// do movement - dead reckoning
			HandleMovers_client();
		}
	}

}


void Cludge_SetLerpFactor() 
{
	// update the current tick
	{CTimerValue tvNow = _pTimer->GetHighPrecisionTimer();
	if (!_pNetwork->ga_bDemoPlay) {
		_pNetwork->ga_sesSessionState.SetLerpFactor(tvNow);
	} else {
		_pNetwork->ga_sesSessionState.SetLerpFactor(CTimerValue(_pNetwork->ga_fDemoTimer));
	}}
}

/*
void CSessionState::RunDemoTick()
{
	static UBYTE aubData[SERVER_MAX_MESSAGE_SIZE];
	SLONG slSize,slCompressedSize;

	ses_bWaitingForServer = FALSE;
	 
	// if this is not an empty tick, read the data to the buffer
	extern UBYTE _aubCompData[SERVER_MAX_MESSAGE_SIZE];
	try {
		_pNetwork->ga_strmDemoPlay.ExpectID_t("DTCK");// demo tick       
		_pNetwork->ga_strmDemoPlay >> slCompressedSize;        
		if (slCompressedSize > 0) {          
			_pNetwork->ga_strmDemoPlay.Read_t(_aubCompData,slCompressedSize);
		}
		_pNetwork->ga_bDemoPlayFinished = FALSE;
	} catch(char *strError) {
		_pNetwork->ga_bDemoPlayFinished = TRUE;
		CPrintF("Error while playing demo: %s\n",strError);
		return;
	}
	slSize = SERVER_MAX_MESSAGE_SIZE;
	CzlibCompressor comp;
	BOOL bOK = comp.Unpack(_aubCompData,slCompressedSize,aubData,slSize);
	ASSERT(bOK);         

	// the compiler messes up the stack in debug if 64 bit integers and 
	// try-catch are used in the same function (stack scope). This is a fix-up cludge. 
	Cludge_SetLerpFactor();

	if (slCompressedSize > 0) {          
		// clear the input buffer
		ses_embReceiveBuffer.Clear();

		// put the tick data into the buffer
		ReceiveTick(_pNetwork->ga_tmNextDemoTick,aubData,slSize);
	
		_vBasePosition    = FLOAT3D(0,0,0);
		_bBasePositionSet = FALSE;

		// if the tick received isn't empty
		if (slSize > 0) {          
			// read and apply all received messages
			while (ReceiveMessage()){};
		}
	}
	
	// update session time
	ses_tmLastProcessedTick = _pNetwork->ga_fDemoTimer;//_tmLocalTick;         
	ses_tmLastTickTime = _pNetwork->ga_fDemoTimer;//_tmLocalTick;

	try {
		CChunkID cid = _pNetwork->ga_strmDemoPlay.PeekID_t();
		if (cid == CChunkID("DTTM")) {
			_pNetwork->ga_strmDemoPlay.ExpectID_t("DTTM");
			_pNetwork->ga_strmDemoPlay>>_pNetwork->ga_tmNextDemoTick;
		} else {
			_pNetwork->ga_strmDemoPlay.ExpectID_t("DEND"); // demo end
			_pNetwork->ga_bDemoPlayFinished = TRUE;
			return;
		}
	} catch(char *strError) {
		_pNetwork->ga_bDemoPlayFinished = TRUE;
		CPrintF("Error while playing demo: %s\n",strError);
	}
};
*/

//0105 함수 추가하기.
void CSessionState::ProcessOneSentence(char *msg)
{
	static char cmd[100];

	msg = AnyOneArg(msg, cmd);
	int cmd_num = m_pCmd->Find(cmd);
	if (cmd_num < 0)
		return;

	m_pCmd->Run(cmd_num, msg);

	return;
}

#define is_space(c)		(c == 0x20 || (c >= 0x09 && c <= 0x0D))
char* CSessionState::AnyOneArg(char *argument, char *first_arg)
{
	SkipSpaces(&argument);

	while (*argument && !is_space(*argument)) {
		*(first_arg++) = *argument;
		argument++;
	}

	*first_arg = '\0';

	return (argument);
}


void CSessionState::SkipSpaces(char **string)
{
	for (; **string && is_space(**string); (*string)++);
}


void CSessionState::StartGame()
{

	char Buf[1024];

	sprintf(Buf, "start 0 0\n");

	_tcpip.SendNetMessage(Buf, TRUE);

	return;
}
//..

/* Session state loop. */
void CSessionState::SessionStateLoop(void)
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	_pfNetworkProfile.StartTimer(CNetworkProfile::PTI_SESSIONSTATE_LOOP);

	// while there is something to do
	BOOL bSomethingToDo = TRUE;

	//	반복문 회수 확인 값.
	int	t_Count_While = 0;

	while (bSomethingToDo && !IsDisconnected()) 
	{
		
		//Sleep(0);				// deleted by seo - 40830
		bSomethingToDo = FALSE;
#if defined (G_KOR)
		//	진입회수 증가.
		t_Count_While++;
#endif

		//! 클라이언트 업데이트
		_cmiComm.Client_Update();					
		CNetworkMessage nmMessage;
		static FLOAT ResponseTime = _pTimer->CurrentTick();

		extern BOOL _bLoginProcess;
		
		// ---- Add national code ----------060711------------->>
		INDEX tv_idx;
		switch(g_iCountry)
		{
		case KOREA:			tv_idx = 0;		break;
		case THAILAND:		tv_idx = 4;		break;
		case USA:			tv_idx = 9;		break;
		case BRAZIL:		tv_idx = 10;	break;
		case ENGLAND:
		case GERMANY:		tv_idx = 13;	break;
		case SPAIN:			tv_idx = 14;	break;
		case FRANCE:		tv_idx = 15;	break;
		case POLAND:		tv_idx = 16;	break;
		case RUSSIA:		tv_idx = 17;	break;
		case ITALY:			tv_idx = 19;	break;
		case MEXICO:		tv_idx = 20;	break;
		default:			tv_idx = g_iCountry;	break;
		}
		// ---- Add national code ----------060711-------------<<

		if(!_bLoginProcess || STAGEMGR()->GetCurStage() == eSTAGE_CREATECHAR || STAGEMGR()->GetCurStage() == eSTAGE_SELCHAR)
		{
			static DWORD AliveTime	= _pTimer->GetHighPrecisionTimer().GetMilliseconds();
			static DWORD SendTime	= _pTimer->GetHighPrecisionTimer().GetMilliseconds();
			static BOOL bIsFirst = TRUE;
			
			DWORD tmpTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();
			
			if(bIsFirst)
			{
				CNetworkMessage nmAliveMsg((UBYTE)MSG_PULSE); 	
				nmAliveMsg << (SLONG)(AliveTime/100);
				// ---- Add national code ----------060711------------->>
				nmAliveMsg << (UBYTE)tv_idx;			
				// ---- Add national code ----------060711-------------<<
				nmAliveMsg << (FLOAT)_pTimer->tmTimeDelay;
				_pNetwork->SendToServerNew(nmAliveMsg);	
				
				bIsFirst = FALSE;
			}
			
			if(tmpTime - AliveTime >= 60000) 
			{
				CNetworkMessage nmAliveMsg((UBYTE)MSG_PULSE); 	
				nmAliveMsg << (SLONG)(tmpTime/100);
				// ---- Add national code ----------060711------------->>
				nmAliveMsg << (UBYTE)tv_idx;			
				// ---- Add national code ----------060711-------------<<
				nmAliveMsg << (FLOAT)_pTimer->tmTimeDelay;
				
				_pNetwork->SendToServerNew(nmAliveMsg);	
				CPrintF("Send Alive MSG...%d \n",tmpTime);

				AliveTime = tmpTime;
			}
#ifdef UPDATE_PLAYTIME
			DWORD tmpDeffTime = tmpTime - _SetPlayStartTime;

			if (tmpDeffTime >= _IntervalPlayCountTime)
			{
				// 플레이 타임 업데이트
				++_PlayTimeCount;
				_pNetwork->SendUpdatePlayTime(_PlayTimeCount * _IntervalTime);
				_SetPlayStartTime = tmpTime;
			}
#endif
			// 싱글던젼 모드의 경우에...
			if( _pNetwork->m_bSingleMode )
			{
				if( tmpTime - SendTime >= 1000 )
				{
					CPrintF( "%ld SendAttack\n", tmpTime );
					//_pNetwork->SendMoveList();
					_pNetwork->SendAttackList();
					SendTime = tmpTime;
				}
			}

/*			if (_pTimer->tmTimeDelay > 0.07f)
			{ // 스피드 핵 사용자 알림 (TickCount는 0.05f가 기본이므로 0.05f보다 빨라진다면 스피드핵 사용자로 본다.
				_pNetwork->SendUsingSpeedHack();
			}*/

		}

		if(_pNetwork->ReceiveFromServerNew(nmMessage))
		{
			bSomethingToDo = TRUE;//0820 이놈의 플래그 m때문에 네트워크가 늦었군..ㅡㅡ;
			ResponseTime = _pTimer->CurrentTick();

#ifdef	VER_TEST
			{
				char*	buf = (char*)nmMessage.GetBuffer();
				if (buf[0] != MSG_MOVE)
				{
					int		sub = buf[1];					

					struct tm* st;
					time_t t = time((time_t*)NULL);
					st = localtime(&t);

					if (buf[0] == MSG_EXTEND)
					{
						sub = ntohl(*((int*)(&buf[1])));

						if (sub < 0 || sub >= MSG_EX_END)
							sub = htonl(sub);

						LOG_DEBUG( "%02d:%02d:%02d Recv MSG(%s[%d] - %s[%d] - %d)", 
							st->tm_hour, st->tm_min, st->tm_sec, 
							strMSG_MAIN[buf[0]], buf[0], strMSG_EXTEND[sub], sub, buf[5] );
					}
					else if (buf[0] == MSG_UPDATE_DATA_FOR_CLIENT)
					{
						if (sub != MSG_SUB_UPDATE_STATUS)
							LOG_DEBUG( "%02d:%02d:%02d Update MSG(%s[%d] - %s[%d])", st->tm_hour, st->tm_min, st->tm_sec, strMSG_MAIN[buf[0]], buf[0], strMSG_UPDATE[sub], sub );
					}
					else
					{
						LOG_DEBUG( "%02d:%02d:%02d Recv MSG(%s[%d] - %d)", st->tm_hour, st->tm_min, st->tm_sec, strMSG_MAIN[buf[0]], buf[0], sub );
					}					
				}
			}
#endif	// VER_TEST

			if(_bLoginProcess)
			{				
				// NOTE : 로그인 시에는 무조건 아래 함수로 들어감.
				// NOTE : 캐릭터 생성 실패시 MSG_FAIL 다음에 캐릭터 슬롯 정보가 바로 오기 때문에
				// NOTE : 서브 타입까지 다 읽은 다음에 처리해야 함.
				ReceiveLoginMessage(&nmMessage);
			}
			else
			{
				UBYTE msg = nmMessage.GetType();
				bool bProcess = false;

#if		!defined(WORLD_EDITOR)
				pTypeBase* pBase = reinterpret_cast<pTypeBase*>(nmMessage.GetBuffer());

				_map_packet_iter iter = m_packet[msg].find(pBase->subType);

				if (iter != m_packet[msg].end())
				{
					if (iter->second != NULL)
					{
						iter->second(&nmMessage);
						bProcess = true;
					}
				}
#endif	// WORLD_EDITOR

				if (bProcess == false)
				{
					switch(msg)
					{
					case MSG_MOVE:
						ReceiveMoveMessage(&nmMessage);
						break;
					case MSG_ATTACK:
						ReceiveAttackMessage(&nmMessage);
						break;
					case MSG_CHAT:
						ReceiveChatMessage(&nmMessage);
						break;
					case MSG_SKILL:
						ReceiveSkillMessage(&nmMessage);
						break;
					case MSG_SSKILL:			// 특수 스킬
						ReceiveSSkillMessage(&nmMessage);
						break;
					case MSG_QUICKSLOT:
						ReceiveQuickSlotMessage(&nmMessage);
						break;
					case MSG_INVENTORY:
						ReceiveInventoryMessage(&nmMessage);
						break;
					case MSG_WEARING:
						((CPlayerEntity*)CEntity::GetPlayerEntity(0))->Read_net_Character(MSG_WEARING, 0, &nmMessage);
						break;
					case MSG_ITEM:
						ReceiveItemMessage(&nmMessage);
						break;
					case MSG_GOTO:
						ReceiveGoToMessage(&nmMessage);
						break;
					case MSG_MEMPOS:
	// [KH_070316] 프리미엄 메모리 관련 추가 및 변경
 					case XXXMSG_MEMPOSPLUS:
 						ReceiveMemPosMessage(&nmMessage);
 						break;
					case MSG_GM:
						ReceiveGmMessage(&nmMessage);
						break;
					case MSG_GO_ZONE:
						ReceiveGoZoneMessage(&nmMessage);
						break;
					case MSG_PARTY:
						ReceivePartyMessage(&nmMessage);
						break;
					case MSG_EXCHANGE:
						ReceiveExchangeMessage(&nmMessage);
						break;
					case MSG_QUEST:
						ReceiveQuestMessage(&nmMessage);
						break;
					case MSG_STATPOINT:
						ReceiveStatMessage(&nmMessage);
						break;
					case MSG_UI:
						ReceiveUIMessage(&nmMessage);
						break;
					case MSG_NPC_REGEN:
						ReceiveNpcRegenMessage(&nmMessage);
						break;
					case MSG_ASSIST:
						ReceiveAssistMessage(&nmMessage);
						break;
					case MSG_PC_REBIRTH:
						ReceiveRebirthMessage(&nmMessage);
						break;
					case MSG_EFFECT:
						ReceiveEffectMessage(&nmMessage);
						break;
					case MSG_SYS:
						ReceiveSystemMessage(&nmMessage);
						break;
					case MSG_EXP_SP:
						ReceiveEXPMessage(&nmMessage);
						break;
					case MSG_ACTION:
						ReceiveActionMessage(&nmMessage);
						break;
					case MSG_ENV://1013
						ReceiveEnvMessage(&nmMessage);
						break;
					case MSG_WARP:
						ReceiveWarpMessage(&nmMessage);
						break;
					case MSG_PK:
						ReceivePkMessage(&nmMessage);
						break;
					case MSG_EVENT:
						ReceiveEventMessage(&nmMessage);
						break;
					case MSG_PERSONALSHOP:
						ReceivePersonalShopMessage(&nmMessage);
						break;
					case MSG_RIGHT_ATTACK:
						ReceiveLegitimateMessage(&nmMessage);
						break;
					case MSG_STASH:
						ReceiveWareHouseMessage(&nmMessage);
						break;
					case MSG_GUILD:
						ReceiveGuildMessage(&nmMessage);
						break;
					case MSG_TEACH:
						ReceiveTeachMessage(&nmMessage);
						break;
					case MSG_CHANGEJOB:
						ReceiveChangeJobMessage(&nmMessage);
						break;
					case MSG_CHANGE:
						((CPlayerEntity*)CEntity::GetPlayerEntity(0))->Read_net_Character(MSG_CHANGE, 0, &nmMessage);
						break;
					case MSG_FRIEND:
						ReceiveMessengerMessage( &nmMessage );
						break;
					case MSG_TRADEAGENT:
						{
							Auction* pAuction = GameDataManager::getSingleton()->GetAuction();
							if (pAuction)
								pAuction->ReceiveTradeAgentMessage(&nmMessage);
						}
						break;
					case MSG_EXPEDITION: // [sora] 원정대 관련
						ReceiveExpeditionMessage( &nmMessage );
						break;
					case MSG_RAID: // [sora] 인스턴트존 메시지
						{
							SBYTE	sbType;
							(nmMessage) >> sbType;

							if( MSG_RAID_ERROR == sbType )
							{
								GAMEDATAMGR()->GetPartyInfo()->RecvRaidError(&nmMessage);
							}
						}
						break;
					case MSG_FACTORY:
						ReceiveFactoryMessage( &nmMessage );
						break;
					//wooss 050818
					case MSG_EXTEND:
						if (RecvExtend(&nmMessage) == false)
						{
							ReceiveExtendMessage( &nmMessage);
						}
						break;
					case MSG_FAIL:
						ReceiveFailMessage(&nmMessage);
						break;

					case MSG_RECOVER_EXP_SP:	// [2012/08/10 : Sora] 부활 주문서 사용시 복구된 경험치, 숙련도
						{
							SQUAD loseExp, loseSp;
							CTString strMessage;
						
							(nmMessage) >> loseExp;
							(nmMessage) >> loseSp;

							if( loseExp > 0 )
							{
								strMessage.PrintF( _S( 5768, "부활주문서3을 소모하여 EXP %I64d 를 복구 하였습니다."), loseExp );
								pUIManager->GetChattingUI()->AddSysMessage( strMessage );
							}
						
							if( loseSp > 0 )
							{
								strMessage.PrintF( _S( 5769, "부활주문서3을 소모하여 SP %I64d 를 복구 하였습니다."), loseSp );
								pUIManager->GetChattingUI()->AddSysMessage( strMessage );
							}

						}
						break;
					case MSG_MONEY :	//2013/04/03 jeil 나스 아이템 삭제
						ReceiveMoneyMessage(&nmMessage);
						break;
	#ifdef HP_PERCENTAGE
					case MSG_DAMAGE_REAL :
						ReceiveDamageRealStatMessage(&nmMessage);
						break;
	#endif // HP_PERCENTAGE

					case MSG_TIMER_ITEM :
						ReceiveTimerItemMessage(&nmMessage);
						break;

					case MSG_EXPRESS_SYSTEM :
						ReceiveExpressMessage(&nmMessage);
						break;

					case MSG_RESERVED_GM_COMMAND :
						ReceiveCalendarMessage(&nmMessage);
						break;
					case MSG_PET_STASH:
						ReceivePetStashMessage(&nmMessage);
						break;
					case MSG_RVR:
						ReceiveRVRMessage(&nmMessage);
						break;
	#ifdef DURABILITY
					case MSG_DURABILITY:
						ReceiveDurabilityMessage(&nmMessage);
						break;
	#endif	//	DURABILITY
					case MSG_ITEMCOLLECTION:
						ReceiveItemCollectionMessage(&nmMessage);
						break;

#ifdef PREMIUM_CHAR
					case MSG_PREMIUM_CHAR:
						ReceivePremiumCharMessage(&nmMessage);
						break;
#endif	//	PREMIUM_CHAR
					default:
						{
							pTypeBase* pBase = reinterpret_cast<pTypeBase*>(nmMessage.GetBuffer());
							CPrintF("Undefined data received!! msgType = %d (%d)\n" , pBase->type, pBase->subType);
						}
						break;
					}
				}
			}
		}
		else
		{
			extern BOOL _bLoginProcess;
			if( _pNetwork->m_bSendMessage && (unsigned int(_pTimer->GetLerpedCurrentTick()*1000)) - _pNetwork->m_uiSendedTime > 15000 )
			{
				if( _bLoginProcess )
				{
					GameDataManager* pGameData = GameDataManager::getSingleton();
					CServerSelect* pServerSelect = NULL;

					if (pGameData != NULL)
						pServerSelect = pGameData->GetServerData();

					//if( pUIManager->GetUIGameState() == UGS_LOGIN && !pUIManager->GetSelServer()->IsEmptyGroup() )
					if( STAGEMGR()->GetCurStage() == eSTAGE_LOGIN && pServerSelect != NULL && pServerSelect->GetServerCount() > 0 )
					{
						_pSound->Mute();
						// NOTE : 로그인 실패시 소켓을 닫고 다시 생성해야하는 부분.				
						_cmiComm.Disconnect();
						
						_pGameState->DisplayErrorMessage( MSG_TIME_OUT, UI_NONE, MSGCMD_LOGIN_ERROR );
						_pNetwork->m_bSendMessage = FALSE;

					}
					else
					//else if(_cmiComm.IsNetworkOn())
					{
						if( !_tcpip.IsConnected(_tcpip.Socket) )
						{
							_pGameState->DisplayErrorMessage( MSG_FAIL_CONNECT_SERVER, UI_NONE, MSGCMD_LOGIN_ERROR );
						}
						else
						{
							_pGameState->DisplayErrorMessage( MSG_TIME_OUT, UI_NONE, MSGCMD_LOGIN_ERROR );
						}
						_pNetwork->m_bSendMessage = FALSE;
					}
				}
			}
		}


		// if there is some unreliable message
		if (_pNetwork->ReceiveFromServer(nmMessage))
		{
			bSomethingToDo = TRUE;
			//! 게임 스트림 메시지라면, 언팩후에 읽어들인다.로컬만 받는다.
			// if it is a gamestream message
			if (nmMessage.GetType() == MSG_GAMESTREAMBLOCKS)
			{
				ses_tvMessageReceived = _pTimer->GetHighPrecisionTimer();
				ses_bWaitingForServer = FALSE;
				
				// unpack the message
				CNetworkMessage nmUnpackedBlocks((UBYTE)MSG_GAMESTREAMBLOCKS);
				nmMessage.UnpackDefault(nmUnpackedBlocks);
				
				// while there are some more blocks in the message
				while (!nmUnpackedBlocks.EndOfMessage()) 
				{
					// read a block to the gamestream
					ses_nsGameStream.ReadBlock(nmUnpackedBlocks);
				}			
			}
			else 
			{
				CPrintF(TRANS("Session state: Unexpected unreliable message during game: %s(%d)\n"),
					ErrorDescription(&MessageTypes, nmMessage.GetType()), nmMessage.GetType());
			}
		}

// #if defined (G_KOR)
// 		if( ( bSomethingToDo ) && ( t_Count_While > PROC_MESSAGE_COUNT_MAX ) )
// 		{
// 			bSomethingToDo = FALSE;
// 		}
// #endif

	}
	_pfNetworkProfile.StopTimer(CNetworkProfile::PTI_SESSIONSTATE_LOOP);
}

/* Get number of active players. */
INDEX CSessionState::GetPlayersCount(void)
{
	INDEX ctPlayers = 0;
	FOREACHINSTATICARRAY(_pNetwork->ga_srvServer.srv_apltPlayers, CPlayerTarget, itplt) {
		if (itplt->IsActive()) {
			ctPlayers++;
		}
	}
	return ctPlayers;
}


// probability (0.0 - 1.0) that the client will drop the tick data (bad network conditions emulation)
extern FLOAT cli_fTickDropRate;


// receive a message from the incoming tick data
BOOL CSessionState::ReceiveMessage() 
{
	ULONG ulEntityID;
	UWORD aubData[1024];
	int   iErr;
	
//	extern INDEX net_bReportClientTraffic;
	
	// read the message from the input buffer
	iErr = ses_embReceiveBuffer.ReadMessage(ses_emEntityMessage);
	
	if (iErr != EMB_SUCCESS_OK) 
	{
		return FALSE;
	} 
	
	// if this message is older than the last fully processed server tick, do not process it
	if ((ses_tmLastTickTime > (ses_emEntityMessage.em_tmMessageTime - (_pTimer->TickQuantum/2))) && !_pNetwork->IsPlayingDemo()) 
	{
		if (net_bReportClientTraffic) 
		{
			//0203 
			//	CPrintF(TRANS("MESSAGE REJECTED! Type: %d, Session time: %5.2f, Message time: %5.2f\n"),ses_emEntityMessage.em_ulType,ses_tmLastTickTime,ses_emEntityMessage.em_tmMessageTime);
		}
		return TRUE;
	}
	
	switch (ses_emEntityMessage.em_ulType) 
	{
		// if adding a new player
	case EMT_ADDPLAYER: 
		{
			INDEX iPlayerIndex,iClient;
			ULONG ulWeaponsID,ulAnimatorID;
			CPlayerCharacter pcCharacter;
			ses_ulMsgId++;
			CEntity* penPlayer;        
			
			// extract neccessary data from the message
			ses_emEntityMessage.ReadAddPlayer(ulEntityID,iPlayerIndex,pcCharacter,ulWeaponsID,ulAnimatorID,iClient);    
			
			if (_pNetwork->ga_World.EntityExists(ulEntityID,penPlayer)) 
			{
				extern INDEX net_bReportMiscErrors;
				if (net_bReportMiscErrors) 
				{
					CPrintF("Warning: Repeated AddPlayer, EntityID: 0x%X\n",ulEntityID);
				}
				break;
			}
			
			// add the player to the game, forcing it to have a received entity ID for CPlayer, CPlayerWeapons and CPlayerAnimator
			// this is because we will need to process messages specifying those ID's later
			AddPlayer(iPlayerIndex,pcCharacter,ulEntityID,ulWeaponsID,ulAnimatorID,iClient);              
			
			// report to console
			if (net_bReportClientTraffic) 
			{
				CPrintF("ADDPLAYER:     %s  EntityID: 0x%X   %6.3f\n  ",(const char*)pcCharacter.pc_strName,ulEntityID,ses_emEntityMessage.em_tmMessageTime);
			}
			
			break;
		}
		//0203 
		/*
		// new placement received for an entity
		case EMT_SETPLACEMENT: 
		{
		CEntity* penEntity;
		CPlacement3D plPlacement,plSpeed;			
		ses_ulMsgId++;		
		//        ASSERT(_bBasePositionSet);
		
		  // extract neccessary data from the message
		  ses_emEntityMessage.ReadPlacement(ulEntityID,plPlacement,plSpeed);      
		  plPlacement.pl_PositionVector += _vBasePosition;
		  
			// if placement received is for an entity that does not exist on the client, 
			// print a warning to the console, and return
			if (!_pNetwork->ga_World.EntityExists(ulEntityID,penEntity)) {
			extern INDEX net_bReportMiscErrors;
			if (net_bReportMiscErrors) {
			CPrintF("Warning: SetPlacement for non existing entity, EntityID: 0x%X\n",ulEntityID);
			}
			break;
			}
			
			  // get the pointer to the specified entity
			  penEntity = _pNetwork->ga_World.EntityFromID(ulEntityID);       
			  
				// it it's a movable entity, adjust it's dead reckoning parameters - position, speed and extrapolation start time
				if (penEntity->en_ulPhysicsFlags & EPF_MOVABLE) {
				CMovableEntity* penMovable = (CMovableEntity*) penEntity;
				if (plSpeed.pl_OrientationAngle.Length() > 0.01 || plSpeed.pl_PositionVector.Length() > 0.01) {
				penMovable->AddToMovers();
				} else {
				if (penMovable->en_lnInMovers.IsLinked()) {
				penMovable->en_lnInMovers.Remove();
				}
				}
				penMovable->AdjustDeadReckoning(plPlacement,plSpeed,ses_emEntityMessage.em_tmMessageTime);
				// if it's not a movable entity, just put it in it's place
				} else {
				penEntity->SetPlacement(plPlacement);
				}
				
				  // report to console
				  if (net_bReportClientTraffic) {
				  CPrintF("%lu: SETPLACEMENT:     EntityID: 0x%X     Position: %f, %f, %f     ",ses_ulMsgId,ulEntityID,plPlacement.pl_PositionVector(1),plPlacement.pl_PositionVector(2),plPlacement.pl_PositionVector(3));
				  CPrintF("Angle: %f, %f, %f\n",plPlacement.pl_OrientationAngle(1),plPlacement.pl_OrientationAngle(2),plPlacement.pl_OrientationAngle(3));
				  }
				  break;
	}*/
		// a new entity should be created on the client
	case EMT_CREATE: 
		{
			UWORD uwClassID;																								 
			CPlacement3D plPlacement;
			CEntity* penEntity;
			EntityClassEntry* peceEntry;
			ses_ulMsgId++;
			
			// get the entity id, placement and class id
			ses_emEntityMessage.ReadEntityCreate(ulEntityID,plPlacement,uwClassID);
			
			// get the pointer to the entity class entry in the class container
			peceEntry = FindClassInContainer(uwClassID);
			// it really should be there
			ASSERT(peceEntry != NULL);
			if (peceEntry == NULL) 
			{
				extern INDEX net_bReportMiscErrors;
				if (net_bReportMiscErrors) 
				{
					CPrintF(TRANS("Warning: Create entity received for an unknown entity class, EntityID: 0x%X\n"),ulEntityID);
				}
				break;
			}
			
			// create the entity with specified data
			penEntity = _pNetwork->ga_World.CreateEntity_t(plPlacement,peceEntry->ece_fnmEntityClass,ulEntityID);
			
			// report to console
			if (net_bReportClientTraffic) 
			{
				CPrintF("Class name: %s\n",peceEntry->ece_fnmEntityClass);          
				CPrintF("%lu: CREATE:           EntityID: 0x%X     EntityClass: %ld",ses_ulMsgId,ulEntityID,uwClassID);
				CPrintF("     Position: %f, %f, %f     ",plPlacement.pl_PositionVector(1),plPlacement.pl_PositionVector(2),plPlacement.pl_PositionVector(3));
				CPrintF("Angle: %f, %f, %f\n",plPlacement.pl_OrientationAngle(1),plPlacement.pl_OrientationAngle(2),plPlacement.pl_OrientationAngle(3));
			}
			
			break;
			
		}
		// copy one entity to another
	case EMT_COPY: 
		{
			ULONG ulTargetID;
			CPlacement3D plPlacementCopy;
			UBYTE ubWithDescentants = 0;
			CEntity* penOriginal;
			//        CEntity* penCopy;
			
			ses_ulMsgId++;
			
			// extract neccessary data from the message
			ses_emEntityMessage.ReadEntityCopy(ulEntityID,ulTargetID,plPlacementCopy,ubWithDescentants);
			
			if (!_pNetwork->ga_World.EntityExists(ulEntityID,penOriginal)) {
				extern INDEX net_bReportMiscErrors;
				if (net_bReportMiscErrors) {
					CPrintF("Warning: Copy from non existing entity, EntityID: 0x%X\n",ulEntityID);
				}
				break;
			}
			ASSERT(penOriginal!=NULL);
			if (penOriginal == NULL) {
				break;
			}
			
			_pNetwork->ga_World.CopyEntityInWorld(*penOriginal,plPlacementCopy,ubWithDescentants,ulTargetID);
			
			// report to console
			if (net_bReportClientTraffic) {
				CPrintF("%lu: COPY:           SourceEntityID: 0x%X     TargetEntityID: 0x%X     ",ses_ulMsgId,ulEntityID,ulTargetID);
				CPrintF("     Position: %f, %f, %f     ",plPlacementCopy.pl_PositionVector(1),plPlacementCopy.pl_PositionVector(2),plPlacementCopy.pl_PositionVector(3));
				CPrintF("Angle: %f, %f, %f\n",plPlacementCopy.pl_OrientationAngle(1),plPlacementCopy.pl_OrientationAngle(2),plPlacementCopy.pl_OrientationAngle(3));
			}
			
			break;
		}
		// entity needs to be destroyed
	case EMT_DESTROY: 
		{
			CEntity* penEntity;      
			
			ses_ulMsgId++;
			
			// get the entity id from themessage
			ses_emEntityMessage.ReadEntityDestroy(ulEntityID);
			
			// get the pointer to the entity
			if (!_pNetwork->ga_World.EntityExists(ulEntityID,penEntity)) {
				extern INDEX net_bReportMiscErrors;
				if (net_bReportMiscErrors) {
					CPrintF(TRANS("Warning: destroy message received for non existing entity, EntityID: 0x%X\n"),ulEntityID);
				}
				break;
			}
			
			ASSERT(penEntity != NULL);
			if (penEntity == NULL) break;
			// destroy the entity - this might not delete it from memory, depending on the number of references to it
			penEntity->Destroy();
			
			// report to console
			if (net_bReportClientTraffic) 
			{
				//0205
				//			CPrintF("%lu: DESTROY:        EntityID: 0x%X\n", ses_ulMsgId,ulEntityID);
			}
			
			break;
		}
		
		// an event has been received for an entity (i.e. Explode, Damage, Die...)
	case EMT_EVENT: 
		{
			SLONG slDataSize;
			SLONG slEventCode;				
			UWORD uwClassID;
			UWORD uwEventID;
			EntityClassEntry* peceEntry;
			CEntityEvent  *peeEvent;
			CEntity* penEntity;
			
			ses_ulMsgId++;
			// get the entity ID, event code and event data from the message
			ses_emEntityMessage.ReadEntityEvent(ulEntityID,slEventCode,(void*) aubData,slDataSize);
			// extract the entity class id and event id
			// an event is uniquely identified only if both of these ids are supplied
			uwEventID = slEventCode & 0x0000FFFF;
			uwClassID = (slEventCode & 0xFFFF0000) >> 16; 
			
			// get the pointer to the entity class entry in the class container
			peceEntry = FindClassInContainer(uwClassID);
			// it really should be there
			ASSERT(peceEntry != NULL);
			if (peceEntry==NULL) {
				extern INDEX net_bReportMiscErrors;
				if (net_bReportMiscErrors) {
					CPrintF(TRANS("Warning: Event received for an unknown entity class, EntityID: 0x%X\n"),ulEntityID);
				}
				break;
			}
			
			// report to console
			if (net_bReportClientTraffic) {
				//			CPrintF("%lu: EVENT:            EntityID: 0x%X     EventCode: 0x%X     DataSize: %d\n",ses_ulMsgId,ulEntityID,slEventCode,slDataSize);
			}
			
			CEntityClass *pecClass;
			
			// obtain the class from the stock
			try 
			{
				pecClass = _pEntityClassStock->Obtain_t(peceEntry->ece_fnmEntityClass);
			} 
			catch (char * strError) 
			{
				extern INDEX net_bReportMiscErrors;
				if (net_bReportMiscErrors) 
				{
					CPrintF(TRANS("Warning: Unable to obtain entity class from stock, class name: %s\n"),peceEntry->ece_fnmEntityClass);
					CPrintF("%s\n",strError);
				}
				break;
			}
			BOOL bFound = FALSE;
			
			// try to find the event in the event table for this class, and when found, create a new empty event
			for (int iEvent=0;iEvent<pecClass->ec_pdecDLLClass->dec_ctEvents;iEvent++) {
				if (slEventCode == pecClass->ec_pdecDLLClass->dec_adeeEvents[iEvent]->dee_slEvent) {
					peeEvent = pecClass->ec_pdecDLLClass->dec_adeeEvents[iEvent]->dee_New();
					bFound=TRUE;
					break;
				}
			}
			// if must be there
			ASSERT (bFound);
			
			if (!bFound) 
			{
				extern INDEX net_bReportMiscErrors;
				if (net_bReportMiscErrors) {
					CPrintF(TRANS("Warning: Unable to find requested entity event, class name: %s, event: %d\n"),peceEntry->ece_fnmEntityClass,uwEventID);
				}
				_pEntityClassStock->Release(pecClass);
				break;
			}
			
			// fill in the event data
			memcpy(((UBYTE*)peeEvent) + sizeof(CEntityEvent),aubData,slDataSize);
			
			// get the pointer to the entity
			if (!_pNetwork->ga_World.EntityExists(ulEntityID,penEntity)) {
				extern INDEX net_bReportMiscErrors;
				if (net_bReportMiscErrors) {
					CPrintF(TRANS("Warning: event received for non existing entity, EntityID: 0x%X\n"),ulEntityID);
				}
				_pEntityClassStock->Release(pecClass);
				delete (UBYTE*)peeEvent;
				break;
			}
			
			if (penEntity->IsFlagOn(ENF_DELETED)) 
			{
				extern INDEX net_bReportMiscErrors;
				if (net_bReportMiscErrors) {
					CPrintF(TRANS("Warning: event received for deleted entity, EntityID: 0x%X\n"),ulEntityID);
				}
				_pEntityClassStock->Release(pecClass);
				delete (UBYTE*)peeEvent;
				break;
			}
			
			// sent the event to the entity
			penEntity->SendEvent(*peeEvent);
			
			// handle the event
			CEntity::HandleSentEvents();
			
			
			
			// release the entity class from the stock (remove a reference)
			_pEntityClassStock->Release(pecClass);
			
			delete (UBYTE*)peeEvent;
			
			break;
			}
			// an entity should be initialized
		case EMT_INITIALIZE: 
			{
				SLONG slDataSize;
				SLONG slEventCode;
				EntityClassEntry* peceEntry;
				CEntityEvent  *peeEvent;
				CEntity* penEntity;
				UWORD uwClassID;
				UWORD uwEventID;
				
				ses_ulMsgId++;
				
				// get the entity ID, event code and event data from the message
				ses_emEntityMessage.ReadEntityInitialize(ulEntityID,slEventCode,(void*) aubData,slDataSize);
				// extract the entity class id and event id
				// an event is uniquely identified only if both of these ids are supplied
				uwEventID = slEventCode & 0x0000FFFF;
				uwClassID = (slEventCode & 0xFFFF0000) >> 16; 
				// get the pointer to the entity class entry in the class container
				peceEntry = FindClassInContainer(uwClassID);
				// it isn't neccessary to get a non-null pointer here - it will be null for an event
				// from 'BaseEvents.h'
				
				// report to console
				if (net_bReportClientTraffic) {
					CPrintF("%lu: INITIALIZE:            EntityID: 0x%X     EntityClass: %d      EventID: %d     DataSize: %d\n",ses_ulMsgId,ulEntityID,uwClassID,uwEventID,slDataSize);
				}
				
				// get the pointer to the entity
				if (!_pNetwork->ga_World.EntityExists(ulEntityID,penEntity)) {
					extern INDEX net_bReportMiscErrors;
					if (net_bReportMiscErrors) {
						CPrintF(TRANS("Warning: initialize received for non existing entity, EntityID: 0x%X\n"),ulEntityID);
					}
					break;
				}
				
				EVoid eVoidEvent;
				// if this is en EVoid, initialize with default
				if (slEventCode == eVoidEvent.ee_slEvent) {
					penEntity->Initialize();
				} 
				else 
				{
					// if it's not an EVoid, check if we got a valid class entry
					ASSERT(peceEntry != NULL);
					
					CEntityClass *pecClass;
					
					try 
					{
						pecClass = _pEntityClassStock->Obtain_t(peceEntry->ece_fnmEntityClass);
					} catch (char * strError){
						extern INDEX net_bReportMiscErrors;
						if (net_bReportMiscErrors) {
							CPrintF(TRANS("Warning: Unable to obtain entity class from stock, class name: %s\n"),peceEntry->ece_fnmEntityClass);
							CPrintF("%s\n",strError);
						}
						break;
					}
					
					BOOL bFound = FALSE;
					
					// find the event in this class, and create a new empty event
					for (int iEvent=0;iEvent<pecClass->ec_pdecDLLClass->dec_ctEvents;iEvent++) {
						if (slEventCode == pecClass->ec_pdecDLLClass->dec_adeeEvents[iEvent]->dee_slEvent) {
							peeEvent = pecClass->ec_pdecDLLClass->dec_adeeEvents[iEvent]->dee_New();
							bFound=TRUE;              
							break;
						}
					}
					// it must be there
					ASSERT (bFound);
					
					if (!bFound) 
					{
						extern INDEX net_bReportMiscErrors;
						if (net_bReportMiscErrors) {
							CPrintF(TRANS("Warning: Unable to find requested entity event, class name: %s, event: %d\n"),peceEntry->ece_fnmEntityClass,uwEventID);
						}
						_pEntityClassStock->Release(pecClass);
						break;
					}
					
					// fill in the data
					memcpy(((UBYTE*)peeEvent) + sizeof(CEntityEvent),aubData,slDataSize);
					
					// initialize the entity
					penEntity->Initialize(*peeEvent);
					
					// release the class (remove reference)
					_pEntityClassStock->Release(pecClass);
					
					delete (UBYTE*)peeEvent;
				}
				
				break;
			}
			// make an entity start playing an animation
		case EMT_STARTANIM: 
			{
				CEntity* penEntity;
				INDEX iNewModelAnim;
				ULONG ulFlags;
				
				ses_ulMsgId++;
				// get the entity id, animation index and flags
				ses_emEntityMessage.ReadStartAnim(ulEntityID,iNewModelAnim,ulFlags);
				
				// get the pointer to the entity
				if (!_pNetwork->ga_World.EntityExists(ulEntityID,penEntity)) {
					extern INDEX net_bReportMiscErrors;
					if (net_bReportMiscErrors) {
						//CPrintF(TRANS("Warning: Start anim (not ska) received for non existing entity, EntityID: 0x%X\n"),ulEntityID);
					}
					break;
				}
				
				// do not do this for local players
				if (penEntity->IsPlayer() && (_pNetwork->IsPlayerLocal(penEntity))) { 
					extern INDEX net_bReportMiscErrors;
					if (net_bReportMiscErrors) {
						//CPrintF(TRANS("Warning: Start anim (not ska) received for local player, EntityID: 0x%X\n"),ulEntityID);
					}
					break; 
				};
				
				// play the animation 
				penEntity->StartModelAnim(iNewModelAnim,ulFlags);
				
				// report to the console
				if (net_bReportClientTraffic) {
					CPrintF("STARTANIM:            EntityID: 0x%X     NewAnim: %ld     Flags: 0x%X\n",ulEntityID,iNewModelAnim,ulFlags);
				}
				
				break;
			}
			// update for player data
		case EMT_PLAYERUPDATE: 
			{
				CPlayerEntity* penPlayerEntity;        
				CNetworkMessage nmMessage;
				CEntity* penEntity;
				
				ses_ulMsgId++;
				// get the player entity id
				ses_emEntityMessage.ReadPlayerUpdate(ulEntityID,nmMessage);
				if (!_pNetwork->ga_World.EntityExists(ulEntityID,penEntity)) {
					extern INDEX net_bReportMiscErrors;
					if (net_bReportMiscErrors) {
						CPrintF(TRANS("Warning: update received for non existing player!\n"));
					}
					break;
				}
				ASSERT (penEntity->IsPlayer());
				if (!penEntity->IsPlayer()) {
					extern INDEX net_bReportMiscErrors;
					if (net_bReportMiscErrors) {
						CPrintF(TRANS("Warning: player update received with non-player entity id !\n"));
					}
					break;
				}
				
				// the pointer to the player entity
				penPlayerEntity = (CPlayerEntity*) penEntity;
				
				// report to the console
				if (net_bReportClientTraffic) {
					//0203
					//				CPrintF("PLAYERUPDATE:            EntityID: 0x%X \n",ulEntityID);
				}      
				
				// find the PlayerSource for this entity - will produce a non null pointer only for local players
				CPlayerSource* pplsSource = _pNetwork->GetPlayerSource(penPlayerEntity);
				// if it's a local player and the game isn't paused
				// if it's a local player, read it's data and reapply local prediction
				if (_pNetwork->IsPlayerLocal(penEntity) || _pNetwork->IsPlayingDemo()) {
					if (!_pNetwork->IsPlayingDemo()) {
						ASSERT(pplsSource != NULL);
						if (pplsSource == NULL) {
							extern INDEX net_bReportMiscErrors;
							if (net_bReportMiscErrors) {
								CPrintF(TRANS("Warning: player source for local player does not exist!\n"));
							}
							break;
						}
					}//0202*/
					//0130 5line 지우기
					/*				
					penPlayerEntity->Read_net(&nmMessage,ses_emEntityMessage.em_tmMessageTime);
					
					  if (!_pNetwork->IsPlayingDemo()) {
					  // apply all actions not executed by the server - local prediction
					  pplsSource->ApplyNewerActions(penPlayerEntity->m_iLastActionTime);          
					  }
					*/				
					// if it's no a local player, read data and apply dead reckoning
					//0202/*	
				}
				/*
				else 
				{		
				static bFirst=TRUE;
				if(bFirst){				
				penPlayerEntity->Read_net_brief(&nmMessage,ses_emEntityMessage.em_tmMessageTime);			
				bFirst=FALSE;
				CPrintF(TRANS("Read_net_brief!\n"));
				}
				
				  }
				*/
				
				
				break;
			}
			/*
			// read base placement
			case EMT_BASEPLACEMENT: 
			{
			ses_emEntityMessage.ReadBasePosition(_vBasePosition);
			_bBasePositionSet = TRUE;
			
			  // report to console
			  if (net_bReportClientTraffic) {
			  CPrintF("BASEPLACEMENT:           Position: %f, %f, %f\n",_vBasePosition(1),_vBasePosition(2),_vBasePosition(3));
			  }
			  
				break;
								}*/
			
			// ska animation event
		case EMT_SKA_ANIM: 
			{
				CEntity* penEntity;        
				UBYTE ubSkaCode;
				SLONG slDataSize = ESKA_MAX_DATA_SIZE;
				static UBYTE aubSkaData[ESKA_MAX_DATA_SIZE];
				UBYTE* pubData = aubSkaData;
				
				ses_ulMsgId++;
				// get the entity id
				ses_emEntityMessage.ReadSka(ulEntityID,ubSkaCode,aubSkaData,slDataSize);
				// the pointer to the entity
				if (!_pNetwork->ga_World.EntityExists(ulEntityID,penEntity))  { 
					extern INDEX net_bReportMiscErrors;
					if (net_bReportMiscErrors) {
						//CPrintF(TRANS("Warning: SKA event received for non existing entity, EntityID: 0x%X\n"),ulEntityID);            
					}
					break;
				}
				//penEntity = _pNetwork->ga_World.EntityFromID(ulEntityID);
				// if it fails in the release version, don't crash ;)
				if (penEntity == NULL) break;
				if (penEntity->en_RenderType != CEntity::RT_SKAMODEL && penEntity->en_RenderType != CEntity::RT_SKAEDITORMODEL) {
					extern INDEX net_bReportMiscErrors;
					if (net_bReportMiscErrors) {
						CPrintF(TRANS("Warning: SKA event received for non SKA entity, EntityID: 0x%X\n"),ulEntityID);
					}
					break;
				}
				
				switch (ubSkaCode) 
				{
				case EM_SKA_ADD: 
					{
						INDEX iAnimID,iLocalAnimID,iGroupID,iLocalGroupID,iModelInstanceID,iLocalModelInstanceID;
						ULONG ulFlags;
						FLOAT fStrength,fSpeedMul;
						extern CStaticStackArray<INDEX> _aiNetSkaIDConversion;
						
						//				ska_StringToID("Windwalker");
						
						iAnimID   = *(INDEX*)pubData; pubData += sizeof(iAnimID);
						ulFlags   = *(ULONG*)pubData; pubData += sizeof(ulFlags);
						fStrength = *(FLOAT*)pubData; pubData += sizeof(fStrength);
						iGroupID  =  *(INDEX*)pubData; pubData += sizeof(iGroupID);
						iModelInstanceID = *(INDEX*)pubData; pubData += sizeof(iModelInstanceID);
						fSpeedMul = *(FLOAT*)pubData;
						
						iLocalAnimID = _aiNetSkaIDConversion[iAnimID];
						iLocalGroupID = _aiNetSkaIDConversion[iGroupID];
						if (iModelInstanceID == ESKA_MASTER_MODEL_INSTANCE) {
							iLocalModelInstanceID = ESKA_MASTER_MODEL_INSTANCE;
						} else {
							iLocalModelInstanceID = _aiNetSkaIDConversion[iModelInstanceID];
						}
						
						penEntity->AddAnimation(iLocalAnimID,ulFlags,fStrength,iLocalGroupID,iLocalModelInstanceID,fSpeedMul);
						
						break;
					}
				case EM_SKA_REM: 
					{
						INDEX iAnimID,iLocalAnimID,iModelInstanceID,iLocalModelInstanceID;
						
						iAnimID   = *(INDEX*)pubData; pubData += sizeof(iAnimID);
						iModelInstanceID = *(INDEX*)pubData; pubData += sizeof(iModelInstanceID);
						iLocalAnimID = _aiNetSkaIDConversion[iAnimID];              
						if (iModelInstanceID == ESKA_MASTER_MODEL_INSTANCE) {
							iLocalModelInstanceID = ESKA_MASTER_MODEL_INSTANCE;
						} else {
							iLocalModelInstanceID = _aiNetSkaIDConversion[iModelInstanceID];
						}
						
						penEntity->RemAnimation(iLocalAnimID,iLocalModelInstanceID);
						
						break;
					}
				case EM_SKA_REMID: 
					{
						INDEX iGroupID,iModelInstanceID,iLocalModelInstanceID;
						
						iGroupID   = *(INDEX*)pubData; pubData += sizeof(iGroupID);
						iModelInstanceID = *(INDEX*)pubData; pubData += sizeof(iModelInstanceID);
						
						if (iModelInstanceID == ESKA_MASTER_MODEL_INSTANCE) {
							iLocalModelInstanceID = ESKA_MASTER_MODEL_INSTANCE;
						} else {
							iLocalModelInstanceID = _aiNetSkaIDConversion[iModelInstanceID];
						}
						
						penEntity->RemAnimsWithID(iGroupID,iLocalModelInstanceID);
						
						break;
					}
				case EM_SKA_NEWCLONED: 
					{
						INDEX iModelInstanceID,iLocalModelInstanceID;
						FLOAT fFadeTime;
						
						fFadeTime = *(FLOAT*)pubData; pubData += sizeof(fFadeTime);
						iModelInstanceID = *(INDEX*)pubData; pubData += sizeof(iModelInstanceID);
						if (iModelInstanceID == ESKA_MASTER_MODEL_INSTANCE) {
							iLocalModelInstanceID = ESKA_MASTER_MODEL_INSTANCE;
						} else {
							iLocalModelInstanceID = _aiNetSkaIDConversion[iModelInstanceID];
						}
						
						penEntity->NewClonedState(fFadeTime,iLocalModelInstanceID);  
						
						break;
					}
				case EM_SKA_NEWCLEAR: 
					{
						INDEX iModelInstanceID,iLocalModelInstanceID;
						FLOAT fFadeTime;
						
						fFadeTime = *(FLOAT*)pubData; pubData += sizeof(fFadeTime);
						iModelInstanceID = *(INDEX*)pubData; pubData += sizeof(iModelInstanceID);
						if (iModelInstanceID == ESKA_MASTER_MODEL_INSTANCE) {
							iLocalModelInstanceID = ESKA_MASTER_MODEL_INSTANCE;
						} else {
							iLocalModelInstanceID = _aiNetSkaIDConversion[iModelInstanceID];
						}
						
						penEntity->NewClearState(fFadeTime,iLocalModelInstanceID);  
						
						break;
					}
				case EM_SKA_STRETCH: 
					{
						INDEX iModelInstanceID,iLocalModelInstanceID;
						FLOAT3D vStretch;
						
						vStretch(1) = *(FLOAT*)pubData; pubData += sizeof(FLOAT);
						vStretch(2) = *(FLOAT*)pubData; pubData += sizeof(FLOAT);
						vStretch(3) = *(FLOAT*)pubData; pubData += sizeof(FLOAT);
						iModelInstanceID = *(INDEX*)pubData; pubData += sizeof(iModelInstanceID);
						if (iModelInstanceID == ESKA_MASTER_MODEL_INSTANCE) {
							iLocalModelInstanceID = ESKA_MASTER_MODEL_INSTANCE;
						} else {
							iLocalModelInstanceID = _aiNetSkaIDConversion[iModelInstanceID];
						}
						
						penEntity->StretchModel(vStretch,iLocalModelInstanceID);  
						
						break;
					}
				case EM_SKA_STRETCHSINGLE: 
					{
						INDEX iModelInstanceID,iLocalModelInstanceID;
						FLOAT3D vStretch;
						
						vStretch(1) = *(FLOAT*)pubData; pubData += sizeof(FLOAT);
						vStretch(2) = *(FLOAT*)pubData; pubData += sizeof(FLOAT);
						vStretch(3) = *(FLOAT*)pubData; pubData += sizeof(FLOAT);
						iModelInstanceID = *(INDEX*)pubData; pubData += sizeof(iModelInstanceID);
						if (iModelInstanceID == ESKA_MASTER_MODEL_INSTANCE) {
							iLocalModelInstanceID = ESKA_MASTER_MODEL_INSTANCE;
						} else {
							iLocalModelInstanceID = _aiNetSkaIDConversion[iModelInstanceID];
						}
						
						penEntity->StretchSingleModel(vStretch,iLocalModelInstanceID);    
						
						break;
					}
				default: 
					{
						ASSERT(FALSE);
					}					
				}
				break;
			}
			// pause/unpause game
		case EMT_PAUSE : 
			{
				CTString strPauser;
				BOOL bPauseBefore = ses_bPause;
				
				ses_ulMsgId++;
				// read the new pause state
				ses_emEntityMessage.ReadPause(ses_bPause,strPauser);
				
				// report to console
				if (net_bReportClientTraffic) {
					CPrintF("PAUSE.\n");
				}
				
				// report who paused
				if (ses_bPause!=bPauseBefore) {
					if (ses_bPause) {
						CPrintF(TRANS("Paused by '%s'\n"), strPauser);
					} else {
						CPrintF(TRANS("Unpaused by '%s'\n"), strPauser);
					}
				}
				
				// must keep wanting current state
				ses_bWantPause = ses_bPause;
				
				break;
			}///*
		case EMT_REMPLAYER: 
			{
				INDEX iPlayer;
				
				ses_ulMsgId++;
				// read the new pause state
				ses_emEntityMessage.ReadRemovePlayer(iPlayer);
				
				// report to console
				//if (net_bReportClientTraffic) {
				CPrintF("REMPLAYER:     %s   index: %ld\n",(const char*)_pNetwork->ga_srvServer.srv_apltPlayers[iPlayer].plt_pcCharacter.pc_strName,iPlayer);
				//}
				
				CPlayerTarget &plt = _pNetwork->ga_srvServer.srv_apltPlayers[iPlayer];
				if (plt.plt_penPlayerEntity == NULL) { 
					if (plt.IsActive()) {
						_pNetwork->ga_srvServer.srv_apltPlayers[iPlayer].Deactivate();
					}
					return SUCCESS_OK; 
				}
				
				// inform entity of disconnnection
				CPrintF(TRANS("%s left\n"), _pNetwork->ga_srvServer.srv_apltPlayers[iPlayer].plt_penPlayerEntity->GetPlayerName());
				_pNetwork->ga_srvServer.srv_apltPlayers[iPlayer].plt_penPlayerEntity->Disconnect();
				// deactivate the player
				_pNetwork->ga_srvServer.srv_apltPlayers[iPlayer].Deactivate();
				// handle all the sent events
				ses_bAllowRandom = TRUE;
				CEntity::HandleSentEvents();
				ses_bAllowRandom = FALSE;				
				
				break;
			}
			//0205	
			/*
			case EMT_PLAYER_MOVE:{
			
			  
				CPlayerTarget &plt = _pNetwork->ga_srvServer.srv_apltPlayers[iPlayer];	
				
				  plt->plt_penPlayerEntity->ClearMovingTemp();
				  plt->plt_penPlayerEntity->PreMoving();
				  plt->plt_penPlayerEntity->DoMoving();
				  plt->plt_penPlayerEntity->PostMoving();
				  plt->plt_penPlayerEntity->ClearMovingTemp();
				  CEntity::HandleSentEvents();
				  
					plt->ApplyActionPacket(paAction);
					plt->plt_penPlayerEntity->SetActionApplied();
					
					  break;
					  }
			*/
			////////////////////////////////						
			
		case EMT_CHANGECHARACTER: 
			{
				CPlayerCharacter pcCharacter;
				CEntity* penEntity;
				
				ses_ulMsgId++;
				
				// extract neccessary data from the message
				ses_emEntityMessage.ReadcharacterChange(ulEntityID,pcCharacter);    
				
				// get the pointer to the entity
				if (!_pNetwork->ga_World.EntityExists(ulEntityID,penEntity)) {
					extern INDEX net_bReportMiscErrors;
					if (net_bReportMiscErrors) {
						CPrintF(TRANS("Warning: Change character received for non existing entity, EntityID: 0x%X\n"),ulEntityID);
					}
					break;
				}
				CPlayerEntity* penPlayerEntity = (CPlayerEntity*) penEntity;
				
				// change the character
				penPlayerEntity->CharacterChanged(pcCharacter);
				
				// report to console
				if (net_bReportClientTraffic) {
					CPrintF("%lu: CHARACTERCHANGE:     EntityID: 0x%X\n",ulEntityID);
				}
				
				break;
			}
			
			// sound animation event
		case EMT_SOUND: 
			{
				CEntity* penEntity;        
				UBYTE ubSoundCode;
				SLONG slDataSize = ESOUND_MAX_DATA_SIZE;
				static UBYTE aubSoundData[ESOUND_MAX_DATA_SIZE];
				UBYTE* pubData = aubSoundData;
				ULONG ulComponentOffset;
				CSoundObject *psoSoundObject;
				
				ses_ulMsgId++;
				// get the entity id
				ses_emEntityMessage.ReadSound(ulEntityID,ubSoundCode,aubSoundData,slDataSize);
				// the pointer to the entity
				if (!_pNetwork->ga_World.EntityExists(ulEntityID,penEntity))  { 
					extern INDEX net_bReportMiscErrors;
					if (net_bReportMiscErrors) {
						//CPrintF(TRANS("Warning: Sound event received for non existing entity, EntityID: 0x%X\n"),ulEntityID);            
					}
					break;
				}
				
				
				
				switch( ubSoundCode)
				{
				case EM_SOUND_PLAY: 
					{
						SLONG idSoundComponent;
						SLONG slPlayType;
						UBYTE ubThis;
						ULONG ulOwnerID;
						CEntity* penSoOwner = penEntity;
						
						ubThis = *(UBYTE*)pubData; pubData += sizeof(ubThis);
						if (!ubThis) {
							ulOwnerID = *(ULONG*)pubData; pubData += sizeof(ulOwnerID);
							if (!_pNetwork->ga_World.EntityExists(ulOwnerID,penSoOwner))  { 
								extern INDEX net_bReportMiscErrors;
								if (net_bReportMiscErrors) {
									CPrintF(TRANS("Warning: Sound event received for non existing entity, OwnerID: 0x%X\n"),ulOwnerID);            
								}
								break;
							}
						}
						if (penEntity->IsPlayer() && _pNetwork->IsPlayerLocal(penEntity)) {
							break;
						}
						ulComponentOffset = *(ULONG*)pubData;  pubData += sizeof(ulComponentOffset);
						idSoundComponent = *(SLONG*)pubData;   pubData += sizeof(idSoundComponent);
						slPlayType = *(SLONG*)pubData; pubData += sizeof(slPlayType);
						psoSoundObject = (CSoundObject*)(((UBYTE*)penSoOwner) + ulComponentOffset);
						penEntity->PlaySound(*psoSoundObject,idSoundComponent,slPlayType); 
						// report to console
						if( net_bReportClientTraffic) {
							CPrintF("%lu: SOUND_PLAY:     EntityID: 0x%X\n",ulEntityID);
						}
						break;
					}
				case EM_SOUND_STOP: 
					{
						ulComponentOffset = *(ULONG*)pubData;  pubData += sizeof(ulComponentOffset);
						psoSoundObject = (CSoundObject*)(((UBYTE*)penEntity) + ulComponentOffset);
						psoSoundObject->Stop();
						// report to console
						if( net_bReportClientTraffic) {
							CPrintF("%lu: EM_SOUND_Stop:     EntityID: 0x%X\n",ulEntityID);
						}
						break;
					}
				case EM_SOUND_SETVOLUME: 
					{
						ulComponentOffset = *(ULONG*)pubData;    pubData += sizeof(ulComponentOffset);
						FLOAT fVolume = ((FLOAT)(*(UBYTE*)pubData))/127.0f;  pubData += sizeof(UBYTE);
						psoSoundObject = (CSoundObject*)(((UBYTE*)penEntity) + ulComponentOffset);
						psoSoundObject->SetVolume(fVolume);
						// report to console
						if( net_bReportClientTraffic) {
							CPrintF("%lu: EM_SOUND_SETVOLUME:     EntityID: 0x%X\n",ulEntityID);
						}
						break;
					}
				case EM_SOUND_SETPITCH: 
					{
						ulComponentOffset = *(ULONG*)pubData;  pubData += sizeof(ulComponentOffset);
						FLOAT fPitch = ((FLOAT)(*(UBYTE*)pubData))/63.0f;  pubData += sizeof(UBYTE);
						psoSoundObject = (CSoundObject*)(((UBYTE*)penEntity) + ulComponentOffset);
						psoSoundObject->SetPitch(fPitch);
						// report to console
						if( net_bReportClientTraffic) {
							CPrintF("%lu: EM_SOUND_SETPITCH:     EntityID: 0x%X\n",ulEntityID);
						}
						break;
					}
				case EM_SOUND_SETRANGE: 
					{
						ulComponentOffset = *(ULONG*)pubData;   pubData += sizeof(ulComponentOffset);
						FLOAT fHotSpot = ((FLOAT)(*(UWORD*)pubData))/5.0f;  pubData += sizeof(UWORD);
						FLOAT fFallOff = ((FLOAT)(*(UWORD*)pubData))/5.0f;  pubData += sizeof(UWORD);
						psoSoundObject = (CSoundObject*)(((UBYTE*)penEntity) + ulComponentOffset);
						psoSoundObject->SetRange( fHotSpot, fFallOff);
						// report to console
						if( net_bReportClientTraffic) {
							CPrintF("%lu: EM_SOUND_SETRANGE:     EntityID: 0x%X\n",ulEntityID);
						}
						break;
					}
				case EM_SOUND_FADE: 
					{
						ulComponentOffset = *(ULONG*)pubData;    pubData += sizeof(ulComponentOffset);
						FLOAT fVolume = ((FLOAT)(*(UBYTE*)pubData))/127.0f;  pubData += sizeof(UBYTE);
						TIME tmPeriod = *((TIME*)pubData);  pubData += sizeof(TIME);
						psoSoundObject = (CSoundObject*)(((UBYTE*)penEntity) + ulComponentOffset);
						psoSoundObject->Fade(fVolume,tmPeriod);
						// report to console
						if( net_bReportClientTraffic) {
							CPrintF("%lu: EM_SOUND_FADE:     EntityID: 0x%X, period: %7.4f, volume: %7.4f\n",ses_ulMsgId,ulEntityID,tmPeriod,fVolume);
						}
						break;
					}
					
				default: 
					{
						ASSERT(FALSE);
					}
				}
				
				break;
			}
			// add a new string to ska string table & net coversion table
		case EMT_ADDSKASTRING : 
			{
				CTString strString;
				INDEX iServerIndex;
				
				ses_ulMsgId++;
				// read server ska ID and ska string to be added
				ses_emEntityMessage.ReadAddSkaString(iServerIndex,strString);
				
				// add it to the local table
				INDEX iLocalIndex = ska_GetIDFromStringTable(strString);
				
				ASSERT(_aiNetSkaIDConversion.Count() == iServerIndex);
				
				// report to console
				if (net_bReportClientTraffic) {
					CPrintF("EMT_ADDSKASTRING:  String  '%s' added to ska table, Local ID is:  %ld, Server ID is: %ld\n",(const char*)strString,iLocalIndex,iServerIndex);
				}
				
				// expand the conversion array, and make a new entry
				INDEX &iNewIndex = _aiNetSkaIDConversion.Push();
				iNewIndex = iLocalIndex;
				
				break;
			}
		case EMT_SETPLACEMENT_NOTIFY: 
			{
				extern INDEX net_bReportMiscErrors;
				if (net_bReportMiscErrors) {
					CPrintF(TRANS("Warning: server received EMT_SETPLACEMENT_NOTIFY\n"));
				}
				break;
			}
			// update for player data
		case EMT_PLAYERACTION: 
			{
				CPlayerEntity* penPlayerEntity;        
				CPlayerAction paAction;
				CEntity* penEntity;
				
				ASSERT(_pNetwork->IsPlayingDemo());
				
				ses_ulMsgId++;
				// get the player entity id
				ses_emEntityMessage.ReadPlayerAction(ulEntityID,paAction);
				if (!_pNetwork->ga_World.EntityExists(ulEntityID,penEntity)) {
					extern INDEX net_bReportMiscErrors;
					if (net_bReportMiscErrors) {
						CPrintF(TRANS("Warning: player action for non existing player!\n"));
					}
					break;
				}
				ASSERT (penEntity->IsPlayer());
				if (!penEntity->IsPlayer()) 
				{
					extern INDEX net_bReportMiscErrors;
					if (net_bReportMiscErrors) {
						CPrintF(TRANS("Warning: player action received with non-player entity id !\n"));
					}
					break;
				}
				
				// the pointer to the player entity
				penPlayerEntity = (CPlayerEntity*) penEntity;
				
				// report to the console
				if (net_bReportClientTraffic) {
					CPrintF("PLAYERACTION:            EntityID: 0x%X \n",ulEntityID);
				}      
				
				// apply this action
				penPlayerEntity->ApplyAction(paAction,0);
				
				// run all player updates
				penPlayerEntity->ClearMovingTemp();
				penPlayerEntity->PreMoving();
				penPlayerEntity->DoMoving(); 
				penPlayerEntity->PostMoving();
				penPlayerEntity->ClearMovingTemp();
				
				if (penPlayerEntity->en_penParent!=NULL && penPlayerEntity->en_penParent->GetPhysicsFlags()&EPF_MOVABLE) 
				{
					penPlayerEntity->en_plLastPlacement.pl_PositionVector =     penPlayerEntity->en_plPlacement.pl_PositionVector - 
						((CMovableEntity*)(penPlayerEntity->en_penParent))->en_vCurrentTranslationAbsolute*_pTimer->TickQuantum;
				}
				
				break;				
			}
			//    default: {
			//        ASSERT(FALSE);
			//        break;
			//			 }
	}	
	return TRUE;	
};


// receive new tick data from the server
void CSessionState::ReceiveTick(TIME tmTickTime,UBYTE* pubData,SLONG slSize) 
{
	
	ASSERT (pubData !=NULL);
	ASSERT (slSize >= 0);
	float fRandom;
	
//	extern INDEX net_bReportClientTraffic;
	
	// emulate bad network conditions
	fRandom = rand() / (float) RAND_MAX;  
	// if tick data is newer than the last data received, and it shouldn't be dropped
	if (ses_tmLastTickTime < tmTickTime && fRandom >= cli_fTickDropRate) {
		// report to console
		if (net_bReportClientTraffic) {
			//0203      CPrintF(TRANS("Client reading tick, time: %5.2f\n"),tmTickTime);
		}
		// put the data into the input buffer
		ses_embReceiveBuffer.WriteTick(tmTickTime,pubData,slSize);       
		// send tick acknowledge to server
		if (!_pNetwork->IsPlayingDemo()) {
			SendAcknowledge(tmTickTime);
		}
		// if tick data is older than last received, just send the acknowledge
	} else if (fRandom >= cli_fTickDropRate) { 
		if (!_pNetwork->IsPlayingDemo()) {
			SendAcknowledge(tmTickTime);
		}
	}
	
};


// acknowledge that tick data for a certian tick time has been received
void CSessionState::SendAcknowledge(float fTickTime)
{
//	extern INDEX net_bReportClientTraffic;

	// report to console
	if (net_bReportClientTraffic) {
//0203    CPrintF(TRANS("Send acknowledge: %5.2f\n"), fTickTime);
	}
};

//-----------------------------------------------------------------------------
// Purpose:
// Input  : *istr - 
//-----------------------------------------------------------------------------
void CSessionState::ReceiveChatMessage(CNetworkMessage *istr)
{
	CTString str;
	CTString strSendName;
	CTString strRecvName;
	BYTE	chChannel = 0; // 채널 넘버 (MSG_CHAT_CHANNEL_LEVEL, MSG_CHAT_CHANNEL_TRADE 에서만 처리)
	unsigned char chattype;
	SLONG 	 sendindex;
	SLONG	 slGroup = -1;
				
	(*istr) >> chattype; //0528 kwon
	(*istr) >> sendindex;
	(*istr) >> strSendName;

	if (chattype == MSG_CHAT_CHANNEL_LEVEL || chattype == MSG_CHAT_CHANNEL_TRADE)
	{
		(*istr) >> chChannel; // BYTE (보낸 채널 정보)
	}
	else
	{
		(*istr) >> strRecvName; // string (받는 사람 이름?)
	}

	(*istr) >> str;

	// [sora] 원정대 그룹 채팅
	if (chattype == MSG_CHAT_EXPEDITION)
	{
		(*istr) >> slGroup; // 원정대 그룹 정보
	}

	// 랭커 입장 메세지 표시.
	if( chattype == MSG_CHAT_RANKER_CONN )
	{
		SBYTE sbJob = atoi(str);
		CTString	strMessage;
		strMessage.PrintF(_S(2358,"%s클래스 랭커 %s님이 입장하였습니다."), CJobInfo::getSingleton()->GetName(sbJob, 0), strSendName );

		_UIAutoHelp->SetGMNotice(strMessage, 0x38FFD7FF);
		return;
	}

	CUIManager* pUIManager = CUIManager::getSingleton();
	
	// Date : 2005-05-25(오전 11:41:19), By Lee Ki-hwan
	if( chattype == MSG_CHAT_MESSENGER )
	{
		/***
		if( !pUIManager->GetMessenger()->IsUseLevel() ) return;
		pUIManager->GetMessenger()->RevTalk( sendindex, strSendName, strRecvName, str );
		return;
		***/
	}

	pUIManager->GetChattingUI()->AddChatMessage( chattype, sendindex, strSendName, str , chChannel, slGroup);
}

//-----------------------------------------------------------------------------
// Purpose:
// Input  : *istr - 
//-----------------------------------------------------------------------------
void CSessionState::ReceiveLoginMessage(CNetworkMessage *istr)
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if((*istr).GetType() == MSG_LOGINSERV_PLAYER)
	{
		_pSound->Mute();
		ReceiveServerListMessage(&(*istr));	
		//_pNetwork->m_bSendMessage = FALSE;
			
#ifdef RESTRICT_SOUND
			extern BOOL _bNTKernel;
			//if( _bNTKernel )
			_pSound->UpdateSounds();
#endif		
	}
	else if((*istr).GetType() == MSG_EXTEND)
	{
		LONG	ubType;
		(*istr) >> ubType;
	
		//if( ubType == MSG_EX_SECURE_CARD)
		{
			UBYTE cSecurityCode[4];

			(*istr) >> cSecurityCode[0];
			(*istr)	>> cSecurityCode[1];
			(*istr)	>> cSecurityCode[2];
			(*istr)	>> cSecurityCode[3];

			//pUIManager->GetLogin()->SecurityCardSet(cSecurityCode);
		}

		_pNetwork->m_bSendMessage = FALSE;
	}	
	else
	{
		unsigned char dbtype;
		(*istr) >> dbtype;

		// FIXME : bFail로 매번 체크하는게 맘에 들지 않음...
		static BOOL bFail	= FALSE;

		if((*istr).GetType() == MSG_FAIL)
		{	
			//wooss 050824
			// ----------->
			
			if(dbtype == MSG_FAIL_NOTLEVEL_FORDELETE )
			{	
				_pGameState->DisplayErrorMessage( dbtype, UI_NONE, MSGCMD_NULL);
				// WSS_TIMEOUT_BUGFIX 070607 ------->><<
				_pNetwork->m_bSendMessage = FALSE;
				return;
			} 
			// <----------
			else if(dbtype == MSG_FAIL_WRONG_IDENTIFICATION)			// 주민번호 틀림
			{
				pUIManager->CloseMessageBox( MSGCMD_NULL );
				CUIMsgBox_Info	MsgBoxInfo;
				MsgBoxInfo.SetMsgBoxInfo( _S( 42, "오류 발생" ), UMBS_OK,
											UI_NONE, MSGCMD_NULL );
				MsgBoxInfo.AddString( _S(2359,"주민등록 번호가 일치하지 않아 캐릭터를 삭제할 수 없습니다.") );

				pUIManager->CreateMessageBox( MsgBoxInfo);
				pUIManager->GetCharacterSelect()->Lock(FALSE);
				// WSS_TIMEOUT_BUGFIX 070607 ------->><<
				_pNetwork->m_bSendMessage = FALSE;				
				return;
			}
			else if( dbtype == MSG_FAIL_SCARD_NOT_MATCHING )	//보안카드 인증 번호 틀림
			{
				pUIManager->CloseMessageBox( MSGCMD_NULL );
				CUIMsgBox_Info	MsgBoxInfo;
				MsgBoxInfo.SetMsgBoxInfo( _S( 3667, "보안 카드" ), UMBS_OK,
											UI_NONE, MSGCMD_NULL );
				MsgBoxInfo.AddString( _S(3669,"보안 카드 인증 실패") );

				pUIManager->CreateMessageBox( MsgBoxInfo);
				pUIManager->GetLogin()->Lock(FALSE);
				_pNetwork->m_bSendMessage = FALSE;
				
				//pUIManager->GetLogin()->SecurityCardSet(NULL);
				return;
			}
			else if ( dbtype == MSG_FAIL_LOGINSERV_BLOCK_USER )
			{	// 패스워드 3번 틀림
				pUIManager->CloseMessageBox( MSGCMD_NULL );
				CUIMsgBox_Info	MsgBoxInfo;
				MsgBoxInfo.SetMsgBoxInfo( _S( 42, "오류 발생" ), UMBS_YESNO, UI_NONE, MSGCMD_LOGINSERV_BLOCK_USER );
				MsgBoxInfo.AddString( _S( 4116, "비밀번호 3회 오류로 30분간 접속할 수 없습니다. 해제를 원하시면 웹사이트 로그인을 통해 ‘나의 계정’ 메뉴에서 게임블록 해제 버튼을 눌러주세요.") );
				pUIManager->CreateMessageBox( MsgBoxInfo );
				pUIManager->GetLogin()->Lock(FALSE);
				_pNetwork->m_bSendMessage = FALSE;
				return;
			}
			else if ( dbtype == MSG_FAIL_CANNOT_CONNECT_SHUTDOWN_TIME )
			{
				pUIManager->CloseMessageBox( MSGCMD_NULL );
				CUIMsgBox_Info	MsgBoxInfo;
				CTString strMessage;
				//strMessage.PrintF( _s( "%s님은 셧다운제 적용 대상 연력으로 확인되어 게임 접속이 불가능 합니다.(00:00~06:00) 문의사항은 고객지원센터로 연락 주십시오. 확인을 누르면 게임이 종료 됩니다." ), pUIManager->GetLogin()->GetUserId() );
				
				MsgBoxInfo.SetMsgBoxInfo( _S( 42, "오류 발생" ), UMBS_YESNO, UI_NONE, MSGCMD_SHUTDOWN_TIME );
				MsgBoxInfo.AddString( strMessage );
				pUIManager->CreateMessageBox( MsgBoxInfo );
				return;				
			}
			else if( dbtype == MSG_FAIL_DB_CANNOT_DEL_CHAR )	// [2012/07/05 : Sora]  캐릭터 슬롯 확장
			{
				pUIManager->CloseMessageBox( MSGCMD_NULL );
				CUIMsgBox_Info	MsgBoxInfo;
				MsgBoxInfo.SetMsgBoxInfo( _S( 42, "오류 발생" ), UMBS_YESNO, UI_NONE, MSGCMD_NULL );
				MsgBoxInfo.AddString( _S( 5704, "추가 캐릭터 생성 슬롯 기간 만료로 캐릭터를 삭제할 수 없습니다." ) );
				pUIManager->CreateMessageBox( MsgBoxInfo );
				pUIManager->GetCharacterSelect()->Lock(FALSE);
				return;		
			}
			_pGameState->DisplayErrorMessage( dbtype, UI_NONE, MSGCMD_LOGIN_ERROR);

			// NOTE : 캐릭터 생성 실패시에는 다음 화면으로 넘어가면 안되기 때문에 플래그 체크함.
			// NOTE : 캐릭터 생성 실패시에 먼저 MSG_FAIL이 온후 MSG_DB_CHAR_EXIST가 온다...
			if(STAGEMGR()->GetCurStage() == eSTAGE_CREATECHAR)
			{
				bFail = TRUE;
			}
			_pNetwork->m_bSendMessage = FALSE;
			return;
		}
		else if ( (*istr).GetType() == MSG_DB )
		{
				_pSound->Mute();
			StageMgr* pStageMgr = StageMgr::getSingleton();

			static BOOL bSlotInfo = FALSE;
			if(dbtype == MSG_DB_SUCCESS)
			{
				if(!bSlotInfo)
				{
					_pGameState->ClearCharacterSlot();
					bSlotInfo = TRUE;
				}
				bFail = FALSE;
			}
			else if(dbtype == MSG_DB_CHAR_EXIST)
			{
				if(!bSlotInfo)
				{
					_pGameState->ClearCharacterSlot();
					bSlotInfo = TRUE;
				}

				_pGameState->ReceiveCharSlot((*istr));		// 캐릭터 생성 후
			}
			// check creatable night shadow. [11/13/2009 rumist]
			else if( dbtype == MSG_DB_NSCREATE )
			{
				_pGameState->SetCreatableNightShadow( TRUE );	
			}
			else if ( dbtype == MSG_DB_CHAR_SLOT_TIME )	// [2012/07/05 : Sora]  캐릭터 슬롯 확장
			{	
				ULONG charSlotTime;
				(*istr) >> charSlotTime;
				
				if (GameDataManager* pGameData = GameDataManager::getSingleton())
				{
					if (CCharacterSelect* pCharSel = pGameData->GetCharSelData())
						pCharSel->SetExpansionTime(charSlotTime);

				}
			}
			else if(dbtype == MSG_DB_CHAR_END)
			{
				bSlotInfo = FALSE;

				//wooss 050820
				ULONG slotTime;
				(*istr) >> slotTime;	// 현재 쓰지 않는 정보임. 
				(*istr) >> slotTime;    // ..

				_pNetwork->m_bSendMessage = FALSE;


				pUIManager->GetCharacterSelect()->Lock(FALSE);

				if (_pNetwork->bMoveCharacterSelectUI == TRUE)
				{
					pStageMgr->SetNextStage(eSTAGE_GAMEEND);
					return;
				}
				// EDIT : BS : 070413 : 신규 패킷 암호화
				
#ifdef CRYPT_NET_MSG
#ifndef CRYPT_NET_MSG_MANUAL
				ULONG ulSeed;
				(*istr) >> ulSeed;
				CNM_MakeKeyFromSeed(&_pNetwork->cnmKeyServer, (unsigned int)ulSeed);
#endif // #ifndef CRYPT_NET_MSG_MANUAL
#endif // ifdef CRYPT_NET_MSG
//#define AUTO_LOGIN
				if(pStageMgr->GetCurStage() == eSTAGE_CREATECHAR)
				{
					if(bFail)
					{
						bFail = FALSE;
						
#ifdef RESTRICT_SOUND
						extern BOOL _bNTKernel;
						//if( _bNTKernel )
							_pSound->UpdateSounds();
#endif	// RESTRICT_SOUND
						return;
					}
				}
				if(pStageMgr->GetCurStage() == eSTAGE_SELCHAR)
				{
					pUIManager->GetCharacterSelect()->UpdateCharacterList();
					pUIManager->GetCharacterSelect()->Lock(FALSE);
				}
				else
				{
					pStageMgr->EndCharData();
				}
			}
			else if(dbtype == MSG_DB_OK)
			{				
				bFail = FALSE;
				_pNetwork->m_bSendMessage = FALSE;
				// Game Start
				ULONG zone;
				(*istr) >> zone;
				g_slZone = zone;//0616 kwon	
				
				extern ENGINE_API BOOL _bLoginProcess;
				_bLoginProcess = FALSE;
				_SetPlayStartTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();

				pStageMgr->SetNextStage(eSTAGE_ZONELOADING);

				// 로그인 모델 삭제.
				pUIManager->GetCharacterSelect()->DeleteModel();
			}

			// EDIT : BS
			else if (dbtype == MSG_DB_OTHER_SERVER)
			{
				ULONG zone;
				CTString ip;
				ULONG port;

				(*istr) >> zone;
				(*istr) >> ip;
				(*istr) >> port;

				MoveOtherServer(zone, ip, port);
			}
			// --- EDIT : BS
			
#ifdef RESTRICT_SOUND
			extern BOOL _bNTKernel;
			//if( _bNTKernel )
				_pSound->UpdateSounds();
#endif // RESTRICT_SOUND
		}
		else if((*istr).GetType() == MSG_MENU)
		{
			if (dbtype == MSG_RETURN_TO_SELECT_CHANNEL)
			{
				UIMGR()->GetCharacterSelect()->ChangeServer();
			}
		}
		else
		{
			CPrintF("Undefined data received!! msgType=%u \n" , (*istr).GetType());
#ifdef RESTRICT_SOUND
			extern BOOL _bNTKernel;
			//if( _bNTKernel )
				_pSound->UpdateSounds();
#endif // RESTRICT_SOUND
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: 서버 목록을 가져옵니다.
// Input  : *istr - 
//-----------------------------------------------------------------------------
void CSessionState::ReceiveServerListMessage(CNetworkMessage *istr)
{
	LONG lRecentGroup;
	LONG lRecentServer;
	LONG lSubServerCount;
	LONG lGroupCount;
	LONG lSeqGroupNumber;
	LONG lGroupNumber;
	UBYTE ubServerType = 0;
	
	(*istr) >> lRecentGroup;		// 최근 접속한 서버 군
	(*istr) >> lRecentServer;		// 최근 접속한 서버 번호
	(*istr) >> lGroupCount;			// 서버 목록의 갯수.
	(*istr) >> lSeqGroupNumber;		// 서버 목록의 갯수.
	(*istr) >> lGroupNumber;		// 서버 목록의 갯수.
	(*istr) >> lSubServerCount;		// 서버 목록의 갯수.

	// --- wooss-060426---추천서버이벤트-------------------->>
	(*istr) >> ubServerType;

	CUIManager* pUIManager = CUIManager::getSingleton();

	// -----------------------------------------------------<<
	// 꽃놀이 이벤트 서버 리스트 초기화
	pUIManager->GetFlowerTree()->ClearList();

	GameDataManager* pGameDataManager = GameDataManager::getSingleton();
	CServerSelect* pServerData = NULL;

	if (pGameDataManager != NULL)
	{
		pServerData = pGameDataManager->GetServerData();
	}

	for(int i = 0; i < lSubServerCount; ++i)
	{
		LONG		lSubNum;		// Sub 번호
		LONG		lPlayerNum;		// 플레이어 수
		CTString	strAddress;		// IP Address
		LONG		lPortNum;		
		
		(*istr) >> lSubNum;		// Sub 번호 
		(*istr) >> lPlayerNum;	// 플레이어 수
		(*istr) >> strAddress;	// IP Address
		(*istr) >> lPortNum;		// Port Num
		
		lPlayerNum -= ADDSERVERVALUE;
		
		if (pServerData != NULL)
		{
			pServerData->AddServerList(lGroupNumber, lSubNum, lPlayerNum, strAddress, lPortNum, ubServerType);
			pServerData->SetRecentServer(lRecentGroup, lRecentServer);
		}
	}
	
	SetRecentServer(lRecentGroup, lRecentServer);
	
	if(lSeqGroupNumber == lGroupCount)
	{
		STAGEMGR()->SetNextStage(eSTAGE_SELSERVER);
		_pNetwork->m_bSendMessage = FALSE;
		_cmiComm.Disconnect();
	}
}

//안태훈 수정 끝	//(5th Closed beta)(0.2)
//-----------------------------------------------------------------------------
// Purpose: 
// Input  : *istr - 
//-----------------------------------------------------------------------------
void CSessionState::ReceiveNpcRegenMessage(CNetworkMessage *istr)
{
	INDEX iIndex;
	SBYTE sbYLayer;//1013
	INDEX iMobType;
	INDEX iEntityID;
	CPlacement3D plPlacement = CPlacement3D(FLOAT3D(0.0f, 0.0f, 0.0f), ANGLE3D(0.0f, 0.0f, 0.0f));
	CEntity* penEntity = NULL;

	(*istr) >> iIndex;								// Index
	(*istr) >> iMobType;							// DB Index
	(*istr) >> plPlacement.pl_PositionVector(1);	// Pos X
	(*istr) >> plPlacement.pl_PositionVector(3);	// Pos Z
	(*istr) >> plPlacement.pl_PositionVector(2);	// Pos H
	(*istr) >> plPlacement.pl_OrientationAngle(1);	// Angle
	(*istr) >> sbYLayer;							// Y Layer
	(*istr) >> iEntityID;							// Entity ID

//안태훈 수정 시작	//(5th Closed beta)(0.2)
	CMobData* MD = CMobData::getData(iMobType);
	if(MD->GetMobIndex() == -1)
	{
		OutputDebugString("Cannot find Mob Data!!!\n");
		ASSERTALWAYS("Cannot find Mob Data!!!\n");
		return;
	}

	CMobTarget* pMT = new CMobTarget;	
		
	penEntity = _pNetwork->ga_World.EntityFromID(iEntityID);
	penEntity->SetNetworkID(iIndex);

	BOOL bNpc = FALSE;
	if(penEntity->IsFirstExtraFlagOn(ENF_EX1_NPC))
	{
		bNpc = TRUE;
	}

	const char *szMobName = CMobData::getData(iMobType)->GetName();
	CTString strMobName = szMobName;
	pMT->SetData( iIndex, iMobType, strMobName, MD->GetLevel(), penEntity, bNpc, sbYLayer );//0826
	pMT->SetSyndicateData(MD->GetSyndicateType(), MD->GetSyndicateGrade());

	if(penEntity != NULL)
	{
		// FIXME : 에너미 Spawner과 중복되는 부분.
		penEntity->SetSkaModel( MD->GetMobSmcFileName() );
		CMobData::SetMobDataToNPC( penEntity, MD, szMobName );
		// 싱글모드일때 생성된 엔티티의 갯수를 저장함.
		if(_pNetwork->m_bSingleMode && 
			!penEntity->IsFirstExtraFlagOn( ENF_EX1_NPC ))
		{
			_pNetwork->wo_dwEnemyCount++;
			if(_pNetwork->m_ubGMLevel > 1)
			{
				CTString strMessage;
				strMessage.PrintF("=====spawned enemy : %d=====\n", _pNetwork->wo_dwEnemyCount);
				_pNetwork->ClientSystemMessage(strMessage);
			}
		}

		// 생성될때 애니메이션이 필요한 에너미들...
		const int iWanderingZombie	= 35;		// 방황하는 좀비
		const int iZombie			= 51;		// 좀비
		const int iDamd				= 142;		// 뎀드(스펠링 맞나???)

		// FIXME : 임시로 하드 코딩한 부분.
		// FIXME : 맘에 들지 않음.
		if( iMobType == iWanderingZombie || iMobType == iZombie || iMobType == iDamd )		// 좀비 & 뎀드
		{
			CEntityProperty &epPropertyStart = *(penEntity->PropertyForTypeAndID(CEntityProperty::EPT_INDEX, 99));	// Walk Animation
			ENTITYPROPERTY( &*penEntity, epPropertyStart.ep_slOffset, INDEX)		= ska_GetIDFromStringTable("m_zm_up01");
		}
		pMT->m_nIdxClient	= penEntity->en_ulID;
		penEntity->Initialize();
		penEntity->en_pMobTarget = pMT;
		
		((CPlayerEntity*)CEntity::GetPlayerEntity(0))->SetMobData(penEntity, MD->GetHealth(), MD->GetHealth(), MD->GetLevel(),FALSE,-1);
		penEntity->FallDownToFloor();
		CPrintF("---Mob Created! S Index : %d, C Index : %d\n", iIndex, pMT->m_nIdxClient);
	}

	ACTORMGR()->AddObject(pMT);

	// NOTE : 11번 던젼의 경우에만...
	if( g_slZone == 11 )
	{
		// NOTE : 밴더스내치 일경우에는 인터페이스를 띄우지 않음.
		if( iMobType != 220 )
			CUIManager::getSingleton()->GetSingleBattle()->OpenSingleBattle();
	}
}

//-----------------------------------------------------------------------------
// Purpose:
// Input  : *istr - 
//-----------------------------------------------------------------------------

void CSessionState::ReceiveMoveMessage(CNetworkMessage *istr)
{
	CEntity* penPlEntity;
	CPlayerEntity* penPlayerEntity;
	penPlEntity = CEntity::GetPlayerEntity(0); //캐릭터 자기 자신
	penPlayerEntity = (CPlayerEntity*) penPlEntity;
	
	ResponseClient::moveMsg* pPack = reinterpret_cast<ResponseClient::moveMsg*>(istr->GetBuffer());
	
	switch( pPack->charType )
	{
	case MSG_CHAR_PC:
		penPlayerEntity->Read_net_Character(MSG_MOVE, 0, &(*istr));	
		break;
	case MSG_CHAR_NPC:
		penPlayerEntity->Read_net_Mob(MSG_MOVE, &(*istr));
		break;
	case MSG_CHAR_PET:
		penPlayerEntity->Read_net_Pet(MSG_MOVE, &(*istr));
		break;
	case MSG_CHAR_ELEMENTAL:
		penPlayerEntity->Read_net_Summon(MSG_MOVE, &(*istr));
		break;
	case MSG_CHAR_WILDPET:
		penPlayerEntity->Read_net_WildPet(MSG_MOVE, &(*istr));
		break;
	}
}

//-----------------------------------------------------------------------------
// Purpose:
// Input  : *istr - 
//-----------------------------------------------------------------------------
// 서버간에 이동이 있을때 쓰는 게 GO_ZONE
// 이동이 없을때 쓰는 게 GO_TO
// FiXME : 아래 함수 정리 필요함.
// FIXME : Restart_Internal()부분과 중복되는 부분이 너무 많음.  초기화하는 부분만 따로 빼낼것.
void CSessionState::ReceiveGoToMessage(CNetworkMessage *istr)
{
	CEntity* penPlEntity;
	CPlayerEntity* penPlayerEntity;
	penPlEntity = CEntity::GetPlayerEntity(0); //캐릭터 자기 자신
	penPlayerEntity = (CPlayerEntity*) penPlEntity;
	
	//0917
	INFO()->TargetClear();
	
	CUIManager* pUIManager = CUIManager::getSingleton();
	ResponseClient::goMsg* pPack = reinterpret_cast<ResponseClient::goMsg*>(istr->GetBuffer());
	
	if(pPack->zoneIndex == g_slZone)	//현재 world내 이동
	{
		ACTORMGR()->RemoveAll();
		penPlayerEntity->Read_net_Character(MSG_GOTO, 0, &(*istr));

		UIMGR()->SetCSFlagOff(CSF_TELEPORT);
	}
	else	//다른 world(zone)로 이동, 재접필요없이 클라이언트 내부 처리.
	{
		_pNetwork->SetDelivery(false);
		g_slZone = pPack->zoneIndex;//0921
		STAGEMGR()->SetNextStage(eSTAGE_ZONELOADING);		
	}
}

// [KH_070316] 변경 프리미엄 메모리 관련
void CSessionState::ReceiveMemPosMessage(CNetworkMessage *istr)
{
	pTypeBase* pBase = reinterpret_cast<pTypeBase*>(istr->GetBuffer());
	CUIManager* pUIManager = CUIManager::getSingleton();

	switch(pBase->subType)
	{
	case MSG_MEMPOS_LIST:
		{
			ResponseClient::memposList* pPack = reinterpret_cast<ResponseClient::memposList*>(istr->GetBuffer());
			
			if(pBase->type == MSG_MEMPOS)
				pUIManager->GetTeleport()->ClearTeleportList();
			else if(pBase->type == XXXMSG_MEMPOSPLUS)
				pUIManager->GetTeleportPrimium()->ClearTeleportList();
			
			pUIManager->GetTeleport()->SetUseTime(pPack->memposTime);

			if (pPack->listCount <= 0)
				return;			

			ResponseClient::memposElement* pData = new ResponseClient::memposElement[pPack->listCount];
			memcpy( pData, &pPack->list[0], sizeof(ResponseClient::memposElement) * pPack->listCount );

			for ( int i = 0; i < pPack->listCount ; ++i )
			{
				if(pBase->type == MSG_MEMPOS && pData[i].index != 255)
					pUIManager->GetTeleport()->SetTeleportInfo( pData[i].index, pData[i].zone, pData[i].x, pData[i].z, CTString(pData[i].comment) );
				else if(pBase->type == XXXMSG_MEMPOSPLUS)
					pUIManager->GetTeleportPrimium()->SetTeleportInfo( pData[i].index, pData[i].zone, pData[i].x, pData[i].z, CTString(pData[i].comment) );
			}

			SAFE_ARRAY_DELETE(pData);
		}
		break;
	case MSG_MEMPOS_WRITE:
		{
			ResponseClient::memposWrite* pPack = reinterpret_cast<ResponseClient::memposWrite*>(istr->GetBuffer());

			if(pBase->type == MSG_MEMPOS)
				pUIManager->GetTeleport()->SetTeleportInfo( pPack->data.index, pPack->data.zone, pPack->data.x, pPack->data.z, CTString(pPack->data.comment) );
			else if(pBase->type == XXXMSG_MEMPOSPLUS)
				pUIManager->GetTeleportPrimium()->SetTeleportInfo( pPack->data.index, pPack->data.zone, pPack->data.x, pPack->data.z, CTString(pPack->data.comment) );
		}
		break;
	}
}
void CSessionState::ReceiveGmMessage(CNetworkMessage *istr)
{
	UBYTE ubGmSubType;
	(*istr) >> ubGmSubType;
	if(ubGmSubType == MSG_GM_WHOAMI)
	{
		(*istr) >> _pNetwork->m_ubGMLevel;
		CPrintF("I'm a GM, level %d.", _pNetwork->m_ubGMLevel);
	}
}
void CSessionState::ReceiveInventoryMessage(CNetworkMessage *istr)
{
/*
	SBYTE	bArrange;	//정렬시 1
	SBYTE	tabId;
	SBYTE	rowId;
	ULONG	uniIndex;
	ULONG	index;
	SBYTE	Wearing;
	ULONG	plus,plus2=0;
	ULONG	flag;
	ULONG	used;
	ULONG	used2;
	SQUAD	count;
	SBYTE	sbOptionCount;
	SBYTE	sbOptionType;
	LONG	lOptionLevel;
	LONG	lComUniIndex;

	CPrintF("Recieve Inven Msg.\n");
	CEntity			*penPlEntity;
	CPlayerEntity	*penPlayerEntity;
	penPlEntity = CEntity::GetPlayerEntity(0); //캐릭터 자기 자신
	penPlayerEntity = static_cast<CPlayerEntity *>(penPlEntity);

	(*istr) >> bArrange;
	(*istr) >> tabId;
	(*istr) >> rowId;

	CUIManager* pUIManager = CUIManager::getSingleton();

	//CPrintF(TRANS("MSG_INVENTORY : (TAB)%d (ROW)%d \n"), tabId,rowId);				
	for( int i = 0;i < 5; i++ )
	{
		(*istr) >> uniIndex;

		if( uniIndex == -1 )
		{
			_pNetwork->MySlotItem[tabId][rowId][i].Init();
			continue;
		}

		(*istr) >> index;
		(*istr) >> Wearing;
		(*istr) >> plus;
		(*istr) >> flag;
		(*istr) >> lComUniIndex;
		(*istr) >> used; 

		(*istr) >> used2;

		(*istr) >> count;
		(*istr) >> sbOptionCount;

		//CPrintF(TRANS("MSG_INVENTORY(%d): (uniIndex)%d (index)%d (wear)%d (plus)%d (flag)%d (count)%ld \n"), i+1,uniIndex,index,Wearing,plus,flag,count);
		// eons 2006.11.1
		CItemData* pItemData	= &_pNetwork->GetItemData(index);

		_pNetwork->MySlotItem[tabId][rowId][i].Item_Wearing = Wearing;
		_pNetwork->MySlotItem[tabId][rowId][i].ItemData		= pItemData;
		_pNetwork->MySlotItem[tabId][rowId][i].SetData( index, uniIndex, tabId, rowId, i,
			plus, flag, lComUniIndex, used, used2, Wearing, count );

		// 펫을 장착한 경우....
		if( pItemData->GetType() == CItemData::ITEM_ACCESSORY && 
			pItemData->GetSubType() == CItemData::ACCESSORY_PET &&
			Wearing != -1)
		{
			const INDEX iPetIndex = _pNetwork->MySlotItem[tabId][rowId][i].Item_Plus;
			CNetworkLibrary::sPetInfo	TempPet;
			TempPet.lIndex				= iPetIndex;
			INFO()->_PetTargetInfo.lIndex		= iPetIndex;
			std::vector<CNetworkLibrary::sPetInfo>::iterator iter = 
				std::find_if(_pNetwork->m_vectorPetList.begin(), _pNetwork->m_vectorPetList.end(), CNetworkLibrary::FindPet(TempPet) );
			if( iter != _pNetwork->m_vectorPetList.end() )
			{
				INFO()->_PetTargetInfo.iLevel		= (*iter).lLevel;
				INFO()->_PetTargetInfo.fHealth		= (*iter).lHP;
				INFO()->_PetTargetInfo.fMaxHealth	= (*iter).lMaxHP;
				INFO()->_PetTargetInfo.fMaxHungry	= (*iter).lMaxHungry;
				INFO()->_PetTargetInfo.fHungry		= (*iter).lHungry;
				INFO()->_PetTargetInfo.lAbility		= (*iter).lAbility;
				INFO()->_PetTargetInfo.bIsActive		= TRUE;				
				INFO()->_PetTargetInfo.strNameCard	= (*iter).strNameCard;
				
				INDEX iPetType;
				INDEX iPetAge;
				_pNetwork->CheckPetType( (*iter).sbPetTypeGrade, iPetType, iPetAge );
				
				INFO()->_PetTargetInfo.iType			= iPetType;
				INFO()->_PetTargetInfo.iAge			= iPetAge;
			}
		}	

		_pNetwork->MySlotItem[tabId][rowId][i].InitOptionData();

		//레어 아이템일때...
		if( pItemData->GetFlag() & ITEM_FLAG_RARE )
		{
			//옵션 개수가 0이면 미감정 레어아이템
			if( sbOptionCount ==0)
				_pNetwork->MySlotItem[tabId][rowId][i].SetRareIndex(0);
			//감정된 레어아이템이면...
			else
			{
				pUIManager->SetRareOption(istr, _pNetwork->MySlotItem[tabId][rowId][i]);
			}
		}
		//레어 아이템이 아니면.....
		else
		{
			LONG lOriginOptionVar = ORIGIN_VAR_DEFAULT;
			for( SBYTE sbOption = 0; sbOption < sbOptionCount; sbOption++ )
			{
				(*istr) >> sbOptionType;
				(*istr) >> lOptionLevel;
#ifdef REFORM_SYSTEM
				if ( pItemData->GetFlag() & ITEM_FLAG_ORIGIN )
				{
					(*istr) >> lOriginOptionVar;
				}
#endif

				_pNetwork->MySlotItem[tabId][rowId][i].SetOptionData( sbOption, sbOptionType, lOptionLevel, lOriginOptionVar );
			}
		}

		if ( pItemData->GetFlag() & ITEM_FLAG_ORIGIN)
		{
			SBYTE sbBelong, sbSkillcont;
			LONG lSkillIndex;
			SBYTE sbSkillLevel;

			(*istr) >> sbBelong;
			(*istr) >> sbSkillcont;

			_pNetwork->MySlotItem[tabId][rowId][i].SetItemBelong(sbBelong);

			for (SBYTE sbSkillpos = 0; sbSkillpos < sbSkillcont; sbSkillpos++)
			{
				(*istr) >> lSkillIndex;
				(*istr) >> sbSkillLevel;

				_pNetwork->MySlotItem[tabId][rowId][i].SetItemSkill(sbSkillpos, lSkillIndex, sbSkillLevel);
			}

		}

		(*istr) >> plus2;
		_pNetwork->MySlotItem[tabId][rowId][i].SetItemPlus2(plus2);

#if defined SOCKET_SYSTEM
		_pNetwork->MySlotItem[tabId][rowId][i].InitSocketInfo();
		// receive data about socket type. [6/23/2010 rumist]
		if( pItemData->GetFlag() & ITEM_FLAG_SOCKET )
		{
			SBYTE	sbSocketCreateCount = 0;
			SBYTE	sbSocketCount = 0;
			LONG	lSocketJewelIndex = 0;
			
#ifdef SOCKET_SYSTEM_S2 // 인벤토리 메시지 소켓 부분 [4/3/2013 Ranma]
			LONG	lSocketInfo[JEWEL_MAX_COUNT] = {-1,};
			int	j;
			
			for (j = 0; j < JEWEL_MAX_COUNT; j++)
			{
				(*istr) >> lSocketInfo[j];
				if (lSocketInfo[j] >= 0)
					sbSocketCreateCount++;
			}
			
			_pNetwork->MySlotItem[tabId][rowId][i].SetSocketCount( sbSocketCreateCount );
			
			for ( j = 0; j < JEWEL_MAX_COUNT; j++)
			{
				if (lSocketInfo[j] >= 0)
					_pNetwork->MySlotItem[tabId][rowId][i].SetSocketOption( j, lSocketInfo[j] );
			}
#else

			(*istr) >> sbSocketCreateCount;
			_pNetwork->MySlotItem[tabId][rowId][i].SetSocketCount( sbSocketCreateCount );

			(*istr) >> sbSocketCount;
			for( SBYTE si = 0; si < sbSocketCreateCount; ++si )
			{
				(*istr) >> lSocketJewelIndex;
				_pNetwork->MySlotItem[tabId][rowId][i].SetSocketOption( si, lSocketJewelIndex );
			}
#endif
		}
#endif

		pUIManager->GetInventory()->InitInventory( tabId, rowId, i, uniIndex, index, Wearing );

		if( CLacarette::IsCoinItem( index) )
		{
			pUIManager->GetLacarette()->SetCoinCount( index, count );
		}

		// BUGFIX : ITS(#1265) 와일드펫 마운트 후 존이동시 무기 이펙트 표시 오류 [6/15/2011 rumist]
		// 이펙트가 정상적인 모델(탑승시에는 차일드가 됨)에 붙을 수 있도록 현재 모델이 어떤 것인지 판단하여
		// 모델의 인스턴스를 확보함.
		CModelInstance *pMI			= NULL;
		// wildpet or pet 탑승일시 모델은 child로. [6/15/2011 rumist]
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
		else
		{
			pMI	= penPlayerEntity->GetModelInstance();
		}

		//INDEX PlayerType = _pNetwork->ga_srvServer.srv_apltPlayers[0].plt_penPlayerEntity->en_pcCharacter.pc_iPlayerType;//타이탄:0,나이트:1,healer:2 메이지:3	
		if(Wearing != -1 && _pNetwork->MyCharacterInfo.sbEvocationType == -1)
		{
			int WearPos = pItemData->GetWearingPosition();
			INDEX iCurWearIndex = -1;
			if ( !(pItemData->GetFlag()&ITEM_FLAG_COSTUME2))
			{
				INDEX iCosWearIndex = _pNetwork->MyCharacterInfo.CostumeWearing[WearPos].idbIndex;
				if (WearPos == WEAR_BACKWING)
				{
					iCosWearIndex = _pNetwork->MyCharacterInfo.CostumeWearing[WEAR_COSTUME_BACKWING].idbIndex;
				}
				if (iCosWearIndex < 0)
				{
					if (!(pItemData->GetWearingPosition() == WEAR_HELMET && (CTString)pItemData->GetItemSmcFileName() == MODEL_TREASURE))
					{
						penPlayerEntity->DeleteCurrentArmor(Wearing);//1005 아이템 깨지는 버그수정
						penPlayerEntity->DeleteDefaultArmor(Wearing);
					}
					
					if(!(pItemData->GetWearingPosition() == WEAR_HELMET && (CTString)pItemData->GetItemSmcFileName() == MODEL_TREASURE))
					{
						_pGameState->WearingArmor( pMI, *pItemData );
					}
					iCurWearIndex = index;
				}
				else
				{
					iCurWearIndex = iCosWearIndex;
				}
				// Date : 2005-04-07(오후 3:39:57), By Lee Ki-hwan
				_pNetwork->MyCharacterInfo.itemEffect.Change(_pNetwork->MyCharacterInfo.job
					, &_pNetwork->GetItemData(iCurWearIndex)
					, Wearing
					, plus
					, &pMI->m_tmSkaTagManager
					, 1, _pNetwork->GetItemData(iCurWearIndex).GetSubType() );
			}
		}
		
		_pNetwork->MyCharacterInfo.itemEffect.Refresh(&pMI->m_tmSkaTagManager, 1);
		_pNetwork->MyCharacterInfo.statusEffect.Refresh(&pMI->m_tmSkaTagManager, CStatusEffect::R_NONE);
	}


	if(bArrange & ITEM_ARRANGE_END)
	{
		for(int row=rowId+1;row<TOTAL_ROW;++row)
		{
			for(int col=0;col<TOTAL_COL;++col)
			{
				_pNetwork->MySlotItem[tabId][(row * INVEN_SLOT_COL) + col].Init();
			}
		}
	}
*/
}

//-----------------------------------------------------------------------------
// Purpose:
// Input  : *istr - 
//-----------------------------------------------------------------------------
// 서버간에 이동이 있을때 쓰는 게 GO_ZONE
// 이동이 없을때 쓰는 게 GO_TO
// FIXME : 아래 함수 정리가 필요함.
void CSessionState::ReceiveGoZoneMessage(CNetworkMessage *istr)
{
	ResponseClient::moveGoZone* pRecv = reinterpret_cast<ResponseClient::moveGoZone*>(istr->GetBuffer());

	CTString	ip = pRecv->ip;				

	MoveOtherServer(pRecv->zone, ip, pRecv->port);
	
	CPrintF(TRANS("MSG_GO_ZONE zone:%d extra:%d ip:%s port:%d \n"), pRecv->zone, pRecv->extra, ip, pRecv->port);	
}

//-----------------------------------------------------------------------------
// Purpose:
// Input  : *istr - 
//-----------------------------------------------------------------------------
void CSessionState::ReceiveItemMessage(CNetworkMessage *istr)
{
	OBFUSCATE();

	SBYTE	type;
	SBYTE	wear_type;						// 입는 위치 
	ULONG	item_uniindex;
	ULONG	item_index;
	ULONG	plus, flag;
	LONG	used, used2;
	SQUAD	count;
	FLOAT	x, h, z, r;
	SBYTE	ylayer;
	SBYTE	cha_type;
	ULONG	cha_index;
	SBYTE	tab,row,col;
	SQUAD	delta;

	//wooss 050818i
	int commandCode = -1;

	//wooss 050818
	CTString strTitle;
	CTString strMessage1,strMessage2;
	CUIMsgBox_Info	MsgBoxInfo;

	CEntity			*penPlEntity;
	CPlayerEntity	*penPlayerEntity;
	penPlEntity = CEntity::GetPlayerEntity(0);		// 캐릭터 자기 자신
	penPlayerEntity = static_cast<CPlayerEntity *>(penPlEntity);

	(*istr) >> type;

	CUIManager* pUIManager = CUIManager::getSingleton();

	switch(type)
	{
	case MSG_ITEM_DISAPPEAR: //0607 kwon
		{
			(*istr) >> item_uniindex;

			ACTORMGR()->RemoveObject(eOBJ_ITEM, item_uniindex);
			}
		break;

	case MSG_ITEM_APPEAR: //0607 kwon
		{
			(*istr) >> item_uniindex;
			(*istr) >> item_index;
			(*istr) >> count;
			(*istr) >> x;
			(*istr) >> z;
			(*istr) >> h;
			(*istr) >> r;
			(*istr) >> ylayer;

			CItemData* pItemData = _pNetwork->GetItemData(item_index);
			const char* szItemName = _pNetwork->GetItemName(item_index);

			CItemTarget* pTarget = new CItemTarget;

			CEntity* penEntity;
			CPlacement3D plPlacement;
			plPlacement.pl_PositionVector(1) = x;
			plPlacement.pl_PositionVector(2) = h + 1.0f;
			plPlacement.pl_PositionVector(3) = z;	
			plPlacement.pl_OrientationAngle = ANGLE3D(0.0f,0.0f,0.0f);

			penEntity = _pNetwork->ga_World.CreateEntity_t(plPlacement, CLASS_ITEM, -1, TRUE);
			pTarget->SetData( item_uniindex, szItemName, penEntity, ylayer, count,FLOAT3D(x,h,z), item_index);	//0은 dummy
			pTarget->m_nType = item_index;
			
			if (penEntity != NULL)
			{
				pTarget->m_nIdxClient	= penEntity->en_ulID;
				penEntity->en_strItemName = szItemName;
				SetDropItemModel(penEntity, pItemData, pTarget);
				penEntity->SetNetworkID(item_uniindex);
			}

			ACTORMGR()->AddObject(pTarget);
		}
		break;

	case MSG_ITEM_DROP:
		{
			(*istr) >> item_uniindex;
			(*istr) >> item_index;
			(*istr) >> count;
			(*istr) >> x;
			(*istr) >> z;
			(*istr) >> h;
			(*istr) >> r;
			(*istr) >> ylayer;
			(*istr) >> cha_type;
			(*istr) >> cha_index;

			//안태훈 수정 시작	//(5th Closed beta)(0.2)
			CItemData *pItemData	= _pNetwork->GetItemData(item_index);
			const char* szItemName = _pNetwork->GetItemName(item_index);
			
			CItemTarget* pTarget = new CItemTarget;
			
			pTarget->SetData(item_uniindex, szItemName, NULL, ylayer, count, FLOAT3D(x,h,z), item_index);//0826
			pTarget->m_nType = item_index;

			if(cha_type == MSG_CHAR_NPC)
			{
				BYTE bLive = 0;

				(*istr) >> bLive; // NPC가 살아 있는가? 1(LIVE), 0(DEATH)

				// edit by cpp2angel (044.12.20) : 자동 도움말
				if ( cha_index == _UIAutoHelp->GetKillMonIndex () )
				{
					_UIAutoHelp->SetInfo ( AU_ITEM_DROP );
				}
				// ... End Edit 

				bool bFind = false;

				if (bLive < ISNPCLIVE)
				{
					ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_ITEM, cha_index);
					if (pObject != NULL)
					{						
						((CPlayerEntity*)CEntity::GetPlayerEntity(0))->AddDeathItem(
							pObject->GetEntity(), FLOAT3D(x,h+1,z), pTarget, pItemData);
						pObject->m_nType = item_index;

						bFind = true;
					}
				}

				// FIXME : 아이템 표시하는 부분 중에서 중복되는 부분이 너무 많음.
				if(bFind == false || bLive == ISNPCLIVE)
				{
					char *szEffectName = NULL;
					CEntity* penEntity;
					CPlacement3D plPlacement;

					plPlacement.pl_PositionVector(1) = x;
					plPlacement.pl_PositionVector(2) = h + 1.0f;
					plPlacement.pl_PositionVector(3) = z;	
					plPlacement.pl_OrientationAngle = ANGLE3D(0.0f,0.0f,0.0f);
					
					penEntity = _pNetwork->ga_World.CreateEntity_t(plPlacement, CLASS_ITEM,-1,TRUE);
					penEntity->SetNetworkID(item_uniindex);
					
					if (penEntity != NULL)
					{
						pTarget->m_nIdxClient	= penEntity->en_ulID;
						pTarget->m_pEntity		= penEntity;
						penEntity->en_strItemName = szItemName;
						SetDropItemModel(penEntity, pItemData, pTarget);
					}

					ACTORMGR()->AddObject(pTarget);
				}				
				}
			else	//MSG_CHA_PC
			{
				if( cha_index == _pNetwork->MyCharacterInfo.index )
				{

					CTString	strSysMessage;

					if( pItemData->GetType() == CItemData::ITEM_ETC &&
						pItemData->GetSubType() == CItemData::ITEM_ETC_MONEY )	// 0829 드롭사운드
					{
						penPlayerEntity->PlayItemSound( FALSE, TRUE );

						CTString strCount;
						strCount.PrintF( "%I64d", count );
						pUIManager->InsertCommaToString( strCount );

						strSysMessage.PrintF( _S( 301, "%s 나스를 버렸습니다." ), strCount );	// 번역 수정
						_pNetwork->ClientSystemMessage( strSysMessage );
					}
					else
					{
						penPlayerEntity->PlayItemSound( FALSE, FALSE );

						strSysMessage.PrintF( _S( 302, "%s %d개를 버렸습니다." ), szItemName, count );
						_pNetwork->ClientSystemMessage( strSysMessage );		
					}
				}

				CEntity* penEntity;
				CPlacement3D plPlacement;
				plPlacement.pl_PositionVector(1) = x;
				plPlacement.pl_PositionVector(2) = h + 1.0f;
				plPlacement.pl_PositionVector(3) = z;	
				plPlacement.pl_OrientationAngle = ANGLE3D(0.0f,0.0f,0.0f);
				penEntity = _pNetwork->ga_World.CreateEntity_t(plPlacement, CLASS_ITEM,-1,TRUE);
				penEntity->SetNetworkID(item_uniindex);

				if (penEntity != NULL)
				{
					pTarget->m_nIdxClient	= penEntity->en_ulID;
					pTarget->m_pEntity		= penEntity;
					penEntity->en_strItemName = szItemName;

					SetDropItemModel(penEntity, pItemData, pTarget);
				}

				ACTORMGR()->AddObject(pTarget);		
			}			

			// [091216: selo] 스킬 배우기 UI 갱신
			if(pUIManager->DoesUIExist(UI_SKILLLEARN))
			{
				pUIManager->GetSkillLearn()->UpdateSkillLearn();
			}
		}
		break;

	case MSG_ITEM_UPDATE:
		{
			LONG	lComUniIndex;
			SBYTE	sbOptionCount;
			SBYTE	sbOptionType;
			LONG	lOptionLevel;

			(*istr) >> tab;
			(*istr) >> row;
			(*istr) >> col;
			(*istr) >> item_uniindex;
			(*istr) >> plus;
			(*istr) >> flag;
			(*istr) >> lComUniIndex; // 합성된 아이템 인덱스(when upgrade item)
			(*istr) >> used;
			(*istr) >> used2;
			(*istr) >> count;
			(*istr) >> delta;
			(*istr) >> sbOptionCount;

			CItems* pItems = &_pNetwork->MySlotItem[tab][(row * INVEN_SLOT_COL) + col];
			pItems->Item_Plus = plus;
			pItems->Item_Flag = flag;
			pItems->ComItem_index = lComUniIndex;
			pItems->Item_Used = used;
			pItems->Item_Used2 = used2;
			pItems->Item_Sum  = count;

			_pNetwork->MySlotItem[tab][(row * INVEN_SLOT_COL) + col].InitOptionData();

			if( pItems->ItemData->GetFlag() & ITEM_FLAG_RARE )
			{
				//옵션 개수가 0이면 미감정 레어아이템
				if( sbOptionCount ==0)
					_pNetwork->MySlotItem[tab][(row * INVEN_SLOT_COL) + col].SetRareIndex(0);
				//감정된 레어아이템이면...
				else
					pUIManager->SetRareOption(istr, _pNetwork->MySlotItem[tab][(row * INVEN_SLOT_COL) + col]);
			}
			else
			{
				LONG lOriginOptionVar = ORIGIN_VAR_DEFAULT;

				for( SBYTE sbOption = 0; sbOption < sbOptionCount; sbOption++ )
				{
					(*istr) >> sbOptionType;
					(*istr) >> lOptionLevel;

					if ( pItems->ItemData->GetFlag() & ITEM_FLAG_ORIGIN )
					{
						(*istr) >> lOriginOptionVar;
					}
					_pNetwork->MySlotItem[tab][(row * INVEN_SLOT_COL) + col].SetOptionData( sbOption, sbOptionType, lOptionLevel, lOriginOptionVar );
				}
			}

			if ( pItems->ItemData->GetFlag() & ITEM_FLAG_ORIGIN)
			{
				SBYTE sbBelong, sbSkillcont;
				LONG lSkillIndex;
				SBYTE sbSkillLevel;

				(*istr) >> sbBelong;
				(*istr) >> sbSkillcont;

				_pNetwork->MySlotItem[tab][(row * INVEN_SLOT_COL) + col].SetItemBelong(sbBelong);

				for (SBYTE sbSkillpos = 0; sbSkillpos < sbSkillcont; sbSkillpos++)
				{
					(*istr) >> lSkillIndex;
					(*istr) >> sbSkillLevel;

					_pNetwork->MySlotItem[tab][(row * INVEN_SLOT_COL) + col].SetItemSkill(sbSkillpos, lSkillIndex, sbSkillLevel);
				}

			}

			LONG lplus2;
			(*istr) >> lplus2;
			_pNetwork->MySlotItem[tab][(row * INVEN_SLOT_COL) + col].SetItemPlus2(lplus2);
			_pNetwork->MySlotItem[tab][(row * INVEN_SLOT_COL) + col].InitSocketInfo();
			// data receive about socket type. [6/23/2010 rumist]
			if( pItems->ItemData->GetFlag() & ITEM_FLAG_SOCKET )
			{
				SBYTE	sbSocketCreateCount = 0;
				SBYTE	sbSocketCount = 0;
				LONG	lSocketJewelIndex = 0;
				LONG	lSocketInfo[JEWEL_MAX_COUNT] = {-1,};
				int	i;

				for (i = 0; i < JEWEL_MAX_COUNT; i++)
				{
					(*istr) >> lSocketInfo[i];
					if (lSocketInfo[i] >= 0)
						sbSocketCreateCount++;
				}

				_pNetwork->MySlotItem[tab][(row * INVEN_SLOT_COL) + col].SetSocketCount( sbSocketCreateCount );	

				for (i = 0; i < JEWEL_MAX_COUNT; i++)
				{
					if (lSocketInfo[i] >= 0)
						_pNetwork->MySlotItem[tab][(row * INVEN_SLOT_COL) + col].SetSocketOption( i, lSocketInfo[i] );
				}
			}

			pUIManager->GetInventory()->InitInventory( tab, (row * INVEN_SLOT_COL) + col, item_uniindex, pItems->Item_Index,
				pItems->Item_Wearing );

			if( delta > 0 )
			{
				CTString	strSysMessage;
				CItemData	*pItemData	= pItems->ItemData;
				const char* szItemName	= _pNetwork->GetItemName(pItems->Item_Index);

				strSysMessage.PrintF( _S( 417, "%s %d개를 얻었습니다." ), szItemName, delta );
				_pNetwork->ClientSystemMessage( strSysMessage );

			}

			// edit by cpp2angel (044.12.20) : 
			if ( pUIManager->GetProcess()->IsVisible () )
			{
				pUIManager->GetProcess()->SelectItem ();
			}

			if ( pUIManager->GetProduct()->IsVisible () )
			{
				pUIManager->GetProduct()->SelectItem ();
			}

			// Date : 2005-04-07(오후 3:40:01), By Lee Ki-hwan
			INDEX iCurWearIndex = pItems->Item_Index;
			if (_pNetwork->MyWearCostItem[pItems->Item_Wearing].Item_Index >0)
			{
				iCurWearIndex = _pNetwork->MyWearCostItem[pItems->Item_Wearing].Item_Index;
			}

			CModelInstance *pMI	= NULL;
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
			else
			{
				pMI	= penPlayerEntity->GetModelInstance();
			}			
			_pNetwork->MyCharacterInfo.itemEffect.Change(_pNetwork->MyCharacterInfo.job
				, _pNetwork->GetItemData(iCurWearIndex)
				, pItems->Item_Wearing
				, pItems->Item_Plus
				, &pMI->m_tmSkaTagManager
				, 1, _pNetwork->GetItemData(iCurWearIndex)->GetSubType() );

			// [sora] 090731 퀵슬롯 아이템 count update
			pUIManager->GetQuickSlot()->UpdateItemCount(item_uniindex, count);

			// [091216: selo] 스킬 배우기 UI 갱신
			if(pUIManager->DoesUIExist(UI_SKILLLEARN))
			{
				pUIManager->GetSkillLearn()->UpdateSkillLearn();
			}
		}
		break;

	case MSG_ITEM_ADD:
		{
			LONG	lComUniIndex;
			SBYTE	sbOptionCount;
			SBYTE	sbOptionType;
			LONG	lOptionLevel;

			(*istr) >> tab;
			(*istr) >> row;
			(*istr) >> col;
			(*istr) >> item_uniindex;					
			(*istr) >> item_index;
			(*istr) >> wear_type;
			(*istr) >> plus;
			(*istr) >> flag;
			(*istr) >> lComUniIndex; // 합성된 아이템 인덱스(when upgrade item)
			(*istr) >> used;
			(*istr) >> used2;
			(*istr) >> count;
			(*istr) >> sbOptionCount;

			Notice* pNotice = GAMEDATAMGR()->GetNotice();

			// 드레이크의 알을 얻었을 경우, Notice에 표시해줌.
			if( item_index == 949 )
			{
				if( !ItemHelp::HaveItem( item_index ) && !ItemHelp::HaveItem( 872 ))
				{
					if (pNotice != NULL)
						pNotice->AddToNoticeList(4001, Notice::NOTICE_EVENT);
				}
			}

			// 판의 피리를 얻었을 경우, Notice에 표시해줌.
			if( item_index == 948 )
			{
				if( !ItemHelp::HaveItem( item_index ) && !ItemHelp::HaveItem( 871 ) )
				{
					if (pNotice != NULL)
						pNotice->AddToNoticeList(4000, Notice::NOTICE_EVENT);
				}
			}

			// wooss 060810
			// 핑크 드레이크의 알을 얻었을 경우, Notice에 표시해줌.
			if( item_index == 1706 )
			{
				if( !ItemHelp::HaveItem( item_index ) && !ItemHelp::HaveItem( 1711 ) )
				{
					if (pNotice != NULL)
						pNotice->AddToNoticeList(4003, Notice::NOTICE_EVENT);
				}
			}
			// 불가사의한 드레이크의 알을 얻었을 경우, Notice에 표시해줌.
			if( item_index == 1709 )
			{
				if( !ItemHelp::HaveItem( item_index ) && !ItemHelp::HaveItem( 1712 ) )
				{
					if (pNotice != NULL)
						pNotice->AddToNoticeList(4005, Notice::NOTICE_EVENT);
				}
			}
			// 파란 판의 피리를 얻었을 경우, Notice에 표시해줌.
			if( item_index == 1707 )
			{
				if( !ItemHelp::HaveItem( item_index ) && !ItemHelp::HaveItem( 1710 ) )
				{
					if (pNotice != NULL)
						pNotice->AddToNoticeList(4002, Notice::NOTICE_EVENT);
				}
			}
			// 불가사의한의 피리를 얻었을 경우, Notice에 표시해줌.
			if( item_index == 1708 )
			{
				if( !ItemHelp::HaveItem( item_index ) && !ItemHelp::HaveItem( 1713 ) )
				{
					if (pNotice != NULL)
						pNotice->AddToNoticeList(4004, Notice::NOTICE_EVENT);
				}
			}

			CItemData* pItemData = _pNetwork->GetItemData(item_index);
			const char* szItemName = _pNetwork->GetItemName( item_index );

			_pNetwork->MySlotItem[tab][(row * INVEN_SLOT_COL) + col].ItemData = pItemData;
			_pNetwork->MySlotItem[tab][(row * INVEN_SLOT_COL) + col].SetData( item_index, item_uniindex, 
				tab, (row * INVEN_SLOT_COL) + col, plus, flag, 
				lComUniIndex, used, used2, wear_type, count );

			_pNetwork->MySlotItem[tab][(row * INVEN_SLOT_COL) + col].InitOptionData();

			if( pItemData->GetFlag() & ITEM_FLAG_RARE)
			{
				//옵션 개수가 0이면 미감정 레어아이템
				if( sbOptionCount ==0)
					_pNetwork->MySlotItem[tab][(row * INVEN_SLOT_COL) + col].SetRareIndex(0);
				//감정된 레어아이템이면...
				else
					pUIManager->SetRareOption(istr, _pNetwork->MySlotItem[tab][(row * INVEN_SLOT_COL) + col]);
			}
			else
			{
				LONG lOriginOptionVar = ORIGIN_VAR_DEFAULT;

				for( SBYTE sbOption = 0; sbOption < sbOptionCount; sbOption++ )
				{
					(*istr) >> sbOptionType;
					(*istr) >> lOptionLevel;

					if ( pItemData->GetFlag() & ITEM_FLAG_ORIGIN )
					{
						(*istr) >> lOriginOptionVar;
					}

					_pNetwork->MySlotItem[tab][(row * INVEN_SLOT_COL) + col].SetOptionData( sbOption, sbOptionType, lOptionLevel, lOriginOptionVar );
				}
			}

			if ( pItemData->GetFlag() & ITEM_FLAG_ORIGIN)
			{
				SBYTE sbBelong, sbSkillcont;
				LONG lSkillIndex;
				SBYTE sbSkillLevel;

				(*istr) >> sbBelong;
				(*istr) >> sbSkillcont;

				_pNetwork->MySlotItem[tab][(row * INVEN_SLOT_COL) + col].SetItemBelong(sbBelong);

				for (SBYTE sbSkillpos = 0; sbSkillpos < sbSkillcont; sbSkillpos++)
				{
					(*istr) >> lSkillIndex;
					(*istr) >> sbSkillLevel;

					_pNetwork->MySlotItem[tab][(row * INVEN_SLOT_COL) + col].SetItemSkill(sbSkillpos, lSkillIndex, sbSkillLevel);
				}
			}

			LONG lplus2;
			(*istr) >> lplus2;
			_pNetwork->MySlotItem[tab][(row * INVEN_SLOT_COL) + col].SetItemPlus2(lplus2);

			if(wear_type != -1)
			{								
				penPlayerEntity->DeleteDefaultArmor(wear_type);
				//penPlayerEntity->WearingArmor(tab, (row * INVEN_SLOT_COL) + col);
				CModelInstance *pMI			= NULL;
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
				else
				{
					pMI	= penPlayerEntity->GetModelInstance();
				}			
				_pGameState->WearingArmor( pMI, *pItemData );				
			}

			_pNetwork->MySlotItem[tab][(row * INVEN_SLOT_COL) + col].InitSocketInfo();

			if( pItemData->GetFlag() & ITEM_FLAG_SOCKET )
			{
				SBYTE	sbSocketCreateCount = 0;
				SBYTE	sbSocketCount = 0;
				LONG	lSocketJewelIndex = 0;
				LONG	lSocketInfo[JEWEL_MAX_COUNT] = {-1,};
				int	i;

				for (i = 0; i < JEWEL_MAX_COUNT; i++)
				{
					(*istr) >> lSocketInfo[i];
					if (lSocketInfo[i] >= 0)
						sbSocketCreateCount++;
				}

				_pNetwork->MySlotItem[tab][(row * INVEN_SLOT_COL) + col].SetSocketCount( sbSocketCreateCount );	

				for (i = 0; i < JEWEL_MAX_COUNT; i++)
				{
					if (lSocketInfo[i] >= 0)
						_pNetwork->MySlotItem[tab][(row * INVEN_SLOT_COL) + col].SetSocketOption( i, lSocketInfo[i] );
				}
			}

			pUIManager->GetInventory()->InitInventory( tab, (row * INVEN_SLOT_COL) + col, item_uniindex, item_index, wear_type );	

			CTString	strSysMessage;

			if( count > 0 )
				strSysMessage.PrintF( _S( 417, "%s %d개를 얻었습니다." ), szItemName, count );
			else
				strSysMessage.PrintF( _S2( 303, szItemName, "%s<를> 얻었습니다." ),
				szItemName );

			_pNetwork->ClientSystemMessage( strSysMessage );

			// edit by cpp2angel (044.12.20) : 
			if ( pUIManager->GetProcess()->IsVisible () )
			{
				pUIManager->GetProcess()->SelectItem ();
			}

			if ( pUIManager->GetProduct()->IsVisible () )
			{
				pUIManager->GetProduct()->SelectItem ();
			}

			if(pUIManager->GetChangeWeapon()->GetCashItem()){ //wooss 051011
				CUIMsgBox_Info	MsgBoxInfo;
				if( pItemData->GetType() == CItemData::ITEM_WEAPON)
				{
					MsgBoxInfo.SetMsgBoxInfo( _S(1049,"무기 교체" ), UMBS_OK,UI_NONE, MSGCMD_NULL);
					MsgBoxInfo.AddString( _S(2229, "무기가 성공적으로 교체되었습니다.") );
				}
				else if(pItemData->GetType() == CItemData::ITEM_SHIELD)
				{
					MsgBoxInfo.SetMsgBoxInfo( _S(3536, "방어구 교체" ), UMBS_OK,UI_NONE, MSGCMD_NULL);
					MsgBoxInfo.AddString( _S(3537, "방어구가 성공적으로 교체되었습니다.") );
				}
				pUIManager->CreateMessageBox( MsgBoxInfo );
				pUIManager->GetChangeWeapon()->close();												
			}

			// [091216: selo] 스킬 배우기 UI 갱신
			if(pUIManager->DoesUIExist(UI_SKILLLEARN))
			{
				pUIManager->GetSkillLearn()->UpdateSkillLearn();
			}	
		}						
		break;

	case MSG_ITEM_USE :
		{			
			updateItemUse(istr);
		}
		break;

	case MSG_ITEM_TAKE:
		{
			penPlayerEntity->Read_net_Character(MSG_ITEM, MSG_ITEM_TAKE, &(*istr));	
			/*
			(*istr) >> cha_index;
			(*istr) >> item_uniindex;		

			for( ipl = 0; ipl < _pNetwork->ga_srvServer.srv_aitItem.Count(); ipl++ ) 
			{
			CItemTarget	&it = _pNetwork->ga_srvServer.srv_aitItem[ipl];
			if( it.item_Index == item_uniindex )
			{
			it.Init();
			//안태훈 수정 시작	//(5th Closed beta)(0.2)
			_pNetwork->ga_srvServer.srv_aitItem.SwapAndPop(ipl);
			//안태훈 수정 끝	//(5th Closed beta)(0.2)
			}						 												
			}
			*/
		}
		break;
	case MSG_ITEM_GET:
		{
			penPlayerEntity->Read_net_Character(MSG_ITEM, MSG_ITEM_GET, &(*istr));	
		}
		break;

	case MSG_ITEM_DELETE:
		{
			(*istr) >> tab;
			(*istr) >> row;
			(*istr) >> col;

			CItemData* pItemData = _pNetwork->MySlotItem[tab][(row * INVEN_SLOT_COL) + col].ItemData;			

			// 펫인 경우 리스트에서 제거합니다.
			if( pItemData->GetType() == CItemData::ITEM_ACCESSORY && 
				(pItemData->GetSubType() == CItemData::ACCESSORY_PET || pItemData->GetSubType() == CItemData::ACCESSORY_WILDPET))
			{
				const INDEX iPetIndex = _pNetwork->MySlotItem[tab][(row * INVEN_SLOT_COL) + col].Item_Plus;
				CNetworkLibrary::sPetInfo	TempPet;
				TempPet.lIndex				= iPetIndex;
				std::vector<CNetworkLibrary::sPetInfo>::iterator iter = 
					std::find_if(_pNetwork->m_vectorPetList.begin(), _pNetwork->m_vectorPetList.end(), CNetworkLibrary::FindPet(TempPet) );
				if( iter != _pNetwork->m_vectorPetList.end() )
				{
					_pNetwork->m_vectorPetList.erase( iter );
				}
			}

			_pNetwork->MySlotItem[tab][(row * INVEN_SLOT_COL) + col].Init();
			pUIManager->GetInventory()->InitInventory( tab, (row * INVEN_SLOT_COL) + col, -1, -1, -1 );

			// [091216: selo] 스킬 배우기 UI 갱신
			if(pUIManager->DoesUIExist(UI_SKILLLEARN))
			{
				pUIManager->GetSkillLearn()->UpdateSkillLearn();
			}
		}
		break;

	case MSG_ITEM_UPGRADE_REP:
		{
			SBYTE	sbResult,sbisRune,sbPlus;
			(*istr) >> sbResult;
			(*istr) >> sbisRune;

			if (sbisRune)
			{
				(*istr) >> sbPlus;
				pUIManager->GetInventory()->UpgradeItem( sbResult, sbPlus );
			}
			else pUIManager->GetInventory()->UpgradeItem( sbResult );
		}
		break;

	case MSG_ITEM_REFINE_REP:
		{
			SBYTE	sbResult;
			(*istr) >> sbResult;
			pUIManager->GetRefine()->RefineRep( sbResult );
		}
		break;

	case MSG_ITEM_PROCESS_REP: 
		{
			SBYTE	sbResult;
			(*istr) >> sbResult;			

			pUIManager->GetProcess()->ProcessRep ( sbResult ); // 이기환 수정 ( 12.3 )
		}
		break;

		// 이기환 수정 ( 12.6 ) 추가 
	case MSG_ITEM_MAKE_REP:
		{
			SBYTE	sbResult;
			(*istr) >> sbResult;			

			pUIManager->GetProduct()->ProductRep ( sbResult ); 
		}
		break;
		// 이기환 수정 끝 

		// 이기환 추가 (12.8)
	case MSG_ITEM_MIX_REP: 
		{
			SBYTE	sbResult;
			(*istr) >> sbResult;			

			pUIManager->GetMix()->MixRep ( sbResult ); 
		}
		break;
		// 이기환 추가 끝
	case MSG_ITEM_ARCANE_REP: 
		{
			SBYTE	sbResult;
			(*istr) >> sbResult;			

			pUIManager->GetCompound()->CompoundRep ( sbResult ); 
		}
		break;



	case MSG_ITEM_OPTION_ADD_REP:
		{
			SBYTE	sbResult;
			(*istr) >> sbResult;
			pUIManager->GetInventory()->OptionAddRep( sbResult );
		}
		break;
	case MSG_ITEM_OPTION_DEL_REP:
		{
			SBYTE	sbResult;
			(*istr) >> sbResult;
			pUIManager->GetInventory()->OptionDelRep( sbResult );
		}
		break;
	case MSG_ITEM_CHANGEWEAPON_REP:
		{
			// 무기 교환 응답		: old_itemdbindex(n) new_itemdbindex(n)
			LONG lOldItemIndex;
			LONG lNewItemIndex;
			(*istr) >> lOldItemIndex;
			(*istr) >> lNewItemIndex;

			// 돈이 모자른 경우.
			if( lOldItemIndex == -1 )
			{	
				pUIManager->GetChattingUI()->AddSysMessage( _S( 1328, "나스가 부족하여 무기를 교체할 수 없습니다." ), SYSMSG_ERROR );		
			}
			// 잘못된 교환을 요청한 경우.
			else if( lNewItemIndex == -1 )
			{
				pUIManager->GetChattingUI()->AddSysMessage( _S( 1329, "무기를 교체할 수 없습니다." ), SYSMSG_ERROR );		
			}
			pUIManager->GetChangeWeapon()->close();
			pUIManager->GetChangeEquipment()->close();
		}
		break;
	case MSG_ITEM_PROCESS_NPC:
		{
			//processItemDBIndex(n) count(n) errcode(n:s)
			LONG processItemDBIndex;
			LONG count;
			LONG errcode;
			(*istr) >> processItemDBIndex;
			(*istr) >> count;
			(*istr) >> errcode;

			pUIManager->GetProcessNPC()->ProcessNPCRep(processItemDBIndex, count, errcode);
		} break;
	case MSG_ITEM_ADD_BOOSTER:
		{
			LONG flag;
			(*istr) >> flag;
			CTString strTitle;
			CTString strContent;

			int commandCode = -1;
			switch(flag)
			{
			case MSG_ITEM_ADD_BOOSTER_ERROR_OK:		// 성공
				{
					strTitle = _S( 1825, "업그레이드 성공" );
					strContent = _S( 1826, "부스터 업그레이드에 성공했습니다." );
					commandCode = MSGCMD_BOOSTER_UPGRADE_REP;
				} break;
			case MSG_ITEM_ADD_BOOSTER_ERROR_USED:	// 실패
				{
					strTitle = _S( 1827, "업그레이드 불가" );
					strContent = _S( 1828, "내구도가 부족합니다." );
					commandCode = MSGCMD_BOOSTER_UPGRADE_REP;
				} break;
			case MSG_ITEM_ADD_BOOSTER_ERROR_ALREADY:	// 카운트 오류
				{
					strTitle = _S( 1827, "업그레이드 불가" );
					strContent = _S( 1829, "이미 부스터 업그레이드가 되어 있는 아이템입니다." );
					commandCode = MSGCMD_BOOSTER_UPGRADE_REP;
				} break;
			}

			pUIManager->CloseMessageBox(MSGCMD_INVEN_UPGRADE_REP);
			CUIMsgBox_Info	MsgBoxInfo;
			MsgBoxInfo.SetMsgBoxInfo( strTitle, UMBS_OK, UI_INVENTORY, commandCode );
			MsgBoxInfo.AddString( strContent );
			pUIManager->CreateMessageBox( MsgBoxInfo );

			pUIManager->GetInventory()->Lock( FALSE, FALSE, LOCK_UPGRADE_ITEM  );
		} break;
		//wooss 050816
		//item prolong message
	case MSG_ITEM_USE_PROLONG :

		ULONG useTime;
		UBYTE prolong;

		(*istr) >> item_index;
		(*istr) >> useTime;
		(*istr) >> prolong;

		if(item_index==MEMSCROLL_EX_ITEM || item_index == MEMSCROLL_EX_ITEM_7DAYS)
		{
			pUIManager->GetTeleport()->SetUseTime(useTime);

			if(prolong&&useTime>0){
				//메모리 스크롤을 이미 사용하고 있을때
				//wooss 050816
				strTitle	=	_S(2088 , "사용 확인" );
				strMessage1	=_S( 2128, 	"이미 메모리 스크롤이 확장되어 있는 상태 입니다." ); 
				strMessage2 =   _S(2148 , "기간을 연장하시겠습니까?" );	
				MsgBoxInfo.SetMsgBoxInfo(strTitle,UMBS_YESNO,UI_NONE,MSGCMD_PROLONG_MEMSCROLL_EX);
				MsgBoxInfo.AddString(strMessage1);
				MsgBoxInfo.AddString(strMessage2);
				pUIManager->CreateMessageBox(MsgBoxInfo);
			}
		}
		else if(item_index==WAREHOUSE_EX_ITEM || item_index ==WAREHOUSE_EX_ITEM_7DAYS )
		{
			pUIManager->GetWareHouse()->SetUseTime(useTime);

			if(prolong&&useTime>0){
				//확장창고를 이미 사용하고 있을때
				//wooss 050816
				strTitle	=_S( 2088, 	"사용 확인" );
				strMessage1	=_S(2129 , 	"이미 개인 창고가 확장되어 있는 상태 입니다." );
				strMessage2 =   _S(2148 , "기간을 연장하시겠습니까?" );	
				MsgBoxInfo.SetMsgBoxInfo(strTitle,UMBS_YESNO,UI_NONE,MSGCMD_PROLONG_WAREHOUSE_EX);
				MsgBoxInfo.AddString(strMessage1);
				MsgBoxInfo.AddString(strMessage2);
				pUIManager->CreateMessageBox(MsgBoxInfo);
			}
		}
		break;
	case MSG_ITEM_USE_ERROR:
		updateItemUseError(istr);
		break;
	case MSG_ITEM_LEND_WEAPON :
		{
			ULONG idx;
			ULONG errType;
			(*istr) >> idx; 
			(*istr) >> errType;
			LeaseWeaponErrChk(errType);
		}
		break;

		// 공성 아이템 조합 
	case MSG_ITEM_MIX_WARITEM:
		{
			LONG nErrorCode;
			(*istr) >> nErrorCode;	
			pUIManager->GetGWMix()->MixRep( nErrorCode );
		}
		break;

	case MSG_ITEM_LEVELDOWN:
		{
			LONG nErrorCode;
			(*istr) >> nErrorCode;

			pUIManager->CloseMessageBox(MSGCMD_ITEM_LEVELDOWN_REP);
			CUIMsgBox_Info MsgBoxInfo;

			switch (nErrorCode)
			{
			case MSG_ITEM_LEVELDOWN_ERROR_OK:			// 성공
				{
					strTitle = _S(1970, "성공");
					strMessage1 = _S(3982, "아이템의 레벨제한이 감소되었습니다.");
				}
				break;
			case MSG_ITEM_LEVELDOWN_ERROR_ALREADY: // 이미 레벨 하락
				{
					strTitle = _S(1971, "실패");
					strMessage1 = _S(3983, "이미 레벨제한 감소가 적용되어 있습니다." );
				}
				break;
			case MSG_ITEM_LEVELDOWN_ERROR_NOEQUIP: // 장비 아님
				{
					strTitle = _S(1971, "실패");
					strMessage1 = _S(3984, "레벨제한 감소를 적용할 수 있는 장비 아이템이 아닙니다." );
				}
				break;
			case MSG_ITEM_LEVELDOWN_ERROR_SCROLL: // 주문서 아님
				{
					strTitle = _S(1971, "실패");
					strMessage1 = _S(3985, "아이템 레벨제한 감소 주문서가 아닙니다." );
				}
				break;
			}

			MsgBoxInfo.SetMsgBoxInfo( strTitle, UMBS_OK, UI_NONE, MSGCMD_NULL );
			MsgBoxInfo.AddString( strMessage1 );
			pUIManager->CreateMessageBox( MsgBoxInfo );
			pUIManager->GetInventory()->Lock( FALSE, FALSE, LOCK_UPGRADE_ITEM  );
		}
		break;

	case MSG_ITEM_CHECK_COMPOSITION:
		{
			SBYTE sbRow, sbCol;
			
			(*istr) >> sbRow;
			(*istr) >> sbCol;
			
			pUIManager->GetInventory()->UpgradeCompositedItem( (sbRow * INVEN_SLOT_COL) + sbCol );
		}
		break;
	case MSG_ITEM_WEAR_COSTUME_SUIT:

	case MSG_ITEM_WEAR_COSTUME:
	case MSG_ITEM_WEAR_COSTUME_TAKEOFF:
	case MSG_ITEM_WEAR:
	case MSG_ITEM_WEAR_TAKE_OFF:
		ReceiveItemWearError(istr);
		break;
	case MSG_ITEM_WEAR_COSTUME_SUIT_TAKEOFF:
		ReceiveCostSuitTakeOff();
		break;
	case MSG_ITEM_LETSPARTY_USE:
		{
			RecieveUsedPartyItemMessage( istr );
		}
		break;

	case MSG_ITEM_MASTERSTONE_USE:
		{
			RecieveMasterStoneMessage( istr );
		}
		break;
		// [2012/07/05 : Sora]  캐릭터 슬롯 확장 아이템
	case MSG_ITEM_USE_CHAR_SLOT_EXT:
		{
			BYTE result;
			(*istr) >> result;

			if( result == 1 ) // 사용 성공
			{
				pUIManager->GetChattingUI()->AddSysMessage( _S( 5706, "캐릭터 생성 슬롯이 성공적으로 활성화 되었습니다." ) );
			}
			else	// 사용 실패
			{
				pUIManager->GetChattingUI()->AddSysMessage( _S( 5312, "아이템을 사용할 수 없습니다." ) );
			}
		}
		break;
		// [2013/02/13] sykim70 Fortune System
	case MSG_ITEM_COSTUME_SUIT_FORTUNE:
		{
			SLONG ret;
			*istr >> ret;
			if (ret == 0)
			{
				SLONG item_index, skill_index, skill_level;
				*istr >> item_index;
				*istr >> skill_index;
				*istr >> skill_level;
				((CUIFortune*)pUIManager->GetUI(UI_FORTUNE))->SetFortune(item_index, skill_index, skill_level);
			}
			else
			{
				((CUIFortune*)pUIManager->GetUI(UI_FORTUNE))->SetFortune(-1, -1, ret);
			}
		}
		break;

	case MSG_ITEM_EXCHANGE:
		{
			ReceiveItemExchange(istr);
		}
		break;

	case MSG_ITEM_COMPOSE:
		{
			ReceiveItemComposMessage(istr);
		}
		break;
	}
}

void CSessionState::ReceiveAttackMessage(CNetworkMessage *istr)
{
	SBYTE	type;							
	(*istr) >> type;				
	
	CEntity* penPlEntity;
	CPlayerEntity* penPlayerEntity;
	penPlEntity = CEntity::GetPlayerEntity(0); //캐릭터 자기 자신
	penPlayerEntity = static_cast<CPlayerEntity*>(penPlEntity);

	if(type == MSG_CHAR_PC)
	{
		penPlayerEntity->Read_net_Character(MSG_ATTACK, 0, &(*istr));	
	}
	else if(type == MSG_CHAR_NPC)
	{
		penPlayerEntity->Read_net_Mob(MSG_ATTACK, &(*istr));						
	}
	else if(type == MSG_CHAR_PET)
	{
		penPlayerEntity->Read_net_Pet(MSG_ATTACK, &(*istr));						
	}
	else if(type == MSG_CHAR_ELEMENTAL)
	{
		penPlayerEntity->Read_net_Summon(MSG_ATTACK, &(*istr));
	}
}

void CSessionState::ReceiveExchangeMessage(CNetworkMessage *istr)
{
	BYTE	ExchType;
	(*istr) >> ExchType;

	CUIManager* pUIManager = CUIManager::getSingleton();

	if( ExchType == MSG_EXCHANGE_REQ )
	{
		BYTE		bExchReqType;
		(*istr) >> bExchReqType;

		switch( bExchReqType )
		{
			case MSG_EXCHANGE_REQ_REQ:
				{
					LONG		lSrcIndex;
					LONG		lDestIndex;
					CTString	strSrcName;
					CTString	strDestName;
					(*istr) >> lSrcIndex;
					(*istr) >> strSrcName;
					(*istr) >> lDestIndex;
					(*istr) >> strDestName;
					pUIManager->GetExchange()->ExchangeReq_Req( lSrcIndex, strSrcName, lDestIndex, strDestName );
				}
				break;
			case MSG_EXCHANGE_REQ_REP:
				pUIManager->GetExchange()->ExchangeReq_Rep();
				break;
			case MSG_EXCHANGE_REQ_REJECT_SRC:
				pUIManager->GetExchange()->ExchangeReq_Rej( TRUE );
				break;
			case MSG_EXCHANGE_REQ_REJECT_DEST:
				pUIManager->GetExchange()->ExchangeReq_Rej( FALSE );
				break;
			case MSG_EXCHANGE_REQ_READY_SRC:
				pUIManager->GetExchange()->ExchangeReq_Ready( TRUE );
				break;
			case MSG_EXCHANGE_REQ_READY_DEST:
				pUIManager->GetExchange()->ExchangeReq_Ready( FALSE );
				break;
			case MSG_EXCHANGE_REQ_READY:
				pUIManager->GetExchange()->ExchangeReq_BothReady();
				break;
			case MSG_EXCHANGE_REQ_FULL_SRC:
				pUIManager->GetExchange()->ExchangeReq_Full( TRUE );
				break;
			case MSG_EXCHANGE_REQ_FULL_DEST:
				pUIManager->GetExchange()->ExchangeReq_Full( FALSE );
				break;
			case MSG_EXCHANGE_REQ_OK_SRC:
				pUIManager->GetExchange()->ExchangeReq_Ok( TRUE );
				break;
			case MSG_EXCHANGE_REQ_OK_DEST:
				pUIManager->GetExchange()->ExchangeReq_Ok( FALSE );
				break;
			case MSG_EXCHANGE_REQ_OK:
				pUIManager->GetExchange()->ExchangeReq_BothOk();
				break;
			case MSG_EXCHANGE_REQ_SRC_PET_ANYMORE :
				pUIManager->GetExchange()->ExchangeReq_Pet_Err(FALSE);
				break;
			case MSG_EXCHANGE_REQ_DEST_PET_ANYMORE : 
				pUIManager->GetExchange()->ExchangeReq_Pet_Err(TRUE);
				break;

		}
	}
	else if( ExchType == MSG_EXCHANGE_ITEM )
	{
		BYTE	bExchItemType;
		SLONG	slUniIndex;
		SQUAD	llCount;
		SLONG	slIndex;
		SLONG	slPlus;
		SLONG	slFlag;
		SLONG	slUsed;
		SLONG	slUsed2;
		SBYTE	sbOptionCount;
		SBYTE	sbOptionType;
		LONG	lOptionLevel;
#ifdef DURABILITY
		LONG	nDurabilityNow;
		LONG	nDurabilityMax;
#endif // DURABILITY

		(*istr) >> bExchItemType;

		switch( bExchItemType )
		{
		case MSG_EXCHANGE_ITEM_ADD_SRC:
			{
				(*istr) >> slUniIndex;
				(*istr) >> llCount;
				(*istr) >> slIndex;
				(*istr) >> slPlus;
				(*istr) >> slFlag;
				(*istr) >> slUsed;
				(*istr) >> slUsed2;
#ifdef	DURABILITY
				(*istr) >> nDurabilityNow;
				(*istr) >> nDurabilityMax;
#endif
				(*istr) >> sbOptionCount;

				pUIManager->GetExchange()->ExchangeItem_Add( TRUE, slUniIndex, llCount, slIndex, slPlus, slFlag, slUsed, slUsed2 );
#ifdef	DURABILITY
				pUIManager->GetExchange()->SetDurability(TRUE, slUniIndex, nDurabilityNow, nDurabilityMax);
#endif	// DURABILITY

				//레어 아이템일때...
				if( _pNetwork->GetItemData(slIndex)->GetFlag() & ITEM_FLAG_RARE )
				{ 
					if( sbOptionCount >0)
						pUIManager->GetExchange()->ExchangeItem_SetRareOption(istr, TRUE, slUniIndex);	
				}
				//레어 아이템이 아니면.....
				else
				{
					LONG lOriginOptionVar = ORIGIN_VAR_DEFAULT;

					for( SBYTE sbOption = 0; sbOption < sbOptionCount; sbOption++ )
					{
						(*istr) >> sbOptionType;
						(*istr) >> lOptionLevel;

						if ( _pNetwork->GetItemData(slIndex)->GetFlag() & ITEM_FLAG_ORIGIN )
						{
							(*istr) >> lOriginOptionVar;
						}

						pUIManager->GetExchange()->ExchangeItem_SetOptionData( TRUE, slUniIndex, sbOption,
																		sbOptionType, lOptionLevel, 0, lOriginOptionVar );
					}
				}

				if ( _pNetwork->GetItemData(slIndex)->GetFlag() & ITEM_FLAG_ORIGIN)
				{
					pUIManager->GetExchange()->ExchangeItem_SetSkill(istr, TRUE, slUniIndex);
				}
				else
				{
					LONG lPlue2 = 0;
					(*istr) >> lPlue2;
					
					// [2010/12/16 : Sora] 아이템 plus2설정
					pUIManager->GetExchange()->ExchangeItem_SetPlus2( TRUE, slUniIndex, lPlue2 );
				}
				
				pUIManager->GetExchange()->ExchangeItem_InitSocket( TRUE, slUniIndex );
				// data receive about socket type. [6/23/2010 rumist]
				if( _pNetwork->GetItemData(slIndex)->GetFlag() & ITEM_FLAG_SOCKET )
				{
					SBYTE	sbSocketCreateCount = 0;
					SBYTE	sbSocketCount = 0;
					LONG	lSocketJewelIndex = 0;
					LONG	lSocketInfo[JEWEL_MAX_COUNT] = {-1,};
					int	i;
					
					for (i = 0; i < JEWEL_MAX_COUNT; i++)
					{
						(*istr) >> lSocketInfo[i];
						if (lSocketInfo[i] >= 0)
							sbSocketCreateCount++;
					}
					for (i = 0; i < JEWEL_MAX_COUNT; i++)
					{
						pUIManager->GetExchange()->ExchangeItem_SetSocketData( TRUE, slUniIndex, sbSocketCreateCount, 
																			i, lSocketInfo[i] );
					}
				}
			}
			break;

		case MSG_EXCHANGE_ITEM_ADD_DEST:
			{
				(*istr) >> slUniIndex;
				(*istr) >> llCount;
				(*istr) >> slIndex;
				(*istr) >> slPlus;
				(*istr) >> slFlag;
				(*istr) >> slUsed;
				(*istr) >> slUsed2;
#ifdef	DURABILITY
				(*istr) >> nDurabilityNow;
				(*istr) >> nDurabilityMax;
#endif
				(*istr) >> sbOptionCount;

				pUIManager->GetExchange()->ExchangeItem_Add( FALSE, slUniIndex, llCount, slIndex, slPlus, slFlag, slUsed, slUsed2 );
#ifdef	DURABILITY
				pUIManager->GetExchange()->SetDurability(FALSE, slUniIndex, nDurabilityNow, nDurabilityMax);
#endif	// DURABILITY

				//레어 아이템일때...
				if( _pNetwork->GetItemData(slIndex)->GetFlag() & ITEM_FLAG_RARE )
				{
					if( sbOptionCount >0)
						pUIManager->GetExchange()->ExchangeItem_SetRareOption( istr, FALSE, slUniIndex);
				}
				//레어 아이템이 아니면.....
				else
				{
					LONG lOriginOptionVar = ORIGIN_VAR_DEFAULT;

					for( SBYTE sbOption = 0; sbOption < sbOptionCount; sbOption++ )
					{
						(*istr) >> sbOptionType;
						(*istr) >> lOptionLevel;

						if ( _pNetwork->GetItemData(slIndex)->GetFlag() & ITEM_FLAG_ORIGIN )
						{
							(*istr) >> lOriginOptionVar;
						}

						pUIManager->GetExchange()->ExchangeItem_SetOptionData( FALSE, slUniIndex, sbOption,
																		sbOptionType, lOptionLevel, 0, lOriginOptionVar );
					}
				}

				if ( _pNetwork->GetItemData(slIndex)->GetFlag() & ITEM_FLAG_ORIGIN)
				{
					pUIManager->GetExchange()->ExchangeItem_SetSkill(istr, FALSE, slUniIndex);
				}
				else
				{
					LONG lPlue2 = 0;
					(*istr) >> lPlue2;

					// [2010/12/16 : Sora] 아이템 plus2설정
					pUIManager->GetExchange()->ExchangeItem_SetPlus2( FALSE, slUniIndex, lPlue2 );
				}
			
				pUIManager->GetExchange()->ExchangeItem_InitSocket( FALSE, slUniIndex );
				// data receive about socket type. [6/23/2010 rumist]
				if( _pNetwork->GetItemData(slIndex)->GetFlag() & ITEM_FLAG_SOCKET )
				{
					SBYTE	sbSocketCreateCount = 0;
					SBYTE	sbSocketCount = 0;
					LONG	lSocketJewelIndex = 0;
					LONG	lSocketInfo[JEWEL_MAX_COUNT] = {-1,};
					int	i;
					
					for (i = 0; i < JEWEL_MAX_COUNT; i++)
					{
						(*istr) >> lSocketInfo[i];
						if (lSocketInfo[i] >= 0)
							sbSocketCreateCount++;
					}

					for (i = 0; i < JEWEL_MAX_COUNT; i++)
					{
						pUIManager->GetExchange()->ExchangeItem_SetSocketData( FALSE, slUniIndex, sbSocketCreateCount, 
							i, lSocketInfo[i] );
					}
				}
			}
			break;

		case MSG_EXCHANGE_ITEM_DEL_SRC:
			{
				(*istr) >> slUniIndex;
				pUIManager->GetExchange()->ExchangeItem_Del( TRUE, slUniIndex );
			}
			break;

		case MSG_EXCHANGE_ITEM_DEL_DEST:
			{
				(*istr) >> slUniIndex;
				pUIManager->GetExchange()->ExchangeItem_Del( FALSE, slUniIndex );
			}
			break;

		case MSG_EXCHANGE_ITEM_UPDATE_SRC:
			{
				SQUAD llNas;
				(*istr) >> slUniIndex;
				(*istr) >> llCount;
				(*istr) >> llNas;
				pUIManager->GetExchange()->ExchangeItem_Update( TRUE, slUniIndex, llCount ,llNas);
			}
			break;

		case MSG_EXCHANGE_ITEM_UPDATE_DEST:
			{
				SQUAD llNas;
				(*istr) >> slUniIndex;
				(*istr) >> llCount;
				(*istr) >> llNas;
				pUIManager->GetExchange()->ExchangeItem_Update( FALSE, slUniIndex, llCount ,llNas);
			}
			break;
		}
	}
}

void CSessionState::ReceivePartyMessage( CNetworkMessage *istr )
{
	SBYTE	sbPartyType;
	(*istr) >> sbPartyType;

	CUIManager* pUIManager = CUIManager::getSingleton();
	Party* pParty = GAMEDATAMGR()->GetPartyInfo();

	if(pParty == NULL)
		return;
	
	switch( sbPartyType )
	{
	case MSG_PARTY_INVITE:
		{
			SBYTE		sbType;
			SLONG		slSrcIndex;
			CTString	strSrcName;
			SBYTE	sbPartyTypeItem = 0;
			SBYTE	sbPartyTypeSPItem = 0;

			(*istr) >> sbType;
			// [sora] 원정대 시스템 추가로 파티 타입 추가

			(*istr) >> sbPartyTypeItem;
			(*istr) >> sbPartyTypeSPItem;

			(*istr) >> slSrcIndex;
			(*istr) >> strSrcName;

			pParty->PartyInvite( sbType, slSrcIndex, strSrcName, sbPartyTypeItem, sbPartyTypeSPItem );
		}
		break;

	case MSG_PARTY_REJECT_SRC:
		pParty->PartyReject( TRUE );
		break;

	case MSG_PARTY_REJECT_DEST:
		pParty->PartyReject( FALSE );
		break;

	case MSG_PARTY_ADD:
		{
			SBYTE		sbLeader, sbJob, sbJob2, sbLayer;
			SLONG		slIndex, slZone;
			SLONG		swLevel, swHP, swMaxHP, swMP, swMaxMP;
			FLOAT		fX, fZ;
			CTString	strName;

			(*istr) >> sbLeader >> slIndex >> strName >> sbJob >> sbJob2 >> swLevel;
#ifdef HP_PERCENTAGE //  [3/29/2013 Ranma] MSG_PARTY_ADD HP 받는 부분
			FLOAT		fHp_percentage;
			(*istr) >> fHp_percentage;
			swHP = (10*fHp_percentage);
			swMaxHP = 1000;
#else
			(*istr) >> swHP >> swMaxHP;
#endif
			(*istr) >> swMP >> swMaxMP;
			(*istr) >> fX >> fZ >> sbLayer >> slZone;

			pParty->PartyAddMember( sbLeader, slIndex, strName, sbJob, sbJob2, swLevel,
													swHP, swMaxHP, swMP, swMaxMP, fX, fZ, sbLayer, slZone );

			// [100216: selo] 지금은 파티중임을 설정한다.
			pParty->SetIsPartyPlay(TRUE);
		}
		break;

	case MSG_PARTY_QUIT:
		{
			SLONG	slIndex;
			(*istr) >> slIndex;

			pParty->PartyQuit( slIndex );
		}
		break;

	case MSG_PARTY_KICK:
		{
			SLONG	slIndex;
			(*istr) >> slIndex;

			pParty->PartyKick( slIndex );
		}
		break;

	case MSG_PARTY_END:
		{
			// [sora] 헤체 알림 메시지 박스가 있으면 닫는다
			if(pUIManager->DoesMessageBoxExist(MSGCMD_EXPEDITION_ERROR))
				pUIManager->CloseMessageBox(MSGCMD_EXPEDITION_ERROR);

			pParty->PartyEnd();

			if(pUIManager->IsPlayInZone())
			{
				pUIManager->SetCSFlagOn(CSF_TELEPORT);
			}

		}
		break;
	case MSG_PARTY_ENDPARTY_START:	// [sora] 파티장이 파티 해체 요청시 파티원들에게 알림
		{
			CUIMsgBox_Info	MsgBoxInfo;
			CTString strMessage;
			int nCommandCode = MSGCMD_EXPEDITION_ERROR;

			strMessage = _S(4658, "20초후 파티가 해체됩니다.");
			// [sora] 인스턴트 존에서는 부활위치로 이동알림
			if(pUIManager->IsPlayInZone())
			{
				nCommandCode = MSGCMD_EXPEDITION_GO_STARTPLACE;
				strMessage += _S(4659, "인스턴트 존의 부활 위치로 이동합니다.(제한 시간 이후 지동으로 이동합니다.)");
			}

			MsgBoxInfo.SetMsgBoxInfo( _S( 97, "파티" ), UMBS_OK,
					UI_PARTY, nCommandCode );
			
			MsgBoxInfo.AddString( strMessage );
			pUIManager->CreateMessageBox( MsgBoxInfo );
		}
		break;

	case MSG_PARTY_ASSISTINFO:
		break;
	case MSG_PARTY_CHANGEBOSS: // 파티 장 위임 
		{	
			CTString	strBossName;
			CTString	strManadateChaName;
			SLONG		slManadateIndex;
			SBYTE		sbMandate;	// 0: 파티장 탈되, 1:위임
			
			(*istr) >> strBossName;
			(*istr) >> slManadateIndex;
			(*istr) >> strManadateChaName;
			(*istr) >> sbMandate;	
			
			GAMEDATAMGR()->GetPartyInfo()->MandateBoss( strBossName, slManadateIndex, strManadateChaName, sbMandate );

		}
		break;
	case MSG_PARTY_ERROR:
		{
			SBYTE	sbError;
			(*istr) >> sbError;

			pParty->PartyError( sbError );
		}
		break;
	case MSG_PARTY_EMPTY: // 퍼스널 던전 입장시 파티 초기화 [1/11/2007 Theodoric]
		if( pParty->GetMemberCount() )
		{
			pParty->PartyEnd();
		}
		break;
	case MSG_PARTY_CHANGETYPE:
		{
			SBYTE sbPartyType;
			(*istr) >> sbPartyType;
			// [sora] 원정대 시스템 추가로 파티 타입 추가
			SBYTE sbDivisionType;
			SBYTE sbAllOneSet;	//파티타입수정 or 세부항목수정 지정
			
			(*istr) >> sbDivisionType;
			(*istr) >> sbAllOneSet;

			if(sbAllOneSet == MSG_PARTY_SETALLONE_ONE) // 세부항목 수정
			{
				pParty->SetDivisionType(sbPartyType, sbDivisionType);
				break;
			}
			else if(sbAllOneSet == MSG_PARTY_SETALLONE_ALL) // 파티타입 수정
			{
				pParty->SetPartyType(sbPartyType);	
			}

			CTString strMessage, strPartyType;

			switch(sbPartyType)
			{
				case PT_PEACEEVER:
					strPartyType = _S(2660, "균등분배 파티");
					break;
				case PT_SURVIVAL:
					strPartyType = _S(2661, "입수우선 파티");
					break;
				case PT_ATTACK:
					strPartyType = _S(2662, "전투형 파티");
					break;
			}

			strMessage.PrintF(_S(4197, "파티 종류가 %s로 변경되었습니다."), strPartyType);

			pUIManager->GetChattingUI()->AddSysMessage( strMessage );

			//파티 정보창은 업데이트되지 않으므로 닫아준다.
			if(pUIManager->DoesMessageBoxExist(MSGCMD_DIVISION_INFO))
			{
				pUIManager->CloseMessageBox(MSGCMD_DIVISION_INFO);
			}
		}
		break;

	case MSG_PARTY_INZONE_CLEAR_REP:
		{
			CTString strTemp = _S(4660, "인스턴트 존이 초기화 되었습니다.");

			pUIManager->GetChattingUI()->AddSysMessage( strTemp );
		}
		break;

	case MSG_PARTY_TYPEINFO:
		{
			SBYTE sbPartyType, sbDivisionTypeItem, sbDivisionTypeSPItem;

			(*istr) >> sbPartyType;
			(*istr) >> sbDivisionTypeItem;
			(*istr) >> sbDivisionTypeSPItem;

			pParty->SetPartyType(sbPartyType);
			pParty->SetDivisionType(sbDivisionTypeItem, MSG_DIVITYPE_ITEM, TRUE);
			pParty->SetDivisionType(sbDivisionTypeSPItem, MSG_DIVITYPE_SPECIAL, TRUE);
			
			// [091119: selo] 지금은 파티중임을 설정한다.
			pParty->SetIsPartyPlay(TRUE);
		}
		break;
	}
}

void CSessionState::ReceiveQuickSlotMessage( CNetworkMessage *istr )
{
	UBYTE	ubType;//1013
	SBYTE	sbPage, sbSlotType;
	(*istr) >> ubType;
	(*istr) >> sbPage;

	CUIManager* pUIManager = CUIManager::getSingleton();

	switch( ubType )
	{
	case MSG_QUICKSLOT_LIST:
		{
			pUIManager->GetQuickSlot()->ClearAllBtns( sbPage );

			//for( int iSlot = 0; iSlot < 10; iSlot++ )
			for( int iSlot = 0; iSlot < QSLOT_BTN_COUNT; iSlot++ )
			{
				(*istr) >> sbSlotType;
				if (sbSlotType == QUICKSLOT_TYPE_EMPTY)
				{
					pUIManager->GetQuickSlot()->AddBtn( sbPage, iSlot );
				}
				else if (sbSlotType == QUICKSLOT_TYPE_ITEM)
				{
					SWORD	nTab, nIdx;
					(*istr) >> nTab;
					(*istr) >> nIdx;
					
					pUIManager->GetQuickSlot()->AddBtn( sbPage, iSlot, sbSlotType, nTab, nIdx );
				}
				else if(sbSlotType == QUICKSLOT_TYPE_ITEM_WEAR)
				{
					SLONG	nType, nIdx;

					(*istr) >> nType;
					(*istr) >> nIdx;

					pUIManager->GetQuickSlot()->AddBtn( sbPage, iSlot, sbSlotType, nType, nIdx );
				}
				else
				{
					SLONG	slIndex; // Date : 2005-03-02,   By Lee Ki-hwan
					(*istr) >> slIndex;

					pUIManager->GetQuickSlot()->AddBtn( sbPage, iSlot, sbSlotType, slIndex );
				}
			}
			// 큇슬롯 버튼이 셋팅 될때 차일드도 띄울지 말지를 ㄹ결정 한다 [12/21/2012 Ranma]

			((CUIChildQuickSlot*)pUIManager->GetUI(UI_QUICKSLOT2))->FirstOpen();
			((CUIChildQuickSlot*)pUIManager->GetUI(UI_QUICKSLOT3))->FirstOpen();
		}
		break;

	case MSG_QUICKSLOT_ADD:
		{
			SBYTE	sbSlotNum;
			(*istr) >> sbSlotNum;
			(*istr) >> sbSlotType;

			if( sbSlotType == QUICKSLOT_TYPE_EMPTY )
			{
				pUIManager->GetQuickSlot()->AddBtn( sbPage, sbSlotNum );
			}
			else if( sbSlotType == QUICKSLOT_TYPE_ITEM )
			{
				SWORD	nTab, nIdx;
				(*istr) >> nTab;
				(*istr) >> nIdx;
				
				pUIManager->GetQuickSlot()->AddBtn( sbPage, sbSlotNum, sbSlotType, nTab, nIdx );
			}
			else if(sbSlotType == QUICKSLOT_TYPE_ITEM_WEAR)
			{
				SLONG	nType, nIdx;

				(*istr) >> nType;
				(*istr) >> nIdx;

				pUIManager->GetQuickSlot()->AddBtn( sbPage, sbSlotNum, sbSlotType, nType, nIdx );
			}
			else
			{
				SLONG	slIndex;		// Date : 2005-03-02,   By Lee Ki-hwan
				(*istr) >> slIndex;
				if ( slIndex >=  0)
				{
					pUIManager->GetQuickSlot()->AddBtn( sbPage, sbSlotNum, sbSlotType, slIndex );
				}
			}
		}
		break;

	case MSG_QUICKSLOT_SWAP:
		{
			SBYTE	sbSlotNum1, sbSlotNum2;
			(*istr) >> sbSlotNum1;
			(*istr) >> sbSlotNum2;

			pUIManager->GetQuickSlot()->SwapBtns( sbPage, sbSlotNum1, sbSlotNum2 );
		}
		break;
	}
}

void CSessionState::ReceiveSkillMessage( CNetworkMessage *istr )
{
	pTypeBase* pBase = reinterpret_cast<pTypeBase*>(istr->GetBuffer());
	CUIManager* pUIManager = CUIManager::getSingleton();
	
	CEntity* penPlEntity;
	CPlayerEntity* penPlayerEntity;
	penPlEntity = CEntity::GetPlayerEntity(0); //캐릭터 자기 자신
	penPlayerEntity = static_cast<CPlayerEntity*>(penPlEntity);

	switch( pBase->subType )
	{
	case MSG_SKILL_LIST:
		{
			ResponseClient::skillList* pPack = reinterpret_cast<ResponseClient::skillList*>(istr->GetBuffer());
			ResponseClient::skillList::tag_list* pList = NULL;

			pUIManager->GetCharacterInfo()->ClearSkills();
			MY_INFO()->ClearSkill();

			if (pPack->count > 0)
			{
				pList = pPack->list;
			}

			for( int i = 0; i < pPack->count; i++ )
			{
				CSkill	&rSelSkill = _pNetwork->GetSkillData( pList[i].index );				

				if ( rSelSkill.GetType() == CSkill::ST_SEAL )
				{
					if (rSelSkill.Skill_Data.index > 0)
						pUIManager->GetCharacterInfo()->AddSeal( pList[i].index, pList[i].sealExp );
				}
				else if ( rSelSkill.GetType() == CSkill::ST_MELEE || 
					rSelSkill.GetType() == CSkill::ST_RANGE ||
					rSelSkill.GetType() == CSkill::ST_MAGIC ||
					rSelSkill.GetType() == CSkill::ST_SUMMON_TOTEM_SKILL )
				{
					if (pList[i].enable <= 0)
						continue;

					if (rSelSkill.Skill_Data.index > 0)
					{
						MY_INFO()->SetSkill(pPack->list[i].index, (int)pPack->list[i].level);
						pUIManager->GetCharacterInfo()->AddSkill( pPack->list[i].index, pPack->list[i].level);
					}
					
					if ( pList[i].index >= 0 && pList[i].index < _pNetwork->ga_World.wo_aSkillData.Count() )
					{
						CSkill &SkillData = _pNetwork->GetSkillData( pList[i].index );

						if ( pList[i].sendTime <= 0 )
						{
							SkillData.ResetStartTime();
							continue;
						}

						SLONG slRemainTime = ((SLONG)time(NULL) - _pNetwork->slServerTimeGap) - pList[i].sendTime; 
						
						if ( slRemainTime > 0 )
							SkillData.SetStartTime(slRemainTime);
					}
				}
				else
				{
					if (rSelSkill.Skill_Data.index > 0)
					{
						MY_INFO()->SetSkill(pPack->list[i].index, (int)pPack->list[i].level);
						pUIManager->GetCharacterInfo()->AddSkill( pPack->list[i].index, pPack->list[i].level );
					}
				}
			}

			if(pUIManager->GetSkillNew()->IsVisible() == TRUE || pUIManager->GetSkillNew()->GetHide() == FALSE )
				pUIManager->GetSkillNew()->UpdateSkillData();
		}
		break;

	case MSG_SKILL_LEARN:
		{
			ResponseClient::skillLearnMsg* pPack = reinterpret_cast<ResponseClient::skillLearnMsg*>(istr->GetBuffer());
			
			_pNetwork->MyCharacterInfo.sp = pPack->skillPoint;
			pUIManager->GetSkillNew()->LearnSkillMessage(pPack->skillIndex, pPack->skillLevel_flag, pPack->skillLevel);
		}
		break;

	case MSG_SKILL_READY:
		{
			//스킬 시전.		
			penPlayerEntity->Read_net_Character(MSG_SKILL, MSG_SKILL_READY, istr);				
		}
		break;

	case MSG_SKILL_FIRE:
		{
			//스킬 발동.
			//2012/11/09 jeil 스킬 발동시 들어오는 부분 카운터값 여기서 체크?		
			penPlayerEntity->Read_net_Character(MSG_SKILL, MSG_SKILL_FIRE, istr);				
		}
		break;

	case MSG_SKILL_CANCEL:
		{
			penPlayerEntity->Read_net_Character(MSG_SKILL, MSG_SKILL_CANCEL, istr);		//1013	
			//스킬취소.
		}
		break;

	case MSG_SKILL_LEARN_ERROR:
		{
			ResponseClient::skillLearnErrorMsg* pPack = reinterpret_cast<ResponseClient::skillLearnErrorMsg*>(istr->GetBuffer());
			
			pUIManager->GetSkillLearn()->LearnSkillError( pPack->errorCode );
		}
		break;
	case MSG_SKILL_AUTO_USE:
		{
			ResponseClient::skillAutoUseMsg* pPack = reinterpret_cast<ResponseClient::skillAutoUseMsg*>(istr->GetBuffer());
			
			pUIManager->GetCharacterInfo()->UseSkill( pPack->skillIndex );
		}
		break;
	case MSG_SKILL_LEARN_SEAL:
		{
			ResponseClient::sealSkillLearn* pPack = reinterpret_cast<ResponseClient::sealSkillLearn*>(istr->GetBuffer());

			pUIManager->GetCharacterInfo()->AddSeal( pPack->skillIndex, pPack->exp );

			// Create message box of skill learn
			CTString	strMessage;
			CUIMsgBox_Info	MsgBoxInfo;
			MsgBoxInfo.SetMsgBoxInfo( _S( 4490, "증표" ), UMBS_OK, UI_NONE, MSGCMD_NULL);
			strMessage.PrintF( _S( 4661, "증표를 습득하였습니다" ));
			MsgBoxInfo.AddString( strMessage );
			pUIManager->CreateMessageBox( MsgBoxInfo );

		}break;
	case MSG_SKILL_SOUL:
		{
			ResponseClient::skillSoulCountMsg* pPack = reinterpret_cast<ResponseClient::skillSoulCountMsg*>(istr->GetBuffer());
			
			_pNetwork->MyCharacterInfo.sbSoulCount = pPack->soulCount; // 영혼 갯수
		}
		break;
	case MSG_SKILL_MIDLEVEL_SUPPORT:
		{
			ResponseClient::skillSupportMsg* pPack = reinterpret_cast<ResponseClient::skillSupportMsg*>(istr->GetBuffer());
			CTString strMessage;

			switch(pPack->flag)
			{
			case 0:
				strMessage = _S(4954, "레벨이 맞지 않아 마법을 받을 수 없습니다.");
				break;
			case 1:
				strMessage = _S(4955, "마법을 받는데 성공하였습니다.");
				break;
			}

			pUIManager->CloseMessageBox(UI_NONE);
			CUIMsgBox_Info MsgBoxInfo;
			MsgBoxInfo.SetMsgBoxInfo(_S(191, "확인"), UMBS_OK, UI_NONE, MSGCMD_NULL);
			MsgBoxInfo.AddString(strMessage);
			pUIManager->CreateMessageBox(MsgBoxInfo);
		}
		break;

	case MSG_SKILL_ERROR:
		{
			ResponseClient::skillErrorMsg* pPack = reinterpret_cast<ResponseClient::skillErrorMsg*>(istr->GetBuffer());

			if(!(_pNetwork->MyCharacterInfo.eMorphStatus == CNetworkLibrary::MyChaInfo::eMORPH_EVOCATION)
				&& !(_pNetwork->MyCharacterInfo.eMorphStatus == CNetworkLibrary::MyChaInfo::eMORPH_TRANSFORMATION))
			{
				_pNetwork->MyCharacterInfo.eMorphStatus		= CNetworkLibrary::MyChaInfo::eMORPH_END;
			}
				
			switch( pPack->errorCode )
			{
			case MSG_SKILL_ERROR_CANNOT_SPELL:
				pUIManager->GetChattingUI()->AddSysMessage( _S( 1438, "스킬 사용이 불가능한 상태입니다." ), SYSMSG_ERROR );	
				break;
			case MSG_SKILL_ERROR_ALREADY_USE:	// 다른 스킬 사용중
				pUIManager->GetChattingUI()->AddSysMessage( _S( 315, "다른 스킬을 사용중입니다." ), SYSMSG_ERROR );
				break;
			case MSG_SKILL_ERROR_NOTFOUND:		// 그런 스킬 없어요
				pUIManager->GetChattingUI()->AddSysMessage( _S( 316, "존재하지 않는 스킬입니다." ), SYSMSG_ERROR );
				break;
			case MSG_SKILL_ERROR_NOTREADY:		// 스킬 준비 중... 즉, 이전 스킬 시전후		다음 스킬 가능 시간이 안되었을때
				pUIManager->GetChattingUI()->AddSysMessage( _S( 317, "스킬을 준비중입니다." ), SYSMSG_ERROR );
				// NOTE : 스킬 시간이 안됐다구 중지시키면서 스킬을 Cancel하면 안됨.
				break;				
			case MSG_SKILL_ERROR_INVAL_TARGET:	// 타겟이 잘못되었음
				pUIManager->GetChattingUI()->AddSysMessage( _S( 318, "타겟이 잘못되었습니다." ), SYSMSG_ERROR );
				break;
			case MSG_SKILL_ERROR_INSUFF_MP:		// 조건이 안 맞아 사용 못함(MP)
				pUIManager->GetChattingUI()->AddSysMessage( _S( 320, "MP가 부족합니다." ), SYSMSG_ERROR );
				break;
			case MSG_SKILL_ERROR_INSUFF_HP:		// 조건이 안 맞아 사용 못함(HP)
				pUIManager->GetChattingUI()->AddSysMessage( _S( 319, "HP가 부족합니다." ), SYSMSG_ERROR );
				break;
			case MSG_SKILL_ERROR_INSUFF_ITEM:	// 조건이 안 맞아 사용 못함(Item)
				pUIManager->GetChattingUI()->AddSysMessage( _S( 1330, "아이템이 부족합니다." ), SYSMSG_ERROR );
				break;
			case MSG_SKILL_ERROR_LONG:	// 멀어서 안됨
				pUIManager->GetChattingUI()->AddSysMessage( _S( 322, "거리가 멀어서 사용할 수 없습니다." ), SYSMSG_ERROR );
				break;
			case MSG_SKILL_ERROR_INVAL_STATE_DEATH:		// 상태 불만족 : 죽어서만 가능
				pUIManager->GetChattingUI()->AddSysMessage( _S( 390, "죽었을 때만 사용가능 합니다." ), SYSMSG_ERROR );
				break;
			case MSG_SKILL_ERROR_INVAL_STATE_WEAPON:		// 상태 불만족 : 무기 장비시만 가능
				pUIManager->GetChattingUI()->AddSysMessage( _S( 391, "무기를 장착해야 사용가능 합니다." ), SYSMSG_ERROR );
				break;
			case MSG_SKILL_ERROR_INVAL_STATE_SITDOWN:	// 상태 불만족 : 앉아서만 가능
				pUIManager->GetChattingUI()->AddSysMessage( _S( 392, "앉아있을 때만 사용가능 합니다." ), SYSMSG_ERROR );
				break;
			case MSG_SKILL_ERROR_INVAL_STATE_PEACEZONE:	// 상태 불만족 : 피스존 불가능
				pUIManager->GetChattingUI()->AddSysMessage( _S( 393, "평화존에서는 사용 불가능합니다." ), SYSMSG_ERROR );
				break;
			case MSG_SKILL_ERROR_INVAL_STATE_STAND:		// 상태 불만족 : 서서만 가능
				pUIManager->GetChattingUI()->AddSysMessage( _S( 394, "서있을 때만 사용가능 합니다." ), SYSMSG_ERROR );
				break;
			case MSG_SKILL_ERROR_INVAL_STATE_SHIELD:	// 상태 불만족 : 방패 미착용
				pUIManager->GetChattingUI()->AddSysMessage( _S( 1331,  "방패를 착용해야만 사용합니다." ), SYSMSG_ERROR );
				break;
			case MSG_SKILL_ERROR_INVAL_STATE_MAGIC:		// 상태 불만족 : 필요 버프 불충분
				{
					CSkill &skill = _pNetwork->GetSkillData( pPack->skillIndex );
					CTString strMessage;
					switch( pPack->skillIndex )
					{
					case 236: // 하이드 스템 
					case 237: // 하이드 샷 
						strMessage.PrintF(_S( 2063,  "%s을 사용하기 위해서는 [인비저빌리티]스킬이 필요합니다." ), skill.GetName() ); 
						break;
					default :
						strMessage = _S( 1332, "특정 버프 스킬이 필요합니다." );
						break;
					}
					pUIManager->GetChattingUI()->AddSysMessage( strMessage, SYSMSG_ERROR );
					//penPlayerEntity->Read_net_Character_SkillError( MSG_SKILL_ERROR_INVAL_STATE_MAGIC );
				}
				break;
			case MSG_SKILL_ERROR_STATPOINT_STR:			// 힘 부족
				pUIManager->GetChattingUI()->AddSysMessage( _S( 1333, "힘이 부족합니다."), SYSMSG_ERROR );
				break;
			case MSG_SKILL_ERROR_STATPOINT_DEX:			// 민첩 부족
				pUIManager->GetChattingUI()->AddSysMessage( _S( 1334, "민첩이 부족합니다."), SYSMSG_ERROR );
				break;
			case MSG_SKILL_ERROR_STATPOINT_INT:			// 지혜 부족
				pUIManager->GetChattingUI()->AddSysMessage( _S( 1335, "지혜가 부족합니다."), SYSMSG_ERROR );
				break;
			case MSG_SKILL_ERROR_STATPOINT_CON:			// 체질 부족
				pUIManager->GetChattingUI()->AddSysMessage( _S( 1336, "체질이 부족합니다."), SYSMSG_ERROR ); 
				break;
			case MSG_SKILL_ERROR_TARGET_PKLIMITLEVEL: // 15레벨 이하는 pvp를 할 수 없습니다.
				pUIManager->GetChattingUI()->AddSysMessage( _S( 2230,"15레벨 이하의 케릭터는 공격할 수 없습니다." ), SYSMSG_ERROR ); 
				pUIManager->LostTarget();
				break;
			case MSG_SKILL_ERROR_ELEMENTAL_TIME:			// 소환 불가능 : 소환수 사망후 5분동안 소환 불가
				{
					CTString strMessage;
					int iMin, iSec;
					if( pPack->extraInfo == 0 )
					{
						iMin = 0;
						iSec = 1;
					}
					else
					{							
						iMin = pPack->extraInfo / 60;
						iSec = pPack->extraInfo % 60;
					}
					strMessage.PrintF( _S(2360, "[%d분 %d초] 후에 소환수가 소환 가능합니다." ), iMin, iSec );						
					pUIManager->GetChattingUI()->AddSysMessage( strMessage, SYSMSG_ERROR );
				}				
				//pUIManager->GetChattingUI()->AddSysMessage( _S(2361, "소환수가 사망하고 5분동안은 소환이 불가능합니다." ), SYSMSG_ERROR ); 
				//pUIManager->CancelSkill();
				break;
			case MSG_SKILL_ERROR_ELEMENTAL_FIRE:			// 소환 불가능 : 불의정령과 중복 불가
				pUIManager->GetChattingUI()->AddSysMessage( _S(2362, "불의 정령이 이미 소환되어 있거나, 불의 정령과 충돌하여 소환수를 소환할수 없습니다." ), SYSMSG_ERROR ); 
				//pUIManager->CancelSkill();
				break;
			case MSG_SKILL_ERROR_ELEMENTAL_WIND:			// 소환 불가능 : 바람의정령과 중복 불가
				pUIManager->GetChattingUI()->AddSysMessage( _S(2363, "바람의 정령이 이미 소환되어 있거나, 바람의 정령과 충돌하여 소환수를 소환할수 없습니다." ), SYSMSG_ERROR ); 
				//pUIManager->CancelSkill();
				break;
			case MSG_SKILL_ERROR_ELEMENTAL_EARTH:		// 소환 불가능 : 대지의정령과 중복 불가
				pUIManager->GetChattingUI()->AddSysMessage( _S(2364, "대지의 정령이 이미 소환되어 있거나, 대지의 정령과 충돌하여 소환수를 소환할수 없습니다." ), SYSMSG_ERROR ); 
				//pUIManager->CancelSkill();
				break;
			case MSG_SKILL_ERROR_ELEMENTAL_WATER:		// 소환 불가능 : 물의정령과 중복 불가
				pUIManager->GetChattingUI()->AddSysMessage( _S(2365, "물의 정령이 이미 소환되어 있거나, 물의 정령과 충돌하여 소환수를 소환할수 없습니다." ), SYSMSG_ERROR ); 
				//pUIManager->CancelSkill();
				break;
			case MSG_SKILL_ERROR_EVOCATION_ALREADY:
				pUIManager->GetChattingUI()->AddSysMessage( _S(2366, "이미 강신한 상태입니다." ), SYSMSG_ERROR ); 
				//pUIManager->CancelSkill();
				break;
			case MSG_SKILL_ERROR_EVOCATION_TIME:
				{
					CTString strMessage;
					int iMin, iSec;
					if( pPack->extraInfo == 0 )
					{
						iMin = 0;
						iSec = 1;
					}
					else
					{							
						iMin = pPack->extraInfo / 60;
						iSec = pPack->extraInfo % 60;
					}
					strMessage.PrintF( _S(2367, "[%d분 %d초] 후에 강신 가능합니다." ), iMin, iSec );
					pUIManager->GetChattingUI()->AddSysMessage( strMessage, SYSMSG_ERROR );
					_pNetwork->MyCharacterInfo.eMorphStatus		= CNetworkLibrary::MyChaInfo::eMORPH_END;
				}				
				break;
			case MSG_SKILL_ERROR_CANNOT_INVISIBLE:
				{
					pUIManager->GetChattingUI()->AddSysMessage( _S(3167, "몬스터에게 타켓팅이 되어 있을 경우 스킬을 사용할 수 없습니다." ), SYSMSG_ERROR ); 
				}
				break;
			case MSG_SKILL_ERROR_CANNOT_DEATHMOTION_ANOTHER_PC:
				{
					pUIManager->GetChattingUI()->AddSysMessage( _S(3168, "반경 15M이내 당신이 모르는 유저가 존재하여 스킬을 사용할 수 없습니다." ), SYSMSG_ERROR ); 
				}
				break;
			case MSG_SKILL_ERROR_MISS_ROGUE_SKILL:		// 070725_ttos: 와일드 라쏘 스킬(index 125) 실패
				pUIManager->GetChattingUI()->AddSysMessage( _S(3595, "스킬 사용에 실패하였습니다." ), SYSMSG_ERROR ); 
				break;
			
			case MSG_SKILL_ERROR_COMPETITION:
				pUIManager->GetChattingUI()->AddSysMessage( _S(4264, "쿨타임이 적용 중입니다." ), SYSMSG_ERROR ); 
				break;
			case MSG_SKILL_ERROR_INVAL_STATE_DARKNESS:
				pUIManager->GetChattingUI()->AddSysMessage( _S(4686, "사도 모드에서 사용할 수 없는 스킬입니다." ), SYSMSG_ERROR );
				break;
			case MSG_SKILL_ERROR_WARP_CAN_NOT_GO_THERE:
				pUIManager->GetChattingUI()->AddSysMessage(_S(4714,"중간에 가로막히거나 갈 수 없는 곳입니다"), SYSMSG_ERROR);
				break;
			case MSG_SKILL_ERROR_SOUL_COUNT_ALREADY_FULL:
				pUIManager->GetChattingUI()->AddSysMessage(_S(4715,"영혼 소유 개수가 최대입니다."), SYSMSG_ERROR);
				break;
			case MSG_SKILL_ERROR_CANNOT_TO_PC:
				pUIManager->GetChattingUI()->AddSysMessage(_S(4716,"PC에게 사용 불가능 합니다.(PK 때도 불가능)"), SYSMSG_ERROR);
				break;
			case MSG_SKILL_ERROR_DONT_HAS_SOUL:
				pUIManager->GetChattingUI()->AddSysMessage(_S(4717,"이미 영혼이 흡수된 대상이라 시전이 불가능 합니다. "), SYSMSG_ERROR);
				break;
			case MSG_SKILL_ERROR_INVAL_STATE_PSERSONAL_DUNGEON:
				pUIManager->GetChattingUI()->AddSysMessage(_S(5183,"싱글 던젼에서 사용할 수 없는 스킬입니다."), SYSMSG_ERROR);
				break;
			case MSG_SKILL_ERROR_DUPLICATE:
				pUIManager->GetChattingUI()->AddSysMessage(_S(556,"이미 사용중인 아이템 입니다."), SYSMSG_ERROR);
				break;
			case MSG_SKILL_ERROR_CANNOT_ZONE:
				pUIManager->GetChattingUI()->AddSysMessage( _S( 5413, "현재 존에서는 사용할 수 없습니다."  ), SYSMSG_ERROR );
				break;
			}

			// NOTE : 스킬 스펠하고 사용을 못하면 락이 걸리기 때문에...
			pUIManager->CancelSkill(FALSE, TRUE);
			
			pUIManager->SetCSFlagOff(CSF_SKILLREADY);
			
		}
		break;

	case MSG_SKILL_RESET_COOLTIME:
		{
			_pNetwork->ResetSkillDelayTime(); //스킬 딜레이 타임 초기화
			pUIManager->GetQuickSlot()->ResetQuickSLotSKill(); // 퀵슬롯에서 스킬 제거			
		}
		break;
	case MSG_SKILL_TOGGLE:
		{
			ReceiveToggleMessage(istr);
		}
		break;
	}
}

// 생산 관련 특수 스킬...(Passive Skill임)
//-----------------------------------------------------------------------------
// Purpose:
// Input  : *istr - 
//-----------------------------------------------------------------------------
void CSessionState::ReceiveSSkillMessage(CNetworkMessage *istr)
{
	pTypeBase* pBase = reinterpret_cast<pTypeBase*>(istr->GetBuffer());
	CUIManager* pUIManager = CUIManager::getSingleton();

	switch(pBase->subType)
	{
	case MSG_SSKILL_LIST:
		{
			ResponseClient::sskillList* pPack = reinterpret_cast<ResponseClient::sskillList*>(istr->GetBuffer());
			pUIManager->GetCharacterInfo()->ClearSSkills();
			MY_INFO()->ClearSSkill();
									
			for( int iSkill = 0; iSkill < pPack->listCount; iSkill++ )
			{
				MY_INFO()->SetSkill(pPack->list[iSkill].skillIndex, pPack->list[iSkill].skillLevel, true);
				pUIManager->GetCharacterInfo()->AddSkill( pPack->list[iSkill].skillIndex, pPack->list[iSkill].skillLevel, TRUE );
			}
		}
		break;
	case MSG_SSKILL_LEARN:
		{
			ResponseClient::sskillLearn* pPack = reinterpret_cast<ResponseClient::sskillLearn*>(istr->GetBuffer());
			
			MY_INFO()->SetSkill(pPack->skillIndex, pPack->skillLevel, true);
			pUIManager->GetSkillLearn()->LearnSkill( pPack->skillIndex, pPack->flag, pPack->skillLevel, TRUE );
			pUIManager->GetCharacterInfo()->SetSSkill();
		}
		break;
	case MSG_SSKILL_LEARN_ERROR:		// 에러 발생.
		{
			ResponseClient::sskillLearnError* pPack = reinterpret_cast<ResponseClient::sskillLearnError*>(istr->GetBuffer());

			pUIManager->GetSkillLearn()->LearnSSkillError( pPack->errorCode );			
		}
		break;
	}
}

void CSessionState::ReceiveStatMessage( CNetworkMessage *istr )
{
	UBYTE	ubType;//1013
	SLONG   swRemainedStat;
	(*istr) >> ubType;

	CUIManager* pUIManager = CUIManager::getSingleton();

	if( ubType == MSG_STATPOINT_REMAIN )
	{
		(*istr) >> swRemainedStat;

		_UIAutoHelp->SetStatpoint(_pNetwork->MyCharacterInfo.StatPoint, swRemainedStat);
		
		_pNetwork->MyCharacterInfo.StatPoint = swRemainedStat;
		pUIManager->GetCharacterInfo()->NotifyStatPoint();
	}
	else if( ubType == MSG_STATPOINT_USE )
	{
		UBYTE	ubStatType;
		SLONG	slStatChange;
		(*istr) >> ubStatType;
		(*istr) >> slStatChange;
		(*istr) >> swRemainedStat;

		_pNetwork->MyCharacterInfo.StatPoint = swRemainedStat;
		pUIManager->GetCharacterInfo()->UseStatPoint( ubStatType, slStatChange );
	}
	else if( ubType == MSG_STATPOINT_ERROR )
	{
		SBYTE	sbError;
		(*istr) >> sbError;

		pUIManager->GetCharacterInfo()->StatPointError( sbError );
	}
	// 스탯 초기화시.
	else if( ubType == MSG_STATPOINT_RESET )
	{
		LONG	lStr;
		LONG	lDex;
		LONG	lInt;
		LONG	lCon;
		int		iTotalStat = 0;

		(*istr) >> lStr;
		(*istr) >> lDex;
		(*istr) >> lInt;
		(*istr) >> lCon;
		iTotalStat += lStr;
		iTotalStat += lDex;
		iTotalStat += lInt;
		iTotalStat += lCon;

		CTString strSysMessage;
		strSysMessage.PrintF( _S( 1337, "총 %d 스탯이 초기화되었습니다." ), iTotalStat );		
		pUIManager->GetChattingUI()->AddSysMessage( strSysMessage, SYSMSG_NORMAL );
		pUIManager->GetInitJob()->CloseInitJob();
	}
}

void CSessionState::ReceiveUIMessage( CNetworkMessage *istr )
{
	UBYTE	ubType;
	(*istr) >> ubType;

	if( ubType == MSG_UI_PLUS_EFFECT_REP )
	{
		SBYTE	sbItemPlusEffect;
		SLONG	slCharIndex;

		(*istr) >> sbItemPlusEffect;
		(*istr) >> slCharIndex;

		if( slCharIndex == _pNetwork->MyCharacterInfo.index )
		{
			_pNetwork->MyCharacterInfo.sbItemEffectOption = sbItemPlusEffect;
		}
		else
		{
			ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_CHARACTER, slCharIndex);

			if (pObject != NULL)
			{
				CCharacterTarget* pTarget = static_cast< CCharacterTarget* >(pObject);

				pTarget->SetItemEffectOption( sbItemPlusEffect );
				pTarget->cha_itemEffect.Refresh(&(pTarget->m_pEntity->GetModelInstance()->m_tmSkaTagManager), sbItemPlusEffect );
				}
			}
		}
}

void CSessionState::ReceiveSystemMessage( CNetworkMessage *istr )
{
	SBYTE		sbType;
	SBYTE		sbTab;	// 1013
	CTString	strSysMessage;

	(*istr) >> sbType;

	CEntity			*penPlEntity;
	CPlayerEntity	*penPlayerEntity;
	penPlEntity = CEntity::GetPlayerEntity(0);		// 캐릭터 자기 자신
	penPlayerEntity = static_cast<CPlayerEntity *>(penPlEntity);

	CUIManager* pUIManager = CUIManager::getSingleton();

	switch( sbType )
	{
		case MSG_SYS_WHISPER_NOTFOUND:	// 귓말 대상 없음	:
			pUIManager->GetChattingUI()->AddSysMessage( _S( 304, "귓속말 상대를 찾을 수 없습니다." ), SYSMSG_ERROR );
			break;

		case MSG_SYS_TIMEOUT:			// 타임 아웃 발생	:
			strSysMessage = "MSG_SYS_TIMEOUT";
			pUIManager->GetChattingUI()->AddSysMessage( strSysMessage, SYSMSG_ERROR );
			break;

		case MSG_SYS_CANNOT_WEAR:		// 착용 오류		:
			{
				pUIManager->GetChattingUI()->AddSysMessage( _S( 305, "장비를 착용할 수 없습니다." ), SYSMSG_ERROR );

				if ( CUIManager::getSingleton()->IsCSFlagOn( CSF_ITEMWEARING ) )
				{
					// 장비 착용 시도 상태 해제
					CUIManager::getSingleton()->SetCSFlagOffElapsed(CSF_ITEMWEARING);
				}
			}			
			break;

		case MSG_SYS_FULL_INVENTORY:	// 인벤토리 가득 참	: tab_index(uc)
			(*istr) >> sbTab;//1013
			pUIManager->GetChattingUI()->AddSysMessage( _S( 265, "인벤토리 공간이 부족합니다." ), SYSMSG_ERROR );
			break;

		case MSG_SYS_CUSTOM:			// 커스텀 메시지	: msg(str)
			{
				CTString	strMsg;//1013
				(*istr) >> strMsg;//1013
				strSysMessage = "MSG_SYS_CUSTOM";
				pUIManager->GetChattingUI()->AddSysMessage( strSysMessage, SYSMSG_ERROR );
			}
			break;

		case MSG_SYS_SHORT_MONEY:		// 돈 사용시 모질람	:
			{
				pUIManager->GetChattingUI()->AddSysMessage( _S( 306, "나스가 부족합니다." ), SYSMSG_ERROR );

				// 에게하 이동중에 올 수 있는 메세지이다. (유럽 버전은 사용료 부과)
				pUIManager->SetCSFlagOff(CSF_TELEPORT);
			}
			break;

		case MSG_SYS_SHOP_CANCEL:		// 상점 거래 취소	:
			pUIManager->GetChattingUI()->AddSysMessage( _S( 307, "상점 거래를 취소하셨습니다." ), SYSMSG_ERROR );
			break;

		case MSG_SYS_EXCHANGE_NOTFOUND:	// 교환 대상 없음	:
			pUIManager->GetChattingUI()->AddSysMessage( _S( 308, "교환 상대를 찾을 수 없습니다." ), SYSMSG_ERROR );
			
			pUIManager->CloseMessageBox( MSGCMD_EXCH_REQ_SRC );
		    if (pUIManager->IsCSFlagOn( CSF_EXCHANGE ))
			{
				pUIManager->SetCSFlagOff( CSF_EXCHANGE );
			}
			break;

		case MSG_SYS_EXCHANGE_ALREADY:	// 이미 교환 중		: 
			pUIManager->GetChattingUI()->AddSysMessage( _S( 309, "이미 교환중입니다." ), SYSMSG_ERROR );
			
			pUIManager->CloseMessageBox( MSGCMD_EXCH_REQ_SRC );
		    if (pUIManager->IsCSFlagOn( CSF_EXCHANGE ))
			{
				pUIManager->SetCSFlagOff( CSF_EXCHANGE );
			}
			break;

		/*
		case MSG_SYS_SKILL_ALREADY_USE:	// 다른 스킬 사용중
			pUIManager->GetChattingUI()->AddSysMessage( _S( 315, "다른 스킬을 사용중입니다." ), SYSMSG_ERROR );
			penPlayerEntity->Read_net_Character( MSG_SYS_SKILL_ALREADY_USE, NULL );
			break;
		
		case MSG_SYS_SKILL_NOTFOUND:		// 그런 스킬 없어요
			pUIManager->GetChattingUI()->AddSysMessage( _S( 316, "존재하지 않는 스킬입니다." ), SYSMSG_ERROR );
			penPlayerEntity->Read_net_Character( MSG_SYS_SKILL_NOTFOUND, NULL );
			break;

		case MSG_SYS_SKILL_NOTREADY:		// 스킬 준비 중... 즉, 이전 스킬 시전후 다음 스킬 가능 시간이 안되었을때
			pUIManager->GetChattingUI()->AddSysMessage( _S( 317, "스킬을 준비중입니다." ), SYSMSG_ERROR );
			penPlayerEntity->Read_net_Character( MSG_SYS_SKILL_NOTREADY, NULL );
			break;

		case MSG_SYS_SKILL_INVAL_TARGET:	// 타겟이 잘못되었음
			pUIManager->GetChattingUI()->AddSysMessage( _S( 318, "타겟이 잘못되었습니다." ), SYSMSG_ERROR );
			penPlayerEntity->Read_net_Character( MSG_SYS_SKILL_INVAL_TARGET, NULL );
			break;
		
		case MSG_SYS_SKILL_INSUFF:		// 조건이 안 맞아 사용 못함(HP, MP, 아이템 부족)
			{
				SBYTE	sbErrType;
				(*istr) >> sbErrType;

				switch( sbErrType )
				{
				case 0:		// HP
					strSysMessage = _S( 319, "HP가 부족합니다." );
					break;
				case 1:		// MP
					strSysMessage = _S( 320, "MP가 부족합니다." );
					break;
				case 2:		// Item
					strSysMessage = _S( 321, "답글" );
					break;
				}
				pUIManager->GetChattingUI()->AddSysMessage( strSysMessage, SYSMSG_ERROR );
				penPlayerEntity->Read_net_Character( MSG_SYS_SKILL_INSUFF, NULL );
			}
			break;

		case MSG_SYS_SKILL_LONG:			// 멀어서 안됨
			pUIManager->GetChattingUI()->AddSysMessage( _S( 322, "거리가 멀어서 사용할 수 없습니다." ), SYSMSG_ERROR );
			penPlayerEntity->Read_net_Character( MSG_SYS_SKILL_LONG, NULL );
			break;
*/
		case MSG_SYS_NOT_OWNER_ITEM:		// 아이템 우선권이 없음
			pUIManager->GetChattingUI()->AddSysMessage( _S( 323, "아이템 권한이 없습니다." ), SYSMSG_ERROR );
			break;
		case MSG_SYS_UPGRADE_NOCONDITION:	// 조건이 안맞아 업그레이드 못함 (일반제련석 레벨이 맞지 않음)
			pUIManager->GetChattingUI()->AddSysMessage( _S( 328, "레벨이 맞지 않아 업그레이드 할 수 없습니다." ), SYSMSG_ERROR );			
			pUIManager->GetInventory()->Lock( FALSE, FALSE, LOCK_UPGRADE_ITEM  );
			break;

		case MSG_SYS_MAKE_REFINE_KIND:		// 무기나 방어구가 아닌 다른걸 제련석으로 전환하려할 때
			pUIManager->GetChattingUI()->AddSysMessage( _S( 329, "무기와 방어구만 업그레이드 할 수 있습니다." ), SYSMSG_ERROR );
			break;

		case MSG_SYS_MAKE_REFINE_WEARING:	// 입고 있는 아이템으로 제련석 전환 불가능
			pUIManager->GetChattingUI()->AddSysMessage( _S( 330, "착용된 아이템은 제련석으로 전환이 불가능합니다." ), SYSMSG_ERROR );
			break;

		case MSG_SYS_MAKE_REFINE_PLUS:		// Plus 값이 셋팅되어 있는 아이템 제련석 전환 불가능
			pUIManager->GetChattingUI()->AddSysMessage( _S( 331, "업그레이드 된 아이템은 제련석으로 전환이 불가능합니다." ), SYSMSG_ERROR );
			break;

		case MSG_SYS_MEMPOS_CANT_WRITE:		// 장소 기억 장치 사용 불가능 존
			pUIManager->GetChattingUI()->AddSysMessage( _S( 333, "메모리 스크롤을 사용할 수 없는 지역입니다." ), SYSMSG_ERROR );
			break;

		case MSG_SYS_LAYERDUNGEON_FULL:		// 레이어 존 가득 차서 입장 불가
			strSysMessage = _S( 1338, "레이어 존이 가득 차서 입장 할 수 없습니다." );		
			pUIManager->GetChattingUI()->AddSysMessage( strSysMessage, SYSMSG_ERROR );
			break;

		case MSG_SYS_CANT_WARP_STATE:
			pUIManager->GetChattingUI()->AddSysMessage( _S( 415, "이동할 수 없는 상태입니다." ), SYSMSG_ERROR );
			pUIManager->GetChattingUI()->AddSysMessage( _S( 340, "이동이 취소되었습니다." ), SYSMSG_ERROR );
			pUIManager->SetCSFlagOff(CSF_TELEPORT);
			break;

		case MSG_SYS_CANT_WARP_OTHER:		// 다른 이동 장치 사용중으로 이동 장치 사용 불가
			pUIManager->GetChattingUI()->AddSysMessage( _S( 334, "다른 이동 장치를 사용중입니다." ), SYSMSG_ERROR );
			break;

		case MSG_SYS_MEMPOS_OTHERZONE:		// 다른 존으로는 장소 기억 이동 불가
		case MSG_SYS_MEMPOS_CASTLE:
			pUIManager->GetChattingUI()->AddSysMessage( _S( 335, "메모리 스크롤로 이동할 수 있는 지역이 아닙니다." ), SYSMSG_ERROR );
			break;

		case MSG_SYS_SINGLEDUNGEON_FULL:	// 싱글 던젼이 꽉 차있음.
			strSysMessage = _S( 1339, "싱글 던젼 인원 초과로 입장 할 수 없습니다." );			
			pUIManager->GetChattingUI()->AddSysMessage( strSysMessage, SYSMSG_ERROR );
			break;
/*
		case MSG_SYS_SKILL_INVAL_STATE:		// 상태 불만족			: flag(c: 0 - 죽었을때만 가능, 1 - 무기 장비시만 가능, 2 - 앉아서만 가능, 3 - 피스존 불가능, 4 - 서서만 가능
			{
				SBYTE	sbErrType;
				(*istr) >> sbErrType;

				switch( sbErrType )
				{
					case 0:		
						strSysMessage = _S( 390, "죽었을 때만 사용가능 합니다." );
						break;
					case 1:		
						strSysMessage = _S( 391, "무기를 장착해야 사용가능 합니다." );
						break;
					case 2:	
						strSysMessage = _S( 392, "앉아있을 때만 사용가능 합니다." );
						break;
					case 3:		
						strSysMessage = _S( 393, "평화존에서는 사용 불가능합니다." );
						break;
					case 4:		
						strSysMessage = _S( 394, "서있을 때만 사용가능 합니다." );
						break;
				}
				
				pUIManager->GetChattingUI()->AddSysMessage( strSysMessage, SYSMSG_ERROR );
			}
			break;
*/
		case MSG_SYS_MAKE_REFINE_SHORT:		// 제련석 만들때 돈 모질람
			pUIManager->GetChattingUI()->AddSysMessage( _S( 369, "제련 비용이 부족합니다." ), SYSMSG_ERROR );
			break;
		case MSG_SYS_PRODUCE_NOTOOL:			// 생산도구가 없음
			pUIManager->GetChattingUI()->AddSysMessage( _S( 657, "생산에 필요한 도구가 없습니다." ), SYSMSG_ERROR );		
			((CPlayerEntity*)CEntity::GetPlayerEntity(0))->CancelProduct();
			break;
		case MSG_SYS_PRODUCE_WEAR_TOOL:		// 생산도구를 착용하시오
			pUIManager->GetChattingUI()->AddSysMessage( _S( 658, "생산 도구를 착용하시오." ), SYSMSG_ERROR );		
			((CPlayerEntity*)CEntity::GetPlayerEntity(0))->CancelProduct();
			break;
		case MSG_SYS_PRODUCE_MATCH_TOOL:		// 맞는 생산도구를 착용하시오
			pUIManager->GetChattingUI()->AddSysMessage( _S( 659, "적절하지 않은 생산 도구입니다." ), SYSMSG_ERROR );		
			((CPlayerEntity*)CEntity::GetPlayerEntity(0))->CancelProduct();
			break;
		case MSG_SYS_PRODUCE_MAXUSE_TOOL:	// 생산도구 다 썼음
			pUIManager->GetChattingUI()->AddSysMessage( _S( 660, "생산 도구가 낡아서 파괴되었습니다." ), SYSMSG_ERROR );		
			((CPlayerEntity*)CEntity::GetPlayerEntity(0))->CancelProduct();
			break;
		case MSG_SYS_PRODUCE_MAX_ACCEPTABLE:	// 생산 가능 인원 초과
			pUIManager->GetChattingUI()->AddSysMessage( _S( 661, "생산 가능 인원을 초과했습니다." ), SYSMSG_ERROR );		
			((CPlayerEntity*)CEntity::GetPlayerEntity(0))->CancelProduct();
			break;
		case MSG_SYS_PRODUCE_NO_SSKILL:			// 해당하는 생산 스킬이 없다!!
			pUIManager->GetChattingUI()->AddSysMessage( _S( 662, "해당하는 생산 스킬이 없습니다." ), SYSMSG_ERROR );		
			((CPlayerEntity*)CEntity::GetPlayerEntity(0))->CancelProduct();
			break;
		case MSG_SYS_PROCESS_DOC_NOT_SSKILL:// 가공문서 쓰는데 가공스킬이 없음
			pUIManager->GetChattingUI()->AddSysMessage( _S( 663, "생산품을 가공하기 위한 가공스킬이 없습니다" ), SYSMSG_ERROR );		
			((CPlayerEntity*)CEntity::GetPlayerEntity(0))->CancelProduct();
			break;
		case MSG_SYS_BLOODITEM:
			{
				SLONG	itemindex;
				(*istr) >> itemindex;
				
				//CItemData &ItemData	= _pNetwork->GetItemData(itemindex);
				//CTString strSysMessage;
				strSysMessage.PrintF( _S( 703, "%s의 아이템에 블러드 옵션을 사용할 수 있습니다." ), _pNetwork->GetItemName(itemindex) );
				_pNetwork->ClientSystemMessage( strSysMessage, SYSMSG_ERROR );
				
				//pUIManager->GetChattingUI()->AddSysMessage( CTString(_S( 665, "블러드 옵션을 사용할수 있습니다." )) );		
				break;
			}
		case MSG_SYS_MAKE_REFINE_CANNOT:
			pUIManager->GetChattingUI()->AddSysMessage( _S( 666, "해당 아이템은 제련석으로 교환할 수 없는 아이템입니다." ), SYSMSG_ERROR );	
			break;

		case MSG_SYS_PKMODE_LIMITLEVEL:		// PK 선언이 불가능한 레벨
			pUIManager->GetChattingUI()->AddSysMessage(  _S( 806, "15레벨 이하는 PVP를 할 수가 없습니다." ) , SYSMSG_ERROR );	
			break;

		case MSG_SYS_CANTCHAT_INVISIBLE:		// [2012/02/03 : Sora] ITS 6649 인비저블 상태에서는 일반 채팅 불가 메시지 처리 추가
			pUIManager->GetChattingUI()->AddSysMessage(  _S( 5666, "일반 채팅을 하실 수 없는 상태입니다." ) , SYSMSG_ERROR );	
			break;

		case MSG_SYS_SHORT_FAME:
			pUIManager->GetChattingUI()->AddSysMessage( _S( 1340, "명성치가 부족합니다." ), SYSMSG_ERROR );		
			break;

		case MSG_SYS_FRIEND_NOTFOUND:
			{
				// 현재 활성화 되어 있는 메제 창으로 에러 메세지를 보내자 
				pUIManager->GetMessenger()->TalkErrorMessage(_S( 2064,  "대화상대가 오프라인이여서 메세지를 전달할 수 없습니다." ) );	
			}
			break;
		//TEMP: 초고급 제련석
		case MSG_SYS_UPGRADE_CANT_SUPERSTONE:
			{
				strSysMessage = _S( 1746, "초 고급 제련석을 이미 사용한 장비입니다. 다른 아이템에 사용해 주십시요." );
				CUIMsgBox_Info	MsgBoxInfo;
				MsgBoxInfo.SetMsgBoxInfo(  _S( 1673, "제련 불가!!!" ), UMBS_OK, UI_SHOP, MSGCMD_SUPERGOJE_NOTIFY);
				MsgBoxInfo.AddString( strSysMessage );
				pUIManager->CreateMessageBox( MsgBoxInfo );
				if(pUIManager->GetInventory()->IsLocked()) //wooss 051024
					pUIManager->GetInventory()->Lock( FALSE, FALSE, LOCK_UPGRADE_ITEM  );
			} break;
		case MSG_SYS_UPGRADE_CANT_14LEVEL:
			{
				strSysMessage = _S( 1747, "초 고급 제련석은 +14장비에는 사용할 수 없습니다." );
				CUIMsgBox_Info	MsgBoxInfo;
				MsgBoxInfo.SetMsgBoxInfo(  _S( 1670, "주의!!!" ), UMBS_OK, UI_SHOP, MSGCMD_SUPERGOJE_NOTIFY);
				MsgBoxInfo.AddString( strSysMessage );
				pUIManager->CreateMessageBox( MsgBoxInfo );
				if(pUIManager->GetInventory()->IsLocked()) //wooss 051024
					pUIManager->GetInventory()->Lock( FALSE, FALSE, LOCK_UPGRADE_ITEM  );
			} break;
		case MSG_SYS_CAN_RECEIVE_SUPERSTONE:
			{
				SLONG fame;
				(*istr) >> fame;
				strSysMessage.PrintF(_S( 1749, "축하합니다.\n명성치가 %d이 되었습니다.\n마법 잡화상 코엔을 통해\n초 고급 제련석을 지급받을 수\n있습니다." ), fame);
				CUIMsgBox_Info	MsgBoxInfo;
				MsgBoxInfo.SetMsgBoxInfo(  _S( 1748, "안내" ), UMBS_OK, UI_SHOP, MSGCMD_SUPERGOJE_NOTIFY);
				MsgBoxInfo.AddString( strSysMessage );
				pUIManager->CreateMessageBox( MsgBoxInfo );
			} break;
		case MSG_SYS_FAME_NOT_ENOUGH:
			{
				strSysMessage = _S( 1750, "명성치가 부족하여 초 고급 제련석을 지급 받을 수 없습니다." );
				pUIManager->GetChattingUI()->AddSysMessage( strSysMessage, SYSMSG_ERROR );
			} break;
		case MSG_SYS_INVEN_NOT_ENOUGH:
			{
				strSysMessage = _S( 1751, "사용중인 인벤토리에 빈공간이 부족하여 초 고급 제련석을 지급할 수 없습니다. 인벤토리를 비워 주십시오." );
				pUIManager->GetChattingUI()->AddSysMessage( strSysMessage, SYSMSG_ERROR );
			} break;
		case MSG_SYS_NO_MORE_SUPERSTONE:
			{
				strSysMessage = _S( 1752, "초 고급 제련석을 모두 지급 받아 더이상 지급 받을 수 없습니다." );
				pUIManager->GetChattingUI()->AddSysMessage( strSysMessage, SYSMSG_ERROR );
			} break;
		case MSG_SYS_APPEAR_NOVICE:
			{
				CTString strName;
				(*istr) >> strName;
				strSysMessage.PrintF(_S( 1753, "신규 유저 %s 님이 지금 게임에 접속했습니다. 후견인 제도를 통한 많은 지원 부탁드립니다." ), strName.str_String);
				pUIManager->GetChattingUI()->AddSysMessage( strSysMessage, SYSMSG_NOTIFY );
			} break;
		case MSG_SYS_NOTICE:
			{
				LONG lIndex;
				(*istr) >> lIndex;
				
				Notice* pNotice = GAMEDATAMGR()->GetNotice();

				if (pNotice != NULL)
					pNotice->AddToNoticeList(2000 + lIndex, Notice::NOTICE_EVENT);

			} break;
		
		case MSG_SYS_PET_CANT_SUMMON:		// 소환 불가능 지역
			{
				strSysMessage = _S( 2130, "소환이 불가능한 지역입니다." );	
				pUIManager->GetChattingUI()->AddSysMessage( strSysMessage, SYSMSG_ERROR );
			}
			break;

		case MSG_SYS_PET_CANT_MOUNT:		// 마운트 불가능 지역
			{
				strSysMessage = _S( 2065, "마운트가 불가능한 지역입니다." );
				pUIManager->GetChattingUI()->AddSysMessage( strSysMessage, SYSMSG_ERROR );
				pUIManager->SetCSFlagOff(CSF_PETRIDING);	// [2011/03/21 : Sora] 마운트 실패시 풀래그 off
			}
			break;
		case MSG_SYS_PRODUCE_NO_MONEY:		// 돈없어서 생산 불가
			{
				strSysMessage = _S( 2066, "나스가 부족하여 생산이 불가능합니다. " );	
				pUIManager->GetChattingUI()->AddSysMessage( strSysMessage, SYSMSG_ERROR );
			}
			break;
		case MSG_SYS_PRODUCE_CANNT_BOOST:	// 부스터로는 랜덤 생산 불가
			{
				pUIManager->CancelSkill();
				pUIManager->CloseMessageBox(MSGCMD_BOOST_RANDON_PRODUCT_ERROR);
				((CPlayerEntity*)CEntity::GetPlayerEntity(0))->CancelProduct();
				strSysMessage = _S( 2067, "부스터를 착용한 장비로는 선택 생산만 가능합니다." );	
						
				CUIMsgBox_Info	MsgBoxInfo;
				MsgBoxInfo.SetMsgBoxInfo( _S(2068, "생산" ), UMBS_OK, NULL, MSGCMD_BOOST_RANDON_PRODUCT_ERROR );
				MsgBoxInfo.AddString( strSysMessage );
				pUIManager->CreateMessageBox( MsgBoxInfo );

			}
			break;
		case MSG_SYS_CANNOT_DUPLICATION:	// 부스터로는 랜덤 생산 불가
			{
				SLONG	nNewItmeIndex;
				SLONG	nCurItemIndex;

				(*istr) >> nNewItmeIndex;
				(*istr) >> nCurItemIndex;
				
				CTString strNewItmeName;
				CTString strCurItmeName;
							
				
				strNewItmeName = _pNetwork->GetItemName( nNewItmeIndex );
				strCurItmeName = _pNetwork->GetItemName( nCurItemIndex );
				
				if( nNewItmeIndex == nCurItemIndex )
				{
					strSysMessage.PrintF( _S2(2069,strNewItmeName,"%s<는> 중복하여 사용할 수 없습니다."), strNewItmeName ); 
				}
				else
				{
					strSysMessage.PrintF( _S3(2070,strNewItmeName,strCurItmeName,"%s<는> %s<과> 중복하여 사용할 수 없습니다."), strNewItmeName, strCurItmeName ); 
				}
				pUIManager->GetChattingUI()->AddSysMessage( strSysMessage, SYSMSG_ERROR );
			}
			break;		
		case MSG_SYS_UPGRADE_CANT_6LEVEL_LUCKY :	// 행운의 제력석 레벨 에러 wooss 051010
			{
				strSysMessage = _S(2151,"행운의 제련석을 사용할 수 없습니다."); 
				CUIMsgBox_Info	MsgBoxInfo;
				MsgBoxInfo.SetMsgBoxInfo(  _S( 1673, "제련 불가!!!" ), UMBS_OK, UI_SHOP, MSGCMD_SUPERGOJE_NOTIFY);
				MsgBoxInfo.AddString( strSysMessage );
				pUIManager->CreateMessageBox( MsgBoxInfo );
				if(pUIManager->GetInventory()->IsLocked() || pUIManager->GetInventory()->IsLockedArrange()) //wooss 051024
					pUIManager->GetInventory()->Lock( FALSE, FALSE, LOCK_UPGRADE_ITEM  );
			}
			break;
			
	case MSG_SYS_TARGET_PKLIMITLEVEL:
			{
				pUIManager->LostTarget();

				strSysMessage = _S(2231, "15레벨 이하의 케릭터는 공격할 수 없습니다." );
				pUIManager->GetChattingUI()->AddSysMessage( strSysMessage, SYSMSG_ERROR );
			}
			break;
	case MSG_SYS_NOT_ADULT : 
		{
			ULONG remTime;
			(*istr)>>remTime;
			CTString strSysMessage;
			strSysMessage.PrintF(_S(2261,"%d분후에 접속이 끊깁니다. 내일 오전 6시 이후에 접속가능합니다."), remTime);
			pUIManager->GetChattingUI()->AddSysMessage( strSysMessage, SYSMSG_ERROR );
		}
		break;

	case MSG_SYS_NOTTIME_GOZONE :
		{
			CTString strSysMessage=_S(2279, "이동 가능한 시간이 아닙니다."); // wooss 051212 번역 
			pUIManager->GetChattingUI()->AddSysMessage( strSysMessage, SYSMSG_ERROR );
		}
		break;

	case MSG_SYS_CANNOT_WEAR_DEADPET:
		{
			// MSG_SYS_CANNOT_WEAR_DEADPET,	// 060221 : bs : 사망한 펫 착용 불가 : petindex(n) remainRebirth(n)
			SLONG	remainRebirth;
			SLONG	lPetIndex;
			(*istr) >> lPetIndex;
			(*istr) >> remainRebirth;
			CTString strSysMessage;
			strSysMessage.PrintF(_S(2508,"펫 봉인해제 시간이 %d시간 %d분 남았습니다."), remainRebirth/3600, (remainRebirth%3600)/60 );
			pUIManager->GetChattingUI()->AddSysMessage( strSysMessage, SYSMSG_ERROR );
		}
		break;
		
	case MSG_SYS_ITEMCOMPOSITED :			// 아이템 합성 성공
		{
			CTString strSysMessage=_S( 2730, "아이템 합성이 성공하였습니다."); // wooss 051212 번역 
			pUIManager->GetChattingUI()->AddSysMessage( strSysMessage, SYSMSG_ERROR );
			pUIManager->GetMixNew()->CloseMixNew();
		}
		break;
	
	case MSG_SYS_EXPIRE_ITEMCOMPOSITE :		// 아이템 합성 만료 : itemdbindex(n)
		{
			CTString strSysMessage;
			LONG tv_idx;
			(*istr) >> tv_idx;
		
			strSysMessage.PrintF( _S( 3041, "[%s]아이템 합성 사용이 만료 되었습니다." ),
				CItemData::getData(tv_idx)->GetName()); // wooss 051212 번역 ")
			pUIManager->GetChattingUI()->AddSysMessage( strSysMessage, SYSMSG_ERROR );
			pUIManager->GetMixNew()->CloseMixNew();
		}
		break;
	case MSG_SYS_CANNOT_RESET_MOUNT:	// 펫 마운트 타입 취소 불가
		{
			CTString strSysMessage = _S( 2900, "펫의 마운트 훈련을 취소 할 수 없습니다." );
			pUIManager->GetChattingUI()->AddSysMessage( strSysMessage, SYSMSG_ERROR );
		}	
		break;
	case MSG_SYS_RESET_MOUNT:	// 펫 마운트 타입 취소
		{
			CTString strSysMessage = _S( 2901, "애완동물의 상태 변환이 완료 되었습니다." );
			pUIManager->GetChattingUI()->AddSysMessage( strSysMessage, SYSMSG_ERROR );

			pUIManager->CloseMessageBox(MSGCMD_PET_MOUNT_CANCEL_CARD);
		}
		break;
	case MSG_SYS_CANNOT_SEPARATE_ITEM:
		{
			CTString strSysMessage = _S(2934, "능력치가 결합된 유료 장비 아이템만 결합 해제가 가능합니다." );
			pUIManager->GetChattingUI()->AddSysMessage( strSysMessage, SYSMSG_ERROR );
			pUIManager->GetMixNew()->CloseMixNew();
		}	
		break;
	case MSG_SYS_SEPARATE_ITEM :
		{
			LONG tv_idx;
			(*istr) >> tv_idx;
			CTString strSysMessage;
			strSysMessage.PrintF( _S(2935, "결합된 장비[%s] 분리가 성공하였습니다."), _pNetwork->GetItemName(tv_idx));
			pUIManager->GetChattingUI()->AddSysMessage( strSysMessage, SYSMSG_ERROR );
			pUIManager->GetMixNew()->CloseMixNew();
		}
		break;

	case MSG_SYS_RAIDMOB_REGEN:
		{
			LONG tv_idx;
			(*istr) >> tv_idx;
			
			CTString strNoticeMsg;
			strNoticeMsg.PrintF("[ %s %s ]", CMobData::getData(tv_idx)->GetName(), _S(3158, "몬스터가 출현하였습니다.") );
			
			_UIAutoHelp->SetGMNotice( strNoticeMsg, 0xFF5014FF);
			
		}
		break;
	case MSG_SYS_TIMEOUT_LUCKY_ACCESSORY:
		{
			LONG tv_idx;
			(*istr) >> tv_idx;
			CTString strSysMessage;
			strSysMessage.PrintF( _S(3188, "%s의 사용기간이 만료되어 아이템이 사라집니다."), _pNetwork->GetItemName(tv_idx));
			pUIManager->GetChattingUI()->AddSysMessage( strSysMessage, SYSMSG_ERROR );
			pUIManager->GetMixNew()->CloseMixNew();
		}
		break;
	case MSG_SYS_CHANGE_LUCKY_BOX:
		{ 
			/***
			LONG tv_idx;
			(*istr) >> tv_idx;
			CTString strSysMessage;
			strSysMessage.PrintF( _S(3187, "럭키드로우상자에서 %s 아이템이 나왔습니다."), _pNetwork->GetItemName(tv_idx));
			pUIManager->GetChattingUI()->AddSysMessage( strSysMessage, SYSMSG_ERROR );
			pUIManager->GetMixNew()->CloseMixNew();
			***/
			LONG iReward, iUsedItem;

			(*istr) >> iReward;
			(*istr) >> iUsedItem;

			CTString strSysMessage;

			if (iReward == 0)
			{
				strSysMessage.PrintF(_s("아이리스 메달을 획득하였습니다"));
			}
			else
			{
				if( iUsedItem==0 || iUsedItem==2037 || iUsedItem==2597 )
					strSysMessage.PrintF( _S(3187, "럭키드로우상자에서 %s 아이템이 나왔습니다."), _pNetwork->GetItemName(iReward) );
				else
					strSysMessage.PrintF( _S(4198, "%s에서 %s 아이템이 나왔습니다."), _pNetwork->GetItemName(iUsedItem), _pNetwork->GetItemName(iReward));
			}
	
			pUIManager->GetChattingUI()->AddSysMessage( strSysMessage, SYSMSG_ERROR );

			pUIManager->GetMixNew()->CloseMixNew();
		}
		break;
	case MSG_SYS_CANNOT_HAVE_DUPLICATE:
		{
			CTString strSysMessage = _S(3218, "중복해서 소유할 수 없는 아이템 입니다." );
			pUIManager->GetChattingUI()->AddSysMessage( strSysMessage, SYSMSG_ERROR );
		}
		break;
	// wooss 070307----------------------------------------------------------------->>
	// kw : WSS_WHITEDAY_2007
	case MSG_SYS_MATCHSEX:
		{
			CTString strSysMessage = _S( 3256, "성별이 맞지 않습니다." );
			pUIManager->GetChattingUI()->AddSysMessage( strSysMessage, SYSMSG_ERROR );
		}
		break;
	// -----------------------------------------------------------------------------<<
	case MSG_SYS_NO_AUTO_ITEM:
		{
			CTString strSysMessage = _S( 3666, "더 이상 사용할 물약이 없습니다." );
			pUIManager->GetChattingUI()->AddSysMessage( strSysMessage, SYSMSG_ERROR );
		}
		break;
		
	case MSG_SYS_EGEHA_MINLEVEL:
		{
			CTString strSysMessage = _S( 2712, "레벨 제한 조건을 만족하지 못합니다" );
			pUIManager->GetChattingUI()->AddSysMessage( strSysMessage, SYSMSG_ERROR );

		}
		break;
	case MSG_SYS_IRISPOINT_ALARM:
		{
			ULONG iCnt;
			(*istr) >> iCnt;

			strSysMessage.PrintF(_S( 4400, "아이리스 메달을 %d 개 획득하였습니다."), iCnt);
			pUIManager->GetChattingUI()->AddSysMessage( strSysMessage, SYSMSG_ERROR );
		}
		break;
	case MSG_SYS_CHANGE_RAID_BOX:	// [sora] 레이드 상자 열기
		{
			LONG iReward, iUsedItem;
			CTString strSysMessage;

			(*istr) >> iReward;
			(*istr) >> iUsedItem;

			if(iReward >= 0)
			{
				strSysMessage.PrintF( _S(4198, "%s에서 %s 아이템이 나왔습니다."), _pNetwork->GetItemName(iUsedItem), _pNetwork->GetItemName(iReward));
			}
			else	// iReward == -1이면 빈상자
			{
				strSysMessage = _S(4662, "상자안에는 아무것도 들어있지 않았습니다.");
			}
			
			pUIManager->GetChattingUI()->AddSysMessage( strSysMessage, SYSMSG_ERROR );
		}
		break;
	case MSG_SYS_FACTORY_EXP:
		{
			SQUAD lExp;
			(*istr) >> lExp;
			
			strSysMessage.PrintF(_S( 4663, "숙련도가 %I64d 상승하였습니다."), lExp);
			pUIManager->GetChattingUI()->AddSysMessage( strSysMessage, SYSMSG_NOTIFY );
		}break;
	case MSG_SYS_CANNOT_DESTROY:
		{
			strSysMessage.PrintF(_S( 4696, "파괴가 불가능한 아이템입니다." ));
			pUIManager->GetChattingUI()->AddSysMessage( strSysMessage, SYSMSG_ERROR );
		}break;
	case MSG_SYS_TITLE_EXPIRED:
		{
			INDEX iCount, iIndex, iItemIndex;

			(*istr) >> iCount;
			
			for (int i = 0 ; i < iCount ; ++i)
			{
				(*istr) >> iIndex;
				//  [3/25/2010 kiny8216] 호칭 이름을 아이템 lod에서 불러오도록 변경
				iItemIndex = CNickNameData::getData(iIndex)->GetItemIndex();
				strSysMessage.PrintF(_S(4838, "%s 호칭의 기간이 만료 되었습니다." ), pUIManager->GetNickName()->GetName(iItemIndex));
				pUIManager->GetChattingUI()->AddSysMessage( strSysMessage, SYSMSG_NOTIFY );
			}
		}break;
	case MSG_SYS_IRISPOINT_DROPMEDAL:
		{
			strSysMessage.PrintF(_s("1시간이 경과되어 아이리스 메달이 드랍됩니다."));
			pUIManager->GetChattingUI()->AddSysMessage( strSysMessage, SYSMSG_ERROR );
		}
		break;
	case MSG_SYS_CANNOT_GOZONE_LEVEL:
		{
			strSysMessage.PrintF(_S(4850, "레벨이 맞지 않아 이동이 불가능합니다."));
			pUIManager->GetChattingUI()->AddSysMessage( strSysMessage, SYSMSG_ERROR );
			pUIManager->SetCSFlagOff(CSF_TELEPORT);
		}break;
	case MSG_SYS_COSTUME2_ITEM_EXPIRED:
		{
			INDEX costume2_index;
			(*istr) >> costume2_index;
			strSysMessage.PrintF( _S(3188, "%s의 사용기간이 만료되어 아이템이 사라집니다."), _pNetwork->GetItemName(costume2_index));
			pUIManager->GetChattingUI()->AddSysMessage( strSysMessage, SYSMSG_ERROR );
		}
		break;
	case MSG_SYS_TRANSLATE_START:		// 펫변신시작
		{
			strSysMessage.PrintF(_S(5210, "잠시 후 변신합니다."));
			pUIManager->GetChattingUI()->AddSysMessage( strSysMessage, SYSMSG_ERROR );
		}break;
	case MSG_SYS_CANT_MOUNT_EVOCATION: // 강신 중 마운트 불가능
		{
			strSysMessage.PrintF(_S(2580, "강신중일때는 애완동물을 탈 수 없습니다."));
			pUIManager->GetChattingUI()->AddSysMessage( strSysMessage, SYSMSG_ERROR );
			pUIManager->SetCSFlagOff(CSF_PETRIDING);
		}
		break;
	case MSG_SYS_CANT_MAKE_PET_EVOCATION: // 강신중 펫 탈것 만들기 못함
		{
			strSysMessage.PrintF(_S(5179, "강신 중에는 이용할 수 없습니다."));
			pUIManager->GetChattingUI()->AddSysMessage( strSysMessage, SYSMSG_ERROR );
		}
		break;
	case MSG_SYS_FULL_INVENTORY_AND_DROP:
		{
			pUIManager->CloseMessageBox( MSGCMD_NULL );
			
			CUIMsgBox_Info	MsgBoxInfo;
			MsgBoxInfo.SetMsgBoxInfo(   _S( 191, "확인" ), UMBS_OK, UI_NONE, MSGCMD_NULL);
			MsgBoxInfo.AddString( _S( 5367, "인벤토리가 부족하여 아이템이 바닥에 떨어집니다. " ) );
			MsgBoxInfo.AddString( _S( 5368, "인벤토리를 비운 후 습득 하시길 바랍니다." ) );
			pUIManager->CreateMessageBox( MsgBoxInfo );
		}
		break;
	case MSG_SYS_ENTER_RAID_ZONE_REMAIN:
		{
			SLONG remainTime;
			
			(*istr) >> remainTime;

			strSysMessage.PrintF( _S( 5369, "%d분 뒤 레이드 던전이 초기화 됩니다." ), remainTime );
			pUIManager->GetChattingUI()->AddSysMessage( strSysMessage, SYSMSG_ERROR );
		}
		break;
	case MSG_SYS_RAID_RESET_COMPLETE:
		{
			strSysMessage.PrintF( _S( 5370, "레이드 던전 초기화가 완료되었습니다." ) );
			pUIManager->GetChattingUI()->AddSysMessage( strSysMessage, SYSMSG_ERROR );
		}
		break;
	case MSG_SYS_NO_ENTER_RAID_ZONE:
		{
			strSysMessage.PrintF( _S( 5371, "레이드 던전 초기화 중입니다." ) );
			pUIManager->GetChattingUI()->AddSysMessage( strSysMessage, SYSMSG_ERROR );
		}
		break;
	case MSG_SYS_GMCOMMAND_NOTICE:
		{
			UBYTE		ubSubType;
			CTString	strMessage;
			CTString	strGM, strUser;
			(*istr) >> ubSubType
					>> strGM
					>> strUser;
			switch( ubSubType )
			{
				case MSG_GM_COMMAND_NOTICE_KICK:
					strMessage.PrintF( _S( 5447, "%s 님이 %s 유저를 게임에서 추방하였습니다." ), strGM, strUser );
					_UIAutoHelp->SetGMNotice(strMessage);
					break;

				case MSG_GM_COMMAND_NOTICE_SILENCE:
					strMessage.PrintF( _S( 5448, "%s 님이 %s 유저를 채팅금지 하였습니다." ), strGM, strUser );
					_UIAutoHelp->SetGMNotice(strMessage);
					break;

				default: break;
			}
		}
		break;
		// BUG FIX : LC-MX-20110506-001 [5/18/2011 rumist]
	case MSG_SYS_PET_CANT_MOUNT_ETC:
		{
			pUIManager->SetCSFlagOff(CSF_PETRIDING);
			pUIManager->GetChattingUI()->AddSysMessage( _S( 5489, "펫을 탑승할 수 없습니다." ), SYSMSG_ERROR );
		}
		break;
	case MSG_SYS_DUP_PREFINE_COMPOSITE:
		{
			pUIManager->GetChattingUI()->AddSysMessage( _S( 5668, "플래티늄 제련석과 결합주문서는 중복하여 사용 하실 수 없습니다." ), SYSMSG_ERROR );
			pUIManager->GetInventory()->Lock( FALSE, FALSE, LOCK_UPGRADE_ITEM  );
		}
		break;
	// 유료아이템 NPC포탈 스크롤 아이템 사용기간 만료 [8/22/2012 Ranma] NEW_NPC_PORTAL_SCROLL_ABS
	case MSG_SYS_NPC_PORTAL_EXPIRED:
		{
			CTString strName, strmassage;
			INDEX nItem_Index;
			strName = _pNetwork->MyCharacterInfo.GetPlayerName();
			
			(*istr) >> nItem_Index;
			
			strmassage.PrintF(_S(4213, "%s 의 %s 아이템이 기간 만료로 사라집니다." ),strName,_pNetwork->GetItemName(nItem_Index));
			pUIManager->GetChattingUI()->AddSysMessage( strmassage, SYSMSG_NORMAL );
		}
		break;
	case MSG_SYS_NOT_ENOUGH_MONEY:
		{
			pUIManager->GetChattingUI()->AddSysMessage( _S( 5902, "현재 가지고 있는 나스 보다 많은 나스를 교환 할 수 없습니다." ), SYSMSG_ERROR );
		}
		break;
	case MSG_SYS_DO_NOT_ATTACK_IMMOTAL:
		pUIManager->GetChattingUI()->AddSysMessage( 
			_S(6080, "대상은 면역 상태입니다. 공격 할 수 없습니다."),
			SYSMSG_ERROR );
		break;
	case MSG_SYS_DO_NOT_ATTACK_DO_MOVE:
		pUIManager->GetChattingUI()->AddSysMessage( 
			_S(6079, "무적 모드에서는 공격 및 스킬 사용이 불가능합니다. 무적 공격 모드로 전환하시려면 캐릭터를 움직이세요."),
			SYSMSG_ERROR );
		break;
	case MSG_SYS_DO_NOT_CHANGE_PK_MODE:
			pUIManager->GetChattingUI()->AddSysMessage( _S( 5596, "분쟁 지역에서는 PVP모드로 전활할 수 없습니다." ), SYSMSG_ERROR );		
		break;
	case MSG_SYS_DO_NOT_SYSTEM_NOT_EQ_RVRTYPE:
			pUIManager->GetChattingUI()->AddSysMessage( _S( 6090, "분쟁 지역에서 적대 관계인 경우 사용할 수 없는 기능입니다."), SYSMSG_ERROR);
		break;
	case MSG_SYS_DO_NOT_PARTY_THISZONE:
			pUIManager->GetChattingUI()->AddSysMessage( _S( 6112, "분쟁 지역에 진입하여 파티 및 원정대에서 탈퇴 되었습니다."), SYSMSG_ERROR);
			break;
	case MSG_SYS_DO_NOT_USE_DURING_IMMORTAL:
			pUIManager->GetChattingUI()->AddSysMessage(_S(6286, "무적 버프가 존재하여 사용할 수 없습니다."), SYSMSG_ERROR);
		break;
	case MSG_SYS_DO_NOT_USE_ITEM_STATE:
		pUIManager->GetChattingUI()->AddSysMessage(_S(5312, "아이템을 사용할 수 없습니다."), SYSMSG_ERROR);
		break;
	case MSG_SYS_DO_NOT_GO_ZONE_GMORDER:
			pUIManager->GetChattingUI()->AddSysMessage(_S(384, "이동할 수 없습니다."), SYSMSG_ERROR);
			pUIManager->SetCSFlagOff(CSF_TELEPORT);
		break;
	case MSG_SYS_DO_NOT_PLAY_PVP:		// PVP를 할 수 없는 상태입니다.
		pUIManager->GetChattingUI()->AddSysMessage(_S(6347, "PVP를 할 수 없는 상태입니다."), SYSMSG_ERROR);
		break;
	case MSG_SYS_DO_NOT_ATTACK_PROTECT_PVP: // PVP보호 상태이므로 공격할 수 없습니다.
		pUIManager->GetChattingUI()->AddSysMessage(_S(6349, "PVP보호 상태이므로 공격할 수 없습니다."), SYSMSG_ERROR);
		break;
	case MSG_SYS_PVP_PROTECT_ITEM_END:	// PVP보호 방패의 사용기간이 만료되어 PVP가 가능한 상태가 되었습니다.
		pUIManager->GetChattingUI()->AddSysMessage(_S(6350, "PVP보호 방패의 사용기간이 만료되어 PVP가 가능한 상태가 되었습니다."), SYSMSG_ERROR);
		break;
	case MSG_SYS_AREADY_USE: //	이미 사용중 입니다.
		pUIManager->GetChattingUI()->AddSysMessage(_S(556, "이미 사용중인 아이템 입니다."), SYSMSG_ERROR);
		break;
	case MSG_SYS_WAR_BUF_NOT_IN_ZONE: // 공성 지역을 벗어나 버프 효과가 사라집니다.
		pUIManager->GetChattingUI()->AddSysMessage(_S(6421, "공성 지역을 벗어나서 버프 효과가 사라집니다. 다시 공성 지역으로 이동 할 경우 버프 효과가 정상적으로 적용 됩니다."), SYSMSG_ERROR);
		break;
	case MSG_SYS_WAR_BUF_END_WAR: // 공성 시간이 종료되어 버프 효과가 사라짐
		pUIManager->GetChattingUI()->AddSysMessage(_S(6420, "공성 시간이 종료 되어 버프 효과가 사라집니다."), SYSMSG_ERROR);
		break;
	case MSG_SYS_ARTIFACT_DONT_IN_DUNGEON: //유물 아이템을 보유한 상태로 던전을 입장 할 수 없습니다.
		pUIManager->SetCSFlagOff(CSF_TELEPORT);
		pUIManager->GetItemCompose()->ShowErrorPopup(eCOMPOSE_POPUP_DONT_IN_DUNGEON);
		break;
	case MSG_SYS_ARTIFACT_PVP_GET_ITEM:	// PVP가 불가능한 캐릭터는 유물 아이템을 획득 할 수 없습니다.
		pUIManager->GetItemCompose()->ShowErrorPopup(eCOMPOSE_POPUP_HAVE_NOT_RELIC);
		break;
	case MSG_SYS_ARTIFACT_ITEM_DELETE_EVENT_OFF:
		pUIManager->GetChattingUI()->AddSysMessage(_S(6443, "유물 사냥꾼 이벤트가 종료되어 모든 유물 아이템이 사라집니다."), SYSMSG_ERROR);
		break;
	case MSG_SYS_ARTIFACT_ITEM_DO_NOT_USE_PVPPROTECT:
		pUIManager->GetChattingUI()->AddSysMessage(_S(6447, "유물 아이템을 가지고 있는 상태에서는 PVP보호방패를 사용할 수 없습니다."), SYSMSG_ERROR);
		break;
	case MSG_SYS_ARTIFACT_USE_PVPPROTECT:
		pUIManager->GetChattingUI()->AddSysMessage(_S(6448, "PVP보호방패를 사용 중 입니다."), SYSMSG_ERROR);
		break;
	case MSG_SYS_SUMMON_TOTEM_NPC: // 토템 아이템이 소환 되었다.
		pUIManager->GetChattingUI()->AddSysMessage(_S(6426, "토템이 소환 되었습니다. 토템과 일정 거리 이상 벗어나거나 HP 및 토템 시간이 0이 되면 토템이 사라지게 됩니다."), SYSMSG_ERROR);
		break;
	case MSG_SYS_NOT_CONDITION_ITEM_ALL_USER:
		pUIManager->GetChattingUI()->AddSysMessage(_S(6446, "아이템을 획득할 수 있는 유저가 없습니다."), SYSMSG_ERROR);
		break;
	case MSG_SYS_BELONG_ITEM:
		pUIManager->GetChattingUI()->AddSysMessage(_S(6453, "아이템이 캐릭터에 귀속되었으므로 이동할 수 없습니다."), SYSMSG_ERROR);
		break;
	default:
		{
			strSysMessage = "Undefined Message..";
			pUIManager->GetChattingUI()->AddSysMessage( strSysMessage, SYSMSG_ERROR );
		}										
		break;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     
	}

}

void CSessionState::ReceiveRebirthMessage( CNetworkMessage *istr )
{	
/*
	SLONG  cha_index
	SWORD  hp,maxHp,mp,maxMp;
		
	(*istr) >> cha_index;
	(*istr) >> hp;
	(*istr) >> maxHp;
	(*istr) >> mp;
	(*istr) >> maxMp;
	
	if(cha_index == _pNetwork->MyCharacterInfo.index)
	{
		_pNetwork->MyCharacterInfo.hp = hp; 
		_pNetwork->MyCharacterInfo.maxHP = maxHP;
		_pNetwork->MyCharacterInfo.mp = mp; 
		_pNetwork->MyCharacterInfo.maxMP = maxMP; 
		((CPlayerEntity*)CEntity::GetPlayerEntity(0))->Rebirth();
		return;
	}
*/
	((CPlayerEntity*)CEntity::GetPlayerEntity(0))->Read_net_Character(MSG_PC_REBIRTH, 0, &(*istr));

}


void CSessionState::ReceiveEffectMessage( CNetworkMessage *istr )
{
	//COMMON
	SBYTE	sbType;
	//SKILL
	SLONG	slSkillId;
	SBYTE	sbTargetType;
	SLONG	slTargetId;
	//ETC
	SBYTE	sbEtcType;
	SBYTE	sbChaType;
	SLONG	slChaIndex;	
		
	(*istr) >> sbType;

	switch(sbType)
	{
	case MSG_EFFECT_SKILL:
		{
			(*istr) >> slSkillId;
			(*istr) >> sbTargetType;
			(*istr) >> slTargetId;

			INDEX	iWeapon = 0;
			CEntity	*penTarget = NULL;
			if( sbTargetType == MSG_CHAR_PC )
			{
				if( _pNetwork->MyCharacterInfo.index == slTargetId )	//내 캐릭이면
				{
					penTarget = CEntity::GetPlayerEntity(0);
					iWeapon = _pNetwork->MyCharacterInfo.bExtension;
				}
				else													//내 캐릭이 아니면
				{
					ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_CHARACTER, slTargetId);

					if (pObject != NULL)
					{
						CCharacterTarget* pTarget = static_cast< CCharacterTarget* >(pObject);

						penTarget = pTarget->GetEntity();
						iWeapon = pTarget->cha_bExtension;
						}
					}

				CSkill &skill = _pNetwork->GetSkillData( slSkillId );
				INDEX iTempAnim = _pNetwork->MyCharacterInfo.bExtension;
				INDEX iSkillAni1 = skill.idPlayer_Anim_Skill[iTempAnim][0];
				INDEX iSkillAni2 = skill.idPlayer_Anim_Skill[iTempAnim][2];
				
				if( slSkillId == 33	//Hardcording, 파티힐만, 파티클 이펙트가 잘 안나올 경우 INDEX를 의심할 것.
				 || (slSkillId == 114 && skill.GetMissileType( 0 ) == 21)//Hardcoding, baal skill 1
				 || slSkillId == 115 //Hardcoding, baal skill 2
				 || slSkillId == 149 //Hardcoding, death knight skill 1
				 || slSkillId == 150 //Hardcoding, death knight skill 2
				 || slSkillId == 169 //Hardcoding, knight telekinesis
				 || slSkillId == 226 //Hardcoding, mage flame storm
				 || skill.GetTargetType() == CSkill::STT_PARTY_ALL // 파티원 들에게 들어갈 효과
				 || skill.GetTargetType() == CSkill::STT_GUILD_ALL // 길드원 들에게 들어갈 효과
				 || (skill.GetFlag() & SF_GUILD && iSkillAni1==-1 && iSkillAni2==-1) // 애니메이션이 없는 길드 스킬
				 || (skill.GetFlag() & SF_SUMMON_NPC)
				 )
				{
					PCStartEffectGroup( skill.GetFireEffect3( iWeapon ), slTargetId, penTarget );
				}
			}
			// Date : 2005-10-28(오후 1:42:53), By Lee Ki-hwan
			// 몬스터가 몬스터에게 이펙트를 사용할때는 예외처리됨
			// 타격 이팩트 처리 됨
			else if( sbTargetType == MSG_CHAR_NPC )
			{
				if( slSkillId == 289 || slSkillId == 290 || _pNetwork->GetSkillData( slSkillId ).GetFlag() & SF_GUILD || _pNetwork->GetSkillData( slSkillId ).GetFlag() & SF_SUMMON_NPC )
				{
					CPrintF( "Target = NPC, Skill ID : %d, TargetID: %d\n", slSkillId, slTargetId );

					ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_MOB, slTargetId);

					if (pObject != NULL)
					{
						penTarget = pObject->GetEntity();
						iWeapon = 0;
					}
				
					CSkill &skill = _pNetwork->GetSkillData( slSkillId );
					PCStartEffectGroup( skill.GetFireEffect3( iWeapon ), slTargetId, penTarget );
				}
			}
		} break;
	case MSG_EFFECT_ETC:
		{
			(*istr) >> sbChaType;
			(*istr) >> slChaIndex;
			(*istr) >> sbEtcType;
			
			CPrintF("ChaType:%d, slChaIndex:%d, sbEtcType:%d\n", sbChaType,slChaIndex,sbEtcType);

			switch(sbEtcType)
			{
			case MSG_EFFECT_ETC_LEVELUP: // 레벨업
				{
					if(sbChaType != MSG_CHAR_PC) break;	//현재 몹의 레벨없은 없음.

					PCStartEffectGroup("LevelUp", slChaIndex);		//Hardcording

					_UIAutoHelp->SetInfo ( AU_LEVEL_UP );
				//	_UIAutoHelp->SetInfo ( AU_LEVEL_UP_QUEST2 );
					_UIAutoHelp->SetInfo ( AU_LEVEL_UP_QUEST8 );
					_UIAutoHelp->SetInfo ( AU_LEVEL_UP_QUEST9 );
					_UIAutoHelp->SetInfo ( AU_LEVEL_UP_QUEST10 );
					_UIAutoHelp->SetInfo ( AU_LEVEL_UP_QUEST16 );
					_UIAutoHelp->SetInfo ( AU_LEVEL_UP_QUEST24 );
				}
				break;
			case MSG_EFFECT_ETC_GOZONE: // GO_World
				{
					if(sbChaType != MSG_CHAR_PC) break;	//현재 몹의 존이동은 없음.
										
					PCStartEffectGroup("Teleport End", slChaIndex);		//Hardcording
				}
				break;
			case MSG_EFFECT_ETC_PRODUCE_MINING:
			case MSG_EFFECT_ETC_PRODUCE_GATHERING:
			case MSG_EFFECT_ETC_PRODUCE_CHARGE:
			case MSG_EFFECT_ETC_ITEM_PROCESS:
			case MSG_EFFECT_ETC_ITEM_MAKE:
				{
					((CPlayerEntity*)CEntity::GetPlayerEntity(0))->PlayItemEffect(-1, sbEtcType);
				}
				break;
			case MSG_EFFECT_ETC_HP_STEEL: // HP 스틸 포션 이펙트
				{
					if(sbChaType != MSG_CHAR_PC) break;
										
					PCStartEffectGroup("HP_STEAL", slChaIndex);
				}
				break;
			case MSG_EFFECT_ETC_MP_STEEL: // MP 스틸 포션 이펙트
				{
					if(sbChaType != MSG_CHAR_PC) break;
										
					PCStartEffectGroup("MP_STEAL", slChaIndex);
				}
				break;
			case MSG_EFFECT_ETC_FIRECRACKER: // 폭죽 아이템을 사용했을 때 나오는 이펙트 // [070824: Su-won] ITEM_FIRECRACKER
				{
					if(sbChaType != MSG_CHAR_PC) break;
										
					PCStartEffectGroup("squid", slChaIndex);
				}
				break;
			case MGS_EFFECT_ETC_COMPETITION:
				{
					if(sbChaType != MSG_CHAR_PC) break;
					PCStartEffectGroup("STATE_UP", slChaIndex);
				}
				break;
			case MSG_EFFECT_ETC_RANDOM_EXP: // 렌덤 경헝치 증폭 
				{
					PCStartEffectGroup("EXP_UP", slChaIndex);
				}
				break;
			case MSG_EFFECT_ETC_RANDOM_SP: // 렌덤 SP 증폭
				{
					PCStartEffectGroup("SP_UP", slChaIndex);
				}
				break;
			case MSG_EFFECT_ETC_RANDOM_DROP: // 렌덤 드랍률 증폭
				{
					PCStartEffectGroup("DROP_UP", slChaIndex);
				}
				break;
			}
		} break;
	case MSG_EFFECT_PRODUCE:		// 생산 이펙트
		{

			SLONG	targetindex;
			SLONG	targethp;
			SBYTE	targettype;
			(*istr) >> sbEtcType;
			(*istr) >> sbChaType;		
			(*istr) >> slChaIndex;		
			(*istr) >> targettype;
			(*istr) >> targetindex;
			(*istr) >> targethp;
			
			((CPlayerEntity*)CEntity::GetPlayerEntity(0))->SetProduct(slChaIndex, sbEtcType);
			((CPlayerEntity*)CEntity::GetPlayerEntity(0))->ProcessProduce(sbChaType, slChaIndex, targettype, targetindex, targethp);
		}
		// FIXME : 생산 이펙트를 일단은 레벨업으로...
		//PCStartEffectGroup("LevelUp", slChaIndex);		//Hardcording
		break;		
	
	case MSG_EFFECT_ITEM:
		{		
			SLONG	slItemIndex;
			(*istr) >> sbChaType;
			(*istr) >> slChaIndex;
			(*istr) >> slItemIndex;

			if( sbChaType != MSG_CHAR_PC )
				break;
			if( slItemIndex == -1 )
				break;
			
			CItemData*	pItemData = _pNetwork->GetItemData( slItemIndex );

	#define INDEX_INVISIBLE_CURE		677
	#define INDEX_STONE_CURE			873
	#define INDEX_BLIND_CURE			874
	#define INDEX_SILENT_CURE			875	
	#define INDEX_RECOMMEND_EVENT		1393	
 
			switch( slItemIndex )
			{
			case INDEX_INVISIBLE_CURE:
				PCStartEffectGroup( "InVisibleCure", slChaIndex );
				return;
			case INDEX_STONE_CURE:
			case INDEX_BLIND_CURE:
			case INDEX_SILENT_CURE:
				PCStartEffectGroup( "CurePosion", slChaIndex );
				return;
			case INDEX_RECOMMEND_EVENT:
				PCStartEffectGroup( "server_event_using", slChaIndex );
				return;
			}
			
			if( pItemData->GetType() == CItemData::ITEM_POTION )
			{
/*				if( pItemData->GetSubType() == CItemData::POTION_HP )
				{
					PCStartEffectGroup( "potionHP", slChaIndex );
				}
				else if( pItemData->GetSubType() == CItemData::POTION_MP )
				{
					PCStartEffectGroup( "potionMP", slChaIndex );
				}*/
				/*if (strlen(pItemData->GetArmorEffectName()) > 0)
				{	// 포션 아이템에 Effect이름이 등록되어 있다는 조건에서 동작
					PCStartEffectGroup(pItemData->GetArmorEffectName(), slChaIndex);
				}*/

				if( slChaIndex == _pNetwork->MyCharacterInfo.index )
				{
					((CPlayerEntity*)CEntity::GetPlayerEntity(0))->PlayItemEffect(slItemIndex, -1);
				}
			}
		}
		break;
	case MSG_EFFECT_FIRE:
		{
			INDEX nExtra;
			INDEX nCount;
			INDEX index;
			INDEX animID[5];
			FLOAT x,z,h;
//			SBYTE yLayer;
			CPlacement3D vDesiredPosition;
			CEntity* penEntity = NULL;
			CTString strEffectname;
			int i;

			(*istr) >> nExtra;
			(*istr) >> nCount;

			CPrintF("MSG_EFFECT_FIRE\n");
			
			for (i=0; i<nCount; i++)
			{
				(*istr) >> index;
				(*istr) >> x;
				(*istr) >> z;
				(*istr) >> h;
		//		(*istr) >> yLayer;

				vDesiredPosition.pl_PositionVector(1) = x;
				vDesiredPosition.pl_PositionVector(2) = h;
				vDesiredPosition.pl_PositionVector(3) = z;
				vDesiredPosition.pl_OrientationAngle(1) = 0.0f;
				vDesiredPosition.pl_OrientationAngle(2) = 0.0f;
				vDesiredPosition.pl_OrientationAngle(3) = 0.0f;

				ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_MOB, index);

				if (pObject != NULL)
				{
					penEntity = pObject->GetEntity();
					if (penEntity != NULL)
						penEntity->Teleport(vDesiredPosition, FALSE);	

					CMobData* MD = CMobData::getData(pObject->m_nType);
					CPrintF("NPC index : %d \n", index);

					animID[0] = ska_StringToID(MD->GetAnimWalkName());		// 화염 이펙트
					animID[1] = ska_StringToID(MD->GetAnimAttackName());		// 바위 이펙트
					animID[2] = ska_StringToID(MD->GetAnimRunName());		// 번개 이펙트
					animID[3] = ska_StringToID(MD->GetAnimDefault2Name());	// 바람 이펙트
					animID[4] = ska_StringToID(MD->GetAnimAttack2Name());	// 얼음 이펙트

					if (penEntity != NULL && nExtra > 0 && nExtra < 6)
						penEntity->GetModelInstance()->AddAnimation(animID[nExtra-1], AN_CLEAR, 1.0f, 0);
				}
			}
		}
		break;
	}
}

void CSessionState::ReceiveEXPMessage( CNetworkMessage *istr )
{
	SQUAD Exp; //1013
	SLONG Sp;

	(*istr) >> Exp;
	(*istr) >> Sp;

	_pNetwork->MyCharacterInfo.curExp += Exp; 	
	_pNetwork->MyCharacterInfo.sp += Sp;

	CTString strSysMessage;
	if( Exp > 0 && Sp > 0 )
		strSysMessage.PrintF( _S( 337, "%I64d의 경험치와 %d의 SP를 얻었습니다." ), Exp, Sp );
	else if( Exp > 0 )
		strSysMessage.PrintF( _S( 427, "%I64d의 경험치를 얻었습니다." ), Exp );
	else
		strSysMessage.PrintF( _S( 428, "%d의 SP를 얻었습니다." ), Sp );

	CUIManager* pUIManager = CUIManager::getSingleton();

	pUIManager->GetChattingUI()->AddSysMessage( strSysMessage );
	pUIManager->GetSimplePlayerInfo()->UpdateEXP();
	pUIManager->GetCharacterInfo()->UpdateExpAndSp();
}
void CSessionState::ReceiveActionMessage( CNetworkMessage *istr )
{
	((CPlayerEntity*)CEntity::GetPlayerEntity(0))->Read_net_Character(MSG_ACTION, 0, &(*istr));
}

void CSessionState::ReceiveEnvMessage( CNetworkMessage *istr )//1013
{
	UBYTE subType;
	(*istr) >> subType;

	SLONG year, zone, sellrate, buyrate;
	SBYTE month, day, hour;
	UBYTE weather;

	// -------------------------------------->>
	LONG startWorldTime = 0;
	// --------------------------------------<<

	if(subType == MSG_ENV_TAX_CHANGE)
	{
		(*istr) >> zone
				>> sellrate
				>> buyrate;
	}
	else if(subType == MSG_ENV_WEATHER)
	{
		(*istr) >> weather;
	}
	else if(subType == MSG_ENV_TIME)
	{
		(*istr) >> year;
		(*istr) >> month;
		(*istr) >> day;
		(*istr) >> hour;
#if !(defined (G_JAPAN))
		(*istr) >> startWorldTime;
#endif	//!(defined (G_JAPAN))
//		if(_pNetwork->m_ubGMLevel < 2)//GM이라면 시간의 흐름을 멈춘다.
		{	
			CUIManager::getSingleton()->GetRadar()->SetTime(year,month,day,hour,startWorldTime);

			if (IS_EVENT_ON(A_EVENT_HOLLOWEEN))
			{
				g_fGWTime = 30*3600; // 할로윈데이2007이벤트에서는 계속 밤을 유지 한다.
				g_fGWTimeMul = 0.0f;
			}
			else
			{
				g_fGWTime = hour*3600;
			}
		}
	}
}

void CSessionState::ReceiveWarpMessage(CNetworkMessage *istr)
{
	UBYTE subtype;

	(*istr) >> subtype;
	static std::map<SLONG, CEffectGroup *> mapEG;

	CUIManager* pUIManager = CUIManager::getSingleton();

	if(subtype == MSG_WARP_START)
	{
		SLONG	char_index;
		(*istr) >> char_index;

//안태훈 수정 시작	//(5th Closed beta)(0.2)
		CTagManager *pTM = NULL;
		if( _pNetwork->MyCharacterInfo.index == char_index )
		{
			pUIManager->SetCSFlagOn(CSF_TELEPORT);
			pUIManager->GetTeleport()->CloseTeleport();

			pUIManager->GetInventory()->Lock( TRUE, FALSE, LOCK_WARP );

			if(CEntity::GetPlayerEntity(0) != NULL
				&& CEntity::GetPlayerEntity(0)->en_pmiModelInstance != NULL)
			{
				pTM = &CEntity::GetPlayerEntity(0)->en_pmiModelInstance->m_tmSkaTagManager;
			}
		}
		else
		{
			ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_CHARACTER, char_index);

			if (pObject != NULL)
			{
				CCharacterTarget* pTarget = static_cast< CCharacterTarget* >(pObject);

				pTM = &pTarget->GetEntity()->en_pmiModelInstance->m_tmSkaTagManager;
					}
				}
		
		CEffectGroup *pEG = StartEffectGroup("Teleport Start"		//Hardcording
						, pTM
						,_pTimer->GetLerpedCurrentTick());
		std::map<SLONG, CEffectGroup *>::value_type tmpVal(char_index, pEG);
		mapEG.insert(tmpVal);
//안태훈 수정 끝	//(5th Closed beta)(0.2)
	}
	else if(subtype == MSG_WARP_CANCEL)
	{
		SLONG	char_index;
		(*istr) >> char_index;

		if( _pNetwork->MyCharacterInfo.index == char_index )
		{
			pUIManager->GetChattingUI()->AddSysMessage( _S( 340, "이동이 취소되었습니다." ) );

			if(pUIManager->DoesMessageBoxExist(MSGCMD_WARP_TO_CANCEL)) pUIManager->CloseMessageBox(MSGCMD_WARP_TO_CANCEL); //wooss 050808
			if(pUIManager->DoesMessageBoxExist(MSGCMD_WARP_TAKE_CANCEL)) pUIManager->CloseMessageBox(MSGCMD_WARP_TAKE_CANCEL); //wooss 050808
			// Character state flags
			pUIManager->SetCSFlagOff( CSF_TELEPORT );
			pUIManager->GetInventory()->Lock( FALSE, FALSE, LOCK_WARP );
		}
//안태훈 수정 시작	//(5th Closed beta)(0.2)
		std::map<SLONG, CEffectGroup *>::iterator it = mapEG.find(char_index);
		if(it != mapEG.end())
		{
			if((*it).second != NULL && CEffectGroupManager::Instance().IsValidCreated((*it).second))
				(*it).second->Stop(0);
			mapEG.erase( it );
		}
	}
	else if(subtype == MSG_WARP_END)
	{
		SLONG char_index;
		(*istr) >> char_index;

		std::map<SLONG, CEffectGroup *>::iterator it = mapEG.find(char_index);
		if(it != mapEG.end())
		{
			if((*it).second != NULL && CEffectGroupManager::Instance().IsValidCreated((*it).second))
				(*it).second->Stop(0);
			mapEG.erase( it );
		}

		if( _pNetwork->MyCharacterInfo.index == char_index )
		{
			pUIManager->GetChattingUI()->AddSysMessage( _S( 395, "이동이 완료되었습니다." ) );

			if(pUIManager->DoesMessageBoxExist(MSGCMD_WARP_TO_CANCEL)) pUIManager->CloseMessageBox(MSGCMD_WARP_TO_CANCEL); //wooss 050808
			if(pUIManager->DoesMessageBoxExist(MSGCMD_WARP_TAKE_CANCEL)) pUIManager->CloseMessageBox(MSGCMD_WARP_TAKE_CANCEL); //wooss 050808
			// Character state flags
			pUIManager->GetInventory()->Lock( FALSE, FALSE, LOCK_WARP );
		}
	}
	else if(subtype == MSG_WARP_TELEPORT)
	{
		// pos(n) x(f:s) z(f:s) h(f:s) r(f:s) ylayer(c:s)
		LONG	lPos;
		float	fPosX;
		float	fPosZ;
		float	fPosH;
		float	fPosR;
		SBYTE	sbYLayer;

		(*istr) >> lPos;
		(*istr) >> fPosX;
		(*istr) >> fPosZ;
		(*istr) >> fPosH;
		(*istr) >> fPosR;
		(*istr) >> sbYLayer;
		
		CEntity* penPlEntity;
		CPlayerEntity* penPlayerEntity;
		penPlEntity = CEntity::GetPlayerEntity(0); //캐릭터 자기 자신
		penPlayerEntity = (CPlayerEntity*) penPlEntity;
		penPlayerEntity->WarpTeleport( fPosX, fPosZ, fPosH, fPosR, sbYLayer );
		UIMGR()->SetCSFlagOff(CSF_TELEPORT);
	}
	//wooss 050808 
	//유료아이템 소환마법서 메세지 처리
	
	else if(subtype == MSG_WARP_TO_REQ)
	{
		CTString charName;
		(*istr) >> charName;
		ConfirmMsgProc(MSG_WARP_TO_REQ,NULL,charName);			
	}else if(subtype == MSG_WARP_TAKE_REQ)
	{
		CTString charName;
		(*istr) >> charName;
		ConfirmMsgProc(MSG_WARP_TAKE_REQ,NULL,charName);			
	}

	//에러처리 
	else if(subtype == MSG_WARP_ERROR)
	{
		UBYTE errType;
		CTString strName;
		(*istr) >>errType;
		
		pUIManager->CloseMessageBox(MSGCMD_WARP_TO_CANCEL);
		pUIManager->CloseMessageBox(MSGCMD_WARP_TAKE_CANCEL);
		UIMGR()->SetCSFlagOff(CSF_TELEPORT);
		switch(errType)
		{
			case MSG_WARP_ERROR_NOTCHAR:
				ConfirmMsgProc(MSG_WARP_ERROR,MSG_WARP_ERROR_NOTCHAR);
				break;

			case MSG_WARP_ERROR_NOTALLOW:
				(*istr) >> strName;
				ASSERT(strName!="");
				ConfirmMsgProc(MSG_WARP_ERROR,MSG_WARP_ERROR_NOTALLOW,strName);
				break;

			case MSG_WARP_ERROR_NOTALLOWZONE:
				ConfirmMsgProc(MSG_WARP_ERROR,MSG_WARP_ERROR_NOTALLOWZONE);
				break;
			case MSG_WARP_ERROR_CANCEL:
				ConfirmMsgProc(MSG_WARP_ERROR,MSG_WARP_ERROR_CANCEL);
				break;
		}								
	}
	else if( subtype == MSG_WARP_PROMPT ) // 공성 시작 시 공성지역으로 이동 할 것인지 확인!!
	{		
		LONG	lZone;
		LONG	lPos;
		
		(*istr) >> lZone;
		(*istr) >> lPos;
		
		_pNetwork->MyCharacterInfo.lWarpZone = lZone;
		_pNetwork->MyCharacterInfo.lWarpPos = lPos;
		
		pUIManager->CloseMessageBox( MSGCMD_SIEGE_WARFARE_MOVEING_CONFIRM );
				
		pUIManager->m_nShowMessageBox = MSGCMD_SIEGE_WARFARE_MOVEING_CONFIRM;
		CUIMsgBox_Info	MsgBoxInfo;	
		CTString		strMessage;
		MsgBoxInfo.SetMsgBoxInfo(  _S( 2071, "공성지역 이동" ), UMBS_YESNO, UI_NONE, MSGCMD_SIEGE_WARFARE_MOVEING_CONFIRM );		
		strMessage.PrintF( _S( 2072, "공성 시작 지점으로 이동하시겠습니까?" ) );	
		MsgBoxInfo.AddString( strMessage );
		pUIManager->CreateMessageBox( MsgBoxInfo );
	} 
		
//안태훈 수정 끝	//(5th Closed beta)(0.2)
}

void CSessionState::ReceiveAssistMessage( CNetworkMessage *istr )
{
	//CTString tempMessage;
	//CUIManager* pUIManager = CUIManager::getSingleton();

	SBYTE	sbType;
	(*istr) >> sbType;

	switch( sbType )
	{
	case MSG_ASSIST_ADD:
		{
			SBYTE		sbCharType;
			SLONG		slCharIndex;

			BuffInfo	sBuff;
			sBuff.m_llStartTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();

			(*istr) >> sbCharType >> slCharIndex;
			//(*istr) >> sBuff.m_slItemIndex >> sBuff.m_slSkillIndex >> sBuff.m_sbLevel >> sBuff.m_slRemain;  //원본
			//2012/11/12 jeil 스킬확장 관련 count값 추가 
			(*istr) >> sBuff.m_slItemIndex;
			(*istr) >> sBuff.m_slSkillIndex;
			(*istr) >> sBuff.m_sbLevel;
			(*istr) >> sBuff.m_slRemain;
			(*istr) >> sBuff.m_slRemainCount;

			CItemData *pItemData = _pNetwork->GetItemData(sBuff.m_slItemIndex);
			CEntity* pDestEntity = NULL;
			ObjInfo* pInfo = ObjInfo::getSingleton();

			if( sbCharType == MSG_CHAR_PC )
			{
				if( _pNetwork->MyCharacterInfo.index == slCharIndex )
				{
				// wooss 070305 --------------------------------------------------------------------------------->>
				// kw : WSS_WHITEDAY_2007
				// 6종 러브매직 스킬
				// 423 424 425 426 427 428
				
					CEffectGroup* pEG = NULL;

					CEntity* penEntity =(CPlayerEntity*)CEntity::GetPlayerEntity(0);

					if( sBuff.m_slSkillIndex >= 423 && sBuff.m_slSkillIndex <= 428 )
					{
						int i, j;
						for(i = 423, j = 0; i <= 428; i++)
						{
							if( (_pUIBuff->IsBuffBySkill(i))!=NULL ){
								j++;
								break;
							}					
						}
						
						// 처음이면 이펙트를 생성함...
						if(j==0)
						{					
							// 지속 이펙트 - 이후 삭제시 지우기위해 저장...
							pEG = StartEffectGroup( "STATE_LOVE", &penEntity->en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
						}
						else
						{
							pEG = _pUIBuff->GetEGFromSkillIndex(i);
						}
						// 처음이 아니면 이전 CEffectGroup의 포인터를 넘겨줌...
						// 이후 버프가 지워질때 최종 하나가 지워질때 이펙트도 지워준다...
						sBuff.m_pEG =  pEG;
											
						// 추가 이펙트
						StartEffectGroup( "STATE_LOVEMAGIC", &penEntity->en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());				
					}
				// ------------------------------------------------------------------------------------------------<<
					else if (pItemData && pItemData->GetType() == CItemData::ITEM_POTION && strlen(pItemData->GetArmorEffectName()) > 0)
					{	// 포션 아이템에 Effect이름이 등록되어 있다는 조건에서 동작
						pEG = StartEffectGroup(pItemData->GetArmorEffectName(), &penEntity->en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
						sBuff.m_pEG = pEG;
					}

					if (sBuff.m_slSkillIndex == 564)	// 그림자 제거 마법가루 효과
					{
						((CPlayerEntity*)CEntity::GetPlayerEntity(0))->SetFlagOn(ENF_SHOWHIDDEN);
					}

					// 타겟에 붙어서 지속적으로 플레이 되는 이펙트를 붙여 주기 위해 추가.
					if (strlen(_pNetwork->GetSkillData(sBuff.m_slSkillIndex).GetAfter_AttachEffect()) > 0)
					{
						sBuff.m_pEG = StartEffectGroup(_pNetwork->GetSkillData(sBuff.m_slSkillIndex).GetAfter_AttachEffect(),
						&penEntity->en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
					}

					_pUIBuff->AddMyBuff( sBuff );

					pDestEntity =CEntity::GetPlayerEntity( 0 );

					//이속향상 물약일때 불이펙트를 위한 하드코딩..ㅡㅡ;
					// wooss 070310 ----------------------->><<
					// kw : WSS_WHITEDAY_2007
					// Add lovemagic buff (wind - skill index 423 )
					if(sBuff.m_slItemIndex ==391||sBuff.m_slItemIndex ==1226||sBuff.m_slItemIndex ==1227||sBuff.m_slSkillIndex == 423
						|| sBuff.m_slItemIndex ==2407 || sBuff.m_slItemIndex ==2408	|| sBuff.m_slItemIndex ==2609 || sBuff.m_slItemIndex == 2845 //광속 아이템, 바람의 속도 물약
						|| sBuff.m_slItemIndex == 2500
						|| sBuff.m_slItemIndex == 5018 || sBuff.m_slItemIndex == 5019 // 광속 2 아이템
						)
					{
						((CPlayerEntity*)CEntity::GetPlayerEntity(0))->SetSpeedUp(TRUE, 0, TRUE);
					}						
					CEntity	*penPlayer = CEntity::GetPlayerEntity( 0 );
					if( pInfo->GetTargetEntity(eTARGET) == penPlayer )
						_pUIBuff->AddTargetBuff( sBuff );
				}
				else
				{
					ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_CHARACTER, slCharIndex);

					if (pObject != NULL)
					{
						CCharacterTarget* pTarget = static_cast< CCharacterTarget* >(pObject);					

							// 6종 러브매직 스킬
							// 423 424 425 426 427 428
							
							CEffectGroup* pEG = NULL;
							CEntity* penEntity = _pNetwork->GetEntityByTypeIndex(sbCharType,slCharIndex);

							if( sBuff.m_slSkillIndex >= 423 && sBuff.m_slSkillIndex <= 428 )
							{
								int i, j;
								for(i = 423, j = 0; i <= 428; i++)
								{
								if( (pTarget->IsBuffBySkill(i)) != NULL){
										j++;
										break;
									}
								}

								// 처음이면 이펙트를 생성함...
								if(j==0)
								{									
									// 지속 이펙트 - 이후 삭제시 지우기위해 저장...
									pEG = StartEffectGroup( "STATE_LOVE", &penEntity->en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
								}
								else 
								{
								pEG = pTarget->IsBuffBySkill(i);	
								}
								// 처음이 아니면 이전 CEffectGroup의 포인터를 넘겨줌...
								// 이후 버프가 지워질때 최종 하나가 지워질때 이펙트도 지워준다...
								if( pEG != NULL)
								{
									sBuff.m_pEG =  pEG;
								}
								
								// 추가 이펙트
								StartEffectGroup( "STATE_LOVEMAGIC", &penEntity->en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());				
							}
							// ------------------------------------------------------------------------------------------------<<
							else if (pItemData->GetType() == CItemData::ITEM_POTION && strlen(pItemData->GetArmorEffectName()) > 0)
							{	// 포션 아이템에 Effect이름이 등록되어 있다는 조건에서 동작
								pEG = StartEffectGroup(pItemData->GetArmorEffectName(), &penEntity->en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
								sBuff.m_pEG = pEG;
							}

							if (strlen(_pNetwork->GetSkillData(sBuff.m_slSkillIndex).GetAfter_AttachEffect()) > 0)
							{
								sBuff.m_pEG = StartEffectGroup(_pNetwork->GetSkillData(sBuff.m_slSkillIndex).GetAfter_AttachEffect(),
									&penEntity->en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
							}

						pTarget->AddBuff( sBuff );
							_pUIBuff->AddPartyBuff( slCharIndex, sBuff );

						pDestEntity = pTarget->GetEntity();

							//이속향상 물약일때 불이펙트를 위한 하드코딩..ㅡㅡ;
							// wooss 070310 ----------------------->><<
							// kw : WSS_WHITEDAY_2007
							// Add lovemagic buff (wind - skill index 423 )
							if(sBuff.m_slItemIndex ==391||sBuff.m_slItemIndex ==1226||sBuff.m_slItemIndex ==1227||sBuff.m_slSkillIndex == 423
								|| sBuff.m_slItemIndex ==2407 || sBuff.m_slItemIndex ==2408	|| sBuff.m_slItemIndex ==2609 || sBuff.m_slItemIndex == 2845	//광속 아이템, 바람의 속도 물약
								|| sBuff.m_slItemIndex == 2500
								|| sBuff.m_slItemIndex == 5018 || sBuff.m_slItemIndex == 5019 // 광속 2 아이템
								)
							{
							((CPlayerEntity*)CEntity::GetPlayerEntity(0))->SetSpeedUp(FALSE, pTarget->GetCIndex(), TRUE);
							}
						
						if (pInfo->GetTargetEntity(eTARGET) == pTarget->GetEntity())
								_pUIBuff->AddTargetBuff( sBuff );
						}
					}
				}
			else if( sbCharType == MSG_CHAR_NPC )
			{
				ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_MOB, slCharIndex);

				if (pObject != NULL)
				{
					CMobTarget* pTarget = static_cast< CMobTarget* >(pObject);

					pDestEntity = pTarget->GetEntity();

					if (pDestEntity == NULL)
						return;

					if (strlen(_pNetwork->GetSkillData(sBuff.m_slSkillIndex).GetAfter_AttachEffect()) > 0)
					{
						CEffectGroup* pEG = StartEffectGroup(_pNetwork->GetSkillData(sBuff.m_slSkillIndex).GetAfter_AttachEffect(),
							&pDestEntity->en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());

						sBuff.m_pEG = pEG;
					}

					pTarget->AddBuff( sBuff );
					pTarget->BuffsStartGroupEffect();

					if( pInfo->GetTargetEntity(eTARGET) == pTarget->GetEntity() )
						_pUIBuff->AddTargetBuff( sBuff );
				}
			}
			else if( sbCharType == MSG_CHAR_ELEMENTAL )
			{
				ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_SLAVE, slCharIndex);

				if (pObject != NULL)
				{
					CSlaveTarget* pTarget = static_cast< CSlaveTarget* >(pObject);

					pTarget->AddBuff( sBuff );

					pDestEntity =pTarget->m_pEntity;

					if (pInfo->GetTargetEntity(eTARGET) == pTarget->GetEntity())
						_pUIBuff->AddTargetBuff( sBuff );
				}
			}

			// 절망의 부적일 경우 타겟에 이펙트 생성
			if( sBuff.m_slItemIndex ==2382 )
			{
				CTString strEffect = _pNetwork->GetSkillData(sBuff.m_slSkillIndex).GetFireEffect1(0);
				StartEffectGroup( strEffect, &pDestEntity->en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
			}
		}
		break;

	case MSG_ASSIST_DEL:
		{
			SBYTE	sbCharType;
			SLONG	slCharIndex, slItemIndex, slSkillIndex;

			(*istr) >> sbCharType >> slCharIndex >> slItemIndex >> slSkillIndex;

			CItemData* pItemData = _pNetwork->GetItemData(slItemIndex);
			ObjInfo* pInfo = ObjInfo::getSingleton();

			if( sbCharType == MSG_CHAR_PC )
			{
				if( _pNetwork->MyCharacterInfo.index == slCharIndex )
				{
					// wooss 070305 ----------------------------------->>
					// kw : WSS_WHITEDAY_2007
					// 6종 러브매직 스킬
					// 423 424 425 426 427 428
					if( slSkillIndex >= 423 && slSkillIndex <= 428 )
					{
						int i, j;
						for(i = 423, j = 0; i <= 428; i++)
						{
							if(_pUIBuff->IsBuffBySkill(i)) j++;
						}
						
						// 마지막 버프가 사라질때 이펙트를 지워줌...
						if(j==1)
						{
							CEffectGroup* pEG = _pUIBuff->GetEGFromSkillIndex(slSkillIndex);
							if(pEG!=NULL)
							{
								DestroyEffectGroup(pEG);
							}
						}
					}
					// -------------------------------------------------<<
					else if (slSkillIndex == 564) // 그림자 제거 마법 가루 효과 삭제
					{
						((CPlayerEntity*)CEntity::GetPlayerEntity(0))->SetFlagOff(ENF_SHOWHIDDEN);
						
						if (pInfo->GetTargetDBIdx(eTARGET) == 504 || pInfo->GetTargetDBIdx(eTARGET) == 505) //안개그림자 드래곤,핏빛그림자 드래곤
						{
							if (pInfo->GetTargetEntity(eTARGET) != NULL)
								CUIManager::getSingleton()->StopTargetEffect(pInfo->GetTargetServerIdx(eTARGET)); // 타겟 이펙트 종료

							((CPlayerEntity*)CEntity::GetPlayerEntity(0))->ClearTargetInfo(pInfo->GetTargetEntity(eTARGET));
						}
					}

					_pUIBuff->RemoveMyBuff( slItemIndex, slSkillIndex );

					
					// wooss 070310 ----------------------->><<
					// kw : WSS_WHITEDAY_2007
					// Add lovemagic buff (wind - skill index 423 )
					if( !_pUIBuff->IsBuff( 391 ) && !_pUIBuff->IsBuff( 1226 ) && !_pUIBuff->IsBuff( 1227 ) && !_pUIBuff->IsBuffBySkill(423) //이속향상 물약일때 불이펙트를 위한 하드코딩..ㅡㅡ;
						&& !_pUIBuff->IsBuff( 2407 ) && !_pUIBuff->IsBuff( 2408 ) && !_pUIBuff->IsBuff(2609) && !_pUIBuff->IsBuff(2845)		//광속 아이템, 바람의 속도 물약
						&& !_pUIBuff->IsBuff( 2500 )
						&& !_pUIBuff->IsBuff( 5018 ) && !_pUIBuff->IsBuff( 5019 ) // 광속 2 아이템
						)
					{
						((CPlayerEntity*)CEntity::GetPlayerEntity(0))->SetSpeedUp(TRUE, 0, FALSE);
					}

					CEntity	*penPlayer = CEntity::GetPlayerEntity( 0 );
					if( pInfo->GetTargetEntity(eTARGET) == penPlayer )
						_pUIBuff->RemoveTargetBuff( slItemIndex, slSkillIndex );
				}
				else
				{
					ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_CHARACTER, slCharIndex);

					if (pObject != NULL)
					{
						CCharacterTarget* pTarget = static_cast< CCharacterTarget* >(pObject);					
					
							// wooss 070312 ------------------------------------>>
							// kw : WSS_WHITEDAY_2007
							if( slSkillIndex >= 423 && slSkillIndex <= 428 )
							{
								int i, j;
								for(i = 423, j = 0; i <= 428; i++)
								{
								if (pTarget->IsBuffBySkill(i)) 
									j++;
								}
								
								// 마지막 버프가 사라질때 이펙트를 지워줌...
								if(j==1)
								{
								CEffectGroup* pEG = pTarget->IsBuffBySkill(slSkillIndex);
									if(pEG!=NULL)
									{
										DestroyEffectGroup(pEG);
									}
								}
							}					
							// -------------------------------------------------<<

						pTarget->RemoveBuff( slItemIndex, slSkillIndex );
							_pUIBuff->RemovePartyBuff( slCharIndex, slItemIndex, slSkillIndex );

							
							// wooss 070310 ----------------------->><<
							// kw : WSS_WHITEDAY_2007
							// Add lovemagic buff (wind - skill index 423 )
							//if( !_pUIBuff->IsBuff( 391 ) && !_pUIBuff->IsBuff( 1226 ) && !_pUIBuff->IsBuff( 1227 ) &&!_pUIBuff->IsBuffBySkill(423) )//이속향상 물약일때 불이펙트를 위한 하드코딩..ㅡㅡ;
						if (!pTarget->IsBuff( 391 ) && !pTarget->IsBuff( 1226 ) && !pTarget->IsBuff( 1227 ) &&!pTarget->IsBuffBySkill(423) //이속향상 물약일때 불이펙트를 위한 하드코딩..ㅡㅡ;
							&&!pTarget->IsBuff( 2407 ) && !pTarget->IsBuff( 2408 ) && !pTarget->IsBuff( 2609 ) && !pTarget->IsBuff(2845)	//광속 아이템
							&&!pTarget->IsBuff( 2500 )
							&&!pTarget->IsBuff( 5018 ) && !pTarget->IsBuff( 5019 ) // 광속 2 아이템
								)
							{
							((CPlayerEntity*)CEntity::GetPlayerEntity(0))->SetSpeedUp(FALSE, pTarget->m_nIdxClient, FALSE);
							}

						if( pInfo->GetTargetEntity(eTARGET) == pTarget->m_pEntity )
								_pUIBuff->RemoveTargetBuff( slItemIndex, slSkillIndex );
						}
					}
				}
			else if( sbCharType == MSG_CHAR_NPC )
			{
				ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_MOB, slCharIndex);

				if (pObject != NULL)
				{
					CMobTarget* pTarget = static_cast< CMobTarget* >(pObject);
					pTarget->RemoveBuff(slItemIndex, slSkillIndex);
					if (pInfo->GetTargetEntity(eTARGET) == pTarget->m_pEntity)
						_pUIBuff->RemoveTargetBuff( slItemIndex, slSkillIndex );
				}
			}
			else if( sbCharType == MSG_CHAR_ELEMENTAL )
			{
				ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_SLAVE, slCharIndex);

				if (pObject != NULL)
				{
					CSlaveTarget* pTarget = static_cast< CSlaveTarget* >(pObject);

					pTarget->RemoveBuff( slItemIndex, slSkillIndex );
					if (pInfo->GetTargetEntity(eTARGET) == pTarget->GetEntity())
						_pUIBuff->RemoveTargetBuff( slItemIndex, slSkillIndex );
				}
			}
		}
		break;
	case MSG_ASSIST_MODIFY:	//2012/11/12 jeil 추가 1119빌드 스킬확장 관련 추가	
		{
			SBYTE		sbCharType;
			SLONG		slCharIndex;
		
			BuffInfo	sBuff;
			sBuff.m_llStartTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();
		
			(*istr) >> sbCharType >> slCharIndex;
			//(*istr) >> sBuff.m_slItemIndex >> sBuff.m_slSkillIndex >> sBuff.m_sbLevel >> sBuff.m_slRemain;  //원본
			//2012/11/12 jeil 스킬확장 관련 count값 추가 
			(*istr) >> sBuff.m_slItemIndex >> sBuff.m_slSkillIndex >> sBuff.m_sbLevel >> sBuff.m_slRemain >> sBuff.m_slRemainCount;
			if(sBuff.m_slRemainCount == 0)
			{
				_pUIBuff->RemoveMyBuff( sBuff.m_slItemIndex, sBuff.m_slSkillIndex );
			}
			else
			{
				if( sbCharType == MSG_CHAR_PC )
				{
					if( _pNetwork->MyCharacterInfo.index == slCharIndex )
					{
						_pUIBuff->ChangeMyBuffRemain( sBuff.m_slItemIndex, sBuff.m_slSkillIndex, sBuff.m_llStartTime, sBuff.m_slRemain );
					}
					else
					{
						ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_CHARACTER, slCharIndex);

						if (pObject != NULL)
						{
							if( INFO()->GetTargetEntity(eTARGET) == pObject->GetEntity() )
									_pUIBuff->ChangeTargetBuffRemain( sBuff.m_slItemIndex, sBuff.m_slSkillIndex, sBuff.m_llStartTime, sBuff.m_slRemain );
							}
					}
				}
			}
		}
		break;
	case MSG_ASSIST_LIST:
		{
			SQUAD	slAssistState;
			SBYTE	sbAssistCount;
			(*istr) >> slAssistState >> sbAssistCount;

			_pUIBuff->ResetMyBuff();			

			BuffInfo	sBuff;
			sBuff.m_llStartTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();
			CEntity* penEntity =(CPlayerEntity*)CEntity::GetPlayerEntity(0);
			CEffectGroup* pEG = NULL;

			for( int i = 0; i < sbAssistCount; i++ )
			{
				//(*istr) >> sBuff.m_slItemIndex >> sBuff.m_slSkillIndex >> sBuff.m_sbLevel >> sBuff.m_slRemain; //원본 
				//2012/11/12 jeil 스킬확장 관련 count값 추가 
				(*istr) >> sBuff.m_slItemIndex;
				(*istr) >> sBuff.m_slSkillIndex;
				(*istr) >> sBuff.m_sbLevel;
				(*istr) >> sBuff.m_slRemain;
				(*istr) >> sBuff.m_slRemainCount;

				CItemData* pItemData = _pNetwork->GetItemData(sBuff.m_slItemIndex);
				// wooss 070305 ------------------------------------>>
				// kw : WSS_WHITEDAY_2007
				// 6종 러브매직 스킬
				// 423 424 425 426 427 428
				if( sBuff.m_slSkillIndex >= 423 && sBuff.m_slSkillIndex <= 428 )
				{
					int i, j;
					for(i = 423, j = 0; i <= 428; i++)
					{
						if(_pUIBuff->IsBuffBySkill(i)) j++;
					}
					
					// 처음이면 이펙트를 생성함...
					if(j==0)
					{
						pEG = StartEffectGroup( "STATE_LOVE", &penEntity->en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
				//		penEntity->GetModelInstance()->pEG = StartEffectGroup( 
				//			"STATE_LOVE", 
				//			&penEntity->en_pmiModelInstance->m_tmSkaTagManager, 
				//			_pTimer->GetLerpedCurrentTick());
					}

					sBuff.m_pEG =  pEG;							
				}
				// -------------------------------------------------<<
				else if (pItemData && pItemData->GetType() == CItemData::ITEM_POTION && strlen(pItemData->GetArmorEffectName()) > 0)
				{	// 포션 아이템에 Effect이름이 등록되어 있다는 조건에서 동작
					pEG = StartEffectGroup(pItemData->GetArmorEffectName(), &penEntity->en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
					sBuff.m_pEG = pEG;
				}

				if (sBuff.m_slSkillIndex == 564)	// 그림자 제거 마법가루 효과
				{
					((CPlayerEntity*)CEntity::GetPlayerEntity(0))->SetFlagOn(ENF_SHOWHIDDEN);
				}
				
				// wooss 070310 ----------------------->><<
				// kw : WSS_WHITEDAY_2007
				// Add lovemagic buff (wind - skill index 423 )
				if(sBuff.m_slItemIndex ==391 ||sBuff.m_slItemIndex ==1226 ||sBuff.m_slItemIndex ==1227 ||sBuff.m_slSkillIndex ==423//이속향상 물약일때 불이펙트를 위한 하드코딩..ㅡㅡ;
					|| sBuff.m_slItemIndex ==2407 ||sBuff.m_slItemIndex ==2408 ||sBuff.m_slItemIndex == 2609 ||	sBuff.m_slItemIndex == 2845	//광속 아이템, 바람의 속도 물약
					|| sBuff.m_slItemIndex == 2500
					|| sBuff.m_slItemIndex == 5018 || sBuff.m_slItemIndex == 5019 // 광속 2 아이템
					)
				{
					((CPlayerEntity*)CEntity::GetPlayerEntity(0))->SetSpeedUp(TRUE, 0, TRUE);
				}

				_pUIBuff->AddMyBuff( sBuff );
			}
		}
		break;
	}
}

// ----------------------------------------------------------------------------
// Name : ReceiveQuestMessage()
// Desc : 
// ----------------------------------------------------------------------------
void CSessionState::ReceiveQuestMessage(CNetworkMessage *istr)
{
	UBYTE ubType;
	(*istr) >> ubType;

	CUIManager* pUIManager = CUIManager::getSingleton();
	Quest* pQuest = GAMEDATAMGR()->GetQuest();

	if (pQuest == NULL)
		return;

	MSG_QUEST_TYPE msgQuestType = MSG_QUEST_TYPE(ubType);
	switch(msgQuestType)
	{
	case MSG_QUEST_PC_LIST:
		{
			INDEX questCount;
			INDEX questIndex;
			INDEX questCurCount[QUEST_MAX_CONDITION];

			(*istr) >> questCount;			// 수행중인 퀘스트 수

			pQuest->ClearQuestList();
			CQuestSystem::Instance().ClearAllDynamicData();
			for( INDEX iQuest = 0; iQuest < questCount; ++iQuest )
			{
				(*istr) >> questIndex;		// 퀘스트 번호
				// connie [2009/9/7] - tutorial quest
 				if( questIndex == 45)
					pUIManager->GetQuestBookList()->OpenTutorialWindow();

				for(int i = 0; i < QUEST_MAX_CONDITION; ++i)
				{
					(*istr) >> questCurCount[i];
				}

				CQuestDynamicData	*pQuestDD = CQuestSystem::Instance().GetDynamicDataByQuestIndex( questIndex );
				if( pQuestDD == NULL )
				{
					pQuestDD = CQuestSystem::Instance().Create( questIndex );
					if( pQuestDD != NULL )
					{
						CQuestSystem::Instance().AddCurrentRequest();
						pQuestDD->SetCurrentConditionValueStatus( questCurCount );
						BOOL bInserted = pQuest->AddQuestList( questIndex, pQuestDD->IsQuestComplete() );
						ASSERT(bInserted && "Somthing is wrong, client fail add quest to list");
						CQuestSystem::Instance().SetQuestAllow(questIndex, CQuestSystem::QAT_DOING);
					}
					else
					{
						ASSERTALWAYS("MSG_QUEST_LIST : Quest Index is abnormal. Server or Client has wrong quest index.");
					}
				}
			}
			pUIManager->GetQuestBookList()->RefreshQuestListNew();
			pUIManager->GetQuestView()->open();
		}
		break;
	case MSG_QUEST_NPC_LIST:
		{
			//TODO : NewQuestSystem
			INDEX iQuestCount;
			INDEX iQuestIndex;
			SBYTE sbQuestFlag;

			(*istr) >> iQuestCount;

			pQuest->ClearQuest();
			
			INDEX iQuestValid = 0;
			for( INDEX iQuest = 0; iQuest < iQuestCount; ++iQuest )
			{
				(*istr) >> iQuestIndex;
				(*istr) >> sbQuestFlag;
				if(sbQuestFlag == QUEST_FLAG_CAN)
				{
					BOOL bInserted = pQuest->AddQuest(iQuestIndex, FALSE);
					ASSERT(bInserted && "Somthing is wrong, client fail add quest to list");
					++iQuestValid;
				}
				else if(sbQuestFlag == QUEST_FLAG_COMPLETE)
				{
					BOOL bInserted = pQuest->AddQuest(iQuestIndex, TRUE);
					ASSERT(bInserted && "Somthing is wrong, client fail add quest to list");
					++iQuestValid;
				}
			}

			pUIManager->GetQuestBookList()->SetTargetIndex(pUIManager->GetQuestAccept()->GetTargetIndex());
			pUIManager->GetQuestAccept()->OpenWindow( iQuestValid > 0 ? TRUE : FALSE );
		}
		break;

	case MSG_QUEST_PD4_RANK_VIEW_REP:		// 싱글던젼 랭킹 뷰 응답.
		{
			// 랭크 뷰응답	: multicount(c) charname(str) bclear(c) deadmon(n) time(ll)
			SBYTE		sbMultiCount;
			CTString	strName;
			SBYTE		sbSuccess;
			INDEX		iNumOfDeadMonster;
			__int64		llTime;

			(*istr) >> sbMultiCount;

			pUIManager->GetRanking()->ClearRankingList();

			for( int i = 0; i < sbMultiCount; ++i )
			{
				(*istr) >> strName;
				(*istr) >> sbSuccess;
				(*istr) >> iNumOfDeadMonster;
				(*istr) >> llTime;
				pUIManager->GetRanking()->AddToRankingList( strName, sbSuccess, iNumOfDeadMonster, llTime );
			}
			
			if( !pUIManager->GetRanking()->IsVisible() )
				pUIManager->GetRanking()->OpenRankingList();
			else
				pUIManager->GetRanking()->RefreshRankingList();
		}
		break;

	case MSG_QUEST_PD4_RANK_REWARD_RANK_REP:	// 싱글던젼 보상 응답.
		{
			// 보상 뷰응답	: multicount(c) charname(str) itemindex(n) itemnum(c) breward(c)			
			SBYTE		sbMultiCount;
			CTString	strName;
			INDEX		iItemIndex;
			SBYTE		sbNumOfItem;
			SBYTE		sbReward;

			(*istr) >> sbMultiCount;

			for( int i = 0; i < sbMultiCount; ++i )
			{
				(*istr) >> strName;
				(*istr) >> iItemIndex;
				(*istr) >> sbNumOfItem;
				(*istr) >> sbReward;
				pUIManager->GetRanking()->AddToPrizeList( strName, iItemIndex, sbNumOfItem, sbReward );
			}

			pUIManager->GetRanking()->OpenRankingPrize();
		}
		break;

	case MSG_QUEST_PD4_REWARD:	// 보상을 받은 경우.
		{
			pUIManager->GetRanking()->ResetRanking();
		}
		break;

	case MSG_QUEST_PD4_ERROR:	// 에러 발생.
		{
			SBYTE	sbErrType;
			(*istr) >> sbErrType;
			CTString strMessage;

			switch( sbErrType )
			{
			case MSG_QUEST_ERR_REWARD_NOT_RANK:		// 보상시 랭커가 아님.
				strMessage = _S( 1754, "보상을 받을 수 있는 조건이 아닙니다." );		
				_pNetwork->ClientSystemMessage( strMessage, SYSMSG_ERROR );
				break;
			case MSG_QUEST_ERR_BREWARD:				// 이미 보상 받았음.
				strMessage = _S( 1755, "이미 보상을 받았습니다." );						
				_pNetwork->ClientSystemMessage( strMessage, SYSMSG_ERROR );
				break;
			}
			pUIManager->GetRanking()->ResetRanking();
		}
		break;

	case MSG_QUEST_CREATE_LIST://자동 퀘스트 부분
		{
			//TODO : NewQuestSystem
			INDEX iQuestCount;
			INDEX iQuestIndex;
			
			(*istr) >> iQuestCount;

			Notice* pNotice = GAMEDATAMGR()->GetNotice();			
			
			for( INDEX iQuest = 0; iQuest < iQuestCount; ++iQuest )
			{
				(*istr) >> iQuestIndex;

				if (pNotice != NULL)
					pNotice->AddToNoticeList(iQuestIndex, Notice::NOTICE_QUEST);
			}
			if(iQuestCount > 0)
			{
				CTString strLevelQuest;
				strLevelQuest.PrintF( _S( 770, "레벨 퀘스트를 %d개 받았습니다." ), iQuestCount);		
				_pNetwork->ClientSystemMessage( strLevelQuest );
			}
		}
		break;

	case MSG_QUEST_START:
		{
			//TODO : NewQuestSystem
			INDEX questIndex;
			INDEX remainTime = -1;		// [090609: selo] 타임어택에서의 남은 시간
			(*istr) >> questIndex;			
			(*istr) >> remainTime;
			//Check Point Init
			_pNetwork->SetCheckPointData(0,0,0);

			CQuestDynamicData	*pQuestDD = CQuestSystem::Instance().Create(questIndex);
			//pUIManager->GetCharacterInfo()->OpenQuestIntro( pQuestDD );

			//TODO : NewQuestSystem
			ASSERT( CQuestSystem::Instance().GetCurrentRequest() != NULL &&
					questIndex == CQuestSystem::Instance().GetCurrentRequest()->GetQuestIndex() );
			CQuestSystem::Instance().AddCurrentRequest();

			BOOL bInserted = pQuest->AddQuestList( questIndex, FALSE );
			pQuest->AddSelectedQuest(questIndex);
			ASSERT(bInserted && "Somthing is wrong, client fail add quest to list");
			
			// [090609: selo] 타임어택의 남은 시간을 설정한다.
			pQuest->SetQuestRemainTime(questIndex, remainTime);

			pUIManager->GetQuestBookList()->RefreshQuestListNew();

			if(g_iQuestBookOpen != 0)
			{
				pUIManager->GetQuestBookList()->open();
			}
			else
			{
				CTString	cstrQuestMessage;
				cstrQuestMessage.PrintF( _S(5588, "[%s] 퀘스트를 수락하였습니다."), CQuestSystem::Instance().GetQuestName(questIndex));
				_pNetwork->ClientSystemMessage(cstrQuestMessage);
			}

			CQuestSystem::Instance().SetQuestAllow(questIndex, CQuestSystem::QAT_DOING);
			INDEX iTargetIndex = pUIManager->GetQuestAccept()->GetTargetIndex();
			ACTORMGR()->RefreshNPCQuestMark(iTargetIndex);

			pUIManager->GetQuestAccept()->close();
			
			if( pQuestDD->GetQuestType1() == QTYPE_KIND_SAVE)			// 싱글던젼 퀘스트의 경우(구출 퀘스트의 경우)
			{
				// 일단 던젼으로 이동 시키고나서...
				// 무조건 0으로 판단함.				
				const int iWorldNum = pQuestDD->GetConditionData(0, 0);						// 월드 번호.
				const int iExtraNum = pQuestDD->GetConditionData(0, 1);						// Extra 번호.
				_pNetwork->GoZone(iWorldNum, iExtraNum,pQuestDD->GetPrizeNPCIndex());
			}
			else if( pQuestDD->GetQuestType1() == QTYPE_KIND_DEFEAT )	// 싱글던젼 퀘스트의 경우(격파 퀘스트의 경우)
			{
				// 일단 던젼으로 이동 시키고나서...
				// 무조건 0으로 판단함.
				const int iWorldNum = pQuestDD->GetConditionData(0, 0);						// 월드 번호.
				const int iExtraNum = pQuestDD->GetConditionData(0, 1);						// Extra 번호.
				_pNetwork->GoZone(iWorldNum, iExtraNum,pQuestDD->GetPrizeNPCIndex());
			}		
		}
		break;

	case MSG_QUEST_STATUS:
		{
			//TODO : NewQuestSystem
			INDEX questIndex;
			INDEX currentConditionValue[3];

			(*istr) >> questIndex;
			CQuestDynamicData *pData = CQuestSystem::Instance().GetDynamicDataByQuestIndex(questIndex);
			int changeIndex = -1;
			for(int i = 0; i < QUEST_MAX_CONDITION; ++i)
			{
				(*istr) >> currentConditionValue[i];
				if(pData->GetCurrentConditionValueStatus(i) != currentConditionValue[i]
				&& (pData->GetConditionType(i) == QCONDITION_ITEM
					|| pData->GetConditionType(i) == QCONDITION_ITEM_NORMAL
					|| pData->GetConditionType(i) == QCONDITION_NPC
					|| pData->GetConditionType(i) == QCONDITION_AREA
					|| pData->GetConditionType(i) == QCONDITION_ITEMUSE)
				)
					changeIndex = i;
			}

			if(pData != NULL)
			{
				pData->SetCurrentConditionValueStatus(currentConditionValue);
				// [090731: selo] 퀘스트 정보 업데이트 처리
				pUIManager->GetQuestBookList()->UpdateQuestContent(questIndex);
				pUIManager->GetQuestView()->UpdateContent();

				if( changeIndex != -1
				&& (_UIAutoHelp->GetGMNoticeColor() == 0xFFFFFFFF || !_UIAutoHelp->IsShowGMNotice()) )
				{
					CTString strCurrentChangedStatus = CQuestSystem::Instance().MakeInfoForCondition(
						questIndex
						, pData->GetConditionType(changeIndex)
						, pData->GetConditionIndex(changeIndex)
						, currentConditionValue[changeIndex]
						, pData->GetConditionNum(changeIndex));
					if(strCurrentChangedStatus.Length() != 0)
						_UIAutoHelp->SetGMNotice(strCurrentChangedStatus, 0xFFFFFFFF);
				}
			}
			else
			{
				ASSERTALWAYS("MSG_QUEST_STATUS : Quest Index is abnormal. Server or Client has wrong quest index.");
			}
		}
		break;

	case MSG_QUEST_COMPLETE:
		{
			INDEX questIndex;
			(*istr) >> questIndex;

			//TODO : NewQuestSystem
			CQuestDynamicData *pData = CQuestSystem::Instance().GetDynamicDataByQuestIndex(questIndex);
			if(pData)
			{
				pData->MakeQuestComplete();
				pQuest->DelQuestList(questIndex, FALSE);
				pQuest->AddQuestList(questIndex, TRUE);
				pUIManager->GetQuestBookList()->RefreshQuestListNew();
				pUIManager->GetQuestBookList()->UpdateQuestContent(questIndex);

				ACTORMGR()->RefreshNPCQuestMark(pData->GetPrizeNPCIndex());
				// 싱글던젼 격파형 퀘스트의 경우.
				if(_pNetwork->m_bSingleMode)
				{
					if(pData->GetQuestType1() == QTYPE_KIND_DEFEAT || pData->GetQuestType1() == QTYPE_KIND_SAVE )
					{
						pUIManager->CloseMessageBox(MSGCMD_QUEST_COMPLETE);
						CTString		strMessage;
						CUIMsgBox_Info	MsgBoxInfo;	
						MsgBoxInfo.SetMsgBoxInfo(  _S( 99, "퀘스트" ), UMBS_OK, UI_CHARACTERINFO, MSGCMD_QUEST_COMPLETE);		
						strMessage.PrintF( _S( 667, "[%s] 퀘스트를 완료 했습니다." ), pData->GetName());	
						MsgBoxInfo.AddString( strMessage );
						pUIManager->CreateMessageBox( MsgBoxInfo );		
					
						// 공주 구출 후 파티를 끊는다. [12/26/2006 Theodoric]
						if( _pNetwork->m_bSingleMode && GAMEDATAMGR()->GetPartyInfo()->GetMemberCount())
						{
							GAMEDATAMGR()->GetPartyInfo()->PartyEnd();
						}
						// 스타트 존으로 이동시킴.
						//_pNetwork->GoZone(0, 0);
					}
				}
			}
			else
			{
				ASSERTALWAYS("MSG_QUEST_COMPLETE : Quest Index is abnormal. Server or Client has wrong quest index.");
			}

			pUIManager->GetQuestView()->open();
		}
		break;

	case MSG_QUEST_UNCOMPLETE:
		{
			INDEX questIndex;
			(*istr) >> questIndex;
			//TODO : NewQuestSystem
			CQuestDynamicData *pData = CQuestSystem::Instance().GetDynamicDataByQuestIndex(questIndex);
			if(pData)
			{
				pData->MakeQuestUncomplete();
				BOOL bDeleted = pQuest->DelQuestList(questIndex, TRUE);
				if(!bDeleted)//뻑나는거 방지용
				{
					bDeleted = pQuest->DelQuestList(questIndex, FALSE);
					ASSERTALWAYS("Somthing is wrong, client fail delete quest from list");
					if(!bDeleted)
					{
						ASSERTALWAYS("Somthing is wrong, unknown quest index");
					}
				}
				BOOL bInserted = pQuest->AddQuestList(questIndex, FALSE);
				ASSERT(bInserted && "Somthing is wrong, client fail add quest to list");
				pUIManager->GetQuestBookList()->RefreshQuestListNew();
				pUIManager->GetQuestBookList()->UpdateQuestContent(questIndex);

				ACTORMGR()->RefreshNPCQuestMark(pData->GetPrizeNPCIndex());
			}
			else
			{
				ASSERTALWAYS("MSG_QUEST_UNCOMPLETE : Quest Index is abnormal. Server or Client has wrong quest index.");
			}
		} break;

	case MSG_QUEST_GIVEUP:
		{
			//TODO : NewQuestSystem
			INDEX questIndex;
			(*istr) >> questIndex;

			if( CQuestSystem::Instance().GetCurrentRequest() != NULL &&
				questIndex == CQuestSystem::Instance().GetCurrentRequest()->GetQuestIndex() )
			{
				CQuestSystem::Instance().DeleteCurrentRequest();
				CQuestSystem::Instance().SetQuestAllow(questIndex, CQuestSystem::QAT_GIVEUP);
				ACTORMGR()->RefreshNPCQuestMark(pUIManager->GetQuestAccept()->GetTargetIndex());
				pUIManager->GetQuestAccept()->close();
			}
			else
			{
				CQuestDynamicData *pData = CQuestSystem::Instance().GetDynamicDataByQuestIndex(questIndex);
				if( pData != NULL )
				{
					if(pUIManager->GetQuestBookComplete()->GetCurrIdx() == questIndex)
						pUIManager->GetQuestBookComplete()->close();
					BOOL bDeleted = pQuest->DelQuestList( questIndex, pData->IsQuestComplete() );
					pQuest->RemoveSelectedQuest(questIndex);
					if(!bDeleted)//뻑나는거 방지용
					{
						bDeleted = pQuest->DelQuestList(questIndex, !pData->IsQuestComplete());
						ASSERTALWAYS("Somthing is wrong, client fail delete quest from list");
						if(!bDeleted)
						{
							ASSERTALWAYS("Somthing is wrong, unknown quest index");
						}
					}
					CQuestSystem::Instance().SetQuestAllow(questIndex, CQuestSystem::QAT_GIVEUP);

					INDEX startType = pData->GetStartType();
					INDEX startData = pData->GetStartData();
					INDEX prizeNPCIndex = pData->GetPrizeNPCIndex();
					CQuestSystem::Instance().Remove( questIndex );
					if(startType == QSTART_NPC)
						ACTORMGR()->RefreshNPCQuestMark(startData);
					ACTORMGR()->RefreshNPCQuestMark(prizeNPCIndex);

 					// 공주구출 퀘수행도중 포기하고 나와도 공주 인포창이 그대로 있는 버그 [12/27/2006 Theodoric]
					if( questIndex == 14 ) // 공주구출
					{	_pNetwork->EndRescueNPC();	}
				}
				else
				{
					CQuestSystem::Instance().DeleteCurrentRequest();
					CQuestSystem::Instance().SetQuestAllow(questIndex, CQuestSystem::QAT_GIVEUP);
					ACTORMGR()->RefreshNPCQuestMark(pUIManager->GetQuestAccept()->GetTargetIndex());
					pUIManager->GetQuestAccept()->close();
				}
			}
			pUIManager->GetQuestBookList()->RefreshQuestListNew();
			pUIManager->GetQuestBookList()->UpdateQuestContent(-1);
		}
		break;

	case MSG_QUEST_PRIZE:
		{
			//TODO : NewQuestSystem
			INDEX questIndex;
			(*istr) >> questIndex;
			
			pUIManager->GetQuestBookComplete()->close();
			
			CQuestDynamicData *pData = CQuestSystem::Instance().GetDynamicDataByQuestIndex(questIndex);
			if(pData)
			{
				if(pData->GetQuestType1() != QTYPE_KIND_TUTORIAL )
				{
					CTString		strMessage;
					strMessage.PrintF("%s", pData->GetName());
					
					INDEX iPrizeNPCIndex = pData->GetPrizeNPCIndex();
					if( CQuestSystem::Instance().Remove( questIndex ) )
					{
						BOOL bDeleted = pQuest->DelQuestList(questIndex, TRUE);
						pQuest->RemoveSelectedQuest(questIndex);
						if(!bDeleted)//뻑나는거 방지용
						{
							bDeleted = pQuest->DelQuestList(questIndex, FALSE);
							ASSERTALWAYS("Somthing is wrong, client fail delete quest from list");
							if(!bDeleted)
							{
								ASSERTALWAYS("Somthing is wrong, unknown quest index");
							}
						}
					}

					pUIManager->GetQuestBookList()->RefreshQuestListNew();
					pUIManager->GetQuestBookList()->UpdateQuestContent(-1);
					CQuestSystem::Instance().SetQuestAllow(questIndex, CQuestSystem::QAT_ALREADY_DONE);
					ACTORMGR()->RefreshNPCQuestMark(iPrizeNPCIndex);

					INDEX iContactNpc = CQuestSystem::Instance().SearchContactQuestIndex(questIndex);

					if (iContactNpc>0 && iContactNpc!=iPrizeNPCIndex)
					{
						ACTORMGR()->RefreshNPCQuestMark(iContactNpc);
					}
				}
				// FIXME : 튜토리얼 퀘스트만 특이해서 이렇게 처리함...
				else
				{
					CTString		strMessage;
					strMessage.PrintF("%s", pData->GetName());
					
					// Create skill learn message box
					pUIManager->CreateMessageBoxL( strMessage, UI_CHARACTERINFO, MSGLCMD_QUEST_TUTORIAL );
					
					strMessage.PrintF("%s", pData->GetDesc2());
					pUIManager->AddMessageBoxLString( MSGLCMD_QUEST_TUTORIAL, TRUE, strMessage );					
					pUIManager->AddMessageBoxLString( MSGLCMD_QUEST_TUTORIAL, FALSE, _S( 191, "확인" ), 0 );
				}
			}
			else
			{
				ASSERTALWAYS("MSG_QUEST_PRIZE : Quest Index is abnormal. Server or Client has wrong quest index.");
			}
		}
		break;

	case MSG_QUEST_ERROR:
		{
			//TODO : NewQuestSystem
			SBYTE	sbErrType;
			(*istr) >> sbErrType;

			if( CQuestSystem::Instance().GetCurrentRequest() )
				CQuestSystem::Instance().DeleteCurrentRequest();

			pUIManager->GetQuest()->QuestError(sbErrType);
		}
		break;

	case MSG_QUEST_FAIL:
		{
			//TODO : NewQuestSystem
			INDEX questIndex;
			(*istr) >> questIndex;
			
			CQuestDynamicData *pData = CQuestSystem::Instance().GetDynamicDataByQuestIndex(questIndex);
			if(pData)
			{
				pUIManager->GetQuest()->SetCurQuest(questIndex);
				INDEX startType = pData->GetStartType();
				INDEX startData = pData->GetStartData();
				INDEX prizeNPCIndex = pData->GetPrizeNPCIndex();
				//CQuestSystem::Instance().SetQuestAllow(questIndex, CQuestSystem::QAT_GIVEUP);
				if(_pNetwork->MyCharacterInfo.hp != 0)
				{					
					pUIManager->CloseMessageBox(MSGCMD_QUEST_FAIL);
					CTString		strMessage;
					CUIMsgBox_Info	MsgBoxInfo;	
					MsgBoxInfo.SetMsgBoxInfo(  _S( 99, "퀘스트" ), UMBS_OK, UI_QUEST, MSGCMD_QUEST_FAIL);		
					strMessage.PrintF( _S( 669, "[%s] 퀘스트를 실패했습니다." ), pData->GetName());	
					MsgBoxInfo.AddString( strMessage );
					pUIManager->CreateMessageBox( MsgBoxInfo );		
				}
				else
				{
					CTString strQuestFail;// = "퀘스트를 실패했습니다.";		
					strQuestFail.PrintF( _S( 669, "[%s] 퀘스트를 실패했습니다." ), pData->GetName()); 
					_pNetwork->ClientSystemMessage( strQuestFail );
				}
				
				// 싱글던젼의 경우에는 버튼을 누른다음에 제거해줌.
				if( !_pNetwork->m_bSingleMode )
				{
					BOOL isComplete = pData->IsQuestComplete();
					if( CQuestSystem::Instance().Remove( questIndex ) )
					{
						BOOL bDeleted = pQuest->DelQuestList(questIndex, isComplete);
						pQuest->RemoveSelectedQuest(questIndex);
						if(!bDeleted)//뻑나는거 방지용
						{
							bDeleted = pQuest->DelQuestList(questIndex, !isComplete);
							ASSERTALWAYS("Somthing is wrong, client fail delete quest from list");
							if(!bDeleted)
							{
								ASSERTALWAYS("Somthing is wrong, unknown quest index");
							}
						}
					}
					pUIManager->GetQuestBookList()->RefreshQuestListNew();
					pUIManager->GetQuestBookList()->UpdateQuestContent(-1);
					
					if(pUIManager->GetQuestBookComplete()->GetCurrIdx() == questIndex)
						pUIManager->GetQuestBookComplete()->close();
					CQuestSystem::Instance().SetQuestAllow(questIndex, CQuestSystem::QAT_ALLOW);
					
					if(startType == QSTART_NPC)
						ACTORMGR()->RefreshNPCQuestMark(startData);
					ACTORMGR()->RefreshNPCQuestMark(prizeNPCIndex);
				}				

				if( questIndex == 14 ) // 공주 구출
				{
					_pNetwork->EndRescueNPC();
				}
			}
		}
		break;
		
	case MSG_QUEST_DONE_LIST:
		{
			INDEX completeQuestCount;
			(*istr) >> completeQuestCount;
			for(int i=0; i<completeQuestCount; ++i)
			{
				INDEX completeQuestIndex;
				(*istr) >> completeQuestIndex;
				CQuestSystem::Instance().SetQuestAllow(completeQuestIndex, CQuestSystem::QAT_ALREADY_DONE);
			}
		} break;
	case MSG_QUEST_ABANDON_LIST:
		{
			INDEX giveupQuestCount;
			(*istr) >> giveupQuestCount;
			for(int i=0; i<giveupQuestCount; ++i)
			{
				INDEX giveupQuestIndex;
				(*istr) >> giveupQuestIndex;
				CQuestSystem::Instance().SetQuestAllow(giveupQuestIndex, CQuestSystem::QAT_GIVEUP);
			}
		} break;
	case MSG_QUEST_FORCE_GIVEUP:
		{
			INDEX questIndex;
			(*istr) >> questIndex;
			CQuestDynamicData *pData = CQuestSystem::Instance().GetDynamicDataByQuestIndex(questIndex);
			if( pData != NULL )
			{
				//확인창
				CUIMsgBox_Info	MsgBoxInfo;
				MsgBoxInfo.SetMsgBoxInfo( _S( 99, "퀘스트" ), UMBS_OK, UI_QUESTBOOK_LIST, MSGCMD_QUEST_NOTIFY );
				CTString strMessage;
				strMessage.PrintF( _S( 1756, "%s 퀘스트가 적정 수행 레벨을 지나 삭제됩니다." ), pData->GetName() );
				MsgBoxInfo.AddString( strMessage );
				pUIManager->CreateMessageBox( MsgBoxInfo );

				//관련해서 열려있는 창 닫기 및 퀘스트 상태 변경
				if(pUIManager->GetQuestBookComplete()->GetCurrIdx() == questIndex)
					pUIManager->GetQuestBookComplete()->close();
				BOOL bDeleted = pQuest->DelQuestList( questIndex, pData->IsQuestComplete() );
				pQuest->RemoveSelectedQuest(questIndex);
				if(!bDeleted)//뻑나는거 방지용
				{
					bDeleted = pQuest->DelQuestList(questIndex, !pData->IsQuestComplete());
					ASSERTALWAYS("Somthing is wrong, client fail delete quest from list");
					if(!bDeleted)
					{
						ASSERTALWAYS("Somthing is wrong, unknown quest index");
					}
				}
				CQuestSystem::Instance().SetQuestAllow(questIndex, CQuestSystem::QAT_GIVEUP);

				//npc의 퀘스트 마크 갱신
				INDEX startType = pData->GetStartType();
				INDEX startData = pData->GetStartData();
				INDEX prizeNPCIndex = pData->GetPrizeNPCIndex();
				CQuestSystem::Instance().Remove( questIndex );
				if(startType == QSTART_NPC)
					ACTORMGR()->RefreshNPCQuestMark(startData);
				ACTORMGR()->RefreshNPCQuestMark(prizeNPCIndex);
			}
			//퀘스트 리스트 갱신
			pUIManager->GetQuestBookList()->RefreshQuestListNew();
			pUIManager->GetQuestBookList()->UpdateQuestContent(-1);
		} break;
		
	case MSG_QUEST_COLLECT :
		{
			LONG npcIdx,charIdx,itemIdx;
			(*istr) >> npcIdx>>charIdx>>itemIdx;
			
			// 	>	0 : NPC는 사망하고 itemdbindex를 지급함
			//	==	0 : NPC는 사망하고 아이템 지급 없음
			//	<   0 : NPC는 그대로 아이템 지급 없음

			if(itemIdx >= 0)
			{	
				// npc를 죽임 ㅡㅡ;;
				CEntity* penEntity = NULL;
				CEntity *penTargetEntity = NULL;
			
				if( _pNetwork->SearchEntityByNetworkID( npcIdx, MSG_CHAR_NPC, penTargetEntity ) )
				{
					((CMovableModelEntity*)penTargetEntity)->DeathNow();
				}
				
				//  수집 중지, 탈착한 무기 장착
				((CPlayerEntity*)CEntity::GetPlayerEntity(0))->CancelProduct();
				pUIManager->GetSelectResource()->SetKeyMove(true);
			}

		}
		break;
		// [100208: selo] 포기 퀘스트 복구
	case MSG_QUEST_RESTORE_ABANDON:
		{
			INDEX storeCount;
			INDEX questIndex = -1;
			CQuestDynamicData *pData = NULL;
			INDEX startType = -1;
			INDEX startData = -1;
			INDEX prizeNPCIndex = -1;

			(*istr) >> storeCount;
			pQuest->ClearAllRestoreList();

			for( int i = 0; i < storeCount; i++ )
			{				
				(*istr) >> questIndex;
				
				CQuestDynamicData QuestDD(CQuestSystem::Instance().GetStaticData(questIndex));
				
				pQuest->AddRestoreQuestList(questIndex, QuestDD.GetNeedMinLevel());

				startType = QuestDD.GetStartType();	
				startData = QuestDD.GetStartData();
				prizeNPCIndex = QuestDD.GetPrizeNPCIndex();
				
				CQuestSystem::Instance().SetQuestAllow(questIndex, CQuestSystem::QAT_ALLOW);
				
				if(startType == QSTART_NPC)
					ACTORMGR()->RefreshNPCQuestMark(startData);
				ACTORMGR()->RefreshNPCQuestMark(prizeNPCIndex);				
			}

			pQuest->SortRestoreQuest();

			pUIManager->GetQuestRestore()->open();
			pUIManager->GetInitJob()->CloseInitJob();
		}
		break;

	default:
		{
			//ASSERTALWAYS("Not defined MSG_QUEST_TYPE.");
		}
		break;
	}
}
			
// ----------------------------------------------------------------------------
// Name : ReceivePkMessage()
// Desc : 
// ----------------------------------------------------------------------------			
void CSessionState::ReceivePkMessage(CNetworkMessage *istr)
{
	UBYTE	ubType;
	(*istr) >> ubType;
	
	switch(ubType)
	{
	case MSG_PK_ITEMSEAL:
		{
			SBYTE sbTab;
			SBYTE sbIdx;
			ULONG ulIndex;
			ULONG ulFlag;
			(*istr) >> sbTab;
			(*istr) >> sbIdx;
			(*istr) >> ulIndex;
			(*istr) >> ulFlag;
			
			const int iIndex = _pNetwork->MySlotItem[sbTab][sbIdx].Item_Index;
			//CItemData& ID = _pNetwork->GetItemData(iIndex);
			CTString strItemSealed;
			strItemSealed.PrintF( _S2( 670, _pNetwork->GetItemName(iIndex), "[%s]<가> 봉인되었습니다." ), _pNetwork->GetItemName(iIndex) );		
			_pNetwork->ClientSystemMessage( strItemSealed );
		}
		break;
	case MSG_PK_RECOVER_ITEMSEAL:	// item 봉인 응답
		{
			SBYTE sbTab;
			SBYTE sbIdx;			
			SLONG slIndex;
			SLONG slFlag;
			(*istr) >> sbTab;			
			(*istr) >> sbIdx;
			(*istr) >> slIndex;
			(*istr) >> slFlag;

			const int iIndex = _pNetwork->MySlotItem[sbTab][sbIdx].Item_Index;
			//CItemData& ID = _pNetwork->GetItemData(iIndex);
			CTString strItemSealed;
			strItemSealed.PrintF( _S2( 672, _pNetwork->GetItemName(iIndex), "[%s]<가> 봉인이 해제되었습니다." ), _pNetwork->GetItemName(iIndex) );	
			_pNetwork->ClientSystemMessage( strItemSealed );
			//CUIManager::getSingleton()->GetRemission()->InitRemission();
		}
		break;
	case MSG_PK_TARGET_SHOWHP: // WSS_PK_TARGET_HP 070726
		{
			SBYTE	sbON;
			(*istr) >> sbON;
			CUIManager::getSingleton()->GetTargetInfoUI()->ShowPKTargetHP( (BOOL)sbON );
		}
		break;
	case MSG_PK_ERROR:
		{
			SBYTE	sbError;
			(*istr) >> sbError;

			//CUIManager::getSingleton()->GetRemission()->RemissionError( sbError );
		}
		break;
	default:
		{
			ASSERTALWAYS("Not defined MSG_QUEST_TYPE.");
		}
		break;
	}
}

// ----------------------------------------------------------------------------
// Name : ReceivePersonalShopMessage()
// Desc : 
// ----------------------------------------------------------------------------
void CSessionState::ReceivePersonalShopMessage(CNetworkMessage *istr)
{
	UBYTE	ubType;
	(*istr) >> ubType;	

	_pNetwork->m_iNetworkResponse[MSG_PERSONALSHOP]		= 0;

	CUIManager* pUIManager = CUIManager::getSingleton();

	switch(ubType)
	{
	case MSG_PERSONALSHOP_ERROR:
		{
			SBYTE	sbError;
			(*istr) >> sbError;

			CTString strMessage;
			switch(sbError)
			{
			//case MSG_PERSONALSHOP_ERROR_OK:
			//	strMessage.PrintF(_S( 771, "정상" ));		
			//	break;
			case MSG_PERSONALSHOP_ERROR_INSUFF_SKILL:		// 스킬 사용
				strMessage.PrintF(_S( 772, "거래를 요청한 캐릭터가 스킬을 사용 중입니다." ));		
				break;
			case MSG_PERSONALSHOP_ERROR_INSUFF_WARP:		// 워프
				strMessage.PrintF(_S( 773, "거래를 요청한 캐릭터가 워프(이동장치) 중입니다." ));		
				break;
			case MSG_PERSONALSHOP_ERROR_INSUFF_EXCHANGE:	// 교환 중
				strMessage.PrintF(_S( 774, "거래를 요청한 캐릭터가 교환 중입니다." ));		
				break;
			case MSG_PERSONALSHOP_ERROR_INSUFF_ALREADY:		// 상점 개설중
				strMessage.PrintF(_S( 775, "거래를 요청한 캐릭터가 이미 개인상점 거래중입니다." ));		
				break;
			case MSG_PERSONALSHOP_ERROR_INSUFF_MONEY:		// 나스를 개인상점으로 거래하고자 할때
				strMessage.PrintF(_S( 776, "나스는 개인상점에서 거래할 수 없습니다." ));		
				break;
			case MSG_PERSONALSHOP_ERROR_INSUFF_ETC:			// 상점을 열수 없는 상태(스킬사용/워프/패킷내용오류 등등)
				strMessage.PrintF(_S( 777, "개인상점 거래가 취소되었습니다." ));		
				break;
			case MSG_PERSONALSHOP_ERROR_DUPLICATION:		// 중복 등록
				strMessage.PrintF(_S( 778, "아이템이 중복 등록 되었습니다." ));		
				break;
			case MSG_PERSONALSHOP_ERROR_NOTSELL:			// 대상 캐릭터가 판매 상점이 아닐 때
				strMessage.PrintF(_S( 779, "대상 캐릭터가 판매 상점이 아닙니다." ));		
				break;
			case MSG_PERSONALSHOP_ERROR_NOTPACKAGE:			// 대상 캐릭터가 패키지 판매가 아닐 때
				strMessage.PrintF(_S( 780, "대상 캐릭터가 패키지 판매중이 아닙니다." ));		
				break;
			case MSG_PERSONALSHOP_ERROR_NOTFOUND:			// 대상 캐릭터가 없을 때
				strMessage.PrintF(_S( 781, "대상 캐릭터가 존재하지 않습니다." ));		
				break;				
			case MSG_PERSONALSHOP_ERROR_MONEY:				// 소지금 부족
				strMessage.PrintF(_S( 782, "소지금 부족하여 거래를 할 수 없습니다." ));		
				break;
			case MSG_PERSONALSHOP_ERROR_COUNT:			// 대상 개인상점보다 요청 판매/구매 카운트가 클때
				strMessage.PrintF(_S( 783, "요청한 아이템 개수가 올바르지 않습니다." ));		
				break;
			case MSG_PERSONALSHOP_ERROR_CANTPREMIUM:	// 소지금 부족으로 패키지 상점 개설 불가
				strMessage.PrintF(_S( 784, "소지금이 부족하여 패키지 상점을 개설할 수 없습니다." ));		
				break;
			// Date : 2005-02-01,   By Lee Ki-hwan
			case MSG_PERSONALSHOP_ERROR_BUY_FULLINVENTORY:	// 인벤토리가 가득 차서 물건을 구입할 수 없습니다.
				strMessage.PrintF( _S( 862, "인벤토리가 가득 차서 물건을 구입할 수 없습니다." ));		
				break;
			case MSG_PERSONALSHOP_ERROR_AREA:
				strMessage.PrintF(_S( 785, "개인상점을 개설할 수 없는 지역입니다." ));		
				break;
			case MSG_PERSONALSHOP_ERROR_ANYMORE_PET :
				strMessage.PrintF(_S( 2582,"펫 소유 제한이 넘어서 취소되었습니다." ));		
				break;
			case MSG_PERSONALSHOP_ERROR_NOSPACE :
				strMessage.PrintF(_S( 265 ,"인벤토리 공간이 부족합니다." ));
				break;
			case MSG_PERSONALSHOP_ERROR_CANNOT_USE:
				strMessage.PrintF(_S(5030, "이용할 수 없는 개인상점입니다."));
				break;
			}
			_pNetwork->ClientSystemMessage( strMessage, SYSMSG_ERROR );
			pUIManager->SetCSFlagOff( CSF_PERSONALSHOP );
		}
		break;
	case MSG_PERSONALSHOP_CHANGE:
		{
			LONG	lChaIndex;
			SBYTE	sbShopType;
			(*istr) >> lChaIndex;
			(*istr) >> sbShopType;

			//// 상점 상태 변화			: charindex(n) shoptype(c)
			if(lChaIndex == _pNetwork->MyCharacterInfo.index)
			{
				_pNetwork->MyCharacterInfo.sbShopType = sbShopType;
				if(sbShopType == PST_NOSHOP)
				{
					CTString strSysMessage;
					strSysMessage.PrintF(_S( 786, "상점을 닫았습니다." ));		
					_pNetwork->ClientSystemMessage( strSysMessage );
					
					pUIManager->SetCSFlagOff( CSF_PERSONALSHOP );
					pUIManager->GetPersonalShop()->ChangeShopState(FALSE);
					_pNetwork->MyCharacterInfo.ShopMsg.Reset();
					((CPlayerEntity*)CEntity::GetPlayerEntity(0))->StandDown( FALSE );
				}
			}
			// 타캐릭터의 상점 상태 변화.
			else
			{
				ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_CHARACTER, lChaIndex);

				if (pObject != NULL)
				{
					CCharacterTarget* pTarget = static_cast< CCharacterTarget* >(pObject);

					pTarget->cha_sbShopType = sbShopType;
						if( sbShopType == PST_NOSHOP )
						{
						pTarget->ShopMsg.Reset();
							if (pUIManager->GetPersonalShop()->EndBuyShop(lChaIndex))
							{
								CTString strSysMessage;
							strSysMessage.PrintF(_S(3075, "%s 님이 개인상점을 닫았습니다."), pTarget->m_strName.c_str());
								_pNetwork->ClientSystemMessage(strSysMessage);
							}
						}

					((CPlayerEntity*)CEntity::GetPlayerEntity(0))->SetShopData( pTarget->GetCIndex(), sbShopType );
				}				
			}
		}
		break;
	case MSG_PERSONALSHOP_SELL_LIST:
		{
			LONG	lChaIndex;
			(*istr) >> lChaIndex;			
			pUIManager->GetPersonalShop()->ReceivePersonalShopItemList(lChaIndex, &(*istr));
		}
		break;	
	case MSG_PERSONALSHOP_BUY:
		{
			pUIManager->GetPersonalShop()->ReceivePersonalShopItemUpdate(&(*istr));
		}
		break;
	case MSG_PERSONALSHOP_SELL_START:
		{
			// charindex(n) shoptype(c) shopname(str)
			LONG lChaIndex;
			SBYTE sbShopType;
			CTString strShopName;

			(*istr) >> lChaIndex;
			(*istr) >> sbShopType;
			(*istr) >> strShopName;

			if(lChaIndex == _pNetwork->MyCharacterInfo.index)
			{
				pUIManager->GetPersonalShop()->ChangeShopState(TRUE);
				((CPlayerEntity*)CEntity::GetPlayerEntity(0))->StandDown( TRUE );
				_pNetwork->MyCharacterInfo.sbShopType = sbShopType;
				_pNetwork->MyCharacterInfo.ShopMsg.SetChatMsg( strShopName );

				CTString strSysMessage;
				strSysMessage.PrintF( _S( 1037, "[%s] 상점을 열었습니다." ), strShopName);
				_pNetwork->ClientSystemMessage( strSysMessage );
				pUIManager->SetCSFlagOn( CSF_PERSONALSHOP );
			}
			else
			{
				ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_CHARACTER, lChaIndex);

				if (pObject != NULL)
				{
					CCharacterTarget* pTarget = static_cast< CCharacterTarget* >(pObject);

					pTarget->cha_sbShopType = sbShopType;
					pTarget->ShopMsg.SetChatMsg( strShopName );

					((CPlayerEntity*)CEntity::GetPlayerEntity(0))->SetShopData( pTarget->GetCIndex(), sbShopType );
					}
				}
			}
		break;
	}
}
// ----------------------------------------------------------------------------
// Name : ReceivePersonalShopMessage()
// Desc : 
// ----------------------------------------------------------------------------
void CSessionState::ReceiveCashPersonShopMessage(CNetworkMessage *istr)
{
	UBYTE	ubType;
	(*istr) >> ubType;	

	CUIManager* pUIManager = CUIManager::getSingleton();

	switch(ubType)
	{
	case MSG_ALTERNATEMERCHANT_ERROR:
		{
			SBYTE	sbError;
			(*istr) >> sbError;

			CTString strMessage;
			switch(sbError)
			{
			//case MSG_PERSONALSHOP_ERROR_OK:
			//	strMessage.PrintF(_S( 771, "정상" ));		
			//	break;
			case MSG_ALTERNATEMERCHANT_ERROR_INSUFF_SKILL:		// 스킬 사용
				strMessage.PrintF(_S( 772, "거래를 요청한 캐릭터가 스킬을 사용 중입니다." ));		
				break;
			case MSG_ALTERNATEMERCHANT_ERROR_INSUFF_WARP:		// 워프
				strMessage.PrintF(_S( 773, "거래를 요청한 캐릭터가 워프(이동장치) 중입니다." ));		
				break;
			case MSG_ALTERNATEMERCHANT_ERROR_INSUFF_EXCHANGE:	// 교환 중
				strMessage.PrintF(_S( 774, "거래를 요청한 캐릭터가 교환 중입니다." ));		
				break;
//			case MSG_ALTERNATEMERCHANT_ERROR_INSUFF_ALREADY:		// 상점 개설중
//				strMessage.PrintF(_S( 775, "거래를 요청한 캐릭터가 이미 개인상점 거래중입니다." ));		
//				break;
			case MSG_ALTERNATEMERCHANT_ERROR_INSUFF_MONEY:		// 나스를 개인상점으로 거래하고자 할때
				strMessage.PrintF(_S( 776, "나스는 개인상점에서 거래할 수 없습니다." ));		
				break;
			case MSG_ALTERNATEMERCHANT_ERROR_INSUFF_ETC:			// 상점을 열수 없는 상태(스킬사용/워프/패킷내용오류 등등)
				strMessage.PrintF(_S( 777, "개인상점 거래가 취소되었습니다." ));		
				break;
			case MSG_ALTERNATEMERCHANT_ERROR_DUPLICATION:		// 중복 등록
				strMessage.PrintF(_S( 778, "아이템이 중복 등록 되었습니다." ));		
				break;
			case MSG_ALTERNATEMERCHANT_ERROR_NOTSELL:			// 대상 캐릭터가 판매 상점이 아닐 때
				strMessage.PrintF(_S( 779, "대상 캐릭터가 판매 상점이 아닙니다." ));		
				break;
			case MSG_ALTERNATEMERCHANT_ERROR_NOTPACKAGE:			// 대상 캐릭터가 패키지 판매가 아닐 때
				strMessage.PrintF(_S( 780, "대상 캐릭터가 패키지 판매중이 아닙니다." ));		
				break;
			case MSG_ALTERNATEMERCHANT_ERROR_NOTFOUND:			// 대상 캐릭터가 없을 때
				strMessage.PrintF(_S( 781, "대상 캐릭터가 존재하지 않습니다." ));		
				break;				
			case MSG_ALTERNATEMERCHANT_ERROR_MONEY:				// 소지금 부족
				strMessage.PrintF(_S( 782, "소지금 부족하여 거래를 할 수 없습니다." ));		
				break;
			case MSG_ALTERNATEMERCHANT_ERROR_COUNT:			// 대상 개인상점보다 요청 판매/구매 카운트가 클때
				strMessage.PrintF(_S( 783, "요청한 아이템 개수가 올바르지 않습니다." ));		
				break;
			case MSG_ALTERNATEMERCHANT_ERROR_CANTPREMIUM:	// 소지금 부족으로 패키지 상점 개설 불가
				strMessage.PrintF(_S( 784, "소지금이 부족하여 패키지 상점을 개설할 수 없습니다." ));		
				break;
			// Date : 2005-02-01,   By Lee Ki-hwan
			case MSG_ALTERNATEMERCHANT_ERROR_BUY_FULLINVENTORY:	// 인벤토리가 가득 차서 물건을 구입할 수 없습니다.
				strMessage.PrintF( _S( 862, "인벤토리가 가득 차서 물건을 구입할 수 없습니다." ));		
				break;
			case MSG_ALTERNATEMERCHANT_ERROR_AREA:
				strMessage.PrintF(_S( 785, "개인상점을 개설할 수 없는 지역입니다." ));		
				break;
			case MSG_ALTERNATEMERCHANT_ERROR_ANYMORE_PET :
				strMessage.PrintF(_S( 2582,"펫 소유 제한이 넘어서 취소되었습니다." ));		
				break;
			case MSG_ALTERNATEMERCHANT_ERROR_NOSPACE :
				strMessage.PrintF(_S( 265 ,"인벤토리 공간이 부족합니다." ));
				break;
			case MSG_ALTERNATEMERCHANT_ERROR_NOCOLLECT_ITEM:
				strMessage.PrintF(_s("회수 되지 않은 아이템이 존재합니다."));
				break;
			case  MSG_ALTERNATEMERCHANT_ERROR_TIMEOVER:
				strMessage.PrintF(_s("판매 대행 시간이 만료 되었습니다. 판매중인 아이템은 판매 대행협회 회장님께 회수 하시기 바랍니다."));
				break;

			}
			_pNetwork->ClientSystemMessage( strMessage, SYSMSG_ERROR );
			pUIManager->SetCSFlagOff( CSF_PERSONALSHOP );
		}
		break;
	case MSG_ALTERNATEMERCHANT_CHANGE:
		{
			LONG	lChaIndex;
			SBYTE	sbShopType;
			(*istr) >> lChaIndex;
			(*istr) >> sbShopType;

			//// 상점 상태 변화			: charindex(n) shoptype(c)
			if(lChaIndex == _pNetwork->MyCharacterInfo.index)
			{
				_pNetwork->MyCharacterInfo.sbShopType = sbShopType;
				if(sbShopType == PST_NOSHOP)
				{
					CTString strSysMessage;
					strSysMessage.PrintF(_S( 786, "상점을 닫았습니다." ));		
					_pNetwork->ClientSystemMessage( strSysMessage );
					
					pUIManager->SetCSFlagOff( CSF_PERSONALSHOP );
					pUIManager->GetPersonalShop()->ChangeShopState(FALSE);
					_pNetwork->MyCharacterInfo.ShopMsg.Reset();
					((CPlayerEntity*)CEntity::GetPlayerEntity(0))->StandDown( FALSE );
				}
			}
			// 타캐릭터의 상점 상태 변화.
			else
			{
				ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_CHARACTER, lChaIndex);

				if (pObject != NULL)
				{
					CCharacterTarget* pTarget = static_cast< CCharacterTarget* >(pObject);

					pTarget->cha_sbShopType = sbShopType;

						if( sbShopType == PST_NOSHOP )
						{
						pTarget->ShopMsg.Reset();
							if (pUIManager->GetPersonalShop()->EndBuyShop(lChaIndex))
							{
								CTString strSysMessage;
							strSysMessage.PrintF(_S(3075, "%s 님이 개인상점을 닫았습니다."), pTarget->m_strName.c_str());
								_pNetwork->ClientSystemMessage(strSysMessage);
							}
						}

					((CPlayerEntity*)CEntity::GetPlayerEntity(0))->SetShopData( pTarget->GetCIndex(), sbShopType );
				}				
			}
		}
		break;
	case MSG_ALTERNATEMERCHANT_SELL_LIST:
		{
			LONG	lChaIndex;
			(*istr) >> lChaIndex;			
			pUIManager->GetPersonalShop()->ReceivePersonalShopItemList(lChaIndex, &(*istr));
		}
		break;	
	case MSG_ALTERNATEMERCHANT_BUY:
		{
			pUIManager->GetPersonalShop()->ReceivePersonalShopItemUpdate(&(*istr));
		}
		break;
	case MSG_ALTERNATEMERCHANT_SELL_START:
		{
			// charindex(n) shoptype(c) shopname(str)
			LONG lChaIndex;
			SBYTE sbShopType;
			CTString strShopName;

			(*istr) >> lChaIndex;
			(*istr) >> sbShopType;
			(*istr) >> strShopName;

			if(lChaIndex == _pNetwork->MyCharacterInfo.index)
			{
				pUIManager->GetPersonalShop()->ChangeShopState(TRUE);
				((CPlayerEntity*)CEntity::GetPlayerEntity(0))->StandDown( TRUE );
				_pNetwork->MyCharacterInfo.sbShopType = sbShopType;
				_pNetwork->MyCharacterInfo.ShopMsg.SetChatMsg( strShopName );

				CTString strSysMessage;
				strSysMessage.PrintF( _S( 1037, "[%s] 상점을 열었습니다." ), strShopName);
				_pNetwork->ClientSystemMessage( strSysMessage );
				pUIManager->SetCSFlagOn( CSF_PERSONALSHOP );
			}
			else
			{
				ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_CHARACTER, lChaIndex);

				if (pObject != NULL)
				{
					CCharacterTarget* pTarget = static_cast< CCharacterTarget* >(pObject);

					pTarget->cha_sbShopType = sbShopType;
					pTarget->ShopMsg.SetChatMsg( strShopName );

					((CPlayerEntity*)CEntity::GetPlayerEntity(0))->SetShopData( pTarget->GetCIndex(), sbShopType );
					}
				}
			}
		break;
	}
}

// ----------------------------------------------------------------------------
// Name : SetRecentServer()
// Desc : 
// ----------------------------------------------------------------------------
void CSessionState::SetRecentServer(int iRecentGroup, int iRecentServer)
{
	m_iRecentGroup	= iRecentGroup;
	m_iRecentServer = iRecentServer;
}

// ----------------------------------------------------------------------------
// Name : ReceiveLegitimateMessage()
// Desc : 
// ----------------------------------------------------------------------------
void CSessionState::ReceiveLegitimateMessage(CNetworkMessage *istr)
{
	UBYTE	ubType;		
	SBYTE	targettype;
	SLONG	targetindex;	

	(*istr) >> ubType;
	(*istr) >> targettype;
	(*istr) >> targetindex;	

	if(targettype != MSG_CHAR_PC)
	{
		return;
	}	

	((CPlayerEntity*)CEntity::GetPlayerEntity(0))->SetLegit(ubType,targetindex);
}

// ----------------------------------------------------------------------------
// Name : ReceiveWareHouseMessage()
// Desc : 
// ----------------------------------------------------------------------------
void CSessionState::ReceiveWareHouseMessage(CNetworkMessage *istr)
{
	UBYTE	ubType;
	(*istr) >> ubType;

	CUIManager* pUIManager = CUIManager::getSingleton();

	switch(ubType)
	{
	case MSG_STASH_ERROR_CODE:
		{
			SBYTE	sbError;
			(*istr) >> sbError;

			CTString strMessage;
			switch(sbError)
			{
			case MSG_STASH_ERROR_OK:						// 정상
			//	strMessage.PrintF( _S( 771, "정상" ));		
				return;
				break;
			case MSG_STASH_ERROR_CANNOT_STATE:				
				strMessage.PrintF(_S( 850, "창고 이용을 할 수 없는 상태입니다." ));		
				break;
			case MSG_STASH_ERROR_NOTCOMPLETE:				// 이전 창고 요청이 마무리 되지 않았음
				strMessage.PrintF(_S( 851, "이전의 창고 요청이 마무리 되지 않았습니다." ));		
				break;
			case MSG_STASH_ERROR_SEALED:					// 창고 긴급 잠김 상태
				strMessage.PrintF(_S( 852, "창고가 긴급 잠김 상태입니다." ));	
				break;
			case MSG_STASH_ERROR_WRONG_PASSWORD:			// 암호 틀림
				strMessage.PrintF(_S( 853, "암호가 올바르지 않습니다." ));	
				break;
			case MSG_STASH_ERROR_WRONG_PACKET:				// 패킷 내용 오류
				strMessage.PrintF( _S( 3257, "요청한 작업이 실패 하였습니다.") );	
				break;
			case MSG_STASH_ERROR_MONEY:						// 돈 부족
				strMessage = _S( 306, "나스가 부족합니다." );
				break;
			case MSG_STASH_ERROR_STASH_FULL:				// 창고 가득 참
				strMessage.PrintF(_S( 855, "창고가 가득 찼습니다." ));	
				break;
			case MSG_STASH_ERROR_INVENTORY_FULL:			// 인벤토리 개수 초과로 찾을 수 없음
				strMessage.PrintF(_S( 857, "인벤토리가 꽉 차서 찾을 수 없습니다." ));		
				break;
			case MSG_STASH_ERROR_WRONG_CHAR:				// 암호에 잘못된 문자가 들어감
				strMessage.PrintF(_S( 809, "암호에 잘못된 문자가 들어가 있습니다." ));	
				break;
			case MSG_STASH_ERROR_CANNOT_SEAL:				// 잠글수 없음
				strMessage.PrintF(_S( 810, "잠글 수 없습니다." ));		
				break;
			case MSG_STASH_ERROR_ALREADY_SEAL:				// 이미 잠김
				strMessage.PrintF(_S( 811, "이미 잠겨있습니다." ));	
				break;
			}

			pUIManager->CloseMessageBox(MSGCMD_WAREHOUSE_ERROR);
			CUIMsgBox_Info	MsgBoxInfo;
			MsgBoxInfo.SetMsgBoxInfo( _S( 823, "창고" ), UMBS_OK, UI_WAREHOUSE, MSGCMD_WAREHOUSE_ERROR );	
			MsgBoxInfo.AddString( strMessage );
			pUIManager->CreateMessageBox( MsgBoxInfo );			

			pUIManager->GetWareHouse()->ResetWareHouse();
			pUIManager->GetSecurity()->ResetSecurity();
		}
		break;

	case MSG_STASH_KEEP_REP:
		{
			// 맡기기 결과				: keepcount(n) [itemdbindex(n) count(ll)]:keepcount
			LONG lKeepCount;
			CTString strMessage;
			(*istr) >> lKeepCount;
			for(int i = 0; i < lKeepCount; ++i)
			{
				LONG	lItemDBIndex;
				SQUAD	llCount;
				(*istr) >> lItemDBIndex;
				(*istr) >> llCount;
				//const char* szName = _pNetwork->GetItemData( lItemDBIndex ).GetName();
				const char* szName = _pNetwork->GetItemName( lItemDBIndex );
				CItemData* pItemData = _pNetwork->GetItemData( lItemDBIndex );
				if( pItemData->GetType() == CItemData::ITEM_ETC &&
					pItemData->GetSubType() == CItemData::ITEM_ETC_MONEY )
				{
					strMessage.PrintF( _S( 1346, "나스를 %I64d개 보관하였습니다." ), llCount);		
				}
				else
				{
					strMessage.PrintF(_S( 808, "%s를 %I64d개 보관하였습니다." ), szName, llCount);
				}
				_pNetwork->ClientSystemMessage( strMessage );
			}
			//pUIManager->SetCSFlagOff( CSF_WAREHOUSE );
			pUIManager->GetWareHouse()->ResetWareHouse();
		}
		break;

	case MSG_STASH_TAKE_REP:
		{
			// 찾기 결과				: takecount(n) [itemdbindex(n) count(ll)]:keepcount
			LONG lTakeCount;
			CTString strMessage;
			(*istr) >> lTakeCount;
			for(int i = 0; i < lTakeCount; ++i)
			{
				LONG	lItemDBIndex;
				SQUAD	llCount;
				(*istr) >> lItemDBIndex;
				(*istr) >> llCount;
				//const char* szName = _pNetwork->GetItemData( lItemDBIndex ).GetName();
				//strMessage.PrintF("%s를 %I64d개 찾았습니다.", szName, llCount);
				//_pNetwork->ClientSystemMessage( strMessage );

			}
			//pUIManager->SetCSFlagOff( CSF_WAREHOUSE );
			pUIManager->GetWareHouse()->ResetWareHouse();
		}
		break;
#ifdef	STASH_PASSWORD		
	case MSG_STASH_DELETE_PASSWORD:
		{
			ResponseClient::doStashDeletePassword* pPack = reinterpret_cast<ResponseClient::doStashDeletePassword*>(istr->GetBuffer());

			pUIManager->GetSecurity()->ShowMsgBoxDelPW(pPack->result);			
		}
		break;
#endif	// STASH_PASSWORD

	case MSG_STASH_CHECK_PASSWORD_REP:		// 암호 확인 결과.
		{
			// 암호가 맞음...
			pUIManager->CloseMessageBox( MSGCMD_NEW_PASSWORD );
			CUIMsgBox_Info	MsgBoxInfo;
			MsgBoxInfo.SetMsgBoxInfo( _S( 1727, "보안 입력" ), UMBS_OK | UMBS_INPUTPASSWORD, UI_SECURITY, MSGCMD_NEW_PASSWORD );		
			CTString	strMessage =  _S( 1738, "새 암호를 입력하여 주십시오." );	
			MsgBoxInfo.AddStringEx( strMessage, 0, 0, 0xF2F2F2FF, TEXT_CENTER );
			strMessage =  _S( 1729, "8자(영문 숫자조합)." );	
			MsgBoxInfo.AddStringEx( strMessage, 1, 0, 0xF2F2F2FF, TEXT_CENTER );
			pUIManager->CreateMessageBox( MsgBoxInfo );
			
			CUIMessageBox* pMsgBox = pUIManager->GetMessageBox( MSGCMD_NEW_PASSWORD );
			ASSERT( pMsgBox != NULL && "Invalid Message Box!!!" );
			//pMsgBox->GetInputBox().SetMaxChar(MAX_PASSWORD);
			pMsgBox->GetInputBox().SetMaxChar(8);
		}
		break;

	case MSG_STASH_SETTING_NEWPASSWORD_REP:	// 암호 설정 결과.
		{
			pUIManager->GetSecurity()->ResetSecurity();

			// 암호 설정 결과.
			SBYTE sbResult;
			(*istr) >> sbResult;
			if(sbResult == 0)
			{
				pUIManager->CloseMessageBox( MSGCMD_SECURITY_NOTIFY );
				CUIMsgBox_Info	MsgBoxInfo;
				MsgBoxInfo.SetMsgBoxInfo(  _S( 1774, "암호 설정" ) , UMBS_OK, UI_SECURITY, MSGCMD_SECURITY_NOTIFY );	
				CTString	strMessage = _S( 1775, "암호가 설정되었습니다. 창고 보안 기능이 활성화됩니다." );	
				MsgBoxInfo.AddString( strMessage );
				pUIManager->CreateMessageBox( MsgBoxInfo );
			}
			else if(sbResult == 1)
			{
				pUIManager->CloseMessageBox( MSGCMD_SECURITY_NOTIFY );
				CUIMsgBox_Info	MsgBoxInfo;
				MsgBoxInfo.SetMsgBoxInfo(  _S( 1774, "암호 설정" ) , UMBS_OK, UI_SECURITY, MSGCMD_SECURITY_NOTIFY );	
				CTString	strMessage = _S( 1776, "적절하지 않은 암호입니다." );	
				MsgBoxInfo.AddString( strMessage );
				pUIManager->CreateMessageBox( MsgBoxInfo );
			}
			else if(sbResult == 2)
			{
			}
		}
		break;	

	case MSG_STASH_SEAL:
		{
			pUIManager->GetSecurity()->ResetSecurity();
		}
		break;
	}
}

// ----------------------------------------------------------------------------
// Name : ReceiveGuildMessage()
// Desc : 
// ----------------------------------------------------------------------------
void CSessionState::ReceiveGuildMessage(CNetworkMessage *istr)
{
	UBYTE	ubType;
	(*istr) >> ubType;

	CUIManager* pUIManager = CUIManager::getSingleton();

	switch(ubType)
	{
	case MSG_GUILD_ERROR:
		{
			SBYTE	sbError;
			(*istr) >> sbError;
			
			CTString strMessage;
			switch(sbError)
			{
			//case MSG_GUILD_ERROR_OK:
			//	strMessage.PrintF( _S( 963, "정상" ) );		
			//	break;
			
			// 길드 배틀 관련 에러 처리 부분
			// UIGuildBattle에서 한꺼번에 처리 합니다.
			case MSG_GUILD_ERROR_BATTLE:
				{
					SBYTE	sbError;
					(*istr) >> sbError;	
					pUIManager->GetGuildBattle()->GBErrorProcess( sbError );
				}
				return;
			case MSG_GUILD_ERROR_PACKET:
				ASSERTALWAYS("호출되면 안되는 부분");
				break;
			case MSG_GUILD_ERROR_CHARLEVEL:
				strMessage.PrintF(  _S( 965, "레벨이 부족합니다." )  );		
				break;
			case MSG_GUILD_ERRRO_SKILLPOINT:
				strMessage.PrintF(  _S( 966, "SP가 부족합니다." )  );		
				break;
			case MSG_GUILD_ERROR_MONEY:
				strMessage.PrintF(  _S( 967, "나스가 부족합니다." )  );		
				break;
			case MSG_GUILD_ERROR_ALREADYBOSS:
				strMessage.PrintF(  _S( 968, "다른 길드의 (부)장이기 때문에 길드를 생성할 수 없습니다." ) );		
				break;			
			case MSG_GUILD_ERROR_DUPNAME:
				strMessage.PrintF(  _S( 969, "이미 존재하는 길드 이름입니다." )  );		
				break;
			case MSG_GUILD_ERROR_GUILDNAME:
				strMessage.PrintF(  _S( 970, "길드 이름에 오류가 있습니다." )  );		
				break;
			case MSG_GUILD_ERROR_GAMESERVER:
				strMessage.PrintF(  _S( 971, "게임 서버의 오류로 인해서 길드를 생성할 수 없습니다." )  );		
				break;
			case MSG_GUILD_ERROR_NOGUILD:
				strMessage.PrintF(  _S( 972, "길드에 속해있지 않습니다." )  );		
				break;
			case MSG_GUILD_ERROR_NOTBOSS:
				strMessage.PrintF(  _S( 973, "권한이 없습니다." )  );		
				break;
			case MSG_GUILD_ERROR_MAXLEVEL:
				strMessage.PrintF(  _S( 974, "더 이상 길드의 등급을 높일 수 없습니다." )  );		
				break;
			case MSG_GUILD_ERROR_CREATE_OK:
				strMessage.PrintF(  _S( 975, "길드가 생성 되었습니다." )  );		
				break;
				//return;
			case MSG_GUILD_ERROR_LEVELUP_OK:
				strMessage.PrintF(  _S( 976, "길드가 승급 되었습니다." )  );		
				break;
				//return;
			case MSG_GUILD_ERROR_BREAKUP_OK:
				strMessage.PrintF(  _S( 977, "길드가 해산 되었습니다." )  );		
				break;
				//return;
			case MSG_GUILD_ERROR_TARGET_NOBOSS:
				strMessage.PrintF(  _S( 978, "대상이 길드장이 아니어서 가입 신청을 할 수 없습니다." )  );		
				break;
			case MSG_GUILD_ERROR_TARGET_BUSY:
				strMessage.PrintF(  _S( 979, "길드장이 다른 요청을 처리하고 있습니다. 다시 시도하십시요." )  );		
			_pNetwork->GuildApplicantReject( FALSE );
				break;
			case MSG_GUILD_ERROR_SOURCE_BUSY:
				strMessage.PrintF(  _S( 980, "길드 가입 신청중에 다른 길드에 가입 신청을 할 수 없습니다." )  );		
				_pNetwork->GuildApplicantReject( FALSE );
				break;
			case MSG_GUILD_ERROR_FULLMEMBER:					// 길드 정원이 초과
				strMessage.PrintF( _S( 981, "길드에 정원이 초과 하여 가입을 할 수 없습니다." )  );		
				break;
			case MSG_GUILD_ERROR_REGIST_OK:					// 길드원 가입 완료
				{				
				pUIManager->CloseMessageBox(MSGCMD_GUILD_JOIN);
				pUIManager->CloseMessageBox(MSGCMD_GUILD_JOIN_REQ);
				pUIManager->CloseMessageBox(MSGCMD_GUILD_APPLICANT_JOIN);
				strMessage.PrintF( _S( 982, "길드에 가입되었습니다." )  );		
				break;
				}
				//return;
			case MSG_GUILD_ERROR_OUTBOSS:					// 길드장이 탈퇴하려면 뷁함
				strMessage.PrintF( _S( 983, "길드장은 탈퇴할 수 없습니다." )  );		
				break;
			case MSG_GUILD_ERROR_OUT_OK:						// 탈퇴 성공
				pUIManager->GetGuild()->ReceiveCorpsInfo( istr );
				strMessage.PrintF( _S( 984, "길드 탈퇴가 완료되었습니다." )  );		
				
				MY_INFO()->_guildmark.remove();
				UIMGR()->SetGuildMark();
				break;
				//return;
			case MSG_GUILD_ERROR_NOTFOUNDMEMBER:				// 강퇴, 임명, 이임등에서 대상 길원이 없을때
				strMessage.PrintF( _S( 985, "해당 길드원을 찾을 수 없습니다." )  );		
				break;
			case MSG_GUILD_ERROR_CANTKICKBOSS:				// 길장은 당퇴 불능
				strMessage.PrintF( _S( 986, "길드(부)장은 퇴출 대상이 아닙니다." ) );		
				break;
			case MSG_GUILD_ERROR_KICK_OK:					// 퇴출 성공
				pUIManager->GetGuild()->ReceiveCorpsInfo( istr );
				strMessage.PrintF( _S( 987, "길드원이 퇴출 되었습니다." )  );		
				break;
				//return;
			case MSG_GUILD_ERROR_CHANGE_BOSS_OK:
				strMessage.PrintF( _S( 988, "길드장이 이임 되었습니다." )  );		
				break;
				//return;
			case MSG_GUILD_ERROR_CANTOFFICER:				// 길드장이나 부관을 다시 부관으로 임명시
				strMessage.PrintF( _S( 989, "해당 길드원을 길드부장으로 임명할 수 없습니다." )  );		
				break;
			case MSG_GUILD_ERROR_APPOINT_OFFICER_OK:		// 부관 임명 성공
				strMessage.PrintF( _S( 990, "길드부장이 임명 되었습니다." )  );		
				break;
				//return;
			case MSG_GUILD_ERROR_FULLOFFICER:				// 부관 가득 참
				strMessage.PrintF( _S( 991, "더 이상 길드부장을 임명할 수 없습니다." )  );		
				break;
			case MSG_GUILD_ERROR_NOTOFFICER:					// 부관이 아니라서 해임 불가
				strMessage.PrintF( _S( 992, "길드부장이 아니라서 해임할 수 없습니다." )  );		
				break;
			case MSG_GUILD_ERROR_FIRE_OFFICER_OK:			// 해임 성공S
				strMessage.PrintF( _S( 993, "해임이 성공하였습니다." )  );		
				break;
				//return;
			case MSG_GUILD_ERROR_REGDELAY:					// 탈퇴후 7일이 지나지 않아 가입 불능
				strMessage.PrintF( _S( 994, "탈퇴 후 7일이 지나지 않아서 길드에 가입할 수 없습니다." )  );		
				break;
			case MSG_GUILD_ERROR_CANTKICK:
				strMessage.PrintF( _S( 995, "길드부장이 길드부장을 퇴출할 수 없습니다." )  );		
				break;
			case MSG_GUILD_ERROR_CANNOT_REGIST_REQ_BATTLE:	// 길드전 중에는 가입신청 불가 
				strMessage.PrintF( _S( 1830, "가입하려는 길드가 길드 전쟁중일 때는 가입신청을 할 수 없습니다." )  ); 
				break;
			case MSG_GUILD_ERROR_CANNOT_BREAK_BATTLE:		// 갈드전 - 길드전 중 길드 해체 불가
				strMessage.PrintF( _S( 1831, "길드 전쟁중일 때는 길드를 해체할 수 없습니다." )  );	
				break;
			case MSG_GUILD_ERROR_CANNOT_KICK_BATTLE:		// 갈드전 - 길드전 중 퇴출 불가   
				strMessage.PrintF( _S( 1832, "길드 전쟁중일 때는 길드원을 퇴출할 수 없습니다." )  ); 
				break;
			case MSG_GUILD_ERROR_CANNOT_OUT_BATTLE:
				strMessage.PrintF( _S( 1833, "길드 전쟁중일 때는 길드를 탈퇴할 수 없습니다." )  );	 
				break;
			case MSG_GUILD_ERROR_CANNOT_BREAK_WAR:
				strMessage.PrintF( _S( 2077, "성주 길드는 길드체를 할 수 없습니다." )  );		
				break;
			// WSS_NEW_GUILD_SYSTEM 070704
			// ---------------------------------------------------------------->>
			case MSG_NEW_GUILD_ERRROR_GUILDPOINT:				// 길드 레벨업시 길드 포인트 부족 
				strMessage.PrintF( _S(3884, "길드 포인트가 부족합니다." )  );		
				break;

			case MSG_NEW_GUILD_ERROR_SAME_INCLNE:				// 길드 성향이 같음
				strMessage.PrintF( _S(3885, "길드 성향이 같습니다." )  );		
				break;
			case MSG_NEW_GUILD_ERROR_CHANG_INCLINE:				// 길드 성향 변경 실패
				strMessage.PrintF( _S(3886, "길드 성향 변경에 실패하였습니다." )  );		
				break;
			case MSG_NEW_GUILD_ERROR_INCLINE_OK:				// 길드 성향 변경 성공
				strMessage.PrintF( _S(3887, "길드 성향이 변경되었습니다." )  );		
				break;
			case MSG_NEW_GUILD_ERROR_ADJUST_FAIL:				// 길드 멤버들의  설정 변경 실패
				strMessage.PrintF( _S(3888, "길드 설정 적용에 실패하였습니다." )  );
				break;
			case MSG_NEW_GUILD_ERROR_ADJUST_OK:	
				pUIManager->GetGuild()->AdjustGuildMemberInfo();
				pUIManager->GetGuild()->ReceiveCorpsInfo( istr );
				strMessage.PrintF( _S(3889, "길드 설정이 적용되었습니다." )  );
				break;		
			case MSG_NEW_GUILD_NOTICE_UPDATE_ERROR_OK:			// 새로운 길드 공지 업데이트 성공
				strMessage.PrintF( _S(3890, "길드 공지 업데이트가 완료되었습니다." )  );		
				break;
			case MSG_NEW_GUILD_NOTICE_UPDATE_ERROR_FAIL:		// 새로운 길드 공지 업데이트 실패
				strMessage.PrintF( _S(3891, "길드 공지 업데이트에 실패하였습니다." )  );		
				break;
			case MSS_NEW_GUILD_NOTICE_TRANSMISSION_ERROR_OK:	
				strMessage.PrintF( _S(3892, "길드 공지 메시지를 보냈습니다." )  );
				break;
			case MSS_NEW_GUILD_NOTICE_TRANSMISSION_ERROR_FAIL:
				strMessage.PrintF( _S(3893 , "길드 공지 메시지 전달에 실패하였습니다." )  );
				break;		
			case MSG_NEW_GUILD_SKILL_ERROR_NOTEXIST:	
				strMessage.PrintF( _S(3894, "길드 스킬이 없습니다." )  );
				break;
			case MSG_NEW_GUILD_NOTICE_ERROR_NOT_MESSAGE:			
				strMessage.PrintF( _S(3895, "전달 메시지가 없습니다." )  );
				break;			
			case MSG_NEW_GUILD_SKILL_LEARN_ERROR_OK:
				strMessage.PrintF( _S(3896, "길드 스킬을 습득하였습니다." )  );
				break;			
			case MSG_NEW_GUILD_SKILL_LEARN_ERROR_LEVEL:  
				strMessage.PrintF( _S(3897, "길드 레벨이 부족합니다." )  );
				break;
			case MSG_NEW_GUILD_SKILL_ALEADY_LEARN: 
				strMessage.PrintF( _S(3897, "길드 레벨이 부족합니다." )  );
				break;
			case MSG_GUILD_GRADE_EX_ERROR_WARCASTLE:
				strMessage.PrintF( _S( 5332, "공성중에는 할 수 없는 행동입니다." )  );
				break;
			case MSG_GUILD_GRADE_EX_ERROR_COUNT_OEVER:
				strMessage.PrintF( _S( 5333, "지정할 수 있는 인원 수를 초과하였습니다." )  );
				break;
			case MSG_GUILD_ERROR_CANNOT_BOSSCHANGE_FAIL:
				strMessage.PrintF( _S( 5360, "길드(부)장을 이임할 수 없습니다." )  );
				break;
			case MSG_NEW_GUILD_ERROR_SYSTEM:
				strMessage.PrintF( _S( 5513, "길드 마크 설정에 오류가 발생하였습니다." )  );
				break;
			case MSG_NEW_GUILD_ERROR_OK_MARK_WND:
				pUIManager->GetGuildMark()->OpenGuildMarkEdit();
				return;
// ==> 1107 길드 시스템 개편 [trylord 11/12/28]
			case MSG_NEW_GUILD_ERROR_MASTER_KICK_RESERVE:
				strMessage.PrintF( _S( 5618, "길드장 해임 대기 상태입니다." ));
				break;
			case MSG_NEW_GUILD_ERROR_MASTER_KICK_LAST_CONNECT:
				strMessage.PrintF( _S( 5619, "길드장 최근 접속 종료일이 7일이 넘지 않아 해임할 수 없습니다." ) );
				break;
			case MSG_NEW_GUILD_ERROR_MASTER_KICK_LAST_CHANGE:
				strMessage.PrintF( _S( 5620, "길드장 최근 변경 일이 7일이 넘지 않아 해임할 수 없습니다." ) );
				break;
			case MSG_NEW_GUILD_ERROR_MASTER_KICK_CANCEL_NOT_RESERVE:
				strMessage.PrintF( _S( 5621, "해임 대기 상태가 아니기 때문에 취소할 수 없습니다." ) );
				break;
			case MSG_NEW_GUILD_ERROR_MASTER_KICK_DATA:
				strMessage.PrintF( _S( 5622, "길드 데이터 오류" ) );
				break;
			// [2012/05/24 : Sora] ITS 7137 길드 에러 메지시 누락으로 추가
			case MSG_NEW_GUILD_ERROR_NOT_EXIST_CHAR:
				strMessage.PrintF(_S(11, "존재하지 않는 캐릭터 입니다."));
				break;
			case MSG_NEW_GUILD_ERROR_EXIST_GUILD:
				strMessage.PrintF(_S( 866, "이미 길드에 가입되어 있습니다.\n먼저 가입되어 있는 길드에서 탈퇴하여 주십시오." ));
				break;
			case MSG_GUILD_ERROR_CANNOT_REGIST_REQ_SERVERTRANS:
				break;
			case MSG_GUILD_ERROR_NOT_EXIST_NPC:
				strMessage.PrintF(_S( 5913, "수행가능한 NPC가 주위에 존재하지 않습니다." ));
				break;
			case MSG_GUILD_ERROR_CANNOT_REGISTER_REQ_ZONE:
				strMessage.PrintF(_S( 6090, "분쟁 지역에서 적대 관계인 경우 사용할 수 없는 기능입니다." ));
				break;
			case MSG_GUILD_ERROR_ITEM:
				strMessage.PrintF(_S( 280, "아이템이 존재하지 않아 스킬을 습득할 수 없습니다." ));
				break;
// <== 1107 길드 시스템 개편 [trylord 11/12/28]
			// ----------------------------------------------------------------<<
			}
			pUIManager->CloseMessageBox(MSGCMD_GUILD_ERROR);
			pUIManager->CloseMessageBox(MSGCMD_GUILD_DESTROY);
			pUIManager->CloseMessageBox(MSGCMD_GUILD_JOIN);
			pUIManager->CloseMessageBox(MSGCMD_GUILD_JOIN_REQ);
			pUIManager->CloseMessageBox(MSGCMD_GUILD_QUIT);
			pUIManager->CloseMessageBox(MSGCMD_GUILD_QUIT_CONFIRM);
			pUIManager->CloseMessageBox(MSGCMD_GUILD_APPLICANT_JOIN);
			pUIManager->CloseMessageBox(MSGCMD_GUILD_KICK);						// 멤버 퇴출
			pUIManager->CloseMessageBox(MSGCMD_GUILD_ADD_VICE_BOSS);				// 부단장 임명
			pUIManager->CloseMessageBox(MSGCMD_GUILD_DEL_VICE_BOSS);				// 부단장 퇴출
			pUIManager->CloseMessageBox(MSGCMD_GUILD_CHANGE_BOSS);					// 단장 이임
			
			CUIMsgBox_Info	MsgBoxInfo;
			MsgBoxInfo.SetMsgBoxInfo(  _S( 865, "길드" ) , UMBS_OK, UI_GUILD, MSGCMD_GUILD_ERROR );		
			MsgBoxInfo.AddString( strMessage );
			pUIManager->CreateMessageBox( MsgBoxInfo );		
			//pUIManager->GetGuild()->ResetGuild();
		}
		break;

	case MSG_GUILD_CREATE:		// 길드 생성
		{
			CPrintF("길드 생성\n");
		}
		break;

	case MSG_GUILD_INFO:		// 내 길드 정보 보내기
		{
			//MSG_GUILD_INFO,	// 내 길드 정보 보내기	: guildindex(n) guildname(str) guildlevel(n) mypos(n)
								// 길드 없으면 guildindex == -1, 이후 항목 생략
			LONG		lGuildIndex;			
			(*istr) >> lGuildIndex;

			_pNetwork->MyCharacterInfo.lGuildIndex = lGuildIndex;

			// 길드에 속해있지 않음.
			if( lGuildIndex == -1 )
			{
			}
			else
			{
				CTString	strGuildName, strGuildClassName;
				LONG		lGuildLevel;
				LONG		lMyPos;
				(*istr) >> strGuildName;
				(*istr) >> lGuildLevel;
				(*istr) >> lMyPos;
				(*istr) >> _pNetwork->MyCharacterInfo.sbGuildRank;	
				(*istr) >> strGuildClassName;
// WSS_NEW_GUILD_SYSTEM 070731
				SBYTE		sbGuildSkill444;	// 길드원 확장 스킬
				(*istr) >> sbGuildSkill444;
				pUIManager->GetGuild()->ResetGuildMaxMember((int)sbGuildSkill444);				

				_pNetwork->MyCharacterInfo.lGuildLevel		= lGuildLevel;
				_pNetwork->MyCharacterInfo.lGuildPosition	= lMyPos;
				_pNetwork->MyCharacterInfo.strGuildName		= strGuildName;
				_pNetwork->MyCharacterInfo.guildPosName		= strGuildClassName;

// [sora] GUILD_MARK
#ifdef GUILD_MARK
				SBYTE gm_row, gm_col, bg_row, bg_col;

				(*istr) >> gm_row;
				(*istr) >> gm_col;
				(*istr) >> bg_row;
				(*istr) >> bg_col;

				MY_INFO()->_guildmark.setData(gm_row, gm_col, bg_row, bg_col);
				UIMGR()->SetGuildMark();
#endif
#ifdef	UPDATE1107_GUILDSYSTEM	// 1107 길드 시스템 개편 [trylord 11/12/28]
				LONG	lGuildMStatus	= 0;

				(*istr) >> lGuildMStatus;
				_pNetwork->MyCharacterInfo.iGuildMasterKickStatus	= (int)lGuildMStatus;

				if(_pNetwork->MyCharacterInfo.iGuildMasterKickStatus == 1)
				{
					pUIManager->CloseMessageBox(MSGCMD_GUILD_ERROR);

					CTString		strMessage;
					CUIMsgBox_Info	MsgBoxInfo;

					strMessage.PrintF(_S(5631, "[%s] 길드의 길드장의 부재로 길드장 직위가 해제될 예정입니다."), _pNetwork->MyCharacterInfo.strGuildName);
					MsgBoxInfo.SetMsgBoxInfo(  _S( 865, "길드" ) , UMBS_OK, UI_GUILD, MSGCMD_GUILD_ERROR );		// 확인만 하면 되므로 MSGCMD_GUILD_ERROR ㄱㄱㅅ
					MsgBoxInfo.AddString(strMessage);
					pUIManager->CreateMessageBox( MsgBoxInfo );	
				}
#endif	// #ifdef	UPDATE1107_GUILDSYSTEM

			}
		}
		break;
		
	case MSG_GUILD_MEMBERLIST:	// 길드원 리스트
		{
			// 길원 리스트			: guildindex(n) count(n) [index(n) name(str) position(n) online(c)]:count
			LONG lGuildIndex;
			LONG lCount;
			(*istr) >> lGuildIndex;
			(*istr) >> lCount;
			
			if( _pNetwork->MyCharacterInfo.lGuildIndex == lGuildIndex )
			{
				pUIManager->GetGuild()->ResetGuild();
				pUIManager->GetGuild()->ClearMemberList();
			}
			
			for(int i = 0; i < lCount; ++i)
			{
				LONG		lIndex;
				CTString	strName;
				LONG		lPosition;
				SBYTE		sbOnline;
				
				(*istr) >> lIndex;
				(*istr) >> strName;
				(*istr) >> lPosition;
				(*istr) >> sbOnline;
				
				if( _pNetwork->MyCharacterInfo.lGuildIndex == lGuildIndex )
				{
					pUIManager->GetGuild()->AddToMemberList( lIndex, strName, lPosition, sbOnline );
				}
						}
					}
		break;
		
	case MSG_GUILD_ONLINE:		// 길드원 온라인 상태
		{
			LONG		lGuildIndex;
			LONG		lIndex;
			CTString	strName;
			//LONG		lPosition;
			SBYTE		sbOnline;
			
			(*istr) >> lGuildIndex;
			(*istr) >> lIndex;
			(*istr) >> strName;
			//(*istr) >> lPosition;
			(*istr) >> sbOnline;
			
			// 우리 길드원.
			if( _pNetwork->MyCharacterInfo.lGuildIndex == lGuildIndex )
			{
				if( _pNetwork->MyCharacterInfo.index != lIndex )
				{
					CTString strMessage;
					if( sbOnline == TRUE )
						strMessage.PrintF( _S( 996, "[%s] 길드원이 접속하셨습니다." ), strName);		
					else
						strMessage.PrintF( _S( 997, "[%s] 길드원이 접속종료 하셨습니다." ), strName);		
					pUIManager->GetChattingUI()->AddSysMessage( strMessage, SYSMSG_NORMAL );
				}
				
				pUIManager->GetGuild()->ChangeMemberOnline( lIndex, sbOnline );

			if(_pNetwork->MyCharacterInfo.lGuildLevel  < LIMIT_GUILD_LEVEL)
			{
				if( _pNetwork->MyCharacterInfo.lGuildPosition == GUILD_MEMBER_BOSS ||
					_pNetwork->MyCharacterInfo.lGuildPosition == GUILD_MEMBER_VICE_BOSS )
				{
					pUIManager->GetGuild()->RefreshMemberList( TRUE );
				}
				else if( _pNetwork->MyCharacterInfo.lGuildPosition == GUILD_MEMBER_MEMBER )
				{
					pUIManager->GetGuild()->RefreshApplicantList();
				}
			}
			}
					}
		break;
		
	case MSG_GUILD_LEVELUP:		// 길드 승급
		{
		}
		break;
		
	case MSG_GUILD_LEVELINFO:	// 길드 레벨 정보
		{
			LONG lGuildIndex;
			LONG lGuildLevel;
			(*istr) >> lGuildIndex;
			(*istr) >> lGuildLevel;
			
			// my Character
			if( _pNetwork->MyCharacterInfo.lGuildIndex == lGuildIndex )
			{
				_pNetwork->MyCharacterInfo.lGuildLevel = lGuildLevel;
			}
			// other Characters
			else
			{
				// 다른 캐릭터들의 길드 레벨을 변경시킴.
				ACTORMGR()->ChangeGuildLevel(lGuildIndex, lGuildLevel);
			}
			pUIManager->GetGuild()->ResetGuild();
		}
		break;
		
	case MSG_GUILD_BREAKUP:		// 길드 해체
		{
			//pUIManager->GetGuild()->ResetGuild();
			//pUIManager->GetGuild()->ClearMemberList();
		}
		break;
		
	case MSG_GUILD_BREAKUP_NOTIFY:
		{
			LONG		lGuildIndex;
			CTString	strGuildName;
			(*istr) >> lGuildIndex;
			(*istr) >> strGuildName;

			ACTORMGR()->ChangeGuildInfo(lGuildIndex, -1, -1, GUILD_MEMBER_NOMEMBER, "");
			
			if( lGuildIndex == _pNetwork->MyCharacterInfo.lGuildIndex)
			{			
				_pNetwork->MyCharacterInfo.lGuildIndex		= -1;
				_pNetwork->MyCharacterInfo.lGuildLevel		= -1;
				_pNetwork->MyCharacterInfo.lGuildPosition	= GUILD_MEMBER_NOMEMBER;
				_pNetwork->MyCharacterInfo.guildPosName =	 "";	// [2010/12/12 : Sora] 길드명도 초기화해야 한다
				
				CTString strMessage;
				strMessage.PrintF( _S( 998, "[%s] 길드가 해체되었습니다." ), strGuildName );			
				pUIManager->GetChattingUI()->AddSysMessage( strMessage, SYSMSG_NORMAL );
				
				pUIManager->CloseMessageBox(MSGCMD_GUILD_ERROR);
				pUIManager->CloseMessageBox(MSGCMD_GUILD_DESTROY);
				pUIManager->CloseMessageBox(MSGCMD_GUILD_JOIN);
				pUIManager->CloseMessageBox(MSGCMD_GUILD_JOIN_REQ);
				pUIManager->CloseMessageBox(MSGCMD_GUILD_QUIT);
				pUIManager->CloseMessageBox(MSGCMD_GUILD_QUIT_CONFIRM);
				pUIManager->CloseMessageBox(MSGCMD_GUILD_APPLICANT_JOIN);
				pUIManager->CloseMessageBox(MSGCMD_GUILD_KICK);						// 멤버 퇴출
				pUIManager->CloseMessageBox(MSGCMD_GUILD_ADD_VICE_BOSS);				// 부단장 임명
				pUIManager->CloseMessageBox(MSGCMD_GUILD_DEL_VICE_BOSS);				// 부단장 퇴출
				pUIManager->CloseMessageBox(MSGCMD_GUILD_CHANGE_BOSS);				// 단장 이임
				
				CUIGuild* pGuild = pUIManager->GetGuild();

				if (pGuild != NULL)
				{
					pGuild->ResetGuild();
					pGuild->ClearMemberList();
					pGuild->ClearGuildSkill();
				}

				((CUIWebBoard*)pUIManager->GetUI(UI_WEBBOARD))->NotifyGuildChange();

				MY_INFO()->_guildmark.remove();
				UIMGR()->SetGuildMark();
			}
		}
		break;
		
	case MSG_GUILD_REGIST_REQ:	// 가입 신청을 함.
		{
			LONG lGuildIndex;
			LONG lBossIndex;
			LONG lRequesterIndex;
			
			(*istr) >> lGuildIndex;
			(*istr) >> lBossIndex;
			(*istr) >> lRequesterIndex;
			
			if( _pNetwork->MyCharacterInfo.lGuildPosition == GUILD_MEMBER_BOSS ||
				_pNetwork->MyCharacterInfo.lGuildPosition == GUILD_MEMBER_VICE_BOSS )
				pUIManager->GetGuild()->ApplicantAccept( lRequesterIndex );
		}
		break;
		
	case MSG_GUILD_REGIST_ALLOW:	// 길드 신청 확인
		{
			pUIManager->CloseMessageBox(MSGCMD_GUILD_JOIN);
			pUIManager->CloseMessageBox(MSGCMD_GUILD_JOIN_REQ);
			pUIManager->CloseMessageBox(MSGCMD_GUILD_APPLICANT_JOIN);
		}
		break;
		
	case MSG_GUILD_REGIST_CANCEL:	// 길드 신청 취소
		{
			//m_bGuildJoinReq;
			
			pUIManager->CloseMessageBox(MSGCMD_GUILD_JOIN);
			pUIManager->CloseMessageBox(MSGCMD_GUILD_JOIN_REQ);
			pUIManager->CloseMessageBox(MSGCMD_GUILD_APPLICANT_JOIN);	
			CTString strMessage;
			strMessage.PrintF( _S( 999, "길드 가입 신청이 취소되었습니다." ) );			
			pUIManager->GetChattingUI()->AddSysMessage( strMessage, SYSMSG_NORMAL );
		}
		break;
		
	case MSG_GUILD_CHANGE_BOSS:
		{
			// 단장 이임			: guildindex(n) current(n) newboss(n)
			LONG lGuildIndex;
			LONG lCharIndex;
			LONG lBossIndex;
			(*istr) >> lGuildIndex;
			(*istr) >> lCharIndex;
			(*istr) >> lBossIndex;

			CTString strNewBossName = "";
			// WSS_NEW_GUILD_SYSTEM 2007/08/07---------------------------------------------->>
			if(_pNetwork->MyCharacterInfo.lGuildLevel  >= LIMIT_GUILD_LEVEL)
			{
				if( lCharIndex == _pNetwork->MyCharacterInfo.index )
				{
					_pNetwork->MyCharacterInfo.lGuildIndex		= lGuildIndex;		// 임시	길드의 인덱스
					_pNetwork->MyCharacterInfo.lGuildPosition	= GUILD_MEMBER_MEMBER;		// 임시 길드에서의 캐릭터의 레벨				
					pUIManager->GetGuild()->ChangeMemberLevel( lCharIndex, GUILD_MEMBER_MEMBER );				
				}

				if( lBossIndex == _pNetwork->MyCharacterInfo.index )
				{
					_pNetwork->MyCharacterInfo.lGuildIndex		= lGuildIndex;				// 임시	길드의 인덱스
					_pNetwork->MyCharacterInfo.lGuildPosition	= GUILD_MEMBER_BOSS;		// 임시 길드에서의 캐릭터의 레벨
					pUIManager->GetGuild()->ChangeMemberLevel( lBossIndex, GUILD_MEMBER_BOSS );
					CTString strMessage;
					strMessage.PrintF( _S( 1000, "길드장에 임명되었습니다." ) );			
					pUIManager->GetChattingUI()->AddSysMessage( strMessage, SYSMSG_NORMAL );
					strNewBossName = _pNetwork->MyCharacterInfo.name;
				}else
				{
					ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_CHARACTER, lBossIndex);

					if (pObject != NULL)
					{
						pUIManager->GetGuild()->ChangeMemberLevel( pObject->GetSIndex(), GUILD_MEMBER_BOSS );
						strNewBossName = pObject->m_strName.c_str();
						}
					}
								
			}else{
				// ------------------------------------------------------------------------------<<
				// 보스 변경.
				if( lCharIndex == _pNetwork->MyCharacterInfo.index )
				{
					_pNetwork->MyCharacterInfo.lGuildIndex		= lGuildIndex;		// 임시	길드의 인덱스
					_pNetwork->MyCharacterInfo.lGuildPosition	= GUILD_MEMBER_MEMBER;		// 임시 길드에서의 캐릭터의 레벨				
					pUIManager->GetGuild()->ChangeMemberLevel( lCharIndex, GUILD_MEMBER_MEMBER );				
					const BOOL bVisible = pUIManager->GetGuild()->IsVisible();
					pUIManager->GetGuild()->ResetGuild();
					if( bVisible )
					{								
						pUIManager->GetGuild()->OpenGuildManager( _pNetwork->MyCharacterInfo.lGuildPosition );
					}
				}
				else
				{
					ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_CHARACTER, lCharIndex);

					if (pObject != NULL)
					{
						CCharacterTarget* pTarget = static_cast< CCharacterTarget* >(pObject);

						pTarget->cha_lGuildIndex		= lGuildIndex;				// 임시	길드의 인덱스
						pTarget->cha_lGuildPosition	= GUILD_MEMBER_MEMBER;		// 임시 길드에서의 캐릭터의 레벨
						}

					pUIManager->GetGuild()->ChangeMemberLevel( lCharIndex, GUILD_MEMBER_MEMBER );
				}
				
				if( lBossIndex == _pNetwork->MyCharacterInfo.index )
				{
					_pNetwork->MyCharacterInfo.lGuildIndex		= lGuildIndex;				// 임시	길드의 인덱스
					_pNetwork->MyCharacterInfo.lGuildPosition	= GUILD_MEMBER_BOSS;		// 임시 길드에서의 캐릭터의 레벨
					pUIManager->GetGuild()->ChangeMemberLevel( lBossIndex, GUILD_MEMBER_BOSS );				
					const BOOL bVisible = pUIManager->GetGuild()->IsVisible();
					pUIManager->GetGuild()->ResetGuild();
					if( bVisible )
					{								
						pUIManager->GetGuild()->OpenGuildManager( _pNetwork->MyCharacterInfo.lGuildPosition );
					}

					CTString strMessage;
					strMessage.PrintF( _S( 1000, "길드장에 임명되었습니다." ) );			
					pUIManager->GetChattingUI()->AddSysMessage( strMessage, SYSMSG_NORMAL );
					strNewBossName = _pNetwork->MyCharacterInfo.name;
				}
				else
				{
					ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_CHARACTER, lBossIndex);

					if (pObject != NULL)
					{
						CCharacterTarget* pTarget = static_cast< CCharacterTarget* >(pObject);

						pTarget->cha_lGuildIndex	= lGuildIndex;
						pTarget->cha_lGuildPosition	= GUILD_MEMBER_BOSS;
						strNewBossName = pTarget->m_strName.c_str();
						}

					pUIManager->GetGuild()->ChangeMemberLevel( lBossIndex, GUILD_MEMBER_BOSS );
				}

				if( _pNetwork->MyCharacterInfo.lGuildPosition == GUILD_MEMBER_BOSS ||
					_pNetwork->MyCharacterInfo.lGuildPosition == GUILD_MEMBER_VICE_BOSS )
				{
					pUIManager->GetGuild()->RefreshMemberList( TRUE );
				}
				else if( _pNetwork->MyCharacterInfo.lGuildPosition == GUILD_MEMBER_MEMBER )
				{
					pUIManager->GetGuild()->RefreshApplicantList();
				}
			}

			if (!strNewBossName.IsEmpty())
			{
				CUIMsgBox_Info	MsgBoxInfo;
				CTString strTemp;
				MsgBoxInfo.SetMsgBoxInfo(  _S( 865, "길드" ) , UMBS_OK, UI_NONE, MSGCMD_NULL);
				strTemp.PrintF( _S(5672, "%s 님께서 길드장으로 임명되셨습니다."), strNewBossName );
				MsgBoxInfo.AddString(strTemp);
				pUIManager->CreateMessageBox( MsgBoxInfo );	
			}

		}
		break;

	case MSG_GUILD_APPOINT_OFFICER:
		{
			// 부단장 임명			: guildindex(n) charindex(n)
			LONG lGuildIndex;
			LONG lCharIndex;			
			(*istr) >> lGuildIndex;
			(*istr) >> lCharIndex;

			// WSS_NEW_GUILD_SYSTEM 2007/08/07---------------------------------------------->>
			if(_pNetwork->MyCharacterInfo.lGuildLevel  >= LIMIT_GUILD_LEVEL)
			{
				if( lCharIndex == _pNetwork->MyCharacterInfo.index )
				{
					_pNetwork->MyCharacterInfo.lGuildIndex		= lGuildIndex;		// 임시	길드의 인덱스
					_pNetwork->MyCharacterInfo.lGuildPosition	= GUILD_MEMBER_VICE_BOSS;		// 임시 길드에서의 캐릭터의 레벨				

					CTString strMessage;
					strMessage.PrintF( _S( 1001, "길드부장에 임명되었습니다." ) );				
					pUIManager->GetChattingUI()->AddSysMessage( strMessage, SYSMSG_NORMAL );
				}
				else 
				{
					ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_CHARACTER, lCharIndex);

					if (pObject != NULL)
					{
						CCharacterTarget* pTarget = static_cast< CCharacterTarget* >(pObject);

						pTarget->cha_lGuildIndex	= lGuildIndex;
						pTarget->cha_lGuildPosition	= GUILD_MEMBER_VICE_BOSS;
						}
				}

				pUIManager->GetGuild()->ChangeMemberLevel( lCharIndex, GUILD_MEMBER_VICE_BOSS );				
			}
			// ------------------------------------------------------------------------------<<
			
			if( lCharIndex == _pNetwork->MyCharacterInfo.index )
			{
				_pNetwork->MyCharacterInfo.lGuildIndex		= lGuildIndex;		// 임시	길드의 인덱스
				_pNetwork->MyCharacterInfo.lGuildPosition	= GUILD_MEMBER_VICE_BOSS;		// 임시 길드에서의 캐릭터의 레벨				
				pUIManager->GetGuild()->ChangeMemberLevel( lCharIndex, GUILD_MEMBER_VICE_BOSS );
				const BOOL bVisible = pUIManager->GetGuild()->IsVisible();
				pUIManager->GetGuild()->ResetGuild();
				if( bVisible )
				{								
					pUIManager->GetGuild()->OpenGuildManager( _pNetwork->MyCharacterInfo.lGuildPosition );
				}
				
				CTString strMessage;
				strMessage.PrintF( _S( 1001, "길드부장에 임명되었습니다." ) );			
				pUIManager->GetChattingUI()->AddSysMessage( strMessage, SYSMSG_NORMAL );
			}
			else
			{
				ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_CHARACTER, lCharIndex);

				if (pObject != NULL)
				{
					CCharacterTarget* pTarget = static_cast< CCharacterTarget* >(pObject);

					pTarget->cha_lGuildIndex	= lGuildIndex;
					pTarget->cha_lGuildPosition	= GUILD_MEMBER_VICE_BOSS;
					}

				pUIManager->GetGuild()->ChangeMemberLevel( lCharIndex, GUILD_MEMBER_VICE_BOSS );
			}

			if( _pNetwork->MyCharacterInfo.lGuildPosition == GUILD_MEMBER_BOSS ||
				_pNetwork->MyCharacterInfo.lGuildPosition == GUILD_MEMBER_VICE_BOSS )
			{
				pUIManager->GetGuild()->RefreshMemberList( TRUE );
			}
			else if( _pNetwork->MyCharacterInfo.lGuildPosition == GUILD_MEMBER_MEMBER )
			{
				pUIManager->GetGuild()->RefreshApplicantList();
			}
		}
		break;

	case MSG_GUILD_FIRE_OFFICER:
		{
			// 부단장 해임			: guildindex(n) charindex(n)
			LONG lGuildIndex;
			LONG lCharIndex;
			(*istr) >> lGuildIndex;
			(*istr) >> lCharIndex;
			
			// WSS_NEW_GUILD_SYSTEM 2007/08/07---------------------------------------------->>
			if(_pNetwork->MyCharacterInfo.lGuildLevel  >= LIMIT_GUILD_LEVEL)
			{
				if( lCharIndex == _pNetwork->MyCharacterInfo.index )
				{
					_pNetwork->MyCharacterInfo.lGuildIndex		= lGuildIndex;		// 임시	길드의 인덱스
					_pNetwork->MyCharacterInfo.lGuildPosition	= GUILD_MEMBER_MEMBER;		// 임시 길드에서의 캐릭터의 레벨				
					CTString strMessage;
					strMessage.PrintF( _S( 1002, "길드부장에서 해임되었습니다." ) );		
					pUIManager->GetChattingUI()->AddSysMessage( strMessage, SYSMSG_NORMAL );
				}
				else
				{
					ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_CHARACTER, lCharIndex);

					if (pObject != NULL)
					{
						CCharacterTarget* pTarget = static_cast< CCharacterTarget* >(pObject);

						pTarget->cha_lGuildIndex	= lGuildIndex;
						pTarget->cha_lGuildPosition	= GUILD_MEMBER_MEMBER;
						}
					}
				
				pUIManager->GetGuild()->ChangeMemberLevel( lCharIndex, GUILD_MEMBER_MEMBER );				
			}
			// ------------------------------------------------------------------------------<<

			if( lCharIndex == _pNetwork->MyCharacterInfo.index )
			{
				_pNetwork->MyCharacterInfo.lGuildIndex		= lGuildIndex;		// 임시	길드의 인덱스
				_pNetwork->MyCharacterInfo.lGuildPosition	= GUILD_MEMBER_MEMBER;		// 임시 길드에서의 캐릭터의 레벨				
				pUIManager->GetGuild()->ChangeMemberLevel( lCharIndex, GUILD_MEMBER_MEMBER );
				const BOOL bVisible = pUIManager->GetGuild()->IsVisible();
				pUIManager->GetGuild()->ResetGuild();
				if( bVisible )
				{								
					pUIManager->GetGuild()->OpenGuildManager( _pNetwork->MyCharacterInfo.lGuildPosition );
				}

				CTString strMessage;
				strMessage.PrintF( _S( 1002, "길드부장에서 해임되었습니다." ) );			
				pUIManager->GetChattingUI()->AddSysMessage( strMessage, SYSMSG_NORMAL );
			}
			else
			{
				ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_CHARACTER, lCharIndex);

				if (pObject != NULL)
				{
					CCharacterTarget* pTarget = static_cast< CCharacterTarget* >(pObject);

					pTarget->cha_lGuildIndex	= lGuildIndex;
					pTarget->cha_lGuildPosition	= GUILD_MEMBER_MEMBER;
					}

				pUIManager->GetGuild()->ChangeMemberLevel( lCharIndex, GUILD_MEMBER_MEMBER );
			}

			if( _pNetwork->MyCharacterInfo.lGuildPosition == GUILD_MEMBER_BOSS ||
				_pNetwork->MyCharacterInfo.lGuildPosition == GUILD_MEMBER_VICE_BOSS )
			{
				pUIManager->GetGuild()->RefreshMemberList( TRUE );
			}
			else if( _pNetwork->MyCharacterInfo.lGuildPosition == GUILD_MEMBER_MEMBER )
			{
				pUIManager->GetGuild()->RefreshApplicantList();
			}
		}
		break;	
	case MSG_GUILD_MEMBER_ADD:		// 멤버 추가
		{
			LONG lGuildIndex;
			LONG lCharIndex;
			CTString strCharName;
			(*istr) >> lGuildIndex;
			(*istr) >> lCharIndex;
			(*istr) >> strCharName;
			
			// WSS_NEW_GUILD_SYSTEM 2007/08/08	
			// 실시간 갱신이 안되므로 메시지만 출력해 준다.
			if(_pNetwork->MyCharacterInfo.lGuildLevel  >= LIMIT_GUILD_LEVEL)
			{	
				CTString strMessage;
				strMessage.PrintF( _S( 1003, "[%s] 길드원이 가입하였습니다." ), strCharName );			
				pUIManager->GetChattingUI()->AddSysMessage( strMessage, SYSMSG_NORMAL );
				break;
			}
			// 내 길드에 추가되는 경우.
			if( _pNetwork->MyCharacterInfo.lGuildIndex == lGuildIndex )
			{
				pUIManager->GetGuild()->AddToMemberList( lCharIndex, strCharName, GUILD_MEMBER_MEMBER, TRUE );			

				if( _pNetwork->MyCharacterInfo.lGuildPosition == GUILD_MEMBER_MEMBER )
				{
					pUIManager->GetGuild()->RefreshApplicantList();
				}
				else if( _pNetwork->MyCharacterInfo.lGuildPosition == GUILD_MEMBER_BOSS ||
					_pNetwork->MyCharacterInfo.lGuildPosition == GUILD_MEMBER_VICE_BOSS )
				{
					pUIManager->GetGuild()->RefreshMemberList();
				}
				
				CTString strMessage;
				strMessage.PrintF( _S( 1003, "[%s] 길드원이 가입하였습니다." ), strCharName );			
				pUIManager->GetChattingUI()->AddSysMessage( strMessage, SYSMSG_NORMAL );
			}
		}
		break;
	case MSG_GUILD_MEMBER_OUT:		// 멤버 탈퇴
		{
			LONG lGuildIndex;
			LONG lCharIndex;
			CTString strCharName;
			(*istr) >> lGuildIndex;
			(*istr) >> lCharIndex;
			(*istr) >> strCharName;	
			
			if( lCharIndex == _pNetwork->MyCharacterInfo.index )
			{
				_pNetwork->MyCharacterInfo.lGuildIndex		= -1;		// 임시	길드의 인덱스
				_pNetwork->MyCharacterInfo.lGuildLevel		= -1;		// 임시 길드의 레벨
				_pNetwork->MyCharacterInfo.lGuildPosition	= GUILD_MEMBER_NOMEMBER;		// 임시 길드에서의 캐릭터의 레벨
				_pNetwork->MyCharacterInfo.strGuildName		= "";

				((CUIWebBoard*)pUIManager->GetUI(UI_WEBBOARD))->NotifyGuildChange();

				pUIManager->GetGuild()->ClearGuildSkill();
			}
			else
			{
				ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_CHARACTER, lCharIndex);

				if (pObject != NULL)
				{
					CCharacterTarget* pTarget = static_cast< CCharacterTarget* >(pObject);

					pTarget->cha_lGuildIndex	= -1;
					pTarget->cha_lGuildLevel	= -1;
					pTarget->cha_lGuildPosition	= GUILD_MEMBER_NOMEMBER;
					pTarget->cha_strGuildName	= "";
					}
			}			
						
			if( _pNetwork->MyCharacterInfo.lGuildIndex == lGuildIndex )
			{
				// WSS_NEW_GUILD_SYSTEM 2007/08/08	
			// 실시간 갱신이 안되므로 메시지만 출력해 준다.
				if(_pNetwork->MyCharacterInfo.lGuildLevel  < LIMIT_GUILD_LEVEL)
				{	
					pUIManager->GetGuild()->DelFromMemberList( lCharIndex );
					
					if( _pNetwork->MyCharacterInfo.lGuildPosition == GUILD_MEMBER_MEMBER )
					{
						pUIManager->GetGuild()->RefreshApplicantList();
					}
					else if( _pNetwork->MyCharacterInfo.lGuildPosition == GUILD_MEMBER_BOSS ||
						_pNetwork->MyCharacterInfo.lGuildPosition == GUILD_MEMBER_VICE_BOSS )
					{
						pUIManager->GetGuild()->RefreshMemberList();
					}
				}
				CTString strMessage;
				strMessage.PrintF( _S( 1004, "[%s] 길드원이 탈퇴하였습니다." ), strCharName );			

				pUIManager->GetChattingUI()->AddSysMessage( strMessage, SYSMSG_NORMAL );
			}
		}
		break;
	case MSG_GUILD_MEMBER_KICK:		// 멤버 강퇴
		{
			// guildindex(n) bossindex(n) charindex(n) charname(str)
			
			LONG lGuildIndex;
			LONG lBossIndex;
			LONG lCharIndex;
			CTString strCharName;
			(*istr) >> lGuildIndex;
			(*istr) >> lBossIndex;
			(*istr) >> lCharIndex;
			(*istr) >> strCharName;
			
			if( lCharIndex == _pNetwork->MyCharacterInfo.index )
			{
				// 자신이 퇴출된거라면...				
				_pNetwork->MyCharacterInfo.lGuildIndex		= -1;		// 임시	길드의 인덱스
				_pNetwork->MyCharacterInfo.lGuildLevel		= -1;		// 임시 길드의 레벨
				_pNetwork->MyCharacterInfo.lGuildPosition	= GUILD_MEMBER_NOMEMBER;		// 임시 길드에서의 캐릭터의 레벨
				_pNetwork->MyCharacterInfo.strGuildName		= "";
				pUIManager->GetGuild()->ResetGuild();
				pUIManager->GetGuild()->ClearMemberList();

				CTString strMessage;
				strMessage.PrintF( _S( 1005, "길드에서 퇴출되었습니다." ) );			
				pUIManager->GetChattingUI()->AddSysMessage( strMessage, SYSMSG_NORMAL );

				((CUIWebBoard*)pUIManager->GetUI(UI_WEBBOARD))->NotifyGuildChange();

				MY_INFO()->_guildmark.remove();
				UIMGR()->SetGuildMark();
				return;
			}
			else
			{
				ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_CHARACTER, lCharIndex);

				if (pObject != NULL)
				{
					CCharacterTarget* pTarget = static_cast< CCharacterTarget* >(pObject);

					pTarget->cha_lGuildIndex	= -1;
					pTarget->cha_lGuildLevel	= -1;
					pTarget->cha_lGuildPosition	= GUILD_MEMBER_NOMEMBER;
					pTarget->cha_strGuildName	= "";
					}
				}
			
			// 내가 갖고 있는 길드 정보에서 해당 캐릭터 정보를 제거함.
			if( _pNetwork->MyCharacterInfo.lGuildIndex == lGuildIndex )
			{			
			// WSS_NEW_GUILD_SYSTEM 2007/08/08	
			// 실시간 갱신이 안되므로 메시지만 출력해 준다.
				if(_pNetwork->MyCharacterInfo.lGuildLevel  < LIMIT_GUILD_LEVEL)
				{		
					pUIManager->GetGuild()->DelFromMemberList( lCharIndex );
					
					if( _pNetwork->MyCharacterInfo.lGuildPosition == GUILD_MEMBER_MEMBER )
					{
						pUIManager->GetGuild()->RefreshApplicantList();
					}
					else if( _pNetwork->MyCharacterInfo.lGuildPosition == GUILD_MEMBER_BOSS ||
						_pNetwork->MyCharacterInfo.lGuildPosition == GUILD_MEMBER_VICE_BOSS )
					{
						pUIManager->GetGuild()->RefreshMemberList();
					}
				}
				CTString strMessage;
				strMessage.PrintF( _S( 1006, "[%s] 길드원을 퇴출하였습니다." ), strCharName );			
				pUIManager->GetChattingUI()->AddSysMessage( strMessage, SYSMSG_NORMAL );
			}
		}
		break;
	case MSG_GUILD_INFO_CHANGE:		// 길드 정보 변경.
		{
			LONG		lCharIndex;
			LONG		lGuildIndex;
			CTString	strGuildName;
			LONG		lCharPos;
			
			(*istr) >> lCharIndex;
			(*istr) >> lGuildIndex;
			(*istr) >> strGuildName;
			(*istr) >> lCharPos;
			SBYTE		sbGuildRank;
			(*istr) >> sbGuildRank;
			
			LONG		lOldPos = _pNetwork->MyCharacterInfo.lGuildPosition;

// [sora] GUILD_MARK
#ifdef GUILD_MARK
			SBYTE gm_row, gm_col, bg_row, bg_col;

			(*istr) >> gm_row;
			(*istr) >> gm_col;
			(*istr) >> bg_row;
			(*istr) >> bg_col;
#endif
						
			// 길드 상에서의 내 위치나 길드명이 변경된 경우...
			if( lCharIndex == _pNetwork->MyCharacterInfo.index )
			{
				_pNetwork->MyCharacterInfo.lGuildIndex		= lGuildIndex;		// 임시	길드의 인덱스				
				_pNetwork->MyCharacterInfo.lGuildPosition	= lCharPos;		// 임시 길드에서의 캐릭터의 레벨
				_pNetwork->MyCharacterInfo.strGuildName		= strGuildName;
				_pNetwork->MyCharacterInfo.sbGuildRank		= sbGuildRank;

// [sora] GUILD_MARK
#ifdef GUILD_MARK
				MY_INFO()->_guildmark.setData(gm_row, gm_col, bg_row, bg_col);
				UIMGR()->SetGuildMark();
#endif
			}
			else
			{
				ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_CHARACTER, lCharIndex);

				if (pObject != NULL)
				{
					CCharacterTarget* pTarget = static_cast< CCharacterTarget* >(pObject);

					pTarget->cha_lGuildIndex	= lGuildIndex;		// 임시	길드의 인덱스
					pTarget->cha_lGuildPosition	= lCharPos;			// 임시 길드에서의 캐릭터의 레벨
					pTarget->cha_strGuildName	= strGuildName;
					pTarget->cha_sbGuildRank	= sbGuildRank;

#ifdef GUILD_MARK
						if( gm_row >= 0 )
						{
						if (pTarget->pGuildmark == NULL)
						{
							pTarget->pGuildmark = new CUIGuildMarkIcon;
							pTarget->pGuildmark->Create(NULL, 0, 0, 15, 15);
						}

						pTarget->pGuildmark->CalcUV(gm_row, gm_col, true);
						pTarget->pGuildmark->CalcUV(bg_row, bg_col, false);
						}
#endif
					}
				}
			
			if( _pNetwork->MyCharacterInfo.lGuildIndex == lGuildIndex )
			{
				pUIManager->GetGuild()->ChangeMemberLevel( lCharIndex, lCharPos );				
			
			// WSS_NEW_GUILD_SYSTEM 2007/08/08	
			// 실시간 갱신이 안되므로 메시지만 출력해 준다.
			if(_pNetwork->MyCharacterInfo.lGuildLevel < LIMIT_GUILD_LEVEL)
			{					
				// 상태에 변경이 없음.
				if( lOldPos == _pNetwork->MyCharacterInfo.lGuildPosition )
				{
					if( _pNetwork->MyCharacterInfo.lGuildPosition == GUILD_MEMBER_MEMBER )
					{
						pUIManager->GetGuild()->RefreshApplicantList( );
					}
					else if( _pNetwork->MyCharacterInfo.lGuildPosition == GUILD_MEMBER_BOSS ||
						_pNetwork->MyCharacterInfo.lGuildPosition == GUILD_MEMBER_VICE_BOSS )
					{
						pUIManager->GetGuild()->RefreshMemberList( );
					}
				}
				else
				{
					// 멤버에서 단장 혹은 부단장으로 변경.
					if( lOldPos == GUILD_MEMBER_MEMBER && 
						( _pNetwork->MyCharacterInfo.lGuildPosition == GUILD_MEMBER_BOSS || 
						_pNetwork->MyCharacterInfo.lGuildPosition == GUILD_MEMBER_VICE_BOSS ) )
					{
						const BOOL bVisible = pUIManager->GetGuild()->IsVisible();
						pUIManager->GetGuild()->ResetGuild();
						if( bVisible )
						{								
							pUIManager->GetGuild()->OpenGuildManager( _pNetwork->MyCharacterInfo.lGuildPosition );
						}
					}
					else if( ( lOldPos == GUILD_MEMBER_BOSS || lOldPos == GUILD_MEMBER_VICE_BOSS ) )						
					{
						// 단장 혹은 부단장에서 일반 멤버로...
						if( _pNetwork->MyCharacterInfo.lGuildPosition == GUILD_MEMBER_MEMBER )
						{
							const BOOL bVisible = pUIManager->GetGuild()->IsVisible();
							pUIManager->GetGuild()->ResetGuild();
							if( bVisible )
							{								
								pUIManager->GetGuild()->OpenGuildManager( _pNetwork->MyCharacterInfo.lGuildPosition );
							}
						}
					}
				}
			}
			}
		}
		break;
		// Guild Battle 
	// 길드전 신청 요청 받음
	case MSG_GUILD_BATTLE_REQ_REQ: 
		{
			LONG		lGuildIndex;
			CTString	strGuildName;
			LONG		lPrize;
			LONG		lTime;

			(*istr) >> lGuildIndex;
			(*istr) >> strGuildName;
			(*istr) >> lPrize;
			(*istr) >> lTime;

			
			pUIManager->GetGuildBattle()->GBReq( lGuildIndex, strGuildName, lPrize, lTime );
		}
		break;

	// 길드전 신청 거절 받음
	case MSG_GUILD_BATTLE_REQ_REJECT: 
		{
			LONG lRejectCharIndex;
			(*istr) >> lRejectCharIndex;
			
			pUIManager->GetGuildBattle()->GBReqReject( lRejectCharIndex );
	
		}
		break;
		
	// 길드전 신청 허락	받음
	case MSG_GUILD_BATTLE_REQ_ACCEPT:
		{
			LONG		lGuildIndex1;
			CTString	strGuildName1;
			LONG		lGuildIndex2;
			CTString	strGuildName2;
			LONG		lPrize;
			LONG		lZone;
			LONG		lTime;
			
			(*istr) >> lGuildIndex1;
			(*istr) >> strGuildName1;
			(*istr) >> lGuildIndex2;
			(*istr) >> strGuildName2;
			(*istr) >> lPrize;
			(*istr) >> lZone;
			(*istr) >> lTime;

					
			pUIManager->GetGuildBattle()->GBReqAccept( lGuildIndex1, strGuildName1, lGuildIndex2, strGuildName2, lPrize, lZone, lTime );
		}
		break;

	// 길드전 중단 요청	받음 
	case MSG_GUILD_BATTLE_STOP_REQ:
		{
			LONG		lGuildIndex;
			CTString	strGuildName;

			(*istr) >> lGuildIndex;
			(*istr) >> strGuildName;
						
			pUIManager->GetGuildBattle()->GBStopReqRev( lGuildIndex, strGuildName );
		}
		break;

	// 길드전 중단 거절 받음 
	case MSG_GUILD_BATTLE_STOP_REJECT:
		{
			LONG lRejectCharIndex;
			(*istr) >> lRejectCharIndex;
			
			pUIManager->GetGuildBattle()->GBStopReqReject( lRejectCharIndex );
		}
		break;

	// 길드전 중단 허락 받음 
	case MSG_GUILD_BATTLE_STOP_ACCEPT:
		{
			pUIManager->GetGuildBattle()->GBStopReqAccept();
		}
		break;

	// 길드전 시작 알림
	case MSG_GUILD_BATTLE_START:
		{
			LONG		lGuildIndex1;
			CTString	strGuildName1;
			LONG		lGuildIndex2;
			CTString	strGuildName2;
			LONG		lPrize;
			LONG		lZone;
			LONG		lTime;
			
			(*istr) >> lGuildIndex1;
			(*istr) >> strGuildName1;
			(*istr) >> lGuildIndex2;
			(*istr) >> strGuildName2;
			(*istr) >> lPrize;
			(*istr) >> lZone;
			(*istr) >> lTime;

			pUIManager->GetGuildBattle()->GBStart ( lGuildIndex1, strGuildName1, lGuildIndex2, strGuildName2, lPrize, lZone, lTime );
			
		}
		break;

	// 길드전 현황			
	case MSG_GUILD_BATTLE_STATUS:
		{
			LONG		lGuildIndex1;
			CTString	strGuildName1;
			LONG		lCount1;
			LONG		lGuildIndex2;
			CTString	strGuildName2;
			LONG		lCount2;
			LONG		lTime;
			LONG		lZone;

			(*istr) >> lGuildIndex1;
			(*istr) >> strGuildName1;
			(*istr) >> lCount1;
			(*istr) >> lGuildIndex2;
			(*istr) >> strGuildName2;
			(*istr) >> lCount2;			
			(*istr) >> lTime;
			(*istr) >> lZone;
			
			pUIManager->GetGuildBattle()->GBStatus( lGuildIndex1, strGuildName1, lCount1, 
												lGuildIndex2, strGuildName2, lCount2, lTime, lZone );
		}
		break;

	// 길드전 종료 	
	case MSG_GUILD_BATTLE_END:
		{
			LONG		lWinnerGuildIndex;
			LONG		lGuildIndex1;
			CTString	strGuildName1;
			LONG		lGuildIndex2;
			CTString	strGuildName2;
			LONG		lPrize;
	
			(*istr) >> lWinnerGuildIndex;
			(*istr) >> lGuildIndex1;
			(*istr) >> strGuildName1;
			(*istr) >> lGuildIndex2;
			(*istr) >> strGuildName2;
			(*istr) >> lPrize;
		
			pUIManager->GetGuildBattle()->GBEnd( lWinnerGuildIndex, lGuildIndex1, strGuildName1, lGuildIndex2, strGuildName2, lPrize );
		}
		break;

// Date : 2005-07-06(오후 2:41:11), By Lee Ki-hwan
// 공성전 관련 메세지 처리 
	case MSG_GUILD_WAR_JOIN_ATTACK_GUILD:
	case MSG_GUILD_WAR_JOIN_DEFENSE_GUILD:
	case MSG_GUILD_WAR_JOIN_ATTACK_CHAR:	
	case MSG_GUILD_WAR_JOIN_DEFENSE_CHAR:
		{
			LONG		nErrorCode;
			(*istr) >> nErrorCode;
			_pUISWDoc->ErrorProc( nErrorCode, ubType );
		}
		break;
	case MSG_GUILD_WAR_GET_TIME:
		{
			LONG		nErrorCode;
			LONG		nGuildIndex;
			CTString	strGuildName;
			SBYTE		nMonth;
			SBYTE		nDay;
			SBYTE		nHour;
			SBYTE		nMin;
				
			(*istr) >> nErrorCode;
			(*istr) >> nGuildIndex;
			(*istr) >> strGuildName;
			(*istr) >> nMonth;
			(*istr) >> nDay;
			(*istr) >> nHour;
			(*istr) >> nMin;
			
			if( nErrorCode != MSG_GUILD_ERROR_OK )	_pUISWDoc->ErrorProc( nErrorCode );
			else
			{
				_pUISWDoc->GetTime( nGuildIndex, strGuildName, nMonth, nDay, nHour, nMin );
			}
			
		}
		break;
	case MSG_GUILD_WAR_START:
		{
			LONG		nZoneIndex;
			LONG		nRemainSec;

			(*istr) >> nZoneIndex;
			(*istr) >> nRemainSec;
			
			_pUISWDoc->GuildWarStart( nZoneIndex, nRemainSec );	
		}
		break;
	case MSG_GUILD_WAR_NOTICE_START_CASTLE:
		{
			LONG		nZoneIndex;
			LONG		nRemainSec;
			LONG		nGuildIndex1;
			CTString	strGuildName1;
			LONG		nGuildIndex2;
			CTString	strGuildName2;
			LONG		nGuildIndex3;
			CTString	strGuildName3;

			(*istr) >> nZoneIndex;
			(*istr) >> nRemainSec;
			(*istr) >> nGuildIndex1;
			(*istr) >> strGuildName1;
			(*istr) >> nGuildIndex2;
			(*istr) >> strGuildName2;
			(*istr) >> nGuildIndex3;
			(*istr) >> strGuildName3;

			_pUISWDoc->StartCastle( nZoneIndex, nRemainSec, nGuildIndex1, strGuildName1, nGuildIndex2, strGuildName2, nGuildIndex3, strGuildName3 );
		}
		break;
	case MSG_GUILD_WAR_POINT:
		{
			LONG		nRemainSec;
			LONG		nGuildIndex1;
			CTString	strGuildName1;
			LONG		nGuildPoint1;
			LONG		nGuildIndex2;
			CTString	strGuildName2;
			LONG		nGuildPoint2;
			LONG		nGuildIndex3;
			CTString	strGuildName3;
			LONG		nGuildPoint3;
			LONG		nMyPoint;
			LONG		nDefensPoint;

			(*istr) >> nRemainSec;		
			(*istr) >> nGuildIndex1;
			(*istr) >> strGuildName1;
			(*istr) >> nGuildPoint1;
			(*istr) >> nGuildIndex2;
			(*istr) >> strGuildName2;
			(*istr) >> nGuildPoint2;
			(*istr) >> nGuildIndex3;
			(*istr) >> strGuildName3;
			(*istr) >> nGuildPoint3;
			(*istr) >> nMyPoint;
			(*istr) >> nDefensPoint;

			SGuild Guild1, Guild2, Guild3;

			Guild1.SetGuildInfo( nGuildIndex1, strGuildName1, nGuildPoint1 );
			Guild2.SetGuildInfo( nGuildIndex2, strGuildName2, nGuildPoint2 );
			Guild3.SetGuildInfo( nGuildIndex3, strGuildName3, nGuildPoint3 );

			_pUISWDoc->UpdatePoint( nRemainSec, Guild1, Guild2, Guild3, nMyPoint, nDefensPoint );
		}
		break;
	case MSG_GUILD_WAR_NOTICE_REMAIN_FIELD_TIME:
		{
			LONG		nRemainSec;
			LONG		nZoneIndex;

			(*istr) >> nZoneIndex;
			(*istr) >> nRemainSec;					
	
			_pUISWDoc->RemainFiledTime( nZoneIndex, nRemainSec );
		}
		break;
	case MSG_GUILD_WAR_NOTICE_TIME:
		{
			LONG		nZoneIndex;
			SBYTE		nMonth;
			SBYTE		nDay;
			SBYTE		nHour;
			SBYTE		nMin;
				
			(*istr) >> nZoneIndex;
			(*istr) >> nMonth;
			(*istr) >> nDay;
			(*istr) >> nHour;
			(*istr) >> nMin;
			
			_pUISWDoc->TimeConfirm( nZoneIndex, nMonth, nDay, nHour, nMin );
		}
		break;
	case MSG_GUILD_WAR_NOTICE_TIME_REMAIN:
		{
			LONG		nZoneIndex;
			SBYTE		nMin;
		
			(*istr) >> nZoneIndex;
			(*istr) >> nMin;		

			_pUISWDoc->RemainStartTime( nZoneIndex, nMin );
		}
		break;
	case MSG_GUILD_WAR_CASTLE_STATE:
		{
			LONG		nZoneIndex;
			LONG		nState;
			LONG		nGateState;
			LONG		nRemainSec;
			LONG		nGuildIndex1;
			CTString	strGuildName1;
			LONG		nGuildPoint1;
			LONG		nGuildIndex2;
			CTString	strGuildName2;
			LONG		nGuildPoint2;
			LONG		nGuildIndex3;
			CTString	strGuildName3;
			LONG		nGuildPoint3;
			LONG		nMyPoint;
			LONG		nDefensPoint;

			(*istr) >> nZoneIndex;		
			(*istr) >> nState;		
			(*istr) >> nGateState;	
			(*istr) >> nRemainSec;
// WSS_DRATAN_SIEGEWARFARE 070723 ------------------>><<
// 드라탄 공성은 포인트 관련 데이터를 받지 않는다.
			if( nZoneIndex == 7)
			{			
				(*istr) >> nGuildIndex1;
				(*istr) >> strGuildName1;
				(*istr) >> nGuildPoint1;
				(*istr) >> nGuildIndex2;
				(*istr) >> strGuildName2;
				(*istr) >> nGuildPoint2;
				(*istr) >> nGuildIndex3;
				(*istr) >> strGuildName3;
				(*istr) >> nGuildPoint3;
				(*istr) >> nMyPoint;
				(*istr) >> nDefensPoint;

				SGuild Guild1, Guild2, Guild3;
				
				Guild1.SetGuildInfo( nGuildIndex1, strGuildName1, nGuildPoint1 );
				Guild2.SetGuildInfo( nGuildIndex2, strGuildName2, nGuildPoint2 );
				Guild3.SetGuildInfo( nGuildIndex3, strGuildName3, nGuildPoint3 );

				_pUISWDoc->CastleState( nZoneIndex, nState, nGateState, nRemainSec, Guild1, Guild2, Guild3, nMyPoint, nDefensPoint );
			}
			else if( nZoneIndex ==4 )
			{
				_pUISWDoc->CastleState( nZoneIndex, nState, nGateState, nRemainSec);
			}


		}
		break;
	case MSG_GUILD_WAR_GATE_STATE:
		{
			LONG lOldGate;
			LONG lNewGate;
			(*istr) >> lOldGate;
			(*istr) >> lNewGate;

			_pUISWDoc->SetGateState( lOldGate, lNewGate );
		
		}
		break;

	case MSG_GUILD_WAR_ATTACK_SYMBOL:
		{
			LONG lChaIndex;
			(*istr) >> lChaIndex;
		}
		break;
		
	case MSG_GUILD_WAR_END:
		{
			LONG		nZoneIndex; 
			BYTE		nWinDefense; 
			LONG		nOwnerGuildIndex; 
			CTString	strOwnerGuildName; 
			LONG		nOwnerIndex; 
			CTString	strOwnerName; 
			LONG		nNextMonth; 
			LONG		nNextDay; 
			LONG		nNextWeek; 
			LONG		nNextHour;

			(*istr) >> nZoneIndex; 
			(*istr) >> nWinDefense; 
			(*istr) >> nOwnerGuildIndex;
			(*istr) >> strOwnerGuildName;
			(*istr) >> nOwnerIndex; 
			(*istr) >> strOwnerName; 
			(*istr) >> nNextMonth; 
			(*istr) >> nNextDay; 
			(*istr) >> nNextHour;
			(*istr) >> nNextWeek; 

			_pUISWDoc->EndWar( nZoneIndex, nWinDefense, nOwnerGuildIndex, strOwnerGuildName, nOwnerIndex, strOwnerName, nNextMonth, nNextDay, nNextWeek, nNextHour );
		}
		break;
	case MSG_GUILD_WAR_SET_TIME_REP:
		{
			LONG		nErrorCode;
			SBYTE		nMonth;
			SBYTE		nDay;
			SBYTE		nHour;
			SBYTE		nMin;
				
			(*istr) >> nErrorCode;
			(*istr) >> nMonth;
			(*istr) >> nDay;
			(*istr) >> nHour;
			(*istr) >> nMin;


			if( nErrorCode != MSG_GUILD_ERROR_OK ) _pUISWDoc->ErrorProc( nErrorCode );
			else
			{
				_pUISWDoc->SetTimeRep( nMonth, nDay, nHour, nMin );
			}

		}
		break;
	case MSG_GUILD_WAR_INVALID_COMMAND:
		{
			BYTE		nReqMsgType;
			(*istr) >> nReqMsgType;
						
			_pUISWDoc->InvalidCommand( nReqMsgType );
		}
		break;

	// Date : 2005-09-06(오전 11:03:59), By Lee Ki-hwan 
	case MSG_GUILD_STASH_HISTORY_REP:
		{
			LONG	nErrorCode;

			LONG		lMonth, lDay;
			LONGLONG	llMoney;

			(*istr) >> nErrorCode;
			
			if( nErrorCode != MSG_GUILD_STASH_ERROR_OK )
			{
				pUIManager->GetGuildStash()->ErrorProc( nErrorCode, ubType );	
				return;
			}
			
			pUIManager->GetGuildStash()->ResetTakeInfo();
			for( int i = 0; i < 7; i++ )
			{
				(*istr) >> lMonth;
				(*istr) >> lDay;
				(*istr) >> llMoney;

				pUIManager->GetGuildStash()->AddTakeInfo( lMonth, lDay, llMoney );					
			}
			pUIManager->GetGuildStash()->OpenView();

		}
		break;

	case MSG_GUILD_STASH_VIEW_REP:
		{
			
			LONG	nErrorCode;
			LONGLONG	llMoney;

			(*istr) >> nErrorCode;

			if( nErrorCode != MSG_GUILD_STASH_ERROR_OK )
			{
				pUIManager->GetGuildStash()->ErrorProc( nErrorCode, ubType );	
				return;
			}

			(*istr) >> llMoney;

			pUIManager->GetGuildStash()->OpenTake( llMoney );
		}
		break;

	case MSG_GUILD_STASH_TAKE_REP:
		{
			LONG	nErrorCode;
			(*istr) >> nErrorCode;

			if( nErrorCode != MSG_GUILD_STASH_ERROR_OK )
			{
				pUIManager->GetGuildStash()->ErrorProc( nErrorCode, ubType );	
				return;
			}

			pUIManager->GetGuildStash()->Message( MSGCMD_GUILDSTASH_INFO, _S( 2078, "금액이 성공적으로 인출 되었습니다. "), UMBS_OK ); 

		}
		break;

	case MSG_NEW_GUILD_INCLINE_ESTABLISH: 		
	case MSG_NEW_GUILD_INFO:
	case MSG_NEW_GUILD_MEMBERLIST:
	case MSG_NEW_GUILD_SKILL:	
	case MSG_NEW_GUILD_NOTICE:
	case MSG_NEW_GUILD_NOTICE_TRANSMISSION:
	case MSG_NEW_GUILD_MANAGE:
	case MSG_NEW_GUILD_POINT_RANKING:
	case MSG_GUILD_GP_INFO:
	case MSG_GUILD_WAR_SET_TIME_MENU:
	case MSG_GUILD_WAR_GET_TIME_UNI_REP:
	case MSG_NEW_GUILD_MARK_EDIT_FIN:
	case MSG_NEW_GUILD_MARK_EXPIRE:
		pUIManager->GetGuild()->ReceiveNewGuildMessage(ubType,istr);
		break;
	case MSG_NEW_GUILD_STASH_LIST:
	case MSG_NEW_GUILD_STASH_KEEP:	
	case MSG_NEW_GUILD_STASH_TAKE:	
	case MSG_NEW_GUILD_STASH_LOG:
	case MSG_NEW_GUILD_STASH_ERROR:
	case MSG_NEW_GUILD_STASH_LIST_MONEY:	//2013/04/05 jeil 나스 아이템 제거 
		pUIManager->GetGuildStash_N()->ReceiveGuildStashMessage( ubType, istr );
		break;	

	// ==> 1107 길드 시스템 개편 [trylord 11/12/28]
	case MSG_GUILD_MASTER_KICK_REP:
		{	// 길드 추방 성공
			INDEX		idxGuild;
			CTString	strGuildMName;
			CTString	strMessage[3];

			(*istr) >> (INDEX)idxGuild;
			(*istr) >> strGuildMName;

			pUIManager->CloseMessageBox(MSGCMD_GUILD_ERROR);
			
			strMessage[0].PrintF( _S( 5623, "[%s] 길드의 길드장 [%s]님의 부재로 길드장 직위가 해제될 예정입니다." ), _pNetwork->MyCharacterInfo.strGuildName, strGuildMName );
			strMessage[1].PrintF( " " );
			strMessage[2].PrintF( _S(5624, "이의 제기 : 근위대장에게 길드장이 제출"));

			CUIMsgBox_Info	MsgBoxInfo;
			MsgBoxInfo.SetMsgBoxInfo(  _S( 865, "길드" ) , UMBS_OK, UI_GUILD, MSGCMD_GUILD_ERROR );		// 확인만 하면 되므로 MSGCMD_GUILD_ERROR ㄱㄱㅅ
			MsgBoxInfo.AddString( strMessage[0] );
			MsgBoxInfo.AddString( strMessage[1] );
			MsgBoxInfo.AddString( strMessage[2] );
			pUIManager->CreateMessageBox( MsgBoxInfo );		
		}
		break;

	case MSG_GUILD_MASTER_KICK_CANCEL_REP:
		{
			INDEX		idxGuild;
			CTString	strMessage;
			CTString	strSubMessage;

			(*istr) >> (INDEX)idxGuild;
			
			pUIManager->CloseMessageBox(MSGCMD_GUILD_ERROR);

			strMessage.PrintF( _S( 5625, "[%s] 길드의 길드장 해임이 취소되었습니다." ), _pNetwork->MyCharacterInfo.strGuildName );

			CUIMsgBox_Info	MsgBoxInfo;
			MsgBoxInfo.SetMsgBoxInfo(  _S( 865, "길드" ) , UMBS_OK, UI_GUILD, MSGCMD_GUILD_ERROR );		// 확인만 하면 되므로 MSGCMD_GUILD_ERROR ㄱㄱㅅ
			MsgBoxInfo.AddString( strMessage );
			pUIManager->CreateMessageBox( MsgBoxInfo );		
		}
		break;

	case MSG_GUILD_MASTER_KICK_STATUS:
		{
			INDEX		idxGuild;
			LONG		lStatus;

			(*istr) >> idxGuild;
			(*istr) >> (LONG)lStatus;

			_pNetwork->MyCharacterInfo.iGuildMasterKickStatus	= (int)lStatus;
		}
		break;

	case MSG_GUILD_REMOTE_JOIN_REQ:
		{
			CTString	strMessage;
			CTString	strTarget;
			CTString	strOwner;
			LONG		lType;
			UI_MSGCMD	eMsgCmd;

			(*istr) >> strTarget;
			(*istr) >> strOwner;
			(*istr) >> lType;

			pUIManager->CloseMessageBox(MSGCMD_GUILD_JOIN);
			pUIManager->CloseMessageBox(MSGCMD_GUILD_JOIN_REQ);
			pUIManager->CloseMessageBox(MSGCMD_GUILD_APPLICANT_JOIN);
			pUIManager->CloseMessageBox(MSGCMD_GUILD_ERROR);

			pUIManager->GetGuild()->SetRemoteGuildJoinTaget(strOwner);

			if( strcmp(strOwner, _pNetwork->MyCharacterInfo.name) == 0)
			{	// 자기 자신이므로 정상적으로 신청되었다고 메세지를 띄워준다
				strMessage.PrintF( _S( 5629, "성공적으로 길드가입 신청하였습니다." ));

				CUIMsgBox_Info	MsgBoxInfo;
				MsgBoxInfo.SetMsgBoxInfo(  _S( 865, "길드" ) , UMBS_OK, UI_GUILD, MSGCMD_GUILD_ERROR );		
				MsgBoxInfo.AddString( strMessage );
				pUIManager->CreateMessageBox( MsgBoxInfo );		
			}
			else
			{
				
				if(lType == 0)
				{	// 유저가 신청 받음
					CUIManager::getSingleton()->CloseMessageBox( MSGCMD_GUILDREMOTE_JOIN_TOUSER );
					
					strMessage.PrintF( _S( 5628, "[%s] 님께서 길드가입 신청을 하셨습니다. 수락하시겠습니까?" ), strOwner  );
					eMsgCmd		= MSGCMD_GUILDREMOTE_JOIN_TOUSER;
				}
				else if(lType == 1)
				{	// 길마가 신청 받음
					CUIManager::getSingleton()->CloseMessageBox( MSGCMD_GUILDREMOTE_JOIN_TOMASTER );
					
					strMessage.PrintF( _S( 5628, "[%s] 님께서 길드가입 신청을 하셨습니다. 수락하시겠습니까?" ), strOwner );
					eMsgCmd		= MSGCMD_GUILDREMOTE_JOIN_TOMASTER;
				}
				
				CUIMsgBox_Info	MsgBoxInfo;
				MsgBoxInfo.SetMsgBoxInfo( _S( 865, "길드" ), UMBS_OKCANCEL, UI_GUILD, eMsgCmd);	
				MsgBoxInfo.AddString( strMessage );
				CUIManager::getSingleton()->CreateMessageBox( MsgBoxInfo );
				
			}
		}
		break;

	case MSG_GUILD_REMOTE_JOIN_OK:
		{
			CTString	strMessage;
			LONG		lType;

			(*istr) >> lType;

			pUIManager->CloseMessageBox(MSGCMD_GUILD_JOIN);
			pUIManager->CloseMessageBox(MSGCMD_GUILD_JOIN_REQ);
			pUIManager->CloseMessageBox(MSGCMD_GUILD_APPLICANT_JOIN);
			pUIManager->CloseMessageBox(MSGCMD_GUILD_ERROR);
			strMessage.PrintF( _S( 982, "길드에 가입되었습니다." )  );

			CUIMsgBox_Info	MsgBoxInfo;
			MsgBoxInfo.SetMsgBoxInfo(  _S( 865, "길드" ) , UMBS_OK, UI_GUILD, MSGCMD_GUILD_ERROR );		
			MsgBoxInfo.AddString( strMessage );
			pUIManager->CreateMessageBox( MsgBoxInfo );		
		}
		break;

	case MSG_GUILD_REMOTE_JOIN_NO:
		{
			CTString	strMessage;
			LONG		lType;

			(*istr) >> lType;

			pUIManager->CloseMessageBox(MSGCMD_GUILD_JOIN);
			pUIManager->CloseMessageBox(MSGCMD_GUILD_JOIN_REQ);
			pUIManager->CloseMessageBox(MSGCMD_GUILD_APPLICANT_JOIN);
			pUIManager->CloseMessageBox(MSGCMD_GUILD_ERROR);
			strMessage.PrintF( _S( 999, "길드 가입 신청이 취소되었습니다." )  );

			CUIMsgBox_Info	MsgBoxInfo;
			MsgBoxInfo.SetMsgBoxInfo(  _S( 865, "길드" ) , UMBS_OK, UI_GUILD, MSGCMD_GUILD_ERROR );		
			MsgBoxInfo.AddString( strMessage );
			pUIManager->CreateMessageBox( MsgBoxInfo );		
		}
		break;
	// <== 1107 길드 시스템 개편 [trylord 11/12/28]
	}
}

// ----------------------------------------------------------------------------
// Name : ReceiveTeachMessage()
// Desc : 
// ----------------------------------------------------------------------------
void CSessionState::ReceiveTeachMessage(CNetworkMessage *istr)
{
	UBYTE	ubType;
	(*istr) >> ubType;

	CUIManager* pUIManager = CUIManager::getSingleton();

	switch(ubType)
	{
	case MSG_TEACH_TEACHER_LIST:		// 선생 리스트
		{
			UBYTE ubSubType;
			(*istr) >> ubSubType;

			switch( ubSubType )
			{
			case MSG_TEACH_TEACHER_LIST_UP:		// 나를 선생 목록에 올림
				{
					pUIManager->GetHelper()->SetRegistredTeacher( TRUE );

					CTString strMessage;
					strMessage.PrintF( _S( 1347, "[%s]님이 후견인 목록에 추가되었습니다." ), _pNetwork->MyCharacterInfo.name );		
					pUIManager->GetChattingUI()->AddSysMessage( strMessage, SYSMSG_NORMAL );
				}
				break;
			case MSG_TEACH_TEACHER_LIST_DN:		// 나를 선생 목록에서 내림
				{
					pUIManager->GetHelper()->SetRegistredTeacher( FALSE );

					CTString strMessage;
					strMessage.PrintF( _S( 1348, "[%s]님이 후견인 목록에서 삭제되었습니다." ), _pNetwork->MyCharacterInfo.name );		
					pUIManager->GetChattingUI()->AddSysMessage( strMessage, SYSMSG_NORMAL );
				}
				break;
			case MSG_TEACH_TEACHER_LIST_SHOW:
				{
					LONG		lCharIndex;
					CTString	strCharName;
					SBYTE		sbCharJob;
					SBYTE		sbCharJob2;

					pUIManager->GetHelper()->ClearHelperList();

					for( int i = 0; i < HELPER_MAX_STUDENTS; ++i )
					{
						(*istr) >> lCharIndex;

						if( lCharIndex != -1 )
						{
							(*istr) >> strCharName;
							(*istr) >> sbCharJob;
							(*istr) >> sbCharJob2;

							SLONG fame, cntTeachingStudent, cntCompleteStudent, cntFailStudent;
							(*istr) >> fame;
							(*istr) >> cntTeachingStudent;
							(*istr) >> cntCompleteStudent;
							(*istr) >> cntFailStudent;

							pUIManager->GetHelper()->AddToHelperList( lCharIndex, strCharName, -1, sbCharJob, sbCharJob2 );
							SBYTE iStartPlayTime;
							SBYTE iEndPlayTime;
							(*istr) >> iStartPlayTime;
							(*istr) >> iEndPlayTime;
							pUIManager->GetHelper()->AddToTeacherInfoList(lCharIndex, fame, cntTeachingStudent, cntCompleteStudent, cntFailStudent, iStartPlayTime, iEndPlayTime );
						}
					}
					pUIManager->GetHelper()->RefreshTeacherList();
				}
				break;
			}
		}
		break;

	case MSG_TEACH_TEACHER_REQ:		// 선생 요청 과정
		{
			UBYTE ubSubType;
			(*istr) >> ubSubType;

			switch( ubSubType )
			{
			case MSG_TEACH_TEACHER_REQ_REQ:
				{
					LONG		lCharIndex;
					CTString	strCharName;

					(*istr) >> lCharIndex;
					(*istr) >> strCharName;
					pUIManager->GetHelper()->TeacherAccept( lCharIndex, strCharName );
				}
				break;
			case MSG_TEACH_TEACHER_REQ_REJECT:
				{
					//UBYTE		ubCharType;
					LONG		lRejectCharIndex;
					CTString	strCharName;

					pUIManager->CloseMessageBox( MSGCMD_HELPER_STUDENT_ACCEPT );
					pUIManager->CloseMessageBox( MSGCMD_HELPER_BEMYTEACHER_REQ );

					//(*istr) >> ubCharType;
					(*istr) >> lRejectCharIndex;
					(*istr) >> strCharName;
					// 선생 요청 거절	: char_type(uc) reject_charindex(n) reject_charname(str)

					if( lRejectCharIndex == _pNetwork->MyCharacterInfo.index )	// 내가 거절했을때...
					{
					}
					else		// 다른 사람이 거절했을때...
					{
						CTString strMessage;
						strMessage.PrintF( _S( 1349, "[%s]님이 거절하였습니다." ), strCharName );		
						pUIManager->GetChattingUI()->AddSysMessage( strMessage, SYSMSG_NORMAL );
					}
					
					//_pNetwork->MyCharacterInfo.lTeacherIndex = -1;
				}
				break;

			case MSG_TEACH_TEACHER_REQ_ACCEPT:
				{
					LONG		lTeacherIndex;
					CTString	strTeacherName;
					LONG		lStudentIndex;
					CTString	strStudentName;

					pUIManager->CloseMessageBox( MSGCMD_HELPER_BEMYTEACHER_REQ );

					(*istr) >> lTeacherIndex;
					(*istr) >> strTeacherName;
					(*istr) >> lStudentIndex;
					(*istr) >> strStudentName;

					INDEX		iCurTeachCnt;
					INDEX		iCompleteTeachCnt;
					INDEX		iFailTeachCnt;

					(*istr) >> iCurTeachCnt;
					(*istr) >> iCompleteTeachCnt;
					(*istr) >> iFailTeachCnt;
					// 선생 허락		: teacher_index(n) teacher_name(str) student_index(n) student_name(str)

					CTString strMessage;
					if( lStudentIndex == _pNetwork->MyCharacterInfo.index )
					{
						// 내가 학생인데, 선생이 생긴경우...
						strMessage.PrintF( _S( 1350, "[%s]님이 [%s]님의 후견인이 되었습니다." ), strTeacherName, strStudentName );		
						pUIManager->GetChattingUI()->AddSysMessage( strMessage, SYSMSG_NORMAL );
					}
					else
					{						
						pUIManager->GetHelper()->SetMyTeachInfo(iCurTeachCnt, iCompleteTeachCnt, iFailTeachCnt);
						strMessage.PrintF( _S( 1351, "[%s]님이 [%s]님의 견습생이 되었습니다." ), strStudentName, strTeacherName );		
						pUIManager->GetChattingUI()->AddSysMessage( strMessage, SYSMSG_NORMAL );
					}
				}
				break;
			case MSG_TEACH_TEACHER_GIVEUP:
				{
					// 학생 포기 요청	: teacher_index(n) teacher_name(str) student_index(n) student_name(str)

					LONG		lTeacherIndex;
					CTString	strTeacherName;
					LONG		lStudentIndex;
					CTString	strStudentName;
					INDEX		iGiveupCount;

					(*istr) >> lTeacherIndex;
					if ( lTeacherIndex != -1 )
					{
						(*istr) >> strTeacherName;
					}
					(*istr) >> lStudentIndex;
					(*istr) >> strStudentName;
					(*istr) >> iGiveupCount;
					
					if( lTeacherIndex == _pNetwork->MyCharacterInfo.index )
					{	// 내가 선생일 때,
						if ( iGiveupCount == -1 )
						{
							INDEX iCurTeachCnt, iCompleteTeachCnt, iFailTeachCnt;
							(*istr) >> iCurTeachCnt;
							(*istr) >> iCompleteTeachCnt;
							(*istr) >> iFailTeachCnt;
							pUIManager->GetHelper()->SetMyTeachInfo( iCurTeachCnt, iCompleteTeachCnt, iFailTeachCnt );
						}

						CTString strMessage;
						strMessage.PrintF( _S( 5096, "[%s]님과의 후견인 관계가 해지되었습니다." ), strStudentName );		
						pUIManager->GetChattingUI()->AddSysMessage( strMessage, SYSMSG_NORMAL );

						pUIManager->GetHelper()->DelFromHelperList( lStudentIndex );
						pUIManager->GetHelper()->RefreshStudentList();
					}
					else if ( lStudentIndex == _pNetwork->MyCharacterInfo.index )
					{	// 내가 학생일 때,
						if ( iGiveupCount != -1 )
						{
							_pNetwork->MyCharacterInfo.iStudentGiveUpCnt= iGiveupCount;
						}
						CTString strMessage;
						strMessage.PrintF( _S( 5096, "[%s]님과의 후견인 관계가 해지되었습니다." ), _pNetwork->MyCharacterInfo.strTeacherName );		
						pUIManager->GetChattingUI()->AddSysMessage( strMessage, SYSMSG_NORMAL );

						_pNetwork->MyCharacterInfo.lTeacherIndex	= -1;
						_pNetwork->MyCharacterInfo.strTeacherName.Clear();
						pUIManager->GetHelper()->ClearHelperList();
						pUIManager->GetHelper()->ResetHelper();
					}
				}
				break;
			}
		}
		break;
	case MSG_TEACH_END:			// 종료
		{
			LONG		lTeacherIndex;
			CTString	strTeacherName;
			LONG		lStudentIndex;
			CTString	strStudentName;
			UBYTE		ubEndType;
			SBYTE		sbEvent;
			LONG		lUpFame = 0;
			INDEX		iCurTeachCnt, iCompleteTeachCnt, iFailTeachCnt;

			(*istr) >> lTeacherIndex;
			(*istr) >> strTeacherName;
			(*istr) >> lStudentIndex;
			(*istr) >> strStudentName;
			(*istr) >> ubEndType;
			(*istr) >> sbEvent;
			(*istr) >> lUpFame;
			(*istr)	>> iCurTeachCnt;
			(*istr)	>> iCompleteTeachCnt;
			(*istr)	>> iFailTeachCnt;

			switch( ubEndType )
			{
			case MSG_TEACH_END_FAIL:		// 실패
				{
					CTString strMessage;

					// 내가 선생이었을 때...
					if( lTeacherIndex == _pNetwork->MyCharacterInfo.index )
					{
						pUIManager->GetHelper()->DelFromHelperList( lStudentIndex );
						pUIManager->GetHelper()->RefreshStudentList();
						pUIManager->GetHelper()->SetMyTeachInfo( iCurTeachCnt, iCompleteTeachCnt, iFailTeachCnt );
					}
					// 내가 학생이었을 때...
					else
					{
						pUIManager->GetHelper()->DelFromHelperList( lTeacherIndex );
						pUIManager->GetHelper()->RefreshTeacherList();
						_pNetwork->MyCharacterInfo.lTeacherIndex	= -1;
						_pNetwork->MyCharacterInfo.strTeacherName.Clear();
					}

					strMessage.PrintF( _S( 5102, "[%s]님이 10일동안 접속하지 않아 후견인 관계가 강제 종료되었습니다." ), strStudentName );
					pUIManager->GetChattingUI()->AddSysMessage( strMessage );
				}
				break;

			case MSG_TEACH_END_SUCCESS:		// 성공
				{
					CTString strMessage;

					pUIManager->CloseMessageBox( MSGCMD_HELPER_NOTIFY );

					CUIMsgBox_Info	MsgBoxInfo;

// [KH_070417] 스승의날 이벤트 관련 추가
					if(IS_EVENT_ON(TEVENT_TEACHER_2007))
						MsgBoxInfo.SetMsgBoxInfo(_S(3344, "스승의 은혜 이벤트"), UMBS_OK, UI_NONE,MSGCMD_NULL);
					else
						MsgBoxInfo.SetMsgBoxInfo( _S( 1355, "견습생 양성 성공" ), UMBS_OK, UI_HELPER, MSGCMD_HELPER_NOTIFY );	
					
					// 내가 선생이었을 때...
					if( lTeacherIndex == _pNetwork->MyCharacterInfo.index )
					{	
						int iPlusFame	= 0;
						// FIXME : 이렇게 엔진 버젼 정보를 체크하는거 맘에 안듬...						
						extern UINT g_uiEngineVersion;
						if( g_uiEngineVersion >= 10000 )
							iPlusFame = 20;
						else						
							iPlusFame = 10;

						if( sbEvent == 1 )
						{
#ifndef TEACHER_PRIZE_EVENT_2PAN4PAN		// 후견인 명성 보상 이벤트가 아닌경우.
							strMessage.PrintF( _S( 1356, "[%s]님의 레벨이 %d이 되어 후견인 관계가 종료 되었습니다." ), strStudentName, pUIManager->GetHelper()->GetLimitLevel());	

							if (lUpFame > 0)
							{
								CTString strAddString;
								strAddString.PrintF(_S(5046, "명성치가 +%d상승하였습니다."), lUpFame);

								strMessage += strAddString;
							}
#else
							iPlusFame = 20;
// [KH_070417] 스승의날 이벤트 관련 추가 (혹시 몰라 넣음)
							if(IS_EVENT_ON(TEVENT_TEACHER_2007))
								MsgBoxInfo.AddString(_S(3346, "축하합니다. 견습생 양성을 성공적으로 이루어 내 노란 카네이션을 획득 하였습니다."));
							else
							{
								strMessage.PrintF( _S( 1356, "[%s]님의 레벨이 %d이 되어 후견인 관계가 종료 되었습니다." ), strStudentName, pUIManager->GetHelper()->GetLimitLevel());	

								if (lUpFame > 0)
								{
									CTString strAddString;
									strAddString.PrintF(_S(5046, "명성치가 +%d상승하였습니다."), lUpFame);

									strMessage += strAddString;
								}
							}
#endif							
						}
						else
						{
// [KH_070417] 스승의날 이벤트 관련 추가
							if(IS_EVENT_ON(TEVENT_TEACHER_2007))
								MsgBoxInfo.AddString(_S(3346, "축하합니다. 견습생 양성을 성공적으로 이루어 내 노란 카네이션을 획득 하였습니다."));
							else
							{
								strMessage.PrintF( _S( 1356, "[%s]님의 레벨이 %d이 되어 후견인 관계가 종료 되었습니다." ), strStudentName, pUIManager->GetHelper()->GetLimitLevel());	

								if (lUpFame > 0)
								{
									CTString strAddString;
									strAddString.PrintF(_S(5046, "명성치가 +%d상승하였습니다."), lUpFame);

									strMessage += strAddString;
								}
							}
						}
						pUIManager->GetHelper()->SetMyTeachInfo( iCurTeachCnt, iCompleteTeachCnt, iFailTeachCnt );
						pUIManager->GetHelper()->DelFromHelperList( lStudentIndex );
						pUIManager->GetHelper()->RefreshStudentList();
					}
					// 내가 학생이었을 때...
					else
					{	
#ifndef TEACHER_PRIZE_EVENT_2PAN4PAN		// 후견인 명성 보상 이벤트가 아닌경우.
						if( sbEvent == 1 )
						{
							// NOTE : 이벤트일때...
							strMessage.PrintF( _S(2079, "후견인 시스템을 통하여 성공적으로 성장하였으므로 이벤트검이 지급됩니다.") );		
						}
						else
						{
							strMessage.PrintF( _S( 1357, "레벨이 %d이 되어 [%s]님과의 후견인 관계가 종료 되었습니다." ), pUIManager->GetHelper()->GetLimitLevel(), strTeacherName );	
						}
#else
// [KH_070417] 스승의날 이벤트 관련 추가
						if(IS_EVENT_ON(TEVENT_TEACHER_2007))
							MsgBoxInfo.AddString(_S(3347, "축하합니다. 레벨 20을 달성하여 스승의날 이벤트 노란 견장과 이벤트검을 획득 하였습니다."));
						else
							strMessage.PrintF( _S( 1357, "레벨이 %d이 되어 [%s]님과의 후견인 관계가 종료 되었습니다." ), pUIManager->GetHelper()->GetLimitLevel(), strTeacherName );	
#endif

						pUIManager->GetHelper()->ClearHelperList();
						pUIManager->GetHelper()->ResetHelper();
						pUIManager->GetHelper()->SetMyTeachInfo( 0, 0, 0 );

						_pNetwork->MyCharacterInfo.lTeacherIndex	= -1;
						_pNetwork->MyCharacterInfo.strTeacherName.Clear();
					}
					
					MsgBoxInfo.AddString( strMessage );
					pUIManager->CreateMessageBox( MsgBoxInfo );
				}
				break;
			}
		}
		break;
	case MSG_TEACH_INFO:		// 사제 정보
		{
			// 자신이 선생인지 학생인지에 따라서...
			SBYTE		sbRegistered;
			SBYTE		sbCharType;
			LONG		lTeacherIndex;
			LONG		lTeacherLevel;
			CTString	strTeacherName;
			SBYTE		sbTeacherJob;
			SBYTE		sbTeacherJob2;
			
			(*istr) >> sbRegistered;
			(*istr) >> sbCharType;			
							
			// 후견인으로 등록되어 있는 경우.				
			pUIManager->GetHelper()->SetRegistredTeacher( sbRegistered );

			// 사제 정보를 받을때 전체 정보를 받기 때문에 무조건 초기화하여 갱신한다. [10/31/2011 ldy1978220]
			pUIManager->GetHelper()->ClearHelperList();
			
			// 자신이 학생일 경우, 이미 후견인 있음.
			if( sbCharType == MSG_TEACH_STUDENT_TYPE )
			{
				INDEX iGiveUpCount;
				(*istr) >> iGiveUpCount;
				_pNetwork->MyCharacterInfo.iStudentGiveUpCnt= iGiveUpCount;

				(*istr) >> lTeacherIndex;
				(*istr) >> lTeacherLevel;
				(*istr) >> strTeacherName;
				(*istr) >> sbTeacherJob;
				(*istr) >> sbTeacherJob2;
				
				_pNetwork->MyCharacterInfo.lTeacherIndex	= lTeacherIndex;
				_pNetwork->MyCharacterInfo.strTeacherName	= strTeacherName;

				SLONG fame, cntTeachingStudent, cntCompleteStudent, cntFailStudent;
				(*istr) >> fame;
				(*istr) >> cntTeachingStudent;
				(*istr) >> cntCompleteStudent;
				(*istr) >> cntFailStudent;

//				pUIManager->GetHelper()->ClearHelperList();
				pUIManager->GetHelper()->AddToHelperList( lTeacherIndex, strTeacherName, lTeacherLevel, sbTeacherJob, sbTeacherJob2 );
				pUIManager->GetHelper()->AddToTeacherInfoList(lTeacherIndex, fame, cntTeachingStudent, cntCompleteStudent, cntFailStudent);
				pUIManager->GetHelper()->RefreshTeacherList();
			}
			// 자신이 선생일 경우.
			else if( sbCharType == MSG_TEACH_TEACHER_TYPE )
			{
				// 학생들 목록을 가져옴.
				//pUIManager->GetHelper()->ClearHelperList();
				int nLoopCnt = 8;
				INDEX iCurTeachCnt, iCompleteTeachCnt, iFailTeachCnt;
				
				(*istr) >> iCurTeachCnt;
				(*istr) >> iCompleteTeachCnt;
				(*istr) >> iFailTeachCnt;
				pUIManager->GetHelper()->SetMyTeachInfo( iCurTeachCnt, iCompleteTeachCnt, iFailTeachCnt );
				nLoopCnt = 10;

				for( int i = 0; i < nLoopCnt; ++i )
				{
					LONG		lStudentIndex;
					LONG		lStudentLevel;
					CTString	strStudentName;
					SBYTE		sbStudentJob;
					SBYTE		sbStudentJob2;
					
					(*istr) >> lStudentIndex;
					
					if( lStudentIndex != -1 )
					{
						(*istr) >> lStudentLevel;
						(*istr) >> strStudentName;
						(*istr) >> sbStudentJob;
						(*istr) >> sbStudentJob2;
						pUIManager->GetHelper()->AddToHelperList( lStudentIndex, strStudentName, lStudentLevel, sbStudentJob, sbStudentJob2 );
						CTString strFirstDate, strFinalDate;

						pUIManager->GetHelper()->AddToStudentInfoList(lStudentIndex, strFirstDate.str_String, strFinalDate.str_String);
					}
				}
				pUIManager->GetHelper()->RefreshStudentList();
			}
		}
		break;
	case MSG_TEACH_STUDENT_LEVELUP:		// 학생 레벨업
		{
			// FIXME : 레벨만 변경되는게 보장 되어야 함.
			LONG		lStudentIndex;
			LONG		lStudentLevel;
			CTString	strStudentName;
			SBYTE		sbStudentJob;
			SBYTE		sbStudentJob2;

			(*istr) >> lStudentIndex;
			(*istr) >> lStudentLevel;
			(*istr) >> strStudentName;
			(*istr) >> sbStudentJob;
			(*istr) >> sbStudentJob2;

			pUIManager->GetHelper()->ChangeHelperLevel( lStudentIndex, lStudentLevel );
			pUIManager->GetHelper()->RefreshStudentList( );

			// 내가 해당 학생인경우...
			if( lStudentIndex == _pNetwork->MyCharacterInfo.index )
			{
				// String Index : 5084, 5085, 5086, 5087, 5088, 5089, 5090, 5091, 5092, 5093
				if ( lStudentLevel%5 == 0 )
				{
					int nIndex = lStudentLevel/5 + 5083;
					CTString strMessage;
					strMessage.PrintF( _S( nIndex,""), _pNetwork->MyCharacterInfo.name, lStudentLevel );
					_UIAutoHelp->SetGMNotice( strMessage, 0x38FFD7FF );
				}
				pUIManager->GetHelper()->ChangeHelperLevel( lStudentIndex, lStudentLevel );
			}
		}
		break;
	case MSG_TEACH_ERR:			// 에러
		{
			UBYTE ubErrorType;
			CTString strMessage;

			pUIManager->CloseMessageBox( MSGCMD_HELPER_NOTIFY );
			pUIManager->CloseMessageBox( MSGCMD_HELPER_BEMYTEACHER );
			pUIManager->CloseMessageBox( MSGCMD_HELPER_BEMYTEACHER_REQ );

			(*istr) >> ubErrorType;

			switch( ubErrorType )
			{
			case MSG_TEACH_ERR_SYSTEM:					// 나오면 안되는 오류
				ASSERTALWAYS("Invalid Error Message!!!");
				break;
			case MSG_TEACH_ERR_TEACHER_LEVEL:			// 선생 가능레벨이 아님
				strMessage.PrintF( _S( 1358, "후견인으로 등록 가능한 레벨이 아닙니다."  ) );				
				pUIManager->GetChattingUI()->AddSysMessage( strMessage, SYSMSG_ERROR );
				break;
			case MSG_TEACH_ERR_TEACHER_LIST_ALREADY:	// 이미 선생 리스트에 있음
				strMessage.PrintF( _S( 1359, "이미 후견인 리스트에 등록되어 있습니다."  ) );				
				pUIManager->GetChattingUI()->AddSysMessage( strMessage, SYSMSG_ERROR );
				break;
			case MSG_TEACH_ERR_TEACHER_LIST_NOT:		// 선생 리스트에 있지 않음
				strMessage.PrintF( _S( 1360, "후견인 리스트에 등록 되어 있지 않습니다."  ) );				
				pUIManager->GetChattingUI()->AddSysMessage( strMessage, SYSMSG_ERROR );
				break;
			case MSG_TEACH_ERR_TEACHER_REQ_ALREADY:		// 이미 신청중
				strMessage.PrintF( _S( 1361, "이미 신청중입니다."  ) );				
				pUIManager->GetChattingUI()->AddSysMessage( strMessage, SYSMSG_ERROR );
				break;
			case MSG_TEACH_ERR_STUDENT_LEVEL:			// 학생 가능레벨이 아님
				strMessage.PrintF( _S( 1362, "견습생이 가능한 레벨이 아닙니다."  ) );				
				pUIManager->GetChattingUI()->AddSysMessage( strMessage, SYSMSG_ERROR );
				break;
			case MSG_TEACH_ERR_HAVE_TEACHER:			// 이미 선생이 있음
				strMessage.PrintF( _S( 1363, "이미 후견인이 있습니다."  ) );				
				pUIManager->GetChattingUI()->AddSysMessage( strMessage, SYSMSG_ERROR );
				break;
			case MSG_TEACH_ERR_TEACHER_NOT_CONNECT:		// 선생이 접속중이 아님
				strMessage.PrintF( _S( 1364, "후견인이 접속중이 아닙니다."  ) );				
				pUIManager->GetChattingUI()->AddSysMessage( strMessage, SYSMSG_ERROR );
				break;
			case MSG_TEACH_ERR_STUDENT_NOT_CONNECT:		// 학생이 접속중이 아님
				strMessage.PrintF( _S( 1365, "견습생이 접속중이 아닙니다."  ) );				
				pUIManager->GetChattingUI()->AddSysMessage( strMessage, SYSMSG_ERROR );
				break;
			case MSG_TEACH_ERR_TEACHER_FULL:			// 제자가 꽉 차 있음
				strMessage.PrintF( _S( 1366, "더 이상 견습생을 받을 수 없습니다."  ) );				
				pUIManager->GetChattingUI()->AddSysMessage( strMessage, SYSMSG_ERROR );
				break;
			case MSG_TEACH_ERR_NOE_REQ:					// 신청한 적 없음
				strMessage.PrintF( _S( 1367, "신청한 적이 없습니다."  ) );				
				pUIManager->GetChattingUI()->AddSysMessage( strMessage, SYSMSG_ERROR );
				break;
			case MSG_TEACH_ERR_NOMORE_STUDENT:			// 더이상 학생 할 수 없음
				strMessage.PrintF( _S( 1368, "더 이상 견습생을 할 수 없습니다."  ) );				
				pUIManager->GetChattingUI()->AddSysMessage( strMessage, SYSMSG_ERROR );
				break;
			case MSG_TEACH_ERR_FAME:
				strMessage.PrintF( _S( 1340, "명성치가 부족합니다." ) );				
				pUIManager->GetChattingUI()->AddSysMessage( strMessage, SYSMSG_ERROR );
				break;
			default:
				ASSERTALWAYS("Invalid Error Message!!!");
				break;
			}
		}
		break;
	case MSG_TEACH_RENEWER_TEACH_GIFT:
		{
			UBYTE ubErrorType;
			CTString strMessage;
			
			(*istr) >> ubErrorType;
			switch( ubErrorType )
			{
			case MSG_TEACH_GIFT_ERROR:
				{
					strMessage.PrintF( _S( 1722, "보상을 받으실 수 있는 조건이 아닙니다"));
					pUIManager->GetChattingUI()->AddSysMessage( strMessage, SYSMSG_ERROR );
				}
				break;
			case MSG_TEACH_GIFT_SUC:
				{

				}
				break;
			case MSG_TEACH_GIFT_NOMORE:
				{
					strMessage.PrintF( _S(4676, "모든 보상을 받았습니다. 더이상 보상받을 아이템이 없습니다.") );
					pUIManager->GetChattingUI()->AddSysMessage( strMessage, SYSMSG_ERROR );
				}
				break;
			}
		}
		break;
	case MSG_TEACH_LOGIN:
		{
			SBYTE iTeachType;
			INDEX iCharIndex;
			CTString strName, strMessage;

			(*istr) >> iTeachType;
			(*istr) >> iCharIndex;
			(*istr) >> strName;
			if ( iTeachType == MSG_TEACH_STUDENT_TYPE )
			{	// 학생일 경우,
				strMessage.PrintF( _S( 5095, "견습생 [%s]님이 게임에 접속하셨습니다."), strName );
			}
			else
			{
				strMessage.PrintF( _S( 5094, "후견인 [%s]님이 게임에 접속하셨습니다."), strName );
			}
			pUIManager->GetChattingUI()->AddSysMessage( strMessage );
		}
		break;
	}	
}

// ----------------------------------------------------------------------------
// Name : ReceiveChangeJobMessage()
// Desc : 
// ----------------------------------------------------------------------------
void CSessionState::ReceiveChangeJobMessage(CNetworkMessage *istr)
{
	pTypeBase* pBase = reinterpret_cast<pTypeBase*>(istr->GetBuffer());
		
	CUIManager* pUIManager = CUIManager::getSingleton();

	switch(pBase->subType)
	{
	case MSG_CHANGEJOB_REP:			// 전직 알림		: charindex(n) job(c) job2(c)
		{
			ResponseClient::changeJob* pPack = reinterpret_cast<ResponseClient::changeJob*>(istr->GetBuffer());

			// 나
			if( pPack->nIndex == _pNetwork->MyCharacterInfo.index )
			{
				_pNetwork->MyCharacterInfo.job				= pPack->job1;
				_pNetwork->MyCharacterInfo.job2				= pPack->job2;

				CTString strMessage;
				strMessage.PrintF( _S( 1369, "[%s]로 전직 되었습니다." ), CJobInfo::getSingleton()->GetName( pPack->job1, pPack->job2 ) );				
				pUIManager->GetChattingUI()->AddSysMessage( strMessage, SYSMSG_NORMAL );

				// 스페셜 리스트 일때만...
				if( pPack->job1 == SORCERER && 
					pPack->job2 == 2 )
				{					
					pUIManager->GetQuickSlot()->RemoveSummonSkill();
				}

				pUIManager->GetSkillNew()->InitArrayData();

				if (pUIManager->GetSkillNew()->IsVisible() == TRUE || pUIManager->GetSkillNew()->GetHide() == FALSE)
					pUIManager->GetSkillNew()->UpdateSkillData(true);
			}
			// 타 캐릭터
			else
			{
				ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_CHARACTER, pPack->nIndex);

				if (pObject != NULL)
				{
					CCharacterTarget* pTarget = static_cast< CCharacterTarget* >(pObject);

					pTarget->m_nType	= pPack->job1;
					pTarget->cha_iJob2	= pPack->job2;
					}
				}
			}
		break;
	case MSG_CHANGEJOB_RESET_REP:	// 직업 포기 알림	: charindex(n) job(c)
		{
			ResponseClient::changeJobReset* pPack = reinterpret_cast<ResponseClient::changeJobReset*>(istr->GetBuffer());

			// 나
			if( pPack->charIndex == _pNetwork->MyCharacterInfo.index )
			{
				CTString strMessage;
				strMessage.PrintF( _S( 1370, "[%s] 직업이 포기 되었습니다." ),  CJobInfo::getSingleton()->GetName( _pNetwork->MyCharacterInfo.job, _pNetwork->MyCharacterInfo.job2 ) );				
				pUIManager->GetChattingUI()->AddSysMessage( strMessage, SYSMSG_NORMAL );
				
				// 스페셜 리스트 일때만...
				if( _pNetwork->MyCharacterInfo.job == SORCERER && 
					_pNetwork->MyCharacterInfo.job2 == 1 )
				{
					pUIManager->GetQuickSlot()->RemoveElementalSkill();
				}
				
				_pNetwork->MyCharacterInfo.job				= pPack->job;
				_pNetwork->MyCharacterInfo.job2				= 0;

				pUIManager->GetInitJob()->CloseInitJob();

				pUIManager->GetSkillNew()->InitArrayData();

				if (pUIManager->GetSkillNew()->IsVisible() == TRUE || pUIManager->GetSkillNew()->GetHide() == FALSE)
					pUIManager->GetSkillNew()->UpdateSkillData(true);
			}
			// 타 캐릭터.
			else
			{
				ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_CHARACTER, pPack->charIndex);

				if (pObject != NULL)
				{
					CCharacterTarget* pTarget = static_cast< CCharacterTarget* >(pObject);

					pTarget->m_nType	= pPack->job;
					pTarget->cha_iJob2	= 0;
				}
			}
		}
		break;
	case MSG_CHANGEJOB_ERROR:			// 에러
		{
			ResponseClient::changeJobError* pPack = reinterpret_cast<ResponseClient::changeJobError*>(istr->GetBuffer());
			CTString strMessage;

			switch( pPack->errorCode )
			{			
			case MSG_CHANGEJOB_ERROR_OK:				
				break;			
			case MSG_CHANGEJOB_ERROR_WRONGMSG:
				strMessage.PrintF( _S( 1371, "잘못 된 메세지입니다."  ) );				
				pUIManager->GetChattingUI()->AddSysMessage( strMessage, SYSMSG_ERROR );
				break;
			case MSG_CHANGEJOB_ERROR_ALREADY:
				strMessage.PrintF( _S( 1372, "이미 전직했습니다."  ) );				
				pUIManager->GetChattingUI()->AddSysMessage( strMessage, SYSMSG_ERROR );
				break;
			case MSG_CHANGEJOB_ERROR_NOCONDITION_LEVEL:
				strMessage.PrintF( _S( 1373, "레벨이 부족하여 전직할 수 없습니다."  ) );				
				pUIManager->GetChattingUI()->AddSysMessage( strMessage, SYSMSG_ERROR );
				break;
			case MSG_CHANGEJOB_ERROR_NOCONDITION_SKILL:
				strMessage.PrintF( _S( 1374, "스킬이 없어서 전직할 수 없습니다."  ) );				
				pUIManager->GetChattingUI()->AddSysMessage( strMessage, SYSMSG_ERROR );
				break;
			case MSG_CHANGEJOB_ERROR_NOCONDITION_SKILLLEVEL:
				strMessage.PrintF( _S( 1375, "스킬 레벨이 부족하여 전직할 수 없습니다."  ) );				
				pUIManager->GetChattingUI()->AddSysMessage( strMessage, SYSMSG_ERROR );
				break;
			case MSG_CHANGEJOB_ERROR_NOMONEY:
				strMessage.PrintF( _S( 1376, "돈이 부족하여 전직할 수 없습니다."  ) );		
				pUIManager->GetChattingUI()->AddSysMessage( strMessage, SYSMSG_ERROR );
				break;
			default:
				ASSERTALWAYS("Invalid Error Message!!!");
				break;
			}
		}
		break;
	}
}

//------------------------------------------------------------------------------
// CSessionState::ReceiveMessengerMessage
// Explain:  메신저 관련 메세지 처리
// Date : 2005-05-21,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CSessionState::ReceiveMessengerMessage( CNetworkMessage *istr )
{
	UBYTE	ubType;
	(*istr) >> ubType;

	CUIManager* pUIManager = CUIManager::getSingleton();

	switch(ubType)
	{
	// 친구 리스트를 받는다.
	case MSG_FRIEND_MEMBERLIST:
		{
			LONG		nCount;
			LONG		lCharIndex;  
			LONG		lGroupIndex;  
			CTString	strReqCharName;
			LONG		lJob;
			LONG		lCondition;

			(*istr) >> nCount;
			int i;
			for( i = 0; i < nCount; i++ )
			{
				(*istr) >> lCharIndex;
				(*istr) >> strReqCharName;
				(*istr) >> lJob;
				(*istr) >> lCondition;
				(*istr) >> lGroupIndex;

				pUIManager->GetMessenger()->AddFriendList( lCharIndex, lGroupIndex, strReqCharName, (eJob)lJob, (eCondition)lCondition, false);
			}
		}
		break;
	default:
		break;
	}
}

void CSessionState::ReceiveMessengerExMessage( CNetworkMessage *istr )
{
	UBYTE	exType;
	(*istr) >> exType;

	CUIManager* pUIManager = CUIManager::getSingleton();

	switch(exType)
	{
		case MSG_EX_MESSENGER_GROUP_LIST :
		{
			LONG		nCount;
			LONG		nGroupIndex;  
			CTString	strGroupName;

			(*istr) >> nCount;
			for(int i=0; i<nCount; ++i)
			{
				(*istr) >> nGroupIndex;
				(*istr) >> strGroupName;
				pUIManager->GetMessenger()->AddGroup(nGroupIndex, strGroupName);
			}

			pUIManager->GetMessenger()->SetDefaultGroupName(_S(2986, "라스트카오스친구"));
		
		}
		break;

		case MSG_EX_MESSENGER_GROUP_ADD :
		{
			LONG		nGroupIndex;  
			CTString	strName;

			(*istr) >> nGroupIndex;
			(*istr) >> strName;
			pUIManager->GetMessenger()->AddGroup(nGroupIndex, strName);
		
		}
		break;

		case MSG_EX_MESSENGER_GROUP_DEL :
		{
			//LONG		nGroupIndex;  

			//(*istr) >> nGroupIndex;
		}
		break;

		case MSG_EX_MESSENGER_GROUP_CHANGE :
		{
			LONG		nGroupIndex;
			CTString	strName;
			
			(*istr) >> nGroupIndex;
			(*istr) >> strName;

			pUIManager->GetMessenger()->RenameGroup(nGroupIndex, strName);
		}
		break;

		case MSG_EX_MESSENGER_INVITE :
		{
			LONG		lMakeCharIndex;  
			LONG		lChatIndex;  
			CTString	strName;

			(*istr) >> lMakeCharIndex;
			(*istr) >> lChatIndex;
			(*istr) >> strName;

			pUIManager->GetMessenger()->SetChat(lMakeCharIndex, lChatIndex, strName);
		}
		break;

		case MSG_EX_MESSENGER_OUT :
		{
			LONG		lMakeCharIndex;  
			LONG		lChatIndex;  
			CTString	strName;

			(*istr) >> lMakeCharIndex;
			(*istr) >> lChatIndex;
			(*istr) >> strName;

			pUIManager->GetMessenger( )->DeleteChatMember( lMakeCharIndex, lChatIndex, strName);
		}
		break;

		case MSG_EX_MESSENGER_CHAT:
		{
			LONG		lMakeCharIndex;
			LONG		lChatIndex;
			LONG		lColIndex;
			CTString	strName;
			CTString	strChat;			
			
			(*istr) >> lMakeCharIndex;
			(*istr) >> lChatIndex;
			(*istr) >> lColIndex;
			(*istr) >> strName;
			(*istr) >> strChat;			

			pUIManager->GetMessenger()->RevTalk( lMakeCharIndex, lChatIndex, strName, strChat, lColIndex);
		}


		case MSG_EX_MESSENGER_CHARLIST :
		{
			LONG		lMakeCharIndex;  
			LONG		lChatIndex; 
			LONG		lCount;
			CTString	strCharName;

			(*istr) >> lMakeCharIndex;
			(*istr) >> lChatIndex;
			(*istr) >> lCount;

			for(int i=0; i<lCount; ++i)
			{
				(*istr) >> strCharName;
				pUIManager->GetMessenger()->AddChatMember(lMakeCharIndex, lChatIndex, strCharName);
			}
		}
		break;

		case MSG_EX_MESSENGER_BLOCK :
		{
			BYTE		cError;  
			LONG		lCharIndex; 
			CTString	strCharName;

			(*istr) >> cError;
			(*istr) >> lCharIndex;
			(*istr) >> strCharName;

			pUIManager->GetMessenger()->Block(cError, lCharIndex, strCharName);
		}
		break;

		case MSG_EX_MESSENGER_RELEASE_BLOCK :
		{
			BYTE		cError;  
			LONG		lCharIndex; 
			CTString	strCharName;

			(*istr) >> cError;
			(*istr) >> lCharIndex;
			(*istr) >> strCharName;

			pUIManager->GetMessenger()->UnBlock(cError, lCharIndex, strCharName);			
		}
		break;


		case MSG_EX_MESSENGER_BLOCK_LIST :
		{
			LONG		lCount;
			LONG		lCharIndex;  
			CTString	strCharName;

			(*istr) >> lCount;
			
			
			for(int i=0; i<lCount; ++i)
			{
				(*istr) >> lCharIndex;
				(*istr) >> strCharName;
				pUIManager->GetMessenger()->AddFriendList(lCharIndex, -1, strCharName, (eJob)0, (eCondition)0, true);
			}
		}
		break;

		case MSG_EX_MESSENGER_ONE_VS_ONE: // 서버의 인덱스가 클라이언트에 없는 1:1 채팅
		{
			LONG		lsIndex;
			CTString	strName;
			LONG		lrIndex;
			LONG		lColIndex;
			CTString	strChat;			
			
			(*istr) >> lsIndex;
			(*istr) >> strName;
			(*istr) >> lrIndex;
			(*istr) >> lColIndex;
			(*istr) >> strChat;			

			pUIManager->GetMessenger()->RevOneVsOneTalk( lsIndex, lrIndex, strName, strChat, lColIndex);

		}
		break;

		case MSG_EX_MESSENGER_ONE_OFF_LINE :	// 1:1대화에서 상대가 응답할 수 없는 경우
		{
			LONG		lsIndex; 
			LONG		lrIndex;			

			(*istr) >> lsIndex;
			(*istr) >> lrIndex;

			// 다시돌아오는 에러 메세지이기 때문에 sender와 resiver를 바꿨다
			pUIManager->GetMessenger()->RevOneVsOneTalk( lrIndex, lsIndex, CTString(""),
				_S(2064,"대화상대가 오프라인이여서 메세지를 전달할 수 없습니다."), 7 /*RED*/ );  

		}
		break;
	}
}

// ----------------------------------------------------------------------------
// Name : ReceiveFailMessage()
// Desc : 
// ----------------------------------------------------------------------------
void CSessionState::ReceiveFailMessage(CNetworkMessage *istr )
{
	UBYTE	ubType;
	(*istr) >> ubType;
	
	CTString		strTitle,strMessage;
	CUIMsgBox_Info	MsgBoxInfo;

	CUIManager* pUIManager = CUIManager::getSingleton();

	switch(ubType)
	{

		case MSG_FAIL_WRONG_CHAR :
			pUIManager->CloseMessageBox(MSG_FAIL_WRONG_CHAR);
			strTitle	=	_S(191,"확인");
			strMessage	=_S(883,"길드 이름에 잘못된 문자가 포함되어 있습니다."); 
			MsgBoxInfo.SetMsgBoxInfo(strTitle,UMBS_OK,UI_NONE,MSGCMD_NULL);
			MsgBoxInfo.AddString(strMessage);
			pUIManager->CreateMessageBox(MsgBoxInfo);
		
			break;
	}

}

// EDIT : BS
void CSessionState::MoveOtherServer(ULONG zone, CTString ip, ULONG port)
{
	extern BOOL _bLoginProcess;
	_bLoginProcess					= TRUE;
	//extern BOOL _bUseSocket;
	//_bUseSocket = TRUE;

	// 소켓의 연결을 끊었다가, 다시 연결함.
	_cmiComm.Reconnect(ip, port);

	// EDIT : BS : 070413 : 신규 패킷 암호화
	// EDIT : BS : 패킷 암호화 : 암호화 키 초기화
#ifdef CRYPT_NET_MSG
	CNM_InitKeyValue(&_pNetwork->cnmKey);
#ifndef CRYPT_NET_MSG_MANUAL
	CNM_InitKeyValue(&_pNetwork->cnmKeyServer);
#endif // #ifndef CRYPT_NET_MSG_MANUAL
#else
	_pNetwork->cnmKey = CNM_INIT_KEY;
#endif // #ifdef CRYPT_NET_MSG
	
	// 로그인 서버에 접속 후, 새로운 서버에 접속을 시도합니다.
	CNetworkMessage nmLoginNew((UBYTE)MSG_LOGIN);			// 로그인 메시지 보내기.	
	ULONG version;	 
	version = 700;
	nmLoginNew << version;	    
	nmLoginNew << (unsigned char)MSG_LOGIN_RE;		// 서버 이동
	
	CTString _strUserID;
	CTString _strUserPW;

	_strUserID = _pNetwork->m_strUserID;
	_strUserPW = _pNetwork->m_strUserPW;
	nmLoginNew << _strUserID;
	nmLoginNew << _strUserPW;
	
#ifdef CRYPT_NET_MSG
	// EDIT : BS : 070413 : 신규 패킷 암호화
	// new Version serialization 060710
	// national code 
	INDEX tv_idx;
	switch(g_iCountry)
	{
	case KOREA :
		tv_idx = 0;
		break;
	case TAIWAN : 
		tv_idx = 1;
		break;
	case CHINA : 
		tv_idx = 3;
		break;
	case THAILAND :
		tv_idx = 4;
		break;
	case TAIWAN2 :
		tv_idx = 2;
		break;
	case JAPAN : 
		tv_idx = 6;
		break;
	case MALAYSIA :
		tv_idx = 7;
		break;
	}

	nmLoginNew << (UBYTE)tv_idx;

#ifndef CRYPT_NET_MSG_MANUAL
	ULONG ulSeed = (ULONG)CNM_MakeSeedForClient(_strUserID, _strUserPW, GetTickCount());
	nmLoginNew << ulSeed;
#endif // #ifndef CRYPT_NET_MSG_MANUAL
#endif // #ifdef CRYPT_NET_MSG

	_pNetwork->SendToServerNew(nmLoginNew, TRUE);
	_cmiComm.Server_Update();
	
	// EDIT : BS : 070413 : 신규 패킷 암호화
#ifdef CRYPT_NET_MSG
#ifndef CRYPT_NET_MSG_MANUAL
	CNM_MakeKeyFromSeed(&_pNetwork->cnmKey, (unsigned int)ulSeed);
#endif // #ifndef CRYPT_NET_MSG_MANUAL
#endif // #ifdef CRYPT_NET_MSG

	unsigned int StartTime = (unsigned int(_pTimer->GetLerpedCurrentTick()*1000)); //timeGetTime(); 
//	StartTime = (unsigned int(_pTimer->GetLerpedCurrentTick()*1000)); //timeGetTime(); 

	CUIManager* pUIManager = CUIManager::getSingleton();

	for(;;)
	{		
		_cmiComm.Server_Update();
		CNetworkMessage nmMessage;	  
		if(_pNetwork->ReceiveFromServerNew(nmMessage))
		{
			if (nmMessage.GetType() == MSG_DB)
			{
				unsigned char dbtype;
				nmMessage >> dbtype;
				
				if(dbtype == MSG_DB_CHAR_END)
				{
#ifdef CRYPT_NET_MSG
					// EDIT : BS : 070413 : 신규 패킷 암호화
					//wooss 050820
					ULONG slotTime;
					nmMessage >> slotTime;
					nmMessage >> slotTime;

#ifndef CRYPT_NET_MSG_MANUAL
					ULONG ulSeed;
					nmMessage >> ulSeed;
					CNM_MakeKeyFromSeed(&_pNetwork->cnmKeyServer, (unsigned int)ulSeed);
#endif // #ifndef CRYPT_NET_MSG_MANUAL
#endif // #ifdef CRYPT_NET_MSG
					break;					
				}
			}
		}

		// FIXME : GO_ZONE의 경우 네트워크 관련된 부분을 내부 함수에서 처리하기 때문에,
		// FIXME : 접속 종료 처리를 이 안에서 하는듯...
		// FIXME : 수정해야 함.
		if((unsigned int(_pTimer->GetLerpedCurrentTick()*1000)) - StartTime > 10000)
		{
			// FIXME : 중복되는 코드가 많음.
			pUIManager->CloseMessageBox(MSGCMD_DISCONNECT);
			CUIMsgBox_Info	MsgBoxInfo;
			MsgBoxInfo.SetMsgBoxInfo( _S( 299, "시스템" ), UMBS_OK, UI_NONE, MSGCMD_DISCONNECT );
			MsgBoxInfo.AddString( _S( 300, "서버와의 연결이 끊어졌습니다." ) );
			pUIManager->CreateMessageBox( MsgBoxInfo );
			//cci_bInitUpdateMasterBuffer = FALSE;
			_pNetwork->m_bSendMessage = FALSE;
			_cmiComm.Disconnect();
			break;
		}
	}

	CNetworkMessage nmChaStart((UBYTE)MSG_MENU); 
	nmChaStart << (unsigned char)MSG_MENU_START;
	nmChaStart << _pNetwork->MyCharacterInfo.index;
	_pNetwork->SendToServerNew(nmChaStart,TRUE);	
	
	StartTime = (unsigned int(_pTimer->GetLerpedCurrentTick()*1000)); //timeGetTime(); 

	for(;;)
	{	
		_cmiComm.Server_Update();
		CNetworkMessage nmMessage;	  
		if(_pNetwork->ReceiveFromServerNew(nmMessage))
		{
			if (nmMessage.GetType() == MSG_DB)
			{
				unsigned char dbtype;
				nmMessage >> dbtype;
				
				if(dbtype == MSG_DB_OK)
				{				
					ULONG zone;
					nmMessage >> zone;
					g_slZone = zone;
				}
				
				_bLoginProcess					= FALSE;
				_SetPlayStartTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();

				CEntity* penPlEntity;
				CPlayerEntity* penPlayerEntity;
				penPlEntity = CEntity::GetPlayerEntity(0); //캐릭터 자기 자신
				penPlayerEntity = static_cast<CPlayerEntity*>(penPlEntity);
				
				g_slZone = zone;//0921

				if(CZoneInfo::getSingleton()->GetZoneType(zone) == ZONE_SDUNGEON)
				{
					CPrintF("===Start Single Mode===\n");
					_pNetwork->m_bSingleMode = TRUE;
					_pNetwork->wo_dwEnemyCount		= 0;
					_pNetwork->wo_dwKilledEnemyCount= 0;
					penPlayerEntity->SingleModeOn();
				}
				else
				{
					CPrintF("===End Single Mode===\n");
					_pNetwork->m_bSingleMode = FALSE;
					penPlayerEntity->FieldModeOn();
					pUIManager->GetSingleBattle()->Close();
				}
				///////////
				_pNetwork->m_ubGMLevel				= 0;
				
				_pNetwork->TogglePause();
				_pNetwork->ChangeLevel(CZoneInfo::getSingleton()->GetZoneWldFile(zone), FALSE, 0);
				
				_pNetwork->DeleteAllMob();
				_pNetwork->MyCharacterInfo.EntranceType = CURRENT_ENTER_NORMAL; // 초기화
				
				EPreLevelChange ePreChange;
				ePreChange.iUserData = _pNetwork->ga_iNextLevelUserData;
				SendLevelChangeNotification(ePreChange);
				CEntity::HandleSentEvents();
				_pNetwork->ChangeLevel_internal();
				EPostLevelChange ePostChange;
				ePostChange.iUserData = _pNetwork->ga_iNextLevelUserData;
				SendLevelChangeNotification(ePostChange);
				CEntity::HandleSentEvents();
				_lphCurrent=LCP_NOCHANGE;
				CNetworkMessage nmStartGame((UBYTE)MSG_START_GAME); 					 
				_pNetwork->SendToServerNew(nmStartGame,TRUE);	
				_cmiComm.Server_Update();

				//_bUseSocket = FALSE;
				//////////
				break;
			}
			// FIXME : GO_ZONE의 경우 네트워크 관련된 부분을 내부 함수에서 처리하기 때문에,
			// FIXME : 접속 종료 처리를 이 안에서 하는듯...
			// FIXME : 수정해야 함.
			else if( nmMessage.GetType() == MSG_FAIL)
			{
				// FIXME : 중복되는 코드가 많음.
				pUIManager->CloseMessageBox(MSGCMD_DISCONNECT);
				CUIMsgBox_Info	MsgBoxInfo;
				MsgBoxInfo.SetMsgBoxInfo( _S( 299, "시스템" ), UMBS_OK, UI_NONE, MSGCMD_DISCONNECT );
				MsgBoxInfo.AddString( _S( 300, "서버와의 연결이 끊어졌습니다." ) );
				pUIManager->CreateMessageBox( MsgBoxInfo );
				//cci_bInitUpdateMasterBuffer = FALSE;
				_pNetwork->m_bSendMessage = FALSE;
				_cmiComm.Disconnect();
				break;
			}
		}

		// FIXME : GO_ZONE의 경우 네트워크 관련된 부분을 내부 함수에서 처리하기 때문에,
		// FIXME : 접속 종료 처리를 이 안에서 하는듯...
		// FIXME : 수정해야 함.
		if((unsigned int(_pTimer->GetLerpedCurrentTick()*1000)) - StartTime > 10000)
		{
			// FIXME : 중복되는 코드가 많음.
			pUIManager->CloseMessageBox(MSGCMD_DISCONNECT);
			CUIMsgBox_Info	MsgBoxInfo;
			MsgBoxInfo.SetMsgBoxInfo( _S( 299, "시스템" ), UMBS_OK, UI_NONE, MSGCMD_DISCONNECT );
			MsgBoxInfo.AddString( _S( 300, "서버와의 연결이 끊어졌습니다." ) );
			pUIManager->CreateMessageBox( MsgBoxInfo );
			//cci_bInitUpdateMasterBuffer = FALSE;
			_pNetwork->m_bSendMessage = FALSE;
			_cmiComm.Disconnect();
			break;
		}
	}
}
// --- EDIT : BS

// ----------------------------------------------------------------------------
// Name : ReceiveExHairChange()
// Desc : 산타 모자 관련( 헤어 변경 메세지 처리 )
// ----------------------------------------------------------------------------
void CSessionState::ReceiveExHairChange(CNetworkMessage *istr)
{
	ULONG ulCharIndex;
	UBYTE ubHairStyle;

	(*istr) >> ulCharIndex >> ubHairStyle;

	CEntity *penEntity = NULL;
	CEntity* penPlEntity = NULL;
	CModelInstance *pMI = NULL;
	CPlayerEntity* penPlayerEntity = NULL;
	penPlEntity = CEntity::GetPlayerEntity(0); //캐릭터 자기 자신
	penPlayerEntity = (CPlayerEntity*) penPlEntity; // SetCharacterHairChange() 호출을 위해 사용

	if ( ulCharIndex == _pNetwork->MyCharacterInfo.index ) // 자기 캐릭터
	{
		penPlayerEntity->SetCharacterHairChange(0,ubHairStyle, TRUE);
	}
	else // 타 캐릭터
	{
		penPlayerEntity->SetCharacterHairChange(ulCharIndex, ubHairStyle, FALSE);		
		}
}
// ----------------------------------------------------------------------------
// Name : ReceiveExPlayerStateChange()
// Desc : 게임 서포터 관련( 특정 아이템 착용시 이펙트 적용 )
// ----------------------------------------------------------------------------
void CSessionState::ReceiveExPlayerStateChange(CNetworkMessage *istr)
{
	ULONG ulCharIndex;
	ULONG ulPlayerState;

	(*istr) >> ulCharIndex >> ulPlayerState;

	CEntity *penEntity = NULL;
	CEntity* penPlEntity = NULL;
	CModelInstance *pMI = NULL;
	BOOL bAdd = FALSE;
	CPlayerEntity* penPlayerEntity = NULL;
	penPlEntity = CEntity::GetPlayerEntity(0); //캐릭터 자기 자신
	penPlayerEntity = (CPlayerEntity*) penPlEntity; // PlAddAccessoryEffect() 호출을 위해 사용

	if ( ulPlayerState & PLAYER_STATE_SUPPORTER ) bAdd = TRUE;
	
	if ( ulCharIndex == _pNetwork->MyCharacterInfo.index )
	{
		// 자기 캐릭터
		_pNetwork->MyCharacterInfo.ulPlayerState = ulPlayerState;

		if ( bAdd )
		{ // 적용
			penPlayerEntity->PlAddAccessoryEffect(penPlEntity, NULL);
		}
		else
		{ // 해제
			_pNetwork->MyCharacterInfo.itemEffect.DelAccessoryEffect();
		}
	}
	else
	{
		// 타 캐릭터
		ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_CHARACTER, ulCharIndex);

		if (pObject != NULL)
		{
			CCharacterTarget* pTarget = static_cast< CCharacterTarget* >(pObject);

			penEntity = pTarget->GetEntity();

			if (penEntity == NULL)
				return;

			penPlayerEntity->SetCharacterState(penEntity, ulPlayerState);

			if ( bAdd )
			{ // 적용
				pTarget->cha_state = ulPlayerState;
				penPlayerEntity->PlAddAccessoryEffect(penEntity, pTarget);
			}
			else
			{ // 적용
				pTarget->cha_state = ulPlayerState;
				pTarget->cha_itemEffect.DelAccessoryEffect();
			}
		}
	}
}

// WSS_NPROTECT 070402 ------------------------------->>
#ifndef NO_GAMEGUARD
void CSessionState::ReceiveExnProtect(CNetworkMessage *istr)
{
//	extern ENGINE_API CNPGameLib npgl;
	GG_AUTH_DATA m_AuthData;
	(*istr) >>m_AuthData.dwIndex
		>>m_AuthData.dwValue1
		>>m_AuthData.dwValue2
		>>m_AuthData.dwValue3;
	
	// Debug...
//	CPrintF("Recv Index : %x\n", m_AuthData.dwIndex);
//	CPrintF("Recv Value1 : %x\n", m_AuthData.dwValue1);
//	CPrintF("Recv Value2 : %x\n", m_AuthData.dwValue2);
//	CPrintF("Recv Value3 : %x\n", m_AuthData.dwValue3);
	
//	CPrintF("[ ---->> GameGuard Auth2...Start <<---- ] - %lu\n",timeGetTime());
	//npgl.Auth2(&m_AuthData);
//	CPrintF("[ ---->> GameGuard Auth2...End   <<---- ] - %lu\n",timeGetTime());
	if ( g_pGameGuardRecvAuth )
	{
		g_pGameGuardRecvAuth(m_AuthData);
	}
}
#endif
// ---------------------------------------------------<<
// [KH_070413] 스승의날 이벤트 관련 추가
void CSessionState::ReceiveEventMaster(CNetworkMessage *istr)
{
	INDEX iValue;
	(*istr) >> iValue;

	CUIMsgBox_Info MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo(_S(3344, "스승의 은혜 이벤트"),UMBS_OK,UI_NONE,MSGCMD_NULL);

	if(iValue < 10)
		MsgBoxInfo.AddString(_S(3348, "어려운 모험에 작은 힘이 되도록 25레벨 일반제련석 10개를 드리겠습니다."));
	else
	{
		CTString strMsg;
		strMsg.PrintF(_S(3349, "견습생을 잘 양성하여 명성치가 +%d이 되었습니다. 앞으로도 많은 견습생 양성에 힘써주시기 바랍니다."), iValue);
		MsgBoxInfo.AddString(strMsg);
	}

	CUIManager::getSingleton()->CreateMessageBox(MsgBoxInfo);
}

// [070613: Su-won] 펫 명찰 아이템 사용 결과 메시지 처리
void CSessionState::ReceiveExPetNameChange(CNetworkMessage *istr)
{
	UBYTE lResult;
	(*istr) >> lResult;

	CUIManager* pUIManager = CUIManager::getSingleton();

	switch(lResult)
	{
	case MSG_EX_PET_CHANGE_NAME_ERROR_OK:	// 이름 변경 성공:
		{
			ULONG lPetIndex;
			CTString strPetName;
			ULONG lOwnerIndex=0;

			(*istr) >> lPetIndex;
			(*istr) >> strPetName;

			//펫 정보 업데이트
			CNetworkLibrary::sPetInfo	TempPet;
			TempPet.lIndex				= lPetIndex;
			std::vector<CNetworkLibrary::sPetInfo>::iterator iter = 
			std::find_if(_pNetwork->m_vectorPetList.begin(), _pNetwork->m_vectorPetList.end(), CNetworkLibrary::FindPet(TempPet) );
		
			if( iter != _pNetwork->m_vectorPetList.end() )
			{		
				(*iter).strNameCard		= strPetName;
			}

			ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_PET, lPetIndex);

			if (pObject != NULL)
			{
				CPetTarget* pTarget = static_cast< CPetTarget* >(pObject);

				pTarget->pet_strNameCard =strPetName;
				lOwnerIndex = pTarget->pet_OwnerIndex;
			}

			_pNetwork->UpdatePetTargetInfo( lPetIndex );

			if( lOwnerIndex ==_pNetwork->MyCharacterInfo.index)
			{
				CUIMsgBox_Info	MsgBoxInfo;
				pUIManager->CloseMessageBox(MSGCMD_NULL);
				MsgBoxInfo.SetMsgBoxInfo( CTString(_pNetwork->GetItemName(PET_NAMECARD_ITEM)), UMBS_OK, UI_NONE, MSGCMD_NULL );
				MsgBoxInfo.AddString( _S(3531, "펫의 명찰이 변경되었습니다.") );
				pUIManager->CreateMessageBox(MsgBoxInfo);
			}
		}
		break;
	case MSG_EX_PET_CHANGE_NAME_ERROR_FAIL:	// Pet 이름 변경 실패
		break;
	case MSG_EX_PET_CHANGE_NAME_ERROR_ITEM:	// 아이템이 없음
		break;
	case MSG_EX_PET_CHANGE_NAME_ERROR_NOPET: // Pet 존재 안함
		break;
	case MSG_EX_PET_CHANGE_NAME_ERROR_DUPLICATE:	// Pet 이름 중복
		{
			CUIMsgBox_Info	MsgBoxInfo;
			pUIManager->CloseMessageBox(MSGCMD_NULL);
			MsgBoxInfo.SetMsgBoxInfo( CTString(_pNetwork->GetItemName(PET_NAMECARD_ITEM)), UMBS_OK, UI_NONE, MSGCMD_NULL );
			MsgBoxInfo.AddString( _S(3532, "입력한 이름은 존재하는 이름입니다. 다시 입력해 주세요.") );
			pUIManager->CreateMessageBox(MsgBoxInfo);
		}
		break;
	}
}

// [070820: Su-won] PET_TEX_CHANGE
void CSessionState::ReceiveExPetColorChange(CNetworkMessage *istr)
{
	ULONG lPetIndex;			// 펫 인덱스
	SBYTE sbPetColor;			// 펫 색깔
	ULONG lOwnerIndex;			// 펫 주인 인덱스
	UBYTE sbTypeGrade;			// 펫 타입

	(*istr) >> lPetIndex;
	(*istr) >> sbPetColor;
	(*istr) >> lOwnerIndex;
	(*istr) >> sbTypeGrade;
	
	// [090827: selo] 펫을 장착하고 있지 않다면 무시한다.
	//if( INFO()->_PetTargetInfo.iAge < 0 )
	//	return;

	INDEX iPetType	= -1;
	INDEX iPetAge	= -1;
	_pNetwork->CheckPetType( sbTypeGrade, iPetType, iPetAge );
	const BOOL bPetRide = PetInfo().IsRide(iPetType, iPetAge);

	// 변경할 텍스쳐 파일
	CTString strTexFile =PetInfo().GetColoredTexFileName(iPetType, iPetAge, sbPetColor);

	CEntity* penEntity =NULL;

	//마운트 상태이면...
	if( bPetRide )
	{
		//내 펫이면...
		if( lOwnerIndex == _pNetwork->MyCharacterInfo.index )
		{
			penEntity = CEntity::GetPlayerEntity(0);
		}
		// 내 펫이 아니면...
		else
		{
			ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_CHARACTER, lOwnerIndex);

			if (pObject != NULL)
			{
				penEntity = pObject->GetEntity();
				}
			}
		}
	//마운트 상태가 아니면...
	else
	{
		ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_PET, lPetIndex);

		if (pObject != NULL)
		{
			penEntity = pObject->GetEntity();
		}
	}

	if( penEntity == NULL )
		return;

	// 펫 모델의 텍스쳐를 바꿔줌...
	penEntity->GetModelInstance()->mi_aMeshInst[0].mi_tiTextures[0].ti_toTexture.SetData_t( strTexFile );


	// 이펙트 뿌려줌...
	{
		CTString strEffect;
		//말이면...
		if( iPetType%6<3 )
			strEffect = CTString("pet_dye");
		//용이면...
		else
			strEffect = CTString("pet_dye_1");

		if( bPetRide )
			PCStartEffectGroup( strEffect, lOwnerIndex );
		else
			PetStartEffectGroup( strEffect, lPetIndex );
	}


	// 내 펫의 색깔이 변경되었을 때만 메시지창 띄워줌...
	if( lOwnerIndex == _pNetwork->MyCharacterInfo.index )
	{
		CUIManager* pUIManager = CUIManager::getSingleton();

		CUIMsgBox_Info	MsgBoxInfo;
		pUIManager->CloseMessageBox(MSGCMD_NULL);
//		MsgBoxInfo.SetMsgBoxInfo( CTString(_pNetwork->GetItemName(PET_NAMECARD_ITEM)), UMBS_OK, UI_NONE, MSGCMD_NULL );
		MsgBoxInfo.SetMsgBoxInfo( _S( 1970, "성공" ), UMBS_OK, UI_NONE, MSGCMD_NULL );
		MsgBoxInfo.AddString( _S(3665, "펫 색상변경이 성공적으로 이루어졌습니다.") );
		pUIManager->CreateMessageBox(MsgBoxInfo);
	}
}

//////////////////////////////////////////////////////////////////////////
// [sora]  원정대 관련
//////////////////////////////////////////////////////////////////////////
void CSessionState::ReceiveExpeditionMessage(CNetworkMessage *istr )
{
	// 할거 : 메시지 처리
	SLONG	slType;
	(*istr) >> slType;

	CUIManager* pUIManager = CUIManager::getSingleton();
	Party* pParty = GAMEDATAMGR()->GetPartyInfo();

	if (pParty == NULL)
		return;

	switch(slType)
	{
		case MSG_CREATE_REP: // 원정대 전환
		{
			pParty->RecvChangeExpedition(istr);
		}
			break;
		case MSG_INVITE_REP: // 초대
		{
			pParty->RecvExpeditionInvite(istr);
		}
			break;
		case MSG_REJECT_DEST:
		{
			pParty->ExpeditionReject(FALSE);
		}
			break;
		case MSG_REJECT_SRC:
		{
			pParty->ExpeditionReject(TRUE);
		}
			break;

		case MSG_ENDEXPED_START: // 원정대장이 원정대 해체요청시 멤버들에게 알림
		{

			CUIMsgBox_Info	MsgBoxInfo;
			CTString strMessage;
			int nCommandCode = MSGCMD_EXPEDITION_ERROR;
			
			strMessage = _S(4664, "20초후 원정대가 해체됩니다.");

			// [sora] 인스턴트 존에서는 부활위치로 이동알림
			if(pUIManager->IsPlayInZone())
			{
				strMessage += _S(4659, "인스턴트 존의 부활 위치로 이동합니다.(제한 시간 이후 지동으로 이동합니다.)");
				nCommandCode = MSGCMD_EXPEDITION_GO_STARTPLACE;
			}

			MsgBoxInfo.SetMsgBoxInfo( _S( 4493, "원정대" ), UMBS_OK | UMBS_USE_TIMER,
					UI_PARTY, nCommandCode );
			
			MsgBoxInfo.AddString( strMessage );
			MsgBoxInfo.SetMsgBoxTimer( 20, FALSE );
			pUIManager->CreateMessageBox( MsgBoxInfo );
		}
			break;

		case MSG_ENDEXPED_REP:
		{
			// 원정대 해체 알림 메시지 박스가 있으면 닫는다
			if(pUIManager->DoesMessageBoxExist(MSGCMD_EXPEDITION_ERROR))
				pUIManager->CloseMessageBox(MSGCMD_EXPEDITION_ERROR);

			pUIManager->GetChattingUI()->AddSysMessage( _S( 4665, "원정대가 해체되었습니다." ) );
			pParty->ExpeditionEnd();
		}
			break;

		case MSG_EXPED_ADD:
		{
			// 원정대원 추가
			SLONG nGroupNum, nPos, slIndex, nPosition, iLevel, iHP, iMaxHP, iMP, iMaxMP, slZone;
			SBYTE sbJob, sbJob2, sbLayer;
			FLOAT fX, fZ;
			CTString strName;

			(*istr) >> slIndex;
			(*istr) >> strName;
			(*istr) >> nGroupNum;
			(*istr) >> nPosition;
			(*istr) >> nPos;
			(*istr) >> sbJob; 
			(*istr) >> sbJob2;
			(*istr) >> iLevel;
#ifdef HP_PERCENTAGE //MSG_EXPED_ADD Rep  [3/29/2013 Ranma]
			FLOAT fHp_percentage;
			(*istr) >> fHp_percentage;
			iHP = fHp_percentage * 10;
			iMaxHP = 1000;
#else
			(*istr) >> iHP;
			(*istr) >> iMaxHP;
#endif
			(*istr) >> iMP;
			(*istr) >> iMaxMP;
			(*istr) >> fX;
			(*istr) >> fZ;
			(*istr) >> sbLayer;
			(*istr) >> slZone;

			pParty->AddExpeditionMember( nGroupNum, nPos, nPosition, slIndex, strName, sbJob, sbJob2,
														iLevel, iHP, iMaxHP, iMP, iMaxMP, fX, fZ, sbLayer, slZone );

			// [100216: selo] 지금은 파티중임을 설정한다.
			pParty->SetIsPartyPlay(TRUE);

		}
			break;

		case MSG_QUIT_REP:
		{
			SLONG slIndex, nType;
			(*istr) >> slIndex;
			(*istr) >> nType;
			
			pParty->ExpeditionQuit(nType, slIndex);
		}
			break;
		case MSG_KICK_REP:
		{
			SLONG slIndex;
			(*istr) >> slIndex;
			
			pParty->ExpeditionKick(slIndex);
		}
			break;
		case MSG_CHANGEBOSS_REP:
		{
			SLONG slIndexSrc, slIndexDesc, slChangeMode;
			(*istr) >> slIndexSrc;
			(*istr) >> slIndexDesc;
			(*istr) >> slChangeMode;

			pParty->ChangeExpeditionLeader(slIndexSrc, slIndexDesc, slChangeMode);
		}
			break;
		case MSG_CHANGETYPE_REP:
		{
			SBYTE sbType, sbDivisionType;
			(*istr) >> sbType;
			(*istr) >> sbDivisionType;

			pParty->SetDivisionType(sbDivisionType, sbType);
		}
			break;	
		case MSG_SETMBOSS_REP:
		{
			SLONG slIndex;
			(*istr) >> slIndex;

			pParty->SetExpeditionPosition(slIndex, MSG_EXPED_MEMBERTYPE_MBOSS);
		}
			break;	
		case MSG_RESETMBOSS_REP:
		{
			SLONG slIndex;
			(*istr) >> slIndex;

			pParty->SetExpeditionPosition(slIndex, MSG_EXPED_MEMBERTYPE_NORMAL);
		}
			break;
		case MSG_MOVEGROUP_REP:
		{
			SLONG slGroupSrc, slIndex, slGroupDesc, slPos;
			(*istr) >> slGroupSrc;
			(*istr) >> slIndex;
			(*istr) >> slGroupDesc;
			(*istr) >> slPos;

			pParty->ChangeGroup(slIndex, slGroupDesc, slPos);
		}
			break;
		case MSG_SET_LABEL_REP:
		{
			// 타겟 지정
			SLONG slType, slMode, slLabel, slIndex;
			(*istr) >> slType;
			(*istr) >> slMode;
			(*istr) >> slLabel;
			(*istr) >> slIndex;

			pParty->SetTargetLabel(slType, slMode, slLabel, slIndex);
		}
			break;
		case MSG_VIEWDETAIL_INVEN:
		case MSG_VIEWDETAIL_PET:
		case MSG_VIEWDETAIL_APET:
		{
			// 살펴보기 정보 설절
			pParty->RecvViewDetail(slType, istr);
		}
			break;
		case MSG_EXPED_ERROR:
		{
			pParty->RecvExpeditionError(istr);
		}
			break;		

		case MSG_EXPED_TYPEINFO:
		{
			// 타겟 지정
			SBYTE sbDivisionTypeEXP, sbDivisionTypeITEM, sbDivisionTypeSPITEM;

			(*istr) >> sbDivisionTypeEXP;
			(*istr) >> sbDivisionTypeITEM;
			(*istr) >> sbDivisionTypeSPITEM;

			pParty->SetDivisionType(sbDivisionTypeEXP, sbDivisionTypeITEM, sbDivisionTypeSPITEM);

			// [091119: selo] 지금은 파티중임을 설정한다.
			pParty->SetIsPartyPlay(TRUE);
		}
			break;

		case MSG_GET_QUESTITEM_SUCCESS_REP:	// 원정대 퀘스트 아이텝 검색 성공
		{
			CUIMsgBox_Info	MsgBoxInfo;

			MsgBoxInfo.SetMsgBoxInfo( _S( 4493, "원정대" ), UMBS_OK, UI_NONE, MSGCMD_NULL );
			MsgBoxInfo.AddString( _S(4778, "아이템 수집이 성공적으로 이루어졌습니다.") );
			pUIManager->CreateMessageBox(MsgBoxInfo);
		}
			break;

		case MSG_GET_QUESTITEM_FAILED_REP: // 원정대 퀘스트 아이템 검색 실패
		{
			CUIMsgBox_Info	MsgBoxInfo;

			MsgBoxInfo.SetMsgBoxInfo( _S( 4493, "원정대" ), UMBS_OK, UI_NONE, MSGCMD_NULL );
			MsgBoxInfo.AddString( _S(4777, "아이템 수집에 실패하였습니다.") );
			pUIManager->CreateMessageBox(MsgBoxInfo);
		}
			break;
		case MSG_EXPED_ADD_SYSMSG:
		{
			CTString strMessage;
			CTString strName;
			(*istr) >> strName;
		
			strMessage.PrintF(_S(5498, "%s가 원정대에 합류하였습니다."), strName);
			pUIManager->GetChattingUI()->AddSysMessage(strMessage);
		}
			break;

	}
}
//////////////////////////////////////////////////////////////////////////
void CSessionState::ReceiveFactoryMessage(CNetworkMessage *istr )
{
	UBYTE	ubType;
	(*istr) >> ubType;

	CUIManager* pUIManager = CUIManager::getSingleton();

	switch(ubType)
	{
	case MSG_FACTORY_ITEM_MAKE_REP:
	case MSG_FACTORY_ERROR:
		{
			pUIManager->CloseMessageBox(MSGCMD_NULL);

			UBYTE	ubErrorType;
			CTString strMessage;
			CUIMsgBox_Info	MsgBoxInfo;

			(*istr) >> ubErrorType;

			switch(ubErrorType)
			{
			case MSG_FACTORY_ERROR_MAKE_OK:
				{
					pUIManager->GetProduct2()->SetItemMaking(TRUE);
					return;
				}break;
			case MSG_FACTORY_ERROR_SYSTEM:
				{
					strMessage.PrintF(_S(1688, "조건이 맞지 않습니다."));
				}break;
			case MSG_FACTORY_ERROR_ALREADY_REG:
				{
					strMessage.PrintF(_S(2701, "이미 리스트에 등록된 상태입니다"));
				}break;
			case MSG_FACTORY_ERROR_SKILL:
				{
					strMessage.PrintF(_S(578, "제작에 필요한 제작 스킬이 없습니다."));
				}break;
			case MSG_FACTORY_ERROR_NAS:
				{
					strMessage.PrintF(_S(306, "나스가 부족합니다."));
				}break;
			case MSG_FACTORY_ERROR_EXP:
				{
					strMessage.PrintF(_S(4667, "숙련도가 부족합니다."));
				}break;
			case MSG_FACTORY_ERROR_ITEM:
				{
					strMessage.PrintF(_S(1799, "재료가 부족합니다. 확인하고 다시 시도하여 주십시오."));
				}break;
			case MSG_FACTORY_ERROR_INVEN:
				{
					strMessage.PrintF(_S(265, "인벤토리 공간이 부족합니다."));
				}break;
			}

			MsgBoxInfo.SetMsgBoxInfo( _S( 191, "확인" ), UMBS_OK, UI_NONE, MSGCMD_NULL );		
			MsgBoxInfo.AddString( strMessage );
			pUIManager->CreateMessageBox( MsgBoxInfo );

		}break;
	case MSG_FACTORY_LEARN:
		{
			SLONG nfactoryIndex;
			CTString strTem;

			(*istr) >> nfactoryIndex;

			CMakeItemData temMakeItem = _pNetwork->GetMakeItemData(nfactoryIndex);
			strTem.PrintF(_S(4668, "%s 제작 기술을 습득하였습니다."), temMakeItem.GetName());
			_pNetwork->ClientSystemMessage(strTem, SYSMSG_NOTIFY);
			pUIManager->GetProductNPC()->CloseProductNPC();

		}break;
	case MSG_FACTORY_ITEM_LIST:
		{
			UBYTE UItype;
			SLONG nfactoryIndex,temSealtype,ncount;

			(*istr) >> (UBYTE)UItype;		// 0: NPC에서 요청 1: 만들기에서 요청
			(*istr) >> temSealtype;
			(*istr) >> ncount;

			pUIManager->GetProduct2()->SetSealType(temSealtype);

			pUIManager->GetProduct2()->ListReset();

			for (int i = 0; i < ncount; i++)
			{
				(*istr) >> nfactoryIndex;

				pUIManager->GetProduct2()->AddMakeItemList(nfactoryIndex);
			}

			// 0: NPC에서 요청 1: 만들기에서 요청
			if (UItype)
			{
				pUIManager->GetProduct2()->OpenProduct2(temSealtype);
			}else
			{
				pUIManager->GetProductNPC()->OpenProductList(temSealtype);
			}
			

		}break;
// 	case MSG_FACTORY_ITEM_MAKE_REP:
// 		{
// 			ULONG lindex;
// 			UBYTE ubResult;
// 
// 			(*istr) >> lindex;
// 			(*istr) >> ubResult;
// 
// 			if (ubResult)
// 			{
// 				pUIManager->GetProduct2()->CloseProduct2();
// 				pUIManager->GetProduct2()->SendMakeItem(MSG_FACTORY_ITEM_MAKE,lindex);
// 			}
// 			
// 
//		}break;
	case MSG_FACTORY_ITEM_MAKE:
		{
			SLONG nItemIndex,nSealIndex;
			SQUAD lSealExp;

			CTString strMessage;
			CUIMsgBox_Info	MsgBoxInfo;

			(*istr) >> nItemIndex;
			(*istr) >> nSealIndex;
			(*istr) >> lSealExp;

			pUIManager->CloseMessageBox( MSGCMD_NULL );	// 이미 열려 있다면 닫는다.

			CMakeItemData& Itemdata = _pNetwork->GetMakeItemData(nItemIndex);
			strMessage.PrintF("%s ",Itemdata.GetName());
			strMessage += _S(580, "제작에 성공 하였습니다.");

			MsgBoxInfo.SetMsgBoxInfo( _S( 191, "확인" ), UMBS_OK, UI_NONE, MSGCMD_NULL );		
			MsgBoxInfo.AddString( strMessage );
			pUIManager->CreateMessageBox( MsgBoxInfo );

			pUIManager->GetCharacterInfo()->UpdateSealExp(nSealIndex,lSealExp);

		}break;

	}

}

// connie [2009/9/8] - NPC 찾기
void CSessionState::ReceiveNPCPortalMessage(CNetworkMessage* istr)
{
	SLONG	slType;

	(*istr) >> slType;
	// [eons]
	// 메세지 타입이 불분명 하다.
	// 기능상 리스트를 요청하고, 응답후 사용하게 되어 있으므로, 요청과 응답의 완료 이후 서브정보로 메세지를 받는 구조로 두는 것이 좋다.
	// 현재 클라이언트는 MSG_NPC_PORTAL_LIST를 직접적으로 사용하지 않는다. 요청과 응답의 결과를 바로 알 수 없다.
	// 현재 ReceiveNPCData() 내부에서 오픈 함수를 작동하게 하였다.
	// Error시에는 창을 띄우지 않는다.

	CUIManager* pUIManager = CUIManager::getSingleton();

	switch(slType)
	{
		case MSG_NPC_PORTAL_LIST:
			{
				pUIManager->GetNpcScroll()->ReceiveNPCData(istr);
			}
			break;
		case MSG_NPC_PORTAL_LOCATION:
			{
				pUIManager->GetNpcScroll()->ReceiveMobLocation(istr);
			}
			break;
		case MSG_NPC_PORTAL_ERROR:
			{
				SLONG slErrorType;
	
				(*istr) >> slErrorType;
				// OpenNPCScrollInfo에서 ErrorType그대로 출력한다. [eons]
				pUIManager->GetNpcScroll()->OpenNPCScrollInfo(slErrorType);
			}
			break;
	}

}

void CSessionState::RecievePromotionEventMessage(CNetworkMessage* istr)
{
	UBYTE ubType;
	(*istr) >> ubType;	

	CUIMsgBox_Info	MsgBoxInfo;
	CTString strMessage;
	MsgBoxInfo.SetMsgBoxInfo( _S( 191, "확인" ), UMBS_OK, UI_NONE, MSGCMD_NULL );		

	switch( ubType )
	{
	case MSG_EVENT_PROMOTION2_REP_ERROR_FAIL:
		{	// 실패 ( 지금은 쿠폰 발급 시스템을 사용할 수 없습니다. )
			strMessage = _S( 5106, "지금은 쿠폰 발급 시스템을 사용할 수 없습니다." );
		}
		break;
	case MSG_EVENT_PROMOTION2_REP_ERROR_SUC:
		{	// 쿠폰 사용 가능 (바로 상품 지급 )
			strMessage = _S( 2224, "이벤트 상품이 지급되었습니다." );
		}
		break;
	case MSG_EVENT_PROMOTION2_REP_ERROR_KEY:
		{	// 사용할 수 없는 쿠폰 (이미 사용한 쿠폰, 
			strMessage = _S( 2405, "요청한 쿠폰 번호가 존재하지 않습니다." );
		}
		break;
	case MSG_EVENT_PROMOTION2_REP_ERROR_USED:
		{	// 이미 사용한 쿠폰
			strMessage = _S( 2406, "이미 지급받은 쿠폰 번호 입니다." );
		}
		break;
	case MSG_EVENT_PROMOTION2_REP_ERROR_USER:
		{	// 쿠폰을 사용할 수 없는 유저가 사용
			strMessage = _S( 1722, "보상을 받으실 수 있는 조건이 아닙니다" );
		}
		break;
	case MSG_EVENT_PROMOTION2_REP_ERROR_DATE:
		{	// 사용 기간이 지났음
			strMessage = _S( 5107, "사용 기간이 만료된 쿠폰입니다." );
		}
		break;
	}

	if ( strMessage.Length() > 0 )
	{
		MsgBoxInfo.AddString( strMessage );
		CUIManager::getSingleton()->CreateMessageBox( MsgBoxInfo );
	}
}

#ifdef KALYDO
void CSessionState::DelayLoadingFromKalydo()
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	CEntity* penPlEntity;
	CPlayerEntity* penPlayerEntity;
	penPlEntity = CEntity::GetPlayerEntity(0); //캐릭터 자기 자신
	penPlayerEntity = static_cast<CPlayerEntity*>(penPlEntity);

	_pNetwork->m_ubGMLevel = 0;
	_pNetwork->TogglePause();
	_pNetwork->ChangeLevel(CZoneInfo::getSingleton()->GetZoneWldFile(g_slZone), FALSE, 0);
	
	// [090826: selo] 시스템 메시지 저장 끝내기
	pUIManager->GetChattingUI()->End_SaveSysMessage(1);

	// Date : 2006-04-26(오전 11:48:00), By eons
	if( penPlayerEntity->IsPolymophing() )	// 변신 중이라면
	{
		penPlayerEntity->ReturnChange( TRUE );
		const int iStopChangeItem = 521;
	}

	pUIManager->GetSummonFirst()->ResetSummon();
	pUIManager->GetSummonSecond()->ResetSummon();

	pUIManager->GetPersonalShop()->ResetShop();
	pUIManager->SetCSFlagOff( CSF_PERSONALSHOP );
	pUIManager->SetCSFlagOff( CSF_MOONSTONE );
	pUIManager->SetCSFlagOff( CSF_MOUNT_HUNGRY );
	pUIManager->GetGuild()->ResetGuild();
	pUIManager->GetGuild()->ClearMemberList();
	_pNetwork->LeavePet( static_cast<CPlayerEntity*>(CEntity::GetPlayerEntity(0)) );
	INFO()->GetMyPetInfo()->Init();	// [091119: selo] 펫 정보 초기화
	// FIXED : wild pet mount bug [12/12/2010 rumist]
	// 초기화 실패로 매시가 깨지는 것 방지.
	static_cast<CPlayerEntity*>(CEntity::GetPlayerEntity(0))->LeavingWildPet( static_cast<CPlayerEntity*>(CEntity::GetPlayerEntity(0)) );
	MY_APET_INFO()->Init();
	// royal rumble init [5/11/2011 rumist]
	pUIManager->GetRadar()->ResetRoyalRumbleStat();
	pUIManager->GetRoyalRumbleIcon()->ShowRoyalRumbleIcon( FALSE );
	// ITS#3867 : 채집 중 이동시 애니메이션 리셋. [2/13/2012 rumist]
	static_cast<CPlayerEntity*>(CEntity::GetPlayerEntity(0))->CancelProduct();

	// [091119: selo] 파티정보 초기화
	pParty->Init();
	pParty->InitExpedition();

//		Bug Fix 060530 wooss
//		_pNetwork->ClearPetList();
//		INFO()->_PetTargetInfo.Init();
	INFO()->m_MySlaveInfo[0].Init();
	INFO()->m_MySlaveInfo[1].Init();
	
	pUIManager->GetHelper()->ResetHelper();
	pUIManager->GetHelper()->ClearHelperList();
	
	penPlayerEntity->ClearMultiTargets();

	_pNetwork->DeleteAllMob();
	_pNetwork->MyCharacterInfo.EntranceType = CURRENT_ENTER_NORMAL; // 초기화

	EPreLevelChange ePreChange;
	ePreChange.iUserData = _pNetwork->ga_iNextLevelUserData;
	SendLevelChangeNotification(ePreChange);
	CEntity::HandleSentEvents();
	_pNetwork->ChangeLevel_internal();
	EPostLevelChange ePostChange;
	ePostChange.iUserData = _pNetwork->ga_iNextLevelUserData;
	SendLevelChangeNotification(ePostChange);
	CEntity::HandleSentEvents();
	_lphCurrent=LCP_NOCHANGE;
	CNetworkMessage nmStartGame(MSG_START_GAME); 					 
	_pNetwork->SendToServerNew(nmStartGame,TRUE);	
	_cmiComm.Server_Update();
//강동민 수정 시작 싱글던젼 스토리	10.07
	// TargetNPC는 대부분 미리 생성되야할 엔티티들임.
	for(std::vector<CEntity*>::iterator iter = _pNetwork->ga_World.m_vectorPreCreateNPC.begin(); iter != _pNetwork->ga_World.m_vectorPreCreateNPC.end(); ++iter)
	{
		(*iter)->Reinitialize();
	}
	_pNetwork->ga_World.m_vectorPreCreateNPC.clear();
	_cmiComm.Server_Update();

	/*
	if(_pNetwork && _pNetwork->m_bSingleMode && _pNetwork->ga_World.wo_iNumOfNPC)
	{
		CEntity* penPlayer = CEntity::GetPlayerEntity(0);
		_pNetwork->ga_World.m_vectorTargetNPC.push_back(penPlayer);
	}
	*/
}


#endif
void CSessionState::ReceiveMoneyMessage(CNetworkMessage *istr)
{
	OBFUSCATE();
	
	SBYTE	type;	//서브 타입
	SQUAD	count;	//총량
	SQUAD	delta;	//변화량

	CUIManager* pUIManager = CUIManager::getSingleton();
	CTString	strSysMessage;

	(*istr) >> type;
	switch(type)
	{
	case MSG_MONEY_COUNT:
		{
			(*istr) >> count;
			(*istr) >> delta;

			if(count < 0 )	//나스가 음수 값이 들어오면 
			{
				ASSERT(FALSE);
				break;
			}

			bool bChange = false;
			if (_pNetwork->MyCharacterInfo.money != count)
			{
				// 나스가 변화 되었다면
				bChange = true;			
			}

			_pNetwork->MyCharacterInfo.money = count;

			if(delta > 0)	//나스 증가
			{
				CTString strCount;
				strCount.PrintF( "%I64d", delta );
				pUIManager->InsertCommaToString( strCount );
			
				strSysMessage.PrintF( _S( 416, "%s 나스를 얻었습니다." ), strCount ); // 번역 수정
				_pNetwork->ClientSystemMessage( strSysMessage );	
			}
			else if(delta == 0)	//나스 변화 없음
			{
			
			}
			else	//나스 감소 
			{
			
			}

			if (bChange == true)
				GameDataManager::getSingleton()->GetAuction()->updateNas();
		}
		break;
	default: 
		ASSERT(FALSE);
		break;

	}
}

void CSessionState::ReceiveExpressMessage(CNetworkMessage* istr)
{
	GameDataManager* pGameDataManager = GameDataManager::getSingleton();

	pGameDataManager->GetExpressData()->RecvExpressMessage(&(*istr));
}

void CSessionState::ReceiveCalendarMessage( CNetworkMessage* istr )
{
	GameDataManager* pGameDataManager = GameDataManager::getSingleton();

	pGameDataManager->GetCalendarData()->RecvCalendarMessage(&(*istr));
}

void CSessionState::ReceiveTimerItemMessage(CNetworkMessage* istr)
{
	_pUIBuff->RecvTimerItemBuff(&(*istr));
}

#ifdef HP_PERCENTAGE
void CSessionState::ReceiveDamageRealStatMessage(CNetworkMessage *istr)
{
	SBYTE	type;	//서브 타입
	LONG	index;
	LONG	curHp;
	LONG	MaxHp;
	LONG	curMp;
	LONG	MaxMp;

	CUIManager* pUIManager = CUIManager::getSingleton();

	(*istr) >> type;
	(*istr) >> index;
	(*istr) >> curHp;
	(*istr) >> MaxHp;
	(*istr) >> curMp;
	(*istr) >> MaxMp;

	switch(type)
	{
	case MSG_CHAR_PC:
		{
			_pNetwork->MyCharacterInfo.hp		= curHp;
			_pNetwork->MyCharacterInfo.maxHP	= MaxHp;
			_pNetwork->MyCharacterInfo.mp		= curMp;
			_pNetwork->MyCharacterInfo.maxMP	= MaxMp;
		}
		break;

	case MSG_CHAR_PET:
	case MSG_CHAR_WILDPET:
		{
			CNetworkLibrary::sPetInfo	TempPet;
			TempPet.lIndex				= index;
			
			std::vector<CNetworkLibrary::sPetInfo>::iterator iter = 
				std::find_if(_pNetwork->m_vectorPetList.begin(), _pNetwork->m_vectorPetList.end(), CNetworkLibrary::FindPet(TempPet) );
			
			if( iter != _pNetwork->m_vectorPetList.end() )
			{
				(*iter).lHP = curHp;
				(*iter).lMaxHP = MaxHp;
			
				_pNetwork->UpdatePetTargetInfo( index );
			}			
		}
		break;

	case MSG_CHAR_ELEMENTAL:
		{
			ObjectBase* pObject = ACTORMGR()->GetObject(index);

			if (pObject != NULL && pObject->m_eType == eOBJ_SLAVE)
			{
				if( st.m_pEntity )
				{
					((CPlayerEntity*)CEntity::GetPlayerEntity(0))->SetElementalData(pObject->GetEntity(), curHp, MaxHp);											
				}
			}
		}
		break;
	default:
		break;
	}

}

#endif // HP_PERCENTAGE

void CSessionState::DeleteObject( SBYTE type, ULONG index )
{
	ACTORMGR()->RemoveObject((eOBJ_TYPE)type, index);
}

void CSessionState::ReceivePetStashMessage( CNetworkMessage* istr )
{
	GameDataManager* pGameDataManager = GameDataManager::getSingleton();
	pGameDataManager->GetStashData()->RecvPetStashMessage(&(*istr));
}

void CSessionState::ReceiveRVRMessage( CNetworkMessage* istr )
{
	GameDataManager* pGameDataManager = GameDataManager::getSingleton();
	pGameDataManager->GetSyndicate()->RecvSyndicateMsg(&(*istr));
}

void CSessionState::ReceiveDurabilityMessage( CNetworkMessage* istr )
{
	CDurability* pData = GameDataManager::getSingleton()->GetDurability();

	if (pData != NULL)
		pData->RecvDurabilityMsg(&(*istr));
}

void CSessionState::ReceiveToggleMessage( CNetworkMessage* istr )
{
	ResponseClient::ToggleMsg* pRecv = reinterpret_cast<ResponseClient::ToggleMsg*>(istr->GetBuffer());

	// 스킬.
	if (pRecv->toggle_type == 0)
	{
		CSkill* pSkill = &_pNetwork->GetSkillData(pRecv->index);
		
		if (pSkill != NULL)
		{
			pSkill->SetToggle(pRecv->toggle);
			UIMGR()->GetSkillNew()->SetToggle(pRecv->index);
		}
	}
	else //if(pRecv->toggle_type == 1) 아이템
	{
		CUIIcon* pIcon = UIMGR()->GetInventory()->GetItemIcon(pRecv->index);

		if (pIcon != NULL)
		{
			CItems* pItems = pIcon->getItems();

			if (pItems != NULL)
				pItems->SetToggle(pRecv->toggle);

			pIcon->SetToggle(pRecv->toggle);
		}

		if (pRecv->toggle == true)
		{
			CEntity			*penPlEntity;
			CPlayerEntity	*penPlayerEntity;

			penPlEntity = CEntity::GetPlayerEntity( 0 );
			penPlayerEntity = (CPlayerEntity *)penPlEntity;

			penPlayerEntity->PlayHolyWaterSound();
		}
	}

	UIMGR()->GetQuickSlot()->SetToggle(pRecv->toggle_type, pRecv->index, pRecv->toggle);
}

void CSessionState::ReceiveItemCollectionMessage( CNetworkMessage* istr )
{
	CItemCollection* pData = GameDataManager::getSingleton()->GetItemCollection();

	if (pData != NULL)
		pData->RecvItemCollectionMsg(&(*istr));
}

void CSessionState::ReceivePremiumCharMessage( CNetworkMessage* istr )
{
#ifdef PREMIUM_CHAR
	CPremiumChar* pData = GameDataManager::getSingleton()->GetPremiumChar();

	if (pData != NULL)
		pData->RecvPremiumCharMsg(&(*istr));
#endif	//	PREMIUM_CHAR
}

void CSessionState::ReceiveItemComposMessage( CNetworkMessage* istr )
{
	CItemCompose* pData = GAMEDATAMGR()->GetItemCompose();

	if (pData != NULL)
		pData->RecvItemComposeMsg(istr);
}

