// ----------------------------------------------------------------------------
//  File : UIAuction.h
//  Desc : Created by dongmin
// ----------------------------------------------------------------------------

#ifndef	UIAUCTION_H_
#define	UIAUCTION_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif

#define AUCTION_WIDTH	644
#define AUCTION_HEIGHT	510
#define AUCTION_SLOT_MAX 10

//탭구분
enum eAuctionTab 
{
	AUCTION_TAB_REFER = 0,		//조회
	AUCTION_TAB_REGISTER,		//등록
	AUCTION_TAB_SETTLEMENT,		//정산
	AUCTION_TAB_TOTAL,
};

//정렬방식
enum eSortType
{
	ITEM_SORT_INDEX = 0,		//인덱스
	ITEM_SORT_COUNT,			//개수
	ITEM_SORT_TOTALNAS,			//나스
	ITEM_SORT_LEVEL,			//레벨
	ITEM_SORT_FINISHDAY,		//종료일
	ITEM_SORT_PASSDAY,			//경과
	ITEM_SORT_TOTAL
};

typedef struct _AuctionItems
{
	int AuctionItemIndex; //거래대행 아이템 번호
	int AuctionItemStatus; //거래대행 아이템 상태(종료일단위, 정산 상태등)		
	SQUAD llNasTotal;			//아이템들 금액
	int	nEtc;				//레벨, 마감일자, 상태를 표시

	_AuctionItems()
	{
		Init();
	}

	void Init()
	{
		AuctionItemIndex = 0; AuctionItemStatus = -1;
		llNasTotal = 0; nEtc = 0;
	}
}AuctionItems;

class CUIAuction : public CUIWindow
{
protected:
	CStaticStackArray<AuctionItems> m_AuctionItemData; // 현재 페이지 내의 거래대행 아이템 정보

	int m_nCurrentSelectedItem;							//현재 선택된 아이템슬롯
	int m_nCurrnetTab;									//현재 탭
	int m_nCurrentPage;									//현재 페이지
	int m_nMaxPage;										//최대 페이지

	int m_nSearchedItemType;							//////////////////////////////////////////////////////////////////////////
	int m_nSearchedItemSubType;							//검색 조건 변경후 페이지 이동시 이전에 검색했던 조건을
	int m_nSearchedClass;								//저장하기 위한 값들
	int m_nCurretnSorting;								//////////////////////////////////////////////////////////////////////////
	int m_nSelectedItemCancel;							// 등록 취소할 아이템 버튼 인덱스

	FLOAT m_fNpcX, m_fNpcZ;								//npc위치(거리 체크용)

	CTString m_strSearched;								//위의 검색 조건 변경과 같은 경우에 사용(검색어 저장)

	BOOL m_bReverse;									//정렬시 오름 내림 차순여부
	BOOL m_bRegister;									// 등록 요청 중인가?
	BOOL m_bSettle;										// 정산할 아이템이 있다.

	SQUAD m_nRegisterItemCount;							//등록할 아이템 개수
	SQUAD m_nRegisterItemPrice;							//등록할 아이템 가격
	SQUAD m_nDepositMoney;								//등록 할 때 보증금

	CTString			m_strTitleName;					//거래대행창 이름

	UIRect				m_rcTitle;						//마우스 드래그로 이동시킬 수 있는 영역
	UIRect				m_rcSelectArea;					//아이템 선택 영역
	UIRect				m_rcRegisterArea;				//등록 아이템 드래그&드롭 영역
	UIRect				m_rcItemArea;					//거래대행 아이템들이 배치되는 영역
	UIRect				m_rcTabArea[AUCTION_TAB_TOTAL];	//상단탭 클릭 영역

	UIRectUV			m_rtSelectedTab;				//선택된 탭(texture)
	UIRectUV			m_rtUnSelectedTab;				//선택되지않은 탭(texture)

	//거래대행에 출력할 이미지들
	//이 순서로 들어감
	//메인 윈도우 0
	//에디트 박스 1
	//도움말 배경 2
	//선택이미지 3
	CUIRectSurface		m_AuctionSurface;				
	CTextureData		*m_ptdButtonTexture;
	
	//버튼들
	CUIButton			m_btnClose;								//닫기
	CUIButton			m_btnSearch;							//검색
	CUIButton			m_btnFirst;								//처음으로
	CUIButton			m_btnPrev;								//1페이지뒤로
	CUIButton			m_btnNext;								//1페이지앞으로
	CUIButton			m_btnLast;								//맨끝으로
	CUIButton			m_btnBuy;								//구매
	CUIButton			m_btnCancelRegister;					//등록취소
	CUIButton			m_btnExecuteSettlement;					//정산
	CUIButton			m_btnSortItem;							//정렬(인덱스)
	CUIButton			m_btnSortCount;							//정렬(개수)
	CUIButton			m_btnSortTotalNas;						//정렬(나스)
	CUIButton			m_btnSortLevel;							//정렬(레벨)
	CUIButton			m_btnSortFinishDay;						//정렬(종료일)
	CUIButton			m_btnSortPassDay;						//정렬(경과)

