#include "stdhdrs.h"

#include "Log.h"
#include "Character.h"
#include "Server.h"
#include "CmdMsg.h"
#include "doFunc.h"
#include "WarCastle.h"

/////////////////
// 원정대 관련 함수
void do_Expedition(CPC* ch, CNetMsg::SP& msg)
{
	// 안보이면 무시
	if (!ch->m_bVisible)
		return ;

	unsigned char subtype;

	msg->MoveFirst();

	RefMsg(msg) >> subtype;

	switch (subtype)
	{
	case MSG_CREATE_REQ:						// 파티 => 원정대 전환
		do_ExpedCreateReq(ch, msg);
		break;

	case MSG_INVITE_REQ:						// 초대 요청
		do_ExpedInviteReq(ch, msg);
		break;

	case MSG_ALLOW_REQ:							// 초대 수락
		do_ExpedAllowReq(ch, msg);
		break;

	case MSG_REJECT_REQ:						// 초대 거부 요청
		do_ExpedRejectReq(ch, msg);
		break;

	case MSG_QUIT_REQ:							// 탈퇴 요청
		do_ExpedQuitReq(ch, msg);
		break;

	case MSG_ENDEXPED_REQ:						// 원정대 해체
		do_ExpedEndExpedReq(ch, msg);
		break;

	case MSG_KICK_REQ:							// 추방 요청
		do_ExpedKickReq(ch, msg);
		break;

	case MSG_CHANGETYPE_REQ:					// 원정대 타입 변경
		do_ExpedChangeTypeReq(ch, msg);
		break;

	case MSG_CHANGEBOSS_REQ:					// 원정 대장 위임(변경)
		do_ExpedChangeBossReq(ch, msg);
		break;

	case MSG_SETMBOSS_REQ:						// 부대장 임명
		do_ExpedSetMBossReq(ch, msg);
		break;

	case MSG_RESETMBOSS_REQ:					// 부대장 임명 해제
		do_ExpedResetMBossReq(ch, msg);
		break;

	case MSG_MOVEGROUP_REQ:						// 그룹 이동
		do_ExpedMoveGroupReq(ch, msg);
		break;

	case MSG_ADDMEMBER_REQ:						// 대원 추가
		do_ExpedAddMemberReq(ch, msg);
		break;

	case MSG_VIEWDETAIL_REQ:					// 살펴보기
		do_ExpedViewDetailReq(ch, msg);
		break;

	case MSG_SET_LABEL_REQ:						// 표식 지정
		do_ExpedSetLabelReq(ch, msg);
		break;
	case MSG_QUESTITEM_CHECK_REQ:
		do_ExpedSearchItemReq(ch, msg);			// Collect Trigger item - 원정대 Trigger 아이템 수집(원정대장 전용 기능)
		break;

	case MSG_EXPEND_OFFLINE:
		do_ExpendOffline(ch, msg);
		break;
	}
}

