#include "stdhdrs.h"

#include "IntergrationServer.h"
#if defined (INTERGRATION_SERVER)

#include "Server.h"
CIntergrationServer::CIntergrationServer(void)
	: m_bEnable(false)
{
}

CIntergrationServer::~CIntergrationServer(void)
{
}

bool CIntergrationServer::IsEnable()
{
	return m_bEnable;
}

void CIntergrationServer::SetEnable(bool b)
{
	m_bEnable = b;
}

bool CIntergrationServer::Check(unsigned char nation)
{
	if (m_bEnable)
	{
		if (static_cast<int>(m_nationList.size()) <= 0)
			return false;

		std::vector<unsigned char>::iterator itr = std::find(m_nationList.begin(), m_nationList.end(), nation);
		if ( itr == m_nationList.end() )
			return false;
		else
			return true;
	}
	else
	{
		if ( nation != gserver.m_national )
			return false;
		else
			return true;
	}
}

void CIntergrationServer::AddNation(unsigned char nation)
{
	m_nationList.push_back(nation);
}

#endif
//