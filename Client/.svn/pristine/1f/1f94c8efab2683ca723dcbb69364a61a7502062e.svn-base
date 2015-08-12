#include "stdh.h"

// 헤더 정리. [12/2/2009 rumist]
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Interface/UIPetTarget.h>
#include <Engine/Interface/UIPetInfo.h>
#include <Engine/Entities/InternalClasses.h>
#include <Engine/Interface/UIPlayerInfo.h>
#include <Engine/Interface/UIWildPetInfo.h>
#include <Engine/Contents/Base/UIQuestBookNew.h>
#include <Engine/Info/MyInfo.h>

#define INFOMARK_POS_X 108
#define INFOMARK_POS_Y 3

#define INFOMARK_SIZE 16

// ----------------------------------------------------------------------------
// Name : CUIPetTargetInfo()
// Desc : Constructor
// ----------------------------------------------------------------------------
CUIPetTargetInfo::CUIPetTargetInfo()
{	
	m_bShowTooltip = FALSE;
}

// ----------------------------------------------------------------------------
// Name : ~CUIPetTargetInfo()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUIPetTargetInfo::~CUIPetTargetInfo()
{
}

// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUIPetTargetInfo::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	CUIWindow::Create(pParentWnd, nX, nY, nWidth, nHeight);

	// Region of each part
	m_rcTitle.SetRect( 0, 0, 140, 22 );
	m_rcHP.SetRect( 10, 27, 130, 35 );
	m_rcHPBack.SetRect( 5, 25, 135, 37 );
	m_rcHungry.SetRect( 10, 41, 130, 46 );	
	m_rcHungryBack.SetRect( 5, 40, 135, 48 );

	// Create inventory texture
	m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\TargetInfo.tex" ) );
	FLOAT	fTexWidth = m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight = m_ptdBaseTexture->GetPixHeight();

	// UV Coordinate of each part
	// Background
	m_rtBackground1.SetUV( 0, 0, 140, 26, fTexWidth, fTexHeight );
	m_rtBackground2.SetUV( 0, 27, 140, 43, fTexWidth, fTexHeight );

	// HP
	m_rtHP.SetUV( 1, 44, 2, 52, fTexWidth, fTexHeight );

	// Hungry
	m_rtHungry.SetUV( 6, 44, 8, 52, fTexWidth, fTexHeight );

	// HP background
	m_rtHPBack.SetUV( 10, 44, 140, 56, fTexWidth, fTexHeight );

	// Hungry background
	m_rtHungryBack.SetUV( 10, 44, 140, 56, fTexWidth, fTexHeight );	

	m_btnPetInfo.Create( this, CTString( "" ), INFOMARK_POS_X, INFOMARK_POS_Y, INFOMARK_SIZE, INFOMARK_SIZE );
	m_btnPetInfo.SetUV( UBS_IDLE, 159, 28, 175, 44, fTexWidth, fTexHeight );
	m_btnPetInfo.SetUV( UBS_CLICK, 159, 28, 175, 44, fTexWidth, fTexHeight );
	m_btnPetInfo.CopyUV( UBS_IDLE, UBS_ON );
	m_btnPetInfo.CopyUV( UBS_IDLE, UBS_DISABLE );
	
	// Set tooptip UV
	m_rtToolTipL.SetUV(142,4,146,24,fTexWidth, fTexHeight );
	m_rtToolTipM.SetUV(146,4,156,24,fTexWidth, fTexHeight );
	m_rtToolTipR.SetUV(156,4,161,24,fTexWidth, fTexHeight );

	
}


// ----------------------------------------------------------------------------
// Name : ResetPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIPetTargetInfo::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	//SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2, pixMinJ );
	SetPos( pixMinI + pUIManager->GetPlayerInfo()->GetWidth() + 1, pixMinJ + (pUIManager->GetPlayerInfo()->GetHeight() - GetHeight()) );
}

// ----------------------------------------------------------------------------
// Name : AdjustPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIPetTargetInfo::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

// ----------------------------------------------------------------------------
// Name : UpdateHPInfo()
// Desc :
// ----------------------------------------------------------------------------
void CUIPetTargetInfo::UpdateHPInfo()
{
	// HP
	// Modified by yjpark
	ObjInfo* pInfo = ObjInfo::getSingleton();
	FLOAT	fHPRatio = Clamp( pInfo->GetMyPetInfo()->fHealth / pInfo->GetMyPetInfo()->fMaxHealth, 0.0f, 1.0f );
	// Modified by yjpark
	m_rcHP.Right = m_rcHP.Left + PETTARGETINFO_BAR_WIDTH * fHPRatio;
}

// ----------------------------------------------------------------------------
// Name : UpdateHungryInfo()
// Desc :
// ----------------------------------------------------------------------------
void CUIPetTargetInfo::UpdateHungryInfo()
{
	// Hungry
	// Modified by yjpark
	ObjInfo* pInfo = ObjInfo::getSingleton();
	FLOAT	fHungryRatio = Clamp( pInfo->GetMyPetInfo()->fHungry / pInfo->GetMyPetInfo()->fMaxHungry, 0.0f, 1.0f );
	// Modified by yjpark
	m_rcHungry.Right = m_rcHungry.Left + PETTARGETINFO_BAR_WIDTH * fHungryRatio;
}

// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
void CUIPetTargetInfo::RenderTooltip()
{
	extern INDEX g_iEnterChat;
	int tv_strSize,tv_posX,tv_posY;
	m_strToolTip = _S(2173,"펫 정보창") ;
	if( g_iEnterChat )
		m_strToolTip+=CTString("(P,ALT+P)");
	else 
		m_strToolTip+=CTString("(ALT+P)");
	tv_posX =m_nPosX + m_btnPetInfo.GetPosX() -  m_btnPetInfo.GetWidth()/2 ;
	tv_posY = m_nPosY + m_btnPetInfo.GetPosY();
	tv_strSize = m_strToolTip.Length() * (_pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing());
//	CTString tv_err;
//	tv_err.PrintF("%d %d %d\n",m_btnPetInfo.GetPosX(),m_btnPetInfo.GetPosY(),m_btnPetInfo.GetWidth());
//	OutputDebugString(tv_err);

	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	// Set target information texture
	pDrawPort->InitTextureData( m_ptdBaseTexture );

	pDrawPort->AddTexture(	tv_posX, tv_posY - _pUIFontTexMgr->GetFontHeight() -5, tv_posX + 4, tv_posY - 2,
										m_rtToolTipL.U0, m_rtToolTipL.V0, m_rtToolTipL.U1, m_rtToolTipL.V1,
										0xFFFFFFFF );
	pDrawPort->AddTexture( tv_posX +4, tv_posY - _pUIFontTexMgr->GetFontHeight()-5, 
										tv_posX +4 + tv_strSize, tv_posY -2,
										m_rtToolTipM.U0, m_rtToolTipM.V0, m_rtToolTipM.U1, m_rtToolTipM.V1,
										0xFFFFFFFF );
	pDrawPort->AddTexture( tv_posX +4 + tv_strSize, tv_posY - _pUIFontTexMgr->GetFontHeight()-5, 
										tv_posX +4 + tv_strSize +5, tv_posY -2,
										m_rtToolTipR.U0, m_rtToolTipR.V0, m_rtToolTipR.U1, m_rtToolTipR.V1,
										0xFFFFFFFF );
	// show text
	pDrawPort->PutTextEx( m_strToolTip, tv_posX +4,
										tv_posY -_pUIFontTexMgr->GetFontHeight()-4, 0xFFFFFFFF );
	// Render all elements
	pDrawPort->FlushRenderingQueue();
	// Flush all render text queue
	pDrawPort->EndTextEx();	
	
}

// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
void CUIPetTargetInfo::Render()
{
	ObjInfo* pInfo = ObjInfo::getSingleton();

	if( !pInfo->GetMyPetInfo()->bIsActive || _pNetwork->MyWearItem[WEAR_PET].IsEmptyItem() == TRUE )
		return;

	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	// Set target information texture
	pDrawPort->InitTextureData( m_ptdBaseTexture );
	
	// Add render regions
	// Background
	pDrawPort->AddTexture( m_nPosX, m_nPosY, m_nPosX + m_nWidth, m_nPosY + 26,
		m_rtBackground1.U0, m_rtBackground1.V0, m_rtBackground1.U1, m_rtBackground1.V1,
		0xFFFFFFFF );

	pDrawPort->AddTexture( m_nPosX, m_nPosY + 26, m_nPosX + m_nWidth, m_nPosY + m_nHeight,
		m_rtBackground2.U0, m_rtBackground2.V0, m_rtBackground2.U1, m_rtBackground2.V1,
		0xFFFFFFFF );
	
	// Render all elements
	pDrawPort->FlushRenderingQueue();
	
	
	// Update info of target
	UpdateHPInfo();
	UpdateHungryInfo();
	
	// Background of HP
	pDrawPort->AddTexture( m_nPosX + m_rcHPBack.Left, m_nPosY + m_rcHPBack.Top,
		m_nPosX + m_rcHPBack.Right, m_nPosY + m_rcHPBack.Bottom,
		m_rtHPBack.U0, m_rtHPBack.V0, m_rtHPBack.U1, m_rtHPBack.V1,
		0xFFFFFFFF );

	// Background of Hungry
	pDrawPort->AddTexture( m_nPosX + m_rcHungryBack.Left, m_nPosY + m_rcHungryBack.Top,
		m_nPosX + m_rcHungryBack.Right, m_nPosY + m_rcHungryBack.Bottom,
		m_rtHungryBack.U0, m_rtHungryBack.V0, m_rtHungryBack.U1, m_rtHungryBack.V1,
		0xFFFFFFFF );
	
	// HP
	pDrawPort->AddTexture( m_nPosX + m_rcHP.Left, m_nPosY + m_rcHP.Top,
		m_nPosX + m_rcHP.Right, m_nPosY + m_rcHP.Bottom,
		m_rtHP.U0, m_rtHP.V0, m_rtHP.U1, m_rtHP.V1,
		0xFFFFFFFF );

	// Hungry
	pDrawPort->AddTexture( m_nPosX + m_rcHungry.Left, m_nPosY + m_rcHungry.Top,
		m_nPosX + m_rcHungry.Right, m_nPosY + m_rcHungry.Bottom,
		m_rtHungry.U0, m_rtHungry.V0, m_rtHungry.U1, m_rtHungry.V1,
		0xFFFFFFFF );

	m_btnPetInfo.Render();
	
	// Render all elements
	pDrawPort->FlushRenderingQueue();
	
	// Pet name
	if( pInfo->GetMyPetInfo()->strNameCard.Length() >0)
	{
		pDrawPort->PutTextCharExCX( pInfo->GetMyPetInfo()->strNameCard, 0,
		m_nPosX + m_nWidth / 2, m_nPosY + PETTARGETINFO_NAME_SY,
		0x008BAAFF );
	}
	else
	{
		pDrawPort->PutTextCharExCX( PetInfo().GetName(pInfo->GetMyPetInfo()->iType, pInfo->GetMyPetInfo()->iAge), 0,
			m_nPosX + m_nWidth / 2, m_nPosY + PETTARGETINFO_NAME_SY,
			0x008BAAFF );
	}

	CTString strTemp;
	strTemp.PrintF("%d", pInfo->GetMyPetInfo()->iLevel);
	pDrawPort->PutTextEx( strTemp, m_nPosX + 18,
										m_nPosY + 5, 0xF2F2F2B2 );
	// Flush all render text queue
	pDrawPort->EndTextEx();		

	// SHOW TOOLTIP
	if(m_bShowTooltip)
	{
		RenderTooltip();
	}
}


// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUIPetTargetInfo::MouseMessage( MSG *pMsg )
{
	ObjInfo* pInfo = ObjInfo::getSingleton();
	// If target is not exist
	if( !pInfo->GetMyPetInfo()->bIsActive )
		return WMSG_FAIL;

	// Title bar
	static BOOL bTitleBarClick = FALSE;

	// Mouse point
	static int	nOldX, nOldY;
	int	nX = LOWORD( pMsg->lParam );
	int	nY = HIWORD( pMsg->lParam );

	// Mouse message
	switch( pMsg->message )
	{
	case WM_MOUSEMOVE:
		{
			m_bShowTooltip = FALSE;
			if( IsInside( nX, nY ) )
			{
				CUIManager::getSingleton()->SetMouseCursorInsideUIs();
			}

			int	ndX = nX - nOldX;
			int	ndY = nY - nOldY;

			// Move target information
			if( bTitleBarClick && ( pMsg->wParam & MK_LBUTTON ) )
			{
				nOldX = nX;	nOldY = nY;

				Move( ndX, ndY );

				// If target is disapeared
				if( !pInfo->GetMyPetInfo()->bIsActive )
					bTitleBarClick = FALSE;

				return WMSG_SUCCESS;
			}

			if( m_btnPetInfo.MouseMessage( pMsg ) != WMSG_FAIL )
			{
				m_bShowTooltip = TRUE;
				CUIManager::getSingleton()->RearrangeOrder( UI_PETTARGETINFO, TRUE );
				return WMSG_SUCCESS;
			}
			
		}
		break;

	case WM_LBUTTONDOWN:
		{
			if( IsInside( nX, nY ) )
			{
				nOldX = nX;		nOldY = nY;

				// Title bar
				if( IsInsideRect( nX, nY, m_rcTitle ) )
				{
					bTitleBarClick = TRUE;
				}
				else
				{
					if( pInfo->GetMyPetInfo()->pen_pEntity )
					{
						CEntity			*penPlEntity;
						CPlayerEntity	*penPlayerEntity;
						penPlEntity = CEntity::GetPlayerEntity( 0 );
						penPlayerEntity = static_cast<CPlayerEntity *>(penPlEntity);
						penPlayerEntity->SetTarget( pInfo->GetMyPetInfo()->pen_pEntity );
					}
				}

				if( m_btnPetInfo.MouseMessage( pMsg ) != WMSG_FAIL )
				{
				}

				CUIManager::getSingleton()->RearrangeOrder( UI_PETTARGETINFO, TRUE );
				return WMSG_SUCCESS;
			}
		}
		break;

	case WM_LBUTTONUP:
		{
			CUIManager* pUIManager = CUIManager::getSingleton();

			// If holding button doesn't exist
			if (pUIManager->GetDragIcon() == NULL)
			{
				// Title bar
				bTitleBarClick = FALSE;
				
				// If Pet Target isn't focused
				if( !IsFocused() )
					return WMSG_FAIL;

				if( IsInside( nX, nY ) )
				{
					// Close button
					if( m_btnPetInfo.MouseMessage( pMsg )  == WMSG_COMMAND )
					{					
						if( pUIManager->GetPetInfo()->IsVisible() )
						{
							pUIManager->GetPetInfo()->ClosePetInfo();								
						}
						else
						{
							pUIManager->GetPetInfo()->OpenPetInfo();
						}						
					
						return WMSG_SUCCESS;
					}
				}
			}
		}break;

	case WM_LBUTTONDBLCLK:
		{
			if( IsInside( nX, nY ) )
				return WMSG_SUCCESS;
		}
		break;
	}

	return WMSG_FAIL;
}


// ----------------------------------------------------------------------------
// Name : CUIPetTargetInfo()
// Desc : Constructor
// ----------------------------------------------------------------------------
CUIWildPetTargetInfo::CUIWildPetTargetInfo()
{	
	m_bShowTooltip = FALSE;
	m_ptdButtonTexture = NULL;
	m_bSizeEx = FALSE;
	fAccuExp = 0.0f;
}

// ----------------------------------------------------------------------------
// Name : ~CUIPetTargetInfo()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUIWildPetTargetInfo::~CUIWildPetTargetInfo()
{
	Destroy();

	STOCK_RELEASE(m_ptdButtonTexture);

	fAccuExp = 0.0f;
}
// ----------------------------------------------------------------------------
// Name : ResetPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIWildPetTargetInfo::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	//SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2, pixMinJ );
	SetPos( pixMinI + pUIManager->GetPlayerInfo()->GetWidth() + 1, pixMinJ + (pUIManager->GetPlayerInfo()->GetHeight() - GetHeight()) );
}

