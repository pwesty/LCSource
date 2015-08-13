#ifndef __DB_MANAGER_H__
#define __DB_MANAGER_H__

#include <vector>
#include <string>
#include <boost/any.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/thread.hpp>

#include "../ShareLib/queue_ts.h"
#include "../ShareLib/mysqldb.h"
#include "../ShareLib/MemoryPoolBase.h"
#include "../ShareLib/packetType/ptype_express_system.h"

#include "Character.h"

enum
{
	DB_PROC_EXIT = -1,

	DB_PROC_CHARACTER_LIST = 0,
	DB_PROC_CREATE_CHAR,
	DB_PROC_DELETE_CHAR,
	DB_PROC_CANCEL_DELETE_CHAR,
	DB_PROC_SELECT_CHAR,
	DB_PROC_SAVE_CHAR,

	DB_PROC_EXPRESS_EXIST,
	DB_PROC_EXPRESS_LIST,
	DB_PROC_EXPRESS_TAKE,
	DB_PROC_EXPRESS_INPUT_ITEM,		// 창고NPC에게 아이템을 입력
	DB_PROC_EXPRESS_INPUT_NAS,		// 창고NPC에게 나스(돈)을 입력
	DB_PROC_EXPRESS_DELETE,			// 아이템(또는 돈)을 유저가 수령했을 경우 삭제함

	DB_PROC_QUERY,

	DB_PROC_MYSQL_PING,
	DB_PROC_SEND_ANY_INFO_AFTER_LOGIN,		// 로그인 후 자동으로 클라이언트에게 보낼 정보를 처리함

	DB_PROC_SEARCH_NICKNAME_FOR_EXPRESS,	// 신비한 석상의 구슬
	DB_PROC_SEND_ITEM_TO_NICKNAME,				// 신비한 석상의 구슬

	DB_PROC_DEL_CHARACTER_FOR_GM,			// gm 명령어로 캐릭터를 지울때 사용

	DB_PROC_QUERY_FOR_AUTH_DB,
	DB_PROC_QUERY_FOR_DATA_DB,

	DB_PROC_GET_CHAR_INDEX_BY_GPS,			// 이름을 통해서 char index를 얻음
};

enum
{
	CHAR_SAVE_TYPE_TCHAR,
	CHAR_SAVE_TYPE_TCHAR_GUILD_POINT,
	CHAR_SAVE_TYPE_TCHAR_QUEST,
	CHAR_SAVE_TYPE_TCHAR_AFFINITY,
	CHAR_SAVE_TYPE_TCHAR_ASSIST,
	CHAR_SAVE_TYPE_TCHAR_ASSIST_ABS_TIME,
	CHAR_SAVE_TYPE_TCHAR_INVEN,
	CHAR_SAVE_TYPE_TCHAR_GAMIGO_HACK_DELETE,
	CHAR_SAVE_TYPE_TCHAR_STASH,
	CHAR_SAVE_TYPE_TCHAR_QUICK_SLOT,
	CHAR_SAVE_TYPE_TCHAR_MEMPOS,
	CHAR_SAVE_TYPE_TCHAR_CASH_ITEM_DATE,
	CHAR_SAVE_TYPE_TCHAR_PD_DATA,
	CHAR_SAVE_TYPE_TCHAR_PET_PRE,
	CHAR_SAVE_TYPE_TCHAR_PET,
	CHAR_SAVE_TYPE_TCHAR_PET_NAME,
	CHAR_SAVE_TYPE_TCHAR_PET_TEMP_DELETE,
	CHAR_SAVE_TYPE_TCHAR_APET_PRE,
	CHAR_SAVE_TYPE_TCHAR_APET,
	CHAR_SAVE_TYPE_TCHAR_APET_OWNER,
	CHAR_SAVE_TYPE_TCHAR_APET_INVEN,
	CHAR_SAVE_TYPE_TCHAR_APET_AI,
	CHAR_SAVE_TYPE_TCHAR_APET_TEMP_DELETE,
	CHAR_SAVE_TYPE_TCHAR_CHARACTER_FACTORY,
	CHAR_SAVE_TYPE_TCHAR_MESSENGER_OPT,
	CHAR_SAVE_TYPE_TCHAR_FRIEND,
	CHAR_SAVE_TYPE_TCHAR_BLOCK_FRIEND,
	CHAR_SAVE_TYPE_TCHAR_WAR_GROUND,
	CHAR_SAVE_TYPE_TCHAR_EVENT_GOMDORI,

