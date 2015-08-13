#include "stdhdrs.h"

#include "Server.h"
#include "Expedition.h"
#include "CmdMsg.h"
#include "doFunc.h"

////////////////////////////
// implement of CExpeditionMember
CExpedMember::CExpedMember(int nCharIndex, const char* strCharName, int nGroupType, int nMemberType,int nListIndex, int nLevel )
	: m_strCharName(MAX_CHAR_NAME_LENGTH + 1)
{
	m_nCharIndex	= nCharIndex;
	m_strCharName	= strCharName;
	m_nGroupType	= nGroupType;
	m_nMemberType	= nMemberType;
	m_nListIndex	= nListIndex;
	m_nQuitType		= MSG_EXPED_QUITMODE_NORMAL;
	m_nLabelType	= 0;

	m_nLevel = nLevel;
}

CExpedMember::~CExpedMember()
{
	m_nCharIndex	= 0;
	m_strCharName	= "";
	m_nGroupType	= -1;
	m_nMemberType	= -1;
	m_nLabelType	= -1;
}

void CExpedMember::SetGroupType(int nGroupType)
{
	m_nGroupType	= nGroupType;
}

void CExpedMember::SetListIndex(int nListIndex)
{
	m_nListIndex	= nListIndex;
}

void CExpedMember::SetMemberType(int nMemberType)
{
	m_nMemberType	= nMemberType;
}

void CExpedMember::SetLabelType(int nLabelType)
{
	m_nLabelType	= nLabelType;
}

void CExpedMember::SetQuitType(int nQuitType)
{
	m_nQuitType	= nQuitType;
}

//////////////////////
// implement of CParty
CExpedition::CExpedition(int nSubNo, char nExpeType1, char nExpeType2, char nExpeType3, char nExpeType4, char nExpeType5, int nBossIndex, const char* strBossName, int nGroupType, int nMemberType, int nBossLevel )
	: m_strRequestName(MAX_CHAR_NAME_LENGTH + 1)
{
	m_nExpedType[MSG_DIVITYPE_EXP]		= nExpeType1;
	m_nExpedType[MSG_DIVITYPE_ITEM]		= nExpeType2;
	m_nExpedType[MSG_DIVITYPE_SPECIAL]	= nExpeType3;
	m_nExpedType[MSG_DIVITYPE_SP]		= nExpeType4;
	m_nExpedType[MSG_DIVITYPE_MONEY]	= nExpeType5;

	int i,j;
	for (i = 0; i < MAX_EXPED_GROUP; i++)
	{
		for(j=0; j < MAX_EXPED_GMEMBER; j++)
		{
			m_listMember[i][j] = NULL;
		}
	}

	for(i=0; i < MAX_EXPED_GROUP; i++)
	{
		m_nGTotCount[i]	= 0;
	}

	m_listMember[0][0] = new CExpedMember(nBossIndex, strBossName, nGroupType, nMemberType, 0, nBossLevel );
	m_nSubNo = nSubNo;

	m_nRequestIndex = -1;
	m_strRequestName = "";

	m_nTotCount = 1;		// 전체 인원수
	m_nGTotCount[0] = 1;	// 그룹1 인원수
}

CExpedition::~CExpedition()
{
	m_nExpedType[MSG_DIVITYPE_EXP]		= -1;
	m_nExpedType[MSG_DIVITYPE_ITEM]		= -1;
	m_nExpedType[MSG_DIVITYPE_SPECIAL]	= -1;
	m_nExpedType[MSG_DIVITYPE_SP]		= -1;
	m_nExpedType[MSG_DIVITYPE_MONEY]	= -1;

	m_nTotCount = 0;

	for(int k=0; k < MAX_EXPED_GROUP; k++)
	{
		m_nGTotCount[k]	= 0;
	}

	for (int i = 0; i < MAX_EXPED_GROUP; i++)
	{
		for(int j=0; j < MAX_EXPED_GMEMBER; j++)
		{
			if (m_listMember[i][j] != NULL)
			{
				delete m_listMember[i][j];
				m_listMember[i][j] = NULL;
			}
		}
	}
	m_nRequestIndex = -1;
	m_strRequestName = "";
}

