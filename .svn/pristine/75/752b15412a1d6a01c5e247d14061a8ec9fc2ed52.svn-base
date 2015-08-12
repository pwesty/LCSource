#include <boost/format.hpp>
#include "stdhdrs.h"

#include "Log.h"
#include "Server.h"
#include "CmdMsg.h"
#include "doFunc.h"
#include "../ShareLib/DBCmd.h"

// BW
bool b_rankinitupdate;
bool b_rewardinitupdate;
bool b_rewardendupdate;
struct tm rankInitUpdateTime;
struct tm rewardInitUpdateTime;
struct tm rewardEndUpdateTime;

void CServer::initTime()
{
	////////////////////
	// 테섭 사항
	// 이번주에 초기화될 시간 세팅 : BW
	time_t now1;
	time( &now1 );

	rankInitUpdateTime = *localtime( &now1 );
	rewardInitUpdateTime = *localtime( &now1 );
	rewardEndUpdateTime = *localtime( &now1 );

	while(1)
	{
		rankInitUpdateTime.tm_mday = rankInitUpdateTime.tm_mday++;
		mktime( &rankInitUpdateTime );

		rewardInitUpdateTime.tm_mday = rewardInitUpdateTime.tm_mday++;
		mktime( &rewardInitUpdateTime );

		rewardEndUpdateTime.tm_mday = rewardEndUpdateTime.tm_mday++;
		mktime( &rewardEndUpdateTime );

		if(rankInitUpdateTime.tm_wday == 2) break;
	}

	rankInitUpdateTime.tm_hour = 8;
	rankInitUpdateTime.tm_min = 0;
	rankInitUpdateTime.tm_sec = 0;

	rewardInitUpdateTime.tm_hour = 8;
	rewardInitUpdateTime.tm_min = 20;
	rewardInitUpdateTime.tm_sec = 0;

	rewardEndUpdateTime.tm_hour = 8;
	rewardEndUpdateTime.tm_min = 10;
	rewardEndUpdateTime.tm_sec = 0;

	b_rankinitupdate = false;
	b_rewardinitupdate = false;
	b_rewardendupdate = false;
}

void CServer::HeartBeat()
{
	///////////////////////////////
	// pd4 랭킹 초기화 : BW
	// 화요일 AM0:00 rankinit
	// 화요일 AM 11:00 rewardinit
	// 화요일 AM 09:00 rewardend
	//테섭 코드
	struct tm when;
	time_t now;
	time( &now );
	when = *localtime( &now );

	if( ( mktime(&rankInitUpdateTime) <= mktime(&when)) && b_rankinitupdate == true)
	{
		b_rankinitupdate = false;
	}
	if( ( mktime(&rewardInitUpdateTime ) <= mktime(&when)) && b_rewardinitupdate == true)
	{
		b_rewardinitupdate = false;
	}
	if( ( mktime(&rewardEndUpdateTime) <= mktime(&when)) && b_rewardendupdate == true)
	{
		b_rewardendupdate = false;
	}

	if( ( mktime(&rankInitUpdateTime) <= mktime(&when)) && b_rankinitupdate == false)
	{
		while(1)
		{
			rankInitUpdateTime.tm_mday++;
			mktime( &rankInitUpdateTime );
			if(rankInitUpdateTime.tm_wday == 2) break;
		}

		// 보낼 메시지 없음
		CNetMsg::SP rmsg(new CNetMsg);
		do_CommandPD4RankInit(rmsg, (CDescriptor*) 0);

		rankInitUpdateTime.tm_hour = 8;
		rankInitUpdateTime.tm_min = 0;
		rankInitUpdateTime.tm_sec = 0;

		mktime(&rankInitUpdateTime);

//		printf("%s\n", asctime(&rankInitUpdateTime) );
		GAMELOG << init("PD4 RANK INIT") << end;
		b_rankinitupdate = true;
	}

	if( ( mktime(&rewardEndUpdateTime) <= mktime(&when)) && b_rewardendupdate == false)
	{
		while(1)
		{
			rewardEndUpdateTime.tm_mday++;
			mktime( &rewardEndUpdateTime );
			if(rewardEndUpdateTime.tm_wday == 2) break;
		}

		// 보낼 메시지 없음
		CNetMsg::SP rmsg(new CNetMsg);
		do_CommandPD4RewardEnd(rmsg, (CDescriptor*) 0);

		rewardEndUpdateTime.tm_hour = 8;
		rewardEndUpdateTime.tm_min = 10;
		rewardEndUpdateTime.tm_sec = 0;
		mktime(&rewardEndUpdateTime);

		GAMELOG << init("PD4 REWARD END") << end;
		b_rewardendupdate = true;
	}

	if( ( mktime(&rewardInitUpdateTime) <= mktime(&when)) && b_rewardinitupdate == false)
	{
		while(1)
		{
			rewardInitUpdateTime.tm_mday++;
			mktime( &rewardInitUpdateTime );
			if(rewardInitUpdateTime.tm_wday == 2) break;
		}

		// 보낼 메시지 없음
		CNetMsg::SP rmsg(new CNetMsg);
		do_CommandPD4RewardInit(rmsg, (CDescriptor*) 0);

		rewardInitUpdateTime.tm_hour = 8;
		rewardInitUpdateTime.tm_min = 20;
		rewardInitUpdateTime.tm_sec = 0;
		mktime(&rewardInitUpdateTime);

		GAMELOG << init("PD4 REWARD INIT") << end;
		b_rewardinitupdate = true;
	}

#ifdef EXTREME_CUBE_VER2
	if(m_CubePointUpdateTime+604800 <= now)
	{
		GAMELOG << init("EXTREME_CUBE_RANKING") << end;
		CubePointRanking(m_CubePointUpdateTime);
		int updatetime = SetCurCubePointUpdateTime();
		GAMELOG << init("EXTREME_CUBE_RANKING_NEXTTIME") << updatetime << end;
	}
#endif // EXTREME_CUBE_VER2

#ifdef DEV_GUILD_MARK
#endif // DEV_GUILD_MARK
	checkGuildProcess(now);
}

