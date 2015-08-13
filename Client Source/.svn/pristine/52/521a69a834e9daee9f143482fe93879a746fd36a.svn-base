
#include "StdH.h"
#include <Engine/Interface/UIInternalClasses.h>

#include "BackImageManager.h"

#define DEF_TEX_WIDTH	(1920.0f)
#define DEF_TEX_HEIGHT	(1200.0f)

#define DEF_RATIO_4_3	(0.75f)
#define DEF_RATIO_16_9	(0.5625f)
#define DEF_RATIO_16_10	(0.625f)
#define DEF_RATIO_5_4	(0.8f)

#define DEF_CAMERA_DISTANCE_4_3 (6.66667f)
#define DEF_CAMERA_DISTANCE_5_4 (6.25f)
#define DEF_CAMERA_DISTANCE		(8.f)

CBackImageManager::CBackImageManager()
	: m_pRenderTex(NULL)
	, m_fTexU0(0.0f)
	, m_fTexV0(0.0f)
	, m_fTexU1(0.0f)
	, m_fTexV1(0.0f)
	, m_nWidth(0)
	, m_nHeight(0)
	, m_nCurrentType(eTEX_NONE)
{
	m_stResolution[eSIZE_800_600].nWidth	= 800; 
	m_stResolution[eSIZE_800_600].nHeight	= 600;

	m_stResolution[eSIZE_1024_768].nWidth	= 1024;
	m_stResolution[eSIZE_1024_768].nHeight	= 768;
	
	m_stResolution[eSIZE_1152_864].nWidth	= 1152;
	m_stResolution[eSIZE_1152_864].nHeight	= 864; 
	
	m_stResolution[eSIZE_1280_720].nWidth	= 1280; 
	m_stResolution[eSIZE_1280_720].nHeight	= 720; 
	
	m_stResolution[eSIZE_1280_800].nWidth	= 1280; 
	m_stResolution[eSIZE_1280_800].nHeight	= 800; 
	
	m_stResolution[eSIZE_1280_960].nWidth	= 1280; 
	m_stResolution[eSIZE_1280_960].nHeight	= 960; 
	
	m_stResolution[eSIZE_1280_1024].nWidth  = 1280; 
	m_stResolution[eSIZE_1280_1024].nHeight = 1024; 
	
	m_stResolution[eSIZE_1440_900].nWidth	= 1440; 
	m_stResolution[eSIZE_1440_900].nHeight	= 900;  
	
	m_stResolution[eSIZE_1600_900].nWidth	= 1600; 
	m_stResolution[eSIZE_1600_900].nHeight	= 900; 
	
	m_stResolution[eSIZE_1600_1200].nWidth  = 1600; 
	m_stResolution[eSIZE_1600_1200].nHeight = 1200; 
	
	m_stResolution[eSIZE_1680_1050].nWidth  = 1680; 
	m_stResolution[eSIZE_1680_1050].nHeight = 1050; 
	
	m_stResolution[eSIZE_1920_1200].nWidth  = 1920; 
	m_stResolution[eSIZE_1920_1200].nHeight = 1200; 

	m_vecTexData.clear();
}

CBackImageManager::~CBackImageManager()
{
	ResetData();
}

void CBackImageManager::InitData()
{
	CTString strPath = "Data\\Interface\\";
	CTString strTmp;

	m_vecTexData.push_back(_pTextureStock->Obtain_t(strPath + "login_bg.tex"));
}

void CBackImageManager::ResetData()
{
	vecTexDataIter bIt = m_vecTexData.begin();
	vecTexDataIter eIt = m_vecTexData.end();

	for (; bIt != eIt; ++bIt)
	{
		if ((*bIt) == NULL)
			continue;

		STOCK_RELEASE((*bIt));
	}

	m_vecTexData.clear();

	m_pRenderTex = NULL;

	m_fTexU0 = 0.0f;
	m_fTexV0 = 0.0f;
	m_fTexU1 = 0.0f;
	m_fTexV1 = 0.0f;

	m_nWidth = 0;
	m_nHeight = 0;

	m_nCurrentType = eTEX_NONE;
}

void CBackImageManager::Render( CDrawPort* pdp )
{
	if (m_nCurrentType == eTEX_NONE)
		return;

	if (m_pRenderTex == NULL)
		return;

	pdp->InitTextureData(m_pRenderTex);

	pdp->AddTexture(0, 0, m_nWidth, m_nHeight, m_fTexU0, m_fTexV0, m_fTexU1, m_fTexV1, DEF_UI_COLOR_WHITE);

	pdp->FlushRenderingQueue();
}

CTextureData* CBackImageManager::TexDataAt( int idx )
{
	if (idx < 0 || idx >= m_vecTexData.size())
		return NULL;
	
	return m_vecTexData[idx];
}

