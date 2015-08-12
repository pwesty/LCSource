#include <boost/thread.hpp>
#include <boost/format.hpp>
#include "stdhdrs.h"

#include "Server.h"
#include "../ShareLib/DBCmd.h"
#include "CmdMsg.h"
#include "Log.h"
#include "doFunc.h"

CRankingSystemEX::CRankingSystemEX()
	: m_lock(false), m_reward(false)
{
	mysql_init(&m_dbrank);
}
CRankingSystemEX::~CRankingSystemEX()
{
}

bool CRankingSystemEX::RankingEX_Refresh(void)
{
	LOG_INFO("RankingEX_RefreshDate");
	if ( !RankingEX_RefreshDate() )
	{
		// 갱신 날짜 설정
		return false;
	}

	LOG_INFO("RankingEX_BackUp");
	if ( !RankingEX_BackUp() )
	{
		// 백업
		return false;
	}

	LOG_INFO("RankingEX_ClearRankingTable");
	if ( !RankingEX_ClearRankingTable() )
	{
		// 초기화
		return false;
	}

	LOG_INFO("RankingEX_RefreshData");
	if ( !RankingEX_RefreshData() )
	{
		return false;
	}

	LOG_INFO("RankingEX_RefreshRankByLevel");
	if ( !RankingEX_RefreshRankByLevel() )
	{
		// 랭킹 산정
		return false;
	}

	LOG_INFO("RankingEX_ClearCharTable");
	if ( !RankingEX_ClearCharTable() )
	{
		// 캐릭터 테이블의 랭킹 관련 필드(a_exp_weekly) 초기화
		return false;
	}

	return true;
}

bool CRankingSystemEX::RankingEX_BackUp(void)
{
	CLCString sql(1024);
	CDBCmd cmdchar;
	cmdchar.Init(&m_dbrank);

	// t_ranking_ex_backup 데이터 삭제
	sql.Format("TRUNCATE  TABLE t_ranking_ex_backup");
	cmdchar.SetQuery(sql);
	if ( !cmdchar.Update() )
	{
		GAMELOG << init("SYSTEM_RANKING_EX ERROR: RankingEX_BackUp()_1") << end;
		return false;
	}

	// t_ranking_ex -> t_ranking_ex_backup으로 이동
	sql.Format("INSERT INTO t_ranking_ex_backup SELECT * FROM t_ranking_ex");
	cmdchar.SetQuery(sql);
	if ( !cmdchar.Update() )
	{
		GAMELOG << init("SYSTEM_RANKING_EX ERROR: RankingEX_BackUp()_2") << end;
		return false;
	}

	// t_ranking_data_backup 데이터 삭제
	sql.Format("TRUNCATE TABLE t_ranking_data_backup");
	cmdchar.SetQuery(sql);
	if ( !cmdchar.Update() )
	{
		GAMELOG << init("SYSTEM_RANKING_EX ERROR: RankingEX_BackUp()_3") << end;
		return false;
	}

	// t_ranking_data -> t_ranking_data_backup으로 이동
	sql.Format("INSERT INTO t_ranking_data_backup SELECT * FROM t_ranking_data");
	cmdchar.SetQuery(sql);
	if ( !cmdchar.Update() )
	{
		GAMELOG << init("SYSTEM_RANKING_EX ERROR: RankingEX_BackUp()_4") << end;
		return false;
	}

	return true;
}

bool CRankingSystemEX::RankingEX_ClearRankingTable(void)
{
	CLCString sql(1024);
	CDBCmd cmdchar;
	cmdchar.Init(&m_dbrank);

	// t_ranking_ex 초기화
	sql.Format("TRUNCATE TABLE t_ranking_ex");
	cmdchar.SetQuery(sql);
	if ( !cmdchar.Update() )
	{
		GAMELOG << init("SYSTEM_RANKING_EX ERROR: RankingEX_ClearRankingTable()_1") << end;
		return false;
	}

	// t_ranking_data 초기화
	sql.Format("TRUNCATE TABLE t_ranking_data");
	cmdchar.SetQuery(sql);
	if ( !cmdchar.Update() )
	{
		GAMELOG << init("SYSTEM_RANKING_EX ERROR: RankingEX_ClearRankingTable()_2") << end;
		return false;
	}
	return true;
}

