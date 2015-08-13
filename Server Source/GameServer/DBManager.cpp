#include <boost/bind.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/format.hpp>
#include <boost/tokenizer.hpp>
#include "stdhdrs.h"

#include "../ShareLib/SignalProcess.h"
#include "Server.h"
#include "CmdMsg.h"
#include "EventProcessWithThread.h"
#include "../ShareLib/DBCmd.h"
#include "../ShareLib/packetType/ptype_tradeAgent_system.h"
#include "../ShareLib/packetType/ptype_express_system.h"
#include "DescManager.h"
#include "DBManager.h"

//////////////////////////////////////////////////////////////////////////
typedef boost::tokenizer<boost::char_separator<char> > stokenizer;
static boost::char_separator<char> sep(" ", NULL, boost::drop_empty_tokens);
//////////////////////////////////////////////////////////////////////////

DBManager::DBManager()
	: thread_count_(0)
	, db_process_(NULL)
	, stop_flag_(false)
{
}

DBManager::~DBManager()
{
	if (db_process_)
	{
		delete [] db_process_;
	}
}

DBManager* DBManager::instance()
{
	static DBManager __instance;
	return &__instance;
}

bool DBManager::Init(int count)
{
	thread_count_ = count;
	db_process_ = new DBProcess[thread_count_];

	for (int i = 0; i < thread_count_; ++i)
	{
		if (db_process_[i].Connect() == false)
			return false;
	}

	for (int i = 0; i < thread_count_; ++i)
	{
		thread_group_.create_thread(boost::bind(&DBProcess::Run, &db_process_[i]));
	}

	return true;
}

void DBManager::JoinAll()
{
	if (stop_flag_)
		return;

	stop_flag_ = true;

	// 현재 접속하여 게임중인 모든 사용자의 정보를 저장
	DescManager::instance()->saveAllBeforServerDown();

	//////////////////////////////////////////////////////////////////////////

	DBProcess::type_t end_data(DB_PROC_EXIT, (int)DB_PROC_EXIT);

	for (int i = 0; i < thread_count_; ++i)
	{
		db_process_[i].queue_.push_signal(end_data);
	}

	thread_group_.join_all();

	delete [] db_process_;
	db_process_ = NULL;
}

//XX 로그인 3
void DBManager::PushCharacterList( CDescriptor* desc )
{
	DBProcess::charlist_t data(desc->m_seq_index, desc->m_index, desc->m_playmode, desc->m_nPrepareSeed);
	DBProcess::type_t pushdata(DB_PROC_CHARACTER_LIST, data);

	int index = desc->m_index % thread_count_;
	db_process_[index].queue_.push_signal(pushdata);
}

//XX 캐릭터생성 2
void DBManager::PushCreateChar( CDescriptor* desc, CreateCharacterInfo* cinfo )
{
	DBProcess::createchar_t data(desc->m_seq_index, desc->m_index, cinfo);
	DBProcess::type_t pushdata(DB_PROC_CREATE_CHAR, data);

	int index = desc->m_index % thread_count_;
	db_process_[index].queue_.push_signal(pushdata);
}

//XX 캐릭터삭제 2
void DBManager::PushDeleteCharacter( CDescriptor* desc, int char_index, int guildoutdate )
{
	DBProcess::delchar_t data(desc->m_seq_index, desc->m_index, char_index, desc->getExtendCharacterSlotTime(), guildoutdate);
	DBProcess::type_t pushdata(DB_PROC_DELETE_CHAR, data);

	int index = desc->m_index % thread_count_;
	db_process_[index].queue_.push_signal(pushdata);
}

//XX 캐릭터복원 2
void DBManager::PushCancelDeleteCharacter( CDescriptor* desc, int char_index )
{
	DBProcess::canceldelchar_t data(desc->m_seq_index, desc->m_index, char_index);
	DBProcess::type_t pushdata(DB_PROC_CANCEL_DELETE_CHAR, data);

	int index = desc->m_index % thread_count_;
	db_process_[index].queue_.push_signal(pushdata);
}

//XX 캐릭터선택 2
void DBManager::PushSelectCharacter( CDescriptor* desc, int char_index, void* guild )
{
	DBProcess::selectchar_t data(desc->m_seq_index, desc->m_index, char_index, desc->m_playmode, (const char *)desc->m_proSite, (const char *)desc->m_idname, guild);
	DBProcess::type_t pushdata(DB_PROC_SELECT_CHAR, data);

	int index = desc->m_index % thread_count_;
	db_process_[index].queue_.push_signal(pushdata);
}

void DBManager::PushSaveCharacter( CDescriptor* desc, save_char_queryt_t* save_data, bool disconnect )
{
	DBProcess::savechar_t data(desc->m_seq_index, desc->m_index, desc->m_pChar->m_index,
							   (const char *)desc->m_idname, (const char *)desc->m_pChar->m_name, disconnect, save_data);
	DBProcess::type_t pushdata(DB_PROC_SAVE_CHAR, data);

	int index = desc->m_index % thread_count_;
	db_process_[index].queue_.push_signal(pushdata);

	LOG_INFO("save Character : id[%s] / user_index[%d] / char_index[%d] / char_name[%s] / disconnect[%d]",
			 (const char *)desc->m_idname, desc->m_index, desc->m_pChar->m_index, (const char *)desc->m_pChar->m_name, disconnect);
}

void DBManager::PushExpressExist( CDescriptor* desc, int send_type )
{
	DBProcess::expressexist_t data(desc->m_seq_index, desc->m_index, desc->m_pChar->m_index, send_type);
	DBProcess::type_t pushdata(DB_PROC_EXPRESS_EXIST, data);

	int index = desc->m_index % thread_count_;
	db_process_[index].queue_.push_signal(pushdata);
}

void DBManager::PushExpressList( CDescriptor* desc, int pageIndex )
{
	DBProcess::expresslist_t data(desc->m_seq_index, desc->m_index, desc->m_pChar->m_index, pageIndex);
	DBProcess::type_t pushdata(DB_PROC_EXPRESS_LIST, data);

	int index = desc->m_index % thread_count_;
	db_process_[index].queue_.push_signal(pushdata);
}

//XX 거래대행 - 찾기 2
void DBManager::PushExpressTake( CDescriptor* desc, expressIndex_t expressIndex )
{
	DBProcess::expresstake_t data(desc->m_seq_index, desc->m_index, desc->m_pChar->m_index, expressIndex);
	DBProcess::type_t pushdata(DB_PROC_EXPRESS_TAKE, data);

	int index = desc->m_index % thread_count_;
	db_process_[index].queue_.push_signal(pushdata);
}

void DBManager::PushExpressInputItem( CDescriptor* desc, ExpressSystemItemInfo* itemInfo, bool contentsFlag )
{
	CItemProto* itemProto = gserver->m_itemProtoList.FindIndex(itemInfo->item_index);
	if (itemInfo == NULL)
	{
		LOG_ERROR("Not found item. userIndex : %d, itemIndex : %d, itemCount : %d", desc->m_index, itemInfo->item_index, itemInfo->item_count);
		return;
	}

	if (itemProto->getItemFlag() & ITEM_FLAG_COUNT)
	{
		if (itemProto->getStackCount() < itemInfo->item_count)
			itemInfo->item_count = itemProto->getStackCount();
	}
	else
	{
		itemInfo->item_count = 1;
	}

	itemInfo->sender[EXPRESS_SENDER_NAME] = '\0';

	DBProcess::expressinputitem_t data(desc->m_seq_index, desc->m_pChar->m_index, itemInfo);
	DBProcess::type_t pushdata(DB_PROC_EXPRESS_INPUT_ITEM, data);

	int index = desc->m_index % thread_count_;
	db_process_[index].queue_.push_signal(pushdata);
}

void DBManager::PushExpressInputNas( CDescriptor* desc, unsigned int nas, int send_type, std::string sender, bool contentsFlag )
{
	DBProcess::expressinputnas_t data(desc->m_seq_index, desc->m_index, desc->m_pChar->m_index, nas, send_type, sender, contentsFlag);
	DBProcess::type_t pushdata(DB_PROC_EXPRESS_INPUT_NAS, data);

	int index = desc->m_index % thread_count_;
	db_process_[index].queue_.push_signal(pushdata);
}

void DBManager::PushExpressInputItemNotConnectUser( int charIndex, ExpressSystemItemInfo* itemInfo, bool contentsFlag )
{
	CItemProto* itemProto = gserver->m_itemProtoList.FindIndex(itemInfo->item_index);
	if (itemProto == NULL)
	{
		LOG_ERROR("Not found item. userIndex : %d, itemIndex : %d, itemCount : %d", 0, itemInfo->item_index, itemInfo->item_count);
		return;
	}

	if (itemProto->getItemFlag() & ITEM_FLAG_COUNT)
	{
		if (itemProto->getStackCount() < itemInfo->item_count)
			itemInfo->item_count = itemProto->getStackCount();
	}
	else
	{
		itemInfo->item_count = 1;
	}

	itemInfo->sender[EXPRESS_SENDER_NAME] = '\0';

	DBProcess::expressinputitem_t data(0, charIndex, itemInfo);
	DBProcess::type_t pushdata(DB_PROC_EXPRESS_INPUT_ITEM, data);

	int index = 0;
	db_process_[index].queue_.push_signal(pushdata);
}

