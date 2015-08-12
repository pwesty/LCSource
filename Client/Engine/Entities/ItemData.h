#ifndef INCL_ITEMDATA_H
#define INCL_ITEMDATA_H
#ifdef PRAGMA_ONCE
#pragma once
#endif

#include <Engine/LocalDefine.h>
#include <Engine/GlobalDefinition.h>
#include <vector>
#include <map>
#include <Engine/Base/CTString.h>
#include <Common/CommonDef.h>
#include <Common/header/def_item.h>
#include <Common/header/def_option.h>
#include <Engine/Help/LoadLod.h>

#define		MAX_MAKE_ITEM_MATERIAL		10	// 아이템 제작시 최대 재료 수

// Item Proto flag
#define		ITEM_FLAG_COUNT			((LONGLONG)1 << 0)	// 셀 수 있는 아이템인가
#define		ITEM_FLAG_DROP			((LONGLONG)1 << 1)	// 드롭  가능 아이템인가가
#define		ITEM_FLAG_UPGRADE		((LONGLONG)1 << 2)	// 업그레이드 가능한 아이템인가
#define		ITEM_FLAG_EXCHANGE		((LONGLONG)1 << 3)	// 교환 가능 아이템인가가 (유저간)
#define		ITEM_FLAG_TRADE			((LONGLONG)1 << 4)	// 매매 가능 아이템인가가
#define		ITEM_FLAG_NOT_DELETE	((LONGLONG)1 << 5)	// 파괴 가능 아이템인가가
#define		ITEM_FLAG_MADE			((LONGLONG)1 << 6)	// 생산 가능 아이템인가가
#define		ITEM_FLAG_MIX			((LONGLONG)1 << 7)	// 조합 아이템 : DB에 저장안됨
#define		ITEM_FLAG_CASH			((LONGLONG)1 << 8)	// 유료 아이템
#define		ITEM_FLAG_LORD			((LONGLONG)1 << 9)	// 성주 전용 아이템
#define		ITEM_FLAG_NO_STASH		((LONGLONG)1 << 10)	// 창고 보관 불가능 아이템
#define		ITEM_FLAG_CHANGE		((LONGLONG)1 << 11)	// 다른 직업용 아이템으로 교환 가능 여부 //wooss 051217
#define		ITEM_FLAG_COMPOSITE		((LONGLONG)1 << 12)	// 아이템 합성 가능 여부
#define		ITEM_FLAG_DUPLICATION	((LONGLONG)1 << 13)	// 중복 사용 묻기
#define		ITEM_FLAG_LENT			((LONGLONG)1 << 14)	// 대여용 무기
#define		ITEM_FLAG_RARE			((LONGLONG)1 << 15)	// 레어 아이템
#define		ITEM_FLAG_ABS			((LONGLONG)1 << 16)	// 아이템의 남은 시간 존재 여부
#define		ITEM_FLAG_NOT_REFORM	((LONGLONG)1 << 17)	// 아이템 리폼 가능 여부
#define		ITEM_FLAG_ZONEMOVE_DEL	((LONGLONG)1 << 18)	// 존 이동시 사라지는 아이템
#define     ITEM_FLAG_ORIGIN		((LONGLONG)1 << 19)	// 오리지널 옵션(SET_ITEM_ADD [ttos_2009_5_22]: 세트 아이템 적용)
#define		ITEM_FLAG_TRIGGER		((LONGLONG)1 << 20)	// 트리거 [090707: selo]
#define		ITEM_FLAG_RAIDSPE		((LONGLONG)1 << 21)	// 레이드 스페셜 아이템 [090707: selo]
#define		ITEM_FLAG_QUEST			((LONGLONG)1 << 22)	// 퀘스트 아이템 [090616: selo]
#define		ITEM_FLAG_BOX			((LONGLONG)1 << 23)	// LuckyDrawBox
#define		ITEM_FLAG_NOTTRADEAGENT	((LONGLONG)1 << 24)	// 거래대행등록불가
#define		ITEM_FLAG_DURABILITY	((LONGLONG)1 << 25)	// 내구도 아이템
#define		ITEM_FLAG_COSTUME2		((LONGLONG)1 << 26)	// 코스튬2 아이템
#define		ITEM_FLAG_SOCKET		((LONGLONG)1 << 27)	// socket flag. [6/22/2010 rumist]
#define		ITEM_FLAG_SELLER		((LONGLONG)1 << 28)		// [2010/08/27 : Sora] ADD_SUBJOB 상인추가
#define		ITEM_FLAG_CASTLLAN		((LONGLONG)1 << 29)		// 성주만 착용 가능한 아이템
#define		ITEM_FLAG_NONRVR		((LONGLONG)1 << 31)	// RVR일 경우 사용 불가 아이템 플래그.
#define		ITEM_FLAG_QUESTGIVE		((LONGLONG)1 << 32)	// 퀘스트 기부 가능 아이템
#define		ITEM_FLAG_TOGGLE		((LONGLONG)1 << 33)	// 토글 아이템.
#define		ITEM_FLAG_COMPOSE		((LONGLONG)1 << 34)	// 합성 아이템.
#define		ITEM_FLAG_NOTSINGLE		((LONGLONG)1 << 35) // 싱글 던전에서 사용 불가.
#define		ITEM_FLAG_INVISIBLE_COSTUME ((LONGLONG)1 << 36)	// 투명 코스튬

