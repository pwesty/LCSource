#ifndef SE_INCL_NETWORK_H
#define SE_INCL_NETWORK_H
#ifdef PRAGMA_ONCE
	#pragma once
#endif

#include <Engine/Base/Stream.h>
#include <Engine/World/World.h>
#include <Engine/Templates/StaticArray.h>
#include <Engine/GlobalDefinition.h>
#include <Engine/Network/ChatMsgBuffer.h>
#include <Engine/Network/MultiTarget.h>
#include <Engine/Network/MessageDispatcher.h>
#include <Engine/Network/MessageDefine.h>

// EDIT : BS : 070413 : 신규 패킷 암호화
// EDIT : BS : 패킷 암호화
#ifdef CRYPT_NET_MSG
#include <LCCrypt.h>
#else
#include <Engine/Network/CryptNetMsg.h>
#endif

// 110802 : 패킷 2차 암호화
#ifdef	CRYPT_NET_MSG_LEVEL2
#include <Engine/Network/CryptNetMsg.h>
#endif

// 공성 참여 상태(status, appear )
#define WCJF_NONE   0  // 비참여
#define WCJF_OWNER   1  // 성주 길드
#define WCJF_DEFENSE_GUILD 2  // 수성 길드
#define WCJF_DEFENSE_CHAR 3  // 수성측 용병
#define WCJF_ATTACK_GUILD 4  // 공성 길드
#define WCJF_ATTACK_CHAR 5  // 공성측 용병


#include <Engine/Entities/StatusEffect.h>
#include <Engine/Entities/ItemEffect.h>
#include <Engine/Entities/CharacterAnimation.h>
//0603 kwon 
#include <Engine/Entities/Items.h>
#include <Engine/Entities/TargetInfo.h>
#include <Engine/Entities/SmcParser.h> // eons 071005 : SmcParser 클래스

// [6/17/2009 rumist] include definition stat.
#include <Engine/LocalDefine.h> 
#include <Engine/Entities/AffinityData.h>
#include <Engine/Entities/Lacarette.h>
#include <Engine/Interface/UIGuildMark.h>
#include <Common/Packet/ptype_inventory.h>

#define NET_MAXGAMECOMPUTERS SERVER_CLIENTS		// max overall computers in game
#define NET_MAXGAMEPLAYERS			1			// max overall players in game
#define NET_MAXLOCALPLAYERS			1			// max players on a single machine

#define NET_WAITMESSAGE_DELAY		50			// wait time between receive message attempts

#define NET_MAXSESSIONPROPERTIES	2048		// size of buffer for custom use by CGame and entities

// demo synchronization constants
#define DEMOSYNC_REALTIME			(0.0f)
#define DEMOSYNC_STOP				(-1.0f)

//[ttos_2009_1_20]: 채팅 금지 flag
#define CHAT_FLAG_NO_SAY  (1 << 0)  // 일반 채팅 금지
#define CHAT_FLAG_NO_PARTY  (1 << 1)  // 파티 채팅 금지
#define CHAT_FLAG_NO_GUILD  (1 << 2)  // 길드 채팅 금지
#define CHAT_FLAG_NO_TRADE  (1 << 3)  // 매매 채팅 금지
#define CHAT_FLAG_NO_WHISPER (1 << 4)  // 귓속말 금지
#define CHAT_FLAG_NO_SHOUT  (1 << 5)  // 외치기 금지

#define PK_SYS_CHAO_1	(1 << 0)	//완전 나쁜놈
#define PK_SYS_CHAO_2	(1 << 1)	//좀 덜 나쁜놈
#define PK_SYS_CHAO_3	(1 << 2)	//별로 안나쁜놈
#define PK_SYS_CHAO_4	(1 << 3)	//별로 안착한놈
#define PK_SYS_CHAO_5	(1 << 4)	//좀 덜 착한놈
#define PK_SYS_CHAO_6	(1 << 5)	//완전 착한놈
 
extern ENGINE_API INDEX net_bLocalPrediction;

enum NetGraphEntryType {
	NGET_ACTION = 0,
	NGET_NONACTION,
	NGET_MISSING,
	NGET_SKIPPEDACTION,
	NGET_REPLICATEDACTION,
};

enum CurEnterType { // 현재 게임 입장 타입(진행 되고 있는 모드?)
	CURRENT_ENTER_NORMAL,
	CURRENT_ENTER_PARTYCUBE,
	CURRENT_ENTER_GUILDCUBE,
};

struct NetGraphEntry {
	enum NetGraphEntryType nge_ngetType;	// type of packet/event
	FLOAT nge_fLatency;						// latency in seconds
	void Clear(void);
};

struct JewelInfo
{
	int Jewel_idx;
	int Jewel_composGrade;
};

typedef		std::vector<JewelInfo>		_vecJewelInfo;
//added by sam 11/02/01 [SAM] 한벌 
typedef std::vector<INDEX> VEC_ONESUIT_OPTION_TYPE;
typedef VEC_ONESUIT_OPTION_TYPE::iterator VEC_ONESUIT_OPTION_TYPE_IT;

typedef std::vector<INDEX> VEC_ONESUIT_OPTION_LEVEL;
typedef VEC_ONESUIT_OPTION_LEVEL::iterator VEC_ONESUIT_OPTION_LEVEL_IT;

#define NONCE_SIZE 8

struct CNeedItems;

/*
 * Network session description.
 */
class ENGINE_API CNetworkSession {
public:
	CListNode ns_lnNode;				// for linking in list of available sessions
	CTString ns_strAddress;				// session address

	CTString ns_strSession;				// session name
	CTString ns_strWorld;				// world name
	TIME     ns_tmPing;					// current players
	INDEX    ns_ctPlayers;				// current players
	INDEX    ns_ctMaxPlayers;			// max number of players
	CTString ns_strGameType;			// game type
	CTString ns_strMod;					// active mod
	CTString ns_strVer;					// version
	TIME     ns_tmLastSeen;				// time of the last rewponse to enumeration
	ULONG    ns_ulAddress;				// server's address
	UWORD    ns_uwPort;					// server's port
	
public:
	void Copy(const CNetworkSession &nsOriginal);

	/* Default constructor. */
	CNetworkSession(void);
	/* Construct a session for connecting to certain server. */
	CNetworkSession(const CTString &strAddress);
};

/*
 * Game, used for general game initialization/manipulation
 */
class ENGINE_API CNetworkLibrary : public CMessageDispatcher 
{	
public:
	// EDIT : BS : BEGIN
	DWORD m_tickSendMoveList;
	// EDIT : BS : END
	BOOL ga_IsServer;								// set if this is a server computer
	CServer &ga_srvServer;							// server (active only if this is server computer)

	CTCriticalSection ga_csNetwork;					// critical section for access to network data

	CSessionState &ga_sesSessionState;				// local session state
	CStaticArray<CPlayerSource> ga_aplsPlayers;		// local players
	CTString ga_strSessionName;						// name of current session
	CTString ga_strServerAddress;					// address of game server (if joined)
	CTFileName ga_fnmWorld;							// filename of current world
	CTString ga_strGameType;						// game type (cooperative, scorematch, deathmatch)
	CTString ga_strMod;								// name of the mod
	CTString ga_strVer;								// version

	UBYTE *ga_pubDefaultState;						// default state for connecting (server only)
	SLONG ga_slDefaultStateSize;
	UBYTE ga_aubDefaultProperties[NET_MAXSESSIONPROPERTIES];
	UBYTE *ga_pubCRCList;							// list of files for CRC checking (server only)
	SLONG ga_slCRCList;
	ULONG ga_ulCRC;									// CRC of CRCs of all files in the list

	BOOL ga_bLocalPause;							// local pause for single player/demo
	BOOL ga_bDemoRec;								// set if currently recording a demo
	CTFileStream ga_strmDemoRec;					// currently recorded demo file
	BOOL ga_bDemoPlay;								// set if currently playing a demo
	BOOL ga_bDemoPlayFinished;						// set if currently playing demo has finished
	CTFileStream ga_strmDemoPlay;					// currently played demo file
	CTimerValue ga_tvDemoTimerLastTime;				// real time timer for demo synchronization
	CTimerValue ga_tvGameFinished;					// time when the game was finished
	TIME ga_tmNextDemoTick;							// time of the next tick to be read from stream for demo play

	CTFileName ga_fnmNextLevel;						// world for next level
	BOOL  ga_bNextRemember;							// remember old levels when changing to new one
	INDEX ga_iNextLevelUserData;					// user data for next level
	UBYTE ga_ubNumLevelChanges;						// how many times has levelchange occured

	CListHead ga_lhEnumeratedSessions;				// list of sessions found after last enumeration was triggered
	FLOAT ga_fEnumerationProgress;					// enumeration progress percentage (0-1)
	CTString ga_strEnumerationStatus;				// description of current operation
	BOOL ga_bEnumerationChange;						// this is raised if something was changed in the list
	BOOL ga_bGuildWar;								// 길드 전쟁 중인가?


	CTString ga_strRequiredMod;						// set if connection failed due to a different mod running on the server

	CTimerValue ga_tvLastMainLoop;					// when was the main loop last run - for tick sync
	BOOL  ga_bFirstMainLoop;						// has the main loop been run before

	// buffer for custom use by CGame and entities
	UBYTE ga_aubProperties[NET_MAXSESSIONPROPERTIES];

	BOOL IsServer(void) { return ga_IsServer;};

	// make actions packet for local players and send to server
	void SendActionsToServer(void);

	/* Loop executed in timer interrupt, every game tick. */
	void TimerLoop(void);

	// really do the level change
	void ChangeLevel_internal(void);
	void ChangeLevel_client_internal_t(void);

	// save current version of engine
	void WriteVersion_t(CTStream &strm);
	// load version of engine saved in file and check against current
	void CheckVersion_t(CTStream &strm, BOOL bAllowReinit, BOOL &bNeedsReinit);

	// add a value to the netgraph
	void AddNetGraphValue(enum NetGraphEntryType nget, FLOAT fLatency);

	// add a value to the bandwidth graph
	void AddBandwidthGraphValue(ULONG ulBandwidth);

