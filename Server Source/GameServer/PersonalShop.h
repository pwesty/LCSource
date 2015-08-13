#ifndef __PERSONAL_SHOP_H__
#define __PERSONAL_SHOP_H__

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include "../ShareLib/MemoryPoolBase.h"

// SHOP CONF
#define PS_MAX_NORMAL_ITEM			10		// 일반 아이템 거래 최대 수
#define PS_MAX_PACKAGE_ITEM			5		// 패키지 판매 거래 최대 수
//#define PS_NEED_MONEY_PREMIUM		5000	// 프리미엄 상점 개설 필요 나스
#define PS_TEX_PREMIUM				200		// 세율: 프리미엄: 2%

#define PS_TEX_NORMAL				0		// 세율: 일반: 0%

// PERSONAL SHOP TYPE
#define PST_NOSHOP			0
#define PST_SELL			(1 << 0)		// 판매
#define PST_BUY				(1 << 1)		// 구매
#define PST_PREMIUM			(1 << 2)		// 프리미엄
#define PST_PACKAGE			(1 << 3)		// 패키지
#define PST_SOLDOUT			(1 << 4)		// 판매 종료

class CPersonalShop : public MemoryPoolBaseWithMutex<CPersonalShop>
{
	char			m_type;
	CLCString		m_name;

	int				m_normalitemindex[PS_MAX_NORMAL_ITEM];
	LONGLONG		m_normalCount[PS_MAX_NORMAL_ITEM];
	LONGLONG		m_normalPrice[PS_MAX_NORMAL_ITEM];

	int				m_packageitemindex[PS_MAX_PACKAGE_ITEM];
	LONGLONG		m_packageCount[PS_MAX_PACKAGE_ITEM];
	LONGLONG		m_packagePrice;

public:

	typedef boost::shared_ptr<CPersonalShop> SP;
	typedef boost::weak_ptr<CPersonalShop> WSP;

	CPersonalShop(char type, const char* name);

	bool AddItem(bool bNormal, int itemindex, LONGLONG count, LONGLONG price);

	int FindItem(bool bNormal, int itemindex);
	void RemoveNormalItem(int itemindex, LONGLONG count);
	void RemovePackage();

	char GetType()
	{
		return m_type;
	}
	const char* GetName()
	{
		return (const char*)m_name;
	}
	char GetNormalCount();
	int GetNextNormalItem(int pos);
	int GetNormalItemIndex(int pos)
	{
		return (pos < 0 || pos >= PS_MAX_NORMAL_ITEM) ? -1 : m_normalitemindex[pos];
	}
	LONGLONG GetNormalItemCount(int pos)
	{
		return (pos < 0 || pos >= PS_MAX_NORMAL_ITEM) ? -1 : m_normalCount[pos];
	}
	LONGLONG GetNormalItemPrice(int pos)
	{
		return (pos < 0 || pos >= PS_MAX_NORMAL_ITEM) ? -1 : m_normalPrice[pos];
	}
	LONGLONG GetPackagePrice()
	{
		return m_packagePrice;
	}
	char GetPackageCount();
	int GetNextPackageItem(int pos);
	int GetPackageItemIndex(int pos)
	{
		return (pos < 0 || pos >= PS_MAX_PACKAGE_ITEM) ? -1 : m_packageitemindex[pos];
	}
	LONGLONG GetPackageItemCount(int pos)
	{
		return (pos < 0 || pos >= PS_MAX_PACKAGE_ITEM) ? 0 : m_packageCount[pos];
	}
};

#endif
//
