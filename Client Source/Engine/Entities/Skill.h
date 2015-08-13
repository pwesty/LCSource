#ifndef INCL_SKILL_H
#define INCL_SKILL_H
#ifdef PRAGMA_ONCE
#pragma once
#endif

///////////
// 효과상수

// 효과 서브타입 - 스탯
#define EST_STAT_ATTACK			0			// 공격
#define EST_STAT_DEFENSE		1			// 방어
#define EST_STAT_MAGIC			2			// 마법
#define EST_STAT_RESIST			3			// 저항
#define EST_STAT_HITRATE		4			// 명중
#define EST_STAT_AVOID			5			// 회피
#define EST_STAT_CRITICAL		6			// 크리
#define EST_STAT_ATTACKSPD		7			// 공속
#define EST_STAT_MAGICSPD		8			// 마속
#define EST_STAT_MOVESPD		9			// 이속
#define EST_STAT_RECOVERHP		10			// HP회복
#define EST_STAT_RECOVERMP		11			// MP회복
#define EST_STAT_MAXHP			12			// 최대HP 증가
#define EST_STAT_MAXMP			13			// 최대MP 증가

// 효과 서브타입 - 상태이상
#define EST_ASSIST_POISON		0			// 독
#define EST_ASSIST_HOLD			1			// 홀드
#define EST_ASSIST_CONFUSION	2			// 혼란
#define EST_ASSIST_STONE		3			// 석화
#define EST_ASSIST_SILENT		4			// 봉인
#define EST_ASSIST_BLOOD		5			// 출혈
#define EST_ASSIST_BLIND		6			// 블라인드
#define EST_ASSIST_STURN		7			// 스턴
#define EST_ASSIST_SLEEP		8			// 슬립
#define EST_ASSIST_HP			9			// HP회복
#define EST_ASSIST_MP			10			// MP회복
#define EST_ASSIST_MOVESPD		11			// 이속 향상
#define EST_ASSIST_HP_CANCEL	12			// HP변화(0, MAX에서 취소)
#define EST_ASSIST_MP_CANCEL	13			// MP변화(0, MAX에서 취소)
#define EST_ASSIST_DIZZY		14			// 현기증
#define EST_ASSIST_INVISIBLE	15			// 인비저블
#define EST_ASSIST_SLOTH		16			// 슬로스 : 공속하락
#define EST_ASSIST_FEAR			17			// 공포 : 해당 NPC가 도망
#define EST_ASSIST_FAKEDEATH	18			// 죽은척
#define EST_ASSIST_PERFECTBODY	19			// 타이탄 퍼펙 바디
#define EST_ASSIST_FRENZY		20			// 타이탄 프렌지
#define EST_ASSIST_DAMAGELINK	21			// 데미지링크
#define EST_ASSIST_BERSERK		22			// 그랜드 몬스터의 분노- 공속
#define EST_ASSIST_DESPAIR		23			// 힐러 디세이어 마크
#define EST_ASSIST_MANA_SCREEN	24			// 소서러 마나 스크린
#define EST_ASSIST_BLESS		25			// 소서러 윈즈 블레싱
#define EST_ASSIST_SAFEGUARD	26			// 보호망(전투불가능)
#define EST_ASSIST_MANTLE		27			// 망토
#define EST_ASSIST_GUARD		28			// 경비병 소환
#define EST_ASSIST_CHARGEATC	29			// 장탄(attack)
#define EST_ASSIST_CHARGEMGC	30			// 장탄(magic)
#define EST_ASSIST_DISEASE		31			// 질병
#define EST_ASSIST_CURSE		32			// 저주
#define EST_ASSIST_CONFUSED		33			// 몬스터만 혼돈에 걸리게 함
#define EST_ASSIST_TAMING		34			// 몬스터를 일정시간 아군으로 만듬
#define EST_ASSIST_FREEZE		35			// 결계(서로 공격 못함, 걸린 것은 이동 못함)
#define EST_ASSIST_INVERSE_DAMAGE	36		// 피격 당하면 데미지 만큼 HP 회복
#define EST_ASSIST_HP_DOT		37			// 지속 데미지 입음
#define EST_ASSIST_REBIRTH		38			// 부활
#define EST_ASSIST_DARKNESS_MODE	39		// 다크니스 모드
#define EST_ASSIST_AURA_DARKNESS	40		// 오오라 - 다크니스
#define EST_ASSIST_AURA_WEAKNESS	41		// 오오라 - 위크니스
#define EST_ASSIST_AURA_ILLUSION	42		// 오오라 - 일루젼
#define EST_ASSIST_MERCENARY		43		// 몬스터 용병
#define EST_ASSIST_SOUL_TOTEM_BUFF	44		// 버프형 토템
#define EST_ASSIST_SOUL_TOTEM_ATTK	45		// 공격형 토템
#define EST_ASSIST_TRAP				46		// 트랩
#define EST_ASSIST_PARASITE			47		// 기생충 감염 상태
#define EST_ASSIST_SUICIDE			48		// 자살 상태
#define EST_ASSIST_INVINCIBILITY	49		// 무적 모드
#define EST_ASSIST_GPS				50		// GPS
#define EST_ASSIST_COUNT			51

