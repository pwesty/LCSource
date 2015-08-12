#ifndef __PTYPE_OLD_DO_FRIEND_H__
#define __PTYPE_OLD_DO_FRIEND_H__

#include "ptype_base.h"

#pragma pack(push, 1)
//////////////////////////////////////////////////////////////////////////
namespace RequestClient
{
	//MSG_FRIEND_REGIST_REQUEST
	struct doFriendRegReq : public pTypeBase
	{
		int requesterindex;
		char name[MAX_CHAR_NAME_LENGTH + 1];
	};

	//MSG_FRIEND_REGIST_ALLOW
	struct doFriendRegAllow : public pTypeBase
	{
		int charIndex;
	};

	//MSG_FRIEND_REGIST_CANCEL
	struct doFriendRegCancel : public pTypeBase
	{
		int charIndex;
	};
	
	//MSG_FRIEND_SET_CONDITION
	struct doFriendSetCondition : public pTypeBase
	{
		int charIndex;
		int condition;
	};

	struct doFriendDeleteMember : public pTypeBase
	{
		int requestIndex;
		int targetIndex;
		char name[MAX_CHAR_NAME_LENGTH + 1];
	};

	struct doFriendDeleteBlock : public pTypeBase
	{
		int charIndex;
	};
}

namespace ResponseClient
{
	struct doFriendError : public pTypeBase
	{
		char errorCode;
	};

	//MSG_FRIEND_REGIST_REQUEST
	struct doFriendRegRep : public pTypeBase
	{
		int requesterindex;
		char name[MAX_CHAR_NAME_LENGTH + 1];
	};

	//MSG_FRIEND_REGIST_CANCEL
	struct doFriendRegCancel : public pTypeBase
	{
	};
	
	//MSG_FRIEND_SET_CONDITION
	struct doFriendSetCondition : public pTypeBase
	{
		int requester;
		int condition;
	};

	//MSG_FRIEND_REGIST_MEMBER_NOTIFY
	struct doFriendAddNotify : public pTypeBase
	{
		int index;
		char name[MAX_CHAR_NAME_LENGTH + 1];
		int job;
		int condition;
	};
	
	//MSG_FRIEND_DELETE_BLOCK
	struct doFriendDeleteBlock : public pTypeBase
	{
	};

	struct doFriendDelNotify : public pTypeBase
	{
		int delIndex;
	};

#ifndef _CLIENT_
	inline void FriendErrorMsg(CNetMsg::SP& msg, MSG_FRIEND_ERROR_TYPE errcode)
	{
		doFriendError *packet = reinterpret_cast<doFriendError*>(msg->m_buf);
		packet->type = MSG_FRIEND;
		packet->subType = MSG_FRIEND_ERROR;
		packet->errorCode = errcode;
		msg->setSize(sizeof(doFriendError));
	}

	inline void FriendRegReqMsg(CNetMsg::SP& msg, int requestIndex, const char* name)
	{
		doFriendRegRep* p = reinterpret_cast<doFriendRegRep*>(msg->m_buf);
		p->type = MSG_FRIEND;
		p->subType = MSG_FRIEND_REGIST_REQUEST;
		p->requesterindex = requestIndex;
		strncpy(p->name, name, MAX_CHAR_NAME_LENGTH);
		p->name[MAX_CHAR_NAME_LENGTH] = '\0';
		msg->setSize(sizeof(doFriendRegRep));
	}

	inline void FriendRegistCancelMsg(CNetMsg::SP& msg)
	{
		doFriendRegCancel* p = reinterpret_cast<doFriendRegCancel*>(msg->m_buf);
		p->type = MSG_FRIEND;
		p->subType = MSG_FRIEND_REGIST_CANCEL;
		msg->setSize(sizeof(doFriendRegCancel));
	}

	inline void FriendSetConditionMsg(CNetMsg::SP& msg, int requester, int condition)
	{
		doFriendSetCondition* p = reinterpret_cast<doFriendSetCondition*>(msg->m_buf);
		p->type = MSG_FRIEND;
		p->subType = MSG_FRIEND_SET_CONDITION;
		p->requester = requester;
		p->condition = condition;
		msg->setSize(sizeof(doFriendSetCondition));
	}

	inline void FriendDeleteBlockMsg(CNetMsg::SP& msg)
	{
		doFriendDeleteBlock* p = reinterpret_cast<doFriendDeleteBlock*>(msg->m_buf);
		p->type = MSG_FRIEND;
		p->subType = MSG_FRIEND_DELETE_BLOCK;
		msg->setSize(sizeof(doFriendDeleteBlock));
	}

	inline void FriendDelNotify(CNetMsg::SP& msg, int delindex)
	{
		doFriendDelNotify* p = reinterpret_cast<doFriendDelNotify*>(msg->m_buf);
		p->type = MSG_FRIEND;
		p->subType = MSG_FRIEND_DELETE_MEMBER_NOTIFY;
		p->delIndex = delindex;
		msg->setSize(sizeof(doFriendDelNotify));
	}

	inline void FriendAddNotify(CNetMsg::SP& msg, int index, const char* name, int job, int condition)
	{
		doFriendAddNotify* p = reinterpret_cast<doFriendAddNotify*>(msg->m_buf);
		p->type = MSG_FRIEND;
		p->subType = MSG_FRIEND_REGIST_MEMBER_NOTIFY;
		p->index = index;
		strncpy(p->name, name, MAX_CHAR_NAME_LENGTH);
		p->name[MAX_CHAR_NAME_LENGTH] = '\0';
		p->job = job;
		p->condition = condition;
		msg->setSize(sizeof(doFriendAddNotify));
	}
#endif
}

