
#include "StdH.h"
#include <Engine/Network/CNetwork.h>
#include <Engine/Network/Server.h>
#include <Engine/Interface/UIInternalClasses.h>
#include "SessionState.h"
#include <Engine/Ska/Render.h>
#include <Engine/SlaveInfo.h>
#include <Engine/Entities/InternalClasses.h>
#include <Engine/Interface/UIManager.h>
#include <Engine/Interface/UIAutoHelp.h>
#include <Engine/Interface/UIPlayerInfo.h>
#include <Engine/Contents/Base/UINoticeNew.h>
#include <Engine/Interface/UISiegeWarfareDoc.h>
#include <Engine/Interface/UISiegeWarfareNew.h>
#include <Engine/Interface/UIInventory.h>
#include <Engine/Interface/UINpcScroll.h>
#include <Engine/Interface/UIPersonalShop.h>
#include <Engine/Interface/UIGuild.h>
#include <Engine/Interface/UIGuildBattle.h>
#include <Engine/Interface/UIMessenger.h>
#include <Engine/Interface/UIMap.h>
#include <Engine/Interface/UIFlowerTree.h>
#include <Engine/Contents/Base/UIPartyNew.h>
#include <Engine/Interface/UIPetInfo.h>
#include <Engine/Contents/function/PremiumChar.h>
#include <Engine/Contents/function/PremiumCharUI.h>
#include <Engine/Interface/UISummon.h>
#include <Engine/Interface/UIWildPetInfo.h>
#include <Engine/Contents/Base/UISkillNew.h>
#include <Engine/Contents/Base/UICharacterInfoNew.h>
#include <Engine/Contents/Base/ExpressSystem.h>
#include <Engine/Entities/InternalClasses.h>
#include <Engine/Templates/Stock_CEntityClass.h>
#include <Engine/Templates/DynamicContainer.cpp>
#include <Engine/GameState.h>
#include <Engine/Object/ActorMgr.h>
#include <Engine/GameDataManager/GameDataManager.h>
#include <Engine/GameStageManager/StageMgr.h>

#include <Common/Packet/ptype_char_status.h>
#include <Common/Packet/ptype_appear.h>
#include <Common/Packet/ptype_premium_char.h>
#include <Engine/Interface/UIGuildWarPortal.h>
#include <Engine/Contents/Base/Quest.h>
#include <Engine/Contents/Base/Party.h>
#include <Engine/Contents/function/SimplePlayerInfoUI.h>
#include <Engine/Contents/function/TargetInfoNewUI.h>
#include <Engine/Contents/function/News.h>
#include <Engine/Contents/function/ItemCompose.h>

#ifdef XTRAP_SECURE_CKBANG_2010_07_20
	#include <Engine/XTrapInterface/XTrapInterface.h>
#endif
#include <Engine/Info/MyInfo.h>

// 속성 시스템 속성 갯수 디파인
#define ATTRIBUTE_ATT_MAX 7
#define ATTRIBUTE_DEF_MAX 7

// 속성 시스템 계산 공식
#define AT_MASK          0xFL
#define AT_LVVEC          4
#define GET_AT_VAR(m)   ((m) & AT_MASK)
#define GET_AT_LV(m)     (((m) >> AT_LVVEC) & AT_MASK)

#define IS_FLAG_ON(flagAll, flagVal) ( (flagAll) & (static_cast<SQUAD>((1L))<<(flagVal)) )

DECLARE_MSG_UPDATE(StatusAPetSmall);
DECLARE_MSG_UPDATE(UpdateLevel);
DECLARE_MSG_UPDATE(UpdateTargetHPInfo);
DECLARE_MSG_UPDATE(UpdateCharHitType);
DECLARE_MSG_UPDATE(updateDisappearEffect);
DECLARE_MSG_UPDATE(updateDisappearAll);
DECLARE_MSG_UPDATE(ClearTarget);
DECLARE_MSG_UPDATE(updatePremiumCharInfo);
DECLARE_MSG_UPDATE(updatePremiumJumpCount);
DECLARE_MSG_UPDATE(updatePremiumCharEnd);
DECLARE_MSG_UPDATE(PremiumFlag);
DECLARE_MSG_UPDATE(updateAttrPc);
DECLARE_MSG_UPDATE(updateAttrNpc);
DECLARE_MSG_UPDATE(updateNewsGmNoti);
DECLARE_MSG_UPDATE(updateNewsWebNoti);
DECLARE_MSG_UPDATE(updateNewsWebClose);
DECLARE_MSG_UPDATE(updateNewsWebClear);
DECLARE_MSG_UPDATE(updateItemCompose);
DECLARE_MSG_UPDATE(updateRelicGpsSysMsg);
DECLARE_MSG_UPDATE(updateGuildRecall);

void CSessionState::reg_packet_info()
{
	REG_PACKET_R(MSG_UPDATE_DATA_FOR_CLIENT, MSG_SUB_UPDATE_STATUS, updateStatus);			// 캐릭터 정보(모두)		
	REG_PACKET_R(MSG_UPDATE_DATA_FOR_CLIENT, MSG_SUB_UPDATE_AT, updateAt);
	REG_PACKET_R(MSG_UPDATE_DATA_FOR_CLIENT, MSG_SUB_UPDATE_APPEAR_PC, updatePcAppear);
	REG_PACKET_R(MSG_UPDATE_DATA_FOR_CLIENT, MSG_SUB_UPDATE_APPEAR_NPC, updateNpcAppear);
	REG_PACKET_R(MSG_UPDATE_DATA_FOR_CLIENT, MSG_SUB_UPDATE_APPEAR_PET, updatePetAppear);
	REG_PACKET_R(MSG_UPDATE_DATA_FOR_CLIENT, MSG_SUB_UPDATE_APPEAR_ELEMENTAL, updateElementalAppear);
	REG_PACKET_R(MSG_UPDATE_DATA_FOR_CLIENT, MSG_SUB_UPDATE_APPEAR_APET, updateAPetAppear);
	REG_PACKET_R(MSG_UPDATE_DATA_FOR_CLIENT, MSG_SUB_UPDATE_DISAPPERAR, updateDisappear);
	REG_PACKET_UPDATE(MSG_UPDATE_DATA_FOR_CLIENT, MSG_SUB_UPDATE_DISAPPERAR_FOR_CLIENT_EFFECT, updateDisappearEffect);
	REG_PACKET_UPDATE(MSG_UPDATE_DATA_FOR_CLIENT, MSG_SUB_UPDATE_DISAPPERAR_ALL, updateDisappearAll);
	REG_PACKET_UPDATE(MSG_UPDATE_DATA_FOR_CLIENT, MSG_SUB_UPDATE_TARGET_CLEAR, ClearTarget);
	REG_PACKET_R(MSG_UPDATE_DATA_FOR_CLIENT, MSG_SUB_UPDATE_ELEMENTAL_STATUS, updateElementalStatus);
	REG_PACKET_R(MSG_UPDATE_DATA_FOR_CLIENT, MSG_SUB_UPDATE_EXPED_INFO, updateExpedMemberInfo);
	REG_PACKET_R(MSG_UPDATE_DATA_FOR_CLIENT, MSG_SUB_UPDATE_PARTY_INFO, updatePartyMemberInfo);
	REG_PACKET_R(MSG_UPDATE_DATA_FOR_CLIENT, MSG_SUB_UPDATE_CHAR_STATUS_PC, updateStatusPC);
	REG_PACKET_R(MSG_UPDATE_DATA_FOR_CLIENT, MSG_SUB_UPDATE_CHAR_STATUS_NPC, updateStatusNPC);
	REG_PACKET_R(MSG_UPDATE_DATA_FOR_CLIENT, MSG_SUB_UPDATE_CHAR_STATUS_PET, updateStatusPet);
	REG_PACKET_R(MSG_UPDATE_DATA_FOR_CLIENT, MSG_SUB_UPDATE_CHAR_STATUS_ELEMENTAL, updateStatusElemental);
	REG_PACKET_R(MSG_UPDATE_DATA_FOR_CLIENT, MSG_SUB_UPDATE_CHAR_STATUS_APET, updateStatusAPet);
	REG_PACKET_R(MSG_UPDATE_DATA_FOR_CLIENT, MSG_SUB_UPDATE_DAMAGE, updateDamage);
	REG_PACKET_UPDATE(MSG_UPDATE_DATA_FOR_CLIENT, MSG_SUB_UPDATE_APET_SMALL_STATUS, StatusAPetSmall);
	REG_PACKET_UPDATE(MSG_UPDATE_DATA_FOR_CLIENT, MSG_SUB_UPDATE_LEVELUP_INFO, UpdateLevel);
	REG_PACKET_UPDATE(MSG_UPDATE_DATA_FOR_CLIENT, MSG_SUB_UPDATE_TARGET_HP_INFO, UpdateTargetHPInfo);
	REG_PACKET_UPDATE(MSG_UPDATE_DATA_FOR_CLIENT, MSG_SUB_UPDATE_HOLYWATER_STATE, UpdateCharHitType);
#ifdef PREMIUM_CHAR
	REG_PACKET_UPDATE(MSG_UPDATE_DATA_FOR_CLIENT, MSG_SUB_UPDATE_PREMIUM_CHAR_INFO, updatePremiumCharInfo);
	REG_PACKET_UPDATE(MSG_UPDATE_DATA_FOR_CLIENT, MSG_SUB_UPDATE_PREMIUM_CHAR_JUMP_COUNT, updatePremiumJumpCount);
	REG_PACKET_UPDATE(MSG_UPDATE_DATA_FOR_CLIENT, MSG_SUB_UPDATE_PREMIUM_CHAR_END, updatePremiumCharEnd);
	REG_PACKET_UPDATE(MSG_UPDATE_DATA_FOR_CLIENT, MSG_SUB_UPDATE_PREMIUM_CHAR_CHANGE_FLAG, PremiumFlag);
#endif	//	PREMIUM_CHAR
	REG_PACKET_UPDATE(MSG_UPDATE_DATA_FOR_CLIENT, MSG_SUB_UPDATE_ATTR_ITEM_USE, updateAttrPc);
	REG_PACKET_UPDATE(MSG_UPDATE_DATA_FOR_CLIENT, MSG_SUB_UPDATE_ATTR_STATE_FOR_NPC, updateAttrNpc);
	REG_PACKET_UPDATE(MSG_UPDATE_DATA_FOR_CLIENT, MSG_SUB_UPDATE_CHECK_NOTICE, updateNewsGmNoti);
	REG_PACKET_UPDATE(MSG_UPDATE_DATA_FOR_CLIENT, MSG_SUB_UPDATE_CHECK_POPUP_NOTICE, updateNewsWebNoti);
	REG_PACKET_UPDATE(MSG_UPDATE_DATA_FOR_CLIENT, MSG_SUB_UPDATE_CLOSE_POPUP_NOTICE, updateNewsWebClose);
	REG_PACKET_UPDATE(MSG_UPDATE_DATA_FOR_CLIENT, MSG_SUB_UPDATE_CLEAR_POPUP_NOTICE, updateNewsWebClear);

	REG_PACKET_UPDATE(MSG_UPDATE_DATA_FOR_CLIENT, MSG_SUB_UPDATE_ARTIFACT_SYSMSG, updateItemCompose);
	REG_PACKET_UPDATE(MSG_UPDATE_DATA_FOR_CLIENT, MSG_SUB_UPDATE_ARTIFACT_FIND_USER_COUNT_MSG, updateRelicGpsSysMsg);	
	REG_PACKET_UPDATE(MSG_UPDATE_DATA_FOR_CLIENT, MSG_SUB_REQUEST_GUILD_RECALL, updateGuildRecall);
}