void DBManager::PushExpressInputNasNotConnectUser( int charIndex, unsigned int nas, int send_type, std::string sender, bool contentsFlag )
{
	DBProcess::expressinputnas_t data(0, 0, charIndex, nas, send_type, sender, contentsFlag);
	DBProcess::type_t pushdata(DB_PROC_EXPRESS_INPUT_NAS, data);

	int index = 0;
	db_process_[index].queue_.push_signal(pushdata);
}

// main thread에서 정상적으로 아이템(또는 돈)을 수령하였을 경우 창고NPC에게서 삭제함
void DBManager::PushExpressDelete( CDescriptor* desc, expressIndex_t expressIndex, bool send_flag )
{
	DBProcess::expressdelete_t data(desc->m_seq_index, desc->m_index, desc->m_pChar->m_index, expressIndex, send_flag);
	DBProcess::type_t pushdata(DB_PROC_EXPRESS_DELETE, data);

	int index = desc->m_index % thread_count_;
	db_process_[index].queue_.push_signal(pushdata);
}

void DBManager::pushQuery( int userIndex, std::string& query )
{
	DBProcess::type_t pushdata(DB_PROC_QUERY, query);

	int index = userIndex % thread_count_;
	db_process_[index].queue_.push_signal(pushdata);
}

void DBManager::MySQLPing()
{
	DBProcess::type_t pushdata(DB_PROC_MYSQL_PING, true);

	for (int i = 0; i < thread_count_; ++i)
	{
		db_process_[i].queue_.push_signal(pushdata);
	}
}

void DBManager::pushSendAnyInfoAfterLogin( CDescriptor* desc )
{
	DBProcess::sendanyinfoafterlogin_t data(desc->m_seq_index, desc->m_index, desc->m_pChar->m_index);
	DBProcess::type_t pushdata(DB_PROC_SEND_ANY_INFO_AFTER_LOGIN, data);

	int index = desc->m_index % thread_count_;
	db_process_[index].queue_.push_signal(pushdata);
}

void DBManager::PushExpressSearchNickName( CDescriptor* desc, const char* nickName )
{
	DBProcess::expresssearchnickname_t data(desc->m_seq_index, desc->m_index, nickName);
	DBProcess::type_t pushdata(DB_PROC_SEARCH_NICKNAME_FOR_EXPRESS, data);

	int index = desc->m_index % thread_count_;
	db_process_[index].queue_.push_signal(pushdata);
}

void DBManager::PushExpressSendItemToNickName( CDescriptor* desc, ExpressSystemItemInfo* itemInfo, std::string receiver, int sender_type )
{
	DBProcess::expresssenditemtonickname_t data(desc->m_seq_index, desc->m_index, desc->m_pChar->m_index, itemInfo, receiver, sender_type);
	DBProcess::type_t pushdata(DB_PROC_SEND_ITEM_TO_NICKNAME, data);

	int index = desc->m_index % thread_count_;
	db_process_[index].queue_.push_signal(pushdata);
}

void DBManager::pushDelCharForGM( CDescriptor* desc, std::string& del_charname )
{
	DBProcess::delcharforgm_t data(desc->m_seq_index, desc->m_index, del_charname);
	DBProcess::type_t pushdata(DB_PROC_DEL_CHARACTER_FOR_GM, data);

	int index = desc->m_index % thread_count_;
	db_process_[index].queue_.push_signal(pushdata);
}

void DBManager::pushQueryForAuthDB( int userIndex, std::string& query )
{
	DBProcess::type_t pushdata(DB_PROC_QUERY_FOR_AUTH_DB, query);

	int index = userIndex % thread_count_;
	db_process_[index].queue_.push_signal(pushdata);
}

void DBManager::pushQueryForDataDB( int userIndex, std::string& query )
{
	DBProcess::type_t pushdata(DB_PROC_QUERY_FOR_DATA_DB, query);

	int index = userIndex % thread_count_;
	db_process_[index].queue_.push_signal(pushdata);
}

void DBManager::pushGetCharIndexByGPS( CDescriptor* desc, std::string& name, int itemVindex )
{
	DBProcess::getcharindexbyGPS_t data(desc->m_seq_index, desc->m_index, name, itemVindex);
	DBProcess::type_t pushdata(DB_PROC_GET_CHAR_INDEX_BY_GPS, data);

	int index = desc->m_index % thread_count_;
	db_process_[index].queue_.push_signal(pushdata);
}

//////////////////////////////////////////////////////////////////////////
DBProcess::DBProcess()
	: is_connect(false)
	, castle_db_(char_db_)
	, charingame_db_(char_db_)
	, trigger_db_(char_db_)
{
	std::string tstr = "";
	for (int i = 0; i < ITEMS_PER_ROW; ++i)
	{
		tstr = boost::str(boost::format("a_item_idx%d") % i);
		a_item_idx_str.push_back(tstr);

		tstr = boost::str(boost::format("a_plus%d") % i);
		a_plus_str.push_back(tstr);

		tstr = boost::str(boost::format("a_wear_pos%d") % i);
		a_wear_pos_str.push_back(tstr);

		tstr = boost::str(boost::format("a_flag%d") % i);
		a_flag_str.push_back(tstr);

		tstr = boost::str(boost::format("a_serial%d") % i);
		a_serial_str.push_back(tstr);

		tstr = boost::str(boost::format("a_count%d") % i);
		a_count_str.push_back(tstr);

		tstr = boost::str(boost::format("a_used%d") % i);
		a_used_str.push_back(tstr);

		tstr = boost::str(boost::format("a_used%d_2") % i);
		a_used2_str.push_back(tstr);

		for (int j = 0; j < MAX_ITEM_OPTION; ++j)
		{
			tstr = boost::str(boost::format("a_item%d_option%d") % i % j);
			a_item_option_str[i].push_back(tstr);
		}

		for (int j = 0; j < MAX_VARIATION_COUNT; ++j)
		{
			tstr = boost::str(boost::format("a_item_%d_origin_var%d") % i % j);
			a_item_origin_str[i].push_back(tstr);
		}

		tstr = boost::str(boost::format("a_socket%d") % i);
		a_socket_str.push_back(tstr);

#ifdef DURABILITY
		tstr = boost::str(boost::format("a_now_dur_%d") % i);
		a_now_durability_str.push_back(tstr);

		tstr = boost::str(boost::format("a_max_dur_%d") % i);
		a_max_durability_str.push_back(tstr);
#endif
	}

	for (int i = 0; i < MAX_MEMPOS; ++i)
	{
		tstr = boost::str(boost::format("a_mem_%d") % i);
		a_mempos_comment.push_back(tstr);
	}
}

bool DBProcess::Connect()
{
	mysqldb::connectInfo char_db_info;
	char_db_info.host_ = gserver->m_config.Find("Char DB", "IP");
	char_db_info.port_ = 0;
	char_db_info.user_ = gserver->m_config.Find("Char DB", "User");
	char_db_info.pw_ = gserver->m_config.Find("Char DB", "Password");
	char_db_info.dbname_ = gserver->m_config.Find("Char DB", "DBName");
	char_db_info.charsetname_ = "";
	if (char_db_.connect(char_db_info) == false)
		return false;

	//////////////////////////////////////////////////////////////////////////

	mysqldb::connectInfo data_db_info;
	data_db_info.host_ = gserver->m_config.Find("Data DB", "IP");
	data_db_info.port_ = 0;
	data_db_info.user_ = gserver->m_config.Find("Data DB", "User");
	data_db_info.pw_ = gserver->m_config.Find("Data DB", "Password");
	data_db_info.dbname_ = gserver->m_config.Find("Data DB", "DBName");
	data_db_info.charsetname_ = "";
	if (data_db_.connect(data_db_info) == false)
		return false;

	//////////////////////////////////////////////////////////////////////////

#ifdef USE_TENTER_BILLING
	//////////////////////////////////////////////////////////////////////////
	mysqldb::connectInfo catal_db_info;
	catal_db_info.host_ = gserver->m_config.Find("Catalog DB", "IP");
	catal_db_info.port_ = 0;
	catal_db_info.user_ = gserver->m_config.Find("Catalog DB", "User");
	catal_db_info.pw_ = gserver->m_config.Find("Catalog DB", "Password");
	catal_db_info.dbname_ = gserver->m_config.Find("Catalog DB", "DBName");
	catal_db_info.charsetname_ = "";
	if (catal_db_.connect(catal_db_info) == false)
		return false;
#endif

#ifdef STASH_PASSWORD
	mysqldb::connectInfo auth_db_info;
	auth_db_info.host_ = gserver->m_config.Find("Auth DB", "IP");
	auth_db_info.port_ = 0;
	auth_db_info.user_ = gserver->m_config.Find("Auth DB", "User");
	auth_db_info.pw_ = gserver->m_config.Find("Auth DB", "Password");
	auth_db_info.dbname_ = gserver->m_config.Find("Auth DB", "DBName");
	auth_db_info.charsetname_ = "";
	if (auth_db_.connect(auth_db_info) == false)
		return false;
#endif

	is_connect = true;

	return true;
}

