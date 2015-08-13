#include <boost/lambda/lambda.hpp>
#include "stdhdrs.h"
#include "Server.h"
#include "DescManager.h"

static DescManager local_instance;

DescManager* DescManager::instance()
{
	return &local_instance;
}

bool DescManager::insert( CDescriptor* desc )
{
	if (this->getDescByUserIndex(desc->m_index))
	{
		LOG_ERROR("duplication data : userIndex[%d] userId[%s]", desc->m_index, (const char *)desc->m_idname);
		return false;
	}

	desc_info info;
	info.m_userIndex = desc->m_index;
	info.m_id = desc->m_idname;
	info.m_desc = desc;

	map_.insert(info);
	return true;
}

bool DescManager::erase( CDescriptor* desc )
{
	map_t::index<Key_User_Index>::type& p = map_.get<Key_User_Index>();
	map_t::index<Key_User_Index>::type::iterator it = p.find(desc->m_index);
	if (it == p.end())
	{
		LOG_ERROR("not found data : userIndex[%d] userId[%s]", desc->m_index, (const char *)desc->m_idname);
		return false;
	}

	p.erase(it);

	return true;
}

CDescriptor* DescManager::getDescByUserIndex( int userIndex )
{
	map_t::index<Key_User_Index>::type& p = map_.get<Key_User_Index>();
	map_t::index<Key_User_Index>::type::iterator it = p.find(userIndex);

	return (it != p.end()) ? (*it).m_desc : NULL;
}

CDescriptor* DescManager::getDescById(const char* id)
{
	std::string tid(id);
	return this->getDescById(tid);
}

CDescriptor* DescManager::getDescById( std::string id )
{
	map_t::index<Key_Id>::type& p = map_.get<Key_Id>();
	map_t::index<Key_Id>::type::iterator it = p.find(id);

	return (it != p.end()) ? (*it).m_desc : NULL;
}

void DescManager::changeUserIndex( int oldUserIndex, int newUserIndex )
{
	map_t::index<Key_User_Index>::type& p = map_.get<Key_User_Index>();
	map_t::index<Key_User_Index>::type::iterator it = p.find(oldUserIndex);
	if (it == p.end())
		return;

	p.modify_key(it, boost::lambda::_1 = newUserIndex);
}

void DescManager::saveAllBeforServerDown()
{
	LOG_INFO(">> saving all user data because server down <<");

	map_t::index<Key_User_Index>::type& p = map_.get<Key_User_Index>();
	map_t::index<Key_User_Index>::type::iterator it = p.begin();
	map_t::index<Key_User_Index>::type::iterator endit = p.end();
	for (; it != endit; ++it)
	{
		gserver->CloseSocket((*it).m_desc);
	}
}
