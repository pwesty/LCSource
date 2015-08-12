#include "stdh.h"

// 헤더 정리. [12/2/2009 rumist]
#include <Engine/Interface/UIInternalClasses.h>
#include <vector>
#include <Engine/Interface/UIOption.h>
#include <Engine/Engine.h>
#include <Engine/Network/Web.h>
#include <Engine/Graphics/Adapter.h>
#include <Engine/GameStageManager/StageMgr.h>

//2013/04/15 jeil 런처에 해상도 변경 추가
#include <fstream>

//안태훈 수정 시작	//(Open beta)(2004-12-30)
extern FLOAT g_fSndVol;
extern FLOAT g_fMscVol;
//안태훈 수정 끝	//(Open beta)(2004-12-30)
extern FLOAT		gfx_fBrightness;
extern FLOAT		gfx_fContrast;
extern FLOAT		gap_fTextureLODBias;
extern INDEX		g_bRenderDecoration;
extern FLOAT		ter_fLODMul;
extern FLOAT		ska_fLODMul;
extern FLOAT		mdl_fLODMul;
extern INDEX		g_iUseBloom;
extern INDEX		g_iShadowDetail;
extern INDEX		g_iReflectionDetail;
extern INDEX		g_iEnterChat;
extern INDEX		g_iAutoAttack;
extern INDEX		g_iShowName;
extern INDEX		g_iShowNameItem;

// wooss 070401 ------------------->><<
// kw : WSS_HELP_SYSTEM_1
extern INDEX		g_iShowHelp1Icon;

extern INDEX		g_iRejectExchange;
extern INDEX		g_iRejectParty;

extern INDEX		g_iQuestBookOpen;

extern BOOL			_bWindowChanging;
extern HWND			_hwndMain;
extern HWND			_hDlgWeb;
extern cWeb			g_web;
extern CDrawPort	*_pdpMain;
extern CDrawPort	*_pdpNormalMain;
extern CDrawPort	*_pdpWideScreenMain;
extern CViewPort	*_pvpViewPortMain;
extern HINSTANCE	_hInstanceMain;
extern INDEX		sam_bFullScreenActive;
extern INDEX		sam_iScreenSizeI;
extern INDEX		sam_iScreenSizeJ;
extern INDEX		sam_iDisplayDepth;
extern INDEX		sam_iDisplayAdapter;
extern INDEX		sam_iGfxAPI;
extern INDEX		sam_bWideScreen;
extern INDEX		cmd_iWindowLeft;
extern INDEX		cmd_iWindowTop;
extern INDEX		g_bSlaveNoTarget;			// UI_REFORM :Su-won
// MBCS -> WBCS전환 [12/21/2011 rumist]
static WCHAR		wchWindowTitle[256];
//static char			achWindowTitle[256];
static PIX			_pixLastSizeI, _pixLastSizeJ;
extern ENGINE_API INDEX sam_bFullScreenActive;

int	g_nCurSelResolution;
extern INDEX	g_iCountry;

#ifdef KALYDO
extern ENGINE_API HWND	g_KalydoWindowHandle;
#endif

// ----------------------------------------------------------------------------
// Name : CUIOption()
// Desc : Constructor
// ----------------------------------------------------------------------------
CUIOption::CUIOption()
{
	int		i;
	m_nCurOptionTab = 0;
	m_bPixelShader = FALSE;

	for( i = 0; i < 11; i++ )
	{
		m_afBrightness[i] = 0.9f * ( ( i - 5 ) * 0.05f );
		m_afContrast[i] = 1.0f + 0.9f * ( ( i - 5 ) * 0.05f );
	}
	for( i = 0; i < 3; i++ )
	{
		m_afTextureQuality[i] = 0.5f - i * 0.5f;
	}
	for( i = 0; i < 5; i++ )
	{
		m_afViewTerrain[i] = 0.7f + i * 0.075;//1.0f + 0.5f * ( ( i - 4 ) / 5.0f );
	}

	m_pixDesktopWidth = 0;
	m_pixDesktopHeight = 0;

	m_bShowToolTip = FALSE;
	m_strToolTip = CTString( "" );
	m_ptdButtonTexture = NULL;
		
}

// ----------------------------------------------------------------------------
// Name : ~CUIOption()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUIOption::~CUIOption()
{
	Destroy();

	if( _pvpViewPortMain != NULL )
	{
		_pGfx->DestroyWindowCanvas( _pvpViewPortMain );
		_pvpViewPortMain = NULL;
		_pdpNormalMain = NULL;
	}

	STOCK_RELEASE(m_ptdButtonTexture);
}

// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUIOption::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	INDEX tv_chat,tv_fame,tv_auto,tv_summon;
	tv_chat = 0;
	tv_fame = 0;
	tv_auto = 0;
	tv_summon = 0;
	extern BOOL g_bIsMalEng;

	switch(g_iCountry)
	{
	case KOREA:
		tv_chat=tv_fame=tv_auto=tv_summon=3;
		break;
	case TAIWAN:
	case TAIWAN2:
		tv_chat=1;
		tv_auto=3;
		tv_fame=1;
		tv_summon=0;
		break;
	case THAILAND:
		tv_chat=tv_fame=tv_auto=tv_summon=3;
		break;
	case JAPAN:
		tv_chat=tv_fame=tv_auto=tv_summon=3;
		break;
	case MALAYSIA:
		extern BOOL g_bIsMalEng;
		if(g_bIsMalEng)
		{
			tv_chat=tv_summon=0;
			tv_auto=1;
			tv_fame=3;
		}
		else
		{
			tv_chat=1;
			tv_auto=3;
			tv_fame=1;
			tv_summon=0;
		}
		break;
	case HONGKONG:
		if(g_bIsMalEng)
		{
			tv_chat=tv_summon=0;
			tv_auto=1;
			tv_fame=3;		
		}
		else
		{
			tv_chat=1;
			tv_auto=3;
			tv_fame=1;
			tv_summon=0;
		}
	case USA:
		tv_chat=tv_summon=0;
		tv_auto=1;
		tv_fame=3;
		break;
	case BRAZIL:
	case GERMANY:
	case SPAIN://FRANCE_SPAIN_CLOSEBETA_NA_20081124
	case FRANCE:
	case POLAND:
	case RUSSIA:
	case TURKEY:
	case MEXICO:
	case ITALY:
	case NETHERLANDS:
		tv_chat=tv_fame=tv_auto=tv_summon=3;
		break;
	default:
		tv_chat=tv_fame=tv_auto=tv_summon=0;
		break;
	}

	CUIWindow::Create(pParentWnd, nX, nY, nWidth, nHeight);

	// Region of each part
	//m_rcTitle.SetRect( 0, 0, 230, 22 );
	//m_rcOptionTab.SetRect( 4, 24, 25, 245 );
	m_rcTitle.SetRect( 0, 0, 255, 35 );
	m_rcOptionTab.SetRect( 6, 40, 27, 316 );
	m_rcInnerBackground.SetRect(32, 61, 245, 363);
		

	// Create option texture
	m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\NewOption.tex" ) );
	m_ptdButtonTexture =CreateTexture( CTString( "Data\\Interface\\CommonBtn.tex" ) );

	FLOAT	fTexWidth	= m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight	= m_ptdBaseTexture->GetPixHeight();

	// UV Coordinate of each part
	// Background
	//m_rtBackground.SetUV( 0, 0, 230, 385, fTexWidth, fTexHeight );
	m_rtBackground.SetUV( 0, 0, 255, 401, fTexWidth, fTexHeight );

	m_rtInnerBackground[0].SetUV(0, 415, 214, 718, fTexWidth, fTexHeight);
	m_rtInnerBackground[1].SetUV(0, 721, 214, 1024, fTexWidth, fTexHeight);
	m_rtInnerBackground[2].SetUV(218, 415, 432, 718, fTexWidth, fTexHeight);	
	m_rtInnerBackground[3].SetUV(218, 721, 432, 1024, fTexWidth, fTexHeight);	

	// Tool tip
	//m_rtToolTipL.SetUV( 183, 419, 190, 441, fTexWidth, fTexHeight );
	//m_rtToolTipM.SetUV( 193, 419, 195, 441, fTexWidth, fTexHeight );
	//m_rtToolTipR.SetUV( 198, 419, 205, 441, fTexWidth, fTexHeight );
	m_rtToolTipL.SetUV( 270, 59, 277, 96, fTexWidth, fTexHeight );
	m_rtToolTipM.SetUV( 277, 59, 461, 96, fTexWidth, fTexHeight );
	m_rtToolTipR.SetUV( 461, 59, 468, 96, fTexWidth, fTexHeight );



	// Item plus effect
	//m_cmbItemPlusEffect.Create( this, OPTION_CONTROL_SX, OPTION_ITEMEFFECT_SY + 17, 170, 15, 156, 1, 13, 13,
	//							10, _pUIFontTexMgr->GetFontHeight() + 4, 8, 4 );
	//m_cmbItemPlusEffect.SetBackUV( 0, 479, 170, 494, fTexWidth, fTexHeight );
	//m_cmbItemPlusEffect.SetDownBtnUV( 168, 401, 181, 414, fTexWidth, fTexHeight );
	//m_cmbItemPlusEffect.SetUpBtnUV( 183, 401, 196, 414, fTexWidth, fTexHeight );
	//m_cmbItemPlusEffect.SetDropListUV( 0, 462, 170, 477, fTexWidth, fTexHeight );
	m_cmbItemPlusEffect.Create( this, 40, 268, 198, 16, 176, 4, 17, 10,
								10, _pUIFontTexMgr->GetFontHeight() + 4, 8, 4 );
