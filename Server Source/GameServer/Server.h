#ifndef __ASERVER_H__
#define __ASERVER_H__

#include <set>
#include <map>
#include "../ShareLib/rnsocketiohandler.h"
#include "../ShareLib/rnsocketioservice.h"
#include "../ShareLib/DBCmd.h"
#include "ConfigFile.h"
#include "Zone.h"
#include "Item.h"
#include "JewelData.h"
#include "Skill.h"
#include "Quest.h"
#include "Option.h"
#include "SSkill.h"
#include "Guild.h"
#include "Friend.h"
#include "RareOptionProto.h"
#include "MonsterCombo.h"
#include "ExtremeCube.h"
#include "PacketTime.h"
#include "LuckyDrawBox.h"

#include "MessengerInGame.h"
#include "ConnectorInGame.h"
#include "HelperInGame.h"
#include "SubHelperInGame.h"

#include "PCList.h"

#include "RewardEvnetItemManager.h"

#include "TitleSystem.h"

#ifdef LACARETTE_SYSTEM
#include "LacaRette.h"
#endif

#include "Expedition.h"

#include "SetItemProto.h"

#include "TrigerSet.h"

#include "Factory.h"

#include "Affinity.h"

#include "RaidData.h"

#ifdef DEV_EVENT_PROMOTION2
#include "Promotion2Event.h"
#endif

#include "MonsterMercenary.h"

#ifdef XTRAP
#ifdef XTRAP_DUMP
#include	"HexaDump4XTrap.h"
#endif //XTRAP_DUMP
#include	"Xtrap_S_Interface.h"
#define		CSFILE_NUM				2			// 사용할 맵파일 개수
#define		XTRAP_CS4_BUFSIZE_MAP	13000		// 맵파일 사이즈 고정
#endif //XTRAP

#include "WarGround.h"

#include <set>

#include "Reward.h"

#include "EventAutomation.h"
#include "DropItem.h"

#if defined (INTERGRATION_SERVER)
#include "IntergrationServer.h"
#endif

#include "RockPaperScissorsGame.h"
#include "PuzzleGame.h"

#include "Fortune_system.h"

#include "SyndicateInfoDataManager.h"
#include "GPSDataManager.h"

#include "AutoBuffer.h"

#define		RESET_RAID_SAVEFILE ".setraidinittime"

#define MAKE_LONGLONG_KEY(x, y)			(((LONGLONG)(x) << 32) | (y))

#ifdef DEV_EVENT_AUTO
class CKingsBirthdayDropItem
{
public:
	CKingsBirthdayDropItem()
		: m_nDropItemIndex(0), m_nDropCount(0), m_nDropProb(0), m_nPoint(0), m_strType()
	{}
	~CKingsBirthdayDropItem() {}
	void setDropItemIndex(int val)
	{
		m_nDropItemIndex = val;
	}
	void setDropCount(int val)
	{
		m_nDropCount = val;
	}
	void setDropProb(int val)
	{
		m_nDropProb = val;
	}
	void setPoint(int val)
	{
		m_nPoint = val;
	}
	void setType(const char* str)
	{
		m_strType.Format(str);
	}
	int getDropItemIndex()
	{
		return m_nDropItemIndex;
	}
	int getDropItemCount()
	{
		return m_nDropCount;
	}
	int getDropItemProb()
	{
		return m_nDropProb;
	}
	int getPoint()
	{
		return m_nPoint;
	}
	const char* getType()
	{
		return m_strType;
	}
private:
	int m_nDropItemIndex;
	int m_nDropCount;
	int m_nDropProb;
	int m_nPoint;
	CLCString m_strType;
};

class CEventKingsBirthdayRewardItem
{
public:
	CEventKingsBirthdayRewardItem()
		: m_nItemIndex(0)
	{}
	~CEventKingsBirthdayRewardItem() {}
	void setItemIndex(int val)
	{
		m_nItemIndex = val;
	}
	int getItemIndex()
	{
		return m_nItemIndex;
	}
	void setRewardItem(CConfigFile& eventConfig)
	{
		setItemIndex(atoi(eventConfig.Find("TLD_KB_HEART_REWARD", "HeartRewardIndex")));
	}
	void giveRewardItem(CPC* pc, int giveCount);
private:
	int m_nItemIndex;
};

class CEventKingsBirthdayFlagReward
{
public:
	CEventKingsBirthdayFlagReward()
		: m_nCount(0)
	{}
	~CEventKingsBirthdayFlagReward()
	{
		//if(m_flagReward)
		//{
		//	delete[] m_flagReward;
		//}
		if(m_flagReward.size())
		{
			std::vector<CEventKingsBirthdayRewardItem*>::iterator it;
			std::vector<CEventKingsBirthdayRewardItem*>::iterator itEnd = m_flagReward.end();
			for(it = m_flagReward.begin(); it != itEnd; it++)
			{
				if(*it)
				{
					delete *it;
					*it = NULL;
				}
			}
			m_flagReward.clear();
		}
	}
	void setCount(int val)
	{
		m_nCount = val;
	}
	int getCount()
	{
		return m_nCount;
	}
	void setFlagReward(CConfigFile& eventConfig);
	CEventKingsBirthdayRewardItem* getFlagRewardItem(int job);
	void giveFlagRewardItem(CPC* pc);
private:
	int m_nCount;
	//CEventKingsBirthdayRewardItem* m_flagReward;
	std::vector<CEventKingsBirthdayRewardItem*> m_flagReward;
};

class CEventKingsBirthdayDropItemManager
{
public:
	CEventKingsBirthdayDropItemManager()
		: m_nDropItemCount(0)
	{}
	~CEventKingsBirthdayDropItemManager()
	{
		//if(m_DropItem)
		//{
		//	delete[] m_DropItem;
		//}
		if(m_DropItem.size())
		{
			std::vector<CKingsBirthdayDropItem*>::iterator it;
			std::vector<CKingsBirthdayDropItem*>::iterator itEnd = m_DropItem.end();
			for(it = m_DropItem.begin(); it != itEnd; it++)
			{
				if(*it)
				{
					delete *it;
					*it = NULL;
				}
			}
			m_DropItem.clear();
		}
	}
	void setDropItem(CConfigFile& eventConfig);
	int getDropItemCount()
	{
		return m_nDropItemCount;
	}
	void setDropItemCount(int val)
	{
		m_nDropItemCount = val;
	}
	CKingsBirthdayDropItem* getDropItem(int arrayIndex);
	CItem* getItem(int prob);
	int calcPoint(CPC* pc);
	void deleteDropItem(CPC* pc);
	void deleteFlagItem(CPC* pc);
private:
	int m_nDropItemCount;
	//CKingsBirthdayDropItem* m_DropItem;
	std::vector<CKingsBirthdayDropItem*> m_DropItem;
};

