#include <boost/format.hpp>
#include <boost/foreach.hpp>
#include "stdhdrs.h"

#include "../ShareLib/bnf.h"
#include "Server.h"
#include "Descriptor.h"
#include "DBManager.h"
#include "CmdMsg.h"
#include "ServerTimer.h"
#include "Notice.h"
#include "../ShareLib/PrintExcuteInfo.h"

#ifdef SERVER_AUTHENTICATION
#include "../ShareLib/ServerAuthentication.h"
#endif

#define SHUTDONW_REBOOT_NOTIFY_SEC		(10) // sec

void ServerTimerPerSecond::operate( rnSocketIOService* service )
{
	//LOG_INFO("DEBUG_FUNC : START");
	gserver->m_nowseconds = time(0);
	memcpy(&gserver->m_tRealSystemTime, localtime(&gserver->m_nowseconds), sizeof(gserver->m_tRealSystemTime));

	TimerItemManager::AllcheckExpireTime(gserver->m_nowseconds);
	PCManager::map_t& playerMap			= PCManager::instance()->getPlayerMap();
	PCManager::map_t::iterator it		= playerMap.begin();
	PCManager::map_t::iterator endIt	= playerMap.end();
	for (; it != endIt; ++it)
	{
		CPC* pChar = (*it).pPlayer;
		if (STATE(pChar->m_desc) == CON_PLAYING && PCManager::instance()->isPlaying(pChar))
		{
			gserver->DecreaseTimeForPC(pChar);

			if (pChar->m_bChangeStatus)
			{
				pChar->SendStatus();
				pChar->SendElementalStatus();
			}
		}
	}

	gserver->NpcCtTimeCount();

	//////////////////////////////////////////////////////////////////////////

	gserver->HeartBeat();

	CArea::checkLifeTime();

	//LOG_INFO("DEBUG_FUNC : END");
}

ServerTimerPerSecond* ServerTimerPerSecond::instance()
{
	static ServerTimerPerSecond __instance;
	return &__instance;
}

void ServerTimerPerSecond::Init()
{
	bnf::instance()->CreateMSecTimer(1 * 1000, this);
}

//////////////////////////////////////////////////////////////////////////

void ServerTimerPerHundredMSecond::operate( rnSocketIOService* service )
{
	++gserver->m_pulse;

	if (gserver->m_pulse - gserver->m_pulseProcHeartBeatPerSec >= PASSES_PER_SEC)
	{
		gserver->m_pulseProcHeartBeatPerSec = gserver->m_pulse;
	}

	gserver->ProcMobItemControl();
}

ServerTimerPerHundredMSecond* ServerTimerPerHundredMSecond::instance()
{
	static ServerTimerPerHundredMSecond __instance;
	return &__instance;
}

void ServerTimerPerHundredMSecond::Init()
{
	bnf::instance()->CreateMSecTimer(1 * 100, this);
}

//////////////////////////////////////////////////////////////////////////

void ServerTimerPerMinute::operate( rnSocketIOService* service )
{
	//LOG_INFO("DEBUG_FUNC : START");
	gserver->PingDB();

	// 자동 저장
	int seq = ((int)time(0) / 60) % AUTO_SAVE_SEQUENCT;
	BOOST_FOREACH(CDescriptor* desc, CDescriptor::auto_set[seq])
	{
		DBManager::instance()->SaveCharacterInfo(desc, false);
	}

	{
		static int statistics_tick = 0;

		if (++statistics_tick >= 5)
		{
			statistics_tick = 0;
			gserver->ProcStatistics();
		}
	}

	if(gserver->m_buffSystem.Started())
		gserver->m_buffSystem.TickSystem();

	PopupNotice::instance()->SendDataForLoop();
	//LOG_INFO("DEBUG_FUNC : END");
}

ServerTimerPerMinute* ServerTimerPerMinute::instance()
{
	static ServerTimerPerMinute __instance;
	return &__instance;
}

void ServerTimerPerMinute::Init()
{
	time_t nowtime = time(0);
	int remain_sec = 60 - ((int)nowtime % 60);

	bnf::instance()->CreateSecTimerPeriod(remain_sec, 60, this);
}

