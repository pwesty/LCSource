// ----------------------------------------------------------------------------
//  File : UIRadar.h
//  Desc : Created by yjpark
// ----------------------------------------------------------------------------

#ifndef	UIRADAR_H_
#define	UIRADAR_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif


// #include <Engine/Interface/UIButton.h>
#include <Engine/Interface/UICheckButton.h>


// Radar option
enum RadarOption
{
	RADAR_PARTY			= 0,
	RADAR_GUILD,
	RADAR_NPC,
	RADER_QUESTNPC,
	RADER_QUESTSUCCESS,
	RADAR_MOB,
	RADAR_EXPEDITION,	// [sora] 원정대원 표시
	RADAR_OPTION_TOTAL,
};

#define	RADAR_WIDTH						140
#define	RADAR_HEIGHT					53


// ----------------------------------------------------------------------------
// Name : CUIRadar
// Desc :
// ----------------------------------------------------------------------------
class CUIMapOption;
class CUIRadar : public CUIWindow
{
	typedef struct _SSignal
	{
		FLOAT	fX, fZ;
		DOUBLE	dStartTime;
		BOOL	bVisible;
	} SSignal;

	class CIconName
	{
		public:
			UIRect rc;
			CTString name;
		
			void SetIconName( int left, int top, int right, int bottom, CTString name )
			{
				rc.SetRect(left, top, right, bottom);
				this->name = name;
			}

			void Clear()
			{
				rc.SetRect(0,0,0,0);
				name = "";
			}

			BOOL IsEmpty()
			{
				return name.IsEmpty();
			}
	};

public:
	CUIRadar();
	~CUIRadar();

	// Create & destroy
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );

	// Render
	void	Render();

	void	FocusLeave();

	// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );
	WMSG_RESULT KeyMessage( MSG *pMsg );

	// Message Box event catcher.
	void	MsgBoxLCommand( int nCommandCode, int nResult );
	void	MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput );

	// 공성 Signal 
	void SetSignal( float fX, float fZ );
	void SendSignal( float fX, float fZ );
	bool RadarToWorld( float& fX, float& fZ );
	bool IsPossibleSignal();
	bool IsPossibleCastleRadar();
	void RenderSignal();
	void SetSignalOn( bool bOn );

	// Set Time 
	inline void SetTime(int year=0,int month =0 ,int day = 0,int hour = 0,LONG startTime=0) { m_year = year, m_month =month,m_day =day, m_hour = hour, m_startTime = startTime; 	}
	inline time_t GetStartTime() { return m_startTime;}
	// Get cipher
	int CountCipher( int tv_time, int tv_cipher = 1 );
	void DisplayNum( int tv_time,UIRect tv_rect);

	void	RoyalRumbleJoinReqMsgBox();
	void	SetRoyalRumbleStatus( const BOOL bEnable )										{ m_bEnableRR = bEnable;												}	
	void	SetRoyalRumbleSandglass( const UBYTE sandglassStat, const INDEX remainTime = 0 )
	{ m_btSandlassStat = sandglassStat; m_liEndTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds() + remainTime*1000;									}
	void	ResetRoyalRumbleStat()															{ m_btSandlassStat = 3;	m_bEnableRR = FALSE;							}
	void	ShowRoyalRumbleTrophy();
	ENGINE_API void	OpenRoyalrumbleMgrMenu( const INDEX iMobIndex );

	const int		GetHour() const	{ return m_hour; } 
	void	AddIconName(int left, int top, int right, int bottom, CTString name);		// [2012/10/11 : Sora] 월드맵 개편

	void	OnExpressNotice();
	void	OffExpressNotice();

	// PremiumChar
	void	SetPremiumCharBenefit(bool bPremiumChar);
		
	CUIMapOption* GetMapOption() { return m_pMapOption;	}