void DBProcess::Run()
{
	LOG_INFO("DBProcess thread start .........................");

	regist_signal_stack();

	while (1)
	{
		type_t data = queue_.wait_front_pop();
		if (data.first == DB_PROC_EXIT)
			break;

		//LOG_INFO("DEBUG_FUNC : START : data.first : %d", data.first);

		switch(data.first)
		{
		case DB_PROC_CHARACTER_LIST:
			CharacterList(data.second);
			break;

		case DB_PROC_CREATE_CHAR:
			CreateChar(data.second);
			break;

		case DB_PROC_DELETE_CHAR:
			DeleteChar(data.second);
			break;

		case DB_PROC_CANCEL_DELETE_CHAR:
			CancelDeleteChar(data.second);
			break;

		case DB_PROC_SELECT_CHAR:
			SelectChar(data.second);
			break;

		case DB_PROC_SAVE_CHAR:
			SaveChar(data.second);
			break;

		case DB_PROC_EXPRESS_EXIST:
			ExpressExist(data.second);
			break;

		case DB_PROC_EXPRESS_LIST:
			ExpressList(data.second);
			break;

		case DB_PROC_EXPRESS_TAKE:
			ExpressTake(data.second);
			break;

		case DB_PROC_EXPRESS_INPUT_ITEM:
			ExpressInputItem(data.second);
			break;

		case DB_PROC_EXPRESS_INPUT_NAS:
			ExpressInputNas(data.second);
			break;

		case DB_PROC_EXPRESS_DELETE:
			ExpressDelete(data.second);
			break;

		case DB_PROC_QUERY:
			QueryProcess(data.second);
			break;

		case DB_PROC_MYSQL_PING:
			MySQLPing();
			break;

		case DB_PROC_SEND_ANY_INFO_AFTER_LOGIN:
			sendAnyInfoAfterLogin(data.second);
			break;

		case DB_PROC_SEARCH_NICKNAME_FOR_EXPRESS:
			ExpressSearchNickName(data.second);
			break;

		case DB_PROC_SEND_ITEM_TO_NICKNAME:
			ExpressSendItemToNickName(data.second);
			break;

		case DB_PROC_DEL_CHARACTER_FOR_GM:
			DelCharForGM(data.second);
			break;

#ifdef STASH_PASSWORD
		case DB_PROC_QUERY_FOR_AUTH_DB:
			QueryProcessForAuthDB(data.second);
			break;
#endif

		case DB_PROC_QUERY_FOR_DATA_DB:
			QueryProcessForDataDB(data.second);
			break;

		case DB_PROC_GET_CHAR_INDEX_BY_GPS:
			getCharIndexByGPS(data.second);
			break;

		default:
			LOG_ERROR("Invalid Command[%d]", data.first);
			break;
		}
		//LOG_INFO("DEBUG_FUNC : END : data.first : %d", data.first);
	}

	LOG_INFO("DBProcess thread end .........................");
}

//////////////////////////////////////////////////////////////////////////

void  DBProcess::SendMessageToClient( LONGLONG& seq_index, int userIndex, CNetMsg::SP& msg )
{
	EventProcessForDB::sendMessageToClient data;
	data.seq_index = seq_index;
	data.user_index = userIndex;
	data.msg = msg;

	EventProcessForDB::instance()->pushSendMessageToClient(data);
}

//////////////////////////////////////////////////////////////////////////

//XX 로그인 4
void DBProcess::CharacterList( boost::any& argv )
{
	charlist_t data = boost::any_cast<charlist_t>(argv);
	LONGLONG seq_index = boost::tuples::get<0>(data);
	int& m_index = boost::tuples::get<1>(data);
	int& m_playmode = boost::tuples::get<2>(data);
	unsigned int& m_nPrepareSeed = boost::tuples::get<3>(data);

	EventProcessForDB::charlist return_charlist(m_index);
	return_charlist.m_seq_index = seq_index;

	int slot0 = 0, slot1 = 0;
	std::string query;

	if (m_playmode == MSG_LOGIN_RE)
	{
		// 서버 이동일 경우 케릭터 슬롯 정보는 안 보낸다
		CNetMsg::SP rmsg(new CNetMsg);
		DBCharEndMsg(rmsg, slot0, slot1, m_nPrepareSeed);
		SendMessageToClient(seq_index, m_index, rmsg);
		return;
	}

	{
		int remainTime = 0;
		LONGLONG endTime = 0;

		query = boost::str(boost::format(
							   "SELECT UNIX_TIMESTAMP(a_end_datetime) - UNIX_TIMESTAMP(NOW()) as remainTime, "
							   "UNIX_TIMESTAMP(a_end_datetime) as endTime FROM t_extendcharslot WHERE a_user_index = %d") % m_index);
		BOOST_MYSQL_RES res = char_db_.select(query);
		if (res == NULL)
			return;

		if (char_db_.getrowcount() > 0)
		{
			MYSQL_ROW row = mysql_fetch_row(res.get());
			remainTime = atoi(row[0]);
			endTime = atoi(row[1]);
			if(remainTime < 0)
				remainTime = 0;
		}

		return_charlist.m_tExtendCharacterSlotTime = (time_t)endTime;

		{
			CNetMsg::SP rmsg(new CNetMsg);
			DBCharSlotExtTime(rmsg, (long)remainTime);
			this->SendMessageToClient(seq_index, m_index, rmsg);
		}
	}

	{
		// 해당 유저의 삭제 대기 중 캐릭중에 삭제 될 녀석들 지우기
		{
			query = boost::str(boost::format(
								   "SELECT a_index FROM t_characters WHERE a_user_index=%d AND a_server=%d AND a_enable=1 "
								   "AND a_deletedelay!=0 AND a_deletedelay<=UNIX_TIMESTAMP(NOW())") % m_index % gserver->m_serverno);
			BOOST_MYSQL_RES res = char_db_.select(query);
			if(res != NULL && char_db_.getrowcount() > 0)
			{
				int rows = char_db_.getrowcount();
				for (int i = 0; i < rows; ++i)
				{
					MYSQL_ROW row = mysql_fetch_row(res.get());
					int nCharIndex = atoi(row[0]);

					CNetMsg::SP rmsg(new CNetMsg);
					HelperCharDelMsg(rmsg, nCharIndex);
					SEND_Q(rmsg, gserver->m_helper);
				}
			}
		}

		{
			query = boost::str(boost::format(
								   "UPDATE t_characters SET a_enable=0, a_datestamp=now()"
								   " WHERE a_user_index=%d AND a_server=%d AND a_enable=1 AND a_deletedelay!=0 AND a_deletedelay<=UNIX_TIMESTAMP(NOW())")
							   % m_index % gserver->m_serverno);
			if (char_db_.excute(query) == false)
				return;
		}
	}

	query = boost::str(boost::format(
						   "SELECT *, IF(a_deletedelay-UNIX_TIMESTAMP(NOW()) < 0, 0, a_deletedelay-UNIX_TIMESTAMP(NOW())) AS `a_delete_delay_reamin`"
						   " FROM t_characters WHERE a_user_index=%d AND a_server=%d") % m_index % gserver->m_serverno);
	query += " AND a_enable=1 ORDER BY a_index";

	BOOST_MYSQL_RES res = char_db_.select(query);
	if (res == NULL)
		return;

	// 루프 : 있는 만큼
	int row_count = char_db_.getrowcount();
	for (int rc = 0; rc < row_count; ++rc)
	{
		MYSQL_ROW row = mysql_fetch_row(res.get());

		int a_index = atoi(row[char_db_.findfield("a_index")]);
		return_charlist.char_index = a_index;

		int wear[DEFAULT_MAX_WEARING];

		std::string wearstring = row[char_db_.findfield("a_wearing")];
		stokenizer tok(wearstring, sep);
		std::vector<std::string> wearvec(tok.begin(), tok.end());
		if (wearvec.size() < (DEFAULT_MAX_WEARING * 2))
		{
			for (int i = wearvec.size(); i < (DEFAULT_MAX_WEARING * 2); ++i)
			{
				wearvec.push_back("0");
			}
		}

		for (int i = 0; i < DEFAULT_MAX_WEARING; i++)
		{
			if ( ( i >= WEARING_SHOW_START && i <= WEARING_BOOTS )
					|| ( i == WEARING_BACKWING )
			   )
			{
				wear[i - WEARING_SHOW_START] = atoi(wearvec[i].c_str());
				CItemProto* proto = gserver->m_itemProtoList.FindIndex(wear[i - WEARING_SHOW_START]);
				if (!proto || proto->getItemWearing() < WEARING_HELMET || proto->getItemWearing() >= DEFAULT_MAX_WEARING)
					wear[i - WEARING_SHOW_START] = -1;
			}
		}
		// 050223 : bs : plus 효과를 위한 정보 추가
		int plus[DEFAULT_MAX_WEARING];
		for (int i = 0; i < DEFAULT_MAX_WEARING; i++)
		{
			if ( ( i >= WEARING_SHOW_START && i <= WEARING_BOOTS )
					|| ( i == WEARING_BACKWING )
			   )
			{
				plus[i - WEARING_SHOW_START] = atoi(wearvec[i + DEFAULT_MAX_WEARING].c_str());
				CItemProto* proto = gserver->m_itemProtoList.FindIndex(wear[i - WEARING_SHOW_START]);
				if (!proto || proto->getItemWearing() < WEARING_HELMET || proto->getItemWearing() >= DEFAULT_MAX_WEARING)
					plus[i - WEARING_SHOW_START] = 0;
			}
		}
		// --- 050223 : bs : plus 효과를 위한 정보 추가
		// 캐릭터 삭제 딜레이
		int nDeleteDelayRemain = 0;
		nDeleteDelayRemain = atoi(row[char_db_.findfield("a_delete_delay_reamin")]);
		if (nDeleteDelayRemain < 0)
			nDeleteDelayRemain = 0;

		char job = (char)atoi(row[char_db_.findfield("a_job")]);
		int level = atoi(row[char_db_.findfield("a_level")]);

		CNetMsg::SP rmsg(new CNetMsg);
		DBCharExistMsg(rmsg,
					   a_index,
					   row[char_db_.findfield("a_name")],
					   row[char_db_.findfield("a_nick")],
					   job,
					   (char)atoi(row[char_db_.findfield("a_job2")]),
					   (char)atoi(row[char_db_.findfield("a_hair_style")]),
					   (char)atoi(row[char_db_.findfield("a_face_style")]),
					   level,
					   ATOLL(row[char_db_.findfield("a_exp")]),
					   atoi(row[char_db_.findfield("a_skill_point")]),
					   atoi(row[char_db_.findfield("a_hp")]),
					   atoi(row[char_db_.findfield("a_max_hp")]),
					   atoi(row[char_db_.findfield("a_mp")]),
					   atoi(row[char_db_.findfield("a_max_mp")]),
					   wear,
					   plus,
					   nDeleteDelayRemain);

		// 길드 정보 가져오기
		{
			CDBCmd dbGuild;
			dbGuild.Init(char_db_.getMYSQL());

			CLCString guildName(MAX_GUILD_NAME_LENGTH + 1);
			std::string select_guild_query = boost::str(boost::format(
												 "SELECT g.a_name FROM t_guildmember AS gm INNER JOIN t_guild AS g ON g.a_index = gm.a_guild_index WHERE gm.a_char_index = %d and g.a_enable = 1") % a_index);
			dbGuild.SetQuery(select_guild_query);
			if (dbGuild.Open() && dbGuild.m_nrecords > 0 && dbGuild.MoveNext())
			{
				dbGuild.GetRec( "a_name", guildName);
			}
			RefMsg(rmsg) << guildName;
		}

		// 90 Level 캐릭터 검사
		if( job == JOB_NIGHTSHADOW )
			return_charlist.m_bNotCreateNightShadow = true;

		if( level >= 90 && job != JOB_NIGHTSHADOW )
			return_charlist.m_bCreate90Lv = true;

		SendMessageToClient(seq_index, m_index, rmsg);
	}

	{
		// 아이템은 쓴적있는지 확인한다
		query = boost::str(boost::format("SELECT * FROM t_nscard WHERE a_user_index = %d") % m_index);
		BOOST_MYSQL_RES res = char_db_.select(query);
		if(res != NULL && char_db_.getrowcount() > 0)
		{
			return_charlist.m_bIsNSCreateCardUse = true;
		}

		if( (return_charlist.m_bCreate90Lv && !(return_charlist.m_bNotCreateNightShadow)) )
		{
			// 90레벨이상 캐릭터 있고 NS 캐릭터가 없다
			CNetMsg::SP rmsg(new CNetMsg);
			DBNSCreateMsg(rmsg);
			SendMessageToClient(seq_index, m_index, rmsg);
		}
		else if(!return_charlist.m_bCreate90Lv && !(return_charlist.m_bNotCreateNightShadow) )
		{
			// 90레벨이상 캐릭터 없고 NS 캐릭터가 없다
			if(return_charlist.m_bIsNSCreateCardUse)  // 아이템 사용여부
			{
				CNetMsg::SP rmsg(new CNetMsg);
				DBNSCreateMsg(rmsg);
				SendMessageToClient(seq_index, m_index, rmsg);
			}
		}
	}

	{
		// 캐릭터 정보를 읽으면서 CDescriptor를 변경할 정보를 Event Session을 이용하여 전달
		EventProcessForDB::instance()->pushCharList(return_charlist);
	}

	{
		// 캐릭터 마지막 정보 전송
		CNetMsg::SP rmsg(new CNetMsg);
		DBCharEndMsg(rmsg, slot0, slot1, m_nPrepareSeed);
		SendMessageToClient(seq_index, m_index, rmsg);
	}
}

