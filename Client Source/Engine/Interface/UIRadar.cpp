#include "stdh.h"

// 헤더 정리. [12/2/2009 rumist]
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Interface/UIRadar.h>
#include <Engine/Interface/UISiegeWarfareDoc.h>
#include <Engine/GameDataManager/GameDataManager.h>
#include <Engine/Contents/Base/ExpressSystem.h>
#include <Engine/Contents/Base/PetStash.h>

#include <Engine/Contents/Base/UIPetStash.h>
#include <Engine/Contents/Login/UIServerSelect.h>
#include <Engine/Contents/Base/UIMapOption.h>
#include <Engine/Contents/Base/UIPartyNew.h>
#include <Engine/Contents/function/ItemCollectionUI.h>
#include <Engine/Interface/UIMap.h>
#include <Engine/Interface/UIGuild.h>
#include <Engine/Object/ActorMgr.h>
#include <Engine/Contents/function/gps.h>
#include <Engine/Contents/Base/Party.h>
#include <Engine/Contents/function/PremiumChar.h>
#include <Engine/Contents/function/PremiumCharUI.h>
#include <Engine/Info/MyInfo.h>

#define RADIAN	(ANGLE_180 / PI)

// Define position
#define	RADAR_CENTER_OFFSETX			71
#define	RADAR_CENTER_OFFSETY			115
#define	RADAR_COORD_TEXT_CX				70
#define	RADAR_COORD_TEXT_SY				35
// #else
// 	#define	RADAR_COORD_TEXT_CX				90
// 	#define	RADAR_COORD_TEXT_SY				4
// #endif
#define	RADAR_OPTION_BASE_WIDTH			35
#define	RADAR_OPTION_SY					2
#define	RADAR_OPTION_BTN_SX				-15
#define	RADAR_OPTION_BTN_SY				14
#define	RADAR_OPTION_ICON_SY			20
#define	RADAR_OPTION_OFFSETY			16


// Define size of radar
#define	RADAR_COMPASS_RADIUS			50.0f
#define	RADAR_VIEW_RADIUS				55.0f
#define	RADAR_VIEW_DISTANCE				70.0f
#define DEF_SHOW_DISTANCE				47.f


// Show ToolTip
#define STT_OPTION		0
#define STT_MAP			1
#define STT_SIGNAL		2
#define STT_EXPRESSREMOTE	3
#define STT_PETSTASH 4
#define STT_ITEMCOLLECTION	5
#define STT_PREMIUMCHAR 6

#define WORLD_TIME_FONT_SIZE  7
#define RADAR_OPTION_TOTAL_EX RADAR_OPTION_TOTAL
#define NOTICE_TICK 800

#ifdef PREMIUM_CHAR
	#define DEF_EMPTY_MENU_MAX	(1)
#else	//	PREMIUM_CHAR
	#define DEF_EMPTY_MENU_MAX	(2)
#endif	//	PREMIUM_CHAR

#define DEF_LOCALTIME_YGAP	(14)

enum __tagRoyalrumbleMsg
{
	ROYALRUMBLE_SCHEDULE,
	ROYALRUMBLE_REJECT,
	ROYALRUMBLE_REWARD,
	ROYALRUMBLE_POINT,
};

// ----------------------------------------------------------------------------
// Name : CUIRadar()
// Desc : Constructor
// ----------------------------------------------------------------------------
CUIRadar::CUIRadar()
	: m_nLocalTimeY(0)
{
	m_szCoord[0] = NULL;
	m_bShowOptionPopup = FALSE;
	m_bShowToolTip = FALSE;
	m_strToolTip = CTString( "" );
	m_strTimer = CTString( "" );
	m_bInsideMouse = FALSE;
	m_bSignalBtnOn = FALSE;
	m_year = m_month = m_hour = m_day= 0;
	m_ptdButtonTexture = NULL;
	m_ptdRoyalRumbleTexture = NULL;
	m_ptdMapTexture = NULL;	// [2012/10/11 : Sora] 월드맵 개편
	m_ptdMapObjTexture = NULL;	// [2012/10/11 : Sora] 월드맵 개편
	m_bEnableRR = FALSE;
	// trophy texture init. [5/31/2011 rumist]
	m_ptdTrophyTexture = NULL;
	m_bRenderTrophy = FALSE;
	m_nZone = -1;	// [2012/10/11 : Sora] 월드맵 개편
	m_nLayer = -1;	// [2012/10/11 : Sora] 월드맵 개편
	m_fZoomRate = 3.0f;	// [2012/10/11 : Sora] 월드맵 개편
	m_nSelectedIcon = -1; // [2012/10/11 : Sora] 월드맵 개편
}

// ----------------------------------------------------------------------------
// Name : ~CUIRadar()
// Desc : Destructorㄱㄱㄱ
// ----------------------------------------------------------------------------
CUIRadar::~CUIRadar()
{
	Destroy();

	_destroyTrophy();
	STOCK_RELEASE( m_ptdRoyalRumbleTexture );
	m_bEnableRR = FALSE;
	
	STOCK_RELEASE(m_ptdButtonTexture);
	STOCK_RELEASE( m_ptdMapTexture );		// [2012/10/11 : Sora] 월드맵 개편
	STOCK_RELEASE( m_ptdMapObjTexture );	// [2012/10/11 : Sora] 월드맵 개편

	SAFE_DELETE(m_pMapOption);
}

// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUIRadar::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	nWidth = RADAR_WIDTH;
	nHeight = 200;
	
	CUIWindow::Create(pParentWnd, nX, nY, nWidth, nHeight);

	// Region of each part
	m_rcTop.SetRect( 0, 0, 141, 57 );
	m_rcCompassIn.SetRect( -47, -47, 47, 47 );
	m_rcCompassOut.SetRect( -76, -76, 76, 76 );

	m_rcPoint[RADAR_PARTY].SetRect( -5, -5, 6, 6 );
	m_rcPoint[RADAR_GUILD].SetRect( -5, -5, 6, 6 );
	m_rcPoint[RADAR_NPC].SetRect( -11, -11, 11, 11 );
	m_rcPoint[RADER_QUESTSUCCESS].SetRect( -11, -11, 11, 11 );
	m_rcPoint[RADER_QUESTNPC].SetRect( -11, -11, 11, 11 );
	m_rcPoint[RADAR_MOB].SetRect( -2, -2, 2, 2 );
	m_rcMyPoint.SetRect( -6, -11, 7, 8 );
	m_rcTarget.SetRect( -2, -2, 3, 3 );
	m_rcSignal.SetRect( -12, -12, 12, 12 );
	m_rcOutSignal.SetRect( -10, -5, 11, 5 );

	// Create radar texture
	m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\TopUI.tex" ) );
	m_ptdButtonTexture =CreateTexture( CTString( "Data\\Interface\\CommonBtn.tex" ) );
	FLOAT	fTexWidth = m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight = m_ptdBaseTexture->GetPixHeight();

	m_rtBackTop.SetUV( 144, 963, 285, 1020, fTexWidth, fTexHeight );

	m_rtCompassIn.SetUV( 517, 376, 623, 481, fTexWidth, fTexHeight );
	m_rtCompassOut.SetUV( 337, 340, 495, 498, fTexWidth, fTexHeight );
	m_rtPoint[RADAR_PARTY].SetUV( 663, 408, 674, 419, fTexWidth, fTexHeight );
	m_rtPoint[RADAR_GUILD].SetUV( 648, 408, 659, 419, fTexWidth, fTexHeight );
	m_rtPoint[RADAR_NPC].SetUV( 665, 427, 674, 438, fTexWidth, fTexHeight );
	m_rtPoint[RADER_QUESTNPC].SetUV( 649, 448, 663, 463,fTexWidth, fTexHeight);
	m_rtPoint[RADER_QUESTSUCCESS].SetUV( 649, 467, 663, 481,fTexWidth, fTexHeight);
	m_rtPoint[RADAR_MOB].SetUV( 654, 395, 658, 399, fTexWidth, fTexHeight );
	m_rtMyPoint.SetUV( 687, 383, 698, 400, fTexWidth, fTexHeight );
	m_rtTarget.SetUV( 661, 395, 666, 400, fTexWidth, fTexHeight );
	m_rtSignal.SetUV( 682, 447, 706, 471, fTexWidth, fTexHeight );
	m_rtOutSignal.SetUV( 683, 427, 704, 437, fTexWidth, fTexHeight );
	m_rtTargetAni.SetUV( 684, 481, 703, 500, fTexWidth, fTexHeight );

	m_rtToolTipL.SetUV( 239, 253, 246, 270, fTexWidth, fTexHeight );
	m_rtToolTipM.SetUV( 246, 253, 328, 270, fTexWidth, fTexHeight );
	m_rtToolTipR.SetUV( 328, 253, 335, 270, fTexWidth, fTexHeight );

	// Option popup
	m_rtOptionL.SetUV( 239, 253, 246, 270, fTexWidth, fTexHeight );
	m_rtOptionM.SetUV( 246, 253, 328, 270, fTexWidth, fTexHeight );
	m_rtOptionR.SetUV( 328, 253, 335, 270, fTexWidth, fTexHeight );

	int nPosX = 646;
	for(int j=0; j<EXPEDITION_GROUP_MAX; j++)
	{
		m_rtPointExpedition[j].SetUV( nPosX, 347, nPosX + 11, 358, fTexWidth, fTexHeight );
		nPosX += 15;
	}
	
	m_rcPoint[RADAR_EXPEDITION].SetRect( -5, -5, 6, 6 );

	// Option button
	m_btnOption.Create( this, CTString( "" ), 3, 129, 25, 25 );
	m_btnOption.SetUV( UBS_IDLE, 791, 711, 816, 736, fTexWidth, fTexHeight );
	m_btnOption.SetUV( UBS_CLICK, 819, 711, 844, 736, fTexWidth, fTexHeight );
	m_btnOption.SetUV( UBS_ON, 847, 711, 872, 736, fTexWidth, fTexHeight );
	m_btnOption.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Map button
	m_btnMap.Create( this, CTString( "" ), -2, 100, 25, 25 );
	m_btnMap.SetUV( UBS_IDLE, 791, 683, 816, 708, fTexWidth, fTexHeight );
	m_btnMap.SetUV( UBS_CLICK, 819, 683, 844, 708, fTexWidth, fTexHeight );
	m_btnMap.SetUV( UBS_ON, 847, 683, 872, 708, fTexWidth, fTexHeight );
	m_btnMap.CopyUV( UBS_IDLE, UBS_DISABLE );

	m_btnSignal.Create( this, CTString( "" ), 24, 152, 25, 25 );
	m_btnSignal.SetUV( UBS_IDLE, 791, 739, 816, 764, fTexWidth, fTexHeight );
	m_btnSignal.SetUV( UBS_CLICK, 819, 739, 844, 764, fTexWidth, fTexHeight );
	m_btnSignal.SetUV( UBS_ON, 847, 739, 872, 764, fTexWidth, fTexHeight );
	m_btnSignal.CopyUV( UBS_IDLE, UBS_DISABLE );

	// world time wooss --------------------------------------->> 060415
	int tv_size = 655;
	for(int i=0; i<10; i++)
	{
		m_rtFigure[i].SetUV(tv_size,367,tv_size+WORLD_TIME_FONT_SIZE,377,fTexWidth,fTexHeight);
		tv_size += WORLD_TIME_FONT_SIZE+1;
	}

	// --------------------------------------------------------<<
	
	m_ExpressIcon.Create(this, CTString(""), 7, 6, 25, 25);
	m_ExpressIcon.SetUV( UBS_IDLE, 650, 679, 675, 704, fTexWidth, fTexHeight );
	m_ExpressIcon.SetUV( UBS_CLICK, 678, 679, 703, 704, fTexWidth, fTexHeight );
	m_ExpressIcon.SetUV( UBS_ON, 678, 707, 703, 732, fTexWidth, fTexHeight );
	m_ExpressIcon.CopyUV( UBS_IDLE, UBS_DISABLE );
	m_ExpressIcon.SetEnable(TRUE);

	m_ExpressNotice.SetUV(678, 707, 703, 732, fTexWidth, fTexHeight );

	m_bNotice = false;
	m_bRemote = false;
	m_bNoticeRenderFlag = false;
	m_nStartTime = 0;

	m_PetStashIcon.Create(this, CTString(""), 32, 6, 25, 25);
	m_PetStashIcon.SetUV( UBS_IDLE, 734, 683, 759, 708, fTexWidth, fTexHeight );
	m_PetStashIcon.SetUV( UBS_ON, 762, 711, 787, 736, fTexWidth, fTexHeight );
	m_PetStashIcon.SetUV( UBS_CLICK, 762, 683, 787, 708, fTexWidth, fTexHeight );
	m_PetStashIcon.CopyUV(UBS_IDLE, UBS_DISABLE);
	m_PetStashIcon.SetEnable(TRUE);

	m_ItemCollectionIcon.Create(this, CTString(""), 57, 6, 25, 25);
	m_ItemCollectionIcon.SetUV( UBS_IDLE, 650, 737, 650 + 25, 737 + 25, fTexWidth, fTexHeight );
	m_ItemCollectionIcon.SetUV( UBS_ON, 678, 765, 678 + 25, 765 + 25, fTexWidth, fTexHeight );
	m_ItemCollectionIcon.SetUV( UBS_CLICK, 678, 737, 678 + 25, 737 + 25, fTexWidth, fTexHeight );
	m_ItemCollectionIcon.SetUV( UBS_DISABLE, 650, 765, 650 + 25, 765 + 25, fTexWidth, fTexHeight );
	m_ItemCollectionIcon.SetEnable(TRUE);

	int nEmptyPosX, nEmptyPosY;