CExpedMember* CExpedition::GetMemberByCharIndex(int nCharIndex) const
{
	for (int i = 0; i < MAX_EXPED_GROUP; i++)
	{
		for(int j=0; j < MAX_EXPED_GMEMBER; j++)
		{
			if (m_listMember[i][j])
			{
				if (m_listMember[i][j]->GetCharIndex() == nCharIndex)
				{
					return m_listMember[i][j];
				}
			}
		}
	}

	return NULL;
}

CExpedMember* CExpedition::GetMemberByListIndex(int nGroup, int nListIndex) const
{
	if (nListIndex < 0 || nListIndex >= MAX_EXPED_GMEMBER)
		return NULL;

	if (m_listMember[nGroup][nListIndex] == NULL)
		return NULL;

	return m_listMember[nGroup][nListIndex];
}

CExpedMember* CExpedition::GetMemberByListIndex(int nGroup, int nListIndex, int nMemberType) const
{
	if (nListIndex < 0 || nListIndex >= MAX_EXPED_GMEMBER)
		return NULL;

	if (m_listMember[nGroup][nListIndex] == NULL)
		return NULL;

	if(m_listMember[nGroup][nListIndex]->GetMemberType() == nMemberType)
		return m_listMember[nGroup][nListIndex];

	return NULL;
}

int CExpedition::FindMemberListIndex(int nCharIndex)
{
	int i;
	for (i = 0; i < MAX_EXPED_GROUP; i++)
	{
		for(int j=0; j < MAX_EXPED_GMEMBER; j++)
		{
			if(m_listMember[i][j] != NULL 	&& m_listMember[i][j]->GetCharIndex() == nCharIndex)
			{
				return j;
			}
		}
	}
	return -1;
}

int CExpedition::FindMemberGroupIndex(int nCharIndex)
{
	int i;
	for (i = 0; i < MAX_EXPED_GROUP; i++)
	{
		for(int j=0; j < MAX_EXPED_GMEMBER; j++)
		{
			if(m_listMember[i][j] != NULL && m_listMember[i][j]->GetCharIndex() == nCharIndex)
			{
				return i;
			}
		}
	}
	return -1;
}

int CExpedition::GetMemberCharIndex(int nGroup, int nListIndex)
{
	if (nListIndex < 0 || nListIndex >= MAX_EXPED_GMEMBER)
		return -1;

	if (m_listMember[nGroup][nListIndex] == NULL)
		return -1;

	return m_listMember[nGroup][nListIndex]->GetCharIndex();
}

void CExpedition::SetRequest(int nRequest, const char* strRequest)
{
	m_nRequestIndex = nRequest;
	m_strRequestName = strRequest;
}

CExpedMember* CExpedition::JoinRequest(const char* strRequestName, int nMemberType , int nLevel )
{
	if (GetRequestIndex() < 1)
		return NULL;

	// 빈자리를 찾고
	int i,j;
	for (i = 0; i < MAX_EXPED_GROUP; i++)
	{
		for(j=0; j < MAX_EXPED_GMEMBER; j++)
		{
			if(m_listMember[i][j] == NULL)
			{
				goto SKIP_JOINREQ;
			}
		}
	}

SKIP_JOINREQ:

	if (i >= MAX_EXPED_GROUP || j >= MAX_EXPED_GMEMBER)
	{
		m_nRequestIndex = -1;
		return NULL;
	}

	// 빈자리에 설정하고
	m_listMember[i][j] = new CExpedMember(m_nRequestIndex, strRequestName, i, nMemberType, j, nLevel );

	m_nTotCount++;
	m_nGTotCount[i]++;

	// request 초기화 하고
	m_nRequestIndex = -1;

	return m_listMember[i][j];
}

// 멤버 참여
CExpedMember* CExpedition::Join(int nCharIndex, const char* strCharName, int nMemberType, int nLevel )
{
	// 빈자리를 찾고
	int i,j;
	for (i = 0; i < MAX_EXPED_GROUP; i++)
	{
		for(j=0; j < MAX_EXPED_GMEMBER; j++)
		{
			if(m_listMember[i][j] == NULL)
			{
				goto SKIP_JOIN;
			}
		}
	}

SKIP_JOIN:

	if (i >= MAX_EXPED_GROUP || j >= MAX_EXPED_GMEMBER)
	{
		return NULL;
	}

	// 빈자리에 설정하고
	m_listMember[i][j] = new CExpedMember(nCharIndex, strCharName, i, nMemberType, j, nLevel);

	m_nTotCount++;
	m_nGTotCount[i]++;

	return m_listMember[i][j];
}

