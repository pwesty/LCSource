#include "stdhdrs.h"

#include "Server.h"
#include "Battle.h"
#include "WarCastle.h"
#include "CmdMsg.h"
#include "Exp.h"
#include "Log.h"
#include "doFunc.h"
#include "Artifact_Manager.h"
#include "../ShareLib/packetType/ptype_old_do_item.h"

#define PVP_TOKEN_ID 11023

void ProcRVR(CPC* df, CCharacter* of);
void ProcDeadQuestProc(CPC* df, CPC* of);

void ProcDead(CPC* df, CCharacter* of)
{
	CDratanCastle * pCastle = CDratanCastle::CreateInstance();
	if (df != NULL)
	{
		pCastle->CheckRespond(df);
	}

	const char* strOFType = "UNKNOWN";
	const char* strOFName = "UNKNOWN";
	int strOFIndex = 0;

	CPC*		opc				= NULL;
	CNPC*		onpc			= NULL;
	CPet*		opet			= NULL;
	CElemental*	oelemental		= NULL;
	CAPet*		oapet			= NULL;

	if( IS_NPC(of) && TO_NPC(of)->Check_MobFlag(STATE_MONSTER_MERCENARY) && TO_NPC(of)->GetOwner() )
	{
		TO_NPC(of)->GetOwner()->SetSummonOwners_target(NULL);
	}

	switch (of->m_type)
	{
	case MSG_CHAR_PC:
		opc = TO_PC(of);
		strOFType = "PC";
		strOFName = opc->GetName();
		strOFIndex = opc->m_index;
		break;

	case MSG_CHAR_NPC:
		onpc = TO_NPC(of);
		strOFType = "NPC";
		strOFName = onpc->m_name;
		strOFIndex = onpc->m_idNum;
		break;

	case MSG_CHAR_PET:
		opet = TO_PET(of);
		opc = opet->GetOwner();
		if (opc == NULL)
			return ;
		strOFType = "PET";
		strOFName = opc->GetName();
		strOFIndex = opc->m_index;
		break;

	case MSG_CHAR_ELEMENTAL:
		oelemental = TO_ELEMENTAL(of);
		opc = oelemental->GetOwner();
		if (opc == NULL)
			return ;
		strOFType = "ELEMENTAL";
		strOFName = opc->GetName();
		strOFIndex = opc->m_index;
		break;
	case MSG_CHAR_APET:
		oapet = TO_APET(of);
		opc = oapet->GetOwner();
		if (opc == NULL)
			return ;
		strOFType = "APET";
		strOFName = opc->GetName();
		strOFIndex = opc->m_index;
		break;

	default:
		return ;
	}

	if( opc )
		opc->SetSummonOwners_target(NULL);

	// NPC에 의한 사망시 사망 패널티는 기본으로 true, PC에게 사망시 사망 패널티는 기본으로 false
	// * bPKPenalty변수는 pk 패널티를 주는것 뿐만 아니라 성향회복에도 관계되므로 성향 회복이나 패널티등 어느것에라도 걸리면 true

	bool bPvP = (opc) ? true : false;
	bool bPKPenalty = (opc) ? IsPK(opc, df) : false;
	bool bDeadPenalty = (bPvP) ? false : true;
	// 아래 boolean변수는 선언과 값대입이 따로 이루어져야 합니다.
	// bool bRestorePKOfDefensePC = true; 이런식으로 선언을 하면 UPDATE1106에서는 사용하지 않는 변수로 warning을 출력합니다.

	//소환NPC에게 죽었을 경우 처리 (EX 트랩) - 트랩의 경우에 타겟을 NPC로 사용하고 있음...ㅡㅡ;;
	if(IS_NPC(of))
	{
		CNPC* npc = TO_NPC(of);
		if(npc->m_owner > 0)
		{
			bPvP = false;
			bPKPenalty = true;
			bDeadPenalty = false;
		}
	}

	bool bRestorePKOfDefensePC;
	bRestorePKOfDefensePC = true;

	// 변신 해제
	if (df->IsSetPlayerState(PLAYER_STATE_CHANGE))
		df->CancelChange();

	if (opc)
	{
#ifdef FREE_PK_SYSTEM
		if( !gserver->m_bFreePk )
		{
#endif // FREE_PK_SYSTEM

#ifdef MAL_DISABLE_PKPENALTY
			if( gserver->m_bDisablePKPaenalty )
				bDeadPenalty = true;
			else if( !gserver->m_bDisablePKPaenalty )
			{
#endif // MAL_DISABLE_PKPENALTY
				// df가 pk모드 이거나 카오면 둘다 트루
				if (df->IsSetPlayerState(PLAYER_STATE_PKMODE) || df->IsChaotic())
					bDeadPenalty = true;
#ifdef MAL_DISABLE_PKPENALTY
			}
#endif // MAL_DISABLE_PKPENALTY

#ifdef FREE_PK_SYSTEM
		}
#endif // FREE_PK_SYSTEM

		// 길드전
		if (opc->m_guildInfo && (opc->m_guildInfo->guild()->battleState() == GUILD_BATTLE_STATE_ING) &&
				df->m_guildInfo && (df->m_guildInfo->guild()->battleState() == GUILD_BATTLE_STATE_ING))
		{
			if (opc->m_guildInfo->guild()->battleIndex() == df->m_guildInfo->guild()->index() &&
					df->m_guildInfo->guild()->battleIndex() == opc->m_guildInfo->guild()->index())
			{
				bDeadPenalty = false;

				int killCount = opc->m_guildInfo->guild()->killCount();

				killCount++;

				if (gserver->isRunHelper())
				{
					CNetMsg::SP rmsg(new CNetMsg);
					HelperGuildBattleKillReqMsg(rmsg, opc->m_guildInfo->guild()->index(), df->m_guildInfo->guild()->index());
					SEND_Q(rmsg, gserver->m_helper);
				}
				else
				{
					GAMELOG << init("GUILD_BATTLE")
							<< "if( gserver->isRunHelper() ) false" << delim
							<< end;
					CNetMsg::SP rmsg(new CNetMsg);
					GuildErrorMsg(rmsg, MSG_GUILD_ERROR_GAMESERVER);
					SEND_Q(rmsg, opc->m_desc);
				}
			}
		}
	} // 공격자가 PC 또는 PC의 소유물일때

	// 공성 포인트 계산
	if (opc)
		CalcWarPoint(opc, df);
	else
		CalcWarPoint(of, df);

	// 공성 도중 사망은 패널티 없음
	CWarCastle* castle = CWarCastle::GetCastleObject(df->m_pZone->m_index);
#ifdef CHECK_CASTLE_AREA
	if (castle && castle->GetState() != WCSF_NORMAL && (df->GetMapAttr() & MATT_WAR || df->m_pZone->IsWarZone((int)df->m_pos.m_x, (int)df->m_pos.m_z)))
#else
	if (castle && castle->GetState() != WCSF_NORMAL && df->GetMapAttr() & MATT_WAR)
#endif // CHECK_CASTLE_AREA
	{
		DropWarCastleTokenDeadPC(df);
		bDeadPenalty = false;
	}

	/////////////////////////////////////////////
	// BANGWALL : 2005-07-18 오전 11:27:24
	// Comment : freepkzone 패널티 없음
	// 공격자와 방어자가 모두 freepkzone에 있으면 pkpenalty 없음
	if( of->GetMapAttr() == df->GetMapAttr() && of->GetMapAttr() & MATT_FREEPKZONE)
		bDeadPenalty = false;

	// PvP에서 PK 적용
	if (opc && bPvP && bPKPenalty)
		CalcPKPoint(opc, df, false);

	bool bDeadExpPenalty = true;

#ifdef FREE_PK_SYSTEM
	if( gserver->m_bFreePk )
	{
		if(!bDeadPenalty)
			bDeadExpPenalty = false;
	}
#endif // FREE_PK_SYSTEM

#if defined(LC_BILA)
	if (bPvP)
#ifdef MAL_DISABLE_PKPENALTY
		if( gserver->m_bDisablePKPaenalty )
		{
			bDeadExpPenalty = false;
		}
#endif // MAL_DISABLE_PKPENALTY
#endif

	// 퍼스널 던전은 무조건 패널티 없음
	if (df->m_pZone->IsPersonalDungeon())
		bDeadPenalty = false;

	// 경험치 하락
	LONGLONG nLoseExp = 0;
	LONGLONG nLoseSP = 0;

	// 하락된 경험치는 최근 것만 기억
	df->m_loseexp = 0;
	df->m_losesp = 0;

	if( df->m_skillPoint < 0 )
		df->m_skillPoint = 0;

	//수비자가 무소속인 경우만 사망 페널티 적용
	if (df->m_pZone->isRVRZone())
	{
		if(df->getSyndicateType() == 0)
			bDeadPenalty = true;
		else
			bDeadPenalty = false;
	}

	if (bDeadPenalty)
	{
		if (bDeadExpPenalty)
		{
			// 사망시 패널티
			if (df->m_level < 11)
			{
				/*				nLoseExp = (LONGLONG)(GetLevelupExp(df->m_level) * DEATH_PENALTY_EXP_1);
								nLoseSP = (LONGLONG)(df->m_skillPoint * DEATH_PENALTY_SP_1);*/
				nLoseExp = 0;
				nLoseSP = 0;
			}
			else if (df->m_level < 21)
			{
				/* 5% */
				/*				nLoseExp = (LONGLONG)(GetLevelupExp(df->m_level) * DEATH_PENALTY_EXP_2);
								nLoseSP = (LONGLONG)(df->m_skillPoint * DEATH_PENALTY_SP_2);*/
				nLoseExp = (LONGLONG)((GetLevelupExp(df->m_level) / 100) * 5);
				nLoseSP = (LONGLONG)((df->m_skillPoint / 100) * 5);
			}
			else if (df->m_level < 36)
			{
				/*				nLoseExp = (LONGLONG)(GetLevelupExp(df->m_level) * DEATH_PENALTY_EXP_3);
								nLoseSP = (LONGLONG)(df->m_skillPoint * DEATH_PENALTY_SP_3);*/
				nLoseExp = (LONGLONG)((GetLevelupExp(df->m_level) / 100) * 3);
				nLoseSP = (LONGLONG)((df->m_skillPoint / 100) * 3);
			}
			else
			{
				/*				nLoseExp = (LONGLONG)(GetLevelupExp(df->m_level) * DEATH_PENALTY_EXP_4);
								nLoseSP = (LONGLONG)(df->m_skillPoint * DEATH_PENALTY_SP_4);*/
				nLoseExp = (LONGLONG)((GetLevelupExp(df->m_level) / 100) * 2);
				nLoseSP = (LONGLONG)((df->m_skillPoint / 100) * 2);
			}

			// 경험의 결정 시리즈 적용
			switch (df->m_assist.m_avAddition.hcDeathExpPlus)
			{
			case 1:
				{
					nLoseExp -= 50000;
					if(nLoseExp < 0)
						nLoseExp = 0;
				}
				break;
			case 2:
				{
					nLoseExp -= 600000;
					if(nLoseExp < 0)
						nLoseExp = 0;
				}
				break;
			case 3:
				{
					nLoseExp /= 2;
				}
				break;
			default:
				break;
			}

			// 노력의 결정 적용
			if (df->m_assist.m_avAddition.hcDeathSPPlus)
				nLoseSP /= 2;
		}

#ifdef FREE_PK_SYSTEM
		if( !gserver->m_bFreePk )
		{
#endif // FREE_PK_SYSTEM

#ifdef MAL_DISABLE_PKPENALTY
			if( !gserver->m_bDisablePKPaenalty )
			{
#endif // MAL_DISABLE_PKPENALTY
				if (df->IsChaotic())
				{
#ifndef REFORM_PK_PENALTY_201108 // PK 패널티 리폼 :: 장비 잠금 상태 기능 삭제
// TODO : DELETE			bSaveLose = false;
					bool bseal = false;

					if (df->m_pkPenalty <= -130)
					{
						nLoseExp = nLoseExp * 225 / 100;
						bseal = (GetRandom(1, 100) <= 13) ? true : false;
					}
					else if (df->m_pkPenalty <= -100)
					{
						nLoseExp = nLoseExp * 200 / 100;
						bseal = (GetRandom(1, 100) <= 11) ? true : false;
					}
					else if (df->m_pkPenalty <=  -70)
					{
						nLoseExp = nLoseExp * 175 / 100;
						bseal = (GetRandom(1, 100) <=  9) ? true : false;
					}
					else if (df->m_pkPenalty <=  -40)
					{
						nLoseExp = nLoseExp * 150 / 100;
						bseal = (GetRandom(1, 100) <=  7) ? true : false;
					}
					else if (df->m_pkPenalty <=  -10)
					{
						nLoseExp = nLoseExp * 125 / 100;
						bseal = (GetRandom(1, 100) <=  5) ? true : false;
					}

					if (bseal)
					{
						CItem* table[MAX_WEARING];
						memset(table, 0, sizeof(CItem*) * MAX_WEARING);
						int i = 0, j = 0;
						while (i < MAX_WEARING)
						{
							if (df->m_wearInventory.wearItemInfo[i] && !(df->m_wearInventory.wearItemInfo[i]->getFlag() & FLAG_ITEM_SEALED))
							{
								// 장비에 따라 봉인 되는지 결정
								switch (i)
								{
								case WEARING_HELMET:
								case WEARING_ARMOR_UP:
								case WEARING_WEAPON:
								case WEARING_ARMOR_DOWN:
								case WEARING_SHIELD:
								case WEARING_GLOVE:
								case WEARING_BOOTS:
									if(!(df->m_wearInventory.wearItemInfo[i]->m_itemProto->getItemFlag() & ITEM_FLAG_COSTUME2))
										table[j] = df->m_wearInventory.wearItemInfo[i];
									j++;
									break;

								default:
									break;
								}
							}
							i++;
						}
						if (j)
						{
							i = GetRandom(0, j - 1);

							if (table[i])
							{
								table[i]->setFlag(table[i]->getFlag() | FLAG_ITEM_SEALED);

								{
									CNetMsg::SP rmsg(new CNetMsg);
									UpdateClient::makeUpdateItemFlag(rmsg, table[i]->tab(), table[i]->getInvenIndex(), table[i]->getFlag());
									SEND_Q(rmsg, df->m_desc);
								}

								{
									CNetMsg::SP rmsg(new CNetMsg);
									PKItemSealMsg(rmsg, table[i]);
									SEND_Q(rmsg, df->m_desc);
								}

								GAMELOG << init("ITEM SEAL" , df)
										<< "ITEM" << delim
										<< itemlog(table[i])
										<< end;
							}
						}
					}
#endif // REFORM_PK_PENALTY_201108 // PK 패널티 리폼 :: 장비 잠금 상태 기능 삭제
					if (nLoseExp < 0)
						nLoseExp = 0;
					if (nLoseSP < 0)
						nLoseSP = 0;

					// 060318 : bs : 공방 모두 카오PC PvP 시에는 죽은 사람 회복 없음
					//             : 죽은카오가 선공이면 회복 없음
					// 성향 회복

#ifndef REFORM_PK_PENALTY_201108 // PK 패널티 리폼 :: 카오 사망시 성향 회복 없음
					if (bRestorePKOfDefensePC)
					{
						if( !gserver->m_bNonPK )
							df->m_pkPenalty += 5;

						if (df->m_pkPenalty > 0)
							df->m_pkPenalty = 0;
					}

					{
						// 페널티 수치 변경 알리기
						CNetMsg::SP rmsg(new CNetMsg);
						CharStatusMsg(rmsg, df, 0);
						df->m_pArea->SendToCell(rmsg, df, false);
					}

					df->m_bChangeStatus = true;
					df->CalcStatus(true);

#endif // REFORM_PK_PENALTY_201108 // PK 패널티 리폼 :: 카오 사망시 성향 회복 없음
				}
				else
				{
					if (df->m_exp < nLoseExp)
						nLoseExp = df->m_exp;
				}
#ifdef MAL_DISABLE_PKPENALTY
			}
			else
			{
				if( df->m_exp < nLoseExp )
					nLoseExp = df->m_exp;
			}
#endif
#ifdef FREE_PK_SYSTEM
		}
		else
		{
			if( df->m_exp < nLoseExp )
				nLoseExp = df->m_exp;
		}
#endif // FREE_PK_SYSTEM


		if (bDeadExpPenalty)
		{
			if (nLoseExp < 0)
				nLoseExp = 0;
			if (nLoseSP < 0)
				nLoseSP = 0;
			if (df->m_skillPoint < nLoseSP)
				nLoseSP = df->m_skillPoint;
// 경험치 - 방지
			if ( df->m_exp < nLoseExp )
				nLoseExp = df->m_exp;

// 수정
			if(df->m_pZone->IsComboZone())
			{
				nLoseExp = nLoseExp / 10;
				nLoseSP = nLoseSP / 10;
			}
			df->m_exp -= nLoseExp;
			if (df->m_exp < 0)
				df->m_exp = 0;

			df->m_skillPoint -= nLoseSP;
			if (df->m_skillPoint < 0)
				df->m_skillPoint = 0;

			df->m_bChangeStatus = true;

// TODO : DELETE			if (bSaveLose)
// TODO : DELETE			{
			df->m_loseexp = nLoseExp;
			df->m_losesp = nLoseSP;
// TODO : DELETE			}
		}
		
		// Fixed death by npc dropping item
		if(IS_PC(of)) {
			CItem* pItem = gserver->m_itemProtoList.CreateItem(PVP_TOKEN_ID, -1, 0, 0, 1);
			if (pItem) {
				CNetMsg::SP rmsg(new CNetMsg);
				pItem->m_preferenceIndex = of->m_index;
				df->m_pArea->DropItem(pItem, df);
				ItemDropMsg(rmsg, df, pItem);
				pItem->m_pArea->SendToCell(rmsg, df, true);
			}
		}
	} // 사망 패널티 적용

	// Accessory 내구도
	int i;
	for (i = WEARING_ACCESSORY1; i <= WEARING_ACCESSORY3; i++)
	{
		if (!df->m_wearInventory.wearItemInfo[i] || df->m_wearInventory.wearItemInfo[i]->m_itemProto->getItemMaxUse() == -1)
			continue;

		df->m_wearInventory.wearItemInfo[i]->setUsed(df->m_wearInventory.wearItemInfo[i]->getUsed() - ACCESSORY_USED_DEATH);

		// 악세사리 소멸
		if (df->m_wearInventory.wearItemInfo[i]->getUsed() <= 0)
		{
			df->m_wearInventory.wearItemInfo[i]->setUsed(0);
			df->CalcStatus(true);
		}
	}

	// 보조효과 리셋
	df->m_assist.ClearAssist(true, false, true, true, false);

	// 워프중이면 취소
	if (df->IsSetPlayerState(PLAYER_STATE_WARP))
	{
		df->m_reqWarpTime = 0;
		df->m_reqWarpType = -1;
		df->m_reqWarpData = -1;

		df->m_reqWarpTime_skill = -1;
		df->m_reqWarpType_skill = -1;
		df->m_reqWarpData_skill = -1;
	}

	df->ResetPlayerState(PLAYER_STATE_SITDOWN | PLAYER_STATE_MOVING | PLAYER_STATE_WARP | PLAYER_STATE_PKMODE | PLAYER_STATE_DARKNESS);

	CPet* pet = df->GetPet();
	if (pet)
	{
		if (pet->IsMountType())
		{
			if(df->m_pZone != NULL && df->m_pZone->m_bCanMountPet == true)
			{
				// 사망 설정
				pet->SetRemainRebirthTime();
			}
		}

		{
			// 펫 상태 보냄
			CNetMsg::SP rmsg(new CNetMsg);
			ExPetStatusMsg(rmsg, pet);
			SEND_Q(rmsg, df->m_desc);
		}
	}

#ifdef LC_USA
	CAPet* apet = df->GetAPet();
	if(apet)
	{
		{
			// 펫 상태 보냄
			CNetMsg::SP rmsg(new CNetMsg);
			ExAPetStatusMsg(rmsg, apet);
			SEND_Q(rmsg, df->m_desc);
		}
	}
#endif // LC_USA

	// 소환 취소
	while (df->m_elementalList)
		df->UnsummonElemental(df->m_elementalList);
	// 강신 취소
	if (df->m_evocationIndex != EVOCATION_NONE)
		df->Unevocation();
	// 강신 시간 초기화
	df->m_pulseEvocation[0] = 0;
	df->m_pulseEvocation[1] = 0;

#ifdef GER_LOG
	GAMELOGGEM << init( 0, "CHAR_DEATH")
			   << LOG_VAL("account-id", df->m_desc->m_idname ) << blank
			   << LOG_VAL("character-id", df->m_desc->m_pChar->m_name ) << blank
			   << LOG_VAL("zone-id", df->m_desc->m_pChar->m_pZone->m_index ) << blank
			   << LOG_VAL("from-id", strOFType ) << blank
			   << LOG_VAL("opponent-id", strOFIndex ) << blank
			   << LOG_VAL("longitude", GET_X(df) ) << blank
			   << LOG_VAL("latitude", GET_Z(df) ) << blank
			   << endGer;

	if ( IS_PC(of) )
	{
		CPC *user = TO_PC(of);
		GAMELOGGEM << init( 0, "CHAR_VICTORY")
				   << LOG_VAL("account-id", user->m_desc->m_idname ) << blank
				   << LOG_VAL("character-id", user->m_desc->m_pChar->m_name ) << blank
				   << LOG_VAL("zone-id", user->m_desc->m_pChar->m_pZone->m_index ) << blank
				   /*<< LOG_VAL("from-id", strOFType ) << blank*/
				   << LOG_VAL("opponent-id", df->m_desc->m_idname ) << blank
				   << LOG_VAL("longitude", GET_X(user) ) << blank
				   << LOG_VAL("latitude", GET_Z(user) ) << blank
				   << endGer;
	}
#endif // GER_LOG
	// 로그
	GAMELOG << init("CHAR_DEATH", df)
			<< "BY" << delim
			<< strOFType << delim
			<< strOFName << delim
			<< strOFIndex << delim
			<< "LOSE EXP" << delim
			<< nLoseExp << delim
			<< "CUR EXP" << delim
			<< df->m_exp << delim
			<< "LOSE SP" << delim
			<< nLoseSP << delim
			<< "CUR SP" << delim
			<< df->m_skillPoint
			<< "POS_X" << delim
			<< GET_X(df) << delim
			<< "POS_Z" << delim
			<< GET_Z(df) << delim
			<< "YLAYER" << delim
			<< GET_YLAYER(df) << delim
			<< "HEIGHT" << delim
			<< GET_H(df)
			<< end;

	DelAttackList(df);

	// 정당방위 해제
	DelRaList(df);

	if (pCastle != NULL)
	{
		if (df->GetJoinFlag(ZONE_DRATAN) != WCJF_NONE
				&& pCastle->GetState() != WCSF_NORMAL
				&& (df->GetMapAttr() & MATT_WAR || df->m_pZone->IsWarZone((int)df->m_pos.m_x, (int)df->m_pos.m_z) ) )
		{
			// 공성중에 공성참가
			int wait_time = -1;
			switch(df->GetJoinFlag(ZONE_DRATAN))
			{
			case WCJF_ATTACK_GUILD:
				// 부활진기가 있으면 부활대기 시간 20초, 없으면 60초
				wait_time = 60;
				if (df->m_guildInfo != NULL
						&& df->m_guildInfo->guild() != NULL)
				{
					for(int i=0; i<7; i++)
					{
						if (df->m_guildInfo->guild()->index() == pCastle->m_nRebrithGuild[i])
						{
							wait_time = 20;
							break;
						}
					}
				}
				break;
			case WCJF_OWNER:
			case WCJF_DEFENSE_GUILD:
				//  부활대기 시간 60초 - 워프타워개수*10
				int count = 0;
				for(int i=0; i<5; i++)
				{
					if(pCastle->m_pWarpNPC[i] != NULL
							&& DEAD(pCastle->m_pWarpNPC[i]) == false)
					{
						count++;
					}
				}

				wait_time = 60 - count*10;
				break;
			}

			if (wait_time > 0)
			{
				CNetMsg::SP rmsg(new CNetMsg);
				WaitTimeMsg(rmsg, wait_time);
				SEND_Q(rmsg, df->m_desc);
			}
		}
	}

#ifdef EXTREME_CUBE
	if(gserver->m_extremeCube.IsGuildCubeTime())
	{
		if(df->m_guildInfo && df->m_guildInfo->guild())
		{
			if(df->m_pZone != NULL && df->m_guildInfo->guild()->m_cubeUniqueIdx >= 0 && df->m_pZone->IsExtremeCube())
			{
				CCubeMemList* memlist = gserver->m_extremeCube.FindMemList(df->m_guildInfo->guild()->m_cubeUniqueIdx);
				if(memlist)
				{
					memlist->DelPC(df);

					if(opc && opc->m_guildInfo && opc->m_guildInfo->guild())
					{
						CCubeMemList* opcMemList = gserver->m_extremeCube.FindMemList(opc->m_guildInfo->guild());
						if(opcMemList)
						{
							time_t lastCubePoint;
							int point;
							time(&lastCubePoint);

							{
								CNetMsg::SP rmsg(new CNetMsg);
								HelperAddCubePointMsg(rmsg, opc->m_guildInfo->guild()->index(), df->m_level * 10, lastCubePoint);
								SEND_Q(rmsg, gserver->m_helper);
							}

							{
								// 개인 큐브포인트 획득
								point = opcMemList->GetPersonalCubePoint(opc, df->m_level);
								CNetMsg::SP rmsg(new CNetMsg);
								HelperAddCubePointPersonalMsg(rmsg, opc->m_index, point, lastCubePoint);
								SEND_Q(rmsg, gserver->m_helper);
							}
						}
					}
				}
			}
		}
	}
	else
	{
		if(df->m_party)
		{
			if(df->m_pZone != NULL && df->m_party->m_cubeUniqueIdx >= 0 && df->m_pZone->IsExtremeCube())
			{
				CCubeMemList* memlist = gserver->m_extremeCube.FindMemList(df->m_party->m_cubeUniqueIdx);
				if(memlist)
				{
					memlist->DelPC(df);
					// 개인 큐브포인트 획득
					if(opc && opc->m_party)
					{
						CCubeMemList* opcMemList = gserver->m_extremeCube.FindMemList(opc->m_party);
						if(opcMemList)
						{
							int point;
							time_t lastCubePoint;

							time(&lastCubePoint);
							point = opcMemList->GetPersonalCubePoint(opc, df->m_level);
							CNetMsg::SP rmsg(new CNetMsg);
							HelperAddCubePointPersonalMsg(rmsg, opc->m_index, point, lastCubePoint);
							SEND_Q(rmsg, gserver->m_helper);
						}
					}
				}
			}
		}
	}
#endif // EXTREME_CUBE

	if(df && df->m_pZone->IsWarGroundZone())
	{
		if(!opc && onpc)
		{
			if(onpc->Check_MobFlag(STATE_MONSTER_TRAP) || onpc->Check_MobFlag(STATE_MONSTER_PARASITE))
			{
				opc = onpc->GetOwner();
			}
		}
		if(opc)
			GAMELOG << init("ROYAL RUMBLE DEAD PC", df) << "ATTACKER" << delim << opc->m_nick << delim << opc->m_index << end;
		CWaitPlayer* p = NULL;
		p = gserver->m_RoyalRumble.m_WaitPlayerList.GetNode(df->m_index);
		if(p)
		{
			int leveltype = p->GetLevelType();
			int leftcount = 0;

			CWaitPlayer* player = NULL;
			CWaitPlayer* playern = NULL;
			playern = gserver->m_RoyalRumble.m_WaitPlayerList.GetHead();
			while((player = playern))
			{
				playern = playern->GetNext();
				if( player->GetLevelType() == leveltype &&
						player->GetCheckIn() == true )
					leftcount++;
			}
			leftcount -= 2;

			{
				CNetMsg::SP rmsg(new CNetMsg);
				RoyalRumbleLeftCount(rmsg, leftcount);
				CNetMsg::SP killmsg(new CNetMsg);
				if(opc)
					RoyalRumbleKillPlayer(killmsg, opc, df);
				switch(leveltype)
				{
				case LEVEL_TYPE_ROOKIE:
					{
						gserver->m_RoyalRumble.m_pRookieArea->SendToAllClient(rmsg);
						gserver->m_RoyalRumble.m_pRookieArea->SendToAllClient(killmsg);
					}
					break;
				case LEVEL_TYPE_SENIOR:
					{
						gserver->m_RoyalRumble.m_pSeniorArea->SendToAllClient(rmsg);
						gserver->m_RoyalRumble.m_pSeniorArea->SendToAllClient(killmsg);
					}
					break;
				case LEVEL_TYPE_MASTER:
					{
						gserver->m_RoyalRumble.m_pMasterArea->SendToAllClient(rmsg);
						gserver->m_RoyalRumble.m_pMasterArea->SendToAllClient(killmsg);
					}
					break;
				default:
					break;
				}
			}
		}
		gserver->m_RoyalRumble.m_WaitPlayerList.DelNode(df->m_index);
	}
	if(opc && opc->m_pZone->IsWarGroundZone())
	{
		// 전장포인트 1포인트 지급
		opc->AddWarGroundPoint(1);

		// 전장 kill수 1 증가
		opc->AddKillCount(1);
	}

	ProcDeadQuestProc(df, opc);

	// rvr 룰 적용
	if (df->m_pZone->isRVRZone())
	{
		ProcRVR(df, of);
	}

	if( ArtifactManager::instance()->isOwnerPC(df->m_index) == true )
	{
		if(IS_PC(of))
		{
			//pvp를 당했는데 수비자가 유물을 갖고 있는 상태라면 공격자에게 아이템 양도
			ArtifactManager::instance()->hunt(df, TO_PC(of));
		}
		else
		{
			//이외에 죽임을 당했다면 아이템 서버로 반납
			ArtifactManager::instance()->dead(df);
		}
	}

#ifdef HARDCORE_SERVER
	if (gserver->m_hardcore_flag_in_gameserver)
	{
		switch (df->m_pZone->m_index )
		{
		case ZONE_START:
			{
				if (df->GetJoinFlag(df->m_pZone->m_index) == WCJF_NONE)	// 공성중이 아니고
				{
					if ( !(df->GetMapAttr() & MATT_FREEPKZONE) )
					{
						df->m_desc->Make_1_Level();
					}
				}
			}
			break;

		case ZONE_FREE_PK_DUNGEON:
		case ZONE_PK_TOURNAMENT:
		case ZONE_ROYAL_RUMBLE:
		case ZONE_RVR:
			break;

		default:
			{
				if (df->GetJoinFlag(df->m_pZone->m_index) == WCJF_NONE)	// 공성중이 아니면
				{
					df->m_desc->Make_1_Level();
				}
			}
			break;
		} // end if
	}
#endif
}
//

