#include "stdh.h"
#include <vector>
#include <deque>

#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Interface/UIMap.h>
#include <Engine/Interface/UISiegeWarfareDoc.h>
#include <Engine/Contents/Base/UIPartyNew.h>
#include <Engine/Contents/Base/Party.h>
#include <Engine/Interface/UINpcScroll.h>
#include <Engine/Interface/UIRadar.h>
#include <Engine/Interface/UIGuild.h>
#include <Engine/Interface/UISiegeWarfareNew.h>
#include <Engine/Base/KeyNames.h>
#include <Engine/Help/DefineHelp.h>
#include <Engine/Contents/Base/UIMapOption.h>
#include <Engine/GameDataManager/GameDataManager.h>
#include <Engine/Contents/function/gps.h>
#include <Common/Packet/ptype_gps.h>
#include <Engine/Entities/InternalClasses.h>
#include <Engine/Help/Util_Help.h>

// const value				
#define MAX_SIGNAL					3	// 화면에 표시되는 최대 Signal 수 

// Position offset of controls
#define	MAP_CLOSE_X					32
#define	MAP_CLOSE_Y					5
#define	MAP_OPACITY_X				227 + 20
#define	MAP_OPACITY_Y				26
#define	MAP_ZOOM_Y					38
#define	MAP_COMBO_X					145 + 20
#define	MAP_COMBO_Y					27
#define	MAP_SLIDEINFO_X				232	+ 20
#define	MAP_SLIDEINFO_Y				29
#define MAP_SIGNAL_X				MAP_OPACITY_X - 20
#define	MAP_SIGNAL_Y				26

// [2012/10/11 : Sora] 월드맵 개편
#define ZONE_WORLD_MAP				99
int	zoneNumList[] = {0, 4, 7, 15, 23, 32, 39, 40, 42, 0,};

extern INDEX	g_iCountry;

bool CompareZ( CastleData first, CastleData second )
{
	return ( first.fZ < second.fZ );
}

CTextureData*	ptdTopUITexture = NULL;
CTextureData*	ptdButtonTexture = NULL;

//extern INDEX g_iCountry;

extern INDEX g_iZoomInMap;
extern INDEX g_iOpacityInMap;
extern INDEX g_iXPosInMap;
extern INDEX g_iYPosInMap;

// ----------------------------------------------------------------------------
// Name : CUIMap()
// Desc : Constructor
// ----------------------------------------------------------------------------
CUIMap::CUIMap()
	: _bDetail(false)
	, m_eMapState(eMS_NONE)
	, m_penPlayerEntity(NULL)
{
	m_bShowWorldMap = TRUE;
	m_bShowFrame = TRUE;
	m_bFrameBlending = FALSE;
	m_bMapBlendingForDetail = FALSE;
	m_bMapBlendingForLayer = FALSE;
	m_bCanScrollMapWorld = FALSE;
	m_bCanScrollMapDetail = FALSE;
	m_llFrameBlendTime = 0;
	m_llFrameBlendElapsedTime = 0;
	m_llMapBlendTimeForDetail = 0;
	m_llMapBlendTimeForLayer = 0;
	m_llMapBlendElapsedTimeForLayer = 0;
	m_llMapBlendTimeForCharPos = 0;

	m_sbCurLayer = 0;
	m_nCurMapData = -1;
	m_nCurDetail = -1;
	m_fZoomWorld = 1.0f;
	m_fZoomDetail = 1.0f;
	m_fMapOpacity = 1.0f;
	m_nResizeWidthOffset = 0;
	m_nResizeHeightOffset = 0;
	m_bShowOpacitySlideInfo = FALSE;
	m_bShowZoomSlideInfo = FALSE;
	m_strOpacitySlideInfo = CTString( "" );
	m_strZoomSlideInfo = CTString( "" );
	m_bShowToolTip = FALSE;
	m_strToolTip = CTString( "" );

	m_ptdWorldTex[0] = NULL;
	m_ptdWorldTex[1] = NULL;
	m_ptdDetailTex = NULL;

	m_bSignalBtnOn = FALSE;
	m_bInsideMouse = FALSE;
	m_bDetailMapClick = FALSE;
	// revision map size. [10/20/2009 rumist]
	m_fMapSizeRevision = 1.0f;	// default size is 1.0f. if map size is default map size, it's current data used.
	m_bHighlight = FALSE;
	m_nNpcHighlightPosX = 0;
	m_nNpcHighlightPosY = 0;
	m_nNpcHighlightIdx = -1;
}

// ----------------------------------------------------------------------------
// Name : ~CUIMap()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUIMap::~CUIMap()
{
	Destroy();
}

// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUIMap::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	CUIWindow::Create(pParentWnd, nX, nY, nWidth, nHeight);

	// Region of each part
	m_rcTitle.SetRect( 0, 0, 512, 22 );
	m_rcResizeLeft.SetRect( 0, 22, 10, m_nHeight - 10 );
	m_rcResizeRight.SetRect( m_nWidth - 10, 22, m_nWidth, m_nHeight - 10 );
	m_rcResizeBottomL.SetRect( 0, m_nHeight - 10, 10, m_nHeight );
	m_rcResizeBottomM.SetRect( 10, m_nHeight - 10, m_nWidth - 10, m_nHeight );
	m_rcResizeBottomR.SetRect( m_nWidth - 10, m_nHeight - 10, m_nWidth, m_nHeight );
	m_rcPC.SetRect( -17, -21, 18, 21 );
	m_rcNPC.SetRect( -11, -11, 11, 11 );

	m_rcHelpNpc.SetRect( -6, -9, 6, 8);
	// connie [2009/9/18] - NPC 찾기
	m_rcHelpMob.SetRect(-6, -9, 6, 8);

	m_rcParty.SetRect( -5, -5, 6, 6 );
	m_rcCompass.SetRect( 5, -97, 80, -5 );
#ifdef NEW_WORLD_MAP		// [2012/10/11 : Sora] 월드맵 개편
	m_rcSubZone[VILLAGE].SetRect( -17, -18, 17, 18 );
#else
	m_rcSubZone[VILLAGE].SetRect( -16, -18, 16, 18 );
#endif
	m_rcSubZone[DUNGEON].SetRect( -16, -18, 16, 18 );
	m_rcSubZone[CHARGE_PRIVATE].SetRect( -13, -23, 13, 23 );
	m_rcSubZone[CHARGE_PUBLIC].SetRect( -13, -23, 13, 23 );
	m_rcSubZone[MINE_PRIVATE].SetRect( -17, -20, 17, 20 );
	m_rcSubZone[MINE_PUBLIC].SetRect( -17, -20, 17, 20 );
	m_rcSubZone[GATHER_PRIVATE].SetRect( -17, -19, 17, 19 );
	m_rcSubZone[GATHER_PUBLIC].SetRect( -17, -19, 17, 19 );
	m_rcSubZone[CASTLE_GATE].SetRect( -9, -9, 9, 9 );//!!

	// 공성 관련 추가 +
	m_rcCastle[CASTLE_LORD].SetRect( -9, -9, 9, 9 );//!!
	m_rcCastle[CASTLE_REGEN].SetRect( -9, -9, 9, 9 );//!!
	m_rcCastle[CASTLE_TOWER].SetRect( -9, -9, 9, 9 );//!!
	m_rcCastle[CASTLE_SIGNAL].SetRect( -17, -17, 16, 16 );//!!
	
	m_rcViewportWorld.SetRect( 0, 0, MAP_FRAME_BASE_WIDTH, MAP_FRAME_BASE_HEIGHT );
	m_rcViewportDetail.SetRect( 0, 0, MAP_FRAME_BASE_WIDTH, MAP_FRAME_BASE_HEIGHT );

	// Create map frame texture
	m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\Map.tex" ) );
	FLOAT	fTexWidth = m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight = m_ptdBaseTexture->GetPixHeight();

	// UV Coordinate of each part
	// Background
	m_rtBackUL.SetUV( 0, 0, 34, 22, fTexWidth, fTexHeight );
	m_rtBackUM.SetUV( 37, 0, 48, 22, fTexWidth, fTexHeight );
	m_rtBackUR.SetUV( 51, 0, 84, 22, fTexWidth, fTexHeight );
	m_rtBackML.SetUV( 0, 25, 34, 30, fTexWidth, fTexHeight );
	m_rtBackMM.SetUV( 37, 25, 48, 30, fTexWidth, fTexHeight );
	m_rtBackMR.SetUV( 51, 25, 84, 30, fTexWidth, fTexHeight );
	m_rtBackLL.SetUV( 0, 33, 34, 48, fTexWidth, fTexHeight );
	m_rtBackLM.SetUV( 37, 33, 48, 48, fTexWidth, fTexHeight );
	m_rtBackLR.SetUV( 51, 33, 84, 48, fTexWidth, fTexHeight );
	m_rtBackListBar.SetUV( 89, 2, 102, 16, fTexWidth, fTexHeight );
	m_rtPC.SetUV( 231, 140, 272, 182, fTexWidth, fTexHeight );

	m_rtHelpMob.SetUV( 127, 64, 140, 82, fTexWidth, fTexHeight );
	m_rtHelpNpc.SetUV( 127, 64, 140, 82, fTexWidth, fTexHeight );

	m_rtParty.SetUV( 108, 67, 119, 78, fTexWidth, fTexHeight );
	m_rtCompass.SetUV( 179, 42, 254, 124, fTexWidth, fTexHeight );
#ifdef NEW_WORLD_MAP		// [2012/10/11 : Sora] 월드맵 개편
	m_rtSubZone[VILLAGE].SetUV( 217, 205, 251, 241, fTexWidth, fTexHeight );
#else
	m_rtSubZone[VILLAGE].SetUV( 67, 91, 99, 127, fTexWidth, fTexHeight );
#endif
	m_rtSubZone[DUNGEON].SetUV( 67, 91, 99, 127, fTexWidth, fTexHeight );
	m_rtSubZone[CHARGE_PRIVATE].SetUV( 108, 82, 134, 128, fTexWidth, fTexHeight );
	m_rtSubZone[CHARGE_PUBLIC].SetUV( 108, 82, 134, 128, fTexWidth, fTexHeight );
	m_rtSubZone[MINE_PRIVATE].SetUV( 135, 88, 169, 128, fTexWidth, fTexHeight );
	m_rtSubZone[MINE_PUBLIC].SetUV( 135, 88, 169, 128, fTexWidth, fTexHeight );
	m_rtSubZone[GATHER_PRIVATE].SetUV( 131, 6, 165, 44, fTexWidth, fTexHeight );
	m_rtSubZone[GATHER_PUBLIC].SetUV( 131, 6, 165, 44, fTexWidth, fTexHeight );
	m_rtSubZone[CASTLE_GATE].SetUV( 170, 10, 188, 28, fTexWidth, fTexHeight );//!!

	m_rtCastle[CASTLE_LORD].SetUV( 208, 4, 226, 24, fTexWidth, fTexHeight );//!!
	m_rtCastle[CASTLE_REGEN].SetUV( 230, 4, 248, 24, fTexWidth, fTexHeight );//!!
	m_rtCastle[CASTLE_TOWER].SetUV( 189, 4, 207, 24, fTexWidth, fTexHeight );//!!
	m_rtCastle[CASTLE_SIGNAL].SetUV( 143, 46, 176, 79, fTexWidth, fTexHeight );//!!

	// Tool tip
	m_rtToolTipL.SetUV( 108, 6, 115, 28, fTexWidth, fTexHeight );
	m_rtToolTipM.SetUV( 118, 6, 120, 28, fTexWidth, fTexHeight );
	m_rtToolTipR.SetUV( 123, 6, 130, 28, fTexWidth, fTexHeight );

	// Close button
	m_btnClose.Create( this, CTString( "" ), 486, 5, 14, 14 );
	m_btnClose.SetUV( UBS_IDLE, 31, 97, 45, 111, fTexWidth, fTexHeight );
	m_btnClose.SetUV( UBS_CLICK, 46, 97, 60, 111, fTexWidth, fTexHeight );
	m_btnClose.CopyUV( UBS_IDLE, UBS_ON );
	m_btnClose.CopyUV( UBS_IDLE, UBS_DISABLE );

	m_btnSignal.Create( this, CTString( "" ), m_nWidth - MAP_SIGNAL_X, MAP_SIGNAL_Y, 19, 19 );
	m_btnSignal.SetUV( UBS_IDLE, 108, 29, 128, 48, fTexWidth, fTexHeight );
	m_btnSignal.SetUV( UBS_CLICK, 0, 96, 19, 115, fTexWidth, fTexHeight );
	m_btnSignal.CopyUV( UBS_IDLE, UBS_ON );
	m_btnSignal.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Map combo box
	m_cmbMap.Create( this, 373, 27, 120, 15, 126-20, 1, 13, 13,
						8, _pUIFontTexMgr->GetFontHeight() + 4, 4, 4 );
	m_cmbMap.SetBackUV( 0, 49, 140, 64, fTexWidth, fTexHeight );
	// updown texture position [8/31/2010 rumist]
// 	m_cmbMap.SetDownBtnUV( 31, 112, 44, 125, fTexWidth, fTexHeight );
// 	m_cmbMap.SetUpBtnUV( 45, 112, 58, 125, fTexWidth, fTexHeight );	
	m_cmbMap.SetDownBtnUV( 45, 112, 58, 125, fTexWidth, fTexHeight );
	m_cmbMap.SetUpBtnUV( 31, 112, 44, 125, fTexWidth, fTexHeight );	
	m_cmbMap.SetDropListUV( 0, 49, 140, 64, fTexWidth, fTexHeight );
	m_cmbMap.CreateScroll( TRUE, 9, 7, 0, 0, 10 );
	// Up button
	m_cmbMap.SetScrollUpUV( UBS_IDLE, 41, 67, 50, 74, fTexWidth, fTexHeight );
	m_cmbMap.SetScrollUpUV( UBS_CLICK, 51, 67, 60, 74, fTexWidth, fTexHeight );
	m_cmbMap.CopyScrollUpUV( UBS_IDLE, UBS_ON );
	m_cmbMap.CopyScrollUpUV( UBS_IDLE, UBS_DISABLE );
	// Bar button
	m_cmbMap.SetScrollBarTopUV( 31, 67, 40, 77, fTexWidth, fTexHeight );
	m_cmbMap.SetScrollBarMiddleUV( 31, 78, 40, 80, fTexWidth, fTexHeight );
	m_cmbMap.SetScrollBarBottomUV( 31, 81, 40, 91, fTexWidth, fTexHeight );
	// Down button
	m_cmbMap.SetScrollDownUV( UBS_IDLE, 41, 76, 50, 83, fTexWidth, fTexHeight );
	m_cmbMap.SetScrollDownUV( UBS_CLICK, 51, 76, 60, 83, fTexWidth, fTexHeight );
	m_cmbMap.CopyScrollDownUV( UBS_IDLE, UBS_ON );
	m_cmbMap.CopyScrollDownUV( UBS_IDLE, UBS_DISABLE );

	// Opacity slide bar
	m_slbOpacity.Create( this, 291, 26, 76, 5, 10, 10, 75, 76, 0, 75 );
	m_slbOpacity.SetBackgroundUV( 108, 0, 184, 5, fTexWidth, fTexHeight );
	m_slbOpacity.SetBarUV( 41, 84, 51, 94, fTexWidth, fTexHeight );

	// Zooming world map slide bar
	m_slbZoomWorld.Create( this, 291, 38, 76, 5, 10, 10, 12, 13, 0, 12 );
	m_slbZoomWorld.SetBackgroundUV( 108, 0, 184, 5, fTexWidth, fTexHeight );
	m_slbZoomWorld.SetBarUV( 41, 84, 51, 94, fTexWidth, fTexHeight );

	// Zooming detail map slide bar
	m_slbZoomDetail.Create( this, 291, 38, 76, 5, 10, 10, 12, 13, 0, 12 );
	m_slbZoomDetail.Create( this, 291, 38, 76, 5, 10, 10, 12, 13, 0, 12 );
	m_slbZoomDetail.SetBackgroundUV( 108, 0, 184, 5, fTexWidth, fTexHeight );
	m_slbZoomDetail.SetBarUV( 41, 84, 51, 94, fTexWidth, fTexHeight );

	m_rcGPS.SetRect(-13, -13, 13, 13);
	m_uvGPS.SetUV(210, 258, 236, 284, fTexWidth, fTexHeight);

	m_rcRelic.SetRect( -25, -14, 25, 14);
	m_uvRelic.SetUV( 196, 287, 246, 314, fTexWidth, fTexHeight );

	ptdTopUITexture = CreateTexture( CTString( "Data\\Interface\\TopUI.tex" ) );
	fTexWidth = ptdTopUITexture->GetPixWidth();
	fTexHeight = ptdTopUITexture->GetPixHeight();
//////////////////////////////////////////////////////////////////////////
// [sora] RAID_SYSTEM 원정대원 표시
//////////////////////////////////////////////////////////////////////////
//	m_rcExpedition.SetRect( -6, -6, 5, 5 );
	m_rcExpedition.SetRect( -5, -5, 6, 6 );

	int nPosX = 646;
	for(int i=0; i<EXPEDITION_GROUP_MAX; i++)
	{
//		m_rtExpedition[i].SetUV( 108, 67, 119, 78, fTexWidth, fTexHeight );
		m_rtExpedition[i].SetUV( nPosX, 347, nPosX + 11, 358, fTexWidth, fTexHeight );
		nPosX += 15;
	}

#ifdef NEW_WORLD_MAP		// [2012/10/11 : Sora] 월드맵 개편
	m_cmbMap.SetEnable(FALSE);
	m_slbZoomWorld.SetEnable(FALSE);
	m_slbZoomDetail.SetEnable(FALSE);

	m_slbOpacity.SetPos(291, 32);

	ptdButtonTexture = CreateTexture( CTString( "Data\\Interface\\CommonBtn.tex" ));
	fTexWidth = ptdButtonTexture->GetPixWidth();
	fTexHeight = ptdButtonTexture->GetPixHeight();
	m_btnGoTop.Create( this, _S( 5784, "상위로 가기"), m_nWidth - 98, 26, 83, 23 );
	m_btnGoTop.SetUV( UBS_IDLE, 112, 0, 112+71, 23, fTexWidth, fTexHeight );
	m_btnGoTop.SetUV( UBS_CLICK, 185, 0, 185+71, 23, fTexWidth, fTexHeight );
	m_btnGoTop.CopyUV( UBS_IDLE, UBS_ON );
	m_btnGoTop.CopyUV( UBS_IDLE, UBS_DISABLE );
#endif


#ifndef NEW_WORLD_MAP		// [2012/10/11 : Sora] 월드맵 개편
	CZoneInfo::getSingleton()->SetZoneName( 17, _S(3050, "에게하") ); // 에게하 던전
#endif	// NEW_WORLD_MAP

	// Read map data
	ReadMapData();
}

// ----------------------------------------------------------------------------
// Name : Destroy()
// Desc :
// ----------------------------------------------------------------------------
void CUIMap::Destroy()
{
	CUIWindow::Destroy();

	STOCK_RELEASE( m_ptdWorldTex[0] );
	STOCK_RELEASE( m_ptdWorldTex[1] );
	STOCK_RELEASE( m_ptdDetailTex );
	STOCK_RELEASE(ptdTopUITexture);
	STOCK_RELEASE(ptdButtonTexture);
}

// ----------------------------------------------------------------------------
// Name : ResetPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIMap::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
	SetSize( MAP_FRAME_WIDTH, MAP_FRAME_HEIGHT );

	m_btnClose.SetPosX( m_nWidth - MAP_CLOSE_X );
#ifdef NEW_WORLD_MAP		// [2012/10/11 : Sora] 월드맵 개편
	m_btnGoTop.SetPosX( m_nWidth - 98 );
	m_slbOpacity.SetPosX( 291 );
#else
	m_slbOpacity.SetPosX( 15 );
#endif
	m_slbZoomWorld.SetPosX( 15 );
	m_slbZoomDetail.SetPosX( 15 );
	m_cmbMap.SetPosX( 97 );
	m_btnSignal.SetPosX( 103 );
}

void CUIMap::ResetSavePosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	ResetPosition(pixMinI, pixMinJ, pixMaxI, pixMaxJ);
	g_iXPosInMap = GetPosX();
	g_iYPosInMap = GetPosY();
}

// ----------------------------------------------------------------------------
// Name : AdjustPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIMap::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

// ----------------------------------------------------------------------------
// Name : ReadMapData()
// Desc :
// ----------------------------------------------------------------------------
void CUIMap::ReadMapData()
{
	if( m_vecMapData.size() > 0  )
		return;

	CTFileName	fnmMapData;
	CTString	strFullPath = _fnmApplicationPath.FileDir();
	fnmMapData = strFullPath + "data\\Map.dta";

	CTFileStream	fsMapData;
	fsMapData.Open_t( fnmMapData );

	CTString	strZoneName;
	int			nZoneCount;
	LONG		lOffsetX;
	LONG		lOffsetZ;

	fsMapData >> nZoneCount;

	int	i, j;
	for( i = 0; i < nZoneCount; i++ )
	{
		MapData	tempMapData;
		int		nIndex, nLeft, nTop, nRight, nBottom;
		FLOAT	fRatio, fX, fZ;
		SBYTE	sbLayer;
		fsMapData >> nIndex;
		fsMapData >> sbLayer;
		fsMapData >> nLeft;
		fsMapData >> nTop;
		fsMapData >> nRight;
		fsMapData >> nBottom;
		fsMapData >> fRatio;
		fsMapData >> lOffsetX;
		fsMapData >> lOffsetZ;

		tempMapData.World.nZoneIndex = nIndex;
		tempMapData.World.fRatio = fRatio;
		tempMapData.World.rcMapSize.SetRect( nLeft, nTop, nRight, nBottom );
		tempMapData.World.sbLayer = sbLayer;
		tempMapData.World.lOffsetX = lOffsetX;
		tempMapData.World.lOffsetZ = lOffsetZ;

		SBYTE	sbDetailCount;
		fsMapData >> sbDetailCount;

		for( j = 0; j < sbDetailCount; j++ )
		{
			DetailMapData	tempDetail;
			int				nSelLeft, nSelTop, nSelRight, nSelBottom;

			fsMapData >> nSelLeft;
			fsMapData >> nSelTop;
			fsMapData >> nSelRight;
			fsMapData >> nSelBottom;
			fsMapData >> nLeft;
			fsMapData >> nTop;
			fsMapData >> nRight;
			fsMapData >> nBottom;
			fsMapData >> fX;
			fsMapData >> fZ;
			fsMapData >> fRatio;

			tempDetail.rcSelRegion.SetRect( nSelLeft, nSelTop, nSelRight, nSelBottom );
			tempDetail.rcMapSize.SetRect( nLeft, nTop, nRight, nBottom );
			tempDetail.fOffsetX = fX;
			tempDetail.fOffsetZ = fZ;
			tempDetail.fRatio = fRatio;
			tempMapData.vecDetail.push_back( tempDetail );
		}

		SBYTE	sbSubZoneCount;
		fsMapData >> sbSubZoneCount;

		for( j = 0; j < sbSubZoneCount; j++ )
		{
			SubZoneData	tempSubZone;
			SBYTE		sbType;

			fsMapData >> nIndex;
			fsMapData >> fX;
			fsMapData >> fZ;
			fsMapData >> sbType;

			tempSubZone.nSubZoneIndex = nIndex;
			tempSubZone.fX = fX;
			tempSubZone.fZ = fZ;
			tempSubZone.sbType = sbType;

			tempMapData.vecSubZone.push_back( tempSubZone );
		}

		int	nNpcCount;
		fsMapData >> nNpcCount;
		
		LONG nYLayer;

		for( j = 0; j < nNpcCount; j++ )
		{
			NpcData	tempNpc;
			
			fsMapData >> nIndex;
			fsMapData >> nYLayer;
			fsMapData >> fX;
			fsMapData >> fZ;

			tempNpc.nIndex = nIndex;
			tempNpc.nYLayer = nYLayer;	
			tempNpc.fX = fX;
			tempNpc.fZ = fZ;
			
			tempMapData.vecNpc.push_back( tempNpc );
		}

#ifdef NEW_WORLD_MAP		// [2012/10/11 : Sora] 월드맵 개편
		if( tempMapData.World.nZoneIndex != ZONE_WORLD_MAP )
		{
			strZoneName = GetZoneName(tempMapData.World.nZoneIndex, tempMapData.World.sbLayer);
		}
		else
		{
			strZoneName = _S( 5797, "월드맵");
		}

#else
		strZoneName = GetZoneName(tempMapData.World.nZoneIndex, tempMapData.World.sbLayer);
#endif
			
		tempMapData.World.strName = strZoneName;

		m_vecMapData.push_back( tempMapData );

	
		if( !IsCastleMap( i ) )	
		m_cmbMap.AddString( strZoneName );
	}
}

