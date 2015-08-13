// OutputBuffer.h: interface for the COutputBuffer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OUTPUTBUFFER_H__0A40D7F4_947A_4BBF_ACA4_9FB66CE4116E__INCLUDED_)
#define AFX_OUTPUTBUFFER_H__0A40D7F4_947A_4BBF_ACA4_9FB66CE4116E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetMsg.h"
#include "MemPool.h"

extern COutputBufferPool g_outbufpool;

class COutputBuffer  
{
	unsigned char*	m_buf;
	int				m_ptr;
	int				m_len;

#ifdef CRYPT_NET_MSG
	unsigned char pCryptBuf[CNM_TEMP_BUFFER_LENGTH];
	unsigned char pCryptBufTmp[CNM_TEMP_BUFFER_LENGTH];
#endif

	int				m_seq;

public:
	COutputBuffer()
	{
		m_buf = g_outbufpool.Get();
		m_ptr = 0;
		m_seq = 0;
	}

	virtual ~COutputBuffer()
	{
		g_outbufpool.Set(m_buf);
		m_buf = NULL;
		m_ptr = 0;
	}

	void Clear()
	{
		m_ptr = 0;
	}

#ifdef CRYPT_NET_MSG
	bool Add(CNetMsg& msg, bool bCrypt, CNM_KEY_TYPE* nKey)
#else
	bool Add(CNetMsg& msg)
#endif // #ifdef CRYPT_NET_MSG
	{
		if (msg.m_mtype == MSG_UNKNOWN)
			return true;

		CNetMsg tmsg = msg;
		
		//서버 다운 추가(09.05.11)
		if(tmsg.m_ptr < 0 || tmsg.m_size >= MAX_OUTPUTBUFFER_SIZE )
			return false;


#ifdef CRYPT_NET_MSG
		if (bCrypt && nKey)
		{
			int nCryptBuf = CNM_Crypt(tmsg.m_buf, tmsg.m_size, nKey, pCryptBuf, pCryptBufTmp);
			if (nCryptBuf < 0)
				return false;
			tmsg.Init();
			tmsg.WriteRaw(pCryptBuf, nCryptBuf);
		}
#endif // #ifdef CRYPT_NET_MSG

		int len = tmsg.m_size + MAX_HEADER_SIZE;

#ifdef USING_CHECKSUM_MD5
		len += USING_CHECKSUM_MD5;
#endif // USING_CHECKSUM_MD5

#ifdef USING_CHECKSUM_CRC32
		len += USING_CHECKSUM_CRC32;
#endif // USING_CHECKSUM_CRC32

		if (m_ptr + len >= MAX_OUTPUTBUFFER_SIZE)
			return false;

		// reliable, seq, id, size
		unsigned short pack_reliable = (1 << 0) | (1 << 7) | (1 << 8);	// UNRELIABLE HEAD + BODY + TAIL
		int seq = m_seq++;
		unsigned short pack_id = 0;
		int size = tmsg.m_size;

		HTONS(pack_reliable);
		HTONL(seq);
		HTONS(pack_id);
		HTONL(size);

		//서버 다운 추가(09.05.11)
		if(m_ptr < 0)
			return false;

		try
		{
			memcpy(m_buf + m_ptr, &pack_reliable,	2);			m_ptr += 2;
			memcpy(m_buf + m_ptr, &seq,				4);			m_ptr += 4;
			memcpy(m_buf + m_ptr, &pack_id,			2);			m_ptr += 2;
			memcpy(m_buf + m_ptr, &size,			4);			m_ptr += 4;
			memcpy(m_buf + m_ptr, tmsg.m_buf, tmsg.m_size);		m_ptr += tmsg.m_size;
		}
		catch (...)
		{
			return false;			
		}


#ifdef USING_CHECKSUM_MD5
		extern CLCMD5 gMD5;
		gMD5.Init();
		gMD5.Update(tmsg.m_buf, tmsg.m_size);
		int pMD5Result[4];
		memcpy(pMD5Result, gMD5.Final(), USING_CHECKSUM_MD5);
		HTONL(pMD5Result[0]);
		HTONL(pMD5Result[1]);
		HTONL(pMD5Result[2]);
		HTONL(pMD5Result[3]);
		memcpy(m_buf + m_ptr, pMD5Result,		4);			m_ptr += 4;
		memcpy(m_buf + m_ptr, pMD5Result + 1,	4);			m_ptr += 4;
		memcpy(m_buf + m_ptr, pMD5Result + 2,	4);			m_ptr += 4;
		memcpy(m_buf + m_ptr, pMD5Result + 3,	4);			m_ptr += 4;
#endif // USING_CHECKSUM_MD5

#ifdef USING_CHECKSUM_CRC32
		extern CLCCRC32 gCRC32;
		unsigned int nCRCResult = gCRC32.CalcCRC32(tmsg.m_buf, tmsg.m_size);
		HTONL(nCRCResult);
		memcpy(m_buf + m_ptr, &nCRCResult,		4);			m_ptr += 4;
#endif // USING_CHECKSUM_CRC32

		return true;
	}

	unsigned char* GetNextPoint()
	{
		if (m_ptr == 0)
			return NULL;
		return m_buf;
	}

	int GetRemain()
	{
		if (m_ptr > MAX_OUTPUTBUFFER_OUTUNIT)
			return MAX_OUTPUTBUFFER_OUTUNIT;
		else
			return m_ptr;
	}

	bool Update(int val)
	{
		if (val < 0)
			return false;

		if (m_ptr <= val)
		{
			m_ptr = 0;
			return true;
		}
		else
		{
			unsigned char* buf = g_outbufpool.Get();
			memcpy(buf, m_buf + val, sizeof(unsigned char) * (m_ptr - val));
			m_ptr -= val;
			g_outbufpool.Set(m_buf);
			m_buf = buf;
			return false;
		}
	}
};

#endif // !defined(AFX_OUTPUTBUFFER_H__0A40D7F4_947A_4BBF_ACA4_9FB66CE4116E__INCLUDED_)
