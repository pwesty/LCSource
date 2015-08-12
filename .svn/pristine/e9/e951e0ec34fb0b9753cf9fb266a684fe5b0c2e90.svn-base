#include "stdhdrs.h"

#include "Log.h"
#include "Server.h"
#include "Guild.h"
#include "CmdMsg.h"
#include "doFunc.h"
#include "Battle.h"

CGuildMember::CGuildMember()
	: m_name(MAX_CHAR_NAME_LENGTH + 1)
//, m_nick(MAX_CHAR_NAME_LENGTH + 1)
	, m_positionName( GUILD_POSITION_NAME +1)
{
	m_charindex = -1;
	m_pos = MSG_GUILD_POSITION_MEMBER;
	m_guild = NULL;
	m_listindex = -1;
	m_online = 0;
	m_pc = NULL;

	m_contributeExp		= 0;
	m_contributeFame	= 0;
	m_cumulatePoint		= 0;
	// kj add
	m_channel			= 1;
	m_zoneindex			= 0;
}

CGuildMember::~CGuildMember()
{
	if (m_pc)
		m_pc->m_guildInfo = NULL;
	m_pc = NULL;
}

char CGuildMember::GetGuildMark()
{
	if (guild() != NULL)
	{
		for(int i = 0; i < 3; i++)
		{
			if(guild()->index() == gserver->m_nGuildMarkTable[i])
			{
				return i+1;
			}
		}
	}

	return 0;
}

CGuild::CGuild(int guildindex, const char* name, int level, int battleIndex, int battlePrize, int battleTime, int battleZone, int battleKillCount, int battleState)
	: m_name(MAX_GUILD_NAME_LENGTH + 1)
{
	if (name && level > 0)
	{
		m_name = name;
		m_level = level;
		m_boss = NULL;
		memset(m_officer, 0, sizeof(CGuildMember*) * GUILD_MAX_OFFICER);
		memset(m_member, 0, sizeof(CGuildMember*) * GUILD_MAX_MEMBER);
		m_membercount = 0;
		m_next = NULL;
		m_prev = NULL;
		m_index = guildindex;

		m_battleIndex = battleIndex;
		m_battlePrize = battlePrize;
		m_battleTime = battleTime;
		m_battleZone = battleZone;
		m_killCount = battleKillCount;
		m_battleState = battleState;
		m_battlePulse = 0;
	}
	else
	{
		memset(this, 0, sizeof(*this));
	}

	memset(&m_guildSignal, 0, sizeof(GUILD_SIGNAL));

#ifdef GMTOOL
	m_bGmtarget = false;
#endif // GMTOOL

	m_landcount = 0;
	memset(m_land, -1, sizeof(m_land) );
	m_GuildPoint		= 0;
	m_incline			= 0;
	m_maxmember = 0;
	m_GuildPointRanking = 0;

	m_bRebirthPos = false;

#ifdef EXTREME_CUBE
	m_cubeUniqueIdx = -1;
#endif // EXTREME_CUBE
	m_nCountRushCaptain		= 0;
	m_nCountSupportCaptain	= 0;
	m_nCountReconCaptain	= 0;
	m_nCountTotalGradeEx	= 0;
#ifdef DEV_GUILD_MARK
	m_GuildMark_row			= -1;
	m_GuildMark_col			= -1;
	m_Background_row		= -1;
	m_Background_col		= -1;
	m_GuildMarkExpire		= -1;
#endif
}

CGuild::~CGuild()
{
	int i;
	for (i = 0; i < GUILD_MAX_OFFICER; i++)
	{
		if (m_member[i])
			delete m_member[i];
	}
	memset(m_officer, 0, sizeof(CGuildMember*) * GUILD_MAX_OFFICER);
	memset(m_member, 0, sizeof(CGuildMember*) * GUILD_MAX_MEMBER);
	m_membercount = 0;
	m_next = NULL;
	m_prev = NULL;

#ifdef EXTREME_CUBE
	m_cubeUniqueIdx = -1;
#endif // EXTREME_CUBE
}

int CGuild::maxmember()
{
	if( m_level == 1 )
		return GUILD_LEVEL1_MAX_MEMBER;
	else if( m_level == 2 )
		return GUILD_LEVEL2_MAX_MEMBER;
	else if( m_level == 3 )
		return GUILD_LEVEL3_MAX_MEMBER;
	else if( m_level == 4 )
		return GUILD_LEVEL4_MAX_MEMBER;
	else if( m_level == 5 )
		return GUILD_LEVEL5_MAX_MEMBER;
	return m_maxmember;
}

