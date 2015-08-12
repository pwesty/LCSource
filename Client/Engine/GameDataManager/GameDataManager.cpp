#include "StdH.h"
#include "GameDataManager.h"
#include <Engine/LocalDefine.h>
#include <Engine/Util.h>
#include <Engine/Help/Util_Help.h>
#include <Engine/Contents/Base/ExpressSystem.h>
#include <Engine/Contents/Base/Auction.h>
#include <Engine/Contents/Base/Calendar.h>
#include <Engine/Contents/Base/PetStash.h>
#include <Engine/Contents/Login/ServerSelect.h>
#include <Engine/Contents/Login/CharacterSelect.h>
#include <Engine/Contents/Login/LoginNew.h>
#include <Engine/Contents/Login/CharacterCreateNew.h>
#include <Engine/Contents/Base/Syndicate.h>
#include <Engine/Contents/Base/Durability.h>
#include <Engine/Contents/Base/Notice.h>
#include <Engine/Contents/function/gps.h>
#include <Engine/Contents/Base/Quest.h>
#include <Engine/Contents/Base/ChangeWeapon.h>
#include <Engine/Contents/Base/Party.h>
#include <Engine/Contents/function/attendance.h>
#include <Engine/Contents/function/ItemCollection.h>
#include <Engine/Contents/function/PremiumChar.h>
#include <Engine/Contents/function/News.h>
#include <Engine/Contents/function/ItemCompose.h>

GameDataManager::GameDataManager()
: m_apUIExpressSystem (NULL) 
, m_pAuction(NULL)
, m_pCalendar(NULL)
, m_pPetStash(NULL)
, m_pServerSelect(NULL)
, m_pCharacterSelect(NULL)
, m_pLogin(NULL)
, m_pCharCreate(NULL)
, m_pSyndicate(NULL)
, m_pDurability(NULL)
, m_pNotice(NULL)
, m_pGPS(NULL)
, m_pQuest(NULL)
, m_pChangeWeapon(NULL)
, m_pParty(NULL)
, m_pAttendance(NULL)
, m_pItemCollection(NULL)
, m_pPremiumChar(NULL)
, m_pNews(NULL)
, m_pItemCompose(NULL)
, m_bCreateData(false)
{
	if (m_bCreateData == false)
		Create();
}

GameDataManager::~GameDataManager()
{
	DestroyAll();
}

void GameDataManager::DestroyAll()
{
	SAFE_DELETE(m_apUIExpressSystem);
	SAFE_DELETE(m_pAuction);
	SAFE_DELETE(m_pCalendar);
	SAFE_DELETE(m_pPetStash);
	SAFE_DELETE(m_pServerSelect);
	SAFE_DELETE(m_pCharacterSelect);
	SAFE_DELETE(m_pLogin);
	SAFE_DELETE(m_pCharCreate);
	SAFE_DELETE(m_pSyndicate);
	SAFE_DELETE(m_pDurability);
	SAFE_DELETE(m_pNotice);
	SAFE_DELETE(m_pGPS);
	SAFE_DELETE(m_pQuest);
	SAFE_DELETE(m_pChangeWeapon);
	SAFE_DELETE(m_pParty);
	SAFE_DELETE(m_pAttendance);
	SAFE_DELETE(m_pItemCollection);
	SAFE_DELETE(m_pPremiumChar);
	SAFE_DELETE(m_pNews);
	SAFE_DELETE(m_pItemCompose);
	UtilHelp::destroy();
}

void GameDataManager::Create()
{
	if (m_bCreateData == true)
		return;

	if(m_apUIExpressSystem == NULL)
		m_apUIExpressSystem = new ExpressSystem;
	
	if (m_pAuction == NULL)
		m_pAuction = new Auction;

	if( m_pCalendar == NULL )
		m_pCalendar = new Calendar;

	if ( m_pPetStash == NULL )
		m_pPetStash = new PetStash;

	if (m_pServerSelect == NULL)
		m_pServerSelect = new CServerSelect;

	if (m_pCharacterSelect == NULL)
		m_pCharacterSelect = new CCharacterSelect;

	if ( m_pLogin == NULL )
		m_pLogin = new LoginNew;

	if ( m_pCharCreate == NULL )
		m_pCharCreate = new CharacterCreateNew;

	if (m_pSyndicate == NULL)
		m_pSyndicate = new CSyndicate;

	if (m_pDurability == NULL)
		m_pDurability = new CDurability;

	if (m_pNotice == NULL)
		m_pNotice = new Notice;

	if (m_pGPS == NULL)
		m_pGPS = new GPS;

	if (m_pQuest == NULL)
		m_pQuest = new Quest;

	if (m_pChangeWeapon == NULL)
		m_pChangeWeapon = new ChangeWeapon;

	if (m_pParty == NULL)
		m_pParty = new Party;

	if (m_pAttendance == NULL)
		m_pAttendance = new Attendance;

	if (m_pItemCollection == NULL)
		m_pItemCollection = new CItemCollection;

	if (m_pPremiumChar == NULL)
		m_pPremiumChar = new CPremiumChar;

	if (m_pNews == NULL)
		m_pNews = new CNews;

	if (m_pItemCompose == NULL)
		m_pItemCompose = new CItemCompose;

	m_bCreateData = true;
}
