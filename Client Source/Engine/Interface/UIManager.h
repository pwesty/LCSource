// ----------------------------------------------------------------------------
//  File : UIManager.h
//  Desc : Created by yjpark
// ----------------------------------------------------------------------------

#ifndef	UIMANAGER_H_
#define	UIMANAGER_H_

#include <Engine/DesignClasses/Singleton/SingletonBase.h>
#include <GameMP/Game.h>
#include <Engine/Interface/UIWindowDeclare.h>
#include <Engine/Interface/UIEditBox.h>
#include <Engine/Interface/UIMultiEditBox.h>
#include <Engine/Interface/UIFocus.h>
#include <Engine/Interface/UITooltipMgr.h>
#include <Engine/Interface/UIDrawFigure.h>

#include <Engine/Base/DamageLogInfo.h>

#include <Engine/Interface/UILacarette.h>


class CWorld;
class CUIListBox;

// Link library
#pragma comment( lib, "imm32.lib" )

// Message box command code
enum UI_MSGCMD
{
	MSGCMD_DROPITEM	= 0,					// 아이템 드롭
	MSGCMD_EXCH_REQ_SRC,					// 교환 요청
	MSGCMD_EXCH_REQ_DEST,					// 교환 요청
	MSGCMD_EXCH_DEL_ITEM,					// 교환창에서 아이템 삭제
	MSGCMD_WAREHOUSE_ADD_ITEM,				// 창고에 추가
	MSGCMD_WAREHOUSE_ADD_PLUSITEM,			// 창고에 추가
	MSGCMD_WAREHOUSE_ADD_MONEY,				// 창고에 돈 넣기.
	MSGCMD_WAREHOUSE_ADD_REQ,				// 보관 확인
	MSGCMD_WAREHOUSE_ERROR,					// 보관 오류.
	MSGCMD_WAREHOUSE_DEL_ITEM,				// 창고에서 인벤에 추가	
	MSGCMD_GUILD_JOIN,						// 길드 가입
	MSGCMD_GUILD_JOIN_REQ,					// 가입 신청중
	MSGCMD_GUILD_QUIT,						// 길드 탈퇴
	MSGCMD_GUILD_QUIT_CONFIRM,				// 길드 탈퇴 확인
	MSGCMD_GUILD_DESTROY,					// 길드 해체
	MSGCMD_GUILD_ERROR,						// 길드에서 에러 발생.
	MSGCMD_GUILD_ROOM,						// 길드 전용 공간으로 이동.
	MSGCMD_GUILD_APPLICANT_JOIN,			// 멤버 신청을 함.
	MSGCMD_GUILD_KICK,						// 멤버 퇴출
	MSGCMD_GUILD_ADD_VICE_BOSS,				// 부단장 임명
	MSGCMD_GUILD_DEL_VICE_BOSS,				// 부단장 퇴출
	MSGCMD_GUILD_CHANGE_BOSS,				// 단장 이임
	MSGCMD_GUILD_LORD_NOTICE,				// 성주 공지
	MSGCMD_GUILD_RECALL_USE_REQ,			// 길드 리콜 사용 여부
	MSGCMD_GUILD_RECALL,					// 길드 리콜 메시지 박스
	 
	// MSGCMD_GUILD_BATTLE_ // Date : 2005-03-19,   By Lee Ki-hwan
	MSGCMD_GUILD_BATTLE_ERROR,
	MSGCMD_GUILD_BATTLE_REQ,
	MSGCMD_GUILD_BATTLE_ACCEPT,
	MSGCMD_GUILD_BATTLE_STOP_REQ,
	MSGCMD_GUILD_BATTLE_STOP_ACCEPT,
	MSGCMD_GUILD_BATTLE_STOP_REQ_REP,
	MSGCMD_GUILD_BATTLE_MESSAGE,

	//	MSGCMD_BILLING // Date : 2005-05-06(오후 1:53:01), By Lee Ki-hwan
	MSGCMD_BILLING_NOT_ALLOW,				// 사용권한 없음(화면7)
	MSGCMD_BILLING_CONFIRM,
	MSGCMD_BILLING_CHARGE_CONFIRM,
	MSGCMD_BILLING_ERROR,
	MSGCMD_BILL_ITEM_REP,

	MSGCMD_SHOP_ADD_PLUSITEM,				// 상점창에서 업그레이드 아이템 추가
	MSGCMD_SHOP_PRICE,						// 상점창에 가격 입력.
	MSGCMD_SHOP_FAIL_ITEM,					// 상점창에서 판매할 수 없는 아이템의 경우
	MSGCMD_SHOP_START,						// 상점 시작.	
	MSGCMD_PERSONALSHOP_ERROR,				// 개인 상점 이름에 오류.
	MSGCMD_SYSMENU_EXIT,					// 게임 종료
	MSGCMD_SYSMENU_RESTART,					// 게임 재시작
	MSGCMD_PARTY_INVITE,					// 파티 초대
	MSGCMD_PARTY_ALLOW,						// 파티 수락
	MSGCMD_PARTY_MANDATE_REQ,				// 파티 위임 요청 
	MSGCMD_PARTY_MANDATE,					// 파티 위임
	MSGCMD_INVEN_UPGRADE_REQ,				// 아이템 업그레이드 요청
	MSGCMD_INVEN_UPGRADE_REP,				// 아이쳄 업그레이드 결과
	MSGCMD_ITEM_LEVELDOWN_REQ,				// 아이템 레벨 다운 요청
	MSGCMD_ITEM_LEVELDOWN_REP,				// 아이템 레벨 다운 결과
	MSGCMD_INVEN_OPTION_ADD_REQ,			// 아이템 옵션 추가 요청
	MSGCMD_INVEN_OPTION_ADD_REP,			// 아이템 옵션 추가 결과
	MSGCMD_INVEN_OPTION_DEL_REQ,			// 아이템 옵션 제거 요청
	MSGCMD_INVEN_OPTION_DEL_REP,			// 아이템 옵션 제거 결과
	MSGCMD_INVEN_DIVIDE,					// 아이템 나누기
	MSGCMD_REFINE_REP,						// 제련 결과
	MSGCMD_QUEST_GIVEUP,					// 퀘스트 포기
//	MSGCMD_QUEST_PRIZE,						// 퀘스트 보상
	MSGCMD_QUEST_NOTIFY,					// 퀘스트 에러, 확인
	MSGCMD_QUEST_NEWQUEST,					// 새로운 퀘스트 확인.
	MSGCMD_QUEST_FAIL,						// 퀘스트 실패(공주가 죽음.)
	MSGCMD_QUEST_COMPLETE,					// 퀘스트 완료 확인.
	MSGCMD_SKILLLEARN_NOTIFY,				// 스킬 습득 에러
	MSGCMD_REMISSION_NOTIFY,				// 면죄부 습득 에러
	MSGCMD_REMISSION_BUY,					// 면죄부 구입
	MSGCMD_QUEST_START,						// 퀘스트 시작
	MSGCMD_TELEPORT_MEMO,					// 메모리 스크롤 저장
	MSGCMD_LOGIN_ERROR,						// 로그인 에러
	MSGCMD_SERVER_ERROR,					// 서버 선택 부분에서 에러
	MSGCMD_SELCHAR_ERROR,					// 캐릭터 선택 부분에서 에러
	MSGCMD_CREATE_ERROR,					// 캐릭터 생성 부분에서 에러
	MSGCMD_DELETE_CHARACTER,				// 캐릭터 삭제
	MSGCMD_DELETE_CHARACTER_SECURITY,				// 캐릭터 삭제 보안 	
	MSGCMD_DELETE_CHARACTER_ERROR,			// 캐릭터 삭제 오류 
	MSGCMD_DISCONNECT,						// 접속끊김
	MSGCMD_CONNECT_ERROR,					// 접속 실패
	MSGCMD_WEBBOARD_ERROR,					// 웹에서 문제 생김
	MSGCMD_PROCESS_REP,						// 가공 결과
	MSGCMD_PROCESS_NOTIFY,					// 가공 에러, 확인
	MSGCMD_PRODUCT_REP,						// 제조 결과
	MSGCMD_PRODUCT_NOTIFY,					// 제조 에러, 확인
	MSGCMD_EVENT_PRIZE,						// 이벤트 보상 요청.
	MSGCMD_EVENT_RESULT,					// 이벤트 보상 결과.
	MSGCMD_WEAPONCHANGE_EVENT,				// 무기 교체 확인.
	MSGCMD_MIX_REP,							// 조합 결과
	MSGCMD_MIX_NOTIFY,						// 조합 에러, 확인
	MSGCMD_COMPOUND_REP,					// 합성 결과
	MSGCMD_CHANGEJOB,						// 전직 할꺼야???
	MSGCMD_INITJOB_NOTIFY,					// 임시
	MSGCMD_INITJOB_GIVEUP,					// 직업 포기
	MSGCMD_INITJOB_STAT,					// 스탯 환원
	MSGCMD_HELPER_NOTIFY,					//
	MSGCMD_HELPER_BEMYTEACHER,				// 내 선생이 되어달라고 요청.
	MSGCMD_HELPER_BEMYTEACHER_REQ,			// 내 선생이 되어달라고 요청 기다림.
	MSGCMD_HELPER_FIRE_MYTEACHER,			// 학생이 선생을 짜름.
	MSGCMD_HELPER_REGISTER,					// 선생으로 목록에 올림.
	MSGCMD_HELPER_CANCEL_REGISTER,			// 선생 목록에서 내림.
	MSGCMD_HELPER_STUDENT_ACCEPT,			// 후견인으로 등록 확인
	MSGCMD_SIEGE_WARFARE_ERROR,
	MSGCMD_SIEGE_WARFARE_INFO,				// 공성전- 공성정보 확인창
	MSGCMD_CHANGEWEAPON_REP,				// 무기 교체 확인
	MSGCMD_CHANGEWEAPON_NOTIFY,				// 무기 교체 메세지
	MSGCMD_PETTRAINING_NOTIFY,
	MSGCMD_PET_CHANGE,						// 탈 것으로 변환?	
	MSGCMD_PET_CHANGE_NOTIFY,				// 탈 것으로 변환?	
	MSGCMD_PET_DESTRUCTION,					// 펫 재료아이템 변환시 소멸 메세지		eons
	MSGCMD_PARTYAUTO_REGIST,				// 파티 오토 시스템 ( 파티 등록 )
	MSGCMD_PARTYAUTO_REGIST_ERROR,			// 파티 오토 시스템 ( 파티 등록 에러 )
	MSGCMD_PARTYAUTO_JOIN_ALLOW,			// 파티 오토 시스템의 조인 허락 및 거절
	
	// Date : 2005-05-23(오후 6:05:37), By Lee Ki-hwan
	MSGCMD_MESSENGER_START =  MSGCMD_CHANGEWEAPON_NOTIFY + 1, 
	MSGCMD_MESSENGER_ADD_REQ = MSGCMD_MESSENGER_START,	// 친구 등록 신청
	MSGCMD_MESSENGER_ADD_REP,				// 친구 등록 수락
	MSGCMD_MESSENGER_ADD_WAIT,				// 친구 등록 대기
	MSGCMD_MESSENGER_BLOCK_REQ,				// 친구 차단 요청
	MSGCMD_MESSENGER_BLOCK,					// 친구 차단
	MSGCMD_MESSENGER_UNBLOCK,					// 친구 해제
	MSGCMD_MESSENGER_DEL_REQ,				// 친구 삭제 요청
	MSGCMD_MESSENGER_DEL,					// 친구 삭제
	MSGCMD_MESSENGER_BLOCK_DEL_REQ,			// 차단 목록에서 삭제 요청
	MSGCMD_MESSENGER_BLOCK_DEL,

