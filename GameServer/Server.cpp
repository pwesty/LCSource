#include <boost/format.hpp>
#include <boost/static_assert.hpp>
#include <boost/foreach.hpp>
#include "stdhdrs.h"

#include "gameserver_config.h"
#include "Log.h"
#include "Exp.h"
#include "Server.h"
#include "Cmd.h"
#include "CmdMsg.h"
#include "../ShareLib/DBCmd.h"
#include "doFunc.h"
#include "Battle.h"
#include "WarCastle.h"
#include "DratanCastle.h"
#include "LuckyDrawBox.h"
#include "WearInvenManager.h"
#include "../ShareLib/rnsocketioserviceTcp.h"
#include "../ShareLib/packetType/ptype_server_to_server.h"
#include "../ShareLib/packetType/ptype_old_do_item.h"
#include "../ShareLib/packetType/ptype_char_status.h"
#include "../ShareLib/packetType/ptype_old_mempos.h"
#include "../ShareLib/packetType/ptype_old_do_action.h"
#include "../ShareLib/packetType/ptype_old_do_skill.h"
#include "../ShareLib/packetType/ptype_old_do_move.h"
#include "../ShareLib/packetType/ptype_old_do_sskill.h"

#include "TitleSystem.h"
#include "RaidData.h"
#include "MonsterMercenary.h"
#include "WarGround.h"
#ifdef GER_LOG
#include <stdlib.h>
#endif // GER_LOG
#include "GMCmdList.h"
#include "DBManager.h"
#include "itemExchangeProto.h"
#include "HolyWaterData.h"
#include "EquipExchangeExtend.h"
#include "ItemCollection.h"
#include "Notice.h"
#include "Artifact_Manager.h"

// DescManager에서 userIndex를 유니크하게 관리를 해야하는데
// 최초 접속시에는 userIndex를 모름으로 더미값을 넣어주고
// Connector로 부터 실제값을 받으면 바꾸어준다.
// 음수의 최대치인 -2147483647에서 -1을 빼면 2147483647이 됨
static int local_dummy_user_index = 0;

CServer* gserver = NULL;
CCmdList gcmdlist;
CCmdList gexcmdlist;

bool    g_bUpgradeEvent;
int		g_nUpgradeProb;

void ServerSrandom(unsigned long initial_seed);

bool IsApllyDebuffSkilltoNPC( CPC* aPc, CNPC* tNpc); // 나이트 쉐도우 스킬 #764,765 스킬 이 몬스터 용병에 적용되지 않도록 예외처리.

static LONGLONG local_seq_index = 0;

CServer::CServer()
	: m_serverAddr(HOST_LENGTH + 1)
	, m_dbcastle(m_dbchar)
	, m_dbcharingame(m_dbchar)
	, m_dbTrigger(m_dbchar)

#ifdef IMP_SPEED_SERVER
	,m_bSpeedServer(false)
#endif //IMP_SPEED_SERVER
{
	m_serverpath = GetServerPath();
	m_serverno = -1;
	m_subno = -1;
	m_maxplayers = 0;
	m_nameserverisslow = true;
	m_bShutdown = false;

	m_pulseReboot = -1;
	m_pulseShutdown = -1;
	m_pulseEndGame = -1;

	m_bLoop = 0;
	m_descLoop = NULL;
	m_observer = NULL;
	memset(m_chatmonitor, 0, sizeof(CDescriptor*) * 20);

#ifdef GMTOOL
	m_gmtool = NULL;
#endif // GMTOOL

	m_nowseconds = time(0);
	m_pulse = 0;

	m_resetWeather = 0;
	m_resetAdjustItem = 0;

	mysql_init(&m_dbchar);
	mysql_init(&m_dbdata);

#ifdef USE_TENTER_BILLING
	mysql_init(&m_dbcatal);
#endif

	m_numZone = 0;
	m_zones = NULL;

	m_clientversionMin = 0;
	m_clientversionMax = 0;

	m_bDoubleEvent = false;

#ifdef DOUBLE_PET_EXP_AUTO
	m_tPetExpEventStart = -1;
	m_tPetExpEventEnd = -1;
#endif
#ifdef UPGRADE_EVENT_AUTO
	m_tUpgradeEventStart = -1;
	m_tUpgradeEventEnd = -1;
#endif
#ifdef NEW_DOUBLE_GM_ZONE
	m_bDoubleEventZone = -1;
#endif // NEW_DOUBLE_GM_ZONE

	for(int i=0; i < 3; i++)
	{
		m_nGuildMarkTable[i] = 0;
	}

#ifdef DOUBLE_PET_EXP
	m_bDoublePetExpEvent = false;
	m_PetExpPercent = 100;
#endif // DOUBLE_PET_EXP

	m_bDoubleNasPercent = DEFAULT_NAS_PERCENT;
	m_bDoubleNasGetPercent = DEFAULT_NAS_GET_PERCENT;
	m_bDoubleExpPercent = DEFAULT_EXP_PERCENT;
	m_bDoubleSpPercent = DEFAULT_SP_PERCENT;
	m_bDoubleProducePercent = DEFAULT_PRO_PERCENT;
	m_bDoubleProduceNum = DEFAULT_PRO_GET_NUMBER;
#ifdef NEW_DOUBLE_GM_AUTO
	m_bDoubleNasPercentBackup = DEFAULT_NAS_PERCENT;
	m_bDoubleNasGetPercentBackup = DEFAULT_NAS_GET_PERCENT;
	m_bDoubleExpPercentBackup = DEFAULT_EXP_PERCENT;
	m_bDoubleSpPercentBackup = DEFAULT_SP_PERCENT;
	m_bDoubleProducePercentBackup = DEFAULT_PRO_PERCENT;
	m_bDoubleProduceNumBackup = DEFAULT_PRO_GET_NUMBER;
	memset(m_iDoubleGMStart, -1, sizeof(m_iDoubleGMStart));
	memset(m_iDoubleGMEnd, -1, sizeof(m_iDoubleGMEnd));
	m_bIsDEtime = false;
#endif // NEW_DOUBLE_GM_AUTO

#ifdef NEW_DOUBLE_EVENT_AUTO_TIME
	memset(m_iDoubleEventStart, -1, sizeof(m_iDoubleEventStart));
	memset(m_iDoubleEventEnd, -1, sizeof(m_iDoubleEventEnd));
#endif // NEW_DOUBLE_EVENT_AUTO_TIME
#ifdef EVENT_ITEMDROP_AUTO
	m_bIsItemdrop = false;
	memset(m_iEventItemdropStart, -1, sizeof(m_iEventItemdropStart));
	memset(m_iEventItemdropEnd, -1, sizeof(m_iEventItemdropEnd));
#endif // EVENT_ITEMDROP_AUTO

#ifdef GER_LOG
	m_hostname = NULL;
#endif // GER_LOG

#ifdef EVENT_DROPITEM
	m_bDropItem			= false;
	m_bDropItemNpcIdx	= -1;
	m_bDropItemIdx		= -1;
	m_bDropProb			= 0;
#endif // EVENT_DROPITEM

	m_CollectBugEventPercent = 10;

	m_bDoubleExpEvent = false;
	m_expPercent = 100;
	m_bLattoEvent = false;
	m_bNewYearEvent = false;
	m_bValentineEvent = false;

	m_nItemDropEventRate = 100;
#ifdef EVENT_ITEMDROP_AUTO
	m_nItemDropEventRateBackup = 100;
#endif

	m_itemDropProb = 100;
	m_moneyDropProb = 100;

	m_statisticsItemBuy = 0;
	m_statisticsItemSell = 0;
	m_statistics152 = 0;
	m_statistics153 = 0;
	m_statistics154 = 0;
	m_statistics155 = 0;
	m_statistics156 = 0;
	m_statistics157 = 0;
	m_statistics158 = 0;
	m_statistics159 = 0;
	m_statistics160 = 0;
	m_statistics161 = 0;
	m_statistics162 = 0;
	m_statistics163 = 0;
	m_statistics197 = 0;
	m_statistics198 = 0;
	m_statistics199 = 0;

	m_taxItem = 0;
	m_taxProduceCastle = 0;
	m_taxSavePulse = 0;
	m_taxDivGuild = -1;

	m_taxItemDratan = 0;
	m_taxProduceCastleDratan = 0;

	m_unEventXmasTreePoint = 0;

	m_bEventOX = false;

	m_bRecommend = false;
	m_pulseRecommendMoonstone = m_pulse;

#if defined (LC_TLD)
	m_national = LC_TLD;
#elif defined (LC_USA)
	m_national = LC_USA;
#elif defined (LC_BRZ)
	m_national = LC_BRZ;
#elif defined (LC_GER)
	m_national = LC_GER;
#elif defined (LC_SPN)
	m_national = LC_SPN;
#elif defined (LC_FRC)
	m_national = LC_FRC;
#elif defined (LC_PLD)
	m_national = LC_PLD;
#elif defined (LC_RUS)
	m_national = LC_RUS;
#elif defined (LC_TUR)
	m_national = LC_TUR;
#elif defined (LC_ITA)
	m_national = LC_ITA;
#elif defined (LC_MEX)
	m_national = LC_MEX;
#elif defined (LC_UK)
	m_national = LC_UK;
#else
	m_national = LC_KOR;
#endif

	m_bpSiteGive = false;
	m_bpSiteCount = 300;

	m_bLoadPartyInfo = false;

	m_pulseProcHeartBeatPerSec = 0;

	m_bNonPK = false;

#ifdef FREE_PK_SYSTEM
	m_bFreePk = false;
#endif

#ifdef MAL_DISABLE_PKPENALTY
	m_bDisablePKPaenalty = false;
#endif //MAL_DISABLE_PKPENALTY

#ifdef DOUBLE_ITEM_DROP
	m_bDoubleItemEvent = false;
	m_bDoubleItemPercent = 0;
#endif	// DOUBLE_ITEM_DROP

	m_nExpLimit = DEFAULT_LIMIT_EXP;
	m_nSpLimit = DEFAULT_LIMIT_EXP;
#ifdef GM_EXP_LIMIT_AUTO
	m_bExpLimit = false;
	m_nExpLimitStart = DEFAULT_LIMIT_EXP;
	m_nExpLimitEnd = DEFAULT_LIMIT_EXP;
	m_nSpLimitStart = DEFAULT_LIMIT_EXP;
	m_nSpLimitEnd = DEFAULT_LIMIT_EXP;
	memset(m_nGMExpLimitStart, -1, sizeof(m_nGMExpLimitStart));
	memset(m_nGMExpLimitEnd, -1, sizeof(m_nGMExpLimitEnd));
#endif // GM_EXP_LIMIT_AUTO

	m_comboZone = NULL;

#ifdef NEW_DOUBLE_EVENT_AUTO
	m_bDoubleEventAuto = true;
	m_bDoubleEventAutoOn = false;
#endif // NEW_DOUBLE_EVENT_AUTO

	g_nUpgradeProb = 100;
	g_bUpgradeEvent = false;

	for(int j = 0; j < 24; j++)
	{
		if((j == 0) || (j % 3 == 0))
			m_bTimeTable[j] = true;
		else
			m_bTimeTable[j] = false;
	}
	m_iTimeInterval = 1;
	m_iStartTime = -1;
	m_iEndTime = -1;
	m_bDungeonTimeToggle = true;
	m_bIsTime = false;
	m_iZoneCount = 0;

	m_tRealSystemTime = NOW();
	m_bCashShopLock = false;

#ifdef MEREC_CASTLE_TEST_GUILD_LEVEL_GM_SETTING
	MeracCastleDebugGuildLevel=0;
#endif
	m_tRaidResetProcess = 0;
	m_nProcess = 0;
	m_bCanEnterTheRaidDungeon = true;
	m_bIsCheckTime = false;
	m_nResetDay = RAID_RESET_TIME_DAY;
	m_nResethr = RAID_RESET_TIME_HOUR;
	m_nResetmin = RAID_RESET_TIME_MIN;

	m_bApplySkillToNPC = false;

#ifdef SYSTEM_TREASURE_MAP
#ifdef SYSTEM_TREASURE_MAP_LINKZONE_DROP
	bTreasureMapLinkZoneDrop = true;
#else
	bTreasureMapLinkZoneDrop = false;
#endif
#endif

#if defined (BILA_INTERGRATION_SERVER)
	m_intergrationInfo.AddNation(INTERGRATION_BRAZIL);
	m_intergrationInfo.AddNation(INTERGRATION_MEXICO);
#endif
#if defined (EU_INTERGRATION_SERVER)
	m_intergrationInfo.AddNation(INTERGRATION_GER);
	m_intergrationInfo.AddNation(INTERGRATION_SPN);
	m_intergrationInfo.AddNation(INTERGRATION_FRC);
	m_intergrationInfo.AddNation(INTERGRATION_PLD);
	m_intergrationInfo.AddNation(INTERGRATION_TUR);
	m_intergrationInfo.AddNation(INTERGRATION_ITA);
	m_intergrationInfo.AddNation(INTERGRATION_UK);
#endif

	m_messengerConnMsg.reset(new CNetMsg);
	m_connectorConnMsg.reset(new CNetMsg);
	m_helperConnMsg.reset(new CNetMsg);
	m_subHelperConnMsg.reset(new CNetMsg);

	m_messenger = MessengerInGame::instance();
	m_connector = ConnectorInGame::instance();
	m_helper = HelperInGame::instance();
	m_subHelper = SubHelperInGame::instance();

	barunsongames_flag = false;
	m_hardcore_flag_in_gameserver = 0;

	m_DratanCheckTime = 3;

	CWearInvenManager::Init();
#ifdef TLD_EVENT_SONG
	tld_event = false;
#endif
}

CServer::~CServer()
{
	return;

	//////////////////////////////////////////////////////////////////////////


	if (m_zones)
		delete[] m_zones;
	m_zones = NULL;
	m_numZone = 0;
	if (m_serverpath)
		delete[] m_serverpath;
	m_serverpath = NULL;

	{
		map_listparty_t::iterator it = m_listParty.begin();
		map_listparty_t::iterator endit = m_listParty.end();
		for (; it != endit; ++it)
		{
			delete (it->second);
		}
		m_listParty.clear();
	}

	{
		map_listexped_t::iterator it = m_listExped.begin();
		map_listexped_t::iterator endit = m_listExped.end();
		for (; it != endit; ++it)
		{
			delete (it->second);
		}
		m_listExped.clear();
	}

	{
		map_listPartyMatchMember_t::iterator it = m_listPartyMatchMember.begin();
		map_listPartyMatchMember_t::iterator endit = m_listPartyMatchMember.end();
		for (; it != endit; ++it)
		{
			delete (it->second);
		}
		m_listPartyMatchMember.clear();
	}

	{
		map_listPartyMatchParty_t::iterator it = m_listPartyMatchParty.begin();
		map_listPartyMatchParty_t::iterator endit = m_listPartyMatchParty.end();
		for (; it != endit; ++it)
		{
			delete (it->second);
		}
		m_listPartyMatchParty.clear();
	}

	{
		map_listOXQuiz_t::iterator it = m_listOXQuiz.begin();
		map_listOXQuiz_t::iterator endit = m_listOXQuiz.end();
		for (; it != endit; ++it)
		{
			delete (it->second);
		}
		m_listOXQuiz.clear();
	}

	m_LuckyDrawBox.RemoveAll();

#ifdef LACARETTE_SYSTEM
	m_lacarette.RemoveAll();
#endif

#ifdef DEV_EVENT_PROMOTION2
	m_promotion2.RemoveAll();
#endif

	m_MonsterMercenary.ReleaseAll();

	if (m_XmasPuzzleGame)
		delete m_XmasPuzzleGame;
	if (m_XmasRockPaperScissorsGame)
		delete m_XmasRockPaperScissorsGame;
}

char* CServer::GetServerPath()
{
	char szBuffer[1024];
#ifdef CIRCLE_WINDOWS
	GetModuleFileName(::GetModuleHandle(NULL), szBuffer, 1000);
	int path_len = strlen(szBuffer);
	int i;

	for (i = path_len - 1; i >= 0; i-- )
	{
		if (szBuffer[i] == '\\')
		{
			szBuffer[i+1] = '\0';
			break;
		}
	}
	if (i < 0)
		return false;
#else
	getcwd (szBuffer, 512);
	strcat (szBuffer, "/");
#endif

	char* ret = new char[strlen(szBuffer) + 1];
	strcpy(ret, szBuffer);
	return ret;
}

bool CServer::LoadSettingFile()
{
	puts("Load setting file....");

	CLCString strTemp(1024);

	strTemp.Format("%s%s", m_serverpath, "data/newStobm.bin");

	if (!m_config.Load(strTemp))
		return false;
#ifdef DEV_EVENT_AUTO
	strTemp.Format("%s%s", m_serverpath, "data/event.ini");
	if(!m_eventConfig.Load(strTemp))
	{
		puts("Loading Error : [data/event.ini] file is not exist.");
		return false;
	}
	else
	{
		m_fathersDay.Load(m_eventConfig);
		m_DropEvent.Load(m_eventConfig);
	}
#endif

	m_serverno = atoi(m_config.Find("Server", "Number"));
	m_subno = atoi(m_config.Find("Server", "SubNumber"));
#ifdef GER_LOG
	m_hostname = getenv("HOSTNAME");
#endif // GER_LOG

#if defined(LC_GAMIGO) || defined ( LC_USA ) || defined (LC_BILA) || defined (LC_RUS)
	if(m_subno == 2 || m_subno == 5)
	{
		m_bNonPK = true;
	}
#elif defined(LC_KOR)
	if(m_subno == 3)
	{
		m_bNonPK = true;
	}
#else
	if(m_subno == 7 || m_subno == 8)
	{
		m_bNonPK = true;
	}
#endif


#if defined FREE_PK_SYSTEM
#if defined ( LC_TLD )
	if( m_serverno == 4 )
		m_bFreePk = true;
	if( (m_serverno == 3 && m_subno == 1) || (m_serverno == 2 && m_subno == 1) || (m_serverno == 1 && m_subno == 1) )
		m_bFreePk = true;
#endif
#endif // FREE_PK_SYSTEM

#if defined MAL_DISABLE_PKPENALTY

	if(m_subno == 2)
		m_bDisablePKPaenalty = true;

#endif
	if (strcmp(m_config.Find("Server", "AllowExternalIP"), "FALSE") == 0)
		m_bOnlyLocal = true;
	else
		m_bOnlyLocal = false;

	if (strcmp(m_config.Find("Server", "NON_PK"), "TRUE") == 0)
		m_bNonPK = true;
	else if (strcmp(m_config.Find("Server", "NON_PK"), "FALSE") == 0)
		m_bNonPK = false;

#ifdef GMTOOL
	if(gserver->m_serverno == 1 && gserver->m_subno == 1)
	{
		const char* p = m_config.Find("GMTOOL", "count");
		if(!p)
			return false;
		m_nGMToolCount = atoi(p);
		if(m_nGMToolCount <= 0)
		{
			m_gmtool = NULL;
		}
		else
		{
			m_gmtool = new CDescriptor*[m_nGMToolCount];
			int i;
			for(i = 0; i < m_nGMToolCount; ++i) m_gmtool[i] = NULL;
		}
	}
#endif // GMTOOL

#if defined (INTERGRATION_SERVER)
	if (strcmp(m_config.Find("Server", "IntergrationServer"), "TRUE") == 0)
		m_intergrationInfo.SetEnable(true);
	else
		m_intergrationInfo.SetEnable(false);
#endif

#ifdef IMP_SPEED_SERVER
	if (strcmp(m_config.Find("Server", "SPEED_SERVER"), "TRUE") == 0)
		m_bSpeedServer = true;
#endif //IMP_SPEED_SERVER

#ifdef HARDCORE_SERVER
	if (strcmp(m_config.Find("Server", "HARDCORE"), "TRUE") == 0)
	{
		m_hardcore_flag_in_gameserver = 1;
	}
#endif

	return true;
}

bool CServer::LoadSettings()
{
	int i;

	GAMELOG << init("SYSTEM")
			<< "Load settings...."
			<< end;

	// version information loading
	GAMELOG << init("SYSTEM")
			<< "Client Version Loading..."
			<< end;

	CDBCmd dbcmd;
	dbcmd.Init(&m_dbdata);
	dbcmd.SetQuery("SELECT a_min, a_max FROM t_clientversion");
	if (!dbcmd.Open() || !dbcmd.MoveFirst() || !dbcmd.GetRec("a_min", m_clientversionMin) || !dbcmd.GetRec("a_max", m_clientversionMax))
	{
		LOG_INFO("Client Version Query Error.");
		return false;
	}

	ComposeItem::instance()->load();

	//공지사항 로드
	Notice::instance()->load();
	//팝업 공지사항 로드
	PopupNotice::instance()->load();

	CEquipExchangeExtend::instance()->load_();

#ifdef LACARETTE_SYSTEM
	if( !m_lacarette.Load())
	{
		GAMELOG << init("ERROR : CANNOT LOAD LACARETTE SYSTEM DATA") << end;
		return false;
	}
#endif

#ifdef DEV_EVENT_PROMOTION2
	if( !m_promotion2.Load() )
	{
		GAMELOG << init("ERROR : CANNOT LOAD EVENT_PROMOTION DATA") << end;
		return false;
	}
#endif

#ifdef XTRAP
	if( !InitXTrap() )
	{
		GAMELOG << init("ERROR : Init XTRAP ") << end;
		return false;
	}
#endif // XTRAP

	// item proto loading
	GAMELOG << init("SYSTEM")
			<< "Item Loading..."
			<< end;
	if (!m_itemProtoList.Load())
	{
		LOG_INFO("ItemProtoList Load Error");
		return false;
	}

	GAMELOG << init("SYSTEM")
			<< "Set Item Loading..."
			<< end;
	if (!m_setItemProtoList.Load())
	{
		LOG_INFO("SetItemProtoList Load Error");
		return false;
	}

	GAMELOG << init("SYSTEM")
			<< "Factory Item Loading..."
			<< end;
	if (!m_factoryItemProtoList.Load())
	{
		LOG_INFO("FactoryItemProtoList Load Error");
		return false;
	}

	GAMELOG << init("SYSTEM")
			<< "CashItem Loading..."
			<< end;
	if( !m_catalogList.Load() )
	{
		LOG_INFO("CatalogList Load Error");
		return false;
	}

	// Apet proto loading
	GAMELOG << init("SYSTEM")
			<< "Apet Loading..."
			<< end;
	if (!m_pApetlist.Load())
	{
		LOG_INFO("Apetlist Load Error");
		return false;
	}

	// skill proto loading
	GAMELOG << init("SYSTEM")
			<< "Skill Loading..."
			<< end;
	if (!m_magicProtoList.Load())
	{
		LOG_INFO("MagicProtoList Load Error");
		return false;
	}
	if (!m_skillProtoList.Load())
	{
		LOG_INFO("SkillProto List Load Error");
		return false;
	}
	// npc proto loading
	GAMELOG << init("SYSTEM")
			<< "Npc Loading..."
			<< end;
	if (!m_npcProtoList.Load())
	{
		LOG_INFO("npcProtoList Load Error");
		return false;
	}

	// quest proto loading
	GAMELOG << init("SYSTEM")
			<< "Quest Loading..."
			<< end;
	if (!m_questProtoList.Load())
	{
		LOG_INFO("questProtoList Load Error");
		return false;
	}

	GAMELOG << init("SYSTEM") << "Reward Loading..." << end;
	if( !m_rewardMgr.loadReward( &gserver->m_dbdata ) )
	{
		LOG_INFO("Reward List Load Error");
		return false;
	}

	GAMELOG << init("SYSTEM") << "Event Automation Loading..." << end;
	if( !m_eventAutomationMgr.loadEvent( ) )
	{
		LOG_INFO("Event Automation List Load Error");
		return false;
	}

	GAMELOG << init("SYSTEM") << "Drop List Loading..." << end;
	if( !m_dropItemMgr.loadDropData(  &gserver->m_dbdata ) )
	{
		LOG_INFO("Drop List Load Error");
		return false;
	}

	// Option Proto Loading
	GAMELOG << init("SYSTEM")
			<< "Option Loading..."
			<< end;
	if (!m_optionProtoList.Load())
	{
		LOG_INFO("optionProtoList Load Error");
		return false;
	}

	// rare option proto loading
	GAMELOG << init("SYSTEM")
			<< "Rare Option Loading..."
			<< end;
	if (!m_rareOptionList.Load())
	{
		LOG_INFO("rareOptionList Load Error");
		return false;
	}

	// SSpecial Skill Proto Loading
	GAMELOG << init("SYSTEM")
			<< "Special Skill Loading..."
			<< end;

	if (!m_sSkillProtoList.Load())
	{
		LOG_INFO("sSkillProtoList Load Error");
		return false;
	}

	// OX Quiz Loading
	GAMELOG << init("SYSTEM")
			<< "OX Quiz Loading..."
			<< end;

	if (!LoadOXQuiz())
	{
		LOG_INFO("LoadOXQuiz Error");
		return false;
	}

	if( !m_moonstoneReward.Load())
	{
		LOG_INFO("moonstoneReward Load Error");
		return false;
	}

	GAMELOG << init("SYSTEM")
			<< "Trigger System Loading..."
			<< end;
	if( !m_CTriggerDBInfo.Load_Trigger())
	{
		LOG_INFO("TriggerDBInfo Load Error");
		return false;
	}

	GAMELOG << init("SYSTEM")
			<< "Load Raid Scene Info"
			<< end;
	if(!m_RaidInfo.Load())
	{
		GAMELOG << init("ERROR : CANNOT LOAD NEW RAID INFO") << end;
		return false;
	}

	GAMELOG << init("SYSTEM")
			<< "Load LuckydrawBox"
			<< end;
	if( !m_LuckyDrawBox.Load() )
	{
		GAMELOG << init("ERROR: CANNOT LOAD LUCKYDRAWBOX DATA") << end;
		return false;
	}

	GAMELOG << init("SYSTEM") << "Load Monster Mercenary System Data" << end;
	if( !m_MonsterMercenary.LoadData() )
	{
		GAMELOG << init("ERROR : CANNOT LOAD MONSTER MERCENARY SYSTEM DATA") << end;
		return false;
	}

	// Loading Zone
	m_numZone = atoi(m_config.Find("Zones", "Count"));
	if (m_numZone == 0 || m_numZone > MAX_ZONES)
	{
		GAMELOG << init("ERROR: ZONE MAX COUNT OVER") << end;
		return false;
	}

	m_zones = new CZone[m_numZone];
	for (i = 0; i < m_numZone; i++)
	{
		if (!m_zones[i].LoadZone(i))
		{
			LOG_INFO("LoadZone Error");
			return false;
		}

		if(m_zones[i].m_index == 21)
		{
			// 공성 보상 던전
			CDratanCastle * pCastle = CDratanCastle::CreateInstance();
			pCastle->m_dvd.SetZone( &m_zones[i] );
		}

		// Shop Proto Loading
		if (!m_zones[i].LoadShop())
		{
			LOG_INFO("LoadShop Error");
			return false;
		}

		m_zones_map.insert(map_zone_t::value_type(m_zones[i].m_index, &m_zones[i]));
	}
#ifdef SYSTEM_TREASURE_MAP
	gserver->readTreasureMapSaveFile();
#endif
	for (i = 0; i < m_numZone; i++)
	{
		if( m_zones[i].m_index == ZONE_ALTER_OF_DARK || m_zones[i].m_index==ZONE_CAPPELLA_1 || m_zones[i].m_index==ZONE_AKAN_TEMPLE
				|| m_zones[i].m_index == ZONE_DUNGEON4
				|| m_zones[i].m_index == ZONE_TARIAN_DUNGEON
		  )
			continue;

		if (!m_zones[i].LoadNPC())
		{
			LOG_INFO("LoadNPC Error");
			return false;
		}
	}

	GAMELOG << init("SYSTEM")
			<< "MISSIONCASE Loading..."
			<< end;

	if(!m_missionCaseList.LoadList())
	{
		LOG_INFO("missionCaseList Load Error");
		return false;
	}

	if(!m_affinityProtoList.LoadAffinity())
	{
		LOG_INFO("LoadAffinity Error");
		return false;
	}

	GAMELOG << init("Load Castle Information...") << end;
	if (!LoadCastleData())
	{
		GAMELOG << init("ERROR: CANNOT LOAD CASTLE INFO") << end;
		return false;
	}

#ifdef EXTREME_CUBE
	GAMELOG << init("SYSTEM")
			<< "Load Cube Data"
			<< end;

	if(!m_extremeCube.Load())
	{
		GAMELOG << init("ERROR: CANNOT LOAD EXTREME CUBE DATA") << end;
		return false;
	}

#endif // EXTREME_CUBE

	GAMELOG << init("SYSTEM")
			<< "Load TitleSystem Data"
			<< end;
	if(!m_titleProtoList.Load())
	{
		GAMELOG << init("ERROR : CANNOT LOAD TITLE SYSTEM DATA") << end;
		return false;
	}

	GAMELOG << init("SYSTEM")
			<< "Load JewelNas Data"
			<< end;
	if(!m_jewelDataList.Load())
	{
		GAMELOG << init("ERROR : CANNOT LOAD JEWEL DATA SYSTEM DATA") << end;
		return false;
	}

#ifdef EVENT_VALENTINE
	gserver->m_bValentineEvent= true;
#endif

#ifdef EVENT_WHITEDAY
	gserver->m_bWhiteDayEvent = true;
#endif

//0704 이벤트에 관한 공지 세팅.
#ifdef NOTICE_EVENT
	i =0;
	memset(m_aNotice, 0, sizeof(int) * 5);

#ifdef EVENT_TREASUREBOX
	addNotice(i, EVENT_TREASUREBOX);

	i++;
#endif // #ifdef EVENT_TREASUREBOX

//#ifdef EVENT_TEACH
//	addNotice(i, EVENT_TEACH);
//	i++;
//#endif // #ifdef EVENT_TEACH

#ifdef EVENT_CHILDERN_DAY
	addNotice(i, EVENT_CHILDERN_DAY);
	i++;
#endif // EVENT_CHILDERN_DAY

	std::map<int, CEventInfo*> * ActiveEventList = gserver->getActiveEvenList();
	if( ActiveEventList && ActiveEventList->size() != 0 )
	{
		std::map<int, CEventInfo*>::iterator itr;
		std::map<int, CEventInfo*>::iterator itrEnd = ActiveEventList->end();
		for( itr=ActiveEventList->begin(); itr!=itrEnd; itr++)
		{
			if( itr->second->getNotice() )
			{
				addNotice(i, itr->second->getEventIndex());
				i++;
				if( i >=5 )
					break;
			}
		}
	}

#ifdef CHAOSBALL

	addNotice(i, CHAOSBALL);
	i++;

#endif

#if defined(LC_KOR)
#ifdef HANARO_EVENT
	// Config_Localize에 define 수정 필요.
	addNotice(i, HANARO_EVENT);
	i++;
#endif // HANARO_EVENT
#endif // LC_KOR

#endif // #ifdef NOTICE_EVENT

#ifdef EVENT_WORLDCUP_2010
	addNotice(i, EVENT_WORLDCUP_2010);
	i++;
#endif

#ifdef EVENT_WORLDCUP_2010_TOTO
	addNotice(i, EVENT_WORLDCUP_2010_TOTO);
	i++;
#endif

	if (i > MAX_NOTICE)
	{
		LOG_ERROR("SYSTEM ERROR: OVERFLOW NOTICE. i = %d MAX_NOTICE = %d", i, MAX_NOTICE);
	}

	m_comboZone = FindZone(ZONE_COMBO_DUNGEON);
	if (m_comboZone == NULL)
	{
		GAMELOG << init("SYSTEM ERROR: NOT FOUND COMBO DUNGEON") << end;
		return false;
	}

	// 출석 이벤트 보상 아이템 로드
	if( !m_rewardItemList.init())
	{
		GAMELOG << init("ERROR : CANNOT LOAD REWARDEVENTITEM SYSTEM DATA") << end;
		return false;
	}
	gGMCmdList->load();

	m_XmasRockPaperScissorsGame = new CRockPaperScissorsGame(27, 1);
	int xmasPuzzleItem[] = {9259, 9260, 9261, 9262, 9263, 9264, 9265, 9266, 9267};
	m_XmasPuzzleGame = new CPuzzleGame(xmasPuzzleItem, 9);

#ifdef TLD_EVENT_SONG
	if (itemExchangeProto::instance()->Load() == false)
		return false;
#endif

	HolyWaterData::instance()->load();

	if( ItemCollectionData::instance()->load() == false )
	{
		return false;
	}
	//CEquipExchangeExtend::instance()->load_();

	// Load the buffing system
	gserver->m_buffSystem.Init();
		
	return true;
}

