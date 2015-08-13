#include "stdhdrs.h"

#include "Server.h"
#include "../ShareLib/DBCmd.h"
#include "Log.h"
#include "CmdMsg.h"
#include "Guild.h"
#include "WarCastle.h"

#include "DratanCastle.h"
#include <boost/format.hpp>

CMeracCastle meracCastle;

CWarCastle::CWarCastle()
	: m_ownerGuildName(MAX_GUILD_NAME_LENGTH + 1)
	, m_ownerCharName(MAX_CHAR_NAME_LENGTH + 1)
{
	m_state = WCSF_NORMAL;
	m_castleNPC = NULL;
	m_lordSymbol = NULL;
	m_bRegenShop = false;
	m_lastWarTime = 0;
	m_nextWarTime = 0;

	ResetOwner();

	m_bNoticeWarTime = false;
	m_noticeRemain = 9999;
	m_defensePoint = 0;
	m_gateState = 0;

	m_countAttackGuild = 0;
	m_defenseGuildList = NULL;
	m_defenseCharList = NULL;
	m_attackGuildList = NULL;
	m_attackCharList = NULL;

	m_top3guild[0] = m_top3guild[1] = m_top3guild[2] = NULL;

	m_bGiveLordWeapon = false;
	m_bMoveToRegen = false;
	m_bWarCastleReady = false;

	m_posRegenPoint = NULL;
	m_nRegenPoint = 0;
	m_nSendInterval = 0;
	m_guildGradeSkillTime = 0;
}

CWarCastle::~CWarCastle()
{
	if (m_posRegenPoint)
		delete [] m_posRegenPoint;
	m_posRegenPoint = NULL;
}

void CWarCastle::CheckWarCastle( bool forced_end_merac )
{
	CHECK_SUBSERVER(this);

//	SendCastleState();	// 메라크만제거

	// 공성 시작 알림
	CheckWarCastleNotice();
	// 야전 시작
	CheckWarCastleStartWar();
	// 공성 시작
	CheckWarCastleStartWarCastle();
	// 공성 끝
	CheckWarCastleEnd(forced_end_merac);
	// 공성 NPC 처리
	CheckWarCastleNPC();
	// 수호병 NPC 처리
	RegenGuardNPC();
	// 권좌 생성
	RegenLordSymbol();
	// 상인 생성
	RegenShop();
}

void CWarCastle::SendCastleState( int zoneindex )
{
	if ( GetState() != WCSF_NORMAL)
	{
		// TODO: 루프 시간 체크
		m_nSendInterval++;
		if( m_nSendInterval > 20 )
		{
			int i = 0;
			m_nSendInterval = 0;
			CNetMsg::SP rmsg(new CNetMsg);
			GuildWarGateStateMsg( rmsg, GetGateState(), GetGateState() );

			PCManager::map_t& playerMap			= PCManager::instance()->getPlayerMap();
			PCManager::map_t::iterator iter		= playerMap.begin();
			PCManager::map_t::iterator endIter	= playerMap.end();
			for(; iter != endIter; ++iter)
			{
				CPC* pc = (*iter).pPlayer;
				if (pc == NULL)
				{
					continue;
				}

				if( pc->m_pZone && pc->m_pZone->m_index == zoneindex && pc->m_pZone->IsWarZone( (int)pc->m_pos.m_x, (int)pc->m_pos.m_z ) )
				{
					SEND_Q(rmsg, pc->m_desc);
				}
			}
		}
	}
}

void CWarCastle::CheckWarCastleNPC()
{
	CHECK_SUBSERVER(this);

	switch (m_state)
	{
	case WCSF_NORMAL:
		if (m_castleNPC == NULL)
			RegenCastleNPC();
		break;

	default:
		if (m_castleNPC != NULL)
			RemoveCastleNPC();
		break;
	}
}

void CWarCastle::RegenCastleNPC()
{
	CHECK_SUBSERVER(this);

	CArea* area = GetArea();
	if (!area)
		return ;

	int i;
	for (i = 0; i < area->m_npcRegenList.m_nCount; i++)
	{
		CNPCRegenInfo* p = area->m_npcRegenList.m_infoList + i;
		if (p->m_totalNum != 0)
			continue ;

		CNPCProto* proto = gserver->m_npcProtoList.FindProto(p->m_npcIdx);
		if (proto && proto->CheckFlag(NPC_WARCASTLE))
		{
			m_castleNPC = gserver->m_npcProtoList.Create(proto->m_index, p);
			if (!m_castleNPC)
				return ;

			GET_X(m_castleNPC) = p->m_regenX;
			GET_Z(m_castleNPC) = p->m_regenZ;
			GET_R(m_castleNPC) = p->m_regenR;
			GET_YLAYER(m_castleNPC) = p->m_regenY;

			m_castleNPC->m_regenX = GET_X(m_castleNPC);
			m_castleNPC->m_regenZ = GET_Z(m_castleNPC);
			m_castleNPC->m_regenY = GET_YLAYER(m_castleNPC);

			m_castleNPC->m_recoverPulse = gserver->m_pulse;

			p->m_bAlive = true;
			p->m_lastDieTime = 0;
			m_castleNPC->InitPointsToMax();
			m_castleNPC->m_disableTime = 0;

			m_castleNPC->InitPointsToMax();
			area->AddNPC(m_castleNPC);
			m_castleNPC->m_postregendelay = 0;

			int cx, cz;
			area->PointToCellNum(GET_X(m_castleNPC), GET_Z(m_castleNPC), &cx, &cz);
			area->CharToCell(m_castleNPC, GET_YLAYER(m_castleNPC), cx, cz);

			{
				CNetMsg::SP rmsg(new CNetMsg);
				AppearMsg(rmsg, m_castleNPC, true);
				area->SendToCell(rmsg, m_castleNPC);
			}

			GAMELOG << init("CASTLE NPC REGEN")
					<< "ZONE" << delim
					<< GetZoneIndex()
					<< end;
		}
	}
}

void CWarCastle::RemoveCastleNPC()
{
	CHECK_SUBSERVER(this);

	if (m_castleNPC == NULL)
		return ;
	CArea* area = m_castleNPC->m_pArea;
	if (!area)
	{
		DelAttackList(m_castleNPC);
		delete m_castleNPC;
	}
	else
	{
		m_castleNPC->SendDisappearAllInCell(true);
		DelAttackList(m_castleNPC);
		area->CharFromCell(m_castleNPC, true);
		area->DelNPC(m_castleNPC);
	}
	m_castleNPC = NULL;
}

bool CWarCastle::LoadCastleData()
{
	GAMELOG << init("LOADING CASTLE DATA")
			<< "ZONE" << delim
			<< GetZoneIndex() << delim
			<< end;

	CDBCmd cmd;
	cmd.Init(&gserver->m_dbcastle);

	// 성 정보 읽기
	std::string select_castle = boost::str(boost::format("SELECT * FROM t_castle WHERE a_zone_index=%d") % GetZoneIndex());
	cmd.SetQuery(select_castle);

	if (!cmd.Open())	return false;

	if (cmd.MoveFirst())
	{
		if (!cmd.GetRec("a_last_war_time", m_lastWarTime))
			return false;
		if (!cmd.GetRec("a_state", m_state))
			return false;
		int nextWarTime;
		if (!cmd.GetRec("a_next_war_time", nextWarTime))
			return false;
		SetNextWarTime(nextWarTime);
		if (!cmd.GetRec("a_owner_guild_index", m_ownerGuildIndex))
			return false;
		if (!cmd.GetRec("a_owner_guild_name", m_ownerGuildName))
			return false;
		if (!cmd.GetRec("a_owner_char_index", m_ownerCharIndex))
			return false;
		if (!cmd.GetRec("a_owner_char_name", m_ownerCharName))
			return false;
	}
	else
	{
		std::string insert_castle = boost::str(boost::format("INSERT INTO t_castle (a_zone_index) VALUES (%d)") % GetZoneIndex());
		cmd.SetQuery(insert_castle);
		if (!cmd.Update())
			return false;
		SetNextWarTime(0);
	}
	int nNextWarTime = GetNextWarTime();
	struct tm tmNextWarTime;
	GetNextWarTime(&tmNextWarTime, true);
	std::string update_castle_query = boost::str(boost::format("UPDATE t_castle SET a_next_war_time=%d WHERE a_zone_index=%d") % nNextWarTime % GetZoneIndex());
	cmd.SetQuery(update_castle_query);
	cmd.Update();
	GAMELOG << init("WAR CASTLE : SAVE NEXT WAR TIME")	<< delim
			<< "UNIX"	<< delim	<< nNextWarTime		<< delim
			<< "TIME"	<< delim	<< tmNextWarTime.tm_year << delim << tmNextWarTime.tm_mon << delim << tmNextWarTime.tm_mday << delim << tmNextWarTime.tm_hour
			<< end;

	// 공성 신청인 정보 읽기
	std::string select_castle_join_query = boost::str(boost::format("SELECT * FROM t_castle_join WHERE a_zone_index=%d") % GetZoneIndex());
	cmd.SetQuery(select_castle_join_query);

	if (!cmd.Open())	return false;

	while (cmd.MoveNext())
	{
		int index;
		char guild;
		char attack;

		if (!cmd.GetRec("a_index", index))		return false;
		if (!cmd.GetRec("a_guild", guild))		return false;
		if (!cmd.GetRec("a_attack", attack))	return false;

		if (attack)
		{
			if (guild)
				AddAttackGuild(index);
			else if (GetZoneIndex() == ZONE_MERAC)			// 메라크만 용병이 있다.
				AddAttackChar(index);
		}
		else
		{
			if (guild)
				AddDefenseGuild(index);
			else if (GetZoneIndex() == ZONE_MERAC)			// 메라크만 용병이 있다.
				AddDefenseChar(index);
		}
	}

	return true;
}