void CSessionState::updateStatus( CNetworkMessage* istr )
{
	UpdateClient::charStatus* pPack = reinterpret_cast<UpdateClient::charStatus*>(istr->GetBuffer());

	// edit by cpp2angel (044.12.20) : 자동 도움말
	if ( _pNetwork->MyCharacterInfo.sp < pPack->skillPoint )
	{
		_UIAutoHelp->SetInfo ( AU_GET_SKILL_POINT );
	}

	if (_pNetwork->MyCharacterInfo.sp != pPack->skillPoint)
		CUIManager::getSingleton()->GetSkillNew()->UpdateSPointAndMoney(pPack->skillPoint);

	_pNetwork->MyCharacterInfo.level = pPack->level;
	static int iOldLevel = -1;
	if(iOldLevel != pPack->level)
	{
		ACTORMGR()->RefreshQuestMark();		
		iOldLevel = pPack->level;
	}
	_pNetwork->MyCharacterInfo.curExp = pPack->exp; 
	_pNetwork->MyCharacterInfo.needExp = pPack->levelupExp; 
	_pNetwork->MyCharacterInfo.hp = pPack->hp; 
	_pNetwork->MyCharacterInfo.maxHP = pPack->maxHp;
	_pNetwork->MyCharacterInfo.mp = pPack->mp; 
	_pNetwork->MyCharacterInfo.maxMP = pPack->maxMp; 
	_pNetwork->MyCharacterInfo.str = pPack->str; 
	_pNetwork->MyCharacterInfo.dex = pPack->dex;
	_pNetwork->MyCharacterInfo.intel = pPack->intl; 
	_pNetwork->MyCharacterInfo.con = pPack->con; 
	_pNetwork->MyCharacterInfo.opt_str = pPack->opStr + pPack->party_arti[eArti_Str]; 
	_pNetwork->MyCharacterInfo.opt_dex = pPack->opDex + pPack->party_arti[eArti_Dex];
	_pNetwork->MyCharacterInfo.opt_intel = pPack->opInt + pPack->party_arti[eArti_Int]; 
	_pNetwork->MyCharacterInfo.opt_con = pPack->opCon + pPack->party_arti[eArti_Con]; 
	_pNetwork->MyCharacterInfo.sp = pPack->skillPoint; 
	_pNetwork->MyCharacterInfo.weight = 0; 
	_pNetwork->MyCharacterInfo.maxweight = 0; 
	_pNetwork->MyCharacterInfo.walkspeed = pPack->walkSpeed; 
	_pNetwork->MyCharacterInfo.runspeed = pPack->run_or_fly_speed; 
	_pNetwork->MyCharacterInfo.attackspeed = pPack->attackSpeed; 
	_pNetwork->MyCharacterInfo.magicspeed = pPack->magicSpeed;
	_pNetwork->MyCharacterInfo.pktitle			= pPack->pkName; 
	_pNetwork->MyCharacterInfo.pkpenalty		= pPack->pkPenalty; 
	_pNetwork->MyCharacterInfo.pkcount			= pPack->pkCount;
	_pNetwork->MyCharacterInfo.fame				= pPack->fame;
	_pNetwork->MyCharacterInfo.attackrange		= pPack->attackRange;
	_pNetwork->MyCharacterInfo.sbJoinFlagMerac	= pPack->joinForMerac;
	_pNetwork->MyCharacterInfo.sbJoinFlagDratan	= pPack->joinForDratan; // WSS_DRATAN_SIEGEWARFARE 070720

	// 속성 시스템 [1/17/2013 Ranma]
	UBYTE nAttrdef = GET_AT_VAR(pPack->attrDef);
	UBYTE nAttrdefLv = GET_AT_LV(pPack->attrDef);
	UBYTE nAttratt = GET_AT_VAR(pPack->attrAtt);
	UBYTE nAttrattLv = GET_AT_LV(pPack->attrAtt);

	if (nAttratt <= 0 || nAttratt >= ATTRIBUTE_ATT_MAX)
	{
		nAttratt = 0; 
		nAttrattLv = 1;
	}

	if (nAttrdef <= 0 || nAttrdef >= ATTRIBUTE_ATT_MAX)
	{
		nAttrdef = 0;	
		nAttrdefLv = 1;
	}

	_pNetwork->MyCharacterInfo.attrdef			= nAttrdef; // 방어 속성
	_pNetwork->MyCharacterInfo.attrdefLv		= nAttrdefLv;  // 방어 속성 Level
	_pNetwork->MyCharacterInfo.attratt			= nAttratt; // 공격 속성
	_pNetwork->MyCharacterInfo.attrattLv		= nAttrattLv;  // 공격 속성 Level

	_pNetwork->MyCharacterInfo.skillSpeed		= pPack->skillSpeed;
	_pNetwork->MyCharacterInfo.sbAttributePos	= pPack->mapAttr;
	_pNetwork->MyCharacterInfo.sbMountPet		= pPack->mountPet;
	_pNetwork->MyCharacterInfo.nEvocationIndex	= pPack->evocationIndex;

	// 080623 라카 기자단 혜택
	_pNetwork->MyCharacterInfo.sbPresscorps		= pPack->bPressCorps;

	_pNetwork->MyCharacterInfo.baseHP			= pPack->maxHpOrg;
	_pNetwork->MyCharacterInfo.baseMP			= pPack->maxMpOrg;
	_pNetwork->MyCharacterInfo.addedAttack 		= pPack->attack_diff;
	_pNetwork->MyCharacterInfo.addedMagic 		= pPack->magic_diff;
	_pNetwork->MyCharacterInfo.addedDefense 	= pPack->defense_diff;
	_pNetwork->MyCharacterInfo.addedResist 		= pPack->resist_diff;

	_pNetwork->MyCharacterInfo.dodgeRate		= pPack->avoid_melee;
	_pNetwork->MyCharacterInfo.baseDodgeRate 	= pPack->avoid_melee_org;
	_pNetwork->MyCharacterInfo.magicDodgeRate 	= pPack->avoid_magic;
	_pNetwork->MyCharacterInfo.baseMagicDodgeRate	= pPack->avoid_magic_org;
	_pNetwork->MyCharacterInfo.hitRate 			= pPack->fhitrate_melee;
	_pNetwork->MyCharacterInfo.baseHitRate 		= pPack->fhitrate_melee_org;

	_pNetwork->MyCharacterInfo.magicHitRate		= pPack->fhitrate_magic;
	_pNetwork->MyCharacterInfo.baseMagicHitRate = pPack->fhitrate_magic_org;
	_pNetwork->MyCharacterInfo.critical 		= pPack->critical;
	_pNetwork->MyCharacterInfo.baseCritical 	= pPack->critical_org;
	_pNetwork->MyCharacterInfo.deadly 			= pPack->deadly;
	_pNetwork->MyCharacterInfo.baseDeadly 		= pPack->deadly_org;

	_pNetwork->MyCharacterInfo.baseRunSpeed 	= pPack->run_or_fly_speed_org;
	_pNetwork->MyCharacterInfo.baseAttackSpeed 	= pPack->attackSpeedOrg;

	_pNetwork->MyCharacterInfo.ep				= pPack->ep;
	_pNetwork->MyCharacterInfo.maxEP			= pPack->maxEp;

	MyInfo* pInfo = MY_INFO();
	pInfo->SetBuyAddition(pPack->opMoney_buy_addition);
	pInfo->SetBuyRate(pPack->opMoney_buy_rate);
	pInfo->SetSellAddition(pPack->opMoney_sell_addition);
	pInfo->SetSellRate(pPack->opMoney_sell_rate);

	CUIManager* pUIManager = CUIManager::getSingleton();

	pUIManager->GetSimplePlayerInfo()->UpdatePlayerBaseInfo();
	pUIManager->GetCharacterInfo()->UpdateStatus( pPack->attack, pPack->magic, pPack->defense, pPack->resist);		// yjpark

#ifdef XTRAP_SECURE_CKBANG_2010_07_20
	if (g_pXTrap_C_SetUserInfoEx_Interface)
	{
		g_pXTrap_C_SetUserInfoEx_Interface("ssss",
			"",
			"",
			"",
			0,
			0);
	}
#endif

	//[ttos_2009_1_15]:쿨타임 감소 및 MP량 감소 옵션을 클라에서 계산하지 않고 서버에서 받음
	pUIManager->SetCoolTimeReductionRate(pPack->prob);
	pUIManager->SetNeedMPReductionRate(pPack->opDecreaseSkillMP);

	//CPrintF(TRANS("MSG_STATUS1: (level)%d (curExp)%I64d (needExp)%I64d (HP)%d/%d (MP)%d/%d (str)%d (dex)%d\n"), level, curExp, needExp, hp, maxHP, mp, maxMP, str, dex);
	//CPrintF(TRANS("MSG_STATUS2: (int)%d (con)%d (opt_str)%d (opt_dex)%d (opt_int)%d (opt_con)%d (attack)%d (magic)%d\n"), _int, con, opt_str, opt_dex, opt_int, opt_con, attack, magic);
	//CPrintF(TRANS("MSG_STATUS3: (defence)%d (resist)%d (sp)%d (weight)%d (maxweight)%d (walkspd)%f (runspd)%f (attackspd)%d  \n"), defense, resist, sp, weight, maxweight, walkspeed, runspeed, attackspeed);

	// [100107: selo] 트레이너 피하기 위한 중요 값 저장	
	CPlayerEntity* penPlayerEntity = (CPlayerEntity*)CEntity::GetPlayerEntity(0); //캐릭터 자기 자신
	penPlayerEntity->SetImportantValues(pPack->walkSpeed, pPack->run_or_fly_speed, pPack->attackRange, pPack->attackSpeed, pPack->magicSpeed);	 

	// Date : 2005-09-07(오후 5:31:54), By Lee Ki-hwan
	// sbMountPet : 팻을 타고있는지에 대한 정보
	// 0 타고 있지 않음, 
	// 그외 PetType과 Age가 bit정보로 옴.
	if( pPack->mountPet ) 
	{
		//-----------------------------------------------------------	
		INDEX iPetType	= -1;
		INDEX iPetAge	= -1;
		_pNetwork->CheckPetType( pPack->mountPet, iPetType, iPetAge );

		// NOTE : 탈수 있는 애완동물인지 판단하여, 캐릭터를 태웁니다.
		// NOTE : 애완동물을 타고 있을 경우에는, Pet타겟 정보를 추가하지 않음(내 애완동물일때는 펫 타겟 정보창 설정할것...)
		const BOOL bPetRide = PetInfo().IsRide(iPetType, iPetAge);		

		if( bPetRide )
		{
			//-----------------------------------------------------------
			if( !((CPlayerEntity*)CEntity::GetPlayerEntity(0))->IsRidePet(CEntity::GetPlayerEntity(0)) )
			{				
				CNetworkLibrary::sPetInfo	TempPet;
				CPetTargetInfom* pPetInfo = INFO()->GetMyPetInfo();

				
				TempPet.lIndex = pPetInfo->lIndex;
				pPetInfo->bIsActive	= TRUE;
				pPetInfo->pen_pEntity	= NULL;
				pPetInfo->iType			= iPetType;
				pPetInfo->iAge			= iPetAge;

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
					pPetInfo->lIndex		= TempPet.lIndex;
					pPetInfo->iType			= iPetType;
					pPetInfo->iAge			= iPetAge;						
					pPetInfo->strNameCard	= (*iter).strNameCard;
				}

				// [070824: Su-won] PET_COLOR_CHANGE
				INDEX iPetColoredType = iPetType | (pPack->colorType<<8);
				_pNetwork->RidePet( (CPlayerEntity*)CEntity::GetPlayerEntity(0), NULL, iPetColoredType );
			}
		}
		//_pNetwork->MyCharacterInfo.bPetRide = bPetRide;
	}

	if(_pNetwork->MyCharacterInfo.hp==0)
	{		
		CEntity* penPlEntity;
		CPlayerEntity* penPlayerEntity;
		penPlEntity = CEntity::GetPlayerEntity(0); //캐릭터 자기 자신
		penPlayerEntity = (CPlayerEntity*) penPlEntity;
		if(!penPlayerEntity->IsAlreadyDie())
		{
			//	penPlayerEntity->DeathYou();
			penPlayerEntity->SetDie();
			_pNetwork->MyCharacterInfo.statusEffect.Reset();
		}
		// WSS_DRATAN_SIEGEWARFARE 2007/10/17
		// 공성 부활대기시 죽었을 경우 처리		
		if( !pUIManager->GetSiegeWarfareNew()->GetWarState()&&
			(pUIManager->GetSiegeWarfareNew()->GetWaitTime()>=0))
		{			
			penPlayerEntity->DeathYou();
		}

		// [090908: selo]
		// 귀환주문서 사용 중 죽을 수도 있어서
		// 내가 죽을 때 인벤토리 Lock을 풀어준다.
		pUIManager->SetCSFlagOff( CSF_TELEPORT );
		pUIManager->GetInventory()->Lock( FALSE, FALSE, LOCK_WARP );

		// connie [2009/10/1] - NPC 찾기 
		pUIManager->GetNpcScroll()->CloseNpcScroll();
	}
