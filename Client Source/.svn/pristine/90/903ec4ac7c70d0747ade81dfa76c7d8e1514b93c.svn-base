#include "stdh.h"

#include <Engine/Base/Stream.h>
#include <Engine/Base/Console.h>

#include "NoticeData.h"

CNoticeData::CNoticeData()
{
}

CNoticeData::~CNoticeData()
{
	m_vScheduleNotice.clear();
}

int CNoticeData::LoadNoticeDataLOD(CTString strFilepath)
{
	FILE*	fpNotice	= NULL;

	m_vScheduleNotice.clear();

	if( fpNotice = fopen(strFilepath.str_String ,"rb") )
	{
		int		_maxCount	= 0;
		int		a_index;
		BOOL	a_enable;
		char	a_title[40];
		char	a_message[120];
		char	a_startTime[50];
		char	a_endTime[50];
		int		a_cycle;
//		char	a_color[10];
//		BOOL	a_enable_link;
//		char	a_link[200];

		int		nTitleLen;
		int		nMessageLen;
		int		nstartLen;
		int		nEndLen;
		int		a_iColor;
//		int		nLinkLen;

		int		iStartYear, iStartMonth, iStartDay, iStartHour, iStartMin, iStartSecond;
		int		iEndYear, iEndMonth, iEndDay, iEndHour, iEndMin, iEndSecond;
				
		time_t		timeStart, timeEnd;
		struct tm	sTime;

		// 이벤트 개수 체크
		fread(&_maxCount, sizeof(int), 1, fpNotice);
		for(int i = 0; i < _maxCount; i++)
		{
			SNotice			sNotice;

			memset(a_title, 0, sizeof(char) * 40);
			memset(a_message, 0, sizeof(char) * 120);
			memset(a_startTime, 0, sizeof(char) * 50);
			memset(a_endTime, 0, sizeof(char) * 50);

			fread(&a_index, sizeof(int), 1, fpNotice);
			fread(&a_enable, sizeof(BOOL), 1, fpNotice);
			fread(&nTitleLen, sizeof(int), 1, fpNotice);
			fread(&a_title, sizeof(char), nTitleLen, fpNotice);
			fread(&nMessageLen, sizeof(int), 1, fpNotice);
			fread(&a_message, sizeof(char), nMessageLen, fpNotice);
			fread(&nstartLen, sizeof(int), 1, fpNotice);
			fread(&a_startTime, sizeof(char), nstartLen, fpNotice);
			fread(&nEndLen, sizeof(int), 1, fpNotice);
			fread(&a_endTime, sizeof(char), nEndLen, fpNotice);
			fread(&a_cycle, sizeof(int), 1, fpNotice);
			fread(&a_iColor, sizeof(int), 1, fpNotice);

			sscanf(a_startTime, "%d-%d-%d %d:%d:%d", &iStartYear, &iStartMonth, &iStartDay, &iStartHour, &iStartMin, &iStartSecond);
			sscanf(a_endTime, "%d-%d-%d %d:%d:%d", &iEndYear, &iEndMonth, &iEndDay, &iEndHour, &iEndMin, &iEndSecond);

			if(iStartYear < 1970 || iEndYear < 1970)
				continue;

			sTime.tm_year		= iStartYear - 1900;
			sTime.tm_mon		= iStartMonth - 1;
			sTime.tm_mday		= iStartDay;
			sTime.tm_hour		= iStartHour;
			sTime.tm_min		= iStartMin;
			sTime.tm_sec		= iStartSecond;
			timeStart			= mktime(&sTime);

			sTime.tm_year		= iEndYear - 1900;
			sTime.tm_mon		= iEndMonth - 1;
			sTime.tm_mday		= iEndDay;
			sTime.tm_hour		= iEndHour;
			sTime.tm_min		= iEndMin;
			sTime.tm_sec		= iEndSecond;
			timeEnd				= mktime(&sTime);

			sNotice.bEnable			= a_enable;
			sNotice.strMessage		= a_message;
			sNotice.timeDateStart	= timeStart;
			sNotice.timeDateEnd		= timeEnd;
			sNotice.dwRepeatDelay	= a_cycle * 1000 * 60;
			sNotice.colMessage		= (COLOR)a_iColor;

			m_vScheduleNotice.push_back(sNotice);
		}
						
	}
	else
	{
		return 0;
	}

	fclose(fpNotice);
	fpNotice		= NULL;

	return 1;
}