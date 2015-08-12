#ifndef	__UI_QUEST_DESIGN_H__
#define	__UI_QUEST_DESIGN_H__

enum eICON_TYPE
{
	eQUEST_TYPE1 = 0,
	eQUEST_TYPE2,
	eQUEST_SCALE,
	eICON_MAX
};

enum eELEMENT_POS
{
	eELEMENT_SUMMURY = 2,
	eELEMENT_TITLE = 32,
	eELEMENT_DESC = 25,
};

const int nType1StringIdx[12] = { 7005, 7006, 7007, 7008, 7009, 7010, 7011, 7012, 7013, 7014, 7015, 7016 };
const int nType2StringIdx[3] = { 7017, 7018, 7019 };
const int nScaleStringIdx[3] = { 7020, 7021, 7022 };

#include <Engine/Interface/UIWindow.h>
#include <Engine/Contents/Base/Quest.h>
#include <Engine/Entities/QuestSystem.h>
#include <Engine/Interface/UIInternalClasses.h>

class CUIQuestDesign : public CUIBase
{
public:
	CUIQuestDesign();
	
	void	initialize();
	
	CUIBase* CloneTitle()
	{
		if (m_plistCopyTitle == NULL)
			return NULL;
		CUIListItem* pItem = m_plistCopyTitle->GetListItemTemplate();
		if (pItem == NULL)
			return NULL;

		return pItem->Clone();
	}

	CUIBase* CloneSummury()
	{
		if (m_ptbSummury == NULL)
			return NULL;
		return m_ptbSummury->Clone();
	}

	CUIBase* CloneDesc()
	{
		if (m_ptbDesc == NULL)
			return NULL;
		return m_ptbDesc->Clone();
	}

	CUIBase* CloneCount()
	{
		if (m_pstrCount == NULL)
			return NULL;
		return m_pstrCount->Clone();
	}

	CUIBase* CloneRestoreNpc()
	{
		if (m_pstrRestoreNpc == NULL)
			return NULL;
		return m_pstrRestoreNpc->Clone();
	}

	CUIBase* CloneRestoreQuest()
	{
		if (m_ptbRestoreName == NULL)
			return NULL;
		return m_ptbRestoreName->Clone();
	}
	CUIBase* CloneListItem()
	{
		if (m_plistCopyItem == NULL)
			return NULL;

		CUIListItem* pItem = m_plistCopyItem->GetListItemTemplate();
		if (pItem == NULL)
			return NULL;

		return pItem->Clone();
	}
	CUIBase* CloneListViewItem()
	{
		if (m_plistCopyView == NULL)
			return NULL;

		CUIListItem* pItem = m_plistCopyView->GetListItemTemplate();
		if (pItem == NULL)
			return NULL;

		return pItem->Clone();
	}
	CUIBase* CloneSelectQuest()
	{
		if (m_pSelectQuest == NULL)
			return NULL;
		return m_pSelectQuest->Clone();
	}
	CUIBase* CloneDescText()
	{
		if (m_pstrDesc == NULL)
			return NULL;
		return m_pstrDesc->Clone();
	}
	CUIBase* CloneSummuryText()
	{
		if (m_pstrSummury == NULL)
			return NULL;
		return m_pstrSummury->Clone();
	}
	CUIBase* CloneSpace()
	{
		if (m_pstrSpace == NULL)
			return NULL;
		return m_pstrSpace->Clone();
	}
private:
	CUIList*	m_plistCopyTitle;
	CUITextBox*	m_ptbDesc;
	CUIText*	m_pstrCount;
	CUIText*	m_pstrRestoreNpc;
	CUITextBox*	m_ptbSummury;
	CUITextBox*	m_ptbRestoreName;
	CUIList*	m_plistCopyItem;
	CUIList*	m_plistCopyView;
	CUIImage*	m_pSelectQuest;
	CUIText*	m_pstrDesc;
	CUIText*	m_pstrSummury;
	CUIText*	m_pstrSpace;
};

#endif	// __UI_QUEST_DESIGN_H__

