#ifndef __CMD_H__
#define __CMD_H__

#include "NetMsg.h"

#define CMD_NUM 200

class CDescriptor;

class CCmd
{
public:
	CCmd();
	~CCmd();

	int	m_cmdType;
	void (*m_cmdFunc) (CNetMsg& msg, CDescriptor*);
};

class CCmdList
{
public:
	CCmdList();
	~CCmdList();

	CCmd* m_cmd[CMD_NUM];
	int m_cnt;

	bool Add(int cmdType, void cmdFunc(CNetMsg&, CDescriptor*));
	void Sort(void);
	int Find(int cmdType);
	void Run(int cmd_num, CNetMsg& msg, CDescriptor* dest);
};

#endif