//XX 캐릭터삭제 3
void DBProcess::DeleteChar( boost::any& argv )
{
	delchar_t data = boost::any_cast<delchar_t>(argv);
	LONGLONG seq_index = boost::tuples::get<0>(data);
	int& m_index = boost::tuples::get<1>(data);
	int& char_index = boost::tuples::get<2>(data);
	time_t& m_tExtendCharacterSlotTime = boost::tuples::get<3>(data);
	int& m_guildoutdate = boost::tuples::get<4>(data);

	EventProcessForDB::deleteChar retVal;
	retVal.m_index = m_index;
	retVal.m_seq_index = seq_index;

	LONGLONG _nowTime = 0;

	{
		std::string query = "SELECT UNIX_TIMESTAMP(NOW()) as nowTime";
		BOOST_MYSQL_RES res = char_db_.select(query);
		if (res == NULL || char_db_.getrowcount() <= 0)
		{
			_nowTime = mktime(&gserver->m_tRealSystemTime);
		}
		else
		{
			MYSQL_ROW row = mysql_fetch_row(res.get());
			_nowTime = ATOLL(row[0]);
		}
	}

	if(m_tExtendCharacterSlotTime <= (time_t)_nowTime)
	{
		std::string query = boost::str(boost::format(
										   "SELECT a_index FROM t_characters WHERE a_enable = 1 AND a_user_index = %d ORDER BY a_index") % m_index);
		BOOST_MYSQL_RES res = char_db_.select(query);
		if(res != NULL && char_db_.getrowcount() > 0)
		{
			int count = 1;
			int row_count = char_db_.getrowcount();
			for (int i = 0; i < row_count; ++i, ++count)
			{
				MYSQL_ROW row = mysql_fetch_row(res.get());
				int a_index = atoi(row[0]);
				if (a_index == char_index)
					break;
			}

			if(count > 4)
			{
				// 삭제 불가 MSG_FAIL_DB_CANNOT_DEL_CHAR
				CNetMsg::SP rmsg(new CNetMsg);
				FailMsg(rmsg, MSG_FAIL_DB_CANNOT_DEL_CHAR);
				SendMessageToClient(seq_index, m_index, rmsg);
				return;
			}
		}
	}

	{
		std::string query = "";

		{
			// 가미고 로그를 출력하기 위해 이름과 직업을 얻어옴
			query = boost::str(boost::format(
								   "SELECT a_name, a_job FROM t_characters WHERE a_index=%1% AND a_user_index=%2% AND a_server=%3% LIMIT 1")
							   % char_index % m_index % gserver->m_serverno);
			BOOST_MYSQL_RES res = char_db_.select(query);
			if (res == NULL || char_db_.getrowcount() <= 0)
			{
				CNetMsg::SP rmsg(new CNetMsg);
				FailMsg(rmsg, MSG_FAIL_DB_UNKNOWN);
				SendMessageToClient(seq_index, m_index, rmsg);
				return;
			}

			MYSQL_ROW row = mysql_fetch_row(res.get());
			retVal.name = row[0];
			retVal.job = atoi(row[1]);
		}

		query = "UPDATE t_characters SET a_deletedelay=UNIX_TIMESTAMP(NOW())+24*60*60, a_datestamp=now()";

#if defined (LC_GAMIGO)
		query += boost::str(boost::format(", a_guildoutdate=%1%") % m_guildoutdate);
#endif

		query += boost::str(boost::format(" WHERE a_user_index=%1% AND a_server=%2% AND a_index=%3% AND a_enable=1")
							% m_index % gserver->m_serverno % char_index);
		if (char_db_.excute(query))
		{
			CNetMsg::SP rmsg(new CNetMsg);
			DBSuccessMsg(rmsg);
			SendMessageToClient(seq_index, m_index, rmsg);

			EventProcessForDB::instance()->pushDeleteCharacter(retVal);
		}
		else
		{
			CNetMsg::SP rmsg(new CNetMsg);
			FailMsg(rmsg, MSG_FAIL_DB_UNKNOWN);
			SendMessageToClient(seq_index, m_index, rmsg);
		}
	}
}

