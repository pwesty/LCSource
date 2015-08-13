#include <boost/format.hpp>
#include "stdhdrs.h"

#include "Log.h"
#include "Character.h"
#include "Server.h"
#include "CmdMsg.h"
#include "doFunc.h"
#include "../ShareLib/DBCmd.h"
#include "WarCastle.h"
#include "../ShareLib/packetType/ptype_old_do_item.h"
#include "../ShareLib/packetType/ptype_old_do_guild.h"

void do_Guild(CPC* ch, CNetMsg::SP& msg)
{
	unsigned char subtype;
	msg->MoveFirst();
	RefMsg(msg) >> subtype;

	if (!IsValidGuildCommandWithWarCastle(ch, subtype))
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildWarInvalidCommandMsg(rmsg, (MSG_GUILD_TYPE)subtype);
		SEND_Q(rmsg, ch->m_desc);

		return ;
	}

	switch (subtype)
	{
	case MSG_GUILD_CREATE:
		do_GuildCreate(ch, msg);
		break;

	case MSG_GUILD_LEVELUP:
		do_GuildLevelUp(ch, msg);
		break;

	case MSG_GUILD_BREAKUP:
		do_GuildBreakUp(ch, msg);
		break;

	case MSG_GUILD_REGIST_REQ:
		do_GuildRegistReq(ch, msg);
		break;

	case MSG_GUILD_REGIST_ALLOW:
		do_GuildRegistAllow(ch, msg);
		break;

	case MSG_GUILD_REGIST_CANCEL:
		do_GuildRegistCancel(ch, msg);
		break;

	case MSG_GUILD_OUT_REQ:
		do_GuildOutReq(ch, msg);
		break;

	case MSG_GUILD_KICK:
		do_GuildKick(ch, msg);
		break;

	case MSG_GUILD_CHANGE_BOSS:
		do_GuildChangeBoss(ch, msg);
		break;

	case MSG_GUILD_APPOINT_OFFICER:
		do_GuildAppointOfficer(ch, msg);
		break;

	case MSG_GUILD_FIRE_OFFICER:
		do_GuildFireOfficer(ch, msg);
		break;

	// 길드전
	case MSG_GUILD_BATTLE_REQ_REQ:
		do_GuildBattleReqReq(ch, msg);
		break;

	case MSG_GUILD_BATTLE_REQ_REJECT:
		do_GuildBattleReqReject(ch, msg);
		break;

	case MSG_GUILD_BATTLE_REQ_ACCEPT:
		do_GuildBattleReqAccept(ch, msg);
		break;

	case MSG_GUILD_BATTLE_STOP_REQ:
		do_GuildBattleStopReq(ch, msg);
		break;

	case MSG_GUILD_BATTLE_STOP_REJECT:
		do_GuildBattleStopReject(ch, msg);
		break;

	case MSG_GUILD_BATTLE_STOP_ACCEPT:
		do_GuildBattleStopAccept(ch, msg);
		break;

// 공성전

	case MSG_GUILD_WAR_SET_TIME_REQ:
		do_GuildWarSetTimeReq(ch, msg);
		break;

	case MSG_GUILD_WAR_GET_TIME:
		do_GuildWarGetTime(ch, msg);
		break;

	case MSG_GUILD_WAR_JOIN_ATTACK_GUILD:
		do_GuildWarJoinAttackGuild(ch, msg);
		break;

	case MSG_GUILD_WAR_JOIN_DEFENSE_GUILD:
		do_GuildWarJoinDefenseGuild(ch, msg);
		break;

	case MSG_GUILD_WAR_JOIN_ATTACK_CHAR:
		do_GuildWarJoinAttackChar(ch, msg);
		break;

	case MSG_GUILD_WAR_JOIN_DEFENSE_CHAR:
		do_GuildWarJoinDefenseChar(ch, msg);
		break;

	case MSG_GUILD_WAR_ATTACK_SYMBOL:
		do_GuildWarAttackSymbol(ch, msg);
		break;

	case MSG_GUILD_STASH_HISTORY_REQ:
		do_GuildStashHistoryReq(ch, msg);
		break;
	case MSG_GUILD_STASH_VIEW_REQ:
		do_GuildStashViewReq(ch, msg);
		break;
	case MSG_GUILD_STASH_TAKE_REQ:
		do_GuildStashTakeReq(ch, msg);
		break;
	case MSG_GUILD_WAR_SET_TIME_MENU:
		do_GuildWarSetTimeMenuReq(ch, msg);
		break;
	case MSG_GUILD_WAR_GET_TIME_UNI_REQ:
		do_GuildWarGetTimeUniReq(ch, msg);
		break;

	case MSG_NEW_GUILD_INCLINE_ESTABLISH:
		do_GuildInclineEstablist( ch, msg );
		break;
	case MSG_NEW_GUILD_MEMBER_ADJUST:
		do_GuildMemberAdjust( ch, msg );
		break;

	case MSG_NEW_GUILD_INFO:
		do_NewGuildInfo( ch, msg );
		break;
	case MSG_NEW_GUILD_MEMBERLIST:
		do_NewGuildMemberList( ch, msg );
		break;
	case MSG_NEW_GUILD_MANAGE:
		do_NewGuildManage( ch, msg );
		break;
	case MSG_NEW_GUILD_NOTICE:
		do_NewGuildNotice( ch, msg );
		break;
	case MSG_NEW_GUILD_NOTICE_UPDATE:
		do_NewGuildNoticeUpdate( ch, msg );
		break;
	case MSG_NEW_GUILD_NOTICE_REQUEST:
		do_NewGuildNoticeTrans( ch, msg );
		break;
	case MSG_NEW_GUILD_SKILL:
		do_NewGuildSkillList( ch, msg );
		break;

	case MSG_NEW_GUILD_SKILL_LEARN:
		do_NewGuildSkillLearn( ch, msg );
		break;

#ifdef DEV_GUILD_MARK
	case MSG_NEW_GUILD_MARK_EDIT_WND:
		do_newGuildSkillMarkEdit(ch, msg);
		break;
	case MSG_NEW_GUILD_MARK_EDIT_FIN:
		do_newGuildSkillMarkEditFinish(ch, msg);
		break;
#endif // GUILD_MARK

#ifdef DEV_GUILD_STASH
	case MSG_NEW_GUILD_STASH_LIST :
		do_newGuildStashList(ch, msg);
		break;
	case MSG_NEW_GUILD_STASH_KEEP :
		do_newGuildStashKeep(ch, msg);
		break;
	case MSG_NEW_GUILD_STASH_TAKE :
		do_newGuildStashTake(ch, msg);
		break;
	case MSG_NEW_GUILD_STASH_LOG :
		do_newGuildStashLog(ch, msg);
		break;
	case MSG_NEW_GUILD_STASH_LIST_MONEY:
		do_newGuildStashList(ch, msg);
		break;
#endif // DEV_GUILD_STASH

	case MSG_GUILD_MASTER_KICK_REQ:
		do_GuildMasterKickReq(ch, msg);
		break;
	case MSG_GUILD_MASTER_KICK_CANCEL_REQ:
		do_GuildMasterKickCancelReq(ch, msg);
		break;

#if defined (GUILD_REMOTE_INVITE)
	case MSG_GUILD_REMOTE_JOIN_REQ:
		do_GuildRemoteJoinReq(ch, msg);
		break;
	case MSG_GUILD_REMOTE_JOIN_OK:
		do_GuildRemoteJoinOK(ch, msg);
		break;
	case MSG_GUILD_REMOTE_JOIN_NO:
		do_GuildRemoteJoinNO(ch, msg);
		break;
#endif // GUILD_REMOTE_INVITE
	case MSG_GUILD_ROOM_RECALL:
		do_GuildRecall(ch, msg);
		break;
	default:
		break;
	}
}

void do_GuildCreate(CPC* ch, CNetMsg::SP& msg)
{
	CLCString name(MAX_GUILD_NAME_LENGTH + 1);

#ifdef NPC_CHECK
	int npcIndex;
	CNPC *npc;
	RefMsg(msg) >> npcIndex
				>> name;
	npc = ch->m_pArea->FindProtoNPCInCell(ch, npcIndex, false, 2);
	if(!npc)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_NOT_EXIST_NPC);
		SEND_Q(rmsg, ch->m_desc);
		GAMELOG << init("Guild Create FAIL: NOT EXIST NPC (Dangerous User)", ch) << end;
		return ;
	}
	if( !npc->IsFlag(NPC_GUILD) )
	{
		GAMELOG << init("Guild Create FAIL: NOT EXIST NPC (Dangerous User)", ch) << end;
		return ;
	}
#else
	RefMsg(msg) >> name;
#endif

	// 이름 검사
	int lenname = strlen(name);
	if (lenname < 2 || lenname > 16)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_GUILDNAME);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if (strchr(name, '\'') || strchr(name, '\\') || strchr(name, '\"') || strchr(name, '%') || strchr(name, '_') || strchr(name, ' '))
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_GUILDNAME);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

// [selo: 101115] 러시아는 러시아어 이외의 글자 들어가면 안된다.
#ifdef LC_RUS
	if( CheckNoRussianCharacter(name) )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_GUILDNAME);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
#endif // LC_RUS

	// 생성 조건 : 레벨 검사
	if (ch->m_level < GUILD_LEVEL1_NEED_LEVEL)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_CHARLEVEL);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 생성 조건 : SP 검사
	if (ch->m_skillPoint < GUILD_LEVEL1_NEED_SP * 10000)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg(rmsg, MSG_GUILD_ERRRO_SKILLPOINT);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 생성 조건 : 돈 검사
	if (ch->m_inventory.getMoney() < GUILD_LEVEL1_NEED_MONEY)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_MONEY);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 이전 길드 검사
	if (ch->m_guildInfo)
	{
		if (ch->m_guildInfo->pos() == MSG_GUILD_POSITION_BOSS)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			GuildErrorMsg(rmsg, MSG_GUILD_ERROR_ALREADYBOSS);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}
	}

	if (gserver->isRunHelper())
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildCreateReqMsg(rmsg, ch, name);
		RefMsg(rmsg) << GUILD_LEVEL1_MAX_MEMBER
					 << ch->m_pZone->m_index;

		SEND_Q(rmsg, gserver->m_helper);
	}
	else
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_GAMESERVER);
		SEND_Q(rmsg, ch->m_desc);
	}
}

void do_GuildLevelUp(CPC* ch, CNetMsg::SP& msg)
{
#ifdef NPC_CHECK
	int npcIndex;
	CNPC *npc;
	RefMsg(msg) >> npcIndex;
	npc = ch->m_pArea->FindProtoNPCInCell(ch, npcIndex, false, 2);
	if(!npc)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_NOT_EXIST_NPC);
		SEND_Q(rmsg, ch->m_desc);
		GAMELOG << init("GuildLevelUp FAIL: NOT EXIST NPC (Dangerous User)", ch) << end;
		return ;
	}
	if( !npc->IsFlag(NPC_GUILD) )
	{
		GAMELOG << init("GuildLevelUp FAIL: NOT EXIST NPC (Dangerous User)", ch) << end;
		return ;
	}
#endif

	if (ch->m_guildInfo == NULL)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_NOGUILD);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	CGuild* guild = ch->m_guildInfo->guild();
	if (guild->boss()->charindex() != ch->m_index)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_NOTBOSS);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	int needlevel		= 0x7fffffff;
	LONGLONG needmoney	= 0x7fffffff;
	int needsp			= 0x7fffffff;
	int	needgp			= 0x7fffffff;
	int gap				= 0x7fffffff;

	if( guild->level() < GUILD_EXTEND_BASE_LEVEL  )
	{
		switch (guild->level())
		{
		case 1:
			needlevel = GUILD_LEVEL2_NEED_LEVEL;
			needmoney = GUILD_LEVEL2_NEED_MONEY;
			needsp = GUILD_LEVEL2_NEED_SP;
			break;

		case 2:
			needlevel = GUILD_LEVEL3_NEED_LEVEL;
			needmoney = GUILD_LEVEL3_NEED_MONEY;
			needsp = GUILD_LEVEL3_NEED_SP;
			break;

		case 3:
			needlevel = GUILD_LEVEL4_NEED_LEVEL;
			needmoney = GUILD_LEVEL4_NEED_MONEY;
			needsp = GUILD_LEVEL4_NEED_SP;
			break;

		case 4:
			needlevel = GUILD_LEVEL5_NEED_LEVEL;
			needmoney = GUILD_LEVEL5_NEED_MONEY;
			needsp = GUILD_LEVEL5_NEED_SP;
			break;
		case 5:
			needlevel = GUILD_LEVEL6_NEED_LEVEL;
			needmoney = GUILD_LEVEL6_NEED_MONEY;
			needsp = GUILD_LEVEL6_NEED_SP;
			break;
		case 6:
			needlevel = GUILD_LEVEL7_NEED_LEVEL;
			needmoney = GUILD_LEVEL7_NEED_MONEY;
			needsp = GUILD_LEVEL7_NEED_SP;
			needgp	= GUILD_LEVEL7_NEED_GP;
			break;
		}
	}
	else if( guild->level() >= GUILD_EXTEND_BASE_LEVEL )
	{
		if( guild->level() > 4 && guild->level() < 11 )
			needlevel = GUILD_LEVEL7_NEED_LEVEL;
		else if( guild->level() > 10 && guild->level() < 20 )
			needlevel = GUILD_LEVEL7_NEED_LEVEL + 10;
		else if( guild->level() > 19 && guild->level() < 30 )
			needlevel = GUILD_LEVEL7_NEED_LEVEL + 20;
		else if( guild->level() > 29 && guild->level() < 40 )
			needlevel = GUILD_LEVEL7_NEED_LEVEL + 30;
		else if( guild->level() > 39 && guild->level() < 50 )
			needlevel = GUILD_LEVEL7_NEED_LEVEL + 40;
		else
			needlevel = GUILD_LEVEL7_NEED_LEVEL + 50;

		gap = guild->level() + 1 - GUILD_EXTEND_BASE_LEVEL;
		if( gap >= 0 )
		{
			needmoney	= (LONGLONG)(GUILD_LEVEL7_NEED_MONEY * pow(1.09, gap));
			needsp		= (int)(GUILD_LEVEL7_NEED_SP * pow(1.09, gap));
			needgp		= (int)(GUILD_LEVEL7_NEED_GP * pow(1.09, gap));
		}
//		else
//		{
//			needmoney	= GUILD_LEVEL7_NEED_MONEY;
//			needsp		= GUILD_LEVEL7_NEED_SP;
//			needgp		= GUILD_LEVEL7_NEED_GP;
//		}
	}

	// 생성 조건
	if( guild->level() >= GUILD_EXTEND_BASE_LEVEL  - 1 )
	{
		if( guild->GetGuildPoint() < needgp )
		{
			CNetMsg::SP rmsg(new CNetMsg);
			GuildErrorMsg( rmsg, MSG_NEW_GUILD_ERRROR_GUILDPOINT );
			SEND_Q( rmsg, ch->m_desc );
			return;
		}
	}

	// 레벨 검사
	if (ch->m_level < needlevel)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_CHARLEVEL);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 생성 조건 : SP 검사
	if (ch->m_skillPoint < needsp * 10000)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg(rmsg, MSG_GUILD_ERRRO_SKILLPOINT);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 생성 조건 : 돈 검사
	if (ch->m_inventory.getMoney() < needmoney)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_MONEY);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// TODO : GUILD : 트리구조시 상위 길드와의 관계 고려
	if (gserver->isRunHelper())
	{
		int guildmaxmember = 0;
		switch( guild->level() +1 )
		{
		case 1:
			guildmaxmember = GUILD_LEVEL1_MAX_MEMBER;
			break;
		case 2:
			guildmaxmember = GUILD_LEVEL2_MAX_MEMBER;
			break;
		case 3:
			guildmaxmember = GUILD_LEVEL3_MAX_MEMBER;
			break;
		case 4:
			guildmaxmember = GUILD_LEVEL4_MAX_MEMBER;
			break;
		case 5:
			guildmaxmember = GUILD_LEVEL5_MAX_MEMBER;
			break;
		case 6:
			guildmaxmember = GUILD_LEVEL6_MAX_MEMBER;
			break;
		default :
			{
				gap = guild->level() + 1 - GUILD_EXTEND_BASE_LEVEL;
				//	gap = guild->level() - GUILD_EXTEND_BASE_LEVEL;
				if( gap >= 0 )
				{
					if( guild->m_passiveSkillList.Find( 444 ) )
					{
						CSkill* skill = guild->m_passiveSkillList.Find( 444 );

						if(skill->m_level <= 5)
							guildmaxmember = GUILD_LEVEL6_MAX_MEMBER + (5 * skill->m_level );
						else
							guildmaxmember = 55 + 9 * (skill->m_level - 5);
					}
					else
						guildmaxmember = GUILD_LEVEL6_MAX_MEMBER;
				}
			}
			break;
		}

		{
			CNetMsg::SP rmsg(new CNetMsg);
			HelperGuildLevelUpReqMsg(rmsg, guild->index(), ch->m_index);
			RefMsg(rmsg) << guildmaxmember;
			SEND_Q(rmsg, gserver->m_helper);
		}
	}
	else
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_GAMESERVER);
		SEND_Q(rmsg, ch->m_desc);
	}
}

void do_GuildBreakUp(CPC* ch, CNetMsg::SP& msg)
{
#ifdef NPC_CHECK
	int npcIndex;
	CNPC *npc;

	RefMsg(msg) >> npcIndex;
	npc = ch->m_pArea->FindProtoNPCInCell(ch, npcIndex, false, 2);
	if(!npc)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_NOT_EXIST_NPC);
		SEND_Q(rmsg, ch->m_desc);
		GAMELOG << init("Castle BreakUp FAIL: NOT EXIST NPC (Dangerous User)", ch) << end;
		return ;
	}
	if( npc->IsFlag(NPC_GUILD) )
	{
		GAMELOG << init("Castle BreakUp FAIL: NOT EXIST NPC (Dangerous User)", ch) << end;
		return ;
	}
