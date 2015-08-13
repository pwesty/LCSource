#include "stdhdrs.h"

#include "../ShareLib/bnf.h"
#include "Server.h"
#include "doFunc.h"
#include "CmdMsg.h"
#include "../ShareLib/DBCmd.h"
#include "Log.h"
#include "../ShareLib/packetType/ptype_tradeAgent_system.h"
#include "../ShareLib/PrintExcuteInfo.h"

void do_Command(CNetMsg::SP& msg, CDescriptor* dest)
{
	int subtype = -1;
	msg->MoveFirst();
	RefMsg(msg) >> subtype;

	switch (subtype)
	{
	case MSG_SUBHELPER_SHUTDOWN: //XX endgame - 4 end
		{
			PrintExcuteInfo::Instance()->SetStopMessage("shutdown");
			bnf::instance()->Stop();
			gserver.m_bshutdown = true;
		}
		break;
	case MSG_SUBHELPER_TITLESYSTEM_INSERT_TITLE_REQ:
		do_Command_TitleSystem_InsertTitleReq(msg, dest);
		break;
	case MSG_SUBHELPER_TITLESYSTEM_CHECK_EXPIRE_TITLE_REQ:
		do_Command_TitleSystem_DisableReq( msg, dest );
		break;
	case MSG_SUBHELPER_TITLE_SYSTEM_TITLE_DELETE_REQ:
		do_Command_TitleSystem_DeleteReq( msg, dest );
		break;
	case MSG_SUBHELPER_TITLE_SYSTEM_TITLE_AUTO_DELETE:
		do_Command_TitleSystem_AutoDelete( msg, dest );
		break;
	case MSG_SUBHELPER_LCBALL_INFO :
		do_Command_LCBallinfo( msg, dest );
		break;
	case MSG_SUBHELPER_LCBALL_USE :
		do_Command_LCBalluse( msg, dest );
		break;
	case MSG_SUBHELPER_LCBALL_USE_SUCCESS :
		do_Command_LCBallsuccess( msg, dest );
		break;
	case MSG_SUBHELPER_LCBALL_RESET:
		do_Command_LCBallReset( msg, dest );
		break;

	case MSG_SUBHELPER_LEVELUP:
		{
			int char_index;
			RefMsg(msg) >> char_index;
			if ( char_index <= 0 )
				return;

			CDBCmd cmd;
			cmd.Init(&gserver.m_dbchar);
			CLCString sql(1024);
			sql.Format("UPDATE t_characters SET a_levelup_date= NOW() WHERE a_index = %d", char_index);
			cmd.SetQuery(sql);
			cmd.Update();
		}
		break;
	case MSG_SUBHELPER_RANKING_EX_REFRESH:
		do_Command_RankingEX_Refresh( msg, dest );
		break;
	case MSG_SUBHELPER_RANKING_EX_LIST:
		do_Command_RankingEX_ListReq( msg, dest );
		break;
	case MSG_SUBHELPER_RANKING_EX_SEARCH:
		do_Command_RankingEX_SearchReq( msg, dest );
		break;
	case MSG_SUBHELPER_RANKING_EX_REWARD:
		do_Command_RankingEx_RewardReq( msg, dest );
		break;

#ifdef REFORM_PK_PENALTY_201108
	case MSG_SUBHELPER_PKPENALTY_REFORM:
		do_CommandPKPenalryReform(msg, dest);
		break;
#endif
	case MSG_SUBHELPER_EXT_CHAR_SLOT_ITEM_USE_REQ:
		do_CommandExtCharacterSlotItemUse(msg, dest);
		break;
	case MSG_SUBHELPER_EXT_CHAR_SLOT_ITEM_USE_ROLLBACK:
		do_CommandExtCharacterSlotItemUseResult(msg, dest);
		break;

	default:
		break;
	}
}

void do_Command_TitleSystem_InsertTitleReq(CNetMsg::SP& msg, CDescriptor* dest, bool force)
{
	CNetMsg::SP rmsg(new CNetMsg);
	int char_index = 0;
	int title_index = 0;
	int endtime = -1;
	int tab;
	int invenIndex;
	CLCString serial;

	RefMsg(msg) >> char_index >> title_index >> endtime >> tab >> invenIndex >> serial;

	CLCString sql(1024);
	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);
