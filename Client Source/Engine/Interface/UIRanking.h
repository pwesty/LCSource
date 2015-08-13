// ----------------------------------------------------------------------------
//  File : UIRanking.h
//  Desc : Created by Dongmin
// ----------------------------------------------------------------------------

#ifndef	UIRANKING_H_
#define	UIRANKING_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif

// #include <Engine/Interface/UIWindow.h>
// #include <Engine/Interface/UIButton.h>
// #include <Engine/Interface/UIListBox.h>
// #include <Engine/Interface/UIEditBox.h>
// #include <vector>

// Define text position
#define	RANKING_TITLE_TEXT_OFFSETX		25
#define	RANKING_TITLE_TEXT_OFFSETY		5

// Define size of Ranking
#define	RANKING_WIDTH					311
#define	RANKING_HEIGHT					280//260

// ----------------------------------------------------------------------------
// Name : CUIRanking
// Desc :
// ----------------------------------------------------------------------------
class CUIRanking : public CUIWindow
{
protected:
	// Ranking State
	enum eRankingState
	{
		RANKING_LIST,		// 랭킹 리스트.
		RANKING_PRIZE,		// 랭킹에 따른 보상
	};

	// internal structure
	struct sUserRanking
	{
		sUserRanking()
		{
			bSuccess		= FALSE;
			lPoint			= 0;
			llTime			= 0;
		}

		~sUserRanking()
		{			
		}
		
		CTString		strName;			// 멤버명?		
		BOOL			bSuccess;			// 성공 여부.
		LONG			lPoint;				// 획득 포인트.
		__int64			llTime;				// 소요 시간.
	};

	struct sUserPrize
	{
		sUserPrize()
		{
			strName			= "";	
			iItemIndex		= -1;			// 아이템 인덱스
			iNumOfItem		= -1;			// 아이템 갯수.
			bReward			= FALSE;
		}
		
		CTString		strName;			// 이름
		INDEX			iItemIndex;			// 아이템 인덱스
		INDEX			iNumOfItem;			// 아이템 갯수.		
		BOOL			bReward;			// 보상물품 가능
	};

	std::vector<sUserRanking>	m_vectorRankingList;		// 단원 목록
	std::vector<sUserPrize>		m_vectorPrizeList;			// 보상 목록

	// Controls
	CUIButton				m_btnClose;						// Close button	

	// Region of each part
	UIRect					m_rcTitle;						// Region of title bar	

	// UV of each part
	UIRectUV				m_rtBackground;					// UV of background		
	UIRectUV				m_rtBackTop;					// UV of top background
	UIRectUV				m_rtBackMiddle1;				// UV of middle background
	UIRectUV				m_rtBackMiddle2;				// UV of middle background
	UIRectUV				m_rtBackManagerTop;				//
	UIRectUV				m_rtBackManagerMiddle;			//
	UIRectUV				m_rtBackManagerBottom;			//
	UIRectUV				m_rtBackBottom;					// UV of bottom background

	UIRectUV				m_rtTab;						// UV of section tab
	
	CUIListBox				m_lbRankingList;				// List box
	CUIListBox				m_lbChoiceList;					// List box
	CUIListBox				m_lbPrizeList;					// List Box

	eRankingState			m_eRankingState;				// Ranking State
	BOOL					m_bSatisfyPrize;

protected:
	// Command
	void	PressOKBtn();	

public:
	CUIRanking();
	~CUIRanking();

	// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );

	// Render
	void	Render();

	// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	// Open guild	
	void	OpenRankingList( );
	void	OpenRankingPrize( );
	void	ResetRanking();
	void	ClearRankingList();
	void	ClearPrizeList();
	
	// 보상 목록 관리.
	void	AddToPrizeList( const CTString& strName, INDEX iItemIndex, INDEX iItemCount, BOOL bReward );	

	// 멤버 목록 관리
	void	AddToRankingList( const CTString& strName, BOOL bSuccess, LONG lPoint, __int64 llTime );		// 멤버 목록에 추가	

	// 멤버 목록 갱신
	// NOTE : 인터페이스를 열때는 반드시 갱신되어야 하는 부분이므로 TRUE로 설정하고,
	// NOTE : 보이는 상태일때만 갱신하고자 한다면 FALSE로 설정할것.
	void	RefreshRankingList( BOOL bInit = FALSE );
	void	RefreshPrizeList( BOOL bInit = FALSE );	

	// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );	

	// Set focus
	void	SetFocus( BOOL bVisible );	
	// esc closing support [5/30/2011 rumist]
	BOOL	CloseWindowByEsc()				{ ResetRanking();			return TRUE;		}
};

class CUICubeRank : public CUIWindow
{
protected:

	struct sCubeRank
	{
		sCubeRank()
		{
			Init();
		}
		
		~sCubeRank()
		{

		}
		
		void Init()
		{
			nRank = 0;
			strGuildName = "";
			strChaName = "";
			lPoint = 0;
		}
		BYTE nRank;
		CTString strGuildName;
		CTString strChaName;
		LONG lPoint;
	};

	LONG m_lMypoint;	
	sCubeRank m_stCubeRank[2][5];
	CUIDrawBox	m_bxBackground[4];
	CUIButton		m_btnOK;

	BOOL		m_bGuild;
	// Region of each part
	UIRect					m_rcTitle;						// Region of title bar	



public:
	CUICubeRank();
	~CUICubeRank();
	// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );
	
	// Render
	void	Render();
	
	// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	WMSG_RESULT MouseMessage( MSG *pMsg );

	void	CubeRankInit();
	void	CubeRankClose();
	void	CreateCubeState(CNetworkMessage *istr, BOOL bGuild);
};

#endif	// UIRANKING_H_