// 효과 서브타입 - 공격
#define EST_ATTACK_NORMAL		0			// 일반 공격

// 효과 서브타입 - 회복
#define EST_RECOVER_HP			0			// HP회복
#define EST_RECOVER_MP			1			// MP회복

// 효과 서브타입 - 치유
#define EST_CURE_POISON			0			// 독
#define EST_CURE_HOLD			1			// 홀드
#define EST_CURE_CONFUSION		2			// 혼란
#define EST_CURE_HURT			3			// 부상
#define EST_CURE_SEAL			4			// 봉인
#define EST_CURE_BLOOD			5			// 출혈
#define EST_CURE_REBIRTH		6			// 부활
#define EST_CURE_INVISIBLE		7			// 인비저블 상태를 해제
#define EST_CURE_STURN			8			// 스턴 해제
#define EST_CURE_SLOTH			9			// 슬로스 해제

// 효과 서브타입 - 그외
#define EST_OTHER_INSTANTDEATH	0			// 즉사
#define EST_OTHER_SKILLCANCEL	1			// 스킬 시전 취소
#define EST_OTHER_TACKLE		2			// 태클
#define EST_OTHER_TACKLE2		3			// 태클2
#define EST_OTHER_REFLEX		4			// 반사 대미지 (일반 공격, 25m내)

// 대미지 타입
#define EDT_ONLYPOWER			0			// 스킬 위력만
#define EDT_ADDITION			1			// 스탯에 덧셈
#define EDT_RATE				2			// 스탯에 비율로 곱셈

// 명중 타입
#define EHT_CONSTANT			0			// 고정
#define EHT_VARIABLE			1			// 수식

// 지속 타입
#define ETT_IMMEDIATE			0			// 일시
#define ETT_DURABLE				1			// 지속

////////////
// 스킬 상수

// 사용/적용 조건
#define SCT_DEATH				(1 << 0)	// 사망
#define SCT_WEAPON				(1 << 1)	// 무기장비
#define SCT_SITDOWN				(1 << 2)	// 앉기
#define SCT_PEACEZONE			(1 << 3)	// 평화지역
#define SCT_SHIELD				(1 << 4)	// 실드장비
#define SCT_STAND				(1 << 5)	// 서기
#define SCT_DARKNESS			(1 << 6)	// 사도모드
#define SCT_NOCOOLTIME			(1 << 7)	// [2012/11/20 : Sora] 쿨타임 감소 영항 받지 않음