void CWarCastle::GetNextWarTime(struct tm* nextWarTime, bool bHumanable)
{
	time_t tNext = (time_t)GetNextWarTime();
	memcpy(nextWarTime, localtime(&tNext), sizeof(struct tm));
	if (bHumanable)
	{
		nextWarTime->tm_year += 1900;
		nextWarTime->tm_mon += 1;
	}
}

CWarCastle* CWarCastle::GetCastleObject(int zoneindex)
{
	if (gserver->FindZone(zoneindex) == NULL)
	{
		// 해당 존이 없으면
		return NULL;
	}

	switch (zoneindex)
	{
	case ZONE_MERAC:
		return &meracCastle;
	case ZONE_DRATAN:
		return  CDratanCastle::CreateInstance();
	}

	return NULL;
}

bool CWarCastle::AddDefenseChar(int index)
{
	CWarCastleJoin* p = new CWarCastleJoin(index, 0);
	if (m_defenseCharList == NULL)
	{
		m_defenseCharList = p;
	}
	else
	{
		p->m_next = m_defenseCharList;
		m_defenseCharList->m_prev = p;
		m_defenseCharList = p;
	}
	return true;
}

bool CWarCastle::AddAttackChar(int index)
{
	CWarCastleJoin* p = new CWarCastleJoin(index, 1);
	if (m_attackCharList == NULL)
	{
		m_attackCharList = p;
	}
	else
	{
		p->m_next = m_attackCharList;
		m_attackCharList->m_prev = p;
		m_attackCharList = p;
	}
	return true;
}

bool CWarCastle::AddAttackGuild(int index)
{
	CWarCastleJoin* p = new CWarCastleJoin(index, 1);
	if (m_attackGuildList == NULL)
	{
		m_attackGuildList = p;
	}
	else
	{
		p->m_next = m_attackGuildList;
		m_attackGuildList->m_prev = p;
		m_attackGuildList = p;
	}

	m_countAttackGuild++;

	return true;
}

bool CWarCastle::AddDefenseGuild(int index)
{
	CWarCastleJoin* p = new CWarCastleJoin(index, 1);
	if (m_defenseGuildList == NULL)
	{
		m_defenseGuildList = p;
	}
	else
	{
		p->m_next = m_defenseGuildList;
		m_defenseGuildList->m_prev = p;
		m_defenseGuildList = p;
	}

	return true;
}

bool CWarCastle::IsAttackGuild(int index)
{
	return (FindAttackGuild(index)) ? true : false;
}

bool CWarCastle::IsAttackChar(int index)
{
	return (FindAttackChar(index)) ? true : false;
}

bool CWarCastle::IsDefenseChar(int index)
{
	return (FindDefenseChar(index)) ? true : false;
}

bool CWarCastle::IsDefenseGuild(int index)
{
	return (FindDefenseGuild(index)) ? true : false;
}

void CWarCastle::CheckWarCastleNotice()
{
	switch (GetState())
	{
	case WCSF_NORMAL:
		// 확정 알리는 것인지 아니면 시작 10분전에 알리는 것인지 검사
		if (!m_bNoticeWarTime)
		{
			// 공성 신청 가능 시간대이고
			if (!IsJoinTime())
				return ;

			// 게임내 모든 유저에게 알리기
			{
				struct tm nextWarTime;
				GetNextWarTime(&nextWarTime, true);

				CNetMsg::SP rmsg(new CNetMsg);
				HelperWarNoticeTimeMsg(rmsg, GetZoneIndex(), nextWarTime.tm_mon, nextWarTime.tm_mday, nextWarTime.tm_hour, nextWarTime.tm_min);
				SEND_Q(rmsg, gserver->m_helper);
				m_bNoticeWarTime = true;
			}
		}
		else
		{
			struct tm nextWarTime;
			GetNextWarTime(&nextWarTime, true);
			int nexttime = GetNextWarTime();
			time_t curtime;
			time(&curtime);

			// 시작 10분전에 매분마다
			if (curtime + 10 * 60 >= nexttime)
			{
				// 남은 시간 구하기
				int remain = (nexttime - curtime) / 60;
				int modnum = (nexttime - curtime) % 60;
				if (modnum)
					remain++;
				if (remain != 0 && m_noticeRemain > remain)
				{
					{
						CNetMsg::SP rmsg(new CNetMsg);
						HelperWarNoticeRemainMsg(rmsg, GetZoneIndex(), remain);
						SEND_Q(rmsg, gserver->m_helper);
					}

					m_noticeRemain = remain;

					// 길드전 중지 요청
					if (curtime + 60 >= nexttime)
					{
						// 서버내의 길드 리스트를 돌면서
						// 길드전 중이면 헬퍼에 요청
						CGuild* p;
						for (p = gserver->m_guildlist.head(); p; p = p->nextguild())
						{
							if (p->battleZone() == GetZoneIndex())
							{
								switch (p->battleState())
								{
								case GUILD_BATTLE_STATE_WAIT:
								case GUILD_BATTLE_STATE_ING:
								case GUILD_BATTLE_STATE_STOP_WAIT:
									{
										CNetMsg::SP rmsg(new CNetMsg);
										HelperGuildBattleStopReqMsg(rmsg, p->index(), p->battleIndex());
										SEND_Q(rmsg, gserver->m_helper);
									}
									break;
								} // switch (p->battleState())
							} // if (p->battleZone() == GetZoneIndex())
						} // for (p = gserver->m_guildlist.head(); p; p = p->nextguild())
					} // if (curtime + 60 >= nexttime)
				} // if (remain != 0 && m_noticeRemain > remain && gserver->isRunHelper())
			} // if (curtime + 10 * 60 >= nexttime)

			// 시작 3분전 이동
			if (!m_bMoveToRegen && curtime + 3 * 60 >= nexttime)
			{
				// 리젠 포인트 할당
				InitRegenPosition();

				// 강제 존이동 시키기
				m_bMoveToRegen = true;
				MoveToRegenPoint();
			}

			// 공성전 시작 1분 전부터는 메모리 스크롤을 사용할 수 없도록 하기 위해..
			if (!m_bWarCastleReady && (curtime + 1 * 60) >= nexttime)
			{
				m_bWarCastleReady = true;
			}
		}
		break;

	case WCSF_WAR_FIELD:
		{
			struct tm nextWarTime;
			GetNextWarTime(&nextWarTime, true);
			int nexttime = GetNextWarTime();
			time_t curtime;
			time(&curtime);

			// 시작시간 + 필드 전투 시간 - 5분부터 1분에 한번씩 메시지 알림
			int remainSec = WCT_FIELD - (curtime - nexttime);
			int remainMin = remainSec / 60;
			int modnum = (remainSec) % 60;
			if (modnum)
				remainMin++;

			if (remainMin > 0 && remainMin <= 5 && m_noticeRemain > remainMin)
			{
				CNetMsg::SP rmsg(new CNetMsg);
				HelperWarNoticeRemainFieldTimeMsg(rmsg, GetZoneIndex(), GetRemainWarTime());
				SEND_Q(rmsg, gserver->m_helper);
				m_noticeRemain = remainMin;
			}
			if (m_bWarCastleReady)
				m_bWarCastleReady = false;
		}
		break;

	case WCSF_WAR_CASTLE:
		{
			if (m_bWarCastleReady)
				m_bWarCastleReady = false;
		}
		break;
	}
}