int CGuild::addmember(int charindex, const char* name)
{
	CGuildMember* pmember = findmember(charindex);
	if (pmember != NULL)
		return pmember->listindex();

	int max = maxmember();
	if (m_membercount >= max)
	{
		return -1;
	}
	else
	{
		int i;
		for (i = 0; i < GUILD_MAX_MEMBER; i++)
		{
			if (m_member[i] == NULL)
			{
				pmember = new CGuildMember();
				pmember->charindex(charindex);
				pmember->name(name);
				//member nick부분 삭제 : bw 060403
//				if( (nick != NULL) && (strcmp(nick, "") != 0) )
//					pmember->nick(nick);
				pmember->pos(MSG_GUILD_POSITION_MEMBER);
				pmember->guild(this);
				pmember->listindex(i);

				m_member[i] = pmember;
				m_membercount++;

				return i;
			}
		}
		return -1;
	}
}

CGuildMember* CGuild::changeboss(int newbosslistindex)
{
	if (newbosslistindex < 0 || newbosslistindex >= GUILD_MAX_MEMBER)
		return NULL;

	CGuildMember* ret = NULL;

	if (m_member[newbosslistindex])
	{
		if (m_boss)
		{
			ret = m_boss;
			m_boss->pos(MSG_GUILD_POSITION_MEMBER);
			m_boss = NULL;
		}
		if (m_member[newbosslistindex]->pos() == MSG_GUILD_POSITION_OFFICER)
		{
			fire(newbosslistindex);
		}
		m_boss = m_member[newbosslistindex];
		m_boss->pos(MSG_GUILD_POSITION_BOSS);
	}

	return ret;
}

CGuildMember* CGuild::findmember(int charindex)
{
	int i;
	for (i = 0; i < GUILD_MAX_MEMBER; i++)
	{
		if (m_member[i])
		{
			if (m_member[i]->charindex() == charindex)
				return m_member[i];
		}
	}
	return NULL;
}

CGuildMember* CGuild::findmember(const char* charname)
{
	int i;
	for (i = 0; i < GUILD_MAX_MEMBER; i++)
	{
		if (m_member[i])
		{
			if (strcmp(m_member[i]->GetName(), charname) == 0)
				return m_member[i];
		}
	}
	return NULL;
}

CGuildList::CGuildList()
{
	m_head = NULL;
	m_count = 0;
}

CGuildList::~CGuildList()
{
	while (m_head)
	{
		CGuild* p = m_head;
		m_head = m_head->nextguild();
		delete p;
	}
}

CGuild* CGuildList::create(int guildindex, const char* guildname, int charindex, const char* bossname)
{
	if (guildname == NULL || guildindex < 1 || charindex < 1 || bossname == NULL)
		return NULL;
	if (strlen(guildname) < 1 || strlen(bossname) < 1)
		return NULL;

	// 길드 만들고
	CGuild* guild = new CGuild(guildindex, guildname, 1);
	// 길드에 추가
	int listindex = guild->addmember(charindex, bossname);
	// 보스 지정
	guild->changeboss(listindex);

	return guild;
}

CGuild* CGuildList::create(int guildindex, const char* guildname, int guildlevel, int battleIndex, int battlePrize, int battleTime, int battleZone, int battleKillCount, int battleState)
{
	if (guildname == NULL || guildindex < 1 || guildlevel < 1 || guildlevel > GUILD_MAX_GUILD_LEVEL)
		return NULL;
	if (strlen(guildname) < 1)
		return NULL;

	if (battlePrize > GUILD_BATTLE_MAX_PRIZE * 2)
		return NULL;

	if (battleIndex != -1)
	{
		if (battleZone != ZONE_START && battleZone != ZONE_DRATAN && battleZone != ZONE_MERAC && battleZone != ZONE_EGEHA
				&& battleZone != ZONE_STREIANA && battleZone != ZONE_MONDSHINE && battleZone != ZONE_TARIAN && battleZone != ZONE_BLOODYMIR )
			return NULL;
		if (battleKillCount < 0 || battleState < 0)
			return NULL;
	}

	CGuild* guild = new CGuild(guildindex, guildname, guildlevel, battleIndex, battlePrize, battleTime, battleZone, battleKillCount, battleState);

	return guild;
}

