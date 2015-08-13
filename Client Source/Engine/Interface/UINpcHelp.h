#ifndef	UINpcHelp_H_
#define	UINpcHelp_H_
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

#define UI_NPCHELP_WIDTH	236
#define UI_NPCHELP_HEIGHT	354
#define READ_MAX_CHAR 32

// ----------------------------------------------------------------------------
// Name : CUINpcHelp
// Desc :
// ----------------------------------------------------------------------------
class CUINpcHelp : public CUIWindow
{


protected:

	enum eRenderType
	{
		NPC_LIST	= 0, // 목록 표시
		NPC_EXPLAN	= 1, // 설명 표시
	};


		// Controls
	CUIButton			m_btnClose;							// Close button
	CUIButton			m_btnNpcHelp;						// Help button
	CUIButton			m_btnList;							// List button
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

	UIRectUV			m_rtSplitterS;
	UIRectUV			m_rtSplitterL;

	CUIListBox			m_lbNpcList;
	CUIListBox			m_lbNpcExplan;
	CUIListBox			m_lbNpcDesc;
	std::vector<int>	m_vectorNpclist;

	int					m_nCurRow;
	int					m_nCurZone;
	int					m_nRenType;
	int					m_nStringCount;
	CTString			m_strDesc[READ_MAX_CHAR];



public:
	CUINpcHelp();
	~CUINpcHelp();

	// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );
	void	OpenNpcHelp();
	void	CloseNpcHelp();
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	Render();
	void	SetViewList();
	void	SetViewContent( int iIndex );
	void	RefreshNpcList();
	void	MultiLineString(CTString &strExplan);	

	// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );	
};

#endif	// UIBUTTON_H_