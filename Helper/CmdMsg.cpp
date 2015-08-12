#include "stdhdrs.h"

#include "Server.h"
#include "../ShareLib/DBCmd.h"
#include "Guild.h"
#include "CmdMsg.h"
#include "doFunc.h"
//#include "Friend.h"

void FailMsg(CNetMsg::SP& msg, MSG_FAIL_TYPE failtype)
{
	msg->Init(MSG_FAIL);
	RefMsg(msg) << (unsigned char)failtype;
}

void HelperWhisperNotfoundMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, int sidx, const char* sname)
{
	msg->Init(MSG_HELPER_REP);
	RefMsg(msg)	<< seq
		<< server << subno << zone
		<< (unsigned char)MSG_HELPER_WHISPER_NOTFOUND
		<< sidx << sname;
}

void HelperWhisperRepMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, int sidx, const char* sname, const char* rname, const char* chat)
{
	msg->Init(MSG_HELPER_REP);
	RefMsg(msg)	<< seq
		<< server
		<< subno
		<< zone
		<< (unsigned char)MSG_HELPER_WHISPER_REP
		<< sidx << sname
		<< rname
		<< chat;
}

void HelperGuildCreateRepMsg(CNetMsg::SP& msg, int charindex, MSG_GUILD_ERROR_TYPE errcode)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_GUILD_CREATE_REP
		<< charindex
		<< (unsigned char)errcode;
}

void HelperGuildCreateNotifyMsg(CNetMsg::SP& msg, CGuild* guild)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_GUILD_CREATE_NTF
		<< guild->index()
		<< guild->level()
		<< guild->name()
		<< guild->boss()->charindex()
		<< guild->boss()->GetName();
}

void HelperGuildOnlineNotifyMsg(CNetMsg::SP& msg, CGuildMember* member)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_GUILD_ONLINE_NTF
		<< member->guild()->index()
		<< member->charindex()
		<< member->online();
}

void HelperGuildMarkTableMsg(CNetMsg::SP& msg)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_GUILD_MARK_TABLE;
	for(int i = 0; i < 3 ; i++)
	{
		RefMsg(msg) << gserver.m_nGuildMarkTable[i];
	};
}

void HelperGuildLoadRepMsg(CNetMsg::SP& msg, const char* idname, int charindex, MSG_GUILD_ERROR_TYPE errcode)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_GUILD_LOAD_REP
		<< idname
		<< charindex
		<< (unsigned char)errcode;
}

void HelperGuildLevelUpRepMsg(CNetMsg::SP& msg, int guildindex, int charindex, MSG_GUILD_ERROR_TYPE errcode)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_GUILD_LEVELUP_REP
		<< guildindex
		<< charindex
		<< (unsigned char)errcode;
}

void HelperGuildLevelUpNotifyMsg(CNetMsg::SP& msg, int guildindex, int level)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_GUILD_LEVELUP_NTF
		<< guildindex
		<< level;
}

void HelperGuildBreakUpRepMsg(CNetMsg::SP& msg, int charindex, MSG_GUILD_ERROR_TYPE errcode)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_GUILD_BREAKUP_REP
		<< charindex
		<< (unsigned char)errcode;
}

void HelperGuildBreakUpNotifyMsg(CNetMsg::SP& msg, int guildindex)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_GUILD_BREAKUP_NTF
		<< guildindex;
}

void HelperGuildMemberAddRepMsg(CNetMsg::SP& msg, int guildindex, int bossindex, int charindex, MSG_GUILD_ERROR_TYPE errcode)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_GUILD_MEMBER_ADD_REP
		<< guildindex
		<< bossindex
		<< charindex
		<< (unsigned char)errcode;
}

void HelperGuildMemberAddNotifyMsg(CNetMsg::SP& msg, int guildindex, int charindex, const char* name, MSG_GUILD_POSITION_TYPE pos)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_GUILD_MEMBER_ADD_NTF
		<< guildindex
		<< charindex
		<< name
		<< pos;
}

void HelperGuildMemberOutRepMsg(CNetMsg::SP& msg, int guildindex, int charindex, MSG_GUILD_ERROR_TYPE errcode)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_GUILD_MEMBER_OUT_REP
		<< guildindex
		<< charindex
		<< (unsigned char)errcode;
}

void HelperGuildMemberOutNotifyMsg(CNetMsg::SP& msg, int guildindex, int charindex, const char* charname)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_GUILD_MEMBER_OUT_NTF
		<< guildindex
		<< charindex
		<< charname;
}

void HelperGuildMemberKickRepMsg(CNetMsg::SP& msg, int guildindex, int bossindex, int charindex, MSG_GUILD_ERROR_TYPE errcode)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_GUILD_MEMBER_KICK_REP
		<< guildindex
		<< bossindex
		<< charindex
		<< (unsigned char)errcode;
}

void HelperGuildMemberKickNotifyMsg(CNetMsg::SP& msg, int guildindex, int bossindex, int charindex)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_GUILD_MEMBER_KICK_NTF
		<< guildindex
		<< bossindex
		<< charindex;
}

void HelperGuildChangeBossRepMsg(CNetMsg::SP& msg, int guildindex, int current, int change, MSG_GUILD_ERROR_TYPE errcode)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_GUILD_CHANGE_BOSS_REP
		<< guildindex
		<< current
		<< change
		<< (unsigned char)errcode;
}

void HelperGuildChangeBossNotifyMsg(CNetMsg::SP& msg, int guildindex, int current, int change)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_GUILD_CHANGE_BOSS_NTF
		<< guildindex
		<< current
		<< change;
}

void HelperGuildAppointOfficerRepMsg(CNetMsg::SP& msg, int guildindex, int bossindex, int charindex, MSG_GUILD_ERROR_TYPE errcode)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_GUILD_APPOINT_OFFICER_REP
		<< guildindex
		<< bossindex
		<< charindex
		<< (unsigned char)errcode;
}

void HelperGuildAppointOfficerNotifyMsg(CNetMsg::SP& msg, int guildindex, int bossindex, int charindex)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_GUILD_APPOINT_OFFICER_NTF
		<< guildindex
		<< bossindex
		<< charindex;
}

void HelperGuildFireOfficerRepMsg(CNetMsg::SP& msg, int guildindex, int bossindex, int charindex, MSG_GUILD_ERROR_TYPE errcode)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_GUILD_FIRE_OFFICER_REP
		<< guildindex
		<< bossindex
		<< charindex
		<< (unsigned char)errcode;
}

void HelperGuildFireOfficerNotifyMsg(CNetMsg::SP& msg, int guildindex, int bossindex, int charindex)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_GUILD_FIRE_OFFICER_NTF
		<< guildindex
		<< bossindex
		<< charindex;
}

void HelperGuildLoadNotifyMsg(CNetMsg::SP& msg, CGuild* guild)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_GUILD_LOAD_NTF
		<< guild->index()
		<< guild->level()
		<< guild->name()
		<< guild->boss()->charindex()
		<< guild->boss()->GetName()
		<< guild->battleIndex()
		<< guild->battlePrize()
		<< guild->battleTime() / 2
		<< guild->battleZone()
		<< guild->killCount()
		<< guild->battleState();
	RefMsg(msg) << guild->maxmember();
