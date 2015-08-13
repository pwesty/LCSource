#include "stdhdrs.h"

#include "RockPaperScissorsProb.h"

CRockPaperScissorsProb::CRockPaperScissorsProb(void)
{
}

CRockPaperScissorsProb::CRockPaperScissorsProb(unsigned char userProb, unsigned char serverProb, unsigned char drawProb)
	: m_userWinProb(userProb), m_serverWinProb(serverProb), m_drawProb(drawProb)
{
}

CRockPaperScissorsProb::~CRockPaperScissorsProb(void)
{
}

void CRockPaperScissorsProb::SetUserWinProb(unsigned char prob)
{
	m_userWinProb = prob;
}

const unsigned char CRockPaperScissorsProb::GetUserWinProb(void) const
{
	return m_userWinProb;
}

void CRockPaperScissorsProb::SetServerWinProb(unsigned char prob)
{
	m_serverWinProb = prob;
}

const unsigned char CRockPaperScissorsProb::GetServerWinProb(void) const
{
	return m_serverWinProb;
}

void CRockPaperScissorsProb::SetDrawProb(unsigned char prob)
{
	m_drawProb = prob;
}

const unsigned char CRockPaperScissorsProb::GetDrawProb(void) const
{
	return m_drawProb;
}
