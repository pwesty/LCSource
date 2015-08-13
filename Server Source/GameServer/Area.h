#ifndef __AAREA_H__
#define __AAREA_H__

#include <vector>
#include <set>

#include "Env.h"
#include "Cell.h"
#include "MonsterCombo.h"

#include "TrigerSet.h"
#include "RaidData.h"

typedef std::vector<CNPCRegenInfo*> vec_raidnpcrregenlist_t;
typedef std::set<int> set_int_t;

#define MAX_AREA_COUNT 300

class CZone;

struct tag_npc_regen_info 
{
	int			index;
	int			npcIndex;
	float		x;
	float		z;
	float		r;
	int			yLayer;
	int			regenSec;
	int			totalNum;

	CNPCProto*	pNPCProto;
};

class CArea
{
public:
	CArea();
	~CArea();
	CGroundEffect2 m_GroundEffect2;
	CPC* m_AkanPCList[8];
	int m_nCountDown;
	int m_nObjectIndex;
	int m_nPauseMobActivity;
	int m_nPauseIndex;
	CNPC* m_pBossAppearNPC;
	int m_nDelayOpenGate;
	int m_nRaidRoomNo;

	bool		m_bEnable;			// 활성화 여부
	CZone*		m_zone;				// 소속 존
	int			m_index;			// 영역 번호

	int			m_size[2];			// 배열 크기: 0: width, 1: height: 셀 단위
	int			m_ycnt;
	CEnv***		m_env;				// 환경 배열
	CCell**		m_cell;				// 셀 배열

	CNPCRegenList m_npcRegenList;	// NPC 리젠 정보 리스트
	vec_raidnpcrregenlist_t m_raidNPCRegenList;		// RAID MONSTER 리젠 정보 리스트

	set_int_t m_NpcPortal_NpcList;	// mob 툴 인덱스
	set_int_t m_NpcPortal_MobList;	// mob 툴 인덱스
	bool IsNpcPortalList_Npc(int index);
	bool IsNpcPortalList_Mob(int index);

	CNPC*		m_npcList;			// NPC 리스트
	int			m_nCountNPC;		// NPC 수
	int			m_pulseNPCAI;		// NPC AI pulse
	int			m_pulseNPCRegen;	// NPC regen pulse
	// int			m_nMaxCountNPC;		// NPC 최대 수

	int			m_nJunkItems;		// 사라진 아이템 수

	int			m_nMakeNPC;		// 싱글던전에서 만든 몹 수
	bool		m_bBoss;		//  발록방 한번 터치패드 통과했는가? : 2번째 통과시 발록방, 첫번째는 2층
	int*		m_regenList;	// 싱글던전에서 리젠되었던 리스트(regen의 a_index)

	int			m_guildIndex;		// 길드룸의 경우 길드 인덱스

	int			m_nNPC_Killed_NPC;

	// 초기화
	void Init(CZone* z, int index, int w, int h, int ycnt);

	// x, z에서 셀 번호 얻음
	void PointToCellNum(float x, float z, int* cx, int* cz);

	// 캐릭을 셀에 넣기
	void CharToCell(CCharacter* ch, char cy, int cx, int cz);
	void CharToCellForSummonNpc(CCharacter* owner, CCharacter* ch, char cy, int cx, int cz);

	// 셀에서 캐릭 빼기
	void CharFromCell(CCharacter* ch, bool bDeleteAttackList);

	// 캐릭 이동
	bool MoveChar(CCharacter* ch, char y, float x, float z, float h, float r, MSG_MOVE_TYPE movekind, CCharacter* target = NULL);

	// 캐릭 이동 시에 블럭되는지 검사
private:
	bool IsNotBlocked(char sy, float sx, float sz, float tx, float tz, bool bCastleNPC, char& ty);		// s에서 t로 이동 가능 여부 검사 : 밑의 캐릭터용 InNotBlocked()에서 사용

public:
	bool IsNotBlocked(CCharacter* ch, CCharacter* tch, bool battack, char& outYlayer);
	bool IsNotBlocked(CCharacter* ch, char y, float x, float z, bool battack, char& outYlayer);

	// 특정 위치의 Attribute를 얻음
	unsigned short GetAttr(char y, float x, float z);
	// 높이 값 얻기
	float GetHeight(char y, float x, float z);

	// ch 근처에서 tch_type 종류의 idx 캐릭을 반환
	CCharacter* FindCharInCell(CCharacter* ch, int idx, MSG_CHAR_TYPE tchType, bool bIncludeDead = false);
	CNPC* FindProtoNPCInCell(float x, float z, int nData, bool bIsFlagMask, int findNumCell, bool bIncludeDead = false);
	CNPC* FindProtoNPCInCell(CCharacter* ch, int nData, bool bIsFlagMask, int findNumCell = 2, bool bIncludeDead = false);

	// area의 tch를 찾음
	CCharacter* FindChar(int idx, MSG_CHAR_TYPE tchType);

	// 셀에 메시지 전달 : 셀에서 CELL_EXT 반경안의 모든 PC에게 전달한다
	bool SendToCell(CNetMsg::SP& msg, CCharacter* ch, bool withme = false, int cellext = CELL_EXT);
	bool SendToCell(CNetMsg::SP& msg, char y, int cx, int cz, int cellext = CELL_EXT);
	// 해당 area의 모든 PC에게 msg 전달
	void SendToAllClient(CNetMsg::SP& msg);