void ProcRVR(CPC* df, CCharacter* of)
{
	CPC* oPC;
	CNPC* oNPC;
	if(IS_PC(of))
	{
		oPC = TO_PC(of);

		if (!df->isSyndicate() && !oPC->isSyndicate())
		{
			return;
		}

		if (df->isSyndicate() && !oPC->isSyndicate())
		{
			LONGLONG point = 1;

			df->m_syndicateManager.decreaseSyndicatePoint(point);
			return;
		}

		if (!df->isSyndicate() && oPC->isSyndicate())
		{
			LONGLONG point = 1;
			oPC->m_syndicateManager.increaseSyndicatePoint(point);
			return;
		}

		if (df->isSyndicate() && oPC->isSyndicate())
		{
			LONGLONG dfPoint = 0;
			LONGLONG attPoint = 0;
			int itemIndex = 0;

			//
			int jewelKailuxPoint		= SyndicateInfoDataManager::instance()->getJewelPoint(SYNDICATE::eSYNDICATE_KAILUX);
			int jewelDealerMoonPoint	= SyndicateInfoDataManager::instance()->getJewelPoint(SYNDICATE::eSYNDICATE_DEALERMOON);

			// 카이룩스의 보석량이 딜라문보다 적다
			if (jewelKailuxPoint < jewelDealerMoonPoint)
			{
				//공격자가 보석량이 작은 결사대 일 경우
				if (oPC->getSyndicateType() == SYNDICATE::eSYNDICATE_KAILUX)
				{
					// 공격자의 보석량이 적고 공격자의 기여도가 높다
					if (df->m_syndicateManager.getSyndicatePoint(df->getSyndicateType()) < oPC->m_syndicateManager.getSyndicatePoint(oPC->getSyndicateType()))
					{
						attPoint = 2;
						dfPoint = 5;
						itemIndex = 10129;
					}
					else if(df->m_syndicateManager.getSyndicatePoint(df->getSyndicateType()) == oPC->m_syndicateManager.getSyndicatePoint(oPC->getSyndicateType()))
					{
						// 공격자의 보석량이 적고 기여도가 같다
						attPoint = 3;
						dfPoint = 3;
						itemIndex = 10130;
					}
					else
					{
						// 공격자의 보석량이 적고 기여도가 낮다
						attPoint = 5;
						dfPoint = 2;
						itemIndex = 10130;
					}
				}
				//공격자가 보석량이 큰 결사대 일 경우
				else if (oPC->getSyndicateType() == SYNDICATE::eSYNDICATE_DEALERMOON)
				{
					if (df->m_syndicateManager.getSyndicatePoint(df->getSyndicateType()) < oPC->m_syndicateManager.getSyndicatePoint(oPC->getSyndicateType()))
					{
						// 공격자의 보석량이 많고 기여도가 높다
						attPoint = 1;
						dfPoint = 1;
						itemIndex = 10129;
					}
					else if(df->m_syndicateManager.getSyndicatePoint(df->getSyndicateType()) == oPC->m_syndicateManager.getSyndicatePoint(oPC->getSyndicateType()))
					{
						// 공격자의 보석략이 많고 기여도가 같다
						attPoint = 1;
						dfPoint = 1;
						itemIndex = 10130;
					}
					else
					{
						// 공격자의 보석략이 많고 기여도가 작다
						attPoint = 2;
						dfPoint = 1;
						itemIndex = 10130;
					}
				}
			}
			else
			{
				if (oPC->getSyndicateType() == SYNDICATE::eSYNDICATE_KAILUX)
				{
					// 공격자의 보석량이 많고 공격자의 기여도가 높다
					// 사망자의 보석량이 적고 기여도가 작다
					if (df->m_syndicateManager.getSyndicatePoint(df->getSyndicateType()) < oPC->m_syndicateManager.getSyndicatePoint(oPC->getSyndicateType()))
					{
						attPoint = 1;
						dfPoint = 1;
						itemIndex = 10129;
					}
					else if(df->m_syndicateManager.getSyndicatePoint(df->getSyndicateType()) == oPC->m_syndicateManager.getSyndicatePoint(oPC->getSyndicateType()))
					{
						// 공격자의 보석량이 많고고 기여도가 같다
						attPoint = 1;
						dfPoint = 1;
						itemIndex = 10130;
					}
					else
					{
						// 공격자의 보석량이 많고 기여도가 낮다
						attPoint = 2;
						dfPoint = 1;
						itemIndex = 10130;
					}
				}
				else if (oPC->getSyndicateType() == SYNDICATE::eSYNDICATE_DEALERMOON)
				{
					if (df->m_syndicateManager.getSyndicatePoint(df->getSyndicateType()) < oPC->m_syndicateManager.getSyndicatePoint(oPC->getSyndicateType()))
					{
						// 공격자의 보석량이 적고 기여도가 높다
						attPoint = 2;
						dfPoint = 5;
						itemIndex = 10129;
					}
					else if(df->m_syndicateManager.getSyndicatePoint(df->getSyndicateType()) == oPC->m_syndicateManager.getSyndicatePoint(oPC->getSyndicateType()))
					{
						// 공격자의 보석량이 적고 기여도가 같다
						attPoint = 3;
						dfPoint = 3;
						itemIndex = 10130;
					}
					else
					{
						// 공격자의 보석량이 적고 기여도가 작다
						attPoint = 5;
						dfPoint = 2;
						itemIndex = 10130;
					}
				}
			}
			//기여도 수정
			df->m_syndicateManager.decreaseSyndicatePoint(dfPoint);
			oPC->m_syndicateManager.increaseSyndicatePoint(attPoint);

			//창조의 보석 드랍
			if(df->m_syndicateManager.isKing())
			{
				itemIndex = 10131;
			}

			CItem* pItem = gserver->m_itemProtoList.CreateItem(itemIndex, -1, 0, 0, 1);
			pItem = df->m_pArea->DropItem(pItem, (CCharacter*)df);

			if(pItem && pItem->m_pArea)
			{
				pItem->m_preferenceIndex = oPC->m_index;
				CNetMsg::SP rmsg(new CNetMsg);
				ItemDropMsg(rmsg, df, pItem);
				pItem->m_pArea->SendToCell(rmsg, GET_YLAYER(pItem), pItem->m_cellX, pItem->m_cellZ);
				GAMELOG << init("Syndicate Jewel Drop.", df) << "ITEM" << delim << itemlog(pItem) << end;
			}

			switch(itemIndex)
			{
			case 10129:
				df->m_syndicateManager.syndicateDelJewel(1);
				break;
			case 10130:
				df->m_syndicateManager.syndicateDelJewel(5);
				break;
			case 10131:
				df->m_syndicateManager.syndicateDelJewel(30);
				break;
			}
		}

		// 로그 추가
		oPC->m_syndicateManager.historyManager_.battleWinHistory(oPC->getSyndicateType(), df->getSyndicateType(), df->GetName());
		df->m_syndicateManager.historyManager_.battleLoseHistory(df->getSyndicateType(), oPC->getSyndicateType(), oPC->GetName());

		// 히스토리 전달
		oPC->m_syndicateManager.historyManager_.sendHistory(oPC->getSyndicateType());
		df->m_syndicateManager.historyManager_.sendHistory(df->getSyndicateType());
	}
	else if(IS_NPC(of))
	{
		oNPC = TO_NPC(of);

		int jewelKailuxPoint		= SyndicateInfoDataManager::instance()->getJewelPoint(SYNDICATE::eSYNDICATE_KAILUX);
		int jewelDealerMoonPoint	= SyndicateInfoDataManager::instance()->getJewelPoint(SYNDICATE::eSYNDICATE_DEALERMOON);

		if(jewelKailuxPoint > jewelDealerMoonPoint)
		{
			if(df->getSyndicateType() == SYNDICATE::eSYNDICATE_KAILUX)
			{
				df->m_syndicateManager.decreaseSyndicatePoint(2);
			}
			else
			{
				df->m_syndicateManager.decreaseSyndicatePoint(1);
			}
		}
		else
		{
			if(df->getSyndicateType() == SYNDICATE::eSYNDICATE_KAILUX)
			{
				df->m_syndicateManager.decreaseSyndicatePoint(1);
			}
			else
			{
				df->m_syndicateManager.decreaseSyndicatePoint(2);
			}
		}
	}
}

