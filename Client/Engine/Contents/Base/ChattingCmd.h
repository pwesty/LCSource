#ifndef __COMMAND_CHATTING_H__
#define __COMMAND_CHATTING_H__

class Command;

class CmdChattingOpenOption : public Command
{
public:
	CmdChattingOpenOption()	: m_pWnd(NULL) {};
	void setData(CChattingUI* pWnd)	{ m_pWnd = pWnd;	}
	void execute()
	{
		if (m_pWnd != NULL)
			m_pWnd->OpenChatOption();
	}
private:
	CChattingUI* m_pWnd;
};

class CmdChattingOptionChange : public Command
{
public:
	CmdChattingOptionChange() : m_pWnd(NULL), m_nTab(-1), m_nOption(-1) {}
	void setData(CChattingUI* pWnd, int nTab, int nOption)	
	{
		m_pWnd = pWnd;
		m_nTab = nTab;
		m_nOption = nOption;
	}
	void execute()
	{
		if (m_pWnd != NULL)
		{
			m_pWnd->ChangeChatOption(m_nTab, m_nOption);
		}
	}
private:
	CChattingUI* m_pWnd;
	int m_nTab;
	int m_nOption;
};

class CmdChattingChangeTab : public Command
{
public: 
	CmdChattingChangeTab() : m_pWnd(NULL), m_nTabIdx(-1), m_bMain(false) {}
	void setData(CChattingUI* pWnd, int nTabIdx, bool bMain)
	{
		m_pWnd = pWnd;
		m_nTabIdx = nTabIdx;
		m_bMain = bMain;
	}
	void execute()
	{
		if (m_bMain == true)
			m_pWnd->ChangeMainTab(m_nTabIdx);
		else
			m_pWnd->ChangeSubTab(m_nTabIdx);
	}
private:
	CChattingUI* m_pWnd;
	int m_nTabIdx;
	bool m_bMain;
};

class CmdUserNotice : public Command
{
public:
	CmdUserNotice() : m_pWnd(NULL)	{}
	void setData(CChattingUI* pWnd, int msgID)	{ m_pWnd = pWnd; m_msgID = msgID;	}
	void execute()
	{
		if (m_pWnd != NULL)
			m_pWnd->CheckUserNoitce(m_msgID);
	}
private:
	CChattingUI* m_pWnd;
	int			 m_msgID;
};
#endif // __COMMAND_CHATTING_H__