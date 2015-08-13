// ----------------------------------------------------------------------------
//  File : UIPetFree.h
//  Desc : Created by Eons
// ----------------------------------------------------------------------------

#ifndef	UI_PETFREE_H_
#define	UI_PETFREE_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif

// Define text position
#define	UI_PETFREE_TITLE_TEXT_OFFSETX		25
#define	UI_PETFREE_TITLE_TEXT_OFFSETY		5

// Define size of refine
#define	UI_PETFREE_TOP_HEIGHT				26
#define	UI_PETFREE_BOTTOM_HEIGHT			7
#define	UI_PETFREE_WIDTH					216
#define	UI_PETFREE_HEIGHT					400


// ----------------------------------------------------------------------------
// Name : CUIChangeWeapon
// Desc :
// ----------------------------------------------------------------------------
class CUIPetFree : public CUIWindow
{
protected:
	CItems* 			m_pSelItem;

	// Controls
	CUIButton			m_btnClose;							// Close button
	CUIButton			m_btnOK;							// OK button
	CUIButton			m_btnCancel;						// Cancel button
	CUIIcon*			m_pIconSlotItem;						// Slot item button

	// Strings
	int					m_nStringCount;						// Count of string	
	CTString			m_strPetFreeMoney;				// String of refine money
	SQUAD				m_llPetFreeMoney;				// ChangeWeapon money
	SLONG				m_slLevel;
	SLONG				m_slPetIndex;					// 
	int					m_nMoneyPosY;						// Position y of refine money text
	int					m_nTextRegionHeight;				// Height of text region
	int					m_nPettype;
	float				m_fNpcPosX, m_fNpcPosZ;
	int					m_nNpcIndex;
	bool				m_bPremiumChar;
	// Region of each part
	UIRect				m_rcTitle;							// Region of title
	UIRect				m_rcItemSlot;						// Region of item slot
	UIRect				m_rcInsertItem;						// Region of inserting item

	// UV of each part
	UIRectUV			m_rtTop;							// UV of top background
	UIRectUV			m_rtMiddle1;						// UV of middle background
	UIRectUV			m_rtMiddle2;						// UV of middle background
	UIRectUV			m_rtMiddle3;						// UV of middle background
	UIRectUV			m_rtBottom;							// UV of bottom background
	UIRectUV			m_rtItemSlot;						// UV of item slot

	UIRectUV			m_rtInputBoxL;						// UV of left region of input box
	UIRectUV			m_rtInputBoxM;						// UV of middle region of input box
	UIRectUV			m_rtInputBoxR;						// UV of right region of input box

protected:
	// Internal functions
	void	AddString( CTString &strDesc );

	// Command functions
	void	SetFreePetItem();
	SQUAD	CalculateNeedNas( int iPetLevel );

public:
	CUIPetFree();
	~CUIPetFree();

	// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );

	// Render
	void	Render();

	// Open & close refine
	ENGINE_API	void	OpenPetFree(float fX, float fZ, int nIdex);
	void	ClosePetFree();

	// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );

	// Send Network
	void	SendPetFreeReq();

	// recevie
	void	PetFreeError( SLONG PetIndex, SBYTE sbResult );

	// 프리미엄 캐릭터
	void	SetPremiumBenefit(bool bUse) { m_bPremiumChar = bUse; }
};


#endif	// UI_PETFREE_H_