////////////////////////////////////////
//SUBHELPER 부하시 클라이언트에서 여러번의 메세지가 올경우 DB에 똑같은 데이터가 쌓인다.
	sql.Format("SELECT a_char_index FROM t_titlelist where a_char_index = %d and a_title_index = %d limit 1", char_index, title_index);
	cmd.SetQuery(sql);
	if (!cmd.Open())
		return ;
	else
	{
		if(cmd.m_nrecords > 0)
			return ;
	}
////////////////////////////////////////
	sql.Format("INSERT DELAYED INTO t_titlelist(a_char_index, a_title_index, a_endtime) values(%d, %d, %d)", char_index, title_index, endtime);
	GAMELOG << init("DB INSERT TO TITLE LIST") << delim
			<< "CHAR_INDEX" << delim << char_index << delim
			<< "TITLE_INDEX" << delim << title_index << end;
	cmd.SetQuery(sql);
	if(!cmd.Update())
	{
		do_Command_TitleSystem_InsertTitleRep(rmsg, char_index, title_index, endtime, 0, tab, invenIndex, serial);
	}
	else
	{
		if(!force)
		{
			do_Command_TitleSystem_InsertTitleRep(rmsg, char_index, title_index, endtime, 1, tab, invenIndex, serial);
		}
		else
		{
			// [selo: 101109] errortype 2 는 강제 적용
			// 강제 적용시에서는 dest=NULL 을 해주어 SEND_Q 에서 버그나 문제가 생길 부분을 없앤다.
			// 모든 게임 서버에 전달한다.
			do_Command_TitleSystem_InsertTitleRep(rmsg, char_index, title_index, endtime, 2, tab, invenIndex, serial);
			gserver.SendToAllGameServer(rmsg);
		}
	}
	SEND_Q(rmsg, dest);
}

void do_Command_TitleSystem_DisableReq(CNetMsg::SP& msg, CDescriptor* dest)
{
	CNetMsg::SP rmsg(new CNetMsg);
	int count = 0;
	int char_index = 0;
	int title_index = 0;
	CLCString sql(1024);
	RefMsg(msg) >> char_index;
	RefMsg(msg) >> count;
	do_Command_TitleSystem_DisableRep(rmsg, char_index, count);
	if (count == 0)
	{
		SEND_Q(rmsg, dest);
		return ;
	}

	for(int i = 0; i < count; i++)
	{
		RefMsg(msg) >> title_index;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		sql.Format("DELETE from t_titlelist where a_char_index = %d and a_title_index = %d", char_index, title_index);
		CDBCmd cmd;
		cmd.Init(&gserver.m_dbchar);
		cmd.SetQuery(sql);
		if( cmd.Update() )
		{
			GAMELOG << init("DELETE TITLE") << delim
					<< "CHAR INDEX" << delim << char_index << delim
					<< "TITLE INDEX" << delim << title_index << end;
			RefMsg(rmsg) << title_index;
		}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	}
	SEND_Q(rmsg, dest);
}

void do_Command_TitleSystem_DeleteReq(CNetMsg::SP& msg, CDescriptor* dest)
{
	int char_index;
	int title_index;
	CLCString sql(1024);
	CDBCmd cmd;
	CNetMsg::SP rmsg(new CNetMsg);
	RefMsg(msg) >> char_index >> title_index;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	sql.Format("DELETE from t_titlelist where a_char_index = %d and a_title_index = %d", char_index, title_index);
	cmd.Init(&gserver.m_dbchar);
	cmd.SetQuery(sql);
	if(cmd.Update())
	{
		GAMELOG << init("DELETE TITLE") << delim
				<< "CHAR INDEX" << delim << char_index << delim
				<< "TITLE INDEX" << delim << title_index << end;
		do_Command_TitleSystem_DeleteRep(rmsg, char_index, title_index, 1);
	}
	else
	{
		GAMELOG << init("CANT DELETE TITLE") << delim
				<< "CHAR INDEX" << delim << char_index << delim
				<< "TITLE INDEX" << delim << title_index << end;
		do_Command_TitleSystem_DeleteRep(rmsg, char_index, title_index, 0);
	}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	SEND_Q(rmsg, dest);
}