// Item Flag : 최상위 1비트는 사용 불가!!! 
// 6비트까지 케르, 넨, 일반 제련석의 레벨과, 플레티늄 제련석을 바른 아이템의 레벨로 사용 
#define		PLATINUM_MAX_PLUS 127 
#define		FLAG_ITEM_PLATINUM_GET(a, b) (b = a & PLATINUM_MAX_PLUS) 
#define		FLAG_ITEM_PLATINUM_SET(a, b) (a = ( (a &~ PLATINUM_MAX_PLUS) | b )) 
#define		FLAG_ITEM_OPTION_ENABLE			(1 << 7)	// 아이템 플래그에 옵션을 붙일 수 있는 상태를 구분, Bit 연산
#define		FLAG_ITEM_SEALED				(1 << 8)	// 아이템 봉인
#define		FLAG_ITEM_SUPER_STONE_USED		(1 << 9)	// 슈퍼고제로 업그레이드 했음..//0627
#define		FLAG_ITEM_BOOSTER_ADDED			(1 << 10)	// 부스터 장착 여부
#define		FLAG_ITEM_SILVERBOOSTER_ADDED	(1 << 11)	// 실버 부스터 장착 여부
#define		FLAG_ITEM_GOLDBOOSTER_ADDED		(1 << 12)	// 골드 부스터 장착 여부
#define		FLAG_ITEM_PLATINUMBOOSTER_ADDED	(1 << 13)	// 플래티늄 부스터 장착 여부
#define		FLAG_ITEM_COMPOSITION			(1 << 14)	// 합성 아이템 적용 여부
#define		FLAG_ITEM_LENT					(1 << 15)	// 대여된 아이템
#define		FLAG_ITEM_LEVELDOWN				(1 << 16)	// 레벨 다운된 아이템
#define		FLAG_ITEM_BELONG				(1 << 17)	// 귀속 아이템 귀속 상태(SET_ITEM_ADD [ttos_2009_5_22]: 세트 아이템 적용)
#define		FLAG_ITEM_SOCKET				(1 << 18)	// 소켓 가공했는지 여부 [5/10/2010 rumist]
#define		FLAG_ITEM_SUPER_RUNE_USED		(1 << 19)	// 가미고 요청으로 인한 초고급 룬 아이템 플래그 [8/24/2012 박훈] 

// SOCKET_SYSTEM_S2 보석 착용 위치 여부 [4/2/2013 Ranma]
#define		JEWEL_COMPOSITE_POSITION_WEAPON		(1 << 0)
#define		JEWEL_COMPOSITE_POSITION_HELMET		(1 << 1)
#define		JEWEL_COMPOSITE_POSITION_ARMOR		(1 << 2)
#define		JEWEL_COMPOSITE_POSITION_PANTS		(1 << 3)
#define		JEWEL_COMPOSITE_POSITION_GLOVES		(1 << 4)
#define		JEWEL_COMPOSITE_POSITION_SHOES		(1 << 5)
#define		JEWEL_COMPOSITE_POSITION_SHIELD		(1 << 6)
#define		JEWEL_COMPOSITE_POSITION_BACKWING	(1 << 7)

//SET_ITEM_ADD				//[ttos_2009_5_22]: 세트 아이템 적용
#define MAX_SET_OPTION  11
#define MAX_ITEM_SKILL			3			// 세트 아이템의 최대 스킬 
#define RUNE_ITEM_LEVEL			146			// 룬 적용 아이템레벨		

struct ItemSmcParseInfo 
{
	INDEX	iItemIndex;
	INDEX	iCount;
	INDEX	iTexCnt[3];
	CTString BmFileName[3];
	CTString ItemTEXFileName[3];
	CTString ItemTEXNormalFileName[3];

	ItemSmcParseInfo() : iItemIndex(0),iCount(0) 
	{
		iTexCnt[0] = 0; iTexCnt[1] = 0; iTexCnt[2];
	}
};

struct sPetItem_Info 
{
	//공격 펫 아이템
	CTString	pet_name;
	INDEX		pet_index;
	INDEX		pet_level;
	INDEX		pet_str;
	INDEX		pet_con;
	INDEX		pet_dex;
	INDEX		pet_int;
	INDEX		pet_cooltime;
	__int64		pet_accexp;
};

struct JewelComosInfo
{
	int index;
	int nor_comp_nas;
	int ca_comp_nas;
	int ca_jew_create;
	int nor_comp_val;
	int ca_comp_val;
	int nor_up_2;
	int nor_up_3;
	int ca_up_2;
	int ca_up_3;
	int nor_down_1;
	int nor_down_2;
	int nor_down_3;
	int ca_down_1;
	int ca_down_2;
	int ca_down_3;
};

class CFortuneData : public stFortune, public LodLoader<CFortuneData>
{
public:
	static bool loadFortuneEx(const char* FileName);
};

class ENGINE_API CItemData : public stItem, public LodLoader<CItemData>
{
private:
	// ITEM 툴에서 사용하고 있는 구조체
	// CItemData의 내부에서만 사용됩니다.
	int transFlag;
	std::string name;		// 아이템 이름.
	std::string descr;		// 설명.

	//0601 kwon 추가.
	char	fileBm[255];
	char	fileTex[255];
	char	fileTexNormal[255];
	char	fileBm2[255];
	char	fileTex2[255];
	char	fileTex2Normal[255];
	char	fileBm3[255];
	char	fileTex3[255];
	char	fileTex3Normal[255];
	int		Item_MeshCnt;
	int		Item_TexCnt;
	int		Item_Tex2Cnt;
	int		Item_Tex3Cnt;
	BOOL	bCreateSet;
	
public:
	DOUBLE	StartTime;

