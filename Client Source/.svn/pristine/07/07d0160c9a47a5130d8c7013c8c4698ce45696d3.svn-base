
#ifndef		UI_WINDOW_DECLARE_H_
#define		UI_WINDOW_DECLARE_H_

#include <Common/CommonDef.h>
#include <Engine/Interface/UIBase.h>

// User interface types
enum UI_TYPE
{
	UI_NONE					= -1,
	UI_TYPE_START			= 0,

	UI_ALWAYS_SHOW_START	= UI_TYPE_START,				// 항상 보이는 인터페이스 |<--
	UI_PLAYERINFO			= UI_ALWAYS_SHOW_START,
	//UI_CHATTING,
	UI_CHATTING_NEW,
	UI_PETTARGETINFO,
	UI_WILDPETTARGETINFO,
	UI_QUICKSLOT,
	UI_SUMMON_START,
	UI_SUMMON_FIRST	= UI_SUMMON_START,
	UI_SUMMON_SECOND,
	UI_SUMMON_END	= UI_SUMMON_SECOND,	
	UI_RADAR,
	UI_NOTICE,
	UI_TARGETINFO,	
	UI_HELP_ICON,	
#ifdef USE_UIAD
	UI_AD,													// 선전 문구 표시(for japan)
#endif
	UI_SIMPLE_PLAYER_INFO,
	UI_ALWAYS_SHOW_END,										// 항상 보이는 인터페이스     -->|

	UI_SELECTIVE_SHOW_START	= UI_ALWAYS_SHOW_END,			// 선택적으로 보이는 인터페이스 |<--
	UI_INVENTORY			= UI_SELECTIVE_SHOW_START,
	UI_INVEN_SLOT1,
	UI_INVEN_SLOT2,
	UI_INVEN_SLOT3,
	UI_INVEN_CASH1,
	UI_INVEN_CASH2,
	UI_QUICKSLOT2,
	UI_QUICKSLOT3,
	UI_CHARACTERINFO,
	UI_EXCHANGE,
	UI_PORTAL,
	UI_SHOP,
	UI_PERSONALSHOP,
	UI_WAREHOUSE,
	UI_SECURITY,
	UI_SYSTEMMENU,
	UI_PARTY,
	UI_EXPEDITION,
	UI_EXPEDITION_VIEW,
	UI_GUILD,
	UI_HELP,
	UI_HELP3,
	UI_SINGLE_BATTLE,
	UI_GUILD_BATTLE, // Date : 2005-03-17,   By Lee Ki-hwan
	UI_SIEGE_WARFARE,	// 공성전 // Date : 2005-06-27(오전 10:46:11), By Lee Ki-hwan
	UI_SIEGE_WARFARE_NEW,	// 신공성전 // WSS_DRATAN_SIEGEWARFARE 070727
	UI_GUILDWARPORTAL,
//	UI_BILLING,			// 대만 유료화(정액) Date : 2005-04-30(오후 3:55:19), By Lee Ki-hwan
//	UI_BILL_ITEM,		// 대만 유료화 아이템 지급 시스템
	UI_MAP,
	UI_OPTION,
	UI_REFINE,
	UI_PROCESS,
	UI_PRODUCT,		// 이기환(12.7)	
	UI_MIX,
	UI_MIXNEW,		//  wooss 060523
	UI_COMPOUND,	// Date : 2005-01-12,   By Lee Ki-hwan
//	UI_REMISSION,
	UI_QUEST,
	UI_QUESTBOOK_LIST,
	UI_QUESTBOOK_COMPLETE,
	UI_QUEST_ACCEPT,
	UI_QUEST_RESTORE,
	UI_QUEST_VIEW,
	UI_SKILLLEARN,
	UI_CHANGEWEAPON,
	UI_CHANGE_EQUIPMENT,
	UI_HELPER,
	UI_INITJOB,
	UI_TELEPORT,
	UI_TELEPORT_PRIMIUM,	// [KH_070315] 프리미엄 메모리스크롤 관련 추가
	UI_MESSENGER,
	UI_WEBBOARD,
//	UI_QUIZ,
	UI_PETTRAINING,
	UI_PETINFO,
	UI_RANKING,	
	UI_GAMBLE,
	UI_PROCESSNPC,
	UI_SELECTRESOURCE,
//	UI_CASH_SHOP,			//wooss 050820 
	UI_GW_MIX, // 공성 아이템 조합 
	UI_GUILDSTASH,	// 길드 나스 보관함 
	UI_SELECTWORD,
	UI_SELECTLIST,	// 친구찾기이벤트 060126
	UI_PETITEMMIX,	// 펫 아이템 조합( 유니크 아이템 제작 )
	UI_PETFREE,			// 펫 봉인 해제
	UI_QUIZEVENT,		// O.X QUIZ EVENT
	UI_PARTYAUTO,
	UI_COLLECTBOX,		// 곤충 채집 박스 
	UI_BINGOBOX,		// 러브러브 이벤트 초코렛 채집 박스
	UI_NPCHELP,			// NPC 안내 시스템
	UI_SIMPLE_POP,		// [KH_070419] 심플 팝업 관련 추가
	UI_MINIGAME,		// WSS_MINIGAME 070418
	UI_FLOWERTREE,		// 꽃놀이 인터페이스	

