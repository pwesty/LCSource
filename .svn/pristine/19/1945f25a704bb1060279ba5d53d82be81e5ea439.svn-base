#ifndef __ACMDMSG_H__
#define __ACMDMSG_H__

#include "Descriptor.h"
#include "Character.h"

#include "DratanCastle.h"

#include "Guild.h"

class CWarCastle;
class CDratanCastle;

// 캐릭터 관련
void AppearMsg(CNetMsg::SP& msg, CCharacter* ch, bool bNew = false, bool bAction = false );
void DisappearMsg(CNetMsg::SP& msg, CCharacter* ch);
void DisappearMsgForClientEffect(CNetMsg::SP& msg, CCharacter* ch, int owner_index);
void DisappearAllMsg(CNetMsg::SP& msg);		//내 주변에 있는 것들을 지워라
void TargetClearMsg(CNetMsg::SP& msg, int targetIndex);
void StatusMsg(CNetMsg::SP& msg, CPC* ch);
void AtMsg(CNetMsg::SP& msg, CPC* ch);
void ExpSPMsg(CNetMsg::SP& msg, LONGLONG exp, int sp);

void NameChangeRepMsg(CNetMsg::SP& msg, MSG_EX_NAMECHANGE_ERROR_TYPE type, const char* name, char bguild);
void CashItemPurchaseRepMsg(CNetMsg::SP& msg, MSG_EX_CASHITEM_ERROR_TYPE errorcode, int cashBalance);
void CashItemBringRepMsg(CNetMsg::SP& msg, MSG_EX_CASHITEM_ERROR_TYPE errorcode, bool bPresent);
void CashItemPurchaseListRepMsg(CNetMsg::SP& msg, MSG_EX_CASHITEM_ERROR_TYPE errorcode, int count, int idx[], int ctid[]);
void CashItemBalanceRepMsg(CNetMsg::SP& msg, MSG_EX_CASHITEM_ERROR_TYPE errorcode, int cashBalace);

void NameChangeFriendMsg(CNetMsg::SP& msg, int index, const char* name, char job );
void NameChangeGuildMemberMsg(CNetMsg::SP& msg, int index, const char* name, char job );

void CashItemMoonstoneStartRepMsg(CNetMsg::SP& msg, MSG_EX_CASHITEM_MOONSTONE_ERROR_TYPE errorcode, char grade);
void CashItemGiftRecvNoticeRepMsg(CNetMsg::SP& msg, char bGiftExist);
void CashItemGiftSendRepMsg(CNetMsg::SP& msg, MSG_EX_CASHITEM_ERROR_TYPE errCode);
void CashItemGiftHistoryRepMsg(CNetMsg::SP& msg, bool bSend, MSG_EX_CASHITEM_ERROR_TYPE errCode );
void CashItemGiftRecvListRepMsg(CNetMsg::SP& msg, unsigned char listflag, CNetMsg::SP& recvMsg);

// 변신 관련
void ChangeErrMsg(CNetMsg::SP& msg, MSG_CHANGE_ERR_TYPE type);
void ChangeStartMsg(CNetMsg::SP& msg, CPC* ch);
void ChangeStopMsg(CNetMsg::SP& msg, CPC* ch);

// 공격 관련
void DamageMsg(CNetMsg::SP& msg, CCharacter* ch, CCharacter* tch, MSG_DAMAGE_TYPE damageType, int skillidx, int damage, char flag, int holyItemIndex);

// Wearing
// 050223 : bs : plus 효과를 위한 정보 추가
void WearingMsg(CNetMsg::SP& msg, CPC* ch, char wearpos, int item_db_index, int item_plus);
// --- 050223 : bs : plus 효과를 위한 정보 추가

// Item 관련
void ItemProlongMsg(CNetMsg::SP& msg, int itemindex, int useTime, bool bprolong);
void ItemTakeMsg(CNetMsg::SP& msg, CCharacter* ch, CItem* item);

void ItemDropMsg(CNetMsg::SP& msg, CCharacter* ch, CItem* item);
void ItemAppearMsg(CNetMsg::SP& msg, CItem* item);
void ItemDisappearMsg(CNetMsg::SP& msg, int vIndex);
void ItemUpgradeRepMsg(CNetMsg::SP& msg, char result, bool isRune = false, char nPlusNum = 0);
void ItemRefineRepMsg(CNetMsg::SP& msg, char result);
void ItemOptionAddRepMsg(CNetMsg::SP& msg, CItem* item, MSG_ITEM_OPTION_ADD_RESULT result);
void ItemOptionDelRepMsg(CNetMsg::SP& msg, CItem* item, MSG_ITEM_OPTION_DEL_RESULT result);
void ItemProcessRepMsg(CNetMsg::SP& msg, MSG_ITEM_PROCESS_RESULT result);
void ItemMakeRepMsg(CNetMsg::SP& msg, MSG_ITEM_MAKE_RESULT result);
void ItemMixRepMsg(CNetMsg::SP& msg, MSG_ITEM_MIX_RESULT result);
void ItemArcaneRepMsg(CNetMsg::SP& msg, MSG_ITEM_ARCANE_RESULT result);
void ItemGetMsg(CNetMsg::SP& msg, CPC* ch, CItem* item);
void ItemProcessNPCMsg(CNetMsg::SP& msg, int itemdbindex, int count, MSG_ITEM_PROCESS_NPC_ERROR_TYPE errcode);
void ItemAddBoosterMsg(CNetMsg::SP& msg, MSG_ITEM_ADD_BOOSTER_ERROR_TYPE errcode);
void ItemMixWarItemMsg(CNetMsg::SP& msg, MSG_ITEM_MIX_WARITEM_ERROR_TYPE errcode);
void ItemCheckCompotion(CNetMsg::SP& msg, unsigned short tab, unsigned short invenIndex, int index );

void ItemWearCostumeMsg(CNetMsg::SP& msg, char wear_pos, CItem* item1, CItem* item2);
void CostumeWearingMsg(CNetMsg::SP& msg, CPC* ch, char wear_pos, int item_db_index);
void PreInventoryMsg(CNetMsg::SP& msg, CPC* ch);

// [110207:selo] 한 벌 의상
void CostumeWearingSuitMsg(CNetMsg::SP& msg, CPC* ch);
void prepareSuitMsg(CNetMsg::SP& msg, CPC* ch);

#ifdef DEV_LETS_PARTYTIME
void ItemPartyTypeUseMsg( CNetMsg::SP& msg, int itemIndex ,const char* nick );
#endif //DEV_LETS_PARTYTIME

void ExAPetStatusMsg(CNetMsg::SP& msg, CAPet* pet);
void ExAPetFuntionMsg( CNetMsg::SP& msg, MSG_EX_ATTACK_PET_ERRORTYPE subtype, CAPet* apet, unsigned char error );
void HelperAttackPetMsg( CNetMsg::SP& msg , unsigned char subtype, int ownerIndex );
void ItemAPetWearMsg( CNetMsg::SP& msg, char wear_pos, CItem* item1, CItem* item2 );
void ItemAPetWearingMsg( CNetMsg::SP& msg, CAPet* apet );
void AddItemExAPetMsg( CNetMsg::SP& msg, CItem* addItem );
void ExApetSellInfo( CNetMsg::SP& msg, CAPet* apet );

void APetAIListMsg(CNetMsg::SP& msg, CAPet* apet);
void APetAIOnOffMsg(CNetMsg::SP& msg, CAPet* apet, unsigned char ucErrMsg);

void APetUseMsg( CNetMsg::SP& msg , unsigned char errorMsg );

// 아이템 정보 전송 시 공통 적인 부분을 이 함수 안에 추가한다..
void ItemPublicMsg(CNetMsg::SP& msg, CItem * pItem, bool bOption = true);

void BillItemListReqMsg(CNetMsg::SP& msg, int userindex, int charindex);
// 채팅관련
void ChatWhisperMsg(CNetMsg::SP& msg, int sindex, const char* sname, const char* rname, const char* chat);
void SayMsg(CNetMsg::SP& msg, MSG_CHAT_TYPE chatType, int sender_index, const char* sender_name, const char* receiver_name, const char* chat,int nGroup=-1);
void SayChannelMsg( CNetMsg::SP& msg, unsigned char chattype, int sender_index ,const char* sender_name, unsigned char channel, const char* chat  );

// DB 관련
void DBCharEndMsg(CNetMsg::SP& msg, int remain0, int remain1, unsigned int nSeed);
void DBCharExistMsg(CNetMsg::SP& msg, int index, const char* name, const char* nick, char job, char job2, char hairstyle, char facestyle, int level, LONGLONG ex, int sp, int hp, int maxhp, int mp, int maxmp, int wear[MAX_WEARING], int plus[MAX_WEARING], int nDeleteDelayRemain);
void DBSuccessMsg(CNetMsg::SP& msg);
void DBOKMsg(CNetMsg::SP& msg, int zone);
void DBOtherServerMsg(CNetMsg::SP& msg, int zone, const char* ip, int port);

// 메신저 관련
void MsgrConnectMsg(CNetMsg::SP& msg, int version, int server, int subno, int count, int* zones);
void MsgrRepWhisperNotFoundMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, int sindex, const char* sname);
void MsgrRepWhisperRepMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, int sindex, const char* sname, const char* rname, const char* chat);
void MsgrWhisperReqMsg(CNetMsg::SP& msg, int seq, int server, int subno, int sindex, const char* sname, const char* rname, const char* chat);
void MsgrNoticeMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, const char* chat);
void MsgrUpdateRevMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, int rev, const char* chat, int date);
void MsgrLogoutReqMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, const char* id);
void MsgrPlayerCountReq(CNetMsg::SP& msg, int server, int subno, int charidx);
void MsgrPlayerCountRep(CNetMsg::SP& msg, int seq, int server, int subno, int zone, int charidx, int countZone, int* countPC, int* countProduce, int* countShop, int* countPlay);
void MsgrLogoutRepMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, char success, const char* id);
void MsgrEndGameReqMsg(CNetMsg::SP& msg);
void MsgrRebootReqMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone);
void MsgrObserverMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, const char* chat);
void MsgrNpcNoticeMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, int npcIndex, const char* chat);

#ifdef NEW_DOUBLE_GM_AUTO
void MsgrDoubleEventReqMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, int charindex, int cmd, int percent[], int start[], int end[]);
#else
void MsgrDoubleEventReqMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, int charindex, int cmd, int percent[]);
#endif // NEW_DOUBLE_GM_AUTO
void MsgrDoubleEventRepMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, int charindex, int cmd);

void MsgrDoubleExpEventReqMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, int charindex, int cmd, int expPercent);

void MsgrValentineEventReqMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, bool bStart, int thisServer, int thisSubno, int charIndex);
void MsgrWhiteDayEventReqMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, bool bStart, int thisServer, int thisSubno, int charIndex);
void MsgrDoubleExpEventRepMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, int charindex, int cmd, int expPercent);
void MsgrEventReqMsg(CNetMsg::SP& msg, int server, int subno, int zone, int thisServer, int thisSubno, int charIndex, const char* eventcmd, int flag);

void MsgrEventReqMsg(CNetMsg::SP& msg, MSG_MSGR_TYPE msgType, int seq, int server, int subno, int zone, int charindex, int cmd, int drop);
void MsgrEventRepMsg(CNetMsg::SP& msg, MSG_MSGR_TYPE msgType, int seq, int server, int subno, int zone, int charindex, int cmd, int drop);

#ifdef NEW_DOUBLE_EVENT_AUTO
#ifdef NEW_DOUBLE_EVENT_AUTO_TIME
void MsgrDoubleEventAutoReqMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, int charindex, char cmd, int start[], int end[]);
void MsgrDoubleEventAutoRepMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, int charindex, char cmd, char state, int start[], int end[]);
#else
void MsgrDoubleEventAutoRepMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, int charindex, char cmd, char state);
void MsgrDoubleEventAutoReqMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, int charindex, char cmd);
#endif // NEW_DOUBLE_EVENT_AUTO_TIME
#endif // NEW_DOUBLE_EVENT_AUTO

#ifdef UPGRADE_EVENT_AUTO
void MsgrUpgradeEventReqMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, int charindex, char cmd, int prob, long tStart = -1, long tEnd = -1);
void MsgrUpgradeEventRepMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, int charindex, char cmd, long start = -1, long end = -1);
#else // UPGRADE_EVENT_AUTO
void MsgrUpgradeEventRepMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, int charindex, char cmd);
void MsgrUpgradeEventReqMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, int charindex, char cmd, int prob);
#endif // UPGRADE_EVENT_AUTO
void EventEnchantRate(CNetMsg::SP& msg, unsigned char IsStart, int nEnchantRate);