	CHAR_SAVE_TYPE_TIMER_ITEM,
	CHAR_SAVE_TYPE_TCHAR_PET_STASH,
	CHAR_SAVE_TYPE_TCHAR_PET_STASH_INFO,
	CHAR_SAVE_TYPE_TCHAR_SKILL_COOLTIME,
	CHAR_SAVE_TYPE_TCHAR_WEAR_ITEM,
	CHAR_SAVE_TYPE_RVR_INFO,
	CHAR_SAVE_TYPE_GPS_INFO,
	CHAR_SAVE_TYPE_ITEM_COLLECTION_INFO,

#ifdef PREMIUM_CHAR
	CHAR_SAVE_TYPE_PREMIUM_CHAR_INFO,
#endif

	CHAR_SAVE_TYPE_TCHAR_MAX,
};

enum
{
	EXPRESS_SENDER_TYPE_MARBLE,				// 신비한 석상의 구슬
	EXPRESS_SENDER_TYPE_WEAR_INVENTORY,		// 장착 인벤토리
};

typedef std::vector<std::vector<std::string> > save_char_queryt_t;

struct CreateCharacterInfo : public MemoryPoolBaseWithMutex<CreateCharacterInfo>
{
	int				m_index;
	time_t			m_tExtendCharacterSlotTime;
#ifdef HANARO_EVENT
	std::string		m_hanaroCode;
#endif
	std::string		m_name;
	unsigned char	m_job;
	unsigned char	m_hairstyle;
	unsigned char	m_facestyle;

	int				m_str;
	int				m_dex;
	int				m_int;
	int				m_con;
	int				m_dbHP;
	int				m_dbMP;

	int				m_level;
	unsigned char	m_job2;
	int				m_statpt_str;
	int				m_statpt_dex;
	int				m_statpt_con;
	int				m_statpt_int;
	int				m_statpt_remain;

	int				m_etcEvent;
};

struct SaveCharacter
{
};
//////////////////////////////////////////////////////////////////////////
class DBProcess;

class DBManager
{
public:
	DBManager();
	~DBManager();

	static DBManager* instance();

	bool Init(int count = 2 /*thread count*/);
	void MySQLPing();
	void JoinAll();

	void PushCharacterList(CDescriptor* desc);
	void PushCreateChar(CDescriptor* desc, CreateCharacterInfo* cinfo);
	void PushDeleteCharacter(CDescriptor* desc, int char_index, int guildoutdate);
	void PushCancelDeleteCharacter(CDescriptor* desc, int char_index);
	void PushSelectCharacter(CDescriptor* desc, int char_index, void* guild);
	void PushSaveCharacter(CDescriptor* desc, save_char_queryt_t* save_data, bool disconnect);

	void PushExpressExist(CDescriptor* desc, int send_type);
	void PushExpressList(CDescriptor* desc, int pageIndex);
	void PushExpressTake(CDescriptor* desc, expressIndex_t expressIndex);
	void PushExpressInputItem(CDescriptor* desc, ExpressSystemItemInfo* itemInfo, bool contentsFlag);
	void PushExpressInputNas(CDescriptor* desc, unsigned int nas, int send_type, std::string sender, bool contentsFlag);
	void PushExpressInputItemNotConnectUser(int charIndex, ExpressSystemItemInfo* itemInfo, bool contentsFlag);
	void PushExpressInputNasNotConnectUser(int charIndex, unsigned int nas, int send_type, std::string sender, bool contentsFlag);
	void PushExpressDelete(CDescriptor* desc, expressIndex_t expressIndex, bool send_flag = false);
	void PushExpressSearchNickName(CDescriptor* desc, const char* nickName);
	void PushExpressSendItemToNickName(CDescriptor* desc, ExpressSystemItemInfo* itemInfo, std::string receiver, int sender_type);

	void pushQuery(int userIndex, std::string& query);

	void pushSendAnyInfoAfterLogin(CDescriptor* desc);

	void SaveCharacterInfo(CDescriptor* d, bool disconnect);

	void pushDelCharForGM(CDescriptor* desc, std::string& del_charname);

	void pushQueryForAuthDB(int userIndex, std::string& query);
	void pushQueryForDataDB(int userIndex, std::string& query);

	void pushGetCharIndexByGPS(CDescriptor* desc, std::string& name, int itemVindex);

private:
	void _SaveCharaterQuery(std::vector<std::string> & vec, CPC* pChar, int table_no);
	void _SaveGuildPointQuery(std::vector<std::string> & vec, CPC* pChar);
	void _SaveQuestQuery(std::vector<std::string> & vec, CPC* pChar, int table_no);
	void _SaveAffinityQuery(std::vector<std::string> & vec, CPC* pChar);
	void _SaveAssistQuery(std::vector<std::string> & vec, CPC* pChar);
	void _SaveAssistABSTimeQuery(std::vector<std::string> & vec, CPC* pChar);
	void _SaveCharInvenQuery(std::vector<std::string> & vec, CPC* pChar,int table_no);
	void _SaveCharcterStashQuery(std::vector<std::string> & vec, CPC* pChar);
	void _SaveCharacterQuickSlotQuery(std::vector<std::string> & vec, CPC* pChar, int table_no);
	void _SaveCharacterMemposQuery(std::vector<std::string> & vec, CPC* pChar);
	void _SaveCharacterCashItemDateQuery(std::vector<std::string> & vec, CPC* pChar, int descriptorUserIndex);
	void _SaveCharacterPersonalDungeonQuery(std::vector<std::string> & vec, CPC* pChar, int table_no);

