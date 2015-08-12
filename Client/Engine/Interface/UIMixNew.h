// ----------------------------------------------------------------------------
//  File : UIMixNew.h
//  Desc : Created by wooss
// ----------------------------------------------------------------------------

#ifndef	UIMIXNEW_H_
#define	UIMIXNEW_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif

// Define max char and line of strings
#define	MAX_MIXNEW_STRING				13


// Define text position
#define	MIXNEW_TITLE_TEXT_OFFSETX		25
#define	MIXNEW_TITLE_TEXT_OFFSETY		5
#define	MIXNEW_DESC_TEXT_SX			15
#define	MIXNEW_DESC_TEXT_SY			85


// Define size of refine
#define	MIXNEW_TOP_HEIGHT				26
#define	MIXNEW_BOTTOM_HEIGHT			7
#define	MIXNEW_WIDTH					216
#define	MIXNEW_HEIGHT					490

#define MIXNEW_ITEM_SLOT_SIZE			34
#define MIXNEW_ITEM_SLOT_COUNT			2


// ----------------------------------------------------------------------------
// Name : CUIMixNew
// Desc :
// ----------------------------------------------------------------------------
class CUIMixNew : public CUIWindow
{
protected:
	// Controls
	CUIButton			m_btnClose;							// Close button
	CUIButton			m_btnOK;							// OK button
	CUIButton			m_btnCancel;						// Cancel button
	CUIIcon*			m_pIconsSlot[MIXNEW_ITEM_SLOT_COUNT];					// Slot item button
	BOOL				m_bWaitMixResult;					// If UI wait result from server or not

	// Strings
	int					m_nStringCount;						// Count of string
	CTString			m_strMixDesc[MAX_MIXNEW_STRING];		// Strings of refine description
	int					m_nTextRegionHeight;				// Height of text region

	// Region of each part
	UIRect				m_rcTitle;								// Region of title
	UIRect				m_rcItemSlot[MIXNEW_ITEM_SLOT_COUNT];	// Region of item slot
	UIRect				m_rcItemSlot_UN;						// Region of item slot
	UIRect				m_rcInsertItem;							// Region of inserting item

	// UV of each part
	UIRectUV			m_rtTop;							// UV of top background
	UIRectUV			m_rtMiddle1;						// UV of middle background
	UIRectUV			m_rtMiddle2;						// UV of middle background
	UIRectUV			m_rtBottom;							// UV of bottom background
	UIRectUV			m_rtItemSlot;						// UV of item slot
	
	int					m_nCurItemSlot;
	int					m_nCurItemCount;

	int					m_nTextItemIndex;					// 조합문서의 정보
	int					m_nTextRow;
	int					m_nTextCol;

	CTString 				m_strItemSlot[MIXNEW_ITEM_SLOT_COUNT];

	BOOL					m_bIsMix;
	BOOL					m_bIsMixMasterStone;

	// Internal functions
	void	AddString( CTString &strDesc );

	// Network message functions ( send )
	void	SendMixNewReq();
	void	SendMixNewMasterStroneReq();
	bool	GetThaiItemJob(CItemData* pSource, CItemData* pDest);

public:
	CUIMixNew();
	~CUIMixNew();

	// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );

	// Render
	void	Render();
	void	RenderMasterStone();

	// Open & close refine
	void	OpenMixNew(BOOL bMix);
	void	OpenMixNewMasterStone();		// 마스터 스톤용 UI
	void	CloseMixNew();

	// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );

	// Network message functions
	void	MixNewRep( SBYTE sbResult );
	void	MixNewMasterStoneRep( int err_type, int success_type );

	// Command functions
	void	SetMixItem ( int nSlotIndex = -1 );
	void	ResetMixItem ();
	void	ResetMixItemAll();
	int		NearPosition ( int nX, int nY );
	void	SetMixEnable(BOOL bMix) { m_bIsMix = bMix; }
	BOOL	GetMixEnable() { return m_bIsMix; }

private:
	bool CheckMasterStone(CItems* pTargetItem, int nMasterStoneIndex);
	void UpdateExplication(int nPlus = -1);
};


#endif	// UIMIX_H_

