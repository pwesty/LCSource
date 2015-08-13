#include "stdhdrs.h"

#include "Server.h"
#include "Battle.h"
#include "WarCastle.h"
#include "CmdMsg.h"
#include "Log.h"
#include "doFunc.h"

void ProcDead_PD2(CNPC* df);
void ProcDead_RVR(CPC* op, CNPC* df);
#ifdef PARTY_QUEST_ITEM_BUG_
void ProcDeadQuestProc(CPC * opc,CNPC * df, int partyScale); //[2009/12/28 derek] 막타 친넘과 데미지 많이 준넘이 다른 넘일 경우 퀘스트 검색을 하지 않아서 퀘스트 처리부분을 함수로 뺐다.
#endif

void ProcDead(CNPC* df, CCharacter* of)
{
	CPC*		opc				= NULL;
	CNPC*		onpc			= NULL;
	CPet*		opet			= NULL;
	CElemental*	oelemental		= NULL;
	CAPet*		oapet			= NULL;

	bool bNPCKilledNPC = false; // npc가 npc를 죽인 경우 꼭 이것을 true로 해줘야한다.

	switch (of->m_type)
	{
	case MSG_CHAR_PC:
		opc = TO_PC(of);
		if (opc == NULL)
			goto END_PROC;
		break;

	case MSG_CHAR_NPC:
		onpc = TO_NPC(of);
		break;

	case MSG_CHAR_PET:
		opet = TO_PET(of);
		opc = opet->GetOwner();
		if (opc == NULL)
			goto END_PROC;
		break;

	case MSG_CHAR_ELEMENTAL:
		oelemental = TO_ELEMENTAL(of);
		opc = oelemental->GetOwner();
		if (opc == NULL)
			goto END_PROC;
		break;
	case MSG_CHAR_APET:
		oapet	= TO_APET(of);
		opc		= oapet->GetOwner();
		if( opc == NULL )
			goto END_PROC;
		break;

	default:
		goto END_PROC;
	}
#ifdef SYSTEM_TREASURE_MAP
//	if( df->m_idNum == TREASURE_BOX_NPC_INDEX)	 // 보물상자 npc를 잡았다. 정보를 없애주자.
//		df->m_pZone->RemoveTreasureBoxNpc(df );
#endif

	// 공주구출 퀘스트 (퍼스널 던전 2) 실패
	ProcDead_PD2(df);

	// 죽은 것이 테이밍 몬스터일 경우
	if (df->Check_MobFlag( STATE_MONSTER_TAMING ) )
	{
		CPC* owner = NULL;			// 몬스터를 테이밍한 캐릭터
		owner = df->GetOwner();		// 몬스터가 테이밍 되었는지 확인

		// 주인이 공격하고 있는 타겟을 지워준다. 주인이 테이밍 중이 아닌걸로 바꿔준다.
		if ( owner )
		{
			owner->DeleteSlave( df );
		}
		goto SKIP_DROP;
	}

	// 죽인 것이 테이밍 몬스터일 경우
	if (onpc && onpc->Check_MobFlag( STATE_MONSTER_TAMING ) )
	{
		CPC* owner = NULL;				// 몬스터를 테이밍한 캐릭터
		owner = onpc->GetOwner();		// 몬스터가 테이밍 되었는지 확인

		// 주인이 공격하고 있는 타겟을 지워준다.
		if ( owner )
		{
			owner->SetOwners_target(NULL);
			// opc에 주인을 넣어준다.
			opc = owner;
			bNPCKilledNPC = true;
		}
		else
			goto SKIP_DROP;
	}
	else if( onpc && onpc->GetOwner() ) // 공격한 NPC가 오너가 있다면
	{
		CNPC* sumNpc = onpc->GetOwner()->GetSummonNpc(onpc);
		if( sumNpc )
		{
			if( sumNpc->Check_MobFlag((STATE_MONSTER_MERCENARY)) )
			{
				sumNpc->GetOwner()->SetSummonOwners_target(NULL);
			}

			opc = onpc->GetOwner();
			bNPCKilledNPC = true;
		}
		else
			goto SKIP_DROP;
	}

	if( df && df->GetOwner() ) // 죽은 넘이 owner 있다면
	{
		if( df->Check_MobFlag(STATE_MONSTER_PARASITE) ) // 패러사이트에 걸려있다면.
		{
			int parasiteCnt = GetRandom(0,3);
			parasiteCnt -= df->GetOwner()->GetBombSummonCont();
			if( parasiteCnt > 0 )
			{
				int parasiteIdx = df->m_assist.GetSummonNpcIndex();
				if( parasiteIdx > 0 )
				{
					int i;
					for(i=0; i<parasiteCnt; i++)
					{
						CNPC* pParasiteNPC;
						pParasiteNPC = gserver->m_npcProtoList.Create(parasiteIdx, NULL );
						if( pParasiteNPC == NULL )
							continue;

						GET_X(pParasiteNPC) = GET_X(df);
						GET_Z(pParasiteNPC) = GET_Z(df);
						GET_R(pParasiteNPC) = GET_R(df);
						GET_YLAYER(pParasiteNPC) = GET_YLAYER(df);

						float fRand = GetRandom(0,1) ? 1.0f : -1.0f ;
						float x  = 2.0f + ( fRand * (float)(GetRandom( 0 , 200 ) / 100.0f) );
						fRand = GetRandom(0,1) ? 1 : -1 ;
						float z  = 2.0f + ( fRand * (float)(GetRandom( 0 , 200 ) / 100.0f) );

						pParasiteNPC->m_regenX = GET_X(pParasiteNPC) += x;
						pParasiteNPC->m_regenZ = GET_Z(pParasiteNPC) += z;
						pParasiteNPC->m_regenY = GET_YLAYER(pParasiteNPC);

						pParasiteNPC->CalcStatus(false);

						CSkill * pSkill = gserver->m_skillProtoList.Create( 1133 ); // 자살 공격
						if( pSkill == NULL )
						{
							delete pParasiteNPC ;
							pParasiteNPC = NULL;
							continue;
						}

						pParasiteNPC->SetOwner(df->GetOwner());

						bool bApply;
						if( 0 != ApplySkill((CCharacter*)df->GetOwner(), (CCharacter*)pParasiteNPC, pSkill, -1, bApply) )
						{
							delete pSkill;
							pSkill = NULL;

							delete pParasiteNPC;

							continue;
						}
						delete pSkill;
						pSkill = NULL;

						if( bApply == false )
						{
							delete pParasiteNPC ;
							pParasiteNPC = NULL;
							continue;
						}
						df->GetOwner()->SetBombSummonNPC(pParasiteNPC);

						int cx, cz;
						df->m_pArea->AddNPC(pParasiteNPC);
						df->m_pArea->PointToCellNum(GET_X(pParasiteNPC), GET_Z(pParasiteNPC), &cx, &cz);
						df->m_pArea->CharToCell(pParasiteNPC, GET_YLAYER(pParasiteNPC), cx, cz);

						{
							CNetMsg::SP rmsg(new CNetMsg);
							AppearMsg(rmsg, pParasiteNPC, true);
							df->m_pArea->SendToCell(rmsg, GET_YLAYER(pParasiteNPC), cx, cz);
						}
					}
				}
			}
		}

		CNPC* sumNpc = df->GetOwner()->GetSummonNpc(df);
		if( sumNpc )
		{
#ifdef BUGFIX_MERCNERAY_DELETE
			sumNpc->GetOwner()->SummonNpcRemove(df, false);
#else
			sumNpc->GetOwner()->SummonNpcRemove(df);
#endif
			goto SKIP_DROP;
		}
	}
	/*

	*/

	// 이곳으로 넘어오면 테이밍이 아니므로, 모든 몬스터는 몬스터에게 죽으면 패스
	else if (onpc && !bNPCKilledNPC)
	{
		goto SKIP_DROP;
	}

	// pc가 npc를 죽이면 테이밍 몬스터의 타겟을 지워준다.
	if (opc)
	{
		opc->SetOwners_target(NULL);
		opc->SetSummonOwners_target(NULL);
	}

	// 리더 사망시 처리
	if (!df->m_proto->CheckFlag(NPC_RAID))
		ProcFollowNPC(df);

	// 공성 포인트 계산
	if (opc)
		CalcWarPoint(opc, df);

	// 죽은 NPC가 공성탑이나 수호병이 아닐 경우 처리
	if (!df->m_proto->CheckFlag(NPC_CASTLE_TOWER | NPC_CASTLE_GUARD))
	{
		int level = -1;
		LONGLONG nTotalDamage = 0;
		// 우선권 PC, 평균 레벨 구하기
		CPC* tpc = FindPreferencePC(df, &level, &nTotalDamage);
#ifdef GER_LOG
		if( IS_PC( of ))
		{
			CPC *user = TO_PC( of );
			GAMELOGGEM << init( 0, "CHAR_VICTORY" )
					   << LOG_VAL("account-id", user->m_desc->m_idname ) << blank
					   << LOG_VAL("character-id", user->m_desc->m_pChar->m_name ) << blank
					   << LOG_VAL("zone-id", user->m_desc->m_pChar->m_pZone->m_index ) << blank
					   << LOG_VAL("victim-id", df->m_index ) << blank
					   /*<< LOG_VAL("opponent-id", kill) << blank*/
					   << LOG_VAL("longitude", GET_X(user) ) << blank
					   << LOG_VAL("latitude", GET_Z(user) ) << blank
					   << endGer;
		}
#endif
		// 보스몹
		if (df->m_proto->CheckFlag(NPC_BOSS | NPC_MBOSS | NPC_RAID))
		{
			GAMELOG << init("MOB DEAD")
					<< "INDEX" << delim
					<< df->m_proto->m_index << delim
					<< "NAME" << delim
					<< df->m_name << delim
					<< "ZONE" << delim
					<< df->m_pZone->m_index << delim
					<< "POSITION" << delim
					<< GET_X(df) << delim
					<< GET_Z(df) << delim
					<< GET_YLAYER(df) << delim
					<< "KILL BY" << delim;
			if (opc)
			{
				GAMELOG << opc->m_index << delim
						<< opc->m_name << delim
						<< opc->m_nick << delim
						<< opc->m_job << delim
						<< opc->m_job2 << delim
						<< opc->m_level;
			}
			else
			{
				GAMELOG << of->m_type << delim
						<< of->m_index << delim
						<< of->m_name << delim
						<< of->m_level;
			}
			GAMELOG << end;

			if (df->m_proto->CheckFlag(NPC_BOSS | NPC_MBOSS))
			{
				// 카오 성향 회복 : 보스몹을 잡으면 회복 보너스
				if (opc && opc->IsChaotic() && tpc == opc)
				{
					if( !gserver->m_bNonPK )
						opc->m_pkPenalty += df->m_level / 10;

					if (opc->m_pkPenalty > 0)
						opc->m_pkPenalty = 0;

					{
						CNetMsg::SP rmsg(new CNetMsg);
						CharStatusMsg(rmsg, opc, 0);
						opc->m_pArea->SendToCell(rmsg, opc, false);
					}

					opc->m_bChangeStatus = true;
				}
			}
		} // 보스몹

		if(opc && opc->m_pArea && df->m_proto->m_index == 1002 && df->m_pZone && df->m_pZone->m_index == ZONE_ALTER_OF_DARK)
		{
			// 네임드 몬스터 죽은 것으로 체크
			opc->m_pArea->m_CTriggerList.Set_TriggerFlag(TRIGGER_FLAG_NAMEDNPC_DEATH1);
			opc->m_pArea->m_CTriggerList.Set_TriggerFlag(TRIGGER_FLAG_NAMEDNPC_DEATH1002_BEFORE);
			opc->m_pArea->m_CTriggerList.SaveTriggerInfo(TRIGGER_SAVE_ALTER_OF_DARK_1002, opc->m_nJoinInzone_RoomNo);	//트리거 정보 저장
			opc->m_pArea->Change_NpcRegenRaid(TRIGGER_SAVE_ALTER_OF_DARK_1002, 1002);
		}
		else if(opc && opc->m_pArea && df->m_proto->m_index == 1003 && df->m_pZone && df->m_pZone->m_index == ZONE_ALTER_OF_DARK)
		{
			// 네임드 몬스터 죽은 것으로 체크
			opc->m_pArea->m_CTriggerList.Set_TriggerFlag(TRIGGER_FLAG_NAMEDNPC_DEATH2);
			opc->m_pArea->m_CTriggerList.Set_TriggerFlag(TRIGGER_FLAG_NAMEDNPC_DEATH1003_BEFORE);
			opc->m_pArea->m_CTriggerList.SaveTriggerInfo(TRIGGER_SAVE_ALTER_OF_DARK_1003, opc->m_nJoinInzone_RoomNo);	//트리거 정보 저장
			opc->m_pArea->Change_NpcRegenRaid(TRIGGER_SAVE_ALTER_OF_DARK_1003, 1003);
		}
		else if(opc && opc->m_pArea && df->m_proto->m_index == 1018 && df->m_pZone && df->m_pZone->m_index == ZONE_ALTER_OF_DARK)
		{
			// 네임드 몬스터 죽은 것으로 체크
			opc->m_pArea->m_CTriggerList.Set_TriggerFlag(TRIGGER_FLAG_NAMEDNPC_DEATH1018_BEFORE);
			opc->m_pArea->m_CTriggerList.SaveTriggerInfo(TRIGGER_SAVE_ALTER_OF_DARK_1018, opc->m_nJoinInzone_RoomNo);	//트리거 정보 저장
			opc->m_pArea->Change_NpcRegenRaid(TRIGGER_SAVE_ALTER_OF_DARK_1018, 1018);
		}
		else if (opc && opc->m_pArea && df->m_proto->m_index == 963 && df->m_pZone && df->m_pZone->m_index == ZONE_CAPPELLA_1)
		{
			// 트리거를 사용하기 위한 npc963 죽은 count 세기
			opc->m_pArea->m_CTriggerList.m_nNPC963_KilledCount += 1;
		}

		int nObjectData;
		int nAkanNpcIdx = df->m_proto->m_index;

		switch(nAkanNpcIdx)
		{
		case 1115:				// 파독스의 인형(Hard)
		case 1170:				// 파독스의 인형(Normal)
			{
				nObjectData = 10;
				if(opc && opc->m_pArea && df->m_pZone && df->m_pZone->m_index == ZONE_AKAN_TEMPLE)
				{
					CNetMsg::SP rmsg(new CNetMsg);
					RaidSceneMsg(rmsg, OBJECT_TYPE_TODO, KILL_NPC, nAkanNpcIdx, nObjectData);
					do_ExRaidScene(opc, rmsg);
				}
			}
			break;
		case 1112:				// 툴만
		case 1116:				// 파독스
		case 1120:				// 쿤타
		case 1124:				// 유작
		case 1126:				// 고위 연금술사 제롬
		case 1127:				// 고위 마법술사 미엘
		case 1128:				// 제사장 미쿠
		case 1167:				// 툴만
		case 1171:				// 파독스
		case 1175:				// 쿤타
		case 1179:				// 유작
		case 1180:				// 고위 연금술사 제롬
		case 1181:				// 고위 마법술사 미엘
		case 1182:				// 제사장 미쿠
			{
				nObjectData = 1;
				if(opc && opc->m_pArea && df->m_pZone && df->m_pZone->m_index == ZONE_AKAN_TEMPLE)
				{
					{
						CNetMsg::SP rmsg(new CNetMsg);
						RaidSceneMsg(rmsg, OBJECT_TYPE_TODO, KILL_NPC, nAkanNpcIdx, nObjectData);
						do_ExRaidScene(opc, rmsg);
					}
				}
			}
			break;
		case 1259:	//망각의 신전
			{
				nObjectData = 1;
				if(opc && opc->m_pArea && df->m_pZone && df->m_pZone->m_index == ZONE_DUNGEON4)
				{
					CNetMsg::SP rmsg(new CNetMsg);
					RaidSceneMsg(rmsg, OBJECT_TYPE_TODO, KILL_NPC, nAkanNpcIdx, nObjectData);
					do_ExRaidScene(opc, rmsg);
				}
			}
			break;
		case 1364:
			{
				if(opc && opc->m_pArea && df->m_pZone && df->m_pZone->m_index == ZONE_TARIAN_DUNGEON)
				{
					CNetMsg::SP rmsg(new CNetMsg);
					RaidSceneMsg(rmsg, OBJECT_TYPE_TODO, KILL_NPC, nAkanNpcIdx, 1);
					do_ExRaidScene(opc, rmsg);
				}
			}
			break;
		default:
			break;
		}

#ifdef REFORM_PK_PENALTY_201108 // PK 패널티 리폼 :: npc를 잡으면 무조건 헌터 쪽으로 향상 시켜준다.
		if( !gserver->m_bNonPK )
		{
			if(df && opc)
			{
				int nlevel = df->m_level - opc->m_level;
				int pkPenalty = 0;
				if( nlevel > 4 )
					pkPenalty += 15;
				else if( nlevel > -5 )
					pkPenalty += 10;
				else if( nlevel <= -5 && nlevel >= -10)
					pkPenalty += 5;

				// 성향 수치 상승 증폭제를 사용 중이라면
				if( opc->m_assist.m_avRate.pkDispositionPointValue > 0 )
				{
					pkPenalty = pkPenalty * opc->m_assist.m_avRate.pkDispositionPointValue;
					opc->m_assist.CureByItemIndex(7474);	// 성향 수치 상승 증폭제
					opc->m_assist.CureByItemIndex(7475);	// 성향 수치 상승 증폭제
					opc->m_assist.CureByItemIndex(7476);	// 성향 수치 상승 증폭제
				}
				opc->AddPkPenalty( pkPenalty );

				{
					CNetMsg::SP rmsg(new CNetMsg);
					CharStatusMsg(rmsg, opc, 0);
					opc->m_pArea->SendToCell(rmsg, opc, false);
				}

				opc->m_bChangeStatus = true;
			}
		}
#else // REFORM_PK_PENALTY_201108 // PK 패널티 리폼
		if (opc && opc->IsChaotic() && df->m_level >= opc->m_level - 5)
		{
			opc->m_pkRecoverNPCCount++;
			if (opc->m_pkRecoverNPCCount >= 25)
			{
				opc->m_pkRecoverNPCCount = 0;

				if( !gserver->m_bNonPK )
					opc->m_pkPenalty++;

				{
					CNetMsg::SP rmsg(new CNetMsg);
					CharStatusMsg(rmsg, opc, 0);
					opc->m_pArea->SendToCell(rmsg, opc, false);
				}

				opc->m_bChangeStatus = true;
			}
		} // 카오 성향 회복
#endif // REFORM_PK_PENALTY_201108 // PK 패널티 리폼
		// Exp, SP 분배
		// 이루틴중 레벨업을 하여 존이동을 하였을 경우 나머지 실행하지 않는다.
		if( DivisionExpSP(df, tpc, nTotalDamage) )
			goto END_PROC;

		// 아이템 드롭
		ProcDropItemAfterBattle(df, opc, tpc, level);

		// 진행중인 Quest 중 죽은 npc로 진행중이면 UpdateData
		if (opc && opc == tpc)
		{
			if( opc->m_pZone->IsPersonalDungeon() )
			{
				opc->m_pArea->m_nMakeNPC++;

#if defined ( LC_GAMIGO ) || defined ( LC_KOR ) || defined ( LC_USA )
				if( df->m_proto->m_index == 5 )
				{
					if(opc->m_pArea->m_nMakeNPC < 103)
						goto SKIP_DROP;
					else
					{
						GAMELOG << init("QUEST COMPLETE PD1", opc)
								<< opc->m_pArea->m_nMakeNPC
								<< end;
					}
				}
				else if(df->m_proto->m_index == 201 && opc->m_pArea->m_nMakeNPC < 50)
				{
					goto SKIP_DROP;
				}
#else
				if( (df->m_proto->m_index == 5 || df->m_proto->m_index == 201 ) && opc->m_pArea->m_nMakeNPC < 50 )
				{
					goto SKIP_DROP;
				}
#endif // LC_GAMIGO || LC_KOR || LC_USA
			}

#ifdef PARTY_QUEST_ITEM_BUG_
			ProcDeadQuestProc(opc, df, QTYPE_SCALE_PERSONAL); // 내가 어그로도 먹고 몬스터도 막타 쳤단다.
#else
			CQuest* pQuest = NULL;
			CQuest* pQuestNext = opc->m_questList.GetNextQuest(NULL, QUEST_STATE_RUN);
			while ((pQuest = pQuestNext))
			{
				pQuestNext = opc->m_questList.GetNextQuest(pQuestNext, QUEST_STATE_RUN);
				// 퀘스트 있고 수행중이고 반복, 수집, 격파, 구출 퀘스트이면
				switch (pQuest->GetQuestType0())
				{
				case QTYPE_KIND_REPEAT:
				case QTYPE_KIND_COLLECTION:
				case QTYPE_KIND_DEFEAT:
				case QTYPE_KIND_SAVE:
					pQuest->QuestUpdateDataForParty(opc, df);
					break;

				default:
					break;
				}
			}
			if( pQuest == NULL && opc->IsParty() && opc->m_party )
			{
				int i;
				const CPartyMember* pPartyMember;
				CPC*	pPartyPC;

				for(i=0; i<MAX_PARTY_MEMBER; ++i)
				{
					pPartyMember = opc->m_party->GetMemberByListIndex(i);
					if(pPartyMember && pPartyMember->GetMemberPCPtr())
					{
						pPartyPC = pPartyMember->GetMemberPCPtr();

						if(opc->m_pArea->FindCharInCell(opc, pPartyPC->m_index, MSG_CHAR_PC))
						{
							pQuest = pPartyPC->m_questList.FindQuestByMob( df->m_idNum);

							if( pQuest == NULL)
								continue;

							if( pQuest->GetPartyScale() != QTYPE_SCALE_PARTY)
								break;

							switch (pQuest->GetQuestType0())
							{
							case QTYPE_KIND_REPEAT:
							case QTYPE_KIND_COLLECTION:
							case QTYPE_KIND_DEFEAT:
							case QTYPE_KIND_SAVE:
								pQuest->QuestUpdateData(pPartyPC, df);
								break;

							default:
								break;
							}
						}
					}
				}
			}
#endif // PARTY_QUEST_ITEM_BUG_
		}
#ifdef PARTY_QUEST_ITEM_BUG_
		else if(opc) // [2010/12/28 derek] opc == NULL 인데도 들어가져서 퀘스트 찾다가 서버 다운되어 추가함.
		{
#ifdef _BATTLEGROUP_QUEST_BUG_PIX
			if( opc->IsExped() ) // 어글은 않먹었지만 막타로 잡았으면
				ProcDeadQuestProc(opc, df, QTYPE_SCALE_BATTLEGROUP);
			else
#endif
				ProcDeadQuestProc(opc, df, QTYPE_SCALE_PARTY);
		}

#endif //PARTY_QUEST_ITEM_BUG_
	} // 죽은 NPC가 공성탑이나 수호병이 아닐 경우 처리
	else
	{
		int level = -1;
		LONGLONG nTotalDamage = 0;
		// 우선권 PC, 평균 레벨 구하기
		CPC* tpc = FindPreferencePC(df, &level, &nTotalDamage);
		DropWarCastleToken(df, opc, tpc, level);
	}

SKIP_DROP:

	// 수호탑은 DelNPC() 안하고 UpdateGateState() 후에 메시지로 알린다.
	if (df->m_proto->CheckFlag(NPC_CASTLE_TOWER) != 0)
	{
		int gstate_old = 0, gstate_new = 0;

		CWarCastle * castle = CWarCastle::GetCastleObject(ZONE_MERAC);
		CDratanCastle * pCastle = CDratanCastle::CreateInstance();

		if (of->m_pZone->m_index == ZONE_MERAC)
		{
			if (castle != NULL)
			{
				gstate_old = castle->GetGateState();
				gstate_old |= pCastle->GetGateState();
				castle->UpdateGateState(df);
				gstate_new = castle->GetGateState();
				gstate_new |= pCastle->GetGateState();
			}
		}
		else if (of->m_pZone->m_index == ZONE_DRATAN)
		{
			gstate_old = pCastle->GetGateState();

			if (castle != NULL)
			{
				gstate_old |= castle->GetGateState();
			}

			pCastle->UpdateGateState(df);
			gstate_new = pCastle->GetGateState();

			if (castle != NULL)
			{
				gstate_new |= castle->GetGateState();
			}

			if (df->m_proto->CheckFlag(NPC_WARCASTLE) != 0)
			{
				// NPC_CASTLE_TOWER 에 NPC_WARCASTLE 면
				// 마스터 타워와 부활진지
				int qindex = df->m_proto->m_index;
				if (qindex >= 390 && qindex <= 396)
				{
					// 부활진지 파괴 알림
					CNetMsg::SP rmsg(new CNetMsg);
					CastleTowerQuartersCrushMsg(rmsg, qindex);
					of->m_pArea->SendToAllClient(rmsg);

					// 부활진지 파괴 처리
					/*pCastle->m_nRebrithGuild[df->m_proto->m_index - 390] =  -1;
					memset((void *)pCastle->m_strRebrithGuild[df->m_proto->m_index - 390], 0, 51);*/
				}
			}
		}

		if (gstate_old != gstate_new)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			GuildWarGateStateMsg(rmsg, gstate_old, gstate_new);
			of->m_pArea->SendToAllClient(rmsg);
		}

		DelAttackList(df);

		if (of->m_pZone->m_index == ZONE_DRATAN)
		{
			if( df->m_proto->m_index == 351)
			{
				// 마스터 타워
				// 모든 타워 기능 정지
				pCastle->StopCastleTower();
			}
			else if (df->m_proto->CheckFlag(NPC_CASTLE_TOWER) != 0)
			{
				// 공격 수호상 (결계의 눈 등..) 로그
				GAMELOG << init("DRATAN CASTLE NPC DEAD : ") << df->m_proto->m_name
						<< " BROKEN BY : " << of->m_name << end;
				// 마스터 타워가 아닌 모든 타워
				int  i;
				// 부활 진지 삭제
//				for (i=0; i<7; i++)
//				{
//					if (pCastle->m_pRebrithNPC[i] == df)
//					{
//						pCastle->m_pRebrithNPC[i] = NULL;
//						pCastle->m_nRebrithGuild[i] = -1;
//						memset((void *)pCastle->m_strRebrithGuild[i], 0, 51);
//#ifdef BUGFIX_WARCASTLE_REGEN
//						pCastle->m_nRegenTimeRebirthNPC[i] = gserver->getNowSecond() + pCastle->GetRegenNPCRebirthTime();
//#else
//						pCastle->m_nRegenTimeRebirthNPC[i] = gserver->m_pulse + pCastle->GetRegenNPCRebirthTime();
//#endif // BUGFIX_WARCASTLE_REGEN
//					}
//				}

				// 워프 타워 삭제
				for (i=0; i<5; i++)
				{
					if (pCastle->m_pWarpNPC[i] == df)
					{
						pCastle->m_pWarpNPC[i] = NULL;
					}
				}

				// 결계의 눈 삭제
				for (i=0; i<5; i++)
				{
					if (pCastle->m_gateNPC[i] == df)
					{
						pCastle->m_gateNPC[i] = NULL;
					}
				}

				// 알림
				of->m_pArea->CharFromCell(df, true);
				of->m_pArea->DelNPC(df);
			}
		}
		return ;
	} // 수호탑은 DelNPC() 안하고 UpdateGateState() 후에 메시지로 알린다.