	void _SaveCharacterPetPreQuery(std::vector<std::string> & vec, CPC* pChar);
	void _SaveCharacterPetQuery(std::vector<std::string> & vec, CPC* pChar, CPet* pet);
	void _SaveCharacterPetNameQuery(std::vector<std::string> & vec, CPC* pChar, CPet* pet);
	void _SaveCharacterPetTempDeleteQuery(std::vector<std::string> & vec, CPC* pChar);

	void _SaveCharacterAttackPetPreQuery(std::vector<std::string> & vec, CPC* pChar);
	void _SaveCharacterAttackPetQuery(std::vector<std::string> & vec, CPC* pChar, CAPet* apet);
	void _SaveCharacterAttackPetInvenQuery(std::vector<std::string> & vec, CPC* pChar, CAPet* apet);
	void _SaveCharacterAttackPetAIQuery(std::vector<std::string> & vec, CPC* pChar, CAPet* apet);
	void _SaveCharacterAttackPetTempDeleteQuery(std::vector<std::string> & vec, CPC* pChar);

	void _SaveCharacterFactoryQuery(std::vector<std::string> & vec, CPC* pChar);
	void _SaveCharacterMessengerOPTQuery(std::vector<std::string> & vec, CPC* pChar);
	void _SaveCharacterMessengerFriendQuery(std::vector<std::string> & vec, CPC* pChar, int table_no);
	void _SaveCharacterMessengerBlockFriendQuery(std::vector<std::string> & vec, CPC* pChar);
	void _SaveCharacterRoyalRumbleQuery(std::vector<std::string> & vec, CPC* pChar);
	void _SaveCharacterEventGomdoriQuery(std::vector<std::string> & vec, CPC* pChar);
	void _SaveTimerItemQuery(std::vector<std::string> & vec, CPC* pChar);
	void _SaveCharacterPetStashDeleteQuery(std::vector<std::string> & vec, CPC* pChar);
	void _SaveCharacterPetStashQuery(std::vector<std::string> & vec, CPC* pChar);
	void _SaveCharacterSkillCoolTimeQuery(std::vector<std::string> & vec, CPC* pChar);
	void _SaveCharacterRVRInfo(std::vector<std::string> & vec, CPC* pChar);
	void _SaveCharacterGPSInfo(std::vector<std::string> & vec, CPC* pChar);
	void _SaveCharacterItemCollectionInfo(std::vector<std::string> & vec, CPC* pChar);

#ifdef PREMIUM_CHAR
	void _SaveCharacterPremiumCharInfo(std::vector<std::string> & vec, CPC* pChar);
#endif

private:
	int						thread_count_;
	boost::thread_group		thread_group_;
	DBProcess*				db_process_;

	bool					stop_flag_;
};

//////////////////////////////////////////////////////////////////////////

class DBProcess
{
public:
	friend class DBManager;

	typedef std::pair<int, boost::any> type_t;

	typedef boost::tuple<LONGLONG, int/*user_index*/, int, unsigned int> charlist_t;
	typedef boost::tuple<LONGLONG, int/*user_index*/, CreateCharacterInfo*> createchar_t;
	typedef boost::tuple<LONGLONG, int/*m_index*/, int/*char_index*/, time_t, int> delchar_t;
	typedef boost::tuple<LONGLONG, int, int> canceldelchar_t;
	typedef boost::tuple<LONGLONG, int/*m_index*/, int/*char_index*/, int/*m_playmode*/, std::string/*m_proSite*/, std::string/*user_id*/, void*/*guild*/> selectchar_t;
	typedef boost::tuple<LONGLONG, int/*m_index*/, int/*charIndex*/, std::string/*id*/, std::string/*charName*/, bool/*disconnect flag*/, save_char_queryt_t*> savechar_t;