// 	m_cmbItemPlusEffect.Create( this, 40, 270, 198, 16, 176, 4, 17, 10,
// 								10, _pUIFontTexMgr->GetFontHeight() + 4, 8, 4 );
	m_cmbItemPlusEffect.SetBackUV( 8, 596, 206, 612, fTexWidth, fTexHeight );
	m_cmbItemPlusEffect.SetDownBtnUV( 283, 5, 300, 15, fTexWidth, fTexHeight );
	m_cmbItemPlusEffect.SetUpBtnUV( 302, 5, 319, 15, fTexWidth, fTexHeight );
	m_cmbItemPlusEffect.SetDropListUV( 270, 60, 468, 96, fTexWidth, fTexHeight );
	m_cmbItemPlusEffect.CreateScroll( TRUE, 9, 7, 0, 0, 10 );
	// Up button
	m_cmbItemPlusEffect.SetScrollUpUV( UBS_IDLE, 209, 416, 218, 423, fTexWidth, fTexHeight );
	m_cmbItemPlusEffect.SetScrollUpUV( UBS_CLICK, 220, 416, 229, 423, fTexWidth, fTexHeight );
	m_cmbItemPlusEffect.CopyScrollUpUV( UBS_IDLE, UBS_ON );
	m_cmbItemPlusEffect.CopyScrollUpUV( UBS_IDLE, UBS_DISABLE );
	// Bar button
	m_cmbItemPlusEffect.SetScrollBarTopUV( 231, 416, 240, 426, fTexWidth, fTexHeight );
	m_cmbItemPlusEffect.SetScrollBarMiddleUV( 231, 427, 240, 429, fTexWidth, fTexHeight );
	m_cmbItemPlusEffect.SetScrollBarBottomUV( 231, 430, 240, 440, fTexWidth, fTexHeight );
	// Down button
	m_cmbItemPlusEffect.SetScrollDownUV( UBS_IDLE, 209, 425, 218, 432, fTexWidth, fTexHeight );
	m_cmbItemPlusEffect.SetScrollDownUV( UBS_CLICK, 220, 425, 229, 432, fTexWidth, fTexHeight );
	m_cmbItemPlusEffect.CopyScrollDownUV( UBS_IDLE, UBS_ON );
	m_cmbItemPlusEffect.CopyScrollDownUV( UBS_IDLE, UBS_DISABLE );
	// Strings
	m_cmbItemPlusEffect.AddString( _S( 1031, "내 효과 비공개" ) );		
	m_cmbItemPlusEffect.AddString( _S( 1032, "내 효과 공개" ) );	

	// Show name slide bar
	//m_slbShowName.Create( this, OPTION_CONTROL_SX, OPTION_SHOWNAME_SY + 16, 170, 5, 10, 10, 0, 6, 0, 5 );
	//m_slbShowName.SetBackgroundUV( 0, 498, 170, 503, fTexWidth, fTexHeight );
	m_slbShowName.Create( this, 43, 93, 191, 5, 12, 12, 0, 6, 0, 5 );
	m_slbShowName.SetBarUV( 288, 31, 300, 43, fTexWidth, fTexHeight );
		
	// Show item name slide bar
	//m_slbShowNameItem.Create( this, OPTION_CONTROL_SX, OPTION_SHOWNAME_ITEM_SY + 16, 170, 5, 10, 10, 0, 6, 0, 5 );
	//m_slbShowNameItem.SetBackgroundUV( 0, 498, 170, 503, fTexWidth, fTexHeight );
	//m_slbShowNameItem.SetBarUV( 198, 386, 208, 396, fTexWidth, fTexHeight );
	m_slbShowNameItem.Create( this, 43, 127, 191, 5, 12, 12, 0, 6, 0, 5 );
	m_slbShowNameItem.SetBarUV( 288, 31, 300, 43, fTexWidth, fTexHeight );

	// Brightness slide bar
	//m_slbBrightness.Create( this, OPTION_CONTROL_SX, OPTION_BRIGHTNESS_SY + 16, 170, 5, 10, 10, 5, 11, 0, 10 );
	//m_slbBrightness.SetBackgroundUV( 0, 498, 170, 503, fTexWidth, fTexHeight );
	//m_slbBrightness.SetBarUV( 198, 386, 208, 396, fTexWidth, fTexHeight );
	m_slbBrightness.Create( this, 43, 161, 191, 5, 12, 12, 5, 11, 0, 10 );
	m_slbBrightness.SetBarUV( 288, 31, 300, 43, fTexWidth, fTexHeight );

	// Contrast slide bar
	//m_slbContrast.Create( this, OPTION_CONTROL_SX, OPTION_CONTRAST_SY + 16, 170, 5, 10, 10, 5, 11, 0, 10 );
	//m_slbContrast.SetBackgroundUV( 0, 498, 170, 503, fTexWidth, fTexHeight );
	//m_slbContrast.SetBarUV( 198, 386, 208, 396, fTexWidth, fTexHeight );
	m_slbContrast.Create( this, 43, 195, 191, 5, 12, 12, 5, 11, 0, 10 );
	m_slbContrast.SetBarUV( 288, 31, 300, 43, fTexWidth, fTexHeight );

	// Texture slide bar
	//m_slbTexture.Create( this, OPTION_CONTROL_SX, OPTION_TEXTURE_SY + 16, 170, 5, 10, 10, 2, 3, 0, 2 );
	//m_slbTexture.SetBackgroundUV( 0, 498, 170, 503, fTexWidth, fTexHeight );
	//m_slbTexture.SetBarUV( 198, 386, 208, 396, fTexWidth, fTexHeight );
	m_slbTexture.Create( this, 43, 93, 191, 5, 12, 12, 2, 3, 0, 2 );
	m_slbTexture.SetBarUV( 288, 31, 300, 43, fTexWidth, fTexHeight );

	// Shadow slide bar
	//m_slbShadow.Create( this, OPTION_CONTROL_SX, OPTION_SHADOW_SY + 16, 170, 5, 10, 10, 2, 5, 0, 4 );
	//m_slbShadow.SetBackgroundUV( 0, 498, 170, 503, fTexWidth, fTexHeight );
	//m_slbShadow.SetBarUV( 198, 386, 208, 396, fTexWidth, fTexHeight );
	m_slbShadow.Create( this, 43, 127, 191, 5, 12, 12, 2, 5, 0, 4 );
	m_slbShadow.SetBarUV( 288, 31, 300, 43, fTexWidth, fTexHeight );

	// Character quality slide bar
	//m_slbCharQuality.Create( this, OPTION_CONTROL_SX, OPTION_CHARACTER_SY + 16, 170, 5, 10, 10, 2, 5, 0, 4 );
	//m_slbCharQuality.SetBackgroundUV( 0, 498, 170, 503, fTexWidth, fTexHeight );
	//m_slbCharQuality.SetBarUV( 198, 386, 208, 396, fTexWidth, fTexHeight );
	m_slbCharQuality.Create( this, 43, 161, 191, 5, 12, 12, 2, 5, 0, 4 );
	m_slbCharQuality.SetBarUV( 288, 31, 300, 43, fTexWidth, fTexHeight );
	m_slbCharQuality.SetEnable( FALSE );	// 임시
	m_slbCharQuality.SetCurPos( 4 );		// 임시

	// View-terrain slide bar
	//m_slbViewTerrain.Create( this, OPTION_CONTROL_SX, OPTION_VIEWTERAIN_SY + 16, 170, 5, 10, 10, 4, 5, 0, 4 );
	//m_slbViewTerrain.SetBackgroundUV( 0, 498, 170, 503, fTexWidth, fTexHeight );
	//m_slbViewTerrain.SetBarUV( 198, 386, 208, 396, fTexWidth, fTexHeight );
	m_slbViewTerrain.Create( this, 43, 195, 191, 5, 12, 12, 4, 5, 0, 4 );
	m_slbViewTerrain.SetBarUV( 288, 31, 300, 43, fTexWidth, fTexHeight );


	// Reflection slide bar
	//m_slbReflection.Create( this, OPTION_CONTROL_SX, OPTION_REFLECTION_SY + 16, 170, 5, 10, 10, 0, 4, 0, 3 );
	//m_slbReflection.SetBackgroundUV( 0, 498, 170, 503, fTexWidth, fTexHeight );
	//m_slbReflection.SetBarUV( 198, 386, 208, 396, fTexWidth, fTexHeight );
	m_slbReflection.Create( this, 43, 229, 191, 5, 12, 12, 0, 4, 0, 3 );
	m_slbReflection.SetBarUV( 288, 31, 300, 43, fTexWidth, fTexHeight );

	// Filtering slide bar
	//m_slbFiltering.Create( this, OPTION_CONTROL_SX, OPTION_FILTERING_SY + 16, 170, 5, 10, 10, 0, 3, 0, 2 );
	//m_slbFiltering.SetBackgroundUV( 0, 498, 170, 503, fTexWidth, fTexHeight );
	//m_slbFiltering.SetBarUV( 198, 386, 208, 396, fTexWidth, fTexHeight );
	m_slbFiltering.Create( this, 43, 263, 191, 5, 12, 12, 0, 7, 0, 6 );; // 신주노맵의 밝기때문에 블롬효과 단계늘림
	m_slbFiltering.SetBarUV( 288, 31, 300, 43, fTexWidth, fTexHeight );

	// BGM slide bar
	//m_slbBGM.Create( this, OPTION_CONTROL_SX, OPTION_BGM_SY + 16, 170, 5, 10, 10, 50, 101, 0, 100 );
	//m_slbBGM.SetBackgroundUV( 0, 498, 170, 503, fTexWidth, fTexHeight );
	//m_slbBGM.SetBarUV( 198, 386, 208, 396, fTexWidth, fTexHeight );
	m_slbBGM.Create( this, 43, 93, 181, 5, 12, 12, 50, 101, 0, 100 );
	m_slbBGM.SetBarUV( 288, 31, 300, 43, fTexWidth, fTexHeight );

	// FX sound slide bar
	//m_slbFXSound.Create( this, OPTION_CONTROL_SX, OPTION_FXSOUND_SY + 16, 170, 5, 10, 10, 50, 101, 0, 100 );
	//m_slbFXSound.SetBackgroundUV( 0, 498, 170, 503, fTexWidth, fTexHeight );
	//m_slbFXSound.SetBarUV( 198, 386, 208, 396, fTexWidth, fTexHeight );
	m_slbFXSound.Create( this, 43, 127, 191, 5, 12, 12, 50, 101, 0, 100 );
	m_slbFXSound.SetBarUV( 288, 31, 300, 43, fTexWidth, fTexHeight );

	// Resolusion combo box
	//m_cmbResolution.Create( this, OPTION_CONTROL_SX, OPTION_RESOLUTION_SY + 17, 170, 15, 156, 1, 13, 13,
	//						10, _pUIFontTexMgr->GetFontHeight() + 4, 8, 4 );
	//m_cmbResolution.SetBackUV( 0, 479, 170, 494, fTexWidth, fTexHeight );
	//m_cmbResolution.SetDownBtnUV( 168, 401, 181, 414, fTexWidth, fTexHeight );
	//m_cmbResolution.SetUpBtnUV( 183, 401, 196, 414, fTexWidth, fTexHeight );
	//m_cmbResolution.SetDropListUV( 0, 462, 170, 477, fTexWidth, fTexHeight );

	m_cmbResolution.Create( this, 40, 80, 198, 16, 176, 4, 17, 10,
							10, _pUIFontTexMgr->GetFontHeight() + 4, 8, 4 );
	m_cmbResolution.SetDownBtnUV( 283, 5, 300, 15, fTexWidth, fTexHeight );
	m_cmbResolution.SetUpBtnUV( 302, 5, 319, 15, fTexWidth, fTexHeight );
	m_cmbResolution.SetDropListUV( 270, 60, 468, 96, fTexWidth, fTexHeight );
	m_cmbResolution.CreateScroll( TRUE, 9, 7, 0, 0, 10 );
	// Up button
	m_cmbResolution.SetScrollUpUV( UBS_IDLE, 209, 416, 218, 423, fTexWidth, fTexHeight );
	m_cmbResolution.SetScrollUpUV( UBS_CLICK, 220, 416, 229, 423, fTexWidth, fTexHeight );
	m_cmbResolution.CopyScrollUpUV( UBS_IDLE, UBS_ON );
	m_cmbResolution.CopyScrollUpUV( UBS_IDLE, UBS_DISABLE );
	// Bar button
	m_cmbResolution.SetScrollBarTopUV( 231, 416, 240, 426, fTexWidth, fTexHeight );
	m_cmbResolution.SetScrollBarMiddleUV( 231, 427, 240, 429, fTexWidth, fTexHeight );
	m_cmbResolution.SetScrollBarBottomUV( 231, 430, 240, 440, fTexWidth, fTexHeight );
	// Down button
	m_cmbResolution.SetScrollDownUV( UBS_IDLE, 209, 425, 218, 432, fTexWidth, fTexHeight );
	m_cmbResolution.SetScrollDownUV( UBS_CLICK, 220, 425, 229, 432, fTexWidth, fTexHeight );
	m_cmbResolution.CopyScrollDownUV( UBS_IDLE, UBS_ON );
	m_cmbResolution.CopyScrollDownUV( UBS_IDLE, UBS_DISABLE );
	// if this client is kalydo version, blocked resolution feature. [11/30/2011 rumist]
#ifdef KALYDO
	m_cmbResolution.SetEnable( FALSE );
#endif

	// 선택 가능 대상 설정
	//m_cmbTargetDest.Create( this, OPTION_CONTROL_SX, OPTION_TARGETDEST_SY + 17, 170, 15, 156, 1, 13, 13,
	//							10, _pUIFontTexMgr->GetFontHeight() + 4, 8, 4 );
	m_cmbTargetDest.Create( this, 40, 308, 198, 16, 176, 4, 17, 10,
								10, _pUIFontTexMgr->GetFontHeight() + 4, 8, 4 );
	m_cmbTargetDest.SetBackUV( 8, 596, 206, 612, fTexWidth, fTexHeight );
	m_cmbTargetDest.SetDownBtnUV( 283, 5, 300, 15, fTexWidth, fTexHeight );
	m_cmbTargetDest.SetUpBtnUV( 302, 5, 319, 15, fTexWidth, fTexHeight );
	m_cmbTargetDest.SetDropListUV( 270, 60, 468, 96, fTexWidth, fTexHeight );
	m_cmbTargetDest.CreateScroll( TRUE, 9, 7, 0, 0, 10 );
	// Up button
	m_cmbTargetDest.SetScrollUpUV( UBS_IDLE, 209, 416, 218, 423, fTexWidth, fTexHeight );
	m_cmbTargetDest.SetScrollUpUV( UBS_CLICK, 220, 416, 229, 423, fTexWidth, fTexHeight );
	m_cmbTargetDest.CopyScrollUpUV( UBS_IDLE, UBS_ON );
	m_cmbTargetDest.CopyScrollUpUV( UBS_IDLE, UBS_DISABLE );
	// Bar button
	m_cmbTargetDest.SetScrollBarTopUV( 231, 416, 240, 426, fTexWidth, fTexHeight );
	m_cmbTargetDest.SetScrollBarMiddleUV( 231, 427, 240, 429, fTexWidth, fTexHeight );
	m_cmbTargetDest.SetScrollBarBottomUV( 231, 430, 240, 440, fTexWidth, fTexHeight );
	// Down button
	m_cmbTargetDest.SetScrollDownUV( UBS_IDLE, 209, 425, 218, 432, fTexWidth, fTexHeight );
	m_cmbTargetDest.SetScrollDownUV( UBS_CLICK, 220, 425, 229, 432, fTexWidth, fTexHeight );
	m_cmbTargetDest.CopyScrollDownUV( UBS_IDLE, UBS_ON );
	m_cmbTargetDest.CopyScrollDownUV( UBS_IDLE, UBS_DISABLE );
	// Strings
	m_cmbTargetDest.AddString( _S(4163, "모두 허가" ) );		
	m_cmbTargetDest.AddString( _S(4164, "소환수&펫 류 제외" ) );
	m_cmbTargetDest.AddString( _S(4386, "캐릭터,소환수&펫 제외"));


	// -------------------------------------------------------------------------------->>
	fTexWidth	= m_ptdButtonTexture->GetPixWidth();
	fTexHeight	= m_ptdButtonTexture->GetPixHeight();

	// Option tab
	m_rtOptionTabBackground.SetUV( 86, 1, 108, 101, fTexWidth, fTexHeight);
	m_rtSelOptionTabBackground.SetUV( 59, 1, 81, 101, fTexWidth, fTexHeight);

	for( int iTab = 0 ; iTab < OPTIONTAB_TOTAL; iTab++ )
	{
		/***
		int nTx = OPTION_TAB_WIDTH * iTab * 2;
		m_rtSelOptionTab[iTab].SetUV( nTx, 386, nTx + OPTION_TAB_WIDTH, 439, fTexWidth, fTexHeight );    
		m_rtOptionTab[iTab].SetUV( nTx + OPTION_TAB_WIDTH, 386, nTx + OPTION_TAB_WIDTH * 2, 439, fTexWidth, fTexHeight );
		***/

		int nTx = 221 +OPTION_TAB_WIDTH * iTab * 2;
		int nTy0=209, nTy1=238;
		if( iTab!=0)
		{
			--nTy0;
			nTy1 -=2;
		}

		m_rtSelOptionTab[iTab].SetUV( nTx +OPTION_TAB_WIDTH, nTy0, nTx + OPTION_TAB_WIDTH*2 -4, nTy1, fTexWidth, fTexHeight );		
		m_rtOptionTab[iTab].SetUV( nTx, nTy0, nTx + OPTION_TAB_WIDTH-4, nTy1, fTexWidth, fTexHeight );
	}

	// Close Button
	//m_btnClose.Create( this, CTString( "" ), 198, 4, 14, 14 );
	//m_btnClose.SetUV( UBS_IDLE, 168, 386, 182, 400, fTexWidth, fTexHeight );
	//m_btnClose.SetUV( UBS_CLICK, 183, 386, 197, 400, fTexWidth, fTexHeight );
	m_btnClose.Create( this, CTString( "" ), 232, 4, 16, 16 );
	m_btnClose.SetUV( UBS_IDLE, 211, 33, 227, 49, fTexWidth, fTexHeight );
	m_btnClose.SetUV( UBS_CLICK, 229, 33, 245, 49, fTexWidth, fTexHeight );
	m_btnClose.CopyUV( UBS_IDLE, UBS_ON );
	m_btnClose.CopyUV( UBS_IDLE, UBS_DISABLE );


	UIRect rcLeft, rcMiddel, rcRight;
	UIRectUV3 rtIdleBtn, rtClickBtn;

	rcLeft.SetRect(0, 0, 14, 21);
	rcMiddel.SetRect(14, 0, 78, 21);
	rcRight.SetRect(78, 0, 92, 21);

	rtIdleBtn.SetUV3(113, 0, 127, 22, 127, 0, 168, 22, 168, 0, 183, 22, fTexWidth, fTexHeight );
	rtClickBtn.SetUV3(187, 0, 201, 22, 201, 0, 242, 22, 242, 0, 256, 22, fTexWidth, fTexHeight);


	// OK Button
	//m_btnOK.Create( this, _S( 191, "확인" ), 61, 345, 63, 21 );
	//m_btnOK.SetUV( UBS_IDLE, 0, 440, 63, 461, fTexWidth, fTexHeight );
	//m_btnOK.SetUV( UBS_CLICK, 65, 440, 128, 461, fTexWidth, fTexHeight );
	m_btnOK.Create( this, _S( 191, "확인" ), 39, 366, 92, 21 );
	m_btnOK.SetRTSurface( UBS_IDLE, rcLeft, rtIdleBtn.rtL );
	m_btnOK.SetRTSurface( UBS_IDLE, rcMiddel, rtIdleBtn.rtM );
	m_btnOK.SetRTSurface( UBS_IDLE, rcRight, rtIdleBtn.rtR );
	m_btnOK.SetRTSurface( UBS_CLICK, rcLeft, rtClickBtn.rtL );
	m_btnOK.SetRTSurface( UBS_CLICK, rcMiddel, rtClickBtn.rtM );
	m_btnOK.SetRTSurface( UBS_CLICK, rcRight, rtClickBtn.rtR );
	m_btnOK.CopyRTSurface( UBS_IDLE, UBS_ON );
	m_btnOK.CopyRTSurface( UBS_IDLE, UBS_DISABLE );
	m_btnOK.SetNewType(TRUE);

	// Init interface Button
	int	nStrWidth = ( _S( 200, "인터페이스" ).Length() + 3 ) *
					( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );
	//m_btnInitInterface.Create( this, _S( 194, "초기화" ), OPTION_TEXT_SX + nStrWidth,
	//							OPTION_INITINTERFACE_SY - 5, 63, 21 );
	//m_btnInitInterface.SetUV( UBS_IDLE, 0, 440, 63, 461, fTexWidth, fTexHeight );
	//m_btnInitInterface.SetUV( UBS_CLICK, 65, 440, 128, 461, fTexWidth, fTexHeight );
	m_btnInitInterface.Create( this, _S( 194, "초기화" ), 128, 335, 75, 22 );
	m_btnInitInterface.SetUV( UBS_IDLE, 113, 0, 182, 22, fTexWidth, fTexHeight );
	m_btnInitInterface.SetUV( UBS_CLICK, 186, 0, 256, 22, fTexWidth, fTexHeight );	
	m_btnInitInterface.CopyUV( UBS_IDLE, UBS_ON );
	m_btnInitInterface.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Cancel Button
	//m_btnCancel.Create( this, _S( 139, "취소" ), 126, 345, 63, 21 );
	//m_btnCancel.SetUV( UBS_IDLE, 0, 440, 63, 461, fTexWidth, fTexHeight );
	//m_btnCancel.SetUV( UBS_CLICK, 65, 440, 128, 461, fTexWidth, fTexHeight );
	m_btnCancel.Create( this, _S( 139, "취소" ), 146, 366, 92, 21 );
	m_btnCancel.SetRTSurface( UBS_IDLE, rcLeft, rtIdleBtn.rtL );
	m_btnCancel.SetRTSurface( UBS_IDLE, rcMiddel, rtIdleBtn.rtM );
	m_btnCancel.SetRTSurface( UBS_IDLE, rcRight, rtIdleBtn.rtR );
	m_btnCancel.SetRTSurface( UBS_CLICK, rcLeft, rtClickBtn.rtL );
	m_btnCancel.SetRTSurface( UBS_CLICK, rcMiddel, rtClickBtn.rtM );
	m_btnCancel.SetRTSurface( UBS_CLICK, rcRight, rtClickBtn.rtR );
	m_btnCancel.CopyRTSurface( UBS_IDLE, UBS_ON );
	m_btnCancel.CopyRTSurface( UBS_IDLE, UBS_DISABLE );
	m_btnCancel.SetNewType(TRUE);

	// Enter chat check button