// 플래그
#define	SF_SINGLEMODE			(1 << 0)	// 싱글모드
#define	SF_FORHELP				(1 << 1)	// 도움을 주는 스킬
#define	SF_NOTHELP				(1 << 2)	// 해꼬지 스킬
#define SF_ABSTIME				(1 << 3) // 060227 : bs : 절대시간 사용
#define SF_NOTDUPLICATE			(1 << 4) // 060227 : bs : 중복 사용 불가 : 중복되는 스킬이 있으면 레벨에 관계없이 사용이 안됨
#define SF_NOCANCEL				(1 << 5) // 다른 스킬/사망에 의해 취소 안됨
#define SF_COMBO				(1 << 6) // 시간상관없이 적용됨(접속종료, 공간이동, 죽었을 때 사라짐)
#define SF_LOGOUT				(1 << 7) // 접속종료 시 사라짐
#define SF_NS_DARKNESS			(1 << 8)    // 나이트 쉐도우 사도 모드에서만 사용하는 스킬인지 확인
#define SF_GUILD				(1 << 9)	// 스킬이 길드스킬인지 체크 하는 flag
#define SF_INFINITE				(1 << 10)	// 무한스킬 - 스킬 시간이 만료되면 다시 스킬이 적용됨
#define SF_ZONE					(1 << 11)	// 존이동시 버프 삭제
#define SF_SUMMON_NPC			(1 << 12)	// 캐릭터가 소환한 토템, 트랩이 사용하는 스킬
#define SF_ITEM_SPECIAL_SKILL	(1 << 14)	// 아이템에 옵션으로 붙어 있는 스킬.
#define SF_NOT_SINGLE			(1 << 16)	// 싱글 던전 사용 불가 스킬.
#define SF_TOGGLE				(1 << 17)	// 토글형 스킬.

#define AST_SLOTH				(1 << MST_ASSIST_SLOTH)			// 슬로스(공속하락)

// 소서러 플래그.
#define SSF_APP_CHARACTER		(1 << 0)	// 캐릭터
#define SSF_APP_FIRE			(1 << 1)	// 불의정령
#define SSF_APP_WIND			(1 << 2)	// 바람의정령
#define SSF_APP_EARTH			(1 << 3)	// 대지의정령
#define SSF_APP_WATER			(1 << 4)	// 물의정령
#define SSF_APP_HELLOUND		(1 << 5)	// 헬운드
#define SSF_APP_ELENEN			(1 << 6)	// 엘레넨
#define SSF_APP_ALL				(SSF_APP_CHARACTER | SSF_APP_FIRE | SSF_APP_WIND | SSF_APP_EARTH | SSF_APP_WATER | SSF_APP_HELLOUND | SSF_APP_ELENEN)
#define SSF_USE_CHARACTER		(1 << 16)	// 캐릭터
#define SSF_USE_FIRE			(1 << 17)	// 불의정령
#define SSF_USE_WIND			(1 << 18)	// 바람의정령
#define SSF_USE_EARTH			(1 << 19)	// 대지의정령
#define SSF_USE_WATER			(1 << 20)	// 물의정령
#define SSF_USE_HELLOUND		(1 << 21)	// 헬운드
#define SSF_USE_ELENEN			(1 << 22)	// 엘레넨
#define SSF_USE_ALL				(SSF_USE_CHARACTER | SSF_USE_FIRE | SSF_USE_WIND | SSF_USE_EARTH | SSF_USE_WATER | SSF_USE_HELLOUND | SSF_USE_ELENEN)

#define WEAPON_COUNT 2

#include <vector>
class  CSkill
{
public:
	struct _SkillData
	{
		std::string	name;						// 이름
		std::string	client_description;
		std::string client_tooltip;

		int		index;							// 스킬 번호
		int		job;							// 직업 플래그
		int		job2;							// 전직 직업
		int		petindex;						// 공격 펫 인덱스		
		char	type;							// 종류
		int		flag;							// 플래그
		int		sorcerer;						// 소서러 전용 플래그
		char	curLevel;						// WSS_NEW_GUILD_SYSTEM 070716
		char	maxLevel;						// 최대 레벨

		// 거리
		float	appRange;						// 범위 적용 반경
		float	fireRange;						// 사정거리
		float	fireRange2;						// 사정거리 2