#endif
	if (ch->m_guildInfo == NULL)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_NOGUILD);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if (ch->m_guildInfo->guild()->boss()->charindex() != ch->m_index)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_NOTBOSS);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if (ch->m_guildInfo->guild()->battleState() != GUILD_BATTLE_STATE_PEACE)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_CANNOT_BREAK_BATTLE);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	int ownZoneCount = 0;
	int* ownZone = CWarCastle::GetOwnCastle(ch->m_guildInfo->guild()->index(), &ownZoneCount);
	if (ownZone)
	{
		int i;
		for (i = 0; i < ownZoneCount; i++)
		{
			CWarCastle* castle = CWarCastle::GetCastleObject(ownZone[i]);
			if (castle)
			{
				if(castle->GetState() != WCSF_NORMAL || castle->IsJoinTime())
				{
					CNetMsg::SP rmsg(new CNetMsg);
					GuildErrorMsg(rmsg, MSG_GUILD_ERROR_CANNOT_BREAK_WAR);
					SEND_Q(rmsg, ch->m_desc);
					return ;
				}
			}
		}
		delete [] ownZone;
	}

	if (gserver->isRunHelper())
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildBreakUpReqMsg(rmsg, ch);
		SEND_Q(rmsg, gserver->m_helper);
	}
	else
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_NOTBOSS);
		SEND_Q(rmsg, ch->m_desc);
	}
}

void do_GuildRegistReq(CPC* ch, CNetMsg::SP& msg)
{
	int guildindex;
	int bossindex;
	int requesterindex;

	RefMsg(msg) >> guildindex
				>> bossindex
				>> requesterindex;

	if (ch->m_pZone->isRVRZone())
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_CANNOT_REGISTER_REQ_ZONE);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if (ch->m_guildoutdate != 0)
	{
		time_t t;
		if (time(&t) / 60 / 60 / 24 < ch->m_guildoutdate + GUILD_REG_DELAY)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			GuildErrorMsg(rmsg, MSG_GUILD_ERROR_REGDELAY);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}
	}

	if (bossindex == ch->m_index || requesterindex != ch->m_index)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_PACKET);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	CGuild* guild = gserver->m_guildlist.findguild(guildindex);
	if (!guild)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_TARGET_NOBOSS);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	CGuildMember* member = guild->findmember(bossindex);
	if (member == NULL)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_TARGET_NOBOSS);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
	if (member->pos() == MSG_GUILD_POSITION_MEMBER)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_TARGET_NOBOSS);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if (member->GetPC() == NULL)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_TARGET_NOBOSS);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if (member->GetPC()->m_regGuild && member->GetPC()->m_regGuild != ch->m_index)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_TARGET_BUSY);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if (ch->m_regGuild && ch->m_regGuild != member->charindex())
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_SOURCE_BUSY);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if (guild->battleState() != GUILD_BATTLE_STATE_PEACE)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_CANNOT_REGIST_REQ_BATTLE);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	member->GetPC()->m_regGuild = ch->m_index;
	ch->m_regGuild = member->charindex();

	SEND_Q(msg, ch->m_desc);
	SEND_Q(msg, member->GetPC()->m_desc);
}

void do_GuildRegistAllow(CPC* ch, CNetMsg::SP& msg)
{
	int guildindex;
	int requesterindex;

	RefMsg(msg) >> guildindex
				>> requesterindex;

	CPC* pc = PCManager::instance()->getPlayerByCharIndex(requesterindex);
	if (!pc)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildRegistCancelMsg(rmsg, false);
		SEND_Q(rmsg, ch->m_desc);
		ch->m_regGuild = 0;
		return ;
	}

	if (pc->m_regGuild != ch->m_index || ch->m_regGuild != pc->m_index)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_NOGUILD);
		SEND_Q(rmsg, ch->m_desc);
		SEND_Q(rmsg, pc->m_desc);
		ch->m_regGuild = 0;
		pc->m_regGuild = 0;
		return ;
	}

	CGuild* guild = gserver->m_guildlist.findguild(guildindex);
	if (!guild)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildRegistCancelMsg(rmsg, true);
		SEND_Q(rmsg, ch->m_desc);
		SEND_Q(rmsg, pc->m_desc);
		ch->m_regGuild = 0;
		pc->m_regGuild = 0;
		return ;
	}

	if (guild->maxmember() <= guild->membercount())
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_FULLMEMBER);
		SEND_Q(rmsg, ch->m_desc);
		SEND_Q(rmsg, pc->m_desc);
		ch->m_regGuild = 0;
		pc->m_regGuild = 0;
		return ;
	}

	// TODO : GUILD : 대상이 이미 다른 길드에 소속되어 있다면, 트리 구성시 길드 간의 레벨을 검사

	CGuildMember* member = guild->findmember(ch->m_index);
	if (!member)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildRegistCancelMsg(rmsg, true);
		SEND_Q(rmsg, ch->m_desc);
		SEND_Q(rmsg, pc->m_desc);
		ch->m_regGuild = 0;
		pc->m_regGuild = 0;
		return ;
	}
	if (member->pos() == MSG_GUILD_POSITION_MEMBER)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildRegistCancelMsg(rmsg, true);
		SEND_Q(rmsg, ch->m_desc);
		SEND_Q(rmsg, pc->m_desc);
		ch->m_regGuild = 0;
		pc->m_regGuild = 0;
		return ;
	}
	if (member->GetPC() == NULL)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildRegistCancelMsg(rmsg, true);
		SEND_Q(rmsg, ch->m_desc);
		SEND_Q(rmsg, pc->m_desc);
		ch->m_regGuild = 0;
		pc->m_regGuild = 0;
		return ;
	}

	if (gserver->isRunHelper())
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildMemberAddReqMsg(rmsg, guildindex, member->charindex(), pc->m_index, pc->GetName());
		RefMsg(rmsg) << pc->m_pZone->m_index;
		SEND_Q(rmsg, gserver->m_helper);
	}
	else
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_GAMESERVER);
		SEND_Q(rmsg, ch->m_desc);
		SEND_Q(rmsg, pc->m_desc);
		ch->m_regGuild = 0;
		pc->m_regGuild = 0;
		return ;
	}
}

void do_GuildRegistCancel(CPC* ch, CNetMsg::SP& msg)
{
	if (ch->m_regGuild == 0)
		return ;

	CPC* pc = PCManager::instance()->getPlayerByCharIndex(ch->m_regGuild);
	if (pc)
	{
		if (pc->m_regGuild == ch->m_index)
		{
			pc->m_regGuild = 0;
			SEND_Q(msg, pc->m_desc);
		}
	}
	ch->m_regGuild = 0;
	SEND_Q(msg, ch->m_desc);
}

void do_GuildOutReq(CPC* ch, CNetMsg::SP& msg)
{
	int guildindex;
	RefMsg(msg) >> guildindex;

	CGuild* guild = gserver->m_guildlist.findguild(guildindex);
	if (!guild)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_NOGUILD);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if (guild->battleState() != GUILD_BATTLE_STATE_PEACE)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_CANNOT_OUT_BATTLE);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	CGuildMember* member = guild->findmember(ch->m_index);
	if (!member)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_NOGUILD);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if (member->pos() == MSG_GUILD_POSITION_BOSS)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_OUTBOSS);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if (gserver->isRunHelper())
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildOutReqMsg(rmsg, guildindex, ch->m_index);
		SEND_Q(rmsg, gserver->m_helper);
	}
	else
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_GAMESERVER);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
}

void do_GuildKick(CPC* ch, CNetMsg::SP& msg)
{
	int guildindex;
	int charindex;

	RefMsg(msg) >> guildindex
				>> charindex;

	CGuild* guild = gserver->m_guildlist.findguild(guildindex);
	if (!guild)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_NOTFOUNDMEMBER);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if (ch->m_guildInfo == NULL)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_NOGUILD);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if (ch->m_guildInfo->guild() != guild)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_NOGUILD);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if (ch->m_guildInfo->pos() == MSG_GUILD_POSITION_MEMBER)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_NOTBOSS);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if (guild->battleState() != GUILD_BATTLE_STATE_PEACE)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_CANNOT_KICK_BATTLE);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	CGuildMember* member = guild->findmember(charindex);
	if (!member)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_NOTFOUNDMEMBER);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if (member->pos() == MSG_GUILD_POSITION_BOSS)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_CANTKICKBOSS);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if (gserver->isRunHelper())
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildKickReqMsg(rmsg, guildindex, ch->m_index, charindex);
		SEND_Q(rmsg, gserver->m_helper);
	}
	else
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_GAMESERVER);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
}

void do_GuildChangeBoss(CPC* ch, CNetMsg::SP& msg)
{
	int guildindex;
	int current;
	int change;

	RefMsg(msg) >> guildindex
				>> current
				>> change;

	if (ch->m_index != current)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_NOTBOSS);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	CGuild* guild = gserver->m_guildlist.findguild(guildindex);
	if (!guild)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_NOGUILD);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	CGuildMember* boss = guild->findmember(current);
	if (!boss)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_NOTBOSS);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if (boss->pos() != MSG_GUILD_POSITION_BOSS)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_NOTBOSS);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	CGuildMember* member = guild->findmember(change);
	if (!member)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_NOTFOUNDMEMBER);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if (member->pos() == MSG_GUILD_POSITION_BOSS)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_PACKET);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	CPC * pc = member->GetPC();
	if ( !pc || pc->m_level < GUILD_LEVEL1_NEED_LEVEL)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg(rmsg,MSG_GUILD_ERROR_CHARLEVEL);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

#ifdef LC_GAMIGO
	if(pc->m_level < guild->BossChangeNeedPCLevel())
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg(rmsg,MSG_GUILD_ERROR_CHARLEVEL);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
#endif // LC_GAMIGO

	if (gserver->isRunHelper())
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildChangeBossReqMsg(rmsg, guildindex, current, change);
		SEND_Q(rmsg, gserver->m_helper);
	}
	else
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_GAMESERVER);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
}

void do_GuildAppointOfficer(CPC* ch, CNetMsg::SP& msg)
{
	int guildindex;
	int charindex;

	RefMsg(msg) >> guildindex
				>> charindex;

	CGuild* guild = gserver->m_guildlist.findguild(guildindex);
	if (!guild)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_NOGUILD);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

//	int i;
//	for (i = 0; i < GUILD_MAX_OFFICER; i++)
//	{
//		if (guild->officer(i) == NULL)
//			break;
//	}
//	if (i == GUILD_MAX_OFFICER)
//	{
//		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_FULLOFFICER);
//		SEND_Q(rmsg, ch->m_desc);
//		return ;
//	}

	CGuildMember* boss = guild->findmember(ch->m_index);
	if (!boss)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_NOTBOSS);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if (boss->pos() != MSG_GUILD_POSITION_BOSS)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_NOTBOSS);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	CGuildMember* member = guild->findmember(charindex);
	if (!member)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_NOTFOUNDMEMBER);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if (member->pos() != MSG_GUILD_POSITION_MEMBER)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_CANTOFFICER);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if (gserver->isRunHelper())
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildAppointOfficerReqMsg(rmsg, guildindex, ch->m_index, charindex);
		SEND_Q(rmsg, gserver->m_helper);
	}
	else
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_GAMESERVER);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
}

void do_GuildFireOfficer(CPC* ch, CNetMsg::SP& msg)
{
	int guildindex;
	int charindex;

	RefMsg(msg) >> guildindex
				>> charindex;

	CGuild* guild = gserver->m_guildlist.findguild(guildindex);
	if (!guild)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_NOGUILD);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	CGuildMember* boss = guild->findmember(ch->m_index);
	if (!boss)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_NOTBOSS);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if (boss->pos() != MSG_GUILD_POSITION_BOSS)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_NOTBOSS);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	CGuildMember* member = guild->findmember(charindex);
	if (!member)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_NOTFOUNDMEMBER);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if (member->pos() != MSG_GUILD_POSITION_OFFICER)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_NOTOFFICER);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if (gserver->isRunHelper())
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildFireOfficerReqMsg(rmsg, guildindex, ch->m_index, charindex);
		SEND_Q(rmsg, gserver->m_helper);
	}
	else
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_GAMESERVER);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
}

void do_GuildBattleReqReq(CPC* ch, CNetMsg::SP& msg)
{
	int charindex;
	int prize, time;

	RefMsg(msg) >> charindex
				>> prize
				>> time;

	// 길드 없음
	if (!ch->m_guildInfo)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildBattleErrMsg(rmsg, MSG_GUILD_ERROR_BATTLE_NOGUILD);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 길드장이 아님
	if (ch->m_guildInfo->guild()->boss() && ch->m_guildInfo->guild()->boss()->charindex() != ch->m_index)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildBattleErrMsg(rmsg, MSG_GUILD_ERROR_BATTLE_NOTBOSS);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 10명이상 길드
	if (ch->m_guildInfo->guild()->membercount() < GUILD_BATTLE_MIN_MEMBER)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildBattleErrMsg(rmsg, MSG_GUILD_ERROR_BATTLE_MEMBERCOUNT);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 길드전이 가능한 존에 있어야
	if (ch->m_pZone->m_index != ZONE_START && ch->m_pZone->m_index != ZONE_DRATAN
			&& ch->m_pZone->m_index != ZONE_MERAC && ch->m_pZone->m_index != ZONE_EGEHA
			&& ch->m_pZone->m_index != ZONE_STREIANA && ch->m_pZone->m_index != ZONE_MONDSHINE
			&& ch->m_pZone->m_index != ZONE_TARIAN && ch->m_pZone->m_index != ZONE_BLOODYMIR)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildBattleErrMsg(rmsg, MSG_GUILD_ERROR_BATTLE_ZONE);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 이미 길드전 중
	if (ch->m_guildInfo->guild()->battleState() != GUILD_BATTLE_STATE_PEACE)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildBattleErrMsg(rmsg, MSG_GUILD_ERROR_ALREADY_BATTLE);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 배팅금액 오류
	if (prize * 10000 < GUILD_BATTLE_MIN_PRIZE || prize * 10000 > GUILD_BATTLE_MAX_PRIZE)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildBattleErrMsg(rmsg, MSG_GUILD_ERROR_BATTLE_PRIZE);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 배팅액 모질람
	if (ch->m_inventory.getMoney() < (LONGLONG)prize)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildBattleErrMsg(rmsg, MSG_GUILD_ERROR_BATTLE_PRIZE);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 시간 초과 검사
	if (time * 10 < GUILD_BATTLE_MIN_TIME || time * 10 > GUILD_BATTLE_MAX_TIME)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildBattleErrMsg(rmsg, MSG_GUILD_ERROR_BATTLE_TIME);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 상대가 이상함
	CPC* tch = PCManager::instance()->getPlayerByCharIndex(charindex);
	if (!tch || !tch->m_guildInfo)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildBattleErrMsg(rmsg, MSG_GUILD_ERROR_BATTLE_NOGUILD);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 상대가 길드장이 아님
	if (tch->m_guildInfo->guild()->boss() && tch->m_guildInfo->guild()->boss()->charindex() != tch->m_index)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildBattleErrMsg(rmsg, MSG_GUILD_ERROR_BATTLE_NOTBOSS);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 상대가 10명 미만 길드
	if (tch->m_guildInfo->guild()->membercount() < GUILD_BATTLE_MIN_MEMBER)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildBattleErrMsg(rmsg, MSG_GUILD_ERROR_BATTLE_MEMBERCOUNT);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 이미 길드전 중
	if (tch->m_guildInfo->guild()->battleState() != GUILD_BATTLE_STATE_PEACE)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildBattleErrMsg(rmsg, MSG_GUILD_ERROR_ALREADY_BATTLE);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 길드전이 가능한 존에 있어야
	if (tch->m_pZone->m_index != ZONE_START && tch->m_pZone->m_index != ZONE_DRATAN
			&& tch->m_pZone->m_index != ZONE_MERAC && tch->m_pZone->m_index != ZONE_EGEHA
			&& tch->m_pZone->m_index != ZONE_STREIANA && tch->m_pZone->m_index != ZONE_MONDSHINE
			&& tch->m_pZone->m_index != ZONE_TARIAN && tch->m_pZone->m_index != ZONE_BLOODYMIR)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildBattleErrMsg(rmsg, MSG_GUILD_ERROR_BATTLE_ZONE);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 셋팅
	ch->m_guildInfo->guild()->BattleSet(tch->m_guildInfo->guild()->index(), prize * 10000, ch->m_pZone->m_index);
	ch->m_guildInfo->guild()->BattleState(GUILD_BATTLE_STATE_START_WAIT);
	ch->m_guildInfo->guild()->BattleTime(GUILD_BATTLE_WAIT_TIME + time * 10);

	tch->m_guildInfo->guild()->BattleSet(ch->m_guildInfo->guild()->index(), prize * 10000, tch->m_pZone->m_index);
	tch->m_guildInfo->guild()->BattleState(GUILD_BATTLE_STATE_START_WAIT);
	tch->m_guildInfo->guild()->BattleTime(GUILD_BATTLE_WAIT_TIME + time * 10);

	{
		// 요청 msg
		CNetMsg::SP rmsg(new CNetMsg);
		GuildBattleReqReqMsg(rmsg, ch->m_guildInfo->guild()->index(), ch->m_guildInfo->guild()->name(), prize, time);
		SEND_Q(rmsg, tch->m_desc);
	}
}