	typedef boost::tuple<LONGLONG, int/*user_index*/, int/*char_index*/, int/*send_type*/> expressexist_t;
	typedef boost::tuple<LONGLONG, int/*user_index*/, int/*char_index*/, int/*page index*/> expresslist_t;
	typedef boost::tuple<LONGLONG, int/*m_index*/, int/*char_index*/, expressIndex_t> expresstake_t;
	typedef boost::tuple<LONGLONG, int/*char_index*/, ExpressSystemItemInfo*, bool> expressinputitem_t;
	typedef boost::tuple<LONGLONG, int/*user_index*/, int/*char_index*/, unsigned int/*nas*/, int/*send_type*/, std::string/*sender*/, bool> expressinputnas_t;
	typedef boost::tuple<LONGLONG, int/*user_index*/, int/*char_index*/, expressIndex_t, bool/*send_flag*/> expressdelete_t;
	typedef boost::tuple<LONGLONG, int/*user_index*/, int/*char_index*/> sendanyinfoafterlogin_t;
	typedef boost::tuple<LONGLONG, int/*user_index*/, std::string/*nickName*/> expresssearchnickname_t;
	typedef boost::tuple<LONGLONG, int/*user_index*/, int/*charIndex*/,ExpressSystemItemInfo*, std::string/*receiver*/, int/*sender_type*/> expresssenditemtonickname_t;
	typedef boost::tuple<LONGLONG, int/*user_index*/, std::string/*del_char_name*/> delcharforgm_t;
	typedef boost::tuple<LONGLONG, int/*user_index*/, std::string/*search_char_name*/, int/*itemVIndex*/> getcharindexbyGPS_t;

	typedef std::string query_t;

public:
	DBProcess();

	~DBProcess()
	{
		char_db_.close();
		data_db_.close();
#ifdef USE_TENTER_BILLING
		catal_db_.close();
#endif

#ifdef STASH_PASSWORD
		auth_db_.close();
#endif
	}

	bool Connect();
	void Run();

	//////////////////////////////////////////////////////////////////////////
	void CharacterList(boost::any& argv);
	void CreateChar(boost::any& argv);
	void DeleteChar(boost::any& argv);
	void CancelDeleteChar(boost::any& argv);
	void SelectChar(boost::any& argv); // DBProcess_SelectChar.cpp 에 위치
	void SaveChar(boost::any& argv); // DBProcess_SaveChar.cpp 에 위치

	void ExpressExist(boost::any& argv);
	void ExpressList(boost::any& argv);
	void ExpressTake(boost::any& argv);
	void ExpressInputItem(boost::any& argv);
	void ExpressInputNas(boost::any& argv);
	void ExpressDelete(boost::any& argv);
	void ExpressSearchNickName(boost::any& argv);
	void ExpressSendItemToNickName(boost::any& argv);

	void QueryProcess(boost::any& argv);
	void sendAnyInfoAfterLogin(boost::any& argv);

	void DelCharForGM(boost::any& argv);

	void QueryProcessForAuthDB(boost::any& argv);
	void QueryProcessForDataDB(boost::any& argv);

	void getCharIndexByGPS(boost::any& argv);

private:
	void SendMessageToClient(LONGLONG& seq_index, int userIndex, CNetMsg::SP& msg);
	void excute_query(std::vector<std::string>& vec); // DBProcess_SaveChar.cpp 에 위치
	void excute_query_commi_and_rollback(std::vector<std::string>& vec); // DBProcess_SaveChar.cpp 에 위치
	void PromotionGive(int m_index, CPC* pChar, int partner_id, bool lucky);
	void getItemByExpress(ExpressSystemItemInfo* info, MYSQL_RES *result);
	void select_rvr_info_query(CPC* pChar);		// 저장되어진 결사대 정보 가져오기

	void FixItemUsedTime(CItem* pItem);

	void MySQLPing();

private:
	queue_ts<type_t>	queue_;

	bool		is_connect;
	mysqldb		char_db_;
	mysqldb		data_db_;
	mysqldb&	castle_db_;
	mysqldb&	charingame_db_;

#ifdef USE_TENTER_BILLING
	mysqldb		catal_db_;
#endif

	mysqldb&	trigger_db_;

#ifdef STASH_PASSWORD
	mysqldb		auth_db_;
#endif

	std::vector<std::string> a_item_idx_str;
	std::vector<std::string> a_plus_str;
	std::vector<std::string> a_wear_pos_str;
	std::vector<std::string> a_flag_str;
	std::vector<std::string> a_serial_str;
	std::vector<std::string> a_count_str;
	std::vector<std::string> a_used_str;
	std::vector<std::string> a_used2_str;

	std::vector<std::string> a_item_option_str[ITEMS_PER_ROW];
	std::vector<std::string> a_item_origin_str[ITEMS_PER_ROW];

	std::vector<std::string> a_socket_str;

#ifdef DURABILITY
	std::vector<std::string> a_now_durability_str;
	std::vector<std::string> a_max_durability_str;
#endif

	std::vector<std::string> a_mempos_comment;

};

#endif
