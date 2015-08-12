#ifndef __JUMPING_SYSTEM_H__
#define __JUMPING_SYSTEM_H__

class CJumpingSystem
{
public:
	CJumpingSystem(CPC* _ch, int _itemIndex);
	~CJumpingSystem(void);

	// 점핑할 수 있는지, 아이템을 지급받을 수 있는지 조건 검사
	bool check(void);

	// 점핑
	bool jump(void);

private:
	// 캐릭터 초기화
	bool initCharacter(void);

	// 직업 초기화
	bool initJob(void);

	// 스킬 초기화
	bool initSkill(void);

	// 스탯 초기화
	bool initStat(void);

	// 보상 지급
	bool giveReward(void);

private:
	CPC* m_pc;					// 점핑 캐릭터
	const int m_itemIndex;		// 점핑에 사용한 아이템 인덱스
	int m_jumpingLevel;			// 점핑 레벨
	int m_rewardIndex;			// 보상 테이블 인덱스
};


#endif