// 서브 타입은 MSG_MSGR_MESSENER 관련된것만
void MsgrMessengerChatMsg(CNetMsg::SP& msg, MSG_MSGR_TYPE subtype, int makeCharIndex, int chatIndex, int chatColor, int charIndex, CLCString chat = "" );
void MsgrMessengerChatMsg(CNetMsg::SP& msg, MSG_MSGR_TYPE subtype, int makeCharIndex, int chatIndex, int chatColor, const char* charName, CLCString chat = "" );

#ifdef DOUBLE_PET_EXP
#ifdef DOUBLE_PET_EXP_AUTO
void MsgrDoublePetExpEventReqMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, int charindex, int cmd, int PetExpPercent, long start = -1, long end = -1);
void MsgrDoublePetExpEventRepMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, int charindex, int cmd, int PetExpPercent, long start = -1, long end = -1);
#else
void MsgrDoublePetExpEventReqMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, int charindex, int cmd, int PetExpPercent);
void MsgrDoublePetExpEventRepMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, int charindex, int cmd, int PetExpPercent);
#endif
#endif // DOUBLE_PET_EXP

#ifdef DOUBLE_ATTACK
void MsgrDoubleAttackEventReqMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, int charindex, int cmd, int AttackPercent);
void MsgrDoubleAttackEventRepMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, int charindex, int cmd, int AttackPercent);
#endif // DOUBLE_ATTACK

#ifdef EVENT_DROPITEM
void MsgrEventDropItemReqMsg( CNetMsg::SP& msg, int seq, int server, int subno, int zone, int charindex, char type, int npcidx, int itemidx, int prob, int thisServer, int thisSub);
void MsgrEventDropItemRepMsg( CNetMsg::SP& msg, int seq, int server, int subno, int zone, int charindex, char type, int npcidx, int itemidx, int prob, int thisServer, int thisSub);
#endif // EVENT_DROPITEM

void MsgrDungeonTimeReqMsg(CNetMsg::SP& msg, MSG_MSGR_TYPE type, int arg1, int arg2);

void MsgrReloadCatalogMsg( CNetMsg::SP& msg , MSG_MSGR_TYPE type );

void MsgrPopupNoticeMsg( CNetMsg::SP& msg , int seq, int server, int subno, int zone, int html_num, int runtime, time_t start_time );
void MsgrPopupNoticeCloseMsg( CNetMsg::SP& msg , int seq, int server, int subno, int zone, int html_num );
void MsgrPopupNoticeClearMsg( CNetMsg::SP& msg , int seq, int server, int subno, int zone);

// 시스템 메시지
void FailMsg(CNetMsg::SP& msg, MSG_FAIL_TYPE failtype);
void SysWhisperNotFoundMsg(CNetMsg::SP& msg);
void SysFullInventoryMsg(CNetMsg::SP& msg, char tab);
void SysMsg(CNetMsg::SP& msg, MSG_SYS_TYPE systype);
void SysBloodItemMsg(CNetMsg::SP& msg, int itemdbindex);
void SysEnableSuperStoneMsg(CNetMsg::SP& msg, int fame);//0627
void NoviceNotifyMsg(CNetMsg::SP& msg, const char* name);
void NoticeInfoMsg(CNetMsg::SP& msg, int notice);//0704
void SysCannotDuplcationMsg(CNetMsg::SP& msg, int newIndex, int curIndex);

// 환경관련
void EnvWeatherMsg(CNetMsg::SP& msg, int zoneNo);

// 교환 관련
void ExchangeReqMsg(CNetMsg::SP& msg, MSG_EXCHANGE_REQ_TYPE reqType, CPC* src, CPC* dest);
void ExchangeItemMsg(CNetMsg::SP& msg, MSG_EXCHANGE_ITEM_ACTION action, CItem* item, LONGLONG count, GoldType_t nasCount);

// 운영자 명령 관련
void GMWhoAmIMsg(CNetMsg::SP& msg, CPC* ch);

// 파티 관련
void PartyInviteMsg(CNetMsg::SP& msg, char cPartyType1, char cPartyType2, char cPartyType3, int nBossIndex, const char* strBossName);;

void PartyMsg(CNetMsg::SP& msg, MSG_PARTY_TYPE subtype);
void PartyAddMsg(CNetMsg::SP& msg, int nCharIndex, const char* strCharName, CPC* tch, char isboss);
void PartyDelMsg(CNetMsg::SP& msg, bool bKick, int nTargetIndex);
void PartyInfoMsg(CNetMsg::SP& msg, CPC* ch, bool isOnline);
void PartyErrorMsg(CNetMsg::SP& msg, MSG_PARTY_ERROR_TYPE partyerror);
void PartyAssistInfoMsg(CNetMsg::SP& msg, CPC* tch);
void PartyChangeBoss(CNetMsg::SP& msg, const char* bossname, int nNewBossIndex, const char* newname, bool bMandate = false);

void HelperPartyChangeTypeReqMsg(CNetMsg::SP& msg, int nBossIndex, char partytype,char cDiviType,char cAllOneSet);
void PartyChangeType(CNetMsg::SP& msg, char cPartyType,char cDiviType,char cAllOneSet);
void PartyTypeinfo(CNetMsg::SP& msg, char cTypeAll, char cType1, char cType2);
void PartyEndPartyStartMsg(CNetMsg::SP& msg);
void HelperPartyEndPartyReqMsg(CNetMsg::SP& msg, int nBossIndex);
void HelperPartyBreakReqMsg(CNetMsg::SP& msg, int nBossIndex);

void HelperPartyOffline( CNetMsg::SP& msg, int nBossIndex, int targetIndex );
void HelperPartyOnline( CNetMsg::SP& msg, int nBossIndex, int targetIndex, int targetLevel );

// QuickSlot 관련
void QuickSlotAddMsg(CNetMsg::SP& msg, CPC* ch, char page, char slot);
void QuickSlotListMsg(CNetMsg::SP& msg, CPC* ch, char page);

// 싱글던전 - NPC 리젠관련
void NPCRegenMsg(CNetMsg::SP& msg, CNPC* npc, int entityIndex);

// 컨넥터 관련
void ConnConnectMsg(CNetMsg::SP& msg, int version, int server, int subno, int count, int* zones);
void ConnLoginMsg(CNetMsg::SP& msg, CDescriptor* d);
void ConnLogoutMsg(CNetMsg::SP& msg, const char* id);
void ConnPlayingMsg(CNetMsg::SP& msg, CDescriptor* d, MSG_LOGIN_TYPE mode);
void ConnRebootReqMsg(CNetMsg::SP& msg);
void ConnPreopenGiftMsg(CNetMsg::SP& msg, int userindex, int charindex, bool bCancel);




#ifdef RANKER_NOTICE
void ConnRankerReqMsg(CNetMsg::SP& msg, int userindex, int charindex);
#endif

// 유료아이템 관련
void ConnCashItemPurchaseHistoryReq(CNetMsg::SP& msg, int userindex, int charindex, int y, char m, char d);
void ConnCashItemPurchaselistReq(CNetMsg::SP& msg, int userindex, int charindex);
// 역시 bPresent 이면 기프트 테이블 참조, 아니면 보통
void ConnCashItemBringReq(CNetMsg::SP& msg, bool bPresent, int userindex, int charindex, int count, int idx[], int ctid[]);
void ConnCashItemPurchaseReqMsg(CNetMsg::SP& msg, int userindex, const char* idname, int charindex, int cash, int count, int ctid[], int requestCash = -1);
void ConnCashItemBalanceReq(CNetMsg::SP& msg, int userindex, const char* idname);
void ConnCashItemPresentHistoryReq(CNetMsg::SP& msg, bool bSend, int userindex, int charindex, int y, char m, char d);
void ConnCashItemGiftReqMsg(CNetMsg::SP& msg, int sendUserIndex, int sendCharIndex, const char* sendCharName, const char* sendMsg, int recvuserIndex, int recvcharIndex, const char* recvcharName, int count, int idx[], int ctid[]);
void ConnCashItemGiftRecvListReqMsg(CNetMsg::SP& msg, int userIndex, int charIndex );

// 캐릭터 보조/저주 상태 관련
void AssistAddMsg(CNetMsg::SP& msg, CCharacter* ch, int itemidx, int index, char level, int remain
				  , int remainCount
				 );
void AssistModifyMsg(CNetMsg::SP& msg, CCharacter* ch, int itemidx, int index, char level, int remain, int remainCount);
void AssistDelMsg(CNetMsg::SP& msg, CCharacter* ch, int itemidx, int index);
void AssistListMsg(CNetMsg::SP& msg, CPC* ch);

// 몹 상태 메시지
void CharStatusMsg(CNetMsg::SP& msg, CCharacter* ch, LONGLONG state2);

// 부활 msg
void RebirthMsg(CNetMsg::SP& msg, CCharacter* ch);

// 기타 이펙트 메시지
void EffectEtcMsg(CNetMsg::SP& msg, CCharacter* ch, MSG_EFFECT_ETC_TYPE effectType);
void EffectSkillMsg(CNetMsg::SP& msg, CCharacter* ch, const CSkillProto* proto);
void EffectProduceMsg(CNetMsg::SP& msg, CCharacter* ch, CCharacter* tch, MSG_PRODUCE_KIND kind);
void EffectItemMsg(CNetMsg::SP& msg, CCharacter* ch, CItem* item);

// 퀘스트 msg
void QuestPCListMsg(CNetMsg::SP& msg, CPC* pc);
void QuestNPCListMsg(CNetMsg::SP& msg, int total, int* flag, int* index);
void QuestCreateListMsg(CNetMsg::SP& msg, int count, int* index);
void QuestStartMsg(CNetMsg::SP& msg, CQuest* quest);
void QuestCompleteMsg(CNetMsg::SP& msg, CQuest* quest);
void QuestStatusMsg(CNetMsg::SP& msg, CQuest* quest);
void QuestPrizeMsg(CNetMsg::SP& msg, CQuest* quest);
void QuestGiveUpMsg(CNetMsg::SP& msg, CQuest* quest);
void QuestFailMsg(CNetMsg::SP& msg, CQuest* quest);
void QuestErrorMsg(CNetMsg::SP& msg, MSG_QUEST_ERROR_TYPE type);

void QuestCompleteListMsg(CNetMsg::SP& msg, CPC* pc);
void QuestAbandonListMsg(CNetMsg::SP& msg, CPC* pc);
void QuestGiveUpMsg(CNetMsg::SP& msg, int questIndex);
void QuestForceGiveUpMsg(CNetMsg::SP& msg, CQuest* quest);

void QuestPD4ErrorMsg(CNetMsg::SP& msg, MSG_QUEST_PD4_ERROR_TYPE type);
void QuestRestoreAbandonMsg(CNetMsg::SP& msg, int nRestoreCount, int nRestoreIndex[]);

// 스탯 포인트 관련
void StatPointRemainMsg(CNetMsg::SP& msg, CPC* ch);
void StatPointUseMsg(CNetMsg::SP& msg, CPC* ch, MSG_STATPOINT_USE_TYPE type, int value);
void StatPointErrorMsg(CNetMsg::SP& msg, MSG_STATPOINT_ERROR_TYPE errcode);

// 이동 장치 관련
void WarpStartMsg(CNetMsg::SP& msg, CPC* ch);
void WarpCancelMsg(CNetMsg::SP& msg, CPC* ch);
void WarpEndMsg(CNetMsg::SP& msg, CPC* ch);
void WarpTeleportMsg(CNetMsg::SP& msg, int pos, CPC* ch);
void WarpPromptMsg(CNetMsg::SP& msg, int zoneindex, int pos);

void WarpErrorMsg(CNetMsg::SP& msg, MSG_WARP_ERROR_TYPE subtype, const char* name);
void WarpReqIngMsg(CNetMsg::SP& msg, MSG_WARP_TYPE subtype);
void WarpReqMsg(CNetMsg::SP& msg, CPC* ch, MSG_WARP_TYPE subtype);

// PK 관련 메시지
void PKItemSealMsg(CNetMsg::SP& msg, CItem* item);
void PKRecoverItemSealMsg(CNetMsg::SP& msg, CItem* item);
void PKErrorMsg(CNetMsg::SP& msg, char errcode);
#ifdef FREE_PK_SYSTEM
void FreePKMsg(CNetMsg::SP& msg);
#endif //FREE_PK_SYSTEM

