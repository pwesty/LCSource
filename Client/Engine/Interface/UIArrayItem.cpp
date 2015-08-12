#include "StdH.h"
#include <Engine/Interface/UIInternalClasses.h>
#include "UIArrayItem.h"

CUIArrayItem::CUIArrayItem(void)
{
	setType(eUI_CONTROL_ARRAY_ITEM);
}

CUIBase* CUIArrayItem::Clone()
{
	CUIArrayItem*	pItem = NULL;

	pItem = new CUIArrayItem(*this);

	if( !pItem )
		return NULL;

	CUIBase::CloneChild(pItem);

	return (CUIBase*)pItem;

}

void CUIArrayItem::OnRender( CDrawPort* pDraw )
{
#ifdef UI_TOOL
	RenderBorder(pDraw);
#endif // UI_TOOL
}

void CUIArrayItem::Hide( BOOL bHide )
{
	CUIBase::Hide(bHide);

	m_bEnter = false;
	
	OnLeave(0, 0);
}
