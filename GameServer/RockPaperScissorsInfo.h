#ifndef __ROCK_PAPER_SCISSORS_INFO_H__
#define __ROCK_PAPER_SCISSORS_INFO_H__

class CRockPaperScissorsInfo
{
public:
	CRockPaperScissorsInfo();
	~CRockPaperScissorsInfo();

	/**
	* 시작 설정
	**/
	void SetWinCount(char cnt);
	char GetWinCount();
	void IncreaseWinCount();

	void SetGameCount(char cnt);
	char GetGameCount();
	void IncreaseGameCount();

	void SetStart(bool bStart);
	bool IsStart();

	void SetTotalCount(int cnt);
	int GetTotalCount();
	void IncreaseTotalCount();

	void SetCanPlay(bool bPlay);
	bool CanPlay();

private:
	char    m_nWinCount;				// 누적 승수
	char    m_nGameCount;				// 현재 게임 가위바위보 시도 수
	bool    m_bStart;					// 게임 시작 여부
	int     m_nTotalCount;				// 게임 누적 수
	bool    m_bCanPlay;					// 선택 가능 여부
};

#endif
