#ifndef UI_RANKING_SYSTEM_H_
#define UI_RANKING_SYSTEM_H_

#include "RankingList.h"

class CUIRankingSystem : public CUIWindow
{
public:
	CUIRankingSystem();
	~CUIRankingSystem();

	typedef enum eRankingMouseEventType
	{
		RANKING_MOUSE_NONE = 0,
		RANKING_MOUSE_OVER,
	} MOUSE_EVENT_TYPE;

	typedef enum eRankingListType
	{
		RANKING_LIST_TOP = 0,
		RANKING_LIST_BOTTOM,
	} RANK_LIST_TYPE;
	////// interface ////////////////////////////////////

	//// Get ////
	CRankingList*	GetRankingList()	{ return &m_oRankingList; }
	// 직업 타입을 스트링으로 얻기
	CTString		GetJobStringByType(UBYTE ubClass);
	/// 랭킹 타입을 스트링으로 얻기
	CTString		GetRankStringByType(ERANKINGEX_SUB_TYPE eSubType);
	// 0 : 레벨마스터랭킹, 1: 직업랭킹
	UCHAR			GetRankingNetworkType();							
	UCHAR			GetRankingNetworkSubType();							// 0:(없음) , 1:(0 : 타이탄직업, 1 : 나이트직업 ...)

	//// Update ////
	void		UpdateBottomRanking();
	void		UpdateTopRanking();

	//// Open , close ////
	void		OpenUI();
	void		OpenUI(ERANKINGEX_SUB_TYPE eSubType);
	void		CloseUI();
	void		ToggleVisible();
	//// command ////
	void		SearchName(CTString cstrName);
	void		ChangeViewType(ERANKINGEX_SUB_TYPE eSubType);
	void		ToggleReset();
	void		SetRankingListUnSelect( RANK_LIST_TYPE ListType, COLOR col );
	void		SetRankingListSelect( RANK_LIST_TYPE ListType, COLOR col );
	void		SetRankingListMouseEvent( RANK_LIST_TYPE ListType, CUIListItem* pListItem, COLOR col, MOUSE_EVENT_TYPE EventType );
	void		SetRankingListStringCol( CUIListItem* pListItem, COLOR col );
	int			GetListCurSel( RANK_LIST_TYPE ListType );
	//// reset	////
	void		ClearRankingList();

	//////////////////////////////////////////////////////
	void	initialize();

	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	BOOL	CloseWindowByEsc()			{ CloseUI();	return TRUE; }

	virtual BOOL	IsEditBoxFocused()			
	{ 
		if ( m_pEbSearch )
		{
			return m_pEbSearch->IsFocused();
		}

		return FALSE;
	}
	// Msg Proc
	WMSG_RESULT OnLButtonDown(UINT16 x, UINT16 y);
	WMSG_RESULT OnLButtonUp(UINT16 x, UINT16 y);
	WMSG_RESULT OnMouseMove(UINT16 x, UINT16 y, MSG* pMsg);
	WMSG_RESULT	OnKeyMessage(MSG* pMsg);
	WMSG_RESULT OnCharMessage(MSG* pMsg);
	WMSG_RESULT	OnIMEMessage(MSG* pMsg);
	////// network ////////////////////
	void	RecvRankingList(CNetworkMessage& nmMessage);
	BOOL	CheckRecvRankingListAndReq();
private:
	void			SetTitle(CTString cstrTitle);
	void			ChangeComboType();
	void			SetListString(SRankingPointData* pRankData, CUIListItem* pListItem, COLOR color);
	void			SetTopListErrorMsgString(CTString strString = "", BOOL bHide = TRUE);
	void			SetBottomListErrorMsgString(CTString strString = "", BOOL bHide = TRUE);
	
	bool			m_bTitleClick;
	UIRect			m_rcOriginPos;
	bool			m_bDrag;
	int				m_nOriX, m_nOriY;
	int				m_nLastCurSel;
	INT				m_nRecvRankTotalCount;			// 전송받을 총 데이터의 갯수 (패킷 받을 총 랭킹 건 수)
	INT				m_nRecvRankCount;				// 전송받은 데이터의 갯수 (패킷 받은 랭킹 건 수)
	BOOL			m_bRecvRank;					// 전송받는 중
	ERANKINGEX_SUB_TYPE		m_eSubType;				// 보여지고있는 분류
	CRankingList			m_oRankingList;
	CTString		m_cstrSearch;					// 검색한 아이디
	CTString		m_cstrTopRankingMsg;			// 윗부분 랭킹리스트 박스 내 표시되는 메세지
	CTString		m_cstrBottomRankingMsg;			// 아랫부분 랭킹리스트 박스 내 표시되는 메세지
protected:
	CUIButton*		m_pbtnClose;
	CUIButton*		m_pbtnSearch;
	CUIButton*		m_pbtnReset;
	CUIList*		m_pTopRankList;
	CUIList*		m_pBottomRankList;
	CUIScrollBar*	m_pTopRankListScroll;
	CUIScrollBar*	m_pBottomRankListScroll;
	CUIImage*		m_pProgressBox;
	CUIProgressBar* m_pProgressBar;
	CUIComboBox*	m_pLankType;
	CUIEditBox*		m_pEbSearch;
	CUIText*		m_pTitle;
	CUIText*		m_pMoveTitle;
	CUIText*		m_pTopRankMsg;
	CUIText*		m_pBottomRankMsg;
};
#endif // UI_RANKING_SYSTEM_H_