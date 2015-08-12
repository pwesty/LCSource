// ---------------------------------------------------------------------
// File : UIPetItemMix.h
// Desc : 펫 아이템 조합
// ---------------------------------------------------------------------
#ifndef UIPETITEMMIX_H_
#define UIPETITEMMIX_H_
#ifdef	PRAGMA_ONCE 
	#pragma once
#endif

#include <Engine/Entities/Itemdata.h>
#include <Engine/Interface/UIProcess.h> // 창의 크기값은 UIProcess의 define값을 사용

enum DRAGON_WEAPON_TYPE
{
	T_DRAGON_SWORD = 0,		// 드래곤 소드
	T_DUAL_DRAGON_SWORD,	// 듀얼 드래곤 소드
	T_DRAGON_BLADE,		// 드래곤 블레이드
	T_DRAGON_AXE,			// 드래곤 액스
	T_DRAGON_WAND,		// 드래곤 완드
	T_DRAGON_BOW,			// 드래곤 보우
	T_DRAGON_STICK,		// 드래곤 스틱
	T_DRAGON_STAFF,		// 드래곤 스테프
	T_DRAGON_DAGGER,		// 드래곤 대거
	T_DRAGON_ARMLETBOW,	// 드래곤 암릿 보우
	T_DRAGON_SIDE,		// 드래곤 사이드
	T_DRAGON_LOAD,		// 드래곤 로드
	T_DRAGON_MAX,
};

enum KNIGHT_AMOR_TYPE
{
	T_HELMET = 0,		// 투구
	T_COAT,			// 상의
	T_PANTS,			// 하의
	T_BOOTS,			// 부츠
	T_GLOVE,			// 장갑
	T_SHIELD,
	T_KNIGHT_MAX,		
};

enum UNIQUE_INDEX				// 유니크 아이템 인덱스
{
	UNI_DRAGON_SWORD = 887,		// 드래곤 소드
	UNI_DUAL_DRAGON_SWORD = 907,	// 듀얼 드래곤 소드
	UNI_DRAGON_BLADE = 908,		// 드래곤 블레이드
	UNI_DRAGON_AXE = 909,			// 드래곤 액스
	UNI_DRAGON_WAND = 910,		// 드래곤 완드
	UNI_DRAGON_BOW = 911,			// 드래곤 보우
	UNI_DRAGON_STICK = 912,		// 드래곤 스틱
	UNI_DRAGON_STAFF = 913,		// 드래곤 스테프
	UNI_DRAGON_DAGGER = 914,		// 드래곤 대거
	UNI_DRAGON_ARMLETBOW = 915,	// 드래곤 암릿 보우
	UNI_DRAGON_SIDE = 916,		// 드래곤 사이드
	UNI_DRAGON_LOAD = 917,		// 드래곤 로드
	UNI_TITAN_HELMET = 918,		// 타이탄 
	UNI_TITAN_COAT = 919,
	UNI_TITAN_PANTS = 920,
	UNI_TITAN_BOOTS = 921,
	UNI_TITAN_GLOVE = 922,
	UNI_KNIGHT_HELMET = 923,	// 기사
	UNI_KNIGHT_COAT = 924,
	UNI_KNIGHT_PANTS = 925,
	UNI_KNIGHT_BOOTS = 926,
	UNI_KNIGHT_GLOVE = 927,
	UNI_KNIGHT_DEFENDSHIELD = 1280,
	UNI_MAGE_CIRCLET = 928,		// 메이지
	UNI_MAGE_COAT = 929,
	UNI_MAGE_PANTS = 930,
	UNI_MAGE_BOOTS = 931,
	UNI_MAGE_GLOVE = 932,
	UNI_LOG_HOOD = 933,			// 로그
	UNI_LOG_COAT = 934,
	UNI_LOG_PANTS = 935,
	UNI_LOG_BOOTS = 936,
	UNI_LOG_GLOVE = 937,
	UNI_HEALER_HEALCIRCLET = 938,		// 힐러
	UNI_HEALER_COAT = 939,
	UNI_HEALER_PANTS = 940,
	UNI_HEALER_BOOTS = 941,
	UNI_HEALER_GLOVE = 942,
	UNI_SORCERER_CIRCLET = 943,			// 소서러
	UNI_SORCERER_COAT = 944,
	UNI_SORCERER_PANTS = 945,
	UNI_SORCERER_BOOTS = 946,
	UNI_SORCERER_GLOVE = 947,
};

