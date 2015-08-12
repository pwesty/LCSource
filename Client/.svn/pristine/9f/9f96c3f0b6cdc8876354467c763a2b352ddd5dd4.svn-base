#include "stdh.h"
#include <Engine/Interface/UIDrawFigure.h>
#include <Engine/Interface/UIManager.h>
#include <Engine/Interface/UITextureManager.h>

// External variables
extern HWND	_hwndMain;


// ----------------------------------------------------------------------------
// Name : WRect()
// Desc : 
// ----------------------------------------------------------------------------
WRect::WRect()
{
	// Set Default value
	Left	= 0;
	Right	= 0;
	Top		= 0;
	Bottom	= 0;
}

WRect::WRect(int tLeft,int tTop,int tRight,int tBottom)
{
	Left	= tLeft;
	Right	= tRight;
	Top		= tTop;
	Bottom	= tBottom;
}

WRect& WRect::operator=(const WRect ct)
{
	Left	= ct.Left;
	Right	= ct.Right;
	Top		= ct.Top;
	Bottom	= ct.Bottom;		
	return *this;
}
WRect& WRect::operator=(const UIRect ct)
{
	Left	= ct.Left;
	Right	= ct.Right;
	Top		= ct.Top;
	Bottom	= ct.Bottom;
	return *this;
}
WRect& WRect::operator-=(int ct)
{
	Left	+= ct;
	Right	-= ct;
	Top		+= ct;
	Bottom	-= ct;
	return *this;
}

WRect& WRect::operator+=(int ct)
{
	Left	-= ct;
	Right	+= ct;
	Top		-= ct;
	Bottom	+= ct;
	return *this;
}


void WRect::SetRect( int nLeft, int nTop, int nRight, int nBottom )
{
	Left = nLeft; Top = nTop; Right = nRight; Bottom = nBottom;
}
void WRect::CopyRect( const WRect &rcSrc )
{
	Left = rcSrc.Left; Top = rcSrc.Top; Right = rcSrc.Right; Bottom = rcSrc.Bottom;
}
void WRect::CopyRect( const UIRect &rcSrc )
{
	Left = rcSrc.Left; Top = rcSrc.Top; Right = rcSrc.Right; Bottom = rcSrc.Bottom;
}
void WRect::Scale( FLOAT fScale )
{
	Left *= fScale;	Top *= fScale;	Right *= fScale;	Bottom *= fScale;
}
void WRect::Offset( int nX, int nY )
{
	Left += nX;		Top += nY;		Right += nX;		Bottom += nY;
}

UIRect WRect::GetUIRect()
{
	UIRect temRect;
	temRect.Left = Left;
	temRect.Top = Top;
	temRect.Right = Right;
	temRect.Bottom = Bottom;
	return temRect;
}
// ----------------------------------------------------------------------------
// Name : CUIDrawBox()
// Desc : Constructor
// ----------------------------------------------------------------------------
CUIDrawBox::CUIDrawBox()
{
	// Set Default value
	m_iThickX	= 0;	
	m_iThickY	= 0;	
}

// ----------------------------------------------------------------------------
// Name : ~CUIDrawBox()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUIDrawBox::~CUIDrawBox()
{

}

// ----------------------------------------------------------------------------
// Name : CopyUV()
// Desc :
// ----------------------------------------------------------------------------
void CUIDrawBox::CopyUV(CUIDrawBox* pDrawBox)
{
	SetBoxUV(pDrawBox->m_pTexture,pDrawBox->m_iThickX,pDrawBox->m_iThickY,pDrawBox->m_rcBoxUV);
}