protected:

	// Internal functinos
	void	RenderObjectLocation();
	void	ShowToolTip( BOOL bShow, int nToolTipID = -1 );

	void	_royalrumbleCreate();
	void	_royalrumbleButtonRender();
	void	_royalrumbleTimeRender();

	// royal rumble showing functions [5/18/2011 rumist]
	void	_initTrophyData();
	void	_createTrophyData();
	void	_renderTrophy();
	void	_destroyTrophy();
	void	_showWinnerMsgBox();
	void	_showRemainTime();

	void	RenderCurrentMap();		// [2012/10/11 : Sora] 월드맵 개편	
	BOOL	IsRadarUse();			// [2012/10/11 : Sora] 월드맵 개편
	void	RenderMyPosition();		// [2012/10/11 : Sora] 월드맵 개편
	void	RenderGPS(CDrawPort* pDraw);

	// Time 
	int m_year;
	int m_month;
	int m_day;
	int m_hour;
	LONG m_startTime;
//	int m_cipher;

	int	m_nZone;											// [2012/10/11 : Sora] 월드맵 개편
	int	m_nLayer;											// [2012/10/11 : Sora] 월드맵 개편
	FLOAT m_fZoomRate;										// [2012/10/11 : Sora] 월드맵 개편
	int	m_nSelectedIcon;									// [2012/10/11 : Sora] 월드맵 개편
	std::vector<CIconName>	m_iconVec;						// [2012/10/11 : Sora] 월드맵 개편
	UIRectUV				m_rtSubZone[9];					// [2012/10/11 : Sora] 월드맵 개편
	UIRect					m_rcSubZone[9];					// [2012/10/11 : Sora] 월드맵 개편


	// Signal 
	SSignal	m_Signal;

	// Controls
	CUIButton			m_btnOption;						// Option button
	CUIButton			m_btnMap;							// Map button
	CUIButton			m_btnSignal;							// Map button

	CUIButton			m_btnZoomIn;						// [2012/10/11 : Sora] 월드맵 개편
	CUIButton			m_btnZoomOut;							// [2012/10/11 : Sora] 월드맵 개편

	CUICheckButton		m_cbtnOption[RADAR_OPTION_TOTAL];	// Option check buttons
	BOOL				m_bShowOptionPopup;					// If popup of radar option is shown or not
	BOOL				m_bShowToolTip;						// If tool tip is shown or not
	BOOL				m_bSignalBtnOn;						// 시그널 버튼 사용 
	BOOL				m_bInsideMouse;						// 마우스가 해당 윈도우 안에 있는지 
	// String
	char				m_szCoord[9];						// String of coordinate
	CTString			m_strToolTip;						// String of tool tip
	CTString			m_strTimer;						// String of tool tip
	COLOR				m_colTooltip;
	COLOR				m_colTooltipTimer;

	// Region of each part
	UIRect				m_rcTop;							// Region of top background
	UIRect				m_rcCompassIn;						// Region of inner compass
	UIRect				m_rcCompassOut;						// Region of outer compass
	UIRect				m_rcOption;							// Region of option
	UIRect				m_rcPoint[RADAR_OPTION_TOTAL];		// Region of point of objects
	UIRect				m_rcMyPoint;						// Region of my point
	UIRect				m_rcTarget;							// Region of target object
	UIRect				m_rcSignal;							// Region of Signal object
	UIRect				m_rcOutSignal;						// Region of Out Signal ( Direction ) object

	UIRect				m_rcToolTip;						// Region of tool tip
	int					m_nOptionIconSX;					// Position x of option icon

	// UV of each part
	UIRectUV			m_rtBackTop;						// UV of top background
	UIRectUV			m_rtCompassIn;						// UV of inner compass
	UIRectUV			m_rtCompassOut;						// UV of outer compass
	UIRectUV			m_rtOptionL;						// UV of left region of option popup
	UIRectUV			m_rtOptionM;						// UV of middle region of option popup
	UIRectUV			m_rtOptionR;						// UV of right region of option popup
	UIRectUV			m_rtPoint[RADAR_OPTION_TOTAL];		// UV of point of objects
	UIRectUV			m_rtMyPoint;						// UV of my point
	UIRectUV			m_rtTarget;							// UV of point of target object
	UIRectUV			m_rtSignal;							// UV of Signal object
	UIRectUV			m_rtOutSignal;						// UV of Out Signal ( Direction ) object
	UIRectUV			m_rtTargetAni;						// UV of animation point of target object
	UIRectUV			m_rtToolTipL;						// UV of left region of tool tip
	UIRectUV			m_rtToolTipM;						// UV of middle region of tool tip
	UIRectUV			m_rtToolTipR;						// UV of right region of tool tip

	//RAID_SYSTEM
	UIRectUV			m_rtPointExpedition[4];				// [sora] 원정대 레이더 표시 이미지

	// time rect
	UIRectUV			m_rtFigure[10];

	// -------------------------------------------------------------------<<
	
	CTextureData		*m_ptdButtonTexture;			// Texture of Button

	CTextureData		*m_ptdRoyalRumbleTexture;		// royal rumble texture [4/25/2011 rumist]

	CTextureData		*m_ptdMapTexture;				// [2012/10/11 : Sora] 월드맵 개편

	CTextureData		*m_ptdMapObjTexture;			// [2012/10/11 : Sora] 월드맵 개편

	CUIButton			m_btnRR;						// royal rumble button.
	UIRect				m_rcBtnRR;						// royal rumble message catch rect.
	UIRectUV			m_rtSandglassRR[3];				// royal rumble sand glass UV rect.
	UIRect				m_rcSandglassRR;				// royal rumble sand glass rect.
	UBYTE				m_btSandlassStat;				// render type
	__int64				m_liEndTime;					// royal rumble left time at join.
	BOOL				m_bShowLeftTimeTooltip;			// royal rumble left time showing flag;
	BOOL				m_bEnableRR;					// enable royal rumble flag.

	// royal rumble winner trophy [5/18/2011 rumist]
	CTextureData*		m_ptdTrophyTexture;			
	BOOL				m_bShowTrophy;
	BOOL				m_bRenderTrophy;
	UIRect				m_rcTrophy;
	UIRectUV			m_rtTrophy;
	UIRectUV			m_rtTrophyEffect;
	TIME				m_tStartTime;
	UBYTE				m_ubTrophyAlpha;
	UBYTE				m_ubTrophyEffectAlpha;