void CGuildList::add(CGuild* guild)
{
	if (guild == NULL)
		return ;

	if (m_head)
	{
		m_head->prevguild(guild);
	}
	guild->prevguild(NULL);
	guild->nextguild(m_head);
	m_head = guild;
	m_count++;
}

CGuild* CGuildList::findguild(int guildindex)
{
	CGuild* ret = m_head;
	while (ret)
	{
		if (ret->index() == guildindex)
			return ret;

		ret = ret->nextguild();
	}
	return NULL;
}

CGuild* CGuildList::findguild(const char* guildname)
{
	CGuild* ret = m_head;
	while (ret)
	{
		if (strcmp(ret->name(), guildname) == 0)
			return ret;

		ret = ret->nextguild();
	}
	return NULL;
}

CGuildMember* CGuildList::findmember(int charindex)
{
	// TODO : GUILD : 길드 트리시 2개 길드에 모두 있는 경우 검사
	CGuildMember* ret = NULL;
	CGuild* p = m_head;
	while (p)
	{
		ret = p->findmember(charindex);
		if (ret)
			return ret;
		else
			p = p->nextguild();
	}
	return NULL;
}

void CGuild::SendToAll(CNetMsg::SP& msg, bool bCell)
{
	int i;
	for (i = 0; i < GUILD_MAX_MEMBER; i++)
	{
		if (m_member[i] != NULL)
		{
			if (m_member[i]->GetPC() != NULL
// << kjban
					&& m_member[i]->online() == 1				// 온라인 상태인 경우
					&& m_member[i]->GetPC()->m_desc != NULL)	// 디스크립터가 존재 하는 경우
// >>
			{
				SEND_Q(msg, m_member[i]->GetPC()->m_desc);
				if(bCell)
				{
					{
						CNetMsg::SP rmsg(new CNetMsg);
						DisappearMsg(rmsg, m_member[i]->GetPC() );
						m_member[i]->GetPC()->m_pArea->SendToCell(rmsg, m_member[i]->GetPC() );
					}

					{
						CNetMsg::SP rmsg(new CNetMsg);
						AppearMsg(rmsg, m_member[i]->GetPC(), true);
						m_member[i]->GetPC()->m_pArea->SendToCell(rmsg, m_member[i]->GetPC() );
					}
				}
			}
		}
	}
}

void CGuild::SendToAllInSameZone(CNetMsg::SP& msg, int zoneindex)
{
	int i;
	for (i = 0; i < GUILD_MAX_MEMBER; i++)
	{
		if (m_member[i])
		{
			if (m_member[i]->GetPC())
			{
				if (m_member[i]->GetPC()->m_pZone->m_index == zoneindex)
				{
					SEND_Q(msg, m_member[i]->GetPC()->m_desc);
				}
			}
		}
	}
}

void CGuild::SendToAllStatusMsg()
{
	for (int i = 0; i < GUILD_MAX_MEMBER; i++)
	{
		if (m_member[i] != NULL)
		{
			if (m_member[i]->GetPC() != NULL
				// << kjban
				&& m_member[i]->online() == 1				// 온라인 상태인 경우
				&& m_member[i]->GetPC()->m_desc != NULL)	// 디스크립터가 존재 하는 경우
				// >>
			{
				m_member[i]->GetPC()->CalcStatus(true);
			}
		}
	}
}

void CGuildList::Reset()
{
	CGuild* p = m_head;
	while (p)
	{
		m_head = p->nextguild();
		delete p;
		p = m_head;
	}
	m_count = 0;
}

void CGuildList::Remove(CGuild* guild)
{
	// TODO : GUILD : 트리구성시 최상위 길드외의 길드 처리

	// 해당 길드원 정보 변경
	int i;
	int guildMaxMember = guild->maxmember();
	for (i = 0; i < guildMaxMember; i++)
		guild->removemember(guild->member(i));

	CGuild* p = m_head;
	while (p)
	{
		if (p == guild)
			break;
		else
			p = p->nextguild();
	}
	if (p == NULL)
		return ;

	if (p->prevguild())
		p->prevguild()->nextguild(p->nextguild());
	if (p->nextguild())
		p->nextguild()->prevguild(p->prevguild());
	if (p == m_head)
		m_head = p->nextguild();
	p->prevguild(NULL);
	p->nextguild(NULL);
	m_count--;
	delete p;
}