bool CServer::ConnectDB()
{
	if (!mysql_real_connect (
				&m_dbchar,
				m_config.Find("Char DB", "IP"),
				m_config.Find("Char DB", "User"),
				m_config.Find("Char DB", "Password"),
				m_config.Find("Char DB", "DBName"),
				0, NULL, 0))
	{
		LOG_ERROR("Can't connect char DB : ip[%s] id[%s] pw[%s] dbname[%s] error[%s]",
				  m_config.Find("Char DB", "IP"),
				  m_config.Find("Char DB", "User"),
				  m_config.Find("Char DB", "Password"),
				  m_config.Find("Char DB", "DBName"),
				  mysql_error(&m_dbchar)
				 );
		return false;
	}

	if (!mysql_real_connect (
				&m_dbdata,
				m_config.Find("Data DB", "IP"),
				m_config.Find("Data DB", "User"),
				m_config.Find("Data DB", "Password"),
				m_config.Find("Data DB", "DBName"),
				0, NULL, 0))
	{
		LOG_ERROR("Can't connect data DB : ip[%s] id[%s] pw[%s] dbname[%s] error[%s]",
				  m_config.Find("Data DB", "IP"),
				  m_config.Find("Data DB", "User"),
				  m_config.Find("Data DB", "Password"),
				  m_config.Find("Data DB", "DBName"),
				  mysql_error(&m_dbdata)
				 );
		return false;
	}

	// 태국 상품 테이블을 마스터 디비로 놓을거임
#ifdef USE_TENTER_BILLING
	const char* ip = m_config.Find("Catalog DB", "IP");
	const char* user = m_config.Find("Catalog DB", "User");
	const char* pw = m_config.Find("Catalog DB", "Password");
	const char* dbname = m_config.Find("Catalog DB", "DBName");
	if (!mysql_real_connect (
				&m_dbcatal,
				ip,
				user,
				pw,
				dbname,
				0, NULL, 0))
	{
		LOG_ERROR("Can't connect catalog DB : ip[%s] id[%s] pw[%s] dbname[%s] error[%s]",
				  m_config.Find("Catalog DB", "IP"),
				  m_config.Find("Catalog DB", "User"),
				  m_config.Find("Catalog DB", "Password"),
				  m_config.Find("Catalog DB", "DBName"),
				  mysql_error(&m_dbcatal)
				 );
		return false;
	}
#endif

	CDBCmd cmdCheckCharDB;
	cmdCheckCharDB.Init(&m_dbcharingame);

	cmdCheckCharDB.SetQuery("SELECT a_etc_event FROM t_characters LIMIT 1");
	if (!cmdCheckCharDB.Open())
	{
		GAMELOG << init("Error: Character DB Check: Not Found t_characters.a_etc_event!!") << end;
		return false;
	}

// 060221 : bs : 펫 사망시간 필드 검사
	cmdCheckCharDB.SetQuery("SELECT a_time_rebirth FROM t_pet LIMIT 1");
	if (!cmdCheckCharDB.Open())
	{
		GAMELOG << init("Error: Character DB Check: Not Found t_pet.a_time_rebirth!!") << end;
		return false;
	}

	// 060227 : bs : 유료화 아이템 2차
	cmdCheckCharDB.SetQuery("SELECT a_index, a_char_index, a_item_index, a_end_time, a_skill_index, a_skill_level, a_hit0, a_hit1, a_hit2 FROM t_assist_abstime LIMIT 1");
	if (!cmdCheckCharDB.Open())
	{
		GAMELOG << init("Error: Character DB Check: Not Found t_assist_abstime!") << end;
		return false;
	}
	// 060227 : bs : 유료화 아이템 2차

	// 자동 지급 테이블

	cmdCheckCharDB.SetQuery("SELECT a_index, a_char_index, a_item_index, a_item_plus, a_item_flag, a_item_used, a_item_count, a_item_option0, a_item_option1, a_item_option2, a_item_option3, a_item_option4 FROM t_auto_give LIMIT 1");
	if (!cmdCheckCharDB.Open())
	{
		GAMELOG << init("Error: Character DB Check: Not Found t_auto_give!") << end;
		return false;
	}

	// 캐릭터 삭제 딜레이 필드
	cmdCheckCharDB.SetQuery("SELECT a_deletedelay FROM t_characters LIMIT 1");
	if (!cmdCheckCharDB.Open())
	{
		GAMELOG << init("Error: Character DB Check: Not Found t_characters.a_deletedelay!") << end;
		return false;
	}

	cmdCheckCharDB.SetQuery("SELECT "CASH_ITEM_DATE_FIELD_MEMPOS", "CASH_ITEM_DATE_FIELD_CHAR_SLOT0", "CASH_ITEM_DATE_FIELD_CHAR_SLOT1", "CASH_ITEM_DATE_FIELD_STASHEXT" FROM t_cashItemdate LIMIT 1");
	if (!cmdCheckCharDB.Open())
	{
		GAMELOG << init("Error: Character DB Check: Not Found t_cashItemdate.a_xxx_new!") << end;
		return false;
	}

	int nQuestTable;
	for (nQuestTable = 0; nQuestTable < 10; nQuestTable++)
	{
		std::string select_questdata_query = boost::str(boost::format(
				"SELECT a_char_index,a_quest_index,a_state,a_value0,a_value1,a_value2 FROM t_questdata%02d LIMIT 1") % nQuestTable);

		cmdCheckCharDB.SetQuery(select_questdata_query);
		if (!cmdCheckCharDB.Open())
		{
			GAMELOG << init("Error: Character DB Check: Not Found t_questdata") << nQuestTable << end;
			return false;
		}
	}

	for (int nInvenTable = 0; nInvenTable < 10; nInvenTable++)
	{
		std::string select_inven_query = boost::str(boost::format(
											 "SELECT a_used0_2, a_used1_2, a_used2_2, a_used3_2, a_used4_2 FROM t_inven%02d LIMIT 1") % nInvenTable);
		cmdCheckCharDB.SetQuery(select_inven_query);
		if (!cmdCheckCharDB.Open())
		{
			GAMELOG << init("Error: Character DB Check: Not Found t_inven") << nInvenTable << ".a_used_2" << end;
			return false;
		}
	}

	for (int nStashTable = 0; nStashTable < 10; nStashTable++)
	{
		std::string select_used_2_query = boost::str(boost::format(
											  "SELECT a_used_2 FROM t_stash%02d LIMIT 1") % nStashTable);
		cmdCheckCharDB.SetQuery(select_used_2_query);
		if (!cmdCheckCharDB.Open())
		{
			GAMELOG << init("Error: Character DB Check: Not Found t_stash") << nStashTable << ".a_used_2" << end;
			return false;
		}
	}

	cmdCheckCharDB.SetQuery("SELECT a_item_used_2 FROM t_auto_give LIMIT 1");
	if (!cmdCheckCharDB.Open())
	{
		GAMELOG << init("Error: Character DB Check: Not Found t_auto_give.a_item_used_2") << end;
		return false;
	}

	cmdCheckCharDB.SetQuery("SELECT * FROM t_characters_factory LIMIT 1");
	if (!cmdCheckCharDB.Open())
	{
		GAMELOG << init("Error: Character DB Check: Not Found t_characters_factory") << end;
		return false;
	}

	CDBCmd cmdCheckEventGomdori2007;
	cmdCheckEventGomdori2007.Init(&m_dbcharingame);
	cmdCheckEventGomdori2007.SetQuery("SELECT a_char_index, a_first_lose, a_first_win, a_total FROM t_event_gomdori_2007 LIMIT 1");
	if (!cmdCheckEventGomdori2007.Open())
	{
		GAMELOG << init("Error: Data DB Check : Not Found t_event_gomdori_2007") << end;
		return false;
	}

#ifdef EVENT_PARENTSDAY_2007
	CDBCmd cmdCheckEventParentsday2007;
	cmdCheckEventParentsday2007.Init(&m_dbcharingame);
	cmdCheckEventParentsday2007.SetQuery("SELECT * FROM t_event_2007_parentsday LIMIT 1");
	if (!cmdCheckEventParentsday2007.Open())
	{
		GAMELOG << init("Error: Data DB Check : Not Found t_event_2007_parentsday") << end;
		return false;
	}

	CDBCmd cmdCheckEventMay2007;
	cmdCheckEventMay2007.Init(&m_dbcharingame);
	cmdCheckEventMay2007.SetQuery("SELECT * FROM t_event_may2007 LIMIT 1");
	if (!cmdCheckEventMay2007.Open())
	{
		GAMELOG << init("Error: Data DB Check : Not Found t_event_may2007") << end;
		return false;
	}
#endif // EVENT_PARENTSDAY_2007

	CDBCmd cmdCheckPetName;
	cmdCheckPetName.Init( &m_dbcharingame );
	cmdCheckPetName.SetQuery( "SELECT * FROM t_pet_name LIMIT 1" );
	if( !cmdCheckPetName.Open() )
	{
		GAMELOG << init("Error: Char DB Check : Not Found t_pet_name") << end;
		return false;
	}

	CDBCmd cmdNewGuild;
	cmdNewGuild.Init(&m_dbcharingame);
	cmdNewGuild.SetQuery( "SELECT * FROM t_characters_guildpoint LIMIT 0" );
	if( !cmdNewGuild.Open() )
	{
		GAMELOG << init("Error: Data DB Check : Not Found t_characters_guildpoint") << end;
		return false;
	}

	cmdNewGuild.SetQuery("SELECT * FROM t_extend_guild LIMIT 0");
	if (!cmdNewGuild.Open())
	{
		GAMELOG << init("Error: Data DB Check : Not Found t_extend_guild") << end;
		return false;
	}

	cmdNewGuild.SetQuery("SELECT * FROM t_extend_guildmember LIMIT 0");
	if (!cmdNewGuild.Open())
	{
		GAMELOG << init("Error: Data DB Check : Not Found t_extend_guildmember") << end;
		return false;
	}

	cmdNewGuild.SetQuery("SELECT * FROM t_guild_notice LIMIT 0");
	if (!cmdNewGuild.Open())
	{
		GAMELOG << init("Error: Data DB Check : Not Found t_guild_notice") << end;
		return false;
	}

	CDBCmd cmdNewCastleWar;
	cmdNewCastleWar.Init(&m_dbcharingame);
	cmdNewCastleWar.SetQuery( "SELECT * FROM t_castle_guard LIMIT 0" );
	if( !cmdNewCastleWar.Open() )
	{
		GAMELOG << init("Error: Data DB Check : Not Found t_castle_guard") << end;
		return false;
	}

	cmdNewCastleWar.SetQuery("SELECT * FROM t_castle_tower LIMIT 0");
	if (!cmdNewCastleWar.Open())
	{
		GAMELOG << init("Error: Data DB Check : Not Found t_castle_tower") << end;
		return false;
	}

	cmdNewCastleWar.SetQuery("SELECT * FROM t_castle_reinforce LIMIT 0");
	if (!cmdNewCastleWar.Open())
	{
		GAMELOG << init("Error: Data DB Check : Not Found t_castle_reinforce") << end;
		return false;
	}

	cmdNewCastleWar.SetQuery("SELECT * FROM t_castle_rebrith LIMIT 0");
	if (!cmdNewCastleWar.Open())
	{
		GAMELOG << init("Error: Data DB Check : Not Found t_castle_rebrith") << end;
		return false;
	}

	CDBCmd cmdPetColor;
	cmdPetColor.Init( &m_dbcharingame );
	cmdPetColor.SetQuery( "SELECT a_color FROM t_pet LIMIT 1" );
	if( !cmdPetColor.Open() )
	{
		GAMELOG << init("Error: Character DB Check: Not Found t_pet") <<  ".a_color" << end;
		return false;
	}

	int nInven, nStash;
	for (nInven = 0; nInven < 10; nInven++)
	{
		std::string select_inven_query = boost::str(boost::format(
											 "SELECT a_socket0, a_socket1, a_socket2, a_socket3, a_socket4 FROM t_inven%02d LIMIT 1") % nInven);

		cmdCheckCharDB.SetQuery(select_inven_query);
		if (!cmdCheckCharDB.Open())
		{
			GAMELOG << init("Error: Character DB Check: Not Found t_inven") << nInven << delim << "socket" << end;
			return false;
		}
	}
	for (nStash = 0; nStash < 10 ; nStash++)
	{
		std::string select_stash_query = boost::str(boost::format(
											 "SELECT a_socket FROM t_stash%02d LIMIT 1") % nStash);

		cmdCheckCharDB.SetQuery(select_stash_query);
		if (!cmdCheckCharDB.Open())
		{
			GAMELOG << init("Error: Character DB Check: Not Found t_stash") << nStash << delim << "socket" << end;
			return false;
		}
	}
	cmdCheckCharDB.SetQuery("SELECT a_item_socket FROM t_auto_give LIMIT 1");
	if (!cmdCheckCharDB.Open())
	{
		GAMELOG << init("Error: Character DB Check: Not Found t_auto_give socket") << end;
		return false;
	}

	return true;
}

void CServer::DisconnectDB(bool bchar)
{
	if (bchar)
	{
		mysql_close (&m_dbchar);
		mysql_close (&m_dbcastle);
		mysql_close (&m_dbTrigger);
		mysql_close (&m_dbcharingame);
	}
	else
	{
		mysql_close (&m_dbdata);
	}
}

bool CServer::InitGame()
{
	BOOST_STATIC_ASSERT(MSG_MSGR_END_MSG < 255);
	BOOST_STATIC_ASSERT(MSG_CONN_END_MSG < 255);
	BOOST_STATIC_ASSERT(MSG_HELPER_END_MSG < 255);
	BOOST_STATIC_ASSERT(MSG_EVENT_END_MSG < 255);
	BOOST_STATIC_ASSERT(MSG_FAIL_END_MSG < 255);
	BOOST_STATIC_ASSERT(MSG_SYS_END_MSG < 255);
	BOOST_STATIC_ASSERT(MSG_ITEM_END_MSG < 255);

	ServerSrandom(time(0));

	InitExp();

	gcmdlist.AddMessage();
	gexcmdlist.AddExMessage();

	GAMELOG << init("SYSTEM")
			<< "Finding player limit."
			<< end;
	m_maxplayers = GetMaxPlayers();

	GAMELOG << init("SYSTEM")
			<< "Opening mother connection."
			<< end;

	SetHeaders();

#ifdef DEV_DEATH_EVENT_SAVEFILE
	FILE* fpdeath = fopen(DEV_DEATH_EVENT_SAVEFILE, "rt");
	if(fpdeath)
	{
		long start = 0;
		long end = 0;
		bool bFail = false;
		char tmpBuf[MAX_STRING_LENGTH] = {0,};

		if(fgets(tmpBuf, 256, fpdeath) == NULL) bFail = true;
		start = atol(tmpBuf);
		if(fgets(tmpBuf, 256, fpdeath) == NULL) bFail = true;
		end = atol(tmpBuf);
		if(bFail == false)
		{
			gserver->m_tDeathEventStart = (time_t)start;
			gserver->m_tDeathEventEnd = (time_t)end;
		}
		fclose(fpdeath);
		fpdeath = NULL;
	}
#endif
	FILE* fpResetRaid = fopen(RESET_RAID_SAVEFILE, "rt");
	if(fpResetRaid)
	{
		int resetday = 0;
		int resethr = 0;
		int resetmin = 0;
		char tmpBuf[MAX_STRING_LENGTH] = {0,};

		bool bFail = false;
		if(fgets(tmpBuf, 256, fpResetRaid) == NULL)	bFail = true;
		resetday = atoi(tmpBuf);
		if(fgets(tmpBuf, 256, fpResetRaid) == NULL)	bFail = true;
		resethr = atoi(tmpBuf);
		if(fgets(tmpBuf, 256, fpResetRaid) == NULL)	bFail = true;
		resetmin = atoi(tmpBuf);
		if(bFail == false)
		{
			this->m_nResetDay = resetday;
			this->m_nResethr = resethr;
			this->m_nResetmin = resetmin;
		}
		fclose(fpResetRaid);
		fpResetRaid = NULL;
	}
#ifdef DOUBLE_PET_EXP_SAVEFILE
	FILE* fpDoublePetExp = fopen(DOUBLE_PET_EXP_SAVEFILE, "rt");
	if(fpDoublePetExp)
	{
		long starttime = -1;
		long endtime = -1;
		int prob = 100;
		bool bFail = false;
		char tmpBuf[MAX_STRING_LENGTH] = {0,};
		if(fgets(tmpBuf, 256, fpDoublePetExp) == NULL) bFail = true;
		starttime = atol(tmpBuf);
		if(fgets(tmpBuf, 256, fpDoublePetExp) == NULL) bFail = true;
		endtime = atol(tmpBuf);
		if(fgets(tmpBuf, 256, fpDoublePetExp) == NULL) bFail = true;
		prob = atoi(tmpBuf);
		if(bFail == false)
		{
			gserver->m_tPetExpEventStart = (time_t)starttime;
			gserver->m_tPetExpEventEnd = (time_t)endtime;
			gserver->m_PetExpPercent = prob;
		}
		fclose(fpDoublePetExp);
		fpDoublePetExp = NULL;
	}
#endif // DOUBLE_PET_EXP_SAVEFILE
#ifdef UPGRADE_EVENT_AUTO_SAVEFILE
	FILE* fpUpgradeEvent = fopen(UPGRADE_EVENT_AUTO_SAVEFILE, "rt");
	{
		if(fpUpgradeEvent)
		{
			long starttime = -1;
			long endtime = -1;
			int prob = 100;
			bool bFail = false;
			char tmpBuf[MAX_STRING_LENGTH] = {0,};
			if(fgets(tmpBuf, 256, fpUpgradeEvent) == NULL) bFail = true;
			starttime = atol(tmpBuf);
			if(fgets(tmpBuf, 256, fpUpgradeEvent) == NULL) bFail = true;
			endtime = atol(tmpBuf);
			if(fgets(tmpBuf, 256, fpUpgradeEvent) == NULL) bFail = true;
			prob = atoi(tmpBuf);
			if(bFail == false)
			{
				gserver->m_tUpgradeEventStart = (time_t)starttime;
				gserver->m_tUpgradeEventEnd = (time_t)endtime;
				g_nUpgradeProb = prob;
			}
			fclose(fpUpgradeEvent);
			fpUpgradeEvent = NULL;
		}
	}
#endif // UPGRADE_EVENT_AUTO_SAVEFILE
#ifdef GM_EXP_LIMIT_AUTO_SAVEFILE
	FILE* fpExpLimit = fopen(GM_EXP_LIMIT_AUTO_SAVEFILE, "rt");
	if(fpExpLimit)
	{
		int i;
		int nLimitStart;
		int nLimitEnd;
		int starttime[6];
		int endtime[6];
		bool bFail = false;
		memset(starttime, -1, sizeof(starttime));
		memset(endtime, -1, sizeof(endtime));
		char tmpBuf[MAX_STRING_LENGTH] = {0,};

		if(fgets(tmpBuf, 256, fpExpLimit) == NULL) bFail = true;
		nLimitStart = atoi(tmpBuf);
		if(fgets(tmpBuf, 256, fpExpLimit) == NULL) bFail = true;
		nLimitEnd = atoi(tmpBuf);

		for(i = 0; i < 6; i++)
		{
			if(fgets(tmpBuf, 256, fpExpLimit) == NULL) bFail = true;
			starttime[i] = atoi(tmpBuf);
		}
		for(i = 0; i < 6; i++)
		{
			if(fgets(tmpBuf, 256, fpExpLimit) == NULL) bFail = true;
			endtime[i] = atoi(tmpBuf);
		}

		if(bFail == false)
		{
			m_nExpLimitStart = nLimitStart;
			m_nExpLimit = m_nExpLimitEnd = nLimitEnd;
			m_nSpLimitStart = nLimitStart;
			m_nSpLimit = m_nSpLimitEnd = nLimitEnd;
			for(i = 0; i < 6; i++)
			{
				m_nGMExpLimitStart[i] = starttime[i];
				m_nGMExpLimitEnd[i] = endtime[i];
			}
		}
		fclose(fpExpLimit);
		fpExpLimit = NULL;
	}
#endif // GM_EXP_LIMIT_AUTO_SAVEFILE
#ifdef EVENT_ITEMDROP_AUTO_SAVEFILE
	FILE* fpItemdrop = fopen(EVENT_ITEMDROP_AUTO_SAVEFILE, "rt");
	if(fpItemdrop)
	{
		int i;
		int rate;
		int starttime[6];
		int endtime[6];
		bool bFail = false;
		memset(starttime, -1, sizeof(starttime));
		memset(endtime, -1, sizeof(endtime));
		char tmpBuf[MAX_STRING_LENGTH] = {0,};

		if(fgets(tmpBuf, 256, fpItemdrop) == NULL) bFail = true;
		rate = atoi(tmpBuf);

		for(i = 0; i < 6; i++)
		{
			if(fgets(tmpBuf, 256, fpItemdrop) == NULL)
				bFail = true;
			starttime[i] = atoi(tmpBuf);
		}
		for(i = 0; i < 6; i++)
		{
			if(fgets(tmpBuf, 256, fpItemdrop) == NULL)
				bFail = true;
			endtime[i] = atoi(tmpBuf);
		}
		if(bFail == false)
		{
			gserver->m_nItemDropEventRateBackup = rate;
			for(i = 0; i < 6; i++)
			{
				m_iEventItemdropStart[i] = starttime[i];
				m_iEventItemdropEnd[i] = endtime[i];
			}
		}
		fclose(fpItemdrop);
		fpItemdrop = NULL;
	}
#endif // EVENT_ITEMDROP_AUTO_SAVEFILE

#ifdef NEW_DOUBLE_EVENT_AUTO_SAVEFILE
	FILE* fpDoubleEventAuto = fopen(NEW_DOUBLE_EVENT_AUTO_SAVEFILE, "rt");
	if(fpDoubleEventAuto)
	{
		int i;
		int starttime[6];
		int endtime[6];
		bool bFail = false;
		memset(starttime, -1, sizeof(starttime));
		memset(endtime, -1, sizeof(endtime));
		char tmpBuf[MAX_STRING_LENGTH] = {0,};
		for(i = 0; i < 6; i++)
		{
			if(fgets(tmpBuf, 256, fpDoubleEventAuto) == NULL)
				bFail = true;
			starttime[i] = atoi(tmpBuf);
		}
		for(i = 0; i < 6; i++)
		{
			if(fgets(tmpBuf, 256, fpDoubleEventAuto) == NULL)
				bFail = true;
			endtime[i] = atoi(tmpBuf);
		}

		if(bFail == false)
		{
			for(i = 0; i < 6; i++)
			{
				m_iDoubleEventStart[i] = starttime[i];
				m_iDoubleEventEnd[i] = endtime[i];
			}
		}
		fclose(fpDoubleEventAuto);
		fpDoubleEventAuto = NULL;
	}
#endif // NEW_DOUBLE_EVENT_AUTO_SAVEFILE

#ifdef NEW_DOUBLE_GM_SAVEFILE
	FILE* fpDoubleEvent = fopen(NEW_DOUBLE_GM_SAVEFILE, "rt");
	if (fpDoubleEvent)
	{
		int 		nDoubleNasPercent;
		int 		nDoubleNasGetPercent;
		int 		nDoubleExpPercent;
		int 		nDoubleSpPercent;
		int 		nDoubleProducePercent;
		int 		nDoubleProduceNum;
#ifdef NEW_DOUBLE_GM_AUTO
		int i;
		int			starttime[6];
		int			endtime[6];
		memset(starttime, -1, sizeof(starttime));
		memset(endtime, -1, sizeof(endtime));
#endif
		char tmpBuf[MAX_STRING_LENGTH] = {0,};

		bool bFail = false;

		if (fgets(tmpBuf, 256, fpDoubleEvent) == NULL)		bFail = true;
		nDoubleNasPercent		= atoi(tmpBuf);

		if (fgets(tmpBuf, 256, fpDoubleEvent) == NULL)		bFail = true;
		nDoubleNasGetPercent	= atoi(tmpBuf);

		if (fgets(tmpBuf, 256, fpDoubleEvent) == NULL)		bFail = true;
		nDoubleExpPercent		= atoi(tmpBuf);

		if (fgets(tmpBuf, 256, fpDoubleEvent) == NULL)		bFail = true;
		nDoubleSpPercent		= atoi(tmpBuf);

		if (fgets(tmpBuf, 256, fpDoubleEvent) == NULL)		bFail = true;
		nDoubleProducePercent	= atoi(tmpBuf);

		if (fgets(tmpBuf, 256, fpDoubleEvent) == NULL)		bFail = true;
		nDoubleProduceNum		= atoi(tmpBuf);
#ifdef NEW_DOUBLE_GM_AUTO
		bool bReadError = false;
		for(i = 0; i < 6; i++)
		{
			if(fgets(tmpBuf, 256, fpDoubleEvent) == NULL)
				bReadError = true;
			starttime[i]		= atoi(tmpBuf);
		}
		for(i = 0; i < 6; i++)
		{
			if(fgets(tmpBuf, 256, fpDoubleEvent) == NULL)
				bReadError = true;
			endtime[i]			= atoi(tmpBuf);
		}
#endif // NEW_DOUBLE_GM_AUTO

		if (!bFail)
		{
			m_bDoubleEvent = true;
#ifdef NEW_DOUBLE_GM_AUTO
			m_bDoubleNasPercentBackup			= nDoubleNasPercent;
			m_bDoubleNasGetPercentBackup		= nDoubleNasGetPercent;
			m_bDoubleExpPercentBackup			= nDoubleExpPercent;
			m_bDoubleSpPercentBackup			= nDoubleSpPercent;
			m_bDoubleProducePercentBackup		= nDoubleProducePercent;
			m_bDoubleProduceNumBackup			= nDoubleProduceNum;

			for(i = 0; i < 6; i++)
			{
				m_iDoubleGMStart[i] = starttime[i];
				m_iDoubleGMEnd[i] = endtime[i];
			}

			if ( m_bDoubleNasPercentBackup < 0
					|| m_bDoubleNasGetPercentBackup  < 0
					|| m_bDoubleExpPercentBackup  < 0
					|| m_bDoubleSpPercentBackup  < 0
					|| m_bDoubleProducePercentBackup  < 0
					|| m_bDoubleProduceNumBackup  < 0
					|| m_bDoubleNasPercentBackup > 200
					|| m_bDoubleNasGetPercentBackup > 200
					|| m_bDoubleExpPercentBackup  > 200
					|| m_bDoubleSpPercentBackup  > 200
					|| m_bDoubleProducePercentBackup  > 200
					|| m_bDoubleProduceNumBackup  > 10 )
			{
				m_bDoubleNasPercentBackup = DEFAULT_NAS_PERCENT;
				m_bDoubleNasGetPercentBackup = DEFAULT_NAS_GET_PERCENT;
				m_bDoubleExpPercentBackup = DEFAULT_EXP_PERCENT;
				m_bDoubleSpPercentBackup = DEFAULT_SP_PERCENT;
				m_bDoubleProducePercentBackup = DEFAULT_PRO_PERCENT;
				m_bDoubleProduceNumBackup = DEFAULT_PRO_GET_NUMBER;

				GAMELOG << init("WARNING") << "USING DEFAULT DOUBLE EVENT VALUE" << end;
			}
#else
			m_bDoubleNasPercent			= nDoubleNasPercent;
			m_bDoubleNasGetPercent		= nDoubleNasGetPercent;
			m_bDoubleExpPercent			= nDoubleExpPercent;
			m_bDoubleSpPercent			= nDoubleSpPercent;
			m_bDoubleProducePercent		= nDoubleProducePercent;
			m_bDoubleProduceNum			= nDoubleProduceNum;

			if ( m_bDoubleNasPercent < 0
					|| m_bDoubleNasGetPercent  < 0
					|| m_bDoubleExpPercent  < 0
					|| m_bDoubleSpPercent  < 0
					|| m_bDoubleProducePercent  < 0
					|| m_bDoubleProduceNum  < 0
					|| m_bDoubleNasPercent > 200
					|| m_bDoubleNasGetPercent > 200
					|| m_bDoubleExpPercent  > 200
					|| m_bDoubleSpPercent  > 200
					|| m_bDoubleProducePercent  > 200
					|| m_bDoubleProduceNum  > 10 )
			{
				m_bDoubleNasPercent = DEFAULT_NAS_PERCENT;
				m_bDoubleNasGetPercent = DEFAULT_NAS_GET_PERCENT;
				m_bDoubleExpPercent = DEFAULT_EXP_PERCENT;
				m_bDoubleSpPercent = DEFAULT_SP_PERCENT;
				m_bDoubleProducePercent = DEFAULT_PRO_PERCENT;
				m_bDoubleProduceNum = DEFAULT_PRO_GET_NUMBER;

				GAMELOG << init("WARNING") << "USING DEFAULT DOUBLE EVENT VALUE" << end;
			}
#endif // NEW_DOUBLE_GM_AUTO
		}

		fclose(fpDoubleEvent);
		fpDoubleEvent = NULL;
	}
#endif // NEW_DOUBLE_GM_SAVEFILE

	SetDungeonTime();

	if( gserver->isActiveEvent( A_EVENT_XMAS ) )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ConnEventXmas2007Msg( rmsg, MSG_CONN_EVENT_XMASTREE_POINT );
		SEND_Q( rmsg, gserver->m_connector );
	}

	this->m_RoyalRumble.Init();

	if(!m_Fortunelist.Make_fortune_proto_list(&m_dbdata))
		GAMELOG << init("SYS_ERR")
				<< "Fortune Costume Loading Failed..."
				<< end;
	GAMELOG << init("SYSTEM")
			<< "Entering game loop."
			<< end;

	return true;
}

int CServer::GetMaxPlayers()
{
#ifndef CIRCLE_UNIX
	return MAX_PLAYING;
#else
	int max_descs = 0;
	const char *method;
	/*
	* First, we'll try using getrlimit/setrlimit.  This will probably work
	* on most systems.  HAS_RLIMIT is defined in sysdep.h.
	*/
#ifdef HAS_RLIMIT
	{
		struct rlimit limit;

		/* find the limit of file descs */
		method = "rlimit";
		if (getrlimit(RLIMIT_NOFILE, &limit) < 0)
		{
			GAMELOG << init("SYS_ERR") << "calling getrlimit" << end;
			exit(1);
		}

		/* set the current to the maximum */
		limit.rlim_cur = limit.rlim_max;
		if (setrlimit(RLIMIT_NOFILE, &limit) < 0)
		{
			GAMELOG << init("SYS_ERR") << "calling setrlimit" << end;
			exit(1);
		}
#ifdef RLIM_INFINITY
		if (limit.rlim_max == RLIM_INFINITY)
			max_descs = MAX_PLAYING + NUM_RESERVED_DESCS;
		else
			max_descs = MIN(MAX_PLAYING + NUM_RESERVED_DESCS, limit.rlim_max);
#else
		max_descs = MIN(MAX_PLAYING + NUM_RESERVED_DESCS, limit.rlim_max);
#endif
	}
#elif defined (OPEN_MAX) || defined(FOPEN_MAX)
#if !defined(OPEN_MAX)
#define OPEN_MAX FOPEN_MAX
#endif
	method = "OPEN_MAX";
	max_descs = OPEN_MAX;		/* Uh oh.. rlimit didn't work, but we have
	* OPEN_MAX */
#elif defined (_SC_OPEN_MAX)
	/*
	* Okay, you don't have getrlimit() and you don't have OPEN_MAX.  Time to
	* try the POSIX sysconf() function.  (See Stevens' _Advanced Programming
	* in the UNIX Environment_).
	*/
	method = "POSIX sysconf";
	errno = 0;
	if ((max_descs = sysconf(_SC_OPEN_MAX)) < 0)
	{
		if (errno == 0)
			max_descs = m_MaxPlaying + NUM_RESERVED_DESCS;
		else
		{
			GAMELOG << init("SYS_ERR") << "Error calling sysconf" << end;
			exit(1);
		}
	}
#else
	/* if everything has failed, we'll just take a guess */
	method = "random guess";
	max_descs = m_MaxPlaying + NUM_RESERVED_DESCS;
#endif

	/* now calculate max _players_ based on max descs */
	max_descs = MIN(MAX_PLAYING, max_descs - NUM_RESERVED_DESCS);

	if (max_descs <= 0)
	{
		GAMELOG << init("SYS_ERR") << "Non-positive max player limit!  (Set at" << max_descs << "using" << method << ")." << end;
		exit(1);
	}

	GAMELOG << init("SYSTEM") << "Setting player limit to" << max_descs << "using" << method << "." << end;

	return (max_descs);
#endif /* CIRCLE_UNIX */
}

