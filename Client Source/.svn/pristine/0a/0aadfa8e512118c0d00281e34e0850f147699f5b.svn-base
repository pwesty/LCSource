#ifndef SE_INCL_SESSIONSTATE_H
#define SE_INCL_SESSIONSTATE_H
#ifdef PRAGMA_ONCE
  #pragma once
#endif

#include <Engine/Templates/StaticStackArray.h>
#include <Engine/Network/PlayerTarget.h>
#include <Engine/Network/CharacterTarget.h>
#include <Engine/Network/MobTarget.h>
#include <Engine/Network/PetTarget.h>
#include <Engine/Network/SlaveTarget.h>
#include <Engine/Network/ItemTarget.h>
#include <Engine/Network/SessionSocket.h>
#include <Engine/Network/Cmd.h>
#include <Engine/GlobalDefinition.h>
#include <Engine/LocalDefine.h>
#include <Engine/Base/Command.h>
#include <Common/Packet/ptype_inventory.h>
#include <Engine/Network/MessageDefine.h>

/*
 * Session state, manipulates local copy of the world
*/

#if		!defined(WORLD_EDITOR)
//using namespace boost::mpl::placeholders;
#endif	// WORLD_EDITOR

#define		DECLARE_PACKET(x)		void Recv##x(CNetworkMessage *istr)
#define		IMPLEMENT_PACKET(x)		void Recv##x(CNetworkMessage *istr)

#define		DECLARE_MSG_UPDATE(x)	void update##x(CNetworkMessage* istr)
#define		IMPLEMENT_MSG_UPDATE(x)	void update##x(CNetworkMessage* istr)

#if		!defined(WORLD_EDITOR)
#	define		REG_PACKET(msg, n, x)			m_packet[msg].insert( std::make_pair(n, &Recv##x) );
#	define		REG_PACKET_UPDATE(msg, n, x)	m_packet[msg].insert( std::make_pair(n, &update##x) );
#	define		REG_PACKET_R(msg, n, real)		{\
													_recv_func f; \
													f = boost::bind(&CSessionState::real, this, _1); \
													m_packet[msg].insert( std::make_pair(n, f) ); \
												}
#else 
#	define		REG_PACKET(msg, n, x)
#	define		REG_PACKET_UPDATE(msg, n, x)
#	define		REG_PACKET_R(msg, n, real)
#endif	// WORLD_EDITOR

// boost map_list_

#define MODEL_TREASURE	("Data\\Item\\Common\\ITEM_treasure02.smc")

class ENGINE_API CSessionState {
public:
//  CStaticArray<CPlayerTarget> ses_apltPlayers; // client targets for all players in game

  CTString ses_strMOTD;               // MOTD as sent from the server
  INDEX ses_iLastProcessedSequence;   // sequence of last processed stream block
  CNetworkStream ses_nsGameStream;    // stream of blocks from server

  // lerp params
  CTimerValue ses_tvInitialization;  // exact moment when the session state was started
  TIME ses_tmInitializationTick;     // tick when the session state was started
  // secondary lerp params for non-predicted movement
  CTimerValue ses_tvInitialization2;  // exact moment when the session state was started
  TIME ses_tmInitializationTick2;     // tick when the session state was started

  TIME ses_tmLastProcessedTick;      // last tick when all actions were processed

  INDEX ses_iMissingSequence;       // first missing sequence
  CTimerValue ses_tvResendTime;     // timer for missing sequence retransmission
  TIME ses_tmResendTimeout;         // timeout value for increasing the request interval
  CTimerValue ses_tvMessageReceived;  // exact moment when the session state was started

  TIME ses_tmLastDemoSequence;    // synchronization timer for demo playing
  ULONG ses_ulRandomSeed;         // seed for pseudo-random number generation
  ULONG ses_ulSpawnFlags;         // spawn flags for current game

  BOOL ses_bKeepingUpWithTime;     // set if the session state is keeping up with the time
  TIME ses_tmLastUpdated;
  CListHead ses_lhRememberedLevels;   // list of remembered levels
  BOOL ses_bAllowRandom;            // set while random number generation is valid
  