void ProcDeadQuestProc(CPC* dpc, CPC* opc)
{
	if(opc == NULL)
		return;

	if(!opc->IsParty()) // 파티가 없으면 나만 퀘스트 업데이트
	{
		CQuest* pQuest = NULL;
		CQuest* pQuestNext = opc->m_questList.GetNextQuest(NULL, QUEST_STATE_RUN);
		while((pQuest = pQuestNext))
		{
			pQuestNext = opc->m_questList.GetNextQuest(pQuestNext, QUEST_STATE_RUN);
			switch(pQuest->GetQuestType0())
			{
			case QTYPE_KIND_REPEAT:
			case QTYPE_KIND_COLLECTION:
			case QTYPE_KIND_DEFEAT:
			case QTYPE_KIND_SAVE:
				{
					if(pQuest->GetPartyScale() == QTYPE_SCALE_PERSONAL || pQuest->GetPartyScale() == QTYPE_SCALE_PARTY)
						pQuest->QuestUpdateData(opc, dpc);
				}
				break;
			default:
				break;
			}
		}
		return ;
	}
	else
	{
		int i;
		CPartyMember* pPartyMember = NULL;
		CPC* pMember = NULL;
		for(i = 0; i < MAX_PARTY_MEMBER; i++)
		{
			pPartyMember = opc->m_party->GetMemberByListIndex(i);
			if(pPartyMember && pPartyMember->GetMemberPCPtr())
			{
				pMember = pPartyMember->GetMemberPCPtr();
				if(opc->m_pArea->FindCharInCell(opc, pMember->m_index, MSG_CHAR_PC))
				{
					CQuest* pQuest = NULL;
					CQuest* pQuestNext = pMember->m_questList.GetNextQuest(NULL, QUEST_STATE_RUN);
					while((pQuest = pQuestNext))
					{
						pQuestNext = pMember->m_questList.GetNextQuest(pQuestNext, QUEST_STATE_RUN);
						switch(pQuest->GetQuestType0())
						{
						case QTYPE_KIND_REPEAT:
						case QTYPE_KIND_COLLECTION:
						case QTYPE_KIND_DEFEAT:
						case QTYPE_KIND_SAVE:
							{
								if(pQuest->GetPartyScale() == QTYPE_SCALE_PERSONAL || pQuest->GetPartyScale() == QTYPE_SCALE_PARTY)
									pQuest->QuestUpdateData(pMember, dpc);
							}
							break;
						default:
							break;
						}
					}
				}
			}
		}
	}
}