#ifdef NEW_CHAO_SYS
	if (pPack->pkPenalty  == 32000)
	{
		if (!(PK_SYS_CHAO_6 & _pNetwork->MyCharacterInfo.pkSysRewardFlag))
		{
			if(_pNetwork->MyCharacterInfo.bpkSysRewardLate == FALSE)
			{//요청 패킷
				_pNetwork->pkPenaltyReformRewardReq(5);
				_pNetwork->MyCharacterInfo.bpkSysRewardLate= TRUE;
			}
		}
	}
	else if(pPack->pkPenalty  >= 19000 && pPack->pkPenalty  < 32000)
	{
		if (!(PK_SYS_CHAO_5&_pNetwork->MyCharacterInfo.pkSysRewardFlag))
		{
			if(_pNetwork->MyCharacterInfo.bpkSysRewardLate == FALSE)
			{//요청 패킷
				_pNetwork->pkPenaltyReformRewardReq(4);
				_pNetwork->MyCharacterInfo.bpkSysRewardLate= TRUE;
			}
		}
	}
	else if(pPack->pkPenalty  >= 6000 && pPack->pkPenalty  < 19000)
	{
		if (!(PK_SYS_CHAO_4&_pNetwork->MyCharacterInfo.pkSysRewardFlag))
		{
			if(_pNetwork->MyCharacterInfo.bpkSysRewardLate == FALSE)
			{	//요청 패킷
				_pNetwork->pkPenaltyReformRewardReq(3);
				_pNetwork->MyCharacterInfo.bpkSysRewardLate= TRUE;
			}
		}
	}
	else if(pPack->pkPenalty  > -19000 && pPack->pkPenalty  <= -6000)
	{
		if (!(PK_SYS_CHAO_3&_pNetwork->MyCharacterInfo.pkSysRewardFlag))
		{
			if(_pNetwork->MyCharacterInfo.bpkSysRewardLate == FALSE)
			{	//요청 패킷
				_pNetwork->pkPenaltyReformRewardReq(2);
				_pNetwork->MyCharacterInfo.bpkSysRewardLate= TRUE;
			}
		}
	}
	else if(pPack->pkPenalty  > -32000 && pPack->pkPenalty  <= -19000)
	{
		if (!(PK_SYS_CHAO_2&_pNetwork->MyCharacterInfo.pkSysRewardFlag))
		{
			if(_pNetwork->MyCharacterInfo.bpkSysRewardLate == FALSE)
			{	//요청 패킷
				_pNetwork->pkPenaltyReformRewardReq(1);
				_pNetwork->MyCharacterInfo.bpkSysRewardLate= TRUE;
			}
		}
	}
	else if(pPack->pkPenalty  == -32000)
	{
		if (!(PK_SYS_CHAO_1&_pNetwork->MyCharacterInfo.pkSysRewardFlag))
		{
			if(_pNetwork->MyCharacterInfo.bpkSysRewardLate == FALSE)
			{	//요청 패킷
				_pNetwork->pkPenaltyReformRewardReq(0);
				_pNetwork->MyCharacterInfo.bpkSysRewardLate= TRUE;
			}
		}
	}
#endif //NEW_CHAO_SYS
}

void CSessionState::updateAt( CNetworkMessage* istr )
{
	UpdateClient::charAtMsg* pPack = reinterpret_cast<UpdateClient::charAtMsg*>(istr->GetBuffer());
	
	pPack->hairstyle %= 10;
	CUIManager* pUIManager = CUIManager::getSingleton();

	//0707
#ifdef SECRET_KEY
	pPack->key = (pPack->key >> 1) - pPack->charIndex; 
	_pNetwork->MyCharacterInfo.secretkey = (SLONG)pPack->key;
#endif

#ifdef USE_UIAD
	if( pUIManager->GetAD() )
	{
		pUIManager->GetAD()->ClearListQ();
		pUIManager->GetAD()->AddListQ(0);
	}
#endif


	CGame* _pGame			= pUIManager->GetGame();
	CPlayerCharacter &pc	= _pGame->gm_apcPlayers[0];
	pc.pc_iPlayerType		= pPack->job;

	((CPlayerEntity*)CEntity::GetPlayerEntity(0))->CharacterChanged(pc);

	_pNetwork->MyCharacterInfo.index = pPack->charIndex;
	_pNetwork->MyCharacterInfo.name = pPack->charName;
	_pNetwork->MyCharacterInfo.job = pPack->job;
	_pNetwork->MyCharacterInfo.job2 = pPack->job2;
	_pNetwork->MyCharacterInfo.zoneNo = pPack->zoneIndex;
	_pNetwork->MyCharacterInfo.areaNo = pPack->areaIndex;
	_pNetwork->MyCharacterInfo.x = pPack->x;
	_pNetwork->MyCharacterInfo.z = pPack->z; 
	_pNetwork->MyCharacterInfo.h = pPack->h;
	_pNetwork->MyCharacterInfo.r = pPack->r;	
	_pNetwork->MyCharacterInfo.yLayer = pPack->yLayer;
	_pNetwork->MyCharacterInfo.userIndex = pPack->userIndex;
	_pNetwork->MyCharacterInfo.hairStyle = pPack->hairstyle;
	_pNetwork->MyCharacterInfo.faceStyle = pPack->facestyle;
	_pNetwork->MyCharacterInfo.pk_mode   = 0; 
	_pNetwork->MyCharacterInfo.sbShopType		= PST_NOSHOP;
	_pNetwork->MyCharacterInfo.lGuildIndex		= -1;
	_pNetwork->MyCharacterInfo.lGuildLevel		= -1;
	_pNetwork->MyCharacterInfo.lGuildPosition	= GUILD_MEMBER_NOMEMBER;
	_pNetwork->MyCharacterInfo.sbGuildRank		= 0;
	_pNetwork->MyCharacterInfo.lTeacherIndex	= -1;	
	_pNetwork->MyCharacterInfo.fame				= 0;	
	_pNetwork->MyCharacterInfo.strTeacherName.Clear();
	_pNetwork->MyCharacterInfo.bExtension = FALSE;

	STAGEMGR()->SetNextStage(eSTAGE_GAMEPLAY);
	pUIManager->SetUIGameState(TRUE);

	pUIManager->GetMessenger()->SetMyInfo( pPack->charIndex, pPack->charName,	(eJob)pPack->job, ONLINE );

#ifdef	VER_TEST
	extern INDEX		sam_bFullScreenActive;
	extern INDEX		sam_iScreenSizeI;
	extern INDEX		sam_iScreenSizeJ;

	pUIManager->SetTitleName( sam_bFullScreenActive, sam_iScreenSizeI, sam_iScreenSizeJ );
#endif	// VER_TEST

	_pNetwork->MyCharacterInfo.sbItemEffectOption = pPack->plusEffect;	

	if ( !(pPack->mapAttr & MATT_UNWALKABLE) )
	{
		_pNetwork->MyCharacterInfo.sbAttributePos = pPack->mapAttr;
	}
	//[ttos_2009_1_23]:채팅 금지
#ifdef CHATTING_BAN
	_pNetwork->MyCharacterInfo.ChatFlag = pPack->chatingFlag;
#endif
	//안태훈 수정 시작	//(Zone Change System)(0.1)
	g_slZone = pPack->zoneIndex;
	_pNetwork->ga_sesSessionState.ses_bWantPause = FALSE;
	//안태훈 수정 끝	//(Zone Change System)(0.1)

	CEntity::GetPlayerEntity(0)->SetNetworkID(pPack->charIndex);
	((CPlayerEntity*)CEntity::GetPlayerEntity(0))->Read_net_Character(MSG_AT, 0, &(*istr));	 

	if(_pUISWDoc->IsWar())
	{
		pUIManager->GetMap()->SetCurrentWorldMap( pPack->zoneIndex, 1 );		// yjpark
	}
	else
	{
		pUIManager->GetMap()->SetCurrentWorldMap( pPack->zoneIndex, pPack->yLayer );		// yjpark
	}

	if( pUIManager->IsCSFlagOn( CSF_PARTY ) )
	{
		pUIManager->GetParty()->open();
	}

	// [090715: selo] 레이드에서 나갈때는 퀘스트 북의 레이드 메시지를 비운다
	if(!pUIManager->IsPlayInZone()) //인스턴트존내부에서 나갈경우
	{		
		GAMEDATAMGR()->GetQuest()->RemoveRaidMessageAll();
	}

	_pNetwork->SendRegenList();

	_pNetwork->MyCharacterInfo.iNickType = pPack->currentTitle;
	((CPlayerEntity*)CEntity::GetPlayerEntity(0))->SetNickNameDamageEffect(pPack->currentTitle, CEntity::NICKNAME_ATTACH_EFFECT);

	pUIManager->GetCharacterInfo()->RegisterActions(); // 캐릭터 정보창 셋팅

	MY_INFO()->_guildmark.remove();

#ifdef ADD_SUBJOB
	_pNetwork->MyCharacterInfo.slSubJob = pPack->jobSub;
#endif
}

