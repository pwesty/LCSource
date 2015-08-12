#include "StdH.h"
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Interface/UISkillToolTip.h>
#include "UIArray.h"

class CmdOnArraScroll : public Command
{
public:
	CmdOnArraScroll() : array_(NULL) {}

	void setData(CUIArray* parent)	{ array_ = parent; }
	void execute()
	{
		if (array_)
		{
			array_->UpdateItem();
		}
	}

private:
	CUIArray* array_;
};

CUIArray::CUIArray()
	: m_nRow(1)
	, m_nCol(1)
	, m_nStartX(0)
	, m_nStartY(0)
	, m_nGapX(0)
	, m_nGapY(0)
	, m_nShowRow(0)
	, m_bSelected(false)
	, m_nSelectIdx(-1)
	, m_bMouseOver(false)
	, m_nMouseOverIdx(-1)
	, m_pArrayTemp(NULL)
	, m_pMouseEventImage(NULL)
	, m_pScroll(NULL)
{
	setType(eUI_CONTROL_ARRAY);
}

CUIArray::~CUIArray()
{
	Destroy();

	SAFE_DELETE(m_pMouseEventImage);
	SAFE_DELETE(m_pArrayTemp);

	m_vecArrayChild.clear();
}

void CUIArray::deleteChildList()
{
	CUIBase::deleteChildList();
	m_vecArrayChild.clear();
}

CUIBase* CUIArray::Clone()
{
	CUIArray* pArray = NULL;
	pArray = new CUIArray(*this);

	if( pArray == NULL )
		return NULL;

	pArray->setTexString( getTexString() );

	CUIRectSurface* pRS = NULL;
	pRS = GetRectSurface();
	if( pRS != NULL )
	{
		{
			CUIRectSurface* pCopy = new CUIRectSurface;
			pCopy->CopyRectSurface(pRS);

			pArray->SetRectSurface(pCopy);
		}		
	}

	CUIArrayItem* pItem = NULL;
	pItem = (CUIArrayItem*)GetArrayItemTemplate();
	if (pItem != NULL)
		pArray->SetArrayTemplate((CUIArrayItem*)pItem->Clone());

	pArray->deleteChildList();	
	pArray->CmdErase();

	vec_uinode_iter iter = m_VecChild.begin();

	while (iter != m_VecChild.end())
	{
		if ((*iter)->getType() == eUI_CONTROL_ARRAY_ITEM)
		{
			pArray->AddArrayItem( (CUIArrayItem*)(*iter)->Clone() );
		}
		else if ((*iter)->getType() == eUI_CONTROL_SCROLL)
		{
			CUIScrollBar* pScroll = (CUIScrollBar*)(*iter)->Clone();
			pArray->SetScroll(pScroll);
			pArray->addChild((CUIBase*)pScroll);
		}
		else
		{
			pArray->addChild((*iter)->Clone());
		}

		++iter;
	}

	return (CUIBase*)pArray;
}

void CUIArray::ResetArray()
{
	// ArrayItem 만 지운다.
	vec_uinode_iter		iter = m_VecChild.begin();

	for(; iter != m_VecChild.end();)
	{
		if ((*iter)->getType() == eUI_CONTROL_ARRAY_ITEM)
		{			
			SAFE_DELETE( (*iter) );

			iter = m_VecChild.erase(iter);
		}
		else
		{
			++iter;
		}
	}

	m_vecArrayChild.clear();
}

void CUIArray::SetScroll( CUIScrollBar* pScroll )
{
	if (pScroll != NULL)
	{
		m_pScroll = pScroll;

		CmdOnArraScroll* pCmd = new CmdOnArraScroll;
		pCmd->setData(this);
		m_pScroll->SetCommand(pCmd);
	}
}

void CUIArray::AddArrayItem( CUIArrayItem* pItem )
{
	if( pItem == NULL )
		return;

	char buf[64];
	sprintf(buf, "array_item_%03d", m_vecArrayChild.size() + 1);
	pItem->setID(buf);

	m_vecArrayChild.push_back( pItem );
	addChild( pItem );

	if( m_pScroll != NULL )
	{
		int nMax = m_vecArrayChild.size() / m_nCol;
		int nDiv = (m_vecArrayChild.size() % m_nCol);
		if (nDiv != 0)
			nMax++;

		m_pScroll->SetCurItemCount(nMax);
	}

	UpdateItem();
}

void CUIArray::UpdateItem()
{
	int x, y;

	int i, max = m_vecArrayChild.size();

	int nStartRow = 0;
	int nCurRow = 0;

	if( m_pScroll )
		nStartRow = m_pScroll->GetScrollPos();

	CUIArrayItem* pItem;

	for( i = 0 ; i < max; i++ )
	{
		pItem = m_vecArrayChild[i];
		nCurRow = i / m_nCol;

		if( pItem == NULL )
			continue;

		if( nCurRow < nStartRow )
		{
			pItem->Hide(TRUE);
			continue;
		}

		if( (nCurRow - nStartRow) < m_nShowRow )
			pItem->Hide(FALSE);
		else
			pItem->Hide(TRUE);
	
		x = m_nStartX + ( (i % m_nCol) * (m_nGapX + pItem->GetWidth()) );
		y = m_nStartY + ( ((i / m_nCol) - nStartRow) * (m_nGapY + pItem->GetHeight()) );

		pItem->SetPos(x, y);
	}
}

