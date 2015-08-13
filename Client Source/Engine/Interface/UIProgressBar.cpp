/********************************************************************
	UIProgressBar.cpp : implementation of the CUIProgressBar class.
*********************************************************************/

#include "stdh.h"
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Interface/UIProgressBar.h>

CUIProgressBar::CUIProgressBar()
:	m_strText(""),
	m_colText( 0xFFFFFFFF ),
	m_sqLower( 0 ),
	m_sqUpper( 100 ),
	m_sqOffset( 1 ),
	m_sqCurrentPos( 0 ),
	m_nTexGap( 1 ),
	m_bShowText( TRUE ),
	m_bShowPercentText( TRUE ),
	m_bAutoProgressComplete( FALSE ),
	m_enStatus( PR_STAT_IDLE ),
	m_fCurPercent( 0.0f ),
	m_strCompleteText(""),
	m_strDoingText(""),
	m_strIdleText(""),
	m_colCompleteText(0xFFFFFFFF),
	m_colDoingText(0xFFFFFFFF),
	m_colIdleText(0xFFFFFFFF),
	m_bShowPercentInComplete(FALSE),
	m_bShowPercentInDoing(FALSE),
	m_bShowPercentInIdle(FALSE),
	m_sqStep(0),
	m_nStartTime(-1),
	m_pCmdComplete(NULL)
{
	setType(eUI_CONTROL_PROGRESSBAR);
	m_nBarSX = 0;
	m_nBarSY = 0;
	int i;
	for( i = 0; i < PR_STAT_TAG_END; i++ )
	{
		m_strProgressText[i] = "";
		m_bPercentText[i] = FALSE;
		m_colProgressText[i] = 0xFFFFFFFF;
#ifdef UI_TOOL
		m_iStringIdx[i] = -1;
#endif // UI_TOOL
	}
}

CUIProgressBar::~CUIProgressBar()
{
	Destroy();
	SAFE_DELETE(m_pCmdComplete);
}

CUIBase* CUIProgressBar::Clone()
{
	CUIProgressBar* pProgress = new CUIProgressBar(*this);
	pProgress->setTexString( getTexString() );

	CUIBase::CloneChild(pProgress);

	return pProgress;
}

void			CUIProgressBar::Create(CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	CUIWindow::Create(pParentWnd, nX, nY, nWidth, nHeight);
	
	m_ptdBaseTexture = CreateTexture( CTString("Data\\Interface\\CommonBtn.tex") );
	FLOAT fTexWidth = m_ptdBaseTexture->GetPixWidth();
	FLOAT fTexHeight = m_ptdBaseTexture->GetPixHeight();

	m_rtsBase[TL].AddRectSurface( UIRect( 0, 0,	1, 4 ),
									UIRectUV( UI_PROGRESS_TEX_X, UI_PROGRESS_TEX_Y,
											  UI_PROGRESS_TEX_X+2, UI_PROGRESS_TEX_Y+4,
											  fTexWidth, fTexHeight ) );
	m_rtsBase[TM].AddRectSurface( UIRect( 1, 0,	nWidth-1, 4 ),
									UIRectUV( UI_PROGRESS_TEX_X+2, UI_PROGRESS_TEX_Y,
											  UI_PROGRESS_TEX_X+158, UI_PROGRESS_TEX_Y+4,
											  fTexWidth, fTexHeight ) );
	m_rtsBase[TR].AddRectSurface( UIRect( nWidth-1, 0, nWidth, 4 ),
									UIRectUV( UI_PROGRESS_TEX_X+158, UI_PROGRESS_TEX_Y,
											  UI_PROGRESS_TEX_X+160, UI_PROGRESS_TEX_Y+4,
											  fTexWidth, fTexHeight ) );

	m_rtsBase[ML].AddRectSurface( UIRect( 0, 4, 1, nHeight-2 ),
									UIRectUV( UI_PROGRESS_TEX_X, UI_PROGRESS_TEX_Y+4,
											  UI_PROGRESS_TEX_X+2, UI_PROGRESS_TEX_Y+18,
											  fTexWidth, fTexHeight ) );
	m_rtsBase[MM].AddRectSurface( UIRect( 1, 4, nWidth-1, nHeight-2 ),
									UIRectUV( UI_PROGRESS_TEX_X+2, UI_PROGRESS_TEX_Y+4,
											  UI_PROGRESS_TEX_X+158, UI_PROGRESS_TEX_Y+18,
											  fTexWidth, fTexHeight ) );
	m_rtsBase[MR].AddRectSurface( UIRect( nWidth-1, 4, nWidth, nHeight-2 ),
									UIRectUV( UI_PROGRESS_TEX_X+158, UI_PROGRESS_TEX_Y+4,
											  UI_PROGRESS_TEX_X+160, UI_PROGRESS_TEX_Y+18,
											  fTexWidth, fTexHeight ) );
	
	m_rtsBase[BL].AddRectSurface( UIRect( 0, nHeight-2, 1, nHeight ),
									UIRectUV( UI_PROGRESS_TEX_X, UI_PROGRESS_TEX_Y+18,
											  UI_PROGRESS_TEX_X+2, UI_PROGRESS_TEX_Y+21,
											  fTexWidth, fTexHeight ) );
	m_rtsBase[BM].AddRectSurface( UIRect( 1, nHeight-2, nWidth-1, nHeight ),
									UIRectUV( UI_PROGRESS_TEX_X+2, UI_PROGRESS_TEX_Y+18,
											  UI_PROGRESS_TEX_X+158, UI_PROGRESS_TEX_Y+21,
											  fTexWidth, fTexHeight ) );
	m_rtsBase[BR].AddRectSurface( UIRect( nWidth-1, nHeight-2, nWidth, nHeight ),
									UIRectUV( UI_PROGRESS_TEX_X+158, UI_PROGRESS_TEX_Y+18,
											  UI_PROGRESS_TEX_X+160, UI_PROGRESS_TEX_Y+21,
											  fTexWidth, fTexHeight ) );
	
	m_rtBar.SetRect( 0, 0, nWidth-2, nHeight-4 );
	m_rtuBar.SetUV( UI_PROGRESS_INNER_TEX_X, UI_PROGRESS_INNER_TEX_Y,
					UI_PROGRESS_INNER_TEX_X+UI_PROGRESS_INNER_TEX_WIDTH, UI_PROGRESS_INNER_TEX_Y+UI_PROGRESS_INNER_TEX_HEIGHT, 
					fTexWidth, fTexHeight );
	_setTextureGap();
	SetStatus();
}

