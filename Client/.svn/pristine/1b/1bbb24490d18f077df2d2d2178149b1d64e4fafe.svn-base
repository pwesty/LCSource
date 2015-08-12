
#ifndef		AUCTION_SYSTEM_H_
#define		AUCTION_SYSTEM_H_

class CNetworkMessage;
struct TradeAgentItem;

namespace RequestClient {
	struct TradeAgentSystemListBase;
}

namespace ResponseClient {
	struct TradeAgentSystemRegList;
}

class Auction
{
public:
	Auction();
	~Auction();

	void	reset();

	void	ReceiveTradeAgentMessage(CNetworkMessage* istr);
	void	updateNas();	// 나스가 변하였음.

	UINT	getServerTime()		{ return m_nServerTime; }

	//-----------------------------------------------------------------
	// Search
	void	clearSearchList();
	int		getSearchCount()	{ return m_nSearchCount; }
	TradeAgentItem* getSearchItem( int idx );
	RequestClient::TradeAgentSystemListBase*	getSearchPacket(int idx);

	void	SendSearchReq();
	void	SendBuyReq(UINT trade_index);
	short	getCurPage()		{ return m_sCurPage; }
	//UINT8	hasNextPage()		{ return m_ucNextPage; }
	void	RequestNextPage(bool bNext);

protected:
	void	RecvSearchList(CNetworkMessage *istr);

	//-----------------------------------------------------------------
	// regist
public:
	void	clearRegList();
	int		getRegistCount()	{ return m_nRegCount; }
	TradeAgentItem* getRegItem( int idx );
	void	SendRegistListReq();
	void	SendRegistItemReq( SWORD iTab, SWORD inven_idx,
		int iIndex, int nCount, __int64 nTotalNas );
	void	SendRegistCancelReq( UINT index );
	bool	get1stRecvRegList()		{ return m_b1stRegList; }
	__int64 getDeposit()			{ return m_nDeposit; }
	float	getFee()				{ return m_fFee; }
	bool	getRegUpdate()			{ return m_bRegUpdate; }
	void	setRegUpdate(bool bVal)		{ m_bRegUpdate = bVal; }
	void	setNetWorkLock(bool bLock)	{ m_bNetWorkLock = bLock; }
	bool	getNetWorkLock()			{ return m_bNetWorkLock; }

	void	SetPremiumBenefit(bool bUse);
	bool	GetPremiumBenefit() { return m_bPremiumChar; }
protected:
	void	RecvRegistList(CNetworkMessage *istr);
	
	//-----------------------------------------------------------------
	// Favorite
public:	
	void	clearFavoriteList();
	int		getFavoriteCount()	{ return m_nFavoriteCount; }
	TradeAgentItem* getFavoriteItem( int idx );
	void	SendFavoriteListReq();
	void	SendFavoriteRegReq(UINT trade_index);
	void	SendFavoriteCancelReq(UINT trade_index);
	bool	get1stRecvFavoriteList()	{ return m_b1stFavoriteList; }
	bool	findFavItem(int idx);

protected:
	void	RecvFavoriteList(CNetworkMessage* istr);

	//-----------------------------------------------------------------

private:

	bool		m_b1stRegList;
	bool		m_b1stFavoriteList;
	UINT		m_nServerTime;

	// Search
	RequestClient::TradeAgentSystemListBase*	m_pSearchLast;
	short		m_sCurPage;
	UINT8		m_ucNextPage;
	int			m_nSearchCount;
	TradeAgentItem*	m_pSearchList;

	// Regist
	int			m_nRegCount;
	TradeAgentItem*	m_pRegList;
	__int64		m_nDeposit;
	float		m_fFee;
	bool		m_bRegUpdate;

	int			m_nFavoriteCount;
	TradeAgentItem*	m_pFavoriteList;

	// PremiumChar
	bool		m_bPremiumChar;

	// NetWorkLock
	bool		m_bNetWorkLock; // 아이템 구매시 리스트가 갱신될때까지 구매할 수 없도록 하기 위해서 락기능 추가
};


#endif		// AUCTION_SYSTEM_H_