	enum ITEMTYPE
	{
		ITEM_WEAPON			= 0,	// 무기
		ITEM_SHIELD			= 1,	// 방어구
		ITEM_ONCEUSE		= 2,	// 일회용
		ITEM_BULLET			= 3,	// 탄환
		ITEM_ETC			= 4,	// 기타
		ITEM_ACCESSORY		= 5,	// 악세서리
		ITEM_POTION			= 6,	// 포션			// Date : 2005-01-07,   By Lee Ki-hwan
	};

	enum ITEMWEAPONTYPE
	{
		ITEM_WEAPON_KNIFE	= 0,	// 기사도(나이트)
		ITEM_WEAPON_CROSSBOW= 1,	// 석궁(로그)
		ITEM_WEAPON_STAFF	= 2,	// 스태프(메이지)
		ITEM_WEAPON_BIGSWORD= 3,	// 대검(타이탄)
		ITEM_WEAPON_AXE		= 4,	// 도끼(타이탄)
		ITEM_WEAPON_SSTAFF	= 5,	// 숏스태프(메이지)
		ITEM_WEAPON_BOW		= 6,	// 활(힐러)
		ITEM_WEAPON_DAGGER	= 7,	// 단검(로그)
		ITEM_WEAPON_MINING	= 8,	// 채굴도구
		ITEM_WEAPON_GATHERING= 9,	// 채집도구
		ITEM_WEAPON_CHARGE	= 10,	// 차지도구
		ITEM_WEAPON_TWOSWORD= 11,	// 이도류(나이트)
		ITEM_WEAPON_WAND	= 12,	// 완드(힐러)
		ITEM_WEAPON_SCYTHE	= 13,	// 사이드
		ITEM_WEAPON_POLEARM	= 14,	// 폴암.		
		ITEM_WEAPON_SOUL	= 15,	// 혼(나이트쉐도우)
	};

	// 탄환 종류
	enum ITEMBULLETTYPE
	{
		ITEM_BULLET_ATTACK	= 0,	// Attack Bullet
		ITEM_BULLET_MANA	= 1,	// Mana Bullet
		ITEM_BULLET_ARROW	= 2,	// Arrow
	};

	// 기타 타입
	enum ITEMETCTYPE
	{
		ITEM_ETC_QUEST			= 0,	// 퀘스트
		ITEM_ETC_EVENT			= 1,	// 이벤트
		ITEM_ETC_SKILL			= 2,	// 스킬 습득
		ITEM_ETC_REFINE			= 3,	// 제련
		ITEM_ETC_MATERIAL		= 4,	// 재료.
		ITEM_ETC_MONEY			= 5,	// 돈
		ITEM_ETC_PRODUCT		= 6,	// 생산품
		ITEM_ETC_PROCESS		= 7,	// 가공품
		ITEM_ETC_OPTION			= 8,	// 옵션 아이템
		ITEM_ETC_SAMPLE			= 9,	// 시료
		// 10번은 Texture보이기 위해서 사용.
		IETC_MIX_TYPE1			= 11, // 공성조합1
		IETC_MIX_TYPE2			= 12, // 공성조합2
		IETC_MIX_TYPE3			= 13, // 공성조합3
		ITEM_PET_AI				= 14,	// 펫 AI
		ITEM_ETC_QUESTTRIGGER	= 15,	// quest trigger. server used only.
		// 서버만 사용하는 퀘스트 트리거
		ITEM_ETC_JEWEL			= 16,	// jewels in socket system.
		ITEM_ETC_STABILIZER		= 17,	// stabilize about jewels combine.
		ITEM_ETC_PROC_SCROLL	= 18,	// process about socket creation.	
		ITEM_ETC_MONSTER_MERCENARY_CARD = 19, // 몬스터 용병 카드
		ITEM_ETC_GUILD_MARK		= 20,	// [sora] GUILD_MARK
		ITEM_ETC_REFORMER		= 21, //리폼 아이템
		ITEM_ETC_CHAOSJEWEL		= 22,
		ITEM_ETC_FUNCTIONS		= 23, // 기능성 아이템
		ITEM_ETC_SYNDICATEJEWEL = 24, // 결사대 창조의 보석 아이템
	};

	enum ITEMREFORMERTYPE
	{
		ITEM_ETC_REFORMER_MID_GRADE		= 0,
		ITEM_ETC_REFORMER_HIGH_GRADE	= 1,
	};

	// 옵션 아이템 종류.
	enum ITEMOPTIONTYPE
	{
		ITEM_OPTION_BLOOD	= 0,	// Blood Item
		ITEM_OPTION_CLEAR	= 1,	// Clear Item
	};

	// 방어구 종류.
	enum ITEMSHIELDTYPE
	{
		ITEM_SHIELD_HEAD	= 0,	// 머리
		ITEM_SHIELD_COAT	= 1,	// 상의
		ITEM_SHIELD_PANTS	= 2,	// 하의
		ITEM_SHIELD_GLOVE	= 3,	// 장갑
		ITEM_SHIELD_SHOES	= 4,	// 신발
		ITEM_SHIELD_SHIELD	= 5,	// 방패	
		ITEM_SHIELD_BACKWING = 6,	// 날개
		ITEM_SHIELD_ONE_SUIT  = 7,   // 한 벌 의상 added by sam 11/01/31 [SAM]		
	};

	// 일회용
	enum ITEMSUBTYPE		// Date : 2005-01-07,   By Lee Ki-hwan : 포션 수정
	{
		ITEM_SUB_WARP			= 0,// 워프
	
