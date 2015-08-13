#include "stdh.h"


#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Interface/UIRankingViewEX.h>
#include <Engine/Interface/UIRankingClassify.h>
#include <Engine/Entities/InternalClasses.h>
#include <Engine/Interface/UIListBoxMultiList.h>

#define	MAXLENGTH	15

CRankingList::CRankingList()
{
}

CRankingList::~CRankingList()
{
	ClearRankingPoint();
	ClearRankingGuild();
	ClearRankingPointSearch();
	ClearRankingGuildSearch();
}

VSRankingPointData& CRankingList::GetRankingPointByType(ERANKINGEX_SUB_TYPE eSubType)
{
	switch(eSubType)
	{
	case ERANKINGEX_SUB_TYPE_LEVELMASTER:
		return m_vRankLevelMaster;

	case ERANKINGEX_SUB_TYPE_LEVELCLASS_TITAN:
		return m_vRankLevelClass_Titan;

	case ERANKINGEX_SUB_TYPE_LEVELCLASS_KNIGHT:
		return m_vRankLevelClass_Knight;

	case ERANKINGEX_SUB_TYPE_LEVELCLASS_HEALER:
		return m_vRankLevelClass_Healer;

	case ERANKINGEX_SUB_TYPE_LEVELCLASS_MAGE:
		return m_vRankLevelClass_Mage;

	case ERANKINGEX_SUB_TYPE_LEVELCLASS_ROGUE:
		return m_vRankLevelClass_Rogue;

	case ERANKINGEX_SUB_TYPE_LEVELCLASS_SORCERER:
		return m_vRankLevelClass_Sorcerer;

	case ERANKINGEX_SUB_TYPE_LEVELCLASS_NIGHTSHADOW:
		return m_vRankLevelClass_NightShadow;

#ifdef CHAR_EX_ROGUE
	case ERANKINGEX_SUB_TYPE_LEVELCLASS_EX_ROGUE:	// [2012/08/27 : Sora] EX로그 추가
		return m_vRankLevelClass_EXRogue;
#endif
#ifdef CHAR_EX_MAGE
	case ERANKINGEX_SUB_TYPE_LEVELCLASS_EX_MAGE:	// 2013/01/08 jeil EX메이지 추가 
		return m_vRankLevelClass_EXMage;
#endif

	case ERANKINGEX_SUB_TYPE_PK:
		return m_vRankPK;

	case ERANKINGEX_SUB_TYPE_QUESTACUM:
		return m_vRankQuestAcum;

	case ERANKINGEX_SUB_TYPE_QUESTWEEK:
		return m_vRankQuestWeek;
	}

	// default
	return m_vRankLevelMaster;
}

VSRankingGuildData& CRankingList::GetRankingGuildByType(ERANKINGEX_SUB_TYPE eSubType)
{
	switch(eSubType)
	{
	case ERANKINGEX_SUB_TYPE_GUILD:
		return m_vRankGuild;
	}

	return m_vRankGuild;
}

std::map<std::string, VSRankingPointData>& CRankingList::GetRankingPointSearchType(ERANKINGEX_SUB_TYPE eSubType)
{
	switch(eSubType)
	{
	case ERANKINGEX_SUB_TYPE_LEVELMASTER:
		return m_mapRankSearchLevelMaster;

	case ERANKINGEX_SUB_TYPE_LEVELCLASS_TITAN:
		return m_mapRankSearchLevelClass_Titan;

	case ERANKINGEX_SUB_TYPE_LEVELCLASS_KNIGHT:
		return m_mapRankSearchLevelClass_Knight;

	case ERANKINGEX_SUB_TYPE_LEVELCLASS_HEALER:
		return m_mapRankSearchLevelClass_Healer;

	case ERANKINGEX_SUB_TYPE_LEVELCLASS_MAGE:
		return m_mapRankSearchLevelClass_Mage;

	case ERANKINGEX_SUB_TYPE_LEVELCLASS_ROGUE:
		return m_mapRankSearchLevelClass_Rogue;

	case ERANKINGEX_SUB_TYPE_LEVELCLASS_SORCERER:
		return m_mapRankSearchLevelClass_Sorcerer;

	case ERANKINGEX_SUB_TYPE_LEVELCLASS_NIGHTSHADOW:
		return m_mapRankSearchLevelClass_NightShadow;

#ifdef CHAR_EX_ROGUE
	case ERANKINGEX_SUB_TYPE_LEVELCLASS_EX_ROGUE:	// [2012/08/27 : Sora] EX로그 추가
		return m_mapRankSearchLevelClass_EXRogue;
#endif
#ifdef CHAR_EX_MAGE
	case ERANKINGEX_SUB_TYPE_LEVELCLASS_EX_MAGE:	// 2013/01/08 jeil EX메이지 추가
		return m_mapRankSearchLevelClass_EXMage;
#endif

	case ERANKINGEX_SUB_TYPE_PK:
		return m_mapRankSearchPK;

	case ERANKINGEX_SUB_TYPE_QUESTACUM:
		return m_mapRankSearchQuestAcum;

	case ERANKINGEX_SUB_TYPE_QUESTWEEK:
		return m_mapRankSearchQuestWeek;
	}

	// default
	return m_mapRankSearchLevelMaster;
}

std::map<std::string, VSRankingGuildData>& CRankingList::GetRankingGuildSearchType(ERANKINGEX_SUB_TYPE eSubType)
{
	switch(eSubType)
	{
	case ERANKINGEX_SUB_TYPE_GUILD:
		return m_mapRankSearchGuild;
	}

	return m_mapRankSearchGuild;
}

void CRankingList::AddRankingPoint(ERANKINGEX_SUB_TYPE eSubType, SRankingPointData oData)
{
	VSRankingPointData&		vData	= GetRankingPointByType(eSubType);

	vData.push_back(oData);
}

void CRankingList::AddRankingGuild(ERANKINGEX_SUB_TYPE eSubType, SRankingGuildData oData)
{
	VSRankingGuildData&		vData	= GetRankingGuildByType(eSubType);

	vData.push_back(oData);
}

void CRankingList::AddRankingPointSearch(ERANKINGEX_SUB_TYPE eSubType, SRankingPointData oData, CTString cstrName)
{
	std::map<std::string, VSRankingPointData>&				mapData		= GetRankingPointSearchType(eSubType);
	std::map<std::string, VSRankingPointData>::iterator		itmapData	= mapData.begin();

	BOOL		bExistData	= FALSE;
	for(; itmapData != mapData.end(); itmapData++)
	{
		if((itmapData->first).c_str() == cstrName)
		{
			bExistData		= TRUE;
			break;
		}
	}

	if(bExistData == TRUE)
	{	// 기존 검색한 기록이 있다
		VSRankingPointData&		vData	= itmapData->second;

		vData.push_back(oData);
	}
	else
	{
		std::string				strTemp	= cstrName.str_String;
		VSRankingPointData		vData;

		vData.push_back(oData);
		mapData.insert(std::map<std::string, VSRankingPointData>::value_type(strTemp, vData));
	}
}

void CRankingList::AddRankingPointGuild(ERANKINGEX_SUB_TYPE eSubType, SRankingGuildData oData, CTString cstrName)
{
}

void CRankingList::ClearRankingPoint()
{
	m_vRankLevelMaster.clear();
	m_vRankLevelClass_Titan.clear();
	m_vRankLevelClass_Knight.clear();
	m_vRankLevelClass_Healer.clear();
	m_vRankLevelClass_Mage.clear();
	m_vRankLevelClass_Rogue.clear();
	m_vRankLevelClass_Sorcerer.clear();
	m_vRankLevelClass_NightShadow.clear();
#ifdef CHAR_EX_ROGUE
	m_vRankLevelClass_EXRogue.clear();	// [2012/08/27 : Sora] EX로그 추가
#endif
#ifdef CHAR_EX_MAGE
	m_vRankLevelClass_EXMage.clear();	// 2013/01/08 jeil EX메이지 추가
#endif
	m_vRankPK.clear();
	m_vRankQuestAcum.clear();
	m_vRankQuestWeek.clear();
}

void CRankingList::ClearRankingGuild()
{
	m_vRankGuild.clear();
}

void CRankingList::ClearRankingPointSearch()
{
	m_mapRankSearchLevelMaster.clear();
	m_mapRankSearchLevelClass_Titan.clear();
	m_mapRankSearchLevelClass_Knight.clear();
	m_mapRankSearchLevelClass_Healer.clear();
	m_mapRankSearchLevelClass_Mage.clear();
	m_mapRankSearchLevelClass_Rogue.clear();
	m_mapRankSearchLevelClass_Sorcerer.clear();
	m_mapRankSearchLevelClass_NightShadow.clear();
#ifdef CHAR_EX_ROGUE
	m_mapRankSearchLevelClass_EXRogue.clear();	// [2012/08/27 : Sora] EX로그 추가
#endif
#ifdef CHAR_EX_MAGE
	m_mapRankSearchLevelClass_EXMage.clear();	// 2013/01/08 jeil EX메이지 추가 
#endif
	m_mapRankSearchPK.clear();	
	m_mapRankSearchQuestAcum.clear();
	m_mapRankSearchQuestWeek.clear();
}

void CRankingList::ClearRankingGuildSearch()
{
	m_mapRankSearchGuild.clear();
}

int CRankingList::GetCountRankPoint(ERANKINGEX_SUB_TYPE eSubType)
{
	VSRankingPointData&		vData	= GetRankingPointByType(eSubType);

	return vData.size();
}

int CRankingList::GetCountRankGuild(ERANKINGEX_SUB_TYPE eSubType)
{
	VSRankingGuildData&		vData	= GetRankingGuildByType(eSubType);

	return vData.size();
}

int CRankingList::GetCountRankPointSearch(CTString cstrName, ERANKINGEX_SUB_TYPE eSubType)
{
	std::map<std::string, VSRankingPointData>&			mapData		= GetRankingPointSearchType(eSubType);
	std::map<std::string, VSRankingPointData>::iterator	itmapData	= mapData.begin();

	BOOL		bExistData	= FALSE;
	for(; itmapData != mapData.end(); itmapData++)
	{
		if((itmapData->first).c_str() == cstrName)
		{
			bExistData		= TRUE;
			break;
		}
	}

	if(bExistData == FALSE)
		return 0;

	return (itmapData->second).size();
}