void CServer::SetHeaders()
{
	int* zones = new int[m_numZone];
	int count = 0;
	int i;
	for (i = 0; i < m_numZone; i++)
	{
		if (m_zones[i].m_bRemote)
			continue ;
		zones[count++] = m_zones[i].m_index;
	}

	MsgrConnectMsg(m_messengerConnMsg, SERVER_VERSION, m_serverno, m_subno, count, zones);
	ConnConnectMsg(m_connectorConnMsg, SERVER_VERSION, m_serverno, m_subno, count, zones);
	HelperConnectMsg(m_helperConnMsg, SERVER_VERSION, m_serverno, m_subno, count, zones);
	SubHelperConnectMsg(m_subHelperConnMsg, SERVER_VERSION, m_serverno, m_subno, count, zones);
	delete [] zones;
	zones = NULL;
}

void CServer::CloseSocket(CDescriptor* d)
{
	if (m_descLoop == d)
	{
		m_descLoop = NULL;
		m_bLoop = 0;
	}

	if (m_observer == d)
	{
		m_observer = NULL;
	}

	for (int i = 0; i < 20; i++)
	{
		if (m_chatmonitor[i] == d)
		{
			m_chatmonitor[i] = NULL;
		}
	}
#ifdef GMTOOL
	for(int i = 0; i < gserver->m_nGMToolCount; ++i)
	{
		if(gserver->m_gmtool[i] == d)
		{
			gserver->m_gmtool[i] = NULL;
		}
	}
#endif // GMTOOL

	if (d->m_pChar)
	{
		d->m_pChar->ProcDisconnect(true, false);
		d->m_pChar->OutputDBItemLog();
	}

	if (d->m_pChar)
	{
		if (PCManager::instance()->isPlaying(d->m_pChar))
		{
			d->m_pChar->m_bPlaying = false;
			// remove form player list..

			PCManager::instance()->deletePlayerByPC(d->m_pChar);

			// 돈 가진거 로그 남기기
			GAMELOG << init("HAVE_MONEY", d->m_pChar->m_name, d->m_pChar->m_nick, d->m_idname)
					<< d->m_pChar->m_inventory.getMoney()
					<< end;

			// 착용 아이템 로그 남기기
			for (int i = 0; i < MAX_WEARING; i++)
			{
				if (d->m_pChar->m_wearInventory.wearItemInfo[i])
				{
					CItem* p = d->m_pChar->m_wearInventory.wearItemInfo[i];
					GAMELOG << init("WEAR ITEM", d->m_pChar->m_name, d->m_pChar->m_nick, d->m_idname)
							<< itemlog(p)
							<< end;
				}
			}

			// 소유 펫 로그 남기기
			CPet* petNext = d->m_pChar->m_petList;
			CPet* pet;
			while ((pet = petNext))
			{
				petNext = pet->m_nextPet;
				// TODO : petlog
				GAMELOG << init("PET INFO", d->m_pChar)
						<< "PET" << delim
						<< pet->m_index << delim
						<< pet->GetPetType() << delim
						<< pet->GetPetGrade() << delim
						<< "LEVEL" << delim
						<< pet->m_level << delim
						<< "AP" << delim
						<< pet->GetAbilityPoint()
						<< end;
			}

			CAPet *apet_next = d->m_pChar->m_pApetlist;
			CAPet *apet;
			while ((apet = apet_next))
			{
				apet_next = apet->m_pNextPet;
				GAMELOG << init("APET_INPO", d->m_pChar )
						<< "APET" << apet->m_index << delim << apet->m_name << delim << apet->m_pProto->Index() << delim
						<< "LEVEL" << delim << apet->m_level << delim << "EXP" << delim << apet->m_exp << end ;
			}

			// 캐쉬 아이템 버프 로그 남기기
			int nAssistABSTypeCount = d->m_pChar->m_assist.GetABSTimeTypeList(NULL);
			if (nAssistABSTypeCount > 0)
			{
				int* nAssistABSItemIndex = new int[nAssistABSTypeCount];
				int* nAssistABSSkillIndex = new int[nAssistABSTypeCount];
				int* nAssistABSSkillLevel = new int[nAssistABSTypeCount];
				char* nAssistABSHit0 = new char[nAssistABSTypeCount];
				char* nAssistABSHit1 = new char[nAssistABSTypeCount];
				char* nAssistABSHit2 = new char[nAssistABSTypeCount];
				int* nAssistABSEndTime = new int[nAssistABSTypeCount];

				d->m_pChar->m_assist.GetABSTimeTypeList(nAssistABSItemIndex, nAssistABSSkillIndex, nAssistABSSkillLevel, nAssistABSHit0, nAssistABSHit1, nAssistABSHit2, nAssistABSEndTime);
				for(int i = 0; i < nAssistABSTypeCount; i++)
				{
					GAMELOG << init("CASH_ABS_ASSIST", d->m_pChar)
							<< nAssistABSItemIndex[i] << delim
							<< nAssistABSSkillIndex[i] << delim
							<< nAssistABSSkillLevel[i] << delim
							<< nAssistABSHit0[i] << delim
							<< nAssistABSHit1[i] << delim
							<< nAssistABSHit2[i] << delim
							<< nAssistABSEndTime[i] << end;
				}
			}
			// 메모리 스크롤 개인 창고 확장 카드 남은 시간 로그 남기기
			GAMELOG << init("CASH_MEMPOS_STASH_TIME", d->m_pChar)
					<< d->m_pChar->m_memposTime << delim
					<< d->m_pChar->m_stashextTime << end;

#ifdef GER_LOG
			//const int BUFSIZE = 128;
			//char buf[BUFSIZE]="";
			//d->m_pChar->GetCalcInGameTime(buf, BUFSIZE);

			GAMELOGGEM << init( 0, "CHAR_LEAVE_GAME")
					   << LOG_VAL("account-id", d->m_idname) << blank
					   << LOG_VAL("char-id", d->m_pChar->m_name ) << blank
					   //<< LOG_VAL("in-game",buf) << blank
					   << endGer;

			GAMELOGGEM << init( 0, "ACCOUNT_LOGOUT")
					   << LOG_VAL("account-id", d->m_idname) << blank
					   << LOG_VAL("char-id", d->m_pChar->m_name ) << blank
					   //<< LOG_VAL("in-game", buf) << blank
					   << endGer;
			// 최종 게임한 시간 계정 필요.
#endif // GER_LOG
			GAMELOG << init("DISCONNECT", d->m_pChar->m_name, d->m_pChar->m_nick, d->m_idname)
					<< end;

			//이보케이션 상태 풀기
			if(d->m_pChar->m_evocationIndex > EVOCATION_NONE)
			{
				d->m_pChar->Unevocation();
			}
			LOG_INFO("LOGOUT_INFO > name[%s], exp[%d], sp[%d], fp[%d]", d->m_pChar->GetName(), d->m_pChar->m_exp, d->m_pChar->m_skillPoint, d->m_pChar->m_syndicateManager.getSyndicatePoint(d->m_pChar->getSyndicateType()));
		}
		// TODO - 캐릭터의 정보를 DB thread로 보내어 저장하도록 함
		DBManager::instance()->SaveCharacterInfo(d, true);
	}

	if (d->m_idname[0] != '\0')
	{
		{
			struct tm connecttm;
			memcpy(&connecttm, localtime(&d->m_connect_time), sizeof(connecttm));

			struct tm disconnecttm;
			memcpy(&disconnecttm, localtime(&gserver->m_nowseconds), sizeof(disconnecttm));

			LOG_INFO("TOTAL CONNECTION TIME : userIndex : %d userId : %s time : %04d-%02d-%02d %02d:%02d:%02d - %04d-%02d-%02d %02d:%02d:%02d total_sec : %d sec",
					 d->m_index, (const char *)d->m_idname,
					 connecttm.tm_year + 1900, connecttm.tm_mon + 1, connecttm.tm_mday,
					 connecttm.tm_hour, connecttm.tm_min, connecttm.tm_sec,
					 disconnecttm.tm_year + 1900, disconnecttm.tm_mon + 1, disconnecttm.tm_mday,
					 disconnecttm.tm_hour, disconnecttm.tm_min, disconnecttm.tm_sec,
					 (gserver->m_nowseconds - d->m_connect_time)
					);
		}

		// 로그인은 진행을 하고, 인맵을 하지 않을때 처리
		// 만약 인맵을 했다면, 데이터 저장 후 처리
		if (d->m_pChar == NULL)
		{
			// Connector Server로 로그아웃 보내기
			// Connector Server에서는 id를 찾지 못하면 return함
			CNetMsg::SP rmsg(new CNetMsg);
			ConnLogoutMsg(rmsg, d->m_idname);
			SEND_Q(rmsg, m_connector);

			LOG_INFO("send logout msg to Connector : user_index[%d] id[%s]", d->m_index, (const char *)d->m_idname);
			
			{
				// Helper, SubHelper에게 접속 정보를 전달
				CNetMsg::SP rmsg(new CNetMsg);
				ServerToServerPacket::makeLogoutUserInfo(rmsg, d->m_index);
				SEND_Q(rmsg, gserver->m_helper);
				SEND_Q(rmsg, gserver->m_subHelper);
			}
		}
	}
}

void CServer::CommandInterpreter(CDescriptor* d, CNetMsg::SP& msg)
{
	CPC* ch = d->m_pChar;

	if (gcmdlist.Find(msg->m_mtype) == false)
	{
		LOG_ERROR("HACKING : invalid command[%d]. charIndex[%d]", msg->m_mtype, ch->m_index);
		d->Close("Invalid command");
		return;
	}

	if (msg->m_mtype != MSG_MOVE)
	{
		if (m_pulse - d->m_checktics >= PULSE_REAL_SEC)
		{
			// 1초마다 공격 및 명령 카운터를 초기화 한다.
			// 즉, 1초에 몇개의 명령을 수행할 수 있는지 정하고
			// 이곳에서 초기화 되기 전에 명령의 개수를 넘기는 경우 핵으로 간주하면 된다
			if (d->m_commandcount > 20)
			{
				if (d->m_pChar)
					GAMELOG << init("HACK COMMAND", d->m_pChar->m_name, d->m_pChar->m_nick, d->m_idname);
				else
					GAMELOG << init("HACK COMMAND", d->m_idname);
				GAMELOG << d->m_commandcount
						<< end;

				// TODO : 핵 카운트 증가
				if( d->IncreaseHackCount(1) == true)
				{
					return;
				}
			}
			d->m_checktics = m_pulse;
			d->m_commandcount = 0;
		}

		d->m_commandcount++;
	}

	pTypeBase* pbase = reinterpret_cast<pTypeBase*>(msg->m_buf);
	int debug_type = pbase->type;
	int debug_subtype = pbase->subType;
	
	//if(debug_type != MSG_MOVE)
		//LOG_INFO("DEBUG_FUNC : START : type : %d : subtype : %d", debug_type, debug_subtype);
	
	gcmdlist.Run(msg->m_mtype, ch, msg);
	
	//if(debug_type != MSG_MOVE)
		//LOG_INFO("DEBUG_FUNC : END : type : %d : subtype : %d", debug_type, debug_subtype);
}