void do_Command_TitleSystem_AutoDelete(CNetMsg::SP& msg, CDescriptor* dest)
{
	int char_index;
	int title_index;
	CLCString sql(1024);
	CDBCmd cmd;
	RefMsg(msg) >> char_index >> title_index;
	sql.Format("DELETE from t_titlelist where a_char_index = %d and a_title_index = %d", char_index, title_index);
	cmd.Init(&gserver.m_dbchar);
	cmd.SetQuery(sql);
	if(cmd.Update())
	{
		GAMELOG << init("DELETE TITLE") << delim
				<< "CHAR INDEX" << delim << char_index << delim
				<< "TITLE INDEX" << delim << title_index << end;
	}
	else
	{
		GAMELOG << init("CANT DELETE TITLE") << delim
				<< "CHAR INDEX" << delim << char_index << delim
				<< "TITLE INDEX" << delim << title_index << end;
	}
}

void do_Command_LCBallinfo( CNetMsg::SP& msg, CDescriptor* dest )
{
	int charIndex = 0, coinIndex = 0;
	int nItemIndex=0, nItemCount=0, nRemain = 0, nMax = 0;
	char courseIndex = 0;
	CLCString	sql(1024);

	CDBCmd		cmd, cmd2;

	cmd.Init( &gserver.m_dbchar );
	cmd2.Init( &gserver.m_dbchar );

	RefMsg(msg) >> charIndex >> coinIndex >> courseIndex ;

	// 여기서부터 메시지를 만들자
	CNetMsg::SP	rmsg(new CNetMsg);
	rmsg->Init(MSG_SUBHELPER_COMMAND);
	RefMsg(rmsg) << MSG_SUBHELPER_LCBALL_INFO
				 << charIndex;

	// 토큰 정보 가져오기
	cmd.SetQuery("select a_tocken_index from t_lcball group by a_tocken_index order by a_item_order;");
	if( !cmd.Open() )
	{
		return;
	}
	int coinCount = cmd.m_nrecords ;
	RefMsg(rmsg) << (char) coinCount;
	while( cmd.MoveNext() )
	{
		int nCoinIndex = 0;
		cmd.GetRec("a_tocken_index", nCoinIndex );
		if( coinIndex == 0 )			// 요청한 코인이 없으면 최초 검색된 코인으로 변경
			coinIndex = nCoinIndex;
		RefMsg(rmsg) << nCoinIndex;
	}

	// 각 코스별 정보 가져오기
	sql.Format(" SELECT a_course_code, sum(a_item_max) as max, sum(a_item_remain) as remain "
			   " FROM t_lcball WHERE a_tocken_index=%d GROUP BY a_tocken_index , a_course_code ORDER BY a_course_code ;" , coinIndex );
	cmd.SetQuery( sql );
	if( !cmd.Open() )
		return;

	RefMsg(rmsg) << (char) cmd.m_nrecords;		// 코스 갯수
	while( cmd.MoveNext() )
	{
		char	curCourse =0;
		int		courseItem=0;
		unsigned int courseItemCount=0;

		cmd.GetRec("a_course_code", curCourse );
		//if( courseIndex == 0)		// 요청한 정보가 0 이면 최초 검색된 코스로 변경
		//	courseIndex = curCourse;
		sql.Format( "SELECT a_item_index , a_item_count FROM t_lcball WHERE a_tocken_index=%d AND a_course_code=%d AND a_order=0", coinIndex, curCourse );
		cmd2.SetQuery( sql);
		if( cmd2.Open() && cmd2.MoveFirst() )
		{
			cmd2.GetRec( "a_item_index", courseItem );
			cmd2.GetRec("a_item_count", courseItemCount );
		}
		nRemain = 0;
		nMax = 0;
		cmd.GetRec("max", nMax );
		cmd.GetRec("remain", nRemain );
		RefMsg(rmsg) << courseItem << courseItemCount << nRemain << nMax;
	}
	// 현재 코인 과 코스 인덱스
	RefMsg(rmsg) << coinIndex << courseIndex;

	// 요청한 코스 정보
	sql.Format(" SELECT a_item_index, a_item_count, a_item_max, a_item_remain FROM t_lcball WHERE a_tocken_index = %d AND a_course_code = %d ORDER BY a_order " , coinIndex, courseIndex );
	cmd.SetQuery( sql );
	if( !cmd.Open() )
		return;

	RefMsg(rmsg) << (char) cmd.m_nrecords;		// 코스 갯수
	while( cmd.MoveNext() )
	{
		nItemIndex = nItemCount = nRemain = nMax = 0;

		cmd.GetRec("a_item_index", nItemIndex );
		cmd.GetRec("a_item_count", nItemCount );
		cmd.GetRec("a_item_max", nMax );
		cmd.GetRec("a_item_remain", nRemain );
		RefMsg(rmsg) << nItemIndex << nItemCount << nRemain << nMax;
	}
	// 정보 만들어서 돌려보내기
	SEND_Q( rmsg, dest );
	return;
}