		// 타겟
		char	targetType;						// 타겟팅 종류

		// 사용조건
		int		useState;						// 사용조건1
		int		useWeaponType0;					// 사용무기서브타입 1
		int		useWeaponType1;					// 사용무기서브타입 2
		int		useMagicIndex1;					// 사용조건2 - 버프번호1
		char	useMagicLevel1;					// 사용조건2 - 버프레벨1
		int		useMagicIndex2;					// 사용조건2 - 버프번호2
		char	useMagicLevel2;					// 사용조건2 - 버프레벨2
		int		useMagicIndex3;					// 사용조건2 - 버프번호3
		char	useMagicLevel3;					// 사용조건2 - 버프레벨3
		int		useSoulCount;					// 사용조건 (영혼의 갯수 조건)

		// 적용조건
		int		appState;						// 적용조건1 - 캐릭터 상태

		// 시간
		int		readyTime;						// 시전시간
		int		waitTime;						// 시전시간
		int		fireTime;						// 발사시간
		int		reuseTime;						// 재사용시간

		bool	bToggle;						// 토글스킬 활성화 여부.
		
		struct sClientSkillData
		{
			// 시전
			char	readyAni[256];
			char	readyEffect1[256];

			// 정지
			char	stillAni[256];

			// 발사
			char	fireAni[256];
			char	fireEffect1[256];
			char	fireEffect2[256];
			char	fireEffect3[256];

			// 발사체
			char	fireobjType;
			float	fireobjSpeed;
			float	fireobjX;
			float	fireobjZ;
			float	fireobjH;
			char	fireobjCoord;
			char	fireobjDelayCount;
			float	fireobjDelay[4];
			float	fireobjDestDelay;
		};
		sClientSkillData client[WEAPON_COUNT];

		char	After_AttachEffect[256];		

		// 아이콘
		int		client_icon_texid;
		int		client_icon_row;
		int		client_icon_col;

		DOUBLE	Skill_StartTime;

		int		transFlag;	//[sora] 미번역 스트링 index 표시		
	};

	_SkillData				Skill_Data;

	struct _SkillLevel
	{
		// 시전 필요 조건
		int needHP;						// 소모HP
		int needMP;						// 소모 MP
		int needGP;						// 소모 GP

		int	durtime;					// 지속시간
		int dummyPower;					// 출력용 위력
		int needItemIndex1;					// 소모아이템 번호1
		int needItemCount1;					// 소모아이템 개수1
		int needItemIndex2;					// 소모아이템 번호2
		int needItemCount2;					// 소모아이템 개수2

		// 습득 조건
		int learnLevel;						// 습득조건 - 레벨
		int learnSP;						// 습득조건 - SP
		int learnGP;						// 습득조건 - GP //WSS_NEW_GUILD_SYSTEM 070716
		int learnSkillIndex[3];				// 습득조건 - 스킬번호
		char learnSkillLevel[3];			// 습득조건 - 스킬레벨
		int learnItemIndex[3];
		int learnItemCount[3];
		int learnStr;						// 습득조건 - 힘
		int learnDex;						// 습득조건 - 민첩 
		int learnInt;						// 습득조건 - 지혜	
		int learnCon;						// 습득조건 - 체질

		// 적용 조건
		int appMagicIndex1;		// 적용조건2 - 효과번호1
		char appMagicLevel1;	// 적용조건2 - 효과레벨1
		int appMagicIndex2;		// 적용조건2 - 효과번호2
		char appMagicLevel2;	// 적용조건2 - 효과레벨2
		int appMagicIndex3;		// 적용조건2 - 효과번호3
		char appMagicLevel3;	// 적용조건2 - 효과레벨3

		// 스킬 효과
		int magicIndex1;		// 스킬효과 번호
		char magicLevel1;		// 스킬효과 레벨
		int magicIndex2;		// 스킬효과 번호
		char magicLevel2;		// 스킬효과 레벨
		int magicIndex3;		// 스킬효과 번호
		char magicLevel3;		// 스킬효과 레벨