void CServer::DecreaseTimeForPC(CPC* ch)
{
	/////////////////////////////////////////////
	// BANGWALL : 2005-06-27 오후 7:09:33
	// Comment : pd4 시간처리

	if(ch->m_pd4StartTime > 0)
	{
		ch->m_pd4StartTime -= TIME_ONE_SECOND;
		if(ch->m_pd4StartTime <= 0 )
		{
			CQuest* pQueset = ch->m_questList.FindQuest(105);
			if (pQueset && pQueset->GetQuestState() == QUEST_STATE_RUN)
			{
				{
					CNetMsg::SP rmsg(new CNetMsg);
					QuestFailMsg(rmsg, pQueset);
					SEND_Q(rmsg, ch->m_desc);
				}

				//do_Quest(ch, msg);

				{
					///////////////////////////////////
					// pd4 퀘스트이면 helper 랭킹에 등록 : BW
					//
					// char index를 보냄
					CNetMsg::SP rmsg(new CNetMsg);

					rmsg->Init(MSG_HELPER_COMMAND);
					RefMsg(rmsg) << MSG_HELPER_PD4_RANK_END
								 << ch->m_index
								 << pQueset->GetQuestValue(0)
								 << 0;

					GAMELOG << init("PD4 END", ch->m_name, ch->m_nick, ch->m_desc->m_idname)
							<< pQueset->GetQuestValue(0) << end;

					if (gserver->isRunHelper())
					{
						SEND_Q(rmsg, gserver->m_helper);
					}
					else
					{
						GAMELOG << init("PD4 QUEST ERROR : RANK END ERROR NOT HELPER")
								<< ch->m_index << end;
					}
				}
			}

			ch->m_pd4StartTime = 0;
		}
	}

	if (ch->IsSetPlayerState(PLAYER_STATE_PKMODEDELAY))
	{
		if (ch->m_pkmodedelay > 0)
		{
			ch->m_pkmodedelay -= TIME_ONE_SECOND;
			if (ch->m_pkmodedelay < 0)
				ch->m_pkmodedelay = 0;
		}
		else
		{
			ch->ResetPlayerState(PLAYER_STATE_PKMODE | PLAYER_STATE_PKMODEDELAY);
			ch->m_pkmodedelay = 0;
			CNetMsg::SP rmsg(new CNetMsg);
			ResponseClient::makeAction(rmsg, ch, 0, AGT_PKMODE);
			ch->m_pArea->SendToCell(rmsg, ch, true);
		}
	}

	// 사제 시스템 시간 체크
	if (ch->m_teachType != MSG_TEACH_NO_TYPE && ch->m_teachType != MSG_TEACH_NO_STUDENT_TYPE )
	{
		// 견습생이 접속한지 10일이 지났다면 관계를 해지로 변경한다.
		if( ch->m_teachType == MSG_TEACH_TEACHER_TYPE  )
		{
			// 초고제 받으라는 메세지
			if( ch->m_displayCanSstone
					&& ((ch->m_fame >= 200  && ch->m_superstone < 1 )
						|| (ch->m_fame >= 300 && ch->m_superstone < 2 )
						|| (ch->m_fame >= 500 && ch->m_superstone < 3)
						|| (ch->m_fame >= 800 && ch->m_superstone < 4)
						|| (ch->m_fame >= 1200 && ch->m_superstone < 5)
						|| (ch->m_fame >= 1700 && ch->m_superstone < 6)
						|| (ch->m_fame >= 2300 && ch->m_superstone < 7)) )
			{
				CNetMsg::SP rmsg(new CNetMsg);
				SysEnableSuperStoneMsg(rmsg, ch->m_fame);
				SEND_Q(rmsg, ch->m_desc);
				ch->m_displayCanSstone = false;
			}
		}
		else if( ch->m_teachType == MSG_TEACH_LIMITE_DAY_FAIL)  // 사제 관계가 10일이 지나 끊어진 견습생인 경우
		{
			if( ch->m_teachTime[0] >= TEACH_STUDENT_MAX_GIVEUP )
				ch->m_teachType = MSG_TEACH_NO_STUDENT_TYPE;
			else
				ch->m_teachType = MSG_TEACH_NO_TYPE;

			{
				CNetMsg::SP rmsg(new CNetMsg);
				TeachEndMsg(rmsg, -1, "none", ch->m_index, ch->m_name, MSG_TEACH_END_FAIL);
				SEND_Q(rmsg, ch->m_desc);
			}

			{
				CNetMsg::SP rmsg(new CNetMsg);
				HelperTeacherGiveup(rmsg, -1, ch->m_index, ch->m_teachType, ch->m_teachTime[0]);
				SEND_Q(rmsg, gserver->m_helper);
			}
		}
	}

	// 정당방위일때
	if (ch->IsSetPlayerState(PLAYER_STATE_RAMODE))
	{
		// 정당방위 리스트가 있다
		bool bHaveTarget = false;

		CRaChar* raList;
		CRaChar* raListNext = ch->m_raList;

		while ((raList = raListNext))
		{
			raListNext = raList->m_next;

			// 가해자만 해당
			if (!raList->m_bAttacker)
				continue;
			raList->m_raPulse -= TIME_ONE_SECOND;
			if (raList->m_raPulse < 0)
				raList->m_raPulse = 0;

			if (raList->m_raPulse < 1)
			{
				if( NULL == TO_PC(raList->m_raTarget) )
					continue;
				CPC* praTarget = PCManager::instance()->getPlayerByCharIndex(raList->m_raTarget->m_index);
				if (praTarget)
				{
					DelRaList(raList->m_raTarget);
				}
				else
				{
					return;
				}
				continue;
			}

			if (raList->m_raPulse == RAMODE_DELAY_PULSE)
			{
				bHaveTarget = true;
				CNetMsg::SP rmsg(new CNetMsg);
				RightAttackMsg(rmsg, raList->m_raTarget, MSG_RIGHT_ATTACK_DELAY);
				SEND_Q(rmsg, ch->m_desc);
				SEND_Q(rmsg, raList->m_raTarget->m_desc);
				continue;
			}
			bHaveTarget = true;
		}

		if (!bHaveTarget)
			ch->ResetPlayerState(PLAYER_STATE_RAMODE);
	}

	// 변신중일때
	if (ch->IsSetPlayerState(PLAYER_STATE_CHANGE) && ch->m_changeIndex != -1)
	{
		//ch->m_changePulse--;
		ch->m_changePulse -= TIME_ONE_SECOND;
		if (ch->m_changePulse < 0)
			ch->m_changePulse = 0;

		if (ch->m_changePulse < 1)
			ch->CancelChange();
	}

	// 1시간에 PK 성향 1포인트 회복
	if (ch->m_pkRecoverPulse != 0 && ch->m_pArea)
	{
		//ch->m_pkRecoverPulse--;
		ch->m_pkRecoverPulse -= TIME_ONE_SECOND;

		if (ch->m_pkRecoverPulse <= 0)
		{
			if (ch->m_pkPenalty < 0)
			{
				if( !gserver->m_bNonPK )
					ch->m_pkPenalty += 10;

				CNetMsg::SP rmsg(new CNetMsg);
				CharStatusMsg(rmsg, ch, 0);
				ch->m_pArea->SendToCell(rmsg, ch, false);
				ch->m_bChangeStatus = true;
			}

			if (ch->m_pkPenalty < 0)
				ch->m_pkRecoverPulse = TIME_ONE_HOUR;
			else
				ch->m_pkRecoverPulse = 0;
		}
	}

	// 현재 스킬을 쓰고 있다면, 지속 시간이 길다면, AppearMsg를 보내야 하는 상황이라면
	if (ch->m_currentSkill && ch->m_currentSkill->m_state == SKILL_CON_FIRE && ch->m_bCheckAppear)
	{
		// 타겟 검사, 이전 타겟이 있고
		if (ch->m_currentSkill->m_targetIndex != -1 && ch->m_pArea)
		{
			// 현재 타겟을 구해서
			CCharacter* tch = ch->m_pArea->FindCharInCell(ch, ch->m_currentSkill->m_targetIndex, ch->m_currentSkill->m_targetType, false);
			// 타겟이 없거나 이전과 다르면
			if (tch)
			{
				boost::scoped_array<MultiTarget> temp(new MultiTarget[ch->m_skillTargetCount]);

				for (int i = 0; i < ch->m_skillTargetCount; ++i)
				{
					temp[i].mtargettype = ch->m_targettype[i];
					temp[i].mtargetindex = ch->m_targetindex[i];
				}

				CNetMsg::SP rmsg(new CNetMsg);
				ResponseClient::makeSkillFireMsg(rmsg, ch, ch->m_currentSkill, tch, ch->m_skillTargetCount, temp.get(), 0, 0.0f, 0.0f, 0.0f, 0.0f, 0);
				ch->m_pArea->SendToCell(rmsg, ch, true);
			}
		}

		// skillfireMsg를 보냈으므로 해제
		ch->m_bCheckAppear = false;
	}

	// 스킬 취소 상태로
	// 죽거나
	// 스킬 발동이후 시간 지났을때
	if (ch->m_currentSkill
			&& (DEAD(ch) ||	(ch->m_currentSkill->m_state == SKILL_CON_FIRE && m_pulse - ch->m_currentSkill->m_usetime > ch->m_currentSkill->m_proto->m_readyTime + ch->m_currentSkill->m_proto->m_stillTime + ch->m_currentSkill->m_proto->m_fireTime)))
	{
		ch->m_currentSkill->Cancel(ch);
		ch->m_currentSkill = NULL;
	}

	// assist 시간 감소
	ch->m_assist.DecreaseTime();

	// pc 상태 회복
	if (!DEAD(ch) && m_pulse - ch->m_recoverPulse >= PC_RECOVER_PULSE)
	{
		ch->RecoverPC();
		ch->m_recoverPulse = m_pulse;
	}

	// 아이템 회복 처리
	if (ch->m_recoverHPItemTime > 0)
	{
		if (DEAD(ch))
			ch->m_recoverHPItemTime = 0;
		else
		{
			ch->m_recoverHPItemTime -= TIME_ONE_SECOND;
			if (ch->m_recoverHPItemTime < 0)
				ch->m_recoverHPItemTime = 0;
			if (ch->m_recoverHPItemTime % 10 == 0)
			{
				ch->m_hp += ch->m_recoverHPItemValue;
				if (ch->m_hp > ch->m_maxHP)
					ch->m_hp = ch->m_maxHP;
				ch->m_bChangeStatus = true;
			}
		}
	}

	if (ch->m_recoverMPItemTime > 0)
	{
		if (DEAD(ch))
			ch->m_recoverMPItemTime = 0;
		else
		{
			ch->m_recoverMPItemTime -= TIME_ONE_SECOND;
			if (ch->m_recoverMPItemTime < 0)
				ch->m_recoverMPItemTime = 0;
			if (ch->m_recoverMPItemTime % 10 == 0)
			{
				ch->m_mp += ch->m_recoverMPItemValue;
				if (ch->m_mp > ch->m_maxMP)
					ch->m_mp = ch->m_maxMP;
				ch->m_bChangeStatus = true;
			}
		}
	}

	// 빌링 아이템 처리
	if ( ch->m_billReqTime > 0)
	{
		ch->m_billReqTime -= TIME_ONE_SECOND;
		if ( ch->m_billReqTime <= 0)
		{
			ch->ResetPlayerState(PLAYER_STATE_CASHITEM);
			ch->m_billReqTime = 0;
		}
	}

	// 순간 이동 처리
	if (ch->m_reqWarpTime > 0)
	{
		if (DEAD(ch))
		{
			ch->m_reqWarpTime = 0;
			ch->m_reqWarpType = -1;
			ch->m_reqWarpData = -1;
			ch->ResetPlayerState(PLAYER_STATE_SITDOWN | PLAYER_STATE_MOVING | PLAYER_STATE_WARP);
		}
		else
		{
			ch->m_reqWarpTime -= TIME_ONE_SECOND;
			if (ch->m_reqWarpTime <= 0)
			{
				{
					CNetMsg::SP rmsg(new CNetMsg);
					WarpEndMsg(rmsg, ch);
					ch->m_pArea->SendToCell(rmsg, ch, true);
				}

				switch (ch->m_reqWarpType)
				{
				case IONCE_WARP_RETURN:
					{
						int zone;
						int zonepos;
						CZone* pZone = NULL;

						// 태국 pvp 존 이동 아이템이
						// 귀환 스크롤의 num1을 존번호로 씁니당.
						// 고로 요거 추가합니당 : bw
						if(ch->m_reqWarpData != -1 )
						{
							zone = ch->m_reqWarpData;
							zonepos = 0;
							pZone = gserver->FindZone(zone);
						}
						else
						{
							pZone = gserver->FindNearestZone(ch->m_pZone->m_index, GET_X(ch), GET_Z(ch), &zone, &zonepos);

							if(ch->m_pZone->IsComboZone())
							{
								zone = ZONE_COMBO_DUNGEON;
								pZone = gserver->FindZone(ZONE_COMBO_DUNGEON);
								zonepos = 0;
							}

							// 레이드에서 나가면 몬드샤인 마을로 간다.
							if(ch->m_pZone->m_index == ZONE_ALTER_OF_DARK
									|| ch->m_pZone->m_index == ZONE_CAPPELLA_1
									|| ch->m_pZone->m_index == ZONE_CAPPELLA_2)
							{
								zone = ZONE_MONDSHINE;
								pZone = gserver->FindZone(ZONE_MONDSHINE);
								zonepos = 0;
							}
							if(ch->m_pZone->m_index == ZONE_AKAN_TEMPLE)
							{
								zone = ZONE_EGEHA;
								pZone = gserver->FindZone(ZONE_EGEHA);
								zonepos = 0;
							}
							if(ch->m_pZone->m_index == ZONE_TARIAN_DUNGEON)
							{
								zone = ZONE_TARIAN;
								pZone = gserver->FindZone(ZONE_TARIAN);
								zonepos = 1;
							}
							if(ch->m_pZone->m_index == ZONE_RVR)
							{
								zone = ZONE_RVR;
								pZone = gserver->FindZone(ZONE_RVR);
								zonepos = ch->m_syndicateType;
							}
						}

						if (pZone == NULL)
						{
							zone = ZONE_START;
							pZone = gserver->FindZone(zone);
							zonepos = 0;
						}

						if (pZone)
						{
							// 060109 : BS : BEGIN : 존이동 메시지 사용하지 않고 직접 호출
							GoZone(ch, zone,
								   pZone->m_zonePos[zonepos][0],														// ylayer
								   GetRandom(pZone->m_zonePos[zonepos][1], pZone->m_zonePos[zonepos][3]) / 2.0f,		// x
								   GetRandom(pZone->m_zonePos[zonepos][2], pZone->m_zonePos[zonepos][4]) / 2.0f);		// z
						}
					}
					break;

				case IONCE_WARP_MEMPOS:
					ch->GoMemPos(ch->m_reqWarpData);
					break;

				case IONCE_WARP_NPC_PORTAL_SCROLL:
					ch->GoNpcPortalScroll();
					ch->m_Npc_Portal_x = -1;
					ch->m_Npc_Portal_z = -1;
					ch->m_Npc_Portal_y = -1;
					break;

				}

				ch->m_reqWarpTime = 0;
				ch->m_reqWarpType = -1;
				ch->m_reqWarpData = -1;
				ch->ResetPlayerState(PLAYER_STATE_SITDOWN | PLAYER_STATE_MOVING | PLAYER_STATE_WARP);
			}
		}
	}

	if (ch->m_reqWarpTime_skill > 0)
	{
		if (DEAD(ch))
		{
			ch->m_reqWarpTime_skill = 0;
			ch->m_reqWarpType_skill = -1;
			ch->m_reqWarpData_skill = -1;
			ch->ResetPlayerState(PLAYER_STATE_SITDOWN | PLAYER_STATE_MOVING | PLAYER_STATE_WARP);
		}
		else
		{
			ch->m_reqWarpTime_skill -= TIME_ONE_SECOND;
			if (ch->m_reqWarpTime_skill <= 0)
			{
				{
					CNetMsg::SP rmsg(new CNetMsg);
					WarpEndMsg(rmsg, ch);
					ch->m_pArea->SendToCell(rmsg, ch, true);
				}
				
				switch (ch->m_reqWarpType_skill)
				{
				case MOVE_TO_GUILDROOM:
					{
						CZone* pZone = gserver->FindZone(ZONE_GUILDROOM);
						int area_count = 0;

						if(pZone == NULL)
							break;

						// 태국 pvp 존 이동 아이템이
						// 귀환 스크롤의 num1을 존번호로 씁니당.
						// 고로 요거 추가합니당 : bw

						if (!ch->m_guildInfo || !(ch->m_guildInfo->guild()))
						{
							CNetMsg::SP rmsg(new CNetMsg);
							SysMsg(rmsg, MSG_SYS_DO_NOT_GO_ZONE_GMORDER);
							SEND_Q(rmsg, ch->m_desc);
							break;
						}

						int idx;
						for (idx=0; idx < pZone->m_countArea; idx++)
						{
							if (!pZone->m_area[idx].m_bEnable)
								continue;

							if (pZone->m_area[idx].m_guildIndex == ch->m_guildInfo->guild()->index())
							{
								area_count = idx;
								break;
							}
						}

						if (idx == pZone->m_countArea)
						{
							area_count = pZone->SetEnableArea();
							// 빈 영억 없음
							if (area_count == -1)
							{
								CNetMsg::SP rmsg(new CNetMsg);
								SysMsg(rmsg, MSG_SYS_SINGLEDUNGEON_FULL);
								SEND_Q(rmsg, ch->m_desc);
								break ;
							}
							pZone->m_area[area_count].m_guildIndex = ch->m_guildInfo->guild()->index();
						}

						if (pZone)
						{
							// 060109 : BS : BEGIN : 존이동 메시지 사용하지 않고 직접 호출
							GoZone(ch, ZONE_GUILDROOM,
								pZone->m_zonePos[0][0],														// ylayer
								GetRandom(pZone->m_zonePos[0][1], pZone->m_zonePos[0][3]) / 2.0f,		// x
								GetRandom(pZone->m_zonePos[0][2], pZone->m_zonePos[0][4]) / 2.0f);	// z
						}
					}
					break;
				}

				ch->m_reqWarpTime_skill = 0;
				ch->m_reqWarpType_skill = -1;
				ch->m_reqWarpData_skill = -1;
				ch->ResetPlayerState(PLAYER_STATE_SITDOWN | PLAYER_STATE_MOVING | PLAYER_STATE_WARP);
			}
		}
	}

	// 퍼스널 던전 입장 카운트 리셋
	time_t pdtime;
	struct tm tm1 = NOW();

	struct tm tm2;

	if ( (ch->m_pd3Count && ch->m_pd3Time) || (ch->m_pd3Count && ch->m_pd3Time == 0))
	{
		pdtime = ch->m_pd3Time;
		memcpy(&tm2, localtime(&pdtime), sizeof(struct tm));
		if (tm1.tm_yday != tm2.tm_yday)
			ch->m_pd3Count = 0;
	}
	if ( (ch->m_pd4Count && ch->m_pd4Time) || (ch->m_pd4Count && ch->m_pd4Time == 0))
	{
		pdtime = ch->m_pd4Time;
		memcpy(&tm2, localtime(&pdtime), sizeof(struct tm));
		if (tm1.tm_yday != tm2.tm_yday)
			ch->m_pd4Count = 0;
	}


	CQuest* pQuestNext = NULL, *pQuest;
	pQuestNext = ch->m_questList.GetNextQuest(NULL);
	int index_debug = 0;
	int level_debug = 0;
	int loop_count_debug = 0;
	int quest_index = 0;
	while ((pQuest = pQuestNext))
	{
		/////////////////// 디버깅 정보 swkwon (10.03.16)///////////////////
		index_debug = ch->m_index;
		level_debug = ch->m_level;
		loop_count_debug++;
		if(pQuestNext != NULL)
			quest_index = pQuestNext->GetQuestIndex();
		/////////////////// 디버깅 정보 swkwon (10.03.16)///////////////////
		pQuestNext = ch->m_questList.GetNextQuest(pQuestNext);
		switch( pQuest->GetQuestState() )
		{
		case QUEST_STATE_RUN:
		case QUEST_STATE_DONE:
			if(pQuest->GetQuestState() == QUEST_STATE_RUN && pQuest->GetQuestProto()->m_failValue > 0)
			{
				if(pQuest->GetFailValue() < gserver->getNowSecond())
				{
					// 타임어택 퀘스트 실패
					CNetMsg::SP rmsg(new CNetMsg);
					QuestFailMsg(rmsg, pQuest);
					SEND_Q(rmsg, ch->m_desc);

					ch->m_questList.DelQuest(ch, pQuest);
				}
			}

			else if((pQuest->GetQuestType1() == QTYPE_REPEAT_DAY) && (pQuest->GetQuestState() == QUEST_STATE_DONE))
			{
				// 날 넘어갔으면 삭제
				time_t nowtime;
				time(&nowtime);
				if(nowtime >= pQuest->GetCompleteTime())
				{
					ch->m_questList.DelQuest(ch, pQuest, QUEST_STATE_DONE );

					// 퀘스트 삭제 메시지 전달
				}
			}
			break;
		}
	}


	////////////////////////
	// 매초 검사해야 할 루틴
	{
		// OX 퀴즈 종료후 돌아가기
		if (ch->m_pZone->IsOXQuizRoom() && ch->m_admin < 2)
		{
			if (!gserver->m_bEventOX)
			{
				CZone* pStartZone = gserver->FindZone(ZONE_START);
				if (pStartZone == NULL)
					return;

				GoZone(ch, ZONE_START,
					   pStartZone->m_zonePos[0][0],													// ylayer
					   GetRandom(pStartZone->m_zonePos[0][1], pStartZone->m_zonePos[0][3]) / 2.0f,		// x
					   GetRandom(pStartZone->m_zonePos[0][2], pStartZone->m_zonePos[0][4]) / 2.0f);	// z
				return ;
			}
		}

		// 공성/수성 리젠 장소에서 회복 증가
		if (!DEAD(ch) && ch->m_pZone && ch->GetMapAttr() & MATT_WAR)
		{
			int joinflag = ch->GetJoinFlag(ch->m_pZone->m_index);
			if (joinflag != WCJF_NONE)
			{
				CWarCastle* castle = CWarCastle::GetCastleObject(ch->m_pZone->m_index);
				if (castle)
				{
					if (castle->GetState() != WCSF_NORMAL)
					{
						bool bIsIn = false;
						if (IS_ATTACK_TEAM(joinflag))
						{
							if (castle->IsInRegenPoint(ch, true))
								bIsIn = true;
						}
						else if (IS_DEFENSE_TEAM(joinflag))
						{
							if (castle->IsInRegenPoint(ch, false))
								bIsIn = true;
						}
						if (bIsIn)
						{
							ch->m_hp += ch->m_maxHP / 20;
							ch->m_mp += ch->m_maxMP / 20;
							if (ch->m_hp > ch->m_maxHP)
								ch->m_hp = ch->m_maxHP;
							if (ch->m_mp > ch->m_maxMP)
								ch->m_mp = ch->m_maxMP;
							ch->m_bChangeStatus = true;
							CNetMsg::SP rmsg(new CNetMsg);
							CharStatusMsg(rmsg, ch, 0);
							ch->m_pArea->SendToCell(rmsg, ch, true);
						}
					}
				}
			}
		}

		if (ch->m_bCreateMixItem)
		{
			CWarCastle* castle = CWarCastle::GetCastleObject(CWarCastle::GetCurSubServerCastleZoneIndex());
			if (!castle || !castle->IsRegenShop())
			{
				item_search_t vec;
				int sc = ch->m_inventory.searchFlagByItemProto(ITEM_FLAG_MIX, vec);
				if (sc > 0)
				{
					ch->m_inventory.deleteItem(vec, sc);
				}

				ch->m_bCreateMixItem = false;
			}
		}

		// 펫 상태 갱신
		ch->UpdatePetValue();
		ch->UpdateAPetValue();

#ifdef EVENT_SEARCHFRIEND_TIME
		// 휴면 케릭 이벤트
		if (((ch->m_bEventSearchFriendSelect == true)
				&& (ch->m_nEventSearchFriendListCount >= 1)
				&& ch->m_pulseEventSearchFriend + PULSE_REAL_MIN * 5 >= gserver->m_pulse)
				&& (ch->m_nTimeEventSearchFriend <= 216000))
		{
			ch->m_nTimeEventSearchFriend++;

			if ((ch->m_nTimeEventSearchFriend % 3600) == 0)
			{
				//1시간 간격으로 휴면케릭 사냥시 알림.
				CNetMsg::SP rmsg(new CNetMsg);
				ch->m_bGoodEventSearchFriendListImprove = true;
				EventHelperSearchFriendOneTimeCheckReqMsg(rmsg, ch->m_nTimeEventSearchFriend, ch->m_index);
				SEND_Q(rmsg, m_helper);
			}
		}
		//보상여부 갱신 되는 부분.
		if((ch->m_bGoodEventSearchFriendListImprove == false)&& ch->m_pulseEventSearchFriendList + PULSE_REAL_MIN * 5 <= gserver->m_pulse)
		{
			ch->m_bGoodEventSearchFriendListImprove = true;
		}
#endif // #ifdef EVENT_SEARCHFRIEND_TIME

		ch->m_inventory.checkItemTime();
		ch->m_wearInventory.checkItemTime();

		int title = ch->m_nCurrentTitle;

		if(!ch->m_titleList.HaveTitle(title) && title != 0)											// 이 부분은 알수없는 오류로 인한 처리 부분으로써
		{
			// 머리에 달고 있는 호칭이 리스트에 보유하고 있는 호칭이 아닐경우
			CNetMsg::SP rmsg(new CNetMsg);																			//
			ch->m_nCurrentTitle = TITLE_SYSTEM_NO_TITLE;											// 호칭을 떼어 버리는 부분이다.
			TitleSystemMsg(rmsg, MSG_EX_TITLE_SYSTEM_TITLE_CANCEL_SUCCESS, title, ch->m_index);		// 이러한 오류는 있어서는 안되는 부분인데..
			SEND_Q(rmsg, ch->m_desc);																// 예외상황의 우려로 인해 추가하게 되었음.
			ch->m_pArea->SendToCell(rmsg, ch);														//
			GAMELOG << init("TITLE SYSTEM ERROR", ch) << delim										//
					<< "This title is not exist in Title list" << delim								//
					<< "TITLE INDEX" << delim << title << end;										//
		}																							// 알수없는 오류 주석 끝.
		if(ch->m_titleList.CheckTitleTime(title))													// 여기 if문은 시간 만료 되었을때 처리하는 것.
		{
			ch->m_nCurrentTitle = TITLE_SYSTEM_NO_TITLE;
			ch->CalcStatus(true);

			{
				CNetMsg::SP rmsg(new CNetMsg);
				SysMsg(rmsg, MSG_SYS_TITLE_EXPIRED);
				RefMsg(rmsg) << 1 << title;
				SEND_Q(rmsg, ch->m_desc);
			}

			{
				CNetMsg::SP rmsg(new CNetMsg);
				TitleSystemMsg(rmsg, MSG_EX_TITLE_SYSTEM_TITLE_EXPIRED, title, ch->m_index);
				SEND_Q(rmsg, ch->m_desc);
				ch->m_pArea->SendToCell(rmsg, ch);
			}

			GAMELOG << init("TITLE EXPIRED TIME", ch) << delim
					<< "TITLE INDEX" << delim << title << end;

			{
				CNetMsg::SP rmsg(new CNetMsg);
				SubHelperTitleSystemTitleDelReq(rmsg, ch->m_index, title);
				SEND_Q(rmsg, gserver->m_subHelper);
			}
		}

#ifdef RESTRICT_PVP_SKILL
		if (ch->m_nRestrictPvPSkillDelaySec > 0)
			ch->m_nRestrictPvPSkillDelaySec--;
#endif // RESTRICT_PVP_SKILL

		// 나이트 쉐도우의 오오라 스킬
		CAssistData *  outData;
		bool outHelp = false;
		int aura_type = 0;
		int level = 0;

		if(IS_IN_CELL(ch) && ch->CanSpell() && !ch->IsSetPlayerState(PLAYER_STATE_WARP))
		{
			// 오오라를 사용하고 있는지 확인
			if (ch->m_assist.FindByType(MT_ASSIST, MST_ASSIST_AURA_DARKNESS))
				aura_type = MST_ASSIST_AURA_DARKNESS;
			else if (ch->m_assist.FindByType(MT_ASSIST, MST_ASSIST_AURA_WEAKNESS))
				aura_type = MST_ASSIST_AURA_WEAKNESS;
			else if (ch->m_assist.FindByType(MT_ASSIST, MST_ASSIST_AURA_ILLUSION))
				aura_type = MST_ASSIST_AURA_ILLUSION;

			// 오오라 사용하면 데이터 가져온다.
			if(aura_type > 0)
				level = ch->m_assist.FindByType(MT_ASSIST, aura_type, &outHelp, &outData);

			if (level > 0 && outHelp == true && outData != NULL && outData->m_remain > 0 && ch->m_pArea && ch->m_pArea->m_cell)
			{
				int auraCount = 0;
				int cx, cz;

				ch->m_pArea->PointToCellNum(GET_X(ch), GET_Z(ch), &cx, &cz);

				// 캐릭터가 있는 셀의 npc에게 자동으로 디버프를, pc에게 자동으로 버프를 건다.
				CCharacter* pChar;
				CCharacter* pCharNext = ch->m_pArea->m_cell[cx][cz].m_listChar;
				while ((pChar = pCharNext))
				{
					pCharNext = pCharNext->m_pCellNext;

					// find char
					if(IS_PC(pChar))
					{
						//지역검사 pvp존 또는 공격모드가 아니면 return;
						if( ch->CanPvP(pChar, false) == false && outData->m_proto->m_index != 766)
							continue;

						if(outData->m_proto->m_index == 766)
						{
							if( pChar->m_index != ch->m_index && TO_PC(pChar)->IsParty() == false )
								continue;

							if(ch->IsParty() == true && ch->m_party->FindMember(pChar->m_index) == -1)
								continue;

							if(ch->m_index == pChar->m_index)
								continue;
						}
					}
					else if(IS_NPC(pChar))
					{
						if( IsApllyDebuffSkilltoNPC(ch, TO_NPC(pChar) ) == false)
						{
							continue;
						}
					}
					ch->applyAuraSkill(pChar, auraCount, level, outData);

					// 오오라 카운트가 총 5회이면 loop를 빠져 나간다.
					if(auraCount >= 5)
						break;
				} // while
			} // if
		}// if

		// 캐릭터 상태가 다크니스 모드가 아니면 버프를 풀어준다.
		if(!ch->IsSetPlayerState(PLAYER_STATE_DARKNESS))
			ch->m_assist.CureAssist(MST_ASSIST_DARKNESS_MODE, 99);

		if(!ch->IsParty() && ch->m_pZone->IsPartyRaidZone())
		{
			// 파티가 아니면서 파티 레이드 존에 입장하면 내보낸다.
			CNetMsg::SP rmsg(new CNetMsg);
			RaidInzoneQuitReq(rmsg,1,0);
			do_Raid(ch, rmsg);
		}
		else if (!ch->IsExped() && ch->m_pZone->IsExpedRaidZone())
		{
			// 원정대가 아니면서 원정대 레이드 존에 입장하면 내보낸다.
			CNetMsg::SP rmsg(new CNetMsg);
			RaidInzoneQuitReq(rmsg,1,0);
			do_Raid(ch, rmsg);
		}
	}

	if(ch->GetMapAttr() & MATT_FREEPKZONE && ch->m_level > PKMODE_LIMIT_LEVEL)
	{
		if( (ch->GetPlayerState() & PLAYER_STATE_PKMODE) == false )
		{
			ch->ResetPlayerState(PLAYER_STATE_PKMODEDELAY | PLAYER_STATE_PKMODE);
			ch->SetPlayerState(PLAYER_STATE_PKMODE);
			//ch->CancelInvisible();

			//PK존에 들어가면 무적버프를 지운다.
			if (ch->m_assist.FindBySkillIndex(IMMOTAL_BUF))
				ch->m_assist.CureBySkillIndex(IMMOTAL_BUF);

			CNetMsg::SP rmsg(new CNetMsg);
			ResponseClient::makeAction(rmsg, ch, ACTION_GENERAL, AGT_PKMODE);
			ch->m_pArea->SendToCell(rmsg, ch, true);
		}

		// 지역이 바뀌면 아이템 정보 갱신을 위해 재계산은 한다 : 공성 전용 아이템을 위해 필요
		if (ch->GetMapAttr() & MATT_WAR || ch->m_recentAtt & MATT_WAR)
			ch->CalcStatus(true);

		ch->m_recentAtt = ch->GetMapAttr();
		ch->m_bChangeStatus = true;
	}

	// 공성지역이 아닌 곳에서 저레벨 PvP 모드 해제
	if (ch->m_level <= PKMODE_LIMIT_LEVEL && ch->IsSetPlayerState(PLAYER_STATE_PKMODE | PLAYER_STATE_PKMODEDELAY))
	{
		bool bPvPOff = true;
		if (ch->m_pZone->m_index == CWarCastle::GetCurSubServerCastleZoneIndex() && ch->GetMapAttr() & MATT_WAR)
		{
			CWarCastle* castle = CWarCastle::GetCastleObject(ch->m_pZone->m_index);
			if (castle && castle->GetState() != WCSF_NORMAL)
				bPvPOff = false;
		}
#ifdef FREE_PK_SYSTEM
		if( gserver->m_bFreePk )
			bPvPOff = false;
#endif // FREE_PK_SYSTEM
		if (bPvPOff)
		{
			ch->ResetPlayerState(PLAYER_STATE_PKMODE | PLAYER_STATE_PKMODEDELAY);
			CNetMsg::SP rmsg(new CNetMsg);
			ResponseClient::makeAction(rmsg, ch, ACTION_GENERAL, AGT_PKMODE);
			ch->m_pArea->SendToCell(rmsg, ch, true);
		}
	}

	ch->m_autoSkillTime -= TIME_ONE_SECOND;

	if( ch->m_hp < ch->m_maxHP * 0.7f)
	{
		if( ch->m_activeSkillList.Find( 152 ) )
		{
			if( ch->m_assist.Find(194, 1)
					|| ch->m_assist.Find(194, 2)
					|| ch->m_assist.Find(194, 3)
					|| ch->m_assist.Find(194, 4)
					|| ch->m_assist.Find(194, 5) )
			{
			}
			else
			{
				if ( ch->m_autoSkillTime < 0
						&& ch->IsSetPlayerState(PLAYER_CRISTAL_RESPOND) == false)
				{
					ch->m_autoSkillTime = PULSE_AUTO_SKILL_DECREASE;
					CNetMsg::SP rmsg(new CNetMsg);
					ResponseClient::makeSkillAutoUseMSg(rmsg, 152);
					SEND_Q(rmsg, ch->m_desc);
				}
			}
		}
	}
	else
	{
		if( ch->m_autoSkillTime < -PULSE_AUTO_SKILL_DECREASE)
			ch->m_autoSkillTime = 0;
	}

	CElemental* pElemental = NULL;
	CElemental* pElementalNext = ch->m_elementalList;
	while ((pElemental = pElementalNext))
	{
		// yhj 수정.. 엘레멘탈 NULL 체크를 안해서 서버가 내려감.. 이 NULL체크 문제가 되면 다른 방법을 사용하자.
		if(!pElemental)
			break;
		pElementalNext = pElemental->m_nextElemental;
		if (pElemental->DecreaseRemainTime())
			ch->UnsummonElemental(pElemental);
		else
			pElemental->m_assist.DecreaseTime();
	}
		
	int hp = ch->m_hp;
	int maxhp = ch->m_maxHP;

	int mp = ch->m_mp;
	int maxmp = ch->m_maxMP;

	int posioncount = 0;

	bool bHpRemain = false;	// 쿨타임
	bool bMpRemain = false;	// 쿨타임

	if (hp < maxhp * 0.5 && hp > 0)
	{
		// 체력이 절반 이하
		CAssistData *  outData = NULL;
		bool outHelp = false;
		ch->m_assist.FindByType(MT_ASSIST, MST_ASSIST_HP, &outHelp, &outData);

		if (outHelp == true
				&& outData != NULL
				&& outData->m_remain > 0)
		{
			// 체력 회복 물약을 먹었던 경우
			bHpRemain = true;
		}

		if (bHpRemain == false)
		{
			for( int k = WEARING_ACCESSORY1; k <= WEARING_ACCESSORY3; k++ )
			{
				if (ch->m_wearInventory.wearItemInfo[k] == NULL)
					continue;

				if( ch->m_wearInventory.wearItemInfo[k]->m_itemProto->getItemIndex() == ONE_PERIOD_ITEM
						|| ch->m_wearInventory.wearItemInfo[k]->m_itemProto->getItemIndex() == SEVEN_PERIOD_ITEM
						|| ch->m_wearInventory.wearItemInfo[k]->m_itemProto->getItemIndex() == THIRTY_PERIOD_ITEM
						|| ch->m_wearInventory.wearItemInfo[k]->m_itemProto->getItemIndex() == 2610
						|| ch->m_wearInventory.wearItemInfo[k]->m_itemProto->getItemIndex() == 4940
						|| ch->m_wearInventory.wearItemInfo[k]->m_itemProto->getItemIndex() == 4941
						|| ch->m_wearInventory.wearItemInfo[k]->m_itemProto->getItemIndex() == 4942
				  )
				{
					item_search_t vec;
					ch->m_inventory.searchAllItem(vec);
					item_search_t::iterator it = vec.begin();
					item_search_t::iterator endit = vec.end();
					for (; it != endit; ++it)
					{
						CItem* item = (*it).pItem;
						if (item->m_itemProto->getItemTypeIdx() != ITYPE_POTION || item->m_itemProto->getItemSubTypeIdx() != IPOTION_HP )
						{
							continue;
						}

						CNetMsg::SP rmsg(new CNetMsg);
						ResponseClient::ItemUseMsg( rmsg, item->tab(), item->getInvenIndex(), item->getVIndex(), item->getDBIndex() );
						do_Item(ch, rmsg);
						posioncount++;
						goto USE_HP_POTION;
					}
				}
			} // end for
		}
	}

USE_HP_POTION:
	if (mp < maxmp * 0.5 )
	{
		// 마나가 절반 이하
		CAssistData *  outData = NULL;
		bool outHelp = false;
		ch->m_assist.FindByType(MT_ASSIST, MST_ASSIST_MP, &outHelp, &outData);

		if (outHelp == true
				&& outData != NULL
				&& outData->m_remain > 0)
		{
			// 마나 회복 물약을 먹었던 경우
			bMpRemain = true;
		}

		if (bMpRemain == false)
		{
			for( int k = WEARING_ACCESSORY1; k <= WEARING_ACCESSORY3; k++ )
			{
				if( ch->m_wearInventory.wearItemInfo[k] )
				{
					if( ch->m_wearInventory.wearItemInfo[k]->m_itemProto->getItemIndex() == ONE_PERIOD_ITEM
							|| ch->m_wearInventory.wearItemInfo[k]->m_itemProto->getItemIndex() == SEVEN_PERIOD_ITEM
							|| ch->m_wearInventory.wearItemInfo[k]->m_itemProto->getItemIndex() == THIRTY_PERIOD_ITEM
							|| ch->m_wearInventory.wearItemInfo[k]->m_itemProto->getItemIndex() == 2610
							|| ch->m_wearInventory.wearItemInfo[k]->m_itemProto->getItemIndex() == 4940
							|| ch->m_wearInventory.wearItemInfo[k]->m_itemProto->getItemIndex() == 4941
							|| ch->m_wearInventory.wearItemInfo[k]->m_itemProto->getItemIndex() == 4942 )
					{
						item_search_t vec;
						ch->m_inventory.searchAllItem(vec);
						item_search_t::iterator it = vec.begin();
						item_search_t::iterator endit = vec.end();
						for (; it != endit; ++it)
						{
							CItem* item = (*it).pItem;
							if (item->m_itemProto->getItemTypeIdx() != ITYPE_POTION || item->m_itemProto->getItemSubTypeIdx() != IPOTION_MP )
							{
								continue;
							}
						
							CNetMsg::SP rmsg(new CNetMsg);
							ResponseClient::ItemUseMsg( rmsg, item->tab(), item->getInvenIndex(), item->getVIndex(), item->getDBIndex() );
							do_Item(ch, rmsg);
							posioncount++;

							goto USE_MP_POTION;
						}
					}
				}
			} // for( i ; i <= WEARING_ACCESSORY3; i++ )
		}
	}

USE_MP_POTION:
	if ( posioncount == 0
			&&
			((hp < maxhp * 0.5 && hp > 0) || (mp < maxmp * 0.5))
			&&  (bHpRemain == false && bMpRemain == false)
	   )

	{
		// 체력이나 마나가 50% 이하에서 사용할 포션이 없을 경우
		bool bWare = true;
		for( int k = WEARING_ACCESSORY1; k <= WEARING_ACCESSORY3; k++ )
		{
			if( ch->m_wearInventory.wearItemInfo[k] )
			{
				if( ch->m_wearInventory.wearItemInfo[k]->m_itemProto->getItemIndex() == ONE_PERIOD_ITEM
						|| ch->m_wearInventory.wearItemInfo[k]->m_itemProto->getItemIndex() == SEVEN_PERIOD_ITEM
						|| ch->m_wearInventory.wearItemInfo[k]->m_itemProto->getItemIndex() == THIRTY_PERIOD_ITEM
						|| ch->m_wearInventory.wearItemInfo[k]->m_itemProto->getItemIndex() == 2610
						|| ch->m_wearInventory.wearItemInfo[k]->m_itemProto->getItemIndex() == 4940
						|| ch->m_wearInventory.wearItemInfo[k]->m_itemProto->getItemIndex() == 4941
						|| ch->m_wearInventory.wearItemInfo[k]->m_itemProto->getItemIndex() == 4942 )
				{
					char wear_pos = ch->m_wearInventory.wearItemInfo[k]->getWearPos();

					ch->m_assist.CureByItemIndex(ch->m_wearInventory.wearItemInfo[k]->m_itemProto->getItemIndex());

					ch->m_wearInventory.DelNormalItem(k);

					bWare = false;
				}
			}
		}

		if(bWare == false)
		{
			// 자동 물약 사용 아이템이 벗겨진 경우
			CNetMsg::SP rmsg(new CNetMsg);
			SysMsg( rmsg, MSG_SYS_NO_AUTO_ITEM );
			SEND_Q(rmsg, ch->m_desc);
		}
	}

	// 스트레이아나 독
	ch->m_SkillTime_511 += TIME_ONE_SECOND;
	if(ch->m_SkillTime_511 >= 2 * TIME_ONE_MIN)
	{
		ch->m_SkillTime_511 = 0;
		if (ch->m_pZone->m_index == ZONE_STREIANA)
		{
			if(ch->m_job2)
			{
				CItemProto* pItem = gserver->m_itemProtoList.FindIndex(2859);
				if(pItem && ch->m_assist.FindBySkillIndex(pItem->getItemNum0()))
					return;

				CSkill * pSkill = gserver->m_skillProtoList.Create(511, 1) ;
				if (pSkill && ch->CanApplySkill(pSkill->m_proto, pSkill->m_proto->Level(pSkill->m_level)))
				{
					bool bApply = false;
					ApplySkill(ch, ch, pSkill, -1 , bApply);
				}
			}
		}
	}

	// 디버프를 준다.
	if(ch && ch->m_pZone->IsWarGroundZone())
	{
		int skilllevel = 1;
		CWaitPlayer* p = NULL;
		p = gserver->m_RoyalRumble.m_WaitPlayerList.GetNode(ch->m_index);
		if(p && p->GetIsEntered()) // 캐릭터가 로얄럼블에 참가 중인 상태에서 피스존에 들어갔다.
		{
			skilllevel += p->GetLevelType();
			if(ch->IsInPeaceZone(true))
			{
				if(!ch->m_assist.FindBySkillIndex(ROYAL_RUMBLE_DEBUFF_SKILL))
				{
					CSkill* pSkill = NULL;
					pSkill = gserver->m_skillProtoList.Create(ROYAL_RUMBLE_DEBUFF_SKILL, skilllevel);
					bool bApply;
					ApplySkill(ch, ch, pSkill, -1, bApply);
					GAMELOG << init("ROYAL RUMBLE",ch) << "DEBUFF SKILL CHARACTER IN THE PEACE AREA" << end;
					if(!bApply && pSkill)
					{
						delete pSkill;
						pSkill = NULL;
					}
				}
			}
		}
	}
#ifdef XTRAP

	ch->m_xtrapCheckPulse -= TIME_ONE_SECOND;

	if( ch->m_xtrapCheckPulse < 0/*10*PULSE_REAL_SEC*/ )
	{
		ch->m_xtrapCheckPulse = 20 * TIME_ONE_SECOND;

		char msgbuffer[128];
		int nRet = XTrap_CS_Step1( ch->m_xtrapSessionBuf, msgbuffer );
#ifdef XTRAP_DUMP
		char filename[128];
		sprintf( filename, "%d.xtrap.log" , ch->m_index );
		HexaDump4XTrap_Std( filename, msgbuffer, 128 , "Step1_After" );
#endif //XTRAP_DUMP

		{
			// 클라이언트에 XTRAP 확인 요청
			CNetMsg::SP rmsg(new CNetMsg);
			rmsg->Init( MSG_EXTEND );
			RefMsg(rmsg) << MSG_EX_XTRAP ;
			rmsg->Write( msgbuffer, 128 );
			SEND_Q( rmsg, ch->m_desc );
		}

		if( nRet != 0 )
		{
			GAMELOG << init( "SYSTEM ERROR : XTRAP" , ch ) << " error code " << nRet << end;
			ch->m_desc->Close("SYSTEM ERROR : XTRAP");
		}
	}
#endif // XTRAP

	//gps
	ch->m_gpsManager.sendGpsTargetMoveInfo();
	ch->m_arti_gpsManager.sendGpsTargetMoveInfo();

	//써치라이프
	if(ch->IsSearchLife() == true)
	{
		if(ch->m_targetPC != NULL && ch->m_targetPC->m_type != MSG_CHAR_UNKNOWN)
		{
			CPC* targetPC = ch->m_targetPC;
			CNetMsg::SP rmsg(new CNetMsg);
			UpdateClient::CharHpInfoMsg(rmsg, targetPC->m_index, targetPC->m_maxHP, targetPC->m_hp);
			SEND_Q(rmsg, ch->m_desc);
		}
	}

#ifdef PREMIUM_CHAR
	if (ch->m_premiumChar.isActive())
	{
		ch->m_premiumChar.checkExpireTime(gserver->m_nowseconds);
	}
#endif
}

CZone* CServer::FindZone(int zone)
{
	map_zone_t::iterator it = m_zones_map.find(zone);
	return (it != m_zones_map.end()) ? it->second : NULL;
}

bool CServer::DropProbLoad()
{
	CDBCmd cmd;
	cmd.Init(&m_dbdata);

	cmd.SetQuery("SELECT * FROM t_drop_prob");

	if (!cmd.Open() || !cmd.MoveFirst())
		return false;

	if (!cmd.GetRec("a_item_prob",	m_itemDropProb) || !cmd.GetRec("a_money_prob",	m_moneyDropProb))
		return false;

	// 최대 300 최소 50 제한
	if (m_itemDropProb > 300)
		m_itemDropProb = 300;
	else if (m_itemDropProb < 50)
		m_itemDropProb = 50;

	return true;
}

//0502 kwon
void CServer::MoonStoneEndProcess(CPC* ch)
{
}

// 세액 -> 성정보
void CServer::SaveTax()
{
	CDBCmd cmd;
	cmd.Init(&m_dbcastle);
	std::string udpate_castle_query = "";

	udpate_castle_query += boost::str(boost::format("UPDATE t_castle SET a_tax_item = a_tax_item + %1%, a_tax_produce = a_tax_produce + %2% WHERE a_zone_index=7") % (m_taxItem / 3) % (m_taxProduceCastle / 3) );

#ifdef LC_TLD // 태국 세금 1/3로 한다
	m_taxItem /= 3;
	m_taxProduceCastle /= 3;
#endif

	cmd.SetQuery(udpate_castle_query);
	if (!cmd.Update())
	{
		GAMELOG << init("MERAC TAX SAVE FAIL")
				<< "ITEM" << delim
				<< (m_taxItem / 3) << delim
				<< "PRODUCE" << delim
				<< (m_taxProduceCastle / 3)
				<< end;
	}
	else
	{
		GAMELOG << init("MERAC TAX SAVE SUCCESS")
				<< "ITEM" << delim
				<< (m_taxItem / 3) << delim
				<< "PRODUCE" << delim
				<< (m_taxProduceCastle / 3)
				<< end;
		ResetTax();
	}

	CDBCmd cmd2;
	cmd2.Init(&m_dbcastle);
	std::string udpate_castle_query1 = "";

	udpate_castle_query1 += boost::str(boost::format(
										   "UPDATE t_castle SET a_tax_item = a_tax_item + %1%, a_tax_produce = a_tax_produce + %2% WHERE a_zone_index=4") % (m_taxItemDratan / 3) % (m_taxProduceCastleDratan / 3));

	cmd2.SetQuery(udpate_castle_query1);
	if (!cmd2.Update())
	{
		GAMELOG << init("DRATAN TAX SAVE FAIL")
				<< "ITEM" << delim
				<< (m_taxItemDratan / 3) << delim
				<< "PRODUCE" << delim
				<< (m_taxProduceCastleDratan / 3)
				<< end;
	}
	else
	{
		GAMELOG << init("DRATAN TAX SAVE SUCCESS")
				<< "ITEM" << delim
				<< (m_taxItemDratan / 3) << delim
				<< "PRODUCE" << delim
				<< (m_taxProduceCastleDratan / 3)
				<< end;
		ResetTaxDratan();
	}
}

// 세금 -> 길드
void CServer::DivideTax()
{
	// 성세금
	CDBCmd cmd;
	cmd.Init(&m_dbcastle);

	int zoneindex = CWarCastle::GetCurSubServerCastleZoneIndex();
	if (zoneindex < 0)
		return ;

	std::string select_castle_query = boost::str(boost::format(
										  "SELECT a_tax_guild_index, a_tax_item, a_tax_produce, a_tax_wday FROM t_castle WHERE a_zone_index = %d") % ZONE_MERAC);
	cmd.SetQuery(select_castle_query);

	if (cmd.Open() && cmd.MoveFirst())
	{
		int guildindex;
		LONGLONG taxItem;
		LONGLONG taxProduce;
		int wday;
		cmd.GetRec("a_tax_guild_index", guildindex);
		cmd.GetRec("a_tax_item", taxItem);
		cmd.GetRec("a_tax_produce", taxProduce);
		cmd.GetRec("a_tax_wday", wday);

		// 최근 저장 요일이 오늘이 아닐 경우에만
		struct tm tmCur = NOW();
		if (wday != tmCur.tm_wday)
		{
			// 헬퍼에서 최소 금액을 보장한다
			CNetMsg::SP rmsg(new CNetMsg);
			HelperGuildStashSaveTaxReqMsg(rmsg, guildindex, ZONE_MERAC, taxItem, taxProduce);
			SEND_Q(rmsg, m_helper);
		}
	}
	else
	{
		GAMELOG << init("MERAC TAX DIVIDE FAIL")
				<< end;
	}

	CDBCmd cmd2;
	cmd2.Init(&m_dbcastle);
	std::string select_castle_query1 = boost::str(boost::format(
										   "SELECT a_tax_guild_index, a_tax_item, a_tax_produce, a_tax_wday FROM t_castle WHERE a_zone_index = %d") % ZONE_DRATAN);
	cmd2.SetQuery(select_castle_query1);

	if (cmd2.Open() && cmd2.MoveFirst())
	{
		int guildindex;
		LONGLONG taxItem;
		LONGLONG taxProduce;
		int wday;
		cmd2.GetRec("a_tax_guild_index", guildindex);
		cmd2.GetRec("a_tax_item", taxItem);
		cmd2.GetRec("a_tax_produce", taxProduce);
		cmd2.GetRec("a_tax_wday", wday);

		// 최근 저장 요일이 오늘이 아닐 경우에만
		struct tm tmCur = NOW();
		if (wday != tmCur.tm_wday)
		{
			// 헬퍼에서 최소 금액을 보장한다
			CNetMsg::SP rmsg(new CNetMsg);
			HelperGuildStashSaveTaxReqMsg(rmsg, guildindex, ZONE_DRATAN, taxItem, taxProduce);
			SEND_Q(rmsg, m_helper);
		}
	}
	else
	{
		GAMELOG << init("DRATAN TAX DIVIDE FAIL")
				<< end;
	}
}

void CServer::ChangeTaxGuild()
{
	CDBCmd cmd;
	cmd.Init(&m_dbcastle);

	std::string update_castle_query = boost::str(boost::format(
										  "UPDATE t_castle SET a_tax_item = 0, a_tax_produce = 0, a_tax_guild_index = a_owner_guild_index WHERE a_zone_index = %d AND a_tax_guild_index != a_owner_guild_index")
									  % CWarCastle::GetCurSubServerCastleZoneIndex());

	cmd.SetQuery(update_castle_query);
	if (!cmd.Update())
	{
		GAMELOG << init("MARAC TAX GUILD RESET FAIL")
				<< "ZONE" << delim
				<< CWarCastle::GetCurSubServerCastleZoneIndex()
				<< end;
	}
	else
	{
		GAMELOG << init(" MARAC TAX GUILD RESET SUCCESS")
				<< "ZONE" << delim
				<< CWarCastle::GetCurSubServerCastleZoneIndex()
				<< end;
	}

	CDBCmd cmd2;
	cmd2.Init(&m_dbcastle);
	std::string update_castle_query1 = boost::str(boost::format(
										   "UPDATE t_castle SET a_tax_item = 0, a_tax_produce = 0, a_tax_guild_index = a_owner_guild_index WHERE a_zone_index = %d AND a_tax_guild_index != a_owner_guild_index")
									   % ZONE_DRATAN);

	cmd2.SetQuery(update_castle_query1);
	if (!cmd2.Update())
	{
		GAMELOG << init("DRATAN TAX GUILD RESET FAIL")
				<< "ZONE" << delim
				<< CWarCastle::GetCurSubServerCastleZoneIndex()
				<< end;
	}
	else
	{
		GAMELOG << init("DRATAN TAX GUILD RESET SUCCESS")
				<< "ZONE" << delim
				<< CWarCastle::GetCurSubServerCastleZoneIndex()
				<< end;
	}
}