// ----------------------------------------------------------------------------
// Name : SetBoxUV()
// Desc :
// ----------------------------------------------------------------------------
void CUIDrawBox::SetBoxUV(CTextureData* pTexture, int iThick,WRect tRectUV )
{
	m_pTexture	= pTexture;
	m_iThickX	= iThick;
	m_iThickY	= iThick;
	m_rcBoxUV	= tRectUV;

	// Set UV
	int fTexWidth  = m_pTexture->GetPixWidth();
	int fTexHeight = m_pTexture->GetPixHeight();
	int fTx0 = m_rcBoxUV.Left;
	int fTy0 = m_rcBoxUV.Top;
	int fTx1 = m_rcBoxUV.Right;
	int fTy1 = m_rcBoxUV.Bottom;
	
	m_rtCandUL.SetUV( fTx0, fTy0, fTx0 + m_iThickX, fTy0 + m_iThickX, fTexWidth, fTexHeight );
	m_rtCandUM.SetUV( fTx0 + m_iThickX, fTy0, fTx1 - m_iThickX, fTy0 + m_iThickX, fTexWidth, fTexHeight );
	m_rtCandUR.SetUV( fTx1 - m_iThickX, fTy0, fTx1, fTy0 + m_iThickX, fTexWidth, fTexHeight );
	m_rtCandML.SetUV( fTx0, fTy0 + m_iThickX , fTx0 + m_iThickX, fTy1 - m_iThickX, fTexWidth, fTexHeight );
	m_rtCandMM.SetUV( fTx0 + m_iThickX, fTy0 + m_iThickX, fTx1 - m_iThickX, fTy1 - m_iThickX, fTexWidth, fTexHeight );
	m_rtCandMR.SetUV( fTx1 - m_iThickX, fTy0 + m_iThickX, fTx1, fTy1 - m_iThickX, fTexWidth, fTexHeight );
	m_rtCandLL.SetUV( fTx0, fTy1 - m_iThickX, fTx0 + m_iThickX, fTy1, fTexWidth, fTexHeight );
	m_rtCandLM.SetUV( fTx0 + m_iThickX, fTy1 - m_iThickX, fTx1 - m_iThickX, fTy1, fTexWidth, fTexHeight );
	m_rtCandLR.SetUV( fTx1 - m_iThickX, fTy1 - m_iThickX, fTx1, fTy1, fTexWidth, fTexHeight );
}	
// ----------------------------------------------------------------------------
// Name : SetBoxUV()
// Desc :
// ----------------------------------------------------------------------------
void CUIDrawBox::SetBoxUV(CTextureData* pTexture, int iThickX,int iThickY,WRect tRectUV )
{
	m_pTexture	= pTexture;
	m_iThickX	= iThickX;
	m_iThickY	= iThickY;
	m_rcBoxUV	= tRectUV;

	// Set UV
	int fTexWidth  = m_pTexture->GetPixWidth();
	int fTexHeight = m_pTexture->GetPixHeight();
	int fTx0 = m_rcBoxUV.Left;
	int fTy0 = m_rcBoxUV.Top;
	int fTx1 = m_rcBoxUV.Right;
	int fTy1 = m_rcBoxUV.Bottom;
	
	m_rtCandUL.SetUV( fTx0, fTy0, fTx0 + m_iThickX, fTy0 + m_iThickY, fTexWidth, fTexHeight );
	m_rtCandUM.SetUV( fTx0 + m_iThickX, fTy0, fTx1 - m_iThickX, fTy0 + m_iThickY, fTexWidth, fTexHeight );
	m_rtCandUR.SetUV( fTx1 - m_iThickX, fTy0, fTx1, fTy0 + m_iThickY, fTexWidth, fTexHeight );
	m_rtCandML.SetUV( fTx0, fTy0 + m_iThickY , fTx0 + m_iThickX, fTy1 - m_iThickY, fTexWidth, fTexHeight );
	m_rtCandMM.SetUV( fTx0 + m_iThickX, fTy0 + m_iThickY, fTx1 - m_iThickX, fTy1 - m_iThickY, fTexWidth, fTexHeight );
	m_rtCandMR.SetUV( fTx1 - m_iThickX, fTy0 + m_iThickY, fTx1, fTy1 - m_iThickY, fTexWidth, fTexHeight );
	m_rtCandLL.SetUV( fTx0, fTy1 - m_iThickY, fTx0 + m_iThickX, fTy1, fTexWidth, fTexHeight );
	m_rtCandLM.SetUV( fTx0 + m_iThickX, fTy1 - m_iThickY, fTx1 - m_iThickX, fTy1, fTexWidth, fTexHeight );
	m_rtCandLR.SetUV( fTx1 - m_iThickX, fTy1 - m_iThickY, fTx1, fTy1, fTexWidth, fTexHeight );
}	

