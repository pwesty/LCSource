#include "stdhdrs.h"

#include "RockPaperScissorsGame.h"
#include "Server.h"
#include "../ShareLib/DBCmd.h"

CRockPaperScissorsGame::CRockPaperScissorsGame(int rewardIdx, int probIdx)
	: m_probIdx(probIdx), m_rewardIdx(rewardIdx), m_totalStageCount(0)
{
	SetProbTable();
}

CRockPaperScissorsGame::~CRockPaperScissorsGame(void)
{
}

int CRockPaperScissorsGame::Play(CPC* user, const unsigned char userChoice, unsigned char& serverChoice)
{
	if (user == NULL || static_cast<int>(m_winProb.size()) <= 0)
		return -1;

	// 가위바위보 게임을 진행해 봅시다. 결과값은 Result 변수에 저장됩니다.
	eWINNER Resut;

	// 1. 현재 이긴 횟수를 구합시다. 왜냐면, 이긴 횟수에 따라서 이길 확률이 달라지니까요.
	int WinCount = static_cast<int>(user->m_RockPaperScissorsInfo.GetWinCount());
	CRockPaperScissorsProb* probTable = m_winProb[WinCount+1];

	if (probTable == NULL)
		return -1;

	// 2. 이번엔 이길 확률을 구해봅시다.
	int WinProb = probTable->GetUserWinProb();

	// 3. 질 확률을 구해봅시다.
	int LoseProb = probTable->GetServerWinProb();

	// 4. 그 외에는 비기는 경우입니다.

	// 5. 승패를 결정할 값을 랜덤으로 정해봅시다.
	int value = GetRandom(1, 100);

	if (user->getRockPaperScissorsFlag() && user->m_admin == 10)
	{
		WinProb = 100;
		LoseProb = 1;
		value = 1;
	}

	// 6. 승패를 확인해 봅시다.
	if (WinProb >= value)
	{
		// 유저가 이겼습니다.
		user->m_RockPaperScissorsInfo.IncreaseWinCount();
		Resut = RESULT_WIN_USER;
	}
	else if (LoseProb+WinProb <= value)
	{
		// 서버가 이겼습니다.
		user->m_RockPaperScissorsInfo.SetStart(false);
		Resut = RESULT_WIN_SERVER;
	}
	else
	{
		// 비겼습니다.
		Resut = RESULT_DRAW;
		serverChoice = userChoice;
	}

	// 결과 메시지 보내자.
	serverChoice = GetServerChoice(Resut, userChoice);
	return static_cast<int>(Resut);
}

const int CRockPaperScissorsGame::GetTotalStage(void) const
{
	return m_totalStageCount;
}

bool CRockPaperScissorsGame::GiveReward(CPC* user)
{
	if (user == NULL || m_rewardIdx < 0 || user->m_RockPaperScissorsInfo.GetGameCount() < 1)
	{
		return false;
	}

	int winCount = static_cast<int>(user->m_RockPaperScissorsInfo.GetWinCount());

	if( !gserver->m_rewardMgr.setRewardData( user, m_rewardIdx, -1, winCount ) || !gserver->m_rewardMgr.doReward(user, m_rewardIdx) )
	{
		// 아이템 지급 실패
		return false;
	}

	// 아이템 지급 성공 메시지
	return true;
}

unsigned char CRockPaperScissorsGame::GetServerChoice(const eWINNER winner, const unsigned char userChoice)
{
	unsigned char serverChoice;
	switch (winner)
	{
	case RESULT_WIN_USER:
		{
			if (userChoice == 0)	// 유저가 가위라면
				serverChoice = 2;	// 서버는 보
			else
				serverChoice = userChoice - 1;
		}
		break;
	case RESULT_WIN_SERVER:
		{
			if (userChoice == 2)	// 유저가 보라면
				serverChoice = 0;	// 서버는 가위
			else
				serverChoice = userChoice + 1;
		}
		break;
	case RESULT_DRAW:
		{
			serverChoice = userChoice;
		}
		break;
	}

	return serverChoice;
}

void CRockPaperScissorsGame::SetProbTable(void)
{
	CDBCmd cmd;
	cmd.Init(&gserver->m_dbcharingame);

	CLCString query(1024);
	query.Format("SELECT * FROM t_rps_prob WHERE a_system_idx = %d", m_probIdx);

	cmd.SetQuery(query);
	if (!cmd.Open() || cmd.m_nrecords == 0)
		return;

	m_totalStageCount = cmd.m_nrecords;

	while (cmd.MoveNext())
	{
		int level=0;
		unsigned char userProb=0, serverProb=0, drawProb=0;

		cmd.GetRec("a_level", level);
		cmd.GetRec("a_userWinProb", userProb);
		cmd.GetRec("a_serverWinProb", serverProb);
		cmd.GetRec("a_drawProb", drawProb);

		CRockPaperScissorsProb* prob = new CRockPaperScissorsProb(userProb, serverProb, drawProb);
		m_winProb[level] = prob;
	}
}

bool CRockPaperScissorsGame::ChangeProb(int level, unsigned char userProb, unsigned char serverProb, unsigned char drawProb)
{
	if (userProb + serverProb + drawProb != 100)
		return false;

	CRockPaperScissorsProb* probTable = m_winProb[level];
	if (probTable == NULL)
		return false;

	probTable->SetUserWinProb(userProb);
	probTable->SetServerWinProb(serverProb);
	probTable->SetDrawProb(drawProb);

	CDBCmd cmd;
	cmd.Init(&gserver->m_dbcharingame);

	CLCString query(1024);
	query.Format("UPDATE t_rps_prob SET a_userWinProb = %d, a_serverWinProb = %d, a_drawProb = %d WHERE a_system_idx = %d and a_level = %d"
				 , userProb, serverProb, drawProb, m_probIdx, level);

	cmd.SetQuery(query);
	if( !cmd.Update() )
		return false;

	return true;
}

const CRockPaperScissorsProb* CRockPaperScissorsGame::GetProb(int level)
{
	return m_winProb[level];
}