CZone* CServer::FindNearestZone(int zone, float x, float z, int* nearZone, int* nearZonePos, int syndicateType)
{
	// x, z가 음수이면 x, z 무시해야 함
	// 기본 위치
	*nearZone = ZONE_START;
	*nearZonePos = 0;

	switch (zone)
	{
	case ZONE_PK_TOURNAMENT:
		*nearZone = ZONE_PK_TOURNAMENT;
		*nearZonePos = 0;
		break;

	case ZONE_STREIANA:
	case ZONE_SPRIT_CAVE:
	case ZONE_QUANIAN_CAVE:
	case ZONE_GOLEM_CAVE:
	case ZONE_TRIVIA_CANYON:
		*nearZone = ZONE_STREIANA;
		*nearZonePos = 0;
		break;

	case ZONE_SINGLE_DUNGEON_TUTORIAL:	// 쥬노 2번에서 시작
		*nearZone = ZONE_START;
		*nearZonePos = 0;
		break;

	case ZONE_DUNGEON4:
	case ZONE_DUNGEON2:
	case ZONE_DRATAN:
	case ZONE_DRATAN_CASTLE_DUNGEON:
		*nearZone = ZONE_DRATAN;
		*nearZonePos = 0;
		break;

	case ZONE_MERAC:
	case ZONE_DUNGEON3:
	case ZONE_MISTY_CANYON:
	case ZONE_EXTREME_CUBE:
		*nearZone = ZONE_MERAC;
		*nearZonePos = 0;
		break;

	case ZONE_EGEHA:
	case ZONE_EGEHA_DUNGEON_1:
	case ZONE_EGEHA_DUNGEON_8:
	case ZONE_EGEHA_DUNGEON_9:
	case ZONE_EGEHA_DUNGEON_10:
	case ZONE_FLORAIM_CAVE :
		*nearZone = ZONE_EGEHA;
		*nearZonePos = 0;
		break;

	case ZONE_COMBO_DUNGEON:
		*nearZone = ZONE_COMBO_DUNGEON;
		*nearZonePos = 0;
		break;

	case ZONE_MONDSHINE :
		*nearZone = ZONE_MONDSHINE;
		*nearZonePos = 0;
		break;

	case ZONE_TARIAN:
		*nearZone = ZONE_TARIAN;
		*nearZonePos = 0;
		break;

	case ZONE_BLOODYMIR:
		*nearZone = ZONE_BLOODYMIR;
		*nearZonePos = 0;
		break;

	case ZONE_RVR:
		*nearZone = ZONE_RVR;
		*nearZonePos = syndicateType;
		break;

	case ZONE_START:	// 쥬노 시작
	case ZONE_DUNGEON1:
	case ZONE_SINGLE_DUNGEON1:
	case ZONE_SINGLE_DUNGEON2:
	case ZONE_GUILDROOM:
	case ZONE_SINGLE_DUNGEON3:
	case ZONE_SINGLE_DUNGEON4:
	case ZONE_OXQUIZROOM:
	case ZONE_EBONY_MINE:
	default:
		*nearZone = ZONE_START;
		*nearZonePos = 0;
		break;
	}

	return FindZone(*nearZone);
}

CParty* CServer::FindPartyByBossIndex(int nBossIndex)
{
	map_listparty_t::iterator it = m_listParty.find(nBossIndex);
	return (it != m_listParty.end()) ? it->second : NULL;
}

CParty* CServer::FindPartyByMemberIndex(int nMemberIndex, bool bIncludeRequest)
{
	map_listparty_t::iterator it = m_listParty.begin();
	map_listparty_t::iterator endit = m_listParty.end();
	for(; it != endit; ++it)
	{
		CParty* pParty = it->second;
		if (pParty->FindMember(nMemberIndex) != -1)
			return pParty;
		if (bIncludeRequest && pParty->GetRequestIndex() == nMemberIndex)
			return pParty;
	}

	return NULL;
}

CExpedition* CServer::FindExpedByBossIndex( int nBossIndex)
{
	map_listexped_t::iterator it = m_listExped.find(nBossIndex);
	return (it != m_listExped.end()) ? it->second : NULL;
}

CExpedition* CServer::FindExpedByMemberIndex(int nMemberIndex, bool bIncludeRequest)
{
	map_listexped_t::iterator it = m_listExped.begin();
	map_listexped_t::iterator endit = m_listExped.end();
	for(; it != endit; ++it)
	{
		CExpedition* pExped = it->second;

		if (pExped->FindMemberListIndex(nMemberIndex) != -1)
			return pExped;
		if (bIncludeRequest && pExped->GetRequestIndex() == nMemberIndex)
			return pExped;
	}

	return NULL;
}

CPartyMatchMember* CServer::FindPartyMatchMemberByCharIndex(int nCharIndex)
{
	map_listPartyMatchMember_t::iterator it = m_listPartyMatchMember.find(nCharIndex);
	return (it != m_listPartyMatchMember.end()) ? it->second : NULL;
}

CPartyMatchParty* CServer::FindPartyMatchPartyByBossIndex(int nBossIndex)
{
	map_listPartyMatchParty_t::iterator it = m_listPartyMatchParty.find(nBossIndex);
	return (it != m_listPartyMatchParty.end()) ? it->second : NULL;
}

bool CServer::LoadOXQuiz()
{
	CLCString sql(1024);
#ifdef LC_KOR
	sql.Format("SELECT a_index, a_question, a_answer FROM t_oxquiz WHERE a_country=%d ORDER BY a_index", OX_QUIZ_COUNTRY_CODE);
#else
	sql.Format("SELECT a_index, a_question, a_answer FROM t_oxquiz WHERE a_country=%d ORDER BY a_index", m_national);
#endif
	CDBCmd cmd;
	cmd.Init(&m_dbdata);
	cmd.SetQuery(sql);
	if (!cmd.Open())
		return false;

	while (cmd.MoveNext())
	{
		int nIndex;
		CLCString strQuestion(256);
		CLCString strAnswer(2);

		if (
			!cmd.GetRec("a_index", nIndex)
			|| !cmd.GetRec("a_question", strQuestion)
			|| !cmd.GetRec("a_answer", strAnswer)
		)
			return false;

		COXQuizData* pQuiz = new COXQuizData(nIndex, strQuestion, (strcmp(strAnswer, "O") == 0) ? true : false);
		m_listOXQuiz.insert(map_listOXQuiz_t::value_type(pQuiz->GetQuizIndex(), pQuiz));
	}

	return true;
}

int CServer::addChatGroup(int& makeCharIndex, int& chatIndex, CLCString charName)
{
	CChatGroup* chatGroup = this->FindChatGroup(makeCharIndex, chatIndex);
	if( chatGroup )
	{
		// 세션이 만들어진 상태
		std::set<CLCString>::iterator it = chatGroup->m_charNameList.find(charName);
		if( it == chatGroup->m_charNameList.end() )
		{
			if( chatGroup->CheckSameName(charName) == false )
			{
				// 중복 체크
				return 0;
			}
			chatGroup->m_charNameList.insert(charName);
			if( chatGroup->m_charNameList.size() == 1 )
			{
				// 처음 생성시
				return -1;
			}
			return 1;
		}
	}
	else
	{
		chatIndex = this->GetMaxChatIndexPerPC(makeCharIndex)+1;
		chatGroup = new CChatGroup(makeCharIndex, chatIndex );
		chatGroup->m_charNameList.insert(charName);
		LONGLONG key = MAKE_LONGLONG_KEY(makeCharIndex, chatIndex);
		this->m_chatList.insert(map_chatList_t::value_type(key, chatGroup));

		return 0;
	}

	return -1;
}

void CServer::discCharChatGroup(CLCString charName)
{
	map_chatList_t::iterator it = m_chatList.begin();
	map_chatList_t::iterator endit = m_chatList.end();
	for(; it != endit; ++it)
	{
		CChatGroup* chatGroup = it->second;

		CNetMsg::SP rmsg(new CNetMsg);
		MsgrMessengerChatMsg(rmsg, MSG_MSGR_MESSENGER_DEL, chatGroup->GetMakeCharIndex(), chatGroup->GetChatIndex(), 0, charName );
		SEND_Q(rmsg, gserver->m_messenger);
	}
}

void CServer::delChatGroup(int makeCharIndex, int chatIndex, CLCString charName)
{
	CChatGroup* chatGroup = this->FindChatGroup(makeCharIndex, chatIndex);
	if( chatGroup == NULL )
		return;

	std::set<CLCString>::iterator sit = chatGroup->m_charNameList.begin();
	std::set<CLCString>::iterator eit = chatGroup->m_charNameList.end();

	for( ; sit != eit; ++sit )
	{
		if (strcmp((*sit), charName) == 0)
		{
			chatGroup->m_charNameList.erase(sit);
			if (chatGroup->m_charNameList.empty())
			{
				//방원이 존재
				LONGLONG key = MAKE_LONGLONG_KEY(makeCharIndex, chatIndex);
				m_chatList.erase(key);
				delete chatGroup;
			}
			break;
		}
	}
}

void CServer::SendChatGroup(MSG_EX_MESSENGER_TYPE subtype, int makeCharIndex, int chatIndex, int chatColor, CLCString charName, CLCString chat)
{
	CChatGroup* chatGroup = this->FindChatGroup(makeCharIndex, chatIndex);
	if( chatGroup )
	{
		chatGroup->SendGroup(subtype, chatColor, charName, chat);
	}
}

CChatGroup* CServer::FindChatGroup(int makeCharIndex, int chatIndex)
{
	LONGLONG key = MAKE_LONGLONG_KEY(makeCharIndex, chatIndex);
	map_chatList_t::iterator it = m_chatList.find(key);
	return (it != m_chatList.end()) ? it->second : NULL;
}

int CServer::GetMaxChatIndexPerPC(int makeCharIndex)
{
	int max = 0;
	map_chatList_t::iterator it = m_chatList.begin();
	map_chatList_t::iterator endit = m_chatList.end();
	for(; it != endit; ++it)
	{
		CChatGroup* chatGroup = it->second;

		if( chatGroup->GetMakeCharIndex() == makeCharIndex && chatGroup->GetChatIndex() > max )
			max = chatGroup->GetChatIndex();
	}

	return max;
}

bool CChatGroup::CheckSameName(CLCString charName)
{
	std::set<CLCString>::iterator it = m_charNameList.find(charName);
	return (it == m_charNameList.end()) ? true : false;
}

void CChatGroup::SendGroup(MSG_EX_MESSENGER_TYPE subtype, int chatColor, CLCString charName, CLCString chat)
{
	CNetMsg::SP rmsg(new CNetMsg);
	rmsg->Init(MSG_EXTEND);
	RefMsg(rmsg) << MSG_EX_MESSENGER
				 << (unsigned char) subtype
				 << this->GetMakeCharIndex()
				 << this->GetChatIndex();

	if( subtype == MSG_EX_MESSENGER_CHAT)
		RefMsg(rmsg) << chatColor;
	RefMsg(rmsg) << charName
				 << chat;

	CNetMsg::SP lmsg(new CNetMsg);
	if( subtype == MSG_EX_MESSENGER_INVITE )
	{
		lmsg->Init(MSG_EXTEND);
		RefMsg(lmsg) << MSG_EX_MESSENGER
					 << (unsigned char) MSG_EX_MESSENGER_CHARLIST
					 << this->GetMakeCharIndex()
					 << this->GetChatIndex()
					 << (int)this->m_charNameList.size();
	}

	CLCString SendcharName(MAX_CHAR_NAME_LENGTH + 1);
	std::set<CLCString>::iterator it = m_charNameList.begin();
	std::set<CLCString>::iterator endit = m_charNameList.end();
	for(; it != endit; ++it)
	{
		SendcharName = *(it);

		if( strcmp( SendcharName, "" ) != 0 )
		{
			CPC* pc = PCManager::instance()->getPlayerByName(SendcharName, true);
			if( pc )
			{
				if( subtype == MSG_EX_MESSENGER_INVITE )
					RefMsg(lmsg) << SendcharName;

				SEND_Q(rmsg, pc->m_desc);
			}
		}
	}

	if( subtype == MSG_EX_MESSENGER_INVITE)
	{
		CPC* pc = PCManager::instance()->getPlayerByName(charName, true);
		if( pc )
			SEND_Q(lmsg, pc->m_desc);
	}
}

CChatGroup::CChatGroup(int makeCharIndex, int chatIndex)
{
	m_makeCharIndex = makeCharIndex;
	m_chatIndex = chatIndex;
}

CChatGroup::~CChatGroup()
{
}

void CServer::CharPrePlay(CDescriptor* d)
{
	if (d->m_pChar->m_pZone == NULL || d->m_pChar->m_pArea == NULL)
	{
		LOG_ERROR("Not Found Zone OR Not Found Area");
		d->Close("Not found zone OR Not Found Area");
		return;
	}

	CArea* area = d->m_pChar->m_pArea;

	d->m_pChar->m_bPlaying = true;
	bool bCashZoneMove = false;
	if(d->m_pChar->IsSetPlayerState(PLAYER_STATE_CASH_ZONE_MOVE))
		bCashZoneMove = true;
	d->m_pChar->ResetPlayerState(~PLAYER_STATE_RESET);
	//초기화 한 후 정당방위 리스트를 검색 한 후 데이터가 있을 경우에는 정당방위 상태로 만들어준다.
	if(d->m_pChar->m_raList != NULL && d->m_pChar->m_raList->m_raPulse > 0)
	{
		d->m_pChar->SetPlayerState(PLAYER_STATE_RAMODE);
	}

	d->m_pChar->m_regGuild = 0;

	d->m_pChar->m_inventory.CheckCompositeValidation();


	// 죽은 상태면 부활
	if (DEAD(d->m_pChar))
	{
		d->m_pChar->m_hp= d->m_pChar->m_dbHP / 2;

		// MP는 반이상 있으면 그대로..
		if (d->m_pChar->m_mp < d->m_pChar->m_dbMP / 2)
			d->m_pChar->m_mp = d->m_pChar->m_dbMP / 2;
	}

	if(d->m_pChar->m_exp > GetLevelupExp(d->m_pChar->m_level))
	{
		GAMELOG << init("REVISE EXP", d->m_pChar)
				<< d->m_pChar->m_exp
				<< " ==>> "
				<< GetLevelupExp(d->m_pChar->m_level)
				<< end;
		d->m_pChar->m_exp = GetLevelupExp(d->m_pChar->m_level);
	}



	int i;

	// 하드코딩 : 튜토리얼 퀘스트 수행중인지 검사 후 여유 영역이 있는가 검사
	if( d->m_pChar->m_job == JOB_NIGHTSHADOW )
	{	}
	else
	{
		CQuest* pQuest;
		CQuest* pQuestNext = d->m_pChar->m_questList.GetNextQuest(NULL, QUEST_STATE_RUN);
		while ((pQuest = pQuestNext))
		{
			pQuestNext = d->m_pChar->m_questList.GetNextQuest(pQuestNext, QUEST_STATE_RUN);
			if (pQuest->GetQuestType0() == QTYPE_KIND_TUTORIAL)
			{
				CZone* pZone = gserver->FindZone(ZONE_SINGLE_DUNGEON_TUTORIAL);
				if (pZone == NULL)
				{
					LOG_ERROR("Not found zone. zone is %d", ZONE_SINGLE_DUNGEON_TUTORIAL);
					d->Close("Not found zone");
					return;
				}

				d->m_pChar->m_pZone = pZone;
				int idx = d->m_pChar->m_pZone->SetEnableArea();

				// 빈 영억 없음 : 접속을 끊는다
				if (idx == -1)
				{
					LOG_ERROR("Not found area");
					d->Close("Not found area");
					return;
				}

				d->m_pChar->m_pArea = d->m_pChar->m_pZone->m_area + idx;
				area = d->m_pChar->m_pArea;

				GET_YLAYER(d->m_pChar)	= d->m_pChar->m_pZone->m_zonePos[0][0];
				GET_R(d->m_pChar)		= 0.0f;
				GET_X(d->m_pChar) = d->m_pChar->m_pZone->m_zonePos[0][1] / 2;
				GET_Z(d->m_pChar) = d->m_pChar->m_pZone->m_zonePos[0][2] / 2;
				break;
			}
		}
	}

	// 길드 정보 세팅
	CGuildMember* member = m_guildlist.findmember(d->m_pChar->m_index);
	if (member && member->guild())
	{
		d->m_pChar->m_guildInfo = member;
	}
	else
	{
		d->m_pChar->m_guildInfo = NULL;
	}

	// 공성 정보 반영
	CWarCastle::CheckJoinAll(d->m_pChar);

	// 공성 참여 유저가 공성 지역으로 이동하면 공성 리젠포인트로 이동 묻기
	int posPromptMove = -1;
	int zonePromptMove = -1;
	int gstate =0;
	int castlezone = CWarCastle::GetCurSubServerCastleZoneIndex();

	// 현재 서버의 공성 정보
	CWarCastle* castle = CWarCastle::GetCastleObject(castlezone);
	if (castle != NULL
			&& d->m_pChar->GetJoinFlag(castlezone) != WCJF_NONE)
	{
		time_t ct;
		time(&ct);
		int nt = castle->GetNextWarTime();
		if (ct + 10 * 60 >= nt || castle->GetState() != WCSF_NORMAL)
		{
			zonePromptMove = castlezone;
			posPromptMove = castle->GetRegenPoint(d->m_pChar->GetJoinFlag(castlezone), d->m_pChar);
		}

#ifdef WARCASTLE_MOVE_MESSAGE_LIMIT
		if(    d->m_pChar->m_pZone->m_index == castle->GetZoneIndex()
				&& d->m_pChar->m_pZone->InExtra( (int)GET_X(d->m_pChar), (int)GET_Z(d->m_pChar) ,posPromptMove)  )
		{
			posPromptMove = -1;
		}
#endif
		{
			gstate = castle->GetGateState();
		}
	}

	// 공성 참여 유저가 공성 지역으로 이동하면 공성 리젠포인트로 이동 묻기
	int posPromptMove_Dratan = -1;
	int zonePromptMove_Dratan = -1;
	CDratanCastle * pCastle = CDratanCastle::CreateInstance();
	if (GetOutDratanDungeon(d->m_pChar))
	{
		area = d->m_pChar->m_pArea;
	}
	// 현재 서버의 공성 정보
	if (d->m_pChar->GetJoinFlag(pCastle->GetZoneIndex()) != WCJF_NONE
			&& gserver->m_subno == WAR_CASTLE_SUBNUMBER_DRATAN)
	{
		time_t ct;
		time(&ct);
		int nt = pCastle->GetNextWarTime();

		if (ct + 10 * 60 >= nt
				|| pCastle->GetState() != WCSF_NORMAL)
		{
			zonePromptMove_Dratan = pCastle->GetZoneIndex();
			posPromptMove_Dratan = pCastle->GetRegenPoint(d->m_pChar->GetJoinFlag(pCastle->GetZoneIndex()), d->m_pChar);
		}

#ifdef WARCASTLE_MOVE_MESSAGE_LIMIT
		if(    d->m_pChar->m_pZone->m_index == pCastle->GetZoneIndex()
				&& d->m_pChar->m_pZone->InExtra( (int)GET_X(d->m_pChar), (int)GET_Z(d->m_pChar) ,posPromptMove_Dratan )  )
		{
			zonePromptMove_Dratan = -1;
		}
#endif

		if(gstate > 0)
			gstate |= pCastle->GetGateState();
		else
			gstate = pCastle->GetGateState();
	}

	if (gstate > 0)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildWarGateStateMsg(rmsg, gstate, gstate);
		SEND_Q(rmsg, d);
	}

#ifdef JUNO_RENEWAL_MESSAGEBOX
	bool bMessageBoxPopUp = false;
#endif // JUNO_RENEWAL_MESSAGEBOX
	if(NULL != d->m_pChar->m_pZone
			&& ZONE_START == d->m_pChar->m_pZone->m_index)
	{
		int extra;
#ifdef JUNO_RENEWAL_MESSAGEBOX
		if(d->m_pChar->m_etcEvent & ETC_EVENT_JUNO_RENEWAL_MESSAGEBOX_POPUP)
		{
			extra = 0;		//  란돌로
			d->m_pChar->m_pos.m_x = GetRandom(d->m_pChar->m_pZone->m_zonePos[extra][1], d->m_pChar->m_pZone->m_zonePos[extra][3]) / 2.0f;
			d->m_pChar->m_pos.m_z = GetRandom(d->m_pChar->m_pZone->m_zonePos[extra][2], d->m_pChar->m_pZone->m_zonePos[extra][4]) / 2.0f;
			bMessageBoxPopUp = true;

			if(d->m_pChar->m_job != JOB_NIGHTSHADOW)
			{
				CNetMsg::SP rmsg(new CNetMsg);
				MsgMessageBox(rmsg, MSG_EX_MSGBOX_CHANGE_START_POINT);
				SEND_Q(rmsg, d);
			}
		}
		else
#endif // JUNO_RENEWAL_MESSAGEBOX
			if( d->m_pChar->m_etcEvent & ETC_EVENT_JUNO_RENEWAL_QUESTCOMPLETE && !bCashZoneMove )
			{
				extra = 6;		// 농장으로
				if(d->m_pChar->m_questList.FindQuest(386, QUEST_STATE_DONE))
				{
					d->m_pChar->m_etcEvent &= ~ETC_EVENT_JUNO_RENEWAL_QUESTCOMPLETE;
					extra = 0;
				}
				d->m_pChar->m_pos.m_x = GetRandom(d->m_pChar->m_pZone->m_zonePos[extra][1], d->m_pChar->m_pZone->m_zonePos[extra][3]) / 2.0f;
				d->m_pChar->m_pos.m_z = GetRandom(d->m_pChar->m_pZone->m_zonePos[extra][2], d->m_pChar->m_pZone->m_zonePos[extra][4]) / 2.0f;
			}
			else
				extra = 0;		// 란돌로
		float x;
		float z;
		x = d->m_pChar->m_pos.m_x*2.0f;
		z = d->m_pChar->m_pos.m_z*2.0f;
		if( (d->m_pChar->m_pZone->m_zonePos[0][1] >= x && d->m_pChar->m_pZone->m_zonePos[0][3] <= x)
				&& d->m_pChar->m_pZone->m_zonePos[0][2] >= z && d->m_pChar->m_pZone->m_zonePos[0][4] <= z)
		{
			d->m_pChar->m_pos.m_x = GetRandom(d->m_pChar->m_pZone->m_zonePos[extra][1], d->m_pChar->m_pZone->m_zonePos[extra][3]) / 2.0f;
			d->m_pChar->m_pos.m_z = GetRandom(d->m_pChar->m_pZone->m_zonePos[extra][2], d->m_pChar->m_pZone->m_zonePos[extra][4]) / 2.0f;
		}
	}

	int cx, cz;
	area->PointToCellNum(GET_X(d->m_pChar), GET_Z(d->m_pChar), &cx, &cz);
	area->CharToCell(d->m_pChar, GET_YLAYER(d->m_pChar), cx, cz);

	{
		// 캐릭터에게 처음 시작하는 위치를 알림.
		CNetMsg::SP rmsg(new CNetMsg);
		AtMsg(rmsg, d->m_pChar);
		SEND_Q(rmsg, d);
	}
	// 길드 정보
	if (d->m_pChar->m_guildInfo)
	{
		if (d->m_pChar->m_first_inmap)
		{
			d->m_pChar->m_guildInfo->online(1);
			d->m_pChar->m_guildInfo->SetPC(d->m_pChar);

			CNetMsg::SP rmsg(new CNetMsg);
			HelperGuildOnline(rmsg, d->m_pChar->m_guildInfo);
			RefMsg(rmsg) << d->m_pChar->m_pArea->m_zone->m_index;
			SEND_Q(rmsg, m_helper);
		}

		{
			CNetMsg::SP rmsg(new CNetMsg);
			GuildInfoMsg(rmsg, d->m_pChar);
			SEND_Q(rmsg, d);
		}

		{
			CNetMsg::SP rmsg(new CNetMsg);
			GuildListMsg(rmsg, d->m_pChar);
			SEND_Q(rmsg, d);
		}
	}

	{
		// STAT POINT
		CNetMsg::SP rmsg(new CNetMsg);
		StatPointRemainMsg(rmsg, d->m_pChar);
		SEND_Q(rmsg, d);
	}

	// 하드코딩 : 0짜리 제련석 40레벨로 바꿈
	CItem* item = d->m_pChar->m_inventory.FindByDBIndex(84, 0 ,0);
	if (item)
	{
		item->setFlag(40);
	}

	// 성주 무기 빼앗거나 지급
	bool bProcLordItem = false;
	if (d->m_pChar->m_guildInfo && d->m_pChar->m_guildInfo->guild())
	{
		int ownZoneCount;
		int* ownZoneIndex = CWarCastle::GetOwnCastle(d->m_pChar->m_guildInfo->guild()->index(), &ownZoneCount);
		if (ownZoneIndex)
		{
			time_t curtime;
			time(&curtime);

			int i;
			for (i = 0; i < ownZoneCount; i++)
			{
				CWarCastle* castle = CWarCastle::GetCastleObject(ownZoneIndex[i]);
				if (castle)
				{
					if (castle->GetOwnerCharIndex() == d->m_pChar->m_index)
					{
						bProcLordItem = castle->GiveLordItem(d->m_pChar);
					}
				}
			}
			delete [] ownZoneIndex;
		}
	}
	// 성주가 아니라서 아무 처리 안했으면 성주 전용 아이템 찾아서 지우기
	if (!bProcLordItem)
	{
		BOOST_FOREACH(CItemProto* flagload_item, gserver->m_itemProtoList.m_flagload)
		{
			item_search_t vec;
			int sc = d->m_pChar->m_inventory.searchItemByCondition(flagload_item->getItemIndex(), 0, 0, vec);
			if (sc == 0)
				continue;

			item_search_t::iterator it = vec.begin();
			item_search_t::iterator endit = vec.end();
			for (; it != endit; ++it)
			{
				CItem* item = (*it).pItem;

				if (item->getWearPos() != WEARING_NONE)
				{
					if (item->getWearPos() >= WEARING_SHOW_START && item->getWearPos() <= WEARING_SHOW_END && d->m_pChar->m_pArea)
					{
						CNetMsg::SP rmsg(new CNetMsg);
						WearingMsg(rmsg, d->m_pChar, item->getWearPos(), -1, 0);
						d->m_pChar->m_pArea->SendToCell(rmsg, d->m_pChar, true);
					}
					d->m_pChar->m_wearInventory.RemoveItem(item->getWearPos());
				}
			}
		}
	}

// yhj 수정 090528... 현재까진 버그 없음
	CPet* pet = d->m_pChar->m_petList;
	while (pet)
	{
		pet->ResetSummonMountFlag();
		{
			CNetMsg::SP rmsg(new CNetMsg);
			ExPetStatusMsg(rmsg, pet);
			SEND_Q(rmsg, d);
		}
		{
			CNetMsg::SP rmsg(new CNetMsg);
			ExPetSkillListMsg(rmsg, pet);
			SEND_Q(rmsg, d);
		}

		// 소환중인 펫이 마운트 타입이 아니면 Appear 시키기
		if (pet->IsWearing())
		{
			if (d->m_pChar->m_pZone->m_bCanMountPet)
			{
				if (pet->IsMountType())
					pet->Mount(true);
				else if (!pet->IsMountType() && d->m_pChar->m_pZone->m_bCanSummonPet)
					pet->Appear(false);
			}
			else
			{
				//퀵 슬롯에서 pet 스킬을 모두 지운다.
				d->m_pChar->RemovePetSkillFromQuickSlot();
			}
		}
		pet = pet->m_nextPet;
	}

	CAPet* apet = d->m_pChar->m_pApetlist;
	while ( apet )
	{
		// Pet 상태 MSG
		if( apet->IsWearing() && !DEAD(apet) )
		{
			if ( d->m_pChar->m_pZone->m_bCanSummonPet )
			{
				apet->Appear(false);				
				{
					CNetMsg::SP rmsg(new CNetMsg);
					ExAPetFuntionMsg(rmsg, MSG_SUB_SKILLLIST, apet, 0 );
					SEND_Q(rmsg, d);
				}

				if (apet->IsMount() && !d->m_pChar->m_pZone->m_bCanMountPet)
				{
					apet->Mount(false);
				}

				{
					CNetMsg::SP rmsg(new CNetMsg);
					APetAIOnOffMsg(rmsg, apet, MSG_APET_ERROR_OK);
					SEND_Q(rmsg, apet->GetOwner()->m_desc);
				}

				{
					CNetMsg::SP rmsg(new CNetMsg);
					APetAIListMsg(rmsg, apet);
					SEND_Q(rmsg, apet->GetOwner()->m_desc);
				}
			}
			else
			{
				apet->m_bMount = false;
				//퀵 슬롯에서 apet 스킬을 모두 지운다.
				d->m_pChar->RemoveApetSkillFromQuickSlot();
			}
		}
		else if(d->m_pChar->m_petStashManager.FindApet(apet))
		{
			CNetMsg::SP rmsg(new CNetMsg);
			ExAPetStatusMsg(rmsg, apet);
			SEND_Q(rmsg, d);
		}
		else
		{
			CNetMsg::SP rmsg(new CNetMsg);
			ExApetSellInfo( rmsg, apet );
			SEND_Q( rmsg, d );
		}
		apet = apet->m_pNextPet;
	}

	// 골든볼 이벤트 응모 카드 회수 : 이벤트 진행 중이 아니면
	if (m_clGoldenBall.GetStatus() == GOLDENBALL_STATUS_NOTHING)
	{
		CItem* pItemCard = d->m_pChar->m_inventory.FindByDBIndex(GOLDENBALL_CARD_INDEX);
		if (pItemCard)
		{
			GAMELOG << init("GOLDEN BALL EVENT RECALL CARD", d->m_pChar)
					<< itemlog(pItemCard)
					<< end;

			d->m_pChar->m_inventory.decreaseItemCount(pItemCard, 1);
		}
	}

	d->m_pChar->ResetPlayerState(PLAYER_STATE_SUPPORTER);
	for (i = WEARING_ACCESSORY1; i <= WEARING_ACCESSORY3; i++)
	{
		if (d->m_pChar->m_wearInventory.wearItemInfo[i])
		{
			// 서포터 아이템
			if (d->m_pChar->m_wearInventory.wearItemInfo[i]->m_itemProto->getItemIndex() == 1912)
				d->m_pChar->SetPlayerState(PLAYER_STATE_SUPPORTER);
		}
	}

	// inven
	d->m_pChar->m_inventory.sendInfoToClient();

	// status
	d->m_pChar->SendStatus();

	// wearInven
	d->m_pChar->m_wearInventory.sendWearInfoToClient();
	{
		CNetMsg::SP rmsg(new CNetMsg);
		CharStatusMsg(rmsg, d->m_pChar, 0);
		SEND_Q(rmsg, d);
	}

	{
		// quest
		CNetMsg::SP rmsg(new CNetMsg);
		QuestPCListMsg(rmsg, d->m_pChar);
		SEND_Q(rmsg, d);
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		QuestCompleteListMsg(rmsg, d->m_pChar);
		SEND_Q(rmsg, d);
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		QuestAbandonListMsg(rmsg, d->m_pChar);
		SEND_Q(rmsg, d);
	}

	// quest complete check
	d->m_pChar->m_questList.CheckComplete(d->m_pChar);

	{
		CNetMsg::SP rmsg(new CNetMsg);
		rmsg->Init(MSG_EXTEND);
		RefMsg(rmsg) << MSG_EX_SERVER_TIME
					 << (int)gserver->m_nowseconds;
		SEND_Q(rmsg, d);
	}

	{
		// skill
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::makeSkillListMsg(rmsg, d->m_pChar);
		SEND_Q(rmsg, d);
	}

	// sskill
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::makeSSkillList(rmsg, d->m_pChar);
		SEND_Q(rmsg, d);
	}
	// Quick Slot MSG
	d->m_pChar->SendQuickSlot();

	if( d->m_pChar->m_mempos.m_count > 0)
	{
		// mempos
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::makeMemposList(rmsg, d->m_pChar);
		SEND_Q(rmsg, d);
	}

	{
		// assist
		CNetMsg::SP rmsg(new CNetMsg);
		AssistListMsg(rmsg, d->m_pChar);
		SEND_Q(rmsg, d);
	}

	{
		// 사제시스템
		CNetMsg::SP rmsg(new CNetMsg);
		TeachInfoMsg(rmsg, d->m_pChar);
		SEND_Q(rmsg, d);
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		AffinityListInfoMsg(rmsg, d->m_pChar);
		SEND_Q(rmsg, d);
	}
	d->m_pChar->m_affinityList.SendRewardNotice(d->m_pChar);

	//0627
	if((d->m_userFlag & NOVICE) && d->m_pChar->m_teachType == MSG_TEACH_NO_TYPE)
	{
		//모든 사제에게 신규유저가 접속 했음을 알린다.
		CNetMsg::SP rmsg(new CNetMsg);
		NoviceNotifyMsg(rmsg, d->m_pChar->GetName() );

		PCManager::instance()->sendToAllTeacher(rmsg);
		d->m_userFlag = d->m_userFlag &~ NOVICE;
	}

	// 길드 정보
	if (d->m_pChar->m_guildInfo)
	{
		// 길드전 일때 status msg 전송
		CGuild* g = d->m_pChar->m_guildInfo->guild();
		if (g && g->battleState() != GUILD_BATTLE_STATE_PEACE)
		{
			CGuild* g2 = gserver->m_guildlist.findguild(g->battleIndex());
			if (g2)
			{
				if (g->battleState() == GUILD_BATTLE_STATE_PRIZE)
				{
					if (g->boss() && g->boss()->charindex() == d->m_pChar->m_index)
					{
						d->m_pChar->m_inventory.increaseMoney(g->battlePrize());

						{
							CNetMsg::SP rmsg(new CNetMsg);
							HelperGuildBattlePeaceReqMsg(rmsg, g);
							SEND_Q(rmsg, gserver->m_helper);
						}
					}
				}
				else
				{
					CNetMsg::SP rmsg(new CNetMsg);
					GuildBattleStatusMsg(rmsg, g->index(), g->name(), g->killCount(), g2->index(), g2->name(), g2->killCount(), g->battleTime(), g->battleZone());
					SEND_Q(rmsg, d);
				}
			}
		}
	}