//XX 캐릭터복원 3
void DBProcess::CancelDeleteChar( boost::any& argv )
{
	canceldelchar_t data = boost::any_cast<canceldelchar_t>(argv);
	LONGLONG seq_index = boost::tuples::get<0>(data);
	int& m_index = boost::tuples::get<1>(data);
	int& char_index = boost::tuples::get<2>(data);

	std::string query = boost::str(boost::format(
									   "UPDATE t_characters SET a_deletedelay=0, a_datestamp=now() WHERE a_user_index=%d AND a_server=%d AND a_index=%d AND a_enable=1")
								   % m_index % gserver->m_serverno % char_index);
	if (char_db_.excute(query))
	{
		CNetMsg::SP rmsg(new CNetMsg);
		DBSuccessMsg(rmsg);
		SendMessageToClient(seq_index, m_index, rmsg);

		EventProcessForDB::cancleDeleteChar retVal;
		retVal.m_index = m_index;
		retVal.m_seq_index = seq_index;
		EventProcessForDB::instance()->pushCancelDeleteCharacter(retVal);
	}
	else
	{
		CNetMsg::SP rmsg(new CNetMsg);
		FailMsg(rmsg, MSG_FAIL_DB_UNKNOWN);
		SendMessageToClient(seq_index, m_index, rmsg);
	}
}

//////////////////////////////////////////////////////////////////////////
void DBProcess::FixItemUsedTime(CItem* pItem)
{
	CItemProto* proto = pItem->m_itemProto;
	if ( proto == NULL )
		return;

	int itemIndex = proto->getItemIndex();

	int dbTime = 0;

	// 기간제, 일회용, 캐쉬아이템 : 창고 이용 주문서, 잡화상 이용 주문서, 써치라이프, 아이템판매대행상인, 스킬마스터이용권
	if ( proto->getItemTypeIdx() == ITYPE_ONCE && proto->getItemSubTypeIdx() == IONCE_CASH && proto->getItemNum3() > 0 )
		dbTime = proto->getItemNum3() * 60 * 60;

	// 파티리콜
	if ( proto->getItemTypeIdx() == ITYPE_ONCE && proto->getItemSubTypeIdx() == IONCE_WARP && proto->getItemNum0() == IONCE_WARP_PARTYRECALL )
		dbTime = proto->getItemNum3() * 60 * 60;

	// 오토포션
	if ( itemIndex == ONE_PERIOD_ITEM || itemIndex == 2610 || itemIndex == 4940 )
		dbTime = 1 * 24 * 60 * 60;

	if ( itemIndex == SEVEN_PERIOD_ITEM || itemIndex == 4941 )
		dbTime = 7 * 24 * 60 * 60;

	if ( itemIndex == THIRTY_PERIOD_ITEM || itemIndex == 4942 )
		dbTime = 30 * 24 * 60 * 60;

	if (proto->isRvrJewel())
		dbTime = proto->getItemNum3() * 60;

	// 결합 아이템
	if ( proto->getItemFlag() & ITEM_FLAG_COMPOSITE )
	{
		if ( pItem->getFlag() & FLAG_ITEM_COMPOSITION )
		{
			// 결합한 상태
			if ( proto->getItemFlag() & ITEM_FLAG_ABS )
			{
				// 시간제 : used 값에 남은 시간, used_2 값에 만료 기간을 저장
				dbTime = proto->getItemNum4() * 60 * 60;
				dbTime += gserver->getNowSecond();

				// 만료 기간 = 남은 시간 + 게임 타임
				if ( pItem->getUsed_2() > dbTime )
					pItem->setUsed_2(pItem->getUsed() + gserver->getNowSecond());

				return;
			}
			else
			{
				// 기간제 : used, used_2 값에 만료 기간을 저장
				dbTime = proto->getItemNum4() * 24 * 60 * 60;
				dbTime += gserver->getNowSecond();

				if ( pItem->getUsed() > dbTime || pItem->getUsed_2() > dbTime )
				{
					pItem->setUsed(dbTime);
					pItem->setUsed_2(dbTime);
				}


				return;
			}
		}
		else
		{
			// 결합하지 않은 상태면
			if ( proto->getItemFlag() & ITEM_FLAG_ABS )
			{
				// 시간제 : used에 남은 시간, used_2는 사용안함(-1)
				return;
			}
			else
			{
				// 기간제 : used에 만료 기간, used_2는 사용안함(-1)
				dbTime = proto->getItemNum4() * 24 * 60 * 60;
			}
		}
	}

	// 플래티늄 아이템
	// 플래티늄 아이템은 최대 30일보다 값이 크면, 30일로 맞춘다.
	int platinumPlus = 0;
	FLAG_ITEM_PLATINUM_GET(pItem->getFlag(), platinumPlus);
	if ( platinumPlus > 0 && (pItem->m_itemProto->getItemTypeIdx() == ITYPE_WEAPON || pItem->m_itemProto->getItemTypeIdx() == ITYPE_WEAR))
		dbTime = 30 * 24 * 60 * 60;

	// 코스튬2 아이템
	if ( proto->getItemFlag() & ITEM_FLAG_COSTUME2 )
		dbTime = proto->getItemNum4() * 24 * 60 * 60;

#ifdef SYSTEM_TREASURE_MAP
	// 보물상자
	if ( itemIndex == TREASURE_MAP_ITEM_INDEX )
		dbTime = proto->getItemNum4() * 60;
#endif

	// 대여 아이템
	if ( pItem->IsLent() )
		dbTime =  24 * 60 * 60;

	// 라카 기자단 아이템 30일
	if ( itemIndex == 2882 )
		dbTime = 30 * 24 * 60 * 60;

	if ( proto->getItemTypeIdx() == ITYPE_ACCESSORY && proto->getItemNum4() > 0 )
		// 기간제 악세서리 아이템
		dbTime = proto->getItemNum4() * 24 * 60 * 60;

	// 기간제 아이템 아니면 리턴
	if ( dbTime == 0 )
		return;

	dbTime += gserver->getNowSecond();

	// 남은 기간이 설정 가능한 최대 시간보다 크면 최대 시간으로 수정
	if ( pItem->getUsed() > dbTime )
		pItem->setUsed(dbTime);
}

//////////////////////////////////////////////////////////////////////////
#ifdef DURABILITY
static const std::string express_select_string = "SELECT a_index, a_item_index, a_plus, a_plus2, a_flag, a_serial,"
		"a_item_count, a_option_count, a_option_0_type, a_option_0_level, a_option_1_type,"
		"a_option_1_level, a_option_2_type, a_option_2_level, a_option_3_type, a_option_3_level, a_option_4_type, a_option_4_level,"
		"a_socket0, a_socket1, a_socket2, a_socket3, a_socket4, a_socket5, a_socket6,"
		"a_item_origin_var0, a_item_origin_var1, a_item_origin_var2, a_item_origin_var3, a_item_origin_var4, a_item_origin_var5, a_now_dur, a_max_dur,"
		"a_nas, a_send_type, a_sender, UNIX_TIMESTAMP(a_registe_date) as a_registe_date, UNIX_TIMESTAMP(a_expire_date) as a_expire_date, a_tradeagent_nas, a_tradeagent_itemIndex, a_tradeagent_itemCount FROM t_express_system ";
#else
static const std::string express_select_string = "SELECT a_index, a_item_index, a_plus, a_plus2, a_flag, a_serial,"
		"a_item_count, a_option_count, a_option_0_type, a_option_0_level, a_option_1_type,"
		"a_option_1_level, a_option_2_type, a_option_2_level, a_option_3_type, a_option_3_level, a_option_4_type, a_option_4_level,"
		"a_socket0, a_socket1, a_socket2, a_socket3, a_socket4, a_socket5, a_socket6,"
		"a_item_origin_var0, a_item_origin_var1, a_item_origin_var2, a_item_origin_var3, a_item_origin_var4, a_item_origin_var5,"
		"a_nas, a_send_type, a_sender, UNIX_TIMESTAMP(a_registe_date) as a_registe_date, UNIX_TIMESTAMP(a_expire_date) as a_expire_date, a_tradeagent_nas, a_tradeagent_itemIndex, a_tradeagent_itemCount FROM t_express_system ";
#endif

void DBProcess::ExpressExist( boost::any& argv )
{
	expressexist_t data = boost::any_cast<expressexist_t>(argv);
	LONGLONG seq_index = boost::tuples::get<0>(data);
	int& user_index = boost::tuples::get<1>(data);
	int& char_index = boost::tuples::get<2>(data);
	int& send_type = boost::tuples::get<3>(data);

	std::string qry = boost::str(boost::format(
									 "SELECT a_index FROM t_express_system WHERE a_char_index=%1% AND now() < a_expire_date LIMIT 1") % char_index);
	BOOST_MYSQL_RES res = char_db_.select(qry);
	if (res == NULL)
		return;

	int flag = (char_db_.getrowcount() > 0) ? send_type : 0;
	CNetMsg::SP rmsg(new CNetMsg);
	ResponseClient::makeExpressExist(rmsg, flag);
	SendMessageToClient(seq_index, user_index, rmsg);
}

