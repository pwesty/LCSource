/**
\par  create date : 10/10/29
\par  author      : sam
\par  desc        : 랭킹 분류
*/

#ifndef UIRANKINGCLASSIFY_H
#define UIRANKINGCLASSIFY_H

#ifdef	PRAGMA_ONCE
	#pragma once
#endif

#include <Engine/Interface/UIWindow.h>
#include <Engine/Interface/UIButton.h>
#include <Engine/Interface/UIListBox.h>
#include <Engine/GlobalDefinition.h>

class CUIRankingClassify : public CUIWindow
{
public:
	enum { 
			RANK_BEST       = 0,
			RANK_TITAN		, 
		   RANK_KNIGHT		, 
		   RANK_HEALER		, 
		   RANK_MAGE		, 
		   RANK_ROGUE		,
		   RANK_SORCERER	, 
		   RANK_NIGHTSHADOW , 		   		
#ifdef CHAR_EX_ROGUE
		   RANK_EX_ROGUE	,	// [2012/08/27 : Sora] EX로그 추가
#endif
#ifdef CHAR_EX_MAGE
		   RANK_EX_MAGE		,	//2013/01/08 jeil EX메이지 추가 
#endif
		   RANK_GUARDIAN 
	};
	enum enSERVER_RANK_ORDER
		{
			SERVER_RANK_BEST       = 100,
			SERVER_RANK_TITAN	=0	, 
			SERVER_RANK_KNIGHT		, 
			SERVER_RANK_HEALER		, 
			SERVER_RANK_MAGE		, 
			SERVER_RANK_ROGUE		,
			SERVER_RANK_SORCERER	, 
			SERVER_RANK_NIGHTSHADOW , 		   		
#ifdef CHAR_EX_ROGUE
			SERVER_RANK_EX_ROGUE	,	// [2012/08/27 : Sora] EX로그 추가
#endif
#ifdef CHAR_EX_MAGE
			SERVER_RANK_EX_MAGE		,	//2013/01/08 jeil EX메이지 추가 
#endif
			SERVER_RANK_GUARDIAN = 101 
		};
	

	CUIRankingClassify ();
	virtual ~CUIRankingClassify ();
	
protected:			
	CTextureData		*m_ptdImageToolTip;	
	
	int					m_bVisible;							// Right View 
	int					m_bVisibleImg;						// Right View Image
	int					m_bSlideImgX;						// for sliding image
	
	// Controls
	CUIButton			m_btnClose;							// Close button
	
	// List 
	CUIListBox			m_lbListContent;					// Listbox of list content 	
	
	// Region of each part
	UIRect				m_rcTitle;							// Region of title		
	
	// UV of each part
	UIRectUV			m_rtTitleUL;						// UV of upper left background
	UIRectUV			m_rtTitleUM;						// UV of upper middle background
	UIRectUV			m_rtTitleUR;						// UV of upper right background

	UIRectUV			m_rtBackUL;							// UV of upper left background
	UIRectUV			m_rtBackUM;							// UV of upper middle background
	UIRectUV			m_rtBackUR;							// UV of upper right background
	UIRectUV			m_rtBackML;							// UV of middle left background
	UIRectUV			m_rtBackMM;							// UV of middle middle background
	UIRectUV			m_rtBackMR;							// UV of middle right background
	UIRectUV			m_rtBackLL;							// UV of lower left background
	UIRectUV			m_rtBackLM;							// UV of lower middle background
	UIRectUV			m_rtBackLR;							// UV of lower right background
	

	UIRect				m_rcToolTip;
	UIRectUV			m_rtToolTipUL;								
	UIRectUV			m_rtToolTipUM;								
	UIRectUV			m_rtToolTipUR;								
	UIRectUV			m_rtToolTipML;								
	UIRectUV			m_rtToolTipMM;								
	UIRectUV			m_rtToolTipMR;								
	UIRectUV			m_rtToolTipLL;								
	UIRectUV			m_rtToolTipLM;								
	UIRectUV			m_rtToolTipLR;								
	
	bool				m_bShowToolTip;						// 툴팁을 보여준다.
	
	struct stToolTip
	{
		CTString ctszrName;
		COLOR    color;
	};
	std::vector<stToolTip> m_vecCtstringToolTip;

	CTString			m_cszCurToolTip;
	COLOR				m_colorCurToolTip;
	
public:	

	// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );

	// Render
	void	Render();
	void	RenderList();
	void    RenderToolTip ();


	// Toggle visible of web board
	void	ToggleVisible();	

	// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	// Messages
	//WMSG_RESULT	KeyMessage( MSG *pMsg );
	WMSG_RESULT	MouseMessage( MSG *pMsg );		
	
	// 선택한 랭킹 리스트 보이기
	void    ShowSelectList ( int iIndex );
	
	// 분류 창 보이기 
	void	Open();
	// 분류 창 닫기
	void	Close();
	
	// 리스트에 목록 추가 하기
	void	AddListContents ( int iStringCode, CTString cstrName, COLOR cColor = 0xFFAD60FF );
	// 툴팁 추가
	void    AddTooltip ( int iStringCode, CTString cstrName, COLOR cColor = 0xFFAD60FF );

	// 툴팁 보여주기 
	void	ShowToolTip ( bool bShow, int iIndex = -1, int x = 0, int y = 0 );

	const int		RankingServerTypeToUIIndex( int nType );            // 서버타입->uiindex서버에서 직업이 0~7 이고 최고레벨이 100, 후견인이 101 이다. 그래서 따로 변환 함수 둔다. 
	const int		RankingClientTypeToServer( int nType );             // 클라이어트 타입-> 서버
	const int		RankingUIIndexToServer ( int nType );					// uiindex->서버	
	const int		RankingClietTypeToUIIndex ( int nType );					// 클라이언트->uiindex	
	const int		RankingServerTypeToClient( int nType );             // 서버 타입->클라이어트 	

	// esc closing support [5/30/2011 rumist]
	BOOL			CloseWindowByEsc()				{ Close();	return TRUE;				}
};

#endif