// ----------------------------------------------------------------------------
// Name : AdjustPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIWildPetTargetInfo::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUIWildPetTargetInfo::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	CUIWindow::Create(pParentWnd, nX, nY, nWidth, nHeight);

	// Create inventory texture
	m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\new_PetInfo.tex") );
	m_ptdButtonTexture = CreateTexture(CTString("Data\\Interface\\CommonBtn.tex"));
	FLOAT	fTexWidth = m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight = m_ptdBaseTexture->GetPixHeight();

	m_bxTitle.SetBoxUV(m_ptdBaseTexture,32 ,28 ,WRect(4,429,166,505));
	m_bxTitle.SetBoxPos(WRect( 0, 0, 160, 77 ));
	
	
	m_rcTitle.SetRect( 0, 0, 130, 22 );
	
	m_rtHP.SetUV(175,415,178,422,fTexWidth,fTexHeight);
	m_rtMP.SetUV(180,415,183,422,fTexWidth,fTexHeight);
	m_rtEXP.SetUV(185,415,188,422,fTexWidth,fTexHeight);
	m_rtAccuExp.SetUV(175, 430, 188, 441, fTexWidth, fTexHeight);

	m_bxAccuState.SetBoxUV(m_ptdBaseTexture, 10, WRect(171, 380, 233, 400));
	m_bxAccuState.SetBoxPos(WRect(0, 73, 160, 93));

// 공격형 펫 UI 개선 [11/28/2011 ldy1978220]
	m_rcHP.SetRect( 16, 33, 128, 40 );
	m_rcMP.SetRect( 16, 47, 128, 54 );
	m_rcEXP.SetRect(16, 61, 128, 68 );
	m_rcAccuExp.SetRect(4, 72, 156,84);

	m_btnPetInfo.Create( this, CTString( "" ), 138, 43, INFOMARK_SIZE, INFOMARK_SIZE );
	m_btnPetInfo.SetUV( UBS_IDLE, 195, 408, 211, 423, fTexWidth, fTexHeight );
	m_btnPetInfo.SetUV( UBS_CLICK, 214, 408, 230, 423, fTexWidth, fTexHeight );
	m_btnPetInfo.CopyUV( UBS_IDLE, UBS_ON );
	m_btnPetInfo.CopyUV( UBS_IDLE, UBS_DISABLE );

	m_btnReSize.Create( this, CTString( "" ), INFOMARK_POS_X+25, INFOMARK_POS_Y+3, INFOMARK_SIZE, INFOMARK_SIZE );
	m_btnReSize.SetUV( UBS_IDLE, 152, 423, 137, 408,  fTexWidth, fTexHeight );
	m_btnReSize.SetUV( UBS_CLICK, 170, 423, 155, 408,  fTexWidth, fTexHeight );
	m_btnReSize.CopyUV( UBS_IDLE, UBS_ON );
	m_btnReSize.CopyUV( UBS_IDLE, UBS_DISABLE );

	m_rcStmImage.SetRect(34, 83, 54, 103);
	m_rcFaithImage.SetRect(110, 83, 130, 103);
	m_bxTooltip.SetBoxUV(m_ptdButtonTexture,10,WRect(0,229,84,250));

	m_bxStatusback.SetBoxUV(m_ptdBaseTexture,14, WRect(525,301,648,332));
	m_bxStatusback.SetBoxPos(WRect(0,76,256,108));
}

void CUIWildPetTargetInfo::ReSize()
{
	m_bSizeEx = !m_bSizeEx;

	FLOAT	fTexWidth = m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight = m_ptdBaseTexture->GetPixHeight();

	if (m_bSizeEx)
	{
		SetSize(PETTARGETINFO_WIDTH_EX, PETTARGETINFO_HEIGHT_EX);
		m_bxTitle.SetBoxPos(WRect( 0, 0, 256, 77 ));
		m_rcTitle.SetRect( 0, 0, 256, 28 );

		m_btnPetInfo.SetPos(234,43);

		m_btnReSize.SetPos(INFOMARK_POS_X+25+96,INFOMARK_POS_Y+3);
		m_btnReSize.SetUV( UBS_IDLE, 137, 408, 152, 423, fTexWidth, fTexHeight );
		m_btnReSize.SetUV( UBS_CLICK, 155, 408, 170, 423, fTexWidth, fTexHeight );
		m_btnReSize.CopyUV( UBS_IDLE, UBS_ON );
		m_btnReSize.CopyUV( UBS_IDLE, UBS_DISABLE );

		m_rcHP.SetRect( 16, 33, 224, 40 );
		m_rcMP.SetRect( 16, 47, 224, 54 );
 		m_rcEXP.SetRect(16, 61, 224, 68 );
		m_rcAccuExp.SetRect(4, 108, 252,120);
		m_bxAccuState.SetBoxPos(WRect(0, 104, 256, 124));

	}else
	{
		SetSize( PETTARGETINFO_WIDTH+20, PETTARGETINFO_HEIGHT );

		m_bxTitle.SetBoxPos(WRect( 0, 0, 160, 77 ));
		m_rcTitle.SetRect( 0, 0, 130, 22 );

		m_btnPetInfo.SetPos(138, 43);

		m_btnReSize.SetPos(INFOMARK_POS_X+25, INFOMARK_POS_Y+3);
		m_btnReSize.SetUV( UBS_IDLE, 152, 423, 137, 408,  fTexWidth, fTexHeight );
		m_btnReSize.SetUV( UBS_CLICK, 170, 423, 155, 408,  fTexWidth, fTexHeight );
		m_btnReSize.CopyUV( UBS_IDLE, UBS_ON );
		m_btnReSize.CopyUV( UBS_IDLE, UBS_DISABLE );

		m_rcHP.SetRect( 16, 33, 128, 40 );
		m_rcMP.SetRect( 16, 47, 128, 54 );
		m_rcEXP.SetRect(16, 61, 128, 68 );
		m_rcAccuExp.SetRect(4, 72, 156,84);
		m_bxAccuState.SetBoxPos(WRect(0, 73, 160, 93));
	}
}