	UI_GUILD_NOTICE,	// WSS_NEW_GUILD_SYSTEM 070715 
	UI_MONSTER_COMBO,	// ttos_080116 : 몬스터 콤보
	UI_WILDPET_INFO,	// 공격 펫 Info
	UI_AUCTION,			// 거래대행
	UI_TATOO,			// [090709: selo] 펫 문양 시스템
	UI_PRODUCT2,		// 아이템 제작
	UI_PRODUCTNPC,		// 아이템 제작 NPC
	UI_NPC_AFFINITYINFO,	// [5/29/2009 rumist] npc affinity. ( npc 친화도. ) 
	UI_ITEMPRODUCT,		// 친화도 UI에서 제작 UI 분리 [2/25/2013 Ranma]
	UI_NPC_AFFINITY,	// [6/3/2009 rumist] npc affinity donation. 기부. 
	UI_RESURRECTION,	// resurrection ui [11/2/2009 rumist]
	UI_CHAT_FILTER,		//[ttos_2009_7_17]: CHARATER_CHAT_FILTER 캐릭터 채팅 필터
	UI_NPC_SCROLL,		// connie [2009/9/9] - NPC 찾기
	UI_NICKNAME,		//[12/9/2009 kiny] 호칭 
	//	UI_TITLESYSTEM,		// Title System UI [11/20/2009 rumist]
	UI_SOCKETSYSTEM,	// Socket System UI [5/11/2010 rumist]
	UI_CUBERANK,
	UI_LACABALL,
	UI_CASHSHOP_EX,

	UI_MONSTER_MERCENARY,	// [2010/10/20 : Sora] 몬스터 용병 카드
	UI_SIEGE_WARFARE_INFO,	// [2010/11/01 : Sora] 공성 개편

	UI_TREASUREMAP,
	UI_ROYALRUMBLE_ICON,			// royal rumble UI [5/11/2011 rumist]
	UI_NEWGUILDSTASH,				// renewal guild stash [6/16/2011 rumist]
	UI_GUILD_MARK,					// [sora] GUILD_MARK
	UI_REFORM_SYSTEM,				// reform system [8/28/2012 박훈]
	UI_FORTUNE,						// [2013/02/13] sykim70 Fortune System
	UI_LCE_SYSTEM,
	UI_CALENDAR,
	UI_CALENDAR_EVENT_INFO,
	UI_MYSTERYOUSBEAD,
	UI_PET_STASH_SELECT_EFFECT,
	UI_PET_STASH,
	UI_INVEN_CASH_BAG_BOX,
	UI_SKILL_NEW,
	UI_HARDCORE_WARNING,
	UI_MYSYNDICATE_INFO, // 결사대 직위 체계 및 정보
#ifdef	DURABILITY
	UI_DURABILITY,
#endif	//	DURABILITY
	UI_TRADE,
	UI_GPS,
	UI_COMPASS,
	UI_MSGBOX_NUMERIC_ONLY,
	UI_ITEMCOLLECTION, 
	UI_PREMIUM_CHAR,
	UI_SIMPLE_PLAYER_MENU,
	UI_NEWS,
	UI_NEWS_WEB,
	UI_ITEM_COMPOSE,
	UI_HELP_WEB,
	UI_SELECTIVE_SHOW_END,									// 선택적으로 보이는 인터페이스     -->|