// 이벤트 관련
void EventErrorMsg(CNetMsg::SP& msg, MSG_EVENT_ERROR_TYPE errcode);
void EventTreasureBoxMsg(CNetMsg::SP& msg, MSG_EVENT_TREASUREBOX_TYPE type, int arg);
void EventSuperStoneMsg(CNetMsg::SP& msg, MSG_EVENT_SUPERSTONE_TYPE type);//0627

// 개인상점
void PersonalShopErrorMsg(CNetMsg::SP& msg, MSG_PERSONALSHOP_ERROR_TYPE errcode);
void PersonalShopSellStartMsg(CNetMsg::SP& msg, CPC* ch);
void PersonalShopSellListMsg(CNetMsg::SP& msg, CPC* ch);
void PersonalShopChangeMsg(CNetMsg::SP& msg, CPC* ch);
void PersonalShopBuyMsg(CNetMsg::SP& msg, CNetMsg::SP& buymsg, CPC* tch);

// 정당방위 관련
void RightAttackMsg(CNetMsg::SP& msg, CPC* target, MSG_RIGHT_ATTACK_TYPE type);

// 길드 관련
void GuildErrorMsg(CNetMsg::SP& msg, MSG_GUILD_ERROR_TYPE errcode, CGuild * guild = NULL);
void GuildInfoMsg(CNetMsg::SP& msg, CPC* pc);
void GuildListMsg(CNetMsg::SP& msg, CPC* pc);
void GuildOnlineMsg(CNetMsg::SP& msg, CGuildMember* member);
void GuildLevelInfoMsg(CNetMsg::SP& msg, int guildindex, int guildlevel);
void GuildBreakUpNotifyMsg(CNetMsg::SP& msg, int guildindex, const char* guildname);
void GuildRegistCancelMsg(CNetMsg::SP& msg, bool bTarget);
void GuildMemberAddMsg(CNetMsg::SP& msg, int guildindex, int charindex, const char* name);
void GuildMemberOutMsg(CNetMsg::SP& msg, int guildindex, int charindex, const char* name);
void GuildMemberKickMsg(CNetMsg::SP& msg, int guildindex, int bossindex, int charindex, const char* name);
void GuildInfoChangeMsg(CNetMsg::SP& msg, int charindex, int guildindex, const char* name, MSG_GUILD_POSITION_TYPE pos, CPC* ch);
void GuildChangeBossMsg(CNetMsg::SP& msg, int guildindex, int current, int change);
void GuildAppointOfficerMsg(CNetMsg::SP& msg, int guildindex, int charindex);
void GuildFireOfficerMsg(CNetMsg::SP& msg, int guildindex, int charindex);

void GuildBattleReqReqMsg(CNetMsg::SP& msg, int guildindex, const char* name, int prize, int time);
void GuildBattleReqRejectMsg(CNetMsg::SP& msg, int reject_charindex);
void GuildBattleReqAccpetMsg(CNetMsg::SP& msg, int guidindex1, const char* name1, int guildindex2, const char* name2, int prize, int zone, int time);
void GuildBattleStopReqMsg(CNetMsg::SP& msg, int guildindex, const char* name);;
void GuildBattleStopRejectMsg(CNetMsg::SP& msg, int reject_charindex);
void GuildBattleEndMsg(CNetMsg::SP& msg, int winner_guildindex, int guidindex1, const char* name1, int guildindex2, const char* name2, int prize);
void GuildBattleStatusMsg(CNetMsg::SP& msg, int guildindex1, const char* name1, int killcount1, int guildindex2, const char* name2, int killcount2, int battletime, int battleZone);
void GuildBattleErrMsg(CNetMsg::SP& msg, MSG_GUILD_EROOR_BATTLE_TYPE type);

void GuildWarGetTimeMsg(CNetMsg::SP& msg, MSG_GUILD_WAR_ERROR_TYPE errcode, CWarCastle* castle);
void GuildWarNoticeTimeMsg(CNetMsg::SP& msg, int zoneindex, char month, char day, char hour, char min);
void GuildWarNoticeTimeRemainMsg(CNetMsg::SP& msg, int zoneindex, char min);
void GuildWarNoticeStartMsg(CNetMsg::SP& msg, int zoneindex, int remainSec);
void GuildWarJoinAttackGuildMsg(CNetMsg::SP& msg, MSG_GUILD_WAR_ERROR_TYPE errcode, CWarCastle* castle);
void GuildWarJoinDefenseGuildMsg(CNetMsg::SP& msg, MSG_GUILD_WAR_ERROR_TYPE errcode, CWarCastle* castle);
void GuildWarJoinAttackCharMsg(CNetMsg::SP& msg, MSG_GUILD_WAR_ERROR_TYPE errcode, CWarCastle* castle);
void GuildWarJoinDefenseCharMsg(CNetMsg::SP& msg, MSG_GUILD_WAR_ERROR_TYPE errcode, CWarCastle* castle);
void GuildWarPointMsg(CNetMsg::SP& msg, CPC* pc, CWarCastle* castle);
void GuildWarNoticeStartCastleMsg(CNetMsg::SP& msg, int zoneindex, int remainSec, int guildindex1, const char* guildname1, int guildindex2, const char* guildname2, int guildindex3, const char* guildname3);
void GuildWarNoticeRemainFieldTimeMsg(CNetMsg::SP& msg, int zoneindex, int remainSec);
void GuildWarCastleStateMsg(CNetMsg::SP& msg, int zoneindex, CPC* pc, CWarCastle* castle);
void GuildWarGateStateMsg(CNetMsg::SP& msg, int oldstate, int newstate);
void GuildWarEndMsg(CNetMsg::SP& msg, int zoneindex, char bWinDefense, int ownerindex, const char* ownername, int charindex, const char* charname, int nextMonth, int nextDay, int nextHour, int wDay);
void GuildWarSetTimeRepMsg(CNetMsg::SP& msg, MSG_GUILD_WAR_ERROR_TYPE errcode, char month, char day, char hour, char min);
void GuildWarInvalidCommandMsg(CNetMsg::SP& msg, MSG_GUILD_TYPE reqtype);
void GuildStashHistoryRepMsg(CNetMsg::SP& msg, MSG_GUILD_STASH_ERROR_TYPE errcode, int month[7], int day[7], LONGLONG money[7]);
void GuildStashViewRepMsg(CNetMsg::SP& msg, MSG_GUILD_STASH_ERROR_TYPE errcode, LONGLONG money);
void GuildStashTakeRepMsg(CNetMsg::SP& msg, MSG_GUILD_STASH_ERROR_TYPE errcode);

void GuildInclineEstablishMsg( CNetMsg::SP& msg, char guildincline );
void GuildNewInfo( CNetMsg::SP& msg, CPC* ch, int avelevel, int guildpoint, int usepoint );
void GuildMemberListRep( CNetMsg::SP& msg, int membercount, int* membercharindex, int* cumulatePoint, const char CharName[][MAX_CHAR_NAME_LENGTH  + 1], const char PositionName[][GUILD_POSITION_NAME+1], char* job, char* job2, int* level, int* position, CGuild* guild );
#ifdef DEV_GUILD_STASH
void GuildNewManageRep( CNetMsg::SP& msg, int membercount, int* membercharindex, int* contributeExp, int* contributeFame, const char CharName[][MAX_CHAR_NAME_LENGTH  + 1], const char PositionName[][GUILD_POSITION_NAME+1], int* level, int* position, char* stashAuth, CGuild* guild, char first );
#else
void GuildNewManageRep( CNetMsg::SP& msg, int membercount, int* membercharindex, int* contributeExp, int* contributeFame, const char CharName[][MAX_CHAR_NAME_LENGTH  + 1], const char PositionName[][GUILD_POSITION_NAME+1], int* level, int* position,  CGuild* guild, char first );
#endif //DEV_GUILD_STASH
void GuildNewNotify( CNetMsg::SP& msg,const char* title, const char* text );
void GuildNewNotifyTrans( CNetMsg::SP& msg, const char* guildname, const char* title, const char* text );
void GuildSkillListRepMsg(CNetMsg::SP& msg, int active_count, int active_skill_index[], int active_skill_level[], int active_skill_cooltime[], int passive_count, int passive_skill_index[],	int passive_skill_level[], int etc_count, int etc_skill_index[], int etc_skill_level[]);

void GuildSkillLearnMsg(CNetMsg::SP& msg, CSkill* skill);
void GuildPointInfo(CNetMsg::SP& msg, int guildpoint);

void HelperGuildInclineEstablishReqMsg( CNetMsg::SP& msg, CPC* ch, char guildincline );
void HelperGuildMemberAdjust( CNetMsg::SP& msg, CPC* ch, int charindex, const char* strPositionName, int contributeExp, int contributFame, int pos );
void HelperNewGuildInfo( CNetMsg::SP& msg, CPC* ch );
void HelperNewGuildMemberList( CNetMsg::SP& msg, CPC* ch );
void HelperNewGuildManage( CNetMsg::SP& msg, CPC* ch );
void HelperNewGuildNotice( CNetMsg::SP& msg, CPC* ch );
void HelperNewGuildNoticeUpdate( CNetMsg::SP& msg, CPC* ch, const char* title, const char* text );
void HelperNewGuildNoticeTrans( CNetMsg::SP& msg, CPC* ch);
void HelperNewGuildSkillList( CNetMsg::SP& msg, CPC* ch );
void HelperSaveGuildPointMsg(CNetMsg::SP& msg, int guildindex, int guildpoint );
void HelperSaveGuildMemberPointMsg( CNetMsg::SP& msg, int guildindex, int charindex, int GuildMemberPoint );
//void HelperSaveGuildSkillMsg( CNetMsg::SP& msg, CGuild* guild );
void HelperNewGuildPointUpdate( CNetMsg::SP& msg, int charindex, int guildindex, int point );
void HelperGuildLoadReq(CNetMsg::SP& msg, int charindex );
//void HelperUseGuildPoint(CNetMsg::SP& msg, int gindex, int needGP);
void HelperGuildSkillLearnMsg( CNetMsg::SP& msg, int gp, CGuild* guild, int skilltype );
void HelperGuildSkillLearnSendMemberMsg( CNetMsg::SP& msg, int guild_index, int skill_type, int skill_index, int skill_level);

void ConnGuildPointRankerReqMsg( CNetMsg::SP& msg, int guildindex, int charindex );
void GuildPointRankingMsg( CNetMsg::SP& msg, int nCharindex, int nGuildindex, int nRanking );

// 헬퍼 관련
void HelperConnectMsg(CNetMsg::SP& msg, int version, int server, int subno, int count, int* zones);
void HelperShutdownMsg(CNetMsg::SP& msg);
void HelperRepWhisperNotFoundMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, int sindex, const char* sname);
void HelperRepWhisperRepMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, int sindex, const char* sname, const char* rname, const char* chat);

void HelperGuildCreateReqMsg(CNetMsg::SP& msg, CPC* ch, const char* name);
void HelperGuildOnline(CNetMsg::SP& msg, CGuildMember* member);
void HelperGuildLoadReq(CNetMsg::SP& msg, const char* idname, int charindex, int channel, int zoneindex );
void HelperGuildLevelUpReqMsg(CNetMsg::SP& msg, int guildindex, int charindex);
void HelperGuildBreakUpReqMsg(CNetMsg::SP& msg, CPC* boss);
void HelperGuildMemberAddReqMsg(CNetMsg::SP& msg, int guildindex, int targetindex, int requester, const char* name);
void HelperGuildOutReqMsg(CNetMsg::SP& msg, int guildindex, int charindex);
void HelperGuildKickReqMsg(CNetMsg::SP& msg, int guildindex, int bossindex, int charindex);
void HelperGuildChangeBossReqMsg(CNetMsg::SP& msg, int guildindex, int current, int change);
void HelperGuildAppointOfficerReqMsg(CNetMsg::SP& msg, int guildindex, int bossindex, int charindex);
void HelperGuildChat(CNetMsg::SP& msg, int guildindex, int charindex, const char* charname, const char* chat);
void HelperGuildFireOfficerReqMsg(CNetMsg::SP& msg, int guildindex, int bossindex, int charindex);
void HelperCharDelMsg(CNetMsg::SP& msg, int charindex);

void HelperGuildBattleReqMsg(CNetMsg::SP& msg, int guildindex1, int guildindex2, int prize, int zone, int time);
void HelperGuildBattleStopReqMsg(CNetMsg::SP& msg, int guildindex1, int guildindex2);
void HelperGuildBattlePeaceReqMsg(CNetMsg::SP& msg, CGuild* g);
void HelperGuildBattleKillReqMsg(CNetMsg::SP& msg, int of_guildindex, int df_guildindex);
void HelperEventMoonStoneUpdateReqMsg(CNetMsg::SP& msg);
void HelperEventMoonStoneJackPotReqMsg(CNetMsg::SP& msg, int chaindex);