void CGuild::removemember(CGuildMember* delmember)
{
	if (delmember == NULL)
		return ;

	int i;
	for (i = 0; i < GUILD_MAX_MEMBER; i++)
	{
		if (m_member[i] == delmember)
		{
			if (delmember->GetPC())
			{
				delmember->GetPC()->RemoveAllGuildSkillFromQuickSlot();
				delmember->GetPC()->m_guildInfo = NULL;
			}

			delmember->SetPC(NULL);
//			if (delmember->pos() == MSG_GUILD_POSITION_OFFICER)
//			{
			int j;
			for (j = 0; j < GUILD_MAX_OFFICER; j++)
			{
				if (m_officer[j] == delmember)
					m_officer[j] = NULL;
			}
//			}
			delete delmember;
			m_member[i] = NULL;
			m_membercount--;
			return ;
		}
	}
}

void CGuild::appoint(int listindex)
{
	if (listindex < 0 || listindex >= GUILD_MAX_MEMBER)
		return ;

	if (m_member[listindex])
	{
		int i;
		for (i = 0; i < GUILD_MAX_OFFICER; i++)
		{
			if (m_officer[i] == NULL)
			{
				m_officer[i] = m_member[listindex];
				m_officer[i]->pos(MSG_GUILD_POSITION_OFFICER);
				return ;
			}
		}
	}
}

void CGuild::fire(int listindex)
{
	if (listindex < 0 || listindex >= GUILD_MAX_MEMBER)
		return ;

	if (m_member[listindex])
	{
		int i;
		for (i = 0; i < GUILD_MAX_OFFICER; i++)
		{
			if (m_officer[i] == m_member[listindex])
			{
				m_officer[i]->pos(MSG_GUILD_POSITION_MEMBER);
				m_officer[i] = NULL;
				return ;
			}
		}
	}
}

#ifdef LC_GAMIGO
int CGuild::BossChangeNeedPCLevel()
{
	int needPCLevel = 10;
	if(m_level == 1)
	{
		needPCLevel = 10;
	}
	else if(m_level == 2)
	{
		needPCLevel = 15;
	}
	else if(m_level == 3)
	{
		needPCLevel = 20;
	}
	else if(m_level == 4)
	{
		needPCLevel = 25;
	}
	else if(m_level >= 5 && m_level <= 10)
	{
		needPCLevel = 30;
	}
	else if(m_level >= 11 && m_level <= 19)
	{
		needPCLevel = 40;
	}
	else if(m_level >= 20 && m_level <= 29)
	{
		needPCLevel = 50;
	}
	else if(m_level >= 30 && m_level <= 39)
	{
		needPCLevel = 60;
	}
	else if(m_level >= 40 && m_level <= 49)
	{
		needPCLevel = 70;
	}
	else if(m_level >= 50)
	{
		needPCLevel = 80;
	}

	return needPCLevel;
}
#endif // LC_GAMIGO

CPC* CGuild::GetNearMember(CCharacter* pPC, int nListIndex)
{
	if(nListIndex < 0 || nListIndex >= GUILD_MAX_MEMBER)
		return NULL;
	if(m_member[nListIndex] == NULL)
		return NULL;
	if(m_member[nListIndex]->GetPC() == NULL)
		return NULL;
	if(!CheckInNearCellExt(pPC, m_member[nListIndex]->GetPC()))
		return NULL;
	return m_member[nListIndex]->GetPC();
}

CSkill* CGuild::FindSkill(int index)
{
	CSkill* ret;
	ret = m_activeSkillList.Find(index);
	if(ret)
		return ret;
	ret = m_passiveSkillList.Find(index);
	if(ret)
		return ret;
	ret = m_etcSkillList.Find(index);
	if(ret)
		return ret;

	return NULL;
}

void CGuild::clearReadySkill()
{
	m_activeSkillList.clearReadySkill();
	m_passiveSkillList.clearReadySkill();
	m_etcSkillList.clearReadySkill();
}