	UI_LOGIN_SHOW_START		= UI_SELECTIVE_SHOW_END,		// 로그인 인터페이스 |<--
	UI_LOGIN				= UI_LOGIN_SHOW_START,
	UI_CREATE_CHAR,
	UI_SERVER_SELECT,
	UI_CHARACTER_SELECT,
	UI_LOGIN_SHOW_END,										// 로그인 인터페이스     -->|

	UI_MESSENGER_TALK_START = UI_LOGIN_SHOW_END,			// 메신저 대화 상자     -->|
	UI_MESSENGER_TALK1		= UI_MESSENGER_TALK_START,
	UI_MESSENGER_TALK2,
	UI_MESSENGER_TALK3,
	UI_MESSENGER_TALK4,
	UI_MESSENGER_TALK5,
	UI_MESSENGER_TALK6,
	UI_MESSENGER_TALK7,
	UI_MESSENGER_TALK8,
	UI_MESSENGER_TALK9,
	UI_MESSENGER_TALK10,
	UI_MESSENGER_TALK_END,									// 메신저 대화 상자 |<--

	UI_MESSAGEBOX_START		= UI_MESSENGER_TALK_END,		// 메세지 박스 인터페이스 |<--
	UI_MESSAGEBOX1			= UI_MESSAGEBOX_START,
	UI_MESSAGEBOX2,
	UI_MESSAGEBOX3,
	UI_MESSAGEBOX4,
	UI_MESSAGEBOX5,
	UI_MESSAGEBOX6,
	UI_MESSAGEBOX7,
	UI_MESSAGEBOX8,
	UI_MESSAGEBOX9,
	UI_MESSAGEBOX10,
	UI_MESSAGEBOX11,
	UI_MESSAGEBOX12,
	UI_MESSAGEBOX13,
	UI_MESSAGEBOX14,
	UI_MESSAGEBOX15,
	UI_MESSAGEBOX_END,										// 메세지 박스 안터페이스     -->|

	UI_MESSAGEBOXL_START	= UI_MESSAGEBOX_END,			// 대화 상자 인터페이스 |<--
	UI_MESSAGEBOXL1			= UI_MESSAGEBOXL_START,
	UI_MESSAGEBOXL2,
	UI_MESSAGEBOXL3,
	UI_MESSAGEBOXL4,
	UI_MESSAGEBOXL5,
	UI_MESSAGEBOXL_END,										// 대화 상자 안터페이스     -->|

	UI_RANKINGVIEW_START	= UI_MESSAGEBOXL_END,
	UI_RANKINGVIEW_RENEWAL,
	UI_RANKINGVIEW_END,
	UI_TYPE_END				= UI_RANKINGVIEW_END,
};

class CUIBase;
class CUITextBox;
class CUIWindow;

