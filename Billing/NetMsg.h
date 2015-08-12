// NetMsg.h: interface for the CNetMsg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NETMSG_H__2004CDC6_1025_47D4_93E0_AD8779DB8E45__INCLUDED_)
#define AFX_NETMSG_H__2004CDC6_1025_47D4_93E0_AD8779DB8E45__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef __BILLING_SERVER__
#ifdef LC_KOR
#include "BillMessage_KOR.inh"
#elif defined(USE_TENTER_BILLING)
#include "BillMessage_TLD.inh"
#else
#include "BillMessage_KOR.inh"
#endif
#endif

#include "MemPool.h"
#ifndef __BILLING_SERVER__
#include "MessageType.h"
#endif

#ifdef COMPRESSION_MESSAGE
	#include "Compression.h"
#endif

#ifdef CRYPT_NET_MSG
#include "CryptNetMsg.h"
#endif // #ifdef CRYPT_NET_MSG

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

extern CNetMsgPool g_netmsgpool;

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

//0707
#ifdef SECRET_KEY
#ifdef __BILLING_SERVER__
	inline BILLMESSAGE_TYPE GetType(void) const { return BILLMESSAGE_TYPE(m_mtype&0x3F); };
#else
	inline MESSAGETYPE GetType(void) const { return MESSAGETYPE(m_mtype&0x3F); };
#endif // __BILLING_SERVER__
#endif // SECRET_KEY
#ifdef COMPRESSION_MESSAGE
	// 현재 클래스의 내용을 압축하여 nmPacked에 넣기
	void PackDefault(CNetMsg &nmPacked);
	// 현재 클래스의 내용을 풀어서 nmUnpacked에 넣기
	void UnpackDefault(CNetMsg &nmUnpacked);
	// 현재 클래스의 내용을 comp로 압축하여 nmPacked에 넣기
	void Pack(CNetMsg &nmPacked, CCompressor &comp);
	// 현재 클래스의 내용을 comp로 풀어서 nmUnpacked에 넣기
	void Unpack(CNetMsg &nmUnpacked, CCompressor &comp);
#endif
};

#endif // !defined(AFX_NETMSG_H__2004CDC6_1025_47D4_93E0_AD8779DB8E45__INCLUDED_)
