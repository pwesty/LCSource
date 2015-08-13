// Cmd.h: interface for the CCmd class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CMD_H__D5669736_0776_4A55_ACEA_FCB8C62E3ABA__INCLUDED_)
#define AFX_CMD_H__D5669736_0776_4A55_ACEA_FCB8C62E3ABA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define CMD_NUM		130

class CSessionState;

class CFunc
{
public:
	char *m_CmdStr;
	void (*m_CmdP) (char *arg, CSessionState *pSession);
};

class CCmd  
{
public:
	CCmd(CSessionState *pSession);
	virtual ~CCmd();

	BOOL AddCmd(char *cmd_str, void cmd_p(char *, CSessionState *));

	//void Sort(void);
	int Find(char *cmd_str);
	void Run(int cmd_num, char *arg);

	CFunc *m_Func[CMD_NUM];
	int m_Count;
	CSessionState *m_pSession;		

		    
};

#endif 