void CWarCastle::CheckWarCastleStartWar()
{
	// 일반 상태이여야 하고
	if (GetState() != WCSF_NORMAL)
		return ;

	// 공성 시작 시간이상이어야 하고
	time_t curTime;
	time(&curTime);
	int nextTime = GetNextWarTime();
	if (nextTime > curTime)
		return ;

	// 알림 메시지 시간 저장 변수를 초기화
	m_noticeRemain = 9999;

	// 공성 신청 길드가 없으면 다음으로 넘어감
	if (m_attackGuildList == NULL)
	{
		SetState(WCSF_WAR_CASTLE);
		GAMELOG << init("WAR CASTLE : SKIP WAR") << GetZoneIndex() << end;
		return ;
	}

	// 수성측제외 성에서 내보내기
	GetOutNonDefense();

	// 상태를 바꾸고
	SetState(WCSF_WAR_FIELD);

	RegenWarpNPC();

	// 공성 신청 길드가 3개 이하이면 야전 스킵
	if (GetCountAttackGuild() < 4)
	{
		GAMELOG << init("WAR CASTLE : SKIP FIELD") << GetZoneIndex() << end;
		return ;
	}

	GAMELOG << init("WAR CASTLE : START FIELD") << GetZoneIndex() << end;

	// 야전 시작을 헬퍼를 통해 알리기
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperWarNoticeStartMsg(rmsg, GetZoneIndex(), GetRemainWarTime());
		SEND_Q(rmsg, gserver->m_helper);
	}
}

void CWarCastle::CheckWarCastleStartWarCastle()
{
	// 야전 상태 검사
	if (GetState() != WCSF_WAR_FIELD)
		return ;

	// 공성 길드가 4개 이상되었거나
	bool bFinishField = false;
	// 공성 신청 길드가 3개 이하이면 야전 스킵
	if (GetCountAttackGuild() < 4)
		bFinishField = true;

	if (!bFinishField)
	{
		// 시간이 15분 이상 지나면
		time_t curTime;
		time(&curTime);
		int nextTime = GetNextWarTime();
		if (curTime > nextTime + WCT_FIELD)
			bFinishField = true;
	}

	if (!bFinishField)
		return ;

	// 상태 변경
	SetState(WCSF_WAR_CASTLE);

	// 최상위 3개 길드 구함
	GetTop3AttackGuild();

	GAMELOG << init("WAR CASTLE : START WAR")
			<< GetZoneIndex();
	int i;
	for (i = 0; i < 3; i++)
	{
		if (m_top3guild[i])
		{
			GAMELOG << delim << m_top3guild[i]->GetIndex() << delim << m_top3guild[i]->GetPoint();
		}
		else
		{
			GAMELOG << delim << -1 << delim << 0;
		}
	}
	GAMELOG << end;

	// 공성 길드 포인트 초기화
	CWarCastleJoin* p = m_attackGuildList;
	while (p)
	{
		for (i = 0; i < 3; i++)
		{
			if (m_top3guild[i] == p)
			{
				p->m_point = 0;
				break;
			}
		}
		if (i == 3)
			p->m_point = -1;
		p = p->m_next;
	}

	// 알림 메시지 시간 저장 변수를 초기화
	m_noticeRemain = 9999;

	// 근위병 해제
	RemoveGuardNPC();

	ApplyGuildGradeSkillAll();

	// 공성 시작을 헬퍼를 통해 알리기
	if (gserver->isRunHelper())
	{
		CGuild* guild[3] = { NULL, NULL, NULL };
		for (i = 0; i < 3; i++)
		{
			if (m_top3guild[i])
				guild[i] = gserver->m_guildlist.findguild(m_top3guild[i]->GetIndex());
		}

		{
			CNetMsg::SP rmsg(new CNetMsg);
			HelperWarNoticeStartAttackCastleMsg(rmsg, GetZoneIndex(), GetRemainWarTime(), guild[0], guild[1], guild[2]);
			SEND_Q(rmsg, gserver->m_helper);
		}
	}

	// 수호탑 생성
	RegenCastleTower();

	// 강제 존이동 시키기
	MoveToRegenPoint();
}

void CWarCastle::CheckJoinAll(CPC* ch)
{
	ch->SetJoinFlag(ZONE_MERAC, meracCastle.CheckJoin(ch));
	ch->SetJoinFlag(ZONE_DRATAN, CDratanCastle::CreateInstance()->CheckJoin(ch));
}

int CWarCastle::CheckJoin(CPC* ch)
{
	if (ch->m_guildInfo)
	{
		CGuild* guild = ch->m_guildInfo->guild();
		// 성주 길드 검사
		if (guild->index() == GetOwnerGuildIndex())
			return WCJF_OWNER;
		// 수성 길드 검사
		if (IsDefenseGuild(guild->index()))
			return WCJF_DEFENSE_GUILD;
		// 공성 길드 검사
		if (IsAttackGuild(guild->index()))
			return WCJF_ATTACK_GUILD;
	}

	// 수성 용병 검사
	if (IsDefenseChar(ch->m_index))
		return WCJF_DEFENSE_CHAR;
	// 공성 용병 검사
	if (IsAttackChar(ch->m_index))
		return WCJF_ATTACK_CHAR;

	return WCJF_NONE;
}

void CWarCastle::MoveToRegenPoint()
{
	CZone* pZone = gserver->FindZone(GetZoneIndex());
	if (pZone == NULL)
		return;

	PCManager::map_t& playerMap			= PCManager::instance()->getPlayerMap();
	PCManager::map_t::iterator iter		= playerMap.begin();
	PCManager::map_t::iterator endIter	= playerMap.end();
	for (; iter != endIter; ++iter)
	{
		CPC* pc = (*iter).pPlayer;
		if(pc == NULL || pc->m_pZone->m_index != GetZoneIndex())
		{
			continue;
		}

		int extra = GetRegenPoint(pc->GetJoinFlag(GetZoneIndex()), pc);
		if (extra == 0)
		{
			continue;
		}
#ifdef WARCASTLE_MOVE_MESSAGE_LIMIT
		if( !( pc->m_pZone->m_index == GetZoneIndex()
				&& pc->m_pZone->InExtra( (int)GET_X(pc), (int)GET_Z(pc) ,extra)  )  )
#endif
		{
			// 확인 메시지 전송
			CNetMsg::SP rmsg(new CNetMsg);
			WarpPromptMsg(rmsg, pZone->m_index, extra);
			SEND_Q(rmsg, pc->m_desc);
		}
	}
}

void CWarCastle::GetOutNonDefense()
{
	CZone*		pZone	= gserver->FindZone(GetZoneIndex());
	if (pZone == NULL)
		return;

	CArea*		pArea	= pZone->m_area;

	char		nYlayer	= 0;
	int			nX0x2	= 0;
	int			nZ0x2	= 0;
	int			nX1x2	= 0;
	int			nZ1x2	= 0;
	GetInnerCastleRect(&nYlayer, &nX0x2, &nZ0x2, &nX1x2, &nZ1x2);

	int			nCellX0	= 0;
	int			nCellZ0	= 0;
	int			nCellX1	= 0;
	int			nCellZ1	= 0;
	pArea->PointToCellNum(nX0x2 / 2.0f, nZ0x2 / 2.0f, &nCellX0, &nCellZ0);
	pArea->PointToCellNum(nX1x2 / 2.0f, nZ1x2 / 2.0f, &nCellX1, &nCellZ1);

	// 먼저 이동시킬 유저를 선별후 이동 // 다른셀로 이동시 또는 이동중 삭제시 문제 발생

	std::vector<CPC*> m_MovePCList;

	int cx, cz;
	for (cx = nCellX0; cx <= nCellX1; cx++)
	{
		if (cx < 0 || cx >= pArea->m_size[0])
			continue ;

		for (cz = nCellZ0; cz <= nCellZ1; cz++)
		{
			if (cz < 0 || cz >= pArea->m_size[1])
				continue ;

			CCell& rCell = pArea->m_cell[cx][cz];
			CCharacter* pCharNext = rCell.m_listChar;
			CCharacter* pChar;
			while ((pChar = pCharNext))
			{
				pCharNext = pCharNext->m_pCellNext;
				if (IS_PC(pChar) && (-1 != pChar->m_index))
				{
					CPC* pPC = TO_PC(pChar);
					if (pPC)
					{
						m_MovePCList.push_back( pPC );
					}
				}
			}
		}
	}

	std::vector<CPC*>::iterator it = m_MovePCList.begin();
	std::vector<CPC*>::iterator endit = m_MovePCList.end();
	for(; it != endit; ++it)
	{
		CPC* movePc = *(it);
		int nJoinFlag = movePc->GetJoinFlag(pZone->m_index);
		if (IsInInnerCastle(movePc))
		{
			int extra = GetRegenPoint(nJoinFlag, movePc);
			if (extra == 0)
				extra = 3;

			GoZone(movePc, pZone->m_index,
				   pZone->m_zonePos[extra][0],
				   GetRandom(pZone->m_zonePos[extra][1], pZone->m_zonePos[extra][3]) / 2.0f,
				   GetRandom(pZone->m_zonePos[extra][2], pZone->m_zonePos[extra][4]) / 2.0f);
			movePc->SetDisableTime(1);
		}
	}
	m_MovePCList.clear();
}