  BOOL ses_bPause;      // set while game is paused
  BOOL ses_bWantPause;  // set while wanting to have paused
  BOOL ses_bGameFinished;  // set when game has finished
  BOOL ses_bWaitingForServer;        // wait for server after level change
  CTString ses_strDisconnected; // explanation of disconnection or empty string if not disconnected
  UBYTE ses_ubNumLevelChanges; // number of server level changes

  INDEX ses_ctMaxPlayers; // maximum number of players allowed in game
  BOOL ses_bWaitAllPlayers; // if set, wait for all players to join before starting
  FLOAT ses_fRealTimeFactor;  // enables slower or faster time for special effects

  CEMsgBuffer     ses_embReceiveBuffer; // buffer used for receiving tick data from the server
  CEntityMessage  ses_emEntityMessage;  // message object used to decode messages from the buffer
  TIME            ses_tmLastTickTime;    // time of the last tick data block received
  ULONG           ses_ulMsgId; // GORAN: temp - to be removed

  // these variables are used for synchronisation with server time
  double ses_dtmOldTickTime;     
  double ses_dtmOldRealTime;
  double ses_dtmAccumulatedDelta; // accumulated difference between server real time and server tick time (detects server stalls)
  BOOL ses_bRestartLocalTime;     // resets the local time delta

  CSessionSocketParams ses_sspParams; // local copy of server-side parameters
public:
  // network message waiters
  void Start_AtServer_t(void);     // throw char *
  // Set lerping factor for current frame.
  void SetLerpFactor(CTimerValue tvNow);
  // notify entities of level change
  void SendLevelChangeNotification(class CEntityEvent &ee);
  // wait for a stream to come from server
  void WaitStream_t(CTMemoryStream &strmMessage, const CTString &strName, INDEX iMsgCode);
  // check if disconnected
  BOOL IsDisconnected(void);

  // print an incoming chat message to console
  void PrintChatMessage(ULONG ulFrom, const CTString &strFrom, const CTString &strMessage);
public:
  /* Constructor. */
  CSessionState(void);
  /* Destructor. */
  ~CSessionState(void);

  // get a pseudo-random number (safe for network gaming)
  ULONG Rnd(void);

  /* Stop the session state. */
  void Stop(void);
  /* Start session state. */
  void Start_t(INDEX ctLocalPlayers); // throw char *

  // do physics for a game tick
  void HandleMovers(void);
  // let the client move the entities around
  void HandleMovers_client(void);
  // do thinking for a game tick
  void HandleTimers(TIME tmCurrentTick);
  // do thinking for a game tick, on the client
  void HandleClientTimers(TIME tmCurrentTick);
  // do a warm-up run of the world for a few ticks
  void WarmUpWorld(void);
  // reset random number generator (always randomizes to same sequence!)
  void ResetRND(void);
  /* Process a game tick. */
  void ProcessGameTick(CNetworkMessage &nmMessage, TIME tmCurrentTick);
  /* Process a gamestream block. */
  void ProcessGameStreamBlock(CNetworkMessage &nmMessage);
  /* Process all eventual avaliable gamestream blocks. */
  void ProcessGameStream(void);
  /* Get number of active players. */
  INDEX GetPlayersCount(void);
  // attach a player entity to this session 
  void AddPlayer(INDEX iPlayerIndex,CPlayerCharacter &pcCharacter,ULONG &ulEntityID,ULONG &ulWeaponsID,ULONG &ulAnimatorID,INDEX iClient);
  void DeleteObject(SBYTE type, ULONG index );

  /* Read session state information from a stream. */
  void Read_t(CTStream *pstr,BOOL bNetwork = FALSE);   // throw char *
  void ReadState_t(CTStream *pstr);            // throw char *
  void ReadWorldAndState_t(CTStream *pstr);    // throw char *
  void ReadRememberedLevels_t(CTStream *pstr);   // throw char *
  /* Write session state information into a stream. */
  void Write_t(CTStream *pstr,BOOL bNetwork = FALSE);  // throw char *
  void WriteWorldAndState_t(CTStream *pstr);   // throw char *
  void WriteState_t(CTStream *pstr);           // throw char *
  void WriteRememberedLevels_t(CTStream *pstr);   // throw char *

