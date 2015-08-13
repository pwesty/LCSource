#include "stdafx.h"
#include "LCDDrawing.h"
#include <Engine/Interface/UITextureManager.h>
#include <Engine/GlobalDefinition.h>
#include <Engine/GameStageManager/StageMgr.h>
#ifdef KALYDO
#include <Kalydo/KRFReadLib/Include/KRFReadLib.h>
#endif

// Define
#define	ROUND					0.005f
#define FIRST_INTO_WORLD		MAX_ZONE_COUNT - 1

//Text Pos
#define PIX_TEXT				18
#define PIY_TEXT				18

// static Variable
static CTextureObject	g_CTexCurLoading[2];
static CTextureObject	ptoProgressBar;
static CTextureObject	ptoLoadingBar;
static CTextureObject	toText;
static CTextureObject	toClassification;	//게임등급표시
static CTextureData*	ptdProgressBar = NULL;
static CTextureData*	ptdLoadingBar = NULL;
static UIRectUV			rtProgressBar[3];
static UIRectUV			rtLoadingBar[3];
static UIRectUV			rtClassification;

static int nLoadingStep = 0;
float g_fCurLoadingStep = 0.f;

// ----------------------------------------------------------------------------
// Name : ObtainLoadingData()
// Desc : written by seo 40815
// ----------------------------------------------------------------------------
BOOL ObtainLoadingData( void )
{
	nLoadingStep = 0;

	try 
	{
		if (g_slZone >= 0)
		{
			CTString strLoadingTex1;
			CTString strLoadingTex2;
			float	 fStep;
			CZoneInfo* pZoneInfo = CZoneInfo::getSingleton();
			
			pZoneInfo->GetLoadingInfo(g_slZone, fStep, strLoadingTex1, strLoadingTex2);
			
			g_CTexCurLoading[0].SetData_t(strLoadingTex1);
			g_CTexCurLoading[1].SetData_t(strLoadingTex2);
			g_fCurLoadingStep = fStep;
		}
		else
		{
			g_CTexCurLoading[0].SetData_t(CTFILENAME( "Data\\Interface\\Loading\\Loading00_1.tex" ));
			g_CTexCurLoading[1].SetData_t(CTFILENAME( "Data\\Interface\\Loading\\Loading00_2.tex" ));
			g_fCurLoadingStep = 16.f;
		}
		
		toClassification.SetData_t( CTFILENAME( "Data\\Interface\\Loading\\Classification_15.tex" ) );
		rtClassification.SetUV(0,0,64,64,toClassification.GetWidth(),toClassification.GetHeight());

		toText.SetData_t( CTFILENAME( "Data\\Interface\\Loading\\Text.tex" ) );

		// Load Progress Bar
		ptoProgressBar.SetData_t( CTFILENAME ( "Data\\Interface\\Loading\\ProgressBar.tex" ) );
		ptoLoadingBar.SetData_t( CTFILENAME ( "Data\\Interface\\Loading\\LoadingBar.tex" ) );

		ptdProgressBar = (CTextureData*)ptoProgressBar.GetData();
		ptdLoadingBar = (CTextureData*)ptoLoadingBar.GetData();

		FLOAT	fTexWidth	= ptdProgressBar->GetPixWidth();
		FLOAT	fTexHeight	= ptdProgressBar->GetPixHeight();

		// UV Coordinate of Progress Bar Icon
 		rtProgressBar[0].SetUV( 0, 0, 13, 17, fTexWidth, fTexHeight );
		rtProgressBar[1].SetUV( 13, 0, 36, 17, fTexWidth, fTexHeight );
		rtProgressBar[2].SetUV( 49, 0, 62, 17, fTexWidth, fTexHeight );

		fTexWidth	= ptdLoadingBar->GetPixWidth();
		fTexHeight	= ptdLoadingBar->GetPixHeight();

		// UV Coordinate of Loading Bar Icon
 		rtLoadingBar[0].SetUV( 0, 0, 4, 11, fTexWidth, fTexHeight );
		rtLoadingBar[1].SetUV( 4, 0, 7, 11, fTexWidth, fTexHeight );
		rtLoadingBar[2].SetUV( 7, 0, 11, 11, fTexWidth, fTexHeight );
				
	} 
	catch( char *strError ) 
	{
		CPrintF("%s\n", strError);
		return FALSE;
	}
	return TRUE;
}