void HelperWarNoticeTimeMsg(CNetMsg::SP& msg, int zoneindex, char month, char day, char hour, char min);
void HelperWarNoticeRemainMsg(CNetMsg::SP& msg, int zoneindex, char remain);
void HelperWarNoticeStartMsg(CNetMsg::SP& msg, int zoneindex, int remainSec);
void HelperWarJoinAttackGuildMsg(CNetMsg::SP& msg, int zoneindex, int guildindex);
void HelperWarJoinDefenseGuildMsg(CNetMsg::SP& msg, int zoneindex, int guildindex);
void HelperWarNoticeStartAttackCastleMsg(CNetMsg::SP& msg, int zoneindex, int remainSec, CGuild* g1, CGuild* g2, CGuild* g3);
void HelperWarNoticeRemainFieldTimeMsg(CNetMsg::SP& msg, int zoneindex, int remainSec);
void HelperWarNoticeEndMsg(CNetMsg::SP& msg, int zoneindex, char bWinDefense, int ownerindex, const char* ownername, int charindex, const char* charname, int nextMonth, int nextDay, int nextHour, int wDay);
void HelperGuildStashHistoryReqMsg(CNetMsg::SP& msg, int guildindex, int charindex);
void HelperGuildStashViewReqMsg(CNetMsg::SP& msg, int guildindex, int charindex);
void HelperGuildStashTakeReqMsg(CNetMsg::SP& msg, int guildindex, int charindex, LONGLONG money);
void HelperGuildStashRollbackMsg(CNetMsg::SP& msg, int guildindex, LONGLONG money);
void HelperGuildStashSaveTaxReqMsg(CNetMsg::SP& msg, int guildIndex, int zoneindex, LONGLONG taxItem, LONGLONG taxProduct);

void HelperPetCreateReqMsg(CNetMsg::SP& msg, int owner, char typeGrade);
void HelperPetDeleteReqMsg(CNetMsg::SP& msg, int index, int owner);

void HelperNameChangeReqMsg(CNetMsg::SP& msg, char bguild, int index, const char* reqname);
void HelperTeachFameUpReqMsg(CNetMsg::SP& msg, int teachidx, int studentidx, const char* studentname, int fame);
void HelperTeachRegisterMsg(CNetMsg::SP& msg, int teachidx, bool bteacher, int studentidx);

void HelperTeacherGiveup(CNetMsg::SP& msg, int teachidx, int studentidx, int teachType, int giveupCnt);
void HelperTeachMsg(CNetMsg::SP& msg, int teachidx, char bTeacher, unsigned char noticetime1=0, unsigned char noticetime2=0);
void HelperTeachLimitTimeCheck(CNetMsg::SP& msg, int charindex);
void HelperTeacherGiftAddReqMsg(CNetMsg::SP& msg, int teacherIdx, int count = -1);
void HelperTeacherGiftReqMsg(CNetMsg::SP& msg, int charindex );
void HelperTeachStudentSucMsg(CNetMsg::SP& msg, int studentIdx, int teacherIdx);

void HelperStuentListRefresh(CNetMsg::SP& msg, int teacherIndex );
void HelperTeacherStudentListCyncReq(CNetMsg::SP& msg, CPC* pc);

void HelperTeachTimeover(CNetMsg::SP& msg, int flag, int teachidx, int studentidx);
void HelperTeacherSuperstoneRecieve(CNetMsg::SP& msg, int charindex);
void HelperTeachLoadReq(CNetMsg::SP& msg, const char* idname, int charindex);

void HelperGiftCharReq(CNetMsg::SP& msg, int sendUserIdx, int sendCharIdx, const char* recvCharName, const char* sendMsg, int count, int idx[], int ctid[] );

// 클라이언트 인터페이스 UI 관련
void UIPlusEffectRep(CNetMsg::SP& msg, CPC* ch);

// 사제 시스템 관련
void TeachTeacherListMsg(CNetMsg::SP& msg, CPC* ch, MSG_TEACH_TEACHER_LIST_TYPE type, CPC** list);
void TeachTeacherReqReqMsg(CNetMsg::SP& msg, CPC* ch);
void TeachTeacherReqRejectMsg(CNetMsg::SP& msg, CPC* ch);
void TeachTeacherReqAcceptMsg(CNetMsg::SP& msg, CPC* teacher, CPC* student);
void TeachEndMsg(CNetMsg::SP& msg, int teacher_index, const char* teacher_name, int studetn_index, const char* student_name, MSG_TEACH_END_TYPE type, int fame = 0);
void TeachInfoMsg(CNetMsg::SP& msg, CPC* ch);
void TeachTeacherGiveUPMsg(CNetMsg::SP& msg, CPC* teacher, CPC* student, int giveupCnt, int teacherFame = -1);
void TeachTeacherGiftMsg(CNetMsg::SP& msg, MSG_TEACH_RENEWER_GIFT_ERROR_TYPE type, CPC* ch, int count = -1);
void TeachLoginMsg(CNetMsg::SP& msg, CPC* ch);
void TeachStudentLevelUPMsg(CNetMsg::SP& msg, CPC* ch);
void TeachErrMsg(CNetMsg::SP& msg, MSG_TEACH_ERR_TYPE type);

void ChatMessengerMsg(CNetMsg::SP& msg, int sindex, const char* sname, const char* rname, const char* chat);
void SysFriendNotFoundMsg(CNetMsg::SP& msg);
void MsgrFriendChatReqMsg(CNetMsg::SP& msg, int seq, int server, int subno, int sindex, const char* sname, const char* rname, const char* chat);
void MsgrRepMessengerChatRepMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, int sindex, const char* sname, const char* rname, const char* chat);
void MsgrRepMessengerNotFoundMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, int sindex, const char* sname);
void HelperFriendSetConditionMsg(CNetMsg::SP& msg, int chaindex, int condition, int bReply, CPC* pc);
void FriendListMsg(CNetMsg::SP& msg, CDescriptor* d);

void BlockPCListMsg(CNetMsg::SP& msg, CDescriptor* d);

void ChatOneMessengerMsg(CNetMsg::SP& msg, int sindex, const char* sname, int rindex, const char* rname, int chatcolor, const char* chat);
void ChatOneOffLineMessengerMsg(CNetMsg::SP& msg, int sindex, int rindex);

void BlockPCRepMsg(CNetMsg::SP& msg, MSG_EX_MESSENGER_ERROR_TYPE errcode, int charIndex, const char* name);
void do_ExMessenger(CPC* ch, CNetMsg::SP& msg);


void MsgrChatLordMsg(CNetMsg::SP& msg, int index, const char* name, const char* chatmsg);

// 펫관련
void ExPetStatusMsg(CNetMsg::SP& msg, CPet* pet);
void ExPetMountMsg(CNetMsg::SP& msg, int charindex, char pettype);
void ExPetLearnMsg(CNetMsg::SP& msg, int skillindex, char level, MSG_EX_PET_LEARN_ERROR_TYPE errcode);
void ExPetSkillListMsg(CNetMsg::SP& msg, CPet* pet);
void ExPetResetSkillMsg(CNetMsg::SP& msg);
void ExPetSellInfoMsg(CNetMsg::SP& msg, CPet* pet);
void ExPetChangeMountMsg(CNetMsg::SP& msg, MSG_EX_PET_CHANGE_MOUNT_ERROR_TYPE errcode);
void ExPetCommandMsg(CNetMsg::SP& msg, int petindex, int commandindex, char targettype, int targetindex);
void ExPetMixItemMsg(CNetMsg::SP& msg, MSG_EX_PET_MIX_ITEM_ERROR_TYPE errcode);
void ExPetLevelupMsg(CNetMsg::SP& msg, CPet* pet);
void ExPetRebirthMsg(CNetMsg::SP& msg, int nPetIndex, MSG_EX_PET_REBIRTH_ERROR_TYPE errcode);

void ExCastleMapRecentMsg(CNetMsg::SP& msg, CWarCastle* castle, CPC* pc);
void ExCastleMapSignalMsg(CNetMsg::SP& msg, MSG_GUILD_POSITION_TYPE flag, CPC* sender, float x, float z);


void ExElementalStatusMsg(CNetMsg::SP& msg, CElemental* elemental);
void ExElementalDeleteMsg(CNetMsg::SP& msg, CElemental* elemental);

void ExEvocationStartMsg(CNetMsg::SP& msg, CPC* ch);
void ExEvocationStopMsg(CNetMsg::SP& msg, CPC* ch);


void ExPetChangeItemMsg(CNetMsg::SP& msg, MSG_EX_PET_CHANGE_ITEM_ERROR_TYPE errcode);
void SysCannotWearDeadPetMsg(CNetMsg::SP& msg, int nPetIndex, int nRemainRebirth);

void ExPartyRecallNoticeMsg(CNetMsg::SP& msg);
void ExPartyRecallPromptMsg(CNetMsg::SP& msg, int charindex, const char* charname);
void ExPartyRecallCancelMsg(CNetMsg::SP& msg, int requestcharindex, const char* requestcharname, int targetcharindex, const char* targetcharname);

#ifdef EVENT_PACKAGE_ITEM
void ConnCouponConfirm(CNetMsg::SP& msg, int charindex, const char* coupon);
#ifdef EVENT_PACKAGE_ITEM_DBLOG

void ConnCouponUse(CNetMsg::SP& msg, int charindex, int cIndex, int type, CLCString idname, CLCString nick);

#else // EVENT_PACKAGE_ITEM_DBLOG

void ConnCouponUse(CNetMsg::SP& msg, int charindex, int cIndex, int type);
#endif // EVENT_PACKAGE_ITEM_DBLOG
#endif // EVENT_PACKAGE_ITEM

void HelperPartyMemberChangeJobMsg(CNetMsg::SP& msg, int nBossIndex, int nCharIndex, char job1, char job2);
void HelperPartyChatMsg(CNetMsg::SP& msg, int nBossIndex, int nCharIndex, const char* strName, const char* strChat);
void HelperPartyRecallPromptMsg(CNetMsg::SP& msg, int nBossIndex, int nReqIndex, const char* strReqName, char cIsInCastle, int nGuildIndex, bool bUseContinent, int zoneID, CPos& pos);
void HelperPartyRecallConfirmMsg(CNetMsg::SP& msg, int nBossIndex, int nReqIndex, const char* strReqName, int nRepIndex, const char* strRepName, char yesno);
void HelperPartyRecallProcMsg(CNetMsg::SP& msg, int nBossIndex, int nCharIndex, int nZone, CPos* pos, char cIsInCastle, int nGuildIndex);

void HelperPartyInviteReqMsg(CNetMsg::SP& msg, int nBossIndex, const char* strBossName, int nBossLevel, int nTargetIndex, char partyType);
void HelperPartyInviteRepMsg(CNetMsg::SP& msg, int nBossIndex, const char* strBossName, int nTargetIndex, const char* strTargetName, char partyType, MSG_HELPER_PARTY_ERROR_TYPE errcode, int nBossLevel );
void HelperPartyAllowReqMsg(CNetMsg::SP& msg, int nBossIndex, int nTargetIndex, const char* strTargetName, int nTargetLevel );
void HelperPartyRejectReqMsg(CNetMsg::SP& msg, int nBossIndex, int nTargetIndex);
void HelperPartyQuitReqMsg(CNetMsg::SP& msg, int nBossIndex, int nTargetIndex);
void HelperPartyKickReqMsg(CNetMsg::SP& msg, int nBossIndex, int nTargetIndex);
void HelperPartyChangeBossReqMsg(CNetMsg::SP& msg, int nBossIndex, const char* strTargetName);
void HelperPartyDataInitReqMsg(CNetMsg::SP& msg);
void HelperPartyRecallConfirmFail(CNetMsg::SP& msg);

void MsgrEventOXSetStartMsg(CNetMsg::SP& msg, int nGMCharIndex, int yy, int mm, int dd, int hh, int min);
void MsgrEventOXEndMsg(CNetMsg::SP& msg, int nGMCharIndex);
void MsgrEventOXQuizMsg(CNetMsg::SP& msg, int nGMCharIndex, int nQuizNo, int nSec);

void EventOXQuizMsg(CNetMsg::SP& msg, MSG_EVENT_OXQUIZ_TYPE subtype);
void EventOXQuizQuizMsg(CNetMsg::SP& msg, int nQuizNo, int nSec, const char* strQuiz);
void EventOXQuizAnswerMsg(CNetMsg::SP& msg, int nQuizNo, bool bAnswer);

