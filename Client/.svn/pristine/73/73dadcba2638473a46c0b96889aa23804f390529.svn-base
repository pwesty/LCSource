#ifndef _CASHSHOP_DATA_H
#define _CASHSHOP_DATA_H
#ifdef PRAGMA_ONCE
#pragma once
#endif
#include <Engine/Entities/Entity.h>

#define	MAX_CASHSHOP_CLASS					10		// wooss 050902 a kind of cash shop items

enum CASH_SHOP_KIND
{	
	CONSUME_ITEM = 0,
	TIME_LIMIT_ITEM,
	POTION_ITEM,
	AVATA_ITEM,
	WEAPON_ITEM,
	DEFEND_ITEM,
	SET_ITEM,
	PLATINUM_ITEM,
	CASH_SHOP_TOTAL,
};
// 2010.11 new defined Cash Category
// 이것은 전 로컬 표준으로 삼는다.(기획적으로 정의 함)
// 2010.11월 기획적으로 정의한 Category들은 모든 상품을 포함함으로 로컬별로 다르게 가는 일은 거의 없을 것임.(있으면 기획 의도에 어긋남)
// Cash type 정보는 10000 단위로 정의 된다.
// CASH_CATEGORY * 10000 = CC
// 이후 subtype들도 단위 별로 Add되어 사용된다.
// first subtype = 100 단위
// second subtype = 1단위
// 상품 하나의 정보는 
// (CASH_CATEGORY * 10000) + (first subtype * 100) + (second subtype) = goods type flag

enum CASH_CATEGORY
{
	CC_HOTandNew,	// Hot & New 상품
	CC_Platinum,	// 플래티늄 상품
	CC_SpendGoods,	// 소비 상품
	CC_EquipGoods,	// 장비 상품
	CC_AVATA,		// 아바타
	CC_ServiceGoods,// 서비스 상품
	CC_PackageGoods,// 패키지 상품
	CC_Total,
};
// 서버에서 사용하고 있는 Flag 선언.. 클라에서도 동일하게 가져다 쓴다.
#define		CATALOG_FLAG_NEW						(1 << 0)			// 신상품
#define		CATALOG_FLAG_POPULAR					(1 << 1)			// 인기상품
#define		CATALOG_FLAG_DISCOUNT					(1 << 2)			// 특가상품
#define		CATALOG_FLAG_RECOMM_1					(1 << 3)			// 추천아이템
#define		CATALOG_FLAG_RECOMM_2					(1 << 4)			// 추천아이템
#define		CATALOG_FLAG_RECOMM_3					(1 << 5)			// 추천아이템
#define		CATALOG_FLAG_RECOMM_4					(1 << 6)			// 추천아이템
#define		CATALOG_FLAG_RECOMM_5					(1 << 7)			// 추천아이템
#define		CATALOG_FLAG_RESV1						(1 << 8)			// 예약플래그
#define		CATALOG_FLAG_RESV2						(1 << 9)			// 예약플래그
#define		CATALOG_FALG_TO_LIMIT(a, b)				(b = 0 | (a >> 10))	// 카탈로그FLAG 출력
#define		CATALOG_FALG_FROM_LIMIT(a, b)			(a = a | (b << 10))	// 카탈로그FLAG 입력
#define		CATALOG_FLAG_MAX_LIMIT					(2000000)			// 카탈로그플래그LIMIT MAX

// subtype은 조금 자유롭다. subtype정보는 캐시샵 오픈시 서버로 부터 받게 끔 처리 된다.
// 기본적으로 type이라 함은 우리 게임 안의 상품에 대해 정의함으로 거의 변화가 없을 것이며(새로운 상품이 추가될 경우 변할 수 있겠죠),
// subtype도 기본적으로 우리 아이템들에 맞게 모두 정의되어 있음.

// 서버에서의 캐시 데이터 type정의를 그대로 사용한다.
// 2010.11.17
// 다시 type 자유도가 없어짐. 서버에서는 모든 타입을 정의하고, 그중 사용하는 타입을 보내게 되어 있다.
// 모든 type을 클라이언트가 알고 있어야 한다.