  // remember current level
  void RememberCurrentLevel(const CTString &strFileName);
  // find a level if it is remembered
  class CRememberedLevel *FindRememberedLevel(const CTString &strFileName);
  // restore some old level
  void RestoreOldLevel(const CTString &strFileName);
  // forget all remembered levels
  void ForgetOldLevels(void);

  /* adjust local time for the remote client */
  void AdjustLocalTime();
  /* Session state loop. */
  void SessionStateLoop(void);
 
  // demo playback tick data processing
//  void RunDemoTick();
  // extract an entity message from the buffer and process it
  BOOL ReceiveMessage();
  // get the tick data from a network message
  void ReceiveTick(TIME tmTickTime,UBYTE* pubData,SLONG slSize);
  // send an acknowledge for a recieved tick
  void SendAcknowledge(TIME tmTickTime);

//0105
  void ProcessOneSentence(char *msg);
  char* AnyOneArg(char *argument, char *first_arg);
  void SkipSpaces(char **string);
  void StartGame();
  char		m_commIn[2048], m_commInTmp[1024], m_commOut[512], m_oneSentence[512];
  CCmd		*m_pCmd;
  
  //0714 kwon
  void ReceiveUIMessage(CNetworkMessage *istr);
  void ReceiveStatMessage(CNetworkMessage *istr);
  void ReceiveSkillMessage(CNetworkMessage *istr);
  void ReceiveSSkillMessage(CNetworkMessage *istr);
  void ReceiveQuickSlotMessage(CNetworkMessage *istr);
  void ReceiveExchangeMessage(CNetworkMessage *istr);
#ifdef HP_PERCENTAGE
  void ReceiveDamageRealStatMessage(CNetworkMessage *istr);
#endif // HP_PERCENTAGE
  void ReceiveAttackMessage(CNetworkMessage *istr);
  void ReceiveItemMessage(CNetworkMessage *istr);
  void ReceiveGoZoneMessage(CNetworkMessage *istr);
  void ReceivePartyMessage(CNetworkMessage *istr);
  void ReceiveInventoryMessage(CNetworkMessage *istr);
  void ReceiveGmMessage(CNetworkMessage *istr);
// [KH_070316] 변경 프리미엄 메모리 관련
  void ReceiveMemPosMessage(CNetworkMessage *istr);
  void ReceiveGoToMessage(CNetworkMessage *istr);
  void ReceiveMoveMessage(CNetworkMessage *istr);
  // 로그인 관련 메시지...
  void ReceiveLoginMessage(CNetworkMessage *istr);
  void ReceiveServerListMessage(CNetworkMessage *istr);
  void ReceiveChatMessage(CNetworkMessage *istr);
  void ReceiveNpcRegenMessage(CNetworkMessage *istr);
  void ReceiveSystemMessage( CNetworkMessage *istr );
  void ReceiveRebirthMessage( CNetworkMessage *istr );
  void ReceiveEffectMessage( CNetworkMessage *istr );
  void ReceiveEXPMessage( CNetworkMessage *istr );
  
  void ReceiveActionMessage( CNetworkMessage *istr );
  void ReceiveQuestMessage(CNetworkMessage *istr);
  
  void ReceiveEnvMessage( CNetworkMessage *istr );//1013
  void ReceiveAssistMessage( CNetworkMessage *istr );
  void ReceiveWarpMessage(CNetworkMessage *istr);
  void ReceivePkMessage(CNetworkMessage *istr);
  void ReceiveEventMessage(CNetworkMessage *istr);
  void ReceivePersonalShopMessage(CNetworkMessage *istr);
  void ReceiveLegitimateMessage(CNetworkMessage *istr);
  void ReceiveWareHouseMessage(CNetworkMessage *istr);

  void ReceiveGuildMessage(CNetworkMessage *istr);
  void ReceiveTeachMessage(CNetworkMessage *istr);
  void ReceiveChangeJobMessage(CNetworkMessage *istr);  
  void ReceiveMessengerMessage( CNetworkMessage *istr ); 
  void ReceiveMessengerExMessage( CNetworkMessage *istr ); 