void CGuild::AddGuildPoint(int GuildPoint)
{
	m_GuildPoint += GuildPoint;

	CGuildMember* pBoss = NULL;
	CPC* pc;
	pBoss = this->boss();
	if(pBoss == NULL)			return ;
	if(pBoss->online() == 0)	return ;
	if(pBoss->GetPC() == NULL)	return ;

	{
		CNetMsg::SP rmsg(new CNetMsg);
		pc = pBoss->GetPC();
		GuildPointInfo(rmsg, this->GetGuildPoint());
		SEND_Q(rmsg, pc->m_desc);
	}
}

void CGuild::InitGradeExPosCount()
{
	m_nCountRushCaptain		= 0;
	m_nCountSupportCaptain	= 0;
	m_nCountReconCaptain	= 0;
	m_nCountTotalGradeEx	= 0;
}

bool CGuild::CheckGradeExPosCount( int pos, int charIndex )
{
	CGuildMember* member = findmember(charIndex);

	switch( pos )
	{
	case MSG_GUILD_POSITION_RUSH_CAPTAIN:			// 돌격조 대장
		if(member != NULL)
		{
			if(m_nCountRushCaptain >= 1 && member->pos() != pos)
				return false;
		}
		else
		{
			if(m_nCountRushCaptain >= 1)
				return false;
		}		
		break;
	case MSG_GUILD_POSITION_SUPPORT_CAPTAIN:		// 지원조 대장
		if(member != NULL)
		{
			if( m_nCountSupportCaptain >= 1 && member->pos() != pos)
				return false;
		}
		else
		{
			if( m_nCountSupportCaptain >= 1 )
				return false;
		}		
		break;
	case MSG_GUILD_POSITION_RECON_CAPTAIN:			// 정찰조 대장
		if(member != NULL)
		{
			if( m_nCountReconCaptain >= 1 && member->pos() != pos)
				return false;
		}
		else
		{
			if( m_nCountReconCaptain >= 1)
				return false;
		}
		break;
	case MSG_GUILD_POSITION_RUSH_MEMBER:			// 돌격조원
	case MSG_GUILD_POSITION_SUPPORT_MEMBER:			// 지원조원
	case MSG_GUILD_POSITION_RECON_MEMBER:			// 정찰조원:
		if( member != NULL &&
			m_nCountTotalGradeEx >= GUILD_MAX_GRADE_EX && member->pos() != pos)
			return false;
		break;
	}
	return true;
}

void CGuild::ChangeGradeExPosCount( int oldpos, int pos)
{
	if( CheckGradeExPosCount(pos) )
	{
		DelGradeExPosCount(oldpos);
		AddGradeExPosCount(pos);
	}
}

void CGuild::DelGradeExPosCount(int pos)
{
	switch( pos )
	{
	case MSG_GUILD_POSITION_RUSH_CAPTAIN:			// 돌격조 대장
		m_nCountRushCaptain --;
		break;
	case MSG_GUILD_POSITION_SUPPORT_CAPTAIN:			// 지원조 대장
		m_nCountSupportCaptain --;
		break;
	case MSG_GUILD_POSITION_RECON_CAPTAIN:			// 정찰조 대장
		m_nCountReconCaptain --;
		break;
	case MSG_GUILD_POSITION_RUSH_MEMBER:				// 돌격조원
	case MSG_GUILD_POSITION_SUPPORT_MEMBER:			// 지원조원
	case MSG_GUILD_POSITION_RECON_MEMBER:			// 정찰조원:
		m_nCountTotalGradeEx --;
		break;
	}
}

void CGuild::AddGradeExPosCount( int pos )
{
	switch( pos )
	{
	case MSG_GUILD_POSITION_RUSH_CAPTAIN:			// 돌격조 대장
		m_nCountRushCaptain ++;
		break;
	case MSG_GUILD_POSITION_SUPPORT_CAPTAIN:			// 지원조 대장
		m_nCountSupportCaptain ++;
		break;
	case MSG_GUILD_POSITION_RECON_CAPTAIN:			// 정찰조 대장
		m_nCountReconCaptain ++;
		break;
	case MSG_GUILD_POSITION_RUSH_MEMBER:				// 돌격조원
	case MSG_GUILD_POSITION_SUPPORT_MEMBER:			// 지원조원
	case MSG_GUILD_POSITION_RECON_MEMBER:			// 정찰조원:
		m_nCountTotalGradeEx ++;
		break;
	}
}