bool CRankingSystemEX::RankingEX_ClearCharTable(void)
{
	CLCString sql(1024);
	CDBCmd cmdchar;
	cmdchar.Init(&m_dbrank);

	// t_characters의 a_exp_weekly 필드 초기화
	sql.Format("UPDATE t_characters SET a_exp_weekly = 0");
	cmdchar.SetQuery(sql);
	if ( !cmdchar.Update() )
	{
		GAMELOG << init("SYSTEM_RANKING_EX ERROR: RankingEX_ClearCharTable()_1") << end;
		return false;
	}
	return true;
}

bool CRankingSystemEX::RankingEX_RefreshDate(void)
{
	struct tm nowtm = NOW();
	m_refreshDate = boost::str(boost::format("%04d-%02d-%02d %02d:%02d:%02d")
							   % (nowtm.tm_year + 1900) % (nowtm.tm_mon + 1) % nowtm.tm_mday % nowtm.tm_hour % nowtm.tm_min % nowtm.tm_sec);

	CLCString sql(1024);
	CDBCmd cmdchar;
	cmdchar.Init(&m_dbrank);

	// Refresh Date 갱신
	sql.Format("INSERT INTO t_ranking_date(a_date) VALUES('%s')", GetRefreshTime().c_str());
	cmdchar.SetQuery(sql);
	if ( !cmdchar.Update() )
	{
		GAMELOG << init("SYSTEM_RANKING_EX ERROR: RankingEX_RefreshDate()_1") << end;
		return false;
	}

	return true;
}

bool CRankingSystemEX::RankingEX_RefreshData(void)
{
	CLCString sql(1024);
	CDBCmd cmdchar;
	cmdchar.Init(&m_dbrank);

	// t_characters의 데이터를 t_ranking_data로
#if defined(LC_KOR)
	sql.Format("INSERT INTO t_ranking_data(a_char_index, a_job, a_level, a_exp, a_levelup_date, a_refresh_date) "
			   "SELECT a_index, a_job, a_level, a_exp_weekly, a_levelup_date, '%s' "
			   "FROM t_characters WHERE a_admin >= 0 and a_enable = 1 and a_datestamp > DATE_SUB(NOW(), INTERVAL 3 MONTH) ORDER BY a_level DESC, a_exp DESC, a_levelup_date, a_index",
			   m_refreshDate.c_str());
#else
	sql.Format("INSERT INTO t_ranking_data(a_char_index, a_job, a_level, a_exp, a_levelup_date, a_refresh_date) "
			   "SELECT a_index, a_job, a_level, a_exp_weekly, a_levelup_date, '%s' "
			   "FROM t_characters WHERE a_admin = 0 and a_enable = 1 and a_datestamp > DATE_SUB(NOW(), INTERVAL 3 MONTH) ORDER BY a_level DESC, a_exp DESC, a_levelup_date, a_index",
			   m_refreshDate.c_str());
#endif
	cmdchar.SetQuery(sql);
	if ( !cmdchar.Update() )
	{
		GAMELOG << init("SYSTEM_RANKING_EX ERROR: RankingEX_RefreshData()_1") << end;
		return false;
	}

	return true;
}

