#include "stdh.h"

#include <Engine/Entities/EntityClass.h>
#include <Engine/Entities/EntityProperties.h>

#include "ZoneData.h"

#include <Engine/Interface/UIManager.h>

#define DEF_EXTRA_MAX (30)

CZoneData::~CZoneData()
{
	
}

int CZoneData::LoadZoneDataLod( const char* szFileName )
{
	FILE *fp		= NULL;	

	if ((fp = fopen(szFileName, "rb")) == NULL) 
	{
		MessageBox(NULL, "File is not Exist.", "error!", MB_OK);
		return -1;
	}

	fflush(fp);

	int	i, j;
	int iReadBytes	= 0;

	CZoneInfo* pZoneInfo = CZoneInfo::getSingleton();
	CUIManager* pUIManager = CUIManager::getSingleton();

	//////////////////////////////////////////////////////////////////////////
	// MACRO DEFINITION
	//////////////////////////////////////////////////////////////////////////

#define LOADINT(d)			iReadBytes = fread(&d, sizeof(int), 1, fp);

	int nZoneCnt = 0;
	int nExtraCnt = 0;

	struct stInfo
	{
		int nZoneType;
		int nExtraCnt;
		int	nString;
		char wldFileName[128];
		char texName1[64];
		char texName2[64];
		float fLoadingStep;
		float fTer_Lodmul;
	};

	struct stExtraInfo
	{
		int nString[DEF_EXTRA_MAX];
	};

	stInfo* _pstInfo;
	stExtraInfo* _pstExtra;

	// 존 데이타 정보 읽어오기.
	LOADINT(nZoneCnt);

	if (nZoneCnt <= 0)
	{
		fclose(fp);
		return 0;
	}

	_pstInfo = new stInfo[nZoneCnt];
	iReadBytes = fread(_pstInfo, sizeof(stInfo), nZoneCnt, fp);

	// extra 정보 읽어오기.
	LOADINT(nExtraCnt);

	if (nExtraCnt <= 0)
	{
		SAFE_ARRAY_DELETE(_pstInfo);

		fclose(fp);
		return 0;
	}

	_pstExtra = new stExtraInfo[nExtraCnt];
	iReadBytes = fread(_pstExtra, sizeof(stExtraInfo), nExtraCnt, fp);

	fclose(fp);

	for (i = 0; i < nZoneCnt; ++i)
	{
		CTString strTmp = "data\\world\\";
		stInfo p = _pstInfo[i];

		if (&p == NULL)
			continue;

		strTmp += p.wldFileName;
		pZoneInfo->SetZoneData(i, p.nZoneType, p.nExtraCnt, pUIManager->getText(p.nString), strTmp.str_String, p.fTer_Lodmul);

		pZoneInfo->AddLoadingInfo(p.fLoadingStep, p.texName1, p.texName2);
	}

	for (i = 0; i < nExtraCnt; ++i)
	{
		for (j = 0; j < _pstInfo[i].nExtraCnt; ++j)
		{
			pZoneInfo->SetExtraName(i, j, pUIManager->getText(_pstExtra[i].nString[j]));
		}
	}
	
	SAFE_ARRAY_DELETE(_pstInfo);
	SAFE_ARRAY_DELETE(_pstExtra);

#undef LOADINT

	return iReadBytes;
}