#ifndef _CLIENT_
namespace UpdateServer
{
	struct doFriendAddReqToHelper : public pTypeServer
	{
		int approvalindex;
		char appname[MAX_CHAR_NAME_LENGTH + 1];
		int appjob;
		int requesterindex;
		char reqname[MAX_CHAR_NAME_LENGTH + 1];
		int reqjob;
	};

	struct doFriendAddRepToHelper : public pTypeServer
	{
		int approvalindex;
		char appname[MAX_CHAR_NAME_LENGTH + 1];
		int appjob;
		int requesterindex;
		char reqname[MAX_CHAR_NAME_LENGTH + 1];
		int reqjob;
		int errorCode;
	};

	struct doFriendDelReqToHelper : public pTypeServer
	{
		int approvalindex;
		int requester;
		char name[MAX_CHAR_NAME_LENGTH + 1];
	};

	struct FrinedIndex
	{
		int index;
	};

	struct doFriendSetConditionToHelper : public pTypeServer
	{
		int chaindex;
		int condition;
		int bReply;
		int count;
		FrinedIndex	index[0];
	};

	struct doFriendChattingToMessenger : public pTypeServer
	{
		int seq;
		int server;
		int subno;
		int sindex;
		char sname[MAX_CHAR_NAME_LENGTH + 1];
		char rname[MAX_CHAR_NAME_LENGTH + 1];
		char chat[1000];
	};

	struct doFriendDelMemberReqToHelper : public pTypeServer
	{
		int approvalIndex;
		int requesterIndex;
		char name[MAX_CHAR_NAME_LENGTH + 1];
	};

	struct doFriendDelMemberRepToGameServer : public pTypeServer
	{
		int removerIndex;
		int deleteIndex;
		int errorCode;
	};

	inline void HelperFriendAddReqMsg(CNetMsg::SP& msg, int approvalindex, const char* appname, int appjob, int requesterindex, const char* reqname, int reqjob)
	{
		doFriendAddReqToHelper *p = reinterpret_cast<doFriendAddReqToHelper*>(msg->m_buf);
		p->type = MSG_HELPER_COMMAND_STRUCT;
		p->subType = MSG_HELPER_FRIEND_MEMBER_ADD_REQ;
		p->approvalindex = approvalindex;
		strncpy(p->appname, appname, MAX_CHAR_NAME_LENGTH);
		p->appname[MAX_CHAR_NAME_LENGTH] = '\0';
		p->appjob = appjob;
		p->requesterindex = requesterindex;
		strncpy(p->reqname, reqname, MAX_CHAR_NAME_LENGTH);
		p->reqname[MAX_CHAR_NAME_LENGTH] = '\0';
		p->reqjob = reqjob;
		msg->setSize(sizeof(doFriendAddReqToHelper));
	}

	inline void HelperFriendAddRepMsg(CNetMsg::SP& msg, int approvalindex, const char* appname, int appjob, int requestindex, const char* reqname, int reqjob, MSG_FRIEND_ERROR_TYPE errcode)
	{
		doFriendAddRepToHelper *p = reinterpret_cast<doFriendAddRepToHelper*>(msg->m_buf);
		p->type = MSG_HELPER_COMMAND_STRUCT;
		p->subType = MSG_HELPER_FRIEND_MEMBER_ADD_REP;
		p->approvalindex = approvalindex;
		p->appjob = appjob;
		strncpy(p->appname, appname, MAX_CHAR_NAME_LENGTH);
		p->appname[MAX_CHAR_NAME_LENGTH] = '\0';
		p->requesterindex = requestindex;
		p->reqjob = reqjob;
		strncpy(p->reqname, reqname, MAX_CHAR_NAME_LENGTH);
		p->reqname[MAX_CHAR_NAME_LENGTH] = '\0';
		msg->setSize(sizeof(doFriendAddRepToHelper));
	}
	
	inline void HelperFriendDelReqMsg(CNetMsg::SP& msg, int approvalindex, int requester, const char* name)
	{
		doFriendDelMemberReqToHelper *p = reinterpret_cast<doFriendDelMemberReqToHelper*>(msg->m_buf);
		p->type = MSG_HELPER_COMMAND_STRUCT;
		p->subType = MSG_HELPER_FRIEND_MEMBER_DEL_REQ;
		p->approvalIndex = approvalindex;
		p->requesterIndex = requester;
		strncpy(p->name, name, MAX_CHAR_NAME_LENGTH);
		p->name[MAX_CHAR_NAME_LENGTH] = '\0';
		msg->setSize(sizeof(doFriendDelMemberReqToHelper));
	}

	inline void HelperFriendDelRepMsg(CNetMsg::SP& msg, int removerindex, int deleteindex, MSG_FRIEND_ERROR_TYPE errcode)
	{
		doFriendDelMemberRepToGameServer *p = reinterpret_cast<doFriendDelMemberRepToGameServer*>(msg->m_buf);
		p->type = MSG_HELPER_COMMAND_STRUCT;
		p->subType = MSG_HELPER_FRIEND_MEMBER_DEL_REP;
		p->removerIndex = removerindex;
		p->deleteIndex = deleteindex;
		p->errorCode = errcode;
		msg->setSize(sizeof(doFriendDelMemberRepToGameServer));
	}
}
#endif

//////////////////////////////////////////////////////////////////////////
#pragma pack(pop)

#endif