void			CUIProgressBar::Render()
{
	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	int nX, nY;
	GetAbsPos( nX, nY );
	pDrawPort->InitTextureData( m_ptdBaseTexture );

	for( int i = TL; i < FRAME_TAG_END; ++i )
	{
		m_rtsBase[i].SetPos( nX, nY );
		m_rtsBase[i].RenderRectSurface( pDrawPort, 0xFFFFFFFF );
	}
	pDrawPort->FlushRenderingQueue();
	
 	pDrawPort->AddTexture( nX+1, nY+2, 
 										nX + m_sqCurrentPos*m_nTexGap, nY + m_nHeight-2,
 										m_rtuBar.U0, m_rtuBar.V0,
 										m_rtuBar.U1, m_rtuBar.V1,
 										0xFFFFFFFF );
	pDrawPort->FlushRenderingQueue();

	// if you can try show message, you will must call SetShowText functions.
	if( m_bShowText )
	{
		float fPercent = _calcProgressPercent();
		CTString strTemp;
		if( m_enStatus == PR_STAT_IDLE )
		{	
			if( IsShowIdlePercentText() )
				strTemp.PrintF( "%s (%0.00f%%)",  m_strIdleText, fPercent );
			else
				strTemp.PrintF( "%s",  m_strIdleText );
		}
		else if( m_enStatus == PR_STAT_DOING )
		{
			if( IsShowDoingPercentText() )
				strTemp.PrintF( "%s (%0.00f%%)",  m_strDoingText, fPercent );
			else
				strTemp.PrintF( "%s",  m_strDoingText );
		}
		else if( m_enStatus == PR_STAT_COMPLETE )
		{
			if( IsShowCompletePercentText() )
				strTemp.PrintF( "%s (%0.00f%%)",  m_strCompleteText, fPercent );
			else
				strTemp.PrintF( "%s",  m_strCompleteText );
		}
		else
		{
			ASSERT( m_enStatus == PR_STAT_TAG_END );
			strTemp.PrintF("Error! status is not valid\n" );
		}

		pDrawPort->PutTextExCX(strTemp, nX+m_nWidth/2, nY+m_nHeight/2-5, m_colText );
 		pDrawPort->EndTextEx();
		pDrawPort->FlushRenderingQueue();
	}
}

void				CUIProgressBar::_setTextureGap()
{ 
	m_nTexGap = ((m_nWidth - (m_nBarSX * 2))-1.0f) / (float)(m_sqUpper - m_sqLower); 
}

const float			CUIProgressBar::_calcProgressPercent()
{
	m_fCurPercent = ( m_sqCurrentPos / (FLOAT)(m_sqUpper-m_sqLower) ) * 100.0f;

	if( m_fCurPercent >= 100 )
	{
		if( m_bAutoProgressComplete )
			m_enStatus = PR_STAT_COMPLETE;
		else
			m_fCurPercent = 100.0f;
	}

	return m_fCurPercent;
}

