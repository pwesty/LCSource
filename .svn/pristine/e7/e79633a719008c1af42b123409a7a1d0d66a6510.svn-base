#define __DB_PROCESS_UPDATE_CHAR_CPP__

#include <boost/scoped_ptr.hpp>
#include "stdhdrs.h"

#include "Server.h"
#include "CmdMsg.h"
#include "EventProcessWithThread.h"
#include "../ShareLib/DBCmd.h"
#include "doFunc.h"
#include "../ShareLib/packetType/ptype_server_to_server.h"

#include "DBManager.h"

//////////////////////////////////////////////////////////////////////////
void DBProcess::excute_query(std::vector<std::string>& vec)
{
	int count = vec.size();
	for (int i = 0; i < count; ++i)
	{
		char_db_.excute(vec[i]);
	}
}

void DBProcess::excute_query_commi_and_rollback(std::vector<std::string>& vec)
{
	bool ok_flag = true;

	char_db_.BEGIN();

	//////////////////////////////////////////////////////////////////////////
	int count = vec.size();
	for (int i = 0; i < count; ++i)
	{
		if (char_db_.excute(vec[i]) == false)
		{
			ok_flag = false;
			break;
		}
	}
	//////////////////////////////////////////////////////////////////////////

	if (ok_flag)
		char_db_.COMMIT();
	else
		char_db_.ROLLBACK();
}
//////////////////////////////////////////////////////////////////////////

