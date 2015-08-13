#include "StdH.h"
#include <Engine/Interface/UIInternalClasses.h>
#include "UIList.h"

//-----------------------------------------------------------------

class CmdOnChangeScroll : public Command
{
public:
	CmdOnChangeScroll() : list_(NULL) {}

	void setData(CUIList* parent)	{ list_ = parent; }
	void execute()
	{
		if (list_)
		{
			list_->UpdateList();
		}
	}

private:
	CUIList* list_;
};

CUIList::CUIList() 
	: m_nNumItems(0)
	, m_nGap(0)		
	, m_nNumShow(0)
	, m_nItemStartIndex(0)
	, m_nItemStartX(0)
	, m_nItemStartY(0)
	, m_nCurSel(-1)
	, m_pListItemTemplate(NULL)
	, m_nCurStart(0)
	, m_pScroll(NULL)
	, m_bPreCreate(false)
	, m_bSelected(false)
	, m_bMouseOver(false)
	, m_bUseSelect(false)
	, m_nItemCount(0)
	, m_pimgEvent(NULL)
	, m_pCmdUnSelect(NULL)
	, m_nItemTotalHeight(0)
	, m_bAlignTop(true)
{
	setType(eUI_CONTROL_LIST);
}

CUIList::~CUIList()
{
	Destroy();

	SAFE_DELETE(m_pListItemTemplate);
	SAFE_DELETE(m_pimgEvent);

	m_vecListArray.clear();

	SAFE_DELETE(m_pCmdUnSelect);
}

void CUIList::deleteChildList()
{
	m_VecChild.clear();
	m_vecListArray.clear();
}

void CUIList::deleteListItem( int nPos )
{
	int iMax = m_VecChild.size();
	if( nPos >= iMax || nPos < 0 )
		return;

	vec_uinode_iter iter = m_vecListArray.begin();

	CUIBase* pDel = (*(iter + nPos));

	deleteChild(pDel);

	m_vecListArray.erase(iter + nPos);
}

CUIBase* CUIList::Clone()
{
	CUIList* pList = NULL;
	pList= new CUIList(*this);

	if( pList == NULL )
		return NULL;

	pList->setTexString( getTexString() );

	CUIRectSurface* pRS = NULL;
	pRS = GetRectSurface();
	if( pRS != NULL )
	{
		{
			CUIRectSurface* pCopy = new CUIRectSurface;
			pCopy->CopyRectSurface(pRS);

			pList->SetRectSurface(pCopy);
		}		
	}

	CUIListItem* pItem = NULL;
	pItem = (CUIListItem*)GetListItemTemplate();
	if (pItem != NULL)
		pList->SetListItemTemplate((CUIListItem*)pItem->Clone());

	pList->deleteChildList();
	pList->CmdErase();

	vec_uinode_iter iter = m_VecChild.begin();

	while (iter != m_VecChild.end())
	{
		if ((*iter)->getType() == eUI_CONTROL_LIST_ITEM)
		{
			pList->AddListItem( (CUIListItem*)(*iter)->Clone() );
		}
		else if ((*iter)->getType() == eUI_CONTROL_SCROLL)
		{
			CUIScrollBar* pScroll = (CUIScrollBar*)(*iter)->Clone();
			pList->setScroll(pScroll);
			pList->addChild((CUIBase*)pScroll);
		}
		else
		{
			pList->addChild((*iter)->Clone());
		}

		++iter;
	}

	return (CUIBase*)pList;
}


void CUIList::SetItemStart( int x, int y )
{
	m_nItemStartX = x;  
	m_nItemStartY = y;
}

void CUIList::AddListItem( CUIBase* pItem )
{
	if (pItem == NULL)
		return;

	// 기본 아이디 지정
	char buf[64];
	int		idx = m_vecListArray.size();
	sprintf(buf, "list_item_%03d", idx + 1);
	pItem->setID(buf);
	pItem->setControlIndex(idx);

	m_vecListArray.push_back( pItem );
	addChild( pItem );
}

CUIBase* CUIList::GetListItem( int idx )
{
	if (idx < 0 || idx >= m_vecListArray.size())
		return NULL;

	return m_vecListArray[idx];
}

void CUIList::UpdateList()
{
	if (m_bAlignTop == true)
		UpdateListTop();
	else
		UpdateListBottom();
}