  void ReceiveExpeditionMessage( CNetworkMessage *istr ); // [sora] 원정대 시스템
  void ReceiveFactoryMessage( CNetworkMessage *istr );		// 아이템 제작

  void ReceiveAffinityMessage( CNetworkMessage *istr );	// [6/5/2009 rumist] affinity message handler. 

  //wooss 050818
  void ReceiveExtendMessage(CNetworkMessage *istr);	//확장 메시지
  void ReceiveFailMessage(CNetworkMessage *istr); // 실패 메시지 출력
 
  // eons 061206
  void ReceiveExHairChange(CNetworkMessage *istr); // 산타모자 관련( 헤어 변경 )
  void ReceiveExPlayerStateChange(CNetworkMessage *istr); // 게임서포터 관련( 특정아이템 착용시 이펙트 적용 )
// [KH_070413] 스승의날 이벤트 관련 추가
  void ReceiveEventMaster(CNetworkMessage *istr);
  ///////////////////////////////////////////////////////////////////////////////////
	//2013/04/03 jeil 나스 아이템 삭제
  void ReceiveMoneyMessage(CNetworkMessage *istr);
  // WSS_NPROTECT 070402 ------------------------------->>
 #ifndef NO_GAMEGUARD
  void ReceiveExnProtect(CNetworkMessage *istr);
 #endif
  // ---------------------------------------------------<<
  
  void ReceivePetStatusMessage( CNetworkMessage *istr ); 
  void ReceivePetMountMessage( CNetworkMessage *istr );
  void ReceivePetLearnMessage( CNetworkMessage *istr );
  void ReceivePetSkillListMessage( CNetworkMessage *istr );
  void ReceivePetResetSkillMessage( CNetworkMessage *istr );
  void ReceivePetSellInfoMessage( CNetworkMessage *istr );
  void ReceivePetChangeMountMessage( CNetworkMessage *istr );
  void ReceivePetLevelUpMessage( CNetworkMessage *istr );
  void ReceviePetItemMixMessage( CNetworkMessage *istr ); // eons
  void ReceviePetItemChangeMessage( CNetworkMessage *istr ); // eons
  void ReceviePetRebirthMessage( CNetworkMessage *istr ); // eons
  
  void ReceivePartyRecall( CNetworkMessage *istr );  // wooss 060306
  
  void ReceiveElementalDeleteMessage( CNetworkMessage *istr );
  void ReceiveEvocationStart( CNetworkMessage *istr );
  void ReceiveEvocationStop( CNetworkMessage *istr );
  // [070613: Su-won] 펫 명찰 아이템 메시지 처리
  void ReceiveExPetNameChange(CNetworkMessage *istr);

  // [070824: Su-won] PET_COLOR_CHANGE
  void ReceiveExPetColorChange(CNetworkMessage *istr);

  void SetRecentServer(int iRecentGroup, int iRecentServer);
  int GetRecentGroup()	{	return m_iRecentGroup;	}
  int GetRecentServer()	{	return m_iRecentServer;	}

  // EDIT : BS
  void MoveOtherServer(ULONG zone, CTString ip, ULONG port);
  // --- EDIT : BS
  // 판매대행 상인
  void ReceiveCashPersonShopMessage(CNetworkMessage *istr);

  void ReceiveExWildPetMessage(UBYTE index, CNetworkMessage *istr);
  // connie [2009/9/8] - NPC 찾기
  void ReceiveNPCPortalMessage(CNetworkMessage* istr);
  
  // message end check. [9/22/2009 rumist]
  void ReceiveExLoadingEndMessage(CNetworkMessage* istr);
  
  void ReceiveNickNameMessage(CNetworkMessage* istr);	// 호칭

  // [100208: selo] 퀘스트 아이템 다시 받기
  void ReceiveTakeAgainQuestItem(CNetworkMessage* istr);

  void RecieveCostume2Message(CNetworkMessage* istr);

  void RecieveSocketSystemMessage( CNetworkMessage* istr );

  void RecieveLordCostumeMessage( CNetworkMessage* istr );
  
