//============================================================================================================
// File : UICollectBox.h
// Date : 2006-06-27(¿ÀÈÄ 4:05:29), By eons
//============================================================================================================

#ifndef UICOLLECTBOX_H_
#define UICOLLECTBOX_H_
#ifdef PRAGMA_ONCE
	#pragma once
#endif

#define UICOLLECTBOX_WIDTH		184
#define UICOLLECTBOX_HEIGHT		194

class CUICollectBox : public CUIWindow
{
protected:
	CUIButton m_btnClose;
	CUIIcon* m_pIconInsectItem[16]; // bugs event slot item 

	//Su-won |------------------------->
	CUIListBox		m_lbItemInfo;		//Su-won
	int				m_iDetailInfo;		//Su-won
	BOOL			m_bDetailInfo;		//Su-won
	CTextureData	*m_ptdInfoTexture;	//Su-won
	UIRectUV			m_rtInfoL;								// UV of left region of information
	UIRectUV			m_rtInfoM;								// UV of middle region of information
	UIRectUV			m_rtInfoR;								// UV of right region of information
	UIRectUV			m_rtInfoUL;								// UV of upper left region of information
	UIRectUV			m_rtInfoUM;								// UV of upper middle region of information
	UIRectUV			m_rtInfoUR;								// UV of upper right region of information
	UIRectUV			m_rtInfoML;								// UV of middle left region of information
	UIRectUV			m_rtInfoMM;								// UV of middle middle region of information
	UIRectUV			m_rtInfoMR;								// UV of middle right region of information
	UIRectUV			m_rtInfoLL;								// UV of lower left region of information
	UIRectUV			m_rtInfoLM;								// UV of lower middle region of information
	UIRectUV			m_rtInfoLR;								// UV of lower right region of information
	//Su-won <-------------------------|
	
	UIRect	m_rcTitle;
	UIRect  m_rcbtnItems;
	
	UIRectUV m_rtTitleL;
	UIRectUV m_rtTitleM;
	UIRectUV m_rtTitleR;
	UIRectUV m_rtItemSlotL;
	UIRectUV m_rtItemSlotM;
	UIRectUV m_rtItemSlotR;
	UIRectUV m_rtPointDescL;
	UIRectUV m_rtPointDescM;
	UIRectUV m_rtPointDescR;
	UIRectUV m_rtBottomL;
	UIRectUV m_rtBottomM;
	UIRectUV m_rtBottomR;

	UIRectUV m_rtSelectOutline;

	int		 m_nSelectItem;
	int		 m_nDropPosition;
	int		 m_nTab;
	int		 m_nInvenIdx;

	CTString m_strTitle;
	CTString m_strPoint;

protected:
	void	SetBtnItem( int num, int nIndex );
	void	DropInsectItem( CTString Name, int nPosition );
public:
	CUICollectBox();
	~CUICollectBox();

	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );

	void	Init( void );
	void	OpenCollectBox( int nTab, int inven_idx );
	void	Render();
	//void	RenderInfo();			//Su-won

	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );

	// Command functions
	void	MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput );

	void	UpDateItem( void );
	CTString GetInsectName( int n );

public :

protected :

};

#endif