#ifdef PREMIUM_CHAR
	m_PremiumCharIcon.Create(this, CTString(""), 82, 6, 25, 25);
	m_PremiumCharIcon.SetUV( UBS_IDLE, 734, 739, 734 + 25, 739 + 25, fTexWidth, fTexHeight );
	m_PremiumCharIcon.SetUV( UBS_ON, 762, 767, 762 + 25, 767 + 25, fTexWidth, fTexHeight );
	m_PremiumCharIcon.SetUV( UBS_CLICK, 762, 739, 762 + 25, 739 + 25, fTexWidth, fTexHeight );
	m_PremiumCharIcon.SetUV( UBS_DISABLE, 734, 767, 734 + 25, 767 + 25, fTexWidth, fTexHeight );
	m_PremiumCharIcon.SetEnable(TRUE);

	m_PremiumCharIcon.SetBtnState(UBS_DISABLE);

	nEmptyPosX = m_PremiumCharIcon.GetPosX() + m_PremiumCharIcon.GetWidth();
	nEmptyPosY = m_PremiumCharIcon.GetPosY();
#else	//	PREMIUM_CHAR
	nEmptyPosX = m_ItemCollectionIcon.GetPosX() + m_ItemCollectionIcon.GetWidth();
	nEmptyPosY = m_ItemCollectionIcon.GetPosY();
#endif	//	PREMIUM_CHAR

	m_uvEmptyMenu.SetUV(650, 707, 675, 732, fTexWidth, fTexHeight);
	m_rcEmptyMenu.SetRect(nEmptyPosX, nEmptyPosY, 25, 25);

	// Option check buttons
	fTexWidth = m_ptdButtonTexture->GetPixWidth();
	fTexHeight = m_ptdButtonTexture->GetPixHeight();

	// trophy data 가 없을 경우 crash 문제 해결을 위해서 [5/25/2011 rumist]
	_royalrumbleCreate();
	fTexWidth = m_ptdBaseTexture->GetPixWidth();
	fTexHeight = m_ptdBaseTexture->GetPixHeight();

	m_btnZoomIn.Create( this, CTString( "" ), 92, 54, 25, 25 );
	m_btnZoomIn.SetUV( UBS_IDLE, 548, 484, 573, 509, fTexWidth, fTexHeight );
	m_btnZoomIn.SetUV( UBS_CLICK, 490, 484, 515, 509, fTexWidth, fTexHeight );
	m_btnZoomIn.SetUV( UBS_DISABLE, 606, 484, 631, 509, fTexWidth, fTexHeight );
	m_btnZoomIn.CopyUV( UBS_IDLE, UBS_ON );

	m_btnZoomOut.Create( this, CTString( "" ), 113, 78, 25, 25 );
	m_btnZoomOut.SetUV( UBS_IDLE, 577, 484, 602, 509, fTexWidth, fTexHeight );
	m_btnZoomOut.SetUV( UBS_CLICK, 519, 484, 544, 509, fTexWidth, fTexHeight );
	m_btnZoomOut.SetUV( UBS_DISABLE, 635, 484, 660, 509, fTexWidth, fTexHeight );
	m_btnZoomOut.CopyUV( UBS_IDLE, UBS_ON );

	m_iconVec.reserve(50);

	m_ptdMapObjTexture = CreateTexture( CTString( "Data\\Interface\\Map.tex" ) );
	fTexWidth = m_ptdMapObjTexture->GetPixWidth();
	fTexHeight = m_ptdMapObjTexture->GetPixHeight();

	m_rcSubZone[VILLAGE].SetRect( -17, -18, 17, 18 );
	m_rcSubZone[DUNGEON].SetRect( -16, -18, 16, 18 );
	m_rcSubZone[CHARGE_PRIVATE].SetRect( -13, -23, 13, 23 );
	m_rcSubZone[CHARGE_PUBLIC].SetRect( -13, -23, 13, 23 );
	m_rcSubZone[MINE_PRIVATE].SetRect( -17, -20, 17, 20 );
	m_rcSubZone[MINE_PUBLIC].SetRect( -17, -20, 17, 20 );
	m_rcSubZone[GATHER_PRIVATE].SetRect( -17, -19, 17, 19 );
	m_rcSubZone[GATHER_PUBLIC].SetRect( -17, -19, 17, 19 );
	m_rcSubZone[CASTLE_GATE].SetRect( -9, -9, 9, 9 );//!!

	m_rtSubZone[VILLAGE].SetUV( 217, 205, 251, 241, fTexWidth, fTexHeight );
	m_rtSubZone[DUNGEON].SetUV( 67, 91, 99, 127, fTexWidth, fTexHeight );
	m_rtSubZone[CHARGE_PRIVATE].SetUV( 108, 82, 134, 128, fTexWidth, fTexHeight );
	m_rtSubZone[CHARGE_PUBLIC].SetUV( 108, 82, 134, 128, fTexWidth, fTexHeight );
	m_rtSubZone[MINE_PRIVATE].SetUV( 135, 88, 169, 128, fTexWidth, fTexHeight );
	m_rtSubZone[MINE_PUBLIC].SetUV( 135, 88, 169, 128, fTexWidth, fTexHeight );
	m_rtSubZone[GATHER_PRIVATE].SetUV( 131, 6, 165, 44, fTexWidth, fTexHeight );
	m_rtSubZone[GATHER_PUBLIC].SetUV( 131, 6, 165, 44, fTexWidth, fTexHeight );
	m_rtSubZone[CASTLE_GATE].SetUV( 170, 10, 188, 28, fTexWidth, fTexHeight );

	m_rcGPS.SetRect(-13, -13, 13, 13);
	m_uvGPS.SetUV(210, 258, 236, 284, fTexWidth, fTexHeight);

	m_pMapOption = new CUIMapOption;
	UIMGR()->LoadXML("UIMap_Option.xml", m_pMapOption);

	m_pMapOption->SetPos(m_nPosX - m_pMapOption->GetWidth(),(m_nPosY + m_rcOption.Top));
}

// ----------------------------------------------------------------------------
// Name : ResetPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIRadar::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( pixMaxI - GetWidth(), pixMinJ );
	_pUIBuff->SetMyBadBuffPos( m_nPosX - 2, 2 );

	PIX pixCenterI = (pixMaxI - pixMinI)/2;
	PIX pixCenterJ = (pixMaxJ - pixMinJ)/2;
	UIRect rcTrophy;
	rcTrophy.SetRect( 0, 0, 332, 312 );
	m_rcTrophy.SetRect( pixCenterI - rcTrophy.GetCenterX(), pixCenterJ - rcTrophy.GetCenterY()-200, 
						pixCenterI + rcTrophy.GetCenterX(), pixCenterJ + rcTrophy.GetCenterY()-200 );

	m_pMapOption->SetPos(m_nPosX - m_pMapOption->GetWidth(),(m_nPosY + m_rcOption.Top));
}

// ----------------------------------------------------------------------------
// Name : AdjustPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIRadar::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
static FLOAT	fSin, fCos;
void CUIRadar::Render()
{
	int i;
	int	nX = m_nPosX + RADAR_CENTER_OFFSETX;
	int	nY = m_nPosY + RADAR_CENTER_OFFSETY;
	fSin = Sin( _pNetwork->MyCharacterInfo.camera_angle );
	fCos = Cos( _pNetwork->MyCharacterInfo.camera_angle );

	CUIManager* pUIManager = CUIManager::getSingleton();
	CDrawPort* pDrawPort = pUIManager->GetDrawPort();

	// Set radar texture
	pDrawPort->InitTextureData( m_ptdBaseTexture );

	// Add render regions
	// Background
	// Top
	pDrawPort->AddTexture( m_nPosX, m_nPosY,
										m_nPosX + m_rcTop.Right, m_nPosY + m_rcTop.Bottom,
										m_rtBackTop.U0, m_rtBackTop.V0, m_rtBackTop.U1, m_rtBackTop.V1,
										0xFFFFFFFF );

	if( IsRadarUse() )
	{
		pDrawPort->FlushRenderingQueue();
		
		RenderCurrentMap();
	
		// Render location of objects
		RenderObjectLocation();

		RenderGPS(pDrawPort);

		pDrawPort->InitTextureData( m_ptdBaseTexture );

		pDrawPort->AddTexture( nX + m_rcCompassOut.Left, nY + m_rcCompassOut.Top, nX + m_rcCompassOut.Right, nY + m_rcCompassOut.Bottom,
								   m_rtCompassOut.U0, m_rtCompassOut.V0, m_rtCompassOut.U1, m_rtCompassOut.V1, 0xFFFFFFFF);
		// Render Signal 
		RenderSignal();
		RenderMyPosition();
		
		// Option button
		m_btnOption.Render();
	
		// Map button
		m_btnMap.Render();
	
		// Signal Button 
		m_btnSignal.Render();

		m_btnZoomIn.Render();
		m_btnZoomOut.Render();

		pDrawPort->FlushRenderingQueue();

		m_nLocalTimeY = m_nPosY + this->GetHeight() - DEF_LOCALTIME_YGAP, DEF_UI_COLOR_WHITE;
	}
	else
	{
		m_nLocalTimeY = m_rcTop.Top + m_rcTop.Bottom + 4, DEF_UI_COLOR_WHITE;
	}
	// Option popup
	if( m_bShowOptionPopup )
	{
		if (m_pMapOption != NULL)
		{
			m_pMapOption->Hide(FALSE);
			m_pMapOption->Render(pDrawPort);
		}
	}
	else
	{
		if (m_pMapOption != NULL)
			m_pMapOption->Hide(TRUE);
	}

	// Text in radar
	sprintf( m_szCoord, "%4d,%-4d", int(_pNetwork->MyCharacterInfo.x), int(_pNetwork->MyCharacterInfo.z) );
	pDrawPort->PutTextCharExCX( m_szCoord, 9, m_nPosX + RADAR_COORD_TEXT_CX,
												m_nPosY + RADAR_COORD_TEXT_SY, 0x72D02EFF );

	time_t tmt;
	tmt = (ULONG)time(NULL) - _pNetwork->slServerTimeGap;

	struct tm* pTime = localtime(&tmt);

	if (pTime != NULL)
	{
		m_strLocalDay.PrintF("%d.%02d.%02d", pTime->tm_year + 1900, pTime->tm_mon + 1, pTime->tm_mday);
		m_strLocalTime.PrintF(" %02d:%02d", pTime->tm_hour, pTime->tm_min);
	
		pDrawPort->PutTextExCX(m_strLocalDay + m_strLocalTime, m_nPosX +this->GetWidth()/2, m_nLocalTimeY);
	}

	CTString strServerInfo;
	strServerInfo.PrintF("%s Ch. %d", pUIManager->GetServerSelect()->GetServerName(_pNetwork->m_iServerGroup), _pNetwork->m_iServerCh );
	pDrawPort->PutTextExCX( strServerInfo, m_nPosX +this->GetWidth()/2, m_nLocalTimeY + 17, 0xF2F2F2B2 );
	
	// Render all elements
	pDrawPort->FlushRenderingQueue();

	// Flush all render text queue
	pDrawPort->EndTextEx();

	pDrawPort->InitTextureData( m_ptdBaseTexture );

	m_ExpressIcon.Render();
	if ( m_bNotice )
	{
		DWORD nCurrent = timeGetTime();
		if ( nCurrent > (m_nStartTime + NOTICE_TICK) )
		{				
			m_nStartTime = nCurrent;
			m_bNoticeRenderFlag = !m_bNoticeRenderFlag;
		}
	}		

	if ( m_bNoticeRenderFlag )
	{
		pDrawPort->AddTexture(m_nPosX + m_ExpressIcon.GetPosX(), m_nPosY + m_ExpressIcon.GetPosY(),
			m_nPosX + m_ExpressIcon.GetPosX() + 25, m_nPosY + m_ExpressIcon.GetPosY() + 25,
			m_ExpressNotice.U0, m_ExpressNotice.V0,
			m_ExpressNotice.U1, m_ExpressNotice.V1, 0xFFFFFFFF);
	}

	m_PetStashIcon.Render();
	m_ItemCollectionIcon.Render();

#ifdef PREMIUM_CHAR
	m_PremiumCharIcon.Render();
#endif	//	PREMIUM_CHAR

	int nEmptyPosX;
	for( i = 0; i < DEF_EMPTY_MENU_MAX; ++i )
	{
		nEmptyPosX = m_nPosX + m_rcEmptyMenu.Left + ( i * m_rcEmptyMenu.Right );

		pDrawPort->AddTexture(nEmptyPosX, m_nPosY + m_rcEmptyMenu.Top,
			nEmptyPosX + m_rcEmptyMenu.Right, m_nPosY + m_rcEmptyMenu.Top + m_rcEmptyMenu.Bottom,
			m_uvEmptyMenu.U0, m_uvEmptyMenu.V0,	m_uvEmptyMenu.U1, m_uvEmptyMenu.V1, 0xFFFFFFFF);
	}

	pDrawPort->FlushRenderingQueue();

	// Tool tip
	if( m_bShowToolTip )
	{
		// Set texture
		pDrawPort->InitTextureData( m_ptdBaseTexture );

		pDrawPort->AddTexture( m_rcToolTip.Left, m_rcToolTip.Top,
											m_rcToolTip.Left + 7, m_rcToolTip.Bottom,
											m_rtToolTipL.U0, m_rtToolTipL.V0, m_rtToolTipL.U1, m_rtToolTipL.V1,
											0xFFFFFFFF );
		pDrawPort->AddTexture( m_rcToolTip.Left + 7, m_rcToolTip.Top,
											m_rcToolTip.Right - 7, m_rcToolTip.Bottom,
											m_rtToolTipM.U0, m_rtToolTipM.V0, m_rtToolTipM.U1, m_rtToolTipM.V1,
											0xFFFFFFFF );
		pDrawPort->AddTexture( m_rcToolTip.Right - 7, m_rcToolTip.Top,
											m_rcToolTip.Right, m_rcToolTip.Bottom,
											m_rtToolTipR.U0, m_rtToolTipR.V0, m_rtToolTipR.U1, m_rtToolTipR.V1,
											0xFFFFFFFF );

		// Render all elements
		pDrawPort->FlushRenderingQueue();

		// Text in tool tip
		pDrawPort->PutTextEx( m_strToolTip, m_rcToolTip.Left + 8, m_rcToolTip.Top + 4 , m_colTooltip);

		if (m_bRemote)
		{
			pDrawPort->PutTextEx( m_strTimer, m_rcToolTip.Left + 8, m_rcToolTip.Top + 22, m_colTooltipTimer);
		}
		// Flush all render text queue
		pDrawPort->EndTextEx();
	}

	if( (m_nSelectedIcon > 0 && m_nSelectedIcon < m_iconVec.size()) && IsRadarUse() )
	{
		pDrawPort->InitTextureData( m_ptdBaseTexture );

		pDrawPort->AddTexture( m_nPosX + m_iconVec[m_nSelectedIcon].rc.Left - ((_pUIFontTexMgr->GetFontWidth() * m_iconVec[m_nSelectedIcon].name.Length())/2) - 6,
							   m_nPosY + m_iconVec[m_nSelectedIcon].rc.Top - 14,
							   m_nPosX + m_iconVec[m_nSelectedIcon].rc.Left - ((_pUIFontTexMgr->GetFontWidth() * m_iconVec[m_nSelectedIcon].name.Length())/2) + 1,
							   m_nPosY + m_iconVec[m_nSelectedIcon].rc.Top,
							   m_rtToolTipL.U0,
							   m_rtToolTipL.V0,
							   m_rtToolTipL.U1,
							   m_rtToolTipL.V1,
							   0xFFFFFFFF );
		pDrawPort->AddTexture( m_nPosX + m_iconVec[m_nSelectedIcon].rc.Left - ((_pUIFontTexMgr->GetFontWidth() * m_iconVec[m_nSelectedIcon].name.Length())/2) + 1,
							   m_nPosY + m_iconVec[m_nSelectedIcon].rc.Top - 14,
							   m_nPosX + m_iconVec[m_nSelectedIcon].rc.Left + ((_pUIFontTexMgr->GetFontWidth() * m_iconVec[m_nSelectedIcon].name.Length())/2) - 1,
							   m_nPosY + m_iconVec[m_nSelectedIcon].rc.Top,
							   m_rtToolTipM.U0,
							   m_rtToolTipM.V0,
							   m_rtToolTipM.U1,
							   m_rtToolTipM.V1,
							   0xFFFFFFFF );
		pDrawPort->AddTexture( m_nPosX + m_iconVec[m_nSelectedIcon].rc.Left + ((_pUIFontTexMgr->GetFontWidth() * m_iconVec[m_nSelectedIcon].name.Length())/2) - 1,
							   m_nPosY + m_iconVec[m_nSelectedIcon].rc.Top - 14,
							   m_nPosX + m_iconVec[m_nSelectedIcon].rc.Left + ((_pUIFontTexMgr->GetFontWidth() * m_iconVec[m_nSelectedIcon].name.Length())/2) + 6,
							   m_nPosY + m_iconVec[m_nSelectedIcon].rc.Top,
							   m_rtToolTipR.U0,
							   m_rtToolTipR.V0,
							   m_rtToolTipR.U1,
							   m_rtToolTipR.V1,
							   0xFFFFFFFF );

		pDrawPort->FlushRenderingQueue();

		pDrawPort->PutTextEx( m_iconVec[m_nSelectedIcon].name,
							  m_nPosX + m_iconVec[m_nSelectedIcon].rc.Left - ((_pUIFontTexMgr->GetFontWidth() * m_iconVec[m_nSelectedIcon].name.Length())/2),
							  m_nPosY + m_iconVec[m_nSelectedIcon].rc.Top - 14 );
		pDrawPort->EndTextEx();
	}

//	m_bEnableRR = TRUE;
	if( m_bEnableRR )
	{
		_royalrumbleButtonRender();
	}
	_royalrumbleTimeRender();
	_renderTrophy();
}

