#ifndef __CMDMSG_H__
#define __CMDMSG_H__

#include "Descriptor.h"

void FailMsg(CNetMsg::SP& msg, MSG_FAIL_TYPE failtype);

typedef struct ItemOption
{
	int	m_nOptionCnt;

	int m_Option_type[MAX_ITEM_OPTION  + 1];
	int m_Option_level[MAX_ITEM_OPTION + 1];

	ItemOption(void)
	{
		m_nOptionCnt = 0;

		for(int i=0; i < MAX_ITEM_OPTION + 1; i++)
		{
			m_Option_type[i] = 0;
			m_Option_level[i] = 0;
		}
	}
} S_ITEMOPTION;

void HelperTradeAgentReturnedMsg(CNetMsg::SP& msg, int nSellcharindex,CLCString stItemName,int nItemCount);

void do_Command_TitleSystem_InsertTitleRep(CNetMsg::SP& msg, int char_index, int title_index, int endtime, int errortype, int tab, int invenIndex, CLCString serial);
void do_Command_TitleSystem_DisableRep(CNetMsg::SP& msg, int char_index, int count);
void do_Command_TitleSystem_DeleteRep(CNetMsg::SP& msg, int char_index, int title_index, int errortype);
// [selo: 101105] 종료 시간 강제 변경
void do_Command_TitleSystem_ForceChangeTime(CNetMsg::SP& msg, int char_index, int title_index, int endtime);

void SubHelperLCBallUseMsg( CNetMsg::SP& msg , int charIndex, int coinCount, char courseIndex, char nOrder, int itemIndex , unsigned int itemCount );
void SubHelperLCBallSuccessMsg( CNetMsg::SP& msg , CLCString name, int itemIndex );

void SubHelperRankingEXRefreshRep(CNetMsg::SP& msg);
void SubHelperRankingEXListRep(CNetMsg::SP& msg, int msgType, unsigned char type, unsigned char subtype, int charIdx);
void SubHelperRankingEXError(CNetMsg::SP& msg, int type, int charIdx);

#ifdef REFORM_PK_PENALTY_201108
void SubHelperPKPenaltyReformRewardRep(CNetMsg::SP& msg, int charIndex, int rewardNum, int titleIndex, int errorType);
void SubHelperPKPenaltyReformRewardInfoRep(CNetMsg::SP& msg, int charIndex, int rewardFlag);
#endif // REFORM_PK_PENALTY_201108

#ifdef NOTICE_SYSTEM
void SubHelperUserNoticeRegRep(CNetMsg::SP& msg, int _characterIndex);
void SubHelperUserNoticePrint(CNetMsg::SP& msg, CLCString _name, CLCString _contents, int _count);
#endif // NOTICE_SYSTEM

void SubHelperExtCharacterSlotItemUseRep(CNetMsg::SP& msg, unsigned char _cmd , int _userIndex, int _charIndex, int _tab, int _invenIndex, CLCString _serial, long _time);

#endif	// __CMDMSG_H__
//