		// 속성 시스템 스킬 LOD에 속성정보 추가 [1/21/2013 Ranma]
		char attratt;
		char attrattLv;
		char attrdef;
		char attrdefLv;
		int  targetmax;
	};
	
	std::vector<_SkillLevel>			m_vectorSkillLevels;
	BOOL								bCanCancel;
	BOOL								bNeedTarget;

	INDEX	idPlayer_Anim_Skill[WEAPON_COUNT][3];

	enum	SKILLTYPE
	{
		ST_MELEE				= 0,			// 근접
		ST_RANGE				= 1,			// 원거리
		ST_MAGIC				= 2,			// 마법
		ST_PASSIVE				= 3,			// 패시브
		ST_PET_COMMAND			= 4,			// 팻 액션
		ST_PET_SKILL_PASSIVE	= 5,			// 팻 스킬 
		ST_PET_SKILL_ACTIVE		= 6,			// 팻
		ST_GUILD_SKILL_PASSIVE	= 7,			// 길드 스킬 패시브 // WSS_NEW_GUILD_SYSTEM 070716
		ST_SEAL					= 8,
		ST_SUMMON_TOTEM_SKILL	= 9,			// 토템 소환 스킬
	};

	enum	SKILLPROPERTY
	{		
		// 속성 상수
		AT_NONE		= 0,			// 무
		AT_FIRE		= 1,			// 화
		AT_WATER	= 2,			// 수
		AT_EARTH	= 3,			// 지
		AT_WIND		= 4,			// 풍
		AT_DARK		= 5,			// 암
		AT_LIGHT	= 6,			// 광
	};

	enum	SKILLTARGET	//Skill_TargetType
	{		
		STT_SELF_ONE			= 0,			// 셀프
		STT_SELF_RANGE			= 1,			// 셀프 범위
		STT_TARGET_ONE			= 2,			// 타겟
		STT_TARGET_RANGE		= 3,			// 타겟 범위
		STT_PARTY_ONE			= 4,			// 파티 1인
		STT_PARTY_ALL			= 5,			// 파티 전체
		STT_TARGET_D120			= 6,			// 타겟 120도
		STT_TARGET_RECT			= 7,			// 타겟 직사각
		STT_GUILD_ALL			= 9,			// 길드전체에게 주는 스킬
		STT_GUILD_ONE			= 10,			// 길드한명에게 주는 스킬
		STT_GUILD_SELF_RANGE	= 11,			// 시전자 주위 길드원에게 주는 스킬
		STT_GUILD_MEMBER_SELF	= 12,			// 길드원 전체 사용 가능 스킬
		/*
		STT_SELF				= 0,			// 셀프
		STT_SELFRANGE_ENEMY		= 1,			// 셀프 범위(적)
		STT_TARGET_FRIEND		= 3,			// 대상(아군)
		STT_TARGET_ENEMY		= 4,			// 대상(적)
		STT_TARGET_ENEMY_LOW	= 5,			// 대상(적 & 저렙)
		STT_TARGETRANGE_ENEMY	= 6,			// 대상범위(적)
		STT_PARTY				= 7,			// 파티
		*/
	};

	enum	STYLE
	{
		Damage      = 0,
		TACKLE		= 1,
		STERN		= 2,
		HOLD		= 3,
		SLEEP		= 4,
		ABILITY		= 5,
		USE_PC		= 6,
	};