//#endif

#ifdef EXTREME_CUBE
	if(df->m_bCubeRegen)
	{
		CCubeSpace* cube = gserver->m_extremeCube.GetExtremeCube(df->m_pArea->m_index);
		if(cube)
		{
			cube->DelMob(df);

			if(gserver->m_extremeCube.IsGuildCubeTime() && opc && opc->m_guildInfo && opc->m_guildInfo->guild())
			{
				CCubeMemList* CubeMemList;
				CubeMemList = gserver->m_extremeCube.FindMemList(opc->m_guildInfo->guild());
				if(CubeMemList)
				{
					time_t lastCubePoint;

					time(&lastCubePoint);
					CNetMsg::SP rmsg(new CNetMsg);
					HelperAddCubePointMsg(rmsg, opc->m_guildInfo->guild()->index(), df->m_level, lastCubePoint);
					SEND_Q(rmsg, gserver->m_helper);
				}
			}
		}
	}

	if(df->m_pZone != NULL && df->m_proto->m_index == 529 && df->m_pZone->IsExtremeCube())
	{
		CCubeSpace* cube = gserver->m_extremeCube.GetExtremeCube(df->m_pArea->m_index);

		if(cube && (cube->m_crystal == df) )
		{
			// cube->m_crystal = NULL;

			cube->DelCrystal(false);
			cube->m_waitTime = gserver->m_pulse + PULSE_REAL_SEC * 10;
			return ;
		}
	}
	else if(df->m_pZone != NULL && df->m_proto->m_index == 527 && df->m_pZone->IsExtremeCube())
	{
		CCubeSpace* cube = gserver->m_extremeCube.GetExtremeCube(df->m_pArea->m_index);

		if(cube && (cube->m_crystal == df) )
		{
			// cube->m_crystal = NULL;

			cube->DelCrystal(false);
			cube->m_waitTime = gserver->m_pulse + PULSE_REAL_SEC * 10;
			return ;
		}
	}
