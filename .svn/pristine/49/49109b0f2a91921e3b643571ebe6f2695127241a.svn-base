#ifndef __WAR_GROUND_H__
#define __WAR_GROUND_H__

class CWGPlayer;
class CWGPlayerList;
class CWaitPlayer;
class CWaitPlayerList;
class CRoyalRumble;

class CWGPlayer
{
private:
	int				m_nWarType;						// 38: Royal Rumble
	int				m_nCharIndex;
	int				m_nLevelType;
	int				m_nSelected;					// 0: 아무것도 아닌 상태, 1: 선택, 2: 탈락, 3: 경기 draw
	CWGPlayer*		m_next;
public:
	CWGPlayer();
	CWGPlayer(int wartype, int charindex, int leveltype, int select = 0);
	virtual ~CWGPlayer();

	void			SetCharIndex(int index);
	void			SetLevelType(int type);
	void			SetSelected(int select);
	void			SetNext(CWGPlayer* node);
	void			SetWarType(int type);
	int				GetCharIndex();
	int				GetLevelType();
	int				GetSelected();
	CWGPlayer*		GetNext();
	int				GetWarType();
};

class CWGPlayerList
{
private:
	CWGPlayer*		m_head;
	int				m_nTotalCount;
	int				m_nRookieCount;
	int				m_nSeniorCount;
	int				m_nMasterCount;

public:
	CWGPlayerList();
	virtual ~CWGPlayerList();

	void			AddTotalCount(int val);
	void			AddRookieCount(int val);
	void			AddSeniorCount(int val);
	void			AddMasterCount(int val);

	int				GetTotalCount();
	int				GetRookieCount();
	int				GetSeniorCount();
	int				GetMasterCount();

	void			AddNode(CWGPlayer* node);
	void			DelNode(int charindex);
	bool			FindNode(int charindex);
	CWGPlayer*		GetNode(int charindex);
	CWGPlayer*		GetHead();
	bool			Select();
	void			ClearList();
	void			SendResult();
	void			SetHead(CWGPlayer* node);
	CWGPlayer*		GetTail();
	void			ShuffleWGPlayer(int nShuffleCount);
};

class CWaitPlayer
{
private:
	bool			m_bCheckIn;
	int				m_nWGType;
	int				m_nLevelType;
	char			m_cCharType;
	int				m_nCharIndex;
	bool			m_bIsEntered;
	int				m_nTeam;
	CWaitPlayer*	m_next;
	bool			m_bHold;

public:
	CWaitPlayer();
	CWaitPlayer(int WGType, int leveltype, char chartype, int charindex, bool entered, int team);
	virtual ~CWaitPlayer();

	bool			GetCheckIn();
	int				GetWGType();
	int				GetLevelType();
	char			GetCharType();
	int				GetCharIndex();
	bool			GetIsEntered();
	int				GetTeam();
	CWaitPlayer*	GetNext();

	void			SetCheckIn(bool checkin);
	void			SetWGType(int type);
	void			SetLevelType(int type);
	void			SetCharType(char type);
	void			SetCharIndex(int index);
	void			SetEntered(bool boolean);
	void			SetTeam(int team);
	void			SetNext(CWaitPlayer* node);
	void			SetHold(bool b);
	bool			GetHold();
	void			UnHold();

};

class CWaitPlayerList
{
private:
	CWaitPlayer*	m_head;
	int				m_nPlayerCount;
	int				m_nRookieCount;
	int				m_nSeniorCount;
	int				m_nMasterCount;

public:
	CWaitPlayerList();
	virtual ~CWaitPlayerList();

	int				GetPlayerCount();
	void			AddNode(CWaitPlayer* node);
	CWaitPlayer*	GetNode(int charindex);
	void			DelNode(int charindex);
	bool			FindNode(int charindex);
	CWaitPlayer*	GetHead();
	CWaitPlayer*	GetTail();
	void			SetHead(CWaitPlayer* node);
	void			ClearList();
	void			AddRookieCount(int n);
	void			AddSeniorCount(int n);
	void			AddMasterCount(int n);
	int				GetRookieCount();
	int				GetSeniorCount();
	int				GetMasterCount();
	void			SendAllWaitPlayer(CNetMsg::SP& msg);
	void			ShuffleWaitPlayer(int nShuffleCount);
};

