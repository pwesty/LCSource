#include "stdh.h"
#include <Engine/Kalydo/PackageManager.h>
#include <string>
#include <Kalydo/KRFReadLib/Include/KRFReadLib.h>

#ifdef KALYDO
extern HWND  _hwndMain;

KPackManager*			KPackManager::s_pPackMgr = NULL;

const bool	KPackManager::loadDownloadedPackages(std::string packName )
{
	std::string temp = packName;
	// double check. [12/2/2011 rumist]
	return ( KRF_SUCCESS( krfLoadPackage( (temp+"_r.krf").c_str() ) ) 
			&& KRF_SUCCESS( krfLoadPackage( (temp+"_o.krf").c_str() ) ) );
}

bool	KPackManager::RequestZoneData(int zoneNo )
{
	char szPath[MAX_PATH] = {0,};
	sprintf( szPath, "%s%d", "zone", zoneNo );
	if( !loadDownloadedPackages(szPath) )
	{
		this->m_enKPMStatus = KPMS_REQUEST;
		PostMessage( _hwndMain, WM_KRFREQUEST, (WPARAM)zoneNo, 0 );
		this->m_uRequestZoneNo = zoneNo;
		this->m_nProgessPercent = 0.0f;
		StartProgress();
		SetProgressDescription(CTString("Downloading World Data"));
		return true;
	}

	return false;
}

bool	KPackManager::UpdateProgress()
{
	CallProgressHook_t( m_nProgessPercent );
	if( this->GetKPMStatus() == KPMS_COMPLETE )
	{
		SetKPMStatus( KPMS_DONE );
		StopProgress();
	}

	return false;
}

void	KPackManager::SetKPMStatus(const KPMSTAT enStat )
{
	this->m_enKPMStatus = enStat;
}

#endif