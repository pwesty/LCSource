#ifndef __COMMAND_ITEMCOLLECTION_H__
#define __COMMAND_ITEMCOLLECTION_H__

class Command;

class CmdItemCollectionClose : public Command
{
public:
	CmdItemCollectionClose() : m_pWnd(NULL) {}
	void setData(CItemCollectionUI* pWnd) { m_pWnd = pWnd;	}
	void execute()
	{
		if (m_pWnd != NULL)
			m_pWnd->CloseUI();
	}

private:
	CItemCollectionUI* m_pWnd;

};

class CmdItemCollectionNext_Prev : public Command
{
public:
	CmdItemCollectionNext_Prev() : m_pWnd(NULL), m_bNext(true) {}
	void setData(CItemCollectionUI* pWnd, bool bNext) { m_pWnd = pWnd;	m_bNext = bNext; }
	void execute()
	{
		if (m_pWnd != NULL)
		{
			if (m_bNext == true)
				m_pWnd->NextCategory();
			else
				m_pWnd->PrevCategory();
		}
	}

private:
	CItemCollectionUI* m_pWnd;
	bool m_bNext;
};

class CmdItemCollectionSelectTheme : public Command
{
public:
	CmdItemCollectionSelectTheme() : m_pWnd(NULL), m_nThemeIdx(-1), m_nCat(-1), m_nTheme(-1), m_bScrollPos(false) {}
	void setData(CItemCollectionUI* pWnd, int nIdx, int nCat, int nTheme, bool bScrollPos = false) 
	{ 
		m_pWnd = pWnd;
		m_nThemeIdx = nIdx;
		m_nCat = nCat;
		m_nTheme = nTheme;
		m_bScrollPos = bScrollPos;
	}
	void execute()
	{
		if (m_pWnd != NULL)
		{
			m_pWnd->SelectTheme(m_nThemeIdx);

			if (m_nCat < 0 || m_nTheme < 0)
				return;

			m_pWnd->SelectImage(m_nCat, m_nTheme);

			if (m_bScrollPos == true)
				m_pWnd->UpdateScrollPos(m_nCat, m_nTheme);
		}
	}
private:
	CItemCollectionUI* m_pWnd;
	int m_nThemeIdx;
	int m_nCat;
	int m_nTheme;
	bool m_bScrollPos;
};

// 보상 또는 기부.
class CmdItemCollectionAmend : public Command
{
public:
	CmdItemCollectionAmend() : m_pWnd(NULL), m_bAmend(false){}
	void setData(CItemCollectionUI* pWnd, bool bAmend) { m_pWnd = pWnd; m_bAmend = bAmend; }
	void execute()
	{
		if (m_pWnd != NULL)
		{
			if (m_bAmend == true)
				m_pWnd->SendAmend();
			else
				m_pWnd->SendGiveItem();
		}
	}

private:
	CItemCollectionUI* m_pWnd;
	bool m_bAmend;
};

class CmdItemCollectionOutputMsg : public Command
{
public :
	CmdItemCollectionOutputMsg() : m_pWnd(NULL)	{}
	void setData(CItemCollectionUI* pWnd, eOUTPUT_MSG eType)	{ m_pWnd = pWnd, m_eMsgType = eType; }
	void execute()
	{
		if (m_pWnd != NULL)
		{
			m_pWnd->OutputMessage(m_eMsgType);
		}
	}
private:
	CItemCollectionUI* m_pWnd;
	eOUTPUT_MSG m_eMsgType;
};

class CmdItemCollectionMax : public Command
{
public:
	CmdItemCollectionMax() : m_pWnd(NULL), m_pArrayItem(NULL), m_nThemeIndex(-1), m_nItemIndex(-1) {}
	void setData(CItemCollectionUI* pWnd, CUIArrayItem* pArrayItem, int nThemeIndex, int nItemIndex)	
	{
		m_pWnd = pWnd;	
		m_pArrayItem = pArrayItem;
		m_nThemeIndex = nThemeIndex;
		m_nItemIndex = nItemIndex;
	}
	void execute()
	{
		if (m_pWnd != NULL && m_pArrayItem != NULL)
		{
			m_pWnd->NeedItemCountMax(m_pArrayItem, m_nThemeIndex, m_nItemIndex);
		}
	}
private:
	CItemCollectionUI* m_pWnd;
	CUIArrayItem* m_pArrayItem;
	int m_nItemIndex;
	int m_nThemeIndex;
};

