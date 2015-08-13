#include <boost/format.hpp>
#include "stdhdrs.h"

#include "Syndicate.h"
#include "Server.h"
#include "../ShareLib/packetType/ptype_syndicate.h"

Syndicate::Syndicate()
{
	for (int i = 0; i < SYNDICATE::eSYNDICATE_MAX; ++i)
	{
		info_[i].jewelPoint_ = 0;
		info_[i].joinCount_ = 0;
		memset(info_[i].kingName, 0x00, sizeof(info_[i].kingName));
	}
}

void Syndicate::load()
{
	std::string select_rvr_jewel_point = "SELECT * FROM t_syndicate_jewel_point";
	CDBCmd cmd;

	cmd.Init(&gserver.m_dbchar);
	cmd.SetQuery(select_rvr_jewel_point);
	if (!cmd.Open())
	{
		LOG_ERROR("DB ERROR : LOAD CHARACTER SYNDICATE_HISTORY TABLE / query[%s] / error[%s]",
				  select_rvr_jewel_point.c_str(), mysql_error(cmd.m_dbconn));
	}
	else
	{
		if (0 < cmd.m_nrecords )
		{
			int syndicateType   = 0;
			LONGLONG jewelPoint = 0;
			LONGLONG userCount	= 0;
			int	kingIndex       = 0;
			while (cmd.MoveNext())
			{
				cmd.GetRec("a_syndicate_type"	, syndicateType);
				cmd.GetRec("a_jewel_point"		, jewelPoint);
				cmd.GetRec("a_user_count"		, userCount);
				cmd.GetRec("a_king_charIndex"	, kingIndex);

				this->setJewelPoint(syndicateType, jewelPoint);
				this->setUserCount(syndicateType, userCount);
				this->setKingIndex(syndicateType, kingIndex);
			}
		}
	}

	for(int i = 0; i < SYNDICATE::eSYNDICATE_MAX; i++)
	{
		std::string query;
		if( i == SYNDICATE::eSYNDICATE_KAILUX )
		{
			query = boost::str(boost::format("SELECT a_nick, a_index, a_syndicate_point_k FROM t_characters WHERE a_syndicate_type = %d and a_syndicate_point_k > %d") % i % MARQUISE_POINT);
		}
		else if( i == SYNDICATE::eSYNDICATE_DEALERMOON )
		{
			query = boost::str(boost::format("SELECT a_nick, a_index, a_syndicate_point_d FROM t_characters WHERE a_syndicate_type = %d and a_syndicate_point_d > %d") % i % ADEPTUS_POINT);
		}
		else
			continue;

		cmd.SetQuery(query);
		if(!cmd.Open())
		{
			LOG_ERROR("DB ERROR : LOAD CHARACTER SYNDICATE KING DATA / query[%s] / error[%s]",
					  query.c_str(), mysql_error(cmd.m_dbconn));
		}
		else
		{
			if ( cmd.m_nrecords > 0 )
			{
				std::string name;
				while(cmd.MoveNext())
				{
					Syndicate2ndUser	user;
					cmd.GetRec("a_index", user.charIndex);
					cmd.GetRec("a_nick", name);
					strcpy(user.userName, name.c_str());

					if(user.charIndex == this->getKingIndex(i))
					{
						user.isKing = true;
					}

					if(i == SYNDICATE::eSYNDICATE_DEALERMOON)
					{
						cmd.GetRec("a_syndicate_point_d", user.point);
						userList2nd_d.push_back(user);
					}
					else if(i == SYNDICATE::eSYNDICATE_KAILUX)
					{
						cmd.GetRec("a_syndicate_point_k", user.point);
						userList2nd_k.push_back(user);
					}

					if(user.isKing)
					{
						setKingIndex(i, user.charIndex);
						setKingPoint(i, user.point);
						setKingName(i, user.userName);
					}
				}
			}
		}
	}
}

