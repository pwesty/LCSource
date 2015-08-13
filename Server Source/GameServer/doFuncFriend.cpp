#include "stdhdrs.h"

#include "Log.h"
#include "Character.h"
#include "Server.h"
#include "CmdMsg.h"
#include "doFunc.h"
#include "../ShareLib/DBCmd.h"
#include "../ShareLib/packetType/ptype_old_do_friend.h"
//#include "Friend.h"

//클라이언트로부터 메시지 받는 부분...
void do_Friend(CPC* ch, CNetMsg::SP& msg)
{
	unsigned char subtype;
	msg->MoveFirst();
	RefMsg(msg) >> subtype;

	switch (subtype)
	{
	case MSG_FRIEND_REGIST_REQUEST://친구 등록 요청
		do_FriendRegistRequest(ch, msg);
		break;

	case MSG_FRIEND_REGIST_ALLOW: // 등록 승인
		do_FriendRegistAllow(ch, msg);
		break;

	case MSG_FRIEND_REGIST_CANCEL: // 등록 거절
		do_FriendRegistRefuse(ch, msg);
		break;

	case MSG_FRIEND_DELETE_MEMBER:
		do_FriendDeleteMember(ch, msg);
		break;
	
	case MSG_FRIEND_SET_CONDITION: //상태변경
		do_FriendSetCondition(ch, msg);
		break;

	case MSG_FRIEND_DELETE_BLOCK:
		do_FriendDeleteBlock(ch, msg);
		break;

	default:
		break;
	}
}

//클라어언트로부터 친구등록 요청 메시지가 왔을때...
void do_FriendRegistRequest(CPC* ch, CNetMsg::SP& msg)
{
	RequestClient::doFriendRegReq *packet = reinterpret_cast<RequestClient::doFriendRegReq*>(msg->m_buf);
	packet->name[MAX_CHAR_NAME_LENGTH] = '\0';

	if( strncmp(ch->m_name, packet->name, ch->m_name.Length()) == 0 || strncmp(ch->m_nick, packet->name, ch->m_nick.Length()) == 0)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::FriendErrorMsg(rmsg, MSG_FRIEND_ERROR_FRIENDNAME);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 이름 검사
	int lenname = strlen(packet->name);
	if (lenname < 2 || lenname > 16)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::FriendErrorMsg(rmsg, MSG_FRIEND_ERROR_FRIENDNAME);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
	if (strchr(packet->name, '\'') || strchr(packet->name, ' ') /* || strchr(name, '\\') || strchr(name, '\"') || strchr(name, '%') || strchr(name, '_')*/)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::FriendErrorMsg(rmsg, MSG_FRIEND_ERROR_FRIENDNAME);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
	if (findPercentChar(packet->name) != NULL)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::FriendErrorMsg(rmsg, MSG_FRIEND_ERROR_FRIENDNAME);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	CPC* pPc = PCManager::instance()->getPlayerByName(packet->name, true);

	//해당 캐릭이 존재하지 않는다면...
	if (pPc == NULL)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::FriendErrorMsg(rmsg, MSG_FRIEND_ERROR_NOT_EXIST);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	if( pPc->IsSetPlayerState(PLAYER_STATE_PKMODE) )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::FriendErrorMsg(rmsg, MSG_FRIEND_ERROR_REGIST_REFUSE_PVP);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	if( pPc->IsBlockPC(ch->m_index, ch->GetName() ) )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::FriendErrorMsg(rmsg, MSG_FRIEND_ERROR_ALREADY_BLOCK);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	//정원 초과했는지 체크...
	int friendcnt;
	friendcnt = pPc->m_Friend->GetFriendSum();

	if(friendcnt >= FRIEND_MAX_MEMBER)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::FriendErrorMsg(rmsg, MSG_FRIEND_ERROR_FULLMEMBER);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	friendcnt = ch->m_Friend->GetFriendSum();

	if(friendcnt >= FRIEND_MAX_MEMBER)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::FriendErrorMsg(rmsg, MSG_FRIEND_ERROR_FULLMEMBER);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
//요청자가 이미 등록되어 있는지 체크..
	CFriendMember* pFriendMem = pPc->m_Friend->FindFriendMember(packet->requesterindex);
	if(pFriendMem != NULL)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::FriendErrorMsg(rmsg, MSG_FRIEND_ERROR_ALREADY_EXIST);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
//이미 다른 사람의 친구요청의 답변을 기다라는 상태라면,,
	if (pPc->m_nRegFriend != 0 && pPc->m_nRegFriend  != ch->m_index)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::FriendErrorMsg(rmsg, MSG_FRIEND_ERROR_WAIT_OTHER);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	pPc->m_nRegFriend = ch->m_index;
	ch->m_nRegFriend = pPc->m_index;

	{
		//메시지 그대로 보낸다.
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::FriendRegReqMsg(rmsg, packet->requesterindex, ch->GetName());		
		SEND_Q(rmsg, pPc->m_desc); //등록 당하는자에게 보내기.
	}
}

