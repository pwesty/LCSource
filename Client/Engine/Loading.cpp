#include "StdH.h"
#include <Engine/Interface/UIInternalClasses.h>
#include "Loading.h"

#include <Engine/Interface/UIManager.h>
#include <Engine/World/World.h>
#include <Engine/Graphics/Raster.h>
#include <Engine/Graphics/ViewPort.h>

#define REFRESH_LOADING_TIME (0.2f)
// 로딩 팁은 최대 50개.
#define DEF_TIP_MAX (50)

extern CDrawPort*	_pdpMain;
extern BOOL			_bWorldEditorApp;

CLoadingImage::CLoadingImage()
	: m_pImgZone(NULL)
	, m_pImgZoneName(NULL)
	, m_pImgLoadingBar(NULL)
	, m_pTBTip(NULL)
	, m_fLoadingStepMax(16.f)
	, m_nCurStep(0)
	, m_nOriginWidth(0)
	, m_eState(ePS_END)
	, m_tvLastUpdate(0I64)
	, m_fLoadingUniStep(0.f)
{
}

CLoadingImage::~CLoadingImage()
{
	Destroy();

	m_pImgZone = NULL;
	m_pImgZoneName = NULL;
	m_pImgLoadingBar = NULL;
	m_pTBTip = NULL;

	m_vecTip.clear();
}

void CLoadingImage::initialize()
{
	CUIBase* pBase = new CUIBase;
	CUIManager::getSingleton()->LoadXML("loding.xml", pBase);
	addChild((CUIBase*)pBase);

	m_pImgZone = (CUIImage*)findUI("img_zone");
	m_pImgZoneName = (CUIImage*)findUI("img_zone_name");
	m_pImgLoadingBar = (CUIImage*)findUI("img_loadingbar");
	m_pTBTip = (CUITextBox*)findUI("tb_tip");

	m_nOriginWidth = m_pImgLoadingBar->GetWidth();

	int i;
	CTString strTmp;

	for (i = 0; i < DEF_TIP_MAX; ++i)
	{
		strTmp = _S(8000+i, "Tip!!");
		
		if (strTmp.IsEmpty() == TRUE)
			continue;

#ifdef G_KOR
		std::string str = strTmp;

		if (str.find("게임 팁") != std::string::npos)
			continue;
#endif // G_KOR
		m_vecTip.push_back(8000 + i);
	}
}

void CLoadingImage::InitLoadingData()
{
	if (m_pImgZone == NULL || m_pImgZoneName == NULL || m_pTBTip == NULL)
		return;

	CTString strZone;
	CTString strZoneName;

	if (g_slZone >= 0)
	{
		CZoneInfo* pZoneInfo = CZoneInfo::getSingleton();
		pZoneInfo->GetLoadingInfo(g_slZone, m_fLoadingStepMax, strZone, strZoneName);

		m_pImgZoneName->ReleaseTexData();

		m_pImgZoneName->setTexString(strZoneName);
		m_pImgZoneName->Hide(FALSE);

		m_pTBTip->SetText(GetTip());

		// 텍스트 센터 정렬. 
		int nTextHeight = m_pTBTip->GetTextTotalHeight();
		int nLine = m_pTBTip->GetLineHeight();

		if (nTextHeight <= nLine)
			m_pTBTip->SetStartXY(0, nLine);
		else if (nTextHeight <= nLine * 2)
			m_pTBTip->SetStartXY(0, nLine / 2);
		else
			m_pTBTip->SetStartXY(0, 0);

		m_pTBTip->OnUpdateBoxInfo();
	}
	else
	{
		strZone = "Loading\\0_startimage.tex";
		m_fLoadingStepMax = 16.f;
		// 인트로시 네임 안씀.
		m_pImgZoneName->Hide(TRUE);
	}

	m_pImgZone->ReleaseTexData();

	m_pImgZone->setTexString(strZone);
	Hide(FALSE);

	m_nCurStep = 0;
	m_fLoadingUniStep = 0.f;

	if (m_pImgLoadingBar != NULL)
	{
		UIRectUV uv = m_pImgLoadingBar->GetAbsUV();
		uv.U1 = 0.f;
		m_pImgLoadingBar->SetUV(uv);
	}

	

	CUIManager* pUIManager = CUIManager::getSingleton();

	int nCW = pUIManager->GetWidth();
	int nCH = pUIManager->GetHeight();

	SetSize(nCW, nCH, true);
}

void CLoadingImage::SetProgress( float f )
{
	if (_bWorldEditorApp == TRUE)
		return;

	m_fStepCompleted = f;

	m_eState = ePS_WARKING;
	UpdateProgress();
}

void CLoadingImage::StopLoading()
{
	if (_bWorldEditorApp == TRUE)
		return;

	m_eState = ePS_END;
	UpdateProgress();

	Hide(TRUE);
}

void CLoadingImage::UpdateProgress()
{
	if (m_pImgLoadingBar == NULL)
		return;

	if (m_nCurStep == m_fLoadingStepMax)
		return;

	// 마지막 업데이트 시간.
	CTimerValue tvNow = _pTimer->GetHighPrecisionTimer();	

	if ( m_fStepCompleted != 0.f && m_fStepCompleted != 1.f &&
		(tvNow - m_tvLastUpdate).GetSeconds() < REFRESH_LOADING_TIME )
	{
		return;
	}

	m_tvLastUpdate = tvNow;

	float fLoadingWidth = m_nOriginWidth / m_fLoadingStepMax;

	if (m_fStepCompleted >= 1 && m_eState == ePS_WARKING)
	{
		if (m_nCurStep < m_fLoadingStepMax)
		{
			++m_nCurStep;
			m_fLoadingUniStep = 0.f;
		}
	}
	else
	{
		if (m_nCurStep < m_fLoadingStepMax)
		{
			if (m_fLoadingUniStep < fLoadingWidth)
				m_fLoadingUniStep += fLoadingWidth / 10;
		}
	}

	UIRectUV uv = m_pImgLoadingBar->GetAbsUV();

	int nEndWidth = 0;

	if (m_eState == ePS_END)
		nEndWidth = m_nOriginWidth;
	else
	{
		nEndWidth = (m_nCurStep * fLoadingWidth) + m_fLoadingUniStep;

		if (nEndWidth > m_nOriginWidth)
			nEndWidth = m_nOriginWidth;
	}

	uv.U1 = uv.U0 + nEndWidth;
	m_pImgLoadingBar->SetWidth(nEndWidth);
	m_pImgLoadingBar->SetUV(uv);

	CDrawPort *pdp = _pdpMain;                           
	ASSERT(pdp!=NULL);
	CDrawPort dpHook(pdp, 0);
	dpHook.SetAsCurrent();

	// clear screen
	dpHook.Fill(C_BLACK|255);

	Render(&dpHook);

	dpHook.dp_Raster->ra_pvpViewPort->SwapBuffers();
}

CTString CLoadingImage::GetTip()
{
	srand(time(NULL));

	int nSize = m_vecTip.size();

	if (nSize <= 0)
		return CTString("");

	int nTipIdx = rand() % nSize;

	if (nTipIdx < 0 || nTipIdx >= nSize)
		return _S(m_vecTip[0], "Tip!!");

	return _S(m_vecTip[nTipIdx], "Tip!!!");
}