void CSessionState::updateNpcAppear( CNetworkMessage *istr )
{
	UpdateClient::appearNpc* pPack = reinterpret_cast<UpdateClient::appearNpc*>(istr->GetBuffer());

	CPlacement3D	plPlacement;
	CTString		name;
	INDEX			Ownerindex = -1;
	CTString		Ownername = "";
	CTString		Ownershopname = "";

	CEntity* penEntity;
	CEntity* penPlEntity;
	CPlayerEntity* penPlayerEntity;
	penPlEntity = CEntity::GetPlayerEntity(0); //캐릭터 자기 자신
	penPlayerEntity = (CPlayerEntity*) penPlEntity;

	plPlacement.pl_PositionVector(1) = pPack->x;
	plPlacement.pl_PositionVector(2) = pPack->h + 1.0f;
	plPlacement.pl_PositionVector(3) = pPack->z;
	plPlacement.pl_OrientationAngle(1) = pPack->r;
	plPlacement.pl_OrientationAngle(2) = 0;
	plPlacement.pl_OrientationAngle(3) = 0;

	CMobData* MD = CMobData::getData(pPack->charDbIndex);
	if(MD->GetMobIndex() == -1)
	{
		OutputDebugString("Cannot find Mob Data!!!\n");
		return;
	}

	UBYTE ubAttrdef = GET_AT_VAR(pPack->attrdef);
	UBYTE ubAttratt = GET_AT_VAR(pPack->attratt);
	UBYTE ubAttrdefLv = GET_AT_LV(pPack->attrdef);
	UBYTE ubAttrattLv = GET_AT_LV(pPack->attratt);

	if (ubAttratt <= 0 || ubAttratt >= ATTRIBUTE_ATT_MAX)
	{
		ubAttratt = 0;
		ubAttrattLv = 1;
	}	

	if (ubAttrdef <= 0 || ubAttrdef >= ATTRIBUTE_ATT_MAX)
	{
		ubAttrdef = 0; 
		ubAttrdefLv = 1;	
	}

	MD->SetAttribute(ubAttratt, ubAttrdef, ubAttrattLv, ubAttrdefLv);

	DeleteObject(MSG_CHAR_NPC, pPack->charVirtualIndex);
	CMobTarget* pMT = new CMobTarget;	

	penEntity = _pNetwork->ga_World.CreateEntity_t(plPlacement, CLASS_ENEMY, -1,TRUE);	
	penEntity->SetNetworkID(pPack->charVirtualIndex);

	BOOL bNpc = FALSE;
	if( MD->IsSkillMaster() || MD->IsSSkillMaster() || MD->IsNPC() )
	{
		bNpc = TRUE;
	}
	
	BuffInfo	sBuff;
	sBuff.m_llStartTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();

	ASSERT( pMT->mob_BuffCount == 0 );
	pMT->mob_BuffCount = 0;
	for( int i = 0; i < pPack->assistInfo.count; i++ )
	{
		sBuff.m_slItemIndex = pPack->assistInfo.list[i].index;
		sBuff.m_slSkillIndex = pPack->assistInfo.list[i].dbIndex;
		sBuff.m_sbLevel = pPack->assistInfo.list[i].level;
		sBuff.m_slRemain = pPack->assistInfo.list[i].remain;
		sBuff.m_slRemainCount = pPack->assistInfo.list[i].remainCount;
		pMT->AddBuff( sBuff );
	}

	// [sora] 원정대 시스템 대상 표식 index
	pMT->mob_Label = pPack->expedLabel;

	if ( !(pPack->mapAttr & MATT_UNWALKABLE) )
	{
		pMT->mob_sbAttributePos = pPack->mapAttr;
	}

	const char *szMobName = CMobData::getData(pPack->charDbIndex)->GetName();
	CTString strMobName;

	if (MD->IsTotemItem())
	{
		if (pPack->ownerIndex == _pNetwork->MyCharacterInfo.index)
		{
			Ownername = _pNetwork->MyCharacterInfo.name;
			_pNetwork->MyCharacterInfo.useTotem = true;
		}
		else
		{
			ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_CHARACTER, pPack->ownerIndex);
			if (pObject != NULL)
				Ownername = pObject->m_strName.c_str();
		}
		strMobName.PrintF("%s%s %s", Ownername,_S(2228, "의"), szMobName );
	}
	else
	{
		strMobName = szMobName;
	}

	pMT->SetData(pPack->charVirtualIndex, pPack->charDbIndex, strMobName, MD->GetLevel(), penEntity, bNpc, pPack->yLayer );
	pMT->SetSyndicateData(MD->GetSyndicateType(), MD->GetSyndicateGrade());

	penEntity->InitAsSkaModel();
	pMT->m_nIdxClient	= penEntity->en_ulID;

	// 속성 추가
	pMT->SetAttribute(eATTR_ATT, ubAttratt, ubAttrattLv);
	pMT->SetAttribute(eATTR_DEF, ubAttrdef, ubAttrdefLv);

	ACTORMGR()->AddObject(pMT);

	// 성주의 권좌일때....
	if( pPack->charDbIndex == LORD_SYMBOL_INDEX )
	{
		if( pPack->hp <= pPack->maxHp * 0.25f)
		{
			penEntity->SetSkaModel("data\\npc\\Gguard\\sword04.smc");			
		}
		else if( pPack->hp <= pPack->maxHp * 0.50f)
		{
			penEntity->SetSkaModel("data\\npc\\Gguard\\sword03.smc");			
		}
		else if( pPack->hp <= pPack->maxHp * 0.75f)
		{
			penEntity->SetSkaModel("data\\npc\\Gguard\\sword02.smc");			
		}			
		else
		{
			penEntity->SetSkaModel(MD->GetMobSmcFileName());
		}
	}
	else
	{
		penEntity->SetSkaModel(MD->GetMobSmcFileName());
	}

	penPlayerEntity->SetMobData(penEntity, pPack->hp, pPack->maxHp, MD->GetLevel(), bNpc, pPack->charDbIndex);
	CMobData::SetMobDataToNPC( penEntity, MD, szMobName );
	penEntity->Initialize();
	penEntity->en_pMobTarget = pMT;
	penEntity->FallDownToFloor();
	penEntity->GetModelInstance()->m_tmSkaTagManager.SetOwner(penEntity);

	// 공성 관련되서... hp가 0이라면...
	// 죽은 NPC이지만 사라지지는 않는 NPC로...(그리고 타겟팅도 안되도록...)
	if( (MD->IsCastleTower()) && pPack->hp <= 0 )
	{
		penEntity->SetFlagOff(ENF_ALIVE);
	}

	if( MD->IsShadowNPC() )
	{
		penEntity->SetFlagOn( ENF_HIDDEN );
	}

	if( pMT->m_nType == 491)
	{
		penEntity->SetFlagOff(ENF_ALIVE);
	}

	if( pMT->m_nType == 1152)
	{
		penEntity->SetFlagOff(ENF_ALIVE);
	}

	// assist_state 값을 비교해여 마법 부여에 대한 이펙트를 추가한다.
	// 버프가 끝날때까지 같은 모양을 유지하는 이펙트는 여기서 붙이고,
	// 피나 독이 뿜어져 나오는 것과 같은 이펙트는 MSG_CHAR_STATUS에서 붙인다.
	if(penEntity->en_pmiModelInstance)
	{
		pMT->ChangeStatus(&penEntity->en_pmiModelInstance->m_tmSkaTagManager, pPack->assistInfo.state);
	}
	else
	{
		ASSERTALWAYS("Mob must have ska model and tag manager.");
	}

	if(bNpc)
	{
		ACTORMGR()->RefreshNPCQuestMark(pPack->charDbIndex);
	}

	//hardcoding effect
	if( penEntity->en_pmiModelInstance == NULL ) return;

	if (pPack->charDbIndex == MOB_FLOWERTREE_INDEX || pPack->charDbIndex == MOB_XMAS_TREE_CENTER || pPack->charDbIndex == MOB_XMAS_TREE_DECO || pPack->charDbIndex == GAMIGO_10TH_CAKE)
	{ //꽃나무, 크리스마스 트리(장식용, 광장용)
		CUIManager* pUIManager = CUIManager::getSingleton();

		pUIManager->GetFlowerTree()->SetMobFlowerTree(pMT); // 꽃나무 MobTarget정보 저장
		pUIManager->GetFlowerTree()->FlowerTreeUpdate(pPack->hp); 
	}
	pMT->BuffsStartGroupEffect();

	pMT->mob_iOwnerIndex = pPack->ownerIndex;

	if ( IS_FLAG_ON( pPack->assistInfo.state, EST_ASSIST_MERCENARY) )
	{
		pMT->SetSubType( CMobTarget::MST_MERCENARY );
		penEntity->SetFirstExtraFlagOn(ENF_EX1_MONSTER_MERCENARY);
	}
	else if ( IS_FLAG_ON( pPack->assistInfo.state, EST_ASSIST_SOUL_TOTEM_BUFF ) )
	{
		pMT->SetSubType( CMobTarget::MST_BUFF_TOTEM );
		penEntity->SetFirstExtraFlagOn(ENF_EX1_TOTEM);
	}
	else if ( IS_FLAG_ON( pPack->assistInfo.state, EST_ASSIST_SOUL_TOTEM_ATTK ) )
	{
		pMT->SetSubType( CMobTarget::MST_ATTACK_TOTEM );
		penEntity->SetFirstExtraFlagOn(ENF_EX1_TOTEM);
	}
	else if ( IS_FLAG_ON( pPack->assistInfo.state, EST_ASSIST_TRAP ) )
	{
		pMT->SetSubType( CMobTarget::MST_TRAP );
		// 위에서 항상 체크하여서 적용하므로 본인의 경우 적용됨.
		// 때문에 강제적으로 풀어줌.
		penEntity->SetFirstExtraFlagOn(ENF_EX1_TRAP);
		if ( pPack->ownerIndex == _pNetwork->MyCharacterInfo.index )
		{
			penEntity->SetFlagOff(ENF_HIDDEN);
		}
	}
	else if ( IS_FLAG_ON(pPack->assistInfo.state, EST_ASSIST_SUICIDE) )
	{
		pMT->SetSubType( CMobTarget::MST_SUICIDE );
		penEntity->SetFirstExtraFlagOn(ENF_EX1_SUICIDE);
	}
	else
	{
		pMT->SetSubType( CMobTarget::MST_NONE );

		if ( MD->IsTotemItem() )
		{
			penEntity->SetFirstExtraFlagOn(ENF_EX1_TOTEM_ITEM);
		}
	}
	// [2011/03/24 : Sora] 몬스터 용병 사이즈 설정
	if( pPack->ownerIndex > 0 )
	{
		if( pPack->mercenarySize > 0.0f )
		{
			penEntity->GetModelInstance()->StretchModel(FLOAT3D(pPack->mercenarySize, pPack->mercenarySize, pPack->mercenarySize));
		}
	}

	CTString strStateEffectName;

	if ( pMT->m_nType == 1374 )
	{
		GameDataManager* pGameDataManager = GameDataManager::getSingleton();
		if (pGameDataManager)
		{
			ExpressSystem* pExpressData = pGameDataManager->GetExpressData();

			if ( pExpressData )
			{
				if( pExpressData->GetNoticeFlag() > 0) 
				{
					pExpressData->ChangeMailNpc(MAIL_NPC_STATE_APPEAR);
					return;
				}
				else
				{
					pExpressData->ChangeMailNpc(MAIL_NPC_STATE_NONE);
				}
			}
		}
	}

	switch( pMT->m_nType )
	{
	case 190: {	strStateEffectName = "DK Normal State";	} break; //데스나이트
	case 236: { strStateEffectName = "npcGolem01"; } break; //아케인 자이언트
	case 454: { strStateEffectName = "halloween_mon"; penEntity->SetFlagOn(ENF_HIDDEN); } break; // 호박 귀신
	case 455: { strStateEffectName = "halloween_crow"; } break; // 호박 광대 허수아비
	case 1116: { strStateEffectName = "padocs_appear"; } break; // 파독스 출현 이펙트
	default:
		return;
	}

	pMT->mob_pNormalEffect = StartEffectGroup( strStateEffectName, &penEntity->en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());//
}