// ----------------------------------------------------------------------------
// Name : RenderObjectLocation()
// Desc :
// ----------------------------------------------------------------------------
void CUIRadar::RenderObjectLocation()
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	CDrawPort* pDrawPort = pUIManager->GetDrawPort();

	m_iconVec.clear();

	//CEntity			*penObject;
	FLOAT3D			vObjectPos;
	FLOAT			fX, fZ;
	int				nX, nY;
	SBYTE			sbLayerDiff;

	MapData* md;
	CMobData* pMobData = NULL;
	CTString strTemp;
	md = pUIManager->GetMap()->GetCurMapData( m_nZone );
	FLOAT	fSqrDist = RADAR_VIEW_DISTANCE * RADAR_VIEW_DISTANCE / m_fZoomRate;
	FLOAT	fRatio = md->World.fRatio * m_fZoomRate;
	FLOAT			fXpc = _pNetwork->MyCharacterInfo.x;
	FLOAT			fZpc = _pNetwork->MyCharacterInfo.z;
	int				nCX = m_nPosX + RADAR_CENTER_OFFSETX;
	int				nCY = m_nPosY + RADAR_CENTER_OFFSETY;
	SBYTE			sbLayerpc = _pNetwork->MyCharacterInfo.yLayer;

	pDrawPort->InitTextureData( m_ptdMapObjTexture );

	int		i;
	for (i = 0; i < md->vecSubZone.size(); i++)
	{
		SBYTE	sbSubType = md->vecSubZone[i].sbType;
		// revision map size. [10/20/2009 rumist]

		if( sbSubType == VILLAGE )
			continue;

		fX = md->vecSubZone[i].fX - fXpc;
		fZ = md->vecSubZone[i].fZ - fZpc;

		fX *= fRatio;
		fZ *= fRatio;

		if( fX > DEF_SHOW_DISTANCE || fX < -DEF_SHOW_DISTANCE || fZ > DEF_SHOW_DISTANCE || fZ < -DEF_SHOW_DISTANCE )
			continue;

		nX = nCX + fX;
		nY = nCY + fZ;

		if( sbSubType == CHARGE_PRIVATE || sbSubType == MINE_PRIVATE || sbSubType == GATHER_PRIVATE )
		{
			strTemp.PrintF( _S( 549, "%s(사유지)" ), CZoneInfo::getSingleton()->GetExtraName( md->World.nZoneIndex, md->vecSubZone[i].nSubZoneIndex ) );
		}
		else
		{
			strTemp = CZoneInfo::getSingleton()->GetExtraName( md->World.nZoneIndex, md->vecSubZone[i].nSubZoneIndex );
		}

		AddIconName( nX + m_rcPoint[RADAR_MOB].Left,  nY + m_rcPoint[RADAR_MOB].Top,
					 nX + m_rcPoint[RADAR_MOB].Right, nY + m_rcPoint[RADAR_MOB].Bottom, strTemp);



		pDrawPort->AddTexture( nX + m_rcSubZone[sbSubType].Left, nY + m_rcSubZone[sbSubType].Top,
											nX + m_rcSubZone[sbSubType].Right, nY + m_rcSubZone[sbSubType].Bottom,
											m_rtSubZone[sbSubType].U0, m_rtSubZone[sbSubType].V0,
											m_rtSubZone[sbSubType].U1, m_rtSubZone[sbSubType].V1,
											0xFFFFFFFF );

	}

	if (m_pMapOption == NULL)
		return;

	// Npc
	for (i = 0; i < md->vecNpc.size(); i++ )
	{
		sbLayerDiff = md->vecNpc[i].nYLayer - sbLayerpc;
		if( sbLayerDiff < -1 || sbLayerDiff > 1 )
			continue;

		if (IS_EVENT_ON(A_EVENT_HOLLOWEEN) == 0)
		{
			if (md->vecNpc[i].nIndex == 454 ||
				md->vecNpc[i].nIndex == 455)
				continue;
		}
		
		pMobData = CMobData::getData(md->vecNpc[i].nIndex);

		if (pMobData != NULL)
		{
			if (pMobData->IsChannelFlag(_pNetwork->m_iServerCh) == false)
				continue;
		}

		fX = (md->vecNpc[i].fX);
		fZ = (md->vecNpc[i].fZ);

		fX -= fXpc;
		fZ -= fZpc;

		fX *= fRatio;
		fZ *= fRatio;

		int left = fX + m_rcPoint[RADAR_NPC].Left;
		int top = fZ + m_rcPoint[RADAR_NPC].Top;
		int right = fX + m_rcPoint[RADAR_NPC].Right;
		int bottom = fZ + m_rcPoint[RADAR_NPC].Bottom;

		if( left < m_rcCompassIn.Left || top < m_rcCompassIn.Top ||
			right > m_rcCompassIn.Right || bottom > m_rcCompassIn.Bottom )
			continue;

		nX = nCX + fX;
		nY = nCY + fZ;

		int nShowIcon = -1;

		if (m_pMapOption->IsCheck(eNT_QUEST_Q) == true && 
			CQuestSystem::Instance().TestNPCForQuest(md->vecNpc[i].nIndex) == CQuestSystem::NQT_HAVE_QUEST)
		{
			nShowIcon = eNT_QUEST_Q;
		}
		else if (m_pMapOption->IsCheck(eNT_QUEST_A) == true &&
				 CQuestSystem::Instance().TestNPCForQuest(md->vecNpc[i].nIndex) == CQuestSystem::NQT_CAN_PRIZE)
		{
			nShowIcon = eNT_QUEST_A;
		}
		else
		{
			for (int j = eNT_SHOP; j < eNT_MONSTER; ++j)
			{
				if (m_pMapOption->IsCheck((eNPC_TYPE)j) == false)
					continue;

				if (m_pMapOption->IsFlag(j, md->vecNpc[i].nIndex) != FALSE)
				{
					nShowIcon = j;
					break;
				}
			}
		}

		if (nShowIcon >= 0)
		{
			UIRectUV uv = m_pMapOption->GetUV((eNPC_TYPE)nShowIcon);
	
			AddIconName( nX + m_rcPoint[RADAR_NPC].Left,  nY + m_rcPoint[RADAR_NPC].Top,
				nX + m_rcPoint[RADAR_NPC].Right, nY +m_rcPoint[RADAR_NPC].Bottom,
				CMobData::getData(md->vecNpc[i].nIndex)->GetName());
	
			pDrawPort->AddTexture( nX + m_rcPoint[RADAR_NPC].Left,  nY + m_rcPoint[RADAR_NPC].Top,
				nX + m_rcPoint[RADAR_NPC].Right, nY + m_rcPoint[RADAR_NPC].Bottom,
				uv.U0, uv.V0, uv.U1, uv.V1, 0xFFFFFFFF );
		}
	}
	
	//pDrawPort->InitTextureData( m_ptdBaseTexture );

	// Mob
	if( m_pMapOption->IsCheck(eNT_MONSTER) == true)
	{
		UIRectUV uv = m_pMapOption->GetUV(eNT_MONSTER);
		ACTORMGR()->DrawMobRadar(nCX, nCY, fRatio, pDrawPort, m_rcPoint[RADAR_MOB], uv);
	}

	// Guild
	if( m_pMapOption->IsCheck(eNT_GUILD_GROUP) == true )
	{
		UIRectUV uv = m_pMapOption->GetUV(eNT_GUILD_GROUP);
		ACTORMGR()->DrawCharRadar(nCX, nCY, fRatio, pDrawPort, m_rcPoint[RADAR_GUILD], uv, true);
	}

	Party* pParty = GAMEDATAMGR()->GetPartyInfo();

	// Party
	if( m_pMapOption->IsCheck(eNT_PARTY) == true )
	{
		if (pParty == NULL)
			return;

		UIRectUV uv = m_pMapOption->GetUV(eNT_PARTY);
		
		for( int iParty = 0; iParty < pParty->GetMemberCount(); iParty++ )
		{
			if(pParty->GetLevel( iParty ) == 0)	// 레벨이 0인 멤버는 오프라인 상태이므로 그리지 않는다.
				continue;

			if (pParty->GetMemberZone(iParty) != _pNetwork->MyCharacterInfo.zoneNo) // 파티원이 다른존에 있을 경우
				continue;

			sbLayerDiff = pParty->GetLayer( iParty ) - sbLayerpc;
			if( sbLayerDiff < -1 || sbLayerDiff > 1 )
				continue;

			// Test distance
			fX = pParty->GetPosX( iParty ) - fXpc;
			fZ = pParty->GetPosZ( iParty ) - fZpc;

			fX *= fRatio;
			fZ *= fRatio;

			if( fX > DEF_SHOW_DISTANCE || fX < -DEF_SHOW_DISTANCE || fZ > DEF_SHOW_DISTANCE || fZ < -DEF_SHOW_DISTANCE )
				continue;

			nX = nCX + fX;
			nY = nCY + fZ;

			AddIconName( nX + m_rcPoint[RADAR_PARTY].Left,  nY + m_rcPoint[RADAR_PARTY].Top,
						 nX + m_rcPoint[RADAR_PARTY].Right, nY + m_rcPoint[RADAR_PARTY].Bottom, pParty->GetMemberName(iParty));

			pDrawPort->AddTexture( nX + m_rcPoint[RADAR_PARTY].Left,  nY + m_rcPoint[RADAR_PARTY].Top,
												nX + m_rcPoint[RADAR_PARTY].Right, nY + m_rcPoint[RADAR_PARTY].Bottom,
												uv.U0, uv.V0, uv.U1, uv.V1, 0xFFFFFFFF );
		}
	}

	if( m_pMapOption->IsCheck(eNT_FELLOWSHIP) == true )
	{
		if (pParty == NULL)
			return;

		UIRectUV uv = m_pMapOption->GetUV(eNT_FELLOWSHIP);

		for(int i=0; i<EXPEDITION_GROUP_MAX; i++)
		{
			for( int j = 0; j < EXPEDITION_MEMBER_PER_GROUP; j++ )
			{
				if(pParty->IsExpedetionDataExist(i, j))
				{
					if(!pParty->GetExpeditionMemberOnline(i,j))	// 온라인 멤버가 아니면 그리지 않는다.
						continue;
					
					if(pParty->GetExpeditionMemberIndex(i,j) == _pNetwork->MyCharacterInfo.index)	// [090729 sora] 원정대 멤버리스트에 자신도 포함되어있으므로 자신은 표시하지 않는다
						continue;

					if (pParty->GetExpeditionMemberZone(i, j) != _pNetwork->MyCharacterInfo.zoneNo) // 원정대 멤버중 같은 존이 아닐경우
						continue;

					sbLayerDiff = pParty->GetExpeditionMemberLayer(i, j) - sbLayerpc;
					if( sbLayerDiff < -1 || sbLayerDiff > 1 )
						continue;

					// Test distance
					fX = pParty->GetExpeditionMemberPosX(i, j) - fXpc;
					fZ = pParty->GetExpeditionMemberPosZ(i, j) - fZpc;

					fX *= fRatio;
					fZ *= fRatio;

					if( fX > DEF_SHOW_DISTANCE || fX < -DEF_SHOW_DISTANCE || fZ > DEF_SHOW_DISTANCE || fZ < -DEF_SHOW_DISTANCE )
						continue;

					nX = nCX + fX;
					nY = nCY + fZ;

					AddIconName( nX + m_rcPoint[RADAR_EXPEDITION].Left,  nY + m_rcPoint[RADAR_EXPEDITION].Top,
								 nX + m_rcPoint[RADAR_EXPEDITION].Right, nY + m_rcPoint[RADAR_EXPEDITION].Bottom, pParty->GetExpeditionMemberName(i, j));

					pDrawPort->AddTexture( nX + m_rcPoint[RADAR_EXPEDITION].Left,  nY + m_rcPoint[RADAR_EXPEDITION].Top,
														nX + m_rcPoint[RADAR_EXPEDITION].Right, nY + m_rcPoint[RADAR_EXPEDITION].Bottom,
														uv.U0, uv.V0, uv.U1, uv.V1, 0xFFFFFFFF );
				}
			}
		}
	}

	// 공성
	// 현재 공성중이고 내가 공성에 참여중이라면
	if( _pUISWDoc->IsWar() && _pNetwork->MyCharacterInfo.sbJoinFlagMerac != WCJF_NONE ) 
	{
		UIRectUV uv = m_pMapOption->GetUV(eNT_MONSTER);
		ACTORMGR()->DrawCharRadar(nCX, nCY, fRatio, pDrawPort, m_rcPoint[RADAR_MOB], uv);
	}

	pDrawPort->FlushRenderingQueue();

	CEntity* penObject = NULL;
	ObjInfo* pInfo = ObjInfo::getSingleton();
	// Target
	if( pInfo->IsTargetActive(eTARGET) )
	{
		// Get target
		penObject = pInfo->GetTargetEntity(eTARGET);

		if( penObject == NULL ) return;

		vObjectPos = penObject->en_plPlacement.pl_PositionVector;

		// Test distance
		fX = vObjectPos(1) - fXpc;
		fZ = vObjectPos(3) - fZpc;

		fX *= fRatio;
		fZ *= fRatio;

		if (fX < DEF_SHOW_DISTANCE || fX > -DEF_SHOW_DISTANCE || fZ < DEF_SHOW_DISTANCE || fZ > -DEF_SHOW_DISTANCE)
		{
			nX = nCX + fX;
			nY = nCY + fZ;

			pDrawPort->InitTextureData( m_ptdBaseTexture );

			pDrawPort->AddTexture( nX + m_rcTarget.Left,  nY + m_rcTarget.Top,
				nX + m_rcTarget.Right, nY + m_rcTarget.Bottom,
				m_rtTarget.U0, m_rtTarget.V0, m_rtTarget.U1, m_rtTarget.V1,
				0xFFFFFFFF );

			static __int64	llOldTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();
			static __int64	llElapsedTime = 0;
			__int64			llCurTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();

			llElapsedTime += llCurTime - llOldTime;
			llOldTime = llCurTime;

			if( llElapsedTime > 1800 )
				llElapsedTime = 0;

			FLOAT	fAniRatio = llElapsedTime / 1800.0f;
			if( fAniRatio < 0.65f )
			{
				if( fAniRatio < 0.5f )
				{
					FLOAT	fSin = sinf( 3.141592f * fAniRatio );
					FLOAT	fSize = 2.0f + 11.0f * fSin;
					UBYTE	ubAlpha = 0xFF * ( 1.0f - fSin );
					pDrawPort->AddTexture( nX - fSize,  nY - fSize, nX + fSize, nY + fSize,
						m_rtTargetAni.U0, m_rtTargetAni.V0, m_rtTargetAni.U1, m_rtTargetAni.V1,
						0xFFFFFF00 | ubAlpha );
				}

				if( fAniRatio > 0.15f )
				{
					FLOAT	fSin = sinf( 3.141592f * ( fAniRatio - 0.15f ) );
					FLOAT	fSize = 2.0f + 11.0f * fSin;
					UBYTE	ubAlpha = 0xFF * ( 1.0f - fSin );
					pDrawPort->AddTexture( nX - fSize,  nY - fSize, nX + fSize, nY + fSize,
						m_rtTargetAni.U0, m_rtTargetAni.V0, m_rtTargetAni.U1, m_rtTargetAni.V1,
						0xFFFFFF00 | ubAlpha );
				}
			}

			pDrawPort->FlushRenderingQueue();
		}
	} // Target
}

