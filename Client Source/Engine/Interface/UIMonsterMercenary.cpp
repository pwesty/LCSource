#include "stdh.h"

#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Interface/UISummon.h>
#include <Engine/Interface/UIMonsterMercenary.h>
#include <Engine/Interface/UIQuickSlot.h>
#include <Common/Packet/ptype_char_status.h>


// ----------------------------------------------------------------------------
// Name : CUIMonsterMercenary()
// Desc :
// ----------------------------------------------------------------------------
CUIMonsterMercenary::CUIMonsterMercenary()
{
	m_ptdAddTexture = NULL;
}

// ----------------------------------------------------------------------------
// Name : ~CUIMonsterMercenary()
// Desc :
// ----------------------------------------------------------------------------
CUIMonsterMercenary::~CUIMonsterMercenary()
{
	Destroy();

	STOCK_RELEASE( m_ptdAddTexture );	
}

// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUIMonsterMercenary::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	CUIWindow::Create(pParentWnd, nX, nY, nWidth, nHeight);

	m_bPopupShown = FALSE;
	m_nItemIndex = 0;

	m_rcTitle.SetRect( 0, 0, 18, 44 );
	m_rcHP.SetRect( 60, 27, 198, 36 );

	m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\Portal.tex" ) );
	FLOAT	fTexWidth = m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight = m_ptdBaseTexture->GetPixHeight();

	m_rtBackground.SetUV( 304, 48, 512, 92, fTexWidth, fTexHeight );

	m_rtHP.SetUV( 346, 96, 349, 104, fTexWidth, fTexHeight );

	m_btnClose.Create( this, CTString( "" ), 187, 6, 16, 16 );
	m_btnClose.SetUV( UBS_IDLE, 308, 94, 324, 110, fTexWidth, fTexHeight );
	m_btnClose.SetUV( UBS_CLICK,326, 94, 342, 110, fTexWidth, fTexHeight );
	m_btnClose.CopyUV( UBS_IDLE, UBS_DISABLE );
	m_btnClose.CopyUV( UBS_IDLE, UBS_ON );
	
	m_ptdAddTexture = CreateTexture( CTString( "Data\\Interface\\CommonBtn.tex" ) );
	fTexWidth = m_ptdAddTexture->GetPixWidth();
	fTexHeight = m_ptdAddTexture->GetPixHeight();

	m_bxPopupInfo.SetBoxUV(m_ptdAddTexture,20,WRect(0,137,141,227));

	m_rsMercenaryInfo.Create(NULL, 0, 0, UI_MONSTER_MERCENARY_INFO_WIDTH-30, 50);

}

// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
void CUIMonsterMercenary::Render()
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	CDrawPort* pDrawPort = pUIManager->GetDrawPort();

	pDrawPort->InitTextureData( m_ptdBaseTexture );

	pDrawPort->AddTexture( m_nPosX, m_nPosY, m_nPosX + m_nWidth, m_nPosY + m_nHeight,
		m_rtBackground.U0, m_rtBackground.V0, m_rtBackground.U1, m_rtBackground.V1,
		0xFFFFFFFF );
	
	float fHP = _pNetwork->MonsterMercenaryInfo.hp;
	float fMaxHP = _pNetwork->MonsterMercenaryInfo.maxHp;
	FLOAT	fHPRatio = Clamp( fHP / fMaxHP, 0.0f, 1.0f );	
	m_rcHP.Right = m_rcHP.Left + ( 138 * fHPRatio );

	pDrawPort->AddTexture( m_nPosX + m_rcHP.Left, m_nPosY + m_rcHP.Top,
		m_nPosX + m_rcHP.Right, m_nPosY + m_rcHP.Bottom,
		m_rtHP.U0, m_rtHP.V0, m_rtHP.U1, m_rtHP.V1,
		0xFFFFFFFF );

	m_btnClose.Render();

	pDrawPort->FlushRenderingQueue();

	int mobImgX = m_nPosX + 19;
	int mobImgY = m_nPosY + 6;

	pDrawPort->AddBtnTexture( m_nTexID, mobImgX, mobImgY, mobImgX + BTN_SIZE, mobImgY + BTN_SIZE,
											m_rtMobImg.U0, m_rtMobImg.V0, m_rtMobImg.U1, m_rtMobImg.V1,
											0xFFFFFFFF );

	pDrawPort->FlushBtnRenderingQueue( UBET_ITEM );

	if( m_bPopupShown )
	{
		int boxWidth = m_rsMercenaryInfo.GetWidth() + 30;
		int	boxHeight = m_rsMercenaryInfo.GetHeight() + 28;

		int boxPosX = m_nPosX + m_nWidth;
		int boxPosY = m_nPosY + m_nHeight - boxHeight;

		if( boxPosX + boxWidth > pUIManager->GetMaxI() )
		{
			boxPosX = m_nPosX - boxWidth;
		}

		if (boxPosY < pUIManager->GetMinJ())
		{
			boxPosY = m_nPosY;
		}

		pDrawPort->InitTextureData( m_ptdAddTexture );

		m_bxPopupInfo.SetBoxPos(WRect( 0, 0, boxWidth, boxHeight ));
		m_bxPopupInfo.Render( boxPosX , boxPosY );

		pDrawPort->FlushRenderingQueue();
		
		m_rsMercenaryInfo.SetPos( boxPosX + 15, boxPosY + 14);
		m_rsMercenaryInfo.Render();

	}

	pDrawPort->PutTextExCX(CMobData::getData(_pNetwork->MonsterMercenaryInfo.index)->GetName(),  m_nPosX + 121, m_nPosY + 8, 0xFFFFFFFF);

	pDrawPort->EndTextEx();
}

