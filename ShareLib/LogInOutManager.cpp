#include <boost/lambda/lambda.hpp>
#include "Config.h"
#include "logsystem.h"
#include "LogInOutManager.h"

LogInOutManager* LogInOutManager::Instance()
{
	static LogInOutManager __instance;
	return &__instance;
}

bool LogInOutManager::Add( rnSocketIOService* service, CNetMsg::SP& msg)
{
	ServerToServerPacket::LoginUserInfo* info = reinterpret_cast<ServerToServerPacket::LoginUserInfo*>(msg->m_buf);
	if (this->isExistByUserIndex(info->userIndex))
	{
		LOG_ERROR("duplication data : userIndex[%d] userId[%s] charIndex[%d] charName[%s]",
				  info->userIndex, info->userId, info->charIndex, info->charName);

		return false;
	}

	userInfo uinfo;
	uinfo.userIndex = info->userIndex;
	uinfo.charIndex = info->charIndex;
	uinfo.serverNo = info->serverNo;
	uinfo.zoneNo = info->zoneNo;
	uinfo.userId = info->userId;
	uinfo.charName = info->charName;
	uinfo.service = service;

	map_.insert(uinfo);

	LOG_INFO("add data : userIndex[%d] userId[%s] charIndex[%d] charName[%s]",
			 info->userIndex, info->userId, info->charIndex, info->charName);

	return true;
}

//////////////////////////////////////////////////////////////////////////
void LogInOutManager::Delete(CNetMsg::SP& msg)
{
	ServerToServerPacket::LogoutUserInfo* packet = reinterpret_cast<ServerToServerPacket::LogoutUserInfo*>(msg->m_buf);

	if (this->isExistByUserIndex(packet->userIndex) == false)
		return;

	this->DeleteByUserIndex(packet->userIndex);

	LOG_INFO("del data : userIndex[%d]", packet->userIndex);
}

void LogInOutManager::DeleteByUserIndex( int userIndex )
{
	map_t::index<Key_user_index>::type& p = map_.get<Key_user_index>();
	p.erase(userIndex);
}

void LogInOutManager::DeleteByCharIndex( int charIndex )
{
	map_t::index<Key_char_index>::type& p = map_.get<Key_char_index>();
	p.erase(charIndex);
}

void LogInOutManager::DeleteByUserId(std::string userId)
{
	map_t::index<Key_user_id>::type& p = map_.get<Key_user_id>();
	p.erase(userId);
}

void LogInOutManager::DeleteByCharName(std::string charName)
{
	map_t::index<Key_char_name>::type& p = map_.get<Key_char_name>();
	p.erase(charName);
}

void LogInOutManager::DeleteBySocketIO( rnSocketIOService* service )
{
	map_t::index<Key_socket_io>::type& p = map_.get<Key_socket_io>();
	p.erase(service);
}

//////////////////////////////////////////////////////////////////////////

bool LogInOutManager::isExistByUserIndex( int userIndex )
{
	map_t::index<Key_user_index>::type& p = map_.get<Key_user_index>();
	map_t::index<Key_user_index>::type::iterator it = p.find(userIndex);
	return (it != p.end()) ? true : false;
}

bool LogInOutManager::isExistByCharIndex( int charIndex )
{
	map_t::index<Key_char_index>::type& p = map_.get<Key_char_index>();
	map_t::index<Key_char_index>::type::iterator it = p.find(charIndex);
	return (it != p.end()) ? true : false;
}

bool LogInOutManager::isExistByUserId( std::string userId )
{
	map_t::index<Key_user_id>::type& p = map_.get<Key_user_id>();
	map_t::index<Key_user_id>::type::iterator it = p.find(userId);
	return (it != p.end()) ? true : false;
}

bool LogInOutManager::isExistByCharName( std::string charName )
{
	map_t::index<Key_char_name>::type& p = map_.get<Key_char_name>();
	map_t::index<Key_char_name>::type::iterator it = p.find(charName);
	return (it != p.end()) ? true : false;
}

//////////////////////////////////////////////////////////////////////////

rnSocketIOService* LogInOutManager::getSocketIOByUserIndex( int userIndex )
{
	map_t::index<Key_user_index>::type& p = map_.get<Key_user_index>();
	map_t::index<Key_user_index>::type::iterator it = p.find(userIndex);
	return (it != p.end()) ? (*it).service : NULL;
}

rnSocketIOService* LogInOutManager::getSocketIOByCharIndex( int charIndex )
{
	map_t::index<Key_char_index>::type& p = map_.get<Key_char_index>();
	map_t::index<Key_char_index>::type::iterator it = p.find(charIndex);
	return (it != p.end()) ? (*it).service : NULL;
}

rnSocketIOService* LogInOutManager::getSocketIOByUserId( std::string userId )
{
	map_t::index<Key_user_id>::type& p = map_.get<Key_user_id>();
	map_t::index<Key_user_id>::type::iterator it = p.find(userId);
	return (it != p.end()) ? (*it).service : NULL;
}

rnSocketIOService* LogInOutManager::getSocketIOByCharName( std::string charName )
{
	map_t::index<Key_char_name>::type& p = map_.get<Key_char_name>();
	map_t::index<Key_char_name>::type::iterator it = p.find(charName);
	return (it != p.end()) ? (*it).service : NULL;
}

//////////////////////////////////////////////////////////////////////////

const LogInOutManager::userInfo* LogInOutManager::getUserInfoByUserIndex( int userIndex )
{
	map_t::index<Key_user_index>::type& p = map_.get<Key_user_index>();
	map_t::index<Key_user_index>::type::iterator it = p.find(userIndex);
	return (it != p.end()) ? &(*it) : NULL;
}

const LogInOutManager::userInfo* LogInOutManager::getUserInfoByCharIndex( int charIndex )
{
	map_t::index<Key_char_index>::type& p = map_.get<Key_char_index>();
	map_t::index<Key_char_index>::type::iterator it = p.find(charIndex);
	return (it != p.end()) ? &(*it) : NULL;
}

const LogInOutManager::userInfo* LogInOutManager::getUserInfoByUserId( std::string userId )
{
	map_t::index<Key_user_id>::type& p = map_.get<Key_user_id>();
	map_t::index<Key_user_id>::type::iterator it = p.find(userId);
	return (it != p.end()) ? &(*it) : NULL;
}

const LogInOutManager::userInfo* LogInOutManager::getUserInfoByCharname( std::string charName )
{
	map_t::index<Key_char_name>::type& p = map_.get<Key_char_name>();
	map_t::index<Key_char_name>::type::iterator it = p.find(charName);
	return (it != p.end()) ? &(*it) : NULL;
}

void LogInOutManager::changeNickName( const char* oldnickname, const char* newnickname )
{
	map_t::index<Key_char_name>::type& p = map_.get<Key_char_name>();
	map_t::index<Key_char_name>::type::iterator it = p.find(oldnickname);
	if (it == p.end())
		return;

	p.modify_key(it, boost::lambda::_1=newnickname);
}
