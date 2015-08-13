#ifndef UICASHSHOPEX_H_
#define UICASHSHOPEX_H_
#ifdef PRAGMA_ONCE
	#pragma once
#endif

#ifdef G_JAPAN
	#define MAX_CASHINVEN	1
#else
	#define MAX_CASHINVEN	10
#endif
#define MAX_CASHEQUIP	6

#include <Engine/Entities/CashShopData.h>
#include <Engine/Interface/UIManager.h>
#include <Engine/Interface/UIGroup.h>
#include <Engine/tinyxml/tinyxml.h>
#include <Engine/Interface/UITrackPopup.h>

class CCashTypeNode
{
public:
	enum CTNINFO
	{
		CTN_CATALOG,
		CTN_FIRST,
		CTN_SECOND,
	};

	CCashTypeNode() : m_Parents(NULL) {}
	CCashTypeNode(CCashTypeNode* pNode, CTString strName, CTNINFO _ctnInfo);
	virtual ~CCashTypeNode() {}

	void	Create(CCashTypeNode* pNode, CTString strName, CCashTypeNode::CTNINFO _ctnInfo);
	void	AddNode(CTString strName, CCashTypeNode::CTNINFO _ctnInfo);

	INDEX	GetNodeCount() { return m_SubList.Count(); }
	CCashTypeNode* GetParent(void) { return m_Parents; }
	CTString	GetName(void) { return m_strName; }
	CCashTypeNode& GetSubNode(INDEX iRef) { return m_SubList[iRef]; }

protected:
	CTNINFO		m_CTNINFO;
	CTString	m_strName;
	CCashTypeNode*	m_Parents;
	CStaticStackArray<CCashTypeNode>	m_SubList;
};

class ENGINE_API CUICashShopEX : public CUIWindow
{
public:
	CUICashShopEX(const char* pcXMLPath);
	~CUICashShopEX();
	// 캐시샵 내부에서 사용될 팝업 창 클래스
	class CCashShopPopup : public CUIWindow{
	public:
		enum CSPopupType{
			CSP_Package, // 패키지 정보 
			CSP_Gift, // 선물 보내기
			CSP_Purchase, // 캐시아이템 구매
			CSP_Total,
		};

		CCashShopPopup();
		~CCashShopPopup();

		void Create(CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight); // 생성
		void SetType(CSPopupType Type) { m_Type = Type; } // 표현할 타입을 설정(CSPopupType)

		WMSG_RESULT MouseMessage(MSG *pMsg);
		WMSG_RESULT KeyMessage(MSG *pMsg);
		WMSG_RESULT IMEMessage(MSG *pMsg);
		WMSG_RESULT CharMessage(MSG *pMsg);

		BOOL IsEditBoxFocused() {
			if (m_edCharacterName.IsFocused() || m_edGiftContent.IsFocused())
			{
				return TRUE;
			}
			return FALSE; 
		}
		void KillFocusEditBox() {
			m_edCharacterName.SetFocus(FALSE);
			m_edGiftContent.SetFocus(FALSE);
		}

		void OpenPopup(CSPopupType type); // type에 맞게 팝업 오픈
		void Render();

		void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
		void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
		void	SetPosition(PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ) // 팝업 위치 보정을 위한 스크린 사이즈(스크린 중앙으로 설정)
		{
			m_pixMinI = pixMinI;
			m_pixMinJ = pixMinJ;
			m_pixMaxI = pixMaxI;
			m_pixMaxJ = pixMaxJ;
		}

		void ClearGoodsData() { m_GoodsDatas.Clear(); } // 상품 정보를 클리어
		void AddGoodsData(CCashShopData::CASH_SHOP_DATA* csData) { m_GoodsDatas.Add(csData); } // 상품 정보를 추가
		void SetPackageItems(void); // CSP_Package 타입에서 m_GoodsDatas에 있는 상품정보를 구성
		void SetPackageItems(INDEX iPos); // 5개이상의 상품 목록에서 스크롤 된 위치에 맞게 상품 정보 구성
		
		void PopupClose();

		// Accessor
		CUIEditBox& GetCharacterName() { return m_edCharacterName; }
		CUIMultiEditBox& GetGiftContent() { return m_edGiftContent; }
		
	protected:
		void SendPurchaseReq(void); // CSP_Purchase 타입에서 구매 요청
		void AddFriendsList(void); // 메신저에서 가지는 모든 친구 목록을 구성

