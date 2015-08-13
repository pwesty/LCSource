#ifndef __INTERGRATION_SERVER_H__
#define __INTERGRATION_SERVER_H__

#if defined (INTERGRATION_SERVER)

#include <vector>
#include <algorithm>

class CIntergrationServer
{
public:
	CIntergrationServer(void);
	~CIntergrationServer(void);

private:
	bool m_bEnable;
	std::vector<unsigned char> m_nationList;

public:
	bool IsEnable();
	void SetEnable(bool b);
	bool Check(unsigned char nation);
	void AddNation(unsigned char nation);
};

#endif

#endif