void Syndicate::save()
{
	std::string deleteStr = "DELETE FROM t_syndicate_jewel_point";
	CDBCmd cmd;

	cmd.Init(&gserver.m_dbchar);
	cmd.SetQuery(deleteStr);
	if (!cmd.Update())
	{
		LOG_ERROR("query error : query[%s] / error[%s]", deleteStr.c_str(), mysql_error(&gserver.m_dbchar));
		return;
	}

	std::string insertStr = "INSERT INTO t_syndicate_jewel_point VALUES";
	insertStr.reserve(4096);

	for(int i = 0; i < SYNDICATE::eSYNDICATE_MAX; i++)
	{
		insertStr += boost::str(
						 boost::format("(%1%, %2%, %3%, %4%),")
						 % i % gserver.m_syndicate.getJewelPoint(i) % gserver.m_syndicate.getUserCount(i) % gserver.m_syndicate.getKingIndex(i) );
	}

	int index = insertStr.rfind(",");
	insertStr.erase(index);

	cmd.SetQuery(insertStr);
	if (!cmd.Update())
	{
		LOG_ERROR("query error : query[%s] / error[%s]", insertStr.c_str(), mysql_error(&gserver.m_dbchar));
		return;
	}

	LOG_INFO("query : query[%s]", insertStr.c_str());
	LOG_INFO("RVR System Save. K_Jewel_Point : %d, K_UserCount : %d, D_Jewel_Point : %d, D_UserCount : %d",
			 gserver.m_syndicate.getJewelPoint(SYNDICATE::eSYNDICATE_KAILUX), gserver.m_syndicate.getUserCount(SYNDICATE::eSYNDICATE_KAILUX),
			 gserver.m_syndicate.getJewelPoint(SYNDICATE::eSYNDICATE_DEALERMOON), gserver.m_syndicate.getUserCount(SYNDICATE::eSYNDICATE_DEALERMOON));
}

void Syndicate::increaseJewelPoint( int syndicateType, int point )
{
	info_[syndicateType].jewelPoint_ += point;
	sendInfo();
}

void Syndicate::decreaseJewelPoint( int syndicateType, int point )
{
	info_[syndicateType].jewelPoint_ -= point;
	
	if( info_[syndicateType].jewelPoint_ < 0 )
		info_[syndicateType].jewelPoint_ = 0;

	sendInfo();
}


int Syndicate::getJewelPoint( int syndicateType )
{
	return info_[syndicateType].jewelPoint_;
}

int Syndicate::getUserCount( int syndicateType )
{
	return this->info_[syndicateType].joinCount_;
}

char* Syndicate::getKingName( int syndicateType )
{
	return this->info_[syndicateType].kingName;
}

void Syndicate::setJewelPoint( int syndicateType, int point )
{
	info_[syndicateType].jewelPoint_ = point;
}

void Syndicate::setUserCount( int syndicateType, int count )
{
	info_[syndicateType].joinCount_ = count;
}

void Syndicate::setKingName( int syndicateType, const char* kingName )
{
	if(kingName == NULL)
		memset(info_[syndicateType].kingName, 0x00, sizeof(info_[syndicateType].kingName));
	else
	{
		strncpy(info_[syndicateType].kingName, kingName, MAX_CHAR_NAME_LENGTH);
		info_[syndicateType].kingName[MAX_CHAR_NAME_LENGTH - 1] = '\0';
	}
}



void Syndicate::sydicateJoin( int syndicateType )
{
	++info_[syndicateType].joinCount_;
	sendInfo();
}

void Syndicate::sydicateLeave( int syndicateType )
{
	--info_[syndicateType].joinCount_;
	sendInfo();
}

void Syndicate::sendInfo()
{
	CNetMsg::SP rmsg(new CNetMsg);
	UpdateServer::SyndicateInfo* packet = reinterpret_cast<UpdateServer::SyndicateInfo*>(rmsg->m_buf);

	packet->type = MSG_HELPER_COMMAND_STRUCT;
	packet->subType = MSG_HELPER_RVR_RVR_INFO;
	packet->jewelCount_d = gserver.m_syndicate.getJewelPoint(SYNDICATE::eSYNDICATE_DEALERMOON);
	packet->jewelCount_k = gserver.m_syndicate.getJewelPoint(SYNDICATE::eSYNDICATE_KAILUX);
	packet->userCount_d = gserver.m_syndicate.getUserCount(SYNDICATE::eSYNDICATE_DEALERMOON);
	packet->userCount_k = gserver.m_syndicate.getUserCount(SYNDICATE::eSYNDICATE_KAILUX);
	rmsg->setSize(sizeof(UpdateServer::SyndicateInfo));

	gserver.SendToAllGameServer(rmsg);
}