class CLevelSection
{
public:
	CLevelSection()
		: m_nMinLevel(0), m_nMaxLevel(0), m_nProb(0)
	{}
	~CLevelSection() {}
	void setMinLevel(int val)
	{
		m_nMinLevel = val;
	}
	void setMaxLevel(int val)
	{
		m_nMaxLevel = val;
	}
	void setProb(int val)
	{
		m_nProb = val;
	}
	int getMinLevel()
	{
		return m_nMinLevel;
	}
	int getMaxLevel()
	{
		return m_nMaxLevel;
	}
	int getProb()
	{
		return m_nProb;
	}
private:
	int m_nMinLevel;
	int m_nMaxLevel;
	int m_nProb;
};

class CEventKingsBirthdayDropProbTable
{
public:
	CEventKingsBirthdayDropProbTable()
		: m_nCount(0), m_firstPriority(0), m_secondPriority(0)
	{}
	~CEventKingsBirthdayDropProbTable()
	{
		//if(m_levelSection)
		//delete[] m_levelSection;

		if(m_levelSection.size())
		{
			std::vector<CLevelSection*>::iterator it;
			std::vector<CLevelSection*>::iterator itEnd = m_levelSection.end();
			for(it = m_levelSection.begin(); it != itEnd; it++)
			{
				if(*it)
				{
					delete *it;
					*it = NULL;
				}
			}
			m_levelSection.clear();
		}
	}
	void setProbTable(CConfigFile& eventConfig);
	int getProb(CNPC* npc);
	void setCount(int val)
	{
		m_nCount = val;
	}
	void setFirstPriority(int val)
	{
		m_firstPriority = val;
	}
	void setSecondPriority(int val)
	{
		m_secondPriority = val;
	}
	int getFirstPriority()
	{
		return m_firstPriority;
	}
	int getSecondPriority()
	{
		return m_secondPriority;
	}
	int getCount()
	{
		return m_nCount;
	}

private:
	int m_nCount;
	//CLevelSection* m_levelSection;
	std::vector<CLevelSection*> m_levelSection;
	int m_firstPriority;
	int m_secondPriority;
};

class CEventKingsBirthday
{
public:
	CEventKingsBirthday()
		:m_bEventStart(false), m_bEventItemDelete(false)
	{}
	~CEventKingsBirthday() {}
	void setEventStart(bool b)
	{
		m_bEventStart = b;
	}
	void setEventDeleteItem(bool b)
	{
		m_bEventItemDelete = b;
	}
	bool getEventStart()
	{
		return m_bEventStart;
	}
	bool getEventDeleteItem()
	{
		return m_bEventItemDelete;
	}
	void LoadDropItemManager(CConfigFile& eventConfig)
	{
		m_DropItemManager.setDropItem(eventConfig);
	}
	void LoadHeartItem(CConfigFile& eventConfig)
	{
		m_HeartRewardItem.setRewardItem(eventConfig);
	}
	void LoadFlagItem(CConfigFile& eventConfig)
	{
		m_FlagRewardItem.setFlagReward(eventConfig);
	}
	void LoadItemDropProbTable(CConfigFile& eventConfig)
	{
		m_ItemDropProbTable.setProbTable(eventConfig);
	}
	void Load(CConfigFile& eventConfig);
	CEventKingsBirthdayDropItemManager&		getDropItemManager()
	{
		return m_DropItemManager;
	}
	CEventKingsBirthdayRewardItem&			getHeartRewardItem()
	{
		return m_HeartRewardItem;
	}
	CEventKingsBirthdayFlagReward&			getFlagRewardItem()
	{
		return m_FlagRewardItem;
	}
	CEventKingsBirthdayDropProbTable&		getItemProbTable()
	{
		return m_ItemDropProbTable;
	}
private:
	bool m_bEventStart;
	bool m_bEventItemDelete;
	CEventKingsBirthdayDropItemManager	m_DropItemManager;
	CEventKingsBirthdayRewardItem		m_HeartRewardItem;
	CEventKingsBirthdayFlagReward		m_FlagRewardItem;
	CEventKingsBirthdayDropProbTable	m_ItemDropProbTable;
};
class CZoneDropProb
{
public:
	CZoneDropProb()
		: m_nZoneIndex(-1), m_nMultiple(1)
	{}
	~CZoneDropProb() {}
	void setZoneIndex(int val)
	{
		m_nZoneIndex = val;
	}
	void setMultiple(int val)
	{
		m_nMultiple = val;
	}
	int getZoneIndex()
	{
		return m_nZoneIndex;
	}
	int getMultiple()
	{
		return m_nMultiple;
	}
private:
	int m_nZoneIndex;
	int m_nMultiple;
};
class CDropProbTable
{
public:
	CDropProbTable()
		: m_nLevelSectionCount(0), m_nZoneCount(0), m_underLevel(0), m_division(1), m_upperLevel(0), m_multiple(1)
	{}
	~CDropProbTable()
	{
		if(m_levelSection.size())
		{
			std::vector<CLevelSection*>::iterator lit;
			std::vector<CLevelSection*>::iterator litEnd = m_levelSection.end();
			for(lit = m_levelSection.begin(); lit != litEnd; lit++)
			{
				delete *lit;
				*lit = NULL;
			}
			m_levelSection.clear();
		}
		if(m_zoneDropProb.size())
		{
			std::vector<CZoneDropProb*>::iterator zit;
			std::vector<CZoneDropProb*>::iterator zitEnd = m_zoneDropProb.end();
			for(zit = m_zoneDropProb.begin(); zit != zitEnd; zit++)
			{
				delete *zit;
				*zit = NULL;
			}
			m_zoneDropProb.clear();
		}
	}
	void Load(CConfigFile& eventConfig);
	void setLevelSectionCount(int val)
	{
		m_nLevelSectionCount = val;
	}
	int getLevelSectionCount()
	{
		return m_nLevelSectionCount;
	}
	void setZoneCount(int val)
	{
		m_nZoneCount = val;
	}
	int getZoneCount()
	{
		return m_nZoneCount;
	}
	//void setFirstPriority(int val){m_firstPriority = val;}
	//int getFirstPriority(){return m_firstPriority;}
	//void setSecondPriority(int val){m_secondPriority = val;}
	//int getSecondPriority(){return m_secondPriority;}
	void setUnderLevel(int val)
	{
		m_underLevel = val;
	}
	void setUpperLevel(int val)
	{
		m_upperLevel = val;
	}
	void setDivision(int val)
	{
		m_division = val;
	}
	void setMultiple(int val)
	{
		m_multiple = val;
	}
	int getUnderLevel()
	{
		return m_underLevel;
	}
	int getUpperLevel()
	{
		return m_upperLevel;
	}
	int getDivision()
	{
		return m_division;
	}
	int getMultiple()
	{
		return m_multiple;
	}
	int getProb(CPC*pc, CNPC* npc);
private:
	int m_nLevelSectionCount;
	int m_nZoneCount;
	std::vector<CLevelSection*> m_levelSection;
	std::vector<CZoneDropProb*> m_zoneDropProb;
	int m_underLevel;
	int m_division;
	int m_upperLevel;
	int m_multiple;
};