void CSessionState::updatePcAppear( CNetworkMessage *istr )
{
	UpdateClient::appearPc* pPack = reinterpret_cast<UpdateClient::appearPc*>(istr->GetBuffer());

	CPlacement3D	plPlacement;

	CEntity* penEntity;
	CEntity* penPlEntity;
	CPlayerEntity* penPlayerEntity;
	penPlEntity = CEntity::GetPlayerEntity(0); //캐릭터 자기 자신
	penPlayerEntity = (CPlayerEntity*) penPlEntity;
		
	plPlacement.pl_PositionVector(1) = pPack->x;
	plPlacement.pl_PositionVector(2) = pPack->h + 1.0f;
	plPlacement.pl_PositionVector(3) = pPack->z;
	plPlacement.pl_OrientationAngle(1) = pPack->r;
	plPlacement.pl_OrientationAngle(2) = 0;
	plPlacement.pl_OrientationAngle(3) = 0;

	DeleteObject(MSG_CHAR_PC, pPack->charIndex);
	// FIXME : Factory로 바꿀까???
	CCharacterTarget* pTarget = new CCharacterTarget;
	penEntity = _pNetwork->ga_World.CreateEntity_t(plPlacement,CTFILENAME("Classes\\Character.ecl"),-1,TRUE);

	CTString strName = pPack->charName;
	pTarget->SetData( pPack->charIndex, pPack->job, strName, penEntity, pPack->yLayer,pPack->playerState,pPack->pkPenalty );//0826
	pTarget->cha_iJob2 = pPack->job2;
	penEntity->InitAsSkaModel();
	pTarget->m_nIdxClient = penEntity->en_ulID;

#ifdef PREMIUM_CHAR
	if (pPack->isPremiumChar == true)
		pTarget->SetPremiumType(PREMIUM_CHAR_TYPE_FIRST);
	else
		pTarget->SetPremiumType(PREMIUM_CHAR_TYPE_NONE);
#endif	//	PREMIUM_CHAR

	ACTORMGR()->AddObject(pTarget);
	
	penPlayerEntity->SetChaData(penEntity->en_ulID, pPack->job, pPack->hp, pPack->maxHp, pPack->hairStyle, pPack->faceStyle, strName, pPack->playerState,pPack->pkName, pPack->pkPenalty);
	pTarget->SetItemEffectOption( pPack->plusEffect );
	
	penEntity->Initialize();
	penEntity->SetNetworkID(pPack->charIndex);
	penEntity->FallDownToFloor();
	penEntity->en_pCharacterTarget = pTarget;
	
	penPlayerEntity->SetCharCostumeWearing(pTarget, penEntity, &(*istr));
	penPlayerEntity->SetChaWearing(pTarget, penEntity, &(*istr));

	BuffInfo	sBuff;
	sBuff.m_llStartTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();

	ASSERT( pTarget->cha_BuffCount == 0 );
	pTarget->cha_BuffCount = 0;

	CEffectGroup* pEG=NULL;
	for( int k = 0; k < pPack->assistInfo.count; k++ )
	{
		sBuff.m_slItemIndex = pPack->assistInfo.list[k].index;
		sBuff.m_slSkillIndex = pPack->assistInfo.list[k].dbIndex;
		sBuff.m_sbLevel = pPack->assistInfo.list[k].level;
		sBuff.m_slRemain = pPack->assistInfo.list[k].remain;
		sBuff.m_slRemainCount = pPack->assistInfo.list[k].remainCount;
	
		CItemData* pItemData = _pNetwork->GetItemData(sBuff.m_slItemIndex);
		// wooss 070305 ------------------------------------>>
		// kw : WSS_WHITEDAY_2007
		// 6종 러브매직 스킬
		// 423 424 425 426 427 428
		if( sBuff.m_slSkillIndex >= 423 && sBuff.m_slSkillIndex <= 428 )
		{
			// 처음이면 이펙트를 생성함...
			if(pEG==NULL)
			{
				// 지속 이펙트 - 이후 삭제시 지우기위해 저장...
				pEG = StartEffectGroup( "STATE_LOVE", &penEntity->en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
				//		penEntity->GetModelInstance()->m_pEG = pEG;
			}

			sBuff.m_pEG =  pEG;
		}
		// -------------------------------------------------<<
		else if (pItemData && pItemData->GetType() == CItemData::ITEM_POTION && strlen(pItemData->GetArmorEffectName()) > 0)
		{
			pEG = StartEffectGroup(pItemData->GetArmorEffectName(), &penEntity->en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
			sBuff.m_pEG = pEG;
		}

		// wooss 070310 ----------------------->><<
		// kw : WSS_WHITEDAY_2007
		// Add lovemagic buff (wind - skill index 423 )
		if(sBuff.m_slItemIndex ==391||sBuff.m_slItemIndex == 1226||sBuff.m_slItemIndex ==1227 || sBuff.m_slSkillIndex == 423	//이속향상 물약일때 불이펙트를 위한 하드코딩..ㅡㅡ;
			|| sBuff.m_slItemIndex ==2407 || sBuff.m_slItemIndex ==2408	|| sBuff.m_slItemIndex ==2609 || sBuff.m_slItemIndex == 2845	//광속 아이템, 바람의 속도 물약
			|| sBuff.m_slItemIndex == 2500
			|| sBuff.m_slItemIndex == 5018 || sBuff.m_slItemIndex == 5019 // 광속 2 아이템
			)	
		{
			((CPlayerEntity*)CEntity::GetPlayerEntity(0))->SetSpeedUp(FALSE, pTarget->m_nIdxClient, TRUE);
		}

		pTarget->AddBuff( sBuff );
	}

	if (_pNetwork->SearchLegitList(pPack->charIndex))
	{
		((CPlayerEntity*)CEntity::GetPlayerEntity(0))->SetRepairLegit(pPack->charIndex, pTarget);
	}

	pTarget->cha_sbShopType = pPack->personalshop.type;		

	if(pPack->personalshop.type != PST_NOSHOP)
	{
		CTString strShopName = pPack->personalshop.name;
		pTarget->ShopMsg.SetChatMsg( strShopName );
		penPlayerEntity->SetShopData( pTarget->m_nIdxClient, pPack->personalshop.type );
	}
	// 길드 관련.

	pTarget->cha_sbGuildRank = pPack->guildinfo.guildMark;

	pTarget->cha_lGuildIndex	= pPack->guildinfo.index;
	pTarget->cha_strGuildName	= pPack->guildinfo.name;
	pTarget->cha_lGuildPosition	= pPack->guildinfo.pos;

#ifdef GUILD_MARK
	if( pPack->guildinfo.guildMark_row >= 0 )
	{
		if (pTarget->pGuildmark == NULL)
		{
			pTarget->pGuildmark = new CUIGuildMarkIcon;
			pTarget->pGuildmark->Create(NULL, 0, 0, 15, 15);
		}

		pTarget->pGuildmark->CalcUV(pPack->guildinfo.guildMark_row, pPack->guildinfo.guildMark_col, true);
		pTarget->pGuildmark->CalcUV(pPack->guildinfo.guildMark_bg_row, pPack->guildinfo.guildMark_bg_col, false);
	}
#endif

	// WSS_DRATAN_SEIGEWARFARE 2007/08/01
	// 교감 애니 적용
	if(pPack->playerState & PLAYER_STATE_CRISTAL_RESPOND)
	{
		((CPlayerEntity*)CEntity::GetPlayerEntity(0))->SetConsensus(pTarget->m_nIdxServer);
		_pUISWDoc->StartConsensusEffect(pTarget->m_nIdxServer, pTarget->m_pEntity); // WSS_DRATAN_SIEGEWARFARE 2007/10/14		
	}

	CUIManager* pUIManager = CUIManager::getSingleton();

	// assist_state 값을 비교해여 마법 부여에 대한 이펙트를 추가한다.
	// 버프가 끝날때까지 같은 모양을 유지하는 이펙트는 여기서 붙이고,
	// 피나 독이 뿜어져 나오는 것과 같은 이펙트는 MSG_CHAR_STATUS에서 붙인다.
	if(penEntity->en_pmiModelInstance)
	{
		CStatusEffect::eRelation rel = CStatusEffect::R_NONE;
		if(GAMEDATAMGR()->GetPartyInfo()->IsPartyMember(pTarget->m_nIdxServer))
		{
			// 051203 파티 멤버일 경우.  플래그를 켜줌.
			penEntity->SetSecondExtraFlagOn( ENF_EX2_MYPARTY );
			rel = CStatusEffect::R_PARTY;
		}
		pTarget->ChangeStatus(&penEntity->en_pmiModelInstance->m_tmSkaTagManager, pPack->assistInfo.state, rel);			
	}
	else
	{
		ASSERTALWAYS("Character must have ska model and tag manager.");
	}
#if defined(G_RUSSIA)
	if(IS_FLAG_ON(pPack->assistInfo.state, EST_ASSIST_INVISIBLE) && _pNetwork->m_ubGMLevel < 7)
#else
	if(IS_FLAG_ON(pPack->assistInfo.state, EST_ASSIST_INVISIBLE) && _pNetwork->m_ubGMLevel < 2)
#endif
	{
		penEntity->SetFlags(penEntity->GetFlags() | ENF_HIDDEN);
	}
	else
	{
		penEntity->SetFlags(penEntity->GetFlags()&~ENF_HIDDEN);
	}

	if(IS_FLAG_ON(pPack->assistInfo.state, EST_ASSIST_FAKEDEATH))
	{
		INDEX animID = ska_StringToID("ro_die_01");//Hardcoding
		penEntity->GetModelInstance()->AddAnimation(animID, AN_CLEAR, 1.0f, 0);
		//hardcoding, CCharacterBase의 m_bIdleAnim property를 가져온다.
		if(penEntity->en_pecClass)
		{
			CDLLEntityClass *pdecDLLBaseClass	= penEntity->GetClass()->ec_pdecDLLClass->dec_pdecBase;
			CEntityProperty *pEP = pdecDLLBaseClass->PropertyForTypeAndID(CEntityProperty::EPT_BOOL, 250);
			if(pEP)
			{
				ENTITYPROPERTY( &*penEntity, pEP->ep_slOffset, BOOL) = FALSE;
			}
		}
	}
	//0221 변신루틴
	pTarget->cha_nChangeMobId = pPack->changeIndex;

	if(pPack->changeIndex != -1)
	{
		penPlayerEntity->AppearChaPolymoph(penEntity, pPack->changeIndex);
	}

	pTarget->cha_nFame = pPack->fame;

	if ( !(pPack->mapAttr & MATT_UNWALKABLE) )
	{
		pTarget->cha_sbAttributePos = pPack->mapAttr;
	}

	pTarget->cha_sbJoinFlagMerac = pPack->merac_join_flag;
	pTarget->cha_sbJoinFlagDratan = pPack->dratan_join_flag;
	pTarget->cha_nTransformIndex = pPack->evocationIndex;

	if(pTarget->cha_nTransformIndex != 0)
	{
		penPlayerEntity->AppearChaTransform(penEntity, pTarget->cha_nTransformIndex);
	}

	if( pPack->pet.grade )
	{		
		INDEX iPetType;
		INDEX iPetAge;
		_pNetwork->CheckPetType( pPack->pet.grade, iPetType, iPetAge );				
		const BOOL bPetRide = PetInfo().IsRide(iPetType, iPetAge);

		pTarget->cha_iPetType = iPetType;
		pTarget->cha_iPetAge	= iPetAge;
		if( pTarget->cha_bPetRide != bPetRide )
		{
			// 펫을 타도록 설정해야 하는 경우...
			if( iPetType != -1 && iPetAge != -1 && bPetRide )			
			{
				// [070824: Su-won] PET_COLOR_CHANGE
				INDEX iPetColoredType = iPetType | (pPack->pet.color<<8);
				_pNetwork->RidePet( pTarget->m_pEntity, NULL, iPetColoredType );
			}
			pTarget->cha_bPetRide = bPetRide;
		}
	}

	if (pPack->isPlayerStateSupporter)
	{
		penPlayerEntity->PlAddAccessoryEffect(penEntity, pTarget);
	}

	pTarget->cha_sbPresscorps = pPack->pressCorps;	
	pTarget->cha_Label = pPack->expedLabel;
	pTarget->cha_NickType = pPack->currentTitle;

	if ( pPack->currentTitle > 0)	// 호칭이 있다면
	{
		penEntity->SetNickNameDamageEffect(pPack->currentTitle, CEntity::NICKNAME_ATTACH_EFFECT);
	}

	if (pPack->mountApetIndex > 0)
	{
		CEntity* tempOtherEntity = NULL;
		tempOtherEntity = _pNetwork->GetEntityByTypeIndex(MSG_CHAR_WILDPET, pPack->mountApetIndex);

		if (tempOtherEntity && tempOtherEntity->en_EntityUseType == CEntity::EU_NORMAL) // 이 순간 펫을 무조건 못 태운 상태다.
		{
			CWildPetTarget* pWild = tempOtherEntity->en_pWildPetTarget;
			pWild->m_bMount = TRUE;
			CTString szModelFileName = CWildPetData::getData(pWild->m_nIndex)->smcFile[pWild->m_sbTransStat];
			_pNetwork->RideWildPet( penEntity, tempOtherEntity, szModelFileName );
		}
	}

	if ( pPack->merac_join_flag > 0 )
	{
		// Date : 2005-07-13(오후 5:29:49), By Lee Ki-hwan
		// 공성 Effect
		_pUISWDoc->StartEffect( pPack->charIndex, penEntity, pPack->merac_join_flag, pPack->guildinfo.index );

		// 길드장 이펙트 추가 // Date : 2005-11-18(오후 3:43:49), By Lee Ki-hwan
		_pUISWDoc->StartGuildMasterEffect( pPack->charIndex, penEntity, pPack->merac_join_flag, pPack->guildinfo.index, pPack->guildinfo.pos );
	}	
	// WSS_DRATAN_SEIGEWARFARE 2007/08/29
	// 공성 이펙트 처리
	if( pPack->dratan_join_flag > 0 )
	{
		// TODO :: 이펙트 나오면 하위 처리...
		_pUISWDoc->StartEffect( pPack->charIndex, penEntity, pPack->dratan_join_flag, pPack->guildinfo.index );
		_pUISWDoc->StartGuildMasterEffect( pPack->charIndex, penEntity, pPack->dratan_join_flag, pPack->guildinfo.index, pPack->guildinfo.pos );
	}

	// Date : 2005-04-06(오후 5:04:54), By Lee Ki-hwan
	// 길드 전투 중이라면 길드 Index를 체크해서 Effect를 뿌려줌
	if ( pUIManager->GetGuildBattle()->IsInBattle() )
	{
		int nEnemyGuild = pUIManager->GetGuildBattle()->IsEnemyGuild( pPack->guildinfo.index );
		if( nEnemyGuild == 1 ) // 적군 (적색)
		{
			pUIManager->GetGuildBattle()->StartGuildEffect( pPack->charIndex, penEntity, TRUE );
		}
		else if( nEnemyGuild == -1 ) // 아군 (청색)
		{
			pUIManager->GetGuildBattle()->StartGuildEffect( pPack->charIndex, penEntity, FALSE );
		}
	}

	pTarget->cha_itemEffect.AddPetStashEffect( pPack->petStashManagerEffect, &penEntity->en_pmiModelInstance->m_tmSkaTagManager );

	pTarget->cha_iSyndicateType = pPack->syndicate.type;
	pTarget->cha_iSyndicateGrade = pPack->syndicate.grade;
}

void CSessionState::updatePetAppear( CNetworkMessage *istr )
{
	UpdateClient::appearPet* pPack = reinterpret_cast<UpdateClient::appearPet*>(istr->GetBuffer());

	CPlacement3D	plPlacement;	

	plPlacement.pl_PositionVector(1)	= pPack->x;
	plPlacement.pl_PositionVector(2)		= pPack->h + 1.0f;
	plPlacement.pl_PositionVector(3)	= pPack->z;
	plPlacement.pl_OrientationAngle(1)	= 0.0f ;
	plPlacement.pl_OrientationAngle(2)	= 0.0f;
	plPlacement.pl_OrientationAngle(3)	= 0.0f;

	INDEX iPetType	= -1;
	INDEX iPetAge	= -1;
	_pNetwork->CheckPetType( pPack->grade, iPetType, iPetAge );

	// NOTE : 탈수 있는 애완동물인지 판단하여, 캐릭터를 태웁니다.
	// NOTE : 애완동물을 타고 있을 경우에는, Pet타겟 정보를 추가하지 않음(내 애완동물일때는 펫 타겟 정보창 설정할것...)
	const BOOL bPetRide = PetInfo().IsRide(iPetType, iPetAge);	

	//-----------------------------------------------------------
	CEntity* penEntity = _pNetwork->ga_World.CreateEntity_t(plPlacement, CLASS_PET, -1, TRUE);

	if (bPetRide == FALSE)
	{
		// NOTE : 애완동물을 타고있는 경우에는, 그 엔티티는 캐릭터로 취급되기 때문에,
		// NOTE : 펫타겟을 추가하면 안된다.
		// NOTE : 만약에 추가하게 되면, DISAPPEAR에서 엔티티를 Destroy하기 때문에
		// NOTE : 애니메이션시 엔티티가 유효하지 않게 되어서 뻑나게 됨.
		// FIXME : Factory로 바꿀까???
		DeleteObject(MSG_CHAR_PET, pPack->charIndex);
		CPetTarget* pTarget = new CPetTarget;
		CTString strPetName = pPack->ownerName + _S(2228,"의 ") + CTString(" ") + PetInfo().GetName(iPetType, iPetAge);
		CTString strPetCardName = pPack->name;
		CTString strPetOwnerName = pPack->ownerName;
		pTarget->SetData( pPack->charIndex, strPetName, strPetOwnerName, pPack->ownerIndex, iPetType, iPetAge, penEntity, pPack->yLayer, strPetCardName);	
		pTarget->m_nIdxClient = penEntity->en_ulID;
		penEntity->SetNetworkID(pPack->charIndex);
		penEntity->en_pPetTarget = pTarget;

		ACTORMGR()->AddObject(pTarget);
	}

	((CPlayerEntity*)CEntity::GetPlayerEntity(0))->SetPetData( penEntity, pPack->hp, pPack->maxHp );

	BOOL bUseAI		= FALSE;

	// 내 펫이며, 인벤토리에 장착해서 데리구 다니는 펫일 경우.
	if( pPack->ownerIndex == _pNetwork->MyCharacterInfo.index )
	{
		// 펫 타겟 정보창 정보를 설정합니다.
		bUseAI		= TRUE;
		penEntity->SetFirstExtraFlagOn(ENF_EX1_CURRENT_PET);
		CPetTargetInfom* pPetInfo = INFO()->GetMyPetInfo();
		pPetInfo->lIndex		= pPack->charIndex;
		pPetInfo->bIsActive		= TRUE;
		pPetInfo->TransformIndex = pPack->turnToNpc; // 펫 변신 NPC 인덱스
		pPetInfo->pen_pEntity	= penEntity;

		_pNetwork->UpdatePetTargetInfo( pPack->charIndex );
	}	

	penEntity->InitAsSkaModel();
	penEntity->SetSkaModel(PetInfo().GetFileName(iPetType, iPetAge));
	PetInfo().SetPetDataToEntity(iPetType, iPetAge, penEntity, bUseAI);
	penEntity->SetNetworkID(pPack->charIndex);
	penEntity->Initialize();
	penEntity->FallDownToFloor();

	// 펫 색깔 변경 아이템을 사용한 상태라면 텍스쳐 파일을 바꿔줌...
	if( pPack->color >0 )
	{
		CTString strTexFile =PetInfo().GetColoredTexFileName(iPetType, iPetAge, pPack->color);
		penEntity->GetModelInstance()->mi_aMeshInst[0].mi_tiTextures[0].ti_toTexture.SetData_t( strTexFile );
	}

	// eons 펫 변신
	// 일단 펫 모델 설정을 거친 후 변신을 하자.
	if (pPack->turnToNpc > 0)
	{
		((CPlayerEntity*)CEntity::GetPlayerEntity(0))->TransfromPet(penEntity, pPack->turnToNpc, pPack->turnToNpcSize);
	}

#if defined (G_KOR)
	if( pPack->mapAttr & MATT_WAR)
		penEntity->SetFlagOn(ENF_HIDDEN);
#else
	if( pPack->mapAttr & MATT_WAR || pPack->mapAttr & MATT_PEACE)
		penEntity->SetFlagOn(ENF_HIDDEN);
#endif

	if(pPack->bNew){
		CTString strEffect;
		strEffect.PrintF( "pet_%c%d_summon", (iPetType==0)?'h':'d', iPetAge+1 );
		PetStartEffectGroup( strEffect, pPack->charIndex );
	}

}

void CSessionState::updateElementalAppear( CNetworkMessage *istr )
{
	UpdateClient::appearElemental* pPack = reinterpret_cast<UpdateClient::appearElemental*>(istr->GetBuffer());

	CPlacement3D	plPlacement;

	plPlacement.pl_PositionVector(1)	= pPack->x;
	plPlacement.pl_PositionVector(2)		= pPack->h + 1.0f;
	plPlacement.pl_PositionVector(3)	= pPack->z;
	plPlacement.pl_OrientationAngle(1)	= 0.0f;
	plPlacement.pl_OrientationAngle(2)	= 0.0f;
	plPlacement.pl_OrientationAngle(3)	= 0.0f;	

	CEntity* penEntity = NULL;
	if( _pNetwork->m_bSingleMode )
	{
		penEntity = _pNetwork->ga_World.CreateEntity_t(CEntity::GetPlayerEntity(0)->GetPlacement(), CLASS_SUMMON, -1, TRUE);
		if( penEntity )
			_pNetwork->ga_World.m_vectorTargetNPC.push_back(penEntity);
	}
	else
	{
		penEntity = _pNetwork->ga_World.CreateEntity_t(plPlacement, CLASS_SUMMON, -1, TRUE);
	}
	penEntity->InitAsSkaModel();	
	penEntity->SetSkaModel( SlaveInfo().GetFileName(pPack->elementalType) );		
	SlaveInfo().SetSlaveDataToEntity( pPack->elementalType, penEntity, pPack->ownerIndex == _pNetwork->MyCharacterInfo.index );
	penEntity->SetNetworkID(pPack->charIndex);
	penEntity->Initialize();
	//penEntity->FallDownToFloor();	
	((CPlayerEntity*)CEntity::GetPlayerEntity(0))->SetElementalData(penEntity, pPack->hp, pPack->maxHp);
	
	ObjInfo* pInfo = ObjInfo::getSingleton();
	CSlaveTargetInfom* pSlaveInfo = NULL;
	// 내 소환수라고 표시해줌.
	if( pPack->ownerIndex == _pNetwork->MyCharacterInfo.index )
	{
		CUIManager* pUIManager = CUIManager::getSingleton();
		penEntity->SetFirstExtraFlagOn(ENF_EX1_CURRENT_SLAVE);

		for( int i = UI_SUMMON_START; i <= UI_SUMMON_END; ++i )
		{
			CUISummon* pUISummon = (CUISummon*)pUIManager->GetUI(i);
			pSlaveInfo = pInfo->GetMySlaveInfo(i - UI_SUMMON_START);
			if( !pUISummon->GetSummonEntity() && pSlaveInfo != NULL )
			{
				pUISummon->SetSummonIndex(pPack->charIndex);
				pUISummon->SetSummonEntity( penEntity );
				pUISummon->SetSummonType( pPack->elementalType );
				//pUISummon->SetCommand(CSlaveInfo::COMMAND_PROTECTION);
				pSlaveInfo->fHealth	= pPack->hp;
				pSlaveInfo->fMaxHealth = pPack->maxHp;						
				break;
			}
		}
	}

	// NOTE : 애완동물을 타고있는 경우에는, 그 엔티티는 캐릭터로 취급되기 때문에,
	// NOTE : 펫타겟을 추가하면 안된다.
	// NOTE : 만약에 추가하게 되면, DISAPPEAR에서 엔티티를 Destroy하기 때문에
	// NOTE : 애니메이션시 엔티티가 유효하지 않게 되어서 뻑나게 됨.
	// FIXME : Factory로 바꿀까???
	DeleteObject(MSG_CHAR_ELEMENTAL, pPack->charIndex);	

	CSlaveTarget* pTarget = new CSlaveTarget;
	
	CTString strSlaveName = pPack->ownerName + _S(2228,"의 ") + CTString(" ") + SlaveInfo().GetName(pPack->elementalType);		// 번역
	CTString strOwnerName = pPack->ownerName;
	pTarget->SetData( pPack->charIndex, strSlaveName, strOwnerName, pPack->ownerIndex, pPack->elementalType, penEntity, pPack->yLayer );	
	pTarget->m_nIdxClient = penEntity->en_ulID;
	penEntity->en_pSlaveTarget	= pTarget;
	
	ACTORMGR()->AddObject(pTarget);

	BuffInfo	sBuff;
	sBuff.m_llStartTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();
	
	ASSERT( pTarget->slave_BuffCount == 0 );
	pTarget->slave_BuffCount = 0;
	for( int i = 0; i < pPack->assistInfo.count; i++ )
	{
		sBuff.m_slItemIndex = pPack->assistInfo.list[i].index;
		sBuff.m_slSkillIndex = pPack->assistInfo.list[i].dbIndex;
		sBuff.m_sbLevel = pPack->assistInfo.list[i].level;
		sBuff.m_slRemain = pPack->assistInfo.list[i].remain;
		sBuff.m_slRemainCount = pPack->assistInfo.list[i].remainCount;
		pTarget->AddBuff( sBuff );
	}

	// assist_state 값을 비교해여 마법 부여에 대한 이펙트를 추가한다.
	// 버프가 끝날때까지 같은 모양을 유지하는 이펙트는 여기서 붙이고,
	// 피나 독이 뿜어져 나오는 것과 같은 이펙트는 MSG_CHAR_STATUS에서 붙인다.
	if(penEntity->en_pmiModelInstance)
	{
		pTarget->ChangeStatus(&penEntity->en_pmiModelInstance->m_tmSkaTagManager, pPack->assistInfo.state);
	}
	else
	{
		ASSERTALWAYS("Character must have ska model and tag manager.");
	}

	SlaveInfo().StartIdleEffect( pPack->charIndex, pPack->elementalType, penEntity );		
	// 소환수 사이즈 조절 ( 캐릭 전직전 작게 )
	if(!pPack->isBigSize)
	{
		penEntity->en_pmiModelInstance->StretchModel(FLOAT3D( 0.7f,0.7f,0.7f ));
	}

#ifdef SORCERER_SUMMON_VILLAGE_VISIBLE_NA_20081008//IsFlagOn(ENF_HIDDEN)
#if defined (G_GERMAN) || defined (G_EUROPE3) || defined (G_EUROPE2) || defined (G_NETHERLANDS)
	if (pPack->mapAttr & MATT_PEACE)
		penEntity->SetFlagOn(ENF_HIDDEN);
#endif
#endif
}

void CSessionState::updateAPetAppear( CNetworkMessage *istr )
{
	UpdateClient::appearApet* pPack = reinterpret_cast<UpdateClient::appearApet*>(istr->GetBuffer());

	CPlacement3D	plPlacement;	
	INDEX			nItemIndex[APET_WEARPOINT], nItemPlus[APET_WEARPOINT];

	int				i;
	for( i = 0; i < APET_WEARPOINT; ++i )
	{
		nItemIndex[i] = pPack->weaponList[i].dbIndex;
		nItemPlus[i] = pPack->weaponList[i].plus;
	}
	
	plPlacement.pl_PositionVector(1)	= pPack->x;
	plPlacement.pl_PositionVector(2)		= pPack->h + 1.0f;
	plPlacement.pl_PositionVector(3)	= pPack->z;
	plPlacement.pl_OrientationAngle(1)	= 0.0f ;
	plPlacement.pl_OrientationAngle(2)	= 0.0f;
	plPlacement.pl_OrientationAngle(3)	= 0.0f;
	
	// 내가 장착한 펫 외의 펫은 안그리도록 방어
	// 마운트 상태가 아닐경우에만 체크(마운트 상태일때는 펫의 어피어가 한번 오게됨)
	if (pPack->ownerIndex == _pNetwork->MyCharacterInfo.index && _pNetwork->MyCharacterInfo.bWildPetRide == FALSE)
	{
		if (_pNetwork->MyWearItem[WEAR_PET].IsEmptyItem() == TRUE)
			return; // 항상 아이템이 먼저 장착된 후 펫이 그려짐		

		if (_pNetwork->MyWearItem[WEAR_PET].GetItemPlus() != pPack->charIndex)
			return; // 착용된 펫과 다른 펫을 가질 수 없음
	}
	
	CTString strFile = CWildPetData::getData(pPack->dbIndex)->smcFile[pPack->transSate];

	if (strFile.IsEmpty() == TRUE)
	{
		LOG_DEBUG("Error : updateAPetAppear No Pet Name!!!");
		return;
	}

	CEntity* penEntity = _pNetwork->ga_World.CreateEntity_t(plPlacement, CLASS_WILDPET, -1, TRUE);

	penEntity->InitAsSkaModel();
	penEntity->SetSkaModel(strFile);
	penEntity->SetNetworkID(pPack->charIndex);

	DeleteObject(MSG_CHAR_WILDPET, pPack->charIndex);

	CWildPetTarget* pTarget = new CWildPetTarget;

	CTString strWildPetName = pPack->name;
	pTarget->m_nIndex = pPack->dbIndex;
	pTarget->m_nLevel = pPack->level;
	pTarget->m_strName = strWildPetName;

	pTarget->m_sbTransStat = pPack->transSate;
	pTarget->m_bMount = pPack->isMount;
	// [ldy1978220 2011/7/6] ITS-1243 펫이 appear 정보 받을 때 배고픔 정보를 바로 받지 않기 때문에 
	// 배고픔 애니메이션이 출력되는 현상이 나타남으로 최소값 1로 세팅 
	pTarget->m_nStm = 1;	
	pTarget->m_nHP = pPack->hp;
	pTarget->m_nMaxHP = pPack->maxHp;
	pTarget->m_nIdxServer = pPack->charIndex;
	pTarget->m_nType = pPack->type;
	pTarget->m_nOwnerIndex = pPack->ownerIndex;
	pTarget->m_yLayer = pPack->yLayer;
	pTarget->m_sbAttributePos = pPack->mapAttr;
	pTarget->m_nIdxClient = penEntity->en_ulID;
	pTarget->SetWildPetToEntity(penEntity, pPack->dbIndex, nItemIndex, nItemPlus);

	ACTORMGR()->AddObject(pTarget);

	for( i = 0; i < 4; ++i )
	{
		pTarget->m_nPetWearIndex[i] = nItemIndex[i];
		pTarget->m_nPetWearPlus[i] = nItemPlus[i];
		_pGameState->WearingArmorTest(penEntity->GetModelInstance(), pTarget->m_nPetWearIndex[i]);
	}

	penEntity->en_pWildPetTarget	= pTarget;

	((CPlayerEntity*)CEntity::GetPlayerEntity(0))->SetWildPetData( penEntity, pPack->hp, pPack->maxHp );

	if (pTarget->m_nOwnerIndex == _pNetwork->MyCharacterInfo.index)
	{
		_pNetwork->MyCharacterInfo.bWildPetRide = pPack->isMount;
		pTarget->bIsActive = TRUE;
		penEntity->SetFirstExtraFlagOn(ENF_EX1_CURRENT_WILDPET);
		INFO()->SetMyApet(pTarget);
		CUIManager::getSingleton()->RearrangeOrder(UI_WILDPETTARGETINFO,TRUE);
	}

	penEntity->Initialize();
	// 서버에 의존 입는다.
	if( pPack->isMount )
	{
		CTString szModelFileName = CWildPetData::getData(pPack->dbIndex)->smcFile[pPack->transSate];
		if( pPack->ownerIndex == _pNetwork->MyCharacterInfo.index )
		{
			_pNetwork->RideWildPet( CEntity::GetPlayerEntity(0), penEntity, szModelFileName );
		}
		else
		{
			CEntity* tempOtherEntity = NULL;
			tempOtherEntity = _pNetwork->GetEntityByTypeIndex(MSG_CHAR_PC, pPack->ownerIndex);

			if( NULL != tempOtherEntity )
				_pNetwork->RideWildPet( tempOtherEntity, penEntity, szModelFileName );
			else	// 만약 마운트를 시도했는데 실패할 경우 모델은 기본 모델만 나온다.
			{
				pTarget->m_bMount = FALSE;
			}
		}
	}
	else
	{
		// 휴먼형이 아니라면 무조건 벗는다. [3/24/2011 rumist]
		if( 1 != CWildPetData::getData(pPack->dbIndex)->type )
		{
			if( pPack->ownerIndex == _pNetwork->MyCharacterInfo.index )
				_pNetwork->LeaveWildPet( CEntity::GetPlayerEntity(0) );
			else
			{
				// 나 아닐 때만 서칭.
				CEntity* tempOtherEntity = NULL;
				tempOtherEntity = _pNetwork->GetEntityByTypeIndex(MSG_CHAR_PC, pPack->ownerIndex);

				if( NULL != tempOtherEntity )
					_pNetwork->LeaveWildPet( tempOtherEntity );
			}
		}
	}
	// pet appear skill. 이건 펫을 장착해서 등장할때만 사용하는 스킬. [12/1/2010 rumist]
	ASSERT( 0 < pPack->dbIndex );
	int temSummonSkill = CWildPetData::getData(pPack->dbIndex)->nSummonSkill[pPack->transSate];
	
	if (pPack->isAction && temSummonSkill > 0 &&
		pPack->ownerIndex == _pNetwork->MyCharacterInfo.index )
	{
		CSelectedEntities m_dcEnemies;
		CSkill &SkillData = _pNetwork->GetSkillData(temSummonSkill);

		m_dcEnemies.Clear();
		m_dcEnemies.Add(penEntity);

		_pNetwork->SendSlaveSkillMessageInContainer(temSummonSkill, penEntity, m_dcEnemies, FALSE);

		int lv = MY_INFO()->GetSkillLevel(temSummonSkill) + 1;
		_pNetwork->FindTargetsInRange(penEntity, penEntity, m_dcEnemies, 
							SkillData.GetAppRange(), SkillData.GetTargetNum(lv) - 1, 360.0f, temSummonSkill );

		_pNetwork->SendSlaveSkillMessageInContainer(temSummonSkill, penEntity, m_dcEnemies, TRUE);
	}
	penEntity->FallDownToFloor();
}

void CSessionState::updateDisappear( CNetworkMessage *istr )
{
	UpdateClient::charDisappear* pPack = reinterpret_cast<UpdateClient::charDisappear*>(istr->GetBuffer());

	DeleteObject(pPack->charType, pPack->charIndex);
}

IMPLEMENT_MSG_UPDATE(updateDisappearEffect)
{
	UpdateClient::charDisappearForClientEffect* pPack = reinterpret_cast<UpdateClient::charDisappearForClientEffect*>(istr->GetBuffer());

	CEntity* penPlEntity;
	CPlayerEntity* penPlayerEntity;
	penPlEntity = CEntity::GetPlayerEntity(0); //캐릭터 자기 자신
	penPlayerEntity = static_cast<CPlayerEntity*>(penPlEntity);

	if (penPlayerEntity != NULL)
	{
		penPlayerEntity->Read_net_DissappearEffect(istr);
	}

	if (pPack->owner_index == _pNetwork->MyCharacterInfo.index)
		_pNetwork->MyCharacterInfo.useTotem = false;
}
IMPLEMENT_MSG_UPDATE(updateDisappearAll)
{
	INFO()->TargetClear();
	ACTORMGR()->RemoveAll();
}

IMPLEMENT_MSG_UPDATE(ClearTarget)
{
	UpdateClient::targetClear* pPack = reinterpret_cast<UpdateClient::targetClear*>(istr->GetBuffer());
	ObjInfo* pInfo = ObjInfo::getSingleton();
	
	if (pPack->targetIndex == -1 || 
		(pInfo->GetTargetServerIdx(eTARGET) == pPack->targetIndex))
	{
		pInfo->TargetClear();
	}	
}

void CSessionState::updateElementalStatus( CNetworkMessage* istr )
{
	UpdateClient::elementalStatus* pPack = reinterpret_cast<UpdateClient::elementalStatus*>(istr->GetBuffer());

	CUIManager* pUIManager = CUIManager::getSingleton();

	ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_SLAVE, pPack->charIndex);
	ObjInfo* pInfo = ObjInfo::getSingleton();
	CSlaveTargetInfom* pSlaveInfo = NULL;

	if (pObject != NULL)
	{
		CSlaveTarget* pTarget = static_cast< CSlaveTarget* >(pObject);

		if (pTarget->GetEntity())
		{
			((CPlayerEntity*)CEntity::GetPlayerEntity(0))->SetElementalData(pTarget->GetEntity(), pPack->hp, pPack->maxHp);
			((CPlayerEntity*)CEntity::GetPlayerEntity(0))->SetElementalStatus(
				pTarget->GetEntity(), pPack->attackSpeed, pPack->magicSpeed, pPack->skillSpeed, pPack->walkSpeed, pPack->runSpeed, pPack->attackRange);
		}

		pTarget->m_yLayer = pPack->yLayer;

		// 내 소환수인 경우...
		if (pTarget->slave_OwnerIndex == _pNetwork->MyCharacterInfo.index)
		{
			for( int i = UI_SUMMON_START; i <= UI_SUMMON_END; ++i )
			{
				CUISummon* pUISummon = (CUISummon*)pUIManager->GetUI(i);
				pSlaveInfo = pInfo->GetMySlaveInfo(i - UI_SUMMON_START);

				if( pUISummon->GetSummonEntity() && pUISummon->GetSummonIndex() == pPack->charIndex && pSlaveInfo != NULL )
				{
					pUISummon->SetMaxTime(pPack->remainTime);		// MaxTime은 한번만 설정됩니다.
					pSlaveInfo->fHealth	= pPack->hp;
					pSlaveInfo->fMaxHealth = pPack->maxHp;
					pUISummon->SetLeftTime(pPack->remainTime);
				}
			}
		}
		BuffInfo	sBuff;
		sBuff.m_llStartTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();	

		pTarget->slave_BuffCount = 0;
		for( int i = 0; i < pPack->assistinfo.count; i++ )
		{
			//(*istr) >> sBuff.m_slItemIndex >> sBuff.m_slSkillIndex >> sBuff.m_sbLevel >> sBuff.m_slRemain;	//원본
			sBuff.m_slItemIndex = pPack->assistinfo.list[i].index;
			sBuff.m_slSkillIndex = pPack->assistinfo.list[i].dbIndex;
			sBuff.m_sbLevel = pPack->assistinfo.list[i].level;
			sBuff.m_slRemain = pPack->assistinfo.list[i].remain;
			sBuff.m_slRemainCount = pPack->assistinfo.list[i].remainCount;	//2012/11/12 jeil 스킬확장관련 추가
			pTarget->AddBuff( sBuff );
		}
	}
}

void CSessionState::updatePartyMemberInfo( CNetworkMessage* istr )
{
	UpdateClient::partyInfo* pPack = reinterpret_cast<UpdateClient::partyInfo*>(istr->GetBuffer());
	GAMEDATAMGR()->GetPartyInfo()->PartyMemberInfo( pPack->charIndex, pPack->level, pPack->hp, pPack->maxHp, pPack->mp, pPack->maxMp, pPack->x, pPack->z, pPack->yLayer, pPack->zoneIndex, pPack->isOnline );
}

void CSessionState::updateExpedMemberInfo( CNetworkMessage* istr )
{
	UpdateClient::expedInfoForChar* pPack = reinterpret_cast<UpdateClient::expedInfoForChar*>(istr->GetBuffer());
	GAMEDATAMGR()->GetPartyInfo()->ExpeditionMemberInfo(pPack->charIndex, pPack->group, pPack->level, pPack->hp, pPack->maxHp, pPack->mp, pPack->maxMp, pPack->x, pPack->z, pPack->yLayer, pPack->zoneIndex, pPack->isOnline);
}

void CSessionState::updateStatusPC( CNetworkMessage *istr )
{
	((CPlayerEntity*)CEntity::GetPlayerEntity(0))->Read_net_StatusPC(istr);
}

void CSessionState::updateStatusNPC( CNetworkMessage *istr )
{
	((CPlayerEntity*)CEntity::GetPlayerEntity(0))->Read_net_StatusNPC(istr);
}

void CSessionState::updateStatusPet( CNetworkMessage *istr )
{	
	((CPlayerEntity*)CEntity::GetPlayerEntity(0))->Read_net_StatusPet(istr);
}

void CSessionState::updateStatusElemental( CNetworkMessage *istr )
{
	((CPlayerEntity*)CEntity::GetPlayerEntity(0))->Read_net_StatusElemental(istr);
}

void CSessionState::updateStatusAPet( CNetworkMessage* istr )
{
	UpdateClient::attackPetStatus* pPack = reinterpret_cast<UpdateClient::attackPetStatus*>(istr->GetBuffer());
	CUIManager* pUIManager = CUIManager::getSingleton();

	CPlayerEntity* penPlayerEntity;
	penPlayerEntity = static_cast<CPlayerEntity*>( CEntity::GetPlayerEntity(0) );

	ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_WILDPET, pPack->charIndex);

	if (pObject != NULL)
	{
		CWildPetTarget* pTarget = static_cast< CWildPetTarget* >(pObject);

		pTarget->m_strName = pPack->charName;
		pTarget->m_nLevel = pPack->level;
		pTarget->m_nType = pPack->protoType;
		pTarget->m_exp = pPack->exp;
		pTarget->m_next_exp = pPack->needExp;
		pTarget->m_nHP = pPack->hp;
		pTarget->m_nMaxHP = pPack->maxHp;
		pTarget->m_nMP = pPack->mp;
		pTarget->m_nMaxMP = pPack->maxMp;
		pTarget->m_nFaith = pPack->faith;
		pTarget->m_nMaxFaith = pPack->maxFaith;
		pTarget->m_nStm = pPack->stamina;
		pTarget->m_nMaxStm = pPack->maxStamina;
		pTarget->m_nLevelupPoint = pPack->remainStat;
		pTarget->m_sbAttributePos = pPack->mapAttr;
		pTarget->m_yLayer = pPack->yLayer;
		pTarget->m_nStr = pPack->str;
		pTarget->m_nStrPlus = pPack->addStr;
		pTarget->m_nCon = pPack->con;
		pTarget->m_nConPlus = pPack->addCon;
		pTarget->m_nDex = pPack->dex;
		pTarget->m_nDexPlus = pPack->addDex;
		pTarget->m_nInt = pPack->intl;
		pTarget->m_nIntPlus = pPack->addIntl;

		pTarget->m_nWildPetState[STATE_ATTACK] = pPack->attack;
		pTarget->m_nWildPetState[STATE_MAGIC_ATTACK] = pPack->magicAttack;
		pTarget->m_nWildPetState[STATE_DEFENCE] = pPack->defence;
		pTarget->m_nWildPetState[STATE_MAGIC_DEFENCE] = pPack->maxDefence;
		pTarget->m_nWildPetState[STATE_HITPOINT] = pPack->hitPoint;
		pTarget->m_nWildPetState[STATE_AVOID] = pPack->avoid;
		
		pTarget->m_sbTransStat = pPack->transSate;
		pTarget->m_bMount = pPack->isMount;

		if (penPlayerEntity && pTarget->GetEntity())
		{
			penPlayerEntity->SetWildPetData(pTarget->GetEntity(), pPack->hp, pPack->maxHp);
		}

		if (pTarget->m_nOwnerIndex == _pNetwork->MyCharacterInfo.index)
		{
			INFO()->SetMyApet(pTarget);

			sPetItem_Info TemPet;

			TemPet.pet_index = pPack->charIndex;
			TemPet.pet_name = pPack->charName;
			TemPet.pet_level = pPack->level;
			TemPet.pet_str = pPack->str;
			TemPet.pet_con = pPack->con;
			TemPet.pet_dex = pPack->dex;
			TemPet.pet_int = pPack->intl;
			if (pPack->coolTime > 0)
			{
				TemPet.pet_cooltime = (unsigned int)_pTimer->GetLerpedCurrentTick() + pPack->coolTime;
			}else
			{
				TemPet.pet_cooltime = 0;
			}

			TemPet.pet_accexp = pPack->accExp;
			pUIManager->GetWildPetInfo()->AddWildPetInfo(TemPet);
		}
	}
}