#if defined (G_THAI)
	nStrWidth = FindThaiLen( _S( 198, "엔터 채팅" ) ) + tv_chat;	
#else
	nStrWidth = ( _S( 198, "엔터 채팅" ).Length() + tv_chat ) *
					( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );
#endif
	//m_cbtnEnterChat.Create( this, OPTION_TEXT_SX + nStrWidth , OPTION_ENTERCHAT_SY, 11, 11,
	//						_S( 198, "엔터 채팅" ), TRUE, nStrWidth, nStrWidth );
	//m_cbtnEnterChat.SetUV( UCBS_NONE, 224, 401, 235, 412, fTexWidth, fTexHeight );
	//m_cbtnEnterChat.SetUV( UCBS_CHECK, 198, 401, 209, 412, fTexWidth, fTexHeight );
#if !defined (G_JAPAN)
	m_cbtnEnterChat.Create( this, 121, 144, 13, 13,
							_S( 198, "엔터 채팅" ), TRUE, nStrWidth, nStrWidth );
#else
	m_cbtnEnterChat.Create( this, 121, 144, 13, 13,
							_S( 198, "엔터 채팅" ), TRUE, nStrWidth - 15, nStrWidth );
#endif
	m_cbtnEnterChat.SetUV( UCBS_NONE, 139, 75, 152, 88, fTexWidth, fTexHeight );
	m_cbtnEnterChat.SetUV( UCBS_CHECK, 119, 75, 132, 88, fTexWidth, fTexHeight );
	m_cbtnEnterChat.CopyUV( UCBS_NONE, UCBS_CHECK_DISABLE );
	m_cbtnEnterChat.CopyUV( UCBS_NONE, UCBS_NONE_DISABLE );
	m_cbtnEnterChat.SetTextColor( TRUE, 0xF2F2F2FF );
	m_cbtnEnterChat.SetTextColor( FALSE, 0xF2F2F2FF );
	m_cbtnEnterChat.SetCheck( TRUE );
	// 엔터채팅 강제 [12/27/2012 Ranma]
	g_iEnterChat = 1;

	// Auto attack check button
#if defined (G_THAI)
	nStrWidth = FindThaiLen( _S( 738, "연속 공격" ) ) + tv_auto;	
#else
	nStrWidth = ( strlen( _S( 738, "연속 공격" ) ) + tv_auto ) *				
					( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );
#endif
	//m_cbtnAutoAttack.Create( this, OPTION_TEXT_SX + nStrWidth, OPTION_AUTOATTACK_SY, 11, 11,
	//							 _S( 738, "연속 공격" ), TRUE, nStrWidth, nStrWidth );		
	//m_cbtnAutoAttack.SetUV( UCBS_NONE, 224, 401, 235, 412, fTexWidth, fTexHeight );
	//m_cbtnAutoAttack.SetUV( UCBS_CHECK, 198, 401, 209, 412, fTexWidth, fTexHeight );
	m_cbtnAutoAttack.Create( this, 121, 172, 13, 13,
									_S( 738, "연속 공격" ), TRUE, nStrWidth, nStrWidth );		
	m_cbtnAutoAttack.SetUV( UCBS_NONE, 139, 75, 152, 88, fTexWidth, fTexHeight );
	m_cbtnAutoAttack.SetUV( UCBS_CHECK, 119, 75, 132, 88, fTexWidth, fTexHeight );
	m_cbtnAutoAttack.CopyUV( UCBS_NONE, UCBS_CHECK_DISABLE );
	m_cbtnAutoAttack.CopyUV( UCBS_NONE, UCBS_NONE_DISABLE );
	m_cbtnAutoAttack.SetTextColor( TRUE, 0xF2F2F2FF );
	m_cbtnAutoAttack.SetTextColor( FALSE, 0xF2F2F2FF );
	m_cbtnAutoAttack.SetCheck( FALSE );

	// Auto attack check button
	//nStrWidth = ( strlen( _S( 1682, "명성 공개" ) ) +tv_fame ) * 
	//			( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );
	//m_cbtnFameOpen.Create( this, OPTION_TEXT_SX + nStrWidth, OPTION_FAMEOPEN_SY, 11, 11,
	//					 _S( 1682, "명성 공개" ), TRUE, nStrWidth, nStrWidth );
	//m_cbtnFameOpen.SetUV( UCBS_NONE, 224, 401, 235, 412, fTexWidth, fTexHeight );
	//m_cbtnFameOpen.SetUV( UCBS_CHECK, 198, 401, 209, 412, fTexWidth, fTexHeight );
	//m_cbtnFameOpen.Create( this, 121, 200, 13, 13,
	//					 _S( 1682, "명성 공개" ), TRUE, nStrWidth, nStrWidth );
	//m_cbtnFameOpen.SetUV( UCBS_NONE, 139, 75, 152, 88, fTexWidth, fTexHeight );
	//m_cbtnFameOpen.SetUV( UCBS_CHECK, 119, 75, 132, 88, fTexWidth, fTexHeight );
	//m_cbtnFameOpen.CopyUV( UCBS_NONE, UCBS_CHECK_DISABLE );
	//m_cbtnFameOpen.CopyUV( UCBS_NONE, UCBS_NONE_DISABLE );
	//m_cbtnFameOpen.SetTextColor( TRUE, 0xF2F2F2FF );
	//m_cbtnFameOpen.SetTextColor( FALSE, 0xF2F2F2FF );
	//m_cbtnFameOpen.SetCheck( TRUE );
	//m_cbtnFameOpen.SetEnable( FALSE );


	// wooss 050809
	// move , summon permission
	// ITS #6255 : 러시아 요청으로 인한 옵션 스트링 줄 맞춤 정리. [12/22/2011 rumist]
#if defined (G_THAI)
	nStrWidth = FindThaiLen( _S( 2118, "소환 허가" ) ) + tv_summon;	
#else
	nStrWidth = ( strlen( _S( 2118, "소환 허가" ) ) + tv_summon) * 
				( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );	
#endif
	m_cbtnMovePermit.Create( this, 225, 144, 13, 13,
							_S( 2118, "소환 허가" ), TRUE, nStrWidth, nStrWidth );

	//m_cbtnMovePermit.SetUV( UCBS_NONE, 224, 401, 235, 412, fTexWidth, fTexHeight );
	//m_cbtnMovePermit.SetUV( UCBS_CHECK, 198, 401, 209, 412, fTexWidth, fTexHeight );
	m_cbtnMovePermit.SetUV( UCBS_NONE, 139, 75, 152, 88, fTexWidth, fTexHeight );
	m_cbtnMovePermit.SetUV( UCBS_CHECK, 119, 75, 132, 88, fTexWidth, fTexHeight );
	m_cbtnMovePermit.CopyUV( UCBS_NONE, UCBS_CHECK_DISABLE );
	m_cbtnMovePermit.CopyUV( UCBS_NONE, UCBS_NONE_DISABLE );
	m_cbtnMovePermit.SetTextColor( TRUE, 0xF2F2F2FF );
	m_cbtnMovePermit.SetTextColor( FALSE, 0xF2F2F2FF );
	m_cbtnMovePermit.SetCheck( TRUE );
	m_cbtnMovePermit.SetEnable( FALSE );

	// wooss 070401 ------------------------------------------------------------------->>
	// kw : WSS_HELP_SYSTEM_1

#if defined (G_THAI)
	// TODO : 적용될 때 따로 위치 손 봐야 할듯...
	nStrWidth = FindThaiLen( _S(284, "도움말" ) ) + tv_summon;
#else
	nStrWidth = ( strlen( _S(284, "도움말" ) ) + tv_summon ) * 
				( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );	
#endif
	m_cbtnHelp1Icon.Create( this, 225, 172, 13, 13,
								_S(284, "도움말" ), TRUE, nStrWidth, nStrWidth );

	//m_cbtnHelp1Icon.SetUV( UCBS_NONE, 224, 401, 235, 412, fTexWidth, fTexHeight );
	//m_cbtnHelp1Icon.SetUV( UCBS_CHECK, 198, 401, 209, 412, fTexWidth, fTexHeight );
	m_cbtnHelp1Icon.SetUV( UCBS_NONE, 139, 75, 152, 88, fTexWidth, fTexHeight );
	m_cbtnHelp1Icon.SetUV( UCBS_CHECK, 119, 75, 132, 88, fTexWidth, fTexHeight );
	m_cbtnHelp1Icon.CopyUV( UCBS_NONE, UCBS_CHECK_DISABLE );
	m_cbtnHelp1Icon.CopyUV( UCBS_NONE, UCBS_NONE_DISABLE );
	m_cbtnHelp1Icon.SetTextColor( TRUE, 0xF2F2F2FF );
	m_cbtnHelp1Icon.SetTextColor( FALSE, 0xF2F2F2FF );
	m_cbtnHelp1Icon.SetCheck( TRUE );

	// [7/9/2009 rumist] rejection request message.
#if defined (G_RUSSIA)
	nStrWidth = ( strlen( _S( 4456, "거래 거부" ) ) + tv_summon ) * 
				( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );
#elif (G_THAI)
	nStrWidth = FindThaiLen( _S( 4456, "거래 거부" ) ) + tv_summon;	
#else
	nStrWidth = ( strlen( _S( 4456, "거래 거부" ) ) + tv_summon ) * 
				( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );
#endif
	m_cbtnRejectExchange.Create( this, 225, 200, 13, 13,
							_S( 4456, "거래 거부" ), TRUE, nStrWidth, nStrWidth );
	m_cbtnRejectExchange.SetUV( UCBS_NONE, 139, 75, 152, 88, fTexWidth, fTexHeight );
	m_cbtnRejectExchange.SetUV( UCBS_CHECK, 119, 75, 132, 88, fTexWidth, fTexHeight );
	m_cbtnRejectExchange.CopyUV( UCBS_NONE, UCBS_CHECK_DISABLE );
	m_cbtnRejectExchange.CopyUV( UCBS_NONE, UCBS_NONE_DISABLE );
	m_cbtnRejectExchange.SetTextColor( TRUE, 0xF2F2F2FF );
	m_cbtnRejectExchange.SetTextColor( FALSE, 0xF2F2F2FF );
	m_cbtnRejectExchange.SetCheck( TRUE );
	m_cbtnRejectExchange.SetEnable( TRUE );	

	// [7/9/2009 rumist] rejection request message.
#if defined (G_RUSSIA)
	nStrWidth = ( strlen( _S( 4457, "파티 거부" ) ) + tv_summon - 1 ) * 
				( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );
#elif (G_THAI)
	nStrWidth = FindThaiLen( _S( 4457, "파티 거부" ) ) + tv_summon;	
#else
	nStrWidth = ( strlen( _S( 4457, "파티 거부" ) ) + tv_summon ) * 
				( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );
#endif
	m_cbtnRejectParty.Create( this, 225, 226, 13, 13,
							_S( 4457, "파티/원정대 거부" ), TRUE, nStrWidth, nStrWidth );
	m_cbtnRejectParty.SetUV( UCBS_NONE, 139, 75, 152, 88, fTexWidth, fTexHeight );
	m_cbtnRejectParty.SetUV( UCBS_CHECK, 119, 75, 132, 88, fTexWidth, fTexHeight );
	m_cbtnRejectParty.CopyUV( UCBS_NONE, UCBS_CHECK_DISABLE );
	m_cbtnRejectParty.CopyUV( UCBS_NONE, UCBS_NONE_DISABLE );
	m_cbtnRejectParty.SetTextColor( TRUE, 0xF2F2F2FF );
	m_cbtnRejectParty.SetTextColor( FALSE, 0xF2F2F2FF );
	m_cbtnRejectParty.SetCheck( TRUE );
	m_cbtnRejectParty.SetEnable( TRUE );

#ifdef	G_KOR
		nStrWidth = ( strlen( _S(5589, "퀘스트북 팝업") ) + 1 ) * 
				( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );
#elif (G_THAI)
	nStrWidth = FindThaiLen( _S( 5589, "퀘스트북 팝업" ) ) + tv_summon;	
#else
		nStrWidth = ( strlen( _S(5589, "퀘스트북 팝업") ) + 3 ) * 
				( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );
#endif
	m_cbtnQuestBookOpen.Create(this, 121, 200, 13, 13, _S(5589, "퀘스트북 팝업"), TRUE, nStrWidth, nStrWidth);
	m_cbtnQuestBookOpen.SetUV( UCBS_NONE, 139, 75, 152, 88, fTexWidth, fTexHeight );
	m_cbtnQuestBookOpen.SetUV( UCBS_CHECK, 119, 75, 132, 88, fTexWidth, fTexHeight );
	m_cbtnQuestBookOpen.CopyUV( UCBS_NONE, UCBS_CHECK_DISABLE );
	m_cbtnQuestBookOpen.CopyUV( UCBS_NONE, UCBS_NONE_DISABLE );
	m_cbtnQuestBookOpen.SetTextColor( TRUE, 0xF2F2F2FF );
	m_cbtnQuestBookOpen.SetTextColor( FALSE, 0xF2F2F2FF );
	m_cbtnQuestBookOpen.SetCheck( TRUE );
	m_cbtnQuestBookOpen.SetEnable( TRUE );

	// Fullscreen check button
	// 대만 버젼에서는 제외
	//nStrWidth = ( strlen( _S( 739, "전체 화면" ) ) + 3 ) *				
	//				( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );
	//m_cbtnFullscreen.Create( this, OPTION_TEXT_SX + nStrWidth, OPTION_FULLSCREEN_SY, 11, 11,
	//							 _S( 739, "전체 화면" ), TRUE, nStrWidth, nStrWidth );		
	//m_cbtnFullscreen.SetUV( UCBS_NONE, 224, 401, 235, 412, fTexWidth, fTexHeight );
	//m_cbtnFullscreen.SetUV( UCBS_CHECK, 198, 401, 209, 412, fTexWidth, fTexHeight );
