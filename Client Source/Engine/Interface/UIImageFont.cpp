#include "StdH.h"
#include <Engine/Interface/UIInternalClasses.h>
#include "UIImageFont.h"
#include <Engine/Help/UISupport.h>


CUIImageFont::CUIImageFont()
:	 m_strSource("")
	,m_prsSource(NULL)
	,m_strText("")
	,m_nFontSX(0)
	,m_nFontSY(0)
	,m_nFontGap(0)
	,m_nOutGap(0)
{		
	m_eType = eUI_CONTROL_IMAGEFONT;
}


CUIImageFont::~CUIImageFont()
{
	Destroy();

	SAFE_DELETE(m_prsSource);
}

CUIBase* CUIImageFont::Clone()
{
	CUIImageFont* pClone = new CUIImageFont(*this);

	pClone->setTexString( getTexString() );

	CUIRectSurface* pRS = NULL;
	pRS = GetRectSurface();
	if( pRS != NULL )
	{
		{
			CUIRectSurface* pCopy = new CUIRectSurface;
			pCopy->CopyRectSurface(pRS);

			pClone->SetRectSurface(pCopy);
		}		
	}

	CUIBase::CloneChild(pClone);

	return pClone;
}

void CUIImageFont::setSourceImage(UIRectUV uv )
{
	m_uvSource = uv;

	calcSource();
}

void CUIImageFont::calcSource()
{
	if (m_strSource.empty() == true)
		return;

	if( m_prsSource == NULL )
		m_prsSource = new CUIRectSurface;

	int		i, nMax = m_strSource.size();
	UIRect	rc(0, 0, m_nFontSX, m_nFontSY);
	UIRectUV uv;

	m_prsSource->Clear();

	for (i = 0; i < nMax; ++i)
	{
// 		rc = m_rcSource;
// 		rc.Left = m_rcSource.Left + ((m_nFontSize + m_nFontGap) * i);		
// 		rc.Right = rc.Left + m_nFontSize;

		uv = m_uvSource;
		uv.U0 += ((m_nFontSX + m_nFontGap) * i);
		uv.U1 = uv.U0 + m_nFontSX;

		UISUPPORT()->DivideTexUV(m_pTexData, uv);

		m_prsSource->AddRectSurface(rc, uv);
	}	
}

//-------------------------------------------------------------------

void CUIImageFont::OnRender( CDrawPort* pDraw )
{
	if( m_bHide == TRUE )
		return;

	if( m_pTexData == NULL )
	{
#ifdef UI_TOOL
		RenderBorder(pDraw);
#endif // UI_TOOL
		return;
	}

	if (m_prsSource == NULL)
		return;

	int i, nSize = m_strText.size();

	if (nSize == 0)
		return;

	pDraw->InitTextureData( m_pTexData );

	int nX, nY, dX, nIdx;
	GetAbsPos(nX, nY);

	for (i = 0; i < nSize; ++i)
	{
		dX = nX + ((m_nFontSX + m_nFontGap) * i);

		if (i > 0)
			dX += (m_nOutGap * i);

		nIdx = m_strSource.find(m_strText.at(i));

		if (nIdx < 0)
			continue;

		if( m_prsSource->m_RectSurfaceArray.Count() <= nIdx )
			continue;

		m_prsSource->SetPos(dX, nY);
		m_prsSource->RenderRectSurface(pDraw, DEF_UI_COLOR_WHITE, nIdx);
	}
	
	pDraw->FlushRenderingQueue();
#ifdef UI_TOOL
	RenderBorder(pDraw);
#endif // UI_TOOL
}
