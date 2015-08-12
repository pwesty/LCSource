#include "stdhdrs.h"

#include "Server.h"
#include "Log.h"
#include "CmdMsg.h"
#include "WarCastle.h"
#include "DratanCastle.h"

void CheckWarCastle()
{
	meracCastle.CheckWarCastle(false);
	CDratanCastle * pCastle = CDratanCastle::CreateInstance();
	pCastle->CheckWarCastle();
}

bool LoadCastleData()
{
	// 메라크
	if (!meracCastle.LoadCastleData())
		return false;

#ifdef WARCASTLE_SUBNUMBER_MEMBER_VALUE
	// yhj... 러시아에서 올라 온 메라크 공성전때 느려진다는 것 때문에 패치
	// 글로벌 변수를 맴버 변수로 저장
	else
		meracCastle.m_subNumber = gserver->m_subno;
#endif // WARCASTLE_SUBNUMBER_MEMBER_VALUE

	// 드라탄(성인 서버만 드라탄 공성이 있다)
	CDratanCastle * pCastle = CDratanCastle::CreateInstance();
	if (pCastle->LoadCastleData() == false)
	{
		return false;
	}

	return true;
}

bool IsValidGuildCommandWithWarCastle(CPC* ch, unsigned char subtype)
{
	// 공성 참여 유저는 길드 관련 커맨드 중 일부를 할 수 없음
	int warZoneCount = 2;
	int warZoneIndex[] = { ZONE_MERAC, ZONE_DRATAN };

	int i;
	for (i = 0; i < warZoneCount; i++)
	{
		CWarCastle* castle = CWarCastle::GetCastleObject(warZoneIndex[i]);
		if (castle)
		{
			if (castle->GetState() != WCSF_NORMAL && ch->GetJoinFlag(warZoneIndex[i]) != WCJF_NONE)
			{
				switch (subtype)
				{
				case MSG_GUILD_CREATE:
				case MSG_GUILD_LEVELUP:
				case MSG_GUILD_BREAKUP:
				case MSG_GUILD_REGIST_REQ:
				case MSG_GUILD_REGIST_ALLOW:
				case MSG_GUILD_REGIST_CANCEL:
				case MSG_GUILD_OUT_REQ:
				case MSG_GUILD_KICK:
				case MSG_GUILD_CHANGE_BOSS:
				case MSG_GUILD_APPOINT_OFFICER:
				case MSG_GUILD_FIRE_OFFICER:
				case MSG_GUILD_BATTLE_REQ_REQ:
				case MSG_GUILD_BATTLE_REQ_REJECT:
				case MSG_GUILD_BATTLE_REQ_ACCEPT:
				case MSG_GUILD_BATTLE_STOP_REQ:
				case MSG_GUILD_BATTLE_STOP_REJECT:
				case MSG_GUILD_BATTLE_STOP_ACCEPT:
				case MSG_GUILD_MASTER_KICK_REQ:
				case MSG_GUILD_MASTER_KICK_CANCEL_REQ:
					return false;
				}
			}
		}
	}

	return true;
}