#endif // EXTREME_CUBE

	if(df && opc)
	{
		vec_affinityList_t::iterator it = df->m_proto->m_affinityList.begin();
		vec_affinityList_t::iterator endit = df->m_proto->m_affinityList.end();
		
		int point = 0;

		for(; it != endit; ++it)
		{
			CAffinityProto* proto = *(it);

			CAffinity* affinity = opc->m_affinityList.FindAffinity(proto->m_index);
			if(affinity)
			{
				point = proto->GetAffinityPointOfNPC(df->m_idNum);
				int bonus = 0;
				if(opc->m_avPassiveAddition.affinity_monster > 0)
				{
					bonus += opc->m_avPassiveAddition.affinity_monster;
				}
				if(opc->m_avPassiveRate.affinity_monster > 0)
				{
					bonus = point * (opc->m_avPassiveRate.affinity_monster - 100) / SKILL_RATE_UNIT;
				}

				affinity->AddPoint( point, opc, bonus);
			}
		}
	}

	if(df->m_ctCount > 0)
	{
		gserver->m_npc_ctTime.erase(df->m_index);
	}

END_PROC:

	//rvr 존에서 공격시에 동작해야 되는 함수 (NPC 가 죽었을 경우)
	if(opc != NULL && of->m_pZone->isRVRZone() && df->m_pZone->isRVRZone())
	{
		ProcDead_RVR(opc, df);
	}

	// 모든 몬스터는 몬스터에게 죽으면 패스
	if ( onpc || bNPCKilledNPC )
	{
		// 해당 에어리어에 죽은 수를 표시한다.  여기서는 바로 안지우고 따로 처리..
		// MobActivity.cpp::MobActivity() 타고 들어오면 꼭 여길 거쳐야한다.
		onpc->m_pArea->m_nNPC_Killed_NPC++;
	}
	else
	{
		DelAttackList(df);
		of->m_pArea->CharFromCell(df, true);
		of->m_pArea->DelNPC(df);
	}
}