	MSGCMD_MESSENGER_GROUP_ADD,				// 그룹 추가
	MSGCMD_MESSENGER_GROUP_DEL,				// 그룹 삭제
	MSGCMD_MESSENGER_GROUP_RENAME,			// 그룹 이름 변경
	MSGCMD_MESSENGER_GROUP_MOVE,			// 그룹 변경

	MSGCMD_MESSENGER_ERROR,					// 에러 
	MSGCMD_MESSENGER_END = MSGCMD_MESSENGER_ERROR,  
	MSGCMD_PC_DEATH,						// 캐릭터 사망시 마을로 이동	
	MSGCMD_SUPERGOJE_REQUEST,				// 초고급 제련석 받고 싶어?
	MSGCMD_SUPERGOJE_NOTIFY,				// 초고급 제련석 관련 알림
	MSGCMD_OLD_PASSWORD,					// 이전 패스워드
	MSGCMD_NEW_PASSWORD,					// 새로운 패스워드
	MSGCMD_CONFIRM_PASSWORD,				// 암호 확인
	MSGCMD_SECURITY_NOTIFY,
	MSGCMD_SECURITY_PASSWORD,				// 암호 묻기

	MSGCMD_BOOSTER_UPGRADE_REQ,
	MSGCMD_BOOSTER_UPGRADE_REP,
	MSGCMD_SIEGE_WARFARE_MOVEING_CONFIRM,	// 공성 지역 이동 확인 	
	MSGCMD_BOOST_RANDON_PRODUCT_ERROR,		// 부스터 생산 오류
	
	// Guild Nast Stash // Date : 2005-09-06(오전 10:03:33), By Lee Ki-hwan
	MSGCMD_GUILDSTASH_TAKE_VIEW,	
	MSGCMD_GUILDSTASH_TAKE_STASH,	
	MSGCMD_GUILDSTASH_INFO,	
	MSGCMD_GUILDSTASH_ERROR,
	// Date : 2005-09-06(오전 10:03:41), By Lee Ki-hwan

	MSGCND_MOON_DAY_EVENT_GIVE_ITEM,		//2006 추석 이벤트: 오색송편을 보상품으로 교환
	MSGCMD_HARVEST_MOON_DAY_EVENT_UPGRADE1,	//2006 추석 이벤트: 송편을 만듦.
	MSGCMD_HARVEST_MOON_DAY_EVENT_UPGRADE2,	//2006 추석 이벤트: 오색송편을 만듦.
	
	// wooss 050802 add MSGCMD 
	MSGCMD_USE_CONFIRM_HEXP,				//사용후 죽었을 경우 주문서 사용여부를 확인한다 
	MSGCMD_USE_CONFIRM_AEXP,
	MSGCMD_USE_CONFIRM_HEXP_LUCKY,
	MSGCMD_USE_CONFIRM_AEXP_LUCKY,
	MSGCMD_USE_CONFIRM_REBIRTH,
	MSGCMD_USE_RECOVER_ITEM_HEXP,			// 경험치 복구 주문서를 사용한다
	MSGCMD_USE_RECOVER_ITEM_AEXP,			// 숙련치 복구 주문서
	MSGCMD_USE_RECOVER_ITEM_HEXP_LUCKY,
	MSGCMD_USE_RECOVER_ITEM_AEXP_LUCKY,
	MSGCMD_USE_REBIRTH_ITEM,				// 부활 주문서
	MSGCMD_USE_REBIRTH_ITEM_SCROLL2,		// rebirth scroll item [11/10/2009 rumist]
	
	// wooss 050803 
	MSGCMD_CHECK_ITEM_HEXP,					// inventory창에서 더블 클릭시  
	MSGCMD_CHECK_ITEM_AEXP,					// 다른 아이템 사용여부 체크
	MSGCMD_CHECK_ITEM_REBIRTH,				// 경험치복구 ,숙련도복구, 부활 주문서

	// wooss 050807
	MSGCMD_USE_WARP_ITEM,					// 순간이동 마법서를 사용한다
	MSGCMD_WARP_TO_CANCEL,					// 순간이동을 취소한다
	MSGCMD_WARP_TAKE_CANCEL,				// 소환을 취소한다 
	MSGCMD_WARP_TO_REQ_CALL,				// 순간이동 요청을 받았을 경우 sessionState에서 처리 
	MSGCMD_WARP_TAKE_REQ_CALL,				// 소환 요청을 받았을 경우 sessionState에서 처리

	// wooss 050817
	MSGCMD_USE_PC_SUMMON_ITEM,				// 소환 마법서를 사용한다
	// wooss 050818
	MSGCMD_USE_BOSS_SUMMON_ITEM,			// 보스 몬스터 소환 마법서
	// wooss 050812
	MSGCMD_CONFIRM_CHANGE_MY_NAME,			// 캐릭터 개명카드 사용여부 확인
	MSGCMD_CONFIRM_CHANGE_GUILD_NAME,		// 길드 개명카드 사용여부 확인
	MSGCMD_USE_CHANGE_MY_NAME_ITEM,			// 캐릭터 개명카드 사용
	MSGCMD_USE_CHANGE_GUILD_NAME_ITEM,		// 길드 개명카드 사용
	MSGCMD_BAN_NAME,						// 형식이 맞지않는 이름

	MSGCMD_CONFIRM_MEMSCROLL_EX,			// 메모리 스크롤 확장 사용확인
	MSGCMD_CONFIRM_WAREHOUSE_EX,			// 창고 확장 사용확인
	MSGCMD_PROLONG_MEMSCROLL_EX,			// 메모리 스크롤을 기한 연장
	MSGCMD_PROLONG_WAREHOUSE_EX,			// 확장 창고 기한 연장
	// wooss 050821
	MSGCMD_CONFIRM_SLOT_ITEM,				// 캐릭터 확장 슬롯 사용 확인 
	MSGCMD_USE_SLOT_ITEM,					// 캐릭터 확장 슬롯 사용

	MSGCMD_CONFIRM_PARTY_RECALL_ITEM,		// 파티리콜 아이템 사용 확인 060303 wooss
	
	MSGCMD_USE_ITEM,						// 단순 확인창을 거칠경우 UseSlotItem() 다시 보낼때	
	MSGCMD_SELECT_WORD_NOTIFY,				// 단어 선택 화면 

	MSGCMD_ASK_ONE_FIVE,					// 2pan4pan 이벤트 메시지 051031
	MSGCMD_ASK_ONE_FOUR,	
	MSGCMD_ASK_TWO_OPEN,

	MSGCMD_ASK_NEWYEAR1,					// 신년 이벤트1 꿈희망 이벤트 
	MSGCMD_ASK_NEWYEAR2,					// 신년 이벤트2 인내의 열매 이벤트

	MSGCMD_EVENT_COUPON_SENDNUM,			// 060118 wooss japan coupon event

	MSGLCMD_EVENT_CLOTHES_EXCHANGE,			// 일본 전통 의상 교환 이벤트
	
	MSGCMD_INPUT_FRIEND,					// 친구 찾기 이벤트 
	MSGCMD_TIME_FRIEND,	
	MSGCMD_REWARD_FRIEND,	
	
	MSGCMD_EX_PARTY_RECALL_PROMPT,			// 파티 리콜 ( 유료 아이템) // wooss 060306

	MSGCMD_CONFIRM_UPGRADESTONE_ITEM,		// wooss 060516 platinum item

	MSGCMD_CONFIRM_MIXNEWREQ_ITEM,				// wooss 060523 composite item 			
	MSGCMD_CONFIRM_MIXNEW_ITEM,				// wooss 060523 composite item 			
	MSGCMD_CONFIRM_UNMIX_ITEM,
	MSGCMD_PET_MOUNT_CANCEL_CARD,			// 펫 마운트 훈련 취소 카드 사용 확인 ( 유료 아이템 )
	MSGCMD_USE_PET_MOUNT_CANCEL_CARD,		// 펫 마운트 훈련 취소 카드 사용( 유료 아이템 )
 	
	MSGCMD_COMFIRM_USE_ITEM,				// (By eons) 같은 스킬의 아이템 사용시 확인 메세지 
	MSGCMD_WORLDCUP_EVENT1_NUM,				// 월드컵 이벤트
	MSGCMD_GOLDENBALL_EVENT,				// (By eons) 골든볼 이벤트

	MSGCMD_GAMBLE_READY,					// Cash MoonStone
	MSGCMD_GAMBLE_READYSEL,

	MSGCMD_SHOP_LEASE_ITEM,					// Lease Shop CMD
	
	MSGCMD_COLLECTBOX_BUY_REQ,					// 곤충 채집 상자 구입
	MSGCMD_INSECTITEM_DROP,						// 곤충 아이템 드롭

	MSGCMD_PROMO_KEY_ENTER,						//프로모 패키지 인증키 입력(말레샤)

	//Su-won
	MSGCMD_GIFT_SEND,
	MSGCMD_GIFT_RECV,
	MSGCMD_GIFT_ERROR,
	MSGCMD_RARE_IDENTIFY,			// 장비 감정 아이템 사용
	MSGCMD_USE_RARE_IDENTIFY,		// 장비 감정 아이템 사용 확인

	MSGCMD_USE_PACKING_PAPER,		// 포장지 아이템 사용
	MSGCMD_SAVE_BINGOITEM,			// 빙고 아이템 저장 확인
	MSGCMD_OK_EXCHANGE_CHOCOBOX,	// 초코상자 교환 확인

	MSGCMD_GOTO_EGEHA,

	// wooss 070305 ----------------------------------------------------->>
	// kw : WSS_WHITEDAY_2007
	MSGCMD_WHITEDAY_BLUELETTER,			// 화이트데이 편지 아이템 사용
	MSGCMD_WHITEDAY_PINKLETTER,
	// ------------------------------------------------------------------<<

// [KH_070326] 달걀 이벤트 관련
	MSGCMD_CONFIRM_EASTER_EGGS,
	
	MSGCMD_SONGKRAN_REWARD,				//태국 송크란 이벤트 금,은그릇 사용
	//WSS_GUILD_MASTER 070411 ----------------------->>
	MSGCMD_GUILDMASTER_1,				// 길드 포인트 윈도우
	MSGCMD_GUILDMASTER_1_1,				// 길드 포인트 등록
	MSGCMD_GUILDMASTER_1_2,				// 길드 포인트 갱신
	MSGCMD_GUILDMASTER_2_1,				// 교환권 받기 
	// ----------------------------------------------<<
	MSGCMD_MINIGAME_SELECT_ITEM,		// WSS_MINIGAME 070420 엽기 무기 선택

	MSGCMD_FLOWERTREE_SUPPORT,			// 꽃놀이 이벤트 개화돕기
	MSGCMD_FLOWERTREE_TICKET,			// 꽃놀이 이벤트 교환권 받기
	MSGCMD_FLOWERTREE_TICKETEX,			// 꽃놀이 이벤트 교환권 받기 확인

	MSGCMD_PET_NAMECARD_USE,			// 펫의 명찰: 아이템 사용
	MSGCMD_PET_NAMECARD_INPUT,			// 펫의 명찰: 이름 입력

	// [070702 :Su-won] SUMMER_2007
	MSGCMD_SUMMER2007_PAPERFOLDING,		//종이접기
	MSGCMD_SUMMER2007_PAPERBLENDING,	//색종이 조합

	// [070807: Su-won] EVENT_ADULT_OPEN
	MSGCMD_ADULTEVENT_ALCHEMIST,		// 성인서버 오픈이벤트(장비조합 연금술 이벤트): 장비 조합
	MSGCMD_EVENT_SHOOT,  // 성인서버 오픈이벤트(성인들만의 특권): 참여 확인창
	