#ifdef DEV_GUILD_MARK
	RefMsg(msg) << guild->GetGuildMarkRow()
		<< guild->GetGuildMarkCol()
		<< guild->GetBackgroundRow()
		<< guild->GetBackgroundCol()
		<< guild->GetMarkTime();
#endif
	RefMsg(msg) << guild->getGuildKick()->getKickStatus()
		<< guild->getGuildKick()->getKickRequestChar()
		<< guild->getGuildKick()->getKickRequestTime();
}

void HelperGuildBattleRepMsg(CNetMsg::SP& msg, CGuild* g1, CGuild* g2, int prize)
{
	msg->Init(MSG_HELPER_COMMAND);

	RefMsg(msg) << MSG_HELPER_GUILD_BATTLE_REP
		<< g1->index()
		<< g1->name()
		<< g2->index()
		<< g2->name()
		<< prize
		<< g1->battleZone()
		<< g1->battleTime() / 2;
}

void HelperGuildBattleStartMsg(CNetMsg::SP& msg, CGuild* g1, CGuild* g2)
{
	msg->Init(MSG_HELPER_COMMAND);

	RefMsg(msg) << MSG_HELPER_GUILD_BATTLE_START
		<< g1->index()
		<< g1->name()
		<< g2->index()
		<< g2->name()
		<< g1->battlePrize()
		<< g1->battleZone()
		<< g1->battleTime() / 2;
}

void HelperGuildBattleStopRepMsg(CNetMsg::SP& msg, int winner_index, CGuild* g1, CGuild* g2)
{
	msg->Init(MSG_HELPER_COMMAND);

	RefMsg(msg) << MSG_HELPER_GUILD_BATTLE_STOP_REP
		<< winner_index
		<< g1->index()
		<< g1->name()
		<< g1->killCount()
		<< g2->index()
		<< g2->name()
		<< g2->killCount()
		<< g1->battlePrize()
		<< g1->battleZone();
}

void HelperGuildBattleStatusMsg(CNetMsg::SP& msg, CGuild* g1, CGuild* g2)
{
	msg->Init(MSG_HELPER_COMMAND);

	RefMsg(msg) << MSG_HELPER_GUILD_BATTLE_STATUS
		<< g1->index()
		<< g1->name()
		<< g1->killCount()
		<< g2->index()
		<< g2->name()
		<< g2->killCount()
		<< g2->battleTime() / 2
		<< g2->battleZone();
}

void HelperGuildBattlePeaceRepMsg(CNetMsg::SP& msg, CGuild* g)
{
	msg->Init(MSG_HELPER_COMMAND);

	RefMsg(msg) << MSG_HELPER_GUILD_BATTLE_PEACE_REP
		<< g->index();
}

void HelperEventMoonStoneLoadMsg(CNetMsg::SP& msg, int moonstone)
{
	msg->Init(MSG_HELPER_COMMAND);

	RefMsg(msg) << MSG_HELPER_EVENT_MOONSTONE_LOAD
		<< moonstone;
}

void HelperEventMoonStoneUpdateRepMsg(CNetMsg::SP& msg, int moonstone)
{
	msg->Init(MSG_HELPER_COMMAND);

	RefMsg(msg) << MSG_HELPER_EVENT_MOONSTONE_UPDATE_REP
		<< moonstone;
}

void HelperEventMoonStoneJackPotRepMsg(CNetMsg::SP& msg, int moonstone, int chaindex)
{
	msg->Init(MSG_HELPER_COMMAND);

	RefMsg(msg) << MSG_HELPER_EVENT_MOONSTONE_JACKPOT_REP
		<< moonstone
		<< chaindex;
}

void HelperBlockCharRepMsg(CNetMsg::SP& msg, int reqindex, int blockIndex, const char* blockName )
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_BLOCKPC_REP
		<< reqindex
		<< blockIndex
		<< blockName;
}

void HelperGiftCharRepMsg(CNetMsg::SP& msg, int sendUserIdx, int sendCharIdx, int recvUserIdx, int recvCharIdx, const char* recvCharName, const char* sendMsg, int count, int idx[], int ctid[] )
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_GIFT_RECVCHARNAME_REP
		<< sendUserIdx
		<< sendCharIdx
		<< recvUserIdx
		<< recvCharIdx
		<< recvCharName
		<< sendMsg
		<< count;

	for(int i = 0; i < count; i++)
	{
		RefMsg(msg) << idx[i]
			<< ctid[i];
	}
}

void HelperFriendSetConditionNotifyMsg(CNetMsg::SP& msg, int chaindex, int condition, int reply, int sum, const int* index)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_FRIEND_SET_CONDITION_NOTIFY
		<< chaindex
		<< condition
		<< reply;

	if(reply == -1)
	{
		RefMsg(msg) << sum;
		for(int i=0 ; i<sum ; i++)
		{
			RefMsg(msg) << index[i];
		}
	}
}

void HelperNameChangeRepMsg(CNetMsg::SP& msg, char bguild, int charindex, const char* name, MSG_EX_NAMECHANGE_ERROR_TYPE error)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_NAME_CHANGE_REP
		<< bguild
		<< charindex
		<< name
		<< (unsigned char) error;
}

void HelperPetCreateRepMsg(CNetMsg::SP& msg, int index, int owner, char typegrade)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_PET_CREATE_REP
		<< index
		<< owner
		<< typegrade;
}

void HelperPetDeleteRepMsg(CNetMsg::SP& msg, int index, int owner)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_PET_DELETE_REP
		<< index
		<< owner;
}

void HelperGuildStashHistoryRepMsg(CNetMsg::SP& msg, int charindex, MSG_HELPER_GUILD_STASH_ERROR_TYPE errcode, int month[7], int day[7], LONGLONG money[7])
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_GUILD_STASH_HISTORY_REP
		<< charindex
		<< errcode;
	int i;
	for (i = 0; i < 7; i++)
	{
		RefMsg(msg) << month[i]
			<< day[i]
			<< money[i];
	}
}

void HelperGuildStashViewRepMsg(CNetMsg::SP& msg, int charindex, MSG_HELPER_GUILD_STASH_ERROR_TYPE errcode, LONGLONG money)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_GUILD_STASH_VIEW_REP
		<< charindex
		<< errcode
		<< money;
}

void HelperGuildStashTakeRepMsg(CNetMsg::SP& msg, int guildindex, int charindex, MSG_HELPER_GUILD_STASH_ERROR_TYPE errcode, LONGLONG money, LONGLONG balance)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_GUILD_STASH_TAKE_REP
		<< guildindex
		<< charindex
		<< errcode
		<< money
		<< balance;
}

void HelperGuildStashSaveTaxRepMsg(CNetMsg::SP& msg, MSG_HELPER_GUILD_STASH_ERROR_TYPE errcode, int guildindex, int zoneindex, LONGLONG taxItem, LONGLONG taxProduce)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_GUILD_STASH_SAVE_TAX_REP
		<< errcode
		<< guildindex
		<< zoneindex
		<< taxItem
		<< taxProduce;
}

void HelperFameupRepMsg(CNetMsg::SP& msg, int teachidx, int studentidx, const char* studentname, int fameup)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_TEACHER_FAMEUP_REP
		<< teachidx
		<< fameup;
}

