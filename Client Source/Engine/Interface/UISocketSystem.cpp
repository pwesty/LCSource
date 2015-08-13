/********************************************************************
	UISocketSystem.cpp : implementation of the CUISocketSystem class.
*********************************************************************/

#include "stdh.h"
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Interface/UISocketSystem.h>	
#include <Engine/Interface/UIShop.h>
#include <Engine/Entities/InternalClasses.h>
#include <Common/Packet/ptype_old_do_exsocket.h>
#include <Engine/Interface/UIInventory.h>
#include <Engine/Interface/UIMessenger.h>
#include <Engine/Loading.h>
#include <Engine/Info/MyInfo.h>

const float	CUISocketSystem::PRICE_CREATE			= 0.25f;
const float	CUISocketSystem::PRICE_CLEAR			= 10000.0f;

static BOOL	m_bProcessing = FALSE;

#define JEWEL_COMPOS_SCROLL_ITEM_INDEX		9816
#define CHAOSJEWEL_COMPOS_SCROLL_ITEM_INDEX  9817

#define JEWEL_DFEND_SCROLL_INDEX 9907

enum eSocketSystemSelection
{
	SOCKET_CREATE,
	SOCKET_COMBINE,
	SOCKET_JEWEL_COMPOS,
	SOCKET_CHAOSJEWEL_COMPOS,
	SOCKET_EMPTY,
	SOCKET_ONCEEMPTY,
	SOCKET_CHANCECARD,
	SOCKET_SHOP,
	SOCKET_TALK,
	SOCKET_EVENT,
};

enum ESocketTex
{
	ESocketInactive = 0, // 비활성
	ESocketSelect, // 선택
	ESocketLock, // 락
};

CUISocketSystem::CUISocketSystem()
:	m_euSocketStat( STAT_CREATE ),
	m_ptdButtonTexture( NULL ),
 	m_ptdSocketCombineTexture( NULL ),
	m_ptdSocketOnceEmptyTexture( NULL ),
 	m_ptdSocketJewelComposTexture( NULL ),
	m_bStartDirection(FALSE),
	m_strToolTip(""),
	m_bShowToolTip( FALSE ),
	m_bPickTitle( FALSE ),
	m_bDrawStatusText( FALSE ),
	m_bActionStatus( SOCKET_CREATE_IDLE ),
	m_sqNeedMoney( 0 ),
	m_nHeightGap( 0 ),
	m_nOrgWidth( 0 ),
	m_nOrgHeight( 0 ),
	m_nSelSlotItemID( -1 ),
	m_fnReq(NULL),
	m_fnRender(NULL),
	m_fnMsgProc(NULL),
	m_pIconEmpty(NULL)
{
}

CUISocketSystem::~CUISocketSystem()
{
	Destroy();

	STOCK_RELEASE( m_ptdButtonTexture );
	STOCK_RELEASE( m_ptdSocketCombineTexture );
	STOCK_RELEASE( m_ptdSocketOnceEmptyTexture );
	STOCK_RELEASE( m_ptdSocketJewelComposTexture );

	for (int i = 0; i < ITEM_MAX_COUNT; ++i)
		SAFE_DELETE(m_pIconsItem[i]);

	SAFE_DELETE(m_pIconEmpty);
}