	// call this while game is not running - to update server enumeration lists
	void GameInactive(void);

	// automatically adjust network settings
	void AutoAdjustSettings(void);

	// initialize gathering of file CRCs to CRC table
	void InitCRCGather(void);
	// finish gathering of file CRCs to CRC table (call for server only!)
	void FinishCRCGather(void);
	
public:
	CWorld ga_World;										// local copy of world
	FLOAT ga_fDemoTimer;									// timer for demo playback (in seconds)
	FLOAT ga_fDemoRealTimeFactor;							// slow/fast playback factor (for DEMOSYNC_REALTIME only)
	FLOAT ga_fGameRealTimeFactor;							// game time accelerator
	FLOAT ga_fDemoSyncRate;									// demo sync speed in FPS (or realtime/stop)
	CStaticArray<struct NetGraphEntry> ga_angeNetGraph;		// array of netgraph entries
	CStaticArray<ULONG> ga_aulBandwidthGraph;				// array of bandwidth entries

	//0603 kwon 
	CItems MySlotItem[INVEN_SLOT_TAB][ITEM_COUNT_IN_INVENTORY_NORMAL];
	CItems MyWearItem[WEAR_TOTAL];
	CItems MyWearCostItem[WEAR_COSTUME_TOTAL];
		
	//int		MyCurrentSkill[50];
	//int		MySkillNum;
	int						wo_iNumOfSkill;			// yjpark
	int						wo_iNumOfJewelGradeData;

	CStaticArray<JewelComosInfo>	wo_aJewelData;		// 보석 소모 나스 값 데이터 저장
	CStaticArray<CMakeItemData>		wo_aMakeItemData;	// 제작 아이템 데이터
	CSmcParserList					wo_aItemSmcInfo;	// 아이템 Smc 정보
	CStaticArray<CAffinityData>		wo_aAffinityData;
	CStaticArray<CLacarette>		wo_aLacaretteData;	// 라카렛
	
	std::map<int, _vecJewelInfo> wo_mapJewelGradeInfo;
	std::map<int, _vecJewelInfo> wo_mapChaosJewelGradeInfo;
	
	int						wo_iNumOfAffinity;
	int						wo_dwEnemyCount;		// 에너미의 갯수.
	int						wo_dwKilledEnemyCount;
	int						wo_iNomOfMakeItemData;	// 제작 아이템 데이터
	struct CHECKPOINT{								// 해당 체크 포인트 진행 여부
		int m_iQuestIndex;
		int m_iMaxCheckFlag;				
		int m_iCheckFlag;
	} ;
	CHECKPOINT wo_stCheckPoint;

	//0917
	CTString	m_strUserID;
	CTString	m_strUserPW;
	CTString	m_strUserCID; // 접속한 웹사이트 식별자	
	int			m_iServerGroup;
	int			m_iServerCh;
	int			m_iServerType;
	int			m_iNetworkResponse[MSG_END];			// 네트워크 응답 플래그
	int			m_iNetworkResponseEx[MSG_EX_END];
	

	BOOL	m_bIsPvp;
	void	SetPvpMode() { m_bIsPvp = !m_bIsPvp; };
	LONG	m_lLoadingZone;

	BOOL	IsRvrZone()	{ return m_bIsRvr;	}	// 현재 RVR존(42)에 있냐.
	void	SetRvrZone(BOOL bRvr)	{ m_bIsRvr = bRvr;	}

	// interface
	/* Default constructor. */
	CNetworkLibrary(void);
	~CNetworkLibrary(void);
	DECLARE_NOCOPYING(CNetworkLibrary);

	/* Initialize game management. */
	void Init(const CTString &strGameID);
	/* Start a peer-to-peer game session. */
	void StartPeerToPeer_t(const CTString &strSessionName,
		const CTFileName &fnmWorld, ULONG ulSpawnFlags, 
		INDEX ctMaxPlayers, BOOL bWaitAllPlayers,
		void *pvSessionProperties); // throw char *

	/* Start numeration of existing sessions. */
	void EnumSessionsStart(BOOL bInternet);
	/* Continue numeration of existing sessions. */
	void EnumSessionsContinue();
	/* Stop numeration of existing sessions. */
	void EnumSessionsStop();

	/* Join a running multi-player game. */
	void JoinSession_t(const CNetworkSession &nsSesssion, INDEX ctLocalPlayers); // throw char *
	/* Start playing a demo. */
	//void StartDemoPlay_t(const CTFileName &fnDemo); // throw char *
	/* Test if currently playing a demo. */
	BOOL IsPlayingDemo(void);
	/* Test if currently recording a demo. */
	//BOOL IsRecordingDemo(void);
	/* Test if currently playing demo has finished. */
	//BOOL IsDemoPlayFinished(void);
	/* Stop currently running game. */
	void StopGame(void);


	// pause/unpause game
	void TogglePause(void);
	// test if game is paused
	BOOL IsPaused(void);
	// test if game is waiting for more players to connect
	BOOL IsWaitingForPlayers(void);
	// test if game is waiting for server
	BOOL IsWaitingForServer(void);
	// mark that the game has finished -- called from AI
	void SetGameFinished(void);
	BOOL IsGameFinished(void);
	// manipulation with realtime factor for slower/faster time -- called from AI
	void SetRealTimeFactor(FLOAT fSpeed);
	FLOAT GetRealTimeFactor(void);   
	// test if having connnection problems (not getting messages from server regulary)
	BOOL IsConnectionStable(void);
	// test if completely disconnected and why
	BOOL IsDisconnected(void);
	const CTString &WhyDisconnected(void);

	// set/get server side pause (for single player or demo only)
	void SetLocalPause(BOOL bPause);
	BOOL GetLocalPause(void);

	// get server/client name and address
	void GetHostName(CTString &strName, CTString &strAddress);

	// test if playing in network or locally
	BOOL IsNetworkEnabled(void);

	// initiate level change
	void ChangeLevel(const CTFileName &fnmNextLevel, BOOL bRemember, INDEX iUserData);

	/* Obtain file name of world that is currently loaded. */
	CTFileName &GetCurrentWorld(void) { return ga_fnmWorld;};

	/* Start recording a demo. */
	//void StartDemoRec_t(const CTFileName &fnDemo); // throw char *
	/* Stop recording a demo. */
	//void StopDemoRec(void);

	/* Read current game situation from a stream. */
	void Read_t(CTStream *pstr);   // throw char *
	/* Write current game situation into a stream. */
	void Write_t(CTStream *pstr);  // throw char *

	/* Save the game. */
	void Save_t(const CTFileName &fnmGame); // throw char *
	/* Load the game. */
	void Load_t(const CTFileName &fnmGame); // throw char *

	/* Save a debugging game. */
	void DebugSave(void);   // this doesn't throw anything

	/* Add a new player to game. */
	CPlayerSource *AddPlayer_t(CPlayerCharacter &pcCharacter);  // throw char *

	/* Loop executed in main application thread. */
	void MainLoop(void);

	/* Get count of local players. */
	INDEX GetLocalPlayerCount(void);
	/* Get player entity for a given local player. */
	CEntity *GetLocalPlayerEntity(CPlayerSource *ppls);
	/* Get player entity for a given player by name. */
	CEntity *GetPlayerEntityByName(const CTString &strName);

	/* Get number of entities with given name. */
	INDEX GetNumberOfEntitiesWithName(const CTString &strName);
	/* Get n-th entity with given name. */
	CEntity *GetEntityWithName(const CTString &strName, INDEX iEntityWithThatName);
	/* Get number of player entities with given name. */
	INDEX GetNumberOfPlayerEntitiesWithName(const CTString &strName);
	/* Get n-th player entity with given name. */
	CPlayerEntity *GetNthPlayerEntityWithName(const CTString &strName, INDEX iEntityWithThatName);
	/* Get n-th player entity. */
	CPlayerEntity *GetNthPlayerEntity(INDEX iEntity);
																			 
	/* Get n-th entity of given class. */
	CEntity *GetEntityOfClass(const CTString &strClass, INDEX iEntityOfThatClass);
	/* Test if a given player is local to this computer. */
	BOOL IsPlayerLocal(CEntity *pen);
	// get player source for a given player if it is local to this computer
	CPlayerSource *GetPlayerSource(CEntity *pen);

	// get game time in currently running game
	TIME GetGameTime(void);

	/* Get session properties for current game. */
	void *GetSessionProperties(void);

	/* Send chat message from some players to some other players. */
	void SendChat(ULONG ulFrom, ULONG ulTo, const CTString &strMessage);
	void SendChatMessage(int index, CTString &strRecvName, CTString &strMessage, SLONG nExpChatType = -1);//0616 kwon
//0105
	void SendMessagetoServer(char *msg);
	//BOOL IsTcpServer;					// by seo-40225 부모클래스 CMessageDispatcher로 변수 위치 옮김.
	BOOL m_bReadyStart; //0311 kwon serverloop()가 구동될 준비가 되었는가.
	BOOL IsReadyStart(){return m_bReadyStart;}
	void SetReadyStart(BOOL bstart){ m_bReadyStart = bstart;}
	BOOL m_bSingleMode;

	CEntity* GetEntityByTypeIndex(SBYTE sbType, SLONG slIndex);
	
	int ReturnCCC(int iSCC);
	
	//-----------------------------------------
	// Pet 관련 구조체.
	struct sPetInfo
	{
		sPetInfo()
		{
			lIndex			= -1;
			sbPetTypeGrade	= -1;
			lLevel			= -1;
			llExp			= -1;
			llNeedExp		= -1;
			lHP				= -1;
			lMaxHP			= 0;
			lAbility		= 0;
			lHungry			= -1;
			lMaxHungry		= -1;
			lSympathy		= -1;
			lMaxSympathy	= -1;
			fX				= 0.0f;
			fZ				= 0.0f;
			fH				= 0.0f;
			fR				= 0.0f;			
			sbMapAttribute	= 0;
			lRemainRebirth	= 0;
			strNameCard		= CTString("");
		};

