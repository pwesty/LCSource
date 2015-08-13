#ifndef		UI_AUCTION_NEW_H_
#define		UI_AUCTION_NEW_H_

//---------------------------------------------------------------
//
// 기존의 Auction 시스템 리뉴얼
//

class Auction;
class CUITab;
class CUIImage;
class CUIList;
class CUIIcon;
class CUIText;
class CUIListItem;
class CUIComboBox;

namespace RequestClient {
	struct TradeAgentSystemListBase;
}

#define		DEF_PAGE_PER_COUNT	10

struct TradeAgentItem;

class CUIAuctionNew : public CUIWindow
{
public:
	enum eSEARCH_TYPE
	{
		eSEARCH_TYPE_NAME = 0,
		eSEARCH_TYPE_JOB,
		eSEARCH_TYPE_KIND,
		eSEARCH_TYPE_GRADE,
		eSEARCH_TYPE_LEVEL,
		eSEARCH_TYPE_ENCHANT,
		eSEARCH_TYPE_BATTLE,
		eSEARCH_TYPE_MAGIC,
		eSEARCH_TYPE_SOCKET,
		eSEARCH_TYPE_PRICE,

		eSEARCH_TYPE_MAX
	};

	enum eEDIT_TYPE
	{
		eEDIT_TYPE_NAME = 0,
		eEDIT_TYPE_LV_S,
		eEDIT_TYPE_LV_L,
		eEDIT_TYPE_P1_S,		// 물리 공격력
		eEDIT_TYPE_P1_L,
		eEDIT_TYPE_P2_S,		// 마법 공격력
		eEDIT_TYPE_P2_L,
		eEDIT_TYPE_PRICE_S,		// 가격
		eEDIT_TYPE_PRICE_L,
		
		eEDIT_TYPE_MAX,
	};

	enum eEDIT_ETC_TYPE
	{
		eEDIT_ETC_TYPE_NAME,
		eEDIT_ETC_TYPE_PRICE1,
		eEDIT_ETC_TYPE_PRICE2,

		eEDIT_ETC_TYPE_MAX
	};

	enum eCOMBO_TYPE
	{
		eCOMBO_TYPE_CLASS = 0,
		eCOMBO_TYPE_TYPE,
		eCOMBO_TYPE_GRADE,
//		eCOMBO_TYPE_JEWEL,
		eCOMBO_TYPE_ENC_S,
		eCOMBO_TYPE_ENC_L,
		eCOMBO_TYPE_SOCKET_S,
		eCOMBO_TYPE_SOCKET_L,
		
		eCOMBO_TYPE_MAX
	};

	enum eMSG_BOX_ERROR
	{
		eMSG_BOX_ERROR_REGIST = 100,	// 등록 일반에러
		eMSG_BOX_ERROR_REG_COUNT,		// 등록 수량 에러
		eMSG_BOX_ERROR_REG_EMPTY_CHAOS, // 비어있는 카오스 소켓
	};

	enum eMSG_COMMAND
	{
		eMSG_COMMAND_USE_AUCTION = 0,
		eMSG_COMMAND_BUY_ITEM,
		eMSG_COMMAND_REGIST,
	};

	enum eRESULT_TYPE
	{
		eRESULT_TYPE_SEARCH_ARMOR = 0,
		eRESULT_TYPE_SEARCH_SHIELD,
		eRESULT_TYPE_SEARCH_ETC,
		eRESULT_TYPE_REGIST,
		eRESULT_TYPE_FAVORITE,

		eRESULT_TYPE_MAX
	};

	CUIAuctionNew();
	~CUIAuctionNew();

//	DECLARE_CREATE_UI();

	void	initialize();

	void	reset();

	void	OpenAuction(int nNpcIndex, FLOAT fNpcPosX, FLOAT fNpcPosZ);
	void	UseAuction();
	void	CloseAuction();

	void	changeMainTab();
	void	changeSearchnTab();

	void	updateNas();

	void	ShowError(int nError);

	void	SelListSearch();
	void	SelListRegist();
	void	SelListFavorite();

	void	ShowMsgBuy(__int64 nas);

private:

	void	updateResult(eRESULT_TYPE eType, TradeAgentItem* pItem, CUIListItem* pListItem);
	void	clearResult(eRESULT_TYPE eType, CUIListItem* pListItem);

	void	clearSearch(eRESULT_TYPE eType);