class CDropItem
{
public:
	CDropItem()
		: m_nItemIndex(-1), m_nDropCount(0)
	{}
	~CDropItem() {}
	void setItemIndex(int val)
	{
		m_nItemIndex = val;
	}
	int getIetmIndex()
	{
		return m_nItemIndex;
	}
	void setDropCount(int val)
	{
		m_nDropCount = val;
	}
	int getDropCount()
	{
		return m_nDropCount;
	}
private:
	int m_nItemIndex;
	int m_nDropCount;
};

class CDropItemManager
{
public:
	CDropItemManager()
		: m_nCount(0) {}
	~CDropItemManager()
	{
		if(m_DropItem.size())
		{
			std::vector<CDropItem*>::iterator it;
			std::vector<CDropItem*>::iterator itEnd = m_DropItem.end();
			for(it = m_DropItem.begin(); it != itEnd; it++)
			{
				delete *it;
				*it = NULL;
			}
			m_DropItem.clear();
		}
	}
	void setCount(int val)
	{
		m_nCount = val;
	}
	int getCount()
	{
		return m_nCount;
	}
	void Load(CConfigFile& eventConfig);
	CItem* getItem();
private:
	int m_nCount;
	std::vector<CDropItem*> m_DropItem;
};

class CItemDropEvent
{
public:
	CItemDropEvent()
		: m_bEventStart(false)
	{}
	~CItemDropEvent() {}
	void Load(CConfigFile& eventConfig);
	void LoadItemDropManager(CConfigFile& eventConfig);
	void LoadDropProbTable(CConfigFile& eventConfig);
	void setEnable(bool b)
	{
		m_bEventStart = b;
	}
	bool getEnable()
	{
		return m_bEventStart;
	}
	CDropItemManager& getDropItemManager()
	{
		return m_DropItemManager;
	}
	CDropProbTable& getDropProbTable()
	{
		return m_DropProbTable;
	}
private:
	bool m_bEventStart;
	CDropItemManager m_DropItemManager;
	CDropProbTable m_DropProbTable;
};
#endif // DEV_EVENT_AUTO

#define MAX_GECHO_SIZE 10
class CGrobalEcho
{
private:
	unsigned int	remainTime[MAX_GECHO_SIZE];
	unsigned int	repeatTime[MAX_GECHO_SIZE];
	unsigned int	checkClock[MAX_GECHO_SIZE];
	CNetMsg::SP		msg[MAX_GECHO_SIZE];

	int	FindIndex( );

public:
	CGrobalEcho( );
	void Set( CNetMsg::SP& _msg, unsigned int _repeatTime , unsigned int _time );
	void Print( );
};

typedef struct _tagGMIPData
{
	char	prefix[20];
	int		from;
	int		to;
} GMIPDATA;

class COXQuizData
{
	int				m_nIndex;			// 퀴즈 번호
	CLCString		m_strQuiz;			// 퀴즈 문장
	bool			m_bAnswerOX;		// true : O, false : X

public:
	COXQuizData(int nIndex, const char* strQuiz, bool bAnswerOX)
		: m_strQuiz(256)
	{
		m_nIndex = nIndex;
		m_strQuiz = strQuiz;
		m_bAnswerOX = bAnswerOX;
	}

	int GetQuizIndex() const
	{
		return m_nIndex;
	}
	const char* GetQuizString() const
	{
		return (const char*)m_strQuiz;
	}
	bool GetAnswer() const
	{
		return m_bAnswerOX;
	}
};

class CGoldenBallEventData
{
	int			m_nStatus;			// 현재 진행 상태

	CLCString	m_strTeam1;			// A팀 이름
	CLCString	m_strTeam2;			// B팀 이름
	int			m_nTeam1Score;		// A팀 점수
	int			m_nTeam2Score;		// B팀 점수

	int			m_nEndYear;			// 종료 시기 : 년
	int			m_nEndMonth;		// 종료 시기 : 월
	int			m_nEndDay;			// 종료 시기 : 일
	int			m_nEndHour;			// 종료 시기 : 시
	int			m_nEndMinute;		// 종료 시기 : 분

	time_t		m_tEndTime;			// 종료 시기 : time_t
	time_t		m_tVoteEndTime;		// 응모 마감 시각 : time_t

public:
	CGoldenBallEventData()
		: m_strTeam1(GOLDENBALL_TEAM_NAME_LENGTH + 1)
		, m_strTeam2(GOLDENBALL_TEAM_NAME_LENGTH + 1)
	{
		m_nStatus = GOLDENBALL_STATUS_NOTHING;

		m_nTeam1Score = 0;
		m_nTeam2Score = 0;

		m_nEndYear = 0;
		m_nEndMonth = 0;
		m_nEndDay = 0;
		m_nEndHour = 0;
		m_nEndMinute = 0;

		m_tEndTime = 0;
		m_tVoteEndTime = 0;
	}

	int GetStatus()
	{
		return m_nStatus;
	}

	const char* GetTeam1Name()
	{
		return m_strTeam1;
	}
	const char* GetTeam2Name()
	{
		return m_strTeam2;
	}
	int GetTeam1Score()
	{
		return m_nTeam1Score;
	}
	int GetTeam2Score()
	{
		return m_nTeam2Score;
	}