void do_GuildBattleReqReject(CPC* ch, CNetMsg::SP& msg)
{
	if (!ch->m_guildInfo)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildBattleErrMsg(rmsg, MSG_GUILD_ERROR_BATTLE_NOGUILD);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 신청받은적 없음
	if (ch->m_guildInfo->guild()->battleState() != GUILD_BATTLE_STATE_START_WAIT)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildBattleErrMsg(rmsg, MSG_GUILD_ERROR_BATTLE_NOT_REQ);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 길드장이 아님
	if (ch->m_guildInfo->guild()->boss() && ch->m_guildInfo->guild()->boss()->charindex() != ch->m_index)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildBattleErrMsg(rmsg, MSG_GUILD_ERROR_BATTLE_NOTBOSS);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 상대방 길드
	CGuild* guild = gserver->m_guildlist.findguild(ch->m_guildInfo->guild()->battleIndex());

	// 없음
	if (!guild)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildBattleErrMsg(rmsg, MSG_GUILD_ERROR_BATTLE_NOGUILD);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 상대방이 신청한적 없음
	if (guild->battleState() != GUILD_BATTLE_STATE_START_WAIT)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildBattleErrMsg(rmsg, MSG_GUILD_ERROR_BATTLE_NOT_REQ);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 상대방 길드장
	CPC* tch = guild->boss()->GetPC();

	// 상대방 길드장이 없음
	if (!tch)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildBattleErrMsg(rmsg, MSG_GUILD_ERROR_BATTLE_NOGUILD);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 셋팅 해제
	ch->m_guildInfo->guild()->BattleSet(-1, 0, -1);
	ch->m_guildInfo->guild()->BattleState(GUILD_BATTLE_STATE_PEACE);
	ch->m_guildInfo->guild()->BattleTime(0);

	guild->BattleSet(-1, 0, -1);
	guild->BattleState(GUILD_BATTLE_STATE_PEACE);
	guild->BattleTime(0);

	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildBattleReqRejectMsg(rmsg, ch->m_index);
		SEND_Q(rmsg, ch->m_desc);
		SEND_Q(rmsg, tch->m_desc);
	}
}

void do_GuildBattleReqAccept(CPC* ch, CNetMsg::SP& msg)
{
	if (!ch->m_guildInfo)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildBattleErrMsg(rmsg, MSG_GUILD_ERROR_BATTLE_NOGUILD);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 신청받은적 없음
	if (ch->m_guildInfo->guild()->battleState() != GUILD_BATTLE_STATE_START_WAIT)
	{
		{
			CNetMsg::SP rmsg(new CNetMsg);
			GuildBattleErrMsg(rmsg, MSG_GUILD_ERROR_BATTLE_NOT_REQ);
			SEND_Q(rmsg, ch->m_desc);
		}

		ch->m_guildInfo->guild()->BattleSet(-1, 0, -1);
		ch->m_guildInfo->guild()->BattleState(GUILD_BATTLE_STATE_PEACE);
		ch->m_guildInfo->guild()->BattleTime(0);

		return;
	}

	// 길드장이 아님
	if (ch->m_guildInfo->guild()->boss() && ch->m_guildInfo->guild()->boss()->charindex() != ch->m_index)
	{
		{
			CNetMsg::SP rmsg(new CNetMsg);
			GuildBattleErrMsg(rmsg, MSG_GUILD_ERROR_BATTLE_NOTBOSS);
			SEND_Q(rmsg, ch->m_desc);
		}

		ch->m_guildInfo->guild()->BattleSet(-1, 0, -1);
		ch->m_guildInfo->guild()->BattleState(GUILD_BATTLE_STATE_PEACE);
		ch->m_guildInfo->guild()->BattleTime(0);

		return;
	}

	// 상대방 길드
	CGuild* guild = gserver->m_guildlist.findguild(ch->m_guildInfo->guild()->battleIndex());

	// 상대방 길드가 없음
	if (!guild)
	{
		{
			CNetMsg::SP rmsg(new CNetMsg);
			GuildBattleErrMsg(rmsg, MSG_GUILD_ERROR_BATTLE_NOGUILD);
			SEND_Q(rmsg, ch->m_desc);
		}

		ch->m_guildInfo->guild()->BattleSet(-1, 0, -1);
		ch->m_guildInfo->guild()->BattleState(GUILD_BATTLE_STATE_PEACE);
		ch->m_guildInfo->guild()->BattleTime(0);

		return;
	}

	// 상대방 길드장
	CPC* tch = guild->boss()->GetPC();

	// 상대방 길드장이 없음
	if (!tch)
	{
		{
			CNetMsg::SP rmsg(new CNetMsg);
			GuildBattleErrMsg(rmsg, MSG_GUILD_ERROR_BATTLE_NOGUILD);
			SEND_Q(rmsg, ch->m_desc);
		}

		ch->m_guildInfo->guild()->BattleSet(-1, 0, -1);
		ch->m_guildInfo->guild()->BattleState(GUILD_BATTLE_STATE_PEACE);
		ch->m_guildInfo->guild()->BattleTime(0);

		guild->BattleSet(-1, 0, -1);
		guild->BattleState(GUILD_BATTLE_STATE_PEACE);
		guild->BattleTime(0);

		return;
	}

	// 배팅액이 모질라
	if (ch->m_inventory.getMoney() < ch->m_guildInfo->guild()->battlePrize())
	{
		{
			CNetMsg::SP rmsg(new CNetMsg);
			GuildBattleErrMsg(rmsg, MSG_GUILD_ERROR_BATTLE_SHORT_PRIZE);
			SEND_Q(rmsg, ch->m_desc);
			SEND_Q(rmsg, tch->m_desc);
		}

		ch->m_guildInfo->guild()->BattleSet(-1, 0, -1);
		ch->m_guildInfo->guild()->BattleState(GUILD_BATTLE_STATE_PEACE);
		ch->m_guildInfo->guild()->BattleTime(0);

		guild->BattleSet(-1, 0, -1);
		guild->BattleState(GUILD_BATTLE_STATE_PEACE);
		guild->BattleTime(0);

		return;
	}

	// 배팅금액이 없음
	if (tch->m_inventory.getMoney() < guild->battlePrize())
	{
		{
			CNetMsg::SP rmsg(new CNetMsg);
			GuildBattleErrMsg(rmsg, MSG_GUILD_ERROR_BATTLE_SHORT_PRIZE);
			SEND_Q(rmsg, ch->m_desc);
			SEND_Q(rmsg, tch->m_desc);
		}

		ch->m_guildInfo->guild()->BattleSet(-1, 0, -1);
		ch->m_guildInfo->guild()->BattleState(GUILD_BATTLE_STATE_PEACE);
		ch->m_guildInfo->guild()->BattleTime(0);

		guild->BattleSet(-1, 0, -1);
		guild->BattleState(GUILD_BATTLE_STATE_PEACE);
		guild->BattleTime(0);
		return;
	}

	// 셋팅된 금액이 틀림
	if (ch->m_guildInfo->guild()->battlePrize() != guild->battlePrize())
	{
		{
			CNetMsg::SP rmsg(new CNetMsg);
			GuildBattleErrMsg(rmsg, MSG_GUILD_ERROR_BATTLE_PRIZE);
			SEND_Q(rmsg, ch->m_desc);
			SEND_Q(rmsg, tch->m_desc);
		}

		ch->m_guildInfo->guild()->BattleSet(-1, 0, -1);
		ch->m_guildInfo->guild()->BattleState(GUILD_BATTLE_STATE_PEACE);
		ch->m_guildInfo->guild()->BattleTime(0);

		guild->BattleSet(-1, 0, -1);
		guild->BattleState(GUILD_BATTLE_STATE_PEACE);
		guild->BattleTime(0);

		return;
	}

	// 셋팅된 존이 틀림
	if (ch->m_guildInfo->guild()->battleZone() != guild->battleZone())
	{
		{
			CNetMsg::SP rmsg(new CNetMsg);
			GuildBattleErrMsg(rmsg, MSG_GUILD_ERROR_BATTLE_ZONE);
			SEND_Q(rmsg, ch->m_desc);
			SEND_Q(rmsg, tch->m_desc);
		}

		ch->m_guildInfo->guild()->BattleSet(-1, 0, -1);
		ch->m_guildInfo->guild()->BattleState(GUILD_BATTLE_STATE_PEACE);
		ch->m_guildInfo->guild()->BattleTime(0);

		guild->BattleSet(-1, 0, -1);
		guild->BattleState(GUILD_BATTLE_STATE_PEACE);
		guild->BattleTime(0);

		return;
	}

	if (gserver->isRunHelper())
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildBattleReqMsg(rmsg, ch->m_guildInfo->guild()->index(), guild->index(), guild->battlePrize(), guild->battleZone(), guild->battleTime());
		SEND_Q(rmsg, gserver->m_helper);
	}
	else
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_GAMESERVER);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
}

void do_GuildBattleStopReq(CPC* ch, CNetMsg::SP& msg)
{
	int charindex;

	RefMsg(msg) >> charindex;

	if (!ch->m_guildInfo)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildBattleErrMsg(rmsg, MSG_GUILD_ERROR_BATTLE_NOGUILD);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 길드전 중 아님
	if (ch->m_guildInfo->guild()->battleState() != GUILD_BATTLE_STATE_ING)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildBattleErrMsg(rmsg, MSG_GUILD_ERROR_BATTLE_NOT_BATTLE);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 길드장이 아님
	if (ch->m_guildInfo->guild()->boss() && ch->m_guildInfo->guild()->boss()->charindex() != ch->m_index)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildBattleErrMsg(rmsg, MSG_GUILD_ERROR_BATTLE_NOTBOSS);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 상대방 길드
	CGuild* guild = gserver->m_guildlist.findguild(ch->m_guildInfo->guild()->battleIndex());

	if (!guild)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildBattleErrMsg(rmsg, MSG_GUILD_ERROR_BATTLE_NOGUILD);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	CPC* tch = guild->boss()->GetPC();

	// 상대방 길드장이 없음
	if (!tch)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildBattleErrMsg(rmsg, MSG_GUILD_ERROR_BATTLE_NOTBOSS);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 상대방이 길장이 아님
	if (charindex != tch->m_index)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildBattleErrMsg(rmsg, MSG_GUILD_ERROR_BATTLE_NOTBOSS);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 중단 신청 상태 셋팅
	ch->m_guildInfo->guild()->BattleState(GUILD_BATTLE_STATE_STOP_WAIT);
	guild->BattleState(GUILD_BATTLE_STATE_STOP_WAIT);

	{
		// 상대방에게 요청
		CNetMsg::SP rmsg(new CNetMsg);
		GuildBattleStopReqMsg(rmsg, ch->m_guildInfo->guild()->index(), ch->m_guildInfo->guild()->name());
		SEND_Q(rmsg, tch->m_desc);
	}
}

void do_GuildBattleStopReject(CPC* ch, CNetMsg::SP& msg)
{
	if (!ch->m_guildInfo)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildBattleErrMsg(rmsg, MSG_GUILD_ERROR_BATTLE_NOGUILD);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 길드전 중이 아님
	if (ch->m_guildInfo->guild()->battleState() != GUILD_BATTLE_STATE_STOP_WAIT)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildBattleErrMsg(rmsg, MSG_GUILD_ERROR_BATTLE_NOT_BATTLE);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 길드장이 아님
	if (ch->m_guildInfo->guild()->boss() && ch->m_guildInfo->guild()->boss()->charindex() != ch->m_index)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildBattleErrMsg(rmsg, MSG_GUILD_ERROR_BATTLE_NOTBOSS);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 상대방 길드
	CGuild* guild = gserver->m_guildlist.findguild(ch->m_guildInfo->guild()->battleIndex());

	if (!guild)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildBattleErrMsg(rmsg, MSG_GUILD_ERROR_BATTLE_NOGUILD);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	CPC* tch = guild->boss()->GetPC();

	// 상대방 길드장이 없음
	if (!tch)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildBattleErrMsg(rmsg, MSG_GUILD_ERROR_BATTLE_NOTBOSS);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 전투 상태 셋팅
	ch->m_guildInfo->guild()->BattleState(GUILD_BATTLE_STATE_ING);
	guild->BattleState(GUILD_BATTLE_STATE_ING);

	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildBattleStopRejectMsg(rmsg, ch->m_index);
		SEND_Q(rmsg, ch->m_desc);
		SEND_Q(rmsg, tch->m_desc);
	}
}

void do_GuildBattleStopAccept(CPC* ch, CNetMsg::SP& msg)
{
	if (!ch->m_guildInfo)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildBattleErrMsg(rmsg, MSG_GUILD_ERROR_BATTLE_NOGUILD);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 길드전 중이 아님
	if (ch->m_guildInfo->guild()->battleState() != GUILD_BATTLE_STATE_STOP_WAIT)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildBattleErrMsg(rmsg, MSG_GUILD_ERROR_BATTLE_NOT_BATTLE);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 길드장이 아님
	if (ch->m_guildInfo->guild()->boss() && ch->m_guildInfo->guild()->boss()->charindex() != ch->m_index)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildBattleErrMsg(rmsg, MSG_GUILD_ERROR_BATTLE_NOTBOSS);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 상대방 길드
	CGuild* guild = gserver->m_guildlist.findguild(ch->m_guildInfo->guild()->battleIndex());

	if (!guild)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildBattleErrMsg(rmsg, MSG_GUILD_ERROR_BATTLE_NOGUILD);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	CPC* tch = guild->boss()->GetPC();

	// 상대방 길드장이 없음
	if (!tch)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildBattleErrMsg(rmsg, MSG_GUILD_ERROR_BATTLE_NOTBOSS);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	if (gserver->isRunHelper())
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildBattleStopReqMsg(rmsg, ch->m_guildInfo->guild()->index(), guild->index());
		SEND_Q(rmsg, gserver->m_helper);
	}
	else
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_GAMESERVER);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
}

void do_GuildWarSetTimeReq(CPC* ch, CNetMsg::SP& msg)
{
	int wday;
	int hour;

	RefMsg(msg) >> wday
				>> hour;

	int zone;
	RefMsg(msg) >> zone;
	CWarCastle* castle = CWarCastle::GetCastleObject(zone);
	if (!castle)
		return ;

	CHECK_SUBSERVER(castle);

	// 길드가 있고
	if (!ch->m_guildInfo)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildWarSetTimeRepMsg(rmsg, MSG_GUILD_WAR_ERROR_NOT_OWNER, 0, 0, 0, 0);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 성주 길드이고
	CGuild* guild = ch->m_guildInfo->guild();
	if (guild->index() != castle->GetOwnerGuildIndex())
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildWarSetTimeRepMsg(rmsg, MSG_GUILD_WAR_ERROR_NOT_OWNER, 0, 0, 0, 0);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 길드 장이고
	if (guild->boss()->charindex() != ch->m_index)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildWarSetTimeRepMsg(rmsg, MSG_GUILD_WAR_ERROR_NOT_OWNER, 0, 0, 0, 0);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 공성중이 아니고
	if (castle->GetState() != WCSF_NORMAL)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildWarSetTimeRepMsg(rmsg, MSG_GUILD_WAR_ERROR_PROGRESS_WAR, 0, 0, 0, 0);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 신청시간이 아닐때
	if (castle->IsJoinTime())
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildWarSetTimeRepMsg(rmsg, MSG_GUILD_WAR_ERROR_NOT_SETTIME, 0, 0, 0, 0);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if(!CheckOverlapWarCastle(wday, hour, zone))
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildWarSetTimeRepMsg(rmsg, MSG_GUILD_WAR_ERROR_INVALID_TIME, 0, 0, 0, 0);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 시간 설정
	if (!castle->SetNextWarTime(wday, hour))
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildWarSetTimeRepMsg(rmsg, MSG_GUILD_WAR_ERROR_INVALID_TIME, 0, 0, 0, 0);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// DB 초기화
	CDBCmd cmd;
	cmd.Init(&gserver->m_dbcastle);

	std::string update_castle_query = boost::str(boost::format("UPDATE t_castle SET a_next_war_time = %d WHERE a_zone_index = %d") % castle->GetNextWarTime() % castle->GetZoneIndex());
	cmd.SetQuery(update_castle_query);
	cmd.Update();

	struct tm tmNext;
	castle->GetNextWarTime(&tmNext, true);

	GAMELOG << init("WAR CASTLE : SET WAR TIME", ch)
			<< tmNext.tm_year << delim
			<< tmNext.tm_mon << delim
			<< tmNext.tm_mday << delim
			<< tmNext.tm_hour << delim
			<< wday
			<< end;

	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildWarSetTimeRepMsg(rmsg, MSG_GUILD_WAR_ERROR_OK, tmNext.tm_mon, tmNext.tm_mday, tmNext.tm_hour, 0);
		SEND_Q(rmsg, ch->m_desc);
	}
}

void do_GuildWarGetTime(CPC* ch, CNetMsg::SP& msg)
{
	// 공성 시간 확인		: zoneindex(n) warerrorcode(n) guildindex(n:s) guildname(n:s) month(n:s) day(n:s) hour(n:s)
	// 해당 존이 공성 존인가?
	CWarCastle* castle = CWarCastle::GetCastleObject(ch->m_pZone->m_index);
	if (castle == NULL)
		return ;

	CHECK_SUBSERVER(castle);

	// 해당 존이 일반 상태인가?
	if (castle->GetState() != WCSF_NORMAL)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildWarGetTimeMsg(rmsg, MSG_GUILD_WAR_ERROR_PROGRESS_WAR, NULL);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildWarGetTimeMsg(rmsg, MSG_GUILD_WAR_ERROR_OK, castle);
		SEND_Q(rmsg, ch->m_desc);
	}
}

