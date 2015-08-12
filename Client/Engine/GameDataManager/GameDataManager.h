\
#ifndef		_MANAGER_DCLASS_H_
#define		_MANAGER_DCLASS_H_

#include <Engine/DesignClasses/Singleton/SingletonBase.h>

class ExpressSystem;
class Auction;
class Calendar;
class PetStash;
class CServerSelect;
class CCharacterSelect;
class LoginNew;
class CharacterCreateNew;
class CSyndicate;
class CDurability;
class Notice;
class GPS;
class Quest;
class ChangeWeapon;
class Party;
class Attendance;
class CItemCollection;
class CPremiumChar;
class CNews;
class CItemCompose;

class ENGINE_API GameDataManager : public CSingletonBase< GameDataManager >
{
public:
	GameDataManager();
	~GameDataManager();

	void			Create();
	void			DestroyAll();
	ExpressSystem*	GetExpressData()	{ return m_apUIExpressSystem;	}
	Auction*		GetAuction()		{ return m_pAuction;			}
	Calendar*		GetCalendarData()	{ return m_pCalendar;			}
	PetStash*		GetStashData()		{ return m_pPetStash;			}
	CServerSelect*	GetServerData()		{ return m_pServerSelect;		}
	CCharacterSelect* GetCharSelData()	{ return m_pCharacterSelect;	}
	LoginNew*		GetLoginData()		{ return m_pLogin;				}
	CharacterCreateNew* GetCharCreate()	{ return m_pCharCreate;			}
	CSyndicate*		GetSyndicate()		{ return m_pSyndicate;			}
	CDurability*	GetDurability()		{ return m_pDurability;			}
	Notice*			GetNotice()			{ return m_pNotice;				}
	GPS*			GetGPS()			{ return m_pGPS;				}
	Quest*			GetQuest()			{ return m_pQuest;				}
	ChangeWeapon*	GetChangeWeapon()	{ return m_pChangeWeapon;		}
	Party*			GetPartyInfo()		{ return m_pParty;				}
	Attendance*		GetAttendance()		{ return m_pAttendance;			}
	CItemCollection* GetItemCollection(){ return m_pItemCollection;		}
	CPremiumChar*	GetPremiumChar()	{ return m_pPremiumChar;		}
	CNews*			GetNews()			{ return m_pNews;				}
	CItemCompose*	GetItemCompose()	{ return m_pItemCompose;		}
protected:
	bool			m_bCreateData;
	ExpressSystem*  m_apUIExpressSystem;
	Auction*		m_pAuction;
	Calendar*		m_pCalendar;
	PetStash*		m_pPetStash;
	CServerSelect*	m_pServerSelect;
	CCharacterSelect* m_pCharacterSelect;
	LoginNew*		m_pLogin;
	CharacterCreateNew* m_pCharCreate;
	CSyndicate*		m_pSyndicate;
	CDurability*	m_pDurability;
	Notice*			m_pNotice;
	GPS*			m_pGPS;
	Quest*			m_pQuest;
	ChangeWeapon*	m_pChangeWeapon;
	Party*			m_pParty;
	Attendance*		m_pAttendance;
	CItemCollection* m_pItemCollection;
	CPremiumChar*	m_pPremiumChar;
	CNews*			m_pNews;
	CItemCompose*	m_pItemCompose;
};

#define GAMEDATAMGR() GameDataManager::getSingleton()

#endif		// _MANAGER_DCLASS_H_