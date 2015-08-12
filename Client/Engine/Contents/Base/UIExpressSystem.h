//------------------------------------------------------------------
//
// LCE System
// 우편을 대체하는 서버로부터 지급 받는 모든 아이템이 이쪽으로 저장된다.
#ifndef UI_LCE_SYSTEM_H_
#define UI_LCE_SYSTEM_H_
#ifdef	PRAGMA_ONCE
#pragma once
#endif

#define LCE_UI_INFO_START_X		0
#define	LCE_UI_INFO_START_Y		0
#define	LCE_UI_INFO_WIDTH		750
#define	LCE_UI_INFO_HEIGHT		491
#define MAX_ITEM_LIST			10

enum eMsgType
{
	MSG_TYPE_RECV = 0,
	MSG_TYPE_RECVALL = 1,
	MSG_TYPE_DELETE = 2,
	MSG_TYPE_DELETEALL = 3
};

class ExpressSystem;
class CUIExpressItemList;

class ENGINE_API CUIExpressSystem : public CUIWindow
{
public:
	CUIExpressSystem(void);
	virtual ~CUIExpressSystem(void);

	//------------------------Interface--------------------------------
	// 익스프레스 아이템 데이타와 연결
	bool		SetLinkData();
	
	void		ClearItemList();
	void		ClearExpress();
	
	// 아이템 리스트 업데이트
	void		UpdateExpressItemList();

	void		SetItemCount(UBYTE nCount)				{ m_nItemCount = nCount; }
	
	void		OpenExpressSystem();
	void		CloseExpressSystem();
	//------------------------------------------------------------------
	// overriding func. (virtual func.)
	void		Create(CUIWindow* pParentWnd, int nX, int nY, int nWidth, int nHeight);

	void		Render();

	void		ResetPosition(PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ);
	void		AdjustPosition(PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ);

	// Message pump.
	WMSG_RESULT MouseMessage(MSG* pMsg);
	BOOL		CloseWindowByEsc();
	// Message Handler.
	void		MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput );
	
protected:
	// UI 위치 셋팅
	int		_GetBtnMiddleSize(CTString strText, int nCurrentSize);
	// UI 버튼 셋팅
	void	_SetBtnRect(int _nMiddleWidth);
	void	_SetPage();

	//-----------------------------------------------------
	// 익스프레스 아이템 리스트
	bool	_SetItemList();
	bool	_SetItemListString(int nIndex, bool bItem);

	CTString _GetSenderType( short SendType );
	
	// 익스프레스 리스트 랜더
	void	_ExpressRender();

	//-----------------------------------------------------
	// 에러 체크
	void	_FailReset();
	// 메시지 박스 체크
	void	_CheckExistMsgBox();
	// 메시지 박스 생성
	void	_CreateMsgBox(eMsgType msgType);

private:
	UIRect						m_rcTitle;
	UIRect						m_rcArea;
	UIRect						m_rcBottom;
	UIRect						m_rcLeft, m_rcMiddel, m_rcRight;
	UIRectUV3					m_rtIdleBtn, m_rtClickBtn, m_rtDisableBtn;

	CUIRectSurface				m_rtsFrame;			// Base
	CUIRectSurface				m_rtsSelectFrame;	// Select Bar
	
	CUIButton					m_btnRecv;			// 받기 버튼
	CUIButton					m_btnAllRecv;		// 모두 받기 버튼
	CUIButton					m_btnDelete;		// 받기 버튼
	CUIButton					m_btnAllDelete;		// 모두 받기 버튼
	CUIButton					m_btnClose;			// 닫기 버튼
	CUIButton					m_btnNextPage;		// 다음 페이지
	CUIButton					m_btnPrevPage;		// 이전 페이지

	CTextureData*				m_ptdLCETexture;
	ExpressSystem*				m_pExPressData;		// 익스프레스 데이타 연결
	bool						m_bItemList;		// 아이템 리스트 추가 성공 실패	
	bool						m_bPrevPage;		// 이전 페이지가 있다 없다
			
	UBYTE						m_nItemCount;		// 아이템 갯수
	int							m_SelectItem_index; // 선택된 아이템 인덱스
	CTString					m_strCurPage;		// 현재 페이지

	CUIExpressItemList*			m_pExpressItemList[MAX_ITEM_LIST];

	// UI 리스트 컬럼 메뉴
	CTString					m_strTitle;
	CTString					m_strCulumName;
	CTString					m_strCulumQuantity;
	CTString					m_strCulumAmount;
	CTString					m_strCulumSender;
	CTString					m_strCulumType;
	CTString					m_strCulumTime;
};
#	endif	// UI_LCE_SYSTEM_H_