void MsgrRecommendMsg(CNetMsg::SP& msg, int nGMCharIndex, int nRecommendServer);
void EventRecommendSetMsg(CNetMsg::SP& msg);
void EventRecommendPotionMsg(CNetMsg::SP& msg);
void MsgrRecommendMoonstoneMsg(CNetMsg::SP& msg, int nCharIndex, const char* strCharName);
void EventRecommendMoonstoneMsg(CNetMsg::SP& msg, int nCharIndex, const char* strCharName);

void ExPartyMatchRegMemberRepMsg(CNetMsg::SP& msg, MSG_EX_PARTY_MATCH_ERROR_TYPE errcode);
void ExPartyMatchRegPartyRepMsg(CNetMsg::SP& msg, MSG_EX_PARTY_MATCH_ERROR_TYPE errcode);
void ExPartyMatchMemberListRepMsg(CNetMsg::SP& msg, int nPageNo, int nCharLevel, std::vector<CPartyMatchMember*>& list);
void ExPartyMatchPartyListRepMsg(CNetMsg::SP& msg, int nPageNo, int nCharLevel, std::vector<CPartyMatchParty*>& list);
void HelperPartyMatchRegMemberReqMsg(CNetMsg::SP& msg, int nCharIndex, const char* strCharName, int nLevel, int nZone, char nJob, char nPartyType);
void HelperPartyMatchRegPartyReqMsg(CNetMsg::SP& msg, int nBossIndex, const char* strBossName, int nBossLevel, int nZone, int nJobFlag, char cLimitLevel, const char* strComment);
void HelperPartyMatchDelReqMsg(CNetMsg::SP& msg, int nCharIndex);
void ExPartyMatchDelRepMsg(CNetMsg::SP& msg);
void HelperPartyMatchInviteReqMsg(CNetMsg::SP& msg, int nBossIndex, const char* strBossName, int nBossLevel, int nCharIndex);
void HelperPartyMatchInviteRepMsg(CNetMsg::SP& msg, MSG_HELPER_PARTY_MATCH_ERROR_TYPE nErrorCode, int nBossIndex, const char* strBossName, int nCharIndex, const char* strCharName, char cPartyType);
void ExPartyMatchInviteRepMsg(CNetMsg::SP& msg, MSG_EX_PARTY_MATCH_ERROR_TYPE nErrorCode, char cPartyType, int nBossIndex, const char* strBossName, int nCharIndex, const char* strCharName);
void HelperPartyMatchJoinReqMsg(CNetMsg::SP& msg, int nBossIndex, int nCharIndex, const char* strCharName, int nCharLevel, char cCharJob);
void ExPartyMatchJoinRepMsg(CNetMsg::SP& msg, MSG_EX_PARTY_MATCH_ERROR_TYPE nErrorCode, char cPartyType, int nBossIndex, const char* strBossName, int nReqCharIndex, const char* strReqCharName, char cReqCharJob);
void HelperPartyMatchJoinAllowReqMsg(CNetMsg::SP& msg, int nBossIndex, int nCharIndex, int nLevel, int reqCharLevel );
void HelperPartyMatchJoinRejectReqMsg(CNetMsg::SP& msg, int nJoinCharIndex, int nRejectCharIndex);
void HelperPartyMatchMemberChangeInfoMsg(CNetMsg::SP& msg, int nCharIndex, MSG_HELPER_PARTY_MATCH_MEMBER_CHANGE_INFO_TYPE nType, const char* strCharName, int nLevel, int nZone);

void SysExpireItemcompositeMsg(CNetMsg::SP& msg, int nItemDBIndex);

void EventWorldcupVoteMsg(CNetMsg::SP& msg, int nCountry, MSG_EVENT_WORLDCUP_ERROR_TYPE nErrorCode);
void EventWorldcupGiftMsg(CNetMsg::SP& msg, int nRank, MSG_EVENT_WORLDCUP_ERROR_TYPE nErrorCode);

void MsgrEventGoldenballVoteMsg(CNetMsg::SP& msg, int nGMCharIndex, const char* strTeam1, const char* strTeam2, int nYear, int nMonth, int nDay, int nHour, int nMinute);
void MsgrEventGoldenballGiftMsg(CNetMsg::SP& msg, int nGMCharIndex, const char* strTeam1, int nTeam1Score, const char* strTeam2, int nTeam2Score, int nYear, int nMonth, int nDay, int nHour, int nMinute, time_t timeEndVote);
void MsgrEventGoldenballEndMsg(CNetMsg::SP& msg, int nGMCharIndex);
void EventGoldenballVoteStartMsg(CNetMsg::SP& msg, const char* strTeam1, const char* strTeam2, int nYear, int nMonth, int nDay, int nHour, int nMin);
void EventGoldenballVoteEndMsg(CNetMsg::SP& msg, const char* strTeam1, const char* strTeam2);
void EventGoldenballGiftStartMsg(CNetMsg::SP& msg, const char* strTeam1, int nTeam1Score, const char* strTeam2, int nTeam2Score, int nYear, int nMonth, int nDay, int nHour, int nMin);
void EventGoldenballGiftEndMsg(CNetMsg::SP& msg, const char* strTeam1, const char* strTeam2);
void EventGoldenballVoteMsg(CNetMsg::SP& msg, int nTeam1Score, int nTeam2Score, MSG_EVENT_GOLDENBALL_ERROR_TYPE nErrorCode);
void EventGoldenballGiftMsg(CNetMsg::SP& msg, MSG_EVENT_GOLDENBALL_ERROR_TYPE nErrorCode);

void EventRain2006Msg(CNetMsg::SP& msg, MSG_EVENT_RAIN_2006_ERROR_TYPE nErrorCode);

void ItemLendWeaponMsg(CNetMsg::SP& msg, int nItemDBIndex, MSG_ITEM_LEND_WEAPON_ERROR_TYPE nErrorCode);
void SysSeparateItemMsg(CNetMsg::SP& msg, int nItemDBIndex);

void EventCollectBugMsg(CNetMsg::SP& msg, MSG_EVENT_COLLECT_BUG_TYPE nType, int nData);

void QuestCollectMsg(CNetMsg::SP& msg, int nNPCIndex, int nCharIndex, int nItemIndex);

void EventChuseok2006Msg(CNetMsg::SP& msg, MSG_EVENT_CHUSEOK_2006_TYPE nErrorcode);

void MsgrLoginServerMsg(CNetMsg::SP& msg, int nCode);

void SysRaidmobRegenMsg(CNetMsg::SP& msg, int nNPCIndex, float x, float z);

void ExHairChangeMsg(CNetMsg::SP& msg, CPC* pPC );

void ExPlayerStateChangeMsg(CNetMsg::SP& msg, CPC* pPC);

void EventWhiteday2007Msg(CNetMsg::SP& msg, MSG_EVENT_WHITEDAY_2007_TYPE nMsg);


void SysChangeLuckyBoxMsg(CNetMsg::SP& msg, int nItemDBIndex, int nUseDBIndex);

void SysChangeRaidBoxMsg(CNetMsg::SP& msg, int nItemDBIndex, int nUseDBIndex);

void EventValentine2007ErrorMsg(CNetMsg::SP& msg, MSG_EVENT_VALENTINE_TYPE nErrorcode);
void EventValentine2007Msg(CNetMsg::SP& msg, MSG_EVENT_VALENTINE_TYPE nErrorcode, int extra);


void EventEggsHunt2007ErrorMsg(CNetMsg::SP& msg, MSG_EVENT_EGGS_HUNT_2007_TYPE nMsg);

void EventGomdori2007Msg(CNetMsg::SP& msg, MSG_EVENT_GOMDORI_2007_TYPE nType);
void EventGomdori2007ResultMsg(CNetMsg::SP& msg, MSG_EVENT_GOMDORI_2007_TYPE nResult, char cWinCount, char cUserSel, char cNPCSel);
void EventGomdori2007EndMsg(CNetMsg::SP& msg, char cWinCount, char cPromptType, int nExtra1, int nExtra2);
void EventGomdori2007SelectGiftMsg(CNetMsg::SP& msg, int nItemDBIndex);
void EventGomdori2007ViewStatusMsg(CNetMsg::SP& msg, int nCount, int* nStatus);
void ConnEventGomdori2007CountMsg(CNetMsg::SP& msg);
void ConnEventGomdori2007StatusMsg(CNetMsg::SP& msg, int nCharIndex);

#ifdef GMTOOL
void MsgGmKickIDCommandReqMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, int thisServer, int thisSubno, int charindex, const char* id);
void MsgGmKickIDCommandRepMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, char success, int charindex, const char* id);
void MsgrGmToolChatMonitorReqMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, int gmcharindex, const char* name, const char* onoff);
void MsgrGmToolChatMonitorRepMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, char success, int gmcharindex, const char* id);
void MsgrNoticeGmChatMonitorMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, int thisServer, int thisSub, int thisZone, const char* chat, int charindex, const char* name, unsigned char chattype);
void MsgrGmToolChatMonitorOffMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, int charindex);
void MsgrGmToolChatMonitorPartyOffMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, int bossindex);
void MsgrGmToolChatMonitorGuildOffMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, int guildindex);
void MsgrNoticeGmChatMonitorGuildMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, int thisServer, int thisSub, int thisZone, const char* chat, int guildindex, const char* name, int charindex, const char* charname);
void MsgrNoticeGmChatMonitorPartyMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, int thisServer, int thisSub, int thisZone, const char* chat, int bossindex, const char* name, int charindex, const char* charname);
void MsgrRepGMWhisperNotFoundMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, int sindex);
void MsgrGMWhisperReqMsg(CNetMsg::SP& msg, int seq, int server, int subno, int sindex, const char* sname, const char* rname, const char* chat);
void MsgrGMWhisperReqMsg(CNetMsg::SP& msg, int seq, int server, int sub, const char* chat, int charindex, const char* name, int serverno, int subno, const char* receiver);
void MsgrNoticeGmChatMonitorWhisperMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, int thisServer, int thisSub, int thisZone, const char* chat, int charindex, const char* name, unsigned char chattype, const char* sender);
void MsgLoopCountRep(CNetMsg::SP& msg, int serverno, int subno, int thisServer, int thisSub, const char* loopbuf);
void MsgExtraGMCommandReq(CNetMsg::SP& msg, int serverno, int subno, MSG_MSGR_TYPE msgtype);
void MsgGMSilenceReq(CNetMsg::SP& msg, int seq, int serverno, int subno, int gmcharindex, const char* charname, int sec);
void MsgGMSilenceRep(CNetMsg::SP& msg, int seq, int thisServer, int thisSub, int success, int gmcharindex, const char* name);
void MsgrRepGMToolWhisperNotFoundMsg(CNetMsg::SP& msg, int nSeq, int nServer, int nSubno, int nZone, int charindex, const char* name);
void ChatGMWhisperMsg(CNetMsg::SP& msg, int sindex, const char* sname, const char* rname, const char* chat);
void MsgrRepGMWhisperRepMsg(CNetMsg::SP& msg, int nSeq, int server, int subno, int zone, int index, const char*  name, const char* chat,  int thisServer, int thisSub, int sindex);
void GMSayMsg(CNetMsg::SP& msg, int server, int sub, int charindex, const char* name, const char* chat, unsigned char chattype);
void MsgrRepGmToolWhisperRepMsg(CNetMsg::SP& msg, int seq, int server, int sub, int zone, int sindex, const char* sname, const char* rname, const char *chat);
void ConnGMToolCommand(CNetMsg::SP& msg, int gmindex, const char* gmname, const char* gmcommand);
void ConnGMToolChatMonitor(CNetMsg::SP& msg, int charindex, const char* name, unsigned char chattype, int server, int sub, const char* chat);
void ConnGMToolChatting(CNetMsg::SP& msg, int server, int sub, int gmindex, const char* gmname, const char* name, const char* chat);
void MsgGmKickCommandReqMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, int thisServer, int thisSubno, int charindex, const char* name);
void MsgGmKickCommandRepMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, char success, int charindex, const char* name);
void MsgrGMShutdownReqMsg(CNetMsg::SP& msg, int server, int sub, int remain, const char* chat);
#endif // GMTOOL

void MsgrChannelChatReqMsg(CNetMsg::SP& msg, int server, int sender_index, const char*  name, const char* chat, unsigned char chatType);