void CWarCastle::RemoveDefenseChar(int index)
{
	CWarCastleJoin* p = FindDefenseChar(index);
	if (p)
	{
		if (p->m_prev)
			p->m_prev->m_next = p->m_next;
		if (p->m_next)
			p->m_next->m_prev = p->m_prev;
		if (m_defenseCharList == p)
			m_defenseCharList = p->m_next;
		delete p;
		p = NULL;
	}
}

void CWarCastle::RemoveDefenseGuild(int index)
{
	CWarCastleJoin* p = FindDefenseGuild(index);
	if (p)
	{
		if (p->m_prev)
			p->m_prev->m_next = p->m_next;
		if (p->m_next)
			p->m_next->m_prev = p->m_prev;
		if (m_defenseGuildList == p)
			m_defenseGuildList = p->m_next;
		delete p;
		p = NULL;
	}
}

void CWarCastle::RemoveAttackChar(int index)
{
	CWarCastleJoin* p = FindAttackChar(index);
	if (p)
	{
		if (p->m_prev)
			p->m_prev->m_next = p->m_next;
		if (p->m_next)
			p->m_next->m_prev = p->m_prev;
		if (m_attackCharList == p)
			m_attackCharList = p->m_next;
		delete p;
		p = NULL;
	}
}

void CWarCastle::RemoveAttackGuild(int index)
{
	CWarCastleJoin* p = FindAttackGuild(index);
	if (p)
	{
		if (p->m_prev)
			p->m_prev->m_next = p->m_next;
		if (p->m_next)
			p->m_next->m_prev = p->m_prev;
		if (m_attackGuildList == p)
			m_attackGuildList = p->m_next;
		delete p;
		p = NULL;
		m_countAttackGuild--;
	}
}

CArea* CWarCastle::GetArea()
{
	CZone* pZone = gserver->FindZone(GetZoneIndex());
	if (pZone == NULL)
		return NULL;
	else
		return pZone->m_area;
}

void CWarCastle::GetTop3AttackGuild()
{
	m_top3guild[0] = m_top3guild[1] = m_top3guild[2] = NULL;

	int i;
	int j;
	CWarCastleJoin* p = m_attackGuildList;
	while (p)
	{
		for (i = 0; i < 3; i++)
		{
			if (m_top3guild[i] == NULL)
			{
				m_top3guild[i] = p;
				break;
			}
			else
			{
				if (m_top3guild[i]->GetPoint() <= p->GetPoint())
				{
					for (j = 2; j > i; j--)
					{
						m_top3guild[j] = m_top3guild[j - 1];
					}
					m_top3guild[i] = p;
					break;
				}
			}
		}
		p = p->m_next;
	}
}

void CWarCastle::RegenGuardNPC()
{
	CHECK_SUBSERVER(this);

	if (GetState() != WCSF_WAR_FIELD)
		return ;

	CArea* area = GetArea();
	if (!area)
		return ;

	int regencount = 0;

	int i;
	for (i = 0; i < area->m_npcRegenList.m_nCount; i++)
	{
		CNPCRegenInfo* p = area->m_npcRegenList.m_infoList + i;
		if (p->m_totalNum != 0 || p->m_bAlive || (gserver->m_pulse - p->m_lastDieTime < p->m_regenSec))
			continue ;

		CNPCProto* proto = gserver->m_npcProtoList.FindProto(p->m_npcIdx);
		if (proto && proto->CheckFlag(NPC_CASTLE_GUARD))
		{
			CNPC* npc = gserver->m_npcProtoList.Create(proto->m_index, p);
			if (!npc)
				return ;

			GET_X(npc) = p->m_regenX;
			GET_Z(npc) = p->m_regenZ;
			GET_R(npc) = p->m_regenR;
			GET_YLAYER(npc) = p->m_regenY;

			npc->m_regenX = GET_X(npc);
			npc->m_regenZ = GET_Z(npc);
			npc->m_regenY = GET_YLAYER(npc);

			npc->m_recoverPulse = gserver->m_pulse;

			p->m_bAlive = true;
			p->m_lastDieTime = 0;
			npc->InitPointsToMax();
			npc->m_disableTime = 0;

			npc->InitPointsToMax();
			area->AddNPC(npc);
			npc->m_postregendelay = 0;

			int cx, cz;
			area->PointToCellNum(GET_X(npc), GET_Z(npc), &cx, &cz);
			area->CharToCell(npc, GET_YLAYER(npc), cx, cz);

			{
				CNetMsg::SP rmsg(new CNetMsg);
				AppearMsg(rmsg, npc, true);
				area->SendToCell(rmsg, npc);
			}

			regencount++;
		}
	}

	if (regencount > 0)
	{
		GAMELOG << init("GUARD NPC REGEN")
				<< "ZONE" << delim
				<< GetZoneIndex() << delim
				<< "COUNT" << delim
				<< regencount
				<< end;
	}
}

void CWarCastle::RegenCastleTower()
{
	CHECK_SUBSERVER(this);

	if (GetState() != WCSF_WAR_CASTLE)
		return ;

	CArea* area = GetArea();
	if (!area)
		return ;

	int regencount = 0;

	int i;
	for (i = 0; i < area->m_npcRegenList.m_nCount; i++)
	{
		CNPCRegenInfo* p = area->m_npcRegenList.m_infoList + i;
		if (p->m_totalNum != 0 || p->m_bAlive || (gserver->m_pulse - p->m_lastDieTime < p->m_regenSec))
			continue ;

		CNPCProto* proto = gserver->m_npcProtoList.FindProto(p->m_npcIdx);
		if (proto && proto->CheckFlag(NPC_CASTLE_TOWER))
		{
			// 리젠 정보에 수성측 포인트 설정
			if (m_defensePoint > 0)
				p->SetHPParam(m_defensePoint * 100);
			else
				p->SetHPParam(0);

			CNPC* npc = gserver->m_npcProtoList.Create(proto->m_index, p);
			if (!npc)
				return ;

			GET_X(npc) = p->m_regenX;
			GET_Z(npc) = p->m_regenZ;
			GET_R(npc) = p->m_regenR;
			GET_YLAYER(npc) = p->m_regenY;

			npc->m_regenX = GET_X(npc);
			npc->m_regenZ = GET_Z(npc);
			npc->m_regenY = GET_YLAYER(npc);

			npc->m_recoverPulse = gserver->m_pulse;

			p->m_bAlive = true;
			p->m_lastDieTime = 0;
			npc->InitPointsToMax();
			npc->m_disableTime = 0;

			npc->InitPointsToMax();
			area->AddNPC(npc);
			npc->m_postregendelay = 0;

			int cx, cz;
			area->PointToCellNum(GET_X(npc), GET_Z(npc), &cx, &cz);
			area->CharToCell(npc, GET_YLAYER(npc), cx, cz);

			{
				CNetMsg::SP rmsg(new CNetMsg);
				AppearMsg(rmsg, npc, true);
				area->SendToCell(rmsg, npc);
			}

			// 성문지기 NPC 설정
			SetGateNPC(npc);

			regencount++;
		}
	}

	if (regencount > 0)
	{
		GAMELOG << init("CASTLE TOWER REGEN")
				<< "ZONE" << delim
				<< GetZoneIndex() << delim
				<< "COUNT" << delim
				<< regencount
				<< end;
	}
}

void CWarCastle::RegenLordSymbol()
{
	CHECK_SUBSERVER(this);

	if (m_lordSymbol != NULL || GetState() != WCSF_WAR_CASTLE)
		return ;

	CArea* area = GetArea();
	if (!area)
		return ;

	int i;
	for (i = 0; i < area->m_npcRegenList.m_nCount; i++)
	{
		CNPCRegenInfo* p = area->m_npcRegenList.m_infoList + i;
		if (p->m_totalNum != 0 || p->m_bAlive || (gserver->m_pulse - p->m_lastDieTime < p->m_regenSec))
			continue ;

		CNPCProto* proto = gserver->m_npcProtoList.FindProto(p->m_npcIdx);
		if (proto && proto->CheckFlag(NPC_LORD_SYMBOL))
		{
			m_lordSymbol = gserver->m_npcProtoList.Create(proto->m_index, p);
			if (!m_lordSymbol)
				return ;

			GET_X(m_lordSymbol) = p->m_regenX;
			GET_Z(m_lordSymbol) = p->m_regenZ;
			GET_R(m_lordSymbol) = p->m_regenR;
			GET_YLAYER(m_lordSymbol) = p->m_regenY;

			m_lordSymbol->m_regenX = GET_X(m_lordSymbol);
			m_lordSymbol->m_regenZ = GET_Z(m_lordSymbol);
			m_lordSymbol->m_regenY = GET_YLAYER(m_lordSymbol);

			m_lordSymbol->m_recoverPulse = gserver->m_pulse;

			p->m_bAlive = true;
			p->m_lastDieTime = 0;
			m_lordSymbol->InitPointsToMax();
			m_lordSymbol->m_disableTime = 0;

			m_lordSymbol->InitPointsToMax();
			area->AddNPC(m_lordSymbol);
			m_lordSymbol->m_postregendelay = 0;

			int cx, cz;
			area->PointToCellNum(GET_X(m_lordSymbol), GET_Z(m_lordSymbol), &cx, &cz);
			area->CharToCell(m_lordSymbol, GET_YLAYER(m_lordSymbol), cx, cz);

			{
				CNetMsg::SP rmsg(new CNetMsg);
				AppearMsg(rmsg, m_lordSymbol, true);
				area->SendToCell(rmsg, m_lordSymbol);
			}

			GAMELOG << init("LORD SYMBOL REGEN")
					<< "ZONE" << delim
					<< GetZoneIndex()
					<< end;

			// TODO : 권좌를 2개이상 설치시 break 제거 및 변수 수정
			break;
		}
	}
}