class CUIAffinity;
class CUIAffinityInfo;
class CUIAuctionNew;
class CUIBingoBox;
class CUICalendar;
class CUICalendarEventInfo;
class CUICashShopEX;
class CUIChangeWeapon;
class CUIChangeEquipment;
class CUICharacterCreateNew;
class CUICharacterInfo;
class CUICharacterSelect;
class CUICharServerMove;
class CUIChatFilter;
//class CUIChatting;
class CUIChildInvenSlot;
class CUIChildQuickSlot;
class CUICollectBox;
class CUICompass;
class CUICompound;
class CUICubeRank;
class CUIDurability;
class CUIExchange;
class CUIExpressSystem;
class CUIExpManage;
class CUIFlowerTree;
class CUIGamble;
class CUIGPS;
class CUIGuild;
class CUIGuildBattle;
class CUIGuildMark;
class CUIGuildNotice;
class CUIGuildStash;
class CUIGuildWarPortal;
class CUIGWMix;
class CUIHardCoreWarning;
class CUIHelp;
class CUIHelp3;
class CUIHelper;
class CUIHelpIcon;
class CUIInitJob;
class CUIInvenCashBag;
class CUIInvenCashBagBox;
class CUIInventory;
class CUIItemProduct;
class CUILacaball;
class CUILoginNew;
class CUIMap;
class CUIMessageBox;
class CUIMessageBoxL;
class CUIMessenger;
class CUIMinigame;
class CUIMix;
class CUIMixNew;
class CUIMonsterCombo;
class CUIMonsterMercenary;
class CUIMouseCursor;
class CUIMsgBox_Info;
class CUIMsgBoxNumericOnly;
class CUIMysteriousBead;
class CUIMySyndicateInfo;
class CUINewGuildStash;
class CUINickName;
class CUINoticeNew;
class CUINpcHelp;
class CUINpcScroll;
class CUIOption;
class CUIOXQuizEvent;
class CUIParty;
class CUIPartyAuto;
class CUIPersonalShop;
class CUIPetFree;
class CUIPetInfo;
class CUIPetItemMix;
class CUIPetStash;
class CUIPetStashSelectEffect;
class CUIPetTargetInfo;
class CUIPetTraining;
class CUIPlayerInfo;
class CUIPortal;
class CUIProcess;
class CUIProcessNPC;
class CUIProduct;
class CUIProduct2;
class CUIProductNPC;
class CUIQuest;
class CUIQuestBook;
class CUIQuestAccept;
class CUIQuestComplete;
class CUIQuestRestore;
class CUIQuestView;
class CUIQuickSlot;
class CUIRadar;
class CUIRanking;
class CUIRankingSystem;
class CUIRefine;
class CUIReformSystem;
class CUIResurrectionMsgBox;
class CUIRoyalRumbleIcon;
class CUISecurity;
class CUISelectList;
class CUISelectResource;
class CUISelectWord;
class CUIServerSelect;
class CUIShop;
class CUISiegeWarfare;
class CUISiegeWarfareInfo;
class CUISiegeWarfareNew;
class CUISimplePop;
class CUISingleBattle;
class CUISkillLearn;
class CUISkillNew;
class CUISocketSystem;
class CUISummon;
class CUISystemMenu;
class CUITalk;
class CTargetInfoUI;
class CUITatoo;
class CUITeleport;
class CUITeleportPrimium;
class CUITrade;
class CUITreasureMap;
class CUIViewDetail;
class CUIWareHouse;
class CUIWebBoard;
class CUIWildPetInfo;
class CUIWildPetTargetInfo;
class CItemCollectionUI;
class CPremiumCharUI;
class CSimplePlayerInfoUI;
class CSimplePlayerMenuUI;
class CChattingUI;
class CNewsUI;
class CNewsWebUI;
class CItemComposeUI;
class CHelpWebUI;

