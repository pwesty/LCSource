#ifndef __LOG_IN_OUT_MANAGER_H__
#define __LOG_IN_OUT_MANAGER_H__

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/composite_key.hpp>

#include "rnsocketioservice.h"
#include "./packetType/ptype_server_to_server.h"

class LogInOutManager
{
public:
	struct userInfo
	{
		int userIndex;
		int charIndex;
		int serverNo;
		int zoneNo;
		std::string userId;
		std::string charName;
		rnSocketIOService* service;
	};

	struct Key_user_index {};
	struct Key_char_index {};
	struct Key_user_id {};
	struct Key_char_name {};
	struct Key_socket_io {};

	typedef boost::multi_index_container <
	userInfo,
	boost::multi_index::indexed_by
	<
	boost::multi_index::hashed_unique
	<
	boost::multi_index::tag<Key_user_index>, BOOST_MULTI_INDEX_MEMBER(userInfo, int, userIndex)
	>,
	boost::multi_index::hashed_unique
	<
	boost::multi_index::tag<Key_char_index>, BOOST_MULTI_INDEX_MEMBER(userInfo, int, charIndex)
	>,
	boost::multi_index::hashed_unique
	<
	boost::multi_index::tag<Key_user_id>, BOOST_MULTI_INDEX_MEMBER(userInfo, std::string, userId)
	>,
	boost::multi_index::hashed_unique
	<
	boost::multi_index::tag<Key_char_name>, BOOST_MULTI_INDEX_MEMBER(userInfo, std::string, charName)
	>,
	boost::multi_index::hashed_non_unique
	<
	boost::multi_index::tag<Key_socket_io>, BOOST_MULTI_INDEX_MEMBER(userInfo, rnSocketIOService*, service)
	>
	>
	> map_t;

public:
	LogInOutManager(void) {}
	~LogInOutManager(void) {}

	static LogInOutManager* Instance();

	int getCount()
	{
		return map_.size();
	}

public:
	bool Add(rnSocketIOService* service, CNetMsg::SP& msg);
	void Delete(CNetMsg::SP& msg);
	void DeleteByUserIndex(int userIndex);
	void DeleteByCharIndex(int charIndex);
	void DeleteByUserId(std::string userId);
	void DeleteByCharName(std::string charName);
	void DeleteBySocketIO(rnSocketIOService* service);

public:
	bool isExistByUserIndex(int userIndex);
	bool isExistByCharIndex(int charIndex);
	bool isExistByUserId(std::string userId);
	bool isExistByCharName(std::string charName);

	rnSocketIOService* getSocketIOByUserIndex(int userIndex);
	rnSocketIOService* getSocketIOByCharIndex(int charIndex);
	rnSocketIOService* getSocketIOByUserId(std::string userId);
	rnSocketIOService* getSocketIOByCharName(std::string charName);

	const userInfo* getUserInfoByUserIndex(int userIndex);
	const userInfo* getUserInfoByCharIndex(int charIndex);
	const userInfo* getUserInfoByUserId(std::string userId);
	const userInfo* getUserInfoByCharname(std::string charName);

	void changeNickName(const char* oldnickname, const char* newnickname);

private:
	map_t		map_;
};

#endif
