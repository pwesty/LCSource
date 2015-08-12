#include "stdhdrs.h"

#include "Server.h"
#include "../ShareLib/DBCmd.h"
#include "CmdMsg.h"
#include "doFunc.h"

void FailMsg(CNetMsg::SP& msg, MSG_FAIL_TYPE failtype)
{
	msg->Init(MSG_FAIL);
	RefMsg(msg) << (unsigned char)failtype;
}

void do_Command_TitleSystem_InsertTitleRep(CNetMsg::SP& msg, int char_index, int title_index, int endtime, int errortype, int tab, int invenIndex, CLCString serial)
{
	msg->Init(MSG_SUBHELPER_COMMAND);
	RefMsg(msg) << MSG_SUBHELPER_TITLESYSTEM_INSERT_TITLE_REP
				<< char_index
				<< title_index
				<< endtime
				<< errortype
				<< tab
				<< invenIndex
				<< serial;
}

void do_Command_TitleSystem_DisableRep(CNetMsg::SP& msg, int char_index, int count)
{
	msg->Init(MSG_SUBHELPER_COMMAND);
	RefMsg(msg) << MSG_SUBHELPER_TITLESYSTEM_CHECK_EXPIRE_TITLE_REP
				<< char_index
				<< count;
}

void do_Command_TitleSystem_DeleteRep(CNetMsg::SP& msg, int char_index, int title_index, int errortype)
{
	msg->Init(MSG_SUBHELPER_COMMAND);
	RefMsg(msg) << MSG_SUBHELPER_TITLE_SYSTEM_TITLE_DELETE_REP
				<< char_index
				<< title_index
				<< errortype;			// errortype = 1 이면 update true, errortype = 0 이면 update false 임.
}

// [selo: 101105] 종료 시간 강제 변경
void do_Command_TitleSystem_ForceChangeTime(CNetMsg::SP& msg, int char_index, int title_index, int endtime)
{
	msg->Init(MSG_SUBHELPER_COMMAND);
	RefMsg(msg) << MSG_SUBHELPER_TITLE_SYSTEM_FORCE_CHANGE_TIME
				<< char_index
				<< title_index
				<< endtime;
}

void SubHelperLCBallUseMsg( CNetMsg::SP& msg , int charIndex, int coinIndex, char courseIndex, char cOrder, int itemIndex, unsigned int itemCount )
{
	msg->Init(MSG_SUBHELPER_COMMAND);
	RefMsg(msg) << MSG_SUBHELPER_LCBALL_USE
				<< charIndex
				<< coinIndex
				<< courseIndex
				<< cOrder
				<< itemIndex
				<< itemCount;
}

void SubHelperLCBallSuccessMsg( CNetMsg::SP& msg , CLCString name, int itemIndex )
{
	msg->Init(MSG_SUBHELPER_COMMAND);
	RefMsg(msg) << MSG_SUBHELPER_LCBALL_USE_SUCCESS
				<< name
				<< itemIndex;
}

void SubHelperRankingEXRefreshRep(CNetMsg::SP& msg)
{
	msg->Init(MSG_SUBHELPER_COMMAND);
	RefMsg(msg) << MSG_SUBHELPER_RANKING_EX_REFRESH;
}
void SubHelperRankingEXListRep(CNetMsg::SP& msg, int msgType, unsigned char type, unsigned char subtype, int charIdx)
{
	msg->Init(MSG_SUBHELPER_COMMAND);
	RefMsg(msg) << msgType
				<< type
				<< subtype
				<< charIdx;
}

void SubHelperRankingEXError(CNetMsg::SP& msg, int type, int charIdx)
{
	msg->Init(MSG_SUBHELPER_COMMAND);
	RefMsg(msg) << MSG_SUBHELPER_RANKING_EX_ERROR
				<< type
				<< charIdx;
}

#ifdef REFORM_PK_PENALTY_201108
void SubHelperPKPenaltyReformRewardRep(CNetMsg::SP& msg, int charIndex, int rewardNum, int titleIndex, int errorType)
{
	msg->Init(MSG_SUBHELPER_COMMAND);
	RefMsg(msg) << MSG_SUBHELPER_PKPENALTY_REFORM
				<< (unsigned char) MSG_SUBHELPER_PKPENALTY_REFORM_REWARD_REP
				<< charIndex
				<< rewardNum
				<< titleIndex
				<< (unsigned char) errorType;
}

void SubHelperPKPenaltyReformRewardInfoRep(CNetMsg::SP& msg, int charIndex, int rewardFlag)
{
	msg->Init(MSG_SUBHELPER_COMMAND);
	RefMsg(msg) << MSG_SUBHELPER_PKPENALTY_REFORM
				<< (unsigned char) MSG_SUBHELPER_PKPENALTY_REFORM_REWARD_INFO_REP
				<< charIndex
				<< rewardFlag;
}
#endif // REFORM_PK_PENALTY_201108

#ifdef NOTICE_SYSTEM
void SubHelperUserNoticeRegRep(CNetMsg::SP& msg, int _characterIndex)
{
	msg->Init(MSG_SUBHELPER_NOTICE);
	RefMsg(msg) << MSG_SUBHELPER_USER_NOTICE_REG_REP
		<< _characterIndex;
}

void SubHelperUserNoticePrint(CNetMsg::SP& msg, CLCString _name, CLCString _contents, int _count)
{
	msg->Init(MSG_SUBHELPER_NOTICE);
	RefMsg(msg) << MSG_SUBHELPER_USER_NOTICE_PRINT
		<< _name
		<< _contents
		<< _count;
}

#endif // NOTICE_SYSTEM


void SubHelperExtCharacterSlotItemUseRep(CNetMsg::SP& msg, unsigned char _cmd , int _userIndex, int _charIndex, int _tab, int _invenIndex, CLCString _serial, long _time)
{
	msg->Init(MSG_SUBHELPER_COMMAND);
	RefMsg(msg) << MSG_SUBHELPER_EXT_CHAR_SLOT_ITEM_USE_REP
				<< _cmd
				<< _userIndex
				<< _charIndex
				<< _tab
				<< _invenIndex
				<< _serial
				<< _time;
}
//