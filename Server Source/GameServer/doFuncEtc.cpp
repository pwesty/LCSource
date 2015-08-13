#include "stdhdrs.h"

#include "Log.h"
#include "Cmd.h"
#include "Character.h"
#include "Server.h"
#include "CmdMsg.h"
#include "doFunc.h"
#include "DratanCastle.h"
#include "Notice.h"
#include "../ShareLib/packetType/ptype_old_do_item.h"
#include "../ShareLib/packetType/ptype_old_extend.h"
#include "../ShareLib/packetType/ptype_old_do_move.h"
#include "../ShareLib/packetType/ptype_old_do_sskill.h"
#include "../ShareLib/packetType/ptype_attendance.h"
#include "../ShareLib/packetType/ptype_notice.h"

extern CCmdList gexcmdlist;

void do_StatPoint(CPC* ch, CNetMsg::SP& msg)
{
	unsigned char subtype = (unsigned char)-1;
	msg->MoveFirst();
	RefMsg(msg) >> subtype;

	switch (subtype)
	{
	case MSG_STATPOINT_USE:
		do_StatPointUse(ch, msg);
		break;

	case MSG_STATPOINT_RESET:
		do_StatPointReset(ch, msg);
		break;
	}
}

void do_StatPointUse(CPC* ch, CNetMsg::SP& msg)
{
	unsigned char type;

	RefMsg(msg) >> type;

	// 남은 포인트 있나 보기
	if (ch->m_statpt_remain < 1)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		StatPointErrorMsg(rmsg, MSG_STATPOINT_ERROR_NOTENOUGH);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	int val;
	if (ch->UseStatPoint((MSG_STATPOINT_USE_TYPE)type, &val))
	{
		GAMELOG << init("STATPOINT USE", ch)
				<< "TYPE" << delim
				<< type << end;

		CNetMsg::SP rmsg(new CNetMsg);
		StatPointUseMsg(rmsg, ch, (MSG_STATPOINT_USE_TYPE)type, val);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
	else
	{
		CNetMsg::SP rmsg(new CNetMsg);
		StatPointErrorMsg(rmsg, MSG_STATPOINT_ERROR_CANTUSE);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
}

void do_StatPointReset(CPC* ch, CNetMsg::SP& msg)
{
#ifndef LC_KOR
	if (ch->m_job2 == 0)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		StatPointErrorMsg(rmsg, MSG_STATPOINT_ERROR_WRONGPACKET);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
#endif

	int reset_str = 0;
	int reset_dex = 0;
	int reset_int = 0;
	int reset_con = 0;
	int reset_all = 0;

#ifdef NPC_CHECK
	int npcIndex;
	CNPC *npc;
	RefMsg(msg) >> npcIndex
				>> reset_str
				>> reset_dex
				>> reset_int
				>> reset_con;
	npc = ch->m_pArea->FindProtoNPCInCell(ch, npcIndex, false, 2);
	if(!npc)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		StatPointErrorMsg(rmsg, MSG_STATPOINT_ERROR_NOT_EXIST_NPC);
		SEND_Q(rmsg, ch->m_desc);
		GAMELOG << init("STATPOINT RESET FAIL: NOT EXIST NPC (Dangerous User)", ch) << end;
		return ;
	}
	if( !npc->IsFlag(NPC_RESETSTAT) )
	{
		GAMELOG << init("STATPOINT RESET FAIL: NOT EXIST NPC (Dangerous User)", ch) << end;
		return ;
	}
#else
	RefMsg(msg) >> reset_str
				>> reset_dex
				>> reset_int
				>> reset_con;
#endif
	if (reset_str < 0 || reset_dex < 0 || reset_int < 0 || reset_con < 0)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		StatPointErrorMsg(rmsg, MSG_STATPOINT_ERROR_WRONGPACKET);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if (reset_str > ch->m_statpt_str || reset_dex > ch->m_statpt_dex || reset_int > ch->m_statpt_int || reset_con > ch->m_statpt_con)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		StatPointErrorMsg(rmsg, MSG_STATPOINT_ERROR_WRONGPACKET);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	reset_all = reset_str + reset_dex + reset_int + reset_con;

	if (reset_all < 1)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		StatPointErrorMsg(rmsg, MSG_STATPOINT_ERROR_WRONGPACKET);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
	LONGLONG needmoney = 20000;		// 1 포인트당 필요 나스
	needmoney *= reset_all;

	if (ch->m_inventory.getMoney() < needmoney)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		StatPointErrorMsg(rmsg, MSG_STATPOINT_ERROR_NOMONEY);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
	else
	{
		ch->m_inventory.decreaseMoney(needmoney);
	}

	GAMELOG << init("STATPOINT RESET", ch)
			<< "MONEY" << delim
			<< needmoney << delim
			<< "RESET POINT" << delim
			<< reset_str << delim
			<< reset_dex << delim
			<< reset_int << delim
			<< reset_con << delim;

	ch->m_statpt_str -= reset_str;
	ch->m_statpt_dex -= reset_dex;
	ch->m_statpt_int -= reset_int;
	ch->m_statpt_con -= reset_con;
	ch->m_statpt_remain += reset_all;

	GAMELOG << "RESULT" << delim
			<< ch->m_statpt_str << delim
			<< ch->m_statpt_dex << delim
			<< ch->m_statpt_int << delim
			<< ch->m_statpt_con << end;

	ch->InitStat();

	{
		CNetMsg::SP rmsg(new CNetMsg);
		StatPointRemainMsg(rmsg, ch);
		SEND_Q(rmsg, ch->m_desc);
	}

#ifdef NPC_CHECK
	{
		CNetMsg::SP rmsg(new CNetMsg);
		rmsg->Init(MSG_STATPOINT);
		RefMsg(rmsg) << (unsigned char)MSG_STATPOINT_RESET
					 << reset_str
					 << reset_dex
					 << reset_int
					 << reset_con;
		SEND_Q(rmsg, ch->m_desc);
	}
#else
	SEND_Q(msg, ch->m_desc);
#endif
	ch->CalcStatus(true);
}

void do_Pulse(CPC* ch, CNetMsg::SP& msg)
{
	int pulse;
	unsigned char nation;

	msg->MoveFirst();

	RefMsg(msg) >> pulse
				>> nation;

#ifdef DISCONNECT_HACK_CHARACTER  // 핵 프로그램 유저 방출

	float tickDelayTime;
	// 딜레이가 0.06 초과이면 핵 유저로 처리
	RefMsg(msg) >> tickDelayTime;

	if ( tickDelayTime > HACK_LIMIT )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperDisconnectHackCharacter(rmsg, ch->m_desc->m_index, (const char*)ch->m_desc->m_idname, ch->m_index, (const char*)ch->m_name, HACK_TYPE_PULSE, tickDelayTime);
		SEND_Q(rmsg, gserver->m_helper);
	}

#endif // DISCONNECT_HACK_CHARACTER

#if defined (INTERGRATION_SERVER)
	if ( !gserver->m_intergrationInfo.Check(nation) )
#else
	if( nation != gserver->m_national )
#endif
	{
		LOG_ERROR("invalid nation. nation is %d. gserver->m_national is %d", nation, gserver->m_national);
		ch->m_desc->Close("invalid nation");
		return;
	}
	ch->m_desc->CheckHackPulse(pulse);
}

#ifdef DISCONNECT_HACK_CHARACTER  // 핵 프로그램 유저 방출
void do_Disconnect_Hack_Character(CPC* ch, CNetMsg::SP& msg)
{
	float delay;

	//msg->MoveFirst();

	RefMsg(msg) >> delay;

	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperDisconnectHackCharacter(rmsg, ch->m_desc->m_index, (const char*)ch->m_desc->m_idname, ch->m_index, (const char*)ch->m_name, HACK_TYPE_CLIENT, delay);
		SEND_Q(rmsg, gserver->m_helper);
	}

	GAMELOG << init("HACK_USER_DISCONNECT", ch)
			<< "Name" << delim
			<< ch->m_name << delim
			<< HACK_TYPE_CLIENT << delim
			<< delay << end;

	// 해당 캐릭터 DIS
	LOG_ERROR("hack user is dis.");
	ch->m_desc->Close("hack user is dis.");
	return;
}
#endif // DISCONNECT_HACK_CHARACTER

void do_Change(CPC* ch, CNetMsg::SP& msg)
{
	unsigned char subtype;

	msg->MoveFirst();

	RefMsg(msg) >> subtype;

	if (subtype != MSG_CHANGE_CANCEL)
		return;

	if (ch->IsSetPlayerState(PLAYER_STATE_CHANGE) && ch->m_changeIndex != -1)
		ch->CancelChange();
}

void do_UI(CPC* ch, CNetMsg::SP& msg)
{
	unsigned char subtype;
	char plusEffect;

	msg->MoveFirst();

	RefMsg(msg) >> subtype
				>> plusEffect;

	if (subtype != MSG_UI_PLUS_EFFECT_REQ)
		return;

	ch->m_plusEffect = plusEffect;

	{
		CNetMsg::SP rmsg(new CNetMsg);
		UIPlusEffectRep(rmsg, ch);
		ch->m_pArea->SendToCell(rmsg, ch, true);
	}
}

void do_Extend(CPC* ch, CNetMsg::SP& msg)
{
	msg->MoveFirst();

	int debug_type = msg->m_mtype;
	int subtype;
	unsigned char thirdtype = 0;

	RefMsg(msg) >> subtype;

	if (gexcmdlist.Find(subtype) == false)
	{
		// 바이트 스트림에서 구조체로 변경된 패킷을 처리하기 위해
		pTypeThirdBase* pThird = reinterpret_cast<pTypeThirdBase*>(msg->m_buf);
		subtype = pThird->subType;
		thirdtype = pThird->thirdType;

		if (gexcmdlist.Find(subtype) == false)
		{
			LOG_ERROR("HACKING : invalid extend command[%d]. charIndex[%d]", subtype, ch->m_index);
			ch->m_desc->Close("Invalid extend command");
			return;
		}
	}
	else
	{
		RefMsg(msg) >> thirdtype;

		msg->MoveFirst();
		RefMsg(msg) >> subtype;
	}

	//LOG_INFO("DEBUG_FUNC : START : type : %d : subtype : %d : thirdType : %d", debug_type, subtype, thirdtype);
	gexcmdlist.Run(subtype, ch, msg);
	//LOG_INFO("DEBUG_FUNC : END : type : %d : subtype : %d : thirdType : %d", debug_type, subtype, thirdtype);
}

void do_Ex_NameChange(CPC* ch, CNetMsg::SP& msg)
{
	// TODO : BS : 길드와 캐릭명의 길이가 달라질 경우 수정이 필요
	// 이름 변경 아이템	: tab_idx(uc) row_idx(uc) col_idx(uc) item_idx(n) name(str)

	//msg->MoveFirst();

	int itemindex;
	CLCString name(MAX_CHAR_NAME_LENGTH + 1);

	RefMsg(msg) >> itemindex
				>> name;
	int r, c;

	// 이름이 0 보다 작으면 안된다.
	if (strlen(name) < 4 || strlen(name) > 16)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		FailMsg(rmsg, MSG_FAIL_WRONG_CHAR);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}
	// 이름에 ' 가 들어 있으면 안된다.
	if (strinc(name, "'") || strinc(name, " "))
	{
		CNetMsg::SP rmsg(new CNetMsg);
		FailMsg(rmsg, MSG_FAIL_WRONG_CHAR);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	if(findPercentChar(name.getBuffer()) != NULL)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		FailMsg(rmsg, MSG_FAIL_WRONG_CHAR);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}
// [selo: 101115] 러시아는 러시아어 이외의 글자 들어가면 안된다.
#ifdef LC_RUS
	if( CheckNoRussianCharacter(name) )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		FailMsg(rmsg, MSG_FAIL_WRONG_CHAR);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}
#endif // LC_RUS

	if( ch && ( ch->IsParty() || ch->IsExped() ) )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		NameChangeRepMsg(rmsg, MSG_EX_NAMECHANGE_ERROR_PARTY, name, 0);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	switch(itemindex)
	{
	// 케릭 이름 변경
	case 842:
		{
			if (ch->m_inventory.FindByDBIndex(842, 0, 0) || ch->m_inventory.FindByDBIndex(1120, 0, 0))
			{
				if(gserver->isRunHelper())
				{
					CNetMsg::SP rmsg(new CNetMsg);
					HelperNameChangeReqMsg(rmsg, 0, ch->m_index, name);
					SEND_Q(rmsg, gserver->m_helper);
				}
				else
				{
					// 이름을 변경하지 못해요
					CNetMsg::SP rmsg(new CNetMsg);
					NameChangeRepMsg(rmsg, MSG_EX_NAMECHANGE_ERROR_HELPER, name, 0);
					SEND_Q(rmsg, ch->m_desc);
				}
			}
		}
		break;
	// 길드 이름 변경
	case 843:
		{
			if (ch->m_inventory.FindByDBIndex(843, 0, 0) == NULL)
			{
				return;
			}
			if(ch->m_guildInfo == NULL || ch->m_guildInfo->guild() == NULL)
			{
				CNetMsg::SP rmsg(new CNetMsg);
				NameChangeRepMsg(rmsg, MSG_EX_NAMECHANGE_ERROR_NOTGUILDBOSS, name, 1);
				SEND_Q(rmsg, ch->m_desc);
				return;
			}
			if( ch->m_index != ch->m_guildInfo->guild()->boss()->charindex() )
			{
				CNetMsg::SP rmsg(new CNetMsg);
				NameChangeRepMsg(rmsg, MSG_EX_NAMECHANGE_ERROR_NOTGUILDBOSS, name, 1);
				SEND_Q(rmsg, ch->m_desc);
				return;
			}

			if(gserver->isRunHelper())
			{
				CNetMsg::SP rmsg(new CNetMsg);
				HelperNameChangeReqMsg(rmsg, 1, ch->m_guildInfo->guild()->index(), name);
				SEND_Q(rmsg, gserver->m_helper);
			}
			else
			{
				// 이름을 변경하지 못해요
				CNetMsg::SP rmsg(new CNetMsg);
				NameChangeRepMsg(rmsg, MSG_EX_NAMECHANGE_ERROR_HELPER, name, 1);
				SEND_Q(rmsg, ch->m_desc);
			}
		}
		break;
	}
}

void do_ExPartyRecall(CPC* ch, CNetMsg::SP& msg)
{
	int subtype;
	RefMsg(msg) >> subtype;

	switch (subtype)
	{
	case MSG_EX_PARTY_RECALL_CONFIRM:
		{
			int reqcharindex = 0;
			char yesno = 0;
			CLCString reqcharname(MAX_CHAR_NAME_LENGTH + 1);
			RefMsg(msg) >> reqcharindex
						>> reqcharname
						>> yesno;

			if (!ch->IsParty())
			{
				CNetMsg::SP rmsg(new CNetMsg);
				HelperPartyRecallConfirmFail(rmsg);
				SEND_Q(rmsg, ch->m_desc);
				return ;
			}

			if (ch->m_index == reqcharindex)
			{
				CNetMsg::SP rmsg(new CNetMsg);
				HelperPartyRecallConfirmFail(rmsg);
				SEND_Q(rmsg, ch->m_desc);
				return ;
			}

			if (ch->m_pZone->IsPersonalDungeon() || ch->m_pZone->IsGuildRoom())
			{
				//이동을 못하는 경우 리콜 리스트에서 제거를 해준다.
				ch->m_party->deletePartyRecallMember(ch->m_index);
				
				CNetMsg::SP rmsg(new CNetMsg);
				HelperPartyRecallConfirmFail(rmsg);
				SEND_Q(rmsg, ch->m_desc);
				return ;
			}

			if (ch->m_party->FindMember(reqcharindex) == -1)
			{
				CNetMsg::SP rmsg(new CNetMsg);
				HelperPartyRecallConfirmFail(rmsg);
				SEND_Q(rmsg, ch->m_desc);
				return ;
			}

			if(ch->m_nJoinInzone_ZoneNo >= 0 && ch->m_nJoinInzone_RoomNo >= 0)
			{
				//이동을 못하는 경우 리콜 리스트에서 제거를 해준다.
				ch->m_party->deletePartyRecallMember(ch->m_index);

				CNetMsg::SP rmsg(new CNetMsg);
				HelperPartyRecallConfirmFail(rmsg);
				SEND_Q(rmsg, ch->m_desc);
				return ;
			}

			{
				CNetMsg::SP rmsg(new CNetMsg);
				HelperPartyRecallConfirmMsg(rmsg, ch->m_party->GetBossIndex(), reqcharindex, reqcharname, ch->m_index, ch->GetName(), yesno);
				SEND_Q(rmsg, gserver->m_helper);
			}

			if(!yesno)
			{
				// 거부
				CNetMsg::SP rmsg(new CNetMsg);
				ExPartyRecallCancelMsg(rmsg, reqcharindex, reqcharname, ch->m_index, ch->GetName());
				SEND_Q(rmsg, ch->m_desc);
			}
		}
		break;

	default:
		break;
	}
}

