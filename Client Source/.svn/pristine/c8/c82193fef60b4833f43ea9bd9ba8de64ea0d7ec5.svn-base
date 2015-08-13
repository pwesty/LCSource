#ifndef	UINpcScroll_H_
#define	UINpcScroll_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif

// #include <vector>
// #include <Engine/Interface/UIDrawFigure.h>
// #include <Engine/Interface/UIButton.h>
// #include <Engine/Interface/UIListBox.h>
// #include <Engine/Interface/UIListBoxEx.h>
// #include <Engine/Interface/UISlideBar.h>
// #include <Engine/Interface/UIScrollBar.h>
// #include <Engine/LocalDefine.h>
// #include <Engine/Entities/NpcHelp.h>
// #include <Engine/Entities/MobData.h>


#define UI_NPCSCROLL_WIDTH	236
#define UI_NPCSCROLL_HEIGHT	354
#define READ_MAX_CHAR 32

// ----------------------------------------------------------------------------
// Name : CUINpcScroll.h
// Desc : Displays a list of NPCs 
// ----------------------------------------------------------------------------
class CUINpcScroll : public CUIWindow
{

protected:

	enum eRenderType
	{
		NPC_LIST	= 0, // 목록 표시
		NPC_DESC	= 1, // 설명 표시
		WARP_YESNO	= 2, // 이동 선택 표시 
	};


		// Controls
	CUIButton			m_btnClose;							// Close button
	CUIButton			m_btnList;							// List button 목록 버튼 
	CUIButton			m_btnWarp;							// 포탈 이동 버튼 
	CUIButton			m_btnYes;
	CUIButton			m_btnNo;
	CUIScrollBar		m_sbTopScrollBar;

		// Region of each part
	UIRect				m_rcTitle;							// Region of title	
	UIRect				m_rcListTitle;						// Region of list title
	UIRect				m_rcListContent;					// Region of list content
	UIRect				m_rcReadSubject;					// Region of read subject
	UIRect				m_rcReadContent;					// Region of read content
		// UV of each part
	CUIDrawBox			m_bxTitle;
	CUIDrawBox			m_bxBackup;
	CUIDrawBox			m_bxBackDown;

	CUIDrawBox			m_bxBackup2;
	CUIDrawBox			m_bxBackDown2;

	CUIDrawBox			m_bxBackup3;
	CUIDrawBox			m_bxBackDown3;

	UIRectUV			m_rtSplitterS;
	UIRectUV			m_rtSplitterL;

	CUIListBox			m_lbNpcList;
	CUIListBox			m_lbNpcDescription;
	CUIListBox			m_lbNpcDialog;
	CUIListBox			m_lbNpcWarpDescription;	
	std::vector<int>	m_vectorNpclist;
	std::vector<CMobData>	m_vectorMobList;


	int					m_nCurRow;
	int					m_nCurZone;
	int					m_nRenType;
	int					m_nStringCount;
	CTString			m_strDesc[READ_MAX_CHAR];
	int					m_nCurSel;
	int					m_nCurNpcIndex;
	int					m_nCurMobIndex;
	

	float				fX, fZ; 
	// check open flag. [9/21/2009 rumist]
	BOOL				m_bIsOpen;
	//BOOL				m_IsMobShown;
	CStaticArray<int> cArrItemIdex; 

public:
	CUINpcScroll();
	~CUINpcScroll();

	std::vector<CMobData>* GetMobList() { return &m_vectorMobList; }
	
	// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );
	void	OpenNPCScroll();
	void	CloseNpcScroll();
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	ResetSavePosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	Render();
	void	SetViewList();
	void	SetViewContent( int iIndex);
	void    SetWarpContent();
	void	RefreshNpcList();
	void	MultiLineString(CTString & strExplantion);	
	COLOR	GetNameColor(int nIndex);
	void	ReceiveNPCData(CNetworkMessage* istr);
	void	ReceiveMobLocation(CNetworkMessage* istr);
	void	OpenNPCScrollInfo(SLONG slIndex);
	BOOL	IsNPC(int nIndex);
	// check & toggling func. [9/21/2009 rumist]
	BOOL	IsOpened() const;
	void	ToggleNPCScroll();
	void	UseNPCScroll();
	BOOL	IsInPeaceArea();
	BOOL	IsOpenDetailedMap(int nZone, int nNpcIndex);

	
	//BOOL	IsMobShown()	{ return m_IsMobShown; }
//	int		GetCurNpcIndex(int nIndex);
	
	// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );
};

#endif	