void CWarCastle::RegenShop()
{
	CHECK_SUBSERVER(this);

	time_t curtime;
	time(&curtime);
	int nexttime = GetNextWarTime();

	// 시작 10분전에 생성
	if (!m_bRegenShop && curtime + 10 * 60 >= nexttime)
	{
		m_bRegenShop = true;

		CArea* area = GetArea();
		if (!area)
			return ;

		int i;
		for (i = 0; i < area->m_zone->m_nShopCount; i++)
		{
			if ( !(area->GetAttr(area->m_zone->m_shopList[i].m_yLayer, area->m_zone->m_shopList[i].m_x, area->m_zone->m_shopList[i].m_z) & MATT_WAR) )
				continue ;

			CNPC* npc = gserver->m_npcProtoList.Create(area->m_zone->m_shopList[i].m_keeperIdx, NULL);
			if (!npc)
				return ;

			GET_X(npc) = area->m_zone->m_shopList[i].m_x;
			GET_Z(npc) = area->m_zone->m_shopList[i].m_z;
			GET_R(npc) = area->m_zone->m_shopList[i].m_r;
			GET_YLAYER(npc) = area->m_zone->m_shopList[i].m_yLayer;

			npc->m_regenX = GET_X(npc);
			npc->m_regenZ = GET_Z(npc);
			npc->m_regenY = GET_YLAYER(npc);

			npc->m_recoverPulse = gserver->m_pulse;

			npc->InitPointsToMax();
			npc->m_disableTime = 0;

			npc->InitPointsToMax();
			area->AddNPC(npc);
			npc->m_postregendelay = 0;

			int cx, cz;
			area->PointToCellNum(GET_X(npc), GET_Z(npc), &cx, &cz);
			area->CharToCell(npc, GET_YLAYER(npc), cx, cz);

			{
				CNetMsg::SP rmsg(new CNetMsg);
				AppearMsg(rmsg, npc, true);
				area->SendToCell(rmsg, npc);
			}

			GAMELOG << init("CASTLE SHOP REGEN")
					<< "ZONE" << delim
					<< GetZoneIndex() << delim
					<< "SHOP" << delim
					<< npc->m_name << delim
					<< GET_X(npc) << delim
					<< GET_Z(npc)
					<< end;
		}
	}
	else if (m_bRegenShop && curtime + 10 * 60 < nexttime)
	{
		m_bRegenShop = false;

		CArea* area = GetArea();
		if (!area)
			return ;

		CNPC* pNext = area->m_npcList;
		CNPC* p;
		while ((p = pNext))
		{
			pNext = p->m_pNPCNext;
			if (p->m_proto->CheckFlag(NPC_SHOPPER) && p->GetMapAttr() & MATT_WAR)
			{
				p->SendDisappearAllInCell(true);

				// 어택리스트에서 삭제
				DelAttackList(p);

				// npc 삭제
				area->CharFromCell(p, true);
				area->DelNPC(p);
			}
		}
	}
}

int CWarCastle::GetTeamPoint(int guildindex)
{
	if (guildindex == 0)
	{
		return m_defensePoint;
	}
	else
	{
		CWarCastleJoin* p = FindAttackGuild(guildindex);
		if (p)
			return p->GetPoint();
	}
	return 0;
}

int CWarCastle::IncreaseTeamPoint(int guildindex, int val)
{
	if (guildindex == 0)
	{
		m_defensePoint += val;
		GAMELOG << init("WAR CASTLE : INCREASE POINT")
				<< "OWNER" << delim
				<< val << delim
				<< m_defensePoint
				<< end;
		return m_defensePoint;
	}
	else
	{
		CWarCastleJoin* p = FindAttackGuild(guildindex);
		if (p)
		{
			p->IncreasePoint(val);
			GAMELOG << init("WAR CASTLE : INCREASE POINT")
					<< guildindex << delim
					<< val << delim
					<< p->GetPoint()
					<< end;
			return p->GetPoint();
		}
	}
	return 0;
}

int CWarCastle::GetRemainWarTime()
{
	int startTime = GetNextWarTime();
	int endTime = startTime + WCT_WAR;
	time_t curTime;
	time(&curTime);

	return endTime - curTime;
}

void CWarCastle::GetTop3AttackGuild(int guildindex[3], char guildname[3][MAX_GUILD_NAME_LENGTH + 1], int guildpoint[3])
{
	GetTop3AttackGuild();

	int i;
	for (i = 0; i < 3; i++)
	{
		CGuild* guild = NULL;
		if (m_top3guild[i])
		{
			guild = gserver->m_guildlist.findguild(m_top3guild[i]->GetIndex());
		}
		if (guild)
		{
			guildindex[i] = guild->index();
			strcpy(guildname[i], guild->name());
			guildpoint[i] = m_top3guild[i]->GetPoint();
		}
		else
		{
			guildindex[i] = -1;
			guildname[i][0] = '\0';
			guildpoint[i] = 0;
		}
	}
}

int CWarCastle::GetGuildPoint(int guildindex)
{
	if (GetState() == WCSF_WAR_FIELD)
	{
		// 수성측은 수성 포인트
		if (guildindex == GetOwnerGuildIndex() || IsDefenseGuild(guildindex))
			return m_defensePoint;
		else
		{
			// 공성측인지 검사하여 공성측은 길드 포인트
			CWarCastleJoin* p = FindAttackGuild(guildindex);
			if (p)
				return p->GetPoint();
			else
				return -1;
		}
	}
	else if (GetState() == WCSF_WAR_CASTLE)
	{
		// 수성측은 없음
		if (guildindex == GetOwnerGuildIndex() || IsDefenseGuild(guildindex))
			return -1;
		else
		{
			// 공성측인지 검사하여 공성측은 길드 포인트
			CWarCastleJoin* p = FindAttackGuild(guildindex);
			if (p)
				return p->GetPoint();
			else
				return -1;
		}
	}
	else
		return -1;
}

CWarCastleJoin* CWarCastle::FindAttackGuild(int index)
{
	CWarCastleJoin* p = m_attackGuildList;
	while (p)
	{
		if (p->m_index == index)
			return p;
		else
			p = p->m_next;
	}
	return NULL;
}

CWarCastleJoin* CWarCastle::FindAttackChar(int index)
{
	CWarCastleJoin* p = m_attackCharList;
	while (p)
	{
		if (p->m_index == index)
			return p;
		else
			p = p->m_next;
	}
	return NULL;
}

CWarCastleJoin* CWarCastle::FindDefenseGuild(int index)
{
	CWarCastleJoin* p = m_defenseGuildList;
	while (p)
	{
		if (p->m_index == index)
			return p;
		else
			p = p->m_next;
	}
	return NULL;
}

CWarCastleJoin* CWarCastle::FindDefenseChar(int index)
{
	CWarCastleJoin* p = m_defenseCharList;
	while (p)
	{
		if (p->m_index == index)
			return p;
		else
			p = p->m_next;
	}
	return NULL;
}

void CWarCastle::RemoveGuardNPC()
{
	CHECK_SUBSERVER(this);

	CArea* area = GetArea();
	if (!area)
		return ;

	CNPC* pNext = area->m_npcList;
	CNPC* p;
	while ((p = pNext))
	{
		pNext = p->m_pNPCNext;
		if (p->m_proto->CheckFlag(NPC_CASTLE_GUARD))
		{
			p->SendDisappearAllInCell(true);

			// 어택리스트에서 삭제
			DelAttackList(p);

			// npc 삭제
			area->CharFromCell(p, true);
			area->DelNPC(p);
		}
	}
}

void CWarCastle::RemoveCastleTower()
{
	CHECK_SUBSERVER(this);

	CArea* area = GetArea();
	if (!area)
		return ;

	CNPC* pNext = area->m_npcList;
	CNPC* p;
	while ((p = pNext))
	{
		pNext = p->m_pNPCNext;
		if (p->m_proto->CheckFlag(NPC_CASTLE_TOWER))
		{
			p->SendDisappearAllInCell(true);

			// 어택리스트에서 삭제
			DelAttackList(p);

			// npc 삭제
			area->CharFromCell(p, true);
			area->DelNPC(p);
		}
	}

	InitGateNPC();
}

