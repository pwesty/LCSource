#define __DB_PROCESS_SELECT_CHAR_CPP__

#include <boost/scoped_ptr.hpp>
#include <boost/format.hpp>
#include <boost/tokenizer.hpp>
#include "stdhdrs.h"

#include "Server.h"
#include "CmdMsg.h"
#include "EventProcessWithThread.h"
#include "../ShareLib/DBCmd.h"
#include "doFunc.h"

#include "DBManager.h"

//XX 캐릭터생성 3
void DBProcess::CreateChar( boost::any& argv )
{
	createchar_t data = boost::any_cast<createchar_t>(argv);
	LONGLONG seq_index = boost::tuples::get<0>(data);
	int user_index = boost::tuples::get<1>(data);
	boost::scoped_ptr<CreateCharacterInfo> pChar(boost::tuples::get<2>(data));

	int maxCharCount = MAX_CHAR_COUNT;

	{
		std::string query = boost::str(boost::format(
										   "SELECT count(a_index) AS nCount, UNIX_TIMESTAMP(NOW()) AS nowTime FROM t_characters WHERE a_user_index = %d") % pChar->m_index);
		query += " AND a_enable = 1";

		BOOST_MYSQL_RES res = char_db_.select(query);
		if (res == NULL || char_db_.getrowcount() == 0)
			return;

		MYSQL_ROW row = mysql_fetch_row(res.get());
		int _count = atoi(row[0]);
		LONGLONG _nowTime = ATOLL(row[1]);

		if(_count >= 4 && pChar->m_tExtendCharacterSlotTime <= (time_t)_nowTime)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			FailMsg(rmsg, MSG_FAIL_DB_FULL_CHAR);
			SendMessageToClient(seq_index, user_index, rmsg);
			return;
		}
	}

	{
		std::string query = boost::str(boost::format(
										   "SELECT a_index FROM t_characters WHERE a_name='%s' AND a_server=%d LIMIT 1") % pChar->m_name % gserver->m_serverno);
		BOOST_MYSQL_RES res = char_db_.select(query);
		if (res == NULL )
			return;

		if (char_db_.getrowcount() > 0)
		{
			// 캐릭터 이름 중복시에
			CNetMsg::SP rmsg(new CNetMsg);
			FailMsg(rmsg, MSG_FAIL_DB_ALREADY_NAME);
			SendMessageToClient(seq_index, user_index, rmsg);
			return;
		}
	}

	{
		std::string query = boost::str(boost::format(
										   "SELECT a_index FROM t_characters WHERE a_nick='%1%' AND a_server=%2% LIMIT 1") % pChar->m_name % gserver->m_serverno);
		BOOST_MYSQL_RES res = char_db_.select(query);
		if (res == NULL )
			return;

		if (char_db_.getrowcount() > 0)
		{
			// 캐릭터 별명 중복시에
			CNetMsg::SP rmsg(new CNetMsg);
			FailMsg(rmsg, MSG_FAIL_DB_ALREADY_NAME);
			SendMessageToClient(seq_index, user_index, rmsg);
			return;
		}
	}

	bool ret = true;

	try
	{
		bool level_100_up = false;

		{
			std::string query = boost::str(boost::format(
											   "SELECT a_index, a_level FROM t_characters WHERE a_user_index=%d AND a_server=%d AND a_enable=1")
										   % pChar->m_index % gserver->m_serverno);
			BOOST_MYSQL_RES res = char_db_.select(query);

			if (res == NULL)	// 데이터베이스 오류
				throw MSG_FAIL_DB_NEW_CHAR;

			if (char_db_.getrowcount() >= maxCharCount)	// 캐릭터 가득 존재
				throw MSG_FAIL_DB_FULL_CHAR;

			// 100 레벨이상인 캐릭터가 있다는 것을 표시
			int row_count = char_db_.getrowcount();
			for (int i = 0; i < row_count; ++i)
			{
				MYSQL_ROW row = mysql_fetch_row(res.get());
				int level = atoi(row[1]);
				if (level >= 100)
				{
					level_100_up = true;
					break;
				}
			}
		}

		char_db_.BEGIN();

		std::string query = "INSERT INTO t_characters "
							"(a_user_index, a_server, "
							"a_name, "
							"a_nick, "
							"a_was_x, a_was_z, a_was_h, a_was_r, a_was_zone, a_was_area, "
							"a_active_skill_index, a_active_skill_level, a_passive_skill_index, a_passive_skill_level, a_etc_skill_index, a_etc_skill_level, "
							"a_wearing, "
							"a_sskill, "
							"a_datestamp, a_createdate, "

							"a_phoenix, "
							"a_job, a_hair_style, a_face_style, a_str, a_dex, a_int, a_con, "
							"a_hp, a_max_hp, a_mp, a_max_mp, "
							"a_level, a_job2, a_statpt_str, a_statpt_dex, a_statpt_con, a_statpt_int, a_statpt_remain, "
							"a_etc_event, "
							"a_skill_point, "
							"a_nas) "

							"VALUES "

							"(";

		query += boost::str(boost::format("%1%,%2%,") % pChar->m_index % gserver->m_serverno);

		query += boost::str(boost::format("'%1%','%1%',") % pChar->m_name);

		query += "0.0000, 0.0000, 0.0000, 0.0000, -1, -1,";

		// 기본 스킬 주기
		// 기본 착용 정보 : 투구 상의 무기 하의 방패 장갑 신발
		switch (pChar->m_job)
		{
		case JOB_TITAN:
			query += "' 1', ' 1', '', '', '', '',";
			query += " ' 72 2 12 3 -1 4 8', ";
			break;

		case JOB_KNIGHT:
			query += "' 14', ' 1', '', '', '', '',";
			query += " ' 75 34 48 38 49 39 41', ";
			break;

		case JOB_HEALER:
			query += "' 27 31', ' 1 1', '', '', '', '',";
			query += " ' 78 26 50 28 -1 30 32', ";
			break;

		case JOB_MAGE:
			query += "' 53', ' 1', '', '', '', '',";
			query += " '24 266 356 18 -1 22 20', ";
			break;

		case JOB_ROGUE:
			query += "' 116', ' 1', '', '', '', '',";
			query += " ' 552 524 528 525 -1 527 526', ";
			break;

		case JOB_SORCERER:
			query += "' 292', ' 1', '', '', '', '',";
			query += " ' 1040 1000 988 1010 -1 1020 1030', ";
			break;

		case JOB_NIGHTSHADOW:			// 기본 스킬 , 착용장비
			query += "' 661 664 673 676 677 ', '1 1 1 1 1', ' 743 744 745 901', ' 1 1 1 1', '', '',";
			query += " ' 4539 4487 4474 4500 -1 4513 4526', ";
			break;

#ifdef EX_ROGUE
		case JOB_EX_ROGUE:
			query += "' 1528', ' 1', '', '', '', '',";
			query += " ' 9208 9209 528 9210 -1 9211 9212', ";
			break;
#endif //EX_ROGUE
#ifdef EX_MAGE
		case JOB_EX_MAGE:
			query += "' 1637', ' 1', '', '', '', '',";
			query += " '9347 9343 356 9344 -1 9345 9346', ";
			break;
#endif //EX_MAGE
		default:
			query += "'', '', '', '', '', '',";
			query += " '', ";
			break;
		}

		// a_sskill
		query += "' 1 1 5 1 9 1 13 1 18 1 23 1 44 1', now(), now(),";

		if( level_100_up )
		{
			query += "0,";
		}
		else
		{
			query += "-1,";
		}

		query += boost::str(boost::format("%1%,") % (int)pChar->m_job);
		query += boost::str(boost::format("%1%,") % (int)pChar->m_hairstyle);
		query += boost::str(boost::format("%1%,") % (int)pChar->m_facestyle);
		query += boost::str(boost::format("%1%,") % pChar->m_str);
		query += boost::str(boost::format("%1%,") % pChar->m_dex);
		query += boost::str(boost::format("%1%,") % pChar->m_int);
		query += boost::str(boost::format("%1%,") % pChar->m_con);
		query += boost::str(boost::format("%1%,") % pChar->m_dbHP);
		query += boost::str(boost::format("%1%,") % pChar->m_dbHP);
		query += boost::str(boost::format("%1%,") % pChar->m_dbMP);
		query += boost::str(boost::format("%1%,") % pChar->m_dbMP);

		query += boost::str(boost::format("%1%,") % pChar->m_level);
		query += boost::str(boost::format("%1%,") % (int)pChar->m_job2);
		query += boost::str(boost::format("%1%,") % pChar->m_statpt_str);
		query += boost::str(boost::format("%1%,") % pChar->m_statpt_dex);
		query += boost::str(boost::format("%1%,") % pChar->m_statpt_con);
		query += boost::str(boost::format("%1%,") % pChar->m_statpt_int);
		query += boost::str(boost::format("%1%,") % pChar->m_statpt_remain);

		query += boost::str(boost::format("%1%,") % pChar->m_etcEvent);

		// 초기 스킬 포인트
		int bagicSkillPoint = 25 * 10000;

#ifdef IMP_SPEED_SERVER
		if( gserver->m_bSpeedServer )
			bagicSkillPoint = 1000 * 10000;		//10,000,000 ( 1000 SP )
#endif

		query += boost::str(boost::format("%1%,") % bagicSkillPoint);

		if( pChar->m_job == JOB_NIGHTSHADOW)
		{
			query += "1000000";
		}
		else
		{
			query += "100";
		}
		query += ")";

		if (char_db_.excute(query) == false)
		{
			throw MSG_FAIL_DB_UNKNOWN;
		}

		int char_index = char_db_.insertid();
		query = boost::str(boost::format(
							   "INSERT INTO t_characters_guildpoint(a_char_index) VALUES (%1%)") % char_index);
		if (char_db_.excute(query) == false)
		{
			throw MSG_FAIL_DB_UNKNOWN;
		}
#ifdef ENABLE_TUTORIAL
		if( pChar->m_job != JOB_NIGHTSHADOW )
		{
			int questdata_index = char_index % 10;
			query = boost::str(boost::format(
								   "INSERT INTO t_questdata%02d(a_char_index,a_quest_index,a_state,a_value0,a_value1,a_value2)"
								   " VALUES(%d,%d,%d,0,0,0)") % questdata_index % char_index % 45 % QUEST_STATE_RUN);
			if (char_db_.excute(query) == false)
			{
				LOG_ERROR("DB ERROR : INSERT QUEST DATA (NEW CHAR) : charname[%s]", pChar->m_name.c_str());
			}
		}
#endif

		// 기본 아이템 주기
#ifdef NPC_PORTAL_SCROLL_DRATAN
		static const int titancount	   = 8;
		static const int knightcount   = 9;
		static const int healercount   = 8;
		static const int magecount     = 8;
		static const int roguecount    = 8;
		static const int sorcerercount = 8;
#if defined (EX_ROGUE)
		static const int exroguecount    = 8;
#endif
#ifdef EX_MAGE
		static const int exmagecount = 8;
#endif
#else // NPC_PORTAL_SCROLL_DRATAN
		static const int titancount	   = 8;
		static const int knightcount   = 9;
		static const int healercount   = 8;
		static const int magecount     = 8;
		static const int roguecount    = 8;
		static const int sorcerercount = 8;
#if defined (EX_ROGUE)
		static const int exroguecount    = 8;
#endif
#if defined (EX_ROGUE)
		static const int exmagecount    = 8;
#endif
#endif // NPC_PORTAL_SCROLL_DRATAN

#if defined(LC_BILA) || defined (LC_GAMIGO) || defined (LC_RUS) || defined (LC_USA)
#ifdef NPC_PORTAL_SCROLL_DRATAN
		static const int nightshadowcount = 9;
#else
		static const int nightshadowcount = 8;
#endif // NPC_PORTAL_SCROLL_DRATAN
#else
#ifdef NPC_PORTAL_SCROLL_DRATAN
#ifdef LC_TLD
		static const int nightshadowcount = 11;
#else
		static const int nightshadowcount = 12;
#endif
		//#endif
#else
#ifdef LC_TLD
		static const int nightshadowcount = 11;
#else // LC_TLD
		static const int nightshadowcount = 12;
#endif // LC_TLD
#endif // NPC_PORTAL_SCROLL_DRATAN
#endif

#ifdef NPC_PORTAL_SCROLL_DRATAN
		static int itemsForTitan[4][titancount] =
		{
			{ 72,  2, 12,  3,  4,  8, 4790, 6080},					// item_idx
			{  0,  1,  2,  3,  5,  6, -1, -1},					// wearpos
			{  1,  1,  1,  1,  1,  1, 1, 1}					// count
		};

		static int itemsForKnight[3][knightcount] =
		{
			{ 75, 34, 48, 38, 49, 39, 41, 4790, 6080},				// item_idx
			{  0,  1,  2,  3,  4,  5,  6, -1, -1},				// wearpos
			{  1,  1,  1,  1,  1,  1,  1, 1, 1}				// count
		};

		static int itemsForHealer[3][healercount] =
		{
			{ 78, 26, 50, 28, 30, 32, 4790, 6080},					// item_idx
			{  0,  1,  2,  3,  5,  6, -1, -1},					// wearpos
			{  1,  1,  1,  1,  1,  1, 1, 1}					// count
		};

		static int itemsForMage[3][magecount] =
		{
			{ 24,266,356, 18, 22, 20, 4790, 6080},					// item_idx
			{  0,  1,  2,  3,  5,  6, -1, -1},					// wearpos
			{  1,  1,  1,  1,  1,  1, 1, 1}					// count
		};

		static int itemsForRogue[3][roguecount] =
		{
			{552,524,528,525,527,526, 4790, 6080},					// item_idx
			{  0,  1,  2,  3,  5,  6, -1, -1},					// wearpos
			{  1,  1,  1,  1,  1,  1, 1, 1}					// count
		};

		static int itemsForSorcerer[3][sorcerercount] =
		{
			{ 1040,1000,988, 1010, 1020, 1030, 4790, 6080},		// item_idx
			{  0,  1,  2,  3,  5,  6, -1, -1 },					// wearpos
			{  1,  1,  1,  1,  1,  1, 1, 1}					// count
		};

#if defined (EX_ROGUE)
		static int itemsForEXRogue[3][exroguecount] =
		{
			{ 9208, 9209, 528, 9210, 9211, 9212, 4790, 6080},					// item_idx
			{  0,  1,  2,  3,  5,  6, -1, -1},					// wearpos
			{  1,  1,  1,  1,  1,  1, 1, 1}					// count
		};
#endif
#ifdef EX_MAGE
		static int itemsForEXMage[3][exmagecount] =
		{
			{ 9347, 9343, 356, 9344, 9345, 9346, 4790, 6080},					// item_idx
			{  0,  1,  2,  3,  5,  6, -1, -1},					// wearpos
			{  1,  1,  1,  1,  1,  1, 1, 1}					// count
		};
#endif
#else
		static int itemsForTitan[4][titancount] =
		{
			{ 72,  2, 12,  3,  4,  8, 4790},					// item_idx
			{  0,  1,  2,  3,  5,  6, -1},					// wearpos
			{  1,  1,  1,  1,  1,  1, 1}					// count
		};

		static int itemsForKnight[3][knightcount] =
		{
			{ 75, 34, 48, 38, 49, 39, 41, 4790},				// item_idx
			{  0,  1,  2,  3,  4,  5,  6, -1},				// wearpos
			{  1,  1,  1,  1,  1,  1,  1, 1}				// count
		};

		static int itemsForHealer[3][healercount] =
		{
			{ 78, 26, 50, 28, 30, 32, 4790},					// item_idx
			{  0,  1,  2,  3,  5,  6, -1},					// wearpos
			{  1,  1,  1,  1,  1,  1, 1}					// count
		};

		static int itemsForMage[3][magecount] =
		{
			{ 24,266,356, 18, 22, 20, 4790},					// item_idx
			{  0,  1,  2,  3,  5,  6, -1},					// wearpos
			{  1,  1,  1,  1,  1,  1, 1}					// count
		};

		static int itemsForRogue[3][roguecount] =
		{
			{552,524,528,525,527,526, 4790},					// item_idx
			{  0,  1,  2,  3,  5,  6, -1},					// wearpos
			{  1,  1,  1,  1,  1,  1, 1}					// count
		};

		static int itemsForSorcerer[3][sorcerercount] =
		{
			{ 1040,1000,988, 1010, 1020, 1030, 4790},		// item_idx
			{  0,  1,  2,  3,  5,  6, -1},					// wearpos
			{  1,  1,  1,  1,  1,  1, 1}					// count
		};

#if defined (EX_ROGUE)
		static int itemsForEXRogue[3][exroguecount] =
		{
			{ 9208, 9209, 528, 9210, 9211, 9212, 4790},					// item_idx
			{  0,  1,  2,  3,  5,  6, -1},					// wearpos
			{  1,  1,  1,  1,  1,  1, 1}					// count
		};
#endif
#ifdef EX_MAGE
		static int itemsForEXMage[3][exmagecount] =
		{
			{ 9347, 9343, 356, 9344, 9345, 9346, 4790},					// item_idx
			{  0,  1,  2,  3,  5,  6, -1},					// wearpos
			{  1,  1,  1,  1,  1,  1, 1}					// count
		};
#endif
#endif // NPC_PORTAL_SCROLL_DRATAN

#if defined(LC_BILA) || defined (LC_GAMIGO) || defined (LC_USA) || defined (LC_RUS)
#ifdef NPC_PORTAL_SCROLL_DRATAN
		static int itemsForNightShadow[3][nightshadowcount] =
		{
			{ 4539,4487,4474, 4500, 4513, 4526, 4552,  4790, 6080},		// item_idx
			{  0,  1,  2,  3,  5,  6, 11, -1, -1},						// wearpos
			{  1,  1,  1,  1,  1,  1,  1, 1, 1}					// count
		};
#else
		static int itemsForNightShadow[3][nightshadowcount] =
		{
			{ 4539,4487,4474, 4500, 4513, 4526, 4552,  4790},		// item_idx
			{  0,  1,  2,  3,  5,  6, 11, -1},						// wearpos
			{  1,  1,  1,  1,  1,  1,  1, 1}					// count
		};
#endif // NPC_PORTAL_SCROLL_DRATAN
#else
#ifdef NPC_PORTAL_SCROLL_DRATAN
#ifdef LC_TLD
		static int itemsForNightShadow[3][nightshadowcount] =
		{
			{ 4539,4487,4474, 4500, 4513, 4526, 4552, 45, 724 ,4790,6080},		// item_idx
			{  0,  1,  2,  3,  5,  6, 11, -1, -1, -1, -1},						// wearpos
			{  1,  1,  1,  1,  1,  1,  1, 100, 100, 1, 1}					// count
		};
#else // LC_TLD
		static int itemsForNightShadow[3][nightshadowcount] =
		{
			{ 4539,4487,4474, 4500, 4513, 4526, 4552, 45, 724 , 1839 ,4790,6080},		// item_idx
			{  0,  1,  2,  3,  5,  6, 11, -1, -1, -1, -1, -1},						// wearpos
			{  1,  1,  1,  1,  1,  1,  1, 100, 100, 10, 1, 1}					// count
		};
#endif // LC_TLD
#else // NPC_PORTAL_SCROLL_DRATAN
#ifdef LC_TLD
		static int itemsForNightShadow[3][nightshadowcount] =
		{
			{ 4539,4487,4474, 4500, 4513, 4526, 4552, 45, 724, 4790},		// item_idx
			{  0,  1,  2,  3,  5,  6, 11, -1, -1, -1},						// wearpos
			{  1,  1,  1,  1,  1,  1,  1, 100, 100, 1}					// count
		};
#else // LC_TLD
		static int itemsForNightShadow[3][nightshadowcount] =
		{
			{ 4539,4487,4474, 4500, 4513, 4526, 4552, 45, 724, 1839, 4790},		// item_idx
			{  0,  1,  2,  3,  5,  6, 11, -1, -1, -1, -1},						// wearpos
			{  1,  1,  1,  1,  1,  1,  1, 100, 100, 10, 1}					// count
		};
#endif // LC_TLD
#endif // NPC_PORTAL_SCROLL_DRATAN
#endif

		int* itemidx = NULL;
		int* itemwearpos = NULL;
		int* itemcount = NULL;
		int defitemcount = 0;

		switch (pChar->m_job)
		{
		case JOB_TITAN:
			itemidx = itemsForTitan[0];
			itemwearpos = itemsForTitan[1];
			itemcount = itemsForTitan[2];
			defitemcount = titancount;
			break;

		case JOB_KNIGHT:
			itemidx = itemsForKnight[0];
			itemwearpos = itemsForKnight[1];
			itemcount = itemsForKnight[2];
			defitemcount = knightcount;
			break;

		case JOB_HEALER:
			itemidx = itemsForHealer[0];
			itemwearpos = itemsForHealer[1];
			itemcount = itemsForHealer[2];
			defitemcount = healercount;
			break;

		case JOB_MAGE:
			itemidx = itemsForMage[0];
			itemwearpos = itemsForMage[1];
			itemcount = itemsForMage[2];
			defitemcount = magecount;
			break;

		case JOB_ROGUE:
			itemidx = itemsForRogue[0];
			itemwearpos = itemsForRogue[1];
			itemcount = itemsForRogue[2];
			defitemcount = roguecount;
			break;

		case JOB_SORCERER:
			itemidx = itemsForSorcerer[0];
			itemwearpos = itemsForSorcerer[1];
			itemcount = itemsForSorcerer[2];
			defitemcount = sorcerercount;
			break;

		case JOB_NIGHTSHADOW:
			itemidx = itemsForNightShadow[0];
			itemwearpos = itemsForNightShadow[1];
			itemcount = itemsForNightShadow[2];
			defitemcount = nightshadowcount;
			break;
#ifdef JOB_EX_ROGUE
		case JOB_EX_ROGUE:
			itemidx = itemsForEXRogue[0];
			itemwearpos = itemsForEXRogue[1];
			itemcount = itemsForEXRogue[2];
			defitemcount = exroguecount;
			break;
#endif // JOB_EX_ROGUE
#ifdef JOB_EX_MAGE
		case JOB_EX_MAGE:
			itemidx = itemsForEXMage[0];
			itemwearpos = itemsForEXMage[1];
			itemcount = itemsForEXMage[2];
			defitemcount = exmagecount;
			break;
#endif
		default:
			defitemcount = 0;
			break;
		}

		std::string sql1;		// INSERT INTO t_inven (a_char_index, a_tab_idx, a_row_idx
		std::string sql2;		// ) VALUES (char, tab, row
		std::string sql3;		// a_item_idx, a_plus, a_wear_pos, a_flag, a_serial, a_count, a_used
		std::string sql4;		// idx, 0, wearpos, 0, str, count, used
		std::string wearInven_sql1;
		std::string wearInven_sql2;

		int itab = 0;
		int irow = 0;
		int icol = 0;

		int table_no = char_index % 10;

		sql1 = boost::str(boost::format("INSERT INTO t_inven%02d(a_char_idx, a_tab_idx, a_row_idx") % table_no);
		sql2 = boost::str(boost::format(") VALUES (%d, 0, 0") % char_index);
		sql3 = "";
		sql4 = "";
		wearInven_sql1 = boost::str(boost::format("INSERT INTO t_wear_inven(a_char_index, a_wear_pos, a_item_idx, a_plus, a_flag, a_serial, a_used, a_used2) values "));
		wearInven_sql2 = "";

		for (int i = 0; i < defitemcount; i++)
		{
			if(itemidx[i] == 4790) // 쥬노 NPC포탈 스크롤
				continue;

			CItemProto* itemproto = gserver->m_itemProtoList.FindIndex(itemidx[i]);
			if (itemproto == NULL)
				continue;

			int used = itemproto->getItemMaxUse();

			std::string serial = GetSerial();

			if (itemproto->getItemWearing() >= 0)
			{
				wearInven_sql2 += boost::str(boost::format(" (%d, %d, %d, %d, %d, '%s', %d, %d), ") % char_index % itemproto->getItemWearing() % itemidx[i] % 0 % 0 % serial % used % 0);
			}
			else
			{
				sql3 += boost::str(boost::format(
									   ", a_item_idx%1%, a_plus%1%, a_wear_pos%1%, a_flag%1%, a_serial%1%, a_count%1%, a_used%1%, a_used%1%_2") % icol);

				sql4 += boost::str(boost::format(", %1%, %2%, %3%, %4%, '%5%', %6%, %7%, %8%")
								   % itemidx[i] % 0 % itemwearpos[i] % 0 % serial % itemcount[i] % used % 0);
				icol++;
			}

			if (icol == 5)
			{
				irow++;
				icol = 0;

				query = sql1 + sql3 + sql2 + sql4 + ")";
				if (char_db_.excute(query) == false)
				{
					throw MSG_FAIL_DB_UNKNOWN;
				}

				sql2 = boost::str(boost::format(") VALUES (%1%, 0, %2% ") % char_index % irow);
				sql3 = "";
				sql4 = "";
			}
		}

		if (sql3.empty() == false)
		{
			query = sql1 + sql3 + sql2 + sql4 + ")";
			if (char_db_.excute(query) == false)
			{
				throw MSG_FAIL_DB_UNKNOWN;
			}
		}

		if (wearInven_sql2.empty() == false)
		{
			query = wearInven_sql1 + wearInven_sql2;
			int index = query.rfind(",");
			query.erase(index);
			if (char_db_.excute(query) == false)
			{
				throw MSG_FAIL_DB_UNKNOWN;
			}
		}

#ifdef HANARO_EVENT
		{
			int* pItemIndex = NULL;
			int* pItemCount = NULL;
			int  nItemCount = 0;
			int  nItemUsed  = -1;

			if (pChar->m_hanaroCode == "HF" || pChar->m_hanaroCode == "hf")
			{
				// 기본 지급 : 부활주문서(초보자용), 플래티늄 아이리스 축복, 플레티늄 숙련의 묘약
				// 하나포스 : 수박, 참외
				static const int nHFCount = 5;
				static int nHFItem[2][nHFCount] =
				{
					{5958, 2658, 2659, 836, 837},
					{   5,    5,    5,   5,   5}
				};

				nItemCount = nHFCount;
				pItemIndex = nHFItem[0];
				pItemCount = nHFItem[1];
			} // HF
			else if (pChar->m_hanaroCode == "SK"  || pChar->m_hanaroCode == "sk")
			{
				// 기본 지급 : HP 스틸 포션, 플래티늄 아이리스 축복, 플레티늄 숙련의 묘약
				// SK브로드밴드 : 결합 주문서, 각 클래스별 교복 코스튬 세트
				static const int nSKCount = 8;
				static int nTitanItem[2][nSKCount] =
				{
					{5958, 2658, 2659, 2664, 1422, 1423, 1424, 1425},
					{   5,    5,    5,    5,    1,    1,    1,    1}
				};
				static int nNightItem[2][nSKCount] =
				{
					{5958, 2658, 2659, 2664, 1426, 1427, 1428, 1429},
					{   5,    5,    5,    5,    1,    1,    1,    1}
				};
				static int nHealerItem[2][nSKCount] =
				{
					{5958, 2658, 2659, 2664, 1430, 1431, 1432, 1433},
					{   5,    5,    5,    5,    1,    1,    1,    1}
				};
				static int nMageItem[2][nSKCount] =
				{
					{5958, 2658, 2659, 2664, 1434, 1435, 1436, 1437},
					{   5,    5,    5,    5,    1,    1,    1,    1}
				};
				static int nRogueItem[2][nSKCount] =
				{
					{5958, 2658, 2659, 2664, 1438, 1439, 1440, 1441},
					{   5,    5,    5,    5,    1,    1,    1,    1}
				};
				static int nSorcererItem[2][nSKCount] =
				{
					{5958, 2658, 2659, 2664, 1442, 1443, 1444, 1445},
					{   5,    5,    5,    5,    1,    1,    1,    1}
				};
				nItemCount = nSKCount;
				nItemUsed  = gserver->getNowSecond() + 30 * 24 * 60 * 60;	// 1개월
				switch (pChar->m_job)
				{
				case JOB_TITAN:
					pItemIndex = nTitanItem[0];
					pItemCount = nTitanItem[1];
					break;
				case JOB_KNIGHT:
					pItemIndex = nNightItem[0];
					pItemCount = nNightItem[1];
					break;
				case JOB_HEALER:
					pItemIndex = nHealerItem[0];
					pItemCount = nHealerItem[1];
					break;
				case JOB_MAGE:
					pItemIndex = nMageItem[0];
					pItemCount = nMageItem[1];
					break;
				case JOB_ROGUE:
#ifdef EX_ROGUE
				case JOB_EX_ROGUE:
#endif // EX_ROGUE
#ifdef EX_MAGE
				case JOB_EX_MAGE:
#endif
					pItemIndex = nRogueItem[0];
					pItemCount = nRogueItem[1];
					break;
				case JOB_SORCERER:
					pItemIndex = nSorcererItem[0];
					pItemCount = nSorcererItem[1];
					break;
				default:
					nItemCount = 0;
				}
			} // SK
			else
			{
#if defined(LC_BILA) || defined(LC_KOR)// 멕시코 하나로 이벤트
				// 기본 지급 : 부활 주문서, 플래티늄 아이리스 축복, 플레티늄 숙련의 묘약
#ifdef EVENT_TREASUREBOX
				static int nItemList[2][4] =
				{
					{5958, 2658, 2659, 6085},
					{  15,   15,   15,    1}
				};
				nItemCount = 4;
#else
				static int nItemList[2][4] =
				{
					{5958, 2658, 2659, 6085},
					{  15,   15,   15,    1}
				};
				nItemCount = 4;
#endif //EVENT_TREASUREBOX
#elif defined(LC_TLD)
				static int nItemList[2][5] =
				{
					{2658, 2659, 2667, 2668, 6646},
					{   5,    5,    5,    5, 10}
				};
				nItemCount = 5;
#else // #if defined(LC_BILA)
				// 기본 지급 : HP 스틸 포션, 플래티늄 아이리스 축복, 플레티늄 숙련의 묘약
				static int nItemList[2][4] =
				{
					{5958, 2658, 2659, 6085},
					{   5,    5,    5,    1}
				};
				nItemCount = 4;
#endif // #if defined(LC_BILA)
				pItemIndex = nItemList[0];
				pItemCount = nItemList[1];
			}

			itab = icol = 0;
			irow = 4;		// 나이트 쉐도우 아이템이 많아서 4로 수정

			sql1 = boost::str(boost::format("INSERT INTO t_inven%02d(a_char_idx, a_tab_idx, a_row_idx") % table_no);
			sql2 = boost::str(boost::format(") VALUES (%d, 0, %d ") % char_index % irow);
			sql3 = "";
			sql4 = "";

			for (int i = 0; i < nItemCount; i++)
			{
				CItemProto* itemproto = gserver->m_itemProtoList.FindIndex(pItemIndex[i]);
				if (itemproto == NULL)
					continue;

				sql3 += boost::str(boost::format(
									   ", a_item_idx%1%, a_plus%1%, a_wear_pos%1%, a_flag%1%, a_serial%1%, a_count%1%, a_used%1%, a_used%1%_2") % icol);

				std::string serial = GetSerial();

				sql4 += boost::str(boost::format(", %1%, %2%, %3%, %4%, '%5%', %6%, %7%, %8%")
								   % pItemIndex[i] % 0 % -1 % 0 % serial % pItemCount[i] % nItemUsed % 0);

				icol++;
				if (icol == 5)
				{
					irow++;
					icol = 0;

					query = sql1 + sql3 + sql2 + sql4 + ")";

					if (pChar->m_job != JOB_NIGHTSHADOW)
					{
						if (char_db_.excute(query) == false)
							throw MSG_FAIL_DB_UNKNOWN;
					}

					sql2 = boost::str(boost::format(") VALUES (%1%, 0, %2%") % char_index % irow);
					sql3 = "";
					sql4 = "";
				}
			} // for

			if (sql3.empty() == false);
			{
				query = sql1 + sql3 + sql2 + sql4 + ")";

				if (pChar->m_job != JOB_NIGHTSHADOW)
				{
					if (char_db_.excute(query) == false)
						throw MSG_FAIL_DB_UNKNOWN;
				}
			}
		}
#endif

		if (pChar->m_job != JOB_NIGHTSHADOW)
		{
			irow = 3;			//3번부터 초반 지급 아이템 확장
			icol = 0;

			static int nItemList[2][1] =
			{
				{760},
				{1}
			};

			int nItemCount = 1;

			int* pItemIndex = nItemList[0];
			int* pItemCount = nItemList[1];


			sql1 = boost::str(boost::format("INSERT INTO t_inven%02d(a_char_idx, a_tab_idx, a_row_idx") % table_no);
			sql2 = boost::str(boost::format(") VALUES (%d, 0, %d ") % char_index % irow);
			sql3 = "";
			sql4 = "";

			for (int i = 0; i < nItemCount; i++)
			{
				CItemProto* itemproto = gserver->m_itemProtoList.FindIndex(pItemIndex[i]);
				if (itemproto == NULL)
					continue;

				sql3 += boost::str(boost::format(
									   ", a_item_idx%1%, a_plus%1%, a_wear_pos%1%, a_flag%1%, a_serial%1%, a_count%1%, a_used%1%, a_used%1%_2") % icol);

				std::string serial = GetSerial();

				sql4 += boost::str(boost::format(", %1%, %2%, %3%, %4%, '%5%', %6%, %7%, %8%")
								   % pItemIndex[i] % 0 % -1 % 0 % serial % pItemCount[i] % itemproto->getItemMaxUse() % 0);

				icol++;
				if(icol > 5)
				{
					throw MSG_FAIL_DB_UNKNOWN;
				}
			} // for

			query = sql1 + sql3 + sql2 + sql4 + ")";

			if (char_db_.excute(query) == false)
				throw MSG_FAIL_DB_UNKNOWN;
		}

		/////////////////////////

		// 디폴트 슬롯 설정
		query = boost::str(boost::format(
							   "INSERT INTO t_quickslot%02d(a_char_idx, a_page_idx, a_slot) VALUES (%d, 0, ") % table_no % char_index);

		switch (pChar->m_job)
		{
		case JOB_TITAN:
			query += "'0 1";			// 파워매그넘
			break;

		case JOB_KNIGHT:
			query += "'0 14";		// 너클스피어
			break;

		case JOB_HEALER:
			query += "'0 27 0 31";	// 샤이닝애로우, 셀프힐
			break;

		case JOB_MAGE:
			query += "'0 53";		// 에너지 볼트
			break;

		case JOB_ROGUE:
			query += "'0 116";		// 에너지 볼트
			break;

		case JOB_SORCERER:
			query += "'0 292";		// 파이어 스윙
			break;

#ifdef EX_ROGUE
		case JOB_EX_ROGUE:
			query += "'0 1528";				// EXROGUE : 스킬 인덱스 추가
			break;
#endif // EX_ROGUE
#ifdef EX_MAGE
		case JOB_EX_MAGE:
			query += "'0 1637";
			break;
#endif // EX_MAGE
		default:
			query += "'";
			break;
		}

		switch (pChar->m_job)
		{
		case JOB_TITAN:
		case JOB_HEALER:
		case JOB_MAGE:
		case JOB_ROGUE:
		case JOB_SORCERER:
#ifdef EX_ROGUE
		case JOB_EX_ROGUE:
#endif // EX_ROGUE
#ifdef EX_MAGE
		case JOB_EX_MAGE:
#endif
			query += " 1 4 1 5 1 6 1 7 1 3 1 30 1 41 1 24 1 52 2 1 2')";
			break;
		case JOB_KNIGHT:
			query += " 1 4 1 5 1 6 1 7 1 3 1 30 1 41 1 24 1 52 2 1 3')";
			break;
		case JOB_NIGHTSHADOW:
			query += " 1 4 1 5 1 6 1 7 1 3 1 30 1 41 1 24 1 52 2 2 1')";
			break;
		default:
			query += " 1 4 1 5 1 6 1 7 1 3 1 30 1 41 1 24 1 52 2 1 2')";
			break;
		}

		if (char_db_.excute(query) == false)
		{
			throw MSG_FAIL_DB_UNKNOWN;
		}

		// 빈슬롯 주기 page 1 ~ 2
		for (int page = 1; page < QUICKSLOT_PAGE_NUM; page++)
		{
			query = boost::str(boost::format(
								   "INSERT INTO t_quickslot%02d(a_char_idx, a_page_idx) VALUES(%d, %d)") % table_no % char_index % page);
			if (char_db_.excute(query) == false)
			{
				throw MSG_FAIL_DB_UNKNOWN;
			}
		}

		char_db_.COMMIT();

		{
			CNetMsg::SP rmsg(new CNetMsg);
			DBSuccessMsg(rmsg);
			SendMessageToClient(seq_index, user_index, rmsg);

			EventProcessForDB::returnCreateChar data;
			data.m_index = pChar->m_index;
			data.m_seq_index = seq_index;
			data.char_index = char_index;
			data.name = pChar->m_name;
			data.job = pChar->m_job;
			EventProcessForDB::instance()->pushCreateCharacter(data);
		}
	}
	catch (MSG_FAIL_TYPE failtype)
	{
		char_db_.ROLLBACK();

		CNetMsg::SP rmsg(new CNetMsg);
		FailMsg(rmsg, failtype);
		SendMessageToClient(seq_index, user_index, rmsg);
	}
	catch (...)
	{
		char_db_.ROLLBACK();

		CNetMsg::SP rmsg(new CNetMsg);
		FailMsg(rmsg, MSG_FAIL_SYSTEM_ERROR);
		SendMessageToClient(seq_index, user_index, rmsg);
	}
}