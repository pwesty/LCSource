#ifndef __LOG_H__
#define __LOG_H__

#include "Descriptor.h"

typedef enum _tagLogHeaderType
{
	LHT_MSG,			// 로그 메시지만 있음
	LHT_MSG_ID,			// 로그 메시지 + 사용자 아이디
	LHT_MSG_ID_NAME,	// 로그 메시지 + 사용자 아이디 + 캐릭터 이름
	LHT_MSG_ID_NICK,	// 로그 메시지 + 사용자 아이디 + 케릭터 이름 + 케릭터 별명
} LOG_HEADER_TYPE;

typedef struct __tagItemLogMani
{
	const CItem*		m_pItem;
	bool				m_bSimple;			// Simple은 index, name, wear, plus, flag, used, count, serial
											// 일반은 simple에 옵션 추가
} ITEMLOGMANI;


class CLogFile;

class CLogHeader
{
public:
	LOG_HEADER_TYPE		m_type;
	const char*			m_msg;
	const char*			m_name;
	const char*			m_nick;
	const char*			m_id;

	CLogHeader(const CLogHeader& src)
	{
		m_type = src.m_type;
		m_msg =	src.m_msg;
		m_name = src.m_name;
		m_nick = src.m_nick;
		m_id = src.m_id;
	}


	CLogHeader(const char* msg, const char* name, const char* nick, const char* id)
	{
		m_type = LHT_MSG_ID_NICK;
		m_msg = msg;
		m_name = name;
		m_nick = nick;
		m_id = id;
	}

	CLogHeader(const char* msg, const char* name, const char* id)
	{
		m_type = LHT_MSG_ID_NAME;
		m_msg = msg;
		m_name = name;
		m_id = id;
	}

	CLogHeader(const char* msg, const char* id)
	{
		m_type = LHT_MSG_ID;
		m_msg = msg;
		m_name = NULL;
		m_id = id;
	}

	CLogHeader(const char* msg)
	{
		m_type = LHT_MSG;
		m_msg = msg;
		m_name = NULL;
		m_id = NULL;
	}
};

class CLogBuffer
{
	char m_buf[10240];
	char m_tbuf[10240];
	CLogFile* m_log;

	void Init() { *m_buf = '\0'; }

public:
	CLogBuffer() { m_log = NULL; Init(); }

	CLogBuffer& operator << (const char* m)
	{
		return ((CLogBuffer&(*)(CLogBuffer*, const char*))0x0811E40C)(this, m);
	}
	CLogBuffer& operator << (int m)
	{
		return ((CLogBuffer&(*)(CLogBuffer*, int))0x0811E42A)(this, m);
	}
	CLogBuffer& operator << (LONGLONG m)
	{
		return ((CLogBuffer&(*)(CLogBuffer*, LONGLONG))0x0811E464)(this, m);
	}
	CLogBuffer& operator << (float m)
	{
		return ((CLogBuffer&(*)(CLogBuffer*, float))0x0811E4B0)(this, m);
	}
	CLogBuffer& operator << (CLogBuffer& (*f)(CLogBuffer&)) { return (*f)(*this); }
	CLogBuffer& operator << (const CLogHeader& lh)
	{
		return ((CLogBuffer&(*)(CLogBuffer*, const CLogHeader&))0x0811E4F2)(this, lh);
	}

	CLogBuffer& operator << (ITEMLOGMANI ilm)
	{
		return ((CLogBuffer&(*)(CLogBuffer*, ITEMLOGMANI))0x0811E6B8)(this, ilm);
	}

	const char* Get() { return (const char*)m_buf; }

	void SetLogFile(CLogFile* logfile) { m_log = logfile; }
	void Write()
	{
		return ((void(*)(CLogBuffer*))0x0811EB5C)(this);
	}
	void Flush();

	friend CLogBuffer& init(CLogBuffer& buf);
	friend class CLogFile;

protected:
	void appendLog(const char* m)
	{
		if(m_buf == NULL || m == NULL ) 	return;

		try
		{
			int len1 = strlen(m_buf);
			int len2 = strlen(m);		//서버 다운 (09.05.11)
			if (len1 + len2 < 10240)
			{
				strcat(m_buf, m);
			}
			else if (len1 + 3 < 10240)
			{
				strcat(m_buf, "N/A");
			}
		}
		catch (...)
		{
			return;
		}

	}
};

class CLogFile
{
	FILE*	m_fp;
	char	m_servercode[7];
	bool	m_bIsDBLog;
	CLogBuffer* m_logBuffer;

	void Close();

public:
	CLogFile();
	~CLogFile();

	void InitLogFile(bool isDBLog, CLogBuffer* destBuffer, const char* servercode = "UC");
	void InitLogFile(int serverno, int subno, int zone, bool isDBLog, CLogBuffer* destBuffer);
	void InitLogFile() { InitLogFile(m_bIsDBLog, m_logBuffer); }
	void Write(CLogBuffer& message);
	void Write(const char* message);

	void Flush() { if (m_fp) fflush(m_fp); }
};

#define GAMELOG			*(CLogBuffer *)(0x0840FC40)
//#define DBLOG			((gThreadIDGameThread == LC_CURRENT_THREAD_ID) ? g_gamelogbuffer : ((gThreadIDDBThread == LC_CURRENT_THREAD_ID) ? g_dblogbuffer : g_gamelogbuffer))

//#define GAMELOG		g_gamelogbuffer
//#define DBLOG		g_dblogbuffer

inline CLogBuffer& init(CLogBuffer& buf)
{
	buf.Init();
	return buf;
}


inline CLogHeader init(const char* msg, CPC* pc)
{
	if (pc == NULL)
		return CLogHeader(msg, "(NO CHAR INFO)");

	return CLogHeader(msg, pc->m_name, pc->m_nick, pc->m_desc->m_idname);
}


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
	//return CLogHeader(msg);
	return ((CLogHeader (*)(int, const char*))0x08054BED)(137613189, msg);
}

inline CLogBuffer& end(CLogBuffer& buf)
{
	buf.Write();
	return buf;
}

inline CLogBuffer& delim(CLogBuffer& buf)
{
	buf << " : ";
	return buf;
}

inline CLogBuffer& flush(CLogBuffer& buf)
{
	buf.Write();
	buf.Flush();
	return buf;
}

inline ITEMLOGMANI itemlog(const CItem* pItem, bool bSimple = false)
{
	ITEMLOGMANI ilm;
	ilm.m_pItem = pItem;
	ilm.m_bSimple = bSimple;
	return ilm;
}

#endif