private:
	UIRectUV			m_ExpressNotice;
	DWORD				m_nStartTime;
	bool				m_bNotice;
	bool				m_bNoticeRenderFlag;
	bool				m_bRemote;

	CUIButton			m_ExpressIcon;
	CUIButton			m_PetStashIcon;
	CUIButton			m_ItemCollectionIcon;
	CUIButton			m_PremiumCharIcon;
	UIRectUV			m_uvEmptyMenu;		// 펫창고, LCE버튼 옆에 빈 공간을 채울 이미지.
	UIRect				m_rcEmptyMenu;

	CTString			m_strLocalTime;
	CTString			m_strLocalDay;
	int					m_nLocalTimeY;
	CUIMapOption*		m_pMapOption;

	UIRect				m_rcGPS;
	UIRectUV			m_uvGPS;
};

class CUIRoyalRumbleIcon : public CUIWindow
{
public:
	enum __tagRoyalrumbleUI
	{
		RRUI_POS_X	= 10,
		RRUI_POS_Y	= 200,
		RRUI_WIDTH	= 112,
		RRUI_HEIGHT = 48
	};
	CUIRoyalRumbleIcon();
	~CUIRoyalRumbleIcon();
	
	void				Create(CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );
	void				Render();

	void				ResetPosition(PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void				AdjustPosition(PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	
	// message pump.
	WMSG_RESULT			MouseMessage(MSG *pMsg );

	void				SetLeftCount( const INDEX iLeftCount )								{ m_iLeftCount = iLeftCount; ShowRoyalRumbleIcon(TRUE);	}
	void				ShowRoyalRumbleIcon( BOOL bShow );
	
private:
	void				_setShowColor();

	BOOL				m_bPickTitle;
	CTextureData		*m_ptdRoyalRumbleNumberTexture;	// royal rumble numbering texture [5/11/2011 rumist]
	UIRectUV			m_rtBackground;					// background uv.
	UIRect				m_rcTitle;	
	INDEX				m_iLeftCount;					// left count					
	COLOR				m_colShowColor;					// left count color.
};


#endif	// UIRADAR_H_