		LONG		lIndex;
		SBYTE		sbPetTypeGrade;
		LONG		lLevel;
		__int64		llExp;
		__int64		llNeedExp;
		LONG		lHP;
		LONG		lMaxHP;
		LONG		lHungry;
		LONG		lMaxHungry;
		LONG		lAbility;
		LONG		lSympathy;
		LONG		lMaxSympathy;
		FLOAT		fX;
		FLOAT		fZ;
		FLOAT		fH;
		FLOAT		fR;		
		UWORD		sbMapAttribute;
		LONG		lRemainRebirth;
		CTString	strNameCard;
	};

	// Function Object
	struct FindPet
	{
		SLONG lIndex;
		FindPet(sPetInfo& info) : lIndex(info.lIndex) {};
		bool operator() (const sPetInfo& info)
		{
			if(info.lIndex == lIndex)
			{
				return true;
			}
			else
				return false;
		}
	};

	// FIXME : 네트워크에 넣는건 좀...-_-;;;
	struct sMoveInfo
	{
		sMoveInfo()
		{
			iIndex	= -1;
			fX		= 0.0f;
			fZ		= 0.0f;
			fH		= 0.0f;
			fAngle	= 0.0f;
		};

		INDEX	iIndex;
		FLOAT	fX;
		FLOAT	fZ;
		FLOAT	fH;
		FLOAT	fAngle;
	};

	// FIXME : 네트워크에 넣는건 좀...-_-;;;
	struct sAttackInfo
	{
		sAttackInfo()
		{
			iAttackIndex	= -1;
			iTargetIndex	= -1;
			ubAttackType	= 0;
			ubTargetType	= 0;
		};

		UBYTE	ubAttackType;
		UBYTE	ubTargetType;
		INDEX	iAttackIndex;
		INDEX	iTargetIndex;
	};

	// FIXME : 네트워크에 넣는건 좀...-_-;;;
	struct sRegenInfo
	{
		sRegenInfo()
		{
			iIndex		= -1;
			iMobType	= -1;
			fX			= 0.0f;
			fY			= 0.0f;
			fZ			= 0.0f;
			fAngle		= 0.0f;
			iYLayer		= 0;
			iEntityID	= -1;
		};
		INDEX	iIndex;
		INDEX	iMobType;
		FLOAT	fX;
		FLOAT	fY;
		FLOAT	fZ;
		FLOAT	fAngle;
		INDEX	iYLayer;
		INDEX	iEntityID;
	};

	std::vector<sPetInfo>		m_vectorPetList;
	std::vector<sMoveInfo>		m_vectorMoveList;
	std::vector<sAttackInfo>	m_vectorAttackNPCList;		// NPC를 공격하는 리스트.
	std::vector<sAttackInfo>	m_vectorAttackPCList;		// PC를 공격하는 리스트.
	std::vector<sAttackInfo>	m_vectorAttackSummonList;	// 소환수를 공격하는 리스트.
	std::vector<sAttackInfo>	m_vectorAttackPetList;	// 소환수를 공격하는 리스트.
	std::vector<sRegenInfo>		m_vectorRegenList;
	std::vector<int>			m_vectorLegitList; // 정당방위 유저 목록
	std::list<int>				m_listAffinityRewardNPCList;	// [100322: selo] 친화도 보상 NPC 리스트
	// FIXME : 네트워크에 넣는건 좀...-_-;;;
	//0527 kwon struct 추가.
	struct MyChaInfo 
	{
		CTString name;
		SLONG	 index;
		UBYTE	 job;
		SBYTE	 job2;					// 전직시...
		ULONG	 zoneNo;
		ULONG	 areaNo;
		ULONG	 LocalNo;				// 지역 정보 ( show signboard )
		FLOAT	 x;
		FLOAT	 h;
		FLOAT	 z;
		FLOAT	 r; //방향
		SBYTE	 yLayer;
		SLONG    userIndex;
		SBYTE	 hairStyle, faceStyle;

		SLONG level, hp, mp, str, dex, intel, con, opt_str, opt_dex, maxMP, maxHP, opt_intel, opt_con;
		SLONG weight, maxweight;
		FLOAT walkspeed, runspeed,attackrange;
		SBYTE attackspeed;
		SQUAD sp, money, curExp, needExp;
		SLONG StatPoint;//1013
		SBYTE sbJoinFlagMerac;			// 공성 참여시 값.
		SBYTE sbJoinFlagDratan;			// WSS_DRATAN_SIEGEWARFARE 070720
		UWORD sbAttributePos;			// 속성맵 값.
		SBYTE sbMountPet;

		FLOAT camera_angle;

		SBYTE pktitle;
		SLONG pkpenalty, pkcount;
		SBYTE pk_mode;
		SLONG fame;						// 명성.
		BOOL  bOtherZone;
		BOOL  bConsensus;							// WSS_DRATAN_SEIGEWARFARE 2007/08/01 교감중
		std::map<int,int> mQuarter;					// WSS_DRATAN_SEIGEWARFARE 2007/08/09 부활진지
		std::map<int,CTString> mQuarterName;		// WSS_DRATAN_SEIGEWARFARE 2007/08/09 부활진지 이름(소유 길드이름 추가)

		SBYTE			sbShopType;		
		LONG			lGuildIndex;		// 임시	길드의 인덱스
		LONG			lGuildLevel;		// 임시 길드의 레벨
		LONG			lGuildPosition;		// 임시 길드에서의 캐릭터의 레벨
		CTString		strGuildName;		// 길드명.
		int				iGuildMasterKickStatus;		// 길드 추방 상태		0 : 상태 이상 없음, 1 : 추방 신청 당한 상태		[trylord 11/12/28]
		UBYTE			ubGuildNameColor;	//[071123: Su-won] DRATAN_SIEGE_DUNGEON
		SBYTE			sbGuildRank;
		BOOL			bExtension;			// 두번째 타입의 무기를 든 경우...

		INDEX			iPetType;			// 애완동물 종류...(?)
		INDEX			iPetAge;			// 애완동물 나이...(?)
		BOOL			bPetRide;			// 애완동물을 타고 있나?
		BOOL			bWildPetRide;		// 공격형 펫 마운트
		
		int 			nEvocationIndex;		// 강신 타입.

		LONG			lTeacherIndex;		// 내 스승의 인덱스
		CTString		strTeacherName;		// 내 스승의 이름.

		ChatMsgBuffer	ChatMsg;		// yjpark
		ChatMsgBuffer	ShopMsg;		// yjpark
		FLOAT			fWaterHeight;	// yjpark - 임시로 추가한 파일. 적당한 위치에 적당한 형식으로 수정 예정..

		SLONG	magicspeed;
		SLONG	skillSpeed;
		CStatusEffect statusEffect;
		CItemEffect itemEffect;
		SBYTE	sbItemEffectOption;
//0707
		SLONG secretkey;
		SLONG lLocation;// Date : 2006-04-27(오후 2:33:10), By eons( NetCafe )

		SLONG lWarpZone;
		SLONG lWarpPos;

// 속성 시스템 [1/17/2013 Ranma]
		UBYTE attrdef, attratt;
		UBYTE attrdefLv, attrattLv;
		// 강신체의 정보가 유지되도록...
		MyChaInfo()
		{
			nEvocationIndex = 0;
			LocalNo = 0;
			sbJoinFlagDratan = WCJF_NONE;			// WSS_DRATAN_SIEGEWARFARE 070720

			slLabel = -1;
			slSubJob = 0;							// [2010/08/25 : Sora] ADD_SUBJOB
			runspeed = 1.0f;

			iGuildMasterKickStatus = 0;
// 속성 시스템 [1/17/2013 Ranma]
			attrdef = 0;
			attratt = 0;

			zoneNo = 0;
			areaNo = 0;
			
			x = 0.0f;
			h = 0.0f;
			z = 0.0f;
			yLayer = 0;

			bExtension = FALSE;
			useTotem = false;
		};
		
		// WSS_VISIBLE 070511
		// 운영자 모드 캐릭트 비저블 처리
		// visible 명령어 사용시 세팅됨
		COLOR m_ModelColor;

		// 080623 라카 기자단 혜택
		SBYTE			sbPresscorps;
		SBYTE			sbSoulCount;

		SLONG			slLabel;	  // [sora] 대상 표식

		// 080901 UI개편
		SLONG			baseHP, baseMP, addedAttack, addedMagic, addedDefense, addedResist;
		SLONG			dodgeRate, baseDodgeRate, magicDodgeRate, baseMagicDodgeRate, hitRate, baseHitRate, magicHitRate, baseMagicHitRate;
		SLONG			critical, baseCritical, deadly, baseDeadly;
		FLOAT			baseRunSpeed;
		SBYTE           baseAttackSpeed;
		CTString		guildPosName;
		ULONG			ChatFlag, ulPlayerState;
		CurEnterType	EntranceType; // 게임중 현재 존 입장 상태 or 게임 모드 상태(특별한 상태로 인해 예외적인 적용을 위한 부분)(예:익스트림 큐브)
		
		INDEX			iNickType;	  // 호칭 타입 ( 0 : 호칭 없음)
		INDEX			iGP;		  // 길드 포인트
		
		INDEX			ep, maxEP;			  // [7/15/2010 kiny8216] Monseter Energy Point

		INDEX			iStudentGiveUpCnt;

		BOOL			bOneSuit;		  // 한벌 의상을 입고 있는지 판단.
		INDEX			iOneSuitDBIndex;
		INDEX			iOneSuitUniIndex;
		CTString GetPlayerName(void) { return name; }

		SLONG			slSubJob;		// [2010/08/25 : Sora] ADD_SUBJOB
		
		ULONG			pkSysRewardFlag;
		BOOL			bpkSysRewardLate;
		INDEX			iSyndicateType;	// 결사대 타입 : 0 무소속, 1 카이룩스, 2 딜라문, 3 플로로
		INDEX			iSyndicateGrade;
		LONGLONG		lSyndicateAccPoint;
		INDEX			iHitEffectType;
		bool			useTotem;