void DBProcess::ExpressList( boost::any& argv )
{
	expresslist_t data = boost::any_cast<expresslist_t>(argv);
	LONGLONG seq_index = boost::tuples::get<0>(data);
	int& user_index = boost::tuples::get<1>(data);
	int& char_index = boost::tuples::get<2>(data);
	int& pageIndex = boost::tuples::get<3>(data);

	int startIndex = pageIndex * EXPRESS_SYSTEM_COUNT_PER_PAGE;
	int endIndex = EXPRESS_SYSTEM_COUNT_PER_PAGE + 1;

	std::string qry = express_select_string;
	qry += boost::str(boost::format("WHERE a_char_index=%1% AND now() < a_expire_date LIMIT %2%, %3%")
					  % char_index % startIndex % endIndex);
	BOOST_MYSQL_RES res = char_db_.select(qry);
	if (res == NULL)
		return;

	// 리스트를 전송하자
	CNetMsg::SP rmsg(new CNetMsg);
	ResponseClient::expressList* p = reinterpret_cast<ResponseClient::expressList*>(rmsg->m_buf);
	p->type = MSG_EXPRESS_SYSTEM;
	p->subType = MSG_SUB_EXPREES_LIST;
	p->nowPage = pageIndex;
	if (char_db_.getrowcount() <= EXPRESS_SYSTEM_COUNT_PER_PAGE)
	{
		p->nextPage = 0;
		p->count = char_db_.getrowcount();
	}
	else
	{
		// 한 페이지에 표시하는 숫자보다 많다는 얘기는, 다음 페이지를 표시할 수 있다는 얘기
		p->nextPage = 1;
		p->count = EXPRESS_SYSTEM_COUNT_PER_PAGE;
	}

	for (int i = 0; i < p->count; ++i)
	{
		getItemByExpress(p->list + i, res.get());
	}

	rmsg->setSize(sizeof(ResponseClient::expressList) + (p->count * sizeof(ExpressSystemItemInfo)));
	SendMessageToClient(seq_index, user_index, rmsg);
}

//XX 거래대행 - 찾기 3
void DBProcess::ExpressTake( boost::any& argv )
{
	expresstake_t data = boost::any_cast<expresstake_t>(argv);
	LONGLONG seq_index = boost::tuples::get<0>(data);
	int& m_index = boost::tuples::get<1>(data);
	int& char_index = boost::tuples::get<2>(data);
	expressIndex_t& index = boost::tuples::get<3>(data);

	std::string qry = express_select_string;
	qry += boost::str(boost::format("WHERE a_index=%1% AND a_char_index=%2% LIMIT 1") % index % char_index);
	BOOST_MYSQL_RES res = char_db_.select(qry);
	if (res == NULL)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::makeExpressTake(rmsg, ResponseClient::ERR_DB_ERROR);
		SendMessageToClient(seq_index, m_index, rmsg);

		LOG_ERROR("EXPRESS SYSTEM TAKE. NOT EXIST ITEM(res) index : %d, charIndex : %d", index, char_index);
		return;
	}

	if (char_db_.getrowcount() == 0)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::makeExpressTake(rmsg, ResponseClient::ERR_NO_TAKE_ITEM);
		SendMessageToClient(seq_index, m_index, rmsg);

		LOG_ERROR("EXPRESS SYSTEM TAKE. NOT EXIST ITEM(row) index : %d, charIndex : %d", index, char_index);
		return;
	}

	// main thread로 정보를 전달하여 아이템(또는 돈을 수령하도록 한다)
	// 포인터 삭제는 main thread에서 처리함
	EventProcessForDB::expressTake retData;
	retData.m_index = m_index;
	getItemByExpress(&retData.itemInfo, res.get());
	EventProcessForDB::instance()->pushExpressTake(retData);
}

void DBProcess::ExpressInputItem( boost::any& argv )
{
	expressinputitem_t data = boost::any_cast<expressinputitem_t>(argv);
	LONGLONG seq_index = boost::tuples::get<0>(data);
	int& char_index = boost::tuples::get<1>(data);
	ExpressSystemItemInfo* itemInfo = boost::tuples::get<2>(data);
	bool contentsFlag = boost::tuples::get<3>(data);

	boost::scoped_ptr<ExpressSystemItemInfo> _auto_delete(itemInfo);		// 자동 삭제

	char tsender[128];
	mysql_real_escape_string(char_db_.getMYSQL(), tsender, itemInfo->sender, strlen(itemInfo->sender));

	std::string qry;
	qry.reserve(2048);
#ifdef DURABILITY
	qry = "INSERT INTO t_express_system(a_char_index, a_item_index, a_plus, a_plus2, a_flag, a_serial,"
		  "a_item_count, a_option_count, a_option_0_type, a_option_0_level, a_option_1_type,"
		  "a_option_1_level, a_option_2_type, a_option_2_level, a_option_3_type, a_option_3_level, a_option_4_type, a_option_4_level,"
		  "a_socket0, a_socket1, a_socket2, a_socket3, a_socket4, a_socket5, a_socket6,"
		  "a_item_origin_var0, a_item_origin_var1, a_item_origin_var2, a_item_origin_var3, a_item_origin_var4, a_item_origin_var5,a_now_dur,a_max_dur,"
		  "a_nas, a_send_type, a_sender, a_registe_date, a_expire_date) VALUES(";
#else
	qry = "INSERT INTO t_express_system(a_char_index, a_item_index, a_plus, a_plus2, a_flag, a_serial,"
		  "a_item_count, a_option_count, a_option_0_type, a_option_0_level, a_option_1_type,"
		  "a_option_1_level, a_option_2_type, a_option_2_level, a_option_3_type, a_option_3_level, a_option_4_type, a_option_4_level,"
		  "a_socket0, a_socket1, a_socket2, a_socket3, a_socket4, a_socket5, a_socket6,"
		  "a_item_origin_var0, a_item_origin_var1, a_item_origin_var2, a_item_origin_var3, a_item_origin_var4, a_item_origin_var5,"
		  "a_nas, a_send_type, a_sender, a_registe_date, a_expire_date) VALUES(";
#endif

	qry += boost::str(boost::format("%1%,") % char_index);
	qry += boost::str(boost::format("%1%,") % itemInfo->item_index);
	qry += boost::str(boost::format("%1%,") % (int)itemInfo->plus);
	qry += boost::str(boost::format("%1%,") % (int)itemInfo->plus2);
	qry += boost::str(boost::format("%1%,") % itemInfo->flag);
	qry += boost::str(boost::format("'%1%',") % itemInfo->serial);
	qry += boost::str(boost::format("%1%,") % itemInfo->item_count);
	qry += boost::str(boost::format("%1%,") % itemInfo->option_count);
	qry += boost::str(boost::format("%1%,") % itemInfo->option_type[0]);
	qry += boost::str(boost::format("%1%,") % itemInfo->option_level[0]);
	qry += boost::str(boost::format("%1%,") % itemInfo->option_type[1]);
	qry += boost::str(boost::format("%1%,") % itemInfo->option_level[1]);
	qry += boost::str(boost::format("%1%,") % itemInfo->option_type[2]);
	qry += boost::str(boost::format("%1%,") % itemInfo->option_level[2]);
	qry += boost::str(boost::format("%1%,") % itemInfo->option_type[3]);
	qry += boost::str(boost::format("%1%,") % itemInfo->option_level[3]);
	qry += boost::str(boost::format("%1%,") % itemInfo->option_type[4]);
	qry += boost::str(boost::format("%1%,") % itemInfo->option_level[4]);
	qry += boost::str(boost::format("%1%,") % itemInfo->socket[0]);
	qry += boost::str(boost::format("%1%,") % itemInfo->socket[1]);
	qry += boost::str(boost::format("%1%,") % itemInfo->socket[2]);
	qry += boost::str(boost::format("%1%,") % itemInfo->socket[3]);
	qry += boost::str(boost::format("%1%,") % itemInfo->socket[4]);
	qry += boost::str(boost::format("%1%,") % itemInfo->socket[5]);
	qry += boost::str(boost::format("%1%,") % itemInfo->socket[6]);
	qry += boost::str(boost::format("%1%,") % itemInfo->item_origin[0]);
	qry += boost::str(boost::format("%1%,") % itemInfo->item_origin[1]);
	qry += boost::str(boost::format("%1%,") % itemInfo->item_origin[2]);
	qry += boost::str(boost::format("%1%,") % itemInfo->item_origin[3]);
	qry += boost::str(boost::format("%1%,") % itemInfo->item_origin[4]);
	qry += boost::str(boost::format("%1%,") % itemInfo->item_origin[5]);
#ifdef DURABILITY
	qry += boost::str(boost::format("%1%,") % itemInfo->now_durability);
	qry += boost::str(boost::format("%1%,") % itemInfo->max_durability);
#endif
	qry += boost::str(boost::format("%1%,") % itemInfo->nas);
	qry += boost::str(boost::format("%1%,") % itemInfo->send_type);
	qry += boost::str(boost::format("'%1%',") % tsender);
	qry += boost::str(boost::format("now(), DATE_ADD(now(), INTERVAL %1% DAY))") % EXPRESS_SYSTEM_EXPIRE_DATE);

	if (char_db_.excute(qry) == false)
		return;

	// SubHelper로 패킷을 전달하여 해당 유저에게 창고NPC에 아이템(또는 돈)이 있다고 알려줌
	{
		CNetMsg::SP rmsg(new CNetMsg);
		RequestGMTool::sendItemToOneUser* packet = reinterpret_cast<RequestGMTool::sendItemToOneUser*>(rmsg->m_buf);
		packet->type = MSG_EXPRESS_SYSTEM;
		packet->subType = MSG_SUB_EXPRESS_ONE_USER;
		packet->char_index = char_index;
		packet->send_type = itemInfo->send_type;
		rmsg->setSize(sizeof(RequestGMTool::sendItemToOneUser));

		gserver->m_subHelper->WriteToOutput(rmsg);
	}

	// 로그 기록 (GM TOOL을 이용한 경우에만 로그를 기록함)
	if (contentsFlag)
		return;
}

