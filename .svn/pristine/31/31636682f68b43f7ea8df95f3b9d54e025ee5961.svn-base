#include "stdhdrs.h"

#include "Log.h"
#include "Server.h"
#include "CmdMsg.h"
#include "doFunc.h"
#include "WarCastle.h"
#include "DratanCastle.h"

void do_ExCastleMapRecent(CPC* ch, CNetMsg::SP& msg)
{
	CWarCastle* castle = CWarCastle::GetCastleObject(CWarCastle::GetCurSubServerCastleZoneIndex());
	if (!castle)
		return ;
	if (ch->m_pZone->m_index != castle->GetZoneIndex())
		return ;

	switch (ch->GetJoinFlag(castle->GetZoneIndex()))
	{
	case WCJF_DEFENSE_GUILD:
	case WCJF_ATTACK_GUILD:
	case WCJF_OWNER:
	case WCJF_DEFENSE_CHAR:
	case WCJF_ATTACK_CHAR:
		break;

	default:
		return ;
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		ExCastleMapRecentMsg(rmsg, castle, ch);
		SEND_Q(rmsg, ch->m_desc);
	}
}

void do_ExCastleMapSignal(CPC* ch, CNetMsg::SP& msg)
{
	float x;
	float z;
	RefMsg(msg) >> x
				>> z;

	if (ch->m_guildInfo == NULL)
		return ;

	switch (ch->m_guildInfo->pos())
	{
	case MSG_GUILD_POSITION_BOSS:
	case MSG_GUILD_POSITION_OFFICER:
		break;

	default:
		return ;
	}

	CWarCastle* castle = CWarCastle::GetCastleObject(CWarCastle::GetCurSubServerCastleZoneIndex());
	if (!castle)
		return ;
	if (ch->m_pZone->m_index != castle->GetZoneIndex())
		return ;

	switch (ch->GetJoinFlag(castle->GetZoneIndex()))
	{
	case WCJF_DEFENSE_GUILD:
	case WCJF_ATTACK_GUILD:
		break;

	default:
		return ;
	}

	if (ch->m_guildInfo->guild() == NULL)
		return ;

	ch->m_guildInfo->guild()->guildSignal(ch->m_guildInfo->pos(), ch->m_index, x, z);

	{
		CNetMsg::SP rmsg(new CNetMsg);
		ExCastleMapSignalMsg(rmsg, ch->m_guildInfo->pos(), ch, x, z);
		ch->m_guildInfo->guild()->SendToAllInSameZone(rmsg, ch->m_pZone->m_index);
	}
}