	int GetEndYear()
	{
		return m_nEndYear;
	}
	int GetEndMonth()
	{
		return m_nEndMonth;
	}
	int GetEndDay()
	{
		return m_nEndDay;
	}
	int GetEndHour()
	{
		return m_nEndHour;
	}
	int GetEndMinute()
	{
		return m_nEndMinute;
	}

	time_t GetEndTime()
	{
		return m_tEndTime;
	}
	time_t GetVoteEndTime()
	{
		return m_tVoteEndTime;
	}

	void Vote(const char* strTeam1, const char* strTeam2, int nYear, int nMonth, int nDay, int nHour, int nMin)
	{
		m_nStatus = GOLDENBALL_STATUS_VOTE;

		m_strTeam1 = strTeam1;
		m_strTeam2 = strTeam2;
		m_nTeam1Score = 0;
		m_nTeam2Score = 0;

		m_nEndYear = nYear;
		m_nEndMonth = nMonth;
		m_nEndDay = nDay;
		m_nEndHour = nHour;
		m_nEndMinute = nMin;

		struct tm tmEndTime;
		memset(&tmEndTime, 0, sizeof(tmEndTime));
		tmEndTime.tm_year	= nYear - 1900;
		tmEndTime.tm_mon	= nMonth - 1;
		tmEndTime.tm_mday	= nDay;
		tmEndTime.tm_hour	= nHour;
		tmEndTime.tm_min	= nMin;
		tmEndTime.tm_isdst	= -1;
		m_tEndTime = mktime(&tmEndTime);
		m_tVoteEndTime = m_tEndTime;
	}

	void Gift(const char* strTeam1, int nTeam1Score, const char* strTeam2, int nTeam2Score, int nYear, int nMonth, int nDay, int nHour, int nMin, time_t timeEndVote)
	{
		m_nStatus = GOLDENBALL_STATUS_GIFT;

		m_strTeam1 = strTeam1;
		m_strTeam2 = strTeam2;
		m_nTeam1Score = nTeam1Score;
		m_nTeam2Score = nTeam2Score;

		m_nEndYear = nYear;
		m_nEndMonth = nMonth;
		m_nEndDay = nDay;
		m_nEndHour = nHour;
		m_nEndMinute = nMin;

		struct tm tmEndTime;
		memset(&tmEndTime, 0, sizeof(tmEndTime));
		tmEndTime.tm_year	= nYear - 1900;
		tmEndTime.tm_mon	= nMonth - 1;
		tmEndTime.tm_mday	= nDay;
		tmEndTime.tm_hour	= nHour;
		tmEndTime.tm_min	= nMin;
		tmEndTime.tm_isdst	= -1;
		m_tEndTime = mktime(&tmEndTime);
		m_tVoteEndTime = timeEndVote;
	}

	void EndVote()
	{
		m_nStatus = GOLDENBALL_STATUS_VOTE_END;
	}

	void End()
	{
		m_nStatus = GOLDENBALL_STATUS_NOTHING;
	}
};

class CChatGroup
{
	int m_makeCharIndex;
	int m_chatIndex;

public:

	std::set<CLCString> m_charNameList;

	CChatGroup(int makeCharIndex, int chatIndex);
	~CChatGroup();

	int GetMakeCharIndex()
	{
		return m_makeCharIndex;
	}
	int GetChatIndex()
	{
		return m_chatIndex;
	}

	void SendGroup(MSG_EX_MESSENGER_TYPE subtype, int chatColor, CLCString charName, CLCString chat);
	bool CheckSameName(CLCString charName);
};


class CMoonStoneRewardData
{
public:
	typedef struct __reward
	{
		int		rewardindex;
		int		rewardcount;
		int		rewardflag;
		float	prob;
	} REWARD;

	CMoonStoneRewardData();
	~CMoonStoneRewardData();

	int	m_nMoonStoneIndex;
	int	m_nTotalProb;

	typedef std::map<int, REWARD*> map_reward_t;
	map_reward_t	m_listReward;

	bool Load(int idx);
};

class CMoonStoneReward
{
public:
	CMoonStoneReward();
	~CMoonStoneReward();

	typedef std::map<int, CMoonStoneRewardData*> map_t;
	int	m_nRewardDataCount;
	CMoonStoneRewardData* m_pReward;
	map_t					map_;

	bool	Load();
	CMoonStoneRewardData*	Find(int moonstoneidx)
	{
		map_t::iterator it = map_.find(moonstoneidx);
		return (it != map_.end()) ? it->second : NULL;
	}
};

typedef std::map<int, CParty*>	map_listparty_t;
typedef std::map<int, CExpedition*>	map_listexped_t;
typedef std::map<int, CPartyMatchMember*> map_listPartyMatchMember_t;
typedef std::map<int, CPartyMatchParty*> map_listPartyMatchParty_t;
typedef std::map<LONGLONG, CChatGroup*> map_chatList_t;
typedef std::map<int, COXQuizData*> map_listOXQuiz_t;
typedef std::vector<CNPC*>	vec_listNpcDropItemAtTime_t;
typedef std::map<int, CZone*> map_zone_t;

class CServer : public rnSocketIOHandler
{
public:
	friend class ServerTimerPerHundredMSecond;

public:
	CServer();
	~CServer();

	bool barunsongames_flag;		// 사내 서버이면 true

	int m_hardcore_flag_in_gameserver;			// 가미고에서 사용하는 hardcore server, 0 - no, 1 - yes

	////////////
	// 소켓 관련
	MessengerInGame* m_messenger;		// 메신저 연결
	ConnectorInGame* m_connector;		// 빌링 서버 연결
	HelperInGame* m_helper;			// 헬퍼 연결
	SubHelperInGame* m_subHelper;			// 헬퍼 연결

	int m_nGuildMarkTable[3];

	////////////
	// 서버 설정
	char*		m_serverpath;		// 모듈 경로
	CConfigFile	m_config;			// 서버 설정
#ifdef DEV_EVENT_AUTO
	CConfigFile m_eventConfig;		// Event Config
	CEventKingsBirthday m_fathersDay;
	CItemDropEvent m_DropEvent;
#endif

	int			m_national;			// 국가
	int			m_serverno;			// 서버 번호
	int			m_subno;			// 서브 번호
	CLCString	m_serverAddr;
	int			m_serverPort;
#ifdef GER_LOG
	char*		m_hostname;			// hostname 저장.
#endif