void CSessionState::updateDamage( CNetworkMessage* istr )
{
	UpdateClient::charDamage* pPack = reinterpret_cast<UpdateClient::charDamage*>(istr->GetBuffer());

	CEntity* penPlEntity;
	CPlayerEntity* penPlayerEntity;
	penPlEntity = CEntity::GetPlayerEntity(0); //캐릭터 자기 자신
	penPlayerEntity = static_cast<CPlayerEntity*>(penPlEntity);

	if (pPack->charType == MSG_CHAR_PC)
	{
		penPlayerEntity->Read_net_Damage_Character(istr);
	}
	else
	{
		// 플레이어를 제외한 다른 엔티티의 경우.
		penPlayerEntity->Read_net_Damage(pPack->charType, &(*istr));
	}	

	// 길드 전쟁중 포탈을 타려고 하는데 공격을 받으면 취소 
	CUIManager::getSingleton()->GetGuildWarPortal()->PortalCancel();
}

IMPLEMENT_MSG_UPDATE(StatusAPetSmall)
{
	UpdateClient::attackPetSmallStatus* pPack = reinterpret_cast<UpdateClient::attackPetSmallStatus*>(istr->GetBuffer());

	// MSG_SUB_APET_INFO: 
	// 이 메세지 처리로, 실질적인 펫 정보와 UI의 펫 정보가 어긋나게 된다.(필요성이 없음)

	CUIManager* pUIManager = CUIManager::getSingleton();
	
	sPetItem_Info temPetInfo;

	temPetInfo.pet_index = pPack->charIndex;
	temPetInfo.pet_name = pPack->charName;
	temPetInfo.pet_level = pPack->level;
	temPetInfo.pet_str = pPack->str;
	temPetInfo.pet_con = pPack->con;
	temPetInfo.pet_dex = pPack->dex;
	temPetInfo.pet_int = pPack->intl;
	temPetInfo.pet_accexp = pPack->accExp;
	if (pPack->coolTime > 0)
	{
		temPetInfo.pet_cooltime = (unsigned int)_pTimer->GetLerpedCurrentTick() + pPack->coolTime;
	}
	else
	{
		temPetInfo.pet_cooltime = 0;
	}
	pUIManager->GetWildPetInfo()->AddWildPetInfo(temPetInfo);

}