bool CRankingSystemEX::RankingEX_RefreshRankByLevel(void)
{
	CLCString sql(1024);
	CDBCmd cmdchar, cmdrank;
	cmdchar.Init(&m_dbrank);
	cmdrank.Init(&m_dbrank);

	int char_index = 0;
	int job = 0;
	int rankBylevel = 1;
#ifdef EX_MAGE
	int rankByjob[JOBCOUNT] = {1,1,1,1,1,1,1,1,1};
#else
#if defined (EX_ROGUE)
	int rankByjob[JOBCOUNT] = {1,1,1,1,1,1,1,1};
#else
	int rankByjob[JOBCOUNT] = {1,1,1,1,1,1,1};
#endif
#endif

	enum
	{
		A_CHAR_INDEX = 0,
		A_JOB,
	};

	sql.Format("SELECT a_char_index, a_job FROM t_ranking_data");
	cmdchar.SetQuery(sql);
	if( !cmdchar.Open() )
	{
		GAMELOG << init("SYSTEM_RANKING_EX ERROR: RankingEX_RefreshRankByLevel()_1") << end;
		return false;
	}

	std::string tstr;
	tstr.reserve(20480);

	int totalcount = cmdchar.m_nrecords;
	for (int k = 0; k < totalcount; )
	{
		tstr = "INSERT INTO t_ranking_ex(a_char_index, a_rank_by_level, a_rank_by_job, a_refresh_date) VALUES";

		int count = 0;
		for (; count < 500 && k < totalcount; ++count, ++k)
		{
			if (cmdchar.MoveNext() == false)
				break;

			cmdchar.GetRec(A_CHAR_INDEX, char_index);
			cmdchar.GetRec(A_JOB, job);

			tstr += boost::str(boost::format("(%1%,%2%,%3%,'%4%'),") % char_index % rankBylevel % rankByjob[job] % m_refreshDate);

			++rankBylevel;
			++rankByjob[job];
		}

		if (count > 0)
		{
			int index = tstr.rfind(",");
			tstr.erase(index);

			cmdrank.SetQuery(tstr);
			if ( !cmdrank.Update() )
			{
				GAMELOG << init("SYSTEM_RANKING_EX ERROR: RankingEX_RefreshRankByLevel()_2") << end;
				return false;
			}
		}
	}

	return true;
}

void RankingRefreshEx(void)
{
	LOG_INFO("Ranking system - refresh start");
	if (gserver.m_RankingSystem.RankingEX_Refresh() == false)
	{
		LOG_ERROR("Ranking system error");

		mysql_close(&gserver.m_RankingSystem.m_dbrank);
		gserver.m_RankingSystem.SetLock(false);
		return;
	}

	mysql_close(&gserver.m_RankingSystem.m_dbrank);

	gserver.m_RankingSystem.SetLock(false);
	CNetMsg::SP msg(new CNetMsg);
	SubHelperRankingEXRefreshRep(msg);
	gserver.SendToAllGameServer(msg);
	gserver.m_RankingSystem.SetExistReward(true);

	LOG_INFO("Ranking system - refresh end");
}

void CRankingSystemEX::StartRefresh(void)
{
	if (this->IsLock())
		return;

	this->SetLock(true);

	if (ConnectDB() == false)
	{
		this->SetLock(false);
		return;
	}

	boost::thread th(RankingRefreshEx);
}

// 매 1 시간마다 불리워짐
void CRankingSystemEX::CheckRankingRefreshTime(void)
{
	struct tm ti = NOW();

	// 매주 월요일 06시
	if (IsLock() == false && ti.tm_wday == RANKING_REFRESH_DAY && ti.tm_hour == RANKING_REFRESH_HOUR)
	{
		LOG_INFO("Ranking system - refresh by timer");
		StartRefresh();
	}
}

std::string CRankingSystemEX::GetRefreshTime(void)
{
	return m_refreshDate;
}

int CRankingSystemEX::GetRankingByName(int type, int subtype, CLCString name)
{
	CLCString sql(1024);
	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);

	char tname[128];
	mysql_real_escape_string(&gserver.m_dbchar, tname, (const char *)name, name.Length());

	// t_characters에서 캐릭터 이름을 찾는다.
	int charIndex = 0;
	sql.Format("SELECT a_index FROM t_characters WHERE a_nick='%s' LIMIT 1", tname);
	cmd.SetQuery(sql);
	if (cmd.Open() == false || cmd.MoveFirst() == false)
	{
		return -1;
	}
	cmd.GetRec("a_index", charIndex);

	switch ( type )
	{
	case RANKTYPE_LEVELMASTER:
		{
			sql.Format("SELECT a_rank_by_level FROM t_ranking_ex WHERE a_char_index = %d", charIndex);
		}
		break;
	case RANKTYPE_JOBRANKER:
		{
			sql.Format("SELECT a_rank_by_job FROM t_ranking_ex WHERE a_char_index = %d", charIndex);
		}
		break;
	default:
		return -1;
	}

	cmd.SetQuery(sql);
	if ( !cmd.Open() || !cmd.MoveFirst() )
	{
		return -1;
	}

	int rank;
	cmd.GetRec(0, rank);

	return rank;
}