		// 강신/변신 상태 플래그
		typedef enum	_eMORPHSTATUS
		{
			eMORPH_END		= 0,				// 변화 종료
			eMORPH_EVOCATION_BEGIN,				// 강신 변화 준비 (네트워크 요청)
			eMORPH_EVOCATION_CONVERTING,		// 강신 중
			eMORPH_EVOCATION,					// 강신 완료 상태
			eMORPH_TRANSFORMATION_BEGIN,		// 변신 변화 준비 (네트워크 요청)
			eMORPH_TRANSFORMATION_CONVERTING,	// 변신 중
			eMORPH_TRANSFORMATION,				// 변신 완료 상태
		}eMORPHSTATUS;
		eMORPHSTATUS	eMorphStatus;
#ifndef	ISEVOCATION
#define	ISEVOCATION(t)		(t >= CNetworkLibrary::MyChaInfo::eMORPH_EVOCATION_BEGIN && t <= CNetworkLibrary::MyChaInfo::eMORPH_EVOCATION ? 1 : 0)
#endif
#ifndef	ISTRANSFORMATION
#define	ISTRANSFORMATION(t)	(t >= CNetworkLibrary::MyChaInfo::eMORPH_TRANSFORMATION_BEGIN && t <= CNetworkLibrary::MyChaInfo::eMORPH_TRANSFORMATION ? 1 : 0)
#endif

	};
	
	MyChaInfo			MyCharacterInfo;


	struct _AffinityInfo
	{
		struct _AffinityPoint
		{
			SLONG	current;
			SLONG	max;
			_AffinityPoint()
			{
				current = 0;
				max = 0;
			};
		};
		typedef std::map<int, _AffinityPoint> mapAffinity;
		typedef	std::map<int, _AffinityPoint>::iterator mapAffIter;
		SLONG			point;
		SLONG			count;
		std::map< int, _AffinityPoint>		mapAffinityList;

		_AffinityInfo()
		{
			point = 0;
			count = 0;
			//vecAffinityList.clear();
			mapAffinityList.clear();
		};
		~_AffinityInfo()
		{
			mapAffinityList.clear();
		}
	};

	_AffinityInfo		AffinityInfo;
	
	struct _MonsterMercenaryInfo // [2010/10/20 : Sora] 몬스터 용병 카드
	{
		INDEX	index;
		SLONG	grade;
		SLONG	attack;
		SLONG	defence;
		SLONG	magicAttack;
		SLONG	magicDefence;
		SLONG	hp;
		SLONG	mp;
		SLONG	maxHp;
		SLONG	maxMp;

		_MonsterMercenaryInfo()
		{
			Init();
		}

		void Init()
		{
			index = 0;
			grade	= 0;
			attack	= 0;
			defence	= 0;
			magicAttack	= 0;
			magicDefence = 0;
			hp = 0;
			mp = 0;
			maxHp = 0;
			maxMp = 0;
		}
	};

	_MonsterMercenaryInfo MonsterMercenaryInfo;

	// EDIT : BS : 070413 : 신규 패킷 암호화		
	// EDIT : BS : 패킷 암호화
#ifdef CRYPT_NET_MSG
	CNM_KEY_TYPE cnmKey;
#ifndef CRYPT_NET_MSG_MANUAL
	CNM_KEY_TYPE cnmKeyServer;
#endif // #ifndef CRYPT_NET_MSG_MANUAL
#else
	ULONG cnmKey;
#endif // #ifdef CRYPT_NET_MSG

//helper서버 패킷 암호화 키 저장
#ifdef CRYPT_NET_MSG
	CNM_KEY_TYPE subHelperKey;
#ifndef CRYPT_NET_MSG_MANUAL
	CNM_KEY_TYPE subHelperKeyServer;
#endif // #ifndef CRYPT_NET_MSG_MANUAL
#else
	ULONG subHelperKey;
#endif // #ifdef CRYPT_NET_MSG

	// 서버 타임 저장용 변수
	SLONG slServerTimeGap;
	SLONG slServerTime;

	// 캐릭터 선택창으로 이동
	BOOL bMoveCharacterSelectUI;

/*
	//0729 kwon 스킬 애니메이션 아이디.
	struct SkillAnimationID
	{
		BOOL bIsBuff;
		INDEX idPlayer_Anim_Skill[3];
	};
	
	SkillAnimationID	_SkillAnimID[300];//우선 스킬 300개만 하자.
*/
	void SendLoginMessage(CTString& strID, CTString& strPW, ULONG ulVersion);
	void SendSecurityMessage(CTString& strSecurity);							//070901_ttos: 보안카드 UI
	void SendDeleteCharacter(ULONG ulIndex, BYTE del = FALSE);
	void SendDeleteCharacter(ULONG ulIndex, CTString& strSecuNum, BYTE del);
	void SendGameStart(ULONG ulIndex);
	void SendStopChange();
	void SendCreateCharacter( CTString& strName, UBYTE ubJob, UBYTE ubHair, UBYTE ubFace );

	void SendItemUse( SWORD nTab, SWORD nInvenIdx, LONG lIndex, LONG lEtc );
	// 타겟이 필요한 아이템 사용
	void SendtargetItemUse( SWORD nTab, SWORD nInvenIdx, LONG lIndex, SBYTE sbType, LONG lMobIndex );	
	// 결합 주문서 사용
	void SendMixItemUse( SWORD nTab, SWORD nInvenIdx, LONG lMixItemidx, LONG lCashItemIdx, LONG lgameItemIdx);
	// 분리 주문서 사용
	void SendUnMixItemUse( SWORD nTab, SWORD nIdx, LONG lMixItemidx, LONG lCashItemIdx);
	
	// Inventory
	void UseSlotItem( int tabId, int inven_idx, SBYTE sbWearType = -1 );
	void DropItem(int tabId, int inven_idx, SQUAD cnt =1);
	void SwapItem(int tabId, int inven_idx, int tabId2, int inven_idx2);
	void DivideItem(SWORD nTabS, SWORD nIdxS, SWORD nTabT, SWORD nIdxT, int count);
	void ArrangeItem(int tab);
	BOOL IsExtensionState( int iJob, CItemData& ID );
	
	bool IsLord();

	// yjpark |<--
	// Teleport
	void SendWarpCancel();
// [KH_070316] 변경 프리미엄 메모리 관련
	void SendTeleportWrite( UBYTE sendMSG, UBYTE ubSlot, CTString &strComment );
	void SendTeleportMove( UBYTE sendMSG, UBYTE ubSlot );
	void SendWarpTeleport( int iTeleportIndex );
	// Learn skill
	void SendSkillLearn( SLONG slIndex, int NpcVirIdx);
	void SendSSkillLearn( SLONG slIndex, int NpcVirIdx);
	// Stat point
	void SendUseStatPoint( UBYTE ubStatType );

	// HP, MP 회복
	void SendRecoverHPMP( SBYTE sbHPCount, SBYTE sbMPCount );
	void SendRecoverItemSeal( SWORD nTab, SWORD nIdx, SLONG slIndex);

	// 이벤트 보상.
	void SendEventPrize();

	// 서버에 보물의 아이템 지급 목록을 얻음.
	void SendEventTreasureList();

	// 서버로 보물상자를 열겠다고 메세지를 보냄.
	void SendEventOpenTreasure();
	
	// 크리스마스,새해 이벤트 
	void SendEventNewyear(int tv_event);

// [KH_070413] 스승의날 이벤트 관련 추가
	void SendEventMaster();

	void SendReqServerTime();

	// Upgrade
	void UpgradeItem( SLONG nWearPos, SLONG VirIndex, SLONG slPlus, SWORD nTab2, SWORD inven_idx2, SLONG slLevel, INDEX iProtect =-1, SWORD nTab3 =-1, SWORD inven_idx3 =-1);
	
	// Item Level Down
	void ItemLevelDown(SWORD inven_idx1, SWORD inven_idx2);

	// 블러드 아이템 & 정화석.
	void OptionAddItem(UWORD slWearPos, int WearVirIdx, SWORD nTab, SWORD inven_idx, int JemVirIdx);
	void OptionDelItem(UWORD slWearPos, int WearVirIdx, SWORD nTab, SWORD inven_idx, int JemVirIdx);
	
	// Refine
	void RefineReq( SWORD nTab, SWORD inven_idx );

	// Process
	void ProcessReq( SWORD nTab, SWORD inven_idx, SLONG slResultIndex, SLONG slResultCount, CNeedItems* NeedItems );

	// 이기환 추가 ( 12. 6 ) : 제조 메세지 
	void ProductReq( SWORD nTab, SWORD inven_idx, SLONG slResultIndex, SLONG slResultCount, CNeedItems* NeedItems );

	// 이기환 추가 ( 12. 8 ) : 조합 메세지 
	void MixReq( SWORD nTab, SWORD inven_idx, SWORD* arrTab, SWORD* arrIdx );
	
	// Date : 2005-01-12,   By Lee Ki-hwan
	void CompoundReq( SWORD nTab, SWORD nInvenIdx, SWORD* arrTab, SWORD* arrIdx );
	/****   WareHouse **********************************************************/
	void	SendWareHouseChangePassword( const CTString& strOld, const CTString& strNew );
	void	SendWareHouseIsSetPassword();
	void	SendWareHouseSeal();
	void	SendWareHouseListReq( const CTString& strPW );
	void	SendWareHouseDeletePassWord( const CTString& strID );
	void	SendWareHouseSetupPassWord( const CTString& strPW, const CTString& strOldPW );

	/****   Guild   ***********************************************************/
	void GuildJoin( SLONG slGuildIndex, SLONG slSrcIndex, SLONG slDestIndex );				// 가입 신청
	void GuildQuit( );									// 탈퇴 신청

	void GuildCreate( const CTString &strGuildName );	// 길드 생성
	void GuildUpgrade();								// 길드 승급
	void GuildDestroy();								// 길드 해체
	void GuildChangeBoss( SLONG slDestIndex );			// 단장 이임
	void GuildAddViceBoss( SLONG slDestIndex );			// 부단장 임명
	void GuildDelViceBoss( SLONG slDestIndex );			// 부단장 해임
	void GuildMemberFire( SLONG slDestIndex );			// 멤버 퇴출