void do_GuildWarJoinAttackGuild(CPC* ch, CNetMsg::SP& msg)
{
#ifdef NPC_CHECK
	int npcIndex;
	CNPC *npc;
	RefMsg(msg) >> npcIndex;
	npc = ch->m_pArea->FindProtoNPCInCell(ch, npcIndex, false, 2);
	if(!npc)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildWarJoinAttackGuildMsg(rmsg, MSG_GUILD_WAR_ERROR_NOT_EXIST_NPC, NULL);
		SEND_Q(rmsg, ch->m_desc);
		GAMELOG << init("GuildWarJoinAttack FAIL: NOT EXIST NPC (Dangerous User)", ch) << end;
		return ;
	}
	if( !npc->IsFlag( NPC_WARCASTLE ) )
	{
	}
#endif

	CWarCastle* castle = CWarCastle::GetCastleObject(ch->m_pZone->m_index);
	if (castle == NULL)
		return ;

	CHECK_SUBSERVER(castle);

	// 길드가 있고
	if (ch->m_guildInfo == NULL)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildWarJoinAttackGuildMsg(rmsg, MSG_GUILD_WAR_ERROR_JOIN_NOT_GUILD, NULL);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 보스 이고
	if (ch->m_guildInfo->pos() != MSG_GUILD_POSITION_BOSS)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildWarJoinAttackGuildMsg(rmsg, MSG_GUILD_WAR_ERROR_JOIN_NOT_GUILD_BOSS, NULL);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	CGuild* guild = ch->m_guildInfo->guild();

	// 성주 길드가 아니고
	if (guild->index() == castle->GetOwnerGuildIndex())
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildWarJoinAttackGuildMsg(rmsg, MSG_GUILD_WAR_ERROR_JOIN_OWNER_GUILD, NULL);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 기존 신청 검사하고
	if (castle->IsAttackGuild(guild->index()))
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildWarJoinAttackGuildMsg(rmsg, MSG_GUILD_WAR_ERROR_JOIN_ALREADY, NULL);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 드라탄과 메라크 동시 신청 불가
	switch(ch->m_pZone->m_index)
	{
	case ZONE_DRATAN:
		{
			CWarCastle * pCastle = CWarCastle::GetCastleObject(ZONE_MERAC);
			if(pCastle != NULL
					&& pCastle->IsAttackGuild(guild->index()))
			{
				// 메라크 공성 신청한 길드가 드라탄 공성 신청할 경우
				CNetMsg::SP rmsg(new CNetMsg);
				GuildWarJoinAttackGuildMsg(rmsg, MSG_GUILD_WAR_ERROR_JOIN_ALREADY, NULL);
				SEND_Q(rmsg, ch->m_desc);
				return ;
			}

			if(pCastle != NULL
					&& guild->index() == pCastle->GetOwnerGuildIndex())
			{
				// 메라크 성주가 드라탄 공성 신청할 경우
				CNetMsg::SP rmsg(new CNetMsg);
				GuildWarJoinAttackGuildMsg(rmsg, MSG_GUILD_WAR_ERROR_JOIN_OWNER_GUILD, NULL);
				SEND_Q(rmsg, ch->m_desc);
				return ;
			}
		}
		break;

	case ZONE_MERAC:
		{
			CWarCastle * pCastle = CWarCastle::GetCastleObject(ZONE_DRATAN);
			if(pCastle != NULL
					&& pCastle->IsAttackGuild(guild->index()))
			{
				// 드라탄 공성 신청한 길드가 메라크 공성 신청할 경우
				CNetMsg::SP rmsg(new CNetMsg);
				GuildWarJoinAttackGuildMsg(rmsg, MSG_GUILD_WAR_ERROR_JOIN_ALREADY, NULL);
				SEND_Q(rmsg, ch->m_desc);
				return ;
			}

			if (pCastle != NULL
					&& guild->index() == pCastle->GetOwnerGuildIndex())
			{
				// 드라탄 성주가 메라크 공성 신청할 경우
				CNetMsg::SP rmsg(new CNetMsg);
				GuildWarJoinAttackGuildMsg(rmsg, MSG_GUILD_WAR_ERROR_JOIN_OWNER_GUILD, NULL);
				SEND_Q(rmsg, ch->m_desc);
				return ;
			}
		}
		break;
	}

	// 조건 검사하고 : 신청 가능 시간, 길드 레벨, 길드원 수, 아이템, 나스
	if (!castle->IsJoinTime())
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildWarJoinAttackGuildMsg(rmsg, MSG_GUILD_WAR_ERROR_JOIN_TIME, NULL);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if (ch->m_pZone->m_index == ZONE_MERAC && guild->level() < WCGF_ATTACK_LEVEL)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildWarJoinAttackGuildMsg(rmsg, MSG_GUILD_WAR_ERROR_JOIN_GUILDLEVEL, NULL);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if (ch->m_pZone->m_index == ZONE_MERAC && guild->membercount() < WCGF_MEMBERCOUNT)
	{
		// 메라크 공성만 20명 제한
		CNetMsg::SP rmsg(new CNetMsg);
		GuildWarJoinAttackGuildMsg(rmsg, MSG_GUILD_WAR_ERROR_JOIN_GUILDMEMBER, NULL);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if (ch->m_inventory.getMoney() < WCGF_MONEY)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildWarJoinAttackGuildMsg(rmsg, MSG_GUILD_WAR_ERROR_JOIN_MONEY, NULL);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// DB에 저장
	// 1. 트랜잭션 시작
	// 2. 현재 캐릭터의 용병 신청 삭제
	// 3. 현재 캐릭터 길드의 신청 삭제
	// 4. 새로 추가
	// 5. 트랜잭션 끝
	int i;
	CDBCmd cmd;
	int guildMaxmemberCount;
	cmd.Init(&gserver->m_dbcastle);
	cmd.BeginTrans();
	try
	{
		// 개인 용병 신청 삭제
		if (ch->GetJoinFlag(ch->m_pZone->m_index) != WCJF_NONE)
		{
			std::string delete_castle_join_query = "";
			switch (ch->GetJoinFlag(ch->m_pZone->m_index))
			{
			case WCJF_DEFENSE_GUILD:
				{
					// 길원 모두 삭제
					std::string tmpString = "";
					guildMaxmemberCount = guild->maxmember();
					for (i = 0; i < guildMaxmemberCount; i++)
					{
						if (guild->member(i))
						{
							if (tmpString.length() == 0)
							{
								tmpString += boost::str(boost::format("%d") % guild->member(i)->charindex());
							}
							else
							{
								tmpString += boost::str(boost::format(",%d") % guild->member(i)->charindex());
							}
						}
					}
					delete_castle_join_query = boost::str(boost::format("DELETE FROM t_castle_join WHERE a_zone_index=%d AND a_index in (%s) AND a_guild=0") % ch->m_pZone->m_index % tmpString.c_str());
				}
				break;

			case WCJF_DEFENSE_CHAR:
			case WCJF_ATTACK_CHAR:
				delete_castle_join_query = boost::str(boost::format("DELETE FROM t_castle_join WHERE a_zone_index=%d AND a_index=%d AND a_guild=0") % ch->m_pZone->m_index % ch->m_index);
				break;
			default:
				LOG_ERROR("not switched. ch->GetJoinFlag(ch->m_pZone->m_index) = %d", ch->GetJoinFlag(ch->m_pZone->m_index));
				throw 0;
				break;
			}
			cmd.SetQuery(delete_castle_join_query);
			if (!cmd.Update())
				throw 0;

			// 길드 신청 삭제
			std::string delete_castle_join_query1 = boost::str(boost::format("DELETE FROM t_castle_join WHERE a_zone_index=%d AND a_index=%d AND a_guild=1") % ch->m_pZone->m_index % guild->index());
			cmd.SetQuery(delete_castle_join_query1);
			if (!cmd.Update())
				throw 0;
		}

		std::string insert_castle_join_query = boost::str(boost::format("INSERT INTO t_castle_join (a_zone_index, a_index, a_guild, a_attack) VALUES (%d, %d, 1, 1)") % ch->m_pZone->m_index % guild->index());
		cmd.SetQuery(insert_castle_join_query);
		if (!cmd.Update())
			throw 0;

		cmd.Commit();
	}
	catch (...)
	{
		cmd.Rollback();
		CNetMsg::SP rmsg(new CNetMsg);
		GuildWarJoinAttackGuildMsg(rmsg, MSG_GUILD_WAR_ERROR_JOIN_ALREADY, NULL);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	GAMELOG << init("WAR CASTLE : JOIN ATTACK GUILD", ch)
			<< guild->index() << delim
			<< guild->name()
			<< end;

	{
		// Helper로 알리기
		// Helper 없으면 그냥 현재 서브 채널만
		CNetMsg::SP rmsg(new CNetMsg);
		HelperWarJoinAttackGuildMsg(rmsg, ch->m_pZone->m_index, guild->index());
		if (gserver->isRunHelper())
		{
			SEND_Q(rmsg, gserver->m_helper);
		}
		else
		{
			gserver->ProcessHelperMessage(rmsg);
		}
	}

	{
		// 결과 알림
		CNetMsg::SP rmsg(new CNetMsg);
		GuildWarJoinAttackGuildMsg(rmsg, MSG_GUILD_WAR_ERROR_OK, castle);
		SEND_Q(rmsg, ch->m_desc);
	}
}

void do_GuildWarJoinDefenseGuild(CPC* ch, CNetMsg::SP& msg)
{
	CWarCastle* castle = CWarCastle::GetCastleObject(ch->m_pZone->m_index);
	if (castle == NULL)
		return ;

	CHECK_SUBSERVER(castle);

	// 성주가 있는 성이고
	if (castle->GetOwnerGuildIndex() <= 0)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildWarJoinDefenseGuildMsg(rmsg, MSG_GUILD_WAR_ERROR_JOIN_NO_OWNER, NULL);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 길드가 있고
	if (ch->m_guildInfo == NULL)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildWarJoinDefenseGuildMsg(rmsg, MSG_GUILD_WAR_ERROR_JOIN_NOT_GUILD, NULL);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 보스 이고
	if (ch->m_guildInfo->pos() != MSG_GUILD_POSITION_BOSS)
		return ;

	CGuild* guild = ch->m_guildInfo->guild();

	// 성주 길드가 아니고
	if (guild->index() == castle->GetOwnerGuildIndex())
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildWarJoinDefenseGuildMsg(rmsg, MSG_GUILD_WAR_ERROR_JOIN_OWNER_GUILD, NULL);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 기존 신청 검사하고
	if (castle->IsAttackGuild(guild->index()) || castle->IsDefenseGuild(guild->index()))
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildWarJoinDefenseGuildMsg(rmsg, MSG_GUILD_WAR_ERROR_JOIN_ALREADY, NULL);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 조건 검사하고 : 신청 가능 시간, 길드 레벨
	if (!castle->IsJoinTime())
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildWarJoinDefenseGuildMsg(rmsg, MSG_GUILD_WAR_ERROR_JOIN_TIME, NULL);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

#ifdef MEREC_CASTLE_TEST_GUILD_LEVEL_GM_SETTING
	int guildLevel = WCGF_DEFENSE_LEVEL;

	// 메라크 존일때만 길드 레벨을 GM이 설정한 것으로 검사한다.
	if( castle->GetCurSubServerCastleZoneIndex() == ZONE_MERAC &&  gserver->MeracCastleDebugGuildLevel > 0 )
		guildLevel = gserver->MeracCastleDebugGuildLevel;

	if (guild->level() < guildLevel )
#else
	if (guild->level() < WCGF_DEFENSE_LEVEL )
#endif
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildWarJoinDefenseGuildMsg(rmsg, MSG_GUILD_WAR_ERROR_JOIN_GUILDLEVEL, NULL);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// DB에 저장
	// 1. 트랜잭션 시작
	// 2. 현재 캐릭터의 용병 신청 삭제
	// 3. 현재 캐릭터 길드의 신청 삭제
	// 4. 새로 추가
	// 5. 트랜잭션 끝
	CDBCmd cmd;
	cmd.Init(&gserver->m_dbcastle);
	cmd.BeginTrans();
	try
	{
		// 개인 용병 신청 삭제
		if (ch->GetJoinFlag(ch->m_pZone->m_index) != WCJF_NONE)
		{
			std::string delete_castle_join_query = "";
			switch (ch->GetJoinFlag(ch->m_pZone->m_index))
			{
			case WCJF_DEFENSE_CHAR:
			case WCJF_ATTACK_CHAR:
				delete_castle_join_query = boost::str(boost::format("DELETE FROM t_castle_join WHERE a_zone_index=%d AND a_index=%d AND a_guild=0") % ch->m_pZone->m_index % ch->m_index);
				break;
			default:
				LOG_ERROR("not switched. ch->GetJoinFlag(ch->m_pZone->m_index) = %d", ch->GetJoinFlag(ch->m_pZone->m_index));
				throw 0;
				break;
			}
			cmd.SetQuery(delete_castle_join_query);
			if (!cmd.Update())
				throw 0;
		}

		std::string insert_castle_join_query = boost::str(boost::format("INSERT INTO t_castle_join (a_zone_index, a_index, a_guild, a_attack) VALUES (%d, %d, 1, 0)") % ch->m_pZone->m_index % guild->index());
		cmd.SetQuery(insert_castle_join_query);
		if (!cmd.Update())
			throw 0;

		cmd.Commit();
	}
	catch (...)
	{
		cmd.Rollback();
		CNetMsg::SP rmsg(new CNetMsg);
		GuildWarJoinDefenseGuildMsg(rmsg, MSG_GUILD_WAR_ERROR_JOIN_ALREADY, NULL);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	GAMELOG << init("WAR CASTLE : JOIN DEFENSE GUILD", ch)
			<< guild->index() << delim
			<< guild->name()
			<< end;

	{
		// Helper로 알리기
		// Helper 없으면 그냥 현재 서브 채널만
		CNetMsg::SP rmsg(new CNetMsg);
		HelperWarJoinDefenseGuildMsg(rmsg, ch->m_pZone->m_index, guild->index());
		if (gserver->isRunHelper())
		{
			SEND_Q(rmsg, gserver->m_helper);
		}
		else
		{
			gserver->ProcessHelperMessage(rmsg);
		}
	}

	{
		// 결과 알림
		CNetMsg::SP rmsg(new CNetMsg);
		GuildWarJoinDefenseGuildMsg(rmsg, MSG_GUILD_WAR_ERROR_OK, castle);
		SEND_Q(rmsg, ch->m_desc);
	}
}

void do_GuildWarJoinAttackChar(CPC* ch, CNetMsg::SP& msg)
{
#ifdef NPC_CHECK
	int npcIndex;
	CNPC *npc;
	RefMsg(msg) >> npcIndex;
	npc = ch->m_pArea->FindProtoNPCInCell(ch, npcIndex, false, 2);
	if(!npc)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildWarJoinAttackCharMsg(rmsg, MSG_GUILD_WAR_ERROR_NOT_EXIST_NPC, NULL);
		SEND_Q(rmsg, ch->m_desc);
		GAMELOG << init("GuildWarJoinAttack FAIL: NOT EXIST NPC (Dangerous User)", ch) << end;
		return ;
	}
	if( !npc->IsFlag(NPC_WARCASTLE) )
	{
		GAMELOG << init("GuildWarJoinAttack FAIL: NOT EXIST NPC (Dangerous User)", ch) << end;
		return ;
	}
#endif

	CWarCastle* castle = CWarCastle::GetCastleObject(ch->m_pZone->m_index);
	if (castle == NULL)
		return ;

	CHECK_SUBSERVER(castle);

	// 길드 방침을 먼저 따름
	switch (ch->GetJoinFlag(ch->m_pZone->m_index))
	{
	case WCJF_OWNER:
		{
			CNetMsg::SP rmsg(new CNetMsg);
			GuildWarJoinAttackCharMsg(rmsg, MSG_GUILD_WAR_ERROR_JOIN_OWNER_GUILD, NULL);
			SEND_Q(rmsg, ch->m_desc);
		}
		return ;

	case WCJF_DEFENSE_GUILD:
		{
			CNetMsg::SP rmsg(new CNetMsg);
			GuildWarJoinAttackCharMsg(rmsg, MSG_GUILD_WAR_ERROR_JOIN_ALREADY_GUILD, NULL);
			SEND_Q(rmsg, ch->m_desc);
		}
		return ;

	case WCJF_ATTACK_GUILD:
		{
			CNetMsg::SP rmsg(new CNetMsg);
			GuildWarJoinAttackCharMsg(rmsg, MSG_GUILD_WAR_ERROR_JOIN_ALREADY_GUILD, NULL);
			SEND_Q(rmsg, ch->m_desc);
		}
		return ;

	case WCJF_ATTACK_CHAR:
		{
			CNetMsg::SP rmsg(new CNetMsg);
			GuildWarJoinAttackCharMsg(rmsg, MSG_GUILD_WAR_ERROR_JOIN_ALREADY, NULL);
			SEND_Q(rmsg, ch->m_desc);
		}
		return ;
	}

	// 조건 검사하고 : 신청 가능 시간, 레벨
	if (!castle->IsJoinTime())
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildWarJoinAttackCharMsg(rmsg, MSG_GUILD_WAR_ERROR_JOIN_TIME, NULL);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if (ch->m_level < WCGF_CHAR_LEVEL)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildWarJoinAttackCharMsg(rmsg, MSG_GUILD_WAR_ERROR_JOIN_CHARLEVEL, NULL);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// DB에 저장
	// 1. 트랜잭션 시작
	// 2. 현재 캐릭터의 용병 신청 삭제
	// 3. 현재 캐릭터 길드의 신청 삭제
	// 4. 새로 추가
	// 5. 트랜잭션 끝
	CDBCmd cmd;
	cmd.Init(&gserver->m_dbcastle);
	cmd.BeginTrans();
	try
	{
		// 개인 용병 신청 삭제
		if (ch->GetJoinFlag(ch->m_pZone->m_index) != WCJF_NONE)
		{
			std::string delete_castle_join_query = "";
			switch (ch->GetJoinFlag(ch->m_pZone->m_index))
			{
			case WCJF_DEFENSE_CHAR:
			case WCJF_ATTACK_CHAR:
				delete_castle_join_query = boost::str(boost::format("DELETE FROM t_castle_join WHERE a_zone_index=%d AND a_index=%d AND a_guild=0") % ch->m_pZone->m_index % ch->m_index);
				break;
			default:
				LOG_ERROR("not switched. ch->GetJoinFlag(ch->m_pZone->m_index) = %d", ch->GetJoinFlag(ch->m_pZone->m_index));
				throw 0;
				break;
			}
			cmd.SetQuery(delete_castle_join_query);
			if (!cmd.Update())
				throw 0;
		}

		std::string insert_castle_join_query = boost::str(boost::format("INSERT INTO t_castle_join (a_zone_index, a_index, a_guild, a_attack) VALUES (%d, %d, 0, 1)") % ch->m_pZone->m_index % ch->m_index);
		cmd.SetQuery(insert_castle_join_query);
		if (!cmd.Update())
			throw 0;

		cmd.Commit();
	}
	catch (...)
	{
		cmd.Rollback();
		CNetMsg::SP rmsg(new CNetMsg);
		GuildWarJoinAttackCharMsg(rmsg, MSG_GUILD_WAR_ERROR_JOIN_ALREADY, NULL);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	GAMELOG << init("WAR CASTLE : JOIN ATTACK CHAR", ch)
			<< end;

	// 정보 변경
	// 성 정보
	castle->RemoveAttackChar(ch->m_index);
	castle->RemoveDefenseChar(ch->m_index);
	castle->AddAttackChar(ch->m_index);

	// 캐릭정보
	ch->SetJoinFlag(ch->m_pZone->m_index, WCJF_ATTACK_CHAR);
	ch->m_bChangeStatus = true;

	{
		// 결과 알림
		CNetMsg::SP rmsg(new CNetMsg);
		GuildWarJoinAttackCharMsg(rmsg, MSG_GUILD_WAR_ERROR_OK, castle);
		SEND_Q(rmsg, ch->m_desc);
	}
}

void do_GuildWarJoinDefenseChar(CPC* ch, CNetMsg::SP& msg)
{
#ifdef NPC_CHECK
	int npcIndex;
	CNPC *npc;
	RefMsg(msg) >> npcIndex;
	npc = ch->m_pArea->FindProtoNPCInCell(ch, npcIndex, false, 2);
	if(!npc)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildWarJoinDefenseGuildMsg(rmsg, MSG_GUILD_WAR_ERROR_NOT_EXIST_NPC, NULL);
		SEND_Q(rmsg, ch->m_desc);
		GAMELOG << init("GuildWarJoinDefense FAIL: NOT EXIST NPC (Dangerous User)", ch) << end;
		return ;
	}
	if( !npc->IsFlag(NPC_WARCASTLE) )
	{
		GAMELOG << init("GuildWarJoinDefense FAIL: NOT EXIST NPC (Dangerous User)", ch) << end;
		return ;
	}
#endif

	CWarCastle* castle = CWarCastle::GetCastleObject(ch->m_pZone->m_index);
	if (castle == NULL)
		return ;

	CHECK_SUBSERVER(castle);

	// 성주가 있는 성이고
	if (castle->GetOwnerGuildIndex() <= 0)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildWarJoinDefenseGuildMsg(rmsg, MSG_GUILD_WAR_ERROR_JOIN_NO_OWNER, NULL);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 길드 방침을 먼저 따름
	switch (ch->GetJoinFlag(ch->m_pZone->m_index))
	{
	case WCJF_OWNER:
		{
			CNetMsg::SP rmsg(new CNetMsg);
			GuildWarJoinDefenseCharMsg(rmsg, MSG_GUILD_WAR_ERROR_JOIN_OWNER_GUILD, NULL);
			SEND_Q(rmsg, ch->m_desc);
		}
		return ;

	case WCJF_DEFENSE_GUILD:
		{
			CNetMsg::SP rmsg(new CNetMsg);
			GuildWarJoinDefenseCharMsg(rmsg, MSG_GUILD_WAR_ERROR_JOIN_ALREADY_GUILD, NULL);
			SEND_Q(rmsg, ch->m_desc);
		}
		return ;

	case WCJF_ATTACK_GUILD:
		{
			CNetMsg::SP rmsg(new CNetMsg);
			GuildWarJoinDefenseCharMsg(rmsg, MSG_GUILD_WAR_ERROR_JOIN_ALREADY_GUILD, NULL);
			SEND_Q(rmsg, ch->m_desc);
		}
		return ;
	}

	// 조건 검사하고 : 신청 가능 시간, 레벨
	if (!castle->IsJoinTime())
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildWarJoinDefenseCharMsg(rmsg, MSG_GUILD_WAR_ERROR_JOIN_TIME, NULL);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if (ch->m_level < WCGF_CHAR_LEVEL)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildWarJoinDefenseCharMsg(rmsg, MSG_GUILD_WAR_ERROR_JOIN_CHARLEVEL, NULL);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// DB에 저장
	// 1. 트랜잭션 시작
	// 2. 현재 캐릭터의 용병 신청 삭제
	// 3. 현재 캐릭터 길드의 신청 삭제
	// 4. 새로 추가
	// 5. 트랜잭션 끝
	CDBCmd cmd;
	cmd.Init(&gserver->m_dbcastle);
	cmd.BeginTrans();
	try
	{
		// 개인 용병 신청 삭제
		if (ch->GetJoinFlag(ch->m_pZone->m_index) != WCJF_NONE)
		{
			std::string delete_castle_join_query = "";
			switch (ch->GetJoinFlag(ch->m_pZone->m_index))
			{
			case WCJF_DEFENSE_CHAR:
			case WCJF_ATTACK_CHAR:
				delete_castle_join_query = boost::str(boost::format("DELETE FROM t_castle_join WHERE a_zone_index=%d AND a_index=%d AND a_guild=0") % ch->m_pZone->m_index % ch->m_index);
				break;
			default:
				LOG_ERROR("not switched. ch->GetJoinFlag(ch->m_pZone->m_index) = %d", ch->GetJoinFlag(ch->m_pZone->m_index));
				throw 0;
				break;
			}
			cmd.SetQuery(delete_castle_join_query);
			if (!cmd.Update())
				throw 0;
		}

		std::string insert_castle_join_query = boost::str(boost::format("INSERT INTO t_castle_join (a_zone_index, a_index, a_guild, a_attack) VALUES (%d, %d, 0, 0)") % ch->m_pZone->m_index % ch->m_index);
		cmd.SetQuery(insert_castle_join_query);
		if (!cmd.Update())
			throw 0;

		cmd.Commit();
	}
	catch (...)
	{
		cmd.Rollback();
		CNetMsg::SP rmsg(new CNetMsg);
		GuildWarJoinDefenseCharMsg(rmsg, MSG_GUILD_WAR_ERROR_JOIN_ALREADY, NULL);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	GAMELOG << init("WAR CASTLE : JOIN DEFENSE CHAR", ch)
			<< end;

	// 정보 변경
	// 성 정보
	castle->RemoveAttackChar(ch->m_index);
	castle->RemoveDefenseChar(ch->m_index);
	castle->AddDefenseChar(ch->m_index);

	// 캐릭정보
	ch->SetJoinFlag(ch->m_pZone->m_index, WCJF_DEFENSE_CHAR);
	ch->m_bChangeStatus = true;

	{
		// 결과 알림
		CNetMsg::SP rmsg(new CNetMsg);
		GuildWarJoinDefenseCharMsg(rmsg, MSG_GUILD_WAR_ERROR_OK, castle);
		SEND_Q(rmsg, ch->m_desc);
	}
}

void do_GuildWarAttackSymbol(CPC* ch, CNetMsg::SP& msg)
{
	CWarCastle* castle = CWarCastle::GetCastleObject(ch->m_pZone->m_index);
	if (castle == NULL)
		return ;

	// 성문중 하나라고 열린곳이 있을때
	if (!castle->IsOpenGate())
		return ;

	// 권좌가 아직 안 부셔졌을 때
	if (DEAD(castle->GetLordSymbol()))
		return ;

	// 길드가 있고
	if (ch->m_guildInfo == NULL)
		return ;
	CGuild* guild = ch->m_guildInfo->guild();

	// 공성탑3 길드중 하나이고
	if (!castle->IsTop3Guild(guild->index()))
		return ;

	int point=0;
	// 길드장이고
	if (ch->m_guildInfo->pos() == MSG_GUILD_POSITION_BOSS )
		point = 3;
	else if(ch->m_guildInfo->pos() == MSG_GUILD_POSITION_OFFICER)
		point = 1;
	else
		return ;

	// 거리가 3미터 이내이고
	if (GetDistance(ch, castle->GetLordSymbol()) > 3.0f)
		return ;

	// 1.2초 마다 한번
	if (gserver->m_pulse - ch->m_lastAttackPulse + PULSE_HACK_ATTACK_THRESHOLD < 12)
		return ;

	ch->CancelInvisible();

	ch->m_lastAttackPulse = gserver->m_pulse;
	ch->m_pulseLastAttackSkill = gserver->m_pulse;

	{
		// 공격 메시지 에코
		CNetMsg::SP rmsg(new CNetMsg);
		DamageMsg(rmsg, ch, castle->GetLordSymbol(), MSG_DAMAGE_MELEE, -1, 1, HITTYPE_NORMAL, -1);
		ch->m_pArea->SendToCell(rmsg, ch, true);
	}

	// 권좌 HP 감소
	castle->DecreaseLordSymbolHP();

	{
		// 권좌 HP 감소 알림
		CNetMsg::SP rmsg(new CNetMsg);
		CharStatusMsg(rmsg, castle->GetLordSymbol(), 0);
		ch->m_pArea->SendToCell(rmsg, ch, true);
	}

	castle->IncreaseTeamPoint(guild->index(), point);

	// 포인트 증가 알림
	PCManager::instance()->sendGuildWarPointMsg(castle, ch->m_pZone->m_index);
}

void do_GuildStashHistoryReq(CPC* ch, CNetMsg::SP& msg)
{
	int month[7] = {0, 0, 0, 0, 0, 0, 0};
	int day[7] = {0, 0, 0, 0, 0, 0, 0};
	LONGLONG money[7] = {0, 0, 0, 0, 0, 0, 0};

	// 길드가 있는 사람만
	if (!ch->m_guildInfo || !ch->m_guildInfo->guild())
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildStashHistoryRepMsg(rmsg, MSG_GUILD_STASH_ERROR_NOTBOSS, month, day, money);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 헬퍼에 요청
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildStashHistoryReqMsg(rmsg, ch->m_guildInfo->guild()->index(), ch->m_index);
		SEND_Q(rmsg, gserver->m_helper);
	}
}

void do_GuildStashViewReq(CPC* ch, CNetMsg::SP& msg)
{
	// 길드가 있는 사람만
	if (!ch->m_guildInfo || !ch->m_guildInfo->guild())
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildStashViewRepMsg(rmsg, MSG_GUILD_STASH_ERROR_NOTBOSS, 0);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 길드장이나 부관만
	if (ch->m_guildInfo->pos() != MSG_GUILD_POSITION_BOSS && ch->m_guildInfo->pos() != MSG_GUILD_POSITION_OFFICER)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildStashViewRepMsg(rmsg, MSG_GUILD_STASH_ERROR_NOTBOSS, 0);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 헬퍼에 요청
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildStashViewReqMsg(rmsg, ch->m_guildInfo->guild()->index(), ch->m_index);
		SEND_Q(rmsg, gserver->m_helper);
	}
}

