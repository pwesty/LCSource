#ifndef __ROCKPAPER_SCISSORS_GAME_H__
#define __ROCKPAPER_SCISSORS_GAME_H__

#include "RockPaperScissorsProb.h"

class CRockPaperScissorsGame
{
public:
	CRockPaperScissorsGame(int rewardIdx, int probIdx);
	~CRockPaperScissorsGame(void);

	enum eWINNER
	{
		RESULT_WIN_USER		= 0,	// 유저 승리
		RESULT_DRAW			= 1,	// 무승부
		RESULT_WIN_SERVER	= 2,	// 서버 승리
	};

	/**
	* 게임 실행
	* param1 : _in_	user choice
	* param2 : _out_ server choice
	* return value : eWINNER
	**/
	int Play(CPC* user, const unsigned char userChoice, unsigned char& serverChoice);

	/**
	* 전체 스테이지 카운트
	* return value : 총 스테이지 수
	**/
	const int GetTotalStage(void) const;

	/**
	* 보상 지급
	* param1 : 보상 지급 유저
	* return value : true:성공, false:실패
	**/
	bool GiveReward(CPC* user);

	/**
	* 확률 변경
	* In Game 확률 변경
	* DB 확률 변경
	* param1 : 변경할 레벨
	* parma2 : 변경할 유저 승리 확률
	* param3 : 변경할 서버 승리 확률
	* param4 : 변경할 비길 확률
	* return value : true(성공), false(실패)
	**/
	bool ChangeProb(int level, unsigned char userProb, unsigned char serverProb, unsigned char drawProb);

	/**
	* 확률 정보 얻기
	* param1 : 얻어올 레벨
	* return value : 확률 정보
	**/
	const CRockPaperScissorsProb* GetProb(int level);

private:
	/**
	* 서버 선택값 구하기
	* param1 : 승자
	* param2 : 유저 선택값
	**/
	unsigned char GetServerChoice(const eWINNER winner, const unsigned char userChoice);

	/**
	* 확률 테이블 세팅하기
	* param1 : 확률 테이블 인덱스로 t_rps_prob의 a_system_idx 필드값
	**/
	void SetProbTable(void);

	/**
	* 확률 시스템 인덱스
	**/
	int m_probIdx;

	/** 보상 아이템 리스트
	* Reward DB Index
	* 보상 아이템은 승리 횟수에 따라서 결정된다.
	**/
	int m_rewardIdx;

	/**
	* 전체 스테이지 수
	**/
	int m_totalStageCount;

	/**
	* 승리 확률
	**/
	std::map<int, CRockPaperScissorsProb*> m_winProb;
};

#endif