class CRoyalRumble
{
private:
	/*time_t			m_tRoyalRumbleNoticeTime;*/
	time_t			m_tRoyalRumbleTime;
	time_t			m_tRookieTime;
	time_t			m_tSeniorTime;
	time_t			m_tMasterTime;
	int				m_nRoyalRumbleProcess;
	bool			m_bRoyalRumbleNotice;
	bool			m_bIsRookiePlaying;
	bool			m_bIsSeniorPlaying;
	bool			m_bIsMasterPlaying;
	bool			m_bRookieLastEnter;
	bool			m_bSeniorLastEnter;
	bool			m_bMasterLastEnter;
	int				m_nRookieWinnerCharIndex;
	int				m_nSeniorWinnerCharIndex;
	int				m_nMasterWinnerCharIndex;
	int				m_nWinnerIsRewarded;		// (1+2+4) 1:rookie보상받음, 2:senior 보상받음, 4:master 보상받음, 3:rookie senior 보상받음, 6:senior master 보상받음
	/*int				m_nTimeNoticeType;*/
	int				m_nMinPlayerCount;
	int				m_nMaxPlayerCount;
	int				m_nStartHour;
	void			loadRoyalRumbleSetting();
public:
	bool			m_bCondition;
	CWGPlayerList	m_WGPlayerList;
	CWaitPlayerList	m_WaitPlayerList;
	int				m_RoyalRumbleTime[ROYAL_RUMBLE_SCHEDULE];

	CZone*			m_pRoyalRumbleZone;
	CArea*			m_pRookieArea;
	CArea*			m_pSeniorArea;
	CArea*			m_pMasterArea;
//	CPC*			m_CheckInRookiePlayer[MAX_ROYAL_RUMBLE_PLAYER];
//	CPC*			m_CheckInSeniorPlayer[MAX_ROYAL_RUMBLE_PLAYER];
//	CPC*			m_CheckInMasterPlayer[MAX_ROYAL_RUMBLE_PLAYER];

	CRoyalRumble();
	virtual ~CRoyalRumble();

	void			Init();
	void			CheckRoyalRumble();				// 경기시작 전 타임라인 콘트롤
	void			CheckRoyalRumbleArea();			// 경기시작 후 타임라인 콘트롤
	void			SetRoyalRumbleTime(time_t time);
	void			AddRoyalRumbleTime(time_t time);
	time_t			GetRoyalRumbleTime();
	void			SetRoyalRumbleNextProcess();
	void			SetInitRoyalRumbleProcess();
	int				GetRoyalRumbleProcess();
	void			SetRoyalRumbleNotice(bool b);
	bool			GetRoyalRumbleNotice();
	void			SetRookiePlaying(bool b);
	void			SetSeniorPlaying(bool b);
	void			SetMasterPlaying(bool b);
	bool			GetRookiePlaying();
	bool			GetSeniorPlaying();
	bool			GetMasterPlaying();
	void			MakeWaitPlayer();
	void			SetInitRookieTime();
	void			SetInitSeniorTime();
	void			SetInitMasterTime();
	void			AddRookieTime(time_t t);
	void			AddSeniorTime(time_t t);
	void			AddMasterTime(time_t t);
	time_t			GetRookieTime();
	time_t			GetSeniorTime();
	time_t			GetMasterTime();
	void			SetRookieLastEnter(bool b);
	void			SetSeniorLastEnter(bool b);
	void			SetMasterLastEnter(bool b);
	bool			GetRookieLastEnter();
	bool			GetSeniorLastEnter();
	bool			GetMasterLastEnter();
	void			EnterTheWarGround(int leveltype);
	int				GetRemainPlayer(int leveltype);
	int				GetEnteredPlayer(int leveltype);
	void			SetDebuff(int debufftype, int leveltype);
	void			UnHold();
	int				GetWinner(int LevelType);
	void			SetWinner(int LevelType, CPC* pc);
	void			SetRewarded(int LevelType, CPC* pc);
	bool			GetRewarded(int LevelType);
	void			InitRewarded(int LevelType);
	/*void			SetTimeNoticeType(int type);*/
	/*int				GetTimeNoticeType();*/
	//void			SetTimeNotice(time_t t);
	//time_t			GetTimeNotice();
	int				GetRemainWaitTime();
	void			setMinPlayerCount(const int val);
	void			setMaxPlayerCount(const int val);
	void			setStartHour(const int val);
	int				getMinPlayerCount() const;
	int				getMaxPlayerCount() const;
	int				getStartHour() const;
	static int checkSettingValue(const int &min, const int &max, const int &startHour);
	enum
	{
		kValueIsValid = 0,
		kMinValueIsInvalid = 1,
		kMaxValueIsInvalid = 2,
		kStartHourValueIsInvalid = 3
	};
};
#endif // __WAR_GROUND_H__
//
