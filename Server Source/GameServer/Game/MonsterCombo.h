// MonsterCombo.h: interface for the CMonsterCombo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MONSTERCOMBO_H__A0046A35_9FFA_4984_98CE_7C324A4237A6__INCLUDED_)
#define AFX_MONSTERCOMBO_H__A0046A35_9FFA_4984_98CE_7C324A4237A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define MAXCOMBOCASE		20
#define MAXEFFECT			5

class CArea;
class CNPC;

struct MOBTYPE
{
	int		npcidx;		// 출연하는 npc 인덱스
	char	count;		// 출연하는 npc 카운트
};

struct MOBREGEN
{
	char		step;		// 몇번째 리젠인가?
	char		typecount;	// 리젠되는 몹타입의 개수
	MOBTYPE*	mobtype;	// 리젠되는 몹타입

	MOBREGEN()
	{
		step = 0;
		typecount = 0;
		mobtype = NULL;
	};

	~MOBREGEN()
	{
		step = 0;
		if(mobtype)
		{
			delete[] mobtype;
			mobtype = NULL;
		}
	}
};

struct PENALTY
{
	int index;
	int level;
};

struct EFFECTPOS
{
	CNPC*	npc;
	float	x;
	float	z;
	float	h;
};

class CMissionCaseProto
{
public:
	int		m_nIndex;
	int		m_nNas;
	int		m_nPoint;
	CLCString m_name;

	// 리젠
	int				m_nRegenCount;		// 리젠 단계
	MOBREGEN*		m_npcRegen;		// 리젠 정보

	// 패널티
	CLCList<PENALTY>	m_listPanalty;		// 패널티 가능한 스킬인덱스

	bool			LoadProto();

	const MOBREGEN*		FindMobRegen(int step);

	CMissionCaseProto();
	~CMissionCaseProto();
	// 보상품
};

class CMIssionCaseList
{
public:
	CMIssionCaseList();
	~CMIssionCaseList();

	int					m_nCount;
	CMissionCaseProto*	m_proto;

	CMissionCaseProto*	FindProto(int index);
	bool				LoadList();

	static int CompIndex(const void* p1, const void* p2)
	{
		CMissionCaseProto* m1 = (CMissionCaseProto*)p1;
		CMissionCaseProto* m2 = (CMissionCaseProto*)p2;

		return (m1->m_nIndex - m2->m_nIndex);
	}
};

struct MISSION
{
	char			step;
	CMissionCaseProto*	proto;
	MISSION()
	{
		step = 0;
		proto = NULL;
	};
};


class CMonsterCombo  
{
public:
	CMonsterCombo();
	virtual ~CMonsterCombo();

	int		m_nIndex;
	int		m_bIsBattle;		// 싸움여부
	int		m_nExtra;			// 진행중인 장소
	int		m_nStage;			// 진행중인 스테이지
	int		m_nRegenStep;		// 진행중인 regen
	char	m_cPlayerCount;		// 나누어서 리젠하려고

	int		m_pulseFirst;		// 1번째 이펙트
	int		m_pulseSecond;		// 2번째 이펙트
	
    int	    m_nTotalMission;
	char	m_cPlayerNum;		// stage에 있는 파티원수

	static int m_virtualIndex;

	// 이펙트 위치 저장
	CLCList<EFFECTPOS> m_listEffect;

	MISSION*	m_case;
	CArea*		m_area;
	CNPC*		m_effectNPC[MAXEFFECT];

	void		SetArea(CArea*	area);
	char		Run();
	void		DeleteAllMob();

	// 0: 리젠진행없음  1: 다음리젠진행		2: 오류
	char		NextNPCRegen();

	// 미션세팅
	bool		SetMission(int step, int index);

	// step: 스테이지번호
	const MISSION*	FindMission(int step);

	// 스테이지로 강제소환
	void RecallToStage();

	// 스테이지 시작
	bool StartStage();
	
	// 패널티 초기화
	void ClearPenalty();

	// 페널티 적용
	void ApplyPenalty(int skillindex, int skilllevel);

	//  선물상자 NPC 리젠
	void GiftMobRegen();

	//  미션케이스 총 포인트 계산
	int GetTotalPoint();

	// 
	int GetMaxCoinCount(int count)
	{
		int temp;
		temp = (int)(0.5f * count);

		float temp2 = (float)(0.5f * count);
		float temp3;
		temp3 = temp2 - (float)temp;

		if(temp3 >= 0.5) 
		{
			temp += 1;
		}

		temp += m_nStage/15;

		if(temp < 1)
			temp = 1;

		return temp;
	}

	// 불기둥 준비 이펙트
	void FireEffectReady();
	void FireEffectReady2();

	// 불기둥 실행 이펙트
	void FireEffectAct();

	// 입장료 계산
	int GetTotalNas();

	// 관리인 npc 리젠
	void ComboNPCRegen(/*char bContinue*/);

	// 다음 미션케이스 변수 설정
	void SetNextVar();

	// 변수 초기화
	void InitStageVar();

	// 초기화
	bool Init(int count);
	// 콤보 인덱스 생성
	void SetVirtualIndex();

	// 스테이지 알림
	void NoticeStage();

	// 콤보존에 있는 플레이어 로그 찍기
	void PrintCharIndex();

	void InitAllVar();

	bool SetMonsterCombo(CPC* pc);
};


#endif // !defined(AFX_MONSTERCOMBO_H__A0046A35_9FFA_4984_98CE_7C324A4237A6__INCLUDED_)