// 이하 서버에서 선언된 Type values
// 1) Category
#define CATEGORY_HOTNNEW				10000	// HOT & NEW
#define CATEGORY_PLATINUM				20000	// 플래티늄
#define CATEGORY_DISPOSABLE				30000	// 소비상품
#define CATEGORY_COSTUME				40000	// 장비상품
#define CATEGORY_PET					50000	// 아바타
#define CATEGORY_SERVICE				60000	// 서비스 상품
#define CATEGORY_PACKAGE				70000	// 패키지 상품

// 2) Type 정의(First subtype)
// none type으로 인해 first subtype에서는 그룹 검색이 안된다.
#define TYPE_HOTNNEW_NONE				10100	// HOT & NEW None(none이라는 고유의 상품 타입이 있다. 상위 전체를 의미하지 않는다.)
#define TYPE_HOTNNEW_NEW				10200	// HOT & NEW 신상품
#define TYPE_HOTNNEW_DISCOUNT			10300	// HOT & NEW 특가
#define TYPE_PLATINUM_NONE				20100	// 플래티늄 NONE
#define TYPE_DISPOSABLE_NONE			30100	// 소비 상품 NONE
#define TYPE_DISPOSABLE_CHARACTER_GROW	30200	// 소비 상품 캐릭터 성장
#define TYPE_DISPOSABLE_ABILITY_BUILDUP	30300	// 소비 상품 능력치 강화
#define TYPE_DISPOSABLE_POTION			30400	// 소비 상품 물약
#define TYPE_EQUIP_NONE					40100	// 장비 상품 NONE
#define TYPE_EQUIP_EQUIPMENT			40200	// 장비 상품 장비
#define TYPE_EQUIP_BUILDUP				40300	// 장비 상품 장비 강화
#define TYPE_AVATAR_NONE				50100	// 아바타 NONE
#define TYPE_AVATAR_PET					50200	// 아바타 펫 상품
#define TYPE_AVATAR_MERCENARY			50300	// 아바타 용병
#define TYPE_SERVICE_NONE				60100	// 서비스 상품 NONE
#define TYPE_SERVICE_CONENIENCE			60200	// 서비스 상품 편의 상품
#define TYPE_SERVICE_ETC				60300	// 서비스 상품 기타
#define TYPE_PACKAGE_NONE				70100	// 패키지 상품 NONE

// 3) SubType 정의(second subtype)
#define SUBTYPE_HOTNNEW_NONE_NONE					10101	// HOT & NEW NONE, NONE
#define SUBTYPE_HOTNNEW_NEW_NONE					10201	// HOT & NEW 신상품 NONE
#define SUBTYPE_HOTNNEW_DISCOUNT_NONE				10301	// HOT & NEW 특가 NONE
#define SUBTYPE_PLATINUM_NONE_NONE					20101	// 플래티늄 NONE NONE
#define SUBTYPE_DISPOSABLE_NONE_NONE				30101	// 소비 상품 NONE NONE
#define SUBTYPE_DISPOSABLE_CHARACTER_GROW_NONE		30201 //소비상품_캐릭터성장_NONE
#define SUBTYPE_DISPOSABLE_CHARACTER_GROW_EXP		30202 // 소비상품_캐릭터성장_경험치
#define SUBTYPE_DISPOSABLE_CHARACTER_GROW_SP		30203 // 소비상품_캐릭터성장_숙련도
#define SUBTYPE_DISPOSABLE_CHARACTER_GROW_DROP		30204 // 소비상품_캐릭터성장_드롭율
#define SUBTYPE_DISPOSABLE_CHARACTER_GROW_MIX		30205 // 소비상품_캐릭터성장_혼합
#define SUBTYPE_DISPOSABLE_ABILITY_BUILDUP_NONE		30301 // 소비상품_능력치강화_NONE
#define SUBTYPE_DISPOSABLE_ABILITY_BUILDUP_HPEXT	30302 // 소비상품_능력치강화_HP확장
#define SUBTYPE_DISPOSABLE_ABILITY_BUILDUP_MPEXT	30303 // 소비상품_능력치강화_MP확장 
#define SUBTYPE_DISPOSABLE_ABILITY_BUILDUP_VOL		30304	// VOL = Velocity of Light 소비상품_능력치강화_광속
#define SUBTYPE_DISPOSABLE_POTION_NONE				30401 // 소비상품_물약_NONE
#define SUBTYPE_DISPOSABLE_POTION_HPRECOVER			30402 // 소비상품_물약_HP회복
#define SUBTYPE_DISPOSABLE_POTION_MPRECOVER			30403 // 소비상품_물약_MP회복
#define SUBTYPE_DISPOSABLE_POTION_DUALRECOVER		30404 // 소비상품_물약_듀얼회복 
#define SUBTYPE_EQUIP_NONE_NONE						40101 // 장비상품_NONE_NONE
#define SUBTYPE_EQUIP_EQUIPMENT_NONE				40201 // 장비상품_장비_NONE
#define SUBTYPE_EQUIP_EQUIPMENT_WEAPON				40202 // 장비상품_장비_무기
#define SUBTYPE_EQUIP_EQUIPMENT_ARMOR				40203 // 장비상품_장비_갑옷
#define SUBTYPE_EQUIP_EQUIPMENT_COSTUME				40204 // 장비상품_장비_코스튬
#define SUBTYPE_EQUIP_BUILDUP_NONE					40301 // 장비상품_장비강화_NONE
#define SUBTYPE_EQUIP_BUILDUP_MELDING				40302 // 장비상품_장비강화_결합주문서