void ProcDead_RVR(CPC* op, CNPC* df)
{
	//적 npc 를 죽였을 경우에는 등급과 각 결사대가 소유한 보석량에 따라서 기여도를 부여해준다.
	int jewelKailuxPoint		= SyndicateInfoDataManager::instance()->getJewelPoint(SYNDICATE::eSYNDICATE_KAILUX);
	int jewelDealerMoonPoint	= SyndicateInfoDataManager::instance()->getJewelPoint(SYNDICATE::eSYNDICATE_DEALERMOON);
	int grade = df->m_proto->m_rvr_grade;

	//공격자가 카이룩스 인 경우
	if(op->getSyndicateType() == SYNDICATE::eSYNDICATE_KAILUX)
	{
		if (jewelKailuxPoint < jewelDealerMoonPoint)
		{
			switch(grade)
			{
			case 1:
				op->m_syndicateManager.increaseSyndicatePoint(200);
				break;
			case 2:
				op->m_syndicateManager.increaseSyndicatePoint(320);
				break;
			case 3:
				op->m_syndicateManager.increaseSyndicatePoint(500);
				break;
			}
		}
		else
		{
			op->m_syndicateManager.increaseSyndicatePoint(1200);
		}
	}
	else if(op->getSyndicateType() == SYNDICATE::eSYNDICATE_DEALERMOON)
	{
		if (jewelKailuxPoint < jewelDealerMoonPoint)
		{
			op->m_syndicateManager.increaseSyndicatePoint(1500);
		}
		else
		{
			switch(grade)
			{
			case 1:
				op->m_syndicateManager.increaseSyndicatePoint(200);
				break;
			case 2:
				op->m_syndicateManager.increaseSyndicatePoint(320);
				break;
			case 3:
				op->m_syndicateManager.increaseSyndicatePoint(500);
				break;
			}
		}
	}
}

