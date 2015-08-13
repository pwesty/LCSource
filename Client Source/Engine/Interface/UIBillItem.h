
// ----------------------------------------------------------------------------
//  File : UIBILL_ITEM.h
//  Desc : Created by 이기환
// ----------------------------------------------------------------------------

#ifndef	UIBILL_ITEM_H_
#define	UIBILL_ITEM_H_
#ifdef	PRAGMA_ONCE 
	#pragma once
#endif

#define	BILL_ITEM_WIDTH					216
#define	BILL_ITEM_HEIGHT				380

class CBillItemData
{
public:
	
	int		m_nIndex;			// item DB index
	int		m_nItemIndex;
	int		m_nItemPlus;	
	int		m_nItemFlag;
	int		m_nItemNumber;
	

	CBillItemData( int nIndex = -1, int nItemIndex = -1, int nItemPlus = -1, int nItemFlag = -1, int nItemNumber = -1 )
	{
		m_nIndex		= nIndex;
		m_nItemIndex	= nItemIndex;
		m_nItemPlus		= nItemPlus;
		m_nItemFlag		= nItemFlag;
		m_nItemNumber	= nItemNumber;
		
	}
	void SetBillItem( int nIndex = -1, int nItemIndex = -1, int nItemPlus = -1, int nItemFlag = -1, int nItemNumber = -1 )
	{
		m_nIndex		= nIndex;
		m_nItemIndex	= nItemIndex;
		m_nItemPlus		= nItemPlus;
		m_nItemFlag		= nItemFlag;
		m_nItemNumber	= nItemNumber;
	}
};




class CUIBillItem : public CUIWindow
{
protected:
	
// Controls...
	
	// Button
	CUIButton				m_btnClose;							// 닫기 버튼 
	CUIButton				m_btnOK;							// 가공 버튼 
	CUIButton				m_btnCancel;						// 취소 버튼 
	
	// Item buttons
	std::vector<CUIButtonEx> m_btnItems;						// 가공 아이템

	// spin buttons
	std::vector<CUISpinButton> m_sbtnItemNum;					// wooss 050830 the number of items 

	std::vector<int>		m_vSelected;						// selected item order					

	// Etc ...
	CUIScrollBar			m_sbBillItem;						// 가공 아이템 창 스크롤 바
	CUIListBox				m_lbBillItemDesc;					// 필요 조건 설명 리스트 박스
	
	
// Region of each part
	UIRect					m_rcTitle;							// Region of title bar
	UIRect					m_rcIcons;							// Region of icons
	UIRect					m_rcTab;							// Region of tab
	UIRect					m_rcItem;
	UIRect					m_rcDesc;

// UV of each part
	UIRectUV				m_rtBackgroundTop;					// UV of background UP
	UIRectUV				m_rtBackgroundBtm;					// UV of background DOWN
	UIRectUV				m_rtSelOutline;						// UV of outline of selected button

// Network ...
	BOOL					m_bWaitResult;				// Wait Message
	
// Tool Tip	
	UIRectUV				m_rtInfoUL;								// UV of upper left region of information
	UIRectUV				m_rtInfoUM;								// UV of upper middle region of information
	UIRectUV				m_rtInfoUR;								// UV of upper right region of information
	UIRectUV				m_rtInfoML;								// UV of middle left region of information
	UIRectUV				m_rtInfoMM;								// UV of middle middle region of information
	UIRectUV				m_rtInfoMR;								// UV of middle right region of information
	UIRectUV				m_rtInfoLL;								// UV of lower left region of information
	UIRectUV				m_rtInfoLM;								// UV of lower middle region of information
	UIRectUV				m_rtInfoLR;								// UV of lower right region of information

	BOOL					m_bShowItemInfo;

	UIRect					m_rcItemInfo;
	int						m_nCurInfoLines;

	CTString				m_strItemInfo[MAX_ITEMINFO_LINE];		// Item information string
	COLOR					m_colItemInfo[MAX_ITEMINFO_LINE];		// Color of item information string
	BOOL					m_bDetailItemInfo;	
	int						m_nSelectItem;
	int						m_nSelectItemSum;						// the number of select items		

public:
	CUIBillItem();
	~CUIBillItem();

	
// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );
	
// Render
	void	Render();

// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	
// Open & close BillItem
	void	Clear();
	ENGINE_API	void	OpenBillItem( );
	void				CloseBillItem();
	
	void AddBillItemData( CBillItemData BillItemData );

// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );
	
// Network message functions ( receive )
	void	BillItemRep( SBYTE sbResult );
	
// Send ...
	void	SendBillItemReq();
// Get item info
	std::vector<CUIButtonEx> GetBtnItemInfo() {return m_btnItems;}
	std::vector<int> GetSelectItemInfo() {return m_vSelected;}

// etc ...
	void	SelectItem( int nIndex = -1 );
	void	AddString( CTString& strText, COLOR colText = 0xffffffff );

// Tool Tip
	void	AddItemInfoString( CTString &strItemInfo, COLOR colItemInfo = 0xF2F2F2FF );
	BOOL	GetItemInfo( int nItemIndex, int &nInfoWidth, int &nInfoHeight );
	void	ShowItemInfo( BOOL bShowInfo, int nItemIndex, BOOL bRenew = FALSE );

	void	RenderItemInfo();
};


#endif	// UIBILL_ITEM_H_

