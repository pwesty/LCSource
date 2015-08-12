#include <boost/foreach.hpp>
#include "stdhdrs.h"

#include "Log.h"
#include "Server.h"
#include "Friend.h"
#include "CmdMsg.h"
#include "doFunc.h"

CFriendMember::CFriendMember()
	: m_strFriendName(MAX_CHAR_NAME_LENGTH + 1)
{
	m_nChaIndex = -1;
//	m_nLastConnectDate = 0; //최근접속일자
	m_nJobType = -1; //종족
	m_nCondition = 	MSG_FRIEND_CONDITION_OFFLINE; //상태
	m_groupIndex = 0;
	m_pPc = NULL;
}

CFriendMember::~CFriendMember()
{
}
/*
int CFriendMember::GetChaIndex()
{
	return m_nChaIndex;
}
*/
void CFriendMember::SetChaIndex(int chaindex)
{
	m_nChaIndex = chaindex;
}

void CFriendMember::SetName(const char* name)
{
	if (name)
	{
		m_strFriendName = name;
	}
	else
	{
		m_strFriendName = "";
	}
}

/*
void CFriendMember::SetFriendListIndex(int listindex)
{
	m_nListIndex = listindex;
}
*/

void CFriendMember::SetCondition(int condition)
{
	m_nCondition = condition;
}

void CFriendMember::SetJob(int job)
{
	m_nJobType = job;
}

CFriend::CFriend()
{
	memset(m_pFriendMember, 0, sizeof(CFriendMember*) * FRIEND_MAX_MEMBER);
//	memset(m_aFriendMeChaIndex, -1, sizeof(int) * FRIEND_MAX_MEMBER);
	m_nFriendCnt = 0;
}

CFriend::~CFriend()
{
	for (int i = 0; i < FRIEND_MAX_MEMBER; i++)
	{
		if (m_pFriendMember[i])
			delete m_pFriendMember[i];
	}
	memset(m_pFriendMember, 0, sizeof(CFriendMember*) * FRIEND_MAX_MEMBER);
	m_nFriendCnt=0;
}

CFriendMember* CFriend::FindFriendMember(int chaindex)
{
	for (int i = 0; i < FRIEND_MAX_MEMBER; i++)
	{
		if (m_pFriendMember[i])
		{
			if (m_pFriendMember[i]->GetChaIndex() == chaindex)
				return m_pFriendMember[i];
		}
	}
	return NULL;
}

CFriendMember* CFriend::FindFriendMember(const char* name)
{
	for (int i = 0; i < FRIEND_MAX_MEMBER; i++)
	{
		if (m_pFriendMember[i])
		{
			if ( strcmp(m_pFriendMember[i]->GetName(), name) == 0 )
				return m_pFriendMember[i];
		}
	}
	return NULL;
}

//친구등록
int CFriend::AddFriend(int chaindex, const char* name, int job, int nCon, int groupIndex)
{
	if (m_nFriendCnt >= FRIEND_MAX_MEMBER)
	{
		return -1;
	}

	CFriendMember* pFriendMember;
	for (int i = 0; i < FRIEND_MAX_MEMBER; i++)
	{
		if (m_pFriendMember[i] == NULL)
		{
			pFriendMember = new CFriendMember();
			pFriendMember->SetChaIndex(chaindex);
			pFriendMember->SetName(name);
			pFriendMember->SetCondition(nCon);
			pFriendMember->SetJob(job);
			map_t::iterator it = m_groupList.find(groupIndex);
			if (it != m_groupList.end())
			{
				pFriendMember->SetGroup(groupIndex);
			}
			else
			{
				pFriendMember->SetGroup(0);
			}

			m_pFriendMember[i] = pFriendMember;
			m_nFriendCnt++;
			return i;	//리스트의 인덱스가 되겠지..
		}
	}

	return -1;
}

//친구삭제
void CFriend::RemoveFriend(int chaindex)
{
	for (int i = 0; i < FRIEND_MAX_MEMBER; i++)
	{
		if (m_pFriendMember[i])
		{
			if (m_pFriendMember[i]->GetChaIndex() == chaindex)
			{
				m_pFriendMember[i] = NULL;
				m_nFriendCnt--;
				return;
			}
		}
	}
}