void CWarCastle::RemoveLordSymbol()
{
	CHECK_SUBSERVER(this);

	if (m_lordSymbol == NULL)
		return ;
	CArea* area = m_lordSymbol->m_pArea;
	if (!area)
	{
		DelAttackList(m_lordSymbol);
		delete m_lordSymbol;
	}
	else
	{
		m_lordSymbol->SendDisappearAllInCell(true);
		DelAttackList(m_lordSymbol);
		area->CharFromCell(m_lordSymbol, true);
		area->DelNPC(m_lordSymbol);
	}
	m_lordSymbol = NULL;
}

bool CWarCastle::IsTop3Guild(int guildindex)
{
	int i;
	for (i = 0; i < 3; i++)
	{
		if (m_top3guild[i])
		{
			if (m_top3guild[i]->GetIndex() == guildindex)
				return true;
		}
	}
	return false;
}

int CWarCastle::DecreaseLordSymbolHP()
{
	if (DEAD(m_lordSymbol))
		return 0;
	m_lordSymbol->m_hp--;

	return m_lordSymbol->m_hp;
}

bool CWarCastle::IsInRegenPoint(CCharacter* ch, bool bAttack)
{
	char ylayer = GET_YLAYER(ch);
	float x = GET_X(ch);
	float z = GET_Z(ch);

	int pos = 0;

	if (IS_NPC(ch))
	{
		CNPC* npc = TO_NPC(ch);
		if (npc->m_proto->CheckFlag(NPC_CASTLE_GUARD))
		{
			pos = GetRegenPoint(WCJF_DEFENSE_GUILD, NULL);
		}
	}
	else if (IS_PET(ch))
	{
		CPet* pet = TO_PET(ch);
		if (pet->GetOwner())
		{
			pos = GetRegenPoint(pet->GetOwner()->GetJoinFlag(GetZoneIndex()), pet->GetOwner());
		}
	}
	else if (IS_PC(ch))
	{
		CPC* pc = TO_PC(ch);
		pos = GetRegenPoint(pc->GetJoinFlag(GetZoneIndex()), pc);
	}

	if (pos == 0)
		return false;

	CArea* area = GetArea();
	if (!area)
		return false;

	CZone* zone = area->m_zone;
	if (!zone)
		return false;

	if (pos < 0 || pos >= zone->m_countZonePos)
		return false;

	if (zone->m_zonePos[pos][0] == ylayer
			&& zone->m_zonePos[pos][1] / 2.0f <= x && x <= zone->m_zonePos[pos][3] / 2.0f
			&& zone->m_zonePos[pos][2] / 2.0f <= z && z <= zone->m_zonePos[pos][4] / 2.0f)
		return true;

	return false;
}

void CWarCastle::CheckWarCastleEnd(bool forced_end_merac)
{
	if (GetState() != WCSF_NORMAL)
	{
		if( gserver->getNowSecond() >= GetGuildGradeSkillTime() + APPLY_GUILD_GRADE_SKILL_TIME  )
		{
			SetGuildGradeSkillTime( gserver->getNowSecond() );
			ApplyGuildGradeSkillAll();
		}
	}

	// 진행 상태 검사
	if (GetState() != WCSF_WAR_CASTLE)
		return ;

	bool bFinished = false;

	if(forced_end_merac == true)
		bFinished = true;

	// 시간이 종료되었나?
	int nextWarTime = GetNextWarTime();
	time_t curTime;
	time(&curTime);
	if (nextWarTime + WCT_WAR <= curTime)
		bFinished = true;

	// 권좌가 부서졌나?
	bool bDestroySymbol = false;
	if (GetLordSymbol() && DEAD(GetLordSymbol()))
	{
		bDestroySymbol = true;
		bFinished = true;
	}

	// 공성 신청 길드가 있는지 검사
	if (m_attackGuildList == NULL)
		bFinished = true;

	if (!bFinished)
		return ;

	// 승리자 구함
	bool bWinDefense = true;
	CGuild* topguild = NULL;
	if (bDestroySymbol)
	{
		// 공성측 승리
		// 가장 포인트가 많은 길드 구하기
		GetTop3AttackGuild();

		if (m_top3guild[0])
		{
			topguild = gserver->m_guildlist.findguild(m_top3guild[0]->GetIndex());
			if (topguild)
				bWinDefense = false;
		}
	}

	// 권좌/수호탑/워프게이트 삭제
	RemoveLordSymbol();
	RemoveCastleTower();
	RemoveWarpNPC();

	GAMELOG << init("WAR CASTLE : END WAR")
			<< GetZoneIndex();
	if (bWinDefense)
	{
		GAMELOG << delim << "WIN DEFENSE" << delim << GetOwnerGuildIndex() << delim <<  GetOwnerGuildName();
	}
	else
	{
		GAMELOG << delim << "WIN ATTACK" << delim << topguild->index() << delim << topguild->name();
		int i;
		for (i = 0; i < 3; i++)
		{
			if (m_top3guild[i])
			{
				GAMELOG << delim << m_top3guild[i]->GetIndex() << delim << m_top3guild[i]->GetPoint();
			}
			else
			{
				GAMELOG << delim << -1 << delim << 0;
			}
		}
	}
	GAMELOG << end;

	// 성 정보 초기화
	SetState(WCSF_NORMAL);
	m_lastWarTime = GetNextWarTime() + WCT_WAR;
	SetNextWarTime(0);
	if (!bWinDefense)
	{
		// 060116 : BS : BEGIN : 공성 끝나고 칼 회수
		CPC* ownerPC = PCManager::instance()->getPlayerByCharIndex(GetOwnerCharIndex());
		if (ownerPC)
		{
			TakeLordItem(ownerPC);
		}
		// 060116 : BS : END : 공성 끝나고 칼 회수
		CGuild * oldGuild =  gserver->m_guildlist.findguild( GetOwnerGuildIndex() );
		SetOwner(topguild);

		if( ownerPC )
		{
			CNetMsg::SP rmsg(new CNetMsg);
			GuildNameColorStateMsg(rmsg, ownerPC);
			ownerPC->m_pArea->SendToCell( rmsg, ownerPC, true);
		}

		if( oldGuild )
		{
			int i=0;
			int oldGuildMaxmember = oldGuild->maxmember();
			for(i=0; i<oldGuildMaxmember; i++)
			{
				if (oldGuild->member(i))
				{
					CPC * pMember = oldGuild->member(i)->GetPC();
					if ( pMember )
					{
						CNetMsg::SP rmsg(new CNetMsg);
						GuildNameColorStateMsg(rmsg, pMember);
						SEND_Q(rmsg, pMember->m_desc );
					}
				}
			}
		}
	}
	m_bNoticeWarTime = false;
	m_noticeRemain = 9999;
	m_defensePoint = 0;
	m_gateState = 0;
	m_bMoveToRegen = false;

	m_countAttackGuild = 0;
	memset(m_top3guild, 0, sizeof(CGuild*) * 3);

	// 칼 지급
	if (GetOwnerCharIndex())
	{
		CPC* ownerPC = PCManager::instance()->getPlayerByCharIndex(GetOwnerCharIndex());
		if (ownerPC)
		{
			GiveLordItem(ownerPC);

			// 메라크 공성 정보 공성 정보 갱신
			CNetMsg::SP rmsg(new CNetMsg);
			GuildNameColorStateMsg(rmsg, ownerPC);
			ownerPC->m_pArea->SendToCell( rmsg, ownerPC, true);
		}

		CGuild * pGuild = gserver->m_guildlist.findguild( GetOwnerGuildIndex() );
		if( pGuild )
		{
			int i=0;
			int guildMaxmember = pGuild->maxmember();
			for(i=0; i<guildMaxmember; i++)
			{
				if (pGuild->member(i))
				{
					CPC * pMember = pGuild->member(i)->GetPC();
					if ( pMember )
					{
						CNetMsg::SP rmsg(new CNetMsg);
						GuildNameColorStateMsg(rmsg, pMember);
						SEND_Q(rmsg, pMember->m_desc );
					}
				}
			}
		}
	}

	// DB 초기화
	CDBCmd cmd;
	cmd.Init(&gserver->m_dbcastle);

	//char guildname[100];
	//char charname[100];
	//BackSlash(guildname, GetOwnerGuildName());
	//BackSlash(charname, GetOwnerCharName());

	std::string update_castle_query = boost::str(boost::format(
										  "UPDATE t_castle SET a_last_war_time = %d, a_state = 0, a_next_war_time = %d, a_owner_guild_index = %d, a_owner_guild_name = '%s', a_owner_char_index=%d, a_owner_char_name='%s'"
										  " WHERE a_zone_index = %d")
									  % m_lastWarTime % GetNextWarTime() % GetOwnerGuildIndex() % GetOwnerGuildName() % GetOwnerCharIndex() % GetOwnerCharName()
									  % GetZoneIndex());
	cmd.SetQuery(update_castle_query);
	if (!cmd.Update())
	{
		GAMELOG << init("SYS_ERR: CANNOT SAVE CASTLE INFO")
				<< "ZONE" << delim
				<< GetZoneIndex() << delim
				<< "OWNER" << delim
				<< GetOwnerGuildIndex() << delim
				<< GetOwnerGuildName() << delim
				<< "LASTTIME" << delim
				<< m_lastWarTime
				<< end;
	}
	else
	{
		GAMELOG << init("WAR CASTLE : SAVE WAR END")
				<< "ZONE"			<< delim << GetZoneIndex()											<< delim
				<< "LAST TIME"		<< delim << m_lastWarTime											<< delim
				<< "NEXT TIME"		<< delim << m_nextWarTime											<< delim
				<< "OWNER GUILD"	<< delim << GetOwnerGuildIndex() << delim << GetOwnerGuildName()	<< delim
				<< "OWNER CHAR"		<< delim << GetOwnerCharIndex() << delim << GetOwnerCharName()
				<< end;
	}

	std::string delete_castle_join_query = boost::str(boost::format("DELETE FROM t_castle_join WHERE a_zone_index = %d") % GetZoneIndex());
	cmd.SetQuery(delete_castle_join_query);
	if (!cmd.Update())
	{
		GAMELOG << init("SYS_ERR: CANNOT DELETE CASTLE JOIN INFO")
				<< "ZONE" << delim
				<< GetZoneIndex()
				<< end;
	}

	// 참여정보 초기화
	ResetJoinFlag();

	// 참여 리스트 초기화
	RemoveAllJoinList();

	// 공성 결과 알리기
	struct tm tmJoin;
	GetJoinTime(&tmJoin, true);
	if (gserver->isRunHelper())
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperWarNoticeEndMsg(rmsg, GetZoneIndex(), bWinDefense, GetOwnerGuildIndex(), GetOwnerGuildName(), GetOwnerCharIndex(), GetOwnerCharName(), tmJoin.tm_mon, tmJoin.tm_mday, tmJoin.tm_hour, tmJoin.tm_wday);
		SEND_Q(rmsg, gserver->m_helper);
	}
	else
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildWarEndMsg(rmsg, GetZoneIndex(), ((bWinDefense) ? (char)1 : (char)0), GetOwnerGuildIndex(), GetOwnerGuildName(), GetOwnerCharIndex(), GetOwnerCharName(), tmJoin.tm_mon, tmJoin.tm_mday, tmJoin.tm_hour, tmJoin.tm_wday);
		PCManager::instance()->sendToAll(rmsg);
	}

	{
		// 성주 정보 전송
		CNetMsg::SP rmsg(new CNetMsg);
		CastleOwnerInfoMsg(rmsg);
		PCManager::instance()->sendToAll(rmsg);
	}
}