#ifdef NOTICE_EVENT
//0704  이벤트 공지 날리기.
	for(i =0; i< MAX_NOTICE; i++)
	{
		if(d->m_notice[i] != 0)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			NoticeInfoMsg(rmsg, d->m_notice[i]);
			SEND_Q(rmsg, d);

			d->m_notice[i] = 0;
		}
	}
#endif

	{
		// appear
		CNetMsg::SP rmsg(new CNetMsg);
		AppearMsg(rmsg, d->m_pChar, true);
		area->SendToCell(rmsg, d->m_pChar);
	}

	if(d->m_pChar->holy_water_item != NULL)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		UpdateClient::holyWaterStateMsg(rmsg, d->m_pChar->m_index, d->m_pChar->holy_water_item->getDBIndex());
		d->m_pChar->m_pArea->SendToCell(rmsg, d->m_pChar, true);
	}

	// 방 주위의 캐릭터들의 정보를 보내줌...
	area->SendCellInfo(d->m_pChar, false);

	{
		// GO_ZONE 효과 메시지 전송
		CNetMsg::SP rmsg(new CNetMsg);
		EffectEtcMsg(rmsg, d->m_pChar, MSG_EFFECT_ETC_GOZONE);
		area->SendToCell(rmsg, d->m_pChar, true);
	}

// 여기에 메신저 초기데이타 보낸다..
	//MSG_EX_MESSENGER_GROUP_LIST, // 그룹 리스트 보내기 : count(n) gIndex(n) gName(str)
	if( d->m_pChar->m_Friend)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		rmsg->Init(MSG_EXTEND);
		RefMsg(rmsg) << MSG_EX_MESSENGER
					 << (unsigned char) MSG_EX_MESSENGER_GROUP_LIST
					 << d->m_pChar->m_Friend->GetGroupCount();

		CLCString gIndexList(255+1);
		CLCString gNameList(255+1);

		int gIndex = 0;

		d->m_pChar->m_Friend->GetGroupIndexString(gIndexList, gNameList);

		const char* pgName = (const char*) gNameList;
		const char* pgIndex = (const char*) gIndexList;

		char tmpBuf[MAX_STRING_LENGTH] = {0,};
		while(*pgIndex && *pgName)
		{
			pgIndex = AnyOneArg(pgIndex, tmpBuf);
			gIndex = atoi(tmpBuf);
			tmpBuf[0] = '\0';
			pgName = AnyOneArg(pgName, tmpBuf);

			RefMsg(rmsg) << gIndex
						 << tmpBuf;

			tmpBuf[0] = '\0';
		}

		SEND_Q(rmsg, d);
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		FriendListMsg(rmsg, d);
	}

	if(d->m_pChar->m_Friend)
	{
		d->m_pChar->m_nCondition = 1;

		CNetMsg::SP rmsg(new CNetMsg);
		HelperFriendSetConditionMsg(rmsg, d->m_pChar->m_index, d->m_pChar->m_nCondition, -1, d->m_pChar);
		SEND_Q(rmsg, m_helper);
	}
	{
		// 블럭리스트 보내기
		CNetMsg::SP rmsg(new CNetMsg);
		BlockPCListMsg(rmsg, d);
	}

	// 신청 가능 시간이면 확정 메시지를
	// 공성전 진행 중이면 진행 메시지를 보낸다
	///=== 메세지 막기 kjtest
	{
		castle = CWarCastle::GetCastleObject(CWarCastle::GetCurSubServerCastleZoneIndex());
		if (castle)
		{
			if (castle->GetState() == WCSF_NORMAL)
			{
				if (castle->IsJoinTime())
				{
					{
						struct tm nextWarTime;
						castle->GetNextWarTime(&nextWarTime, true);
						CNetMsg::SP rmsg(new CNetMsg);
						GuildWarNoticeTimeMsg(rmsg, ZONE_MERAC, nextWarTime.tm_mon, nextWarTime.tm_mday, nextWarTime.tm_hour, nextWarTime.tm_min);
						SEND_Q(rmsg, d);
					}
				}
			}
			else
			{
				CNetMsg::SP rmsg(new CNetMsg);
				GuildWarCastleStateMsg(rmsg, ZONE_MERAC, d->m_pChar, castle);
				SEND_Q(rmsg, d);
			}
		}

		// 이동 묻기
		if (zonePromptMove != -1)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			WarpPromptMsg(rmsg, zonePromptMove, posPromptMove);
			SEND_Q(rmsg, d);
		}
	}

	// 신청 가능 시간이면 확정 메시지를
	// 공성전 진행 중이면 진행 메시지를 보낸다
	{
		pCastle = CDratanCastle::CreateInstance();
		if (pCastle->GetState() == WCSF_NORMAL)
		{
			if (pCastle->IsJoinTime())
			{
				struct tm nextWarTime;
				pCastle->GetNextWarTime(&nextWarTime, true);
				CNetMsg::SP rmsg(new CNetMsg);
				GuildWarNoticeTimeMsg(rmsg, ZONE_DRATAN, nextWarTime.tm_mon, nextWarTime.tm_mday, nextWarTime.tm_hour, nextWarTime.tm_min);
				SEND_Q(rmsg, d);
			}
		}
		else
		{
			CNetMsg::SP rmsg(new CNetMsg);
			GuildWarCastleStateMsg(rmsg, ZONE_DRATAN, d->m_pChar, pCastle);
			SEND_Q(rmsg, d);
		}

		// 이동 묻기
		if (zonePromptMove_Dratan != -1)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			WarpPromptMsg(rmsg, zonePromptMove_Dratan, posPromptMove_Dratan);
			SEND_Q(rmsg, d);
		}
	}

#ifdef GER_LOG
	// 케릭터 로그인 시간기록
//	d->m_pChar->SetLoginTime();

	GAMELOGGEM << init( 0 , "CHAR_ENTER_GAME")
			   << LOG_VAL("account-id", d->m_idname) << blank
			   << LOG_VAL("character-name", d->m_pChar->m_name) << blank
			   << LOG_VAL("ipv4", d->getHostString()) << blank
			   << LOG_VAL("zone-id", d->m_pChar->m_pZone->m_index) << blank
			   << endGer;
#endif // GER_LOG
	GAMELOG << init("JOIN", d->m_pChar->m_name, d->m_pChar->m_nick, d->m_idname)
			<< d->m_pChar->m_pZone->m_index	<< delim
			<< d->getHostString()
			<< end;

	// 캐릭터 시간 정보 초기화
	d->SetHackCheckPulse();

#ifdef RANKER_NOTICE
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ConnRankerReqMsg(rmsg, d->m_pChar->m_index, d->m_pChar->m_index);
		SEND_Q(rmsg, gserver->m_connector);
	}
#endif // RANKER_NOTICE

	if( d->m_pChar->m_guildInfo && d->m_pChar->m_guildInfo->guild() )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ConnGuildPointRankerReqMsg( rmsg, d->m_pChar->m_guildInfo->guild()->index(), d->m_pChar->m_index );
		SEND_Q(rmsg, gserver->m_connector );
	}

	if( d->m_pChar->m_guildInfo && d->m_pChar->m_guildInfo->guild() && d->m_pChar->m_pArea)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildPointRankingMsg( rmsg, d->m_pChar->m_index, d->m_pChar->m_guildInfo->guild()->index(), d->m_pChar->m_guildInfo->guild()->GetGuildPointRanking() );
		d->m_pChar->m_pArea->SendToCell( rmsg, d->m_pChar );
	}
	if (d->m_pChar->m_pArea)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildNameColorStateMsg(rmsg, d->m_pChar );
		d->m_pChar->m_pArea->SendToCell(rmsg, d->m_pChar );
		SEND_Q(rmsg, d->m_pChar->m_desc);
	}

	// 파티 리스트에서 현재 유저의 파티 정보를 찾아 설정한다
	d->m_pChar->m_party = FindPartyByMemberIndex(d->m_pChar->m_index, false);
	if (d->m_pChar->IsParty())
	{
#ifdef PARTY_BUG_GER
		GAMELOG << init("SET_PARTY_MEMBER", d->m_pChar)
				<< "BOSSINDEX" << delim << d->m_pChar->m_party->GetBossIndex()
				<< end;
#endif // PARTY_BUG_GER

#ifdef BATTLE_PARTY_BOSS_CHANGE // 전투파티 버그 수정. 자기와 파장의 레벨차이가 10레벨이상 차이 날때 로그오프 했다가 들어오는 사람은 파탈함.
		if( d->m_pChar->m_party->GetPartyType(0) == MSG_PARTY_TYPE_BATTLE &&
				d->m_pChar->m_party->GetPartyType(3) == MSG_PARTY_TYPE_BATTLE &&
				d->m_pChar->m_party->GetPartyType(4) == MSG_PARTY_TYPE_BATTLE )
		{
			CPartyMember* member = d->m_pChar->m_party->GetMemberByListIndex(0);
			if(member != NULL)
			{
				if(ABS(d->m_pChar->m_level - member->m_nLevel) < 11)
				{
					d->m_pChar->m_party->SetMemberRegister_AfterGoZone(d->m_pChar);
				}
				else
				{
					// 로그인 유저 파티 탈퇴
					{
						CNetMsg::SP rmsg(new CNetMsg);
						HelperPartyQuitReqMsg(rmsg, d->m_pChar->m_party->GetBossIndex(), d->m_pChar->m_index);
						SEND_Q(rmsg, gserver->m_helper);
					}
				}
			}
		}
		else
			d->m_pChar->m_party->SetMemberRegister_AfterGoZone(d->m_pChar);
#else
		d->m_pChar->m_party->SetMemberRegister_AfterGoZone(d->m_pChar);
#endif // BATTLE_PARTY_BOSS_CHANGE
	}

	// 원정대 리스트에서 현재 유저의 원정대 정보를 찾아 설정한다
	d->m_pChar->m_Exped = FindExpedByMemberIndex(d->m_pChar->m_index, false);
	if(d->m_pChar->IsExped())
	{
		d->m_pChar->m_Exped->SetMemberRegister_AfterGoZone(d->m_pChar);
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperPartyMatchMemberChangeInfoMsg(rmsg, d->m_pChar->m_index, MSG_HELPER_PARTY_MATCH_MEMBER_CHANGE_INFO_ZONE, "", d->m_pChar->m_level, d->m_pChar->m_pZone->m_index);
		SEND_Q(rmsg, gserver->m_helper);
	}

	if (d->m_pChar->m_nGoldenBallNoticeStatus != m_clGoldenBall.GetStatus())
	{
		d->m_pChar->m_nGoldenBallNoticeStatus = m_clGoldenBall.GetStatus();

		switch (m_clGoldenBall.GetStatus())
		{
		case GOLDENBALL_STATUS_VOTE:
			{
				CNetMsg::SP rmsg(new CNetMsg);
				EventGoldenballVoteStartMsg(rmsg, m_clGoldenBall.GetTeam1Name(), m_clGoldenBall.GetTeam2Name(), m_clGoldenBall.GetEndYear(), m_clGoldenBall.GetEndMonth(), m_clGoldenBall.GetEndDay(), m_clGoldenBall.GetEndHour(), m_clGoldenBall.GetEndMinute());
				SEND_Q(rmsg, d);
			}
			break;

		case GOLDENBALL_STATUS_VOTE_END:
			{
				CNetMsg::SP rmsg(new CNetMsg);
				EventGoldenballVoteEndMsg(rmsg, m_clGoldenBall.GetTeam1Name(), m_clGoldenBall.GetTeam2Name());
				SEND_Q(rmsg, d);
			}
			break;

		case GOLDENBALL_STATUS_GIFT:
			{
				CNetMsg::SP rmsg(new CNetMsg);
				EventGoldenballGiftStartMsg(rmsg, m_clGoldenBall.GetTeam1Name(), m_clGoldenBall.GetTeam1Score(), m_clGoldenBall.GetTeam2Name(), m_clGoldenBall.GetTeam2Score(), m_clGoldenBall.GetEndYear(), m_clGoldenBall.GetEndMonth(), m_clGoldenBall.GetEndDay(), m_clGoldenBall.GetEndHour(), m_clGoldenBall.GetEndMinute());
				SEND_Q(rmsg, d);
			}
			break;

		default:
			break;
		}
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		CashItemGiftRecvNoticeRepMsg(rmsg, (d->m_userFlag & RECV_GIFT) ? 1 : 0);
		SEND_Q(rmsg, d);
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		ExPlayerStateChangeMsg(rmsg, d->m_pChar);
		SEND_Q(rmsg, d);
	}

	if(d->m_pChar->m_teachType == MSG_TEACH_TEACHER_TYPE)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperTeachLimitTimeCheck(rmsg, d->m_pChar->m_index);
		SEND_Q(rmsg, gserver->m_helper);
	}

	d->m_pChar->m_bLoadChar = true;
	d->m_pChar->OutputDBItemLog();

#ifdef FREE_PK_SYSTEM
	if( gserver->m_bFreePk )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		FreePKMsg(rmsg);
		SEND_Q( rmsg, d );
	}
#endif

	if( gserver->isActiveEvent( A_EVENT_XMAS ) )
	{
		CSkill *pSkill = gserver->m_skillProtoList.Create( 490 , 1 );
		if( pSkill )
		{
			bool bApply = false ;
			ApplySkill( d->m_pChar, d->m_pChar, pSkill, -1 , bApply );
			if( !bApply )
			{
				GAMELOG << init("EVENT XMAS SKILL FAILED (LOGIN) ", d->m_pChar ) << end;// 스킬 적용 실패
			}
		}
	}
	else
	{
		if(d->m_pChar->m_assist.FindBySkillIndex(490))
		{
			d->m_pChar->m_assist.CureBySkillIndex(490);
		}
	}

	if(d->m_pChar->m_assist.FindBySkillIndex(993))
	{
		d->m_pChar->m_assist.CureBySkillIndex(993);
	}

#ifdef EVENT_PCBANG_2ND
	if(d->m_location == BILL_LOCATION_PCBANG)
	{
		CSkill * pSkill = gserver->m_skillProtoList.Create( 493 , 1 );
		if(pSkill != NULL)
		{
			bool bApply = false;
			ApplySkill(d->m_pChar, d->m_pChar, pSkill, -1 , bApply);
			if(bApply == false)
			{
				GAMELOG << init("EVENT_PCBANG_2NDS SKILL FAILED (LOGIN) ", d->m_pChar ) << end;// 스킬 적용 실패
			}
		}
	}
#endif // EVENT_PCBANG_2ND

	// 피닉스 케릭터 이면서 100 레벨이 넘지 않는다면 피닉스 버프를 준다.
	if( d->m_pChar->m_bPhoenix_Char == 1 && d->m_pChar->m_level <= 100 )
	{
		CSkill * pSkill1 = gserver->m_skillProtoList.Create( 516, 1 );
		if(pSkill1 != NULL)
		{
			bool bApply = false;
			ApplySkill(d->m_pChar, d->m_pChar, pSkill1, -1 , bApply);
			if(bApply == false)
			{
				GAMELOG << init("EVENT_PHOENIX SKILL FAILED (LOGIN) ", d->m_pChar ) << end;// 스킬 적용 실패
			}
		}
	}

	if (pCastle != NULL && d->m_pChar != NULL && d->m_pChar->m_pZone->m_index == ZONE_DRATAN)
	{
		// 공성중이면 부활 진지 정보 전송
		CNetMsg::SP rmsg(new CNetMsg);
		CastletowerQuartersListMsg(rmsg, pCastle);
		SEND_Q(rmsg, d);
	}

	switch( d->m_pChar->m_job )
	{
	case JOB_TITAN:
	case JOB_KNIGHT:
	case JOB_MAGE:
#ifdef EX_MAGE
	case JOB_EX_MAGE:
#endif // EX_MAGE
	case JOB_ROGUE:
#ifdef EX_ROGUE
	case JOB_EX_ROGUE:
#endif // EX_ROGUE
	case JOB_SORCERER:
	default:
		d->m_pChar->m_attacktype = ATTACK_TYPE_NORMAL;
		break;
	case JOB_HEALER:
	case JOB_NIGHTSHADOW:
		d->m_pChar->m_attacktype = ATTACK_TYPE_DOUBLE;
		break;
	}

	// 트리거를 사용하는 Area이면
	if (area && area->m_bUseTriggerEvent)
	{
		// 해당 Area의 트리거 리스트 정보를 보내준다.
		area->m_CTriggerList.SyncForClient_TriggerInfo(d->m_pChar);
	}


	{
		CNetMsg::SP rmsg(new CNetMsg);
		NpcPortalLoadingEndUseMsg(rmsg);
		SEND_Q(rmsg, d);
	}

	// 던전타임중일 때 알림 보낼것.
	if(m_bDungeonTimeToggle == true && m_bIsTime == true)
	{
		//msg 구성 할 것.
		int exp = 100;
		for(int i = 0; i < gserver->m_iZoneCount; i++)
		{
			if(gserver->m_iZoneExp[i] > 100)
			{
				exp = gserver->m_iZoneExp[i] - 100;
				break;
			}
		}

		CNetMsg::SP rmsg(new CNetMsg);
		DungeonTimeNoticeMsg(rmsg, MSG_EX_DUNGEONTIME_NOTICE, m_iStartTime, m_iEndTime, exp);
		SEND_Q(rmsg, d);
	}

	if( d->m_pChar->m_job == JOB_NIGHTSHADOW )
		d->m_pChar->m_newtutoComplete = 1;

	if( d->m_pChar->m_newtutoComplete == 0 )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		NewTutorialMsg(rmsg);
		SEND_Q(rmsg,d);
	}

	if(d->m_pChar->m_pZone->m_index == ZONE_TARIAN_DUNGEON)
	{
		{
			CNetMsg::SP rmsg(new CNetMsg);
			SendRaidSceneObjectStateMsg(rmsg, d->m_pChar);
			SEND_Q(rmsg, d->m_pChar->m_desc);
		}

		int loop;
		for(loop = 0; loop < 8; loop++)
		{
			if(d->m_pChar->m_pArea->m_AkanPCList[loop] == NULL)
			{
				d->m_pChar->m_pArea->m_AkanPCList[loop] = d->m_pChar;
				break;
			}
		}
		if(loop == 8)
		{
			// 아칸사원 인원은 꽉찾는데, 더 들어왔다. 튕겨버려!
			CNetMsg::SP rmsg(new CNetMsg);
			RaidInzoneQuitReq(rmsg,1,0);
			do_Raid(d->m_pChar, rmsg);
			return ;
		}
	}

	if(d->m_pChar->m_pZone->m_index == ZONE_AKAN_TEMPLE)
	{
		{
			CNetMsg::SP rmsg(new CNetMsg);
			SendRaidSceneObjectStateMsg(rmsg, d->m_pChar);
			SEND_Q(rmsg, d->m_pChar->m_desc);
		}

		int loop;
		for(loop = 0; loop < 8; loop++)
		{
			if(d->m_pChar->m_pArea->m_AkanPCList[loop] == NULL)
			{
				d->m_pChar->m_pArea->m_AkanPCList[loop] = d->m_pChar;
				break;
			}
		}
		if(loop == 8)
		{
			// 아칸사원 인원은 꽉찾는데, 더 들어왔다. 튕겨버려!
			CNetMsg::SP rmsg(new CNetMsg);
			RaidInzoneQuitReq(rmsg,1,0);
			do_Raid(d->m_pChar, rmsg);
			return ;
		}
	}

	char bCastellan;
	int castleNum;
	if(!d->m_pChar->CheckCastellanType(castleNum, bCastellan))
		castleNum = -1;

	// castleNum 제외된 아이템, 호칭지움
	d->m_pChar->m_inventory.CastllanItemRemove(castleNum, false, true, bCastellan);
	d->m_pChar->CastllanTitleDelete(castleNum, true, bCastellan);

	{
		CNetMsg::SP rmsg(new CNetMsg);
		if(gserver->m_bCashShopLock == true)
			MsgCashShopLock(rmsg, MSG_EX_CASHITEM_SHOP_LOCK);
		else
			MsgCashShopLock(rmsg, MSG_EX_CASHITEM_SHOP_UNLOCK);
		SEND_Q(rmsg, d->m_pChar->m_desc);
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);

		if(g_bUpgradeEvent)
			EventEnchantRate(rmsg, 1, g_nUpgradeProb);
		else
			EventEnchantRate(rmsg, 0, g_nUpgradeProb);

		SEND_Q(rmsg, d->m_pChar->m_desc);
	}

#ifdef JUNO_RENEWAL_MESSAGEBOX
	if(bMessageBoxPopUp)
	{
		if(d->m_pChar->m_job != JOB_NIGHTSHADOW)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			MsgMessageBox(rmsg, MSG_EX_MSGBOX_CHANGE_START_POINT);
			SEND_Q(rmsg, d);
		}
	}
#endif // JUNO_RENEWAL_MESSAGEBOX

	if(d->m_pChar->m_pZone->IsWarGroundZone())
	{
		CWaitPlayer* pWp = NULL;
		pWp = gserver->m_RoyalRumble.m_WaitPlayerList.GetNode(d->m_pChar->m_index);
		if(pWp != NULL)
			pWp->SetCheckIn(true);
	}
	else if(gserver->m_RoyalRumble.GetRoyalRumbleNotice())
	{
		if(gserver->m_subno == WAR_GROUND_CHANNEL)
		{
			if(!gserver->m_RoyalRumble.m_WGPlayerList.FindNode(d->m_pChar->m_index))
			{
				{
					CNetMsg::SP rmsg(new CNetMsg);
					RoyalRumbleRegistMenu(rmsg, 1);
					SEND_Q(rmsg, d->m_pChar->m_desc);
				}

				if(d->m_pChar->m_admin > 0)
				{
					GAMELOG << init("ROYAL RUMBLE") << "ROYAL RUMBLE NOTICE IS true" << end;
				}
			}
			int command = gserver->m_RoyalRumble.GetRoyalRumbleProcess();
			if(command >= 1 && command <= 5)
			{
				if(command == 1 || command == 2)
					command = 0;
				else if(command > 2)
					command = command - 2;

				{
					CNetMsg::SP rmsg(new CNetMsg);
					RoyalRumbleTimeNotice(rmsg, (unsigned char)command, gserver->m_RoyalRumble.GetRemainWaitTime());
					SEND_Q(rmsg, d->m_pChar->m_desc);
				}
			}
		}
	}
	else if(!gserver->m_RoyalRumble.GetRoyalRumbleNotice())
	{
		if(gserver->m_subno == WAR_GROUND_CHANNEL)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			RoyalRumbleRegistMenu(rmsg, 0);
			SEND_Q(rmsg, d->m_pChar->m_desc);
			if(d->m_pChar->m_admin > 0)
			{
				GAMELOG << init("ROYAL RUMBLE") << "ROYAL RUMBLE NOTICE IS false" << end;
			}
		}
	}
	// 존이동 시 ITEM_FLAG_ZONEMOVE_DEL 인 아이템은 지워버림.
	{
		item_search_t vec;
		d->m_pChar->m_inventory.searchFlagByItemProto(ITEM_FLAG_ZONEMOVE_DEL, vec);
		item_search_t::iterator it = vec.begin();
		item_search_t::iterator endit = vec.end();
		for (; it != endit; ++it)
		{
			CItem* item = (*it).pItem;

			CNetMsg::SP rmsg(new CNetMsg);
			insideServer_do_ItemDelete(rmsg, item);
			do_Item(d->m_pChar, rmsg);
		}
	}

#ifdef REFORM_PK_PENALTY_201108 // 2011-08 PK 패널티 리폼
	{
		CNetMsg::SP rmsg(new CNetMsg);
		MsgSubHelperPKPenaltyRewardInfoReq( rmsg, d->m_pChar->m_index );
		SEND_Q( rmsg, gserver->m_subHelper );
	}
#endif // REFORM_PK_PENALTY_201108

	std::map<int, CEventInfo*> * eventList = gserver->getActiveEvenList();
	if( eventList && eventList->size() > 0 )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		EventActiveListMsg(rmsg, eventList);
		SEND_Q(rmsg, d->m_pChar->m_desc);
	}

	if( d->m_pChar->m_pArea->GetPCCount() > 1
			&& d->m_pChar->m_admin == 0
			&& d->m_pChar->m_pZone->IsComboZone()
			&& (!d->m_pChar->IsParty()) )			// area에 다른 사람이 있는데 내가 파티가 아니다. 그러면 파티 몬스터 콤보인데 내가 잘못들어간 것이다. 다시 나간다.
	{
		int extra = 0;
		CZone* pZone = gserver->FindZone(ZONE_DRATAN);
		if(pZone)
		{
			GAMELOG << init("GET OUT COMBO AREA BY PARTY QUIT", d->m_pChar)
					<< "AREA INDEX: "
					<< d->m_pChar->m_pArea->m_index
					<< "COMBO INDEX: "
					<< d->m_pChar->m_pArea->m_monsterCombo->m_nIndex
					<< end;

			GoZoneForce(d->m_pChar, pZone->m_index,
						pZone->m_zonePos[extra][0],
						GetRandom(pZone->m_zonePos[extra][1], pZone->m_zonePos[extra][3]) / 2.0f,
						GetRandom(pZone->m_zonePos[extra][2], pZone->m_zonePos[extra][4]) / 2.0f);
		}
	}

	d->m_pChar->m_pArea->SetMelted();

	{
		if (d->m_pChar->m_memposTime - gserver->getNowSecond() <= 0)
		{
			d->m_pChar->m_memposTime = 0;
		}

		if (d->m_pChar->m_stashextTime - gserver->getNowSecond() <= 0)
		{
			d->m_pChar->m_stashextTime = 0;
		}

		CNetMsg::SP rmsg(new CNetMsg);
		UpdateClient::makeOldTimerItem(rmsg, d->m_pChar->m_memposTime, d->m_pChar->m_stashextTime);
		SEND_Q(rmsg, d);
	}

#ifdef PREMIUM_CHAR
	d->m_pChar->m_premiumChar.sendInfo();

	// 이 구문은 로그아웃상태에서 시간이 종료되었을 떄를 검사함
	d->m_pChar->m_premiumChar.checkExpireTime(gserver->getNowSecond());

	d->m_pChar->m_premiumChar.checkJumpCount();
#endif

	d->m_pChar->m_first_inmap = false;	// 이 구문은 항상 함수 맨 마지막에 사용할것
}

#ifdef GMTOOL
void CServer::GMToolCharPrePlay(CDescriptor* d)
{
	int i = 0;
	char id[MAX_ID_NAME_LENGTH + 1];
	char tmpBuf[MAX_STRING_LENGTH] = {0,};
	for(i = 0; i < gserver->m_nGMToolCount; ++i)
	{
		sprintf(tmpBuf, "GMTOOL_%d", i);
		if(gserver->m_config.Find(tmpBuf, "ID"))
		{
			memset(id, 0, MAX_ID_NAME_LENGTH + 1);
			strcpy(id, gserver->m_config.Find(tmpBuf, "ID"));
			if(strcmp(id, d->m_idname) == 0)
			{
				break;
			}
		}
	}

	if(i == gserver->m_nGMToolCount)
	{
		d->Close("Not gm id.");
		return ;
	}

	if(gserver->m_nGMToolCount > 0)
	{
		for(i = 0; i < gserver->m_nGMToolCount; ++i)
		{
			if(gserver->m_gmtool[i] == NULL)
			{
				gserver->m_gmtool[i] = d;
				break;
			}
		}

		if(i == gserver->m_nGMToolCount)
		{
			d->Close("gm tool is full.");
			return ;
		}
	}
	else
	{
		d->Close("gm tool count is 0.");
		return ;
	}

	{
		int key = 0;
		CNetMsg::SP rmsg(new CNetMsg);
		rmsg->Init(MSG_LOGIN);
		RefMsg(rmsg) << d->m_pChar->m_index
					 << d->m_pChar->m_name
					 << d->m_nPrepareSeed
					 << d->m_pChar->m_job
					 << key;

		SEND_Q(rmsg, d);
	}

	STATE(d) = CON_PLAYING;
	CDescriptor::auto_set[d->m_autoSaveSeq].erase(d);
}
#endif // GMTOOL

CMoonStoneRewardData::CMoonStoneRewardData()
{
}

CMoonStoneRewardData::~CMoonStoneRewardData()
{
	map_reward_t::iterator it = m_listReward.begin();
	map_reward_t::iterator endit = m_listReward.end();
	for (; it != endit; ++it)
	{
		delete (it->second);
	}
	m_listReward.clear();
}

bool CMoonStoneRewardData::Load(int idx)
{
	CLCString sql(1024);
	sql.Format("SELECT * FROM t_moonstone_reward WHERE a_type = %d ORDER BY a_giftindex", idx);

	CDBCmd cmd;
	cmd.Init(&gserver->m_dbdata);
	cmd.SetQuery(sql);
	if(!cmd.Open())
		return false;

	m_nMoonStoneIndex = idx;
	m_nTotalProb = 0;
	int giftindex, giftcount, giftflag;
	float giftprob;
	while(cmd.MoveNext())
	{
		REWARD* reward = new REWARD;

		cmd.GetRec("a_giftindex", giftindex);
		cmd.GetRec("a_giftcount", giftcount);
		cmd.GetRec("a_giftflag", giftflag);
		cmd.GetRec("a_giftprob", giftprob);

		reward->rewardindex = giftindex;
		reward->rewardcount = giftcount;
		reward->prob		= giftprob;
		reward->rewardflag	= giftflag;

		m_listReward.insert(map_reward_t::value_type(reward->rewardindex, reward));
		m_nTotalProb += (int)(giftprob * 10000.0);
	}

	return true;
}

CMoonStoneReward::CMoonStoneReward()
{
	m_pReward = NULL;
	m_nRewardDataCount = 0;
}

CMoonStoneReward::~CMoonStoneReward()
{
	if(m_pReward)
	{
		delete[] m_pReward;
		m_pReward = NULL;
	}

	m_nRewardDataCount = 0;
}

bool CMoonStoneReward::Load()
{
	CLCString sql(1024);
	sql.Format("SELECT distinct a_type FROM t_moonstone_reward ORDER BY a_type");
	CDBCmd cmd, cmddata;
	cmd.Init(&gserver->m_dbdata);
	cmddata.Init(&gserver->m_dbdata);

	cmd.SetQuery(sql);
	if(!cmd.Open())
		return false;

	if(cmd.m_nrecords == 0)
		return false;

	int i = 0;
	int type;
	m_nRewardDataCount = cmd.m_nrecords;
	m_pReward = new CMoonStoneRewardData[cmd.m_nrecords];
	while(cmd.MoveNext())
	{
		cmd.GetRec("a_type", type);
		if(!m_pReward[i].Load(type))
			return false;

		map_.insert(map_t::value_type(m_pReward[i].m_nMoonStoneIndex, &m_pReward[i]));

		i++;
	}
	return true;
}