void do_GuildStashTakeReq(CPC* ch, CNetMsg::SP& msg)
{
	LONGLONG money = 0;
	RefMsg(msg) >> money;

	if (money < 1)
		return ;

	// 길드가 있는 사람만
	if (!ch->m_guildInfo || !ch->m_guildInfo->guild())
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildStashTakeRepMsg(rmsg, MSG_GUILD_STASH_ERROR_NOTBOSS);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 길드장이나 부관만
	if (ch->m_guildInfo->pos() != MSG_GUILD_POSITION_BOSS && ch->m_guildInfo->pos() != MSG_GUILD_POSITION_OFFICER)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildStashTakeRepMsg(rmsg, MSG_GUILD_STASH_ERROR_NOTBOSS);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 헬퍼에 요청
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildStashTakeReqMsg(rmsg, ch->m_guildInfo->guild()->index(), ch->m_index, money);
		SEND_Q(rmsg, gserver->m_helper);

		GAMELOG << init("GUILD STASH TAKE REQUEST", ch)
				<< "MONEY" << delim
				<< money
				<< end;
	}
}

void do_GuildInclineEstablist( CPC* ch, CNetMsg::SP& msg )
{
	char guildincline;

	RefMsg(msg) >> guildincline;

	// 조건 검사
	// 길드가 있는 사람
	if( !ch->m_guildInfo || !ch->m_guildInfo->guild() )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg( rmsg, MSG_GUILD_ERROR_NOGUILD );
		SEND_Q( rmsg, ch->m_desc );
		return;
	}
	// 길드장만
	if (ch->m_guildInfo->pos() != MSG_GUILD_POSITION_BOSS )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_NOTBOSS);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 길드 레벨 검사
	if( ch->m_guildInfo->guild()->level() < 6 )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg(rmsg, MSG_NEW_GUILD_ERROR_CHANG_INCLINE);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	//현재 길드 성향
//#define GUILD_INCLINE_GENERAL		0		// 일반 길드
//#define GUILD_INCLINE_CHAO			1		// 카오 길드
//#define GUILD_INCLINE_HUNTER		2		// 헌터 길드
	if( ch->m_guildInfo->guild()->GetIncline() == guildincline || guildincline == GUILD_INCLINE_GENERAL )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg(rmsg, MSG_NEW_GUILD_ERROR_SAME_INCLNE );
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
	LONGLONG needMoney = 0;
	int needGP = 0;

	if( guildincline == GUILD_INCLINE_CHAO )
	{
		// 나스 검사
		if (ch->m_inventory.getMoney() < 2000000)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			GuildErrorMsg(rmsg, MSG_NEW_GUILD_ERROR_CHANG_INCLINE);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}

		// gp 검사
		if( ch->m_guildInfo->guild()->GetGuildPoint() < 10000 )
		{
			CNetMsg::SP rmsg(new CNetMsg);
			GuildErrorMsg(rmsg, MSG_NEW_GUILD_ERROR_CHANG_INCLINE);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}
		needMoney = 2000000;
		needGP	= 10000;
	}
	else if ( guildincline == GUILD_INCLINE_HUNTER )
	{
		// 나스 검사
		if (ch->m_inventory.getMoney() < 5000000 )
		{
			CNetMsg::SP rmsg(new CNetMsg);
			GuildErrorMsg(rmsg, MSG_NEW_GUILD_ERROR_CHANG_INCLINE);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}

		// gp 검사
		if( ch->m_guildInfo->guild()->GetGuildPoint() < 5000 )
		{
			CNetMsg::SP rmsg(new CNetMsg);
			GuildErrorMsg(rmsg, MSG_NEW_GUILD_ERROR_CHANG_INCLINE);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}

		needMoney = 5000000;
		needGP	= 5000;
	}

	if (gserver->isRunHelper())
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildInclineEstablishReqMsg( rmsg, ch, guildincline );
		RefMsg(rmsg) << needMoney << needGP;

		SEND_Q(rmsg, gserver->m_helper);
	}
	else
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_GAMESERVER);
		SEND_Q(rmsg, ch->m_desc);
	}
}
void do_GuildMemberAdjust( CPC* ch, CNetMsg::SP& msg )
{
	int			charindex;
	CLCString	strPositionName(GUILD_POSITION_NAME+1);
	unsigned int			contributeExp;
	unsigned int			contributFame;
	int			pos;
#ifdef DEV_GUILD_STASH
	char		cStashAuth;			// 0 :사용불가, 1:사용가능
#endif // DEV_GUILD_STASH

	RefMsg(msg) >> charindex
				>> strPositionName
				>> contributeExp
				>> contributFame;
	RefMsg(msg) >> pos;

#ifdef DEV_GUILD_STASH
	RefMsg(msg) >> cStashAuth;
#endif //DEV_GUILD_STASH

	if((contributeExp < 0 || contributeExp > 100) ||
			(contributFame < 0 || contributFame > 100))
	{
		return ;
	}

	// 길드가 있는 사람
	if( !ch->m_guildInfo || !ch->m_guildInfo->guild() )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg( rmsg, MSG_GUILD_ERROR_NOGUILD );
		SEND_Q( rmsg, ch->m_desc );
		return;
	}

	//길드장인지 체크
	if( ch->m_guildInfo->pos() != MSG_GUILD_POSITION_BOSS )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_NOTBOSS);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

#ifdef GUILD_CONTRIBUTE_EXP_LIMIT // 길두 기부 경험치를 max 50%까지만 가능하도록 수정
	if( contributeExp > GUILD_CONTRIBUTE_EXP_LIMIT )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg(rmsg, MSG_NEW_GUILD_ERROR_ADJUST_FAIL);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}
