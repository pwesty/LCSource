#include "stdhdrs.h"
#include "Log.h"
#include "Cmd.h"
#include "Character.h"
#include "Server.h"
#include "CmdMsg.h"
#include "doFunc.h"
#include "Exp.h"
#include "WarCastle.h"
#include "DBCmd.h"
#include "Utils.h"
#include "PacketTime.h"

#ifdef PACKET_TIME_CHECK

CPacketTime::CPacketTime()
{
	m_fp = NULL;
	m_iCount = 0;

	for(int i=0; i < MAX_PACKETTIME ; i++)
	{
		m_PackTIme[i].index = -1;
		m_PackTIme[i].Sec   = 0;
		m_PackTIme[i].MSec  = 0;
	}
}

CPacketTime::~CPacketTime()
{

}

bool CPacketTime::Add(int index,long Sec,long	MSec)
{
	if(index < 0 && index > MAX_PACKETTIME) return false;

	for(int i=0; i < MAX_PACKETTIME ; i++)
	{
		if(m_PackTIme[i].index == -1)
		{
			m_PackTIme[i].index = index;
			m_PackTIme[i].Sec   = Sec;
			m_PackTIme[i].MSec  = MSec;

			m_iCount++;

			return true;
		}
	}

	return false;
}

bool CPacketTime::IsSet(int index)
{
	for(int i=0; i < MAX_PACKETTIME ; i++)
	{
		if(m_PackTIme[i].index == index)
			return true;
	}

	return false;
}


bool CPacketTime::Del(int index)
{
	if(index < 0 && index > MAX_PACKETTIME) return false;

	for(int i=0; i < MAX_PACKETTIME ; i++)
	{
		if(m_PackTIme[i].index == index)
		{
			m_PackTIme[i].index = -1;
			m_PackTIme[i].Sec   = 0;
			m_PackTIme[i].MSec  = 0;

			m_iCount--;

			return true;
		}
	}

	return false;
}

bool CPacketTime::Get(int index,long& Sec,long& MSec)
{
	if(index < 0 && index > MAX_PACKETTIME) return false;

	for(int i=0; i < MAX_PACKETTIME ; i++)
	{
		if(m_PackTIme[i].index == index)
		{
			Sec  =  m_PackTIme[i].Sec;
			MSec =  m_PackTIme[i].MSec  = 0;

			return true;
		}
	}

	return false;
}

bool CPacketTime::Set(int index,long Sec,long MSec)
{
	if(index < 0 && index > MAX_PACKETTIME) return false;

	for(int i=0; i < MAX_PACKETTIME ; i++)
	{
		if(m_PackTIme[i].index == index)
		{
			m_PackTIme[i].Sec	= Sec;
			m_PackTIme[i].MSec  = MSec;

			return true;
		}
	}

	return false;
}



bool CPacketTime::init(void)
{
	char logstr[128] = "";

//	strcpy(logstr, "../LogFiles/Log/PacketTImeCheck.log");
	strcpy(logstr, "./PacketTImeCheck.log");

	m_fp = fopen(logstr, "at");

	if(m_fp)
		return true;

	return false;
}

void CPacketTime::AddPackTime(int index)
{
	struct timeval  time;
	GetTimeofDay(&time, (struct timezone *) 0);

	if(IsSet(index))
	{
		Set(index,time.tv_sec,time.tv_usec);
	}
	else
	{
		Add(index,time.tv_sec,time.tv_usec);
	}

}

void CPacketTime::ChkPackTime(int index,char *pLogTitle)
{
	struct timeval  endtime;
	struct timeval  starttime;

	GetTimeofDay(&endtime, (struct timezone *) 0);

	long startSec,startMSec;
	startSec = startMSec = 0;

	if(IsSet(index))
	{
		if(Get(index,startSec,startMSec))
		{
			starttime.tv_sec = startSec;
			starttime.tv_usec = startSec;

			//시간 차이 계산(소요시간)
			struct timeval  process_time;

			TimeDiff(&process_time, &starttime, &endtime);

			//로그 기록
			LogFile(index,pLogTitle,&process_time);

			//Del(index);
		}
	}

}

void CPacketTime::LogFile(int index,char* pLogTitle,struct timeval *Process_time)
{
	char buf[1024] = "";

	sprintf(buf,"%d %s Sec:%ld milliSec:%ld", index, pLogTitle , Process_time->tv_sec, Process_time->tv_usec);

	if(m_fp)
	{
		strcat(buf, "\n");
		fputs(buf, m_fp);
	}

	if (m_fp) fflush (m_fp);
}

void CPacketTime::Close(void)
{
	if (m_fp != NULL)
	{
		fclose(m_fp);
		m_fp = NULL;
	}
}

#endif //PACKET_TIME_CHECK
//

