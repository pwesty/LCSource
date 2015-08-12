#include "stdhdrs.h"

#include "Log.h"
#include "Character.h"
#include "Server.h"
#include "CmdMsg.h"
#include "doFunc.h"
#include "hardcoding.h"
#include "Battle.h"
#include "Exp.h"
#include "WarCastle.h"
#include "DratanCastle.h"


void do_Raid(CPC* ch, CNetMsg::SP& msg)
{
	// 죽거나 교환중이면 무시
	if (!ch || DEAD(ch) || ch->m_currentSkill != NULL || ch->m_personalshop != NULL)
		return ;

	msg->MoveFirst();

	unsigned char subtype;
	RefMsg(msg) >> subtype;

	switch (subtype)
	{
	case MSG_RAID_INZONE_JOIN:
		do_Raid_InzoneJoin(ch, msg);
		break;
	case MSG_RAID_INZONE_QUIT:
		do_Raid_InzoneQuit(ch, msg);
		break;
	}
}

//각 유저 인존 입장 요청
//인존에 혼자서도 입장 할 수 없다
void do_Raid_InzoneJoin(CPC* ch, CNetMsg::SP& msg)
{
	int nZoneNo;	//존번호(들어갈)
	int nDifficulty = -1;

	RefMsg(msg) >> nZoneNo;
	RefMsg(msg) >> nDifficulty;
	if(nDifficulty == -1 && nZoneNo == ZONE_AKAN_TEMPLE)
		nDifficulty = 1;
	if(nDifficulty == -1 && nZoneNo == ZONE_TARIAN_DUNGEON)
		nDifficulty = 0;

	GAMELOG << init("RAID DEBUG INZONE JOIN REQ", ch)
			<< "nZoneNo" << delim << nZoneNo;

	CZone* pZone = gserver->FindZone(nZoneNo);
	if (pZone == NULL)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		RaidErrorMsg(rmsg, MSG_RAID_ERROR_INZONE_NOT_FOUND_ZONE_INFO);
		SEND_Q(rmsg, ch->m_desc);
		
		GAMELOG << " CheckZone Failed" << end;
		return ;
	}

#ifdef LC_GAMIGO
	if(pZone->m_countArea < 2)
	{
		GAMELOG << " CountArea Failed" << end;
		return ;
	}
