#ifndef INCL_SHOPDATA_H
#define INCL_SHOPDATA_H
#ifdef PRAGMA_ONCE
#pragma once
#endif
#include <Engine/Entities/Entity.h>
#include <vector>

// Shop의 정보를 관리하는 클래스.
class ENGINE_API CShopData
{
private:
	typedef struct _tagShopData
	{
		int		iIndex;
		char	szShopName[50];
		int		iSellRate;
		int		iBuyRate;		
	}TShopData, *PTShopData;
public:
	CShopData();
	~CShopData();

	inline	int		GetIndex()			const	{	return	m_ShopData.iIndex;		};
	inline	int		GetSellRate()		const	{	return	m_ShopData.iSellRate;	};
	inline	int		GetBuyRate()		const	{	return	m_ShopData.iBuyRate;	};

	// 쓸모없는 부분으로 MobName이 출력될것임.
	inline	const char*	GetShopName()	const	{	return	m_ShopData.szShopName;	};

	int					m_iNumOfItem;			// 판매하는 아이템의 갯수.
	std::vector<int>	m_vectorSellItems;
private:
	TShopData			m_ShopData;				// Shop에 대한 정보를 갖고 있는 구조체.
public:
	void		SetData(int iKeeperIdx, const char* pszShopName, int iSellRate, int iBuyRate);
	static int	LoadShopDataFromFile(CStaticArray<CShopData> &apShopData, const char* FileName);
};
#endif