void CUIList::UpdateListTop()
{
	int		x = m_nItemStartX;
	int		y = m_nItemStartY;

	int		i, nMax = m_vecListArray.size();
	CUIBase* pItem;
	int		nListHeight = GetHeight();
	int		nMaxHeight = 0;
	m_nItemTotalHeight = 0;

	if (m_pScroll != NULL)
	{
		m_nCurStart = m_pScroll->GetScrollPos();
	}

	for (i = 0; i < nMax; ++i)
	{
		pItem = m_vecListArray[i];

		if (pItem == NULL)
			continue;

		if (m_nCurStart > i)
		{
			pItem->Hide(TRUE);

			if (m_nCurSel < i)	// 선택된 아이템 초기화.
				setCurSel(-1);

			continue;
		}

		pItem->SetPos(x, y);
		int nAddHeight = pItem->GetHeight() + m_nGap;

		y = pItem->GetPosY() + nAddHeight;

		if (/*(i - m_nCurStart) < m_nNumShow &&*/y <= nListHeight + m_nGap)
		{
			pItem->Hide(FALSE);
		}
		else
		{
			pItem->Hide(TRUE);

			if (m_nCurSel <= i)	// 선택된 아이템 초기화.
				setCurSel(-1);
		}

		if (nAddHeight > nMaxHeight)
			nMaxHeight = nAddHeight;

		m_nItemTotalHeight += nAddHeight;
	}

	if (nMaxHeight > 0)
	{
		float fDiv = (float)((float)GetHeight() / (float)(nMaxHeight + m_nGap));
		m_nNumShow = fDiv;
	}
}

void CUIList::UpdateListBottom()
{
	if (m_pListItemTemplate == NULL)
		return;

	if (m_pScroll != NULL)
	{
		m_nCurStart = m_pScroll->GetScrollPos();
	}

	CUIBase* pItem = NULL;
	int nPosY = m_nHeight - m_nItemStartY;
	int nPosX = m_nItemStartX;

	m_nNumShow = (float)((float)nPosY / (float)(m_pListItemTemplate->GetHeight() + m_nGap));

	int nStartIndex = m_nCurStart + m_nNumShow;
	int nItemCount = m_vecListArray.size();

	if (nStartIndex > nItemCount)
		nStartIndex = nItemCount;

	for (int i = nItemCount; i > 0; --i)
	{
		pItem = m_vecListArray[i - 1];

		if (pItem == NULL)
			continue;

		if (i > nStartIndex)
		{
			pItem->Hide(TRUE);
		}
		else
		{
			pItem->SetPos(nPosX, nPosY - (pItem->GetHeight() + m_nGap));
			nPosY = pItem->GetPosY();
			
			if (nPosY >= m_nPosY)
			{
				pItem->Hide(FALSE);
			}
			else
			{
				pItem->Hide(TRUE);
	
				if (m_nCurSel <= i)	// 선택된 아이템 초기화.
					setCurSel(-1);
			}
		}
	}
}

void CUIList::UpdateHeight( int nHeight )
{
	int nPrevH = m_nHeight;

	SetHeight(nHeight);

	if (m_pScroll == NULL)
		return;

	if (m_pListItemTemplate != NULL)
	{
		int nPosY = m_nHeight - m_nItemStartY;
		m_nNumShow = (float)((float)nPosY / (float)(m_pListItemTemplate->GetHeight() + m_nGap));
	}

	int nScrollHeight = m_pScroll->GetHeight() + (nHeight - nPrevH);
	m_pScroll->UpdateSize(nScrollHeight, m_nNumShow);
	m_pScroll->SetScrollCurPos(m_vecListArray.size() - m_nNumShow);

	UpdateList();
}

void CUIList::UpdateScroll( int nItemCount )
{
	if (m_pScroll == NULL)
		return;

	m_pScroll->SetCurItemCount(nItemCount);
}

//--------------------------------------------------------------

void CUIList::OnPostRender( CDrawPort* pDraw )
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

	if( m_pimgEvent == NULL || m_pimgEvent->m_RectSurfaceArray.Count() <= 0)
		return;

	int		nX, nY;

	if (m_bSelected == true)
	{
		GetAbsPos( nX, nY );
		nX += m_ptSelOffset.x;
		nY += m_ptSelOffset.y;

		m_pimgEvent->SetPos(nX, nY);
		m_pimgEvent->RenderRectSurface( pDraw, DEF_UI_COLOR_WHITE, eTYPE_SELECT );
	}
	if( m_pimgEvent->m_RectSurfaceArray.Count() > 1 && m_bMouseOver == true)
	{
		GetAbsPos( nX, nY );
		nX += m_ptMouseOverOffset.x;
		nY += m_ptMouseOverOffset.y;

		m_pimgEvent->SetPos(nX, nY);
		m_pimgEvent->RenderRectSurface( pDraw, DEF_UI_COLOR_WHITE, eTYPE_OVER );
	}

	pDraw->FlushRenderingQueue();

#ifdef UI_TOOL
	RenderBorder(pDraw);
#endif // UI_TOOL
}

void CUIList::setScroll( CUIScrollBar* pScroll )
{
	if (pScroll != NULL)
	{
		m_pScroll = pScroll;

		CmdOnChangeScroll* pCmd = new CmdOnChangeScroll;
		pCmd->setData(this);
		m_pScroll->SetCommand(pCmd);

		int nMax = m_vecListArray.size();
		m_pScroll->SetCurItemCount(nMax);
		m_pScroll->SetItemsPerPage(m_nNumShow);
	}
}

