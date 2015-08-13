// NetMsg.h: interface for the CNetMsg class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __NET_MSG_H__
#define __NET_MSG_H__

#include <boost/shared_ptr.hpp>
#include "Conf.h"
#include "MemoryPoolBase.h"
#include "config_packet.h"
#include "MessageType.h"
#include "LCString.h"

#ifndef HTONL
#define HTONL(n)		(n) = htonl((n))
#define HTONS(n)		(n) = htons((n))
#define HTONLL(n)		(n) = htonll((n))
#define NTOHL(n)		(n) = ntohl((n))
#define NTOHS(n)		(n) = ntohs((n))
#define NTOHLL(n)		(n) = ntohll((n))
#endif

inline LONGLONG ntohll(LONGLONG n)
{
	char* p = (char*)&n;
	char ch;
	ch = p[0];
	p[0] = p[7];
	p[7] = ch;
	ch = p[1];
	p[1] = p[6];
	p[6] = ch;
	ch = p[2];
	p[2] = p[5];
	p[5] = ch;
	ch = p[3];
	p[3] = p[4];
	p[4] = ch;
	return n;
}

inline LONGLONG htonll(LONGLONG n)
{
	char* p = (char*)&n;
	char ch;
	ch = p[0];
	p[0] = p[7];
	p[7] = ch;
	ch = p[1];
	p[1] = p[6];
	p[6] = ch;
	ch = p[2];
	p[2] = p[5];
	p[5] = ch;
	ch = p[3];
	p[3] = p[4];
	p[4] = ch;
	return n;
}

#pragma pack(push, 1)
struct MsgHeader
{
	unsigned short	reliable;
	unsigned int	seq;
	unsigned short	id;
	int				size;
};
#pragma pack(pop)

#define RefMsg(x)		(*(x.get()))

class CNetMsg : public MemoryPoolBaseWithMutex<CNetMsg>
{
public:
	typedef boost::shared_ptr<CNetMsg> SP;

public:
	int					m_mtype;
	unsigned char*		m_buf_all;
	unsigned char*		m_buf;
	int					m_ptr;
	int					m_size;
	int					secretkey;
	bool				crypt_flag_;			// only write
	bool				crc32_flag_;			// only write
	bool				make_header_flag_;		// only write make header / crypt / crc32
	int GetSecretKey()
	{
		return secretkey;
	}

	CNetMsg();
	CNetMsg(int mtype);
	CNetMsg(const CNetMsg& src);
	CNetMsg(const CNetMsg::SP& src);
	virtual ~CNetMsg();
	CNetMsg(int mtype, int key);

	void Init();
	void Init(int mtype);
	void Init(const CNetMsg& src);
	void Init(const CNetMsg::SP& src);
	void Init(int mtype, int key);
	void BufferClear()
	{
		if(m_buf_all)
		{
			memset(m_buf_all, 0x00, MAX_MESSAGE_SIZE);
		}
	}

	bool CanRead(int nSpace = 1)
	{
		return (m_ptr <= m_size - nSpace);
	}
	bool CanWrite(int nSpace = 1)
	{
		return (m_ptr <= MAX_MESSAGE_SIZE - nSpace);
	}
	void MoveFirst()
	{
		m_ptr = MAX_MESSAGE_TYPE;
	}
	void Reduce(int pos, int length);

	void Read(void* buf, long size);
	void Write(void* buf, long size);
	void WriteRaw(void* buf, long size);

	int getSize()
	{
		return m_size;
	}

	void setSize(int size);

	CNetMsg& operator >> (float& f)
	{
		Read(&f, sizeof(f));
		return *this;
	}
	CNetMsg& operator >> (double& d)
	{
		Read(&d, sizeof(d));
		return *this;
	}
	CNetMsg& operator >> (ULONGLONG& ull)
	{
		Read(&ull, sizeof(ull));
		NTOHLL(ull);
		return *this;
	}
	CNetMsg& operator >> (LONGLONG& ll)
	{
		Read(&ll, sizeof(ll));
		NTOHLL(ll);
		return *this;
	}
	CNetMsg& operator >> (unsigned long& ul)
	{
		Read(&ul, sizeof(ul));
		NTOHL(ul);
		return *this;
	}
	CNetMsg& operator >> (long& l)
	{
		Read(&l, sizeof(l));
		NTOHL(l);
		return *this;
	}
	CNetMsg& operator >> (unsigned int& ul)
	{
		Read(&ul, sizeof(ul));
		NTOHL(ul);
		return *this;
	}
	CNetMsg& operator >> (int& l)
	{
		Read(&l, sizeof(l));
		NTOHL(l);
		return *this;
	}
	CNetMsg& operator >> (unsigned short& us)
	{
		Read(&us, sizeof(us));
		NTOHS(us);
		return *this;
	}
	CNetMsg& operator >> (short& s)
	{
		Read(&s, sizeof(s));
		NTOHS(s);
		return *this;
	}
	CNetMsg& operator >> (unsigned char& ub)
	{
		Read(&ub, sizeof(ub));
		return *this;
	}
	CNetMsg& operator >> (char& b)
	{
		Read(&b, sizeof(b));
		return *this;
	}
	CNetMsg& operator >> (CLCString& str);

	CNetMsg& operator << (float f)
	{
		Write(&f, sizeof(f));
		return *this;
	}
	CNetMsg& operator << (double d)
	{
		Write(&d, sizeof(d));
		return *this;
	}
	CNetMsg& operator << (ULONGLONG ull)
	{
		HTONLL(ull);
		Write(&ull, sizeof(ull));
		return *this;
	}
	CNetMsg& operator << (LONGLONG ll)
	{
		HTONLL(ll);
		Write(&ll, sizeof(ll));
		return *this;
	}
	CNetMsg& operator << (unsigned long ul)
	{
		HTONL(ul);
		Write(&ul, sizeof(ul));
		return *this;
	}
	CNetMsg& operator << (long l)
	{
		HTONL(l);
		Write(&l, sizeof(l));
		return *this;
	}
	CNetMsg& operator << (unsigned int ul)
	{
		HTONL(ul);
		Write(&ul, sizeof(ul));
		return *this;
	}
	CNetMsg& operator << (int l)
	{
		HTONL(l);
		Write(&l, sizeof(l));
		return *this;
	}
	CNetMsg& operator << (unsigned short us)
	{
		HTONS(us);
		Write(&us, sizeof(us));
		return *this;
	}
	CNetMsg& operator << (short s)
	{
		HTONS(s);
		Write(&s, sizeof(s));
		return *this;
	}
	CNetMsg& operator << (unsigned char ub)
	{
		Write(&ub, sizeof(ub));
		return *this;
	}
	CNetMsg& operator << (char b)
	{
		Write(&b, sizeof(b));
		return *this;
	}
	CNetMsg& operator << (const char* str);
	CNetMsg& operator << (CNetMsg& src);
	CNetMsg& operator << (CNetMsg::SP& src);
	CNetMsg& operator << (bool b)
	{
		if (b) (*this) << (char)1;
		else (*this) << (char)0;
		return *this;
	}

	inline MESSAGETYPE GetType(void) const
	{
		return MESSAGETYPE(m_mtype&0x3F);
	};

	//////////////////////////////////////////////////////////////////////////
	void crypt();
	void decrypt();
	void makeCRC32();
	bool checkCRC32();
	void makeHeader();
	bool isRightPacket();

	static void InitSEEDEncrypt();
};

#endif