class CmdItemCollectionSum : public Command
{
public:
	CmdItemCollectionSum() : m_pWnd(NULL), m_pArrayItem(NULL), m_nThemeIndex(-1), m_nItemIndex(-1) {}
	void setData(CItemCollectionUI* pWnd, CUIArrayItem* pArrayItem, int nThemeIndex, int nItemIndex)	
	{
		m_pWnd = pWnd;	
		m_nItemIndex = nItemIndex;
		m_nThemeIndex = nThemeIndex;
		m_pArrayItem = pArrayItem;
	}
	void execute()
	{
		if (m_pWnd != NULL)
		{
			m_pWnd->NeedItemCountSum(m_pArrayItem, m_nThemeIndex, m_nItemIndex);
		}
	}
private:
	CItemCollectionUI* m_pWnd;
	CUIArrayItem* m_pArrayItem;
	int m_nItemIndex;
	int m_nThemeIndex;
};

class CmdItemCollectionSub : public Command
{
public:
	CmdItemCollectionSub() : m_pWnd(NULL), m_pArrayItem(NULL), m_nThemeIndex(-1), m_nItemIndex(-1) {}
	void setData(CItemCollectionUI* pWnd, CUIArrayItem* pArrayItem, int nThemeIndex, int nItemIndex)	
	{
		m_pWnd = pWnd;	
		m_pArrayItem = pArrayItem;
		m_nThemeIndex = nThemeIndex;
		m_nItemIndex = nItemIndex;
	}
	void execute()
	{
		if (m_pWnd != NULL)
		{
			m_pWnd->NeedItemCountSub(m_pArrayItem, m_nThemeIndex, m_nItemIndex);
		}
	}
private:
	CItemCollectionUI* m_pWnd;
	CUIArrayItem* m_pArrayItem;
	int m_nItemIndex;
	int m_nThemeIndex;
};

class CmdItemCollectionSearch : public Command
{
public:
	CmdItemCollectionSearch() : m_pWnd(NULL) {}
	void setData(CItemCollectionUI* pWnd)	{ m_pWnd = pWnd;	}
	void execute()
	{
		if (m_pWnd != NULL)
		{
			if (m_pWnd->ArrangeSearchList() == true)
				m_pWnd->UpdateSearchList();
		}
	}

private:
	CItemCollectionUI* m_pWnd;
};

class CmdItemCollectionCheckState : public Command
{
public:
	CmdItemCollectionCheckState() : m_pWnd(NULL), m_pCheck(NULL) {}
	void setData(CItemCollectionUI* pWnd, CUICheckButton* pCheck)	{ m_pWnd = pWnd; m_pCheck = pCheck;	}
	void execute()
	{
		if (m_pWnd != NULL)
		{
			m_pWnd->UnCheck(m_pCheck);
		}
	}

private:
	CItemCollectionUI* m_pWnd;
	CUICheckButton* m_pCheck;
};

class CmdItemCollectionChangeTab : public Command
{
public:
	CmdItemCollectionChangeTab() : m_pWnd(NULL), m_nID(-1), m_bMain(true)	{}
	void setData(CItemCollectionUI* pWnd, int nID, bool bMain)	{ m_pWnd = pWnd; m_nID = nID; m_bMain = bMain;	}
	void execute()
	{
		if (m_pWnd != NULL)
		{
			if (m_bMain == true)
				m_pWnd->ChangeMainTab(m_nID);
			else
				m_pWnd->ChangeListTab(m_nID);
		}
	}
private:
	CItemCollectionUI* m_pWnd;
	int m_nID;
	bool m_bMain;
};

class CmdItemCollectionEnterColor : public Command
{
public:
	CmdItemCollectionEnterColor() : m_pWnd(NULL), m_nCat(-1), m_nTheme(-1), m_bEnter(true) {}
	void setData(CItemCollectionUI* pWnd, int nCat, int nTheme, bool bEnter)
	{
		m_pWnd = pWnd;
		m_nCat = nCat;
		m_nTheme = nTheme;
		m_bEnter = bEnter;
	}
	void execute()
	{
		if (m_pWnd != NULL)
		{
			m_pWnd->ChangeTreeListEnterColor(m_nCat, m_nTheme, m_bEnter);
		}
	}

private:
	CItemCollectionUI* m_pWnd;
	int m_nCat;
	int m_nTheme;
	bool m_bEnter;
};

#endif // __COMMAND_ITEMCOLLECTION_H__