void do_ExMessenger(CPC* ch, CNetMsg::SP& msg)
{
	char subtype;

	RefMsg(msg) >> subtype;

	switch(subtype)
	{
	case MSG_EX_MESSENGER_ONE_VS_ONE:	// 1:1대화 : sCharIndex(n) tCharIndex(x) chat(str)
		{
			int sCharIndex, tCharIndex;
			CLCString chat(1000);

			RefMsg(msg) >> sCharIndex
						>> tCharIndex
						>> chat;

			// 친구 온라인 여부 확인
			CFriendMember * pMember = ch->m_Friend->FindFriendMember(tCharIndex);
			if( pMember == NULL ||
					pMember->GetCondition() == MSG_FRIEND_CONDITION_OFFLINE )
			{
				LOG_INFO("HACKING? invalid offline state : charIndex[%d]", ch->m_index);
				ch->m_desc->Close("invalid offline state");
				return;
			}
			else if(ch->m_nCondition == MSG_FRIEND_CONDITION_OFFLINE)
			{
				LOG_INFO("HACKING? invalid offline state : charIndex[%d]", ch->m_index);
				ch->m_desc->Close("invalid offline state");
				return;
			}
			else
			{
				{
					CNetMsg::SP rmsg(new CNetMsg);
					MsgrMessengerChatMsg(rmsg, MSG_MSGR_MESSENGER_ONE_VS_ONE, sCharIndex, tCharIndex,
										 ch->m_Friend->GetChatColor(), ch->GetName(), chat );
					SEND_Q(rmsg, gserver->m_messenger);
				}

				{
					CNetMsg::SP rmsg(new CNetMsg);
					ChatOneMessengerMsg(rmsg, sCharIndex, ch->GetName(), tCharIndex, "",
										ch->m_Friend->GetChatColor(), chat );
					SEND_Q( rmsg, ch->m_desc );
				}
			}
		}
		break;
	case MSG_EX_MESSENGER_INVITE:	// 해당 케릭터 초대 : makeCharIndex(n) chatIndex(n) charindex(n)	// chatIndex 가 -1이면 대화방 추가
		{
			int chatIndex, makeCharIndex, charIndex;
			RefMsg(msg) >> makeCharIndex
						>> chatIndex
						>> charIndex;

			GAMELOG << init( "MESSENGER_INVITE", ch )
					<< makeCharIndex << delim
					<< chatIndex << delim
					<< charIndex
					<< end;

			if( chatIndex == -1 )
			{
				// 대화방 생성일 경우
				// 대화방 생성
				makeCharIndex = ch->m_index;
				chatIndex = gserver->GetMaxChatIndexPerPC(makeCharIndex) + 1;
				//if ( result != -1 )
				//	gserver->SendChatGroup( (MSG_EX_MESSENGER_TYPE) MSG_EX_MESSENGER_INVITE , makeCharIndex, chatIndex, 0, ch->GetName(), "" );
				{
					CNetMsg::SP rmsg(new CNetMsg);
					MsgrMessengerChatMsg(rmsg, MSG_MSGR_MESSENGER_ADD, makeCharIndex, chatIndex, 0, ch->GetName() );
					SEND_Q(rmsg, gserver->m_messenger);
				}
			}

			// 초대 케릭터 추가
			{
				// 메신저로 보내면 거시기가 알아서 할꺼여
				CNetMsg::SP rmsg(new CNetMsg);
				MsgrMessengerChatMsg(rmsg, MSG_MSGR_MESSENGER_ADD_CONFIRM, makeCharIndex, chatIndex, 0, charIndex);
				SEND_Q(rmsg, gserver->m_messenger);
			}
		}
		break;
	case MSG_EX_MESSENGER_OUT:		// 해당 케릭터 대화방에서 나감 : chatIndex(n) charName(str)
		{
			int chatIndex, makeCharIndex;

			RefMsg(msg) >> makeCharIndex
						>> chatIndex;

			{
				// 메신저로 보내면 거시기가 알아서 할꺼여
				CNetMsg::SP rmsg(new CNetMsg);
				MsgrMessengerChatMsg(rmsg, MSG_MSGR_MESSENGER_DEL, makeCharIndex, chatIndex, 0, ch->GetName() );
				SEND_Q(rmsg, gserver->m_messenger);
			}
		}
		break;
	case MSG_EX_MESSENGER_CHAT:		// 해당 케릭터 대화방에서 나감 : chatIndex(n) chat(str)
		{
			CLCString chat(1000);
			int chatIndex, makeCharIndex;

			RefMsg(msg) >> makeCharIndex
						>> chatIndex
						>> chat;

			if( ch->m_Friend )
			{
				CNetMsg::SP rmsg(new CNetMsg);
				MsgrMessengerChatMsg(rmsg, MSG_MSGR_MESSENGER_CHAT, makeCharIndex, chatIndex, ch->m_Friend->GetChatColor(), ch->GetName(), chat);

				SEND_Q(rmsg, gserver->m_messenger);
			}
		}
		break;

	case MSG_EX_MESSENGER_GROUP_ADD:	// 친구 그룹 추가	: groupName(str)
		{
			CLCString groupName(MAX_GROUP_NAME_LENGTH+1);
			RefMsg(msg) >> groupName;

			int gIndex = ch->m_Friend->addGroup(groupName);

			{
				CNetMsg::SP rmsg(new CNetMsg);
				rmsg->Init(MSG_EXTEND);
				RefMsg(rmsg) << MSG_EX_MESSENGER
							 << (unsigned char) subtype
							 << gIndex
							 << groupName;

				SEND_Q(rmsg, ch->m_desc);
			}
		}
		break;
	case MSG_EX_MESSENGER_GROUP_DEL:	// 친구 그룹 삭제	: gIndex(n)
		{
			int gIndex;
			RefMsg(msg) >> gIndex;

			if( gIndex == 0 )
			{
				gIndex = -1;
			}
			else
				gIndex = ch->m_Friend->delGroup(gIndex);

			{
				CNetMsg::SP rmsg(new CNetMsg);
				rmsg->Init(MSG_EXTEND);
				RefMsg(rmsg) << MSG_EX_MESSENGER
							 << (unsigned char) subtype
							 << gIndex;

				SEND_Q(rmsg, ch->m_desc);
			}
		}
		break;
	case MSG_EX_MESSENGER_GROUP_CHANGE:	// 친구 그룹 이름변경 : gIndex(n) newName(str)
		{
			int gIndex;
			CLCString newName(MAX_GROUP_NAME_LENGTH+1);
			RefMsg(msg) >> gIndex
						>> newName;
			gIndex = ch->m_Friend->changeGroupName(gIndex, newName);

			{
				CNetMsg::SP rmsg(new CNetMsg);
				rmsg->Init(MSG_EXTEND);
				RefMsg(rmsg) << MSG_EX_MESSENGER
							 << (unsigned char) subtype
							 << gIndex
							 << newName;

				SEND_Q(rmsg, ch->m_desc);
			}
		}
		break;
	case MSG_EX_MESSENGER_GROUP_MOVE:	// 친구 그룹 이동	  : charName(str) gIndex(n)
		{
			int gIndex, charIndex;

			RefMsg(msg) >> gIndex
						>> charIndex;

			gIndex = ch->m_Friend->moveGroupFriend(gIndex, charIndex);

			{
				CNetMsg::SP rmsg(new CNetMsg);
				rmsg->Init(MSG_EXTEND);
				RefMsg(rmsg) << MSG_EX_MESSENGER
							 << (unsigned char) subtype
							 << gIndex
							 << charIndex;

				SEND_Q(rmsg, ch->m_desc);
			}
		}
		break;
	case MSG_EX_MESSENGER_CHATCOLOR_CHAGNE:
		{
			int color = 0;
			RefMsg(msg) >> color;

			ch->m_Friend->SetChatColor(color);
		}
		break;
	case MSG_EX_MESSENGER_BLOCK:		// 해당유저 차단	: name(str)
		// GS->C			: errcode(c) charIndex(n) name(str)
		{
			CLCString name(MAX_CHAR_NAME_LENGTH + 1);
			RefMsg(msg) >> name;

			if( strinc(name, "'") || strinc(name, "%") || strinc(name, " ") )
			{
				CNetMsg::SP rmsg(new CNetMsg);
				BlockPCRepMsg(rmsg, MSG_EX_MESSENGER_BLOCK_INVALIDNAME, -1, name);
				SEND_Q(rmsg, ch->m_desc);
				return;
			}

			// 최대 블럭 인원수 20명
			if( ch->m_listBlockPC.size() >= BLOCK_MAX_MEMBER )
			{
				CNetMsg::SP rmsg(new CNetMsg);
				BlockPCRepMsg(rmsg, MSG_EX_MESSENGER_NOMORE_BLOCK, -1, name);
				SEND_Q(rmsg, ch->m_desc);
				return;
			}

			// 헬퍼로 블럭 요청하고 헬퍼에서 인덱스와 이름을 받는다
			{
				CNetMsg::SP rmsg(new CNetMsg);
				rmsg->Init(MSG_HELPER_COMMAND);
				RefMsg(rmsg) << MSG_HELPER_BLOCKPC_REQ
							 << ch->m_index
							 << name;

				SEND_Q(rmsg, gserver->m_helper);
			}
		}
		break;
	case MSG_EX_MESSENGER_RELEASE_BLOCK:	// 해당유저 해제: charIndex(n)
		// GS->C			: errcode(c) charIndex(n) name(str)
		{
			int charIndex;

			RefMsg(msg) >> charIndex;
			CLCString name(MAX_CHAR_NAME_LENGTH + 1 );
			ch->ReleaseBlockPC(charIndex, name);

			{
				CNetMsg::SP rmsg(new CNetMsg);
				rmsg->Init(MSG_EXTEND);
				RefMsg(rmsg) << MSG_EX_MESSENGER
							 << (unsigned char) MSG_EX_MESSENGER_RELEASE_BLOCK;
				if( strcmp( name, "" ) == 0 )
					RefMsg(rmsg) << (unsigned char) MSG_EX_MESSENGER_BLOCK_NOTCHAR;
				else
					RefMsg(rmsg) << (unsigned char) MSG_EX_MESSENGER_SUCCESS;

				RefMsg(rmsg) << charIndex
							 << name;

				SEND_Q(rmsg, ch->m_desc);
			}

			if( ch->m_Friend )
			{
				CNetMsg::SP rmsg(new CNetMsg);
				HelperFriendSetConditionMsg(rmsg, ch->m_index, ch->m_nCondition, -1, ch);
				SEND_Q(rmsg, gserver->m_helper);
			}
		}
		break;
	}
}
void do_ExCastleWar(CPC * ch, CNetMsg::SP& msg)
{
	char subtype;

	RefMsg(msg) >> subtype;

	switch(subtype)
	{
	case MSG_CASTLE_CRISTAL_RESPOND_START:	// 교감 시작
		do_ExCastleCristalRespondStart(ch, msg);
		break;

	case MSG_CASTLE_CRISTAL_RESPOND_END:	// 교감 완료
		do_ExCastleCristalRespondEnd(ch, msg);
		break;

	case MSG_CASTLE_TOWER_CONTRAL_LIST:		// 타워 리스트
		do_ExCastleTowerList(ch, msg);
		break;

	case MSG_CASTLE_TOWER_CONTRAL:			// 타워 리스트 추가
		do_ExcastleTowerAdd(ch, msg);
		break;

	case MSG_CASTLE_TOWER_REINFORCE:			// 마스터 타워 강화 설정		type(c), step(c)
		do_ExCastleTowerReinforce(ch, msg);
		break;

	case MSG_CASTLE_TOWER_REINFORCE_LIST:	// 마스터 타워 강화 리스트		(type(c), step(c))
		do_ExCastleTowerReinforceList(ch, msg);
		break;

	case MSG_CASTLE_TOWER_REPAIRE:			// 공성 타워 수리				cl->gs idx(n) | gs->cl	idx(n)
		do_ExCastleTowerRepaire(ch, msg);
		break;

	case MSG_CASTLE_TOWER_REPAIRE_LIST:		// 공성 타워 수리 상태			cl->gs idx(n) | gs->cl	money(n)
		do_ExCastleTowerRepaireList(ch, msg);
		break;

	case MSG_CASTLE_TOWER_WARP_LIST:		// 워프 타워 리스트 요청
		do_ExCastleTowerWarpList(ch, msg);
		break;

	case MSG_CASTLE_QUARTERS_INSTALL:		// 부활진지 설치
		do_ExCastleQuartersInstall(ch, msg);
		break;
	}
}