		// 생산 시스템
		ITEM_SUB_PROCESS_DOC		= 1,	// 가공 문서
		ITEM_SUB_MAKE_TYPE_DOC		= 2,	// 제작 종류, 레벨 문서
		ITEM_SUB_BOX				= 3,	// 상자
		ITEM_SUB_MAKE_POTION_DOC	= 4,	// 포션 제작 문서 // Date : 2005-01-07,   By Lee Ki-hwan
		ITEM_SUB_CHANGE_DOC			= 5,
		ITEM_SUB_QUEST_SCROLL		= 6,
		ITEM_SUB_CASH_ITEM			= 7,	// 일회용 캐쉬아이템
		ITEM_SUB_ETC				= 9,	// 기타 일회
		ITEM_SUB_TARGET				= 10,	// 타겟 아이템용
		ITEM_SUB_TITLE				= 11,	// 호칭 아이템
		ITEM_SUB_JUMPING_PAKAGE		= 12,	// 점핑 패키지 : 사용하면 점핑 포션과 보상 아이템을 지급해준다.
		ITEM_SUB_JUMPING_POTION		= 13,	// 점핑 포션 : 사용하면 캐릭테 레벨을 특정 레벨로 올려주는 아이템
		ITEM_SUB_CHAR_SLOT_EXTEND	= 14,	// 캐릭터 슬롯 확장
		ITEM_SUB_CHAR_SERVER_MOVE	= 15,	// 캐릭터 서버 이전 [Ranma]
		ITEM_SUB_EXPRESS_REMOTE		= 16,	// LCE 원격 
		ITEM_SUB_JEWEL_POCKET		= 17,	// 일반 보석 주머니
		ITEM_SUB_CHAOS_JEWEL_POCKET	= 18,	// 카오스 보석 주머니
		ITEM_SUB_INVEN_CASH_BAG_KEY	= 19,	// 캐쉬 인벤토리 열쇠 아이템.
		ITEM_SUB_PET_STASH_USE_ITEM = 20,	// 펫 창고 이용권
		ITEM_SUB_GPS				= 21,	// GPS
		ITEM_SUB_HOLY_WATER			= 22,	// 성수 아이템.
		ITEM_SUB_PROTECT_PVP		= 23,	// PVP 방어아이템.
	};

	enum ACCESSORYTYPE
	{
		ACCESSORY_CHARM			= 0,	// 부적
		ACCESSORY_MAGICSTONE	= 1,	// 마련석
		ACCESSORY_LIGHTSTONE	= 2,	// 반짝이는돌
		ACCESSORY_EARING		= 3,	// 귀걸이.
		ACCESSORY_RING			= 4,	// 반지
		ACCESSORY_NECKLACE		= 5,	// 목걸이
		ACCESSORY_PET			= 6,	// 펫
		ACCESSORY_WILDPET		= 7,	// 공격형 펫
		ACCESSORY_RELIC			= 8,	// 유물
	};

	// SubType : 포션  // Date : 2005-01-07,   By Lee Ki-hwan
	enum POTIONTYPE
	{
		POTION_STATE	= 0,	// 상태이상치료
		POTION_HP		= 1,	// 생명회복
		POTION_MP		= 2,	// 마나회복
		POTION_DUAL		= 3,	// HP+MP 회복
		POTION_STAT		= 4,	// Stat 상승
		POTION_ETC		= 5,	// 기타 (이동향상)	
		POTION_UP		= 6,	// 공방향상
		POTION_TEARS	= 7,	// 눈물
		POTION_CRYSTAL	= 8,	// 결정
		POTION_POTAL_SCROLL = 9, // NPC포탈 스크롤
		POTION_INC_HEAL_HP, // HP 회복속도 증가.
		POTION_INC_HEAL_MP, // MP 회복속도 증가.
		POTION_PET_HEAL_HP,		// 펫 HP회복.
		POTION_INC_PET_MOVE_SPD, // 펫 이동속도 증가.
		POTION_TOTEM,			// 토템 아이템.
		POTION_PET_HEAL_MP		// 펫 MP회복
	};

	enum POTIONSUBTYPE_UP
	{
		POTION_UP_PHYSICAL	= 0,	// 물리
		POTION_UP_MAGIC		= 1,	// 마법
		POTION_UP_ATTACK	= 0,	// 공격
		POTION_UP_DEFENSE	= 1,	// 방어
	};

	// IONCE_PROCESS_DOC 종류 1
	enum PROCESSDOCTYPE
	{
		PROCESS_DOC_STONE		= 0,   // 광석 정련 문서
		PROCESS_DOC_PLANT		= 1,   // 식물 가공 문서
		PROCESS_DOC_ELEMENT		= 2,   // 원소 정제 문서
	};

	// 스톤 정련 문서 종류
	enum PROCESSDOCSUBTYPE_STONE 
	{
		PROCESS_DOC_SUB_STONE0	= 0,		// 스톤 온 정련 문서
		PROCESS_DOC_SUB_STONE1	= 1,		// 스톤 리스 정련 문서
		PROCESS_DOC_SUB_STONE2	= 2,		// 스톤 웨버 정련 문서
		PROCESS_DOC_SUB_STONE3	= 3,		// 스톤 비스트 정련 문서
		PROCESS_DOC_SUB_STONE4	= 4,		// 스톤 웰스던 정련 문서
	};
	
	// 식물 정련 문서 종류
	enum PROCESSDOCSUBTYPE_PLANT
	{
		PROCESS_DOC_SUB_PLANT0	= 0,		// 크락의 노란잎 가공 문서
		PROCESS_DOC_SUB_PLANT1	= 1,		// 크락의 줄기 가공 문서
		PROCESS_DOC_SUB_PLANT2	= 2,		// 크락의 파란잎 가공 문서
		PROCESS_DOC_SUB_PLANT3	= 3,		// 크락의 가시 가공 문서
		PROCESS_DOC_SUB_PLANT4	= 4,		// 크락의 꽃 가공 문서
	};