//////////////////////////////////////////////////////////////////////////

void ServerTimerPerHour::operate( rnSocketIOService* service )
{
	//LOG_INFO("DEBUG_FUNC : START");
	time_t nowtime = time(0); //초단위로 반환

	memcpy(&gserver->m_tRealSystemTime, localtime(&nowtime), sizeof(gserver->m_tRealSystemTime));

	LOG_INFO("\n\nServerTimerPerHour Start!!!!!\n\n");

	gserver->CheckDungeonTime();

#ifdef REFORM_PK_PENALTY_201108 // 정시에 한번 성향 수지 장치 버프가 있는 플레이어에게 pk 성향 포인트 보상 보상해줘야한다.
	gserver->CheckPkDisPosiotionPointReward();
#endif

	//정시에 한번 체크를 하고
	//3시간에 한번씩 3시간동안 유지가 되었는지 확인 후에 보상을 해 준다.
	gserver->CheckDratanCastleDungeon( (nowtime / 60 / 60) % gserver->m_DratanCheckTime );
	if( (((nowtime / 60 / 60) % gserver->m_DratanCheckTime) == 0) && gserver->m_subno == 1)
	{
		gserver->CheckDratanCastleDungeonReward();
	}

	int JewelPoint_K = SyndicateInfoDataManager::instance()->getJewelPoint(SYNDICATE::eSYNDICATE_KAILUX);
	int JewelPoint_D = SyndicateInfoDataManager::instance()->getJewelPoint(SYNDICATE::eSYNDICATE_DEALERMOON);

	if( JewelPoint_K > 100 )
	{
		BOOST_FOREACH(CPC* pc, gserver->m_kailuxUser)
		{
			bool bApply;
			CSkill* skill = gserver->m_skillProtoList.Create(1758, 1);
			ApplySkill(pc, pc, skill, -1, bApply);
		}
	}
	if ( JewelPoint_D > 100 )
	{
		BOOST_FOREACH(CPC* pc, gserver->m_dealermoonUser)
		{
			bool bApply;
			CSkill* skill = gserver->m_skillProtoList.Create(1758, 1);
			ApplySkill(pc, pc, skill, -1, bApply);
		}
	}
#ifdef TLD_EVENT_SONG
	if(gserver->isActiveEvent(A_EVENT_SONGKRAN))
		gserver->checkTldEvent();
#endif
	//LOG_INFO("DEBUG_FUNC : END");
}

ServerTimerPerHour* ServerTimerPerHour::instance()
{
	static ServerTimerPerHour __instance;
	return &__instance;
}

void ServerTimerPerHour::Init()
{
	time_t nowtime = time(0);
	int remain_sec = 3600 - ((int)nowtime % 3600);

	bnf::instance()->CreateSecTimerPeriod(remain_sec, 3600, this);
}

//////////////////////////////////////////////////////////////////////////

void ServerTimerPerMidnight::operate( rnSocketIOService* service )
{
	LOG_INFO("Midnight timer is running.");

#ifdef SERVER_AUTHENTICATION
	ServerAuthentication::instance()->initConnect();
#endif
}

ServerTimerPerMidnight* ServerTimerPerMidnight::instance()
{
	static ServerTimerPerMidnight __instance;
	return &__instance;
}

void ServerTimerPerMidnight::Init()
{
	time_t nowtime = time(0);
	struct tm nowtm;
	memcpy(&nowtm, localtime(&nowtime), sizeof(nowtm));
	nowtm.tm_hour = 0;
	nowtm.tm_min = 0;
	nowtm.tm_sec = 0;

	time_t newtime = mktime(&nowtm);	// 오늘 0시 0분 0초
	newtime += 86400;					// 내일 0시 0분 0초

	int remain_sec = (int)(newtime - nowtime);

	bnf::instance()->CreateSecTimerPeriod(remain_sec, 86400, this);
}

//////////////////////////////////////////////////////////////////////////