	int			m_maxplayers;		// 허용 가능 최대 플레이어 수
	bool		m_nameserverisslow; // 왜 쓰는 것일까?
	bool		m_bShutdown;			// 쎠따운
	int			m_pulseReboot;		// 리부트까지 남은 시간
	int			m_pulseShutdown;	// 셧다운까지 남은 시간
	int			m_pulseEndGame;		// 전체 서버 다운까지 남은 시간
	int			m_clientversionMin;	// 클라이언트 버전
	int			m_clientversionMax;	// 클라이언트 버전
	bool		m_bOnlyLocal;		// 외부 접속 허용 여부
	int			m_pulseProcHeartBeatPerSec;		// HeartBeat()에서 매초 처리를 위한 변수

	LONGLONG	m_statisticsItemBuy;
	LONGLONG	m_statisticsItemSell;
	LONGLONG	m_statistics152;
	LONGLONG	m_statistics153;
	LONGLONG	m_statistics154;
	LONGLONG	m_statistics155;
	LONGLONG	m_statistics156;
	LONGLONG	m_statistics157;
	LONGLONG	m_statistics158;
	LONGLONG	m_statistics159;
	LONGLONG	m_statistics160;
	LONGLONG	m_statistics161;
	LONGLONG	m_statistics162;
	LONGLONG	m_statistics163;
	LONGLONG	m_statistics197;
	LONGLONG	m_statistics198;
	LONGLONG	m_statistics199;
#define STATISTICS(x, v)		gserver->m_statistics##x += (v)

	LONGLONG	m_taxItem;			// 성주에게 줄 세금 : 상점 판매,구매
	LONGLONG	m_taxProduceCastle;	// 성주에게 줄 세금 : 생산
	int			m_taxSavePulse;		// 세금 갱신 주기 : 성 정보 갱신
	int			m_taxDivGuild;		// 길드에 세금 전달 했는지 여부 : 전달 후 해당 요일을 설정, -1이면 전달 한적 없는 상태

	LONGLONG	m_taxItemDratan;			// 성주에게 줄 세금 : 상점 판매,구매, 공성 입장료
	LONGLONG	m_taxProduceCastleDratan;	// 성주에게 줄 세금 : 생산

	bool		m_bLoadPartyInfo;	// 헬퍼로부터 현재 파티 정보를 모두 받았는지 저장

	struct timeval	m_tvLoop;			// 루프 시간 측정용
	int				m_nLoop;			// 루프 카운트
	int				m_bLoop;			// 측정 시작 여부
	CDescriptor*	m_descLoop;			// 요청 디스크립터
	CDescriptor*	m_observer;			// 옵저버
	CDescriptor*	m_chatmonitor[20];	// 채팅 모니터

#ifdef GMTOOL
	CDescriptor**	m_gmtool;
	int				m_nGMToolCount;
	void			GMToolCharPrePlay(CDescriptor* d);			// 지엠툴로 접속했을 때
#endif // GMTOOL

	///////////////////
	// 전체 파티 리스트
	map_listparty_t			m_listParty;	// 서버내 전체 파티 리스트

	// 전체 원정대 리스트
	map_listexped_t			m_listExped;	// 서버내 전체 원정대 리스트

	///////////////////
	// 파티 매칭 테이블
	map_listPartyMatchMember_t		m_listPartyMatchMember;		// 일반 매칭
	map_listPartyMatchParty_t		m_listPartyMatchParty;		// 파티 매칭

	// 새로운 메신저 채팅방
	map_chatList_t					m_chatList;
	// 1 : 맴버만 추가 0 : 채팅방 생성 -1 : 실패
	int		addChatGroup(int& m_makeCharIndex, int& chatIndex,  CLCString charName);
	void	delChatGroup(int m_makeCharIndex, int chatIndex, CLCString charName);
	void	SendChatGroup(MSG_EX_MESSENGER_TYPE subtype, int m_makeCharIndex, int chatIndex, int chatColor, CLCString charName, CLCString chat);
	CChatGroup*	FindChatGroup(int makeCharIndex, int chatIndex);
	void	discCharChatGroup(CLCString charName);
	int		GetMaxChatIndexPerPC(int makeCharIndex);

	// 게임시간
	int getNowSecond()
	{
		return (int)m_nowseconds;
	}

	///////////
	// 아이템 관련
	CItemList			m_itemProtoList;		// 전체 아이템 리스트
	CCatalogList		m_catalogList;

	CJewelDataList				m_jewelDataList;

	std::set<int>		m_limitCT;

	CSetItemProtoList	m_setItemProtoList;		// 세트 아이템 리스트

	CFactoryProtoList	m_factoryItemProtoList;		// 제작 아이템 리스트

	RewardEvnetItemManager m_rewardItemList;	// 출석 이벤트 보상 아이템 리스트

	//////////
	// 틱 관련
	time_t		m_nowseconds;			// unix time (ServerTimerPerSecond::operate()에서 값 셋팅)
	int			m_pulse;			// 틱 카운터 (1/10초 마다 1씩 증가함 : ServerTimerPerHundredMSecond::operate()에서 값 셋팅)
	int			m_resetWeather;		// 날씨 변화 : 현재 30분
	int			m_resetAdjustItem;	// Adjust Item 로그 출력 : 1분

	//////////
	// DB 연결
	MYSQL		m_dbchar;			// 캐릭터 데이터
	MYSQL		m_dbdata;			// 게임 데이터
	MYSQL&		m_dbcastle;			// 공성 데이터
	MYSQL&		m_dbTrigger;		// 트리거 데이터

#ifdef USE_TENTER_BILLING
	MYSQL		m_dbcatal;			// 게임 카탈로그 디비
#endif

	MYSQL&		m_dbcharingame;		// 게임에서 사용할 캐릭터 DB

	// 메신저 관련
	CNetMsg::SP	m_messengerConnMsg;	// 메신저 연결 메시지

	// 컨넥터 관련
	CNetMsg::SP	m_connectorConnMsg;	// 컨넥터 연결 메시지

	// 헬퍼 접속 메시지
	CNetMsg::SP	m_helperConnMsg;

	CNetMsg::SP	m_subHelperConnMsg;

	//////////
	// 존 관련
	int			m_numZone;			// 존의 개수
	CZone*		m_zones;			// 존 배열
	map_zone_t	m_zones_map;

	//////////////
	// NPC 관련
	CNPCProtoList m_npcProtoList;	// NPC 기본 리스트