void CUIWildPetTargetInfo::Render()
{
	ObjInfo* pInfo = ObjInfo::getSingleton();

	if(pInfo->GetMyApetInfo() == NULL || pInfo->GetMyApetInfo()->bIsActive == FALSE)
		return;

	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	// Set target information texture
	pDrawPort->InitTextureData( m_ptdBaseTexture );

	CWildPetData	*m_Petdata;
	m_Petdata = CWildPetData::getData(pInfo->GetMyApetInfo()->m_nIndex);

	if (m_Petdata->nFlag & WILDPET_FLAG_EXP)
	{
		m_bxAccuState.Render(m_nPosX,m_nPosY);
		RederAccuExp();
	}

	m_bxTitle.Render(m_nPosX, m_nPosY);
	
	m_btnPetInfo.Render();
	
	UpdateHPInfo();
	UpdateMPInfo();
	UpdateEXPInfo();

	// HP
	pDrawPort->AddTexture( m_nPosX + m_rcHP.Left, m_nPosY + m_rcHP.Top,
		m_nPosX + m_rcHP.Right, m_nPosY + m_rcHP.Bottom,
		m_rtHP.U0, m_rtHP.V0, m_rtHP.U1, m_rtHP.V1,0xFFFFFFFF );


	// MP
	pDrawPort->AddTexture( m_nPosX + m_rcMP.Left, m_nPosY + m_rcMP.Top,
		m_nPosX + m_rcMP.Right, m_nPosY + m_rcMP.Bottom,
		m_rtMP.U0, m_rtMP.V0, m_rtMP.U1, m_rtMP.V1,0xFFFFFFFF );

	// EXP
	pDrawPort->AddTexture( m_nPosX + m_rcEXP.Left, m_nPosY + m_rcEXP.Top,
			m_nPosX + m_rcEXP.Right, m_nPosY + m_rcEXP.Bottom,
			m_rtEXP.U0, m_rtEXP.V0, m_rtEXP.U1, m_rtEXP.V1,0xFFFFFFFF );

	CTString tempString;

	tempString.PrintF("%d",pInfo->GetMyApetInfo()->m_nLevel);
	pDrawPort->PutTextEx(tempString, m_nPosX+15, m_nPosY+8);

	if (m_bSizeEx)
	{
		tempString = CUIQuestBook::MakeTitleString(pInfo->GetMyApetInfo()->m_strName.c_str(), 30);
	}else
	{
		tempString = CUIQuestBook::MakeTitleString(pInfo->GetMyApetInfo()->m_strName.c_str(), 14);
	}
	pDrawPort->PutTextExCX(tempString, m_nPosX+m_nWidth/2, m_nPosY+8);

	tempString.PrintF("%d/%d",pInfo->GetMyApetInfo()->m_nHP,pInfo->GetMyApetInfo()->m_nMaxHP);
	pDrawPort->PutTextExCX(tempString, m_nPosX+m_nWidth/2, m_nPosY+33);

	tempString.PrintF("%d/%d",pInfo->GetMyApetInfo()->m_nMP,pInfo->GetMyApetInfo()->m_nMaxMP);
	pDrawPort->PutTextExCX(tempString, m_nPosX+m_nWidth/2, m_nPosY+47);
	
	tempString.PrintF("%.2f%%",((FLOAT)pInfo->GetMyApetInfo()->m_exp/(FLOAT)pInfo->GetMyApetInfo()->m_next_exp)*100.0f);
	pDrawPort->PutTextExCX(tempString, m_nPosX+m_nWidth/2, m_nPosY+61);

//----------
	if(m_bSizeEx)
	{
		FLOAT	fTexWidth = m_ptdBaseTexture->GetPixWidth();
		FLOAT	fTexHeight = m_ptdBaseTexture->GetPixHeight();

		m_bxStatusback.Render(m_nPosX, m_nPosY);
		// 스테미너
		// [090824 sora 배고픔값 %에서 수치로 변경]
		FLOAT temResult = (FLOAT)pInfo->GetMyApetInfo()->m_nStm/(FLOAT)pInfo->GetMyApetInfo()->m_nMaxStm *100.0f;
		if(temResult > 60)
		{
			m_rtStmImage.SetUV(0,405,18,423,fTexWidth,fTexHeight);
		}else if(temResult > 20)
		{
			m_rtStmImage.SetUV(23,405,41,423,fTexWidth,fTexHeight);
		}else
		{
			m_rtStmImage.SetUV(46,405,64,423,fTexWidth,fTexHeight);
		}
		
		pDrawPort->AddTexture(m_nPosX+m_rcStmImage.Left,m_nPosY+m_rcStmImage.Top,m_nPosX+m_rcStmImage.Right,m_nPosY+m_rcStmImage.Bottom,
			m_rtStmImage.U0,m_rtStmImage.V0,m_rtStmImage.U1,m_rtStmImage.V1,0xFFFFFFFF);
		
		tempString.PrintF("%d/%d",pInfo->GetMyApetInfo()->m_nStm, pInfo->GetMyApetInfo()->m_nMaxStm);
		
		pDrawPort->PutTextExCX(tempString,m_nPosX+82,m_nPosY+86);
		
		
		// 충성도
		// [090824 sora 충성도값 %에서 수치로 변경]
		
		temResult = (FLOAT)pInfo->GetMyApetInfo()->m_nFaith/(FLOAT)pInfo->GetMyApetInfo()->m_nMaxFaith *100.0f;
		if(temResult > 60)
		{
			m_rtFaithImage.SetUV(69,405,87,423,fTexWidth,fTexHeight);
		}else if(temResult > 20)
		{
			m_rtFaithImage.SetUV(92,405,110,423,fTexWidth,fTexHeight);
		}else
		{
			m_rtFaithImage.SetUV(115,405,133,423,fTexWidth,fTexHeight);
		}

		pDrawPort->AddTexture(m_nPosX+m_rcFaithImage.Left,m_nPosY+m_rcFaithImage.Top,m_nPosX+m_rcFaithImage.Right,m_nPosY+m_rcFaithImage.Bottom,
			m_rtFaithImage.U0,m_rtFaithImage.V0,m_rtFaithImage.U1,m_rtFaithImage.V1,0xFFFFFFFF);

		tempString.PrintF("%d/%d",pInfo->GetMyApetInfo()->m_nFaith, pInfo->GetMyApetInfo()->m_nMaxFaith);
		
		pDrawPort->PutTextExCX(tempString,m_nPosX+158,m_nPosY+86);

		if (CUIManager::getSingleton()->GetWildPetInfo()->GetAIActive())
		{

			m_rvAIONOFF.SetUV(2, 375,26,399,fTexWidth,fTexHeight);
			
		}else
		{

			m_rvAIONOFF.SetUV(58, 375, 82, 399, fTexWidth, fTexHeight);
		}
		pDrawPort->AddTexture(m_nPosX+196, m_nPosY+83, m_nPosX+216, m_nPosY+103,
											m_rvAIONOFF.U0, m_rvAIONOFF.V0, m_rvAIONOFF.U1, m_rvAIONOFF.V1,0xFFFFFFFF);
	}
	//---------
	m_btnReSize.Render();

	// Render all elements
	pDrawPort->FlushRenderingQueue();
	pDrawPort->EndTextEx();

		// SHOW TOOLTIP
	if(m_bShowTooltip)
	{
		RenderTooltip();
	}
}