void CExpedition::DeleteMember(int nCharIndex)
{
	for (int i = 0; i < MAX_EXPED_GROUP; i++)
	{
		for(int j=0; j < MAX_EXPED_GMEMBER; j++)
		{
			if (m_listMember[i][j])
			{
				if (m_listMember[i][j]->GetCharIndex() == nCharIndex)
				{
					delete m_listMember[i][j];
					m_listMember[i][j] = NULL;
					m_nTotCount--;
					m_nGTotCount[i]--;

					return;
				}
			}
		}
	}
	return ;
}

bool CExpedition::ChangeBoss(int nTargetIndex, int nOldBossIndex )
{
	CExpedMember* pMemberTarget = (CExpedMember*)GetMemberByCharIndex(nTargetIndex);
	CExpedMember* pMemberOld = (CExpedMember*)GetMemberByCharIndex(nOldBossIndex);

	if( pMemberOld && pMemberTarget && pMemberTarget->m_nLevel > 0 )
	{
		pMemberOld->SetMemberType(MSG_EXPED_MEMBERTYPE_NORMAL);
		pMemberTarget->SetMemberType(MSG_EXPED_MEMBERTYPE_BOSS);
		LONGLONG key = MAKE_LONGLONG_KEY(this->GetSubNo(), nOldBossIndex);
		gserver.m_listExped.erase(key);
		key = MAKE_LONGLONG_KEY(this->GetSubNo(), nTargetIndex);
		gserver.m_listExped.insert(map_expedition_t::value_type(key, this));
		return true;
	}

	return false;
}

int CExpedition::GetGroupMemberCount(int nGroupType)
{
	int nMemberCnt=0;

	for(int i=0; i < MAX_EXPED_GMEMBER; i++)
	{
		if (m_listMember[nGroupType][i])
		{
			nMemberCnt++;
		}
	}

	return nMemberCnt;
}

int CExpedition::GetGroupBeEmptySlot(void)
{
	int nCutMemebrCnt=0;
	for(int iGroup=0;  iGroup < MAX_EXPED_GROUP; iGroup++)
	{
		nCutMemebrCnt = GetGroupMemberCount(iGroup);
		if( nCutMemebrCnt >= 0 && nCutMemebrCnt < MAX_EXPED_GMEMBER)
			return iGroup;
	}

	return -1;
}

// 그룹 멤버 타임 수 획득
int CExpedition::GetGroupMembertypeCount(int nGroupType, int nMemberType)
{
	int nMemberCount = 0;

	for(int i=0; i < MAX_EXPED_GMEMBER; i++)
	{
		if (m_listMember[nGroupType][i])
		{
			const CExpedMember* pMember = m_listMember[nGroupType][i];

			if(pMember->GetMemberType() == nMemberType)
				nMemberCount++;
		}
	}

	return	nMemberCount;
}

bool CExpedition::SetMBoss(int nNewMBossIndex)
{
	for (int i = 0; i < MAX_EXPED_GROUP; i++)
	{
		for(int j=0; j < MAX_EXPED_GMEMBER; j++)
		{
			if (m_listMember[i][j] != NULL && m_listMember[i][j]->GetCharIndex() == nNewMBossIndex)
			{
				CExpedMember* pMember = (CExpedMember*)m_listMember[i][j];
				pMember->SetMemberType(MSG_EXPED_MEMBERTYPE_MBOSS);
				return true;
			}
		}
	}
	return false;
}

bool CExpedition::ResetMBoss(int nNewMBossIndex)
{
	for (int i = 0; i < MAX_EXPED_GROUP; i++)
	{
		for(int j=0; j < MAX_EXPED_GMEMBER; j++)
		{
			if(m_listMember[i][j] != NULL && m_listMember[i][j]->GetCharIndex() == nNewMBossIndex)
			{
				CExpedMember* pMember = (CExpedMember*)m_listMember[i][j];
				pMember->SetMemberType(MSG_EXPED_MEMBERTYPE_NORMAL);
				return true;
			}
		}
	}
	return false;
}