void CBackImageManager::SetRenderTex( eTEX_TYPE eType )
{
	m_nCurrentType = eType;

	m_pRenderTex = TexDataAt(eType);

	if (m_pRenderTex == NULL)
		return;

	FLOAT fTexW = m_pRenderTex->GetWidth();
	FLOAT fTexH = m_pRenderTex->GetHeight();

	m_nWidth  = CUIManager::getSingleton()->GetWidth();
	m_nHeight = CUIManager::getSingleton()->GetHeight();

	eRATIO eRatio = GetRatio(m_nWidth, m_nHeight);

	int nRatioWidth = m_nWidth;
	int nRatioHeight = m_nHeight;

	m_fTexU0 = 0.0f;
	m_fTexV0 = 0.0f;
	m_fTexU1 = DEF_TEX_WIDTH / fTexW;
	m_fTexV1 = DEF_TEX_HEIGHT / fTexH;

	if (eRatio == eRATIO_4_3)
	{
		nRatioWidth = (DEF_TEX_WIDTH - (DEF_TEX_HEIGHT * 4.f / 3.f)) * 0.5f;
		m_fTexU0 = nRatioWidth / fTexW;
		m_fTexU1 = ((DEF_TEX_WIDTH - (FLOAT)nRatioWidth)) / fTexW;
	}
	else if (eRatio == eRATIO_5_4)
	{
		nRatioWidth = (DEF_TEX_WIDTH - (DEF_TEX_HEIGHT * 5.f / 4.f)) * 0.5f;
		m_fTexU0 = nRatioWidth / fTexW;
		m_fTexU1 = ((DEF_TEX_WIDTH - (FLOAT)nRatioWidth)) / fTexW;
	}
	else if (eRatio == eRATIO_16_9)
	{
		nRatioHeight = (DEF_TEX_HEIGHT - (DEF_TEX_WIDTH * 9.f / 16.f)) * 0.5f;
		m_fTexV0 = (FLOAT)nRatioHeight / fTexH;
		m_fTexV1 = ((DEF_TEX_HEIGHT - (FLOAT)nRatioHeight)) / fTexH;
	}
}

void CBackImageManager::GetArrangeOffset( int &x, int &y, int nResolusionIdx /*= -1*/ )
{
	InIRead();	// 테스트용으로 일단 여기에..

	if (nResolusionIdx != -1 && nResolusionIdx < eSIZE_MAX)
	{
		x = m_stResolution[nResolusionIdx].nOffsetX;
		y = m_stResolution[nResolusionIdx].nOffsetY;

		return;
	}

	int nIdx = GetResolutionIndex() ;
	if (nIdx != -1)
	{
		x = m_stResolution[nIdx].nOffsetX;
		y = m_stResolution[nIdx].nOffsetY;
	}
}

int CBackImageManager::GetResolutionIndex()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	int nClientWidth = pUIManager->GetWidth();
	int nClientHeight = pUIManager->GetHeight();

	int i;

	for (i = 0; i < eSIZE_MAX; ++i)
	{
		if (m_stResolution[i].nWidth == nClientWidth &&
			m_stResolution[i].nHeight == nClientHeight )
		{
			return i;
		}
	}

	return -1;
}

void CBackImageManager::SetArrangeOffset( int x, int y, int nResolusionIdx )
{
	m_stResolution[nResolusionIdx].nOffsetX = x;
	m_stResolution[nResolusionIdx].nOffsetY = y;
}

CBackImageManager::eRATIO CBackImageManager::GetRatio( int nWidth, int nHeight )
{
	float fRatio = (float)nHeight / (float)nWidth;

	if (DEF_RATIO_16_10 < fRatio && DEF_RATIO_5_4 > fRatio)
		return eRATIO_4_3;
	else if (DEF_RATIO_16_10 > fRatio)
		return eRATIO_16_9;
	else if (DEF_RATIO_5_4 <= fRatio)
		return eRATIO_5_4;

	return eRATIO_16_10;
}

float CBackImageManager::GetRatioCamera( float fFov, int nWidth, int nHeight )
{
	eRATIO eRatio = GetRatio(nWidth, nHeight);

	float fTan = fFov * 0.5f;	// eRATIO_16_10

	int nResolutionIdx = GetResolutionIndex();

	float fDis = DEF_CAMERA_DISTANCE;
		
	if (eRatio == eRATIO_4_3)
		fDis = DEF_CAMERA_DISTANCE_4_3;
	if (eRatio == eRATIO_5_4)
		fDis = DEF_CAMERA_DISTANCE_5_4;

	float fl = 8.f + (fDis / Tan(fTan));

	return fl;
}

