#ifndef __CONFIG_LOCALIZE_H__
#define __CONFIG_LOCALIZE_H__
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////

// 국가별 로컬라이즈 파일로 관리

#if defined (LC_KOR)
#include "Config_Localize_KOR.h"

#elif defined(LC_GER) || defined (LC_FRC) || defined (LC_SPN) || defined (LC_PLD) || defined (LC_ITA) || defined (LC_UK)
#include "Config_Localize_gamigo.h"

#elif defined (LC_MEX) || defined (LC_BRZ)
#include "Config_Localize_BILA.h"

#elif defined (LC_RUS)
#include "Config_Localize_RUS.h"

#elif defined (LC_USA)
#include "Config_Localize_USA.h"

#elif defined (LC_TLD)
#include "Config_Localize_TLD.h"
#endif

// 이벤트는 요기서 찾으삼.
#include "Config_Localize_event.h"

/////////////////////////////////////////////////////////////////////////////////

// ---------------- 전체 공통 사용 아이템  ----------------- //

#define USE_GROCERY_ITEM1               2458    // 잡화상 이용 주문서 1일
#define USE_GROCERY_ITEM2               2459    // 잡화상 이용 주문서 7일
#define USE_GROCERY_ITEM3               2460    // 잡화상 이용 주문서 30일

#define USE_WAREHOUSE_ITEM1             2455    // 창고 이용 주문서 1일
#define USE_WAREHOUSE_ITEM2             2456    // 창고 이용 주문서 7일
#define USE_WAREHOUSE_ITEM3             2457    // 창고 이용 주문서 30일

#define SHOW_HP_ITEM1                   2461    // 써치 라이프 ( 캐릭터의 HP를 볼  있음 ) 1일
#define SHOW_HP_ITEM2                   2462    // 써치 라이프 ( 캐릭터의 HP를 볼  있음 ) 7일
#define SHOW_HP_ITEM3                   2463    // 써치 라이프 ( 캐릭터의 HP를 볼  있음 ) 30일

#define ONE_PERIOD_ITEM                 2404
#define SEVEN_PERIOD_ITEM               2405
#define THIRTY_PERIOD_ITEM              2406

#define EXPERIENCE_COMPETITION_ITEM     2407
#define PERIOD_COMPETITION_ITEM         2408
#define USE_COMPETITION_ITEM_NAS        1000

#define SKILL_POTION_ITEM               2452    // 숙련의 묘약
#define PLATINUM_SKILL_POTION_ITEM      2453    // 플래티늄 숙련의 묘약
#define PET_HPRECOVER_ITEM              2454    // 펫 HP 회복 물약
#define FIRECRACKER_ITEM                2403    //폭죽 아이템

#define LOW_RUNE_PROTECTION_ITEM        2394
#define HIGH_RUNE_PROTECTION_ITEM       2395
#define HIGH_RUNE_PROTECTION_ITEM_LEVLEL_LIMIT_1_60		5091
#define HIGH_RUNE_PROTECTION_ITEM_LEVLEL_LIMIT_61_90	5092
#define SUPER_HIGH_RUNE_PROTECTION_ITEM	6218
#define MIDDLE_HIGH_RUNE_PROTECTION_ITEM		11023

#define PET_RED_COLOR_ITEM              2396    // 빨간색
#define PET_SCARLET_COLOR_ITEM          2397    // 주홍
#define PET_YELLOW_COLOR_ITEM           2398    // 노랑
#define PET_GREEN_COLOR_ITEM            2399    // 초록
#define PET_BLUE_COLOR_ITEM             2400    // 파랑
#define PET_DEEP_BLUE_COLOR_ITEM        2401    // 남색
#define PET_VIOLET_COLOR_ITEM           2402    // 보라
#define PET_WHITE_COLOR_ITEM            2568    // 흰
#define PET_BLACK_COLOR_ITEM            2569    // 검정
#define PET_NON_COLOR_ITEM              2869    // 기본색
#define PET_BLACK_HEART_COLOR_ITEM      3548    // 검은말 하트
#define PET_WHITE_HEART_COLOR_ITEM      3549    // 하양말 하트
#define PET_YELLOW_HEART_COLOR_ITEM     3550    // 노랑말 하트
#define HORSE_COLOR_CHANGE_ITEM			740		// 말 색상문양 변경
#define DRAGON_COLOR_CHANGE_ITEM		741		// 용 색상문양 변경
#define HORSE_WILLIAM_COLOR_ITEM		6728	// 타는말 전용 껍데기 WILLIAM
#define HORSE_ALBER_COLOR_ITEM			6729	// 타는말 전용 껍데기 ALBER
#define HORSE_REDEYE_COLOR_ITEM			6730	// 타는말 전용 껍데기 REDEYE
#define HORSE_ZEBRA_COLOR_ITEM			6731	// 타는말 전용 껍데기 ZEBRA
#define HORSE_HAROLD_COLOR_ITEM			6732	// 타는말 전용 껍데기 HAROLD
#define HORSE_VIOLET_COLOR_ITEM			6733	// 타는말 전용 껍데기 VIOLET

// ---------------- 전로컬 적용 시스템 --------------------- //

// 쥬노개편으로 5,6이벤트 비트필드 모든 국가 공통으로 사용함
#define		ETC_EVENT_JUNO_RENEWAL_MESSAGEBOX_POPUP	(1 << 5)  // 팝업 체크
#define		ETC_EVENT_JUNO_RENEWAL_QUESTCOMPLETE	(1 << 6)  // 퀘스트 완료 체크

// 이벤트 자동화 시스템

// ----------------- 공통 define 설정 start -------------- //

// -------------------- 이벤트 설정 start -------------------- //

#define     NOTADULTMIN                 6
#define     NOTADULTMAX                 22
#define     NOTADULTNOTICE              21
#define     NOTADULTNOTICE_INTERVAL     10

#endif