void do_Command_LCBalluse( CNetMsg::SP& msg, CDescriptor* dest )
{
	CLCString	sql(1024);
	CDBCmd		cmd;
	cmd.Init( &gserver.m_dbchar );

	int		charIndex=0, coinIndex=0 ;
	char	courseIndex = 0;
	RefMsg(msg) >> charIndex >> coinIndex >> courseIndex ;

	// 지급아이템 결정
	struct COURSEITEM
	{
		int itemIndex;
		unsigned int itemCount;
		int remainCnt;
	};

	sql.Format(" SELECT a_item_index, a_item_count, a_item_remain FROM t_lcball WHERE a_tocken_index = %d AND a_course_code = %d ORDER BY a_order " , coinIndex, courseIndex );
	cmd.SetQuery( sql );
	if( !cmd.Open() )
		return;

	COURSEITEM * pItem = new COURSEITEM [cmd.m_nrecords];
	int totalRemain = 0 , idx=0;
	while( cmd.MoveNext() )
	{
		cmd.GetRec("a_item_index", pItem[idx].itemIndex );
		cmd.GetRec("a_item_count", pItem[idx].itemCount );
		cmd.GetRec("a_item_remain", pItem[idx].remainCnt );
		totalRemain += pItem[idx].remainCnt;
		++idx;
	}

	int rnd = GetRandom( 0, totalRemain -1 );
	int getIdx = 0;
	for( idx=0; idx<cmd.m_nrecords ; idx++ )
	{
		rnd -= pItem[idx].remainCnt;
		if ( rnd <= 0 )
		{
			getIdx = idx ;
			break;
		}
	}

	// DB LOCK
	CNetMsg::SP rmsg(new CNetMsg);
	cmd.BeginTrans();

	// UPDATE
	sql.Format("UPDATE t_lcball SET a_item_remain = a_item_remain -1 WHERE a_tocken_index = %d and a_course_code = %d and a_order=%d and a_item_index = %d; " , coinIndex, courseIndex, getIdx, pItem[getIdx].itemIndex );
	cmd.SetQuery( sql );
	if(!cmd.Update() )
	{
		cmd.Rollback();
		// DB UPDATE FAIL
		SubHelperLCBallUseMsg( rmsg, charIndex, -1 , -1, -1, -1 , 0  );
	}
	else
	{
		cmd.Commit();
		SubHelperLCBallUseMsg( rmsg, charIndex, coinIndex , courseIndex, (char)getIdx, pItem[getIdx].itemIndex , pItem[getIdx].itemCount  );
	}
	SEND_Q( rmsg, dest );

	delete[] pItem;

	return;
}