void EndGameTimer::Init(int sec)
{
	if (run_flag_)
		return;

	bnf::instance()->CreateMSecTimer(sec * 1000, this);

	run_flag_ = true;
}

//XX endgame - 2
void EndGameTimer::operate( rnSocketIOService* service )
{
	CNetMsg::SP rmsg(new CNetMsg);
	MsgrEndGameReqMsg(rmsg);
	SEND_Q(rmsg, gserver->m_messenger);
}

EndGameTimer* EndGameTimer::instance()
{
	static EndGameTimer __instance;
	return &__instance;
}

//////////////////////////////////////////////////////////////////////////

void ShutdownTimer::Init( int sec )
{
	if (run_flag_)
		return;

	int start_sec = 0;
	if (sec > SHUTDONW_REBOOT_NOTIFY_SEC)
	{
		remain_sec_ = SHUTDONW_REBOOT_NOTIFY_SEC;
		start_sec = sec - SHUTDONW_REBOOT_NOTIFY_SEC;
	}
	else
	{
		remain_sec_ = sec;
		start_sec = 1;
	}

	bnf::instance()->CreateSecTimerPeriod(start_sec, 1, this);

	run_flag_ = true;
}

void ShutdownTimer::operate( rnSocketIOService* service )
{
	if (remain_sec_ <= 0)
	{
		{
			CNetMsg::SP rmsg(new CNetMsg);
			ConnRebootReqMsg(rmsg);
			SEND_Q(rmsg, gserver->m_connector);
		}
		{
			CNetMsg::SP rmsg(new CNetMsg);
			HelperShutdownMsg(rmsg);
			SEND_Q(rmsg, gserver->m_helper);
		}
		{
			CNetMsg::SP rmsg(new CNetMsg);
			SubHelperShutdownMsg(rmsg);
			SEND_Q(rmsg, gserver->m_subHelper);
		}

		PrintExcuteInfo::Instance()->SetStopMessage("expire shutdown timer");
		bnf::instance()->Stop();

		return;
	}

	{
		// 유저에게 서버 다운 남은 시간을 전송
		CNetMsg::SP rmsg(new CNetMsg);
		std::string tstr = boost::str(boost::format("%1%") % remain_sec_);
		SayMsg(rmsg, MSG_CHAT_NOTICE_SERVERDOWN, 0, "", "", tstr.c_str());
		PCManager::instance()->sendToAll(rmsg);
	}

	// 카운트 감소
	--remain_sec_;
}

ShutdownTimer* ShutdownTimer::instance()
{
	static ShutdownTimer __instance;
	return &__instance;
}

//////////////////////////////////////////////////////////////////////////

void RebootTimer::Init( int sec )
{
	if (run_flag_)
		return;

	int start_sec = 0;
	if (sec > SHUTDONW_REBOOT_NOTIFY_SEC)
	{
		remain_sec_ = SHUTDONW_REBOOT_NOTIFY_SEC;
		start_sec = sec - SHUTDONW_REBOOT_NOTIFY_SEC;
	}
	else
	{
		remain_sec_ = sec;
		start_sec = 1;
	}

	bnf::instance()->CreateSecTimerPeriod(start_sec, 1, this);

	run_flag_ = true;
}

void RebootTimer::operate( rnSocketIOService* service )
{
	if (remain_sec_ <= 0)
	{
		GAMELOG << init("shutdown by command") << end;

		PrintExcuteInfo::Instance()->SetStopMessage("expire reboot timer");
		bnf::instance()->Stop();
		return;
	}

	{
		// 유저에게 서버 다운 남은 시간을 전송
		CNetMsg::SP rmsg(new CNetMsg);
		std::string tstr = boost::str(boost::format("%1%") % remain_sec_);
		SayMsg(rmsg, MSG_CHAT_NOTICE_SERVERDOWN, 0, "", "", tstr.c_str());
		PCManager::instance()->sendToAll(rmsg);
	}

	--remain_sec_;
}

RebootTimer* RebootTimer::instance()
{
	static RebootTimer __instance;
	return &__instance;
}