// ----------------------------------------------------------------------------
// Name : ShowToolTip()
// Desc :
// ----------------------------------------------------------------------------
void CUIMap::ShowToolTip( BOOL bShow, CTString &strName, int nX, int nY )
{
	if( !bShow )
	{
		m_bShowToolTip = FALSE;
		m_strToolTip.Clear();
		return;
	}

	m_bShowToolTip = TRUE;

	if( m_strToolTip.IsEqualCaseSensitive( strName ) == TRUE )
		return;

	if( m_bShowWorldMap )
	{
		UIRect	rcTempWorld = m_rcMapWorld;
		rcTempWorld.Scale( m_fZoomWorld );
		// revision map size. [10/20/2009 rumist]
		// 이미 nX, nY는 m_fMapSizeRevision를 반영한 위치값이므로 m_fMapSizeRevision를 반영하지 않는다.
		nX *= m_fZoomWorld;
		nY *= m_fZoomWorld;
		nX += m_rcViewportWorld.Left - rcTempWorld.Left + m_nPosX + MAP_SX;
		nY += m_rcViewportWorld.Top - rcTempWorld.Top + m_nPosY + MAP_SY;
	}
	else
	{
		UIRect	rcTempDetail = m_rcMapDetail;
		rcTempDetail.Scale( m_fZoomDetail );
		
		nX *= m_fZoomDetail;
		nY *= m_fZoomDetail;
		nX += m_rcViewportDetail.Left - rcTempDetail.Left + m_nPosX + MAP_SX;
		nY += m_rcViewportDetail.Top - rcTempDetail.Top + m_nPosY + MAP_SY;
	}
	
	m_strToolTip.PrintF( "%s", strName );
	
	CUIManager* pUIManager = CUIManager::getSingleton();
	int nInfoWidth;

	//if(g_iCountry == THAILAND) 
#if defined G_THAI
	{
		nInfoWidth = 19 - _pUIFontTexMgr->GetFontSpacing() + FindThaiLen(m_strToolTip);				
	} 
	//else
#else
	{
		//if(g_iCountry == RUSSIA)
#if defined G_RUSSIA
		extern CFontData *_pfdDefaultFont;
		nInfoWidth = UTIL_HELP()->GetNoFixedWidth(_pfdDefaultFont, m_strToolTip.str_String) + 16;
		//else
#else
		{
			nInfoWidth = 19 - _pUIFontTexMgr->GetFontSpacing() + m_strToolTip.Length() *
				( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );
		}
#endif
		
	}
#endif
	int	nInfoHeight = 22;
	
	nX -= nInfoWidth / 2;
	
	if( nX < pUIManager->GetMinI() )
		nX = pUIManager->GetMinI();
	else if( nX + nInfoWidth > pUIManager->GetMaxI() )
		nX = pUIManager->GetMaxI() - nInfoWidth;
	
	if( nY - nInfoHeight < pUIManager->GetMinJ() )
		nY = pUIManager->GetMinJ() + nInfoHeight;
	else if( nY > pUIManager->GetMaxJ() )
		nY = pUIManager->GetMaxJ();
	
	m_rcToolTip.SetRect( nX, nY - nInfoHeight, nX + nInfoWidth, nY );
}

// ----------------------------------------------------------------------------
// Name : FindMapData()
// Desc :
// ----------------------------------------------------------------------------
int	CUIMap::FindMapData( int nZoneIndex, SBYTE sbLayer )
{
	for( int i = 0; i < m_vecMapData.size(); i++ )
	{
		if( m_vecMapData[i].World.nZoneIndex == nZoneIndex &&
			m_vecMapData[i].World.sbLayer == sbLayer )
			return i;
	}

	return -1;
}

// ----------------------------------------------------------------------------
// Name : MakeBaseMapSize()
// Desc :
// ----------------------------------------------------------------------------
void CUIMap::MakeBaseMapSize( BOOL bWorldMap )
{
	if( bWorldMap )
	{
		m_nMapWorldBaseWidth = MAP_FRAME_BASE_WIDTH;
		m_nMapWorldBaseHeight = MAP_FRAME_BASE_HEIGHT;

		if( m_nCurMapData != -1 )
		{
			m_bCanScrollMapWorld = FALSE;
			UIRect	rcTempWorld = m_vecMapData[m_nCurMapData].World.rcMapSize;
			if( rcTempWorld.GetWidth() <= m_nMapWorldBaseWidth )
				m_nMapWorldBaseWidth = rcTempWorld.GetWidth();
			else
				m_bCanScrollMapWorld = TRUE;

			if( rcTempWorld.GetHeight() <= m_nMapWorldBaseHeight )
				m_nMapWorldBaseHeight = rcTempWorld.GetHeight();
			else
				m_bCanScrollMapWorld = TRUE;
		}
	}
	else
	{
		m_nMapDetailBaseWidth = MAP_FRAME_BASE_WIDTH;
		m_nMapDetailBaseHeight = MAP_FRAME_BASE_HEIGHT;

		if( m_nCurMapData != -1 && m_nCurDetail != -1 )
		{
			if ( m_vecMapData.size() <= m_nCurMapData || m_vecMapData[m_nCurMapData].vecDetail.size() <= m_nCurDetail )
			{
				return;
			}
			m_bCanScrollMapDetail = FALSE;
			UIRect	rcTempDetail = m_vecMapData[m_nCurMapData].vecDetail[m_nCurDetail].rcMapSize;
			if( rcTempDetail.GetWidth() <= m_nMapDetailBaseWidth )
				m_nMapDetailBaseWidth = rcTempDetail.GetWidth();
			else
				m_bCanScrollMapDetail = TRUE;

			if( rcTempDetail.GetHeight() <= m_nMapDetailBaseHeight )
				m_nMapDetailBaseHeight = rcTempDetail.GetHeight();
			else
				m_bCanScrollMapDetail = TRUE;
		}
	}
}

// ----------------------------------------------------------------------------
// Name : SetCurrentWorldMap()
// Desc :
// ----------------------------------------------------------------------------
void CUIMap::SetCurrentWorldMap( int nWorld, SBYTE sbLayer )
{
	if( m_nCurMapData != -1 && m_vecMapData[m_nCurMapData].World.nZoneIndex == nWorld && m_vecMapData[m_nCurMapData].World.sbLayer == sbLayer )
		return;

	// ********************************************************* //
	//                      임시 추가                            //
	_pNetwork->MyCharacterInfo.fWaterHeight = -100.0f;
	switch( nWorld )
	{
	case 0:
		// 주노 리폼으로 물 지역 처리를 제거함( 높이가 일정하지 않음 )
		_pNetwork->MyCharacterInfo.fWaterHeight = -1.0f;
		break;
	case 4:
		_pNetwork->MyCharacterInfo.fWaterHeight = -1.0f;	// WSS_DRATAN_SIEGEWARFARE 2007/10/16 공성지역 저지대로 인해 수정 99.5f -> -1.0f
		break;
	}
	// ********************************************************* //

	int	nMapDataIndex = FindMapData( nWorld, sbLayer );
	if( nMapDataIndex == -1 && m_nCurMapData == -1 )	// If map window is opened first time and map is not exist, default map(start zone) is set...
		nMapDataIndex = 0;
	if( nMapDataIndex == -1 )							// No map
		return;

	m_bShowWorldMap = TRUE;
	m_sbCurLayer = sbLayer;
	m_nCurMapData = nMapDataIndex;
	//공성 비공성 지역 표시
	m_cmbMap.SetCurSel( (!IsPossibleCastleMap()&&m_nCurMapData>=4) ? m_nCurMapData-1:m_nCurMapData );
	nWorld = m_vecMapData[m_nCurMapData].World.nZoneIndex;

	MakeBaseMapSize( TRUE );
	int	nCX = _pNetwork->MyCharacterInfo.x * m_vecMapData[m_nCurMapData].World.fRatio / m_fZoomWorld;
	int	nCY = _pNetwork->MyCharacterInfo.z * m_vecMapData[m_nCurMapData].World.fRatio / m_fZoomWorld;
	int	nHalfW = m_nMapWorldBaseWidth / m_fZoomWorld * 0.5f;
	int	nHalfH = m_nMapWorldBaseHeight / m_fZoomWorld * 0.5f;
#ifdef NEW_WORLD_MAP		// [2012/10/11 : Sora] 월드맵 개편
	m_rcMapWorld.SetRect( 0, 0, 512, 512 );
#else
	m_rcMapWorld.SetRect( nCX - nHalfW, nCY - nHalfH, nCX + nHalfW, nCY + nHalfH );
#endif
	
	// revision map size. [10/20/2009 rumist]
	// decision of map scaling value.
#ifdef NEW_WORLD_MAP		// [2012/10/11 : Sora] 월드맵 개편
	if (m_vecMapData[m_nCurMapData].World.rcMapSize.GetWidth() == 1024 )
	{
		//m_fZoomWorld = 0.5f;
		m_fMapSizeRevision = 0.5f;
	}
	else
	{
		
		m_fMapSizeRevision = 1.0f;
	}

	m_fZoomWorld = 1.0f;
#else
	if (m_vecMapData[m_nCurMapData].World.rcMapSize.GetWidth() == 1024 )
	{
		m_fMapSizeRevision = 0.5f;
	}
	else
		m_fMapSizeRevision = 1.0f;
#endif

	AdjustViewport( 6 );

	m_fMoveElapsedErrXWorld = 0.0f;
	m_fMoveElapsedErrYWorld = 0.0f;
	m_bNowScrollMapWorld = FALSE;
	m_bNowScrollMapDetail = FALSE;

	// Release map texture
	if( m_ptdWorldTex[0] )
	{
		_pTextureStock->Release( m_ptdWorldTex[0] );
		m_ptdWorldTex[0] = NULL;
	}
	if( m_ptdWorldTex[1] )
	{
		_pTextureStock->Release( m_ptdWorldTex[1] );
		m_ptdWorldTex[1] = NULL;
	}
	if( m_ptdDetailTex )
	{
		_pTextureStock->Release( m_ptdDetailTex );
		m_ptdDetailTex = NULL;
	}

	// Create map texture
	CTString	strFileName;

	if (nWorld == ZONE_WORLD_MAP)
	{
		strFileName.PrintF( "Local\\%s\\Map_World%d%d.tex", DefHelp::getNationPostfix(g_iCountry, true), nWorld, sbLayer );
	}
	else
	{
		strFileName.PrintF( "Data\\Interface\\Map_World%d%d.tex", nWorld, sbLayer );
	}

	m_nCurWorldTex = 0;
	m_ptdWorldTex[0] = CreateTexture( strFileName );
	m_fWorldTexWidth = m_ptdWorldTex[0]->GetPixWidth();
	m_fWorldTexHeight = m_ptdWorldTex[0]->GetPixHeight();

#ifdef NEW_WORLD_MAP		// [2012/10/11 : Sora] 월드맵 개편
	if (m_vecMapData[m_nCurMapData].World.rcMapSize.GetWidth() == 1024 )
		m_rtWorld.SetUV( 0, 0, 1024, 1024,
						m_fWorldTexWidth, m_fWorldTexHeight );
	else
#endif
		m_rtWorld.SetUV( m_rcMapWorld.Left, m_rcMapWorld.Top, m_rcMapWorld.Right, m_rcMapWorld.Bottom,
						m_fWorldTexWidth, m_fWorldTexHeight );
}

// ----------------------------------------------------------------------------
// Name : ChangeMap()
// Desc :
// ----------------------------------------------------------------------------
void CUIMap::ChangeMap()
{
	// 공성 용 맵의 경우에는 공성 맵 정보 요청
	if(m_nCurMapData!=-1){
		if( IsPossibleCastleMap() && IsCastleMap( m_nCurMapData ) )
		{
			_pNetwork->SendCastleMapRecent();
		}
	}

	if( m_cmbMap.GetCurSel() == -1 || m_nCurMapData == -1 ||m_nCurMapData == ((m_cmbMap.GetCurSel() >= 3) ? m_cmbMap.GetCurSel()+1:m_cmbMap.GetCurSel()) )
		return;

	// Change 
	m_qSignalData.clear();
	// 신규필드 추가로 3(메라크 공성지역)이후 +1을 더한다.
	// m_vecMapData 모든 지역 데이터 포함 공성이 아닌경우 공성 지역은 표시에서 제외된다.
	m_nCurMapData	= m_cmbMap.GetCurSel();
	m_nCurMapData	= (!IsPossibleCastleMap()&&m_nCurMapData>=3) ? m_nCurMapData+1:m_nCurMapData;
	int	nZoneIndex	= m_vecMapData[m_nCurMapData].World.nZoneIndex;
	m_sbCurLayer	= m_vecMapData[m_nCurMapData].World.sbLayer;
	m_bShowWorldMap = TRUE;

	MakeBaseMapSize( TRUE );
	int	nLayerDiff = m_sbCurLayer - _pNetwork->MyCharacterInfo.yLayer;
	int	nMapIndex = FindMapData( nZoneIndex, _pNetwork->MyCharacterInfo.yLayer );
	if( nZoneIndex == _pNetwork->MyCharacterInfo.zoneNo &&
		( nMapIndex == -1 || ( nLayerDiff >= -1 && nLayerDiff <= 1 ) ) )
	{
		int	nCX = _pNetwork->MyCharacterInfo.x * m_vecMapData[m_nCurMapData].World.fRatio / m_fZoomWorld;
		int	nCY = _pNetwork->MyCharacterInfo.z * m_vecMapData[m_nCurMapData].World.fRatio / m_fZoomWorld;
		int	nHalfW = m_nMapWorldBaseWidth / m_fZoomWorld * 0.5f;
		int	nHalfH = m_nMapWorldBaseHeight / m_fZoomWorld * 0.5f;
		m_rcMapWorld.SetRect( nCX - nHalfW, nCY - nHalfH, nCX + nHalfW, nCY + nHalfH );
		AdjustViewport( 6 );
	}
	else
	{
		m_rcMapWorld.SetRect( 0, 0, m_nMapWorldBaseWidth / m_fZoomWorld, m_nMapWorldBaseHeight / m_fZoomWorld );
		AdjustViewport( 1 );
	}

	m_fMoveElapsedErrXWorld = 0.0f;
	m_fMoveElapsedErrYWorld = 0.0f;
	m_fMoveElapsedErrXDetail = 0.0f;
	m_fMoveElapsedErrYDetail = 0.0f;
	m_bNowScrollMapWorld = FALSE;
	m_bNowScrollMapDetail = FALSE;

	// Release map texture
	if( m_ptdWorldTex[0] )
	{
		_pTextureStock->Release( m_ptdWorldTex[0] );
		m_ptdWorldTex[0] = NULL;
	}
	if( m_ptdWorldTex[1] )
	{
		_pTextureStock->Release( m_ptdWorldTex[1] );
		m_ptdWorldTex[1] = NULL;
	}
	if( m_ptdDetailTex )
	{
		_pTextureStock->Release( m_ptdDetailTex );
		m_ptdDetailTex = NULL;
	}

	// Create map texture
	CTString	strFileName;
	strFileName.PrintF( "Data\\Interface\\Map_World%d%d.tex", nZoneIndex, m_sbCurLayer );

	m_nCurWorldTex = 0;
	m_ptdWorldTex[0] = CreateTexture( strFileName );
	m_fWorldTexWidth = m_ptdWorldTex[0]->GetPixWidth();
	m_fWorldTexHeight = m_ptdWorldTex[0]->GetPixHeight();
	

	// revision map size. [10/20/2009 rumist]
	// decision of map scaling value.
	//if (m_vecMapData[m_nCurMapData].World.rcMapSize.GetWidth() == 1024 )
#ifdef NEW_WORLD_MAP		// [2012/10/11 : Sora] 월드맵 개편
	if (m_fWorldTexWidth == 1024 && !_pUISWDoc->IsWar())
	{
		//m_fZoomWorld = 0.5f;
		m_fMapSizeRevision = 0.5f;
	}
	else
	{
		
		m_fMapSizeRevision = 1.0f;
	}

	m_fZoomWorld = 1.0f;
#else
	if (m_fWorldTexWidth == 1024 && !_pUISWDoc->IsWar())
	{
		m_fMapSizeRevision = 0.5f;	}
	else
		m_fMapSizeRevision = 1.0f;
#endif


	m_rtWorld.SetUV( m_rcMapWorld.Left, m_rcMapWorld.Top, m_rcMapWorld.Right, m_rcMapWorld.Bottom,
						m_fWorldTexWidth * m_fMapSizeRevision, m_fWorldTexHeight * m_fMapSizeRevision );

	AdjustViewport( 6 );

	m_eMapState = eMS_ZONE;
}

// ----------------------------------------------------------------------------
// Name : OpenDetailMap()
// Desc :
// ----------------------------------------------------------------------------
void CUIMap::OpenDetailMap( int nIndex )
{
	if( m_nCurMapData == -1 )
		return;

	_bDetail = true;

	if( nIndex >= m_vecMapData[m_nCurMapData].vecDetail.size() )
	{
		SetDetailMap( FALSE );
		return;
	}

	//if( nIndex >= 0 )
		m_nCurDetail = nIndex;

	int		nZoneIndex = m_vecMapData[m_nCurMapData].World.nZoneIndex;
	SBYTE	sbLayer = m_vecMapData[m_nCurMapData].World.sbLayer;

	m_slbZoomDetail.SetCurPos( MAP_ZOOM_SLIDE_ORIGIN );
	m_fZoomDetail = 1.0f;

	MakeBaseMapSize( FALSE );
	if( nZoneIndex == _pNetwork->MyCharacterInfo.zoneNo )
	{
		int	nCX = ( _pNetwork->MyCharacterInfo.x - m_vecMapData[m_nCurMapData].vecDetail[m_nCurDetail].fOffsetX ) *
					m_vecMapData[m_nCurMapData].vecDetail[m_nCurDetail].fRatio / m_fZoomDetail;
		int	nCY = ( _pNetwork->MyCharacterInfo.z - m_vecMapData[m_nCurMapData].vecDetail[m_nCurDetail].fOffsetZ ) *
					m_vecMapData[m_nCurMapData].vecDetail[m_nCurDetail].fRatio / m_fZoomDetail;
		int	nHalfW = m_nMapDetailBaseWidth / m_fZoomDetail * 0.5f;
		int	nHalfH = m_nMapDetailBaseHeight / m_fZoomDetail * 0.5f;
		m_rcMapDetail.SetRect( nCX - nHalfW, nCY - nHalfH, nCX + nHalfW, nCY + nHalfH );
		AdjustViewport( 6 );
	}
	else
	{
		m_rcMapDetail.SetRect( 0, 0, m_nMapDetailBaseWidth / m_fZoomDetail, m_nMapDetailBaseHeight / m_fZoomDetail );
		AdjustViewport( 6 );
	}

	m_fMoveElapsedErrXDetail = 0.0f;
	m_fMoveElapsedErrYDetail = 0.0f;
	m_bNowScrollMapWorld = FALSE;
	m_bNowScrollMapDetail = FALSE;

	// Release map texture
	if( m_ptdDetailTex )
	{
		_pTextureStock->Release( m_ptdDetailTex );
		m_ptdDetailTex = NULL;
	}

	// Create map texture
	CTString	strFileName;
	strFileName.PrintF( "Data\\Interface\\Map_World%d%d_%d.tex", nZoneIndex, sbLayer, nIndex );

	m_ptdDetailTex = CreateTexture( strFileName );
	m_fDetailTexWidth = m_ptdDetailTex->GetPixWidth();
	m_fDetailTexHeight = m_ptdDetailTex->GetPixHeight();

	m_rtDetail.SetUV( m_rcMapDetail.Left, m_rcMapDetail.Top, m_rcMapDetail.Right, m_rcMapDetail.Bottom,
						m_fDetailTexWidth, m_fDetailTexHeight );

	m_bMapBlendingForDetail = TRUE;
	m_llMapBlendTimeForDetail = _pTimer->GetHighPrecisionTimer().GetMilliseconds();

#ifdef NEW_WORLD_MAP		// [2012/10/11 : Sora] 월드맵 개편
	m_fZoomWorld = 1.0f;
	m_fMapSizeRevision = 1.0f;
#endif

	m_eMapState = eMS_VILLAGE;
}

// ----------------------------------------------------------------------------
// Name : CloseDetailMap()
// Desc :
// ----------------------------------------------------------------------------
void CUIMap::CloseDetailMap()
{
	_bDetail = false;
	m_eMapState = eMS_ZONE;

	int	nZoneIndex = m_vecMapData[m_nCurMapData].World.nZoneIndex;
	if( _pNetwork->MyCharacterInfo.zoneNo != nZoneIndex )
	{
		m_bMapBlendingForDetail = TRUE;
		m_llMapBlendTimeForDetail = _pTimer->GetHighPrecisionTimer().GetMilliseconds();

		// 맵사이즈 변경 [11/6/2012 Ranma]
		if (m_vecMapData[m_nCurMapData].World.rcMapSize.GetWidth() == 1024)
		{
			m_fMapSizeRevision = 0.5f;
		}
 	
		return;
	}

	if( nZoneIndex == _pNetwork->MyCharacterInfo.zoneNo )
	{
		int	nCX = _pNetwork->MyCharacterInfo.x * m_vecMapData[m_nCurMapData].World.fRatio / m_fZoomWorld;
		int	nCY = _pNetwork->MyCharacterInfo.z * m_vecMapData[m_nCurMapData].World.fRatio / m_fZoomWorld;
		int	nHalfW = m_nMapWorldBaseWidth / m_fZoomWorld * 0.5f;
		int	nHalfH = m_nMapWorldBaseHeight / m_fZoomWorld * 0.5f;
		m_rcMapWorld.SetRect( nCX - nHalfW, nCY - nHalfH, nCX + nHalfW, nCY + nHalfH );
		AdjustViewport( 6 );
	}

	m_fMoveElapsedErrXWorld = 0.0f;
	m_fMoveElapsedErrYWorld = 0.0f;
	m_bNowScrollMapWorld = FALSE;
	m_bNowScrollMapDetail = FALSE;

	m_bMapBlendingForDetail = TRUE;
	m_llMapBlendTimeForDetail = _pTimer->GetHighPrecisionTimer().GetMilliseconds();

#ifdef NEW_WORLD_MAP		// [2012/10/11 : Sora] 월드맵 개편
	if (m_vecMapData[m_nCurMapData].World.rcMapSize.GetWidth() == 1024 )
	{
		m_fMapSizeRevision = 0.5f;
	}
	else
	{
		m_fMapSizeRevision = 1.0f;
	}
	m_fZoomWorld = 1.0f;
		
#endif
}