#ifdef G_THAI
	nStrWidth = FindThaiLen( _S( 739, "전체 화면" ) ) + tv_summon;
#else
	nStrWidth = ( strlen( _S( 739, "전체 화면" ) ) + 1 ) *				
					( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );
#endif // (G_THAI)

	// [091119: selo] 전체 화면 글씨 왼쪽으로 나오는거에서 오른쪽으로 나오게 하기
#if ( defined(G_USA) || defined(G_JAPAN) )
	m_cbtnFullscreen.Create( this, 98, 105, 13, 13,
								_S( 739, "전체 화면" ), FALSE, 15);
#else
	m_cbtnFullscreen.Create( this, 98, 105, 13, 13,
									_S( 739, "전체 화면" ), TRUE, nStrWidth, nStrWidth );
#endif
	m_cbtnFullscreen.SetUV( UCBS_NONE, 139, 75, 152, 88, fTexWidth, fTexHeight );
	m_cbtnFullscreen.SetUV( UCBS_CHECK, 119, 75, 132, 88, fTexWidth, fTexHeight );
	m_cbtnFullscreen.CopyUV( UCBS_NONE, UCBS_CHECK_DISABLE );
	m_cbtnFullscreen.CopyUV( UCBS_NONE, UCBS_NONE_DISABLE );
	m_cbtnFullscreen.SetTextColor( TRUE, 0xF2F2F2FF );
	m_cbtnFullscreen.SetTextColor( FALSE, 0xAAAAAAFF );
	m_cbtnFullscreen.SetCheck( TRUE );
		
	// Background check button
	//nStrWidth = ( _S( 207, "배경효과" ).Length() + 3 ) *
	//				( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );
	//m_cbtnBackground.Create( this, OPTION_TEXT_SX + nStrWidth, OPTION_BACKGROUND_SY, 11, 11,
	//							_S( 207, "배경효과" ), TRUE, nStrWidth, nStrWidth );
	//m_cbtnBackground.SetUV( UCBS_NONE, 224, 401, 235, 412, fTexWidth, fTexHeight );
	//m_cbtnBackground.SetUV( UCBS_CHECK, 198, 401, 209, 412, fTexWidth, fTexHeight );
#ifdef G_THAI
	nStrWidth = FindThaiLen( _S( 207, "배경효과" ) ) + tv_summon;
#else
	nStrWidth = ( _S( 207, "배경효과" ).Length() + 1 ) *
					( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );
#endif // (G_THAI)

	if (g_iCountry == MEXICO)
	{
		m_cbtnBackground.Create( this, 99, 282, 13, 13,
								_S( 207, "배경효과" ), FALSE, 15, 0 );
	}
	else
	{
		m_cbtnBackground.Create( this, 99, 282, 13, 13,
								_S( 207, "배경효과" ), TRUE, nStrWidth, nStrWidth );
	}

	m_cbtnBackground.SetUV( UCBS_NONE, 139, 75, 152, 88, fTexWidth, fTexHeight );
	m_cbtnBackground.SetUV( UCBS_CHECK, 119, 75, 132, 88, fTexWidth, fTexHeight );
	m_cbtnBackground.CopyUV( UCBS_NONE, UCBS_CHECK_DISABLE );
	m_cbtnBackground.CopyUV( UCBS_NONE, UCBS_NONE_DISABLE );
	m_cbtnBackground.SetTextColor( TRUE, 0xF2F2F2FF );
	m_cbtnBackground.SetTextColor( FALSE, 0xF2F2F2FF );
	m_cbtnBackground.SetCheck( TRUE );
		
	// --------------------------------------------------------------------------------<<
}

// ----------------------------------------------------------------------------
// Name : ResetPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIOption::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
}

// ----------------------------------------------------------------------------
// Name : AdjustPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIOption::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

// ----------------------------------------------------------------------------
// Name : InitOption()
// Desc :
// ----------------------------------------------------------------------------
void CUIOption::InitOption()
{
	m_cbtnFullscreen.SetEnable( TRUE );
	m_cbtnFullscreen.SetCheck( IsFullScreen( sam_bFullScreenActive) );
		
	// Date : 2005-01-15,   By Lee Ki-hwan
	// 03-14, 잠금 기능 해제
	// 04-08, 중국 원도우 모드 지원
	//if ( g_iCountry == TAIWAN )
	{
		//m_cbtnFullscreen.SetEnable( FALSE );
	//	m_cmbItemPlusEffect.SetEnable ( FALSE );	// Date : 2005-03-03,   By Lee Ki-hwan
	}

// [2011/04/07 : Sora] DX9.0 이상 버전에서는 지원하지 않으므로 비활성화
#if defined(G_RUSSIA) || defined(G_GERMAN) || defined(G_EUROPE3) || defined(G_EUROPE2)
	m_slbBrightness.SetEnable( FALSE );
	m_slbContrast.SetEnable( FALSE );
#else
	m_slbBrightness.SetEnable( IsFullScreen( sam_bFullScreenActive ) );
	m_slbContrast.SetEnable( IsFullScreen( sam_bFullScreenActive ) );
#endif
	// Pixel shader
	m_bPixelShader = TRUE;
	if( !( ( _pGfx->gl_ulFlags & GLF_VERTEXPROGRAM ) && ( _pGfx->gl_ulFlags & GLF_PIXELPROGRAM ) &&
		( _pGfx->gl_ctTextureUnits >= 4 ) ) )
	{
		m_bPixelShader = FALSE;
		m_nReflectionQuality = 0;
		m_slbReflection.SetCurPos( 0 );
		m_slbReflection.SetEnable( FALSE );
		m_nFiltering = 0;
		m_slbFiltering.SetCurPos( 0 );
		m_slbFiltering.SetEnable( FALSE );
	}

	CreateResolutionList();

	m_nBrightness = -1;
	m_nContrast = -1;
	int		i;
	for( i = 0; i < 11; i++ )
	{
		if( m_afBrightness[i] == gfx_fBrightness )
		{
			m_slbBrightness.SetCurPos( i );
			m_nBrightness = i;
		}

		if( m_afContrast[i] == gfx_fContrast )
		{
		m_slbContrast.SetCurPos( i );
			m_nContrast = i;
		}
	}
	if( m_nBrightness == -1 )
	{
		m_nBrightness = 5;
		m_slbBrightness.SetCurPos( m_nBrightness );
	}
	if( m_nContrast == -1 )
	{
		m_nContrast = 5;
		m_slbContrast.SetCurPos( m_nContrast );
	}

	m_nTextureQuality = -1;
	for( i = 0; i < 3; i++ )
	{
		if( m_afTextureQuality[i] == gap_fTextureLODBias )
		{
			m_slbTexture.SetCurPos( i );
			m_nTextureQuality = i;
		}
	}
	if( m_nTextureQuality == -1 )
	{
		m_nTextureQuality = 0;
		m_slbTexture.SetCurPos( m_nTextureQuality );
	}

	m_cbtnBackground.SetCheck( g_bRenderDecoration != 0 );

	m_nViewTerrain = -1;

	float fScale = CZoneInfo::getSingleton()->GetTerMul(g_slZone);
	float fTmpView = 0.f;

	for( i = 0; i < 5; i++ )
	{
		fTmpView = m_afViewTerrain[i] * fScale;

		if( fTmpView == ter_fLODMul )
		{
			m_slbViewTerrain.SetCurPos( i );
			m_nViewTerrain = i;
			break;
		}
	}
	if( m_nViewTerrain == -1 )
	{
		m_nViewTerrain = 0;
		m_slbViewTerrain.SetCurPos( m_nViewTerrain );
	}

	m_slbFiltering.SetCurPos( g_iUseBloom );
	m_nFiltering = g_iUseBloom;

	m_slbShadow.SetCurPos( g_iShadowDetail );
	m_nShadowQuality = g_iShadowDetail;

	m_slbReflection.SetCurPos( g_iReflectionDetail );
	m_nReflectionQuality = g_iReflectionDetail;

//안태훈 수정 시작	//(Open beta)(2004-12-30)
	m_slbBGM.SetCurPos( g_fMscVol * 100 );
	m_nBGMVolume = m_slbBGM.GetCurPos();
	m_slbFXSound.SetCurPos( g_fSndVol * 100 );
	m_nFXVolume = m_slbFXSound.GetCurPos();
//안태훈 수정 끝	//(Open beta)(2004-12-30)

	// default option change to enter chat. [9/22/2009 rumist]
	m_cbtnEnterChat.SetCheck( g_iEnterChat != 0 );
	m_bEnterChat = g_iEnterChat != 0;

	m_cbtnAutoAttack.SetCheck( g_iAutoAttack != 0 );
	m_bAutoAttack = g_iAutoAttack != 0;

	//TEMP:명성공개
//	m_cbtnFameOpen.SetCheck( _pNetwork->MyCharacterInfo.bFameOpen != 0 );

	m_slbShowName.SetCurPos( g_iShowName );
	m_nShowName = g_iShowName;

	m_slbShowNameItem.SetCurPos( g_iShowNameItem );
	m_nShowNameItem = g_iShowNameItem;

	//wooss 050809
	UBYTE tv_byte=_pNetwork->MyCharacterInfo.sbItemEffectOption;

	m_cmbItemPlusEffect.SetCurSel( tv_byte&0x01);

	//wooss 050809
	//default true
		
	m_cbtnMovePermit.SetCheck(!(tv_byte& (0x01 << 1)));
	m_cbtnMovePermit.SetEnable(STAGEMGR()->GetCurStage() == eSTAGE_GAMEPLAY);

	// wooss 070401 ------------------------------------->>
	// kw : WSS_HELP_SYSTEM_1
	m_cbtnHelp1Icon.SetCheck( g_iShowHelp1Icon != 0 );
	m_iShowHelp1Icon = g_iShowHelp1Icon != 0;
	// --------------------------------------------------<<

	// [7/9/2009 rumist] rejection.
	m_cbtnRejectExchange.SetCheck( g_iRejectExchange != 0 );
	m_bRejectExchange = g_iRejectExchange != 0;

	// [7/9/2009 rumist] rejection.
	m_cbtnRejectParty.SetCheck( g_iRejectParty != 0 );
	m_bRejectParty = g_iRejectParty != 0;

	// 1106 UI 개편 - 퀘스트북 열기 [09/05/11 trylord]
	m_cbtnQuestBookOpen.SetCheck(g_iQuestBookOpen != 0 );
	m_bQuestBookOpen	= g_iQuestBookOpen != 0;

	// UI_REFORM :Su-won
	m_cmbTargetDest.SetCurSel(g_bSlaveNoTarget);
}

// ----------------------------------------------------------------------------
// Name : CreateResolutionList()
// Desc :
// ----------------------------------------------------------------------------
void CUIOption::CreateResolutionList()
{
	m_cmbResolution.ResetStrings();
	m_vecPixWidth.clear();
	m_vecPixHeight.clear();
	m_vecPixDepth.clear();

	CDisplayAdapter	&daDisplayAdapter = _pGfx->gl_gaAPI[_pGfx->gl_eCurrentAPI].ga_adaAdapter[_pGfx->gl_iCurrentAdapter];
	INDEX			ctDisplayMode = daDisplayAdapter.da_ctDisplayModes;
	INDEX			ctDisplayCount = 0;

	PIX	pixDepth = 0;
	if( _pGfx->gl_dmCurrentDisplayMode.dm_ddDepth == DISPD_16BIT )
		pixDepth = 16;
	else if( _pGfx->gl_dmCurrentDisplayMode.dm_ddDepth == DISPD_32BIT )
		pixDepth = 32;
	else if( _pGfx->gl_dmCurrentDisplayMode.dm_ddDepth == DISPD_DEFAULT )
	{
		if( _pGfx->gl_d3dColorFormat == D3DFMT_X8R8G8B8 ||
			_pGfx->gl_d3dColorFormat == D3DFMT_A8R8G8B8 )
			pixDepth = 32;
		else if( _pGfx->gl_d3dColorFormat == D3DFMT_R5G6B5 ||
			_pGfx->gl_d3dColorFormat == D3DFMT_X1R5G5B5 ||
			_pGfx->gl_d3dColorFormat == D3DFMT_A1R5G5B5 ||
			_pGfx->gl_d3dColorFormat == D3DFMT_X4R4G4B4 ||
			_pGfx->gl_d3dColorFormat == D3DFMT_A4R4G4B4 )
			pixDepth = 16;
	}

	for( INDEX idm = 0; idm < ctDisplayMode; idm++ )
	{
		CDisplayMode	&dmDisplayMode = daDisplayAdapter.da_admDisplayModes[idm];
		if( dmDisplayMode.dm_pixSizeI >= 800 && dmDisplayMode.dm_pixSizeJ >= 600 )
		{
			if( !m_cbtnFullscreen.IsChecked() &&
				( dmDisplayMode.dm_pixSizeI > m_pixDesktopWidth || dmDisplayMode.dm_pixSizeJ > m_pixDesktopHeight ) )
				continue;

			m_vecPixWidth.push_back( dmDisplayMode.dm_pixSizeI );
			m_vecPixHeight.push_back( dmDisplayMode.dm_pixSizeJ );
			if( m_cbtnFullscreen.IsChecked() )
				m_vecPixDepth.push_back( 16 );
			else
				m_vecPixDepth.push_back( pixDepth );
			ctDisplayCount++;
		}
	}

	if( ctDisplayCount == 0 )
	{
		m_vecPixWidth.push_back( 800 );
		m_vecPixHeight.push_back( 600 );
		if( m_cbtnFullscreen.IsChecked() )
			m_vecPixDepth.push_back( 16 );
		else
			m_vecPixDepth.push_back( pixDepth );
		ctDisplayCount++;
	}

	if( m_cbtnFullscreen.IsChecked() )
	{
		for( INDEX iRes = 0; iRes < ctDisplayCount; iRes++ )
		{
			m_vecPixWidth.push_back( m_vecPixWidth[iRes] );
			m_vecPixHeight.push_back( m_vecPixHeight[iRes] );
			m_vecPixDepth.push_back( 32 );
		}
	}

	CTString	strDisplay;
	for( INDEX iRes = 0; iRes < m_vecPixWidth.size(); iRes++ )
	{
		if( m_cbtnFullscreen.IsChecked() )
			strDisplay.PrintF( "%4dx%-4d %2dbit", m_vecPixWidth[iRes], m_vecPixHeight[iRes], m_vecPixDepth[iRes] );
		else
			strDisplay.PrintF( "%4dx%-4d", m_vecPixWidth[iRes], m_vecPixHeight[iRes] );

		m_cmbResolution.AddString( strDisplay );

		if( m_vecPixWidth[iRes] == sam_iScreenSizeI &&
			m_vecPixHeight[iRes] == sam_iScreenSizeJ &&
			m_vecPixDepth[iRes] == pixDepth )
		{
			m_cmbResolution.SetCurSel( iRes );
		}
	}
}