#endif

	if(findPercentChar(strPositionName.getBuffer()) != NULL)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg(rmsg, MSG_NEW_GUILD_ERROR_ADJUST_FAIL);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 직위값이 유효하지 않을 때
	if( pos < 0 || pos > MSG_GUILD_POSITION_RECON_MEMBER )
	{
		GAMELOG << init("GUILD MEMBER POSITION UPDATE FAIL : POSITION VALUE INVALID")	<< delim
				<< "GUILD INDEX" << delim
				<< ch->m_guildInfo->guild()->index() << delim
				<< "GUILD NAME"	<< delim
				<< ch->m_guildInfo->GetName() << delim
				<< "POSITION" << delim
				<< pos
				<< end;
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg(rmsg, MSG_NEW_GUILD_ERROR_ADJUST_FAIL);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
	// 메라크 공성 중이고 참여 길드이면
	CWarCastle* castle = CWarCastle::GetCastleObject(ZONE_MERAC);
	if( castle != NULL && castle->GetState() != WCSF_NORMAL
			&& ( IS_ATTACK_TEAM(ch->GetJoinFlag(ZONE_MERAC)) || IS_DEFENSE_TEAM(ch->GetJoinFlag(ZONE_MERAC)) ) )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg(rmsg, MSG_GUILD_GRADE_EX_ERROR_WARCASTLE);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 드라탄 공성 중이고 참여 길드이면
	castle = CWarCastle::GetCastleObject(ZONE_DRATAN);
	if( castle != NULL && castle->GetState() != WCSF_NORMAL
			&& ( IS_ATTACK_TEAM(ch->GetJoinFlag(ZONE_DRATAN)) || IS_DEFENSE_TEAM(ch->GetJoinFlag(ZONE_DRATAN)) ) )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg(rmsg, MSG_GUILD_GRADE_EX_ERROR_WARCASTLE);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if( ch->m_guildInfo->guild()->GetGradeExPosNeedGuilPoint( pos ) > ch->m_guildInfo->guild()->GetGuildPoint() )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg(rmsg, MSG_NEW_GUILD_ERRROR_GUILDPOINT);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	if( !ch->m_guildInfo->guild()->CheckGradeExPosCount( pos, charindex ) )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg(rmsg, MSG_GUILD_GRADE_EX_ERROR_COUNT_OEVER);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if (gserver->isRunHelper())
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildMemberAdjust( rmsg, ch, charindex, strPositionName, contributeExp, contributFame, pos );
#ifdef DEV_GUILD_STASH
		RefMsg(rmsg) << cStashAuth;
#endif // DEV_GUILD_STASH

		SEND_Q(rmsg, gserver->m_helper);
	}
	else
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_GAMESERVER);
		SEND_Q(rmsg, ch->m_desc);
	}
}

void do_NewGuildInfo( CPC* ch, CNetMsg::SP& msg )
{
	// 길드가 있는 사람인지 체크
	if( !ch->m_guildInfo || !ch->m_guildInfo->guild() )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg( rmsg, MSG_GUILD_ERROR_NOGUILD );
		SEND_Q( rmsg, ch->m_desc );
		return;
	}

	if( gserver->isRunHelper() )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperNewGuildInfo( rmsg, ch );
		SEND_Q( rmsg, gserver->m_helper );
	}
	else
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_GAMESERVER);
		SEND_Q(rmsg, ch->m_desc);
	}
}
void do_NewGuildMemberList( CPC* ch, CNetMsg::SP& msg )
{
	// 길드가 있는 사람인지 체크
	if( !ch->m_guildInfo || !ch->m_guildInfo->guild() )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg( rmsg, MSG_GUILD_ERROR_NOGUILD );
		SEND_Q( rmsg, ch->m_desc );
		return;
	}

	if( gserver->isRunHelper() )
	{
		ch->m_guildInfo->guild()->InitGradeExPosCount();

		CNetMsg::SP rmsg(new CNetMsg);
		HelperNewGuildMemberList( rmsg, ch );
		SEND_Q( rmsg, gserver->m_helper );
	}
	else
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_GAMESERVER);
		SEND_Q(rmsg, ch->m_desc);
	}
}

void do_NewGuildManage( CPC* ch, CNetMsg::SP& msg )
{
	// 길드가 있는 사람인지 체크
	if( !ch->m_guildInfo || !ch->m_guildInfo->guild() )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg( rmsg, MSG_GUILD_ERROR_NOGUILD );
		SEND_Q( rmsg, ch->m_desc );
		return;
	}
	//길드장인지 체크
	if( ch->m_guildInfo->pos() != MSG_GUILD_POSITION_BOSS  && ch->m_guildInfo->pos() != MSG_GUILD_POSITION_OFFICER)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_NOTBOSS);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if( gserver->isRunHelper() )
	{
		ch->m_guildInfo->guild()->InitGradeExPosCount();
		CNetMsg::SP rmsg(new CNetMsg);
		HelperNewGuildManage( rmsg, ch );
		SEND_Q( rmsg, gserver->m_helper );
	}
	else
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_GAMESERVER);
		SEND_Q(rmsg, ch->m_desc);
	}
}
void do_NewGuildNotice( CPC* ch, CNetMsg::SP& msg )
{
	// 길드가 있는 사람인지 체크
	CGuild* guild;
	if( !ch->m_guildInfo )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg( rmsg, MSG_GUILD_ERROR_NOGUILD );
		SEND_Q( rmsg, ch->m_desc );
		return;
	}

	guild = ch->m_guildInfo->guild();
	if( !guild )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg( rmsg, MSG_GUILD_ERROR_NOGUILD );
		SEND_Q( rmsg, ch->m_desc );
		return;
	}

	// 공지 스킬을 배운 길드인지 체크
	if( !guild->m_passiveSkillList.Find( 443 ) )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg( rmsg, MSG_NEW_GUILD_SKILL_ERROR_NOTEXIST );
		SEND_Q( rmsg, ch->m_desc );
		return;
	}

	if( gserver->isRunHelper() )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperNewGuildNotice( rmsg, ch );
		SEND_Q( rmsg, gserver->m_helper );
	}
	else
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_GAMESERVER);
		SEND_Q(rmsg, ch->m_desc);
	}
}
void do_NewGuildNoticeUpdate( CPC* ch, CNetMsg::SP& msg )
{
	CLCString title( GUILD_NOTICE_TITLE_MAX + 1 );
	CLCString text( GUILD_NOTICE_TEXT_MAX + 1 );

	RefMsg(msg) >> title
				>> text;

	if(findPercentChar(text.getBuffer()) != NULL)
		return ;

	// 길드가 있는 사람인지 체크
	if( !ch->m_guildInfo || !ch->m_guildInfo->guild() )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg( rmsg, MSG_GUILD_ERROR_NOGUILD );
		SEND_Q( rmsg, ch->m_desc );
		return;
	}

	//길드장인지 체크
	if( ch->m_guildInfo->pos() != MSG_GUILD_POSITION_BOSS )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_NOTBOSS);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 공지 스킬을 배운 길드인지 체크
	CGuild* guild = ch->m_guildInfo->guild();
	if( !guild->m_passiveSkillList.Find( 443 ) )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg( rmsg, MSG_NEW_GUILD_SKILL_ERROR_NOTEXIST );
		SEND_Q( rmsg, ch->m_desc );
		return;
	}

	if( gserver->isRunHelper() )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperNewGuildNoticeUpdate( rmsg, ch, title, text );
		SEND_Q( rmsg, gserver->m_helper );
	}
	else
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_GAMESERVER);
		SEND_Q(rmsg, ch->m_desc);
	}
}
void do_NewGuildNoticeTrans( CPC* ch, CNetMsg::SP& msg )
{
//	CLCString title( GUILD_NOTICE_TITLE_MAX + 1 );
//	CLCString text( GUILD_NOTICE_TEXT_MAX + 1 );
//
//	RefMsg(msg) >> title
//		>> text;

	// 길드가 있는 사람인지 체크
	if( !ch->m_guildInfo || !ch->m_guildInfo->guild() )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg( rmsg, MSG_GUILD_ERROR_NOGUILD );
		SEND_Q( rmsg, ch->m_desc );
		return;
	}

	//길드장인지 체크
	if( ch->m_guildInfo->pos() != MSG_GUILD_POSITION_BOSS )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_NOTBOSS);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if( gserver->isRunHelper() )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperNewGuildNoticeTrans( rmsg, ch );
		SEND_Q( rmsg, gserver->m_helper );
	}
	else
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_GAMESERVER);
		SEND_Q(rmsg, ch->m_desc);
	}
}

void do_NewGuildSkillList( CPC* ch, CNetMsg::SP& msg )
{
	// 길드가 있는 사람인지 체크
	if( !ch->m_guildInfo || !ch->m_guildInfo->guild() )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg( rmsg, MSG_GUILD_ERROR_NOGUILD );
		SEND_Q( rmsg, ch->m_desc );
		return;
	}

	if( gserver->isRunHelper() )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperNewGuildSkillList( rmsg, ch );
		SEND_Q( rmsg, gserver->m_helper );
	}
	else
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_GAMESERVER);
		SEND_Q(rmsg, ch->m_desc);
	}
}

void do_NewGuildSkillLearn( CPC* ch, CNetMsg::SP& msg )
{
	int skillindex;
	int skilltype;
	RefMsg(msg) >> skillindex;

	// 길드가 있는 사람인지 체크
	if( !ch->m_guildInfo || !ch->m_guildInfo->guild() )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg( rmsg, MSG_GUILD_ERROR_NOGUILD );
		SEND_Q( rmsg, ch->m_desc );
		return;
	}

	//길드장인지 체크
	if( ch->m_guildInfo->pos() != MSG_GUILD_POSITION_BOSS )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_NOTBOSS);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	int guildindex = ch->m_guildInfo->guild()->index();

	const CSkillProto* proto = gserver->m_skillProtoList.Find( skillindex );

	if( !proto )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg( rmsg, MSG_NEW_GUILD_SKILL_ERROR_NOTEXIST );
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	CSkillList* list = NULL;
	if( proto->IsActive() )
	{
		list = &ch->m_guildInfo->guild()->m_activeSkillList;
		skilltype = 0;
	}
	else if( proto->IsPassive() )
	{
		list = &ch->m_guildInfo->guild()->m_passiveSkillList;
		skilltype = 1;
	}
	else
	{
		list = &ch->m_guildInfo->guild()->m_etcSkillList;
		skilltype = 2;
	}

	CSkill* skill = list->Find( skillindex );
	int nextLevel = ( skill ) ? skill->m_level+1 : 1;

	const CSkillLevelProto* levelproto = proto->Level( nextLevel );
	if( !levelproto )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg( rmsg, MSG_NEW_GUILD_SKILL_ERROR_NOTEXIST );
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 스킬 검사
	if( skillindex == 444 )
	{
		CSkill* pCompSkill = list->Find( 443 );
		if( pCompSkill == NULL )
		{
			CNetMsg::SP rmsg(new CNetMsg);
			GuildErrorMsg( rmsg, MSG_NEW_GUILD_SKILL_ERROR_NOTEXIST );
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}
	}

	//레벨 검사
	if( levelproto->m_learnLevel > ch->m_guildInfo->guild()->level() )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg( rmsg, MSG_NEW_GUILD_SKILL_LEARN_ERROR_LEVEL );
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
	// gp 검사
	if( levelproto->m_learnGP > ch->m_guildInfo->guild()->GetGuildPoint() )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg( rmsg, MSG_NEW_GUILD_ERRROR_GUILDPOINT );
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 아이템 검사
	item_search_t vec[MAX_SKILL_LEARN_ITEM];
	for (int i = 0; i < MAX_SKILL_LEARN_ITEM; i++)
	{
		if (levelproto->m_learnItemIndex[i] < 0)
			continue ;

		if (levelproto->m_learnItemIndex[i] == NAS_ITEM_DB_INDEX)		//길드스킬을 배우는데 필요한 아이템이 돈이라면
		{
			if (ch->m_inventory.getMoney() < levelproto->m_learnItemCount[i])
			{
				CNetMsg::SP rmsg(new CNetMsg);
				GuildErrorMsg( rmsg, MSG_GUILD_ERROR_MONEY);
				SEND_Q(rmsg, ch->m_desc);
				return ;
			}
		}
		else
		{
			int sc = ch->m_inventory.searchItemByCondition(levelproto->m_learnItemIndex[i], 0, 0, vec[i]);
			if (sc == 0 || sc < levelproto->m_learnItemCount[i])
			{
				CNetMsg::SP rmsg(new CNetMsg);
				GuildErrorMsg( rmsg, MSG_GUILD_ERROR_ITEM);
				SEND_Q(rmsg, ch->m_desc);
				return ;
			}
		}
	}

	// GP 소모
	gserver->m_guildlist.findguild( guildindex )->AddGuildPoint( -levelproto->m_learnGP );

	// 아이템 소모
	for (int i = 0; i < MAX_SKILL_LEARN_ITEM; i++)
	{
		if (levelproto->m_learnItemIndex[i] == NAS_ITEM_DB_INDEX)		//길드스킬을 배우는데 필요한 아이템이 돈이라면
		{
			ch->m_inventory.decreaseMoney(levelproto->m_learnItemCount[i]);
		}
		else
		{
			ch->m_inventory.deleteItem(vec[i], levelproto->m_learnItemCount[i]);
		}
	}

	// 스킬 추가
	if (skill == NULL)
	{
		// 새로 배움
		skill = gserver->m_skillProtoList.Create(skillindex);
		if (!skill)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			GuildErrorMsg( rmsg, MSG_NEW_GUILD_SKILL_ERROR_NOTEXIST );
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}
		if (proto->IsPassive())
			gserver->m_guildlist.findguild( guildindex )->m_passiveSkillList.Add(skill);
		else if(proto->IsActive())
			gserver->m_guildlist.findguild( guildindex )->m_activeSkillList.Add(skill);
		else
			gserver->m_guildlist.findguild( guildindex )->m_etcSkillList.Add(skill);

#ifdef DEV_GUILD_STASH
		if ( skillindex == 1377/*'길드창고스킬'*/ )
		{
			// 핼퍼로 길드 창고 만들기 요청
			CNetMsg::SP rmsg(new CNetMsg);
			HelperGuildStashCreateMsg( rmsg , ch->m_guildInfo->guild()->index(), ch->m_index );
			SEND_Q( rmsg, gserver->m_helper )
		}
#endif //DEV_GUILD_STASH
	}
	else
	{
		// 있는 스킬 레벨 업
		if (skill->m_level >= proto->m_maxLevel)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			GuildErrorMsg( rmsg, MSG_NEW_GUILD_SKILL_ALEADY_LEARN );
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}
		skill->m_level++;
	}

	GAMELOG << init("GUILD SKILL LEARN", ch)
			<< skill->m_proto->m_index
			<< delim
			<< skill->m_level
			<< end;
	{
		CNetMsg::SP rmsg(new CNetMsg);

		unsigned char skill_type = -1;

		if(skill->m_proto->IsActive())
			skill_type = (unsigned char)SKILL_TYPE_ACTIVE;
		else if(skill->m_proto->IsPassive())
			skill_type = (unsigned char)SKILL_TYPE_PASSIVE;
		else
			skill_type = (unsigned char)SKILL_TYPE_ETC;

		HelperGuildSkillLearnSendMemberMsg(rmsg, ch->m_guildInfo->guild()->index(), skill_type, skill->m_proto->m_index, skill->m_level);
		SEND_Q(rmsg, gserver->m_helper);
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg( rmsg, MSG_NEW_GUILD_SKILL_LEARN_ERROR_OK );
		SEND_Q(rmsg, ch->m_desc );
	}

	GAMELOG << init("GUILD LEVEL UP ", ch )
			<< "Guild Index" << delim << ch->m_guildInfo->guild()->index() << delim
			<< "Guild Point" << delim << ch->m_guildInfo->guild()->GetGuildPoint() << delim
			<< "Need GP" << delim << levelproto->m_learnGP << delim
			<< end;

	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildSkillLearnMsg( rmsg, -levelproto->m_learnGP, ch->m_guildInfo->guild(), skilltype );
		SEND_Q( rmsg, gserver->m_helper  );
	}
}

void do_GuildWarSetTimeMenuReq(CPC* ch, CNetMsg::SP& msg)
{
	CWarCastle* merakcaslte = CWarCastle::GetCastleObject(ZONE_MERAC);
	CWarCastle* dratancaslte = CWarCastle::GetCastleObject(ZONE_DRATAN);
	if(!merakcaslte || !dratancaslte)
		return ;

	// 길드가 있고
	if (!ch->m_guildInfo)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildWarSetTimeRepMsg(rmsg, MSG_GUILD_WAR_ERROR_NOT_OWNER, 0, 0, 0, 0);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 성주 길드이고
	CGuild* guild = ch->m_guildInfo->guild();
	if ((guild->index() != merakcaslte->GetOwnerGuildIndex()) &&
			(guild->index() != dratancaslte->GetOwnerGuildIndex()) )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildWarSetTimeRepMsg(rmsg, MSG_GUILD_WAR_ERROR_NOT_OWNER, 0, 0, 0, 0);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 길드 장이고
	if (guild->boss()->charindex() != ch->m_index)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildWarSetTimeRepMsg(rmsg, MSG_GUILD_WAR_ERROR_NOT_OWNER, 0, 0, 0, 0);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 공성중이 아니고
	if (merakcaslte->GetState() != WCSF_NORMAL || dratancaslte->GetState() != WCSF_NORMAL)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildWarSetTimeRepMsg(rmsg, MSG_GUILD_WAR_ERROR_PROGRESS_WAR, 0, 0, 0, 0);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 신청시간이 아닐때
	if (merakcaslte->IsJoinTime() || dratancaslte->IsJoinTime())
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildWarSetTimeRepMsg(rmsg, MSG_GUILD_WAR_ERROR_NOT_SETTIME, 0, 0, 0, 0);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		rmsg->Init(MSG_GUILD);
		RefMsg(rmsg) << (unsigned char)MSG_GUILD_WAR_SET_TIME_MENU
					 << (char)2;

		struct tm nextWarTime;

		merakcaslte->GetNextWarTime(&nextWarTime, false);
		RefMsg(rmsg) << (char)merakcaslte->GetZoneIndex()
					 << (char)nextWarTime.tm_wday
					 << (char)nextWarTime.tm_hour;

		dratancaslte->GetNextWarTime(&nextWarTime, false);
		RefMsg(rmsg) << (char)dratancaslte->GetZoneIndex()
					 << (char)nextWarTime.tm_wday
					 << (char)nextWarTime.tm_hour;

		SEND_Q(rmsg, ch->m_desc);
	}
}