void HelperInsertGuardMsg(CNetMsg::SP& msg, CPC * ch, CItem * item);
void InsertGuardMsg(CNetMsg::SP& msg, CPC * ch, CItem * item);
void CastleCristalRespondStartMsg(CNetMsg::SP& msg, CPC * ch);
void CastleCristalRespondEndMsg(CNetMsg::SP& msg, CPC * ch);
void CastleCristalRespondFailMsg(CNetMsg::SP& msg, CPC * ch);
void CastleTowerListMsg(CNetMsg::SP& msg, CDratanCastle * pCastle);
void CastleTowerAddMsg(CNetMsg::SP& msg, CDratanCastle * pCastle);
void HelperCastleTowerAddMsg(CNetMsg::SP& msg, int zoneidx, CDratanCastle * pCastle);
void CastleTowerReinforceListMsg(CNetMsg::SP& msg, char type, CDratanCastle * pCastle);
void CastleTowerReinforceMsg(CNetMsg::SP& msg, char type, CDratanCastle * pCastle);
void HelperCastleReinforceMsg(CNetMsg::SP& msg, char type, char step, int zoneidx);
void CastleTowerWarpListMsg(CNetMsg::SP& rmsg, int * aliveindex, char total);
void CastleTowerRepaireListMsg(CNetMsg::SP& msg, int idx, LONGLONG money);
void CastleTowerRepaireMsg(CNetMsg::SP& msg, int idx, LONGLONG money);
void CastleTowerRebrithInstallMsg(CNetMsg::SP& msg, int qindex, int gindex, const char * gname, int list_index);
void HeplerCastleRebrithInstallMsg(CNetMsg::SP& msg, int qindex, int gindex, const char * ganme);
void CastleTowerQuartersCrushMsg(CNetMsg::SP& msg, int index);
void CastletowerQuartersListMsg(CNetMsg::SP& msg, CDratanCastle * pCastle);
void WaitTimeMsg(CNetMsg::SP& msg, int wait_time);
void CastleErrorMsg(CNetMsg::SP& msg, MSG_EX_CASTLE_ERROR_TYPE error);
void CastleErrorMsg(CNetMsg::SP& msg, unsigned char error);

void EventOpenAdultServerMsg(CNetMsg::SP& msg, MSG_EVENT_OPEN_ADULT_SERVER_TYPE type);

void PetNameChange( CNetMsg::SP& msg, MSG_EX_PET_CHANGE_NAME_ERROR_TYPE err, int petidx , const char* strPetName );
void HelperPetNameChange( CNetMsg::SP& msg, int charindex, int petindex, const char* strPetName );

void HelperPetColorChange( CNetMsg::SP& msg, int charindex, int petindex, char petCorlorType );
void PetChangeColor( CNetMsg::SP& msg, int petindex, char petColorType, int ownerindex, char petMount );


#ifdef DOUBLE_ITEM_DROP
void MsgrDoubleItemEventReqMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, int charindex, int cmd, int itemPercent);
void MsgrDoubleItemEventRepMsg(CNetMsg::SP& msg, int seq, int server, int subno, int zone, int charindex, int cmd, int itemPercent);
#endif // DOUBLE_ITEM_DROP

#if defined( EVENT_SUMMER_VACATION_2007_RESULT )
void EventSummerVacationInchenMsg( CNetMsg::SP& msg, MSG_EVENT_SUMMER_VACATION_2007_TYPE vacationType, int itemindex );
void EventSummerVacation2007FigureMsg( CNetMsg::SP& msg, MSG_EVENT_SUMMER_VACATION_2007_TYPE vacationType, int figuretype, int itemindex, int count );
#endif// defined( EVENT_SUMMER_VACATION_2007_RESULT )

void EventHalloween2007Msg( CNetMsg::SP& msg, MSG_EVENT_HALLOWEEN_2007_TYPE type, unsigned char error );
void EventHalloween2007Msg( CNetMsg::SP& msg, MSG_EVENT_HALLOWEEN_2007_TYPE type );
void HelperHalloween2007Msg( CNetMsg::SP& msg, int char_index );

void EventNewMoonStoneMsg( CNetMsg::SP& msg, MSG_EVENT_NEW_MOONSTONE_TYPE type);

void CastleOwnerInfoMsg(CNetMsg::SP& msg);

void DVDInfoMsg(CPC * pPC, CNetMsg::SP& msg, int nMobRate, int nEnvRate );
void GuildNameColorStateMsg( CNetMsg::SP& msg, CPC* ch );
void DVDManagementMsg( CNetMsg::SP& msg, MSG_DVD_INFO_SUBTYPE subtype );
void DVDDungeonEnterMsg( CNetMsg::SP& msg, MSG_DVD_INFO_SUBTYPE subtype );
void HelperDVDRateChangeMsg(CNetMsg::SP& msg, MSG_DVD_INFO_SUBTYPE subtype, int nRate, int mode);
void HelperDVDNormalChangeNoticeMsg(CNetMsg::SP& msg);
void HelperDVDNormalChangeTimeMsg(CNetMsg::SP& msg, int time);
void DVDDungeonNormalChangeNoticeMsg(CNetMsg::SP& msg);
void DVDDungeonChangeToOwnerMsg( CNetMsg::SP& msg );

void ConnEventXmas2007Msg( CNetMsg::SP& msg, MSG_CONN_EVENT_XMAS_2007_TYPE subtype );
void EventXmas2007Msg( CNetMsg::SP& msg, MSG_EVENT_XMAS_2007_TYPE subtype );
void EventXmasPuzzleMsg( CNetMsg::SP& msg, MSG_EVENT_XMAS_2007_TYPE subtype, int result = -1);
void EventXmasPuzzleRes( CNetMsg::SP& msg, unsigned char result, unsigned char winCount, unsigned char userChoice, unsigned char serverChoice);
void EventXmasPuzzleEnd( CNetMsg::SP& msg, unsigned char winCount, int giftIndex = -1, LONGLONG giftCount = -1 );

void HelperPetTurnToNPCMsg( CNetMsg::SP& msg, int charindex, int petindex, int toNpc, int size );
void PetTurnToNPCMsg( CNetMsg::SP& msg, int petindex, int toNpc, int ownerindex , int size);

#ifdef GM_EXP_LIMIT_AUTO
void MsgrSetExpLimitMsg( CNetMsg::SP& msg, int server,  int charindex, int nLimitStart, int nLimitEnd, int start[], int end[] );
#else
void MsgrSetExpLimitMsg( CNetMsg::SP& msg, int server,  int charindex, int nLimit );
#endif // GM_EXP_LIMIT_AUTO

void HelperComboGotoWaitRoomPrompt(CNetMsg::SP& msg, int bossindex, const char* bossname);
//void ComboRecallToWaitRoomPrompt(CNetMsg::SP& msg, int bossindex, int nas);
void HelperRecallToWaitRoomPrompt(CNetMsg::SP& msg, int bossindex);
void EffectFireReadyMsg(CNetMsg::SP& msg, int extra, int count, int* effectNPC, float* x, float* z, float* h);
void HelperCreateComboMsg(CNetMsg::SP& msg, int subserver, int charindex, int areaindex, int comboindex);

#if defined (SAKURA_EVENT_2008_REWARD)
void EventSakuraEvent2008Msg( CNetMsg::SP& msg, MSG_EVNET_SAKURA_2008_TYPE type );
#endif // defined (SAKURA_EVENT_2008_REWARD)

void ConnClientRestartMsg( CNetMsg::SP& msg , const char* userid );

#ifdef CHAOSBALL
void ChaosBallMsg(CNetMsg::SP& msg, MSG_EX_CHAOSBALL_ERRORTYPE error);
#endif


void HelperEventPhoenixMsg(CNetMsg::SP& msg, int nUserIdx, int nCharIdx );
void EventPhoenixMsg(CNetMsg::SP& msg, MSG_EVENT_PHOENIX_ERROR_TYPE eError);
void EventPhoenix_SuccessMsg(CNetMsg::SP& msg);

#ifdef EXTREME_CUBE
void HelperCubeStateReqMsg( CNetMsg::SP& msg, int charindex);
void HelperCubeStatePersonalReqMsg(CNetMsg::SP& msg, int charindex);
void HelperAddCubePointMsg(CNetMsg::SP& msg, int ofguildindex, int ofcubepoint, long lastcubepoint);
void HelperAddCubePointPersonalMsg(CNetMsg::SP& msg, int ofcharidx ,int ofcubepoint, long lastCubePoint);
void HelperGuildCubeNoticeMsg(CNetMsg::SP& msg, int type);
void ExtremeCubeErrorMsg(CNetMsg::SP& msg, MSG_EX_EXTREME_CUBE_ERROR_TYPE errortype);
void ExtremeCubeMsg(CNetMsg::SP& msg, MSG_EX_EXTREME_CUBE_TYPE type);
#ifdef EXTREME_CUBE_VER2
void HelperCubeRewardPersonalReqMsg( CNetMsg::SP& msg, int charindex);
void HelperCubeRewardPersonalRollBackMsg(CNetMsg::SP& msg, int updatetime, int charindex);
#endif // EXTREME_CUBE_VER2
#endif

#if defined(EVENT_WORLDCUP_2010) || defined(EVENT_WORLDCUP_2010_TOTO)  || defined(EVENT_WORLDCUP_2010_TOTO_STATUS) || defined(EVENT_WORLDCUP_2010_TOTO_GIFT) || defined(EVENT_WORLDCUP_2010_ATTENDANCE) || defined(EVENT_WORLDCUP_2010_KOREA)
void EventWorldcup2010TradeErrorMsg(CNetMsg::SP& msg, MSG_EVENT_WORLDCUP2010_ERROR_TYPE eError, int tradeType);
void EventWorldcup2010ErrorMsg(CNetMsg::SP& msg, MSG_EVENT_WORLDCUP2010_TYPE repType, MSG_EVENT_WORLDCUP2010_ERROR_TYPE eError);
void EventWorldcup2010TOTOStatusErrorMsg(CNetMsg::SP& msg, MSG_EVENT_WORLDCUP2010_ERROR_TYPE eError, int resultType, int itemindex=-1);
void EventWorldcup2010KoreaErrorMsg(CNetMsg::SP& msg, MSG_EVENT_WORLDCUP2010_ERROR_TYPE eError);
void HelperWorldcup2010TOTORepMsg(CNetMsg::SP& msg, int charindex, int CountryItemIndex1);
void HelperWorldcup2010TOTOStatusRepMsg(CNetMsg::SP& msg, int charindex, int resultType);
void HelperWorldcup2010TOTOGiftRepMsg(CNetMsg::SP& msg, int charindex, int restore=0);
void HelperWorldcup2010AttendanceRepMsg(CNetMsg::SP& msg, int charindex);
void HelperWorldcup2010KoreaRepMsg(CNetMsg::SP& msg, int charindex, int type);
#endif

void InitSSkillMsg(CNetMsg::SP& msg, MSG_EX_INIT_SSkill_ERROR_TYPE type);

typedef struct ItemOption
{
	int	m_nOptionCnt;

	int m_Option_type[MAX_ITEM_OPTION];
	int m_Option_level[MAX_ITEM_OPTION];

	ItemOption(void)
	{
		m_nOptionCnt = 0;

		for(int i=0; i < MAX_ITEM_OPTION; i++)
		{
			m_Option_type[i] = 0;
			m_Option_level[i] = 0;
		}
	}
} S_ITEMOPTION;

#ifdef DISCONNECT_HACK_CHARACTER  // 핵 프로그램 유저 방출
void HelperDisconnectHackCharacter(CNetMsg::SP& msg, int nUserIndex, const char* user_id, int nCharIndex, const char* char_id, int nHack_type, float delay);
#endif // DISCONNECT_HACK_CHARACTER

void ExpedErrorMsg(CNetMsg::SP& msg, unsigned char errorType);
void HelperExpedCreateReqMsg(CNetMsg::SP& msg,int nBossIndex,CLCString BossName);
void HelperExpedCreateResultMsg(CNetMsg::SP& msg, int nResult, int nBossIndex);
void HelperExpedInviteReqMsg(CNetMsg::SP& msg, int nBossIndex, CLCString strBossName, int nTargetIndex,CLCString strTargetName);
void HelperExpedAllowReqMsg(CNetMsg::SP& msg, int nBossIndex, int nAllowIndex, CLCString strAllowCharName, int nAllowLevel );
void HelperExpedRejectReqMsg(CNetMsg::SP& msg, int nBossIndex, int nTargetIndex);
void HelperExpedKickReqMsg(CNetMsg::SP& msg, int nBossIndex, int nTargetIndex);
void HelperExpedQuitReqMsg(CNetMsg::SP& msg, int nBossIndex, int nTargetIndex,int nQuitMode);
void HelperExpedChangeBossReqMsg(CNetMsg::SP& msg, int nBossIndex, int nChangeMode, int nNewBossIndex);
void HelperExpedChangeTypeReqMsg(CNetMsg::SP& msg, int nBossIndex, char cExpedType,char cDiviType);
void HelperExpedEndExpedMsg(CNetMsg::SP& msg, int nBossIndex);
void HelperExpedSetMBossReqMsg(CNetMsg::SP& msg, int nBossIndex, int nNewMBossIndex);
void HelperExpedResetMBossReqMsg(CNetMsg::SP& msg, int nBossIndex, int nNewMBossIndex);
void HelperExpedMoveGroupReqMsg(CNetMsg::SP& msg, int nBossIndex, int nSourceGroup, int nMoveCharIndex, int nTargetGroup, int nTargetListindex);
void HelperExpendOfflineMsg( CNetMsg::SP& msg, int nBossIndex, int nTargetIndex );
void HelperExpendOnlineMsg( CNetMsg::SP& msg, int nBossIndex, int nTargetIndex, int nTargetLevel );
void ExpendOfflineMsg(CNetMsg::SP& msg);

