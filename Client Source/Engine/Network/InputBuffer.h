#ifndef __INPUTBUFFER_H__
#define __INPUTBUFFER_H__

#include "CPacket.h"

#define HTONL(n)		{ n = htonl(n); }
#define HTONS(n)		{ n = htons(n); }
#define NTOHL(n)		{ n = ntohl(n); }
#define NTOHS(n)		{ n = ntohs(n); }

class CInputBuffer  
{
public:
	unsigned char*	m_buf;
	int				m_size;
	int				m_ptr;
	unsigned short	m_id;

	CInputBuffer(int size = 40*MAX_PACKET_SIZE)
	{
		m_size = size + 100;
		m_buf = new unsigned char[m_size];
		Clear();
	}

	virtual ~CInputBuffer()
	{
		delete [] m_buf;
		m_buf = NULL;
		m_size = 0;
		m_ptr = 0;
	}

	void Clear()
	{
		memset(m_buf, 0, m_size);
		m_ptr = 0;
	}

	void ClearBuffer();

	unsigned char* GetPoint()
	{
		ASSERT( m_ptr <= m_size && "BOF" );
		return m_buf + m_ptr;
	}

	int GetRemain()
	{
		ASSERT( m_size - m_ptr >= 0 && "Not enough buffer..." );
		return m_size - m_ptr;
	}

	void SetPoint(int dval)
	{
		m_ptr += dval;
	}

	CPacket* GetPacket();
};

#endif