// ----------------------------------------------------------------------------
// Name : OpenOption()
// Desc :
// ----------------------------------------------------------------------------
void CUIOption::OpenOption()
{
	if( IsVisible() == TRUE )
		return;

	ResetPosition( _pdpMain->dp_MinI, _pdpMain->dp_MinJ,
					_pdpMain->dp_MaxI, _pdpMain->dp_MaxJ );

	CUIManager* pUIManager = CUIManager::getSingleton();

//	m_cbtnFameOpen.SetEnable( pUIManager->GetUIGameState() == UGS_GAMEON );
	m_cmbItemPlusEffect.SetEnable( STAGEMGR()->GetCurStage() == eSTAGE_GAMEPLAY );

	// Set default option
	InitOption();

	// Store current selected resolution
	g_nCurSelResolution = m_cmbResolution.GetCurSel();

	// Set visible
	pUIManager->RearrangeOrder( UI_OPTION, TRUE );
}

// toggle option UI [7/19/2010 rumist]
// ----------------------------------------------------------------------------
// Name : ToggleOption()
// Desc :
// ----------------------------------------------------------------------------
void	CUIOption::ToggleOption()
{
	if( IsVisible() )
		CancelOption();
	else
		OpenOption();
}

// ----------------------------------------------------------------------------
// Name : CloseMainWindow()
// Desc : close the main application window
// ----------------------------------------------------------------------------
static void CloseMainWindow(void)
{
	if (g_web.GetWebHandle() != NULL)
	{
		HWND hTmpDlg = g_web.GetWebHandle();
		g_web.CloseWebPage(hTmpDlg);
		DestroyWindow(hTmpDlg);
	}

	CUIManager::getSingleton()->DestroyRenderTarget();
		
	// if window exists
	//	김영환 클라이언트의 경우 메인 핸들 삭제 안함.
	if(_bClientApp == FALSE)
	{
		if( _hwndMain != NULL )
		{
			// destroy it
			DestroyWindow( _hwndMain );
			_hwndMain = NULL;
		}
	}
}


// ----------------------------------------------------------------------------
// Name : CloseMainWindow()
// Desc : open the main application window for fullscreen mode
// ----------------------------------------------------------------------------
static void OpenMainWindowFullScreen( PIX pixSizeI, PIX pixSizeJ )
{
	//	김영환 : 클라이언트 아닌 겨우에만. 생성.
	if(_bClientApp == FALSE)
	{
		ASSERT( _hwndMain == NULL );
		// create a window, invisible initially
		_hwndMain = CreateWindowEx(	WS_EX_TOPMOST | WS_EX_APPWINDOW,
									"Nksp",
									"",   // title
									WS_POPUP,
									0, 0,
									pixSizeI, pixSizeJ,  // window size
									NULL,
									NULL,
									_hInstanceMain,
									NULL );
		// didn't make it?
		if( _hwndMain == NULL ) FatalError( TRANS( "Cannot open main window!" ) );
	}
	else
	{
		//	김영환 : 윈도우 메인 핸들 최상의 팝업 윈도우 설정.
		SetWindowLong(_hwndMain,GWL_EXSTYLE,WS_EX_TOPMOST | WS_EX_APPWINDOW);
		SetWindowLong(_hwndMain,GWL_STYLE,WS_POPUP);
	}

	_hDlgWeb = CreateDialog(_hInstanceMain, MAKEINTRESOURCE(g_web.GetWebDlgID()), _hwndMain, g_web.GetWebDlgCallBack());

	if( _hDlgWeb==NULL) FatalError(TRANS("Cannot open Web Dialog!"));

	SE_UpdateWindowHandle( _hwndMain, _hDlgWeb);

	// set window title and show it
	// ITS # 6311 : 러시아 스트링 변경 요청. [12/15/2011 rumist]
#if defined (G_RUSSIA)
	swprintf( wchWindowTitle, L"Загрузка" );
#else
	swprintf( wchWindowTitle, L"Loading..." );
#endif
	SetWindowTextW( _hwndMain, wchWindowTitle );
}

// ----------------------------------------------------------------------------
// Name : ResetMainWindowNormal()
// Desc : try to start a new display mode
// ----------------------------------------------------------------------------
static void ResetMainWindowNormal(void)
{
// screen change bug in fullscreen. [9/27/2011 rumist]
	if (sam_bFullScreenActive)
	{
		HDC hDC = GetDC( GetDesktopWindow() );
		int iDesktopBitsPixel = GetDeviceCaps( hDC, BITSPIXEL );
		int iDesktopWidth = GetDeviceCaps( hDC, HORZRES );
		int iDesktopHeight = GetDeviceCaps( hDC, VERTRES );
		ReleaseDC( GetDesktopWindow(), hDC );

		DEVMODE dm;
		memset( &dm, 0, sizeof( dm ) );
		dm.dmSize = sizeof( dm );
		dm.dmPelsWidth  = _pixLastSizeI;
		dm.dmPelsHeight = _pixLastSizeJ;
		dm.dmFields     = DM_PELSWIDTH | DM_PELSHEIGHT;
		ChangeDisplaySettings( &dm, CDS_FULLSCREEN );
	}

	int	pixPosX,pixPosY;
	PIX	pixWidth, pixHeight;

	if( sam_bFullScreenActive )
	{
		pixPosX = 0;
		pixPosY = 0;
		pixWidth = _pixLastSizeI;
		pixHeight = _pixLastSizeJ;
	}
	else 
	{
		// add edges and title bar to window size so client area would have size that we requested
		RECT	rWindow, rClient;
		GetClientRect( _hwndMain, &rClient );
		GetWindowRect( _hwndMain, &rWindow );
		pixWidth  = _pixLastSizeI + (rWindow.right-rWindow.left) - (rClient.right-rClient.left);
		pixHeight = _pixLastSizeJ + (rWindow.bottom-rWindow.top) - (rClient.bottom-rClient.top);
			
		if( cmd_iWindowLeft > 0 )
		{    
			pixPosX = cmd_iWindowLeft > (::GetSystemMetrics(SM_CXSCREEN)) ? (::GetSystemMetrics(SM_CXSCREEN)) : cmd_iWindowLeft;
			cmd_iWindowLeft = -1;
		}
		else
		{
			pixPosX = (::GetSystemMetrics(SM_CXSCREEN) - pixWidth ) / 2;
		}
		if( cmd_iWindowTop > 0 )
		{
			pixPosY = cmd_iWindowTop > (::GetSystemMetrics(SM_CYSCREEN)) ? (::GetSystemMetrics(SM_CYSCREEN)) : cmd_iWindowTop;
			cmd_iWindowTop = -1;
		}
		else
		{
			pixPosY = (::GetSystemMetrics(SM_CYSCREEN) - pixHeight ) / 2;
		}
	}

	// set new window size and show it
	// ITS # 6311 : 러시아 스트링 변경 요청. [12/15/2011 rumist]
#if defined (G_RUSSIA)
	swprintf( wchWindowTitle, L"Загрузка" );
#else
	swprintf( wchWindowTitle, L"Loading..." );
#endif

	SetWindowTextW( _hwndMain, wchWindowTitle );
	if (sam_bFullScreenActive)
		SetWindowPos( _hwndMain, HWND_TOP, pixPosX,pixPosY, pixWidth,pixHeight, SWP_NOZORDER);
	else
		SetWindowPos(_hwndMain, HWND_NOTOPMOST, pixPosX,pixPosY, pixWidth,pixHeight, SWP_SHOWWINDOW | SWP_FRAMECHANGED);
}

// ----------------------------------------------------------------------------
// Name : OpenMainWindowNormal()
// Desc : open the main application window for windowed mode
// ----------------------------------------------------------------------------
static void OpenMainWindowNormal( PIX pixSizeI, PIX pixSizeJ )
{
	_pixLastSizeI = pixSizeI;
	_pixLastSizeJ = pixSizeJ;

	//	김영환 : 클라이어트의 경우에는 윈도우 생성 안함.
	if(_bClientApp == FALSE)
	{
		ASSERT( _hwndMain == NULL );
	
		if (sam_bFullScreenActive)
		{
			// create a window, invisible initially
			_hwndMain = CreateWindowEx(
				WS_EX_TOPMOST | WS_EX_APPWINDOW,
				"Nksp",
				"",   // title
				WS_POPUP,//WS_OVERLAPPED|WS_CAPTION|WS_MINIMIZEBOX|WS_SYSMENU,
				10,10,
				100,100,  // window size
				NULL,
				NULL,
				_hInstanceMain,
				NULL);
		}
		else
		{
			// create a window, invisible initially
			_hwndMain = CreateWindowEx(
				WS_EX_APPWINDOW,
				"Nksp",
				"",   // title
				WS_OVERLAPPED|WS_CAPTION|WS_MINIMIZEBOX|WS_SYSMENU,
				10,10,
				100,100,  // window size
				NULL,
				NULL,
				_hInstanceMain,
				NULL);
		}

		// didn't make it?
		if( _hwndMain == NULL ) FatalError( TRANS( "Cannot open main window!" ) );
	}
	else
	{
		ShowWindow(_hwndMain, SW_HIDE);
		//	김영환 : 메인 윈도우 스타일만 변경.
		if (sam_bFullScreenActive)
		{
			//	풀모드
			SetWindowLong(_hwndMain,GWL_EXSTYLE,WS_EX_TOPMOST | WS_EX_APPWINDOW);
			SetWindowLong(_hwndMain,GWL_STYLE,WS_POPUP);
		}
		else
		{
			//	창모드
			SetWindowLong(_hwndMain,GWL_EXSTYLE,WS_EX_APPWINDOW);
			SetWindowLong(_hwndMain,GWL_STYLE, (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX));
		}
	}

	_hDlgWeb = CreateDialog(_hInstanceMain, MAKEINTRESOURCE(g_web.GetWebDlgID()), _hwndMain, g_web.GetWebDlgCallBack());

	if( _hDlgWeb==NULL) FatalError(TRANS("Cannot open Web Dialog!"));


	SE_UpdateWindowHandle( _hwndMain, _hDlgWeb);

	// set window title
	// ITS # 6311 : 러시아 스트링 변경 요청. [12/15/2011 rumist]
#if defined (G_RUSSIA)
	swprintf( wchWindowTitle, L"Загрузка" );
#else
	swprintf( wchWindowTitle, L"Loading..." );
#endif
	SetWindowTextW( _hwndMain, wchWindowTitle );
	//ResetMainWindowNormal();
	CUIManager::getSingleton()->DestroyRenderTarget();
}

// ----------------------------------------------------------------------------
// Name : TryToSetDisplayMode()
// Desc : try to start a new display mode
// ----------------------------------------------------------------------------
BOOL TryToSetDisplayMode( enum GfxAPIType eGfxAPI, INDEX iAdapter, PIX pixSizeI, PIX pixSizeJ,
							enum DisplayDepth eColorDepth, BOOL bFullScreenMode)
{
	CDisplayMode dmTmp;
	dmTmp.dm_ddDepth = eColorDepth;

	// mark to start ignoring window size/position messages until settled down
	_bWindowChanging = TRUE;
	// remember new settings

	CUIManager* pUIManager = CUIManager::getSingleton();

	// destroy canvas if existing
	if( _pvpViewPortMain != NULL )
	{
		_pGfx->DestroyWindowCanvas( _pvpViewPortMain );
		_pvpViewPortMain = NULL;
		_pdpNormalMain = NULL;
	}

	// close the application window
	CloseMainWindow();
	extern void FinalizeShaders(void);
	FinalizeShaders();	// Effect shader handle 삭제

	if (bFullScreenMode && !sam_bFullScreenActive)
	{
		DEVMODE dm;
		memset( &dm, 0, sizeof( dm ) );
		dm.dmSize = sizeof( dm );
		dm.dmPelsWidth  = pixSizeI;
		dm.dmPelsHeight = pixSizeJ;
		dm.dmFields     = DM_PELSWIDTH | DM_PELSHEIGHT;
		ChangeDisplaySettings( &dm, CDS_FULLSCREEN );
	}
	else if (!bFullScreenMode && sam_bFullScreenActive)
	{
		//ChangeDisplaySettings(&_pGfx->gl_devmode, CDS_RESET);
		ChangeDisplaySettings(NULL, 0);
	}

	sam_bFullScreenActive = (bFullScreenMode==FULLSCREEN_MODE)?FULLSCREEN_MODE:OLD_WINDOW_MODE;

	// try to set new display mode
	BOOL bSuccess;

	if( bFullScreenMode) 
	{
		if( eGfxAPI==GAT_D3D) OpenMainWindowFullScreen( pixSizeI, pixSizeJ);
		bSuccess = _pGfx->SetDisplayMode( eGfxAPI, iAdapter, pixSizeI, pixSizeJ, eColorDepth);
		if( bSuccess && eGfxAPI==GAT_OGL) OpenMainWindowFullScreen( pixSizeI, pixSizeJ);
	} 
	else 
	{
		if( eGfxAPI==GAT_D3D) OpenMainWindowNormal( pixSizeI, pixSizeJ);
		bSuccess = _pGfx->ResetDisplayMode( eGfxAPI);
		if( bSuccess && eGfxAPI==GAT_OGL) OpenMainWindowNormal( pixSizeI, pixSizeJ);
		if( bSuccess && eGfxAPI==GAT_D3D) ResetMainWindowNormal();
	}