	MSGCMD_SIEGE_WARFARE_BUY_QUARTER,	// WSS_DRATAN_SEIGEWARFARE 2007/08/13
	MSGCMD_SIEGE_WARFARE_WAIT_QUARTER,	// WSS_DRATAN_SEIGEWARFARE 2007/08/13
	MSGCMD_SIEGEWARFARE_REBIRTH,		// WSS_DRATAN_SEIGEWARFARE 2007/08/22

	MSGCMD_BJMONO_2007_CHANGE_REQ,		// SK BJ MONO 응모권 교환 요청 메세지 박스
	MSGCMD_TG2007_1000DAYS_CAP_UPGRADE,	// 기념모자 업그레이드 메세지 박스

	// [070824: Su-won] REFINE_PROTECT
	MSGCMD_INVEN_UPGRADE_PROTECT_REQ,	// 제련 보호 아이템 사용

	// [070824: Su-won] PET_COLOR_CHANGE
	MSGCMD_PET_COLORCHANGE_USE,			// 펫 색상 변경 아이템 사용
	// [090713: selo] PET_TATOO_CHANGE
	MSGCMD_PET_TATOOCHANGE_USE,			// 펫 색상 및및 문양 변경 아이템 사용

	// [071122: Su-won] DRATAN_SIEGE_DUNGEON
	MSGCMD_SIEGE_DUNGEON_STATE,						//던전 상태창
	MSGCMD_SIEGE_DUNGEON_ENVIRONMENT_CONTROL,		//던전 내부 환경 제어창
	MSGCMD_SIEGE_DUNGEON_MONSTER_CONTROL,			//던전 내부 몬스터 제어창
	MSGCMD_SIEGE_DUNGEON_ADMISSION_FEE_CONTROL,		//던전 입장료 설정창
	MSGCMD_SIEGE_DUNGEON_HUNTING_FEE_CONTROL,		//던전 수렵세율 설정창

	MSGCMD_SIEGE_DUNGEON_ENVIRONMENT_CHANGE,		//던전 내부 환경 변경 확인창
	MSGCMD_SIEGE_DUNGEON_MONSTER_CHANGE,			//던전 내부 몬스터 변경 확인창
	MSGCMD_SIEGE_DUNGEON_ADMISSION_FEE_CHANGE,		//던전 입장료 변경 확인창
	MSGCMD_SIEGE_DUNGEON_HUNTING_FEE_CHANGE,		//던전 수렵세율 변경 확인창
	MSGCMD_SIEGE_DUNGEON_ENTER,						//던전 이동 확인창

	// [071122: Su-won] NEW_MOONSTONE
	MSGCMD_MOONSTONE_MIX,
	
	MSGCMD_EVENT_XMAS_2007_DECO,					// 크리스마스 트리 확인창
	MSGCMD_PETTRANSFORM_RU,							// 펫 루돌프 변신 주문서 확인창

	MSGCMD_EVENT_NEWYEAR_2008,						// 2008 신년이벤트, 버프 수락 확인창

	MSGCMD_USE_SMALL_CORD,							// 복주머니 노끈 사용
	MSGCMD_OK_EXCHANGE_LUCKYBOX,					// 소망상자 교환 확인

	MSGCMD_EVENT_SAKURA_2008,						// 2008년 벚꽃 이벤트: 보상교환 확인창

	MSGCMD_EVENT_CHAOSBALL,							//카오스볼 이벤트: 카오스볼 봉인해제 창
	
	MSGCMD_PRESSCORPS,

	MSGCMD_ITEMDELETE,								// 인벤토리 휴지통 메시지 처리

	MSGCMD_PARTY_OPTION,							//파티 옵션(종류 변경) 창

	MSGCMD_EVENT_PHOENIX,							//피닉스 이벤트
	///////////////////////////////////////////////////////////////////////////////////	
	// Cube
	MSGCMD_CUBE_PARTY_GROUP_ENTRANCE,						//Cube: 파티 입장
	MSGCMD_CUBE_PARTY_PERSON_ENTRANCE,							//Cube: 파티 개인 입장
	MSGCMD_CUBE_GUILD_GROUP_ENTRANCE,						//Cube: 길드 입장
	MSGCMD_CUBE_GUILD_PERSON_ENTRANCE,							//Cube: 길드 개인 입장
	MSGCMD_CUBE_STATE,											//Cube: 파티 및 길드 큐브 현황
	MSGCMD_CUBE_CHANGE_RATE,											//Cube: 세율 변경
	// Cube
	///////////////////////////////////////////////////////////////////////////////////	
	MSGCMD_INITJOB_SSKILL,							// 스페셜 스킬 초기화 확인창
	
	MSGCMD_AUCTION_BUYITEM,
	MSGCMD_AUCTION_FAVORITE,
	MSGCMD_AUCTION_FAVORITE_CANCEL,
	MSGCMD_AUCTION_CANCELREGISTER,
	MSGCMD_AUCTION_INFO,
	
	MSGCMD_CONFIRM_OBJECTCLICK,						// 오브젝트 클릭 확인

	MSGCMD_ITEM_BELONG,

	// [sora] 원정대
	MSGCMD_EXPEDITION_INVITE,						//원정대 초대
	MSGCMD_EXPEDITION_INVITE_BY_NAME,
	MSGCMD_EXPEDITION_ALLOW,						//원정대 참여 여부
	MSGCMD_EXPEDITION_END,
	MSGCMD_EXPEDITION_QUIT,
	MSGCMD_EXPEDITION_SET_BOSS,
	MSGCMD_EXPEDITION_SET_MBOSS,
	MSGCMD_EXPEDITION_RESET_MBOSS,
	MSGCMD_EXPEDITION_KICK,
	MSGCMD_EXPEDITION_GO_STARTPLACE,				// 부활위치로 이동
	MSGCMD_EXPEDITION_ERROR,
	MSGCMD_EXPEDITION_COLLECTITEM,
	//원정대 에러 메시지
	// [6/3/2009 rumist] define NPC Affinity System Message type.
	MSGCMD_AFFINITY_CONTRACT,
	MSGCMD_AFFINITY_TAKEPRESENT,

	MSGCMD_DIVISION_INFO,							// [sora] 파티, 원정대 분배방식 정보표시

//[ttos_2009_7_17]: CHARATER_CHAT_FILTER 캐릭터 채팅 필터
	MSGCMD_CHAT_BLOCK_REQ,
	MSGCMD_CHAT_BLOCK,
	MSGCMD_CHAT_ERROR,

	// [sora 090824 제작 관련]
	MSGCMD_SEAL_MEMBER,					// 제작 협회 등록
	MSGCMD_LEARN_SEAL,					// 제작 증표 획득
	
	MSGCMD_PET_EVOLUTION,				// 펫 진화

	MSGCMD_RAID_INFO,					// 귀속된 레이드 존 정보
	MSGCMD_NICKNAME_DELETE,

	MSGCMD_QUEST_RESTORE_ITEM,			// [100208: selo] 퀘스트 아이템 다시 받기
	MSGCMD_QUEST_RESTORE_ITEM_RESULT,	// [100208: selo] 퀘스트 아이템 다시 받기 결과

	// [100322: selo] 친화도 보상 알람
	MSGCMD_AFFINITY_REWARD_NOTICE,

	MSGCMD_LACARETTE_GET_TOKEN,			// [100325 : sora]라카렛 토큰 받기
	MSGCMD_LACARETTE_ERROR,				// [2010/04/16 : Sora] 라카렛 메시지 창
	MSGCMD_LACARETTE_REMIND,			// [2010/04/21 : Sora] 라카렛 사용확인 메시지
	
	MSGCMD_EVENT_WORLDCUP_EVENT2_REREQ,	// ($E_WC2010) [100517: selo] 2010 월드컵 이벤트2 우승 국가 재응모
	
	MSGCMD_EVENT_WORLDCUP_EVENT2_REQ,	// ($E_WC2010) [100514: selo] 2010 월드컵 이벤트2 우승 국가 응모
	MSGCMD_EVENT_WORLDCUP_EVENT2_STATUS,// ($E_WC2010) [100514: selo] 2010 월드컵 이벤트2 응모 현황

	MSGCMD_EVENT_WORLDCUP_EVENT3,		// ($E_WC2010) [100518: selo] 2010 월드컵 이벤트3 응답 메시지 박스
	
	MSGCMD_SOCKETSYSTEM_CHANCECARD,		// socket system : one more chance card [5/18/2010 rumist]
	MSGCMD_QUEST_TUTORIAL_START,		// tutorial renewal. [8/2/2010 rumist]
	MSGCMD_QUEST_TUTORIAL_CANCEL,

	MSGCMD_HELPER_PLAYTIME,				// [8/31/2010 kiny8216] 후견인 시스템 개편 : 주이용시간
	MSGCMD_HELPER_FIRE_MYSTUDENT,		// [8/31/2010 kiny8216] 후견인 시스템 개편 : 후견인 포기

	MSGCMD_SOULOFSCALP_CONFIRM,			// 영혼의 징표 교환 확인 창
	MSGCMD_REGISTER_MERCHANT,			// [2010/08/25 : Sora] ADD_SUBJOB
	MSGCMD_COMBO_ERROR,					// [2010/10/06 : Sora] 몬스터 콤보 에러 메시지 중복 표시 수정

	MSGCMD_PROMOTION2_REQ,				// [10/6/2010 kiny8216] 쿠폰 프로모션 이벤트 : 쿠폰 이벤트 상품 요청
	
	MSGCMD_LOGINSERV_BLOCK_USER,		// [10/27/2010 kiny8216] 브라질 패스워드 3번 틀려서 블록
	MSGCMD_MONSTER_MERCENARY_CARD_USE,		// [2010/10/20 : Sora] 몬스터 용병 카드
	MSGCMD_MONSTER_MERCENARY_CARD_HOLDER_USE,
	MSGCMD_MONSTER_MERCENARY_CARD_ERROR,
	MSGCMD_FACECHANGE,					// 외형 변경 모드
	MSGCMD_FACECHANGE_OK,				// 외형 변경 요청
	
	MSGCMD_CASH_EX_ERROR,			// 소망상자 이용중 에러

	MSGCMD_PET_STAT_INIT_CARD_USE,		// pet status initialize card [1/19/2011 rumist]

	MSGCMD_ATTENDANCE,				// [2011/01/18 : Sora] 출석 이벤트	
	MSGCMD_LACABALL_ERROR,				// laca ball error. [1/27/2011 rumist]
	
	MSGCMD_COMEBACK_CONFIRM,		// 휴면 계정 보상 이벤트 선물 받기
	MSGCMD_BIRTHDAY_CONFIRM,		// 생일 선물 받기
	MSGCMD_CHANGE_START_POINT,		// 독일 요청으로 주노 스타트 포인트 변경 [4/6/2011 ldy1978220]
	MSGCMD_ROYALRUMBLE_JOIN_REQ,		// royal rumble game 신청 [4/26/2011 rumist]
	MSGCMD_ROYALRUMBLE_REJECT_REQ,		// royal rumble game 신청 취소 
	MSGCMD_ROYALRUMBLE_GO_ZONE,			// go to royal rumble zone.
	MSGCMD_ROYALRUMBLE_WINNER_OUT,		// go to start zone.

	MSGCMD_GUILD_MARK_CONFIRM,			// [sora] GUILD_MARK
	MSGCMD_GUILD_MARK_REGISTER,			// [sora] GUILD_MARK
	MSGCMD_KB_EXCHAGE_HEART,			// [2011/11/14 : Sora] 태국 국왕 탄생일 이벤트
	MSGCMD_KB_EXCHAGE_FLAG,				// [2011/11/14 : Sora] 태국 국왕 탄생일 이벤트