// ----------------------------------------------------------------------------
// Name : SetBoxPos()
// Desc :
// ----------------------------------------------------------------------------
void CUIDrawBox::SetBoxPos(WRect tRectPos)
{
	m_rcBoxRT	= tRectPos;
}

void CUIDrawBox::SetBoxPos(UIRect tRectPos)
{
	m_rcBoxRT	= tRectPos;
}

// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
void CUIDrawBox::Render(int nX,int nY,const COLOR nCol,FLOAT fDepth)
{
	int nXL = nX + m_rcBoxRT.Left;
	int	nXR = nX + m_rcBoxRT.Right;
	int nYT = nY + m_rcBoxRT.Top;
	int nYB = nY + m_rcBoxRT.Bottom;

	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	// Upper left
	pDrawPort->AddTexture( nXL, nYT,
										nXL + m_iThickX, nYT + m_iThickY,
										m_rtCandUL.U0, m_rtCandUL.V0,
										m_rtCandUL.U1, m_rtCandUL.V1,
										nCol,fDepth);
	if( (m_iThickX) <  m_rcBoxRT.GetWidth()/2 )
	{
		// Upper middle
		pDrawPort->AddTexture( nXL + m_iThickX, nYT,
											nXR - m_iThickX, nYT + m_iThickY,
											m_rtCandUM.U0, m_rtCandUM.V0,
											m_rtCandUM.U1, m_rtCandUM.V1,
											nCol,fDepth );
	}
	if( (m_iThickX) <  m_rcBoxRT.GetWidth() )
	{
		// Upper right
		pDrawPort->AddTexture( nXR - m_iThickX, nYT,
											nXR, nYT + m_iThickY,
											m_rtCandUR.U0, m_rtCandUR.V0,
											m_rtCandUR.U1, m_rtCandUR.V1,
											nCol,fDepth );
	}

	if(  m_iThickY <  m_rcBoxRT.GetHeight()/2 )
	{
		// Middle left
		pDrawPort->AddTexture( nXL, nYT + m_iThickY,
											nXL + m_iThickX, nYB - m_iThickY,
											m_rtCandML.U0, m_rtCandML.V0,
											m_rtCandML.U1, m_rtCandML.V1,
											nCol,fDepth );
		if(  m_iThickX <  m_rcBoxRT.GetWidth()/2 )
		{
			// Middle middle
			pDrawPort->AddTexture( nXL + m_iThickX, nYT + m_iThickY,
												nXR - m_iThickX, nYB - m_iThickY,
												m_rtCandMM.U0, m_rtCandMM.V0,
												m_rtCandMM.U1, m_rtCandMM.V1,
												nCol,fDepth );
		}
		if( m_iThickX <  m_rcBoxRT.GetWidth() )
		{
			// Middle right
			pDrawPort->AddTexture( nXR - m_iThickX, nYT + m_iThickY,
												nXR, nYB - m_iThickY,
												m_rtCandMR.U0, m_rtCandMR.V0,
												m_rtCandMR.U1, m_rtCandMR.V1,
												nCol,fDepth );
		}
	
	}

	if(  m_iThickY <  m_rcBoxRT.GetHeight() )
	{
		// Lower left
		pDrawPort->AddTexture( nXL, nYB - m_iThickY,
											nXL + m_iThickX, nYB,
											m_rtCandLL.U0, m_rtCandLL.V0,
											m_rtCandLL.U1, m_rtCandLL.V1,
											nCol,fDepth );
		if(  m_iThickX <  m_rcBoxRT.GetWidth()/2 )
		{
			// Lower middle
			pDrawPort->AddTexture( nXL + m_iThickX, nYB - m_iThickY,
												nXR - m_iThickX, nYB,
												m_rtCandLM.U0, m_rtCandLM.V0,
												m_rtCandLM.U1, m_rtCandLM.V1,
												nCol,fDepth);
		}
		if( m_iThickX <  m_rcBoxRT.GetWidth() )
		{
			// Lower right
			pDrawPort->AddTexture( nXR - m_iThickX, nYB - m_iThickY,
												nXR, nYB,
												m_rtCandLR.U0, m_rtCandLR.V0,
												m_rtCandLR.U1, m_rtCandLR.V1,
												nCol,fDepth );
		}
	}

	
}