void CUIArray::OnPostRender( CDrawPort* pDraw )
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
	
	if( m_pMouseEventImage == NULL || m_pMouseEventImage->m_RectSurfaceArray.Count() <= 0)
		return;

	int		nX, nY;

	if (m_bSelected == true)
	{
		GetAbsPos( nX, nY );
		nX += m_ptSelectOffSet.x;
		nY += m_ptSelectOffSet.y;

		m_pMouseEventImage->SetPos(nX, nY);
		m_pMouseEventImage->RenderRectSurface( pDraw, DEF_UI_COLOR_WHITE, 0 );
	}
	if( m_pMouseEventImage->m_RectSurfaceArray.Count() >= 1 && m_bMouseOver == true)
	{
		GetAbsPos( nX, nY );
		nX += m_ptMouseOverOffset.x;
		nY += m_ptMouseOverOffset.y;

		m_pMouseEventImage->SetPos(nX, nY);
		m_pMouseEventImage->RenderRectSurface( pDraw, DEF_UI_COLOR_WHITE, 1 );
	}

	pDraw->FlushRenderingQueue();
#ifdef UI_TOOL
	RenderBorder(pDraw);
#endif // UI_TOOL

}

CUIArrayItem* CUIArray::GetArrayItem( int idx )
{
	if( idx < 0 || idx >= m_vecArrayChild.size() )
		return NULL;

	return m_vecArrayChild[idx];
}

WMSG_RESULT CUIArray::OnLButtonDown(UINT16 x, UINT16 y)
{
	SetSelectIdx(-1);
	if (m_bHide)
		return WMSG_FAIL;

	if (IsInside(x, y) == FALSE)
	{
		m_bSelected = false;
		return WMSG_FAIL;
	}

	int		i, nMax;

	nMax = m_vecArrayChild.size();
	for (i = 0; i < nMax; ++i)
	{
		if (m_vecArrayChild[i]->IsInside(x, y) == TRUE)
		{
			SetSelectIdx(i);

			if (m_pCmd)
				m_pCmd->execute();

			return WMSG_FAIL;
		}
	}

	if (m_pCmd)
		m_pCmd->execute();

	return WMSG_FAIL;
}
WMSG_RESULT CUIArray::OnLButtonUp(UINT16 x, UINT16 y)
{
	if (m_bHide)
		return WMSG_FAIL;

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	return WMSG_FAIL;
}

WMSG_RESULT CUIArray::OnMouseMove( UINT16 x, UINT16 y, MSG* pMsg )
{
	if (m_bHide)
		return WMSG_FAIL;

	if (IsInside(x, y) == FALSE)
	{
		m_bMouseOver = false;
		return WMSG_FAIL;
	}

	HitTest(x, y);

	return WMSG_FAIL;
}

int CUIArray::HitTest( int x, int y )
{
	int		i, nMax;

	nMax = m_vecArrayChild.size();
	for (i = 0; i < nMax; ++i)
	{
		if (m_vecArrayChild[i]->IsInside(x, y) == TRUE)
		{
			SetMouseOverItem(i);

			return i;
		}
	}
	return -1;
}

void CUIArray::SetSelectIdx( int idx )
{
	m_nSelectIdx = idx;

	if (idx < 0 || idx >= m_vecArrayChild.size())
	{
		m_bSelected = false;
		return;
	}

	if( m_vecArrayChild[idx]->GetHide() == TRUE )
	{
		m_bSelected = false;
		return;
	}

	m_bSelected = true;

	m_ptSelectOffSet.x = m_vecArrayChild[idx]->GetPosX();
	m_ptSelectOffSet.y = m_vecArrayChild[idx]->GetPosY();
}

void CUIArray::SetMouseEventImage( UIRect rc, UIRectUV uv, int type )
{
	if( m_pMouseEventImage == NULL )
		m_pMouseEventImage = new CUIRectSurface;

#ifdef UI_TOOL
	m_rcMouseEventImage[type] = rc;
	m_uvMouseEventImage[type] = uv;
#endif // UI_TOOL

	UISUPPORT()->DivideTexUV(m_pTexData, uv);

	m_pMouseEventImage->AddRectSurface(rc, uv);
}

void CUIArray::SetMouseOverItem( int idx )
{
	m_nMouseOverIdx = idx;

	if (idx < 0 || idx >= m_vecArrayChild.size())
	{
		m_bMouseOver = false;
		return;
	}

	if( m_vecArrayChild[idx]->GetHide() == TRUE )
	{
		m_bMouseOver = false;
		return;
	}

	m_bMouseOver = true;

	m_ptMouseOverOffset.x = m_vecArrayChild[idx]->GetPosX();
	m_ptMouseOverOffset.y = m_vecArrayChild[idx]->GetPosY();
}