int CRankingSystemEX::GetRankerCount(int type, int subtype)
{
	CLCString sql(1024);
	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);

	switch ( type )
	{
	case RANKTYPE_LEVELMASTER:
		{
			sql.Format("SELECT a_rank_by_level FROM t_ranking_ex ORDER BY a_rank_by_level DESC LIMIT 1");
		}
		break;
	case RANKTYPE_JOBRANKER:
		{
			sql.Format("SELECT RANK.a_rank_by_job FROM t_ranking_data AS DATA, t_ranking_ex AS RANK "
					   "WHERE DATA.a_job = %d AND DATA.a_char_index = RANK.a_char_index ORDER BY RANK.a_char_index DESC LIMIT 1", subtype);
		}
		break;
	default:
		return -1;
	}

	cmd.SetQuery(sql);
	if ( !cmd.Open() || !cmd.MoveFirst() )
	{
		return -1;
	}

	int count;
	cmd.GetRec(0, count);

	return count;
}
void CRankingSystemEX::RankingEX_TOP50List(CNetMsg::SP& msg, CDescriptor* dest, int type, int subtype)
{
	int char_index;
	RefMsg(msg) >> char_index;

	CLCString sql(1024);
	CDBCmd cmdchar, cmdrank;
	cmdchar.Init(&gserver.m_dbchar);
	cmdrank.Init(&gserver.m_dbchar);

	// 이전 랭킹 갱신 날짜 가져오기
	std::string refresh_date = GetPreviousRefreshDate();

	// 타입에 따른 쿼리 추가 조건, ranking 필드, value 필드 설정
	CLCString ranking_field(20), value_field(20), add_condition;
	switch ( type )
	{
	case RANKTYPE_LEVELMASTER:
		{
			ranking_field.Format("a_rank_by_level");
			value_field.Format("a_exp");
			add_condition.Format("");
		}
		break;
	case RANKTYPE_JOBRANKER:
		{
			ranking_field.Format("a_rank_by_job");
			value_field.Format("a_exp");
			add_condition.Format("DATA.a_job = %d AND", subtype);
		}
		break;
	default:
		return;
	}
	sql.Format("SELECT CH.a_index, CH.a_nick, RANK.%s, DATA.%s, DATA.a_level, DATA.a_job, DATA.a_levelup_date FROM t_characters AS CH, t_ranking_ex AS RANK, t_ranking_data AS DATA "
			   "WHERE %s CH.a_index = RANK.a_char_index and CH.a_index = DATA.a_char_index ORDER BY RANK.%s LIMIT 50", (const char *)ranking_field, (const char *)value_field, (const char *)add_condition, (const char *)ranking_field);
	cmdchar.SetQuery(sql);
	if ( !cmdchar.Open() )
	{
		GAMELOG << init("SYSTEM_RANKING_EX ERROR: RankingEX_TOP50ByLevel()_1") << end;
		return;
	}

	// 총 리스트 숫자, 이번에 전달할 리스트 숫자(최대 10)
	const int nMaxCount = 10;
	int total_count = cmdchar.m_nrecords;
	int loop_count = total_count / nMaxCount;
	if ( total_count % nMaxCount > 0 )
		loop_count++;

	if ( total_count == 0 )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		SubHelperRankingEXError(rmsg, 0, char_index);
		SEND_Q(rmsg, dest);
		return;
	}
	// 루프돌면서 데이터 전송
	for ( int i = 0 ; i < loop_count ; ++i )
	{
		int start_count = 0;
		int list_count = nMaxCount;
		if ( total_count % nMaxCount != 0 && loop_count -1 == i )
		{
			list_count = total_count % nMaxCount;
		}

		CNetMsg::SP rmsg(new CNetMsg);

		SubHelperRankingEXListRep(rmsg, MSG_SUBHELPER_RANKING_EX_LIST, type, subtype, char_index);

		RefMsg(rmsg) << total_count
					 << list_count;

		while ( cmdchar.MoveNext() )
		{
			CRankerData rankerData;
			start_count++;
			cmdchar.GetRec("a_index", rankerData.m_char_index);
			cmdchar.GetRec("a_nick", rankerData.m_name);
			cmdchar.GetRec((const char *)ranking_field, rankerData.m_rank);
			cmdchar.GetRec((const char *)value_field, rankerData.m_value);
			cmdchar.GetRec("a_level", rankerData.m_level);
			cmdchar.GetRec("a_job", rankerData.m_job);
			cmdchar.GetRec("a_levelup_date", rankerData.m_date);

			CLCString cmpDate;
			cmpDate.Format("0000-00-00 00:00:00");
			if ( strcmp((const char *)rankerData.m_date, (const char *)cmpDate) == 0 )
			{
				rankerData.m_date = "";
			}
			// 전주 데이터 구하기 (ranking, value)
			sql.Format("SELECT RANK.%s, DATA.%s FROM t_ranking_ex_backup AS RANK, t_ranking_data_backup AS DATA WHERE DATA.a_refresh_date = '%s' and RANK.a_refresh_date = '%s' and "
					   "RANK.a_char_index = %d and DATA.a_char_index = %d", (const char *)ranking_field, (const char *)value_field, refresh_date.c_str(), refresh_date.c_str(), rankerData.m_char_index, rankerData.m_char_index);
			cmdrank.SetQuery(sql);
			if ( !cmdrank.Open() )
			{
				GAMELOG << init("SYSTEM_RANKING_EX ERROR: RankingEX_TOP50ByLevel()_3") << end;
			}
			if ( cmdrank.MoveFirst() )
			{
				cmdrank.GetRec((const char *)ranking_field, rankerData.m_rank_pre);
				cmdrank.GetRec((const char *)value_field, rankerData.m_value_pre);
			}

			RefMsg(rmsg) << rankerData.m_name
						 << rankerData.m_rank
						 << rankerData.m_rank_pre
						 << rankerData.m_value
						 << rankerData.m_value_pre
						 << rankerData.m_level
						 << (unsigned char)rankerData.m_job
						 << rankerData.m_date;

			if ( start_count % nMaxCount == 0 )
				break;
		}

		SEND_Q(rmsg, dest);
	}
}

