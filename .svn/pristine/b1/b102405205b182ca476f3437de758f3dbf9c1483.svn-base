#include "stdhdrs.h"

#include "Server.h"
#include "Log.h"
#include "doFunc.h"
#include "CmdMsg.h"

void do_ExPartyMatch_RegMemberReq(CPC* ch, CNetMsg::SP& msg);
void do_ExPartyMatch_RegPartyReq(CPC* ch, CNetMsg::SP& msg);
void do_ExPartyMatchMemberListReq(CPC* ch, CNetMsg::SP& msg);
void do_ExPartyMatchPartyListReq(CPC* ch, CNetMsg::SP& msg);
void do_ExPartyMatchDelReq(CPC* ch, CNetMsg::SP& msg);
void do_ExPartyMatchInviteReq(CPC* ch, CNetMsg::SP& msg);
void do_ExPartyMatchJoinReq(CPC* ch, CNetMsg::SP& msg);
void do_ExPartyMatchJoinAllow(CPC* ch, CNetMsg::SP& msg);
void do_ExPartyMatchJoinReject(CPC* ch, CNetMsg::SP& msg);

void do_ExPartyMatch(CPC* ch, CNetMsg::SP& msg)
{
	int nSubType = -1;

	RefMsg(msg) >> nSubType;

	switch (nSubType)
	{
	case MSG_EX_PARTY_MATCH_REG_MEMBER_REQ:
		do_ExPartyMatch_RegMemberReq(ch, msg);
		break;

	case MSG_EX_PARTY_MATCH_REG_PARTY_REQ:
		do_ExPartyMatch_RegPartyReq(ch, msg);
		break;

	case MSG_EX_PARTY_MATCH_MEMBER_LIST_REQ:
		do_ExPartyMatchMemberListReq(ch, msg);
		break;

	case MSG_EX_PARTY_MATCH_PARTY_LIST_REQ:
		do_ExPartyMatchPartyListReq(ch, msg);
		break;

	case MSG_EX_PARTY_MATCH_DEL_REQ:
		do_ExPartyMatchDelReq(ch, msg);
		break;

	case MSG_EX_PARTY_MATCH_INVITE_REQ:
		do_ExPartyMatchInviteReq(ch, msg);
		break;

	case MSG_EX_PARTY_MATCH_JOIN_REQ:
		do_ExPartyMatchJoinReq(ch, msg);
		break;

	case MSG_EX_PARTY_MATCH_JOIN_ALLOW:
		do_ExPartyMatchJoinAllow(ch, msg);
		break;

	case MSG_EX_PARTY_MATCH_JOIN_REJECT:
		do_ExPartyMatchJoinReject(ch, msg);
		break;
	}
}

void do_ExPartyMatch_RegMemberReq(CPC* ch, CNetMsg::SP& msg)
{
	if (ch->m_pZone == NULL)
		return ;

	char nPartyType = -1;

	RefMsg(msg) >> nPartyType;

	switch (nPartyType)
	{
	case MSG_PARTY_TYPE_RANDOM:
	case MSG_PARTY_TYPE_FIRSTGET:
	case MSG_PARTY_TYPE_BATTLE:
		break;
	default:
		return ;
	}

	if( ch->IsParty()
			|| ch->IsExped()
			|| ch->m_pZone->IsPersonalDungeon()
			|| ch->m_pZone->IsWarGroundZone()
			|| ch->m_pZone->IsPersonalRaid()
	  )
		return;

	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperPartyMatchRegMemberReqMsg(rmsg, ch->m_index, ch->GetName(), ch->m_level, ch->m_pZone->m_index, ch->m_job, nPartyType);
		SEND_Q(rmsg, gserver->m_helper);
	}
}