// ----------------------------------------------------------------------------
// Name : ScrollMapWorld()
// Desc :
// ----------------------------------------------------------------------------
void CUIMap::ScrollMapWorld( int ndX, int ndY )
{
	if( m_nCurMapData == -1 )
		return;

	FLOAT	fdX = (FLOAT)ndX / m_fZoomWorld;
	FLOAT	fdY = (FLOAT)ndY / m_fZoomWorld;

	BOOL	bModified = FALSE;
	UIRect	rcWhole = m_vecMapData[m_nCurMapData].World.rcMapSize;

	// Scroll horizontal
	if( ( ndX < 0 && m_rcMapWorld.Left > rcWhole.Left ) ||
		( ndX > 0 && m_rcMapWorld.Right < rcWhole.Right ) )
	{
		int	nX = (int)( fdX + m_fMoveElapsedErrXWorld );
		if( m_rcMapWorld.Left + nX < rcWhole.Left )
			nX = rcWhole.Left - m_rcMapWorld.Left;
		else if( m_rcMapWorld.Right + nX > rcWhole.Right )
			nX = rcWhole.Right - m_rcMapWorld.Right;
		m_rcMapWorld.Left += nX;
		m_rcMapWorld.Right += nX;
		m_fMoveElapsedErrXWorld += fdX - (FLOAT)nX;
		bModified = TRUE;
	}

	// Scroll vertical
	if( ( ndY < 0 && m_rcMapWorld.Top > rcWhole.Top ) ||
		( ndY > 0 && m_rcMapWorld.Bottom < rcWhole.Bottom ) )
	{
		int	nY = (int)( fdY + m_fMoveElapsedErrYWorld );
		if( m_rcMapWorld.Top + nY < rcWhole.Top )
			nY = rcWhole.Top - m_rcMapWorld.Top;
		else if( m_rcMapWorld.Bottom + nY > rcWhole.Bottom )
			nY = rcWhole.Bottom - m_rcMapWorld.Bottom;
		m_rcMapWorld.Top += nY;
		m_rcMapWorld.Bottom += nY;
		m_fMoveElapsedErrYWorld += fdY - (FLOAT)nY;
		bModified = TRUE;
	}

	if( bModified )
	{
		m_rtWorld.SetUV( m_rcMapWorld.Left, m_rcMapWorld.Top, m_rcMapWorld.Right, m_rcMapWorld.Bottom,
						 m_fWorldTexWidth * m_fMapSizeRevision,
						 m_fWorldTexHeight * m_fMapSizeRevision
							);
	}
}

// ----------------------------------------------------------------------------
// Name : ScrollMapDetail()
// Desc :
// ----------------------------------------------------------------------------
void CUIMap::ScrollMapDetail( int ndX, int ndY )
{
	if( m_nCurMapData == -1 || m_nCurDetail == -1 )
		return;

	FLOAT	fdX = (FLOAT)ndX / m_fZoomDetail;
	FLOAT	fdY = (FLOAT)ndY / m_fZoomDetail;

	if ( m_vecMapData.size() <= m_nCurMapData || m_vecMapData[m_nCurMapData].vecDetail.size() <= m_nCurDetail )
	{
		return;
	}
	BOOL	bModified = FALSE;
	UIRect	rcWhole = m_vecMapData[m_nCurMapData].vecDetail[m_nCurDetail].rcMapSize;

	// Scroll horizontal
	if( ( ndX < 0 && m_rcMapDetail.Left > rcWhole.Left ) ||
		( ndX > 0 && m_rcMapDetail.Right < rcWhole.Right ) )
	{
		int	nX = (int)( fdX + m_fMoveElapsedErrXDetail );
		if( m_rcMapDetail.Left + nX < rcWhole.Left )
			nX = rcWhole.Left - m_rcMapDetail.Left;
		else if( m_rcMapDetail.Right + nX > rcWhole.Right )
			nX = rcWhole.Right - m_rcMapDetail.Right;
		m_rcMapDetail.Left += nX;
		m_rcMapDetail.Right += nX;
		m_fMoveElapsedErrXDetail += fdX - (FLOAT)nX;
		bModified = TRUE;
	}

	// Scroll vertical
	if( ( ndY < 0 && m_rcMapDetail.Top > rcWhole.Top ) ||
		( ndY > 0 && m_rcMapDetail.Bottom < rcWhole.Bottom ) )
	{
		int	nY = (int)( fdY + m_fMoveElapsedErrYDetail );
		if( m_rcMapDetail.Top + nY < rcWhole.Top )
			nY = rcWhole.Top - m_rcMapDetail.Top;
		else if( m_rcMapDetail.Bottom + nY > rcWhole.Bottom )
			nY = rcWhole.Bottom - m_rcMapDetail.Bottom;
		m_rcMapDetail.Top += nY;
		m_rcMapDetail.Bottom += nY;
		m_fMoveElapsedErrYDetail += fdY - (FLOAT)nY;
		bModified = TRUE;
	}

	if( bModified )
	{
		m_rtDetail.SetUV( m_rcMapDetail.Left, m_rcMapDetail.Top, m_rcMapDetail.Right, m_rcMapDetail.Bottom,
							m_fDetailTexWidth, m_fDetailTexHeight );
	}
}

// ----------------------------------------------------------------------------
// Name : AdjustViewport()
// Desc : nSelRegion - 1: left, 2: right, 3: middle bottom, 4: left bottom
//        5: right bottom, 6: center, 7 : left top
// ----------------------------------------------------------------------------
void CUIMap::AdjustViewport( int nSelRegion )
{
	FLOAT	fRatioW = (FLOAT)( m_nWidth - MAP_FRAME_EDGE_HORZ ) / (FLOAT)MAP_FRAME_BASE_WIDTH;
	FLOAT	fRatioH = (FLOAT)( m_nHeight - MAP_FRAME_EDGE_VERT ) / (FLOAT)MAP_FRAME_BASE_HEIGHT;

	UIRect	rcTempWorld, rcTempDetail;
	if( nSelRegion == 1 || nSelRegion == 4 )
	{
		rcTempWorld.SetRect( m_rcMapWorld.Right - m_nMapWorldBaseWidth * fRatioW / m_fZoomWorld,
								m_rcMapWorld.Top,
								m_rcMapWorld.Right,
								m_rcMapWorld.Top + m_nMapWorldBaseHeight * fRatioH / m_fZoomWorld );
		rcTempDetail.SetRect( m_rcMapDetail.Right - m_nMapDetailBaseWidth * fRatioW / m_fZoomDetail,
								m_rcMapDetail.Top,
								m_rcMapDetail.Right,
								m_rcMapDetail.Top + m_nMapDetailBaseWidth * fRatioH / m_fZoomDetail );
	}
	else if( nSelRegion == 6 )
	{
		int	nHalfWidth = m_nMapWorldBaseWidth * fRatioW / m_fZoomWorld * 0.5f;
		int	nHalfHeight = m_nMapWorldBaseHeight * fRatioH / m_fZoomWorld * 0.5f;
		int	nCX = m_rcMapWorld.Left + m_rcMapWorld.GetWidth() / 2;
		int	nCY = m_rcMapWorld.Top + m_rcMapWorld.GetHeight() / 2;
		rcTempWorld.SetRect( nCX - nHalfWidth, nCY - nHalfHeight,
								nCX + nHalfWidth, nCY + nHalfHeight );

		nHalfWidth = m_nMapDetailBaseWidth * fRatioW / m_fZoomDetail * 0.5f;
		nHalfHeight = m_nMapDetailBaseWidth * fRatioH / m_fZoomDetail * 0.5f;
		nCX = m_rcMapDetail.Left + m_rcMapDetail.GetWidth() / 2;
		nCY = m_rcMapDetail.Top + m_rcMapDetail.GetHeight() / 2;
		rcTempDetail.SetRect( nCX - nHalfWidth, nCY - nHalfHeight,
								nCX + nHalfWidth, nCY + nHalfHeight );
	}
	else if ( nSelRegion == 7 )
	{
		rcTempWorld.SetRect( m_rcMapWorld.Left, m_rcMapWorld.Top, 
								m_rcMapWorld.Left + m_nMapWorldBaseWidth * fRatioW / m_fZoomWorld,
								m_rcMapWorld.Top + m_nMapWorldBaseHeight * fRatioH / m_fZoomWorld );

		rcTempDetail.SetRect( m_rcMapDetail.Left, m_rcMapDetail.Top, 
								m_rcMapDetail.Left + m_nMapDetailBaseWidth * fRatioW / m_fZoomDetail,
								m_rcMapDetail.Top + m_nMapDetailBaseWidth * fRatioH / m_fZoomDetail );
	}
	else
	{
		rcTempWorld.SetRect( m_rcMapWorld.Left,
								m_rcMapWorld.Top,
								m_rcMapWorld.Left + m_nMapWorldBaseWidth * fRatioW / m_fZoomWorld,
								m_rcMapWorld.Top + m_nMapWorldBaseHeight * fRatioH / m_fZoomWorld );
		rcTempDetail.SetRect( m_rcMapDetail.Left,
								m_rcMapDetail.Top,
								m_rcMapDetail.Left + m_nMapDetailBaseWidth * fRatioW / m_fZoomDetail,
								m_rcMapDetail.Top + m_nMapDetailBaseWidth * fRatioH / m_fZoomDetail );
	}

	if( m_nCurMapData != -1 )
	{
		int	ndX = 0, ndY = 0;
		UIRect	rcWholeWorld = m_vecMapData[m_nCurMapData].World.rcMapSize;
#ifdef NEW_WORLD_MAP		// [2012/10/11 : Sora] 월드맵 개편
		m_rcMapWorld.SetRect(0,0,512, 512);
		if(m_fWorldTexWidth == 1024)
			m_rtWorld.SetUV( m_rcMapWorld.Left, m_rcMapWorld.Top, 1024, 1024,
							m_fWorldTexWidth , m_fWorldTexHeight );
		else
			m_rtWorld.SetUV( m_rcMapWorld.Left, m_rcMapWorld.Top, m_rcMapWorld.Right, m_rcMapWorld.Bottom,
							m_fWorldTexWidth * m_fMapSizeRevision, m_fWorldTexHeight * m_fMapSizeRevision );
#else
		// default map size is 512. but dratan & montsain zone map texture size is 1024. [10/20/2009 rumist]
		// revision map size. [10/20/2009 rumist]
		rcWholeWorld.Scale( m_fMapSizeRevision );
		if( rcTempWorld.Left < rcWholeWorld.Left )
			ndX = rcWholeWorld.Left - rcTempWorld.Left;
		else if( rcTempWorld.Right > rcWholeWorld.Right )
			ndX = rcWholeWorld.Right - rcTempWorld.Right;

		if( rcTempWorld.Top < rcWholeWorld.Top )
			ndY = rcWholeWorld.Top - rcTempWorld.Top;
		else if( rcTempWorld.Bottom > rcWholeWorld.Bottom )
			ndY = rcWholeWorld.Bottom - rcTempWorld.Bottom;

		rcTempWorld.Left += ndX;
		rcTempWorld.Right += ndX;
		rcTempWorld.Top += ndY;
		rcTempWorld.Bottom += ndY;

		m_bCanScrollMapWorld = FALSE;
		if( rcWholeWorld.GetWidth() <= rcTempWorld.GetWidth() )
		{
			m_rcMapWorld.Left = rcWholeWorld.Left;
			m_rcMapWorld.Right = rcWholeWorld.Right;

			int	nNewWidth = m_rcMapWorld.GetWidth() * m_fZoomWorld;
			m_rcViewportWorld.Left = 0;//m_rcViewportWorld.Left;//( m_nWidth - MAP_FRAME_EDGE_HORZ - nNewWidth ) / 2;	
			m_rcViewportWorld.Right = m_rcViewportWorld.Left + nNewWidth;
		}
		else
		{
			m_rcMapWorld.Left = rcTempWorld.Left;
			m_rcMapWorld.Right = rcTempWorld.Right;
			m_rcViewportWorld.Left = 0;
			m_rcViewportWorld.Right = m_nWidth - MAP_FRAME_EDGE_HORZ;
			m_bCanScrollMapWorld = TRUE;
		}

		if( rcWholeWorld.GetHeight() <= rcTempWorld.GetHeight() )
		{
			m_rcMapWorld.Top = rcWholeWorld.Top;
			m_rcMapWorld.Bottom = rcWholeWorld.Bottom;

			int	nNewHeight = m_rcMapWorld.GetHeight() * m_fZoomWorld;

			m_rcViewportWorld.Top = 0;//m_rcViewportWorld.Top;//( m_nHeight - MAP_FRAME_EDGE_VERT - nNewHeight ) / 2;
			m_rcViewportWorld.Bottom = m_rcViewportWorld.Top + nNewHeight;
		}
		else
		{
			m_rcMapWorld.Top = rcTempWorld.Top;
			m_rcMapWorld.Bottom = rcTempWorld.Bottom;
			m_rcViewportWorld.Top = 0;
			m_rcViewportWorld.Bottom = m_nHeight - MAP_FRAME_EDGE_VERT;
			m_bCanScrollMapWorld = TRUE;
		}

		// revision map size. [10/20/2009 rumist]
		m_rtWorld.SetUV( m_rcMapWorld.Left, m_rcMapWorld.Top, m_rcMapWorld.Right, m_rcMapWorld.Bottom,
							m_fWorldTexWidth * m_fMapSizeRevision, m_fWorldTexHeight * m_fMapSizeRevision );
#endif	
		if( m_nCurDetail != -1 )
		{
			int	ndX = 0, ndY = 0;
			if ( m_vecMapData.size() <= m_nCurMapData || m_vecMapData[m_nCurMapData].vecDetail.size() <= m_nCurDetail )
			{
				return;
			}
			UIRect	rcWholeDetail = m_vecMapData[m_nCurMapData].vecDetail[m_nCurDetail].rcMapSize;

//			rcWholeDetail.Scale( m_fMapSizeRevision );
#ifdef NEW_WORLD_MAP		// [2012/10/11 : Sora] 월드맵 개편
			m_rcMapDetail.SetRect(0,0,512, 512);
			if(m_fWorldTexWidth == 1024)
				m_rtDetail.SetUV( m_rcMapDetail.Left, m_rcMapDetail.Top, m_rcMapDetail.Right * 2, m_rcMapDetail.Bottom * 2,
								m_fWorldTexWidth * m_fMapSizeRevision, m_fWorldTexHeight * m_fMapSizeRevision );
			else
				m_rtDetail.SetUV( m_rcMapDetail.Left, m_rcMapDetail.Top, m_rcMapDetail.Right, m_rcMapDetail.Bottom,
								m_fWorldTexWidth * m_fMapSizeRevision, m_fWorldTexHeight * m_fMapSizeRevision );
#else
			if( rcTempDetail.Left < rcWholeDetail.Left )
				ndX = rcWholeDetail.Left - rcTempDetail.Left;
			else if( rcTempDetail.Right > rcWholeDetail.Right )
				ndX = rcWholeDetail.Right - rcTempDetail.Right;

			if( rcTempDetail.Top < rcWholeDetail.Top )
				ndY = rcWholeDetail.Top - rcTempDetail.Top;
			else if( rcTempDetail.Bottom > rcWholeDetail.Bottom )
				ndY = rcWholeDetail.Bottom - rcTempDetail.Bottom;

			rcTempDetail.Left += ndX;
			rcTempDetail.Right += ndX;
			rcTempDetail.Top += ndY;
			rcTempDetail.Bottom += ndY;

			m_bCanScrollMapDetail = FALSE;
			if( rcWholeDetail.GetWidth() <= rcTempDetail.GetWidth() )
			{
				m_rcMapDetail.Left = rcWholeDetail.Left;
				m_rcMapDetail.Right = rcWholeDetail.Right;

				int	nNewWidth = m_rcMapDetail.GetWidth() * m_fZoomDetail;

				m_rcViewportDetail.Left = 0;//m_rcViewportDetail.Left;//( m_nWidth - MAP_FRAME_EDGE_HORZ - nNewWidth ) / 2;
				m_rcViewportDetail.Right = m_rcViewportDetail.Left + nNewWidth;
			}
			else
			{
				m_rcMapDetail.Left = rcTempDetail.Left;
				m_rcMapDetail.Right = rcTempDetail.Right;
				m_rcViewportDetail.Left = 0;
				m_rcViewportDetail.Right = m_nWidth - MAP_FRAME_EDGE_HORZ;
				m_bCanScrollMapDetail = TRUE;
			}

			if( rcWholeDetail.GetHeight() < rcTempDetail.GetHeight() )
			{
				m_rcMapDetail.Top = rcWholeDetail.Top;
				m_rcMapDetail.Bottom = rcWholeDetail.Bottom;

				int	nNewHeight = m_rcMapDetail.GetHeight() * m_fZoomDetail;

				m_rcViewportDetail.Top = 0;//m_rcViewportDetail.Top;//( m_nHeight - MAP_FRAME_EDGE_VERT - nNewHeight ) / 2;
				m_rcViewportDetail.Bottom = m_rcViewportDetail.Top + nNewHeight;
			}
			else
			{
				m_rcMapDetail.Top = rcTempDetail.Top;
				m_rcMapDetail.Bottom = rcTempDetail.Bottom;
				m_rcViewportDetail.Top = 0;
				m_rcViewportDetail.Bottom = m_nHeight - MAP_FRAME_EDGE_VERT;
				m_bCanScrollMapDetail = TRUE;
			}

			m_rtDetail.SetUV( m_rcMapDetail.Left, m_rcMapDetail.Top, m_rcMapDetail.Right, m_rcMapDetail.Bottom,
								m_fDetailTexWidth, m_fDetailTexHeight );
#endif
		}
	}
}

// ----------------------------------------------------------------------------
// Name : ResizeMapFrame()
// Desc : nSelRegion - 1: left, 2: right, 3: middle bottom, 4: left bottom
//        5: right bottom
// ----------------------------------------------------------------------------
void CUIMap::ResizeMapFrame( int nSelRegion, int ndX, int ndY )
{
	m_nResizeWidthOffset += ndX;
	m_nResizeHeightOffset += ndY;

	int	nX = ndY;//m_nResizeWidthOffset;
	int	nY = ndX;//m_nResizeHeightOffset;

	switch( nSelRegion )
	{
	case 1:
		if( m_nWidth - nX < MAP_FRAME_WIDTH_MIN )
			nX = m_nWidth - MAP_FRAME_WIDTH_MIN;
		m_nPosX += nX;
		m_nWidth -= nX;
		break;

	case 2:
		if( m_nWidth + nX < MAP_FRAME_WIDTH_MIN )
			nX = MAP_FRAME_WIDTH_MIN - m_nWidth;
		m_nWidth += nX;
		break;

	case 3:
		if( m_nHeight + nY < MAP_FRAME_HEIGHT_MIN )
			nY = MAP_FRAME_HEIGHT_MIN - m_nHeight;
		m_nHeight += nY;
		break;

	case 4:
		if( m_nWidth - nX < MAP_FRAME_WIDTH_MIN )
			nX = m_nWidth - MAP_FRAME_WIDTH_MIN;
		m_nPosX += nX;
		m_nWidth -= nX;
		
		if( m_nHeight + nY < MAP_FRAME_HEIGHT_MIN )
			nY = MAP_FRAME_HEIGHT_MIN - m_nHeight;
		m_nHeight += nY;
		break;

	case 5:
		if( m_nWidth + nX < MAP_FRAME_WIDTH_MIN )
			nX = MAP_FRAME_WIDTH_MIN - m_nWidth;
		m_nWidth += nX;
		if( m_nHeight + nY < MAP_FRAME_HEIGHT_MIN )
			nY = MAP_FRAME_HEIGHT_MIN - m_nHeight;
		m_nHeight += nY;
		break;
	}

	m_btnClose.SetPosX( m_nWidth - MAP_CLOSE_X );
	m_rcTitle.Right = m_nWidth;
	m_rcResizeLeft.SetRect( 0, 22, 10, m_nHeight - 10 );
	m_rcResizeRight.SetRect( m_nWidth - 10, 22, m_nWidth, m_nHeight - 10 );
	m_rcResizeBottomL.SetRect( 0, m_nHeight - 10, 10, m_nHeight );
	m_rcResizeBottomM.SetRect( 10, m_nHeight - 10, m_nWidth - 10, m_nHeight );
	m_rcResizeBottomR.SetRect( m_nWidth - 10, m_nHeight - 10, m_nWidth, m_nHeight );

#ifdef NEW_WORLD_MAP		// [2012/10/11 : Sora] 월드맵 개편
	AdjustViewport( 6 );
#else
	AdjustViewport( nSelRegion );
#endif
}

// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
void CUIMap::Render()
{
	int		nX, nY;
	float	fX, fZ;

	// World map changing by layer
	if( _pNetwork->MyCharacterInfo.zoneNo == m_vecMapData[m_nCurMapData].World.nZoneIndex )
	{
		int	nLayerDiff = _pNetwork->MyCharacterInfo.yLayer - m_sbCurLayer;
		if( nLayerDiff < -1 || nLayerDiff > 1 )
		{
			m_sbCurLayer = _pNetwork->MyCharacterInfo.yLayer;
			int	nMapDataIndex = FindMapData( _pNetwork->MyCharacterInfo.zoneNo,
												_pNetwork->MyCharacterInfo.yLayer );
			if( nMapDataIndex != -1 )
			{
				m_nCurMapData = nMapDataIndex;
				m_cmbMap.SetCurSel( (!IsPossibleCastleMap()&&m_nCurMapData>=4) ? m_nCurMapData-1:m_nCurMapData );
				m_bMapBlendingForLayer = TRUE;
				m_nCurWorldTex = m_nCurWorldTex == 0 ? 1 : 0;

				// Release map texture
				if( m_ptdWorldTex[m_nCurWorldTex] )
				{
					_pTextureStock->Release( m_ptdWorldTex[m_nCurWorldTex] );
					m_ptdWorldTex[m_nCurWorldTex] = NULL;
				}

				// Create map texture
				CTString	strFileName;
				strFileName.PrintF( "Data\\Interface\\Map_World%d%d.tex",
									_pNetwork->MyCharacterInfo.zoneNo, m_sbCurLayer );
				m_ptdWorldTex[m_nCurWorldTex] = CreateTexture( strFileName );

				m_llMapBlendTimeForLayer = _pTimer->GetHighPrecisionTimer().GetMilliseconds();
				m_llMapBlendElapsedTimeForLayer = 0;
			}
		}
	}

	// Frame blending
	FLOAT	fBackRatio = 1.0f;
	if( m_bFrameBlending )
	{
		__int64	llCurTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();
		m_llFrameBlendElapsedTime += llCurTime - m_llFrameBlendTime;
		m_llFrameBlendTime = llCurTime;

		if( m_llFrameBlendElapsedTime < MAP_FRAME_BLENDTIME )
		{
			fBackRatio = m_llFrameBlendElapsedTime / (FLOAT)MAP_FRAME_BLENDTIME;
			if( m_bShowFrame )
				fBackRatio = 1.0f - fBackRatio;
		}
		else
		{
			m_llFrameBlendElapsedTime = 0;
			m_bFrameBlending = FALSE;
			m_bShowFrame = !m_bShowFrame;
		}
	}

	// Map blending for detail map
	FLOAT	fDetailRatio = 1.0f;
	if( m_bMapBlendingForDetail )
	{
		__int64	llCurTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();
		if( llCurTime - m_llMapBlendTimeForDetail < MAP_DETAIL_BLENDTIME )
		{
			fDetailRatio = ( llCurTime - m_llMapBlendTimeForDetail ) / (FLOAT)MAP_DETAIL_BLENDTIME;
			if( m_bShowWorldMap )
				fDetailRatio = 1.0f - fDetailRatio;
		}
		else
		{
			m_bMapBlendingForDetail = FALSE;
			m_bShowWorldMap = !m_bShowWorldMap;
		}
	}

	// Emphasis of Character position [9/14/2009 rumist]
	// Map blending for char position.
	m_fCharRatio = 1.0f;
	{
		__int64 llCurTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();
		
		if( llCurTime - m_llMapBlendTimeForCharPos > MAP_CHAR_BLENDTIME )
		{
			m_llMapBlendTimeForCharPos = llCurTime;
		}

		if( llCurTime - m_llMapBlendTimeForCharPos < (MAP_CHAR_BLENDTIME / 2) )
		{
			m_fCharRatio = ( llCurTime - m_llMapBlendTimeForCharPos ) / ((FLOAT)MAP_CHAR_BLENDTIME / 2);
		}
		else if( llCurTime - m_llMapBlendTimeForCharPos <= MAP_CHAR_BLENDTIME )
		{
			m_fCharRatio = 2.0f - ( ( llCurTime - m_llMapBlendTimeForCharPos ) / ((FLOAT)MAP_CHAR_BLENDTIME / 2) );
		}
	}

	// Map blending for layer
	FLOAT	fLayerRatio = 1.0f;
	if( m_bMapBlendingForLayer )
	{
		__int64	llCurTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();
		m_llMapBlendElapsedTimeForLayer += llCurTime - m_llMapBlendTimeForLayer;
		m_llMapBlendTimeForLayer = llCurTime;

		if( m_llMapBlendElapsedTimeForLayer < MAP_LAYER_BLENDTIME )
		{
			fLayerRatio = m_llMapBlendElapsedTimeForLayer / (FLOAT)MAP_LAYER_BLENDTIME;
			if( m_nCurWorldTex == 1 )
				fLayerRatio = 1.0f - fLayerRatio;
		}
		else
		{
			m_llMapBlendElapsedTimeForLayer = 0;
			m_bMapBlendingForLayer = FALSE;
		}
	}

	CUIManager* pUIManager = CUIManager::getSingleton();
	CDrawPort* pDrawPort = pUIManager->GetDrawPort();

	// Background
	if( m_bShowFrame || m_bFrameBlending )
	{
		COLOR	colBlend = 0xFFFFFF00 | UBYTE( 0xFF * fBackRatio * m_fMapOpacity );

		// Set map frame texture
		pDrawPort->InitTextureData( m_ptdBaseTexture );

		// Add render regions
		pDrawPort->AddTexture( m_nPosX + 3, m_nPosY + 22,
											m_nPosX + m_nWidth - 3, m_nPosY + m_nHeight - 3,
											m_rtBackListBar.U0, m_rtBackListBar.V0, m_rtBackListBar.U1, m_rtBackListBar.V1,
											colBlend );

		// Render all elements
		pDrawPort->FlushRenderingQueue();
	}

	nX = m_nPosX + MAP_SX;
	nY = m_nPosY + MAP_SY;

	// Detail map
	if( m_bMapBlendingForDetail || !m_bShowWorldMap )
	{
		COLOR	colBlend;
		if( m_bShowWorldMap )
			colBlend = 0xFFFFFF00 | UBYTE( 0xFF * ( 1.0f - fDetailRatio ) * m_fMapOpacity );
		else
			colBlend = 0xFFFFFF00 | UBYTE( 0xFF * fDetailRatio * m_fMapOpacity );

		// Set detail texture
		pDrawPort->InitTextureData( m_ptdDetailTex );

		// Add render regions
		pDrawPort->AddTexture( nX + m_rcViewportDetail.Left, nY + m_rcViewportDetail.Top,
											nX + m_rcViewportDetail.Right, nY + m_rcViewportDetail.Bottom,
											m_rtDetail.U0, m_rtDetail.V0, m_rtDetail.U1, m_rtDetail.V1,
											colBlend );

		// Render all elements
		pDrawPort->FlushRenderingQueue();
	}

	// World map
	if( m_bMapBlendingForDetail || m_bShowWorldMap )
	{
		BOOL	bShowTex0 = TRUE, bShowTex1 = TRUE;
		if( !m_bMapBlendingForLayer )
		{
			if( m_nCurWorldTex == 0 )
				bShowTex1 = FALSE;
			else
				bShowTex0 = FALSE;
		}

		if( bShowTex0 && m_ptdWorldTex[0] )
		{
			COLOR	colBlend = 0xFFFFFF00 | UBYTE( 0xFF * fDetailRatio * fLayerRatio * m_fMapOpacity );

			// Set world texture
			pDrawPort->InitTextureData( m_ptdWorldTex[0] );

			// Add render regions
			pDrawPort->AddTexture( nX + m_rcViewportWorld.Left, nY + m_rcViewportWorld.Top,
												nX + m_rcViewportWorld.Right, nY + m_rcViewportWorld.Bottom,
												m_rtWorld.U0, m_rtWorld.V0, m_rtWorld.U1, m_rtWorld.V1,
												colBlend );

			// Render all elements
			pDrawPort->FlushRenderingQueue();
		}

		if( bShowTex1 && m_ptdWorldTex[1] )
		{
			COLOR	colBlend = 0xFFFFFF00 | UBYTE( 0xFF * fDetailRatio * ( 1.0f - fLayerRatio ) * m_fMapOpacity );

			// Set world texture
			pDrawPort->InitTextureData( m_ptdWorldTex[1] );

			// Add render regions
			pDrawPort->AddTexture( nX + m_rcViewportWorld.Left, nY + m_rcViewportWorld.Top,
												nX + m_rcViewportWorld.Right, nY + m_rcViewportWorld.Bottom,
												m_rtWorld.U0, m_rtWorld.V0, m_rtWorld.U1, m_rtWorld.V1,
												colBlend );

			// Render all elements
			pDrawPort->FlushRenderingQueue();
		}
	}

	// Set map frame texture
	pDrawPort->InitTextureData( m_ptdBaseTexture );


	// Location of subzone
	int	i;
	UIRect	rcTempWorld = m_rcMapWorld;
	UIRect	rcTempDetail = m_rcMapDetail;
	rcTempWorld.Scale( m_fZoomWorld );
	rcTempDetail.Scale( m_fZoomDetail );
	if( m_bMapBlendingForDetail || m_bShowWorldMap )
	{
		COLOR	colBlend = 0xFFFFFF00 | UBYTE( 0xFF * m_fMapOpacity );

		for( i = 0; i < m_vecMapData[m_nCurMapData].vecSubZone.size(); i++ )
		{
			SBYTE	sbSubType = m_vecMapData[m_nCurMapData].vecSubZone[i].sbType;
			if( m_vecMapData[m_nCurMapData].World.nZoneIndex != ZONE_WORLD_MAP )
			{
				if( sbSubType == VILLAGE )
					continue;

				// revision map size. [10/20/2009 rumist]
				fX = (m_vecMapData[m_nCurMapData].vecSubZone[i].fX - m_vecMapData[m_nCurMapData].World.lOffsetX)*m_fMapSizeRevision;
				fZ = (m_vecMapData[m_nCurMapData].vecSubZone[i].fZ - m_vecMapData[m_nCurMapData].World.lOffsetZ)*m_fMapSizeRevision;	
				nX = fX * m_vecMapData[m_nCurMapData].World.fRatio * m_fZoomWorld;
				nY = fZ * m_vecMapData[m_nCurMapData].World.fRatio * m_fZoomWorld;
				nX += m_rcViewportWorld.Left - rcTempWorld.Left;
				nY += m_rcViewportWorld.Top - rcTempWorld.Top;
			}
			else
			{
				nX = m_vecMapData[m_nCurMapData].vecSubZone[i].fX;
				nY = m_vecMapData[m_nCurMapData].vecSubZone[i].fZ;	
				nX += m_rcViewportWorld.Left - rcTempWorld.Left;
				nY += m_rcViewportWorld.Top - rcTempWorld.Top;
			}

			if( nX + m_rcSubZone[sbSubType].Left >= m_rcViewportWorld.Left &&
				nX + m_rcSubZone[sbSubType].Right <= m_rcViewportWorld.Right &&
				nY + m_rcSubZone[sbSubType].Top >= m_rcViewportWorld.Top &&
				nY + m_rcSubZone[sbSubType].Bottom <= m_rcViewportWorld.Bottom )
			{
				nX += m_nPosX + MAP_SX;
				nY += m_nPosY + MAP_SY;
				pDrawPort->AddTexture( nX + m_rcSubZone[sbSubType].Left, nY + m_rcSubZone[sbSubType].Top,
													nX + m_rcSubZone[sbSubType].Right, nY + m_rcSubZone[sbSubType].Bottom,
													m_rtSubZone[sbSubType].U0, m_rtSubZone[sbSubType].V0,
													m_rtSubZone[sbSubType].U1, m_rtSubZone[sbSubType].V1,
													colBlend );
			}
		}
	}
		

	// Render Castle object 
	if( m_bMapBlendingForDetail || m_bShowWorldMap )
	{
		if( IsCastleMap( m_nCurMapData ) )
		{
			std::sort( m_vecMapData[m_nCurMapData].vecCastleData.begin(), m_vecMapData[m_nCurMapData].vecCastleData.end(), CompareZ );

			COLOR	colBlend = 0xFFFFFF00 | UBYTE( 0xFF * m_fMapOpacity );

			for( i = 0; i < m_vecMapData[m_nCurMapData].vecCastleData.size(); i++ )
			{
				LONG	nType = m_vecMapData[m_nCurMapData].vecCastleData[i].nType;
				
				if( nType == CASTLE_TOWER ) // 수호탑의 경우에는 hp가 0이면 그리지 않는다.
				{
					if( m_vecMapData[m_nCurMapData].vecCastleData[i].nHP <= 0 )
					{
						continue;
					}
				}
				
				// revision map size. [10/20/2009 rumist]
				fX = (m_vecMapData[m_nCurMapData].vecCastleData[i].fX - m_vecMapData[m_nCurMapData].World.lOffsetX)*m_fMapSizeRevision;
				fZ = (m_vecMapData[m_nCurMapData].vecCastleData[i].fZ - m_vecMapData[m_nCurMapData].World.lOffsetZ)*m_fMapSizeRevision;

				nX = fX * m_vecMapData[m_nCurMapData].World.fRatio * m_fZoomWorld;
				nY = fZ * m_vecMapData[m_nCurMapData].World.fRatio * m_fZoomWorld;
				nX += m_rcViewportWorld.Left - rcTempWorld.Left;
				nY += m_rcViewportWorld.Top - rcTempWorld.Top;

				if( nX + m_rcCastle[nType].Left >= m_rcViewportWorld.Left &&
					nX + m_rcCastle[nType].Right <= m_rcViewportWorld.Right &&
					nY + m_rcCastle[nType].Top >= m_rcViewportWorld.Top &&
					nY + m_rcCastle[nType].Bottom <= m_rcViewportWorld.Bottom )
				{
					nX += m_nPosX + MAP_SX;
					nY += m_nPosY + MAP_SY;
					pDrawPort->AddTexture( nX + m_rcCastle[nType].Left, nY + m_rcCastle[nType].Top,
														nX + m_rcCastle[nType].Right, nY + m_rcCastle[nType].Bottom,
														m_rtCastle[nType].U0, m_rtCastle[nType].V0,
														m_rtCastle[nType].U1, m_rtCastle[nType].V1,
														colBlend );
				}

			}
		}
	}

	// connie [2009/9/18] - NPC 찾기 Render Mob Blinking 
	COLOR	colBlend = 0x00FFFF00 | UBYTE( 0xFF * m_fMapOpacity );
	
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

	// connie [2009/9/18] - NPC 찾기
	// BUG FIX : TO-KR-T20100223-010 [2/24/2010 rumist]
	// [2011/04/28 : Sora] m_nCurMapData값은 현재 존값이 아님
	if(pUIManager->m_IsHelpMob && m_bShowWorldMap && pUIManager->m_NPCScrollZoneNo == m_vecMapData[m_nCurMapData].World.nZoneIndex )
	{
		fX = pUIManager->m_fHelpMobX;
		fZ = pUIManager->m_fHelpMobZ;

// 		char temp[128];
// 		sprintf(temp, "%f, %f", fX, fZ);
// 		OutputDebugString(temp);

		float fWorldRatio = m_vecMapData[m_nCurMapData].World.fRatio;

		// revision map size. [10/20/2009 rumist]
		nX = fX * fWorldRatio * m_fZoomWorld * m_fMapSizeRevision;
		nY = fZ * fWorldRatio * m_fZoomWorld * m_fMapSizeRevision;
		nX += m_rcViewportWorld.Left - rcTempWorld.Left;
		nY += m_rcViewportWorld.Top - rcTempWorld.Top;

		if( nX + m_rcNPC.Left >= m_rcViewportWorld.Left && nX + m_rcNPC.Right <= m_rcViewportWorld.Right &&
				nY + m_rcNPC.Top >= m_rcViewportWorld.Top && nY + m_rcNPC.Bottom <= m_rcViewportWorld.Bottom )
		{
			nX += m_nPosX + MAP_SX;
			nY += m_nPosY + MAP_SY;

			if(bHighlight)
			{
				pDrawPort->AddTexture( nX + m_rcHelpMob.Left, nY + m_rcHelpMob.Top,
					nX + m_rcHelpMob.Right, nY + m_rcHelpMob.Bottom,
					m_rtHelpMob.U0, m_rtHelpMob.V0, m_rtHelpMob.U1, m_rtHelpMob.V1,
					colBlend ); 
			}
		}
		
	}

	m_nNpcHighlightIdx = -1;

	// Location of NPC
	for( i = 0; i < m_vecMapData[m_nCurMapData].vecNpc.size(); i++ )
	{
		/**** NPC 안내시스템 NPC 마크 렌더링 *****/	
		static BOOL		bHighlight = FALSE;
		static DOUBLE	dElapsedTime = 0.0;
		static DOUBLE	dOldTime = _pTimer->GetHighPrecisionTimer().GetSeconds();
		DOUBLE	dCurTime = _pTimer->GetHighPrecisionTimer().GetSeconds();
		CMobData* pMobData = NULL;

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
		/*---------------------------------*/

#ifdef NEW_WORLD_MAP		// [2012/10/11 : Sora] 월드맵 개편
		if( m_vecMapData[m_nCurMapData].World.nZoneIndex != ZONE_WORLD_MAP )
		{
			if( CZoneInfo::getSingleton()->GetZoneType( m_vecMapData[m_nCurMapData].World.nZoneIndex ) == ZONE_DUNGEON )
			{
				if( m_vecMapData[m_nCurMapData].World.sbLayer != m_vecMapData[m_nCurMapData].vecNpc[i].nYLayer )
				{
					continue;
				}
			}
		}
#endif

		pMobData = CMobData::getData(m_vecMapData[m_nCurMapData].vecNpc[i].nIndex);

		if (pMobData != NULL)
		{
			if (pMobData->IsChannelFlag(_pNetwork->m_iServerCh) == false)
				continue;
		}

		if (IS_EVENT_ON(A_EVENT_HOLLOWEEN) == 0)
		{
			if (m_vecMapData[m_nCurMapData].vecNpc[i].nIndex == 454 ||
				m_vecMapData[m_nCurMapData].vecNpc[i].nIndex == 455)
				continue;
		}

		fX = (m_vecMapData[m_nCurMapData].vecNpc[i].fX);
		fZ = (m_vecMapData[m_nCurMapData].vecNpc[i].fZ);

		if( m_bShowWorldMap )
		{
			// revision map size. [10/20/2009 rumist]
			fX *= m_fMapSizeRevision;
			fZ *= m_fMapSizeRevision;
			fX -= m_vecMapData[m_nCurMapData].World.lOffsetX;
			fZ -= m_vecMapData[m_nCurMapData].World.lOffsetZ;
			nX = fX * m_vecMapData[m_nCurMapData].World.fRatio * m_fZoomWorld;
			nY = fZ * m_vecMapData[m_nCurMapData].World.fRatio * m_fZoomWorld;
			nX += m_rcViewportWorld.Left - rcTempWorld.Left;
			nY += m_rcViewportWorld.Top - rcTempWorld.Top;
			if( nX + m_rcNPC.Left >= m_rcViewportWorld.Left && nX + m_rcNPC.Right <= m_rcViewportWorld.Right &&
				nY + m_rcNPC.Top >= m_rcViewportWorld.Top && nY + m_rcNPC.Bottom <= m_rcViewportWorld.Bottom )
			{
				nX += m_nPosX + MAP_SX;
				nY += m_nPosY + MAP_SY;

				// 탑 랜더를 위해 따로 처리.
				if(m_vecMapData[m_nCurMapData].vecNpc[i].nIndex == UIMGR()->m_nHelpNpc_Index)
				{
					m_nNpcHighlightPosX = nX;
					m_nNpcHighlightPosY = nY;
					m_bHighlight = bHighlight;
					m_nNpcHighlightIdx = m_vecMapData[m_nCurMapData].vecNpc[i].nIndex;
					continue;
				}

				RenderNpc(pDrawPort, m_vecMapData[m_nCurMapData].vecNpc[i].nIndex, nX, nY, bHighlight);
			}
		}
		else
		{
			if (m_nCurDetail >= 0)
			{
				fX -= m_vecMapData[m_nCurMapData].vecDetail[m_nCurDetail].fOffsetX;
				fZ -= m_vecMapData[m_nCurMapData].vecDetail[m_nCurDetail].fOffsetZ;
				nX = fX * m_vecMapData[m_nCurMapData].vecDetail[m_nCurDetail].fRatio * m_fZoomDetail;
				nY = fZ * m_vecMapData[m_nCurMapData].vecDetail[m_nCurDetail].fRatio * m_fZoomDetail;
				nX += m_rcViewportDetail.Left - rcTempDetail.Left;
				nY += m_rcViewportDetail.Top - rcTempDetail.Top;

				if( nX + m_rcNPC.Left >= m_rcViewportDetail.Left && nX + m_rcNPC.Right <= m_rcViewportDetail.Right &&
					nY + m_rcNPC.Top >= m_rcViewportDetail.Top && nY + m_rcNPC.Bottom <= m_rcViewportDetail.Bottom )
				{
					nX += m_nPosX + MAP_SX;
					nY += m_nPosY + MAP_SY;

					// 탑 랜더를 위해 따로 처리.
					if(m_vecMapData[m_nCurMapData].vecNpc[i].nIndex == UIMGR()->m_nHelpNpc_Index)
					{
						m_nNpcHighlightPosX = nX;
						m_nNpcHighlightPosY = nY;
						m_bHighlight = bHighlight;
						m_nNpcHighlightIdx = m_vecMapData[m_nCurMapData].vecNpc[i].nIndex;
						continue;
					}

					RenderNpc(pDrawPort, m_vecMapData[m_nCurMapData].vecNpc[i].nIndex, nX, nY, bHighlight);
				}
			}
		}
	}

	if (m_nNpcHighlightIdx > 0 )
	{
		RenderNpc(pDrawPort, m_nNpcHighlightIdx, m_nNpcHighlightPosX, m_nNpcHighlightPosY, m_bHighlight);
	}

	pDrawPort->FlushRenderingQueue();

	CUIMapOption* pOption = UIMGR()->GetRadar()->GetMapOption();
	Party* pParty = GAMEDATAMGR()->GetPartyInfo();

	if( _pNetwork->MyCharacterInfo.zoneNo == m_vecMapData[m_nCurMapData].World.nZoneIndex )
	{
		int	nLayerDiff;
		COLOR	colBlend;

		colBlend = 0xFFFFFF00 | UBYTE( 0xFF * m_fCharRatio );
		// Location of Party

		if (pOption->IsCheck(eNT_PARTY) == true)
		{
			if (pParty == NULL)
				return;

			for( i = 0; i < pParty->GetMemberCount(); i++ )
			{

				if(pParty->GetLevel( i ) == 0)	// [sora] 레벨이 0인 멤버는 오프라인 상태이므로 그리지 않는다.
					continue;

				nLayerDiff = pParty->GetLayer( i ) - m_sbCurLayer;
				if( nLayerDiff < -1 || nLayerDiff > 1 )
					continue;
				
				// 파티원 멤버가 같은 맵에 없다면 표시 않함
				if( m_vecMapData[m_nCurMapData].World.nZoneIndex != pParty->GetMemberZone( i ) )
					continue;

				fX = pParty->GetPosX( i );
				fZ = pParty->GetPosZ( i );
				if( m_bShowWorldMap )
				{
					// revision map size. [10/20/2009 rumist]
					fX *= m_fMapSizeRevision;
					fZ *= m_fMapSizeRevision;
					fX -= m_vecMapData[m_nCurMapData].World.lOffsetX;
					fZ -= m_vecMapData[m_nCurMapData].World.lOffsetZ;
					nX = fX * m_vecMapData[m_nCurMapData].World.fRatio * m_fZoomWorld;
					nY = fZ * m_vecMapData[m_nCurMapData].World.fRatio * m_fZoomWorld;
					nX += m_rcViewportWorld.Left - rcTempWorld.Left;
					nY += m_rcViewportWorld.Top - rcTempWorld.Top;
					if( nX + m_rcParty.Left >= m_rcViewportWorld.Left && nX + m_rcParty.Right <= m_rcViewportWorld.Right &&
						nY + m_rcParty.Top >= m_rcViewportWorld.Top && nY + m_rcParty.Bottom <= m_rcViewportWorld.Bottom )
					{
						nX += m_nPosX + MAP_SX;
						nY += m_nPosY + MAP_SY;
						pDrawPort->AddTexture( nX + m_rcParty.Left, nY + m_rcParty.Top,
															nX + m_rcParty.Right, nY + m_rcParty.Bottom,
															m_rtParty.U0, m_rtParty.V0, m_rtParty.U1, m_rtParty.V1,
															0xFFFFFFFF );
					}
				}
				else
				{
					if (m_nCurDetail >= 0)
					{
						fX -= m_vecMapData[m_nCurMapData].vecDetail[m_nCurDetail].fOffsetX;
						fZ -= m_vecMapData[m_nCurMapData].vecDetail[m_nCurDetail].fOffsetZ;
						nX = fX * m_vecMapData[m_nCurMapData].vecDetail[m_nCurDetail].fRatio * m_fZoomDetail;
						nY = fZ * m_vecMapData[m_nCurMapData].vecDetail[m_nCurDetail].fRatio * m_fZoomDetail;
						nX += m_rcViewportDetail.Left - rcTempDetail.Left;
						nY += m_rcViewportDetail.Top - rcTempDetail.Top;
						if( nX + m_rcParty.Left >= m_rcViewportDetail.Left && nX + m_rcParty.Right <= m_rcViewportDetail.Right &&
							nY + m_rcParty.Top >= m_rcViewportDetail.Top && nY + m_rcParty.Bottom <= m_rcViewportDetail.Bottom )
						{
							nX += m_nPosX + MAP_SX;
							nY += m_nPosY + MAP_SY;
							pDrawPort->AddTexture( nX + m_rcParty.Left, nY + m_rcParty.Top,
																nX + m_rcParty.Right, nY + m_rcParty.Bottom,
																m_rtParty.U0, m_rtParty.V0, m_rtParty.U1, m_rtParty.V1,
																0xFFFFFFFF );
						}
					}
				}
			}
		}

		pDrawPort->FlushRenderingQueue();

		if(pUIManager->IsCSFlagOn(CSF_EXPEDITION) && pOption->IsCheck(eNT_FELLOWSHIP) == true) // [sora] 원정대원 맵표시
		{
			pDrawPort->InitTextureData(ptdTopUITexture);

			if (pParty == NULL)
				return;

			for(i=0; i<EXPEDITION_GROUP_MAX; i++)
			{
				for(int j = 0; j < EXPEDITION_MEMBER_PER_GROUP; j++ )
				{
					if(!pParty->IsExpedetionDataExist(i,j))
						continue;

					if(!pParty->GetExpeditionMemberOnline(i,j)) // 온라인 상태가 아니면 그리지 않음
						continue;

					nLayerDiff = pParty->GetExpeditionMemberLayer(i,j) - m_sbCurLayer;
					if( nLayerDiff < -1 || nLayerDiff > 1 )
						continue;
					
					// 파티원 멤버가 같은 맵에 없다면 표시 않함
					if( m_vecMapData[m_nCurMapData].World.nZoneIndex != pParty->GetExpeditionMemberZone(i,j) )
						continue;

					fX = pParty->GetExpeditionMemberPosX(i,j);
					fZ = pParty->GetExpeditionMemberPosZ(i,j);
					if( m_bShowWorldMap )
					{
						// revision map size. [10/20/2009 rumist]
						fX *= m_fMapSizeRevision;
						fZ *= m_fMapSizeRevision;
						fX -= m_vecMapData[m_nCurMapData].World.lOffsetX;
						fZ -= m_vecMapData[m_nCurMapData].World.lOffsetZ;
						nX = fX * m_vecMapData[m_nCurMapData].World.fRatio * m_fZoomWorld;
						nY = fZ * m_vecMapData[m_nCurMapData].World.fRatio * m_fZoomWorld;
						nX += m_rcViewportWorld.Left - rcTempWorld.Left;
						nY += m_rcViewportWorld.Top - rcTempWorld.Top;
						if( nX + m_rcParty.Left >= m_rcViewportWorld.Left && nX + m_rcParty.Right <= m_rcViewportWorld.Right &&
							nY + m_rcParty.Top >= m_rcViewportWorld.Top && nY + m_rcParty.Bottom <= m_rcViewportWorld.Bottom )
						{
							nX += m_nPosX + MAP_SX;
							nY += m_nPosY + MAP_SY;
							pDrawPort->AddTexture( nX + m_rcExpedition.Left, nY + m_rcExpedition.Top,
																nX + m_rcExpedition.Right, nY + m_rcExpedition.Bottom,
																m_rtExpedition[i].U0, m_rtExpedition[i].V0, m_rtExpedition[i].U1, m_rtExpedition[i].V1,
																0xFFFFFFFF );
						}
					}
					else
					{
						if (m_nCurDetail >= 0)
						{
							fX -= m_vecMapData[m_nCurMapData].vecDetail[m_nCurDetail].fOffsetX;
							fZ -= m_vecMapData[m_nCurMapData].vecDetail[m_nCurDetail].fOffsetZ;
							nX = fX * m_vecMapData[m_nCurMapData].vecDetail[m_nCurDetail].fRatio * m_fZoomDetail;
							nY = fZ * m_vecMapData[m_nCurMapData].vecDetail[m_nCurDetail].fRatio * m_fZoomDetail;
							nX += m_rcViewportDetail.Left - rcTempDetail.Left;
							nY += m_rcViewportDetail.Top - rcTempDetail.Top;
							if( nX + m_rcParty.Left >= m_rcViewportDetail.Left && nX + m_rcParty.Right <= m_rcViewportDetail.Right &&
								nY + m_rcParty.Top >= m_rcViewportDetail.Top && nY + m_rcParty.Bottom <= m_rcViewportDetail.Bottom )
							{
								nX += m_nPosX + MAP_SX;
								nY += m_nPosY + MAP_SY;
								pDrawPort->AddTexture( nX + m_rcExpedition.Left, nY + m_rcExpedition.Top,
																	nX + m_rcExpedition.Right, nY + m_rcExpedition.Bottom,
																	m_rtExpedition[i].U0, m_rtExpedition[i].V0, m_rtExpedition[i].U1, m_rtExpedition[i].V1,
																	0xFFFFFFFF );
							}
						}
					}
				}
			}

			pDrawPort->FlushRenderingQueue();
		}

		pDrawPort->InitTextureData( m_ptdBaseTexture );

		// Position & direction of character
		nLayerDiff = _pNetwork->MyCharacterInfo.yLayer - m_sbCurLayer;
		if( nLayerDiff >= -1 && nLayerDiff <= 1 )
		{
			FLOAT	fSin = Sin( -_pNetwork->MyCharacterInfo.r );
			FLOAT	fCos = Cos( -_pNetwork->MyCharacterInfo.r );
			FLOAT	fLeftCos = m_rcPC.Left * fCos;
			FLOAT	fLeftSin = m_rcPC.Left * fSin;
			FLOAT	fRightCos = m_rcPC.Right * fCos;
			FLOAT	fRightSin = m_rcPC.Right * fSin;
			FLOAT	fTopSin = m_rcPC.Top * fSin;
			FLOAT	fTopCos = m_rcPC.Top * fCos;
			FLOAT	fBottomSin = m_rcPC.Bottom * fSin;
			FLOAT	fBottomCos = m_rcPC.Bottom * fCos;

			FLOAT	fX1 = fLeftCos - fTopSin;
			FLOAT	fY1 = fLeftSin + fTopCos;
			FLOAT	fX2 = fLeftCos - fBottomSin;
			FLOAT	fY2 = fLeftSin + fBottomCos;
			FLOAT	fX3 = fRightCos - fBottomSin;
			FLOAT	fY3 = fRightSin + fBottomCos;
			FLOAT	fX4 = fRightCos - fTopSin;
			FLOAT	fY4 = fRightSin + fTopCos;

			if( m_bShowWorldMap )
			{
				// revision map size. [10/20/2009 rumist]
				nX = ( _pNetwork->MyCharacterInfo.x - m_vecMapData[m_nCurMapData].World.lOffsetX ) * m_vecMapData[m_nCurMapData].World.fRatio * m_fZoomWorld * m_fMapSizeRevision;
				nY = ( _pNetwork->MyCharacterInfo.z - m_vecMapData[m_nCurMapData].World.lOffsetZ ) * m_vecMapData[m_nCurMapData].World.fRatio * m_fZoomWorld * m_fMapSizeRevision;
				nX += m_rcViewportWorld.Left - rcTempWorld.Left;
				nY += m_rcViewportWorld.Top - rcTempWorld.Top;
				if( nX + m_rcPC.Left >= m_rcViewportWorld.Left && nX + m_rcPC.Right <= m_rcViewportWorld.Right &&
					nY + m_rcPC.Top >= m_rcViewportWorld.Top && nY + m_rcPC.Bottom <= m_rcViewportWorld.Bottom )
				{
					nX += m_nPosX + MAP_SX;
					nY += m_nPosY + MAP_SY;
					pDrawPort->AddTexture( nX + fX1, nY + fY1, m_rtPC.U0, m_rtPC.V0, colBlend,
						nX + fX2, nY + fY2, m_rtPC.U0, m_rtPC.V1, colBlend,
						nX + fX3, nY + fY3, m_rtPC.U1, m_rtPC.V1, colBlend,
						nX + fX4, nY + fY4, m_rtPC.U1, m_rtPC.V0, colBlend );
				}
			}
			else
			{
				if (m_nCurDetail >= 0)
				{
					fX = _pNetwork->MyCharacterInfo.x - m_vecMapData[m_nCurMapData].vecDetail[m_nCurDetail].fOffsetX;
					fZ = _pNetwork->MyCharacterInfo.z - m_vecMapData[m_nCurMapData].vecDetail[m_nCurDetail].fOffsetZ;
					nX = fX * m_vecMapData[m_nCurMapData].vecDetail[m_nCurDetail].fRatio * m_fZoomDetail;
					nY = fZ * m_vecMapData[m_nCurMapData].vecDetail[m_nCurDetail].fRatio * m_fZoomDetail;
					nX += m_rcViewportDetail.Left - rcTempDetail.Left;
					nY += m_rcViewportDetail.Top - rcTempDetail.Top;
					if( nX + m_rcPC.Left >= m_rcViewportDetail.Left && nX + m_rcPC.Right <= m_rcViewportDetail.Right &&
						nY + m_rcPC.Top >= m_rcViewportDetail.Top && nY + m_rcPC.Bottom <= m_rcViewportDetail.Bottom )
					{
						nX += m_nPosX + MAP_SX;
						nY += m_nPosY + MAP_SY;
						pDrawPort->AddTexture( nX + fX1, nY + fY1, m_rtPC.U0, m_rtPC.V0, colBlend,
							nX + fX2, nY + fY2, m_rtPC.U0, m_rtPC.V1, colBlend,
							nX + fX3, nY + fY3, m_rtPC.U1, m_rtPC.V1, colBlend,
							nX + fX4, nY + fY4, m_rtPC.U1, m_rtPC.V0, colBlend );
					}
				}
			}
		}
	}

	RenderSignal();

	// Render all elements
	pDrawPort->FlushRenderingQueue();

	RenderGPS(pDrawPort);

	RenderRelic(pDrawPort);

	// Background
	if( m_bShowFrame || m_bFrameBlending )
	{
		COLOR	colBlend = 0xFFFFFF00 | UBYTE( 0xFF * fBackRatio );

		// Add render regions
		pDrawPort->AddTexture( m_nPosX, m_nPosY,
											m_nPosX + 34, m_nPosY + 22,
											m_rtBackUL.U0, m_rtBackUL.V0, m_rtBackUL.U1, m_rtBackUL.V1,
											colBlend );
		pDrawPort->AddTexture( m_nPosX + 34, m_nPosY,
											m_nPosX + m_nWidth - 34, m_nPosY + 22,
											m_rtBackUM.U0, m_rtBackUM.V0, m_rtBackUM.U1, m_rtBackUM.V1,
											colBlend );
		pDrawPort->AddTexture( m_nPosX + m_nWidth - 34,
											m_nPosY, m_nPosX + m_nWidth, m_nPosY + 22,
											m_rtBackUR.U0, m_rtBackUR.V0, m_rtBackUR.U1, m_rtBackUR.V1,
											colBlend );
		pDrawPort->AddTexture( m_nPosX, m_nPosY + 22,
											m_nPosX + 34, m_nPosY + m_nHeight - 15,
											m_rtBackML.U0, m_rtBackML.V0, m_rtBackML.U1, m_rtBackML.V1,
											colBlend );
		pDrawPort->AddTexture( m_nPosX + 34, m_nPosY + 22,
											m_nPosX + m_nWidth - 34, m_nPosY + m_nHeight - 15,
											m_rtBackMM.U0, m_rtBackMM.V0, m_rtBackMM.U1, m_rtBackMM.V1,
											colBlend );
		pDrawPort->AddTexture( m_nPosX + m_nWidth - 34, m_nPosY + 22,
											m_nPosX + m_nWidth, m_nPosY + m_nHeight - 15,
											m_rtBackMR.U0, m_rtBackMR.V0, m_rtBackMR.U1, m_rtBackMR.V1,
											colBlend );
		pDrawPort->AddTexture( m_nPosX, m_nPosY + m_nHeight - 15,
											m_nPosX + 34, m_nPosY + m_nHeight,
											m_rtBackLL.U0, m_rtBackLL.V0, m_rtBackLL.U1, m_rtBackLL.V1,
											colBlend );
		pDrawPort->AddTexture( m_nPosX + 34, m_nPosY + m_nHeight - 15,
											m_nPosX + m_nWidth - 34, m_nPosY + m_nHeight,
											m_rtBackLM.U0, m_rtBackLM.V0, m_rtBackLM.U1, m_rtBackLM.V1,
											colBlend );
		pDrawPort->AddTexture( m_nPosX + m_nWidth - 34, m_nPosY + m_nHeight - 15,
											m_nPosX + m_nWidth, m_nPosY + m_nHeight,
											m_rtBackLR.U0, m_rtBackLR.V0, m_rtBackLR.U1, m_rtBackLR.V1,
											colBlend );

		pDrawPort->AddTexture( m_nPosX + 3, m_nPosY + 22,
											m_nPosX + m_nWidth - 3, m_nPosY + 47,
											m_rtBackListBar.U0, m_rtBackListBar.V0, m_rtBackListBar.U1, m_rtBackListBar.V1,
											colBlend );

		pDrawPort->AddTexture( m_nPosX + m_rcCompass.Left, m_nPosY + m_nHeight + m_rcCompass.Top,
											m_nPosX + m_rcCompass.Right, m_nPosY + m_nHeight + m_rcCompass.Bottom,
											m_rtCompass.U0, m_rtCompass.V0, m_rtCompass.U1, m_rtCompass.V1,
											colBlend );

		// Text in map
		pDrawPort->PutTextEx( _S( 190, "지도" ), m_nPosX + MAP_TITLE_TEXT_OFFSETX,
											m_nPosY + MAP_TITLE_TEXT_OFFSETY, colBlend );
		if( m_bShowOpacitySlideInfo )
		{
			if( m_nWidth - MAP_SLIDEINFO_X - m_nOpacityStringWidth > 0 )
			{
				colBlend = ( colBlend & 0x000000FF ) | 0xDDDDDDFF;
#ifdef NEW_WORLD_MAP		// [2012/10/11 : Sora] 월드맵 개편
				pDrawPort->PutTextEx( m_strOpacitySlideInfo, m_slbOpacity.GetAbsPosX() + m_slbOpacity.GetWidth() + 10, m_nPosY + MAP_SLIDEINFO_Y, colBlend);
#else
				pDrawPort->PutTextExRX( m_strOpacitySlideInfo, m_nPosX + m_nWidth - 20/*MAP_SLIDEINFO_X*/,
														m_nPosY + MAP_SLIDEINFO_Y, colBlend );
#endif
			}
		}
		else if( m_bShowZoomSlideInfo )
		{
			if( m_nWidth - MAP_SLIDEINFO_X - m_nZoomStringWidth > 0 )
			{
				colBlend = ( colBlend & 0x000000FF ) | 0xDDDDDDFF;
				pDrawPort->PutTextExRX( m_strZoomSlideInfo, m_nPosX + m_nWidth - 20/*MAP_SLIDEINFO_X*/,
														m_nPosY + MAP_SLIDEINFO_Y, colBlend );
			}
		}

		if( fBackRatio >= 1.0f )
		{
			// Close button
			m_btnClose.Render();

			if( IsPossibleSignal() )
			{
				m_btnSignal.Render();
			}

#ifndef NEW_WORLD_MAP		// [2012/10/11 : Sora] 월드맵 개편
			// Map combo box
			m_cmbMap.Render();

			// Zoom slide bar
			if( m_bShowWorldMap )
				m_slbZoomWorld.Render();
			else
				m_slbZoomDetail.Render();
#else
			pDrawPort->PutTextEx( m_vecMapData[m_nCurMapData].World.strName, m_nPosX + 15, m_nPosY + MAP_SLIDEINFO_Y);
			pDrawPort->PutTextEx( _S( 5798, "투명도"), m_slbOpacity.GetAbsPosX() - 10 - (CTString(_S( 5798, "투명도")).Length() * (_pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing())), m_nPosY + MAP_SLIDEINFO_Y);
#endif

			// Opacity slide bar
			m_slbOpacity.Render();
		}

		// Render all elements
		pDrawPort->FlushRenderingQueue();

		// Flush all render text queue
		pDrawPort->EndTextEx();

#ifdef NEW_WORLD_MAP		// [2012/10/11 : Sora] 월드맵 개편
		if( fBackRatio >= 1.0f )
		{
			pDrawPort->InitTextureData( ptdButtonTexture );
			m_btnGoTop.Render();
			pDrawPort->FlushRenderingQueue();
			pDrawPort->EndTextEx();
		}
#endif
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
			pDrawPort->PutTextEx( m_strToolTip, m_rcToolTip.Left + 8, m_rcToolTip.Top + 3 );

			// Flush all render text queue
			pDrawPort->EndTextEx();
		}
	}
}