int CRankingList::GetCountRankGuildSearch(CTString cstrName, ERANKINGEX_SUB_TYPE eSubType)
{
	std::map<std::string, VSRankingGuildData>&			mapData		= GetRankingGuildSearchType(eSubType);
	std::map<std::string, VSRankingGuildData>::iterator	itmapData	= mapData.begin();

	BOOL		bExistData	= FALSE;
	for(; itmapData != mapData.end(); itmapData++)
	{
		if((itmapData->first).c_str() == cstrName)
		{
			bExistData		= TRUE;
			break;
		}
	}

	if(bExistData == FALSE)
		return 0;

	return (itmapData->second).size();
}

BOOL CRankingList::GetRankPoint(SRankingPointData* poData, INDEX idxRank, ERANKINGEX_SUB_TYPE eSubType)
{
	VSRankingPointData&		vData	= GetRankingPointByType(eSubType);

	if(idxRank < 0 || idxRank >= vData.size())
		return FALSE;

	*poData		= vData[idxRank];

	return TRUE;
}

BOOL CRankingList::GetRankPointSearch(SRankingPointData* poData, CTString cstrName, INDEX idxRank, ERANKINGEX_SUB_TYPE eSubType)
{
	std::map<std::string, VSRankingPointData>&			mapData		= GetRankingPointSearchType(eSubType);
	std::map<std::string, VSRankingPointData>::iterator	itmapData	= mapData.begin();

	BOOL		bExistData	= FALSE;
	for(; itmapData != mapData.end(); itmapData++)
	{
		if((itmapData->first).c_str() == cstrName)
		{
			bExistData		= TRUE;
			break;
		}
	}

	if(bExistData == FALSE)
		return FALSE;

	VSRankingPointData&		vRankData	= itmapData->second;

	if(idxRank < 0 || idxRank >= vRankData.size())
		return FALSE;

	*poData		= vRankData[idxRank];

	return TRUE;
}

BOOL CRankingList::GetRankGuild(SRankingGuildData* poData, INDEX idxRank, ERANKINGEX_SUB_TYPE eSubType)
{
	VSRankingGuildData&		vData	= GetRankingGuildByType(eSubType);

	if(idxRank < 0 || idxRank >= vData.size())
		return FALSE;

	*poData		= vData[idxRank];

	return TRUE;
}

BOOL CRankingList::GetRankGuildSearch(SRankingGuildData* poData, CTString cstrName, INDEX idxRank, ERANKINGEX_SUB_TYPE eSubType)
{
	std::map<std::string, VSRankingGuildData>&			mapData		= GetRankingGuildSearchType(eSubType);
	std::map<std::string, VSRankingGuildData>::iterator	itmapData	= mapData.begin();

	BOOL		bExistData	= FALSE;
	for(; itmapData != mapData.end(); itmapData++)
	{
		if((itmapData->first).c_str() == cstrName)
		{
			bExistData		= TRUE;
			break;
		}
	}

	if(bExistData == FALSE)
		return FALSE;

	VSRankingGuildData&		vRankData	= itmapData->second;

	if(idxRank < 0 || idxRank >= vRankData.size())
		return FALSE;

	*poData		= vRankData[idxRank];

	return TRUE;
}


// =======================================================================================
// CUIRankingViewEx


CUIRankingViewEX::CUIRankingViewEX( int nClassID )
{
	m_iClassID				= nClassID;
	m_eSubType				= _ERANKINGEX_SUB_TYPE_END;

	m_nRecvRankTotalCount	= 0;
	m_nRecvRankCount		= 0;
	m_bRecvRank				= FALSE;
}

CUIRankingViewEX::~CUIRankingViewEX()
{
	Destroy();

	STOCK_RELEASE( m_ptexBaseInterface );
	STOCK_RELEASE( m_ptexBaseMessageBox );
	STOCK_RELEASE( m_ptexBaseCashshop );
	STOCK_RELEASE( m_ptexBaseRanking );
}

CTString CUIRankingViewEX::GetJobStringByType(UBYTE ubClass)
{
	CTString		cstrTemp;

	switch(ubClass)
	{
	case TITAN:
		cstrTemp	= _S(43, "타이탄");
		break;

	case KNIGHT:
		cstrTemp	= _S(44, "기사");
		break;

	case HEALER:
		cstrTemp	= _S(45, "힐러");
		break;

	case MAGE:
		cstrTemp	= _S(46, "메이지");
		break;

	case ROGUE:
		cstrTemp	= _S(47, "로그");
		break;

	case SORCERER:
		cstrTemp	= _S(48, "소서러");
		break;

	case NIGHTSHADOW:
		cstrTemp	= _S(4410, "나이트셰도우");
		break;

#ifdef CHAR_EX_ROGUE
	case EX_ROGUE:	// [2012/08/27 : Sora] EX로그 추가
		cstrTemp	= _S( 5732, "EX로그" );
		break;
#endif
#ifdef  CHAR_EX_MAGE
	case EX_MAGE:	//2013/01/08 jeil EX메이지 추가 스트링 받으면 추가 수정 필요 
		cstrTemp	= _S( 5820, "아크메이지" );
		break;
#endif

	default:
		break;
	}

	return cstrTemp;
}

CTString CUIRankingViewEX::GetRankStringByType(ERANKINGEX_SUB_TYPE eSubType)
{
	switch(eSubType)
	{
	case ERANKINGEX_SUB_TYPE_LEVELMASTER:
		return _S(5189, "랭킹 마스터");

	case ERANKINGEX_SUB_TYPE_LEVELCLASS_TITAN:
		return _S(5190, "타이탄 랭킹");

	case ERANKINGEX_SUB_TYPE_LEVELCLASS_KNIGHT:
		return _S(5191, "나이트 랭킹");

	case ERANKINGEX_SUB_TYPE_LEVELCLASS_HEALER:
		return _S(5192, "힐러 랭킹");

	case ERANKINGEX_SUB_TYPE_LEVELCLASS_MAGE:
		return _S(5193, "메이지 랭킹");

	case ERANKINGEX_SUB_TYPE_LEVELCLASS_ROGUE:
		return _S(5194, "로그 랭킹");

	case ERANKINGEX_SUB_TYPE_LEVELCLASS_SORCERER:
		return _S(5195, "소서러 랭킹");

	case ERANKINGEX_SUB_TYPE_LEVELCLASS_NIGHTSHADOW:
		return _S(5196, "나이트셰도우 랭킹");

#ifdef CHAR_EX_ROGUE
	case ERANKINGEX_SUB_TYPE_LEVELCLASS_EX_ROGUE:	// [2012/08/27 : Sora] EX로그 추가
		return _S( 5737,"EX로그 랭킹");
#endif
#ifdef CHAR_EX_ROGUE
	case ERANKINGEX_SUB_TYPE_LEVELCLASS_EX_MAGE:	// 2013/01/08 jeil EX메이지 추가 수정 스트링 받으면 추가 수정 필요 
		return _S( 5825,"아크메이지 랭킹");
#endif
	}

	return "";
}

UCHAR CUIRankingViewEX::GetRankingNetworkType()
{
	switch(m_eSubType)
	{
	case ERANKINGEX_SUB_TYPE_LEVELMASTER:
		return 0;

	case ERANKINGEX_SUB_TYPE_LEVELCLASS_TITAN:
	case ERANKINGEX_SUB_TYPE_LEVELCLASS_KNIGHT:
	case ERANKINGEX_SUB_TYPE_LEVELCLASS_HEALER:
	case ERANKINGEX_SUB_TYPE_LEVELCLASS_MAGE:
	case ERANKINGEX_SUB_TYPE_LEVELCLASS_ROGUE:
	case ERANKINGEX_SUB_TYPE_LEVELCLASS_SORCERER:
	case ERANKINGEX_SUB_TYPE_LEVELCLASS_NIGHTSHADOW:
#ifdef CHAR_EX_ROGUE
	case ERANKINGEX_SUB_TYPE_LEVELCLASS_EX_ROGUE:	// [2012/08/27 : Sora] EX로그 추가
#endif
#ifdef CHAR_EX_MAGE
	case ERANKINGEX_SUB_TYPE_LEVELCLASS_EX_MAGE:	// 2013/01/08 jeil EX메이지 추가 
#endif
		return 1;
	}

	return 0;
}

UCHAR CUIRankingViewEX::GetRankingNetworkSubType()
{
	switch(m_eSubType)
	{
	case ERANKINGEX_SUB_TYPE_LEVELCLASS_TITAN:
		return TITAN;

	case ERANKINGEX_SUB_TYPE_LEVELCLASS_KNIGHT:
		return KNIGHT;

	case ERANKINGEX_SUB_TYPE_LEVELCLASS_HEALER:
		return HEALER;

	case ERANKINGEX_SUB_TYPE_LEVELCLASS_MAGE:
		return MAGE;

	case ERANKINGEX_SUB_TYPE_LEVELCLASS_ROGUE:
		return ROGUE;

	case ERANKINGEX_SUB_TYPE_LEVELCLASS_SORCERER:
		return SORCERER;

	case ERANKINGEX_SUB_TYPE_LEVELCLASS_NIGHTSHADOW:
		return NIGHTSHADOW;

#ifdef CHAR_EX_ROGUE
	case ERANKINGEX_SUB_TYPE_LEVELCLASS_EX_ROGUE:	// [2012/08/27 : Sora] EX로그 추가
		return EX_ROGUE;
#endif
#ifdef CHAR_EX_MAGE
	case ERANKINGEX_SUB_TYPE_LEVELCLASS_EX_MAGE:	// 2013/01/08 jeil EX메이지 추가 
		return EX_MAGE;
#endif
	}

	return TITAN;
}