void CUIProgressBar::OnRender( CDrawPort* pDraw )
{
	int nX, nY;
	GetAbsPos( nX, nY );
	if( m_bHide == TRUE || m_pTexData == NULL)
		return;

	pDraw->InitTextureData( m_pTexData );
	
	pDraw->AddTexture(nX, nY, nX + m_nWidth, nY + m_nHeight
		, m_uvBack.U0, m_uvBack.V0, m_uvBack.U1, m_uvBack.V1, 0xFFFFFFFF);

	float fTexWidth = m_nBarSX + m_sqCurrentPos*m_nTexGap;

	pDraw->AddTexture( nX+m_nBarSX, nY+m_nBarSY, 
		nX + fTexWidth, nY + m_nHeight-m_nBarSY,
		m_rtuBar.U0, m_rtuBar.V0,
		m_rtuBar.U1, m_rtuBar.V1,
		0xFFFFFFFF );

	pDraw->FlushRenderingQueue();

	// if you can try show message, you will must call SetShowText functions.
	if( m_bShowText )
	{
		float fPercent = _calcProgressPercent();
		CTString strTemp;

		if( getShowPercentText( m_enStatus ) )
			strTemp.PrintF( "%s (%0.00f%%)",  m_strProgressText[m_enStatus], fPercent );
		else
			strTemp.PrintF( "%s",  m_strProgressText[m_enStatus] );

		pDraw->PutTextExCX(strTemp, nX+m_nWidth/2, nY+m_nHeight/2-5, m_colProgressText[m_enStatus] );
		pDraw->EndTextEx();
	}

#ifdef UI_TOOL
	RenderBorder(pDraw);
#endif // UI_TOOL
}

void CUIProgressBar::initialize()
{
}

void CUIProgressBar::setUVBack( UIRectUV uv )
{
	if( m_pTexData != NULL )
	{
		FLOAT fTexWidth = m_pTexData->GetPixWidth();
		FLOAT fTexHeight = m_pTexData->GetPixHeight();

		uv.U0 /= fTexWidth ;
		uv.V0 /= fTexHeight ;
		uv.U1 /= fTexWidth ;
		uv.V1 /= fTexHeight ;
	}
	m_uvBack = uv;
}

void CUIProgressBar::setUVBar( UIRectUV uv )
{
	if( m_pTexData != NULL )
	{
		FLOAT fTexWidth = m_pTexData->GetPixWidth();
		FLOAT fTexHeight = m_pTexData->GetPixHeight();

		uv.U0 /= fTexWidth ;
		uv.V0 /= fTexHeight ;
		uv.U1 /= fTexWidth ;
		uv.V1 /= fTexHeight ;
	}
	m_rtuBar = uv;
}

UIRectUV CUIProgressBar::getUVBack()
{
	UIRectUV uv = m_uvBack;
	if( m_pTexData != NULL )
	{
		FLOAT fTexWidth = m_pTexData->GetPixWidth();
		FLOAT fTexHeight = m_pTexData->GetPixHeight();

		uv.U0 *= fTexWidth ;
		uv.V0 *= fTexHeight ;
		uv.U1 *= fTexWidth ;
		uv.V1 *= fTexHeight ;
	}
	return uv;
}

UIRectUV CUIProgressBar::getUVBar()
{
	UIRectUV uv = m_rtuBar;
	if( m_pTexData != NULL )
	{
		FLOAT fTexWidth = m_pTexData->GetPixWidth();
		FLOAT fTexHeight = m_pTexData->GetPixHeight();

		uv.U0 *= fTexWidth ;
		uv.V0 *= fTexHeight ;
		uv.U1 *= fTexWidth ;
		uv.V1 *= fTexHeight ;
	}
	return uv;
}

void CUIProgressBar::setProgressText( _tagProgressStatus state, CTString &strText, BOOL _bShowPercent /*= FALSE*/, COLOR colText /*= 0xFFFFFFF */ )
{
	if( state < 0 && state > PR_STAT_TAG_END )
		return;

	m_strProgressText[state] = strText;
	setProgressTextColor(state, colText);
	m_bPercentText[state] = _bShowPercent; 
}

void CUIProgressBar::OnUpdate( float fDeltaTime, ULONG ElapsedTime )
{
	if (m_bAutoProgressComplete == TRUE && m_enStatus == PR_STAT_DOING)
	{
		UpdateBarPos();
	}
}

void CUIProgressBar::UpdateBarPos()
{
	m_sqCurrentPos += getStep();

	if (m_sqCurrentPos >= m_sqUpper)
		CompleteProgress();
}

void CUIProgressBar::CompleteProgress()
{
	OffsetProgressPos( m_sqUpper ); 
	m_enStatus = PR_STAT_COMPLETE;
	
	if (m_pCmdComplete != NULL)
		m_pCmdComplete->execute();
}