#ifdef DEV_GUILD_MARK
#endif

void CServer::checkGuildProcess(time_t now)
{
	// 전체 길드 루프
	CGuild* p = NULL;
	CGuild* pNext = NULL;
	pNext = gserver.m_guildlist.head();

	while((p = pNext))
	{
		pNext = pNext->nextguild();

		// 길드 배틀 시간 체크
		checkGuildBattle(now, p);

#if defined (DEV_GUILD_MARK)
		// 길드 마크 시간 체크
		checkGuildMarkTime(now, p);
#endif // DEV_GUILD_MARK

		// 길드 마스터 자동 킥 체크
		checkGuildMasterAutoKick(now, p);
	}
}

void CServer::checkGuildMasterAutoKick(time_t now, CGuild* p)
{
	if (p->getGuildKick()->getKickStatus() != GMKS_RESERVE)
		return;
	if (p->battleState() != GUILD_BATTLE_STATE_PEACE)
		return;
	if (p->getGuildKick()->getKickRequestTime() < now - KICK_RESERVE_TIME)
	{
		// 해임 신청 후 하루가 지났으면,
		int _newBoss = p->getNewBossByKick();
		if ( do_GuildBossChange(p->index(), _newBoss) )
		{
			// 길마 변경 성공 시,
			do_GuildStatusChange(p->index(), GMKS_NORMAL);
		}
		else
		{
			do_GuildStatusChange(p->index(), GMKS_NORMAL);
		}
	}
}

