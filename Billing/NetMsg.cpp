// NetMsg.cpp: implementation of the CNetMsg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdhdrs.h"
#include "NetMsg.h"

CNetMsgPool g_netmsgpool;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CNetMsg::CNetMsg()
{
	m_buf = NULL;
	Init();
}

CNetMsg::CNetMsg(const CNetMsg& src)
{
	m_buf = NULL;
	Init(src);
}

CNetMsg::CNetMsg(int mtype)
{
	m_buf = NULL;
	Init(mtype);
}

//0707
CNetMsg::CNetMsg(int mtype, int key)
{
	m_buf = NULL;
	Init(mtype, key);
}

CNetMsg::~CNetMsg()
{
	if (m_buf)
	{
		g_netmsgpool.Set(m_buf);
	}
	m_mtype = MSG_UNKNOWN;
	m_buf = NULL;
	m_size = 0;
	m_ptr = 0;
}

void CNetMsg::Init()
{
	m_mtype = MSG_UNKNOWN;
	if (!m_buf)
	{
		m_buf = g_netmsgpool.Get(false);
	}
	m_size = 0;
	m_ptr = 0;

	secretkey = 0;
}

void CNetMsg::Init(int mtype)
{
	Init();
	m_mtype = mtype;

	unsigned char ubtype = (unsigned char)m_mtype;
	*this << ubtype;

	secretkey = 0;
}

void CNetMsg::Init(const CNetMsg& src)
{
	Init();
	m_mtype = src.m_mtype;
	m_size = src.m_size;
	memcpy(m_buf, src.m_buf, m_size);
	m_ptr = src.m_ptr;

	//0707
	secretkey = src.secretkey;
}

//0707
void CNetMsg::Init(int mtype, int key)
{
	Init();
	m_mtype = mtype;

	unsigned char ubtype = (unsigned char)m_mtype;
	*this << ubtype;

	secretkey = key;
}

void CNetMsg::Read(void* buf, long size)
{
	if (size < 1 || !CanRead(size))
		return ;
	memcpy(buf, m_buf + m_ptr, size);
	m_ptr += size;
}

void CNetMsg::Write(void* buf, long size)
{
	if (size < 1 || !CanWrite(size))
		return ;
	memcpy(m_buf + m_ptr, buf, size);
	m_ptr += size;
	if (m_ptr > m_size)
		m_size = m_ptr;
}

void CNetMsg::WriteRaw(void* buf, long size)
{
	if (size > MAX_MESSAGE_SIZE)
	{
		Init(MSG_UNKNOWN);
		return ;
	}
	unsigned char* mtype = (unsigned char*)buf;
	Init((int)(*mtype), secretkey);
	Write(mtype + MAX_MESSAGE_TYPE, size - MAX_MESSAGE_TYPE);
	m_size = size;
	m_ptr = 0;
	MoveFirst();
}

CNetMsg& CNetMsg::operator >> (CLCString& str)
{
	// 현재 포인트에서 NULL을 찾는다
	int i;
	for (i = m_ptr; i < m_size; i++)
	{
		if (m_buf[i] == '\0')
			break;
	}

	// NULL을 찾으면
	if (i < m_size)
	{
		// 길이(NULL 포함)을 구하고
		int len = i - m_ptr + 1;
		// 실제 읽을 길이를 구함
		int realRead = len;
		// 길이가 출력 버퍼(str)보다 크면 출력 버퍼 크기에 맞춘다
		if (realRead > str.BufferSize())
			realRead = str.BufferSize();
		// 데이터를 읽음
		char* tmp = new char[realRead];
		memcpy(tmp, m_buf + m_ptr, realRead - 1);
		tmp[realRead - 1] = '\0';
		str = tmp;
		delete [] tmp;
		// 실제 읽은 데이터와 관계없이 NULL 다음으로 m_ptr 이동
		m_ptr += len;
	}
	// 못찾으면
	else
	{
		// 읽은 데이터는 없고
		// 다음 읽기 포인터도 없도록 하여 데이터 읽기가 완료된다
		str = "";
		m_ptr = m_size;
	}

	return *this;
}

CNetMsg& CNetMsg::operator << (const char* str)
{
	// NULL을 포함해서 넣을 수 있으면 넣고 아니면 NULL만 넣는다, NULL도 못 넣으면 그냥 스킵
	if (CanWrite(strlen(str) + 1))
	{
		Write((void*)((const char*)str), strlen(str) + 1);
	}
	else if (CanWrite())
	{
		char ch = 0;
		*this << ch;
	}
	return *this;
}