void do_Command_LCBallsuccess( CNetMsg::SP& msg, CDescriptor* dest )
{
	CLCString	sql(1024);
	CDBCmd		cmd;
	cmd.Init( &gserver.m_dbchar );

	int		coinIndex=0 ;
	char	courseIndex=0, errorCode=0 , cOrder=-1;

	RefMsg(msg) >> errorCode;
	RefMsg(msg) >> coinIndex >> courseIndex >> cOrder ;

	if( errorCode )
	{
		// DB ROLLBACK
		sql.Format("UPDATE t_lcball SET a_item_remain = a_item_remain +1 WHERE a_tocken_index = %d and a_course_code = %d and a_order=%d ; ", coinIndex, courseIndex, cOrder );
		cmd.SetQuery( sql );
		cmd.Update();
		return;
	}

	CLCString	name(MAX_CHAR_NAME_LENGTH + 1);
	int			itemIndex;
	RefMsg(msg) >>	name >> itemIndex;

	// 메인아이템인경우
	if( cOrder == 0 )
	{
		// 서버 해당아이템 reflash
		sql.Format("UPDATE t_lcball SET a_item_remain = a_item_max WHERE a_tocken_index=%d and a_course_code=%d ", coinIndex, courseIndex );
		cmd.SetQuery(sql);
		cmd.Update();

		// 전서버 MSG 전달
		CNetMsg::SP rmsg(new CNetMsg);
		SubHelperLCBallSuccessMsg( rmsg, name, itemIndex );
		gserver.SendToAllGameServer( rmsg );
	}
//	}
	return;
}

void do_Command_LCBallReset( CNetMsg::SP& msg, CDescriptor* dest )
{
	int coinIndex =0 ;
	char courseIndex = 0;

	CLCString	sql(1024);
	CDBCmd		cmd;
	cmd.Init( &gserver.m_dbchar );
	RefMsg(msg) >> coinIndex >> courseIndex;

	cmd.BeginTrans();
	if( coinIndex > 0 && courseIndex >= 0 )
	{
		sql.Format("UPDATE t_lcball SET a_item_remain = a_item_max WHERE a_tocken_index=%d and a_course_code=%d ", coinIndex, courseIndex );
	}
	else if ( coinIndex > 0 && courseIndex < 0 ) // 코인아이템 전부 리셋
	{
		sql.Format("UPDATE t_lcball SET a_item_remain = a_item_max WHERE a_tocken_index=%d ", coinIndex );
	}
	else if ( coinIndex < 0 && courseIndex < 0 ) // 전부 리셋
	{
		sql.Format("UPDATE t_lcball SET a_item_remain = a_item_max " );
	}
	cmd.SetQuery(sql);
	if( cmd.Update() )
		cmd.Commit();
	else
		cmd.Rollback();

	return;
}

void do_Command_RankingEX_Refresh( CNetMsg::SP& msg, CDescriptor* dest )
{
	int charIdx;
	RefMsg(msg) >> charIdx;

	// 랭킹 갱신 중이면, 리턴
	if ( gserver.m_RankingSystem.IsLock() )
	{
		SubHelperRankingEXError(msg, MSG_EX_RANKING_SYSTEM_ERROR_REFRESH_TIME, charIdx);
		SEND_Q(msg, dest);
		return;
	}
	gserver.m_RankingSystem.StartRefresh();
}

void do_Command_RankingEX_ListReq( CNetMsg::SP& msg, CDescriptor* dest )
{
	unsigned char type, subtype;

	RefMsg(msg) >> type >> subtype;

	// 랭킹 갱신 중이면, 리턴
	if ( gserver.m_RankingSystem.IsLock() )
	{
		int charIdx;
		RefMsg(msg) >> charIdx;

		SubHelperRankingEXError(msg, MSG_EX_RANKING_SYSTEM_ERROR_REFRESH_TIME, charIdx);
		SEND_Q(msg, dest);
		return;
	}

	gserver.m_RankingSystem.RankingEX_TOP50List(msg, dest, type, subtype);
}
void do_Command_RankingEX_SearchReq( CNetMsg::SP& msg, CDescriptor* dest )
{
	unsigned char type, subtype;
	CLCString name(20);

	RefMsg(msg) >> type >> subtype;

	// 랭킹 갱신 중이면, 리턴
	if ( gserver.m_RankingSystem.IsLock() )
	{
		int charIdx;
		RefMsg(msg) >> charIdx;

		SubHelperRankingEXError(msg, MSG_EX_RANKING_SYSTEM_ERROR_REFRESH_TIME, charIdx);
		SEND_Q(msg, dest);
		return;
	}
	gserver.m_RankingSystem.RankingEX_SearchList(msg, dest, type, subtype);
}