IMPLEMENT_MSG_UPDATE(UpdateLevel)
{
	UpdateClient::LevelupInfo* pPack = reinterpret_cast<UpdateClient::LevelupInfo*>(istr->GetBuffer());

	_pNetwork->MyCharacterInfo.level = pPack->level;

	CTString strMessage;
	strMessage.PrintF(_S(6225, "축하합니다. [%d]레벨이 되었습니다."), pPack->level);

	CUIManager::getSingleton()->GetChattingUI()->AddSysMessage( strMessage, SYSMSG_USER);

	_UIAutoHelp->GetLvGuidString(pPack->level);
	_UIAutoHelp->LearnTheSkill(pPack->level);

	if (_UIAutoHelp->GetUpStatpoint() > 0)
	{
		strMessage.PrintF(_S(6212, "성장 포인트 %d개가 생성 되었습니다."), _UIAutoHelp->GetUpStatpoint());
		CUIManager::getSingleton()->GetChattingUI()->AddSysMessage( strMessage, SYSMSG_USER);
	}

}

IMPLEMENT_MSG_UPDATE(UpdateTargetHPInfo)
{
	UpdateClient::charHpInfo* pPack = reinterpret_cast<UpdateClient::charHpInfo*>(istr->GetBuffer());

	ObjInfo* pInfo = ObjInfo::getSingleton();
	
	if (pInfo->GetTargetServerIdx(eTARGET) == pPack->charIndex)
	{
		pInfo->SetTargetHealth(pPack->hp, pPack->maxHp);
	}
}

