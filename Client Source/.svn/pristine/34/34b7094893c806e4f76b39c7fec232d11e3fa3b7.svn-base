#ifndef	PACKMGR_H_
#define	PACKMGR_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif

#ifdef KALYDO
#define WM_KRFREQUEST	WM_USER+100

#include <string>


class ENGINE_API KPackManager
{
public:
	typedef enum __tag_KPM_STAT
	{
		KPMS_ERROR = -1,
		KPMS_DONE = 0,
		KPMS_REQUEST,
		KPMS_COMPLETE,
	}KPMSTAT;
private:
	// non support inherit
	KPackManager() : m_uRequestZoneNo(0), m_enKPMStatus(KPMS_DONE), m_nProgessPercent(0.0f)
																		{																	}
	~KPackManager()														{ release();														}
protected:
	void					release()									{ if( s_pPackMgr ) delete s_pPackMgr;	s_pPackMgr = NULL;			}
	const bool				loadDownloadedPackages( std::string packName );
public:
	void					SetProgressPercent( const int nPercent )	{ this->m_nProgessPercent = nPercent;								}
	void					SetKPMStatus( const KPMSTAT enStat );
	void					SetSessionState( CSessionState* pSession )	{ this->m_pSession = pSession;										}
	CSessionState*			GetSessionState() const						{ return this->m_pSession;											}
	KPMSTAT					GetKPMStatus() const						{ return m_enKPMStatus;												}
	unsigned int			GetZoneNo()	const							{ return m_uRequestZoneNo;											}
	bool					UpdateProgress();
	bool					RequestZoneData( int zoneNo );
	void					SetComplete()								{ m_enKPMStatus = KPMS_DONE;										}
	const bool				IsComplete() const							{ return (m_enKPMStatus == KPMS_DONE);								}
	// 현재는 이렇게. 나중에 상속개념으로 처리.
	static KPackManager*	GetInstance()
	{
		if( NULL == s_pPackMgr )
		{
			s_pPackMgr = new KPackManager();
		}
		
		return s_pPackMgr;
	}

private:
	unsigned int						m_uRequestZoneNo;
	KPMSTAT								m_enKPMStatus;
	int									m_nProgessPercent;
	CSessionState*						m_pSession;
protected:
	static KPackManager*				s_pPackMgr;
public:

};

#endif

#endif //PACKMGR_H_