void DBProcess::ExpressInputNas( boost::any& argv )
{
	expressinputnas_t data = boost::any_cast<expressinputnas_t>(argv);
	LONGLONG seq_index = boost::tuples::get<0>(data);
	int& user_index = boost::tuples::get<1>(data);
	int& char_index = boost::tuples::get<2>(data);
	unsigned int& nas = boost::tuples::get<3>(data);
	int send_type = boost::tuples::get<4>(data);
	std::string& sender = boost::tuples::get<5>(data);
	bool contentsFlag = boost::tuples::get<6>(data);

	char tsender[128];
	mysql_real_escape_string(char_db_.getMYSQL(), tsender, sender.c_str(), sender.length());

	std::string qry = boost::str(boost::format(
									 "INSERT INTO t_express_system(a_char_index,a_nas,a_send_type,a_sender,a_registe_date,a_expire_date) VALUES"
									 "(%1%,%2%,%3%,'%4%',now(),DATE_ADD(now(), INTERVAL %5% DAY))")
								 % char_index % nas % send_type % tsender % EXPRESS_SYSTEM_EXPIRE_DATE);
	if (char_db_.excute(qry) == false)
		return;

	// 해당 유저에게 창고NPC에 아이템(또는 돈)이 있다고 알려줌
	CNetMsg::SP rmsg(new CNetMsg);
	ResponseClient::makeExpressExist(rmsg, 1);
	SendMessageToClient(seq_index, user_index, rmsg);

	// 로그 기록 (GM TOOL을 이용한 경우에만 로그를 기록함)
	if (contentsFlag)
		return;
}

void DBProcess::ExpressDelete( boost::any& argv )
{
	expressdelete_t data = boost::any_cast<expressdelete_t>(argv);
	LONGLONG seq_index = boost::tuples::get<0>(data);
	int user_index = boost::tuples::get<1>(data);
	int char_index = boost::tuples::get<2>(data);
	expressIndex_t& expressIndex = boost::tuples::get<3>(data);
	bool send_flag = boost::tuples::get<4>(data);

	LOG_INFO("EXPRESS SYSTEM DELETE. char_index : %d, expressIndex : %d, send_flag : %d", char_index, expressIndex, (int)send_flag);

	std::string qry = boost::str(boost::format(
									 "DELETE FROM t_express_system WHERE a_index=%1% AND a_char_index=%2% LIMIT 1") % expressIndex % char_index);
	bool qbool = char_db_.excute(qry);

	if (send_flag)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		if (qbool)
		{
			LOG_INFO("EXPRESS ITEM DELETE SUCCESS. expressIndex : %d, charIndex : %d", expressIndex, char_index);
			ResponseClient::makeExpressDelete(rmsg, ResponseClient::ERR_NO_ERROR);
		}
		else
		{
			LOG_ERROR("EXPRESS ITEM DELETE FAIL. expressIndex : %d, charIndex : %d", expressIndex, char_index);
			ResponseClient::makeExpressDelete(rmsg, ResponseClient::ERR_DB_ERROR);
		}
		this->SendMessageToClient(seq_index, user_index, rmsg);
	}
}

void DBProcess::getItemByExpress( ExpressSystemItemInfo* info, MYSQL_RES *result )
{
	MYSQL_ROW row = mysql_fetch_row(result);

	info->index = atoi(row[char_db_.findfield("a_index")]);
	info->item_index = atoi(row[char_db_.findfield("a_item_index")]);
	info->plus = atoi(row[char_db_.findfield("a_plus")]);
	info->plus2 = atoi(row[char_db_.findfield("a_plus2")]);
	info->flag = atoi(row[char_db_.findfield("a_flag")]);
	memcpy(info->serial, row[char_db_.findfield("a_serial")], MAX_SERIAL_LENGTH);
	info->serial[MAX_SERIAL_LENGTH] = '\0';
	info->item_count = atoi(row[char_db_.findfield("a_item_count")]);
	info->option_count = atoi(row[char_db_.findfield("a_option_count")]);
	info->option_type[0] = atoi(row[char_db_.findfield("a_option_0_type")]);
	info->option_level[0] = atoi(row[char_db_.findfield("a_option_0_level")]);
	info->option_type[1] = atoi(row[char_db_.findfield("a_option_1_type")]);
	info->option_level[1] = atoi(row[char_db_.findfield("a_option_1_level")]);
	info->option_type[2] = atoi(row[char_db_.findfield("a_option_2_type")]);
	info->option_level[2] = atoi(row[char_db_.findfield("a_option_2_level")]);
	info->option_type[3] = atoi(row[char_db_.findfield("a_option_3_type")]);
	info->option_level[3] = atoi(row[char_db_.findfield("a_option_3_level")]);
	info->option_type[4] = atoi(row[char_db_.findfield("a_option_4_type")]);
	info->option_level[4] = atoi(row[char_db_.findfield("a_option_4_level")]);
	info->socket[0] = atoi(row[char_db_.findfield("a_socket0")]);
	info->socket[1] = atoi(row[char_db_.findfield("a_socket1")]);
	info->socket[2] = atoi(row[char_db_.findfield("a_socket2")]);
	info->socket[3] = atoi(row[char_db_.findfield("a_socket3")]);
	info->socket[4] = atoi(row[char_db_.findfield("a_socket4")]);
	info->socket[5] = atoi(row[char_db_.findfield("a_socket5")]);
	info->socket[6] = atoi(row[char_db_.findfield("a_socket6")]);
	info->item_origin[0] = atoi(row[char_db_.findfield("a_item_origin_var0")]);
	info->item_origin[1] = atoi(row[char_db_.findfield("a_item_origin_var1")]);
	info->item_origin[2] = atoi(row[char_db_.findfield("a_item_origin_var2")]);
	info->item_origin[3] = atoi(row[char_db_.findfield("a_item_origin_var3")]);
	info->item_origin[4] = atoi(row[char_db_.findfield("a_item_origin_var4")]);
	info->item_origin[5] = atoi(row[char_db_.findfield("a_item_origin_var5")]);
	info->nas = (LONGLONG)ATOLL(row[char_db_.findfield("a_nas")]);
	info->send_type = atoi(row[char_db_.findfield("a_send_type")]);
	memcpy(info->sender, row[char_db_.findfield("a_sender")], sizeof(info->sender));
	info->sender[EXPRESS_SENDER_NAME] = '\0';
	info->registe_date = atoi(row[char_db_.findfield("a_registe_date")]);
	info->expire_date = atoi(row[char_db_.findfield("a_expire_date")]);

#ifdef DURABILITY
	info->now_durability = atoi(row[char_db_.findfield("a_now_dur")]);
	info->max_durability = atoi(row[char_db_.findfield("a_max_dur")]);
#endif
	info->tradeagent_nas = ATOLL(row[char_db_.findfield("a_tradeagent_nas")]);
	info->tradeagent_itemIndex = atoi(row[char_db_.findfield("a_tradeagent_itemIndex")]);
	info->tradeagent_itemCount = atoi(row[char_db_.findfield("a_tradeagent_itemCount")]);
}

void DBProcess::QueryProcess( boost::any& argv )
{
	std::string str = boost::any_cast<std::string>(argv);
	char_db_.excute(str);
}

void DBProcess::MySQLPing()
{
	if (is_connect == false)
		return;

	char_db_.ping();
	data_db_.ping();

#ifdef USE_TENTER_BILLING
	catal_db_.ping();
#endif

#ifdef STASH_PASSWORD
	auth_db_.ping();
#endif
}