	// 원소 정련 문서 종류
	enum PROCESSDOCSUBTYPE_ELEMENT
	{
		PROCESS_DOC_SUB_ELEMENT0	= 0,	// E등급 원소 정제 문서
		PROCESS_DOC_SUB_ELEMENT1	= 1,	// D등급 원소 정제 문서
		PROCESS_DOC_SUB_ELEMENT2	= 2,	// C등급 원소 정제 문서
		PROCESS_DOC_SUB_ELEMENT3	= 3,	// B등급 원소 정제 문서
		PROCESS_DOC_SUB_ELEMENT4	= 4,	// A등급 원소 정제 문서
	};

	// ONCE_BOX  종류 // Date : 2005-01-12,   By Lee Ki-hwan
	enum BOXTYPE
	{
		BOX_REMAKE		= 0,		// 재활의상자
		BOX_ARCANE		= 1,		// 비밀의상자
	};

	enum REFINE_TYPE // Date : 2005-01-12,   By Lee Ki-hwan
	{
		REFINE_GENERAL	= 0,		// 일반제련석
		REFINE_SPECIAL	= 1,		// 고급제련석
		REFINE_SUPER_SPECIAL = 2,	// 초고급제련석
		REFINE_BOOSTER	= 3,		// 부스터
		REFINE_LUCKY	= 4,		// 행운의 고급 제련석
		REFINE_PLATINUM	= 5,		// 플래티넘 제련석
		REFINE_CHAOS	= 6,		// 특급 제련석
		REFINE_PURITY_RUNE = 7,		// 정화된 룬
		REFINE_DEVIL_RUNE = 8,		// 악마의 룬 
		REFINE_CHAOS_RUNE = 9,		// 카오스 룬 [8/24/2012 박훈] 
		REFINE_SUPER_RUNE = 10,		// 초고급 룬
		REFINE_LUCKY_RUNE = 11		// 행운 룬
	};

	enum LORD_ITEM_TYPE
	{
		LORD_ITEM_BIGSWORD	= 0,	// 대검(타이탄)
		LORD_ITEM_AXE		= 1,	// 도끼(타이탄)
		LORD_ITEM_KNIFE		= 2,	// 기사도(나이트)
		LORD_ITEM_TWOSWORD	= 3,	// 이도류(나이트)
		LORD_ITEM_BOW		= 4,	// 활(힐러)
		LORD_ITEM_WAND		= 5,	// 완드(힐러)
		LORD_ITEM_SSTAFF	= 6,	// 숏스태프(메이지)
		LORD_ITEM_STAFF		= 7,	// 스태프(메이지)
		LORD_ITEM_DAGGER	= 8,	// 단검(로그)
		LORD_ITEM_CROSSBOW	= 9,	// 석궁(로그)
		LORD_ITEM_SCYTHE    = 10,   // 사이드(소서러)
		LORD_ITEM_POLARM    = 11,	// 폴암(소서러)
		LORD_ITEM_TOTAL		= 12
	};

	// AI 적용 범위
	enum PET_AI_SET0 
	{
		PET_AI_RANGE_3M		= 1,		
		PET_AI_RANGE_5M		= 2,		
		PET_AI_RANGE_10M	= 3,
		PET_AI_RANGE_20M	= 4,
		PET_AI_RANGE_30M	= 5,
	};
	// AI 세부선택
	enum PET_AI_SET1
	{
		PET_AI_HP		= 1,
		PET_AI_MP		= 2,
		PET_AI_STM		= 3,
		PET_AI_FAITH	= 4,
	};

	// AI 세부선택 조건( %이하 )
	enum PET_AI_SET2
	{
		PET_AI_PERCENT_99	= 1,
		PET_AI_PERCENT_90	= 2,
		PET_AI_PERCENT_80	= 3,
		PET_AI_PERCENT_70	= 4,
		PET_AI_PERCENT_60	= 5,
		PET_AI_PERCENT_50	= 6,
	};

	// AI 적용 타겟
	enum PET_AI_SET3
	{
		PET_AI_OWNER_ATTACK_ENEMY	= 1,	// 주인이 공격하는 적
		PET_AI_ENEMY_ATTACK_OWNER	= 2,	// 주인을 공격하는 적
		PET_AI_ENEMY_ATTACK_PET		= 3,	// 펫을 공격하는 적
		PET_AI_ENEMY				= 4,	// 적
		PET_AI_PET					= 5,	// 펫
		PET_AI_OWNER				= 6,	// 주인
		PET_AI_ITEM					= 7,	// 아이템
	};

	// 공성전 ITEM 사용
	enum eCASTLE_WAR
	{
		eANYWHERE = 0,		//	0	어디서든 사용 가능
		eSIEGEAREA_ALL,		//	1	전체 공성에서만 사용 가능
		eSIEGEAREA_MERAC,	//	2	메라크 공성에서만 사용 가능
		eSIEGEAREA_DRATAN,	//	3	드라탄 공성에서만 사용 가능
	};
	
	/* Default constructor. */
	CItemData(void);
	/* Destructor. */
	~CItemData(void);

	CItemData& operator = (const CItemData& data);

