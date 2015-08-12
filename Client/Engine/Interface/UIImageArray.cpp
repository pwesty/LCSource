
#include "StdH.h"
#include <Engine/Interface/UIInternalClasses.h>
#include "UIImageArray.h"

CUIImageArray::CUIImageArray()
	: m_nRenderIdx(0)
	, m_color(DEF_UI_COLOR_WHITE)
	, m_fDepth(0.f)
	, m_bDepthMode(FALSE)
{
	setType(eUI_CONTROL_IMAGE_ARRAY);
	Clear();
}

CUIImageArray::~CUIImageArray()
{
	Destroy();
	Clear();
}

CUIBase* CUIImageArray::Clone()
{
	CUIImageArray* pImageArray = NULL;

	pImageArray = new CUIImageArray(*this);

	if (pImageArray == NULL)
		return NULL;

	pImageArray->setTexString( getTexString() );

	CUIBase::CloneChild(pImageArray);

	return (CUIBase*)pImageArray;
}

void CUIImageArray::initialize()
{

}

void CUIImageArray::OnRender( CDrawPort* pDraw )
{
	if (m_bHide == TRUE)
		return;

	if (m_pTexData == NULL)
	{
#ifdef UI_TOOL
		RenderBorder(pDraw);
#endif // UI_TOOL
		return;
	}

	if (m_bDepthMode == TRUE)
		pDraw->InitTextureData( m_pTexData, FALSE, PBT_BLEND, TRUE );
	else
		pDraw->InitTextureData( m_pTexData);

	if (m_nRenderIdx >= 0 && m_nRenderIdx < m_vecData.size())
	{
		UIRect rc = m_vecData[m_nRenderIdx].rc;
		UIRectUV uv = m_vecData[m_nRenderIdx].uv;

		int	nX, nY;
		GetAbsPos( nX, nY );

		pDraw->AddTexture( nX + rc.Left, nY + rc.Top, nX + rc.Left + rc.Right, nY + rc.Top + rc.Bottom, 
			uv.U0, uv.V0, uv.U1, uv.V1, m_color, m_fDepth );

		pDraw->FlushRenderingQueue();
	}

#ifdef UI_TOOL
		RenderBorder(pDraw);
#endif // UI_TOOL
}

bool CUIImageArray::RemoveImage( int idx )
{
	if (idx < 0 || idx > m_vecData.size())
		return false;

	vecIter bIt = m_vecData.begin();

	m_vecData.erase(bIt + idx);

	return true;
}

void CUIImageArray::AddImage( UIRect rc, UIRectUV uv, int idx /* = -1 */, std::string desc /* = empty*/)
{	
	UISUPPORT()->DivideTexUV(m_pTexData, uv);

	if (idx >= 0 && idx < m_vecData.size())
	{
		m_vecData[idx].rc = rc;
		m_vecData[idx].uv = uv;
#ifdef UI_TOOL
		m_vecData[idx].desc = desc;
#endif // UI_TOOL
	}
	else
	{
		stData	data;		
		data.rc = rc;
		data.uv = uv;
#ifdef UI_TOOL
		data.desc = desc;
#endif // UI_TOOL

		m_vecData.push_back(data);
	}
}

void CUIImageArray::DataAt( int idx, UIRect& rc, UIRectUV& uv, std::string& desc )
{
	if (idx < 0 || idx > m_vecData.size())
		return;

	rc = m_vecData[idx].rc;
	uv = m_vecData[idx].uv;

#ifdef UI_TOOL
	desc = m_vecData[idx].desc;
#endif // UI_TOOL

	if (m_pTexData != NULL)
	{
		FLOAT fW = m_pTexData->GetPixWidth();
		FLOAT fH = m_pTexData->GetPixHeight();

		uv.U0 *= fW;		uv.V0 *= fH;
		uv.U1 *= fW;		uv.V1 *= fH;
	}
}

void CUIImageArray::SetImageWidth( int idx, int nWidth )
{
	if (idx < 0 || idx > m_vecData.size())
		return;

	m_vecData[idx].rc.Right = nWidth;
}