bool CExpedition::MoveGroup(int nSourceGroup, int nMoveCharIndex, int nTargetGroup, int nTargetListindex)
{
	if(m_listMember[nTargetGroup][nTargetListindex] != NULL)
		return false;

	if(GetGroupMemberCount(nTargetGroup) >= MAX_EXPED_GMEMBER)
		return false;

	int nSourceListindex = FindMemberListIndex(nMoveCharIndex);

	if(m_listMember[nSourceGroup][nSourceListindex] == NULL)
		return false;

	if(nSourceListindex >= 0)
	{
		// 이동
		m_listMember[nTargetGroup][nTargetListindex] = m_listMember[nSourceGroup][nSourceListindex];
		m_nGTotCount[nTargetGroup]++;

		m_listMember[nTargetGroup][nTargetListindex]->SetGroupType(nTargetGroup);
		m_listMember[nTargetGroup][nTargetListindex]->SetListIndex(nTargetListindex);

		// 소스 초기화
		m_listMember[nSourceGroup][nSourceListindex] = NULL;
		m_nGTotCount[nSourceGroup]--;

		return true;
	}

	return false;
}

//위임할 보스 찾기
const CExpedMember* CExpedition::FindNextBoss(void)
{
	for (int i = 0; i < MAX_EXPED_GROUP; i++)
	{
		for(int j=0; j < MAX_EXPED_GMEMBER; j++)
		{
			if(m_listMember[i][j])
			{
				CExpedMember* pMember = m_listMember[i][j];
				if( pMember != NULL &&
						pMember->m_nLevel > 0 &&
						pMember->GetQuitType() != MSG_EXPED_QUITMODE_UNUSUAL &&
						(pMember->GetMemberType() == MSG_EXPED_MEMBERTYPE_MBOSS ||
						 pMember->GetMemberType() == MSG_EXPED_MEMBERTYPE_NORMAL)
				  )
				{
					return m_listMember[i][j];
				}
			}
		}
	}
	return NULL;
}

char CExpedition::GetExpedType(int nType)
{
	if(nType < 0 || nType >= MAX_EXPED_DIVITYPE) return -1;

	return m_nExpedType[nType];
}

int	CExpedition::GetMemberCountOnline()
{
	int ret = 0;
	for (int i = 0; i < MAX_EXPED_GROUP; i++)
	{
		for(int j=0; j < MAX_EXPED_GMEMBER; j++)
		{
			if( m_listMember[i][j] != NULL )
			{
				CExpedMember* pMember = m_listMember[i][j];
				if( pMember && pMember->m_nLevel > 0 )
					ret++;
			}
		}
	}
	return ret;
}

// boss 인덱스 반환
int CExpedition::GetBossIndex()
{
	for (int i = 0; i < MAX_EXPED_GROUP; i++)
	{
		for(int j=0; j < MAX_EXPED_GMEMBER; j++)
		{
			if(m_listMember[i][j])
			{
				CExpedMember* pMember = (CExpedMember*)m_listMember[i][j];
				if(pMember && pMember->GetMemberType() == MSG_EXPED_MEMBERTYPE_BOSS)
				{
					return pMember->GetCharIndex();
				}
			}
		}
	}

	return  -1;
}

const char* CExpedition::GetBossName()
{
	for (int i = 0; i < MAX_EXPED_GROUP; i++)
	{
		for(int j=0; j < MAX_EXPED_GMEMBER; j++)
		{
			if(m_listMember[i][j])
			{
				CExpedMember* pMember = (CExpedMember*)m_listMember[i][j];
				if( pMember && pMember->GetMemberType() == MSG_EXPED_MEMBERTYPE_BOSS)
				{
					return pMember->GetCharName();
				}
			}
		}
	}

	return  NULL;
}

void CExpedition::SetEndExped()
{
	CExpedMember* pMember = NULL;

	for(int i=0; i < MAX_EXPED_GROUP; i++)
	{
		for(int j=0; j < MAX_EXPED_GMEMBER; j++)
		{
			pMember = (CExpedMember*) GetMemberByListIndex(i,j);
			if(pMember)
			{
				DeleteMember(pMember->GetCharIndex());
			}
		}
	}
}