struct  CNeedStuff {
	int		nNeedA[3];		// 필요재료 
	int		nNeedB[3];
};

// 네트워크 메세지를 위한 데이터 구조체 ( UIProces의 CNeedItems 이용 )

class CUIPetItemMix : public CUIWindow
{
protected:
	
// Controls...
	
	// Button
	CUIButton				m_btnClose;							// 닫기 버튼 
	CUIButton				m_btnOK[3];							// 제작 버튼( KNIGHT [1 -> A, 2 -> B] , 0 -> DRAGON ) 
	CUIButton				m_btnCancel;						// 취소 버튼 
	
	// Skill buttons
	std::vector< CUIIcon* > m_vecIcons;							// 조합 아이템


	// Etc ...
	CUIScrollBar			m_sbProcessItem;					// 제작 아이템 창 스크롤 바
	CUIListBox				m_lbPreconditionDesc;				// 필요 조건 설명 리스트 박스
	
	
//	Process Item Info

//	int						m_nProcessText;						// 가공 문서 아이템 인덱스
	SBYTE					m_nRow;
	SBYTE					m_nCol;

	int						m_nSelectProcessItem;				// 현재 선택된 가공물
	int						m_nProcessItemCount;				// 가공문서에 맞는 가공품 갯수
	
	BOOL					m_bSatisfiedA;						// 조건이 충분한가?
	BOOL					m_bSatisfiedB;						// 조건이 충분한가?
	
	CNeedItems				m_NeedItems[MAX_MAKE_ITEM_MATERIAL];// 필요 아이템 정보

	int						m_nNeedItemCount;					// 필요한 아이템 종류의 수
	int						m_nJobIndex[TOTAL_JOB];				// 직업 별 생산 유니크 아이템 인덱스 시작 값

	CNeedStuff				m_NStuffKnight[T_KNIGHT_MAX];		// 나이트 메어 필요 재료 정보

// Region of each part
	UIRect					m_rcTitle;							// Region of title bar
	UIRect					m_rcIcons;							// Region of icons
	UIRect					m_rcTab;							// Region of tab
	UIRect					m_rcItem;
	UIRect					m_rcDesc;
	
// UV of each part
	UIRectUV				m_rtBackgroundTop;					// UV of background UP
	UIRectUV				m_rtBackgroundBtm;					// UV of background DOWN
	UIRectUV				m_rtSelOutline;						// UV of outline of selected button

// Network ...
	BOOL					m_bWaitProcessResult;				// Wait Message
	
	int						m_nPetType;

	CTString				m_strItemInfo[T_DRAGON_MAX];		// Item information string
	COLOR					m_colItemInfo[T_DRAGON_MAX];		// Color of item information string
	BOOL					m_bDetailItemInfo;

	void	ClearProcessItems();

public:
	CUIPetItemMix();
	~CUIPetItemMix();

	
// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );
	
// Render
	void	Render();

// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	
// Open & close Process
	void	Clear ();
	void	InitPetItemMix( int nType );

	ENGINE_API	void	OpenPetItemMix( int nType );
	void				ClosePetItemMix();

// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );

// Network message functions ( receive )
	void	PetItemMixRep( SBYTE sbResult );
	
// Send ...
	void	SendPetItemMixReq( int nbtn, int nType );

// etc ...
	void	SelectItem ( int nIndex = -1 );
	void	AddString ( CTString& strText, COLOR colText = 0xffffffff );
	void	InitNeedItem( void );
};

#endif