void CRankingSystemEX::RankingEX_SearchList(CNetMsg::SP& msg, CDescriptor* dest, int type, int subtype)
{
	CLCString name(20);
	int char_index;
	RefMsg(msg) >> char_index >> name;

	CLCString sql(1024), refresh_date(20);

	CDBCmd cmdchar, cmdrank;
	cmdchar.Init(&gserver.m_dbchar);
	cmdrank.Init(&gserver.m_dbchar);

	// 이전 랭킹 날짜 얻기
	refresh_date = GetPreviousRefreshDate().c_str();

	// 타입에 따른 쿼리 추가 조건, ranking 필드, value 필드 설정
	CLCString ranking_field(20), value_field(20), add_condition;
	switch ( type )
	{
	case RANKTYPE_LEVELMASTER:
		{
			ranking_field.Format("a_rank_by_level");
			value_field.Format("a_exp");
			add_condition.Format("");
		}
		break;
	case RANKTYPE_JOBRANKER:
		{
			ranking_field.Format("a_rank_by_job");
			value_field.Format("a_exp");
			add_condition.Format("DATA.a_job = %d AND", subtype);
		}
		break;
	default:
		return;
	}

	// 찾고자 하는 캐릭터의 랭킹 구하기
	int user_rank = GetRankingByName(type, subtype, name);
	if ( user_rank < 0 )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		SubHelperRankingEXError(rmsg, MSG_EX_RANKING_SYSTEM_ERROR_NOT_EXIST, char_index);
		SEND_Q(rmsg, dest);
		return;
	}

	// 위, 아래 5명씩 총 11명(자신포함) 정보 구하기
	const int nMaxCount = 11;
	int list_count = nMaxCount;
	int start_rank = user_rank - 6;
	if ( start_rank < 0 )
	{
		// start_rank가 마이너스이면 보내는 리스트 수가 적어진다
		list_count -= abs(start_rank);
		start_rank = 0;
	}

	sql.Format("SELECT CH.a_index, CH.a_nick, RANK.%s, DATA.%s, DATA.a_level, DATA.a_job, DATA.a_levelup_date FROM t_characters AS CH, t_ranking_ex AS RANK, t_ranking_data AS DATA "
			   "WHERE %s CH.a_index = RANK.a_char_index and CH.a_index = DATA.a_char_index ORDER BY RANK.%s LIMIT %d, %d"
			   , (const char *)ranking_field, (const char *)value_field, (const char *)add_condition, (const char *)ranking_field, start_rank, list_count);
	cmdchar.SetQuery(sql);
	if ( !cmdchar.Open() )
	{
		GAMELOG << init("SYSTEM_RANKING_EX ERROR: RankingEX_SearchList()_1") << end;
		return;
	}

	CNetMsg::SP rmsg(new CNetMsg);
	SubHelperRankingEXListRep (rmsg, MSG_SUBHELPER_RANKING_EX_SEARCH, type, subtype, char_index);
	RefMsg(rmsg) << cmdchar.m_nrecords;

	while( cmdchar.MoveNext() )
	{
		CRankerData rankerData;
		cmdchar.GetRec("a_index", rankerData.m_char_index);
		cmdchar.GetRec("a_nick", rankerData.m_name);
		cmdchar.GetRec((const char *)ranking_field, rankerData.m_rank);
		cmdchar.GetRec((const char *)value_field, rankerData.m_value);
		cmdchar.GetRec("a_level", rankerData.m_level);
		cmdchar.GetRec("a_job", rankerData.m_job);
		cmdchar.GetRec("a_levelup_date", rankerData.m_date);

		CLCString cmpDate;
		cmpDate.Format("0000-00-00 00:00:00");
		if ( strcmp((const char *)rankerData.m_date, (const char *)cmpDate) == 0 )
		{
			rankerData.m_date = "";
		}

		sql.Format("SELECT RANK.%s, DATA.%s FROM t_ranking_ex_backup AS RANK, t_ranking_data_backup AS DATA WHERE DATA.a_refresh_date = '%s' and RANK.a_refresh_date = '%s' and "
				   "RANK.a_char_index = %d and DATA.a_char_index = %d", (const char *)ranking_field, (const char *)value_field, (const char *)refresh_date, (const char *)refresh_date, rankerData.m_char_index, rankerData.m_char_index);
		cmdrank.SetQuery(sql);
		if ( !cmdrank.Open() )
		{
			GAMELOG << init("SYSTEM_RANKING_EX ERROR: RankingEX_SearchList()_2") << end;
		}
		if ( cmdrank.MoveFirst() )
		{
			cmdrank.GetRec((const char *)ranking_field, rankerData.m_rank_pre);
			cmdrank.GetRec((const char *)value_field, rankerData.m_value_pre);
		}

		RefMsg(rmsg) << rankerData.m_name
					 << rankerData.m_rank
					 << rankerData.m_rank_pre
					 << rankerData.m_value
					 << rankerData.m_value_pre
					 << rankerData.m_level
					 << (unsigned char)rankerData.m_job
					 << rankerData.m_date;
	}
	SEND_Q(rmsg, dest);
}