// ----------------------------------------------------------------------------
// Name : ResetPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIMonsterMercenary::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	SetPos( pUIManager->GetQuickSlot()->GetAbsPosX() - m_nWidth, 
		pUIManager->GetQuickSlot()->GetAbsPosY() - (m_nHeight * 2));
}

// ----------------------------------------------------------------------------
// Name : AdjustPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIMonsterMercenary::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUIMonsterMercenary::MouseMessage( MSG *pMsg )
{
	static BOOL bTitleBarClick = FALSE;

	static int	nOldX, nOldY;
	int	nX = LOWORD( pMsg->lParam );
	int	nY = HIWORD( pMsg->lParam );

	WMSG_RESULT	wmsgResult;

	switch( pMsg->message )
	{
		case WM_MOUSEMOVE:
			{
				m_btnClose.MouseMessage( pMsg );

				if( IsInside( nX, nY ) )
					CUIManager::getSingleton()->SetMouseCursorInsideUIs();

				int	ndX = nX - nOldX;
				int	ndY = nY - nOldY;

				if( bTitleBarClick && ( pMsg->wParam & MK_LBUTTON ) )
				{
					
					nOldX = nX;	nOldY = nY;
					
					Move( ndX, ndY );

					return WMSG_SUCCESS;
				}
			}
			break;

		case WM_LBUTTONDOWN:
			{
				if( IsInside( nX, nY ) == FALSE )
					break;

				nOldX = nX;		nOldY = nY;
				if( IsInsideRect( nX, nY, m_rcTitle ) )	
				{
					bTitleBarClick = TRUE;
				}
				
				if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}

				CUIManager::getSingleton()->RearrangeOrder( UI_MONSTER_MERCENARY, TRUE );
				return WMSG_SUCCESS;
			}
			break;

		case WM_LBUTTONUP:
			{
				bTitleBarClick = FALSE;

				CUIManager* pUIManager = CUIManager::getSingleton();

				if (pUIManager->GetDragIcon() == NULL)
				{
					if( !IsFocused() )
						return WMSG_FAIL;

					if( ( wmsgResult = m_btnClose.MouseMessage( pMsg ) ) != WMSG_FAIL )
					{
						if( wmsgResult == WMSG_COMMAND )
						{
							m_bPopupShown = !m_bPopupShown;
							if( m_bPopupShown )
								SetMonsterMercenaryInfo();
						}

						return WMSG_SUCCESS;
					}
				}
				else
				{
					if( IsInside( nX, nY ) )	// [2010/12/09 : Sora] UI내부에 있을때만 버튼을 리셋해야 한다.
					{
						pUIManager->ResetHoldBtn();
						return WMSG_SUCCESS;
					}
				}
			}
			break;
	}
	return WMSG_FAIL;
}

// ----------------------------------------------------------------------------
// Name : ToggleMonsterMercenary()
// Desc : 용병정보창 열고 닫기
// ----------------------------------------------------------------------------
void CUIMonsterMercenary::ToggleMonsterMercenary(int toggle )
{
	BOOL bShown = toggle > 0 ? TRUE : FALSE;
	
	m_bPopupShown = FALSE;

	CUIManager* pUIManager = CUIManager::getSingleton();

	pUIManager->RearrangeOrder( UI_MONSTER_MERCENARY, bShown );
}

