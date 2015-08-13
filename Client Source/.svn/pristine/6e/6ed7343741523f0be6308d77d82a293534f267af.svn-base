// ----------------------------------------------------------------------------
//  File : UIOption.h
//  Desc : Created by dongmin
// ----------------------------------------------------------------------------

#ifndef	UIOption_H_
#define	UIOption_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif


// #include <vector>
#include <Engine/Interface/UIButton.h>
#include <Engine/Interface/UICheckButton.h>
#include <Engine/Interface/UIComboBox.h>
#include <Engine/Interface/UISlideBar.h>


// Define tabs
enum OptionTab
{
	OPTIONTAB_GAME =  0,
	OPTIONTAB_DISPLAY,
	OPTIONTAB_GRAPHIC,
	OPTIONTAB_AUDIO,
	OPTIONTAB_TOTAL,
};

//#define	OPTION_TAB_WIDTH			21
//#define	OPTION_TAB_HEIGHT			53
//#define	OPTION_TAB_GAP				3

#define	OPTION_TAB_WIDTH			17
#define	OPTION_TAB_HEIGHT			68
#define	OPTION_TAB_GAP				1


// Define text position
#define	OPTION_TITLE_TEXT_OFFSETX	25
#define	OPTION_TITLE_TEXT_OFFSETY	5
#define OPTION_TITLE_TEXT_CX		126
#define OPTION_TITLE_TEXT_CY		17
#define OPTION_SUB_TITLE_TEXT_CX	126
#define OPTION_SUB_TITLE_TEXT_CY	43
#define	OPTION_TITLE2_TEXT_CX		126
#define	OPTION_TITLE2_TEXT_SY		32
//#define	OPTION_TITLE2_TEXT_SY		17

#define	OPTION_TEXT_SX				35
#define	OPTION_TEXT2_SX				120	//wooss 050809 소환허가
#define	OPTION_CONTROL_SX			40
// 국내의 define값과 대만 태국과의 define값이 다른다.
#define	OPTION_SHOWNAME_SY			54
#define	OPTION_SHOWNAME_ITEM_SY		84
#define	OPTION_ENTERCHAT_SY			119
#define	OPTION_AUTOATTACK_SY		144
#define OPTION_FAMEOPEN_SY			169

#define	OPTION_SUMMON_SY			179  // 태국에만 적용

#define	OPTION_ITEMEFFECT_SY		194
#define	OPTION_INITINTERFACE_SY		305

#define	OPTION_RESOLUTION_SY		54
#define	OPTION_FULLSCREEN_SY		97
#define	OPTION_BRIGHTNESS_SY		122
#define	OPTION_CONTRAST_SY			152

#define	OPTION_TEXTURE_SY			54
#define	OPTION_SHADOW_SY			84
#define	OPTION_CHARACTER_SY			114
#define	OPTION_VIEWTERAIN_SY		144
#define	OPTION_REFLECTION_SY		174
#define	OPTION_FILTERING_SY			204
#define	OPTION_BACKGROUND_SY		241

#define	OPTION_BGM_SY				54
#define	OPTION_FXSOUND_SY			84

#define	OPTION_TARGETDEST_SY		239


// Define distance of showing name
#define	OPTION_NAME_DISTANCE		15.0f


// Define size of option
//#define	OPTION_WIDTH				230
//#define	OPTION_HEIGHT				385
#define	OPTION_WIDTH				254
#define	OPTION_HEIGHT				400


// ----------------------------------------------------------------------------
// Name : CUIOption
// Desc :
// ----------------------------------------------------------------------------
class ENGINE_API CUIOption : public CUIWindow
{
protected:
	// Controls
	CUIButton			m_btnClose;							// Close button
	CUIButton			m_btnOK;							// OK button
	CUIButton			m_btnCancel;						// Cancel button

	CUICheckButton		m_cbtnEnterChat;					// Enter chatting check button
	CUICheckButton		m_cbtnAutoAttack;					// Auto attack check button
	CUICheckButton		m_cbtnFameOpen;						// Fame open check button
	CUICheckButton		m_cbtnMovePermit;					// Move or Summon permit button	wooss 050809
	CUICheckButton		m_cbtnHelp1Icon;					// Help1 Icon 
	CUICheckButton		m_cbtnRejectExchange;				// Rejection of request message that exchange
	CUICheckButton		m_cbtnRejectParty;					// and party.
	CUICheckButton		m_cbtnQuestBookOpen;				// 퀘스트 수락시 퀘스트북 여는 여부 [09/05/11 trylord]

	CUISlideBar			m_slbShowName;						// Show name slide bar
	CUISlideBar			m_slbShowNameItem;					// Show item name slide bar
	CUIComboBox			m_cmbItemPlusEffect;				// Show item plus effect
	CUIButton			m_btnInitInterface;					// Intializing interface button

	CUIComboBox			m_cmbResolution;					// Resolusion combo box
	CUISlideBar			m_slbBrightness;					// Brightness slide bar
	CUISlideBar			m_slbContrast;						// Contrast slide bar
	CUICheckButton		m_cbtnFullscreen;					// Fullscreen check button