	void GuildApplicantAccept( SLONG slDestIndex );		// 가입 승인
	void GuildApplicantReject( SBYTE sbWhoCancel );		// 가입 거부

	// Date : 2005-03-18,   By Lee Ki-hwan
	void GBReq( SLONG nCharIndex, SLONG nPrize, SLONG nTime );	
	void GBReqReject();	
	void GBReqAccept();	
	void GBStopReq( SLONG nCharIndex );		
	void GBStopReqReject();	
	void GBStopReqAccept();
	/****   Teach   ***********************************************************/
	void TeachTeacherRegister(SBYTE sbStartPlayTime = -1, SBYTE sbEndPlayTime = -1);			// 선생 목록에 추가
	void TeachTeacherCancelRegister();		// 선생 목록에서 빼줘
	void TeachRefreshTeacherList();			// 후견인 목록 갱신
	void TeachTeacherRequest( SLONG slCharIndex, CTString& strName );				// 선생이 되어달라고 요청함.
	void TeachTeacherReject( BOOL bStudent, SLONG slCharIndex, CTString& strName );	// 선생 안할래.
	void TeachTeacherAccept( SLONG slTeacherIndex, CTString& strTeacherName, SLONG slStudentIndex, CTString& strStudentName );	// 선생 해줄께.
	void TeachTeacherGiveUp( SLONG slTeacherIndex, CTString& strTeacherName, SLONG slStudentIndex, CTString& strStudentName );	// 선생 필요없어~!!!

	/****   ChangeJob ***********************************************************/
	void ChangeJobReq( SBYTE sbJob, int nNpcVirIdx );		// 전직 요청
	void ChangeJobGiveUp(int nNpcVirIdx);					// 직업 포기

	/****   ChangeWeapon ********************************************************/
	void ChangeWeaponReq( SWORD nTab, SWORD inven_idx, LONG lItemVirIndex, LONG lTradeIndex, LONG lChangeType, LONG lTokenVirIndex = -1, LONG lTokenCount = -1 );
	void ChangeWeaponEventReq( LONG lItemIndex, LONG lChangeType );

	// 스탯 초기화
	void StatReset( int iStr, int iDex, int iInt, int iCon );

	// 무기 교환 이벤트
	void SendChangeWeaponEvent();

	/************************************************************************/
	bool SendItemWearingMSG(SBYTE sbType, SBYTE sbWearPos, SWORD nTab, SWORD inven_idx, char take, int nVirIndex = -1);
	/************************************************************************/

	/****   MoonStone   ********************************************************/
	void SendMoonStoneStartReq();
	void SendMoonStoneTryReq();
	void SendMoonStoneResultReq();

	/****   MoonStone   ********************************************************/
	void SendCashMoonStoneReward();
	/****   Pet System   ********************************************************/
	void SendPetChangeRide();
	void SendPetSkillInit();
	void SendPetWarpTown();
	void SendPetDestruction();	// eons	펫 소멸
	void SendPetItemMix( SLONG slPetItemIndex, SLONG slmethod );		// eons 펫 조합
	void SendPetRebirth( SLONG slPetIndex );		// eons 펫 봉인 해제 요청 메세지
	void SendWildPetRebirth( int nTab, int nIdx, int nNpcIdx );
	void SendPetitemInfo(SLONG slOwnerIndex,int nitemIndex);		//인벤의 펫 아이템 정보받

	/****   Exchange   ********************************************************/
	void ExchangeReq_Req( SLONG slDestIndex, CTString &strDestName );
	void ExchangeReq_Rep();
	void ExchangeReq_Rej();
	void ExchangeReq_Ready();
	void ExchangeReq_Ok();
	void ExchangeItem_Add( int nUniIndex, SQUAD llCount );
	void ExchangeItem_Del( int nUniIndex, SQUAD llCount );
	// Quick slot
	void AddQuickSlot( int nPage, int nSlotNum, int nSlotType, int nData0, int nData1 );
	void SwapQuickSlot( int nPage1, int nSlot1, int nPage2, int nSlot2 );
	// Party
	// TO-KR-T20090903-005 관련 파티 신청 수정. [11/27/2009 rumist]
	//void PartyInvite( SBYTE sbType, SLONG slIndex );
	void PartyInvite( SBYTE sbType, SLONG slIndex, CTString strName ="" );
	void PartyAllow();
	void PartyReject();
	void PartyQuit();
	void PartyKick( SLONG slIndex );
	// Item plus effect
	void ItemPlusEffectReq( SBYTE sbOption );
	// yjpark     -->|

	// 라카 기자단 혜댁 이벤트
	void SendPresscorpsMessage(ULONG ulItemIndex, CTString Sendstr);
	
	void SendInput(void); // SendUsingSpeedHack --> SendInput 작명 변경 // 스피드핵 사용자 통보 

	// [2010/08/25 : Sora] ADD_SUBJOB
	BOOL IsMySubJob( SLONG slSubJob ) { return MyCharacterInfo.slSubJob & slSubJob; }
	
	// PLAYTIME 10분간격으로 전달
	void SendUpdatePlayTime(ULONG nTime);
	void SendInitSSkillReq(void);
//안태훈 수정 시작	//(Zone Change System)(0.1)
	void GoZone(int zone, int extra, int NpcIdx = -1 );
//안태훈 수정 끝	//(Zone Change System)(0.1)

//안태훈 수정 시작	//(Teleport System)(0.1)
	void WriteCurrentPos(int slot, const char *szComment);
//안태훈 수정 끝	//(Teleport System)(0.1)
//안태훈 수정 시작	//(GM Command)(0.1)

	void ResetSkillDelayTime(); // 스킬 쿨타임 초기화
	void ResetItemDelayTime(); // 아이템 쿨타임 초기화
	
	//-------------------Item Data---------------------------
	inline CItemData*	GetItemData(int iIndex)
	{
		return CItemData::getData(iIndex);
	}

	inline const char* GetItemName(int iIndex)
	{
		return CItemData::getData(iIndex)->GetName();
	}

	inline const char* GetItemDesc(int iIndex)
	{
		return CItemData::getData(iIndex)->GetDesc();
	}
	//------------------Jewel Data---------------------------

	int  GetJewelComposNas( int nGrade, bool bChaos)
	{
		if (bChaos)
			return wo_aJewelData[nGrade - 1].ca_comp_nas;
		else
			return wo_aJewelData[nGrade - 1].nor_comp_nas;
	}

	BOOL GetJewelGradeInfo( int GradeKey, _vecJewelInfo& Info )
	{
		std::map<int, _vecJewelInfo>::iterator	iterJewelGradeInfo = wo_mapJewelGradeInfo.find(GradeKey);
		std::map<int, _vecJewelInfo>::iterator	iterJewelGradeInfoEnd = wo_mapJewelGradeInfo.end();
		
		if (iterJewelGradeInfo == iterJewelGradeInfoEnd)
			return FALSE;
		
		Info = iterJewelGradeInfo->second;		
		return TRUE;
	}
	
	BOOL GetChaosJewelGradeInfo( int GradeKey, _vecJewelInfo& Info )
	{
		std::map<int, _vecJewelInfo>::iterator	iterChaosJewelGradeInfo = wo_mapChaosJewelGradeInfo.find(GradeKey);
		std::map<int, _vecJewelInfo>::iterator	iterChaosJewelGradeInfoEnd = wo_mapChaosJewelGradeInfo.end();
		
		if (iterChaosJewelGradeInfo == iterChaosJewelGradeInfoEnd)
			return FALSE;
		
		Info = iterChaosJewelGradeInfo->second;		
		return TRUE;
	}

	//------------------Skill Data---------------------------
	inline CSkill	&GetSkillData( const int iIndex )
	{
		if( iIndex > wo_iNumOfSkill || iIndex < 0 )
		{
			//ASSERTALWAYS("Invalid Skill Index!!!");
			return ga_World.wo_aSkillData[0];
		}
		return ga_World.wo_aSkillData[iIndex];
	}

	inline CShopData&	GetShopData(const int iIndex)
	{
		if(iIndex > ga_World.wo_iNumOfShop || iIndex < 0)
		{
			//ASSERTALWAYS("Invalid Shop Index!!!");
			return ga_World.wo_aShopData[0];
		}
		return ga_World.wo_aShopData[iIndex];
	}

	//-------------------Cash Shop Data-----------------------
	// wooss 050903
	inline CCashShopData& GetCashShopData(void)
	{
		return ga_World.wo_aCashGoodsData;
	}

	inline CCashShopData& GetCashShopData(const int iIndex)
	{
		if( iIndex < 0 ) 
		{
			//ASSERTALWAYS("Invalid Shop Index!!!");
			return ga_World.wo_aCashShopData[0];
		}
		return ga_World.wo_aCashShopData[iIndex];
	}

	inline CMakeItemData& GetMakeItemData(int iIndex)
	{
		for(int i =0; i < wo_iNomOfMakeItemData; i++)
		{
			if (wo_aMakeItemData[i].GetFactoryIndex() == iIndex)
			{
				return wo_aMakeItemData[i];
			}
		}
		return wo_aMakeItemData[0]; 
	}

	CTString GetWearingEffect(eEquipment Type, INDEX subType);	// 장비 아이템의 이펙트 이름을 가져온다.