	//----------------------------------------------------
	// Properties
	// - ItemData는 ItemTool에서 설정된 데이터를 받아서 처리하는데,
	// - 게임 내부에서 데이터가 변경될 일이 거의 없을 것이다.
	// - 고로, 추후에는 프로퍼티 함수를 갖고 있는 것보다는 구조체를 
	// - public으로 노출하는게 좋을듯.	
	// - 아니면, ItemData 구조체를 반환하는 것도 좋을거 같은데...
	// - 또한, 게임 내에서 데이터가 변경될 일이 없으므로, Set 메소드들은 필요가 없을듯...
	//----------------------------------------------------	

	// 싱글 모드 전용.
	inline BOOL IsExclusiveSingleMode()
	{	return FALSE; };

	inline BOOL	CanUse(int iPlayerID)	
	{	return ( job & (1 << iPlayerID) );	};
	
	inline BOOL IsProductionItem()
	{
		return ( subType == ITEM_WEAPON_MINING ||
			subType == ITEM_WEAPON_GATHERING ||
			subType == ITEM_WEAPON_CHARGE );
	}

	inline void SetItemSmcFileName(const char* FileName)
	{	strcpy(fileSMC, FileName);}

	inline void SetArmorEffectName(const char* FileName)
	{	strcpy(efffectName, FileName);	}

	inline void SetItemBMFileName(const char* FileName)
	{	strcpy(fileBm, FileName);}
	
	inline void SetItemTEXFileName(const char* FileName)
	{	strcpy(fileTex, FileName);}
	
	inline void SetItemTEXNormalFileName(const char* FileName)
	{	strcpy(fileTexNormal, FileName);}
	
	inline void SetItemBM2FileName(const char* FileName)
	{	strcpy(fileBm2, FileName);}
	
	inline void SetItemTEX2FileName(const char* FileName)
	{	strcpy(fileTex2, FileName);	}
	
	inline void SetItemTEX2NormalFileName(const char* FileName)
	{	strcpy(fileTex2Normal, FileName);}

	inline void SetItemBM3FileName(const char* FileName)
	{	strcpy(fileBm3, FileName);}
	
	inline void SetItemTEX3FileName(const char* FileName)
	{	strcpy(fileTex3, FileName);	}
	
	inline void SetItemTEX3NormalFileName(const char* FileName)
	{	strcpy(fileTex3Normal, FileName);}
	
	inline void SetMeshCnt(int Index)
	{	Item_MeshCnt = Index;}
	
	inline void SetTexCnt(int Index) //첫번째 메쉬의 텍스쳐 카운트.
	{	Item_TexCnt = Index;}
	
	inline void SetTex2Cnt(int Index) //두번째 메쉬의 텍스쳐 카운트.
	{	Item_Tex2Cnt = Index;}
	
	inline void SetTex3Cnt(int Index) //세번째 메쉬의 텍스쳐 카운트.
	{	Item_Tex3Cnt = Index;}
	
	inline void SetCreateSet(BOOL bSet)
	{ bCreateSet = bSet; }

	//--------------------------------------------------------------
	inline const char* GetItemSmcFileName()
	{	return fileSMC;}

	inline const char* GetArmorEffectName()
	{	return efffectName;	}

	inline const char* GetMissileShotEffect()
	{	return attackEffectName; }

	inline const char* GetShotHitEffect()
	{	return damageEffectName;	}
	
	inline const char* GetItemBMFileName()
	{	return fileBm;}
	
	inline const char* GetItemTEXFileName()
	{	return fileTex;}
	
	inline const char* GetItemTEXNormalFileName()
	{	return fileTexNormal;}
	
	inline const char* GetItemBM2FileName()
	{	return fileBm2;}
	
	inline const char* GetItemTEX2FileName()
	{	return fileTex2;}
	
	inline const char* GetItemTEX2NormalFileName()
	{	return fileTex2Normal;}

	inline const char* GetItemBM3FileName()
	{	return fileBm3;}
	
	inline const char* GetItemTEX3FileName()
	{	return fileTex3;}
	
	inline const char* GetItemTEX3NormalFileName()
	{	return fileTex3Normal;}
	
	inline __int64 GetFlag()		const
	{	return flag;}

	inline bool IsFlag( __int64 nFlag ) const 
	{
		if( flag & nFlag ) return true;
		return false;
	}

	inline BOOL GetCreateSet( void ) const
	{ return bCreateSet; }
	
	inline int GetMeshCnt()		const
	{	return Item_MeshCnt;}
	
	inline int GetTexCnt()		const
	{	return Item_TexCnt;}
	
	inline int GetTex2Cnt()		const
	{	return Item_Tex2Cnt;}

	inline int GetTex3Cnt()		const
	{	return Item_Tex3Cnt;}
	
	inline int	GetNum0() const		
	{	return num0;	}

	inline int	GetNum1() const		
	{	return num1;	}

	inline int	GetNum2() const		
	{	return num2;	}

	inline int	GetNum3() const		
	{	return num3;	}

	inline int GetItemIndex()	const
	{	return index;	}
	
	inline int GetGrade()		const
	{	return num2;}

	inline int GetJob()			const
	{	return job; }

	inline int GetLevel()		const
	{	return level;	}
	inline int GetStack() const { return stack;	}

	inline int GetFame()		const
	{	return fame;	}
	
	// 착용 위치를 반환합니다.
	inline int GetWearingPosition()	const
	{	return wearing;}
	
	inline int	GetType()		const
	{	return type;	}
	
	inline int	GetSubType()	const
	{	return subType;	}
	
	inline int	GetZoneNum()	const
	{	return num1;	}

	inline int	GetExtraNum()	const
	{	return num2;	}

	inline int	GetQuestNum()	const
	{	return	num0;	}

	inline int	GetPetType()	const
	{	return num0;	}
	
	inline int	GetPrice()		const
	{	return price;}

	inline int	GetProcessType()	const		// 가공 타입
	{	return num0;	}

