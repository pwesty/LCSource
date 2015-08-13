#include <boost/thread/tss.hpp>
#include "stdhdrs.h"

#include "Log.h"

//////////////////////////////////////////////////////////////////////////
static boost::thread_specific_ptr<CLogBuffer> _ptr;
//////////////////////////////////////////////////////////////////////////
CLogBuffer::CLogBuffer()
{
	m_buf = new char[MAX_BUFFER_SIZE];
}

CLogBuffer::~CLogBuffer()
{
	delete [] m_buf;
	m_buf = NULL;
}

CLogBuffer& CLogBuffer::operator << (const char* m)
{
	m_nowpos += snprintf(m_buf + m_nowpos, MAX_BUFFER_SIZE - m_nowpos, m);
	return *this;
}

CLogBuffer& CLogBuffer::operator << (int m)
{
	m_nowpos += snprintf(m_buf + m_nowpos, MAX_BUFFER_SIZE - m_nowpos, "%d", m);
	return *this;
}

CLogBuffer& CLogBuffer::operator << (LONGLONG m)
{
#ifdef WIN32
	m_nowpos += snprintf(m_buf + m_nowpos, MAX_BUFFER_SIZE - m_nowpos, "%I64d", m);
#else
	m_nowpos += snprintf(m_buf + m_nowpos, MAX_BUFFER_SIZE - m_nowpos, "%lld", m);
#endif
	return *this;
}

CLogBuffer& CLogBuffer::operator << (float m)
{
	m_nowpos += snprintf(m_buf + m_nowpos, MAX_BUFFER_SIZE - m_nowpos, "%.4f", m);
	return *this;
}

CLogBuffer& CLogBuffer::operator << (const CLogHeader& lh)
{
	m_nowpos = 0;

	switch (lh.m_type)
	{
	case LHT_MSG:
		m_nowpos = snprintf(m_buf, MAX_BUFFER_SIZE, "%s > ", lh.m_msg);
		break;

	case LHT_MSG_ID:
		m_nowpos = snprintf(m_buf, MAX_BUFFER_SIZE, "%s > %s : ", lh.m_msg, lh.m_id);
		break;

	case LHT_MSG_ID_NAME:
		m_nowpos = snprintf(m_buf, MAX_BUFFER_SIZE, "%s > %s : %s : ", lh.m_msg, lh.m_name, lh.m_id);
		break;

	case LHT_MSG_ID_NICK:
		m_nowpos = snprintf(m_buf, MAX_BUFFER_SIZE, "%s > %s : %s : %s : ", lh.m_msg, lh.m_name, lh.m_nick, lh.m_id);
		break;
	}

	return *this;
}

CLogBuffer& CLogBuffer::GetCLogBuffer()
{
	if (_ptr.get() == NULL)
	{
		_ptr.reset(new CLogBuffer);
	}

	return *(_ptr.get());
}

void CLogBuffer::Write()
{
	LOG_INFO(m_buf);
}