#endif // LC_GAMIGO

	GAMELOG << end;

	if(gserver->m_bCanEnterTheRaidDungeon == false) // 레이드 던전을 들어갈 수 없는 상황이라면...false가 못들어간다.
	{
		//에러메시지 발송
		//MSG_RAID_ERROR_INZONE_INIT_TIME
		CNetMsg::SP rmsg(new CNetMsg);
		RaidErrorMsg(rmsg, MSG_RAID_ERROR_INZONE_INIT_TIME);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	if(ch->IsSetPlayerState(PLAYER_STATE_WARP))
	{
		// 이동할 수 없는 상태입니다.
		CNetMsg::SP rmsg(new CNetMsg);
		SysMsg(rmsg, MSG_SYS_CANT_WARP_OTHER);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	if(!ch->IsParty() && !ch->IsExped())
	{
		//에러: 파티 및 원정대에 가입 되어 있지 않음
		CNetMsg::SP rmsg(new CNetMsg);
		RaidErrorMsg(rmsg, MSG_RAID_ERROR_INZONE_JOIN_NOTJOIN_PARTYOREXPED);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}
	if( nZoneNo == ZONE_TARIAN_DUNGEON)
	{
		if( !(ch->m_questList.FindQuest(617, QUEST_STATE_DONE)) )
		{
			CNetMsg::SP rmsg(new CNetMsg);
			RaidErrorMsg(rmsg, MSG_RAID_ERROR_INZONE_NOT_QUEST);
			SEND_Q(rmsg, ch->m_desc);
			return;
		}
	}
	//암흑의 제단(원정대)
	if((nZoneNo == ZONE_ALTER_OF_DARK || nZoneNo == ZONE_AKAN_TEMPLE
			|| nZoneNo == ZONE_TARIAN_DUNGEON
	   ) && ch->m_Exped==NULL)
	{
		//에러: 해당 존은 원정대 존
		CNetMsg::SP rmsg(new CNetMsg);
		RaidErrorMsg(rmsg, MSG_RAID_ERROR_INZONE_JOIN_THISZONE_EXPEDZONE);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	//예배당(파티)
	if(nZoneNo == ZONE_CAPPELLA_1 && !ch->IsParty())
	{
		//에러: 해당 존은 파티 존
		CNetMsg::SP rmsg(new CNetMsg);
		RaidErrorMsg(rmsg, MSG_RAID_ERROR_INZONE_JOIN_THISZONE_PARTYZONE);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 레이드 입장 카드가 있으면 기존 보다 10 레벨 적게 제한된다.
	// 인벤토리 아이템 검사, 레이드 입장 카드
	if (ch->m_inventory.FindByDBIndex(4913, 0, 0))
	{
		//인존 입장 PC레벨 제한
		if( (nZoneNo == ZONE_ALTER_OF_DARK && (ch->m_level < RAID_JOIN_LEVEL_LIMIT_ALTER_OF_DARK - 10)) ||
				(nZoneNo == ZONE_CAPPELLA_1 && (ch->m_level < RAID_JOIN_LEVEL_LIMIT_CAPPELLA - 10)))
		{
			//에러: 레벨 제한으로 입장할수 없음
			CNetMsg::SP rmsg(new CNetMsg);
			RaidErrorMsg(rmsg, MSG_RAID_ERROR_INZONE_JOIN_INVALIDELEVEL);
			SEND_Q(rmsg, ch->m_desc);
			return;
		}

		if( nZoneNo == ZONE_AKAN_TEMPLE
				&& ((ch->m_level < RAID_JOIN_LEVEL_LOW_LIMIT_AKAN_TEMPLE - 10) || (ch->m_level > RAID_JOIN_LEVEL_HIGH_LIMIT_AKAN_TEMPLE))
				&& nDifficulty == DUNGEON_DIFFICULTY_HARD)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			RaidErrorMsg(rmsg, MSG_RAID_ERROR_INZONE_JOIN_INVALIDELEVEL);
			SEND_Q(rmsg, ch->m_desc);
			return;
		}
		if( nZoneNo == ZONE_AKAN_TEMPLE
				&& ((ch->m_level < RAID_JOIN_LEVEL_LOW_LIMIT_AKAN_TEMPLE_NORMAL - 10) || (ch->m_level > RAID_JOIN_LEVEL_HIGH_LIMIT_AKAN_TEMPLE_NORMAL))
				&& nDifficulty == DUNGEON_DIFFICULTY_NORMAL)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			RaidErrorMsg(rmsg, MSG_RAID_ERROR_INZONE_JOIN_INVALIDELEVEL);
			SEND_Q(rmsg, ch->m_desc);
			return;
		}
		if( nZoneNo == ZONE_TARIAN_DUNGEON
				&& (ch->m_level < RAID_JOIN_LEVEL_LOW_LIMIT_TARIAN_DUNGEON - 10)
				&& nDifficulty == DUNGEON_DIFFICULTY_NORMAL)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			RaidErrorMsg(rmsg, MSG_RAID_ERROR_INZONE_JOIN_INVALIDELEVEL);
			SEND_Q(rmsg, ch->m_desc);
			return;
		}
	}
	else

		//인존 입장 PC레벨 제한
		if( (nZoneNo == ZONE_ALTER_OF_DARK && (ch->m_level < RAID_JOIN_LEVEL_LIMIT_ALTER_OF_DARK)) ||
				(nZoneNo == ZONE_CAPPELLA_1 && (ch->m_level < RAID_JOIN_LEVEL_LIMIT_CAPPELLA)))
		{
			//에러: 레벨 제한으로 입장할수 없음
			CNetMsg::SP rmsg(new CNetMsg);
			RaidErrorMsg(rmsg, MSG_RAID_ERROR_INZONE_JOIN_INVALIDELEVEL);
			SEND_Q(rmsg, ch->m_desc);
			return;
		}
		else if( nZoneNo == ZONE_AKAN_TEMPLE
				 && ((ch->m_level < RAID_JOIN_LEVEL_LOW_LIMIT_AKAN_TEMPLE_NORMAL) || (ch->m_level > RAID_JOIN_LEVEL_HIGH_LIMIT_AKAN_TEMPLE_NORMAL))
				 && nDifficulty == DUNGEON_DIFFICULTY_NORMAL)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			RaidErrorMsg(rmsg, MSG_RAID_ERROR_INZONE_JOIN_INVALIDELEVEL);
			SEND_Q(rmsg, ch->m_desc);
			return;
		}
		else if( nZoneNo == ZONE_AKAN_TEMPLE
				 && ((ch->m_level < RAID_JOIN_LEVEL_LOW_LIMIT_AKAN_TEMPLE) || (ch->m_level > RAID_JOIN_LEVEL_HIGH_LIMIT_AKAN_TEMPLE))
				 && nDifficulty == DUNGEON_DIFFICULTY_HARD)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			RaidErrorMsg(rmsg, MSG_RAID_ERROR_INZONE_JOIN_INVALIDELEVEL);
			SEND_Q(rmsg, ch->m_desc);
			return;
		}
		else if( nZoneNo == ZONE_TARIAN_DUNGEON
				 && (ch->m_level < RAID_JOIN_LEVEL_LOW_LIMIT_TARIAN_DUNGEON)
				 && nDifficulty == DUNGEON_DIFFICULTY_NORMAL)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			RaidErrorMsg(rmsg, MSG_RAID_ERROR_INZONE_JOIN_INVALIDELEVEL);
			SEND_Q(rmsg, ch->m_desc);
			return;
		}

	// 원정대이면 원정대 레이드 채널 검사
	if (ch->IsExped() && ch->m_Exped->m_nSubNumber != RAID_SUBNUMBER)
	{
		//에러: 현재 채널은 원정대 레이드 채널이 아님
		CNetMsg::SP rmsg(new CNetMsg);
		RaidErrorMsg(rmsg, MSG_RAID_ERROR_INZONE_NOT_EXPED_RAID_SUBNUMBER);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 파티이면 파티 레이드 채널 검사
	else if (ch->IsParty() && ch->m_party->m_nSubNumber != RAID_SUBNUMBER)
	{
		//에러: 현재 채널은 원정대 레이드 채널이 아님
		CNetMsg::SP rmsg(new CNetMsg);
		RaidErrorMsg(rmsg, MSG_RAID_ERROR_INZONE_NOT_EXPED_RAID_SUBNUMBER);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	int nBossIndex = 0;
	int nJoinType=0;

	if(ch->IsParty())
	{
		nBossIndex = ch->m_party->GetBossIndex();
		nJoinType = MSG_JOINTYPE_PARTY;
	}

	else if(ch->IsExped())
	{
		nBossIndex = ch->m_Exped->GetBossIndex();
		nJoinType = MSG_JOINTYPE_EXPED;
	}

	// 원정대 이면서 원정대장은 아니고 원정대장이 들어가 있지 않다면 입장 불가
	if (ch->IsExped() && ch->m_index != nBossIndex && ch->m_Exped->m_nRaidAreaNum == -1)
	{
		//에러: 원정대장이 입장해 있지 않음
		CNetMsg::SP rmsg(new CNetMsg);
		RaidErrorMsg(rmsg, MSG_RAID_ERROR_INZONE_NOT_JOIN_BOSS);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	if(ch->IsExped())
	{
		CPC* pPCBoss = NULL;
		CExpedMember* pMember = NULL;
		pMember = ch->m_Exped->GetMemberByCharIndex(nBossIndex);
		if(pMember == NULL)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			RaidErrorMsg(rmsg, MSG_RAID_ERROR_INZONE_NOT_JOIN_BOSS);
			SEND_Q(rmsg, ch->m_desc);
			return;
		}

		pPCBoss = pMember->GetMemberPCPtr();
		if(pPCBoss == NULL)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			RaidErrorMsg(rmsg, MSG_RAID_ERROR_INZONE_NOT_JOIN_BOSS);
			SEND_Q(rmsg, ch->m_desc);
			return;
		}

		if(pPCBoss->m_index != ch->m_index && pPCBoss->m_pZone->m_index != nZoneNo)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			RaidErrorMsg(rmsg, MSG_RAID_ERROR_INZONE_NOT_JOIN_BOSS);
			SEND_Q(rmsg, ch->m_desc);
			return;
		}
	}

	// 원정대원 이면서 원정대장은 (Normal || Hard)인데 원정대원이 그 반대 난이도로 진입하려는 경우
	// 원정대장이 있는지는 위에서 검사를 했으니까...
	// 현재 캐릭터가 선택한 난이도가 원정대의 난이도가 틀림. 그래서 입장 불가...
	if( ch->IsExped() &&
			nZoneNo == ZONE_AKAN_TEMPLE &&
			ch->m_Exped->m_nDifficulty > -1 &&
			ch->m_Exped->m_nDifficulty != nDifficulty )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		RaidErrorMsg(rmsg, MSG_RAID_ERROR_INZONE_INVALID_DIFFICULTY);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	if (ch->m_exchange)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ExchangeReqMsg(rmsg, MSG_EXCHANGE_REQ_REJECT, NULL, NULL);
		do_Exchange(ch, rmsg);
	}
	//대원 모두 요청
	{
		ch->SetPlayerState(PLAYER_STATE_WARP);

		CNetMsg::SP rmsg(new CNetMsg);
		HelperRaidInzoneGetRoomNoReqMsg(rmsg, ch->m_index, nZoneNo, nBossIndex, nJoinType);
		RefMsg(rmsg) << nDifficulty;
		SEND_Q(rmsg, gserver->m_helper);
	}
}

