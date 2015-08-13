#ifndef NETMSG_H_INCLUDED
#define NETMSG_H_INCLUDED

#include "LCString.h"
#include "Sysdep.h"
typedef long long LONGLONG;
typedef unsigned long long ULONGLONG;

#define MAX_PACKET_HEAD		((int) (sizeof(short)) + (sizeof(int)) + (sizeof(short) ) )
#define MAX_PACKET_DATA		(MAX_PACKET_SIZE - MAX_PACKET_HEAD)
#define MAX_PACKET_SIZE 1000

// 버퍼관련 길이 상수
#define MAX_MESSAGE_TYPE		((int)(sizeof(unsigned char)))
#define MAX_MESSAGE_SIZE		1000
#define MAX_MESSAGE_DATA		(MAX_MESSAGE_SIZE - MAX_MESSAGE_TYPE)

// Reliable(2) + Sequence(4) + ID(2) + TxSize(4)
#define MAX_HEADER_SIZE ((long)(sizeof(unsigned short) + sizeof(unsigned long) + sizeof(unsigned short) + sizeof(unsigned long)))
#define MAX_INPUTBUFFER_SIZE		((MAX_HEADER_SIZE + MAX_MESSAGE_SIZE) * 30)
#define MAX_OUTPUTBUFFER_SIZE		((MAX_MESSAGE_SIZE + MAX_HEADER_SIZE) * 300)
#define MAX_OUTPUTBUFFER_OUTUNIT	4096


#define HTONL(n)		(n) = htonl((n))
#define HTONS(n)		(n) = htons((n))
#define HTONLL(n)		(n) = htonll((n))
#define NTOHL(n)		(n) = ntohl((n))
#define NTOHS(n)		(n) = ntohs((n))
#define NTOHLL(n)		(n) = ntohll((n))

typedef struct __tagLCDES3CONTEXT {
       unsigned int KnL[32];
       unsigned int nKeyHigh;
       unsigned int nKeyLow;
     public:
       __tagLCDES3CONTEXT(void);
   } LCDES3CONTEXT;

inline LONGLONG ntohll(LONGLONG n)
{
#ifdef LC_BIG_ENDIAN
	return n;
#else
	char* p = (char*)&n;
	char ch;
	ch = p[0];	p[0] = p[7];	p[7] = ch;
	ch = p[1];	p[1] = p[6];	p[6] = ch;
	ch = p[2];	p[2] = p[5];	p[5] = ch;
	ch = p[3];	p[3] = p[4];	p[4] = ch;
	return n;
#endif
}

inline LONGLONG htonll(LONGLONG n)
{
#ifdef LC_BIG_ENDIAN
	return n;
#else
	char* p = (char*)&n;
	char ch;
	ch = p[0];	p[0] = p[7];	p[7] = ch;
	ch = p[1];	p[1] = p[6];	p[6] = ch;
	ch = p[2];	p[2] = p[5];	p[5] = ch;
	ch = p[3];	p[3] = p[4];	p[4] = ch;
	return n;
#endif
}

class CNetMsg
{
public:
	int					m_mtype;
	unsigned char*		m_buf;
	int					m_ptr;
	int					m_size;
//0707
	int					secretkey;
	int GetSecretKey() { return secretkey; }

	CNetMsg();
	CNetMsg(int mtype);
	CNetMsg(const CNetMsg& src);
	virtual ~CNetMsg();
	CNetMsg(int mtype, int key);

	void Init();
	void Init(int mtype);
	void Init(const CNetMsg& src);
	void Init(int mtype, int key);//0707

	bool CanRead(int nSpace = 1) { return (m_ptr <= m_size - nSpace); }
	bool CanWrite(int nSpace = 1) { return (m_ptr <= MAX_MESSAGE_SIZE - nSpace); }
	void MoveFirst() { m_ptr = MAX_MESSAGE_TYPE; }

	void Read(void* buf, long size);
	void Write(void* buf, long size);
	void WriteRaw(void* buf, long size);

	const CNetMsg& operator = (const CNetMsg& src) { Init(src); return src; }

	CNetMsg& operator >> (float& f)				{ Read(&f, sizeof(f)); return *this; }
	CNetMsg& operator >> (double& d)			{ Read(&d, sizeof(d)); return *this; }
	CNetMsg& operator >> (ULONGLONG& ull)		{ Read(&ull, sizeof(ull)); NTOHLL(ull); return *this; }
	CNetMsg& operator >> (LONGLONG& ll)			{ Read(&ll, sizeof(ll)); NTOHLL(ll); return *this; }
	CNetMsg& operator >> (unsigned long& ul)	{ Read(&ul, sizeof(ul)); NTOHL(ul); return *this; }
	CNetMsg& operator >> (long& l)				{ Read(&l, sizeof(l)); NTOHL(l); return *this; }
	CNetMsg& operator >> (unsigned int& ul)		{ Read(&ul, sizeof(ul)); NTOHL(ul); return *this; }
	CNetMsg& operator >> (int& l)				{ Read(&l, sizeof(l)); NTOHL(l); return *this; }
	CNetMsg& operator >> (unsigned short& us)	{ Read(&us, sizeof(us)); NTOHS(us); return *this; }
	CNetMsg& operator >> (short& s)				{ Read(&s, sizeof(s)); NTOHS(s); return *this; }
	CNetMsg& operator >> (unsigned char& ub)	{ Read(&ub, sizeof(ub)); return *this; }
	CNetMsg& operator >> (char& b)				{ Read(&b, sizeof(b)); return *this; }
	CNetMsg& operator >> (CLCString& str);

	CNetMsg& operator << (float f)				{ Write(&f, sizeof(f)); return *this; }
	CNetMsg& operator << (double d)				{ Write(&d, sizeof(d)); return *this; }
	CNetMsg& operator << (ULONGLONG ull)		{ HTONLL(ull); Write(&ull, sizeof(ull)); return *this; }
	CNetMsg& operator << (LONGLONG ll)			{ HTONLL(ll); Write(&ll, sizeof(ll)); return *this; }
	CNetMsg& operator << (unsigned long ul)		{ HTONL(ul); Write(&ul, sizeof(ul)); return *this; }
	CNetMsg& operator << (long l)				{ HTONL(l); Write(&l, sizeof(l)); return *this; }
	CNetMsg& operator << (unsigned int ul)		{ HTONL(ul); Write(&ul, sizeof(ul)); return *this; }
	CNetMsg& operator << (int l)				{ HTONL(l); Write(&l, sizeof(l)); return *this; }
	CNetMsg& operator << (unsigned short us)	{ HTONS(us); Write(&us, sizeof(us)); return *this; }
	CNetMsg& operator << (short s)				{ HTONS(s); Write(&s, sizeof(s)); return *this; }
	CNetMsg& operator << (unsigned char ub)		{ Write(&ub, sizeof(ub)); return *this; }
	CNetMsg& operator << (char b)				{ Write(&b, sizeof(b)); return *this; }
	CNetMsg& operator << (const char* str);
	CNetMsg& operator << (CNetMsg& src);
	CNetMsg& operator << (bool b)				{ if (b) (*this) << (char)1; else (*this) << (char)0; return *this; }

	//inline MESSAGETYPE GetType(void) const { return MESSAGETYPE(m_mtype&0x3F); };
};

#endif
