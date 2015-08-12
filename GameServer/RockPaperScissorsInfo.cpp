#include "stdhdrs.h"

#include "RockPaperScissorsInfo.h"

CRockPaperScissorsInfo::CRockPaperScissorsInfo(void)
	: m_nWinCount(0), m_nGameCount(0),  m_bStart(0), m_nTotalCount(0), m_bCanPlay(0)
{
}

CRockPaperScissorsInfo::~CRockPaperScissorsInfo(void)
{
}

void CRockPaperScissorsInfo::SetWinCount(char cnt)
{
	m_nWinCount = cnt;
}

char CRockPaperScissorsInfo::GetWinCount()
{
	return m_nWinCount;
}

void CRockPaperScissorsInfo::IncreaseWinCount()
{
	++m_nWinCount;
}

void CRockPaperScissorsInfo::SetGameCount(char cnt)
{
	m_nGameCount = cnt;
}

char CRockPaperScissorsInfo::GetGameCount()
{
	return m_nGameCount;
}

void CRockPaperScissorsInfo::IncreaseGameCount()
{
	++m_nGameCount;
}

void CRockPaperScissorsInfo::SetStart(bool bStart)
{
	m_bStart = bStart;
}

bool CRockPaperScissorsInfo::IsStart()
{
	return m_bStart;
}

void CRockPaperScissorsInfo::SetTotalCount(int cnt)
{
	m_nTotalCount = cnt;
}

int CRockPaperScissorsInfo::GetTotalCount()
{
	return m_nTotalCount;
}

void CRockPaperScissorsInfo::IncreaseTotalCount()
{
	++m_nTotalCount;
}

void CRockPaperScissorsInfo::SetCanPlay(bool bPlay)
{
	m_bCanPlay = bPlay;
}

bool CRockPaperScissorsInfo::CanPlay()
{
	return m_bCanPlay;
}

//