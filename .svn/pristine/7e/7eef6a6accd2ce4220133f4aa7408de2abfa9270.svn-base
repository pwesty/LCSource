#include <boost/format.hpp>
#include "stdhdrs.h"

#include "Server.h"
#include "doFunc.h"
#include "CmdMsg.h"
#include "Log.h"

bool do_GuildBossChange(int _guildIndex, int _newBoss)
{
	CGuild* guild = gserver.m_guildlist.findguild(_guildIndex);
	if (!guild)
	{
		GAMELOG << init("GUILD MASTER AUTO CHANGE")
				<< "ERROR : NOT EXIST GUILD";
		return false;
	}

	CGuildMember* boss = guild->boss();
	if (!boss)
	{
		GAMELOG << init("GUILD MASTER AUTO CHANGE")
				<< "ERROR : NOT EXIST BOSS";
		return false;
	}
	int _currentBoss = boss->charindex();

	CGuildMember* member = guild->findmember(_newBoss);
	if (!member)
	{
		GAMELOG << init("GUILD MASTER AUTO CHANGE")
				<< "ERROR : NOT MEMBER" << delim
				<< guild->index() << delim
				<< guild->name() << delim
				<< _currentBoss << delim
				<< boss->GetName() << delim
				<< _newBoss;
		return false;
	}
	if (member->pos() == MSG_GUILD_POSITION_BOSS)
	{
		GAMELOG << init("GUILD MASTER AUTO CHANGE")
				<< "ERROR : ALREADY BOSS" << delim
				<< guild->index() << delim
				<< guild->name() << delim
				<< _currentBoss << delim
				<< boss->GetName() << delim
				<< _newBoss << delim
				<< member->GetName();
		return false;
	}

	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);

	// 성주 길드는 성주 변경 불가능
	std::string select_castle_query = boost::str(boost::format("SELECT * FROM t_castle WHERE a_owner_guild_index = %d;") % _guildIndex);
	cmd.SetQuery( select_castle_query );
	if( cmd.Open() && cmd.m_nrecords > 0 )
	{
		GAMELOG << init("GUILD MASTER AUTO CHANGE")
				<< "ERROR : CASTLE OWNER" << delim
				<< guild->index() << delim
				<< guild->name() << delim
				<< _currentBoss << delim
				<< boss->GetName() << delim
				<< _newBoss << delim
				<< member->GetName();
		return false;
	}

	cmd.BeginTrans();
	std::string update_guild_member_query = boost::str(boost::format(
			"UPDATE t_guildmember SET a_pos = %d WHERE a_guild_index = %d AND a_char_index = %d")
											% MSG_GUILD_POSITION_MEMBER % _guildIndex % _currentBoss);
	cmd.SetQuery(update_guild_member_query);
	if (!cmd.Update())
	{
		cmd.Rollback();

		GAMELOG << init("GUILD MASTER AUTO CHANGE")
				<< "ERROR : UPDATE CURRENT GUILD BOSS POSITION" << delim
				<< guild->index() << delim
				<< guild->name() << delim
				<< _currentBoss << delim
				<< boss->GetName() << delim
				<< _newBoss << delim
				<< member->GetName();

		return false;
	}

	std::string update_guild_member_query_1 = boost::str(boost::format(
				"UPDATE t_guildmember SET a_pos = %d WHERE a_guild_index = %d AND a_char_index = %d")
			% MSG_GUILD_POSITION_BOSS % _guildIndex % _newBoss);
	cmd.SetQuery(update_guild_member_query_1);
	if (!cmd.Update() || cmd.m_nrecords < 1)
	{
		cmd.Rollback();

		GAMELOG << init("GUILD MASTER AUTO CHANGE")
				<< "ERROR : UPDATE NEW GUILD BOSS POSITION" << delim
				<< guild->index() << delim
				<< guild->name() << delim
				<< _currentBoss << delim
				<< boss->GetName() << delim
				<< _newBoss << delim
				<< member->GetName();

		return false;
	}
	cmd.Commit();

	std::string update_guild_query = boost::str(boost::format("UPDATE t_guild SET a_recentdate=NOW() WHERE a_index = %d") % _guildIndex);
	cmd.SetQuery(update_guild_query);
	cmd.Update();

	int currentTime = (int)time(NULL);
	std::string update_guild_extend_guild = boost::str(boost::format("UPDATE t_extend_guild SET a_lastdate_change_boss = %d WHERE a_guild_index = %d") % currentTime % _guildIndex);
	cmd.SetQuery(update_guild_extend_guild);
	cmd.Update();

	guild->changeboss(member->listindex());

	CNetMsg::SP rmsg(new CNetMsg);
	HelperGuildChangeBossNotifyMsg(rmsg, _guildIndex, _currentBoss, _newBoss);
	gserver.SendToAllGameServer(rmsg);

	GAMELOG << init("GUILD MASTER AUTO CHANGE")
			<< "SUCCESS" << delim
			<< guild->index() << delim
			<< guild->name() << delim
			<< _currentBoss << delim
			<< boss->GetName() << delim
			<< _newBoss << delim
			<< member->GetName();

	return true;
}

void do_GuildStatusChange(int _guildIndex, int _status)
{
	CGuild* _guild = gserver.m_guildlist.findguild(_guildIndex);
	if (!_guild)
		return;
	if (_status < GMKS_NORMAL || _status > GMKS_RESERVE)
		return;

	CLCString changeStatusQuery(256);
	changeStatusQuery.Format("UPDATE t_extend_guild SET a_kick_status = %d WHERE a_guild_index = %d", _status, _guildIndex);

	CDBCmd changeStatusCmd;
	changeStatusCmd.Init(&gserver.m_dbchar);
	changeStatusCmd.SetQuery(changeStatusQuery);

	if (!changeStatusCmd.Update())
		return;

	_guild->getGuildKick()->setKickStatus(_status);

	CNetMsg::SP rmsg(new CNetMsg);
	HelperGuildMasterKickStatus(rmsg, _guildIndex, _status);
	gserver.SendToAllGameServer(rmsg);
}
