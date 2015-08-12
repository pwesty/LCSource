#ifndef __DO_FUNCTION_H__
#define __DO_FUNCTION_H__

#include "../ShareLib/DBCmd.h"

void do_Request(CNetMsg::SP& msg, CDescriptor* dest);
void do_Reply(CNetMsg::SP& msg, CDescriptor* dest);
void do_Command(CNetMsg::SP& msg, CDescriptor* dest);
void do_CommandShutdown(CNetMsg::SP& msg, CDescriptor* dest);
void do_TradeAgent(CNetMsg::SP& msg, CDescriptor* dest);
void checkExpireItemInTradeAgent();

void do_command_TradeAgent_List(CNetMsg::SP& msg, CDescriptor* desc);
void do_command_TradeAgent_RegList(CNetMsg::SP& msg, CDescriptor* desc);
void do_command_TradeAgent_Reg(CNetMsg::SP& mag, CDescriptor* desc);
void do_command_TradeAgent_RegCancel(CNetMsg::SP& msg, CDescriptor* desc);
void do_command_TradeAgent_Buy(CNetMsg::SP& msg, CDescriptor* desc);
void do_command_TradeAgent_LikeList(CNetMsg::SP& msg, CDescriptor* desc);
void do_command_TradeAgent_LikeReg(CNetMsg::SP& msg, CDescriptor* desc);
void do_command_TradeAgent_LikeCancel(CNetMsg::SP& msg, CDescriptor* desc);

void do_Command_TitleSystem_InsertTitleReq(CNetMsg::SP& msg, CDescriptor* dest, bool force=false);
void do_Command_TitleSystem_DisableReq(CNetMsg::SP& msg, CDescriptor* dest);
void do_Command_TitleSystem_DeleteReq(CNetMsg::SP& msg, CDescriptor* dest);
void do_Command_TitleSystem_AutoDelete(CNetMsg::SP& msg, CDescriptor* dest);

void do_Command_LCBallinfo( CNetMsg::SP& msg, CDescriptor* dest );
void do_Command_LCBalluse( CNetMsg::SP& msg, CDescriptor* dest );
void do_Command_LCBallsuccess( CNetMsg::SP& msg, CDescriptor* dest );
void do_Command_LCBallReset( CNetMsg::SP& msg, CDescriptor* dest );

void do_Command_RankingEX_Refresh( CNetMsg::SP& msg, CDescriptor* dest );
void do_Command_RankingEX_ListReq( CNetMsg::SP& msg, CDescriptor* dest );
void do_Command_RankingEX_SearchReq( CNetMsg::SP& msg, CDescriptor* dest );
void do_Command_RankingEx_RewardReq( CNetMsg::SP& msg, CDescriptor* dest );

#ifdef REFORM_PK_PENALTY_201108
void do_CommandPKPenalryReform(CNetMsg::SP& msg, CDescriptor* dest);
void do_CommandPKPenalryReformRewardInfoReq(CNetMsg::SP& msg, CDescriptor* dest);
void do_CommandPKPenalryReformRewardReq(CNetMsg::SP& msg, CDescriptor* dest);
#endif

#ifdef NOTICE_SYSTEM
void do_Notice(CNetMsg::SP& msg, CDescriptor* dest);
#endif


enum COUNT
{
	ZERO = 0,
	ONE,
	TWO,
	THREE,
	FOUR,
	FIVE
};

void do_CommandExtCharacterSlotItemUse(CNetMsg::SP& msg, CDescriptor* dest);
void do_CommandExtCharacterSlotItemUseResult(CNetMsg::SP& msg, CDescriptor* dest);

#endif //__DO_FUNCTION_H__
//