int CGuild::GetGradeExPosNeedGuilPoint( int pos )
{
	switch( pos )
	{
	case MSG_GUILD_POSITION_RUSH_CAPTAIN:		// 돌격조 대장
	case MSG_GUILD_POSITION_SUPPORT_CAPTAIN:	// 지원조 대장
	case MSG_GUILD_POSITION_RECON_CAPTAIN:		// 정찰조 대장
		return 1000;
		break;
	case MSG_GUILD_POSITION_RUSH_MEMBER:		// 돌격조원
	case MSG_GUILD_POSITION_SUPPORT_MEMBER:		// 지원조원
	case MSG_GUILD_POSITION_RECON_MEMBER:		// 정찰조원
		return 500;
		break;
	}

	return 0;
}

#ifdef DEV_GUILD_MARK
void CGuild::SetGuildMark(char gm_row, char gm_col, char bg_row, char bg_col, int markTime)
{
	m_GuildMark_row = gm_row;
	m_GuildMark_col = gm_col;
	m_Background_row = bg_row;
	m_Background_col = bg_col;
	// 길드마크 시간은 실제 로컬 시간으로 저장되어 있지만 게임에서 사용할때는 게임시간을 클라이언트에 보내준다.
	// 실제 시간이 저장이 된다.
	m_GuildMarkExpire = markTime;
}

char CGuild::GetGuildMarkRow()
{
	return m_GuildMark_row;
}

char CGuild::GetGuildMarkCol()
{
	return m_GuildMark_col;
}

char CGuild::GetBackgroundRow()
{
	return m_Background_row;
}

char CGuild::GetBackgroundCol()
{
	return m_Background_col;
}

int CGuild::GetMarkTime()
{
	// 길드마크 시간은 실제 로컬 시간으로 저장되어 있지만 게임에서 사용할때는 게임시간을 클라이언트에 보내준다.
	return m_GuildMarkExpire;
}
int CGuild::GetMarkTimeConvertGameTime()
{
	// 길드마크 시간은 실제 로컬 시간으로 저장되어 있지만 게임에서 사용할때는 게임시간을 클라이언트에 보내준다.
	if(m_GuildMarkExpire == -1)
		return -1;
	int tGap = 0;
	tGap = m_GuildMarkExpire - (int)mktime(&gserver->m_tRealSystemTime);
	if(tGap < 0)
		tGap = 0;
	return (gserver->getNowSecond() + tGap);
}
#endif

CGuildKick* CGuild::getGuildKick(void)
{
	return &m_guildKick;
}

bool CGuild::isPossibleKickMaster(void)
{
	int _standardTime = getStandardTimeForKick();
	int _bossDateStamp = getBossDateStamp();

	if ( _standardTime < 0 || _bossDateStamp < 0 )
		return false;

	if ( _bossDateStamp > _standardTime )
		return false;

	return true;
}

int CGuild::getStandardTimeForKick(void)
{
	// 7일 전 시간 구하기
	CLCString getStandardTimeQuery(512);
	getStandardTimeQuery.Format("SELECT UNIX_TIMESTAMP(DATE_SUB(NOW(), INTERVAL 7 DAY))");

	CDBCmd getStandardTimeCmd;
	getStandardTimeCmd.Init(&gserver->m_dbcharingame);
	getStandardTimeCmd.SetQuery(getStandardTimeQuery);

	if ( !getStandardTimeCmd.Open() || !getStandardTimeCmd.MoveFirst() )
	{
		GAMELOG << init("GUILD MASTER KICK ERROR")
				<< "GET STANDARD TIME ERROR"
				<< end;
		return -1;
	}

	int _standardTime(50);
	getStandardTimeCmd.GetRec(0, _standardTime);

	return _standardTime;
}

int CGuild::getBossDateStamp(void)
{
	int _bossIndex = boss()->charindex();
	CLCString getBossDateStampQuery(512);
	getBossDateStampQuery.Format("SELECT UNIX_TIMESTAMP(a_datestamp) FROM t_characters WHERE a_index = %d", _bossIndex);

	CDBCmd getBoossDateStampCmd;
	getBoossDateStampCmd.Init(&gserver->m_dbcharingame);
	getBoossDateStampCmd.SetQuery(getBossDateStampQuery);

	if ( !getBoossDateStampCmd.Open() || !getBoossDateStampCmd.MoveFirst() )
	{
		GAMELOG << init("GUILD MASTER KICK ERROR")
				<< "GET BOSS TIMESTAMP ERROR"
				<< end;
		return -1;
	}

	int _bossDateStamp;
	getBoossDateStampCmd.GetRec(0, _bossDateStamp);

	return _bossDateStamp;
}