		CUIRectSurface m_BgPackagePopup; // CSP_Package 타입의 Surface 정보
		CUIRectSurface m_BgGiftPopup; // CSP_Gift 타입의 Surface 정보
		CUIRectSurface m_BgPurchasePopup; // CSP_Purchase 타입의 Surface 정보

		CUIScrollBar m_PackageScroll; // CSP_Package 타입의 스크롤 UI
		CUIScrollBar m_GiftScroll; // CSP_Gift 타입의 스크롤 UI

		CUIButton m_btnClose; // close 버튼
		CUIButton m_btnConfirm; // 확인, 결제
		CUIButton m_btnCancel; // 취소

		CUIEditBox m_edCharacterName; // 선물 받을 캐릭터 쓰기를 위한 EditBox
		CUIMultiEditBox	m_edGiftContent; // 선물 보낼 메시지 쓰기를 위한 MultiEditBox
		CUIComboBox m_cbFriends; // 친구 목록 표현을 위한 콤보 박스

		CUIIcon* m_pIconPackage; // CSP_Package의 Package 정보를 위한 ButtonEx
		CStaticArray<CUIIcon*> m_PackageItems; // CSP_Package의 아이템정보를 위한 ButtonEx
		CStaticStackArray<CCashShopData::CASH_SHOP_DATA*> m_GoodsDatas; // 팝업에서 사용될 상품 목록
		
		CSPopupType m_Type; // 팝업의 타입

		PIX m_pixMinI;
		PIX m_pixMinJ;
		PIX m_pixMaxI;
		PIX m_pixMaxJ;
	};

	enum CashShopFrame
	{
		CSF_MainTab,
		CSF_Category,
		CSF_View,
		CSF_Recommand,
		CSF_Preview,
		CSF_Inven,
		CSF_Total,
	};

	enum CS_MainTab
	{
		CS_IrisShop, // 아이리스 상점
		CS_PurchaseInfo, // 구매 정보
		CS_GotTheGift, // 받은 선물
		CS_WishPage, // 소망 상자
		CS_Total,
	};

	enum CashShopCategory
	{
		CSC_NONE = -1,
		CSC_HOTandNew,
		CSC_Platinum,
		CSC_SpendGoods,
		CSC_EquipGoods,
		CSC_Avata,
		CSC_ServiceGoods,
		CSC_PackageGoods,
		CSC_Total,
	};

	enum CategorySubType
	{
		CST_First,
		CST_Second,
		CST_Total,
	};

	enum ViewFrameType
	{
		VFT_GoodsList,
		VFT_WishList,
		VFT_PurchaseList,
		VFT_GiftList,
	};

	enum ViewPageType
	{
		VPT_Begin,
		VPT_Front,
		VPT_Back,
		VPT_End,
		VPT_Total,
	};

	enum BtnInvenType
	{
		BIT_Clear,
		BIT_PurchaseItem,
		BIT_CashCharge,
		BIT_Exit,
		BIT_Total,
	};

	enum GoodsFlagInfo
	{
		CFI_New,
		CFI_Popular,
		CFI_Discount,
		CFI_Total,
	};

	enum UVRectPos {
		UV_UL, UV_UM, UV_UR,
		UV_ML, UV_MM, UV_MR,
		UV_LL, UV_LM, UV_LR,
		UV_Total,
	};

	enum CashShopCommand
	{
		CSC_ItemPurchase, // 단독 구입
		CSC_CostumeWear,
		CSC_AddInven,
		CSC_AddWishBox,
		CSC_Cancel,
	};

	typedef struct _GOODSBOX
	{
		CUIRectSurface BoxSurface;
		// Background Image
		CCashShopData::CASH_SHOP_DATA* GoodsData; // 캐쉬 아이템 정보 참조용
		CUIIcon* pIconGoods; // 상품 캐시 아이콘 버튼
		CUIButton btnPkgInfo; // 패키지 정보 버튼
		// 그외의 특이 사항 이미지 추가 예정, (New, 특가 등 성격에 맞는 이미지)
		_GOODSBOX() : GoodsData(NULL) {
			pIconGoods = new CUIIcon;
		}
		~_GOODSBOX() {
			SAFE_DELETE(pIconGoods);
		}

	}GoodsBox;

