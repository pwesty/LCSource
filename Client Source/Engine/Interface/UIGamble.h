// ----------------------------------------------------------------------------
//  File : UIGamble.h
//  Desc : Created by dongmin
// ----------------------------------------------------------------------------

#ifndef	UIGAMBLE_H_
#define	UIGAMBLE_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif

class CUIIcon;

#include <Engine/Ska/ModelInstance.h>


// Define text position
#define	GAMBLE_TITLE_TEXT_OFFSETX			25
#define	GAMBLE_TITLE_TEXT_OFFSETY			5

// Define size of shop
#define GAMBLE_MAIN_WIDTH					255
#define	GAMBLE_MAIN_HEIGHT					213
#define GAMBLE_TOTAL_MARK					7
#define GAMBLE_TOTAL_NUMBER					10
#define GAMBLE_TOTAL_SLOT					5

// [071122] New_MoonStone :Su-won
#define MAX_REWARD_COUNT					24		//최대 보상품 수

#ifdef MOONSTONE_COUNT_ADD	// [2010/11/23 : Sora] 문스톤 개수 추가
	#define MOONSTONE_COUNT						6		//문스톤의 종류 수
#else
	#define MOONSTONE_COUNT						5		//문스톤의 종류 수
#endif

// ----------------------------------------------------------------------------
// Name : CUIGamble
// Desc :
// ----------------------------------------------------------------------------
class CUIGamble : public CUIWindow
{
protected:
	// Region
	UIRect				m_rcMainTitle;	
	UIRect				m_rcSlot[GAMBLE_TOTAL_SLOT];
	DOUBLE				m_dSlotOld[GAMBLE_TOTAL_SLOT];
	DOUBLE				m_dSlotElapsed[GAMBLE_TOTAL_SLOT];	
	int					m_iSelectedMarker[GAMBLE_TOTAL_SLOT];		// 서버에서 받은 선택된 마커값...
	int					m_iCurMarker[GAMBLE_TOTAL_SLOT];
	int					m_iMarkerScore[GAMBLE_TOTAL_MARK];			// 복권 점수 결정...
	BOOL				m_iStoppedMarker[GAMBLE_TOTAL_MARK];
	BOOL				m_bSelectedFixed;
	int					m_iMaxMarker;
	int					m_iAccumulate;
	CTString			m_strAccumulate;

	// Background
	UIRectUV			m_rtBackTop;
	UIRectUV			m_rtBackMiddle;
	UIRectUV			m_rtBackBottom;	

	UIRectUV			m_rtMark[GAMBLE_TOTAL_MARK];				// 문스톤 표시 마크...
	UIRectUV			m_rtSelect;									// 문스톤 선택된 마크 표시.
	UIRectUV			m_rtSmallNumber[GAMBLE_TOTAL_NUMBER];		// 문스톤 포인트 표시 숫자
	
	// Buttons	
	CUIButton			m_btnStop;
	CUIButton			m_btnCancel;

	// Position of target npc
	//FLOAT				m_fNpcX, m_fNpcZ;
	BOOL				m_bBegin;
	BOOL				m_bStart;
	BOOL				m_bSendedStart;

	int					m_iStopSlot;
	CEntity*			m_pNpcEntity;

	BOOL			m_bIsCashMoonStone;
	BOOL			m_bIsReady;			// Check Ready State
	INDEX			m_selCashItemIdx;

	// Cash MoonStone Structure
	struct CASHITEMS{
		INDEX			cashIdx;		// cash item catalog index
		CTString		cashName;		// cash item name
		INDEX			cashCnt;		// cash item count (exclude set items)
	};
	
	std::vector<CASHITEMS> m_cashItemList;


	//////////////////////////////////////////////////////////////////////////////////////////////
	// [071122: Su-won] NEW_MOONSTONE
	CStaticArray<INDEX>	m_saReward[MOONSTONE_COUNT];
	INDEX*				m_iTexID;					// 문스톤 아이콘 텍스쳐 ID
	UIRectUV*			m_rtNewMark;				// 문스톤 아이콘 텍스쳐 좌표
	
	INDEX				m_iUsedMoonStone;							// 문스톤에 사용된 문스톤 종료

	CUIIcon*			m_pIconsMoonStone[5];								// 룰렛에 사용할 문스톤 버튼
	// [071108: Su-won] NEW_MOONSTONE
	//////////////////////////////////////////////////////////////////////////////////////////////

public:
	CUIGamble();
	~CUIGamble();

	// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );
	ENGINE_API void	OpenGamble( CEntity* pEntity ,BOOL bIsCashMoonStone = FALSE);
	void	ShowGamble();

	void	Start();
	void	Stop();

	// Render
	void	Render();	

	// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );	

	void	ResetGamble();

	// 보상 이펙트 표시하기...
	void	ShowPrizeEffect();
	
	void	SetSelectedMarker( int iSlot, int iCount );
	void	SetAccumulate( int iAccumulate );

	// Check Cash Item
	BOOL	 	CheckCashItem(int itemIdx);
	void		SetSelCashItemIdx(int idx) { m_selCashItemIdx = idx; }
	int			GetSelCashItemIdx() { return m_selCashItemIdx; }

	//////////////////////////////////////////////////////////
	// [071122: Su-won] NEW_MOONSTONE
	void		ReadMoonStoneLOD();
	INDEX		GetUsedMoonStoneIndex();
	void		SetSelectedItem( INDEX	iRewardItem );
	// [071122: Su-won] NEW_MOONSTONE
	//////////////////////////////////////////////////////////

	// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );

	
	std::vector<CASHITEMS>& GetCashItemList() { return m_cashItemList; }

protected:	
	void	DrawNumber( int x, int y, int nNumber );				// 숫자표시
	void	DrawNumber( int x, int y, CTString& strNumber );		// 숫자표시
	void	RenderSlot( int iSlot, int iMarker );
	void	RenderSelectedSlot( );
	void	Init();
};

#endif // UIGAMBLE_H_