	MSGCMD_USERNOTICE,					// [11/17/11 trylord] 유저 공지 시스템

	MSGCMD_SHUTDOWN_TIME,				// [2011/12/07 : Sora] 셧다운 메시지

	MSGCMD_GUILDMASTER_KICK_REQUEST,		// 길드장 해임 심사 신청 확인 [trylord 111228]
	MSGCMD_GUILDMASTER_KICK_CANCEL_REQUEST,	// 길드장 해임 이의 신청 확인 [trylord 111228]
	MSGCMD_GUILDREMOTE_JOIN_TOMASTER,		// 길드원격 - 길마 신청받음
	MSGCMD_GUILDREMOTE_JOIN_TOUSER,			// 길드원격 - 유저가 신청받음

	MSGCMD_USE_PET_ACCUMULATE,			// 축적펫 메시지 박스
	MSGCMD_USE_PET_ACCUMULATE2,
	MSGCMD_SENDMAIL_COUNT_INPUT,		// [2012/01/18] 메일 보내기 아이템 카운트 세팅
	MSGCMD_SENDMAIL_COUNT_ADD,			// [2012/01/18] 메일 보내기 아이템 카운트 더하기
	MSGCMD_SENDMAIL_MONEY_INPUT,		// [2012/01/26] 메일 보내기 돈 입력 메시지 박스
	MSGCMD_JUMPIMGPOTION,				// 독일 요청 점핑 포션 사용시
	MSGCMD_DELETMAIL_CHECK,				// 메일 삭제 재 확인

	MSGCMD_CHAR_SLOT_EXTEND,			// [2012/07/05 : Sora]  캐릭터 슬롯 확장 아이템
	//MSGCMD_CHAR_SERVER_MOVE,			// 캐릭터 서버 이전 아이템 [7/25/2012 Ranma]

	MSGCMD_NEWGUILDSTASH_ILLEGAL_USER,	// 부정 사용자. 해킹 사용 가능성 유저. [7/17/2012 rumist]

	MSGCMD_CHARMOVETOANOTHERSERVER_SUCCESS, // ENABLE_CHARACTER_MOVE_TO_OTHER_SERVER_ITEM // 성공시 messagebox 보여주기 (타이머 포함) [7/25/2012 Ranma]
	// wooss 050806
	MSGCMD_FORTUNE,						// [2013/02/13] sykim70 Fortune System
	MSGCMD_MYSTERIOUS_BEAD_SEND_ITEM,	// 신비한 석상 구슬 아이템 사용후 EXPRESS로 아이템 전송시 사용 MSG
	MSGCMD_WAREHOUSE_TAKE_MONEY,			//2013/04/04 jeil 나스 아이템 제거 
	MSGCMD_EXCH_ADD_NAS,				//2013/04/09 jeil 나스 아이템 제거 
	MSGCMD_NULL,						// 단순 확인 메시지박스 처리 
	MSGCMD_JEWEL_COMPOS_JEWEL_COMPOS_SCROLL,
	MSGCMD_EXPRESS_RECV,
	MSGCMD_EXPRESS_RECVALL,
	MSGCMD_EXPRESS_DELETE,
	MSGCMD_EXPRESS_DELETEALL,
	MSGCMD_EXPRESS_REMOTE_ITEMUSE,
	MSGCMD_EXPRESS_REMOTE_ERROR,
	MSGCMD_PET_STASH_INSTASH,
	MSGCMD_PET_STASH_OUTSTASH,
	MSGCMD_PET_STASH_ERROR,
	MSGCMD_PET_STASH_ERROR_SEALED,
	MSGCMD_PET_STASH_USEITEM,
	MSGCMD_TEST_UIBASE,
	MSGCMD_ITEM_DUPLICATION_CHECK,
	MSGCMD_INVENTORY_ARRANGE,
	MSGCMD_USE_SEALED_SOUL,
	MSGCMD_SYNDICATE_PUTON_JEWEL_REQ,
	MSGCMD_SYNDICATE_SECESSION_REQ,
#ifdef DURABILITY
	MSGCMD_DURABILITY_MATERIAL_COUNT,
	MSGCMD_DURABILITY_REPAIR,
	MSGCMD_DURABILITY_SPECIAL_REPAIR,
	MSGCMD_DURABILITY_RECOVER,
#endif	//	DURABILITY
	MSGCMD_EXP_PET_COOLTIME_REMOVE,
	MSGCMD_ATTENDANCE_ASSURE_OK_UPD,
	MSGCMD_ATTENDANCE_ITEM_USE,
#ifdef PREMIUM_CHAR	
	MSGCMD_PREMIUM_CHAR_ITEM_USE,
#endif	//	PREMIUM_CHAR
	MSGCMD_LOGOUT_RELIC_WARRING,
	MSGCMD_TOTAL
};

enum UI_MSGLCMD
{
	MSGLCMD_REFINE_REQ = 0,					// 제련 요청창
	MSGLCMD_PROCESS_REQ,					// 가공 요청창
	MSGLCMD_PRODUCT_REQ,					// 제조 요청창
	MSGLCMD_MIX_REQ,						// 조합(재활의 상자) 요청창
	MSGLCMD_COMPOUND_REQ,					// 합성(비밀의 상자) 요청창
	MSGLCMD_QUEST_INTRO,					// 퀘스트 소개창
	MSGLCMD_QUEST_PRIZE,					// 퀘스트 보상창.
// FIXME : 튜토리얼만 특이해서... 맘에 안듬.
	MSGLCMD_QUEST_TUTORIAL,					// 튜토리얼 퀘스트
	MSGLCMD_SHOP_REQ,						// SHOP REQUEST		Insert By cpp2angle (04.12.18)
	MSGLCMD_GUILD_REQ,						// 길드
	MSGLCMD_GUILD_BATTLE_REQ = 10,				// 길드 전투 // Date : 2005-03-17,   By Lee Ki-hwan
	MSGLCMD_WAREHOUSE_REQ,					// 창고...
	MSGLCMD_SECURITY_REQ,					// 보안...
	MSGLCMD_SKILLLEARN_REQ,					// 스킬 습득창
	MSGLCMD_SSKILLLEARN_REQ,				// 특수 스킬 습득창
	MSGLCMD_CHANGEJOB_REQ,					// 전직창...
	MSGLCMD_REMISSION_REQ,					// 면죄부
	MSGLCMD_QUEST_REQ,						// 퀘스트.
	MSGLCMD_EVENT,							// 이벤트 선택창.
	MSGLCMD_HARVEST_MOON_DAY1,				// 추석맞이 이벤트 
	MSGLCMD_HARVEST_MOON_DAY2 = 20,				// 추석맞이 이벤트 
	MSGLCMD_HELPER_REQ,						// 후견인 시스템.
	MSGLCMD_INITJOB_REQ,					// 임시
	MSGLCMD_CHANGEWEAPON_REQ,				// 무기 교체
	MSGLCMD_STORY_INTRO,					// 싱글던젼 내용 소개
	MSGLCMD_CHANGEWEAPON_EVENT_REQ,			// 무기 교체 이벤트	
	MSGLCMD_GATE_OF_DIMENSION,				// 퍼스널 던젼 4 - 차원의 문
	MSGLCMD_EVENT_NOTICE,					// 이벤트 알림
	MSGLCMD_TREASUREBOX_EVENT,				// 보물 상자 이벤트.
	MSGLCMD_SIEGE_WARFARE,					// 공성전 - 기본
	MSGLCMD_MERCEAR_REQ = 30,	
	MSGLCMD_PROCESSNPC_REQ,					// 가공 NPC
	MSGLCMD_PROCESSNPC_MINERAL_REQ,			// 광석 정련
	MSGLCMD_PROCESSNPC_ENERGY_REQ,			// 원소 정제
	MSGLCMD_PROCESSNPC_CROP_REQ,			// 식물 가공.
	MSGLCMD_PETTRAINING_REQ,				// 애완동물 훈련.
	MSGLCMD_PETCHANGE_REQ,					// 애완동물 변형
	MSGLCMD_PETSKILLINIT_REQ,				// 애완동물 스킬 초기화
	MSGLCMD_PETSTUFFCHANGE_REQ,				// 애완동물을 재료 아이템으로 변환  // eons
	MSGLCMD_PETUNIQUEPRODUCT_REQ,			// 유니크 아이템 제작		// eons
	MSGLCMD_GW_MIX_REQ = 40,						// 공성 아이템 조합 
	MSGLCMD_PARTYAUTOMATCH_REQ,				// 파티 오토 매칭 시스템	// eons
	MSGCMD_GW_MIX_REP,
	MSGLCMD_GUILDSTASH_REQ,
	MSGLCMD_RENUAL_EVENT,
	MSGLCMD_EVENT_2PAN4PAN_ONE,				// 이판사판 이벤트 머니 교환 wooss 051031
	MSGLCMD_EVENT_2PAN4PAN_TWO,				// 이판사판 이벤트 보물상자 열기
	MSGCMD_EVENT_2PAN4PAN_THREE,			// 이판사판 이벤트 캐릭터보상 카드
	
	MSGLCMD_EVENT_NEWYEAR1,					// 신년이벤트 꿈과 희망
	MSGLCMD_EVENT_NEWYEAR2,					// 신년이벤트 인내의 열매
	MSGLCMD_EVENT_FIND_FRIEND = 50,				// 친구 찾기 이벤트
	MSGLCMD_CLOTHES_EXCHANGE,				// 일본 전통의상 이벤트
	MSGLCMD_EVENT_OXQUIZ,					// O.X이벤트
	MSGLCMD_PLATINUM_STONE_EXCHANGE,		// 플래티늄 제련석 교환
	MSGCMD_ASK_PLATINUM_EXCHANGE1,			// 플래티늄 제련석 교환 확인
	MSGCMD_ASK_PLATINUM_EXCHANGE2,
	MSGCMD_ASK_PLATINUM_EXCHANGE3,
	MSGLCMD_RAINYDAY,						// 일본 빗방울 이벤트
	MSGLCMD_MOTHERDAY,						// 태국 어머니날 이벤트
	MSGCMD_WORLDCUP_EVENT1,			// 월드컵 이벤트 1 (우승국)
	MSGCMD_WORLDCUP_EVENT1_EXCHANGE,
	MSGCMD_WORLDCUP_EVENT1_RECEIVE,

	MSGCMD_WORLDCUP_GOLDENBALL,				// 골든볼 이벤트
	MSGCMD_WORLDCUP_GOLDENBALL_ENTRY,		// 골든볼 이벤트 응모
	MSGCMD_WORLDCUP_GOLDENBALL_REQUITAL,	// 골든 볼 이벤트 보상
	
	MSGLCMD_BUDDHISM_EVENT,					// 불교 촛불 축제 이벤트( 태국 )
	MSGLCMD_COLLECT_INSECT_EVENT,				// 여름 곤충 채집 이벤트
	MSGLCMD_COLLECT_BOX_GIFT,				// 곤충 채집상자 보상
	MSGLCMD_EVENT_PROMOPACK,				// 프로모 패키지 이벤트 (말레샤)

	MSGLCMD_EVENT_XMAS_2006,				// 2006 XMas Event [12/11/2006 Theodoric]
	MSGLCMD_EVENT_XMAS_2006_COUNT,			// 2006 XMas Event [12/11/2006 Theodoric]
	MSGLCMD_EVENT_XMAS_2006_CHANGE,			// 2006 XMas Event [12/11/2006 Theodoric]

	MSGLCMD_EVENT_LOVE_LOVE,				// 러브러브 이벤트