	// Category 별로 구성
	typedef struct _CS_FirstSubType 
	{
		CStaticStackArray<INDEX> SecondSubTypeIndexList;
		INDEX iFirstSubType;

		void Clear()
		{
			iFirstSubType = -1;
			SecondSubTypeIndexList.Clear();
		}
	}CS_FirstSubType;

	typedef struct _CS_CategoryTypeInfo
	{
		CStaticStackArray<CS_FirstSubType> FirstSubTypeIndexList;
	}CS_CategoryTypeInfo;

	typedef struct _CS_PurchaseGoods 
	{
		BOOL bEnable;
		BOOL bSelected;
		CUIIcon* pIconGoods;
		_CS_PurchaseGoods () : bEnable(FALSE),bSelected(FALSE) {
			pIconGoods = new CUIIcon;
		}

		~_CS_PurchaseGoods() {
			SAFE_DELETE(pIconGoods);
		}

		void Clear()
		{
			bEnable = FALSE;
			bSelected = FALSE;
			pIconGoods->clearIconData();
		}
	}CS_PurchaseGoods;

	typedef struct _CS_GiftData
	{
		CTString strName;
		CTString strMessage;
		ULONG	ulDate;
		ULONG	ulUniIndex;
		CCashShopData::CASH_SHOP_DATA* GoodsData;
		_CS_GiftData() : GoodsData(NULL), ulDate(0), ulUniIndex(0) {}

		void Clear()
		{
			ulDate = 0;
			ulUniIndex = 0;
			GoodsData = NULL;
		}
	}CS_GiftData;

	void Create(CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight);

	void Render();

	WMSG_RESULT MouseMessage(MSG *pMsg);
	WMSG_RESULT KeyMessage(MSG *pMsg);
	WMSG_RESULT IMEMessage(MSG *pMsg);
	WMSG_RESULT CharMessage(MSG *pMsg);

	void MsgBoxCommand(int nCommandCode, BOOL bOK, CTString &strInput ,CTString &strConfirm);
	void MsgBoxCommand(int nCommandCode, BOOL bOK, CTString &strInput);
	void MsgBoxLCommand(int nCommandCode, int nResult);

	void SetCashType(CNetworkMessage *istr); // 서버로 부터 사용할 카테고리 타입을 받는다.(서브 타입 정보들)
	void SetMyCash(SQUAD myCash); // 현재 보유 캐시정보 설정.(invisible 상태에서는 OpenCashShopEX() 호출)
	void SetGoodsList(CNetworkMessage *istr); // 요청한 상품 리스트 정보 갱신
	void SetRecommandList(CNetworkMessage *istr); // 추천 상품 리스트 정보 갱신
	void SetWishList(CNetworkMessage *istr); // 소망 상자 상품 리스트 정보 갱신
	void SetPurchaseItemList(CNetworkMessage *istr); // 구매한 상품 정보 갱신
	void RevWishList(CNetworkMessage* istr, BOOL bCommit); // 소망상자 아이템 정보 갱신
	void RevGiftList(CNetworkMessage* istr); // 선물 목록 갱신
	void RevResultGetGift(void); // 선물 가져오기 결과

	void SendPurchaseReq(); // 구매요청 메시지
	void SendBringReq(); // 구매 물품 가져가기
	void SendGiftReq(void); // 선물 보내기

	void SetCurrentPageType(INDEX iType) { m_CurrentPageType = iType; } // 현재 보여주는 ViewFrame의 검색 타입 설정
	BOOL SetCashIndexToBtn(int nCashIndex,int nUniIndex); // 구매정보에 캐시아이템을 추가
	void SetRClickMenu(INDEX iItem); // 우클릭시 팝업 메뉴 설정
	void SetCashShopLock(BOOL bLock) { m_bCashShopLock = bLock; } // 오픈 가능 유무를 위한 락 설정
	void SetWearingModel(CUIIcon* srcBtn, INDEX iCashIndex = 0); // Preview의 캐릭터 장비를 입힌다.
	void SetTakeOffModel(INDEX iRef); // Preview의 캐릭터 장비를 벗는다.
	void SetPackageWearingModel(CUIIcon* srcBtn, INDEX iCashIndex = 0);
	void SetSuitWearingModel(INDEX iItem, INDEX iCash);