void ProcDead_PD2(CNPC* df)
{
	if (df->m_pZone->m_index == ZONE_SINGLE_DUNGEON2)
	{
		CCharacter* ch = NULL;
		CPC* pc = NULL;

		int i, j;
		for (i = 0; i < df->m_pArea->m_size[0]; i++)
		{
			for (j = 0; j < df->m_pArea->m_size[1]; j++)
			{
				ch = df->m_pArea->m_cell[i][j].m_listChar;
				while (ch)
				{
					if (IS_PC(ch) && !DEAD(ch))
					{
						pc = TO_PC(ch);
						break;
					}
					ch = ch->m_pCellNext;
				}
			}
		}

		if (!pc)
			return ;

		// 퀘스트 실패
		CQuest* pQuest = NULL;
		CQuest* pQuestNext = NULL;
		const CQuestProto* pQuestProto = NULL;
		pQuestNext = pc->m_questList.GetNextQuest(NULL, QUEST_STATE_RUN);
		while ((pQuest = pQuestNext))
		{
			pQuestNext = pc->m_questList.GetNextQuest(pQuestNext, QUEST_STATE_RUN);
			// 구출형 퀘스트 이고
			if (pQuest->GetQuestType0() == QTYPE_KIND_SAVE)
			{
				pQuestProto = pQuest->GetQuestProto();
				int j;
				for (j = 0; j < QUEST_MAX_CONDITION; j++)
				{
					// 죽은 df가 구출할 공주 이면
					if (pQuestProto->m_conditionType[j] == QCONDITION_NPC &&
							pQuestProto->m_conditionIndex[j] == df->m_proto->m_index)
					{
						// 아이템 삭제
						pQuest->RemoeQuestItem(pc);

						// Quest Fail Log
						GAMELOG << init("QUEST FAIL", pc)
								<< pQuestProto->m_index
								<< end;

						CNetMsg::SP rmsg(new CNetMsg);
						QuestFailMsg(rmsg, pQuest);
						pc->m_questList.DelQuest(pc, pQuest);
						SEND_Q(rmsg, pc->m_desc);
						return ;
					}
				}
			}
		} // while (pQuest)
	}
}