//////////////////////////////////////////////////////////////////////////
// Protect functions.
void CUISocketSystem::_toggleVisible( const _SOCKET_STAT _status /* = STAT_CREATE  */)
{
	m_euSocketStat = _status;

	if( _status == STAT_CREATE )
		_setCreateInfo();
	else if( _status == STAT_COMBINE )
		_setCombineInfo();
	else if( _status == STAT_JEWEL_COMPOS )
		_setJewelcomposInfo();
	else if( _status == STAT_CHAOSJEWEL_COMPOS )
		_setChaosJewelcomposInfo();
	else if( _status == STAT_EMPTY )
		_setEmptyInfo();
	else if ( _status == STAT_ONCEEMPTY )
		_setEmptyOnceInfo();

	BOOL bVisible = !( IsVisible() );

	m_bProcessing = FALSE;
	CUIManager* pUIManager = CUIManager::getSingleton();

	if( bVisible )
	{
		pUIManager->RearrangeOrder( UI_SOCKETSYSTEM, TRUE );
		pUIManager->GetInventory()->Lock( TRUE, TRUE, LOCK_SOCKETSYSTEM );
	}
	else
	{
		pUIManager->RearrangeOrder( UI_SOCKETSYSTEM, FALSE );
		pUIManager->GetInventory()->Lock( FALSE, FALSE, LOCK_SOCKETSYSTEM );
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CUISocketSystem::_close()
{
	for( int i = 0; i < ITEM_MAX_COUNT; ++i )
	{
		m_pIconsItem[i]->clearIconData();
	}

	m_nSelSlotItemID = -1;
	m_bShowToolTip = FALSE;
	m_bPickTitle = FALSE;
	m_bUseTimer = FALSE;

	m_btnConfirm.SetEnable( TRUE );
	m_fnReq = NULL;

	m_bProcessing = FALSE;
	CUIManager* pUIManager = CUIManager::getSingleton();
	pUIManager->RearrangeOrder( UI_SOCKETSYSTEM, FALSE );
	pUIManager->GetInventory()->Lock( FALSE, FALSE, LOCK_SOCKETSYSTEM );
	_ResetEmptyGuideCombine();

	if( pUIManager->DoesMessageBoxExist(MSGCMD_JEWEL_COMPOS_JEWEL_COMPOS_SCROLL) )
	{
		pUIManager->CloseMessageBox(MSGCMD_JEWEL_COMPOS_JEWEL_COMPOS_SCROLL);
	}
	_ResetJewelCompos();	
	_ResetEmptyGuide();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CUISocketSystem::_reset()
{
	m_euSocketStat = STAT_CREATE;
	m_fnReq = NULL;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CUISocketSystem::_setUIRect(const _SOCKET_STAT _status, const int _nHeight /* = 0  */)
{
	switch( _status)
	{
	case STAT_COMBINE:
		{
			SetSize(EX_SOCKET_UI_FRAME_WIDTH, EX_COMBINE_UI_FRAME_HEIGHT);

			m_rcTop.SetRect( 0, 0, m_nWidth, EX_SOCKET_UI_FRAME_TITLE);
			m_rcMiddle.SetRect( 0, EX_SOCKET_UI_FRAME_TITLE, m_nWidth, m_nHeight-EX_SOCKET_UI_FRAME_INTERVAL );
			m_rcBottom.SetRect( 0, m_nHeight-EX_SOCKET_UI_FRAME_INTERVAL, m_nWidth, m_nHeight);

			m_rcArea.SetRect(	0, 
				EX_SOCKET_UI_FRAME_TITLE, 
				EX_SOCKET_UI_FRAME_WIDTH, 
				EX_COMBINE_UI_FRAME_HEIGHT-_nHeight );
		}
		break;
	case STAT_ONCEEMPTY:
		{
			SetSize(EX_SOCKET_UI_FRAME_WIDTH, EX_SOCKET_UI_FRAME_HEIGHT);

			m_rcTop.SetRect( 0, 0, m_nWidth, EX_SOCKET_UI_FRAME_TITLE);
			m_rcMiddle.SetRect( 0, EX_SOCKET_UI_FRAME_TITLE, m_nWidth, m_nHeight-EX_SOCKET_UI_FRAME_INTERVAL );
			m_rcBottom.SetRect( 0, m_nHeight-EX_SOCKET_UI_FRAME_INTERVAL, m_nWidth, m_nHeight);

			m_rcArea.SetRect(	0, 
				EX_SOCKET_UI_FRAME_TITLE, 
				EX_SOCKET_UI_FRAME_WIDTH, 
				EX_SOCKET_UI_FRAME_HEIGHT-_nHeight );
		}
		break;

	case STAT_JEWEL_COMPOS:
	case STAT_CHAOSJEWEL_COMPOS:
		{
				SetSize(JEWEL_COMPOS_UI_FRAME_WIDTH, JEWEL_COMPOS_UI_FRAME_HEIGHT);
				
				m_rcTop.SetRect( 0, 0, m_nWidth, JEWEL_COMPOS_UI_FRAME_TITLE);
				m_rcMiddle.SetRect( 0, JEWEL_COMPOS_UI_FRAME_TITLE, m_nWidth, m_nHeight-JEWEL_COMPOS_UI_FRAME_BOTTOM );
				m_rcBottom.SetRect( 0, m_nHeight-JEWEL_COMPOS_UI_FRAME_BOTTOM, m_nWidth, m_nHeight);
				
				m_rcArea.SetRect(	0, 
					JEWEL_COMPOS_UI_FRAME_TITLE, 
					JEWEL_COMPOS_UI_FRAME_WIDTH, 
						JEWEL_COMPOS_UI_FRAME_HEIGHT-_nHeight );
		}
		break;
	case STAT_EMPTY:
	case STAT_CREATE:
		{
			SetSize(SOCKET_UI_FRAME_WIDTH, SOCKET_UI_FRAME_HEIGHT);
			m_nHeightGap = _nHeight;
			m_nHeight = m_nOrgHeight - m_nHeightGap;

			m_rcTop.SetRect( 0, 0, m_nWidth, SOCKET_UI_FRAME_TITLE);
			m_rcMiddle.SetRect( 0, SOCKET_UI_FRAME_TITLE, m_nWidth, m_nHeight-SOCKET_UI_FRAME_INTERVAL );
			m_rcBottom.SetRect( 0, m_nHeight-SOCKET_UI_FRAME_INTERVAL, m_nWidth, m_nHeight);
			
			m_rcArea.SetRect(	0, 
				SOCKET_UI_FRAME_TITLE, 
				SOCKET_UI_FRAME_WIDTH, 
						SOCKET_UI_FRAME_HEIGHT-m_nHeightGap );
		}
		break;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//SOCKET_SYSTEM_S2 본체 UV 사이즈 변경
void CUISocketSystem::_setUIUVRect(const _SOCKET_STAT _status)
{
	FLOAT		fTexWidth = 0.0f;
	FLOAT		fTexHeight = 0.0f;

	switch( _status)
	{
	case STAT_COMBINE:
		{
			fTexWidth = m_ptdSocketCombineTexture->GetPixWidth();
			fTexHeight = m_ptdSocketCombineTexture->GetPixHeight();
			m_rtTop.SetUV( 0, 0, EX_SOCKET_UI_TEX_FRAME_WIDTH, EX_SOCKET_UI_FRAME_TITLE, fTexWidth, fTexHeight );
			m_rtMiddle.SetUV( 0, EX_SOCKET_UI_FRAME_TITLE, EX_SOCKET_UI_TEX_FRAME_WIDTH, EX_COMBINE_UI_TEX_FRAME_HEIGHT-EX_SOCKET_UI_FRAME_BOTTOM, fTexWidth, fTexHeight );
			m_rtBottom.SetUV( 0, EX_COMBINE_UI_TEX_FRAME_HEIGHT-EX_SOCKET_UI_FRAME_BOTTOM, EX_SOCKET_UI_TEX_FRAME_WIDTH, EX_COMBINE_UI_TEX_FRAME_HEIGHT, fTexWidth, fTexHeight );
		}
		break;
	case STAT_ONCEEMPTY:
		{
			fTexWidth = m_ptdSocketOnceEmptyTexture->GetPixWidth();
			fTexHeight = m_ptdSocketOnceEmptyTexture->GetPixHeight();
			m_rtTop.SetUV( 0, 0, EX_SOCKET_UI_TEX_FRAME_WIDTH, EX_SOCKET_UI_FRAME_TITLE, fTexWidth, fTexHeight );
			m_rtMiddle.SetUV( 0, EX_SOCKET_UI_FRAME_TITLE, EX_SOCKET_UI_TEX_FRAME_WIDTH, EX_SOCKET_UI_TEX_FRAME_HEIGHT-EX_SOCKET_UI_FRAME_BOTTOM, fTexWidth, fTexHeight );
			m_rtBottom.SetUV( 0, EX_SOCKET_UI_TEX_FRAME_HEIGHT-EX_SOCKET_UI_FRAME_BOTTOM, EX_SOCKET_UI_TEX_FRAME_WIDTH, EX_SOCKET_UI_TEX_FRAME_HEIGHT, fTexWidth, fTexHeight );
		}
		break;

	case STAT_JEWEL_COMPOS:
	case STAT_CHAOSJEWEL_COMPOS:
		{
			fTexWidth = m_ptdSocketJewelComposTexture->GetPixWidth();
			fTexHeight = m_ptdSocketJewelComposTexture->GetPixHeight();

			m_rtTop.SetUV( 0, 0, JEWEL_COMPOS_UI_TEX_FRAME_WIDTH, JEWEL_COMPOS_UI_FRAME_TITLE, fTexWidth, fTexHeight );
			m_rtMiddle.SetUV( 0, JEWEL_COMPOS_UI_FRAME_TITLE, JEWEL_COMPOS_UI_TEX_FRAME_WIDTH, JEWEL_COMPOS_UI_TEX_FRAME_HEIGHT-JEWEL_COMPOS_UI_FRAME_BOTTOM, fTexWidth, fTexHeight );
			m_rtBottom.SetUV( 0, JEWEL_COMPOS_UI_TEX_FRAME_HEIGHT-JEWEL_COMPOS_UI_FRAME_BOTTOM, JEWEL_COMPOS_UI_TEX_FRAME_WIDTH, JEWEL_COMPOS_UI_TEX_FRAME_HEIGHT, fTexWidth, fTexHeight );	
		}
		break;
	case STAT_EMPTY:
	case STAT_CREATE:
		{
			fTexWidth = m_ptdBaseTexture->GetPixWidth();
			fTexHeight = m_ptdBaseTexture->GetPixHeight();

			m_rtTop.SetUV( 0, 0, SOCKET_UI_TEX_FRAME_WIDTH, SOCKET_UI_FRAME_TITLE, fTexWidth, fTexHeight );
			m_rtMiddle.SetUV( 0, SOCKET_UI_FRAME_TITLE, SOCKET_UI_TEX_FRAME_WIDTH, SOCKET_UI_TEX_FRAME_HEIGHT-SOCKET_UI_FRAME_BOTTOM, fTexWidth, fTexHeight );
			m_rtBottom.SetUV( 0, SOCKET_UI_TEX_FRAME_HEIGHT-SOCKET_UI_FRAME_BOTTOM, SOCKET_UI_TEX_FRAME_WIDTH, SOCKET_UI_TEX_FRAME_HEIGHT, fTexWidth, fTexHeight );
		}
		break;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//합성연출
void CUISocketSystem::_JewelComposDirectionStart(UBYTE StartType)
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	// 시작 시간 저장
	m_dwStartTime = timeGetTime();
	m_dwChanegeTime = m_dwStartTime;
	// 합성될 타입 저장
	m_DirectionType = StartType;
	// 보조 슬롯, 보석 슬롯 반투명 처리
	m_bBtnTranslucency = TRUE;
	// 합성 연출 시작
	m_bStartDirection = TRUE;
	m_eJewelActionState = JEWEL_COMPOS_ACTION_DIRECTION;
	m_iterChanegeInfo = m_DirectionInfo.begin();

	// 결과 보석 슬롯 초기화 후 진행
	m_pIconsItem[JEWEL_COMPOS_BTN_JEWEL_RESULT]->clearIconData();

	// 사운드 재생
	int SoundType = rand() % 10;
	if (SoundType > 0)
		_PlaySound(JEWEL_SOUND_DULL);
	else
		_PlaySound(JEWEL_SOUND_REFINEMENT);

	pUIManager->RearrangeOrder( UI_SOCKETSYSTEM, TRUE );
	pUIManager->GetInventory()->Lock( FALSE, FALSE, LOCK_SOCKETSYSTEM );
	pUIManager->GetInventory()->Lock( TRUE, FALSE, LOCK_SOCKETSYSTEM );
}

void CUISocketSystem::_JewelComposDirectionEnd()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	// 합성 연출 끝
	m_bStartDirection = FALSE;
	// 보조 슬롯, 보석 슬롯 반투명 해제
	m_bBtnTranslucency = FALSE;
	// 서버로 패킷 보내기
	SendUpgradeReq();

	m_pIconsItem[JEWEL_COMPOS_BTN_JEWEL_RESULT]->clearIconData();
}

BOOL CUISocketSystem::_JewelComposDirectionGradeInfo(eJEWEL_COMPOS_TYPE composType)
{
	m_DirectionInfo.clear();

	int i, Grade[JEWEL_COMPOS_JEWEL_MAX], nAverageGrade = 0, nSum = 0;
	
	for (i = 0; i < JEWEL_COMPOS_JEWEL_MAX; i++)
	{
		if (m_pIconsItem[i]->IsEmpty())
			return FALSE;

		CItemData* pItemData = m_pIconsItem[i]->getItems()->ItemData;
		Grade[i] = pItemData->GetNum0();
		nSum += Grade[i];
	}

	nAverageGrade = nSum / JEWEL_COMPOS_JEWEL_MAX;

	if (composType == GENARAL_JEWEL_COMPOS)
		return _pNetwork->GetJewelGradeInfo(nAverageGrade, m_DirectionInfo);
	else 
		return _pNetwork->GetChaosJewelGradeInfo(nAverageGrade, m_DirectionInfo);

}

BOOL CUISocketSystem::_CheckComposDirectionTime()
{
	DWORD dwCurrentTime = timeGetTime();
	
	if( dwCurrentTime - m_dwStartTime > JEWEL_COMPOS_DIRECTION_TIME )
		return TRUE;
	
	return FALSE;
}

void CUISocketSystem::_JewelComposDirection()
{
	BOOL bEnd = _CheckComposDirectionTime();

	if(bEnd)
	{
		_JewelComposDirectionEnd();
	}
	else
	{
		_ChangeDirectionEfect();
	}

}

void CUISocketSystem::_ChangeDirectionEfect()
{
//	int InfoCount = m_DirectionInfo.size();
	DWORD dwCurrentTime = timeGetTime();
	if(dwCurrentTime > m_dwChanegeTime)
	{
		m_dwChanegeTime += JEWEL_COMPOS_CHANGE_JEWEL_EFFECT_TIME;
			
		int jewelindex = m_iterChanegeInfo->Jewel_idx;
		m_pIconsItem[JEWEL_COMPOS_BTN_JEWEL_RESULT]->setData(UBET_ITEM, jewelindex);
		++m_iterChanegeInfo;

		if (m_iterChanegeInfo == m_DirectionInfo.end())
			m_iterChanegeInfo = m_DirectionInfo.begin();		
	}
}

void CUISocketSystem::_RenderResultJewel()
{
	if (m_bStartDirection)
		return;

	if (m_pIconsItem[JEWEL_COMPOS_BTN_JEWEL_RESULT]->IsEmpty() == true)
		return;

	CUIManager* pUIManager = CUIManager::getSingleton();
	CDrawPort* pDrawPort = pUIManager->GetDrawPort();


	CItemData* pItemDataResultJewel = m_pIconsItem[JEWEL_COMPOS_BTN_JEWEL_RESULT]->getItems()->ItemData;

	if( pItemDataResultJewel->GetSubType() == CItemData::ITEM_ETC_CHAOSJEWEL )
	{
		m_rtsResultJewel.m_RectSurfaceArray[CHAOS_JEWEL_COMPOS].m_RT = UIRect(m_nPosX + m_pIconsItem[JEWEL_COMPOS_BTN_JEWEL_RESULT]->GetPosX() - 9,
			m_nPosY + m_pIconsItem[JEWEL_COMPOS_BTN_JEWEL_RESULT]->GetPosY() - 8,
			m_nPosX + m_pIconsItem[JEWEL_COMPOS_BTN_JEWEL_RESULT]->GetPosX() + 41,
			m_nPosY + m_pIconsItem[JEWEL_COMPOS_BTN_JEWEL_RESULT]->GetPosY() + 40);
			m_rtsResultJewel.RenderRectSurface(pDrawPort, 0xFFFFFFFF, CHAOS_JEWEL_COMPOS);
	}
	else
	{
		m_rtsResultJewel.m_RectSurfaceArray[GENARAL_JEWEL_COMPOS].m_RT = UIRect(m_nPosX + m_pIconsItem[JEWEL_COMPOS_BTN_JEWEL_RESULT]->GetPosX() - 9,
			m_nPosY + m_pIconsItem[JEWEL_COMPOS_BTN_JEWEL_RESULT]->GetPosY() - 8,
			m_nPosX + m_pIconsItem[JEWEL_COMPOS_BTN_JEWEL_RESULT]->GetPosX() + 41,
			m_nPosY + m_pIconsItem[JEWEL_COMPOS_BTN_JEWEL_RESULT]->GetPosY() + 40);
			m_rtsResultJewel.RenderRectSurface(pDrawPort, 0xFFFFFFFF, GENARAL_JEWEL_COMPOS);
	}
}

void CUISocketSystem::_JewelRenderBtnTranslucency()
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	CDrawPort* pDrawPort = pUIManager->GetDrawPort();
	// 아이템 버튼 백그라운드 회색 처리 [9/13/2012 Ranma]
	// jewel1
	CItemData* pItemDataJewel1 = m_pIconsItem[JEWEL_COMPOS_BTN_JEWEL_1]->getItems()->ItemData;
	UIRectUV	rectJewel1UV;

	rectJewel1UV.SetUV(pItemDataJewel1->GetIconTexCol() * BTN_SIZE,
		pItemDataJewel1->GetIconTexRow() * BTN_SIZE,
		(pItemDataJewel1->GetIconTexCol()+1) * BTN_SIZE,
		(pItemDataJewel1->GetIconTexRow()+1) * BTN_SIZE,
		_pUIBtnTexMgr->GetTexWidth( UBET_ITEM, pItemDataJewel1->GetIconTexID() ),
		_pUIBtnTexMgr->GetTexHeight( UBET_ITEM, pItemDataJewel1->GetIconTexID() ) );

	pDrawPort->AddBtnTexture(	pItemDataJewel1->GetIconTexID(),
		m_nPosX + m_pIconsItem[JEWEL_COMPOS_BTN_JEWEL_1]->GetPosX() , m_nPosY + m_pIconsItem[JEWEL_COMPOS_BTN_JEWEL_1]->GetPosY(), 
		m_nPosX + m_pIconsItem[JEWEL_COMPOS_BTN_JEWEL_1]->GetPosX() + BTN_SIZE , m_nPosY + m_pIconsItem[JEWEL_COMPOS_BTN_JEWEL_1]->GetPosY() + BTN_SIZE,
		rectJewel1UV.U0, rectJewel1UV.V0,
		rectJewel1UV.U1, rectJewel1UV.V1, 0x404040FF );

	// jewel2
	CItemData* pItemDataJewel2 = m_pIconsItem[JEWEL_COMPOS_BTN_JEWEL_2]->getItems()->ItemData;
	UIRectUV	rectJewel2UV;
	
	rectJewel2UV.SetUV(pItemDataJewel2->GetIconTexCol() * BTN_SIZE,
		pItemDataJewel2->GetIconTexRow() * BTN_SIZE,
		(pItemDataJewel2->GetIconTexCol()+1) * BTN_SIZE,
		(pItemDataJewel2->GetIconTexRow()+1) * BTN_SIZE,
		_pUIBtnTexMgr->GetTexWidth( UBET_ITEM, pItemDataJewel2->GetIconTexID() ),
		_pUIBtnTexMgr->GetTexHeight( UBET_ITEM, pItemDataJewel2->GetIconTexID() ) );
	
	pDrawPort->AddBtnTexture(	pItemDataJewel2->GetIconTexID(),
		m_nPosX + m_pIconsItem[JEWEL_COMPOS_BTN_JEWEL_2]->GetPosX() , m_nPosY + m_pIconsItem[JEWEL_COMPOS_BTN_JEWEL_2]->GetPosY(), 
		m_nPosX + m_pIconsItem[JEWEL_COMPOS_BTN_JEWEL_2]->GetPosX() + BTN_SIZE , m_nPosY + m_pIconsItem[JEWEL_COMPOS_BTN_JEWEL_2]->GetPosY() + BTN_SIZE,
		rectJewel2UV.U0, rectJewel2UV.V0,
		rectJewel2UV.U1, rectJewel2UV.V1, 0x404040FF );

	//jewel3
	CItemData* pItemDataJewel3 = m_pIconsItem[JEWEL_COMPOS_BTN_JEWEL_3]->getItems()->ItemData;
	UIRectUV	rectJewel3UV;
	
	rectJewel3UV.SetUV(pItemDataJewel3->GetIconTexCol() * BTN_SIZE,
		pItemDataJewel3->GetIconTexRow() * BTN_SIZE,
		(pItemDataJewel3->GetIconTexCol()+1) * BTN_SIZE,
		(pItemDataJewel3->GetIconTexRow()+1) * BTN_SIZE,
		_pUIBtnTexMgr->GetTexWidth( UBET_ITEM, pItemDataJewel3->GetIconTexID() ),
		_pUIBtnTexMgr->GetTexHeight( UBET_ITEM, pItemDataJewel3->GetIconTexID() ) );
	
	pDrawPort->AddBtnTexture(	pItemDataJewel3->GetIconTexID(),
		m_nPosX + m_pIconsItem[JEWEL_COMPOS_BTN_JEWEL_3]->GetPosX() , m_nPosY + m_pIconsItem[JEWEL_COMPOS_BTN_JEWEL_3]->GetPosY(), 
		m_nPosX + m_pIconsItem[JEWEL_COMPOS_BTN_JEWEL_3]->GetPosX() + BTN_SIZE , m_nPosY + m_pIconsItem[JEWEL_COMPOS_BTN_JEWEL_3]->GetPosY() + BTN_SIZE,
		rectJewel3UV.U0, rectJewel3UV.V0,
		rectJewel3UV.U1, rectJewel3UV.V1, 0x404040FF );

	//scroll
	if (m_pIconsItem[JEWEL_COMPOS_BTN_SCROLL]->IsEmpty() == false)
	{
		CItemData* pItemDataBottonScroll = m_pIconsItem[JEWEL_COMPOS_BTN_SCROLL]->getItems()->ItemData;
		UIRectUV	rectScrollUV;
		
		rectScrollUV.SetUV(pItemDataBottonScroll->GetIconTexCol() * BTN_SIZE,
			pItemDataBottonScroll->GetIconTexRow() * BTN_SIZE,
			(pItemDataBottonScroll->GetIconTexCol()+1) * BTN_SIZE,
			(pItemDataBottonScroll->GetIconTexRow()+1) * BTN_SIZE,
			_pUIBtnTexMgr->GetTexWidth( UBET_ITEM, pItemDataBottonScroll->GetIconTexID() ),
			_pUIBtnTexMgr->GetTexHeight( UBET_ITEM, pItemDataBottonScroll->GetIconTexID() ) );
		
		pDrawPort->AddBtnTexture(	pItemDataBottonScroll->GetIconTexID(),
			m_nPosX + m_pIconsItem[JEWEL_COMPOS_BTN_SCROLL]->GetPosX() , m_nPosY + m_pIconsItem[JEWEL_COMPOS_BTN_SCROLL]->GetPosY(), 
			m_nPosX + m_pIconsItem[JEWEL_COMPOS_BTN_SCROLL]->GetPosX() + BTN_SIZE , m_nPosY + m_pIconsItem[JEWEL_COMPOS_BTN_SCROLL]->GetPosY() + BTN_SIZE,
			rectScrollUV.U0, rectScrollUV.V0,
		rectScrollUV.U1, rectScrollUV.V1, 0x404040FF );
	}

	pDrawPort->FlushBtnRenderingQueue( UBET_ITEM );
}

void CUISocketSystem::_ResetJewelCompos()
{
	m_eJewelActionState = JEWEL_COMPOS_ACTION_IDLE;
	m_colorJewelcomposNasStr = 0xFFFFFFFF;
	m_nOneMoreComposScrollpay = 0;// 한번의 합성에 사용되는 스크롤 갯수
	m_sqNeedMoney = 0;
	m_DirectionInfo.clear();
	m_iterChanegeInfo = m_DirectionInfo.begin();
	m_dwStartTime = 0;
	m_bStartDirection = FALSE;
	m_bBtnTranslucency = FALSE;
	m_DirectionType = 0;
	m_dwChanegeTime = 0;
}
// combine empty guide popup
void CUISocketSystem::_CreateEmptyGuideCombinePopup()
{
	m_CombineEmptyBtnGuide.Create( this, 124, 218, BTN_SIZE, BTN_SIZE);
	m_CombineEmptyBtnGuide.SetRenderRegion(0, 0, BTN_SIZE, BTN_SIZE);

	CTString textEmptyGuide = _S( 6052, "겹합 보조아이템을 올려주세요." );
	int nStrWidthSize = textEmptyGuide.Length() * _pUIFontTexMgr->GetFontWidth() + JEWEL_COMPOS_EMPTY_GUIDE_WIDTH;
	m_CombineEmptyBtnGuide.SetPopupInfo(textEmptyGuide, TRUE, 0xFFFFFFFF, nStrWidthSize);
}

void CUISocketSystem::_CheckEmptyGuideCombine()	
{
	m_bCombineEmptyBtnGuide = m_pIconsItem[8]->IsEmpty(); // 결합 보조 아이템의 유무
}

void CUISocketSystem::_RenderEmptyGuideCombine()
{
	if ( m_bCombineEmptyBtnGuide == TRUE )
			m_CombineEmptyBtnGuide.PopupRender();
}

void CUISocketSystem::_ResetEmptyGuideCombine()
{
	m_bCombineEmptyBtnGuide = FALSE;
	m_CombineEmptyBtnGuide.ResetImageBox();
}
//Popup
void CUISocketSystem::_ResetEmptyGuide()
{
	for (int i = 0; i < BTN_EMPTY_MAX; i++)
	{
		m_bJewelComposEmptyBtnGuide[i] = FALSE;
		m_ImageJewelComposEmptyBtnGuide[i].ResetImageBox();
	}
}

void CUISocketSystem::_CreateEmptyGuidePopup(eJEWEL_COMPOS_TYPE composType)
{
	m_ImageJewelComposEmptyBtnGuide[BTN_EMPTY_JEWEL1].Create( this, JEWEL_COMPOS_BTN_JEWEL1_POS_X, JEWEL_COMPOS_BTN_JEWEL1_POS_Y, BTN_SIZE, BTN_SIZE);
	m_ImageJewelComposEmptyBtnGuide[BTN_EMPTY_JEWEL1].SetRenderRegion(0, 0, BTN_SIZE, BTN_SIZE);

	m_ImageJewelComposEmptyBtnGuide[BTN_EMPTY_JEWEL2].Create( this, JEWEL_COMPOS_BTN_JEWEL2_POS_X, JEWEL_COMPOS_BTN_JEWEL2_POS_Y, BTN_SIZE, BTN_SIZE);
	m_ImageJewelComposEmptyBtnGuide[BTN_EMPTY_JEWEL2].SetRenderRegion(0, 0, BTN_SIZE, BTN_SIZE);

	m_ImageJewelComposEmptyBtnGuide[BTN_EMPTY_JEWEL3].Create( this, JEWEL_COMPOS_BTN_JEWEL3_POS_X, JEWEL_COMPOS_BTN_JEWEL3_POS_Y, BTN_SIZE, BTN_SIZE);
	m_ImageJewelComposEmptyBtnGuide[BTN_EMPTY_JEWEL3].SetRenderRegion(0, 0, BTN_SIZE, BTN_SIZE);

	m_ImageJewelComposEmptyBtnGuide[BTN_EMPTY_SCROLL].Create( this, JEWEL_COMPOS_BTN_SCROLL_POS_X, JEWEL_COMPOS_BTN_SCROLL_POS_Y, BTN_SIZE, BTN_SIZE);
	m_ImageJewelComposEmptyBtnGuide[BTN_EMPTY_SCROLL].SetRenderRegion(0, 0, BTN_SIZE, BTN_SIZE);

	m_ImageJewelComposEmptyBtnGuide[BTN_EMPTY_RESULT].Create( this, JEWEL_COMPOS_BTN_RESULT_POS_X, JEWEL_COMPOS_BTN_RESULT_POS_Y, BTN_SIZE, BTN_SIZE);
	m_ImageJewelComposEmptyBtnGuide[BTN_EMPTY_RESULT].SetRenderRegion(0, 0, BTN_SIZE, BTN_SIZE);

	m_ImageJewelComposEmptyBtnGuide[BTN_EMPTY_NASBAR].Create( this, JEWEL_COMPOS_NASBAR_POS_X, JEWEL_COMPOS_NASBAR_POS_Y, JEWEL_COMPOS_NASBAR_WIDTH, JEWEL_COMPOS_NASBAR_HEIGHT);
	m_ImageJewelComposEmptyBtnGuide[BTN_EMPTY_NASBAR].SetRenderRegion(0, 0, JEWEL_COMPOS_NASBAR_WIDTH, JEWEL_COMPOS_NASBAR_HEIGHT);


	CTString textEmptyGuide;
	int		nStrWidthSize = 0;
	if (composType == GENARAL_JEWEL_COMPOS)
		textEmptyGuide.PrintF( _S(5931, "일반보석을 등록하세요"));
	else
		textEmptyGuide.PrintF( _S(5929, "카오스보석을 등록하세요"));
	
	nStrWidthSize = textEmptyGuide.Length() * _pUIFontTexMgr->GetFontWidth() + JEWEL_COMPOS_EMPTY_GUIDE_WIDTH;
	m_ImageJewelComposEmptyBtnGuide[BTN_EMPTY_JEWEL1].SetPopupInfo(textEmptyGuide, TRUE, 0xFFFFFFFF, nStrWidthSize);
	m_ImageJewelComposEmptyBtnGuide[BTN_EMPTY_JEWEL2].SetPopupInfo(textEmptyGuide, TRUE, 0xFFFFFFFF, nStrWidthSize);
	m_ImageJewelComposEmptyBtnGuide[BTN_EMPTY_JEWEL3].SetPopupInfo(textEmptyGuide, TRUE, 0xFFFFFFFF, nStrWidthSize);

	textEmptyGuide.PrintF( _S(5937, "보석 합성스크롤을 등록하면 합성에 도움이 됩니다."));
	nStrWidthSize = textEmptyGuide.Length() * _pUIFontTexMgr->GetFontWidth() + JEWEL_COMPOS_EMPTY_GUIDE_WIDTH;
	m_ImageJewelComposEmptyBtnGuide[BTN_EMPTY_SCROLL].SetPopupInfo(textEmptyGuide, TRUE, 0xFFFFFFFF, nStrWidthSize);

	textEmptyGuide.PrintF( _S(5938, "합성으로 생성되는 보석을 표시합니다."));
	nStrWidthSize = textEmptyGuide.Length() * _pUIFontTexMgr->GetFontWidth() + JEWEL_COMPOS_EMPTY_GUIDE_WIDTH;
	m_ImageJewelComposEmptyBtnGuide[BTN_EMPTY_RESULT].SetPopupInfo(textEmptyGuide, TRUE, 0xFFFFFFFF, nStrWidthSize);

	textEmptyGuide.PrintF( _S(5939, "합성에 필요한 나스입니다."));
	nStrWidthSize = textEmptyGuide.Length() * _pUIFontTexMgr->GetFontWidth() + JEWEL_COMPOS_EMPTY_GUIDE_WIDTH;
	m_ImageJewelComposEmptyBtnGuide[BTN_EMPTY_NASBAR].SetPopupInfo(textEmptyGuide, TRUE, 0xFFFFFFFF, nStrWidthSize);
}

void CUISocketSystem::_CheckEmptyGuide()
{
	for (int i = 0; i < BTN_EMPTY_MAX; i++)
	{
		if (i == BTN_EMPTY_NASBAR)
		{
			m_bJewelComposEmptyBtnGuide[i] = TRUE;
			continue;
		}
		m_bJewelComposEmptyBtnGuide[i] = m_pIconsItem[i]->IsEmpty();
	}
}

void CUISocketSystem::_RenderEmptyGuidePopup()
{
	for (int i = 0; i < BTN_EMPTY_MAX; i++)
	{
		if ( !m_bJewelComposEmptyBtnGuide[i] )
			continue;

		m_ImageJewelComposEmptyBtnGuide[i].PopupRender();
	}
}

void CUISocketSystem::_PlaySound(eJEWEL_SOUND_TYPE playSound)
{
	CEntity			*penPlEntity;
	CPlayerEntity	*penPlayerEntity;
	
	penPlEntity = CEntity::GetPlayerEntity( 0 );
	penPlayerEntity = (CPlayerEntity *)penPlEntity;
	
	switch(playSound)
	{
	case JEWEL_SOUND_DULL:
		penPlayerEntity->PlayJewelDullSound();
		break;
	case JEWEL_SOUND_REFINEMENT:
		penPlayerEntity->PlayJewelRefinementSound();
		break;
	case JEWEL_SOUND_SHINY:
		penPlayerEntity->PlayJewelShinySound();
		break;
	}
	
}

void CUISocketSystem::_InteractionMsgBoxReject()
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	
	// 파티 초대[신청받음]
	if(pUIManager->DoesMessageBoxExist(MSGCMD_PARTY_ALLOW))
	{
		pUIManager->GetMessageBox(MSGCMD_PARTY_ALLOW)->ReturnCommand(FALSE);
	}
	
	// 파티 초대[신청]
	if(pUIManager->DoesMessageBoxExist(MSGCMD_PARTY_INVITE))
	{
		pUIManager->GetMessageBox(MSGCMD_PARTY_INVITE)->ReturnCommand(FALSE);
	}
	
	// 파티리콜[신청 받음]
	if(pUIManager->DoesMessageBoxExist(MSGCMD_EX_PARTY_RECALL_PROMPT))
	{
		pUIManager->GetMessageBox(MSGCMD_EX_PARTY_RECALL_PROMPT)->ReturnCommand(FALSE);
	}
	
	// 교환 요청 [요청]
	if(pUIManager->DoesMessageBoxExist(MSGCMD_EXCH_REQ_SRC))
	{
		pUIManager->GetMessageBox(MSGCMD_EXCH_REQ_SRC)->ReturnCommand(FALSE);
	}
	
	// 교환 요청 [요청받음]
	if(pUIManager->DoesMessageBoxExist(MSGCMD_EXCH_REQ_DEST))
	{
		pUIManager->GetMessageBox(MSGCMD_EXCH_REQ_DEST)->ReturnCommand(FALSE);
	}
	
	// 길드 가입 요청 [요청받음]
	if(pUIManager->DoesMessageBoxExist(MSGCMD_GUILD_JOIN))
	{
		pUIManager->GetMessageBox(MSGCMD_GUILD_JOIN)->ReturnCommand(FALSE);
	}
	
	// 길드 가입 요청 [요청]
	if(pUIManager->DoesMessageBoxExist(MSGCMD_GUILD_JOIN_REQ))
	{
		pUIManager->GetMessageBox(MSGCMD_GUILD_JOIN_REQ)->ReturnCommand(FALSE);
	}
	
	// 길드 전투 요청
	if(pUIManager->DoesMessageBoxExist(MSGCMD_GUILD_BATTLE_REQ))
	{
		pUIManager->GetMessageBox(MSGCMD_GUILD_BATTLE_REQ)->ReturnCommand(FALSE);
	}
	
	// 길드 전투 신청 창
	if(pUIManager->DoesMessageBoxExist(MSGCMD_GUILD_BATTLE_MESSAGE))
	{
		pUIManager->GetMessageBox(MSGCMD_GUILD_BATTLE_MESSAGE)->ReturnCommand(FALSE);
	}
	
	// 길드 전투 수락창 [요청 받음]
	if(pUIManager->DoesMessageBoxExist(MSGCMD_GUILD_BATTLE_ACCEPT))
	{
		pUIManager->GetMessageBox(MSGCMD_GUILD_BATTLE_ACCEPT)->ReturnCommand(FALSE);
	}
	
	// 순간이동 요청을 받았을 경우 sessionState에서 처리 
	if(pUIManager->DoesMessageBoxExist(MSGCMD_WARP_TO_REQ_CALL))
	{
		pUIManager->GetMessageBox(MSGCMD_WARP_TO_REQ_CALL)->ReturnCommand(FALSE);
	}
	
	// 소환 요청을 받았을 경우 sessionState에서 처리
	if(pUIManager->DoesMessageBoxExist(MSGCMD_WARP_TAKE_REQ_CALL))
	{
		pUIManager->GetMessageBox(MSGCMD_WARP_TAKE_REQ_CALL)->ReturnCommand(FALSE);
	}
	
	// 공성 지역 이동 확인
	if(pUIManager->DoesMessageBoxExist(MSGCMD_SIEGE_WARFARE_MOVEING_CONFIRM))
	{
		pUIManager->GetMessageBox(MSGCMD_SIEGE_WARFARE_MOVEING_CONFIRM)->ReturnCommand(FALSE);
	}
	
	// 후견인으로 등록 확인
	if(pUIManager->DoesMessageBoxExist(MSGCMD_HELPER_STUDENT_ACCEPT))
	{
		pUIManager->GetMessageBox(MSGCMD_HELPER_STUDENT_ACCEPT)->ReturnCommand(FALSE);
	}
	
	// go to royal rumble zone.
	if(pUIManager->DoesMessageBoxExist(MSGCMD_ROYALRUMBLE_GO_ZONE))
	{
		pUIManager->GetMessageBox(MSGCMD_ROYALRUMBLE_GO_ZONE)->ReturnCommand(FALSE);
	}
}

void CUISocketSystem::_CheckUiItemslot()// 슬롯 업데이트
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	int nJeweluniIdx[JEWEL_COMPOS_JEWEL_MAX];

	for(int i = 0; i < JEWEL_COMPOS_JEWEL_MAX; i++)
	{
		nJeweluniIdx[i] = m_pIconsItem[i]->getItems()->Item_UniIndex;
		CItems* pItems = pUIManager->GetInventory()->GetItems(nJeweluniIdx[i]);

		if (pItems == NULL || pItems->IsEmptyItem() == TRUE)
		{
			nJeweluniIdx[i] = -1;
			m_pIconsItem[i]->clearIconData();
			continue;
		}

		m_pIconsItem[i]->setData(pItems);
	}

	int nTempCount = 0; // 동일 보석의 소모 총 갯수
	int nSameIndex = 0; // 동일한 보석의 고유 인덱스

	if ( nJeweluniIdx[JEWEL_COMPOS_BTN_JEWEL_1] == nJeweluniIdx[JEWEL_COMPOS_BTN_JEWEL_2] )
	{
		if (nJeweluniIdx[JEWEL_COMPOS_BTN_JEWEL_1] > 0)
		{
			nTempCount = 2;
			nSameIndex = nJeweluniIdx[JEWEL_COMPOS_BTN_JEWEL_1];

			if ( nJeweluniIdx[JEWEL_COMPOS_BTN_JEWEL_1] == nJeweluniIdx[JEWEL_COMPOS_BTN_JEWEL_3] )
				nTempCount = 3;

			if (pUIManager->GetInventory()->GetItemCountByUniIdx(nSameIndex) < nTempCount)
			{
				while (pUIManager->GetInventory()->GetItemCountByUniIdx(nSameIndex) >= nTempCount)
				{
					m_pIconsItem[nTempCount - 1]->clearIconData();
					nTempCount--;
				}
			}
		}
	}
	else if( nJeweluniIdx[JEWEL_COMPOS_BTN_JEWEL_3] == nJeweluniIdx[JEWEL_COMPOS_BTN_JEWEL_2] )
	{
		if (nJeweluniIdx[JEWEL_COMPOS_BTN_JEWEL_3] > 0)
		{
			nTempCount = 2;
			nSameIndex = nJeweluniIdx[JEWEL_COMPOS_BTN_JEWEL_3];
		
			if (pUIManager->GetInventory()->GetItemCountByUniIdx(nSameIndex) < nTempCount)
			{
				while (pUIManager->GetInventory()->GetItemCountByUniIdx(nSameIndex) >= nTempCount)
				{
					m_pIconsItem[nTempCount - 1]->clearIconData();
					nTempCount--;
				}
			}
		}
	}
	else if( nJeweluniIdx[JEWEL_COMPOS_BTN_JEWEL_1] == nJeweluniIdx[JEWEL_COMPOS_BTN_JEWEL_3] )
	{
		if (nJeweluniIdx[JEWEL_COMPOS_BTN_JEWEL_1] > 0)
		{
			nTempCount = 2;
			nSameIndex = nJeweluniIdx[JEWEL_COMPOS_BTN_JEWEL_1];
			
			if (pUIManager->GetInventory()->GetItemCountByUniIdx(nSameIndex) < nTempCount)
			{
				while (pUIManager->GetInventory()->GetItemCountByUniIdx(nSameIndex) >= nTempCount)
				{
					if (m_pIconsItem[nTempCount]->getIndex() == nSameIndex)
					{
						m_pIconsItem[nTempCount]->clearIconData();
					}
					nTempCount--;
				}
			}
		}
	}

	if (m_pIconsItem[JEWEL_COMPOS_BTN_SCROLL]->IsEmpty() == false)
	{
		int	nIndex = m_pIconsItem[JEWEL_COMPOS_BTN_SCROLL]->getIndex();
		int ItemIdx = m_pIconsItem[JEWEL_COMPOS_BTN_SCROLL]->getItems()->Item_UniIndex;

		if( pUIManager->GetInventory()->GetItemVisble(ItemIdx) )
		{
//			m_pIconsItem[JEWEL_COMPOS_BTN_SCROLL].Copy( pUIManager->GetInventory()->GetItemButtonEx(ItemIdx) );
			m_pIconsItem[JEWEL_COMPOS_BTN_SCROLL]->setCount(m_nOneMoreComposScrollpay);

			if (pUIManager->GetInventory()->GetItemCount(nIndex) < m_nOneMoreComposScrollpay)
			{
				m_pIconsItem[JEWEL_COMPOS_BTN_SCROLL]->clearIconData();
				m_nOneMoreComposScrollpay = 0;
			}
		}
		else
		{
			m_pIconsItem[JEWEL_COMPOS_BTN_SCROLL]->clearIconData();
			m_nOneMoreComposScrollpay = 0;
		}
	}
}

BOOL CUISocketSystem::_CheckErrorCompos(eJEWEL_COMPOS_TYPE composType) // 합성시 에러 체크
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	int nJeweluniIdx[JEWEL_COMPOS_JEWEL_MAX];

	for(int i = 0; i < JEWEL_COMPOS_JEWEL_MAX; i++)
	{
		if (m_pIconsItem[i]->IsEmpty() == true)
		{
			if (composType == GENARAL_JEWEL_COMPOS)
				_showErrorSystemMessage(JEWEL_COMPOS_ERROR_NOT_GENERAL_JEWEL);
			else
				_showErrorSystemMessage(JEWEL_COMPOS_ERROR_NOT_CHAOS_JEWEL);
			return FALSE;
		}
		else
		{
			if (pUIManager->GetInventory()->GetItemVisble(m_pIconsItem[i]->getItems()->Item_UniIndex) == TRUE)
			{
				nJeweluniIdx[i] = m_pIconsItem[i]->getItems()->Item_UniIndex;
			}
			else
			{
				_showErrorSystemMessage(JEWEL_COMPOS_ERROR_NOT_INVENTORY);
				return FALSE;
			}
		}
	}
	int nTempCount = 0; // 동일 보석의 소모 총 갯수
	int nSameIndex = 0; // 동일한 보석의 고유 인덱스

	if ( nJeweluniIdx[JEWEL_COMPOS_BTN_JEWEL_1] == nJeweluniIdx[JEWEL_COMPOS_BTN_JEWEL_2] )
	{
		nTempCount = 2;
		nSameIndex = nJeweluniIdx[JEWEL_COMPOS_BTN_JEWEL_1];

		if ( nJeweluniIdx[JEWEL_COMPOS_BTN_JEWEL_1] == nJeweluniIdx[JEWEL_COMPOS_BTN_JEWEL_3] )
			nTempCount = 3;

		if (pUIManager->GetInventory()->GetItemCountByUniIdx(nSameIndex) < nTempCount)
		{
			_showErrorSystemMessage(JEWEL_COMPOS_ERROR_JEWEL_LACK);
			return FALSE;
		}
	}
	else if( nJeweluniIdx[JEWEL_COMPOS_BTN_JEWEL_3] == nJeweluniIdx[JEWEL_COMPOS_BTN_JEWEL_2] )
	{
		nTempCount = 2;
		nSameIndex = nJeweluniIdx[JEWEL_COMPOS_BTN_JEWEL_3];

		if (pUIManager->GetInventory()->GetItemCountByUniIdx(nSameIndex) < nTempCount)
		{
			_showErrorSystemMessage(JEWEL_COMPOS_ERROR_JEWEL_LACK);
			return FALSE;
		}
	}
	else if( nJeweluniIdx[JEWEL_COMPOS_BTN_JEWEL_1] == nJeweluniIdx[JEWEL_COMPOS_BTN_JEWEL_3] )
	{
		nTempCount = 2;
		nSameIndex = nJeweluniIdx[JEWEL_COMPOS_BTN_JEWEL_1];
		
		if (pUIManager->GetInventory()->GetItemCountByUniIdx(nSameIndex) < nTempCount)
		{
			_showErrorSystemMessage(JEWEL_COMPOS_ERROR_JEWEL_LACK);
			return FALSE;
		}
	}

	if ( _pNetwork->MyCharacterInfo.money < m_sqNeedMoney )
	{
		_showErrorSystemMessage(JEWEL_COMPOS_ERROR_NAS_LACK);
		return FALSE;
	}

	if (m_pIconsItem[JEWEL_COMPOS_BTN_SCROLL]->IsEmpty() == false)
	{
		if (pUIManager->GetInventory()->GetItemVisble(m_pIconsItem[JEWEL_COMPOS_BTN_SCROLL]->getItems()->Item_UniIndex) == FALSE)
		{
			_showErrorSystemMessage(JEWEL_COMPOS_ERROR_COMPOS_SCROLL);
			return FALSE;
		}
	}
	
	if( pUIManager->GetInventory()->IsInventoryFull() )
	{
		_showErrorSystemMessage(JEWEL_COMPOS_ERROR_NOT_INVENTORY_EMPTY);
		return FALSE;
	}	

	return TRUE;
}

void CUISocketSystem::_SetJewelcomposNas()
{
	int i, tempNas = 0;
	int JewelGrade = 0;
	bool bChaos = false;

	for (i = 0; i < JEWEL_COMPOS_JEWEL_MAX; i++)
	{
		if (m_pIconsItem[i]->IsEmpty() == true)
		{
			tempNas = 0;
			JewelGrade = 0;
			break;
		}		
		CItemData* pItemData = m_pIconsItem[i]->getItems()->ItemData;
		JewelGrade += pItemData->GetNum0();
		
		if (pItemData->GetSubType() == CItemData::ITEM_ETC_CHAOSJEWEL)
			bChaos = true;
		else
			bChaos = false;
	}

	if (JewelGrade > 0)
	{
		tempNas = _pNetwork->GetJewelComposNas(JewelGrade / JEWEL_COMPOS_JEWEL_MAX, bChaos);
	}
	m_sqNeedMoney = tempNas;
}

BOOL CUISocketSystem::_CheckOneMoreComposScrollpay()
{
	if (m_pIconsItem[JEWEL_COMPOS_BTN_SCROLL]->IsEmpty() == true)
	{
		return FALSE;
	}
	else
	{
		if (m_nOneMoreComposScrollpay > 0)
		{
			return TRUE;
		}
	}
	return FALSE;
}

void CUISocketSystem::_setActionCheck()
{
	if ( m_pIconsItem[JEWEL_COMPOS_BTN_JEWEL_1]->IsEmpty() ||
		m_pIconsItem[JEWEL_COMPOS_BTN_JEWEL_2]->IsEmpty() ||
		m_pIconsItem[JEWEL_COMPOS_BTN_JEWEL_3]->IsEmpty() )
	{
		m_btnConfirm.SetEnable(FALSE);
		m_eJewelActionState = JEWEL_COMPOS_ACTION_IDLE;
	}
	else
	{
		if ( _pNetwork->MyCharacterInfo.money >= m_sqNeedMoney )
		{
			m_btnConfirm.SetEnable(TRUE);
			m_eJewelActionState = JEWEL_COMPOS_ACTION_READY;
			m_colorJewelcomposNasStr = 0xFFFFFFFF;
		}
		else
		{
			m_btnConfirm.SetEnable(FALSE);
			m_eJewelActionState = JEWEL_COMPOS_ACTION_NAS_LACK;
			m_colorJewelcomposNasStr = 0xFF0000FF;
		}		
	}
}

void CUISocketSystem::_SetButtonLock(BOOL bOnOff, SBYTE OpenPos)
{
	if(m_pIconsItem[0]->IsEmpty())
		return;
	
	CItems* pItem = m_pIconsItem[0]->getItems();

	if ( pItem->ItemData->IsFlag(ITEM_FLAG_SOCKET) )
	{			
		for( int i = 1; i < ITEM_MAX_COUNT - 1; ++i )
		{			
			LONG jewelindex = pItem->GetSocketJewelIndex(i - 1);
			
			if ( jewelindex == 0)
			{
				if (bOnOff)
				{
					if (OpenPos == i)
						continue;

					m_bItemButtonLock[i] = TRUE;
				}
				else
				{
					m_bItemButtonLock[i] = FALSE;
				}
			}
		}
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CUISocketSystem::_calcCmdTimer()
{
	if( m_bUseTimer )
	{
		__int64 tmpTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();
		if( tmpTime - m_uStartTime > ( m_uEndTime * 1000 ) )
		{
			(this->*m_fnReq)();
			m_bUseTimer = FALSE;
		}
		else 
			m_pbBar.OffsetProgressPos( (tmpTime - m_uStartTime) ); 
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CUISocketSystem::_setCreateInfo()
{

	_setUIRect(STAT_CREATE);
	_setUIUVRect(STAT_CREATE);

	m_btnConfirm.SetText( _S( 4961, "생성") );
	m_btnConfirm.SetPos( 40, 225 );
	m_btnClose.SetPos(SOCKET_UI_BTN_CLOSE_POS_X, SOCKET_UI_BTN_CLOSE_POS_Y);
	m_btnConfirm.SetEnable( FALSE );
	
	for( int i = 0; i < ITEM_MAX_COUNT; ++i )
		m_pIconsItem[i]->clearIconData();

	m_pIconsItem[0]->SetPos( SOCKET_UI_FRAME_BORDER + 22, SOCKET_UI_FRAME_TITLE + SOCKET_UI_FRAME_BORDER + 6 );
	m_pIconsItem[1]->SetPos( SOCKET_UI_FRAME_BORDER + 106, SOCKET_UI_FRAME_TITLE + SOCKET_UI_FRAME_BORDER + 6 );
	m_pIconsItem[2]->SetPos( SOCKET_UI_FRAME_BORDER + 64, SOCKET_UI_FRAME_TITLE + SOCKET_UI_FRAME_BORDER + 77 );
	m_pIconsItem[3]->SetPos( 0, 0 );
	m_pIconsItem[4]->SetPos( 0, 0 );
	
	m_pbBar.SetPos(SOCKET_UI_FRAME_BORDER, 251);
	//m_pbBar.SetIdleText( _S( 4962, "아이템을 올려주세요."), FALSE );
	m_pbBar.SetDoingText( _S( 4963, "생성중"), TRUE );
	m_pbBar.SetCompleteText( _S( 4964, "생성 완료"), TRUE );
	// bug fix. [6/16/2010 rumist]
	m_pbBar.IdleProgress();
	m_pbBar.SetProgressPos( 0 );
	m_sqNeedMoney = 0;

	// SOCKET_SYSTEM_S2 소켓 생성 칼라 [4/1/2013 Ranma]
	m_ColorStr[0] = 0x1C54CCFF;
	m_ColorStr[1] = 0x20A51CFF;
	m_ColorStr[2] = 0xE5E230FF;
	m_ColorStr[3] = 0xB2FDB7FF;
	m_ColorStr[4] = 0x01FDEEFF;
	m_ColorStr[5] = 0xCC66FFFF;
	m_ColorStr[6] = 0xFFFFFFFF;

	m_bActionStatus = FALSE;

	this->m_fnRender = &CUISocketSystem::_createRender;
	this->m_fnMsgProc = &CUISocketSystem::_createMouseMessage;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CUISocketSystem::_setCombineInfo()
{
	_setUIRect(STAT_COMBINE);
	_setUIUVRect(STAT_COMBINE);
	
	_CreateEmptyGuideCombinePopup();

	m_bDefendScroll = FALSE;

	m_btnConfirm.SetText( _S( 4965, "결합") );
	m_btnClose.SetPos(SOCKET_UI_BTN_CLOSE_POS_X, SOCKET_UI_BTN_CLOSE_POS_Y);
	//결합 버튼
	m_btnConfirm.SetPos( 40, 300 );
	m_pbBar.SetPos(SOCKET_UI_FRAME_BORDER, EX_SOCKET_UI_FRAME_TITLE  + EX_COMBINE_UI_INNERFRAME_HEIGHT + 97 );
	m_btnConfirm.SetEnable( FALSE );

	for( int i = 0; i < ITEM_MAX_COUNT; ++i )
	{
		m_pIconsItem[i]->clearIconData();
	}

	m_pIconsItem[0]->SetPos( 70, 261 ); // 장비
	m_pIconsItem[1]->SetPos( 70, 106 ); // 카오스 보석
	m_pIconsItem[2]->SetPos( 70, 51 ); // 보석1
	m_pIconsItem[3]->SetPos( 119, 78 ); // 2
	m_pIconsItem[4]->SetPos( 119, 133 ); // 3
	m_pIconsItem[5]->SetPos( 70, 161 ); // 4
	m_pIconsItem[6]->SetPos( 21, 133 ); // 5
	m_pIconsItem[7]->SetPos( 21, 78 ); // 6
	m_pIconsItem[8]->SetPos( 124, 218 ); // 보석 보호 스크롤

	//m_pbBar.SetIdleText( _S( 4962, "아이템을 올려주세요."), FALSE );
	m_pbBar.SetDoingText( _S( 4966, "결합중"), TRUE );
	m_pbBar.SetCompleteText( _S( 4967, "결합 완료"), TRUE );

	// bug fix. [6/16/2010 rumist]
	m_pbBar.IdleProgress();
	m_pbBar.SetProgressPos( 0 );
	m_sqNeedMoney = 0;
	m_bActionStatus = SOCKET_CREATE_IDLE;

	this->m_fnRender = &CUISocketSystem::_combineRender;
	this->m_fnMsgProc = &CUISocketSystem::_combineMouseMessage;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CUISocketSystem::_setEmptyOnceInfo()
{
	_setUIRect(STAT_ONCEEMPTY);
	_setUIUVRect(STAT_ONCEEMPTY);

	m_nSelCleanItemID = -1;
	m_btnConfirm.SetText( _S( 148, "준비완료") );
	// 결합 , 나스, 결합 경고, 프로그래스바 위치 셋팅
	//결합 버튼
	m_btnConfirm.SetPos( 40, 280 );
	m_btnConfirm.SetEnable( FALSE );
	m_btnClose.SetPos(SOCKET_UI_BTN_CLOSE_POS_X, SOCKET_UI_BTN_CLOSE_POS_Y);
	
	for( int i = 0; i < ITEM_MAX_COUNT; ++i )
		m_pIconsItem[i]->clearIconData();
	
	// 소켓 결합 버튼 위치 셋업
	m_pIconsItem[0]->SetPos( 70, 234 ); // 장비
	m_pIconsItem[1]->SetPos( 70, 106 ); // 카오스 보석
	m_pIconsItem[2]->SetPos( 70, 51 ); // 1
	m_pIconsItem[3]->SetPos( 119, 78 ); // 2
	m_pIconsItem[4]->SetPos( 119, 133 ); // 3
	m_pIconsItem[5]->SetPos( 70, 161 ); // 4
	m_pIconsItem[6]->SetPos( 21, 133 ); // 5
	m_pIconsItem[7]->SetPos( 21, 78 ); // 6
	
	// bug fix. [6/16/2010 rumist]
	m_pbBar.IdleProgress();
	m_sqNeedMoney = 0;
	
	this->m_fnRender = &CUISocketSystem::_OnceemptyRender;
	this->m_fnMsgProc = &CUISocketSystem::_OnceemptyMouseMessage;
}

void CUISocketSystem::_OnceEmptyMessageBox()
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	if( pUIManager->DoesMessageBoxExist( MSGLCMD_SOCKET_SYSTEM_EMPTY_BOX ) ) return;
	
// 	int nTab, nIndex;
// 	pUIManager->GetInventory()->GetUseItemSlotInfo(nTab, nIndex);

	CUIMsgBox_Info	MsgBoxInfo;
	CTString		strMessage[MAX_MSGSTRING];
	strMessage[0] = _S( 5897, "보석 개별 제거" );
	strMessage[1] = _S( 5899, "보석 제거 작업에 필요한 마법 스크롤을 올려주세요." );

	MsgBoxInfo.SetMsgBoxInfo( strMessage[0], UMBS_YESNO | UMBS_BUTTONEX, UI_NONE, MSGLCMD_SOCKET_SYSTEM_EMPTY_BOX );
	
	MsgBoxInfo.SetBtnType( UBET_ITEM, CItemData::ITEM_ONCEUSE, CItemData::ITEM_SUB_PROCESS_DOC );
	MsgBoxInfo.AddString( strMessage[1] );
	if (pUIManager->CreateMessageBox( MsgBoxInfo ))
						pUIManager->GetSocketSystem()->CloseSocketSystem();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CUISocketSystem::_setEmptyInfo()
{
	_setUIRect( STAT_EMPTY, m_nOrgHeight - ( SOCKET_UI_FRAME_TITLE + SOCKET_UI_FRAME_BORDER*7+m_msgEmptyText.GetRenderHeight()+BTN_SIZE+2+22+SOCKET_UI_FRAME_BORDER ) );
	_setUIUVRect(STAT_EMPTY);

	m_btnConfirm.SetText( _S( 4971, "제거") );
	m_btnClose.SetPos(SOCKET_UI_BTN_CLOSE_POS_X, SOCKET_UI_BTN_CLOSE_POS_Y);
	m_btnConfirm.SetPos( 40, SOCKET_UI_FRAME_TITLE + SOCKET_UI_FRAME_BORDER*7+m_msgEmptyText.GetRenderHeight()+BTN_SIZE+2 );
	m_btnConfirm.SetEnable( FALSE );

	for( int i = 0; i < ITEM_MAX_COUNT; ++i )
		m_pIconsItem[i]->clearIconData();
	
	m_pIconsItem[0]->SetPos( (m_nWidth-BTN_SIZE)/2, SOCKET_UI_FRAME_TITLE + SOCKET_UI_FRAME_BORDER*3 + m_msgEmptyText.GetRenderHeight() + 1 );
	m_pIconsItem[1]->SetPos( 0, 0 );
	m_pIconsItem[2]->SetPos( 0, 0 );
	m_pIconsItem[3]->SetPos( 0, 0 );
	m_pIconsItem[4]->SetPos( 0, 0 );

	// bug fix. [6/16/2010 rumist]
//	m_pbBar.SetPos(SOCKET_UI_FRAME_BORDER, 251);
	m_pbBar.IdleProgress();
	m_sqNeedMoney = 0;

	this->m_fnRender = &CUISocketSystem::_emptyRender;
	this->m_fnMsgProc = &CUISocketSystem::_emptyMouseMessage;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CUISocketSystem::_setJewelcomposBtn()
{
	m_btnConfirm.SetText( _S( 5933, "합성") );
	m_btnConfirm.SetPos( JEWEL_COMPOS_BTN_CONFIRM_POS_X, JEWEL_COMPOS_BTN_CONFIRM_POS_Y);
	m_btnConfirm.SetEnable( FALSE );
	m_btnClose.SetPos(JEWEL_COMPOS_BTN_CLOSE_POS_X, JEWEL_COMPOS_BTN_CLOSE_POS_Y);
	
	for( int i = 0; i < ITEM_MAX_COUNT; ++i )
		m_pIconsItem[i]->clearIconData();
		
	m_pIconsItem[JEWEL_COMPOS_BTN_JEWEL_1]->SetPos( JEWEL_COMPOS_BTN_JEWEL1_POS_X , JEWEL_COMPOS_BTN_JEWEL1_POS_Y );
	m_pIconsItem[JEWEL_COMPOS_BTN_JEWEL_2]->SetPos( JEWEL_COMPOS_BTN_JEWEL2_POS_X, JEWEL_COMPOS_BTN_JEWEL2_POS_Y );
	m_pIconsItem[JEWEL_COMPOS_BTN_JEWEL_3]->SetPos( JEWEL_COMPOS_BTN_JEWEL3_POS_X, JEWEL_COMPOS_BTN_JEWEL3_POS_Y );
	m_pIconsItem[JEWEL_COMPOS_BTN_SCROLL]->SetPos( JEWEL_COMPOS_BTN_SCROLL_POS_X, JEWEL_COMPOS_BTN_SCROLL_POS_Y );
	m_pIconsItem[JEWEL_COMPOS_BTN_JEWEL_RESULT]->SetPos( JEWEL_COMPOS_BTN_RESULT_POS_X, JEWEL_COMPOS_BTN_RESULT_POS_Y );
}

void CUISocketSystem::_setJewelcomposInfo()
{
	_setJewelcomposBtn();
	m_sqNeedMoney = 0;
	m_nOneMoreComposScrollpay = 0;
	m_DirectionInfo.clear();
	m_dwStartTime = 0;
	m_bStartDirection = FALSE;
	m_bBtnTranslucency = FALSE;
	m_DirectionType = 0;
	_setUIRect(STAT_JEWEL_COMPOS);
	_setUIUVRect(STAT_JEWEL_COMPOS);

	_CreateEmptyGuidePopup(GENARAL_JEWEL_COMPOS);

	this->m_fnRender = &CUISocketSystem::_JewelRender;
	this->m_fnMsgProc = &CUISocketSystem::_JewelMouseMessage;
}

void CUISocketSystem::_setChaosJewelcomposInfo()
{
	_setJewelcomposBtn();	
	m_sqNeedMoney = 0;
	m_nOneMoreComposScrollpay = 0;
	m_DirectionInfo.clear();
	m_dwStartTime = 0;
	m_bStartDirection = FALSE;
	m_bBtnTranslucency = FALSE;
	m_DirectionType = 0;
	_setUIRect(STAT_CHAOSJEWEL_COMPOS);
	_setUIUVRect(STAT_CHAOSJEWEL_COMPOS);

	_CreateEmptyGuidePopup(CHAOS_JEWEL_COMPOS);
	
	this->m_fnRender = &CUISocketSystem::_ChaosJewelRender;
	this->m_fnMsgProc = &CUISocketSystem::_ChaosJewelMouseMessage;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CUISocketSystem::_createRender()
{
	int nX, nY, i;
	GetAbsPos( nX, nY );

	CUIManager* pUIManager = CUIManager::getSingleton();
	CDrawPort* pDrawPort = pUIManager->GetDrawPort();

	pDrawPort->InitTextureData( m_ptdBaseTexture );

	pDrawPort->AddTexture( nX+m_rcTop.Left, nY+m_rcTop.Top, nX+m_rcTop.Right, nY+m_rcTop.Bottom,
										m_rtTop.U0, m_rtTop.V0, m_rtTop.U1, m_rtTop.V1, 0xFFFFFFFF );
	pDrawPort->AddTexture( nX+m_rcMiddle.Left, nY+m_rcMiddle.Top, nX+m_rcMiddle.Right, nY+m_rcMiddle.Bottom,
										m_rtMiddle.U0, m_rtMiddle.V0, m_rtMiddle.U1, m_rtMiddle.V1, 0xFFFFFFFF );
	pDrawPort->AddTexture( nX+m_rcBottom.Left, nY+m_rcBottom.Top, nX+m_rcBottom.Right, nY+m_rcBottom.Bottom,
										m_rtBottom.U0, m_rtBottom.V0, m_rtBottom.U1, m_rtBottom.V1, 0xFFFFFFFF );

	pDrawPort->FlushRenderingQueue();
	// button render.
	pDrawPort->InitTextureData(m_ptdButtonTexture);
	m_btnClose.Render();
	m_btnConfirm.Render();

	pDrawPort->FlushRenderingQueue();
	// Flush all render text queue
	pDrawPort->EndTextEx();

	// text setting
	CTString strTemp;
	COLOR	 colStrTemp;

	strTemp.PrintF( _S(	4973, "소켓 생성") );
	pDrawPort->PutTextExCX(strTemp, m_nPosX+SOCKET_UI_FRAME_WIDTH/2, m_nPosY+18, 0xDED9A0FF);
	pDrawPort->EndTextEx();

	pDrawPort->InitTextureData( m_ptdBaseTexture );
	m_rtsCreate.SetPos( m_nPosX + SOCKET_UI_FRAME_BORDER, m_nPosY + SOCKET_UI_FRAME_TITLE + SOCKET_UI_FRAME_BORDER );
	m_rtsCreate.RenderRectSurface( pDrawPort, 0xFFFFFFFF );

	pDrawPort->FlushRenderingQueue();

	pDrawPort->InitTextureData(m_ptdButtonTexture);
	// Need money.

	m_rtsNeedMoney.SetPos( m_nPosX + SOCKET_UI_FRAME_BORDER, m_nPosY + SOCKET_UI_FRAME_TITLE + SOCKET_UI_FRAME_BORDER * 3 + SOCKET_UI_INNERFRAME_HEIGHT + 22 );
	m_rtsNeedMoney.RenderRectSurface( pDrawPort, 0xFFFFFFFF );
	pDrawPort->FlushRenderingQueue();
	
	if( m_bActionStatus == SOCKET_CREATE_SUCCESS ) 
	{
		int nSocketCount = m_pIconsItem[2]->getItems()->Item_SocketCount;
		strTemp.PrintF( _S( 4974, "%d SOCKET SUCCESS!!"),  nSocketCount );
		colStrTemp = m_ColorStr[nSocketCount - 2]; // 소캣 생성은 최소 2부터 시작이므로 [4/1/2013 Ranma]
	}
	else if( m_bActionStatus == SOCKET_CREATE_FAIL )
	{
 		strTemp.PrintF( _S( 4975, "소켓 추가 실패") );
 		colStrTemp = 0xFF0000FF;
	}
	else 
	{
		if (m_bProcessing)
		{
			strTemp.PrintF( CTString("") );
		}
		else if (m_pIconsItem[0]->IsEmpty())
		{
			strTemp.PrintF( _S( 4962, "아이템을 올려주세요.") );
			
		}
		else if (m_pIconsItem[1]->IsEmpty())
		{
			strTemp.PrintF( _S( 4984, "스크롤을 올려주세요.") );
		}
		else
		{
			strTemp.PrintF( _S( 5914, "생성 버튼을 눌러주세요.") );
		}

		colStrTemp = 0xFFFFFFFF;
	}

	pDrawPort->PutTextEx(	strTemp, m_nPosX+SOCKET_UI_FRAME_BORDER*2, 
										m_nPosY + SOCKET_UI_FRAME_TITLE + SOCKET_UI_FRAME_BORDER * 3 + SOCKET_UI_INNERFRAME_HEIGHT + 4, colStrTemp);

	strTemp.PrintF( "%I64d", m_sqNeedMoney );
	pUIManager->InsertCommaToString(strTemp);
	pDrawPort->PutTextExRX(	strTemp, m_nPosX+SOCKET_UI_FRAME_WIDTH-SOCKET_UI_FRAME_BORDER*2, 
											m_nPosY + SOCKET_UI_FRAME_TITLE + SOCKET_UI_FRAME_BORDER * 3 + SOCKET_UI_INNERFRAME_HEIGHT + 26, 0xFFFFFFFF);

	pDrawPort->EndTextEx();
	
	m_pbBar.Render();
	pDrawPort->FlushRenderingQueue();

	for( i = 0; i < 3; ++i )
	{
		if (m_pIconsItem[i]->IsEmpty())
			continue;

		m_pIconsItem[i]->Render(pDrawPort);
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CUISocketSystem::_combineRender()
{
	int nX, nY, i;
	GetAbsPos( nX, nY );

	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();
	CUIManager* pUIManager = CUIManager::getSingleton();

	pDrawPort->InitTextureData( m_ptdSocketCombineTexture );

	pDrawPort->AddTexture( nX+m_rcTop.Left, nY+m_rcTop.Top, nX+m_rcTop.Right, nY+m_rcTop.Bottom,
										m_rtTop.U0, m_rtTop.V0, m_rtTop.U1, m_rtTop.V1, 0xFFFFFFFF );
	pDrawPort->AddTexture( nX+m_rcMiddle.Left, nY+m_rcMiddle.Top, nX+m_rcMiddle.Right, nY+m_rcMiddle.Bottom,
										m_rtMiddle.U0, m_rtMiddle.V0, m_rtMiddle.U1, m_rtMiddle.V1, 0xFFFFFFFF );
	pDrawPort->AddTexture( nX+m_rcBottom.Left, nY+m_rcBottom.Top, nX+m_rcBottom.Right, nY+m_rcBottom.Bottom,
										m_rtBottom.U0, m_rtBottom.V0, m_rtBottom.U1, m_rtBottom.V1, 0xFFFFFFFF );

	pDrawPort->FlushRenderingQueue();
	// button render.
	pDrawPort->InitTextureData(m_ptdButtonTexture);
	m_btnClose.Render();
	m_btnConfirm.Render();

	pDrawPort->FlushRenderingQueue();
	// Flush all render text queue
	pDrawPort->EndTextEx();
	// text setting
	CTString strTemp;
	strTemp.PrintF( _S( 4976, "보석 결합") );
	pDrawPort->PutTextExCX(strTemp, m_nPosX+SOCKET_UI_FRAME_WIDTH/2, m_nPosY+18, 0xDED9A0FF);
	pDrawPort->EndTextEx();

	pDrawPort->InitTextureData( m_ptdSocketCombineTexture );
	m_rtsCombine.SetPos( m_nPosX + SOCKET_UI_FRAME_BORDER, m_nPosY + EX_SOCKET_UI_FRAME_TITLE);
	m_rtsCombine.RenderRectSurface( pDrawPort, 0xFFFFFFFF );

	pDrawPort->FlushRenderingQueue();

	pDrawPort->InitTextureData( m_ptdButtonTexture );

	// Need money. SOCKET_SYSTEM_S2 돈 배경 그리는 곳
	m_rtsNeedMoney.SetPos( m_nPosX + SOCKET_UI_FRAME_BORDER, m_nPosY + EX_SOCKET_UI_FRAME_TITLE  + EX_COMBINE_UI_INNERFRAME_HEIGHT + 70 );
	m_rtsNeedMoney.RenderRectSurface( pDrawPort, 0xFFFFFFFF );
	pDrawPort->FlushRenderingQueue();

	// 돈 수치 랜더
	strTemp.PrintF( "%I64d", m_sqNeedMoney );
	pUIManager->InsertCommaToString(strTemp);
	pDrawPort->PutTextExRX(	strTemp, m_nPosX+EX_SOCKET_UI_FRAME_WIDTH-SOCKET_UI_FRAME_BORDER*2, 
		m_nPosY + EX_SOCKET_UI_FRAME_TITLE  + EX_COMBINE_UI_INNERFRAME_HEIGHT + 74, 0xFFFFFFFF);
	
	pDrawPort->EndTextEx();
	// 프로그레스 바 랜더 결합 부분 [4/4/2013 Ranma]
	m_pbBar.Render();

	pDrawPort->FlushRenderingQueue();

	for( i = 0; i < ITEM_MAX_COUNT; ++i )
	{
		if (m_pIconsItem[i]->IsEmpty())
			continue;

		m_pIconsItem[i]->Render(pDrawPort);
	}

	pDrawPort->InitTextureData( m_ptdSocketCombineTexture );

	// text setting
	COLOR	 colStrTemp;
	bool	bCombineWarning = false;

	if (m_bProcessing)
		strTemp.PrintF( CTString("") );

	if (m_pIconsItem[0]->IsEmpty())
	{
		for( i = 1; i < ITEM_MAX_COUNT - 1; ++i )
		{
			m_rtsCombineSocket.m_RectSurfaceArray[ESocketInactive].m_RT = UIRect(m_nPosX + m_pIconsItem[i]->GetPosX() - 1,m_nPosY + m_pIconsItem[i]->GetPosY() - 1,
			m_nPosX + m_pIconsItem[i]->GetPosX() + 33, m_nPosY + m_pIconsItem[i]->GetPosY() + 33);
			m_rtsCombineSocket.RenderRectSurface(pDrawPort, 0xFFFFFFFF, ESocketInactive);
		}

		if (!m_bProcessing)
			strTemp.PrintF( _S( 4962, "아이템을 올려주세요.") );
	}
	else
	{
		CItems* pItems = m_pIconsItem[0]->getItems();
	
		if (!m_bProcessing)
			strTemp.PrintF( _S( 5917, "결합 할 보석을 올려주세요.") );		

		if ( pItems->ItemData != NULL && pItems->ItemData->IsFlag(ITEM_FLAG_SOCKET) )
		{			
			for( i = 1; i < ITEM_MAX_COUNT - 1; ++i )
			{
				
				LONG jewelindex = pItems->GetSocketJewelIndex(i - 1);
				
				if ( jewelindex < 0 )
				{
					m_bButtonLock[i] = TRUE;
					m_rtsCombineSocket.m_RectSurfaceArray[ESocketInactive].m_RT = UIRect(m_nPosX + m_pIconsItem[i]->GetPosX() - 1,m_nPosY + m_pIconsItem[i]->GetPosY() - 1,
						m_nPosX + m_pIconsItem[i]->GetPosX() + 33, m_nPosY + m_pIconsItem[i]->GetPosY() + 33);
					m_rtsCombineSocket.RenderRectSurface(pDrawPort, 0xFFFFFFFF, ESocketInactive);
				}
				else if ( jewelindex == 0)
				{
					m_bButtonLock[i] = FALSE;
				}
				else
				{
					m_pIconsItem[i]->setData(UBET_ITEM, jewelindex);
					m_bButtonLock[i] = TRUE;
					
					m_rtsCombineSocket.m_RectSurfaceArray[ESocketLock].m_RT = UIRect(m_nPosX + m_pIconsItem[i]->GetPosX() - 4,m_nPosY + m_pIconsItem[i]->GetPosY() - 2,
						m_nPosX + m_pIconsItem[i]->GetPosX() + 36, m_nPosY + m_pIconsItem[i]->GetPosY() + 34);
					m_rtsCombineSocket.RenderRectSurface(pDrawPort, 0xFFFFFFFF, ESocketLock);
				}
				
				if (m_bItemButtonLock[i])
				{
					m_rtsCombineSocket.m_RectSurfaceArray[ESocketInactive].m_RT = UIRect(m_nPosX + m_pIconsItem[i]->GetPosX() - 1,m_nPosY + m_pIconsItem[i]->GetPosY() - 1,
						m_nPosX + m_pIconsItem[i]->GetPosX() + 33, m_nPosY + m_pIconsItem[i]->GetPosY() + 33);
					m_rtsCombineSocket.RenderRectSurface(pDrawPort, 0xFFFFFFFF, ESocketInactive);
				}
			}
		}
		
		for ( i = 1; i < ITEM_MAX_COUNT - 1; i++ )
		{
			if(m_bButtonLock[i])
				continue;

			if (m_pIconsItem[i]->IsEmpty() == false)
			{
				if (!m_bProcessing)
					strTemp= CTString("");

				bCombineWarning = true;
				continue;
			}
		}
		
	}

	pDrawPort->FlushRenderingQueue();

	colStrTemp = 0xFFFFFFFF;

	m_msgCombineSuccess.SetRenderPos(m_nPosX, m_nPosY + EX_SOCKET_UI_FRAME_TITLE  + EX_COMBINE_UI_INNERFRAME_HEIGHT + 25);
	m_msgCombineFail.SetRenderPos(m_nPosX, m_nPosY + EX_SOCKET_UI_FRAME_TITLE  + EX_COMBINE_UI_INNERFRAME_HEIGHT + 25);
	m_msgCombineFail2.SetRenderPos(m_nPosX, m_nPosY + EX_SOCKET_UI_FRAME_TITLE  + EX_COMBINE_UI_INNERFRAME_HEIGHT + 25);
	m_msgCombineWarning.SetRenderPos(m_nPosX, m_nPosY + EX_SOCKET_UI_FRAME_TITLE  + EX_COMBINE_UI_INNERFRAME_HEIGHT + 25);
	
	// 결합 가이드 랜더
	if (m_bActionStatus == SOCKET_CREATE_SUCCESS)
	{
		m_msgCombineSuccess.Render();
	}
	else if (m_bActionStatus == SOCKET_CREATE_FAIL)
	{
		if (m_bDefendScroll == TRUE)
			m_msgCombineFail2.Render();
		else
			m_msgCombineFail.Render();
	}
	else
	{
		if (bCombineWarning)
			m_msgCombineWarning.Render();
		else
			pDrawPort->PutTextEx( strTemp, m_nPosX+SOCKET_UI_FRAME_BORDER, 
				m_nPosY + EX_SOCKET_UI_FRAME_TITLE  + EX_COMBINE_UI_INNERFRAME_HEIGHT + 30, colStrTemp);
	}
	pDrawPort->EndTextEx();
	pDrawPort->InitTextureData( m_ptdButtonTexture );

	_RenderEmptyGuideCombine();
	pDrawPort->FlushBtnRenderingQueue( UBET_ITEM );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CUISocketSystem::_upgradeRender()
{
	int nX, nY, i;
	GetAbsPos( nX, nY );

	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	pDrawPort->InitTextureData( m_ptdBaseTexture );

	pDrawPort->AddTexture( nX+m_rcTop.Left, nY+m_rcTop.Top, nX+m_rcTop.Right, nY+m_rcTop.Bottom,
										m_rtTop.U0, m_rtTop.V0, m_rtTop.U1, m_rtTop.V1, 0xFFFFFFFF );
	pDrawPort->AddTexture( nX+m_rcMiddle.Left, nY+m_rcMiddle.Top, nX+m_rcMiddle.Right, nY+m_rcMiddle.Bottom,
										m_rtMiddle.U0, m_rtMiddle.V0, m_rtMiddle.U1, m_rtMiddle.V1, 0xFFFFFFFF );
	pDrawPort->AddTexture( nX+m_rcBottom.Left, nY+m_rcBottom.Top, nX+m_rcBottom.Right, nY+m_rcBottom.Bottom,
										m_rtBottom.U0, m_rtBottom.V0, m_rtBottom.U1, m_rtBottom.V1, 0xFFFFFFFF );

	pDrawPort->FlushRenderingQueue();
	// button render.
	pDrawPort->InitTextureData(m_ptdButtonTexture);
	m_btnClose.Render();
	m_btnConfirm.Render();

	pDrawPort->FlushRenderingQueue();
	// Flush all render text queue
	pDrawPort->EndTextEx();

	// text setting
	CTString strTemp;
	strTemp.PrintF( _S( 4977, "보석 강화") );
	pDrawPort->PutTextExCX(strTemp, m_nPosX+SOCKET_UI_FRAME_WIDTH/2, m_nPosY+18, 0xDED9A0FF);
	pDrawPort->EndTextEx();
	
	pDrawPort->InitTextureData( m_ptdBaseTexture );
	m_rtsUpgrade.SetPos( m_nPosX + SOCKET_UI_FRAME_BORDER, m_nPosY + SOCKET_UI_FRAME_TITLE + SOCKET_UI_FRAME_BORDER );
	m_rtsUpgrade.RenderRectSurface( pDrawPort, 0xFFFFFFFF );
	m_rtsWarning.SetPos( m_nPosX + SOCKET_UI_FRAME_BORDER, m_nPosY + SOCKET_UI_FRAME_TITLE + SOCKET_UI_FRAME_BORDER+2 + SOCKET_UI_INNERFRAME_HEIGHT );
	m_rtsWarning.RenderRectSurface( pDrawPort, 0xFFFFFFFF );

	pDrawPort->FlushRenderingQueue();

	pDrawPort->InitTextureData( m_ptdButtonTexture );

	// warning message.
 	m_msgUpgradeText.SetRenderPos(	m_nPosX, 
									m_nPosY + SOCKET_UI_FRAME_TITLE + SOCKET_UI_FRAME_BORDER + SOCKET_UI_INNERFRAME_HEIGHT );
 	m_msgUpgradeText.Render();

	pDrawPort->FlushRenderingQueue();

	m_pbBar.Render();

	for( i = 0; i < 4; ++i )
	{
		if (m_pIconsItem[i]->IsEmpty() == true)
			continue;

		m_pIconsItem[i]->Render(pDrawPort);
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CUISocketSystem::_emptyRender()
{
	int nX, nY, i;
	GetAbsPos( nX, nY );

	CUIManager* pUIManager = CUIManager::getSingleton();
	CDrawPort* pDrawPort = pUIManager->GetDrawPort();

	pDrawPort->InitTextureData( m_ptdBaseTexture );

	pDrawPort->AddTexture( nX+m_rcTop.Left, nY+m_rcTop.Top, nX+m_rcTop.Right, nY+m_rcTop.Bottom,
										m_rtTop.U0, m_rtTop.V0, m_rtTop.U1, m_rtTop.V1, 0xFFFFFFFF );
	pDrawPort->AddTexture( nX+m_rcMiddle.Left, nY+m_rcMiddle.Top, nX+m_rcMiddle.Right, nY+m_rcMiddle.Bottom,
										m_rtMiddle.U0, m_rtMiddle.V0, m_rtMiddle.U1, m_rtMiddle.V1, 0xFFFFFFFF );
	pDrawPort->AddTexture( nX+m_rcBottom.Left, nY+m_rcBottom.Top, nX+m_rcBottom.Right, nY+m_rcBottom.Bottom,
										m_rtBottom.U0, m_rtBottom.V0, m_rtBottom.U1, m_rtBottom.V1, 0xFFFFFFFF );

	pDrawPort->FlushRenderingQueue();
	// button render.
	pDrawPort->InitTextureData(m_ptdButtonTexture);
	m_btnClose.Render();
	m_btnConfirm.Render();

	pDrawPort->FlushRenderingQueue();
	// Flush all render text queue
	pDrawPort->EndTextEx();

	// text setting
	CTString strTemp;
 	strTemp.PrintF( _S( 4978, "보석 제거") );
 	pDrawPort->PutTextExCX(strTemp, m_nPosX+SOCKET_UI_FRAME_WIDTH/2, m_nPosY+18, 0xDED9A0FF); 	
	pDrawPort->EndTextEx();

	pDrawPort->InitTextureData( m_ptdBaseTexture );
	pDrawPort->AddTexture( m_nPosX+(m_nWidth-BTN_SIZE)/2-1, 
										m_nPosY+SOCKET_UI_FRAME_TITLE + SOCKET_UI_FRAME_BORDER*3+m_msgEmptyText.GetRenderHeight(), 
										m_nPosX+(m_nWidth-BTN_SIZE)/2+BTN_SIZE+1, 
										m_nPosY+SOCKET_UI_FRAME_TITLE + SOCKET_UI_FRAME_BORDER*3+m_msgEmptyText.GetRenderHeight()+BTN_SIZE+2,
										m_rtItemSlot.U0, m_rtItemSlot.V0, m_rtItemSlot.U1, m_rtItemSlot.V1, 0xFFFFFFFF );
	pDrawPort->FlushRenderingQueue();

	pDrawPort->InitTextureData( m_ptdButtonTexture );

	// warning message.
  	m_msgEmptyText.SetRenderPos(	m_nPosX, 
 									m_nPosY + SOCKET_UI_FRAME_TITLE + SOCKET_UI_FRAME_BORDER );
  	m_msgEmptyText.Render();
	CTString sqMoney;
	sqMoney.PrintF( "%I64d", m_sqNeedMoney );
	pUIManager->InsertCommaToString( sqMoney );
	strTemp.PrintF( "%s:%s", _S( 4979, "제거 비용"), sqMoney );
	pDrawPort->PutTextExCX(	strTemp, m_nPosX+m_nWidth/2, 
											m_nPosY + SOCKET_UI_FRAME_TITLE + SOCKET_UI_FRAME_BORDER * 4+m_msgEmptyText.GetRenderHeight()+BTN_SIZE+2, 
											0xFFFFFFFF);

	pDrawPort->EndTextEx();

	pDrawPort->FlushRenderingQueue();

	for( i = 0; i < 1; ++i )
	{
		if( m_pIconsItem[i]->IsEmpty() )
			continue;

		m_pIconsItem[i]->Render(pDrawPort);
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CUISocketSystem::_OnceemptyRender()
{
	int nX, nY, i;
	GetAbsPos( nX, nY );

	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();
	CUIManager* pUIManager = CUIManager::getSingleton();
	pDrawPort->InitTextureData( m_ptdSocketOnceEmptyTexture );

	pDrawPort->AddTexture( nX+m_rcTop.Left, nY+m_rcTop.Top, nX+m_rcTop.Right, nY+m_rcTop.Bottom,
										m_rtTop.U0, m_rtTop.V0, m_rtTop.U1, m_rtTop.V1, 0xFFFFFFFF );
	pDrawPort->AddTexture( nX+m_rcMiddle.Left, nY+m_rcMiddle.Top, nX+m_rcMiddle.Right, nY+m_rcMiddle.Bottom,
										m_rtMiddle.U0, m_rtMiddle.V0, m_rtMiddle.U1, m_rtMiddle.V1, 0xFFFFFFFF );
	pDrawPort->AddTexture( nX+m_rcBottom.Left, nY+m_rcBottom.Top, nX+m_rcBottom.Right, nY+m_rcBottom.Bottom,
										m_rtBottom.U0, m_rtBottom.V0, m_rtBottom.U1, m_rtBottom.V1, 0xFFFFFFFF );

	pDrawPort->FlushRenderingQueue();
	// button render.
	pDrawPort->InitTextureData(m_ptdButtonTexture);
	m_btnClose.Render();
	m_btnConfirm.Render();

	pDrawPort->FlushRenderingQueue();
	// Flush all render text queue
	pDrawPort->EndTextEx();
	// text setting
	CTString strTemp;
	bool bOnceMsg = true;
	strTemp.PrintF( _S( 5897, "보석 개별 제거") );
	pDrawPort->PutTextExCX(strTemp, m_nPosX+SOCKET_UI_FRAME_WIDTH/2, m_nPosY+18, 0xDED9A0FF);
	pDrawPort->EndTextEx();

	pDrawPort->InitTextureData( m_ptdSocketOnceEmptyTexture );
	m_rtsOnceEmpty.SetPos( m_nPosX + SOCKET_UI_FRAME_BORDER, m_nPosY + EX_SOCKET_UI_FRAME_TITLE);
	m_rtsOnceEmpty.RenderRectSurface( pDrawPort, 0xFFFFFFFF );

	pDrawPort->FlushRenderingQueue();
	
	pDrawPort->InitTextureData( m_ptdButtonTexture );

	// Need money. SOCKET_SYSTEM_S2 돈 배경 그리는 곳
	m_rtsNeedMoney.SetPos( m_nPosX + SOCKET_UI_FRAME_BORDER, m_nPosY + EX_SOCKET_UI_FRAME_TITLE  + EX_SOCKET_UI_INNERFRAME_HEIGHT + 90 );
	m_rtsNeedMoney.RenderRectSurface( pDrawPort, 0xFFFFFFFF );
	pDrawPort->FlushRenderingQueue();

	// 돈 수치 랜더
	strTemp.PrintF( "%I64d", m_sqNeedMoney );
	pUIManager->InsertCommaToString(strTemp);
	pDrawPort->PutTextExRX(	strTemp, m_nPosX+EX_SOCKET_UI_FRAME_WIDTH-SOCKET_UI_FRAME_BORDER*2, 
		m_nPosY + EX_SOCKET_UI_FRAME_TITLE  + EX_SOCKET_UI_INNERFRAME_HEIGHT + 94, 0xFFFFFFFF);
	
	pDrawPort->EndTextEx();
	pDrawPort->FlushRenderingQueue();

	for( i = 0; i < ITEM_MAX_COUNT - 1; ++i )
	{
		if( m_pIconsItem[i]->IsEmpty() )
			continue;

		m_pIconsItem[i]->Render(pDrawPort);
	}

	pDrawPort->InitTextureData( m_ptdSocketOnceEmptyTexture );

	if (m_pIconsItem[0]->IsEmpty())
	{
		for( i = 1; i < ITEM_MAX_COUNT - 1; ++i )
		{
			m_rtsCombineSocket.m_RectSurfaceArray[ESocketInactive].m_RT = UIRect(m_nPosX + m_pIconsItem[i]->GetPosX() - 1,m_nPosY + m_pIconsItem[i]->GetPosY() - 1,
			m_nPosX + m_pIconsItem[i]->GetPosX() + 33, m_nPosY + m_pIconsItem[i]->GetPosY() + 33);
			m_rtsCombineSocket.RenderRectSurface(pDrawPort, 0xFFFFFFFF, ESocketInactive);
		}
	}
	else
	{
		CItems* pItems = m_pIconsItem[0]->getItems();

		if ( pItems->ItemData->IsFlag(ITEM_FLAG_SOCKET) )
		{			
			for( i = 1; i < ITEM_MAX_COUNT - 1; ++i )
			{
				
				LONG jewelindex = pItems->GetSocketJewelIndex(i - 1);
				
				if ( jewelindex <= 0 )
				{
					m_rtsCombineSocket.m_RectSurfaceArray[ESocketInactive].m_RT = UIRect(m_nPosX + m_pIconsItem[i]->GetPosX() - 1,m_nPosY + m_pIconsItem[i]->GetPosY() - 1,
						m_nPosX + m_pIconsItem[i]->GetPosX() + 33, m_nPosY + m_pIconsItem[i]->GetPosY() + 33);
					m_rtsCombineSocket.RenderRectSurface(pDrawPort, 0xFFFFFFFF, ESocketInactive);
				}
				else
				{
					m_pIconsItem[i]->setData(UBET_ITEM, jewelindex);

					if(m_nSelCleanItemID + 1 == i)
					{
						bOnceMsg = false;
						m_btnConfirm.SetEnable(TRUE);
					
						m_rtsCombineSocket.m_RectSurfaceArray[ESocketSelect].m_RT = UIRect(m_nPosX + m_pIconsItem[i]->GetPosX() - 5, m_nPosY + m_pIconsItem[i]->GetPosY() - 5,
							m_nPosX + m_pIconsItem[i]->GetPosX() + 37, m_nPosY + m_pIconsItem[i]->GetPosY() + 37);
						m_rtsCombineSocket.RenderRectSurface(pDrawPort, 0xFFFFFFFF, ESocketSelect);						
					}
					else if( m_nSelCleanItemID < 0)
					{
						m_nSelCleanItemID = i - 1;
					}
				}		
			}
		}
	}

	pDrawPort->FlushRenderingQueue();

	// 개별 제거 가이드 랜더
	m_msgOnceEmptyText.SetRenderPos(m_nPosX, m_nPosY + EX_SOCKET_UI_FRAME_TITLE  + EX_SOCKET_UI_INNERFRAME_HEIGHT + 40);
	if (bOnceMsg)
		m_msgOnceEmptyText.Render();
	pDrawPort->EndTextEx();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CUISocketSystem::_JewelRender()
{
	_InteractionMsgBoxReject(); // 상호 작용 막음

	int nX, nY;
	GetAbsPos( nX, nY );
	
	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	pDrawPort->InitTextureData( m_ptdSocketJewelComposTexture );
	
	pDrawPort->AddTexture( nX+m_rcTop.Left, nY+m_rcTop.Top, nX+m_rcTop.Right, nY+m_rcTop.Bottom,
		m_rtTop.U0, m_rtTop.V0, m_rtTop.U1, m_rtTop.V1, 0xFFFFFFFF );
	pDrawPort->AddTexture( nX+m_rcMiddle.Left, nY+m_rcMiddle.Top, nX+m_rcMiddle.Right, nY+m_rcMiddle.Bottom,
		m_rtMiddle.U0, m_rtMiddle.V0, m_rtMiddle.U1, m_rtMiddle.V1, 0xFFFFFFFF );
	pDrawPort->AddTexture( nX+m_rcBottom.Left, nY+m_rcBottom.Top, nX+m_rcBottom.Right, nY+m_rcBottom.Bottom,
		m_rtBottom.U0, m_rtBottom.V0, m_rtBottom.U1, m_rtBottom.V1, 0xFFFFFFFF );
	
	pDrawPort->FlushRenderingQueue();
	// button render.
	pDrawPort->InitTextureData(m_ptdButtonTexture);
	m_btnClose.Render();
	m_btnConfirm.Render();

	pDrawPort->FlushRenderingQueue();
	pDrawPort->EndTextEx();

	// text setting
	CTString strTemp;
	strTemp.PrintF( _S( 5927, "일반보석 합성") );
	pDrawPort->PutTextExCX(strTemp, m_nPosX+JEWEL_COMPOS_UI_FRAME_WIDTH/2, m_nPosY+18, 0xDED9A0FF);
	pDrawPort->EndTextEx();
	
	pDrawPort->InitTextureData( m_ptdSocketJewelComposTexture );
	m_rtsJewelCompos.SetPos( m_nPosX + JEWEL_COMPOS_UI_FRAME_BORDER_X, m_nPosY + JEWEL_COMPOS_UI_FRAME_BORDER_Y );
	m_rtsJewelCompos.RenderRectSurface( pDrawPort, 0xFFFFFFFF );
	pDrawPort->FlushRenderingQueue();

	switch(m_eJewelActionState)
	{
	case JEWEL_COMPOS_ACTION_IDLE:
		m_msgJewelcomposIdle.SetRenderPos(m_nPosX + JEWEL_COMPOS_MSG_POS_X, m_nPosY + JEWEL_COMPOS_MSG_POS_Y);
		m_msgJewelcomposIdle.Render();
		break;	
		
	case JEWEL_COMPOS_ACTION_NAS_LACK:
		m_msgJewelcomposNaslack.SetRenderPos(m_nPosX + JEWEL_COMPOS_MSG_POS_X, m_nPosY + JEWEL_COMPOS_MSG_POS_Y);
		m_msgJewelcomposNaslack.Render();
		break;
		
	case JEWEL_COMPOS_ACTION_READY:
		m_msgJewelcomposReady.SetRenderPos(m_nPosX + JEWEL_COMPOS_MSG_POS_X, m_nPosY + JEWEL_COMPOS_MSG_POS_Y);
		m_msgJewelcomposReady.Render();
		break;
	}

	// 돈 수치 랜더
	CUIManager* pUIManager = CUIManager::getSingleton();
	strTemp.PrintF( "%I64d", m_sqNeedMoney );
	pUIManager->InsertCommaToString( strTemp );
	pDrawPort->PutTextExRX(	strTemp, m_nPosX + JEWEL_COMPOS_NAS_POS_X,
		m_nPosY + JEWEL_COMPOS_NAS_POS_Y, m_colorJewelcomposNasStr);
	
	pDrawPort->FlushRenderingQueue();
	pDrawPort->EndTextEx();
	
	pDrawPort->InitTextureData( m_ptdButtonTexture );

	if (m_bStartDirection)
		_JewelComposDirection();

	int i;
	for( i = 0; i < JEWEL_COMPOS_BTN_JEWEL_MAX; ++i )
	{
		if (m_pIconsItem[i]->IsEmpty() == true)
			continue;
		
		m_pIconsItem[i]->Render(pDrawPort);
	}
	pDrawPort->FlushBtnRenderingQueue( UBET_ITEM );

	pDrawPort->InitTextureData( m_ptdSocketJewelComposTexture );
	_RenderResultJewel();
	pDrawPort->FlushRenderingQueue();
		
	if(m_bBtnTranslucency)
		_JewelRenderBtnTranslucency();

	_RenderEmptyGuidePopup();
	pDrawPort->FlushRenderingQueue();
	pDrawPort->EndTextEx();
}

void CUISocketSystem::_ChaosJewelRender()
{
	_InteractionMsgBoxReject(); // 상호 작용 막음
	
	int nX, nY;
	GetAbsPos( nX, nY );
	
	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();
	
	pDrawPort->InitTextureData( m_ptdSocketJewelComposTexture );
	
	pDrawPort->AddTexture( nX+m_rcTop.Left, nY+m_rcTop.Top, nX+m_rcTop.Right, nY+m_rcTop.Bottom,
		m_rtTop.U0, m_rtTop.V0, m_rtTop.U1, m_rtTop.V1, 0xFFFFFFFF );
	pDrawPort->AddTexture( nX+m_rcMiddle.Left, nY+m_rcMiddle.Top, nX+m_rcMiddle.Right, nY+m_rcMiddle.Bottom,
		m_rtMiddle.U0, m_rtMiddle.V0, m_rtMiddle.U1, m_rtMiddle.V1, 0xFFFFFFFF );
	pDrawPort->AddTexture( nX+m_rcBottom.Left, nY+m_rcBottom.Top, nX+m_rcBottom.Right, nY+m_rcBottom.Bottom,
		m_rtBottom.U0, m_rtBottom.V0, m_rtBottom.U1, m_rtBottom.V1, 0xFFFFFFFF );
	
	pDrawPort->FlushRenderingQueue();
	// button render.
	pDrawPort->InitTextureData(m_ptdButtonTexture);
	m_btnClose.Render();
	m_btnConfirm.Render();
	
	pDrawPort->FlushRenderingQueue();
	pDrawPort->EndTextEx();
	
	// text setting
	CTString strTemp;
	strTemp.PrintF( _S( 5928, "카오스보석 합성") );
	pDrawPort->PutTextExCX(strTemp, m_nPosX+JEWEL_COMPOS_UI_FRAME_WIDTH/2, m_nPosY+18, 0xDED9A0FF);
	pDrawPort->EndTextEx();
	
	pDrawPort->InitTextureData( m_ptdSocketJewelComposTexture );
	m_rtsChaosJewelCompos.SetPos( m_nPosX + JEWEL_COMPOS_UI_FRAME_BORDER_X, m_nPosY + JEWEL_COMPOS_UI_FRAME_BORDER_Y );
	m_rtsChaosJewelCompos.RenderRectSurface( pDrawPort, 0xFFFFFFFF );
	pDrawPort->FlushRenderingQueue();

	switch(m_eJewelActionState)
	{
	case JEWEL_COMPOS_ACTION_IDLE:
		m_msgChaosJewelcomposIdle.SetRenderPos(m_nPosX + JEWEL_COMPOS_MSG_POS_X, m_nPosY + JEWEL_COMPOS_MSG_POS_Y);
		m_msgChaosJewelcomposIdle.Render();
		break;	
		
	case JEWEL_COMPOS_ACTION_NAS_LACK:
		m_msgJewelcomposNaslack.SetRenderPos(m_nPosX + JEWEL_COMPOS_MSG_POS_X, m_nPosY + JEWEL_COMPOS_MSG_POS_Y);
		m_msgJewelcomposNaslack.Render();
		break;
		
	case JEWEL_COMPOS_ACTION_READY:
		m_msgJewelcomposReady.SetRenderPos(m_nPosX + JEWEL_COMPOS_MSG_POS_X, m_nPosY + JEWEL_COMPOS_MSG_POS_Y);
		m_msgJewelcomposReady.Render();
		break;
	}
	
	// 돈 수치 랜더
	CUIManager* pUIManager = CUIManager::getSingleton();
	strTemp.PrintF( "%I64d", m_sqNeedMoney );
	pUIManager->InsertCommaToString( strTemp );
	pDrawPort->PutTextExRX(	strTemp, m_nPosX + JEWEL_COMPOS_NAS_POS_X,
		m_nPosY + JEWEL_COMPOS_NAS_POS_Y, m_colorJewelcomposNasStr);
	
	pDrawPort->FlushRenderingQueue();
	pDrawPort->EndTextEx();
	
	pDrawPort->InitTextureData( m_ptdButtonTexture );
	
	if (m_bStartDirection)
		_JewelComposDirection();
	
	int i;
	for( i = 0; i < JEWEL_COMPOS_BTN_JEWEL_MAX; ++i )
	{
		if (m_pIconsItem[i]->IsEmpty() == true)
			continue;
		
		m_pIconsItem[i]->Render(pDrawPort);
	}
	pDrawPort->FlushBtnRenderingQueue( UBET_ITEM );
	
	pDrawPort->InitTextureData( m_ptdSocketJewelComposTexture );
	_RenderResultJewel();
	pDrawPort->FlushRenderingQueue();

	if(m_bBtnTranslucency)
		_JewelRenderBtnTranslucency();
		
	_RenderEmptyGuidePopup();
	pDrawPort->FlushRenderingQueue();
	pDrawPort->EndTextEx();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CUISocketSystem::_showErrorMessage( const _SOCKET_ERROR_TYPE type /*=SOCKET_ERROR_INEXACT*/)
{
	CUIMsgBox_Info MsgInfo;
	MsgInfo.SetMsgBoxInfo( _S( 191, "확인" ), UMBS_OK, UI_NONE, MSGCMD_NULL );
	if( type == SOCKET_ERROR_INEXACT )
		MsgInfo.AddString( _S( 4981, "올바른 아이템이 아닙니다. 확인 후 다시 시도해 주시기 바랍니다.") );
	else if( type == SOCKET_ERROR_EMPTY )
		MsgInfo.AddString( _S( 4982, "소켓 아이템을 먼저 장착한 후 다시 시도해 주시기 바랍니다.") );
	else if( type == SOCKET_ERROR_EXCEED )
		MsgInfo.AddString( _S( 4983, "소캣 개수를 초과하였습니다. 확인 후 다시 시도해 주시기 바랍니다.") );
	else if( type == SOCKET_ERROR_ITEM_LACK )
		MsgInfo.AddString( _S( 5011,"아이템 개수가 부족합니다. 아이템 개수를 확인해 주세요.") );
	else if( type == SOCKET_ERROR_JEWEL_OVERLAP )
		MsgInfo.AddString( _S( 5012, "같은 종류의 보석은 더 이상 결합할 수 없습니다.") );
	else if( type == SOCKET_ERROR_COMBINE_TERMS )
		MsgInfo.AddString( _S( 5924, "결합 조건에 맞지 않은 보석입니다.") );
	else if( type == SOCKET_ERROR_NOT_ENOUGH_INVEN_SLOT )
		MsgInfo.AddString( _S( 4237, "인벤토리의 공간이 부족 합니다. 인벤토리를 비운 후 다시 시도해주시기 바랍니다."));

	CUIManager::getSingleton()->CreateMessageBox( MsgInfo );
}

void CUISocketSystem::_showErrorSystemMessage( const _JEWEL_ERROR_TYPE type)
{
	CTString strMessage;
	
	if ( type == JEWEL_COMPOS_ERROR_NOT_GENERAL_JEWEL )
		strMessage.PrintF( _S( 5931, "일반보석을 등록하세요.") );
	else if (type == JEWEL_COMPOS_ERROR_NOT_CHAOS_JEWEL )
		strMessage.PrintF( _S( 5929, "카오스보석을 등록하세요.") );
	else if (type == JEWEL_COMPOS_ERROR_NOT_COMPOS_SCROLL)
		strMessage.PrintF( _S( 4346, "등록할 수 없는 아이템 입니다.") );
	else if (type == JEWEL_COMPOS_ERROR_NOT_INVENTORY)
		strMessage.PrintF( _S( 5934, "합성하는 보석이 없습니다.") );
	else if (type == JEWEL_COMPOS_ERROR_JEWEL_LACK)
		strMessage.PrintF( _S( 5935, "보석이 부족합니다.") );
	else if (type == JEWEL_COMPOS_ERROR_NAS_LACK)
		strMessage.PrintF( _S( 306, "나스가 부족합니다.") );
	else if (type == JEWEL_COMPOS_ERROR_COMPOS_SCROLL)
		strMessage.PrintF( _S( 5936, "보석 합성스크롤이 잘못 되었습니다.") );
	else if (type == JEWEL_COMPOS_ERROR_NOT_INVENTORY_EMPTY)
		strMessage.PrintF( _S( 265, "인벤토리 공간이 부족합니다.") );

	_pNetwork->ClientSystemMessage(strMessage, SYSMSG_ERROR);
}
// 선택 좌표가 일일이 다 다르므로 이런식으로 변경해서 쓰도록 함.
// 나중에 함수포인터 써서 동적 바인딩 되면 좋다.
// 근데 귀찮음.;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
WMSG_RESULT CUISocketSystem::_createMouseMessage(MSG *pMsg )
{
	WMSG_RESULT	wmsgResult;

	// Title bar
	static BOOL bTitleBarClick = FALSE;
	// Extended button clicked
	static BOOL	bLButtonDownInBtn = FALSE;

	// Mouse point
	static int	nOldX, nOldY;
	int	nX = LOWORD( pMsg->lParam );
	int	nY = HIWORD( pMsg->lParam );

	switch( pMsg->message )
	{
		case WM_MOUSEMOVE:
		{
			CUIManager* pUIManager = CUIManager::getSingleton();

			if( IsInside( nX, nY ) )
				pUIManager->SetMouseCursorInsideUIs();

			int	ndX = nX - nOldX;
			int	ndY = nY - nOldY;

			// Hold item button
			if (pUIManager->GetDragIcon() == NULL && bLButtonDownInBtn && 
				(pMsg->wParam & MK_LBUTTON) && (ndX != 0 || ndY != 0))
				{
					if( m_nSelSlotItemID >= 0 )
					{
						// Close message box of inventory
						pUIManager->CloseMessageBox( MSGCMD_DROPITEM );

						pUIManager->SetHoldBtn(m_pIconsItem[m_nSelSlotItemID]);
					}
					bLButtonDownInBtn = FALSE;
				}

			// Move UI window.
			if( m_bPickTitle && ( pMsg->wParam & MK_LBUTTON ) )
			{
				nOldX = nX;	nOldY = nY;

				Move( ndX, ndY );

				return WMSG_SUCCESS;
			}
			else if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else if( m_btnConfirm.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else
			{
				if( IsInsideRect( nX, nY, m_rcArea ) )
				{
					for( int i = 0; i < 3; ++i )
						m_pIconsItem[i]->MouseMessage( pMsg );
				}
				return WMSG_SUCCESS;
			}
		}
		break;
		case WM_LBUTTONDOWN:
		{
			if( IsInside( nX, nY ) )
			{
				CUIManager* pUIManager = CUIManager::getSingleton();
				nOldX = nX;		nOldY = nY;

				// Close button
				if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				else if( m_btnConfirm.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				else if( IsInsideRect( nX, nY, m_rcTitle ) )
				{
					m_bPickTitle = TRUE;
				}
				else if( IsInsideRect( nX, nY, m_rcArea ) )
				{
					for( int i = 0; i < 3; ++i )
						if( m_pIconsItem[i]->MouseMessage( pMsg ) != WMSG_FAIL )
						{
							if( m_bProcessing )
							{
								if( pUIManager->DoesMessageBoxExist( MSGCMD_NULL ) )
									return WMSG_SUCCESS;
								CUIMsgBox_Info MsgInfo;
								CTString strTemp;
								MsgInfo.SetMsgBoxInfo( _S( 191, "확인" ), UMBS_OK, UI_NONE, MSGCMD_NULL );
								strTemp.PrintF( _S( 5010, "%s 중에는 아이템을 옮길 수 없습니다."), _S( 4973, "소켓 생성" ) );
								MsgInfo.AddString( strTemp );
								pUIManager->CreateMessageBox( MsgInfo );
								return WMSG_SUCCESS;
							}	
							m_pbBar.IdleProgress();
							m_pbBar.SetProgressPos( 0 );
							m_nSelSlotItemID = i;

							bLButtonDownInBtn = TRUE;
							m_bActionStatus = SOCKET_CREATE_IDLE;
							pUIManager->RearrangeOrder( UI_SOCKETSYSTEM, TRUE );
							return WMSG_SUCCESS;
						}
				}

				pUIManager->RearrangeOrder( UI_SOCKETSYSTEM, TRUE );
				return WMSG_SUCCESS;
			}
		}
		break;
		case WM_LBUTTONUP:
		{
			CUIManager* pUIManager = CUIManager::getSingleton();
			m_bPickTitle = FALSE;

			if (pUIManager->GetDragIcon() == NULL)
			{
				if( !IsFocused() )
					return WMSG_FAIL;

				// Close button
				if( ( wmsgResult = m_btnClose.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
					{
						_close();
					}
					return WMSG_SUCCESS;
				}
				else if( ( wmsgResult = m_btnConfirm.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
					{
						if( m_pIconsItem[0]->IsEmpty() )
						{
							CUIMsgBox_Info MsgInfo;
							MsgInfo.SetMsgBoxInfo( _S( 191, "확인" ), UMBS_OK, UI_NONE, MSGCMD_NULL );
							MsgInfo.AddString( _S( 4962, "아이템을 올려주세요.") );
							pUIManager->CreateMessageBox( MsgInfo );
							return WMSG_FAIL;
						}
						if( m_pIconsItem[1]->IsEmpty() )
						{
							CUIMsgBox_Info MsgInfo;
							MsgInfo.SetMsgBoxInfo( _S( 191, "확인" ), UMBS_OK, UI_NONE, MSGCMD_NULL );
							MsgInfo.AddString( _S( 4984, "스크롤을 올려주세요.") );
							pUIManager->CreateMessageBox( MsgInfo );
							return WMSG_FAIL;
						}
						//SendCreateReq();
						// bug fix. [6/22/2010 rumist]
						m_btnConfirm.SetEnable( FALSE );
						CommandTimer( &CUISocketSystem::SendCreateReq );
						return WMSG_SUCCESS;
					}
				}
				else if( IsInsideRect( nX, nY, m_rcArea ) )
				{
				}
				//return WMSG_SUCCESS;
			}
			else
			{
				if( IsInside( nX, nY ) )
				{
					pUIManager->GetMessenger()->SetDragging( false );
					if( IsInsideRect( nX, nY, m_rcArea) )
					{
						CUIIcon* pDrag = pUIManager->GetDragIcon();

						CItems* pItems = pDrag->getItems();

						if (pItems == NULL)
							return WMSG_FAIL;

						if (pDrag->getBtnType() == UBET_ITEM )
						{
							m_pbBar.IdleProgress();
							m_pbBar.SetProgressPos( 0 );
							if( m_pIconsItem[0]->IsInside( nX, nY ) )
							{
								// 거래대행 불가								
								CItemData* pItemData = pItems->ItemData;
								
								if ((pItemData->IsFlag(ITEM_FLAG_SOCKET)) &&
									pItems->Item_Wearing < 0 && // 착용중인 아이템이 아니어야 한다.
									!((pItems->IsFlag(FLAG_ITEM_SOCKET)) || (pItems->Item_Flag & FLAG_ITEM_COMPOSITION)))
								{
									m_pIconsItem[0]->copyItem(pDrag);
									
									if( pItemData->IsFlag( ITEM_FLAG_LORD ) )
										m_sqNeedMoney = 0.0f;
									else
										m_sqNeedMoney = pItemData->GetPrice() * PRICE_CREATE;
									if (m_pIconsItem[1]->IsEmpty() == false)
										m_btnConfirm.SetEnable( TRUE );
								}
								else
								{
									_showErrorMessage();
								}	
							}// if( m_btnItemButton[0].IsInside( nX, nY ) )
							else if (m_pIconsItem[1]->IsInside(nX, nY))
							{
								CItemData* pItemData = pItems->ItemData;
								if( pItemData->GetType() == CItemData::ITEM_ETC && pItemData->GetSubType() == CItemData::ITEM_ETC_PROC_SCROLL )
								{
									m_pIconsItem[1]->cloneItem(pDrag);
									m_pIconsItem[1]->setCount(pDrag->getItems()->Item_Sum);

									if (m_pIconsItem[1]->IsEmpty() == false)
										m_btnConfirm.SetEnable( TRUE );

								}
								else
								{
									_showErrorMessage();
								}
							}// if( m_btnItemButton[1].IsInside( nX, nY ) )
						}
					}
					pUIManager->ResetHoldBtn();
					return WMSG_SUCCESS;
				}
				else if( m_nSelSlotItemID > -1 )
				{
					m_pIconsItem[m_nSelSlotItemID]->clearIconData();
					m_nSelSlotItemID = -1;
					m_btnConfirm.SetEnable( FALSE );
					pUIManager->ResetHoldBtn();
					return WMSG_SUCCESS;
				}
			}
			pUIManager->ResetHoldBtn();
		}
		break;
		case WM_LBUTTONDBLCLK:
		{
		}
		break;
		case WM_MOUSEWHEEL:
		{
		}
		break;
	}
	return WMSG_FAIL;
}

WMSG_RESULT CUISocketSystem::_combineMouseMessage(MSG *pMsg )
{
	WMSG_RESULT	wmsgResult;

	// Title bar
	static BOOL bTitleBarClick = FALSE;
	// Extended button clicked
	static BOOL	bLButtonDownInBtn = FALSE;

	// Mouse point
	static int	nOldX, nOldY;
	int	nX = LOWORD( pMsg->lParam );
	int	nY = HIWORD( pMsg->lParam );

	switch( pMsg->message )
	{
		case WM_MOUSEMOVE:
		{
			CUIManager* pUIManager = CUIManager::getSingleton();

			if( IsInside( nX, nY ) )
				pUIManager->SetMouseCursorInsideUIs();

			int	ndX = nX - nOldX;
			int	ndY = nY - nOldY;

			// Hold item button
			if (pUIManager->GetDragIcon() == NULL && bLButtonDownInBtn && 
				(pMsg->wParam& MK_LBUTTON) && (ndX != 0 || ndY != 0))
			{
				if( m_nSelSlotItemID >= 0 )
				{
					pUIManager->CloseMessageBox( MSGCMD_DROPITEM );

					if (!m_bButtonLock[m_nSelSlotItemID])
					{
						pUIManager->SetHoldBtn(m_pIconsItem[m_nSelSlotItemID]);
					}
				}
				bLButtonDownInBtn = FALSE;
			}

			// Move UI window.
			if( m_bPickTitle && ( pMsg->wParam & MK_LBUTTON ) )
			{
				nOldX = nX;	nOldY = nY;

				Move( ndX, ndY );

				return WMSG_SUCCESS;
			}
			else if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else if( m_btnConfirm.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else
			{
				if( IsInsideRect( nX, nY, m_rcArea ) )
				{
					for( int i = 0; i < ITEM_MAX_COUNT; ++i )
					{
						_CheckEmptyGuideCombine();

						if (m_bCombineEmptyBtnGuide == TRUE)
						{
							m_CombineEmptyBtnGuide.MouseMessage(pMsg);
						}
						
						m_pIconsItem[i]->MouseMessage( pMsg );
					}
				}
				return WMSG_SUCCESS;
			}
		}
		break;
		case WM_LBUTTONDOWN:
		{
			if( IsInside( nX, nY ) )
			{
				CUIManager* pUIManager = CUIManager::getSingleton();
				nOldX = nX;		nOldY = nY;

				m_bActionStatus = SOCKET_CREATE_IDLE;

				// Close button
				if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				else if( m_btnConfirm.MouseMessage( pMsg )!= WMSG_FAIL )
				{
					// Nothing
				}
				else if( IsInsideRect( nX, nY, m_rcTitle ) )
				{
					m_bPickTitle = TRUE;
				}
				else if( IsInsideRect( nX, nY, m_rcArea ) )
				{
					for( int i = 0; i < ITEM_MAX_COUNT; ++i )
						if( m_pIconsItem[i]->MouseMessage( pMsg ) != WMSG_FAIL )
						{	
							if( m_bProcessing )
							{
								if( pUIManager->DoesMessageBoxExist( MSGCMD_NULL ) )
									return WMSG_SUCCESS;
								CUIMsgBox_Info MsgInfo;
								CTString strTemp;
								MsgInfo.SetMsgBoxInfo( _S( 191, "확인" ), UMBS_OK, UI_NONE, MSGCMD_NULL );
								strTemp.PrintF( _S( 5010, "%s 중에는 아이템을 옮길 수 없습니다."), _S( 4976, "보석 결합" ) );
								MsgInfo.AddString( strTemp );
								pUIManager->CreateMessageBox( MsgInfo );
								return WMSG_SUCCESS;
							}

							if (m_bButtonLock[i])
								return WMSG_SUCCESS;
							
							m_pbBar.IdleProgress();
							m_pbBar.SetProgressPos( 0 );
							m_bActionStatus = SOCKET_CREATE_IDLE;
							m_nSelSlotItemID = i;
							bLButtonDownInBtn = TRUE;
							pUIManager->RearrangeOrder( UI_SOCKETSYSTEM, TRUE );
							return WMSG_SUCCESS;
						}
				}

				pUIManager->RearrangeOrder( UI_SOCKETSYSTEM, TRUE );
				return WMSG_SUCCESS;
			}
		}
		break;
		case WM_LBUTTONUP:
		{
			CUIManager* pUIManager = CUIManager::getSingleton();
			m_bPickTitle = FALSE;

			if (pUIManager->GetDragIcon() == NULL)
			{
				if( !IsFocused() )
					return WMSG_FAIL;

				// Close button
				if( ( wmsgResult = m_btnClose.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
					{
						_close();
					}
					return WMSG_SUCCESS;
				}
				else if( ( wmsgResult = m_btnConfirm.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
					{
						if( pUIManager->GetInventory()->IsInventoryFull() )
						{
							_showErrorSystemMessage(JEWEL_COMPOS_ERROR_NOT_INVENTORY_EMPTY);
							return WMSG_SUCCESS;
						}	
						CommandTimer( &CUISocketSystem::SendCombineReq );
						// bug fix. [6/22/2010 rumist]
						m_btnConfirm.SetEnable( FALSE );
						//SendCombineReq();
					}
				}
				else if( IsInsideRect( nX, nY, m_rcArea ) )
				{
					
				}
				//return WMSG_SUCCESS;
			}
			else
			{
				if( IsInside( nX, nY ) )
				{
					CUIIcon* pDrag = pUIManager->GetDragIcon();

					CItems* pItems = pDrag->getItems();

					if (pItems == NULL)
						return WMSG_FAIL;

					CItemData* pItemData = pItems->ItemData;

					pUIManager->GetMessenger()->SetDragging( false );
					if( IsInsideRect( nX, nY, m_rcArea) )
					{
						if (pDrag->getBtnType() == UBET_ITEM)
						{
							m_pbBar.IdleProgress();
							m_pbBar.SetProgressPos( 0 );
							if( m_pIconsItem[0]->IsInside( nX, nY ) )	// weapon & shield(armor)
							{
								// 거래대행 불가.								
								if ((pItemData->IsFlag( ITEM_FLAG_SOCKET )) &&
									pItems->Item_Wearing < 0 && // 착용중인 아이템이 아니어야 한다.
									(pItems->GetSocketCount() > 0) && 
									!(pItems->Item_Flag & FLAG_ITEM_COMPOSITION))
								{
									m_pIconsItem[0]->copyItem(pDrag);
									
									for (int i = 1; i < ITEM_MAX_COUNT - 1; i++)
									{
										m_pIconsItem[i]->clearIconData();
									}
									m_sqNeedMoney = 0.0f;
									_SetButtonLock(FALSE);
									m_btnConfirm.SetEnable( FALSE );
								}
								else
								{
									_showErrorMessage();
								}
							}
							else if (m_pIconsItem[8]->IsInside(nX, nY))    // 보석 합성 스크롤
							{
								

								if( pItemData->GetType() == CItemData::ITEM_ETC && 
									pItemData->GetSubType() == CItemData::ITEM_ETC_FUNCTIONS &&
									pItemData->GetItemIndex() == JEWEL_DFEND_SCROLL_INDEX )
								{
									m_pIconsItem[8]->copyItem(pDrag);
								}
								else
								{
									_showErrorMessage();
								}

							}
							else/* if( m_btnItemButton[1~7].IsInside( nX, nY ) )*/
							{
								for( int i = 1; i < ITEM_MAX_COUNT - 1; ++i )
								{
									if (m_pIconsItem[i]->IsInside(nX, nY))
									{
										if (m_bButtonLock[i])
											continue;
										if (m_bItemButtonLock[i])
											continue;

										if( m_pIconsItem[0]->IsEmpty() )
										{
											_showErrorMessage( SOCKET_ERROR_EMPTY );
										}
										else
										{
											BOOL bOverlap = FALSE;
											CItemData* pItemHoldBtn = pItems->ItemData;
											LONG HoldBtnOptionType = pItemHoldBtn->GetSocketOptionIndex();

											for( SBYTE sbJw = 1; sbJw < JEWEL_MAX_COUNT; ++sbJw )
											{	// 보석 옵션의 중복 여부 체크 sbJw 가 1부터인 이유는 카오스 보석과 일반보석의 옵션이 중복되기에 1부터 시작
												if ( i == 1)
													break;

												const LONG jwIdx = m_pIconsItem[0]->getItems()->GetSocketJewelIndex(sbJw);
												CItemData* pData = _pNetwork->GetItemData( jwIdx );
												LONG	equipOptionType = -1;

												if (jwIdx > 0)
													equipOptionType = pData->GetSocketOptionIndex();

												if (equipOptionType == HoldBtnOptionType)
												{
													bOverlap = TRUE;
													break;
												}
											}

											if( !bOverlap )
											{
												if( m_nSelSlotItemID > 0 )	// 무기가 올라올 수 있으므로.
												{
												}
												else if( pItemData->GetType() == CItemData::ITEM_ETC && (pItemData->GetSubType() == CItemData::ITEM_ETC_JEWEL ||
													pItemData->GetSubType() == CItemData::ITEM_ETC_CHAOSJEWEL))
												{
													if (i == 1)
													{
														if (pItemData->GetSubType() != CItemData::ITEM_ETC_CHAOSJEWEL)
														{
															_showErrorMessage(SOCKET_ERROR_COMBINE_TERMS);
															break;
														}

														m_sqNeedMoney = 1000000.0f;
													}
													else
													{
														if (pItemData->GetSubType() == CItemData::ITEM_ETC_CHAOSJEWEL)
														{
															_showErrorMessage(SOCKET_ERROR_COMBINE_TERMS);
															break;
														}

														m_sqNeedMoney = 100000.0f;
													}

													_SetButtonLock(TRUE, i);												
													
													m_pIconsItem[i]->cloneItem(pDrag);
													m_pIconsItem[i]->setCount(pDrag->getItems()->Item_Sum);

													if( m_pIconsItem[0]->IsEmpty() == FALSE )
														m_btnConfirm.SetEnable( TRUE );
												}
												else
												{
													_showErrorMessage();
												}
											}
											else
											{
												_showErrorMessage(SOCKET_ERROR_JEWEL_OVERLAP);
											}
										}
										break;									
									}
								}						
							}// if( m_btnItemButton[1~7].IsInside( nX, nY ) )
						}
					}
					m_nSelSlotItemID = -1;
					pUIManager->ResetHoldBtn();
					return WMSG_SUCCESS;
				}
				else if( m_nSelSlotItemID > -1 )
				{
					m_pIconsItem[m_nSelSlotItemID]->clearIconData();

					if (m_nSelSlotItemID != 8) // 스크롤을 비울때 어떤 영향도 미쳐선 안된다.
					{
						_SetButtonLock(FALSE);

						if( m_nSelSlotItemID == 0)
						{
							m_sqNeedMoney = 0.0f;
							for (int i = 1; i < ITEM_MAX_COUNT; i++)
							{
								m_pIconsItem[i]->clearIconData();
							}
							m_btnConfirm.SetEnable( FALSE );
						}
						else
						{
							m_sqNeedMoney = 0.0f;					

							if (m_pIconsItem[0]->IsEmpty())
								m_btnConfirm.SetEnable( FALSE );
							else
							{
								for (int i = 1; i < ITEM_MAX_COUNT - 1; i++)
								{
									if (m_bButtonLock[i])
										continue;
									if( !m_pIconsItem[i]->IsEmpty() )
									{
										m_btnConfirm.SetEnable( TRUE );
										break;
									}
									else
									{
										m_btnConfirm.SetEnable( FALSE );
									}
								}
							}
						}
					} // if (m_nSelSlotItemID != 8)

					m_nSelSlotItemID = -1;
					pUIManager->ResetHoldBtn();
					return WMSG_SUCCESS;
				} // else if( m_nSelSlotItemID > -1 )
			}
			pUIManager->ResetHoldBtn();
		}
		break;
		case WM_LBUTTONDBLCLK:
		{
		}
		break;
		case WM_MOUSEWHEEL:
		{
		}
		break;
	}
	return WMSG_FAIL;
}

WMSG_RESULT CUISocketSystem::_upgradeMouseMessage(MSG *pMsg )
{
	WMSG_RESULT	wmsgResult;

	// Title bar
	static BOOL bTitleBarClick = FALSE;
	// Extended button clicked
	static BOOL	bLButtonDownInBtn = FALSE;

	// Mouse point
	static int	nOldX, nOldY;
	int	nX = LOWORD( pMsg->lParam );
	int	nY = HIWORD( pMsg->lParam );

	switch( pMsg->message )
	{
		case WM_MOUSEMOVE:
		{
			CUIManager* pUIManager = CUIManager::getSingleton();

			if( IsInside( nX, nY ) )
				pUIManager->SetMouseCursorInsideUIs();

			int	ndX = nX - nOldX;
			int	ndY = nY - nOldY;

			// Hold item button
			if (pUIManager->GetDragIcon() == NULL && bLButtonDownInBtn && 
				(pMsg->wParam& MK_LBUTTON) && (ndX != 0 || ndY != 0))
				{
					if( m_nSelSlotItemID >= 0 )
					{
						// Close message box of inventory
						pUIManager->CloseMessageBox( MSGCMD_DROPITEM );

						pUIManager->SetHoldBtn(m_pIconsItem[m_nSelSlotItemID]);
					}
					bLButtonDownInBtn = FALSE;
				}

			// Move UI window.
			if( m_bPickTitle && ( pMsg->wParam & MK_LBUTTON ) )
			{
				nOldX = nX;	nOldY = nY;

				Move( ndX, ndY );

				return WMSG_SUCCESS;
			}
			else if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else if( m_btnConfirm.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else
			{
				if( IsInsideRect( nX, nY, m_rcArea ) )
				{
					for( int i = 0; i < 4; ++i )
						m_pIconsItem[i]->MouseMessage( pMsg );
				}
				return WMSG_SUCCESS;
			}
		}
		break;
		case WM_LBUTTONDOWN:
		{
			if( IsInside( nX, nY ) )
			{
				CUIManager* pUIManager = CUIManager::getSingleton();
				nOldX = nX;		nOldY = nY;

				// Close button
				if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				else if( m_btnConfirm.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				else if( IsInsideRect( nX, nY, m_rcTitle ) )
				{
					m_bPickTitle = TRUE;
				}
				else if( IsInsideRect( nX, nY, m_rcArea ) )
				{
					for( int i = 0; i < 4; ++i )
						if( m_pIconsItem[i]->MouseMessage( pMsg ) != WMSG_FAIL )
						{
							if( m_bProcessing )
							{
								if( pUIManager->DoesMessageBoxExist( MSGCMD_NULL ) )
									return WMSG_SUCCESS;
								CUIMsgBox_Info MsgInfo;
								CTString strTemp;
								MsgInfo.SetMsgBoxInfo( _S( 191, "확인" ), UMBS_OK, UI_NONE, MSGCMD_NULL );
								strTemp.PrintF( _S( 5010, "%s 중에는 아이템을 옮길 수 없습니다."), _S( 4977, "보석 강화" ) );
								MsgInfo.AddString( strTemp );
								pUIManager->CreateMessageBox( MsgInfo );
								return WMSG_SUCCESS;
							}	
							m_pbBar.IdleProgress();
							m_pbBar.SetProgressPos( 0 );
							m_nSelSlotItemID = i;
							bLButtonDownInBtn = TRUE;
							pUIManager->RearrangeOrder( UI_SOCKETSYSTEM, TRUE );
							return WMSG_SUCCESS;
						}
				}
				pUIManager->RearrangeOrder( UI_SOCKETSYSTEM, TRUE );
				return WMSG_SUCCESS;
			}
		}
		break;
		case WM_LBUTTONUP:
		{
			CUIManager* pUIManager = CUIManager::getSingleton();
			m_bPickTitle = FALSE;

			if (pUIManager->GetDragIcon() == NULL)
			{
				if( !IsFocused() )
					return WMSG_FAIL;

				// Close button
				if( ( wmsgResult = m_btnClose.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
					{
						_close();
					}
					return WMSG_SUCCESS;
				}
				else if( ( wmsgResult = m_btnConfirm.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
					{
						// bug fix. [6/22/2010 rumist]
						m_btnConfirm.SetEnable( FALSE );
						CommandTimer( &CUISocketSystem::SendUpgradeReq );
						//SendUpgradeReq();
					}
				}
				else if( IsInsideRect( nX, nY, m_rcArea ) )
				{
					// nothing to do.
				}
				//return WMSG_SUCCESS;
			}
			else
			{
				if( IsInside( nX, nY ) )
				{
					CUIIcon* pDrag = pUIManager->GetDragIcon();
					CItems* pItems = pDrag->getItems();

					if (pItems == NULL)
						return WMSG_FAIL;

					CItemData* pItemData = pItems->ItemData;

					pUIManager->GetMessenger()->SetDragging( false );
					if( IsInsideRect( nX, nY, m_rcArea) )
					{
						if (pDrag->getBtnType() == UBET_ITEM)
						{
							m_pbBar.IdleProgress();
							m_pbBar.SetProgressPos( 0 );
							if( m_pIconsItem[0]->IsInside( nX, nY ) )
							{
								if( m_nSelSlotItemID > -1 )	// right jewels pos.
								{
									//m_pIconsItem[0].Swap( m_pIconsItem[m_nSelSlotItemID] );
									m_nSelSlotItemID = -1;
								}
								else if (pItemData->GetType() == CItemData::ITEM_ETC && 
										 pItemData->GetSubType() == CItemData::ITEM_ETC_JEWEL)
								{
									if (!m_pIconsItem[1]->IsEmpty() &&
										(m_pIconsItem[1]->getItems()->Item_UniIndex == pItems->Item_UniIndex && 
										 pItems->Item_Sum < 2))
										_showErrorMessage(SOCKET_ERROR_ITEM_LACK);
									else
									{
										m_pIconsItem[0]->copyItem(pDrag);

										if( !( m_pIconsItem[1]->IsEmpty() || m_pIconsItem[2]->IsEmpty() ) )
											m_btnConfirm.SetEnable( TRUE );
									}
								}
								else
								{
									_showErrorMessage();
								}
							}// if( m_btnItemButton[0].IsInside( nX, nY ) )
							else if( m_pIconsItem[1]->IsInside( nX, nY ) )
							{
								if (m_pIconsItem[1]->getItems()->Item_UniIndex == pItems->Item_UniIndex)
									_showErrorMessage();
								
								if( m_nSelSlotItemID > -1 )
								{
									//m_pIconsItem[1].Swap( m_pIconsItem[m_nSelSlotItemID] );
									m_nSelSlotItemID = -1;
								}
								else if( pItemData->GetType() == CItemData::ITEM_ETC && pItemData->GetSubType() == CItemData::ITEM_ETC_JEWEL )
								{
									if (m_pIconsItem[0]->IsEmpty() == false &&
										(m_pIconsItem[0]->getItems()->Item_UniIndex == pItems->Item_UniIndex && 
										 pItems->Item_Sum < 2))
										_showErrorMessage(SOCKET_ERROR_ITEM_LACK);
									else
									{
										m_pIconsItem[1]->copyItem(pDrag);

										if( !( m_pIconsItem[0]->IsEmpty() || m_pIconsItem[2]->IsEmpty() ) )
											m_btnConfirm.SetEnable( TRUE );
									}
								}
								else
								{
									_showErrorMessage();
								}
							}// if( m_btnItemButton[1].IsInside( nX, nY ) )
							else if( m_pIconsItem[2]->IsInside( nX, nY ) )
							{								
								if( pItemData->GetType() == CItemData::ITEM_ETC && pItemData->GetSubType() == CItemData::ITEM_ETC_STABILIZER )
								{
									m_pIconsItem[2]->copyItem(pDrag);

									if( !( m_pIconsItem[0]->IsEmpty() || m_pIconsItem[1]->IsEmpty() ) )
										m_btnConfirm.SetEnable( TRUE );
								}
								else
								{
									_showErrorMessage();
								}
							}// if( m_btnItemButton[2].IsInside( nX, nY ) )
						}
					}
					pUIManager->ResetHoldBtn();
					return WMSG_SUCCESS;
				}
				else if( m_nSelSlotItemID > -1 )
				{
					m_pIconsItem[m_nSelSlotItemID]->clearIconData();
					m_btnConfirm.SetEnable( FALSE );
					m_nSelSlotItemID = -1;
					pUIManager->ResetHoldBtn();
					return WMSG_SUCCESS;
				}
			}
			pUIManager->ResetHoldBtn();
		}
		break;
		case WM_LBUTTONDBLCLK:
		{
		}
		break;
		case WM_MOUSEWHEEL:
		{
		}
		break;
	}
	return WMSG_FAIL;
}

WMSG_RESULT CUISocketSystem::_OnceemptyMouseMessage(MSG *pMsg )
{
	WMSG_RESULT	wmsgResult;
	
	// Title bar
	static BOOL bTitleBarClick = FALSE;
	// Extended button clicked
	static BOOL	bLButtonDownInBtn = FALSE;
	
	// Mouse point
	static int	nOldX, nOldY;
	int	nX = LOWORD( pMsg->lParam );
	int	nY = HIWORD( pMsg->lParam );
	
	switch( pMsg->message )
	{
	case WM_MOUSEMOVE:
		{
			CUIManager* pUIManager = CUIManager::getSingleton();
			
			if( IsInside( nX, nY ) )
				pUIManager->SetMouseCursorInsideUIs();
			
			int	ndX = nX - nOldX;
			int	ndY = nY - nOldY;
			
			// Hold item button
			if (pUIManager->GetDragIcon() == NULL && bLButtonDownInBtn && 
				(pMsg->wParam& MK_LBUTTON) && (ndX != 0 || ndY != 0))
			{
				if( m_nSelSlotItemID >= 0 )
				{
					// Close message box of inventory
					pUIManager->CloseMessageBox( MSGCMD_DROPITEM );
					
					pUIManager->SetHoldBtn(m_pIconsItem[m_nSelSlotItemID]);
				}
				bLButtonDownInBtn = FALSE;
			}
			
			// Move UI window.
			if( m_bPickTitle && ( pMsg->wParam & MK_LBUTTON ) )
			{
				nOldX = nX;	nOldY = nY;
				
				Move( ndX, ndY );
				
				return WMSG_SUCCESS;
			}
			else if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else if( m_btnConfirm.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else
			{
				if( IsInsideRect( nX, nY, m_rcArea ) )
				{
					for( int i = 0; i < ITEM_MAX_COUNT; ++i )
					{
						m_pIconsItem[i]->MouseMessage( pMsg );
					}
				}
				return WMSG_SUCCESS;
			}
		}
		break;
	case WM_LBUTTONDOWN:
		{
			if( IsInside( nX, nY ) )
			{
				CUIManager* pUIManager = CUIManager::getSingleton();
				nOldX = nX;		nOldY = nY;				
				
				// Close button
				if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				else if( m_btnConfirm.MouseMessage( pMsg) != WMSG_FAIL )
				{
					// Nothing
				}
				else if( IsInsideRect( nX, nY, m_rcTitle ) )
				{
					m_bPickTitle = TRUE;
				}
				else if( IsInsideRect( nX, nY, m_rcArea ) )
				{
					if( m_pIconsItem[0]->MouseMessage( pMsg ) != WMSG_FAIL )
					{	
						m_nSelSlotItemID = 0;
						bLButtonDownInBtn = TRUE;
						pUIManager->RearrangeOrder( UI_SOCKETSYSTEM, TRUE );
						return WMSG_SUCCESS;
					}
					
				}
				pUIManager->RearrangeOrder( UI_SOCKETSYSTEM, TRUE );
				return WMSG_SUCCESS;
			}
		}
		break;
	case WM_LBUTTONUP:
		{
			CUIManager* pUIManager = CUIManager::getSingleton();
			m_bPickTitle = FALSE;
			
			if (pUIManager->GetDragIcon() == NULL)
			{
				if( !IsFocused() )
					return WMSG_FAIL;
				
				// Close button
				if( ( wmsgResult = m_btnClose.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
					{
						_close();
					}
					return WMSG_SUCCESS;
				}
				else if( ( wmsgResult = m_btnConfirm.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
					{
						m_btnConfirm.SetEnable( FALSE );
						m_pIconEmpty->clearIconData();
						m_pIconEmpty->copyItem(m_pIconsItem[0]);
						m_sqNeedMoney = 0.0f;
						m_nEmptyJewelPos = m_nSelCleanItemID;
						if( IsEmptyInvenSlotCount() > 0 )
							_OnceEmptyMessageBox();
						else
							_showErrorMessage( SOCKET_ERROR_NOT_ENOUGH_INVEN_SLOT );
 						//SendCleanOneJewelReq(m_nSelCleanItemID);
					}
				}
				else if( IsInsideRect( nX, nY, m_rcArea ) )
				{
					if( m_pIconsItem[0]->IsInside( nX, nY ) == FALSE )
					{
						for( int i = 1; i < ITEM_MAX_COUNT - 1; ++i )
						{
							//if( m_btnItemButton[1].IsEmpty() )
							if( m_pIconsItem[i]->IsInside( nX, nY ) )	
							{
								m_nSelCleanItemID = i - 1;
								m_btnConfirm.SetEnable(FALSE);
								m_sqNeedMoney = 0.0f;						
							}
						}
					}
				}
				else
				{
					if (m_nSelCleanItemID >= 0 &&
						m_pIconsItem[m_nSelCleanItemID]->IsEmpty() == true)
					{
						m_nSelCleanItemID = -1;
						m_sqNeedMoney = 0.0f;
						m_btnConfirm.SetEnable(FALSE);	
					}
				}
				//return WMSG_SUCCESS;
			}
			else
			{
				if( IsInside( nX, nY ) )
				{
					CUIIcon* pDrag = pUIManager->GetDragIcon();
					CItems* pItems = pDrag->getItems();

					if (pItems == NULL)
						return WMSG_FAIL;

					pUIManager->GetMessenger()->SetDragging( false );
					if( IsInsideRect( nX, nY, m_rcArea) )
					{
						if (pDrag->getBtnType() == UBET_ITEM)
						{
							if (m_pIconsItem[0]->IsInside(nX, nY))
							{
								CItemData* pItemData = pItems->ItemData;

								int i, jewelcount = 0;
								
								for (i = 0; i < JEWEL_MAX_COUNT; i++)
								{
									if (0 < pItems->GetSocketJewelIndex(i))
										jewelcount++;
								}

								// if item combined jewels.
								if (( pItemData->IsFlag( ITEM_FLAG_SOCKET )) &&
									pItems->Item_Wearing < 0 && // 착용중인 아이템이 아니어야 한다.
									(pItems->GetSocketCount() > 0) && jewelcount > 0)
								{									
									m_pIconsItem[0]->copyItem(pDrag);

									for (i = 1; i < ITEM_MAX_COUNT - 1; i++)
									{
										m_pIconsItem[i]->clearIconData();
									}
									m_sqNeedMoney = 0.0f;
									m_btnConfirm.SetEnable( FALSE );
								}
								else
								{
									_showErrorMessage();
								}	
							}
							
						}
					}
					pUIManager->ResetHoldBtn();
					return WMSG_SUCCESS;
				}
				else if( m_nSelSlotItemID > -1 )
				{
					m_pIconsItem[m_nSelSlotItemID]->clearIconData();
					int i;
					if (m_nSelSlotItemID == 0)
					{
						for (i = 1; i < ITEM_MAX_COUNT - 1; i++)
						{
							m_pIconsItem[i]->clearIconData();
						}
					}					
					m_btnConfirm.SetEnable( FALSE );
					m_nSelSlotItemID = -1;
					m_nSelCleanItemID = -1;
					m_sqNeedMoney = 0.0f;
					pUIManager->ResetHoldBtn();
					return WMSG_SUCCESS;
				}
			}
			pUIManager->ResetHoldBtn();
		}
		break;
	case WM_LBUTTONDBLCLK:
		{
		}
		break;
	case WM_MOUSEWHEEL:
		{
		}
		break;
	}
	return WMSG_FAIL;

}

WMSG_RESULT CUISocketSystem::_emptyMouseMessage(MSG *pMsg )
{
	WMSG_RESULT	wmsgResult;

	// Title bar
	static BOOL bTitleBarClick = FALSE;
	// Extended button clicked
	static BOOL	bLButtonDownInBtn = FALSE;

	// Mouse point
	static int	nOldX, nOldY;
	int	nX = LOWORD( pMsg->lParam );
	int	nY = HIWORD( pMsg->lParam );

	switch( pMsg->message )
	{
		case WM_MOUSEMOVE:
		{
			CUIManager* pUIManager = CUIManager::getSingleton();

			if( IsInside( nX, nY ) )
				pUIManager->SetMouseCursorInsideUIs();

			int	ndX = nX - nOldX;
			int	ndY = nY - nOldY;

			// Hold item button
			if (pUIManager->GetDragIcon() == NULL && bLButtonDownInBtn && 
				(pMsg->wParam& MK_LBUTTON) && (ndX != 0 || ndY != 0))
				{
					if( m_nSelSlotItemID >= 0 )
					{
						// Close message box of inventory
						pUIManager->CloseMessageBox( MSGCMD_DROPITEM );

						pUIManager->SetHoldBtn(m_pIconsItem[m_nSelSlotItemID]);
					}
					bLButtonDownInBtn = FALSE;
				}

			// Move UI window.
			if( m_bPickTitle && ( pMsg->wParam & MK_LBUTTON ) )
			{
				nOldX = nX;	nOldY = nY;

				Move( ndX, ndY );

				return WMSG_SUCCESS;
			}
			else if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else if( m_btnConfirm.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else
			{
				if( IsInsideRect( nX, nY, m_rcArea ) )
				{
					m_pIconsItem[0]->MouseMessage( pMsg );
				}
				return WMSG_SUCCESS;
			}
		}
		break;
		case WM_LBUTTONDOWN:
		{
			if( IsInside( nX, nY ) )
			{
				CUIManager* pUIManager = CUIManager::getSingleton();
				nOldX = nX;		nOldY = nY;				

				// Close button
				if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				else if( m_btnConfirm.MouseMessage( pMsg) != WMSG_FAIL )
				{
					// Nothing
				}
				else if( IsInsideRect( nX, nY, m_rcTitle ) )
				{
					m_bPickTitle = TRUE;
				}
				else if( IsInsideRect( nX, nY, m_rcArea ) )
				{
					if( m_pIconsItem[0]->MouseMessage( pMsg ) != WMSG_FAIL )
					{	
						if( m_bProcessing )
						{
							if( pUIManager->DoesMessageBoxExist( MSGCMD_NULL ) )
								return WMSG_SUCCESS;
							CUIMsgBox_Info MsgInfo;
							CTString strTemp;
							MsgInfo.SetMsgBoxInfo( _S( 191, "확인" ), UMBS_OK, UI_NONE, MSGCMD_NULL );
							strTemp.PrintF( _S( 5010, "%s 중에는 아이템을 옮길 수 없습니다."), _S( 4978, "보석 제거" ) );
							MsgInfo.AddString( strTemp );
							pUIManager->CreateMessageBox( MsgInfo );
							return WMSG_SUCCESS;
						}
						m_nSelSlotItemID = 0;
						bLButtonDownInBtn = TRUE;
						pUIManager->RearrangeOrder( UI_SOCKETSYSTEM, TRUE );
						return WMSG_SUCCESS;
					}
				}
				pUIManager->RearrangeOrder( UI_SOCKETSYSTEM, TRUE );
				return WMSG_SUCCESS;
			}
		}
		break;
		case WM_LBUTTONUP:
		{
			CUIManager* pUIManager = CUIManager::getSingleton();
			m_bPickTitle = FALSE;

			if (pUIManager->GetDragIcon() == NULL)
			{
				if( !IsFocused() )
					return WMSG_FAIL;

				// Close button
				if( ( wmsgResult = m_btnClose.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
					{
						_close();
					}
					return WMSG_SUCCESS;
				}
				else if( ( wmsgResult = m_btnConfirm.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
					{
						// bug fix. [6/22/2010 rumist]
						CItems* pItems = m_pIconsItem[0]->getItems();
						if( pItems->GetAmountOfJewelryInserted() <= IsEmptyInvenSlotCount() )
						{
							m_btnConfirm.SetEnable( FALSE );
							SendCleanAllJewelReq();
						}
						else
							_showErrorMessage(SOCKET_ERROR_NOT_ENOUGH_INVEN_SLOT);
					}
				}
				else if( IsInsideRect( nX, nY, m_rcArea ) )
				{
					// nothing to do.
				}
				//return WMSG_SUCCESS;
			}
			else
			{
				if( IsInside( nX, nY ) )
				{
					CUIIcon* pDrag = pUIManager->GetDragIcon();
					CItems* pItems = pDrag->getItems();

					if (pItems == NULL)
						return WMSG_FAIL;

					pUIManager->GetMessenger()->SetDragging( false );
					if( IsInsideRect( nX, nY, m_rcArea) )
					{
						if (pDrag->getBtnType() == UBET_ITEM)
						{
							if( m_pIconsItem[0]->IsInside( nX, nY ) )
							{
								CItemData* pItemData = pItems->ItemData;
								// if item combined jewels.
								if (( pItemData->IsFlag( ITEM_FLAG_SOCKET )) &&
									pItems->Item_Wearing < 0 && // 착용중인 아이템이 아니어야 한다.
									(pItems->GetSocketCount() > 0))
								{
									m_pIconsItem[0]->copyItem(pDrag);
									m_sqNeedMoney = 0.0f;
									SBYTE sbCount = 0;
									SBYTE sbjewelCount = 0;
									for( int k = 0; k < MAX_SOCKET_OPTION; ++k )
									{
										if (k == 0)
										{
											if( m_pIconsItem[0]->getItems()->GetSocketJewelIndex( k ) > 0 )
												m_sqNeedMoney += 100000.0f;
										}

										if( m_pIconsItem[0]->getItems()->GetSocketJewelIndex( k ) >= 0 )
											++sbCount;
										if (m_pIconsItem[0]->getItems()->GetSocketJewelIndex( k ) > 0)
											++sbjewelCount;
									}

									if( pItemData->IsFlag( ITEM_FLAG_LORD ) )
										m_sqNeedMoney = 0;
									else
										m_sqNeedMoney += sbjewelCount * sbCount * PRICE_CLEAR;

									m_btnConfirm.SetEnable( TRUE );
								}
								else
								{
									_showErrorMessage();
								}	
							}
						}
					}
					pUIManager->ResetHoldBtn();
					return WMSG_SUCCESS;
				}
				else if( m_nSelSlotItemID > -1 )
				{
					m_pIconsItem[m_nSelSlotItemID]->clearIconData();
					m_btnConfirm.SetEnable( FALSE );
					m_nSelSlotItemID = -1;
					pUIManager->ResetHoldBtn();
					return WMSG_SUCCESS;
				}
			}
			pUIManager->ResetHoldBtn();
		}
		break;
		case WM_LBUTTONDBLCLK:
		{
		}
		break;
		case WM_MOUSEWHEEL:
		{
		}
		break;
	}
	return WMSG_FAIL;
}
////////////////////////////////////////////////////////////////////////////
WMSG_RESULT CUISocketSystem::_JewelMouseMessage( MSG *pMsg )
{
	if(m_eJewelActionState == JEWEL_COMPOS_ACTION_DIRECTION)
		return WMSG_FAIL;
	WMSG_RESULT	wmsgResult;

	// Title bar 
	static BOOL bTitleBarClick = FALSE;
	// Extended button clicked
	static BOOL	bLButtonDownInBtn = FALSE;

	// Mouse point
	static int	nOldX, nOldY;
	int	nX = LOWORD( pMsg->lParam );
	int	nY = HIWORD( pMsg->lParam );



	switch( pMsg->message )
	{
		case WM_MOUSEMOVE:
		{
			CUIManager* pUIManager = CUIManager::getSingleton();

			if( IsInside( nX, nY ) )
				pUIManager->SetMouseCursorInsideUIs();

			int	ndX = nX - nOldX;
			int	ndY = nY - nOldY;

			// Hold item button
			if (pUIManager->GetDragIcon() == NULL && bLButtonDownInBtn && 
				(pMsg->wParam& MK_LBUTTON) && (ndX != 0 || ndY != 0))
			{
				if( m_nSelSlotItemID >= 0 )
				{
					// Close message box of inventory
					pUIManager->CloseMessageBox( MSGCMD_DROPITEM );

					pUIManager->SetHoldBtn(m_pIconsItem[m_nSelSlotItemID]);
				}
				bLButtonDownInBtn = FALSE;
			}

			// Move UI window.
			if( m_bPickTitle && ( pMsg->wParam & MK_LBUTTON ) )
			{
				nOldX = nX;	nOldY = nY;

				Move( ndX, ndY );

				return WMSG_SUCCESS;
			}
			else if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else if( m_btnConfirm.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else
			{
				if( IsInsideRect( nX, nY, m_rcArea ) )
				{
					// 빈슬롯 가이드 체크
					_CheckEmptyGuide();
					int i;
					for( i = 0; i < JEWEL_COMPOS_BTN_JEWEL_MAX; ++i )
						m_pIconsItem[i]->MouseMessage( pMsg );
					
					for ( i = 0; i < BTN_EMPTY_MAX; i++)
					{
						if (!m_bJewelComposEmptyBtnGuide[i])
							continue;

						m_ImageJewelComposEmptyBtnGuide[i].MouseMessage(pMsg);
					}					
				}
				return WMSG_SUCCESS;
			}
		}
		break;

		case WM_LBUTTONDOWN:
		{
			if (m_bStartDirection)
				return WMSG_SUCCESS;

			if( IsInside( nX, nY ) )
			{
				CUIManager* pUIManager = CUIManager::getSingleton();
				nOldX = nX;		nOldY = nY;

				// Close button
				if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				else if( m_btnConfirm.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				else if( IsInsideRect( nX, nY, m_rcTitle ) )
				{
					m_bPickTitle = TRUE;
				}
				else if( IsInsideRect( nX, nY, m_rcArea ) )
				{
					for( int i = 0; i < JEWEL_COMPOS_BTN_JEWEL_MAX; ++i )
						if( m_pIconsItem[i]->MouseMessage( pMsg ) != WMSG_FAIL )
						{
							
							m_nSelSlotItemID = i;
							bLButtonDownInBtn = TRUE;
							pUIManager->RearrangeOrder( UI_SOCKETSYSTEM, TRUE );
							return WMSG_SUCCESS;
						}
				}
				pUIManager->RearrangeOrder( UI_SOCKETSYSTEM, TRUE );
				return WMSG_SUCCESS;
			}
		}
		break;

		case WM_RBUTTONDOWN:
		{
			if (m_bStartDirection)
				return WMSG_SUCCESS;
			CUIManager* pUIManager = CUIManager::getSingleton();
				
			if (pUIManager->GetDragIcon() == NULL)
			{
				if( IsInside( nX, nY ) )
				{
					pUIManager->GetMessenger()->SetDragging( false );

					if( IsInsideRect( nX, nY, m_rcArea) )
					{
						for (int i = 0; i < JEWEL_COMPOS_BTN_RESET_MAX; i++)
						{
							if( m_pIconsItem[i]->IsInside( nX, nY ) )
							{
								if (m_pIconsItem[i]->IsEmpty() == true)
									continue;

								m_pIconsItem[i]->clearIconData();
							}
						}
					}	
				}
				_SetJewelcomposNas();
				_setActionCheck();
				return WMSG_SUCCESS;
			}
		}
		break;

		case WM_LBUTTONUP:
		{
			if (m_bStartDirection)
				return WMSG_SUCCESS;

			CUIManager* pUIManager = CUIManager::getSingleton();
			m_bPickTitle = FALSE;

			if (pUIManager->GetDragIcon() == NULL)
			{
				if( !IsFocused() )
					return WMSG_FAIL;

				// Close button
				if( ( wmsgResult = m_btnClose.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
					{
						_close();
					}
					return WMSG_SUCCESS;
				}
				else if( ( wmsgResult = m_btnConfirm.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
					{
						if( pUIManager->DoesMessageBoxExist(MSGCMD_JEWEL_COMPOS_JEWEL_COMPOS_SCROLL) )
							return WMSG_SUCCESS;

						if( !_CheckErrorCompos(GENARAL_JEWEL_COMPOS) )
							return WMSG_SUCCESS;

						//TO.DO 합성 연출
						if(_JewelComposDirectionGradeInfo(GENARAL_JEWEL_COMPOS))
						{
							_JewelComposDirectionStart(MSG_EX_SOCKET_UPGRADE_JEWEL_REQ);
							m_btnConfirm.SetEnable( FALSE );
						}
						
						return WMSG_SUCCESS;
					}
				}
				else if( IsInsideRect( nX, nY, m_rcArea ) )
				{
					// nothing to do.
				}
				//return WMSG_SUCCESS;
			}
			else
			{
				if( IsInside( nX, nY ) )
				{
					CUIIcon* pDrag = pUIManager->GetDragIcon();
					CItems* pItems = pDrag->getItems();

					if (pItems == NULL)
						return WMSG_FAIL;

					pUIManager->GetMessenger()->SetDragging( false );
					if( IsInsideRect( nX, nY, m_rcArea) )
					{
						CItemData* pItemData = pItems->ItemData;

						for (int i = 0; i < JEWEL_COMPOS_JEWEL_MAX; i++)
						{
							if( m_pIconsItem[i]->IsInside( nX, nY ) )
							{
								if ( pItemData->GetType() == CItemData::ITEM_ETC && pItemData->GetSubType() == CItemData::ITEM_ETC_JEWEL )						
								{
									m_pIconsItem[i]->cloneItem(pDrag);
									m_pIconsItem[i]->setCount(pDrag->getItems()->Item_Sum);
								}
								else
								{
									_showErrorSystemMessage(JEWEL_COMPOS_ERROR_NOT_GENERAL_JEWEL);
								}
								break;
							}
						}

						if ( m_pIconsItem[JEWEL_COMPOS_BTN_SCROLL]->IsInside( nX, nY ) )
						{
							if ( pItemData->GetType() == CItemData::ITEM_ETC && pItemData->GetSubType() == CItemData::ITEM_ETC_FUNCTIONS &&
								pItemData->GetItemIndex() == JEWEL_COMPOS_SCROLL_ITEM_INDEX)	//TO.DO 기타, 기능성, NUM0 == 0					
							{
								if(!pUIManager->DoesMessageBoxExist(MSGCMD_JEWEL_COMPOS_JEWEL_COMPOS_SCROLL))
								{
									CTString strMessage;
									CUIMsgBox_Info msgBoxInfo;
									CTString strItemCount;
									msgBoxInfo.SetMsgBoxInfo(_S( 5925,"보석 합성"), UMBS_OKCANCEL | UMBS_INPUTBOX, UI_SOCKETSYSTEM, MSGCMD_JEWEL_COMPOS_JEWEL_COMPOS_SCROLL);
									
									strItemCount.PrintF(_s("%I64d"), pItems->Item_Sum);
									
									strMessage.PrintF( _S( 5930, "%s 몇 개를 등록 하시겠습니까? (최대 99개 등록, 1회 합성마다 소모)"), pItemData->GetName() );
									msgBoxInfo.AddString(strMessage);
									pUIManager->CreateMessageBox( msgBoxInfo );
									pUIManager->GetMessageBox(MSGCMD_JEWEL_COMPOS_JEWEL_COMPOS_SCROLL)->GetInputBox().SetMaxChar(2);
									//pUIManager->GetMessageBox(MSGCMD_JEWEL_COMPOS_JEWEL_COMPOS_SCROLL)->GetInputBox().SetString(strItemCount.str_String);
									
									m_pIconsItem[JEWEL_COMPOS_BTN_SCROLL]->cloneItem(pDrag);
									m_pIconsItem[JEWEL_COMPOS_BTN_SCROLL]->setCount(_atoi64(strItemCount));
									m_nOneMoreComposScrollpay = 1;
								}
							}
							else
							{
								_showErrorSystemMessage(JEWEL_COMPOS_ERROR_NOT_COMPOS_SCROLL);
							}
						}
					}
					pUIManager->ResetHoldBtn();
					_SetJewelcomposNas();
					_setActionCheck();
					return WMSG_SUCCESS;
				}
				else if( m_nSelSlotItemID > -1 )
				{
					m_pIconsItem[m_nSelSlotItemID]->clearIconData();
					m_btnConfirm.SetEnable( FALSE );
					m_nSelSlotItemID = -1;
					pUIManager->ResetHoldBtn();
					_SetJewelcomposNas();
					_setActionCheck();
					return WMSG_SUCCESS;
				}
			}
			pUIManager->ResetHoldBtn();
		}
		break;
		case WM_LBUTTONDBLCLK:
		{
		}
		break;
		case WM_MOUSEWHEEL:
		{
		}
		break;
	}
	return WMSG_FAIL;
}
////////////////////////////////////////////////////////////////////////////
WMSG_RESULT CUISocketSystem::_ChaosJewelMouseMessage( MSG *pMsg )
{
	if(m_eJewelActionState == JEWEL_COMPOS_ACTION_DIRECTION)
		return WMSG_FAIL; 

	WMSG_RESULT	wmsgResult;

	// Title bar
	static BOOL bTitleBarClick = FALSE;
	// Extended button clicked
	static BOOL	bLButtonDownInBtn = FALSE;

	// Mouse point
	static int	nOldX, nOldY;
	int	nX = LOWORD( pMsg->lParam );
	int	nY = HIWORD( pMsg->lParam );

	switch( pMsg->message )
	{
		case WM_MOUSEMOVE:
		{
			CUIManager* pUIManager = CUIManager::getSingleton();

			if( IsInside( nX, nY ) )
				pUIManager->SetMouseCursorInsideUIs();

			int	ndX = nX - nOldX;
			int	ndY = nY - nOldY;

			// Hold item button
			if (pUIManager->GetDragIcon() == NULL && bLButtonDownInBtn && 
				(pMsg->wParam& MK_LBUTTON) && (ndX != 0 || ndY != 0))
			{
				if( m_nSelSlotItemID >= 0 )
				{
					// Close message box of inventory
					pUIManager->CloseMessageBox( MSGCMD_DROPITEM );

					pUIManager->SetHoldBtn(m_pIconsItem[m_nSelSlotItemID]);
				}
				bLButtonDownInBtn = FALSE;
			}

			// Move UI window.
			if( m_bPickTitle && ( pMsg->wParam & MK_LBUTTON ) )
			{
				nOldX = nX;	nOldY = nY;

				Move( ndX, ndY );

				return WMSG_SUCCESS;
			}
			else if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else if( m_btnConfirm.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else
			{
				if( IsInsideRect( nX, nY, m_rcArea ) )
				{
					// 빈슬롯 가이드 체크
					_CheckEmptyGuide();
					int i;
					for( i = 0; i < JEWEL_COMPOS_BTN_JEWEL_MAX; ++i )
						m_pIconsItem[i]->MouseMessage( pMsg );
					
					for ( i = 0; i < BTN_EMPTY_MAX; i++)
					{
						if (!m_bJewelComposEmptyBtnGuide[i])
							continue;
						m_ImageJewelComposEmptyBtnGuide[i].MouseMessage(pMsg);
					}					
				}
				return WMSG_SUCCESS;
			}
		}
		break;

		case WM_LBUTTONDOWN:
		{
			if (m_bStartDirection)
				return WMSG_SUCCESS;

			if( IsInside( nX, nY ) )
			{
				CUIManager* pUIManager = CUIManager::getSingleton();
				nOldX = nX;		nOldY = nY;

				// Close button
				if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				else if( m_btnConfirm.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				else if( IsInsideRect( nX, nY, m_rcTitle ) )
				{
					m_bPickTitle = TRUE;
				}
				else if( IsInsideRect( nX, nY, m_rcArea ) )
				{
					for( int i = 0; i < JEWEL_COMPOS_BTN_JEWEL_MAX; ++i )
						if( m_pIconsItem[i]->MouseMessage( pMsg ) != WMSG_FAIL )
						{
							
							m_nSelSlotItemID = i;
							bLButtonDownInBtn = TRUE;
							pUIManager->RearrangeOrder( UI_SOCKETSYSTEM, TRUE );
							return WMSG_SUCCESS;
						}
				}
				pUIManager->RearrangeOrder( UI_SOCKETSYSTEM, TRUE );
				return WMSG_SUCCESS;
			}
		}
		break;

		case WM_RBUTTONDOWN:
		{
			if (m_bStartDirection)
				return WMSG_SUCCESS;
			CUIManager* pUIManager = CUIManager::getSingleton();
				
			if (pUIManager->GetDragIcon() == NULL)
			{
				if( IsInside( nX, nY ) )
				{
					pUIManager->GetMessenger()->SetDragging( false );

					if( IsInsideRect( nX, nY, m_rcArea) )
					{
						for (int i = 0; i < JEWEL_COMPOS_BTN_RESET_MAX; i++)
						{
							if( m_pIconsItem[i]->IsInside( nX, nY ) )
							{
								if( m_pIconsItem[i]->IsEmpty())
									continue;

								m_pIconsItem[i]->clearIconData();
							}
						}
					}	
				}
				_SetJewelcomposNas();
				_setActionCheck();
				return WMSG_SUCCESS;
			}
		}
		break;

		case WM_LBUTTONUP:
		{
			if (m_bStartDirection)
				return WMSG_SUCCESS;

			CUIManager* pUIManager = CUIManager::getSingleton();
			m_bPickTitle = FALSE;

			if (pUIManager->GetDragIcon() == NULL)
			{
				if( !IsFocused() )
					return WMSG_FAIL;

				// Close button
				if( ( wmsgResult = m_btnClose.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
					{
						_close();
					}
					return WMSG_SUCCESS;
				}
				else if( ( wmsgResult = m_btnConfirm.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
					{
						if( pUIManager->DoesMessageBoxExist(MSGCMD_JEWEL_COMPOS_JEWEL_COMPOS_SCROLL) )
							return WMSG_SUCCESS;

						if( !_CheckErrorCompos(CHAOS_JEWEL_COMPOS) )
							return WMSG_SUCCESS;

						//TO.DO 합성 연출
						if(_JewelComposDirectionGradeInfo(CHAOS_JEWEL_COMPOS))
						{
							_JewelComposDirectionStart(MSG_EX_SOCKET_UPGRADE_JEWEL_CHAOS_REQ);
							m_btnConfirm.SetEnable( FALSE );
						}
						
						return WMSG_SUCCESS;
					}
				}
				else if( IsInsideRect( nX, nY, m_rcArea ) )
				{
					// nothing to do.
				}
				//return WMSG_SUCCESS;
			}
			else
			{
				if( IsInside( nX, nY ) )
				{
					CUIIcon* pDrag = pUIManager->GetDragIcon();
					CItems* pItems = pDrag->getItems();

					if (pItems == NULL)
						return WMSG_FAIL;

					pUIManager->GetMessenger()->SetDragging( false );
					if( IsInsideRect( nX, nY, m_rcArea) )
					{
						CItemData* pItemData = pItems->ItemData;

						for (int i = 0; i < JEWEL_COMPOS_JEWEL_MAX; i++)
						{
							if (m_pIconsItem[i]->IsInside(nX, nY))
							{
								if ( pItemData->GetType() == CItemData::ITEM_ETC && pItemData->GetSubType() == CItemData::ITEM_ETC_CHAOSJEWEL )						
								{
									m_pIconsItem[i]->copyItem(pDrag);
								}
								else
								{
									_showErrorSystemMessage(JEWEL_COMPOS_ERROR_NOT_CHAOS_JEWEL);
								}
								break;
							}
						}

						if ( m_pIconsItem[JEWEL_COMPOS_BTN_SCROLL]->IsInside( nX, nY ) )
						{
							if ( pItemData->GetType() == CItemData::ITEM_ETC && pItemData->GetSubType() == CItemData::ITEM_ETC_FUNCTIONS &&
								pItemData->GetItemIndex() == CHAOSJEWEL_COMPOS_SCROLL_ITEM_INDEX)//TO.DO 기타, 기능성, NUM0 == 1					
							{
								if(!pUIManager->DoesMessageBoxExist(MSGCMD_JEWEL_COMPOS_JEWEL_COMPOS_SCROLL))
								{
									CTString strMessage;
									CUIMsgBox_Info msgBoxInfo;
									CTString strItemCount;
									msgBoxInfo.SetMsgBoxInfo(_S( 5925,"보석 합성"), UMBS_OKCANCEL | UMBS_INPUTBOX, UI_SOCKETSYSTEM, MSGCMD_JEWEL_COMPOS_JEWEL_COMPOS_SCROLL);
									
									strItemCount.PrintF(_s("%I64d"), pItems->Item_Sum);
									
									strMessage.PrintF( _S( 5930, "%s 몇 개를 등록 하시겠습니까? (최대 99개 등록, 1회 합성마다 소모)"), pItemData->GetName() );
									msgBoxInfo.AddString(strMessage);
									pUIManager->CreateMessageBox( msgBoxInfo );
									pUIManager->GetMessageBox(MSGCMD_JEWEL_COMPOS_JEWEL_COMPOS_SCROLL)->GetInputBox().SetMaxChar(2);
									//pUIManager->GetMessageBox(MSGCMD_JEWEL_COMPOS_JEWEL_COMPOS_SCROLL)->GetInputBox().SetString(strItemCount.str_String);
									
									m_pIconsItem[JEWEL_COMPOS_BTN_SCROLL]->cloneItem(pDrag);
									m_pIconsItem[JEWEL_COMPOS_BTN_SCROLL]->setCount(_atoi64(strItemCount));
									m_nOneMoreComposScrollpay = 1;
								}
							}
							else
							{
								_showErrorSystemMessage(JEWEL_COMPOS_ERROR_NOT_COMPOS_SCROLL);
							}
						}
					}
					pUIManager->ResetHoldBtn();
					_SetJewelcomposNas();
					_setActionCheck();
					return WMSG_SUCCESS;
				}
				else if( m_nSelSlotItemID > -1 )
				{
					m_pIconsItem[m_nSelSlotItemID]->clearIconData();
					m_btnConfirm.SetEnable( FALSE );
					m_nSelSlotItemID = -1;
					pUIManager->ResetHoldBtn();
					_SetJewelcomposNas();
					_setActionCheck();
					return WMSG_SUCCESS;
				}
			}
			pUIManager->ResetHoldBtn();
		}
		break;
		case WM_LBUTTONDBLCLK:
		{
		}
		break;
		case WM_MOUSEWHEEL:
		{
		}
		break;
	}
	return WMSG_FAIL;
}
//////////////////////////////////////////////////////////////////////////
// Public functions.
void CUISocketSystem::Create(CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	int		i;
	
	CUIWindow::Create(pParentWnd, nX, nY, nWidth, nHeight);

	m_nOrgWidth = nWidth;
	m_nOrgHeight = nHeight;
	// create texture.
	m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\SocketSystem.tex" ) );
	m_ptdButtonTexture = CreateTexture( CTString( "Data\\Interface\\CommonBtn.tex" ) );
	m_ptdSocketCombineTexture = CreateTexture( CTString( "Data\\Interface\\SocketSystemCombine.tex" ) );
	m_ptdSocketOnceEmptyTexture = CreateTexture( CTString( "Data\\Interface\\SocketSystemEx.tex") );
	m_ptdSocketJewelComposTexture = CreateTexture( CTString( "Data\\Interface\\Jewel_Compos.tex"));
	// get texture size.
	FLOAT		fTexWidth = m_ptdBaseTexture->GetPixWidth();	
	FLOAT		fTexHeight = m_ptdBaseTexture->GetPixHeight();

	m_rtTop.SetUV( 0, 0, SOCKET_UI_TEX_FRAME_WIDTH, SOCKET_UI_FRAME_TITLE, fTexWidth, fTexHeight );
	m_rtMiddle.SetUV( 0, SOCKET_UI_FRAME_TITLE, SOCKET_UI_TEX_FRAME_WIDTH, SOCKET_UI_TEX_FRAME_HEIGHT-SOCKET_UI_FRAME_BOTTOM, fTexWidth, fTexHeight );
	m_rtBottom.SetUV( 0, SOCKET_UI_TEX_FRAME_HEIGHT-SOCKET_UI_FRAME_BOTTOM, SOCKET_UI_TEX_FRAME_WIDTH, SOCKET_UI_TEX_FRAME_HEIGHT, fTexWidth, fTexHeight );
	m_rtItemSlot.SetUV( 412, 172, 412 + BTN_SIZE+ 2, 172 + BTN_SIZE + 2, fTexWidth, fTexHeight );

	// set size of title.
	m_rcTitle.SetRect( 0, 0, nWidth, SOCKET_UI_FRAME_TITLE );

	// set rect size.
	_setUIRect(STAT_CREATE);

	// set size of create UI on this texture.
	m_rtsCreate.AddRectSurface( UIRect( 0, 0, SOCKET_UI_INNERFRAME_WIDTH, SOCKET_UI_INNERFRAME_HEIGHT), 
								UIRectUV( SOCKET_UI_TEX_CREATE_X, SOCKET_UI_TEX_CREATE_Y, 
										  SOCKET_UI_TEX_CREATE_X+SOCKET_UI_INNERFRAME_WIDTH, 
										  SOCKET_UI_TEX_CREATE_Y+SOCKET_UI_INNERFRAME_HEIGHT, 
										  fTexWidth, fTexHeight ) );

	m_rtsWarning.AddRectSurface( UIRect( 0, 0, SOCKET_UI_INNERFRAME_WIDTH, 49), 
										UIRectUV( SOCKET_UI_TEX_WARNING_X, SOCKET_UI_TEX_WARNING_Y,
												  SOCKET_UI_TEX_WARNING_X+SOCKET_UI_INNERFRAME_WIDTH,
												  SOCKET_UI_TEX_WARNING_Y+49,
												  fTexWidth, fTexHeight ) );

	m_rtsComplite.AddRectSurface( UIRect( 0, 0, BTN_SIZE, BTN_SIZE ), 
									 UIRectUV( SOCKET_UI_TEX_COMPLITE_X, SOCKET_UI_TEX_COMPLITE_Y, 
											   SOCKET_UI_TEX_COMPLITE_X+BTN_SIZE,
											   SOCKET_UI_TEX_COMPLITE_Y+BTN_SIZE, 
											   fTexWidth, fTexHeight ) );

	m_rtsUnknown.AddRectSurface( UIRect( 0, 0, BTN_SIZE, BTN_SIZE ), 
									 UIRectUV( SOCKET_UI_TEX_UNKNOWN_X, SOCKET_UI_TEX_UNKNOWN_Y, 
											   SOCKET_UI_TEX_UNKNOWN_X+BTN_SIZE,
											   SOCKET_UI_TEX_UNKNOWN_Y+BTN_SIZE, 
											   fTexWidth, fTexHeight ) );

	fTexWidth = m_ptdSocketJewelComposTexture->GetPixWidth();
	fTexHeight = m_ptdSocketJewelComposTexture->GetPixHeight();
	
	m_rtsResultJewel.AddRectSurface( UIRect(0, 0, 50,48 ), UIRectUV( 198, 61, 248, 109, fTexWidth, fTexHeight) ); // 일반 보석
	m_rtsResultJewel.AddRectSurface( UIRect(0, 0, 50,48 ), UIRectUV( 198, 4, 248, 52, fTexWidth, fTexHeight) ); // 카오스 보석

	m_rtsJewelCompos.AddRectSurface( UIRect( 0, 0, JEWEL_COMPOS_UI_INNERFRAME_WIDTH, JEWEL_COMPOS_UI_INNERFRAME_HEIGHT), 
		UIRectUV( JEWEL_COMPOS_UI_TEX_INNERFRAME_X, JEWEL_COMPOS_UI_TEX_INNERFRAME_Y, 
		JEWEL_COMPOS_UI_TEX_INNERFRAME_X+JEWEL_COMPOS_UI_INNERFRAME_WIDTH,
		JEWEL_COMPOS_UI_TEX_INNERFRAME_Y+JEWEL_COMPOS_UI_INNERFRAME_HEIGHT, 
		fTexWidth, fTexHeight ) );

	m_rtsChaosJewelCompos.AddRectSurface( UIRect( 0, 0, JEWEL_COMPOS_UI_INNERFRAME_WIDTH, JEWEL_COMPOS_UI_INNERFRAME_HEIGHT), 
		UIRectUV( CHAOSJEWEL_COMPOS_UI_TEX_INNERFRAME_X, CHAOSJEWEL_COMPOS_UI_TEX_INNERFRAME_Y, 
		CHAOSJEWEL_COMPOS_UI_TEX_INNERFRAME_X+JEWEL_COMPOS_UI_INNERFRAME_WIDTH,
		CHAOSJEWEL_COMPOS_UI_TEX_INNERFRAME_Y+JEWEL_COMPOS_UI_INNERFRAME_HEIGHT, 
		fTexWidth, fTexHeight ) );

	// 결합 UI 크리에이트 부분
	fTexWidth = m_ptdSocketCombineTexture->GetPixWidth();
	fTexHeight = m_ptdSocketCombineTexture->GetPixHeight();

	m_rtsCombine.AddRectSurface( UIRect( 0, 0, EX_SOCKET_UI_INNERFRAME_WIDTH, EX_COMBINE_UI_INNERFRAME_HEIGHT), 
		UIRectUV( EX_SOCKET_UI_TEX_COMBINE_X, EX_SOCKET_UI_TEX_COMBINE_Y,
		EX_SOCKET_UI_TEX_COMBINE_X+EX_SOCKET_UI_INNERFRAME_WIDTH,
		EX_SOCKET_UI_TEX_COMBINE_Y+EX_COMBINE_UI_INNERFRAME_HEIGHT,
		fTexWidth, fTexHeight ) );

	// 슬롯 상태 배경 이미지 셋팅 하는 부분
	m_rtsCombineSocket.AddRectSurface( UIRect(0, 0, 34,34 ), UIRectUV( 174, 0, 208, 34, fTexWidth, fTexHeight) ); // 비활성
	m_rtsCombineSocket.AddRectSurface( UIRect(0, 0, 42,42 ), UIRectUV( 175, 36, 217, 78, fTexWidth, fTexHeight) ); // 선택
	m_rtsCombineSocket.AddRectSurface( UIRect(0, 0, 40,36 ), UIRectUV( 176, 81, 216, 117, fTexWidth, fTexHeight) ); // 잠김

	// 결합 보석 락버튼 설정 [4/4/2013 Ranma]
	for( i = 0; i < ITEM_MAX_COUNT; i++ )
	{
		m_bButtonLock[i] = FALSE;
		m_bItemButtonLock[i] = FALSE;
	}

	fTexWidth = m_ptdSocketOnceEmptyTexture->GetPixWidth();
	fTexHeight = m_ptdSocketOnceEmptyTexture->GetPixHeight();

	m_rtsOnceEmpty.AddRectSurface( UIRect( 0, 0, EX_SOCKET_UI_INNERFRAME_WIDTH, EX_SOCKET_UI_INNERFRAME_HEIGHT), 
		UIRectUV( EX_SOCKET_UI_TEX_COMBINE_X, EX_SOCKET_UI_TEX_COMBINE_Y,
		EX_SOCKET_UI_TEX_COMBINE_X+EX_SOCKET_UI_INNERFRAME_WIDTH,
		EX_SOCKET_UI_TEX_COMBINE_Y+EX_SOCKET_UI_INNERFRAME_HEIGHT,
		fTexWidth, fTexHeight ) );

	///////////////////////////////////////////////////////////////
	// set button texture.
	fTexWidth = m_ptdButtonTexture->GetPixWidth();
	fTexHeight = m_ptdButtonTexture->GetPixHeight();

	// close button
	m_btnClose.Create( this, CTString(""), SOCKET_UI_BTN_CLOSE_POS_X, SOCKET_UI_BTN_CLOSE_POS_Y, SOCKET_UI_BTN_CLOSE_SIZE, SOCKET_UI_BTN_CLOSE_SIZE );
	m_btnClose.SetUV(UBS_IDLE,211, 33, 227, 49,fTexWidth,fTexHeight);
	m_btnClose.SetUV(UBS_CLICK,229, 33, 245, 49,fTexWidth,fTexHeight);
	m_btnClose.CopyUV(UBS_IDLE,UBS_ON);
	m_btnClose.CopyUV(UBS_IDLE,UBS_DISABLE);

	// 버튼 위치 지정.
	UIRect rcLeft, rcMiddel, rcRight;
	UIRectUV3 rtIdleBtn, rtClickBtn;

	rcLeft.SetRect(0, 0, 14, 21);
	rcMiddel.SetRect(14, 0, 78, 21);
	rcRight.SetRect(78, 0, 92, 21);

	rtIdleBtn.SetUV3(113, 0, 127, 22, 127, 0, 168, 22, 168, 0, 183, 22, fTexWidth, fTexHeight );
	rtClickBtn.SetUV3(187, 0, 201, 22, 201, 0, 242, 22, 242, 0, 256, 22, fTexWidth, fTexHeight);

	m_btnConfirm.Create( this, _S( 4961, "생성" ), 40, 225, SOCKET_UI_BTN_CONFIRM_WIDTH, SOCKET_UI_BTN_CONFIRM_HEIGHT );
	m_btnConfirm.SetRTSurface( UBS_IDLE, rcLeft, rtIdleBtn.rtL );
	m_btnConfirm.SetRTSurface( UBS_IDLE, rcMiddel, rtIdleBtn.rtM );
	m_btnConfirm.SetRTSurface( UBS_IDLE, rcRight, rtIdleBtn.rtR );
	m_btnConfirm.SetRTSurface( UBS_CLICK, rcLeft, rtClickBtn.rtL );
	m_btnConfirm.SetRTSurface( UBS_CLICK, rcMiddel, rtClickBtn.rtM );
	m_btnConfirm.SetRTSurface( UBS_CLICK, rcRight, rtClickBtn.rtR );
	m_btnConfirm.CopyRTSurface( UBS_IDLE, UBS_ON );
	m_btnConfirm.CopyRTSurface( UBS_IDLE, UBS_DISABLE );
	m_btnConfirm.SetNewType(TRUE);
	
	m_rtsNeedMoney.AddRectSurface( UIRect( 0, 0, 161, 22 ),
								   UIRectUV( 265, 68,
											 265 + 161,
											 68 + 22,
											 fTexWidth, fTexHeight ) );
	m_msgCombineWarning.SetRenderRect( 0, 0, 161, 22 );
	m_msgCombineWarning.AddString(_S( 5918, "결합 보조아이템이 없으면 결합 실패 시 보석의 등급이 하락됩니다. 결합 버튼을 눌러주세요." ));
	m_msgCombineSuccess.SetRenderRect( 0, 0, 161, 22 );
	m_msgCombineSuccess.AddString(_S(5920, "축하합니다! 보석결합에 성공하였습니다!!"), 0x0000EEFF);
	m_msgCombineFail.SetRenderRect( 0, 0, 161, 22 );
	m_msgCombineFail.AddString(_S(5921, "보석결합에 실패하여 보석의 등급이 하락하였습니다!"), 0xEE0000FF); // 보조 스크롤이 없을경우
	m_msgCombineFail2.SetRenderRect( 0, 0, 161, 22 );
	m_msgCombineFail2.AddString(_S(6051, "보석결합에 실패하였습니다!"), 0xEE0000FF); // 보조 스크롤이 있을 경우
	m_msgJewelcomposIdle.SetRenderRect( 0, 0, JEWEL_COMPOS_MSG_WIDTH, JEWEL_COMPOS_MSG_HEIGHT);
	m_msgJewelcomposIdle.AddString( _S( 5931, "일반보석을 등록하세요." ) );
	m_msgChaosJewelcomposIdle.SetRenderRect( 0, 0, JEWEL_COMPOS_MSG_WIDTH, JEWEL_COMPOS_MSG_HEIGHT);
	m_msgChaosJewelcomposIdle.AddString( _S( 5929, "카오스보석을 등록하세요." ) );
	m_msgJewelcomposReady.SetRenderRect( 0, 0, JEWEL_COMPOS_MSG_WIDTH, JEWEL_COMPOS_MSG_HEIGHT);
	m_msgJewelcomposReady.AddString( _S( 5932, "보석합성이 가능합니다." ) );
	m_msgJewelcomposNaslack.SetRenderRect( 0, 0, JEWEL_COMPOS_MSG_WIDTH, JEWEL_COMPOS_MSG_HEIGHT);
	m_msgJewelcomposNaslack.AddString( _S( 306, "나스가 부족합니다." ) );

	m_eJewelActionState = JEWEL_COMPOS_ACTION_IDLE;
	m_colorJewelcomposNasStr = 0xFFFFFFFF;

	m_msgUpgradeText.SetRenderRect( 0, 0, 161, 22 );
	m_msgUpgradeText.AddString( _S( 4986, "주의: 보석의 종류와 레벨이 다른 아이템이 생성될 수도 있습니다." ) );
	m_msgEmptyText.SetRenderRect( 0, 0, 161, 22 );
	m_msgEmptyText.AddString( _S( 4987, "소켓 비우기를 하면 박혀있는 모든 보석아이템이 사라지게 됩니다."
								  " 그래도 소켓 비우기를 하시겠습니까?" ) );
	m_msgOnceEmptyText.SetRenderRect( 0, 0, 161, 22);
	m_msgOnceEmptyText.AddString( _S( 5898, "장비를 올려놓은 후 제거를 원하는 소켓의 보석을 선택하여 주세요." ) );
// 	m_msgChanceCardText.SetRenderRect( 0, 0, 161, 22 );
// 	m_msgChanceCardText.AddString( _S( 4988, "소켓 한번 더 찬스 카드를 소켓을 한번 뚫은 아이템에 사용하면 다시 소켓을 뚫을"
// 									   " 수 있는 상태로 변경됩니다." ) );
	
	m_pbBar.Create( this, nX+SOCKET_UI_FRAME_BORDER, nY+251, SOCKET_UI_FRAME_WIDTH-SOCKET_UI_FRAME_BORDER*2, 25 );
	m_pbBar.SetProgressRange( 0, 100 );
	m_pbBar.SetProgressStep( 1 );
	m_pbBar.OffsetProgressPos( 0 );
	m_pbBar.SetAutoComplete( FALSE );
	/////////////////////////////////////////////////////////////////////////////
	// Create Item Button
	for( i = 0; i < ITEM_MAX_COUNT; ++i )
	{
		m_pIconsItem[i] = new CUIIcon();
		m_pIconsItem[i]->Create(this, 0, 0, BTN_SIZE, BTN_SIZE, UI_SOCKETSYSTEM, UBET_ITEM);
	}
	// 5초에 해당하는 timegettime() 수치.
	m_uEndTime = 2;

	m_pbBar.SetProgressRange( 0, m_uEndTime* 1000 );
	m_bUseTimer = FALSE;

	m_pIconEmpty = new CUIIcon;
	m_pIconEmpty->Create(NULL, 0, 0, BTN_SIZE, BTN_SIZE, UI_SOCKETSYSTEM, UBET_ITEM);
}

void CUISocketSystem::Render()
{
	_calcCmdTimer();
	_ASSERT( this->m_fnRender != NULL );

	(this->*m_fnRender)();	
}

void CUISocketSystem::ResetPosition(PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
}

void CUISocketSystem::AdjustPosition(PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

WMSG_RESULT CUISocketSystem::MouseMessage(MSG *pMsg )
{
	_ASSERT( this->m_fnMsgProc != NULL );

	return (this->*m_fnMsgProc)(pMsg);
}

void CUISocketSystem::ToggleVisible( const _SOCKET_STAT _status /* = STAT_CREATE  */)
{
	_toggleVisible( _status );
}

void CUISocketSystem::SetNeedMoneyForCreate(const SQUAD _money /* = 0  */)
{
	m_sqNeedMoney = _money;
}

void CUISocketSystem::OpenSocketSystem(int iMobIndex, BOOL bHasQuest, FLOAT fX, FLOAT fZ )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	// If this is already exist.
	if( pUIManager->DoesMessageBoxLExist( MSGLCMD_SOCKET_SYSTEM ) || IsVisible() )
		return;
	if( pUIManager->DoesMessageBoxLExist( MSGLCMD_SOCKET_SYSTEM_EMPTY ) || IsVisible() )
		return;
	if( pUIManager->DoesMessageBoxExist( MSGLCMD_SOCKET_SYSTEM_EMPTY_BOX ) || IsVisible() )
		return;
	if( pUIManager->DoesMessageBoxLExist( MSGLCMD_SOCKET_SYSTEM_JEWEL_COMPOS ) || IsVisible() )
		return;

	CMobData* MD = CMobData::getData( iMobIndex );

	m_nNPCIndex = iMobIndex;
	m_fNPCPosX = fX;
	m_fNPCPosZ = fZ;

	if (CEntity* pEntity = INFO()->GetTargetEntity(eTARGET))
		m_nNPCVIdx = pEntity->GetNetworkID();

	// Create refine message box.
	pUIManager->CreateMessageBoxL( _S( 4989, "소켓 생성 및 보석 관리" ), UI_SOCKETSYSTEM, MSGLCMD_SOCKET_SYSTEM );
	CTString strNpcName = CMobData::getData( iMobIndex )->GetName();
	pUIManager->AddMessageBoxLString( MSGLCMD_SOCKET_SYSTEM, TRUE, strNpcName, -1, 0xE18600FF );
	pUIManager->AddMessageBoxLString( MSGLCMD_SOCKET_SYSTEM, TRUE, _S( 4990, "아이템을 가져오면 소켓을 뚫어주겠네." 
																	"단 소켓 가공을 이미 끝낸 아이템은 다시 "
																	"작업하기 힘들어 가공이 안되니 주의하게. "
																	"그리고 소켓 뚫린 아이템과 보석을 가져다 주면, "
																	"내가 소켓 홈에다 가 보석을 결합시켜 주겠네."), -1, 0xA3A1A3FF );

	CTString strMessage;
	strMessage.PrintF( _S( 4973, "소켓 생성" ) );
	pUIManager->AddMessageBoxLString( MSGLCMD_SOCKET_SYSTEM, FALSE, strMessage, SOCKET_CREATE );
	strMessage.PrintF( _S( 4976, "보석 결합" ) );
	pUIManager->AddMessageBoxLString( MSGLCMD_SOCKET_SYSTEM, FALSE, strMessage, SOCKET_COMBINE );
	strMessage.PrintF( _S( 5925, "보석 합성" ) );
	pUIManager->AddMessageBoxLString( MSGLCMD_SOCKET_SYSTEM, FALSE, strMessage, SOCKET_JEWEL_COMPOS );
	strMessage.PrintF( _S( 4978, "보석 제거" ) );
	pUIManager->AddMessageBoxLString( MSGLCMD_SOCKET_SYSTEM, FALSE, strMessage, SOCKET_EMPTY );
	strMessage.PrintF( _S( 4991, "재료 구매" ) );
	pUIManager->AddMessageBoxLString( MSGLCMD_SOCKET_SYSTEM, FALSE, strMessage, SOCKET_SHOP );
	// quest
	if( bHasQuest )
	{
		strMessage.PrintF( _S( 1053, "이야기 한다." ) );
		pUIManager->AddMessageBoxLString( MSGLCMD_SOCKET_SYSTEM, FALSE, strMessage, SOCKET_TALK );
	}
	// event
	if( MD->IsEvent() )
	{
		strMessage.PrintF( _S( 100, "이벤트." ) );
		pUIManager->AddMessageBoxLString( MSGLCMD_SOCKET_SYSTEM, FALSE, strMessage, SOCKET_EVENT );
	}
	// cancel
	strMessage.PrintF( _S( 1220, "취소한다." ) );
	pUIManager->AddMessageBoxLString( MSGLCMD_SOCKET_SYSTEM, FALSE, strMessage );
}

void CUISocketSystem::MsgBoxLCommand(int nCommandCode, int nResult )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if (nResult >= 0)	// 소켓 UI를 열려고 할때.
	{
		if (pUIManager->GetInventory()->IsLocked() == TRUE ||
			pUIManager->GetInventory()->IsLockedArrange() == TRUE)
		{
			// 이미 Lock 인 창이 있을 경우 열지 못한다.
			pUIManager->GetInventory()->ShowLockErrorMessage();
			return;
		}
	}
	switch( nCommandCode )
	{
		case MSGLCMD_SOCKET_SYSTEM:
		{
			if( nResult == SOCKET_CREATE )
			{
				ToggleVisible( STAT_CREATE );

				pUIManager->RearrangeOrder( UI_INVENTORY, TRUE );
			}
			else if( nResult == SOCKET_COMBINE )
			{
				ToggleVisible( STAT_COMBINE );

				pUIManager->RearrangeOrder( UI_INVENTORY, TRUE );
			}
			else if( nResult == SOCKET_JEWEL_COMPOS )
			{
				CUIManager* pUIManager = CUIManager::getSingleton();
				
				pUIManager->CreateMessageBoxL( _S( 5925, "보석 합성" ), UI_SOCKETSYSTEM, MSGLCMD_SOCKET_SYSTEM_JEWEL_COMPOS );
				CTString strNpcName = CMobData::getData( m_nNPCIndex )->GetName();
				pUIManager->AddMessageBoxLString( MSGLCMD_SOCKET_SYSTEM_JEWEL_COMPOS, TRUE, strNpcName, -1, 0xE18600FF );
				pUIManager->AddMessageBoxLString( MSGLCMD_SOCKET_SYSTEM_JEWEL_COMPOS, TRUE, _S( 5926, "보석 3개를 합성하여 새로운 보석을 만들어줄수 있지."
					"아주 가끔은 카오스 보석이 나올수도 있다네."
					"마법 스크롤을 사용하면 좋은 보석을 얻을수 있다네."), -1, 0xA3A1A3FF );
				
				CTString strMessage;
				strMessage.PrintF( _S( 5927, "일반보석 합성" ) );
				pUIManager->AddMessageBoxLString( MSGLCMD_SOCKET_SYSTEM_JEWEL_COMPOS, FALSE, strMessage, SOCKET_JEWEL_COMPOS );
				strMessage.PrintF( _S( 5928, "카오스보석 합성" ) );
				pUIManager->AddMessageBoxLString( MSGLCMD_SOCKET_SYSTEM_JEWEL_COMPOS, FALSE, strMessage, SOCKET_CHAOSJEWEL_COMPOS );
				strMessage.PrintF( _S( 1220, "취소한다." ) );
				pUIManager->AddMessageBoxLString( MSGLCMD_SOCKET_SYSTEM_JEWEL_COMPOS, FALSE, strMessage );
			}
			else if( nResult == SOCKET_EMPTY )
			{
				CUIManager* pUIManager = CUIManager::getSingleton();

				pUIManager->CreateMessageBoxL( _S( 4978, "보석 제거" ), UI_SOCKETSYSTEM, MSGLCMD_SOCKET_SYSTEM_EMPTY );
				CTString strNpcName = CMobData::getData( m_nNPCIndex )->GetName();
				pUIManager->AddMessageBoxLString( MSGLCMD_SOCKET_SYSTEM_EMPTY, TRUE, strNpcName, -1, 0xE18600FF );
				pUIManager->AddMessageBoxLString( MSGLCMD_SOCKET_SYSTEM_EMPTY, TRUE, _S( 5895, "보석이 결합된 장비에서 보석을 제거하는"
					"작업은 매우 어려운데다가 원하는 보석만 제거하려면 특별한 마법 스크롤도 필요하지. "
					"게다가 제거 작업 중에 보석이 파괴되는 경우가 많으니 이점도 꼭 명심하게."), -1, 0xA3A1A3FF );

				CTString strMessage;
				strMessage.PrintF( _S( 5896, "보석 일괄 제거" ) );
				pUIManager->AddMessageBoxLString( MSGLCMD_SOCKET_SYSTEM_EMPTY, FALSE, strMessage, SOCKET_EMPTY );
				strMessage.PrintF( _S( 5897, "보석 개별 제거" ) );
				pUIManager->AddMessageBoxLString( MSGLCMD_SOCKET_SYSTEM_EMPTY, FALSE, strMessage, SOCKET_ONCEEMPTY );
				strMessage.PrintF( _S( 1220, "취소한다." ) );
				pUIManager->AddMessageBoxLString( MSGLCMD_SOCKET_SYSTEM_EMPTY, FALSE, strMessage );
			}
			else if( nResult == SOCKET_SHOP )
			{
				CUIManager* pUIManager = CUIManager::getSingleton();

				if(pUIManager->DoesMessageBoxLExist( MSGLCMD_SHOP_REQ ))
					return;

				pUIManager->GetShop()->OpenShop( m_nNPCIndex, -1, 0, m_fNPCPosX, m_fNPCPosZ );
			}
			else if( nResult == SOCKET_TALK )
			{

			}
			else if( nResult == SOCKET_EVENT )
			{

			}
			else
			{

			}
		}// end case MSGLCMD
		break;
		case MSGLCMD_SOCKET_SYSTEM_EMPTY:
		{
			if( nResult == SOCKET_EMPTY )
			{
				ToggleVisible( STAT_EMPTY );

				pUIManager->RearrangeOrder( UI_INVENTORY, TRUE );
			}
			else if( nResult == SOCKET_ONCEEMPTY )
			{
				ToggleVisible( STAT_ONCEEMPTY );

				pUIManager->RearrangeOrder( UI_INVENTORY, TRUE );
			}
		}
		break;
		case MSGLCMD_SOCKET_SYSTEM_JEWEL_COMPOS:
			{
				if( nResult == SOCKET_JEWEL_COMPOS )
				{
					ToggleVisible( STAT_JEWEL_COMPOS );

					pUIManager->RearrangeOrder( UI_INVENTORY, TRUE );
				}
				else if( nResult == SOCKET_CHAOSJEWEL_COMPOS )
				{
					ToggleVisible( STAT_CHAOSJEWEL_COMPOS );

					pUIManager->RearrangeOrder( UI_INVENTORY, TRUE );
				}
			}
			break;
	} // end switch.
}

void CUISocketSystem::MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	switch(nCommandCode)
	{
	case MSGCMD_JEWEL_COMPOS_JEWEL_COMPOS_SCROLL:
		{			
			if(bOK)
			{
				int nIndex = m_pIconsItem[JEWEL_COMPOS_BTN_SCROLL]->getIndex();
				int nMax = CUIManager::getSingleton()->GetInventory()->GetItemCount(nIndex);

				if (strInput.IsInteger() && _atoi64(strInput) <= nMax && _atoi64(strInput) > 0)
				{
					if (_atoi64(strInput) <= m_pIconsItem[JEWEL_COMPOS_BTN_SCROLL]->getItems()->Item_Sum)
					{
						m_nOneMoreComposScrollpay = _atoi64(strInput);
						m_pIconsItem[JEWEL_COMPOS_BTN_SCROLL]->setCount(m_nOneMoreComposScrollpay);
					}
					else
					{
						m_nOneMoreComposScrollpay = m_pIconsItem[JEWEL_COMPOS_BTN_SCROLL]->getItems()->Item_Sum;
					}
				}
				else if (_atoi64(strInput) > nMax)
				{
					if ( m_pIconsItem[JEWEL_COMPOS_BTN_SCROLL]->getItems()->Item_Sum >= nMax )
					{
						m_nOneMoreComposScrollpay = nMax;
					}
					else
					{
						m_nOneMoreComposScrollpay = m_pIconsItem[JEWEL_COMPOS_BTN_SCROLL]->getItems()->Item_Sum;
					}	
				}
				else
				{
					m_nOneMoreComposScrollpay = 1;
				}
			}
			else
			{
				m_pIconsItem[JEWEL_COMPOS_BTN_SCROLL]->clearIconData();
				m_nOneMoreComposScrollpay = 0;
			}
		}
	break;
	}

	if (pUIManager->DoesMessageBoxExist(nCommandCode))
		pUIManager->CloseMessageBox(nCommandCode);
}

void CUISocketSystem::UpdateCreateUI( const BOOL bSuccess /* = TRUE  */)
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if( bSuccess )
	{
		m_pIconsItem[1]->clearIconData();				// 스크롤 위치
		m_pIconsItem[2]->copyItem(m_pIconsItem[0]);		// 완성된 아이템 위치										
		m_pIconsItem[0]->clearIconData();				// 시작 아이템 위치.
		m_sqNeedMoney = 0;
		m_btnConfirm.SetEnable( FALSE );
		m_bProcessing = FALSE;
	}	
	else
	{
		m_pIconsItem[1]->clearIconData();	// 스크롤 위치.
		m_bActionStatus = SOCKET_CREATE_FAIL;
		m_btnConfirm.SetEnable( FALSE );
		m_bProcessing = FALSE;
	}

	pUIManager->GetInventory()->Lock( FALSE, FALSE, LOCK_SOCKETSYSTEM );
	pUIManager->GetInventory()->Lock( TRUE, TRUE, LOCK_SOCKETSYSTEM );
}

void CUISocketSystem::_UpdateBtn( int nBtnPos )
{
	if (m_pIconsItem[nBtnPos]->IsEmpty())
		return;

	CUIManager* pUIManager = CUIManager::getSingleton();

	int nIndex = m_pIconsItem[nBtnPos]->getItems()->Item_UniIndex;

	if (pUIManager->GetInventory()->GetItemVisble(nIndex) == TRUE &&
		m_pIconsItem[nBtnPos]->getItems()->IsEmptyItem() == FALSE)
	{
		m_pIconsItem[nBtnPos]->copyItem( pUIManager->GetInventory()->GetItemIcon(nIndex) );
	}
	else
	{
		m_pIconsItem[nBtnPos]->clearIconData();
	}	
}

void CUISocketSystem::UpdateCombineUI( const BOOL bSuccess, bool bResult  )
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	int i = 0;

	for( i = 0; i < ITEM_MAX_COUNT; ++i )
	{
		_UpdateBtn(i);
	}

	if (bResult == false)
	{
		BOOL bLock = FALSE;

		for( i = 1; i < ITEM_MAX_COUNT - 1; ++i )
		{
			if (m_pIconsItem[i]->IsEmpty() == false)
			{
				_SetButtonLock(TRUE, i);
				bLock = TRUE;
				m_btnConfirm.SetEnable( TRUE );
			}
		}

		if (bLock == FALSE)
		{
			_SetButtonLock(FALSE);
			m_sqNeedMoney = 0;
		}
	}

	m_bProcessing = FALSE;

	if (bSuccess)
		m_bActionStatus = SOCKET_CREATE_SUCCESS;
	else
		m_bActionStatus = SOCKET_CREATE_FAIL;

	pUIManager->GetInventory()->Lock( FALSE, FALSE, LOCK_SOCKETSYSTEM );
	pUIManager->GetInventory()->Lock( TRUE, TRUE, LOCK_SOCKETSYSTEM );
}

void CUISocketSystem::UpdateUpgradeUI( const int nUniqueIdx )
{
	for( int i = 0; i < ITEM_MAX_COUNT; ++i )
		m_pIconsItem[i]->clearIconData();

	CUIManager* pUIManager = CUIManager::getSingleton();

	m_pIconsItem[2]->copyItem( pUIManager->GetInventory()->GetItemIcon( nUniqueIdx ) );
	m_bProcessing = FALSE;
	pUIManager->GetInventory()->Lock( FALSE, FALSE, LOCK_SOCKETSYSTEM );
	pUIManager->GetInventory()->Lock( TRUE, TRUE, LOCK_SOCKETSYSTEM );
}

void CUISocketSystem::UpdateUI(const SBYTE sbPos, const int nUniqueIdx )
{
	for( int i = 0; i < ITEM_MAX_COUNT; ++i )
	{
		m_pIconsItem[i]->clearIconData();
	}

	CUIManager* pUIManager = CUIManager::getSingleton();

	// copy from inventory.
	// it's keep to last version.
	m_pIconsItem[sbPos]->copyItem( pUIManager->GetInventory()->GetItemIcon( nUniqueIdx ) );
	m_bActionStatus = SOCKET_CREATE_SUCCESS;
	m_sqNeedMoney = 0;
	m_btnConfirm.SetEnable( FALSE );
	m_bProcessing = FALSE;
	_SetButtonLock(FALSE);	
	pUIManager->GetInventory()->Lock( FALSE, FALSE, LOCK_SOCKETSYSTEM );
	pUIManager->GetInventory()->Lock( TRUE, TRUE, LOCK_SOCKETSYSTEM );
}

void CUISocketSystem::UpdateJewelComposUI( eJEWEL_COMPOS_TYPE UpdateType, const int nUniqueIdx )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	// 사운드 
	_PlaySound(JEWEL_SOUND_SHINY);
	// 결과 보석 업데이트
	m_pIconsItem[JEWEL_COMPOS_BTN_JEWEL_RESULT]->copyItem( pUIManager->GetInventory()->GetItemIcon( nUniqueIdx ) );
	// 돈 초기화
	m_sqNeedMoney = 0;
	// 슬롯 검사
	_CheckUiItemslot();
	// 소모 나스 체크
	_SetJewelcomposNas();
	// 한줄 가이드 엑션 체크
	_setActionCheck();

	pUIManager->GetInventory()->Lock( FALSE, FALSE, LOCK_SOCKETSYSTEM );
	pUIManager->GetInventory()->Lock( TRUE, TRUE, LOCK_SOCKETSYSTEM );
}

void CUISocketSystem::ClearProcessing()
{
	m_bProcessing = FALSE;
	for( int i = 0; i < ITEM_MAX_COUNT; ++i )
	{
		m_pIconsItem[i]->clearIconData();
	}
}
//////////////////////////////////////////////////////////////////////////
// user define network message functions.
void CUISocketSystem::SendCreateReq()
{
	CNetworkMessage nmMessage;
	RequestClient::doExSocketMake* packet = reinterpret_cast<RequestClient::doExSocketMake*>(nmMessage.nm_pubMessage);
	packet->type = MSG_EXTEND;
	packet->subType = htonl(MSG_EX_SOCKET);
	packet->thirdType = MSG_EX_SOCKET_MAKE_REQ;
	packet->npcIndex = m_nNPCVIdx;
	packet->tab_1 = m_pIconsItem[0]->getItems()->Item_Tab;
	packet->invenIndex_1 = m_pIconsItem[0]->getItems()->InvenIndex;
	packet->virualIndex_1 = m_pIconsItem[0]->getItems()->Item_UniIndex;
	packet->tab_2 = m_pIconsItem[1]->getItems()->Item_Tab;
	packet->invenIndex_2 = m_pIconsItem[1]->getItems()->InvenIndex;
	packet->virualIndex_2 = m_pIconsItem[1]->getItems()->Item_UniIndex;
	nmMessage.setSize( sizeof(*packet) );

	_pNetwork->SendToServerNew( nmMessage );
}
//-----------------------------------------------------------------
//	Jewel Index
//					2
//				7		3
//					1
//				6		4
//					5
//					|
//					|--	8 (파괴방지석)
//					0

void CUISocketSystem::SendCombineReq()
{
	CNetworkMessage nmMessage;
	RequestClient::doExSocketCombineJewel* packet = reinterpret_cast<RequestClient::doExSocketCombineJewel*>(nmMessage.nm_pubMessage);
	packet->type = MSG_EXTEND;
	packet->subType = htonl(MSG_EX_SOCKET);
	packet->thirdType = MSG_EX_SOCKET_COMBINE_JEWEL_REQ;
	packet->npcIndex = m_nNPCVIdx;
	packet->tab_1 = m_pIconsItem[DEF_TARGET_ITEM]->getItems()->Item_Tab;
	packet->invenIndex_1 = m_pIconsItem[DEF_TARGET_ITEM]->getItems()->InvenIndex;
	packet->virualIndex_1 = m_pIconsItem[DEF_TARGET_ITEM]->getItems()->Item_UniIndex;

 	int i = 0;
 	
	for (i = 1; i < ITEM_MAX_COUNT - 1; ++i)
 	{
 		if( !m_pIconsItem[i]->IsEmpty() )
 		{
 			if (m_bButtonLock[i])
 				continue;

			packet->tab_2 = m_pIconsItem[i]->getItems()->Item_Tab;
			packet->invenIndex_2 = m_pIconsItem[i]->getItems()->InvenIndex;
			packet->virualIndex_2 = m_pIconsItem[i]->getItems()->Item_UniIndex;
			packet->pos = (i - 1);

 			break;
 		}
 	}

	if ( m_pIconsItem[DEF_PREVENT_ITEM_SLOT]->IsEmpty() == FALSE ) // 스크롤이 있을경우 아이템 위치 idx, virtual idx
	{
		packet->tab_3 = m_pIconsItem[DEF_PREVENT_ITEM_SLOT]->getItems()->Item_Tab;
		packet->invenIndex_3 = m_pIconsItem[DEF_PREVENT_ITEM_SLOT]->getItems()->InvenIndex;
		packet->virualIndex_3 = m_pIconsItem[DEF_PREVENT_ITEM_SLOT]->getItems()->Item_UniIndex;
		m_bDefendScroll = TRUE;
	}
	else // 스크롤이 없을 경우 -1 -1 -1 을 보낸다.
	{
		packet->tab_3 = INVALID_SHORT_INDEX;
		packet->invenIndex_3 = INVALID_SHORT_INDEX;
		packet->virualIndex_3 = INVALID_SHORT_INDEX;
		m_bDefendScroll = FALSE;
	}

	nmMessage.setSize( sizeof(*packet) );

	_pNetwork->SendToServerNew( nmMessage );
}

void CUISocketSystem::SendUpgradeReq()
{
	// doExSocketUpgradeChaosJewel 공용 사용

	CNetworkMessage nmMessage;
	RequestClient::doExSocketUpgradeJewel* packet = reinterpret_cast<RequestClient::doExSocketUpgradeJewel*>(nmMessage.nm_pubMessage);
	packet->type = MSG_EXTEND;
	packet->subType = htonl(MSG_EX_SOCKET);
	packet->thirdType = m_DirectionType;
	packet->npcIndex = m_nNPCVIdx;
	packet->tab_1 = m_pIconsItem[JEWEL_COMPOS_BTN_JEWEL_1]->getItems()->Item_Tab;
	packet->invenIndex_1 = m_pIconsItem[JEWEL_COMPOS_BTN_JEWEL_1]->getItems()->InvenIndex;
	packet->virualIndex_1 = m_pIconsItem[JEWEL_COMPOS_BTN_JEWEL_1]->getItems()->Item_UniIndex;
	packet->tab_2 = m_pIconsItem[JEWEL_COMPOS_BTN_JEWEL_2]->getItems()->Item_Tab;
	packet->invenIndex_2 = m_pIconsItem[JEWEL_COMPOS_BTN_JEWEL_2]->getItems()->InvenIndex;
	packet->virualIndex_2 = m_pIconsItem[JEWEL_COMPOS_BTN_JEWEL_2]->getItems()->Item_UniIndex;
	packet->tab_3 = m_pIconsItem[JEWEL_COMPOS_BTN_JEWEL_3]->getItems()->Item_Tab;
	packet->invenIndex_3 = m_pIconsItem[JEWEL_COMPOS_BTN_JEWEL_3]->getItems()->InvenIndex;
	packet->virualIndex_3 = m_pIconsItem[JEWEL_COMPOS_BTN_JEWEL_3]->getItems()->Item_UniIndex;

	if (m_pIconsItem[JEWEL_COMPOS_BTN_SCROLL]->IsEmpty())
	{
		packet->tab_4 = 0;
		packet->invenIndex_4 = 0;
		packet->virualIndex_4 = 0;
	}
	else
	{
		packet->tab_4 = m_pIconsItem[JEWEL_COMPOS_BTN_SCROLL]->getItems()->Item_Tab;
		packet->invenIndex_4 = m_pIconsItem[JEWEL_COMPOS_BTN_SCROLL]->getItems()->InvenIndex;
		packet->virualIndex_4 = m_pIconsItem[JEWEL_COMPOS_BTN_SCROLL]->getItems()->Item_UniIndex;
	}

	packet->itemCount = m_nOneMoreComposScrollpay;
	nmMessage.setSize( sizeof(*packet) );

	_pNetwork->SendToServerNew( nmMessage );
}

void CUISocketSystem::SendInfoReq()
{
	CNetworkMessage nm(MSG_EXTEND);
	nm << (SLONG)MSG_EX_SOCKET;
	nm << (UBYTE)MSG_EX_SOCKET_INFO_REQ;
	// get info slot.
	nm << (SBYTE)(m_pIconsItem[0]->getItems()->InvenIndex);
	nm << (SLONG)(m_pIconsItem[0]->getItems()->Item_UniIndex);

	_pNetwork->SendToServerNew( nm );
}

void CUISocketSystem::SendCleanAllJewelReq()
{
	CNetworkMessage nmMessage;
	RequestClient::doExSocketCleanJewel* packet = reinterpret_cast<RequestClient::doExSocketCleanJewel*>(nmMessage.nm_pubMessage);
	packet->type = MSG_EXTEND;
	packet->subType = htonl(MSG_EX_SOCKET);
	packet->thirdType = MSG_EX_SOCKET_CLEAN_JEWEL_REQ;
	packet->npcIndex = m_nNPCVIdx;
	packet->tab = m_pIconsItem[0]->getItems()->Item_Tab;
	packet->invenIndex = m_pIconsItem[0]->getItems()->InvenIndex;
	packet->virualIndex = m_pIconsItem[0]->getItems()->Item_UniIndex;

	nmMessage.setSize( sizeof(*packet) );

	_pNetwork->SendToServerNew( nmMessage );
}

void CUISocketSystem::SendCleanOneJewelReq(SWORD tab, SWORD inven_idx, SLONG index)
{
	CNetworkMessage nmMessage;
	RequestClient::doExSocketCleanOneJewel* packet = reinterpret_cast<RequestClient::doExSocketCleanOneJewel*>(nmMessage.nm_pubMessage);
	packet->type = MSG_EXTEND;
	packet->subType = htonl(MSG_EX_SOCKET);
	packet->thirdType = MSG_EX_SOCKET_CLEAN_ONE_JEWEL_REQ;
	packet->npcIndex = m_nNPCVIdx;
	packet->tab_1 = m_pIconEmpty->getItems()->Item_Tab;
	packet->invenIndex_1 = m_pIconEmpty->getItems()->InvenIndex;
	packet->virualIndex_1 = m_pIconEmpty->getItems()->Item_UniIndex;

	packet->tab_2 = tab;
	packet->invenIndex_2 = inven_idx;
	packet->virualIndex_2 = index;

	packet->pos = m_nEmptyJewelPos;

	nmMessage.setSize( sizeof(*packet) );

	_pNetwork->SendToServerNew( nmMessage );
}

void CUISocketSystem::CommandTimer(pReq req, UINT _second /* = 5  */)
{
	m_bProcessing = TRUE;
	this->m_fnReq = req;
	m_uEndTime = _second;
	m_bUseTimer = TRUE;
	m_pbBar.SetProgressPos( 0 );
	m_pbBar.StartProgress();
	m_uStartTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();

	CUIManager* pUIManager = CUIManager::getSingleton();

	pUIManager->GetInventory()->Lock( FALSE, FALSE, LOCK_SOCKETSYSTEM );
	pUIManager->GetInventory()->Lock( TRUE, FALSE, LOCK_SOCKETSYSTEM );
}

const BOOL CUISocketSystem::IsOpenedUI() const
{
	return IsVisible() || CUIManager::getSingleton()->DoesMessageBoxLExist( MSGLCMD_SOCKET_SYSTEM );
}

void CUISocketSystem::CloseAllUI()
{
	if (m_bStartDirection)
		return;

	CloseSocketSystem();
	CUIManager::getSingleton()->CloseMessageBoxL( MSGLCMD_SOCKET_SYSTEM );	
}

void CUISocketSystem::absCloseAllUI()
{
	CloseSocketSystem();
	CUIManager::getSingleton()->CloseMessageBoxL( MSGLCMD_SOCKET_SYSTEM );	
}

int CUISocketSystem::IsEmptyInvenSlotCount()
{
	if( CUIManager::getSingleton()->GetInventory()->IsInventoryFull() )
		return 0;

	int	  nInvenSlotMax =  INVEN_SLOT_COL * INVEN_SLOT_ROW_TOTAL;
	SQUAD count = CUIManager::getSingleton()->GetInventory()->GetItemAll();

	return nInvenSlotMax - count;
}