	enum	CANUSESTATE
	{
		RUN         = 0,
		STAND	    = 1,
		SIT_DOWN    = 2,
		AFTERNOON   = 3,
		NIGHT		= 4,
		DEATH		= 5,
		SKILLING	= 6,
		SET_WEAPON	= 7,
		OFF_WEAPON	= 8,
		IN_WATER	= 9,
	};

//안태훈 수정 시작	//(Open beta)(2004-12-03)
	//for client only
	enum MISSILE_TYPE
	{
		MT_NONE			= 0,//근접공격.
		MT_ARROW		= 1,//일반 화살이 있고 추가로 Effect가 붙는 형식.
		MT_DIRECT		= 2,//발사체가 없는 마법. 데미지를 주지 않는다.
		MT_CONTINUE		= 3,//안보이는 발사체에 Effect만 붙는 형식. 단, 전의 이펙트에서 발사체의 이펙트가 이어짐.
		MT_INVISIBLE	= 4,//안보이는 발사체에 Effect만 붙는 형식, 이어지지 않음.
		MT_MAGIC		= 5,//발사체가 있는데 시전자에서 시작되지 않음.(ex:미티어 스트라이크)
		MT_INVERT		= 6,//역으로 시전자쪽으로 뭔가 날라옴. Only Effect.
		MT_MAGECUTTER	= 7,//메이지 스킬 중 커터 날라가는것.
		MT_DIRECTDAMAGE	= 8,//발사체가 없는 마법. 데미지를 준다.
		MT_NOTHING		= 9,//마지막 이펙트 없음. Network에서 받아서만 처리하는 스킬임. ex>파티힐같은 경우.
		MT_GOWAITBACK	= 10,//발사체 안보임. 목표로 갔다가 일정시간 후 다시 돌아옴.
							 //타격 이펙트는 목표에 도달시 나옴. 타격이펙트에 '(다시)를 붙인 이펙트는 돌아왔을때 나옴.
		MT_CONNECT		= 11,//발사체 안보임. 목표와 시전자를 mdl이펙트로 연결함.(데미지 링크)
		MT_FALLDOWN		= 12,//발사체 안보임. 하늘에서 떨어짐. 땅에 레이를 쏴서 그 포지션에 도달시 타격이펙트 나옴.
		MT_DASH			= 13,//안보이는 발사체에 Effect만 붙는 형식. 캐릭터가 사라지고, HIT시 다시 나타 남(Worp) :: 나이트( 스킬:대쉬 )
	};
//안태훈 수정 끝	//(Open beta)(2004-12-03)

public:
	/* Default constructor. */
	CSkill(void);
	/* Destructor. */
	~CSkill(void);

	void Init(void);

	inline int GetIndex()		const
	{	return Skill_Data.index;	}

	inline int GetJob()			const
	{	return Skill_Data.job;	}

	inline int GetJob2()		const
	{	return Skill_Data.job2; }

	inline int GetWildpetIndex() const
	{	return Skill_Data.petindex;	}

	inline const char* GetName()		const
	{	return Skill_Data.name.c_str();	}

	inline int GetType()			const
	{	return Skill_Data.type;	}

	inline int GetFlag()		const
	{	return Skill_Data.flag;	}

	inline int GetSorcererFlag()	const
	{	return Skill_Data.sorcerer;	}

	inline int GetUsingSorcererFlag()	const
	{	return ( ( Skill_Data.sorcerer & 0xFFFF0000 ) >> 16 ); };

	inline int GetApplicationSorcererFlag()	const
	{	return ( ( Skill_Data.sorcerer & 0xFFFF ) ); };

	inline char GetTargetType()		const
	{	return Skill_Data.targetType;	}

	// 사용무기서브타입 1
	inline int GetUseWeaponType0()	const			
	{	return Skill_Data.useWeaponType0;	}

	// 사용무기서브타입 2
	inline int GetUseWeaponType1()	const
	{	return Skill_Data.useWeaponType1;	}

	// 최대 타겟 수
	inline int GetTargetNum(int CurLv)	const
	{	
		if (CurLv - 1 < 0 || CurLv - 1 >= m_vectorSkillLevels.size())
			return 0;
		return m_vectorSkillLevels[CurLv-1].targetmax;
	}
	// 현재레벨
	inline int GetCurLevel()	const
	{	return Skill_Data.curLevel;	}
	// 최대레벨
	inline int GetMaxLevel()	const
	{	return Skill_Data.maxLevel;	}