	// 셀의 정보를 ch에게 전달
	// bool bExcludeOwnPetElemental : ch의 소유인 펫과 소환수를 제외
	void SendCellInfo(CPC* ch, bool bExcludeOwnPetElemental);

	// 특정 셀의 인접 지역의 PC 수를 반환
	int GetPCCount(CCharacter* ch, int findArea = CELL_EXT);
	int GetPCCount(int cx, int cz, int findArea = CELL_EXT);
	int	GetPCCount();

	// 리젠 정보 설정
	void LoadNPC(std::vector<tag_npc_regen_info>& vec);

	// NPC를 추가한다
	void AddNPC(CNPC* npc);

	// NPC를 제거
	void DelNPC(CNPC* npc);

	// Mob의 상태 이상 등을 적용
	CNPC* MobTimeCheck(CNPC* npc);

	// 영역 활성화
	void SetEnable(int nRaidRoomNum=-1, int nDifficulty = -1);

	// 영역 비활성화
	void SetDisable();

	// 몹 행동
	void MobActivity();

	// 몹 리젠
	void MobRegen();
	// 레이드 몬스터 리젠, 하나라도 리젠되면 true, 아니면 false
	bool RaidMobRegen(float x, float z, float r, char y);

	// npc와 가까운 공격대상을 찾기
	CCharacter* FindNearestTarget(CNPC* npc, float* dist);

	// 테이밍된 npc가 가까운 npc를 찾기
	CCharacter*	FindNearestTarget_Taming_or_Confused(CNPC* npc, float* dist);

	// 몹의 행동 루틴 독립적으로 수행
	void	MobActivity_Taming(CNPC* npc);
	void	MobActivity_Confused(CNPC* npc);

	// m_npcList가 꼬이는 것을 위한 동기화
	void	Monster_synchronization( int nCount );
	void MobActivity_Mercenary(CNPC* npc);
	void MobActivity_Totem(CNPC *npc);
	void MobActivity_Suicide(CNPC *npc);
	void MobActivity_Item_Totem(CNPC* npc);

	// 아이템을 셀에 넣기
	void ItemToCell(CItem* item, char cy, int cx, int cz);

	// 아이템에서 캐릭 빼기
	void ItemFromCell(CItem* item);

	// 아이템 땅에 떨구기
	CItem* DropItem(int item_db_idx, CCharacter* ch, int plus, int flag, LONGLONG count, bool bOption = false);
	CItem* DropItem(CItem* item, CCharacter* ch);

	// 떨어진 아이템 정리
	void GroundItemControl();

	// aitype에 해당하는 동족 찾기, aitype이 -1이면 무시, hpParam미만의 체력이 남은 대상(단위는%)
	CNPC* FindNearestFamily(CNPC* npc, int aitype = -1, int hpParam = 100);

	// 영역안에 있는 캐릭터 모두 이동 시키기
	void MoveAllChar(int nIndex, int nExtra);

	CMonsterCombo* m_monsterCombo;
	void RunComboZone();
	int  CountPCInExtra(int extra, bool bDead);
	int	 CountMobInCombo();
	void CloseComboZone();

	bool			m_bUseTriggerEvent;		// 트리거를 사용합니까?
	CTrigger_List	m_CTriggerList;			// 해당 area에 사용되는 트리거 리스트
	int				m_pulseProcTrigger;		// 초당 관리해야 하는 트리거를 위한 펄스

	void Change_NpcRegenRaid(int level, int exceptingNpcIndex = 0);	// 레이드 npc regen 레벨 계산하여 활성화, regen save도 같이 함
	int	 GetAllClientCount_Area();									// Area에 존재하는 모든 pc의 숫자를 센다.
	void GotoAllClient_StartPoint();							// area의 모든 유저를 시작 지점으로 보냄(몬트샤인)
	void ApplySkillToAllClient(CSkill* skill, bool &bApply);	// area의 모든 유저에게 디버프를 시전한다.

	CAreaRaidInfoList m_RaidInfoList[LEVEL_COUNT];
	int m_RaidDifficulty;

private:
	int m_mobAISeq;
	bool m_bFreeze;
	int m_freezePulse;
public:
	int GetMobAISeq(void)
	{
		return m_mobAISeq;
	}
	int GetMobAIVec(void)
	{
		return (1 << m_mobAISeq);
	}
	void SetFreeze(int m_pulse);
	void SetMelted(void);
	bool IsFreeze(int m_pulse);

	int findNpcVIndex(int dbIndex);

	int m_inCount;

public:
	static void checkLifeTime();

private:
	static std::set<CNPC*> m_lifetime_set;

};

#define GoZone(ch,zone,layer,x,z)		GoZoneBase(ch,zone,layer,x,z,__FILE__,__LINE__)
#define GoZoneForce(ch,zone,layer,x,z)		GoZoneForceBase(ch,zone,layer,x,z,__FILE__,__LINE__)

bool GoZoneBase(CPC* ch, int zone, char ylayer, float x, float z, const char* fname, const int line);
void GoTo(CCharacter* ch, char y, float x, float z, float h, float r, char bHack = 0);
bool _GMGoZone(CPC* ch, CPC* pc);
void GMGoZone(CPC* ch, CPC* pc);
void GoZoneForceBase(CPC* ch, int zone, char ylayer, float x, float z, const char* fname, const int line);

#endif
//