void CRankingSystemEX::RankingEX_GiveReward(int endTime)
{
	RankingEX_GiveRewardByType(RANKTYPE_LEVELMASTER, 0, endTime);

	for ( int i=0 ; i<JOBCOUNT ; ++i )
		RankingEX_GiveRewardByType(RANKTYPE_JOBRANKER, i, endTime);

	SetExistReward(false);
}

void CRankingSystemEX::RankingEX_GiveRewardByType(int type, int subtype, int endTime)
{
	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);

	int title_index = RankingEX_GetRewardTitle(type, subtype);

	CLCString sql(1024);
	switch ( type )
	{
	case RANKTYPE_LEVELMASTER:
		{
			sql.Format("SELECT a_char_index FROM t_ranking_ex WHERE a_rank_by_level = 1");
		}
		break;
	case RANKTYPE_JOBRANKER:
		{
			sql.Format("SELECT RANK.a_char_index FROM t_ranking_ex AS RANK, t_ranking_data AS DATA WHERE DATA.a_job = %d and RANK.a_rank_by_job = 1 and DATA.a_char_index = RANK.a_char_index", subtype);
		}
		break;
	}
	cmd.SetQuery(sql);
	if ( !cmd.Open() || !cmd.MoveFirst() )
	{
		GAMELOG << init("SYSTEM_RANKING_EX ERROR: RankingEX_GiveRewardByType()_1") << end;
		return;
	}

	int char_index;
	cmd.GetRec("a_char_index", char_index);

	// 타이틀 리스트에 charIndex, titleIndex 가 있는지 확인
	sql.Format("SELECT a_char_index FROM t_titlelist WHERE a_char_index=%d AND a_title_index=%d LIMIT 1", char_index, title_index);
	cmd.SetQuery(sql);
	if( cmd.Open() )
	{
		if( cmd.MoveNext() )
		{
			// 있으면 시간 변경
			sql.Format("UPDATE t_titlelist SET a_endtime=%d WHERE a_char_index=%d AND a_title_index=%d", endTime, char_index, title_index);
			cmd.Init(&gserver.m_dbchar);
			cmd.SetQuery(sql);
			if( !cmd.Update() )
			{
				GAMELOG << init("SYSTEM_RANKING_EX ERROR: RankingEX_GiveRewardByType()_2")
						<< "charindex"
						<< char_index
						<< "titleindex"
						<< title_index << end;
			}

			CNetMsg::SP rmsg(new CNetMsg);
			do_Command_TitleSystem_ForceChangeTime(rmsg, char_index, title_index, endTime);
			gserver.SendToAllGameServer(rmsg);
		}
		else
		{
			int temp = 0;
			// 없으면 title 에 추가
			CNetMsg::SP rmsg(new CNetMsg);
			rmsg->Init(MSG_SUBHELPER_COMMAND);
			RefMsg(rmsg) << MSG_SUBHELPER_TITLESYSTEM_INSERT_TITLE_REQ << char_index << title_index << endTime << -1 << -1;
			// 아래의 함수에서 SEND 처리까지 다 한다.
			RefMsg(rmsg).MoveFirst();
			RefMsg(rmsg) >> temp;
			do_Command_TitleSystem_InsertTitleReq(rmsg, NULL, true);
		}
	}
}