	inline float GetAppRange()		const
	{	return Skill_Data.appRange;	}

	inline float GetFireRange()		const
	{	return Skill_Data.fireRange;	}
	
	inline float GetFireRange2()	const
	{	return Skill_Data.fireRange2;	}

	inline int GetReadyTime()		const
	{	return Skill_Data.readyTime;	}

	inline int GetFireTime()		const
	{	return Skill_Data.fireTime;	}

	inline int GetReUseTime()		const
	{	return Skill_Data.reuseTime;	}

	inline float GetMissileSpeed(INDEX iWeapon)	const
	{	return Skill_Data.client[iWeapon].fireobjSpeed;	}

	inline char GetDelayCount(INDEX iWeapon)		const
	{	return Skill_Data.client[iWeapon].fireobjDelayCount;	}

	inline char GetDestDelay(INDEX iWeapon)		const
	{	return Skill_Data.client[iWeapon].fireobjDestDelay;	}

	inline float GetDelay(int i, INDEX iWeapon)		const
	{	return Skill_Data.client[iWeapon].fireobjDelay[i];	}

	inline MISSILE_TYPE GetMissileType(INDEX iWeapon)	const
	{	return (MISSILE_TYPE)(int)Skill_Data.client[iWeapon].fireobjType;	}

	inline bool IsSingleDungeonSkill() const
	{	return Skill_Data.flag & SF_SINGLEMODE;		}

	inline const char*	GetReadyAnim(INDEX iWeapon)	const
	{	return Skill_Data.client[iWeapon].readyAni;	}

	inline const char*	GetStillAnim(INDEX iWeapon)	const
	{	return Skill_Data.client[iWeapon].stillAni;	}

	inline BOOL IsNeedTarget() const
	{	return bNeedTarget;		}

	inline const char*	GetFireAnim(INDEX iWeapon)	const
	{	return Skill_Data.client[iWeapon].fireAni;	}

	inline const char* GetReadyEffect(INDEX iWeapon)	const
	{
		return Skill_Data.client[iWeapon].readyEffect1;
	}

	inline const char* GetFireEffect1(INDEX iWeapon)	const
	{
		return Skill_Data.client[iWeapon].fireEffect1;
	}

	inline const char* GetFireEffect2(INDEX iWeapon)	const
	{
		return Skill_Data.client[iWeapon].fireEffect2;
	}

	inline const char* GetFireEffect3(INDEX iWeapon)	const
	{
		return Skill_Data.client[iWeapon].fireEffect3;
	}

	inline const char* GetAfter_AttachEffect() const
	{
		return Skill_Data.After_AttachEffect;
	}

	inline const char* GetDescription()		const
	{	return Skill_Data.client_description.c_str();	}

	inline const char* GetToolTip()		const
	{	return Skill_Data.client_tooltip.c_str();	}

	inline int GetNeedMP(int iLevel)	const
	{	return m_vectorSkillLevels[iLevel].needMP;	}

	inline int GetNeedHP(int iLevel)	const
	{	return m_vectorSkillLevels[iLevel].needHP;	}

	inline int GetNeedItemIndex1(int iLevel) const
	{	return m_vectorSkillLevels[iLevel].needItemIndex1;	}

	inline int GetNeedItemIndex2(int iLevel) const
	{	return m_vectorSkillLevels[iLevel].needItemIndex2;	}

	inline int GetLearnLevel(int iLevel)	const
	{	return m_vectorSkillLevels[iLevel].learnLevel;	}

	inline int GetLearnSP(int iLevel)	const
	{	return m_vectorSkillLevels[iLevel].learnSP;	}
	// WSS_NEW_GUILD_SYSTEM 070716 ------------------->>
	inline int GetLearnGP(int iLevel)	const
	{	return m_vectorSkillLevels[iLevel].learnGP;	}
	// -----------------------------------------------<<
	inline int GetLearnStr(int iLevel) const
	{	return m_vectorSkillLevels[iLevel].learnStr;	}

