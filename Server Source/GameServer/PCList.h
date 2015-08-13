#ifndef __PC_LIST_H__
#define __PC_LIST_H__

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/member.hpp>

#include "Character.h"

// 현재 게임 중(zone에 소속해 있는 )인 User와 모든 GameServer에 연결된 User리스트
class PCManager
{
	// boost::multi_index_container Definition
public:
	struct PlayerInfo
	{
		int charIndex;
		int userIndex;
		std::string charName;
		std::string charNickName;
		CPC* pPlayer;
	};

	struct Key_User_Index		{};
	struct Key_Char_Index		{};
	struct Key_Char_Name		{};
	struct Key_Char_Nick_Name	{};

	typedef boost::multi_index_container<
	PlayerInfo,
	boost::multi_index::indexed_by
	<
	boost::multi_index::hashed_unique
	<
	boost::multi_index::tag<Key_User_Index>, BOOST_MULTI_INDEX_MEMBER(PlayerInfo, int, userIndex)
	>,
	boost::multi_index::hashed_unique
	<
	boost::multi_index::tag<Key_Char_Index>, BOOST_MULTI_INDEX_MEMBER(PlayerInfo, int, charIndex)
	>,
	boost::multi_index::hashed_unique
	<
	boost::multi_index::tag<Key_Char_Name>, BOOST_MULTI_INDEX_MEMBER(PlayerInfo, std::string, charName)
	>,
	boost::multi_index::hashed_unique
	<
	boost::multi_index::tag<Key_Char_Nick_Name>, BOOST_MULTI_INDEX_MEMBER(PlayerInfo, std::string, charNickName)
	>
	>
	> map_t;

public:
	PCManager() {}
	~PCManager() {}

	static PCManager* instance();

public:
	void	addPlayer(CPC* pPlayer);

	CPC*	getPlayerByUserIndex(int userIndex);
	CPC*	getPlayerByCharIndex(int charIndex);
	CPC*	getPlayerByName(const char* name, bool isNicName);
	int		getPlayerCount()
	{
		return playerMap_.size();
	}

	map_t&  getPlayerMap()
	{
		return playerMap_;
	}

	bool	isPlaying(CPC *ch);

	void	deletePlayerByPC(CPC* pc);
	void	sendToAll(CNetMsg::SP& msg);

	// GuildWar 다른 매니저로 뺄것
	void	sendGuildWarPointMsg(CWarCastle* castle, int zoneindex);
	// WarJoinUser 다른 매니저로 뺄것
	void	sendToWarJoinUser(CNetMsg::SP& msg, int zoneindex, bool bInZone);
	// Teacher 다른 매니저로 뺄것 -> 사제 매니저 제작 후
	void	sendToAllTeacher(CNetMsg::SP& msg);

	// zone 매니저 생성 후 이동 시킬 것
	void	sendToZone(CNetMsg::SP& msg, int zoneIndex);

	void	changeNickName(const char* oldname, const char* newname);

#ifdef PREMIUM_CHAR
	void	resetPremiumCharJumpCount();
#endif

private:
	CPC*	getPlayerByCharName(const char* charName);
	CPC*	getPlayerByCharNickName(const char* charName);

	void	deletePlayerByUserIndex(int userIndex);
	void	deletePlayerByCharIndex(int charIndex);
	void	deletePlayerByCharName(const char* charName);
	void	deletePlayerByCharNickName(const char* charName);

private:
	map_t	playerMap_;
};
#endif