void CServer::checkGuildBattle(time_t, CGuild* g)
{
	if (g->battleState() == GUILD_BATTLE_STATE_PEACE)
		return;

	int diffPulse = m_pulse - g->battlePulse();
	int battleTime = g->battleTime() - diffPulse;

	// 길드전 중일때
	switch (g->battleState())
	{
	case GUILD_BATTLE_STATE_START_WAIT:
	case GUILD_BATTLE_STATE_STOP_WAIT:
		break;

	case GUILD_BATTLE_STATE_WAIT:

		if (diffPulse > GUILD_BATTLE_WAIT_TIME)
		{
			int settime = g->battleTime() - GUILD_BATTLE_WAIT_TIME;

			// 길드전 시작요청
			CGuild* guild = gserver.m_guildlist.findguild(g->battleIndex());
			if (!guild)
				return;

			// DB UPDATE
			std::string update_guild_query = boost::str(boost::format("UPDATE t_guild SET a_battle_time = %d, a_battle_state = %d WHERE a_index = %d")
											 % settime % GUILD_BATTLE_STATE_ING % g->index());
			CDBCmd cmd;
			cmd.Init(&gserver.m_dbchar);
			cmd.SetQuery(update_guild_query);
			if (!cmd.Update())
			{
				return;
			}

			std::string update_guild_query_1 = boost::str(boost::format("UPDATE t_guild SET a_battle_time = %d, a_battle_state = %d WHERE a_index = %d")
											   % settime % GUILD_BATTLE_STATE_ING % guild->index());
			cmd.Init(&gserver.m_dbchar);
			cmd.SetQuery(update_guild_query_1);
			if (!cmd.Update())
			{
				return;
			}

			// 세부 셋팅
			g->BattleTime(settime);
			g->BattleState(GUILD_BATTLE_STATE_ING);
			g->BattlePulse(m_pulse);

			guild->BattleTime(settime);
			guild->BattleState(GUILD_BATTLE_STATE_ING);
			guild->BattlePulse(m_pulse);

			GAMELOG << init("GUILD BATTLE START")
					<< g->index() << delim
					<< g->name() << delim
					<< "<==>" << delim
					<< guild->index() << delim
					<< guild->name() << delim
					<< g->battlePrize() << delim
					<< g->battleZone() << delim
					<< g->battleTime() << delim
					<< end;

			CNetMsg::SP rmsg(new CNetMsg);
			HelperGuildBattleStartMsg(rmsg, g, guild);
			gserver.SendToAllGameServer(rmsg);
		}
		break;

	case GUILD_BATTLE_STATE_ING:

		// 길드전 끝
		if (battleTime < 1)
		{
			CGuild* guild = gserver.m_guildlist.findguild(g->battleIndex());

			if (!guild)
				return;

			CNetMsg::SP rmsg(new CNetMsg);
			rmsg->Init(MSG_HELPER_COMMAND);
			RefMsg(rmsg) << MSG_HELPER_GUILD_BATTLE_STOP_REQ
						 << g->index()
						 << guild->index();

			do_Command(rmsg, NULL);
		}

		// 길드전 상황 보고 1분
		if (diffPulse > 1200)
		{
			CGuild* guild = gserver.m_guildlist.findguild(g->battleIndex());
			if (!guild)
				return;

			// DB UPDATE
			std::string update_guild_query = boost::str(boost::format("UPDATE t_guild SET a_battle_time = %d WHERE a_index = %d")
											 % battleTime % g->index());
			CDBCmd cmd;
			cmd.Init(&gserver.m_dbchar);
			cmd.SetQuery(update_guild_query);
			if (!cmd.Update())
			{
				return;
			}

			std::string update_guild_query_1 = boost::str(boost::format("UPDATE t_guild SET a_battle_time = %d WHERE a_index = %d" )
											   % battleTime % guild->index());
			cmd.Init(&gserver.m_dbchar);
			cmd.SetQuery(update_guild_query_1);
			if (!cmd.Update())
			{
				return;
			}

			g->BattleTime(battleTime);
			g->BattlePulse(m_pulse);

			guild->BattleTime(battleTime);
			guild->BattlePulse(m_pulse);

			CNetMsg::SP rmsg(new CNetMsg);
			HelperGuildBattleStatusMsg(rmsg, g, guild);
			gserver.SendToAllGameServer(rmsg);
		}

		break;
	}
}

#if defined (DEV_GUILD_MARK)
void CServer::checkGuildMarkTime(time_t now, CGuild* p)
{
	if(p->GetMarkTime() != -1)
	{
		// 시간을 now와 확인을 한다.
		if(p->GetMarkTime() <= (int)now)
		{
			// 길드마크는 역사속으로...ㅠㅠ
			CDBCmd cmd;
			cmd.Init(&gserver.m_dbchar);
			CLCString sql(1024);
			sql.Format("UPDATE t_extend_guild SET a_gm_row = -1, a_gm_col = -1, a_bg_row = -1, a_bg_col = -1, a_marktime = -1 WHERE a_guild_index = %d", p->index());
			cmd.SetQuery(sql);
			if(!cmd.Update())
			{
				// 업데이트 안됨 시스템 에러 메시지 발송
				GAMELOG << init("ERROR GUILD MARK EXPIRE UPDATE FAILED")
						<< "GUILD INDEX" << delim << p->index() << end;
			}
			else
			{
				// 제대로 업데이트 됨 성공 메시지 발송
				p->SetGuildMark(-1, -1, -1, -1, -1);

				CNetMsg::SP rmsg(new CNetMsg);
				HelperGuildMarkExpireMsg(rmsg, 0, p->index());
				gserver.SendToAllGameServer(rmsg);
			}
		}
	}
}
#endif // DEV_GUILD_MARK