void CUIRankingViewEX::Create(CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight)
{
	CUIWindow::Create(pParentWnd, nX, nY, nWidth, nHeight);

	m_ptexBaseInterface				= CreateTexture(CTString("Data\\Interface\\new_interface.tex"));
	m_ptexBaseMessageBox			= CreateTexture(CTString("Data\\Interface\\MessageBox.tex"));
	m_ptexBaseCashshop				= CreateTexture(CTString("Data\\Interface\\Cashshop.tex"));
	m_ptexBaseRanking				= CreateTexture(CTString("Data\\Interface\\ranking.tex"));

	float	fTexWidthBaseInterface	= m_ptexBaseInterface->GetPixWidth();
	float	fTexHeightBaseInterface	= m_ptexBaseInterface->GetPixHeight();
	float	fTexWidthBaseMessageBox	= m_ptexBaseMessageBox->GetPixWidth();
	float	fTexHeightBaseMessageBox= m_ptexBaseMessageBox->GetPixHeight();
	float	fTexWidthBaseCashshop	= m_ptexBaseCashshop->GetPixWidth();
	float	fTexHeightBaseCashshop	= m_ptexBaseCashshop->GetPixHeight();
	float	fTexWidthBaseRanking	= m_ptexBaseRanking->GetPixWidth();
	float	fTexHeightBaseRanking	= m_ptexBaseRanking->GetPixHeight();

	// RECT
	// 타이틀 영역
	m_rcTitleArea.SetRect(0, 0, nWidth, 43);

	// UI 텍스처 UV 지정
	// 그리기 영역 지정
	// Left : xpos, Top : ypos, Right : xpos + width, Bottom : ypos + height
	// UI 상단 부분
	m_rcBackT[0].SetRect(0, 0, 191, 43);						// left
	m_rcBackT[1].SetRect(191, 0, nWidth - 191, 43);				// middle
	m_rcBackT[2].SetRect(m_rcBackT[1].Right, 0, nWidth, 43);	// right
	m_rcuvBackTL.SetUV(428, 0, 619, 43, fTexWidthBaseInterface, fTexHeightBaseInterface);
	m_rcuvBackTM.SetUV(619, 0, 752, 43, fTexWidthBaseInterface, fTexHeightBaseInterface);
	m_rcuvBackTR.SetUV(752, 0, 943, 43, fTexWidthBaseInterface, fTexHeightBaseInterface);
	// UI 중단 부분
	m_rcBackM[0].SetRect(0, 43, 4, nHeight - 11);		// left
	m_rcBackM[1].SetRect(4, 43, nWidth - 4, nHeight - 11);		// right
	m_rcBackM[2].SetRect(m_rcBackM[1].Right, 43, nWidth, nHeight - 11);		// right
	m_rcuvBackML.SetUV(428, 51, 432, 55, fTexWidthBaseInterface, fTexHeightBaseInterface);
	m_rcuvBackMM.SetUV(440, 51, 450, 55, fTexWidthBaseInterface, fTexHeightBaseInterface);
	m_rcuvBackMR.SetUV(939, 51, 943, 55, fTexWidthBaseInterface, fTexHeightBaseInterface);
	// UI 하단 부분
	m_rcBackB[0].SetRect(0, m_rcBackM[0].Bottom, 11, nHeight);		// left
	m_rcBackB[1].SetRect(11, m_rcBackM[0].Bottom, nWidth - 11, nHeight);		// middle
	m_rcBackB[2].SetRect(m_rcBackB[1].Right, m_rcBackM[0].Bottom, nWidth, nHeight);		// right
	m_rcuvBackBL.SetUV(428, 149, 439, 160, fTexWidthBaseInterface, fTexHeightBaseInterface);
	m_rcuvBackBM.SetUV(450, 149, 455, 160, fTexWidthBaseInterface, fTexHeightBaseInterface);
	m_rcuvBackBR.SetUV(932, 149, 943, 160, fTexWidthBaseInterface, fTexHeightBaseInterface);

	// 안쪽의 뒷배경
	m_rcBackgroundInside[0].SetRect(8, 62, 13, 67);						// left top
	m_rcBackgroundInside[1].SetRect(13, 62, nWidth - 13, 67);			// top
	m_rcBackgroundInside[2].SetRect(nWidth - 13, 62, nWidth - 8, 67);	// right top
	m_rcBackgroundInside[3].SetRect(8, 67, 13, nHeight - 43);			// left middle
	m_rcBackgroundInside[4].SetRect(13, 67, nWidth - 13, nHeight - 43);	// middle
	m_rcBackgroundInside[5].SetRect(nWidth - 13, 67, nWidth - 8, nHeight - 43);	// right middle
	m_rcBackgroundInside[6].SetRect(8, nHeight - 43, 13, nHeight - 38);			// left bottom
	m_rcBackgroundInside[7].SetRect(13, nHeight - 43, nWidth - 13, nHeight - 38);	// bottom
	m_rcBackgroundInside[8].SetRect(nWidth - 13, nHeight - 43, nWidth - 8, nHeight - 38);	// bottom
	m_rcuvBackgoundInside[0].SetUV(294, 153, 300, 159, fTexWidthBaseCashshop, fTexHeightBaseCashshop);
	m_rcuvBackgoundInside[1].SetUV(299, 153, 379, 159, fTexWidthBaseCashshop, fTexHeightBaseCashshop);
	m_rcuvBackgoundInside[2].SetUV(378, 153, 384, 159, fTexWidthBaseCashshop, fTexHeightBaseCashshop);
	m_rcuvBackgoundInside[3].SetUV(294, 158, 300, 224, fTexWidthBaseCashshop, fTexHeightBaseCashshop);
	m_rcuvBackgoundInside[4].SetUV(299, 158, 379, 224, fTexWidthBaseCashshop, fTexHeightBaseCashshop);
	m_rcuvBackgoundInside[5].SetUV(378, 158, 384, 224, fTexWidthBaseCashshop, fTexHeightBaseCashshop);
	m_rcuvBackgoundInside[6].SetUV(294, 223, 300, 229, fTexWidthBaseCashshop, fTexHeightBaseCashshop);
	m_rcuvBackgoundInside[7].SetUV(299, 223, 379, 229, fTexWidthBaseCashshop, fTexHeightBaseCashshop);
	m_rcuvBackgoundInside[8].SetUV(378, 223, 384, 229, fTexWidthBaseCashshop, fTexHeightBaseCashshop);

	// 검색 박스 테두리
	m_rcSearchBox[0].SetRect(221, 392, 227, 412);
	m_rcSearchBox[1].SetRect(227, 392, 370, 412);
	m_rcSearchBox[2].SetRect(370, 392, 376, 412);
	m_rcuvSearchBox[0].SetUV(39, 236, 45, 256, fTexWidthBaseCashshop, fTexHeightBaseCashshop);
	m_rcuvSearchBox[1].SetUV(46, 236, 150, 256, fTexWidthBaseCashshop, fTexHeightBaseCashshop);
	m_rcuvSearchBox[2].SetUV(151, 236, 157, 256, fTexWidthBaseCashshop, fTexHeightBaseCashshop);

	m_rcRankTypeBox.SetRect(8, 36, 205, 63);
	m_rcuvRankTypeBox.SetUV(0, 50, 198, 78, fTexWidthBaseRanking, fTexHeightBaseRanking);

	// 프로그래시브 박스
	m_rcProgressBox.SetRect(151, 320, 551, 364);
	m_rcuvProgressBox.SetUV(0, 0, 401, 45, fTexWidthBaseRanking, fTexHeightBaseRanking);
	m_prbProgress.Create(this, 162, 333, 378, 18);
	m_prbProgress.SetProgressRange(0, 100);
	m_prbProgress.SetProgressStep(1);
	m_prbProgress.OffsetProgressPos(50);
	m_prbProgress.SetAutoComplete(FALSE);
	m_prbProgress.SetIdleText(_S(5598, "생성중"), TRUE, 0xF2F2F2FF);
	m_prbProgress.SetDoingText(_S(5598, "생성중"), TRUE, 0xF2F2F2FF);
	m_prbProgress.SetCompleteText(_S(5598, "생성중"), TRUE, 0xF2F2F2FF);

	// UI Control Creates

	// 종료 버튼
	m_btnClose.Create(this, CTString(""), (nWidth - 7) - 14, 4, 14, 14);
	m_btnClose.SetUV(UBS_CLICK, 219, 0, 233, 14, fTexWidthBaseMessageBox, fTexHeightBaseMessageBox);
	m_btnClose.SetUV(UBS_IDLE, 234, 0, 248, 14, fTexWidthBaseMessageBox, fTexHeightBaseMessageBox);
	m_btnClose.CopyUV(UBS_IDLE, UBS_ON);
	m_btnClose.CopyUV(UBS_IDLE, UBS_DISABLE);

	// 랭킹 종류 콤보박스
	m_cbRankType.Create(this, 29, 40, 155, 20, 139, 4, 13, 13, 10, _pUIFontTexMgr->GetFontHeight(), 4, 4);
	m_cbRankType.SetDownBtnUV(420, 0, 434, 14, fTexWidthBaseRanking, fTexHeightBaseRanking);
	m_cbRankType.SetUpBtnUV(450, 0, 464, 14, fTexWidthBaseRanking, fTexHeightBaseRanking);
	m_cbRankType.SetDropListUV(21, 54, 177, 75, fTexWidthBaseRanking, fTexHeightBaseRanking);
	for(int eType = ERANKINGEX_SUB_TYPE_LEVELMASTER; eType < _ERANKINGEX_SUB_TYPE_END; eType++)
	{
		m_cbRankType.AddString(GetRankStringByType((ERANKINGEX_SUB_TYPE)eType));
	}

	// 초기화 버튼
	m_btnReset.Create(this, _S(194, "초기화"), (nWidth - 18) - 84, 38, 84, 21);
	m_btnReset.SetUV(UBS_IDLE, 282, 244, 352, 266, fTexWidthBaseInterface, fTexHeightBaseInterface);
	m_btnReset.SetUV(UBS_CLICK, 364, 244, 434, 266, fTexWidthBaseInterface, fTexHeightBaseInterface);
	m_btnReset.CopyUV(UBS_IDLE, UBS_ON);
	m_btnReset.CopyUV(UBS_IDLE, UBS_DISABLE);

	// 검색 버튼
	m_btnSearch.Create(this, _S(386, "검색"), 378, 391, 84, 21);
	m_btnSearch.SetUV(UBS_IDLE, 282, 244, 352, 266, fTexWidthBaseInterface, fTexHeightBaseInterface);
	m_btnSearch.SetUV(UBS_CLICK, 364, 244, 434, 266, fTexWidthBaseInterface, fTexHeightBaseInterface);
	m_btnSearch.CopyUV(UBS_IDLE, UBS_ON);
	m_btnSearch.CopyUV(UBS_IDLE, UBS_DISABLE);

	// 검색 에디트박스
	m_ebSearch.Create(this, 229, 392, 150, 20, MAXLENGTH + 1);
	m_ebSearch.SetReadingWindowUV(282, 244, 352, 266, fTexWidthBaseInterface, fTexHeightBaseInterface);
	m_ebSearch.SetCandidateUV(282, 244, 352, 266, fTexWidthBaseInterface, fTexHeightBaseInterface);
	m_ebSearch.SetFocus(FALSE);

	// 목록 상단
	m_lbTopRanking.Create(this, 13, 86, 644, 133, _pUIFontTexMgr->GetLineHeight(), 13, 3, 10, 8, TRUE);
	m_lbTopRanking.CreateScroll(TRUE, 2, 0, 9, 133, 9, 7, 0, 0, 5);
	m_lbTopRanking.SetOverColor(0xF8E1B5FF);
	m_lbTopRanking.SetSelectColor(0xF8E1B5FF);
	// scroll up button
	m_lbTopRanking.SetScrollUpUV(UBS_IDLE, 134, 439, 144, 449, fTexWidthBaseInterface, fTexHeightBaseInterface);
	m_lbTopRanking.SetScrollUpUV(UBS_CLICK, 146, 439, 156, 449, fTexWidthBaseInterface, fTexHeightBaseInterface);
	m_lbTopRanking.CopyScrollUpUV(UBS_IDLE, UBS_ON);
	m_lbTopRanking.CopyScrollUpUV(UBS_IDLE, UBS_DISABLE);
	// scroll bar
	m_lbTopRanking.SetScrollBarTopUV(163, 438, 173, 446, fTexWidthBaseInterface, fTexHeightBaseInterface);
	m_lbTopRanking.SetScrollBarMiddleUV(163, 446, 173, 470, fTexWidthBaseInterface, fTexHeightBaseInterface);
	m_lbTopRanking.SetScrollBarBottomUV(163, 470, 173, 478, fTexWidthBaseInterface, fTexHeightBaseInterface);
	// scroll down button
	m_lbTopRanking.SetScrollDownUV(UBS_IDLE, 134, 451, 144, 461, fTexWidthBaseInterface, fTexHeightBaseInterface);
	m_lbTopRanking.SetScrollDownUV(UBS_CLICK, 146, 451, 156, 461, fTexWidthBaseInterface, fTexHeightBaseInterface);
	m_lbTopRanking.CopyScrollDownUV(UBS_IDLE, UBS_ON);
	m_lbTopRanking.CopyScrollDownUV(UBS_IDLE, UBS_DISABLE);

	// 목록 하단
	m_lbBottomRanking.Create(this, 13, 246, 644, 133, _pUIFontTexMgr->GetLineHeight(), 13, 3, 10, 8, TRUE);
	m_lbBottomRanking.CreateScroll(TRUE, 2, 0, 9, 133, 9, 7, 0, 0, 5);
	m_lbBottomRanking.SetOverColor(0xF8E1B5FF);
	m_lbTopRanking.SetSelectColor(0xF8E1B5FF);
	// scroll up button
	m_lbBottomRanking.SetScrollUpUV(UBS_IDLE, 134, 439, 144, 449, fTexWidthBaseInterface, fTexHeightBaseInterface);
	m_lbBottomRanking.SetScrollUpUV(UBS_CLICK, 146, 439, 156, 449, fTexWidthBaseInterface, fTexHeightBaseInterface);
	m_lbBottomRanking.CopyScrollUpUV(UBS_IDLE, UBS_ON);
	m_lbBottomRanking.CopyScrollUpUV(UBS_IDLE, UBS_DISABLE);
	// scroll bar
	m_lbBottomRanking.SetScrollBarTopUV(163, 438, 173, 446, fTexWidthBaseInterface, fTexHeightBaseInterface);
	m_lbBottomRanking.SetScrollBarMiddleUV(163, 446, 173, 470, fTexWidthBaseInterface, fTexHeightBaseInterface);
	m_lbBottomRanking.SetScrollBarBottomUV(163, 470, 173, 478, fTexWidthBaseInterface, fTexHeightBaseInterface);
	// scroll down button
	m_lbBottomRanking.SetScrollDownUV(UBS_IDLE, 134, 451, 144, 461, fTexWidthBaseInterface, fTexHeightBaseInterface);
	m_lbBottomRanking.SetScrollDownUV(UBS_CLICK, 146, 451, 156, 461, fTexWidthBaseInterface, fTexHeightBaseInterface);
	m_lbBottomRanking.CopyScrollDownUV(UBS_IDLE, UBS_ON);
	m_lbBottomRanking.CopyScrollDownUV(UBS_IDLE, UBS_DISABLE);

	for(int i = 0; i < _ERANKINGEX_SUB_TYPE_END; i++)
	{
		m_rcListTitleWidth[i][0]		= 127;		// 아이디
		m_rcListTitleWidth[i][1]		= 66;		// 전주 랭킹
		m_rcListTitleWidth[i][2]		= 66;		// 금주 랭킹
		m_rcListTitleWidth[i][3]		= 72;		// 전주 달성값
		m_rcListTitleWidth[i][4]		= 72;		// 금주 달성값
		m_rcListTitleWidth[i][5]		= 47;		// 레벨
		m_rcListTitleWidth[i][6]		= 127;		// 직업
		m_rcListTitleWidth[i][7]		= 67;		// 달성날짜
	}

	m_lbTopRanking.SetRowWidth(0, m_rcListTitleWidth[0][0]);
	m_lbTopRanking.SetRowWidth(1, m_rcListTitleWidth[0][1]);
	m_lbTopRanking.SetRowWidth(2, m_rcListTitleWidth[0][2]);
	m_lbTopRanking.SetRowWidth(3, m_rcListTitleWidth[0][3]);
	m_lbTopRanking.SetRowWidth(4, m_rcListTitleWidth[0][4]);
	m_lbTopRanking.SetRowWidth(5, m_rcListTitleWidth[0][5]);
	m_lbTopRanking.SetRowWidth(6, m_rcListTitleWidth[0][6]);
	m_lbTopRanking.SetRowWidth(7, m_rcListTitleWidth[0][7]);
	m_lbTopRanking.SetRowAlign(0, TEXT_CENTER);
	m_lbTopRanking.SetRowAlign(1, TEXT_CENTER);
	m_lbTopRanking.SetRowAlign(2, TEXT_CENTER);
	m_lbTopRanking.SetRowAlign(3, TEXT_CENTER);
	m_lbTopRanking.SetRowAlign(4, TEXT_CENTER);
	m_lbTopRanking.SetRowAlign(5, TEXT_CENTER);
	m_lbTopRanking.SetRowAlign(6, TEXT_CENTER);
	m_lbTopRanking.SetRowAlign(7, TEXT_CENTER);

	m_lbBottomRanking.SetRowWidth(0, m_rcListTitleWidth[0][0]);
	m_lbBottomRanking.SetRowWidth(1, m_rcListTitleWidth[0][1]);
	m_lbBottomRanking.SetRowWidth(2, m_rcListTitleWidth[0][2]);
	m_lbBottomRanking.SetRowWidth(3, m_rcListTitleWidth[0][3]);
	m_lbBottomRanking.SetRowWidth(4, m_rcListTitleWidth[0][4]);
	m_lbBottomRanking.SetRowWidth(5, m_rcListTitleWidth[0][5]);
	m_lbBottomRanking.SetRowWidth(6, m_rcListTitleWidth[0][6]);
	m_lbBottomRanking.SetRowWidth(7, m_rcListTitleWidth[0][7]);
	m_lbBottomRanking.SetRowAlign(0, TEXT_CENTER);
	m_lbBottomRanking.SetRowAlign(1, TEXT_CENTER);
	m_lbBottomRanking.SetRowAlign(2, TEXT_CENTER);
	m_lbBottomRanking.SetRowAlign(3, TEXT_CENTER);
	m_lbBottomRanking.SetRowAlign(4, TEXT_CENTER);
	m_lbBottomRanking.SetRowAlign(5, TEXT_CENTER);
	m_lbBottomRanking.SetRowAlign(6, TEXT_CENTER);
	m_lbBottomRanking.SetRowAlign(7, TEXT_CENTER);

	// 랭킹 리스트의 각 항목
	m_rcRankingColumn[0][0].SetRect(13, 69, m_rcListTitleWidth[0][0] + 13, 219);
	m_rcRankingColumn[0][1].SetRect(m_rcRankingColumn[0][0].Right, 69, m_rcRankingColumn[0][0].Right + m_rcListTitleWidth[0][1], 219);
	m_rcRankingColumn[0][2].SetRect(m_rcRankingColumn[0][1].Right, 69, m_rcRankingColumn[0][1].Right + m_rcListTitleWidth[0][2], 219);
	m_rcRankingColumn[0][3].SetRect(m_rcRankingColumn[0][2].Right, 69, m_rcRankingColumn[0][2].Right + m_rcListTitleWidth[0][3], 219);
	m_rcRankingColumn[0][4].SetRect(m_rcRankingColumn[0][3].Right, 69, m_rcRankingColumn[0][3].Right + m_rcListTitleWidth[0][4], 219);
	m_rcRankingColumn[0][5].SetRect(m_rcRankingColumn[0][4].Right, 69, m_rcRankingColumn[0][4].Right + m_rcListTitleWidth[0][5], 219);
	m_rcRankingColumn[0][6].SetRect(m_rcRankingColumn[0][5].Right, 69, m_rcRankingColumn[0][5].Right + m_rcListTitleWidth[0][6], 219);
	m_rcRankingColumn[0][7].SetRect(m_rcRankingColumn[0][6].Right, 69, m_rcRankingColumn[0][6].Right + m_rcListTitleWidth[0][7], 219);
	m_rcRankingColumn[1][0].SetRect(13, 229, m_rcListTitleWidth[1][0] + 13, 379);
	m_rcRankingColumn[1][1].SetRect(m_rcRankingColumn[1][0].Right, 229, m_rcRankingColumn[1][0].Right + m_rcListTitleWidth[1][1], 379);
	m_rcRankingColumn[1][2].SetRect(m_rcRankingColumn[1][1].Right, 229, m_rcRankingColumn[1][1].Right + m_rcListTitleWidth[1][2], 379);
	m_rcRankingColumn[1][3].SetRect(m_rcRankingColumn[1][2].Right, 229, m_rcRankingColumn[1][2].Right + m_rcListTitleWidth[1][3], 379);
	m_rcRankingColumn[1][4].SetRect(m_rcRankingColumn[1][3].Right, 229, m_rcRankingColumn[1][3].Right + m_rcListTitleWidth[1][4], 379);
	m_rcRankingColumn[1][5].SetRect(m_rcRankingColumn[1][4].Right, 229, m_rcRankingColumn[1][4].Right + m_rcListTitleWidth[1][5], 379);
	m_rcRankingColumn[1][6].SetRect(m_rcRankingColumn[1][5].Right, 229, m_rcRankingColumn[1][5].Right + m_rcListTitleWidth[1][6], 379);
	m_rcRankingColumn[1][7].SetRect(m_rcRankingColumn[1][6].Right, 229, m_rcRankingColumn[1][6].Right + m_rcListTitleWidth[1][7], 379);
	m_rcuvRankingColumn[0].SetUV(0, 84, 8, 235, fTexWidthBaseRanking, fTexHeightBaseRanking);
	m_rcuvRankingColumn[1].SetUV(46, 84, 53, 235, fTexWidthBaseRanking, fTexHeightBaseRanking);
	m_rcuvRankingColumn[2].SetUV(52, 84, 59, 235, fTexWidthBaseRanking, fTexHeightBaseRanking);
}