	void	OnTabKeyProc();

	//---------------------------------------------------------------
	// Search
public:
	bool	CanSearch();
	void	updateSearchList();
	void	CanBuyItem();
	void	ChangePage(bool bNext);
	void	setButtonState(INT16 page, UINT8 next);

protected:
	void	initSearch();
	void	initInputTab(int idx, CUIList* pList);
	void	clearInput();
	CTString getGradeString(int idx);
	int		CheckSearchInput(RequestClient::TradeAgentSystemListBase* pData);
	void	ShowErrorSearch(int nError);
	
	//---------------------------------------------------------------
	// Regist
public:
	bool	CanRegister();
	void	CheckRegist();
	void	updateRegistList();
	void	clearRegInput();
	void	updateFeeDeposit(__int64 deposit, float fee);
	void	checkItemCount();
	void	checkPrice();
	void	showRegOK();
	void	showMsgRegCancel();

	void	updatePremiumBenefit();
protected:
	void	initRegist();
	void	tempRegist();	// 임시 등록	

private:
	void	RegisterItem();
	void	RegCancel();
	//---------------------------------------------------------------

	// Favorite
public:
	bool	CanRegFavorite(bool bRegist);
	void	updateFavoriteList();
	void	showMsgFavorite(bool bRegist);

protected:
	void	initFavorite();

	//---------------------------------------------------------------


	//---------------------------------------------------------------
	// Msg
public:
	void	MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput );
	//void	MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput ,CTString &strConfirm);
	void	MsgBoxLCommand(int nCommandCode, int nResult );
	BOOL	CloseWindowByEsc();
	void	CloseAuctionMessageBox();

	WMSG_RESULT OnLButtonDown(UINT16 x, UINT16 y);
	WMSG_RESULT OnLButtonUp(UINT16 x, UINT16 y);
	WMSG_RESULT OnMouseMove(UINT16 x, UINT16 y, MSG* pMsg);

	WMSG_RESULT OnKeyMessage(MSG* pMsg);

	BOOL	IsEditBoxFocused();

	BOOL	IsFarNPC();

	// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

private:
	UINT16	ox, oy;
	bool	m_bTitleClick;
	Auction* m_pAuction;
	
	CTString m_strRegname;
	int		m_nQty;
	__int64 m_nPrice;
	__int64 m_nDepositMoney;
	int		m_nFee;
	int		m_nCurSelItem;
	float	m_fNpcX, m_fNpcZ;								//npc위치(거리 체크용)

	int		m_nBuyIndex;

	// Main
	CUITab*		m_pMainTab;
	CUIText*	m_pTxtTitle;

	// Search
	int			m_nCurSearchSel;
	CUITab*		m_pSearchTab;

	CUIImage*	m_pImgFavoriteItem;

	CUIEditBox*	m_pEdit[2][eEDIT_TYPE_MAX];
	CUIComboBox* m_pCombo[2][eCOMBO_TYPE_MAX];	
	CUIEditBox*	m_pEditEtc[eEDIT_ETC_TYPE_MAX];
	CUIList*	m_pListSearchRes;
	CUIList*	m_pListSearchResEtc;
	CUIText*	m_pTxtPage;
	CUIText*	m_pTxtNas;
	CUIButton*	m_pPrev;
	CUIButton*	m_pNext;
	CUIButton*	m_pBtnRegFav;
	CUIButton*	m_pBtnBuy;

	// Regist
	CItems*		m_pTempRegist;
	CUIIcon*	m_pIconRegist;
	CUIList*	m_pListRegist;
	CUIList*	m_pListRegistResult;
	CUIEditBox*	m_pEditQuantity;
	CUIEditBox* m_pEditPrice;
	CUIText*	m_pTxtFee;
	CUIText*	m_pTxtDeposit;
	CUIText*	m_pTxtRegCount;
	CUIButton*	m_pBtnRegist;
	CUIButton*	m_pBtnRegCancel;
	CUIBase*	m_pDropArea;
	CUIText*	m_pTxtRegNas;
	CUITextBox* m_pTbPremium;

	// Favorite
	CUIList*	m_pListFavorite;
	CUIText*	m_pTxtFavNas;
	CUIText*	m_pTxtFavCount;
	CUIButton*	m_pBtnFavCancel;
	CUIButton*	m_pBtnFavBuy;
};


#endif		// UI_AUCTION_NEW_H_