											// wooss 070305 kw: WSS_WHITEDAY_2007
	MSGLCMD_WHITEDAY_2007,					// 2007 WhiteDay Event
	
	MSGLCMD_EVENT_MAY,						// 5월 가정의달 이벤트
	
	MSGLCMD_GUILDMASTER,					// WSS_GUILD_MASTER 070411
	MSGLCMD_GUILDMASTER_2,
	
	MSGLCMD_EVENT_MINIGAME,					// WSS_MINIGAME 070420	

	MSGLCMD_SUMMER_2007,					// [070702 :Su-won] SUMMER_2007
	MSGLCMD_RESERVE_REQ,					// 예비 (악덕 상인, 어둠의 스킬 마스터 사용)	
	MSGLCMD_SIEGE_WARFARE_UPGRADE,			// WSS_DRATAN_SIEGEWARFARE 070730
	
	////////////////////////////////////////////////////////////////////////////////////////////////
	// [070807: Su-won] EVENT_ADULT_OPEN
	MSGLCMD_EVENT_ADULT_MAGICCARD,			// 성인서버 오픈 이벤트(매직카드를 찾아라!)
	MSGLCMD_EVENT_ADULT_CHANGEJOB,			// 성인서버 오픈 이벤트(전직달성!절망의 부적을 받아라!)
	MSGLCMD_EVENT_ADULT_ALCHEMIST,			// 성인서버 오픈 이벤트(장비조합 연금술 이벤트)
	// [070807: Su-won] EVENT_ADULT_OPEN
	////////////////////////////////////////////////////////////////////////////////////////////////
	
	MSGLCMD_TG2007_FRUITFULL,				// WSS_TG2007 2007/09/14
	
	// [071122: Su-won] DRATAN_SIEGE_DUNGEON
	MSGLCMD_DRATAN_SIEGE_DUNGEON_REQ,
	MSGLCMD_DRATAN_SIEGE_DUNGEON_CONTROL,

	MSGLCMD_RED_TREASUREBOX_EVENT,
	MSGCMD_EX_MONSTERCOMBO_GOTO_COMBO_PROMPT, // 몬스터 콤보 대기방 이동
	MSGCMD_EX_MONSTERCOMBO_RECALL_TO_COMBO_PROMPT, // 대기방에서 파티 소환
	MSGCMD_COMBO_GIVEUP,						// 몬스터 콤보 포기

	MSGLCMD_CHAOSBALL_EVENT,				//카오스볼 이벤트	

	//Cube
	MSGLCMD_CUBE_PARTY_ENTRANCE,		//파티입장 선택창
	MSGLCMD_CUBE_GUILD_ENTRANCE,		//길드입장 선택창

	MSGLCMD_RAID_CONTINUED_MESSAGE,		// 레이드 시스템 진행 메세지
	//MakeItem
	MSGLCMD_PRODUCETNPC_VOUCHER,		// 제작 아이템 증표
	MSGLCMD_PRODUCETNPC_LEARN,			// 제작 아이템 기술 습

	// [6/3/2009 rumist] define NPC Affinity System Request Message Type.
	MSGLCMD_AFFINITY_DONATE_REQ,		// request to donation view.

	// connie [2009/9/18] - 
	MSGLCMD_NPC_SCROLL_MESSAGE,

	// tutorial open. [8/2/2010 rumist]
	MSGLCMD_QUEST_TUTORIAL_OPEN,
	
	MSGLCMD_TUTORIAL_MESSAGE1,
	MSGLCMD_TUTORIAL_MESSAGE2,
	MSGLCMD_TUTORIAL_MESSAGE3,
	MSGLCMD_TUTORIAL_MESSAGE4,
	MSGLCMD_TUTORIAL_MESSAGE5,
	MSGLCMD_TUTORIAL_MESSAGE6,
	// add tutorial message box.
	MSGLCMD_TUTORIAL_MESSAGE7,
	
	MSGLCMD_AUCTION,
	
	// [100222: selo] 연속 다이얼로그 트리거 
	MSGLCMD_CONSECUTIVE_DIALOGUE,
	MSGLCMD_SOCKET_SYSTEM,
	MSGLCMD_SOCKET_SYSTEM_EMPTY,
	MSGLCMD_SOCKET_SYSTEM_EMPTY_BOX,
	MSGLCMD_SOCKET_SYSTEM_JEWEL_COMPOS,
	MSGLCMD_LACARETTE_GET_TOKEN,		// 라카렛 토큰 획득

	MSGLCMD_EVENT_WORLDCUP_EVENT1,		// ($E_WC2010) [100511: selo] 2010 월드컵 이벤트1
	MSGLCMD_EVENT_WORLDCUP_EVENT2,		// ($E_WC2010) [100514: selo] 2010 월드컵 이벤트2
	MSGLCMD_EVENT_WORLDCUP_EVENT3,		// ($E_WC2010) [100514: selo] 2010 월드컵 이벤트3
	
	MSGLCMD_ATTENDANCE_SYSTEM,			// [7/2/2010 kiny8216] 출석 시스템
	
	MSGLCMD_REGISTER_MERCHANT,			// // [2010/08/26 : Sora] ADD_SUBJOB 상인 등록
	MSGLCMD_LORD_COSTUME_REQ,			// [9/15/2010 kiny8216] 성주 코스튬

	MSGLCMD_ROYALRUMBLE_MANAGER,		// royal rumble 관리인. [4/25/2011 rumist]
	MSGLCMD_NEWGUILDSTASH_REQ,
	MSGLCMD_REFORM_SYSTEM,				// reform system [8/31/2012 박훈]	
	MSGLCMD_SYNDICATE_JEWEL_GIVE,
	MSGLCMD_SYNDICATE_SECESSION_REQ,
	MSGLCMD_TOTAL
};

enum CHECK_ITEM_FLAG
{
	UI_ITEM_FLAG_DEFAULT,
	UI_ITEM_FLAG_RARE,
	UI_ITEM_FLAG_SET,
};

// wooss 050802 define cash item 
#define		RECOVER_HEXP_ITEM		844		// 경험치 복구 주문서
#define		RECOVER_HEXP_ITEM_LUCKY	2035	// 럭키 경험치 복구 주문서	
#define		RECOVER_AEXP_ITEM		845		// 숙련도 복구 주문서
#define		RECOVER_AEXP_ITEM_LUCKY	2036	// 럭키 숙련도 복구 주문서
#define		REBIRTH_ITEM			846		// 부활 주문서
#define		WARP_ITEM				857		// 순간이동 마법서
#define		PC_SUMMON_ITEM			858		// 캐릭터(PC) 소환 마법서	
#define		BAAL_SUMMON_ITEM		859		// 바알 소환 마법서
#define		DEATHNIGHT_SUMMON_ITEM	860		// 데스나이트 소환 마법서
#define		CHANGE_MY_NAME_ITEM		842		// 캐릭터 개명카드
#define		CHANGE_GUILD_NAME_ITEM	843		// 길드 개명카드	
#define		MEMSCROLL_EX_ITEM		840		// 메모리 스크롤 확장 카드
#define		WAREHOUSE_EX_ITEM		841		// 개인창고 확장 카드
#define		INIT_STATUS_ITEM		847		// 스테이터스 초기화 카드
#define		INIT_COMBAT_SKILL_ITEM	848		// 전투스킬 초기화 카드
#define		INIT_SPECIAL_SKILL_ITEM	849		// 특수스킬 초기화 카드
#define		INIT_SKILL_ITEM			850		// 스킬 초기화 아이템.
#define		INIT_ALL_ITEM			851		// 능력치 초기화 카드
#define		EXCHANGE_WEAPON_ITEM	975		// 무기 교환 카드	
#define		EXCHANGE_ARMOR_ITEM		2355	// 방어구 교환 카드
#define		CHANGE_MY_NAME_ITEM_UE	1120	// 캐릭터 개명 카드(통합이벤트 기존 개명카드에서 속서을 다르게 처리)
#define		PARTY_RECALL_ITEM1		1298	// 파티 리콜 아이템	(24시간)
#define		PARTY_RECALL_ITEM2		1299	// 파티 리콜 아이템	(7일간)
#define		PARTY_RECALL_ITEM3		1300	// 파티 리콜 아이템	(30일간)
#define		PARTY_RECALL_ITEM4		1413	// 파티 리콜 아이템	(3시간)
#define		PARTY_RECALL_ITEM5		1414	// 파티 리콜 아이템	(7시간)
#define		PARTY_RECALL_ITEM6		1415	// 파티 리콜 아이템	(1시간)
#define		PARTY_RECALL_IRIS		2945	// 파티 리콜 아이템 (아이리스 대륙에서만 사용[1시간])
#define		PARTY_RECALL_ASKA		2946	// 파티 리콜 아이템 (아스카디아 대륙에서만 사용[1시간])
#define		PLATINUM_IRIS_ITEM				1416	// 플래티늄 아이리스 
#define		PLATINUM_BOOSTER_ITEM			1417	// 플래티늄 부스터 
#define		PLATINUM_UPGRADESTONE_ITEM1		1418	// 플래티늄 제련석(감정 전)	
#define		PLATINUM_UPGRADESTONE_ITEM2		1419	// 플래티늄 제련석(감정 후)
#define		COMPOSITE_ITEM1					1420	// 결합 주문서
#define		COMPOSITE_ITEM2					1421	// 결합 주문서
#define		COMPOSITE_ITEM_EVENT1			2664	// 결합 주문서
#define		COMPOSITE_ITEM_EVENT2			2665	// 결합 주문서
#define		PET_MOUNT_CANCEL_CARD			1519	// 펫 마운트 훈련 취소 카드
#define		UNCOMPOSITE_ITEM				1540	// 결합 분리 주문서
#define		PACKING_PAPER_ITEM				2046	// 포장지 아이템
#define		SMALL_CORD_ITEM					2632	// 복주머니 노끈
// wooss 070305
// kw : WSS_WHITEDAY_2007
#define		BLUE_LETTER_ITEM				2135	// 하늘색 편지지
#define		PINK_LETTER_ITEM				2136	// 핑크색 편지지

// [KH_070315] 프리미엄 메모리스크롤 관련 추가
#define		PRIMIUM_TELEPORT				2052	// [KH_070315] 프리미엄 메모리스크

// [KH_070326] 달걀 이벤트 관련
#define		EASTER_EGGS						2148
// [KH_070413] 스승의날 이벤트 관련 추가
#define		MEDAL_OF_DISCIPLE				2329
#define		MEDAL_OF_TEACHER				2330

// WSS_GUILDMASTER 070511 어버이날 이벤트 상품으로 추가
#define		MEMSCROLL_EX_ITEM_7DAYS		2348		// 메모리 스크롤 확장 카드
#define		WAREHOUSE_EX_ITEM_7DAYS		2347		// 개인창고 확장 카드

#define		PET_NAMECARD_ITEM			2360		// 펫의 명찰

// [070824: Su-won] PET_COLOR_CHANGE
#define		PET_COLOR_RED				2396
#define		PET_COLOR_ORNAGE			2397
#define		PET_COLOR_YELLOW			2398
#define		PET_COLOR_GREEN				2399
#define		PET_COLOR_BLUE				2400
#define		PET_COLOR_INDIGO			2401
#define		PET_COLOR_VIOLET			2402
#define		PET_COLOR_WHITE				2568
#define		PET_COLOR_BLACK				2569
#define		PET_COLOR_PHOENIX			3262
#define		PET_COLOR_BLACK_HEART		3548
#define		PET_COLOR_WHITE_HEART		3549
#define		PET_COLOR_YELLOW_HEART		3550

// [090710: selo] PET_TATOO
#define		PET_TATOO_HORSE				740
#define		PET_TATOO_DRAGON			741