	void SendLeaseItem(int iItemIdx);
	void SendEventItem(int iItemIdx, int iItemcont);
	void BuyItem(int iShopID, int iNumOfItem, __int64 iTotalPrice);			// 상점이 팔고 있는 아이템을 구입함.
	void SellItem(int iShopID, int iNumOfItem, __int64 iTotalPrice);		// 자신이 가지고 있는 아이템을 판매함.
	void FieldShopBuyItem( int iNumOfItem, __int64 iTotalPrice);			// ttos : 잡화상 이용 주문서 사용시
	void FieldShopSellItem(int iNumOfItem, __int64 iTotalPrice);			// ttos : 잡화상 이용 주문서 사용시
	void SendWhoAmI();
	void SendGMCommand(const char *szCommand);

// 강동민 수정 시작		// 2nd Single Dungeon
	void AddRescueNPC(CEntity* pEntity);	
	void UpdateRescueNPC(CEntity* pEntity, int iHP, int iMP);	
	void EndRescueNPC();
// 강동민 수정 끝		// 2nd Single Dungeon

// 강동민 수정 시작
	// NOTE : 메세지를 보낸 시간.
	// NOTE : 타임 아웃을 체크하기 위한 부분.
	unsigned int	m_uiSendedTime;		// 메세지를 보냈을때의 시간.
	BOOL			m_bSendMessage;		// 메세지를 보냈으며(TRUE), 그에 대한 응답을 받으면(FALSE)
// 강동민 수정 끝
	UBYTE	m_ubGMLevel;
//안태훈 수정 끝	//(GM Command)(0.1)
	
	//0701 kwon	
	SLONG SearchClientChaIndex(ULONG Index);

	BOOL SearchEntityByNetworkID( long lIndex, SBYTE sbType, CEntity* &pEntity );
	void SendPickMessage( CEntity *pEntity, ULONG ItemIndex, BOOL bLayerCheck = TRUE );
	void SendStopMessage(CEntity *pEntity, CPlacement3D MyPlacement);
	void SendMoveMessage(CEntity *pEntity, CPlacement3D plPlacement, SLONG speed);
	void SendAttackMessage(CEntity *pAttackerEntity, CEntity *pTargetEntity, BOOL bPvp=FALSE);	
	void SendAttackSymbol();
	void SendProduceMessage(ULONG Index, SBYTE sbKind);
	void SendCollectProduceMessage(LONG npc_client_index);
	void SendSelectProduceMessage(LONG npc_client_index, LONG item_db_index);
	void SendNPCProcessMessage(LONG processItemDBIndex, LONG count);
	void SendBoosterUpgrade(LONG itemServerIndex);
	void SendCancelSkillMessage();
	void SendAttackMessageInContainer(CSelectedEntities& dcEntities);
	void SendSkillMessage(int nSkillIndex, CEntity *pEntity, int nTargetIndex, BOOL bFire, UBYTE ubMove=0);
	void SendPetSkillMessage(int nSkillIndex, CEntity *pSourceEntity, CEntity *pTargetEntity, BOOL bFire);
	void SendPetCommandMessage( int nSkillIndex, CEntity *pSourceEntity );

	// 소환수의 스킬 메세지.
	void SendSlaveSkillMessage(int nSkillIndex, CEntity *pSourceEntity, CEntity *pTargetEntity, BOOL bFire);
	void SendSlaveSkillMessageInContainer(int nSkillIndex, CEntity *pSourceEntity, CSelectedEntities &dcEntities, BOOL bFire);
	
	void SendSkillMessageInContainer(int nSkillIndex, CSelectedEntities& dcEntities, BOOL bFire);	
	void InitSkillID();
	void SetMyPosition(CPlacement3D plPlacement, FLOAT camera_angle);

	// 0 = SYSMSG_NORMAL
	void ClientSystemMessage( CTString &strSysMessage, int nSysType = 0 );
	void SendRebirthMessage();
	void DeleteAllMob();
	
	//wooss 050805
	//확장된 리버스 메시지 
	void SendRebirthMessageEx(int, BOOL ,BOOL);
	//wooss 050808
	//send warp message
	void SendWarpItemMessage(UBYTE warpType,CTString charName=CTString(""),BOOL allow=FALSE);
	void SendProlongMessage(int tabId, int inven_idx);
	//wooss 050817
	//send chang name message
	void SendChangMyName(CTString strMsg);
	void SendChangGuildName(CTString strMsg);
	//wooss 050820
	void SendExSlotMessage(int slotNum);
	//wooss 051031 
	//send 2pan4pan event message
	void SendEvent24(int sel,SBYTE eventType);
	
	// send find friend 060201
	void SendFindFriend(int tv_event, void * tv_input = NULL);
	void SendCashItemMessage(int nType);
	void SendCashItemMessage(int nType, INDEX searchType, INDEX ReqPage = 1);

	void SendActionMessage(SBYTE action_type, SBYTE action_id, SBYTE state, CEntityPointer epTarget = NULL );
	void DelMobTarget(ULONG ClientIndex);
	void ResetMobStatus(ULONG ClientIndex);
	void DelChaTarget(ULONG ClientIndex);
	void ResetChaStatus(ULONG ClientIndex);
	void DelWildPetTarget(ULONG ulSIndex);

	void SendQuestMessage(UBYTE msgQuestType, INDEX data);
	void SendQuestPrizeMessage(UBYTE msgType, INDEX iQuestIndex, INDEX iNpcIndex, INDEX iOptionItemIndex, INDEX iOptionItemPlus);

	// Ranking
	void Ranking_RequestList( SBYTE sbJob );
	void Ranking_RequestPrizeList();
	void Ranking_Prize();

	// 싱글던젼에서 메세지를 묶어서 보내기 위해서 필요한 부분.
	void AddAttackList( UBYTE ubAttackType, INDEX iAttackIndex, UBYTE ubTargetType, INDEX iTargetIndex );
	void SendAttackList();
	void ClearAttackList();

	void AddMoveList( INDEX iIndex, FLOAT fX, FLOAT fZ, FLOAT FH, FLOAT fAngle );
	// EDIT : BS : BEGIN
	void AddMoveList(CEntity &en);
	void SendMoveList(bool bForce = false);
	// EDIT : BS : END
	void ClearMoveList();

	void AddRegenList(INDEX iIndex, INDEX iMobType, FLOAT fX, FLOAT fY, FLOAT fZ, FLOAT fAngle, INDEX iYLayer, INDEX iEntityID);
	void SendRegenList();
	void ClearRegenList();

	// 정당방위 리스트 관리
	void AddLegitList(int Index);
	BOOL SearchLegitList(int Index, BOOL bDel = FALSE);
	void DelLegitList(int Index);

	// [100322: selo] 친화도 보상 NPC 리스트 관리
	void AddAffinityRewardNPC(int iIndex);
	void RemoveAffinityRewardNPC(int iIndex);
	void ClearAffinityRewardNPC();
	const int GetAffinityRewardNPCListCount();
	const std::list<int>& GetAffinityRewardNPCList();

	// 멀티 타겟을 위해서 범위내의 적들을 모두 찾아냅니다.
	void FindTargetsInRange(
								 CEntity* pPlayer, 
								 CEntity* pCenter,
								 CSelectedEntities &cen, 
								 FLOAT fFallOffRange, 
								 INDEX iMaxEnemies,
								 FLOAT fAngle = 360.0f,
								 INDEX iSkillIndex = -1,
								 INDEX	iTargetType = CSkill::STT_TARGET_D120 );
	
	void FindTargetsInRangeEx(CEntity* pCenter,	CSelectedEntities& cen, FLOAT fFallOffRange, INDEX iMaxEnemies);

	void CheckAttackTarget( INDEX iSkillIndex, CEntity* pTargetEntity, FLOAT fDist = 0.0f, BOOL bElementalAttack = FALSE ); 
	// 스킬 공격을 할수 있는지 판단합니다.
	BOOL CheckSkillAttack( INDEX iSkillIndex, CEntity* pEntity );

	// Date : 2005-05-19(오후 8:25:50), By Lee Ki-hwan
	void MgrRegistReq( int nCharIndex, CTString strCharName );
	void MgrRegistAllow( int nCharIndex, CTString strReqCharName );
	void MgrSetCondition( int nCharIndex, int nCondition );
//	void MgrChatting();
	void MgrRegistCancel(int nCharIndex, CTString strReqName);
	void MgrFriendDeleteBlock(int nCharIndex);
	void MgrDeleteMember( int nCharIndex, int nTargetIndex, CTString strName );
	void SendFriendCatting( int nCharIndex, CTString strName, CTString strTargetName, CTString strMessage );
	//메신저 추가 기능
	void MgrFriendInvite( int nCharIndex, int nChatIndex, int nTargetIndex);
	void MgrFriendOut( int nCharIndex, int nChatIndex );
	void MgrFriendChat( int nCharIndex, int nChatIndex, CTString strChat);
	void MgrBlock( CTString strTarget );
	void MgrUnBlock( int nCharIndex);
	void MgrGroupAdd( CTString strName);
	void MgrGroupDel( int nGroupIndex);
	void MgrGroupRename( int nGroupIndex, CTString strNewName);
	void MgrGroupMove( int nGroupIndex, int nCharIndex);
	void MgrSetChatColor( int nColIndex);
	void MgrOneVsOneConnect( int nCharIndex, int nOtherCharIndex, CTString strChat );
	
	// Date : 2005-07-06(오후 2:36:01), By Lee Ki-hwan
	//  공성 관련 메세지 
	void SetTimeReq( int nDay, int nHour, int nZone ); // 공성시간 설정 요청	// [2010/11/01 : Sora] 공성 개편
	void GetTimeReq();						// 공성 시간 확인 요청
	void AttackReq();						// 공성 신청 
	void DefenseGuildReq();					// 수성 신청 
	void AttackCharReq();					// 공성 용병신청
	void AttackGuildReq();					// 수성 용병 신청 
	void WarItemMixReq( SWORD* arrTab, SWORD* arrIdx ); // 공성 아이템 조합 신청 

	// 애완동물 관련...
	void CallPet( LONG lIndex );													// 펫을 호출함.
	void CheckPetType( SBYTE sbPetTypeGrade, INDEX &iPetType, INDEX &iPetAge );		// 펫 타입과 나이를 얻음.
	void RidePet( CEntity *pCharacter, CEntity *pPet, INDEX iPetType );		// 애완동물을 타도록 처리.
	void LeavePet( CEntity *pCharacter );									// 애완동물에서 내리도록 처리.
	// wildpet riding functions [12/21/2010 rumist]
	void RideWildPet( CEntity *pCharacter, CEntity* pWildPet, CTString strFileName );
	void LeaveWildPet( CEntity *pCharacter );
	void UpdatePetTargetInfo( INDEX iPetIndex );						// 애완동물의 정보를 갱신함.
	void ClearPetList();
	void SendMovingGuildWarArea();

