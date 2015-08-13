#include "stdhdrs.h"

#include "Server.h"
#include "Party.h"
#include "CmdMsg.h"

////////////////////////////
// implement of CPartyMember
CPartyMember::CPartyMember(int nCharIndex, const char* strCharName, int nLevel )
	: m_strCharName(MAX_CHAR_NAME_LENGTH + 1)
{
	m_nCharIndex = nCharIndex;
	m_strCharName = strCharName;

	m_nLevel = nLevel;
}

CPartyMember::~CPartyMember()
{
	m_nCharIndex = 0;
	m_strCharName = "";
	m_nLevel = 0;
}

CParty::CParty(int nSubNo, char nPartyType, int nBossIndex, const char* strBossName, int nRequest, const char* strRequest, int nBossLevel )
	: m_strRequest(MAX_CHAR_NAME_LENGTH + 1)
{
	m_nPartyType[MSG_DIVITYPE_EXP]		= nPartyType;			   // 경험치(균등(랜던),입수우선,전투)
	m_nPartyType[MSG_DIVITYPE_ITEM]		= MSG_PARTY_TYPE_FIRSTGET; // 디폴터 설정(입수우선)
	m_nPartyType[MSG_DIVITYPE_SPECIAL]	= MSG_PARTY_TYPE_OPENBOX;  // 디폴터 설정(상자열기)
	m_nPartyType[MSG_DIVITYPE_SP]		= nPartyType;
	m_nPartyType[MSG_DIVITYPE_MONEY]	= nPartyType;

	memset(m_listMember, 0, sizeof(CPartyMember*) * MAX_PARTY_MEMBER);

	m_listMember[0] = new CPartyMember(nBossIndex, strBossName, nBossLevel);
	m_nSubNo = nSubNo;

	m_nRequest = nRequest;
	m_strRequest = strRequest;
	m_nCount = 1;

#ifdef GMTOOL
	m_bGmtarget = false;
#endif // GMTOOL
}

CParty::~CParty()
{
	m_nPartyType[MSG_DIVITYPE_EXP] = -1;
	m_nPartyType[MSG_DIVITYPE_ITEM] = -1;
	m_nPartyType[MSG_DIVITYPE_SPECIAL] = -1;
	m_nPartyType[MSG_DIVITYPE_SP] = -1;
	m_nPartyType[MSG_DIVITYPE_MONEY] = -1;

	m_nCount = 0;
	int i;
	for (i = 0; i < MAX_PARTY_MEMBER; i++)
	{
		if (m_listMember[i] != NULL)
		{
			delete m_listMember[i];
			m_listMember[i] = NULL;
		}
	}
	m_nRequest = -1;
}

int CParty::GetMemberCountOnline()
{
	int ret = 0;
	int i = 0;
	for (i = 0; i < MAX_PARTY_MEMBER; i++)
	{
		if( m_listMember[i] != NULL && m_listMember[i]->m_nLevel > 0 )
			ret++;
	}

	return ret;
}

//파티 대표값 설정
void CParty::SetPartyTypeAll(char cPartyType)
{
	m_nPartyType[MSG_DIVITYPE_EXP]		= cPartyType;
	m_nPartyType[MSG_DIVITYPE_SP]		= cPartyType;
	m_nPartyType[MSG_DIVITYPE_MONEY]	= cPartyType;
}

CPartyMember* CParty::GetMemberByListIndex(int nListIndex) const
{
	if (nListIndex < 0 || nListIndex >= MAX_PARTY_MEMBER)
		return NULL;
	if (m_listMember[nListIndex] == NULL)
		return NULL;
	return m_listMember[nListIndex];
}

int CParty::FindMember(int nCharIndex)
{
	int i;
	for (i = 0; i < MAX_PARTY_MEMBER; i++)
	{
		if (	   m_listMember[i] != NULL
				   && m_listMember[i]->GetCharIndex() == nCharIndex
		   )
		{
			return i;
		}
	}
	return -1;
}

int CParty::GetMemberCharIndex(int nListIndex)
{
	if (nListIndex < 0 || nListIndex >= MAX_PARTY_MEMBER)
		return -1;
	if (m_listMember[nListIndex] == NULL)
		return -1;
	return m_listMember[nListIndex]->GetCharIndex();
}

void CParty::SetRequest(int nRequest, const char* strRequest)
{
	m_nRequest = nRequest;
	m_strRequest = strRequest;
}

int CParty::JoinRequest(const char* strRequestName, int nLevel )
{
	if (GetRequestIndex() < 1)
		return -1;

	// 빈자리를 찾고
	int i;
	for (i = 0; i < MAX_PARTY_MEMBER; i++)
	{
		if (m_listMember[i] == NULL)
			break;
	}
	if (i == MAX_PARTY_MEMBER)
	{
		m_nRequest = -1;
		return -1;
	}

	// 빈자리에 설정하고
	m_listMember[i] = new CPartyMember(m_nRequest, strRequestName, nLevel );
	m_nCount++;

	// request 초기화 하고
	m_nRequest = -1;

	return i;
}

void CParty::DeleteMember(int nCharIndex)
{
	int i;
	for (i = 0; i < MAX_PARTY_MEMBER; i++)
	{
		if (m_listMember[i])
		{
			if (m_listMember[i]->GetCharIndex() == nCharIndex)
			{
				delete m_listMember[i];
				m_listMember[i] = NULL;
				m_nCount--;
				if (i == 0)
				{
					LONGLONG key = MAKE_LONGLONG_KEY(this->GetSubNo(), nCharIndex);
					gserver.m_listParty.erase(key);
				}
			}
		}
		if (i < MAX_PARTY_MEMBER - 1 && m_listMember[i] == NULL)
		{
			m_listMember[i] = m_listMember[i + 1];
			m_listMember[i + 1] = NULL;
			if (i == 0)
			{
				LONGLONG key = MAKE_LONGLONG_KEY(this->GetSubNo(), m_listMember[i]->GetCharIndex());
				gserver.m_listParty.insert(map_party_t::value_type(key, this));
			}
		}
	}
	return ;
}

bool CParty::ChangeBoss(const char* strNewBossName)
{
	int i;
	for (i = 0; i < MAX_PARTY_MEMBER; i++)
	{
		if (m_listMember[i] && m_listMember[i]->m_nLevel > 0 )
		{
			if (strcmp(m_listMember[i]->GetCharName(), strNewBossName) == 0)
			{
				CPartyMember* pMember = m_listMember[0];
				m_listMember[0] = m_listMember[i];
				m_listMember[i] = pMember;
				LONGLONG key = MAKE_LONGLONG_KEY(this->GetSubNo(), pMember->GetCharIndex());
				gserver.m_listParty.erase(key);
				key = MAKE_LONGLONG_KEY(this->GetSubNo(), m_listMember[0]->GetCharIndex());
				gserver.m_listParty.insert(map_party_t::value_type(key, this));
				return true;
			}
		}
	}
	return false;
}

void CParty::SetEndParty()
{
	for(int i=0; i < MAX_PARTY_MEMBER; i++)
	{
		delete m_listMember[i];
		m_listMember[i] = NULL;
	}
}
