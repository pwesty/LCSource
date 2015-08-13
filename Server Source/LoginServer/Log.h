#ifndef __LOG_H__
#define __LOG_H__

#include "../ShareLib/Conf.h"

#ifdef WIN32
#define snprintf	_snprintf
#endif

typedef enum _tagLogHeaderType
{
	LHT_MSG,			// 로그 메시지만 있음
	LHT_MSG_ID,			// 로그 메시지 + 사용자 아이디
	LHT_MSG_ID_NAME,	// 로그 메시지 + 사용자 아이디 + 캐릭터 이름
	LHT_MSG_ID_NICK,	// 로그 메시지 + 사용자 아이디 + 케릭터 이름 + 케릭터 별명

	LHT_TIME_CLASSTYPE_MSG, // 로그인시간 + 클래스 타입 + 로그 메시지
} LOG_HEADER_TYPE;

class CLogHeader
{
public:
	LOG_HEADER_TYPE		m_type;
	const char*			m_msg;
	const char*			m_name;
	const char*			m_nick;
	const char*			m_id;

public:
	CLogHeader(const CLogHeader& src)
		: m_type(src.m_type)
		, m_msg(src.m_msg)
		, m_name(src.m_name)
		, m_nick(src.m_nick)
		, m_id(src.m_id)
	{
	}

	CLogHeader(int classType, const char* msg)
		: m_type(LHT_TIME_CLASSTYPE_MSG)
		, m_msg(msg)
		, m_name(NULL)
		, m_nick(NULL)
		, m_id(NULL)
	{
	}

	CLogHeader(const char* msg, const char* name, const char* nick, const char* id)
		: m_type(LHT_MSG_ID_NICK)
		, m_msg(msg)
		, m_name(name)
		, m_nick(nick)
		, m_id(id)
	{
	}

	CLogHeader(const char* msg, const char* name, const char* id)
		: m_type(LHT_MSG_ID_NAME)
		, m_msg(msg)
		, m_name(name)
		, m_nick(NULL)
		, m_id(id)
	{
	}

	CLogHeader(const char* msg, const char* id)
		: m_type(LHT_MSG_ID)
		, m_msg(msg)
		, m_name(NULL)
		, m_nick(NULL)
		, m_id(id)
	{
	}

	CLogHeader(const char* msg)
		: m_type(LHT_MSG)
		, m_msg(msg)
		, m_name(NULL)
		, m_nick(NULL)
		, m_id(NULL)
	{
	}
};

class CLogBuffer
{
private:
	enum
	{
		MAX_BUFFER_SIZE = 20480,
	};

	char	*m_buf;
	int		m_nowpos;

public:
	CLogBuffer();
	~CLogBuffer();

	CLogBuffer& operator << (const char* m);
	CLogBuffer& operator << (int m);
	CLogBuffer& operator << (LONGLONG m);
	CLogBuffer& operator << (float m);
	CLogBuffer& operator << (void (*f)(CLogBuffer&))
	{
		(*f)(*this);
		return (*this);
	}
	CLogBuffer& operator << (const CLogHeader& lh);

	void Write();

public:
	static CLogBuffer& GetCLogBuffer();
};

//////////////////////////////////////////////////////////////////////////
inline CLogHeader init(const char* msg, const char* name, const char* nick, const char* id)
{
	return CLogHeader(msg, name, nick, id);
}

inline CLogHeader init(const char* msg, const char* name, const char* id)
{
	return CLogHeader(msg, name, id);
}

inline CLogHeader init(const char* msg, const char* id)
{
	return CLogHeader(msg, id);
}

inline CLogHeader init(const char* msg)
{
	return CLogHeader(msg);
}

inline void end(CLogBuffer& buf)
{
	buf.Write();
}

inline void endGer(CLogBuffer& buf)
{
	buf << "]";
	buf.Write();
}

inline void blank(CLogBuffer& buf)
{
	buf << " ";
}

inline void delim(CLogBuffer& buf)
{
	buf << " : ";
}

#define LOG_VAL(name, val)  name"=\"" << val << "\""
#define GAMELOG			(CLogBuffer::GetCLogBuffer())
#define DBLOG			(CLogBuffer::GetCLogBuffer())
#define GAMELOGGEM		(CLogBuffer::GetCLogBuffer())

#endif