void do_Command_RankingEx_RewardReq( CNetMsg::SP& msg, CDescriptor* dest )
{
	if ( gserver.m_RankingSystem.IsLock() )
		return;

	if ( !gserver.m_RankingSystem.IsExistReward() )
		return;

	int endTime = 0;
	RefMsg(msg) >> endTime;

	gserver.m_RankingSystem.RankingEX_GiveReward(endTime);
}

#ifdef REFORM_PK_PENALTY_201108
void do_CommandPKPenalryReform(CNetMsg::SP& msg, CDescriptor* dest)
{
	unsigned char subType;
	RefMsg(msg) >> subType;

	switch( subType )
	{
	case MSG_SUBHELPER_PKPENALTY_REFORM_REWARD_INFO_REQ:
		do_CommandPKPenalryReformRewardInfoReq(msg, dest);
		break;
	case MSG_SUBHELPER_PKPENALTY_REFORM_REWARD_REQ:
		do_CommandPKPenalryReformRewardReq(msg, dest);
		break;
	default:
		break;
	}
}

// 보상 정보를 보내주자.
void do_CommandPKPenalryReformRewardInfoReq(CNetMsg::SP& msg, CDescriptor* dest)
{
	int charIndex;
	CDBCmd cmd;
	RefMsg(msg) >> charIndex;

	cmd.Init(&gserver.m_dbchar);
	CLCString sql(1024);
	sql.Format("SELECT * FROM t_pkpenalty_reward WHERE a_char_idx = %d", charIndex);
	cmd.SetQuery(sql);

	int rewardFlag = 0;

	if(!cmd.Open())
	{
		GAMELOG << init("DB_ERROR") << "CAN NOT OPEN t_pkpenalty_reward table." << end;
		return ;
	}
	if(!cmd.MoveFirst())
	{
		// 기록된게 없다면 insert 해준다.
		sql.Format("INSERT INTO t_pkpenalty_reward(a_char_idx, a_reward_flag) VALUES(%d, 0 )", charIndex);
		cmd.SetQuery(sql);
		if(!cmd.Update()) // insert 가 않된다면.. 뭐 어쩌란 말이냐 ㅡㅡ;;
		{
			GAMELOG << init("DB_ERROR") << "CAN NOT INSERT t_pkpenalty_reward table." << end;
			return ;
		}
	}
	else
		cmd.GetRec("a_reward_flag", rewardFlag);

	CNetMsg::SP rmsg(new CNetMsg);
	SubHelperPKPenaltyReformRewardInfoRep( rmsg, charIndex, rewardFlag);
	SEND_Q(rmsg, dest);
}