void HelperTeacherInfo(CNetMsg::SP& rmsg, const char* idname, int charindex, char m_teachType, int m_fame, char bteacher, int m_cntCompleteStudent,
		int m_cntFailStudent, int m_superstone, int m_guardian, const char* strTeachIdx, const char* strTeachSec, int msgSubType)
{
	int m_teachIdx[TEACH_MAX_STUDENTS];
	char		m_teachJob[TEACH_MAX_STUDENTS];
	char		m_teachJob2[TEACH_MAX_STUDENTS];
	int		m_teachLevel[TEACH_MAX_STUDENTS];
	char	m_teachName[TEACH_MAX_STUDENTS][MAX_CHAR_NAME_LENGTH + 1];
	int		m_teachTime[TEACH_MAX_STUDENTS];
	int		m_cntTeachingStudent = 0;
	char	tbuf[255] ={0,};

	memset(m_teachIdx, -1, sizeof(m_teachIdx) );
	memset(m_teachTime, 0, sizeof(m_teachTime) );

	if( m_teachType == MSG_TEACH_TEACHER_TYPE)
	{
		//strTeachIdx[0] = '\0';
		const char* pTeach = strTeachIdx;

		int cnt =0;
		int count = 0;
		while (*pTeach)
		{
			int idx;
			pTeach = AnyOneArg(pTeach, tbuf);
			idx = atoi(tbuf);

			if (idx == -1)
			{
				m_teachIdx[cnt] = -1;
				m_teachJob[cnt] = -1;
				m_teachJob2[cnt] = 0;
				m_teachLevel[cnt] = 0;
				m_teachName[cnt][0] = '\0';
				m_teachTime[cnt] = 0;
			}
			else
			{
				CDBCmd dbTmp;
				dbTmp.Init(&gserver.m_dbchar);

				CLCString sql(1024);

				sql.Format("SELECT a_index, a_name, a_nick, a_level, a_job, a_job2 FROM t_characters WHERE a_enable = 1 and a_index=%d", idx);

				dbTmp.SetQuery(sql);
				if (dbTmp.Open() && dbTmp.MoveFirst())
				{
					int teach_index;
					int teach_level;
					CLCString teach_name(MAX_CHAR_NAME_LENGTH + 1);
					CLCString teach_nick(MAX_CHAR_NAME_LENGTH + 1);
					char teach_job;
					char teach_job2;

					dbTmp.GetRec("a_index",		teach_index);
					dbTmp.GetRec("a_name",		teach_name);
					dbTmp.GetRec("a_nick",		teach_nick);
					dbTmp.GetRec("a_level",		teach_level);
					dbTmp.GetRec("a_job",		teach_job);
					dbTmp.GetRec("a_job2",		teach_job2);
					m_teachIdx[cnt] = teach_index;
					m_teachJob[cnt] = teach_job;
					m_teachJob2[cnt] = teach_job2;
					m_teachLevel[cnt] = teach_level;
					strcpy(m_teachName[cnt], teach_nick);
					count++;
				}
				else
				{
					// 학생 케릭이 삭제되었을 경우. 캔슬
					CDBCmd tempCmd;
					tempCmd.Init(&gserver.m_dbchar);

					tempCmd.BeginTrans();
					do_TeachCancel(tempCmd, charindex, idx, CANCELTEACHER);
					tempCmd.Commit();
				}
			}

			cnt++;
		}

		m_cntTeachingStudent = count;

		//strTeachSec[0] = '\0';
		const char* pTeachSec = strTeachSec;
		cnt = 0;
		while (*pTeachSec)
		{
			int sec;
			pTeachSec = AnyOneArg(pTeachSec, tbuf);
			sec = atoi(tbuf);
			m_teachTime[cnt++] = sec;
			if( cnt >= 2 )
				break;
		}

		// 사제 시스템을 읽고 난후 메세지를 보낸다.
		rmsg->Init(MSG_HELPER_COMMAND);
		RefMsg(rmsg) << msgSubType
			 << idname
			 << charindex
			 << m_cntTeachingStudent
			 << m_guardian
			 << m_superstone
			 << m_cntFailStudent
			 << m_cntCompleteStudent
			 << m_teachType
			 << m_fame
			 << bteacher;
		RefMsg(rmsg) << m_teachTime[0]
			 << m_teachTime[1];

		for(int i = 0; i < TEACH_MAX_STUDENTS; i++)
		{
			RefMsg(rmsg) << m_teachIdx[i]
				 << m_teachJob[i]
				 << m_teachJob2[i]
				 << m_teachLevel[i]
				 << m_teachName[i];
		}
	}
	else
	{
		const char* pTeach = strTeachIdx;

		int idx;
		pTeach = AnyOneArg(pTeach, tbuf);
		idx = atoi(tbuf);

		CDBCmd dbTmp;
		dbTmp.Init(&gserver.m_dbchar);

		CLCString sql(1024);

		sql.Format("SELECT a_index, a_name, a_nick, a_level, a_job, a_job2, a_teach_idx, a_fame, a_teach_complete, a_teach_fail "
			"FROM t_characters WHERE a_enable  = 1 and a_index=%d", idx);

		dbTmp.SetQuery(sql);
		if (dbTmp.Open() && dbTmp.MoveFirst())
		{
			CLCString teachName(MAX_CHAR_NAME_LENGTH + 1);
			CLCString teachIdx(256);
			dbTmp.GetRec("a_index", m_teachIdx[0]);
			//dbTmp.GetRec("a_name", m_teachName[0]);
			dbTmp.GetRec("a_nick", teachName);
			strcpy(m_teachName[0], teachName);
			dbTmp.GetRec("a_level", m_teachLevel[0]);
			dbTmp.GetRec("a_job", m_teachJob[0]);
			dbTmp.GetRec("a_job2", m_teachJob2[0]);
			// 자신이 학생이면 idx[1]을 선생의 명성수치로, idx[2]를 선생의 양성중이 초보로, time[1]을 완료인원, time[2]을 실패인원으로 쓴다.
			dbTmp.GetRec("a_teach_idx", teachIdx);
			dbTmp.GetRec("a_fame", m_teachIdx[1]);
			dbTmp.GetRec("a_teach_complete", m_teachTime[1]);
			dbTmp.GetRec("a_teach_fail", m_teachTime[2]);

			const char* pTemp = teachIdx;

			m_teachIdx[2] = 0;
			while(*pTemp)
			{
				int idx;
				pTemp = AnyOneArg(pTemp, tbuf);
				idx = atoi(tbuf);

				if( idx != -1 )
				{
					m_teachIdx[2]++;
				}
			}
		}
		
		// 사제 성립 시간을 견습생은 포기 횟수로 사용한다.
		const char* pTeachSec = strTeachSec;

		int sec;
		pTeachSec = AnyOneArg(pTeachSec, tbuf);
		sec = atoi(tbuf);
		m_teachTime[0] = sec;
		rmsg->Init(MSG_HELPER_COMMAND);

		RefMsg(rmsg) << MSG_HELPER_TEACHER_LOAD_REP
			 << idname
			 << charindex
			 << m_cntTeachingStudent
			 << m_guardian
			 << m_superstone
			 << m_cntFailStudent
			 << m_cntCompleteStudent
			 << m_teachType
			 << m_fame
			 << bteacher;
		if( m_teachType == MSG_TEACH_STUDENT_TYPE)

		{
			RefMsg(rmsg) << m_teachIdx[0]
				 << m_teachJob[0]
				 << m_teachJob2[0]
				 << m_teachLevel[0]

			 	 << m_teachName[0]
				// 사제 성립 시간을 견습생은 포기 횟수로 사용한다.
				 << m_teachTime[0];

			// 자신이 학생이면 idx[1]을 선생의 명성수치로, idx[2]를 선생의 양성중이 초보로, time[1]을 완료인원, time[2]을 실패인원으로 쓴다.
			RefMsg(rmsg) << m_teachIdx[1]
				 << m_teachIdx[2]
				 << m_teachTime[1]
				 << m_teachTime[2];
		}
		else
		{
			RefMsg(rmsg) << m_teachTime[0];
		}
	}
}

