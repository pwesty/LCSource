#ifndef __TRADE_ITEM_H__
#define __TRADE_ITEM_H__

struct stEventTrade;
class CTradeItem
{
public:
	static void Destroy();
	static int LoadTradeItemLod(const char* szFileName);

	static std::vector<stEventTrade*>	m_vecTradeItem;
	typedef std::vector<stEventTrade*>::iterator	vecTradeIter;

	static stEventTrade* GetTradeItem(int nIndex) 
	{ 
		if (nIndex < 0 || nIndex >= m_vecTradeItem.size())
			return NULL;
		return m_vecTradeItem[nIndex];
	}

	static int	GetItemCount()	{ return m_vecTradeItem.size();	}

private:
	static stEventTrade* _data;
};
#endif // __TRADE_ITEM_H__