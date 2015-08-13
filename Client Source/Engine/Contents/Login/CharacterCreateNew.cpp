#include "StdH.h"
#include <Engine/Interface/UIManager.h>
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Entities/InternalClasses.h>
#include "CharacterCreateNew.h"
#include <Engine/Network/CNetwork.h>
#include <Engine/Network/Server.h>
#include <Engine/Templates/StaticArray.h>
#include <Engine/GameDataManager/GameDataManager.h>
#include <Engine/GameState.h>

CharacterCreateNew::CharacterCreateNew(void)
	:	m_nSelectJob(0)
	,	m_nSelPrevJob(0)
	,	m_nSelNextJob(0)
	,	m_nSelectHair(0)
	,	m_nSelectFace(0)
	,	m_nSelectArmor(0)
	,	m_nSelectSocial(0)
	,	m_nSocialShowLastIdx(DEF_BTN_SHOW_MAX)
	,	m_charName(CTString(""))
	,	m_bNSCreate(FALSE)
{
}

CharacterCreateNew::~CharacterCreateNew(void)
{
}

void CharacterCreateNew::SetJob( int nSelIdx )
{
	if (nSelIdx < 0 )
		nSelIdx = TOTAL_JOB - 1;
		
	if (nSelIdx >= TOTAL_JOB)
		nSelIdx = 0;

	m_nSelectJob = nSelIdx;
	m_nSelPrevJob = nSelIdx - 1;
	m_nSelNextJob = nSelIdx + 1;

	if (m_nSelPrevJob < 0)
		m_nSelPrevJob = TOTAL_JOB - 1;

	if (m_nSelNextJob >= TOTAL_JOB)
		m_nSelNextJob = 0;
}

void CharacterCreateNew::Reset()
{
	m_nSelectJob = 0;
	m_nSelPrevJob = TOTAL_JOB - 1;
	m_nSelNextJob = m_nSelectJob + 1;
	m_charName = CTString("");
	m_bNSCreate= FALSE;
}

void CharacterCreateNew::InfoReset()
{
	m_nSelectHair = 0;
	m_nSelectFace = 0;
	m_nSelectArmor = 0;
	m_nSelectSocial = 0;
}

void CharacterCreateNew::SendCreateChar()
{
	_pNetwork->SendCreateCharacter( m_charName, m_nSelectJob, m_nSelectHair + 1, m_nSelectFace + 1 );
}