void HelperTeachGiftRepMsg( CNetMsg::SP& msg, MSG_HELPER_TEACH_RENEWER_GIFT_ERROR_TYPE type, int charindex, int count)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_TEACHER_SYSTEM_RENEWER
		<< (unsigned char) MSG_HELPER_TEACH_RENEWER_TEACHER_GIFT_REP
		<< (unsigned char) type
		<< charindex;
	if( count != -1 )
		RefMsg(msg) << count;
}

//휴면케릭 응답 보내는 부분.
void HelperEventSearchFriendMemberAddRepMsg(CNetMsg::SP& msg
			, int approvalindex,
			const char reqnick[MAX_CHAR_NAME_LENGTH + 1], MSG_EVENT_SEARCHFRIEND_ERROR_TYPE errcode)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_SEARCHFRIEND_ADD_REP
		<< approvalindex
		<< reqnick
		<< (unsigned char)errcode;
}

//휴면케릭이 리스트에서 선택한 결과를 보내는 부분
void HelperEventSearchFriendMemberSelectAddRepMsg(CNetMsg::SP& msg
			, int approvalindex, MSG_EVENT_SEARCHFRIEND_ERROR_TYPE errcode)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_SEARCHFRIEND_SELECT_ADD_REP
		<< approvalindex
		<< (unsigned char)errcode;
}

//휴면케릭이 한시간 단위로 사냥한 시간을 저장하고 상태를 GameServer로 보냄.
void HelperEventSearchFriendMemberOneTimeCheckRepMsg(CNetMsg::SP& msg
			, int timesec, int appDormantindex, MSG_EVENT_SEARCHFRIEND_ERROR_TYPE errcode)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_SEARCHFRIEND_ONETIME_ADD_REP
		<< timesec
		<< appDormantindex
		<< (unsigned char)errcode;
}

//휴면케릭 이벤트 등록된 친구 보상가능 여부 데이터를 GameServer로 보냄.
void HelperEventSearchFriendMemberListGoodRepMsg(CNetMsg::SP& msg, int approvalindex, int* requestindex, const char requestnick[][MAX_CHAR_NAME_LENGTH + 1], int* itemgood, int request_listmember, int startindex, int nTotal)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_SEARCHFRIEND_LISTGOOD_REP
		<< approvalindex
		<< request_listmember
		<< startindex
		<< nTotal;

	for (int i = 0; i < request_listmember; i++)
	{
		RefMsg(msg) << requestindex[i]
			<< requestnick[i]
			<< itemgood[i];
	}
}

//휴면케릭 이벤트 등록된 친구 보상 결과 데이터를 GameServer로 보냄.
void HelperEventSearchFriendMemberGoodRepMsg(CNetMsg::SP& msg, int approvalindex, MSG_EVENT_SEARCHFRIEND_GOODS_ERROR_TYPE errcode)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_SEARCHFRIEND_GOOD_REP
		<< approvalindex
		<< (unsigned char)errcode;
}

void HelperPartyInviteReqMsg(CNetMsg::SP& msg, int nBossIndex, const char* strBossName, int nBossLevel, int nTargetIndex, char nPartyType)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_PARTY_INVITE_REQ
		<< nBossIndex
		<< strBossName
		<< nBossLevel
		<< nTargetIndex
		<< nPartyType;
}

void HelperPartyInviteRepMsg(CNetMsg::SP& msg, int nBossIndex, const char* strBossName, int nTargetIndex, const char* strTargetName, char cPartyType1, char cPartyType2, char cPartyType3, MSG_HELPER_PARTY_ERROR_TYPE errcode)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_PARTY_INVITE_REP
		<< nBossIndex
		<< strBossName
		<< nTargetIndex
		<< strTargetName
		<< cPartyType1
		<< cPartyType2
		<< cPartyType3
		<< errcode;
}

void HelperPartyAllowRepMsg(CNetMsg::SP& msg, int nBossIndex, int nTargetIndex, const char* strTargetName, MSG_HELPER_PARTY_ERROR_TYPE errcode)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_PARTY_ALLOW_REP
		<< nBossIndex
		<< nTargetIndex
		<< strTargetName
		<< errcode;
}

void HelperPartyRejectRepMsg(CNetMsg::SP& msg, int nBossIndex, int nTargetIndex, MSG_HELPER_PARTY_ERROR_TYPE errcode)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_PARTY_REJECT_REP
		<< nBossIndex
		<< nTargetIndex
		<< errcode;
}

void HelperPartyQuitRepMsg(CNetMsg::SP& msg, int nBossIndex, int nTargetIndex, MSG_HELPER_PARTY_ERROR_TYPE errcode, int nNewBossIndex, const char* strNewBossName)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_PARTY_QUIT_REP
		<< nBossIndex
		<< nTargetIndex
		<< errcode
		<< nNewBossIndex
		<< strNewBossName;
}

void HelperPartyKickRepMsg(CNetMsg::SP& msg, int nBossIndex, int nTargetIndex, MSG_HELPER_PARTY_ERROR_TYPE errcode)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_PARTY_KICK_REP
		<< nBossIndex
		<< nTargetIndex
		<< errcode;
}

void HelperPartyChangeBossRepMsg(CNetMsg::SP& msg, int nBossIndex, const char* strBossName, int nNewBossIndex, const char* strNewBossName)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_PARTY_CHANGE_BOSS_REP
		<< nBossIndex
		<< strBossName
		<< nNewBossIndex
		<< strNewBossName;
}

void HelperPartyChangeTypeRepMsg(CNetMsg::SP& msg, int nBossIndex, char cPartyType, char cDiviType, char cAllOneSet)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_PARTY_CHANGE_TYPE_REP
		<< nBossIndex
		<< cPartyType
		<< cDiviType
		<< cAllOneSet;
}
void HelperPartyEndPartyRepMsg(CNetMsg::SP& msg, int nBossIndex)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_PARTY_END_PARTY_REP
		<< nBossIndex;
}

void HelperPartyMatchRegMemberRepMsg(CNetMsg::SP& msg, MSG_HELPER_PARTY_MATCH_ERROR_TYPE nErrorCode, int nCharIndex, const char* strCharName, int nCharLevel, int nZone, char cJob, char cPartyType)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_PARTY_MATCH_REG_MEMBER_REP
		<< nErrorCode
		<< nCharIndex
		<< strCharName
		<< nCharLevel
		<< nZone
		<< cJob
		<< cPartyType;
}

