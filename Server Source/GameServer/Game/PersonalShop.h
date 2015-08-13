#ifndef __PERSONAL_SHOP_H__
#define __PERSONAL_SHOP_H__

// SHOP CONF
#define PS_MAX_SHOPNAME				100		// 상점 이름 최대 길이 100 바이트
#define PS_MAX_NORMAL_ITEM			10		// 일반 아이템 거래 최대 수
#define PS_MAX_PACKAGE_ITEM			5		// 패키지 판매 거래 최대 수
//#define PS_NEED_MONEY_PREMIUM		5000	// 프리미엄 상점 개설 필요 나스
#if defined (LC_TWN) || defined (LC_JPN)
#define PS_TEX_PREMIUM				1000	// 세율: 프리미엄: 10%
#else
#define PS_TEX_PREMIUM				200		// 세율: 프리미엄: 2%
#endif
#define PS_TEX_NORMAL				0		// 세율: 일반: 0%

// PERSONAL SHOP TYPE
#define PST_NOSHOP			0
#define PST_SELL			(1 << 0)		// 판매
#define PST_BUY				(1 << 1)		// 구매
#define PST_PREMIUM			(1 << 2)		// 프리미엄
#define PST_PACKAGE			(1 << 3)		// 패키지
#define PST_SOLDOUT			(1 << 4)		// 판매 종료

class CPersonalShop
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

	CPersonalShop(char type, const char* name);

	bool AddItem(bool bNormal, int itemindex, LONGLONG count, LONGLONG price);

	int FindItem(bool bNormal, int itemindex);
	void RemoveNormalItem(int itemindex, LONGLONG count);
	void RemovePackage();

	char GetType() { return m_type; }
	const char* GetName() { return (const char*)m_name; }
	char GetNormalCount();
	int GetNextNormalItem(int pos);
	int GetNormalItemIndex(int pos) { return (pos < 0 || pos >= PS_MAX_NORMAL_ITEM) ? -1 : m_normalitemindex[pos]; }
	LONGLONG GetNormalItemCount(int pos) { return (pos < 0 || pos >= PS_MAX_NORMAL_ITEM) ? -1 : m_normalCount[pos]; }
	LONGLONG GetNormalItemPrice(int pos) { return (pos < 0 || pos >= PS_MAX_NORMAL_ITEM) ? -1 : m_normalPrice[pos]; }
	LONGLONG GetPackagePrice() { return m_packagePrice; }
	char GetPackageCount();
	int GetNextPackageItem(int pos);
	int GetPackageItemIndex(int pos) { return (pos < 0 || pos >= PS_MAX_PACKAGE_ITEM) ? -1 : m_packageitemindex[pos]; }
	LONGLONG GetPackageItemCount(int pos) { return (pos < 0 || pos >= PS_MAX_PACKAGE_ITEM) ? 0 : m_packageCount[pos]; }
};




// 대리 상인
#ifdef ALTERNATE_MERCHANT
class CAlternateMerchant
{
private:	
	// 상점 타입
	char			m_cShopType;
	// 상점 이름
	CLCString		m_strShopName;
	// 주인 
	CPC*			m_pShopOwner;


	// 일반 아이템 인덱스 arr
	int				m_nNormalItemIndex[PS_MAX_NORMAL_ITEM];
	// 일반 아이템 카운트 arr
	LONGLONG		m_lNormalItemCount[PS_MAX_NORMAL_ITEM];
	// 일반 아이템 가격 arr
	LONGLONG		m_lNormalItemPrice[PS_MAX_NORMAL_ITEM];


	// 패키지 아이템 인덱스 arr
	int				m_nPackageItemIndex[PS_MAX_PACKAGE_ITEM];
	// 패키지 아이템 카운트 arr
	LONGLONG		m_lPackageItemCount[PS_MAX_PACKAGE_ITEM];
	// 패키지 아이템 가격
	LONGLONG		m_lPackagePrice;

	//물건을 팔고 가지고 있는 나스 
	LONGLONG		m_lNasCount;
public:
	// 리스트에서 인덱스
	int				m_nlistIdx;

	CItem*			m_NormalItems[PS_MAX_PACKAGE_ITEM];
	CItem*			m_PackItems[PS_MAX_PACKAGE_ITEM];

public:
	// 생성자
	CAlternateMerchant( void );
	CAlternateMerchant( char cShopType, const char* strShopName );

	// 아이템 추가
	/*
	* FunName		: AddItem
	* Description	: 개인 상점 대행 NPC에게
	*
	*
	*/
	bool	AddItem( bool bNormal, int nItemIndex, LONGLONG lItemCount, LONGLONG lItemdPrice );

	// 아이템 검색
	CItem*	FindItem( bool bNormal, int nItemIndex );
	int		FindItemPos( bool bNormal, int nItemIndex );

	// 아이템 삭제
	void	RemoveNormalItem( int nItemIndex, LONGLONG lItemCount );

	// 패키지 아이템 삭제
	void	RemovePackage( void );