// [071122: Su-won] NEW_MOONSTONE
#define		MOONSTONE_POOR_MIX_BOX				2552
#define		MOONSTONE_USEFUL_MIX_BOX			2553
#define		MOONSTONE_SOLID_MIX_BOX				2554
#define		MOONSTONE_STRONG_MIX_BOX			2555
#define		MOONSTONE_TRUSTY_MIX_BOX			2556

// eons 루돌프 변신 주문서
#define		PET_TRANSFORM_RU			2604
#define		PET_TRANSFORM_RETURN		2605

#define		REBIRTH_ITEM_NEWBIE			2667		// 부활 주문서(초보자용)
#define		REBIRTH_ITEM_PHOENIX		3218		// 피닉스의 부활 아이템
#define		REBIRTH_ITEM_SCROLL2		4933		// rebirth second item. [11/10/2009 rumist]
#define		REBIRTH_ITEM_EVENT			7056		// [이벤트용]부활 주문서

// 캐쉬 인벤 열쇠 아이템.
#define		INVEN_CASH_KEY1				(9936)
#define		INVEN_CASH_KEY2				(9937)

#define		DEF_HOLYWATER_STRONG		(10295) // 힘의성수 (홀리워터) 아이템
#define		DEF_HOLYWATER_MYSTERY		(10296) // 신비한성수 (홀리워터) 아이템
#define		DEF_HOLYWATER_DAMAGE		(10978) // 격돌의성수 (홀리워터) 아이템
#define		DEF_HOLYWATER_SKILLDAMAGE	(10979) // 기적의성수 (홀리워터) 아이템

// Type:일회용 - SubType:기타일회용 아이템
enum ITEM_ONCEUSE_ETC
{
	RARE_IDENTITY_ITEM1	=1838,			// 돋보기. 레어아이템 감정. 일정 확률로 감정
	RARE_IDENTITY_ITEM2	=1839,			// 황금 돋보기. 레어아이템 감정. 100% 감정
	GOLD_DISH =2146,					// 태국 송크란 이벤트 금그릇 
	SILVER_DISH =2147,					// 태국 송크란 이벤트 은그릇
	RARE_IDENTITY_ITEM_NEWBIE =2666,	// 초보자용 돋보기.
	// socket system [5/18/2010 rumist]
	SOCKET_ITEM_CHANCE_CARD = 5329,		// socket one more chance card.
	PET_STAT_INIT_CARD		= 6281,		// pet status initialize card. [1/19/2011 rumist]
	MYSTERIOUS_BEAD			= 9938,		// 신비한 석상 구슬 아이템
	SEALED_SOUL				= 10022,	// 봉인된 영혼
	EXP_PET_COOLTIME_REMOVE = 10286,	// 경험치펫 쿨타임 제거제.
};

// HUD Type
enum HUD_DATA_TYPE
{
	HUD_CHARACTER_TYPE	= 0,
	HUD_ITEM_TYPE		= 1,
};

// Character state flags
#define	CSF_EMPTYALL			0x00000000
#define	CSF_SKILL				0x00000001		// 스킬 사용중
#define	CSF_TELEPORT			0x00000002		// 순간 이동중
#define	CSF_PARTY				0x00000004		// 파티 플레이중
#define	CSF_PARTY_REQ			0x00000008		// 파티 요청중
#define	CSF_EXCHANGE			0x00000010		// 교환중
#define	CSF_SHOP				0x00000020		// 상점 거래중
#define CSF_REMISSION			0x00000040		// 면죄부.
#define	CSF_PERSONALSHOP		0x00000080		// 상점 거래중
#define CSF_WAREHOUSE			0x00000100		// 보관함 이용중.
#define CSF_CHANGEWEAPON		0x00000200		// 무기 교체중...
#define CSF_QUIZ				0x00000400		// 퀴즈 맞추는 중.
#define CSF_MOONSTONE			0x00000800		// 문스톤 이벤트 중.
#define CSF_CASTLE_WAR_READY	0x00001000		// 공성 준비중에는 못 움직임...
#define CSF_MOUNT_HUNGRY		0x00002000		// 마운트 상태에서 배가 고픔.
#define	CSF_EXPEDITION			0x00004000		// [sora] 원정대 플레이중
#define	CSF_EXPEDITION_REQ		0x00008000		// [sora] 원정대 참여 요청중
#define CSF_PETRIDING			0x00010000		// 펫을 타고 있는 중~~(클라에서 서버응답때까지)
#define CSF_SKILLREADY			0x00020000		// 스킬 레디 처리 중
#define CSF_ITEMWEARING			0x00040000		// 장비 착용 및 교체 시도 중
// 공성, 길드 등등 추가 예정

// 이동할 수 없는 상태
#define CSF_CANNOT_MOVE_MASK		( CSF_SKILL | CSF_TELEPORT | CSF_PERSONALSHOP | CSF_MOONSTONE | CSF_CASTLE_WAR_READY | CSF_MOUNT_HUNGRY )
// 순간 이동 할 수 없는 상태 ([sora]원정대 추가)
#define	CSF_CANNOT_TELEPORT_MASK	( CSF_SKILL | CSF_TELEPORT | CSF_PARTY_REQ |		\
									  CSF_EXCHANGE | CSF_PERSONALSHOP | CSF_EXPEDITION_REQ )					\
// 앉을 수 없는 상태
#define CSF_CANNOT_SITDOWN_MASK		( CSF_SKILL | CSF_TELEPORT | CSF_EXCHANGE )
#define CSF_CANNOT_ATTACK_MASK		( CSF_CASTLE_WAR_READY )


// Valid distance of UI
#define	UI_VALID_SQRDIST			300.0f


// edit by cpp2angel (04.12.20) : 생산 인터페이스에 사용되는 칼라 값
#define COLOR_TITLE		0xe1b300ff	// 타이틀 
#define COLOR_SUB_TITLE	0xff9170ff	// 서브 타이틀 
#define COLOR_TEXT		0xffffffff	// 기본 텍스트에 사용 
#define COLOR_NONE		0x7f7f7fff	// 없는 조건들

// Time 관련 메크로
// Date : 2005-07-07(오후 3:21:10), By Lee Ki-hwan
#define MINUTE_TO_SEC(x)	((x)*60)
#define HOUR_TO_SEC(x)		((x)*60*60)
#define DAY_TO_SEC(x)		((x)*60*60*24)

// Week 관련 메크로
// Date : 2005-07-07(오후 3:21:20), By Lee Ki-hwan
enum EWEEK { SUNDAY, MONDAY, TUESDAY, WEDNESDAY, THURSDAY, FRIDAY, SATURDAY };

// Date : 2005-11-16(오후 7:42:49), By Lee Ki-hwan
// 타격 Type 
#define		HITTYPE_MISS			(1 << 0)		// 회피
#define		HITTYPE_WEAK			(1 << 1)		// 약하게
#define		HITTYPE_NORMAL			(1 << 2)		// 보통
#define		HITTYPE_STRONG			(1 << 3)		// 강하게
#define		HITTYPE_HARD			(1 << 4)		// 매우 강하게
#define		HITTYPE_CRITICAL		(1 << 5)		// 크리티컬
#define		HITTYPE_DEADLY			(1 << 6)		// 죽도록 강하게

// Date : 2005-11-16(오후 1:17:00), By Lee Ki-hwan
typedef struct _tagDAMAGE
{
	SBYTE	sbTargetFlag;	// 타격 종류 
	ULONG	ulDamage;		// 데미지 크기
	ULONG	ulIndex;		// 타격을 준 Index
	FLOAT	fX, fY;			// 출력 
	DOUBLE	dStartTime;
	BOOL	bVisible;
	FLOAT	fSize;
	FLOAT	fAlpha;
	BOOL	bDamaged;
	
	_tagDAMAGE()
	{
		sbTargetFlag	= 0;
		ulDamage		= 0;
		ulIndex			= 0;
		fX = fY			= 0.0f;
		dStartTime		= 0.0f;
		bVisible		= FALSE;
		fSize			= 1.0f;
		fAlpha			= 1.0f;
		bDamaged		= FALSE;
	}			

} DAMAGE;

// wooss 070324 --------------------------------------------------->>
// kw : WSS_HELP_SYSTEM_1
typedef struct _stHELP1
{
	int			m_index;
	int			m_subNum;
	int			m_subEnable;
	int			m_subLevel;
	CTString	m_strName;
	CTString	m_strDesc;
	CTString	m_strFile;
	int			m_uv_x;
	int			m_uv_y;
	int			m_width;
	int			m_height;

	int		transFlag;	//[sora] 미번역 스트링 index 표시

	_stHELP1()
	{
		m_index = -1;
		transFlag = 0;
	}

	//[sora] 미번역 스트링 index 표시
	void SetNoTranslate();
	void ClearNoTranslate();
} HELP1;
typedef HELP1* pHELP1;
// ----------------------------------------------------------------<<
// FIXME : 잦은 삽입과 삭제가 있는 부분이므로 list가 저절할듯...
typedef std::vector<DAMAGE>		DAMAGE_deque;

// added by sam 11/03/02
// 번역 되지않은 스트링 저장을 위한 벡터 
// [2011/05/31 : Sora] 클라이언트 만이 아니고 다른곳에도 사용할 수 있게 확장
enum eTransTab
{
	CLIENT_STRING = 0,
	ITEM_STRING,
	NPC_STRING,
	SKILL_STRING,
	QUEST_STRING,
	RARE_OPTION_STRING,
	ACTION_STRING,
	OPTION_STRING,
	SPECIAL_SKILL_STRING,
	HELP1_STRING,
	MONSTER_COMBO_STRING,
	AFFINITY_STRING,
	ITEMCOLLECTION_STRING,
	TYPE_MAX
};

//_RTWidth = 366, _RTHeight = 226,
#define		DEF_DEFAULT_RT_WIDTH	366
#define		DEF_DEFAULT_RT_HEIGHT	226

class CUIGuildMarkIcon;
class CUIIcon;