// ----------------------------------------------------------------------------
// Name : ToggleVisible()
// Desc :
// ----------------------------------------------------------------------------
void CUIMap::ToggleVisible()
{
	UpdateMap(false);
}

// ----------------------------------------------------------------------------
// Name : ManipulateNpcScroll()
// Desc :
// ----------------------------------------------------------------------------
void CUIMap::ManipulateNpcScroll(BOOL IsNpcScrollVisible)
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	extern SLONG	g_slZone;
	
	// 현재 던전의 경우에는 지도를 표시하지 않는다.
	if( CZoneInfo::getSingleton()->GetZoneType( g_slZone ) == ZONE_SDUNGEON )
	{
		if( IsVisible() )
			pUIManager->RearrangeOrder( UI_MAP, FALSE );

		return;
	}

	if( IsVisible() || pUIManager->GetNpcScroll()->IsOpened())
	{
		g_iOpacityInMap = m_slbOpacity.GetCurPos();
		if( m_bShowWorldMap )
			g_iZoomInMap = m_slbZoomWorld.GetCurPos();
		else
			g_iZoomInMap = m_slbZoomDetail.GetCurPos();

		g_iXPosInMap = GetPosX();
		g_iYPosInMap = GetPosY();
	}

	m_bShowWorldMap = FALSE;
	// !! 공성전 Map표시 조건에 맞다면 서버로 정보 요청 
	//
	if( IsNpcScrollVisible )
	{
		pUIManager->RearrangeOrder( UI_MAP, IsNpcScrollVisible);
		IsDetailMap(TRUE);
		m_btnGoTop.SetEnable(TRUE);
		if (m_nCurMapData > -1)
		{
			if (IsPossibleCastleMap() && IsCastleMap( m_nCurMapData ))
			{
				ReSetData();
				_pNetwork->SendCastleMapRecent();
			}
		}
	}
	else
	{
		if (IsEnabled() == TRUE)
			UpdateMap(false);

		m_qSignalData.clear();
	}

	ClearPlayerEntity();
}


// ----------------------------------------------------------------------------
// Name : MakeCenterForZoom()
// Desc :
// ----------------------------------------------------------------------------
void CUIMap::ResizeMapFrame( BOOL bSign )
{
	if( m_bShowWorldMap )
	{
		if( bSign )
			m_slbZoomWorld.SetCurPos( m_slbZoomWorld.GetCurPos() + 1 );
		else
			m_slbZoomWorld.SetCurPos( m_slbZoomWorld.GetCurPos() - 1 );
		
		UpdateZoomSlideInfo( TRUE );
		UpdateZoomRatio( TRUE );
			
		m_nWidth = (m_nMapWorldBaseWidth * (m_fZoomWorld > 1.0f ? 1.0f : m_fZoomWorld));
		m_nHeight = (m_nMapWorldBaseHeight * (m_fZoomWorld > 1.0f ? 1.0f : m_fZoomWorld));
	}
	else
	{
		if( bSign )
			m_slbZoomDetail.SetCurPos( m_slbZoomDetail.GetCurPos() + 1 );
		else
			m_slbZoomDetail.SetCurPos( m_slbZoomDetail.GetCurPos() - 1 );
		
		UpdateZoomSlideInfo( FALSE );
		UpdateZoomRatio( FALSE );
		
		m_nWidth = (m_nMapDetailBaseWidth * (m_fZoomDetail > 1.0f ? 1.0f : m_fZoomDetail));
		m_nHeight = (m_nMapDetailBaseHeight * (m_fZoomDetail > 1.0f ? 1.0f : m_fZoomDetail));
	}

	ResizeMapFrame( 5, 0, 0 );
}

// ----------------------------------------------------------------------------
// Name : SetMapData()
// Desc :
// ----------------------------------------------------------------------------
void CUIMap::SetMapData()
{

	SetPos( g_iXPosInMap, g_iYPosInMap );

	m_nWidth = m_nMapWorldBaseWidth;
	m_nHeight = m_nMapWorldBaseHeight;
	m_fZoomWorld = m_fZoomDetail = 1.0f;
	m_btnClose.SetPos( 486, 5 );

#ifdef NEW_WORLD_MAP		// [2012/10/11 : Sora] 월드맵 개편
	m_slbOpacity.SetCurPos( 75 );
	m_slbZoomDetail.SetCurPos( 12 );
	m_slbZoomWorld.SetCurPos( 12 );
#else
	m_slbOpacity.SetCurPos( g_iOpacityInMap );
	m_slbZoomDetail.SetCurPos( g_iZoomInMap );
	m_slbZoomWorld.SetCurPos( g_iZoomInMap );
#endif

	if( !m_bShowWorldMap )
	{
		UpdateZoomSlideInfo( TRUE );
		UpdateZoomRatio( TRUE );
		m_nWidth = (m_nMapWorldBaseWidth * (m_fZoomWorld > 1.0f ? 1.0f : m_fZoomWorld));
		m_nHeight = (m_nMapWorldBaseHeight * (m_fZoomWorld > 1.0f ? 1.0f : m_fZoomWorld));
	}
	else
	{	
		UpdateZoomSlideInfo( FALSE );
		UpdateZoomRatio( FALSE );
		m_nWidth = (m_nMapDetailBaseWidth * (m_fZoomDetail > 1.0f ? 1.0f : m_fZoomDetail));
		m_nHeight = (m_nMapDetailBaseHeight * (m_fZoomDetail > 1.0f ? 1.0f : m_fZoomDetail));
	}

	ResizeMapFrame( 5, 0, 0 );
	UpdateOpacitySlideInfo();
	UpdateOpacityRatio();
}

// ----------------------------------------------------------------------------
// Name : MakeCenterForZoom()
// Desc :
// ----------------------------------------------------------------------------
void CUIMap::MakeCenterForZoom( BOOL bWorldMap )
{
	if( bWorldMap )
	{
		m_nZoomCenterX = m_rcMapWorld.Left;// + m_rcMapWorld.GetWidth() / 2;
		m_nZoomCenterY = m_rcMapWorld.Top;// + m_rcMapWorld.GetHeight() / 2;

	}
	else
	{
		m_nZoomCenterX = m_rcMapDetail.Left;// + m_rcMapDetail.GetWidth() / 2;
		m_nZoomCenterY = m_rcMapDetail.Top;// + m_rcMapDetail.GetHeight() / 2;

	}
}

// ----------------------------------------------------------------------------
// Name : ZoomMap()
// Desc :
// ----------------------------------------------------------------------------
void CUIMap::ZoomMap( BOOL bWorldMap )
{
// 	if( bWorldMap )
// 	{
// 		int	nHalfW = m_nMapWorldBaseWidth / m_fZoomWorld * 0.5f;
// 		int	nHalfH = m_nMapWorldBaseHeight / m_fZoomWorld * 0.5f;
// 		m_rcMapWorld.SetRect( m_nZoomCenterX - nHalfW, m_nZoomCenterY - nHalfH,
// 								m_nZoomCenterX + nHalfW, m_nZoomCenterY + nHalfH );
// 		AdjustViewport( 6 );
// 	}
// 	else
// 	{
// 		int	nHalfW = m_nMapDetailBaseWidth / m_fZoomDetail * 0.5f;
// 		int	nHalfH = m_nMapDetailBaseHeight / m_fZoomDetail * 0.5f;
// 		m_rcMapDetail.SetRect( m_nZoomCenterX - nHalfW, m_nZoomCenterY - nHalfH,
// 								m_nZoomCenterX + nHalfW, m_nZoomCenterY + nHalfH );
// 		AdjustViewport( 6 );
// 	}

	if( bWorldMap )
	{
		int	nfW = m_nMapWorldBaseWidth / m_fZoomWorld;
		int	nfH = m_nMapWorldBaseHeight / m_fZoomWorld;
		m_rcMapWorld.SetRect( m_nZoomCenterX, m_nZoomCenterY,
								m_nZoomCenterX + nfW, m_nZoomCenterY + nfH );
		AdjustViewport( 6 );
	}
	else
	{
		int	nfW = m_nMapDetailBaseWidth / m_fZoomDetail;
		int	nfH = m_nMapDetailBaseHeight / m_fZoomDetail;
		m_rcMapDetail.SetRect( m_nZoomCenterX, m_nZoomCenterY,
								m_nZoomCenterX + nfW, m_nZoomCenterY + nfH );
		AdjustViewport( 6 );
	}
}

// ----------------------------------------------------------------------------
// Name : IsInsideMap()
// Desc :
// ----------------------------------------------------------------------------
BOOL CUIMap::IsInsideMap( BOOL bWinPos, BOOL bWorldMap, int nX, int nY, BOOL bZoomRect, UIRect &rcRect )
{
	UIRect rcTemp = rcRect;	
	
	if( bWinPos )
	{
		nX -= m_nPosX + MAP_SX;
		nY -= m_nPosY + MAP_SY;
	}

	if( bWorldMap )
	{
		nX -= m_rcViewportWorld.Left;
		nY -= m_rcViewportWorld.Top;
		nX /= m_fZoomWorld;
		nY /= m_fZoomWorld;
		nX += m_rcMapWorld.Left;
		nY += m_rcMapWorld.Top;

		if( nX < m_rcMapWorld.Left || nX > m_rcMapWorld.Right ||
			nY < m_rcMapWorld.Top || nY > m_rcMapWorld.Bottom )
			return FALSE;

		if( bZoomRect )
		{
			int	nWidth = rcTemp.GetWidth() / m_fZoomWorld;
			int	nHeight = rcTemp.GetHeight() / m_fZoomWorld;
			int	nCX = rcTemp.GetCenterX();
			int	nCY = rcTemp.GetCenterY();

			rcTemp.Left = nCX - nWidth / 2;
			rcTemp.Right = rcTemp.Left + nWidth;
			rcTemp.Top = nCY - nHeight / 2;
			rcTemp.Bottom = rcTemp.Top + nHeight;
		}

		if( nX < rcTemp.Left || nX > rcTemp.Right ||
			nY < rcTemp.Top || nY > rcTemp.Bottom )
			return FALSE;
	}
	else
	{
		nX -= m_rcViewportDetail.Left;
		nY -= m_rcViewportDetail.Top;
		nX /= m_fZoomDetail;
		nY /= m_fZoomDetail;
		nX += m_rcMapDetail.Left;
		nY += m_rcMapDetail.Top;

		if( nX < m_rcMapDetail.Left || nX > m_rcMapDetail.Right ||
			nY < m_rcMapDetail.Top || nY > m_rcMapDetail.Bottom )
			return FALSE;

		if( bZoomRect )
		{
			int	nWidth = rcTemp.GetWidth() / m_fZoomDetail;
			int	nHeight = rcTemp.GetHeight() / m_fZoomDetail;
			int	nCX = rcTemp.GetCenterX();
			int	nCY = rcTemp.GetCenterY();

			rcTemp.Left = nCX - nWidth / 2;
			rcTemp.Right = rcTemp.Left + nWidth;
			rcTemp.Top = nCY - nHeight / 2;
			rcTemp.Bottom = rcTemp.Top + nHeight;
		}

		if( nX < rcTemp.Left || nX > rcTemp.Right ||
			nY < rcTemp.Top || nY > rcTemp.Bottom )
			return FALSE;
	}

	return TRUE;
}