void do_ExPartyMatch_RegPartyReq(CPC* ch, CNetMsg::SP& msg)
{
	if (ch->m_pZone == NULL)
		return ;

	CLCString		strComment(PARTY_MATCHING_COMMENT + 1);
	int				nJobFlag = 0;
	char			cLimitLevel = 0;

	// 파티중이고 파장
	if (ch->m_party == NULL || ch->m_party->GetBossIndex() != ch->m_index)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ExPartyMatchRegPartyRepMsg(rmsg, MSG_EX_PARTY_MATCH_ERROR_REG_PARTY_NO_BOSS);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if ( ch->IsExped() )
	{
		return;
	}

	RefMsg(msg) >> strComment
				>> nJobFlag
				>> cLimitLevel;

	if (nJobFlag == 0)
	{
		nJobFlag = (1 << JOB_TITAN)
				   | (1 << JOB_KNIGHT)
				   | (1 << JOB_HEALER)
				   | (1 << JOB_MAGE)
				   | (1 << JOB_ROGUE)
				   | (1 << JOB_SORCERER);
#ifdef EX_ROGUE
		nJobFlag |= (1 << JOB_NIGHTSHADOW);
		nJobFlag |= (1 << JOB_EX_ROGUE);
#endif // EX_ROGUE
#ifdef EX_MAGE
		nJobFlag |= (1 << JOB_EX_MAGE);
#endif // EX_MAGE
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperPartyMatchRegPartyReqMsg(rmsg, ch->m_index, ch->GetName(), ch->m_level, ch->m_pZone->m_index, nJobFlag, cLimitLevel, strComment);
		SEND_Q(rmsg, gserver->m_helper);
	}
}

void do_ExPartyMatchMemberListReq(CPC* ch, CNetMsg::SP& msg)
{
	int		nPageNo;
	char	cJob;
	char	cLimitLevel;
	char	cPartyType;

	RefMsg(msg) >> nPageNo
				>> cJob
				>> cLimitLevel
				>> cPartyType;

	int		nSkipCount = nPageNo * PARTY_MATCHING_DATA_PER_PAGE;
	std::vector<CPartyMatchMember*> list;
	map_listPartyMatchMember_t::iterator it = gserver->m_listPartyMatchMember.begin();
	map_listPartyMatchMember_t::iterator endit = gserver->m_listPartyMatchMember.end();
	for(; it != endit; ++it)
	{
		CPartyMatchMember* pMatchMember = it->second;
		bool bValidCondition = true;

		// 직업이 -1이 아니면 특정 직업
		if (cJob != -1 && pMatchMember->GetJob() != cJob)
			bValidCondition = false;

		// 레벨 제한은 2레벨까지
		if (cLimitLevel && ABS(pMatchMember->GetLevel() - ch->m_level) > PARTY_MATCHING_LIMIT_LEVEL)
			bValidCondition = false;

		// 희망 파티 검사, -1이면 전체
		if (cPartyType != -1 && pMatchMember->GetPartyType() != cPartyType)
			bValidCondition = false;

		// 조건에 맞을 때, page만큼 넘기고 전달
		if (bValidCondition)
		{
			if (nSkipCount > 0)
			{
				nSkipCount--;
			}
			else
			{
				list.push_back(pMatchMember);
			}
		}

		if (list.size() == PARTY_MATCHING_DATA_PER_PAGE)
			break;
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		ExPartyMatchMemberListRepMsg(rmsg, nPageNo, ch->m_level, list);
		SEND_Q(rmsg, ch->m_desc);
	}
}

void do_ExPartyMatchPartyListReq(CPC* ch, CNetMsg::SP& msg)
{
	int		nPageNo;
	char	cJob;
	char	cLimitLevel;
	char	cPartyType;

	RefMsg(msg) >> nPageNo
				>> cJob
				>> cLimitLevel
				>> cPartyType;

	int		nSkipCount = nPageNo * PARTY_MATCHING_DATA_PER_PAGE;
	std::vector<CPartyMatchParty*> list;
	map_listPartyMatchParty_t::iterator it = gserver->m_listPartyMatchParty.begin();
	map_listPartyMatchParty_t::iterator endit = gserver->m_listPartyMatchParty.end();
	for(; it != endit; ++it)
	{
		CPartyMatchParty* pMatchParty = it->second;
		bool bValidCondition = true;

		// 직업이 -1이 아니면 특정 직업
		if (cJob != -1 && (pMatchParty->GetJobFlag() & (1 << cJob)) == 0)
			bValidCondition = false;

		// 레벨 제한은 2레벨까지
		if (cLimitLevel && ABS(pMatchParty->GetBossLevel() - ch->m_level) > PARTY_MATCHING_LIMIT_LEVEL)
			bValidCondition = false;

		// 희망 파티 검사, -1이면 전체
		if (cPartyType != -1 && pMatchParty->GetPartyType() != cPartyType)
			bValidCondition = false;

		// 조건에 맞을 때, page만큼 넘기고 전달
		if (bValidCondition)
		{
			if (nSkipCount > 0)
			{
				nSkipCount--;
			}
			else
			{
				list.push_back(pMatchParty);
			}
		}

		if (list.size() == PARTY_MATCHING_DATA_PER_PAGE)
			break;
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		ExPartyMatchPartyListRepMsg(rmsg, nPageNo, ch->m_level, list);
		SEND_Q(rmsg, ch->m_desc);
	}
}