	//////////////////////////////////////////////////////////////////////////
	CUIComboBox			m_cmbItemType;							//아이템 타입(무기,방어구...)
	CUIComboBox			m_cmbItemSubType;						//아이템 서브타입(아이템 타입에 따라 다름)
	CUIComboBox			m_cmbClass;								//사용가능 클래스

	CUIEditBox			m_ebSearch;								//검색어 입력하는 곳

	CUIButtonEx			m_btnAuctionItem[AUCTION_SLOT_MAX];		//아이템 표시버튼들
	CUIButtonEx			m_btnRegisterItem;						//등록할 아이템

public :
	CUIAuction();
	~CUIAuction();

	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );

	// Render
	void	Render();

	// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );
	WMSG_RESULT	KeyMessage( MSG *pMsg );
	WMSG_RESULT	IMEMessage(MSG *pMsg );
	WMSG_RESULT CharMessage(MSG *pMsg );

	// Command functions
	void	MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput );
	void	MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput ,CTString &strConfirm);
	void	MsgBoxLCommand(int nCommandCode, int nResult );
	BOOL	CloseWindowByEsc()	{ CloseAuction(); return TRUE; }

	BOOL	IsEditBoxFocused() { return m_ebSearch.IsFocused(); }

	// 기타 함수
	void	OpenAuction(int nNpcIndex, FLOAT fNpcPosX, FLOAT fNpcPosZ);
	void	UseAuction();
	void	CloseAuction();

	void	CloseAuctionMessageBox();
	void	InitSlotItem();
	void	InitAll();

	void	SetTab(int nIndex);
	void	SetSlotItem(int nIndex, int nAuctionIndex, CUIButtonEx &btnAuctionItem, SQUAD nNasTotal, int nEtc, int nItemStatus = -1);
	void	SetItemSubType(int nItemType);
	void	SetCurrentPage(int nPageNum){ m_nCurrentPage = nPageNum; }
	void	SetMaxPage(int nMaxPageNum){ m_nMaxPage = nMaxPageNum; }
	void	SetDepositMoney(SQUAD nDepositMoney){ m_nDepositMoney = nDepositMoney; }
	void	SetRegistering(BOOL bEnable) { m_bRegister = bEnable; }
	void	SetSettling(BOOL bEnable) { m_bSettle = bEnable; m_btnExecuteSettlement.SetEnable(bEnable);}

	void	GetRegisterItemData(CTString &strItemName, SQUAD& nItemCount, SQUAD& nItemPrice); // 아이템 등록 정보
	int		GetCurrentPage(){ return m_nCurrentPage; } // 현재 페이지
	SQUAD	GetDepositMoney(){ return m_nDepositMoney; } // 보증금
	int		GetCurrentTab(){ return m_nCurrnetTab;} // 현재 탭 정보
	int		GetCurrentAuctionItemCount(); // 현재 페이지의 상품 개수
	BOOL	GetRegistering() { return m_bRegister; }
	BOOL	GetSettling() { return m_bSettle; }

	BOOL	IsSearchConditionChanged(); // 검색 조건 변경 유무
	void	SortItem(int nSortType, BOOL bReverse = FALSE); // 정렬 (서버 요청)
	void	SearchItem(int nPageNum, int nAlignType);
	void	EnableSortBtn(); // 정렬 활성화
	void	BuyItem(); // 아이템 구입

	void	CancelRegister(); // 등록 취소
	void	RegisterItem(); // 아이템 등록
	BOOL	CanRegister(); // 등록 가능 유무
	
	void	GoPage(int nPageNum); // 페이지 이동
	CTString GetItemFullName(CUIButtonEx &btnItem);

	BOOL	IsFarNPC();

public :
	
protected:
	void initialize();
	void OnUpdate( float fElapsedTime );
#ifdef UI_TEST
// 	CUIComboBox*			m_cmbItemType;							//아이템 타입(무기,방어구...)
// 	CUIComboBox*			m_cmbItemSubType;						//아이템 서브타입(아이템 타입에 따라 다름)
// 	CUIComboBox*			m_cmbClass;								//사용가능 클래스
#endif //UI_TEST

};
#endif // UIAUCTION_H_