void DBProcess::SaveChar( boost::any& argv )
{
	savechar_t data = boost::any_cast<savechar_t>(argv);
	LONGLONG seq_index = boost::tuples::get<0>(data);
	int userIndex = boost::tuples::get<1>(data);
	int charIndex = boost::tuples::get<2>(data);
	std::string id = boost::tuples::get<3>(data);
	std::string charName = boost::tuples::get<4>(data);
	bool disconnect_flag = boost::tuples::get<5>(data);
	save_char_queryt_t* pSaveChar = boost::tuples::get<6>(data);
	boost::scoped_ptr<save_char_queryt_t> __auto_delete(pSaveChar);

	save_char_queryt_t& query_vec = *pSaveChar;

	if (query_vec[CHAR_SAVE_TYPE_TCHAR].empty() == false)
	{
		char_db_.excute(query_vec[CHAR_SAVE_TYPE_TCHAR][0]);
	}

	if (query_vec[CHAR_SAVE_TYPE_TCHAR_GUILD_POINT].empty() == false)
	{
		char_db_.excute(query_vec[CHAR_SAVE_TYPE_TCHAR_GUILD_POINT][0]);
	}

	if (query_vec[CHAR_SAVE_TYPE_TCHAR_QUEST].empty() == false)
	{
		excute_query_commi_and_rollback(query_vec[CHAR_SAVE_TYPE_TCHAR_QUEST]);
	}

	if (query_vec[CHAR_SAVE_TYPE_TCHAR_AFFINITY].empty() == false)
	{
		excute_query_commi_and_rollback(query_vec[CHAR_SAVE_TYPE_TCHAR_AFFINITY]);
	}

	if (query_vec[CHAR_SAVE_TYPE_TCHAR_ASSIST].empty() == false)
	{
		excute_query_commi_and_rollback(query_vec[CHAR_SAVE_TYPE_TCHAR_ASSIST]);
	}

	if (query_vec[CHAR_SAVE_TYPE_TCHAR_ASSIST_ABS_TIME].empty() == false)
	{
		excute_query_commi_and_rollback(query_vec[CHAR_SAVE_TYPE_TCHAR_ASSIST_ABS_TIME]);
	}

	if (query_vec[CHAR_SAVE_TYPE_TCHAR_INVEN].empty() == false)
	{
		excute_query_commi_and_rollback(query_vec[CHAR_SAVE_TYPE_TCHAR_INVEN]);
	}

	if (query_vec[CHAR_SAVE_TYPE_TCHAR_GAMIGO_HACK_DELETE].empty() == false)
	{
		std::vector<std::string>& vec = query_vec[CHAR_SAVE_TYPE_TCHAR_GAMIGO_HACK_DELETE];
		std::string& query = vec[0];

		char_db_.excute(query);

		LOG_INFO("HACK ITEM DELETE : query[%s]", query.c_str());
	}

	if (query_vec[CHAR_SAVE_TYPE_TCHAR_STASH].empty() == false)
	{
		excute_query_commi_and_rollback(query_vec[CHAR_SAVE_TYPE_TCHAR_STASH]);
	}

	if (query_vec[CHAR_SAVE_TYPE_TCHAR_QUICK_SLOT].empty() == false)
	{
		excute_query_commi_and_rollback(query_vec[CHAR_SAVE_TYPE_TCHAR_QUICK_SLOT]);
	}

	if (query_vec[CHAR_SAVE_TYPE_TCHAR_MEMPOS].empty() == false)
	{
		std::vector<std::string>& vec = query_vec[CHAR_SAVE_TYPE_TCHAR_MEMPOS];

		BOOST_MYSQL_RES res = char_db_.select(vec[0]);
		if (res != NULL)
		{
			if (char_db_.getrowcount() > 0)
			{
				// update
				char_db_.excute(vec[1]);
			}
			else
			{
				// insert
				char_db_.excute(vec[2]);
			}
		}
	}

	if (query_vec[CHAR_SAVE_TYPE_TCHAR_CASH_ITEM_DATE].empty() == false)
	{
		std::vector<std::string>& vec = query_vec[CHAR_SAVE_TYPE_TCHAR_CASH_ITEM_DATE];

		BOOST_MYSQL_RES res = char_db_.select(vec[0]);
		if (res != NULL)
		{
			if (char_db_.getrowcount() > 0)
			{
				// update
				char_db_.excute(vec[1]);
			}
			else
			{
				// insert
				char_db_.excute(vec[2]);
			}

			char_db_.excute(vec[3]);
		}
	}

	if (query_vec[CHAR_SAVE_TYPE_TCHAR_PD_DATA].empty() == false)
	{
		std::vector<std::string>& vec = query_vec[CHAR_SAVE_TYPE_TCHAR_PD_DATA];

		BOOST_MYSQL_RES res = char_db_.select(vec[0]);
		if (res != NULL)
		{
			if (char_db_.getrowcount() > 0)
			{
				// update
				char_db_.excute(vec[1]);
			}
			else
			{
				// insert
				char_db_.excute(vec[2]);
			}
		}
	}

	if (query_vec[CHAR_SAVE_TYPE_TCHAR_PET_PRE].empty() == false)
	{
		char_db_.excute(query_vec[CHAR_SAVE_TYPE_TCHAR_PET_PRE][0]);
	}

	if (query_vec[CHAR_SAVE_TYPE_TCHAR_PET].empty() == false)
	{
		std::vector<std::string>& vec = query_vec[CHAR_SAVE_TYPE_TCHAR_PET];
		int count = vec.size();
		for (int i = 0; i < count; i += 3)
		{
			BOOST_MYSQL_RES res = char_db_.select(vec[i + 0]);
			if (res != NULL && char_db_.getrowcount() > 0)
			{
				// update
				char_db_.excute(vec[i + 1]);
			}
			else
			{
				// insert
				char_db_.excute(vec[i + 2]);
			}
		}
	}

	if (query_vec[CHAR_SAVE_TYPE_TCHAR_PET_NAME].empty() == false)
	{
		std::vector<std::string>& vec = query_vec[CHAR_SAVE_TYPE_TCHAR_PET_NAME];
		BOOST_MYSQL_RES res = char_db_.select(vec[0]);
		if (res != NULL && char_db_.getrowcount() > 0)
		{
			// update
			char_db_.excute(vec[1]);
		}
		else
		{
			// insert
			char_db_.excute(vec[2]);
		}
	}

	if (query_vec[CHAR_SAVE_TYPE_TCHAR_PET_TEMP_DELETE].empty() == false)
	{
		char_db_.excute(query_vec[CHAR_SAVE_TYPE_TCHAR_PET_TEMP_DELETE][0]);
	}

	if (query_vec[CHAR_SAVE_TYPE_TCHAR_APET_PRE].empty() == false)
	{
		char_db_.excute(query_vec[CHAR_SAVE_TYPE_TCHAR_APET_PRE][0]);
	}

	if (query_vec[CHAR_SAVE_TYPE_TCHAR_APET].empty() == false)
	{
		std::vector<std::string>& vec = query_vec[CHAR_SAVE_TYPE_TCHAR_APET];
		int count = vec.size();
		for (int i = 0; i < count; i += 3)
		{
			BOOST_MYSQL_RES res = char_db_.select(vec[i + 0]);
			if (res != NULL && char_db_.getrowcount() > 0)
			{
				// update
				char_db_.excute(vec[i + 1]);
			}
			else
			{
				// insert
				char_db_.excute(vec[i + 2]);
			}
		}
	}

	if (query_vec[CHAR_SAVE_TYPE_TCHAR_APET_INVEN].empty() == false)
	{
		excute_query_commi_and_rollback(query_vec[CHAR_SAVE_TYPE_TCHAR_APET_INVEN]);
	}

	if (query_vec[CHAR_SAVE_TYPE_TCHAR_APET_AI].empty() == false)
	{
		excute_query_commi_and_rollback(query_vec[CHAR_SAVE_TYPE_TCHAR_APET_AI]);
	}

	if (query_vec[CHAR_SAVE_TYPE_TCHAR_APET_TEMP_DELETE].empty() == false)
	{
		char_db_.excute(query_vec[CHAR_SAVE_TYPE_TCHAR_APET_TEMP_DELETE][0]);
	}

	if (query_vec[CHAR_SAVE_TYPE_TCHAR_CHARACTER_FACTORY].empty() == false)
	{
		excute_query_commi_and_rollback(query_vec[CHAR_SAVE_TYPE_TCHAR_CHARACTER_FACTORY]);
	}

	if (query_vec[CHAR_SAVE_TYPE_TCHAR_MESSENGER_OPT].empty() == false)
	{
		excute_query_commi_and_rollback(query_vec[CHAR_SAVE_TYPE_TCHAR_MESSENGER_OPT]);
	}

	if (query_vec[CHAR_SAVE_TYPE_TCHAR_FRIEND].empty() == false)
	{
		excute_query_commi_and_rollback(query_vec[CHAR_SAVE_TYPE_TCHAR_FRIEND]);
	}

	if (query_vec[CHAR_SAVE_TYPE_TCHAR_BLOCK_FRIEND].empty() == false)
	{
		excute_query_commi_and_rollback(query_vec[CHAR_SAVE_TYPE_TCHAR_BLOCK_FRIEND]);
	}

	if (query_vec[CHAR_SAVE_TYPE_TCHAR_WAR_GROUND].empty() == false)
	{
		excute_query_commi_and_rollback(query_vec[CHAR_SAVE_TYPE_TCHAR_WAR_GROUND]);
	}

	if (query_vec[CHAR_SAVE_TYPE_TCHAR_EVENT_GOMDORI].empty() == false)
	{
		std::vector<std::string>& vec = query_vec[CHAR_SAVE_TYPE_TCHAR_EVENT_GOMDORI];
		BOOST_MYSQL_RES res = char_db_.select(vec[0]);
		if (res != NULL && char_db_.getrowcount() <= 0)
		{
			// insert
			char_db_.excute(vec[1]);
		}

		int count = vec.size() - 2;
		for (int i = 2; i < count; ++i)
		{
			char_db_.excute(vec[i]);
		}
	}

	if (query_vec[CHAR_SAVE_TYPE_TIMER_ITEM].empty() == false)
	{
		excute_query_commi_and_rollback(query_vec[CHAR_SAVE_TYPE_TIMER_ITEM]);
	}

	if (query_vec[CHAR_SAVE_TYPE_TCHAR_PET_STASH].empty() == false)
	{
		excute_query_commi_and_rollback(query_vec[CHAR_SAVE_TYPE_TCHAR_PET_STASH]);
	}

	if (query_vec[CHAR_SAVE_TYPE_TCHAR_PET_STASH_INFO].empty() == false)
	{
		std::vector<std::string>& vec = query_vec[CHAR_SAVE_TYPE_TCHAR_PET_STASH_INFO];

		BOOST_MYSQL_RES res = char_db_.select(vec[0]);
		if (res != NULL && char_db_.getrowcount() > 0)
		{
			// update
			char_db_.excute(vec[1]);
		}
		else
		{
			// insert
			char_db_.excute(vec[2]);
		}
	}

	if (query_vec[CHAR_SAVE_TYPE_TCHAR_SKILL_COOLTIME].empty() == false)
	{
		excute_query(query_vec[CHAR_SAVE_TYPE_TCHAR_SKILL_COOLTIME]);
	}

	if (query_vec[CHAR_SAVE_TYPE_TCHAR_WEAR_ITEM].empty() == false)
	{
		excute_query(query_vec[CHAR_SAVE_TYPE_TCHAR_WEAR_ITEM]);
	}

	if (query_vec[CHAR_SAVE_TYPE_RVR_INFO].empty() == false)
	{
		excute_query(query_vec[CHAR_SAVE_TYPE_RVR_INFO]);
	}

	if (query_vec[CHAR_SAVE_TYPE_GPS_INFO].empty() == false)
	{
		excute_query(query_vec[CHAR_SAVE_TYPE_GPS_INFO]);
	}

	if (query_vec[CHAR_SAVE_TYPE_ITEM_COLLECTION_INFO].empty() == false)
	{
		excute_query(query_vec[CHAR_SAVE_TYPE_ITEM_COLLECTION_INFO]);
	}

#ifdef PREMIUM_CHAR
	if (query_vec[CHAR_SAVE_TYPE_PREMIUM_CHAR_INFO].empty() == false)
	{
		excute_query(query_vec[CHAR_SAVE_TYPE_PREMIUM_CHAR_INFO]);
	}
#endif

	LOG_INFO("save Character completed: id[%s] / user_index[%d] / char_index[%d] / char_name[%s] / disconnect[%d]",
			 id.c_str(), userIndex, charIndex, charName.c_str(), disconnect_flag);

	if (disconnect_flag)
	{
		// 게임을 종료하면서 저장하는 것이라면
		// Connector Server로 로그아웃 보내기
		// Connector Server에서는 id를 찾지 못하면 return함
		CNetMsg::SP rmsg(new CNetMsg);
		ConnLogoutMsg(rmsg, id.c_str());
		SEND_Q(rmsg, gserver->m_connector);

		{
			// Helper, SubHelper에게 접속 정보를 전달
			CNetMsg::SP rmsg(new CNetMsg);
			ServerToServerPacket::makeLogoutUserInfo(rmsg, userIndex);
			SEND_Q(rmsg, gserver->m_helper);
			SEND_Q(rmsg, gserver->m_subHelper);
		}

		LOG_INFO("send logout msg to Connector : user_index[%d] id[%s]", userIndex, id.c_str());
	}
}