/*
void CFriend::SetFriendList(int index, int friendindex, const char* name, int job, int condition)
{
	if(m_pFriendMember)
	{
		SetFriendCount(index+1);
//		m_pFriendMember[index]->SetFriendListIndex(index);
		m_pFriendMember[index]->SetChaIndex(friendindex);
		m_pFriendMember[index]->SetName(name);
		m_pFriendMember[index]->SetJob(job);
		m_pFriendMember[index]->SetCondition(condition);
	}
}
*/

void CFriend::SendToAll(CNetMsg::SP& msg)
{
	int i;
	for (i = 0; i < FRIEND_MAX_MEMBER; i++)
	{
		if (m_pFriendMember[i])
		{
			if (m_pFriendMember[i]->GetPC())
			{
				SEND_Q(msg, m_pFriendMember[i]->GetPC()->m_desc);
			}
		}
	}
}

int CFriend::addGroup(const char* gName, int gIndex)
{
	// 문자열 검사
	if (strinc(gName, "'") || strinc(gName, "%") || strinc(gName, " "))
	{
		return -1;
	}

	// 최대 갯수가 넘으면
	if (this->m_groupList.size() >= FRIEND_GROUP_MAX_NUMBER)
	{
		return -1;
	}

	// 동일 그룹명이 있는지 검색
	BOOST_FOREACH(map_t::value_type& p, this->m_groupList)
	{
		CFriendGroup* pGroup = p.second;
		if( strcmp( (const char*) pGroup->GetGroupName(), gName) == 0 )
			return -1;
	}

	// 디비에서 읽은 데이터이면 바로 입력 처리
	CFriendGroup* pGroup = NULL;
	if (gIndex != -1)
	{
		pGroup = new CFriendGroup(gIndex, gName);
	}
	else
	{
		// 최종 인덱스를 구함
		if (this->m_groupList.empty())
		{
			gIndex = 0;
		}
		else
		{
			// std::map은 자동으로 오름차순으로 정리되므로 맨마지막 key값이 가장 큼
			map_t::iterator it = --m_groupList.end();
			gIndex = (it->first) + 1;
		}

		pGroup = new CFriendGroup(gIndex, gName);
	}

	m_groupList.insert(map_t::value_type(gIndex, pGroup));

	return gIndex;
}

//void	delGroup(int gIndex);
//int		changeGroupName(int gIndex, const char* newName);
//int		moveGroupFriend(int gIndex, const char* charName);

int CFriend::delGroup(int gIndex)
{
	map_t::iterator it = m_groupList.find(gIndex);
	if (it != m_groupList.end())
	{
		m_groupList.erase(it);
		return gIndex;
	}

	return -1;
}

int CFriend::changeGroupName(int gIndex, const char* newName)
{
	if( strinc(newName, "'") || strinc(newName, "%") || strinc(newName, " ") )
	{
		return -1;
	}

	map_t::iterator it = m_groupList.find(gIndex);
	if (it != m_groupList.end())
	{
		CFriendGroup* fgroup = it->second;
		fgroup->SetName(newName);
		return gIndex;
	}

	return -1;
}

int CFriend::moveGroupFriend(int gIndex, int charIndex)
{
	CFriendMember* fmember = this->FindFriendMember(charIndex);

	if( fmember )
	{
		fmember->SetGroup(gIndex);
		return gIndex;
	}

	return -1;
}

void CFriend::GetGroupIndexString(CLCString& gIndexList, CLCString& gNameList)
{
	CFriendGroup* fgroup = NULL;
	map_t::iterator it = m_groupList.begin();
	map_t::iterator endit = m_groupList.end();
	char tmpbuf[1024] = {0,};
	for(; it != endit; ++it)
	{
		CFriendGroup* fgroup = it->second;

		sprintf(tmpbuf, "%d", fgroup->GetGroupIndex() );
		gIndexList += tmpbuf;
		gIndexList += " ";
		gNameList += fgroup->GetGroupName();
		gNameList += " ";
	}
}

CFriendGroup::~CFriendGroup()
{
}
//