void CUIList::addEventImage( UIRect rect, UIRectUV uv, int type )
{
	if( m_pimgEvent == NULL )
		m_pimgEvent = new CUIRectSurface;

#ifdef UI_TOOL
	m_rcEventImg[type] = rect;
	m_uvEventImg[type] = uv;
#endif // UI_TOOL
	
	UISUPPORT()->DivideTexUV(m_pTexData, uv);

	m_pimgEvent->AddRectSurface(rect, uv);

	// 이미지를 초기화 해야 사용으로 간주
	//m_bUseSelect = true;
}

void CUIList::setCurSel( int idx )
{
	if (idx < 0 || idx >= m_vecListArray.size())
	{
		m_bSelected = false;
		m_nCurSel = idx;
		return;
	}

	if (m_nCurSel >= 0 && m_nCurSel < m_vecListArray.size())
	{
		if (m_pCmdUnSelect != NULL)
			m_pCmdUnSelect->execute();
	}

	m_bSelected = true;
	m_nCurSel = idx;

	m_ptSelOffset.x = m_vecListArray[idx]->GetPosX();
	m_ptSelOffset.y = m_vecListArray[idx]->GetPosY();
}

WMSG_RESULT CUIList::OnLButtonDown(UINT16 x, UINT16 y)
{
	if (m_bHide == TRUE)
		return WMSG_FAIL;

	if (IsInside(x, y) == FALSE)
	{		
		return WMSG_FAIL;
	}

	int		i, nMax;

	nMax = m_vecListArray.size();
	for (i = 0; i < nMax; ++i)
	{
		if (m_vecListArray[i]->GetHide() == FALSE && 
			m_vecListArray[i]->IsInside(x, y) == TRUE)
		{
			setCurSel(i);
			//return WMSG_SUCCESS;
			// 메세지를 무시해야 Child 에 포커스가 간다.
			if (m_pCmd)
				m_pCmd->execute();

			return WMSG_FAIL;
		}
	}

	// 선택이 되지 않으면, 선택 안된 상태로 만든다.
	setCurSel(-1);

	if (m_pCmd)
		m_pCmd->execute();

	return WMSG_FAIL;
}

bool CUIList::DeleteAllListItem()
{
	vec_listitem_iter		iter = m_vecListArray.begin();
	vec_listitem_iter		eiter = m_vecListArray.end();

	for(; iter != eiter; ++iter )
	{
		deleteChild( (*iter) );
	}

	m_vecListArray.clear();

	return true;
}

WMSG_RESULT CUIList::OnLButtonDBLClick( UINT16 x, UINT16 y )
{
	if (m_bHide == TRUE)
		return WMSG_FAIL;

	if (IsInside(x, y) == FALSE)
	{		
		return WMSG_FAIL;
	}

	int		i, nMax;

	nMax = m_vecListArray.size();
	for (i = 0; i < nMax; ++i)
	{
		if (m_vecListArray[i]->GetHide() == FALSE && 
			m_vecListArray[i]->IsInside(x, y) == TRUE)
		{
			setCurSel(i);

			if (m_pCmdDBL)
				m_pCmdDBL->execute();

			return WMSG_FAIL;
		}
	}

	// 선택이 되지 않으면, 선택 안된 상태로 만든다.
	setCurSel(-1);

	if (m_pCmdDBL)
		m_pCmdDBL->execute();

	return WMSG_FAIL;
}

WMSG_RESULT CUIList::OnMouseMove( UINT16 x, UINT16 y, MSG* pMsg )
{
	if (m_bHide)
		return WMSG_FAIL;

	if (IsInside(x, y) == FALSE)
	{
		m_bMouseOver = false;
		return WMSG_FAIL;
	}

	int idx = HitTest(x, y);
	setMouseOverItem(idx);

	return WMSG_FAIL;
}

int CUIList::HitTest( UINT16 x, UINT16 y )
{
	int		i, nMax;

	nMax = m_vecListArray.size();
	for (i = 0; i < nMax; ++i)
	{
		if (m_vecListArray[i]->GetHide() == FALSE &&
			m_vecListArray[i]->IsInside(x, y) == TRUE)
			return i;
	}
	return -1;
}

void CUIList::setMouseOverItem( int idx )
{
	m_nMouseOverItem = idx;

	if (idx < 0 || idx >= m_vecListArray.size())
	{
		m_bMouseOver = false;
		return;
	}

	if( m_vecListArray[idx]->GetHide() == TRUE )
	{
		m_bMouseOver = false;
		return;
	}

	m_bMouseOver = true;

	m_ptMouseOverOffset.x = m_vecListArray[idx]->GetPosX();
	m_ptMouseOverOffset.y = m_vecListArray[idx]->GetPosY();
}