void HelperPartyMatchRegPartyRepMsg(CNetMsg::SP& msg, MSG_HELPER_PARTY_MATCH_ERROR_TYPE nErrorCode, int nBossIndex, const char* strBossName, int nBossLevel, int nZone, int nMemberCount, int nJobFlag, char cLimitLevel, char cPartyType, const char* strComment)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_PARTY_MATCH_REG_PARTY_REP
		<< nErrorCode
		<< nBossIndex
		<< strBossName
		<< nBossLevel
		<< nZone
		<< nMemberCount
		<< nJobFlag
		<< cLimitLevel
		<< cPartyType;
	if (nErrorCode == MSG_HELPER_PARTY_MATCH_ERROR_REG_PARTY_OK)
		RefMsg(msg) << strComment;
	else
		RefMsg(msg) << "";
}

void HelperPartyMatchDelRepMsg(CNetMsg::SP& msg, int nCharIndex, MSG_HELPER_PARTY_MATCH_ERROR_TYPE nErrorCode)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_PARTY_MATCH_DEL_REP
		<< nCharIndex
		<< nErrorCode;
}

void HelperPartyMatchInviteRepMsg(CNetMsg::SP& msg, MSG_HELPER_PARTY_MATCH_ERROR_TYPE nErrorCode, int nBossIndex, const char* strBossName, int nCharIndex, const char* strCharName, char cPartyType)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_PARTY_MATCH_INVITE_REP
		<< nErrorCode
		<< nBossIndex
		<< strBossName
		<< nCharIndex
		<< strCharName
		<< cPartyType;
}

void HelperPartyMatchJoinRepMsg(CNetMsg::SP& msg, MSG_HELPER_PARTY_MATCH_ERROR_TYPE nErrorCode, char cPartyType, int nBossIndex, const char* strBossName, int nCharIndex, const char* strCharName, char cCharJob)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_PARTY_MATCH_JOIN_REP
		<< nErrorCode
		<< cPartyType
		<< nBossIndex
		<< strBossName
		<< nCharIndex
		<< strCharName
		<< cCharJob;
}

void HelperPartyMatchJoinAllowRepMsg(CNetMsg::SP& msg, MSG_HELPER_PARTY_MATCH_ERROR_TYPE nErrorCode, int nBossIndex, int nCharIndex, const char* strCharName)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_PARTY_MATCH_JOIN_ALLOW_REP
		<< nErrorCode
		<< nBossIndex
		<< nCharIndex
		<< strCharName;
}

void HelperPartyMatchJoinRejectRepMsg(CNetMsg::SP& msg, MSG_HELPER_PARTY_MATCH_ERROR_TYPE nErrorCode, int nJoinCharIndex, int nRejectCharIndex)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_PARTY_MATCH_JOIN_REJECT_REP
		<< nErrorCode
		<< nJoinCharIndex
		<< nRejectCharIndex;
}

void HelperPartyMatchMemberChangeInfoMsg(CNetMsg::SP& msg, int nCharIndex, MSG_HELPER_PARTY_MATCH_MEMBER_CHANGE_INFO_TYPE nType, const char* strCharName, int nLevel, int nZone)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_PARTY_MATCH_MEMBER_CHANGE_INFO
		<< nCharIndex
		<< nType;

	switch (nType)
	{
	case MSG_HELPER_PARTY_MATCH_MEMBER_CHANGE_INFO_NAME:
		RefMsg(msg) << strCharName;
		break;

	case MSG_HELPER_PARTY_MATCH_MEMBER_CHANGE_INFO_LEVEL:
		RefMsg(msg) << nLevel;
		break;

	case MSG_HELPER_PARTY_MATCH_MEMBER_CHANGE_INFO_ZONE:
		RefMsg(msg) << nLevel
			<< nZone;
		break;
	}
}

void HelperPartyInfoEndMsg(CNetMsg::SP& msg)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_PARTY_INFO_END;
}

void HelperGuildInclineEstablishRepMsg( CNetMsg::SP& msg, int guildindex, int charindex, char guildincline, MSG_GUILD_ERROR_TYPE errcode )
{
	msg->Init( MSG_HELPER_COMMAND );
	RefMsg(msg) << MSG_HELPER_GUILD_INCLINE_ESTABLISH_REP
		<< guildindex
		<< charindex
		<< guildincline
		<< errcode;
}

void HelperGuildMemberAdjustRepMsg( CNetMsg::SP& msg, int guildindex, int ownerindex, int charindex, const char* strPositionName, int contributeExp, int contributeFame, int pos, MSG_GUILD_ERROR_TYPE errcode )
{
	msg->Init( MSG_HELPER_COMMAND );
	RefMsg(msg) << MSG_HELPER_GUILD_MEMBER_ADJUST_REP
		<< guildindex
		<< ownerindex
		<< charindex
		<< strPositionName
		<< contributeExp
		<< contributeFame
		<< errcode;
	RefMsg(msg) << pos;
}

void HelperNewGuildInfoRepMsg( CNetMsg::SP& msg, int charindex, MSG_GUILD_ERROR_TYPE errorcode )
{
	msg->Init( MSG_HELPER_COMMAND );
	RefMsg(msg) << MSG_HELPER_NEW_GUILD_INFO_REP
		<< charindex
		<< errorcode;
}
void HelperNewGuildInfoNotifyMsg( CNetMsg::SP& msg, int charindex, int guildindex, int avelevel, int usepoint, MSG_GUILD_ERROR_TYPE errorcode  )
{
	msg->Init( MSG_HELPER_COMMAND );
	RefMsg(msg) << MSG_HELPER_NEW_GUILD_INFO_NTF
		<< charindex
		<< guildindex
		<< avelevel
		<< usepoint
		<< errorcode;
}
void HelperNewGuildMemberListRepMsg( CNetMsg::SP& msg, int endcount, int & guildstart, int charindex, int guildindex, MSG_GUILD_ERROR_TYPE errcode,  int* membercharindex, int* cumulatePoint, const char CharName[][MAX_CHAR_NAME_LENGTH + 1], const char positionName[][GUILD_POSITION_NAME+1], char* job, char* job2, int* level, int* position )
{
	msg->Init( MSG_HELPER_COMMAND );
	RefMsg(msg) << MSG_HELPER_NEW_GUILD_MEMBERLIST_REP
		<< charindex
		<< guildindex
		<< errcode
		<< endcount - guildstart;

	for( ; guildstart < endcount; guildstart++ )
	{
		RefMsg(msg) << membercharindex[guildstart]
			<< cumulatePoint[guildstart]
			<< CharName[guildstart]
			<< positionName[guildstart]
			<< job[guildstart]
			<< job2[guildstart]
			<< level[guildstart]
			<< position[guildstart];
	}
}
#ifdef DEV_GUILD_STASH
void HelperNewGuildManageRepMsg( CNetMsg::SP& msg, int endcount, int & guildstart, int charindex, int guildindex, MSG_GUILD_ERROR_TYPE errcode,  int* membercharindex, int* contributeExp, int* contributeFame, const char CharName[][MAX_CHAR_NAME_LENGTH + 1], const char positionName[][GUILD_POSITION_NAME+1], int* level, int* position, char* stashAuth, char first)
#else
void HelperNewGuildManageRepMsg( CNetMsg::SP& msg, int endcount, int & guildstart, int charindex, int guildindex, MSG_GUILD_ERROR_TYPE errcode,  int* membercharindex, int* contributeExp, int* contributeFame, const char CharName[][MAX_CHAR_NAME_LENGTH + 1], const char positionName[][GUILD_POSITION_NAME+1], int* level, int* position, char first)
#endif //DEV_GUILD_STASH
{
	msg->Init( MSG_HELPER_COMMAND );
	RefMsg(msg) << MSG_HELPER_NEW_GUILD_MANAGE_REP
		<< first
		<< charindex
		<< guildindex
		<< errcode
		<< endcount - guildstart;

	for( ; guildstart < endcount; guildstart++ )
	{
		RefMsg(msg) << membercharindex[guildstart]
			<< contributeExp[guildstart]
			<< contributeFame[guildstart]
			<< CharName[guildstart]
			<< positionName[guildstart]
			<< level[guildstart]
			<< position[guildstart];
#ifdef DEV_GUILD_STASH
		RefMsg(msg) << stashAuth[guildstart];
#endif //DEV_GUILD_STASH
	}
}