// ----------------------------------------------------------------------------
// Name : ShowToolTip()
// Desc :
// ----------------------------------------------------------------------------
void CUIRadar::ShowToolTip( BOOL bShow, int nToolTipID )
{
	static int	nOldToolTipID = -1;

	if( !bShow )
	{
		nOldToolTipID = -1;
		m_bShowToolTip = FALSE;
		return;
	}

	m_bShowToolTip = TRUE;

	if( nOldToolTipID != nToolTipID )
	{
		m_bRemote = false;
		extern INDEX	g_iEnterChat;
		int	nInfoX, nInfoY, nWidth, nHeight;

		nOldToolTipID = nToolTipID;
		switch( nToolTipID )
		{
		case STT_OPTION:		// Option
			{
				m_strToolTip = _S( 442, "정보표시 옵션" );
				m_colTooltip = 0xF2F2F2FF;
				m_btnOption.GetAbsPos( nInfoX, nInfoY );
				nWidth = m_btnOption.GetWidth();
				nHeight = m_btnOption.GetHeight();
			}
			break;

		case STT_MAP:		// Map
			{
				if( g_iEnterChat )
					m_strToolTip.PrintF( "%s %s", _S( 190, "지도" ), CTString( "(Alt+M)" ) );
				else
					m_strToolTip.PrintF( "%s %s", _S( 190, "지도" ), CTString( "(M,Alt+M)" ) );

				m_colTooltip = 0xF2F2F2FF;
				m_btnMap.GetAbsPos( nInfoX, nInfoY );
				nWidth = m_btnMap.GetWidth();
				nHeight = m_btnMap.GetHeight();
			}
			break;

		case STT_SIGNAL:		// Signal
			{
				m_strToolTip = _S(2222, "시그널" );
				m_colTooltip = 0xF2F2F2FF;
				m_btnSignal.GetAbsPos( nInfoX, nInfoY );
				nWidth = m_btnSignal.GetWidth();
				nHeight = m_btnSignal.GetHeight();
			}
			break;

		case STT_EXPRESSREMOTE:
			{
				if ( _pUIBuff->GetMyTimerItemBuffExist(TIMETITEM_TYPE_REMOTE_EXPRESS_SYSTEM) == true)
				{
					tm*	pTimeEnd = NULL;
					int	nTime	= _pUIBuff->GetMyTimerItemBuffExpireTime(TIMETITEM_TYPE_REMOTE_EXPRESS_SYSTEM);
					char expire[25] = {0,};

					pTimeEnd = localtime((time_t*)&nTime);
					if (pTimeEnd)
					{
						strftime(expire, sizeof(expire), "%Y/%m/%d %I:%M:%S", pTimeEnd);
						m_strToolTip.PrintF( _S( 6023, "원격 사용 가능") );
						m_strTimer = expire;
						m_colTooltip = 0x00FA00FF;
						m_colTooltipTimer = 0x00FA00FF;
						m_bRemote = true;
					}
				}
				else if ( GAMEDATAMGR()->GetExpressData() != NULL && GAMEDATAMGR()->GetExpressData()->GetPremiumBenefit() == true)
				{
					m_strToolTip.PrintF( _S( 6023, "원격 사용 가능") );
					m_strTimer.PrintF( _S( 6328, "프리미엄 캐릭터 혜택 적용 중") );
					m_colTooltip = 0x00FA00FF;
					m_colTooltipTimer = 0xD67FFFFF;
					m_bRemote = true;
				}
				else
				{
					m_strToolTip = _S( 6022, "원격 사용 불가능" );
					m_colTooltip = 0xFA0000FF;
					m_bRemote = false;
				}			
				m_ExpressIcon.GetAbsPos( nInfoX, nInfoY );
				nWidth = m_ExpressIcon.GetWidth();
				nHeight = m_ExpressIcon.GetHeight();
			}			
			break;

		case STT_PETSTASH:
			{
				m_strToolTip = _S(5954, "펫 창고" );
				m_colTooltip = 0xF2F2F2FF;
				m_PetStashIcon.GetAbsPos( nInfoX, nInfoY );
				nWidth = m_PetStashIcon.GetWidth();
				nHeight = m_PetStashIcon.GetHeight();
			}
			break;
		case STT_ITEMCOLLECTION:
			{
				m_strToolTip = _S(7036, "아이리스 아이템 도감" );
				m_colTooltip = 0xF2F2F2FF;
				m_ItemCollectionIcon.GetAbsPos( nInfoX, nInfoY );
				nWidth = m_ItemCollectionIcon.GetWidth();
				nHeight = m_ItemCollectionIcon.GetHeight();
			}
			break;
		case STT_PREMIUMCHAR:
			{
#ifdef PREMIUM_CHAR
				CPremiumChar* pChar = GAMEDATAMGR()->GetPremiumChar();

				if (pChar == NULL)
					return;

				if (pChar->getType() != PREMIUM_CHAR_TYPE_NONE)
				{
					tm*	pTimeEnd = NULL;
					int	nTime	= pChar->getExpireTime();
					char expire[25] = {0,};

					pTimeEnd = localtime((time_t*)&nTime);

					if (pTimeEnd)
					{
						m_strToolTip = _S(6328, "프리미엄 캐릭터 혜택 적용 중" );
						m_strTimer.PrintF( _S( 6070, "만료 : %d년%d월%d일%d시%d분"), pTimeEnd->tm_year + 1900, pTimeEnd->tm_mon + 1, pTimeEnd->tm_mday, pTimeEnd->tm_hour, pTimeEnd->tm_min );
						m_colTooltip = 0x00FA00FF;
						m_colTooltipTimer = 0x00FA00FF;
						m_bRemote = true;
					}
				}
				else
				{
					m_strToolTip = _S( 6327, "프리미엄 캐릭터 혜택 없음" );
					m_colTooltip = 0xFA0000FF;
					m_bRemote = false;
				}

				m_PremiumCharIcon.GetAbsPos( nInfoX, nInfoY );
				nWidth = m_PremiumCharIcon.GetWidth();
				nHeight = m_PremiumCharIcon.GetHeight();
#else
				return;
#endif	//	PREMIUM_CHAR
			}
			break;
		}

		int nInfoWidth;
		int nInfoHeight;

		int _MaxString = m_strTimer.Length() + 1;

		if ( _MaxString < m_strToolTip.Length() )
		{
			_MaxString = m_strToolTip.Length() + 1;
		}
		int _nLine = 2;
		if (m_bRemote)
		{
#if defined G_THAI
			nInfoWidth = 19 - _pUIFontTexMgr->GetFontSpacing() + FindThaiLen(m_strToolTip);
#else	//	if defined G_THAI
			nInfoWidth = 19 - _pUIFontTexMgr->GetFontSpacing() + _MaxString *
				( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );
#endif	//	if defined G_THAI
			nInfoHeight = 20 * _nLine;
		}
		else
		{
#if defined G_THAI
			nInfoWidth = 19 - _pUIFontTexMgr->GetFontSpacing() + FindThaiLen(m_strToolTip);
#else	//	if defined G_THAI
			nInfoWidth = 19 - _pUIFontTexMgr->GetFontSpacing() + m_strToolTip.Length() *
				( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );
#endif	//	if defined G_THAI
			nInfoHeight = 22;
		}

		nInfoX += ( nWidth - nInfoWidth ) / 2;
		nInfoY += nHeight + 2;

		CUIManager* pUIManager = CUIManager::getSingleton();

		if( nInfoX + nInfoWidth > pUIManager->GetMaxI() )
			nInfoX += pUIManager->GetMaxI() - ( nInfoX + nInfoWidth );

		m_rcToolTip.SetRect( nInfoX, nInfoY, nInfoX + nInfoWidth, nInfoY + nInfoHeight );
	}
}

// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUIRadar::MouseMessage( MSG *pMsg )
{
	WMSG_RESULT	wmsgResult;

	static BOOL	bInOptionPopup = FALSE;	// If mouse is in option popup or not

	// Mouse point
	int	nX = LOWORD( pMsg->lParam );
	int	nY = HIWORD( pMsg->lParam );

	if (m_pMapOption != NULL && m_pMapOption->IsInside(nX, nY))
	{
		if (m_pMapOption->GetHide() == FALSE)
		{
			if (m_bShowOptionPopup == TRUE)
				bInOptionPopup = TRUE;
	
			return m_pMapOption->MouseMessage(pMsg);
		}
	}

	// Mouse message
	switch( pMsg->message )
	{
	case WM_MOUSEMOVE:
		{
			m_bShowLeftTimeTooltip = FALSE;
			// Popup of option
			// Close option popup
			if( bInOptionPopup )
			{
				bInOptionPopup = FALSE;
				m_bShowOptionPopup = FALSE;
			}
			// Option button
			if( m_btnRR.MouseMessage( pMsg ) != WMSG_FAIL ||
				m_btnZoomIn.MouseMessage( pMsg ) != WMSG_FAIL ||
				m_btnZoomOut.MouseMessage( pMsg ) != WMSG_FAIL )
			{
				return WMSG_SUCCESS;
			}
			if( IsInsideRect( nX, nY, m_rcSandglassRR ) )
			{
				m_bShowLeftTimeTooltip = TRUE;
				return WMSG_SUCCESS;
			}
			if( m_btnMap.MouseMessage( pMsg ) != WMSG_FAIL )
			{
				if (IsRadarUse())
					ShowToolTip( TRUE, STT_MAP );
				return WMSG_SUCCESS;
			}
			if( m_btnOption.MouseMessage( pMsg ) != WMSG_FAIL )
			{
				if (IsRadarUse())
					ShowToolTip( TRUE, STT_OPTION );
				return WMSG_SUCCESS;
			}
			if( m_btnSignal.MouseMessage( pMsg ) != WMSG_FAIL ) 
			{
				if (IsRadarUse())
					ShowToolTip( TRUE, STT_SIGNAL );
				return WMSG_SUCCESS;
			}

			bool bToolTip = false;
			if ( m_ExpressIcon.MouseMessage( pMsg ) != WMSG_FAIL )
			{
				ShowToolTip( TRUE, STT_EXPRESSREMOTE );
				bToolTip = true;
			}
			if ( m_PetStashIcon.MouseMessage( pMsg ) != WMSG_FAIL )
			{
				ShowToolTip( TRUE, STT_PETSTASH );
				bToolTip = true;
			}
			if (m_ItemCollectionIcon.MouseMessage( pMsg ) != WMSG_FAIL)
			{
				ShowToolTip(TRUE, STT_ITEMCOLLECTION);
				bToolTip = true;
			}
#ifdef PREMIUM_CHAR
			if ( m_PremiumCharIcon.MouseMessage( pMsg ) != WMSG_FAIL || 
				(m_PremiumCharIcon.GetBtnState() == UBS_DISABLE && m_PremiumCharIcon.IsInside(nX, nY) ) )
			{
				ShowToolTip( TRUE, STT_PREMIUMCHAR );
				bToolTip = true;
			}
#endif	// PREMIUM_CHAR

			if (bToolTip == true)
				return WMSG_SUCCESS;
		
			if( IsInside( nX, nY ) )
			{
				m_bInsideMouse = TRUE;

				if( m_bSignalBtnOn ) 
				{
					SetSignalOn( true );
					return WMSG_SUCCESS;
				}
				else
				{
					CUIManager::getSingleton()->SetMouseCursorInsideUIs();
				}

				m_nSelectedIcon = -1;
				for( int i = 0; i < m_iconVec.size(); ++i )
				{
					if( IsInsideRect( nX, nY, m_iconVec[i].rc ) )
					{
						m_nSelectedIcon = i;
						break;
					}
				}
			}
			else // 레이더 영역 밖으로 나가면 취소됨.
			{
				SetSignalOn( FALSE );
				m_bInsideMouse = FALSE;
			}			
			// Hide tool tip
			ShowToolTip( FALSE );
		}
		break;

	case WM_LBUTTONDOWN:
		{
			if( m_bEnableRR && IsInsideRect( nX, nY, m_rcBtnRR) )
			{
				if( m_btnRR.MouseMessage( pMsg ) != WMSG_FAIL )	{}
				return WMSG_SUCCESS;
			}
			else if( IsInside( nX, nY ) )
			{
				// Option button
				m_btnOption.MouseMessage( pMsg );
				// Map button
				m_btnMap.MouseMessage( pMsg );	
				m_btnSignal.MouseMessage( pMsg );
				m_btnZoomIn.MouseMessage( pMsg );
				m_btnZoomOut.MouseMessage( pMsg );
				m_ExpressIcon.MouseMessage( pMsg );
				m_PetStashIcon.MouseMessage( pMsg );
				m_ItemCollectionIcon.MouseMessage(pMsg);
#ifdef PREMIUM_CHAR
				m_PremiumCharIcon.MouseMessage( pMsg );
#endif	//	PREMIUM_CHAR

				CUIManager::getSingleton()->RearrangeOrder( UI_RADAR, TRUE );
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
				if( m_bEnableRR && ( wmsgResult = m_btnRR.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
					{
						RoyalRumbleJoinReqMsgBox();
					}

					return WMSG_SUCCESS;
				}
				// If radar isn't focused
				if( !IsFocused() )
					return WMSG_FAIL;

				if (IsRadarUse())
				{
					// Option button
					if( ( wmsgResult = m_btnOption.MouseMessage( pMsg ) ) != WMSG_FAIL )
					{
						if( wmsgResult == WMSG_COMMAND )
						{
							m_bShowOptionPopup = !m_bShowOptionPopup;
						}
	
						return WMSG_SUCCESS;
					}
					
					// Map button
					else if( ( wmsgResult = m_btnMap.MouseMessage( pMsg ) ) != WMSG_FAIL )
					{
						if( wmsgResult == WMSG_COMMAND )
						{
							pUIManager->GetMap()->ToggleVisible();
						}
	
						return WMSG_SUCCESS;
					}
					else if( ( wmsgResult = m_btnSignal.MouseMessage( pMsg ) ) != WMSG_FAIL ) 
					{
						if( wmsgResult == WMSG_COMMAND )
						{
							if( !IsPossibleSignal() ) return WMSG_SUCCESS;
							// 시그널 버튼을 누르면 레이더상에 최종 위치 표시해줌
							m_Signal.dStartTime = _pTimer->GetHighPrecisionTimer().GetSeconds();
							m_Signal.bVisible = TRUE;
	
							// Signal 셋팅
							SetSignalOn( TRUE );
						}
	
						return WMSG_SUCCESS;
					}
					else if( ( wmsgResult = m_btnZoomIn.MouseMessage( pMsg ) ) != WMSG_FAIL )
					{
						if( wmsgResult == WMSG_COMMAND )
						{
							if( m_fZoomRate < 5.0f )
							{
								m_fZoomRate += 1.0f;
							}
						}
	
						return WMSG_SUCCESS;
					}
					else if( ( wmsgResult = m_btnZoomOut.MouseMessage( pMsg ) ) != WMSG_FAIL )
					{
						if( wmsgResult == WMSG_COMMAND )
						{
							if( m_fZoomRate > 1.0f )
							{
								m_fZoomRate -= 1.0f;
							}
						}
	
						return WMSG_SUCCESS;
					}
				}
				if ( m_ExpressIcon.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					ExpressSystem* pData = GAMEDATAMGR()->GetExpressData();
					
					if(_pUIBuff->GetMyTimerItemBuffExist(TIMETITEM_TYPE_REMOTE_EXPRESS_SYSTEM) == true ||
						( pData != NULL && pData->GetPremiumBenefit() == true))
					{
						GameDataManager* pGameData = GameDataManager::getSingleton();
						if (pGameData)
						{
							ExpressSystem* pExpress = pGameData->GetExpressData();
							
							if (pExpress)
							{
								pExpress->RemoteitemUse();
							}
						}
					}
					else
					{
						if ( pUIManager->DoesMessageBoxExist(MSGCMD_EXPRESS_REMOTE_ERROR) )
						{
							pUIManager->CloseMessageBox( MSGCMD_EXPRESS_REMOTE_ERROR );
						}
						CTString strMessage, strTitle;
						CUIMsgBox_Info msgBoxInfo;						
						strTitle.PrintF(_S( 191, "확인" ));
						msgBoxInfo.SetMsgBoxInfo( strTitle, UMBS_OK, UI_RADAR ,MSGCMD_EXPRESS_REMOTE_ERROR);
						strMessage.PrintF( _S( 6025, "신비한 석상을 찾아 가시거나, LCE 원격 이용권을 통해 이용하실 수 있습니다.") );
						msgBoxInfo.AddString(strMessage);	
						if ( pUIManager )
							pUIManager->CreateMessageBox( msgBoxInfo );
					}
					return WMSG_SUCCESS;
				}
				else if ( m_PetStashIcon.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					if (pUIManager->GetPetStash()->IsVisible() == FALSE)
						pUIManager->GetPetStash()->OpenPetStash();
					return WMSG_SUCCESS;
				}
				else if (m_ItemCollectionIcon.MouseMessage( pMsg )!= WMSG_FAIL)
				{
					if (pUIManager->GetItemCollection()->IsVisible() == FALSE)
						pUIManager->GetItemCollection()->OpenUI();
					return WMSG_SUCCESS;
				}
#ifdef PREMIUM_CHAR
				else if (m_PremiumCharIcon.MouseMessage( pMsg )!= WMSG_FAIL)
				{
					if (m_PremiumCharIcon.GetBtnState() == UBS_DISABLE)
						return WMSG_SUCCESS;

					if (pUIManager->GetPremiumChar()->IsVisible() == FALSE)
						pUIManager->GetPremiumChar()->OpenUI();
					return WMSG_SUCCESS;
				}
#endif	//	PREMIUM_CHAR
				if( IsInside( nX, nY ) )
				{
					if( !m_bSignalBtnOn )
					{
						return WMSG_SUCCESS;
					}

					SetSignalOn( FALSE );
					
					FLOAT fX = nX;
					FLOAT fY = nY;

					if( RadarToWorld( fX, fY ) )
					{
						SendSignal( fX, fY );
						pUIManager->RearrangeOrder( UI_RADAR, TRUE );
					}
					return WMSG_SUCCESS;
				}
			}
			// If holding button exists
			else
			{
				if( IsInside( nX, nY ) )
				{
					SetSignalOn( FALSE );
					// Reset holding button
					pUIManager->ResetHoldBtn();

					return WMSG_SUCCESS;
				}
			}
		}
		break;

	case WM_LBUTTONDBLCLK:
		{
			if( m_btnZoomIn.MouseMessage( pMsg ) != WMSG_FAIL )
			{
				// Nothing.
				return WMSG_SUCCESS;
			}
			else if( m_btnZoomOut.MouseMessage( pMsg ) != WMSG_FAIL )
			{
				// Nothing.
				return WMSG_SUCCESS;
			}
			else if( IsInsideRect( nX, nY, m_rcTop ) )
				return WMSG_SUCCESS;
		}
		break;
	}

	return WMSG_FAIL;
}

#define SIGNAL_TIME				5.0f								// 시그널 표시 시간 
#define SIGNAL_COUNT			5.0f								// 시그널 표시 횟수 
#define SIGNAL_UNITTIME			( SIGNAL_TIME / SIGNAL_COUNT )		// 한번 표시가 할때 걸리는 시간
#define SIGNAL_UNITTIME_HALF	( SIGNAL_UNITTIME / 2 ) 

//------------------------------------------------------------------------------
// CUIRadar::RenderSignal
// Explain:  
// Date : 2005-10-27,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIRadar::RenderSignal()
{
	static FLOAT fAngleOffSet = 270.0f; // 방향 표시기가 ^를 향하기 있도록 하기위해서 270도를 꺽어준다.
	MapData* md;
	md = CUIManager::getSingleton()->GetMap()->GetCurMapData( m_nZone );
	FLOAT	fSqrDist = RADAR_VIEW_DISTANCE * RADAR_VIEW_DISTANCE / m_fZoomRate;
	FLOAT	fRatio = md->World.fRatio * m_fZoomRate;

	if( !m_Signal.bVisible ) return;

	// 표시 시간 및 2d 효과 조정
	DOUBLE dElapsedTime = _pTimer->GetHighPrecisionTimer().GetSeconds();
	DOUBLE dDealy		= dElapsedTime - m_Signal.dStartTime;

	COLOR	colBlend;
	
	if( dDealy > SIGNAL_TIME ) 
	{
		m_Signal.bVisible = FALSE;
		return;
	}

	BYTE	bBlend;
	BOOL	bFirst = TRUE;
	COLOR	colOriBlend = 0xFFFFFFFF;
	int		nCount = SIGNAL_COUNT-1;
			
	// signal의 Alpha & Size 조정 
	while( dDealy  > SIGNAL_UNITTIME )
	{
		nCount--;
		dDealy -= SIGNAL_UNITTIME;
		bFirst	= FALSE;
	}

	if( dDealy > SIGNAL_UNITTIME_HALF )
	{
		DOUBLE fTemp = ( dDealy - SIGNAL_UNITTIME_HALF ) / SIGNAL_UNITTIME_HALF;
		fTemp = 0xFF * ( fTemp );
		bBlend = 0xFF - fTemp;

		if( nCount <= 0 ) 
		{
			colOriBlend = ( 0xFFFFFFFF - 0xFF ) | bBlend;
		}
	}
	else 
	{
		DOUBLE fTemp = ( dDealy ) / SIGNAL_UNITTIME_HALF;
		fTemp = 0xFF * ( fTemp );
		bBlend = fTemp;
	}
		
	colBlend = ( 0xFFFFFFFF - 0xFF ) | bBlend;

	int	nX = m_nPosX + RADAR_CENTER_OFFSETX;
	int	nY = m_nPosY + RADAR_CENTER_OFFSETY;

	// Get My position
	FLOAT	fXpc = _pNetwork->MyCharacterInfo.x;
	FLOAT	fZpc = _pNetwork->MyCharacterInfo.z;

	// 케릭터의 위치를 원점으로 한 시그널의 위치
	FLOAT fX = m_Signal.fX - fXpc;
	FLOAT fZ = m_Signal.fZ - fZpc;
	
	FLOAT fDist = fX * fX + fZ * fZ;
	
	// Position & direction of character
	FLOAT	fSin = Sin( _pNetwork->MyCharacterInfo.camera_angle );
	FLOAT	fCos = Cos( _pNetwork->MyCharacterInfo.camera_angle );

	// Render Signal ( 레이더의 범위안에 있는 경우 ) 
	if( fDist <= fSqrDist ) 
	{
		fX *= fRatio;
		fZ *= fRatio;

		nX += fX;
		nY += fZ;

		FLOAT fWidth = (  m_rcSignal.Right - m_rcSignal.Left ) / 2;
		FLOAT fHeight = ( m_rcSignal.Bottom - m_rcSignal.Top ) / 2;

		FLOAT	fOriWidth = fWidth;
		FLOAT	fOriHeight = fHeight;

		if( bFirst ) // 맨 처음에는 크게 ~
		{
			fWidth = fWidth + (fWidth + fWidth) * dDealy;
			fHeight = fHeight + (fHeight + fHeight) * dDealy;
		}
		else // 기본 크기로 
		{
			fWidth = fWidth + fWidth * dDealy;
			fHeight = fHeight + fHeight * dDealy;
		}

		CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

		pDrawPort->AddTexture( nX - fWidth,  nY - fHeight,
											nX + fWidth, nY + fHeight,
											m_rtSignal.U0, m_rtSignal.V0,
											m_rtSignal.U1, m_rtSignal.V1,
											colBlend );
	
		pDrawPort->AddTexture( nX - fOriWidth,  nY - fOriHeight,
											nX + fOriWidth, nY + fOriHeight,
											m_rtSignal.U0, m_rtSignal.V0,
											m_rtSignal.U1, m_rtSignal.V1,
											colOriBlend );
		return;
	}

	// 레이더의 표시 범위안에 있지 않는다면 레이더 외각에 방향으로 표시
	
	// 케릭터의 X축을 중심으로 시그널의 각도 구하기
	FLOAT	angle = GetAngle( fX, fZ, 1.0f, 0.0f );	 // X 축 중심
	angle *= RADIAN;

	if( fZ < 0 ) // 360도 
	{
		angle = 360 - angle;
	}

	angle -= fAngleOffSet;  // 방향 표시기가 ^를 향하기 있도록 하기위해서 90도를 꺽어준다.

	FLOAT	fSin2 = Sin( _pNetwork->MyCharacterInfo.camera_angle + angle );
	FLOAT	fCos2 = Cos( _pNetwork->MyCharacterInfo.camera_angle + angle );
	
	FLOAT	fLeftCos = m_rcOutSignal.Left * fCos2;
	FLOAT	fLeftSin = m_rcOutSignal.Left * fSin2;
	FLOAT	fRightCos = m_rcOutSignal.Right * fCos2;
	FLOAT	fRightSin = m_rcOutSignal.Right * fSin2;
	FLOAT	fTopSin = m_rcOutSignal.Top * fSin2;
	FLOAT	fTopCos = m_rcOutSignal.Top * fCos2;
	FLOAT	fBottomSin = m_rcOutSignal.Bottom * fSin2;
	FLOAT	fBottomCos = m_rcOutSignal.Bottom * fCos2;

	FLOAT	fX1 = fLeftCos - fTopSin;
	FLOAT	fY1 = fLeftSin + fTopCos;
	FLOAT	fX2 = fLeftCos - fBottomSin;
	FLOAT	fY2 = fLeftSin + fBottomCos;
	FLOAT	fX3 = fRightCos - fBottomSin;
	FLOAT	fY3 = fRightSin + fBottomCos;
	FLOAT	fX4 = fRightCos - fTopSin;
	FLOAT	fY4 = fRightSin + fTopCos;
	
	FLOAT	fResizeRatio = RADAR_VIEW_DISTANCE / sqrtf( fDist );
	
	fX *= fResizeRatio;
	fZ *= fResizeRatio;

	fX *= fRatio;
	fZ *= fRatio;

	nX += fX;
	nY += fZ;

	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	pDrawPort->AddTexture( nX + fX1, nY + fY1, m_rtOutSignal.U0, m_rtOutSignal.V0, colBlend,
										nX + fX2, nY + fY2, m_rtOutSignal.U0, m_rtOutSignal.V1, colBlend,
										nX + fX3, nY + fY3, m_rtOutSignal.U1, m_rtOutSignal.V1, colBlend,
										nX + fX4, nY + fY4, m_rtOutSignal.U1, m_rtOutSignal.V0, colBlend );
}

//------------------------------------------------------------------------------
// CUIRadar::SetSignal
// Explain:  레이더의 시그널의 항상 최신 것을 기준으로 1개만 표시된다.
// Date : 2005-11-01,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIRadar::SetSignal( float fX, float fZ )
{
	m_Signal.bVisible = TRUE;
	
	m_Signal.fX = fX;
	m_Signal.fZ = fZ;

	m_Signal.dStartTime = _pTimer->GetHighPrecisionTimer().GetSeconds();
}

//------------------------------------------------------------------------------
// CUIRadar::SendSignal
// Explain: 서버에 시그널을 보냄
// 맵의 시그널 추가 루틴을 통해서 정송됨.
// Date : 2005-11-01,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIRadar::SendSignal( float fX, float fZ )
{
	if( IsPossibleSignal() )
	{
		CUIManager::getSingleton()->GetMap()->SendSignal( fX, fZ );
	}
}
		

//------------------------------------------------------------------------------
// CUIRadar::RadarToWorld
// Explain:  레이더 상의 좌표를 World 좌표로 변경 
// * Return Value : 입력된 자표가 레이더 상에 있는지
// Date : 2005-11-01,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
bool CUIRadar::RadarToWorld( float& fX, float& fZ )
{
	MapData* md;
	md = CUIManager::getSingleton()->GetMap()->GetCurMapData( m_nZone );
	FLOAT	fSqrDist = RADAR_VIEW_DISTANCE * RADAR_VIEW_DISTANCE / m_fZoomRate;
	FLOAT	fRatio = md->World.fRatio * m_fZoomRate;

	double nTempX;
	double nTempZ;
	
	double	fSin = Sin( -_pNetwork->MyCharacterInfo.camera_angle ); // 현재 케릭의 각도에 역 회전 방향
	double	fCos = Cos( -_pNetwork->MyCharacterInfo.camera_angle );
	
	fX -= m_nPosX + RADAR_CENTER_OFFSETX; // 레이더 중심 좌표를 기준( 레이터 상의 케릭터의 위치 )
	fZ -= m_nPosY + RADAR_CENTER_OFFSETY;

	fX /= fRatio;	// 축척 
	fZ /= fRatio;

	nTempX = ( fX * fCos - fZ * fSin ); // 회전 
	nTempZ = ( fX * fSin + fZ * fCos );

	fX = nTempX;
	fZ = nTempZ;

	// Get My position
	FLOAT	fXpc = _pNetwork->MyCharacterInfo.x;
	FLOAT	fZpc = _pNetwork->MyCharacterInfo.z;
	
	if( ( fX * fX + fZ * fZ ) <= fSqrDist )
	{
		fX += fXpc; // 케릭터의 위치가 중심이기 때문에 Offset 조정
		fZ += fZpc;
	
		return true;
	}
		
	return false;
}

//------------------------------------------------------------------------------
// CUIRadar::IsPossibleSignal
// Explain:  시그널을 보낼 수 있는지 확인
// Date : 2005-11-01,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
bool CUIRadar::IsPossibleSignal()
{
	// 길드장, 부장인경우 
	// 공성 맵인 경우 
	LONG nRanking = _pNetwork->MyCharacterInfo.lGuildPosition;

	if( nRanking != GUILD_MEMBER_BOSS && nRanking != GUILD_MEMBER_VICE_BOSS )
		return false; 

	if( !IsPossibleCastleRadar() ) 
		return false; 

	return true;
}

//------------------------------------------------------------------------------
// IsPossibleCastleRadar
// Explain: 공성 레이더를 사용할 수 있는 상황인지 확인 ( 버튼 활성화 여부 )
// Date : 2005-11-01,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
bool CUIRadar::IsPossibleCastleRadar()
{
	if( !_pUISWDoc->IsWar() || _pNetwork->MyCharacterInfo.sbJoinFlagMerac == WCJF_NONE )
		return false;
	
	if( _pNetwork->MyCharacterInfo.zoneNo != 7 )
		return false;

	return true;
}

//------------------------------------------------------------------------------
// CUIMap::SetSignalOn
// Explain:  시그널 변수를 셋팅하고 마우스 커서를 변경 합니다.
// Date : 2005-11-01,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIRadar::SetSignalOn( bool bOn ) 
{
	if( !bOn )
	{
		if( m_bSignalBtnOn ) 
		{
			m_bSignalBtnOn = FALSE; 
			CUIManager::getSingleton()->SetMouseCursorInsideUIs();
		}	
	}
	else
	{
		if( !IsPossibleSignal() ) return;

		m_bSignalBtnOn = TRUE;
		CUIManager::getSingleton()->SetMouseCursorInsideUIs( UMCT_SIGNAL );
	}

}


//------------------------------------------------------------------------------
//  CUIRadar::KeyMessage
// Explain: Ctrl키를 눌렀을 때 시그널을 보낼 수 있도록 KeyMessage를 받는다 
// * 일반적인 UIWindow는 KEYUP 메세지를 받지 않는다. ( 조합키를 사용하는 곳이 없음)
// *  UIMgr에서 MAP과 Rar만 예외 처리해서 KEYUP메세지를 받도록 하였음
// Date : 2005-10-24,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
WMSG_RESULT CUIRadar::KeyMessage( MSG *pMsg )
{
	if( !IsFocused() )
		return WMSG_FAIL;
	
	// 마우스가 지도안에 있을 때만 처리한다.
	if( !m_bInsideMouse ) 
	{
		return WMSG_FAIL;
	}

	/* UP키를 예외 처리하지 않고 F1키 눌민면 그냥 가는방식
	if( pMsg->wParam == VK_F1 )
	{
		SetSignalOn( true );
		return WMSG_SUCCESS;
	}
	*/
	
	// 시그널을 사용할 수 있는지 체크 
	if( !IsPossibleSignal() ) 
	{
		return WMSG_FAIL;
	}

	switch( pMsg->message )
	{
	case WM_KEYDOWN:
		if( pMsg->wParam == VK_CONTROL )
		{
			SetSignalOn( true );
			return WMSG_SUCCESS;
		}
		break;

	case WM_KEYUP:
		if( pMsg->wParam == VK_CONTROL )
		{
			SetSignalOn( false );
			return WMSG_SUCCESS;
		}
		break;
	}
	return WMSG_FAIL;
}

// ----------------------------------------------------------------------------
// Name : MsgBoxLCommand(int nCommandCode, int nResult )
// Desc : l-type message box event catcher.
// ----------------------------------------------------------------------------
void CUIRadar::MsgBoxLCommand(int nCommandCode, int nResult )
{
	switch( nCommandCode )
	{
	case MSGLCMD_ROYALRUMBLE_MANAGER:
		{
			if( ROYALRUMBLE_SCHEDULE == nResult )
			{
				_pNetwork->SendRoyalRumbleNextTimeReq();
			}
			else if( ROYALRUMBLE_REJECT == nResult )
			{
				CUIMsgBox_Info	MsgBoxInfo;
				MsgBoxInfo.SetMsgBoxInfo( _S(5405, "로얄럼블 참가신청 취소"), UMBS_YESNO, UI_RADAR, MSGCMD_ROYALRUMBLE_REJECT_REQ);
											
				MsgBoxInfo.AddString( _S(5406, "로얄럼블 참가신청을 취소하시면 참가신청 비용은 돌려받을 수 없습니다. 로얄럼블 참가신청을 취소하시겠습니까?") );
				CUIManager::getSingleton()->CreateMessageBox(MsgBoxInfo);
			}
			else if( ROYALRUMBLE_REWARD == nResult )
			{
				_pNetwork->SendRoyalRumbleRewardReq();
			}
			else if( ROYALRUMBLE_POINT == nResult )
			{
				_pNetwork->SendRoyalRumblePointReq();
			}
			else
			{
				// nothing.
			}

		}
		break;
	}
}

// ----------------------------------------------------------------------------
// Name : MsgBoxCommand(int nCommandCode, BOOL bOK, CTString &strInput )
// Desc : normal-type message box event catcher.
// ----------------------------------------------------------------------------
void CUIRadar::MsgBoxCommand(int nCommandCode, BOOL bOK, CTString &strInput )
{
	switch( nCommandCode )
	{
		case MSGCMD_ROYALRUMBLE_JOIN_REQ:
			{
				if( bOK )
				{
					_pNetwork->SendRoyalRumbleJoinReq();
				}
			}
			break;
		case MSGCMD_ROYALRUMBLE_REJECT_REQ:
			{
				if( bOK )
				{
					_pNetwork->SendRoyalRumbleRejectReq();
				}
			}
			break;
		case MSGCMD_ROYALRUMBLE_GO_ZONE:
			{
				if( bOK )
				{
					_pNetwork->SendRoyalRumbleStartReq();
				}	
			}
			break;
		case MSGCMD_ROYALRUMBLE_WINNER_OUT:
			{
				if( bOK )
				{
					_pNetwork->GoZone( 0, 0 );
				}
				m_bShowTrophy = FALSE;
				m_bRenderTrophy = FALSE;
				m_ubTrophyAlpha = 0;
			}
			break;
	}
}
// ----------------------------------------------------------------------------
// Name : CountCipher()
// Desc : return their cipher
// ----------------------------------------------------------------------------
int CUIRadar::CountCipher(int tv_time, int tv_cipher /* = 1  */)
{
	// [2011/09/16 : Sora] 기존 자리수계산수식에 문제가 있어서  한지리 부족하게 계산되어서 공식수정 및 정리
	int tv_sum = tv_time/ 10;	
	return tv_sum>0 ? CountCipher( tv_sum, tv_cipher+1 ) : tv_cipher;
}

void CUIRadar::DisplayNum( int tv_time,UIRect tv_rect)
{
	int tv_c,tv_i,tv_num;
	tv_c = CountCipher(tv_time);
	char *tv_str;
	tv_str = new(char[40]);
	itoa(tv_time,tv_str,10);

	CTString str;
	str.PrintF("%d %d ",tv_c,tv_time);

	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();
	for(tv_i = 0; tv_i < tv_c ; tv_i++ ){
		tv_num=tv_str[tv_c-tv_i-1]-'0';
		pDrawPort->AddTexture( m_nPosX+tv_rect.Left -WORLD_TIME_FONT_SIZE*tv_i, m_nPosY+tv_rect.Top,
										m_nPosX+tv_rect.Right -WORLD_TIME_FONT_SIZE*tv_i, m_nPosY+tv_rect.Bottom,
										m_rtFigure[tv_num].U0, m_rtFigure[tv_num].V0, m_rtFigure[tv_num].U1, m_rtFigure[tv_num].V1,
										0xFFFFFFFF );			
	}
	delete [] tv_str;
}


void CUIRadar::_royalrumbleCreate()
{
	m_ptdRoyalRumbleTexture = CreateTexture( CTString("Data\\Interface\\Map.tex") );
	FLOAT fTexWidth = m_ptdRoyalRumbleTexture->GetPixWidth();
	FLOAT fTexHeight = m_ptdRoyalRumbleTexture->GetPixHeight();

	UIRectUV rtRRIdle, rtRRClick;
	rtRRIdle.SetUV( 52, 136, 136, 183, fTexWidth, fTexHeight );
	rtRRClick.SetUV( 145, 136, 229, 183, fTexWidth, fTexHeight );

	//m_btnRR.Create( this, CTString(""), 33, 193, 84, 47 );
	m_btnRR.Create( this, CTString(""), 6, 230, 84, 47 );
	m_btnRR.CopyUV( UBS_IDLE, rtRRIdle );
	m_btnRR.CopyUV( UBS_CLICK, rtRRClick );
	m_btnRR.CopyUV( UBS_IDLE, UBS_ON );
	m_btnRR.CopyUV( UBS_IDLE, UBS_DISABLE );

	//m_rcRR.SetRect( 33, 193, 84, 47 );
	m_rcBtnRR.SetRect( 6, 230, 92, 277 );
	m_rcSandglassRR.SetRect( 95, 230, 119, 277 );
	m_rtSandglassRR[0].SetUV( 184, 198, 208, 245, fTexWidth, fTexHeight );	// green
	m_rtSandglassRR[1].SetUV( 155, 198, 179, 245, fTexWidth, fTexHeight );	// yellow
	m_rtSandglassRR[2].SetUV( 126, 198, 150, 245, fTexWidth, fTexHeight );	// red

	m_btSandlassStat = 3;		// default : don't render.
	m_liEndTime = 0;
	m_bShowLeftTimeTooltip = FALSE;

	_createTrophyData();
}

void CUIRadar::_royalrumbleButtonRender()
{
	// Timer for highlight buttons
	static BOOL		bHighlight = FALSE;
	static DOUBLE	dElapsedTime = 0.0;
	static DOUBLE	dOldTime = _pTimer->GetHighPrecisionTimer().GetSeconds();
	DOUBLE	dCurTime = _pTimer->GetHighPrecisionTimer().GetSeconds();
	dElapsedTime += dCurTime - dOldTime;
	dOldTime = dCurTime;
	if( dElapsedTime > 0.5 )
	{
		bHighlight = !bHighlight;
		do
		{
			dElapsedTime -= 0.5;
		}
		while( dElapsedTime > 0.5 );
	}	

	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	pDrawPort->InitTextureData( m_ptdRoyalRumbleTexture );
	m_btnRR.Render();
	pDrawPort->FlushRenderingQueue();

	pDrawPort->InitTextureData( m_ptdRoyalRumbleTexture, FALSE, PBT_ADD );
	if( bHighlight )
			m_btnRR.RenderHighlight( 0x404040FF );
	pDrawPort->FlushRenderingQueue();
}

void CUIRadar::_royalrumbleTimeRender()
{
	if( m_btSandlassStat > 2 )
		return;

	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	pDrawPort->InitTextureData( m_ptdRoyalRumbleTexture );
	pDrawPort->AddTexture( m_nPosX + m_rcSandglassRR.Left, m_nPosY + m_rcSandglassRR.Top, m_nPosX + m_rcSandglassRR.Right, m_nPosY + m_rcSandglassRR.Bottom,
										m_rtSandglassRR[m_btSandlassStat].U0, m_rtSandglassRR[m_btSandlassStat].V0, m_rtSandglassRR[m_btSandlassStat].U1, m_rtSandglassRR[m_btSandlassStat].V1,
										0xFFFFFFFF );
	pDrawPort->FlushRenderingQueue();
	if( m_bShowLeftTimeTooltip )
		_showRemainTime();
}

void CUIRadar::RoyalRumbleJoinReqMsgBox()
{
	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( _S(5407, "로얄럼블 참가신청"), UMBS_YESNO, UI_RADAR, MSGCMD_ROYALRUMBLE_JOIN_REQ);
								
	MsgBoxInfo.AddString( _S(5408, "안녕하십니까? 로얄럼블 관리회입니다. 로얄럼블에 참가하기 위해서는 10,000,000 나스의 참가비가 필요합니다. 로얄럼블에 참가하시겠습니까?") );
	CUIManager::getSingleton()->CreateMessageBox(MsgBoxInfo);
}

void CUIRadar::OpenRoyalrumbleMgrMenu( const INDEX iMobIndex )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	pUIManager->CloseMessageBoxL(MSGLCMD_ROYALRUMBLE_MANAGER);
	
	CMobData* MD = CMobData::getData( iMobIndex );

	// Create refine message box.
	pUIManager->CreateMessageBoxL( _S( 5404, "로얄럼블" ), UI_RADAR, MSGLCMD_ROYALRUMBLE_MANAGER );
	CTString strNpcName = CMobData::getData( iMobIndex )->GetName();
	pUIManager->AddMessageBoxLString( MSGLCMD_ROYALRUMBLE_MANAGER, TRUE, strNpcName, -1, 0xE18600FF );
	pUIManager->AddMessageBoxLString( MSGLCMD_ROYALRUMBLE_MANAGER, TRUE, _S(5409,	"안녕하십니까." 
																			"저는 이 곳에서 로얄럼블 관리를 맡고있습니다."
																			"용사님의 강함을 증명하고 싶지 않으십니까?"
																			"그렇다면 로얄럼블에 한번 도전해 보십시오."
																			"\n"
																			"혹시 궁금하신 것이 있으십니까?" ), -1, 0xA3A1A3FF );

	pUIManager->AddMessageBoxLString( MSGLCMD_ROYALRUMBLE_MANAGER, TRUE, _s(	"" ), -1, 0xA3A1A3FF );
	CTString strMessage;
	strMessage.PrintF( _S( 5410, "로얄럼블 일정 확인" ) );
	pUIManager->AddMessageBoxLString( MSGLCMD_ROYALRUMBLE_MANAGER, FALSE, strMessage, ROYALRUMBLE_SCHEDULE );
	strMessage.PrintF( _S( 5405, "로얄럼블 참가신청 취소" ) );
	pUIManager->AddMessageBoxLString( MSGLCMD_ROYALRUMBLE_MANAGER, FALSE, strMessage, ROYALRUMBLE_REJECT );
	strMessage.PrintF( _S( 5411, "로얄럼블 보상품 지급" ) );
	pUIManager->AddMessageBoxLString( MSGLCMD_ROYALRUMBLE_MANAGER, FALSE, strMessage, ROYALRUMBLE_REWARD );
	strMessage.PrintF( _S( 5412, "전장 포인트 조회" ) );
	pUIManager->AddMessageBoxLString( MSGLCMD_ROYALRUMBLE_MANAGER, FALSE, strMessage, ROYALRUMBLE_POINT );
	
	// cancel
	strMessage.PrintF( _S( 1220, "취소한다." ) );
	pUIManager->AddMessageBoxLString( MSGLCMD_ROYALRUMBLE_MANAGER, FALSE, strMessage );
}

