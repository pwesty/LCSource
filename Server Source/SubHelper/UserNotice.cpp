#include "UserNotice.h"

#if defined (NOTICE_SYSTEM)

CUserNotice::CUserNotice(void)
	: m_viewCount(0), m_charindex(0)
{
	memset( m_name, 0x00, sizeof(m_name) );
	memset( m_contents, 0x00, sizeof(m_contents) );
}

CUserNotice::~CUserNotice(void)
{
}

void CUserNotice::setName(const char* _name)
{
	memset( m_name, 0x00, sizeof(m_name) );
	memcpy( m_name, _name, strlen(_name) );
}

char* CUserNotice::getName(void)
{
	return m_name;
}

void CUserNotice::setContents(const char* _contents)
{
	memset( m_contents, 0x00, sizeof(m_contents) );
	memcpy( m_contents, _contents, strlen(_contents) );
}

char* CUserNotice::getContents(void)
{
	return m_contents;
}

void CUserNotice::setViewCount(int _count)
{
	m_viewCount = _count;
}

int CUserNotice::getViewCount(void)
{
	return m_viewCount;
}

void CUserNotice::increaseVieCount(void)
{
	m_viewCount++;
}

void CUserNotice::setCharIndex(int _index)
{
	m_charindex = _index;
}

int CUserNotice::getCharIndex(void)
{
	return m_charindex;
}

#endif // NOTICE_SYSTEM
//