WMSG_RESULT CUIWildPetTargetInfo::MouseMessage(MSG *pMsg )
{
	ObjInfo* pInfo = ObjInfo::getSingleton();
	
	// If target is not exist
	if (pInfo->GetMyApetInfo() == NULL || pInfo->GetMyApetInfo()->bIsActive == FALSE)
		return WMSG_FAIL;

//	WMSG_RESULT	wmsgResult;

	// Title bar
	static BOOL bTitleClick = FALSE;

	// Mouse point
	static int	nOldX, nOldY;
	int	nX = LOWORD( pMsg->lParam );
	int	nY = HIWORD( pMsg->lParam );

	// Mouse message
	switch( pMsg->message )
	{
	case WM_MOUSEMOVE:
		{
			m_bShowTooltip = FALSE;
			m_strToolTip.Clear();

			if( IsInside( nX, nY ) )
			{
				CUIManager::getSingleton()->SetMouseCursorInsideUIs();
			}

			int	ndX = nX - nOldX;
			int	ndY = nY - nOldY;

			// Move target information
			if( bTitleClick && ( pMsg->wParam & MK_LBUTTON ) )
			{

				nOldX = nX;	nOldY = nY;

				Move( ndX, ndY );

				// If target is disapeared
				if(!pInfo->GetMyApetInfo()->bIsActive )
					bTitleClick = FALSE;

				return WMSG_SUCCESS;
			}

			if( m_btnPetInfo.MouseMessage( pMsg ) != WMSG_FAIL )
			{
				m_bShowTooltip = TRUE;
				// BUGFIX : ITS2899 esc 키를 이용하여 펫 정보창을 닫기가 불가능한 현상 수정 [7/12/2011 rumist]
				//CUIManager::getSingleton()->RearrangeOrder( UI_WILDPETTARGETINFO, TRUE );
				m_lpViewPos = pMsg->lParam;
				extern INDEX g_iEnterChat;
				m_strToolTip = _S(2173,"펫 정보창") ;
				if( g_iEnterChat )
					m_strToolTip+=CTString("(P,ALT+P)");
				else 
					m_strToolTip+=CTString("(ALT+P)");
				return WMSG_SUCCESS;
			}else 
			{
				m_btnReSize.MouseMessage(pMsg);

				if (m_bSizeEx)
				{
					if(IsInsideRect(nX, nY, m_rcFaithImage))
					{
						m_lpViewPos = pMsg->lParam;
						m_bShowTooltip = TRUE;
						m_strToolTip = _S(4209,"충성도");
						return WMSG_SUCCESS;

					}else if(IsInsideRect(nX, nY, m_rcStmImage))
					{
						m_lpViewPos = pMsg->lParam;
						m_bShowTooltip = TRUE;
						m_strToolTip = _S(4210,"배고품");
						return WMSG_SUCCESS;
					}
				}
				if (IsInsideRect(nX, nY, m_bxAccuState.m_rcBoxRT.GetUIRect()))
				{
					CWildPetData	*m_Petdata;
					m_Petdata = CWildPetData::getData(pInfo->GetMyApetInfo()->m_nIndex);

					if (!(m_Petdata->nFlag & WILDPET_FLAG_EXP))
					{
						return WMSG_SUCCESS;
					}

					m_lpViewPos = pMsg->lParam;
					m_bShowTooltip = TRUE;

					sPetItem_Info temPetitem;
					CUIManager::getSingleton()->GetWildPetInfo()->GetWildPetInfo(pInfo->GetMyApetInfo()->m_nIdxServer, temPetitem);

					if (temPetitem.pet_cooltime > 0)
					{
						m_strToolTip.PrintF(_S(5644, "쿨타임"));						
						
						INDEX temCooltime = temPetitem.pet_cooltime - (unsigned int)_pTimer->GetLerpedCurrentTick();
						
						int iSec = temCooltime % 60;
						temCooltime /= 60;
						
						int iMin = temCooltime % 60;
						int iHour = temCooltime /= 60;
						CTString temtime;
						
						m_strToolTip += ": ";
						
						if (iHour > 0)
						{
							temtime.PrintF(_S(2512,"%d시간"),iHour);
							m_strToolTip += temtime;
						}
						if (iMin > 0)
						{
							temtime.PrintF(_S(2513, "%d분"),iMin);
							m_strToolTip += temtime;
						}
						if (iSec > 0)
						{
							temtime.PrintF(_S(2514,"%d초"),iSec);
							m_strToolTip += temtime;
							
						}
						
					}else	
					{
						FLOAT temresult = (float)m_Petdata->accexpData[0].nMaxAccParam1 + (float)(MY_APET_INFO()->m_nLevel * m_Petdata->accexpData[0].nMaxAccParam2) / 100.0f;
						// 부동소수점 연산으로 인한 계산오차가 발생하므로 연산식 수정 [1/13/2012 ldy1978220]
						SQUAD lAccuresult = _pNetwork->MyCharacterInfo.needExp * (int)(temresult * 100) / 10000;						
						
						m_strToolTip.PrintF("%I64d / %I64d", temPetitem.pet_accexp, lAccuresult);

					}					
					return WMSG_SUCCESS;	
				}
			}
	
		}
		break;

	case WM_LBUTTONDOWN:
		{
			if( IsInside( nX, nY ) )
			{
				nOldX = nX;		nOldY = nY;

				// Title bar
				if( IsInsideRect( nX, nY, m_rcTitle ))
				{
					bTitleClick = TRUE;
				}
				else if( m_btnPetInfo.MouseMessage( pMsg ) != WMSG_FAIL )
				{
				}
				else
				{
					if( pInfo->GetMyApetInfo()->m_pEntity )
					{
						CEntity			*penPlEntity;
						CPlayerEntity	*penPlayerEntity;
						penPlEntity = CEntity::GetPlayerEntity( 0 );
						penPlayerEntity = static_cast<CPlayerEntity *>(penPlEntity);
						penPlayerEntity->SetTarget( pInfo->GetMyApetInfo()->m_pEntity );
					}
				}
				
				if (m_btnReSize.MouseMessage(pMsg) != WMSG_FAIL)
				{
				}

				CUIManager::getSingleton()->RearrangeOrder( UI_WILDPETTARGETINFO, TRUE );
				return WMSG_SUCCESS;
			}
		}
		break;

	case WM_LBUTTONUP:
		{
			CUIManager* pUIManager = CUIManager::getSingleton();

			// If holding button doesn't exist
			if (pUIManager->GetDragIcon() == NULL)
			{
				// Title bar
				bTitleClick = FALSE;
				
				// If Pet Target isn't focused
				if( !IsFocused() )
					return WMSG_FAIL;

				if( IsInside( nX, nY ) )
				{
					// Close button
					if( m_btnPetInfo.MouseMessage( pMsg ) == WMSG_COMMAND )
					{
						if( pUIManager->GetWildPetInfo()->IsVisible() )
						{
							pUIManager->GetWildPetInfo()->CloseWildPetInfo();								
						}
						else
						{
							pUIManager->GetWildPetInfo()->OpenWildPetInfo();
						}
						return WMSG_SUCCESS;
					}
					if (m_btnReSize.MouseMessage(pMsg) == WMSG_COMMAND)
					{
						ReSize();
					}
					return WMSG_SUCCESS;
								
				}
			}
		}
		break;

	case WM_LBUTTONDBLCLK:
		{
			if( IsInside( nX, nY ) )
				return WMSG_SUCCESS;
		}
		break;
	}

	return WMSG_FAIL;
}