// 보상을 해주기 위해 DB에 기록을 해보자.
void do_CommandPKPenalryReformRewardReq(CNetMsg::SP& msg, CDescriptor* dest)
{
	int charIndex;
	int rewardNum;
	int titleIndex;
	RefMsg(msg) >> charIndex
				>> rewardNum
				>> titleIndex;

	if( rewardNum < 0 || rewardNum > 5)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		SubHelperPKPenaltyReformRewardRep( rmsg, charIndex, rewardNum, titleIndex, MSG_EX_PKPENALTY_REFORM_REWARD_ERROR_FAIL );
		SEND_Q(rmsg, dest);
	}

	CDBCmd cmd;

	cmd.Init(&gserver.m_dbchar);
	CLCString sql(1024);
	sql.Format("SELECT * FROM t_pkpenalty_reward WHERE a_char_idx = %d", charIndex);
	cmd.SetQuery(sql);

	if(!cmd.Open())
	{
		GAMELOG << init("DB_ERROR") << "CAN NOT OPEN t_pkpenalty_reward table." << end;
		CNetMsg::SP rmsg(new CNetMsg);
		SubHelperPKPenaltyReformRewardRep( rmsg, charIndex, rewardNum, titleIndex, MSG_EX_PKPENALTY_REFORM_REWARD_ERROR_FAIL );
		SEND_Q(rmsg, dest);
		return ;
	}
	if(!cmd.MoveFirst())
	{
		GAMELOG << init("DB_ERROR") << "NO DATA t_pkpenalty_reward" << "CHAR INDEX" << delim << charIndex << end;
		CNetMsg::SP rmsg(new CNetMsg);
		SubHelperPKPenaltyReformRewardRep( rmsg, charIndex, rewardNum, titleIndex, MSG_EX_PKPENALTY_REFORM_REWARD_ERROR_FAIL );
		SEND_Q(rmsg, dest);
		return ;
	}

	int rewardFlag;
	cmd.GetRec("a_reward_flag", rewardFlag);

	// 이미 보상 받았는지 확인
	if( rewardFlag & ( 1 << rewardNum )  )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		SubHelperPKPenaltyReformRewardRep( rmsg, charIndex, rewardNum, titleIndex, MSG_EX_PKPENALTY_REFORM_REWARD_ERROR_ONCE );
		SEND_Q(rmsg, dest);
		return ;
	}

	// 타이틀을 밀어 넣어주자.
	sql.Format("SELECT a_char_index FROM t_titlelist where a_char_index = %d and a_title_index = %d limit 1", charIndex, titleIndex);
	cmd.SetQuery(sql);
	if (!cmd.Open() )
	{
		GAMELOG << init("DB_ERROR") << "CAN NOT OPEN t_titlelist table." << end;
		CNetMsg::SP rmsg(new CNetMsg);
		SubHelperPKPenaltyReformRewardRep( rmsg, charIndex, rewardNum, titleIndex, MSG_EX_PKPENALTY_REFORM_REWARD_ERROR_FAIL );
		SEND_Q(rmsg, dest);
		return ;
	}
	else
	{
		// 타이틀이 있는지 확인
		if(cmd.m_nrecords > 0)
		{
			GAMELOG << init("ALREADY Pkpenalty title ") << "REWARD NUM" << rewardNum << delim << end;
			CNetMsg::SP rmsg(new CNetMsg);
			SubHelperPKPenaltyReformRewardRep( rmsg, charIndex, rewardNum, titleIndex, MSG_EX_PKPENALTY_REFORM_REWARD_ERROR_ONCE );
			SEND_Q(rmsg, dest);
			// 여기에 들어오면 타이릍은 있는 지급 기록이 없는 것. rewardFlag는 기록해주자.
			rewardFlag |= ( 1 << rewardNum ) ;
			sql.Format("UPDATE t_pkpenalty_reward SET a_reward_flag=%d WHERE a_char_idx = %d", rewardFlag, charIndex);
			cmd.SetQuery(sql);
			cmd.Update();
			return;
		}
		else
		{
			// 타이틀을 밀어 넣어주자.
			sql.Format("INSERT DELAYED INTO t_titlelist(a_char_index, a_title_index, a_endtime) values(%d, %d, -1)", charIndex, titleIndex );
			cmd.SetQuery(sql);
			if( !cmd.Update() )
			{
				GAMELOG << init("DB_ERROR") << "INSERT t_titlelist table." << end;
				CNetMsg::SP rmsg(new CNetMsg);
				SubHelperPKPenaltyReformRewardRep( rmsg, charIndex, rewardNum, titleIndex, MSG_EX_PKPENALTY_REFORM_REWARD_ERROR_FAIL );
				SEND_Q(rmsg, dest);
				return ;
			}
		}
	}
	// 보상 정보를 이제 업데이트 하자
	rewardFlag |= ( 1 << rewardNum ) ;
	sql.Format("UPDATE t_pkpenalty_reward SET a_reward_flag=%d WHERE a_char_idx = %d", rewardFlag, charIndex);
	cmd.SetQuery(sql);
	if(!cmd.Update() )
	{
		GAMELOG << init("DB_ERROR") << "UPDATE t_pkpenalty_reward" << delim << "CHAR INDEX" << delim << charIndex
				<< "REWARD" << delim << rewardFlag << "REWARD NUM" << delim << rewardNum << end;
		CNetMsg::SP rmsg(new CNetMsg);
		SubHelperPKPenaltyReformRewardRep( rmsg, charIndex, rewardNum, titleIndex, MSG_EX_PKPENALTY_REFORM_REWARD_ERROR_FAIL );
		SEND_Q(rmsg, dest);
		return ;
	}

	// 여기까지 왔으면 성공.
	CNetMsg::SP rmsg(new CNetMsg);
	SubHelperPKPenaltyReformRewardRep( rmsg, charIndex, rewardNum, titleIndex, MSG_EX_PKPENALTY_REFORM_REWARD_ERROR_SUC );
	SEND_Q(rmsg, dest);
}
#endif // REFORM_PK_PENALTY_201108 // 2011-08 PK 패널티 리폼