void HelperNewGuildNoticeRepMsg( CNetMsg::SP& msg, int charindex, int guildindex, const char* title, const char* text )
{
	msg->Init( MSG_HELPER_COMMAND );
	RefMsg(msg) << MSG_HELPER_NEW_GUILD_NOTICE_REP
		<< charindex
		<< guildindex;
	if( title != NULL && text != NULL )
	{
		RefMsg(msg) << title
			<< text;
	}
}
void HelperNewGuildNoticeUpdateRepMsg( CNetMsg::SP& msg, int charindex, int guildindex, MSG_GUILD_ERROR_TYPE errcode )
{
	msg->Init( MSG_HELPER_COMMAND );
	RefMsg(msg) << MSG_HELPER_NEW_GUILD_NOTICE_UPDATE_REP
		<< charindex
		<< guildindex
		<< errcode;
}

void HelperNewGuildNoticeTransMsg( CNetMsg::SP& msg, int guildindex, const char* title, const char* text )
{
	msg->Init( MSG_HELPER_COMMAND );
	RefMsg(msg) << MSG_HELPER_NEW_GUILD_NOTICE_TRANSMISSION_NTF
		<< guildindex
		<< title
		<< text;
}

void HelperNewGuildNoticeTransRep( CNetMsg::SP& msg, int guildindex, int charindex, MSG_GUILD_ERROR_TYPE errcode )
{
	msg->Init( MSG_HELPER_COMMAND );
	RefMsg(msg) << MSG_HELPER_NEW_GUILD_NOTICE_TRANSMISSION_REP
		<< guildindex
		<< charindex
		<< errcode;
}

void HelperNewGuildSkillListRepMsg( CNetMsg::SP& msg, int charindex, int guildindex, MSG_GUILD_ERROR_TYPE errcode, int Activeskillcount, int* Activeskillindex, int* ActiveskillLevel, int Passiveskillcount, int* Passiveskillindex, int* PassiveskillLevel, int Etcskillcount, int* Etcskillindex, int* EtcskillLevel )
{
	msg->Init( MSG_HELPER_COMMAND );
	RefMsg(msg) << MSG_HELPER_NEW_GUILD_NOTICE_SKILLLIST_REP
		<< charindex
		<< guildindex
		<< errcode;

	int i;
	RefMsg(msg) << GUILD_ACTIVE_SKILL_START					// Active Skill 시작
		<< Activeskillcount;
	for(i = 0; i < Activeskillcount; i++)
	{
		RefMsg(msg) << Activeskillindex[i]
			<< ActiveskillLevel[i];
	}

	RefMsg(msg) << GUILD_PASSIVE_SKILL_START				// PassiveSkill 시작
		<< Passiveskillcount;
	for(i = 0; i < Passiveskillcount; i++)
	{
		RefMsg(msg) << Passiveskillindex[i]
			<< PassiveskillLevel[i];
	}

	RefMsg(msg) << GUILD_ETC_SKILL_START					// EtcSkill 시작
		<< Etcskillcount;
	for(i = 0; i < Etcskillcount; i++)
	{
		RefMsg(msg) << Etcskillindex[i]
			<< EtcskillLevel[i];
	}
}

void HelperExtendGuildLoadNotifyMsg( CNetMsg::SP& msg, CGuild* guild, int skillcount, int* skillindex, int* skillLevel, int skilltype )
{
	msg->Init( MSG_HELPER_COMMAND );
	RefMsg(msg) << MSG_HELPER_NEW_GUILD_LOAD_NTF
		<< guild->index()
		<< guild->GetGuildPoint()
		<< guild->GetIncline()
		<< guild->maxmember()
		<< guild->GetLandCount();
	int landcount = guild->GetLandCount();
	if( landcount == 0 )
	{
		RefMsg(msg) << -1;
	}
	else
	{
		for( int i = 0; i < landcount; i++ )
		{
			int* land = guild->GetLand();
			if( land[i] != -1 )
			{
				RefMsg(msg) << land[i];
			}
			else
				continue;
		}
	}
	RefMsg(msg) << skillcount;
	for( int i = 0; i < skillcount; i++ )
	{
		RefMsg(msg) << skillindex[i]
			<< skillLevel[i];
	}
	RefMsg(msg) << skilltype;
}

void HelperNewGuildPointUpdateMsg( CNetMsg::SP& msg, int charindex, int guildindex, int guildpoint )
{
	msg->Init( MSG_HELPER_COMMAND );
	RefMsg(msg) << MSG_HELPER_UPDATE_GUILD_POINT
		<< charindex
		<< guildindex
		<< guildpoint;
}

void HelperNewGuildNotice( CNetMsg::SP& msg, int charindex, int guildindex, const char* title, const char* text )
{
	msg->Init( MSG_HELPER_COMMAND );
	RefMsg(msg) << MSG_HELPER_GUILD_NOTICE
		<< charindex
		<< guildindex;
	if( title != NULL && text != NULL )
	{
		RefMsg(msg) << title
			<< text;
	}
}

void HelperNewGuildMemberPointSaveMsg( CNetMsg::SP& msg, int charindex, int guildindex, int memberpoint )
{
	msg->Init( MSG_HELPER_COMMAND );
	RefMsg(msg) << MSG_HELPER_SAVE_GUILD_MEMBER_POINT
		<< guildindex
		<< charindex
		<< memberpoint;
}

void HelperPetNameChange( CNetMsg::SP& msg, MSG_EX_PET_CHANGE_NAME_ERROR_TYPE err,int charidx, int petidx, const char* strPetName )
{
	msg->Init( MSG_HELPER_COMMAND );
	RefMsg(msg) << MSG_HELPER_PET_NAME_CHANGE
		<< err
		<< charidx
		<< petidx
		<< strPetName;
}

void HelperHalloween2007Msg( CNetMsg::SP& msg, int charIndex, unsigned char error )
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_HALLOWEEN_2007
		<< charIndex
		<< error;
}

void HelperDVDRateChangeMsg( CNetMsg::SP& msg, unsigned char subtype, int nRate )
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_DVD_RATE_CHANGE
		<< subtype
		<< nRate;
}

