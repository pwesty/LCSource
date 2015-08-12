// ----------------------------------------------------------------------------
//  File : UIMix.h
//  Desc : Created by yjpark
// ----------------------------------------------------------------------------

#ifndef	UIMIX_H_
#define	UIMIX_H_
#ifdef	PRAGMA_ONCE
#pragma once
#endif

// Define max char and line of strings
#define	MAX_MIX_STRING				4


// Define text position
#define	MIX_TITLE_TEXT_OFFSETX		25
#define	MIX_TITLE_TEXT_OFFSETY		5
#define	MIX_DESC_TEXT_SX			15
#define	MIX_DESC_TEXT_SY			33


// Define size of refine
#define	MIX_TOP_HEIGHT				26
#define	MIX_BOTTOM_HEIGHT			7
#define	MIX_WIDTH					216
#define	MIX_HEIGHT					190

#define MIX_ITEM_SLOT_SIZE			34
#define MIX_ITEM_SLOT_COUNT			3


// ----------------------------------------------------------------------------
// Name : CUIMix
// Desc :
// ----------------------------------------------------------------------------
class CUIMix : public CUIWindow
{
protected:
    // Controls
    CUIButton			m_btnClose;							// Close button
    CUIButton			m_btnOK;							// OK button
    CUIButton			m_btnCancel;						// Cancel button
    CUIIcon*			m_pIconsSlot[3];					// Slot item button
    BOOL				m_bWaitMixResult;					// If UI wait result from server or not

    // Strings
    int					m_nStringCount;						// Count of string
    CTString			m_strMixDesc[MAX_MIX_STRING];		// Strings of refine description
    int					m_nTextRegionHeight;				// Height of text region

    // Region of each part
    UIRect				m_rcTitle;							// Region of title
    UIRect				m_rcItemSlot[3];						// Region of item slot
    UIRect				m_rcInsertItem;						// Region of inserting item

    // UV of each part
    UIRectUV			m_rtTop;							// UV of top background
    UIRectUV			m_rtMiddle1;						// UV of middle background
    UIRectUV			m_rtMiddle2;						// UV of middle background
    UIRectUV			m_rtBottom;							// UV of bottom background
    UIRectUV			m_rtItemSlot;						// UV of item slot

    int					m_nCurItemSlot;
    int					m_nCurItemCount;

    int					m_nTextItemIndex;					// 조합문서의 정보
    SWORD				m_nTab;
    SWORD				m_nInvenIdx;

protected:
    // Internal functions
    void	AddString(CTString& strDesc);

    // Network message functions ( send )
    void	SendMixReq();

public:
    CUIMix();
    ~CUIMix();

    // Create
    void	Create(CUIWindow* pParentWnd, int nX, int nY, int nWidth, int nHeight);

    // Render
    void	Render();

    // Open & close refine
    ENGINE_API	void	OpenMix(int nItemIndex, SWORD nTab, SWORD inven_idx);
    void	CloseMix();

    // Adjust position
    void	ResetPosition(PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ);
    void	AdjustPosition(PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ);

    // Messages
    WMSG_RESULT	MouseMessage(MSG* pMsg);

    // Network message functions
    void	MixRep(SBYTE sbResult);

    // Command functions
    void	SetMixItem(int nSlotIndex = -1);
    void	ResetMixItem();
    int		NearPosition(int nX, int nY);

    // RenderItemInfo ...
    void	RenderItemInfo();
};


#endif	// UIMIX_H_

