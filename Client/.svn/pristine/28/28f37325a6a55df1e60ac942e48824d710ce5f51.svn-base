//---------------------------------------------------------------------->>
// Name : LocalDefine.h
// Desc : for localization
// Date : [6/27/2006] , Wooss
//----------------------------------------------------------------------<<

#ifndef	LOCALDEFINE_H_
#define	LOCALDEFINE_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif

#include <map>

// #define WORLD_EDITOR
// #define HP_PERCENTAGE		// Hp 즉시 수치에서 퍼센테이지로 변경
#define	CRYPT_NET_MSG_LEVEL2		// 네트워크 패킷 2차 암호화 [trylord 11/09/29]
#define SEED_ENCIPHERMENT	// 블록 암호화

#if defined G_KOR
	#include "Define_KOR.h"
#endif

#if defined G_USA
	#include "Define_USA.h"
#endif

#if defined G_RUSSIA
	#include "Define_RUS.h"
#endif

#if defined G_GERMAN
	#include "Define_GAMIGO.h"
#endif

#if defined G_BRAZIL
	#include "Define_BRA.h"
#endif // G_BRAZIL

#if defined G_THAI
	#include "Define_THAI.h"
#endif

#include <Common/CommonDef.h>

/********************************************************************************/

// wooss 070228 ----------------------------------------------------->>
// 이벤트 툴의 event.lod데이터 값을 이용하여 이벤트를 적용한다.
// 추가되는 이벤트는 마지막 리스트에 서버 이벤트 인덱스에 맞게 적용 시킨다.
enum eEVENTALL
{
	TEVENT_START					=	0,	
	TEVENT_TREASUREBOX				=	1,	// 보물 상자 이벤트 
	TEVENT_TEACH					=	2,	// 후견인 이벤트 
	TEVENT_FRUIT					=	3,	// 과일 이벤트 
	TEVENT_CHUSEOK					=	4,	// 추석 이벤트 	
	TEVENT_SEPTEMBER				=	5,	// 9월 이벤트 
	TEVENT_2PAN4PAN1					=	6,	// 2판4판 이벤트 
	TEVENT_2PAN4PAN2					=	7,	// 2판4판 이벤트 
	TEVENT_PEPERO					=	8,	// 빼빼로 이벤트
	TEVENT_XMAS						=	9,	// 크리스마스 이벤트
	TEVENT_NEWYEAR					=	10, // 신년 이벤트
	TEVENT_CHANGE_ARMOR				=	11,	// 무기/방어구 교환 이벤트
	TEVENT_SEARCHFRIEND				=	12,	// 친구 찾기 이벤트
	TEVENT_VALENTINE_2006			=	13, // 발렌타인 이벤트
	TEVENT_WHITEDAY					=	14, // 화이트 데이 이벤트
	TEVENT_OLD_SERVER				=	15,	// 업데이트 기념 이벤트
	TEVENT_NEW_SERVER				=	16,	// 업데이트 기념 이벤트
	TEVENT_SAKURA					=	17, // 사쿠라 이벤트
	TEVENT_OX_QUIZ					=	18, // OX 퀴즈 이벤트
	TEVENT_WORLDCUP					=	19, // 월드컵 이벤트
	TEVENT_RAIN						=	20, // 빗방울 이벤트
	TEVENT_BUDDHIST					=	21,	// 태국 불교 이벤트
//	TEVENT_COLLECT_BUG				=	22,	// 곤충 채집 이벤트
	TEVENT_NEWSERVER_BASTARD		=	23,	// 신섭 바스타드 오픈 이벤트: 신섭
	TEVENT_NEWSERVER_BASTARD_OLD	=	24,	// 신섭 바스타드 오픈 이벤트: 구섭
	TEVENT_CHUSEOK_2006				=	25, // 추석 이벤트
	TEVENT_HALLOWEEN				=	26, // 할로윈 이벤트
	TEVENT_XMAS_2006					=   27,	// 2006 크리스마스 이벤트
	TEVENT_CHILDREN_DAY				=	28,	// 태국 Children Day 이벤트 후견인 이벤트
//	TEVENT_VALENTINE_2007			=	29, // 발렌타인데이 이벤트
	TEVENT_JPN_2007_NEWSERVER		=	30, // 일본 2007년 3월 신규 서버 오픈 접속자 이벤트
//	TEVENT_WHITEDAY2007				=	31, // 화이트 데이 2007
	TEVENT_SONGKRAN					=	32, // 태국 송크란 이벤트
	TEVENT_PARENTS_2007				=	34, // 어버이날 이벤트 	
	TEVENT_GOMDOLI				    =	35, // 곰돌이 이벤트
	TEVENT_CHILDREN_2007				=	36, // 어린이날 이벤트
	TEVENT_FLOWERTREE			    =	37, // 꽃놀이 이벤트 
	TEVENT_TEACHER_2007				=   38, // 스승의날 이벤트
	TEVENT_UCC_2007					=	39, // UCC 컨테스트 이벤트
	TEVENT_INDEPENDENCEDAY			=	40, // 미국 독립기념일 이벤트
	TEVENT_SUMMER_2007				=	41, // 2007 여름 종이접기 이벤트
	TEVENT_MOTHER					=	42, // 2007 태국 어머니날 이벤트
	TEVENT_ADULT_OPEN				=	44, // 성인 서버 오픈 이벤트('매직카드를 찾아라', '거침없이 쏜다!')
	TEVENT_BJMONO_2007				=	45, // SK BJ Mono 이벤트
	TEVENT_TG2007_1000DAYS			=	46, // 라스트 카오스 천일 이벤트
	TEVENT_TG2007_FRUITFULL			=	47, // 풍년 이벤트
	TEVENT_TG2007_SCREENSHOT			=	48, // 스크린샷 이벤트
//	TEVENT_HOLLOWEEN2007				=	49,	// 할로윈 이벤트(2007)
	TEVENT_XMAS_2007					=	50, // 크리스마스 이벤트(2007)
	TEVENT_NEWYEAR_2008				=	51, // 2008 신년 이벤트
	TEVENT_LUNARNEWYEAR_2008			=	52,	// 2008 설날 이벤트
	TEVENT_RED_TREASUREBOX			=	53, // 붉은색 보물상자 이벤트
	TEVENT_SAKURA_2008				=	54, // 2008 벚꽃 이벤트
	TEVENT_CHAOSBALL				=	55, // 카오스볼 이벤트
	TEVENT_NOMNOMNOM2008				=	56, // 놈놈놈 이벤트
	TEVENT_SUMMER_2008				=	57, // 2008 여름 이벤트
	TEVENT_PHOENIX					=	58, // 피닉스 이벤트
	TEVENT_HANARO					=	59, // 하나로 이벤트
	TEVENT_ADD_UP_AFRON_2009			=	60, // 블리스, 아프론 서버 통합 이벤트(아프론 버전 다운)
	TEVENT_QUARTER_FINAL_TOUR		=	61, // 라스트카오스 월드 토너먼트 8강 진출 이벤트
	TEVENT_GOLDWEEK					=	62,	// 일본 골드위크 이벤트
	TEVENT_WORLDCUP_2010				=	63, // ($E_WC2010) 2010 남아공 월드컵 Event 1 황금 축구공을 모아라
	TEVNET_WORLDCUP_2010_ATTENDANCE	=	67, // ($E_WC2010) 2010 남아공 월드컵 Event 3 매일 매일 응원하기
	TEVENT_OCEANS_PROMOTION			=	68, // 영화 '오션스' 제휴 이벤트(단발성 이벤트 용으로 사용) 100907 아칸사원 이벤트
	TEVENT_PROMOTION2				=	69,	// 쿠폰 등록 이벤트 : 유저당 한개의 쿠폰 사용 가능
	TEVENT_COMEBACK					=	70,	// 휴면 계정 보상 이벤트
	TEVENT_BIRTHDAY					=	71,	// 캐릭터 1주년 이벤트
	TEVENT_LOI_KRATHONG_TLD			=	72,	// 태국 크라통 이벤트
	TEVENT_TLD_KING_BIRTHDAY		=	73,	// 태국 국왕 탄생일 이벤트
	TEVNET_WORLDCUP_2010_TOTO		=	80,	// ($E_WC2010) 2010 남아공 월드컵 Event 2 우승국을 예상하라 응모
	TEVNET_WORLDCUP_2010_TOTO_STATUS =  81, // ($E_WC2010) 2010 남아공 월드컵 Event 2 우승국을 예상하라	현황
	TEVNET_WORLDCUP_2010_TOTO_GIFT	=	82, // ($E_WC2010) 2010 남아공 월드컵 Event 2 우승국을 예상하라 보상
	TEVENT_END								
};