	////////////
	// 스킬 관련
	CMagicProtoList		m_magicProtoList;
	CSkillProtoList		m_skillProtoList;

	//////////////
	// Quest 관련
	CQuestProtoList m_questProtoList;	// Quest Proto List

	//////////////
	// Option 관련
	COptionProtoList m_optionProtoList;	// Option Proto List
	CRareOptionProtoList m_rareOptionList; // 레어 옵션

	/////////////////////
	// Special Skill 관련
	CSSkillProtoList m_sSkillProtoList;	// Special Skill Proto List
	/////////////
	// 길드리스트
	CGuildList		m_guildlist;

	CAPetProtoList	m_pApetlist;

	CLuckyDrawBox m_LuckyDrawBox;

	CMonsterMercenary m_MonsterMercenary;
	//////////////
	// 이벤트
	// 050120 : 경험치, 나스, SP, 생산품 2배 이벤트
	bool		m_bDoubleEvent;
#ifdef NEW_DOUBLE_GM_ZONE
	int		m_bDoubleEventZone;
#endif // NEW_DOUBLE_GM_ZONE

#ifdef EVENT_DROPITEM
	bool	m_bDropItem;
	int		m_bDropItemNpcIdx;
	int		m_bDropItemIdx;
	int		m_bDropProb;
#endif // EVENT_DROPITEM

#ifdef DOUBLE_PET_EXP_AUTO
	time_t		m_tPetExpEventStart;
	time_t		m_tPetExpEventEnd;
#endif
#ifdef UPGRADE_EVENT_AUTO
	time_t		m_tUpgradeEventStart;
	time_t		m_tUpgradeEventEnd;
#endif
	int			m_bDoubleNasPercent;
	int			m_bDoubleNasGetPercent;
	int			m_bDoubleExpPercent;
	int			m_bDoubleSpPercent;
	int			m_bDoubleProducePercent;
	int			m_bDoubleProduceNum;
#ifdef NEW_DOUBLE_GM_AUTO
	int			m_bDoubleNasPercentBackup;
	int			m_bDoubleNasGetPercentBackup;
	int			m_bDoubleExpPercentBackup;
	int			m_bDoubleSpPercentBackup;
	int			m_bDoubleProducePercentBackup;
	int			m_bDoubleProduceNumBackup;
	int			m_iDoubleGMStart[6];
	int			m_iDoubleGMEnd[6];
#endif // NEW_DOUBLE_GM_AUTO
#ifdef NEW_DOUBLE_EVENT_AUTO_TIME
	int			m_iDoubleEventStart[6];
	int			m_iDoubleEventEnd[6];
#endif // NEW_DOUBLE_EVENT_AUTO_TIME
#ifdef EVENT_ITEMDROP_AUTO
	int			m_iEventItemdropStart[6];
	int			m_iEventItemdropEnd[6];
#endif // EVENT_ITEMDROP_AUTO
	int			m_CollectBugEventPercent;

	bool		m_bDoubleExpEvent;
	int			m_expPercent;
	bool		m_bLattoEvent;
	bool		m_bNewYearEvent;
	bool		m_bValentineEvent;
	bool		m_bWhiteDayEvent;

	bool		m_bpSiteGive;
	int			m_bpSiteCount;

	// 아이템, 나스 드롭율 조정 변수
	int			m_itemDropProb;
	int			m_moneyDropProb;
//0704
	int			m_aNotice[5]; //이벤트에 관한 공지.
	void		addNotice(int _pos, int _index);

	int			m_nMoonStoneNas;

	int			m_nItemDropEventRate;
#ifdef EVENT_ITEMDROP_AUTO
	int			m_nItemDropEventRateBackup;
#endif

	bool		m_bEventOX;					// OX 퀴즈 활성화, start에서 true가 되고, end에서 false
	time_t		m_timeEventOXStart;			// OX 퀴즈 이벤트 시작 시간
	// m_bEventOX가 true이고 m_timeEventOXStart 이전이면 입장 가능
	map_listOXQuiz_t	m_listOXQuiz;		// 퀴즈 리스트
	int			m_nOXQuizSeqNo;				// 퀴즈 순차 번호, start에서 1로 되고 매 문제마다 1씩 증가한다
	bool		m_bOXQuizNoticeStart;		// 퀴즈 시작 알림 여부, start에서 false되고 시작 알리고 나서 true 됨
	time_t		m_timeOXQuizLimit;			// 퀴즈 제한 시간
	int			m_nOXQuizRealNo;			// 현재 진행중인 퀴즈 번호, 없으면 -1
	int			m_nOXQuizGMCharIndex;		// 현재 퀴즈를 낸 GM

	bool		m_bRecommend;				// 추천 서버 설정 여부
	int			m_pulseRecommendMoonstone;	// 추천 서버 문스톤 지급 시간

	bool		m_bNonPK;

#ifdef FREE_PK_SYSTEM
	bool		m_bFreePk;
#endif

#ifdef MAL_DISABLE_PKPENALTY
	bool		m_bDisablePKPaenalty;
#endif // MAL_DISABLE_PKPENALTY

	// 길드 랭킹용 경험치 업데이트 주기
	int			m_nPulseSaveGuildPoint;

#ifdef  DOUBLE_ITEM_DROP
	bool		m_bDoubleItemEvent;
	int			m_bDoubleItemPercent;

#endif // DOUBLE_ITEM_DROP

#ifdef DOUBLE_PET_EXP
	bool		m_bDoublePetExpEvent;
	int			m_PetExpPercent;
#endif // DOUBLE_PET_EXP

#ifdef DOUBLE_ATTACK
	bool		m_bDoubleAttackEvent;
	int			m_AttackPercent;
#endif // DOUBLE_ATTACK

#ifdef NEW_DOUBLE_EVENT_AUTO
	bool		m_bDoubleEventAuto;
	bool		m_bDoubleEventAutoOn;
#endif // NEW_DOUBLE_EVENT_AUTO

	int m_nDVDCharAttackRate;
	int m_nDVDCharDefanseRate;
	int m_nDVDCharMoveSpeedRate;

	unsigned int m_unEventXmasTreePoint;

	int m_nExpLimit;
	int m_nSpLimit;
#ifdef GM_EXP_LIMIT_AUTO
	int m_nExpLimitStart;
	int m_nExpLimitEnd;
	int m_nSpLimitStart;
	int m_nSpLimitEnd;
	int m_nGMExpLimitStart[6];
	int m_nGMExpLimitEnd[6];
	bool m_bExpLimit;
#endif // GM_EXP_LIMIT_AUTO