	CUISlideBar			m_slbTexture;						// Texture slide bar
	CUISlideBar			m_slbShadow;						// Shadow slide bar
	CUISlideBar			m_slbCharQuality;					// Character quality slide bar
	CUISlideBar			m_slbViewTerrain;					// View-terrain slide bar
	CUISlideBar			m_slbReflection;					// Reflection slide bar
	CUISlideBar			m_slbFiltering;						// Filtering slide bar
	CUICheckButton		m_cbtnBackground;					// Background option check button

	CUISlideBar			m_slbBGM;							// BGM slide bar
	CUISlideBar			m_slbFXSound;						// FX sound slide bar

	CUIComboBox			m_cmbTargetDest;					// UI_REFORM :Su-won -선택가능 대상 설정 콤보 박스

	int					m_nCurOptionTab;					// Current tab of option ( game, video, audio )
	BOOL				m_bShowToolTip;						// If tool tip is shown or not
	CTString			m_strToolTip;						// String of tool tip

	// Selected option information
	BOOL				m_bPixelShader;
	BOOL				m_bEnterChat;
	BOOL				m_bAutoAttack;
	INDEX				m_iShowHelp1Icon;					// wooss 070401 kw : WSS_HELP_SYSTEM_1
	BOOL				m_bFameOpen;
	BOOL				m_bRejectExchange;						// [7/9/2009 rumist] rejection options. 
	BOOL				m_bRejectParty;
	BOOL				m_bQuestBookOpen;
	int					m_nShowName;
	int					m_nShowNameItem;
	int					m_nBrightness;
	int					m_nContrast;
	int					m_nTextureQuality;
	int					m_nShadowQuality;
	int					m_nCharacterQuality;
	int					m_nViewTerrain;
	int					m_nViewCharacter;
	int					m_nReflectionQuality;
	int					m_nFiltering;
	BOOL				m_bBackground;
	int					m_nBGMVolume;
	int					m_nFXVolume;

	// Preset option data
	FLOAT				m_afBrightness[11];
	FLOAT				m_afContrast[11];
	FLOAT				m_afTextureQuality[3];
	FLOAT				m_afViewTerrain[5];
	FLOAT				m_afViewCharacter[5];

	// Display information
	PIX					m_pixDesktopWidth;
	PIX					m_pixDesktopHeight;
	std::vector<PIX>	m_vecPixWidth;
	std::vector<PIX>	m_vecPixHeight;
	std::vector<PIX>	m_vecPixDepth;

	// Region of each part
	UIRect				m_rcTitle;							// Region of title
	UIRect				m_rcOptionTab;						// Region of option tab
	UIRect				m_rcToolTip;						// Region of tool tip
	UIRect				m_rcInnerBackground;				// Region of innder background

	// UV of each part
	UIRectUV			m_rtBackground;						// UV of background
	UIRectUV			m_rtInnerBackground[OPTIONTAB_TOTAL];		// UV of inner background
	UIRectUV			m_rtOptionTab[OPTIONTAB_TOTAL];		// UV of option tab
	UIRectUV			m_rtSelOptionTab[OPTIONTAB_TOTAL];	// UV of selected option tab
	UIRectUV			m_rtOptionTabBackground;			// UV of option background tab
	UIRectUV			m_rtSelOptionTabBackground;			// UV of selected option backtground tab
	UIRectUV			m_rtToolTipL;						// UV of left region of tool tip
	UIRectUV			m_rtToolTipM;						// UV of left region of tool tip
	UIRectUV			m_rtToolTipR;						// UV of left region of tool tip

	CTextureData		*m_ptdButtonTexture;		// Texture of Button

protected:
	// Internal functions
	void	CreateResolutionList();
	void	ShowToolTip( BOOL bShow, int nToolTipID = -1 );

public:
	CUIOption();
	~CUIOption();

	// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );

	// Render
	void	Render();

	// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	// Open option
	void	OpenOption();
	// toggle support. [7/19/2010 rumist]
	// Toggle option
	void	ToggleOption();

	// Init option
	void	InitOption();

	// Init interface
	void	InitInterface();

	// Apply & cancel option
	void	ApplyOption();
	void	CancelOption();

	// Set desktop size
	void	SetDesktopSize( PIX pixWidth, PIX pixHeigth )
	{
		m_pixDesktopWidth = pixWidth;
		m_pixDesktopHeight = pixHeigth;
	}
	
	void	ResetDisplay(void);

	// Get texture quality
	FLOAT	GetTextureQuality( int nStep ) const { return m_afTextureQuality[nStep]; }

	// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );

	//윈도우 모드로 강제 변경 added by sam 11/01/17
	void ChangeWindowMode ();

	void UpdateTerMul();
#ifdef KALYDO
	const BOOL	ToggleFullscreenMode(const BOOL _bEnableFullScreenMode = FALSE );
	BOOL	m_bFullscreen;
#endif
};

#endif // UISELCHAR_H_