void CServer::ProcMonsterCombo()
{
	int i;
	for(i = 0; i < m_comboZone->m_countArea; ++i)
	{
		if(m_comboZone->m_area[i].m_bEnable)
			m_comboZone->m_area[i].RunComboZone();
	}

	CZone* pAkanZone = gserver->FindZone(ZONE_AKAN_TEMPLE);
	if (pAkanZone)
	{
		for(int i = 0; i < pAkanZone->m_countArea; i++)
		{
			if(pAkanZone->m_area[i].m_GroundEffect2.IsStarted())
			{
				pAkanZone->m_area[i].m_GroundEffect2.Activity();
			}
		}
	}

	CZone* pDungeon4 = gserver->FindZone(ZONE_DUNGEON4);
	if (pDungeon4)
	{
		for(int i = 0; i < pDungeon4->m_countArea; i++)
		{
			if(pDungeon4->m_area[i].m_GroundEffect2.IsStarted())
			{
				pDungeon4->m_area[i].m_GroundEffect2.Activity();
			}
		}
	}
}

void CServer::ProcEndExped()
{
	time_t t_now = time(NULL);

	//원정대 해체 처리
	map_listexped_t::iterator it = m_listExped.begin();
	map_listexped_t::iterator endit = m_listExped.end();
	for(; it != endit; ++it)
	{
		CExpedition* pExped = it->second;

		int nEndExpedStartTime = pExped->GetEndExpedTime();
		if(nEndExpedStartTime > 0)
		{
			//20 초과 체크
			if((t_now - nEndExpedStartTime) >= 20)
			{
				//원정대 해체 메세지 전송
				if (gserver->isRunHelper())
				{
					CNetMsg::SP rmsg(new CNetMsg);
					HelperExpedEndExpedMsg(rmsg, pExped->GetBossIndex());
					SEND_Q(rmsg, gserver->m_helper);

					pExped->SetEndExpedTime(0);
				}
			}
		}
	}
}

void CServer::ProcEndParty()
{
	time_t t_now = time(NULL);

	//파티 해체 처리
	map_listparty_t::iterator it = m_listParty.begin();
	map_listparty_t::iterator endit = m_listParty.end();
	for(; it != endit; ++it)
	{
		CParty* pParty = it->second;

		int nEndPartyStartTime = pParty->GetEndPartyTime();
		if(nEndPartyStartTime > 0)
		{
			//20 초과 체크
			if((t_now - nEndPartyStartTime) >= 20)
			{
				//파티 해체 메세지 전송
				if (gserver->isRunHelper())
				{
					CNetMsg::SP rmsg(new CNetMsg);
					HelperPartyEndPartyReqMsg(rmsg, pParty->GetBossIndex());
					SEND_Q(rmsg, gserver->m_helper);

					pParty->SetEndPartyTime(0);
				}
			}
		}
	}
}

void CServer::ProcTrigger()
{
	int i, j;

	for (i=0; i < this->m_numZone; i++)
	{
		if (m_zones[i].m_bRemote)
			continue ;
		for (j=0; j< m_zones[i].m_countArea; j++)
		{
			// 트리거를 사용하는 Area이면
			if (m_zones[i].m_area[j].m_bEnable && m_zones[i].m_area[j].m_bUseTriggerEvent)
			{
				if (m_pulse - m_zones[i].m_area[j].m_pulseProcTrigger >= PULSE_TRIGGER_SYSTEM)
				{
					// 타이머 체크하는 함수 사용, hardcording 한 것을 처리하기
					m_zones[i].m_area[j].m_pulseProcTrigger = m_pulse;
					m_zones[i].m_area[j].m_CTriggerList.TriggerTimerCheck();
				}
			}
		}
	}
}

/* 추가 요망...나중에 존인덱스 추가되면 여기 수정*/
int	CServer::GetTriggerSetTypeToZoneIndex(int zoneIndex)
{
	int triggerSetType = 0;

	switch(zoneIndex)
	{
	case ZONE_EGEHA:
		triggerSetType = TRIGGERSET_EGEHA;
		break;

	case ZONE_STREIANA:
		triggerSetType = TRIGGERSET_STREIANA;
		break;

	case ZONE_MONDSHINE:
		triggerSetType = TRIGGERSET_MONDSHINE;
		break;

	case ZONE_CAPPELLA_1:
		triggerSetType = TRIGGERSET_CAPPELLA_1;
		break;

	case ZONE_CAPPELLA_2:
		triggerSetType = TRIGGERSET_CAPPELLA_2;
		break;

	case ZONE_ALTER_OF_DARK:
		triggerSetType = TRIGGERSET_ALTER_OF_DARK;
		break;

	case ZONE_AKAN_TEMPLE:
		break;
	case ZONE_TARIAN_DUNGEON:
		break;
	case 999:
		triggerSetType = TRIGGERSET_TESTRAID;
		break;

	default:
		break;
	}

	return triggerSetType;
}

CGrobalEcho::CGrobalEcho( )
{
	for( int i=0; i< MAX_GECHO_SIZE; i++ )
	{
		checkClock[i] = 0;
		remainTime[i] = 0;
		repeatTime[i] = 1;
	}
}

int CGrobalEcho::FindIndex( )
{
	int idx = 0 ;
	unsigned int min = remainTime[0];
	for( int i=1 ; i < MAX_GECHO_SIZE; i++ )
	{
		if( remainTime[i] < min )
		{
			min = remainTime[i];
			idx = i;
		}
	}
	return idx;
}

void CGrobalEcho::Set( CNetMsg::SP& _msg , unsigned int _repeatTime, unsigned int _time )
{
	int idx = FindIndex();
	msg[idx].reset();
	msg[idx] = _msg;
	repeatTime[idx] = _repeatTime * 60;		// 분단위로 입력해서 초로 변환
	remainTime[idx] = _time * 60;			// 분단위로 입력해에 초로 변환
}

void CGrobalEcho::Print( )
{
	for( int idx=0; idx < MAX_GECHO_SIZE; idx++ )
	{
		if( remainTime[idx] > 0 )
		{
			checkClock[idx]++;
			remainTime[idx]--;
			if( remainTime[idx] < 0 )
				remainTime[idx] = 0;

			if( repeatTime[idx] < 1 )
				repeatTime[idx] = 30;

			if( checkClock[idx] % repeatTime[idx] == 0 )
			{
				SEND_Q( msg[idx], gserver->m_messenger);
				checkClock[idx] = 0;
			}
		}
	}
}
void CServer::SetDungeonTime()
{
	CDBCmd cmd;

	cmd.Init(&gserver->m_dbdata);
	std::string select_zonedata_query = "SELECT a_zone_index FROM t_zonedata";
	cmd.SetQuery(select_zonedata_query);
	if (!cmd.Open() || !cmd.MoveFirst())
		return ;

	m_iZoneExp = new int[cmd.m_nrecords];
	m_iZoneCount = cmd.m_nrecords;

	for(int i = 0; i < cmd.m_nrecords; i++)
	{
		switch(i)
		{
		case ZONE_DUNGEON1:
		case ZONE_DUNGEON2:
		case ZONE_DUNGEON3:
		case ZONE_DUNGEON4:
		case ZONE_EGEHA_DUNGEON_1:
		case ZONE_EGEHA_DUNGEON_8:
		case ZONE_EGEHA_DUNGEON_9:
		case ZONE_EGEHA_DUNGEON_10:
		case ZONE_DRATAN_CASTLE_DUNGEON:
		case ZONE_SPRIT_CAVE:
		case ZONE_QUANIAN_CAVE:
		case ZONE_GOLEM_CAVE:
		case ZONE_FLORAIM_CAVE:
		case ZONE_TRIVIA_CANYON:
		case ZONE_MISTY_CANYON:
#if !defined (BILA_DUNGEON_TIME_EXP_CHANGE)
			m_iZoneExp[i] = 150;
#else
		case ZONE_EBONY_MINE:
			m_iZoneExp[i] = 150;
#endif // BILA_DUNGEON_TIME_EXP_CHANGE
			break;

		default:
			m_iZoneExp[i] = 100;
			break;
		}
	}

	// FILE OPEN 하여서 세팅된 값 읽어오기.
	FILE* fDungeonTime = fopen(DUNGEON_TIME_SAVEFILE, "rt");
	if(fDungeonTime == NULL)
	{
		//기본 세팅값 파일에 저장.
		SaveDungeonTime(m_iZoneCount);
	}
	else
	{
		//파일저장값 load.
		fclose(fDungeonTime);
		fDungeonTime = NULL;
		LoadDungeonTime(m_iZoneCount);
	}
	ModifyDungeonTime(0);
}

void CServer::SaveDungeonTime(int count)
{
	int i;
	FILE* fDungeonTime = fopen(DUNGEON_TIME_SAVEFILE, "wt");
	if(fDungeonTime)
	{
		fprintf(fDungeonTime, "%d\n", count);
		for(i = 0; i < count; i++)
		{
			fprintf(fDungeonTime, "%d\n", gserver->m_iZoneExp[i]);
		}
		for(i = 0; i < 24; i++)
		{
			if(gserver->m_bTimeTable[i] == false)
				fprintf(fDungeonTime, "0\n");
			else if(gserver->m_bTimeTable[i] == true)
				fprintf(fDungeonTime, "1\n");
		}
		fprintf(fDungeonTime, "%d\n", gserver->m_iTimeInterval);
		if(gserver->m_bDungeonTimeToggle == false)
			fprintf(fDungeonTime, "0\n");
		else if(gserver->m_bDungeonTimeToggle == true)
			fprintf(fDungeonTime, "1\n");
		fclose(fDungeonTime);
		fDungeonTime = NULL;
	}
}

void CServer::LoadDungeonTime(int count)
{
	int i;
	int iZoneCount;
	bool bLoadFail = false;
	int* iExp;
	iExp = new int[count];
	int iInterval;
	bool bTime[24];
	bool bStart;

	FILE* fDungeonTime = fopen(DUNGEON_TIME_SAVEFILE, "rt");
	if(fDungeonTime != NULL)
	{
		char tmpBuf[MAX_STRING_LENGTH] = {0,};
		if(fgets(tmpBuf, 256, fDungeonTime) == NULL)			bLoadFail = true;
		else	iZoneCount = atoi(tmpBuf);

		if (iZoneCount < count)
		{
			LOG_ERROR("not equal DungeonTime. db[%d] config[%d]", count, iZoneCount);
			exit(1);
		}

		for(i = 0; i < count; i++)
		{
			if(fgets(tmpBuf, 256, fDungeonTime) == NULL)		bLoadFail = true;
			else	iExp[i] = (short int)atoi(tmpBuf);
		}

		for(i = 0; i < 24; i++)
		{
			if(fgets(tmpBuf, 256, fDungeonTime) == NULL)		bLoadFail = true;
			else
			{
				if(atoi(tmpBuf) == 0)
					bTime[i] = false;
				else if(atoi(tmpBuf) == 1)
					bTime[i] = true;
			}
		}

		if(fgets(tmpBuf, 256, fDungeonTime) == NULL)			bLoadFail = true;
		else	iInterval = (short int)atoi(tmpBuf);

		if(fgets(tmpBuf, 256, fDungeonTime) == NULL)			bLoadFail = true;
		else
		{
			if(atoi(tmpBuf) == 0)
				bStart = false;
			else if(atoi(tmpBuf) == 1)
				bStart = true;
		}

		if(bLoadFail == false)								// bLoadFail이 true 이면 기본 세팅으로 놔둔다.
		{
			for(i = 0; i < count; i++)
			{
				m_iZoneExp[i] = iExp[i];
			}

			for(i = 0; i < 24; i++)
			{
				m_bTimeTable[i] = bTime[i];
			}

			m_iTimeInterval = iInterval;
			m_bDungeonTimeToggle = bStart;
		}
	}
}
void CServer::CheckDungeonTime()
{
	// 시작 시간
	LOG_INFO("DUNGEON TIME > RealSystemTime.Hour : %d and TimeTable : %d", m_tRealSystemTime.tm_hour, m_bTimeTable[m_tRealSystemTime.tm_hour] ? 1 : 0);
	for(int i=0; i<24; i++)
	{
		LOG_INFO("DUNGEON TIME > TimeTableList : %d and TimeTableRealDate : %d", m_bTimeTable[i] ? 1 : 0, m_bTimeTable[i]);
	}

	if(m_bTimeTable[m_tRealSystemTime.tm_hour] == true)
	{
		// 던전타임 시작 메세지 전 플레이어들에게 전송
		m_iStartTime = m_tRealSystemTime.tm_hour;
		m_iEndTime = m_iStartTime + m_iTimeInterval;

		if(m_iEndTime > 23)
			m_iEndTime = m_iEndTime - 24;

		int exp = 100;
		for(int i = 0; i < gserver->m_iZoneCount; i++)
		{
			if(gserver->m_iZoneExp[i] > 100)
			{
				exp = gserver->m_iZoneExp[i] - 100;
				break;
			}
		}

		{
			CNetMsg::SP rmsg(new CNetMsg);
			DungeonTimeNoticeMsg(rmsg, MSG_EX_DUNGEONTIME_START, m_iStartTime, m_iEndTime, exp);
			PCManager::instance()->sendToAll(rmsg);
		}

		m_bIsTime = true;
		LOG_INFO("DUNGEON TIME START > StartTime : %d and EndTime : %d and Interval : %d", m_iStartTime, m_iEndTime, m_iTimeInterval);
		return ;
	}

	else if (m_tRealSystemTime.tm_hour == m_iEndTime)
	{
		// 종료
		// 던전타임 종료 메세지 전 플레이어들에게 전송
		CNetMsg::SP rmsg(new CNetMsg);
		DungeonTimeNoticeMsg(rmsg, MSG_EX_DUNGEONTIME_END, -1, -1, -1);
		LOG_INFO("DUNGEON TIME END > StartTime : %d and EndTime : %d and Interval : %d", m_iStartTime, m_iEndTime, m_iTimeInterval);
		PCManager::instance()->sendToAll(rmsg);
		m_bIsTime = false;
		return ;
	}

}
void CServer::ModifyDungeonTime(int flag)
{
	int iStart = -1;
	int iEnd = -1;
	bool bTime = m_bIsTime;
	int nowtime = -1;

	nowtime = m_tRealSystemTime.tm_hour;

	for(int i = 23; i >= 0; i--)
	{
		if(m_bTimeTable[i] == true && i <= nowtime)
		{
			iStart = i;
			break;
		}
	}
	if(iStart == -1)
	{
		for(int i = 23; i >= 0; i--)
		{
			if(m_bTimeTable[i] == true)
			{
				iStart = i;
				break;
			}
		}
	}

	iEnd = iStart + m_iTimeInterval;
	m_iStartTime = iStart;
	m_iEndTime = iEnd;
	if(iEnd > 23)
	{
		iEnd -= 24;
		if(nowtime >= iStart || nowtime < iEnd)
		{
			//지금은 던전타임!!!
			//false에서 true로 바뀌면 공지!!!
			int exp = 100;
			for(int i = 0; i < gserver->m_iZoneCount; i++)
			{
				if(gserver->m_iZoneExp[i] > 100)
				{
					exp = gserver->m_iZoneExp[i] - 100;
					break;
				}
			}
			m_bIsTime = true;
			if(bTime == false)
			{
				if(gserver->m_bDungeonTimeToggle == true)
				{
					CNetMsg::SP rmsg(new CNetMsg);
					DungeonTimeNoticeMsg(rmsg, MSG_EX_DUNGEONTIME_START, m_iStartTime, m_iEndTime, exp);
					PCManager::instance()->sendToAll(rmsg);
					GAMELOG << init("DUNGEON TIME") << "START" << end;
				}
			}
			if(flag == 1)
			{
				CNetMsg::SP rmsg(new CNetMsg);
				DungeonTimeNoticeMsg(rmsg, MSG_EX_DUNGEONTIME_START, m_iStartTime, m_iEndTime, exp);
				PCManager::instance()->sendToAll(rmsg);
				GAMELOG << init("DUNGEON TIME") << "START" << end;
			}
			if(flag == 2)
			{
				CNetMsg::SP rmsg(new CNetMsg);
				DungeonTimeNoticeMsg(rmsg, MSG_EX_DUNGEONTIME_END, -1, -1, -1);
				PCManager::instance()->sendToAll(rmsg);
				GAMELOG << init("DUNGEON TIME") << "END" << end;
			}
		}
		else
		{
			// 지금은 쉬는 타임
			// true에서 false로 바뀌었으면 종료 공지!!
			m_bIsTime = false;
			if(bTime == true)
			{
				CNetMsg::SP rmsg(new CNetMsg);
				DungeonTimeNoticeMsg(rmsg, MSG_EX_DUNGEONTIME_END, -1, -1, -1);
				PCManager::instance()->sendToAll(rmsg);
				GAMELOG << init("DUNGEON TIME") << "END" << end;
			}
		}
	}
	else
	{
		if (iStart <= nowtime && iEnd > nowtime)
		{
			// 지금은 던전 타임!!!
			// false 에서 true 로 바뀌면 공지!!!
			int exp = 100;
			for(int i = 0; i < gserver->m_iZoneCount; i++)
			{
				if(gserver->m_iZoneExp[i] > 100)
				{
					exp = gserver->m_iZoneExp[i] - 100;
					break;
				}
			}
			m_bIsTime = true;
			if(bTime == false)
			{
				CNetMsg::SP rmsg(new CNetMsg);
				DungeonTimeNoticeMsg(rmsg, MSG_EX_DUNGEONTIME_START, m_iStartTime, m_iEndTime, exp);
				PCManager::instance()->sendToAll(rmsg);
			}
			if(flag == 1)
			{
				CNetMsg::SP rmsg(new CNetMsg);
				DungeonTimeNoticeMsg(rmsg, MSG_EX_DUNGEONTIME_START, m_iStartTime, m_iEndTime, exp);
				PCManager::instance()->sendToAll(rmsg);
			}
			if(flag == 2)
			{
				CNetMsg::SP rmsg(new CNetMsg);
				DungeonTimeNoticeMsg(rmsg, MSG_EX_DUNGEONTIME_END, -1, -1, -1);
				PCManager::instance()->sendToAll(rmsg);
			}
		}
		else
		{
			// 지금은 쉬는 타임
			// true에서 false로 바뀌면 공지!!!
			m_bIsTime = false;
			if(bTime == true)
			{
				CNetMsg::SP rmsg(new CNetMsg);
				DungeonTimeNoticeMsg(rmsg, MSG_EX_DUNGEONTIME_END, -1, -1, -1);
				PCManager::instance()->sendToAll(rmsg);
			}
		}
	}
}

void CServer::CheckRaidReset()
{
	if(this->m_subno != RAID_SUBNUMBER)
	{
		return ;
	}

	if( (this->m_nResetDay != 7) &&
			(this->m_tRealSystemTime.tm_wday != this->m_nResetDay) &&
			(this->m_tRaidResetProcess == 0) )
	{
		return ;
	}

	if(m_tRaidResetProcess == 0)
	{
		if( this->m_tRealSystemTime.tm_hour == this->m_nResethr &&
				this->m_tRealSystemTime.tm_min == this->m_nResetmin )
		{
			m_tRaidResetProcess = mktime(&m_tRealSystemTime);
			// 첫번째 공지 시작
			// (5 - m_nProcess)
			GAMELOG << init("RAID AUTO RESET REMAIN") << (5-m_nProcess) << end;
			CNetMsg::SP rmsg(new CNetMsg);
			SysRaidZoneRemain(rmsg, (5-m_nProcess));

			PCManager::instance()->sendToAll(rmsg);
			// 첫번째 공지 끝
			m_tRaidResetProcess += 60;
			m_nProcess++;
			m_bIsCheckTime = true;
		}
		else
			return ;
	}
	else
	{
		if( mktime(&m_tRealSystemTime) >= m_tRaidResetProcess )
		{
			// 1~n차 공지
			if(m_nProcess <= 5)
			{
				//(5 - m_nProcess)
				if((5 - m_nProcess) == 0)
				{
					GAMELOG << init("NO ENTER RAID ZONE") << end;
					CNetMsg::SP rmsg(new CNetMsg);
					SysMsg(rmsg, MSG_SYS_NO_ENTER_RAID_ZONE);

					PCManager::instance()->sendToAll(rmsg);
					m_bCanEnterTheRaidDungeon = false;
				}
				else
				{
					GAMELOG << init("RAID AUTO RESET REMAIN") << (5-m_nProcess) << end;
					CNetMsg::SP rmsg(new CNetMsg);
					SysRaidZoneRemain(rmsg, (5-m_nProcess));
					PCManager::instance()->sendToAll(rmsg);
				}
			}

			if(m_nProcess == 6)
			{
				// 초기화
				GAMELOG << init("START RAID RESET") << end;
				ResetRaid();
			}

			if(m_nProcess == 10)
			{
				// 종료공지시작
				CNetMsg::SP rmsg(new CNetMsg);
				SysMsg(rmsg, MSG_SYS_RAID_RESET_COMPLETE);
				PCManager::instance()->sendToAll(rmsg);
				GAMELOG << init("END RAID RESET") << end;
				// 종료공지 끝
				m_bCanEnterTheRaidDungeon = true;
				m_bIsCheckTime = false;
				m_tRaidResetProcess = 0;
				m_nProcess = 0;
			}

			if(m_bIsCheckTime == true)
			{
				m_tRaidResetProcess += 60;
				m_nProcess++;
			}
		}
	}
}
void CServer::ResetRaid()
{
	int i, j;
	int AreaArrayIndex = 0;
	for(i = 0; i < this->m_numZone; i++)
	{
		int Area[MAX_AREA_COUNT][2] = {{-1,},};
		if(m_zones[i].m_bRemote)
			continue;
		else
		{
			if(m_zones[i].IsExpedRaidZone())
			{
				for(j = 0; j < m_zones[i].m_countArea; j++)
				{
					if(m_zones[i].m_area[j].m_bEnable)
					{
						Area[AreaArrayIndex][0] = m_zones[i].m_area[j].m_nRaidRoomNo;
						Area[AreaArrayIndex][1] = m_zones[i].m_area[j].m_RaidDifficulty;
						AreaArrayIndex++;
					}
				}
				// 이제 여기서 메시지를 만들고 Helper로 보낸다.
				// 보내는 정보. 삭제 요청 메시지 << AreaArrayIndex << zoneNo. << roomNo. << diffyculty
				// 주의!!! 여기 정보만 제외하고 다른 것들을 삭제해야한다!!!!
				{
					CNetMsg::SP rmsg(new CNetMsg);
					HelperAutoResetRaid(rmsg, AreaArrayIndex, m_zones[i].m_index, Area);
					SEND_Q(rmsg, gserver->m_helper);
				}
			}
		}
	}
}

#ifdef NEW_DOUBLE_GM_AUTO
void CServer::SwapDoubleEventValue()
{
	int swap;

	swap = m_bDoubleNasPercent;
	m_bDoubleNasPercent = m_bDoubleNasPercentBackup;
	m_bDoubleNasPercentBackup = swap;

	swap = m_bDoubleNasGetPercent;
	m_bDoubleNasGetPercent = m_bDoubleNasGetPercentBackup;
	m_bDoubleNasGetPercentBackup = swap;

	swap = m_bDoubleExpPercent;
	m_bDoubleExpPercent = m_bDoubleExpPercentBackup;
	m_bDoubleExpPercentBackup = swap;

	swap = m_bDoubleSpPercent;
	m_bDoubleSpPercent = m_bDoubleSpPercentBackup;
	m_bDoubleSpPercentBackup = swap;

	swap = m_bDoubleProducePercent;
	m_bDoubleProducePercent = m_bDoubleProducePercentBackup;
	m_bDoubleProducePercentBackup = swap;

	swap = m_bDoubleProduceNum;
	m_bDoubleProduceNum = m_bDoubleProduceNumBackup;
	m_bDoubleProduceNumBackup = swap;
}
#endif

void CServer::CheckEventTime()
{
	time_t curtime;
	curtime = mktime(&m_tRealSystemTime);
#if defined (NEW_DOUBLE_GM_AUTO) || defined (NEW_DOUBLE_EVENT_AUTO_TIME) || defined (EVENT_ITEMDROP_AUTO) || defined (GM_EXP_LIMIT_AUTO)
	bool bBoolean;
	time_t starttime;
	time_t endtime;
	struct tm tStart;
	struct tm tEnd;
#endif

#ifdef DOUBLE_PET_EXP_AUTO
	if(curtime >= gserver->m_tPetExpEventStart && curtime < gserver->m_tPetExpEventEnd)
	{
		if(gserver->m_bDoublePetExpEvent == false)
		{
			GAMELOG << init("PET_EVENT_START") << "prob" << delim << gserver->m_PetExpPercent << end;
		}
		gserver->m_bDoublePetExpEvent = true;
	}
	else
	{
		if(gserver->m_bDoublePetExpEvent == true)
		{
			GAMELOG << init("PET_EVENT_END") << end;
		}
		gserver->m_bDoublePetExpEvent = false;
	}
#endif
#ifdef UPGRADE_EVENT_AUTO
	if(curtime >= gserver->m_tUpgradeEventStart && curtime < gserver->m_tUpgradeEventEnd)
	{
		// 이벤트 시간임.
		if(g_bUpgradeEvent == false)
		{
			GAMELOG << init("UPGRADE_EVENT_START") << "prob" << delim << g_nUpgradeProb << end;
			CNetMsg::SP rmsg(new CNetMsg);
			EventEnchantRate(rmsg, 1, g_nUpgradeProb);
			PCManager::instance()->sendToAll(rmsg);
		}
		g_bUpgradeEvent = true;
	}
	else
	{
		if(g_bUpgradeEvent == true)
		{
			GAMELOG << init("UPGRADE_EVENT_END") << end;
			CNetMsg::SP rmsg(new CNetMsg);
			EventEnchantRate(rmsg, 0, g_nUpgradeProb);
			PCManager::instance()->sendToAll(rmsg);
		}
		g_bUpgradeEvent = false;
	}
#endif // UPGRADE_EVENT_AUTO

#ifdef GM_EXP_LIMIT_AUTO
	memset(&tStart, 0x00, sizeof(tStart));
	memset(&tEnd, 0x00, sizeof(tEnd));

	bBoolean = m_bExpLimit;
	tStart.tm_year = m_nGMExpLimitStart[0] - 1900;
	tStart.tm_mon = m_nGMExpLimitStart[1] - 1;
	tStart.tm_mday = m_nGMExpLimitStart[2];
	tStart.tm_hour = m_nGMExpLimitStart[3];
	tStart.tm_min = m_nGMExpLimitStart[4];
	tStart.tm_sec = m_nGMExpLimitStart[5];
	tStart.tm_isdst = -1;
	starttime = mktime(&tStart);

	tEnd.tm_year = m_nGMExpLimitEnd[0] - 1900;
	tEnd.tm_mon = m_nGMExpLimitEnd[1] - 1;
	tEnd.tm_mday = m_nGMExpLimitEnd[2];
	tEnd.tm_hour = m_nGMExpLimitEnd[3];
	tEnd.tm_min = m_nGMExpLimitEnd[4];
	tEnd.tm_sec = m_nGMExpLimitEnd[5];
	tEnd.tm_isdst = -1;
	endtime = mktime(&tEnd);

	if(curtime >= starttime && curtime < endtime)
	{
		// 이벤트 시간임.
		m_bExpLimit = true;
		if(bBoolean == false)
		{
			m_nExpLimit = m_nExpLimitStart;
			m_nSpLimit = m_nSpLimitStart;
			GAMELOG << init("EXP SP LIMIT SET") << "EXP LIMIT VALUE" << delim << m_nExpLimit << delim << "SP LIMIT VALUE" << delim << m_nSpLimit << end;
		}
	}
	else
	{
		// 이벤트 시간이 아님.
		m_bExpLimit = false;
		if(bBoolean == true)
		{
			m_nExpLimit = m_nExpLimitEnd;
			m_nSpLimit = m_nSpLimitEnd;
			GAMELOG << init("EXP SP LIMIT SET") << "EXP LIMIT VALUE" << delim << m_nExpLimit << delim << "SP LIMIT VALUE" << delim << m_nSpLimit << end;
		}
	}
#endif //GM_EXP_LIMIT_AUTO
#ifdef EVENT_ITEMDROP_AUTO
	memset(&tStart, 0x00, sizeof(tStart));
	memset(&tEnd, 0x00, sizeof(tEnd));

	bBoolean = m_bIsItemdrop;
	tStart.tm_year = m_iEventItemdropStart[0] - 1900;
	tStart.tm_mon = m_iEventItemdropStart[1] - 1;
	tStart.tm_mday = m_iEventItemdropStart[2];
	tStart.tm_hour = m_iEventItemdropStart[3];
	tStart.tm_min = m_iEventItemdropStart[4];
	tStart.tm_sec = m_iEventItemdropStart[5];
	tStart.tm_isdst = -1;
	starttime = mktime(&tStart);

	tEnd.tm_year = m_iEventItemdropEnd[0] - 1900;
	tEnd.tm_mon = m_iEventItemdropEnd[1] - 1;
	tEnd.tm_mday = m_iEventItemdropEnd[2];
	tEnd.tm_hour = m_iEventItemdropEnd[3];
	tEnd.tm_min = m_iEventItemdropEnd[4];
	tEnd.tm_sec = m_iEventItemdropEnd[5];
	tEnd.tm_isdst = -1;
	endtime = mktime(&tEnd);

	if(curtime >= starttime && curtime < endtime)
	{
		// 이벤트 시간임.
		m_bIsItemdrop = true;
		if(bBoolean == false)
		{
			//SwapEventItemdropValue();
			gserver->m_nItemDropEventRate = gserver->m_nItemDropEventRateBackup;
			GAMELOG << init("Event ItemDrop Start") << "RATE" << delim << gserver->m_nItemDropEventRate << end;
		}
	}
	else
	{
		// 이벤트 시간이 아님.
		m_bIsItemdrop = false;
		gserver->m_nItemDropEventRate = 100;
		if(bBoolean == true)
		{
			GAMELOG << init("Event ItemDrop End") << "RATE" << delim << gserver->m_nItemDropEventRate << end;
		}
	}
#endif // EVENT_ITEMDROP_AUTO
#ifdef NEW_DOUBLE_GM_AUTO
	memset(&tStart, 0x00, sizeof(tStart));
	memset(&tEnd, 0x00, sizeof(tEnd));

	bBoolean = m_bIsDEtime;
	tStart.tm_year = m_iDoubleGMStart[0] - 1900;
	tStart.tm_mon = m_iDoubleGMStart[1] - 1;
	tStart.tm_mday = m_iDoubleGMStart[2];
	tStart.tm_hour = m_iDoubleGMStart[3];
	tStart.tm_min = m_iDoubleGMStart[4];
	tStart.tm_sec = m_iDoubleGMStart[5];
	tStart.tm_isdst = -1;
	starttime = mktime(&tStart);

	tEnd.tm_year = m_iDoubleGMEnd[0] - 1900;
	tEnd.tm_mon = m_iDoubleGMEnd[1] - 1;
	tEnd.tm_mday = m_iDoubleGMEnd[2];
	tEnd.tm_hour = m_iDoubleGMEnd[3];
	tEnd.tm_min = m_iDoubleGMEnd[4];
	tEnd.tm_sec = m_iDoubleGMEnd[5];
	tEnd.tm_isdst = -1;
	endtime = mktime(&tEnd);

	if(curtime >= starttime && curtime < endtime)
	{
		// 이벤트 시간
		m_bIsDEtime = true;
		if(bBoolean == false)
		{
			//swap
			SwapDoubleEventValue();
			GAMELOG << init("Double Event Time Start")
					<< "NAS PERCENT : " << m_bDoubleNasPercent
					<< " NAS GET PERCENT : " << m_bDoubleNasGetPercent
					<< " EXP PERCENT : " << m_bDoubleExpPercent
					<< " SP PERCENT : " << m_bDoubleSpPercent
					<< " PRODUCE PERCENT : " << m_bDoubleProducePercent
					<< " PRODUCE NUM : " << m_bDoubleProduceNum << end;
		}
	}
	else
	{
		// 이벤트 시간 아님.
		m_bIsDEtime = false;
		if(bBoolean == true)
		{
			//swap
			SwapDoubleEventValue();

			GAMELOG << init("Double Event Time End")
					<< "NAS PERCENT : " << m_bDoubleNasPercent
					<< " NAS GET PERCENT : " << m_bDoubleNasGetPercent
					<< " EXP PERCENT : " << m_bDoubleExpPercent
					<< " SP PERCENT : " << m_bDoubleSpPercent
					<< " PRODUCE PERCENT : " << m_bDoubleProducePercent
					<< " PRODUCE NUM : " << m_bDoubleProduceNum << end;
		}
	}
#endif
#ifdef NEW_DOUBLE_EVENT_AUTO_TIME
	memset(&tStart, 0x00, sizeof(tStart));
	memset(&tEnd, 0x00, sizeof(tEnd));

	bBoolean = gserver->m_bDoubleEventAuto;
	tStart.tm_year = m_iDoubleEventStart[0] - 1900;
	tStart.tm_mon = m_iDoubleEventStart[1] - 1;
	tStart.tm_mday = m_iDoubleEventStart[2];
	tStart.tm_hour = m_iDoubleEventStart[3];
	tStart.tm_min = m_iDoubleEventStart[4];
	tStart.tm_sec = m_iDoubleEventStart[5];
	tStart.tm_isdst = -1;

	tEnd.tm_year = m_iDoubleEventEnd[0] - 1900;
	tEnd.tm_mon = m_iDoubleEventEnd[1] - 1;
	tEnd.tm_mday = m_iDoubleEventEnd[2];
	tEnd.tm_hour = m_iDoubleEventEnd[3];
	tEnd.tm_min = m_iDoubleEventEnd[4];
	tEnd.tm_sec = m_iDoubleEventEnd[5];
	tEnd.tm_isdst = -1;

	starttime = mktime(&tStart);
	endtime = mktime(&tEnd);

	if(curtime >= starttime && curtime < endtime)
	{
		gserver->m_bDoubleEventAuto = true;
		gserver->NewDoubleEventAuto();
		if (bBoolean == false)
		{
			// 변경되었다는 게임로그
			GAMELOG << init("Combo Double Event Auto Reservation Time") << "Start" << end;
		}
	}
	else
	{
		gserver->m_bDoubleEventAuto = false;
		gserver->m_bDoubleEventAutoOn = false;
		if(bBoolean == true)
		{
			// 변경되었다는 게임로그
			GAMELOG << init("Combo Double Event Auto Reservation Time") << "End" << end;
		}
	}
#endif // NEW_DOUBLE_EVENT_AUTO_TIME
	return ;
}

