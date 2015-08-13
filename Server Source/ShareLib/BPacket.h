// BPacket.h: interface for the CBPacket class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __BPACKET_H__
#define __BPACKET_H__

#include <boost/shared_ptr.hpp>
#include "Conf.h"
#include "MemoryPoolBase.h"
#include "config_packet.h"
#include "LCString.h"

#ifndef HTONL
#define HTONL(n)		(n) = htonl((n))
#define HTONS(n)		(n) = htons((n))
#define HTONLL(n)		(n) = htonll((n))
#define NTOHL(n)		(n) = ntohl((n))
#define NTOHS(n)		(n) = ntohs((n))
#define NTOHLL(n)		(n) = ntohll((n))
#endif

#pragma pack(push, 1)
struct BPacketHeader
{
	unsigned short	type;
	int				sn;
	short			size;
};
#pragma pack(pop)

#define RefBPacket(x)		(*(x.get()))

class CBPacket : public MemoryPoolBaseWithMutex<CBPacket>
{
public:
	typedef boost::shared_ptr<CBPacket> SP;

public:
	CBPacket();
	virtual ~CBPacket();

	void Init();
	void Init(short ptype);
	void Init(const CBPacket& src);

	short				m_mtype;
	int					m_serial;
	short				m_size;
	unsigned char*		m_buf_all;
	unsigned char*		m_buf;
	int					m_ptr;
	bool				make_header_flag_;		// only write make header / crypt / crc32

	bool CanRead(int nSpace = 1)
	{
		return (m_ptr <= m_size - nSpace);
	}
	bool CanWrite(int nSpace = 1)
	{
		return (m_ptr <= MAX_PACKET_SIZE - nSpace);
	}
	void MoveFirst()
	{
		m_ptr = 0;
	}

	void Read(void* buf, long size);
	void Write(void* buf, long size);
	void WriteRaw(void* buf, long size);
//  NTOHS(s);
//  NTOHL(l);
//  HTONS(s);
//  HTONL(l);

	CBPacket& operator >> (short& s)
	{
		Read(&s, sizeof(s));
		NTOHS(s);
		return *this;
	}
	CBPacket& operator >> (unsigned char& ub)
	{
		Read(&ub, sizeof(ub));
		return *this;
	}
	CBPacket& operator >> (int& l)
	{
		Read(&l, sizeof(l));
		NTOHL(l);
		return *this;
	}
	CBPacket& operator >> (char& b)
	{
		Read(&b, sizeof(b));
		return *this;
	}
	CBPacket& operator >> (char* str);
	CBPacket& operator >> (CLCString& str);

	CBPacket& operator << (short s)
	{
		HTONS(s);
		Write(&s, sizeof(s));
		return *this;
	}
	CBPacket& operator << (unsigned char ub)
	{
		Write(&ub, sizeof(ub));
		return *this;
	}
	CBPacket& operator << (int l)
	{
		HTONL(l);
		Write(&l, sizeof(l));
		return *this;
	}
	CBPacket& operator << (char b)
	{
		Write(&b, sizeof(b));
		return *this;
	}
	CBPacket& operator << (const char* str);
	CBPacket& operator << (CLCString& str);

	void makeHeader();
};

#endif