void HelperAttackPet( CNetMsg::SP& msg, MSG_HELPER_APET_TYPE type, int charIndex )
{
	msg->Init( MSG_HELPER_COMMAND );
	RefMsg(msg) << MSG_HELPER_APET
		<< (unsigned char) type
		<< charIndex;
}

void HelperEventPhoenixMsg(CNetMsg::SP& msg, MSG_HELPER_EVENT_PHOENIX_ERROR_TYPE nError, int nCharIdx )
{
	msg->Init( MSG_HELPER_COMMAND );
	RefMsg(msg) << MSG_HELPER_EVENT_PHOENIX
		<< (unsigned char) nError
		<< nCharIdx;
}


void HelperExpedErrorMsg(CNetMsg::SP& msg, unsigned char errorType, int CharIndex)
{
	msg->Init( MSG_HELPER_COMMAND );
	RefMsg(msg) << (int)MSG_HELPER_EXPED_ERROR
		<< errorType
		<< CharIndex;
}

void HelperExpedCreateRepMsg(CNetMsg::SP& msg, char nExpedType1,char nExpedType2,char nExpedType3, char nExpedType4, char nExpedType5, int nBossIndex,CLCString BossCharName,int nMemberCount,int *pCharIdex,CLCString* pCharName,int* pGroupType,int* pMemberType,int *pSetLabelType,int* pQuitType)
{
	msg->Init( MSG_HELPER_COMMAND );
	RefMsg(msg) << (int)MSG_HELPER_EXPED_CREATE_REP
		<< nExpedType1
		<< nExpedType2
		<< nExpedType3
		<< nExpedType4
		<< nExpedType5
		<< nBossIndex
		<< BossCharName
		<< nMemberCount;

	for(int i=0; i < nMemberCount; i++)
	{
		RefMsg(msg) << pCharIdex[i]
			<< pCharName[i]
			<< pGroupType[i]
			<< pMemberType[i]
			<< pSetLabelType[i]
			<< pQuitType[i];
	}
}

void HelperExpedInviteRepMsg(CNetMsg::SP& msg, int nBossIndex, CLCString strBossName, int nTargetIndex, CLCString strTargetName,char cExpedType1,char cExpedType2,char cExpedType3)
{
	msg->Init( MSG_HELPER_COMMAND );
	RefMsg(msg) << (int)MSG_HELPER_EXPED_INVITE_REP
		<< nBossIndex
		<< strBossName
		<< nTargetIndex
		<< strTargetName
		<< cExpedType1
		<< cExpedType2
		<< cExpedType3;
}

void HelperExpedAllowRepMsg(CNetMsg::SP& msg, int nBossIndex, int nTargetIndex, CLCString strTargetName,int nTargetGroup, int nTargetMember,int nTargetListIndex,int nErrorCode)
{
	msg->Init( MSG_HELPER_COMMAND );
	RefMsg(msg) << (int)MSG_HELPER_EXPED_ALLOW_REP
		<< nBossIndex
		<< nTargetIndex
		<< strTargetName
		<< nTargetGroup
		<< nTargetMember
		<< nTargetListIndex
		<< nErrorCode;
}

void HelperExpedRejectRepMsg(CNetMsg::SP& msg, int nBossIndex, int nTargetIndex,int nErrorCode)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << (int)MSG_HELPER_EXPED_REJECT_REP
		<< nBossIndex
		<< nTargetIndex
		<< nErrorCode;
}

void HelperExpedQuitRepMsg(CNetMsg::SP& msg, int nBossIndex, int nTargetIndex, int nQuitMode, int nErrorCode)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << (int)MSG_HELPER_EXPED_QUIT_REP
		<< nBossIndex
		<< nTargetIndex
		<< nQuitMode
		<< nErrorCode;
}

void HelperExpedKickRepMsg(CNetMsg::SP& msg, int nBossIndex, int nTargetIndex, int errcode)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << (int)MSG_HELPER_EXPED_KICK_REP
		<< nBossIndex
		<< nTargetIndex
		<< errcode;
}

void HelperExpedChangeBossRepMsg(CNetMsg::SP& msg, int nBossIndex,  int nNewBossIndex, int nChangeMode)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << (int)MSG_HELPER_EXPED_CHANGEBOSS_REP
		<< nBossIndex
		<< nNewBossIndex
		<< nChangeMode;
}
void HelperExpedChangeTypeRepMsg(CNetMsg::SP& msg, int nBossIndex, char cExpedType, char cDiviType)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << (int)MSG_HELPER_EXPED_CHANGETYPE_REP
		<< nBossIndex
		<< cExpedType
		<< cDiviType;
}

void HelperExpedEndExpedRepMsg(CNetMsg::SP& msg, int nBossIndex)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << (int)MSG_HELPER_EXPED_ENDEXPED_REP
		<< nBossIndex;
}

void HelperExpedSetMBossRepMsg(CNetMsg::SP& msg, int nBossIndex, int nNewMBossIndex)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << (int)MSG_HELPER_EXPED_SETMBOSS_REP
		<< nBossIndex
		<< nNewMBossIndex;
}

void HelperExpedResetMBossRepMsg(CNetMsg::SP& msg, int nBossIndex, int nNewMBossIndex)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << (int)MSG_HELPER_EXPED_RESETMBOSS_REP
		<< nBossIndex
		<< nNewMBossIndex;
}

void HelperExpedMoveGroupRepMsg(CNetMsg::SP& msg,int nBossIndex,int nSourceGroup, int nMoveCharIndex, int nTargetGroup,int nTargetListindex)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << (int)MSG_HELPER_EXPED_MOVEGROUP_REP
		<< nBossIndex
		<< nSourceGroup
		<< nMoveCharIndex
		<< nTargetGroup
		<< nTargetListindex;
}

void HelperRaidInzoneGetRoomNoRep(CNetMsg::SP& msg, int nCharIndex, int nZoneNo,int nRoomNo, int nBossIndex,int nBossRoomNo)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_INZONE_GET_ROOMNO_REP
		<< nCharIndex
		<< nZoneNo
		<< nRoomNo
		<< nBossIndex
		<< nBossRoomNo;
}

void HelperDeleteRaidCharacterRep(CNetMsg::SP& msg, int nCharIndex, int nSuccess)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_DELETE_RAID_CHAR
		<< nCharIndex
		<< nSuccess;
}

void HelperRaidInfoRep(CNetMsg::SP& msg, int nCharIndex, int nRaidCount, int* nZoneNum, int* nRoomNum)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_RAID_INFO
		<< nCharIndex
		<< nRaidCount;

	for (int i=0; i < nRaidCount; i++)
	{
		RefMsg(msg) << nZoneNum[i]
			<< nRoomNum[i];
	}
}

void HelperNSCreateCardErrorMsg(CNetMsg::SP& msg, MSG_HELPER_NS_CARD_TYPE subtype, int userindex, int charindex)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_NS_CARD
		<< (unsigned char) subtype
		<< userindex
		<< charindex;
}