//친구등록을 허락했다...
void do_FriendRegistAllow(CPC* ch, CNetMsg::SP& msg)
{
	RequestClient::doFriendRegAllow *packet = reinterpret_cast<RequestClient::doFriendRegAllow*>(msg->m_buf);

	CPC* pPc;
	pPc= PCManager::instance()->getPlayerByCharIndex(packet->charIndex);

	if (!pPc)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::FriendRegistCancelMsg(rmsg);
		SEND_Q(rmsg, ch->m_desc);
		ch->m_nRegFriend = 0;
		return ;
	}
//서로의 인덱스가 맞지 않는다면..
	if (pPc->m_nRegFriend != ch->m_index || ch->m_nRegFriend != pPc->m_index)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::FriendErrorMsg(rmsg, MSG_FRIEND_ERROR_NOT_MATCH_INDEX);
		SEND_Q(rmsg, ch->m_desc);
		SEND_Q(rmsg, pPc->m_desc);
		ch->m_nRegFriend = 0;
		pPc->m_nRegFriend = 0;
		return ;
	}

	if (gserver->isRunHelper())
	{
		CNetMsg::SP rmsg(new CNetMsg);
		UpdateServer::HelperFriendAddReqMsg(rmsg, ch->m_index, ch->GetName(), ch->m_job, pPc->m_index, pPc->GetName(), pPc->m_job);
		SEND_Q(rmsg, gserver->m_helper);
	}
	else
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::FriendErrorMsg(rmsg, MSG_FRIEND_ERROR_HELPER_SERVER);
		SEND_Q(rmsg, ch->m_desc);
		SEND_Q(rmsg, pPc->m_desc);
		ch->m_nRegFriend = 0;
		pPc->m_nRegFriend = 0;
		return ;
	}
}
//친구 등록을 거절했다...
void do_FriendRegistRefuse(CPC* ch, CNetMsg::SP& msg)
{
	RequestClient::doFriendRegCancel *packet = reinterpret_cast<RequestClient::doFriendRegCancel*>(msg->m_buf);
	
	CPC* pPc;
	pPc= PCManager::instance()->getPlayerByCharIndex(packet->charIndex);

	if(pPc)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::FriendRegistCancelMsg(rmsg);
		SEND_Q(rmsg, pPc->m_desc);

		pPc->m_nRegFriend = 0;
	}

	ch->m_nRegFriend = 0;
}

void do_FriendDeleteMember(CPC* ch, CNetMsg::SP& msg)
{
	RequestClient::doFriendDeleteMember *packet = reinterpret_cast<RequestClient::doFriendDeleteMember*>(msg->m_buf);
	packet->name[MAX_CHAR_NAME_LENGTH] = '\0';
	
	//자신을 지우려고 했거나, 요청자가 자신이 아니라면...
	if (packet->targetIndex == ch->m_index || packet->requestIndex != ch->m_index)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::FriendErrorMsg(rmsg, MSG_FRIEND_ERROR_PACKET);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	if (packet->targetIndex == -1)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::FriendErrorMsg(rmsg, MSG_FRIEND_ERROR_PACKET);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	int lenname = strlen(packet->name);
	if (lenname < 2 || lenname > 16)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::FriendErrorMsg(rmsg, MSG_FRIEND_ERROR_FRIENDNAME);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
	if (findPercentChar(packet->name) != NULL)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::FriendErrorMsg(rmsg, MSG_FRIEND_ERROR_FRIENDNAME);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		UpdateServer::HelperFriendDelReqMsg(rmsg, ch->m_index, packet->targetIndex, packet->name);
		SEND_Q(rmsg, gserver->m_helper);
	}
}


// 상태 변경하기..
void do_FriendSetCondition(CPC* ch, CNetMsg::SP& msg)
{
	RequestClient::doFriendSetCondition* packet = reinterpret_cast<RequestClient::doFriendSetCondition*>(msg->m_buf);

	ch->m_nCondition = packet->condition;
//0521
	if(ch->m_Friend)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperFriendSetConditionMsg(rmsg, ch->m_index, packet->condition, -1, ch);
		SEND_Q(rmsg, gserver->m_helper);
	}

	{
		//임시..릴레이.
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::FriendSetConditionMsg(rmsg, packet->charIndex, packet->condition);
		SEND_Q(rmsg, ch->m_desc);
	}
}

//블럭 멤버 삭제
void do_FriendDeleteBlock(CPC* ch, CNetMsg::SP& msg)
{
	RequestClient::doFriendDeleteBlock* packet = reinterpret_cast<RequestClient::doFriendDeleteBlock*>(msg->m_buf);

	map_listblock_t::iterator it = ch->m_listBlockPC.find(packet->charIndex);
	if (it != ch->m_listBlockPC.end())
	{
		{
			CNetMsg::SP rmsg(new CNetMsg);
			UpdateServer::HelperFriendDelReqMsg(rmsg, ch->m_index, it->second->GetIndex(), it->second->GetName());
			SEND_Q(rmsg, gserver->m_helper);
		}
		
		ch->m_listBlockPC.erase(it);

		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::FriendDeleteBlockMsg(rmsg);
		SEND_Q(rmsg, ch->m_desc);
	}
	else
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::FriendErrorMsg(rmsg, MSG_FRIEND_ERROR_NOT_EXIST);
		SEND_Q(rmsg, ch->m_desc);
	}
}