void do_ExCastleCristalRespondStart(CPC * ch, CNetMsg::SP& msg)
{
	// 공성전 참가 확인
	if (ch->GetJoinFlag(ZONE_DRATAN) == WCJF_NONE)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		CastleErrorMsg(rmsg, MSG_EX_CASTLE_ERROR_NOT_JOIN);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 공성중 확인
	CDratanCastle * pCastle = CDratanCastle::CreateInstance();
	if( pCastle->GetState() == WCSF_NORMAL)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		CastleErrorMsg(rmsg, MSG_EX_CASTLE_ERROR_NOT_WAR);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 위치 확인
	if(ch->m_pZone->m_index != ZONE_DRATAN)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		CastleErrorMsg(rmsg, MSG_EX_CASTLE_ERROR_POS_FAIL);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 인원 확인 최대 5명
#ifdef BUGFIX_DRATAN_CRISTAL_RESPOND
	if(pCastle->GetRespondCount() >= 5)
#else
	if(pCastle->GetRespondCount() > 5)
#endif // BUGFIX_DRATAN_CRISTAL_RESPOND
	{
		CNetMsg::SP rmsg(new CNetMsg);
		CastleErrorMsg(rmsg, MSG_EX_CASTLE_ERROR_MEM_5_FAIL);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 성주 길드 확인
	if(pCastle->GetOwnerGuildIndex() == ch->m_guildInfo->guild()->index())
	{
		CNetMsg::SP rmsg(new CNetMsg);
		CastleErrorMsg(rmsg, MSG_EX_CASTLE_ERROR_NOT_OWNER);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// NPC(외성문 결계의 눈:388) 파괴 체크
	if( pCastle->m_gateNPC[0] && !DEAD(pCastle->m_gateNPC[0])
		&& pCastle->m_gateNPC[3] && !DEAD(pCastle->m_gateNPC[3])
		&& pCastle->m_gateNPC[4] && !DEAD(pCastle->m_gateNPC[4]))
	{
		CNetMsg::SP rmsg(new CNetMsg);
		CastleErrorMsg(rmsg, MSG_CASTLE_CRISTAL_RESPOND_FAIL);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// NPC(내성문 결계의 눈:389,404) 파괴 체크
	if( (pCastle->m_gateNPC[1] && !DEAD(pCastle->m_gateNPC[1]))
			&& ( pCastle->m_gateNPC[2] && !DEAD(pCastle->m_gateNPC[2])) )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		CastleErrorMsg(rmsg, MSG_CASTLE_CRISTAL_RESPOND_FAIL);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 인비저빌리티 상태에서는 크리스탈 교감 불가능
	if ( ch->IsInvisible() )
	{
		// 인비저빌리티 상태를 없애서 다굴당하게 함..
		ch->CancelInvisible();
		/*ch->m_assist.CureAssist(MST_ASSIST_INVISIBLE, 99);*/

		return;
	}

	// 레벨이 15보다 낮으면 크리스탈 교감 불가능
	if ( ch->m_level <= PKMODE_LIMIT_LEVEL )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		CastleErrorMsg(rmsg, MSG_CASTLE_CRISTAL_RESPOND_FAIL);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 죽었으면 크리스탈 교감 불가능
	if (DEAD(ch))
	{
		CNetMsg::SP rmsg(new CNetMsg);
		CastleErrorMsg(rmsg, MSG_CASTLE_CRISTAL_RESPOND_FAIL);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 캐릭터 상태 변화
	ch->SetPlayerState(PLAYER_CRISTAL_RESPOND);

	// 교감 시작 시간 저장
//	ch->SetRespondTime(gserver->m_pulse);
	time_t curTime;
	time(&curTime);
	ch->SetRespondTime(curTime);

	// 공성 교감 인원 추가
	pCastle->SetRespondMember(ch);

	{
		// 주위에 전달
		CNetMsg::SP rmsg(new CNetMsg);
		CastleCristalRespondStartMsg(rmsg, ch);
		ch->m_pArea->SendToCell(rmsg, ch, true);
	}
}

void do_ExCastleCristalRespondEnd(CPC * ch, CNetMsg::SP& msg)
{
	// 공성전 참가 확인
	if (ch->GetJoinFlag(ZONE_DRATAN) == WCJF_NONE)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		CastleErrorMsg(rmsg, MSG_EX_CASTLE_ERROR_NOT_JOIN);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 공성중 확인
	CDratanCastle * pCastle = CDratanCastle::CreateInstance();
	if( pCastle->GetState() == WCSF_NORMAL)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		CastleErrorMsg(rmsg, MSG_EX_CASTLE_ERROR_NOT_WAR);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 위치 확인
	if(ch->m_pZone->m_index != ZONE_DRATAN)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		CastleErrorMsg(rmsg, MSG_EX_CASTLE_ERROR_POS_FAIL);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 캐릭터 상태 확인
	if(ch->IsSetPlayerState(PLAYER_CRISTAL_RESPOND) == false)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		CastleErrorMsg(rmsg, MSG_EX_CASTLE_ERROR_NOT_RESPOND);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}
///===
	// 유지 시간 확인 3분
//	if(ch->GetRespondTime() == 0  //교감 시작 시간이 없거나
//		|| gserver->m_pulse	- ch->GetRespondTime() < 3*PULSE_REAL_MIN) // 3분이내면
//	{
//		CastleErrorMsg(rmsg, MSG_EX_CASTLE_ERROR_NOT_RESPOND_TIME);
//		SEND_Q(rmsg, ch->m_desc);
//		return;
//	}

	// 유지 시간 확인 3분
	time_t curTime;
	time(&curTime);
	if(ch->GetRespondTime() == 0  //교감 시작 시간이 없거나
			|| curTime	- ch->GetRespondTime() < 175 ) // 2분 55초이내면
	{
		CNetMsg::SP rmsg(new CNetMsg);
		CastleErrorMsg(rmsg, MSG_EX_CASTLE_ERROR_NOT_RESPOND_TIME);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 교감 정보 삭제
	pCastle->InitRespond();

	{
		// 주위에 전달
		CNetMsg::SP rmsg(new CNetMsg);
		CastleCristalRespondEndMsg(rmsg, ch);
		//	ch->m_pArea->SendToCell(rmsg, ch, true);
		ch->m_pArea->SendToAllClient(rmsg);
	}

	// 성주 바꿈
	CGuildMember * pMember = ch->m_guildInfo;
	if (pMember == NULL)
	{
		return;
	}
	CGuild * pGuild = pMember->guild() ;
	if (pGuild == NULL)
	{
		return;
	}

	GAMELOG << init("DRATAN CASTLE CHANGE OWNER")
			<< "before " << delim << pCastle->GetOwnerGuildIndex() << delim
			<< "after " << delim  << pGuild->index() << delim << "char " << delim << ch->m_index << end;

	// 공성 리셋
	// 죽은 부활진지 리젠  -071016 - whs25 추가 작업
	pCastle->RegenCastleControlNPC();

	// 해당 길드 부활 진지 존재 -> 부활 진지 삭제
	for(int i=0; i<7; i++)
	{
		if (pCastle->m_nRebrithGuild[i] == pGuild->index())
		{
			pCastle->m_nRebrithGuild[i] = -1;
			memset((void *)pCastle->m_strRebrithGuild[i], 0, 51);
			break;
		}
	}

	{
		// 부활 진지 정보 전송
		CNetMsg::SP rmsg(new CNetMsg);
		CastletowerQuartersListMsg(rmsg, pCastle);
		ch->m_pArea->SendToAllClient(rmsg);
	}

	// 성주 길드 플래그 <=> 공성 길드 플래그 변환
	pCastle->ChangeSide(pGuild->index());

	// 성주 길드 변환
	pCastle->SetOwner(pGuild);

	//  NPC_CASTLE_TOWER hp 1/2후 리젠
	pCastle->RegenCastleTowerHalf();

	//  해당 길드 이외 캐릭터 부활진지, 간이마을로 이동
	pCastle->GetOutNonDefense();

	{
		//	공성 정보 전송
		CNetMsg::SP rmsg(new CNetMsg);
		GuildWarCastleStateMsg(rmsg, ZONE_DRATAN, ch, pCastle);
		ch->m_pArea->SendToAllClient(rmsg);
	}
}

void do_ExCastleTowerList(CPC * ch, CNetMsg::SP& msg)
{
	// 리스트 요청
	// 캐릭터 확인
	CDratanCastle * pCastle = CDratanCastle::CreateInstance();
	if( pCastle->GetState() == WCSF_NORMAL				// 공성중이 아닐때
			&& pCastle->GetOwnerCharIndex() == ch->m_index		// 성주 일 경우
			&& ch->m_pZone->m_index == ZONE_DRATAN)				// 캐릭터가 드라탄에 있을때
	{
		CNetMsg::SP rmsg(new CNetMsg);
		CastleTowerListMsg(rmsg, pCastle);
		SEND_Q(rmsg, ch->m_desc);
	}
}

void do_ExcastleTowerAdd(CPC * ch, CNetMsg::SP& msg)
{
	char status[7];
	memset(status, 0, 7);

	for(int i = 0; i < 7; i++)
	{
		RefMsg(msg) >> status[i];
	}

	// 캐릭터 확인
	CDratanCastle * pCastle = CDratanCastle::CreateInstance();
	if( pCastle->GetState() == WCSF_NORMAL				// 공성중이 아닐때
			&& pCastle->GetOwnerCharIndex() == ch->m_index		// 성주 일 경우
			&& ch->m_pZone->m_index == ZONE_DRATAN)				// 캐릭터가 드라탄에 있을때
	{
		// 돈확인
		int nMoney = 1000000;
		int total_money = 0;

		int change_status = 0;
		for(int i = 0; i< 7; i++)
		{
			change_status = status[i] - pCastle->GetTowerStatus(i);
			if (change_status > 0)
			{
				// 변화 체크
				int count = status[i];
				for(int j = pCastle->GetTowerStatus(i); j < count; j++)
				{
					total_money += (j+1) * nMoney;
				}
			}
		}

		if (ch->m_inventory.getMoney() >= total_money)
		{
			ch->m_inventory.decreaseMoney(total_money);
		}
		else
		{
			// 돈부족 메세지 전송
			CNetMsg::SP rmsg(new CNetMsg);
			CastleErrorMsg(rmsg, MSG_EX_CASTLE_ERROR_NO_MONEY);
			SEND_Q(rmsg, ch->m_desc);
			return;
		}

		// 적용
		for(int k = 0; k < 7; k++)
		{
			pCastle->SetTowerStatus(k, status[k]);
		}

		{
			CNetMsg::SP rmsg(new CNetMsg);
			HelperCastleTowerAddMsg(rmsg, ZONE_DRATAN, pCastle);
			SEND_Q(rmsg, gserver->m_helper);
		}

		{
			CNetMsg::SP rmsg(new CNetMsg);
			CastleTowerAddMsg(rmsg, pCastle);
			SEND_Q(rmsg, ch->m_desc);
		}
	}
}

void do_ExCastleTowerReinforce(CPC * ch, CNetMsg::SP& msg)
{
	// 타워 강화
	// 수성 길드원인지 확인
	CDratanCastle * pCastle = CDratanCastle::CreateInstance();
	if (pCastle->GetOwnerGuildIndex() != ch->m_guildInfo->guild()->index())
	{
		CNetMsg::SP rmsg(new CNetMsg);
		CastleErrorMsg(rmsg, MSG_EX_CASTLE_ERROR_NOT_ATTACK);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	char type = 0, step = 0;
	RefMsg(msg) >> type >> step;

	// type : 0. 공격형 타워 1. 가드형 타워 2. 성문
	// step 1 - 3
	if (type < 0 || type > 3 || step <= 0 || step > 3)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		CastleErrorMsg(rmsg, MSG_EX_CASTLE_ERROR_SAME_DATA);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 이전과 비교
	char before_step = pCastle->GetReinforceStep((int)type);
	char diff = step - before_step;
	if (diff <= 0 || step > 3)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		CastleErrorMsg(rmsg, MSG_EX_CASTLE_ERROR_NO_DIFFER);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 가격 비교
	int total_money = 0;
	int price[3][3] =
	{
		{ 1000000, 1000000, 2000000 },
		{ 2000000, 2000000, 2000000 },
		{ 3000000, 3000000, 3000000 },
	};

	for(char i = step; i > before_step; i--)
	{
		total_money += price[(int)i-1][(int)type];
	}

	if(ch->m_inventory.getMoney() >= total_money)
	{
		ch->m_inventory.decreaseMoney(total_money);
	}
	else
	{
		// 돈부족 메세지 전송
		CNetMsg::SP rmsg(new CNetMsg);
		CastleErrorMsg(rmsg, MSG_EX_CASTLE_ERROR_NO_MONEY);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 적용
	pCastle->SetReinforceStep(type, step);

	{
		// 저장
		CNetMsg::SP rmsg(new CNetMsg);
		HelperCastleReinforceMsg(rmsg, type, step, ZONE_DRATAN);
		SEND_Q(rmsg, gserver->m_helper);
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		CastleTowerReinforceMsg(rmsg, type, pCastle);
		SEND_Q(rmsg, ch->m_desc);
	}

	///=== 성공 메세지 필요
}

void do_ExCastleTowerReinforceList(CPC * ch, CNetMsg::SP& msg)
{
	// 마스터 타워 강화 리스트
	char type = 0;
	RefMsg(msg) >> type;
	if( type < 0 || type > 2)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		CastleErrorMsg(rmsg, MSG_EX_CASTLE_ERROR_NO_DATA);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	CDratanCastle * pCastle = CDratanCastle::CreateInstance();
	if (pCastle->GetOwnerGuildIndex() == ch->m_guildInfo->guild()->index())
	{
		CNetMsg::SP rmsg(new CNetMsg);
		CastleTowerReinforceListMsg(rmsg, type, pCastle);
		SEND_Q(rmsg, ch->m_desc);
	}
}

void do_ExCastleTowerRepaire(CPC * ch, CNetMsg::SP& msg)
{
	// 수리 요청
	int idx = 0;
	RefMsg(msg) >> idx;

	if (idx < 351 || idx > 381)	// 마스터 타워 포함
	{
		CNetMsg::SP rmsg(new CNetMsg);
		CastleErrorMsg(rmsg, MSG_EX_CASTLE_ERROR_NO_DATA);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// npc리스트에서 검색
	CDratanCastle * pCastle = CDratanCastle::CreateInstance();
	if (pCastle->CheckBuyTower(idx) == false)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		CastleErrorMsg(rmsg, MSG_EX_CASTLE_ERROR_NOT_BUY);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	CNPCProto * pProto = gserver->m_npcProtoList.FindProto(idx);
	if( pProto == NULL)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		CastleErrorMsg(rmsg, MSG_EX_CASTLE_ERROR_NO_NPC);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	CZone* pZone = gserver->FindZone(ZONE_DRATAN);
	if (pZone == NULL)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		CastleErrorMsg(rmsg, MSG_EX_CASTLE_ERROR_POS_FAIL);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	CArea * pArea = pZone->m_area;
	if( pArea == NULL)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		CastleErrorMsg(rmsg, MSG_EX_CASTLE_ERROR_POS_FAIL);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 살아 있는지 검색
	CNPC * pNpc = pArea->FindProtoNPCInCell( ch, idx, false, 9, true);
	if (pNpc == NULL)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		CastleErrorMsg(rmsg, MSG_EX_CASTLE_ERROR_NO_NPC);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	if(DEAD(pNpc))
	{
		CNetMsg::SP rmsg(new CNetMsg);
		CastleErrorMsg(rmsg, MSG_EX_CASTLE_ERROR_DEAD_NPC);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	if( ch->m_guildInfo == NULL && ch->m_guildInfo->guild() == NULL)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		CastleErrorMsg(rmsg, MSG_EX_CASTLE_ERROR_PERMISSION_DENIED);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	if( ch->m_guildInfo->pos() != MSG_GUILD_POSITION_BOSS
			&& ch->m_guildInfo->pos() != MSG_GUILD_POSITION_OFFICER )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		CastleErrorMsg(rmsg, MSG_EX_CASTLE_ERROR_PERMISSION_DENIED);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 수리비용 검색
	LONGLONG total_money = 0;
	if (idx == 351)
	{
		// 마스터 타워
		total_money = 10000000;
	}
	else
	{
		total_money = ( ( (idx - 352) % 5) + 1 ) * 1000000;
	}

	total_money = total_money - (total_money * pNpc->m_hp) / (pNpc->m_maxHP);

	if(ch->m_inventory.getMoney() >= total_money)
	{
		ch->m_inventory.decreaseMoney(total_money);
	}
	else
	{
		// 돈부족 메세지 전송
		CNetMsg::SP rmsg(new CNetMsg);
		CastleErrorMsg(rmsg, MSG_EX_CASTLE_ERROR_NO_MONEY);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 적용
	pNpc->m_hp = pNpc->m_maxHP;
	pNpc->m_mp = pNpc->m_maxMP;

	{
		// 주변에 전송
		CNetMsg::SP rmsg(new CNetMsg);
		CharStatusMsg(rmsg, pNpc, 0);
		ch->m_pArea->SendToCell(rmsg, ch, true);
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		CastleTowerRepaireMsg(rmsg, idx, total_money);
		SEND_Q(rmsg, ch->m_desc);
	}
}

void do_ExCastleTowerRepaireList(CPC * ch, CNetMsg::SP& msg)
{
	// 수리 비용 전송
	int idx = 0;
	RefMsg(msg) >> idx;

	if (idx < 351 || idx > 381)	// 마스터 타워 포함
	{
		CNetMsg::SP rmsg(new CNetMsg);
		CastleErrorMsg(rmsg, MSG_EX_CASTLE_ERROR_NO_DATA);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// npc리스트에서 검색
	CDratanCastle * pCastle = CDratanCastle::CreateInstance();
	if (pCastle->CheckBuyTower(idx) == false)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		CastleErrorMsg(rmsg, MSG_EX_CASTLE_ERROR_NOT_BUY);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	CNPCProto * pProto = gserver->m_npcProtoList.FindProto(idx);
	if( pProto == NULL)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		CastleErrorMsg(rmsg, MSG_EX_CASTLE_ERROR_NO_NPC);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	CZone* pZone = gserver->FindZone(ZONE_DRATAN);
	if (pZone == NULL)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		CastleErrorMsg(rmsg, MSG_EX_CASTLE_ERROR_POS_FAIL);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	CArea * pArea = pZone->m_area;
	if (pArea == NULL)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		CastleErrorMsg(rmsg, MSG_EX_CASTLE_ERROR_POS_FAIL);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 살아 있는지 검색
	CNPC * pNpc = pArea->FindProtoNPCInCell(ch, idx, false, 9, true);
	if (pNpc == NULL)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		CastleErrorMsg(rmsg, MSG_EX_CASTLE_ERROR_NO_NPC);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	if(DEAD(pNpc))
	{
		CNetMsg::SP rmsg(new CNetMsg);
		CastleErrorMsg(rmsg, MSG_EX_CASTLE_ERROR_DEAD_NPC);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 수리비용 검색
	LONGLONG total_money = 0;
	if (idx == 351)
	{
		// 마스터 타워
		total_money = 10000000;
	}
	else
	{
		total_money = ( ( (idx - 352) % 5) + 1 ) * 1000000;
	}

	total_money = total_money - (total_money * pNpc->m_hp) / (pNpc->m_maxHP);

	{
		CNetMsg::SP rmsg(new CNetMsg);
		CastleTowerRepaireListMsg(rmsg, idx, total_money);
		SEND_Q(rmsg, ch->m_desc);
	}
}

void do_ExCastleTowerWarpList(CPC * ch, CNetMsg::SP& msg)
{
	int curindex = 0;
	RefMsg(msg) >> curindex;

	// 워프타워 번호 확인
	if (curindex < 382 || curindex > 386)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		CastleErrorMsg(rmsg, MSG_EX_CASTLE_ERROR_NO_DATA);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	CDratanCastle * pCastle = CDratanCastle::CreateInstance();
	// 가동중인 워프 타워 확인
	unsigned char total = 0;
	int aliveindex[5];
	for(unsigned char i = 0; i < 5; i++)
	{
		if(pCastle->m_pWarpNPC[i] == NULL)
		{
			continue;
		}

		CNPCProto * pProto = pCastle->m_pWarpNPC[i]->m_proto;
		if (pProto == NULL)
		{
			continue;
		}
		aliveindex[total] = pCastle->m_pWarpNPC[i]->m_proto->m_index;
		total++;
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		CastleTowerWarpListMsg(rmsg, aliveindex, total);
		SEND_Q(rmsg, ch->m_desc);
	}
}

void do_ExCastleQuartersInstall(CPC * ch, CNetMsg::SP& msg)
{
	int qindex = 0;

	RefMsg(msg) >> qindex;

	// 공성확인
	CDratanCastle * pCastle = CDratanCastle::CreateInstance();
	// 부활진지 번호 확인
	if(qindex < 390 || qindex > 396)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		CastleErrorMsg(rmsg, MSG_EX_CASTLE_ERROR_NO_DATA);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 캐릭터의 길드 확인
	CGuildMember * pMember = ch->m_guildInfo;
	if (pMember == NULL)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		CastleErrorMsg(rmsg, MSG_EX_CASTLE_ERROR_HAVE_NO_GUILD);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	CGuild * pGuild = pMember->guild();
	if (pGuild == NULL)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		CastleErrorMsg(rmsg, MSG_EX_CASTLE_ERROR_HAVE_NO_GUILD);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	int gindex = pGuild->index();
	if (gindex < 0)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		CastleErrorMsg(rmsg, MSG_EX_CASTLE_ERROR_HAVE_NO_GUILD);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 공성길드 확인
	if (ch->GetJoinFlag(ZONE_DRATAN) != WCJF_ATTACK_GUILD)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		CastleErrorMsg(rmsg, MSG_EX_CASTLE_ERROR_NOT_ATTACK);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 선택한 부활진지 살아있는지 확인
	int idx = qindex - 390;
	if (pCastle->m_pRebrithNPC[idx] == NULL)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		CastleErrorMsg(rmsg, MSG_EX_CASTLE_ERROR_NO_NPC2);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	if (DEAD(pCastle->m_pRebrithNPC[idx]))
	{
		CNetMsg::SP rmsg(new CNetMsg);
		CastleErrorMsg(rmsg, MSG_EX_CASTLE_ERROR_DEAD_NPC2);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 선택한 부활 진지 사용중 확인
	if (pCastle->m_nRebrithGuild[idx] != -1)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		CastleErrorMsg(rmsg, MSG_EX_CASTLE_ERROR_USE_NPC);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 다른 부활진지 사용중 확인
	for(int i=0; i<7; i++)
	{
		if (pCastle->m_nRebrithGuild[i] == gindex)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			CastleErrorMsg(rmsg, MSG_EX_CASTLE_ERROR_USE_NPC_OTHER);
			SEND_Q(rmsg, ch->m_desc);
			return;
		}
	}

	const int nNeedMoney[10] = { 3000000, 2000000, 2000000, 2000000, 2000000,
								 1000000, 1000000, 1000000, 1000000, 1000000
							   };

	if( ch->m_inventory.getMoney() < nNeedMoney[idx] )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		CastleErrorMsg(rmsg, MSG_EX_CASTLE_ERROR_NO_MONEY); 	// 돈이 부족합니다. c
		SEND_Q(rmsg, ch->m_desc);
		return;
	}
	else
	{
		ch->m_inventory.decreaseMoney(nNeedMoney[idx]);
	}

	// 몹의 리스트 인덱스
	int list_index = 0;
	list_index = pCastle->m_pRebrithNPC[idx]->m_index;

	// 부활 진지 설치
	pCastle->m_nRebrithGuild[idx] = gindex;
	strncpy(pCastle->m_strRebrithGuild[idx], pGuild->name(), 51);

	{
		CNetMsg::SP rmsg(new CNetMsg);
		HeplerCastleRebrithInstallMsg(rmsg, qindex, gindex, pGuild->name());
		SEND_Q(rmsg, gserver->m_helper);
	}

	{
		// 090106 yhj  함수 수정함..매개변수 한개 추가
		CNetMsg::SP rmsg(new CNetMsg);
		CastleTowerRebrithInstallMsg(rmsg, qindex, gindex, pGuild->name(), list_index);
		ch->m_pArea->SendToAllClient(rmsg);
	}

	// 부활 진지 사용 로그
	GAMELOG << init("WAR CASTLE : BUY QUARTERS") << delim << "QUARTER index" << delim << qindex
			<< delim << "GUILD index" << delim << gindex << delim << "GUILD name" << delim << pGuild->name() << end;
}

void do_DVD(CPC * ch, CNetMsg::SP& msg)
{
	CDratanCastle * pCastle = CDratanCastle::CreateInstance();
	if (pCastle->m_dvd.GetZone() == 0)
	{
		// 드라탄 공성 던전 지역이 없을때
		return;
	}

	char subtype;
	RefMsg(msg) >> subtype;

	switch(subtype)
	{
	case MSG_DUNGEON_ENTER:
		do_dvdDungenEnter( ch, msg );
		break;

	case MSG_DUNGEON_INFO :
		do_dvdInfo(ch, msg);
		break;

	case MSG_MANAGEMENT:
		do_dvdManagement( ch, msg );
		break;
	}
}

void do_dvdManagement(CPC * ch, CNetMsg::SP& msg)
{
	char	subType;
	int		nRate;

#ifdef NPC_CHECK
	int npcIndex;
	CNPC *npc;
	RefMsg(msg) >> subType
				>> npcIndex;
	npc = ch->m_pArea->FindProtoNPCInCell(ch, npcIndex, false, 2);
	if(!npc)
	{
		GAMELOG << init("DVD Management FAIL: NOT EXIST NPC (Dangerous User)", ch) << end;
		return ;
	}
	if( !npc->IsFlag(NPC_ZONEMOVER) && !npc->IsFlag(NPC_WARCASTLE) )
	{
		GAMELOG << init("DVD Management FAIL: NOT EXIST NPC (Dangerous User)", ch) << end;
		return ;
	}
#else
	RefMsg(msg) >> subType;
#endif

	CDratanCastle * pCastle = CDratanCastle::CreateInstance();
	if( ch == NULL )
	{
		// ERROR
		CNetMsg::SP rmsg(new CNetMsg);
		DVDManagementMsg( rmsg, MSG_MANAGEMENT_ETC );
		RefMsg(rmsg) << (unsigned char) MSG_DVD_ERROR_ZONE;
		SEND_Q( rmsg, ch->m_desc );
		return;
	}

	//일반 관리 메뉴 추가
	if( subType != MSG_MANAGEMENT_ENV_INFO 
		&& subType != MSG_MANAGEMENT_STATE_INFO 
		&& subType != MSG_MANAGEMENT_ENV_CHANGE 
		&& subType != MSG_MANAGEMENT_STATE_CHANGE 
		&& subType != MSG_MANAGEMENT_MANAGER_CONFIRM_NORMAL)
	{
		if( ch->m_guildInfo == NULL)
		{
			// ERROR 길드 없음
			CNetMsg::SP rmsg(new CNetMsg);
			DVDManagementMsg( rmsg, MSG_MANAGEMENT_MANAGER_CONFIRM );
			RefMsg(rmsg) << (unsigned char) MSG_DVD_ERROR_CONFIRM;
			SEND_Q( rmsg, ch->m_desc );
			return;
		}
	}
	
	switch(subType)
	{
	case MSG_MANAGEMENT_MANAGER_CONFIRM :
		{
			if( gserver->m_subno != WAR_CASTLE_SUBNUMBER_DRATAN )
			{
				// ERROR CONFIRM FAIL
				CNetMsg::SP rmsg(new CNetMsg);
				DVDManagementMsg( rmsg, MSG_MANAGEMENT_MANAGER_CONFIRM );
				RefMsg(rmsg) << (unsigned char) MSG_DVD_ERROR_ZONE;
				SEND_Q( rmsg, ch->m_desc );
				break;
			}

			if( ch->m_guildInfo == NULL )
			{
				LOG_ERROR("HACKING? : GuildInfo is NULL. charIndex[%d]", ch->m_index);
				ch->m_desc->Close("GuildInfo is NULL");
				return;
			}

			if( ch->m_guildInfo->guild()->index() == pCastle->GetOwnerGuildIndex()
					&& ch->m_guildInfo->pos() < 2 )
			{
				if(pCastle->m_dvd.GetOwnerMode() == false)
				{
					GoldType_t nas = 0;

					int time = gserver->getNowSecond() - pCastle->m_dvd.GetChangeNormalTime();
					nas = ((LONGLONG)time / (60 * 60) + 1) * 3000000;

					if( nas >= 36000000 )
						nas = 36000000;

					pCastle->m_dvd.setChangeNeedNas(nas);

					CNetMsg::SP rmsg(new CNetMsg);
					DVDManagementMsg( rmsg, MSG_MANAGEMENT_MANAGER_CONFIRM );
					RefMsg(rmsg) << (unsigned char) MSG_DVD_ERROR_NORMAL_MODE
						<< nas;
					SEND_Q( rmsg, ch->m_desc );
				}
				else
				{
					// ERROR OK
					CNetMsg::SP rmsg(new CNetMsg);
					DVDManagementMsg( rmsg, MSG_MANAGEMENT_MANAGER_CONFIRM );
					RefMsg(rmsg) << (unsigned char) MSG_DVD_ERROR_OK;
					SEND_Q( rmsg, ch->m_desc );
				}
			}
			else
			{
				// ERROR CONFIRM FAIL
				CNetMsg::SP rmsg(new CNetMsg);
				DVDManagementMsg( rmsg, MSG_MANAGEMENT_MANAGER_CONFIRM );
				RefMsg(rmsg) << (unsigned char) MSG_DVD_ERROR_CONFIRM;
				SEND_Q( rmsg, ch->m_desc );
			}
		}
		break;
		// INFO REQ
	case MSG_MANAGEMENT_ENV_INFO :
		{
			CNetMsg::SP rmsg(new CNetMsg);
			DVDManagementMsg( rmsg, MSG_MANAGEMENT_ENV_INFO );
			RefMsg(rmsg) << pCastle->m_dvd.GetEnvRate();
			if(pCastle->m_dvd.GetOwnerMode() == false)
			{
				RefMsg(rmsg) << (LONGLONG)CASTLE_DUNGEON_ENV_CHANGE_NEED_NAS_NORMAL;
			}
			else
			{
				RefMsg(rmsg) << (LONGLONG)CASTLE_DUNGEON_ENV_CHANGE_NEED_NAS_OWNER;
			}
			SEND_Q( rmsg, ch->m_desc );
		}
		break;
	case MSG_MANAGEMENT_STATE_INFO :
		{
			CNetMsg::SP rmsg(new CNetMsg);
			DVDManagementMsg( rmsg, MSG_MANAGEMENT_STATE_INFO );
			RefMsg(rmsg) << pCastle->m_dvd.GetMobRate();
			if(pCastle->m_dvd.GetOwnerMode() == false)
			{
				RefMsg(rmsg) << (LONGLONG)CASTLE_DUNGEON_ENV_CHANGE_NEED_NAS_NORMAL;
			}
			else
			{
				RefMsg(rmsg) << (LONGLONG)CASTLE_DUNGEON_ENV_CHANGE_NEED_NAS_OWNER;
			}
			SEND_Q( rmsg, ch->m_desc );
		}
		break;
	case MSG_MANAGEMENT_TAX_INFO :
		{
			CNetMsg::SP rmsg(new CNetMsg);
			DVDManagementMsg( rmsg, MSG_MANAGEMENT_TAX_INFO );
			RefMsg(rmsg) << pCastle->m_dvd.GetFeeRate();
			SEND_Q( rmsg, ch->m_desc );
		}
		break;
	case MSG_MANAGEMENT_HUNTER_TAX_INFO :
		{
			CNetMsg::SP rmsg(new CNetMsg);
			DVDManagementMsg( rmsg, MSG_MANAGEMENT_HUNTER_TAX_INFO );
			RefMsg(rmsg) << pCastle->m_dvd.GetHuntRate();
			SEND_Q( rmsg, ch->m_desc );
		}
		break;

		// CHANGE REQ
	case MSG_MANAGEMENT_ENV_CHANGE :
		{
			RefMsg(msg) >> nRate;

			if( ch->m_guildInfo != NULL
				&& ch->m_guildInfo->guild()->index() != pCastle->GetOwnerGuildIndex())
			{
				if(pCastle->m_dvd.GetOwnerMode() == true)
				{
					LOG_ERROR("HACKING? Invalid ENV CHANGE INFO EnvRate : %d, MobRate : %d",  pCastle->m_dvd.GetEnvRate(), pCastle->m_dvd.GetMobRate());
					ch->m_desc->Close("Invalid ENV CHANGE INFO");
					return;
				}
			}

			if (nRate <= 0)
				return ;

			LONGLONG needMoney = 0;

			if(pCastle->m_dvd.GetOwnerMode() == true)
			{
				needMoney = nRate/10 * 500000;
			}
			else
			{
				needMoney = nRate/10 * 1000000;
			}

			if(	ch->m_inventory.getMoney() < needMoney)
			{
				// ERROR 돈 없음
				CNetMsg::SP rmsg(new CNetMsg);
				DVDManagementMsg( rmsg, MSG_MANAGEMENT_ENV_CHANGE );
				RefMsg(rmsg) << (unsigned char) MSG_DVD_ERROR_MONEY;
				SEND_Q( rmsg, ch->m_desc );
				return;
			}
			else
			{
				ch->m_inventory.decreaseMoney(needMoney);
			}

			pCastle->m_dvd.ChangeEnvRate(nRate);

			{
				// 헬퍼에 변경 요청 nRate
				CNetMsg::SP rmsg(new CNetMsg);
				HelperDVDRateChangeMsg(rmsg, MSG_MANAGEMENT_ENV_CHANGE , pCastle->m_dvd.GetEnvRate(), pCastle->m_dvd.GetOwnerMode() );
				SEND_Q(rmsg, gserver->m_helper);
			}

			{
				// 변경 완료 메세지
				CNetMsg::SP rmsg(new CNetMsg);
				DVDManagementMsg( rmsg, MSG_MANAGEMENT_ENV_CHANGE );
				RefMsg(rmsg) << (unsigned char) MSG_DVD_ERROR_OK;
				SEND_Q( rmsg, ch->m_desc );
			}

			GAMELOG << init(" DRATAN DYNAMIC DUNGEON ENVIRONMENT CHANGE " , ch ) << delim
				<< "CHNAGE RATE : " << nRate << delim
				<< "CURRENT RATE : " << pCastle->m_dvd.GetEnvRate() << delim
				<< "Spend Money : " << needMoney << end;
		}
		break;

	case MSG_MANAGEMENT_STATE_CHANGE :
		{
			// 돈검사 10%당 1000000
			RefMsg(msg) >> nRate;
			if (nRate <= 0)
				return ;

			if(ch->m_guildInfo != NULL 
				&& ch->m_guildInfo->guild()->index() != pCastle->GetOwnerGuildIndex())
			{
				if(pCastle->m_dvd.GetOwnerMode() == true)
				{
					LOG_ERROR("HACKING? Invalid ENV CHANGE INFO EnvRate : %d, MobRate : %d",  pCastle->m_dvd.GetEnvRate(), pCastle->m_dvd.GetMobRate());
					ch->m_desc->Close("Invalid ENV CHANGE INFO");
					return;
				}
			}

			LONGLONG needMoney = 0;

			if(pCastle->m_dvd.GetOwnerMode() == true)
			{
				needMoney = nRate/10 * 500000;
			}
			else
			{
				needMoney = nRate/10 * 1000000;
			}

			if(	ch->m_inventory.getMoney() < needMoney)
			{
				// ERROR 돈 없음
				CNetMsg::SP rmsg(new CNetMsg);
				DVDManagementMsg( rmsg, MSG_MANAGEMENT_ENV_CHANGE );
				RefMsg(rmsg) << (unsigned char) MSG_DVD_ERROR_MONEY;
				SEND_Q( rmsg, ch->m_desc );
				return;
			}
			else
			{
				ch->m_inventory.decreaseMoney(needMoney);
			}

			{
				// 헬퍼에 변경 요청 nRate
				pCastle->m_dvd.ChangeMobRate(nRate);
				CNetMsg::SP rmsg(new CNetMsg);
				HelperDVDRateChangeMsg(rmsg, MSG_MANAGEMENT_STATE_CHANGE , pCastle->m_dvd.GetMobRate(), pCastle->m_dvd.GetOwnerMode() );
				SEND_Q(rmsg, gserver->m_helper);
			}

			{
				CNetMsg::SP rmsg(new CNetMsg);
				DVDManagementMsg( rmsg, MSG_MANAGEMENT_STATE_CHANGE );
				RefMsg(rmsg) << (unsigned char) MSG_DVD_ERROR_OK;
				SEND_Q( rmsg, ch->m_desc );
			}

			GAMELOG << init(" DRATAN DYNAMIC DUNGEON MONSTER RATE CHANGE " , ch ) << delim
				<< "CHNAGE RATE : " << nRate << delim
				<< "CURRENT RATE : " << pCastle->m_dvd.GetMobRate() << delim
				<< "Spend Money : " << needMoney << end;
		}
		break;

	case MSG_MANAGEMENT_TAX_CHANGE :
		{
			RefMsg(msg) >> nRate;
			if (nRate <= 0)
				return ;
			pCastle->m_dvd.ChangeFeeRate(nRate);

			{
				// 헬퍼에 변경 요청 nRate
				CNetMsg::SP rmsg(new CNetMsg);
				HelperDVDRateChangeMsg(rmsg, MSG_MANAGEMENT_TAX_CHANGE , pCastle->m_dvd.GetFeeRate(), pCastle->m_dvd.GetOwnerMode() );
				SEND_Q(rmsg, gserver->m_helper);
			}

			{
				CNetMsg::SP rmsg(new CNetMsg);
				DVDManagementMsg( rmsg, MSG_MANAGEMENT_TAX_CHANGE );
				RefMsg(rmsg) << (unsigned char) MSG_DVD_ERROR_OK;
				SEND_Q( rmsg, ch->m_desc );
			}

			GAMELOG << init(" DRATAN DYNAMIC DUNGEON TAX CHANGE " , ch ) << delim
				<< "CHNAGE RATE : " << nRate << end;
		}
		break;

	case MSG_MANAGEMENT_HUNTER_TAX_CHANGE :
		{
			RefMsg(msg) >> nRate;
			if (nRate <= 0)
				return ;
			pCastle->m_dvd.ChangeHuntRate(nRate);

			{
				// 헬퍼에 변경 요청 nRate
				CNetMsg::SP rmsg(new CNetMsg);
				HelperDVDRateChangeMsg(rmsg, MSG_MANAGEMENT_HUNTER_TAX_CHANGE , pCastle->m_dvd.GetHuntRate(), pCastle->m_dvd.GetOwnerMode() );
				SEND_Q(rmsg, gserver->m_helper);
			}

			{
				CNetMsg::SP rmsg(new CNetMsg);
				DVDManagementMsg( rmsg, MSG_MANAGEMENT_HUNTER_TAX_CHANGE );
				RefMsg(rmsg) << (unsigned char) MSG_DVD_ERROR_OK;
				SEND_Q( rmsg, ch->m_desc );
			}

			GAMELOG << init(" DRATAN DYNAMIC DUNGEON HUNTER TAX CHANGE " , ch ) << delim
					<< "CHNAGE RATE : " << nRate << end;
		}
		break;
		
	case MSG_MANAGEMENT_MANAGER_CONFIRM_NORMAL:
		{
			if( gserver->m_subno != WAR_CASTLE_SUBNUMBER_DRATAN )
			{
				// ERROR CONFIRM FAIL
				CNetMsg::SP rmsg(new CNetMsg);
				DVDManagementMsg( rmsg, MSG_MANAGEMENT_MANAGER_CONFIRM_NORMAL );
				RefMsg(rmsg) << (unsigned char) MSG_DVD_ERROR_ZONE;
				SEND_Q( rmsg, ch->m_desc );
				break;
			}

			if( ch->m_guildInfo != NULL 
				&& ch->m_guildInfo->guild()->index() == pCastle->GetOwnerGuildIndex()
				&& ch->m_guildInfo->pos() < 2 )
			{
				if(pCastle->m_dvd.GetOwnerMode() == false)
				{
					// ERROR CONFIRM NORMAL SUCCESS
					CNetMsg::SP rmsg(new CNetMsg);
					DVDManagementMsg( rmsg, MSG_MANAGEMENT_MANAGER_CONFIRM_NORMAL );
					RefMsg(rmsg) << (unsigned char) MSG_DVD_ERROR_NORMAL_MODE;
					SEND_Q( rmsg, ch->m_desc );
					break;
				}
				else
				{
					// ERROR CONFIRM NORMAL FAIL
					CNetMsg::SP rmsg(new CNetMsg);
					DVDManagementMsg( rmsg, MSG_MANAGEMENT_MANAGER_CONFIRM_NORMAL );
					RefMsg(rmsg) << (unsigned char) MSG_DVD_ERROR_CONFIRM;
					SEND_Q( rmsg, ch->m_desc );
					break;
				}
			}

			if(pCastle->m_dvd.GetOwnerMode() == false)
			{
				// ERROR CONFIRM NORMAL SUCCESS
				CNetMsg::SP rmsg(new CNetMsg);
				DVDManagementMsg( rmsg, MSG_MANAGEMENT_MANAGER_CONFIRM_NORMAL );
				RefMsg(rmsg) << (unsigned char) MSG_DVD_ERROR_OK;
				SEND_Q( rmsg, ch->m_desc );
				break;
			}
			else
			{
				// ERROR CONFIRM NORMAL FAIL
				CNetMsg::SP rmsg(new CNetMsg);
				DVDManagementMsg( rmsg, MSG_MANAGEMENT_MANAGER_CONFIRM_NORMAL );
				RefMsg(rmsg) << (unsigned char) MSG_DVD_ERROR_CONFIRM;
				SEND_Q( rmsg, ch->m_desc );
				break;
			}
		}
	case MSG_NAMAGEMENT_CHANGE_OWNER_MODE:
		
		if(ch->m_guildInfo == NULL)
		{
			LOG_ERROR("HACKING? : GuildInfo is NULL. charIndex[%d]", ch->m_index);
			ch->m_desc->Close("GuildInfo is NULL");
			return;
		}
		if( ch->m_guildInfo->guild()->index() == pCastle->GetOwnerGuildIndex()
			&& ch->m_guildInfo->pos() < 2 )
		{
			GoldType_t needNas = pCastle->m_dvd.getChangeNeedNas();

			if(	ch->m_inventory.getMoney() < needNas)
			{
				// ERROR 돈 없음
				CNetMsg::SP rmsg(new CNetMsg);
				DVDManagementMsg( rmsg, MSG_NAMAGEMENT_CHANGE_OWNER_MODE );
				RefMsg(rmsg) << (unsigned char) MSG_DVD_ERROR_MONEY;
				SEND_Q( rmsg, ch->m_desc );
				return;
			}
			else
			{
				ch->m_inventory.decreaseMoney(needNas);
			}
		}
		else
		{
			CNetMsg::SP rmsg(new CNetMsg);
			DVDManagementMsg( rmsg, MSG_NAMAGEMENT_CHANGE_OWNER_MODE );
			RefMsg(rmsg) << (unsigned char) MSG_DVD_ERROR_CONFIRM;
			SEND_Q( rmsg, ch->m_desc );
			return;
		}
		
		pCastle->m_dvd.setOwnerMode(true);
		
		{
			// 헬퍼에 변경 요청 nRate
			pCastle->m_dvd.ChangeMobRate(100);
			CNetMsg::SP rmsg(new CNetMsg);
			HelperDVDRateChangeMsg(rmsg, MSG_MANAGEMENT_STATE_CHANGE , pCastle->m_dvd.GetMobRate(), pCastle->m_dvd.GetOwnerMode() );
			SEND_Q(rmsg, gserver->m_helper);
		}

		{
			CNetMsg::SP rmsg(new CNetMsg);
			DVDManagementMsg( rmsg, MSG_MANAGEMENT_STATE_CHANGE );
			RefMsg(rmsg) << (unsigned char) MSG_DVD_ERROR_OK;
			SEND_Q( rmsg, ch->m_desc );
		}

		{
			// 헬퍼에 변경 요청 nRate
			pCastle->m_dvd.ChangeEnvRate(100);
			CNetMsg::SP rmsg(new CNetMsg);
			HelperDVDRateChangeMsg(rmsg, MSG_MANAGEMENT_ENV_CHANGE , pCastle->m_dvd.GetEnvRate(), pCastle->m_dvd.GetOwnerMode() );
			SEND_Q(rmsg, gserver->m_helper);
		}

		{
			// 변경 완료 메세지
			CNetMsg::SP rmsg(new CNetMsg);
			DVDManagementMsg( rmsg, MSG_MANAGEMENT_ENV_CHANGE );
			RefMsg(rmsg) << (unsigned char) MSG_DVD_ERROR_OK;
			SEND_Q( rmsg, ch->m_desc );
		}

		{
			//성주모드 변경 완료 메시지
			CNetMsg::SP rmsg(new CNetMsg);
			DVDManagementMsg( rmsg, MSG_NAMAGEMENT_CHANGE_OWNER_MODE);
			RefMsg(rmsg) << (unsigned char) MSG_DVD_ERROR_OK;
			SEND_Q( rmsg, ch->m_desc );
		}		
		break;
	}
}

void do_dvdDungenEnter( CPC *ch, CNetMsg::SP& msg )
{
	LONGLONG needMoney=0;

	CDratanCastle * pCastle = CDratanCastle::CreateInstance();
	if (pCastle->m_dvd.GetZone() == NULL)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		DVDManagementMsg( rmsg, MSG_MANAGEMENT_ETC );
		RefMsg(rmsg) << (unsigned char) MSG_DVD_ERROR_ZONE;
		SEND_Q( rmsg, ch->m_desc );
		return;
	}

#ifdef LC_TLD
	time_t curTime;
	time(&curTime);
	int nextTime = pCastle->GetNextWarTime();
	if(curTime + 60 > nextTime)
	{
		GAMELOG << init("DRATAN DYNAMIC DUNGEON TIME ERROR", ch)
				<< "NOW" << delim << (int)curTime << delim
				<< "NEXT WARTIME" << delim << nextTime
				<< end;
		return ;
	}
#endif // LC_TLD

	unsigned char subType;
	RefMsg(msg) >> subType;

	if( ch->m_guildInfo != NULL )
	{
		// 성주 길드이면 0원
		if( ch->m_guildInfo->guild()->index() == pCastle->GetOwnerGuildIndex() )
		{
			needMoney = 0;
		}
		else
		{
			needMoney = ( ch->m_level * 1000 ) * 2 * ( pCastle->m_dvd.GetFeeRate() ) / 100 ;
		}
	}
	else
	{
		needMoney = ( ch->m_level * 1000 ) * 2 * ( pCastle->m_dvd.GetFeeRate() ) / 100;
	}

	switch( subType )
	{
	case MSG_DUNGEON_ENTER_FEE :
		{
			CNetMsg::SP rmsg(new CNetMsg);
			DVDDungeonEnterMsg( rmsg, MSG_DUNGEON_ENTER_FEE );
			RefMsg(rmsg) <<  needMoney;
			SEND_Q( rmsg, ch->m_desc );
		}
		break;

	case MSG_DUNGEON_GO :
		{
			if( needMoney > 0 )
			{
				if(	ch->m_inventory.getMoney() < needMoney)
				{
					// ERROR 돈 없음
					CNetMsg::SP rmsg(new CNetMsg);
					DVDDungeonEnterMsg( rmsg, MSG_DUNGEON_GO );
					RefMsg(rmsg) << (unsigned char) MSG_DVD_ERROR_MONEY;
					SEND_Q( rmsg, ch->m_desc );
					return;
				}
				else
				{
					ch->m_inventory.decreaseMoney(needMoney);
				}

				//세금 지급
				LONGLONG nTax = needMoney - ( ch->m_level * 1000 );
				if( nTax > 0 )
					gserver->AddTaxItemDratan( needMoney - ( nTax ) );
			}

			CZone* pZone = gserver->FindZone( ZONE_DRATAN_CASTLE_DUNGEON );
			if( pZone )
			{
				int extra = 0;
				GoZone(ch, pZone->m_index ,
					   pZone->m_zonePos[extra][0],														// ylayer
					   GetRandom(pZone->m_zonePos[extra][1], pZone->m_zonePos[extra][3]) / 2.0f,		// x
					   GetRandom(pZone->m_zonePos[extra][2], pZone->m_zonePos[extra][4]) / 2.0f);		// z
			}
		}
		break;
	}
}

void do_dvdInfo(CPC * ch, CNetMsg::SP& msg)
{
#ifdef NPC_CHECK
	int npcIndex;
	CNPC *npc;
	RefMsg(msg) >> npcIndex;
	npc = ch->m_pArea->FindProtoNPCInCell(ch, npcIndex, false, 2);
	if(!npc)
	{
		GAMELOG << init("DVD Info FAIL: NOT EXIST NPC (Dangerous User)", ch) << end;
		return ;
	}
	if( !npc->IsFlag(NPC_WARCASTLE) && !npc->IsFlag(NPC_ZONEMOVER) )
	{
		GAMELOG << init("DVD Info FAIL: NOT EXIST NPC (Dangerous User)", ch) << end;
		return ;
	}
#endif
	CDratanCastle * pCastle = CDratanCastle::CreateInstance();
	if (pCastle->m_dvd.GetZone() == NULL)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		DVDManagementMsg( rmsg, MSG_MANAGEMENT_ETC );
		RefMsg(rmsg) << (unsigned char) MSG_DVD_ERROR_ZONE;
		SEND_Q( rmsg, ch->m_desc );
		return;
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		DVDInfoMsg(ch, rmsg, pCastle->m_dvd.GetEnvRate(), pCastle->m_dvd.GetMobRate() );
		SEND_Q(rmsg, ch->m_desc);
	}
}

void do_ClientRestart( CPC* ch, CNetMsg::SP& msg )
{
	GAMELOG << init( "CLIENT_RESTART", ch ) << end;

	{
		// 클라이언트에 restart MSG 전송
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::makeExRestart(rmsg, 0);
		SEND_Q( rmsg, ch->m_desc );
	}

	//이보케이션 상태 풀기
	if(ch->m_evocationIndex > EVOCATION_NONE)
	{
		ch->Unevocation();
	}

	ch->m_desc->returnToCharacterSelectMode();
}

void do_ExInitSSkill( CPC* ch, CNetMsg::SP& msg )
{
	if(!ch->InitSSkill())
	{
		CNetMsg::SP rmsg(new CNetMsg);
		InitSSkillMsg(rmsg, MSG_EX_INIT_SSkill_ERROR_FAIL);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	ch->CalcStatus(true);

	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::makeSSkillList(rmsg, ch);
		SEND_Q(rmsg, ch->m_desc);
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		InitSSkillMsg(rmsg, MSG_EX_INIT_SSkill_ERROR_SUCCESS);
		SEND_Q(rmsg, ch->m_desc);
	}

	GAMELOG << init("INIT SSKILL", ch) << end;
}

void  do_NpcPortalScroll(CPC* ch, CNetMsg::SP& msg)
{
	if (ch == NULL || DEAD(ch))
		return ;
	int msgType = -1;
	int index = -1;
	static CLCString buf(MAX_MESSAGE_SIZE);
	RefMsg(msg) >> msgType;

	switch( msgType )
	{
	case MSG_NPC_PORTAL_GO:
	case MSG_NPC_PORTAL_LOCATION:
		RefMsg(msg) >> index;
		do_NpcportalScrollGoNPC(ch, index, msgType);
		break;
	}
	return;
}

void do_NpcportalScrollGoNPC(CPC* ch, int index, int msgType)
{
	CArea* area = NULL;

	if( ch && ch->m_pArea )
		area = ch->m_pArea;
	else
		return;

	int i, j;

	int sx = 0;
	int ex = area->m_size[0] - 1;
	int sz = 0;
	int ez = area->m_size[1] - 1;
	float retdist = GetDistance(0, 0, 0, area->m_size[0] * CELL_SIZE, area->m_size[1] * CELL_SIZE, 0);

	CCharacter* p;
	CNPC* npc;
	CNPC* ret = NULL;
	float dist;

	switch( msgType )
	{
	case MSG_NPC_PORTAL_GO:
		if( !ch->m_pArea->IsNpcPortalList_Npc( index ) )
		{
			CNetMsg::SP rmsg(new CNetMsg);
			NpcPortalListErrorMsg(rmsg, MSG_NPC_PORTAL_ERROR_MOB);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}
		break;
	case MSG_NPC_PORTAL_LOCATION:
		if( !ch->m_pArea->IsNpcPortalList_Mob( index ) )
		{
			CNetMsg::SP rmsg(new CNetMsg);
			NpcPortalListErrorMsg(rmsg, MSG_NPC_PORTAL_ERROR_MOB);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}
		break;
	}

	// 자신 주변에서
	for (i = sx; i <= ex; i++)
	{
		if (i < 0 || i >= area->m_size[0])
			continue ;

		for (j = sz; j <= ez; j++)
		{
			if (j < 0 || j >= area->m_size[1])
				continue ;

			// NPC를 찾기
			p = area->m_cell[i][j].m_listChar;

			while (p)
			{
				if (IS_NPC(p))
				{
					npc = TO_NPC(p);
					if ((index != -1 && npc->m_idNum == index))
					{
						// 거리를 비교해서 가까운 넘으로
						dist = GetDistance(ch, npc);
						if (dist < retdist)
						{
							retdist = dist;
							ret = npc;
						}
					}
				}
				p = p->m_pCellNext;
			}
		}
	}

	if (ret == NULL)
	{
		// 해당 몬스터가 필드에 없을때 보낸다.
		CNetMsg::SP rmsg(new CNetMsg);
		NpcPortalListErrorMsg(rmsg, MSG_NPC_PORTAL_ERROR_MOB);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	switch( msgType )
	{
	case MSG_NPC_PORTAL_GO:
		// 10초후 이동하게 하기
		ch->m_reqWarpType = IONCE_WARP_NPC_PORTAL_SCROLL;
		ch->m_reqWarpData = -1;
		ch->m_reqWarpTime = PULSE_WARPDELAY;
		ch->SetPlayerState(PLAYER_STATE_WARP);

		ch->m_Npc_Portal_x = GET_X(ret);
		ch->m_Npc_Portal_z = GET_Z(ret);
		ch->m_Npc_Portal_y = GET_YLAYER(ret);

		{
			// 사용했음을 알리기
			CNetMsg::SP rmsg(new CNetMsg);
			WarpStartMsg(rmsg, ch);
			ch->m_pArea->SendToCell(rmsg, ch, true);
		}
		//GoZone(ch, ch->m_pArea->m_zone->m_index, ch->m_pos.m_yLayer, GET_X(ret), GET_Z(ret)+1.0f);
		//GoTo(ch, GET_YLAYER(ret), GET_X(ret), GET_Z(ret)+1.0f, ch->m_pArea->GetHeight(GET_YLAYER(ret), GET_X(ret), GET_Z(ret)), GET_R(ch));

		{
			// GO_ZONE 효과 메시지 전송
			CNetMsg::SP rmsg(new CNetMsg);
			EffectEtcMsg(rmsg, ch, MSG_EFFECT_ETC_GOZONE);
			area->SendToCell(rmsg, ch, true);
		}
		break;

	case MSG_NPC_PORTAL_LOCATION:
		{
			CNetMsg::SP rmsg(new CNetMsg);
			rmsg->Init(MSG_EXTEND);
			RefMsg(rmsg) << MSG_EX_NPC_PORTAL_SCROLL;
			RefMsg(rmsg) << MSG_NPC_PORTAL_LOCATION;
			RefMsg(rmsg) << GET_X(ret);
			RefMsg(rmsg) << GET_Z(ret);
			SEND_Q(rmsg, ch->m_desc);
		}
		break;
	}
}

void do_ExRaidInfo(CPC* ch, CNetMsg::SP& msg)
{
	if (!ch || DEAD(ch))
		return;

	long subtype;
	msg->MoveFirst();
	RefMsg(msg) >> subtype;

	{
		// 귀속된 레이드 정보를 검색하러 헬퍼로 간다.
		CNetMsg::SP rmsg(new CNetMsg);
		HelperRaidInfoMsg(rmsg, ch->m_index);
		SEND_Q(rmsg, gserver->m_helper);
	}
}

// 퀘스트 아이템 다시 받는 기능
void do_ExTakeAgainQuestItem(CPC* ch, CNetMsg::SP& msg)
{
	if (!ch || DEAD(ch))
		return;

	long subtype;

	msg->MoveFirst();
	RefMsg(msg) >> subtype;

	// 인벤토리에 아이템이 존재하는 지 확인
	if ( ch->m_inventory.FindByDBIndex(4659, 0, 0) )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		TakeAgainQuestItemMsg(rmsg, MSG_EX_TAKE_AGAIN_QUEST_ITEM_ERROR_ALREADY_EXIST);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 인벤토리 가능 검사
	else if ( ch->m_inventory.getEmptyCount() < 1 )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		TakeAgainQuestItemMsg(rmsg, MSG_EX_TAKE_AGAIN_QUEST_ITEM_ERROR_FULL_INVENTORY);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 퀘스트가 완료가 안되어 있으면 아이템 지급 불가
	bool questComplete = ch->m_questList.FindQuest(248, QUEST_STATE_DONE);
	if(!questComplete)
		return;

	CItem* pItem = gserver->m_itemProtoList.CreateItem(4659, -1, 0, 0, 1);

	if(pItem == NULL)
		return;

	if (ch->m_inventory.addItem(pItem) == false)
	{
		delete pItem;
		return;
	}

	{
		// 성공했다는 메세지 보내기
		CNetMsg::SP rmsg(new CNetMsg);
		TakeAgainQuestItemMsg(rmsg, MSG_EX_TAKE_AGAIN_QUEST_ITEM_SUCCESS);
		SEND_Q(rmsg, ch->m_desc);
	}
}

void do_ExTitleSytem(CPC* ch, CNetMsg::SP& msg)
{
	if(!ch || DEAD(ch))
		return ;

	int msgtype;
	RefMsg(msg) >> msgtype;
	switch(msgtype)
	{
	case MSG_EX_TITLE_SYSTEM_LIST_REQ:
		do_ExTitleSystemListReq(ch);
		break;
	case MSG_EX_TITLE_SYSTEM_TITLE_SELECT_REQ:
		do_ExTitleSystemTitleSelectReq(ch, msg);
		break;
	case MSG_EX_TITLE_SYSTEM_TITLE_CANCEL_REQ:
		do_ExTitleSystemTitleCancelReq(ch, msg);
		break;
	case MSG_EX_TITLE_SYSTEM_TITLE_DELETE_REQ:
		do_ExTitleSystemTitleDeleteReq(ch, msg);
		break;
	default:
		break;
	}
}

void do_ExTitleSystemListReq(CPC* ch)
{
	if( !ch )
		return ;

	if (ch->m_titleList.m_nCount == 0)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		TitleSystemListRep(rmsg, MSG_EX_TITLE_SYSTEM_LIST_REP_NO_LIST, ch->m_titleList.m_nCount);
		SEND_Q(rmsg, ch->m_desc);
	}
	else
	{
		CNetMsg::SP rmsg(new CNetMsg);
		SubHelperTitleSystemCheckTimeReq(rmsg, ch->m_index, ch->m_titleList.m_head);
		SEND_Q(rmsg, gserver->m_subHelper);
	}
}

void do_ExTitleSystemTitleSelectReq(CPC* ch, CNetMsg::SP& msg)
{
	if(!ch)
		return ;
	int title_index;
	RefMsg(msg) >> title_index;

	if(!ch->m_titleList.HaveTitle(title_index))
	{
		CNetMsg::SP rmsg(new CNetMsg);
		TitleSystemMsg(rmsg, MSG_EX_TITLE_SYSTEM_NOT_HAVE_TITLE, title_index, ch->m_index);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
	else
	{
		ch->m_nCurrentTitle = title_index;
		ch->CalcStatus(true);

		{
			CNetMsg::SP rmsg(new CNetMsg);
			TitleSystemMsg(rmsg, MSG_EX_TITLE_SYSTEM_TITLE_SELECT_SUCCESS, title_index, ch->m_index);
			SEND_Q(rmsg, ch->m_desc);
			ch->m_pArea->SendToCell(rmsg, ch);
		}

		GAMELOG << init("TITLE SELECT", ch) << delim
				<< "TITLE INDEX" << delim << title_index << end;
	}
}

void do_ExTitleSystemTitleCancelReq(CPC* ch, CNetMsg::SP& msg)
{
	if(!ch)
		return ;
	int title_index;
	RefMsg(msg) >> title_index;

	ch->m_nCurrentTitle = TITLE_SYSTEM_NO_TITLE;
	ch->CalcStatus(true);

	{
		CNetMsg::SP rmsg(new CNetMsg);
		TitleSystemMsg(rmsg, MSG_EX_TITLE_SYSTEM_TITLE_CANCEL_SUCCESS, title_index, ch->m_index);
		SEND_Q(rmsg, ch->m_desc);
		ch->m_pArea->SendToCell(rmsg, ch);
	}

	GAMELOG << init("TITLE CANCEL", ch) << delim
			<< "TITLE INDEX" << delim << title_index << end;
}

void do_ExTitleSystemTitleDeleteReq(CPC* ch, CNetMsg::SP& msg)
{
	if(!ch)
		return ;
	int title_index;
	RefMsg(msg) >> title_index;

	if(!ch->m_titleList.HaveTitle(title_index))
	{
		CNetMsg::SP rmsg(new CNetMsg);
		TitleSystemMsg(rmsg, MSG_EX_TITLE_SYSTEM_NOT_HAVE_TITLE, title_index, ch->m_index);
		SEND_Q(rmsg, ch->m_desc);
	}
	else
	{
		if(gserver->isRunSubHelper())
		{
			GAMELOG << init("TITLE DELETE REQ", ch) << delim
					<< "TITLE INDEX" << delim << title_index << end;
			CNetMsg::SP rmsg(new CNetMsg);
			SubHelperTitleSystemDeleteReq(rmsg, ch->m_index, title_index);
			SEND_Q(rmsg, gserver->m_subHelper);
		}
	}
}

#ifdef LACARETTE_SYSTEM

void do_ExLacaretteSystem(CPC* ch, CNetMsg::SP& msg)
{
#ifdef LC_RUS
	return;
#endif // LC_RUS

	if( !ch )
		return;

	unsigned char sybType;
	RefMsg(msg) >> sybType;

	switch( sybType )
	{
	case MSG_EX_LACARETTE_TOKEN_READY_REQ:
		{
			CNetMsg::SP rmsg(new CNetMsg);
			ConnLacaretteTokenReadyMsg(rmsg, ch->m_desc->m_index, ch->m_index);
			SEND_Q(rmsg, gserver->m_connector);
			return;
		}
		break;
	case MSG_EX_LACARETTE_TOKEN_REQ:
		{
			CNetMsg::SP rmsg(new CNetMsg);
			ConnLacaretteTokenMsg(rmsg, ch->m_desc->m_index, ch->m_index);
			SEND_Q(rmsg, gserver->m_connector);
			return;
		}
		break;

	case MSG_EX_LACARETTE_RETTE_REQ:
		{
			int cosArrNum;
			int cosIndex;
			int tokenIndex;

			RefMsg(msg) >> cosArrNum
						>> cosIndex
						>> tokenIndex;

			CNetMsg::SP rmsg(new CNetMsg);
			ConnLacaretteRepeatMsg(rmsg, ch->m_desc->m_index, ch->m_index, cosArrNum, cosIndex, tokenIndex);
			SEND_Q(rmsg, gserver->m_connector);
			return;
		}
		break;

	case MSG_EX_LACARETTE_RESULT_REQ: // 아이템을 지급 해주장.
		{
			// 먼저 DB에 횟수 증가 시킨다.
			int giveitemindex;
			RefMsg(msg) >> giveitemindex;

			CNetMsg::SP rmsg(new CNetMsg);
			ConnLacaretteRepeatAddMsg(rmsg ,ch->m_desc->m_index, ch->m_index, giveitemindex );
			SEND_Q(rmsg, gserver->m_connector);
		}
		break;

	case MSG_EX_LACARETTE_UI_REQ:
		{
			CNetMsg::SP rmsg(new CNetMsg);
			ConnLacaretteUIMsg(rmsg, ch->m_desc->m_index, ch->m_index, 0, 0, 0);
			SEND_Q(rmsg, gserver->m_connector);
			return;
		}
		break;
	case MSG_EX_LACARETTE_UI_COLSE:
		ch->m_lacaretteCosArrNum	= -1;
		ch->m_lacaretteItemArrNum	= -1;
		ch->m_lacaretteTokenArrNum	= -1;
		ch->m_lacaretteRepeat		= -1;
		break;
	}
}
#endif

void do_FuncAttendanceExp ( CPC* ch, CNetMsg::SP& msg )
{
	unsigned char thirdtype;

	pTypeThirdBase* pThird = reinterpret_cast<pTypeThirdBase*>(msg->m_buf);
	thirdtype = pThird->thirdType;

	switch( thirdtype )
	{
	case MSG_SUB_ATTENDANCE_CHECK_REQ:
		{
			CNetMsg::SP rmsg(new CNetMsg);
			ResponseClient::makeAttendanceCheckMsg(rmsg, ch->m_attendanceManager.getAccCount(), ch->m_nAttendance_exp);
			SEND_Q(rmsg, ch->m_desc);
		}
		break;
	case MSG_SUB_ATTENDANCE_REWARD_REQ:
		{
			ch->m_attendanceManager.reward();
		}
		break;
	case MSG_SUB_ATTENDANCE_ASSURE_CANCEL_OK_REQ:
		{
			//종료 하겠다.
			ch->m_attendanceManager.assureOk();
		}
		break;
	}
}

#ifdef XTRAP
void do_FuncXtrap ( CPC* ch, CNetMsg::SP& msg )
{
	char subtype;

	RefMsg(msg) >> subtype;

	if( ch == NULL )
		return;

	switch( subtype )
	{
	case 0x002: //XTRAP_CMD_STEP_TWO
		{
			char msgbuffer[128];
			msg->Read( msgbuffer, 128 );
#ifdef XTRAP_DUMP
			char filename[128];
			sprintf( filename, "%d.xtrap.log" , ch->m_index );
			HexaDump4XTrap_Std( filename, msgbuffer, 128 , "Step3_Before" );
#endif // XTRAP_DUMP
			XTrap_CS_Step3( ch->m_xtrapSessionBuf, msgbuffer );
		}
		break;
	case 0x00f:	// XTRAP_MAP_FILE_CHANGE
		{
			FILE*	fp;
			fp = fopen("data/map1.cs3","rb");
			if( fp == NULL )
				return;

			fread( gserver->m_XTrapMap[0], XTRAP_CS4_BUFSIZE_MAP, 1 ,fp );
			fclose(fp);
		}
	}
}
#else
void do_FuncXtrap ( CPC* ch, CNetMsg::SP& msg )
{
	return;
}
#endif //XTRAP

void do_ExFuncTutorialRenewer(CPC* ch, CNetMsg::SP& msg)
{
	if (!ch)
		return;

	CItem* pItem = gserver->m_itemProtoList.CreateItem(6085, -1, 0, 0, 1);
	if(pItem == NULL)
		return;

	if (ch->m_inventory.addItem(pItem))
	{
		{
			CNetMsg::SP rmsg(new CNetMsg);
			ResponseClient::ItemUseMsg(rmsg, pItem->tab(), pItem->getInvenIndex(), pItem->getVIndex(), pItem->getDBIndex());
			do_Item(ch, rmsg);
		}

		{
			CNetMsg::SP rmsg(new CNetMsg);
			tutorialrenewerMsg(rmsg, pItem->m_itemProto->getItemNum0());
			SEND_Q(rmsg, ch->m_desc);
		}

		ch->m_newtutoComplete = 1;
	}
	else
	{
		delete pItem;
		return;
	}
}

void do_ExFaceOff(CPC* ch, CNetMsg::SP& msg)
{
	unsigned char subtype;
#ifdef NPC_CHECK
	int npcIndex;
	CNPC *npc;
	RefMsg(msg) >> subtype
				>> npcIndex;
	npc = ch->m_pArea->FindProtoNPCInCell(ch, npcIndex, false, 2);
	if(!npc)
	{
		GAMELOG << init("STASH_KEEP FAIL: NOT EXIST NPC (Dangerous User)", ch) << end;
		return ;
	}
	if( !npc->IsFlag(NPC_RESETSTAT) )
	{
		GAMELOG << init("STASH_KEEP FAIL: NOT EXIST NPC (Dangerous User)", ch) << end;
		return ;
	}
#else
	RefMsg(msg) >> subtype;
#endif

	switch( subtype )
	{
	case MSG_EX_FACEOFF_UIOPEN_REQ:
		{
			if (ch->m_inventory.getEmptyCount() <= 0)
			{
				CNetMsg::SP rmsg(new CNetMsg);
				ResponseClient::ItemTakeoffMsg(rmsg, ResponseClient::WEAR_ERR_NOT_ENOUGH_INVEN);
				SEND_Q(rmsg, ch->m_desc);
				return;
			}

			CItem * pItem = ch->m_wearInventory.wearItemInfo[WEARING_HELMET];
			if( pItem )
			{
				if (pItem->getWearPos() != WEARING_NONE)
				{
					CNetMsg::SP rmsg(new CNetMsg);
					insideServer_do_Item_Wear_TakeOff(rmsg, pItem->getWearPos());
					do_Item(ch, rmsg);
				}
			}
			pItem = ch->m_wearInventory.wearItemInfo[COSTUME2_WEARING_HELMET];
			if( pItem )
			{
				if (pItem->m_itemProto->getItemFlag() & ITEM_FLAG_COSTUME2 && pItem->getWearPos() != WEARING_NONE)
				{
					CNetMsg::SP rmsg(new CNetMsg);
					insideServer_do_Item_Costume(rmsg, pItem->getWearPos());
					do_Item(ch, rmsg);
				}
			}
		}
		break;
	case MSG_EX_FACEOFF_REQ:
		{
			unsigned char hairStyle, faceStyle;
			RefMsg(msg) >> hairStyle >> faceStyle;
			if(ch->m_hairstyle != hairStyle || ch->m_facestyle != faceStyle)
			{
				bool success_flag = false;
				CItem * pItem = NULL;

#ifdef PREMIUM_CHAR
				//XX - 프리미엄캐릭터 : 외형 변경권 아이템 없어도 외형 변경 가능
				if (ch->m_premiumChar.isActive())
				{
					success_flag = true;
					pItem = NULL;
				}
				else
#endif
				{
					pItem = NULL;

					static const int needItem[2] = {7055, 5969}; // 성형 카드
					for(int i = 0; i < 2; i++)
					{
						pItem = ch->m_inventory.FindByDBIndex(needItem[i], 0, 0);
						if (pItem)
						{
							success_flag = true;
							break;
						}
					}
				}

				if (success_flag)
				{
					if (pItem)
						ch->m_inventory.decreaseItemCount(pItem, 1);

					ch->m_hairstyle = hairStyle;
					ch->m_facestyle = faceStyle;

					{
						CNetMsg::SP rmsg(new CNetMsg);
						rmsg->Init(MSG_HELPER_COMMAND);
						RefMsg(rmsg) << MSG_HELPER_FACEOFF_REQ
									 << ch->m_index
									 << ch->m_hairstyle
									 << ch->m_facestyle;
						SEND_Q(rmsg, gserver->m_helper);
					}

					GAMELOG << init("FACE OFF", ch)
							<< "hairStyle : " << hairStyle << delim
							<< "faceStyle : " << faceStyle << end;

					{
						CNetMsg::SP rmsg(new CNetMsg);
						rmsg->Init(MSG_EXTEND);
						RefMsg(rmsg) << MSG_EX_FACEOFF
									 << (unsigned char) MSG_EX_FACEOFF_REP
									 << (unsigned char) MSG_EX_FACEOFF_ERROR_SUC
									 << (unsigned char)ch->m_hairstyle
									 << (unsigned char)ch->m_facestyle;
						RefMsg(rmsg) << ch->m_index;
						ch->m_pArea->SendToCell(rmsg, ch, true );
					}
					return;
				}

				{
					CNetMsg::SP rmsg(new CNetMsg);
					rmsg->Init(MSG_EXTEND);
					RefMsg(rmsg) << MSG_EX_FACEOFF
								 << (unsigned char) MSG_EX_FACEOFF_REP
								 << (unsigned char) MSG_EX_FACEOFF_ERROR_FAIL;
					SEND_Q(rmsg, ch->m_desc);
				}
			}
			else
			{
				CNetMsg::SP rmsg(new CNetMsg);
				rmsg->Init(MSG_EXTEND);
				RefMsg(rmsg) << MSG_EX_FACEOFF
							 << (unsigned char) MSG_EX_FACEOFF_REP
							 << (unsigned char) MSG_EX_FACEOFF_ERROR_PLZ_CHOISE;
				SEND_Q(rmsg, ch->m_desc);
			}
		}
		break;
	}
}

void do_ExLCBall( CPC* ch, CNetMsg::SP& msg )
{
	char type;
	int		coinIndex = 0;
	char	courseIndex = 0;

	RefMsg(msg) >> type ;
	RefMsg(msg) >> coinIndex >> courseIndex;

	switch( type )
	{
	case MSG_EX_LCBALL_INFO:
		{
			CNetMsg::SP rmsg(new CNetMsg);
			SubHelperLCBallinfoMsg( rmsg, ch->m_index, coinIndex, courseIndex );
			SEND_Q( rmsg, gserver->m_subHelper );
		}
		break;

	case MSG_EX_LCBALL_USE:
		{
			// 인벤검사
			if( ch->m_inventory.getEmptyCount() < 1 ) // 빈공간 없음
			{
				CNetMsg::SP rmsg(new CNetMsg);
				LCBallUseMsg( rmsg, 1, 0 );
				SEND_Q(rmsg, ch->m_desc );
				return;
			}
			// 코인(토큰)이 있는지
			if (ch->m_inventory.FindByDBIndex(coinIndex) == NULL)
			{
				CNetMsg::SP rmsg(new CNetMsg);
				LCBallUseMsg( rmsg, 2 , coinIndex );	// 코인이 없습니다.
				SEND_Q(rmsg, ch->m_desc );
				return ;
			}

			{
				CNetMsg::SP rmsg(new CNetMsg);
				SubHelperLCBalluseMsg( rmsg, ch->m_index, coinIndex, courseIndex );
				SEND_Q( rmsg, gserver->m_subHelper );
			}
		}
		break;
	}
	return;
}

#ifdef JUNO_RENEWAL_MESSAGEBOX
void do_ExMsgBox( CPC* ch, CNetMsg::SP& msg )
{
	unsigned char subtype;
	RefMsg(msg) >> subtype;
	switch(subtype)
	{
	case MSG_EX_MSGBOX_CHANGE_START_POINT:
		{
			char selected;
			RefMsg(msg) >> selected;
			ch->m_etcEvent &= ~ETC_EVENT_JUNO_RENEWAL_MESSAGEBOX_POPUP;

			if(selected)		// 확인
			{
				if(ch->m_etcEvent & ETC_EVENT_JUNO_RENEWAL_QUESTCOMPLETE)
				{
					// 농장으로 이동
					CZone* pZone = gserver->FindZone(ZONE_START);
					if (pZone == NULL)
						return;

					GoZone(ch, pZone->m_index,
						   pZone->m_zonePos[6][0],														// ylayer
						   GetRandom(pZone->m_zonePos[6][1], pZone->m_zonePos[6][3]) / 2.0f,		// x
						   GetRandom(pZone->m_zonePos[6][2], pZone->m_zonePos[6][4]) / 2.0f);		// z
				}
			}
			else
				ch->m_etcEvent &= ~ETC_EVENT_JUNO_RENEWAL_QUESTCOMPLETE;

			break;
		}
	default:
		break;
	}
}
#endif // JUNO_RENEWAL_MESSAGEBOX

void do_ExRoyalRumble(CPC* ch, CNetMsg::SP& msg)
{
	// MSG_EXTEND
	// MSG_EX_ROYAL_RUMBLE
	// (uc)MSG_EX_ROYAL_RUMBLE_PLAYER_REQ
	unsigned char subtype;
	RefMsg(msg) >> subtype;
	switch(subtype)
	{
	case MSG_EX_ROYAL_RUMBLE_PLAYER_REQ:
		{
			// 선수등록
			do_ExRoyalRumblePlayerReq(ch, msg);
		}
		break;
	case MSG_EX_ROYAL_RUMBLE_PLAYER_UNREGIST_REQ:
		{
			do_ExRoyalRumblePlayerUnRegReq(ch, msg);
		}
		break;
	case MSG_EX_ROYAL_RUMBLE_REWARD_REQ:
		{
			do_ExRoyalRumbleRewardReq(ch, msg);
		}
		break;
	case MSG_EX_ROYAL_RUMBLE_POINT_REQ:
		{
			do_ExRoyalRumblePointReq(ch, msg);
		}
		break;
	case MSG_EX_ROYAL_RUMBLE_TIME_REQ:
		{
			do_ExRoyalRumbleTimeReq(ch, msg);
		}
		break;
	case MSG_EX_ROYAL_RUMBLE_GO_ZONE:
		{
			do_ExRoyalRumbleGoZone(ch, msg);
		}
		break;
	}
}
void do_ExRoyalRumblePlayerReq(CPC* ch, CNetMsg::SP& msg)
{
	if(!ch)
		return ;
	int leveltype = 0;	// 0:rookie, 1:senior, 2:master, 3:other
	LONGLONG neednas = 0;
	int finditem = -1;
	bool bInspection = true;
	if(ch->m_level >= MIN_ROOKIE_LEVEL && ch->m_level <= MAX_ROOKIE_LEVEL)
	{
		leveltype = LEVEL_TYPE_ROOKIE;
		neednas = ROOKIE_REGIST_NEED_PRICE;		// 백만
		finditem = ROOKIE_REGIST_ITEM;
	}
	else if(ch->m_level >= MIN_SENIOR_LEVEL && ch->m_level <= MAX_SENIOR_LEVEL)
	{
		leveltype = LEVEL_TYPE_SENIOR;
		neednas = SENIOR_REGIST_NEED_PRICE;		// 천만
		finditem = SENIOR_REGIST_ITEM;
	}
	else if(ch->m_level >= MIN_MASTER_LEVEL)
	{
		leveltype = LEVEL_TYPE_MASTER;
		neednas = MASTER_REGIST_NEED_PRICE;	// 일억
		finditem = MASTER_REGIST_ITEM;
	}
	else
		leveltype = -1;

	if(leveltype == -1)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		RoyalRumbleRegist(rmsg, 2);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 아이템을 찾아서 있으면 돈검사와 인벤 공간 검사를 안한다.
	if(ch->m_inventory.FindByDBIndex(finditem, 0, 0))
	{
		bInspection = false;
	}

	if(bInspection && (ch->m_inventory.getMoney() < neednas))
	{
		// 돈이 없습니다.
		CNetMsg::SP rmsg(new CNetMsg);
		RoyalRumbleRegist(rmsg, 1);
		SEND_Q(rmsg, ch->m_desc);
		GAMELOG << init("ROYAL RUMBLE REGIST", ch) << "NO MONEY" << end;
		return ;
	}
	if(bInspection && ch->m_inventory.getEmptyCount() < 1) // 인벤토리가 가득 차 있으면...
	{
		CNetMsg::SP rmsg(new CNetMsg);
		SysFullInventoryMsg(rmsg, 0);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	bool b = false;
	//b = gserver->m_RoyalRumble.GetRookiePlaying();
	//b = gserver->m_RoyalRumble.GetSeniorPlaying();
	//b = gserver->m_RoyalRumble.GetMasterPlaying();

	if( gserver->m_RoyalRumble.GetRookiePlaying() ||
			gserver->m_RoyalRumble.GetSeniorPlaying() ||
			gserver->m_RoyalRumble.GetMasterPlaying() )
	{
		b = true;
	}

	if(b)
	{
		// 현재 leveltype에서 경기중이니까 신청이 안됨.
		CNetMsg::SP rmsg(new CNetMsg);
		RoyalRumbleRegist(rmsg, 2);
		SEND_Q(rmsg, ch->m_desc);
		GAMELOG << init("ROYAL RUMBLE REGIST", ch) << "GAME IS PLAYING CANNOT REGIST" << end;
		return ;
	}

	// 이미신청 하셨습니다.
	if(gserver->m_RoyalRumble.m_WGPlayerList.FindNode(ch->m_index))
	{
		CNetMsg::SP rmsg(new CNetMsg);
		RoyalRumbleRegist(rmsg, 3);
		SEND_Q(rmsg, ch->m_desc);
		GAMELOG << init("ROYAL RUMBLE REGIST", ch) << "ALREADY REGIST" << end;
		return ;
	}

	CWGPlayer* pWGp = NULL;

	pWGp = new CWGPlayer(ZONE_ROYAL_RUMBLE, ch->m_index, leveltype);

	if(!pWGp)
		return ;

	gserver->m_RoyalRumble.m_WGPlayerList.AddNode(pWGp);

	if(bInspection)
	{
		// 아이템을 넣어준다.
		CItem* pItem = gserver->m_itemProtoList.CreateItem(finditem, -1, 0, 0, 1);
		if(pItem == NULL)
			return ;

		if (ch->m_inventory.addItem(pItem) == false)
		{
			delete pItem;

			// 인벤이 가득차서 아이템 습득 불가.
			CNetMsg::SP rmsg(new CNetMsg);
			SysFullInventoryMsg(rmsg, 0);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}

		{
			// 신청비 받는 구간 시작
			if(ch->m_inventory.getMoney() >= neednas)
			{
				ch->m_inventory.decreaseMoney(neednas);
			}
			else
			{
				CNetMsg::SP rmsg(new CNetMsg);
				SysMsg(rmsg, MSG_SYS_NOT_ENOUGH_MONEY);
				SEND_Q(rmsg, ch->m_desc);
			}
			// 신청비 받는 구간 끝
		}
	}

	{
		// 등록이 완료되었습니다.
		CNetMsg::SP rmsg(new CNetMsg);
		RoyalRumbleRegist(rmsg, 0);
		GAMELOG << init("ROYAL RUMBLE REGIST", ch) << "COMPLETE REGIST" << end;
		SEND_Q(rmsg, ch->m_desc);
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		RoyalRumbleRegistMenu(rmsg, 0);
		SEND_Q(rmsg, ch->m_desc);
	}
}
void do_ExRoyalRumblePlayerUnRegReq(CPC* ch, CNetMsg::SP& msg)
{
	if(!ch)
		return ;
#ifdef NPC_CHECK
	int npcIndex;
	CNPC *npc;
	RefMsg(msg) >> npcIndex;
	npc = ch->m_pArea->FindProtoNPCInCell(ch, npcIndex, false, 2);
	if(!npc)
	{
		GAMELOG << init("RoyalRumblePlayerunReg FAIL: NOT EXIST NPC (Dangerous User)", ch) << end;
		return ;
	}
	if( !npc->IsFlag(NPC_QUEST) )
	{
		GAMELOG << init("RoyalRumblePlayerunReg FAIL: NOT EXIST NPC (Dangerous User)", ch) << end;
		return ;
	}
#endif
	// 취소 시간인가?
	if(!gserver->m_RoyalRumble.GetRoyalRumbleNotice())
	{
		//취소할 수 있는 시간이 아닙니다.
		CNetMsg::SP rmsg(new CNetMsg);
		RoyalRumbleUnRegist(rmsg, 1);
		SEND_Q(rmsg, ch->m_desc);
		GAMELOG << init("ROYAL RUMBLE UNREGIST", ch) << "CANNOT THIS TIME UNREGIST" << end;
		return ;
	}
	// 등록자 명단에 있나?
	if(!gserver->m_RoyalRumble.m_WGPlayerList.FindNode(ch->m_index))
	{
		//등록자 명단에 없습니다.
		CNetMsg::SP rmsg(new CNetMsg);
		RoyalRumbleUnRegist(rmsg, 2);
		SEND_Q(rmsg, ch->m_desc);
		GAMELOG << init("ROYAL RUMBLE UNREGIST", ch) << "NOT REGIST" << end;
		return ;
	}

	gserver->m_RoyalRumble.m_WGPlayerList.DelNode(ch->m_index);

	{
		// 취소가 완료되었습니다.
		CNetMsg::SP rmsg(new CNetMsg);
		RoyalRumbleUnRegist(rmsg, 0);
		GAMELOG << init("ROYAL RUMBLE UNREGIST", ch) << "COMPLETE UNREGIST" << end;
		SEND_Q(rmsg, ch->m_desc);
	}

	if(gserver->m_RoyalRumble.GetRoyalRumbleNotice())
	{
		if(gserver->m_subno == WAR_GROUND_CHANNEL)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			RoyalRumbleRegistMenu(rmsg, 1);
			SEND_Q(rmsg, ch->m_desc);
		}
	}
}
void do_ExRoyalRumbleRewardReq(CPC* ch, CNetMsg::SP& msg)
{
	// 보상요청
	if(!ch)
		return ;
#ifdef NPC_CHECK
	int npcIndex;
	CNPC *npc;
	RefMsg(msg) >> npcIndex;
	npc = ch->m_pArea->FindProtoNPCInCell(ch, npcIndex, false, 2);
	if(!npc)
	{
		GAMELOG << init("RoyalRumbleReward FAIL: NOT EXIST NPC (Dangerous User)", ch) << end;
		return ;
	}
	if( !npc->IsFlag(NPC_QUEST) )
	{
		GAMELOG << init("RoyalRumbleReward FAIL: NOT EXIST NPC (Dangerous User)", ch) << end;
		return ;
	}
#endif
	int nReward = 0;
	int LevelType = -1;
	if(gserver->m_RoyalRumble.GetWinner(LEVEL_TYPE_ROOKIE) == ch->m_index)
	{
		if(gserver->m_RoyalRumble.GetRewarded(LEVEL_TYPE_ROOKIE))
		{
			// 이미 받았습니다.
			CNetMsg::SP rmsg(new CNetMsg);
			RoyalRumbleReward(rmsg, 1);
			SEND_Q(rmsg, ch->m_desc);
			GAMELOG << init("ROYAL RUMBLE REWARD", ch) << "ALREADY REWARD" << end;
			return ;
		}
		// TODO: 수정필요
		nReward = 5974;
		LevelType = LEVEL_TYPE_ROOKIE;
	}
	else if(gserver->m_RoyalRumble.GetWinner(LEVEL_TYPE_SENIOR) == ch->m_index)
	{
		if(gserver->m_RoyalRumble.GetRewarded(LEVEL_TYPE_SENIOR))
		{
			// 이미 받았습니다.
			CNetMsg::SP rmsg(new CNetMsg);
			RoyalRumbleReward(rmsg, 1);
			SEND_Q(rmsg, ch->m_desc);
			GAMELOG << init("ROYAL RUMBLE REWARD", ch) << "ALREADY REWARD" << end;
			return ;
		}
		// TODO: 수정필요
		nReward = 6997;
		LevelType = LEVEL_TYPE_SENIOR;
	}
	else if(gserver->m_RoyalRumble.GetWinner(LEVEL_TYPE_MASTER) == ch->m_index)
	{
		if(gserver->m_RoyalRumble.GetRewarded(LEVEL_TYPE_MASTER))
		{
			// 이미 받았습니다.
			CNetMsg::SP rmsg(new CNetMsg);
			RoyalRumbleReward(rmsg, 1);
			SEND_Q(rmsg, ch->m_desc);
			GAMELOG << init("ROYAL RUMBLE REWARD", ch) << "ALREADY REWARD" << end;
			return ;
		}
		// TODO: 수정필요
		nReward = 6998;
		LevelType = LEVEL_TYPE_MASTER;
	}
	else
	{
		// 우승자 아님
		CNetMsg::SP rmsg(new CNetMsg);
		RoyalRumbleReward(rmsg, 0);
		SEND_Q(rmsg, ch->m_desc);
		GAMELOG << init("ROYAL RUMBLE REWARD", ch) << "NOT WINNER" << end;
		return ;
	}

	// 아이템 지급
	if(ch->m_inventory.getEmptyCount() < 1)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		SysFullInventoryMsg(rmsg, 0);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
	CItem* pItem = gserver->m_itemProtoList.CreateItem(nReward, -1, 0, 0, 1);
	if(!pItem)
		return;

	if (ch->m_inventory.addItem(pItem))
	{
		gserver->m_RoyalRumble.SetRewarded(LevelType, ch);
	}
	else
	{
		delete pItem;
	}
}
void do_ExRoyalRumblePointReq(CPC* ch, CNetMsg::SP& msg)
{
#ifdef NPC_CHECK
	int npcIndex;
	CNPC *npc;
	RefMsg(msg) >> npcIndex;
	npc = ch->m_pArea->FindProtoNPCInCell(ch, npcIndex, false, 2);
	if(!npc)
	{
		GAMELOG << init("RoyalRumble PointReq FAIL: NOT EXIST NPC (Dangerous User)", ch) << end;
		return ;
	}
	if( !npc->IsFlag(NPC_QUEST) )
	{
		GAMELOG << init("RoyalRumble PointReq FAIL: NOT EXIST NPC (Dangerous User)", ch) << end;
		return ;
	}
#endif

	{
		CNetMsg::SP rmsg(new CNetMsg);
		RoyalRumbleWarPoint(rmsg, ch->GetWarGroundPoint(), ch->GetWarGroundAccPoint());
		SEND_Q(rmsg, ch->m_desc);
	}
}

void do_ExRoyalRumbleTimeReq(CPC* ch, CNetMsg::SP& msg)
{
	bool b = false;
	//b = gserver->m_RoyalRumble.GetRookiePlaying();
	//b = gserver->m_RoyalRumble.GetSeniorPlaying();
	//b = gserver->m_RoyalRumble.GetMasterPlaying();

#ifdef NPC_CHECK
	int npcIndex;
	CNPC *npc;
	RefMsg(msg) >> npcIndex;
	npc = ch->m_pArea->FindProtoNPCInCell(ch, npcIndex, false, 2);
	if(!npc)
	{
		GAMELOG << init("RoyalRumble TimeReq FAIL: NOT EXIST NPC (Dangerous User)", ch) << end;
		return ;
	}
	if( !npc->IsFlag(NPC_QUEST) )
	{
		GAMELOG << init("RoyalRumble TimeReq FAIL: NOT EXIST NPC (Dangerous User)", ch) << end;
		return ;
	}
#endif

	if( gserver->m_RoyalRumble.GetRookiePlaying() ||
			gserver->m_RoyalRumble.GetSeniorPlaying() ||
			gserver->m_RoyalRumble.GetMasterPlaying() )
	{
		b = true;
	}
	if(gserver->m_RoyalRumble.GetRoyalRumbleTime() != 0)
	{
		b = true;
	}
	if(b)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		RoyalRumbleTimeReq(rmsg, -1);
		SEND_Q(rmsg, ch->m_desc);
	}
	else
	{
		int h = gserver->m_tRealSystemTime.tm_hour;
		int i;
		int nexttime = -1;
		for(i = 0; i < ROYAL_RUMBLE_SCHEDULE; i++)
		{
			if(gserver->m_RoyalRumble.m_RoyalRumbleTime[i] > h)
			{
				nexttime = gserver->m_RoyalRumble.m_RoyalRumbleTime[i];
				break;
			}
		}
		if(nexttime == -1)
			nexttime = gserver->m_RoyalRumble.m_RoyalRumbleTime[0];
		// m_RoyalRumbleTime 에서 시간 관리를 하다가 22시로 고정되어 시작시간은 상수처리
		// 그래서 다음 시간은 무조건 22시로 보냄. 추후 변경이 필요할 때 수정이 필요함.
		nexttime = gserver->m_RoyalRumble.getStartHour();

		{
			CNetMsg::SP rmsg(new CNetMsg);
			RoyalRumbleTimeReq(rmsg, nexttime);
			SEND_Q(rmsg, ch->m_desc);
		}
	}
}

void do_ExRoyalRumbleGoZone(CPC* ch, CNetMsg::SP& msg)
{
	int finditem;
	int r, c;
	// 클라이언트에서 고존을 보내왔다.
	// 그러면 서버는 아이템 체크 하고..아이템을 없애고..고존을 해준다......
	CWaitPlayer* wp = gserver->m_RoyalRumble.m_WaitPlayerList.GetNode(ch->m_index);
	if(wp == NULL)
	{
		// 신청자 목록에 없다.
		CNetMsg::SP rmsg(new CNetMsg);
		RoyalRumbleNotfoundItem(rmsg);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	switch(wp->GetLevelType())
	{
	case LEVEL_TYPE_ROOKIE:
		{
			finditem = ROOKIE_REGIST_ITEM;
		}
		break;

	case LEVEL_TYPE_SENIOR:
		{
			finditem = SENIOR_REGIST_ITEM;
		}
		break;

	case LEVEL_TYPE_MASTER:
		{
			finditem = MASTER_REGIST_ITEM;
		}
		break;

	default:
		return;
		break;
	}
	CItem* pItem = ch->m_inventory.FindByDBIndex(finditem, 0, 0);
	if (pItem)
	{
		// 아이템을 찾았으니 뺐자.
		ch->m_inventory.decreaseItemCount(pItem, 1);

		// 삭제를 해주었으니 고존을 시켜준다.
		CNetMsg::SP rmsg(new CNetMsg);
		RequestClient::makeGoZone(rmsg, ZONE_ROYAL_RUMBLE, 0, -1);
		do_GoZone(ch, rmsg);
	}
	else
	{
		// 아이템이 없으니 이동을 못한다.
		CNetMsg::SP rmsg(new CNetMsg);
		RoyalRumbleNotfoundItem(rmsg);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
}

#ifdef GAMIGO_CASH_AUTHENTICATION_CODE_USE
void do_ExCashAuthenticationCode(CPC* ch, CNetMsg::SP& msg)
{
	CLCString sql(1024);
	CLCString md5AuthCode;
	sql.Format("select MD5( CONCAT( UPPER('%s'),'%s') ) as md5AuthCode" , (const char*)ch->m_desc->m_idname, LOCAL_AUTHENTICATION_SECRETCODE );
	CDBCmd cmd;
	cmd.Init(&gserver->m_dbdata);
	cmd.SetQuery(sql);

	unsigned char bAuthCode = 1;

	if( !cmd.Open() || !cmd.MoveFirst() )
	{
		bAuthCode = 0;
		md5AuthCode ="FAIL";
	}
	else
	{
		cmd.GetRec("md5AuthCode", md5AuthCode);
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		rmsg->Init(MSG_EXTEND);
		RefMsg(rmsg) << MSG_EX_CASH_AUTHENTICATION_CODE
					 << bAuthCode;
		if( bAuthCode == 1 )
		{
			RefMsg(rmsg) << ch->m_desc->m_idname
						 << (unsigned char) LCOAL_GAMIGO_GROUP_ID
						 << md5AuthCode;
		}
		SEND_Q(rmsg, ch->m_desc);
	}
}
#endif

#ifdef SYSTEM_TREASURE_MAP
#ifdef SYSTEM_TREASURE_MAP_GAMIGO
void do_ExTreasureMapKeyExchange(CPC* ch, CNetMsg::SP& msg)
{
	if( !ch )
		return;

	// 나무상자 열쇠가 100개 있는지 체크하자

	// 인벤 자리를 확인하자.
	if( ch->m_inventory.getEmptyCount() < 1 )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		SysMsg(rmsg, MSG_SYS_FULL_INVENTORY);
		SEND_Q(rmsg, ch->m_desc );
		return;
	}

	item_search_t vec;
	int sc = ch->m_inventory.searchItemByCondition(TREASURE_KEY_ITEM_INDEX, 0, 0, vec);
	if (sc == 0)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		MsgTreaserMapKeyExchangeError(rmsg, MSG_EX_TRS_MAP_KEY_EXCHANGE_ERROR_FAIL);
		SEND_Q(rmsg, ch->m_desc );
		return;
	}

	// 개수확인
	int deleteCnt = 1000;
	if (sc < deleteCnt)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		MsgTreaserMapKeyExchangeError(rmsg, MSG_EX_TRS_MAP_KEY_EXCHANGE_ERROR_COUNT);
		SEND_Q(rmsg, ch->m_desc );
		return;
	}

	// 지급할 아이템을 생성하자.
	CItem * pAddItem = gserver->m_itemProtoList.CreateItem(TREASURE_KEY_ITEM_INDEX+1, -1, 0, 0, 1);
	if (pAddItem == NULL)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		MsgTreaserMapKeyExchangeError(rmsg, MSG_EX_TRS_MAP_KEY_EXCHANGE_ERROR_FAIL);
		SEND_Q(rmsg, ch->m_desc );
		return;
	}

	// 아이템을 넣어주자.
	if (ch->m_inventory.addItem(pAddItem))
	{
		ch->m_inventory.deleteItem(vec, deleteCnt);
	}
	else
	{
		delete pAddItem;

		CNetMsg::SP rmsg(new CNetMsg);
		MsgTreaserMapKeyExchangeError(rmsg, MSG_EX_TRS_MAP_KEY_EXCHANGE_ERROR_FAIL);
		SEND_Q(rmsg, ch->m_desc );

		return;
	}

	{
		// 성공 했다 성공 메시지를 보내주자.
		CNetMsg::SP rmsg(new CNetMsg);
		MsgTreaserMapKeyExchangeError(rmsg, MSG_EX_TRS_MAP_KEY_EXCHANGE_ERROR_SUC);
		SEND_Q(rmsg, ch->m_desc );
	}
}
#endif
#endif

void do_rankingSystem(CPC* ch, CNetMsg::SP& msg)
{
	if ( !ch )
		return;

	unsigned char msgType, type, subtype;
	CLCString name(20);

	RefMsg(msg) >> msgType >> type >> subtype;

	switch ( msgType )
	{
	case MSG_EX_RANKING_LIST:
		{
			CNetMsg::SP rmsg(new CNetMsg);
			SubHelperRankingListReqEx(rmsg, type, subtype, ch->m_index);
			SEND_Q(rmsg, gserver->m_subHelper);
		}
		break;
	case MSG_EX_RANKING_SEARCH:
		{
			RefMsg(msg) >> name;

			if(findPercentChar(name.getBuffer()) != NULL)
				return ;

			CNetMsg::SP rmsg(new CNetMsg);
			SubHelperRankingSearchReqEx(rmsg, type, subtype, ch->m_index, name);
			SEND_Q(rmsg, gserver->m_subHelper);
		}
		break;
	}
}

#ifdef REFORM_PK_PENALTY_201108 // 2011-08 PK 패널티 리폼 :: 클라가 지급 요청 했다.
void do_ExPKPeneltyReform(CPC* ch, CNetMsg::SP& msg)
{
	unsigned char subType;
	int rewardNum;

	RefMsg(msg) >> subType
				>> rewardNum;

	if( subType != MSG_EX_PKPENALTY_REFORM_REWARD_REQ )
		return;

	// 클라가 요청한 보상이 정당한지 검사.
	if( ch->GetPKPenaltyRewardNum() != rewardNum )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		MsgPKPenaltyRewardRep(rmsg, rewardNum, MSG_EX_PKPENALTY_REFORM_REWARD_ERROR_FAIL );
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 타이틀 개수 제한 확인
	if(ch->m_titleList.m_nCount >= MAX_TITLE_COUNT)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		MsgPKPenaltyRewardRep(rmsg, rewardNum, MSG_EX_PKPENALTY_REFORM_REWARD_ERROR_COUNT);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 보상 타이틀 인덱스 구하자
	int titleIndex = -1;
	titleIndex = 85 + rewardNum;

	// 이미 보상을 받았나 확인
	if( ch->CheckPKPenaltyReward(rewardNum) || ch->m_titleList.Find(titleIndex) != NULL )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		MsgPKPenaltyRewardRep(rmsg, rewardNum, MSG_EX_PKPENALTY_REFORM_REWARD_ERROR_ONCE);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 타이틀dl 지급 가능한지 확인
	CTitleProto * titleProto = gserver->m_titleProtoList.FindProto(titleIndex);
	if( !titleProto )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		MsgPKPenaltyRewardRep(rmsg, rewardNum, MSG_EX_PKPENALTY_REFORM_REWARD_ERROR_FAIL );
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// DB 기록을 위해서...
	if( gserver->isRunSubHelper() )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		MsgSubHelperPKPenaltyRewardReq(rmsg, ch->m_index, rewardNum, titleProto->m_index, titleProto->m_time );
		SEND_Q(rmsg, gserver->m_subHelper);
		return;
	}
	else
	{
		// subhelper가 구동 않되었으니 에러를 보낸다.
		CNetMsg::SP rmsg(new CNetMsg);
		MsgPKPenaltyRewardRep(rmsg, rewardNum, MSG_EX_PKPENALTY_REFORM_REWARD_ERROR_FAIL );
		SEND_Q(rmsg, ch->m_desc);
		return;
	}
}

#endif // REFORM_PK_PENALTY_201108 // 2011-08 PK 패널티 리폼

#ifdef NOTICE_SYSTEM
void do_UserNotice(CPC* ch, CNetMsg::SP& msg)
{
	int _type;
	RefMsg(msg) >> _type;

	switch (_type)
	{
	case MSG_EX_USER_NOTICE_REG_REQ:
		{
			CLCString _contents(81);
			RefMsg(msg) >> _contents;

			if ( _contents.Length() <= 0 )
			{
				CNetMsg::SP rmsg(new CNetMsg);
				UserNoticeErrorMsg(rmsg, MSG_EX_USER_NOTICE_ERROR_LENGTH);
				SEND_Q(rmsg, ch->m_desc);
				return;
			}

			// TODO :: 아이템 가지고 있는지 검색
			const int _noticeItem = 7736;
			CItem* _item = ch->m_inventory.FindByDBIndex(_noticeItem, 0, 0);
			if (_item)
			{
				ch->m_inventory.decreaseItemCount(_item, 1);
			}
			else
			{
				CNetMsg::SP rmsg(new CNetMsg);
				UserNoticeErrorMsg(rmsg, MSG_EX_USER_NOTICE_ERROR_NOITEM);
				SEND_Q(rmsg, ch->m_desc);
				return;
			}

			if ( gserver->isRunSubHelper() )
			{
				CNetMsg::SP rmsg(new CNetMsg);
				SubHelperUserNoticeRegMsg(rmsg, ch->m_index, ch->GetName(), _contents);
				SEND_Q(rmsg, gserver->m_subHelper);

				GAMELOG << init("[USER NOTICE] REGIST REQ") << delim
					<< ch->GetName() << delim
					<< _contents << end;
			}
		}
		break;
	}
}
#endif // NOTICE_SYSTEM


void do_serverTime(CPC *ch, CNetMsg::SP& msg)
{
	if (ch == NULL)
		return ;

	CNetMsg::SP rmsg(new CNetMsg);
	rmsg->Init(MSG_EXTEND);
	RefMsg(rmsg) << MSG_EX_SERVER_TIME
				 << (int)gserver->m_nowseconds;
	SEND_Q(rmsg, ch->m_desc);
}

void do_setTarget(CPC* ch, CNetMsg::SP& msg)
{
	if(ch == NULL)
		return;

	RequestClient::doSetTargetInfo *packet = reinterpret_cast<RequestClient::doSetTargetInfo*>(msg->m_buf);

	if(packet->charIndex <= -1)
		ch->m_targetPC = NULL;
	else
	{
		CPC* pc = PCManager::instance()->getPlayerByCharIndex(packet->charIndex);
		if(pc == NULL)
		{
			LOG_ERROR("HACKING? Invalid charIndex. targetIndex[%d]", packet->charIndex);
		}
		
		ch->m_targetPC = pc;
	}
}

void do_noticeExtend(CPC* ch, CNetMsg::SP& rmsg)
{
	if(ch == NULL)
		return ;

	pTypeThirdBase* pThird = reinterpret_cast<pTypeThirdBase*>(rmsg->m_buf);
	int thirdtype = pThird->thirdType;

	switch(thirdtype)
	{
	case MSG_EX_NOTICE_LIST:
		Notice::instance()->SendNoticeList(ch->m_index, rmsg);
		break;
	}
}
//
