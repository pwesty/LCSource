#include "stdh.h"
#include <Engine/LoginJobInfo.h>
#include <Engine/GlobalDefinition.h>
#include <Engine/Entities/ItemData.h>
#include <Engine/Util.h>

#define PLAYER_LOGIN_TITAN ("Data\\Character\\Titan\\Login\\ti_Login.smc")
#define PLAYER_LOGIN_MAGE ("Data\\Character\\Mage\\Login\\ma_Login.smc")
#define PLAYER_LOGIN_HEALER ("Data\\Character\\Healer\\Login\\hw_Login.smc")
#define PLAYER_LOGIN_KNIGHT ("Data\\Character\\Knight\\Login\\ni_Login.smc")
#define PLAYER_LOGIN_ROGUE ("Data\\Character\\Rogue\\Login\\ro_Login.smc")
#define PLAYER_LOGIN_SORCERER ("Data\\Character\\Sorcerer\\Login\\so_Login.smc")
#define PLAYER_LOGIN_NIGHTSHADOW ("Data\\Character\\NightShadow\\Login\\ns_Login.smc")

#ifdef CHAR_EX_ROGUE
	#define PLAYER_LOGIN_EX_ROGUE ("Data\\Character\\Rogue_EX\\Login\\ro_ex_Login.smc")	// [2012/08/27 : Sora] EX로그 추가
#endif
#ifdef CHAR_EX_MAGE
	#define PLAYER_LOGIN_EX_MAGE ("Data\\Character\\Mage_Arc\\Login\\ma_arc_Login.smc") // 2013/01/08 jeil EX메이지 추가 
#endif

// ----------------------------------------------------------------------------
// Name : CJobInfo()
// Desc :
// ----------------------------------------------------------------------------
CLoginJobInfo::CLoginJobInfo() : 
m_pLoginJobInfo( NULL )
{
	m_pLoginJobInfo				= new sLoginJobInfo[TOTAL_JOB];
	InitLoginCharacterInfo();
}

// ----------------------------------------------------------------------------
// Name : ~CJobInfo()
// Desc :
// ----------------------------------------------------------------------------
CLoginJobInfo::~CLoginJobInfo()
{
	LoginModelDelete();

	if( m_pLoginJobInfo )
		delete [] m_pLoginJobInfo;
}

// ----------------------------------------------------------------------------
// Name : GetFileName()
// Desc :
// ----------------------------------------------------------------------------
CTString CLoginJobInfo::GetFileName( eJob Job )
{
	return m_pLoginJobInfo[Job].strFileName;
}

void CLoginJobInfo::InitLoginCharacterInfo()
{
	m_pLoginJobInfo[TITAN].strFileName				= PLAYER_LOGIN_TITAN;
	//-------------------------------------------------------
	m_pLoginJobInfo[KNIGHT].strFileName				= PLAYER_LOGIN_KNIGHT;
	//-------------------------------------------------------
	m_pLoginJobInfo[HEALER].strFileName				= PLAYER_LOGIN_HEALER;
	//-------------------------------------------------------
	m_pLoginJobInfo[MAGE].strFileName				= PLAYER_LOGIN_MAGE;
	//-------------------------------------------------------
	m_pLoginJobInfo[ROGUE].strFileName				= PLAYER_LOGIN_ROGUE;
	//-------------------------------------------------------		
	m_pLoginJobInfo[SORCERER].strFileName			= PLAYER_LOGIN_SORCERER;
	//-------------------------------------------------------
	m_pLoginJobInfo[NIGHTSHADOW].strFileName		= PLAYER_LOGIN_NIGHTSHADOW;
#ifdef CHAR_EX_ROGUE
	//-------------------------------------------------------
	m_pLoginJobInfo[EX_ROGUE].strFileName			= PLAYER_LOGIN_EX_ROGUE;
	//-------------------------------------------------------		
#endif

#ifdef CHAR_EX_MAGE	
	//-------------------------------------------------------
	m_pLoginJobInfo[EX_MAGE].strFileName			= PLAYER_LOGIN_EX_MAGE;
#endif
}

CEntity* CLoginJobInfo::GetLoginModel( eJob Job )
{
	return m_pLoginJobInfo[Job].pLogin_MI;
}

void CLoginJobInfo::InitLoginModelInit()
{
	for (int i = 0; i < TOTAL_JOB; i++)
	{
		if (m_pLoginJobInfo[i].pLogin_MI != NULL)
			continue;

		m_pLoginJobInfo[i].pLogin_MI = new CEntity;

		m_pLoginJobInfo[i].pLogin_MI->en_EntityUseType = CEntity::EU_DUMMY;
		m_pLoginJobInfo[i].pLogin_MI->InitAsSkaModel();

		m_pLoginJobInfo[i].pLogin_MI->en_pmiModelInstance = CreateModelInstance("");
		m_pLoginJobInfo[i].pLogin_MI->en_pmiModelInstance->mi_bDummyModel = TRUE;
	}
}

void CLoginJobInfo::LoginModelCreate()
{
	InitLoginModelInit();

	for (int i = 0; i < TOTAL_JOB; i++)
	{
		m_pLoginJobInfo[i].pLogin_MI->SetSkaModel(m_pLoginJobInfo[i].strFileName);
	}
}

void CLoginJobInfo::LoginModelDelete()
{
	for (int i = 0; i < TOTAL_JOB; i++)
	{
		if (m_pLoginJobInfo[i].pLogin_MI != NULL)
		{
			if (m_pLoginJobInfo[i].pLogin_MI->GetModelInstance() != NULL)
				m_pLoginJobInfo[i].pLogin_MI->GetModelInstance()->DeleteAllChildren();

			m_pLoginJobInfo[i].pLogin_MI->End();
		}
		SAFE_DELETE(m_pLoginJobInfo[i].pLogin_MI);
	}
}