void CWarCastle::RemoveAllJoinList()
{
	CureGuildGradeSkillAll();

	CWarCastleJoin* p;

	while (m_attackGuildList)
	{
		p = m_attackGuildList->m_next;
		delete m_attackGuildList;
		m_attackGuildList = p;
	}

	while (m_defenseGuildList)
	{
		p = m_defenseGuildList->m_next;
		delete m_defenseGuildList;
		m_defenseGuildList = p;
	}

	while (m_attackCharList)
	{
		p = m_attackCharList->m_next;
		delete m_attackCharList;
		m_attackCharList = p;
	}

	while (m_defenseCharList)
	{
		p = m_defenseCharList->m_next;
		delete m_defenseCharList;
		m_defenseCharList = p;
	}
}

void CWarCastle::ResetJoinFlag()
{
	PCManager::map_t& playerMap			= PCManager::instance()->getPlayerMap();
	PCManager::map_t::iterator iter		= playerMap.begin();
	PCManager::map_t::iterator endIter	= playerMap.end();

	for (; iter != endIter; ++iter)
	{
		CPC* pc = (*iter).pPlayer;
		if (pc == NULL)
		{
			continue;;
		}

		if( pc->m_guildInfo )
			pc->m_guildInfo->guild()->SetRerithPos( false );   // 071016 - whs25 추가 길드의 부활진지 정보 초기화

		if (pc->m_guildInfo && pc->m_guildInfo->guild()->index() == GetOwnerGuildIndex())
			pc->SetJoinFlag(GetZoneIndex(), WCJF_OWNER);
		else
			pc->SetJoinFlag(GetZoneIndex(), WCJF_NONE);
	}
}

int* CWarCastle::GetOwnCastle(int guildindex, int* count)
{
	int zoneCount = 2;
	int zones[] = { ZONE_DRATAN, ZONE_MERAC };
	int res[] = { -1, -1 };

	*count = 0;
	CWarCastle* castle;

	int i;
	for (i = 0; i < zoneCount; i++)
	{
		castle = CWarCastle::GetCastleObject(zones[i]);
		if (castle)
		{
			if (castle->GetOwnerGuildIndex() == guildindex)
			{
				res[*count] = zones[i];
				(*count)++;
			}
		}
	}

	if (*count > 0)
	{
		int* ret = new int[*count];
		for (i = 0; i < *count; i++)
		{
			ret[i] = res[i];
		}

		return ret;
	}

	return NULL;
}

void CWarCastle::DeleteGuild(int guildindex)
{
	// 검사할 존 리스트
	int count = 1;
	int zoneindex[] = { ZONE_MERAC };

	// 길드 얻어서 길드 멤버 참여정보 초기화
	CGuild* guild = gserver->m_guildlist.findguild(guildindex);

	int i;
	for (i = 0; i < count; i++)
	{
		CWarCastle* castle = GetCastleObject(zoneindex[i]);
		if (!castle)
			continue ;

		// 공성 도중에는 반영 안함
		if (castle->GetState() != WCSF_NORMAL)
			return ;

		// 성주 길드 검사
		if (castle->GetOwnerGuildIndex() == guildindex)
			castle->ResetOwner();
		else
		{
			// 수성 길드 검사
			castle->RemoveDefenseGuild(guildindex);

			// 공성 길드 검사
			castle->RemoveAttackGuild(guildindex);
		}

		if (guild)
		{
			int j;
			int guildMaxMember = guild->maxmember();
			for (j = 0; j < guildMaxMember; j++)
			{
				if (guild->member(j) && guild->member(j)->GetPC())
				{
					guild->member(j)->GetPC()->SetJoinFlag(zoneindex[i], WCJF_NONE);
					guild->member(j)->GetPC()->m_bChangeStatus = true;
				}
			}
		}
	}

	// 참여 정보 DB에 저장
	std::string delete_castle_join_query = boost::str(boost::format("DELETE FROM t_castle_join WHERE a_index=%d AND a_guild=0") % guildindex);
	CDBCmd cmd;
	cmd.Init(&gserver->m_dbcastle);
	cmd.SetQuery(delete_castle_join_query);
	cmd.Update();
}

void CWarCastle::ResetOwner()
{
	m_ownerGuildIndex = 0;
	m_ownerGuildName = "";
	m_ownerCharIndex = 0;
	m_ownerCharName = "";
	m_oldOwnerCharIndex = 0;
}

void CWarCastle::SetOwner(CGuild* guild)
{
	if (guild)
	{
		m_ownerGuildIndex = guild->index();
		m_ownerGuildName = guild->name();
		m_ownerCharIndex = guild->boss()->charindex();
		m_ownerCharName = guild->boss()->GetName();
	}
	else
		ResetOwner();
}