void doInsertToExtendTable(CLCString& sql, int& _userIndex, CDBCmd& cmd, unsigned char& _ret, int& _days)
{
	sql.Format("DELETE FROM t_extendcharslot WHERE a_user_index = %d", _userIndex);

	cmd.SetQuery(sql);
	if(!cmd.Update())
	{
		GAMELOG << init("EXTEND CHARACTER SLOT QUERY ERROR") << sql << end;
	}
	else
	{
		sql.Format("INSERT INTO t_extendcharslot (a_user_index, a_use_datetime, a_end_datetime) VALUES (%d, NOW(), NOW() + INTERVAL %d day)", _userIndex, _days);
		cmd.SetQuery(sql);
		if(!cmd.Update())
		{
			GAMELOG << init("EXTEND CHARACTER SLOT QUERY ERROR") << sql << end;
		}
		else
		{
			_ret = 1;
		}
	}
}
void do_CommandExtCharacterSlotItemUse(CNetMsg::SP& msg, CDescriptor* dest)
{
	int _userIndex;
	int _charIndex;
	int _tab;
	int _invenIndex;
	CLCString _serial;
	int _days;
	unsigned int _time = 0;
	int _remainTime = 0;
	unsigned char _ret = 0; // 0: 실패, 1: 성공, 2: 잔여시간 남음

	RefMsg(msg) >> _userIndex >> _charIndex >> _tab >> _invenIndex >> _serial >> _days;

	CDBCmd cmd;

	cmd.Init(&gserver.m_dbchar);

	CLCString sql(1024);

	sql.Format("SELECT UNIX_TIMESTAMP(NOW() + INTERVAL %d DAY) AS time", _days);
	cmd.SetQuery(sql);
	if(cmd.Open() && cmd.MoveFirst())
	{
		cmd.GetRec("time", _time);
		sql.Format("SELECT UNIX_TIMESTAMP(a_end_datetime) - (UNIX_TIMESTAMP(NOW())) as remainTime FROM t_extendcharslot WHERE a_user_index = %d", _userIndex);
		cmd.SetQuery(sql);
		if(cmd.Open())
		{
			if(!cmd.MoveFirst())
			{
				doInsertToExtendTable(sql, _userIndex, cmd, _ret, _days);
			}
			else
			{
				cmd.GetRec("remainTime", _remainTime);
				if(_remainTime > 0)
				{
					_ret = 2;
				}
				else
				{
					doInsertToExtendTable(sql, _userIndex, cmd, _ret, _days);
				}
			}
		}
	}
	else
	{
		GAMELOG << init("EXTEND CHARACTER SLOT QUERY ERROR") << sql << end;
	}

	CNetMsg::SP rmsg(new CNetMsg);
	SubHelperExtCharacterSlotItemUseRep(rmsg, _ret, _userIndex, _charIndex, _tab, _invenIndex, _serial, _time);
	SEND_Q(rmsg, dest);

	return ;
}
void do_CommandExtCharacterSlotItemUseResult(CNetMsg::SP& msg, CDescriptor* dest)
{
	int _userIndex;
	RefMsg(msg) >> _userIndex;
	CDBCmd cmd;
	cmd.Init(&gserver.m_dbchar);

	CLCString sql(1024);
	sql.Format("DELETE FROM t_extendcharslot WHERE a_user_index = %d", _userIndex);
	cmd.SetQuery(sql);
	if(!cmd.Update())
	{
		GAMELOG << init("EXTEND CHARACTER SLOT QUERY ERROR") << sql << end;
	}
}
//