  // [2010/08/25 : Sora] ADD_SUBJOB 보조직업 관련 메시지처리
  void RecieveSubJobMessage(CNetworkMessage* istr);
  
  void RecievePromotionEventMessage(CNetworkMessage* istr);

  //added by sam 10/11/11
  void RecieveRankingView(CNetworkMessage* istr);  
  void ReceiveExpressMessage(CNetworkMessage* istr);
  void ReceivePetStashMessage(CNetworkMessage* istr);
  void ReceiveCalendarMessage(CNetworkMessage* istr);
void ReceiveRVRMessage(CNetworkMessage* istr);

  void ReceiveTimerItemMessage(CNetworkMessage* istr);
  
  void RecieveMonsterMercenaryCardMessage( CNetworkMessage* istr );		// [2010/10/20 : Sora] 몬스터 용병 카드

  void RecieveFaceOffMessage( CNetworkMessage* istr );
  
  void RecieveLuckyDrawBoxMessage( CNetworkMessage* istr );

  void RecieveMsgBoxShow ( CNetworkMessage* istr );		//added by sam 11/01/04 메시지 박스를 보여 줄 메시지를 서버에서 보내준다. 
  void RecieveUsedPartyItemMessage( CNetworkMessage* istr );		// used item for party only. [4/21/2011 rumist]
  void RecieveRoyalrumbleMessage( CNetworkMessage* istr );			// royal rumble [4/19/2011 rumist]
  void RecieveMasterStoneMessage( CNetworkMessage* istr );			// 마스터 스톤 사용 결과 받기
  void RecieveRankingListExMessage( CNetworkMessage* istr );		// 랭킹 시스템 개편 [trylord : 110825]
  void ReceiveDurabilityMessage(CNetworkMessage* istr); // 내구도
  void ReceiveAttendanceMessage(CNetworkMessage* istr); // 출석 체크

  void ReceiveToggleMessage(CNetworkMessage* istr);	// 토글 스킬 메시지.
  void ReceiveItemCollectionMessage(CNetworkMessage* istr);	// 아이템 수집 메시지.

  void ReceivePremiumCharMessage(CNetworkMessage* istr); // 프리미엄 캐릭터 메시지.
  void ReceiveNewsMessage(CNetworkMessage* istr);	// GM 공지

  void ReceiveItemComposMessage(CNetworkMessage* istr); // 1407 유물 사냥꾼 ( 합성 시스템 )

#ifdef KALYDO
  enum eKALYDO_SEND_TYPE
  {
	  eKST_LOGIN,
	  eKST_SEL_SERVER,
	  eKST_CREATE_CHAR,
	  eKST_ENTER_GAME,
	  
	  eKST_MAX
  };

  void DelayLoadingFromKalydo();

  void SetSendCommand(eKALYDO_SEND_TYPE eType, Command* pCmd) {
	  if (eType >= 0 && eType < eKST_MAX)
		  m_pCmdKalydo[eType] = pCmd;
  }
#endif

protected:
	int		m_iRecentGroup;
	int		m_iRecentServer;
#ifdef	KALYDO
	Command*	m_pCmdKalydo[eKST_MAX];
#endif	// KALYDO

private:
	void updateMoney(CNetworkMessage* istr);
	void setItemInfo(int tabId, UpdateClient::itemInfo* pInfo);
	void setWearItemInfo( UpdateClient::itemInfo* pInfo );
	void setWearCostItemInfo( UpdateClient::itemInfo* pInfo );
	void setNewItemEffect(int tabId, int InvenIdx, BOOL bOnOff);
	void updateInvenList(CNetworkMessage* istr);
	void updateItemAdd(CNetworkMessage* istr);
	void updateItemDelete(CNetworkMessage* istr);
	void updateItemCount(CNetworkMessage* istr);
	void updateItemCountSwap(CNetworkMessage* istr);

	void updateItemPlus(CNetworkMessage* istr);
	void updateItemPlus2(CNetworkMessage* istr);
	void updateItemFlag(CNetworkMessage* istr);
	void updateItemUsed(CNetworkMessage* istr);
	void updateItemUsed2(CNetworkMessage* istr);
	void updateItemWearPos(CNetworkMessage* istr);
	void updateItemOption(CNetworkMessage* istr);
	void updateItemSocket(CNetworkMessage* istr);
	void updateItemOriginVar(CNetworkMessage* istr);
	void updateItemComposite(CNetworkMessage* istr);
		