//원정대 생성(파티 전환)(대장)
void do_ExpedCreateReq(CPC* ch, CNetMsg::SP& msg)
{
	GAMELOG << init("EXPED DEBUG CREATE REQ", ch)
			<< end;

	if(ch->m_Exped)
		return ;
#ifdef BLOCK_CHANGE_EXPED_IN_PARTY_RAID_ZONE
	if(ch->m_pZone->IsPartyRaidZone())
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ExpedErrorMsg(rmsg, MSG_EXPED_ERROR_CREATE_INVALIDZONE);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
#endif
	//파티에 소속되어 있는가?
	if (ch->m_party == NULL)
	{
		// 파티에 소속되지 않음(파티 전환 불가)
		CNetMsg::SP rmsg(new CNetMsg);
		ExpedErrorMsg(rmsg, MSG_EXPED_ERROR_NOT_PARTY);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	//파티장인가?
	if(ch->m_party->GetBossIndex() != ch->m_index)
	{
		// 파티장이 아니다.	(파티전환 불가)
		CNetMsg::SP rmsg(new CNetMsg);
		ExpedErrorMsg(rmsg, MSG_EXPED_ERROR_NOT_PARTYBOSS);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

#ifdef EXTREME_CUBE
	if ( ch->m_party->m_cubeUniqueIdx > 0 )
	{
		return ;
	}
#endif

	//파티 맴버 중 한명이라도 로그아웃 되어 있으면 원정대 전환 불가.
	int onlineCount = ch->m_party->GetMemberCountOnline();
	int totalCount = ch->m_party->GetMemberCount();
	if( onlineCount != totalCount )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ExpedErrorMsg(rmsg, MSG_EXPED_ERROR_EXIST_LOGOUT_MEMBER);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperExpedCreateReqMsg(rmsg, ch->m_party->GetBossIndex(),ch->GetName());
		SEND_Q(rmsg, gserver->m_helper);
	}
}

//원정대 초대(대장)
void do_ExpedInviteReq(CPC* ch, CNetMsg::SP& msg)
{
	int destindex;

	RefMsg(msg) >>  destindex;

	GAMELOG << init("EXPED DEBUG INVITE REQ", ch)
			<< "DESTINDEX" << delim << destindex
			<< end;

	if (ch->m_index == destindex)
		return ;

	CPC * pTargetPC = PCManager::instance()->getPlayerByCharIndex(destindex);
	if(pTargetPC == NULL)
		return;

	if(pTargetPC->m_party != NULL)
	{
		//에러: 다른 그룹에 소속되어 있음
		CNetMsg::SP rmsg(new CNetMsg);
		ExpedErrorMsg(rmsg, MSG_EXPED_ERROR_ALREADY_JOIN_OTHER);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if (ch->m_Exped == NULL)
	{
		//에러: 원정대에 소속 되어 있지 않음
		CNetMsg::SP rmsg(new CNetMsg);
		ExpedErrorMsg(rmsg, MSG_EXPED_ERROR_NOT_EXPED);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if(ch->m_Exped->GetMemberCount() >= MAX_EXPED_MEMBER)
	{
		//에러: 더 이상 원정대원 추가할 수 없음
		CNetMsg::SP rmsg(new CNetMsg);
		ExpedErrorMsg(rmsg, MSG_EXPED_ERROR_FULL_EXPED);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if(ch->IsSetPlayerState(PLAYER_STATE_PKMODE) || pTargetPC->IsSetPlayerState(PLAYER_STATE_PKMODE))
	{
		return ;
	}

	//권한 체크
	//-->
	CExpedMember* pMember = NULL;
	pMember = (CExpedMember*) ch->m_Exped->GetMemberByCharIndex(ch->m_index);
	if(!pMember)
		return;

	if(pMember && ((pMember->GetMemberType() != MSG_EXPED_MEMBERTYPE_BOSS) && (pMember->GetMemberType() != MSG_EXPED_MEMBERTYPE_MBOSS)) )
	{
		//에러: 권한 실패
		CNetMsg::SP rmsg(new CNetMsg);
		ExpedErrorMsg(rmsg, MSG_EXPED_ERROR_NOT_EXPEDBOSS);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}
	//<--

	if(pTargetPC->m_Exped  && ch->m_Exped != pTargetPC->m_Exped)
	{
		//에러: 이미 원정대에 소속되어 있습니다.
		CNetMsg::SP rmsg(new CNetMsg);
		ExpedErrorMsg(rmsg, MSG_EXPED_ERROR_ALREADY_JOIN_ME);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	CPC* pBossPC = ch;
	// 요청 캐릭터가 부대장이면 원정대장 찾아서 pBossPC에 대입
	if (pMember->GetMemberType() == MSG_EXPED_MEMBERTYPE_MBOSS)
	{
		int nBossIndex = ch->m_Exped->GetBossIndex();
		pBossPC = PCManager::instance()->getPlayerByCharIndex(nBossIndex);

		if( !pBossPC )
		{
			CNetMsg::SP rmsg(new CNetMsg);
			ExpedErrorMsg(rmsg, MSG_EXPED_ERROR_NOT_EXPEDBOSS);
			SEND_Q(rmsg, ch->m_desc);
			return;
		}
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperExpedInviteReqMsg(rmsg, pBossPC->m_index, pBossPC->GetName(), destindex,pTargetPC->GetName());
		SEND_Q(rmsg, gserver->m_helper);
	}
}

//초대 수락(모두)
void do_ExpedAllowReq(CPC* ch, CNetMsg::SP& msg)
{
	// 원정대 정보가 있고
	GAMELOG << init("EXPED DEBUG ALLOW REQ", ch)
			<< end;

	if(ch->m_Exped == NULL)
	{
		//에러: 원정대에 소속 되어 있지 않음
		CNetMsg::SP rmsg(new CNetMsg);
		ExpedErrorMsg(rmsg, MSG_EXPED_ERROR_NOT_EXPED);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperExpedAllowReqMsg(rmsg, ch->m_Exped->GetBossIndex(), ch->m_index, ch->GetName(), ch->m_level );
		SEND_Q(rmsg, gserver->m_helper);
	}

	{
		// 파티 매칭에서 제거
		CNetMsg::SP rmsg(new CNetMsg);
		rmsg->Init(MSG_EXTEND);
		RefMsg(rmsg) << MSG_EX_PARTY_MATCH
					 << MSG_EX_PARTY_MATCH_DEL_REQ;
		do_Extend(ch, rmsg);
	}
}

//초대 거부(모두)
void do_ExpedRejectReq(CPC* ch, CNetMsg::SP& msg)
{
	GAMELOG << init("EXPED DEBUG REJECT REQ", ch)
			<< end;

	if(ch->m_Exped == NULL)
	{
		//에러: 원정대에 소속 되어 있지 않음
		CNetMsg::SP rmsg(new CNetMsg);
		ExpedErrorMsg(rmsg, MSG_EXPED_ERROR_NOT_EXPED);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	CNetMsg::SP rmsg(new CNetMsg);
	HelperExpedRejectReqMsg(rmsg, ch->m_Exped->GetBossIndex(), ch->m_index);
	SEND_Q(rmsg, gserver->m_helper);
}

//탈퇴(모두)
void do_ExpedQuitReq(CPC* ch, CNetMsg::SP& msg)
{
	int nQuitMode;			// 정상,비정상 구분

	RefMsg(msg) >> nQuitMode;

	GAMELOG << init("EXPED DEBUG QUIT REQ", ch)
			<< "QUITMODE" << delim << nQuitMode
			<< end;

	if(ch->m_Exped == NULL)
	{
		//에러: 원정대에 소속 되어 있지 않음
		CNetMsg::SP rmsg(new CNetMsg);
		ExpedErrorMsg(rmsg, MSG_EXPED_ERROR_NOT_EXPED);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperExpedQuitReqMsg(rmsg, ch->m_Exped->GetBossIndex(), ch->m_index, nQuitMode);
		SEND_Q(rmsg, gserver->m_helper);
	}
}

//원정대 해체(대장)(20초 대기)
void do_ExpedEndExpedReq(CPC* ch, CNetMsg::SP& msg)
{
	GAMELOG << init("EXPED DEBUG ENDEXPED REQ", ch)
			<< end;

	if (ch->m_Exped == NULL)
	{
		//에러: 원정대에 소속 되어 있지 않음
		CNetMsg::SP rmsg(new CNetMsg);
		ExpedErrorMsg(rmsg, MSG_EXPED_ERROR_NOT_EXPED);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	//권한 체크
	//-->
	CExpedMember* pMember = NULL;
	pMember = (CExpedMember*) ch->m_Exped->GetMemberByCharIndex(ch->m_index);
	if(!pMember)
		return;

	if(pMember && (pMember->GetMemberType() != MSG_EXPED_MEMBERTYPE_BOSS))
	{
		//에러: 권한 실패
		CNetMsg::SP rmsg(new CNetMsg);
		ExpedErrorMsg(rmsg, MSG_EXPED_ERROR_NOT_EXPEDBOSS);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}
	//<--

	// 원정대 해체 시간 설정
	//-->
	time_t t_now;
	t_now = time(NULL);

	ch->m_Exped->SetEndExpedTime(t_now);
	//<--

	{
		// 원정대원 전체에게 20초 후에 해체 됨 알림
		CNetMsg::SP rmsg(new CNetMsg);
		ExpedEndExpedStartMsg(rmsg);
		ch->m_Exped->SendToAllPC(rmsg);
	}
}

//추방(대장,부대장)
void do_ExpedKickReq(CPC* ch, CNetMsg::SP& msg)
{
	int nTargetIndex;
	RefMsg(msg) >> nTargetIndex;

	GAMELOG << init("EXPED DEBUG KICK REQ", ch)
			<< "TARGETINDEX" << delim << nTargetIndex
			<< end;

	if (ch->m_Exped == NULL)
	{
		//에러: 원정대에 소속 되어 있지 않음
		CNetMsg::SP rmsg(new CNetMsg);
		ExpedErrorMsg(rmsg, MSG_EXPED_ERROR_NOT_EXPED);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	if (ch->m_index == nTargetIndex)
		return ;

	//권한 체크
	//-->
	CExpedMember* pMember = NULL;
	pMember = (CExpedMember*) ch->m_Exped->GetMemberByCharIndex(ch->m_index);
	if(!pMember)
		return;
	if(pMember && (pMember->GetMemberType() != MSG_EXPED_MEMBERTYPE_BOSS && pMember->GetMemberType() != MSG_EXPED_MEMBERTYPE_MBOSS ))
	{
		//에러: 권한 실패
		CNetMsg::SP rmsg(new CNetMsg);
		ExpedErrorMsg(rmsg, MSG_EXPED_ERROR_NOT_EXPEDBOSS);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}
	if(ch->m_Exped->GetBossIndex() == nTargetIndex)
	{
		//에러: 권한 실패
		CNetMsg::SP rmsg(new CNetMsg);
		ExpedErrorMsg(rmsg, MSG_EXPED_ERROR_NOT_EXPEDBOSS);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperExpedKickReqMsg(rmsg, ch->m_Exped->GetBossIndex(), nTargetIndex);
		SEND_Q(rmsg, gserver->m_helper);
	}
}

//원정 대장 위임(대장)
void do_ExpedChangeBossReq(CPC* ch, CNetMsg::SP& msg)
{
	int nChangeMode, nNewBossIndex;

	RefMsg(msg) >> nChangeMode						// 수동,자동
				>> nNewBossIndex;					// 자동(-1)

	GAMELOG << init("EXPED DEBUG CHANGEBOSS REQ", ch)
			<< "CHANGEMODE" << delim << nChangeMode << delim
			<< "NEWBOSSINDEX" << delim << nNewBossIndex << delim
			<< end;

	if (ch->m_Exped == NULL)
	{
		//에러: 원정대에 소속 되어 있지 않음
		CNetMsg::SP rmsg(new CNetMsg);
		ExpedErrorMsg(rmsg, MSG_EXPED_ERROR_NOT_EXPED);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	//권한 체크
	//-->
	CExpedMember* pMember = NULL;
	pMember = (CExpedMember*) ch->m_Exped->GetMemberByCharIndex(ch->m_index);
	if(!pMember)
		return;

	if(pMember && (pMember->GetMemberType() != MSG_EXPED_MEMBERTYPE_BOSS))
	{
		//에러: 권한 실패
		CNetMsg::SP rmsg(new CNetMsg);
		ExpedErrorMsg(rmsg, MSG_EXPED_ERROR_NOT_EXPEDBOSS);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}
	//<--

	//자기 자신 위임 체크
	if(ch->m_index == nNewBossIndex)
		return;

	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperExpedChangeBossReqMsg(rmsg, ch->m_index, nChangeMode, nNewBossIndex);
		SEND_Q(rmsg, gserver->m_helper);
	}
}

//타입 변경(대장)
void do_ExpedChangeTypeReq(CPC* ch, CNetMsg::SP& msg)
{
	char cExpedType, cDiviType;
	RefMsg(msg) >> cExpedType
				>> cDiviType;

	GAMELOG << init("EXPED DEBUG CHANGETYPE REQ", ch)
			<< "EXPEDTYPE" << delim << cExpedType << delim
			<< "DIVITYPE" << delim << cDiviType << delim
			<< end;

	if (ch->m_Exped == NULL)
	{
		//에러: 원정대에 소속 되어 있지 않음
		CNetMsg::SP rmsg(new CNetMsg);
		ExpedErrorMsg(rmsg, MSG_EXPED_ERROR_NOT_EXPED);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	//권한 체크
	//-->
	CExpedMember* pMember = NULL;
	pMember = (CExpedMember*) ch->m_Exped->GetMemberByCharIndex(ch->m_index);
	if(!pMember)
		return;

	if(pMember && (pMember->GetMemberType() != MSG_EXPED_MEMBERTYPE_BOSS))
	{
		//에러: 권한 실패
		CNetMsg::SP rmsg(new CNetMsg);
		ExpedErrorMsg(rmsg, MSG_EXPED_ERROR_NOT_EXPEDBOSS);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}
	//<--

	if(ch->m_Exped->GetExpedType(cDiviType) ==  cExpedType)
		return ;

	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperExpedChangeTypeReqMsg(rmsg, ch->m_index, cExpedType, cDiviType);
		SEND_Q(rmsg, gserver->m_helper);
	}
}

//부대장 임명(대장)
void do_ExpedSetMBossReq(CPC* ch, CNetMsg::SP& msg)
{
	int nNewMBossIndex;

	RefMsg(msg) >> nNewMBossIndex;

	GAMELOG << init("EXPED DEBUG SETMBOSS REQ", ch)
			<< "NEW MBOSS INDEX" << delim << nNewMBossIndex
			<< end;

	if (ch->m_Exped == NULL)
	{
		//에러: 원정대에 소속 되어 있지 않음
		CNetMsg::SP rmsg(new CNetMsg);
		ExpedErrorMsg(rmsg, MSG_EXPED_ERROR_NOT_EXPED);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	//권한 체크
	//-->
	CExpedMember* pMember = NULL;
	pMember = (CExpedMember*) ch->m_Exped->GetMemberByCharIndex(ch->m_index);
	if(!pMember)
		return;

	if(pMember && (pMember->GetMemberType() != MSG_EXPED_MEMBERTYPE_BOSS))
	{
		//에러: 권한 실패
		CNetMsg::SP rmsg(new CNetMsg);
		ExpedErrorMsg(rmsg, MSG_EXPED_ERROR_NOT_EXPEDBOSS);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}
	//<--

	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperExpedSetMBossReqMsg(rmsg, ch->m_index, nNewMBossIndex);
		SEND_Q(rmsg, gserver->m_helper);
	}
}

//부대장 해임(대장)
void do_ExpedResetMBossReq(CPC* ch, CNetMsg::SP& msg)
{
	int nNewMBossIndex;

	RefMsg(msg) >> nNewMBossIndex;

	GAMELOG << init("EXPED DEBUG RESETMBOSS REQ", ch)
			<< "NEW MBOSS INDEX" << delim << nNewMBossIndex
			<< end;

	if (ch->m_Exped == NULL)
	{
		//에러: 원정대에 소속 되어 있지 않음
		CNetMsg::SP rmsg(new CNetMsg);
		ExpedErrorMsg(rmsg, MSG_EXPED_ERROR_NOT_EXPED);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	//권한 체크
	//-->
	CExpedMember* pMember = NULL;
	pMember = (CExpedMember*) ch->m_Exped->GetMemberByCharIndex(ch->m_index);
	if(!pMember)
		return;

	if(pMember && (pMember->GetMemberType() != MSG_EXPED_MEMBERTYPE_BOSS))
	{
		//에러: 권한 실패
		CNetMsg::SP rmsg(new CNetMsg);
		ExpedErrorMsg(rmsg, MSG_EXPED_ERROR_NOT_EXPEDBOSS);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}
	//<--

	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperExpedResetMBossReqMsg(rmsg, ch->m_index, nNewMBossIndex);
		SEND_Q(rmsg, gserver->m_helper);
	}
}

//그룹 이동(대장)
void do_ExpedMoveGroupReq(CPC* ch, CNetMsg::SP& msg)
{
	int nSourceGroup, nMoveCharIndex, nTargetGroup, nTargetListindex;

	RefMsg(msg) >> nSourceGroup
				>> nMoveCharIndex
				>> nTargetGroup
				>> nTargetListindex;

	GAMELOG << init("EXPED DEBUG MOVEGROUP REQ", ch)
			<< "SOURCE GROUP" << delim << nMoveCharIndex << delim
			<< "MOVE CHAR INDEX" << delim << nMoveCharIndex << delim
			<< "TARGET GROUP" << delim << nTargetGroup << delim
			<< "TARGET CHAR INDEX" << delim << nTargetListindex
			<< end;

	if (ch->m_Exped == NULL)
	{
		//에러: 원정대에 소속 되어 있지 않음
		CNetMsg::SP rmsg(new CNetMsg);
		ExpedErrorMsg(rmsg, MSG_EXPED_ERROR_NOT_EXPED);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	//권한 체크
	//-->
	CExpedMember* pMember = NULL;
	pMember = (CExpedMember*) ch->m_Exped->GetMemberByCharIndex(ch->m_index);
	if(!pMember)
		return;

	if(pMember && (pMember->GetMemberType() != MSG_EXPED_MEMBERTYPE_BOSS))
	{
		//에러: 권한 실패
		CNetMsg::SP rmsg(new CNetMsg);
		ExpedErrorMsg(rmsg, MSG_EXPED_ERROR_NOT_EXPEDBOSS);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}
	//<--

	//이동 대상 권한 체크
	//-->
	pMember = (CExpedMember*) ch->m_Exped->GetMemberByCharIndex(nMoveCharIndex);
	if(!pMember)
		return;

	if(pMember && (pMember->GetMemberType() == MSG_EXPED_MEMBERTYPE_BOSS || (pMember->GetMemberType() == MSG_EXPED_MEMBERTYPE_MBOSS)))
	{
		//에러: 원정대장, 부대장 이동 불가
		return;
	}
	//<--

	// 대상이 비어 있는지 체크
	pMember = (CExpedMember*) ch->m_Exped->GetMemberByListIndex(nTargetGroup, nTargetListindex);
	if(pMember)
	{
		//에러: 비어 있지 않음
		return;
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperExpedMoveGroupReqMsg(rmsg, ch->m_index, nSourceGroup, nMoveCharIndex, nTargetGroup, nTargetListindex);
		SEND_Q(rmsg, gserver->m_helper);
	}
}

//대원 추가(대장)
void do_ExpedAddMemberReq(CPC* ch, CNetMsg::SP& msg)
{
	CLCString addCharName(MAX_CHAR_NAME_LENGTH + 1);

	RefMsg(msg) >>	addCharName;

	GAMELOG << init("EXPED DEBUG ADDMEMBER REQ", ch)
			<< "ADDCHARNAME" << delim << addCharName
			<< end;

	CPC * pTargetPC = PCManager::instance()->getPlayerByName(addCharName,false);
	if(pTargetPC == NULL)
		return;

	if(ch->m_Exped == NULL)
	{
		//에러: 원정대에 소속 되어 있지 않음
		CNetMsg::SP rmsg(new CNetMsg);
		ExpedErrorMsg(rmsg, MSG_EXPED_ERROR_NOT_EXPED);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	//권한 체크
	//-->
	CExpedMember* pMember = NULL;
	pMember = (CExpedMember*) ch->m_Exped->GetMemberByCharIndex(ch->m_index);
	if(!pMember)
		return;

	if(pMember && (pMember->GetMemberType() != MSG_EXPED_MEMBERTYPE_BOSS))
	{
		//에러: 권한 실패
		CNetMsg::SP rmsg(new CNetMsg);
		ExpedErrorMsg(rmsg, MSG_EXPED_ERROR_NOT_EXPEDBOSS);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}
	//<--

	// 타입이 EXPED_TYPE_BATTLE 이면 tch와 ch의 레벨 차이를 검사
	// 차이가 +- 10이상이면 ERROR;
	if (ch->m_Exped->GetExpedType(MSG_DIVITYPE_EXP) == MSG_EXPED_TYPE_BATTLE)
	{
		if (ABS(pTargetPC->m_level - ch->m_level) > 10)
		{
			return ;
		}
	}

	{
		//초대 요청
		CNetMsg::SP rmsg(new CNetMsg);
		HelperExpedInviteReqMsg(rmsg, ch->m_index, ch->GetName(), pTargetPC->m_index,pTargetPC->GetName());
		SEND_Q(rmsg, gserver->m_helper);
	}
}

//살펴 보기(대장,부대장)
void do_ExpedViewDetailReq(CPC* ch, CNetMsg::SP& msg)
{
	int nGroup;
	int nDestIndex;

	RefMsg(msg) >> nGroup
				>> nDestIndex;

	GAMELOG << init("EXPED DEBUG VIEWDETAIL REQ", ch)
			<< "GROUP" << delim << nGroup << delim
			<< "DEST INDEX" << delim << nDestIndex
			<< end;

	if(ch->m_Exped == NULL)
	{
		//에러: 원정대에 소속 되어 있지 않음
		CNetMsg::SP rmsg(new CNetMsg);
		ExpedErrorMsg(rmsg, MSG_EXPED_ERROR_NOT_EXPED);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	//권한 체크
	//-->
	CExpedMember* pMember = NULL;
	pMember = (CExpedMember*) ch->m_Exped->GetMemberByCharIndex(ch->m_index);
	if(!pMember)
		return;

	if(pMember && (pMember->GetMemberType() != MSG_EXPED_MEMBERTYPE_BOSS && pMember->GetMemberType() != MSG_EXPED_MEMBERTYPE_MBOSS ))
	{
		//에러: 권한 실패
		CNetMsg::SP rmsg(new CNetMsg);
		ExpedErrorMsg(rmsg, MSG_EXPED_ERROR_NOT_EXPEDBOSS);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}
	//<--

	CPC * pPC = PCManager::instance()->getPlayerByCharIndex(nDestIndex);
	if(pPC == NULL)
		return;

	// 펫정보
	//-->
	CPet* pet = pPC->m_petList;
	while (pet)
	{
		if(pet->IsWearing())
		{
			CNetMsg::SP rmsg(new CNetMsg);
			ExpedViewDail_ExPetMsg(rmsg, pet);
			SEND_Q(rmsg, ch->m_desc);
		}

		pet = pet->m_nextPet;
	}

	CAPet* apet = pPC->m_pApetlist;
	while ( apet )
	{
		if(apet->IsWearing())
		{
			CNetMsg::SP rmsg(new CNetMsg);
			ExpedViewDail_ExAPetMsg(rmsg, apet);
			SEND_Q(rmsg, ch->m_desc);
		}
		apet = apet->m_pNextPet;
	}

	{
		//인벤토리 정보
		CNetMsg::SP rmsg(new CNetMsg);
		ExpedViewDail_InvenMsg(msg, pPC);
		SEND_Q(msg, ch->m_desc);
	}
}

//표식 설정
void do_ExpedSetLabelReq(CPC* ch, CNetMsg::SP& msg)
{
	int nType,nMode,nLabel,nDestIndex;

	RefMsg(msg) >> nType				// pc,npc 구분
				>> nMode				// 설정,해제 구분
				>> nLabel				// 라벨 타입
				>> nDestIndex;			// 설정 대상 인덱스(charindex,mob index)

	GAMELOG << init("EXPED DEBUG SETLABEL REQ", ch)
			<< "TYPE" << delim << nType << delim
			<< "MODE" << delim << nMode << delim
			<< "LABEL" << delim << nLabel << delim
			<< "DESTINDEX" << delim << nDestIndex
			<< end;

	if(!ch->IsExped())
	{
		//에러: 원정대에 소속 되어 있지 않음
		CNetMsg::SP rmsg(new CNetMsg);
		ExpedErrorMsg(rmsg, MSG_EXPED_ERROR_NOT_EXPED);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	if(ch->m_nJoinInzone_ZoneNo == -1 &&  ch->m_nJoinInzone_RoomNo == -1)
	{
		// 인존 내부가 아니어서 표식 설정 불가.
		CNetMsg::SP rmsg(new CNetMsg);
		ExpedErrorMsg(rmsg, MSG_EXPED_ERROR_SETLABEL_NOTINZONE);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	if(nType == MSG_EXPED_SETLABEL_TYPE_NPC)
	{
		CArea* area = ch->m_pArea;
		if(!area)	return;

		CCharacter* tch = area->FindCharInCell(ch, nDestIndex, (MSG_CHAR_TYPE)MSG_CHAR_NPC);
		if(tch == NULL || !IS_NPC(tch))	return;

		if(nMode == MSG_EXPED_SETLABEL_MODE_SET)
		{
			tch->SetExpedLabel(nLabel);
		}
		else if(nMode == MSG_EXPED_SETLABEL_MODE_RESET)
		{
			tch->SetExpedLabel(-1);
		}
	}

	else if(nType == MSG_EXPED_SETLABEL_TYPE_PC)
	{
		CArea* area = ch->m_pArea;
		if(!area)	return;

		CCharacter* tch = area->FindCharInCell(ch, nDestIndex, (MSG_CHAR_TYPE)MSG_CHAR_PC);
		if(tch == NULL || !IS_PC(tch))	return;

		if(nMode == MSG_EXPED_SETLABEL_MODE_SET)
		{
			tch->SetExpedLabel(nLabel);
		}
		else if(nMode == MSG_EXPED_SETLABEL_MODE_RESET)
		{
			tch->SetExpedLabel(-1);
		}
	}

	{
		//성공
		CNetMsg::SP rmsg(new CNetMsg);
		ExpedSetLabelRepMsg(rmsg, nType,nMode,nLabel,nDestIndex);
		ch->m_Exped->SendToAllPC(rmsg);
	}
}

// Write : 권상욱
// Date : 20090825
// 이동되지 않은 Trigger Item을 원정대장이 수동으로 가져오기 버튼을 눌러 Item을 가져오는 함수
void do_ExpedSearchItemReq(CPC* ch, CNetMsg::SP& msg) //
{
	GAMELOG << init("EXPED DEBUG SEARCH TRIGGER ITEM REQ", ch)
			<< "BOSS CHARACTER INDEX" << delim
			<< ch->m_index << end;

	// 넘어온 케릭터가 NULL인지 확인.
	if( ch == NULL )
		return ;

	// 현재 케릭터가 원정대에 소속되어 있는지 확인
	// 원정대에 소속되어 있지 않으면 MSG_EXPED_ERROR_NOT_EXPED 발송
	// TODO:

	if (ch->m_Exped == NULL)
	{
		//에러: 원정대에 소속 되어 있지 않음
		CNetMsg::SP rmsg(new CNetMsg);
		ExpedErrorMsg(rmsg, MSG_EXPED_ERROR_NOT_EXPED);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 현재 케릭터가 원정대 list에서 원정대장인지 확인
	// 원정대장이 아니라면 MSG_EXPED_ERROR_NOT_EXPEDBOSS 발송
	// TODO:

	CExpedMember* pMember = NULL;
	pMember = (CExpedMember*) ch->m_Exped->GetMemberByCharIndex(ch->m_index);
	if(!pMember)
		return;

	if(pMember && (pMember->GetMemberType() != MSG_EXPED_MEMBERTYPE_BOSS))
	{
		//에러: 권한 실패
		CNetMsg::SP rmsg(new CNetMsg);
		ExpedErrorMsg(rmsg, MSG_EXPED_ERROR_NOT_EXPEDBOSS);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 원정대원들의 퀘스트 아이템 인벤 조사
	pMember = NULL;
	bool DidCollect = false;			// 한번이라도 Trigger Item을 가져왔으면 TRUE, 아니면 FALSE
	bool UsualProcess = true;			// 수집 중 비정상적으로 수집 프로세스가 종료될 경우FALSE, 정상적으로 종료될 경우 TRUE;
	int GroupNumber = 0;				// 한 원정대의 Group을 카운트 하기 위한 변수
	int CountMemberEachbyGroup = 0;		// 각 그룹당 인원수를 카운트 하기 위한 변수
	CPC* FromChar;
	CPC* BossChar;

	BossChar = ch;

	for( GroupNumber = 0; GroupNumber < MAX_EXPED_GROUP; GroupNumber++ )
	{
		for( CountMemberEachbyGroup = 0; CountMemberEachbyGroup < MAX_EXPED_GMEMBER; CountMemberEachbyGroup++ )
		{
			pMember = BossChar->m_Exped->GetMemberByListIndex(GroupNumber, CountMemberEachbyGroup);	// 보스케릭터에서 갖고 있는 원정대원들을 원정대 리스트에서 뽑음.

			if( pMember != NULL && pMember->GetMemberType() != MSG_EXPED_MEMBERTYPE_BOSS )			// 원정대원이 NULL아니고, 원정대장이 아닐때,
			{
				FromChar = PCManager::instance()->getPlayerByCharIndex(pMember->GetCharIndex());						// 게임서버 플레이어 리스트에서 케릭터 인덱스를 갖고 플레이어 정보를 얻음.
				if( FromChar )
					SearchTriggerItem(FromChar, BossChar, DidCollect, UsualProcess);					// 원정대원이 갖고 있는 Trigger Item을 조사
			}
		}
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		if( (DidCollect == true) && (UsualProcess == true) )											// 한개의 아이템이라도 수집이 성공하고, 정상적으로 수집 프로세스가 종료가 되면
		{
			ExpedSearchTriggerItemMsg(rmsg, MSG_GET_QUESTITEM_SUCCESS_REP);								// 수집 성공 메시지 발송
			GAMELOG << init("EXPED DEBUG COLLECT TRIGGER ITEM SUCCESS", ch)
					<< "BOSS CHARACTER INDEX" << delim
					<< ch->m_index << end;
		}
		else																							// 그외의 경우
		{
			// 1. 보스의 퀘스트 인벤이 가득 차있어 가져오고 싶어도 못가져오는 경우
			ExpedSearchTriggerItemMsg(rmsg, MSG_GET_QUESTITEM_FAILED_REP);								// 2. 시스템적 오류가 생길 경우
			GAMELOG << init("EXPED DEBUG COLLECT TRIGGER ITEM FAILED", ch)								// 3. 원정대원으로 부터 가져올 Trigger Item이 없을 경우
					<< "BOSS CHARACTER INDEX" << delim
					<< ch->m_index << end;
		}
		SEND_Q(rmsg, BossChar->m_desc);
	}
}

void SearchTriggerItem(CPC* FromChar, CPC* BossChar, bool& DidCollect, bool& UsualProcess)												// 원정대원 TriggerItem 체크
{
	item_search_t vec;
	FromChar->m_inventory.searchFlagByItemProto(ITEM_FLAG_TRIGGER, vec);

	item_search_t::iterator it = vec.begin();
	item_search_t::iterator endit = vec.end();
	for (; it != endit; ++it)
	{
		GiveTriggerItemMemberToBoss(FromChar, BossChar, *it, DidCollect, UsualProcess);
	}
}

void GiveTriggerItemMemberToBoss(CPC* FromChar, CPC* BossChar, item_search_pair_t& p, bool& DidCollect, bool& UsualProcess)
{
	if( !BossChar )														// 원정대장 케릭터가 NULL일 때
	{
		UsualProcess = false;
		return ;
	}

	CItem* item = p.pItem;
	if (item == NULL)
	{
		UsualProcess = false;
		return ;
	}

	if(item->Count() < 1)												// 아이템 개수가 0이하일 때 리턴 false
	{
		UsualProcess = false;
		return ;
	}

	CItem* bossitem = gserver->m_itemProtoList.CreateItem(item->getDBIndex(), -1, 0, 0, item->getItemCount());

	if (BossChar->m_inventory.addItem(bossitem))
	{
		//성공하면 상대방 인벤토리에서는 제거를 해줘야 한다.
		{
			FromChar->m_inventory.deleteItemByItem(item);
		}

		DidCollect = true;
		GAMELOG << init("EXPED DEBUG GIVE TRIGGER ITEM SUCCESS") << end;
		GAMELOG	<< init("FROM MEMBER") << delim << FromChar->m_name << delim
				<< "CHARACTER INDEX" << delim << FromChar->m_index << end;
		GAMELOG << init("TO BOSS") << delim << BossChar->m_name << delim
				<< "CHARACTER INDEX" << delim << BossChar->m_index << end;

		GAMELOG	<< init("GIVE ITEM") << bossitem->m_itemProto->getItemName() << bossitem->Count() << "EA" << end;
	}
	else
	{
		delete bossitem;

		GAMELOG << init("EXPED DEBUG GIVE TRIGGER ITEM FAILED") << end;
		GAMELOG	<< init("FROM MEMBER") << delim << FromChar->m_name << delim
				<< "CHARACTER INDEX" << delim << FromChar->m_index << end;
		GAMELOG << init("TO BOSS") << delim << BossChar->m_name << delim
				<< "CHARACTER INDEX" << delim << BossChar->m_index << end;
		GAMELOG	<< init("GIVE ITEM") << bossitem->m_itemProto->getItemName() << bossitem->Count() << "EA" << end;

		UsualProcess = false;
	}
}

void do_ExpendOffline(CPC* ch, CNetMsg::SP& msg)
{
	if( !ch && ch->m_Exped != NULL )
		return;	
	
	// 원정대원 로그 아웃 처리
	CExpedMember* pMember = NULL;
	pMember = (CExpedMember*) ch->m_Exped->GetMemberByCharIndex(ch->m_index);
	if(pMember) pMember->m_nLevel = 0;

	// 핼퍼로 로그 아웃 처리
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperExpendOfflineMsg( rmsg, ch->m_Exped->GetBossIndex() , ch->m_index );
		SEND_Q(rmsg, gserver->m_helper);
	}
	return;
}
