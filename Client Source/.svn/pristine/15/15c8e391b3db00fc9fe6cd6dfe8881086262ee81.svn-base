#include "StdH.h"
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Entities/InternalClasses.h>
#include "UIPetStashSelectEffect.h"

#include <Engine/GameDataManager/GameDataManager.h>
#include <Engine/Contents/Base/PetStash.h>
#include <Engine/Network/CNetwork.h>
#include <Engine/Help/Util_Help.h>
#include <Engine/Interface/UICommand.h>
#include "PetStashSelectEffectCmd.h"
#include <string>

#define DEF_PSANI_FILE		"Data\\Etc\\PSAni.ini"
#define DEF_APP_NAME		"Setting" 
#define DEF_KEY_TOTAL		"AniTotalTime"
#define DEF_KEY_DELAY		"DelayTime"
#define DEF_KEY_SLOW		"slowTime"
#define DEF_DEFAULT_TOTAL	3000
#define DEF_DEFAULT_DELAY	10
#define DEF_DEFAULT_SLOW	30



CUIPetStashSelectEffect::CUIPetStashSelectEffect()
	: m_nMaxFrame(0)
	, m_nCurFrame(0)
	, m_bAni(FALSE)
	, m_dwAniStartTime(0)
	, m_dwAniMaitionTime(0)
	, m_dwDelayTime(0)
	, m_dwDelay(0)
	, m_dwSlowTime(0)
	, m_bSelect(FALSE)
	, m_pbtnOk(NULL)
	, m_imgSelect(NULL)
	, m_aniLeft(NULL)
	, m_aniRight(NULL)
	, m_aniMid(NULL)
{
	setInherit(false);
}

CUIPetStashSelectEffect::~CUIPetStashSelectEffect()
{
	Destroy();
}

//------------------------------------------------------------------------------------
// Ani

void CUIPetStashSelectEffect::UpdateAni()
{
	DWORD nCurrent = timeGetTime();

	if ( nCurrent < m_dwAniStartTime + m_dwAniMaitionTime )
	{
		if (nCurrent > m_dwDelayTime)
		{
			m_nCurFrame = m_nCurFrame % m_nMaxFrame;

			m_aniLeft->SetRenderIdx( m_nCurFrame );
			m_aniMid->SetRenderIdx( m_nCurFrame + 1 );
			m_aniRight->SetRenderIdx( m_nCurFrame + 2 );

			m_dwDelayTime = nCurrent + m_dwDelay;
			m_dwDelay += m_dwSlowTime;
			m_nCurFrame++;

			_PlaySound(SOUND_TYPE_MOVE);
		}		
	}
	else
	{
		if ( m_bSelect == TRUE)
			EndAni();
		else
		{
			if ( nCurrent > m_dwDelayTime)
			{
				SelectModel();
			}			
		}
	}
}

void CUIPetStashSelectEffect::StartAni()
{
	m_nMaxFrame			= m_aniLeft->GetMaxFrame();
	m_nCurFrame			= 0;
	m_bAni				= TRUE;
	m_bSelect			= FALSE;
	m_dwAniStartTime	= timeGetTime();
	m_dwDelayTime		= 0;

	m_imgSelect->Hide(TRUE);
	m_pbtnOk->SetEnable(FALSE);
	m_pbtnOk->SetBtnState(UBS_DISABLE);

	InitPSAniTIme();
}

void CUIPetStashSelectEffect::EndAni()
{
	PetStash* pPetStash = GameDataManager::getSingleton()->GetStashData();

	int nSelectIndex = -1;

	if (pPetStash)
	{
		nSelectIndex = pPetStash->GetEffectModelNo();
		pPetStash->SetEffectModelNo(nSelectIndex);
		pPetStash->SendEffectReq(nSelectIndex);
	}

	_PlaySound(SOUND_TYPE_SELECT);

	m_bAni = FALSE;
	m_imgSelect->Hide(FALSE);
	m_pbtnOk->SetEnable(TRUE);
	m_pbtnOk->SetBtnState(UBS_IDLE);
}

void CUIPetStashSelectEffect::_ResetAni()
{
	m_nMaxFrame			= 0;
	m_nCurFrame			= 0;
	m_bAni				= FALSE;
	m_bSelect			= FALSE;
	m_dwAniStartTime	= 0;
	m_dwAniMaitionTime	= 0;
	m_dwSlowTime		= 0;
	m_dwDelayTime		= 0;
	m_dwDelay			= 0;
	m_imgSelect->Hide(TRUE);
}