// ----------------------------------------------------------------------------
// Name : KeyMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUIRankingViewEX::KeyMessage( MSG *pMsg )
{
	// 랭킹 패킷 수신중일 때는 어떠한 작업을 허용하지 않음
	if(m_bRecvRank == TRUE)
		return WMSG_SUCCESS;

	if(pMsg->wParam == VK_RETURN)
	{
		if(m_ebSearch.IsFocused())
		{
			SearchName(m_ebSearch.GetString());
		}

		return WMSG_SUCCESS;
	}
	else if(m_ebSearch.KeyMessage( pMsg ) != WMSG_FAIL)
	{
		return WMSG_SUCCESS;
	}

	return WMSG_FAIL;
}

// ----------------------------------------------------------------------------
// Name : CharMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUIRankingViewEX::CharMessage( MSG *pMsg )
{
	// 랭킹 패킷 수신중일 때는 어떠한 작업을 허용하지 않음
	if(m_bRecvRank == TRUE)
		return WMSG_SUCCESS;

	if(m_ebSearch.CharMessage( pMsg ) != WMSG_FAIL)
	{
//		if( pMsg->wParam == '!' )

		return WMSG_SUCCESS;
	} 

	return WMSG_FAIL;
}

// ----------------------------------------------------------------------------
// Name : IMEMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT	CUIRankingViewEX::IMEMessage( MSG *pMsg )
{
	// 랭킹 패킷 수신중일 때는 어떠한 작업을 허용하지 않음
	if(m_bRecvRank == TRUE)
		return WMSG_SUCCESS;

	return m_ebSearch.IMEMessage( pMsg );
}

// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUIRankingViewEX::MouseMessage(MSG *pMsg)
{
	WMSG_RESULT	wmsgResult			= WMSG_FAIL;
	static BOOL	bTitleBarClicked	= FALSE;

	if(!IsVisible())
		return WMSG_FAIL;

	static int	nOldX, nOldY;
	int			nX	= LOWORD(pMsg->lParam);
	int			nY	= HIWORD(pMsg->lParam);
	
	// Mouse message
	switch( pMsg->message )
	{
	case WM_MOUSEMOVE:
		{
			if(IsInside(nX, nY))
			{
				CUIManager::getSingleton()->SetMouseCursorInsideUIs();
			}

			if(bTitleBarClicked && (pMsg->wParam & MK_LBUTTON))
			{
				int		ndX	= nX - nOldX;
				int		ndY	= nY - nOldY;
				nOldX = nX;	nOldY = nY;

				Move(ndX, ndY);

				return WMSG_SUCCESS;
			}
			// *** 랭킹 패킷 수신중일 때는 어떠한 작업을 허용하지 않음
			else if(m_bRecvRank == TRUE)
				return WMSG_SUCCESS;
			// Close button
			else if(m_btnClose.MouseMessage(pMsg) != WMSG_FAIL)
			{
				return WMSG_SUCCESS;
			}
			else if(m_cbRankType.MouseMessage( pMsg ) != WMSG_FAIL)
			{
			}
			// List box
			else if(m_lbTopRanking.MouseMessage( pMsg ) != WMSG_FAIL)
			{
				return WMSG_SUCCESS;
			}
			else if(m_lbBottomRanking.MouseMessage( pMsg ) != WMSG_FAIL)
			{
				return WMSG_SUCCESS;
			}
		}
		break;

	case WM_LBUTTONDOWN:
		{
			if(IsInside(nX, nY))
			{
				SetFocus(TRUE);
				CUIManager::getSingleton()->RearrangeOrder(m_iClassID, TRUE);

				nOldX		= nX;
				nOldY		= nY;

				// Close button
				if(m_btnClose.MouseMessage(pMsg) != WMSG_FAIL)
				{

				}
				// Title bar
				else if(IsInsideRect(nX, nY, m_rcTitleArea))
				{
					bTitleBarClicked	= TRUE;
				}
				// *** 랭킹 패킷 수신중일 때는 어떠한 작업을 허용하지 않음
				else if(m_bRecvRank == TRUE)
					return WMSG_SUCCESS;
				// Rank Type ComboBox
				else if(m_cbRankType.MouseMessage( pMsg ) != WMSG_FAIL)
				{
					static int	iLastCurSel		= m_cbRankType.GetCurSel();

					if(iLastCurSel != m_cbRankType.GetCurSel())
					{
						m_ebSearch.ResetString();
						m_cstrSearch			= _pNetwork->MyCharacterInfo.name;
						iLastCurSel				= m_cbRankType.GetCurSel();
						m_cstrTopRankingMsg		= "";
						m_cstrBottomRankingMsg	= "";
						m_lbTopRanking.ResetAllStrings();
						m_lbBottomRanking.ResetAllStrings();
						ChangeViewType((ERANKINGEX_SUB_TYPE)m_cbRankType.GetCurSel());
					}
				}
				// Reset button
				else if(m_btnReset.MouseMessage(pMsg) != WMSG_FAIL)
				{
					
				}
				// Search button
				else if(m_btnSearch.MouseMessage(pMsg) != WMSG_FAIL)
				{
					m_btnSearch.SetFocus(FALSE);
				}
				else if((m_ebSearch.MouseMessage(pMsg)) != WMSG_FAIL)
				{
					m_ebSearch.SetFocus(TRUE);
				}
				// List box
				else if((m_lbTopRanking.MouseMessage(pMsg)) != WMSG_FAIL)
				{

				}
				else if((m_lbBottomRanking.MouseMessage(pMsg)) != WMSG_FAIL)
				{

				}
				

				return WMSG_SUCCESS;
			}
		}
		break;

	case WM_LBUTTONUP:
		{
			CUIManager* pUIManager = CUIManager::getSingleton();

			if(pUIManager->GetHoldBtn().IsEmpty())
			{
				bTitleBarClicked		= FALSE;

				if(IsFocused() == FALSE)
					return WMSG_FAIL;

				// Close button
				if(m_btnClose.MouseMessage(pMsg) != WMSG_FAIL)
				{
					Close();

				}
				// Title bar
				else if(IsInsideRect(nX, nY, m_rcTitleArea))
				{
					bTitleBarClicked	= TRUE;
				}
				// *** 랭킹 패킷 수신중일 때는 어떠한 작업을 허용하지 않음
				else if(m_bRecvRank == TRUE)
					return WMSG_SUCCESS;
				// Rank Type ComboBox
				else if(m_cbRankType.MouseMessage( pMsg ) != WMSG_FAIL)
				{
				}
				// Reset button
				else if(m_btnReset.MouseMessage(pMsg) != WMSG_FAIL)
				{
					ToggleReset();
				}
				// Search button
				else if(m_btnSearch.MouseMessage(pMsg) != WMSG_FAIL)
				{
					SearchName(m_ebSearch.GetString());
				}
				// List box
				if((wmsgResult = m_lbTopRanking.MouseMessage(pMsg)) != WMSG_FAIL)
				{

				}
				if((wmsgResult = m_lbBottomRanking.MouseMessage(pMsg)) != WMSG_FAIL)
				{

				}

				return WMSG_SUCCESS;
			}
			else
			{
				if(IsInside(nX, nY))
				{
					pUIManager->ResetHoldBtn();

					return WMSG_SUCCESS;
				}
			}
		}
		break;

	case WM_LBUTTONDBLCLK:
		{
			if(IsInside(nX, nY))
			{
				// Rank Type ComboBox
				if(m_cbRankType.MouseMessage( pMsg ) != WMSG_FAIL)
				{
				}
				return WMSG_SUCCESS;
			}
		}
		break;

	case WM_MOUSEWHEEL:
		{
			if(IsInside(nX, nY))
			{
				// Rank Type ComboBox
				if(m_cbRankType.MouseMessage( pMsg ) != WMSG_FAIL)
				{
				}
				else if(m_lbTopRanking.MouseMessage( pMsg ) != WMSG_FAIL)
				{
					return WMSG_SUCCESS;
				}
				else if(m_lbBottomRanking.MouseMessage( pMsg ) != WMSG_FAIL)
				{
					return WMSG_SUCCESS;
				}
			}
		}
		break;

	}

	return WMSG_FAIL;
}

// ----------------------------------------------------------------------------
// Name : ResetPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIRankingViewEX::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
}

