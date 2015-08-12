#ifndef __TRIGER_SET_H__
#define __TRIGER_SET_H__

// 하나의 트리거 동작 담장
class CTrigger
{
public:

/////////////////////// 공통 변수
	/////////DB 정보////바뀌면 안됨
	int			m_index;								// DB Index
	int			m_triggerset_type;						// 트리거 속성
	int			m_trigger_touchID;						// 이것을 식별자로 사용
	int			m_trigger_playID;
	int			m_trigger_type;
	CLCString	m_trigger_data;
	int			m_nSaveLevel;							// 중간 저장을 위한 정보
	int			m_group_index;
	int			m_nType_sub;							// 서브 타입이 있다면 이것을 사용
	int			m_bContinue;							// 트리거가 유지되는 것인지 여부
	int			m_bDomino;								// 도미노 형식 트리거 발동인지 여부
	int			m_bPlayOnePC;							// 일반 필드에서 한 명에게만 보여야 한다.

	////////메모리 정보////초기화 대상
	bool		m_bUsed;								// 이미 트리거가 발동하였는지 여부
	long		m_TriggerTime;							// 트리거 제어를 위한 시간변수

////////////////// 각 트리거별 변수

	/////// ITEM_COMPOSITION
	int		m_nItem_index_ItemComposition;				// 합성된 아이템 인덱스
	int		m_nMeterial_IndexCount;						// 사용 아이템 종류
	int		m_nMeterial_index[TRIGGER_MAX_ARRAYCOUNT];	// 사용 아이템 인덱스
	int		m_nMeterial_count[TRIGGER_MAX_ARRAYCOUNT];	// 사용 아이템 개수

	/////// ITEM_DROP
	int		m_ItemDrop_index;							// 드랍되는 아이템 인덱스
	int		m_ItemDrop_percent;							// 드랍되는 확률, 1 ~ 100%

	/////// OPEN_THE_DOOR	- 서브 타입 사용
	int		m_Killed_Npc_Index;							// 문을 열기 위해 죽여야 하는 npc 종류
	int		m_Killed_Npc_Count;							// 문을 열기 위해 죽여야 하는 npc 숫자
	int		m_NeedItem_IndexCount;						// 문을 열기 위해 얻어야 하는 item 종류 수
	int		m_NeedItem_Index[TRIGGER_MAX_ARRAYCOUNT];	// 문을 열기 위해 얻어야 하는 item 인덱스
	int		m_NeedItem_Count[TRIGGER_MAX_ARRAYCOUNT];	// 문을 열기 위해 얻어야 하는 item 숫자

	/////// START_RAID

	/////// MESSAGE_PRINT
	int		m_nStringIndex;								// t_string_index 를 지정

	/////// SUMMON_NPC
	int		m_nSummonNpc_indexCount;					// 소환되는 npc 종류 수
	int		m_nSummonNpc_index[TRIGGER_MAX_ARRAYCOUNT];	// 소환되는 npc 인덱스
	int		m_nSummonNpc_count[TRIGGER_MAX_ARRAYCOUNT];	// 소환되는 npc 숫자

	/////// USE_DEBUF
	int		m_nUseDebuf_indexCount;						// 디버프 걸리는 종류 수
	int		m_nUseDebuf_index[TRIGGER_MAX_ARRAYCOUNT];	// 디버프 걸리는 인덱스

	/////// ACTIVE_OBJECT

	/////// REMOVE_NPC	- 서브 타입 사용
	int		m_Item_Index_removeNPC;						// 죽이는데 사용되는 item index
	int		m_KilledNpc_Index_ForRemoveNpc;				// 죽는 npc index
	int		m_KillCount;								// item 당 죽는 npc 수

	/////// TIME_LIMIT
	long	m_TimeLimitSec;								// 제한 시간 (sec)

	/////// QUEST_COMPLETE
	int		m_nQuestIndex;								// 완료할 퀘스트 인덱스

	/////// ITEM_GET_INVEN
	int		m_nGetItem_TotalCount;						// 인벤에 들어가는 아이템 총 종류
	int		m_nGetItem_Index[TRIGGER_MAX_ARRAYCOUNT];	// 인벤에 들어가는 아이템 인덱스
	int		m_nGetItem_Count[TRIGGER_MAX_ARRAYCOUNT];	// 인벤에 들어가는 아이템 개수
	int		m_nGetItem_Prob[TRIGGER_MAX_ARRAYCOUNT];	// 인벤에 들어가는 획득 확률