class CUIWindowDeclare : public CUIBase
{
public:

#ifdef	USE_UIAD
	CUIAD*					GetAD() { return (CUIAD*)m_apUIs[UI_AD]; }
#endif	// USE_UIAD
	CUIAffinity*			GetAffinity()	{ return (CUIAffinity*)m_apUIs[UI_NPC_AFFINITY];}
	CUIAffinityInfo*		GetAffinityInfo() { return (CUIAffinityInfo*)m_apUIs[UI_NPC_AFFINITYINFO];	}
	CUIAuctionNew*			GetAuction() { return (CUIAuctionNew*)m_apUIs[UI_AUCTION]; }
	CUIBingoBox*			GetBingoBox() { return (CUIBingoBox*)m_apUIs[UI_BINGOBOX]; } // 초코렛 수집 상자
	CUICalendar*			GetCalendar() { return (CUICalendar*)m_apUIs[UI_CALENDAR]; }
    CUICalendarEventInfo*	GetCalendarEventInfo()	{ return (CUICalendarEventInfo*)m_apUIs[UI_CALENDAR_EVENT_INFO];	}
    CUICashShopEX*			GetCashShopEX() { return (CUICashShopEX*) m_apUIs[UI_CASHSHOP_EX]; }
    CUIChangeWeapon*		GetChangeWeapon() { return (CUIChangeWeapon*)m_apUIs[UI_CHANGEWEAPON]; }
	CUIChangeEquipment*		GetChangeEquipment() { return (CUIChangeEquipment*)m_apUIs[UI_CHANGE_EQUIPMENT]; }
    CUICharacterCreateNew*	GetCreateChar() { return (CUICharacterCreateNew*)m_apUIs[UI_CREATE_CHAR]; }
    CUICharacterInfo*		GetCharacterInfo() { return (CUICharacterInfo*)m_apUIs[UI_CHARACTERINFO]; }
    CUICharacterSelect*		GetCharacterSelect()	{ return (CUICharacterSelect*)m_apUIs[UI_CHARACTER_SELECT];	}
    CUIChatFilter*			GetChatFilter()	{ return (CUIChatFilter*)m_apUIs[UI_CHAT_FILTER]; }
    //CUIChatting*			GetChatting() { return (CUIChatting*)m_apUIs[UI_CHATTING]; }
    CUICollectBox*			GetCollectBox() { return (CUICollectBox*)m_apUIs[UI_COLLECTBOX]; } // 곤충 채집상자
	CUICompass*				GetCompass()	{ return (CUICompass*)m_apUIs[UI_COMPASS]; }
	CUICompound*			GetCompound() { return (CUICompound*)m_apUIs[UI_COMPOUND]; }
    CUICubeRank*			GetCubeRank()	{ return (CUICubeRank*) m_apUIs[UI_CUBERANK]; }
#ifdef	DURABILITY
    CUIDurability*			GetDurability()		{ return (CUIDurability*)m_apUIs[UI_DURABILITY]; }
#endif	// DURABILITY
    CUIExchange*			GetExchange() { return (CUIExchange*)m_apUIs[UI_EXCHANGE]; }
    CUIExpressSystem*		GetLCESystem()	{ return (CUIExpressSystem*) m_apUIs[UI_LCE_SYSTEM];		}
	CUIExpManage*			GetExpedition() {return (CUIExpManage*)m_apUIs[UI_EXPEDITION]; }
    CUIFlowerTree*			GetFlowerTree() {return (CUIFlowerTree*)m_apUIs[UI_FLOWERTREE];} // 꽃놀이 인터페이스
    CUIGamble*				GetGamble() { return (CUIGamble*)m_apUIs[UI_GAMBLE]; }	
	CUIGPS*					GetGPS()	{ return (CUIGPS*)m_apUIs[UI_GPS]; }
	CUIGuild*				GetGuild() { return (CUIGuild*)m_apUIs[UI_GUILD]; }
    CUIGuildBattle*			GetGuildBattle() { return (CUIGuildBattle*)m_apUIs[UI_GUILD_BATTLE]; }
    CUIGuildMark*			GetGuildMark()			{ return (CUIGuildMark*) m_apUIs[UI_GUILD_MARK]; }
    CUIGuildNotice*			GetGuildNotice() {return (CUIGuildNotice*)m_apUIs[UI_GUILD_NOTICE];}// WSS_NEW_GUILD_SYSTEM 070715
    CUIGuildStash*			GetGuildStash() { return (CUIGuildStash*)m_apUIs[UI_GUILDSTASH]; }
    CUIGuildWarPortal*		GetGuildWarPortal() { return (CUIGuildWarPortal*)m_apUIs[UI_GUILDWARPORTAL]; }
    CUIGWMix*				GetGWMix() { return (CUIGWMix*)m_apUIs[UI_GW_MIX]; }
    CUIHardCoreWarning*		GetHardCoreWarning()		{ return (CUIHardCoreWarning*)m_apUIs[UI_HARDCORE_WARNING]; }
    CUIHelp*				GetHelp() { return (CUIHelp*)m_apUIs[UI_HELP]; }
    CUIHelp3*				GetHelp3() { return (CUIHelp3*)m_apUIs[UI_HELP3]; }
    CUIHelper*				GetHelper() { return (CUIHelper*)m_apUIs[UI_HELPER]; }
    CUIHelpIcon*			GetHelpIcon() { return (CUIHelpIcon*)m_apUIs[UI_HELP_ICON]; }		// 도움말 시스템 추가
    CUIInitJob*				GetInitJob() { return (CUIInitJob*)m_apUIs[UI_INITJOB]; }
    CUIInvenCashBagBox*		GetInvenCashBagBox()		{ return (CUIInvenCashBagBox*)m_apUIs[UI_INVEN_CASH_BAG_BOX]; }
    CUIInventory*			GetInventory() { return (CUIInventory*)m_apUIs[UI_INVENTORY]; }
    CUIItemProduct*			GetItemProduct() { return (CUIItemProduct*)m_apUIs[UI_ITEMPRODUCT];	}
    CUILacaball*			GetLacaBall()	{ return (CUILacaball*)(m_apUIs[UI_LACABALL]);	}
    CUILoginNew*			GetLogin() { return (CUILoginNew*)m_apUIs[UI_LOGIN]; }
    CUIMap*					GetMap() { return (CUIMap*)m_apUIs[UI_MAP]; }
	CUIMsgBoxNumericOnly*	GetMsgBoxNumOnly()		{ return (CUIMsgBoxNumericOnly*)m_apUIs[UI_MSGBOX_NUMERIC_ONLY]; }
    CUIMessenger*			GetMessenger() { return (CUIMessenger*)m_apUIs[UI_MESSENGER]; }
    CUIMinigame*			GetMinigame() {return (CUIMinigame*)m_apUIs[UI_MINIGAME];} //WSS_MINIGAME 070418
    CUIMix*					GetMix() { return (CUIMix*)m_apUIs[UI_MIX]; }
    CUIMixNew*				GetMixNew() { return (CUIMixNew*)m_apUIs[UI_MIXNEW]; }
    CUIMonsterCombo*		GetCombo() {return (CUIMonsterCombo*)m_apUIs[UI_MONSTER_COMBO]; }
    CUIMonsterMercenary*	GetMonsterMercenary()	{ return (CUIMonsterMercenary*) m_apUIs[UI_MONSTER_MERCENARY]; }
    CUIMysteriousBead*		GetMysteryousBead()	{ return (CUIMysteriousBead*)m_apUIs[UI_MYSTERYOUSBEAD]; }
    CUIMySyndicateInfo*		GetMySyndiInfo()		{ return (CUIMySyndicateInfo*)m_apUIs[UI_MYSYNDICATE_INFO]; }
    CUINewGuildStash*		GetGuildStash_N()		{ return (CUINewGuildStash*) m_apUIs[UI_NEWGUILDSTASH];			}
    CUINickName*			GetNickName() { return (CUINickName*) m_apUIs[UI_NICKNAME]; }
    CUINoticeNew*			GetNotice() { return (CUINoticeNew*)m_apUIs[UI_NOTICE]; }
    CUINpcHelp*				GetNpcHelp() {return (CUINpcHelp*)m_apUIs[UI_NPCHELP];} //Npc 안내시스템
    CUINpcScroll*			GetNpcScroll() { return (CUINpcScroll*) m_apUIs[UI_NPC_SCROLL]; }
    CUIOption*				GetOption() { return (CUIOption*)m_apUIs[UI_OPTION]; }
    CUIOXQuizEvent*			GetOXQuizEvent() { return (CUIOXQuizEvent*)m_apUIs[UI_QUIZEVENT]; } // O.X 퀴즈 이벤트
    CUIParty*				GetParty() { return (CUIParty*)m_apUIs[UI_PARTY]; }
    CUIPartyAuto*			GetPartyAuto() { return (CUIPartyAuto*)m_apUIs[UI_PARTYAUTO]; } // party auto matching
    CUIPersonalShop*		GetPersonalShop() { return (CUIPersonalShop*)m_apUIs[UI_PERSONALSHOP]; }
    CUIPetFree*				GetPetFree()	{ return (CUIPetFree*)m_apUIs[UI_PETFREE]; } // 펫 봉인 해제
    CUIPetInfo*				GetPetInfo() { return (CUIPetInfo*)m_apUIs[UI_PETINFO]; }
    CUIPetItemMix*			GetPetItemMix() { return (CUIPetItemMix*)m_apUIs[UI_PETITEMMIX]; } // 펫아이템 조합( 유니크 )
    CUIPetStash*			GetPetStash()				{ return (CUIPetStash*)m_apUIs[UI_PET_STASH]; }
    CUIPetStashSelectEffect*	GetPetStashSelectEffect()	{ return (CUIPetStashSelectEffect*)m_apUIs[UI_PET_STASH_SELECT_EFFECT]; }
    CUIPetTargetInfo*		GetPetTargetInfo() { return (CUIPetTargetInfo*)m_apUIs[UI_PETTARGETINFO]; }
    CUIPetTraining*			GetPetTraining() { return (CUIPetTraining*)m_apUIs[UI_PETTRAINING]; }
    CUIPlayerInfo*			GetPlayerInfo() { return (CUIPlayerInfo*)m_apUIs[UI_PLAYERINFO]; }
    CUIPortal*				GetPortal() { return (CUIPortal*)m_apUIs[UI_PORTAL]; }
    CUIProcess*				GetProcess() { return (CUIProcess*)m_apUIs[UI_PROCESS]; }
    CUIProcessNPC*			GetProcessNPC() { return (CUIProcessNPC*)m_apUIs[UI_PROCESSNPC]; }
    CUIProduct*				GetProduct() { return (CUIProduct*)m_apUIs[UI_PRODUCT]; }
    CUIProduct2*			GetProduct2()	{return (CUIProduct2*)m_apUIs[UI_PRODUCT2]; }
    CUIProductNPC*			GetProductNPC() { return (CUIProductNPC*)m_apUIs[UI_PRODUCTNPC]; }
    CUIQuest*				GetQuest() { return (CUIQuest*)m_apUIs[UI_QUEST]; }
	CUIQuestBook*			GetQuestBookList() { return (CUIQuestBook *)m_apUIs[UI_QUESTBOOK_LIST]; }
	CUIQuestComplete*		GetQuestBookComplete() { return (CUIQuestComplete*)m_apUIs[UI_QUESTBOOK_COMPLETE]; }
	CUIQuestAccept*			GetQuestAccept() { return (CUIQuestAccept *)m_apUIs[UI_QUEST_ACCEPT]; }
	CUIQuestRestore*		GetQuestRestore() { return (CUIQuestRestore *)m_apUIs[UI_QUEST_RESTORE]; }
	CUIQuestView*			GetQuestView() { return (CUIQuestView *)m_apUIs[UI_QUEST_VIEW]; }
    CUIQuickSlot*			GetQuickSlot() { return (CUIQuickSlot*)m_apUIs[UI_QUICKSLOT]; }
    CUIRadar*				GetRadar() { return (CUIRadar*)m_apUIs[UI_RADAR]; }
    CUIRanking*				GetRanking() { return (CUIRanking*)m_apUIs[UI_RANKING]; }
    CUIRankingSystem*		GetRankingViewEx()		{ return (CUIRankingSystem*)m_apUIs[UI_RANKINGVIEW_RENEWAL]; }
    CUIRefine*				GetRefine() { return (CUIRefine*)m_apUIs[UI_REFINE]; }
    CUIReformSystem*		GetReformSystem()		{ return (CUIReformSystem*) m_apUIs[UI_REFORM_SYSTEM];		}
    CUIResurrectionMsgBox*	GetResurrectionMsgBox()	{ return (CUIResurrectionMsgBox*)m_apUIs[UI_RESURRECTION];	}
    CUIRoyalRumbleIcon*		GetRoyalRumbleIcon()	{ return (CUIRoyalRumbleIcon*) m_apUIs[UI_ROYALRUMBLE_ICON];	}
    CUISecurity*			GetSecurity() { return (CUISecurity*)m_apUIs[UI_SECURITY]; }
    CUISelectList*			GetSelectList() { return (CUISelectList*)m_apUIs[UI_SELECTLIST]; } // 친구찾기 이벤트 060126
    CUISelectResource*		GetSelectResource() { return (CUISelectResource*)m_apUIs[UI_SELECTRESOURCE]; }
    CUISelectWord*			GetSelectWord() { return (CUISelectWord*)m_apUIs[UI_SELECTWORD]; }
    CUIServerSelect*		GetServerSelect()	{ return (CUIServerSelect*)m_apUIs[UI_SERVER_SELECT];	}
    CUIShop*				GetShop() { return (CUIShop*)m_apUIs[UI_SHOP]; }
    CUISiegeWarfare*		GetSiegeWarfare() { return (CUISiegeWarfare*)m_apUIs[UI_SIEGE_WARFARE]; }
    CUISiegeWarfareInfo*	GetSiegeWarfareInfo()	{ return (CUISiegeWarfareInfo*) m_apUIs[UI_SIEGE_WARFARE_INFO]; }
    CUISiegeWarfareNew*		GetSiegeWarfareNew() { return (CUISiegeWarfareNew*)m_apUIs[UI_SIEGE_WARFARE_NEW]; }  // WSS_DRATAN_SIEGEWARFARE 070727
    CUISimplePop*			GetSimplePop() { return (CUISimplePop*)m_apUIs[UI_SIMPLE_POP]; }	// [KH_070419] 심플 팝업 관련 추가
    CUISingleBattle*		GetSingleBattle() { return (CUISingleBattle*)m_apUIs[UI_SINGLE_BATTLE]; }
    CUISkillLearn*			GetSkillLearn() { return (CUISkillLearn*)m_apUIs[UI_SKILLLEARN]; }
    CUISkillNew*			GetSkillNew()				{ return (CUISkillNew*)m_apUIs[UI_SKILL_NEW]; }
    CUISocketSystem*		GetSocketSystem()	{ return (CUISocketSystem*) m_apUIs[UI_SOCKETSYSTEM]; }
    CUISummon*				GetSummonFirst() { return (CUISummon*)m_apUIs[UI_SUMMON_FIRST]; }
    CUISummon*				GetSummonSecond() { return (CUISummon*)m_apUIs[UI_SUMMON_SECOND]; }
    CUISystemMenu*			GetSystemMenu() { return (CUISystemMenu*)m_apUIs[UI_SYSTEMMENU]; }
    CUITalk*				GetTalk(int iUIIndex) { return (CUITalk*)m_apUIs[iUIIndex]; }
    CTargetInfoUI*			GetTargetInfoUI() { return (CTargetInfoUI*)m_apUIs[UI_TARGETINFO]; }
    CUITatoo*				GetTatoo() { return (CUITatoo*)m_apUIs[UI_TATOO]; }	// [090709: selo] 펫 문양 시스템
    CUITeleport*			GetTeleport() { return (CUITeleport*)m_apUIs[UI_TELEPORT]; }
    CUITeleportPrimium*		GetTeleportPrimium() { return (CUITeleportPrimium*)m_apUIs[UI_TELEPORT_PRIMIUM]; }	// [KH_070315] 프리미엄 메모리스크롤 관련 추가
    CUITrade*				GetTrade()				{ return (CUITrade*)m_apUIs[UI_TRADE];	}
    CUITreasureMap*			GetTreasureMap()		{ return (CUITreasureMap*) m_apUIs[UI_TREASUREMAP]; }
	CUIViewDetail*			GetExpeditionView() {return (CUIViewDetail*)m_apUIs[UI_EXPEDITION_VIEW]; }
    CUIWareHouse*			GetWareHouse() { return (CUIWareHouse*)m_apUIs[UI_WAREHOUSE]; }
    CUIWebBoard*			GetWebBoard() { return (CUIWebBoard*)m_apUIs[UI_WEBBOARD]; }
    CUIWildPetInfo*			GetWildPetInfo() {return (CUIWildPetInfo*)m_apUIs[UI_WILDPET_INFO]; }
    CUIWildPetTargetInfo*	GetWildPetTargetInfo() { return (CUIWildPetTargetInfo*)m_apUIs[UI_WILDPETTARGETINFO]; }
	CItemCollectionUI*		GetItemCollection()		{ return (CItemCollectionUI*)m_apUIs[UI_ITEMCOLLECTION];	}
	CPremiumCharUI*			GetPremiumChar()		{ return (CPremiumCharUI*)m_apUIs[UI_PREMIUM_CHAR];	}
	CSimplePlayerInfoUI*	GetSimplePlayerInfo()	{ return (CSimplePlayerInfoUI*)m_apUIs[UI_SIMPLE_PLAYER_INFO];	}
	CSimplePlayerMenuUI*	GetSimplePlayerMenuUI()	{ return (CSimplePlayerMenuUI*)m_apUIs[UI_SIMPLE_PLAYER_MENU];	}
	CChattingUI*			GetChattingUI()			{ return (CChattingUI*)m_apUIs[UI_CHATTING_NEW];	}
	CNewsUI*				GetNewsUI()				{ return (CNewsUI*)m_apUIs[UI_NEWS];	}
	CNewsWebUI*				GetNewsWebUI()			{ return (CNewsWebUI*)m_apUIs[UI_NEWS_WEB];	}
	CItemComposeUI*			GetItemCompose()		{ return (CItemComposeUI*)m_apUIs[UI_ITEM_COMPOSE]; }
	CHelpWebUI*				GetHelpWebUI()			{ return (CHelpWebUI*)m_apUIs[UI_HELP_WEB];	}

	void					InitRankSystem();

	bool ENGINE_API LoadXML( const char* strPath);
	bool LoadXML( const char* strPath, CUIBase* parent );

protected:
	void			initUI();

	CUIWindow		*m_apUIs[UI_TYPE_END];				// Pointers of UIs
};


#endif		// UI_WINDOW_DECLARE_H_
