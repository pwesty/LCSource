#include "stdh.h"
#include "UIQuestDesign.h"

//////////////////////////////////////////////////////////////////////////

CUIQuestDesign::CUIQuestDesign()
	:	m_plistCopyTitle(NULL)
	,	m_ptbSummury(NULL)
	,	m_ptbDesc(NULL)
	,	m_pstrCount(NULL)
	,	m_pstrRestoreNpc(NULL)
	,	m_ptbRestoreName(NULL)
	,	m_plistCopyItem(NULL)
	,	m_plistCopyView(NULL)
	,	m_pSelectQuest(NULL)
	,	m_pstrDesc(NULL)
	,	m_pstrSummury(NULL)
	,	m_pstrSpace(NULL)
{
}

void CUIQuestDesign::initialize()
{
	m_plistCopyTitle	= (CUIList*)findUI("list_copyTitle");
	m_ptbDesc			= (CUITextBox*)findUI("tb_Desc");
	m_pstrCount			= (CUIText*)findUI("str_copyCount");
	m_pstrRestoreNpc	= (CUIText*)findUI("str_copyRestoreNpc");
	m_ptbSummury		= (CUITextBox*)findUI("tb_Summury");
	m_ptbRestoreName	= (CUITextBox*)findUI("tb_copyRestoreName");
	m_plistCopyItem		= (CUIList*)findUI("list_copyitem");
	m_plistCopyView		= (CUIList*)findUI("list_copyview");
	m_pSelectQuest		= (CUIImage*)findUI("img_selectMenu");
	m_pstrDesc			= (CUIText*)findUI("str_Desc");
	m_pstrSummury		= (CUIText*)findUI("str_Summury");
	m_pstrSpace			= (CUIText*)findUI("str_space");
}