	/////// CONSECUTIVE_DIALOGUE
	int		m_nMainDialogueString;						// 연속 대화 처음 창
	int		m_nNextDialogueString1;						// 연속 대화 스트링 인덱스
	int		m_nNextDialogueString2;						// 연속 대화 스트링 인덱스
	int		m_nNextDialogueString3;						// 연속 대화 스트링 인덱스
	int		m_nNextDialogueString4;						// 연속 대화 스트링 인덱스
	int		m_nNextDialogueString5;						// 연속 대화 스트링 인덱스
	int		m_nNextDialogueTriggerID1;					// 연속 대화 트리거 아이디
	int		m_nNextDialogueTriggerID2;
	int		m_nNextDialogueTriggerID3;
	int		m_nNextDialogueTriggerID4;
	int		m_nNextDialogueTriggerID5;

	/////// ACTIVE_EFFECT
	CLCString	m_strEffectName;						// 이펙트 이름
	int			m_nTargetType;							// 자신인지 다른 npc 인지
	int			m_nEffectNPCIndex;						// 다른 npc이면 npc 인덱스

	CTrigger();
	~CTrigger();

	void		SetTriggerTime(long time)
	{
		m_TriggerTime = time;
	};
	long		GetTriggerTime()
	{
		return m_TriggerTime;
	};

	void		CopyTriggerDate(CTrigger* trigger);
};

// 트리거들의 정보를 담고 있는 관리용 리스트
class CTrigger_List
{
public:

	// DB
	CTrigger*	m_Trigger;
	int			m_nCount;
	CArea*		m_pArea_UseTrigerEvent;

	// 예배당 1차 npc 963 몇마리 죽었는지 체크(하드코딩에서 사용)
	int			m_nNPC963_KilledCount;
	int			m_flagTriggerEvent;				// 트리거 이벤트 사용 플래그 첫번째

	int     Check_TriggerFlag(int mask)
	{
		return m_flagTriggerEvent & mask;
	}
	void    Set_TriggerFlag(int state)
	{
		m_flagTriggerEvent |= state;
	}
	void    Reset_TriggerFlag(int state)
	{
		m_flagTriggerEvent &= ~state;
	}

	void	InitTriggerList();
	bool	Load_Trigger();

	CTrigger_List();
	~CTrigger_List();

	bool	ResetTriggerInfo(bool bDelete);			//	트리거 초기화, 레이드에만 사용할 것, bSave는 DB세이브도 초기화 할지 여부
	bool	SaveTriggerInfo(int saveInfo, int roomNumber=0);	//	트리거 정보를 저장을 함
	void	LoadTriggerSaveInfo(int roomNumber);	//  서버 부팅 시 트리거의 중간 정보를 불러 옴
	bool	LoadTriggerInfo(int triggerSetType);	//  서버 부팅 시 트리거 정보를 불러 옴

	bool	SyncForClient_TriggerInfo(CPC* ch);		//  클라이언트 입장시 현재 트리거 정보를 받음
	void	TriggerTimerCheck();					//	타이머를 사용하는 트리거 체크, 하드코딩 처리

	void	SetArea(CArea* pArea)
	{
		m_pArea_UseTrigerEvent = pArea;
	};
	CArea*	GetArea()
	{
		return m_pArea_UseTrigerEvent;
	};

private:
	void		Swap_Trigger(CTrigger* trigger1, CTrigger* trigger2);
};

typedef struct NPC_REGEN_FOR_RAID
{
	int		m_npcIdx;
	float	m_pos_x;
	float	m_pos_z;
	float	m_pos_h;
	float	m_pos_r;
	int		m_y_layer;
	int		m_storeMiddle;				//중간 저장 등급 - 등급에 따른 npc 리젠 처리
} NPC_REGEN_FOR_RAID;

// 인스턴트 던전의 리젠 정보를 담음
// 선행 탐색이라 알고리즘 필요 없음
// 이 정보는 DB 정보이므로 한 개만 필요함
class CNpc_Regen_Raid
{
public:

	int m_nCount_regen;
	NPC_REGEN_FOR_RAID* m_regenList_InstantDoungen;

	CNpc_Regen_Raid();
	~CNpc_Regen_Raid();
};

#endif
