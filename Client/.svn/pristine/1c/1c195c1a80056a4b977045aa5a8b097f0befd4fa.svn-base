#include "stdh.h"
#include "InputBuffer.h"

CPacket* CInputBuffer::GetPacket()
{
	if (m_ptr < MAX_HEADER_SIZE) 
		return NULL;
	unsigned short reliable;
	unsigned long seq;
	unsigned short id;
	unsigned long txSize;
	unsigned char* p = m_buf;
	memcpy(&reliable, p, sizeof(reliable));		p += sizeof(reliable);
	memcpy(&seq, p, sizeof(seq));				p += sizeof(seq);
	memcpy(&id, p, sizeof(id));					p += sizeof(id);
	memcpy(&txSize, p, sizeof(txSize));			p += sizeof(txSize);

	//0524 kwon
	NTOHS(reliable);
	NTOHL(seq);
	NTOHS(id);
	NTOHL(txSize);

	ASSERT(txSize <= MAX_PACKET_SIZE && txSize > 0);

	// [2013/01/09] sykim70
	if ((int)(txSize + MAX_HEADER_SIZE + sizeof(unsigned int)) > m_ptr)
		return NULL;
	CPacket* ret = CreatePacket();
	ret->WriteToPacketRaw(m_buf,txSize + MAX_HEADER_SIZE);
	// [2013/01/09] sykim70
	p += (txSize + sizeof(unsigned int));
	int moveLength = (m_buf + m_ptr) - p;
	if (moveLength > 0)
		memmove(m_buf, p, m_ptr);
	m_ptr = moveLength;

	return ret;
}

void CInputBuffer::ClearBuffer()
{
	if (m_ptr < MAX_HEADER_SIZE) 
		return;

	unsigned short reliable;
	unsigned long seq;
	unsigned short id;
	unsigned long txSize;
	unsigned char* p;

	for (;;)
	{
		p = m_buf;
		memcpy(&reliable, p, sizeof(reliable));		p += sizeof(reliable);
		memcpy(&seq, p, sizeof(seq));				p += sizeof(seq);
		memcpy(&id, p, sizeof(id));					p += sizeof(id);
		memcpy(&txSize, p, sizeof(txSize));			p += sizeof(txSize);

		//0524 kwon
		NTOHS(reliable);
		NTOHL(seq);
		NTOHS(id);
		NTOHL(txSize);

		ASSERT(txSize <= MAX_PACKET_SIZE && txSize > 0);

		if ((int)(txSize + MAX_HEADER_SIZE + sizeof(unsigned int)) > m_ptr)
			return;

		p += (txSize + sizeof(unsigned int));
		int moveLength = (m_buf + m_ptr) - p;
		if (moveLength > 0)
			memmove(m_buf, p, m_ptr);
		
		m_ptr = moveLength;

		if (m_ptr < MAX_HEADER_SIZE) 
			return;
	}

	return;
}