	// Date : 2005-09-06(오전 11:22:30), By Lee Ki-hwan
	void SendGuildStashHistroyReq();
	void SendGuildStashViewReq();
	void SendGuildStashTakeReq( LONGLONG llMoney );
	
	// Date : 2005-09-08(오후 5:59:52), By Lee Ki-hwan
	void SendChuseokUpgrade();
	void SendChuseokExchange();

	///////////////////////////////////////////////////////////////////////////////////
	// 2006 추석 이벤트 :Su-won	|---------->
	
	//송편 만들기
	void Send2006ChuseokRicecakeMake();
	void Send2006ChuseokRainbowMake();
	//오색송편 보상품으로 교환하기
	void Send2006ChuseokExchange();
	
	// 2006 추석 이벤트 :Su-won	<----------|
	///////////////////////////////////////////////////////////////////////////////////
	
	// 2006 X-Mase Event [12/12/2006 Theodoric]
	void Send2006XMasEvent( SLONG nCakeCount);
		
	void SendSuperStoneReq();
// 060118 wooss japan package item....
	void SendCouponItemReq(CTString strNum);
// 060223 wooss japan connect item....
	void SendConnectItemReq();
// 060321 eons japan clothes exchange event
	void SendClothesExchange( int nValue );

// 060601 wooss japan rainyday event
	void SendRainyDayExchange();

	void Send2p4pLetter( LONG lGameIndex );

	// 060306 wooss Party Recall CashItem
	void SendPartyRecallConfirm(CTString tmp_str,LONG tmp_idx,BOOL tmp_answer);
	// Date : 2006-04-27(오후 2:36:02), By eons
	void SendNetCafeOpenBox( void );

	// Date : 2006-11-16(오전 11:08:04), By eons
	void SendUsaOpenBetaGift(void);

	// Date : 2006-05-09(오후 5:08:22), By eons
	//파티 매칭 관련
	void SendPartyRegistNormal( int nType );
	void SendPartyRegistParty( CTString strComment, DWORD JobFlag, int LimitLevel );
	void SendPartyListReq( int nPage, int nJob, int nLimitLv, int nPtType, BOOL bState );
	void SendPartyRegistDelete( void );
	void SendPartyInviteReq( int nCharIndex );
	void SendPartyJoinReq( int nBossIndex );
	void SendPartyJoinAllow( int nReqIndex );
	void SendPartyJoinReject( int nReqIndex );
	
	// 060601	wooss worldcup event 1
	void SendWorldCupEvent( int msgCmd, int iSelCountry );
	void SendWorldCupGoldenBallEvent( int nSendType, int nTeamAScore, int nTeamBScore );

	// Date : 2006-06-27(오전 10:42:39), By eons
	void SendBuddhismEvent( BYTE nType );

	// Date : 2006-06-28(오후 4:18:06), By eons
	// 곤충 채집 이벤트
	void SendBuyCollectBox( void );
	void SendGiftCollectBox( void );
	void SendDropInsect( int nInsect );

	// 러브 러브 이벤트( 2007 발렌타인 데이 )
	void SendSaveBingoItem(BYTE nPos, int nGiftItemIndex, int nBoxItemIndex);
	void SendReqGiftChocoBox(int nUniItemIndex);

	int		m_iLastError;

	void LearnPetSkill( LONG nSkillIndex ); 
	void LearnWildPetSkill(LONG nSkillIndex);
	void MandateBossReq( CTString strMandateCharName );
	void SendCastleMapRecent();
	void SendCastleMapSignal( FLOAT fX, FLOAT fY, LONG lSenderFlag, LONG lSenderIndex );

	void CreateSlave( int iOwnerIndex, CEntity* pEntity, int iType );
	void SendGameStart();
	void SetLoadingZone( LONG lZone = -1 );
	LONG GetLoadingZone();

	// check point for dungeon quest
	void SetCheckPointData(int questIdx,int chkNumFlag, int maxChkflag=-1);

	// wooss 070307 --------------------------------------------------->>
	// kw : WSS_WHITEDAY_2007
	void SendWhiteday2007(UBYTE ubType);
	void SendWhiteday2007Letter(int itemIdx,COLOR colMsg,CTString tStr);
	// ----------------------------------------------------------------<<

	// WSS_GUILDMASTER 070416 ----------------------------------------->>
	void SendRequestParentsday(int reqIdx);
	void SendParentsdayAddPoint(int charIdx);
	// ----------------------------------------------------------------<<		

	/*
#ifndef NO_GAMEGUARD
	// WSS_NPROTECT 070402 -------------------------------------------->>
    void SendnProtectAuth2(DWORD dwArg);
	BOOL Init_nProtect();
	void SetHwnd_nProtect(HWND _hwndMain);
	void Check_nProtect();
	BOOL NPGameMonCallbackErrChk( DWORD dwMsg, DWORD dwArg);
	// ----------------------------------------------------------------<<		
#endif
	/**/
	// WSS_MINIGAME 070420 -------------------------------------------->>
	void SendMinigameDefaultMessage(UBYTE eventType, UBYTE subType);
	void SendMinigameSelect(UBYTE eventType, UBYTE subType, UBYTE cSelect);
	void SendMinigameSelectGift(ULONG cSelect);
	// ----------------------------------------------------------------<<

	/*** FLOWERTREE2007 ****************************************************/
	void SendFlowerTreeReq(UBYTE subType);
	/***********************************************************************/

	//////////////////////////////////////////////////////////////////////////
	// 피닉스 이벤트

	void SendPhoenixCharacterCondition();		//피닉스 캐릭터 생성 가능여부
	void SendCreatePhoenixCharacter();	//피닉스 캐릭터 생성	

	//////////////////////////////////////////////////////////////////////////
	// [sora] 원정대 시스템
	
	void ExpeditionCreateReq();																			// 원정대 생성
	void ExpeditionInviteReq( SLONG slIndex );
	void ExpeditionAllowReq();
	void ExpeditionRejectReq();
	void ExpeditionQuitReq();
	void ExpeditionKickReq( SLONG slIndex );
	void ExpeditionEndReq();
	void ExpeditionChangeDivisionTypeReq(UBYTE msgType, SBYTE sbExpedType, SBYTE sbDiviType);
	void ExpeditionChangeLeaderReq( SLONG slIndex );
	void ExpeditionChangeSubLeaderReq( BOOL bIsSubLeader, SLONG slIndex );
	void ExpeditionChangeGroupReq( SLONG slGroupSrc, SLONG slIndex, SLONG slGroupDesc, SLONG slPos );
	void ExpeditionAddCharReq( CTString &strCharName );
	void ExpeditionViewDetailReq( SLONG slGroup, SLONG slIndex );
	void ExpeditionSetLabelReq(SLONG slCharType, SLONG slMode, SLONG slLabelType, SLONG slIndex);
	
	void ExpeditionCollectQuestItemReq(SLONG slIndex);													// 원정대 퀘스트 아이템 수집

	// [sora] 파티 레이드
	void PartyEndReq();							// 파티 해체
	void InitInZoneReq();						// 인던 초기화

	// [sora] 인존 이동
	void RaidInzoneJoinReq(SLONG slZoneNo, SLONG exParam = -1);
	void RaidInzoneQuitReq(SLONG slZoneNo = -1, SLONG slExtraNo = -1);
	
	// [sora] 인스턴트 존 귀속정보요청
	void SendRaidInfoReq();
	//////////////////////////////////////////////////////////////////////////

	// [070613: Su-won]
	// 무기,방어구 교환카드 사용 요청
	void UseChangeWeaponItem( SWORD nTab, SWORD inven_idx, LONG lItemIndex, LONG lChangeType, LONG lTradeItemIndex );
	// 펫 명찰 아이템 사용 요청
	void SendPetNameChageReq( int nPetIndex, CTString strPetName );

	/////////////////////////////////////////////////////////////////////////////
	// [070705: Su-won] EVENT_SUMMBER_2007
	void SendPaperFoldingReq(int nItemIndex0, int nItemIndex1, int nItemIndex2);
	void SendPaperBlendingReq(int nItemIndex);
	// [070705: Su-won] EVENT_SUMMBER_2007
	/////////////////////////////////////////////////////////////////////////////

	// Eons 070905
	void SendBJMono2007Req(UBYTE subType, ULONG ulNum=0); // SK BJ Mono Event
	void SendLC1000DayHatUpgradeReq(SWORD nIdx, LONG lItemIndex); // 1000일 이벤트 기념모자 업그레이드
	void SendLC1000DayTakeHatReq();
	void SendTakeFireCracker();

	// [070807: Su-won] EVENT_ADULT_OPEN
	// 성인서버 오픈 이벤트 관련 보상 요청
	void SendAdultOpenEventReq( int iEvent, int tv_tab=-1, int tv_row=-1, int tv_col=-1);
	
	// WSS_TG2007 2007/09/14
	// 스크린샷 이벤트
	void SendTG2007ScreenshotReq();
	void SendTG2007RichYearReq(int subType);
	
	// Eons 071012 할로윈 이벤트 2007
	void SendHalloween2007Req(UBYTE subType);
	
	// [071122: Su-won] NEW_MOONSTONE
	void SendMoonStoneMix(INDEX iMoonStone, INDEX iMoonStoneBox);

	//[071123: Su-won] DRATAN_SIEGE_DUNGEON
	void SendDratanSiegeDungeonMSG(UBYTE ubSubType, UBYTE ubSubSubType =-1, int nValue =-1);
	
	////////////////////////////////////////////////////////////
	void SendCashPersonShop(UBYTE subType);

	// Eons 071204
	void SendXMAS2007DecoReq(UBYTE subType);

	void SendRestartGame();
	void SendReceiveRestartGame();
	
	void SendUseGoDungeon(INDEX iItemIndex, INDEX iZone, INDEX iExtra);

	void SendItemDelete(SWORD iTab, SWORD inven_idx, INDEX UniIndex, SQUAD count);
	//공격 펫 장비 아이템 사용시
	void SendUseWildPetItem(int posId, int tabId, int inven_idx, int item_index);
	
