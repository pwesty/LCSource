#include <boost/thread/tss.hpp>
#include "stdhdrs.h"

#include "Log.h"
#include "Server.h"

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

	case LHT_TIME_CLASSTYPE_MSG:
		m_nowpos = snprintf(m_buf, MAX_BUFFER_SIZE, "GROUP_%d CHANNEL_%d - [ lc@barunson ", gserver->m_serverno, gserver->m_subno, lh.m_msg);
		break;
	}

	return *this;
}

CLogBuffer& CLogBuffer::operator << (ITEMLOGMANI ilm)
{
	if (ilm.m_pItem == NULL)
	{
		(*this) << "(null)";
		return (*this);
	}

	if (ilm.m_pItem->IsRareItem() == true
			&& ilm.m_pItem->m_pRareOptionProto != NULL
			&& ilm.m_pItem->m_pRareOptionProto->GetIndex() > 0)
	{
		(*this) << "[ rare index: " << ilm.m_pItem->m_pRareOptionProto->GetIndex() << delim
				<< "rare bit: " << ilm.m_pItem->m_nRareOptionBit << "] " ;
	}

	(*this) << ilm.m_pItem->getDBIndex() << delim
			<< ilm.m_pItem->m_itemProto->getItemName() << delim
			<< ilm.m_pItem->m_serial.c_str() << delim
			<< ilm.m_pItem->getWearPos() << delim
			<< ilm.m_pItem->getPlus() << delim
			<< ilm.m_pItem->getFlag() << delim
			<< ilm.m_pItem->getUsed() << delim
			<< ilm.m_pItem->getUsed_2() << delim
			<< ilm.m_pItem->Count();

	if (!ilm.m_bSimple)
	{
		(*this) << delim << "OPTION"
				<< delim << ilm.m_pItem->m_nOption;
		int i;
		for (i = 0; i < ilm.m_pItem->m_nOption; i++)
		{
			(*this) << delim << ilm.m_pItem->m_option[i].m_type
					<< delim << ilm.m_pItem->m_option[i].m_value;
		}
	}

	(*this) << delim << "SOCKET" << delim << ilm.m_pItem->m_socketList.GetSocketCount();

	for (int i = 0; i < MAX_SOCKET_COUNT; i++)
	{
		(*this) << delim << ilm.m_pItem->m_socketList.GetJewelAt(i);
	}

	if (ilm.m_pItem->IsOriginItem())
	{
		(*this) << delim << "REFORM VARIATION INFO";
		for (int i = 0; i < MAX_VARIATION_COUNT; i++)
		{
			(*this) << delim << ilm.m_pItem->getOriginVar(i);
		}
	}

	return (*this);
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