void CBackImageManager::InIRead()
{
	// ini파일은 로컬에 전달되는 파일이 아니므로 개발버전에서만 읽고,
	// 로컬에서는 디폴트 값으로 세팅되어짐.

	std::string strFullPath = _fnmApplicationPath.FileDir();
	strFullPath += DEF_INI_PATH;

	int i;
	CTString strTmp;

	char szBuffer[16];

	const int def_value[eSIZE_MAX][2] = 
	{
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
	};

	const char* def_value2[eSIZE_MAX][eSET_MAX] = 
	{
		{ "18", "18", "18", "20", "3.4", "4" },
		{ "18", "19", "18", "20", "3.4", "4" },
		{ "17", "19", "17", "20", "3.4", "4" },
		{ "17", "21", "17", "22", "3.4", "4" },
		{ "19", "22", "19", "22", "3.4", "4" },
		{ "17", "19", "17", "20", "3.4", "4" },
		{ "17", "18", "17", "19", "3.4", "4" },
		{ "19", "21", "19", "22", "3.4", "4" },
		{ "19", "21", "19", "22", "3.4", "4" },
		{ "17", "19", "17", "19", "3.4", "4" },
		{ "20", "21", "20", "22", "3.4", "4" },
		{ "20", "21", "20", "22", "3.4", "4" },
	};

	for (i = 0; i < eSIZE_MAX; ++i)
	{
		strTmp.PrintF("GAP_%d_%d", m_stResolution[i].nWidth, m_stResolution[i].nHeight);	// 읽고자 하는 헤더 네임.

		m_stResolution[i].nOffsetX = GetPrivateProfileInt( strTmp.str_String, "x", def_value[i][0], 
			strFullPath.c_str() );

		m_stResolution[i].nOffsetY = GetPrivateProfileInt( strTmp.str_String, "y", def_value[i][1], 
			strFullPath.c_str() );

		 GetPrivateProfileString( strTmp.str_String, "create", def_value2[i][eSET_CREATE_DIS], szBuffer, 16, strFullPath.c_str() );	// 생성창에서의 캐릭터 거리.

		 m_stResolution[i].fDisCreate = atof(szBuffer);

		 GetPrivateProfileString( strTmp.str_String, "select", def_value2[i][eSET_SELECT_DIS], szBuffer, 16, strFullPath.c_str() );	// 캐릭터 선택창에서의 캐릭터 거리.

		 m_stResolution[i].fDisSelect = atof(szBuffer);

		 GetPrivateProfileString( strTmp.str_String, "mindis", def_value2[i][eSET_CREATE_ZOOM_DIS_MIN], szBuffer, 16, strFullPath.c_str() );	

		 m_stResolution[i].fDisZoom = atof(szBuffer);

		 GetPrivateProfileString( strTmp.str_String, "maxdis", def_value2[i][eSET_CREATE_ZOOM_DIS_MAX], szBuffer, 16, strFullPath.c_str() );	

		 m_stResolution[i].fDisZoomMax = atof(szBuffer);

		 GetPrivateProfileString( strTmp.str_String, "minheight", def_value2[i][eSET_CREATE_ZOOM_HEIGHT_MIN], szBuffer, 16, strFullPath.c_str() );	

		 m_stResolution[i].fHeightZoom = atof(szBuffer);

		 GetPrivateProfileString( strTmp.str_String, "maxheight", def_value2[i][eSET_CREATE_ZOOM_HEIGHT_MAX], szBuffer, 16, strFullPath.c_str() );	

		 m_stResolution[i].fHeightZoomMax = atof(szBuffer);
	}
}

float CBackImageManager::GetCharDistance( bool bSelectUI )
{
	InIRead(); 

	int nRevIdx = GetResolutionIndex();

	if (nRevIdx < 0)
		return 0.f;

	if (bSelectUI == true)
		return m_stResolution[nRevIdx].fDisSelect;
	
	return m_stResolution[nRevIdx].fDisCreate;
}

float CBackImageManager::GetZoomDisValue( bool bMax )
{
	int nRevIdx = GetResolutionIndex();
	float fValue = 0.0f;

	if (nRevIdx < 0)
		return 0.f;

	if (bMax == true)
		fValue = m_stResolution[nRevIdx].fDisZoomMax;
	else
		fValue = m_stResolution[nRevIdx].fDisZoom;

	return fValue;
}

float CBackImageManager::GetZoomHeightValue( bool bMax )
{
	int nRevIdx = GetResolutionIndex();
	float fValue = 0.0f;

	if (nRevIdx < 0)
		return 0.f;

	if (bMax == true)
		fValue = m_stResolution[nRevIdx].fHeightZoomMax;
	else
		fValue = m_stResolution[nRevIdx].fHeightZoom;

	return fValue;
}