void do_GuildWarGetTimeUniReq(CPC* ch, CNetMsg::SP& msg)
{
	CWarCastle* merak = CWarCastle::GetCastleObject(ZONE_MERAC);
	CWarCastle* dratan = CWarCastle::GetCastleObject(ZONE_DRATAN);
	if(!merak || !dratan)
		return;

#ifdef NPC_CHECK
	int npcIndex;
	CNPC *npc;
	RefMsg(msg) >> npcIndex;
	npc = ch->m_pArea->FindProtoNPCInCell(ch, npcIndex, false, 2);
	if(!npc)
	{
		GAMELOG << init("GuildWarGetTime FAIL: NOT EXIST NPC (Dangerous User)", ch) << end;
		return ;
	}
	if( !npc->IsFlag(NPC_WARCASTLE) )
	{
		GAMELOG << init("GuildWarGetTime FAIL: NOT EXIST NPC (Dangerous User)", ch) << end;
		return ;
	}
#endif

	{
		CNetMsg::SP rmsg(new CNetMsg);
		rmsg->Init(MSG_GUILD);
		RefMsg(rmsg) << (unsigned char)MSG_GUILD_WAR_GET_TIME_UNI_REP
					 << (char)2;

		// count(c) zone(c) ownerguildname(s) month(c) day(c) hour(c) info(c) guildname(s) guildboss(s) joinflag(c) zoneflag(c)
		struct tm nextWarTime;

		merak->GetNextWarTime(&nextWarTime, true);
		RefMsg(rmsg) << (char)ZONE_MERAC
					 << merak->GetOwnerGuildName()
					 << (char)nextWarTime.tm_mon
					 << (char)nextWarTime.tm_mday
					 << (char)nextWarTime.tm_hour;

		dratan->GetNextWarTime(&nextWarTime, true);
		RefMsg(rmsg) << (char)ZONE_DRATAN
					 << dratan->GetOwnerGuildName()
					 << (char)nextWarTime.tm_mon
					 << (char)nextWarTime.tm_mday
					 << (char)nextWarTime.tm_hour;

		if(ch->m_guildInfo && ch->m_guildInfo->guild())
		{
			RefMsg(rmsg) << (char)1
						 << ch->m_guildInfo->guild()->name()
						 << ch->m_guildInfo->guild()->boss()->GetName()
						 << (char)2;

			if(ch->GetJoinFlag(ZONE_MERAC) != WCJF_NONE)
			{
				RefMsg(rmsg) << (char)ch->GetJoinFlag(ZONE_MERAC)
							 << (char)ZONE_MERAC;
			}
			else
			{
				RefMsg(rmsg) << (char)WCJF_NONE
							 << (char)-1;
			}

			if(ch->GetJoinFlag(ZONE_DRATAN) != WCJF_NONE)
			{
				RefMsg(rmsg) << (char)ch->GetJoinFlag(ZONE_DRATAN)
							 << (char)ZONE_DRATAN;
			}
			else
			{
				RefMsg(rmsg) << (char)WCJF_NONE
							 << (char)-1;
			}
		}
		else
		{
			RefMsg(rmsg) << (char)0;
		}

		SEND_Q(rmsg, ch->m_desc);
	}
}

bool CheckOverlapWarCastle(int wday, int hour, int zone)
{
	CWarCastle* castle = CWarCastle::GetCastleObject(zone);
	if(!castle)
		return false;

	CWarCastle* castle2 = NULL;
	switch(zone)
	{
	case ZONE_MERAC:
		castle2 = CWarCastle::GetCastleObject(ZONE_DRATAN);
		break;
	case ZONE_DRATAN:
		castle2 = CWarCastle::GetCastleObject(ZONE_MERAC);
		break;
	default:
		return false;
	}

	if(!castle2)
		return false;

	tm nextWarTime;
	castle2->GetNextWarTime(&nextWarTime, true);
	if(nextWarTime.tm_wday != wday)
		return true;

	// 비교할 공성 시간
	if( hour * 3600 + castle->GetWarTime() <= nextWarTime.tm_hour * 3600 )
		return true;

	if ( (nextWarTime.tm_hour * 3600 + castle2->GetWarTime() ) <= hour * 3600 )
		return true;

	return false;
}

#ifdef DEV_GUILD_MARK
void do_newGuildSkillMarkEdit(CPC* ch, CNetMsg::SP& msg)
{
	// 스킬 인덱스 수정
	if(ch == NULL || DEAD(ch))
		return ;

	if(!ch->m_guildInfo)
		return ;

	CGuild* pGuild = NULL;
	pGuild = ch->m_guildInfo->guild();
	if(!pGuild)	// 길드가 없으면..
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_NOGUILD);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if(!pGuild->m_passiveSkillList.Find(GUILD_MARK_SKILL_INDEX))
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg( rmsg, MSG_NEW_GUILD_SKILL_ERROR_NOTEXIST );
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	//if(pGuild->boss() && pGuild->boss()->GetPC())
	//{
	//	if(pGuild->boss()->GetPC()->m_index != ch->m_index)
	//	{
	//		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_NOTBOSS);
	//		SEND_Q(rmsg, ch->m_desc);
	//		return ;
	//	}
	//}

	if(pGuild->boss())
	{
		if(pGuild->boss()->charindex() != ch->m_index)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			GuildErrorMsg(rmsg, MSG_GUILD_ERROR_NOTBOSS);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}
	}

	{
		//MSG_NEW_GUILD_SKILL_ERROR_OK
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg(rmsg, MSG_NEW_GUILD_ERROR_OK_MARK_WND);
		SEND_Q(rmsg, ch->m_desc);
	}
}
void do_newGuildSkillMarkEditFinish(CPC* ch, CNetMsg::SP& msg)
{
	RequestClient::doNewGuildSkillMarkEditFinish* packet = reinterpret_cast<RequestClient::doNewGuildSkillMarkEditFinish*>(msg->m_buf);

	if (ch->m_guildInfo == NULL)
	{
		LOG_ERROR("not joined to guild. char_index[%d]", ch->m_index);
		return;
	}

	CGuild* pGuild = ch->m_guildInfo->guild();
	if (pGuild == NULL)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_NOGUILD);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if (pGuild->m_passiveSkillList.Find(GUILD_MARK_SKILL_INDEX) == NULL)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg( rmsg, MSG_NEW_GUILD_SKILL_ERROR_NOTEXIST );
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if(pGuild->boss())
	{
		if(pGuild->boss()->charindex() != ch->m_index)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			GuildErrorMsg(rmsg, MSG_GUILD_ERROR_NOTBOSS);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}
	}

	if (packet->gm_row == -1 || packet->gm_col == -1 || packet->bg_row == -1 || packet->bg_col == -1)
	{
		return;
	}

	if (ch->m_inventory.isValidNormalInventory(packet->tab, packet->invenIndex) == false)
	{
		LOG_ERROR("HACKING? : invalid packet. char_index[%d] tab[%d] invenIndex[%d]",
				  ch->m_index, packet->tab, packet->invenIndex);
		ch->m_desc->Close("invalid packet");
		return;
	}

	CItem* pItem = ch->m_inventory.getItem(packet->tab, packet->invenIndex);
	if (pItem == NULL)
	{
		LOG_ERROR("HACKING? : not found item. char_index[%d] tab[%d] invenIndex[%d]",
				  ch->m_index, packet->tab, packet->invenIndex);
		ch->m_desc->Close("not found item");
		return;
	}

	if (pItem->m_itemProto->getItemTypeIdx() != ITYPE_ETC || pItem->m_itemProto->getItemSubTypeIdx() != IETC_GUILD_MARK)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::ItemNotUseMsg(rmsg, MSG_ITEM_USE_ERROR_NOITEM);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	{
		//여기까지 통과 했으면 MSG_HELPER_NEW_GUILD_MARK_REGIST를 Helper로 보내야한다~!~!~!
		int time = (int)mktime(&gserver->m_tRealSystemTime);
		int markTime = time + (pItem->m_itemProto->getItemNum0() * 24 * 60 * 60);
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildMarkRegistReqMsg(rmsg, pGuild->index(), ch->m_index,
									packet->gm_row, packet->gm_col, packet->bg_row, packet->bg_col, packet->tab, packet->invenIndex, markTime, pItem->m_serial.c_str());
		SEND_Q(rmsg, gserver->m_helper);
	}
}
#endif

#ifdef DEV_GUILD_STASH
//XX 길드창고 - 리스트 (1)
void do_newGuildStashList( CPC* ch, CNetMsg::SP& msg )
{
	int guildIndex = 0;
	unsigned char subtype;

	msg->MoveFirst();
	RefMsg(msg) >> subtype;

	if( ch->m_guildInfo && ch->m_guildInfo->guild() )
	{
		guildIndex = ch->m_guildInfo->guild()->index();
	}

	//  TODO :  길드 권한 같은거 확인 하고
	if( guildIndex > 0 )
	{
		CSkill* pSkill = ch->m_guildInfo->guild()->FindSkill( 1377/*'길드창고스킬'*/);
		if( !pSkill )
		{
			CNetMsg::SP rmsg(new CNetMsg);
			GuildStashErrorMsg( rmsg, MSG_NEW_GUILD_STASH_ERROR, 7 );	 // 7: 스킬을 배워야함
			SEND_Q( rmsg, ch->m_desc );
			return;
		}

		if(subtype == MSG_NEW_GUILD_STASH_LIST)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			HelperGuildStashListMsg( rmsg , guildIndex, ch->m_index );
			SEND_Q( rmsg, gserver->m_helper );
		}
	}
	else
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildStashErrorMsg( rmsg, MSG_NEW_GUILD_STASH_ERROR, 1 );	 // 1: 길드가 없습니다.
		SEND_Q( rmsg, ch->m_desc );
		return;
	}
}

//XX 길드창고 - 보관 (1)
void do_newGuildStashKeep( CPC* ch, CNetMsg::SP& msg )
{
	int guildIndex = 0;
	if( ch->m_guildInfo && ch->m_guildInfo->guild() )
	{
		guildIndex = ch->m_guildInfo->guild()->index();
	}
	// TODO :  길드 권한 같은거 확인 하고

	if( guildIndex > 0 )
	{
		CSkill* pSkill = ch->m_guildInfo->guild()->FindSkill( 1377/*'길드창고스킬'*/ );
		if( !pSkill )
		{
			CNetMsg::SP rmsg(new CNetMsg);
			GuildStashErrorMsg( rmsg, MSG_NEW_GUILD_STASH_ERROR, 7 );	 // 7: 스킬을 배워야함
			SEND_Q( rmsg, ch->m_desc );
			return;
		}
	}
	else
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildStashErrorMsg( rmsg, MSG_NEW_GUILD_STASH_ERROR, 6 );	 // 6: 길드가 없습니다.
		SEND_Q( rmsg, ch->m_desc );
		return;
	}

	RequestClient::doNewGuildStashKeep* packet = reinterpret_cast<RequestClient::doNewGuildStashKeep*>(msg->m_buf);

	if (packet->keepMoney <= 0 && packet->listCount <= 0)
	{
		std::string str = boost::str(boost::format(
										 "HACKING? : invalid keeyMoney[%1%] or listCount[%2%]. charIndex[%3%]") % packet->keepMoney % packet->listCount % ch->m_index);
		ch->m_desc->Close("invalid keeyMoney or listCount");
		return;
	}

	if (packet->keepMoney > 0)
	{
		if (ch->m_inventory.getMoney() < packet->keepMoney)
		{
			LOG_ERROR("HACKING? : not enought keepMoney. char_index[%d]", ch->m_index);
			ch->m_desc->Close("not enought keepMoney");
			return;
		}
	}

	if (packet->listCount < 0 || packet->listCount > 5)
	{
		LOG_ERROR("HACKING? : invalid itemCount[%d]. char_index[%d]", packet->listCount, ch->m_index);
		ch->m_desc->Close("invalid itemCount");
		return;
	}

	{
		std::set<int> check_duplication;

		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildStashKeepMsg( rmsg, guildIndex, ch->m_index, packet->keepMoney, packet->listCount );

		for (int i = 0; i < packet->listCount; ++i)
		{
			if (packet->list[i].itemDBIndex <= 0)
			{
				LOG_ERROR("HACKING? : invalid itemIndex[%d]. char_index[%d]", packet->list[i].itemDBIndex, ch->m_index);
				ch->m_desc->Close("invalid itemIndex");
				return;
			}

			if (packet->list[i].count <= 0)
			{
				std::string str = boost::str(boost::format("HACKING? : invalid count[%1%]. char_index[%2%]")
											 % packet->list[i].count % ch->m_index);
				LOG_ERROR(str.c_str());
				ch->m_desc->Close("invalid count");
				return;
			}

			if (ch->m_inventory.isValidNormalInventory(packet->list[i].tab, packet->list[i].invenIndex) == false)
			{
				LOG_ERROR("HACKING? : invalid packet. char_index[%d] tab[%d] invenIndex[%d]",
						  ch->m_index, packet->list[i].tab, packet->list[i].invenIndex);
				ch->m_desc->Close("invalid packet");
				return;
			}

			if( packet->list[i].tab >= INVENTORY_TAB_MAX )
			{
				LOG_ERROR("HACKING? : not usable tab. char_index[%d] tab[%d] invenIndex[%d]",
						  ch->m_index, packet->list[i].tab, packet->list[i].invenIndex);
				ch->m_desc->Close("not usable tab");
				return;
			}

			CItem* pItem = ch->m_inventory.getItem(packet->list[i].tab, packet->list[i].invenIndex);

			if (pItem == NULL)
			{
				LOG_ERROR("HACKING? : not found item. char_index[%d] tab[%d] invenIndex[%d]",
						  ch->m_index, packet->list[i].tab, packet->list[i].invenIndex);
				ch->m_desc->Close("not found item");
				return;
			}

			if (pItem->Count() < packet->list[i].count)
			{
				std::string str = boost::str(boost::format("HACKING? : invalid count[%1%]. char_index[%2%]")
											 % packet->list[i].count % ch->m_index);
				LOG_ERROR(str.c_str());
				ch->m_desc->Close("invalid count");
				return;
			}

			if (check_duplication.insert(packet->list[i].tab << 16 | packet->list[i].invenIndex).second == false)
			{
				LOG_ERROR("HACKING? : duplicate invenIndex. char_index[%d]", ch->m_index);
				ch->m_desc->Close("duplicate invenIndex");
				return;
			}

			if(pItem->IsBelong() == true)
			{
				CNetMsg::SP rmsg(new CNetMsg);
				SysMsg(rmsg, MSG_SYS_BELONG_ITEM);
				SEND_Q(rmsg, ch->m_desc);
				return;
			}

			// 보관할 수 없는 아이템
			if( pItem->CanKeepStash() == false 				// 보환할 수 없거나
#ifdef ENABLE_SUBJOB
					|| pItem->CanUseTrader( TRADE_STASH, ch->IsSubJob(JOB_SUB_TRADER) ) == false 	// 상인이 사용할수 없거나
#endif //ENABLE_SUBJOB
					|| pItem->getDBIndex() != packet->list[i].itemDBIndex 	// 아이템이 다르거나
			  )
			{
				CNetMsg::SP rmsg(new CNetMsg);
				GuildStashErrorMsg( rmsg, MSG_NEW_GUILD_STASH_KEEP , 5 ); // 5: 보관 할 수 없는 아이템이 있습니다.
				SEND_Q( rmsg, ch->m_desc );
				return;
			}

			RefMsg(rmsg) << pItem->getDBIndex()
						 << pItem->getVIndex()
						 << pItem->GetItemPlus()
						 << pItem->getFlag()
						 << pItem->getUsed()
						 << pItem->getUsed_2()
						 << pItem->getNowDurability()
						 << pItem->getMaxDurability()
						 << packet->list[i].count;

			bool bIsRare = false;
			if (pItem->IsRareItem())
				bIsRare = true;
			if (bIsRare)
			{
				if (pItem->m_nOption == 2)
				{
					RefMsg(rmsg) << (short) pItem->m_nRareOptionIndex;
					RefMsg(rmsg) << (short) pItem->m_nRareOptionBit;
					for (int j = 2; j < MAX_ITEM_OPTION; ++j)
					{
						RefMsg(rmsg) <<  (short)0;
					}
				}
				else
				{
					for (int j = 0; j < MAX_ITEM_OPTION; ++j)
					{
						RefMsg(rmsg) << (short)0;
					}
				}
			}
			else
			{
				for (int j = 0; j < MAX_ITEM_OPTION; ++j)
				{
					if (pItem->m_option[j].m_level > 0)
					{
						short dbval = 0;
						dbval |= (pItem->m_option[j].m_type << OPTION_VALUE_SHIFT) & 0xff00;
						dbval |= pItem->m_option[j].m_level & 0x00ff;
						RefMsg(rmsg) << dbval;
					}
					else
						RefMsg(rmsg) << (short) 0;
				}
			}

			char	socket[SOCKET_DB_LENGTH] = {0,};
			pItem->m_socketList.GetSocketInfoString(socket);

			// 나누어지는 경우라면 serial을 따로 만든다.
			if (pItem->getItemCount() == packet->list[i].count)
			{
				RefMsg(rmsg) << pItem->m_serial.c_str();
			}
			else
			{
				std::string tserial = GetSerial();
				RefMsg(rmsg) << tserial.c_str();
			}

			RefMsg(rmsg) << socket
						 << pItem->m_itemProto->getItemFlag();

			for (int j = 0; j < MAX_VARIATION_COUNT; ++j)
				RefMsg(rmsg) << pItem->getOriginVar(j);
		}

		SEND_Q( rmsg, gserver->m_helper );
	}
}

