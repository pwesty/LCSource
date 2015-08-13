#include "stdhdrs.h"

#include "Server.h"
#include "Expedition.h"
#include "CmdMsg.h"
#include "doFunc.h"

#include "Battle.h"

////////////////////////////
// implement of CExpeditionMember
CExpedMember::CExpedMember(int nCharIndex, const char* strCharName, int nGroupType, int nMemberType,int nListIndex, CPC* pc)
	: m_strCharName(MAX_CHAR_NAME_LENGTH + 1)
{
	m_nCharIndex	= nCharIndex;
	m_strCharName	= strCharName;
	m_nGroupType	= nGroupType;
	m_nMemberType	= nMemberType;
	m_nListIndex	= nListIndex;
	m_nQuitType		= MSG_EXPED_QUITMODE_NORMAL;
	m_nLabelType	= 0;

	if(pc)	m_nLevel = pc->m_level;
	else	m_nLevel = 0;

	m_pChar = pc;
}

CExpedMember::~CExpedMember()
{
	m_nCharIndex	= 0;
	m_strCharName	= "";
	m_nGroupType	= -1;
	m_nMemberType	= -1;
	m_nLabelType	= -1;

	m_pChar = NULL;
}

void CExpedMember::SetMemberPCPtr(CPC* pc)
{
	if (m_pChar != NULL && pc == NULL)
		m_pChar->m_Exped = NULL;						//CPC 클래스에 원정대 추가 필요
	m_pChar = pc;
	if( pc )
		m_nLevel = pc->m_level;
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
CExpedition::CExpedition(char nExpeType1, char nExpeType2, char nExpeType3, char nExpeType4, char nExpeType5, int nBossIndex, const char* strBossName, int nGroupType, int nMemberType, CPC* pBoss)
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

	m_listMember[0][0] = new CExpedMember(nBossIndex, strBossName, nGroupType, nMemberType, 0,pBoss);

	m_nRequestIndex = -1;
	m_strRequestName = "";

	m_nTotCount = 1;		// 전체 인원수
	m_nGTotCount[0] = 1;	// 그룹1 인원수

	m_nEndExpedTime = 0;	// 원정대 해체 시간
	m_nRaidZoneNum = -1;
	m_nRaidAreaNum = -1;
	m_nSubNumber = gserver->m_subno;
	m_nDifficulty = -1;
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
				if (m_listMember[i][j]->GetMemberPCPtr())
				{
					m_listMember[i][j]->SetMemberPCPtr(NULL);
				}
				delete m_listMember[i][j];
				m_listMember[i][j] = NULL;
			}
		}
	}
	m_nRequestIndex = -1;
	m_strRequestName = "";

	m_nEndExpedTime = 0;	// 원정대 해체 시간
}

//////////////////////////////////////////////////////////////////////
// 게임 서버 전용 함수들
int CExpedition::GetNearExpeditionMemberCount(CCharacter* pCenter)
{
	int i;
	int ret = 0;
	for (i = 0; i < MAX_EXPED_GROUP; i++)
	{
		for(int j=0; j < MAX_EXPED_GMEMBER; j++)
		{
			if ( m_listMember[i][j] != NULL
					&& m_listMember[i][j]->GetMemberPCPtr()
					&& CheckInNearCellExt(pCenter, m_listMember[i][j]->GetMemberPCPtr())
			   )
			{
				ret++;
			}
		}
	}

	return ret;
}

void CExpedition::SendToAllPC(CNetMsg::SP& msg, int nExcludeCharIndex)
{
	int i,j;
	CExpedMember* pMember = NULL;

	for (i = 0; i < MAX_EXPED_GROUP; i++)
	{
		for(j = 0; j < MAX_EXPED_GMEMBER; j++)
		{
			pMember = (CExpedMember*)m_listMember[i][j];
			if ( pMember != NULL)
			{
				if (pMember->GetCharIndex() != nExcludeCharIndex)
				{
					CPC* pPC = TO_PC(pMember->GetMemberPCPtr());
					if(pPC != NULL && pPC->m_index>0 && pPC->m_level>0 )
						SEND_Q(msg, pPC->m_desc);
				}
			}
		}
	}
}

void CExpedition::SendToPCInSameZone(int nZone, int nArea, CNetMsg::SP& msg)
{
	int i;
	CExpedMember* pMember = NULL;

	for (i = 0; i < MAX_EXPED_GROUP; i++)
	{
		for(int j=0; j < MAX_EXPED_GMEMBER; j++)
		{
			pMember = (CExpedMember*)m_listMember[i][j];
			if ( pMember != NULL)
			{
				CPC* pPC = pMember->GetMemberPCPtr();
				if(pPC != NULL && pPC->m_index>0 && pPC->m_level>0 &&
						pPC->m_pZone && pPC->m_pArea && pPC->m_pZone->m_index == nZone && pPC->m_pArea->m_index == nArea)
				{
					SEND_Q(msg, pPC->m_desc);
				}
			}
		}
	}
}

