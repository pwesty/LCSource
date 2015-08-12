#include "stdh.h"
#include <vector>
#include <Engine/Interface/UIAD.h>
#include <Engine/Entities/EntityProperties.h>
#include <Engine/Interface/UIManager.h>
#include <Engine/Interface/UITextureManager.h>

#define ANI_A1_DELAY	0.2f;

// ----------------------------------------------------------------------------
// Name : CUIAD()
// Desc : Constructorreturn ;
// ----------------------------------------------------------------------------
CUIAD::CUIAD()
{	
	m_fDelay = 0.0f;
	m_selAni = 0;

}	

// ----------------------------------------------------------------------------
// Name : ~CUIAD()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUIAD::~CUIAD()
{
	Destroy();
}

// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUIAD::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	CUIWindow::Create(pParentWnd, nX, nY, nWidth, nHeight);	

	// Create AD texture
	m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\AD.tex" ) );
	
	FLOAT	fTexWidth = m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight = m_ptdBaseTexture->GetPixHeight();

	// AD UV
	m_uvAD[0].SetUV(0,0,104,48,fTexWidth,fTexHeight);
	m_uvAD[1].SetUV(0,56,104,104,fTexWidth,fTexHeight);
	
	// Attach Map window
	extern ENGINE_API INDEX sam_iScreenSizeI;
//	m_rcAD[0].SetRect(sam_iScreenSizeI - 115, 145,sam_iScreenSizeI - 14,197);
	m_rcAD[0].SetRect(sam_iScreenSizeI - 115, 152,sam_iScreenSizeI - 14,205);
	m_rcAD[1].CopyRect( m_rcAD[0] );

	m_rcAD_ani = m_rcAD[0];
	m_eAniState = ANI_IDLE;
	m_eAniType = SLOW_APPEAR;
	SetPos( m_rcAD[0].Left, m_rcAD[0].Top );
	SetSize( m_rcAD[0].GetWidth(), m_rcAD[0].GetHeight() );

	// Set Delay Time(sec)
	m_delayT = 60;

	// start timer
	m_startT = _pTimer->GetHighPrecisionTimer().GetSeconds();
	
	// random seed
	srand(time(NULL));
}


// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
void CUIAD::Render()
{
	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	// Set skill learn texture
	pDrawPort->InitTextureData( m_ptdBaseTexture );

	SelectLogo();

	PlayLC(m_eAniType);
	
	// Attach Map window
	extern ENGINE_API INDEX sam_iScreenSizeI;
//	m_rcAD[0].SetRect(sam_iScreenSizeI - 115, 145,sam_iScreenSizeI - 14,197);
	m_rcAD[0].SetRect(sam_iScreenSizeI - 115, 152,sam_iScreenSizeI - 14,205);
	m_rcAD[1].CopyRect( m_rcAD[0] );

	m_rcAD_ani = m_rcAD[0];
	
	pDrawPort->AddTexture( m_rcAD_ani.Left  , m_rcAD_ani.Top  , m_rcAD_ani.Right , m_rcAD_ani.Bottom ,
		m_uvAD[m_selAni].U0, m_uvAD[m_selAni].V0,	m_uvAD[m_selAni].U1, m_uvAD[m_selAni].V1,	0xFFFFFFFF );
	

	// Render all elements
	pDrawPort->FlushRenderingQueue();	
	
	// Flush all render text queue
	pDrawPort->EndTextEx();
}

// ----------------------------------------------------------------------------
// Name : SelectLogo()
// Desc :
// ----------------------------------------------------------------------------
RESULT CUIAD::SelectLogo()
{
	
	if( TimeGap() > m_delayT 
		&& m_eAniState == ANI_END)
	{
				
//		random
//		m_selAni = m_listQ[rand()%m_listQ.size()];
		m_selAni++;
		if(m_selAni > m_listQ.size()-1) m_selAni = 0;
		
		m_rcAD_ani = m_rcAD[m_listQ[m_selAni]];
		
		m_startT = _pTimer->GetHighPrecisionTimer().GetSeconds();
		m_eAniState = ANI_IDLE;
		return TRUE;
	}
	
	return FALSE;

}

// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT	CUIAD::MouseMessage( MSG *pMsg )
{
/*
	// Mouse point
	static int	nOldX, nOldY;
	int	nX = LOWORD( pMsg->lParam );
	int	nY = HIWORD( pMsg->lParam );
	
	// Mouse message
	switch( pMsg->message )
	{
		case WM_MOUSEMOVE:
		{
			if( IsInside( nX, nY ) )
			{
				if( m_eAniState == ANI_IDLE ) m_eAniState = ANI_DISAPPEAR;
				else if( m_eAniState == ANI_DISAPPEAR_END ) m_eAniState = ANI_START;
			}
			
		}
		break;
	
	}
	*/
	return WMSG_FAIL;
}


// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc :
// ----------------------------------------------------------------------------
void CUIAD::PlayLC(ANI_TYPE aniType)
{
	switch(aniType)
	{
		case SLOW_APPEAR :
			{
				switch(m_eAniState)
				{
					case ANI_IDLE :
						{
							m_rcAD_ani.Left = m_rcAD_ani.Right;
					//		if(TimeGap()>5)
								m_eAniState = ANI_APPEAR;
				
						}
						break;

					case ANI_DISAPPEAR :
						{
							m_rcAD_ani.Left = m_rcAD[m_selAni].Left;
							m_eAniState = ANI_DISAPPEAR_ON;
							m_fDelay = 0.0f;
							break;
						}

					case ANI_DISAPPEAR_ON :
						{
							if( m_rcAD_ani.Right <= m_rcAD[m_selAni].Left ) {
								m_rcAD_ani.Right = m_rcAD[m_selAni].Left;
								m_eAniState = ANI_END;
								m_fDelay = 0.0f;
							}
							else {
								m_fDelay +=ANI_A1_DELAY;
								m_rcAD_ani.Right -= m_fDelay;
							}
							break;
						}
										
					case ANI_APPEAR :
						{
							m_rcAD_ani.Left = m_rcAD[m_selAni].Right ;
							m_eAniState = ANI_APPEAR_ON;
							m_fDelay = 0.0f;
							break;
						}
					case ANI_APPEAR_ON :
						{
							
							if( m_rcAD_ani.Left <= m_rcAD[m_selAni].Left ) {
								m_rcAD_ani.Left = m_rcAD[m_selAni].Left;
								if(TimeGap() > m_delayT) 
									m_eAniState = ANI_DISAPPEAR;
								
							}
							else {
								m_fDelay +=ANI_A1_DELAY;
								m_rcAD_ani.Left -= m_fDelay;
							}
							break;
								
						}
					case ANI_END :
						{
							m_fDelay = 0.0f;
							
							break;
						}
				}		
			}
		break;	// SLOW_APPEAR
		
	}
}

void CUIAD::OnUpdate( float fElapsedTime )
{
	SelectLogo();

	PlayLC(m_eAniType);

	// Attach Map window
	extern ENGINE_API INDEX sam_iScreenSizeI;
	//	m_rcAD[0].SetRect(sam_iScreenSizeI - 115, 145,sam_iScreenSizeI - 14,197);

	m_rcAD[0].SetRect(sam_iScreenSizeI - 115, 152,sam_iScreenSizeI - 14,205);
	m_rcAD[1].CopyRect( m_rcAD[0] );

	m_rcAD_ani = m_rcAD[0];
}

void CUIAD::OnRender( CDrawPort* pDraw )
{
	// Set skill learn texture
	pDraw->InitTextureData( m_ptdBaseTexture );

	pDraw->AddTexture( m_rcAD_ani.Left  , m_rcAD_ani.Top  , m_rcAD_ani.Right , m_rcAD_ani.Bottom ,
		m_uvAD[m_selAni].U0, m_uvAD[m_selAni].V0,	m_uvAD[m_selAni].U1, m_uvAD[m_selAni].V1,	0xFFFFFFFF );

	// Render all elements
	pDraw->FlushRenderingQueue();	

	// Flush all render text queue
	pDraw->EndTextEx();
}