IMPLEMENT_MSG_UPDATE(UpdateCharHitType)
{
	UpdateClient::holyWaterState* pRecv = reinterpret_cast<UpdateClient::holyWaterState*>(istr->GetBuffer());

	if (pRecv == NULL)
		return;

	int nCharIndex = pRecv->charIndex;

	// 0 : 일반, 1 : 힘의성수, 2 : 신비한 성수
	int nType = 0;

	switch(pRecv->holyWaterItemIndex)
	{
	case DEF_HOLYWATER_STRONG:
		nType = 1;
		break;
	case DEF_HOLYWATER_MYSTERY:
		nType = 2;
		break;
	case DEF_HOLYWATER_DAMAGE:
		nType = 3;
		break;
	case DEF_HOLYWATER_SKILLDAMAGE:
		nType = 4;
		break;
	default:
		nType = 0;
		break;
	}

	if (_pNetwork->MyCharacterInfo.index == nCharIndex)
	{
		CEntity			*penPlEntity;
		penPlEntity = CEntity::GetPlayerEntity( 0 );
		
		_pNetwork->MyCharacterInfo.iHitEffectType = nType;
		_pNetwork->MyCharacterInfo.itemEffect.AddHolyWaterEffect(nType, &(penPlEntity->en_pmiModelInstance->m_tmSkaTagManager));
	}
	else
	{
		ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_CHARACTER, nCharIndex);

		if (pObject != NULL)
		{
			CCharacterTarget* pTarget = static_cast< CCharacterTarget* >(pObject);
			
			pTarget->cha_iHitType = nType;
			pTarget->cha_itemEffect.AddHolyWaterEffect(nType, &(pTarget->m_pEntity->en_pmiModelInstance->m_tmSkaTagManager));
		}
	}	
}

IMPLEMENT_MSG_UPDATE(updatePremiumCharInfo)
{
#ifdef PREMIUM_CHAR
	UpdateClient::premiumCharInfo* pRecv = reinterpret_cast<UpdateClient::premiumCharInfo*>(istr->GetBuffer());

	CPremiumChar* pChar = GAMEDATAMGR()->GetPremiumChar();
	
	if (pChar == NULL)
		return;

	pChar->updateInfo(pRecv->premiumType, pRecv->jumpCount, pRecv->expireTime);
#endif	//	PREMIUM_CHAR
}

IMPLEMENT_MSG_UPDATE(updatePremiumJumpCount)
{
#ifdef PREMIUM_CHAR
	UpdateClient::premiumCharJumpCount* pRecv = reinterpret_cast<UpdateClient::premiumCharJumpCount*>(istr->GetBuffer());

	CUIManager* pUImgr = CUIManager::getSingleton();
	CPremiumChar* pChar = GAMEDATAMGR()->GetPremiumChar();

	if (pChar == NULL)
		return;

	pChar->setCount(pRecv->jumpCount);

	pUImgr->GetPremiumChar()->updateAc_Jump();
	pUImgr->GetMessenger()->UpdateMemberMenu();
#endif	//	PREMIUM_CHAR
}

IMPLEMENT_MSG_UPDATE(updatePremiumCharEnd)
{
#ifdef PREMIUM_CHAR
	CPremiumChar* pChar = GAMEDATAMGR()->GetPremiumChar();

	if (pChar == NULL)
		return;

	pChar->Clear();
	UIMGR()->GetPremiumChar()->CloseUI();

	CTString strErrorMsg;
	strErrorMsg.PrintF(_S( 6326, "프리미엄 혜택의 사용기간이 만료 되었습니다."));
	_pNetwork->ClientSystemMessage(strErrorMsg, SYSMSG_ERROR);
#endif	//	PREMIUM_CHAR
}

IMPLEMENT_MSG_UPDATE(PremiumFlag)
{
#ifdef	PREMIUM_CHAR
	UpdateClient::changePremiumCharFlag* pRecv = reinterpret_cast<UpdateClient::changePremiumCharFlag*>(istr->GetBuffer());

	ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_CHARACTER, pRecv->charIndex);

	if (pObject == NULL)
		return;

	if (pRecv->flag == true)
		pObject->SetPremiumType(PREMIUM_CHAR_TYPE_FIRST);
	else
		pObject->SetPremiumType(PREMIUM_CHAR_TYPE_NONE);
#endif	// PREMIUM_CHAR
}

IMPLEMENT_MSG_UPDATE(updateAttrPc)
{
	// 속성 업데이트.
	UpdateClient::AttrItemUse* pRecv = reinterpret_cast<UpdateClient::AttrItemUse*>(istr->GetBuffer());

	CTString strAtt;
	int nMsgCode = 0;

	UBYTE ubAttr = GET_AT_VAR(pRecv->attr);

	switch(pRecv->state_type)
	{
	case 1:
		nMsgCode = 6351; // %s의 공격 속성으로 결정되었습니다.
		break;
	case 2:
		nMsgCode = 6352; // %s의 방어 속성으로 결정되었습니다.
		break;
	default:
		return;
	}

	strAtt = UtilHelp::getSingleton()->GetAttributeString(ubAttr);

	CTString strErrorMsg;
	strErrorMsg.PrintF(_S( nMsgCode, "%s 속성 변경 "), strAtt);
	_pNetwork->ClientSystemMessage(strErrorMsg, SYSMSG_ERROR);
}

IMPLEMENT_MSG_UPDATE(updateAttrNpc)
{
	// 속성 업데이트.
	UpdateClient::AttrNpcStateInfo* pRecv = reinterpret_cast<UpdateClient::AttrNpcStateInfo*>(istr->GetBuffer());
	
	switch(pRecv->state_type)
	{
	case 1: // 공격 속성 변경
	case 2: // 방어 속성 변경
		break;
	default:
		return;
	}

	UBYTE ubAttr = GET_AT_VAR(pRecv->attr);
	UBYTE ubAttrLv = GET_AT_LV(pRecv->attr);

	if (ubAttr <= 0 || ubAttr >= ATTRIBUTE_ATT_MAX)
		ubAttrLv = 1;

	ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_MOB, pRecv->npc_index);

	if (pObject != NULL)
	{
		pObject->SetAttribute(pRecv->state_type, ubAttr, ubAttrLv);
	}

	if (INFO()->GetTargetEntity(eTARGET) != NULL && INFO()->GetTargetServerIdx(eTARGET) == pRecv->npc_index)
	{
		UIMGR()->GetTargetInfoUI()->UpdateAttrIcon();
	}
}

IMPLEMENT_MSG_UPDATE(updateNewsGmNoti)
{
	GAMEDATAMGR()->GetNews()->UpdateRevision(istr);
}

IMPLEMENT_MSG_UPDATE(updateNewsWebNoti)
{
	GAMEDATAMGR()->GetNews()->UpdateWebNoti(istr);
}

IMPLEMENT_MSG_UPDATE(updateNewsWebClose)
{
	GAMEDATAMGR()->GetNews()->UpdateWebClose(istr);
}

IMPLEMENT_MSG_UPDATE(updateNewsWebClear)
{
	GAMEDATAMGR()->GetNews()->UpdateWebClear(istr);
}

IMPLEMENT_MSG_UPDATE(updateItemCompose)
{
	GAMEDATAMGR()->GetItemCompose()->UpdateItemComposeMsg(istr);
}

IMPLEMENT_MSG_UPDATE(updateRelicGpsSysMsg)
{
	GAMEDATAMGR()->GetItemCompose()->UpdateGPSRelicMsg(istr);
}

IMPLEMENT_MSG_UPDATE(updateGuildRecall)
{
	CUIManager* pUIMgr = UIMGR();
	pUIMgr->CloseMessageBox( MSGCMD_GUILD_RECALL );

	CTString	strMessage;
	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( _S( 865, "길드" ), UMBS_OKCANCEL, UI_GUILD, MSGCMD_GUILD_RECALL );	
	strMessage.PrintF( _S( 6436, "길드장께서 전 길드원을 길드방으로 소환요청 하였습니다. 소환에 응하시겠습니까?" ) );
	MsgBoxInfo.AddString( strMessage );

	pUIMgr->CreateMessageBox( MsgBoxInfo );		
}