void CUIRadar::_createTrophyData()
{
	m_ptdTrophyTexture = CreateTexture( CTString("Data\\Interface\\trophy.tex") );
	FLOAT fTexWidth = m_ptdTrophyTexture->GetPixWidth();
	FLOAT fTexHeight = m_ptdTrophyTexture->GetPixHeight();
	m_bShowTrophy = FALSE;
	m_rtTrophy.SetUV( 20, 18, 352, 330, fTexWidth, fTexHeight );
	m_rtTrophyEffect.SetUV( 396, 0, 636, 248, fTexWidth, fTexHeight );
	m_rcTrophy.SetRect( 0, 0, 332, 312 );
	_initTrophyData();
}

void CUIRadar::_renderTrophy()
{
	if( !m_bRenderTrophy )
		return;
	COLOR colBaseCol = 0xFFFFFF00;
	static BOOL bReverse = FALSE;

	TIME currentTime  = _pTimer->GetHighPrecisionTimer().GetMilliseconds();

	// step 1
	if( currentTime - m_tStartTime > 50 )
	{
		m_tStartTime = currentTime;
		if( bReverse )
		{
			int iAlpha = m_ubTrophyEffectAlpha;
			iAlpha -= 10;
			if( iAlpha < 0x00 )
			{	m_ubTrophyEffectAlpha = 0x00; bReverse = FALSE;	_showWinnerMsgBox(); }
			else
				m_ubTrophyEffectAlpha = iAlpha;
		}
		else 
		{
			int iAlpha = m_ubTrophyEffectAlpha;
			iAlpha += 10;
			if( iAlpha > 0xFF )
				m_ubTrophyEffectAlpha = 0xFF;
			else
				m_ubTrophyEffectAlpha = iAlpha;
		}

		if( m_ubTrophyEffectAlpha > 200 || m_bShowTrophy )
		{
			m_bShowTrophy = TRUE;
			int iAlpha = m_ubTrophyAlpha;
			iAlpha += 20;
			if( iAlpha > 0xFF )
			{
				m_ubTrophyAlpha = 0xFF;
				bReverse = TRUE;
			}
			else
				m_ubTrophyAlpha = iAlpha;
		}
	}

	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	pDrawPort->InitTextureData( m_ptdTrophyTexture );

	pDrawPort->AddTexture( m_rcTrophy.Left-50, m_rcTrophy.Top-50, m_rcTrophy.Right+50, m_rcTrophy.Bottom+50,
										m_rtTrophyEffect.U0, m_rtTrophyEffect.V0, m_rtTrophyEffect.U1, m_rtTrophyEffect.V1, (colBaseCol|m_ubTrophyEffectAlpha) );
	pDrawPort->AddTexture( m_rcTrophy.Left, m_rcTrophy.Top, m_rcTrophy.Right, m_rcTrophy.Bottom,
										m_rtTrophy.U0, m_rtTrophy.V0, m_rtTrophy.U1, m_rtTrophy.V1, (colBaseCol|m_ubTrophyAlpha) );
	pDrawPort->FlushRenderingQueue();
	
}