void CWarCastle::RegenWarpNPC()
{
	CHECK_SUBSERVER(this);

	if (GetState() == WCSF_NORMAL)
		return ;

	CArea* area = GetArea();
	if (!area)
		return ;

	int regencount = 0;

	int i;
	for (i = 0; i < area->m_npcRegenList.m_nCount; i++)
	{
		CNPCRegenInfo* p = area->m_npcRegenList.m_infoList + i;
		if (p->m_totalNum != 0 || p->m_bAlive || (gserver->m_pulse - p->m_lastDieTime < p->m_regenSec))
			continue ;

		CNPCProto* proto = gserver->m_npcProtoList.FindProto(p->m_npcIdx);
		if (proto && proto->CheckFlag(NPC_ZONEMOVER))
		{
			CNPC* npc = gserver->m_npcProtoList.Create(proto->m_index, p);
			if (!npc)
				return ;

			GET_X(npc) = p->m_regenX;
			GET_Z(npc) = p->m_regenZ;
			GET_R(npc) = p->m_regenR;
			GET_YLAYER(npc) = p->m_regenY;

			npc->m_regenX = GET_X(npc);
			npc->m_regenZ = GET_Z(npc);
			npc->m_regenY = GET_YLAYER(npc);

			npc->m_recoverPulse = gserver->m_pulse;

			p->m_bAlive = true;
			p->m_lastDieTime = 0;
			npc->InitPointsToMax();
			npc->m_disableTime = 0;

			npc->InitPointsToMax();
			area->AddNPC(npc);
			npc->m_postregendelay = 0;

			int cx, cz;
			area->PointToCellNum(GET_X(npc), GET_Z(npc), &cx, &cz);
			area->CharToCell(npc, GET_YLAYER(npc), cx, cz);

			{
				CNetMsg::SP rmsg(new CNetMsg);
				AppearMsg(rmsg, npc, true);
				area->SendToCell(rmsg, npc);
			}

			regencount++;
		}
	}

	if (regencount > 0)
	{
		GAMELOG << init("CASTLE WARP NPC REGEN")
				<< "ZONE" << delim
				<< GetZoneIndex() << delim
				<< "COUNT" << delim
				<< regencount
				<< end;
	}
}

void CWarCastle::RemoveWarpNPC()
{
	CHECK_SUBSERVER(this);

	CArea* area = GetArea();
	if (!area)
		return ;

	CNPC* pNext = area->m_npcList;
	CNPC* p;
	while ((p = pNext))
	{
		pNext = p->m_pNPCNext;
		if (p->m_proto->CheckFlag(NPC_ZONEMOVER))
		{
			p->SendDisappearAllInCell(true);

			// 어택리스트에서 삭제
			DelAttackList(p);

			// npc 삭제
			area->CharFromCell(p, true);
			area->DelNPC(p);
		}
	}
}

bool CWarCastle::CanLordChat(CPC* pc)
{
	CWarCastle* castle = NULL;
#ifdef WARCASTLE_SUBNUMBER_MEMBER_VALUE
	switch(meracCastle.m_subNumber)
#else // WARCASTLE_SUBNUMBER_MEMBER_VALUE
	switch (gserver->m_subno)
#endif // WARCASTLE_SUBNUMBER_MEMBER_VALUE
	{
	case WAR_CASTLE_SUBNUMBER_MERAC:
		castle = CWarCastle::GetCastleObject(ZONE_MERAC);
		if (!castle)
			return false;
		if (castle->GetState() != WCSF_NORMAL)
			return false;
		if (castle->GetOwnerCharIndex() != pc->m_index)
			return false;
		return true;

	default:
		return false;
	}
}

bool CWarCastle::CheckSubServer()
{
	return (GetZoneIndex() == GetCurSubServerCastleZoneIndex());
}

int CWarCastle::GetCurSubServerCastleZoneIndex()
{
#ifdef WARCASTLE_SUBNUMBER_MEMBER_VALUE
	switch(meracCastle.m_subNumber)
#else // WARCASTLE_SUBNUMBER_MEMBER_VALUE
	switch (gserver->m_subno)
#endif // WARCASTLE_SUBNUMBER_MEMBER_VALUE
	{
	case WAR_CASTLE_SUBNUMBER_MERAC:
		return ZONE_MERAC;

	default:
		return -1;
	}
}

bool CWarCastle::IsInInnerCastle(CCharacter* pChar)
{
	char		nYlayer	= 0;
	int			nX0x2	= 0;
	int			nZ0x2	= 0;
	int			nX1x2	= 0;
	int			nZ1x2	= 0;
	GetInnerCastleRect(&nYlayer, &nX0x2, &nZ0x2, &nX1x2, &nZ1x2);

	if (nX0x2 / 2.0f > GET_X(pChar) || GET_X(pChar) > nX1x2 / 2.0f || nZ0x2 / 2.0f > GET_Z(pChar) || GET_Z(pChar) > nZ1x2 / 2.0f)
		return false;
	else
		return true;
}

void CWarCastle::GuildGradeSkillAll( bool apply )
{
	CWarCastleJoin* p;
	CGuild * guild = NULL;

	guild = gserver->m_guildlist.findguild( m_ownerGuildIndex );
	if( guild )
	{
		int i;
		int guildMemberCount = guild->membercount();
		for( i = 0; i < guildMemberCount; i++ )
		{
			CGuildMember * gMember = guild->member(i);
			if( gMember && gMember->GetPC() )
				gMember->GetPC()->GuildGradeSkill(apply);
		}
	}

	p = m_attackGuildList;
	while (p)
	{
		guild = gserver->m_guildlist.findguild( p->GetIndex() );
		if( guild )
		{
			int i;
			int guildMemberCount = guild->membercount();
			for( i = 0; i < guildMemberCount; i++ )
			{
				CGuildMember * gMember = guild->member(i);
				if( gMember && gMember->GetPC() )
					gMember->GetPC()->GuildGradeSkill(apply);
			}
		}
		p = p->m_next;
	}

	p = m_defenseGuildList;
	while (p)
	{
		guild = gserver->m_guildlist.findguild( p->GetIndex() );
		if( guild )
		{
			int i;
			int guildMemberCount = guild->membercount();
			for( i = 0; i < guildMemberCount; i++ )
			{
				CGuildMember * gMember = guild->member(i);
				if( gMember && gMember->GetPC() )
					gMember->GetPC()->GuildGradeSkill(apply);
			}
		}
		p = p->m_next;
	}

	p = m_attackCharList;
	while (p)
	{
		CPC * pc = PCManager::instance()->getPlayerByCharIndex(p->GetIndex() );
		if(pc)
			pc->GuildGradeSkill(apply);
		p = p->m_next;
	}

	p = m_defenseCharList;
	while (p)
	{
		CPC * pc = PCManager::instance()->getPlayerByCharIndex(p->GetIndex() );
		if(pc)
			pc->GuildGradeSkill(apply);
		p = p->m_next;
	}
}

void CWarCastle::EndWarRegenPoint()
{
	CZone*		pZone	= gserver->FindZone(GetZoneIndex());
	if (pZone == NULL)
		return;

	CArea*		pArea	= pZone->m_area;

	char		nYlayer	= 0;
	int			nX0x2	= 0;
	int			nZ0x2	= 0;
	int			nX1x2	= 0;
	int			nZ1x2	= 0;
	GetInnerCastleRect(&nYlayer, &nX0x2, &nZ0x2, &nX1x2, &nZ1x2);

	int			nCellX0	= 0;
	int			nCellZ0	= 0;
	int			nCellX1	= 0;
	int			nCellZ1	= 0;
	pArea->PointToCellNum(nX0x2 / 2.0f, nZ0x2 / 2.0f, &nCellX0, &nCellZ0);
	pArea->PointToCellNum(nX1x2 / 2.0f, nZ1x2 / 2.0f, &nCellX1, &nCellZ1);

	// 먼저 이동시킬 유저를 선별후 이동 // 다른셀로 이동시 또는 이동중 삭제시 문제 발생
	std::vector<CPC*> m_MovePCList;

	int cx, cz;
	for (cx = nCellX0; cx <= nCellX1; cx++)
	{
		if (cx < 0 || cx >= pArea->m_size[0])
			continue ;

		for (cz = nCellZ0; cz <= nCellZ1; cz++)
		{
			if (cz < 0 || cz >= pArea->m_size[1])
				continue ;

			CCell& rCell = pArea->m_cell[cx][cz];
			CCharacter* pCharNext = rCell.m_listChar;
			CCharacter* pChar;
			while ((pChar = pCharNext))
			{
				pCharNext = pCharNext->m_pCellNext;

				if ( IS_PC(pChar) && (-1 != pChar->m_index) )
				{
					CPC* pPC = TO_PC(pChar);
					if (pPC)
					{
						m_MovePCList.push_back( pPC );
					}
				}
			}
		}
	}

	// 리스트에 있는 PC 를 이동.
	std::vector<CPC*>::iterator it = m_MovePCList.begin();
	std::vector<CPC*>::iterator endit = m_MovePCList.end();
	for(; it != endit; ++it)
	{
		CPC* movePc = *(it);

		ClearWarItemSkill(movePc);

		int nJoinFlag = movePc->GetJoinFlag(pZone->m_index);

		switch (nJoinFlag)
		{
		case WCJF_OWNER:
		case WCJF_DEFENSE_GUILD:
			break;
		default:
			{
				int extra = 0;
				GoZone(movePc, pZone->m_index,
					pZone->m_zonePos[extra][0],
					GetRandom(pZone->m_zonePos[extra][1], pZone->m_zonePos[extra][3]) / 2.0f,
					GetRandom(pZone->m_zonePos[extra][2], pZone->m_zonePos[extra][4]) / 2.0f);
				movePc->SetDisableTime(1);
			}
			break;
		}
	}

	m_MovePCList.clear();
}