#ifdef XTRAP
bool CServer::InitXTrap()
{
	FILE* fp = fopen("data/map1.CS3", "rb");
	if( fp == NULL )
	{
		LOG_ERROR("data/map1.CS3 open error.");
		return false;
	}
	fread(m_XTrapMap[0], XTRAP_CS4_BUFSIZE_MAP, 1,fp);
	fclose(fp);

	if( XTrap_S_LoadDll() != XTRAP_API_RETURN_OK )
	{
		LOG_ERROR("XTrap_S_LoadDll() error.");
		return false;
	}

	unsigned int ret = XTrap_S_SetAllowDelay(XTRAP_ALLOW_DELAY_2);
	if( ret != 0)
	{
		LOG_ERROR("XTrap_S_SetAllowDelay(XTRAP_ALLOW_DELAY_2) error.");
		return false;
	}

	XTrap_S_Start( 600, 2, m_XTrapMap, NULL );

	return true;
}
#endif //XTRAP

int CServer::GetCastllanZoneZum(int charIndex)
{
	if( charIndex == CDratanCastle::CreateInstance()->GetOwnerCharIndex() )
	{
		return CDratanCastle::CreateInstance()->GetZoneIndex();
	}
	return -1;
}

bool IsApllyDebuffSkilltoNPC( CPC* aPc, CNPC* tNpc)
{
	if( !tNpc->Check_MobFlag(STATE_MONSTER_MERCENARY) )
		return true;

	if( tNpc->GetOwner() )
	{
		CPC* pTpc =  tNpc->GetOwner();
		if(pTpc == aPc )
			return false;

		if( !(IsRaList(pTpc, aPc) || IsRaList(aPc, pTpc)) )
			return false;
	}

	return true;
}

#ifdef SYSTEM_TREASURE_MAP
void CServer::writeTreasureMapSaveFile()
{
	FILE* fp = fopen( GM_TREASURE_MAP_SAVE_FILE, "wt");
	if(fp)
	{
		fprintf(fp, "%d\n", ( gserver->bTreasureMapLinkZoneDrop? 1 : 0 ) );

		int i;
		for(i=0; i<gserver->m_numZone; i++)
		{
			CZone * pZone = gserver->m_zones+i;
			if( !pZone)
				continue;
			if( !pZone->IsTreasureDropZone() )
				continue;

			fprintf( fp, "%d\n", pZone->m_index );
			fprintf( fp, "%d\n", ( pZone->CheckTreasureDropFlag()? 1 : 0 ) );
		}

		fclose(fp);
		fp = NULL;
	}
}

void CServer::readTreasureMapSaveFile()
{
	FILE* fp = fopen(GM_TREASURE_MAP_SAVE_FILE, "rt");

	if(fp)
	{
		char tmpBuf[MAX_STRING_LENGTH] = {0,};
		if( fgets(tmpBuf, 8, fp) == NULL )
			return;
		gserver->bTreasureMapLinkZoneDrop = (atoi(tmpBuf)? true:false);

		while(1)
		{
			if( fgets(tmpBuf, 8, fp) == NULL )
				return;
			int zone = atoi(tmpBuf);

			if( fgets(tmpBuf, 8, fp) == NULL )
				return;
			int bDrop = atoi(tmpBuf);

			CZone* pZone = gserver->FindZone( zone );
			if (pZone == NULL)
				continue;

			if( !pZone->IsTreasureDropZone() )
				continue;

			if( bDrop )
				pZone->SetTreasureDropFlag();
			else
				pZone->ResetTreasureDropFlag();
		}

		fclose(fp);
		fp = NULL;

		GAMELOG << init("SYSTEM")
				<< "TREASUREMAP SAVE FILE READ SUC"
				<< end;

		return;
	}

	GAMELOG << init("SYSTEM")
			<< "TREASUREMAP SAVE FILE READ FAIL"
			<< end;
}
#endif

#ifdef REFORM_PK_PENALTY_201108 // 정시에 한번 성향 수지 장치 버프가 있는 플레이어에게 pk 성향 포인트 보상 보상해줘야한다.
void CServer::CheckPkDisPosiotionPointReward()
{
	PCManager::map_t& playerMap			= PCManager::instance()->getPlayerMap();
	PCManager::map_t::iterator iter		= playerMap.begin();
	PCManager::map_t::iterator endIter	= playerMap.end();
	for (; iter != endIter; ++iter)
	{
		CPC* pc = (*iter).pPlayer;
		if (pc == NULL)
		{
			continue;
		}

		if (0 < pc->m_assist.m_avAddition.pkDispositionPointValue)
		{
			pc->AddPkPenalty(pc->m_assist.m_avAddition.pkDispositionPointValue);

			CNetMsg::SP rmsg(new CNetMsg);
			CharStatusMsg(rmsg, pc, 0);
			pc->m_pArea->SendToCell(rmsg, pc, false);
		}
	}
}

#endif

void CServer::CheckDratanCastleDungeonReward()
{
	for(int i = 0; i < gserver->m_DratanCheckTime; i++)
	{
		if(m_dratanCastleEnvCheck[i] == false)
		{
			return;
		}
		m_dratanCastleEnvCheck[i] = false;
	}

	CDratanCastle * pCastle = CDratanCastle::CreateInstance();
	ExpressSystemItemInfo* iteminfo = new ExpressSystemItemInfo;
	iteminfo->item_index = 10014;
	iteminfo->item_count = 1;
	iteminfo->send_type = EXPRESS_SEND_TYPE_ETC;
	memcpy(iteminfo->sender, "Dratan", sizeof(iteminfo->sender));
	DBManager::instance()->PushExpressInputItemNotConnectUser(pCastle->GetOwnerCharIndex(), iteminfo, false);
}

#ifdef DEV_EVENT_AUTO
void CEventKingsBirthdayRewardItem::giveRewardItem(CPC* pc, int giveCount)
{
	if (pc == NULL)
		return;

	CItem* pGiveItem = gserver->m_itemProtoList.CreateItem(getItemIndex(), -1, 0, 0, giveCount);
	if(pGiveItem == NULL)
		return;

	if (pc->m_inventory.addItem(pGiveItem) == false)
	{
		GAMELOG << init("NO ADD TO INVENTORY Heart Reward Item You Must Recovery!!", pc) << itemlog(pGiveItem) << end;

		delete pGiveItem;

		return ;
	}
}

void CEventKingsBirthdayFlagReward::giveFlagRewardItem(CPC* pc)
{
	if (pc == NULL)
		return;

	CEventKingsBirthdayRewardItem* rewardItem = getFlagRewardItem(pc->m_job);
	if(!rewardItem)
		return ;

	CItem* pGiveItem = gserver->m_itemProtoList.CreateItem(rewardItem->getItemIndex(), -1, 0, 0, 1);
	if (pGiveItem == NULL)
		return;

	if (pc->m_inventory.addItem(pGiveItem) == false)
	{
		GAMELOG << init("NO ADD TO INVENTORY Flag Reward Item You Must Recovery!!", pc) << itemlog(pGiveItem) << end;

		delete pGiveItem;

		return ;
	}
}
CItem* CEventKingsBirthdayDropItemManager::getItem(int prob)
{
	CItem* pItem = NULL;
	int isProb = 0;
	//for(int i = 0; i < getDropItemCount(); i++)
	//{
	//	isProb = isProb + m_DropItem[i].getDropItemProb();
	//	if(isProb >= prob)
	//	{
	//		// drop
	//		pItem = gserver->m_itemProtoList.CreateItem(m_DropItem[i].getDropItemIndex(), -1, 0, 0, m_DropItem[i].getDropItemCount());
	//		break;
	//	}
	//}
	std::vector<CKingsBirthdayDropItem*>::const_iterator cit;
	std::vector<CKingsBirthdayDropItem*>::const_iterator citEnd = m_DropItem.end();
	CKingsBirthdayDropItem* pDropitem = NULL;
	for(cit = m_DropItem.begin(); cit != citEnd; cit++)
	{
		pDropitem = *cit;
		isProb = isProb + pDropitem->getDropItemProb();
		if(isProb >= prob)
		{
			// drop
			pItem = gserver->m_itemProtoList.CreateItem(pDropitem->getDropItemIndex(), -1, 0, 0, pDropitem->getDropItemCount());
			break;
		}
	}
	return pItem;
}
void CEventKingsBirthdayDropItemManager::deleteDropItem(CPC* pc)
{
	if (pc == NULL)
		return;

	int dropItemCount = getDropItemCount();
	for(int i = 0; i < dropItemCount; i++)
	{
		if (getDropItem(i) == NULL)
			continue;

		if (strcmp(getDropItem(i)->getType(), "Heart") != 0)
			continue;

		CItem* pItem = pc->m_inventory.FindByDBIndex(getDropItem(i)->getDropItemIndex(), 0, 0);
		if (pItem == NULL)
			continue;

		GAMELOG << init("FathersDay Delete Item(Exchange)", pc) << itemlog(pItem) << end;

		pc->m_inventory.decreaseItemCount(pItem, 1);
	}
}
void CEventKingsBirthdayDropItemManager::deleteFlagItem(CPC* pc)
{
	if (pc == NULL)
		return;

	int dropItemCount = getDropItemCount();
	for(int i = 0; i < dropItemCount; i++)
	{
		if (getDropItem(i) == NULL)
			continue;

		if (strcmp(getDropItem(i)->getType(), "Flag") != 0)
			continue;

		CItem* pItem = pc->m_inventory.FindByDBIndex(getDropItem(i)->getDropItemIndex(), 0, 0);
		if (pItem == NULL)
			continue;

		GAMELOG << init("FathersDay Delete Item(Exchange)", pc) << itemlog(pItem) << end;

		pc->m_inventory.decreaseItemCount(pItem, 1);
	}
}

void CEventKingsBirthdayFlagReward::setFlagReward(CConfigFile& eventConfig)
{
	setCount(atoi(eventConfig.Find("TLD_KB_FLAG_REWARDS","Count")));
	if(getCount())
	{
		CLCString grouptag;
		int count = getCount();

		for(int i = 0; i < count; i++)
		{
			CEventKingsBirthdayRewardItem* pReward = new CEventKingsBirthdayRewardItem;
			grouptag.Format("TLD_KB_FLAG_REWARD_%d", i);
			pReward->setItemIndex(atoi(eventConfig.Find(grouptag, "FlagRewardIndex")));
			m_flagReward.push_back(pReward);
		}
	}
}

CEventKingsBirthdayRewardItem* CEventKingsBirthdayFlagReward::getFlagRewardItem(int job)
{
	//if(job < 0 || job >= getCount())
	//	job = 0;
	////if(m_flagReward)
	//return m_flagReward[job];
	if(m_flagReward.size())
	{
		if(job < 0 || job >= getCount())
			job = 0;
		std::vector<CEventKingsBirthdayRewardItem*>::const_iterator cit;
		cit = m_flagReward.begin();
		cit = cit + job;
		return *cit;
	}
	else
		return NULL;
}

void CEventKingsBirthdayDropItemManager::setDropItem(CConfigFile& eventConfig)
{
	setDropItemCount(atoi(eventConfig.Find("TLD_KB_DROP_ITEMS", "Count")));
	if(getDropItemCount())
	{
		//m_DropItem = new CKingsBirthdayDropItem[getDropItemCount()];
		CLCString grouptag;
		int dropItemCount = getDropItemCount();
		for(int i = 0; i < dropItemCount; i++)
		{
			CKingsBirthdayDropItem* pDropItem = new CKingsBirthdayDropItem;
			grouptag.Format("TLD_KB_DROP_ITEM_%d", i);
			pDropItem->setDropItemIndex(atoi(eventConfig.Find(grouptag, "Index")));
			pDropItem->setDropCount(atoi(eventConfig.Find(grouptag, "Count")));
			pDropItem->setDropProb(atoi(eventConfig.Find(grouptag, "Prob")));
			pDropItem->setPoint(atoi(eventConfig.Find(grouptag, "Point")));
			pDropItem->setType(eventConfig.Find(grouptag, "Type"));
			m_DropItem.push_back(pDropItem);
			//m_DropItem[i].setDropItemIndex(atoi(eventConfig.Find(grouptag, "Index")));
			//m_DropItem[i].setDropCount(atoi(eventConfig.Find(grouptag, "Count")));
			//m_DropItem[i].setDropProb(atoi(eventConfig.Find(grouptag, "Prob")));
			//m_DropItem[i].setPoint(atoi(eventConfig.Find(grouptag, "Point")));
			//m_DropItem[i].setType(eventConfig.Find(grouptag, "Type"));
		}
	}
}

CKingsBirthdayDropItem* CEventKingsBirthdayDropItemManager::getDropItem(int arrayIndex)
{
//	if(arrayIndex < 0 || arrayIndex >= getDropItemCount())
//		arrayIndex = 0;
////		if(m_DropItem)
//	return m_DropItem[arrayIndex];

	if(arrayIndex < 0 || arrayIndex >= getDropItemCount())
		return NULL;

	if(m_DropItem.size())
	{
		std::vector<CKingsBirthdayDropItem*>::const_iterator cit;
		cit = m_DropItem.begin();
		cit = cit + arrayIndex;
		return *cit;
	}
	else
		return NULL;
}

int CEventKingsBirthdayDropItemManager::calcPoint(CPC* pc)
{
	if (pc == NULL)
		return 0;

	int allPoint=0;
	int dropItemCount = getDropItemCount();
	for(int i = 0; i < dropItemCount; i++)
	{
		if (getDropItem(i) == NULL)
			continue;

		if (strcmp(getDropItem(i)->getType(), "Heart") != 0)
			continue;

		item_search_t vec;
		int sc = pc->m_inventory.searchItemByCondition(getDropItem(i)->getDropItemIndex(), 0, 0, vec);
		if (sc == 0)
			continue;

		allPoint += sc * getDropItem(i)->getPoint();
	}

	return allPoint;
}

void CEventKingsBirthdayDropProbTable::setProbTable(CConfigFile& eventConfig)
{
	setCount(atoi(eventConfig.Find("TLD_KB_DROP_PROBS","Count")));
	CLCString grouptag;
	if(getCount())
	{
		//m_levelSection = new CLevelSection[getCount()];
		int count = getCount();
		for(int i = 0; i < count; i++)
		{
			CLevelSection* plevel = new CLevelSection;
			grouptag.Format("TLD_KB_DROP_PROB_%d", i);
			plevel->setMinLevel(atoi(eventConfig.Find(grouptag, "Min")));
			plevel->setMaxLevel(atoi(eventConfig.Find(grouptag, "Max")));
			plevel->setProb(atoi(eventConfig.Find(grouptag, "Prob")));
			m_levelSection.push_back(plevel);
		}
	}
	setFirstPriority(atoi(eventConfig.Find("TLD_KB_DROP_PROB_MULTIPLICATION","MBOSS_BOSS_RAID_NPC")));
	setSecondPriority(atoi(eventConfig.Find("TLD_KB_DROP_PROB_MULTIPLICATION","PARTY_NPC")));
}

int CEventKingsBirthdayDropProbTable::getProb(CNPC* npc)
{
	int prob = 0;
	//for(int i = 0; i < getCount(); i++)
	//{
	//	if(m_levelSection[i].getMinLevel() <= npc->m_level && m_levelSection[i].getMaxLevel() >= npc->m_level)
	//	{
	//		prob = m_levelSection[i].getProb();
	//		break;
	//	}
	//}
	std::vector<CLevelSection*>::const_iterator cit;
	std::vector<CLevelSection*>::const_iterator citEnd = m_levelSection.end();
	CLevelSection* plevel = NULL;
	for(cit = m_levelSection.begin(); cit != citEnd; cit++)
	{
		plevel = *cit;
		if(plevel->getMinLevel() <= npc->m_level && plevel->getMaxLevel() >= npc->m_level)
		{
			prob = plevel->getProb();
			break;
		}
	}

	if(npc->m_proto->CheckFlag(NPC_MBOSS | NPC_BOSS | NPC_RAID))
	{
		prob = prob * m_firstPriority;
	}
	else if(npc->m_proto->CheckFlag(NPC_PARTY))
	{
		prob = prob * m_secondPriority;
	}

	return prob;
}

void CEventKingsBirthday::Load(CConfigFile& eventConfig)
{
	LoadDropItemManager(eventConfig);
	LoadHeartItem(eventConfig);
	LoadFlagItem(eventConfig);
	LoadItemDropProbTable(eventConfig);
	if(atoi(eventConfig.Find("TLD_EVENT_ENABLE","Enable")) == 1)
	{
		setEventStart(true);
	}
	else
	{
		setEventStart(false);
	}
	if(atoi(eventConfig.Find("TLD_EVENT_ENABLE","EventItemDelete")) == 1)
	{
		setEventDeleteItem(true);
	}
	else
	{
		setEventDeleteItem(false);
	}
}

void CItemDropEvent::Load(CConfigFile& eventConfig)
{
	LoadItemDropManager(eventConfig);
	LoadDropProbTable(eventConfig);
	if(atoi(eventConfig.Find("KOKO_EVENT","Enable")) == 1)
	{
		setEnable(true);
	}
	else
	{
		setEnable(false);
	}
}

void CItemDropEvent::LoadItemDropManager(CConfigFile& eventConfig)
{
	m_DropItemManager.Load(eventConfig);
}
void CItemDropEvent::LoadDropProbTable(CConfigFile& eventConfig)
{
	m_DropProbTable.Load(eventConfig);
}
void CDropProbTable::Load(CConfigFile& eventConfig)
{
	CLCString grouptag;
	setLevelSectionCount(atoi(eventConfig.Find("KOKO_DROP_PROBS", "Count")));
	if(getLevelSectionCount())
	{
		int levelSectionCount = getLevelSectionCount();
		for(int i = 0; i < levelSectionCount; i++)
		{
			grouptag.Format("KOKO_DROP_PROB_%d", i);
			CLevelSection* pLevel = new CLevelSection;
			pLevel->setMinLevel(atoi(eventConfig.Find(grouptag,"Min")));
			pLevel->setMaxLevel(atoi(eventConfig.Find(grouptag,"Max")));
			pLevel->setProb(atoi(eventConfig.Find(grouptag, "Prob")));
			m_levelSection.push_back(pLevel);
		}
		setUnderLevel(atoi(eventConfig.Find("KOKO_DROP_PROB_MULTIPLICATION","UNDER_LEVEL")));
		setUpperLevel(atoi(eventConfig.Find("KOKO_DROP_PROB_MULTIPLICATION","UP_LEVEL")));
		setDivision(atoi(eventConfig.Find("KOKO_DROP_PROB_MULTIPLICATION","DIVISION")));
		setMultiple(atoi(eventConfig.Find("KOKO_DROP_PROB_MULTIPLICATION","MULTIPLE")));
		//setFirstPriority(atoi(eventConfig.Find("KOKO_DROP_PROB_MULTIPLICATION","MBOSS_BOSS_RAID_NPC")));
		//setSecondPriority(atoi(eventConfig.Find("KOKO_DROP_PROB_MULTIPLICATION","PARTY_NPC")));
	}

	setZoneCount(atoi(eventConfig.Find("ZONES_MULTIPLE", "Count")));
	if(getZoneCount())
	{
		int zoneCount = getZoneCount();;
		for(int i = 0; i < zoneCount; i++)
		{
			grouptag.Format("ZONE_%d",i);
			CZoneDropProb* pDrop = new CZoneDropProb;
			pDrop->setZoneIndex(atoi(eventConfig.Find(grouptag, "Zindex")));
			pDrop->setMultiple(atoi(eventConfig.Find(grouptag, "Multiple")));
			m_zoneDropProb.push_back(pDrop);
		}
	}
}
void CDropItemManager::Load(CConfigFile& eventConfig)
{
	CLCString grouptag;
	setCount(atoi(eventConfig.Find("KOKO_DROP_ITEMS", "Count")));
	if(getCount())
	{
		int count = getCount();
		for(int i = 0; i < count; i++)
		{
			grouptag.Format("KOKO_DROP_ITEM_%d", i);
			CDropItem* pDropItem = new CDropItem;
			pDropItem->setItemIndex(atoi(eventConfig.Find(grouptag, "Index")));
			pDropItem->setDropCount(atoi(eventConfig.Find(grouptag, "Count")));
			m_DropItem.push_back(pDropItem);
		}
	}
}

int CDropProbTable::getProb(CPC* pc, CNPC* npc)
{
	int prob = 0;
	std::vector<CLevelSection*>::const_iterator cit;
	std::vector<CLevelSection*>::const_iterator citEnd = m_levelSection.end();
	CLevelSection* plevel = NULL;
	for(cit = m_levelSection.begin(); cit != citEnd; cit++)
	{
		plevel = *cit;
		if(plevel->getMinLevel() <= npc->m_level && plevel->getMaxLevel() >= npc->m_level)
		{
			prob = plevel->getProb();
			break;
		}
	}

	if((pc->m_level - npc->m_level) >= getUnderLevel())
	{
		prob = prob / getDivision();
	}
	else if((npc->m_level - pc->m_level) >= getUpperLevel())
	{
		prob = prob * getMultiple();
	}
	if(m_zoneDropProb.size())
	{
		CZoneDropProb* pProb;
		std::vector<CZoneDropProb*>::const_iterator cit;
		std::vector<CZoneDropProb*>::const_iterator citEnd = m_zoneDropProb.end();
		for(cit = m_zoneDropProb.begin(); cit != citEnd; cit++)
		{
			pProb = *cit;
			if(pc->m_pZone->m_index == pProb->getZoneIndex())
			{
				prob = prob * pProb->getMultiple();
				break;
			}
		}
	}

	//if(npc->m_proto->CheckFlag(NPC_MBOSS | NPC_BOSS | NPC_RAID))
	//{ prob = prob * m_firstPriority; }
	//else if(npc->m_proto->CheckFlag(NPC_PARTY))
	//{ prob = prob * m_secondPriority; }

	return prob;
}
CItem* CDropItemManager::getItem()
{
	CItem* pItem = NULL;
	if(m_DropItem.size())
	{
		CDropItem* pDropItem = NULL;
		std::vector<CDropItem*>::const_iterator cit;
		cit = m_DropItem.begin();
		pDropItem = *cit;
		pItem = gserver->m_itemProtoList.CreateItem(pDropItem->getIetmIndex(), -1, 0, 0, pDropItem->getDropCount());
	}
	return pItem;
}
#endif

void CServer::addNotice(int _pos, int _index)
{
	if( _pos < 0 || _pos >= 5 )
	{
		GAMELOG << init("NOTICE ADD OVER" ) << "count : " << _pos << delim << "index : " << _index << end;
		return;
	}

	if( m_aNotice[_pos] != 0 )
		return;

	m_aNotice[_pos] = _index;
}

bool CServer::removeActiveEvent(int _eventIndex, bool _dbUpdate)
{
	if( !m_eventAutomationMgr.removeActiveEvent(_eventIndex, _dbUpdate) )
		return false;

	//이벤트가 유물 이벤트 이면 해당 아이템을 모두 제거
	if( _eventIndex == EVENT_ARTIFACT)
	{
		ArtifactManager::instance()->eventOff();
	}

	// 모든존을 검사해서 해당 npc를 삭제할까? ㅡㅡ;;
	CEventInfo * eInfo = getEventInfo(_eventIndex);
	if( !eInfo )
		return false;

	CNPC *npc = NULL;
	int i=0,j=0;
	for (i = 0; i < m_numZone; i++)
	{
#if defined( SYSTEM_TREASURE_MAP )
		if( !m_zones[i].IsFieldZone() )
			continue;
#endif //SYSTEM_TREASURE_MAP

		for (j=0; j< m_zones[i].m_countArea; j++)
		{
			CArea * pArea = &m_zones[i].m_area[j];

			if( !pArea->m_bEnable )
				continue;

			CNPC* next;
			next = pArea->m_npcList;
			while((npc = next))
			{
				next = npc->m_pNPCNext;

				if( eInfo->isEventNpc( npc->m_idNum ) )
				{
					GAMELOG << init("Event NPC Delete")
							<< "EVENT INDEX : " << _eventIndex << delim
							<< "NPC DB INDEX : " << npc->m_idNum << end;
					npc->SendDisappearAllInCell(true);
					DelAttackList(npc);
					pArea->CharFromCell(npc, true);
					pArea->DelNPC(npc);
				}
			}
		}
	}
	return true;
}

void CServer::doEventDropItem(CNPC* npc, CPC* pc, CPC* tpc)
{
	if( tpc == NULL || pc == NULL || npc == NULL )
		return;

	CPC * pPc = pc;
	if( pc != tpc )
		pPc = tpc;

	if( pPc->m_pZone->IsPersonalDungeon() )
		return;

	if( m_eventAutomationMgr.getActiveEventCount() == 0 )
		return;

	std::map<int, CEventInfo*> * activeList = m_eventAutomationMgr.getActiveEvenList();
	std::map<int, CEventInfo*>::iterator itr;
	std::map<int, CEventInfo*>::iterator itrEnd = activeList->end();

	for(itr=activeList->begin(); itr!=itrEnd; itr++)
	{
		if( itr->second->getDropCount() == 0 )
			continue;

		int i=0;
		int dropCount = itr->second->getDropCount();
		for(i=0; i<dropCount; i++)
			m_dropItemMgr.doDrop( pPc, npc, itr->second->getDropIndex(i) );
	}
}

bool CServer::GetOutDratanDungeon(CPC* pc)
{
	// 공성전 진행 중이고, 이동하려는 곳이 드라탄 보상 던전(테오의 무덤)이면, 드라탄으로 보낸다.
	// Return Value
	// true : 보냈음, false : 안보냈음
	if ( CDratanCastle::CreateInstance()->GetState() != WCSF_NORMAL )
	{
		if ( pc->m_pZone->m_index == ZONE_DRATAN_CASTLE_DUNGEON )
		{
			int extra = 0;
			CZone* pZone = gserver->FindZone(ZONE_DRATAN);
			if (pZone == NULL)
				return false;

			pc->m_pZone = pZone;
			pc->m_pArea = pZone->m_area;

			GET_YLAYER(pc)	= pZone->m_zonePos[extra][0];
			GET_R(pc)		= 0.0f;
			GET_X(pc)		= GetRandom(pZone->m_zonePos[extra][1], pZone->m_zonePos[extra][3]) / 2.0f;
			GET_Z(pc)		= GetRandom(pZone->m_zonePos[extra][2], pZone->m_zonePos[extra][4]) / 2.0f;

			{
				CNetMsg::SP rmsg(new CNetMsg);
				ResponseClient::makeGoMsg(rmsg, pc);
				SEND_Q(rmsg, pc->m_desc);
			}

			return true;
		}
	}

	return false;
}

void CServer::ClearMobAISeq(void)
{
	m_mobAISeq = 0;
	memset(&m_mobAIArea, 0, sizeof(int) * PASSES_PER_SEC);
	memset(&m_mobAIMobCnt, 0, sizeof(int) * PASSES_PER_SEC);
}

int CServer::AddMobAISeq(int regenCnt)
{
	int min = 2147483647, minseq = 0;
	int i;

	for (i = 0; i < PASSES_PER_SEC; i++)
	{
		if (m_mobAIMobCnt[i] < min)
		{
			min = m_mobAIMobCnt[i];
			minseq = i;
		}
	}

	m_mobAIArea[minseq]++;
	m_mobAIMobCnt[minseq] += regenCnt;

	if (m_mobAIMobCnt[minseq] < 0)
		m_mobAIMobCnt[minseq] = 2147483647;

	//m_mobAIArea[minseq]++;
	//m_mobAIMobCnt[minseq] = 2147483647;

	return minseq;
}

void CServer::RemoveMobAISeq(int minseq, int regenCnt)
{
	if (minseq < 0 || minseq >= PASSES_PER_SEC)
		return;

	m_mobAIArea[minseq]--;
	m_mobAIMobCnt[minseq]-= regenCnt;

	if(m_mobAIMobCnt[minseq] < 0)
		m_mobAIMobCnt[minseq] = 0;
}

void CServer::operate( rnSocketIOService* service )
{
	CDescriptor* newd = new CDescriptor(service);
	newd->m_seq_index = ++local_seq_index;
	newd->m_connect_time = gserver->m_nowseconds;
	service->SetUserData(newd);
	service->setCrypt();

	rnSocketIOServiceTcp* pTcp = reinterpret_cast<rnSocketIOServiceTcp*>(service);
	pTcp->setCheckSequence();

	STATE(newd) = CON_GET_LOGIN;

	newd->m_index = local_dummy_user_index;

	--local_dummy_user_index;
	if (local_dummy_user_index > 0)
		local_dummy_user_index = 0;
}

void CServer::NpcCtTimeCount()
{
	std::map<int, CNPC*>::iterator it = gserver->m_npc_ctTime.begin();
	std::map<int, CNPC*>::iterator endit = gserver->m_npc_ctTime.end();

	for(; it != endit; ++it)
	{
		if( (it->second)->m_ctTime > 0 )
		{
			--it->second->m_ctTime;
		}
		else
		{
			--it->second->m_ctTime = IMMUN_SKILL_MCT_TIME;
			if( (it->second)->m_ctCount > 0 )
				(it->second)->m_ctCount--;
			else
			{
				gserver->m_npc_ctTime_erase.push_back(it->first);
			}
		}
	}

	std::vector<int>::iterator it_e = gserver->m_npc_ctTime_erase.begin();
	std::vector<int>::iterator endit_e = gserver->m_npc_ctTime_erase.end();

	for(; it_e != endit_e; ++it_e)
	{
		gserver->m_npc_ctTime.erase(*it_e);
	}
	gserver->m_npc_ctTime_erase.clear();
}

void CServer::CheckDratanCastleDungeon(int num)
{
	CDratanCastle * pCastle = CDratanCastle::CreateInstance();
	if(pCastle->m_dvd.GetOwnerMode() == true)
	{
		if( pCastle->m_dvd.GetEnvRate() >= 90 && pCastle->m_dvd.GetMobRate() >= 90 )
		{
			m_dratanCastleEnvCheck[num] = true;
		}
		else
		{
			m_dratanCastleEnvCheck[num] = false;
		}
	}
	else
	{
		m_dratanCastleEnvCheck[num] = false;
	}
}
#ifdef TLD_EVENT_SONG
void CServer::checkTldEvent()
{
	time_t nowtime = ::time(0);
	struct tm ttm;
	memcpy(&ttm, localtime(&nowtime), sizeof(ttm));

	switch (ttm.tm_hour)
	{
	case 16:
	case 17:
	case 20:
	case 21:
		tld_event = true;
		break;

	default:
		tld_event = false;
		break;
	}
}
#endif
