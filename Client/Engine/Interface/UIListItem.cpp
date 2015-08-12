#include "StdH.h"
#include "UIListItem.h"
#include <Engine/Interface/UIManager.h>


CUIListItem::CUIListItem() 
	: m_nIdx(-1)
	, m_pCmdDrag(NULL)
{
	setType(eUI_CONTROL_LIST_ITEM);
}

CUIListItem::~CUIListItem()
{
	Destroy();
	SAFE_DELETE(m_pCmdDrag);
}

CUIBase* CUIListItem::Clone()
{
	CUIListItem*	pItem = NULL;

	pItem = new CUIListItem(*this);

	CUIBase::CloneChild(pItem);

	return (CUIBase*)pItem;
}

//---------------------------------------------------------------------------

void CUIListItem::OnRender( CDrawPort* pDraw )
{
#ifdef	UI_TOOL
	RenderBorder(pDraw);
#endif	// UI_TOOL
}

WMSG_RESULT CUIListItem::OnLButtonDown( UINT16 x, UINT16 y )
{
	if (m_bHide == TRUE)
		return WMSG_FAIL;

	if (IsInside(x, y) == FALSE)
	{		
		return WMSG_FAIL;
	}

	if (m_pCmd)
		m_pCmd->execute();

	return WMSG_FAIL;
}

WMSG_RESULT CUIListItem::OnLButtonDBLClick( UINT16 x, UINT16 y )
{
	if (m_bHide == TRUE)
		return WMSG_FAIL;

	if (IsInside(x, y) == FALSE)
	{		
		return WMSG_FAIL;
	}

	if (m_pCmdDBL)
		m_pCmdDBL->execute();

	return WMSG_FAIL;
}

WMSG_RESULT CUIListItem::OnLButtonUp( UINT16 x, UINT16 y )
{
	if (m_bHide == TRUE)
		return WMSG_FAIL;

	if (IsInside(x, y) == FALSE)
	{		
		return WMSG_FAIL;
	}

	if (m_pCmdUp)
		m_pCmdUp->execute();

	return WMSG_FAIL;
}

WMSG_RESULT CUIListItem::OnRButtonDown( UINT16 x, UINT16 y )
{
	if (m_bHide == TRUE)
		return WMSG_FAIL;

	if (IsInside(x, y) == FALSE)
	{		
		return WMSG_FAIL;
	}

	if (m_pCmdR)
		m_pCmdR->execute();

	return WMSG_FAIL;
}

WMSG_RESULT CUIListItem::OnMouseMove( UINT16 x, UINT16 y, MSG* pMsg )
{
	if (m_bHide == TRUE)
		return WMSG_FAIL;

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	if (pMsg->wParam & MK_LBUTTON && CUIManager::getSingleton()->GetDragBase() == NULL)
	{
		if (m_pCmdDrag)
			m_pCmdDrag->execute();
	}

	return WMSG_FAIL;
}