	inline int	GetProcessSubType()	const		// 서브 타입
	{	return num1;	}

	inline int	GetProductType()	const		// 제조 타입
	{	return num0;	}

	inline int	GetProductSubType()	const		// 제조 서브 타입
	{	return num1;	}

	inline int	GetProcessSkill()	const		// 가공 스킬
	{	return num2;	}

	inline int	GetBoxType()	const		// Box type
	{	return num0;	}

	inline int	GetRefineType()	const		// Refinetype type
	{	return num0;	}

	inline int GetMakePotionCount () const 
	{	return num3;	}

	inline int	GetNeedItemIndex( int nIndex )	const	
	{	return needItemIndex[nIndex];	}

	inline int	GetNeedItemCount( int nIndex )	const
	{	return needItemCount[nIndex];	}

	inline int	GetNeedSSkillIndex()	const
	{	return needSSkillIndex;	}

	inline int	GetNeedSSkillCount()	const
	{	return needSSkillCount;	}

	inline int	GetNeedSSkillIndex2()	const
	{	return needSSkillIndex2;	}

	inline int	GetNeedSSkillCount2()	const
	{	return needSSkillCount2;	}
	
	inline int	GetPhysicalDefence()	const
	{	return num0;	}
	
	int	GetPhysicalAttack();		// 공성 보상 작업 추가로 일반 함수로 전환 
	int	GetMagicAttack();
		
	inline int	GetMagicDefence()		const
	{	return num1;	}
	
	inline int	GetAttackSpeed()		const
	{	return num2;}
	
	inline int	GetRecoverSec()			const
	{	return num0;		}

	inline int	GetRecoverHP()			const
	{	return num1;	}
	
	inline int	GetRecoverMP()			const
	{	return num2;	}
	
	inline int	GetSkillType()			const
	{	return num0;	}
	
	inline int	GetSkillLevel()			const
	{	return num1;}

	inline int	GetOptionType()			const
	{	return num0;}
	
	inline const char* GetName()
	{	return name.c_str();	}

	inline void SetName(const char* str)	{ name = str; }

	inline const char* GetDesc()
	{	return descr.c_str();	}

	inline void SetDesc(const char* str)	{ descr = str; }

	inline int	GetWarpType() const 
	{ return num0; }
	
	inline int GetIconTexID()			const
	{	return textureID;}
	
	inline int GetIconTexRow()			const
	{	return textureRow;}
	
	inline int GetIconTexCol()			const
	{	return textureCol;	}

	inline int GetPetAISet0() const // SOCKET_SYSTEM_S2 에서 보석일 경우 장착 위치가 넘어옴 [4/2/2013 Ranma]
	{ return set0; }

	inline int GetPetAISet1() const
	{ return set1; }

	inline int GetPetAISet2() const
	{ return set2; }

	inline int GetPetAISet3() const
	{ return set3; }

	//SET_ITEM_ADD				//[ttos_2009_5_22]: 세트 아이템 적용
	inline int GetSetItemIndex() const
	{ return set4; }


	// [090608: selo] 퀘스트 아이템 사용 제한
	inline int GetRestrictionZoneNo() const		// 사용제한 존 번호
	{ return set0; }

	inline int GetRestrictionPosX() const		// 사용제한 X좌표
	{ return set1; }

	inline int GetRestrictionPosY() const		// 사용제한 Y좌표
	{ return set3; }

	inline int GetRestrictionPosZ() const		// 사용제한 Z좌표
	{ return set2; }

	inline int GetRestrictionRadius() const		// 사용제한 반경
	{ return set4; }
	
	// socket system [7/16/2010 rumist]
	inline int GetSocketOptionIndex()	const
	{ return rareOptionType[0];	}

	inline int GetSocketOptionLevel()	const
	{ return rareOptionLevel[0];	}

	inline	int GetJumpingLevel() const
	{ return num0;	}

	// [2013/02/13] sykim70 Fortune System
	inline int GetFortuneIndex() const
	{ return fortuneIndex; }

	static bool IsLordItem( int nIndex ) ; // 성주 무기 인지 아닌지 판다. 
	static bool IsUniqueItem( int nIndex ) ; // 유니크 무기 인지 아닌지 판다. 

	// [2010/10/20 : Sora] 용병 카드집 체크
	bool IsMercenaryCardHolder();

	inline int GetItemBelong()	{ return rareOptionType[0]; }

	inline int GetOptionOriginType(int idx)
	{ 
		if (idx < 0 || idx >= MAX_ORIGIN_OPTION)
			return 0;
		return rareOptionType[idx + 1];
	}
	inline int GetOptionOriginLevel(int idx)
	{ 
		if (idx < 0 || idx >= MAX_ORIGIN_OPTION)
			return 0;
		return rareOptionLevel[idx + 1];
	}

	inline int GetOptionSkillType(int idx)
	{ 
		if (idx < 0 || idx >= MAX_ITEM_SKILL)
			return 0;
		return rareOptionType[idx + MAX_ORIGIN_OPTION + 1];
	}
	inline int GetOptionSkillLevel(int idx)
	{ 
		if (idx < 0 || idx >= MAX_ITEM_SKILL)
			return 0;
		return rareOptionLevel[idx + MAX_ORIGIN_OPTION + 1];
	}

	inline char GetCastleWar()
	{ 
		return castleWar;
	}

	int& GetTransFlag() {
		return transFlag;
	}

	inline int GetSyndicateType()
	{ 
		return syndicate_type;
	}

	inline int GetSyndicateGrade()
	{ 
		return syndicate_grade;
	}

