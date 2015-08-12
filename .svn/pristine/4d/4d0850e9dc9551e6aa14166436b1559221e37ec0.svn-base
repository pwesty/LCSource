#ifndef __PACKCETTIME_H__
#define __PACKCETTIME_H__
#include <stdio.h>

#ifdef PACKET_TIME_CHECK

#define MAX_PACKETTIME   1000

typedef struct
{
	int		index;			// 캐릭터 인텍스(키)
	long	Sec;			// 초
	long	MSec;			// 밀리초
} S_PACKETTIME;

class CPacketTime
{
private:
	FILE*			m_fp;
	S_PACKETTIME	m_PackTIme[MAX_PACKETTIME];
	int				m_iCount;

	bool Add(int index,long Sec,long MSec);
	bool IsSet(int index);
	bool Del(int index);
	bool Get(int index,long& Sec,long& MSec);
	bool Set(int index,long Sec,long MSec);
	void LogFile(int index,char* pLogTitle,struct timeval *Process_time);

public:
	CPacketTime();
	virtual ~CPacketTime();

	bool init(void);
	void AddPackTime(int index);
	void ChkPackTime(int index,char *pLogTitle);
	void Close(void);

};

#endif // PACKET_TIME_CHECK
#endif

//