void CUIRadar::_destroyTrophy()
{
	STOCK_RELEASE( m_ptdTrophyTexture );
	m_bRenderTrophy = FALSE;
}

void CUIRadar::_initTrophyData()
{
	m_bRenderTrophy = FALSE;
	m_bShowTrophy = FALSE;
	m_tStartTime = 0;
	m_ubTrophyAlpha = 0;
	m_ubTrophyEffectAlpha = 0;
}

void CUIRadar::_showWinnerMsgBox()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if( pUIManager->DoesMessageBoxExist( MSGCMD_ROYALRUMBLE_WINNER_OUT ) )
		return;
	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( _S(5424, "대기실 이동"), UMBS_OK|UMBS_USE_TIMER, UI_RADAR, MSGCMD_ROYALRUMBLE_WINNER_OUT);
									
	MsgBoxInfo.AddString( _S(5426,	"우승을 축하드립니다."
									"쥬노존으로 이동합니다.") );

	MsgBoxInfo.SetMsgBoxTimer( 30, TRUE );			// 30 seconds.
	pUIManager->CreateMessageBox(MsgBoxInfo);
}

void CUIRadar::_showRemainTime()
{
	CTString strTitle = _S( 5579, "남은 시간" );
	CTString strTime;
	UIRect rcTitle;
	__int64 curTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();
	int LeftTime = (m_liEndTime - curTime) / 1000;
	strTime.PrintF( _s("%d : %d"), LeftTime/60, LeftTime%60 );

	int nInfoWidth;
#if defined G_THAI
		nInfoWidth = 19 - _pUIFontTexMgr->GetFontSpacing() + FindThaiLen(strTitle);
#else
		nInfoWidth = 19 - _pUIFontTexMgr->GetFontSpacing() + strTitle.Length() *
						( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );
#endif
	rcTitle.SetRect( m_nPosX + m_rcSandglassRR.Right - nInfoWidth,
					 m_nPosY + m_rcSandglassRR.Bottom + 3,
					 m_nPosX + m_rcSandglassRR.Right,
					 m_nPosY + m_rcSandglassRR.Bottom + 3 + 3 + 12 + 12 + 3 );

	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	// Set texture
	pDrawPort->InitTextureData( m_ptdBaseTexture );

	pDrawPort->AddTexture( rcTitle.Left, rcTitle.Top, rcTitle.Left + 7, rcTitle.Bottom,
										m_rtToolTipL.U0, m_rtToolTipL.V0, m_rtToolTipL.U1, m_rtToolTipL.V1,
										0xFFFFFFFF );
	pDrawPort->AddTexture( rcTitle.Left + 7, rcTitle.Top, rcTitle.Right - 7, rcTitle.Bottom,
										m_rtToolTipM.U0, m_rtToolTipM.V0, m_rtToolTipM.U1, m_rtToolTipM.V1,
										0xFFFFFFFF );
	pDrawPort->AddTexture( rcTitle.Right - 7, rcTitle.Top, rcTitle.Right, rcTitle.Bottom,
										m_rtToolTipR.U0, m_rtToolTipR.V0, m_rtToolTipR.U1, m_rtToolTipR.V1,
										0xFFFFFFFF );
	// Render all elements
	pDrawPort->FlushRenderingQueue();

	// Text in tool tip
	pDrawPort->PutTextExCX( strTitle, rcTitle.Left + rcTitle.GetWidth()/2, rcTitle.Top + 3, 0xFF8000FF );
	pDrawPort->PutTextExCX( strTime, rcTitle.Left + rcTitle.GetWidth()/2,rcTitle.Top + 3 + 12, 0xFF8000FF );

	// Flush all render text queue
	pDrawPort->EndTextEx();
}

void CUIRadar::ShowRoyalRumbleTrophy()
{
	m_bShowTrophy = FALSE;
	m_bRenderTrophy = TRUE;
	m_ubTrophyAlpha = 0;
	m_ubTrophyEffectAlpha = 0;
	m_tStartTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();
}