void do_ExPartyMatchDelReq(CPC* ch, CNetMsg::SP& msg)
{
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperPartyMatchDelReqMsg(rmsg, ch->m_index);
		SEND_Q(rmsg, gserver->m_helper);
	}
}

void do_ExPartyMatchInviteReq(CPC* ch, CNetMsg::SP& msg)
{
	int	nCharIndex;

	RefMsg(msg) >> nCharIndex;

	if (nCharIndex == ch->m_index)
		return ;

	if (ch->IsExped())
		return ;
	CPC* tagetPC = PCManager::instance()->getPlayerByCharIndex(nCharIndex);

	if(!tagetPC)
		return ;

	if( tagetPC->IsExped() )
		return ;

	if(	!tagetPC->m_pZone || !ch->m_pZone)
		return;

	if(	tagetPC->m_pZone->IsPersonalDungeon() ||
			tagetPC->m_pZone->IsWarGroundZone() ||
			ch->m_pZone->IsPersonalDungeon()
			|| ch->m_pZone->IsWarGroundZone()
			|| ch->m_pZone->IsPersonalRaid()
	  )
		return ;

	if (ch->m_pZone->isRVRZone())
	{
		if (!tagetPC->m_pZone->isRVRZone())
		{
			LOG_INFO("Target is no RVR ZONE.");
			return;
		}

		if (ch->getSyndicateType() != tagetPC->getSyndicateType())
		{
			LOG_INFO("Target is another team.");
			return;
		}
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperPartyMatchInviteReqMsg(rmsg, ch->m_index, ch->GetName(), ch->m_level, nCharIndex);
		SEND_Q(rmsg, gserver->m_helper);
	}
}

void do_ExPartyMatchJoinReq(CPC* ch, CNetMsg::SP& msg)
{
	int			nBossIndex;

	RefMsg(msg) >> nBossIndex;

	if (ch->m_party)
		return ;

	if (ch->m_Exped)
		return ;

	CExpedition* pExped = gserver->FindExpedByBossIndex(nBossIndex);
	if(pExped)
		return ;

	// 참가 요청자가 퍼스널던전, 로얄럼블, 망각의 신전에 있으면 리턴
	if( ch->m_pZone->IsPersonalDungeon()
			|| ch->m_pZone->IsWarGroundZone()
			|| ch->m_pZone->IsPersonalRaid()
	  )
		return ;

	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperPartyMatchJoinReqMsg(rmsg, nBossIndex, ch->m_index, ch->GetName(), ch->m_level, ch->m_job);
		SEND_Q(rmsg, gserver->m_helper);
	}
}

void do_ExPartyMatchJoinAllow(CPC* ch, CNetMsg::SP& msg)
{
	int	nReqCharIndex;

	RefMsg(msg) >> nReqCharIndex;

	CPC* pReqChar = PCManager::instance()->getPlayerByCharIndex(nReqCharIndex);
	if(!pReqChar)
		return ;

	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperPartyMatchJoinAllowReqMsg(rmsg, ch->m_index, nReqCharIndex, ch->m_level, pReqChar->m_level );
		SEND_Q(rmsg, gserver->m_helper);
	}
}

void do_ExPartyMatchJoinReject(CPC* ch, CNetMsg::SP& msg)
{
	int			nReqCharIndex;

	RefMsg(msg) >> nReqCharIndex;

	{
		CNetMsg::SP rmsg(new CNetMsg);
		if (nReqCharIndex)
			HelperPartyMatchJoinRejectReqMsg(rmsg, nReqCharIndex, ch->m_index);
		SEND_Q(rmsg, gserver->m_helper);
	}
}