	void updateItemUse(CNetworkMessage* istr);
	void updateItemUseError(CNetworkMessage* istr);

	// 일반 장비
	void updateWearItemUsed(CNetworkMessage* istr);
	void updateWearItemPlus(CNetworkMessage* istr);
	void updateWearItemFlag(CNetworkMessage* istr);

	void ReceiveItemWearError(CNetworkMessage* istr);

	void updateItemWear(CNetworkMessage* istr);
	void updateItemTakeOff(CNetworkMessage* istr);
	void updateWearItemList(CNetworkMessage* istr);
	void updateWearItemInfo(CNetworkMessage* istr);

	// 코스튬 장비
	void updateCostWear(CNetworkMessage* istr);
	void updateCostTakeOff(CNetworkMessage* istr);
	void updateWearCostList(CNetworkMessage* istr);

	// 한벌 의상
	void updateCostSuitWear(CNetworkMessage* istr);
	void ReceiveCostSuitTakeOff();
	
	void updateMoneyReason(CNetworkMessage* istr);

	void updateOldTimerItem(CNetworkMessage* istr);
	void updateEraseAllItem(CNetworkMessage* istr);

	void updateStashPasswordFlag(CNetworkMessage* istr);

	void updateDurabilityForInventory(CNetworkMessage* istr);
	void updateDurabilityForWear(CNetworkMessage* istr);

	void ReceiveItemExchange(CNetworkMessage* istr);

	//-------------------------------------------------------------
	void updateStatus(CNetworkMessage* istr); // 캐릭터 정보
	void updateAt(CNetworkMessage* istr); // 캐릭터 위치 정보

	void updateElementalStatus(CNetworkMessage* istr); // 소환수 정보
	void updatePartyMemberInfo(CNetworkMessage* istr); // 파티원 정보
	void updateExpedMemberInfo(CNetworkMessage* istr); // 원정대원 정보
	
	// Appear
	void updateNpcAppear(CNetworkMessage *istr);			
	void updatePcAppear(CNetworkMessage *istr);	
	void updatePetAppear(CNetworkMessage *istr);
	void updateElementalAppear(CNetworkMessage *istr);
	void updateAPetAppear(CNetworkMessage *istr);
	void updateDisappear(CNetworkMessage *istr);

	void updateStatusPC(CNetworkMessage *istr);
	void updateStatusNPC(CNetworkMessage *istr);
	void updateStatusPet(CNetworkMessage *istr);
	void updateStatusElemental(CNetworkMessage *istr);
	void updateStatusAPet(CNetworkMessage* istr); // APet 정보

	void updateDamage(CNetworkMessage* istr);
	//-------------------------------------------------------------
	// Extend
	bool RecvExtend(CNetworkMessage* istr);
	void recvExRestart(CNetworkMessage* istr);
	void recvExMonsterCombo(CNetworkMessage* istr);

#if		!defined(WORLD_EDITOR)
	typedef boost::function< void(CNetworkMessage*) >	_recv_func;
	typedef std::map< int, _recv_func>					_map_packet;
	typedef std::map< int, _recv_func>::iterator		_map_packet_iter;
#endif	// WORLD_EDITOR
	
	void reg_packet();
	void reg_packet_info();
	void reg_packet_comm();

#if		!defined(WORLD_EDITOR)
	_map_packet		m_packet[MSG_MAX];
#endif	// WORLD_EDITOR
};

BOOL PCStartEffectGroup(const char *szEffectGroupName, SLONG slPCIndex, CEntity *penPC = NULL, float fDelayTime=0.0f);
BOOL ENGINE_API WildPetStartEffectGroup(const char *szEffectGroupName, SLONG slPetIndex, CEntity *penPet = NULL );

#ifdef	VER_TEST
const char* strMSG_MAIN[];
const char* strMSG_EXTEND[];
const char* strMSG_UPDATE[];
#endif	// VER_TEST

#endif  /* include-once check. */