// ----------------------------------------------------------------------------
// Name : AdjustPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIRankingViewEX::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
void CUIRankingViewEX::Render(void)
{
	int		nX, nY, nWidth, nHeight;

	nX		= m_nPosX;
	nY		= m_nPosY;
	nWidth	= m_nWidth;
	nHeight	= m_nHeight;

	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	// Set web board texture
	pDrawPort->InitTextureData(m_ptexBaseInterface);
	
	// UI Backgrounds
	pDrawPort->AddTexture(nX + m_rcBackT[0].Left, nY + m_rcBackT[0].Top, nX + m_rcBackT[0].Right, nY + m_rcBackT[0].Bottom,
		m_rcuvBackTL.U0, m_rcuvBackTL.V0, m_rcuvBackTL.U1, m_rcuvBackTL.V1, 0xFFFFFFFF);
	pDrawPort->AddTexture(nX + m_rcBackT[1].Left, nY + m_rcBackT[1].Top, nX + m_rcBackT[1].Right, nY + m_rcBackT[1].Bottom,
		m_rcuvBackTM.U0, m_rcuvBackTM.V0, m_rcuvBackTM.U1, m_rcuvBackTM.V1, 0xFFFFFFFF);
	pDrawPort->AddTexture(nX + m_rcBackT[2].Left, nY + m_rcBackT[2].Top, nX + m_rcBackT[2].Right, nY + m_rcBackT[2].Bottom,
		m_rcuvBackTR.U0, m_rcuvBackTR.V0, m_rcuvBackTR.U1, m_rcuvBackTR.V1, 0xFFFFFFFF);
	pDrawPort->AddTexture(nX + m_rcBackM[0].Left, nY + m_rcBackM[0].Top, nX + m_rcBackM[0].Right, nY + m_rcBackM[0].Bottom,
		m_rcuvBackML.U0, m_rcuvBackML.V0, m_rcuvBackML.U1, m_rcuvBackML.V1, 0xFFFFFFFF);
	pDrawPort->AddTexture(nX + m_rcBackM[1].Left, nY + m_rcBackM[1].Top, nX + m_rcBackM[1].Right, nY + m_rcBackM[1].Bottom,
		m_rcuvBackMM.U0, m_rcuvBackMM.V0, m_rcuvBackMM.U1, m_rcuvBackMM.V1, 0xFFFFFFFF);
	pDrawPort->AddTexture(nX + m_rcBackM[2].Left, nY + m_rcBackM[2].Top, nX + m_rcBackM[2].Right, nY + m_rcBackM[2].Bottom,
		m_rcuvBackMR.U0, m_rcuvBackMR.V0, m_rcuvBackMR.U1, m_rcuvBackMR.V1, 0xFFFFFFFF);
	pDrawPort->AddTexture(nX + m_rcBackB[0].Left, nY + m_rcBackB[0].Top, nX + m_rcBackB[0].Right, nY + m_rcBackB[0].Bottom,
		m_rcuvBackBL.U0, m_rcuvBackBL.V0, m_rcuvBackBL.U1, m_rcuvBackBL.V1, 0xFFFFFFFF);
	pDrawPort->AddTexture(nX + m_rcBackB[1].Left, nY + m_rcBackB[1].Top, nX + m_rcBackB[1].Right, nY + m_rcBackB[1].Bottom,
		m_rcuvBackBM.U0, m_rcuvBackBM.V0, m_rcuvBackBM.U1, m_rcuvBackBM.V1, 0xFFFFFFFF);
	pDrawPort->AddTexture(nX + m_rcBackB[2].Left, nY + m_rcBackB[2].Top, nX + m_rcBackB[2].Right, nY + m_rcBackB[2].Bottom,
		m_rcuvBackBR.U0, m_rcuvBackBR.V0, m_rcuvBackBR.U1, m_rcuvBackBR.V1, 0xFFFFFFFF);

	pDrawPort->FlushRenderingQueue();
	pDrawPort->InitTextureData(m_ptexBaseMessageBox);

	m_btnClose.Render();

	pDrawPort->FlushRenderingQueue();
	pDrawPort->InitTextureData(m_ptexBaseCashshop);

	// 안쪽의 테두리뒷배경
	pDrawPort->AddTexture(nX + m_rcBackgroundInside[0].Left, nY + m_rcBackgroundInside[0].Top, nX + m_rcBackgroundInside[0].Right, nY + m_rcBackgroundInside[0].Bottom,
		m_rcuvBackgoundInside[0].U0, m_rcuvBackgoundInside[0].V0, m_rcuvBackgoundInside[0].U1, m_rcuvBackgoundInside[0].V1, 0xFFFFFFFF);
	pDrawPort->AddTexture(nX + m_rcBackgroundInside[1].Left, nY + m_rcBackgroundInside[1].Top, nX + m_rcBackgroundInside[1].Right, nY + m_rcBackgroundInside[1].Bottom,
		m_rcuvBackgoundInside[1].U0, m_rcuvBackgoundInside[1].V0, m_rcuvBackgoundInside[1].U1, m_rcuvBackgoundInside[1].V1, 0xFFFFFFFF);
	pDrawPort->AddTexture(nX + m_rcBackgroundInside[2].Left, nY + m_rcBackgroundInside[2].Top, nX + m_rcBackgroundInside[2].Right, nY + m_rcBackgroundInside[2].Bottom,
		m_rcuvBackgoundInside[2].U0, m_rcuvBackgoundInside[2].V0, m_rcuvBackgoundInside[2].U1, m_rcuvBackgoundInside[2].V1, 0xFFFFFFFF);
	pDrawPort->AddTexture(nX + m_rcBackgroundInside[3].Left, nY + m_rcBackgroundInside[3].Top, nX + m_rcBackgroundInside[3].Right, nY + m_rcBackgroundInside[3].Bottom,
		m_rcuvBackgoundInside[3].U0, m_rcuvBackgoundInside[3].V0, m_rcuvBackgoundInside[3].U1, m_rcuvBackgoundInside[3].V1, 0xFFFFFFFF);
	pDrawPort->AddTexture(nX + m_rcBackgroundInside[4].Left, nY + m_rcBackgroundInside[4].Top, nX + m_rcBackgroundInside[4].Right, nY + m_rcBackgroundInside[4].Bottom,
		m_rcuvBackgoundInside[4].U0, m_rcuvBackgoundInside[4].V0, m_rcuvBackgoundInside[4].U1, m_rcuvBackgoundInside[4].V1, 0xFFFFFFFF);
	pDrawPort->AddTexture(nX + m_rcBackgroundInside[5].Left, nY + m_rcBackgroundInside[5].Top, nX + m_rcBackgroundInside[5].Right, nY + m_rcBackgroundInside[5].Bottom,
		m_rcuvBackgoundInside[5].U0, m_rcuvBackgoundInside[5].V0, m_rcuvBackgoundInside[5].U1, m_rcuvBackgoundInside[5].V1, 0xFFFFFFFF);
	pDrawPort->AddTexture(nX + m_rcBackgroundInside[6].Left, nY + m_rcBackgroundInside[6].Top, nX + m_rcBackgroundInside[6].Right, nY + m_rcBackgroundInside[6].Bottom,
		m_rcuvBackgoundInside[6].U0, m_rcuvBackgoundInside[6].V0, m_rcuvBackgoundInside[6].U1, m_rcuvBackgoundInside[6].V1, 0xFFFFFFFF);
	pDrawPort->AddTexture(nX + m_rcBackgroundInside[7].Left, nY + m_rcBackgroundInside[7].Top, nX + m_rcBackgroundInside[7].Right, nY + m_rcBackgroundInside[7].Bottom,
		m_rcuvBackgoundInside[7].U0, m_rcuvBackgoundInside[7].V0, m_rcuvBackgoundInside[7].U1, m_rcuvBackgoundInside[7].V1, 0xFFFFFFFF);
	pDrawPort->AddTexture(nX + m_rcBackgroundInside[8].Left, nY + m_rcBackgroundInside[8].Top, nX + m_rcBackgroundInside[8].Right, nY + m_rcBackgroundInside[8].Bottom,
		m_rcuvBackgoundInside[8].U0, m_rcuvBackgoundInside[8].V0, m_rcuvBackgoundInside[8].U1, m_rcuvBackgoundInside[8].V1, 0xFFFFFFFF);
	// 검색창 뒷배경
	pDrawPort->AddTexture(nX + m_rcSearchBox[0].Left, nY + m_rcSearchBox[0].Top, nX + m_rcSearchBox[0].Right, nY + m_rcSearchBox[0].Bottom,
		m_rcuvSearchBox[0].U0, m_rcuvSearchBox[0].V0, m_rcuvSearchBox[0].U1, m_rcuvSearchBox[0].V1, 0xFFFFFFFF);
	pDrawPort->AddTexture(nX + m_rcSearchBox[1].Left, nY + m_rcSearchBox[1].Top, nX + m_rcSearchBox[1].Right, nY + m_rcSearchBox[1].Bottom,
		m_rcuvSearchBox[1].U0, m_rcuvSearchBox[1].V0, m_rcuvSearchBox[1].U1, m_rcuvSearchBox[1].V1, 0xFFFFFFFF);
	pDrawPort->AddTexture(nX + m_rcSearchBox[2].Left, nY + m_rcSearchBox[2].Top, nX + m_rcSearchBox[2].Right, nY + m_rcSearchBox[2].Bottom,
		m_rcuvSearchBox[2].U0, m_rcuvSearchBox[2].V0, m_rcuvSearchBox[2].U1, m_rcuvSearchBox[2].V1, 0xFFFFFFFF);

	pDrawPort->FlushRenderingQueue();
	pDrawPort->InitTextureData(m_ptexBaseInterface);

	// Controls
	m_btnReset.Render();
	m_btnSearch.Render();
	m_ebSearch.Render();

	// Title	
	pDrawPort->PutTextExCX(m_cstrTitle, nX + (GetWidth() / 2), nY + (m_rcTitleArea.Top + 12));

	pDrawPort->FlushRenderingQueue();
	pDrawPort->EndTextEx();
	pDrawPort->InitTextureData(m_ptexBaseRanking);

	// 랭킹 리스트 항목 그리기
	{
		for(int i = 0; i < 2; i++)
		{
			for(int j = 0; j < 8; j++)
			{
				pDrawPort->AddTexture(nX + m_rcRankingColumn[i][j].Left, nY + m_rcRankingColumn[i][j].Top, nX + m_rcRankingColumn[i][j].Left + 7, nY + m_rcRankingColumn[i][j].Bottom,
					m_rcuvRankingColumn[0].U0, m_rcuvRankingColumn[0].V0, m_rcuvRankingColumn[0].U1, m_rcuvRankingColumn[0].V1, 0xFFFFFFFF);
				pDrawPort->AddTexture(nX + m_rcRankingColumn[i][j].Left + 7, nY + m_rcRankingColumn[i][j].Top, nX + m_rcRankingColumn[i][j].Right - 7, nY + m_rcRankingColumn[i][j].Bottom,
					m_rcuvRankingColumn[1].U0, m_rcuvRankingColumn[1].V0, m_rcuvRankingColumn[1].U1, m_rcuvRankingColumn[1].V1, 0xFFFFFFFF);
				pDrawPort->AddTexture(nX + m_rcRankingColumn[i][j].Right - 7, nY + m_rcRankingColumn[i][j].Top, nX + m_rcRankingColumn[i][j].Right, nY + m_rcRankingColumn[i][j].Bottom,
					m_rcuvRankingColumn[2].U0, m_rcuvRankingColumn[2].V0, m_rcuvRankingColumn[2].U1, m_rcuvRankingColumn[2].V1, 0xFFFFFFFF);
			}
		}
	}

	// 링킹 리스트 타이틀 항목
	{
		for(int i = 0; i < 2; i++)
		{
			pDrawPort->PutTextExCX(_S(1715, "아이디"), nX + m_rcRankingColumn[i][0].Left + ((m_rcRankingColumn[i][0].Right - m_rcRankingColumn[i][0].Left) / 2), nY + m_rcRankingColumn[i][0].Top + 2);
			pDrawPort->PutTextExCX(_S(5563, "전주랭킹"), nX + m_rcRankingColumn[i][1].Left + ((m_rcRankingColumn[i][1].Right - m_rcRankingColumn[i][1].Left) / 2), nY + m_rcRankingColumn[i][1].Top + 2);
			pDrawPort->PutTextExCX(_S(5564, "금주랭킹"), nX + m_rcRankingColumn[i][2].Left + ((m_rcRankingColumn[i][2].Right - m_rcRankingColumn[i][2].Left) / 2), nY + m_rcRankingColumn[i][2].Top + 2);
			pDrawPort->PutTextExCX(_S(5565, "전주달성"), nX + m_rcRankingColumn[i][3].Left + ((m_rcRankingColumn[i][3].Right - m_rcRankingColumn[i][3].Left) / 2), nY + m_rcRankingColumn[i][3].Top + 2);
			pDrawPort->PutTextExCX(_S(5566, "금주달성"), nX + m_rcRankingColumn[i][4].Left + ((m_rcRankingColumn[i][4].Right - m_rcRankingColumn[i][4].Left) / 2), nY + m_rcRankingColumn[i][4].Top + 2);
			pDrawPort->PutTextExCX(_S(73, "레벨"), nX + m_rcRankingColumn[i][5].Left + ((m_rcRankingColumn[i][5].Right - m_rcRankingColumn[i][5].Left) / 2), nY + m_rcRankingColumn[i][5].Top + 2);
			pDrawPort->PutTextExCX(_S(141, "직업"), nX + m_rcRankingColumn[i][6].Left + ((m_rcRankingColumn[i][6].Right - m_rcRankingColumn[i][6].Left) / 2), nY + m_rcRankingColumn[i][6].Top + 2);
			pDrawPort->PutTextExCX(_S(5567, "달성날짜"), nX + m_rcRankingColumn[i][7].Left + ((m_rcRankingColumn[i][7].Right - m_rcRankingColumn[i][7].Left) / 2), nY + m_rcRankingColumn[i][7].Top + 2);
		}
	}

	pDrawPort->FlushRenderingQueue();
	pDrawPort->InitTextureData(m_ptexBaseInterface);

	m_lbTopRanking.Render();
	m_lbBottomRanking.Render();

	pDrawPort->FlushRenderingQueue();
	pDrawPort->EndTextEx();
	pDrawPort->InitTextureData(m_ptexBaseRanking);

	// Top /Bottom rankinglist message
	if(m_cstrTopRankingMsg.Length() > 0)
		pDrawPort->PutTextExCX(m_cstrTopRankingMsg, nX + (GetWidth() / 2), nY + (m_lbTopRanking.GetPosY() + ((m_lbTopRanking.GetHeight() / 2) - (_pUIFontTexMgr->GetFontHeight() / 2))));
	if(m_cstrBottomRankingMsg.Length() > 0)
		pDrawPort->PutTextExCX(m_cstrBottomRankingMsg, nX + (GetWidth() / 2), nY + (m_lbBottomRanking.GetPosY() + ((m_lbBottomRanking.GetHeight() / 2) - (_pUIFontTexMgr->GetFontHeight() / 2))));

	// RankType Combobox list
	pDrawPort->AddTexture(nX + m_rcRankTypeBox.Left, nY + m_rcRankTypeBox.Top, nX + m_rcRankTypeBox.Right, nY + m_rcRankTypeBox.Bottom,
		m_rcuvRankTypeBox.U0, m_rcuvRankTypeBox.V0, m_rcuvRankTypeBox.U1, m_rcuvRankTypeBox.V1, 0xFFFFFFFF);
	m_cbRankType.Render();

	// ProgressBarBox
	if(m_bRecvRank == TRUE)
	{
		pDrawPort->AddTexture(nX + m_rcProgressBox.Left, nY + m_rcProgressBox.Top, nX + m_rcProgressBox.Right, nY + m_rcProgressBox.Bottom,
			m_rcuvProgressBox.U0, m_rcuvProgressBox.V0, m_rcuvProgressBox.U1, m_rcuvProgressBox.V1, 0xFFFFFFFF);
	}

	pDrawPort->FlushRenderingQueue();
	pDrawPort->EndTextEx();

	if(m_ebSearch.DoesShowReadingWindow())
	{
		pDrawPort->InitTextureData(m_ptexBaseInterface);

		m_ebSearch.RenderReadingWindow();

		pDrawPort->FlushRenderingQueue();
		pDrawPort->EndTextEx();
	}

	if(m_bRecvRank == TRUE)
	{
		m_prbProgress.Render();
	}
}