	// 상점 타입 반환
	char	GetShopType( void )					{ return m_cShopType; }

	// 상점 이름 반환
	const char* GetShopName( void )				{ return (const char*)m_strShopName; }

	// 전체 판매 중인 일반 아이템 개수
	char	 GetNormalCount( void );

	// 일반 아이템 위치
	int		GetNextNormalItem( int nPos );

	// 현재 클릭된 일반 아이템 인덱스  
	int		GetNormalItemIndex( int nPos )			{ return (nPos < 0 || nPos >= PS_MAX_NORMAL_ITEM) ? -1 : m_nNormalItemIndex[nPos]; }

	// 현재 클릭된 일반 아이템 갯수
	LONGLONG GetNormalItemCount( int nPos )		{ return (nPos < 0 || nPos >= PS_MAX_NORMAL_ITEM) ? -1 : m_lNormalItemCount[nPos]; }

	// 현재 클릭된 일반 아이템 가격
	LONGLONG GetNormalItemPrice( int nPos )		{ return (nPos < 0 || nPos >= PS_MAX_NORMAL_ITEM) ? -1 : m_lNormalItemPrice[nPos]; }

	// 패키지 아이템 가격
	LONGLONG GetPackagePrice( void )			{ return m_lPackagePrice; }

	// 전체 패키지 아이템 갯수
	char	GetPackageCount( void );

	// 패키지 아이템의 다음 아이템 가져 오기
	int		GetNextPackageItem( int nPos );

	// 패키지 아이템 인덱스 가져오기
	int		GetPackageItemIndex( int nPos )			{ return (nPos < 0 || nPos >= PS_MAX_PACKAGE_ITEM) ? -1 : m_nPackageItemIndex[nPos]; }

	// 패키지 아이템 갯수 가져오기
	LONGLONG GetPackageItemCount( int nPos )	{ return (nPos < 0 || nPos >= PS_MAX_PACKAGE_ITEM) ? 0 : m_lPackageItemCount[nPos]; }

	// 나스 추가
	void	AddNas( LONGLONG lCount )			{ 	m_lNasCount = m_lNasCount + lCount; }

	// 나스 반환
	LONGLONG GetNas( void )						{ return m_lNasCount;	};

	// 소유자 반환
	CPC*	GetOwnerPC( void )					{ return m_pShopOwner;	};

	// 아이템 사용시 소유자 설정
	void	SetOwnerPC( CPC* pOwner )			{ m_pShopOwner = pOwner;	};

	// 대리 상인 초기화
	void	InitAlternateMerchant( void );

	// 대리 상점 나타나게 함
	void	AppearAlternateMerchant( bool bIncludeOwner );

	// 대리 상점 사라지게 함
	void	DisappearAlternateMerchant( void );
};


class CAlternateMerchantList
{
public: 
	CAlternateMerchant* m_pAlternateMerchantList[MAX_PLAYER_LIST];		//  배열로 사용
	int	m_nMax;
	int m_nCur;

	CAlternateMerchantList()
	{
		int i;
		for ( i = 0; i < MAX_PLAYER_LIST; i++ )
			m_pAlternateMerchantList[i] = NULL;
		m_nMax = 0;
		m_nCur = 0;
	}

	~CAlternateMerchantList()
	{
		m_nMax = m_nCur = 0;
	}

	int AddAlternateMerchant( CAlternateMerchant* pAlternateMerchant )
	{
		int i;
		if (m_nMax >= MAX_PLAYER_LIST )
			return -1;

		if (pAlternateMerchant->m_nlistIdx != -1 && m_pAlternateMerchantList[pAlternateMerchant->m_nlistIdx] == pAlternateMerchant) 
			return pAlternateMerchant->m_nlistIdx;

		for (i = 0; i < m_nMax && m_pAlternateMerchantList[i]; i++)
			;

		m_pAlternateMerchantList[i] = pAlternateMerchant;
//		pAlternateMerchant->m_nlistIdx = i;
		if (i == m_nMax)
			m_nMax++;

		m_nMax++;

		return i;
	}

	void RemoveAlternateMerchant( CAlternateMerchant* pAlternateMerchant )
	{
		int i;
//		DelAttackList(pAlternateMerchant);

		i = pAlternateMerchant->m_nlistIdx;
		if (i < 0 || i >= MAX_PLAYER_LIST)	return ;

		m_pAlternateMerchantList[i] = NULL;
		pAlternateMerchant->m_nlistIdx = -1;
		if (i == m_nMax) m_nMax--;
		m_nCur--;
	}

	CAlternateMerchant* Find(int nAlternateMerchantIndex)
	{
		int i;

		for (i = 0; i < m_nMax; i++)
			if (m_pAlternateMerchantList[i] && m_pAlternateMerchantList[i]->m_nlistIdx == nAlternateMerchantIndex )
				return m_pAlternateMerchantList[i];

		return NULL;
	}

};
#endif // ALTERNATE_MERCHANT



#endif