void CExpedition::SendToPCInSameGroup(int nGroup, CNetMsg::SP& msg)
{
	if(nGroup < 0 ||  nGroup >= MAX_EXPED_GROUP) return;

	for(int j=0; j < MAX_EXPED_GMEMBER; j++)
	{
		if (m_listMember[nGroup][j] != NULL
				&& m_listMember[nGroup][j]->GetMemberPCPtr())
		{
			SEND_Q(msg, m_listMember[nGroup][j]->GetMemberPCPtr()->m_desc);
		}
	}
}

CPC* CExpedition::GetNearMember(CCharacter* pPC, int nGroup,int nListIndex)
{
	if (nListIndex < 0 || nListIndex >= MAX_EXPED_GMEMBER)
		return NULL;

	if (m_listMember[nGroup][nListIndex] == NULL)
		return NULL;

	if (m_listMember[nGroup][nListIndex]->GetMemberPCPtr() == NULL)
		return NULL;

	if (!CheckInNearCellExt(pPC, m_listMember[nGroup][nListIndex]->GetMemberPCPtr()))
		return NULL;

	return m_listMember[nGroup][nListIndex]->GetMemberPCPtr();
}

CPC* CExpedition::GetMemberPCPtr(int nGroup, int nListIndex)
{
	if (nGroup < 0 || nGroup >= MAX_EXPED_GROUP)
		return NULL;

	if (nListIndex < 0 || nListIndex >= MAX_EXPED_GMEMBER)
		return NULL;

	if (m_listMember[nGroup][nListIndex] == NULL)
		return NULL;

	return m_listMember[nGroup][nListIndex]->GetMemberPCPtr();
}

void CExpedition::SetMemberPCPtr(int nCharIndex, CPC* pPC)
{
	int i;
	for (i = 0; i < MAX_EXPED_GROUP; i++)
	{
		for(int j=0; j < MAX_EXPED_GMEMBER; j++)
		{
			if (m_listMember[i][j] != NULL
					&& m_listMember[i][j]->GetCharIndex() == nCharIndex
			   )
			{
				m_listMember[i][j]->SetMemberPCPtr(pPC);
				return ;
			}
		}
	}
	return ;
}

// 원정대 해체 시간 설정 (20초 경과 후 해체)
void CExpedition::SetEndExpedTime(int nNow)
{
	m_nEndExpedTime = nNow;
}

// 원정대 해체 시간 획득 (20초 경과 후 해체)
int CExpedition::GetEndExpedTime()
{
	return m_nEndExpedTime;
}

bool CExpedition::SetMemberRegister_AfterGoZone( CPC* pPC )
{
	if (!pPC || !pPC->m_desc)
		return false;

	SetMemberPCPtr(pPC->m_index, pPC);

	{
		//원정대 타입 정보
		CNetMsg::SP rmsg(new CNetMsg);
		ExpedTypeinfo(rmsg, GetExpedType(MSG_DIVITYPE_EXP), GetExpedType(MSG_DIVITYPE_ITEM), GetExpedType(MSG_DIVITYPE_SPECIAL));
		SEND_Q(rmsg, pPC->m_desc);
	}

	// 처음 접속에 원정대가 있으면
	for(int i = 0; i < MAX_EXPED_GROUP; i++)
	{
		for(int j = 0; j < MAX_EXPED_GMEMBER; j++)
		{
			const CExpedMember* pMember = GetMemberByListIndex(i,j);

			// 모두 전송
			if (pMember)
			{
				//대원들 정보 ==> 나에게 전송
				CNetMsg::SP rmsg(new CNetMsg);
				ExpedAddMsg(rmsg, pMember->GetCharIndex(), pMember->GetCharName(), i, pMember->GetMemberType(), pMember->GetListIndex(), pMember->GetMemberPCPtr());
				SEND_Q(rmsg, pPC->m_desc);
			}
		}
	}

	//내 정보 ==> 다른 대원에게 전송
	const CExpedMember* pMember = GetMemberByCharIndex(pPC->m_index);
	if(pMember)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ExpedAddMsg(rmsg, pMember->GetCharIndex(), pMember->GetCharName(), pMember->GetGroupType(), pMember->GetMemberType(), pMember->GetListIndex(), pMember->GetMemberPCPtr());
		SendToAllPC(rmsg, pPC->m_index);
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperExpendOnlineMsg( rmsg, GetBossIndex() , pPC->m_index , pPC->m_level );
		SEND_Q(rmsg, gserver->m_helper);
	}

	return true;
}