// ----------------------------------------------------------------------------
// Name : ToggleVisible()
// Desc :
// ----------------------------------------------------------------------------
void CUIRankingViewEX::ToggleVisible()
{
	if(IsVisible() == FALSE)
	{
		Open(ERANKINGEX_SUB_TYPE_LEVELMASTER);
	}
	else
	{
		Close();
	}
}

// ----------------------------------------------------------------------------
// Name : ToggleReset()
// Desc :
// ----------------------------------------------------------------------------
void CUIRankingViewEX::ToggleReset()
{
	m_ebSearch.ResetString();
	m_cstrSearch			= _pNetwork->MyCharacterInfo.name;
	m_cstrTopRankingMsg		= "";
	m_cstrBottomRankingMsg	= "";
	m_lbTopRanking.ResetAllStrings();
	m_lbBottomRanking.ResetAllStrings();

	ChangeViewType(m_eSubType);
}

// ----------------------------------------------------------------------------
// Name : Open()
// Desc :
// ----------------------------------------------------------------------------
void CUIRankingViewEX::Open()
{
	CUIManager::getSingleton()->RearrangeOrder(m_iClassID, TRUE);

	ChangeViewType(m_eSubType);
}

void CUIRankingViewEX::Open(ERANKINGEX_SUB_TYPE eSubType)
{
	m_eSubType		= eSubType;

	Open();
}

// ----------------------------------------------------------------------------
// Name : Close()
// Desc :
// ----------------------------------------------------------------------------
void CUIRankingViewEX::Close()
{
	m_ebSearch.ResetString();
	m_ebSearch.SetFocus(FALSE);
	m_cstrSearch			= _pNetwork->MyCharacterInfo.name;
	m_cstrTopRankingMsg		= "";
	m_cstrBottomRankingMsg	= "";
	m_lbTopRanking.ResetAllStrings();
	m_lbBottomRanking.ResetAllStrings();
	m_bRecvRank				= FALSE;

	CUIManager::getSingleton()->RearrangeOrder( m_iClassID , FALSE );
}

// ----------------------------------------------------------------------------
// Name : ChangeViewType()
// Desc :
// ----------------------------------------------------------------------------
void CUIRankingViewEX::ChangeViewType(ERANKINGEX_SUB_TYPE eSubType)
{
	m_eSubType	= eSubType;
	if(m_eSubType >= _ERANKINGEX_SUB_TYPE_END)
		m_eSubType	= (ERANKINGEX_SUB_TYPE)(_ERANKINGEX_SUB_TYPE_END - 1);

	m_cbRankType.SetCurSel((int)m_eSubType);

	SetTitle(GetRankStringByType(eSubType));

	// 기본 검색대상은 본인임
	m_cstrSearch	= _pNetwork->MyCharacterInfo.name;

	// 선택된 타입의 랭킹 정보가 들어있는지 체크하고
	// 없다면 랭킹 요청 패킷 전송.
	CheckRecvRankingListAndReq();
}

void CUIRankingViewEX::SearchName(CTString cstrName)
{
	if(cstrName.Length() > 0)
	{
		m_cstrSearch			= cstrName;
		m_cstrBottomRankingMsg	= "";
		m_lbBottomRanking.ResetAllStrings();
		m_ebSearch.SetEnable(FALSE);
		CheckRecvRankingListAndReq();
	}
}

// ----------------------------------------------------------------------------
// Name : UpdateTopRanking()
// Desc :
// ----------------------------------------------------------------------------
void CUIRankingViewEX::UpdateTopRanking()
{
	m_lbTopRanking.ResetAllStrings();

	if(m_eSubType == ERANKINGEX_SUB_TYPE_GUILD)
	{	// 길드는 아직 적용하지 않음
		
	}
	else
	{
		int		nTotal	= m_oRankingList.GetCountRankPoint(m_eSubType);
	
		if(nTotal <= 0)
			return;

		for(int i = 0; i < nTotal; i++)
		{
			SRankingPointData	oRankData;

			if(m_oRankingList.GetRankPoint(&oRankData, i, m_eSubType) == FALSE)
				return;
			
			char			szBuff[64]	= { 0, };
			CTString		cstrTemp;
			COLOR			colorText	= 0xF2F2F2FF;

			if(oRankData.cstrCharacterName == _pNetwork->MyCharacterInfo.name)		// 자신의 아이디와 같다면 하이라이트
				colorText	= 0xF22020FF;

			// 1 : 이름
			AddTopRankingString(i, 0, oRankData.cstrCharacterName, colorText);
			// 2 : 전주 랭킹
			_itoa(oRankData.nLastRank, szBuff, 10);
			cstrTemp	= szBuff;
			AddTopRankingString(i, 1, cstrTemp, colorText);
			// 3 : 금주 랭킹
			_itoa(oRankData.nThisRank, szBuff, 10);
			cstrTemp	= szBuff;
			AddTopRankingString(i, 2, cstrTemp, colorText);
			// 4 : 전주 달성
			_itoa(oRankData.nLastPoint, szBuff, 10);
			cstrTemp	= szBuff;
			AddTopRankingString(i, 3, cstrTemp, colorText);
			// 5 : 금주 달성
			_itoa(oRankData.nThisPoint, szBuff, 10);
			cstrTemp	= szBuff;
			AddTopRankingString(i, 4, cstrTemp, colorText);
			// 6 : 레벨
			_itoa(oRankData.nLevel, szBuff, 10);
			cstrTemp	= szBuff;
			AddTopRankingString(i, 5, cstrTemp, colorText);
			// 7 : 직업
			AddTopRankingString(i, 6, GetJobStringByType(oRankData.ubClass), colorText);
			// 8 : 달성날짜
			AddTopRankingString(i, 7, oRankData.cstrFixedDate, colorText);
		}
	}	
}

