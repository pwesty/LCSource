#ifndef __CMD_H__
#define __CMD_H__

#include <map>

#define CMD_NUM 200

class CDescriptor;

class CCmd
{
public:
	CCmd();
	~CCmd();

	int	m_cmdType;
	void (*m_cmdFunc) (CNetMsg::SP& msg, CDescriptor*);
	int m_minLevel;
};

class CCmdList
{
public:
	CCmdList();
	~CCmdList();

	bool Add(int cmdType, void (*cmdFunc)(CNetMsg::SP&, CDescriptor*), int minlevel);
	bool Run(int cmd_num, CNetMsg::SP& msg, CDescriptor* dest);

private:
	typedef std::map<int, CCmd> map_t;
	map_t		map_;
};

#endif
//
