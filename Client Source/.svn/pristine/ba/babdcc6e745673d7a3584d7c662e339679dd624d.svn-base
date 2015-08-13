// ----------------------------------------------------------------------------
//  File : UIMix.h
//  Desc : Created by yjpark
// ----------------------------------------------------------------------------

#ifndef	UIGWMIX_H_
#define	UIGWMIX_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif

// Define max char and line of strings
#define	MAX_GW_MIX_STRING				4
#define GW_MIX_ITEM_SLOT_COUNT			3

// ----------------------------------------------------------------------------
// Name : CUIGWMix
// Desc :
// ----------------------------------------------------------------------------
class CUIGWMix : public CUIWindow
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
	CTString			m_strMixDesc[MAX_GW_MIX_STRING];	// Strings of refine description
	int					m_nTextRegionHeight;				// Height of text region

	// Region of each part
	UIRect				m_rcTitle;							// Region of title
	UIRect				m_rcItemSlot[3];					// Region of item slot
	UIRect				m_rcInsertItem;						// Region of inserting item

	// UV of each part
	UIRectUV			m_rtTop;							// UV of top background
	UIRectUV			m_rtMiddle1;						// UV of middle background
	UIRectUV			m_rtMiddle2;						// UV of middle background
	UIRectUV			m_rtBottom;							// UV of bottom background
	UIRectUV			m_rtItemSlot;						// UV of item slot
	
	int					m_nCurItemSlot;
	int					m_nCurItemCount;
	BOOL				m_bDetailItemInfo;

protected:
	// Internal functions
	void	AddString( CTString &strDesc );

	// Network message functions ( send )
	void	SendMixReq();

public:
	CUIGWMix();
	~CUIGWMix();

	// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );

	// Render
	void	Render();

	// Open & close refine
	ENGINE_API	void	OpenGWMix();
	void	CloseGWMix();

	// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );

	// Network message functions
	void	MixRep( SBYTE sbResult );

	// Command functions
	void	SetMixItem ( int nSlotIndex = -1 );
	void	ResetMixItem ();
	int		NearPosition ( int nX, int nY );

	CUIIcon* GetItemSlot ( int nIndex )
	{
		if ( nIndex < 0 && nIndex >= GW_MIX_ITEM_SLOT_COUNT )
		{
			return NULL;
		}

		return m_pIconsSlot[nIndex];
	}
};


#endif	// UIGWMIX_H_

