#ifndef	_NOTICEDATA_H_
#define _NOTICEDATA_H_

#include <vector>
#include <Engine/DesignClasses/Singleton/SingletonBase.h>

typedef struct _SNotice
{
	BOOL		bEnable;
	CTString	strMessage;
	DWORD		dwRepeatDelay;
	time_t		timeDateStart;
	time_t		timeDateEnd;
	COLOR		colMessage;
	CTString	strLink;

	void operator = (const _SNotice& oData)
	{
		bEnable			= oData.bEnable;
		strMessage		= oData.strMessage;
		dwRepeatDelay	= oData.dwRepeatDelay;
		timeDateStart	= oData.timeDateStart;
		timeDateEnd		= oData.timeDateEnd;
		colMessage		= oData.colMessage;
		strLink			= oData.strLink;
	}
}SNotice;

class CNoticeData : public CSingletonBase< CNoticeData >
{
protected:
	std::vector<SNotice>	m_vScheduleNotice;
	static CNoticeData*		g_cNoticeData;			// singletone

public:
	CNoticeData();
	virtual ~CNoticeData();

public:
	int			LoadNoticeDataLOD(CTString strFilepath);
	int			GetNoticeDataCount()						{ return m_vScheduleNotice.size();		}
	SNotice&	GetNoticeData(INDEX idx)					{ return m_vScheduleNotice[idx];		}
};


#endif	// #ifndef	_NOTICEDATA_H_