//------------------------------------------------------------------------------------
// 이펙트 모델 선택
void CUIPetStashSelectEffect::SelectModel()
{
	PetStash* pPetStash = GameDataManager::getSingleton()->GetStashData();

	int nSelectIndex = -1;

	if (pPetStash)
		nSelectIndex = pPetStash->GetEffectModelNo();

	if (m_aniMid->GetRenderIdx() == nSelectIndex)
	{
		m_bSelect = TRUE;
	}
	else
	{
		m_nCurFrame = m_nCurFrame % m_nMaxFrame;

		m_aniLeft->SetRenderIdx( m_nCurFrame );
		m_aniMid->SetRenderIdx( m_nCurFrame + 1 );
		m_aniRight->SetRenderIdx( m_nCurFrame + 2 );

		m_dwDelayTime += m_dwDelay;
		m_nCurFrame++;

		_PlaySound(SOUND_TYPE_MOVE);
	}
}

// ini 파일에서 에니타임 얻는 함수
void CUIPetStashSelectEffect::InitPSAniTIme()
{
	std::string strFullPath = _fnmApplicationPath.FileDir();
	strFullPath += DEF_PSANI_FILE;

	m_dwAniMaitionTime = GetPrivateProfileInt( DEF_APP_NAME, DEF_KEY_TOTAL, DEF_DEFAULT_TOTAL, 
		strFullPath.c_str() );

	m_dwDelay = GetPrivateProfileInt( DEF_APP_NAME, DEF_KEY_DELAY, DEF_DEFAULT_DELAY, 
		strFullPath.c_str() );

	m_dwSlowTime = GetPrivateProfileInt( DEF_APP_NAME, DEF_KEY_SLOW, DEF_DEFAULT_SLOW, 
		strFullPath.c_str() );
}

//-----------------------------------------------------------------------------------
// 사운드 플레이

void CUIPetStashSelectEffect::_PlaySound(eSoundType type)
{
	CEntity			*penPlEntity;
	CPlayerEntity	*penPlayerEntity;

	penPlEntity = CEntity::GetPlayerEntity( 0 );
	penPlayerEntity = (CPlayerEntity *)penPlEntity;

	switch(type)
	{
	case SOUND_TYPE_MOVE:
		penPlayerEntity->PlayPetStashEffectSound();
		break;

	case SOUND_TYPE_SELECT:
		penPlayerEntity->PlayPetStashCardSelSound();
		break;
	}
	
}

//-----------------------------------------------------------------------------------

void CUIPetStashSelectEffect::OnUpdate( float fDeltaTime, ULONG ElapsedTime )
{
	if ( m_bAni )
	{
		UpdateAni();
	}
}

void CUIPetStashSelectEffect::initialize()
{
	m_pbtnOk = (CUIButton*)findUI("btn_ok");

	if( m_pbtnOk != NULL )
	{
		CmdPetSelectEffectClose* pCmd = new CmdPetSelectEffectClose;
		pCmd->setData(this);
		m_pbtnOk->SetCommandUp( pCmd );
		m_pbtnOk->SetEnable(FALSE);
		m_pbtnOk->SetBtnState(UBS_DISABLE);
	}

	// 가운데 선택 테두리
	m_imgSelect = (CUIImage*)findUI("imgSelCard");

	// 펫 카드
	m_aniLeft = (CUISpriteAni*)findUI("imgLCard");
	m_aniRight = (CUISpriteAni*)findUI("imgRCard");
	m_aniMid= (CUISpriteAni*)findUI("imgMCard");
}

void CUIPetStashSelectEffect::ClosePetStashSelectEffect()
{
	if (m_bAni == TRUE)
		return;

	SetVisible(FALSE);
	Hide(TRUE);

	_ResetAni();

	CUIManager::getSingleton()->RearrangeOrder(UI_PET_STASH_SELECT_EFFECT, FALSE);
}

void CUIPetStashSelectEffect::OpenPetStashSelectEffect()
{
	if(IsVisible() == TRUE )
		return;

	SetVisible(TRUE);
	Hide(FALSE);

	CUIManager::getSingleton()->RearrangeOrder(UI_PET_STASH_SELECT_EFFECT, TRUE);

	SetPos( m_rcOriginPos.Left, m_rcOriginPos.Top );

	StartAni();
}

void CUIPetStashSelectEffect::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
	m_rcOriginPos.SetRect(m_nPosX, m_nPosY, m_nWidth, m_nHeight);
}

void CUIPetStashSelectEffect::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

