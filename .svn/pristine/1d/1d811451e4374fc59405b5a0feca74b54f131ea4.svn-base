#ifndef __FRIEND_H__
#define __FRIEND_H__

#include <map>
#include "../ShareLib/MemoryPoolBase.h"

class Friend;

//친구의 상태표시
typedef enum _tagFriendConditionType
{
	MSG_FRIEND_CONDITION_OFFLINE, //오프라인
	MSG_FRIEND_CONDITION_NORMAL,  //일반상태
//		MSG_FRIEND_CONDITION_HUNTING, //사냥중
//		MSG_FRIEND_CONDITION_SELLING, //상점
	MSG_FRIEND_CONDITION_EMPTY,   //자리비움
} MSG_FRIEND_CONDITION_TYPE;

class CFriendGroup
{
	int m_gIndex;
	CLCString m_gName;

public:
	CFriendGroup();
	CFriendGroup(int gIndex, CLCString gName): m_gName(MAX_GROUP_NAME_LENGTH+1)
	{
		m_gIndex = gIndex;
		m_gName = gName;
	}
	~CFriendGroup();

	void SetName(const char* gName)
	{
		m_gName = gName;
	}

	int			GetGroupIndex()
	{
		return m_gIndex;
	}
	const char* GetGroupName()
	{
		return m_gName;
	}
};

class CFriendMember
{
	friend class CFriend;

	int   m_nChaIndex;
	CLCString m_strFriendName;
//	int   m_nLastConnectDate; //최근접속일자
	char  m_nJobType; //종족
	int   m_nCondition; //상태
//	int   m_nListIndex;
	int		m_groupIndex;  // 0은 기타

//	bool  m_bChatOn;

	CPC*			m_pPc;

public:
	~CFriendMember();
	CFriendMember();

	int GetChaIndex()
	{
		return m_nChaIndex;
	}

	void SetChaIndex(int chaindex);
	void SetName(const char* name);
//	void SetFriendListIndex(int listindex);
	void SetCondition(int condition);
	void SetJob(int job);
	void SetGroup(int gIndex)
	{
		m_groupIndex = gIndex;
	}

	const char* GetName()
	{
		return (const char*)m_strFriendName;
	}
	//int GetFriendListIndex() { return m_nListIndex; }
	int GetCondition()
	{
		return m_nCondition;
	}
	int GetJob()
	{
		return m_nJobType;
	}

	int GetGroup()
	{
		return m_groupIndex;
	}

	void SetPC(CPC* pc)
	{
		m_pPc = pc;
	}
	CPC* GetPC()
	{
		return m_pPc;
	}
};

//캐릭터쪽에 생성해야겠지...
class CFriend : public MemoryPoolBaseWithMutex<CFriend>
{
	//int m_nMyChaIndex; //필요없을지도..
	CFriendMember* m_pFriendMember[FRIEND_MAX_MEMBER];
	//int m_aFriendMeChaIndex[FRIEND_MAX_MEMBER];
	//int m_nFriendMe;
	int	m_nFriendCnt;
	//int  m_aGroupChat[10]; //그룹채팅중인 캐릭터 인덱스
	//int  m_nGroupChatMemberSum; //0일때는 No그룹채팅. 1이상은 그룹채팅 인원.
	typedef std::map<int, CFriendGroup*> map_t;
	map_t	m_groupList;
	int		m_chatColor;

public:
	CFriend();
	~CFriend();
	int AddFriend(int chaindex, const char* name, int job, int nCond, int m_groupIndex);
	void RemoveFriend(int chaindex);
	CFriendMember* FindFriendMember(int chaindex);
	CFriendMember* FindFriendMember(const char* name);
	int GetFriendSum()
	{
		return m_nFriendCnt;
	}
	void SetFriendCount(int count)
	{
		m_nFriendCnt = count;
	}
	int GetFriendCount()
	{
		return m_nFriendCnt;
	}
	CFriendMember* GetFriendMember(int index)
	{
		return (index < 0 || index >= FRIEND_MAX_MEMBER) ? NULL : m_pFriendMember[index];
	};
//	void SetFriendList(int index, int friendindex, const char* name, int job, int condition);
	void SendToAll(CNetMsg::SP& msg);
//	void SetMyCondition(int index, int condition);

	void SetChatColor(int charColor)
	{
		m_chatColor = charColor;
	}
	int GetChatColor()
	{
		return m_chatColor;
	}

	// -1 이면 에러 나머지는 gIndex를 리턴
	int		addGroup(const char* gName, int gIndex = -1 );
	// -1 이면 에러 나머지는 gIndex를 리턴
	int	delGroup(int gIndex);
	// -1 이면 에러 나머지는 gIndex를 리턴
	int	changeGroupName(int gIndex, const char* newName);
	// -1 이면 에러 나머지는 gIndex를 리턴
	int	moveGroupFriend(int gIndex, int charIndex);
	void	GetGroupIndexString(CLCString& gIndexList, CLCString& gNameList);
	int		GetGroupCount()
	{
		return m_groupList.size();
	}
};

#endif
//
