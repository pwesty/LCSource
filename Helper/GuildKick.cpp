#include "stdhdrs.h"

#include "GuildKick.h"
#include "Server.h"
#include "Log.h"

CGuildKick::CGuildKick(void)
	:m_masterKickStauts(GMKS_NORMAL)
	,m_masterKickRequestChar(0)
	,m_masterKickRequestTime(0)
{
}

CGuildKick::~CGuildKick(void)
{
}

void CGuildKick::setKickStatus(int _status)
{
	m_masterKickStauts = _status;
}

int CGuildKick::getKickStatus(void)
{
	return m_masterKickStauts;
}

void CGuildKick::setKickReuestChar(int _charIndex)
{
	m_masterKickRequestChar = _charIndex;
}

int CGuildKick::getKickRequestChar(void)
{
	return m_masterKickRequestChar;
}

void CGuildKick::setKickRequestTime(int _time)
{
	m_masterKickRequestTime = _time;
}

int CGuildKick::getKickRequestTime(void)
{
	return m_masterKickRequestTime;
}