// max line of item informaion 
#define	MAX_CASH_ITEMINFO_LINE				20

class ENGINE_API CCashShopData
{
public :

	typedef struct _ItemData
	{
		INDEX		m_itemIndex;
		INDEX		m_flag;
		INDEX		m_plus;
		INDEX		m_option;
		INDEX		m_itemCnt;
	} ITEM_DATA;


	// inner data class
	class CASH_SHOP_DATA
	{
	public :
		INDEX		m_shopItemIndex;
		INDEX		m_type;	// old type
		INDEX		m_cash;
		INDEX		m_mileage;

		INDEX		m_texID;
		INDEX		m_texRow;
		INDEX		m_texCol;
		INDEX		m_flag;
		CHAR		m_enable;

		CTString	m_itemName;
		CTString	m_itemDesc;
		INDEX		m_itemListCnt;
		INDEX		m_typeIndex;		// 각 상품 배열에서의 인덱스 

		INDEX		m_limitCnt;			// 플래티늄 아이템 남은 수량

		std::vector<ITEM_DATA> m_vItemDataArray;
		
		CASH_SHOP_DATA() :	m_shopItemIndex(-1) ,
							m_type(0),
							m_cash(0),
							m_mileage(0),
							m_itemListCnt(0) ,
							m_limitCnt(0),
							m_texID(0), m_texRow(0), m_texCol(0) {};
		~CASH_SHOP_DATA() { m_vItemDataArray.clear(); }

		BOOL IsNew() { return static_cast<BOOL>(m_flag & CATALOG_FLAG_NEW); }
		BOOL IsPopular() { return static_cast<BOOL>(m_flag & CATALOG_FLAG_POPULAR); }
		BOOL IsDiscount() { return static_cast<BOOL>(m_flag & CATALOG_FLAG_DISCOUNT); }
		BOOL IsPackage() { return (m_type > CATEGORY_PACKAGE && m_type < (CATEGORY_PACKAGE+10000)); }
		BOOL IsPackageEx() { return (m_itemListCnt > 1); }

		CASH_SHOP_DATA(const CASH_SHOP_DATA& csData)
		{
			(*this) = csData;
		}

		void operator = (const CASH_SHOP_DATA& csData);
		bool operator () ( const CASH_SHOP_DATA& _other )	{ return ((this->m_shopItemIndex == _other.m_shopItemIndex)?true:false); }
	};

	~CCashShopData();

	std::vector<CASH_SHOP_DATA> m_vShopItemArray;

	CASH_SHOP_DATA* GetCashShopData(INDEX shopItemIndex);

	static int	LoadShopDataFromFile(CStaticArray<CCashShopData> &apShopData, const char* FileName);
	static int	LoadShopDataFromFile(CCashShopData &apShopData, const char* FileName);
};

#endif