typedef std::vector<INDEX> NO_TRANS_STRING_INDEX;
typedef NO_TRANS_STRING_INDEX::iterator NO_TRANS_STRING_INDEX_IT;
// ----------------------------------------------------------------------------
// Name : CUIManager
// Desc :
// ----------------------------------------------------------------------------
class ENGINE_API CUIManager : public CUIWindowDeclare,
							  public CSingletonBase<CUIManager>
{
	friend CSingletonBase<CUIManager>;

private:
	CUIManager();
	virtual ~CUIManager();
	
public:
	enum UVRectPos {
		UV_UL, UV_UM, UV_UR,
		UV_ML, UV_MM, UV_MR,
		UV_LL, UV_LM, UV_LR,
		UV_Total,
	};

protected:
	CDrawPort		*m_pUIDrawPort;						// Pointer of draw port for rendering UIs
	
	CUIWindow		*m_apUIsOld;						// Prev Select UI
	int				m_aUIOrder[UI_TYPE_END];			// Order of UIs
	int				m_nDragX, m_nDragY;
	CUIIcon*		m_pIconDrag;
	CUIBase*		m_pBaseDrag;
	CUIMouseCursor	*m_pumcMouseCursor;					// Pointer of mouse cursor
	BOOL			m_bShowUIs;							// If UIs are shown or not
	BOOL			m_bRButtonDown;						// Hide mouse cursor if lbutton is down
	BOOL			m_bMouseInsideUIs;					// Show normal mouse cursor if mouse point is inside UIs
	PIX				m_pixMinI, m_pixMinJ;				// Size of drawport
	PIX				m_pixMaxI, m_pixMaxJ;				// Size of drawport
	UIMCType		m_umctTypeInUI;						// Wanted mouse type in UIs

	// Name, chatting & shop popup
	CTextureData	*m_ptdPopupTexture;					// Texture of popup
	CTextureData	*m_ptBlackTexture;					// Texture of popup
	CTextureData	*m_ptdExpeditionTexture;

	UIRect			m_rcName;							// Region of name
	UIRect			m_rcGuildName;						// Region of name
	UIRect			m_rcNickName;						// Region of nickname
	UIRect			m_rcGuild;							// Region of guild
	UIRect			m_rcChat;							// Region of chatting
	UIRect			m_rcShop;							// Region of shop

	UIRectUV		m_rtBlack;							// Region of name
	UIRectUV		m_rtNameL;							// UV of name
	UIRectUV		m_rtNameC;
	UIRectUV		m_rtNameR;
	UIRectUV		m_rtPremiumNameL;					// UV of Premium char name effect
	UIRectUV		m_rtPremiumNameC;
	UIRectUV		m_rtPremiumNameR;
	UIRectUV		m_rtNameRPK;
	UIRectUV		m_rtNameRDefPK1;
	UIRectUV		m_rtNameRDefPK2;
	UIRectUV		m_rtChatLU;							// UV of chatting
	UIRectUV		m_rtChatUp;
	UIRectUV		m_rtChatRU;
	UIRectUV		m_rtChatL;
	UIRectUV		m_rtChatC;
	UIRectUV		m_rtChatR;
	UIRectUV		m_rtChatLL;
	UIRectUV		m_rtChatLo;
	UIRectUV		m_rtChatRL;
	UIRectUV		m_rtShopLU;							// UV of personal shop
	UIRectUV		m_rtShopUp;
	UIRectUV		m_rtShopRU;
	UIRectUV		m_rtShopL;
	UIRectUV		m_rtShopC;
	UIRectUV		m_rtShopR;
	UIRectUV		m_rtShopLL;
	UIRectUV		m_rtShopLo;
	UIRectUV		m_rtShopRL;
	UIRectUV		m_rtShopPremLU;						// UV of premium personal shop
	UIRectUV		m_rtShopPremUp;
	UIRectUV		m_rtShopPremRU;
	UIRectUV		m_rtShopPremL;
	UIRectUV		m_rtShopPremC;
	UIRectUV		m_rtShopPremR;
	UIRectUV		m_rtShopPremLL;
	UIRectUV		m_rtShopPremLo;
	UIRectUV		m_rtShopPremRL;
	UIRectUV		m_rtNickNameL;							// UV of name
	UIRectUV		m_rtNickNameC;
	UIRectUV		m_rtNickNameR;

	UIRectUV		m_rtTargetLabel[LABEL_INDEX_TOTAL];	//0 도끼, 1 유령, 2 발바닥, 3 조각칼, 4 똥, 5 악마, 6 해골 

	CStaticArray<CTString>	m_aStringData;				// Array of string data

	DWORD			m_dwCSF;							// Character state flags
	TIME			m_tCSFElapsedTime;					// CSF ElapsedTime

	CGame			*m_pGame;
	int				m_LocalVersion;

	CTString		m_strSubJobName[TOTAL_SUBJOB];			// [2010/08/26 : Sora] ADD_SUBJOB 보조 직업명

	CRenderTexture* m_RenderTarget;							// [3/18/2011 kiny8216] LuckyDraw Open Rendter Target
	BOOL			m_bShowAni;								// [3/18/2011 kiny8216] LuckyDraw Render
	FLOAT			m_fAniStartTime;						// [3/18/2011 kiny8216] LuckyDraw Animation Time
	CTextureData	*m_ptdAniBG;							// [3/18/2011 kiny8216] LuckyDraw Animation BackGround Texture
	CUIRectSurface	m_AniBackGround;						// [3/18/2011 kiny8216] LuckyDraw Animation BackGround Render Surface
public :
	// wooss 050822 
	// TEST UI를 위해 추가
	// TEST 환경이 아닐 경우 이부분을 0로 처리 
	// testUI_TYPE
	int				m_mPosX,m_mPosY;
	int				m_JapanTempEvent;					// 일본 보물 상자 이벤트 발생 유무 저장

	int				m_nHelpNpc_Index;	// NPC 안내시스템 
	// connie [2009/9/16] - NPC 찾기
	FLOAT			m_fHelpMobX, m_fHelpMobZ;
	BOOL			m_IsHelpMob;
	UINT			m_NPCScrollZoneNo;				// BUG FIX : TO-KR-T20100223-010 [2/24/2010 rumist]
	BOOL			m_IsInField;
	// wooss 070324 --------------------------------------------------->><<
	// kw : WSS_HELP_SYSTEM_1
	CStaticArray<HELP1>	m_aHelp1Data;				// Array of string data

	CUIDrawBox		m_bxGuildRankBox;				// WSS_GUILDMASTER 070517	

	CUIEditBox		m_ebRefCntReturn;				// 참조 관련 체크용.(아놔 원래 이렇게 안짜는데.;;ㅜㅜ;) [11/23/2009 rumist]
	CUIMultiEditBox m_mbRefCntReturn;				// 멀티용.

	CDamageLogInfo	m_DamageLogInfo;				// QA팀 밸런스 테스트 용

	int				 	m_nCurInfoLines;						// Count of current item information lines
	CTString			m_strItemInfo[MAX_CASH_ITEMINFO_LINE];							// Item information string
	COLOR				m_colItemInfo[MAX_CASH_ITEMINFO_LINE];							// Color of item information string

	NO_TRANS_STRING_INDEX m_vecNoTrans[TYPE_MAX];

	bool	m_bCursorInGame;
	std::vector<std::string>	m_vecAdditionalPath;

protected:
	// Internal functions
	void	RenderObjectNamePopup( CProjection3D* pprProjection );
	void	RenderObjectIndexPopup( CProjection3D* pprProjection );
	void	LoadStringData( const CTFileName &fnString );
	// wooss 070324 --------------------------------------------------->>
	// kw : WSS_HELP_SYSTEM_1
	void	LoadHelp1Data( const CTFileName &fnString );
	void	LoadHelp1String();
	// ----------------------------------------------------------------<<

	// Command functions
	void	DropItem( int nTab, int inven_idx );

	// Network message functions ( send )
	void	SendDropItem( int nTab, int inven_idx, SQUAD llCount = 1 );

public:
	// WSS_GUILDMASTER 070517 ---------------------------------------->><<
	void	DrawGuildRankBox(UIRect,SBYTE,FLOAT);

	int				m_nShowMessageBox;

	// Create & destroy
	void	Create();

	void	InitHardCoreCreate();
	void	DestroyAll();

	void	InitUIString();

	// Render
	void	Render( CDrawPort *pdp, CProjection3D* pprProjection );
	
	void	SetGameHandle( CGame *pGame );

	// Get drawport
	CDrawPort	*GetDrawPort() const { return m_pUIDrawPort; }
	void	SetDrawPort( CDrawPort* pDraw ) { m_pUIDrawPort = pDraw; }
	PIX		GetMinI() const { return m_pixMinI; }
	PIX		GetMinJ() const { return m_pixMinJ; }
	PIX		GetMaxI() const { return m_pixMaxI; }
	PIX		GetMaxJ() const { return m_pixMaxJ; }

	// String data
	CTString	&GetString( INDEX iIndex )
	{
		#pragma warning( disable:4172 )
		static CTString nullStr = "";
		//nullStr = "";
		return iIndex < m_aStringData.Count() ? m_aStringData[iIndex] : nullStr;
	}
	CTString	&GetString( INDEX iIndex, INDEX iSrc );
	CTString	&GetString( INDEX iIndex, const char *szSrc );
	CTString	&GetString( INDEX iIndex, const CTString &strSrc );
	CTString	&GetString( INDEX iIndex, const char *szSrc1, const char *szSrc2 );
	CTString	&GetString( INDEX iIndex, const CTString &strSrc1, const CTString &strSrc2 );

	// Game state
	void	SetUIGameState(BOOL bGameOn = FALSE);
	
	// Toggle visible UIs
	void	ToggleVisibleUIs() { m_bShowUIs = !m_bShowUIs; }
	BOOL	IsVisibleUIs()		{ return m_bShowUIs; }

	// Rearrange UIs
	void	RearrangeOrder( int nCurrentUI, BOOL bFrontOrder = TRUE );
	void	ChangeOrder( int nCurrentUI, int nOrder );

	// Adjust position of UIs
	void	ResetUIPos( CDrawPort *pdp );		// Initialize position of UIs
	void	ResetUISavePos( CDrawPort *pdp );		// Initialize position of UIs
	void	AdjustUIPos( CDrawPort *pdp );		// Adjust position of UIs when main window changes

	// Set & get character state flags
	void	SetCSFlagOn( DWORD dwCSF );
	void	SetCSFlagOff( DWORD dwCSF ) {
#ifdef	VER_TEST
	if (dwCSF == CSF_TELEPORT)
		LOG_DEBUG("@@@@@ FREE CSF_TELEPORT");
#endif	// VER_TEST
		
		m_dwCSF &= ~dwCSF; 
	}
	void	ResetCSFlag() { m_dwCSF = 0; }
	DWORD	IsCSFlagOn( DWORD dwCSF ) { return m_dwCSF & dwCSF; }
	BOOL	IsInputLock();

	void	SetCSFlagOnElapsed( DWORD dwCSF, TIME tTime ) { m_dwCSF |= dwCSF; m_tCSFElapsedTime = tTime; }
	void	SetCSFlagOffElapsed( DWORD dwCSF ) { m_dwCSF &= ~dwCSF; m_tCSFElapsedTime = 0; }
	void	ResetCSFlagElapsed() { m_dwCSF = 0; m_tCSFElapsedTime = 0; }
	DWORD	IsCSFlagOnElapsed( DWORD dwCSF, TIME tElapsedTime );

	// Set & get mouse position state
	void	SetMouseCursorInsideUIs( UIMCType umctType = UMCT_NORMAL )
	{
		m_umctTypeInUI = umctType;
		m_bMouseInsideUIs = TRUE;
	}
	void	SetMouseInsideUIs(BOOL bOn) { m_bMouseInsideUIs = bOn; }
	BOOL	IsMouseInsideUIs() const { return m_bMouseInsideUIs; }
	BOOL	IsInsideUpperUIs( INDEX iCurUIIndex, int nX, int nY );

	// Get mouse cursor
	CUIMouseCursor		*GetMouseCursor() { return m_pumcMouseCursor; }

	// Get UI index by order
	int		GetUIIndexByOrder( int nOrder ) { return m_aUIOrder[nOrder]; }

	// Get UIs
	CUIWindow			*GetUI( int nUIIndex ) const { return m_apUIs[nUIIndex]; }

	CGame*	GetGame() { return m_pGame; }
	
	// Holding button
	void	SetHoldBtn(CUIIcon* pIcon);
	void	SetHoldBtn(CUIBase* pBase);

	void	ResetHoldBtn() { 
		m_pIconDrag = NULL;
		m_pBaseDrag = NULL;
		m_nDragX = -100;
		m_nDragY = -100;
	}
	CUIIcon* GetDragIcon()		{ return m_pIconDrag; }
	CUIBase* GetDragBase()		{ return m_pBaseDrag; }

	// MessageBox
	BOOL	CreateMessageBox( CUIMsgBox_Info &rMsgBoxInfo, int nPosX = -1, int nPosY = -1 );
	void	CloseMessageBox( int nCommandCode );
	BOOL	DoesMessageBoxExist( int nCommandCode );

	void	CreateMessageBoxL( CTString &strTitle, int nWhichUI, int nCommandCode,
								int nPosX = -1, int nPosY = -1 );
	void	AddMessageBoxLString( int nCommandCode, BOOL bDesc, CTString &strMessage, int iValue = -1, const COLOR colDesc = 0xF2F2F2FF, CTString strPrefix = "o" );
	void	CloseMessageBoxL( int nCommandCode );
	BOOL	DoesMessageBoxLExist( int nCommandCode );
	CUIMessageBox *GetMessageBox( int nCommandCode );

	// [090909 sora]
	CUIMessageBoxL *GetMessageBoxL( int nCommandCode );

	// WSS_SELECTIVE_UI_FUCNTION 2007/08/15 
	// SELECTIVE UI
	void	CloseSelectiveUI( int nUIidx=-1 );
	BOOL	DoesUIExist( int nUIidx );
	BOOL	IsUIVisible(int nUIidx);


	// Message
	void	MsgProc( MSG *pMsg, BOOL *pbIMEProc );
	void	MsgSimpleProc( MSG *pMsg );
	void	MsgProcWeb(MSG *pMsg);

	// Command functions
	void	MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput );
	void	MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput ,CTString &strConfirm );
	void	MsgCommonCommand( int nCommandCode );
	
	//wooss 050805 add message command for select button
	void	MsgBoxBtnCommand( int nCommandCode, int btnNum, CTString &strInput );
	
	void	MsgBoxLCommand( int nCommandCode, int nResult ) {}

	// Misc functions
	void	InsertCommaToString( CTString &strCount );
	COLOR	GetNasColor( SQUAD llNas );
	COLOR	GetNasColor( CTString strNas );
	CTString NasNumToWord( SQUAD llNas);

	void	CancelSkill( BOOL bLostTarget = TRUE, BOOL bSkillError=FALSE);
	void	Reset();
	void	LostTarget();

	void	SetVersion(int ver){m_LocalVersion = ver;};
	int		GetVersion(){return m_LocalVersion;};
	
	// wooss  060404 
	BOOL	IsPet(CItemData* pItemData);
	BOOL	IsWildPet(CItemData* pItemData);

	BOOL checkName(CTString strMsg, int chkType);

	CTString GetWeekToString( int nWeek )
	{
		if( nWeek < 0 || nWeek > 6 ) return CTString("None ");
		CTString strWeek[7] = { "일" , "월" , "화" , "수" , "목" , "금" , "토" };	// 번역
		return strWeek[nWeek];
	}

	// GetWeekToString 가 번역된 텍스트를 넘겨 주지 않아서 추가 
	// added by sam 101220
	int GetWeekToStringIndex( int nWeek )
	{
		if( nWeek < 0 || nWeek > 6 ) return -1;
		switch ( nWeek )
		{
			case 0:
			{
				return 1930;
			}
			case 1:
			{
				return 1931;
			}
			case 2:
			{
				return 1932;
			}
			case 3:
			{
				return 1933;
			}
			case 4:
			{
				return 1934;
			}
			case 5:
			{
				return 1935;
			}
			case 6:
			{
				return 1936;
			}
			default:
			{
				return -1;				
			}
		}
	}
	
	void SetTitleName( INDEX bFullScreen, int iScreenSizeI, int iScreenSizeJ );
	static void GetClassOfItem( CItemData* pItemData, CTString &strClass );
	DAMAGE_deque	m_qDamage;	
	UIRectUV		m_rtNumber[10];
	UIRect			m_rcNumber;
	
	UIRectUV		m_rtShowText[3];
	UIRect			m_rcShowText[3];
	
	BOOL			m_bDamageState; 
	
	int				m_nCoolTimeReductionRate;		//스킬쿨타임감소옵션이 붙은 장비를 입고 있을 때 쿨타임 감소비율
	int				m_nNeedMPReductionRate;				//MP소모량 감소 옵션이 붙은 장비를 입고 있을 때 감소비율

	std::map<SLONG, CEffectGroup *> m_mapEG;
	
	void AddDamageData( ULONG ulDamage, SBYTE sbTargetFlag, ULONG ulClientIndex, BOOL bDamaged );
	void ShowDamage( ULONG ulClientIndex );
	
	void SetDamageState();	
	void ShowDamageList( FLOAT3D vPopupPos, FLOAT fPopupZ, ULONG ulClientIndex );
	
	void ClearDamageData();	
	
	void DrawNumber( int nX, int nY, int nNumber, COLOR colColor = 0xFFFFFFFF, FLOAT fRatio = 1.0f );
	void DrawText( int nX, int nY, SBYTE sbTargetFlag, COLOR colColor = 0xFFFFFFFF, FLOAT fRatio = 1.0f );

	void SetRareOption( CNetworkMessage *istr, CItems& rItem );
	void SetRareOption( UpdateClient::itemInfo* pInfo, CItems& rItem );
	
	int  GetCoolTimeReductionRate() { return m_nCoolTimeReductionRate; }
	int  GetNeedMPReductionRate() { return m_nNeedMPReductionRate; }
	void SetCoolTimeReductionRate(int CoolTimeReductionRate) { m_nCoolTimeReductionRate =CoolTimeReductionRate; }
	void SetNeedMPReductionRate(int nNeedMPReductionRate) { m_nNeedMPReductionRate =nNeedMPReductionRate; }
	void AddCoolTimeReductionRate(int Add) { m_nCoolTimeReductionRate += Add; }
	void AddNeedMPReductionRate(int Add) { m_nNeedMPReductionRate += Add; }

	// WSS_MINIGAME_070422 -------------------------------------->>
	// For multilined string
	void AddStringToList(CUIListBox* pListBox,CTString& tStr, INDEX iMaxChar, COLOR tColor =0xFFFFFFFF,INDEX ilistCol=0);

	// ----------------------------------------------------------<<

	BOOL IsEnemy( void* pTarget, TARGET_TYPE eType, BOOL bSearch =FALSE );
	void StartTargetEffect( SLONG slIndex, CEntity* penEntity, BOOL bEnemy );
	void StopTargetEffect( SLONG slIndex );

	// [sora] 원정대, RAID 관련
	BOOL IsPlayInZone();	// 인스턴트존 플레이중인지 체크
	BOOL IsInstantZone(int nZone);

	// Focus Check용 함수. [11/23/2009 rumist]
	inline BOOL	IsFocusInAllEditbox() const { return CUIEditBox::IsFocusingEditBox(); }
	//inline const BOOL	IsFocusInAllEditbox() { return m_ebRefCntReturn.IsFocusedInAll() | m_mbRefCntReturn.IsFocusedInAll(); }
	BOOL IsFocusAllEditBox();	
	// [2010/08/26 : Sora] ADD_SUBJOB 보조 직업 관련 추가함수
	CTString GetSubJobName( SLONG slSubJobCode );
	BOOL CheckSellerItem(  UI_TYPE uiType,  int flag );

	// [2010/09/17 : Sora] Player.es에서 소켓 시스템 호출용
	void OpenSocketSystem(int iMobIndex, BOOL bHasQuest, FLOAT fX, FLOAT fZ );

	void InitCurInfoLines(void) { m_nCurInfoLines = 0; }
	void AddItemInfoString(CTString &strItemInfo, COLOR colItemInfo ,int maxLine, int maxChars);
	void RenderBtnInfo(CTextureData* texData, CUIButton& srcBtn, UIRectUV rtUV[], int nLength=34);

	// added by sam 11/03/02 번역되지 않은 스트링 넘버 보이기
	void ShowNoTranslationString ();
	void HideNoTranslationString ();
	void ToggleNoTranslationString ();
	// [2011/05/31 : Sora] 클라이언트 만이 아니고 다른곳에도 사용할 수 있게 확장
	void SetNoTranslationString( int transType, int index );
	void InitRenderTarget(int nWidth = DEF_DEFAULT_RT_WIDTH, int nHeight = DEF_DEFAULT_RT_HEIGHT);
	void DestroyRenderTarget();
	void HUD_SetItemModelData( INDEX iIndex );
	void HUD_DrawItemModel();
	void SetShowAni( BOOL bShow )	{	m_bShowAni = bShow;	}
	BOOL IsShowAni()	{	return m_bShowAni;	}
	CRenderTexture* GetRenderTarget()	{	return m_RenderTarget;	}

	BOOL CheckDratanWarInside(CCharacterTarget* pTarget);
	BOOL MyCheckDratanWarInside();

	//[sora] 미번역 스트링 index 표시
	void SetNoTranslateFlag( eNoTransType transType, int* flag )
	{
		(*flag) |= transType;
	}

	BOOL IsNotTranslated( eNoTransType transType, int flag )
	{
		return transType & flag;
	}

	void Update( float fDeltaTime, float fElapsedTime );