	BOOL GetCashShopLock(void) { return m_bCashShopLock; }
	INDEX GetCurrentPageType(void) { return m_CurrentPageType; } // 현재 뷰 페이지 타입
	ULONG GetMyCash(void) { return (ULONG)m_hasCash; } // 보유 캐시
	UIRectUV* GetUVbtnEXInfo(void) { return m_UVbtnEXInfo; } // 버튼 정보를 표시하기 위한 배경박스

	void OpenCashShopEX(void); // 상점 오픈

	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	void AddBtnItemInfoString(CUIIcon* srcBtn, BOOL bRemain = FALSE); // ButtonEx(캐시아이템 정보)의 정보를 문자열 구성
	void AddPackageItemInfo(CUIIcon* srcBtn); // ButtonEx(패키지 캐시 아이템 정보)의 정보를 문자열 구성

	void CompletePurchase(void); // 구매 완료
	void CompleteBringItems(void); // 아이템 가져오기 완료

	BOOL IsEditBoxFocused() {
		return m_Popup.IsEditBoxFocused();
	}
	void KillFocusEditBox() {
		m_Popup.KillFocusEditBox();
	}

	void ForcedExit(void); // 강제 종료
	void SetLoadDataState(BOOL bState) { m_bLoadingData = bState; } // 서버로 데이터 요청 중
	
	void InitRenderTarget(void); // RenderTarget 초기화
	void DestroyRenderTarget(void); // RenderTarget 삭제

	void Message(int nCommandCode, CTString strTitle, CTString strMessage, DWORD dwStyle); // Error 출력을 위한 메세지 박스
	
	BOOL IsPopupOpened(void) { return m_Popup.IsVisible(); }
	void SetPopupVisible(BOOL bVisible) { m_Popup.SetVisible(bVisible); }

protected:
	// Frame별로 배경이 되는 Surface 초기화
	void BasicBackgroundInit(FLOAT fTexWidth, FLOAT fTexHeight);
	void MainTabFrameInit(FLOAT fTexWidth, FLOAT fTexHeight);
	void CategoryFrameInit(FLOAT fTexWidth, FLOAT fTexHeight);
	void ViewFrameInit(FLOAT fTexWidth, FLOAT fTexHeight);
	void RecommandFrameInit(FLOAT fTexWidth, FLOAT fTexHeight);
	void PreviewFrameInit(FLOAT fTexWidth, FLOAT fTexHeight);
	void InvenFrameInit(FLOAT fTexWidth, FLOAT fTexHeight);

	// MouseMove 일괄 처리
	WMSG_RESULT MouseMoveLoop(MSG *pMsg, CashShopFrame type);

	// 클리어 함수들
	void ClearCashType();
	void ClearGoodsList();
	void ClearRecommandList();
	void ClearWishList();
	void ClearPurchaseItemList();

	// 현재 검색 설정을 Index화 한다.
	INDEX GetSearchType(void);
	// 우클릭시 팝업메뉴에서 선택된 것을 실행
	void RunRClickMenu(INDEX iType);

	// 콤보 박스 설정
	void SetComboBox(CashShopCategory categorytype);
	void SetSubComboBox(CashShopCategory categorytype, INDEX iType, INDEX iSubType);
	// 스크롤 시 받은 선물 셋팅
	void SetRevGiftDatas(INDEX iPos);

	void ArrangePurchaseGetGoods(void); // 옮길 물품 정리
	void MovePurchaseGoods(BOOL bOver = FALSE); // 최대 10개를 한꺼번에 자리 옮김

	// 선물할 아이템 정보를 Popup 클래스에 등록
	void AddGoodsBringData(void);

	void SendWishItems(ULONG ulCTID, BOOL bCommit = TRUE); // 소망상자로 등록 요청
	void SendGetRevGiftItem(INDEX iPos); // 선물 보내기(현재 하나만 보냄)

	void InitCatalogNameNodes(void);

	TiXmlElement* FromXmltoCashData(CCashTypeNode& CashNode, TiXmlElement* pNode, CCashTypeNode::CTNINFO NodeType);

	// Background
	CUIRectSurface m_Background;

	// Frame 영역이 있어야..
	UIRect m_CashShopFrame[CSF_Total];

	// CSF_MainTab
	CUIButton m_BtnMainTab[CS_Total];
	CS_MainTab m_SelMainTab;

		// CS_WishPage 소망 상자
	CStaticArray<GoodsBox> m_GoodsBoxList;	// Max 8(page당 8개 상품 cpp에서 선언하자.
	CStaticArray<CS_PurchaseGoods> m_PurchaseItemList;
	CStaticArray<CUIIcon*> m_PurchaseGetItemList;
	CStaticArray<GoodsBox> m_WishBoxList;
	CStaticStackArray<CS_GiftData> m_RevGiftList;

