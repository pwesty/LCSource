#ifndef __EVENT_PROCESS_WITH_THREAD_H__
#define __EVENT_PROCESS_WITH_THREAD_H__

#include <boost/any.hpp>
#include <boost/tuple/tuple.hpp>
#include "../ShareLib/rnsocketiohandler.h"
#include "../ShareLib/rnsocketioservice.h"
#include "../ShareLib/event_session.h"
#include "../ShareLib/packetType/ptype_express_system.h"

enum
{
	EVENT_TYPE_NOT_USED,
	EVENT_TYPE_CHAR_LIST,
	EVENT_TYPE_CREATE_CHARACTER,
	EVENT_TYPE_DELETE_CHARACTER,
	EVENT_TYPE_CANCEL_DELETE_CHARACTER,
	EVENT_TYPE_SELECT_CHARACTER,
	EVENT_TYPE_SELECT_CHARACTER_OTHER_ZONE,
	EVENT_TYPE_EXPRESS_TAKE,

	EVENT_TYPE_SEND_MESSAGE_TO_CLIENT,			// db thread에서 전달된 패킷을 클라이언트에게 전달
	EVENT_TYPE_GET_CHAR_INDEX_BY_GPS,
};

// DBManger thread에서 일어난 결과를 리턴받아 최종 결과를 처리하는 클래스
class EventProcessForDB : public rnSocketIOHandler
{
public:
	struct charlist
	{
		explicit charlist(int index) : m_index(index)
			, m_tExtendCharacterSlotTime(0)
			, m_bNotCreateNightShadow(false)
			, m_bCreate90Lv(false)
			, m_bIsNSCreateCardUse(false)
		{ }

		int m_index;
		LONGLONG m_seq_index;
		int char_index;
		time_t m_tExtendCharacterSlotTime;
		bool m_bNotCreateNightShadow;
		bool m_bCreate90Lv;
		bool m_bIsNSCreateCardUse;
	};

	struct returnCreateChar
	{
		int m_index;
		LONGLONG m_seq_index;
		int char_index;
		std::string name;
		int job;
	};

	struct selectChar
	{
		int		m_index;
		LONGLONG	m_seq_index;
		CPC*	pChar;
		int		m_notice[MAX_NOTICE];
	};

	struct deleteChar
	{
		int				m_index;
		LONGLONG		m_seq_index;
		std::string		name;
		int				job;
	};

	struct cancleDeleteChar 
	{
		int				m_index;
		LONGLONG		m_seq_index;
	};

	struct expressTake
	{
		int		m_index;
		ExpressSystemItemInfo itemInfo;
	};

	struct sendMessageToClient
	{
		LONGLONG		seq_index;
		int				user_index;
		CNetMsg::SP		msg;
	};

	struct moveOtherZone 
	{
		int			m_index;
		LONGLONG	m_seq_index;
	};

	struct getCharIndexByGPS 
	{
		LONGLONG	seq_index;
		int			user_index;
		int			char_index;
		int			itemVIndex;
		std::string char_name;
	};

public:
	EventProcessForDB(void);
	~EventProcessForDB(void);

	static EventProcessForDB* instance();

	virtual void operate(rnSocketIOService* service);

	void pushCharList(charlist& list);
	void pushCreateCharacter(returnCreateChar& data);
	void pushDeleteCharacter(deleteChar& data);
	void pushCancelDeleteCharacter(cancleDeleteChar& data);
	void pushSelectCharacter(selectChar& data);
	void pushSelectCharacterOtherZone(moveOtherZone& data);
	void pushExpressTake(expressTake& data);
	void pushSendMessageToClient(sendMessageToClient& data);
	void pushGetCharIndexByGPS(getCharIndexByGPS& data);

private:
	void process_char_list(boost::any& any);
	void process_create_character(boost::any& any);
	void process_delete_character(boost::any& any);
	void process_cancel_delete_character(boost::any& any);
	void process_select_character(boost::any& any);
	void process_select_character_other_zone(boost::any& any);
	void process_express_take(boost::any& any);
	void process_send_message_to_client(boost::any& any);

private:
	session_handle	handle_;
	EventSession*	session_;
};

#endif