protected:
	void RenderDamage( int nIndex, DAMAGE_deque::iterator iter, DOUBLE dDealyTime, int nX, int nY );
	void RenderNotice();	


public:
	void Render(CDrawPort* pDraw) 
	{
		CUIBase::Render( pDraw );

		CUIBase* pUI = CUIFocus::getSingleton()->getUI();
		if (pUI)
			pUI->Render(pDraw);

		CUITooltipMgr::getSingleton()->Render(pDraw);
	}
	virtual void OnRender(CDrawPort* pDraw) {};

	bool isCreateVisible(UI_TYPE type)
	{
		// 새로운 UI는 Hide 사용
		if (m_apUIs[type] != NULL && 
			m_apUIs[type]->IsVisible() == TRUE || m_apUIs[type]->GetHide() == FALSE)
			return true;

		return false;
	}

	bool getCursorInGame()	{ return m_bCursorInGame; }
	std::string GetFullPath(std::string strFileName);
	int	GetPathCount()	{ return m_vecAdditionalPath.size();	}
	void ItemNameColorInI();
	COLOR GetItemNameColor(int nType) {	return m_colItemDropName[nType];	}
	void DropItemCallback();
	void SetGuildMark();

#ifdef	VER_TEST
	void setIPString(const char* strIP) { 
		extern INDEX sam_bFullScreenActive;
		extern INDEX sam_iScreenSizeI;
		extern INDEX sam_iScreenSizeJ;

		m_strIP = strIP; 
		SetTitleName( sam_bFullScreenActive, sam_iScreenSizeI, sam_iScreenSizeJ );
	}

	bool GetDebugOutMode()			{ return m_bOutDebugString; }
	void SetDebugOutMode(bool bOut)	{ m_bOutDebugString = bOut; }
private:
	std::string m_strIP;

	bool m_bOutDebugString;	
#endif	// VER_TEST
private:
	COLOR m_colItemDropName[eDROPITEM_NAME_COL_MAX];
	CUIGuildMarkIcon*	m_pIconGuildMark;
};

#define		UIMGR()		CUIManager::getSingleton()

// KALYDO
ENGINE_API extern  struct PlayerControls pctlCurrent;



inline ENGINE_API CTString _SFunc( int x );

//#define KEY_SHOW_STRING_INDEX
// 미번역 찾아내기 [10/13/2011 rumist]
#define _S(x,comment) _SFunc(x)
#define _S2(x,str1,comment) CUIManager::getSingleton()->GetString(x,str1)
#define _S3(x,str1,str2,comment) CUIManager::getSingleton()->GetString(x,str1,str2)
#define _s(str) CTString(str)

// 임시로 추가된 Define
// getSingleton에서 singleton object를 새로 생성하여 받게 되어, comment를 찾을 수 없게 된다.
// 직접 참조 객체를 전달하는 define 함수를 선언하였다.
#define _SMgr(m, x, comment) m->GetString(x)
#define _S2Mgr(m, x, str1, comment) m->GetString(x, str1)
#define _S3Mgr(m, x, str1, str2, comment) m->GetString(x, str1, str2)

#endif	// UIMANAGER_H_