	// CSF_Category
	CUIButton m_BtnCategory[CSC_Total];
	CUIEditBox m_ebCategoryName; // 카테고리 선택시 카테고리 이름 표시
	CUIComboBox m_cbCategoryType[CST_Total];
	CUIButton m_BtnSearch; // 검색 버튼

	UIRectUV m_UVSelCategory;

	CashShopCategory m_SelCategoryTab;
	CashShopCategory m_OverCategoryTab;

	INDEX m_CurrentPageType; // 현재 보여주고 있는 검색 type

	// CSF_View
	CUIRectSurface m_blueBoxGoodsList;
	CUIRectSurface m_blueBoxPurchase;
	CUIRectSurface m_BlackBoxPage;
	ViewFrameType m_ViewFrameType;
	CUITrackPopup m_tpCommand; // 팝업 메뉴

	CUIRectSurface m_BlackBoxRevGiftList;
	CUIScrollBar m_GiftScroll;

	INDEX m_iCurrentPage;
	INDEX m_iMaxPage;
	CUIEditBox m_ebPage; // 현재 페이지 표시용
	CUIButton m_BtnPage[VPT_Total];

	UIRectUV m_uvFlag[CFI_Total];

	CUIMultiEditBox	m_edGiftContent; // 선물 내용 보기 용
	UIRect m_rtGiftDataList;

	UIRect m_PurchaseItemRect; // 구매한 물품 공간
	UIRect m_PurchaseGetItemRect; // 옮길 물품 공간
	CUIButton m_BtnMove; // 모두 옮기기
	CUIButton m_BtnGet; //  가져오기(받은 선물, 구매정보)
	CUIButton m_BtnGift; // 선물하기
	INDEX m_SelectedCount; // 최대 10개까지 선택 가능
	
	CUIIcon* m_pIconRevGift[5];
	INDEX	m_SelectedGift;

	// CSF_Recommand
	CStaticArray<GoodsBox> m_RecommandBoxList; // Max 5 ( 1~5 Rank Info)
	UIRectUV m_uvRecommandImage; // 뭘까?

	// CSF_Preview
	UIRect m_rtPreview; // Preview 공간
	CUIButton m_btnRestore; // 회복 버튼
	CUIIcon* m_pIconEquip[MAX_CASHEQUIP];
	INDEX m_iEquipCashIndex[MAX_CASHEQUIP+1]; // +1은 방패를 착용했는지를 위해서 가진다.
	INDEX m_iEquipDropCashIndex;
	CRenderTexture* m_ModelRenderTarget; // 3D 모델이 실제 랜더링 될 공간

	// CSF_Inven
	CUIIcon* m_pIconInven[10];		// Japan은 MAX_CASHINVEN이 1이다. 버튼 생성은 해야해서 10으로 변경.
	CUIButton m_btnInven[BIT_Total];
	SQUAD	m_GoodsPrice;
	SQUAD	m_hasCash;
	UIRect	m_btnExInvenRect;

	// Common UV and button
	UIRectUV m_UVbtnEXLock;
	CUIButton m_btnClose;
	UIRectUV m_UVbtnEXInfo[UV_Total];

	// Popup instance
	CCashShopPopup m_Popup;

	CS_CategoryTypeInfo m_CashSubTypeInfo[CSC_Total]; // Cashshop에서 사용할 type 정보이다.

	int m_SelectedGoods;
	int m_SelectedGetPurchase;
	int m_SelectedIven;
	int m_SelectedRecommand;
	int m_SelectedEquip;
	int m_SelectedWishList;

	INDEX m_iRClickMenuIndex;

	BOOL m_bCashShopLock;

	CUIRectSurface m_LoadingBackBox;
	CUIAnim m_LoadingAnim;
	BOOL m_bLoadingData;
	BOOL m_bPackageInfo;
	BOOL m_bShowPurchaseOver;

	CCashTypeNode	m_CatalogNamesNodes[CSC_Total];

	bool m_bPreviewDrag;
	bool m_bMoveStartInven;
public:
	bool IsVFT_GoodList() 
	{ 
		if (m_ViewFrameType == VFT_GoodsList)
			return true;
		return false;
	}
};

#endif