// ----------------------------------------------------------------------------
// Name : GetMonsterGrade()
// Desc : 몬스터 등급 표시
// ----------------------------------------------------------------------------
CTString CUIMonsterMercenary::GetMonsterGrade( int grade )
{
	switch( grade )
	{
		case 1:
			return _S( 5152, "S등급");
		case 2:
			return _S( 5153, "A등급");
		case 3:
			return _S( 5154, "B등급");
		case 4:
			return _S( 5155, "C등급");
	}

	return _s("");
}

// ----------------------------------------------------------------------------
// Name : SetMonsterMercenaryInfo()
// Desc : 용병창 보조 정보 설정
// ----------------------------------------------------------------------------
void CUIMonsterMercenary::SetMonsterMercenaryInfo()
{
	CTString strtemp;
	CMobData* MD = CMobData::getData(_pNetwork->MonsterMercenaryInfo.index);

	m_rsMercenaryInfo.ClearString();

	strtemp.PrintF(_s("%s%s"), _S( 5151, "[용병]"), CMobData::getData(_pNetwork->MonsterMercenaryInfo.index)->GetName());
	m_rsMercenaryInfo.AddString(strtemp);

	strtemp.PrintF(_S( 5156, "등급 : %s"), CUIManager::getSingleton()->GetMonsterMercenary()->GetMonsterGrade(_pNetwork->MonsterMercenaryInfo.grade) );
	m_rsMercenaryInfo.AddString(strtemp);

	strtemp.PrintF(_S( 5157, "스킬 : %s"), _pNetwork->GetSkillData( MD->GetSkill0Index() ).GetName() );
	m_rsMercenaryInfo.AddString(strtemp);

	strtemp.PrintF(_s("%s"), _pNetwork->GetSkillData( MD->GetSkill1Index() ).GetName() );
	m_rsMercenaryInfo.AddString(strtemp);

	m_rsMercenaryInfo.SetHeight(m_rsMercenaryInfo.GetMaxStrHeight());
}

// ----------------------------------------------------------------------------
// Name : SetUseItemData()
// Desc : 용병창에 쓸 버튼 이미지 설정 
// ----------------------------------------------------------------------------
void CUIMonsterMercenary::SetUseItemData( INDEX index )
{
	CItemData* pItemData = _pNetwork->GetItemData( index );

	m_rtMobImg.SetUV(	pItemData->GetIconTexCol() * BTN_SIZE,
						pItemData->GetIconTexRow() * BTN_SIZE,
						(pItemData->GetIconTexCol()+1) * BTN_SIZE,
						(pItemData->GetIconTexRow()+1) * BTN_SIZE,
						_pUIBtnTexMgr->GetTexWidth( UBET_ITEM, pItemData->GetIconTexID() ),
						_pUIBtnTexMgr->GetTexHeight( UBET_ITEM, pItemData->GetIconTexID() ) );
	
	m_nTexID = pItemData->GetIconTexID();
}

// ----------------------------------------------------------------------------
// Name : ReceiveMercenaryMessage()
// Desc : 
// ----------------------------------------------------------------------------
BOOL CUIMonsterMercenary::ReceiveMercenaryMessage( CNetworkMessage* istr, SLONG hp, SLONG maxhp, SLONG mp, SLONG maxmp )
{
	UpdateClient::charStatusNpc* pPack = reinterpret_cast<UpdateClient::charStatusNpc*>(istr->GetBuffer());
	
	if( pPack->ownerIndex == _pNetwork->MyCharacterInfo.index )
	{
		_pNetwork->MonsterMercenaryInfo.grade = pPack->classIndex;
		_pNetwork->MonsterMercenaryInfo.attack = pPack->melee;
		_pNetwork->MonsterMercenaryInfo.defence = pPack->defence;
		_pNetwork->MonsterMercenaryInfo.magicAttack = pPack->magic;
		_pNetwork->MonsterMercenaryInfo.magicDefence = pPack->resist;
		_pNetwork->MonsterMercenaryInfo.hp = hp;
		_pNetwork->MonsterMercenaryInfo.maxHp = maxhp;
		_pNetwork->MonsterMercenaryInfo.mp = mp;
		_pNetwork->MonsterMercenaryInfo.maxMp = maxmp;

		return true;
	}

	return false;
}