CUIRoyalRumbleIcon::CUIRoyalRumbleIcon() :
	m_ptdRoyalRumbleNumberTexture(NULL),
	m_iLeftCount(0),
	m_bPickTitle(FALSE),
	m_colShowColor(0xFFFFFFFF)		// white.
{
	
}

CUIRoyalRumbleIcon::~CUIRoyalRumbleIcon()
{
	STOCK_RELEASE( m_ptdRoyalRumbleNumberTexture );
}

void		CUIRoyalRumbleIcon::ShowRoyalRumbleIcon(BOOL bShow )
{ 
	CUIManager::getSingleton()->RearrangeOrder( UI_ROYALRUMBLE_ICON, bShow );	
}

void		CUIRoyalRumbleIcon::_setShowColor()
{
	if( m_iLeftCount < 10 )
	{
		m_colShowColor = 0xAA0000FF;
	}
	else
	{
		m_colShowColor = 0xFFFFFFFF;
	}
}

void		CUIRoyalRumbleIcon::Create(CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	CUIWindow::Create(pParentWnd, nX, nY, nWidth, nHeight);
	
	m_ptdBaseTexture = CreateTexture( CTString("Data\\Interface\\Map.tex") );
	FLOAT fTexWidth = m_ptdBaseTexture->GetPixWidth();
	FLOAT fTexHeight = m_ptdBaseTexture->GetPixHeight();


	m_rtBackground.SetUV( 8, 195, 120, 243, fTexWidth, fTexHeight );
	m_rcTitle.SetRect( 0, 13, 112, 48 );

	m_ptdRoyalRumbleNumberTexture = CreateTexture( CTString("Data\\Interface\\NamePopup.tex") );
}

void		CUIRoyalRumbleIcon::Render()
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	CDrawPort* pDrawPort = pUIManager->GetDrawPort();

	pDrawPort->InitTextureData( m_ptdBaseTexture );
	pDrawPort->AddTexture( m_nPosX, m_nPosY, m_nPosX+RRUI_WIDTH, m_nPosY+RRUI_HEIGHT,
										m_rtBackground.U0, m_rtBackground.V0, m_rtBackground.U1, m_rtBackground.V1,
										0xFFFFFFFF );
	pDrawPort->FlushRenderingQueue();
	pDrawPort->InitTextureData( m_ptdRoyalRumbleNumberTexture );
	pUIManager->DrawNumber( m_nPosX+50, m_nPosY+20, m_iLeftCount, m_colShowColor );
	pDrawPort->FlushRenderingQueue();

}

void		CUIRoyalRumbleIcon::ResetPosition(PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( RRUI_POS_X, RRUI_POS_Y );
}

void		CUIRoyalRumbleIcon::AdjustPosition(PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

WMSG_RESULT	CUIRoyalRumbleIcon::MouseMessage(MSG *pMsg )
{
	//WMSG_RESULT	wmsgResult;

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
			if( IsInside( nX, nY ) )
				CUIManager::getSingleton()->SetMouseCursorInsideUIs();

			int	ndX = nX - nOldX;
			int	ndY = nY - nOldY;

			// Move UI window.
			if( m_bPickTitle && ( pMsg->wParam & MK_LBUTTON ) )
			{
				nOldX = nX;	nOldY = nY;

				Move( ndX, ndY );

				return WMSG_SUCCESS;
			}
		}
		break;
		case WM_LBUTTONDOWN:
		{
			if( IsInside( nX, nY ) )
			{
				nOldX = nX;		nOldY = nY;

				if( IsInsideRect( nX, nY, m_rcTitle ) )
				{
					m_bPickTitle = TRUE;
				}
				
				CUIManager::getSingleton()->RearrangeOrder( UI_ROYALRUMBLE_ICON, TRUE );
				return WMSG_SUCCESS;
			}
		}
		break;
		case WM_LBUTTONUP:
		{
			m_bPickTitle = FALSE;
			CUIManager::getSingleton()->ResetHoldBtn();
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

// [2012/10/11 : Sora] 월드맵 개편
// 현재 배경지도를 그린다
void CUIRadar::RenderCurrentMap()
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	CDrawPort* pDrawPort = pUIManager->GetDrawPort();
	int	nX = m_nPosX + RADAR_CENTER_OFFSETX;
	int	nY = m_nPosY + RADAR_CENTER_OFFSETY;

	if( CUIManager::getSingleton()->GetMap()->IsMapExist( _pNetwork->MyCharacterInfo.zoneNo,_pNetwork->MyCharacterInfo.yLayer ) )
	{
		if( _pNetwork->MyCharacterInfo.zoneNo != m_nZone || _pNetwork->MyCharacterInfo.yLayer != m_nLayer )
		{
			if( m_ptdMapTexture )
			{
				_pTextureStock->Release(m_ptdMapTexture);	
			}
			// Create map texture
			CTString	strFileName;
			strFileName.PrintF( "Data\\Interface\\Map_World%d%d.tex",
								_pNetwork->MyCharacterInfo.zoneNo, _pNetwork->MyCharacterInfo.yLayer );
			m_ptdMapTexture = CreateTexture( strFileName );

			m_nZone = _pNetwork->MyCharacterInfo.zoneNo;
			m_nLayer = _pNetwork->MyCharacterInfo.yLayer;
		}


		FLOAT	fSin = Sin( -_pNetwork->MyCharacterInfo.r );
		FLOAT	fCos = Cos( -_pNetwork->MyCharacterInfo.r );
		FLOAT	fLeftCos = m_rcMyPoint.Left * fCos;
		FLOAT	fLeftSin = m_rcMyPoint.Left * fSin;
		FLOAT	fRightCos = m_rcMyPoint.Right * fCos;
		FLOAT	fRightSin = m_rcMyPoint.Right * fSin;
		FLOAT	fTopSin = m_rcMyPoint.Top * fSin;
		FLOAT	fTopCos = m_rcMyPoint.Top * fCos;
		FLOAT	fBottomSin = m_rcMyPoint.Bottom * fSin;
		FLOAT	fBottomCos = m_rcMyPoint.Bottom * fCos;

		FLOAT	fX1 = fLeftCos - fTopSin;
		FLOAT	fY1 = fLeftSin + fTopCos;
		FLOAT	fX2 = fLeftCos - fBottomSin;
		FLOAT	fY2 = fLeftSin + fBottomCos;
		FLOAT	fX3 = fRightCos - fBottomSin;
		FLOAT	fY3 = fRightSin + fBottomCos;
		FLOAT	fX4 = fRightCos - fTopSin;
		FLOAT	fY4 = fRightSin + fTopCos;


		FLOAT	fX = _pNetwork->MyCharacterInfo.x;
		FLOAT	fZ = _pNetwork->MyCharacterInfo.z ;

		pDrawPort->InitTextureData( m_ptdMapTexture );

		MapData* md;
		md = pUIManager->GetMap()->GetCurMapData( m_nZone );

		fX -= md->World.lOffsetX;	
		fZ -= md->World.lOffsetZ;

		// 지도에 대한 축척과 지도 Zoom비율
		fX = fX * md->World.fRatio / m_fZoomRate;	
		fZ = fZ * md->World.fRatio / m_fZoomRate;

		FLOAT widthRate = ((FLOAT)m_rcCompassIn.GetWidth() / 2.0f) / (FLOAT)m_ptdMapTexture->GetPixWidth();
		FLOAT heightRate = ((FLOAT)m_rcCompassIn.GetHeight() / 2.0f) / (FLOAT)m_ptdMapTexture->GetPixHeight();

		widthRate /= m_fZoomRate;
		heightRate /= m_fZoomRate;

		m_rtCompassIn.U0 = ( fX / ((FLOAT)m_ptdMapTexture->GetPixWidth() / m_fZoomRate) ) - widthRate;
		m_rtCompassIn.V0 = ( fZ / ((FLOAT)m_ptdMapTexture->GetPixHeight() / m_fZoomRate) ) - heightRate;

		m_rtCompassIn.U1 = ( fX / ((FLOAT)m_ptdMapTexture->GetPixWidth() / m_fZoomRate) ) + widthRate;
		m_rtCompassIn.V1 = ( fZ / ((FLOAT)m_ptdMapTexture->GetPixHeight() / m_fZoomRate) ) + heightRate;

		pDrawPort->AddTexture( nX + m_rcCompassIn.Left, nY + m_rcCompassIn.Top,
							   nX + m_rcCompassIn.Right, nY + m_rcCompassIn.Bottom,
							   m_rtCompassIn.U0, m_rtCompassIn.V0,
							   m_rtCompassIn.U1, m_rtCompassIn.V1,0xFFFFFFFF );

		pDrawPort->FlushRenderingQueue();
	}
	
}

void CUIRadar::AddIconName(int left, int top, int right, int bottom, CTString name)
{
	CIconName icon;
	icon.SetIconName(left - m_nPosX, top - m_nPosY, right - m_nPosX, bottom - m_nPosY, name);

	m_iconVec.push_back(icon);
}

BOOL CUIRadar::IsRadarUse()
{
	if( CZoneInfo::getSingleton()->GetZoneType( g_slZone ) == ZONE_SDUNGEON ||
		CZoneInfo::getSingleton()->GetZoneType( g_slZone ) == ZONE_DUNGEON )
	{
		return FALSE;
	}

	return TRUE;
}

void CUIRadar::RenderMyPosition()
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	CDrawPort* pDrawPort = pUIManager->GetDrawPort();

	int	nX = m_nPosX + RADAR_CENTER_OFFSETX;
	int	nY = m_nPosY + RADAR_CENTER_OFFSETY;

	FLOAT	fSin = Sin( -_pNetwork->MyCharacterInfo.r );
	FLOAT	fCos = Cos( -_pNetwork->MyCharacterInfo.r );
	FLOAT	fLeftCos = m_rcMyPoint.Left * fCos;
	FLOAT	fLeftSin = m_rcMyPoint.Left * fSin;
	FLOAT	fRightCos = m_rcMyPoint.Right * fCos;
	FLOAT	fRightSin = m_rcMyPoint.Right * fSin;
	FLOAT	fTopSin = m_rcMyPoint.Top * fSin;
	FLOAT	fTopCos = m_rcMyPoint.Top * fCos;
	FLOAT	fBottomSin = m_rcMyPoint.Bottom * fSin;
	FLOAT	fBottomCos = m_rcMyPoint.Bottom * fCos;

	FLOAT	fX1 = fLeftCos - fTopSin;
	FLOAT	fY1 = fLeftSin + fTopCos;
	FLOAT	fX2 = fLeftCos - fBottomSin;
	FLOAT	fY2 = fLeftSin + fBottomCos;
	FLOAT	fX3 = fRightCos - fBottomSin;
	FLOAT	fY3 = fRightSin + fBottomCos;
	FLOAT	fX4 = fRightCos - fTopSin;
	FLOAT	fY4 = fRightSin + fTopCos;


	FLOAT	fX = _pNetwork->MyCharacterInfo.x;
	FLOAT	fZ = _pNetwork->MyCharacterInfo.z ;


	pDrawPort->FlushRenderingQueue();
	pDrawPort->InitTextureData( m_ptdBaseTexture );


	pDrawPort->AddTexture( nX + fX1, nY + fY1, m_rtMyPoint.U0, m_rtMyPoint.V0, 0xFFFFFFFF,
										nX + fX2, nY + fY2, m_rtMyPoint.U0, m_rtMyPoint.V1, 0xFFFFFFFF,
										nX + fX3, nY + fY3, m_rtMyPoint.U1, m_rtMyPoint.V1, 0xFFFFFFFF,
										nX + fX4, nY + fY4, m_rtMyPoint.U1, m_rtMyPoint.V0, 0xFFFFFFFF );

	pDrawPort->FlushRenderingQueue();
}

void CUIRadar::RenderGPS(CDrawPort* pDraw)
{
	if (GAMEDATAMGR()->GetGPS()->IsPosition() == false)
		return;

	UpdateClient::GPSTargetMoveInfo* pInfo = GAMEDATAMGR()->GetGPS()->getInfo();

	MapData* md;
	CTString strTemp;
	md = UIMGR()->GetMap()->GetCurMapData( m_nZone );
	FLOAT	fSqrDist = RADAR_VIEW_DISTANCE * RADAR_VIEW_DISTANCE / m_fZoomRate;
	FLOAT	fRatio = md->World.fRatio * m_fZoomRate;
	FLOAT			fXpc = _pNetwork->MyCharacterInfo.x;
	FLOAT			fZpc = _pNetwork->MyCharacterInfo.z;

	float fX, fZ;
	int nX, nY;

	int		nCX = m_nPosX + RADAR_CENTER_OFFSETX;
	int		nCY = m_nPosY + RADAR_CENTER_OFFSETY;

	fX = pInfo->x - fXpc;
	fZ = pInfo->z - fZpc;

	fX *= fRatio;
	fZ *= fRatio;

	if (fX > DEF_SHOW_DISTANCE || fX < -DEF_SHOW_DISTANCE || fZ > DEF_SHOW_DISTANCE || fZ < -DEF_SHOW_DISTANCE)
		return;

	nX = nCX + fX;
	nY = nCY + fZ;

	pDraw->InitTextureData(m_ptdMapObjTexture);

	pDraw->AddTexture(m_rcGPS.Left + nX, m_rcGPS.Top + nY, m_rcGPS.Right + nX, m_rcGPS.Bottom + nY, m_uvGPS, DEF_UI_COLOR_WHITE);

	pDraw->FlushRenderingQueue();
}

void CUIRadar::OnExpressNotice()
{
	m_bNotice = true;
	m_bNoticeRenderFlag = true;
	m_nStartTime = timeGetTime();	
}
void CUIRadar::OffExpressNotice()
{
	m_bNotice = false;
	m_bNoticeRenderFlag = false;
}

void CUIRadar::FocusLeave()
{
	m_PetStashIcon.SetBtnState(UBS_IDLE);
	ShowToolTip( FALSE );
}

void CUIRadar::SetPremiumCharBenefit( bool bPremiumChar )
{
	if ( bPremiumChar == true)
		m_PremiumCharIcon.SetBtnState(UBS_IDLE);
	else
		m_PremiumCharIcon.SetBtnState(UBS_DISABLE);
}