void CUIWildPetTargetInfo::UpdateHPInfo()
{
	ObjInfo* pInfo = ObjInfo::getSingleton();
	FLOAT	fHPRatio = Clamp( (FLOAT)pInfo->GetMyApetInfo()->m_nHP / (FLOAT)pInfo->GetMyApetInfo()->m_nMaxHP, 0.0f, 1.0f );

	if(m_bSizeEx)
	{
		m_rcHP.Right = m_rcHP.Left + (PETTARGETINFO_BAR_WIDTH_EX) * fHPRatio;
	}else
	{
		m_rcHP.Right = m_rcHP.Left + (PETTARGETINFO_BAR_WIDTH-8) * fHPRatio;
	}
}

void CUIWildPetTargetInfo::UpdateMPInfo()
{
	ObjInfo* pInfo = ObjInfo::getSingleton();
	FLOAT fMPRatio = Clamp( (FLOAT)pInfo->GetMyApetInfo()->m_nMP / (FLOAT)pInfo->GetMyApetInfo()->m_nMaxMP, 0.0f, 1.0f);

	if(m_bSizeEx)
	{
		m_rcMP.Right = m_rcMP.Left + (PETTARGETINFO_BAR_WIDTH_EX) * fMPRatio;
	}else
	{
		m_rcMP.Right = m_rcMP.Left + (PETTARGETINFO_BAR_WIDTH-8) * fMPRatio;
	}
}