// ----------------------------------------------------------------------------
// Name : UpdateOpacityRatio()
// Desc :
// ----------------------------------------------------------------------------
void CUIMap::UpdateOpacityRatio()
{
	m_fMapOpacity = ( 25 + m_slbOpacity.GetCurPos() ) / 100.0f;
}

// ----------------------------------------------------------------------------
// Name : UpdateOpacitySlideInfo()
// Desc :
// ----------------------------------------------------------------------------
void CUIMap::UpdateOpacitySlideInfo()
{
	m_bShowOpacitySlideInfo = TRUE;
	m_bShowZoomSlideInfo = FALSE;
#ifdef NEW_WORLD_MAP		// [2012/10/11 : Sora] 월드맵 개편
	m_strOpacitySlideInfo.PrintF("%d%%", 25 + m_slbOpacity.GetCurPos());
#else
	m_strOpacitySlideInfo.PrintF( _S( 1035, "투명 %d%%" ), ( 25 + m_slbOpacity.GetCurPos() ) );
	m_nOpacityStringWidth = m_strOpacitySlideInfo.Length() * ( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );
#endif
}

// ----------------------------------------------------------------------------
// Name : UpdateZoomSlideInfo()
// Desc :
// ----------------------------------------------------------------------------
void CUIMap::UpdateZoomSlideInfo( BOOL bWorldMap )
{
	m_bShowOpacitySlideInfo = FALSE;
	m_bShowZoomSlideInfo = TRUE;

	if( bWorldMap )
	{
		if( m_slbZoomWorld.GetCurPos() < MAP_ZOOM_SLIDE_ORIGIN )
			m_strZoomSlideInfo.PrintF( _S( 1036, "크기 %d%%" ), 100 - ( MAP_ZOOM_SLIDE_ORIGIN - m_slbZoomWorld.GetCurPos() ) * 5 );
		else
			m_strZoomSlideInfo.PrintF( _S( 1036, "크기 %d%%" ), 100 + ( m_slbZoomWorld.GetCurPos() - MAP_ZOOM_SLIDE_ORIGIN ) * 25 );
	}
	else
	{
		if( m_slbZoomDetail.GetCurPos() < MAP_ZOOM_SLIDE_ORIGIN )
			m_strZoomSlideInfo.PrintF( _S( 1036, "크기 %d%%" ), 100 - ( MAP_ZOOM_SLIDE_ORIGIN - m_slbZoomDetail.GetCurPos() ) * 5 );
		else
			m_strZoomSlideInfo.PrintF( _S( 1036, "크기 %d%%" ), 100 + ( m_slbZoomDetail.GetCurPos() - MAP_ZOOM_SLIDE_ORIGIN ) * 25 );
	}

	m_nZoomStringWidth = m_strZoomSlideInfo.Length() * ( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );
}

// ----------------------------------------------------------------------------
// Name : UpdateZoomRatio()
// Desc :
// ----------------------------------------------------------------------------
void CUIMap::UpdateZoomRatio( BOOL bWorldMap )
{
	if( bWorldMap )
	{
		if( m_slbZoomWorld.GetCurPos() < MAP_ZOOM_SLIDE_ORIGIN )
			m_fZoomWorld = (FLOAT)( 100 - ( MAP_ZOOM_SLIDE_ORIGIN - m_slbZoomWorld.GetCurPos() ) * 5 ) / 100.0f;
		else
			m_fZoomWorld = (FLOAT)( 100 + ( m_slbZoomWorld.GetCurPos() - MAP_ZOOM_SLIDE_ORIGIN ) * 25 ) / 100.0f;
	}
	else
	{
		if( m_slbZoomDetail.GetCurPos() < MAP_ZOOM_SLIDE_ORIGIN )
			m_fZoomDetail = (FLOAT)( 100 - ( MAP_ZOOM_SLIDE_ORIGIN - m_slbZoomDetail.GetCurPos() ) * 5 ) / 100.0f;
		else
			m_fZoomDetail = (FLOAT)( 100 + ( m_slbZoomDetail.GetCurPos() - MAP_ZOOM_SLIDE_ORIGIN ) * 25 ) / 100.0f;
	}

	ZoomMap( bWorldMap );
}

// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUIMap::MouseMessage( MSG *pMsg )
{
	WMSG_RESULT	wmsgResult;

	// Title bar
	static BOOL bTitleBarClick = FALSE;
	// Resize bar
	static int	nResizeBarClick = 0;	// 0: None, 1:Left, 2:Right, 3:Middle bottom, 4:Left bottom, 5:Right bottom
	// Scroll world map
	static BOOL	bScrollWorldMap = FALSE;
	// Scroll detail map
	static BOOL	bScrollDetailMap = FALSE;
	static BOOL bScrolledDetailMap = FALSE;
	// Detail map click
	static BOOL	bDetailMapClick = FALSE;

	// ignore detail map click event.
	BOOL	bIgnoreDetailMapClick = FALSE;

	// Mouse point
	static int	nOldX, nOldY;
	int	nX = LOWORD( pMsg->lParam );
	int	nY = HIWORD( pMsg->lParam );

	// Mouse message
	switch( pMsg->message )
	{
	case WM_MOUSEMOVE:
		{
			// Slide info
			m_bShowOpacitySlideInfo = FALSE;
			m_bShowZoomSlideInfo = FALSE;

			if( IsInside( nX, nY ) )
			{
				m_bInsideMouse = true;
				if( nResizeBarClick == 0 && !bTitleBarClick && !bScrollWorldMap && !bScrollDetailMap )
				{
					if( m_bFrameBlending )
					{
						if( m_bShowFrame )
						{
							m_llFrameBlendElapsedTime = MAP_FRAME_BLENDTIME - m_llFrameBlendElapsedTime;
							m_bShowFrame = FALSE;
						}
					}
					else if( !m_bShowFrame )
					{
						m_llFrameBlendElapsedTime = 0;
						m_llFrameBlendTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();
						m_bFrameBlending = TRUE;
					}
				}

			}
			else
			{
				// 컨트롤 밖으로 나가면 무조건 취소
				SetSignalOn( false );
				m_bInsideMouse = false;
				if( nResizeBarClick == 0 && !bTitleBarClick && !bScrollWorldMap && !bScrollDetailMap )
				{
					if( m_bFrameBlending )
					{
						if( !m_bShowFrame )
						{
							m_llFrameBlendElapsedTime = MAP_FRAME_BLENDTIME - m_llFrameBlendElapsedTime;
							m_bShowFrame = TRUE;
						}
					}
					else if( m_bShowFrame )
					{
						m_llFrameBlendElapsedTime = 0;
						m_llFrameBlendTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();
						m_bFrameBlending = TRUE;
					}
				}
		
			}

			if( m_bShowFrame )
			{
				// 시그널이 온인 경우네는 다른 모든 마우스 커서 컨트롤 무시
				if( m_bSignalBtnOn ) 
				{
					SetSignalOn( true );
					return WMSG_SUCCESS;
				}

				CUIManager* pUIManager = CUIManager::getSingleton();

#ifndef NEW_WORLD_MAP		// [2012/10/11 : Sora] 월드맵 개편
				// Resize map
				if( nResizeBarClick != 0 && ( pMsg->wParam & MK_LBUTTON ) )
				{
					pUIManager->SetMouseCursorInsideUIs( UMCT_SIZE );

					int	ndX = nX - nOldX;
					int	ndY = nY - nOldY;
					nOldX = nX;	nOldY = nY;

					if( ndX > 5 || ndY > 5 )
						ResizeMapFrame( TRUE );
					else if( ndX < -5 || ndX < -5 )
						ResizeMapFrame( FALSE );

					return WMSG_SUCCESS;
				}

				pUIManager->SetMouseCursorInsideUIs();
#endif

				// Move map
				if( bTitleBarClick && ( pMsg->wParam & MK_LBUTTON ) )
				{
					int	ndX = nX - nOldX;
					int	ndY = nY - nOldY;
					nOldX = nX;	nOldY = nY;

					Move( ndX, ndY );

					return WMSG_SUCCESS;
				}
				// Close button
				if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
				if( m_btnSignal.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
				// Opacity slide bar
				if( ( wmsgResult = m_slbOpacity.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					UpdateOpacitySlideInfo();
					if( wmsgResult == WMSG_COMMAND )
						UpdateOpacityRatio();

					return WMSG_SUCCESS;
				}
				// World zoom slide bar
				if( m_bShowWorldMap && ( wmsgResult = m_slbZoomWorld.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					UpdateZoomSlideInfo( TRUE );
					if( wmsgResult == WMSG_COMMAND )
					{
						UpdateZoomRatio( TRUE );
						m_nWidth = (m_nMapWorldBaseWidth * ( m_fZoomWorld > 1.0f ? 1.0f : m_fZoomWorld ) );
						m_nHeight = (m_nMapWorldBaseHeight * ( m_fZoomWorld > 1.0f ? 1.0f : m_fZoomWorld ) );
						ResizeMapFrame( 5, 0, 0 );
					}

					return WMSG_SUCCESS;
				}
				// Detail zoom slide bar
				if( !m_bShowWorldMap && ( wmsgResult = m_slbZoomDetail.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					UpdateZoomSlideInfo( FALSE );
					if( wmsgResult == WMSG_COMMAND )
					{
						UpdateZoomRatio( FALSE );
						m_nWidth = (m_nMapDetailBaseWidth * (m_fZoomDetail > 1.0f ? 1.0f : m_fZoomDetail));
						m_nHeight = (m_nMapDetailBaseHeight * (m_fZoomDetail > 1.0f ? 1.0f : m_fZoomDetail));
 						ResizeMapFrame( 5, 0, 0 );
					}

					return WMSG_SUCCESS;
				}
				// Map list combo box
				if( m_cmbMap.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;

#ifdef NEW_WORLD_MAP		// [2012/10/11 : Sora] 월드맵 개편
				if( m_btnGoTop.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
#else
				// Resize region
				if( ( IsInsideRect( nX, nY, m_rcResizeLeft ) || IsInsideRect( nX, nY, m_rcResizeRight ) ||
					IsInsideRect( nX, nY, m_rcResizeBottomM ) || IsInsideRect( nX, nY, m_rcResizeBottomL ) ||
					IsInsideRect( nX, nY, m_rcResizeBottomR ) ) && !pUIManager->IsInsideUpperUIs( UI_MAP, nX, nY ) )
				{
					pUIManager->SetMouseCursorInsideUIs( UMCT_SIZE );
					return WMSG_SUCCESS;
				}
#endif

				if( !pUIManager->IsInsideUpperUIs( UI_MAP, nX, nY ) )
				{
					// Tool tip
					UIRect	rcTip;
					int		nTipX = 0, nTipZ = 0;
					FLOAT	fTipX, fTipZ;
					BOOL	bShowToolTip = FALSE;
					CMobData* pMobData = NULL;

					if (m_nCurMapData < 0 && m_vecMapData.size() >= m_nCurMapData)
						return WMSG_FAIL;

					for( int i = 0; i < m_vecMapData[m_nCurMapData].vecNpc.size(); i++ )
					{
						pMobData = CMobData::getData(m_vecMapData[m_nCurMapData].vecNpc[i].nIndex);

						if (pMobData != NULL)
						{
							if (pMobData->IsChannelFlag(_pNetwork->m_iServerCh) == false)
								continue;
						}

						if( CZoneInfo::getSingleton()->GetZoneType( m_vecMapData[m_nCurMapData].World.nZoneIndex ) == ZONE_DUNGEON )
						{
							if( m_vecMapData[m_nCurMapData].World.sbLayer != m_vecMapData[m_nCurMapData].vecNpc[i].nYLayer )
							{
								continue;
							}
						}

						if (IS_EVENT_ON(A_EVENT_HOLLOWEEN) == 0)
						{
							if (m_vecMapData[m_nCurMapData].vecNpc[i].nIndex == 454 ||
								m_vecMapData[m_nCurMapData].vecNpc[i].nIndex == 455)
								continue;
						}
							
						fTipX = m_vecMapData[m_nCurMapData].vecNpc[i].fX;
						fTipZ = m_vecMapData[m_nCurMapData].vecNpc[i].fZ;
						if( m_bShowWorldMap )
						{
							// revision map size. [10/20/2009 rumist]							
							fTipX -= m_vecMapData[m_nCurMapData].World.lOffsetX;
							fTipZ -= m_vecMapData[m_nCurMapData].World.lOffsetZ;
							
							fTipX *= m_fMapSizeRevision;
							fTipZ *= m_fMapSizeRevision;

							nTipX = fTipX * m_vecMapData[m_nCurMapData].World.fRatio;
							nTipZ = fTipZ * m_vecMapData[m_nCurMapData].World.fRatio;
						}
						else
						{
							if (m_nCurDetail >= 0 && m_nCurDetail < m_vecMapData[m_nCurMapData].vecDetail.size() )	// 방어코드 추가.
							{
								fTipX -= m_vecMapData[m_nCurMapData].vecDetail[m_nCurDetail].fOffsetX;
								fTipZ -= m_vecMapData[m_nCurMapData].vecDetail[m_nCurDetail].fOffsetZ;
								nTipX = fTipX * m_vecMapData[m_nCurMapData].vecDetail[m_nCurDetail].fRatio;
								nTipZ = fTipZ * m_vecMapData[m_nCurMapData].vecDetail[m_nCurDetail].fRatio;
							}
						}

						rcTip = m_rcNPC;
						rcTip.Offset( nTipX, nTipZ );
					
						CUIMapOption* pOption = UIMGR()->GetRadar()->GetMapOption();
						bool bShow = false;
						int nIndex = m_vecMapData[m_nCurMapData].vecNpc[i].nIndex;

						if (pOption->IsCheck(eNT_QUEST_Q) &&
							CQuestSystem::Instance().TestNPCForQuest(nIndex) == CQuestSystem::NQT_HAVE_QUEST)
						{
							bShow = true;
						}
						else if (pOption->IsCheck(eNT_QUEST_A) &&
							CQuestSystem::Instance().TestNPCForQuest(nIndex) == CQuestSystem::NQT_CAN_PRIZE)
						{
							bShow = true;
						}
						else
						{
							for (int j = eNT_SHOP; j < eNT_MONSTER; ++j)
							{
								if (pOption->IsCheck((eNPC_TYPE)j) == false)
									continue;

								if (pOption->IsFlag(j, nIndex) != FALSE)
								{
									bShow = true;
									break;
								}
							}
						}

						if (bShow == true && IsInsideMap( TRUE, m_bShowWorldMap, nX, nY, TRUE, rcTip ))
						{
							bShowToolTip = TRUE;
							CTString	strNpcName = CMobData::getData(nIndex)->GetName();
							ShowToolTip( TRUE, strNpcName, rcTip.GetCenterX(), rcTip.GetCenterY() );
							break;
						}
					}
					if( !bShowToolTip )
					{
						for( int i = 0; i < m_vecMapData[m_nCurMapData].vecSubZone.size(); i++ )
						{
							SBYTE	sbType = m_vecMapData[m_nCurMapData].vecSubZone[i].sbType;
							fTipX = m_vecMapData[m_nCurMapData].vecSubZone[i].fX;
							fTipZ = m_vecMapData[m_nCurMapData].vecSubZone[i].fZ;	
							if( m_bShowWorldMap )
							{
								if( m_vecMapData[m_nCurMapData].World.nZoneIndex != ZONE_WORLD_MAP )
								{
									// revision map size. [10/20/2009 rumist]
									fTipX *= m_fMapSizeRevision;
									fTipZ *= m_fMapSizeRevision;
									nTipX = fTipX * m_vecMapData[m_nCurMapData].World.fRatio;
									nTipZ = fTipZ * m_vecMapData[m_nCurMapData].World.fRatio;
								}
								else
								{
									nTipX = fTipX;
									nTipZ = fTipZ;
								}

							}
							else
							{
								if (m_nCurDetail >= 0 && m_nCurDetail < m_vecMapData[m_nCurMapData].vecDetail.size() )	// 방어코드 추가.
								{
									fTipX -= m_vecMapData[m_nCurMapData].vecDetail[m_nCurDetail].fOffsetX;
									fTipZ -= m_vecMapData[m_nCurMapData].vecDetail[m_nCurDetail].fOffsetZ;
									nTipX = fTipX * m_vecMapData[m_nCurMapData].vecDetail[m_nCurDetail].fRatio;
									nTipZ = fTipZ * m_vecMapData[m_nCurMapData].vecDetail[m_nCurDetail].fRatio;
								}
							}

							rcTip = m_rcSubZone[sbType];
							rcTip.Offset( nTipX, nTipZ );
							if( IsInsideMap( TRUE, m_bShowWorldMap, nX, nY, TRUE, rcTip ) )
							{
								CTString	strTemp;
								if( m_vecMapData[m_nCurMapData].World.nZoneIndex != ZONE_WORLD_MAP )
								{
									if( sbType == CHARGE_PRIVATE || sbType == MINE_PRIVATE || sbType == GATHER_PRIVATE )
									{
										strTemp.PrintF( _S( 549, "%s(사유지)" ),			
														CZoneInfo::getSingleton()->GetExtraName( m_vecMapData[m_nCurMapData].World.nZoneIndex,
																					m_vecMapData[m_nCurMapData].vecSubZone[i].nSubZoneIndex ) );
									}
									else
									{
										strTemp = CZoneInfo::getSingleton()->GetExtraName( m_vecMapData[m_nCurMapData].World.nZoneIndex,
																			m_vecMapData[m_nCurMapData].vecSubZone[i].nSubZoneIndex );
									}

								}
								else
								{
									strTemp = CZoneInfo::getSingleton()->GetZoneName( m_vecMapData[m_nCurMapData].vecSubZone[i].nSubZoneIndex );
								}

								bShowToolTip = TRUE;
								ShowToolTip( TRUE, strTemp, rcTip.GetCenterX(), rcTip.GetCenterY() );
								break;
							}
						}
					}
					if( !bShowToolTip )
						ShowToolTip( FALSE, CTString( "" ) );

					// Detail map region
					if( m_bShowWorldMap /*&& _pNetwork->MyCharacterInfo.sbAttributePos != ATTC_PEACE*/ )
					{
						for( int i = 0; i < m_vecMapData[m_nCurMapData].vecDetail.size(); i++ )
						{
							UIRect	rcTemp = m_vecMapData[m_nCurMapData].vecDetail[i].rcSelRegion;
							rcTemp.Scale( m_fMapSizeRevision );
							if( IsInsideMap( TRUE, TRUE, nX, nY, FALSE, rcTemp ) )
							{
								pUIManager->SetMouseCursorInsideUIs( UMCT_ZOOMIN );
								return WMSG_SUCCESS;
							}
						}
					}
				}
			}
		}
		break;

	case WM_LBUTTONDOWN:
		{
#ifndef NEW_WORLD_MAP		// [2012/10/11 : Sora] 월드맵 개편
			if( m_bShowFrame )
			{
				CUIManager* pUIManager = CUIManager::getSingleton();

				if( !m_bSignalBtnOn ) // 시그널이 아닌경우에만 가능 
				{
					// Resize region - left
					if( IsInsideRect( nX, nY, m_rcResizeLeft ) )
					{
						nOldX = nX;		nOldY = nY;
						nResizeBarClick = 1;
						m_nResizeWidthOffset = 0;
						m_nResizeHeightOffset = 0;
					
						pUIManager->SetMouseCursorInsideUIs( UMCT_SIZE );
						pUIManager->RearrangeOrder( UI_MAP, TRUE );
						return WMSG_SUCCESS;
					}

					// Resize region - right
					if( IsInsideRect( nX, nY, m_rcResizeRight ) )
					{
						nOldX = nX;		nOldY = nY;
						nResizeBarClick = 2;
						m_nResizeWidthOffset = 0;
						m_nResizeHeightOffset = 0;

						pUIManager->SetMouseCursorInsideUIs( UMCT_SIZE );
						pUIManager->RearrangeOrder( UI_MAP, TRUE );
						return WMSG_SUCCESS;
					}

					// Resize region - middle bottom
					if( IsInsideRect( nX, nY, m_rcResizeBottomM ) )
					{
						nOldX = nX;		nOldY = nY;
						nResizeBarClick = 3;
						m_nResizeWidthOffset = 0;
						m_nResizeHeightOffset = 0;

						pUIManager->SetMouseCursorInsideUIs( UMCT_SIZE );
						pUIManager->RearrangeOrder( UI_MAP, TRUE );
						return WMSG_SUCCESS;
					}

					// Resize region - left bottom
					if( IsInsideRect( nX, nY, m_rcResizeBottomL ) )
					{
						nOldX = nX;		nOldY = nY;
						nResizeBarClick = 4;
						m_nResizeWidthOffset = 0;
						m_nResizeHeightOffset = 0;

						pUIManager->SetMouseCursorInsideUIs( UMCT_SIZE );
						pUIManager->RearrangeOrder( UI_MAP, TRUE );
						return WMSG_SUCCESS;
					}
				}

				// Resize region - right bottom
				if( IsInsideRect( nX, nY, m_rcResizeBottomR ) )
				{
					nOldX = nX;		nOldY = nY;
					nResizeBarClick = 5;
					m_nResizeWidthOffset = 0;
					m_nResizeHeightOffset = 0;

					pUIManager->SetMouseCursorInsideUIs( UMCT_SIZE );
					pUIManager->RearrangeOrder( UI_MAP, TRUE );
					return WMSG_SUCCESS;
				}
			}
#endif
			// 바로 가기 기능
			if (_pNetwork->m_ubGMLevel > 1)
			{
				if( IsInside( nX, nY ) )
				{
					if (pMsg->wParam & MK_CONTROL)
					{
						if (Goto(nX - m_nPosX, nY - m_nPosY) == true)
							return WMSG_SUCCESS;
					}
				}
			}

			if( IsInside( nX, nY ) )
			{
				bDetailMapClick = FALSE;
				nOldX = nX;		nOldY = nY;

				CUIManager* pUIManager = CUIManager::getSingleton();

				if( m_bShowFrame )
				{
					// Close button
					if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
					{
						// Nothing
					}
					else if( m_btnSignal.MouseMessage( pMsg ) != WMSG_FAIL )
					{
						
					}
					// Opacity slide bar
					else if( ( wmsgResult = m_slbOpacity.MouseMessage( pMsg ) ) != WMSG_FAIL )
					{
						UpdateOpacitySlideInfo();
						if( wmsgResult == WMSG_COMMAND )
							UpdateOpacityRatio();

						return WMSG_SUCCESS;
					}
					// World zoom slide bar
					else if( m_bShowWorldMap && ( wmsgResult = m_slbZoomWorld.MouseMessage( pMsg ) ) != WMSG_FAIL )
					{
						UpdateZoomSlideInfo( TRUE );
						MakeCenterForZoom( TRUE );
						if( wmsgResult == WMSG_COMMAND )
						{
							UpdateZoomRatio( TRUE );
							m_nResizeWidthOffset = 0;
							m_nResizeHeightOffset = 0;
							m_nWidth = (m_nMapWorldBaseWidth * ( m_fZoomWorld > 1.0f ? 1.0f : m_fZoomWorld ) );
							m_nHeight = (m_nMapWorldBaseHeight * ( m_fZoomWorld > 1.0f ? 1.0f : m_fZoomWorld ) );
							ResizeMapFrame( 5, 0, 0 );
						}

						return WMSG_SUCCESS;
					}
					// Detail zoom slide bar
					else if( !m_bShowWorldMap && ( wmsgResult = m_slbZoomDetail.MouseMessage( pMsg ) ) != WMSG_FAIL )
					{
						UpdateZoomSlideInfo( FALSE );
						MakeCenterForZoom( FALSE );
						if( wmsgResult == WMSG_COMMAND )
						{
							UpdateZoomRatio( FALSE );
							m_nResizeWidthOffset = 0;
							m_nResizeHeightOffset = 0;
							m_nWidth = (m_nMapDetailBaseWidth * (m_fZoomDetail > 1.0f ? 1.0f : m_fZoomDetail));
							m_nHeight = (m_nMapDetailBaseHeight * (m_fZoomDetail > 1.0f ? 1.0f : m_fZoomDetail));
							ResizeMapFrame( 5, 0, 0 );
						}

						return WMSG_SUCCESS;
					}
					// Map combo box
					else if( ( wmsgResult = m_cmbMap.MouseMessage( pMsg ) ) != WMSG_FAIL )
					{
						if( wmsgResult == WMSG_COMMAND )
							ChangeMap();
						
					}
					// Title bar
					else if( IsInsideRect( nX, nY, m_rcTitle ) )
					{
						bTitleBarClick = TRUE;
					}
#ifdef NEW_WORLD_MAP		// [2012/10/11 : Sora] 월드맵 개편
					else if( m_btnGoTop.MouseMessage( pMsg ) != WMSG_FAIL )
					{

					}
#endif
					else
					{
						int	nNewX = nX - MAP_SX;
						int	nNewY = nY - MAP_SY;
						if( m_bShowWorldMap )
						{
							if( IsInsideRect( nNewX, nNewY, m_rcViewportWorld ) )
							{
								if( m_bSignalBtnOn )
								{
									if( m_qSignalData.size() < MAX_SIGNAL )
									{
										FLOAT fX = nNewX - m_nPosX; 
										FLOAT fY = nNewY - m_nPosY;
									
										MapToWorld( fX, fY );
										//AddSignal( fX, fY, -1, -1 );
										SendSignal( fX, fY );
									}
																	
									if( pMsg->wParam != MK_CONTROL )
									{
										SetSignalOn( false );
									}
									return WMSG_SUCCESS;
								}

								// Open detail map
								for( int i = 0; i < m_vecMapData[m_nCurMapData].vecDetail.size(); i++ )
								{
									UIRect rcTemp = m_vecMapData[m_nCurMapData].vecDetail[i].rcSelRegion;
									rcTemp.Scale( m_fMapSizeRevision );
									if( IsInsideMap( TRUE, TRUE, nX, nY, FALSE, rcTemp ) )
									{
										bDetailMapClick = TRUE;
										pUIManager->RearrangeOrder( UI_MAP, TRUE );
								
									
										return WMSG_SUCCESS;
									}
								}
							
								// Scroll world map
								if( m_bCanScrollMapWorld )
								{
									bScrollWorldMap = TRUE;
									pUIManager->SetMouseCursorInsideUIs( UMCT_PICK );
								}
							}
						}
						else
						{
							// connie [2009/10/19] - 
							if( !(_pNetwork->MyCharacterInfo.sbAttributePos & MATT_PEACE) && !IsPlayerInPeaceArea())
							{
								if( IsInsideRect( nNewX, nNewY, m_rcViewportDetail ) )
								{
									// Scroll detail map
									bScrolledDetailMap = FALSE;
									if( m_bCanScrollMapDetail )
									{
										pUIManager->SetMouseCursorInsideUIs( UMCT_PICK );
										bScrollDetailMap = TRUE;
									}
								}
							}
							else
							{
								bIgnoreDetailMapClick = TRUE;
							}
						}
					}
					
					if( !bIgnoreDetailMapClick )
					{
						//key가 눌린 후 Control이 아닌경우 모두 취소 
						if( pMsg->wParam != MK_CONTROL )
						{
							SetSignalOn( false );
						}
						pUIManager->RearrangeOrder( UI_MAP, TRUE );
						return WMSG_SUCCESS;
					}
				}
				pUIManager->RearrangeOrder( UI_MAP, TRUE );
				return WMSG_SUCCESS;
			}
			else 
			{
				SetSignalOn( false );
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

				// Resize bar
				nResizeBarClick = 0;

				// Reset cursor
				if( bScrollWorldMap || bScrollDetailMap )
					pUIManager->SetMouseCursorInsideUIs();

				// Scroll world map
				bScrollWorldMap = FALSE;
				m_fMoveElapsedErrXWorld = 0;
				m_fMoveElapsedErrYWorld = 0;

				// Scroll detail map
				bScrollDetailMap = FALSE;
				m_fMoveElapsedErrXDetail = 0;
				m_fMoveElapsedErrYDetail = 0;

				// If map isn't focused
				if( !IsFocused() )
					return WMSG_FAIL;

				if( m_bShowFrame || m_bFrameBlending )
				{
					// Close button
					if( ( wmsgResult = m_btnClose.MouseMessage( pMsg ) ) != WMSG_FAIL )
					{
						if( wmsgResult == WMSG_COMMAND )
							ToggleVisible();
							//pUIManager->RearrangeOrder( UI_MAP, FALSE );
							

						return WMSG_SUCCESS;
					}
					else if( (wmsgResult = m_btnSignal.MouseMessage( pMsg ) ) != WMSG_FAIL )
					{
						if( wmsgResult == WMSG_COMMAND && IsPossibleSignal() )
						{
							SetSignalOn( true );
							pUIManager->RearrangeOrder( UI_MAP, TRUE );
						}

						return WMSG_SUCCESS;
					}
					// Opacity slide bar
					else if( m_slbOpacity.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
					// World zoom slide bar
					else if( m_slbZoomWorld.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
					// Detail zoom slide bar
					else if( m_slbZoomDetail.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
					// Map combo box
					else if( ( wmsgResult = m_cmbMap.MouseMessage( pMsg ) ) != WMSG_FAIL )
						return WMSG_SUCCESS;
#ifdef NEW_WORLD_MAP		// [2012/10/11 : Sora] 월드맵 개편
					else if( ( wmsgResult = m_btnGoTop.MouseMessage( pMsg ) ) != WMSG_FAIL )
					{
						if( wmsgResult == WMSG_COMMAND )
						{
							GoTopMap();
						}
						return WMSG_SUCCESS;
					}
#endif
					else
					{
						int	nNewX = nX - MAP_SX;
						int	nNewY = nY - MAP_SY;
						// Open detail map
						if( m_bShowWorldMap )
						{
							if( ( bDetailMapClick || m_bDetailMapClick ) && IsInsideRect( nNewX, nNewY, m_rcViewportWorld ) )
							{
								for( int i = 0; i < m_vecMapData[m_nCurMapData].vecDetail.size(); i++ )
								{
									UIRect rcTemp = m_vecMapData[m_nCurMapData].vecDetail[i].rcSelRegion;
									rcTemp.Scale( m_fMapSizeRevision );
									if( IsInsideMap( TRUE, TRUE, nX, nY, FALSE, rcTemp ) )
									{
										if( m_vecMapData[m_nCurMapData].World.nZoneIndex == ZONE_WORLD_MAP )
										{
											m_btnGoTop.SetEnable(TRUE);
											SetCurrentWorldMap( zoneNumList[i], 0 );
										}
										else
										{
											bDetailMapClick = FALSE;
											m_bDetailMapClick = FALSE;
											//UpdateZoomRatio(FALSE);
											//UpdateZoomSlideInfo(FALSE);
											//SetMapData();

											int zoom = m_slbZoomWorld.GetCurPos();
											OpenDetailMap( i );
										
											// FIX BUG : change to detail map. [10/6/2009 rumist]
											m_slbZoomDetail.SetCurPos( zoom );
											UpdateZoomSlideInfo( FALSE );
											UpdateZoomRatio( FALSE );
											m_nResizeWidthOffset = 0;
											m_nResizeHeightOffset = 0;
											m_nWidth = (m_nMapDetailBaseWidth * (m_fZoomDetail > 1.0f ? 1.0f : m_fZoomDetail));
											m_nHeight = (m_nMapDetailBaseHeight * (m_fZoomDetail > 1.0f ? 1.0f : m_fZoomDetail));
											ResizeMapFrame( 5, 0, 0 );
										}
										return WMSG_SUCCESS;
									}
								}
							}
						}
						// Close detail map
						else
						{
							// connie [2009/10/19] - 
							if( !(_pNetwork->MyCharacterInfo.sbAttributePos & MATT_PEACE) && !IsPlayerInPeaceArea())
							{
								if( !bScrolledDetailMap &&
									!IsInsideRect( nX, nY, m_rcTitle ) &&
									IsInsideRect( nNewX, nNewY, m_rcViewportDetail ) )
								{
									CloseDetailMap();
									return WMSG_SUCCESS;
								}
							}
						}
					}
				}
			}
			// If holding button exists
			else
			{
				if( IsInside( nX, nY ) )
				{
					// Reset holding button
					pUIManager->ResetHoldBtn();

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

//------------------------------------------------------------------------------
//  CUIMap::KeyMessage
// Explain: Ctrl키를 눌렀을 때 시그널을 보낼 수 있도록 KeyMessage를 받는다 
// * 일반적인 UIWindow는 KEYUP 메세지를 받지 않는다. ( 조합키를 사용하는 곳이 없음)
// *  UIMgr에서 MAP과 Rar만 예외 처리해서 KEYUP메세지를 받도록 하였음
// Date : 2005-10-24,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
WMSG_RESULT CUIMap::KeyMessage( MSG *pMsg )
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

//------------------------------------------------------------------------------
// CUIMap::MapToWorld
// Explain:  지도상의 좌표를 월드 좌표로 변환( Detail Map은 지원하지 않음 )
// Date : 2005-10-20,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIMap::MapToWorld( float& fX, float& fY )
{
	UIRect	rcTempWorld = m_rcMapWorld;
	rcTempWorld.Scale( m_fZoomWorld );
	
	// Viewport에 의한 위치 이동 
	fX = fX - m_rcViewportWorld.Left + rcTempWorld.Left;	
	fY = fY - m_rcViewportWorld.Top + rcTempWorld.Top;

	// 지도에 대한 축척과 지도 Zoom비율
	fX = fX / m_vecMapData[m_nCurMapData].World.fRatio / m_fZoomWorld;	
	fY = fY / m_vecMapData[m_nCurMapData].World.fRatio / m_fZoomWorld;

	// 지도의 Offset~
	fX += m_vecMapData[m_nCurMapData].World.lOffsetX;	
	fY += m_vecMapData[m_nCurMapData].World.lOffsetZ;
}


//------------------------------------------------------------------------------
// CUIMap::WorldToMap
// Explain:  지도상의 좌표를 월드 좌표로 변환( Detail Map은 지원하지 않음 )
// Date : 2005-10-20,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIMap::WorldToMap( float& fX, float& fY )
{
	UIRect	rcTempWorld = m_rcMapWorld;
	rcTempWorld.Scale( m_fZoomWorld );

	// 지도의 Offset~
	fX -= m_vecMapData[m_nCurMapData].World.lOffsetX;	
	fY -= m_vecMapData[m_nCurMapData].World.lOffsetZ;

	// 지도에 대한 축척과 지도 Zoom비율
	fX = fX * m_vecMapData[m_nCurMapData].World.fRatio * m_fZoomWorld;	
	fY = fY * m_vecMapData[m_nCurMapData].World.fRatio * m_fZoomWorld;
	
	// Viewport에 의한 위치 이동 
	fX = fX + m_rcViewportWorld.Left - rcTempWorld.Left;	
	fY = fY + m_rcViewportWorld.Top - rcTempWorld.Top;

}

//------------------------------------------------------------------------------
// CUIMap::InitCastleData
// Explain: 지도상에 표시되는 공성 관련 데이터 초기화 
// * 서버에서 공성 데이터 올때 마다 초기화 한후에 다시 셋팅한다.
// Date : 2005-10-21,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIMap::InitCastleData()
{
	m_vecMapData[m_nCurMapData].vecCastleData.clear();
	ClearAllSignal();
}


//------------------------------------------------------------------------------
// CUIMap::AddCastleData
// Explain: 서버에서 보내주는 공성 관련 데이터 셋팅
// * 현재는 HP는 처리하지 않는다.
// Date : 2005-10-21,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIMap::AddCastleData( LONG nType, FLOAT fX, FLOAT fY, LONG nIndex, LONG nHP, LONG nMaxHP )
{
	CastleData TempCastleData;

	TempCastleData.nType	= nType;
	TempCastleData.fX		= fX;
	TempCastleData.fZ		= fY;
	TempCastleData.nIndex	= nIndex;
	TempCastleData.nHP		= nHP;
	TempCastleData.nMaxHP	= nMaxHP;

	m_vecMapData[m_nCurMapData].vecCastleData.push_back( TempCastleData );
}


//------------------------------------------------------------------------------
// CUIMap::SendSignal
// Explain: 서버에 시그널 좌표를 전송한다. ( 월드 좌표 )
// * Radar Signal 도 이곳을 통해서 전송됨.
// Date : 2005-10-21,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIMap::SendSignal( FLOAT fX, FLOAT fY )
{
	if( m_qSignalData.size() >= MAX_SIGNAL ) return;
	_pNetwork->SendCastleMapSignal( fX, fY, _pNetwork->MyCharacterInfo.lGuildPosition, _pNetwork->MyCharacterInfo.index );
}

//------------------------------------------------------------------------------
// CUIMap::AddSignal
// Explain: 서버에서 시그널 메세지를 받음
// * Radar에도 시그널 메세지를 보내준다.
// Date : 2005-10-21,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIMap::AddSignal( FLOAT fX, FLOAT fY, LONG lSenderFlag, LONG lSenderIndex )
{
	CastleData	TempCastleData;
	CPrintF( "AddSignal(%f, %f)", fX, fY);
	TempCastleData.fX = fX;
	TempCastleData.fZ = fY;
	TempCastleData.nIndex = lSenderIndex;

	TempCastleData.dStartTime = _pTimer->GetHighPrecisionTimer().GetSeconds();
	
	m_qSignalData.push_back( TempCastleData );

	// Date : 2005-11-10(오후 1:30:55), By Lee Ki-hwan 
	// 시그널 사운드 추가 
	PCStartEffectGroup( "snd_signal", _pNetwork->MyCharacterInfo.index );

	CUIManager::getSingleton()->GetRadar()->SetSignal( fX, fY );
}

//------------------------------------------------------------------------------
// CUIMap::ClearAllSignal
// Explain: 지도를 닫을 때는 모든 시그널 메세지를 삭제한다.
// * 다시 지도를 띄웠을 때 최종 시그널 메세지를 표시 하기 때문에 
//   중복 표시 되는 걸 막기 위해서  
// Date : 2005-10-21,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIMap::ClearAllSignal()
{
	m_qSignalData.clear();
}

//------------------------------------------------------------------------------
// CUIMap::IsCastleMap
// Explain: 해당 MAP이 공성맵인지 체크한다.
// * 공성 맵의 조건 
//   1. 던전 맵이 아니다.
//   2. 레이어가 1이상이다. 
//   * 던전의 경우에는 층수에 따라서 레이어가 존재 함 ( 현재는 구현되어 있지 않음) 
// Date : 2005-10-21,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
BOOL CUIMap::IsCastleMap( LONG nMapIndex )
{
#ifdef NEW_WORLD_MAP		// [2012/10/11 : Sora] 월드맵 개편
	if( m_vecMapData[nMapIndex].World.nZoneIndex == ZONE_WORLD_MAP )
		return FALSE;
#endif
	if( CZoneInfo::getSingleton()->GetZoneType( m_vecMapData[nMapIndex].World.nZoneIndex ) == ZONE_DUNGEON ) 
	{
		return FALSE;	
	}
	
	if( m_vecMapData[nMapIndex].World.sbLayer < 1 )
	{
		return FALSE;
	}

	return TRUE;

}

// 시그널 표시 정보
#define SIGNAL_SHOWTIME		5.0f							// 5초 동안
#define SIGNAL_COUNT		5.0f							// 5번 
#define SIGNAL_UNITTIME		(SIGNAL_SHOWTIME/SIGNAL_COUNT)  // 1번 표시할때 걸리는 시간.
#define SIGNAL_UNITTIME_HALF (SIGNAL_UNITTIME/2)
#define SIGNAL_ALPHA		0xFF

//------------------------------------------------------------------------------
// CUIMap::RenderSignal
// Explain: 2D 이펙트를 위한 하드 코딩~~~~즐~ 
// Date : 2005-10-21,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIMap::RenderSignal()
{
	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();
	
	static FLOAT fOpacity	= 0.0f;

	for( int i = 0; i < m_qSignalData.size(); i++ )
	{
		DOUBLE dElapsedTime = _pTimer->GetHighPrecisionTimer().GetSeconds();
		DOUBLE dDealy = dElapsedTime - m_qSignalData[i].dStartTime;
		
		if( dDealy > SIGNAL_SHOWTIME ) // 다끝났으면 삭제 
		{
			std::deque<CastleData>::iterator iter = m_qSignalData.begin() + i;
			m_qSignalData.erase( iter );
			return;
		}
		
		FLOAT	fX = 0.0f;
		FLOAT	fY = 0.0f;
		BOOL	bFirst = TRUE;
		UBYTE	bBlend;
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
			fTemp = SIGNAL_ALPHA * ( fTemp );
			bBlend = SIGNAL_ALPHA - fTemp;

			if( nCount <= 0 ) 
			{
				colOriBlend = ( 0xFFFFFFFF - SIGNAL_ALPHA ) | bBlend;
			}
		}
		else 
		{
			DOUBLE fTemp = ( dDealy ) / SIGNAL_UNITTIME_HALF;
			fTemp = SIGNAL_ALPHA * ( fTemp );
			bBlend = fTemp;
		}
		
		COLOR	colBlend = ( 0xFFFFFFFF - SIGNAL_ALPHA ) | bBlend;
		
		
		// Convert position
		fX = m_qSignalData[i].fX;
		fY = m_qSignalData[i].fZ;

		WorldToMap( fX, fY );

		if( fX + m_rcCastle[CASTLE_SIGNAL].Left >= m_rcViewportWorld.Left &&
			fX + m_rcCastle[CASTLE_SIGNAL].Right <= m_rcViewportWorld.Right &&
			fY + m_rcCastle[CASTLE_SIGNAL].Top >= m_rcViewportWorld.Top &&
			fY + m_rcCastle[CASTLE_SIGNAL].Bottom <= m_rcViewportWorld.Bottom )
		{
			fX += m_nPosX + MAP_SX;
			fY += m_nPosY + MAP_SY;
		
			FLOAT fWidth = ( m_rcCastle[CASTLE_SIGNAL].Right - m_rcCastle[CASTLE_SIGNAL].Left ) / 2;
			FLOAT fHeight = ( m_rcCastle[CASTLE_SIGNAL].Bottom - m_rcCastle[CASTLE_SIGNAL].Top ) / 2;
		
			FLOAT fOriWidth = fWidth;
			FLOAT fOriHeight = fHeight;

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
		
			// 잔상 Signal
			pDrawPort->AddTexture( fX - fWidth, fY - fHeight,
												fX + fWidth, fY + fHeight,
												m_rtCastle[CASTLE_SIGNAL].U0, m_rtCastle[CASTLE_SIGNAL].V0,
												m_rtCastle[CASTLE_SIGNAL].U1, m_rtCastle[CASTLE_SIGNAL].V1,
												colBlend );
			
			// 기본 Signal 
			pDrawPort->AddTexture( fX - fOriWidth, fY - fOriHeight,
												fX + fOriWidth, fY + fOriHeight,
												m_rtCastle[CASTLE_SIGNAL].U0, m_rtCastle[CASTLE_SIGNAL].V0,
												m_rtCastle[CASTLE_SIGNAL].U1, m_rtCastle[CASTLE_SIGNAL].V1,
												colOriBlend );
		} // if

	} // for( iter = iterBegin; i != iterEnd ; iter++ )
} 

//------------------------------------------------------------------------------
// CUIMap::SetSignalOn
// Explain:  시그널 변수를 셋팅하고 마우스 커서를 변경 합니다.
// Date : 2005-11-01,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIMap::SetSignalOn( bool bOn ) 
{
	if( !bOn )
	{
		if( m_bSignalBtnOn ) 
		{
			m_bSignalBtnOn = false; 
			CUIManager::getSingleton()->SetMouseCursorInsideUIs();
		}	
	}
	else
	{
		m_bSignalBtnOn = true;
		CUIManager::getSingleton()->SetMouseCursorInsideUIs( UMCT_SIGNAL );
	}

}

//------------------------------------------------------------------------------
// CUIMap::IsPossibleSignal
// Explain:  시그널을 사용할 수 있는 지 체크 
// Date : 2005-11-01,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
bool CUIMap::IsPossibleSignal()
{
	// 길드장, 부장인경우 
	// 공성 맵인 경우 
	LONG nRanking = _pNetwork->MyCharacterInfo.lGuildPosition;

	if( nRanking != GUILD_MEMBER_BOSS && nRanking != GUILD_MEMBER_VICE_BOSS )
		return false; 

	if( m_nCurMapData < 0 ) 
		return false;

	if( !IsCastleMap( m_nCurMapData ) )
		return false;

	if( !IsPossibleCastleMap() ) 
		return false; 

	return true;
}

//------------------------------------------------------------------------------
// CUIMap::IsPossibleCastleMap
// Explain: 공성전 지도를 사용할 수 있는지 체크 
// * 1. 공성전 중이고, 2. 내가 공성전에 참여 중이고, 공성전이 있는 존이여야함
// Date : 2005-11-01,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
bool CUIMap::IsPossibleCastleMap()
{	
	// 공성전이 진행 되고 있어야함 
	if( !_pUISWDoc->IsWar() || _pNetwork->MyCharacterInfo.sbJoinFlagMerac == WCJF_NONE )
		return false;
	if( _pNetwork->MyCharacterInfo.zoneNo != 7 )
		return false;

	return true;
}

//------------------------------------------------------------------------------
// CUIMap::ReSetData
// Explain: 맵 선택 콤보상자에 공성전 Map를 보이거나 그렇지 않거나 하도록 함.
// Date : 2005-10-25,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIMap::ReSetData()
{
	// 1. Map List
	m_cmbMap.ResetStrings();
	
	for( int i = 0; i < m_vecMapData.size(); i++ )
	{
		if( IsCastleMap( i ) ) // 공성전 멥이라면 
		{
			if( IsPossibleCastleMap() ) // 공성전이 가능한 상황인지 파악해서 추가
			{
				m_cmbMap.AddString( m_vecMapData[i].World.strName );	
			}
		}
		else // 공성 맵이 아니라면 그냥 추가 
		{
			m_cmbMap.AddString( m_vecMapData[i].World.strName );	
		}
	}

	m_cmbMap.SetCurSel( (!IsPossibleCastleMap()&&m_nCurMapData>=4) ? m_nCurMapData-1:m_nCurMapData );
}

//------------------------------------------------------------------------------
// CUIMap::IsDetailMap(BOOL IsNpcScrollDefined)
// Explain: 
// Date : 
//------------------------------------------------------------------------------

BOOL CUIMap::IsDetailMap(BOOL IsNpcScrollDefined)
{
	if(IsNpcScrollDefined)
	{
		if(!CUIManager::getSingleton()->m_IsInField )
		{
			// TO-KR-T20091204-008 [12/4/2009 rumist]
			// 여긴 다른 체크 부분. 그러나 같은 점검이 필요하므로 아래와 비슷한 루틴을 넣기 위해
			// IsPlayerInPeaceArea() 를 호출해 준다.
			IsPlayerInPeaceArea();
			SetDetailMap( TRUE );
		}
		else
		{
			CloseDetailMap();
		}
	}

	else
	{
		// TO-KR-T20091204-008 [12/4/2009 rumist]
		// 둘다 검사를 해야 하므로 아래와 같이 변경함.
		// if( ( _pNetwork->MyCharacterInfo.sbAttributePos == ATTC_PEACE ) || IsPlayerInPeaceArea() )
		// LC-KR-P20100219-005 [100429 selo]
		// 디테일 맵의 0번만 마을 맵으로 연다
		// bug fix [1/5/2012 rumist]
		CUIManager* pUIManager = CUIManager::getSingleton();
		
		int nSub = -1;

		if( (GetPlayerInArea(nSub) == eMS_VILLAGE) && pUIManager->m_nHelpNpc_Index == -1)
		{	
			SetDetailMap( TRUE, nSub );
		}
		// [2011/08/16 : Sora] 드라탄 공성중 공성지역에 있으면 세부맵을 호출하도록 수정 
		else if( pUIManager->GetSiegeWarfareNew()->GetWarState() && 
			_pNetwork->MyCharacterInfo.sbJoinFlagDratan != WCJF_NONE && 
			pUIManager->MyCheckDratanWarInside() )
		{
			SetDetailMap( TRUE, 2 );
		}
		else
		{
			SetDetailMap( FALSE );
		}
	}
	m_llMapBlendTimeForDetail = _pTimer->GetHighPrecisionTimer().GetMilliseconds() - MAP_DETAIL_BLENDTIME;

	return FALSE;
}

//------------------------------------------------------------------------------
// CUIMap::IsNpcInField(INDEX nZoneNum, INDEX nNpcIndex)
// Explain: 
// Date : 
//------------------------------------------------------------------------------
BOOL CUIMap::IsNpcInField(INDEX nZoneNum, INDEX nNpcIndex)
{
	FLOAT	fX = 0.0f;
	FLOAT	fZ = 0.0f;
	INT		nX = 0;
	INT		nY = 0;
	int		i;

	m_nCurMapData = -1;

	// nZoneNum으로 넘어온 실제 존 인덱스로 백터에 저장된 
	for( i = 0; i < m_vecMapData.size(); i++ )
	{
		if (m_vecMapData[i].World.nZoneIndex == nZoneNum)
		{
			m_nCurMapData = i;
			break;
		}
	}

	if (m_nCurMapData < 0)
		return TRUE;

	for( i = 0; i < m_vecMapData[m_nCurMapData].vecNpc.size(); i++ )
 	{
		if (IS_EVENT_ON(A_EVENT_HOLLOWEEN) == 0)
		{
			if (m_vecMapData[m_nCurMapData].vecNpc[i].nIndex == 454 ||
				m_vecMapData[m_nCurMapData].vecNpc[i].nIndex == 455)
				continue;
		}

 		if (m_vecMapData[m_nCurMapData].vecNpc[i].nIndex == nNpcIndex)
 		{
 			fX = m_vecMapData[m_nCurMapData].vecNpc[i].fX;
 			fZ = m_vecMapData[m_nCurMapData].vecNpc[i].fZ;
 		}
 	}

	FLOAT fWorldX = 0.0f;
	FLOAT fWorldZ = 0.0f;

	for( i = 0; i < m_vecMapData[m_nCurMapData].vecDetail.size(); i++)
 	{
		if( i == 0) //주노 마을 (월드 맵 오른쪽 아래 )
		{
			fWorldX = m_vecMapData[m_nCurMapData].vecDetail[i].fOffsetX;
			fWorldZ = m_vecMapData[m_nCurMapData].vecDetail[i].fOffsetZ;
		}
 	}

	if( fX < fWorldX || (fZ < fWorldZ && fX > fWorldX))
		return TRUE;

	FLOAT fWorldRatio = m_vecMapData[m_nCurMapData].World.fRatio;

	nX = fX * fWorldRatio;
	nY = fZ * fWorldRatio;

	if( nX < 0 || nY < 0 )
		return TRUE;

	UIRect temp(m_vecMapData[m_nCurMapData].vecDetail[0].rcSelRegion); 

	FLOAT fOffset = (temp.Right - temp.Left) * 0.2f;
	temp.Left -= fOffset; 
	temp.Right += fOffset;
	temp.Top -= fOffset;
	temp.Bottom += fOffset;

	CPrintF("fWorldRatio : %f \n", fWorldRatio );
	CPrintF("fX : %f , fY : %f \n", fX, fZ );
	CPrintF("X : %d , Y : %d \n", nX, nY );
	CPrintF("Region Value(LTRB): %d %d %d %d \n", temp.Left, temp.Top, temp.Right, temp.Bottom );

	if( nX >= temp.Left && nY >= temp.Top && nX < temp.Right && nY < temp.Bottom )	
	{	
		return FALSE;
	}

	return TRUE;
}

//------------------------------------------------------------------------------
// CUIMap::IsPlayerInPeaceArea()
// Explain: 
// Date : 
//------------------------------------------------------------------------------
BOOL CUIMap::IsPlayerInPeaceArea()
{
	FLOAT fX = 0.0f;
	FLOAT fZ = 0.0f;
	INT   nX = 0;
	INT   nY = 0;

	fX = _pNetwork->MyCharacterInfo.x;
	fZ = _pNetwork->MyCharacterInfo.z;

// 	FLOAT fWorldX = 0.0f;
// 	FLOAT fWorldZ = 0.0f;
// 
// 	for(int i = 0; i < m_vecMapData[m_nCurMapData].vecDetail.size(); i++)
//  	{	
// 		fWorldX = m_vecMapData[m_nCurMapData].vecDetail[i].fOffsetX;
// 		fWorldZ = m_vecMapData[m_nCurMapData].vecDetail[i].fOffsetZ;
//  	}
// 
// 	if( fX < fWorldX || (fZ < fWorldZ && fX > fWorldX))
// 		return FALSE;

	FLOAT fWorldRatio = m_vecMapData[m_nCurMapData].World.fRatio;

	nX = fX * fWorldRatio;
	nY = fZ * fWorldRatio;

	// TO-KR-T20091204-008 [12/4/2009 rumist]
	// 기본적인 호출시 detail 정보는 항상 0으로 초기화 한다.
	// 이유는 기본적으로 열리는 맵 번호는 항상 0번이기 때문이다.
	m_nCurDetail = 0;

	if( nX < 0 || nY < 0 )
		return FALSE;

	for(int i = 0; i < m_vecMapData[m_nCurMapData].vecDetail.size(); i++)
	{
		UIRect temp(m_vecMapData[m_nCurMapData].vecDetail[i].rcSelRegion); 
		FLOAT fOffset = (temp.Right - temp.Left) * 0.2f;
		temp.Left -= fOffset; 
		temp.Right += fOffset;
		temp.Top -= fOffset;
		temp.Bottom += fOffset;
		if( nX >= temp.Left && nY >= temp.Top && nX < temp.Right && nY < temp.Bottom )	
		{	
			// TO-KR-T20091204-008 [12/4/2009 rumist]
			// 만약 열렸다면 현재 인덱스 번호를 받는다.
			m_nCurDetail = i;
			return TRUE;
		}
	}
	//UIRect temp(m_vecMapData[m_nCurMapData].vecDetail[0].rcSelRegion); 


// 	CPrintF("fWorldRatio : %f \n", fWorldRatio );
// 	CPrintF("fX : %f , fY : %f \n", fX, fZ );
// 	CPrintF("X : %d , Y : %d \n", nX, nY );
// 	CPrintF("Region Value(LTRB): %d %d %d %d \n", temp.Left, temp.Top, temp.Right, temp.Bottom );

	return FALSE;
}

BOOL	CUIMap::GetWorldToMapPos(INDEX nZoneNum, float& fX, float& fY, float fZoomRatio)
{
	int	nMapDataIndex = FindMapData( nZoneNum, 0 );	

	if (nMapDataIndex == -1)
	{
		return FALSE;
	}
	// 지도의 Offset~
	fX -= m_vecMapData[nMapDataIndex].World.lOffsetX;	
	fY -= m_vecMapData[nMapDataIndex].World.lOffsetZ;

	// 지도에 대한 축척과 지도 Zoom비율
	fX = fX * m_vecMapData[nMapDataIndex].World.fRatio * fZoomRatio;	
	fY = fY * m_vecMapData[nMapDataIndex].World.fRatio * fZoomRatio;

	return TRUE;
}

void CUIMap::SetDetailMap( BOOL bOpen, int mapIndex /* = 0  */ )
{
	m_bDetailMapClick = bOpen;
	m_bShowWorldMap = bOpen;
	
	if( bOpen )
	{
		OpenDetailMap( mapIndex );
	}
	else
	{
		CloseDetailMap();
	}
}

// [2012/10/11 : Sora] 월드맵 개편
// 맵 이름 설정
CTString CUIMap::GetZoneName( int zone, int layer )
{
	CTString strReturn;
	if( CZoneInfo::getSingleton()->GetZoneType( zone ) == ZONE_DUNGEON )
	{
		strReturn.PrintF( "%s %dF", CZoneInfo::getSingleton()->GetZoneName( zone ), ( layer + 2 ) / 2);
	}
	else
	{
		if( layer > 0 )
		{
			strReturn.PrintF( _S( 3083, "%s 공성지역" ), _pUISWDoc->GetCastleName( zone ) );
		}
		else
		{
			strReturn.PrintF( "%s", CZoneInfo::getSingleton()->GetZoneName( zone ) );
		}
	}

	return strReturn;
}

// [2012/10/11 : Sora] 월드맵 개편
// 상위 맵으로 이동
void CUIMap::GoTopMap()
{
	if( m_bShowWorldMap )
	{
		m_btnGoTop.SetEnable(FALSE);
		SetCurrentWorldMap( ZONE_WORLD_MAP, 0 );

		m_eMapState = eMS_WORLD;
	}
	else
	{
		CloseDetailMap();
		ChangeMap();
	}
}

BOOL CUIMap::IsMapExist( int nZoneIndex, SBYTE sbLayer )
{
	return (FindMapData(nZoneIndex, sbLayer) != -1 ? TRUE : FALSE);
}

MapData* CUIMap::GetCurMapData( int nZoneIndex )
{
	int mapIndex = FindMapData( nZoneIndex, 0 );	

	if( mapIndex != -1 )
	{
		return &m_vecMapData[mapIndex];
	}

	return &m_vecMapData[0];
}

bool CUIMap::Goto(int x, int y)
{
	extern SLONG g_slZone;

	if (m_nCurMapData < 0)
		return false;

	if (m_vecMapData[m_nCurMapData].World.nZoneIndex != g_slZone)
		return false;

	int nMX, nMY;

	if (_bDetail == false || m_vecMapData[m_nCurMapData].vecDetail.size() == 0)
	{
		nMX = x * m_fZoomDetail / m_vecMapData[m_nCurMapData].World.fRatio 
			+ m_vecMapData[m_nCurMapData].World.lOffsetX;

		nMY = y * m_fZoomDetail / m_vecMapData[m_nCurMapData].World.fRatio 
			+ m_vecMapData[m_nCurMapData].World.lOffsetZ;
	}
	else
	{
		if (m_nCurDetail >= 0)
		{
			nMX = x * m_fZoomDetail / m_vecMapData[m_nCurMapData].vecDetail[m_nCurDetail].fRatio 
				+ m_vecMapData[m_nCurMapData].vecDetail[m_nCurDetail].fOffsetX;

			nMY = y * m_fZoomDetail / m_vecMapData[m_nCurMapData].vecDetail[m_nCurDetail].fRatio 
				+ m_vecMapData[m_nCurMapData].vecDetail[m_nCurDetail].fOffsetZ;
		}
	}

	CTString	strGOTO;
	strGOTO.PrintF("goto %d %d 0", nMX, nMY);

	_pNetwork->SendGMCommand(strGOTO);

	return true;
}

void CUIMap::RenderNpc(CDrawPort* pDraw, int nIndex, int nX, int nY, BOOL bHighlight )
{
	COLOR	colBlend = 0xFFFFFF00 | UBYTE( 0xFF * m_fMapOpacity );

	//NPC 안내시스템 -> 안내된 NPC의 미니맵 표시를 틀리게 함
	if(nIndex == UIMGR()->m_nHelpNpc_Index)
	{
		if(bHighlight)
		{
			pDraw->AddTexture( nX + m_rcHelpNpc.Left, nY + m_rcHelpNpc.Top,
				nX + m_rcHelpNpc.Right, nY + m_rcHelpNpc.Bottom,
				m_rtHelpNpc.U0, m_rtHelpNpc.V0, m_rtHelpNpc.U1, m_rtHelpNpc.V1,
				colBlend); 
		}
	}
	else
	{
		CUIMapOption* pOption = UIMGR()->GetRadar()->GetMapOption();

		if (pOption == NULL)
			return;

		int nShowIcon = -1;
		if (pOption->IsCheck(eNT_QUEST_Q) &&
			CQuestSystem::Instance().TestNPCForQuest(nIndex) == CQuestSystem::NQT_HAVE_QUEST)
		{
			nShowIcon = eNT_QUEST_Q;
		}
		else if (pOption->IsCheck(eNT_QUEST_A) &&
			CQuestSystem::Instance().TestNPCForQuest(nIndex) == CQuestSystem::NQT_CAN_PRIZE)
		{
			nShowIcon = eNT_QUEST_A;
		}
		else
		{
			for (int j = eNT_SHOP; j < eNT_MONSTER; ++j)
			{
				if (pOption->IsCheck((eNPC_TYPE)j) == false)
					continue;

				if (pOption->IsFlag(j, nIndex) != FALSE)
				{
					nShowIcon = j;
					break;
				}
			}
		}

		if (nShowIcon >= 0)
		{
			if (bHighlight == TRUE && nShowIcon == eNT_QUEST_A)
				return;

			UIRectUV uv = pOption->GetUV((eNPC_TYPE)nShowIcon);

			pDraw->AddTexture(nX + m_rcNPC.Left, nY + m_rcNPC.Top,
				nX + m_rcNPC.Right, nY + m_rcNPC.Bottom,
				uv.U0, uv.V0, uv.U1, uv.V1, colBlend);
		}
	}
}

void CUIMap::RenderGPS(CDrawPort* pDraw )
{
	// 전체 지도 보기에서는 렌더링 하지 않는다.
	if (m_vecMapData[m_nCurMapData].World.nZoneIndex == ZONE_WORLD_MAP)
		return;

	float	fX, fZ;
	int		nX, nY;

	UIRect	rcTempDetail = m_rcMapDetail;
	UIRect	rcTempWorld = m_rcMapWorld;
	rcTempWorld.Scale( m_fZoomWorld );
	rcTempDetail.Scale( m_fZoomDetail );	

	if (GAMEDATAMGR()->GetGPS()->IsPosition() == false)
		return;

	UpdateClient::GPSTargetMoveInfo* pInfo = GAMEDATAMGR()->GetGPS()->getInfo();

	if (m_bShowWorldMap)
	{
		nX = ( pInfo->x - m_vecMapData[m_nCurMapData].World.lOffsetX ) * m_vecMapData[m_nCurMapData].World.fRatio * m_fZoomWorld * m_fMapSizeRevision;
		nY = ( pInfo->z - m_vecMapData[m_nCurMapData].World.lOffsetZ ) * m_vecMapData[m_nCurMapData].World.fRatio * m_fZoomWorld * m_fMapSizeRevision;

		nX += m_rcViewportWorld.Left - rcTempWorld.Left;
		nY += m_rcViewportWorld.Top - rcTempWorld.Top;
	}
	else
	{
		if (m_nCurDetail >= 0)
		{
			fX = pInfo->x - m_vecMapData[m_nCurMapData].vecDetail[m_nCurDetail].fOffsetX;
			fZ = pInfo->z - m_vecMapData[m_nCurMapData].vecDetail[m_nCurDetail].fOffsetZ;

			nX = fX * m_vecMapData[m_nCurMapData].vecDetail[m_nCurDetail].fRatio * m_fZoomDetail;
			nY = fZ * m_vecMapData[m_nCurMapData].vecDetail[m_nCurDetail].fRatio * m_fZoomDetail;

			nX += m_rcViewportDetail.Left - rcTempDetail.Left;
			nY += m_rcViewportDetail.Top - rcTempDetail.Top;
		}
	}


	if (nX + m_rcGPS.Left >= m_rcViewportDetail.Left && nX + m_rcGPS.Right <= m_rcViewportDetail.Right &&
		nY + m_rcGPS.Top >= m_rcViewportDetail.Top && nY + m_rcGPS.Bottom <= m_rcViewportDetail.Bottom)
	{
		COLOR	colBlend;
		colBlend = 0xFFFFFF00 | UBYTE( 0xFF * m_fCharRatio );

		nX += m_nPosX + MAP_SX;
		nY += m_nPosY + MAP_SY;

		pDraw->InitTextureData(m_ptdBaseTexture);

		pDraw->AddTexture(m_rcGPS.Left + nX, m_rcGPS.Top + nY, m_rcGPS.Right + nX, m_rcGPS.Bottom + nY, m_uvGPS, colBlend);

		pDraw->FlushRenderingQueue();
	}	
}

void CUIMap::RenderRelic( CDrawPort* pDraw)
{
	// 전체 지도 보기에서는 렌더링 하지 않는다.
	if (m_vecMapData[m_nCurMapData].World.nZoneIndex == ZONE_WORLD_MAP)
		return;

	if (m_vecMapData[m_nCurMapData].World.nZoneIndex != _pNetwork->MyCharacterInfo.zoneNo)
		return;
	
	GPS* pInfo = GAMEDATAMGR()->GetGPS();

	if (pInfo == NULL)
		return;

	float	fX, fZ;
	int		nX, nY;

	UIRect	rcTempDetail = m_rcMapDetail;
	UIRect	rcTempWorld = m_rcMapWorld;
	rcTempWorld.Scale( m_fZoomWorld );
	rcTempDetail.Scale( m_fZoomDetail );

	float fposX = 0.f, fposZ = 0.f;

	if (pInfo->GetRelicPosCount() <= 0)
		return;

	GPS::mapRelicPos::iterator it = pInfo->GetMapRelicPos()->begin();
	GPS::mapRelicPos::iterator it_end = pInfo->GetMapRelicPos()->end();

	for (; it != it_end; ++it)
	{
		if ((*it).second.nCharIdx <= 0)
			continue;

		fposX = (*it).second.fX;
		fposZ = (*it).second.fZ;

		if (m_bShowWorldMap)
		{
			nX = ( fposX - m_vecMapData[m_nCurMapData].World.lOffsetX ) * m_vecMapData[m_nCurMapData].World.fRatio * m_fZoomWorld * m_fMapSizeRevision;
			nY = ( fposZ - m_vecMapData[m_nCurMapData].World.lOffsetZ ) * m_vecMapData[m_nCurMapData].World.fRatio * m_fZoomWorld * m_fMapSizeRevision;

			nX += m_rcViewportWorld.Left - rcTempWorld.Left;
			nY += m_rcViewportWorld.Top - rcTempWorld.Top;
		}
		else
		{
			if (m_nCurDetail >= 0)
			{
				fX = fposX - m_vecMapData[m_nCurMapData].vecDetail[m_nCurDetail].fOffsetX;
				fZ = fposZ - m_vecMapData[m_nCurMapData].vecDetail[m_nCurDetail].fOffsetZ;

				nX = fX * m_vecMapData[m_nCurMapData].vecDetail[m_nCurDetail].fRatio * m_fZoomDetail;
				nY = fZ * m_vecMapData[m_nCurMapData].vecDetail[m_nCurDetail].fRatio * m_fZoomDetail;

				nX += m_rcViewportDetail.Left - rcTempDetail.Left;
				nY += m_rcViewportDetail.Top - rcTempDetail.Top;
			}
		}

		if (nX >= m_rcViewportDetail.Left && nX <= m_rcViewportDetail.Right &&
			nY >= m_rcViewportDetail.Top && nY <= m_rcViewportDetail.Bottom)
		{
			COLOR	colBlend;
			colBlend = 0xFFFFFF00 | UBYTE( 0xFF * m_fCharRatio );

			nX += m_nPosX + MAP_SX;
			nY += m_nPosY + MAP_SY;

			pDraw->InitTextureData(m_ptdBaseTexture);
			pDraw->AddTexture(m_rcRelic.Left + nX, m_rcRelic.Top + nY, m_rcRelic.Right + nX, m_rcRelic.Bottom + nY, m_uvRelic, colBlend);
			pDraw->FlushRenderingQueue();
		}	
	}
}

void CUIMap::UpdateMap( bool bUpdate /*= false*/ )
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	extern SLONG	g_slZone;

	// 현재 던전의 경우에는 지도를 표시하지 않는다.
	// [100204: selo] 일반 던전도 지도를 표시하지 않는다.
	if( CZoneInfo::getSingleton()->GetZoneType( g_slZone ) == ZONE_SDUNGEON ||
		CZoneInfo::getSingleton()->GetZoneType( g_slZone ) == ZONE_DUNGEON )
	{
		if( IsVisible() )
			pUIManager->RearrangeOrder( UI_MAP, FALSE );

		pUIManager->GetChattingUI()->AddSysMessage( _S( 5785, "현재 지역에서는 월드맵을 출력 할 수 없습니다.." ), SYSMSG_ERROR );
		m_eMapState = eMS_NONE;
		return;
	}

	if( IsVisible() && bUpdate == false)
	{
		g_iOpacityInMap = m_slbOpacity.GetCurPos();
		if( m_bShowWorldMap )
			g_iZoomInMap = m_slbZoomWorld.GetCurPos();
		else
			g_iZoomInMap = m_slbZoomDetail.GetCurPos();

		g_iXPosInMap = GetPosX();
		g_iYPosInMap = GetPosY();
		pUIManager->m_nHelpNpc_Index = -1;
	}

	// Toggle map
	if (bUpdate == false)
	{
		BOOL	bVisible = !IsVisible();
		pUIManager->RearrangeOrder( UI_MAP, bVisible );
	}

#ifdef NEW_WORLD_MAP		// [2012/10/11 : Sora] 월드맵 개편
	if(_pUISWDoc->IsWar())
	{
		pUIManager->GetMap()->SetCurrentWorldMap( _pNetwork->MyCharacterInfo.zoneNo, 1 );		// yjpark
	}
	else
	{
		pUIManager->GetMap()->SetCurrentWorldMap( _pNetwork->MyCharacterInfo.zoneNo, _pNetwork->MyCharacterInfo.yLayer );		// yjpark
	}
#endif

	
	IsDetailMap();
	
	if (bUpdate == false)
		SetMapData();

#ifdef NEW_WORLD_MAP		// [2012/10/11 : Sora] 월드맵 개편
	if( IsVisible() )
	{
		if (m_nCurMapData > -1)
		{
			m_btnGoTop.SetEnable(m_vecMapData[m_nCurMapData].World.nZoneIndex != ZONE_WORLD_MAP);
		}

	}
#endif

	// !! 공성전 Map표시 조건에 맞다면 서버로 정보 요청 
	//
	if( IsVisible() )
	{
		if (m_nCurMapData > -1)
		{
			if (IsPossibleCastleMap() && IsCastleMap( m_nCurMapData ))
			{
				ReSetData();
				_pNetwork->SendCastleMapRecent();
			}
		}
	}
	else
	{
		m_qSignalData.clear();
	}

	if (m_penPlayerEntity != CEntity::GetPlayerEntity(0))
	{
		CEntity* penPlEntity;
		penPlEntity = CEntity::GetPlayerEntity(0); //캐릭터 자기 자신
		m_penPlayerEntity = (CPlayerEntity*)penPlEntity;
	}
}

void CUIMap::OnUpdate( float fDeltaTime, ULONG ElapsedTime )
{
	if (IsVisible() && m_nCurMapData >= 0)
	{
		if (m_penPlayerEntity == NULL)
			return;

		if ((m_lastPos - m_penPlayerEntity->en_plLastPlacement.pl_PositionVector).Length() < 1.f)
			return;

		m_lastPos = m_penPlayerEntity->en_plLastPlacement.pl_PositionVector;

		extern SLONG g_slZone;

		if (m_eMapState == eMS_WORLD)
			return;

		int nSub = -1;
		if (m_eMapState != GetPlayerInArea(nSub))
			UpdateMap(true);
	}
}

eMAP_STATE CUIMap::GetPlayerInArea(int &nSubIndex)
{
	nSubIndex = -1;

	if (m_vecMapData[m_nCurMapData].World.nZoneIndex == ZONE_WORLD_MAP)
		return eMS_WORLD;

	float fX = _pNetwork->MyCharacterInfo.x;
	float fZ = _pNetwork->MyCharacterInfo.z;

	GetWorldToMapPos(m_vecMapData[m_nCurMapData].World.nZoneIndex, fX, fZ, m_fMapSizeRevision);

	int i;

	for(i = 0; i < m_vecMapData[m_nCurMapData].vecDetail.size(); i++ )
	{
		UIRect rcTemp = m_vecMapData[m_nCurMapData].vecDetail[i].rcSelRegion;
		rcTemp.Scale( m_fMapSizeRevision );

		if( fX > rcTemp.Left && fZ > rcTemp.Top && fX < rcTemp.Right && fZ < rcTemp.Bottom )
		{
			nSubIndex = i;
			return eMS_VILLAGE;
		}
	}

	return eMS_ZONE;
}