	//[sora] 미번역 스트링 index 표시
	void SetNoTranslate();
	void ClearNoTranslate();

	static int LoadItemNameFromFile(const char* FileName);
	
	//-----------------------------------------------------------------------------
	// Purpose: 아이템 데이터를 화일로부터 읽어들입니다.
	// Input  : &apItemData - 저장될 배열.
	//			FileName - 파일명
	// Output : 	static int
	//-----------------------------------------------------------------------------
	static bool loadItemEx(const char* FileName);
	static int LoadJewelItemDataFromFile(CStaticArray<JewelComosInfo> &apJewelData, const char* FileName);
	static void LoadItemSmcDataFromFile(CStaticArray<ItemSmcParseInfo> &apItemSmcData, const char* FileName);

#if		(_MSC_VER <= 1200)

	static int getsize()	{ return (int)_mapdata.size(); }

	static CItemData* getDataSeq(int Idx)
	{
		if (Idx < 0 || Idx >= _vecdata.size())
			return m_dummy;

		return _vecdata[Idx];
	}

#endif	// (_MSC_VER <= 1200)
};

class CItemRareOption : public stRareOption, public LodLoader<CItemRareOption>
{
private:
	std::string m_strPrefix;			// 접두어
public:
	CItemRareOption();
	~CItemRareOption();

	inline const int GetIndex()				
	{	return getindex(); }
	inline const int GetGrade()				
	{	return a_grade;		}	
	inline const int GetType()				
	{	return a_type; }
	void SetPrefix(const char* str)			{ m_strPrefix = str;		}
	inline const char* GetPrefix()			{ return m_strPrefix.c_str(); }
	inline const int GetPhysicalAttack()	
	{	return a_attack;	}
	inline const int GetPhysicalDefence()	
	{	return a_defense;	}
	inline const int GetMagicAttack()		
	{	return a_magic;		}
	inline const int GetMagicDefence()		
	{	return a_resist;	}

	static bool loadEx(const char* FileName);

	//[sora] 미번역 스트링 index 표시
	void SetNoTranslate()
	{
		char buff[MAX_PATH] = {0,};
		sprintf( buff, "[%d] : rare option", getindex() );
		m_strPrefix = buff;
	}

	void ClearNoTranslate()
	{
		m_strPrefix = "";
	}
};

//SET_ITEM_ADD				//[ttos_2009_5_22]: 세트 아이템 적용
class CSetItemData : public stSetItem, public LodLoader<CSetItemData>
{
private:
	struct SetOption
	{
		int nSetCount;
		int nSetType;
		int nSetOptionIndex;
		int nOptionLevel;
	};
	std::string	m_strSetItemName;	// SetItem Name

public:
	enum SET_OPTION_TYPE
	{
		SET_ITEM_TYPE_NONE = -1,
		SET_ITEM_TYPE_OPTION = 0,
		SET_ITEM_TYPE_SKILL = 1,
	};
	CSetItemData();
	~CSetItemData();

	inline const int GetSetItemIndex()		{ return getindex(); }
	void SetName(const char* str)			{ m_strSetItemName = str; }
	inline const char* GetName()			{ return m_strSetItemName.c_str(); }
	inline const LONG GetApplyItemIndex(int nWearPos)	{ return item[nWearPos]; }
	inline const SetOption GetOption(int OptionNum)		
	{
		SetOption setOpdata;
		
		setOpdata.nSetCount = wearCnt[OptionNum];
		setOpdata.nSetType = optionType[OptionNum];
		setOpdata.nSetOptionIndex = option[OptionNum];
		setOpdata.nOptionLevel = optionLevel[OptionNum];
			
		return setOpdata;
	}
	inline const int GetMaxOption()		{ return optionCnt; }
	static bool loadSetItemEx(const char* FileName);
};

#define  MAX_STUFF_ITEM	5

class CMakeItemData
{
private:
	struct SetStuff
	{
		int nStuff_Index;
		int	nStuff_Count;
	};

	int		m_nFactory_Index;				// 제작 아이템 인덱스
	char	m_strFactory_Name[50];			// 제작 아이템 이름
	int		m_nFactory_Type;				// 제작 타입
	int		m_nFactory_Subtype;				// 제작 서브 타입
	int		m_nItemIndex;					// 아이템 데이터 인덱스
	UQUAD	m_nMakeExp;						// 고유 숙련도
	UQUAD	m_nNeedExp;						// 필요 숙련도
	UQUAD	m_nNeedNas;						// 필요 나스
	SetStuff m_StuffItem[MAX_STUFF_ITEM];	// 재료 아이템

public:
	CMakeItemData();
	~CMakeItemData();

	int GetMakeItemType() { return m_nFactory_Type; }
	int	GetMakeItemSubType() { return m_nFactory_Subtype; }
	char* GetName()	{ return m_strFactory_Name; }
	int GetFactoryIndex()	{ return m_nFactory_Index; }
	UQUAD GetMakeExp()		{ return m_nMakeExp; }
	UQUAD GetNeedNas()		{ return m_nNeedNas; }
	UQUAD GetNeedExp()		{ return m_nNeedExp; }
	int GetItemButtonIndex() { return m_nItemIndex; }
	SetStuff GetStuffItem(int Col)	{ return m_StuffItem[Col]; }
	int	GetStuffItemIndex(int Col)	{ return m_StuffItem[Col].nStuff_Index; }
	int	GetStuffItemCount(int Col)	{ return m_StuffItem[Col].nStuff_Count; }

	static int LoadMakeItemFromFile(CStaticArray<CMakeItemData> &apMakeItem, const char* FileName);
};

#endif