int CRankingSystemEX::RankingEX_GetRewardTitle(int type, int subtype)
{
	int RewardByLevel = 24;
#ifdef EX_MAGE
	int RewardByJob[JOBCOUNT] = {25, 26, 27, 28, 29, 30, 31, 97, 100};	//EX_MAGE 초기 데이터 수정요망
#else
#if defined (EX_ROGUE)
	int RewardByJob[JOBCOUNT] = {25, 26, 27, 28, 29, 30, 31, 97};
#else
	int RewardByJob[JOBCOUNT] = {25, 26, 27, 28, 29, 30, 31};
#endif
#endif

	switch ( type )
	{
	case RANKTYPE_LEVELMASTER:
		return RewardByLevel;
	case RANKTYPE_JOBRANKER:
		return RewardByJob[subtype];
	}

	return -1;
}

std::string CRankingSystemEX::GetPreviousRefreshDate( void )
{
	std::string return_string = "";
	CLCString sql(1024);
	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);

	sql.Format("SELECT a_date FROM t_ranking_date ORDER BY a_date DESC LIMIT 1, 1");
	cmd.SetQuery(sql);
	if ( !cmd.Open() || !cmd.MoveFirst() )
	{
		GAMELOG << init("SYSTEM_RANKING_EX ERROR: GetPreviousRefreshDate()") << end;
	}
	else
	{
		cmd.GetRec("a_date", return_string);
	}

	return return_string;
}

bool CRankingSystemEX::TestConnectDB()
{
	if (ConnectDB() == false)
		return false;

	mysql_close(&m_dbrank);
	return true;
}

bool CRankingSystemEX::ConnectDB()
{
	if (!mysql_real_connect (
				&m_dbrank,
				gserver.m_config.Find("Char DB", "IP"),
				gserver.m_config.Find("Char DB", "User"),
				gserver.m_config.Find("Char DB", "Password"),
				gserver.m_config.Find("Char DB", "DBName"),
				0, NULL, 0))
	{
		LOG_ERROR("Can't connect rank DB : ip[%s] id[%s] pw[%s] dbname[%s] error[%s]",
				  gserver.m_config.Find("Char DB", "IP"),
				  gserver.m_config.Find("Char DB", "User"),
				  gserver.m_config.Find("Char DB", "Password"),
				  gserver.m_config.Find("Char DB", "DBName"),
				  mysql_error(&m_dbrank)
				 );

		return false;
	}

	return true;
}