void CUIWildPetTargetInfo::UpdateEXPInfo()
{
	ObjInfo* pInfo = ObjInfo::getSingleton();
	FLOAT fExpRatio = Clamp( (FLOAT)pInfo->GetMyApetInfo()->m_exp / (FLOAT)pInfo->GetMyApetInfo()->m_next_exp, 0.0f, 1.0f);
	if(m_bSizeEx)
	{
		m_rcEXP.Right = m_rcEXP.Left + (PETTARGETINFO_BAR_WIDTH_EX) * fExpRatio;
	}else
	{
		m_rcEXP.Right = m_rcEXP.Left + (PETTARGETINFO_BAR_WIDTH-8) * fExpRatio;
	}
}

void CUIWildPetTargetInfo::UpdateAccuEXPInfo()
{
	CWildPetData	*m_Petdata;
	ObjInfo* pInfo = ObjInfo::getSingleton();
	m_Petdata = CWildPetData::getData(pInfo->GetMyApetInfo()->m_nIndex);
	
	FLOAT temresult = (float)m_Petdata->accexpData[0].nMaxAccParam1 + (float)(pInfo->GetMyApetInfo()->m_nLevel * m_Petdata->accexpData[0].nMaxAccParam2) / 100.0f;
		// 부동소수점 연산으로 인한 계산오차가 발생하므로 연산식 수정 [1/13/2012 ldy1978220]
	SQUAD lAccuresult = _pNetwork->MyCharacterInfo.needExp * (int)(temresult * 100) / 10000;

	sPetItem_Info temPetitem;
	CUIManager::getSingleton()->GetWildPetInfo()->GetWildPetInfo(pInfo->GetMyApetInfo()->m_nIdxServer, temPetitem);

	fAccuExp = (FLOAT)temPetitem.pet_accexp / (FLOAT)lAccuresult;
	if (fAccuExp > 100.0f)
	{
		fAccuExp = 0.0f;
	}
	FLOAT temfAccu	= Clamp(fAccuExp, 0.0f, 1.0f);

	if(m_bSizeEx)
	{
		m_rcAccuExp.Right = m_rcAccuExp.Left + (PETTARGETINFO_BAR_WIDTH_EX+42) * temfAccu;
	}else
	{
		m_rcAccuExp.Right = m_rcAccuExp.Left + (PETTARGETINFO_BAR_WIDTH+32) * temfAccu;
	}
}

// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
void CUIWildPetTargetInfo::RenderTooltip()
{
	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	pDrawPort->InitTextureData( m_ptdButtonTexture );

//	extern INDEX g_iEnterChat;
	int tv_strSize,tv_posX,tv_posY;
//	m_strToolTip = _S(2173,"펫 정보창") ;
//	if( g_iEnterChat )
//		m_strToolTip+=CTString("(P,ALT+P)");
//	else 
//		m_strToolTip+=CTString("(ALT+P)");
	
	//tv_posX = m_nPosX + m_btnPetInfo.GetPosX() -  m_btnPetInfo.GetWidth()/2;
	//tv_posY = m_nPosY + m_btnPetInfo.GetPosY();

	tv_strSize = m_strToolTip.Length() * (_pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing());

	tv_posX = LOWORD( m_lpViewPos ) - (tv_strSize/2);
	tv_posY = HIWORD( m_lpViewPos );

	m_bxTooltip.SetBoxPos(WRect(0,0,tv_strSize+10,20));

	m_bxTooltip.Render(tv_posX,tv_posY-20);
	// show text
	pDrawPort->PutTextEx( m_strToolTip, tv_posX +4,
										tv_posY -_pUIFontTexMgr->GetFontHeight()-4, 0xFFFFFFFF );
	// Render all elements
	pDrawPort->FlushRenderingQueue();
	// Flush all render text queue
	pDrawPort->EndTextEx();	
	
}

void CUIWildPetTargetInfo::RederAccuExp()
{
	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	UpdateAccuEXPInfo();

	pDrawPort->AddTexture( m_nPosX + m_rcAccuExp.Left, m_nPosY + m_rcAccuExp.Top,
		m_nPosX + m_rcAccuExp.Right, m_nPosY + m_rcAccuExp.Bottom,
		m_rtAccuExp.U0, m_rtAccuExp.V0, m_rtAccuExp.U1, m_rtAccuExp.V1,0xFFFFFFFF );

	CTString temString;
	
	temString.PrintF("%.2f%%",fAccuExp*100.0f);
	if(m_bSizeEx)
	{
		pDrawPort->PutTextExCX(temString, m_nPosX+m_nWidth/2, m_nPosY+107);
	}else
	{
		pDrawPort->PutTextExCX(temString, m_nPosX+m_nWidth/2, m_nPosY+76);
	}

}