//////////////////////////////////////////////////////////////////////////
// 기존 lod에서 읽어오는 방식에서 → 서버에서 알려주는 방식으로 변경. Event enum 재인덱싱.
//////////////////////////////////////////////////////////////////////////
typedef enum _eventAutomationList
{
	A_EVENT_COLLECT_BUG		= 22,	// 곤충 채집 이벤트
	A_EVENT_VALENTINE_DAY	= 29,	// 발렌타인 데이 이벤트
	A_EVENT_WHITE_DAY		= 31,	// 화이트 데이
	A_EVENT_SONGKRAN		= 32,	// 태국 송크란 이벤트
	A_EVENT_EGGS_HUNT		= 33,	// 부활절 이벤트
	A_EVENT_HOLLOWEEN		= 49,	// 할로윈 이벤트
	
};

extern ENGINE_API std::map<int,int> g_mapEvent;

// 이벤트가 설정되어 있다면 event value값을 리턴하고 아니면 FALSE(0)리턴...
#define IS_EVENT_ON(x) ((g_mapEvent.find(x)==g_mapEvent.end())?0:g_mapEvent[x])
// ------------------------------------------------------------------<<

// WSS_NPROTECT 070402 ------------------------------->>
#if defined( DEBUG ) || defined( _DEBUG ) || !defined ( USE_GAMEGUARD )
#define NO_GAMEGUARD
#endif
//#define NO_GAMEGUARD
// ---------------------------------------------------<<

// 디스어셈블러를 속이기 위한 간단한 코드 난독화 매크로
#if !defined(_DEBUG) && !defined(KALYDO) && !defined(G_KOR) && !defined(VER_TEST)
#define paste(a, b) a##b
#define pastesymbols(a, b) paste(a, b)
#define OBFUSCATE() \
	_asm { mov	eax, __LINE__ * 0x86f16351			}; \
	_asm { cmp	eax, __LINE__ * 0x6d489cb1			}; \
	_asm { je	pastesymbols(Junk, __LINE__)		}; \
	_asm { mov	eax, pastesymbols(After, __LINE__)	}; \
	_asm { jmp	pastesymbols(After, __LINE__)		}; \
	_asm { pastesymbols(Junk, __LINE__):			}; \
	_asm { _emit	(0xd8 + __LINE__ % 8)			}; \
	_asm { pastesymbols(After, __LINE__):			};
#else
#define OBFUSCATE() (void)0
#endif

#endif