CNetMsg& CNetMsg::operator << (CNetMsg& src)
{
	if (src.m_size < 1 || !CanWrite(src.m_size))
		return *this;
	memcpy(m_buf + m_ptr, src.m_buf, src.m_size);
	m_ptr += src.m_size;
	if (m_ptr > m_size)
		m_size = m_ptr;

	return *this;
}


//0707
#ifdef COMPRESSION_MESSAGE
// compression type bits in the messages are different than compression type cvar values
// this is to keep backward compatibility with old demos saved with full compression
void CNetMsg::PackDefault(CNetMsg &nmPacked)
{
	int m_CompressionType;

	m_CompressionType = COMPRESSION_TYPE;

	if (m_CompressionType == 2)
	{
		// pack with zlib only
		CzlibCompressor compzlib;
		Pack(nmPacked, compzlib);
		nmPacked.m_mtype |= 0 << 6;
	}
	else if (m_CompressionType == 1)
	{
		// pack with LZ only
		CLZCompressor compLZ;
		Pack(nmPacked, compLZ);
		nmPacked.m_mtype |= 1 << 6;
	}
	else
	{
		// no packing
		int slUnpackedSize	= m_size - MAX_MESSAGE_TYPE;
		void *pvUnpacked	= m_buf + MAX_MESSAGE_TYPE;
		void *pvPacked      = nmPacked.m_buf + MAX_MESSAGE_TYPE;
		nmPacked.m_size		= slUnpackedSize + MAX_MESSAGE_TYPE;
		memcpy(pvPacked, pvUnpacked, slUnpackedSize);
		nmPacked.m_mtype |= 2 << 6;
	}
	nmPacked.m_buf[0] = (char)nmPacked.m_mtype;
}

void CNetMsg::UnpackDefault(CNetMsg &nmUnpacked)
{
	switch (m_mtype >> 6)
	{
	case 0:
		{
			// unpack with zlib only
			CzlibCompressor compzlib;
			Unpack(nmUnpacked,compzlib);
		}
		break;

	case 1:
		{
			// unpack with LZ only
			CLZCompressor compLZ;
			Unpack(nmUnpacked,compLZ);
		}
		break;

	default:
	case 2:
		{
			// no unpacking
			int slPackedSize	= m_size - MAX_MESSAGE_TYPE;
			void *pvPacked		= m_buf + MAX_MESSAGE_TYPE;
			void *pvUnpacked	= nmUnpacked.m_buf + MAX_MESSAGE_TYPE;
			nmUnpacked.m_size	= slPackedSize + MAX_MESSAGE_TYPE;
			memcpy(pvUnpacked, pvPacked, slPackedSize);
		}
		break;
	}
}

/*
 * Pack a message to another message (message type is left untouched).
 */
void CNetMsg::Pack(CNetMsg &nmPacked, CCompressor &comp)
{
	// get size and pointers for packing, leave the message type alone
	int slUnpackedSize	= m_size - MAX_MESSAGE_TYPE;
	void *pvUnpacked    = m_buf + MAX_MESSAGE_TYPE;

	int slPackedSize    = MAX_MESSAGE_SIZE - MAX_MESSAGE_TYPE;
	void *pvPacked      = nmPacked.m_buf + MAX_MESSAGE_TYPE;

	// pack it there
	comp.Pack(pvUnpacked, slUnpackedSize, pvPacked, slPackedSize);

	// set up the destination message size
	nmPacked.m_size = slPackedSize + MAX_MESSAGE_TYPE;
}

/*
 * Unpack a message to another message (message type is left untouched).
 */
void CNetMsg::Unpack(CNetMsg &nmUnpacked, CCompressor &comp)
{
	// get size and pointers for unpacking, leave the message type alone
	int slPackedSize	= m_size - MAX_MESSAGE_TYPE;
	void *pvPacked      = m_buf + MAX_MESSAGE_TYPE;
	
	int slUnpackedSize  = MAX_MESSAGE_SIZE - MAX_MESSAGE_TYPE;
	void *pvUnpacked    = nmUnpacked.m_buf + MAX_MESSAGE_TYPE;
	
	// unpack it there
	comp.Unpack(pvPacked, slPackedSize, pvUnpacked, slUnpackedSize);
	
	// set up the destination message size
	nmUnpacked.m_size = slUnpackedSize + MAX_MESSAGE_TYPE;
}
#endif