#if defined(EVENT_WORLDCUP_2010) || defined(EVENT_WORLDCUP_2010_TOTO) || defined(EVENT_WORLDCUP_2010_TOTO_STATUS) || defined(EVENT_WORLDCUP_2010_TOTO_GIFT) || defined(EVENT_WORLDCUP_2010_ATTENDANCE) || defined(EVENT_WORLDCUP_2010_KOREA)
void HelperWorldcup2010TOTOErrorMsg(CNetMsg::SP& msg, MSG_HELPER_WORLDCUP2010_ERROR_TYPE subType, int charIndex, int itemidx)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_EVENT_WORLDCUP2010
		<< (unsigned char) MSG_HELPER_WORLDCUP2010_TOTO_REP
		<< (unsigned char) subType
		<< charIndex
		<< itemidx;
}
void HelperWorldcup2010TOTOStatusErrorMsg(CNetMsg::SP& msg, MSG_HELPER_WORLDCUP2010_ERROR_TYPE subType, int charIndex, int resultType, int intemIndex)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_EVENT_WORLDCUP2010
		<< (unsigned char) MSG_HELPER_WORLDCUP2010_TOTO_STATUS_REP
		<< (unsigned char) subType
		<< charIndex
		<< resultType
		<< intemIndex;
}
void HelperWorldcup2010TOTOGiftErrorMsg(CNetMsg::SP& msg, MSG_HELPER_WORLDCUP2010_ERROR_TYPE subType,  int charIndex)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_EVENT_WORLDCUP2010
		<< (unsigned char) MSG_HELPER_WORLDCUP2010_TOTO_GIFT_REP
		<< (unsigned char) subType
		<< charIndex;
}
void HelperWorldcup2010AttendanceErrorMsg(CNetMsg::SP& msg, MSG_HELPER_WORLDCUP2010_ERROR_TYPE subType,  int charIndex)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_EVENT_WORLDCUP2010
		<< (unsigned char) MSG_HELPER_WORLDCUP2010_ATTENDANCE_REP
		<< (unsigned char) subType
		<< charIndex;
}

void HelperWorldcup2010KoreaErrorMsg(CNetMsg::SP& msg, MSG_HELPER_WORLDCUP2010_ERROR_TYPE subType, int charIndex)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_EVENT_WORLDCUP2010
		<< (unsigned char) MSG_HELPER_WORLDCUP2010_KOREA_REP
		<< (unsigned char) subType
		<< charIndex;
}
#endif // defined(EVENT_WORLDCUP_2010) || defined(EVENT_WORLDCUP_2010_TOTO) || defined(EVENT_WORLDCUP_2010_TOTO_GIFT) || defined(EVENT_WORLDCUP_2010_ATTENDANCE) || defined(EVENT_WORLDCUP_2010_KOREA)

#ifdef EXTREME_CUBE_VER2
void HelperCubeRewardGuildPointRepMsg(CNetMsg::SP& msg, int guildindex, char rank, int guildpoint)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_CUBEREWARD_GUILDPOINT_REP
		<< guildindex
		<< rank
		<< guildpoint;
}

void HelperCubeRewardPersonalRepMsg(CNetMsg::SP& msg, int updatetime,int charindex, char rank)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_CUBEREWARD_PERSONAL_REP
		<< updatetime
		<< charindex
		<< rank;
}

void HelperExtremeCubeErrorMsg(CNetMsg::SP& msg, HELPER_EXTREME_CUBE_ERROR_TYPE errortype, int charindex)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_EXTREME_CUBE_ERROR
		<< (char)errortype;
}
#endif // EXTREME_CUBE_VER2

#ifdef EVENT_CHAR_BIRTHDAY
void HelperEventCharBirthdayGiftRepMsg(CNetMsg::SP& msg, MSG_EVENT_CHAR_BIRTHDAY_ERROR_TYPE errortype, int charindex, int years)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_CHAR_BIRTHDAY
		<< (unsigned char) MSG_EVENT_CHAR_BIRTHDAY_GIFT_REP
		<< charindex
		<< (unsigned char)errortype;
	if( errortype == MSG_EVENT_CHAR_BIRTHDAY_ERROR_SUC )
	{
		RefMsg(msg) << years;
	}
}

void HelperEventCharBirthdayBDayRepMsg(CNetMsg::SP& msg, int charindex, int year, char month, char day)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_CHAR_BIRTHDAY
		<< (unsigned char) MSG_EVENT_CHAR_BIRTHDAY_BDAY_REP
		<< charindex
		<< year
		<< month
		<< day;
}
#endif
#ifdef DEV_GUILD_MARK
void HelperGuildMarkRegistRepMsg(CNetMsg::SP& msg, int GuildIndex, int CharIndex, char command, char gm_row, char gm_col, char bg_row, char bg_col, unsigned short tab, unsigned short invenIndex, int markTime, const char* serial)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_NEW_GUILD_MARK_REGIST
		<< GuildIndex
		<< CharIndex
		<< command
		<< gm_row
		<< gm_col
		<< bg_row
		<< bg_col
		<< tab
		<< invenIndex
		<< markTime
		<< serial;
}
void HelperGuildMarkExpireMsg(CNetMsg::SP& msg, unsigned char command, int GuildIndex)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_NEW_GUILD_MARK_EXPIRE
		<< command
		<< GuildIndex;
}
#endif

#ifdef DEV_GUILD_STASH
void HelperGuildStashListMsg( CNetMsg::SP& msg , int charIndex )
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_GUILD_STASH_LIST
		<< charIndex;
}

void HelperGuildStashKeepMsg( CNetMsg::SP& msg , int charIndex, int guildIndex, char error, LONGLONG keeyMoney, int itemCount )
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_GUILD_STASH_KEEP
		<< charIndex
		<< guildIndex
		<< error
		<< keeyMoney
		<< itemCount;
}

void HelperGuildStashTakeMsg( CNetMsg::SP& msg , int charIndex, int guildIndex, char error, LONGLONG takeMoney, int itemCount )
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_GUILD_STASH_TAKE
		<< charIndex
		<< guildIndex
		<< error
		<< takeMoney
		<< itemCount;
}

void HelperGuildStashLogMsg( CNetMsg::SP& msg, int charIndex, int guildIndex, int logCount )
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_GUILD_STASH_LOG
		<< charIndex
		<< guildIndex
		<< logCount;
}

void HelperGuildStashErrorMsg( CNetMsg::SP& msg, int charIndex, char error )
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_GUILD_STASH_ERROR
		<< charIndex
		<< error;
}

#endif // DEV_GUILD_STASH

void HelperGuildMasterKickRep(CNetMsg::SP& msg, int _guildIndex, int _requestChar, int _result, int _requestTime)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_GUILD_MASTER_KICK_REP
		<< _guildIndex
		<< _requestChar
		<< _result
		<< _requestTime;
}

void HelperGuildMasterKickCancelRep(CNetMsg::SP& msg, int _guildIndex, int _requestChar, int _result)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_GUILD_MASTER_KICK_CANCEL_REP
		<< _guildIndex
		<< _requestChar
		<< _result;
}

void HelperGuildMasterKickStatus(CNetMsg::SP& msg, int _guildIndex, int _status)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_GUILD_MASTER_KICK_STATUS
		<< _guildIndex
		<< _status;
}

void HelperGuildMasterKickReset(CNetMsg::SP& msg, int _charIndex, int _reset)
{
	msg->Init(MSG_HELPER_COMMAND);
	RefMsg(msg) << MSG_HELPER_GUILD_MASTER_KICK_RESET
		<< _charIndex
		<< _reset;
}