void ExpedCreateRepMsg(CNetMsg::SP& msg, char cExpedType1, char cExpedType2, char cExpedType3, int nBossIndex,CLCString BossCharName,int nMemberCount,int *pCharIdex,CLCString* pCharName,int* pGroupType,int* pMemberType, int* pSetLabelType,int* pQuitType);
void ExpedInviteRepMsg(CNetMsg::SP& msg, char cExpedType1, char cExpedType2, char cExpedType3, int nBossIndex, CLCString strBossName);
void ExpedAddMsg(CNetMsg::SP& msg, int nCharIndex, const char* strCharName,int nCharGroup,int nCharMember, int nCharListIndex, CPC* tch);
void ExpedInfoMsg(CNetMsg::SP& msg, CPC* ch, int nGroup, bool isOnline);
void ExpedQuitRepMsg(CNetMsg::SP& msg, int nTargetIndex, int nQuitMode);
void ExpedQuitReqMsg(CNetMsg::SP& msg, int nQuitMode);
void ExpedKickMsg(CNetMsg::SP& msg, int nTargetIndex);
void ExpedMsg(CNetMsg::SP& msg, int nSubtype);
void ExpedRejectReqMsg(CNetMsg::SP& msg);
void ExpedEndExpedStartMsg(CNetMsg::SP& msg);
void ExpedChangeBoss(CNetMsg::SP& msg, int nBossIndex,  int nNewBossIndex, int nChangeMode);
void ExpedTypeinfo(CNetMsg::SP& msg, char cType1, char cType2, char cType3);
void ExpedChangeType(CNetMsg::SP& msg, char cExpedType, char cDiviType);
void ExpedSetMbossMsg(CNetMsg::SP& msg, int nNewMBossIndex);
void ExpedResetMbossMsg(CNetMsg::SP& msg, int nNewMBossIndex);
void ExpedMoveGroupRepMsg(CNetMsg::SP& msg,int nSourceGroup, int nMoveCharIndex, int nTargetGroup, int nTargetListindex);
void ExpedSetLabelRepMsg(CNetMsg::SP& msg,int nType, int nMode, int nLabel,int nDestIndex);
bool ExpedViewDail_InvenMsg(CNetMsg::SP& msg, CPC* ch);
void ExpedViewDail_ExPetMsg(CNetMsg::SP& msg, CPet* pet);
void ExpedViewDail_ExAPetMsg(CNetMsg::SP& msg, CAPet* pet);
void ExpedSearchTriggerItemMsg(CNetMsg::SP& msg, int nSubtype);
void ExpedAddSysMsg(CNetMsg::SP& msg, const char* strCharName);

void FactoryLearnMsg(CNetMsg::SP& msg, CFactoryProto * pFactory);
void FactoryListMsg(CNetMsg::SP& msg, CPC* ch, int nSkillIdx = -1);
void FactoryErrorMsg(CNetMsg::SP& msg, MSG_FACTORY_ERROR_TYPE errorcode);
void FactoryMakeMsg(CNetMsg::SP& msg, CPC* ch, CFactoryProto * pFactory);

void TriggerEvnetMsg(CNetMsg::SP& msg, MSG_EX_TRIGGER_EVENT_TYPE type, int touch_ID, int Play_ID, int bContinue);
void HelperTriggerEvnetMsg(CNetMsg::SP& msg, int nZoneIdx, int nAreaIdx, int nSaveInfo);
void RaidInfoMsg(CNetMsg::SP& msg, int nRaidCount, int* nZoneNum, int* nRoomNum);
void HelperDeleteRaidCharacterMsg(CNetMsg::SP& msg, int nCharIndex);
void HelperRaidInfoMsg(CNetMsg::SP& msg, int nCharIndex);

void RaidErrorMsg(CNetMsg::SP& msg, unsigned char errorType,int nErrData1=-1, int nErrData2=-1);
void RaidInzoneQuitReq(CNetMsg::SP& msg, int nGoZone,int nRebirth);
void HelperRaidInzoneGetRoomNoReqMsg(CNetMsg::SP& msg, int nCharindex, int nZoneNo, int nBossIndex,int nJoinType);
void HelperRaidInzoneGameDataClearReqMsg(CNetMsg::SP& msg, int nZone);
void HelperRaidInzoneSetNo(CNetMsg::SP& msg, int nNewBosIndex, int nOldBossIndex, int nZoneNum, int nJoinType, int difficulty = -1 );

void AffinityMsg(CNetMsg::SP& msg, MSG_EX_AFFINITY_TYPE type);
void AffinityErrorMsg(CNetMsg::SP& msg, MSG_EX_AFFINITY_ERROR_TYPE errortype);
void AffinityUpdatePointMsg(CNetMsg::SP& msg, int affinityidx, int point, bool bBoostered, int bonus = 0);

void AffinityAddInfoMsg(CNetMsg::SP& msg, int affinityidx, int maxpoint);
void AffinityListInfoMsg(CNetMsg::SP& msg, CPC* ch);

void AffinityRewardNoticeMsg(CNetMsg::SP& msg, int noticeCount, int* npcidx);

void SubHelperConnectMsg(CNetMsg::SP& msg, int version, int server, int subno, int count, int* zones);
void SubHelperShutdownMsg(CNetMsg::SP& msg);

void NpcPortalLoadingEndUseMsg(CNetMsg::SP& msg);
void NpcPortalListMsg(CNetMsg::SP& msg, int zoneNum);
void NpcPortalListErrorMsg(CNetMsg::SP& msg, int ErrorType);

void NewTutorialMsg(CNetMsg::SP& msg);

void LuckyDrawBoxErrorMsg(CNetMsg::SP& msg, int ErrorType);

void DBNSCreateMsg(CNetMsg::SP& msg);
void HelperNSCreateCardUse(CNetMsg::SP& msg, MSG_HELPER_NS_CARD_TYPE subtype, int userindex, int charindex);

void PartyRecallErrorMsg(CNetMsg::SP& msg, MSG_EX_PARTY_RECALL_TYPE type);

void TitleItemUseMsg(CNetMsg::SP& msg, int titleindex, int endtime);
void TitleItemCantUseMsg(CNetMsg::SP& msg, int titleindex);
void SubHelperTitleSystemInsertReq(CNetMsg::SP& msg, int char_index, int title_index, int endtime, int tab, int invenIndex, CLCString serial);
void TitleSystemListRep(CNetMsg::SP& msg, MSG_EX_TITLE_SYSTEM_TYPE type, int count);
void SubHelperTitleSystemCheckTimeReq(CNetMsg::SP& msg, int char_index, CTitleNode* temp);
void TitleSystemMsg(CNetMsg::SP& msg, MSG_EX_TITLE_SYSTEM_TYPE type, int title_index, int char_index);
void SubHelperTitleSystemDeleteReq(CNetMsg::SP& msg, int char_index, int title_index);
void SubHelperTitleSystemTitleDelReq(CNetMsg::SP& msg, int char_index, int title_index);
// [selo: 101105] 종료 시간 강제 변경
void TitleSysstemForceChangeTimeMsg(CNetMsg::SP& msg, MSG_EX_TITLE_SYSTEM_TYPE type, int title_index, int endtime);

void TakeAgainQuestItemMsg(CNetMsg::SP& msg, MSG_EX_TAKE_AGAIN_QUEST_ITEM_TYPE type);

#ifdef LACARETTE_SYSTEM
void ConnLacaretteTokenReadyMsg(CNetMsg::SP& msg, int userIndex, int charIndex);
void ConnLacaretteTokenMsg(CNetMsg::SP& msg, int userIndex, int charIndex);
void ConnLacaretteTokenAddMag(CNetMsg::SP& msg, int userIndex, int charIndex, int bAdd=1);
void ConnLacaretteRepeatMsg(CNetMsg::SP& msg, int userIndex, int charIndex, int cosArrNum, int cosIndex, int tokenIndex);
void ConnLacaretteRepeatAddMsg(CNetMsg::SP& msg, int userIndex, int charIndex, int giveItemIdx, int bAdd=1);
void ConnLacaretteUIMsg(CNetMsg::SP& msg, int userIndex, int charIndex, int cosArrNum, int cosIndex, int tokenIndex);

void lacaretteTokenReadyErrorMSg(CNetMsg::SP& msg, MSG_EX_LACARETTE_TOKEN_ERROR_TYPE type );
void lacaretteTokenErrorMSg(CNetMsg::SP& msg, MSG_EX_LACARETTE_TOKEN_ERROR_TYPE type );
void lacaretteRetteErrorMSg(CNetMsg::SP& msg, MSG_EX_LACARETTE_RETTE_ERROR_TYPE type );
void lacaretteRetteErrorSucMSg(CNetMsg::SP& msg, int giveitemArrNum, int giveitemIndex); //, int courseArrNum, int courseIndex);
void lacaretteResultErrorMSg(CNetMsg::SP& msg, MSG_EX_LACARETTE_RESULT_ERROR_TYPE type );
void lacaretteResultErrorSucMSg(CNetMsg::SP& msg, int repeat);
void lacaretteUIrepMsg(CNetMsg::SP& msg, int repeat);
void lacaretteJackpotMsg(CNetMsg::SP& msg, int itemIndex, const char* chName );
#endif
void SysExpireCostumeItem(CNetMsg::SP& msg, int index);
void DungeonTimeNoticeMsg(CNetMsg::SP& msg, MSG_EX_DUNGEONTIME_TYPE type, int start, int end, int exp);	// 던전타임.

void SocketMsg(CNetMsg::SP& msg, MSG_EX_SOCKET_TYPE type);
void SocketErrorMsg(CNetMsg::SP& msg, MSG_EX_SOCKET_ERROR_TYPE type);
void ItemSocketMsg(CNetMsg::SP& msg, const CItem *item);

void tutorialrenewerMsg(CNetMsg::SP& msg, int index);

void RaidSceneMsg(CNetMsg::SP& msg, int TodoOutput, int object_type, int object_index, int object_data);
void SendOutputMsg(CNetMsg::SP& msg, int type, int index, int data);
void SendRaidSceneObjectStateMsg(CNetMsg::SP& msg, CPC* ch);
void SendRaidSceneObjectRefreshMsg(CNetMsg::SP& msg, CPC* ch, int object_type, int index, int state);
void SendRaidScenePadoxSkillAction(CNetMsg::SP& msg);
void SendRaidSceneCountDown(CNetMsg::SP& msg, int count);

#ifdef ENABLE_SUBJOB
void SubJobMsg( CNetMsg::SP& msg, MSG_EX_SUBJOB_TYPE type );
void SubJobErrorMsg( CNetMsg::SP& msg, int error );
#endif //ENABLE_SUBJOB

void CastllanErrorMsg( CNetMsg::SP& rmsg, MSG_EX_CASTLLAN_ERROR_TYPE errorType );

#ifdef DEV_EVENT_PROMOTION2
void EventPromotion2RepErrorMsg(CNetMsg::SP& msg, int errorType);
void ConnPromotion2CouponUseReqMsg(CNetMsg::SP& msg, int userIndex, int charIndex,const char* couponNum );
void ConnPromotion2CouponItemReqMsg(CNetMsg::SP& msg, int userIndex, int charIndex, int keyIndex );
#endif

void MonsterMercenarySummonMsg(CNetMsg::SP& msg, int index, char enable );
void MonsterMercenaryErrorMsg(CNetMsg::SP& msg, MSG_EX_MONSTER_MERCENARY_ERROR_TYPE errorType );