//존에서 나옴(죽음후)
void do_Raid_InzoneQuit(CPC* ch, CNetMsg::SP& msg)
{
	int i;

	int nZoneNo = -1;		//존번호(나갈)
	int nPcZoneNo,nPcRoomNo;
	int nExtra = -1;			//위치 서버에서 지정
	int nGoZone=1;		//존이동 하나?(1:이동 0:이동 안함)
	int nRebirth=0;		//부활인가?(1:부활)
	CZone* pCurZone=NULL;

	if(msg->CanRead(sizeof(nGoZone)))	//데이타가 있을 경우에만 처리
	{
		RefMsg(msg) >> nGoZone
					>> nRebirth
					>> nZoneNo;
		RefMsg(msg) >> nExtra;
	}

	if(ch->IsSetPlayerState(PLAYER_STATE_WARP))
	{
		// 이동할 수 없는 상태입니다.
		CNetMsg::SP rmsg(new CNetMsg);
		SysMsg(rmsg, MSG_SYS_CANT_WARP_OTHER);
		SEND_Q(rmsg, ch->m_desc);
		goto ERROR_RETURN;
	}

	if(nZoneNo >= 0)
	{
		CZone* pGoZone = gserver->FindZone(nZoneNo);
		if (pGoZone == NULL)
			return ;

		if( ch->m_level < pGoZone->m_minLevel || ch->m_level > pGoZone->m_maxLevel )
		{
			CNetMsg::SP rmsg(new CNetMsg);
			SysMsg( rmsg, (MSG_SYS_TYPE) MSG_SYS_CANNOT_GOZONE_LEVEL );
			SEND_Q( rmsg, ch->m_desc );
			return;
		}
	}

	ch->SetPlayerState(PLAYER_STATE_WARP);

	GAMELOG << init("RAID DEBUG INZONE QUIT REQ", ch)
			<< "nGoZone" << delim << nGoZone  << delim
			<< "nRebirth" << delim << nRebirth
			<< end;

	//존번호 ,룸번호 백업
	nPcZoneNo = ch->m_nJoinInzone_ZoneNo;
	nPcRoomNo = ch->m_nJoinInzone_RoomNo;

	pCurZone = gserver->FindZone(ch->m_nJoinInzone_ZoneNo);
	if(pCurZone && pCurZone->m_area)
	{
		i = ch->m_nJoinInzone_AreaNo;
		if(i == -1)
			goto ERROR_RETURN;

		CArea* area;
		area = pCurZone->m_area + i;
		if(!area)
			goto ERROR_RETURN;

		if(ch->m_nJoinInzone_ZoneNo==ZONE_ALTER_OF_DARK)	// 현재 존: 암흑의 재단
		{
			nZoneNo = ZONE_MONDSHINE;

			if(nRebirth==1)
			{
				nExtra = 4;
			}
			else
			{
				nExtra = 3;
			}
		}
		else if(ch->m_nJoinInzone_ZoneNo==ZONE_CAPPELLA_1 && nZoneNo == -1)	// 현재 존: 예배당 1, 2
		{
			nZoneNo = ZONE_MONDSHINE;
			if(nRebirth==1)
			{
				nExtra = 2;
			}
			else
			{
				nExtra = 1;
			}
		}
		else if(ch->m_nJoinInzone_ZoneNo==ZONE_CAPPELLA_1 && nZoneNo != -1)	// 예배당에서 예배당 지하로 간다.
		{
			if(nZoneNo == ZONE_MONDSHINE)
				nExtra = 1;
			else if(nZoneNo == ZONE_CAPPELLA_2)
				nExtra = 0;
			else
				goto ERROR_RETURN;
		}
		else if(ch->m_nJoinInzone_ZoneNo == ZONE_AKAN_TEMPLE)
		{
			if(nExtra == -1)
				nExtra = 4;
			// 아칸사원에서 존을 나갈 때 정보..
			if(nRebirth == 1)
			{
				nZoneNo = ZONE_EGEHA;
//				nExtra = 0;
			}
			else if(nRebirth == 0)
			{
				nZoneNo = ZONE_EGEHA;		// 에게하로 튕겨버려!
//				nExtra = 0;
			}
			else
				goto ERROR_RETURN;
		}
		else if(ch->m_nJoinInzone_ZoneNo == ZONE_TARIAN_DUNGEON)
		{
			if(nExtra == -1)
				nExtra = 1;
			// 아칸사원에서 존을 나갈 때 정보..
			if(nRebirth == 1)
			{
				nZoneNo = ZONE_TARIAN;
//				nExtra = 0;
			}
			else if(nRebirth == 0)
			{
				nZoneNo = ZONE_TARIAN;		// 에게하로 튕겨버려!
//				nExtra = 0;
			}
			else
				goto ERROR_RETURN;
		}
		else
		{
			goto ERROR_RETURN;
		}

		if(ch->IsExped())
		{
			ch->SetExpedLabel(-1);

			{
				CNetMsg::SP rmsg(new CNetMsg);
				ExpedSetLabelRepMsg(rmsg, MSG_EXPED_SETLABEL_TYPE_PC,MSG_EXPED_SETLABEL_MODE_RESET,-1,ch->m_index);
				ch->m_Exped->SendToAllPC(rmsg);
			}

			if(nGoZone==1)
			{
				CZone* pZone = gserver->FindZone(nZoneNo);
				if (pZone == NULL)
					goto ERROR_RETURN;

				if (pZone->m_zonePos == NULL)
					goto ERROR_RETURN;

				if (nExtra < 0 || nExtra >= pZone->m_countZonePos)
					goto ERROR_RETURN;

				//현재 들오온 존, 룸번호 초기화
				CArea* area = NULL;
				
				ch->m_nJoinInzone_ZoneNo = -1;
				ch->m_nJoinInzone_RoomNo = -1;
				ch->m_nJoinInzone_AreaNo = -1;

				GoZone(ch, nZoneNo,
					   pZone->m_zonePos[nExtra][0],													// ylayer
					   GetRandom(pZone->m_zonePos[nExtra][1], pZone->m_zonePos[nExtra][3]) / 2.0f,		// x
					   GetRandom(pZone->m_zonePos[nExtra][2], pZone->m_zonePos[nExtra][4]) / 2.0f);	// z
			}
		}

		// 파티 레이드는 이곳으로 온다.
		else if (ch->IsParty() && nGoZone==1)
		{
			CZone* pZone = gserver->FindZone(nZoneNo);
			if (pZone == NULL)
				goto ERROR_RETURN;

			if (pZone->m_zonePos == NULL)
				goto ERROR_RETURN;

			if (nExtra < 0 || nExtra >= pZone->m_countZonePos)
				goto ERROR_RETURN;

			//현재 들오온 존, 룸번호 초기화
			ch->m_nJoinInzone_ZoneNo = -1;
			ch->m_nJoinInzone_RoomNo = -1;
			ch->m_nJoinInzone_AreaNo = -1;

			GoZone(ch, nZoneNo,
				   pZone->m_zonePos[nExtra][0],													// ylayer
				   GetRandom(pZone->m_zonePos[nExtra][1], pZone->m_zonePos[nExtra][3]) / 2.0f,		// x
				   GetRandom(pZone->m_zonePos[nExtra][2], pZone->m_zonePos[nExtra][4]) / 2.0f);	// z
		}

		// 어떤 것도 아니면 몬드샤인 마을로 간다
		else
		{
			CZone* pZone = gserver->FindZone(nZoneNo);
			if (pZone == NULL)
				goto ERROR_RETURN;

			if (pZone->m_zonePos == NULL)
				goto ERROR_RETURN;

			// 0이 몬드샤인
			nExtra = 0;

			//현재 들오온 존, 룸번호 초기화
			ch->m_nJoinInzone_ZoneNo = -1;
			ch->m_nJoinInzone_RoomNo = -1;
			ch->m_nJoinInzone_AreaNo = -1;

			GoZone(ch, nZoneNo,
				   pZone->m_zonePos[nExtra][0],													// ylayer
				   GetRandom(pZone->m_zonePos[nExtra][1], pZone->m_zonePos[nExtra][3]) / 2.0f,		// x
				   GetRandom(pZone->m_zonePos[nExtra][2], pZone->m_zonePos[nExtra][4]) / 2.0f);	// z
		}
	}

ERROR_RETURN:
	ch->ResetPlayerState(PLAYER_STATE_WARP);
	return;
}

