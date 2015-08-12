#ifndef		_RANKING_SYSTEM_COMMAND_H_
#define		_RANKING_SYSTEM_COMMAND_H_

class Command;
//--------------------------------------------------------------
class CmdRankingClose : public Command
{
public:
	CmdRankingClose() : pUI_(NULL) {}
	void setData(CUIRankingSystem* pUI)	{ pUI_ = pUI; }
	void execute() {
		if (pUI_)
			pUI_->CloseUI();
	}
private:
	CUIRankingSystem* pUI_;
};

//--------------------------------------------------------------
class CmdRankingReset : public Command
{
public:
	CmdRankingReset() : pUI_(NULL) {}
	void setData(CUIRankingSystem* pUI)	{ pUI_ = pUI; }
	void execute() {
		if (pUI_)
			pUI_->ToggleReset();
	}
private:
	CUIRankingSystem* pUI_;
};

//--------------------------------------------------------------
class CmdRankingSearch : public Command
{
public:
	CmdRankingSearch() : pUI_(NULL), pEditBox_(NULL) {}
	void setData(CUIRankingSystem* pUI, CUIEditBox* pEditBox)	
	{
		pUI_ = pUI; 
		pEditBox_ = pEditBox;
	}
	void execute() {
		if (pUI_)
		{
			if (pEditBox_ != NULL)
			{
				pUI_->SearchName( pEditBox_->GetString() );
				pEditBox_->SetFocus(FALSE);
			}
		}
			
	}
private:
	CUIRankingSystem* pUI_;
	CUIEditBox*	pEditBox_;
};

//--------------------------------------------------------------
class CmdRankingMouseEvent : public Command
{
public:
	CmdRankingMouseEvent() : pUI_(NULL), m_pItem(NULL), m_Color(DEF_UI_COLOR_WHITE) {}
	void setData(CUIRankingSystem* pUI, CUIRankingSystem::RANK_LIST_TYPE ListType, 
		CUIListItem* pItem, COLOR col, CUIRankingSystem::MOUSE_EVENT_TYPE type)	
	{
		pUI_ = pUI;
		m_ListType = ListType;
		m_pItem = pItem;
		m_Color = col;
		m_eventType = type;
	}
	void execute() {
		if (pUI_)
		{
			pUI_->SetRankingListMouseEvent( m_ListType, m_pItem, m_Color, m_eventType );
		}

	}
private:
	CUIRankingSystem*	pUI_;
	CUIListItem*		m_pItem;
	COLOR				m_Color;
	CUIRankingSystem::RANK_LIST_TYPE	m_ListType;
	CUIRankingSystem::MOUSE_EVENT_TYPE	m_eventType;
};

//--------------------------------------------------------------
class CmdRankingListUnSelect : public Command
{
public:
	CmdRankingListUnSelect() : pUI_(NULL) {}
	void setData(CUIRankingSystem* pUI, CUIRankingSystem::RANK_LIST_TYPE ListType, COLOR col)	
	{
		pUI_ = pUI;
		m_Color = col;
		m_ListType = ListType;
	}
	void execute() {
		if (pUI_)
		{
			pUI_->SetRankingListUnSelect( m_ListType, m_Color );
		}

	}
private:
	CUIRankingSystem*	pUI_;
	COLOR				m_Color;
	CUIRankingSystem::RANK_LIST_TYPE	m_ListType;
};

//--------------------------------------------------------------
class CmdRankingListSelect : public Command
{
public:
	CmdRankingListSelect() : pUI_(NULL) {}
	void setData(CUIRankingSystem* pUI, CUIRankingSystem::RANK_LIST_TYPE ListType, COLOR col)	
	{
		pUI_ = pUI;
		m_Color = col;
		m_ListType = ListType;
	}
	void execute() {
		if (pUI_)
		{
			pUI_->SetRankingListSelect( m_ListType, m_Color );
		}

	}
private:
	CUIRankingSystem*	pUI_;
	COLOR				m_Color;
	CUIRankingSystem::RANK_LIST_TYPE	m_ListType;
};
#endif		// _RANKING_SYSTEM_COMMAND_H_