void DBProcess::sendAnyInfoAfterLogin( boost::any& argv )
{
	sendanyinfoafterlogin_t data = boost::any_cast<sendanyinfoafterlogin_t>(argv);
	LONGLONG seq_index = boost::tuples::get<0>(data);
	int userIndex = boost::tuples::get<1>(data);
	int charIndex = boost::tuples::get<2>(data);

	{
		// 구매대행의 판매 항목 또는 반품항목을 전달
		std::string qry = boost::str(boost::format(
										 "SELECT a_msg_type, a_item_index, a_item_count FROM t_tradeagent_after_sell_returned"
										 " WHERE a_char_index = %1%") % charIndex);
		BOOST_MYSQL_RES res = char_db_.select(qry);
		if (res && char_db_.getrowcount() > 0)
		{
			int count = char_db_.getrowcount();
			for (int i = 0; i < count; ++i)
			{
				MYSQL_ROW row = mysql_fetch_row(res.get());
				int a_msg_type = atoi(row[0]);
				int a_item_index = atoi(row[1]);
				int a_item_count = atoi(row[2]);

				CNetMsg::SP rmsg(new CNetMsg);
				if (a_msg_type == TRADEAGENT_DB_MSG_TYPE_SELL)
				{
					makeTradeAgentAfterSell(rmsg, charIndex, a_item_index, a_item_count);
				}
				else
				{
					makeTradeAgentAfterReturned(rmsg, charIndex, a_item_index, a_item_count);
				}

				this->SendMessageToClient(seq_index, userIndex, rmsg);
			}

			qry = boost::str(boost::format(
								 "DELETE FROM t_tradeagent_after_sell_returned WHERE a_char_index = %1%") % charIndex);
			char_db_.excute(qry);
		}
	}
}

void DBProcess::ExpressSearchNickName( boost::any& argv )
{
	expresssearchnickname_t data = boost::any_cast<expresssearchnickname_t>(argv);
	LONGLONG seq_index = boost::tuples::get<0>(data);
	int user_index = boost::tuples::get<1>(data);
	std::string nickName = boost::tuples::get<2>(data);

	char tnickName[MAX_CHAR_NAME_LENGTH * 2];
	mysql_real_escape_string(char_db_.getMYSQL(), tnickName, nickName.c_str(), nickName.length());

	std::string qry = boost::str(boost::format(
									 "SELECT a_index FROM t_characters WHERE a_nick='%1%' LIMIT 1") % tnickName);
	BOOST_MYSQL_RES res = char_db_.select(qry);
	if (res && char_db_.getrowcount() > 0) // found
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::makeExpressSearchNickname(rmsg, 0);
		this->SendMessageToClient(seq_index, user_index, rmsg);
	}
	else // not found
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::makeExpressSearchNickname(rmsg, 1);
		this->SendMessageToClient(seq_index, user_index, rmsg);
	}
}

void DBProcess::ExpressSendItemToNickName( boost::any& argv )
{
	expresssenditemtonickname_t data = boost::any_cast<expresssenditemtonickname_t>(argv);
	LONGLONG seq_index = boost::tuples::get<0>(data);
	int user_index = boost::tuples::get<1>(data);
	int charIndex = boost::tuples::get<2>(data);
	ExpressSystemItemInfo* itemInfo = boost::tuples::get<3>(data);
	std::string receiver = boost::tuples::get<4>(data);
	int sender_type = boost::tuples::get<5>(data);

	// 받는 유저 검색 (기본적으로 클라이언트에서 검색을 진행한 후 보내므로 무조건 찾아야함)
	char tnickName[MAX_CHAR_NAME_LENGTH * 2];
	mysql_real_escape_string(char_db_.getMYSQL(), tnickName, receiver.c_str(), receiver.length());

	std::string qry = boost::str(boost::format(
									 "SELECT a_index FROM t_characters WHERE a_nick='%1%' LIMIT 1") % tnickName);
	BOOST_MYSQL_RES res = char_db_.select(qry);
	if (res == NULL || char_db_.getrowcount() == 0)
	{
		// 만약 쿼리 에러이거나, 유저를 찾지 못하면 자신의 Express로 다시 넣어줌
		if (sender_type == EXPRESS_SENDER_TYPE_MARBLE)
		{
			LOG_ERROR("MARBLE_SEND : not found user : charIndex : %d, itemIndex : %d, plsu : %d, flag : %d, itemCount : %d, receiver : %s",
					  charIndex, itemInfo->item_index, itemInfo->plus, itemInfo->flag, itemInfo->item_count, receiver.c_str());
		}
		else
		{
			LOG_ERROR("Not found user(express) : charIndex : %d, itemIndex : %d, plsu : %d, flag : %d, itemCount : %d, receiver : %s",
					  charIndex, itemInfo->item_index, itemInfo->plus, itemInfo->flag, itemInfo->item_count, receiver.c_str());
		}

		expressinputitem_t ldata(seq_index, charIndex, itemInfo);
		boost::any localpushdata(ldata);
		this->ExpressInputItem(localpushdata);

		{
			CNetMsg::SP rmsg(new CNetMsg);
			ResponseClient::makeExpressSendToNickName(rmsg, ResponseClient::ERR_NOT_FOUND_NICKNAME_SYS_ERR);
			this->SendMessageToClient(seq_index, user_index, rmsg);
		}

		return;
	}

	{
		MYSQL_ROW row = mysql_fetch_row(res.get());
		int receive_charIndex = atoi(row[0]);
		expressinputitem_t ldata(seq_index, receive_charIndex, itemInfo);
		boost::any localpushdata(ldata);
		this->ExpressInputItem(localpushdata);
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::makeExpressSendToNickName(rmsg, ResponseClient::ERR_NO_ERROR);
		this->SendMessageToClient(seq_index, user_index, rmsg);
	}
}

void DBProcess::DelCharForGM( boost::any& argv )
{
	delcharforgm_t data = boost::any_cast<delcharforgm_t>(argv);
	LONGLONG seq_index = boost::tuples::get<0>(data);
	int user_index = boost::tuples::get<1>(data);
	std::string del_charname = boost::tuples::get<2>(data);

	int char_index = 0;

	{
		std::string qry = boost::str(boost::format(
										 "SELECT a_index FROM t_characters WHERE a_enable = 1 AND a_user_index=%d AND a_nick='%s' LIMIT 1") % user_index % del_charname);
		BOOST_MYSQL_RES res = char_db_.select(qry);
		if (res == NULL || char_db_.getrowcount() == 0)
		{
			return;
		}

		MYSQL_ROW row = mysql_fetch_row(res.get());
		char_index = atoi(row[0]);
	}

	{
		// 길드에 가입한 캐릭터인지 검사를 한다.
		std::string qry = boost::str(boost::format("SELECT a_char_index FROM t_guildmember WHERE a_char_index = %d LIMIT 1") % char_index);
		BOOST_MYSQL_RES res = char_db_.select(qry);
		if (res == NULL || char_db_.getrowcount() > 0)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			SayMsg(rmsg, MSG_CHAT_SAY, 0, "", "", "Can't delete guild member");
			SendMessageToClient(seq_index, user_index, rmsg);

			return;
		}
	}

	{
		// 모든 조건을 만족했으므로 캐릭터를 지운다.
		std::string qry = boost::str(boost::format(
										 "UPDATE t_characters SET a_enable = 0 WHERE a_index = %d LIMIT 1") % char_index);
		if (char_db_.excute(qry) == false)
			return;

		std::string tstr = boost::str(boost::format(
										  "delete ok. char name is %s") % del_charname);
		CNetMsg::SP rmsg(new CNetMsg);
		SayMsg(rmsg, MSG_CHAT_SAY, 0, "", "", tstr.c_str());
		SendMessageToClient(seq_index, user_index, rmsg);
	}
}

#ifdef STASH_PASSWORD
void DBProcess::QueryProcessForAuthDB( boost::any& argv )
{
	std::string str = boost::any_cast<std::string>(argv);
	auth_db_.excute(str);
}
#endif

void DBProcess::QueryProcessForDataDB(boost::any& argv)
{
	std::string str = boost::any_cast<std::string>(argv);
	data_db_.excute(str);
}

void DBProcess::getCharIndexByGPS( boost::any& argv )
{
	getcharindexbyGPS_t data = boost::any_cast<getcharindexbyGPS_t>(argv);
	LONGLONG seq_index = boost::tuples::get<0>(data);
	int user_index = boost::tuples::get<1>(data);
	std::string search_charname = boost::tuples::get<2>(data);
	int itemVIndex = boost::tuples::get<3>(data);

	EventProcessForDB::getCharIndexByGPS retval;
	retval.seq_index = seq_index;
	retval.user_index = user_index;
	retval.char_index = 0;
	retval.itemVIndex = itemVIndex;
	retval.char_name = search_charname;

	std::string query = boost::str(boost::format(
		"SELECT a_index FROM t_characters WHERE a_nick='%s' LIMIT 1") % search_charname);
	BOOST_MYSQL_RES res = char_db_.select(query);
	if (res == NULL || char_db_.getrowcount() == 0)
	{
		// 찾지 못한 경우
		retval.char_index = 0;
	}
	else
	{
		MYSQL_ROW row = mysql_fetch_row(res.get());
		retval.char_index = atoi(row[0]);
	}

	EventProcessForDB::instance()->pushGetCharIndexByGPS(retval);
}