void Syndicate::sendKingInfo()
{
	CNetMsg::SP rmsg(new CNetMsg);
	UpdateServer::SyndicateUpdateKingInfoAll* packet = reinterpret_cast<UpdateServer::SyndicateUpdateKingInfoAll*>(rmsg->m_buf);

	packet->type = MSG_HELPER_COMMAND_STRUCT;
	packet->subType = MSG_HELPER_KING_INFO_UPDATE_ALL;

	strncpy(packet->kingName_d, info_[SYNDICATE::eSYNDICATE_DEALERMOON].kingName, MAX_CHAR_NAME_LENGTH);
	packet->kingName_d[MAX_CHAR_NAME_LENGTH] = '\0';
	packet->kingPoint_d = info_[SYNDICATE::eSYNDICATE_DEALERMOON].kingPoint;
	packet->kingIndex_d = info_[SYNDICATE::eSYNDICATE_DEALERMOON].kingIndex;

	strncpy(packet->kingName_k, info_[SYNDICATE::eSYNDICATE_KAILUX].kingName, MAX_CHAR_NAME_LENGTH);
	packet->kingName_k[MAX_CHAR_NAME_LENGTH] = '\0';
	packet->kingPoint_k = info_[SYNDICATE::eSYNDICATE_KAILUX].kingPoint;
	packet->kingIndex_k = info_[SYNDICATE::eSYNDICATE_KAILUX].kingIndex;

	rmsg->setSize(sizeof(UpdateServer::SyndicateUpdateKingInfoAll));

	gserver.SendToAllGameServer(rmsg);
}

void Syndicate::sendDownGrade(int type, int charIndex, int grade, LONGLONG point)
{
	CNetMsg::SP rmsg(new CNetMsg);
	UpdateServer::HelperServerRVRKingDownGrade(rmsg, type, charIndex, point);
	gserver.SendToAllGameServer(rmsg);
}


LONGLONG Syndicate::getKingPoint( int syndicateType )
{
	return info_[syndicateType].kingPoint;
}

void Syndicate::setKingPoint( int syndicateType, LONGLONG point )
{
	info_[syndicateType].kingPoint = point;
}

int Syndicate::getKingIndex( int syndicateType )
{
	return info_[syndicateType].kingIndex;
}

void Syndicate::setKingIndex( int syndicateType, int charIndex )
{
	info_[syndicateType].kingIndex = charIndex;
}

void Syndicate::add2ndUser(Syndicate2ndUser& st, int syndicateType)
{
	std::vector<Syndicate2ndUser>* vec_ = find2ndInfo(syndicateType);
	vec_->push_back(st);
}

void Syndicate::del2ndUser(int charIndex, int syndicateType)
{
	std::vector<Syndicate2ndUser>* vec_ = find2ndInfo(syndicateType);

	std::vector<Syndicate2ndUser>::iterator it = vec_->begin();
	std::vector<Syndicate2ndUser>::iterator it_end = vec_->end();

	for(; it != it_end; it++)
	{
		if(it->charIndex == charIndex)
		{
			it = vec_->erase(it);
			return;
		}
	}
}

bool Syndicate::checkKing(int type, LONGLONG point)
{
	if(point > getKingPoint(type))
	{
		return true;
	}
	return false;
}

void Syndicate::setKingData( int type, int charIndex, LONGLONG point, char* name )
{
	gserver.m_syndicate.setKingIndex(type, charIndex);
	gserver.m_syndicate.setKingName(type, name);
	gserver.m_syndicate.setKingPoint(type, point);
}

std::vector<Syndicate2ndUser>* Syndicate::find2ndInfo( int syndicateType )
{
	if(syndicateType == SYNDICATE::eSYNDICATE_DEALERMOON)
	{
		return &userList2nd_d;
	}
	else
	{
		return &userList2nd_k;
	}
}