	// if new mode was set
	if( bSuccess) {
		// create canvas
		ASSERT( _pvpViewPortMain==NULL);
		ASSERT( _pdpNormalMain==NULL);
		_pGfx->CreateWindowCanvas( _hwndMain, &_pvpViewPortMain, &_pdpNormalMain);

		// erase context of both buffers (for the sake of wide-screen)
		_pdpMain = _pdpNormalMain;
   		if( _pdpMain!=NULL && _pdpMain->Lock()) {
   			_pdpMain->Fill(C_BLACK|CT_OPAQUE);
   			_pdpMain->Unlock();
   			_pvpViewPortMain->SwapBuffers();
   			_pdpMain->Lock();
   			_pdpMain->Fill(C_BLACK|CT_OPAQUE);
   			_pdpMain->Unlock();
   			_pvpViewPortMain->SwapBuffers();
  		}

		// Win7 듀얼 모니터 환경에서 
		// 전체화면 변경 시 서브 모니터에서 전체 화면이 풀린다.
		SetFocus(_hwndMain);

		// if the mode is not working, or is not accelerated
		if( !_pGfx->IsCurrentModeAccelerated() )
		{ // report error
#ifdef _DEBUG
			CPrintF( TRANS("This mode does not support hardware acceleration.\n"));
#endif
			// destroy canvas if existing
			if( _pvpViewPortMain!=NULL) {
 				_pGfx->DestroyWindowCanvas( _pvpViewPortMain);
				_pvpViewPortMain = NULL;
				_pdpNormalMain = NULL;
			}
			// close the application window
			CloseMainWindow();
			// report failure
			return FALSE;
		}

		// Compressed texture
		if( eGfxAPI == GAT_D3D )
		{
			HRESULT	hrDXT1 = _pGfx->gl_pD3D->CheckDeviceFormat( iAdapter,
																D3DDEVTYPE_HAL,
																_pGfx->gl_d3dColorFormat,
																0,
																D3DRTYPE_TEXTURE,
																D3DFMT_DXT1 );
			HRESULT	hrDXT3 = _pGfx->gl_pD3D->CheckDeviceFormat( iAdapter,
																D3DDEVTYPE_HAL,
																_pGfx->gl_d3dColorFormat,
																0,
																D3DRTYPE_TEXTURE,
																D3DFMT_DXT3 );
			HRESULT	hrDXT5 = _pGfx->gl_pD3D->CheckDeviceFormat( iAdapter,
																D3DDEVTYPE_HAL,
																_pGfx->gl_d3dColorFormat,
																0,
																D3DRTYPE_TEXTURE,
																D3DFMT_DXT5 );

			_pGfx->gl_bCompressedTexture = SUCCEEDED( hrDXT1 ) && SUCCEEDED( hrDXT3 ) && SUCCEEDED( hrDXT5 );
		}

		sam_iScreenSizeI = pixSizeI;
		sam_iScreenSizeJ = pixSizeJ;
		sam_iDisplayDepth = eColorDepth;
		sam_iDisplayAdapter = iAdapter;
		sam_iGfxAPI = eGfxAPI;

		// Date : 2005-09-22(오후 8:26:30), By Lee Ki-hwan
		// 타이틀 바 재 설졍 
		pUIManager->SetTitleName( sam_bFullScreenActive, sam_iScreenSizeI, sam_iScreenSizeJ );

		pUIManager->DestroyRenderTarget();
		pUIManager->InitRenderTarget();

		extern void InitializeShaders(void);
		InitializeShaders(); // Effect shader handle 설정			
		// report success
		return TRUE;
	}

	// if couldn't set new mode
	else {
		// close the application window
		CloseMainWindow();
		// report failure
		return FALSE;
	}
}

#ifdef KALYDO
const BOOL CUIOption::ToggleFullscreenMode(const BOOL _bEnableFullScreenMode /* = FALSE  */)
{
	// already apply this option.
	if( _bEnableFullScreenMode == m_bFullscreen )
	{
		return FALSE;
	}	

	if (!m_bFullscreen)
	{
		m_cbtnFullscreen.SetCheck(TRUE);
		m_bFullscreen = TRUE;
	}
	else
	{
		// We need to remember the fullscreen state
		m_cbtnFullscreen.SetCheck(FALSE);
		m_bFullscreen = FALSE;
	}

	return TRUE;
}
#endif

// ----------------------------------------------------------------------------
// Name : ApplyOption()
// Desc :
// ----------------------------------------------------------------------------
void CUIOption::ApplyOption()
{
	// Save option
	m_bEnterChat = m_cbtnEnterChat.IsChecked();
	m_bAutoAttack = m_cbtnAutoAttack.IsChecked();
	m_nShowName = m_slbShowName.GetCurPos();
	m_nShowNameItem = m_slbShowNameItem.GetCurPos();

	m_iShowHelp1Icon = (INDEX)m_cbtnHelp1Icon.IsChecked();    // wooss 070401 WSS_HELP_SYSTEM_1

	m_bRejectExchange = m_cbtnRejectExchange.IsChecked();		// [7/9/2009 rumist] rejection 
	m_bRejectParty = m_cbtnRejectParty.IsChecked();	

	m_bQuestBookOpen	= m_cbtnQuestBookOpen.IsChecked();

	m_nBrightness = m_slbBrightness.GetCurPos();
	m_nContrast = m_slbContrast.GetCurPos();

	m_nTextureQuality = m_slbTexture.GetCurPos();
	m_nShadowQuality = m_slbShadow.GetCurPos();
	m_nCharacterQuality = m_slbCharQuality.GetCurPos();
	m_nViewTerrain = m_slbViewTerrain.GetCurPos();
	m_nReflectionQuality = m_slbReflection.GetCurPos();
	m_nFiltering = m_slbFiltering.GetCurPos();
	m_bBackground = m_cbtnBackground.IsChecked();

	m_nBGMVolume = m_slbBGM.GetCurPos();
	m_nFXVolume = m_slbFXSound.GetCurPos();

	// If resolution is changed or window state is toggled
	PIX	pixDepth = 0;
	if( _pGfx->gl_dmCurrentDisplayMode.dm_ddDepth == DISPD_16BIT )
		pixDepth = 16;
	else if( _pGfx->gl_dmCurrentDisplayMode.dm_ddDepth == DISPD_32BIT )
		pixDepth = 32;
	else if( _pGfx->gl_dmCurrentDisplayMode.dm_ddDepth == DISPD_DEFAULT )
	{
		if( _pGfx->gl_d3dColorFormat == D3DFMT_X8R8G8B8 ||
			_pGfx->gl_d3dColorFormat == D3DFMT_A8R8G8B8 )
			pixDepth = 32;
		else if( _pGfx->gl_d3dColorFormat == D3DFMT_R5G6B5 ||
			_pGfx->gl_d3dColorFormat == D3DFMT_X1R5G5B5 ||
			_pGfx->gl_d3dColorFormat == D3DFMT_A1R5G5B5 ||
			_pGfx->gl_d3dColorFormat == D3DFMT_X4R4G4B4 ||
			_pGfx->gl_d3dColorFormat == D3DFMT_A4R4G4B4 )
			pixDepth = 16;
	}
#ifdef KALYDO
	// 만약 스크린이 변경되었으면 아래 루틴은 우회한다.
	if( ToggleFullscreenMode() )
		return;
#endif
	int	iRes = m_cmbResolution.GetCurSel();
	if( iRes >= 0 )
	{
		if( g_nCurSelResolution != iRes ||
			m_vecPixWidth[iRes] != sam_iScreenSizeI ||
			m_vecPixHeight[iRes] != sam_iScreenSizeJ ||
			( IsFullScreen( sam_bFullScreenActive ) && m_vecPixDepth[iRes] != pixDepth ) ||
			m_cbtnFullscreen.IsChecked() != IsFullScreen( sam_bFullScreenActive ) )
		{
			DisplayDepth	eDD = DISPD_DEFAULT;
			if( m_cbtnFullscreen.IsChecked() )
			{
				if( m_vecPixDepth[iRes] == 0 )
					eDD = DISPD_DEFAULT;
				else if( m_vecPixDepth[iRes] == 16 )
					eDD = DISPD_16BIT;
				else if( m_vecPixDepth[iRes] == 32 )
					eDD = DISPD_32BIT;
			}

			TryToSetDisplayMode( (GfxAPIType)sam_iGfxAPI, sam_iDisplayAdapter,
									m_vecPixWidth[iRes], m_vecPixHeight[iRes], eDD, m_cbtnFullscreen.IsChecked() );
			CreateResolutionList();

			// 해상도가 변경되었다면, UIManager 크기를 갱신해야 한다.
			SE_Get_UIManagerPtr()->InitPos(0, 0, m_vecPixWidth[iRes], m_vecPixHeight[iRes]);
			// Adjust position of UIs
			SE_Get_UIManagerPtr()->AdjustUIPos( _pdpMain );
		}
	}

	// fame opn option
//	BOOL bFameOpen = m_cbtnFameOpen.IsChecked();
	//TEMP:명성공개
/*
	if(_pNetwork->MyCharacterInfo.bFameOpen != bFameOpen)
	{
		_pNetwork->FameOpenReq( bFameOpen );
	}
*/

	// Item plus effect
//	SBYTE	sbItemEffectOption = m_cmbItemPlusEffect.GetCurSel();
//	if( sbItemEffectOption >= 0 &&
//		_pNetwork->MyCharacterInfo.sbItemEffectOption != sbItemEffectOption )
//	{
//		_pNetwork->ItemPlusEffectReq( sbItemEffectOption );
//	}

	// wooss 050809
	// move, summon permission check + // Item plus effect
	// 기존 sbItemEffectOption을 비트 단위로 처리
	// 0x00		Effect	x	Warp	o
	// 0x01				o			o
	// 0x10				x			x
	// 0x11				o			x

	SBYTE	bItemEffectOption = m_cmbItemPlusEffect.GetCurSel();
	BOOL	bMove = m_cbtnMovePermit.IsChecked();
	SBYTE	tv_byte=0x0;
	if(bItemEffectOption) tv_byte = 0x01;  
	if(!bMove) tv_byte |= (0x01 << 1); 	
	
	_pNetwork->ItemPlusEffectReq( tv_byte );
		
	// UI_REFORM :Su-won
	g_bSlaveNoTarget =m_cmbTargetDest.GetCurSel();

	// Set invisible
	CUIManager::getSingleton()->RearrangeOrder( UI_OPTION, FALSE );
}

void CUIOption::ResetDisplay(void)
{
	TryToSetDisplayMode( (GfxAPIType)sam_iGfxAPI, sam_iDisplayAdapter,
							sam_iScreenSizeI, sam_iScreenSizeJ, (enum DisplayDepth)sam_iDisplayDepth, IsFullScreen( sam_bFullScreenActive) );
}

// ----------------------------------------------------------------------------
// Name : CancelOption()
// Desc :
// ----------------------------------------------------------------------------
void CUIOption::CancelOption()
{
	g_iEnterChat = m_bEnterChat;
	g_iAutoAttack = m_bAutoAttack ? 1 : 0;
	g_iShowName = m_nShowName;
	g_iShowNameItem = m_nShowNameItem;

	g_iShowHelp1Icon = m_iShowHelp1Icon ? 1 : 0;			// wooss 070401 WSS_HELP_SYSTEM_1

	g_iRejectExchange = m_bRejectExchange ? 1 : 0;					// [7/9/2009 rumist] rejection 
	g_iRejectParty	= m_bRejectParty ? 1 : 0;

	g_iQuestBookOpen	= m_bQuestBookOpen ? 1 : 0;

	gfx_fBrightness = m_afBrightness[m_nBrightness];
	gfx_fContrast = m_afContrast[m_nContrast];

	gap_fTextureLODBias = m_afTextureQuality[m_nTextureQuality];
	g_iShadowDetail = m_nShadowQuality;

	float fScale = CZoneInfo::getSingleton()->GetTerMul(g_slZone);

	ter_fLODMul = ska_fLODMul = mdl_fLODMul = (m_afViewTerrain[m_nViewTerrain] * fScale);
	g_iReflectionDetail = m_nReflectionQuality;
	g_iUseBloom = m_nFiltering;
	g_bRenderDecoration = m_bBackground ? 1 : 0;

//안태훈 수정 시작	//(5th Closed beta)(0.2)
	g_fMscVol = m_nBGMVolume / 100.0f;
	g_fSndVol = m_nFXVolume / 100.0f;
//안태훈 수정 끝	//(5th Closed beta)(0.2)

	// Set invisible
	CUIManager::getSingleton()->RearrangeOrder( UI_OPTION, FALSE );
}

// ----------------------------------------------------------------------------
// Name : InitInterface()
// Desc :
// ----------------------------------------------------------------------------
void CUIOption::InitInterface()
{
	CUIManager::getSingleton()->ResetUIPos( _pdpMain );
	CUIManager::getSingleton()->ResetUISavePos( _pdpMain );
}

// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
void CUIOption::Render()
{
	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	// Set option texture
	pDrawPort->InitTextureData( m_ptdBaseTexture );

	// Add render regions
	int	nX;
	// Background
	pDrawPort->AddTexture( m_nPosX, m_nPosY, m_nPosX + m_nWidth, m_nPosY + m_nHeight,
										m_rtBackground.U0, m_rtBackground.V0,
										m_rtBackground.U1, m_rtBackground.V1,
										0xFFFFFFFF );
	pDrawPort->AddTexture( m_nPosX +m_rcInnerBackground.Left, m_nPosY +m_rcInnerBackground.Top, 
										m_nPosX +m_rcInnerBackground.Right, m_nPosY +m_rcInnerBackground.Bottom,
										m_rtInnerBackground[m_nCurOptionTab].U0, m_rtInnerBackground[m_nCurOptionTab].V0,
										m_rtInnerBackground[m_nCurOptionTab].U1, m_rtInnerBackground[m_nCurOptionTab].V1,
										0xFFFFFFFF );
	// Render all elements
	pDrawPort->FlushRenderingQueue();
		
	pDrawPort->InitTextureData( m_ptdButtonTexture );

	// Option tab
	for( int iTab = 0; iTab < OPTIONTAB_TOTAL; iTab++ )
	{
		int	nTabPosY = ( OPTION_TAB_HEIGHT + OPTION_TAB_GAP ) * iTab + m_rcOptionTab.Top;
		if( iTab == m_nCurOptionTab )
		{
			/***
			pDrawPort->AddTexture( m_nPosX + m_rcOptionTab.Left, m_nPosY + nTabPosY,
												m_nPosX + m_rcOptionTab.Right, m_nPosY + nTabPosY + OPTION_TAB_HEIGHT,
												m_rtSelOptionTab[iTab].U0, m_rtSelOptionTab[iTab].V0,
												m_rtSelOptionTab[iTab].U1, m_rtSelOptionTab[iTab].V1,
												0xFFFFFFFF );
			***/

			pDrawPort->AddTexture( m_nPosX + m_rcOptionTab.Left, m_nPosY + nTabPosY,
												m_nPosX + m_rcOptionTab.Right, m_nPosY + nTabPosY + OPTION_TAB_HEIGHT,
												m_rtSelOptionTabBackground.U0, m_rtSelOptionTabBackground.V0,
												m_rtSelOptionTabBackground.U1, m_rtSelOptionTabBackground.V1,
												0xFFFFFFFF );
				
			pDrawPort->AddTexture( m_nPosX + m_rcOptionTab.Left +6, m_nPosY + nTabPosY +18,
												m_nPosX + m_rcOptionTab.Right -2, m_nPosY + nTabPosY + OPTION_TAB_HEIGHT -19,
												m_rtSelOptionTab[iTab].U0, m_rtSelOptionTab[iTab].V0,
												m_rtSelOptionTab[iTab].U1, m_rtSelOptionTab[iTab].V1,
												0xFFFFFFFF );
		}
		else
		{
			/***
			pDrawPort->AddTexture( m_nPosX + m_rcOptionTab.Left, m_nPosY + nTabPosY,
												m_nPosX + m_rcOptionTab.Right, m_nPosY + nTabPosY + OPTION_TAB_HEIGHT,
												m_rtOptionTab[iTab].U0, m_rtOptionTab[iTab].V0,
												m_rtOptionTab[iTab].U1, m_rtOptionTab[iTab].V1,
												0xFFFFFFFF );
			***/

			pDrawPort->AddTexture( m_nPosX + m_rcOptionTab.Left, m_nPosY + nTabPosY,
												m_nPosX + m_rcOptionTab.Right, m_nPosY + nTabPosY + OPTION_TAB_HEIGHT,
												m_rtOptionTabBackground.U0, m_rtOptionTabBackground.V0,
												m_rtOptionTabBackground.U1, m_rtOptionTabBackground.V1,
												0xFFFFFFFF );			

			pDrawPort->AddTexture( m_nPosX + m_rcOptionTab.Left +6, m_nPosY + nTabPosY +18,
												m_nPosX + m_rcOptionTab.Right -2, m_nPosY + nTabPosY + OPTION_TAB_HEIGHT -19,
												m_rtOptionTab[iTab].U0, m_rtOptionTab[iTab].V0,
												m_rtOptionTab[iTab].U1, m_rtOptionTab[iTab].V1,
												0xFFFFFFFF );
		}
	}
		
	// Close button
	m_btnClose.Render();

	// OK Button
	m_btnOK.Render();

	// Cancel button
	m_btnCancel.Render();

	// Option controls
	// Game option
	if( m_nCurOptionTab == OPTIONTAB_GAME )
	{
		// Show name slide bar
		//m_slbShowName.Render();

		// Show item name slide bar
		//m_slbShowNameItem.Render();

		// Enter chatting
		m_cbtnEnterChat.Render();

		// Auto attack
		m_cbtnAutoAttack.Render();

		//fame open
//		m_cbtnFameOpen.Render();

		//wooss 050809
		//MovePermit render
		m_cbtnMovePermit.Render();

		// wooss 070401 ----------->><<
		// WSS_HELP_SYSYTEM_1
		m_cbtnHelp1Icon.Render();

		// [7/9/2009 rumist] rejection. 
		m_cbtnRejectExchange.Render();
		m_cbtnRejectParty.Render();

		m_cbtnQuestBookOpen.Render();

	//	if ( g_iCountry == KOREA  ) 03-14, 잠금 기능 해제
		//m_cmbItemPlusEffect.Render();			// Item plus effect

		// Intializing interface button
		m_btnInitInterface.Render();
	}
	// Display option
	else if( m_nCurOptionTab == OPTIONTAB_DISPLAY )
	{
		// Fullscreen check button
		// Date : 2005-01-15,   By Lee Ki-hwan : 대만로컬라이징
		/* Date : 2005-04-08(오전 9:50:11), By Lee Ki-hwan
			MDO 요청으로 중국도 윈도우 모드 지원, 대만 버전일때만 렌더링
		*/
		//if ( g_iCountry != TAIWAN  ) 
			m_cbtnFullscreen.Render();

		// Brightness slide bar
		//m_slbBrightness.Render();

		// Contrast slide bar
		//m_slbContrast.Render();
	}
	// Graphic option
	else if( m_nCurOptionTab == OPTIONTAB_GRAPHIC )
	{
		// Texture slide bar
		//m_slbTexture.Render();

		// Shadow slide bar
		//m_slbShadow.Render();

		// Character quality slide bar
		//m_slbCharQuality.Render();

		// Background option check button
		m_cbtnBackground.Render();

		// View-terrain slide bar
		//m_slbViewTerrain.Render();

		// Reflection slide bar
		//m_slbReflection.Render();

		// Filtering slide bar
		//m_slbFiltering.Render();
	}
	// Audio option
	else
	{
		// BGM slide bar
		//m_slbBGM.Render();

		// FX sound slide bar
		//m_slbFXSound.Render();
	}

	// Render all elements
	pDrawPort->FlushRenderingQueue();

	// Text in option
	//pDrawPort->PutTextEx( _S( 196, "옵션" ), m_nPosX + OPTION_TITLE_TEXT_OFFSETX,
	//									m_nPosY + OPTION_TITLE_TEXT_OFFSETY, 0xFFFFFFFF );
	pDrawPort->PutTextExCX( _S( 196, "옵션" ),
												m_nPosX + OPTION_TITLE_TEXT_CX,
												m_nPosY + OPTION_TITLE_TEXT_CY, 0xFFFFFFFF );

	// Game option
	if( m_nCurOptionTab == OPTIONTAB_GAME )
	{
		pDrawPort->PutTextExCX( _S( 197, "게임" ),
												m_nPosX + OPTION_SUB_TITLE_TEXT_CX,
												m_nPosY + OPTION_SUB_TITLE_TEXT_CY, 0xE1B300FF );

		nX = m_nPosX + OPTION_TEXT_SX;
		pDrawPort->PutTextEx( _S( 199, "캐릭터 이름 표시" ),
											//nX, m_nPosY + OPTION_SHOWNAME_SY, 0xF2F2F2FF );
											nX, m_slbShowName.GetAbsPosY() -20, 0xF2F2F2FF );		
		pDrawPort->PutTextEx( _S( 740, "아이템 이름 표시" ),			
											//nX, m_nPosY + OPTION_SHOWNAME_ITEM_SY, 0xF2F2F2FF );
											nX, m_slbShowNameItem.GetAbsPosY() -20, 0xF2F2F2FF );
	//	if ( g_iCountry == KOREA  ) 03-14, 잠금 기능 해제
		pDrawPort->PutTextEx( _S( 1033, "아이템 업그레이드 효과" ),			
											//nX, m_nPosY + OPTION_ITEMEFFECT_SY, 0xF2F2F2FF );
											nX, m_cmbItemPlusEffect.GetAbsPosY() -20, 0xF2F2F2FF );

		// UI_REFORM :Su-won
		pDrawPort->PutTextEx( _S(4165, "선택가능 대상 설정" ),			
											//nX, m_nPosY + OPTION_TARGETDEST_SY, 0xF2F2F2FF );
											nX, m_cmbTargetDest.GetAbsPosY() -20, 0xF2F2F2FF );

		pDrawPort->PutTextEx( _S( 200, "인터페이스" ),
											//nX, m_nPosY + OPTION_INITINTERFACE_SY, 0xF2F2F2FF );
											nX, m_btnInitInterface.GetAbsPosY(), 0xF2F2F2FF );
	}
	// Video option
	else if( m_nCurOptionTab == OPTIONTAB_DISPLAY )
	{
		pDrawPort->PutTextExCX( _S( 201, "디스플레이" ),
												m_nPosX + OPTION_SUB_TITLE_TEXT_CX,
												m_nPosY + OPTION_SUB_TITLE_TEXT_CY, 0xE1B300FF );

		nX = m_nPosX + OPTION_TEXT_SX;
		pDrawPort->PutTextEx( _S( 202, "해상도" ),
											//nX, m_nPosY + OPTION_RESOLUTION_SY, 0xF2F2F2FF );
											nX, m_cmbResolution.GetAbsPosY() -20, 0xF2F2F2FF );
		pDrawPort->PutTextEx( _S( 203, "밝기" ),
											//nX, m_nPosY + OPTION_BRIGHTNESS_SY,
											nX, m_slbBrightness.GetAbsPosY() -20,
											IsFullScreen( sam_bFullScreenActive ) ? 0xF2F2F2FF : 0xAAAAAAFF );
		pDrawPort->PutTextEx( _S( 204, "대비" ),
											//nX, m_nPosY + OPTION_CONTRAST_SY,
											nX, m_slbContrast.GetAbsPosY() -20 ,
											IsFullScreen( sam_bFullScreenActive ) ? 0xF2F2F2FF : 0xAAAAAAFF );
	}
	// Graphic option
	else if( m_nCurOptionTab == OPTIONTAB_GRAPHIC )
	{
		pDrawPort->PutTextExCX( _S( 741, "그래픽" ),			
												m_nPosX + OPTION_SUB_TITLE_TEXT_CX,
												m_nPosY + OPTION_SUB_TITLE_TEXT_CY, 0xE1B300FF );

		nX = m_nPosX + OPTION_TEXT_SX;
		pDrawPort->PutTextEx( _S( 205, "텍스쳐 퀄리티" ),
											//nX, m_nPosY + OPTION_TEXTURE_SY, 0xF2F2F2FF );
											nX, m_slbTexture.GetAbsPosY() -20, 0xF2F2F2FF );
		pDrawPort->PutTextEx( _S( 206, "그림자 퀄리티" ),
											//nX, m_nPosY + OPTION_SHADOW_SY, 0xF2F2F2FF );
											nX, m_slbShadow.GetAbsPosY() -20, 0xF2F2F2FF );
		pDrawPort->PutTextEx( _S( 209, "캐릭터 퀄리티" ),
											//nX, m_nPosY + OPTION_CHARACTER_SY, 0xAAAAAAFF );
											nX, m_slbCharQuality.GetAbsPosY() -20, 0xAAAAAAFF );
		pDrawPort->PutTextEx( _S( 208, "시야-지형" ),
											//nX, m_nPosY + OPTION_VIEWTERAIN_SY, 0xF2F2F2FF );
											nX, m_slbViewTerrain.GetAbsPosY() -20, 0xF2F2F2FF );
		pDrawPort->PutTextEx( _S( 210, "반사 효과" ),
											//nX, m_nPosY + OPTION_REFLECTION_SY,
											nX, m_slbReflection.GetAbsPosY() -20,
											m_bPixelShader ? 0xF2F2F2FF : 0xAAAAAAFF );
		pDrawPort->PutTextEx( _S( 211, "필터링 효과" ),
											//nX, m_nPosY + OPTION_FILTERING_SY,
											nX, m_slbFiltering.GetAbsPosY() -20,
											m_bPixelShader ? 0xF2F2F2FF : 0xAAAAAAFF );
	}
	// Audio option
	else
	{
		pDrawPort->PutTextExCX( _S( 212, "오디오" ),
												m_nPosX + OPTION_SUB_TITLE_TEXT_CX,
												m_nPosY + OPTION_SUB_TITLE_TEXT_CY, 0xE1B300FF );

		nX = m_nPosX + OPTION_TEXT_SX;
		pDrawPort->PutTextEx( _S( 213, "배경음 볼륨" ),
											//nX, m_nPosY + OPTION_BGM_SY, 0xF2F2F2FF );
											nX, m_slbBGM.GetAbsPosY() -20, 0xF2F2F2FF );									
		pDrawPort->PutTextEx( _S( 214, "효과음 볼륨" ),
											//nX, m_nPosY + OPTION_FXSOUND_SY, 0xF2F2F2FF );
											nX, m_slbFXSound.GetAbsPosY() -20, 0xF2F2F2FF );		
	}

	// Flush all render text queue
	pDrawPort->EndTextEx();


	// Set option texture
	pDrawPort->InitTextureData( m_ptdBaseTexture );

	// UI_REFORM :Su-won
	if( m_nCurOptionTab == OPTIONTAB_GAME )
	{
		// Show name slide bar
		m_slbShowName.Render();

		// Show item name slide bar
		m_slbShowNameItem.Render();

		// 기존처럼 텍스트보다 먼저 렌더링하면 콤보박스창이 텍스트에 가리기 때문에 텍스트보다 나중에 렌더링함.
		m_cmbTargetDest.Render();

		// Render all elements
		pDrawPort->FlushRenderingQueue();
		// Flush all render text queue
		pDrawPort->EndTextEx();

		// Set option texture
		pDrawPort->InitTextureData( m_ptdBaseTexture );
		m_cmbItemPlusEffect.Render();			// Item plus effect		
	}
	else
	// Video option
	if( m_nCurOptionTab == OPTIONTAB_DISPLAY )
	{
		// Brightness slide bar
		m_slbBrightness.Render();

		// Contrast slide bar
		m_slbContrast.Render();

		// Resolution combo box
		m_cmbResolution.Render();
	}

	else if( m_nCurOptionTab == OPTIONTAB_GRAPHIC)
	{
		// Texture slide bar
		m_slbTexture.Render();

		// Shadow slide bar
		m_slbShadow.Render();

		// Character quality slide bar
		m_slbCharQuality.Render();

		// View-terrain slide bar
		m_slbViewTerrain.Render();

		// Reflection slide bar
		m_slbReflection.Render();

		// Filtering slide bar
		m_slbFiltering.Render();
	}
	else
	{
		// BGM slide bar
		m_slbBGM.Render();

		// FX sound slide bar
		m_slbFXSound.Render();
	}
		

	// Render all elements
	pDrawPort->FlushRenderingQueue();


	pDrawPort->InitTextureData( m_ptdButtonTexture );
	// Tool tip
	if( m_bShowToolTip )
	{
		// Render all elements
		pDrawPort->FlushRenderingQueue();

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

		// Text in tool tip
		pDrawPort->PutTextEx( m_strToolTip, m_rcToolTip.Left + 8, m_rcToolTip.Top + 3 );
	}

	// Render all elements
	pDrawPort->FlushRenderingQueue();

	// Flush all render text queue
	pDrawPort->EndTextEx();
}

// ----------------------------------------------------------------------------
// Name : ShowToolTip()
// Desc :
// ----------------------------------------------------------------------------
void CUIOption::ShowToolTip( BOOL bShow, int nToolTipID )
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
		int	nInfoX, nInfoY;

		nOldToolTipID = nToolTipID;
		switch( nToolTipID )
		{
		case OPTIONTAB_GAME:
			m_strToolTip = _S( 197, "게임" );
			break;

		case OPTIONTAB_DISPLAY:
			m_strToolTip = _S( 201, "디스플레이" );
			break;

		case OPTIONTAB_GRAPHIC:
			m_strToolTip = _S( 741, "그래픽" );		
			break;

		case OPTIONTAB_AUDIO:
			m_strToolTip = _S( 212, "오디오" );
			break;

		default:
			m_bShowToolTip = FALSE;
			return;
		}

		int nInfoWidth;
#if defined (G_THAI)
		nInfoWidth = 19 - _pUIFontTexMgr->GetFontSpacing() + FindThaiLen(m_strToolTip);				
#else
		nInfoWidth = 19 - _pUIFontTexMgr->GetFontSpacing() + m_strToolTip.Length() *
						( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );
#endif
		int	nInfoHeight = 22;

		nInfoX = m_nPosX + m_rcOptionTab.Right + 2;
		nInfoY = m_nPosY + m_rcOptionTab.Top +
					( OPTION_TAB_HEIGHT - _pUIFontTexMgr->GetLineHeight() ) / 2;

		CUIManager* pUIManager = CUIManager::getSingleton();

		if( nInfoX + nInfoWidth > pUIManager->GetMaxI() )
			nInfoX = pUIManager->GetMaxI() - nInfoWidth;

		m_rcToolTip.Left = nInfoX;
		m_rcToolTip.Top = nInfoY + ( OPTION_TAB_HEIGHT + OPTION_TAB_GAP ) * nToolTipID;
		m_rcToolTip.Right = m_rcToolTip.Left + nInfoWidth;
		m_rcToolTip.Bottom = m_rcToolTip.Top + nInfoHeight;
	}
}

// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUIOption::MouseMessage( MSG *pMsg )
{
	WMSG_RESULT	wmsgResult;

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
			if( IsInside( nX, nY ) )
				CUIManager::getSingleton()->SetMouseCursorInsideUIs();

			// Move option
			if( bTitleBarClick && ( pMsg->wParam & MK_LBUTTON ) )
			{
				int	ndX = nX - nOldX;
				int	ndY = nY - nOldY;
				nOldX = nX;	nOldY = nY;

				Move( ndX, ndY );

				return WMSG_SUCCESS;
			}
			// Tab region
			else if( IsInsideRect( nX, nY, m_rcOptionTab ) )
			{
				int	nToolTipID = ( nY - m_nPosY - m_rcOptionTab.Top ) /
									( OPTION_TAB_HEIGHT +  OPTION_TAB_GAP );
				ShowToolTip( TRUE, nToolTipID );

				return WMSG_SUCCESS;
			}
			// Game option
			else if( m_nCurOptionTab == OPTIONTAB_GAME )
			{
				// Item plus effect combo box
				if( m_cmbItemPlusEffect.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
				// Show name slide bar
				else if( ( wmsgResult = m_slbShowName.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
						g_iShowName = m_slbShowName.GetCurPos();

					return WMSG_SUCCESS;
				}
				// Show item name slide bar
				else if( ( wmsgResult = m_slbShowNameItem.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
						g_iShowNameItem = m_slbShowNameItem.GetCurPos();

					return WMSG_SUCCESS;
				}
				// Intializing interface button
				else if( m_btnInitInterface.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
				// UI_REFORM :Su-won
				else if( m_cmbTargetDest.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
			}
			// Display option
			else if( m_nCurOptionTab == OPTIONTAB_DISPLAY )
			{
				// Resolusion combo box
				if( m_cmbResolution.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
				// Brightness slide bar
				else if( ( wmsgResult = m_slbBrightness.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
						gfx_fBrightness = m_afBrightness[m_slbBrightness.GetCurPos()];

					return WMSG_SUCCESS;
				}
				// Contrast slide bar
				else if( ( wmsgResult = m_slbContrast.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
						gfx_fContrast = m_afContrast[m_slbContrast.GetCurPos()];

					return WMSG_SUCCESS;
				}
			}
			// Graphic option
			else if( m_nCurOptionTab == OPTIONTAB_GRAPHIC )
			{
				// Texture slide bar
				if( ( wmsgResult = m_slbTexture.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
						gap_fTextureLODBias = m_afTextureQuality[m_slbTexture.GetCurPos()];

					return WMSG_SUCCESS;
				}
				// Shadow slide bar
				else if( ( wmsgResult = m_slbShadow.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
						g_iShadowDetail = m_slbShadow.GetCurPos();

					return WMSG_SUCCESS;
				}
				// Character quality slide bar
				else if( ( wmsgResult = m_slbCharQuality.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					//if( wmsgResult == WMSG_COMMAND )
					//	g_iCharacterDetail = m_slbCharQuality.GetCurPos();

					return WMSG_SUCCESS;
				}
				// View-terrain slide bar
				else if( ( wmsgResult = m_slbViewTerrain.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
					{
						UpdateTerMul();
					}

					return WMSG_SUCCESS;
				}
				// Reflection slide bar
				else if( ( wmsgResult = m_slbReflection.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
						g_iReflectionDetail = m_slbReflection.GetCurPos();

					return WMSG_SUCCESS;
				}
				// Filtering slide bar
				else if( ( wmsgResult = m_slbFiltering.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
						g_iUseBloom = m_slbFiltering.GetCurPos();

					return WMSG_SUCCESS;
				}
			}
			// Audio option
			else
			{
				// BGM slide bar
				if( ( wmsgResult = m_slbBGM.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
						g_fMscVol = m_slbBGM.GetCurPos() / 100.0f;

					return WMSG_SUCCESS;
				}
				// FX sound slide bar
				else if( ( wmsgResult = m_slbFXSound.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
						g_fSndVol = m_slbFXSound.GetCurPos() / 100.0f;

					return WMSG_SUCCESS;
				}
			}
			// Close button
			if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			// OK button
			if( m_btnOK.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			// Cancel button
			if( m_btnCancel.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			
			// Hide tool tip
			ShowToolTip( FALSE );
		}
		break;

	case WM_LBUTTONDOWN:
		{
			if( m_nCurOptionTab == OPTIONTAB_DISPLAY )
			{
				// Resolution combo box
				if( m_cmbResolution.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					return WMSG_SUCCESS;
				}
			}

			if( IsInside( nX, nY ) )
			{
				nOldX = nX;		nOldY = nY;

				// Close button
				if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				// Title bar
				else if( IsInsideRect( nX, nY, m_rcTitle ) )
				{
					bTitleBarClick = TRUE;
				}
				// OK Button
				else if( m_btnOK.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				// Cancel Button
				else if( m_btnCancel.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				// Option tab
				else if( IsInsideRect( nX, nY, m_rcOptionTab ) )
				{
					int	nCurSelTab = ( nY - m_nPosY - m_rcOptionTab.Top ) / OPTION_TAB_HEIGHT;
					if( nCurSelTab >= 0 && nCurSelTab < OPTIONTAB_TOTAL )
						m_nCurOptionTab = nCurSelTab;
				}
				// Game option
				else if( m_nCurOptionTab == OPTIONTAB_GAME )
				{
					// Item plus effect combo box
					if( m_cmbItemPlusEffect.MouseMessage( pMsg ) != WMSG_FAIL )
					{
					}
					// Enter chatting check button
					else if( m_cbtnEnterChat.MouseMessage( pMsg ) != WMSG_FAIL )
					{
						g_iEnterChat = m_cbtnEnterChat.IsChecked() ? 1 : 0;
					}
					// Auto attack check button
					else if( m_cbtnAutoAttack.MouseMessage( pMsg ) != WMSG_FAIL )
					{
						g_iAutoAttack = m_cbtnAutoAttack.IsChecked() ? 1 : 0;
					}
					// Fame open check button
					//else if( m_cbtnFameOpen.MouseMessage( pMsg ) != WMSG_FAIL )
					//{
					//}
					// wooss 070401 WSS_HELP_SYSTEM_1
					else if( m_cbtnHelp1Icon.MouseMessage( pMsg ) != WMSG_FAIL )
					{
						g_iShowHelp1Icon = m_cbtnHelp1Icon.IsChecked() ? 1 : 0;
					}
					else if( m_cbtnQuestBookOpen.MouseMessage( pMsg ) != WMSG_FAIL )
					{
						g_iQuestBookOpen	= m_cbtnQuestBookOpen.IsChecked() ? 1 : 0;
					}
					else if( m_cbtnRejectExchange.MouseMessage( pMsg ) != WMSG_FAIL )
					{
						g_iRejectExchange = m_cbtnRejectExchange.IsChecked() ? 1 : 0;
					}
					else if( m_cbtnRejectParty.MouseMessage( pMsg ) != WMSG_FAIL )
					{
						g_iRejectParty = m_cbtnRejectParty.IsChecked() ? 1 : 0;
					}
					// wooss 050809
					// MovePermit open check button 
					else if( m_cbtnMovePermit.MouseMessage( pMsg ) != WMSG_FAIL )
					{
					}

					// Show name slide bar
					else if( ( wmsgResult = m_slbShowName.MouseMessage( pMsg ) ) != WMSG_FAIL )
					{
						if( wmsgResult == WMSG_COMMAND )
							g_iShowName = m_slbShowName.GetCurPos();
					}
					// Show item name slide bar
					else if( ( wmsgResult = m_slbShowNameItem.MouseMessage( pMsg ) ) != WMSG_FAIL )
					{
						if( wmsgResult == WMSG_COMMAND )
							g_iShowNameItem = m_slbShowNameItem.GetCurPos();
					}
					// Intializing interface button
					else if( m_btnInitInterface.MouseMessage( pMsg ) != WMSG_FAIL )
					{
					}
					// UI_REFORM :Su-won
					if( m_cmbTargetDest.MouseMessage( pMsg ) != WMSG_FAIL )
					{
					}
				}
				// Display option
				else if( m_nCurOptionTab == OPTIONTAB_DISPLAY )
				{
					// Fullscreen check button
					if( ( wmsgResult = m_cbtnFullscreen.MouseMessage( pMsg ) ) != WMSG_FAIL )
					{
						CreateResolutionList();
					}
					// Brightness slide bar
					else if( ( wmsgResult = m_slbBrightness.MouseMessage( pMsg ) ) != WMSG_FAIL )
					{
						if( wmsgResult == WMSG_COMMAND )
							gfx_fBrightness = m_afBrightness[m_slbBrightness.GetCurPos()];
					}
					// Contrast slide bar
					else if( ( wmsgResult = m_slbContrast.MouseMessage( pMsg ) ) != WMSG_FAIL )
					{
						if( wmsgResult == WMSG_COMMAND )
							gfx_fContrast = m_afContrast[m_slbContrast.GetCurPos()];
					}
				}
				// Graphic option
				else if( m_nCurOptionTab == OPTIONTAB_GRAPHIC )
				{
					// Texture slide bar
					if( ( wmsgResult = m_slbTexture.MouseMessage( pMsg ) ) != WMSG_FAIL )
					{
						if( wmsgResult == WMSG_COMMAND )
							gap_fTextureLODBias = m_afTextureQuality[m_slbTexture.GetCurPos()];
					}
					// Shadow slide bar
					else if( ( wmsgResult = m_slbShadow.MouseMessage( pMsg ) ) != WMSG_FAIL )
					{
						if( wmsgResult == WMSG_COMMAND )
							g_iShadowDetail = m_slbShadow.GetCurPos();
					}
					// Character quality slide bar
					else if( ( wmsgResult = m_slbCharQuality.MouseMessage( pMsg ) ) != WMSG_FAIL )
					{
						//if( wmsgResult == WMSG_COMMAND )
						//	g_iCharacterDetail = m_slbCharQuality.GetCurPos();
					}
					// Background check button
					else if( m_cbtnBackground.MouseMessage( pMsg ) != WMSG_FAIL )
					{
						g_bRenderDecoration = m_cbtnBackground.IsChecked() ? 1 : 0;
					}
					// View-terrain slide bar
					else if( ( wmsgResult = m_slbViewTerrain.MouseMessage( pMsg ) ) != WMSG_FAIL )
					{
						if( wmsgResult == WMSG_COMMAND )
						{
							UpdateTerMul();
						}
					}
					// Reflection slide bar
					else if( ( wmsgResult = m_slbReflection.MouseMessage( pMsg ) ) != WMSG_FAIL )
					{
						if( wmsgResult == WMSG_COMMAND )
							g_iReflectionDetail = m_slbReflection.GetCurPos();
					}
					// Filtering slide bar
					else if( ( wmsgResult = m_slbFiltering.MouseMessage( pMsg ) ) != WMSG_FAIL )
					{
						if( wmsgResult == WMSG_COMMAND )
							g_iUseBloom = m_slbFiltering.GetCurPos();
					}
				}
				// Audio option
				else
				{
					// BGM slide bar
					if( ( wmsgResult = m_slbBGM.MouseMessage( pMsg ) ) != WMSG_FAIL )
					{
						if( wmsgResult == WMSG_COMMAND )
							g_fMscVol = m_slbBGM.GetCurPos() / 100.0f;
					}
					// FX sound slide bar
					else if( ( wmsgResult = m_slbFXSound.MouseMessage( pMsg ) ) != WMSG_FAIL )
					{
						if( wmsgResult == WMSG_COMMAND )
							g_fSndVol = m_slbFXSound.GetCurPos() / 100.0f;
					}
				}

				CUIManager::getSingleton()->RearrangeOrder( UI_OPTION, TRUE );
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

				// If option isn't focused
				if( !IsFocused() )
					return WMSG_FAIL;

				// Close button
				if( ( wmsgResult = m_btnClose.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
						CancelOption();

					return WMSG_SUCCESS;
				}
				// OK button
				else if( ( wmsgResult = m_btnOK.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
						ApplyOption();

					return WMSG_SUCCESS;
				}
				// Cancel button
				else if( ( wmsgResult = m_btnCancel.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
						CancelOption();

					return WMSG_SUCCESS;
				}
				// Game option
				else if( m_nCurOptionTab == OPTIONTAB_GAME )
				{
					// Item plus effect combo box
					if( m_cmbItemPlusEffect.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
					// Show name slide bar
					else if( m_slbShowName.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
					// Show item name slide bar
					else if( m_slbShowNameItem.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
					// Intializing interface button
					else if( ( wmsgResult = m_btnInitInterface.MouseMessage( pMsg ) ) != WMSG_FAIL )
					{
						if( wmsgResult == WMSG_COMMAND )
							InitInterface();

						return WMSG_SUCCESS;
					}
					// UI_REFORM :Su-won
					if( m_cmbTargetDest.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
				}
				// Display option
				else if( m_nCurOptionTab == OPTIONTAB_DISPLAY )
				{
					// Resolusion combo box
					if( m_cmbResolution.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
					// Brightness slide bar
					else if( m_slbBrightness.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
					// Contrast slide bar
					else if( m_slbContrast.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
				}
				// Graphic option
				else if( m_nCurOptionTab == OPTIONTAB_GRAPHIC )
				{
					// Texture slide bar
					if( m_slbTexture.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
					// Shadow slide bar
					else if( m_slbShadow.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
					// Character quality slide bar
					else if( m_slbCharQuality.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
						// View-terrain slide bar
					else if( m_slbViewTerrain.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
					// Reflection slide bar
					else if( m_slbReflection.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
					// Filtering slide bar
					else if( m_slbFiltering.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
				}
				// Audio option
				else
				{
					// BGM slide bar
					if( m_slbBGM.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
					// FX sound slide bar
					else if( m_slbFXSound.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
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
			{
				// Game option
				if( m_nCurOptionTab == OPTIONTAB_GAME )
				{
					// Item plus effect combo box
					if( m_cmbItemPlusEffect.MouseMessage( pMsg ) != WMSG_FAIL )
					{
						// Nothing
					}
					// UI_REFORM :Su-won
					if( m_cmbTargetDest.MouseMessage( pMsg ) != WMSG_FAIL )
					{
						// Nothing
					}
				}
				// Video option
				else if( m_nCurOptionTab == OPTIONTAB_DISPLAY )
				{
					// Resolusion combo box
					if( m_cmbResolution.MouseMessage( pMsg ) != WMSG_FAIL )
					{
						// Nothing
					}
				}

				return WMSG_SUCCESS;
			}
		}
		break;

	case WM_MOUSEWHEEL:
		{
			if( IsInside( nX, nY ) )
			{
				// Game option
				if( m_nCurOptionTab == OPTIONTAB_GAME )
				{
					// Item plus effect combo box
					if( m_cmbItemPlusEffect.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;

					// UI_REFORM :Su-won
					if( m_cmbTargetDest.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
				}
				// Video option
				else if( m_nCurOptionTab == OPTIONTAB_DISPLAY )
				{
					// Resolusion combo box
					if( m_cmbResolution.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
				}
			}
		}
		break;
	}

	return WMSG_FAIL;
}

void CUIOption::ChangeWindowMode ()
{	
	if ( IsFullScreen( sam_bFullScreenActive) )
	{
		m_cbtnFullscreen.SetCheck(FALSE);	
		TryToSetDisplayMode( (GfxAPIType)sam_iGfxAPI, sam_iDisplayAdapter,
									sam_iScreenSizeI,sam_iScreenSizeJ, DISPD_DEFAULT, m_cbtnFullscreen.IsChecked() );	
	}	
}

void CUIOption::UpdateTerMul()
{
	float fScale = CZoneInfo::getSingleton()->GetTerMul(g_slZone);
	ter_fLODMul = ska_fLODMul = mdl_fLODMul = (m_afViewTerrain[m_slbViewTerrain.GetCurPos()] * fScale);
}