//XX 길드창고 - 찾기 (1)
void do_newGuildStashTake( CPC* ch, CNetMsg::SP& msg )
{
	RequestClient::doNewGuildStashTake* packet = reinterpret_cast<RequestClient::doNewGuildStashTake*>(msg->m_buf);
	if (ch->m_guildInfo == NULL)
	{
		LOG_ERROR("HACKING? : not guild info. charIndex[%d]", ch->m_index);
		ch->m_desc->Close("not guild info");
		return;
	}

	if (ch->m_guildInfo->guild() == NULL)
	{
		LOG_ERROR("HACKING? : not guild info. charIndex[%d]", ch->m_index);
		ch->m_desc->Close("not guild info");
		return;
	}

	if (ch->m_guildInfo->guild()->index() != packet->guildIndex)
	{
		LOG_ERROR("HACKING? : not equal guild index[%d]. charIndex[%d]", packet->guildIndex, ch->m_index);
		ch->m_desc->Close("not equal guild index");
		return;
	}

	if (packet->takeMoney == 0 && packet->listCount == 0)
	{
		std::string str = boost::str(boost::format(
										 "HACKING? : invalid takeMoney[%1%] or listCount[%2%]. charIndex[%3%]")
									 % packet->takeMoney % packet->listCount % ch->m_index);
		LOG_ERROR(str.c_str());
		ch->m_desc->Close("invalid takeMoney or listCount");
		return;
	}

	CSkill* pSkill = ch->m_guildInfo->guild()->FindSkill( 1377/*'길드창고스킬'*/ );
	if (pSkill == NULL)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildStashErrorMsg( rmsg, MSG_NEW_GUILD_STASH_ERROR, 2 );	 // 2: 스킬을 배워야함
		SEND_Q( rmsg, ch->m_desc );
		return;
	}

	{
		std::set<int> check_duplicate;
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildStashTakeMsg( rmsg, packet->guildIndex, ch->m_index, packet->takeMoney, packet->listCount );
		for (int i = 0; i < packet->listCount; ++i)
		{
			if (check_duplicate.insert(packet->list[i].stashIndex).second == false)
			{
				LOG_ERROR("HACKING? : stash index is duplication. charIndex[%d]", ch->m_index);
				ch->m_desc->Close("stash index is duplication");
				return;
			}

			RefMsg(rmsg) << packet->list[i].stashIndex
						 << packet->list[i].itemCount;
		}
		SEND_Q( rmsg, gserver->m_helper );
	}
}

void do_newGuildStashLog( CPC* ch, CNetMsg::SP& msg )
{
	int lastLogIdx = 0;
	RefMsg(msg) >> lastLogIdx;

	int guildIndex = 0;
	if( ch->m_guildInfo && ch->m_guildInfo->guild() )
	{
		guildIndex = ch->m_guildInfo->guild()->index();
	}

	// TODO : 길드 권한 같은거 확인 하고
	if( guildIndex > 0 )
	{
		CSkill* pSkill = ch->m_guildInfo->guild()->FindSkill( 1377/*'길드창고스킬'*/ );
		if( !pSkill )
		{
			CNetMsg::SP rmsg(new CNetMsg);
			GuildStashErrorMsg( rmsg, MSG_NEW_GUILD_STASH_ERROR, 7 );	 // 7: 스킬을 배워야함
			SEND_Q( rmsg, ch->m_desc );
			return;
		}

		{
			CNetMsg::SP rmsg(new CNetMsg);
			HelperGuildStashLogMsg( rmsg , ch->m_index, guildIndex, lastLogIdx );
			SEND_Q( rmsg, gserver->m_helper );
		}
	}
	else
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildStashErrorMsg( rmsg, MSG_NEW_GUILD_STASH_ERROR, 1 );	 // 1: 길드가 없습니다.
		SEND_Q( rmsg, ch->m_desc );
		return;
	}
}
#endif // DEV_GUILD_STASH

void do_GuildMasterKickReq( CPC* ch, CNetMsg::SP& msg )
{
	int _guildIndex = -1;
	RefMsg(msg) >> _guildIndex;

	// TODO 1 :: 길드 유효성 검사
	CGuild* _guild = gserver->m_guildlist.findguild(_guildIndex);
	if ( !_guild )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_NOGUILD);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 길드가 해임 대기 상태인지 검사
	if ( _guild->getGuildKick()->getKickStatus() == GMKS_RESERVE )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg(rmsg, MSG_NEW_GUILD_ERROR_MASTER_KICK_RESERVE);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// TODO 2 :: 신청 캐릭터 유효성 검사
	CGuildMember* _member = _guild->findmember(ch->m_index);
	if ( !_member )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_NOTFOUNDMEMBER);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 신청 캐릭터가 길드 부장인지 검사
	if ( ch->m_guildInfo->pos() != MSG_GUILD_POSITION_OFFICER )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_NOTOFFICER);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// TODO 3 :: 길드마스터 접속 날짜 7일 확인
	if ( !_guild->isPossibleKickMaster() )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg(rmsg, MSG_NEW_GUILD_ERROR_MASTER_KICK_LAST_CONNECT);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// TODO 4 :: 길드마스터가 변경되고 7일이 지났는지 확인
	if ( !_guild->isPastLastChangeBoss() )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg(rmsg, MSG_NEW_GUILD_ERROR_MASTER_KICK_LAST_CHANGE);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// TODO 5 :: 보스 추방 요청
	if ( gserver->isRunHelper() )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildMasterKickReq(rmsg, _guildIndex, ch->m_index);
		SEND_Q(rmsg, gserver->m_helper);
		return;
	}
	else
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_GAMESERVER);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
}

void do_GuildMasterKickCancelReq( CPC* ch, CNetMsg::SP& msg )
{
	int _guildIndex = -1;
	RefMsg(msg) >> _guildIndex;

	// TODO 1 :: 길드 유효성 검사
	CGuild* _guild = gserver->m_guildlist.findguild(_guildIndex);
	if ( !_guild )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_NOGUILD);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// TODO 2 :: 길드가 해임 대기 상태인지 검사
	if ( _guild->getGuildKick()->getKickStatus() != GMKS_RESERVE )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg(rmsg, MSG_NEW_GUILD_ERROR_MASTER_KICK_CANCEL_NOT_RESERVE);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// TODO 3 :: 캐릭터가 유효성 검사
	CGuildMember* _member = _guild->findmember(ch->m_index);
	if ( !_member )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_NOGUILD);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// TODO 4 :: 캐릭터가 보스인지 검사
	if ( ch->m_guildInfo->pos() != MSG_GUILD_POSITION_BOSS)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_NOTBOSS);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// TODO 5 :: 헬퍼에 취소 요청
	if ( gserver->isRunHelper() )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildMasterKickCancelReq(rmsg, _guildIndex, ch->m_index);
		SEND_Q(rmsg, gserver->m_helper);
		return;
	}
	else
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_GAMESERVER);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
}

#if defined (GUILD_REMOTE_INVITE)
void do_GuildRemoteJoinReq( CPC* ch, CNetMsg::SP& msg )
{
	CLCString targetName(MAX_CHAR_NAME_LENGTH + 1), requestName(MAX_CHAR_NAME_LENGTH + 1);
	int type = 0;

	RefMsg(msg) >> targetName
				>> requestName
				>> type;

	CPC* bossPC = NULL;		// 길마 PC
	CPC* joinPC = NULL;		// 가입 PC

	if ( type == GRI_MASTER )
	{
		// 길마가 가입 권유했을 때,
		bossPC = ch;
		joinPC = PCManager::instance()->getPlayerByName(targetName, true);
	}
	else if ( type == GRI_JOINER )
	{
		// 유저가 가입 신청했을 때,
		bossPC = PCManager::instance()->getPlayerByName(targetName, true);
		joinPC = ch;
	}
	else
	{
		// 그 외는 없다.
		return;
	}

	if ( (bossPC != NULL && bossPC->m_pZone->isRVRZone()) 
		|| (joinPC != NULL && joinPC->m_pZone->isRVRZone()) )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_CANNOT_REGISTER_REQ_ZONE);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	int _checkResult = do_CheckGuildJoin(bossPC, joinPC);
	if ( _checkResult != 0 )
	{
		// 검사 통과를 못했다면, 에러를 그대로 보낸다
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg(rmsg, (MSG_GUILD_ERROR_TYPE)_checkResult);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 검사 통과
	bossPC->m_regGuild = joinPC->m_index;
	joinPC->m_regGuild = bossPC->m_index;

	SEND_Q(msg, bossPC->m_desc);
	SEND_Q(msg, joinPC->m_desc);
}
void do_GuildRemoteJoinOK( CPC* ch, CNetMsg::SP& msg )
{
	CLCString targetName(MAX_CHAR_NAME_LENGTH + 1);
	int type = 0;
	RefMsg(msg) >> targetName
				>> type;

	CPC* bossPC = NULL;		// 길마 PC
	CPC* joinPC = NULL;		// 가입 PC

	if ( type == GRI_MASTER )
	{
		// 길마가 가입 권유했을 때,
		bossPC = ch;
		joinPC = PCManager::instance()->getPlayerByName(targetName, true);
	}
	else if ( type == GRI_JOINER )
	{
		// 유저가 가입 신청했을 때,
		bossPC = PCManager::instance()->getPlayerByName(targetName, true);
		joinPC = ch;
	}
	else
	{
		// 그 외는 없다.
		return;
	}

	// 둘 중 하나라도 없으면, 취소
	if (!bossPC && joinPC)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildRegistCancelMsg(rmsg, false);
		SEND_Q(rmsg, joinPC->m_desc);
		joinPC->m_regGuild = 0;
		return ;
	}
	else if (!joinPC && bossPC)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildRegistCancelMsg(rmsg, false);
		SEND_Q(rmsg, bossPC->m_desc);
		bossPC->m_regGuild = 0;
		return ;
	}

	// 서로 가입 처리중인 내용이 다르면, 취소
	if (bossPC->m_regGuild != joinPC->m_index || joinPC->m_regGuild != bossPC->m_index)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_NOGUILD);
		SEND_Q(rmsg, bossPC->m_desc);
		SEND_Q(rmsg, joinPC->m_desc);
		bossPC->m_regGuild = 0;
		joinPC->m_regGuild = 0;
		return ;
	}

	CGuild* guild = bossPC->m_guildInfo->guild();
	// 가입하고자 하는 길드가 없으면, 취소
	if (!guild)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildRegistCancelMsg(rmsg, true);
		SEND_Q(rmsg, bossPC->m_desc);
		SEND_Q(rmsg, joinPC->m_desc);
		bossPC->m_regGuild = 0;
		joinPC->m_regGuild = 0;
		return ;
	}

	// 가입받을 자리가 없으면, 취소
	if (guild->maxmember() <= guild->membercount())
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_FULLMEMBER);
		SEND_Q(rmsg, bossPC->m_desc);
		SEND_Q(rmsg, joinPC->m_desc);
		bossPC->m_regGuild = 0;
		joinPC->m_regGuild = 0;
		return ;
	}

	CGuildMember* member = guild->findmember(bossPC->m_index);
	// 보스 캐릭터 검사
	if (!member)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildRegistCancelMsg(rmsg, true);
		SEND_Q(rmsg, bossPC->m_desc);
		SEND_Q(rmsg, joinPC->m_desc);
		bossPC->m_regGuild = 0;
		joinPC->m_regGuild = 0;
		return ;
	}
	// 보스가 아니면, 취소
	if (member->pos() != MSG_GUILD_POSITION_BOSS && member->pos() != MSG_GUILD_POSITION_OFFICER)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildRegistCancelMsg(rmsg, true);
		SEND_Q(rmsg, bossPC->m_desc);
		SEND_Q(rmsg, joinPC->m_desc);
		bossPC->m_regGuild = 0;
		joinPC->m_regGuild = 0;
		return ;
	}
	if (member->GetPC() == NULL)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildRegistCancelMsg(rmsg, true);
		SEND_Q(rmsg, bossPC->m_desc);
		SEND_Q(rmsg, joinPC->m_desc);
		bossPC->m_regGuild = 0;
		joinPC->m_regGuild = 0;
		return ;
	}

	if (gserver->isRunHelper())
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildMemberAddReqMsg(rmsg, guild->index(), bossPC->m_index, joinPC->m_index, joinPC->GetName());
		RefMsg(rmsg) << joinPC->m_pZone->m_index;
		SEND_Q(rmsg, gserver->m_helper);
	}
	else
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_GAMESERVER);
		SEND_Q(rmsg, bossPC->m_desc);
		SEND_Q(rmsg, joinPC->m_desc);
		bossPC->m_regGuild = 0;
		joinPC->m_regGuild = 0;
		return ;
	}
}

void do_GuildRemoteJoinNO( CPC* ch, CNetMsg::SP& msg )
{
	if (ch->m_regGuild == 0)
		return ;

	CPC* pc = PCManager::instance()->getPlayerByCharIndex(ch->m_regGuild);
	if (pc)
	{
		if (pc->m_regGuild == ch->m_index)
		{
			pc->m_regGuild = 0;
			SEND_Q(msg, pc->m_desc);
		}
	}
	ch->m_regGuild = 0;
	SEND_Q(msg, ch->m_desc);
}

int do_CheckGuildJoin(CPC* bossPC, CPC* joinPC)
{
	// 가입 캐릭터 유효성 검사
	if ( !joinPC )
	{
		return MSG_NEW_GUILD_ERROR_NOT_EXIST_CHAR;
	}

	// 길마 캐릭터 유효성 검사
	if ( !bossPC )
	{
		return MSG_NEW_GUILD_ERROR_NOT_EXIST_CHAR;
	}

	// 길마 캐릭터와 가입 캐릭터가 같은지 검사
	if ( bossPC->m_index == joinPC->m_index )
	{
		return MSG_NEW_GUILD_ERROR_NOT_EXIST_CHAR;
	}

	// 길드 유효성 검사
	if ( !bossPC->m_guildInfo )
	{
		return MSG_GUILD_ERROR_NOTBOSS;
	}
	CGuild* guild = bossPC->m_guildInfo->guild();
	if ( !guild )
	{
		return MSG_GUILD_ERROR_NOTBOSS;
	}

	// 가입 캐릭터가 이미 길드가 있는지 검사
	if ( joinPC->m_guildInfo )
	{
		return MSG_NEW_GUILD_ERROR_EXIST_GUILD;
	}

	// 가입할 캐릭터의 재가입 딜레이 검사
	if (joinPC->m_guildoutdate != 0)
	{
		time_t t;
		if (time(&t) / 60 / 60 / 24 < joinPC->m_guildoutdate + GUILD_REG_DELAY)
		{
			return MSG_GUILD_ERROR_REGDELAY;
		}
	}

	//int guildindex = guild->index();

	// 보스 검사
	CGuildMember* member = guild->findmember(bossPC->m_index);
	if (member == NULL)
	{
		return MSG_GUILD_ERROR_NOTBOSS;
	}
	if (member->pos() != MSG_GUILD_POSITION_BOSS && member->pos() != MSG_GUILD_POSITION_OFFICER)
	{
		return MSG_GUILD_ERROR_NOTBOSS;
	}

	// 가입 처리중인 캐릭터가 있는지 검사
	if (bossPC->m_regGuild && bossPC->m_regGuild != joinPC->m_index)
	{
		return MSG_GUILD_ERROR_TARGET_BUSY;
	}

	// 가입 신청 처리중인지 검사
	if (joinPC->m_regGuild && joinPC->m_regGuild != member->charindex())
	{
		return MSG_GUILD_ERROR_SOURCE_BUSY;
	}

	// 공성 중인지 검사
	if (guild->battleState() != GUILD_BATTLE_STATE_PEACE)
	{
		return MSG_GUILD_ERROR_CANNOT_REGIST_REQ_BATTLE;
	}

	return 0;
}
#endif // GUILD_REMOTE_INVITE

void do_GuildRecall(CPC* pc, CNetMsg::SP& rmsg)
{
	//수락 하였을 경우에만 전달되는 메시지로 길드방으로 이동시켜주면 된다.
	CZone* pZone = gserver->FindZone(ZONE_GUILDROOM);
	int area_count = 0;

	if(pZone == NULL)
		return;

	// 태국 pvp 존 이동 아이템이
	// 귀환 스크롤의 num1을 존번호로 씁니당.
	// 고로 요거 추가합니당 : bw

	if (!pc->m_guildInfo || !(pc->m_guildInfo->guild()))
	{
		CNetMsg::SP rmsg(new CNetMsg);
		SysMsg(rmsg, MSG_SYS_DO_NOT_GO_ZONE_GMORDER);
		SEND_Q(rmsg, pc->m_desc);
		return;
	}

	int idx;
	for (idx=0; idx < pZone->m_countArea; idx++)
	{
		if (!pZone->m_area[idx].m_bEnable)
			continue;

		if (pZone->m_area[idx].m_guildIndex == pc->m_guildInfo->guild()->index())
		{
			area_count = idx;
			break;
		}
	}

	if (idx == pZone->m_countArea)
	{
		area_count = pZone->SetEnableArea();
		// 빈 영억 없음
		if (area_count == -1)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			SysMsg(rmsg, MSG_SYS_SINGLEDUNGEON_FULL);
			SEND_Q(rmsg, pc->m_desc);
			return ;
		}
		pZone->m_area[area_count].m_guildIndex = pc->m_guildInfo->guild()->index();
	}

	if (pZone)
	{
		// 060109 : BS : BEGIN : 존이동 메시지 사용하지 않고 직접 호출
		GoZone(pc, ZONE_GUILDROOM,
			pZone->m_zonePos[0][0],														// ylayer
			GetRandom(pZone->m_zonePos[0][1], pZone->m_zonePos[0][3]) / 2.0f,		// x
			GetRandom(pZone->m_zonePos[0][2], pZone->m_zonePos[0][4]) / 2.0f);	// z
	}
}
//
