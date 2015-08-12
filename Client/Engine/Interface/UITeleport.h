// ----------------------------------------------------------------------------
//  File : UITeleport.h
//  Desc : Created by yjpark
// ----------------------------------------------------------------------------

#ifndef	UITELEPORT_H_
#define	UITELEPORT_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif

// 
// #include <Engine/Interface/UIButton.h>
// #include <Engine/Interface/UIScrollBar.h>

#define	TELEPORT_MAX_MEMO			15
// [KH_070315] 프리미엄은 20개
#define	TELEPORT_PRIMIUM_MAX_MEMO	20
// wooss 050810
#define	TELEPORT_MAX_ROW			5


// Define text position
#define	TELEPORT_TITLE_OFFSETX		25
#define	TELEPORT_TITLE_OFFSETY		5
#define	TELEPORT_CONTENT_NAME_SY	32
#define	TELEPORT_CONTENT_SY			50
#define	TELEPORT_CONTENT_OFFSETY	15
#define	TELEPORT_NUM_SX				19  
#define	TELEPORT_ZONE_CX			88
#define	TELEPORT_COORD_CX			173
#define	TELEPORT_MEMO_CX			309
#define	TELEPORT_MEMO_SX			212

//wooss 050810
#define	TELEPORT_REMAIN_CX			440
#define	TELEPORT_TIME_SX			420 // wooss

// Define size of teleport
#define	TELEPORT_WIDTH				472	// wooss 050810 427 -> 472
#define	TELEPORT_HEIGHT				160

// [KH_070315] 프리미엄 메모리 스크롤 관련 추가
#define	TELEPORT_PRIMIUM_TITLE_OFFSETX		TELEPORT_TITLE_OFFSETX				// 25
#define	TELEPORT_PRIMIUM_TITLE_OFFSETY		TELEPORT_TITLE_OFFSETY		+ 2		// 7
#define	TELEPORT_PRIMIUM_CONTENT_NAME_SY	TELEPORT_CONTENT_NAME_SY	+ 20	// 52
#define	TELEPORT_PRIMIUM_CONTENT_SY			TELEPORT_CONTENT_SY			+ 20	// 70
#define	TELEPORT_PRIMIUM_CONTENT_OFFSETY	TELEPORT_CONTENT_OFFSETY			// 15
#define	TELEPORT_PRIMIUM_NUM_SX				TELEPORT_NUM_SX						// 19
#define	TELEPORT_PRIMIUM_ZONE_CX			TELEPORT_ZONE_CX					// 88
#define	TELEPORT_PRIMIUM_COORD_CX			TELEPORT_COORD_CX					// 173
#define	TELEPORT_PRIMIUM_MEMO_CX			TELEPORT_MEMO_CX					// 309
#define	TELEPORT_PRIMIUM_MEMO_SX			TELEPORT_MEMO_SX					// 212

#define	TELEPORT_PRIMIUM_REMAIN_CX			TELEPORT_REMAIN_CX					// 440
#define	TELEPORT_PRIMIUM_TIME_SX			TELEPORT_TIME_SX					// 420

#define	TELEPORT_PRIMIUM_WIDTH				429									// 429
#define	TELEPORT_PRIMIUM_HEIGHT				TELEPORT_HEIGHT + 20				// 182


// ----------------------------------------------------------------------------
// Name : CUITeleport
// Desc :
// ----------------------------------------------------------------------------
class CUITeleport : public CUIWindow
{
protected:
	// Control
	CUIButton			m_btnClose;							// Close button
	CUIButton			m_btnMove;							// Move button
	CUIButton			m_btnMemorize;						// Memorize button
	CUIButton			m_btnCancel;						// Cancel button
	SBYTE				m_sbSelection;						// Index of selected content
	SBYTE				m_sbPage;								// [KH_070316] Index of selected Page : 프리미엄에서만 쓴다.
	
	//wooss 050810
	CUIScrollBar		m_sbScrollBar;						// Scroll Bar
	
	// String
// [KH_070316] 프리미엄과 같이 쓰므로 20개(TELEPORT_PRIMIUN_MAX_MEMO)로 잡음
	CTString			m_strZone[TELEPORT_PRIMIUM_MAX_MEMO];	// Strings of zone name
	CTString			m_strCoord[TELEPORT_PRIMIUM_MAX_MEMO];	// Strings of coordinate
	CTString			m_strMemo[TELEPORT_PRIMIUM_MAX_MEMO];	// Strings of memo
	int					m_nZoneNum[TELEPORT_PRIMIUM_MAX_MEMO];	// Numbers of zone
	int					m_useTime;							// is used memscroll_ex item
	BOOL				m_bPrimium;

	// Region of each part
	UIRect				m_rcTitle;							// Region of title
	UIRect				m_rcContents;						// Region of contents

	// UV of each part
	UIRectUV			m_rtBackground;						// UV of background
	UIRectUV			m_rtSelBar;							// UV of selection bar

protected:
	// Internal functions
	void	OpenMemorizeMsgBox();

	// Network message functions ( send )
	void	SendTeleportWrite();
	void	SendTeleportMove();

public:
	CUITeleport();
	~CUITeleport();

	// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );

	// Render
	void	Render();

	// Open & close teleport
	void	OpenTeleport();
	void	CloseTeleport();

	// wooss 050816 
	// expire use-time
	void	NotUseMemEx(); 	

	// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );

	// Network message functions ( receive )
	// wooss 050816 add useTime 
	void	SetTeleportInfo( UBYTE ubSlot, SLONG slZone, FLOAT fX, FLOAT fZ, CTString &strComment );
	void	SetUseTime(int t);
	int		GetUseTime() { return m_useTime; }

	// Command functions
	void	ClearTeleportList();
	void	ShowTeleportError();
	void	MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput );
};

// [KH_070315] 프리미엄 메모리스크롤 관련 추가
#define TELEPORT_PAGE_MAX 4

class CUITeleportPrimium : public CUITeleport
{
protected:
	CTextureData		*m_ptdPopupTexture;					// Texture of popup
	// Control
	CUIButton			m_btnPage[TELEPORT_PAGE_MAX];		// Page1 button

	UIRectUV			m_rtBackgroundR;					// UV of background
	UIRectUV			m_rtBackgroundPage;
	UIRectUV			m_rtBackgroundPageR;
	UIRectUV			m_rtBackgroundBottom;
	UIRectUV			m_rtBackgroundBottomR;

	UIRectUV			m_rtShopPremLU;						// UV of premium personal shop
	UIRectUV			m_rtShopPremUp;
	UIRectUV			m_rtShopPremRU;
//	UIRectUV			m_rtShopPremL;
//	UIRectUV			m_rtShopPremR;
	UIRectUV			m_rtShopPremLL;
	UIRectUV			m_rtShopPremLo;
	UIRectUV			m_rtShopPremRL;

	UIRectUV			m_rtShopPremBL;
	UIRectUV			m_rtShopPremBR;
	UIRectUV			m_rtShopPremBLL;
	UIRectUV			m_rtShopPremBLo;
	UIRectUV			m_rtShopPremBRL;

public:
	CUITeleportPrimium();
	~CUITeleportPrimium();

	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );	// Create OverRiding
	void	Render();																	// Render OverRiding

	// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );
};

#endif	// UITELEPORT_H_