// ----------------------------------------------------------------------------
// Name : UpdateBottomRanking()
// Desc :
// ----------------------------------------------------------------------------
void CUIRankingViewEX::UpdateBottomRanking()
{
	static ERANKINGEX_SUB_TYPE	msgLastSubType	= _ERANKINGEX_SUB_TYPE_END;

	m_lbBottomRanking.ResetAllStrings();
	msgLastSubType	= m_eSubType;

	if(m_eSubType == ERANKINGEX_SUB_TYPE_GUILD)
	{

	}
	else
	{
		int		nTotal	= m_oRankingList.GetCountRankPointSearch(m_cstrSearch, m_eSubType);
	
		if(nTotal <= 0)
			return;

		for(int i = 0; i < nTotal; i++)
		{
			SRankingPointData	oRankData;

			if(m_oRankingList.GetRankPointSearch(&oRankData, m_cstrSearch, i, m_eSubType) == FALSE)
				return;
			
			char			szBuff[64]	= { 0, };
			CTString		cstrTemp;
			COLOR			colorText	= 0xF2F2F2FF;

			if(oRankData.cstrCharacterName == _pNetwork->MyCharacterInfo.name)		// 자신의 아이디와 같다면 하이라이트
				colorText	= 0xF22020FF;
			else if(oRankData.cstrCharacterName == m_cstrSearch)					// 검색 아이디와 같다면 하이라이트
				colorText	= 0x20F220FF;

			// 1 : 이름
			AddBottomRankingString(i, 0, oRankData.cstrCharacterName, colorText);
			// 2 : 전주 랭킹
			_itoa(oRankData.nLastRank, szBuff, 10);
			cstrTemp	= szBuff;
			AddBottomRankingString(i, 1, cstrTemp, colorText);
			// 3 : 금주 랭킹
			_itoa(oRankData.nThisRank, szBuff, 10);
			cstrTemp	= szBuff;
			AddBottomRankingString(i, 2, cstrTemp, colorText);
			// 4 : 전주 달성
			_itoa(oRankData.nLastPoint, szBuff, 10);
			cstrTemp	= szBuff;
			AddBottomRankingString(i, 3, cstrTemp, colorText);
			// 5 : 금주 달성
			_itoa(oRankData.nThisPoint, szBuff, 10);
			cstrTemp	= szBuff;
			AddBottomRankingString(i, 4, cstrTemp, colorText);
			// 6 : 레벨
			_itoa(oRankData.nLevel, szBuff, 10);
			cstrTemp	= szBuff;
			AddBottomRankingString(i, 5, cstrTemp, colorText);
			// 7 : 직업
			AddBottomRankingString(i, 6, GetJobStringByType(oRankData.ubClass), colorText);
			// 8 : 달성날짜
			AddBottomRankingString(i, 7, oRankData.cstrFixedDate, colorText);
		}
	}
}

void CUIRankingViewEX::RecvRankingList(CNetworkMessage& nmMessage)
{
	ERANKINGEX_SUB_TYPE	eSubType	= ERANKINGEX_SUB_TYPE_LEVELMASTER;
	SLONG	iListType				= 0;
	UCHAR	ucType					= 0;
	UCHAR	ucClass					= 0;
	SLONG	nTotalCount				= 0;
	SLONG	nRecvListCount			= 0;

	nmMessage >> (SLONG)iListType;		// (INT)MSG_EX_RANKING_SYSTEM_EX_TYPE
	switch(iListType)
	{
	case MSG_EX_RANKING_LIST:		// (s->c) (uc)type, (uc)subtype, (int)total_count, (int)list_count, (str)name, (int)ranking, (int)ranking_pre, (int)exp, (int)exp_pre, (int)level, (uc)job, (str)date.....
		{
			nmMessage >> (UCHAR)ucType;			// (uc)type
			nmMessage >> (UCHAR)ucClass;		// (uc)subtype
			nmMessage >> (SLONG)nTotalCount;	// (int)total_count
			nmMessage >> (SLONG)nRecvListCount;	// (int)list_count
		}
		break;

	case MSG_EX_RANKING_SEARCH:		// (s->c) (uc)type, (uc)subtype, (int)list_count, (str)name, (int)ranking, (int)ranking_pre, (int)exp, (int)exp_pre, (int)level, (uc)job, (str)date.....
		{
			nmMessage >> (UCHAR)ucType;			// (uc)type
			nmMessage >> (UCHAR)ucClass;		// (uc)subtype
			nmMessage >> (SLONG)nRecvListCount;	// (int)list_count

			nTotalCount		= nRecvListCount;
		}
		break;

	case MSG_EX_RANKING_REFRESH:	// (s->c)
		{
			ClearRankingList();
			return;
		}
		break;

	case MSG_EX_RANKING_ERROR:		// (s->c) (uc)type
		{
			nmMessage >> (UCHAR)ucType;			// (uc)type

			switch(ucType)
			{
			case MSG_EX_RANKING_SYSTEM_ERROR:
				m_cstrTopRankingMsg		= _S(5581, "랭킹 서버 오류입니다.");

				m_lbTopRanking.ResetAllStrings();
				m_lbBottomRanking.ResetAllStrings();
				break;

			case MSG_EX_RANKING_SYSTEM_ERROR_NOT_EXIST:
				m_cstrBottomRankingMsg	= _S(5574, "해당 캐릭터에 대한 랭킹 정보가 없습니다.");
				break;

			case MSG_EX_RANKING_SYSTEM_ERROR_REFRESH_TIME:
				m_cstrTopRankingMsg		= _S(5575, "랭킹이 갱신된 이후에 다시 확인 해주십시오.");

				m_lbTopRanking.ResetAllStrings();
				m_lbBottomRanking.ResetAllStrings();
				break;
			}

			m_ebSearch.SetEnable(TRUE);
			m_bRecvRank		= FALSE;

			return;
		}
		break;
	}

	if(nTotalCount <= 0 || nRecvListCount <= 0)
		return;

	// 요청받은 리스트의 서브 카테고리 (레벨마스터, 각 클래스)
	if(ucType == 0)
	{	// 레벨마스터
		eSubType	= ERANKINGEX_SUB_TYPE_LEVELMASTER;
	}
	else if(ucType == 1)
	{	// 각 클래스 랭크
		eSubType	= (ERANKINGEX_SUB_TYPE)(ucClass + 1);
	}

	for(int i = 0; i < nRecvListCount; i++)
	{
		CTString	cstrName;
		SLONG		nRanking;
		SLONG		nRankingLast;
		SLONG		nPoint;
		SLONG		nPointLast;
		SLONG		nLevel;
		UCHAR		ucJob;
		CTString	cstrDate;

		nmMessage >> cstrName;			// (str)name
		nmMessage >> (SLONG)nRanking;		// (int)ranking
		nmMessage >> (SLONG)nRankingLast;	// (int)ranking_pre
		nmMessage >> (SLONG)nPoint;		// (int)exp
		nmMessage >> (SLONG)nPointLast;	// (int)exp_pre
		nmMessage >> (SLONG)nLevel;		// (int)level
		nmMessage >> (UCHAR)ucJob;		// (uc)job
		nmMessage >> cstrDate;			// (str)date


		SRankingPointData	oData;

		oData.cstrCharacterName		= cstrName;
		oData.nThisRank				= nRanking;
		oData.nLastRank				= nRankingLast;
		oData.nThisPoint			= nPoint;
		oData.nLastPoint			= nPointLast;
		oData.nLevel				= nLevel;
		oData.ubClass				= ucJob;
		// 날짜 포멧 변환
		if(cstrDate.Length() > 0 && cstrDate.str_String[0] != NULL)
		{
			char	szBuff[32]		= { 0, };
			int		iYear, iMonth, iDay, iHour, iMin, iSec;
			sscanf(cstrDate, "%d-%d-%d %d:%d:%d", &iYear, &iMonth, &iDay, &iHour, &iMin, &iSec);
			sprintf(szBuff, "%02d-%02d-%04d", iMonth, iDay, iYear);

			oData.cstrFixedDate		= szBuff;
		}
		
		if(iListType == MSG_EX_RANKING_LIST)
		{
			m_oRankingList.AddRankingPoint(eSubType, oData);
		}
		else if(iListType == MSG_EX_RANKING_SEARCH)
		{
			m_oRankingList.AddRankingPointSearch(eSubType, oData, m_cstrSearch);
		}
	}

	m_nRecvRankCount		+= nRecvListCount;
	m_nRecvRankTotalCount	= nTotalCount;
	m_prbProgress.SetProgressRange(0, m_nRecvRankTotalCount);
	m_prbProgress.OffsetProgressPos(m_nRecvRankCount);

	if(m_nRecvRankCount >= m_nRecvRankTotalCount)
	{
		CheckRecvRankingListAndReq();
	}
}

BOOL CUIRankingViewEX::CheckRecvRankingListAndReq()
{
	if(m_oRankingList.GetCountRankPoint(m_eSubType) <= 0)
	{
		m_bRecvRank				= TRUE;
		m_nRecvRankCount		= 0;
		m_nRecvRankTotalCount	= 0;
		m_prbProgress.SetProgressRange(0, 100);
		m_prbProgress.OffsetProgressPos(100);
		_pNetwork->SendRankingListEx(GetRankingNetworkType(), GetRankingNetworkSubType());

		return FALSE;
	}
	else if(m_oRankingList.GetCountRankPoint(m_eSubType) > 0)
	{
		UpdateTopRanking();
	}

	if(m_cstrSearch.Length() > 0 && m_oRankingList.GetCountRankPointSearch(m_cstrSearch, m_eSubType) <= 0)
	{
		m_bRecvRank				= TRUE;
		m_nRecvRankCount		= 0;
		m_nRecvRankTotalCount	= 0;
		m_prbProgress.SetProgressRange(0, 100);
		m_prbProgress.OffsetProgressPos(100);
		_pNetwork->SendRankingSearchListEx(GetRankingNetworkType(), GetRankingNetworkSubType(), m_cstrSearch);

		return FALSE;
	}
	else if(m_oRankingList.GetCountRankPointSearch(m_cstrSearch, m_eSubType) > 0)
	{
		UpdateBottomRanking();
		m_ebSearch.SetEnable(TRUE);
	}

	m_bRecvRank			= FALSE;

	return TRUE;
}

void	CUIRankingViewEX::ClearRankingList()
{
	m_oRankingList.ClearRankingPoint();
	m_oRankingList.ClearRankingPointSearch();
	m_oRankingList.ClearRankingGuild();
	m_oRankingList.ClearRankingGuildSearch();
}


void CUIRankingViewEX::AddTopRankingString(int iCol, int iRow, CTString cstrString, COLOR cColor)
{
	m_lbTopRanking.AddString(iCol, iRow, cstrString, cColor);
}

void CUIRankingViewEX::AddBottomRankingString(int iCol, int iRow, CTString cstrString, COLOR cColor)
{
	m_lbBottomRanking.AddString(iCol, iRow, cstrString, cColor);
}