int CGuild::getLastChansgeBossTime(void)
{
	int _guildIndex = this->index();
	CLCString getLastChangeBossTimeQuery(512);
	getLastChangeBossTimeQuery.Format("SELECT a_lastdate_change_boss FROM t_extend_guild WHERE a_guild_index = %d", _guildIndex);

	CDBCmd getLastChangeBossTimeCmd;
	getLastChangeBossTimeCmd.Init(&gserver->m_dbcharingame);
	getLastChangeBossTimeCmd.SetQuery(getLastChangeBossTimeQuery);

	if ( !getLastChangeBossTimeCmd.Open() || !getLastChangeBossTimeCmd.MoveFirst() )
	{
		GAMELOG << init("GUILD MASTER KICK ERROR")
				<< "GET LAST CHANGE BOSS TIME ERROR"
				<< end;
		return -1;
	}

	int _lastChangeBossTime;
	getLastChangeBossTimeCmd.GetRec(0, _lastChangeBossTime);

	return _lastChangeBossTime;
}

bool CGuild::isPastLastChangeBoss(void)
{
	time_t t = time(NULL);
	int _lastTime = getLastChansgeBossTime();

	if ( t -_lastTime < LAST_CONNECT_TIME_FOR_KICK )
		return false;

	return true;
}

int CGuild::getNewBossByKick(void)
{
	int _newBossIndex = 0;

	std::vector<int> _vecCandidate;	// 후보자 리스트

	// 후보자 선출
	// 1순위 : 길드 부장(없으면 일반 길드원)
	CGuildMember* _officer1 = officer(0);
	CGuildMember* _officer2 = officer(1);

	if (_officer1 && _officer2)
	{
		// 둘다 존재
		_vecCandidate.push_back(_officer1->charindex());
		_vecCandidate.push_back(_officer2->charindex());
	}
	else if (_officer1 && !_officer2)
	{
		// 부장 한명만 존재 : 단독 출마 당선
		_newBossIndex = _officer1->charindex();
		return _newBossIndex;
	}
	else if (_officer2 && !_officer1)
	{
		// 부장 한명만 존재 : 단독 출마 당선
		_newBossIndex = _officer2->charindex();
		return _newBossIndex;
	}
	else
	{
		// 부장이 없을 때는 모든 길드원이 후보
		for (int i = 0; i < GUILD_MAX_MEMBER; i++)
		{
			if (m_member[i])
			{
				if (m_member[i]->charindex() != boss()->charindex())
					_vecCandidate.push_back(m_member[i]->charindex());
			}
		}
	}

	if (_vecCandidate.empty())
		return _newBossIndex;

	// 2순위 : 최근 접속 일자
	// 3순위 : 캐릭터 레벨
	// 4순위 : 길드 포인트 기여도
	CLCString cadidateDataQuery(1024);
	cadidateDataQuery.Format("SELECT ch.a_index FROM t_characters as ch, t_extend_guildmember as eg "
							 "WHERE eg.a_guild_index = %d and eg.a_char_index = ch.a_index ORDER BY ch.a_datestamp DESC, ch.a_level DESC, eg.a_point DESC, ch.a_index"
							 ,m_index);

	CDBCmd cadidateDataCmd;
	cadidateDataCmd.Init(&gserver->m_dbchar);
	cadidateDataCmd.SetQuery(cadidateDataQuery);

	if (!cadidateDataCmd.Open())
		return _newBossIndex;

	while(cadidateDataCmd.MoveNext())
	{
		int characterIndex;
		cadidateDataCmd.GetRec(0, characterIndex);

		/*
		std::vector<int>::iterator itr;
		itr = find(_vecCandidate.begin(), _vecCandidate.end(), characterIndex);

		if ( itr != _vecCandidate.end() )
		{
			_newBossIndex = (*itr);
			return _newBossIndex;
		}
		*/

		std::vector<int>::iterator itr = std::find(_vecCandidate.begin(), _vecCandidate.end(), characterIndex);
		if ( itr != _vecCandidate.end() )
		{
			_newBossIndex = (*itr);
			break;
		}
	}

	return _newBossIndex;
}