#ifdef PARTY_QUEST_ITEM_BUG_
void ProcDeadQuestProc(CPC * opc,CNPC * df, int partyScale)
{
#ifdef HUNT_QUEST_COMPLETE_FOR_OTHER_PARTYMEMBER
	// 파티 스케일이 어떤것이 넘어오든 상관없다.
	// 막타친사람이 원정대인지, 파티인지, 솔로인지만 체크한다.
	// 원정대일 경우 원정대 퀘스트만 수행된다. 솔로퀘스트도 수행이 안된다.
	// 솔로나 파티일경우 자기 퀘스트도 되고, 파티원 퀘스트도 같이 된다.
	if(opc->IsExped())
	{
		// 원정대 퀘스트
		int i, j;
		CPC* pExpedPC = NULL;
		if(opc->m_Exped)
		{
			for(i = 0; i < MAX_EXPED_GROUP; i++)
			{
				for(j = 0; j < MAX_EXPED_GMEMBER; j++)
				{
					pExpedPC = opc->m_Exped->GetMemberPCPtr(i, j);
					if(pExpedPC == NULL)
						continue;

					if(opc->m_pArea->FindCharInCell(opc, pExpedPC->m_index, MSG_CHAR_PC))
					{
						CQuest* pQuest = NULL;
						CQuest* pQuestNext = pExpedPC->m_questList.GetNextQuest(NULL, QUEST_STATE_RUN);
						while((pQuest = pQuestNext))
						{
							pQuestNext = pExpedPC->m_questList.GetNextQuest(pQuestNext, QUEST_STATE_RUN);
							switch(pQuest->GetQuestType0())
							{
							case QTYPE_KIND_REPEAT:
							case QTYPE_KIND_COLLECTION:
							case QTYPE_KIND_DEFEAT:
							case QTYPE_KIND_SAVE:
								{
									if(pQuest->GetPartyScale() == QTYPE_SCALE_BATTLEGROUP)
										pQuest->QuestUpdateData(pExpedPC, df);
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
		return ;
	}
	else
	{
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
							pQuest->QuestUpdateData(opc, df);
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
										pQuest->QuestUpdateData(pMember, df);
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

#else // HUNT_QUEST_COMPLETE_FOR_OTHER_PARTYMEMBER
	CQuest* pQuest = NULL;
	CQuest* pQuestNext = opc->m_questList.GetNextQuest(NULL, QUEST_STATE_RUN);

	while ((pQuest = pQuestNext))
	{
		pQuestNext = opc->m_questList.GetNextQuest(pQuestNext, QUEST_STATE_RUN);
		// 퀘스트 있고 수행중이고 반복, 수집, 격파, 구출 퀘스트이면
		if( (partyScale == QTYPE_SCALE_PARTY && pQuest->GetPartyScale() != QTYPE_SCALE_PARTY )
#ifdef _BATTLEGROUP_QUEST_BUG_PIX
				|| (partyScale == QTYPE_SCALE_BATTLEGROUP && pQuest->GetPartyScale() != QTYPE_SCALE_BATTLEGROUP )
#endif // _BATTLEGROUP_QUEST_BUG_PIX
		  )
		{
			return;
		}

		switch (pQuest->GetQuestType0() )
		{
		case QTYPE_KIND_REPEAT:
		case QTYPE_KIND_COLLECTION:
		case QTYPE_KIND_DEFEAT:
		case QTYPE_KIND_SAVE:
			pQuest->QuestUpdateDataForParty(opc, df);
			break;

		default:
			break;
		}
	}
#endif // HUNT_QUEST_COMPLETE_FOR_OTHER_PARTYMEMBER
}
#endif // PARTY_QUEST_ITEM_BUG_
//