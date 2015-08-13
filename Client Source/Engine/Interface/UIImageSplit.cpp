
#include "StdH.h"
#include "UIImageSplit.h"
#include "UIManager.h"
#include <Engine/Help/UISupport.h>

//------------------------------------------------------------------------

CUIImageSplit::CUIImageSplit()
	: _pSurface(NULL)
	, _nUnit(0)
	, _eSplitType(SPLIT_3H)
{
	setType(eUI_CONTROL_IMAGE_SPLIT);
}

CUIImageSplit::~CUIImageSplit()
{
	SAFE_DELETE(_pSurface);
	Destroy();
}

//------------------------------------------------------------------------

CUIBase* CUIImageSplit::Clone()
{
	CUIImageSplit* pImg = new CUIImageSplit(*this);

	pImg->setTexString( getTexString() );
	pImg->SetSplitMode( (eSPLIT_TYPE)GetMode() );
	pImg->SetUnit( GetUnit() );

	CUIRectSurface* pRS = NULL;
	pRS = GetSurface();
	if( pRS != NULL )
	{
		{
			CUIRectSurface* pCopy = new CUIRectSurface;
			pCopy->CopyRectSurface(pRS);
			pImg->SetSurface(pCopy);
		}		
	}

	CUIBase::CloneChild(pImg);

	return pImg;
}


void CUIImageSplit::OnRender( CDrawPort* pDraw )
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

	pDraw->InitTextureData( m_pTexData );

	int	nX, nY;
	GetAbsPos( nX, nY );

	if (_pSurface != NULL)
	{
		_pSurface->SetPos(nX, nY);
		_pSurface->RenderRectSurface(pDraw, m_color);
	}

	pDraw->FlushRenderingQueue();
#ifdef UI_TOOL
	RenderBorder(pDraw);
#endif // UI_TOOL
}

void CUIImageSplit::UpdateSplit()
{
	if ( _nUnit <= 0)
		return;

	if (_pSurface == NULL)
		_pSurface = new CUIRectSurface;

	UIRect rc(0, 0, GetWidth(), GetHeight());

	switch(_eSplitType)
	{
	case SPLIT_3H:
		UISUPPORT()->Split3PlaneHorizontal((*_pSurface), m_pTexData, rc , m_absUV, _nUnit);
		break;
	case SPLIT_3V:
		UISUPPORT()->Split3PlaneVertical((*_pSurface), m_pTexData, rc , m_absUV, _nUnit);
		break;
	case SPLIT_9P:
		UISUPPORT()->Split9Plane((*_pSurface), m_pTexData, rc , m_absUV, _nUnit, _nUnit);
		break;
	}	
}