	inline int GetLearnDex(int iLevel) const
	{	return m_vectorSkillLevels[iLevel].learnDex;	}

	inline int GetLearnInt(int iLevel) const
	{	return m_vectorSkillLevels[iLevel].learnInt;	}		
	
	inline int GetLearnCon(int iLevel) const
	{	return m_vectorSkillLevels[iLevel].learnCon;	}

	inline int GetDurTime(int iLevel)	const
	{	return m_vectorSkillLevels[iLevel].durtime;	}

	inline int GetPower(int iLevel)	const
	{	return m_vectorSkillLevels[iLevel].dummyPower; }

	inline int GetLearnSkillIndex( int iLevel, int iIndex ) const
	{
		return m_vectorSkillLevels[iLevel].learnSkillIndex[iIndex];
	}
	inline SBYTE	GetLearnSkillLevel( int iLevel, int iIndex ) const
	{
		return m_vectorSkillLevels[iLevel].learnSkillLevel[iIndex];
	}
	inline int	GetLearnItemIndex( int iLevel, int iIndex ) const
	{
		return m_vectorSkillLevels[iLevel].learnItemIndex[iIndex];
	}
	inline int	GetLearnItemCount( int iLevel, int iIndex ) const
	{
		return m_vectorSkillLevels[iLevel].learnItemCount[iIndex];
	}

	inline int	GetIconTexID() const	{ return Skill_Data.client_icon_texid; }
	inline int	GetIconTexRow() const	{ return Skill_Data.client_icon_row; }
	inline int	GetIconTexCol() const	{ return Skill_Data.client_icon_col; }

	ENGINE_API void	SetStartTime( int nRemainTime = 0 );			// yjpark
	ENGINE_API void ResetStartTime();
	
	void SetStartSkillDelay(int nSkillIndex);

	// WSS_NEW_GUILD_SYSTEM 070716 ------------------->>
	inline void SetCurLevel(int iLevel)	
	{ Skill_Data.curLevel = iLevel;	}
	// -----------------------------------------------<<

	inline int GetNeedGP(int iLevel)	const
	{	return m_vectorSkillLevels[iLevel].needGP;	}
	inline int GetNeedItemCount1(int iLevel) const
	{
		return m_vectorSkillLevels[iLevel].needItemCount1;
	}
	inline int GetNeedItemCount2(int iLevel) const
	{
		return m_vectorSkillLevels[iLevel].needItemCount2;
	}


	// 속성 시스템 스킬 속성 정보 얻기 [1/21/2013 Ranma]
	inline char GetAttrAtt(int iLevel)	const
	{
		if (iLevel < 0)
			return 0;

		return m_vectorSkillLevels[iLevel].attratt;	
	}

	inline char GetAttrAttLv(int iLevel)	const
	{
		if (iLevel < 0)
			return 0;

		return m_vectorSkillLevels[iLevel].attrattLv;
	}

	inline char GetAttrDef(int iLevel)	const
	{
		if (iLevel < 0)
			return 0;

		return m_vectorSkillLevels[iLevel].attrdef;	
	}

	inline char GetAttrDefLv(int iLevel)	const
	{
		if (iLevel < 0)
			return 0;

		return m_vectorSkillLevels[iLevel].attrdefLv;	
	}

	//[sora] 미번역 스트링 index 표시
	void SetNoTranslate();
	void ClearNoTranslate();

	void SetName(const char* str)			{ Skill_Data.name = str; }
	void SetDescription(const char* str)	{ Skill_Data.client_description = str; }
	void SetTooltip(const char* str)		{ Skill_Data.client_tooltip = str; }

	int& GetTransFlag()						{ return Skill_Data.transFlag; }
	void SetToggle(bool bToggle)			{ Skill_Data.bToggle = bToggle;	}
	bool GetToggle()						{ return Skill_Data.bToggle;	}

	static int LoadSkillDataFromFile(CStaticArray<CSkill> &apSkillData, const char* FileName);
};

#endif