void MsgCashshopTypeListRep(CNetMsg::SP& msg, int t_count, int type_list[], int st_count, int subtype_list[]);
void MsgCashshopListRep(CNetMsg::SP& msg, int search_depth, int ct_count, int ctid[], int depth[], int remain_count[], int price[], int now_page, int total_page);
void MsgCashshopRecommandRep(CNetMsg::SP& msg, int ct_count, int ctid[], int type[], int remain_count[], int price[]);
void MsgCashshopWishlistRep(CNetMsg::SP& msg, int ct_count, int ctid[], int type[], int remain_count[], int price[]);
void MsgCashshopWishlistSaveRep(CNetMsg::SP& msg, int errorcode, int ctid);
void MsgCashshopWishlistDelRep(CNetMsg::SP& msg, int errorcode, int ctid);
void ConnWishlistReq(CNetMsg::SP& msg, CPC* pc);
void ConnWishlistSaveReq(CNetMsg::SP& msg, CPC* pc, int ctid);
void ConnWishlistDelReq(CNetMsg::SP& msg, CPC* pc, int ctid);
void MsgCashShopLock(CNetMsg::SP& msg, MSG_EX_CASHITEM_TYPE type);

void MsgMessageBox(CNetMsg::SP& msg, MSG_EX_MSGBOX_TYPE type);

void LCBallUseMsg(CNetMsg::SP& msg, char errorCode , int nItemIndex );
void LCBallSuccessMsg(CNetMsg::SP& msg, CLCString& strName, int nItemIndex);
void SubHelperLCBallinfoMsg( CNetMsg::SP& msg, int charIndex, int coinIndex, char courseIndex );
void SubHelperLCBalluseMsg( CNetMsg::SP& msg, int charIndex, int coinIndex, char courseIndex );
void SubHelperLCBalluse_successMsg( CNetMsg::SP& msg, char error, CLCString name, int coinIndex, char courseIndex , char cOrder, int itemIndex  );

#ifdef EVENT_USER_COMEBACK
void ConnUserComebackGiftReqMsg(CNetMsg::SP& msg, int userindex, int charindex);
void EventUserComebackErrorMsg(CNetMsg::SP& msg, MSG_EVENT_USER_COMEBACK_ERROR_TYPE errortype );
#endif

#ifdef EVENT_CHAR_BIRTHDAY
void HelperCharBirthdayReqMsg(CNetMsg::SP& msg, MSG_EVENT_CHAR_BIRTHDAY_TYPE subtype ,int charindex );
void EventCharBirthdayErrorMsg(CNetMsg::SP& msg, MSG_EVENT_CHAR_BIRTHDAY_ERROR_TYPE errortype);
#endif

#ifdef EVENT_CHAR_BIRTHDAY

#endif

#ifdef EVENT_EUR2_PROMOTION
void ConnEUR2PromotionMsg(CNetMsg::SP& msg, int CharIndex, int UserIndex, CLCString& Key, unsigned char bInvenSpace, unsigned char bWeight);
void EventEUR2PromotionMsg(CNetMsg::SP& msg, MSG_EVENT_EUR2_PROMOTION_ERROR_TYPE type);
#endif

void NpcNoticeMsg(CNetMsg::SP& msg, MSG_CHAT_TYPE chatType, int npcIndex, const char* chat);
#ifdef DEV_GUILD_MARK
void HelperGuildMarkRegistReqMsg(CNetMsg::SP& msg, int GuildIndex, int CharIndex, char gm_row, char gm_col, char bg_row, char bg_col, unsigned short tab, unsigned short invenIndex, int markTime, const char* itemserial);
void GuildMarkEditFinMsg(CNetMsg::SP& msg, char gm_row, char gm_col, char bg_row, char bg_col, int markTime);
void GuildMarkExpireMsg(CNetMsg::SP& msg);
#endif

#ifdef DEV_GUILD_STASH
void GuildStashListMsg( CNetMsg::SP& msg, CLCString& timeStamp, int capacity, GoldType_t money, int itemCount );
void GuildStashErrorMsg( CNetMsg::SP& msg, unsigned char subtype, char error );
void GuildStashLogMsg( CNetMsg::SP& msg , int logCount );
void HelperGuildStashCreateMsg( CNetMsg::SP& msg , int guildIndex, int charIndex );
void HelperGuildStashListMsg( CNetMsg::SP& msg , int guildIndex, int charIndex );
void HelperGuildStashKeepMsg( CNetMsg::SP& msg , int guildIndex, int charIndex, LONGLONG keepMoney, int itemCount );
void HelperGuildStashKeepErrorMsg( CNetMsg::SP& msg , int charIndex, int guildIndex, char error, LONGLONG money = 0 );
void HelperGuildStashTakeMsg( CNetMsg::SP& msg , int guildIndex, int charIndex, LONGLONG takeMoney, int itemCount );
void HelperGuildStashTakeErrorMsg(  CNetMsg::SP& msg , int charIndex, int guildIndex, char error, LONGLONG money = 0 );
void HelperGuildStashLogMsg( CNetMsg::SP& msg , int charIndex, int guildIndex, int lastLogIdx );
void HelperGuildStashKeepMoneyMsg( CNetMsg::SP& msg , int guildIndex, int charIndex, int itemCount );
void HelperGuildStashTakeMoneyMsg( CNetMsg::SP& msg , int guildIndex, int charIndex, int itemCount );
#endif // DEV_GUILD_STASH

void SysRaidZoneRemain(CNetMsg::SP& msg, int remain);
void HelperAutoResetRaid(CNetMsg::SP& msg, int index, int zone, int area[][2]);

#ifdef WARCASTLE_STATE_CHANNEL_CYNC
void HelperWarCastleStateCync(CNetMsg::SP& msg);
#endif

void MsgrRoyalRumbleStart(CNetMsg::SP& msg, int seq, int server, int subno, int zone, MSG_MSGR_TYPE type, int cmd);
void MsgrRoyalRumbleWinner(CNetMsg::SP& msg, int seq, int server, int subno, int zone, int leveltype, int charindex, CLCString nick);
void RoyalRumbleNotice(CNetMsg::SP& msg, unsigned char command);
void RoyalRumbleRegistMenu(CNetMsg::SP& msg, unsigned char command);
void RoyalRumbleUnRegist(CNetMsg::SP& msg, unsigned char command);
void RoyalRumbleRegist(CNetMsg::SP& msg, unsigned char command);
void RoyalRumbleRegistResult(CNetMsg::SP& msg, unsigned char command);
void RoyalRumbleGoZone(CNetMsg::SP& msg);
void RoyalRumbleEnterNextPlayer(CNetMsg::SP& msg, int second);
void RoyalRumbleHold(CNetMsg::SP& msg, unsigned char boolean);
void RoyalRumbleWinner(CNetMsg::SP& msg, int leveltype, int charindex, CLCString nick);
void RoyalRumbleReward(CNetMsg::SP& msg, unsigned char command);
void RoyalRumbleWarPoint(CNetMsg::SP& msg, int warpoint, int waraccpoint);
void RoyalRumbleTimeReq(CNetMsg::SP& msg, int time);
void RoyalRumbleLeftCount(CNetMsg::SP& msg, int count);
void RoyalRumbleNotfoundItem(CNetMsg::SP& msg);
void RoyalRumbleKillPlayer(CNetMsg::SP& msg, CPC* opc, CPC* dpc);
void RoyalRumbleTimeNotice(CNetMsg::SP& msg, unsigned char command, int remain);

#ifdef SYSTEM_TREASURE_MAP
void MsgrSetTreasureMap( CNetMsg::SP& msg, int server, int charindex, const char* arg);
#ifdef SYSTEM_TREASURE_MAP_GAMIGO
void MsgTreaserMapKeyExchangeError( CNetMsg::SP& msg, MSG_EX_TRS_MAP_KEY_EXCHANGE_ERROR_TYPE errorType );
#endif // SYSTEM_TREASURE_MAP_GAMIGO
#endif // SYSTEM_TREASURE_MAP

#ifdef GM_COMMAND_NOTICE
void MsgSetGMCommandNotice( CNetMsg::SP& msg, MSG_GM_COMMAND_NOTICE_TYPE Type, CLCString gm, CLCString user );
#endif

#ifdef GUILD_MEMBER_KICK_JOIN_DELAY
void HelperGuildKickOutDateUpdateReqMsg(CNetMsg::SP& msg, int charindex, int outDate );
#endif

void SubHelperLevelup(CNetMsg::SP& msg, int char_index);
void SubHelperRankingRefreshEx(CNetMsg::SP& msg, int char_index);
void SubHelperRankingListReqEx(CNetMsg::SP& msg, unsigned char type, unsigned char subtype, int char_index);
void SubHelperRankingSearchReqEx(CNetMsg::SP& msg, unsigned char type, unsigned char subtype, int char_index, CLCString name);
void SubHelperRankingRewardReqEx(CNetMsg::SP& msg, int endTime);
void RankingEXList(CNetMsg::SP& msg, int listType, unsigned char type, unsigned char subtype);
void RankingEXRefresh(CNetMsg::SP& msg);
void RankingEXError(CNetMsg::SP& msg, unsigned char type);

#ifdef REFORM_PK_PENALTY_201108 // 2011-08 PK 패널티 리폼
void MsgPKPenaltyRewardRep(CNetMsg::SP& msg, int rewardNum, int errorType);
void MsgPKPenaltyRewardInfoRep(CNetMsg::SP& msg, int rewardFlag);
void MsgSubHelperPKPenaltyRewardReq(CNetMsg::SP& msg, int charindex, int rewardNum, int titleIndex, int endtime );
void MsgSubHelperPKPenaltyRewardInfoReq(CNetMsg::SP& msg, int charindex);
#endif // REFORM_PK_PENALTY_201108 // 2011-08 PK 패널티 리폼

#ifdef NOTICE_SYSTEM
void SubHelperUserNoticeRegMsg(CNetMsg::SP& msg, int _characterIndex, CLCString _name, CLCString _contents);
void UserNoticePrintMsg(CNetMsg::SP& msg, CLCString _name, CLCString _contents);
void UserNoticeRegRepMsg(CNetMsg::SP& msg);
void UserNoticeErrorMsg(CNetMsg::SP& msg, int _errorType);
#endif // NOTICE_SYSTEM

void HelperGuildMasterKickReq(CNetMsg::SP& msg, int _guildIndex, int _requstCharIndex);
void HelperGuildMasterKickCancelReq(CNetMsg::SP& msg, int _guildIndex, int _requestCharIndex);
void GuildMasterKickRep(CNetMsg::SP& msg, int _guildIndex, CLCString _currentBoss);
void GuildMasterKickCancelRep(CNetMsg::SP& msg, int _guildIndex);
void GuildMasterKickStatus(CNetMsg::SP& msg, int _guildIndex, int _status);
void HelperGuildMasterKickReset(CNetMsg::SP& msg, int _charIndex, int _guildIndex);

void EventActiveListMsg(CNetMsg::SP& msg, std::map<int, CEventInfo*> * _eventList);
void EventActiveUpdateMsg(CNetMsg::SP& msg, unsigned char _subtype, int _eventIndex);
void MsgrEventActiveUpdateMsg(CNetMsg::SP& msg, MSG_EX_EVENT_AUTOMATION_SYSTEM_TYPE _subtype, int _eventIndex, int _notice=0);

void JumpingResultMsg(CNetMsg::SP& msg, MSG_EX_JUMPING_SYSTEM_TYPE _subtype, int _value = -1);

void HelperGuildMasterChange(CNetMsg::SP& msg, int _newBossIndex, int _guildIndex);

void SubHelperCharSlotExtUse(CNetMsg::SP& msg, int _userIndex, int _charIndex, int _tab, int _invenIndex, CLCString _serial, int _days);
void SubHelperCharSlotExtUseResult(CNetMsg::SP& msg, int _userIndex);
void ItemUseCharSlotExt(CNetMsg::SP& msg, unsigned char cmd);
void DBCharSlotExtTime(CNetMsg::SP& msg, long time);

void RecoverExpSp(CNetMsg::SP& msg, LONGLONG recoverEXP, LONGLONG recoverSP);
void SysNpcPortalExpiredMsg(CNetMsg::SP& msg, int itemIndex);

void ReformerSystemRep(CNetMsg::SP& msg, unsigned char cmdType_, CItem *pReformItem_ = NULL);
void MsgrRoyalRumbleSettingReq(CNetMsg::SP& msg, const int min, const int max, const int startHour, const int charindex);
void MsgrSendMsgToCharacterReq(CNetMsg::SP& msg, const int charindex, const char* message);
void NoticeFortuneMsg(CNetMsg::SP& msg, int ret, const int item_index, int skill_index, int skill_level);

#endif // #ifndef __ACMDMSG_H__
