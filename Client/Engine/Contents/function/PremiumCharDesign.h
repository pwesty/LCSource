#ifndef	__UI_PREMIUMCHAR_DESIGN_H__
#define	__UI_PREMIUMCHAR_DESIGN_H__

#include <Engine/Interface/UIWindow.h>
#include <Engine/Interface/UIInternalClasses.h>

class CPremiumCharDesign : public CUIBase
{
public:
	CPremiumCharDesign();
	
	void	initialize();
	
	
	CUIBase* CloneListPassive()
	{
		if (m_plistCopyPassive == NULL)
			return NULL;

		CUIListItem* pItem = m_plistCopyPassive->GetListItemTemplate();
		if (pItem == NULL)
			return NULL;

		return pItem->Clone();
	}
	CUIBase* CloneListActive()
	{
		if (m_plistCopyActive == NULL)
			return NULL;

		CUIListItem* pItem = m_plistCopyActive->GetListItemTemplate();
		if (pItem == NULL)
			return NULL;

		return pItem->Clone();
	}
	
private:
	CUIList*	m_plistCopyPassive;
	CUIList*	m_plistCopyActive;
};

#endif	// __UI_PREMIUMCHAR_DESIGN_H__