// ----------------------------------------------------------------------------
// Name : ReleaseLoadingData()
// Desc : 
// Last Edit : 이기환 ( 12. 10 )
// ----------------------------------------------------------------------------
void ReleaseLoadingData( void )
{
	g_CTexCurLoading[0].SetData(NULL);
	g_CTexCurLoading[1].SetData(NULL);

	ptoProgressBar.SetData ( NULL );
	ptdProgressBar = NULL;
	ptoLoadingBar.SetData ( NULL );
	ptdLoadingBar = NULL;

	toText.SetData ( NULL );

	toClassification.SetData ( NULL );
	//toClassification = NULL;

}

// ----------------------------------------------------------------------------
// Name : RenderLoading()
// Desc : 
// Last Edit : 이기환 ( 12. 10 )
// ----------------------------------------------------------------------------
void RenderLoading( CDrawPort *pdp, ULONG ulLevelMask, CProgressHookInfo *pphi)
{
	static float fMaxStep = 0.0f;

	// 최초 실행
	if( pphi->phi_phsStatus == PHS_INIT )	
	{
		if( !ObtainLoadingData() )
		{
			ReleaseLoadingData();
			return;
		}
		
		fMaxStep = g_fCurLoadingStep;
	}
	// Render Back Image ==============================================================================
	
  	PIX pixdpw = pdp->GetWidth();
	PIX pixdph = pdp->GetHeight();

	FLOAT fPointStretch	= 0.0f;;	// 축소 시 아래 쪽 그림이 출력될 위치
	FLOAT fStretch		= 1.0f;	// 가로대 세로 축소 비율 ( 800/600, 1024/768 ... )
			
	PIX pixOriginalX	= 1024;		// 원본 이미지의 크기
	PIX pixOriginalY	= 768;

	PIX ImageSizeY512	= 512;		// 커팅 된 이미지 싸이즈
	PIX ImageSizeY256	= 256;

	PIX pixSX			= 0;		// 이미지 출력 위치
	PIX pixSY			= 0;

	float fPersent		= 1.0f;
	
	if( pixdpw == 1024 && pixdph == 768 )	// 1024 x 768 : 그냥 뿌러 주자
	{
 		pdp->PutTexture( &g_CTexCurLoading[0], PIXaabbox2D( PIX2D(pixSX,pixSY), PIX2D(pixOriginalX,ImageSizeY512)), C_WHITE|255 );
 		pdp->PutTexture( &g_CTexCurLoading[1], PIXaabbox2D( PIX2D(pixSX,ImageSizeY512), PIX2D(pixOriginalX,pixOriginalY)), C_WHITE|255 );
	}
	else if ( pixdph < 768 ) // 1024 x 768 보다 작을 때 ( 800 x 600 ) : 세로 크기를 기준으로 축소 한두 가운데 정렬
	{
		fStretch		= 1.3333f;
		
		fPersent = (float)pixdph / (float)pixOriginalY;
		
		// 축소
		fPointStretch	= (FLOAT)ImageSizeY512 / (FLOAT)pixOriginalY;
		PIX	pixPointY	= ( pixdph * fPointStretch );

		// 비율로 조정된 가로 위치 ( 소수점 3째 자리에서 반올림 )
		pixOriginalX	= pixdph *  fStretch + ROUND;
		
		// 가로 가운데 정렬
		pixSX			= ( pixdpw - pixOriginalX ) / 2;
		
 		pdp->PutTexture( &g_CTexCurLoading[0], PIXaabbox2D( PIX2D(pixSX,pixSY), PIX2D(pixdpw-pixSX,pixPointY)), C_WHITE|255);
 		pdp->PutTexture( &g_CTexCurLoading[1], PIXaabbox2D( PIX2D(pixSX,pixPointY), PIX2D(pixdpw-pixSX,pixdph)), C_WHITE|255);

	}
	else // 1024 x 768 보다 클 때	// 가운대로 띄우자
	{
		// 출력 위치를 가운데로 보정
		pixSX = ( pixdpw - pixOriginalX ) / 2;
		pixSY = ( pixdph - pixOriginalY ) / 2;
		
 		pdp->PutTexture( &g_CTexCurLoading[0], PIXaabbox2D( PIX2D(pixSX, pixSY), PIX2D(pixSX+pixOriginalX,pixSY+ImageSizeY512)), C_WHITE|255);
 		pdp->PutTexture( &g_CTexCurLoading[1], PIXaabbox2D( PIX2D(pixSX, pixSY+ImageSizeY512), PIX2D(pixSX+pixOriginalX, pixSY+pixOriginalY)), C_WHITE|255);
	}

	// Render Text
	if ( g_slZone == -1 ) // 처음 월드로 들어갈 때 화면
	{
		float fWidth = toText.GetWidth();	// * fPersent;	// 등급표시는 축소 안함
		float fHeight = toText.GetHeight();	// * fPersent;		
			
		// 배경 이미지의 우측 상단
		PIX	pixX = pixSX + PIX_TEXT;
		PIX	pixY = pixSY + PIY_TEXT;
				
		pdp->PutTexture( &toText, PIXaabbox2D( PIX2D(pixX, pixY), PIX2D(pixX+fWidth,pixY+fHeight)), C_WHITE|255);	
	}

	if ( g_slZone == -1 || g_slZone == 14 ) // 임시 14:OX존,마지막존 기본 로딩이미지로...
	{
		
		// Render Progress Bar ==============================================================================
		float m_pX				= pixSX + 49 * fPersent;		// 이미지 출력 위치
		float m_pY				= pixSY + 736 * fPersent - ROUND;
		
		float fUniStepWidth		= 0.0f;							// 단계 하나가 차지 하는 이미지 길이
		
		float fSideImageWidth	= 12.0f * fPersent;				// 시작 이미지의 크기
		float fSideImageHeight	= 16.0f * fPersent; 
		
		float fProgressLength	= 902.0f / fMaxStep * fPersent;	// 한단계의 로딩이 이동해야 하는 길이
		
		float m_eX = 0.0f;
		
		// Render Start ...
 		pdp->InitTextureData( ptdProgressBar );
 		
// 		// Render Start Image
		pdp->AddTexture( m_pX, m_pY, m_pX + fSideImageWidth, m_pY + fSideImageHeight,
			rtProgressBar[0].U0, rtProgressBar[0].V0, rtProgressBar[0].U1, rtProgressBar[0].V1,
			0xFFFFFFFF );
		
		// Progress Start Position 
		m_pX += fSideImageWidth;
		
		if( ( pphi->phi_fCompleted >= 1 ) && ( pphi->phi_phsStatus == PHS_WORKING ) )
		{
			if ( nLoadingStep < fMaxStep ) 
			{
				nLoadingStep++;
			}
		}
		else 
		{
			fUniStepWidth = fProgressLength * ( ( pphi->phi_phsStatus == PHS_WORKING)?pphi->phi_fCompleted:0 );
		}
		
		// Progress End Position
		m_eX  = m_pX + ( nLoadingStep * fProgressLength ) + fUniStepWidth;
		
		
		// Render Middle Image
		pdp->AddTexture(  m_pX, m_pY, m_eX, m_pY + fSideImageHeight,
			rtProgressBar[1].U0, rtProgressBar[1].V0, rtProgressBar[1].U1, rtProgressBar[1].V1,
			0xFFFFFFFF );
		m_pX = m_eX;
		
		// Render End Image
		pdp->AddTexture( m_pX, m_pY, m_pX + fSideImageWidth, m_pY + fSideImageHeight,
			rtProgressBar[2].U0, rtProgressBar[2].V0, rtProgressBar[2].U1, rtProgressBar[2].V1,
			0xFFFFFFFF );
		
		// ... Render Start 
		pdp->FlushRenderingQueue();
		
		// Render Text ( Description ) =========================================================================
		float fFontSize = fPersent / 2 - 0.05;
#if defined G_RUSSIA
		pdp->SetFont( _pfdDefaultFont );
		pdp->SetTextScaling( fFontSize * 2.5 );
#else
		pdp->SetFont( _pfdDisplayFont );
		pdp->SetTextScaling( fFontSize );
#endif
		pdp->SetTextAspect( 1.0f );
		pdp->SetTextShadow( +2 );
		
		m_pX = pixSX + pixOriginalX / 2;
		pdp->PutTextC ( pphi->phi_strDescription, m_pX, m_pY, 0xD88842FF );
		
	}
	else
	{			
		// Render Progress Bar ==============================================================================
		float m_pX				= pixSX + 149 * fPersent;		// 이미지 출력 위치
		float m_pY				= pixSY + 612 * fPersent - ROUND;
		
		float fUniStepWidth		= 0.0f;							// 단계 하나가 차지 하는 이미지 길이
		
		float fSideImageWidth	= 11.0f * fPersent;				// 시작 이미지의 크기
		float fSideImageHeight	= 11.0f * fPersent; 
		
		// [2012/01/19 : Sora] 로딩 바 길이가 727인데 11(시작 이미지) + 716(중간 이미지) + 11(끝)으로 계산해서 벗어나고 있었음(716 -> 704로 수정)
		float fLoadingLength	= 704.0f / fMaxStep * fPersent;	// 한단계의 로딩이 이동해야 하는 길이
		
		float m_eX = 0.0f;
		
		// Render Start ...
		pdp->InitTextureData( ptdLoadingBar );
		
		// Render Start Image
		pdp->AddTexture( m_pX, m_pY, m_pX + fSideImageWidth, m_pY + fSideImageHeight,
			rtLoadingBar[0].U0, rtLoadingBar[0].V0, rtLoadingBar[0].U1, rtLoadingBar[0].V1,
			0xFFFFFFFF );
		
		// Loading Start Position 
		m_pX += fSideImageWidth;
		
		if( ( pphi->phi_fCompleted >= 1 ) && ( pphi->phi_phsStatus == PHS_WORKING ) )
		{
			if ( nLoadingStep < fMaxStep ) 
			{
				nLoadingStep++;
			}
		}
// [2012/01/10 : Sora] 굳이 더할 필요 없을듯;; 일단 러시아만 털어 본다
#if !defined(G_RUSSIA)
		else 
		{
			if ( nLoadingStep < fMaxStep ) 
			{
				fUniStepWidth = fLoadingLength * ( ( pphi->phi_phsStatus == PHS_WORKING)?pphi->phi_fCompleted:0 );
			}
		}
#endif
		
		// Loading End Position
		m_eX  = m_pX + ( nLoadingStep * fLoadingLength ) + fUniStepWidth;
		
		// Render Middle Image
		pdp->AddTexture(  m_pX, m_pY, m_eX, m_pY + fSideImageHeight,
			rtLoadingBar[1].U0, rtLoadingBar[1].V0, rtLoadingBar[1].U1, rtLoadingBar[1].V1,
			0xFFFFFFFF );
		m_pX = m_eX;
		
		// Render End Image
		pdp->AddTexture( m_pX, m_pY, m_pX + fSideImageWidth, m_pY + fSideImageHeight,
			rtLoadingBar[2].U0, rtLoadingBar[2].V0, rtLoadingBar[2].U1, rtLoadingBar[2].V1,
			0xFFFFFFFF );
		
		// ... Render Start 
		pdp->FlushRenderingQueue();
		
		// Render Text ( Description ) =========================================================================
		float fFontSize = fPersent / 2 - 0.05;
#if defined G_RUSSIA
		pdp->SetFont( _pfdDefaultFont );
		pdp->SetTextScaling( fFontSize * 2.5 );
#else
		pdp->SetFont( _pfdDisplayFont );
		pdp->SetTextScaling( fFontSize );
#endif
		pdp->SetTextAspect( 1.0f );
		pdp->SetTextShadow( +2 );
		
		m_pX = pixSX + pixOriginalX / 2;
		pdp->PutTextC ( pphi->phi_strDescription, m_pX, m_pY, 0xD88842FF );
	}
	
	// free textures used in map rendering
	if( pphi->phi_phsStatus == PHS_END )
	{
		ReleaseLoadingData();

		// 로딩이 완료되면, Stage 전환
		if (STAGEMGR()->GetCurStage() == eSTAGE_ZONELOADING)
		{
			_pNetwork->SendGameStart();
		}
	}
}

