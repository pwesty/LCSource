// BPacket.cpp: implementation of the CBPacket class.
//
//////////////////////////////////////////////////////////////////////
#include <boost/pool/singleton_pool.hpp>
#include "BPacket.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
struct tag_pool {};
typedef boost::singleton_pool<tag_pool, MAX_PACKET_SIZE> g_Bpacket_pool;

CBPacket::CBPacket()
	:m_mtype(-1)
	,m_serial(0)
	,m_size(0)
	,m_buf_all(NULL)
	,m_buf(NULL)
	,m_ptr(0)
	,make_header_flag_(false)
{
	Init();
}

CBPacket::~CBPacket()
{
	if (m_buf)
	{
		g_Bpacket_pool::free(m_buf_all);
	}
}

void CBPacket::Init()
{
	m_mtype = -1;
	if (!m_buf_all)
	{
		m_buf_all = (unsigned char*)g_Bpacket_pool::malloc();
		m_buf = m_buf_all + sizeof(BPacketHeader);
	}
	m_serial = 0;
	m_size = 0;
	m_ptr = 0;
	make_header_flag_ = false;
}

void CBPacket::Init(short mtype)
{
	Init();
	m_mtype = mtype;
}

void CBPacket::Init(const CBPacket& src)
{
	Init();
	m_mtype = src.m_mtype;
	m_size = src.m_size;
	m_serial = src.m_serial;
	memcpy(m_buf, src.m_buf, (int) m_size);
	m_ptr = src.m_ptr;
}

void CBPacket::Read(void* buf, long size)
{
	if (size < 1 || !CanRead(size))
		return ;
	memcpy(buf, m_buf + m_ptr, size);
	m_ptr += size;
}

void CBPacket::Write(void* buf, long size)
{
	if (size < 1 || !CanWrite(size))
		return ;
	memcpy(m_buf + m_ptr, buf, size);
	m_ptr += size;
	if ((short) m_ptr > m_size)
		m_size = (short) m_ptr;
}

void CBPacket::WriteRaw(void* buf, long size)
{
	Write(buf, size);
	m_size = (short) size;
	m_ptr = 0;
	MoveFirst();
}

CBPacket& CBPacket::operator >> (char* str)
{
	int i = 0;
	while (i <= 31)
	{
		*this >> (*str);
		str++;
		i++;
	}

	return *this;
}

CBPacket& CBPacket::operator >> (CLCString& str)
{
	char* tmp = new char[str.BufferSize()-1];

	memcpy(tmp, m_buf+m_ptr, str.BufferSize() - 1);
	m_ptr += str.BufferSize() -1;
	str = tmp;

	return *this;
}

CBPacket& CBPacket::operator << (CLCString& str)
{
	memcpy(m_buf + m_ptr, (const char*) str, str.BufferSize() - 1 );
	m_ptr += str.BufferSize() - 1;

	return *this;
}

CBPacket& CBPacket::operator << (const char* str)
{
	int i = 0;
	while (i <= 31)
	{
		*this << (*str);
		str++;
		i++;
	}

	return *this;
}

void CBPacket::makeHeader()
{
	BPacketHeader* h = (BPacketHeader*)m_buf_all;

	h->type = m_mtype;
	h->sn = m_serial;
	h->size = m_size;

	HTONS(h->type);
	HTONL(h->sn);
	HTONS(h->size);

	make_header_flag_ = true;
}