	CGoldenBallEventData	m_clGoldenBall;

	struct tm m_tRealSystemTime;

	bool m_bTimeTable[24];			// 몇시에 시작하는지 설정해주는 time flag
	int* m_iZoneExp;				// 어떤존에 경험치 % 설정.
	int m_iTimeInterval;			// 몇시간동안 던전타임인지 설정.
	int m_iStartTime;				// 현재시간으로부터 가장 가까운 시작시간
	int m_iEndTime;					// 현재시간으로부터 가장 가까운 종료시간
	bool m_bDungeonTimeToggle;		// 운영자 ON/OFF TOGGLE
	bool m_bIsTime;					// 자동으로 진행될 때 현재 던전타임인지 확인.
	int m_iZoneCount;

	CGrobalEcho		gecho;
	std::map<int, CNPC*>		m_npc_ctTime;
	std::vector<int>			m_npc_ctTime_erase;

	void NpcCtTimeCount();

	bool DropProbLoad();
	bool m_bCashShopLock;

	virtual void operate(rnSocketIOService* service);

	/////////////////
	// 서버 환경 관련
	char* GetServerPath();			// 경로 얻기
	bool LoadSettingFile();			// 설정 파일 읽기
	bool LoadSettings();			// 설정 읽기
	bool ConnectDB();				// DB 연결
	void DisconnectDB(bool bchar);	// DB 연결 끊기, bchar가 false이면 user와 data, true이면 char
	bool InitGame();				// 게임 초기화
	int GetMaxPlayers();			// 허용 가능 최대 플레이어 수 구하기
	CZone* FindZone(int zone);			// 존 찾기
	// zone의 x, z의 위치에서 가까운 마을 찾기
	CZone* FindNearestZone(int zone, float x, float z, int* nearZone, int* nearZonePos, int syndicateType = 0);

#ifdef XTRAP
	unsigned char	m_XTrapMap[CSFILE_NUM][XTRAP_CS4_BUFSIZE_MAP];
	bool			InitXTrap();
#endif //XTRAP

	void SetHeaders();						// 빌링/메신저 헤더 생성

	////////////
	// 빌링 관련
	void ProcessBilling(CNetMsg::SP& msg);		// 빌링 처리

	//////////////
	// 메신저 관련
	void ProcessMessengerMessage(CNetMsg::SP& msg);	// 메신저 메시지 처리
	void ProcessEventMessage(CNetMsg::SP& msg);		// 이벤트 메시지 처리

	//////////////
	// 헬퍼 관련
	void ProcessHelperMessage(CNetMsg::SP& msg);	// 헬퍼 메시지 처리

	///////////////
	// 서브 핼퍼 ( 거래대행등 )
	void ProcessSubHelperMessage( CNetMsg::SP& msg );

	/////////////////
	// 서버 수행 관련
	void HeartBeat();		// 1초마다 수행되는 함수

	void CloseSocket(CDescriptor* d);		// 디스크립터 종료, bForce: 15초 딜레이 무시 여부

	/////////////////
	// 명령 처리 관련
	void CommandInterpreter(CDescriptor* d, CNetMsg::SP& msg); // 명렁 처리

	//////////////
	// 캐릭터 관련
	void CharPrePlay(CDescriptor* d);			// 캐릭터 준비
	void DecreaseTimeForPC(CPC* ch);			// 캐릭터 관련 시간 감소

	void MoonStoneEndProcess(CPC* ch);

	// 세액 증가 : 판매/구매
	void AddTaxItem(LONGLONG val)
	{
		if (val > 0 && m_taxItem + val > 0) m_taxItem += val;
	}
	// 세액 증가 : 생산
	void AddTaxProduceCastle(LONGLONG val)
	{
		if (val > 0 && m_taxProduceCastle + val > 0) m_taxProduceCastle += val;
	}
	// 세액 초기화
	void ResetTax()
	{
		m_taxItem = m_taxProduceCastle = 0;
	}
	void ResetTaxDratan()
	{
		m_taxItemDratan = m_taxProduceCastleDratan = 0;
	}
	// 세액 증가 : 판매/구매
	void AddTaxItemDratan(LONGLONG val)
	{
		if (val > 0 && m_taxItemDratan + val > 0) m_taxItemDratan += val;
	}
	// 세액 증가 : 생산
	void AddTaxProduceCastleDratan(LONGLONG val)
	{
		if (val > 0 && m_taxProduceCastleDratan + val > 0) m_taxProduceCastleDratan += val;
	}
	// 세액 -> 성정보
	void SaveTax();
	// 세금 -> 길드
	void DivideTax();
	// 세금 받을 길드 변경
	void ChangeTaxGuild();

	// 파티 리스트 노드 찾기 : 보스 인덱스 사용
	CParty* FindPartyByBossIndex(int nBossIndex);

	// 파티 리스트 노드 찾기 : 멤버 인덱스 사용
	CParty* FindPartyByMemberIndex(int nMemberIndex, bool bIncludeRequest);

	// 파티 리스트 노드 찾기 : 보스 인덱스 사용
	CExpedition* FindExpedByBossIndex(int nBossIndex);

	// 파티 리스트 노드 찾기 : 멤버 인덱스 사용
	CExpedition* FindExpedByMemberIndex(int nMemberIndex, bool bIncludeRequest);

	CPartyMatchMember* FindPartyMatchMemberByCharIndex(int nCharIndex);
	CPartyMatchParty* FindPartyMatchPartyByBossIndex(int nBossIndex);

	// OX Quiz Loading
	bool LoadOXQuiz();

	CMoonStoneReward m_moonstoneReward;

	// 상점 거래등 세금 자료 통계
	void ProcStatistics();

private:

	// 세금 처리
	void ProcTax();

	// 몹, 아이템 컨트롤
	void ProcMobItemControl();

	// 날씨 변화
	void ChangeWeather();

	// loop count 처리
	void ProcLoopCount();

	// OX 퀴즈 처리
	void ProcOXQuiz();

	// 추천서버 문스톤 처리
	void ProcRecommendServerMoonstone();

	// 골든볼 종료 처리
	void ProcGoldenBall();

public:

	void ModifyDungeonTime(int flag);
	void CheckDungeonTime();
	void LoadDungeonTime(int count);
	void SaveDungeonTime(int count);
	void SetDungeonTime();
	void ProcMonsterCombo();
	CZone* m_comboZone;
	CMIssionCaseList m_missionCaseList;

#ifdef NEW_DOUBLE_EVENT_AUTO
	void NewDoubleEventAuto();
#endif // NEW_DOUBLE_EVENT_AUTO

#ifdef EXTREME_CUBE
	CExtremeCube m_extremeCube;
#endif // EXTREME_CUBE

	void ProcEndExped();		//원정대 해제 처리

	void ProcEndParty();		//파티 해제 처리

	CTrigger_List	m_CTriggerDBInfo;						// DB에서 받은 proto data
	int		GetTriggerSetTypeToZoneIndex(int zoneIndex);	// zone index에 따라 트리거세트를 구별하는 함수
	void	ProcTrigger();									// 매초 필요한 트리거 체크

	CAffinityProtoList	m_affinityProtoList;

	CTitleProtoList m_titleProtoList;

#ifdef LACARETTE_SYSTEM
	CLacarette m_lacarette;
#endif

#ifdef DEV_EVENT_PROMOTION2
	CPromotion2 m_promotion2;
#endif

	void CheckMobActivityAndAkanProc(CZone& Zone, CArea& Area);

#ifdef NEW_DOUBLE_GM_AUTO
	bool m_bIsDEtime;
	void SwapDoubleEventValue();
#endif
#ifdef EVENT_ITEMDROP_AUTO
	bool m_bIsItemdrop;
//	void CServer::SwapEventItemdropValue();
#endif
	void CheckEventTime();
	CRaidInfo m_RaidInfo;

#ifdef MEREC_CASTLE_TEST_GUILD_LEVEL_GM_SETTING
	int MeracCastleDebugGuildLevel;
#endif

	int GetCastllanZoneZum( int charIndex );

	void CheckRaidReset();
	void ResetRaid();
	time_t m_tRaidResetProcess;
	int m_nProcess;
	bool m_bCanEnterTheRaidDungeon;
	bool m_bIsCheckTime;
	int m_nResetDay;
	int m_nResethr;
	int m_nResetmin;

	vec_listNpcDropItemAtTime_t		m_listNpcDropItemAtTime;
	void			ItemDropAtTime();
	bool			m_bApplySkillToNPC;

public:
	CRoyalRumble m_RoyalRumble;

	Fortune_proto_list m_Fortunelist;
#ifdef SYSTEM_TREASURE_MAP
	bool bTreasureMapLinkZoneDrop;
	void writeTreasureMapSaveFile();
	void readTreasureMapSaveFile();
#endif

#ifdef REFORM_PK_PENALTY_201108 // 정시에 한번 성향 수지 장치 버프가 있는 플레이어에게 pk 성향 포인트 보상 보상해줘야한다.
	void CheckPkDisPosiotionPointReward();
#endif

	bool m_dratanCastleEnvCheck[3];
	void CheckDratanCastleDungeon(int num);
	void CheckDratanCastleDungeonReward();

#ifdef IMP_SPEED_SERVER
	bool	m_bSpeedServer;
#endif //IMP_SPEED_SERVER

#if defined (INTERGRATION_SERVER)
public:
	CIntergrationServer m_intergrationInfo;
#endif

	CRewardManager m_rewardMgr;

	CDropManager			m_dropItemMgr;
	CEventAutomationManager m_eventAutomationMgr;

	CEventInfo* getEventInfo(int _eventIndex)
	{
		return m_eventAutomationMgr.getEventInfo(_eventIndex);
	}
	CEventInfo* getActiveEventInfo(int _eventIndex)
	{
		return m_eventAutomationMgr.getActiveEventInfo(_eventIndex);
	}
	bool addActiveEvent(int _eventIndex, bool _dbUpdate=false)
	{
		return m_eventAutomationMgr.addActiveEvent(_eventIndex, _dbUpdate);
	}
	bool removeActiveEvent(int _eventIndex, bool _dbUpdate=false);
	bool isActiveEvent(int _eventIndex)
	{
		return m_eventAutomationMgr.isActiveEvent(_eventIndex);
	}
	std::map<int, CEventInfo*> * getActiveEvenList()
	{
		return m_eventAutomationMgr.getActiveEvenList();
	}
	std::map<int, CEventInfo> * getEvenList()
	{
		return m_eventAutomationMgr.getEvenList();
	}
	bool isActiveEventNpc(int _npcDBIndex, int & _eventIndex)
	{
		return m_eventAutomationMgr.isActiveEventNpc( _npcDBIndex, _eventIndex);
	}
	bool isEventNpc(int _npcDBIndex, int & _eventIndex, bool & _bEnable )
	{
		return m_eventAutomationMgr.isEventNpc(_npcDBIndex, _eventIndex, _bEnable );
	}
	void addNpcRegenInfo(CNPCRegenInfo* _pNpcRegenInfo)
	{
		m_eventAutomationMgr.addNpcRegenInfo(_pNpcRegenInfo);
	}
	void doEventDropItem(CNPC* npc, CPC* pc, CPC* tpc);

	bool GetOutDratanDungeon(CPC* pc);
public:
	void PingDB(void);		// DB Ping
private:
	int m_mobAISeq;
	int m_mobAIArea[PASSES_PER_SEC];
	int m_mobAIMobCnt[PASSES_PER_SEC];
public:
	void ClearMobAISeq(void);
	int AddMobAISeq(int regenCnt);
	void RemoveMobAISeq(int minseq, int regenCnt);

	CRockPaperScissorsGame* m_XmasRockPaperScissorsGame;
	CPuzzleGame* m_XmasPuzzleGame;

	int m_DratanCheckTime;

public:
	bool isRunMessenger()
	{
		return m_messenger->isRun();
	}
	bool isRunConnector()
	{
		return m_connector->isRun();
	}
	bool isRunHelper()
	{
		return m_helper->isRun();
	}
	bool isRunSubHelper()
	{
		return m_subHelper->isRun();
	}

	std::set<CPC*> m_kailuxUser;
	std::set<CPC*> m_dealermoonUser;
#ifdef TLD_EVENT_SONG
public:
	bool	tld_event;
	void	checkTldEvent();
#endif
	// My methods and objects
public:
	AutoBuffSystem m_buffSystem;
};

extern CServer* gserver;

extern	bool    g_bUpgradeEvent;
extern	int		g_nUpgradeProb;

#endif
//