// 게임 서버 전용 함수들
//////////////////////////////////////////////////////////////////////

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
	// 게임 서버에서는 요청자를 찾아 파티 정보를 리셋한다.
	if (m_nRequestIndex != -1)
	{
		CPC* pPC = PCManager::instance()->getPlayerByCharIndex(m_nRequestIndex);
		if (pPC)
			pPC->m_Exped = NULL;
	}
	m_nRequestIndex = nRequest;
	m_strRequestName = strRequest;
}

CExpedMember* CExpedition::JoinRequest(const char* strRequestName, int nMemberType, CPC* pRequest)
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
	m_listMember[i][j] = new CExpedMember(m_nRequestIndex, strRequestName, i, nMemberType, j, pRequest);

	m_nTotCount++;
	m_nGTotCount[i]++;

	// request 초기화 하고
	m_nRequestIndex = -1;

	return m_listMember[i][j];
}

// 멤버 참여
CExpedMember* CExpedition::Join(int nCharIndex, const char* strCharName, int nMemberType, CPC* pRequest)
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
	m_listMember[i][j] = new CExpedMember(nCharIndex, strCharName, i, nMemberType, j, pRequest);

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
		gserver->m_listExped.erase(nOldBossIndex);
		gserver->m_listExped.insert(map_listexped_t::value_type(nTargetIndex, this));
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

// 원정대 종료 함수,  게임 서버 전용과 헬퍼 전용 함수가 다르다.
void CExpedition::SetEndExped()
{
	CExpedMember* pMember=NULL;
	CPC* pc=NULL;
	int i,j;

	// 레이드 안이면 인존 바깥에 시작 지점에 위치 하게 처리
	for(i=0; i < MAX_EXPED_GROUP; i++)
	{
		for(j=0; j < MAX_EXPED_GMEMBER; j++)
		{
			pMember = (CExpedMember*) GetMemberByListIndex(i,j);
			if(pMember)
			{
				pc = TO_PC(pMember->GetMemberPCPtr());
				if( pc && pc->m_index == -1 ) pc = NULL;
				if(pc && pc->m_nJoinInzone_ZoneNo >=0 && pc->m_nJoinInzone_RoomNo >= 0)
				{
					// 레이드존을 나가야 되는데 앉아있어서 못나가는 버그 수정
					if(pc->IsSetPlayerState(PLAYER_STATE_SITDOWN))
					{
						pc->TogglePlayerState(PLAYER_STATE_SITDOWN);
						pc->CalcStatus(true);
					}
					{
						CNetMsg::SP rmsg(new CNetMsg);
						RaidInzoneQuitReq(rmsg,1,0);
						do_Raid(pc, rmsg);
					}
				}
			}
		}
	}

	{
		// 원정대 해체 알림
		CNetMsg::SP rmsg(new CNetMsg);
		ExpedMsg(rmsg, MSG_ENDEXPED_REP);
		SendToAllPC(rmsg);
	}

	// 초대 거절
	if (GetRequestIndex() > 0)
	{
		CPC* pRequestPC = PCManager::instance()->getPlayerByCharIndex(GetRequestIndex());
		if (pRequestPC)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			ExpedMsg(rmsg, MSG_REJECT_SRC);
			SEND_Q(rmsg, pRequestPC->m_desc);
		}
		SetRequest(-1, "");
	}

	// 모든 맴버의 메모리는 NULL로 바꾸어준다.
	for(i=0; i < MAX_EXPED_GROUP; i++)
	{
		for(j=0; j < MAX_EXPED_GMEMBER; j++)
		{
			CExpedMember* pMember = (CExpedMember*)GetMemberByListIndex(i,j);
			if(pMember)
			{
				SetMemberPCPtr(pMember->GetCharIndex(), NULL);
				DeleteMember(pMember->GetCharIndex());
			}
		}
	}
}

void CExpedition::SendToExpedTargetClear(CCharacter *ch)
{
	int i;
	CExpedMember* pMember = NULL;

	for (i = 0; i < MAX_EXPED_GROUP; i++)
	{
		for(int j=0; j < MAX_EXPED_GMEMBER; j++)
		{
			pMember = (CExpedMember*)m_listMember[i][j];
			if ( pMember != NULL)
			{
				CPC* pPC = pMember->GetMemberPCPtr();
				if(pPC != NULL && pPC->m_index>0 && pPC->m_level>0 &&
					pPC->m_pZone && pPC->m_pArea && pPC->m_pZone->m_index == ch->m_pZone->m_index && pPC->m_pArea->m_index == ch->m_pArea->m_index)
				{
					pPC->m_targetPC == NULL;
					CNetMsg::SP rmsg(new CNetMsg);
					TargetClearMsg(rmsg, ch->m_index);
					SEND_Q(rmsg, pPC->m_desc);
				}
			}
		}
	}
}