	void SendSupportSkill(UBYTE ubMsg); // 초보 및 중저레벨 지원사 스킬 메시지 요청(고레벨 지원사추가시 같이 사용)

	void SendHanaposEvent(int nNpcVirIdx);
	
	void SendEventRequital(int nRequital);

	// 레이드 시스템 오브젝트 할성화 메세지(클릭 및 진입)
	void SendRaidObjectEvent(ULONG iObjectID);

	void SendMakeItemList(ULONG sealtype, UBYTE UItype);		//배운 제작 리스트 요청

	inline _AffinityInfo*		GetAffinityPointer() { return (&AffinityInfo);	}
	void SendAffinityConnectReq( SLONG npcIdx );
	void SendAffinityShopReq( SLONG npcIdx );
	void SendAffinityGiftInfoReq( SLONG npcIdx );
	void SendAffinityGiftReq( SLONG npcIdx );
	void SendAffinityInfoTabReq( SLONG npcIdx ); // 친화도 개편2 친화도 정보 요청 [2/13/2013 Ranma]
	inline CAffinityData*		GetAffinityData()	{ return &wo_aAffinityData[0]; }
	void ClearAffinityData();
	
	// connie [2009/9/15] - NPC 찾기
	void SendNPCPortalGoReq(SLONG slIndex);
	void SendNPCPortalLocationReq(SLONG slIndex);

	// [091028 sora] 의상 아이템과 연결된 아이템을 찾기 요청
	void SendCheckComposition(SWORD nTab, SWORD nInvenIdx, SLONG slUniIndex);

	void SendNickNameMessage( SLONG nType, SLONG iNickIndex ); // 호칭 변경 (선택, 취소, 삭제)
	
	// [100208: selo] 포기 퀘스트 복구
	void SendRestoreAbandonQuest(SQUAD llMoney);

	// [100208: selo] 퀘스트 아이템 다시 받기 요청
	void SendTakeAgainQuestItem();

	// [100324 : sora] 라카렛
	void SendLacaretteTokenReq();
	void SendLacaretteRetteReq( SLONG courseNum, SLONG couseIndex, SLONG tokenIndex );
	void SendLacaretteUseCountReq();
	void SendLacaretteResultReq( INDEX itemIndex );
	void SendLacaretteCloseReq();
	void SendLacaretteTokenReadyReq();
	
	// [2010/08/25 : Sora] ADD_SUBJOB 보조 직업 등록
	void SendSubJobRegister( SLONG subJobCode );

	// [100513: selo] ($E_WC2010) 2010 남아공 월드컵
	void SendWorldCup2010_Event(MSG_EVENT_WORLDCUP2010_TYPE e_Type, INDEX iParam = 0);
	
	// [2010/06/30 : Sora] 성주 길드 버프부여
	void SendOwnerGuildBuffReq();

	//  [7/2/2010 kiny8216] ATTENDANCE_SYSTEM : 출석 시스템
	void SendAttendanceReq( INDEX subType );

	// [7/15/2010 kiny8216] MONSTER_ENERGY_IGNITION_SYSTEM
	void SendEnergySkillMessage();

	// [8/31/2010 kiny8216] 후견인 시스템 개편
	void TeachGiftReq();
	
	// [9/29/2010 kiny8216] 성주 코스튬
	void LordCostumeReq();
	// 성주 길드 코스튬 [1/21/2011 ldy1978220]
	void LordGuildCostumeReq();
	void LordGuildTitleReq();

	// [10/6/2010 kiny8216] 쿠폰 프로모션 이벤트
	void SendPromotionEventReq(CTString strNum);
	// [2010/11/01 : Sora] 공성 개편
	void SendGuildWarTimeMenu();
	void SendGuildWarTimeReq();
	
	// [2011/01/18 : Sora] 출석 이벤트
	void SendAttendanceRewardReq();

	// [2011/02/09 : Sora] 퀘스트 아이템 다시 받기
	void SendRestoreQuestItem( SLONG questIndex );

	void SendFaceOffMessage( UBYTE ubType, UBYTE ubFaceType = -1, UBYTE ubHairType = -1 );
	//////////////////////////////////////////////////////////////////////////
	// void SendEventMsg()는 이벤트 메세지 함수가 계속 늘어나는것을 회피하기 위하여 통합 처리를 하도록 합시다.
	// CNetwork class에서 이벤트 전송 파라미터를 받아서 한번에 SendEventMsg()로 전송하도록 합시다.
	// 전송 파라미터는 각 변수 타입 별로, ULONG, UBYTE, CTString 3가지 타입만으로 전송 처리가 용의하므로(type casting),
	// 전송 파라미터 변수는 static_Array<ULONG>, static_array<UBYTE>, static_array<CTString>으로 합시다.
	// 이벤트 메세지 패킷 프로토콜을 서버와 논의하여 맞추도록 합시다.
	// 이해 안되면 바보~
	void SendEventMsg(UBYTE ubEventType);
	
	void SendRaidScene(INDEX ObjType, INDEX Id, INDEX iData = -1);

	void SendNetworkMessage(CNetworkMessage& NetworkMsg) { SendToServerNew(NetworkMsg); }	
		//added by sam 10/11/11
	void SendRankingLsit ( INDEX iIndex, INDEX iPageIndex );
	void SendWildPetMountReq( const BOOL bMount );
	void SendComebackMessage();
	void SendBirthdayMessage( INDEX index );
	// royal rumble [4/19/2011 rumist]
	void SendRoyalRumbleJoinReq();			// 참가
	void SendRoyalRumbleRejectReq();		// 참가 취소.
	void SendRoyalRumbleRewardReq();		// 보상 요청
	void SendRoyalRumblePointReq();			// 포인트 조회.
	void SendRoyalRumbleNextTimeReq();		// 다음 경기 일정 조회.
	void SendRoyalRumbleStartReq();			// 최종 참가 요청.

	void SendMasterStoneUseReq(SWORD nMStoneTab, SWORD ubMStoneIdx, SLONG slMStone, 
		SWORD nTargetTab, SWORD ubItemTgtIdx, SLONG slItemTgt);
	// Temp Message str, int
	// add wooss 060306 for message argument
	CTString	m_tmp_str;
	LONG		m_tmp_idx;

	void SendGuildMarkWndOpenReq();	// [sora] GUILD_MARK
	void SendGuildMarkEditEndReq( SBYTE gm_row, SBYTE gm_col, SBYTE bg_row, SBYTE bg_col, SWORD nTab, SWORD nInvenIdx ); // [sora] GUILD_MARK

	// 랭킹 시스템 개편 [trylord : 110825]
	void SendRankingListEx(UCHAR ucType, UCHAR ucSubType);
	void SendRankingSearchListEx(UCHAR ucType, UCHAR ucSubType, CTString pcstrSearch);

	void pkPenaltyReformRewardReq(LONG titlenum);

	void SendEventKrathongReq();
	void SendUserNotice(CTString strMessage);
	// [2011/11/14 : Sora] 태국 국왕 탄생일 이벤트
	void SendKBRewardReq( UCHAR commandNum );
	// 길드 시스템 개편 [trylord 111227]
	void SendGuildMasterKickReq(INDEX idxGuild);				// 길마 추방 신청 [trylord 111228]
	void SendGuildMasterKickCancelReq(INDEX idxGuild);		// 길마 추방 이의제기 [trylord 111228]
	void SendGuildRemoteJoinReq(CTString strTargetName, LONG lType);
	void SendGuildRemoteJoinOKReq(CTString strTargetName, LONG lType);
	void SendGuildRemoteJoinNOReq();
	void SendGuildRecallReq();

	void SendPetAccumulateUse(SBYTE scType, LONG lIndex);

	// 타겟대상을 서버에 알려준다. (현재는 pc만)
	void SendClickObject(int charIndex);

	// 프리미엄 캐릭터 관련
	void SendPremiumCharItemUse(SWORD tab, SWORD invenIdx, int VirtualIdx);
	void SendPremiumCharJumpReq(CTString strCharName);

	//	FD_TEST 김영환 : 기능함수 추가
	void	Set_MyChar_MorphStatus_EVOCATION_CONVERTING();
	void	Set_MyChar_MorphStatus_EVOCATION();
	void	Set_MyChar_MorphStatus_MORPH_END();
	void	Set_MyChar_MorphStatus_TRANSFORMATION();
	void	Set_MyChar_MorphStatus_EVOCATION_BEGIN();
	bool	Is_MyChar_MorphStatus_MORPH_END();
	int		Get_MyChar_MorphStatus();
	bool	IsAttackMe( int targetID ); // TO.DO HP
	bool	Get_MyChar_Attack(int targetID,LONG	targetHP);
	void	Set_MyChar_Effect(int item_E, int statusEffect);
	SBYTE	Get_MyChar_faceStyle();
	SBYTE	Get_MyChar_hairStyle();
	UBYTE	Get_MyChar_Job();

	void	SendReformItemReq( UCHAR reformerGrade, 
		SWORD reformerTab, SWORD reformerIdx, 
		SWORD magnifierTab, SWORD magnifierIdx, ULONG magnifierCount, 
		SWORD reformItemTab, SWORD reformItemIdx, int nNPCVIdx );

	UINT	getSeq()	{ return ++m_seq; }		// 서버에 패킷을 전송할 때 증가시킴.
	void	resetSeq()	{ m_seq = 0; }			// 서버 연결시에 초기화 시킴.

	void	RestartGame();
private:
	bool	CheckSendSkill(INDEX nIndex);

	UINT	m_seq;
	BOOL	m_bIsRvr;
	INDEX	m_nLastSkillIndex;
	__int64	m_nSkillSendTime;
};

// pointer to global instance of the only network object in the application
ENGINE_API extern CNetworkLibrary *_pNetwork;

// convert string address to a number
ENGINE_API extern ULONG StringToAddress(const CTString &strAddress);
// convert address to a printable string
ENGINE_API extern CTString AddressToString(ULONG ulHost);

#endif  /* include-once check. */

