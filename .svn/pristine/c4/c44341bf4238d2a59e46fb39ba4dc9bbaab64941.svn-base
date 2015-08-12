#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>
#include <boost/format.hpp>
#include "stdhdrs.h"

#include "Log.h"
#include "Character.h"
#include "Server.h"
#include "CmdMsg.h"
#include "doFunc.h"
#include "Exp.h"
#include "WarCastle.h"
#include "../ShareLib/DBCmd.h"
#include "Utils.h"
#include "APetProto.h"
#include "Battle.h"
#include "DratanCastle.h"

#include "GMCmdList.h"

#include "ServerTimer.h"
#include "Area.h"
#include "Notice.h"
#include "Artifact_Manager.h"
#include "../ShareLib/packetType/ptype_tradeAgent_system.h"
#include "../ShareLib/packetType/ptype_old_do_item.h"
#include "../ShareLib/packetType/ptype_old_do_reform_system.h"
#include "../ShareLib/packetType/ptype_syndicate.h"
#include "../ShareLib/packetType/ptype_old_do_changejob.h"
#include "../ShareLib/packetType/ptype_old_do_skill.h"
#include "../ShareLib/packetType/ptype_old_do_sskill.h"
#include "../ShareLib/packetType/ptype_attendance.h"

#ifdef DURABILITY
#include "../ShareLib/packetType/ptype_durability.h"
#endif

#include "../ShareLib/gm_command.h"
#include "../ShareLib/packetType/ptype_server_to_server_kick.h"
#include "itemExchangeProto.h"
#include "DBManager.h"

#ifdef PREMIUM_CHAR
#include "../ShareLib/packetType/ptype_premium_char.h"
#endif

///////////////////
// 운영자 명령 관련

void do_GM(CPC* ch, CNetMsg::SP& msg)
{
	msg->MoveFirst();

	unsigned char subtype;

	RefMsg(msg) >> subtype;

	switch (subtype)
	{
	case MSG_GM_WHOAMI:
		{
			CNetMsg::SP rmsg(new CNetMsg);
			GMWhoAmIMsg(rmsg, ch);
			SEND_Q(rmsg, ch->m_desc);

			GAMELOG << init("GM COMMAND")
					<< "whoami" << delim
					<< ch->m_index << delim
					<< ch->m_name << delim
					<< ch->m_nick << delim
					<< ch->m_pZone->m_index
					<< end;
		}
		break;

	case MSG_GM_COMMAND:
		{
			static CLCString buf(MAX_MESSAGE_SIZE);
			char buf2[MAX_MESSAGE_SIZE] = {0,};

#if defined (LC_BILA)
			if ( ch->m_admin < 2 )
				return;
#endif
			buf = "";
			RefMsg(msg) >> buf;

			const char* p = buf;
			p = AnyOneArg(p, buf2, true);

			gGMCmdList->run(ch, buf);

#ifdef GMTOOL
			if(ch->m_desc->m_userFlag & IS_GM_CHAR)
			{
				CNetMsg::SP rmsg(new CNetMsg);
				ConnGMToolCommand(rmsg, ch->m_index, ch->m_name, buf);
				SEND_Q(rmsg, gserver->m_connector);
			}
#endif
		}
		break;
	}
}

void do_GMEcho(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	char tmpBuf[MAX_MESSAGE_SIZE] = {0,};
#ifdef GMTOOL
	if(ch->m_desc->m_userFlag & IS_GM_CHAR)
	{
		*tmpBuf = '\0';		// argument for arg
		if (!arg || !(*arg))		return ;

		int serverno, subno;
		char* p1;
		arg = AnyOneArg(arg, tmpBuf);
		p1 = tmpBuf;
		while (*p1)
		{
			if (!isdigit(*p1))
				return ;		//goto INVALID_FORMAT;
			p1++;
		}
		serverno = atoi(tmpBuf);

		if (!arg || !(*arg))
			return;

		arg = AnyOneArg(arg, tmpBuf);
		p1 = tmpBuf;
		while (*p1)
		{
			if (!isdigit(*p1))
				return ;		//goto INVALID_FORMAT;
			p1++;
		}
		subno = atoi(tmpBuf);

		arg = AnyOneArg(arg, tmpBuf, true);
		// 0: server, 1: all, 2: subserver
		int type;
		if (strcmp(tmpBuf, GM_CMD_ECHO_SERVER) == 0)		type = 0;
		else if (strcmp(tmpBuf, GM_CMD_ECHO_ALL) == 0)	type = 1;
		else if (strcmp(tmpBuf, GM_CMD_ECHO_SUBSERVER) == 0)		type = 2;
		else return ;

		arg = SkipSpaces(arg);
		if (!arg || !(*arg))		return ;

		{
			CNetMsg::SP rmsg(new CNetMsg);
			switch (type)
			{
			case 0:
				MsgrNoticeMsg(rmsg, -1, serverno, -1, -1, arg);
				break;
			case 1:
				MsgrNoticeMsg(rmsg, -1, -1, -1, -1, arg);
				//DB에 내용(arg) 저장 및 게임 서버 리비전 번호 업데이트
				//리비전 번호를 내가 알고 있는 것에 +1 을 해서 저장 하도록 함.
				//후에 메신저 서버에서 패킷을 받으면 리비전 번호를 +1 해줌.
				break;
			case 2:
				MsgrNoticeMsg(rmsg, -1, serverno, subno, -1, arg);
				break;
			}
			SEND_Q(rmsg, gserver->m_messenger);
		}

		return ;
	}
#endif // GMTOOL
	*tmpBuf = '\0';		// argument for arg
	if (!arg || !(*arg))		return ;
	arg = AnyOneArg(arg, tmpBuf, true);
	// 0: zone, 1: server, 2: all, 3: subserver
	int type;
	if (strcmp(tmpBuf, GM_CMD_ECHO_ZONE) == 0)		type = 0;
	else if (strcmp(tmpBuf, GM_CMD_ECHO_SERVER) == 0)	type = 1;
	else if (strcmp(tmpBuf, GM_CMD_ECHO_ALL) == 0)		type = 2;
	else if (strcmp(tmpBuf, GM_CMD_ECHO_SUBSERVER) == 0)	type = 3;
	else if (strcmp(tmpBuf, GM_CMD_ECHO_ALLZONE) == 0)	type = 4;
	else return ;

	unsigned int	nUntiltime = 0 ;
	unsigned int	nRepeattime = 0;
	bool			bUntil = false;
	AnyOneArg(arg, tmpBuf);
	if( strcmp("until",tmpBuf)==0 )
	{
		arg = AnyOneArg(arg, tmpBuf);
		arg = AnyOneArg(arg, tmpBuf);
		nRepeattime = atoi(tmpBuf);
		arg = AnyOneArg(arg, tmpBuf);
		nUntiltime = atoi(tmpBuf);
		bUntil = true;
	}

	arg = SkipSpaces(arg);
	if (!arg || !(*arg))		return ;

	{
		CNetMsg::SP rmsg(new CNetMsg);
		switch (type)
		{
		case 0:
			MsgrNoticeMsg(rmsg, -1, gserver->m_serverno, gserver->m_subno, ch->m_pZone->m_index, arg);
			break;
		case 1:
			MsgrNoticeMsg(rmsg, -1, gserver->m_serverno, -1, -1, arg);
			break;
		case 2:
			MsgrNoticeMsg(rmsg, -1, -1, -1, -1, arg);
			//리비전을 1 증가 시킨 후
			Notice::instance()->setLastRev(Notice::instance()->getLastRev() + 1);
			//DB에 저장
			{
				std::string query = boost::str(boost::format("INSERT INTO t_notice_all values(%d, '%s', now())") % Notice::instance()->getLastRev() % arg);
				DBManager::instance()->pushQueryForDataDB(0, query);
				CNetMsg::SP rmsg2(new CNetMsg);
				MsgrUpdateRevMsg(rmsg2, -1, -1, -1, -1, Notice::instance()->getLastRev(), arg, gserver->getNowSecond());
				SEND_Q(rmsg2, gserver->m_messenger);
			}
			break;
		case 3:
			MsgrNoticeMsg(rmsg, -1, gserver->m_serverno, gserver->m_subno, -1, arg);
			break;
		case 4:
			MsgrNoticeMsg(rmsg, -1, -1, -1, ch->m_pZone->m_index, arg);
			break;
		}
		SEND_Q(rmsg, gserver->m_messenger);

		if( bUntil )
			gserver->gecho.Set( rmsg, nRepeattime , nUntiltime );
	}
}

void do_GMGoZone(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	char tmpBuf[MAX_MESSAGE_SIZE] = {0,};
	
	if (!arg || !(*arg))
	{
		CNetMsg::SP rmsg(new CNetMsg);
		SysMsg(rmsg, MSG_SYS_DO_NOT_GO_ZONE_GMORDER);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
	
	arg = AnyOneArg(arg, tmpBuf);
	int zone = atoi(tmpBuf);
	
	if (!arg || !(*arg))
	{
		CNetMsg::SP rmsg(new CNetMsg);
		SysMsg(rmsg, MSG_SYS_DO_NOT_GO_ZONE_GMORDER);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
	
	arg = AnyOneArg(arg, tmpBuf);
	int extra = atoi(tmpBuf);

	if (zone == ZONE_SINGLE_DUNGEON_TUTORIAL)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		SysMsg(rmsg, MSG_SYS_DO_NOT_GO_ZONE_GMORDER);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	if (zone == ZONE_COMBO_DUNGEON)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		SysMsg(rmsg, MSG_SYS_DO_NOT_GO_ZONE_GMORDER);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	if (zone == ZONE_SINGLE_DUNGEON1 || zone == ZONE_SINGLE_DUNGEON2)
	{
		CQuest* pQuest = NULL;

		int i;
		for (i=0; i < gserver->m_questProtoList.m_nCount; i++)
		{
			if (zone == ZONE_SINGLE_DUNGEON1)
			{
				if (gserver->m_questProtoList.m_proto[i].m_type[0] == QTYPE_KIND_DEFEAT)
				{
					pQuest = ch->m_questList.AddQuest(gserver->m_questProtoList.m_proto[i].m_index);
					break;
				}
			}

			if (zone == ZONE_SINGLE_DUNGEON2)
			{
				if (gserver->m_questProtoList.m_proto[i].m_type[0] == QTYPE_KIND_SAVE)
				{
					pQuest = ch->m_questList.AddQuest(gserver->m_questProtoList.m_proto[i].m_index);
					break;
				}
			}
		}

		if (pQuest == NULL)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			SysMsg(rmsg, MSG_SYS_DO_NOT_GO_ZONE_GMORDER);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}
		ch->m_questList.SetQuestState(pQuest, QUEST_STATE_RUN);
		pQuest->SetComplete1(false);
		pQuest->SetComplete2(false);

		// Quest Start Log
#ifdef GER_LOG
		GAMELOGGEM << init( 0, "CHAR_QUEST_STARTED")
				   << LOG_VAL("account-id", ch->m_desc->m_idname) << blank
				   << LOG_VAL("char-id", ch->m_desc->m_pChar->m_name) << blank
				   << LOG_VAL("zone-id", ch->m_desc->m_pChar->m_pZone->m_index ) << blank
				   << LOG_VAL("start-type", pQuest->GetQuestProto()->m_startType ) << blank
				   << LOG_VAL("npc-id", pQuest->GetQuestProto()->m_startData ) << blank
				   << LOG_VAL("quest-id", pQuest->GetQuestIndex() ) << blank
				   << endGer;

#endif // GERLOG
		GAMELOG << init("QUEST START", ch)
				<< pQuest->GetQuestIndex()
				<< end;

		{
			// Quest Start Msg
			CNetMsg::SP rmsg(new CNetMsg);
			QuestStartMsg(rmsg, pQuest);
			SEND_Q(rmsg, ch->m_desc);
			return;
		}
	}

	// 동일 싱글 던전으로는 이동 불가
	if ((ch->m_pZone->IsPersonalDungeon() || ch->m_pZone->IsGuildRoom()) && ch->m_pZone->m_index == zone)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		SysMsg(rmsg, MSG_SYS_DO_NOT_GO_ZONE_GMORDER);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	// 존을 찾아서
	CZone* pZone = gserver->FindZone(zone);
	if (pZone == NULL)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		SysMsg(rmsg, MSG_SYS_DO_NOT_GO_ZONE_GMORDER);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	if (extra < 0 || extra >= pZone->m_countZonePos)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		SysMsg(rmsg, MSG_SYS_DO_NOT_GO_ZONE_GMORDER);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	GoZone(ch, zone,
		   pZone->m_zonePos[extra][0],														// ylayer
		   GetRandom(pZone->m_zonePos[extra][1], pZone->m_zonePos[extra][3]) / 2.0f,		// x
		   GetRandom(pZone->m_zonePos[extra][2], pZone->m_zonePos[extra][4]) / 2.0f);		// z
}

void do_GMGoto(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	char tmpBuf[MAX_MESSAGE_SIZE] = {0,};
	if (!arg || !(*arg))		return ;
	arg = AnyOneArg(arg, tmpBuf);
	float x = atof(tmpBuf);
	if (!arg || !(*arg))		return ;
	arg = AnyOneArg(arg, tmpBuf);
	float z = atof(tmpBuf);
	if (!arg || !(*arg))		return ;
	arg = AnyOneArg(arg, tmpBuf);
	char ylayer = (char)atoi(tmpBuf);

	//if (ylayer < 0 || ylayer >= ch->m_pZone->m_countY)
	// yLayer 수정
	if (ylayer < 0 || ylayer > ((ch->m_pZone->m_countY - 1) * 2))
		return ;

	if (x < 0 || z < 0 || x >= ch->m_pZone->m_attrMap[ylayer/2].m_size[0] || z >= ch->m_pZone->m_attrMap[ylayer/2].m_size[1])
		return ;

	GoTo(ch, ylayer, x, z, ch->m_pArea->GetHeight(ylayer, x, z), GET_R(ch));
}

void do_GMItemDrop(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	char tmpBuf[MAX_MESSAGE_SIZE] = {0,};
	int dbindex, plus = 0, flag = 0;
	LONGLONG count = 1;

	int		optionCount = 0;
	boost::scoped_array<int>	optionType;
	boost::scoped_array<int>	optionLevel;

	if (!arg || !(*arg))		return ;
	arg = AnyOneArg(arg, tmpBuf);
	dbindex = atoi(tmpBuf);
	if (arg && (*arg))
	{
		arg = AnyOneArg(arg, tmpBuf);
		plus = atoi(tmpBuf);
	}
	if (arg && (*arg))
	{
		arg = AnyOneArg(arg, tmpBuf);
		flag = atoi(tmpBuf);
	}
	if (arg && (*arg))
	{
		arg = AnyOneArg(arg, tmpBuf);
		count = ATOLL(tmpBuf);
	}
	int socketcount = 0;
	if (arg && (*arg))
	{
		arg = AnyOneArg(arg, tmpBuf);
		socketcount = atoi(tmpBuf);
	}

	if (count == 0)
		count = 1;

	if(dbindex <= 0 || dbindex == NAS_ITEM_DB_INDEX)
		return;

	CItem* item = gserver->m_itemProtoList.CreateItem(dbindex, -1, plus, flag, count);

	if (item == NULL)
		return ;

	if (socketcount > 0 && item->m_itemProto->getItemFlag() & ITEM_FLAG_SOCKET)
	{
		if(socketcount > 10)//old socket 생성
		{
			if(item->m_socketList.SetSocketCountForAdmin(socketcount, true))
			{
				item->setFlag(item->getFlag() | FLAG_ITEM_SOCKET);
			}
		}
		else if(socketcount > 0)
		{
			if(item->m_socketList.SetSocketCountForAdmin(socketcount))
			{
				item->setFlag(item->getFlag() | FLAG_ITEM_SOCKET);
			}
		}
	}

	if( item->getDBIndex() == 2148 && gserver->isActiveEvent( A_EVENT_EGGS_HUNT) )
		item->setUsed(1);	// 1: 운영자가 떨어뜨린 것, -1: 몬스터가 떨어뜨린 것

	// itemdrop <index> <plus> <flag> <count> <option_count> <option_type> <option_level>
	if (arg && (*arg))
	{
		arg = AnyOneArg(arg, tmpBuf);
		optionCount = atoi(tmpBuf);

		bool bIsRare = false;

		if (item->IsRareItem())
			bIsRare = true;

		if (bIsRare)
		{
			if (optionCount == 1)
			{
				if (!arg || !(*arg))
				{
					delete item;
					return ;
				}
				arg = AnyOneArg(arg, tmpBuf);
				int nRareIndex = atoi(tmpBuf);

				if (!arg || !(*arg))
				{
					delete item;
					return ;
				}
				arg = AnyOneArg(arg, tmpBuf);
				int nRareBit = atoi(tmpBuf);

				const CRareOptionProto* pProto = gserver->m_rareOptionList.Find(nRareIndex);
				if (pProto)
				{
					int i;
					for (i = 0; i < MAX_RARE_OPTION_SETTING; i++)
					{
						const RAREOPTIONDATA* pROD = pProto->GetData(i);
						if (pROD->optionlevel == 0)
						{
							nRareBit &= ~pROD->rarebit;
						}
					}
					if (nRareBit != 0)
					{
						item->m_nOption = 2;
						item->m_option[0].GetDBValue((short)nRareIndex);
						item->m_option[1].GetDBValue((short)nRareBit);
						item->m_pRareOptionProto = pProto;
						item->m_nRareOptionIndex = nRareIndex;
						item->m_nRareOptionBit = nRareBit;
					}
				}
			}
			else
			{
				delete item;
				return ;
			}
		}
		else
		{
			// 옵션 부분 추가
			if (optionCount > 0 && optionCount <= MAX_ITEM_OPTION)
			{
				optionType.reset(new int[optionCount]);
				optionLevel.reset(new int[optionCount]);

				int i;
				for (i=0; i < optionCount; i++)
				{
					if (!arg || !(*arg))
					{
						delete item;
						return ;
					}
					arg = AnyOneArg(arg, tmpBuf);
					optionType[i] = atoi(tmpBuf);

					if (!arg || !(*arg))
					{
						delete item;
						return ;
					}
					arg = AnyOneArg(arg, tmpBuf);
					optionLevel[i] = atoi(tmpBuf);

					if (optionType[i] < 0 || optionType[i] >= MAX_NUM_OPTION || optionLevel[i] < 1 )
						continue;

					// Option 붙이기
					COptionProto* proto = gserver->m_optionProtoList.FindProto(optionType[i]);

					if (!proto)
						continue;

					if( (int) proto->m_levelValue.size() < optionLevel[i] )
						continue;

					item->m_option[item->m_nOption].m_proto = proto;
					item->m_option[item->m_nOption].m_type = optionType[i];
					item->m_option[item->m_nOption].m_level = optionLevel[i];
					item->m_option[item->m_nOption].m_value = proto->m_levelValue[optionLevel[i] - 1];
					item->m_option[item->m_nOption].SetDBValue();
					++item->m_nOption;
				}
			}
		}

		if (item->IsOriginItem())
		{
			item->m_pSetItemProto = gserver->m_setItemProtoList.Find(item->m_itemProto->GetSetIndex());
		}
	}

#ifdef SYSTEM_TREASURE_MAP
	if( item->getDBIndex() == TREASURE_MAP_ITEM_INDEX )
	{
		// 드랍 지역도 아니고 드랍지역이 필드도 아니고 아이템에 맵 셋팅이 제대로 않되었따면 드랍하지 않는다.
		if( !ch->m_pZone->CheckTreasureDropFlag() || !ch->m_pZone->IsFieldZone() || !ch->m_pZone->SetTresaureMapInfo( item ) )
		{
			delete item;
			item = NULL;

			return;
		}
	}
#endif

	if(item->m_itemProto->getItemTypeIdx() == ITYPE_ACCESSORY && item->m_itemProto->getItemSubTypeIdx() == IACCESSORY_ARTIFACT)
	{
		if(ArtifactManager::instance()->getCount() >= ARTIFACT_MAX_ITEM_COUNT)
			return ;
	}

	if (item->IsAccessary() && item->m_nOption == 0)
	{
		OptionSettingItem(ch, item);
	}

	ch->m_pArea->DropItem(item, ch);

	{
		CNetMsg::SP rmsg(new CNetMsg);
		ItemDropMsg(rmsg, ch, item);
		ch->m_pArea->SendToCell(rmsg, ch, true);
	}

	GAMELOG << init("ITEM_LOAD", ch)
			<< itemlog(item)
			<< end;
}

void do_GMCountPC(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	char tmpBuf[MAX_MESSAGE_SIZE] = {0,};
	int mode = 0;
	int server = -1;

	if (!arg || !*arg)
		mode = 0;

	else
	{
		arg = AnyOneArg(arg, tmpBuf, true);
		TrimString(tmpBuf);

		if (strlen(tmpBuf) == 0)
			mode = 0;
		else if (strcmp(tmpBuf, "all") == 0)
			mode = 1;
		else
		{
			mode = 2;
			server = atoi(tmpBuf);
		}
	}

	{
		// 메신저에 요청
		CNetMsg::SP rmsg(new CNetMsg);
		switch (mode)
		{
		case 0:		// 현재 서버 - 서브
			MsgrPlayerCountReq(rmsg, gserver->m_serverno, gserver->m_subno, ch->m_index);
			break;
		case 1:		// 전체 서버
			MsgrPlayerCountReq(rmsg, -1, -1, ch->m_index);
			break;
		case 2:		// 특정 서버군
			MsgrPlayerCountReq(rmsg, server, -1, ch->m_index);
			break;
		default:
			return ;
		}
		SEND_Q(rmsg, gserver->m_messenger);
	}
}

void do_GMLoopCount(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	char tmpBuf[MAX_MESSAGE_SIZE] = {0,};
#ifdef GMTOOL
	if(ch->m_desc->m_userFlag & IS_GM_CHAR)
	{
		if (!arg || !(*arg))
			return;

		int serverno, subno;
		char* p1;
		arg = AnyOneArg(arg, tmpBuf);
		p1 = tmpBuf;
		while (*p1)
		{
			if (!isdigit(*p1))
				return ;		//goto INVALID_FORMAT;
			p1++;
		}
		serverno = atoi(tmpBuf);

		if (!arg || !(*arg))
			return;

		arg = AnyOneArg(arg, tmpBuf);
		p1 = tmpBuf;
		while (*p1)
		{
			if (!isdigit(*p1))
				return ;		//goto INVALID_FORMAT;
			p1++;
		}
		subno = atoi(tmpBuf);

		gserver->m_descLoop = ch->m_desc;

		{
			CNetMsg::SP rmsg(new CNetMsg);
			MsgExtraGMCommandReq(rmsg, serverno, subno, MSG_MSGR_GM_LOOPCOUNT_REQ);
			SEND_Q(rmsg, gserver->m_messenger);
		}

		{
			CNetMsg::SP rmsg(new CNetMsg);
			SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Sending LoopCount");
			SEND_Q(rmsg, ch->m_desc);
		}

		return ;
	}
#endif // GMTOOL

	if (gserver->m_bLoop)
		return ;

	gserver->m_bLoop = 1;
	gserver->m_nLoop = 0;
	GetTimeofDay(&gserver->m_tvLoop, NULL);
	gserver->m_descLoop = ch->m_desc;
}

void do_GMImmortal(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	CLCString chat(100);

	if (ch->m_bImmortal)
	{
		ch->m_bImmortal = false;
		chat = "Immortal Set Off";
	}
	else
	{
		ch->m_bImmortal = true;
		DelAttackList(ch);
		chat = "Immortal Set On";
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", chat);
		SEND_Q(rmsg, ch->m_desc);
	}
}

void do_GMShutdown(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	char tmpBuf[MAX_MESSAGE_SIZE] = {0,};
#ifdef GMTOOL
	if(ch->m_desc->m_userFlag & IS_GM_CHAR)
	{
		if (!arg || !(*arg))
			return;

		int serverno, subno;
		char* p1;
		arg = AnyOneArg(arg, tmpBuf);
		p1 = tmpBuf;
		while (*p1)
		{
			if (!isdigit(*p1))
				return ;		//goto INVALID_FORMAT;
			p1++;
		}
		serverno = atoi(tmpBuf);

		if (!arg || !(*arg))
			return;

		arg = AnyOneArg(arg, tmpBuf);
		p1 = tmpBuf;
		while (*p1)
		{
			if (!isdigit(*p1))
				return ;		//goto INVALID_FORMAT;
			p1++;
		}
		subno = atoi(tmpBuf);

		arg = AnyOneArg(arg, tmpBuf);
		int t = atoi(tmpBuf);
		CLCString chat = arg;
		chat.Trim();

		{
			CNetMsg::SP rmsg(new CNetMsg);
			MsgrGMShutdownReqMsg(rmsg, serverno, subno, t, (const char*)chat);
			SEND_Q(rmsg, gserver->m_messenger);
		}
		return ;
	}
#endif // GMTOOL
	if (!arg || !(*arg))
		return;

	if (ShutdownTimer::instance()->isRun())
		return;

	arg = AnyOneArg(arg, tmpBuf);
	int sec = atoi(tmpBuf);
	CLCString chat = arg;
	chat.Trim();

	if (chat.Length() > 0)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", chat);
		PCManager::instance()->sendToAll(rmsg);
	}

	ShutdownTimer::instance()->Init(sec);
}

void do_GMReboot(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	char tmpBuf[MAX_MESSAGE_SIZE] = {0,};
	if (!arg || !(*arg))		return ;

	if (gserver->m_pulseReboot != -1)
		return ;

	arg = AnyOneArg(arg, tmpBuf);

	CNetMsg::SP rmsg(new CNetMsg);
	if( strcmp(tmpBuf, GM_CMD_ECHO_ALL) == 0 )
	{
		MsgrRebootReqMsg(rmsg, -1, -1, -1, -1);
	}
	else if( strcmp(tmpBuf, GM_CMD_ECHO_SERVER) == 0 )
	{
		MsgrRebootReqMsg(rmsg, -1, gserver->m_serverno, -1, -1);
	}
	else if( strcmp(tmpBuf, GM_CMD_ECHO_SUBSERVER) == 0 )
	{
		MsgrRebootReqMsg(rmsg, -1, gserver->m_serverno, gserver->m_subno, -1);
	}
	else
	{
		return;
	}

	arg = AnyOneArg(arg, tmpBuf);
	int t = atoi(tmpBuf);
	CLCString chat = arg;
	chat.Trim();

	RefMsg(rmsg) << t
				 << chat;
	SEND_Q(rmsg, gserver->m_messenger);
}

void do_GMCountNPC(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	CLCString chat(100);
	int i;

	for (i=0; i < gserver->m_numZone; i++)
	{
		if (gserver->m_zones[i].m_countArea > 1)
			continue;

		chat.Format("Zone :%d First :%d Now :%d ShopCount :%d"
					, gserver->m_zones[i].m_index, gserver->m_zones[i].m_area[0].m_npcRegenList.m_nCount, gserver->m_zones[i].m_area[0].m_nCountNPC, gserver->m_zones[i].m_nShopCount);

		{
			CNetMsg::SP rmsg(new CNetMsg);
			SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", chat);
			SEND_Q(rmsg, ch->m_desc);
		}
	}
}

void do_GMVisible(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	ch->SetVisible();

	CNetMsg::SP rmsg(new CNetMsg);

	if (ch->m_bVisible)
	{
		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Visible: ON");
	}
	else
	{
		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Visible: OFF");
	}
	SEND_Q(rmsg, ch->m_desc);
}

void do_GMGoPC(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	if (!arg || !(*arg))		return;

	// 이름 가져 오기
	CLCString name = arg;
	name.Trim();

	// 나랑 비교
	if (strcmp(name, ch->m_name) == 0 || strcmp(name, ch->m_nick) == 0)
		return ;

	// 전체 유저 중에
	CPC* pc = PCManager::instance()->getPlayerByName(name, true);

	if (pc == NULL)
	{
		// TODO : 아예 없으면 Use 메신저
		// 없으면 메시지
		CNetMsg::SP rmsg(new CNetMsg);
		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Not Found the PC");
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	if (strcmp(pc->m_name, name) == 0 || strcmp(pc->m_nick, name) == 0)
	{
		if (pc->m_pZone->IsPersonalDungeon())
		{
			CNetMsg::SP rmsg(new CNetMsg);
			SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "This PC in SINGLE_DUNGEON");
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}

		if (pc->m_bPlaying == false)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "This PC is NOT playing!");
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}

		GMGoZone(ch, pc);
		return ;
	}
}

void do_GMGoNPC(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	if (!arg || !(*arg))		return;

	char tmpBuf[MAX_MESSAGE_SIZE] = {0,};

	arg = AnyOneArg(arg, tmpBuf);

	CLCString name(128);
	name = tmpBuf;
	name.Trim();
	const char* str = name;

	if(str == NULL) return;

	if (strlen(str) < 1)
		return ;

	bool bName = false;
	while (*str)
	{
		if ( (*str) < 48 || (*str) > 57 )
		{
			bName = true;
			break;
		}
		str++;
	}

	bool bToMe = false;
	if( bName && strncmp( str, "tome", 4 ) == 0 )
	{
		arg = AnyOneArg(arg, tmpBuf);
		name = tmpBuf;
		name += arg;
		name.Trim();

		if ( name.Length() < 1)
			return ;

		const char* str2 = name;
		bName = false;
		while (*str2)
		{
			if (!isdigit(*str2))
			{
				bName = true;
				break;
			}
			str2++;
		}

		bToMe = true;
	}
	else
	{
		name += arg;
		name.Trim();
	}

	int index = -1;
	if (!bName)
		index = atoi(name);

	CArea* area = ch->m_pArea;

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
					if ((index == -1 && strcmp(npc->m_name, name) == 0) || (index != -1 && npc->m_idNum == index))
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
		// 없으면 메시지
		CNetMsg::SP rmsg(new CNetMsg);
		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Not Found the NPC");
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if( bToMe )
	{
		GoTo(ret, GET_YLAYER(ch), GET_X(ch), GET_Z(ch), ret->m_pArea->GetHeight(GET_YLAYER(ch), GET_X(ch), GET_Z(ch)), GET_R(ret));
	}
	else
		// 이동 시키기
		GoTo(ch, GET_YLAYER(ret), GET_X(ret), GET_Z(ret), ch->m_pArea->GetHeight(GET_YLAYER(ret), GET_X(ret), GET_Z(ret)), GET_R(ch));
}

void do_GMLevelUP(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	if  (!arg || !(*arg))		return;

	CLCString var = arg;
	var.Trim();

	if (var.Length() < 1)
		return ;

	int level = atoi(var);

	if (level < 1 || level > MAX_LEVEL)
		return;

	ch->m_exp = 0;
	ch->m_level = level;

	{
		CNetMsg::SP rmsg(new CNetMsg);
		SubHelperLevelup(rmsg, ch->m_index);
		SEND_Q(rmsg, gserver->m_subHelper);
	}
	ch->LevelUp(true);

	ch->CalcStatus(true);
}

void do_GMKICK(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	if (arg == NULL)
		return;

	std::string kickedName(arg);
	boost::algorithm::trim(kickedName);

	if (kickedName == ch->GetName())
	{
		CNetMsg::SP rmsg(new CNetMsg);
		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "can't kick myself");
		SEND_Q(rmsg, ch->m_desc);

		return;
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		ServerToServerPacket::makeKickUser(rmsg, ch->m_index, kickedName.c_str());
		SEND_Q(rmsg, gserver->m_helper);
	}
}

void do_GMComon(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	if (!arg || !(*arg))		return;

	// 이름 가져 오기
	CLCString name = arg;
	name.Trim();

	// 나랑 비교
	if (strcmp(name, ch->m_nick) == 0)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Can't summon myself");
		SEND_Q(rmsg, ch->m_desc);

		return ;
	}

	CPC* pc = PCManager::instance()->getPlayerByName(name, true);
	if (pc == NULL)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Not Found the PC");
		SEND_Q(rmsg, ch->m_desc);

		return;
	}

	// 같은 존
	if (pc->m_pZone == ch->m_pZone)
	{
		// 이동 시키기
		GoTo(pc, GET_YLAYER(ch), GET_X(ch), GET_Z(ch), pc->m_pArea->GetHeight(GET_YLAYER(ch), GET_X(ch), GET_Z(ch)), GET_R(pc));
		return ;
	}
	// 같은 존에 없으면 해당존으로 ch의 좌표로 이동
	else
	{
		if (ch->m_pZone->IsPersonalDungeon() || ch->m_pZone->IsGuildRoom())
		{
			CNetMsg::SP rmsg(new CNetMsg);
			SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "I is Personal Dungeon or Guild Room");
			SEND_Q(rmsg, ch->m_desc);

			return;
		}

		if (pc->m_bPlaying == false)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "PC is not playing");
			SEND_Q(rmsg, ch->m_desc);

			return;
		}

		GoZone(pc, ch->m_pZone->m_index, ch->m_pos.m_yLayer, ch->m_pos.m_x, ch->m_pos.m_z);
		return;
	}
}

void do_GMSummon(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	char tmpBuf[MAX_MESSAGE_SIZE] = {0,};
	char buf2[MAX_MESSAGE_SIZE] = {0,};
	int npcindex = -1;
	int count = 1;
	int i;
	bool bName = false;
	const char* str = tmpBuf;
	const char* name = tmpBuf;

	tmpBuf[0] = '\0';
	buf2[0] = '\0';
	arg = AnyOneArg(arg, tmpBuf);		// npc
	TrimString(tmpBuf);
	CLCString var = arg;
	var.Trim();

	if (tmpBuf[0] == '\0')
		return ;
	if (var[0] != '\0')
	{
		count = atoi(tmpBuf);
		str = var;
		name = var;
	}

	if (ch->IsInPeaceZone(true))
		return ;

	while (!bName && *str)
	{
		if (!isdigit(*str))
			bName = true;
		str++;
	}
	if (bName)
	{
		for (i = 0; i < gserver->m_npcProtoList.m_nCount; i++)
		{
			if (strcmp(gserver->m_npcProtoList.m_npcProtoList[i].m_name, name) == 0)
			{
				npcindex = gserver->m_npcProtoList.m_npcProtoList[i].m_index;
				break;
			}
		}
		if (npcindex == -1)
			return ;
	}
	else
	{
		npcindex = atoi(name);
		CNPCProto* proto = gserver->m_npcProtoList.FindProto(npcindex);
		if (!proto)
			return ;
	}

	if (count > 2000)
		count = 2000;

	for (i = 0; i < count; i++)
	{
		CNPC* npc = gserver->m_npcProtoList.Create(npcindex, NULL);
		if (!npc)
			return ;

		int nTry = 0;
		do
		{
			GET_X(npc) = GET_X(ch) + (GetRandom(0, 1) ? -1 : 1) * GetRandom(20, 50) / 10.0f;
			GET_Z(npc) = GET_Z(ch) + (GetRandom(0, 1) ? -1 : 1) * GetRandom(20, 50) / 10.0f;
			GET_YLAYER(npc) = GET_YLAYER(ch);
			GET_R(npc) = GetRandom(0, (int) (PI_2 * 10000)) / 10000;
			if (ch->m_pArea->GetAttr(GET_YLAYER(npc), GET_X(npc), GET_Z(npc)) & MATT_WALKABLE)
				break;
			nTry++;
		}
		while (nTry <= 10);
		if (nTry > 10)
		{
			GET_X(npc) = GET_X(ch);
			GET_Z(npc) = GET_Z(ch);
			GET_YLAYER(npc) = GET_YLAYER(ch);
			GET_R(npc) = GetRandom(0, (int) (PI_2 * 10000)) / 10000;
		}

		npc->m_regenX = GET_X(npc);
		npc->m_regenY = GET_YLAYER(npc);
		npc->m_regenZ = GET_Z(npc);

		int cx, cz;
		ch->m_pArea->AddNPC(npc);
		ch->m_pArea->PointToCellNum(GET_X(npc), GET_Z(npc), &cx, &cz);
		ch->m_pArea->CharToCell(npc, GET_YLAYER(npc), cx, cz);

		{
			CNetMsg::SP rmsg(new CNetMsg);
			AppearMsg(rmsg, npc, true);
			ch->m_pArea->SendToCell(rmsg, GET_YLAYER(npc), cx, cz);
		}
	}
}

void do_GMSilence(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	char tmpBuf[MAX_MESSAGE_SIZE] = {0,};
#ifdef GMTOOL
	if(ch->m_desc->m_userFlag & IS_GM_CHAR)
	{
		if (!arg || !(*arg))		return ;

		char charname[100];
		char* p1;
		arg =  AnyOneArg(arg, tmpBuf);
		TrimString(tmpBuf);
		strcpy(charname, tmpBuf);

		if (strlen(tmpBuf) < 4)
			return;

		if (!arg || !(*arg))
			return ;

		int sec;
		arg =  AnyOneArg(arg, tmpBuf);
		TrimString(tmpBuf);
		p1 = tmpBuf;
		while (*p1)
		{
			if (!isdigit(*p1))
				return ;		//goto INVALID_FORMAT;
			p1++;
		}
		sec = atoi(tmpBuf);
		if(sec < 1)
			return;

		if (!arg || !(*arg))
			return ;

		int serverno, subno;
		arg =  AnyOneArg(arg, tmpBuf);
		TrimString(tmpBuf);
		p1 = tmpBuf;
		while (*p1)
		{
			if (!isdigit(*p1))
				return ;		//goto INVALID_FORMAT;
			p1++;
		}
		serverno = atoi(tmpBuf);

		if (!arg || !(*arg))
			return ;
		arg =  AnyOneArg(arg, tmpBuf);
		TrimString(tmpBuf);
		p1 = tmpBuf;
		bool bResult = false;
		while (*p1)
		{
			if (!isdigit(*p1))
			{
				//return ;		//goto INVALID_FORMAT;
				bResult = true;
				break;
			}
			p1++;
		}
		if ( bResult )
			subno = -1;
		else
			subno = atoi(tmpBuf);

		{
			CNetMsg::SP rmsg(new CNetMsg);
			MsgGMSilenceReq(rmsg, 0, serverno, subno, ch->m_index, charname, sec);
			SEND_Q(rmsg, gserver->m_messenger);
		}

		return ;
	}
#endif // GMTOOL

	if (!arg || !(*arg))		return ;

	arg = AnyOneArg(arg, tmpBuf);

	if (strlen(tmpBuf) < 4)
		return;

	int sec = 0;

	PCManager::map_t& playerMap			= PCManager::instance()->getPlayerMap();
	PCManager::map_t::iterator iter		= playerMap.begin();
	PCManager::map_t::iterator endIter	= playerMap.end();

	for (; iter != endIter; ++iter)
	{
		CPC* pc = (*iter).pPlayer;
		if (pc == NULL)
			continue;
		// 유저 검사
		if (( strcmp(pc->m_name, tmpBuf) == 0 || strcmp(pc->m_nick, tmpBuf) == 0 ))
		{
			arg = AnyOneArg(arg, tmpBuf);
			sec = atoi(tmpBuf);

			if (sec < 1)
				return;

			pc->m_silencePulse = gserver->m_pulse + (sec * PULSE_REAL_SEC);
#ifdef GM_COMMAND_NOTICE
			{
				CNetMsg::SP rmsg(new CNetMsg);
				MsgSetGMCommandNotice(rmsg, MSG_GM_COMMAND_NOTICE_SILENCE,ch->GetName() ,pc->GetName() );
				PCManager::instance()->sendToAll(rmsg);
			}
#endif // GM_COMMAND_NOTIC
		}
	}
}

void do_GMWhereAmI(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	CLCString chat(100);

	chat.Format("Server: %d, Subserver: %d, Zone: %d, Area: %d", gserver->m_serverno, gserver->m_subno, ch->m_pZone->m_index, ch->m_pArea->m_index);

	CNetMsg::SP rmsg(new CNetMsg);
	SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", chat);
	SEND_Q(rmsg, ch->m_desc);
}

void do_GMQuestComplete(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	if (!arg || !(*arg))	return;

	CLCString var = arg;
	var.Trim();

	if (var.Length() < 1)
		return ;


	if( strcmp((const char*)var, "all") == 0 )
	{
		CQuest* pQuest;
		CQuest* pQuestNext = ch->m_questList.GetNextQuest(NULL, QUEST_STATE_ALL);
		ch->m_questList.DelQuestALL(ch, QUEST_STATE_ALL);

		int count = gserver->m_questProtoList.m_nCount;
		for(int i=0; i<count; i++)
		{
			pQuest = ch->m_questList.AddQuest(gserver->m_questProtoList.m_proto[i].m_index);
			if (pQuest == NULL)
				return;
			ch->m_questList.SetQuestState(pQuest, QUEST_STATE_RUN);
			pQuest->SetComplete1(false);
			pQuest->SetComplete2(false);
		}

		pQuestNext = ch->m_questList.GetNextQuest(NULL, QUEST_STATE_RUN);
		while(pQuest = pQuestNext)
		{
			if(pQuest == NULL)
				return;
			pQuestNext = ch->m_questList.GetNextQuest(pQuestNext, QUEST_STATE_RUN);
			// 진행중인 퀘스트가 있고 완료되지 않았고 index가 같으면
			if (!pQuest->IsCompleted())
			{
				pQuest->SetComplete1(true);
				pQuest->SetComplete2(true);
				ch->m_questList.SetQuestState(pQuest, QUEST_STATE_DONE);
			}
		}
		CNetMsg::SP rmsg(new CNetMsg);
		std::string tstr = boost::str(boost::format("QuestComplete ALL OK"));
		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", tstr.c_str());
		SEND_Q(rmsg, ch->m_desc);
	}
	else
	{
		int index = atoi(var);
		CQuest* pQuest;
		CQuest* pQuestNext = ch->m_questList.GetNextQuest(NULL, QUEST_STATE_RUN);
		while ((pQuest = pQuestNext))
		{
			pQuestNext = ch->m_questList.GetNextQuest(pQuestNext, QUEST_STATE_RUN);
			// 진행중인 퀘스트가 있고 완료되지 않았고 index가 같으면
			if (!pQuest->IsCompleted() && pQuest->GetQuestIndex() == index)
			{
				// Quest Complete Log
				GAMELOG << init("QUEST COMPLETE (GMQuestComplete)", ch)
						<< pQuest->GetQuestIndex()
						<< end;

				CNetMsg::SP rmsg(new CNetMsg);
				QuestCompleteMsg(rmsg, pQuest);
				SEND_Q(rmsg, ch->m_desc);

				pQuest->SetComplete1(true);
				pQuest->SetComplete2(true);
				return;
			}
		}
	}
}

void do_GMStatPoint(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	if (!arg || !(*arg))		return;

	char tmpBuf[MAX_MESSAGE_SIZE] = {0,};
	arg = AnyOneArg(arg, tmpBuf);

	if( !strcmp(tmpBuf, "all") )
	{
		if (!arg || !(*arg))		return;
		arg = AnyOneArg(arg, tmpBuf);
		int allstat = atoi(tmpBuf);

		if( allstat < 0 )
			return;

		ch->m_statpt_remain = allstat;

		{
			CNetMsg::SP rmsg(new CNetMsg);
			StatPointRemainMsg(rmsg, ch);
			SEND_Q(rmsg, ch->m_desc);
		}

		ch->m_statpt_str = allstat;
		ch->m_statpt_dex = allstat;
		ch->m_statpt_int = allstat;
		ch->m_statpt_con = allstat;

		ch->InitStat();
		ch->CalcStatus(true);

		if( arg && (*arg) )
		{
			arg = AnyOneArg(arg, tmpBuf);
			int hpmp = atoi(tmpBuf);

			if( hpmp > 0)
			{
				ch->m_hp = ch->m_maxHP * hpmp / 100;
				ch->m_mp = ch->m_maxMP * hpmp / 100;
			}
		}
	}
	else
	{
		int statpt = atoi(tmpBuf);
		if (statpt < 0)
			return ;
		ch->m_statpt_remain = statpt;

		{
			CNetMsg::SP rmsg(new CNetMsg);
			StatPointRemainMsg(rmsg, ch);
			SEND_Q(rmsg, ch->m_desc);
		}

		if (!arg || !(*arg))		return;
		arg = AnyOneArg(arg, tmpBuf);
		int strpt = atoi(tmpBuf);
		if (!arg || !(*arg))		return;
		arg = AnyOneArg(arg, tmpBuf);
		int dexpt = atoi(tmpBuf);
		if (!arg || !(*arg))		return;
		arg = AnyOneArg(arg, tmpBuf);
		int intpt = atoi(tmpBuf);
		if (!arg || !(*arg))		return;
		arg = AnyOneArg(arg, tmpBuf);
		int conpt = atoi(tmpBuf);
		if (strpt < 0 || dexpt < 0 || intpt < 0 || conpt < 0)
			return ;
		ch->m_statpt_str = strpt;
		ch->m_statpt_dex = dexpt;
		ch->m_statpt_int = intpt;
		ch->m_statpt_con = conpt;

		ch->InitStat();
		ch->CalcStatus(true);
	}
}

void do_GMSkillPoint(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	if (!arg || !(*arg))		return;

	LONGLONG skillpt = ATOLL(arg);
	LONGLONG _basic = 10000;

	ch->m_skillPoint = skillpt * _basic;
	ch->m_bChangeStatus = true;
}

void do_GMSetRegenSec(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	if (!arg || !(*arg))		return ;

	char tmpBuf[MAX_MESSAGE_SIZE] = {0,};
	arg = AnyOneArg(arg, tmpBuf);
	int index = atoi(tmpBuf);

	arg = AnyOneArg(arg, tmpBuf);
	int regenSec = atoi(tmpBuf);

	if (index <= 0 || regenSec <= 0)
		return;

	int i;
	for (i=0; i < ch->m_pArea->m_npcRegenList.m_nCount; i++)
	{
		if (ch->m_pArea->m_npcRegenList.m_infoList[i].m_npcIdx == index)
			ch->m_pArea->m_npcRegenList.m_infoList[i].m_regenSec = regenSec;
	}
}

void do_GMNPCRegen(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	if (!arg || !(*arg))
		return;

	char tmpBuf[MAX_MESSAGE_SIZE] = {0,};
	arg = AnyOneArg(arg, tmpBuf);
	int index = atoi(tmpBuf);

	if (index <= 0)
		return;

	int i;
	for (i=0; i < ch->m_pArea->m_npcRegenList.m_nCount; i++)
	{
		// 해당 npc이고 bAlive가 false이면
		if (ch->m_pArea->m_npcRegenList.m_infoList[i].m_npcIdx == index &&
				!ch->m_pArea->m_npcRegenList.m_infoList[i].m_bAlive)
			// 바로 리젠
			ch->m_pArea->m_npcRegenList.m_infoList[i].m_regenSec = 0;
	}
}

void do_GMKillNPC(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	if (!arg || !(*arg))
		return;

	char tmpBuf[MAX_MESSAGE_SIZE] = {0,};
	arg = AnyOneArg(arg, tmpBuf);
	int distance = atoi(tmpBuf);

	if (distance <= 0)
		return;

	// Cell 2개 정도에서
	int sx = ch->m_cellX - 2;
	int ex = ch->m_cellX + 2;
	int sz = ch->m_cellZ - 2;
	int ez = ch->m_cellZ + 2;

	int x, z;

	for (x = sx; x <= ex; x++)
	{
		if (x < 0 || x >= ch->m_pArea->m_size[0])
			continue;

		for (z = sz; z <= ez; z++)
		{
			if (z < 0 || z >= ch->m_pArea->m_size[1])
				continue;

			CCharacter* tch;;
			CCharacter* tchNext = ch->m_pArea->m_cell[x][z].m_listChar;
			while ((tch = tchNext))
			{
				tchNext = tchNext->m_pCellNext;

				// npc이고
				if (IS_NPC(tch))
				{
					CNPC* tnpc = TO_NPC(tch);

					if( tnpc->GetOwner() )
						continue;

					if( tnpc->m_proto->m_index == 491 ||				// 바닥데미지 삭제 불가
							( tnpc->m_flag & NPC_WARCASTLE ) )				// 공성 NPC 삭제 불가
						continue;

#ifdef EXTREME_CUBE
					switch(tnpc->m_proto->m_index)
					{
					case 526:
					case 527:
					case 528:
					case 529:
					case 530:
						continue;
						break;
					}
#endif // EXTREME_CUBE

					// 웜의 둥지
					if(tnpc->m_proto->m_index == 1152)
						continue;

					// npc와 해당거리내에 있으면
					if (GetDistance(ch, tnpc) < distance)
					{
						{
							// 결과 메시지 보냄
							CNetMsg::SP rmsg(new CNetMsg);
							DamageMsg(rmsg, ch, tnpc, MSG_DAMAGE_MELEE, -1, 1, HITTYPE_NORMAL, -1);
							tnpc->m_pArea->SendToCell(rmsg, tnpc, false);
						}

						tnpc->m_hp = 0;

						if (DEAD(tnpc))
						{
#ifdef EXTREME_CUBE
							if(tnpc->m_bCubeRegen)
							{
								CCubeSpace* cube = gserver->m_extremeCube.GetExtremeCube(tnpc->m_pArea->m_index);
								if(cube)
								{
									cube->DelMob(tnpc);
								}
							}
#endif // EXTREME_CUBE
							tnpc->SendDisappearAllInCell(true);

							// 어택리스트에서 삭제
							DelAttackList(tnpc);

							// npc 삭제
							ch->m_pArea->CharFromCell(tnpc, true);
							ch->m_pArea->DelNPC(tnpc);
						}
					}
				}
			}
		}
	}
}

void do_GMExpUp(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	if (!arg || !*arg)
		return ;

	double exppercent = atof(arg);
	if (exppercent < 0 || exppercent > 100)
		return ;
	LONGLONG exp = GetLevelupExp(ch->m_level);
	exp = (LONGLONG)(exp * exppercent / 100);
	ch->m_exp = exp;
	ch->m_bChangeStatus = true;
}

void do_GMBloodPoint(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	char tmpBuf[MAX_MESSAGE_SIZE] = {0,};
	if (arg && *arg)
	{
		int bloodPoint = atoi(arg);

		if (bloodPoint < 0 || bloodPoint > PC_BLOOD_POINT)
			return;

		ch->m_bloodPoint = bloodPoint;

		strcpy(tmpBuf, "Blood points: ");
		IntCat(tmpBuf, ch->m_bloodPoint);

		{
			CNetMsg::SP rmsg(new CNetMsg);
			SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", tmpBuf);
			SEND_Q(rmsg, ch->m_desc);
		}

		if (ch->m_bloodPoint <= 0)
		{
			ch->m_bloodPoint = PC_BLOOD_POINT;
			ch->SettingItemOption();
		}

		return;
	}

	strcpy(tmpBuf, "Blood points: ");
	IntCat(tmpBuf, ch->m_bloodPoint);

	{
		CNetMsg::SP rmsg(new CNetMsg);
		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", tmpBuf);
		SEND_Q(rmsg, ch->m_desc);
	}
}

//XX endgame - 1
void do_GMEndGame(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	if (EndGameTimer::instance()->isRun())
		return;

	if (!arg || !(*arg))
		return ;

	char tmpBuf[MAX_MESSAGE_SIZE] = {0,};
	arg = AnyOneArg(arg, tmpBuf);
	int sec = atoi(tmpBuf);
	CLCString var = arg;
	var.Trim();

	if (var.Length() > 0)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		MsgrNoticeMsg(rmsg, -1, -1, -1, -1, arg);
		SEND_Q(rmsg, gserver->m_messenger);
	}

	EndGameTimer::instance()->Init(sec);
}

void do_GMObserve(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	char tmpBuf[MAX_MESSAGE_SIZE] = {0,};
	if (gserver->m_observer)
	{
		if (gserver->m_observer == ch->m_desc)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Observe Reset");
			SEND_Q(rmsg, ch->m_desc);
			gserver->m_observer = NULL;
			return ;
		}

		strcpy(tmpBuf, "Change Observer: ");

		if (gserver->m_observer->m_pChar)
		{
			strcat(tmpBuf, gserver->m_observer->m_pChar->GetName());
		}
		else
		{
			strcat(tmpBuf, gserver->m_observer->m_idname);
		}

		strcat(tmpBuf, " -> ");
		strcat(tmpBuf, ch->GetName());

		{
			CNetMsg::SP rmsg(new CNetMsg);
			SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", tmpBuf);
			SEND_Q(rmsg, gserver->m_observer);
			SEND_Q(rmsg, ch->m_desc);
		}
	}

	gserver->m_observer = ch->m_desc;
}

void do_GMChatMonitor(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	int i;
	// 있으면 해제
	for (i = 0; i < 20; i++)
	{
		if (gserver->m_chatmonitor[i] == ch->m_desc)
		{
			if (!arg || !*arg)
			{
				gserver->m_chatmonitor[i] = NULL;

				CNetMsg::SP rmsg(new CNetMsg);
				SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Monitoring Finished!!!");
				SEND_Q(rmsg, ch->m_desc);
				return ;
			}
			break;
		}
	}

	// 없으면 추가
	if (i == 20)
	{
		for (i = 0; i < 20; i++)
		{
			if (gserver->m_chatmonitor[i] == NULL)
			{
				gserver->m_chatmonitor[i] = ch->m_desc;
				break;
			}
		}
	}

	// 20명까지 가능
	if (i == 20)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Cannot More Monitor!!!");
		SEND_Q(rmsg, ch->m_desc);
	}

	// 대상 캐릭터 찾기
	if (!arg || !*arg)
		return ;

	char tmpBuf[MAX_MESSAGE_SIZE] = {0,};
	arg = AnyOneArg(arg, tmpBuf);
	CLCString name = tmpBuf;
	name.Trim();
	if (strlen(tmpBuf) < 1)
		return ;

	CPC* pc = PCManager::instance()->getPlayerByName(name, false );
	if (pc == NULL)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Not Found the PC");
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	*tmpBuf = '\0';
	arg = AnyOneArg(arg, tmpBuf, true);
	TrimString(tmpBuf);
	pc->m_bChatMonitor = true;
	if (strcmp(tmpBuf, "off") == 0)
		pc->m_bChatMonitor = false;
	else
		pc->m_bChatMonitor = true;

	{
		strcpy(tmpBuf, "SET CHAT MONITOR: ");
		strcat(tmpBuf, pc->m_nick);
		CNetMsg::SP rmsg(new CNetMsg);
		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", tmpBuf);
		SEND_Q(rmsg, ch->m_desc);
	}
}

void do_GMDoubleEvent(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
#ifdef EVENT_DOUBLE
	int target = -1;			// 대상 서버 군 번호: -1: all, 1 ~ n: server group
	int	zonenum;			// 대상 존 번호: -1: all, 0 ~ : zonenum
	int subno = -1;
	zonenum = -1;
	char tmpBuf[MAX_MESSAGE_SIZE] = {0,};

	// 대상 서버군 정하기
	if (!arg || !(*arg))
		goto INVALID_FORMAT;

	arg = AnyOneArg(arg, tmpBuf, true);
	TrimString(tmpBuf);
	if (strcmp(tmpBuf, "all") == 0)
		target = -1;
	else if (strcmp(tmpBuf, "cur") == 0)
		target = gserver->m_serverno;
	else
	{
		// all, cur 모두 아니면 숫자여야함
		char* p = tmpBuf;
		while (*p)
		{
			if (!isdigit(*p))
				goto INVALID_FORMAT;
			p++;
		}
		target = atoi(tmpBuf);
		if (target < 1)
			goto INVALID_FORMAT;
	}

	// 채널 읽기
	if (!arg || !(*arg))
		goto INVALID_FORMAT;

	arg = AnyOneArg(arg, tmpBuf, true);
	TrimString(tmpBuf);

	if (strcmp(tmpBuf, "all") == 0)
		subno = -1;
	else if (strcmp(tmpBuf, "cur") == 0)
		subno = gserver->m_subno;
	else
	{
		// all, cur 모두 아니면 숫자여야함
		char* p = tmpBuf;
		while (*p)
		{
			if (!isdigit(*p))
				goto INVALID_FORMAT;
			p++;
		}
		subno = atoi(tmpBuf);
		if (subno < 1)
			goto INVALID_FORMAT;
	}

	// 명령 정하기
	if (!arg || !(*arg))
		goto INVALID_FORMAT;
	arg = AnyOneArg(arg, tmpBuf, true);
	TrimString(tmpBuf);

	int percent[6];
	int start_dt[6];	//dt is datetime
	int end_dt[6];		//dt is datetime

	memset(percent, -1, sizeof(percent) );
	memset(start_dt, -1, sizeof(start_dt));
	memset(end_dt, -1, sizeof(end_dt));

	if (strcmp(tmpBuf, "start") == 0)
	{
#ifdef NEW_DOUBLE_GM_ZONE
		if (!arg || !(*arg))
			goto INVALID_FORMAT;
		arg = AnyOneArg(arg, tmpBuf, true);
		TrimString(tmpBuf);
		if (strcmp(tmpBuf, "all") == 0)
			zonenum = -1;
		else
		{
			// all 아니면 숫자여야함
			char* p = tmpBuf;
			while (*p)
			{
				if (!isdigit(*p))
					goto INVALID_FORMAT;
				p++;
			}
			zonenum = atoi(tmpBuf);
			if ( zonenum < 0 )
				goto INVALID_FORMAT;
		}
#endif // NEW_DOUBLE_GM_ZONE

		for( int i = 0; i < 6; i ++)
		{
			arg = AnyOneArg(arg, tmpBuf, true);
			TrimString(tmpBuf);

			percent[i] = atoi(tmpBuf);

			if( i == 5 && (percent[i] < 0 || percent[i] > 10) )
				goto INVALID_FORMAT;
#ifdef TLD_DOUBLE_EVENT
			if( percent[i] < 0 || percent[i] > TLD_DOUBLE_EVENT )
				goto INVALID_FORMAT;
#else
			if( percent[i] < 0 || percent[i] > 600)
				goto INVALID_FORMAT;
#endif //TLD_DOUBLE_EVENT
		}
#ifdef NEW_DOUBLE_GM_AUTO

		if(ParsingTime(arg, start_dt, end_dt) == false)
			goto INVALID_FORMAT;

#endif // NEW_DOUBLE_GM_AUTO
		{
			CNetMsg::SP rmsg(new CNetMsg);
#ifdef NEW_DOUBLE_GM_AUTO
			MsgrDoubleEventReqMsg(rmsg, 0, target, subno, zonenum, ch->m_index, 0, percent, start_dt, end_dt);
#else
			MsgrDoubleEventReqMsg(rmsg, 0, target, subno, zonenum, ch->m_index, 0, percent);
#endif
			SEND_Q(rmsg, gserver->m_messenger);
		}
	}
	else if (strcmp(tmpBuf, "ing") == 0)
	{
		CNetMsg::SP rmsg(new CNetMsg);
#ifdef NEW_DOUBLE_GM_AUTO
		MsgrDoubleEventReqMsg(rmsg, 0, target, subno, zonenum, ch->m_index, 1, percent, start_dt, end_dt);
#else
		MsgrDoubleEventReqMsg(rmsg, 0, target, subno, zonenum, ch->m_index, 1, percent);
#endif
		SEND_Q(rmsg, gserver->m_messenger);
	}
	else if (strcmp(tmpBuf, "stop") == 0)
	{
		CNetMsg::SP rmsg(new CNetMsg);
#ifdef NEW_DOUBLE_GM_AUTO
		MsgrDoubleEventReqMsg(rmsg, 0, target, subno, zonenum, ch->m_index, 2, percent, start_dt, end_dt);
#else
		MsgrDoubleEventReqMsg(rmsg, 0, target, subno, zonenum, ch->m_index, 2, percent);
#endif
		SEND_Q(rmsg, gserver->m_messenger);
	}
	else
		goto INVALID_FORMAT;

	return;

INVALID_FORMAT:
#ifdef NEW_DOUBLE_GM_ZONE
	{
		CNetMsg::SP rmsg(new CNetMsg);
		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Invalid Command");
		SEND_Q(rmsg, ch->m_desc);
	}
	{
		CNetMsg::SP rmsg(new CNetMsg);
#ifdef NEW_DOUBLE_GM_AUTO
		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Usage: double_event <server-group no | all | cur> <channel no | all | cur> <start | ing | stop> <zonenum no | all> <nasdrop> <nasget> <exp> <sp> <produce> <producenum> <start datetime> <end datetime>");
#else
		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Usage: double_event <server-group no | all | cur> <channel no | all | cur> <start | ing | stop> <zonenum no | all> <nasdrop> <nasget> <exp> <sp> <produce> <producenum>");
#endif // NEW_DOUBLE_GM_AUTO
		SEND_Q(rmsg, ch->m_desc);
	}
#else
	{
		CNetMsg::SP rmsg(new CNetMsg);
		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Invalid Command");
		SEND_Q(rmsg, ch->m_desc);
	}
	{
		CNetMsg::SP rmsg(new CNetMsg);
#ifdef NEW_DOUBLE_GM_AUTO
		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Usage: double_event <server-group no | all | cur> <channel no | all | cur> <start | ing | stop> <nasdrop> <nasget> <exp> <sp> <produce> <producenum> <start datetime> <end datetime>");
#else
		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Usage: double_event <server-group no | all | cur> <channel no | all | cur> <start | ing | stop> <nasdrop> <nasget> <exp> <sp> <produce> <producenum> ");
#endif
		SEND_Q(rmsg, ch->m_desc);
	}
#endif // NEW_DOUBLE_GM_ZONE
#endif // EVENT_DOUBLE
}

void do_GMDoubleExpEvent(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	int target = -1;			// 대상 서버 군 번호: -1: all, 1 ~ n: server group
	char tmpBuf[MAX_MESSAGE_SIZE] = {0,};

	// 대상 서버군 정하기
	if (!arg || !(*arg))
		goto INVALID_FORMAT;

	arg = AnyOneArg(arg, tmpBuf, true);
	TrimString(tmpBuf);
	if (strcmp(tmpBuf, "all") == 0)
		target = -1;
	else if (strcmp(tmpBuf, "cur") == 0)
		target = gserver->m_serverno;
	else
	{
		// all, cur 모두 아니면 숫자여야함
		char* p = tmpBuf;
		while (*p)
		{
			if (!isdigit(*p))
				goto INVALID_FORMAT;
			p++;
		}
		target = atoi(tmpBuf);
		if (target < 1)
			goto INVALID_FORMAT;
	}

	// 명령 정하기
	if (!arg || !(*arg))
		goto INVALID_FORMAT;

	{
		CNetMsg::SP rmsg(new CNetMsg);
		arg = AnyOneArg(arg, tmpBuf, true);
		TrimString(tmpBuf);
		if (strcmp(tmpBuf, "start") == 0)
		{
			arg = AnyOneArg(arg, tmpBuf, true);
			TrimString(tmpBuf);

			if( atoi(tmpBuf) < 0 || atoi(tmpBuf) > 200)
				goto INVALID_FORMAT;

			MsgrDoubleExpEventReqMsg(rmsg, 0, target, -1, -1, ch->m_index, 0, atoi(tmpBuf));
		}
		else if (strcmp(tmpBuf, "ing") == 0)
			MsgrDoubleExpEventReqMsg(rmsg, 0, target, -1, -1, ch->m_index, 1, -1);
		else if (strcmp(tmpBuf, "stop") == 0)
			MsgrDoubleExpEventReqMsg(rmsg, 0, target, -1, -1, ch->m_index, 2, -1);
		else
			goto INVALID_FORMAT;

		if (gserver->isRunMessenger())
		{
			SEND_Q(rmsg, gserver->m_messenger);
		}
		else
		{
			CNetMsg::SP rmsg(new CNetMsg);
			SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Messenger Not Running");
			SEND_Q(rmsg, ch->m_desc);
		}
	}
	return ;

INVALID_FORMAT:
	{
		CNetMsg::SP rmsg(new CNetMsg);
		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Invalid Command");
		SEND_Q(rmsg, ch->m_desc);
	}
	{
		CNetMsg::SP rmsg(new CNetMsg);
		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Usage: doubleexp_event <server-group no | all | cur> <start | ing | stop>");
		SEND_Q(rmsg, ch->m_desc);
	}
}

void do_GMLattoEvent(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
}

void do_GMNewYearEvent(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
}

void do_GMValentineEvent(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
#ifdef EVENT_VALENTINE

	if (!arg || !(*arg))		return ;

	char tmpBuf[MAX_MESSAGE_SIZE] = {0,};

	arg = AnyOneArg(arg, tmpBuf);

	TrimString(tmpBuf);

	if (strcmp(tmpBuf, "start") == 0)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		MsgrValentineEventReqMsg(rmsg, -1, -1, -1, -1, true, gserver->m_serverno, gserver->m_subno, ch->m_index);
		SEND_Q(rmsg, gserver->m_messenger);
	}
	else if (strcmp(tmpBuf, "stop") == 0)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		MsgrValentineEventReqMsg(rmsg, -1, -1, -1, -1, false, gserver->m_serverno, gserver->m_subno, ch->m_index);
		SEND_Q(rmsg, gserver->m_messenger);
	}
	else if (strcmp(tmpBuf, "ing") == 0)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		if (gserver->m_bValentineEvent)
			SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "This Server Valentine Event Ing..");
		else
			SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "This Server Valentine Event Stop..");

		SEND_Q(rmsg, ch->m_desc);
		return;
	}

#endif
}

void do_GMWhiteDayEvent(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
#ifdef EVENT_WHITEDAY

	if (!arg || !(*arg))		return ;

	char tmpBuf[MAX_MESSAGE_SIZE] = {0,};
	arg = AnyOneArg(arg, tmpBuf);

	TrimString(tmpBuf);

	if (strcmp(tmpBuf, "start") == 0)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		MsgrWhiteDayEventReqMsg(rmsg, -1, -1, -1, -1, true, gserver->m_serverno, gserver->m_subno, ch->m_index);
		SEND_Q(rmsg, gserver->m_messenger);
	}
	else if (strcmp(tmpBuf, "stop") == 0)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		MsgrWhiteDayEventReqMsg(rmsg, -1, -1, -1, -1, false, gserver->m_serverno, gserver->m_subno, ch->m_index);
		SEND_Q(rmsg, gserver->m_messenger);
	}
	else if (strcmp(tmpBuf, "ing") == 0)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		if (gserver->m_bWhiteDayEvent)
			SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "This Server WhiteDay Event Ing..");
		else
			SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "This Server WhiteDay Event Stop..");

		SEND_Q(rmsg, ch->m_desc);
		return;
	}
#endif
}

void do_GMReloadGMIP(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
}

void do_GMSpeedUp(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	if (!arg || !(*arg))		return ;

	char tmpBuf[MAX_MESSAGE_SIZE] = {0,};
	arg = AnyOneArg(arg, tmpBuf);
	int spd = atoi(tmpBuf);
	if (spd < 1)
	{
		// 원래 속도로
		ch->m_speedup = 0;
	}
	else
	{
		// 입력 속도로
		ch->m_speedup = spd;
	}
	ch->CalcStatus(true);
}

void do_GMFameUp(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	if (!arg || !(*arg))		return ;

	char tmpBuf[MAX_MESSAGE_SIZE] = {0,};
	arg = AnyOneArg(arg, tmpBuf);
	int fame = atoi(tmpBuf);

	if (fame < 0)
		fame = 0;

	ch->m_fame = fame;
	ch->CalcStatus(true);
}

void do_GMPD4(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	if (!arg || !(*arg))		return ;

	char tmpBuf[MAX_MESSAGE_SIZE] = {0,};
	arg = AnyOneArg(arg, tmpBuf);

	// BW
	if (strcmp(tmpBuf, "rankinit") == 0)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		rmsg->Init(MSG_HELPER_COMMAND);
		RefMsg(rmsg) << MSG_HELPER_PD4_RANK_INIT;
		SEND_Q(rmsg, gserver->m_helper);
	}
	else if (strcmp(tmpBuf, "rewardinit") == 0)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		rmsg->Init(MSG_HELPER_COMMAND);
		RefMsg(rmsg) << MSG_HELPER_PD4_REWARD_INIT;
		SEND_Q(rmsg, gserver->m_helper);
	}
	else if (strcmp(tmpBuf, "rewardend") == 0)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		rmsg->Init(MSG_HELPER_COMMAND);
		RefMsg(rmsg) << MSG_HELPER_PD4_REWARD_END;
		SEND_Q(rmsg, gserver->m_helper);
	}
}

void do_GMKickID(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	char tmpBuf[MAX_MESSAGE_SIZE] = {0,};
#ifdef GMTOOL
	if(ch->m_desc->m_userFlag & IS_GM_CHAR)
	{
		if (!arg || !(*arg))
			return ;

		int serverno, subno;
		char id[100];
		char* p1;

		arg = AnyOneArg(arg, tmpBuf);
		TrimString(tmpBuf);

		if (strlen(tmpBuf) < 1)
			return ;

		if (strcmp(tmpBuf, ch->m_desc->m_idname) == 0)
			return ;

		strcpy(id, tmpBuf);

		if (!arg || !(*arg))
			return ;

		arg = AnyOneArg(arg, tmpBuf);
		TrimString(tmpBuf);
		p1 = tmpBuf;
		while (*p1)
		{
			if (!isdigit(*p1))
				return ;		//goto INVALID_FORMAT;
			p1++;
		}
		serverno = atoi(tmpBuf);

		if (!arg || !(*arg))
			return ;

		arg = AnyOneArg(arg, tmpBuf);
		TrimString(tmpBuf);
		p1 = tmpBuf;
		while (*p1)
		{
			if (!isdigit(*p1))
				return ;		//goto INVALID_FORMAT;
			p1++;
		}
		subno = atoi(tmpBuf);

		{
			CNetMsg::SP rmsg(new CNetMsg);
			MsgGmKickIDCommandReqMsg(rmsg, 0, serverno, subno, -1, 1, 1, ch->m_index, id);
			SEND_Q(rmsg, gserver->m_messenger);
		}

		{
			char buf2[MAX_MESSAGE_SIZE] = {0,};
			// 보냈다는 메시지
			sprintf(buf2, "Sending Kick ID: %s", id);
			CNetMsg::SP rmsg(new CNetMsg);
			SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", buf2);
			SEND_Q(rmsg, ch->m_desc);
		}

		return ;
	}
#endif // GMTOOL

	if (vec.size() != 1)
		return;

	if ((const char*)ch->m_desc->m_idname == vec[0])
	{
		CNetMsg::SP rmsg(new CNetMsg);
		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "can't kick myself");
		SEND_Q(rmsg, ch->m_desc);

		return;
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		ServerToServerPacket::makeKickUserByUserId(rmsg, ch->m_index, vec[0].c_str());
		SEND_Q(rmsg, gserver->m_helper);
	}
}

void do_GMResurrection(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	if(!arg || !(*arg) )
		return;
	char tmpBuf[MAX_MESSAGE_SIZE] = {0,};
	arg = AnyOneArg(arg, tmpBuf);
	CLCString name = tmpBuf;
	name.Trim();
	CPC* tpc = PCManager::instance()->getPlayerByName(name, false);

	if( !tpc )
		return;

	CSkill* skill = new CSkill(gserver->m_skillProtoList.Find(50), 5);

	bool bApply;
	ApplySkill(ch, tpc, skill, -1, bApply);

	delete skill;
}

void do_GMRecommend(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	char tmpBuf[MAX_MESSAGE_SIZE] = {0,};
	arg = AnyOneArg(arg, tmpBuf);
	int nServerNo = atoi(tmpBuf);
	if (nServerNo < 1 && nServerNo != -1)
		return ;

	{
		CNetMsg::SP rmsg(new CNetMsg);
		MsgrRecommendMsg(rmsg, ch->m_index, nServerNo);
		SEND_Q(rmsg, gserver->m_messenger);
	}
}

void do_GMSetLimit(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	char tmpBuf[MAX_MESSAGE_SIZE] = {0,};
	char buf2[MAX_MESSAGE_SIZE] = {0,};
	arg = AnyOneArg(arg, tmpBuf);
	int ctid = atoi(tmpBuf);
	arg = AnyOneArg(arg, tmpBuf);
	int toSell = atoi(tmpBuf);

	CCatalog* catal;
	catal = gserver->m_catalogList.Find(ctid);
	if( !catal )
	{
		sprintf(buf2, "No Catalog Index: %d", ctid);
		CNetMsg::SP rmsg(new CNetMsg);
		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", buf2);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}
	if( catal->GetToSell() + toSell < 0 || catal->GetLimit() < toSell || catal->GetToSell() + toSell > catal->GetLimit() )
	{
		sprintf(buf2, "Not Currect to sell, ctid : %d, number : %d, remain : %d, limit : %d", ctid, toSell, catal->GetToSell(), catal->GetLimit() );
		CNetMsg::SP rmsg(new CNetMsg);
		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", buf2);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}
	// 메세지 보낸다.

	if( gserver->isRunConnector())
	{
		{
			CNetMsg::SP rmsg(new CNetMsg);
			rmsg->Init(MSG_CONN_REQ);
			RefMsg(rmsg) << (unsigned char) MSG_CONN_LIMIT_CATALOG
						 << ctid
						 << toSell;

			SEND_Q(rmsg, gserver->m_connector);
		}

		{
			CNetMsg::SP rmsg(new CNetMsg);
			sprintf(buf2, "Now Catalog %s, Additional %d products will be sold from now on.(remain : %d, limit : %d) ", catal->GetName(), toSell, catal->GetToSell(), catal->GetLimit() );
			SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", buf2);
			SEND_Q(rmsg, ch->m_desc);
		}
	}
	else
	{
		CNetMsg::SP rmsg(new CNetMsg);
		sprintf(buf2, "Connector Server Error");
		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", buf2);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}
}

void do_GMDisguise(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	if( ch->m_changeIndex == 301 )
	{
		ch->m_changeIndex = -1;
		ch->m_changePulse = 0;

		ch->ResetPlayerState(PLAYER_STATE_CHANGE);

		{
			CNetMsg::SP rmsg(new CNetMsg);
			ChangeStopMsg(rmsg, ch);
			ch->m_pArea->SendToCell(rmsg, ch, true);
		}
	}
	else
	{
		ch->m_changeIndex = -1;
		ch->m_changePulse = 0;

		ch->ResetPlayerState(PLAYER_STATE_CHANGE);

		{
			CNetMsg::SP rmsg(new CNetMsg);
			ChangeStopMsg(rmsg, ch);
			ch->m_pArea->SendToCell(rmsg, ch, true);
		}

		ch->m_changeIndex = 301;
		ch->m_changePulse = INT_MAX;

		ch->SetPlayerState(PLAYER_STATE_CHANGE);

		{
			CNetMsg::SP rmsg(new CNetMsg);
			ChangeStartMsg(rmsg, ch);
			ch->m_pArea->SendToCell(rmsg, ch, true);
		}
	}
}

void do_GMLoginServer(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	char tmpBuf[MAX_MESSAGE_SIZE] = {0,};

	arg = AnyOneArg(arg, tmpBuf);
	if (strcmp2(tmpBuf, "open") == 0)			// 1
	{
		{
			CNetMsg::SP rmsg(new CNetMsg);
			MsgrLoginServerMsg(rmsg, 1);
			SEND_Q(rmsg, gserver->m_messenger);
		}

		{
			CNetMsg::SP rmsg(new CNetMsg);
			SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "send open msg to login server");
			SEND_Q(rmsg, ch->m_desc);
		}
	}
	else if (strcmp2(tmpBuf, "close") == 0)		// 0
	{
		{
			CNetMsg::SP rmsg(new CNetMsg);
			MsgrLoginServerMsg(rmsg, 0);
			SEND_Q(rmsg, gserver->m_messenger);
		}

		{
			CNetMsg::SP rmsg(new CNetMsg);
			SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "send close msg to login server");
			SEND_Q(rmsg, ch->m_desc);
		}
	}
	else if (strcmp2(tmpBuf, "reboot") == 0)		// 2
	{
		{
			CNetMsg::SP rmsg(new CNetMsg);
			MsgrLoginServerMsg(rmsg, 2);
			SEND_Q(rmsg, gserver->m_messenger);
		}

		{
			CNetMsg::SP rmsg(new CNetMsg);
			SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "send reboot msg to login server");
			SEND_Q(rmsg, ch->m_desc);
		}
	}
}

void do_GMEventGoldenBall_End(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	// 이벤트 종료		: /event gball end
	CNetMsg::SP rmsg(new CNetMsg);
	MsgrEventGoldenballEndMsg(rmsg, ch->m_index);
	SEND_Q(rmsg, gserver->m_messenger);
}

void do_GMEventGoldenBall_Status(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	// 현재 상태 보기	: /event gball status
	//					: 응모중 - A/B팀 이름과 종료일시 알려줌
	//					: 보상중 - A/B팀 이름/점수와 종료일시 알려줌

	CLCString strStatus(1000);
	switch (gserver->m_clGoldenBall.GetStatus())
	{
	case GOLDENBALL_STATUS_VOTE:
		strStatus.Format("GOLDEN BALL VOTE : [%s] vs [%s] : %04d-%02d-%02d %02d:%02d"
						 , gserver->m_clGoldenBall.GetTeam1Name()
						 , gserver->m_clGoldenBall.GetTeam2Name()
						 , gserver->m_clGoldenBall.GetEndYear()
						 , gserver->m_clGoldenBall.GetEndMonth()
						 , gserver->m_clGoldenBall.GetEndDay()
						 , gserver->m_clGoldenBall.GetEndHour()
						 , gserver->m_clGoldenBall.GetEndMinute()
						);
		break;

	case GOLDENBALL_STATUS_GIFT:
		strStatus.Format("GOLDEN BALL GIFT : [%s][%d] vs [%s][%d] : %04d-%02d-%02d %02d:%02d"
						 , gserver->m_clGoldenBall.GetTeam1Name()
						 , gserver->m_clGoldenBall.GetTeam1Score()
						 , gserver->m_clGoldenBall.GetTeam2Name()
						 , gserver->m_clGoldenBall.GetTeam2Score()
						 , gserver->m_clGoldenBall.GetEndYear()
						 , gserver->m_clGoldenBall.GetEndMonth()
						 , gserver->m_clGoldenBall.GetEndDay()
						 , gserver->m_clGoldenBall.GetEndHour()
						 , gserver->m_clGoldenBall.GetEndMinute()
						);
		break;

	case GOLDENBALL_STATUS_VOTE_END:
		strStatus.Format("GOLDEN BALL VOTE END : [%s] vs [%s] : %04d-%02d-%02d %02d:%02d"
						 , gserver->m_clGoldenBall.GetTeam1Name()
						 , gserver->m_clGoldenBall.GetTeam2Name()
						 , gserver->m_clGoldenBall.GetEndYear()
						 , gserver->m_clGoldenBall.GetEndMonth()
						 , gserver->m_clGoldenBall.GetEndDay()
						 , gserver->m_clGoldenBall.GetEndHour()
						 , gserver->m_clGoldenBall.GetEndMinute()
						);
		break;

	default:
		strStatus = "GOLDEN BALL NOTHING";
		break;
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", strStatus);
		SEND_Q(rmsg, ch->m_desc);
	}
}

void do_GMEventGoldenBall_Gift(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	// 보상 시작하기	: /event gball gift <team1 score> <team2 score> <year> <month> <day> <hour> <min>
	//					: 현재부터 보상 시작하여 지정된 일시에 보상 종료

	// 1. 현재 상태 검사 : GOLDENBALL_STATUS_VOTE_END
	// 2. 명령어에서 각 데이터 파싱
	// 3. 현재 시간과 입력 시간 비교
	// 4. 메신저로 전달

	// 1. 현재 상태 검사 : GOLDENBALL_STATUS_VOTE_END OR GOLDENBALL_STATUS_GIFT
	if (gserver->m_clGoldenBall.GetStatus() != GOLDENBALL_STATUS_VOTE_END && gserver->m_clGoldenBall.GetStatus() != GOLDENBALL_STATUS_GIFT)
		return ;

	// 2. 명령어에서 각 데이터 파싱
	int			nTeam1Score;
	int			nTeam2Score;
	int			nYear;
	int			nMonth;
	int			nDay;
	int			nHour;
	int			nMin;

	char tmpBuf[MAX_MESSAGE_SIZE] = {0,};
	tmpBuf[0] = '\0';
	arg = AnyOneArg(arg, tmpBuf);
	if (*tmpBuf == '\0') return ;
	nTeam1Score	= atoi(tmpBuf);
	tmpBuf[0] = '\0';
	arg = AnyOneArg(arg, tmpBuf);
	if (*tmpBuf == '\0') return ;
	nTeam2Score	= atoi(tmpBuf);
	tmpBuf[0] = '\0';
	arg = AnyOneArg(arg, tmpBuf);
	if (*tmpBuf == '\0') return ;
	nYear		= atoi(tmpBuf);
	tmpBuf[0] = '\0';
	arg = AnyOneArg(arg, tmpBuf);
	if (*tmpBuf == '\0') return ;
	nMonth		= atoi(tmpBuf);
	tmpBuf[0] = '\0';
	arg = AnyOneArg(arg, tmpBuf);
	if (*tmpBuf == '\0') return ;
	nDay		= atoi(tmpBuf);
	tmpBuf[0] = '\0';
	arg = AnyOneArg(arg, tmpBuf);
	if (*tmpBuf == '\0') return ;
	nHour		= atoi(tmpBuf);
	tmpBuf[0] = '\0';
	arg = AnyOneArg(arg, tmpBuf);
	if (*tmpBuf == '\0') return ;
	nMin		= atoi(tmpBuf);

	// 3. 현재 시간과 입력 시간 비교
	time_t tCurrent;
	time(&tCurrent);
	struct tm tmEndGift;
	memset(&tmEndGift, 0, sizeof(tmEndGift));
	tmEndGift.tm_year	= nYear - 1900;
	tmEndGift.tm_mon	= nMonth - 1;
	tmEndGift.tm_mday	= nDay;
	tmEndGift.tm_hour	= nHour;
	tmEndGift.tm_min	= nMin;
	tmEndGift.tm_isdst	= -1;
	time_t tEndGift = mktime(&tmEndGift);
	if (tEndGift <= tCurrent)
		return ;

	// 4. 메신저로 전달
	{
		CNetMsg::SP rmsg(new CNetMsg);
		MsgrEventGoldenballGiftMsg(rmsg, ch->m_index, gserver->m_clGoldenBall.GetTeam1Name(), nTeam1Score, gserver->m_clGoldenBall.GetTeam2Name(), nTeam2Score, nYear, nMonth, nDay, nHour, nMin, gserver->m_clGoldenBall.GetVoteEndTime());
		SEND_Q(rmsg, gserver->m_messenger);
	}
}

void do_GMEventGoldenBall_Vote(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	// 응모 시작하기	: /event gball vote <team1> <team2> <year> <month> <day> <hour> <min>
	//					: 현재부터 응모 시작하여 지정된 일시에 응모 종료

	// 1. 현재 상태 검사 : GOLDENBALL_STATUS_NOTHING
	// 2. 명령어에서 각 데이터 파싱
	// 3. 현재 시간과 입력 시간 비교
	// 4. 메신저로 전달

	// 1. 현재 상태 검사 : GOLDENBALL_STATUS_NOTHING
	if (gserver->m_clGoldenBall.GetStatus() != GOLDENBALL_STATUS_NOTHING)
		return ;

	// 2. 명령어에서 각 데이터 파싱
	CLCString	strTeam1(GOLDENBALL_TEAM_NAME_LENGTH + 1);
	CLCString	strTeam2(GOLDENBALL_TEAM_NAME_LENGTH + 1);
	int			nYear;
	int			nMonth;
	int			nDay;
	int			nHour;
	int			nMin;

	char tmpBuf[MAX_MESSAGE_SIZE] = {0,};
	tmpBuf[0] = '\0';
	arg = AnyOneArg(arg, tmpBuf);
	if (*tmpBuf == '\0') return ;
	strTeam1	= tmpBuf;
	tmpBuf[0] = '\0';
	arg = AnyOneArg(arg, tmpBuf);
	if (*tmpBuf == '\0') return ;
	strTeam2	= tmpBuf;
	tmpBuf[0] = '\0';
	arg = AnyOneArg(arg, tmpBuf);
	if (*tmpBuf == '\0') return ;
	nYear		= atoi(tmpBuf);
	tmpBuf[0] = '\0';
	arg = AnyOneArg(arg, tmpBuf);
	if (*tmpBuf == '\0') return ;
	nMonth		= atoi(tmpBuf);
	tmpBuf[0] = '\0';
	arg = AnyOneArg(arg, tmpBuf);
	if (*tmpBuf == '\0') return ;
	nDay		= atoi(tmpBuf);
	tmpBuf[0] = '\0';
	arg = AnyOneArg(arg, tmpBuf);
	if (*tmpBuf == '\0') return ;
	nHour		= atoi(tmpBuf);
	tmpBuf[0] = '\0';
	arg = AnyOneArg(arg, tmpBuf);
	if (*tmpBuf == '\0') return ;
	nMin		= atoi(tmpBuf);

	// 3. 현재 시간과 입력 시간 비교
	time_t tCurrent;
	time(&tCurrent);
	struct tm tmEndVote;
	memset(&tmEndVote, 0, sizeof(tmEndVote));
	tmEndVote.tm_year	= nYear - 1900;
	tmEndVote.tm_mon	= nMonth - 1;
	tmEndVote.tm_mday	= nDay;
	tmEndVote.tm_hour	= nHour;
	tmEndVote.tm_min	= nMin;
	tmEndVote.tm_isdst	= -1;
	time_t tEndVote = mktime(&tmEndVote);
	if (tEndVote <= tCurrent)
		return ;

	// 4. 메신저로 전달
	{
		CNetMsg::SP rmsg(new CNetMsg);
		MsgrEventGoldenballVoteMsg(rmsg, ch->m_index, strTeam1, strTeam2, nYear, nMonth, nDay, nHour, nMin);
		SEND_Q(rmsg, gserver->m_messenger);
	}
}

void do_GMEventGoldenBall(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	// 응모 시작하기	: /event gball vote <team1> <team2> <year> <month> <day> <hour> <min>
	//					: 현재부터 응모 시작하여 지정된 일시에 응모 종료
	// 보상 시작하기	: /event gball gift <team1 score> <team2 score> <year> <month> <day> <hour> <min>
	//					: 현재부터 보상 시작하여 지정된 일시에 보상 종료
	// 현재 상태 보기	: /event gball status
	//					: 응모중 - A/B팀 이름과 종료일시 알려줌
	//					: 보상중 - A/B팀 이름/점수와 종료일시 알려줌
	// 이벤트 종료		: /event gball end
	char tmpBuf[MAX_MESSAGE_SIZE] = {0,};
	arg = AnyOneArg(arg, tmpBuf);
	CLCString strSubCommand = tmpBuf;

	if (strcmp(strSubCommand, "vote") == 0)
	{
		do_GMEventGoldenBall_Vote(ch, arg, vec);
	}
	else if (strcmp(strSubCommand, "gift") == 0)
	{
		do_GMEventGoldenBall_Gift(ch, arg, vec);
	}
	else if (strcmp(strSubCommand, "status") == 0)
	{
		do_GMEventGoldenBall_Status(ch, arg, vec);
	}
	else if (strcmp(strSubCommand, "end") == 0)
	{
		do_GMEventGoldenBall_End(ch, arg, vec);
	}
	else
		return ;
}

void do_GMEventOXQuiz_Quiz(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	char tmpBuf[MAX_MESSAGE_SIZE] = {0,};
	arg = AnyOneArg(arg, tmpBuf);
	if (!tmpBuf[0])		return ;
	int nIndex = atoi(tmpBuf);

	tmpBuf[0] = '\0';
	arg = AnyOneArg(arg, tmpBuf);
	int nSec = (tmpBuf[0]) ? atoi(tmpBuf) : 20;

	// 메신저로 모든 서버군에 전달한다
	{
		CNetMsg::SP rmsg(new CNetMsg);
		MsgrEventOXQuizMsg(rmsg, ch->m_index, nIndex, nSec);
		SEND_Q(rmsg, gserver->m_messenger);
	}
}

void do_GMEventOXQuiz_End(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	// 메신저로 모든 서버군에 전달한다
	{
		CNetMsg::SP rmsg(new CNetMsg);
		MsgrEventOXEndMsg(rmsg, ch->m_index);
		SEND_Q(rmsg, gserver->m_messenger);
	}
}

void do_GMEventOXQuiz_Start(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	// 시작 시간 구하기
	struct tm tmStart;
	int	year, mon, day, hour, min;
	memset(&tmStart, 0, sizeof(tmStart));
	char tmpBuf[MAX_MESSAGE_SIZE] = {0,};

	tmpBuf[0] = '\0';
	arg = AnyOneArg(arg, tmpBuf);
	year = atoi(tmpBuf) - 1900;

	tmpBuf[0] = '\0';
	arg = AnyOneArg(arg, tmpBuf);
	mon = atoi(tmpBuf) - 1;

	tmpBuf[0] = '\0';
	arg = AnyOneArg(arg, tmpBuf);
	day = atoi(tmpBuf);

	tmpBuf[0] = '\0';
	arg = AnyOneArg(arg, tmpBuf);
	hour = atoi(tmpBuf);

	tmpBuf[0] = '\0';
	arg = AnyOneArg(arg, tmpBuf);
	min = atoi(tmpBuf);

	tmStart.tm_year = year;
	tmStart.tm_mon  = mon;
	tmStart.tm_mday = day;
	tmStart.tm_hour = hour;
	tmStart.tm_min  = min;
	tmStart.tm_isdst = -1;

	time_t timeStart = mktime(&tmStart);

	// 현재 시간과 비교
	time_t timeCurrent;
	time(&timeCurrent);
	if (timeCurrent >= timeStart)
		return ;

	// 메신저로 현재 서브 채널의 존서버에 모두 전달
	{
		CNetMsg::SP rmsg(new CNetMsg);
		MsgrEventOXSetStartMsg(rmsg, ch->m_index, year + 1900, mon + 1, day, hour, min);
		SEND_Q(rmsg, gserver->m_messenger);
	}
}

void do_GMEventOXQuiz(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	// 시작하기		: oxevent start <year> <mon> <day> <hour> <min>
	//				: 바로 입장 가능하며 min분후 이벤트 시작, 해당 접속 채널에만 적용
	// 종료하기		: oxevent end
	//				: oxevent 가 바로 종료되며, 존안의 유저들은 모두 주노마을로 이동한다., 전체 서버군 적용
	// 문제 내기	: oxevent quiz <num> <sec>
	//				: num에 해당하는 퀴즈 제시, 제한시간 sec 초, sec이 없으면 기본20초, 전체 서버군 적용
	char tmpBuf[MAX_MESSAGE_SIZE] = {0,};
	arg = AnyOneArg(arg, tmpBuf);
	CLCString strSubCommand = tmpBuf;

	if (strcmp(strSubCommand, "start") == 0)
	{
		do_GMEventOXQuiz_Start(ch, arg, vec);
	}
	else if (strcmp(strSubCommand, "end") == 0)
	{
		do_GMEventOXQuiz_End(ch, arg, vec);
	}
	else if (strcmp(strSubCommand, "quiz") == 0)
	{
		do_GMEventOXQuiz_Quiz(ch, arg, vec);
	}
	else
		return ;
}

void do_GMEventValentine2006(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	if (!arg || !(*arg))		return ;

	char tmpBuf[MAX_MESSAGE_SIZE] = {0,};
	arg = AnyOneArg(arg, tmpBuf);

	TrimString(tmpBuf);

	{
		CNetMsg::SP rmsg(new CNetMsg);

		if (strcmp(tmpBuf, "start") == 0)
			MsgrEventReqMsg(rmsg, -1, -1, -1, gserver->m_serverno, gserver->m_subno, ch->m_index, "valentine2006", 1);
		else if (strcmp(tmpBuf, "stop") == 0)
			MsgrEventReqMsg(rmsg, -1, -1, -1, gserver->m_serverno, gserver->m_subno, ch->m_index, "valentine2006", 0);
		else if (strcmp(tmpBuf, "ing") == 0)
			MsgrEventReqMsg(rmsg, -1, -1, -1, gserver->m_serverno, gserver->m_subno, ch->m_index, "valentine2006", 2);

		if (gserver->isRunMessenger())
		{
			SEND_Q(rmsg, gserver->m_messenger);
		}
		else
		{
			SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Not running Messenger Server..");
			SEND_Q(rmsg, ch->m_desc);
		}
	}
}

void do_GMEventXMas2005(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	if (!arg || !(*arg))		return ;

	char tmpBuf[MAX_MESSAGE_SIZE] = {0,};
	arg = AnyOneArg(arg, tmpBuf);

	TrimString(tmpBuf);

	{
		CNetMsg::SP rmsg(new CNetMsg);

		if (strcmp(tmpBuf, "start") == 0)
			MsgrEventReqMsg(rmsg, -1, -1, -1, gserver->m_serverno, gserver->m_subno, ch->m_index, "xmas2005", 1);
		else if (strcmp(tmpBuf, "stop") == 0)
			MsgrEventReqMsg(rmsg, -1, -1, -1, gserver->m_serverno, gserver->m_subno, ch->m_index, "xmas2005", 0);
		else if (strcmp(tmpBuf, "ing") == 0)
			MsgrEventReqMsg(rmsg, -1, -1, -1, gserver->m_serverno, gserver->m_subno, ch->m_index, "xmas2005", 2);

		if (gserver->isRunMessenger())
		{
			SEND_Q(rmsg, gserver->m_messenger);
		}
		else
		{
			SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Not running Messenger Server..");
			SEND_Q(rmsg, ch->m_desc);
		}
	}
}

void do_GMEventNewYear2006(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	if (!arg || !(*arg))		return ;

	char tmpBuf[MAX_MESSAGE_SIZE] = {0,};
	arg = AnyOneArg(arg, tmpBuf);

	TrimString(tmpBuf);

	{
		CNetMsg::SP rmsg(new CNetMsg);

		if (strcmp(tmpBuf, "start") == 0)
			MsgrEventReqMsg(rmsg, -1, -1, -1, gserver->m_serverno, gserver->m_subno, ch->m_index, "newyear2006", 1);
		else if (strcmp(tmpBuf, "stop") == 0)
			MsgrEventReqMsg(rmsg, -1, -1, -1, gserver->m_serverno, gserver->m_subno, ch->m_index, "newyear2006", 0);
		else if (strcmp(tmpBuf, "ing") == 0)
			MsgrEventReqMsg(rmsg, -1, -1, -1, gserver->m_serverno, gserver->m_subno, ch->m_index, "newyear2006", 2);

		if (gserver->isRunMessenger())
		{
			SEND_Q(rmsg, gserver->m_messenger);
		}
		else
		{
			SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Not running Messenger Server..");
			SEND_Q(rmsg, ch->m_desc);
		}
	}
}

// event itemdrop <serverno | all | cur> <start rate | ing | stop>
// rate : 100 ~ 200 [%]
//		: 0 - stop
//		: -1 - ing
void do_GMEventItemDrop(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	int target = -1;			// 대상 서버 군 번호: -1: all, 1 ~ n: server group

	// 대상 서버군 정하기
	char tmpBuf[MAX_MESSAGE_SIZE] = {0,};
	TrimString(tmpBuf);
	arg = AnyOneArg(arg, tmpBuf, true);
	if (tmpBuf[0] == '\0') goto INVALID_FORMAT;
	if (strcmp(tmpBuf, "all") == 0)
		target = -1;
	else if (strcmp(tmpBuf, "cur") == 0)
		target = gserver->m_serverno;
	else
	{
		// all, cur 모두 아니면 숫자여야함
		char* p = tmpBuf;
		while (*p)
		{
			if (!isdigit(*p))
				goto INVALID_FORMAT;
			p++;
		}
		target = atoi(tmpBuf);
		if (target < 1)
			goto INVALID_FORMAT;
	}

	// 명령 정하기
	tmpBuf[0] = '\0';
	TrimString(tmpBuf);
	arg = AnyOneArg(arg, tmpBuf, true);
	if (tmpBuf[0] == '\0') goto INVALID_FORMAT;

#ifdef EVENT_ITEMDROP_AUTO
	int start_dt[6];
	int end_dt[6];
	memset(start_dt, -1, sizeof(start_dt));
	memset(end_dt, -1, sizeof(end_dt));
#endif

	{
		CNetMsg::SP rmsg(new CNetMsg);

		if (strcmp(tmpBuf, "start") == 0)
		{
			// rate 정하기
			tmpBuf[0] = '\0';
			TrimString(tmpBuf);
			arg = AnyOneArg(arg, tmpBuf, true);
			if (tmpBuf[0] == '\0') goto INVALID_FORMAT;
			int rate = atoi(tmpBuf);
			if (rate < 100 || rate > 200)
				goto INVALID_FORMAT;

#ifdef EVENT_ITEMDROP_AUTO
			if(ParsingTime(arg, start_dt, end_dt) == false)
				goto INVALID_FORMAT;

#endif // EVENT_ITEMDROP_AUTO
			MsgrEventReqMsg(rmsg, target, -1, -1, gserver->m_serverno, gserver->m_subno, ch->m_index, "itemdrop", rate);
		}
		else if (strcmp(tmpBuf, "ing") == 0)
		{
			MsgrEventReqMsg(rmsg, target, -1, -1, gserver->m_serverno, gserver->m_subno, ch->m_index, "itemdrop", -1);
		}
		else if (strcmp(tmpBuf, "stop") == 0)
		{
			MsgrEventReqMsg(rmsg, target, -1, -1, gserver->m_serverno, gserver->m_subno, ch->m_index, "itemdrop", 0);
		}
		else
			goto INVALID_FORMAT;

#ifdef EVENT_ITEMDROP_AUTO // /MsgrEventReqMsg 함수가 다른 곳에서도 사용하므로 외부에서 rmsg 구성함.
		int i;
		for(i = 0; i < 6; i++)
		{
			RefMsg(rmsg) << start_dt[i];
		}
		for(i = 0; i < 6; i++)
		{
			RefMsg(rmsg) << end_dt[i];
		}
#endif // EVENT_ITEMDROP_AUTO

		if (gserver->isRunMessenger())
		{
			SEND_Q(rmsg, gserver->m_messenger);
		}
		else
		{
			SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Messenger Not Running");
			SEND_Q(rmsg, ch->m_desc);
		}
	}

	return ;

INVALID_FORMAT:
	{
		CNetMsg::SP rmsg(new CNetMsg);
		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Invalid Command");
		SEND_Q(rmsg, ch->m_desc);
	}
	{
		CNetMsg::SP rmsg(new CNetMsg);
		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Usage: event itemdrop <server-group no | all | cur> <start <rate> | ing | stop> <Start time> <End time>");
		SEND_Q(rmsg, ch->m_desc);
	}
	{
		CNetMsg::SP rmsg(new CNetMsg);
		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "rate[%]: 100 ~ 200");
		SEND_Q(rmsg, ch->m_desc);
	}
}

void do_GMEventAkan(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	return ;
}

void do_GMEvent(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	char tmpBuf[MAX_MESSAGE_SIZE] = {0,};
	const char* p = AnyOneArg(arg, tmpBuf, true);
	TrimString(tmpBuf);

	if (strcmp(tmpBuf, "itemdrop") == 0)
		do_GMEventItemDrop(ch, p, vec);

	else if (strcmp(tmpBuf, "xmas2005") == 0)
		do_GMEventXMas2005(ch, p, vec);

	else if (strcmp(tmpBuf, "newyear2006") == 0)
		do_GMEventNewYear2006(ch, p, vec);

	else if (strcmp(tmpBuf, "valentine2006") == 0)
		do_GMEventValentine2006(ch, p, vec);

	else if (strcmp(tmpBuf, "oxevent") == 0)
		do_GMEventOXQuiz(ch, p, vec);

	else if (strcmp(tmpBuf, "gball") == 0)
		do_GMEventGoldenBall(ch, p, vec);

	else if (strcmp(tmpBuf, "akan") == 0)
		do_GMEventAkan(ch, p, vec);
}

void do_GMKickCharIndex(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	if (vec.size() != 1)
		return;

	int kicked_charIndex = atoi(vec[0].c_str());
	if (ch->m_index == kicked_charIndex)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "can't kick myself");
		SEND_Q(rmsg, ch->m_desc);

		return;
	}

	CNetMsg::SP rmsg(new CNetMsg);
	ServerToServerPacket::makeKickUserByCharIndex(rmsg, ch->m_index, kicked_charIndex);
	SEND_Q(rmsg, gserver->m_helper);
}

void do_GMEventShow(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
}

void do_GMEventSetting(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
}

void do_GMChanceEvent(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
}

void do_GMToolChatMonitor(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
#ifdef GMTOOL
	int serverno, subno;
	char onoff[100], name[100];
	char* p1;
	if(!(ch->m_desc->m_userFlag & IS_GM_CHAR))
	{
		CNetMsg::SP rmsg(new CNetMsg);
		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "FAILED(Only GMTOOL)");
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if (!arg || !*arg)
		return ;
	char tmpBuf[MAX_MESSAGE_SIZE] = {0,};
	// 이름
	arg = AnyOneArg(arg, tmpBuf);
	TrimString(tmpBuf);
	if (strlen(tmpBuf) < 1)
		return ;
	strcpy(name, tmpBuf);

	if (!arg || !(*arg) )
	{
		goto INVALID_FORMAT;
		return;
	}
	// ON or OFF
	arg = AnyOneArg(arg, tmpBuf);
	TrimString(tmpBuf);
	if(strcmp(tmpBuf, "on") != 0 && strcmp(tmpBuf, "off") != 0)
	{
		goto INVALID_FORMAT;
		return ;
	}
	strcpy(onoff, tmpBuf);

	if (!arg || !(*arg) )
		return;

	// server, sub
	arg = AnyOneArg(arg, tmpBuf);
	TrimString(tmpBuf);
	p1 = tmpBuf;
	while (*p1)
	{
		if (!isdigit(*p1))
			return ;		//goto INVALID_FORMAT;
		p1++;
	}
	serverno = atoi(tmpBuf);

	if (!arg || !(*arg) )
		return;

	arg = AnyOneArg(arg, tmpBuf);
	TrimString(tmpBuf);
	p1 = tmpBuf;
	while (*p1)
	{
		if (!isdigit(*p1))
			return ;		//goto INVALID_FORMAT;
		p1++;
	}
	subno = atoi(tmpBuf);

	{
		CNetMsg::SP rmsg(new CNetMsg);
		MsgrGmToolChatMonitorReqMsg(rmsg, 0, serverno, subno, -1, ch->m_index, name, onoff);
		SEND_Q(rmsg, gserver->m_messenger);
	}

	{
		char tmpMessageBuf[MAX_MESSAGE_SIZE] = {0,};
		sprintf(tmpMessageBuf, "Sending Chatmonitor %s %s", name, onoff);
		CNetMsg::SP rmsg(new CNetMsg);
		GMSayMsg(rmsg, 0, 0, 0, "", tmpMessageBuf, (unsigned char)MSG_CHAT_GM_SAY);
		SEND_Q(rmsg, ch->m_desc);
	}

	return ;

INVALID_FORMAT:
	{
		CNetMsg::SP rmsg(new CNetMsg);
		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Invalid Command");
		SEND_Q(rmsg, ch->m_desc);
	}
	{
		CNetMsg::SP rmsg(new CNetMsg);
		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Usage: gmchatmonitor <name> <on | off>");
		SEND_Q(rmsg, ch->m_desc);
	}
#endif // GMTOOL
}

void do_GMDoubleItemEvent(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
#ifdef DOUBLE_ITEM_DROP
	int target = -1;			// 대상 서버 군 번호: -1: all, 1 ~ n: server group
	char tmpBuf[MAX_MESSAGE_SIZE] = {0,};

	// 대상 서버군 정하기
	if (!arg || !(*arg))
		goto INVALID_FORMAT;

	arg = AnyOneArg(arg, tmpBuf, true);
	TrimString(tmpBuf);
	if (strcmp(tmpBuf, "all") == 0)
		target = -1;
	else if (strcmp(tmpBuf, "cur") == 0)
		target = gserver->m_serverno;
	else
	{
		// all, cur 모두 아니면 숫자여야함
		char* p = tmpBuf;
		while (*p)
		{
			if (!isdigit(*p))
				goto INVALID_FORMAT;
			p++;
		}
		target = atoi(tmpBuf);
		if (target < 1)
			goto INVALID_FORMAT;
	}

	// 명령 정하기
	if (!arg || !(*arg))
		goto INVALID_FORMAT;

	{
		char tmpBuf1[MAX_MESSAGE_SIZE] = {0,};

		CNetMsg::SP rmsg(new CNetMsg);
		arg = AnyOneArg(arg, tmpBuf1, true);
		TrimString(tmpBuf1);
		if (strcmp(tmpBuf1, "start") == 0)
		{
			arg = AnyOneArg(arg, tmpBuf, true);
			TrimString(tmpBuf);

			///// yhj  081014  ///////////////////// 확률 제한해제
			//if( atoi(g_buf) < 0 || atoi(g_buf) > 200)
			//	goto INVALID_FORMAT;
			///////////////////////////////////////

			MsgrDoubleItemEventReqMsg(rmsg, 0, target, -1, -1, ch->m_index, 0, atoi(tmpBuf));
		}
		else if (strcmp(tmpBuf1, "ing") == 0)
			MsgrDoubleItemEventReqMsg(rmsg, 0, target, -1, -1, ch->m_index, 1, -1);
		else if (strcmp(tmpBuf1, "stop") == 0)
			MsgrDoubleItemEventReqMsg(rmsg, 0, target, -1, -1, ch->m_index, 2, -1);
		else
			goto INVALID_FORMAT;

		if (gserver->isRunMessenger())
		{
			SEND_Q(rmsg, gserver->m_messenger);
		}
		else
		{
			SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Messenger Not Running");
			SEND_Q(rmsg, ch->m_desc);
		}
	}

	return ;

INVALID_FORMAT:
	{
		CNetMsg::SP rmsg(new CNetMsg);
		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Invalid Command");
		SEND_Q(rmsg, ch->m_desc);
	}
	{
		CNetMsg::SP rmsg(new CNetMsg);
		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Usage: doubleitem_event <server-group no | all | cur> <start | ing | stop>");
		SEND_Q(rmsg, ch->m_desc);
	}
#endif // DOUBLE_ITEM_DROP
}

void do_GMDoublePetExpEvent(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	char tmpBuf[MAX_MESSAGE_SIZE] = {0,};
#ifdef DOUBLE_PET_EXP
	int target = -1;			// 대상 서버 군 번호: -1: all, 1 ~ n: server group

	// 대상 서버군 정하기
	if (!arg || !(*arg))
		goto INVALID_FORMAT;
	arg = AnyOneArg(arg, tmpBuf, true);
	TrimString(tmpBuf);
	if (strcmp(tmpBuf, "all") == 0)
		target = -1;
	else if (strcmp(tmpBuf, "cur") == 0)
		target = gserver->m_serverno;
	else
	{
		// all, cur 모두 아니면 숫자여야함
		char* p = tmpBuf;
		while (*p)
		{
			if (!isdigit(*p))
				goto INVALID_FORMAT;
			p++;
		}
		target = atoi(tmpBuf);
		if (target < 1)
			goto INVALID_FORMAT;
	}

	// 명령 정하기
	if (!arg || !(*arg))
		goto INVALID_FORMAT;

	{
		CNetMsg::SP rmsg(new CNetMsg);
		arg = AnyOneArg(arg, tmpBuf, true);
		TrimString(tmpBuf);
		if (strcmp(tmpBuf, "start") == 0)
		{
			arg = AnyOneArg(arg, tmpBuf, true);
			TrimString(tmpBuf);
			int nExpPercent = 100;
			nExpPercent = atoi(tmpBuf);

			if( nExpPercent < 100 || nExpPercent > 1000)
				goto INVALID_FORMAT;
			if( (nExpPercent % 10) != 0)
				goto INVALID_FORMAT;

#ifdef DOUBLE_PET_EXP_AUTO
			// 시간 설정 부분..
			int start_dt[6];
			int end_dt[6];
			memset(start_dt, -1, sizeof(start_dt));
			memset(end_dt, -1, sizeof(end_dt));

			if(ParsingTime(arg, start_dt, end_dt) == false)
			{
				goto INVALID_FORMAT;
			}
			else
			{
				struct tm tmStart;
				struct tm tmEnd;
				memset(&tmStart, 0, sizeof(tmStart));
				memset(&tmEnd, 0, sizeof(tmEnd));
				tmStart.tm_year = start_dt[0] - 1900;
				tmStart.tm_mon = start_dt[1] - 1;
				tmStart.tm_mday = start_dt[2];
				tmStart.tm_hour = start_dt[3];
				tmStart.tm_min = start_dt[4];
				tmStart.tm_sec = start_dt[5];
				tmStart.tm_isdst = -1;

				tmEnd.tm_year = end_dt[0] - 1900;
				tmEnd.tm_mon = end_dt[1] - 1;
				tmEnd.tm_mday = end_dt[2];
				tmEnd.tm_hour = end_dt[3];
				tmEnd.tm_min = end_dt[4];
				tmEnd.tm_sec = end_dt[5];
				tmEnd.tm_isdst = -1;

				time_t tStart = mktime(&tmStart);
				time_t tEnd = mktime(&tmEnd);
				MsgrDoublePetExpEventReqMsg(rmsg, 0, target, -1, -1, ch->m_index, 0, nExpPercent, (long)tStart, (long)tEnd);
			}
#else // DOUBLE_PET_EXP_AUTO
			MsgrDoublePetExpEventReqMsg(rmsg, 0, target, -1, -1, ch->m_index, 0, nExpPercent);
#endif
		}
		else if (strcmp(tmpBuf, "ing") == 0)
			MsgrDoublePetExpEventReqMsg(rmsg, 0, target, -1, -1, ch->m_index, 1, -1);
		else if (strcmp(tmpBuf, "stop") == 0)
			MsgrDoublePetExpEventReqMsg(rmsg, 0, target, -1, -1, ch->m_index, 2, -1);
		else
			goto INVALID_FORMAT;

		if (gserver->isRunMessenger())
		{
			SEND_Q(rmsg, gserver->m_messenger);
		}
		else
		{
			SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Messenger Not Running");
			SEND_Q(rmsg, ch->m_desc);
		}
	}

	return ;

INVALID_FORMAT:
	{
		CNetMsg::SP rmsg(new CNetMsg);
		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Invalid Command");
		SEND_Q(rmsg, ch->m_desc);
	}
	{
		CNetMsg::SP rmsg(new CNetMsg);
#ifdef DOUBLE_PET_EXP_AUTO
		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Usage: doublepetexp_event <server-group no | all | cur> <start | ing | stop> <Starttime> <Endtime>");
#else
		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Usage: doublepetexp_event <server-group no | all | cur> <start | ing | stop>");
#endif
		SEND_Q(rmsg, ch->m_desc);
	}
#endif // DOUBLE_PET_EXP
}

void do_GMDoubleAttackEvent(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
#ifdef DOUBLE_ATTACK
	int target = -1;			// 대상 서버 군 번호: -1: all, 1 ~ n: server group
	char tmpBuf[MAX_MESSAGE_SIZE] = {0,};

	// 대상 서버군 정하기
	if (!arg || !(*arg))
		goto INVALID_FORMAT;

	arg = AnyOneArg(arg, tmpBuf, true);
	TrimString(tmpBuf);
	if (strcmp(tmpBuf, "all") == 0)
		target = -1;
	else if (strcmp(tmpBuf, "cur") == 0)
		target = gserver->m_serverno;
	else
	{
		// all, cur 모두 아니면 숫자여야함
		char* p = tmpBuf;
		while (*p)
		{
			if (!isdigit(*p))
				goto INVALID_FORMAT;
			p++;
		}
		target = atoi(tmpBuf);
		if (target < 1)
			goto INVALID_FORMAT;
	}

	// 명령 정하기
	if (!arg || !(*arg))
		goto INVALID_FORMAT;

	{
		CNetMsg::SP rmsg(new CNetMsg);
		arg = AnyOneArg(arg, tmpBuf, true);
		TrimString(tmpBuf);
		if (strcmp(tmpBuf, "start") == 0)
		{
			arg = AnyOneArg(arg, tmpBuf, true);
			TrimString(tmpBuf);

			if( atoi(tmpBuf) < 0 || atoi(tmpBuf) > 200)
				goto INVALID_FORMAT;

			MsgrDoubleAttackEventReqMsg(rmsg, 0, target, -1, -1, ch->m_index, 0, atoi(tmpBuf));
		}
		else if (strcmp(tmpBuf, "ing") == 0)
			MsgrDoubleAttackEventReqMsg(rmsg, 0, target, -1, -1, ch->m_index, 1, -1);
		else if (strcmp(tmpBuf, "stop") == 0)
			MsgrDoubleAttackEventReqMsg(rmsg, 0, target, -1, -1, ch->m_index, 2, -1);
		else
			goto INVALID_FORMAT;

		if (gserver->isRunMessenger())
		{
			SEND_Q(rmsg, gserver->m_messenger);
		}
		else
		{
			SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Messenger Not Running");
			SEND_Q(rmsg, ch->m_desc);
		}
	}

	return ;

INVALID_FORMAT:
	{
		CNetMsg::SP rmsg(new CNetMsg);
		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Invalid Command");
		SEND_Q(rmsg, ch->m_desc);
	}
	{
		CNetMsg::SP rmsg(new CNetMsg);
		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Usage: doubleattack_event <server-group no | all | cur> <start | ing | stop>");
		SEND_Q(rmsg, ch->m_desc);
	}
#endif // DOUBLE_ATTACK
}

void do_GMDropItemEvent(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
#ifdef EVENT_DROPITEM
	int target = -1;
	int npcidx, itemidx, prob;
	npcidx = -1;
	itemidx = -1;
	prob = 0;
	char tmpBuf[MAX_MESSAGE_SIZE] = {0,};

	if (!arg || !(*arg))
		goto INVALID_FORMAT;

	arg = AnyOneArg(arg, tmpBuf, true);
	TrimString(tmpBuf);
	if (strcmp(tmpBuf, "all") == 0)
		target = -1;
	else if (strcmp(tmpBuf, "cur") == 0)
		target = gserver->m_serverno;
	else
	{
		// all, cur 모두 아니면 숫자여야함
		char* p = tmpBuf;
		while (*p)
		{
			if (!isdigit(*p))
				goto INVALID_FORMAT;
			p++;
		}
		target = atoi(tmpBuf);
		if (target < 1)
			goto INVALID_FORMAT;
	}

	// 명령 정하기
	if (!arg || !(*arg))
		goto INVALID_FORMAT;
	arg = AnyOneArg(arg, tmpBuf, true);
	TrimString(tmpBuf);

	if (strcmp(tmpBuf, "start") == 0)
	{
		// npcidx
		arg = AnyOneArg(arg, tmpBuf);
		char* p;
		p = tmpBuf;
		while (*p)
		{
			if (!isdigit(*p))
				goto INVALID_FORMAT;
			p++;
		}
		npcidx = atoi(tmpBuf);

		CNPCProto* npcproto = NULL;
		npcproto = gserver->m_npcProtoList.FindProto(npcidx);
		if(!npcproto)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Not FOUND NPC");
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}

		// itemidx
		arg = AnyOneArg(arg, tmpBuf);
		p = tmpBuf;
		while (*p)
		{
			if (!isdigit(*p))
				goto INVALID_FORMAT;
			p++;
		}
		itemidx = atoi(tmpBuf);

		CItem* item = gserver->m_itemProtoList.CreateItem(itemidx, -1, 0, 0, 1);
		if(!item)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Not FOUND ITEM");
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}
		delete item;
		item = NULL;

		// prob
		arg = AnyOneArg(arg, tmpBuf);
		p = tmpBuf;
		while (*p)
		{
			if (!isdigit(*p))
				goto INVALID_FORMAT;
			p++;
		}

		prob = atoi(tmpBuf);
		if(prob <= 0 || prob > 100)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "SELECT PROB(1 ~ 100)");
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}

		{
			CNetMsg::SP rmsg(new CNetMsg);
			MsgrEventDropItemReqMsg(rmsg, 0, target, -1, -1, ch->m_index, 0, npcidx, itemidx, prob, gserver->m_serverno, gserver->m_subno);
			SEND_Q(rmsg, gserver->m_messenger);
		}
	}
	else if (strcmp(tmpBuf, "ing") == 0)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		MsgrEventDropItemReqMsg(rmsg, 0, target, -1, -1, ch->m_index, 1, npcidx, itemidx, prob, gserver->m_serverno, gserver->m_subno);
		SEND_Q(rmsg, gserver->m_messenger);
	}
	else if (strcmp(tmpBuf, "stop") == 0)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		MsgrEventDropItemReqMsg(rmsg, 0, target, -1, -1, ch->m_index, 2, npcidx, itemidx, prob, gserver->m_serverno, gserver->m_subno);
		SEND_Q(rmsg, gserver->m_messenger);
	}
	else
		goto INVALID_FORMAT;

	return;

INVALID_FORMAT:
	{
		CNetMsg::SP rmsg(new CNetMsg);
		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Invalid Command");
		SEND_Q(rmsg, ch->m_desc);
	}
	{
		CNetMsg::SP rmsg(new CNetMsg);
		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Usage: npcdropitem_event <server-group no | all | cur> <start | ing | stop>");
		SEND_Q(rmsg, ch->m_desc);
	}
#endif // EVENT_DROPITEM
}

void do_GMSilenceMulti(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
#ifdef SILENCE_PC_MULTI
	int distance, sec;
	char* p1;
	p1 = NULL;

	if (!arg || !(*arg))		return ;

	char tmpBuf[MAX_MESSAGE_SIZE] = {0,};
	arg = AnyOneArg(arg, tmpBuf);
	TrimString(tmpBuf);
	p1 = tmpBuf;
	while (*p1)
	{
		if (!isdigit(*p1))
			return ;		//goto INVALID_FORMAT;
		p1++;
	}
	distance = atoi(tmpBuf);

	if (!arg || !(*arg))		return ;

	arg = AnyOneArg(arg, tmpBuf);
	TrimString(tmpBuf);
	p1 = tmpBuf;
	while (*p1)
	{
		if (!isdigit(*p1))
			return ;		//goto INVALID_FORMAT;
		p1++;
	}
	sec = atoi(tmpBuf);

	if(distance <= 0 || time <= 0)
		return ;

	int sx = ch->m_cellX - 2;
	int ex = ch->m_cellX + 2;
	int sz = ch->m_cellZ - 2;
	int ez = ch->m_cellZ + 2;

	int x, z;
	for(x = sx ; x <= ex ; ++x)
	{
		if (x < 0 || x >= ch->m_pArea->m_size[0])
			continue;

		for(z = sz ; z <= ez ; ++z)
		{
			if (z < 0 || z >= ch->m_pArea->m_size[1])
				continue;

			CCharacter* tch;
			CCharacter* tchNext = ch->m_pArea->m_cell[x][z].m_listChar;
			while((tch = tchNext))
			{
				tchNext = tchNext->m_pCellNext;

				if( IS_PC(tch) && (tch != ch) )
				{
					CPC* pc = TO_PC(tch);
					if (GetDistance(ch, pc) < distance)
					{
						pc->m_silencePulse = gserver->m_pulse + (sec * PULSE_REAL_SEC);
					}
				}
			}
		}
	}

#endif // SILENCE_PC_MULTI
}

void do_GMKickMulti(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
#ifdef KICK_PC_MULTI
	if (!arg || !(*arg))		return ;

	char tmpBuf[MAX_MESSAGE_SIZE] = {0,};
	char* p1;
	int distance;
	arg = AnyOneArg(arg, tmpBuf);
	TrimString(tmpBuf);
	p1 = tmpBuf;
	while (*p1)
	{
		if (!isdigit(*p1))
			return ;		//goto INVALID_FORMAT;
		p1++;
	}
	distance = atoi(tmpBuf);

	if(distance <= 0)
		return ;

	int sx = ch->m_cellX - 2;
	int ex = ch->m_cellX + 2;
	int sz = ch->m_cellZ - 2;
	int ez = ch->m_cellZ + 2;

	int x, z;
	for(x = sx ; x <= ex ; ++x)
	{
		if (x < 0 || x >= ch->m_pArea->m_size[0])
			continue;

		for(z = sz ; z <= ez ; ++z)
		{
			if (z < 0 || z >= ch->m_pArea->m_size[1])
				continue;

			CCharacter* tch;
			CCharacter* tchNext = ch->m_pArea->m_cell[x][z].m_listChar;
			while((tch = tchNext))
			{
				tchNext = tchNext->m_pCellNext;

				if( IS_PC(tch) && (tch != ch) )
				{
					CPC* pc = TO_PC(tch);
					if (GetDistance(ch, pc) < distance)
					{
						CNetMsg::SP rmsg(new CNetMsg);
						MsgrLogoutReqMsg(rmsg, -1, -1, -1, -1, pc->m_desc->m_idname);
						SEND_Q(rmsg, gserver->m_messenger);
					}
				}
			}
		}
	}
#endif // KICK_PC_MULTI
}

void do_GMExpLimit(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	int target = -1;
	char tmpBuf[MAX_MESSAGE_SIZE] = {0,};

#ifdef GM_EXP_LIMIT_AUTO
	int nLimitStart = 250;
	int nLimitEnd = 250;
	int start_dt[6];
	int end_dt[6];
	memset(start_dt, -1, sizeof(start_dt));
	memset(end_dt, -1, sizeof(end_dt));
#else
	int nLimit = 250;
#endif // GM_EXP_LIMIT_AUTO

	if (!arg || !(*arg))
		goto INVALID_FORMAT;

	arg = AnyOneArg(arg, tmpBuf, true);
	TrimString(tmpBuf);
	if (strcmp(tmpBuf, "all") == 0)
		target = -1;
	else if (strcmp(tmpBuf, "cur") == 0)
		target = gserver->m_serverno;
#ifdef GM_EXP_LIMIT_AUTO
	else if (strcmp(tmpBuf, "ing") == 0)
	{
		// 시간 및 현재 LIMIT 전송.
		sprintf(tmpBuf, "EXP Limit:%d SP Limit:%d Start Time:%d-%d-%d %d:%d:%d End Time:%d-%d-%d %d:%d:%d",
				gserver->m_nExpLimit,
				gserver->m_nSpLimit,
				gserver->m_nGMExpLimitStart[0],
				gserver->m_nGMExpLimitStart[1],
				gserver->m_nGMExpLimitStart[2],
				gserver->m_nGMExpLimitStart[3],
				gserver->m_nGMExpLimitStart[4],
				gserver->m_nGMExpLimitStart[5],
				gserver->m_nGMExpLimitEnd[0],
				gserver->m_nGMExpLimitEnd[1],
				gserver->m_nGMExpLimitEnd[2],
				gserver->m_nGMExpLimitEnd[3],
				gserver->m_nGMExpLimitEnd[4],
				gserver->m_nGMExpLimitEnd[5]
			   );

		{
			CNetMsg::SP rmsg(new CNetMsg);
			SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", tmpBuf);
			SEND_Q(rmsg, ch->m_desc);
		}
		return ;
	}
#endif
	else
	{
		// all, cur 모두 아니면 숫자여야함
		char* p = tmpBuf;
		while (*p)
		{
			if (!isdigit(*p))
				goto INVALID_FORMAT;
			p++;
		}
		target = atoi(tmpBuf);
		if (target < 1)
			goto INVALID_FORMAT;
	}

	// 명령 정하기
#ifdef GM_EXP_LIMIT_AUTO
	if (!arg || !(*arg))
		goto INVALID_FORMAT;
	arg = AnyOneArg(arg, tmpBuf, true);
	TrimString(tmpBuf);
	nLimitStart = atoi( tmpBuf );
	if ( nLimitStart <0 || nLimitStart > 3000 )
		goto INVALID_FORMAT;

	if (!arg || !(*arg))
		goto INVALID_FORMAT;
	arg = AnyOneArg(arg, tmpBuf, true);
	TrimString(tmpBuf);
	nLimitEnd = atoi( tmpBuf );
	if ( nLimitEnd <0 || nLimitEnd > 3000 )
		goto INVALID_FORMAT;

	if(ParsingTime(arg, start_dt, end_dt) == false)
		goto INVALID_FORMAT;

#else
	if (!arg || !(*arg))
		goto INVALID_FORMAT;
	arg = AnyOneArg(arg, tmpBuf, true);
	TrimString(tmpBuf);
	nLimit = atoi(tmpBuf);
	if ( nLimit <0 || nLimit > 3000 )
		goto INVALID_FORMAT;
#endif // GM_EXP_LIMIT_AUTO

	{
		CNetMsg::SP rmsg(new CNetMsg);
#ifdef GM_EXP_LIMIT_AUTO
		// 메세지 재구성
		MsgrSetExpLimitMsg( rmsg, target, ch->m_index, nLimitStart, nLimitEnd, start_dt, end_dt );
#else
		MsgrSetExpLimitMsg( rmsg, target, ch->m_index, nLimit );
#endif // GM_EXP_LIMIT_AUTO
		SEND_Q(rmsg, gserver->m_messenger);
	}
	return;

INVALID_FORMAT:
	{
		CNetMsg::SP rmsg(new CNetMsg);
		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Invalid Command");
		SEND_Q(rmsg, ch->m_desc);
	}
	{
		CNetMsg::SP rmsg(new CNetMsg);
#ifdef GM_EXP_LIMIT_AUTO
		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Usage: exp_limit <server-group no | all | cur> <exp_limit_percent_start> <exp_limit_percent_end> <start_time> <end_time>");
#else
		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Usage: exp_limit <server-group no | all | cur> exp_limit_percent");
#endif // GM_EXP_LIMIT_AUTO
		SEND_Q(rmsg, ch->m_desc);
	}
}

void do_GMDoubleEventAuto(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
#ifdef NEW_DOUBLE_EVENT_AUTO
	int target = -1;			// 대상 서버 군 번호: -1: all, 1 ~ n: server group
	int	zonenum;			// 대상 존 번호: -1: all, 0 ~ : zonenum
	zonenum = -1;
	char tmpBuf[MAX_MESSAGE_SIZE] = {0,};

	// 대상 서버군 정하기
	if (!arg || !(*arg))
		goto INVALID_FORMAT;

	arg = AnyOneArg(arg, tmpBuf, true);
	TrimString(tmpBuf);
	if (strcmp(tmpBuf, "all") == 0)
		target = -1;
	else if (strcmp(tmpBuf, "cur") == 0)
		target = gserver->m_serverno;
	else
	{
		// all, cur 모두 아니면 숫자여야함
		char* p = tmpBuf;
		while (*p)
		{
			if (!isdigit(*p))
				goto INVALID_FORMAT;
			p++;
		}
		target = atoi(tmpBuf);
		if (target < 1)
			goto INVALID_FORMAT;
	}

	// 명령 정하기
	if (!arg || !(*arg))
		goto INVALID_FORMAT;
	arg = AnyOneArg(arg, tmpBuf, true);
	TrimString(tmpBuf);
#ifdef NEW_DOUBLE_EVENT_AUTO_TIME
	int start_dt[6];
	int end_dt[6];
	memset(start_dt, -1, sizeof(start_dt));
	memset(end_dt, -1, sizeof(end_dt));
#endif // NEW_DOUBLE_EVENT_AUTO_TIME
	if (strcmp(tmpBuf, "start") == 0)
	{
#ifdef NEW_DOUBLE_EVENT_AUTO_TIME

		if(ParsingTime(arg, start_dt, end_dt) == false)
			goto INVALID_FORMAT;

#endif // NEW_DOUBLE_EVENT_AUTO_TIME

		{
			CNetMsg::SP rmsg(new CNetMsg);
#ifdef NEW_DOUBLE_EVENT_AUTO_TIME
			MsgrDoubleEventAutoReqMsg(rmsg, 0, target, -1, -1, ch->m_index, 0, start_dt, end_dt);
#else
			MsgrDoubleEventAutoReqMsg(rmsg, 0, target, -1, -1, ch->m_index, 0);
#endif // NEW_DOUBLE_EVENT_AUTO_TIME
			SEND_Q(rmsg, gserver->m_messenger);
		}
	}
	else if(strcmp(tmpBuf, "ing") == 0)
	{
		CNetMsg::SP rmsg(new CNetMsg);
#ifdef NEW_DOUBLE_EVENT_AUTO_TIME
		MsgrDoubleEventAutoReqMsg(rmsg, 0, target, -1, -1, ch->m_index, 1, start_dt, end_dt);
#else
		MsgrDoubleEventAutoReqMsg(rmsg, 0, target, -1, -1, ch->m_index, 1);
#endif // NEW_DOUBLE_EVENT_AUTO_TIME
		SEND_Q(rmsg, gserver->m_messenger);
	}
	else if(strcmp(tmpBuf, "stop") == 0)
	{
		CNetMsg::SP rmsg(new CNetMsg);
#ifdef NEW_DOUBLE_EVENT_AUTO_TIME
		MsgrDoubleEventAutoReqMsg(rmsg, 0, target, -1, -1, ch->m_index, 2, start_dt, end_dt);
#else
		MsgrDoubleEventAutoReqMsg(rmsg, 0, target, -1, -1, ch->m_index, 2);
#endif // NEW_DOUBLE_EVENT_AUTO_TIME
		SEND_Q(rmsg, gserver->m_messenger);
	}
	else
		goto INVALID_FORMAT;

	return ;

INVALID_FORMAT:
	{
		CNetMsg::SP rmsg(new CNetMsg);
		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Invalid Command");
		SEND_Q(rmsg, ch->m_desc);
	}
	{
		CNetMsg::SP rmsg(new CNetMsg);
#ifdef NEW_DOUBLE_EVENT_AUTO_TIME
		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Usage: double_event_auto <server-group no | all | cur> < start | ing | stop > <start_time> <end_time>");
#else
		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Usage: double_event_auto <server-group no | all | cur> < start | ing | stop >");
#endif
		SEND_Q(rmsg, ch->m_desc);
	}
#endif // NEW_DOUBLE_EVENT_AUTO
}

void do_GMUpgradeEvent(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	int target = -1;			// 대상 서버 군 번호: -1: all, 1 ~ n: server group
	int prob = 0;
	char tmpBuf[MAX_MESSAGE_SIZE] = {0,};

	// 대상 서버군 정하기
	if (!arg || !(*arg))
		goto INVALID_FORMAT;

	arg = AnyOneArg(arg, tmpBuf, true);
	TrimString(tmpBuf);
	if (strcmp(tmpBuf, "all") == 0)
		target = -1;
	else if (strcmp(tmpBuf, "cur") == 0)
		target = gserver->m_serverno;
	else
	{
		// all, cur 모두 아니면 숫자여야함
		char* p = tmpBuf;
		while (*p)
		{
			if (!isdigit(*p))
				goto INVALID_FORMAT;
			p++;
		}
		target = atoi(tmpBuf);
		if (target < 1)
			goto INVALID_FORMAT;
	}

	// 명령 정하기
	if (!arg || !(*arg))
		goto INVALID_FORMAT;
	arg = AnyOneArg(arg, tmpBuf, true);
	TrimString(tmpBuf);

	if (strcmp(tmpBuf, "start") == 0)
	{
		if (!arg || !(*arg))
			goto INVALID_FORMAT;
		arg = AnyOneArg(arg, tmpBuf, true);
		TrimString(tmpBuf);

		char* p = tmpBuf;
		while (*p)
		{
			if (!isdigit(*p))
				goto INVALID_FORMAT;
			p++;
		}

		prob = atoi( tmpBuf );
		switch(prob)
		{
		case 100:
		case 125:
		case 150:
		case 175:
		case 200:
			break;
		default:
			goto INVALID_FORMAT;
		}

#ifdef UPGRADE_EVENT_AUTO
		int start_dt[6];
		int end_dt[6];
		memset(start_dt, -1, sizeof(start_dt));
		memset(end_dt, -1, sizeof(end_dt));

		if(ParsingTime(arg, start_dt, end_dt) == false)
		{
			goto INVALID_FORMAT;
		}
		else
		{
			struct tm tmStart;
			struct tm tmEnd;
			memset(&tmStart, 0, sizeof(tmStart));
			memset(&tmEnd, 0, sizeof(tmEnd));
			tmStart.tm_year = start_dt[0] - 1900;
			tmStart.tm_mon = start_dt[1] - 1;
			tmStart.tm_mday = start_dt[2];
			tmStart.tm_hour = start_dt[3];
			tmStart.tm_min = start_dt[4];
			tmStart.tm_sec = start_dt[5];
			tmStart.tm_isdst = -1;

			tmEnd.tm_year = end_dt[0] - 1900;
			tmEnd.tm_mon = end_dt[1] - 1;
			tmEnd.tm_mday = end_dt[2];
			tmEnd.tm_hour = end_dt[3];
			tmEnd.tm_min = end_dt[4];
			tmEnd.tm_sec = end_dt[5];
			tmEnd.tm_isdst = -1;

			time_t tStart = mktime(&tmStart);
			time_t tEnd = mktime(&tmEnd);

			{
				CNetMsg::SP rmsg(new CNetMsg);
				MsgrUpgradeEventReqMsg(rmsg, 0, target, -1, -1, ch->m_index, 0, prob, (long)tStart, (long)tEnd);
				SEND_Q(rmsg, gserver->m_messenger);
			}
		}

#else // UPGRADE_EVENT_AUTO
		{
			CNetMsg::SP rmsg(new CNetMsg);
			MsgrUpgradeEventReqMsg(rmsg, 0, target, -1, -1, ch->m_index, 0, prob);
			SEND_Q(rmsg, gserver->m_messenger);
		}
#endif // UPGRADE_EVENT_AUTO
	}
	else if(strcmp(tmpBuf, "ing") == 0)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		MsgrUpgradeEventReqMsg(rmsg, 0, target, -1, -1, ch->m_index, 1, prob);
		SEND_Q(rmsg, gserver->m_messenger);
	}
	else if(strcmp(tmpBuf, "stop") == 0)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		MsgrUpgradeEventReqMsg(rmsg, 0, target, -1, -1, ch->m_index, 2, prob);
		SEND_Q(rmsg, gserver->m_messenger);
	}
	else
		goto INVALID_FORMAT;

	return ;

INVALID_FORMAT:
	;
}

void do_GMKickGuildMember(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	CGuild* guild;
	CGuildMember* guildmember;
	CGuildMember* boss;
	char guildname[MAX_GUILD_NAME_LENGTH + 1], charname[MAX_CHAR_NAME_LENGTH + 1];

	if (!arg || !(*arg))
		return ;

	char tmpBuf[MAX_MESSAGE_SIZE] = {0,};
	arg = AnyOneArg(arg, tmpBuf);
	TrimString(tmpBuf);

	if (strlen(tmpBuf) < 1)
		return ;
	strcpy(guildname, tmpBuf);

	arg = AnyOneArg(arg, tmpBuf);
	TrimString(tmpBuf);

	if (strlen(tmpBuf) < 1)
		return ;

	strcpy(charname, tmpBuf);

	guild = gserver->m_guildlist.findguild(guildname);
	if(!guild)
		return ;

	guildmember = guild->findmember(charname);
	if(!guildmember)
		return ;

	boss = guild->boss();
	if(!boss)
		return ;

	if (guild->battleState() != GUILD_BATTLE_STATE_PEACE)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_CANNOT_KICK_BATTLE);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if (guildmember->pos() == MSG_GUILD_POSITION_BOSS)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_CANTKICKBOSS);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if (gserver->isRunHelper())
	{
		CPC* bosspc = PCManager::instance()->getPlayerByCharIndex(boss->charindex());
		if(bosspc)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			HelperGuildKickReqMsg(rmsg, guild->index(), boss->charindex(), guildmember->charindex());
			SEND_Q(rmsg, gserver->m_helper);
		}
	}
	else
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_GAMESERVER);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
}

void do_GMApplySkill(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	int skillIndex = 0;
	int skillLevel = 1;
	CCharacter* targetCh = ch;
	bool bcure = false;
	bool isNpc = false;

	if (!arg || !(*arg))		return ;

	char tmpBuf[MAX_MESSAGE_SIZE] = {0,};
	arg = AnyOneArg(arg, tmpBuf);
	skillIndex = atoi(tmpBuf);
	if( skillIndex == 0 && strcmp("cure",tmpBuf)==0 )
	{
		bcure = true;
		arg = AnyOneArg(arg, tmpBuf);
		skillIndex = atoi(tmpBuf);
	}
	else if ( skillIndex == 0 && strcmp("npc",tmpBuf)==0 )
	{
		isNpc = true;
		arg = AnyOneArg(arg, tmpBuf);
		skillIndex = atoi(tmpBuf);

		arg = AnyOneArg(arg, tmpBuf);
		int npcIndex = atoi(tmpBuf);
		targetCh = ch->m_pArea->FindChar( npcIndex , MSG_CHAR_NPC );
	}

	if (arg && (*arg))
	{
		arg = AnyOneArg(arg, tmpBuf);
		skillLevel = atoi(tmpBuf);

		if (arg && (*arg))
		{
			arg = AnyOneArg(arg, tmpBuf);
			CLCString name = tmpBuf;
			name.Trim();

			if (tmpBuf[0] != '\0')
			{

				PCManager::map_t& playerMap = PCManager::instance()->getPlayerMap();
				PCManager::map_t::iterator iter = playerMap.begin();
				PCManager::map_t::iterator endIter = playerMap.end();
				for ( ; iter != endIter; ++iter)
				{
					CPC* tpc = (*iter).pPlayer;
					if (tpc == NULL)
						continue;
					// 유저 검사
					if (( strcmp(tpc->m_name, name) == 0 || strcmp(tpc->m_nick, name) == 0))
					{
						targetCh = tpc;
						break;
					}
					else
					{
						targetCh = NULL;
					}
				}
			}
		}
	}

	if( bcure && targetCh )
	{
		targetCh->m_assist.CureBySkillIndex( skillIndex );
		return;
	}

	CSkill * pSkill = gserver->m_skillProtoList.Create(skillIndex,skillLevel);
	if( pSkill == NULL )
	{
		return;
	}

	bool bApply;

	if( isNpc )
	{
		ApplySkill(ch, targetCh, pSkill, -1, bApply);
	}
	else
	{
		if( ch->IsParty() && pSkill->m_proto->m_targetType == STT_PARTY_ALL )
			ApplySkillParty(ch,pSkill, -1, bApply );
		else if ( ch->IsExped() && pSkill->m_proto->m_targetType == STT_PARTY_ALL )
			ApplySkillExped (ch,pSkill, -1, bApply );
		else
			ApplySkill(ch, targetCh, pSkill, -1, bApply);
	}

	if( !bApply && pSkill )
	{
		delete pSkill;
		pSkill = NULL;
	}
}

// 클라이언트에서 캐릭터 상태플래그 변화를 확인하기 위한 명령어
void do_GMStateFlag(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	int stateflag = 0;

	if (!arg || !(*arg))
		return ;

	char tmpBuf[MAX_MESSAGE_SIZE] = {0,};
	arg = AnyOneArg(arg, tmpBuf);
	stateflag = atoi(tmpBuf);

	if (ch->IsSetPlayerState(1 << stateflag))
		ch->ResetPlayerState(1 << stateflag);
	else
		ch->SetPlayerState(1 << stateflag);

	{
		CNetMsg::SP rmsg(new CNetMsg);
		ExPlayerStateChangeMsg(rmsg, ch);
		ch->m_pArea->SendToCell(rmsg, ch, true);
	}
}

void do_GMDate(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	struct tm t;
	time_t t2;
	time(&t2);
	t = *localtime(&t2);

	CLCString time;
	time.Format( "[ %d-%d-%d %d:%d:%d NOW! ]" , t.tm_year +1900, t.tm_mon +1 , t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec );

	{
		CNetMsg::SP rmsg(new CNetMsg);
		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", (const char*) time );
		SEND_Q(rmsg, ch->m_desc);
	}
}

// 트리거 몇 개를 동시에 활성화하는 테스트를 위한 명령어
void do_GMPlayTrigger(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	if (!arg || !(*arg))
		return ;

	char tmpBuf[MAX_MESSAGE_SIZE] = {0,};
	int i, count = 0;

	arg = AnyOneArg(arg, tmpBuf);
	count = atoi(tmpBuf);

	if (count < 1)
		return;

	for (i=0; i < count; i++)
	{
		arg = AnyOneArg(arg, tmpBuf);
		{
			CNetMsg::SP rmsg(new CNetMsg);
			rmsg->Init(MSG_EXTEND);
			RefMsg(rmsg) << MSG_EX_TRIGGER_EVENT
						 << atoi(tmpBuf);

			do_TriggerEvent(ch, rmsg);
		}
	}
}

// 길드토너먼트 시작 명령
void do_GMBattleStart(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	int guildindex1;
	int guildindex2;
	int time;
	int zone;

	CGuild* g1;
	CGuild* g2;

	if (!arg || !(*arg))		return;

	char tmpBuf[MAX_MESSAGE_SIZE] = {0,};
	arg = AnyOneArg(arg, tmpBuf);
	guildindex1 = atoi(tmpBuf);

	if (!arg || !(*arg))		return;
	arg = AnyOneArg(arg, tmpBuf);
	guildindex2 = atoi(tmpBuf);

	if (!arg || !(*arg))		return;
	arg = AnyOneArg(arg, tmpBuf);
	zone = atoi(tmpBuf);

	if (!arg || !(*arg))		return;
	arg = AnyOneArg(arg, tmpBuf);
	time = atoi(tmpBuf);

	if (guildindex1 < 0 || guildindex2 < 0 || time < 0 || zone < 0)
		return;

	g1 = gserver->m_guildlist.findguild(guildindex1);
	g2 = gserver->m_guildlist.findguild(guildindex2);

	if (!g1 ||!g2)
		return;

	// 길마가 없다면 시작 시키지 않는다.
	CPC * boss1 = g1->boss()->GetPC();
	CPC * boss2 = g2->boss()->GetPC();

	if( !(boss1 && boss2))
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildBattleErrMsg(rmsg, MSG_GUILD_ERROR_BATTLE_NOTBOSS);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	if(gserver->isRunHelper())
	{
		CLCString	rmsgStr(512);

		{
			// 토너먼트에선 보상금이 없다.
			CNetMsg::SP rmsg(new CNetMsg);
			HelperGuildBattleReqMsg(rmsg, guildindex1, guildindex2, 0 /*prize is 0 */, zone, time);
			SEND_Q(rmsg, gserver->m_helper);
		}
	}
	else
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_GAMESERVER);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
}

//길드토너먼트 종료 명령
void do_GMBattleStop(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	int guildindex1;
	int guildindex2;

	CGuild* g1;
	CGuild* g2;

	if (!arg || !(*arg))		return;

	char tmpBuf[MAX_MESSAGE_SIZE] = {0,};
	arg = AnyOneArg(arg, tmpBuf);
	guildindex1 = atoi(tmpBuf);

	if (!arg || !(*arg))		return;
	arg = AnyOneArg(arg, tmpBuf);
	guildindex2 = atoi(tmpBuf);

	if (guildindex1 < 0 || guildindex2 < 0 || time < 0)
		return;

	g1 = gserver->m_guildlist.findguild(guildindex1);
	g2 = gserver->m_guildlist.findguild(guildindex2);

	if(g1->battleState() != GUILD_BATTLE_STATE_ING || g2->battleState() != GUILD_BATTLE_STATE_ING)
		return;

	if(gserver->isRunHelper())
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildBattleStopReqMsg(rmsg, guildindex1, guildindex2);
		SEND_Q(rmsg, gserver->m_helper);
	}
	else
	{
		CNetMsg::SP rmsg(new CNetMsg);
		GuildErrorMsg(rmsg, MSG_GUILD_ERROR_GAMESERVER);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
}

// 레이드 관련 DB데이터를 초기화 하기 위한 명령어
void do_GMResetRaid(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	if (arg && (*arg))
	{
		char tmpBuf[MAX_MESSAGE_SIZE] = {0,};
		char buf2[MAX_MESSAGE_SIZE] = {0,};
		int resetday = 0;
		int resethr = 0;
		int resetmin = 0;
		arg = AnyOneArg(arg, tmpBuf);
		if(strcmp(tmpBuf, "now") == 0)
		{
			// 여기서 메신저로 전 서버군에 뿌리고 레이드 채널은 지금부터 10분간 레이드 초기화에 들어간다.
			CNetMsg::SP rmsg(new CNetMsg);
			rmsg->Init(MSG_MSGR_REQ);
			RefMsg(rmsg) << (int)-1
						 << (int)-1
						 << (int)-1
						 << (int)-1
						 << (unsigned char)MSG_MSGR_RESET_RAID_NOW;
			SEND_Q(rmsg, gserver->m_messenger);
		}
		else if(strcmp(tmpBuf, "check") == 0)
		{
			// 세팅된 시간 확인하기
			sprintf(buf2, "RAID RESET TIME > ");
			switch(gserver->m_nResetDay)
			{
			case 0:
				strcat(buf2, "SUN ");
				break;
			case 1:
				strcat(buf2, "MON ");
				break;
			case 2:
				strcat(buf2, "TUE ");
				break;
			case 3:
				strcat(buf2, "WED ");
				break;
			case 4:
				strcat(buf2, "THU ");
				break;
			case 5:
				strcat(buf2, "FRI ");
				break;
			case 6:
				strcat(buf2, "SAT ");
				break;
			case 7:
				strcat(buf2, "ALL ");
				break;
			default:
				break;
			}
			if(gserver->m_nResethr < 10)
				strcat(buf2, "0");

			IntCat(buf2, gserver->m_nResethr, false);

			strcat(buf2, ":");

			if(gserver->m_nResetmin < 10)
				strcat(buf2, "0");

			IntCat(buf2, gserver->m_nResetmin, false);

			{
				CNetMsg::SP rmsg(new CNetMsg);
				SayMsg(rmsg, MSG_CHAT_GM, 0, "", "", buf2);
				SEND_Q(rmsg, ch->m_desc);
			}
		}
		else
		{
			// 요일별 리셋날짜 정하기
			if(strcmp(tmpBuf, "sun") == 0)	resetday = 0;
			else if(strcmp(tmpBuf, "mon") == 0)	resetday = 1;
			else if(strcmp(tmpBuf, "tue") == 0)	resetday = 2;
			else if(strcmp(tmpBuf, "wed") == 0)	resetday = 3;
			else if(strcmp(tmpBuf, "thu") == 0)	resetday = 4;
			else if(strcmp(tmpBuf, "fri") == 0)	resetday = 5;
			else if(strcmp(tmpBuf, "sat") == 0)	resetday = 6;
			else if(strcmp(tmpBuf, "all") == 0)	resetday = 7;

			if (arg && (*arg))
			{
				arg = AnyOneArg(arg, tmpBuf);
				resethr = atoi(tmpBuf);
				if(resethr < 0 || resethr > 23)
					resethr = 0;
			}
			if (arg && (*arg))
			{
				arg = AnyOneArg(arg, tmpBuf);
				resetmin = atoi(tmpBuf);
				if(resetmin < 0 || resetmin > 59)
					resetmin = 0;
			}

			// 요일, 시, 분을 전부 입력 받았으니 화면에 세팅 날짜를 뿌려주고 메신저로 보내서 전 서버군으로 전달한다.
			sprintf(buf2, "RAID RESET TIME > ");
			switch(resetday)
			{
			case 0:
				strcat(buf2, "SUN ");
				break;
			case 1:
				strcat(buf2, "MON ");
				break;
			case 2:
				strcat(buf2, "TUE ");
				break;
			case 3:
				strcat(buf2, "WED ");
				break;
			case 4:
				strcat(buf2, "THU ");
				break;
			case 5:
				strcat(buf2, "FRI ");
				break;
			case 6:
				strcat(buf2, "SAT ");
				break;
			case 7:
				strcat(buf2, "ALL ");
				break;
			default:
				break;
			}
			if(resethr < 10)
				strcat(buf2, "0");

			IntCat(buf2, resethr, false);

			strcat(buf2, ":");

			if(resetmin < 10)
				strcat(buf2, "0");

			IntCat(buf2, resetmin, false);

			{
				CNetMsg::SP rmsg(new CNetMsg);
				SayMsg(rmsg, MSG_CHAT_GM, 0, "", "", buf2);
				SEND_Q(rmsg, ch->m_desc);
			}

			{
				CNetMsg::SP rmsg(new CNetMsg);
				rmsg->Init(MSG_MSGR_REQ);
				RefMsg(rmsg) << (int)-1
							 << (int)-1
							 << (int)-1
							 << (int)-1
							 << (unsigned char)MSG_MSGR_RESET_RAID_SET
							 << resetday
							 << resethr
							 << resetmin;
				SEND_Q(rmsg, gserver->m_messenger);
			}
		}

		return ;
	}

	// 원정대 DB 정보를 초기화
	{
		{
			CNetMsg::SP rmsg(new CNetMsg);
			HelperRaidInzoneGameDataClearReqMsg(rmsg, ZONE_ALTER_OF_DARK);
			SEND_Q(rmsg, gserver->m_helper);
		}

		{
			CNetMsg::SP rmsg(new CNetMsg);
			HelperRaidInzoneGameDataClearReqMsg(rmsg, ZONE_AKAN_TEMPLE);
			SEND_Q(rmsg, gserver->m_helper);
		}

		{
			CNetMsg::SP rmsg(new CNetMsg);
			HelperRaidInzoneGameDataClearReqMsg(rmsg, ZONE_TARIAN_DUNGEON);
			SEND_Q(rmsg, gserver->m_helper);
		}
	}

	int i, j;

	if (!gserver->m_zones)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		SayMsg(rmsg, MSG_CHAT_GM, 0, "", "", "Error : resetraid 1");
		SEND_Q(rmsg, ch->m_desc);
	}

	// 트리거가 있는 레이드 DB 정보 초기화
	for (i=0; i < gserver->m_numZone; i++)
	{
		if (gserver->m_zones[i].m_index != ZONE_ALTER_OF_DARK && gserver->m_zones[i].m_index != ZONE_CAPPELLA_1)
			continue ;
		for (j=0; j< gserver->m_zones[i].m_countArea; j++)
		{
			if(&gserver->m_zones[i].m_area[j] && gserver->m_zones[i].m_area[j].m_bUseTriggerEvent)
			{
				gserver->m_zones[i].m_area[j].m_CTriggerList.ResetTriggerInfo(true);
				gserver->m_zones[i].m_area[j].m_bEnable = false;
			}
			else
			{
				CNetMsg::SP rmsg(new CNetMsg);
				SayMsg(rmsg, MSG_CHAT_GM, 0, "", "", "Error : resetraid ResetTriggerInfo");
				SEND_Q(rmsg, ch->m_desc);
			}
		}
	}

	{
		// 정상적으로 완료했다고 전달해줌
		CNetMsg::SP rmsg(new CNetMsg);
		SayMsg(rmsg, MSG_CHAT_GM, 0, "", "", "resetraid Complete");
		SEND_Q(rmsg, ch->m_desc);
	}
}

// 제작 아이템 레벨 업을 위한 명령어
void do_GMSetSealExp(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	if (!arg || !(*arg))
		return ;

	char tmpBuf[MAX_MESSAGE_SIZE] = {0,};
	if (!ch->m_sealSkillExp)
		return;

	int exp = 0;
	arg = AnyOneArg(arg, tmpBuf);
	exp = atoi(tmpBuf);

	if(exp < 1 && exp >= 200)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		SayMsg(rmsg, MSG_CHAT_GM, 0, "", "", "setsealexp fail : incorrect level");
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	for (int i = 0; i < MAX_SEAL_TYPE_SKILL; i++)
	{
		if (ch->m_sealSkillExp[i].nSkillIdx > 0)
			ch->SetSealExp(ch->m_sealSkillExp[i].nSkillIdx, exp);
	}

	{
		// 정상적으로 완료했다고 전달해줌
		CNetMsg::SP rmsg(new CNetMsg);
		SayMsg(rmsg, MSG_CHAT_GM, 0, "", "", "setsealexp Complete");
		SEND_Q(rmsg, ch->m_desc);
	}
}

void do_GMEndSubHelper(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	{
		CNetMsg::SP rmsg(new CNetMsg);
		SubHelperShutdownMsg(rmsg);
		SEND_Q(rmsg, gserver->m_subHelper);
	}
	return ;
}

// 내가 admin level이 몇 인지 확인하는 명령어
void do_GMMyAdmin(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	if(!ch)
		return;

	CLCString adminStr;
	adminStr.Format( "My admin Level is %d" , ch->m_admin );
	CNetMsg::SP rmsg(new CNetMsg);
	SayMsg(rmsg, MSG_CHAT_GM, 0, "", "", (const char*) adminStr );
	SEND_Q(rmsg, ch->m_desc);
}

void do_GMDTTimeSet(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	int time = -1;
	int arg2 = -1;

	if(!arg || !(*arg))
		return;

	char tmpBuf[MAX_MESSAGE_SIZE] = {0,};
	arg = AnyOneArg(arg, tmpBuf, true);
	if(strcmp(tmpBuf, "ing") == 0)
	{
		// 현재 상황 출력.
		char str[1024];
		int count = 0;
		strcpy(str, "Dungeon Time set :");
		for(int i = 0; i < 24; i++)
		{
			if(gserver->m_bTimeTable[i] == true)
			{
				IntCat(str, i, true);
				count++;
			}
		}
		if(count == 0)
		{
			strcat(str, " no set.");
		}

		{
			CNetMsg::SP rmsg(new CNetMsg);
			SayMsg(rmsg, MSG_CHAT_GM, 0, "", "", str);
			SEND_Q(rmsg, ch->m_desc);
		}

		return ;
	}
	else if(strcmp(tmpBuf, "all") == 0)
		time = 24;
	else
		time = atoi(tmpBuf);

	if(time == 0)
	{
		if(strcmp(tmpBuf, "0") != 0)
		{
			time = -1; // error 운영자 입력 오류.
		}
	}

	if (arg && (*arg))
	{
		arg = AnyOneArg(arg, tmpBuf, true);
	}
	if (strcmp(tmpBuf, "on") == 0)
	{
		arg2 = 1;
	}
	else if(strcmp(tmpBuf, "off") == 0)
	{
		arg2 = 0;
	}

	if (time < 0 || time > 24)
		return ;
	if (arg2 < 0 || arg2 > 1)
		return ;

	if(gserver->m_bDungeonTimeToggle == true)
	{
		char str[1024];
		strcpy(str, "Cannot change during Dungeon time system. Use '/dt_toggle off'");
		CNetMsg::SP rmsg(new CNetMsg);
		SayMsg(rmsg, MSG_CHAT_GM, 0, "", "", str);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	{
		// 여기서 시간과(time) toggle을 MSGR로 보내서 전 서버에 적용 시킬 것.
		CNetMsg::SP rmsg(new CNetMsg);
		MsgrDungeonTimeReqMsg(rmsg, MSG_MSRG_DUNGEON_TIME_TIMESET, time, arg2);
		SEND_Q(rmsg, gserver->m_messenger);
	}

	return ;
}

void do_GMDTExpSet(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	int set;
	if(!arg || !(*arg))
		return;

	char tmpBuf[MAX_MESSAGE_SIZE] = {0,};
	arg = AnyOneArg(arg, tmpBuf, true);

	if(strcmp(tmpBuf, "ing") == 0)
	{
		int current = 0;
		char str[1024];
		strcpy(str, "Dungeon Time Exp Set :");
		for(int i = 0; i < gserver->m_iZoneCount; i++)
		{
			if(gserver->m_iZoneExp[i] > 100)
			{
				current = gserver->m_iZoneExp[i];
				IntCat(str, current, false);
				break;
			}
		}
		if(current == 0)
		{
			strcat(str, " No set");
		}

		{
			CNetMsg::SP rmsg(new CNetMsg);
			SayMsg(rmsg, MSG_CHAT_GM, 0, "", "", str);
			SEND_Q(rmsg, ch->m_desc);
		}

		return ;
	}

	set = atoi(tmpBuf);

	if(set  < 100 || set > 600)
		return ;

	{
		// 여기서 set을 MSGR로 발송.
		CNetMsg::SP rmsg(new CNetMsg);
		MsgrDungeonTimeReqMsg(rmsg, MSG_MSRG_DUNGEON_TIME_EXPSET, set, -1);
		SEND_Q(rmsg, gserver->m_messenger);
	}

	return ;
}

void do_GMDTTimeInterval(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	int interval;
	if(!arg || !(*arg))
		return;

	char tmpBuf[MAX_MESSAGE_SIZE] = {0,};
	arg = AnyOneArg(arg, tmpBuf, true);

	if(strcmp(tmpBuf, "ing") == 0)
	{
		char str[1024];
		strcpy(str, "Dungeon Time Interval set :");
		if(gserver->m_iTimeInterval < 1 || gserver->m_iTimeInterval > 12)
		{
			strcat(str, " No set");
		}
		else
		{
			IntCat(str, gserver->m_iTimeInterval, true);
		}

		{
			CNetMsg::SP rmsg(new CNetMsg);
			SayMsg(rmsg, MSG_CHAT_GM, 0, "", "", str);
			SEND_Q(rmsg, ch->m_desc);
		}
		return ;
	}

	interval = atoi(tmpBuf);

	if(interval  < 1 || interval > 12)
		return ;

	if(gserver->m_bDungeonTimeToggle == true)
	{
		char str[1024];
		strcpy(str, "Cannot change during Dungeon time system. Use '/dt_toggle off'");
		CNetMsg::SP rmsg(new CNetMsg);
		SayMsg(rmsg, MSG_CHAT_GM, 0, "", "", str);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	{
		// 여기서 set을 MSGR로 발송.
		CNetMsg::SP rmsg(new CNetMsg);
		MsgrDungeonTimeReqMsg(rmsg, MSG_MSRG_DUNGEON_TIME_INTERVAL, interval, -1);
		SEND_Q(rmsg, gserver->m_messenger);
	}

	return ;
}

void do_GMDTToggle(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	int arg1 = -1;
	if(!arg || !(*arg))
		return;

	char tmpBuf[MAX_MESSAGE_SIZE] = {0,};
	arg = AnyOneArg(arg, tmpBuf, true);

	if(strcmp(tmpBuf, "ing") == 0)
	{
		char str[1024];
		strcpy(str, "Dungeon Time System is ");
		if(gserver->m_bDungeonTimeToggle == false)
			strcat(str, "OFF");
		else if(gserver->m_bDungeonTimeToggle == true)
			strcat(str, "ON");

		{
			CNetMsg::SP rmsg(new CNetMsg);
			SayMsg(rmsg, MSG_CHAT_GM, 0, "", "", str);
			SEND_Q(rmsg, ch->m_desc);
		}
		return ;
	}

	if(strcmp(tmpBuf, "on") == 0)
	{
		arg1 = 1;
	}
	else if(strcmp(tmpBuf, "off") == 0)
	{
		arg1 = 0;
	}

	if(arg1  < 0 || arg1 > 1)
		return ;

	{
		// 여기서 set을 MSGR로 발송.
		CNetMsg::SP rmsg(new CNetMsg);
		MsgrDungeonTimeReqMsg(rmsg, MSG_MSRG_DUNGEON_TIME_TOGGLE, arg1, -1);
		SEND_Q(rmsg, gserver->m_messenger);
	}

	return ;
}

void do_GMReloadCatalog(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	//	gserver->m_catalogList.Load();
	CNetMsg::SP rmsg(new CNetMsg);
	MsgrReloadCatalogMsg(rmsg, MSG_MSGR_RELOAD_CATALOG );
	SEND_Q(rmsg, gserver->m_messenger);

	return;
}

bool ParsingTime(const char*& arg, int start_dt[], int end_dt[])
{
	/////////////////////////////////////////////////여기부터 스타트 타임.
	/////// year
	char* p;
	if(!arg || !(*arg))
		return false;

	char tmpBuf[MAX_MESSAGE_SIZE] = {0,};
	arg = GetToken(arg, tmpBuf, true);
	if(strlen(tmpBuf) != 4)
		return false;
	p = tmpBuf;
	while(*p)
	{
		if(!isdigit(*p))
			return false;
		p++;
	}
	start_dt[0] = atoi(tmpBuf);
	/////// month
	if(!arg || !(*arg))
		return false;
	arg = GetToken(arg, tmpBuf, true);
	if(strlen(tmpBuf) < 1 || strlen(tmpBuf) > 2)
		return false;
	p = tmpBuf;
	while(*p)
	{
		if(!isdigit(*p))
			return false;
		p++;
	}
	start_dt[1] = atoi(tmpBuf);
	if(start_dt[1] < 1 || start_dt[1] > 12)
		return false;
	/////// day
	if(!arg || !(*arg))
		return false;
	arg = GetToken(arg, tmpBuf, true);
	if(strlen(tmpBuf) < 1 || strlen(tmpBuf) > 2)
		return false;
	p = tmpBuf;
	while(*p)
	{
		if(!isdigit(*p))
			return false;
		p++;
	}
	start_dt[2] = atoi(tmpBuf);
	if(start_dt[1] == 1 || start_dt[1] == 3 || start_dt[1] == 5 || start_dt[1] == 7 || start_dt[1] == 8 || start_dt[1] == 10 || start_dt[1] == 12)
	{
		if(start_dt[2] < 1 || start_dt[2] > 31)
			return false;
	}
	else if(start_dt[1] == 4 || start_dt[1] == 6 || start_dt[1] == 9 || start_dt[1] == 11)
	{
		if(start_dt[2] < 1 || start_dt[2] > 30)
			return false;
	}
	else
	{
		if(start_dt[0]/4 == 0 && (start_dt[0] % 100 != 0 || start_dt[0] % 400 == 0))		//윤년
		{
			if(start_dt[2] < 1 || start_dt[2] > 29)
				return false;
		}
		else	//평년
		{
			if(start_dt[2] < 1 || start_dt[2] > 28)
				return false;
		}
	}
	/////// hour
	if(!arg || !(*arg))
		return false;
	arg = GetToken(arg, tmpBuf, true);
	if(strlen(tmpBuf) < 1 || strlen(tmpBuf) > 2)
		return false;
	p = tmpBuf;
	while(*p)
	{
		if(!isdigit(*p))
			return false;
		p++;
	}
	start_dt[3] = atoi(tmpBuf);
	if(start_dt[3] < 0 || start_dt[3] > 23)
		return false;
	/////// minute
	if(!arg || !(*arg))
		return false;
	arg = GetToken(arg, tmpBuf, true);
	if(strlen(tmpBuf) < 1 || strlen(tmpBuf) > 2)
		return false;
	p = tmpBuf;
	while (*p)
	{
		if(!isdigit(*p))
			return false;
		p++;
	}
	start_dt[4] = atoi(tmpBuf);
	if(start_dt[4] < 0 || start_dt[4] > 59)
		return false;
	/////// sec
	if(!arg || !(*arg))
		return false;
	arg = GetToken(arg, tmpBuf, true);
	if(strlen(tmpBuf) < 1 || strlen(tmpBuf) > 2)
		return false;
	p = tmpBuf;
	while (*p)
	{
		if(!isdigit(*p))
			return false;
		p++;
	}
	start_dt[5] = atoi(tmpBuf);
	if(start_dt[5] < 0 || start_dt[5] > 59)
		return false;
	/////////////////////////////////////////////////여기까지 스타트 타임.
	/////////////////////////////////////////////////여기부터 종료 타임.
	/////// year
	if(!arg || !(*arg))
		return false;
	arg = GetToken(arg, tmpBuf, true);
	if(strlen(tmpBuf) != 4)
		return false;
	p = tmpBuf;
	while(*p)
	{
		if(!isdigit(*p))
			return false;
		p++;
	}
	end_dt[0] = atoi(tmpBuf);
	/////// month
	if(!arg || !(*arg))
		return false;
	arg = GetToken(arg, tmpBuf, true);
	if(strlen(tmpBuf) < 1 || strlen(tmpBuf) > 2)
		return false;
	p = tmpBuf;
	while(*p)
	{
		if(!isdigit(*p))
			return false;
		p++;
	}
	end_dt[1] = atoi(tmpBuf);
	if(end_dt[1] < 1 || end_dt[1] > 12)
		return false;
	/////// day
	if(!arg || !(*arg))
		return false;
	arg = GetToken(arg, tmpBuf, true);
	if(strlen(tmpBuf) < 1 || strlen(tmpBuf) > 2)
		return false;
	p = tmpBuf;
	while(*p)
	{
		if(!isdigit(*p))
			return false;
		p++;
	}
	end_dt[2] = atoi(tmpBuf);
	if(end_dt[1] == 1 || end_dt[1] == 3 || end_dt[1] == 5 || end_dt[1] == 7 || end_dt[1] == 8 || end_dt[1] == 10 || end_dt[1] == 12)
	{
		if(end_dt[2] < 1 || end_dt[2] > 31)
			return false;
	}
	else if(end_dt[1] == 4 || end_dt[1] == 6 || end_dt[1] == 9 || end_dt[1] == 11)
	{
		if(end_dt[2] < 1 || end_dt[2] > 30)
			return false;
	}
	else
	{
		if(end_dt[0]/4 == 0 && (end_dt[0] % 100 != 0 || end_dt[0] % 400 == 0))		//윤년
		{
			if(end_dt[2] < 1 || end_dt[2] > 29)
				return false;
		}
		else	//평년
		{
			if(end_dt[2] < 1 || end_dt[2] > 28)
				return false;
		}
	}
	/////// hour
	if(!arg || !(*arg))
		return false;
	arg = GetToken(arg, tmpBuf, true);
	if(strlen(tmpBuf) < 1 || strlen(tmpBuf) > 2)
		return false;
	p = tmpBuf;
	while(*p)
	{
		if(!isdigit(*p))
			return false;
		p++;
	}
	end_dt[3] = atoi(tmpBuf);
	if(end_dt[3] < 0 || end_dt[3] > 23)
		return false;
	/////// minute
	if(!arg || !(*arg))
		return false;
	arg = GetToken(arg, tmpBuf, true);
	if(strlen(tmpBuf) < 1 || strlen(tmpBuf) > 2)
		return false;
	p = tmpBuf;
	while (*p)
	{
		if(!isdigit(*p))
			return false;
		p++;
	}
	end_dt[4] = atoi(tmpBuf);
	if(end_dt[4] < 0 || end_dt[4] > 59)
		return false;
	/////// sec
	if(!arg || !(*arg))
		return false;
	arg = GetToken(arg, tmpBuf, true);
	if(strlen(tmpBuf) < 1 || strlen(tmpBuf) > 2)
		return false;
	p = tmpBuf;
	while (*p)
	{
		if(!isdigit(*p))
			return false;
		p++;
	}
	end_dt[5] = atoi(tmpBuf);
	if(end_dt[5] < 0 || end_dt[5] > 59)
		return false;
	/////////////////////////////////////////////////여기까지 종료 타임.
	return true;
}

void do_GMNPCDrop(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	if(!arg || !(*arg))
		return ;

	char tmpBuf[MAX_MESSAGE_SIZE] = {0,};
	arg = GetToken(arg, tmpBuf, true);

	char* p;
	int nNpcIndex, i;
	CItem* pItem;
	char str[1024];

	p = tmpBuf;
	while (*p)
	{
		if(!isdigit(*p))
			return ;
		p++;
	}
	nNpcIndex = atoi(tmpBuf);
	CNPCProto* pProto;
	pProto = gserver->m_npcProtoList.FindProto(nNpcIndex);

	if(pProto == NULL)
	{
		strcpy(str, "Cannot Found NPC Index : ");
		IntCat(str, nNpcIndex, false);
		CNetMsg::SP rmsg(new CNetMsg);
		SayMsg(rmsg, MSG_CHAT_GM, 0, "", "", str);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if(pProto->m_itemCount < 1)
	{
		strcpy(str, "No have item"); // pProto->m_item[i]
		CNetMsg::SP rmsg(new CNetMsg);
		SayMsg(rmsg, MSG_CHAT_GM, 0, "", "", str);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	for(i = 0; i < pProto->m_itemCount; i++)
	{
		pItem = gserver->m_itemProtoList.CreateItem(pProto->m_item[i], -1, 0, 0, 1);
		if(pItem == NULL)
		{
			strcpy(str, "ERROR : Cannot Find Item : "); // pProto->m_item[i]
			IntCat(str, pProto->m_item[i], false);
			CNetMsg::SP rmsg(new CNetMsg);
			SayMsg(rmsg, MSG_CHAT_GM, 0, "", "", str);
			SEND_Q(rmsg, ch->m_desc);

			continue;
		}

		ch->m_pArea->DropItem(pItem, ch);
		CNetMsg::SP rmsg(new CNetMsg);
		ItemDropMsg(rmsg, ch, pItem);
		pItem->m_pArea->SendToCell(rmsg, GET_YLAYER(pItem), pItem->m_cellX, pItem->m_cellZ);
	}

	for(i=0; i<MAX_NPC_DROPITEM; i++ )
	{
		pItem = gserver->m_itemProtoList.CreateItem(pProto->m_dropallitem[i], -1, 0, 0, 1);
		if(pItem == NULL)
		{
			strcpy(str, "ERROR : Cannot Find Item : "); // pProto->m_item[i]
			IntCat(str, pProto->m_dropallitem[i], false);
			CNetMsg::SP rmsg(new CNetMsg);
			SayMsg(rmsg, MSG_CHAT_GM, 0, "", "", str);
			SEND_Q(rmsg, ch->m_desc);

			continue;
		}

		ch->m_pArea->DropItem(pItem, ch);
		CNetMsg::SP rmsg(new CNetMsg);
		ItemDropMsg(rmsg, ch, pItem);
		pItem->m_pArea->SendToCell(rmsg, GET_YLAYER(pItem), pItem->m_cellX, pItem->m_cellZ);
	}
}

void do_GMGodBless(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	int skillIndex = 0;
	int skillLevel = 1;
	int	 i = 0 ;
	bool bcure = false;
	CSkill *pSkill = NULL;

	if (!arg || !(*arg))		return ;

	char tmpBuf[MAX_MESSAGE_SIZE] = {0,};
	arg = AnyOneArg(arg, tmpBuf);
	skillIndex = atoi(tmpBuf);
	if( skillIndex == 0 && strcmp("cure",tmpBuf)==0 )
	{
		bcure = true;
		arg = AnyOneArg(arg, tmpBuf);
		skillIndex = atoi(tmpBuf);
	}
	else
	{
		arg = AnyOneArg(arg, tmpBuf);
		skillLevel = atoi(tmpBuf);

		if( skillLevel < 1)
			skillLevel = 1;

		pSkill = gserver->m_skillProtoList.Create(skillIndex,skillLevel);
		if( pSkill == NULL )
		{
			return;
		}
		else
		{
			delete pSkill;
			pSkill = NULL;
		}
	}

	PCManager::map_t& playerMap = PCManager::instance()->getPlayerMap();
	PCManager::map_t::iterator iter = playerMap.begin();
	PCManager::map_t::iterator endIter = playerMap.end();
	for (; iter != endIter; ++iter)
	{
		CPC* tpc = (*iter).pPlayer;
		if (tpc == NULL)
		{
			continue;
		}

		if (bcure)
		{
			tpc->m_assist.CureBySkillIndex( skillIndex );

			CNetMsg::SP rmsg(new CNetMsg);
			EffectEtcMsg( rmsg, tpc, MSG_EFFECT_ETC_LEVELUP );
			tpc->m_pArea->SendToCell(rmsg, tpc, true);
		}
		else
		{
			pSkill = gserver->m_skillProtoList.Create(skillIndex,skillLevel);

			bool bApply;
			ApplySkill(tpc, tpc, pSkill, -2, bApply);

			if( !bApply && pSkill )
			{
				delete pSkill;
				pSkill = NULL;
			}
			else
			{
				CNetMsg::SP rmsg(new CNetMsg);
				EffectEtcMsg( rmsg, tpc, MSG_EFFECT_ETC_FIRECRACKER );
				tpc->m_pArea->SendToCell(rmsg, tpc, true);
			}
		}
	}


	arg = SkipSpaces(arg);
	if (!arg || !(*arg))
		return ;

	{
		CNetMsg::SP rmsg(new CNetMsg);
		MsgrNoticeMsg(rmsg, -1, gserver->m_serverno, gserver->m_subno, -1, arg);
		SEND_Q(rmsg, gserver->m_messenger);
	}
}

void do_GMEnchant(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	int nItemIndex = 0;
	int nEnchantItemIndex = 0;
	int nNowPlusValue = 0;
	int nEnchantCount = 0;
	int nResultSuccess = 0;
	int nResultBroken = 0;
	int nResultNoCodition = 0;
	int nResultMinus = 0;
	int nLoop = 0;

	if(!arg || !(*arg))	return ;

	char tmpBuf[MAX_MESSAGE_SIZE] = {0,};
	arg = AnyOneArg(arg, tmpBuf);
	nItemIndex = atoi(tmpBuf);
	CItem *pItem = gserver->m_itemProtoList.CreateItem(nItemIndex, -1, 0, 0, 1);

	if(pItem == NULL)
		return ;
	if( pItem->m_itemProto->getItemTypeIdx() != ITYPE_WEAPON
			&& pItem->m_itemProto->getItemTypeIdx() !=  ITYPE_WEAR )
	{
		char tmpMessageBuf[MAX_MESSAGE_SIZE] = {0,};
		sprintf(tmpMessageBuf, "Wrong ItemCode.");
		CNetMsg::SP rmsg(new CNetMsg);
		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", tmpMessageBuf);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	arg = AnyOneArg(arg, tmpBuf);
	nEnchantItemIndex = atoi(tmpBuf);
	if( nEnchantItemIndex != 85
			&& nEnchantItemIndex != 3888
			&& nEnchantItemIndex != 3889 )
	{
		char tmpMessageBuf[MAX_MESSAGE_SIZE] = {0,};
		sprintf(tmpMessageBuf, "Wrong Enchant Item.");
		CNetMsg::SP rmsg(new CNetMsg);
		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", tmpMessageBuf);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
	CItem *reItem = gserver->m_itemProtoList.CreateItem(nEnchantItemIndex, -1, 0, 0, 1);
	if(reItem == NULL)
		return ;

	arg = AnyOneArg(arg, tmpBuf);
	nNowPlusValue = atoi(tmpBuf);
	if( nNowPlusValue < 0
			|| nNowPlusValue > 30 )
	{
		char tmpMessageBuf[MAX_MESSAGE_SIZE] = {0,};
		sprintf(tmpMessageBuf, "Wrong Current Enchat.");
		CNetMsg::SP rmsg(new CNetMsg);
		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", tmpMessageBuf);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
	arg = AnyOneArg(arg, tmpBuf);
	nEnchantCount = atoi(tmpBuf);
	if( nEnchantCount < 0
			|| nEnchantCount > 1000 )
	{
		char tmpMessageBuf[MAX_MESSAGE_SIZE] = {0,};

		sprintf(tmpMessageBuf, "Wrong Loop Count.");
		CNetMsg::SP rmsg(new CNetMsg);
		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", tmpMessageBuf);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	for(nLoop = 0; nLoop < nEnchantCount; nLoop++)
	{
		switch(nEnchantItemIndex)
		{
		case 85:
			{
				if (nNowPlusValue >= 14)
				{
					int prob = GetRandom(1, 100);
					int upprob = 18;

					// 성공/변함없음(+3미만)/실패
					if (prob <= upprob)
					{
						nResultSuccess++;
					}
					else
					{
						prob -= upprob;
						// 하락 62%/그외는 무변
						int downprob = 62;
						if (prob <= downprob)
							nResultMinus++;
						else
							nResultNoCodition++;
					}
				}
				else
				{
					int prob = GetRandom(1, 100);
					int upprob = 60 - nNowPlusValue * 3;

					if (upprob < 1)
						upprob = 1;

					// 성공/변함없음(+3미만)/실패
					if (prob <= upprob)
						nResultSuccess++;

					else if (nNowPlusValue < 3)
						nResultNoCodition++;
					else
					{
						prob -= upprob;
						int destruct = 5;

						// 파괴 5%
						if (prob <= destruct)
						{
							nResultBroken++;
						}
						else
						{
							prob -= destruct;
							// 하락 13%/그외는 무변
							int downprob = 13;
							if (prob <= downprob)
								nResultMinus++;
							else
								nResultNoCodition++;
						}
					}
				}
			}
			break;
		case 3888:
			{
				if(nNowPlusValue < 5)
					nResultSuccess++;
				else
				{
					int nProb = GetRandom(1, 100);
					int nLevelDiff = reItem->m_itemProto->GetItemProtoLevel() - pItem->GetItemLevel();
					if (nLevelDiff > 9)
						nLevelDiff = 9;
					else if (nLevelDiff < -9)
						nLevelDiff = -9;

					int nUpProb[4] = { 0,0,100,0 };

					nUpProb[0] = 75 - (nNowPlusValue * 4 ) + nLevelDiff;	// 성공
					if( nUpProb[0] < 1 )
						nUpProb[0] = 1;
					nUpProb[1] = nNowPlusValue;							// 하락
					nUpProb[2] = 25 - nNowPlusValue;						// 유지
					nUpProb[3] = 75 - nUpProb[0];							// 파괴
					int idx = 0;
					while( nProb > 0 )
					{
						nProb -= nUpProb[idx];
						if(  nProb > 0 )
							idx++;
					}
					switch( idx )
					{
					case 0:
						nResultSuccess++;
						break;		// 성공
					case 1:
						nResultMinus++;
						break;		// 하락
					case 2:
						nResultNoCodition++;
						break;		// 유지
					case 3:
						nResultBroken++;
						break;		// 파괴
					default:
						nResultNoCodition++;
						break;
					}
				}
			}
			break;
		case 3889:
			{
				if(nNowPlusValue < 5)
					nResultSuccess++;
				else
				{
					int nProb = GetRandom(1, 100);
					int nLevelDiff = reItem->m_itemProto->GetItemProtoLevel() - pItem->GetItemLevel();
					if (nLevelDiff > 9)
						nLevelDiff = 9;
					else if (nLevelDiff < -9)
						nLevelDiff = -9;

					int nUpProb[4] = { 0,0,100,0 };

					nUpProb[0] = 95 - (nNowPlusValue * 5 ) + nLevelDiff;	// 성공
					if( nUpProb[0] < 1 )
						nUpProb[0] = 1;
					nUpProb[1] = 0;											// 하락
					nUpProb[2] = 0;											// 유지
					nUpProb[3] = 100 - nUpProb[0];							// 파괴
					int idx = 0;
					while( nProb > 0 )
					{
						nProb -= nUpProb[idx];
						if(  nProb > 0 )
							idx++;
					}
					switch( idx )
					{
					case 0:
						nResultSuccess++;
						break;		// 성공
					case 1:
						nResultMinus++;
						break;		// 하락
					case 2:
						nResultNoCodition++;
						break;		// 유지
					case 3:
						nResultBroken++;
						break;		// 파괴
					default:
						nResultNoCodition++;
						break;
					}
				}
			}
			break;
		default:
			break;
		}
	}

	{
		char tmpMessageBuf[MAX_MESSAGE_SIZE] = {0,};
		CNetMsg::SP rmsg(new CNetMsg);
		sprintf(tmpMessageBuf, "Success : %d", nResultSuccess);
		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", tmpMessageBuf);
		SEND_Q(rmsg, ch->m_desc);
	}

	{
		char tmpMessageBuf[MAX_MESSAGE_SIZE] = {0,};
		CNetMsg::SP rmsg(new CNetMsg);
		sprintf(tmpMessageBuf, "Broken : %d", nResultBroken);
		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", tmpMessageBuf);
		SEND_Q(rmsg, ch->m_desc);
	}

	{
		char tmpMessageBuf[MAX_MESSAGE_SIZE] = {0,};
		CNetMsg::SP rmsg(new CNetMsg);
		sprintf(tmpMessageBuf, "No Change : %d", nResultNoCodition);
		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", tmpMessageBuf);
		SEND_Q(rmsg, ch->m_desc);
	}

	{
		char tmpMessageBuf[MAX_MESSAGE_SIZE] = {0,};
		CNetMsg::SP rmsg(new CNetMsg);
		sprintf(tmpMessageBuf, "Minus : %d", nResultMinus);
		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", tmpMessageBuf);
		SEND_Q(rmsg, ch->m_desc);
	}

	if(pItem)
	{
		delete pItem;
		pItem = NULL;
	}

	if(reItem)
	{
		delete reItem;
		reItem = NULL;
	}
}

void do_GMModifyCashShop(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	int cmd = -1;
	char tmpBuf[MAX_MESSAGE_SIZE] = {0,};
	arg = AnyOneArg(arg, tmpBuf);
	CLCString strSub = tmpBuf;
	if(strcmp(strSub, "lock") == 0)
	{
		cmd = 0;
	}
	else if(strcmp(strSub, "reload_cash") == 0)
	{
		cmd = 1;
	}
	else if(strcmp(strSub, "unlock") == 0)
	{
		cmd = 2;
	}
	else if(strcmp(strSub, "wlr") == 0) // wlr = Wish List Request
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ConnWishlistReq(rmsg, ch);
		SEND_Q(rmsg, gserver->m_connector);
	}
	else if(strcmp(strSub, "wlsr") == 0) // wlsr = Wish List Save Request
	{
		int ctid;
		if (!arg || !(*arg))
			return ;
		arg = AnyOneArg(arg, tmpBuf);
		ctid = atoi(tmpBuf);

		CNetMsg::SP rmsg(new CNetMsg);
		ConnWishlistSaveReq(rmsg, ch, ctid);
		SEND_Q(rmsg, gserver->m_connector);

		return ;
	}
	else if(strcmp(strSub, "wldr") == 0) // wldr = Wish List Delete Request
	{
		int ctid;
		if (!arg || !(*arg))
			return ;
		arg = AnyOneArg(arg, tmpBuf);
		ctid = atoi(tmpBuf);

		CNetMsg::SP rmsg(new CNetMsg);
		ConnWishlistDelReq(rmsg, ch, ctid);
		SEND_Q(rmsg, gserver->m_connector);

		return ;
	}
	else if(strcmp(strSub, "test") == 0)
	{
		int ctid;
		if (!arg || !(*arg))
			return ;
		arg = AnyOneArg(arg, tmpBuf);
		ctid = atoi(tmpBuf);
		CCatalog* catal = NULL;
		catal = gserver->m_catalogList.Find(ctid);
	}
	else
		return ;

	{
		CNetMsg::SP rmsg(new CNetMsg);
		MsgrReloadCatalogMsg(rmsg, MSG_MSGR_MODIFY_CASHSHOP );
		RefMsg(rmsg) << cmd;
		SEND_Q(rmsg, gserver->m_messenger);
	}
	return;
}

void do_GMLCBallReset (CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	int		nTockenIndex = 0;
	char	nCourseIndex = -1;

	char tmpBuf[MAX_MESSAGE_SIZE] = {0,};
	// tocken index
	arg = AnyOneArg(arg, tmpBuf, true);
	TrimString(tmpBuf);
	if (strcmp(tmpBuf, "all") == 0)
		nTockenIndex = -1;
	else
	{
		nTockenIndex = atoi(tmpBuf);
	}

	// course index
	arg = AnyOneArg(arg, tmpBuf);
	CLCString strSub = tmpBuf;
	if(strcmp(strSub, "all") == 0)
	{
		nCourseIndex = -1 ;
	}
	else
	{
		nCourseIndex = atoi(tmpBuf);
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		rmsg->Init(MSG_SUBHELPER_COMMAND);
		RefMsg(rmsg) << (int)MSG_SUBHELPER_LCBALL_RESET
					 << nTockenIndex << nCourseIndex;
		SEND_Q( rmsg, gserver->m_subHelper );
	}
}

void do_GMAttEvent(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	return ;
}

void do_GMFunc(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	//int val = 0;
	char tmpBuf[MAX_MESSAGE_SIZE] = {0,};
	arg = AnyOneArg(arg, tmpBuf);

	if(strcmp(tmpBuf, "inven_del") == 0)
	{
		item_search_t vec;
		ch->m_inventory.searchAllItem(vec);
		item_search_t::iterator it = vec.begin();
		item_search_t::iterator endit = vec.end();
		for (; it != endit; ++it)
		{
			item_search_pair_t& p = *it;

			CNetMsg::SP rmsg(new CNetMsg);
			insideServer_do_ItemDelete(rmsg, p.pItem);
			do_Item(ch, rmsg);
		}
	}
	else if(strcmp(tmpBuf, "raid_normal") == 0)
	{
		int difficulty = 0;
		CNetMsg::SP rmsg(new CNetMsg);
		rmsg->Init(MSG_RAID);
		RefMsg(rmsg) << (unsigned char)MSG_RAID_INZONE_JOIN
					 << (int)36
					 << difficulty;
		do_Raid(ch, rmsg);
	}
	else if(strcmp(tmpBuf, "raid_hard") == 0)
	{
		int difficulty = 1;
		CNetMsg::SP rmsg(new CNetMsg);
		rmsg->Init(MSG_RAID);
		RefMsg(rmsg) << (unsigned char)MSG_RAID_INZONE_JOIN
					 << (int)36
					 << difficulty;
		do_Raid(ch, rmsg);
	}
	else if(strcmp(tmpBuf, "underlab") == 0)
	{
		int difficulty = 0;
		CNetMsg::SP rmsg(new CNetMsg);
		rmsg->Init(MSG_RAID);
		RefMsg(rmsg) << (unsigned char)MSG_RAID_INZONE_JOIN
					 << (int)41
					 << difficulty;
		do_Raid(ch, rmsg);
	}
	else if(strcmp(tmpBuf, "outlab") == 0)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		RaidInzoneQuitReq(rmsg,1,0);
		do_Raid(ch, rmsg);
	}

	else if(strcmp(tmpBuf, "raidtodo") == 0)
	{
		if( ch->m_pArea->m_zone->m_index != ZONE_AKAN_TEMPLE
				&& ch->m_pArea->m_zone->m_index != ZONE_DUNGEON4
				&& ch->m_pArea->m_zone->m_index != ZONE_TARIAN_DUNGEON
		  )
			return ;

		//	int arg1 = -1;
		if(!arg || !(*arg))
			return;
		arg = AnyOneArg(arg, tmpBuf, true);

		int index = -1;
		index = atoi(tmpBuf);

		if (index < 0)
			return ;

		CRaidDataProto* pProto;
		pProto = gserver->m_RaidInfo.m_RaidDataProtoList.Find(index);

		if(pProto == NULL)
			return ;

		{
			CNetMsg::SP rmsg(new CNetMsg);
			RaidSceneMsg(rmsg, OBJECT_TYPE_TODO, pProto->GetTodo()->GetObjectType(), pProto->GetTodo()->GetObjectIndex(), pProto->GetTodo()->GetObjectData());
			do_ExRaidScene(ch, rmsg);
		}
	}
	else if(strcmp(tmpBuf, "eur2") == 0)
	{
		if(!arg || !(*arg))
			return ;
		arg = AnyOneArg(arg, tmpBuf, true);

		{
			CNetMsg::SP rmsg(new CNetMsg);
			rmsg->Init(MSG_EVENT);
			RefMsg(rmsg) << (unsigned char)MSG_EVENT_EUR2_PROMOTION
						 << tmpBuf;
			do_Event(ch, rmsg);
		}
	}
	else if(strcmp(tmpBuf, "gogo") == 0)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		RoyalRumbleGoZone(rmsg);
		SEND_Q(rmsg, ch->m_desc);
	}
	else if(strcmp(tmpBuf, "rrstart") == 0)
	{
		if(gserver->m_subno != WAR_GROUND_CHANNEL)
			return ;
		if( gserver->m_RoyalRumble.GetRoyalRumbleTime() == 0)
		{
			if( !gserver->m_RoyalRumble.GetRookiePlaying() &&
					!gserver->m_RoyalRumble.GetSeniorPlaying() &&
					!gserver->m_RoyalRumble.GetMasterPlaying() )
			{
				gserver->m_RoyalRumble.m_bCondition = true;
			}
			else
			{
				CNetMsg::SP rmsg(new CNetMsg);
				RoyalRumbleTimeReq(rmsg, -1);
				SEND_Q(rmsg, ch->m_desc);
			}
		}
		else
		{
			CNetMsg::SP rmsg(new CNetMsg);
			RoyalRumbleTimeReq(rmsg, -1);
			SEND_Q(rmsg, ch->m_desc);
		}
	}
	else if(strcmp(tmpBuf, "addgp") == 0)
	{
		int gp = 0;
		arg = AnyOneArg(arg, tmpBuf);
		gp = atoi(tmpBuf);
		if(gp < 0 || gp > 100000) // 0보다 작거나, 100,000보다 크면 return
			return ;
		if( ch
				&& NULL != ch->m_guildInfo
				&& NULL != ch->m_guildInfo->guild())
		{
			int g_index;
			g_index = ch->m_guildInfo->guild()->index();
			CGuild* guild = gserver->m_guildlist.findguild(g_index);
			guild->AddGuildPoint(gp);
			ch->m_guildInfo->AddCumulatePoint(gp);

			{
				CNetMsg::SP rmsg(new CNetMsg);
				HelperNewGuildPointUpdate(rmsg, ch->m_index, guild->index(), gp);
				SEND_Q(rmsg, gserver->m_helper);
			}

			{
				CNetMsg::SP rmsg(new CNetMsg);
				HelperSaveGuildMemberPointMsg( rmsg, guild->index(), ch->m_index, gp );
				SEND_Q( rmsg, gserver->m_helper );
			}
		}
	}
#ifdef LC_KOR
	else if(strcmp(tmpBuf, "npcstat") == 0)
	{
		int val = 0;
		int i;
		if(!arg || !(*arg))
			return ;
		arg = AnyOneArg(arg, tmpBuf, true);
		int npcidx = atoi(tmpBuf);
		//CNPCProto* np = NULL;
		//np = gserver->m_npcProtoList.FindProto(npcidx);
		//if(!np)
		//	return;
		bool bFind = false;
		for(i = 0; i < gserver->m_npcProtoList.m_nCount; i++)
		{
			if(gserver->m_npcProtoList.m_npcProtoList[i].m_index == npcidx)
			{
				bFind = true;
				break;
			}
		}
		if(bFind == false)
			return ;
		else
		{
			if(!arg || !(*arg))
				return ;
			arg = AnyOneArg(arg, tmpBuf, true);
			if(strcmp(tmpBuf, "level") == 0)
			{
				if(!arg || !(*arg))
					return ;
				arg = AnyOneArg(arg, tmpBuf, true);

				val = atoi(tmpBuf);
				if(val < 1 || val > 165)
					return ;
				gserver->m_npcProtoList.m_npcProtoList[i].m_level = val;
			}
			else if(strcmp(tmpBuf, "hp") == 0)
			{
				if(!arg || !(*arg))
					return ;
				arg = AnyOneArg(arg, tmpBuf, true);

				val = atoi(tmpBuf);
				if(val < 1 || val > 100000000)
					return ;
				gserver->m_npcProtoList.m_npcProtoList[i].m_hp = val;
			}
			else if(strcmp(tmpBuf, "exp") == 0)
			{
				if(!arg || !(*arg))
					return ;
				arg = AnyOneArg(arg, tmpBuf, true);

				val = atoi(tmpBuf);
				if(val < 1 || val > 2000000000)
					return ;
				gserver->m_npcProtoList.m_npcProtoList[i].setExp(val);
			}
			else if(strcmp(tmpBuf, "sp") == 0)
			{
				if(!arg || !(*arg))
					return ;
				arg = AnyOneArg(arg, tmpBuf, true);

				val = atoi(tmpBuf);
				if(val < 1 || val > 10000000)
					return ;
				gserver->m_npcProtoList.m_npcProtoList[i].setSkillPoint(val);
			}
			else if(strcmp(tmpBuf, "runspeed") == 0)
			{
				if(!arg || !(*arg))
					return ;
				arg = AnyOneArg(arg, tmpBuf, true);

				val = atoi(tmpBuf);
				if(val < 1 || val > 10)
					return ;
				gserver->m_npcProtoList.m_npcProtoList[i].m_runSpeed = val;
			}
			else if(strcmp(tmpBuf, "sight") == 0)
			{
				if(!arg || !(*arg))
					return ;
				arg = AnyOneArg(arg, tmpBuf, true);

				val = atoi(tmpBuf);
				if(val < 1 || val > 50)
					return ;
				gserver->m_npcProtoList.m_npcProtoList[i].m_sight = val;
			}
			else if(strcmp(tmpBuf, "attack") == 0)
			{
				if(!arg || !(*arg))
					return ;
				arg = AnyOneArg(arg, tmpBuf, true);

				val = atoi(tmpBuf);
				if(val < 1 || val > 50000)
					return ;
				gserver->m_npcProtoList.m_npcProtoList[i].m_attack = val;
			}
			else if(strcmp(tmpBuf, "mattack") == 0)
			{
				if(!arg || !(*arg))
					return ;
				arg = AnyOneArg(arg, tmpBuf, true);

				val = atoi(tmpBuf);
				if(val < 1 || val > 10000)
					return ;
				gserver->m_npcProtoList.m_npcProtoList[i].m_magic = val;
			}
			else if(strcmp(tmpBuf, "defense") == 0)
			{
				if(!arg || !(*arg))
					return ;
				arg = AnyOneArg(arg, tmpBuf, true);

				val = atoi(tmpBuf);
				if(val < 1 || val > 100000)
					return ;
				gserver->m_npcProtoList.m_npcProtoList[i].m_defense = val;
			}
			else if(strcmp(tmpBuf, "resist") == 0)
			{
				if(!arg || !(*arg))
					return ;
				arg = AnyOneArg(arg, tmpBuf, true);

				val = atoi(tmpBuf);
				if(val < 1 || val > 50000)
					return ;
				gserver->m_npcProtoList.m_npcProtoList[i].m_resist = val;
			}
			else if(strcmp(tmpBuf, "attackspeed") == 0)
			{
				if(!arg || !(*arg))
					return ;
				arg = AnyOneArg(arg, tmpBuf, true);

				val = atoi(tmpBuf);
				if(val < 1 || val > 500)
					return ;
				gserver->m_npcProtoList.m_npcProtoList[i].m_attackSpeed = val;
			}
			else if(strcmp(tmpBuf, "str") == 0)
			{
				if(!arg || !(*arg))
					return ;
				arg = AnyOneArg(arg, tmpBuf, true);

				val = atoi(tmpBuf);
				if(val < 1 || val > 50000)
					return ;
				gserver->m_npcProtoList.m_npcProtoList[i].m_str = val;
			}
			else if(strcmp(tmpBuf, "dex") == 0)
			{
				if(!arg || !(*arg))
					return ;
				arg = AnyOneArg(arg, tmpBuf, true);

				val = atoi(tmpBuf);
				if(val < 1 || val > 50000)
					return ;
				gserver->m_npcProtoList.m_npcProtoList[i].m_dex = val;
			}
			else if(strcmp(tmpBuf, "int") == 0)
			{
				if(!arg || !(*arg))
					return ;
				arg = AnyOneArg(arg, tmpBuf, true);

				val = atoi(tmpBuf);
				if(val < 1 || val > 50000)
					return ;
				gserver->m_npcProtoList.m_npcProtoList[i].m_int = val;
			}
			else if(strcmp(tmpBuf, "con") == 0)
			{
				if(!arg || !(*arg))
					return ;
				arg = AnyOneArg(arg, tmpBuf, true);

				val = atoi(tmpBuf);
				if(val < 1 || val > 50000)
					return ;
				gserver->m_npcProtoList.m_npcProtoList[i].m_con = val;
			}
		}
	}
	else if(strcmp(tmpBuf, "idrare") == 0)
	{
		unsigned short tab;
		unsigned short invenIndex;
		if(!arg || !(*arg)) return;
		arg = AnyOneArg(arg, tmpBuf, true);
		tab = atoi(tmpBuf);
		if(!arg || !(*arg)) return;
		arg = AnyOneArg(arg, tmpBuf, true);
		invenIndex = atoi(tmpBuf);
		CItem *pItem = ch->m_inventory.getItem(tab, invenIndex);
		if(!pItem)
			return ;
		if(!pItem->IsRareItem())
			return ;

		pItem->m_nOption = 0;
		pItem->IdentifyRareOption();

		ch->m_inventory.sendOneItemInfo(pItem);

		{
			CNetMsg::SP rmsg(new CNetMsg);
			ResponseClient::ItemNotUseMsg(rmsg, MSG_ITEM_USE_ERROR_IDENTIFY_RARE_OK);
			SEND_Q(rmsg, ch->m_desc);
		}

		ch->CalcStatus(true);
	}
	else if(strcmp(tmpBuf, "mf") == 0)
	{
		unsigned char row1, col1, row2, col2, row3, col3, grade = 0;
		int count;
		if(!arg || !(*arg)) return;
		arg = AnyOneArg(arg, tmpBuf, true);
		row1 = atoi(tmpBuf);
		if(!arg || !(*arg)) return;
		arg = AnyOneArg(arg, tmpBuf, true);
		col1 = atoi(tmpBuf);

		if(!arg || !(*arg)) return;
		arg = AnyOneArg(arg, tmpBuf, true);
		row2 = atoi(tmpBuf);
		if(!arg || !(*arg)) return;
		arg = AnyOneArg(arg, tmpBuf, true);
		col2 = atoi(tmpBuf);
		if(!arg || !(*arg)) return;
		arg = AnyOneArg(arg, tmpBuf, true);
		count = atoi(tmpBuf);

		if(!arg || !(*arg)) return;
		arg = AnyOneArg(arg, tmpBuf, true);
		row3 = atoi(tmpBuf);
		if(!arg || !(*arg)) return;
		arg = AnyOneArg(arg, tmpBuf, true);
		col3 = atoi(tmpBuf);

		{
			CNetMsg::SP rmsg(new CNetMsg);
			rmsg->Init(MSG_EXTEND);
			RefMsg(rmsg) << MSG_EX_REFORMER_SYSTEM << (unsigned char)MSG_EX_REFORMER_SYSTEM_REQ << grade << row1 << col1 << row2 << col2 << count << row3 << col3;
			do_Extend(ch, rmsg);
		}
	}
	else if(strcmp(tmpBuf, "hf") == 0)
	{
		unsigned char row1, col1, row2, col2, row3, col3, grade = 1;
		int count;
		if(!arg || !(*arg)) return;
		arg = AnyOneArg(arg, tmpBuf, true);
		row1 = atoi(tmpBuf);
		if(!arg || !(*arg)) return;
		arg = AnyOneArg(arg, tmpBuf, true);
		col1 = atoi(tmpBuf);

		if(!arg || !(*arg)) return;
		arg = AnyOneArg(arg, tmpBuf, true);
		row2 = atoi(tmpBuf);
		if(!arg || !(*arg)) return;
		arg = AnyOneArg(arg, tmpBuf, true);
		col2 = atoi(tmpBuf);
		if(!arg || !(*arg)) return;
		arg = AnyOneArg(arg, tmpBuf, true);
		count = atoi(tmpBuf);

		if(!arg || !(*arg)) return;
		arg = AnyOneArg(arg, tmpBuf, true);
		row3 = atoi(tmpBuf);
		if(!arg || !(*arg)) return;
		arg = AnyOneArg(arg, tmpBuf, true);
		col3 = atoi(tmpBuf);

		{
			CNetMsg::SP rmsg(new CNetMsg);
			rmsg->Init(MSG_EXTEND);
			RefMsg(rmsg) << MSG_EX_REFORMER_SYSTEM << (unsigned char)MSG_EX_REFORMER_SYSTEM_REQ << grade << row1 << col1 << row2 << col2 << count << row3 << col3;
			do_Extend(ch, rmsg);
		}
	}
#endif // LC_KOR
	else if(strcmp(tmpBuf, "del_item") == 0)
	{
		int item_index = 0;
		if(!arg || !(*arg)) return;
		arg = AnyOneArg(arg, tmpBuf, true);
		item_index = atoi(tmpBuf);
		if (item_index < 0)
			return;
		// Cell 2개 정도에서
		int sx = ch->m_cellX - 2;
		int ex = ch->m_cellX + 2;
		int sz = ch->m_cellZ - 2;
		int ez = ch->m_cellZ + 2;

		int x, z;
		CItem* item;
		CItem* nItem;
		for (x = sx; x <= ex; x++)
		{
			if (x < 0 || x >= ch->m_pArea->m_size[0])
				continue;
			for (z = sz; z <= ez; z++)
			{
				if (z < 0 || z >= ch->m_pArea->m_size[1])
					continue;

				nItem = ch->m_pArea->m_cell[x][z].m_listItem;

				while ((item = nItem))
				{
					nItem = nItem->m_pCellNext;
					if (item_index == 0)
					{
						item->m_groundPulse = 0;
					}
					else if (item->getDBIndex() == item_index)
					{
						item->m_groundPulse = 0;
					}
				}
			}
		}
	}
	else if(strcmp(tmpBuf, "not_del_item") == 0)
	{
		if(!arg || !(*arg))
			return;

		arg = AnyOneArg(arg, tmpBuf, true);
		int item_index = atoi(tmpBuf);

		if (item_index < 0)
			return;

		// Cell 2개 정도에서
		int sx = ch->m_cellX - 2;
		int ex = ch->m_cellX + 2;
		int sz = ch->m_cellZ - 2;
		int ez = ch->m_cellZ + 2;

		int x, z;
		CItem* item;
		CItem* nItem;
		for (x = sx; x <= ex; x++)
		{
			if (x < 0 || x >= ch->m_pArea->m_size[0])
				continue;
			for (z = sz; z <= ez; z++)
			{
				if (z < 0 || z >= ch->m_pArea->m_size[1])
					continue;

				nItem = ch->m_pArea->m_cell[x][z].m_listItem;

				while ((item = nItem))
				{
					nItem = nItem->m_pCellNext;
					if (item_index == 0)
					{
						item->m_groundPulse = 0;
					}
					else if (item->getDBIndex() != item_index)
					{
						item->m_groundPulse = 0;
					}
				}
			}
		}
	}
	else if(strcmp(tmpBuf, "kill_npc") == 0)
	{
		int distance = 0;
		if(!arg || !(*arg)) return;
		arg = AnyOneArg(arg, tmpBuf, true);
		distance = atoi(tmpBuf);
		if (distance <= 0)
			return;

		// Cell 2개 정도에서
		int sx = ch->m_cellX - 2;
		int ex = ch->m_cellX + 2;
		int sz = ch->m_cellZ - 2;
		int ez = ch->m_cellZ + 2;

		int x, z;

		for (x = sx; x <= ex; x++)
		{
			if (x < 0 || x >= ch->m_pArea->m_size[0])
				continue;

			for (z = sz; z <= ez; z++)
			{
				if (z < 0 || z >= ch->m_pArea->m_size[1])
					continue;

				CCharacter* tch;;
				CCharacter* tchNext = ch->m_pArea->m_cell[x][z].m_listChar;
				while ((tch = tchNext))
				{
					tchNext = tchNext->m_pCellNext;

					// npc이고
					if (IS_NPC(tch))
					{
						CNPC* tnpc = TO_NPC(tch);

						if( tnpc->GetOwner() )
							continue;

						if( tnpc->m_proto->m_index == 491 ||				// 바닥데미지 삭제 불가
								( tnpc->m_flag & NPC_WARCASTLE ) )				// 공성 NPC 삭제 불가
							continue;

#ifdef EXTREME_CUBE
						switch(tnpc->m_proto->m_index)
						{
						case 526:
						case 527:
						case 528:
						case 529:
						case 530:
							continue;
							break;
						}
#endif // EXTREME_CUBE

						// 웜의 둥지
						if(tnpc->m_proto->m_index == 1152)
							continue;

						// npc와 해당거리내에 있으면
						if (GetDistance(ch, tnpc) < distance)
						{
							ApplyHateByDamage(ch, tnpc, 16, tnpc->m_hp);
							ApplyDamage(ch, tnpc, MSG_DAMAGE_MELEE, NULL, NULL, tnpc->m_hp , 16, false);
							ProcDead(tnpc, ch);
						}
					}
				}
			}
		}
	}

#ifdef DEV_EVENT_AUTO
	else if(strcmp(tmpBuf, "heart") == 0)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		rmsg->Init(MSG_EVENT);
		RefMsg(rmsg) << (unsigned char)MSG_EVENT_KB_REWARD
					 << (unsigned char)0;
		do_Event(ch, rmsg);
	}
	else if(strcmp(tmpBuf, "flag") == 0)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		rmsg->Init(MSG_EVENT);
		RefMsg(rmsg) << (unsigned char)MSG_EVENT_KB_REWARD
					 << (unsigned char)1;
		do_Event(ch, rmsg);
	}
#endif // DEV_EVENT_AUTO
	else if (strcmp(tmpBuf, "fc") == 0)
	{
		unsigned short tab = -1;
		unsigned short invenIndex = -1;
		if(!arg || !(*arg)) return;
		arg = AnyOneArg(arg, tmpBuf, true);
		tab = atoi(tmpBuf);
		if(!arg || !(*arg)) return;
		arg = AnyOneArg(arg, tmpBuf, true);
		invenIndex = atoi(tmpBuf);

		CNetMsg::SP rmsg(new CNetMsg);
		insideServer_do_Item_Make_Fortuen(rmsg, tab, invenIndex);
		do_Item(ch, rmsg);
	}
	else if (strcmp(tmpBuf, "del_fc") == 0)
	{
		char tab = -1;
		char row = -1;
		char col = -1;
		if(!arg || !(*arg)) return;
		arg = AnyOneArg(arg, tmpBuf, true);
		tab = atoi(tmpBuf);
		if(!arg || !(*arg)) return;
		arg = AnyOneArg(arg, tmpBuf, true);
		row = atoi(tmpBuf);
		if(!arg || !(*arg)) return;
		arg = AnyOneArg(arg, tmpBuf, true);
		col = atoi(tmpBuf);

		CItem* pSuit = ch->m_inventory.getItem(tab, (row * ITEMS_PER_ROW + col));

		if (pSuit == NULL)
			return ;

		if(!pSuit->Is_fortune_item())
			return ;
		if(!pSuit->getWearPos() == 1)
			return ;
		pSuit->setPlus(0);

		ch->m_inventory.sendOneItemInfo(pSuit);
	}
	else if (strcmp(tmpBuf, "print_fc") == 0)
	{
		int item_idx;
		if (!arg || !(*arg)) return;
		arg = AnyOneArg(arg, tmpBuf, true);
		item_idx = atoi(tmpBuf);
		Fortune_head *f = gserver->m_Fortunelist.Get_fortune_head_proto(item_idx);
		if (!f)
			return ;
		std::vector<Fortune_data*>::iterator itr;
		std::vector<Fortune_data*>::iterator itrEnd = f->Get_Fortune_data().end();
		Fortune_data* p;

		{
			CNetMsg::SP rmsg(new CNetMsg);
			strcpy(tmpBuf, "Print Fortune data <Skill Index : Skill Level : Fortune Prob>");
			strcat(tmpBuf, " Item:");
			IntCat(tmpBuf, item_idx);
			SayMsg(rmsg, MSG_CHAT_GM, 0, "", "", tmpBuf);
			SEND_Q(rmsg, ch->m_desc);
		}

		for (itr = f->Get_Fortune_data().begin(); itr != itrEnd; itr++)
		{
			p = *itr;
			if (p)
			{
				strcpy(tmpBuf, "<");
				IntCat(tmpBuf, p->Get_fortune_data_skill_index());
				strcat(tmpBuf, ":");
				IntCat(tmpBuf, p->Get_fortune_data_skill_level());
				strcat(tmpBuf, ":");
				IntCat(tmpBuf, p->Get_fortune_data_prob());
				strcat(tmpBuf, ">");
				CNetMsg::SP rmsg(new CNetMsg);
				SayMsg(rmsg, MSG_CHAT_GM, 0, "", "", tmpBuf);
				SEND_Q(rmsg, ch->m_desc);
			}
		}
	}
}

void do_GMEchoNpc(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	if (!arg || !(*arg))		return ;

	char tmpBuf[MAX_MESSAGE_SIZE] = {0,};
	arg = AnyOneArg(arg, tmpBuf, true);

	// 0: zone, 1: server, 2: all, 3: subserver
	int type;

	if (strcmp(tmpBuf, GM_CMD_ECHO_ZONE) == 0)		type = 0;
	else if (strcmp(tmpBuf, GM_CMD_ECHO_SERVER) == 0)	type = 1;
	else if (strcmp(tmpBuf, GM_CMD_ECHO_ALL) == 0)		type = 2;
	else if (strcmp(tmpBuf, GM_CMD_ECHO_SUBSERVER) == 0)	type = 3;
	else if (strcmp(tmpBuf, GM_CMD_ECHO_ALLZONE) == 0)	type = 4;
	else return ;

	// npc index
	int npcIndex = 0;		// 0 이면 주변 모든 NPC, -1 주변 평화, -2 주변 평화X, 특정 index
	arg = AnyOneArg(arg, tmpBuf, true);
	npcIndex = atoi(tmpBuf);

	arg = SkipSpaces(arg);
	if (!arg || !(*arg))		return ;

	{
		CNetMsg::SP rmsg(new CNetMsg);
		switch (type)
		{
		case 0:
			MsgrNpcNoticeMsg(rmsg, -1, gserver->m_serverno, gserver->m_subno, ch->m_pZone->m_index, npcIndex, arg);
			break;
		case 1:
			MsgrNpcNoticeMsg(rmsg, -1, gserver->m_serverno, -1, -1,npcIndex, arg);
			break;
		case 2:
			MsgrNpcNoticeMsg(rmsg, -1, -1, -1, -1, npcIndex, arg);
			break;
		case 3:
			MsgrNpcNoticeMsg(rmsg, -1, gserver->m_serverno, gserver->m_subno, -1, npcIndex, arg);
			break;
		case 4:
			MsgrNpcNoticeMsg(rmsg, -1, -1, -1, ch->m_pZone->m_index, npcIndex, arg);
			break;
		}
		SEND_Q(rmsg, gserver->m_messenger);
	}
}

void do_GMCastleStateCync(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
#ifdef WARCASTLE_STATE_CHANNEL_CYNC
	CNetMsg::SP rmsg(new CNetMsg);
	HelperWarCastleStateCync(rmsg);
	SEND_Q(rmsg, gserver->m_helper);
#endif
}

void do_GMTreasureMapDrop(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
#ifdef SYSTEM_TREASURE_MAP
	if (!arg || !(*arg))
		return ;

	char tmpBuf[MAX_MESSAGE_SIZE] = {0,};
	arg = AnyOneArg(arg, tmpBuf, true);

	if( strcmp(tmpBuf, "set" ) == 0 )
	{
		arg = AnyOneArg(arg, tmpBuf, true);
		int target = -1;
		if( strcmp(tmpBuf, "all" ) == 0 )
		{
			target = -1;
		}
		else if(strcmp(tmpBuf, "cur" ) == 0 )
			target = gserver->m_serverno;
		else
			return;

		{
			CNetMsg::SP rmsg(new CNetMsg);
			MsgrSetTreasureMap( rmsg, target, ch->m_index, arg );
			SEND_Q( rmsg, gserver->m_messenger);
		}
	}
	else if( strcmp(tmpBuf, "ing" ) == 0 )
	{
		CLCString str;
		if( gserver->bTreasureMapLinkZoneDrop  )
			str = "link Zone DROP : TRUE";
		else
			str = "link Zone DROP : FALSE";

		{
			CNetMsg::SP rmsg(new CNetMsg);
			SayMsg(rmsg, MSG_CHAT_GM, 0, "", "", str);
			SEND_Q(rmsg, ch->m_desc);
		}

		str="Drop On [ ";

		int i=0, j=0;;
		for(i=0; i<gserver->m_numZone; i++)
		{
			CZone * pZone = gserver->m_zones+i;
			if( !pZone)
				continue;
			if( !pZone->IsTreasureDropZone() )
				continue;

			if( !pZone->CheckTreasureDropFlag() )
				continue;

			CLCString temp;
			temp.Format("%d", pZone->m_index);
			str += temp;
			str += " ";
			j++;
			if( j !=0 && j%10==0 )
			{
				CNetMsg::SP rmsg(new CNetMsg);
				SayMsg(rmsg, MSG_CHAT_GM, 0, "", "", str);
				SEND_Q(rmsg, ch->m_desc);
				str = "";
				j=0;
			}
		}
		str+="]";

		{
			CNetMsg::SP rmsg(new CNetMsg);
			SayMsg(rmsg, MSG_CHAT_GM, 0, "", "", str);
			SEND_Q(rmsg, ch->m_desc);
		}
	}
#endif
}

void do_GMDeathEvent(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
}

void do_GMAttendanceChange(CPC* ch, const char* arg, std::vector<std::string>& vec)
{

	int attentDay; // 출석일
	int attentMax; // 최대 출석일
	int rewardPoint; // 보상받은날짜. ( 50 / 100 / 150 / 200 / 250 / 300 / 365 )

	if (!arg || !(*arg))
	{
		CNetMsg::SP rmsg(new CNetMsg);
		SayMsg(rmsg, MSG_CHAT_GM, 0, "", "", "First arg empty...");
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	char tmpBuf[MAX_MESSAGE_SIZE] = {0,};
	arg = AnyOneArg(arg, tmpBuf);
	attentDay = atoi(tmpBuf);

	if (!arg || !(*arg))
	{
		CNetMsg::SP rmsg(new CNetMsg);
		SayMsg(rmsg, MSG_CHAT_GM, 0, "", "", "Second arg empty...");
		SEND_Q(rmsg, ch->m_desc);
		return;
	}
	arg = AnyOneArg(arg, tmpBuf);
	attentMax = atoi(tmpBuf);

	if (!arg || !(*arg))
	{
		CNetMsg::SP rmsg(new CNetMsg);
		SayMsg(rmsg, MSG_CHAT_GM, 0, "", "", "Third arg empty...");
		SEND_Q(rmsg, ch->m_desc);
		return;
	}
	arg = AnyOneArg(arg, tmpBuf);
	rewardPoint = atoi(tmpBuf);

	if ( attentDay < 0 || attentMax < 0 || rewardPoint < 0 )
	{
		LOG_ERROR("Input Data ERROR. attentDay[%d], attentMax[%d], rewardPoint[%d]", attentDay, attentMax, rewardPoint);
		return;
	}

	const int MAXCOUNT = 3650;

	if( attentDay < 0 || attentDay > MAXCOUNT )
	{
		LOG_ERROR("Input Data ERROR. attentDay[%d], MAXCOUNT[%d]", attentDay, MAXCOUNT);
		return;
	}

	{
		{
			std::string query = boost::str(boost::format("DELETE from t_attendance_exp_system where a_char_index = %d") % ch->m_index);
			DBManager::instance()->pushQuery(0, query);
		}
		
		{
			std::string query = boost::str(boost::format("INSERT INTO t_attendance_exp_system ( a_char_index, a_last_date, a_acc_count, a_max_acc, a_last_reward_point ) VALUES ( %d, NOW(), %d, %d, 0 )") % ch->m_index % attentDay % attentMax);
			DBManager::instance()->pushQuery(0, query);
		}
		
		char	reward_exp[6] = { 0, 0 , 3, 4, 5, 10 };
		int		reward_idx = ch->m_attendanceManager.getAccCount() > 5 ? 5 : ch->m_attendanceManager.getAccCount();
		ch->m_attendanceManager.setAccCount(attentDay);
		ch->m_attendanceManager.setMaxAcc(attentMax);
		ch->m_attendanceManager.setLastRewardPoint(rewardPoint);
		ch->m_nAttendance_exp = reward_exp[reward_idx];

		CNetMsg::SP rmsg(new CNetMsg);
		UpdateClient::makeAttendanceInfo(rmsg, ch->m_attendanceManager.getAccCount(), ch->m_nAttendance_exp);
		SEND_Q(rmsg, ch->m_desc);
	}
}

void do_GMSet(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
}

void do_CraftPoint(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
}

void do_GMEventAutomation(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	if(!ch)
		return ;

	char tmpBuf[MAX_MESSAGE_SIZE] = {0,};

	arg = AnyOneArg(arg, tmpBuf, true);

	if(strcmp(tmpBuf, "on") == 0) // on
	{
		arg = AnyOneArg(arg, tmpBuf, true);
		int eventIndex;
		eventIndex = atoi(tmpBuf);

		CLCString str;//
		if( gserver->addActiveEvent( eventIndex, true ) )
		{
			bool notice=0;
			CEventInfo* eInfo = gserver->getEventInfo( eventIndex);
			if( eInfo && eInfo->getNotice() )
				notice = 1;

			CNetMsg::SP rmsg(new CNetMsg);
			MsgrEventActiveUpdateMsg(rmsg, MSG_EX_EVENT_AUTOMATION_ADD, eventIndex, notice );
			SEND_Q(rmsg, gserver->m_messenger);
			str.Format("EVENT ON SUCC : %d ", eventIndex);
			GAMELOG << init("EVENT_AUTOMATION") << "EVENT ON SUCCESS, EVENT ID : " << eventIndex << end;
		}
		else
		{
			str.Format("EVENT ON FAIL : %d ", eventIndex);
			GAMELOG << init("EVENT_AUTOMATION") << "EVENT ON FAILED, EVENT ID : " << eventIndex << end;
		}

		{
			CNetMsg::SP rmsg(new CNetMsg);
			SayMsg(rmsg, MSG_CHAT_GM, 0, "", "", str);
			SEND_Q(rmsg, ch->m_desc);
		}

		return;
	}
	else if(strcmp(tmpBuf, "off") == 0) // off
	{
		arg = AnyOneArg(arg, tmpBuf, true);
		int eventIndex;
		eventIndex = atoi(tmpBuf);

		CLCString str;
		if( gserver->removeActiveEvent( eventIndex, true ) )
		{
			CNetMsg::SP rmsg(new CNetMsg);
			MsgrEventActiveUpdateMsg(rmsg, MSG_EX_EVENT_AUTOMATION_REMOVE, eventIndex);
			SEND_Q(rmsg, gserver->m_messenger);

			str.Format("EVENT OFF SUCC : %d ", eventIndex);
			GAMELOG << init("EVENT_AUTOMATION") << "EVENT OFF SUCCESS, EVENT ID : " << eventIndex << end;
		}
		else
		{
			str.Format("EVENT OFF FAIL : %d ", eventIndex);
			GAMELOG << init("EVENT_AUTOMATION") << "EVENT OFF FAILED, EVENT ID : " << eventIndex << end;
		}

		{
			CNetMsg::SP rmsg(new CNetMsg);
			SayMsg(rmsg, MSG_CHAT_GM, 0, "", "", str);
			SEND_Q(rmsg, ch->m_desc);
		}

		return;
	}
	else if(strcmp(tmpBuf, "activelist") == 0) //list
	{
		std::map<int, CEventInfo*> * eventList = gserver->getActiveEvenList();
		CLCString str;

		if( !eventList || eventList->size() == 0 )
			str = "ACTIVE LIST : NONE";
		else
		{
			str = "ACTIVE LIST : ";
			CLCString strIndex;
			std::map<int, CEventInfo*>::iterator itr;
			std::map<int, CEventInfo*>::iterator itrEnd = eventList->end();
			for( itr=eventList->begin(); itr!=itrEnd; itr++)
			{
				strIndex.Format("%d ", itr->first );
				str += strIndex;
			}
		}

		{
			CNetMsg::SP rmsg(new CNetMsg);
			SayMsg(rmsg, MSG_CHAT_GM, 0, "", "", str);
			SEND_Q(rmsg, ch->m_desc);
		}
		return;
	}
	else if(strcmp(tmpBuf, "list") == 0) //list
	{
		std::map<int, CEventInfo> * eventList = gserver->getEvenList();
		CLCString str;

		if( !eventList || eventList->size() == 0 )
			str = "LIST : NONE";
		else
		{
			str = "LIST : ";
			CLCString strIndex;
			std::map<int, CEventInfo>::iterator itr;
			std::map<int, CEventInfo>::iterator itrEnd = eventList->end();
			for( itr=eventList->begin(); itr!=itrEnd; itr++)
			{
				strIndex.Format("%d ", itr->first );
				str += strIndex;
			}
		}

		{
			CNetMsg::SP rmsg(new CNetMsg);
			SayMsg(rmsg, MSG_CHAT_GM, 0, "", "", str);
			SEND_Q(rmsg, ch->m_desc);
		}
		return;
	}
	else if(strcmp(tmpBuf, "reward") == 0) //list
	{
		arg = AnyOneArg(arg, tmpBuf, true);
		int rewardIndex = atoi(tmpBuf);

		CLCString str;

		if( !gserver->m_rewardMgr.setRewardData(ch, rewardIndex)
				|| !gserver->m_rewardMgr.doReward(ch, rewardIndex) )
			str.Format("Reward System FAIL : %d ", rewardIndex);
		else
			str.Format("Reward System SUCC : %d ", rewardIndex);

		{
			CNetMsg::SP rmsg(new CNetMsg);
			SayMsg(rmsg, MSG_CHAT_GM, 0, "", "", str);
			SEND_Q(rmsg, ch->m_desc);
		}

		return;
	}
}

void do_GMSkillLearn(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	if ( !ch )
		return;

	char tmpBuf[MAX_MESSAGE_SIZE] = {0,};

	arg = AnyOneArg(arg, tmpBuf, true);

	if ( !strcmp(tmpBuf, "thanks") || !strcmp(tmpBuf, "thankyou") )
		gserver->m_skillProtoList.LearnAllSkill(ch);
}

void do_GMApetHungry(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	if ( !ch || !ch->GetAPet() )
		return;

	char tmpBuf[MAX_MESSAGE_SIZE] = {0,};

	arg = AnyOneArg(arg, tmpBuf, true);

	int _hungry = atoi(tmpBuf);
	CAPet* pPet = ch->GetAPet();
	if (pPet == NULL)
		return;

	if ( _hungry > pPet->m_pProto->m_nMaxStm )
		_hungry = pPet->m_pProto->m_nMaxStm;

	pPet->SetStamina(_hungry);
}

void do_GMApetFaith(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	if ( !ch || !ch->GetAPet() )
		return;

	char tmpBuf[MAX_MESSAGE_SIZE] = {0,};

	arg = AnyOneArg(arg, tmpBuf, true);

	int _faith = atoi(tmpBuf);
	CAPet* pPet = ch->GetAPet();
	if (pPet == NULL)
		return;

	if ( _faith > pPet->m_pProto->m_nMaxFaith )
		_faith = pPet->m_pProto->m_nMaxFaith;

	pPet->SetFaith(_faith);
}

void do_GMCompareBoxItem(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	CLCString str(512);

	char tmpBuf[MAX_MESSAGE_SIZE] = {0,};
	arg = AnyOneArg(arg, tmpBuf, true);

	int index = atoi(tmpBuf);
	if ( index <= 0 || index > gserver->m_LuckyDrawBox.GetCount() )
	{
		str.Format("Invalid box index...");
		CNetMsg::SP rmsg(new CNetMsg);
		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", str);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	LuckyDrawBoxData* pData = gserver->m_LuckyDrawBox.GetData(index);
	if ( pData == NULL )
	{
		// error
		str.Format("not exist box...");
		CNetMsg::SP rmsg(new CNetMsg);
		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", str);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	int count = pData->nResultCount;
	int existCount=0;
	char* exist;
	char* type;

	for(int i=0; i<count; ++i)
	{
		int rewardIndex = pData->resultList[i].item_idx;
		CItemProto* itemProto = gserver->m_itemProtoList.FindIndex(rewardIndex);
		bool bSuccess = false;
		if ( itemProto != NULL )
			bSuccess = true;

		if(static_cast<int>(bSuccess) == 1)
		{
			exist = "exist";
			existCount++;
		}
		else
			exist = "not exist";

		if(pData->random == 0)
			type = "prob";
		else if(pData->random == 1)
			type = "random";
		else if(pData->random == 2)
			type = "All";

		{
			str.Format("[BoxIndex : %d]  [rewardIndex : %d]  [isExist : %s]  [type : %s]  [prob : %d]", index, rewardIndex, exist, type, pData->resultList[i].prob);
			CNetMsg::SP rmsg(new CNetMsg);
			SayMsg(rmsg, MSG_CHAT_SAY, 0, "", "", str);
			SEND_Q(rmsg, ch->m_desc);
		}
	}

	{
		str.Format("[existCount : %d]", existCount);
		CNetMsg::SP rmsg(new CNetMsg);
		SayMsg(rmsg, MSG_CHAT_SAY, 0, "", "", str);
		SEND_Q(rmsg, ch->m_desc);
	}
}

void do_GMGuildMasterKickReset(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	if ( ch == NULL )
		return;

	if ( ch->m_guildInfo == NULL || ch->m_guildInfo->guild() == NULL )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Reset Fail : Guild join, please");
		SEND_Q(rmsg, ch->m_desc);
		return;
	}
	if ( ch->m_guildInfo->guild()->boss()->charindex() == ch->m_index )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Reset Fail : Boss can't Reset, Try again Another Character");
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	int guildIndex = ch->m_guildInfo->guild()->index();
	if ( guildIndex <= 0 )
		return;

	if ( gserver->isRunHelper() )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildMasterKickReset(rmsg, ch->m_index, guildIndex);
		SEND_Q(rmsg, gserver->m_helper);
	}
	else
	{
		CNetMsg::SP rmsg(new CNetMsg);
		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Reset Fail : Helper is not running");
		SEND_Q(rmsg, ch->m_desc);
	}
}

void do_GMGuildChangeBoss(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	if ( ch == NULL )
		return;

	if ( ch->m_guildInfo == NULL || ch->m_guildInfo->guild() == NULL )
		return;

	CGuild* pGuild = ch->m_guildInfo->guild();
	if ( pGuild == NULL )
		return;

	if ( pGuild->boss()->charindex() == ch->m_index )
		return;

	if ( gserver->isRunHelper() )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperGuildMasterChange(rmsg, ch->m_index, pGuild->index());
		SEND_Q(rmsg, gserver->m_helper);
	}
}

void do_GMJobReset(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	ch->InitSkill(&ch->m_activeSkillList);
	ch->InitSkill(&ch->m_passiveSkillList, true);
	ch->InitSSkill();
	if( ch->m_job != JOB_NIGHTSHADOW )
		ch->m_job2 = 0;
	ch->InitStat();

	ch->CalcStatus(true);

	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::makeSkillListMsg(rmsg, ch);
		SEND_Q(rmsg, ch->m_desc);
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::makeSSkillList(rmsg, ch);
		SEND_Q(rmsg, ch->m_desc);
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::makeChangeJobReset(rmsg, ch);
		SEND_Q(rmsg, ch->m_desc);
	}
}

void do_GMJobChange(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	int		iJob	= 0;
	int		iJob2nd	= 0;

	if (!arg || !(*arg))		return ;

	char tmpBuf[MAX_MESSAGE_SIZE] = {0,};
	arg		= AnyOneArg(arg, tmpBuf);
	iJob	= atoi(tmpBuf);
	if (arg && (*arg))
	{
		arg		= AnyOneArg(arg, tmpBuf);
		iJob2nd	= atoi(tmpBuf);
	}

	if ((iJob == JOB_NIGHTSHADOW) && (iJob2nd != 1))
		return;

	if(iJob < JOB_TITAN || iJob > JOBCOUNT)
		return;

	ch->m_job	= iJob;
	if(iJob2nd >= 0 && iJob2nd <= 2)
		ch->m_job2		= iJob2nd;

	ch->InitSkill(&ch->m_activeSkillList);
	ch->InitSkill(&ch->m_passiveSkillList, true);
	ch->InitSSkill();
	ch->InitStat();
	ch->CalcStatus(true);

	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::makeSkillListMsg(rmsg, ch);
		SEND_Q(rmsg, ch->m_desc);
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::makeSSkillList(rmsg, ch);
		SEND_Q(rmsg, ch->m_desc);
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		StatPointRemainMsg(rmsg, ch);
		SEND_Q(rmsg, ch->m_desc);
	}

	CItem* itemWear;
	for(int i = 0; i < COSTUME2_WEARING_BACK; i++)	// WEARING_HELMET to COSTUME2_WEARING_BACK
	{
		itemWear	= ch->m_wearInventory.wearItemInfo[i];
		// 벗기기
		if(itemWear)
		{
			//나한테 벗기
			ch->m_wearInventory.DelNormalItem(i);
			{
				// 남들한테 벗기기
				CNetMsg::SP rmsg(new CNetMsg);
				WearingMsg(rmsg, ch, i, -1, 0);
				ch->m_pArea->SendToCell(rmsg, ch, false);
			}
		}
	}

	CDBCmd dbChar;
	dbChar.Init(&gserver->m_dbchar);
	char update_character_query[MAX_MESSAGE_SIZE] = "";
	sprintf(update_character_query, "UPDATE t_characters SET a_job = %d WHERE a_index = %d", (int)ch->m_job, ch->m_index);
	dbChar.SetQuery( update_character_query );
	if (!dbChar.Update())
	{
		DBLOG	<< init("DB ERROR : UPDATE CHARACTER", ch)
				<< mysql_error(dbChar.m_dbconn) << delim
				<< "JOB" << delim << ch->m_job
				<< end;
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::makeChangeJob(rmsg, ch->m_index, ch->m_job, ch->m_job2);
		ch->m_pArea->SendToCell(rmsg, ch, true);
	}
}

void do_GMItemGet(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	int dbindex, plus = 0, flag = 0;
	LONGLONG count = 1;

	int		optionCount = 0;
	boost::scoped_array<int>	optionType;
	boost::scoped_array<int>	optionLevel;

	if (!arg || !(*arg))		return ;

	char tmpBuf[MAX_MESSAGE_SIZE] = {0,};
	arg = AnyOneArg(arg, tmpBuf);
	dbindex = atoi(tmpBuf);

	if(dbindex <= 0 || dbindex == NAS_ITEM_DB_INDEX)
		return;

	if (arg && (*arg))
	{
		arg = AnyOneArg(arg, tmpBuf);
		plus = atoi(tmpBuf);
	}
	if (arg && (*arg))
	{
		arg = AnyOneArg(arg, tmpBuf);
		flag = atoi(tmpBuf);
	}
	if (arg && (*arg))
	{
		arg = AnyOneArg(arg, tmpBuf);
		count = ATOLL(tmpBuf);
	}
	int socketcount = 0;
	if (arg && (*arg))
	{
		arg = AnyOneArg(arg, tmpBuf);
		socketcount = atoi(tmpBuf);
	}

	if (count == 0)
		count = 1;

	CItem* item = gserver->m_itemProtoList.CreateItem(dbindex, -1, plus, flag, count);

	if (item == NULL)
		return ;

	if (socketcount > 0 && item->m_itemProto->getItemFlag() & ITEM_FLAG_SOCKET)
	{
		if(item->m_socketList.SetSocketCountForAdmin(socketcount))
		{
			item->setFlag(item->getFlag() | FLAG_ITEM_SOCKET);
		}
	}

	if( item->getDBIndex() == 2148 && gserver->isActiveEvent( A_EVENT_EGGS_HUNT) )
		item->setUsed(1);	// 1: 운영자가 떨어뜨린 것, -1: 몬스터가 떨어뜨린 것

	// itemdrop <index> <plus> <flag> <count> <option_count> <option_type> <option_level>
	if (arg && (*arg))
	{
		arg = AnyOneArg(arg, tmpBuf);
		optionCount = atoi(tmpBuf);

		bool bIsRare = false;

		if (item->IsRareItem())
			bIsRare = true;

		if (bIsRare)
		{
			if (optionCount == 1)
			{
				if (!arg || !(*arg))
				{
					delete item;
					return ;
				}
				arg = AnyOneArg(arg, tmpBuf);
				int nRareIndex = atoi(tmpBuf);

				if (!arg || !(*arg))
				{
					delete item;
					return ;
				}
				arg = AnyOneArg(arg, tmpBuf);
				int nRareBit = atoi(tmpBuf);

				const CRareOptionProto* pProto = gserver->m_rareOptionList.Find(nRareIndex);
				if (pProto)
				{
					int i;
					for (i = 0; i < MAX_RARE_OPTION_SETTING; i++)
					{
						const RAREOPTIONDATA* pROD = pProto->GetData(i);
						if (pROD->optionlevel == 0)
						{
							nRareBit &= ~pROD->rarebit;
						}
					}
					if (nRareBit != 0)
					{
						item->m_nOption = 2;
						item->m_option[0].GetDBValue((short)nRareIndex);
						item->m_option[1].GetDBValue((short)nRareBit);
						item->m_pRareOptionProto = pProto;
						item->m_nRareOptionIndex = nRareIndex;
						item->m_nRareOptionBit = nRareBit;
					}
				}
			}
			else
			{
				delete item;
				return ;
			}
		}
		else
		{
			// 옵션 부분 추가
			if (optionCount > 0 && optionCount <= MAX_ITEM_OPTION)
			{
				optionType.reset(new int[optionCount]);
				optionLevel.reset(new int[optionCount]);

				int i;
				for (i=0; i < optionCount; i++)
				{
					if (!arg || !(*arg))
					{
						delete item;
						return ;
					}
					arg = AnyOneArg(arg, tmpBuf);
					optionType[i] = atoi(tmpBuf);

					if (!arg || !(*arg))
					{
						delete item;
						return ;
					}
					arg = AnyOneArg(arg, tmpBuf);
					optionLevel[i] = atoi(tmpBuf);

					if (optionType[i] < 0 || optionType[i] >= MAX_NUM_OPTION || optionLevel[i] < 1 )
						continue;

					// Option 붙이기
					COptionProto* proto = gserver->m_optionProtoList.FindProto(optionType[i]);

					if (!proto)
						continue;

					if( (int) proto->m_levelValue.size() < optionLevel[i] )
						continue;

					item->m_option[item->m_nOption].m_proto = proto;
					item->m_option[item->m_nOption].m_type = optionType[i];
					item->m_option[item->m_nOption].m_level = optionLevel[i];
					item->m_option[item->m_nOption].m_value = proto->m_levelValue[optionLevel[i] - 1];
					item->m_option[item->m_nOption].SetDBValue();
					++item->m_nOption;
				}
			}
		}

		if (item->IsOriginItem())
		{
			item->m_pSetItemProto = gserver->m_setItemProtoList.Find(item->m_itemProto->GetSetIndex());
		}
	}

	int bArtifact = false;

	if(item->m_itemProto->getItemTypeIdx() == ITYPE_ACCESSORY && item->m_itemProto->getItemSubTypeIdx() == IACCESSORY_ARTIFACT)
	{
		if(ArtifactManager::instance()->getCount() >= ARTIFACT_MAX_ITEM_COUNT)
		{
			return ;
		}
		bArtifact = true;
	}
	
	if (item->IsAccessary() && item->m_nOption == 0)
	{
		OptionSettingItem(ch, item);
	}

	if (ch->m_inventory.addItem(item) == false)
	{
		delete item;

		CNetMsg::SP rmsg(new CNetMsg);
		SysFullInventoryMsg(rmsg, 0);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	addItemResult& addResult = ch->m_inventory.getLastAddItemResult();
	if (addResult.pItem)
	{
		GAMELOG << init("ITEM_LOAD", ch)
				<< itemlog(addResult.pItem)
				<< end;
	}

	if(item->m_itemProto->getItemTypeIdx() == ITYPE_ACCESSORY && item->m_itemProto->getItemSubTypeIdx() == IACCESSORY_ARTIFACT)
	{
		int error_ = ArtifactManager::instance()->checkValidUser(ch);
		if(error_ != 0)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			SysMsg(rmsg, (MSG_SYS_TYPE)error_);
			SEND_Q(rmsg, ch->m_desc);
			return;
		}
			ArtifactManager::instance()->addOwner(ch, item);
	}
}

void do_GMItemGetRange(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	int dbindex = 0, dbindex2 = 0, plus = 0, flag = 0;
	LONGLONG count = 1;

	int		optionCount = 0;
	boost::scoped_array<int>	optionType;
	boost::scoped_array<int>	optionLevel;

	if (!arg || !(*arg))		return ;

	char tmpBuf[MAX_MESSAGE_SIZE] = {0,};
	arg = AnyOneArg(arg, tmpBuf);
	dbindex = atoi(tmpBuf);

	if (!arg || !(*arg))		return ;
	if (arg && (*arg))
	{
		arg = AnyOneArg(arg, tmpBuf);
		dbindex2 = atoi(tmpBuf);
	}
	if (arg && (*arg))
	{
		arg = AnyOneArg(arg, tmpBuf);
		plus = atoi(tmpBuf);
	}
	if (arg && (*arg))
	{
		arg = AnyOneArg(arg, tmpBuf);
		flag = atoi(tmpBuf);
	}
	if (arg && (*arg))
	{
		arg = AnyOneArg(arg, tmpBuf);
		count = ATOLL(tmpBuf);
	}

	if (count == 0)
		count = 1;

	for(; dbindex <= dbindex2; dbindex++)
	{
		if (dbindex == NAS_ITEM_DB_INDEX) // nas는 발행하지 않음
			continue;

		CItem* item = gserver->m_itemProtoList.CreateItem(dbindex, -1, plus, flag, count);

		if (item == NULL)
			continue ;

		if( item->getDBIndex() == 2148 && gserver->isActiveEvent( A_EVENT_EGGS_HUNT) )
			item->setUsed(1);	// 1: 운영자가 떨어뜨린 것, -1: 몬스터가 떨어뜨린 것

		// itemdrop <index> <plus> <flag> <count> <option_count> <option_type> <option_level>
		if (arg && (*arg))
		{
			arg = AnyOneArg(arg, tmpBuf);
			optionCount = atoi(tmpBuf);

			bool bIsRare = false;

			if (item->IsRareItem())
				bIsRare = true;

			if (bIsRare)
			{
				if (optionCount == 1)
				{
					if (!arg || !(*arg))
					{
						delete item;
						return ;
					}
					arg = AnyOneArg(arg, tmpBuf);
					int nRareIndex = atoi(tmpBuf);

					if (!arg || !(*arg))
					{
						delete item;
						return ;
					}
					arg = AnyOneArg(arg, tmpBuf);
					int nRareBit = atoi(tmpBuf);

					const CRareOptionProto* pProto = gserver->m_rareOptionList.Find(nRareIndex);
					if (pProto)
					{
						int i;
						for (i = 0; i < MAX_RARE_OPTION_SETTING; i++)
						{
							const RAREOPTIONDATA* pROD = pProto->GetData(i);
							if (pROD->optionlevel == 0)
							{
								nRareBit &= ~pROD->rarebit;
							}
						}
						if (nRareBit != 0)
						{
							item->m_nOption = 2;
							item->m_option[0].GetDBValue((short)nRareIndex);
							item->m_option[1].GetDBValue((short)nRareBit);
							item->m_pRareOptionProto = pProto;
							item->m_nRareOptionIndex = nRareIndex;
							item->m_nRareOptionBit = nRareBit;
						}
					}
				}
				else
				{
					delete item;
					return ;
				}
			}
			else
			{
				// 옵션 부분 추가
				if (optionCount > 0 && optionCount <= MAX_ITEM_OPTION)
				{
					optionType.reset(new int[optionCount]);
					optionLevel.reset(new int[optionCount]);

					int i;
					for (i=0; i < optionCount; i++)
					{
						if (!arg || !(*arg))
						{
							delete item;
							return ;
						}
						arg = AnyOneArg(arg, tmpBuf);
						optionType[i] = atoi(tmpBuf);

						if (!arg || !(*arg))
						{
							delete item;
							return ;
						}
						arg = AnyOneArg(arg, tmpBuf);
						optionLevel[i] = atoi(tmpBuf);

						if (optionType[i] < 0 || optionType[i] >= MAX_NUM_OPTION || optionLevel[i] < 1 )
							continue;

						// Option 붙이기
						COptionProto* proto = gserver->m_optionProtoList.FindProto(optionType[i]);

						if (!proto)
							continue;

						if( (int) proto->m_levelValue.size() < optionLevel[i] )
							continue;

						item->m_option[item->m_nOption].m_proto = proto;
						item->m_option[item->m_nOption].m_type = optionType[i];
						item->m_option[item->m_nOption].m_level = optionLevel[i];
						item->m_option[item->m_nOption].m_value = proto->m_levelValue[optionLevel[i] - 1];
						item->m_option[item->m_nOption].SetDBValue();
						++item->m_nOption;
					}
				}
			}

			if (item->IsOriginItem())
			{
				item->m_pSetItemProto = gserver->m_setItemProtoList.Find(item->m_itemProto->GetSetIndex());
			}
		}

		if (ch->m_inventory.addItem(item))
		{
			GAMELOG << init("ITEM_LOAD", ch)
					<< itemlog(item)
					<< end;
		}
		else
		{
			delete item;

			CNetMsg::SP rmsg(new CNetMsg);
			SysFullInventoryMsg(rmsg, 0);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}
	}
}

void do_GMXMASPuzzle(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	if (!arg || !(*arg))
		return ;

	char tmpBuf[MAX_MESSAGE_SIZE] = {0,};
	arg = AnyOneArg(arg, tmpBuf);

	if (strncmp("changeprob", tmpBuf, 10) == 0)
	{
		arg = AnyOneArg(arg, tmpBuf);

		int level = atoi(tmpBuf);
		if (level > gserver->m_XmasRockPaperScissorsGame->GetTotalStage() || level <= 0)
			return;

		arg = AnyOneArg(arg, tmpBuf);

		int userProb = atoi(tmpBuf);
		if (userProb > 100 || userProb < 0)
			return;

		arg = AnyOneArg(arg, tmpBuf);

		int serverProb = atoi(tmpBuf);
		if (serverProb > 100 || serverProb < 0)
			return;

		int drawProb = 100 - userProb - serverProb;
		if (drawProb < 0)
			return;

		if (gserver->m_XmasRockPaperScissorsGame->ChangeProb(level, userProb, serverProb, drawProb) )
		{
			CNetMsg::SP rmsg(new CNetMsg);
			SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Chang Prob Success");
			SEND_Q(rmsg, ch->m_desc);
		}
	}
	else if (strncmp("ing", tmpBuf, 3) == 0)
	{
		for (int i=1 ; i <= gserver->m_XmasRockPaperScissorsGame->GetTotalStage() ; ++i)
		{
			const CRockPaperScissorsProb* prob = gserver->m_XmasRockPaperScissorsGame->GetProb(i);
			if (prob != NULL)
			{
				CLCString strProb(1024);
				strProb.Format("LEVEL(%d) : UserWin(%d%%), ServerWin(%d%%), Draw(%d%%)", i, prob->GetUserWinProb(), prob->GetServerWinProb(), prob->GetDrawProb());
				CNetMsg::SP rmsg(new CNetMsg);
				SayMsg(rmsg, MSG_CHAT_SAY, 0, "", "", strProb);
				SEND_Q(rmsg, ch->m_desc);
			}
		}
	}
}

void doGMRoyalRumbleSetting(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	/*
	* 조건 1. min은 4보다 큰 양수이어야 한다.
	* 조건 2. max는 min 보다 큰 양수이어야 한다.
	* 조건 3. max는 40보다 클 수 없다.
	* 조건 4. startHour는 0 ~ 23인 양수이어야 한다.
	* 명령어 규칙
	* /royalrumble <min> <max> <startHour>
	*/

	const int maxParam = 3;
	int nParam[3] = {0,};
	CLCString message(1024);

	char tmpBuf[MAX_MESSAGE_SIZE] = {0,};
	for(int i = 0; i < maxParam; i++)
	{
		if (!arg || !(*arg))
		{
			message.Format("RoyalRumble setting values is invalid.");
			CNetMsg::SP rmsg(new CNetMsg);
			SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", message);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}
		arg = AnyOneArg(arg, tmpBuf);
		if(!isdigit(tmpBuf[0])) return ;
		nParam[i] = atoi(tmpBuf);
	}
	if(CRoyalRumble::checkSettingValue(nParam[0], nParam[1], nParam[2]) != CRoyalRumble::kValueIsValid)
	{
		message.Format("RoyalRumble setting values is invalid.");
		CNetMsg::SP rmsg(new CNetMsg);
		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", message);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
	/*
	* 값에 이상이 없으니 메신져로 보낸다.
	*/
	{
		CNetMsg::SP rmsg(new CNetMsg);
		MsgrRoyalRumbleSettingReq(rmsg, nParam[0], nParam[1], nParam[2], ch->m_index);
		SEND_Q(rmsg, gserver->m_messenger);
	}

	return ;
}

void do_GMAffinitySet(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	CLCString message(1024);
	int nParam[2] = {0,};

	char tmpBuf[MAX_MESSAGE_SIZE] = {0,};
	for(int i=0; i<2; i++)
	{
		if (!arg || !(*arg))
		{
			message.Format("Affinity setting values is invalid.");
			CNetMsg::SP rmsg(new CNetMsg);
			SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", message);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}
		arg = AnyOneArg(arg, tmpBuf);
		if(!isdigit(tmpBuf[0])) return ;
		nParam[i] = atoi(tmpBuf);
	}
	CAffinity *affinity;
	affinity = ch->m_affinityList.FindAffinity(nParam[0]);

	if (affinity == NULL)
	{
		if (ch->m_affinityList.AddAffinity(nParam[0], nParam[1]) == false)
		{
			message.Format("Affinity setting values is invalid.");
			CNetMsg::SP rmsg(new CNetMsg);
			SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", message);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}
	}
	else
	{
		affinity->m_point = nParam[1];
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		AffinityUpdatePointMsg(rmsg, nParam[0], nParam[1], false);
		SEND_Q(rmsg, ch->m_desc);
	}
}

void do_GMAffinityRewardInit(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	CLCString message(1024);

	if (!arg || !(*arg))
	{
		message.Format("Affinity reward setting values is invalid.");
		CNetMsg::SP rmsg(new CNetMsg);
		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", message);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	CAffinity *affinity;

	if( !strcmp(arg, " all") )
	{
		for(int i=1; i<17; i++)
		{
			affinity = ch->m_affinityList.FindAffinity(i);
			if(affinity)
			{
				affinity->UpdateRewardStep(affinity->m_proto->m_affinityNPC->m_npcidx, 0);
			}
		}
	}
	else
	{
		affinity = ch->m_affinityList.FindAffinity(atoi(arg));
		if(!affinity)
		{
			message.Format("Affinity reward setting values is invalid.");
			CNetMsg::SP rmsg(new CNetMsg);
			SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", message);
			SEND_Q(rmsg, ch->m_desc);
			return ;
		}
		affinity->UpdateRewardStep(affinity->m_proto->m_affinityNPC->m_npcidx, 0);
	}
}

void do_GMNasSet(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	CLCString message(1024);

	if (!arg || !(*arg))
	{
		message.Format("Nas setting values is invalid.");
		CNetMsg::SP rmsg(new CNetMsg);
		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", message);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	GoldType_t moneyItem;

	moneyItem = ATOLL(arg);

	if(moneyItem < 0)
	{
		message.Format("Nas setting values is invalid.");
		CNetMsg::SP rmsg(new CNetMsg);
		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", message);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	{
		ch->m_inventory.setMoney(moneyItem);
	}
}

void do_GMJewelEvent(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	CLCString message(1024);

	if (!arg || !(*arg))
	{
		message.Format("Event setting values is invalid.");
		CNetMsg::SP rmsg(new CNetMsg);
		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", message);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	char tmpBuf[MAX_MESSAGE_SIZE] = {0,};
	arg = AnyOneArg(arg, tmpBuf);
	bool isAll = false;
	int kind = -1;
	if( strcmp(tmpBuf, "all") == 0)
		isAll = true;
	else
		kind = atoi(tmpBuf);

	arg = AnyOneArg(arg, tmpBuf);
	int eventData = atoi(tmpBuf);

	switch (kind)
	{
	case 0:
		{
			gserver->m_jewelDataList.m_jewelData->setEventChaosCreateProb(eventData);
			message.Format("ChaosCreate Event setting values is %d", eventData);
			CNetMsg::SP rmsg(new CNetMsg);
			SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", message);
			SEND_Q(rmsg, ch->m_desc);
		}
		break;
	case 1:
		{
			gserver->m_jewelDataList.m_jewelData->setEventComposeProb(eventData);
			message.Format("JewelCompose Event setting values is %d", eventData);
			CNetMsg::SP rmsg(new CNetMsg);
			SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", message);
			SEND_Q(rmsg, ch->m_desc);
		}
		break;
	case 2:
		{
			gserver->m_jewelDataList.m_jewelData->setEventUpgradePorb(eventData);
			message.Format("JewelUpgrade Event setting values is %d", eventData);
			CNetMsg::SP rmsg(new CNetMsg);
			SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", message);
			SEND_Q(rmsg, ch->m_desc);
		}
		break;
	case 3:
		{
			gserver->m_jewelDataList.m_jewelData->setEventCombineProb(eventData);
			message.Format("JewelCombine Event setting values is %d", eventData);
			CNetMsg::SP rmsg(new CNetMsg);
			SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", message);
			SEND_Q(rmsg, ch->m_desc);
		}
		break;
	case 4:
		{
			gserver->m_jewelDataList.m_jewelData->setEventCollectProb(eventData);
			message.Format("JewelCollect Event setting values is %d", eventData);
			CNetMsg::SP rmsg(new CNetMsg);
			SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", message);
			SEND_Q(rmsg, ch->m_desc);
		}
		break;
	}

	if( isAll )
	{
		gserver->m_jewelDataList.m_jewelData->setEventChaosCreateProb(eventData);
		gserver->m_jewelDataList.m_jewelData->setEventComposeProb(eventData);
		gserver->m_jewelDataList.m_jewelData->setEventUpgradePorb(eventData);
		gserver->m_jewelDataList.m_jewelData->setEventCombineProb(eventData);
		gserver->m_jewelDataList.m_jewelData->setEventCollectProb(eventData);
		message.Format("JewelALL Event setting values is %d", eventData);
		CNetMsg::SP rmsg(new CNetMsg);
		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", message);
		SEND_Q(rmsg, ch->m_desc);
	}
}

#include "DBManager.h"

#ifdef LC_KOR

extern void process_expressInputItem(CNetMsg::SP& msg);

void do_Crash(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	// 강제로 서버 크래쉬를 유발하는 함수
	char* p = NULL;

	*p = 0;
}
#endif

void do_NotGM(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	ch->m_admin = 0;

	CNetMsg::SP rmsg(new CNetMsg);
	GMWhoAmIMsg(rmsg, ch);
	SEND_Q(rmsg, ch->m_desc);
}

void do_GMTimerItemSet(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	if (vec.size() != 2)
		return;

	int type = atoi(vec[0].c_str());
	int sec = atoi(vec[1].c_str());
	ch->m_TimerItem.setTime(type, sec);
}

void do_PickupItem(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	int itemDBIndex = 0;
	if (vec.empty() == false)
	{
		itemDBIndex = atoi(vec[0].c_str());
	}

	std::vector<CItem*> vIndexvec;
	int sx = ch->m_cellX - 1;
	int ex = ch->m_cellX + 1;
	int sz = ch->m_cellZ - 1;
	int ez = ch->m_cellZ + 1;

	for (int x = sx; x <= ex; x++)
	{
		if (x < 0 || x >= ch->m_pArea->m_size[0])
			continue ;

		for (int z = sz; z <= ez; z++)
		{
			if (z < 0 || z >= ch->m_pArea->m_size[1])
				continue ;

			CItem* item = NULL;
			CItem* itemNext = ch->m_pArea->m_cell[x][z].m_listItem;
			while ((item = itemNext))
			{
				itemNext = item->m_pCellNext;

				if (itemDBIndex == 0)
				{
					vIndexvec.push_back(item);
				}
				else if (itemDBIndex == item->getDBIndex())
				{
					vIndexvec.push_back(item);
				}
			}
		}
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		RequestClient::doItemTake* packet = reinterpret_cast<RequestClient::doItemTake*>(rmsg->m_buf);
		rmsg->m_mtype = packet->type = MSG_ITEM;
		packet->subType = MSG_ITEM_TAKE;
		packet->char_type = MSG_CHAR_PC;
		packet->char_index = ch->m_index;

		BOOST_FOREACH(CItem* pItem, vIndexvec)
		{
			packet->virtualIndex = pItem->getVIndex();
			do_ItemTake(ch, rmsg);
		}
	}
}
//////////////////////////////////////////////////////////////////////////
extern float g_TradeAgent_Fee;
extern int g_TradeAgent_Deposit;

void do_TradeAgent_Fee(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	if (vec.size() != 0 && vec.size() != 1)
		return;

	if (vec.size() == 1)
	{
		g_TradeAgent_Fee = atof(vec[0].c_str());

		// subHelper로 전송하여 값을 변경하도록 함
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::TradeAgentChangeFee* packet = reinterpret_cast<ResponseClient::TradeAgentChangeFee*>(rmsg->m_buf);
		packet->type = MSG_TRADEAGENT;
		packet->subType = MSG_TRADEAGENT_CHANGE_FEE;
		packet->fee = g_TradeAgent_Fee;
		rmsg->setSize(sizeof(ResponseClient::TradeAgentChangeFee));
		SEND_Q(rmsg, gserver->m_subHelper);
	}

	CNetMsg::SP rmsg(new CNetMsg);
	std::string tstr = boost::str(boost::format("TradeAgent Fee : %1%") % g_TradeAgent_Fee);
	SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", tstr.c_str());
	SEND_Q(rmsg, ch->m_desc);
}

void do_TradeAgent_Deposit(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	if (vec.size() != 0 && vec.size() != 1)
		return;

	if (vec.size() == 1)
	{
		g_TradeAgent_Deposit = atoi(vec[0].c_str());

		// subHelper로 전송하여 모든 클라이언트가 인식하도록 한다.
		CNetMsg::SP rmsg(new CNetMsg);
		ResponseClient::TradeAgentChangeDeposit* packet = reinterpret_cast<ResponseClient::TradeAgentChangeDeposit*>(rmsg->m_buf);
		packet->type = MSG_TRADEAGENT;
		packet->subType = MSG_TRADEAGENT_CHANGE_DEPOSIT;
		packet->deposit = g_TradeAgent_Deposit;
		rmsg->setSize(sizeof(ResponseClient::TradeAgentChangeDeposit));
		SEND_Q(rmsg, gserver->m_subHelper);
	}

	CNetMsg::SP rmsg(new CNetMsg);
	std::string tstr = boost::str(boost::format("TradeAgent Deposit : %1%") % g_TradeAgent_Deposit);
	SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", tstr.c_str());
	SEND_Q(rmsg, ch->m_desc);
}

void do_FindNpcAndGoZone(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	int npcIndex = 0;
	int extra = 0;

	if (!arg || !(*arg))
		return ;

	char tmpBuf[MAX_MESSAGE_SIZE] = {0,};
	arg = AnyOneArg(arg, tmpBuf);

	npcIndex = atoi(tmpBuf);

	for(int i=0; i<MAX_ZONES - 1; i++)
	{
		if(&(gserver->m_zones[i]) == NULL)
			continue;
		if(&(gserver->m_zones[i].m_area) == NULL)
			continue;

		int zone = gserver->m_zones[i].m_index;

		if( gserver->m_zones[i].m_area->m_npcRegenList.Find(npcIndex, zone) == true )
		{
			CZone* pZone = gserver->m_zones + zone;

			if (extra < 0 || extra >= pZone->m_countZonePos)
				return ;

			if (ch->m_pZone->m_index == ZONE_SINGLE_DUNGEON_TUTORIAL && zone == ZONE_START)
				extra = 2;

			GoZone(ch, zone,
				   pZone->m_zonePos[extra][0],														// ylayer
				   GetRandom(pZone->m_zonePos[extra][1], pZone->m_zonePos[extra][3]) / 2.0f,		// x
				   GetRandom(pZone->m_zonePos[extra][2], pZone->m_zonePos[extra][4]) / 2.0f);		// z
			return;
		}
		else
		{
			vec_raidnpcrregenlist_t::iterator it = gserver->m_zones[i].m_area->m_raidNPCRegenList.begin();
			vec_raidnpcrregenlist_t::iterator endit = gserver->m_zones[i].m_area->m_raidNPCRegenList.end();
			for (; it != endit; ++it)
			{
				if( (*it)->m_npcIdx == npcIndex )
				{
					CZone* pZone = gserver->m_zones + (*it)->m_zoneNo;
					if (extra < 0 || extra >= pZone->m_countZonePos)
						return ;

					GoZone(ch, (*it)->m_zoneNo,
						   pZone->m_zonePos[extra][0],														// ylayer
						   GetRandom(pZone->m_zonePos[extra][1], pZone->m_zonePos[extra][3]) / 2.0f,		// x
						   GetRandom(pZone->m_zonePos[extra][2], pZone->m_zonePos[extra][4]) / 2.0f);		// z
					return;
				}
			}
		}
	}
}
//////////////////////////////////////////////////////////////////////////

void do_StashGetRange(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	if (vec.size() != 2)
		return;

	int startno = atoi(vec[0].c_str());
	int endno = atoi(vec[1].c_str());
	if (endno < startno)
		return;

	for (int i = startno; i <= endno; ++i)
	{
		if (i == NAS_ITEM_DB_INDEX)
			continue;

		CItem* pItem = gserver->m_itemProtoList.CreateItem(i, -1, 0, 0, 1);
		if(pItem == NULL)
			continue;

		if (ch->m_inventory.addItemInStash(pItem) == false)
		{
			delete pItem;
		}
	}
}

void do_StashDel(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	item_search_t item_vec;
	ch->m_inventory.searchAllItemInStash(item_vec);

	BOOST_FOREACH(item_search_pair_t& p, item_vec)
	{
		ch->m_inventory.deleteItemByItem(p.pItem);
	}
}

void do_MonsterCard(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	if (vec.size() != 3)
		return;

	int tab = atoi(vec[0].c_str());
	int invenIndex = atoi(vec[1].c_str());
	int sec = atoi(vec[2].c_str());

	CItem* pItem = ch->m_inventory.getItem(tab, invenIndex);
	if (pItem == NULL)
		return;

	if (pItem->m_itemProto->getItemTypeIdx() != ITYPE_ETC || pItem->m_itemProto->getItemSubTypeIdx() != IETC_MONSTER_MERCENARY_CARD)
		return;

	pItem->setUsed(gserver->getNowSecond() + sec);
	CNetMsg::SP rmsg(new CNetMsg);
	UpdateClient::makeUpdateItemUsed(rmsg, tab, invenIndex, pItem->getUsed());
	SEND_Q(rmsg, ch->m_desc);
}

void do_ViewAllShopList(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	if (vec.size() != 2)
		return;

	int zoneNo = atoi(vec[0].c_str());
	int npcIndex = atoi(vec[1].c_str());

	CZone* pZone = gserver->FindZone(zoneNo);
	if (pZone == NULL)
		return;

	CShop* pShop = pZone->FindShop(npcIndex);
	if (pShop == NULL)
		return;

	std::string tstr = boost::str(boost::format(
									  "\nNPC INDEX : %1%, TOTAL : %2%\n") % npcIndex % pShop->m_itemCount);
	for (int i = 0; i < pShop->m_itemCount; ++i)
	{
		CItemProto* pItemProto = gserver->m_itemProtoList.FindIndex(pShop->m_itemDBIdx[i]);
		if (pItemProto != NULL)
		{
			LONGLONG price = pItemProto->getItemPrice() * pShop->m_buyRate / 100;
			tstr += boost::str(boost::format("%1%\t%2%\n") % pShop->m_itemDBIdx[i] % price);
		}
		else
		{
			tstr += boost::str(boost::format("%1%\tnot found item proto.\n") % pShop->m_itemDBIdx[i]);
		}
	}

	LOG_INFO(tstr.c_str());
}

void do_DelCharacter(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	if (vec.size() != 1)
		return;

	if (strcmp(ch->m_nick, vec[0].c_str()) == 0)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		SayMsg(rmsg, MSG_CHAT_SAY, 0, "", "", "Can't delete itself");
		SEND_Q(rmsg, ch->m_desc);

		return;
	}

	DBManager::instance()->pushDelCharForGM(ch->m_desc, vec[0]);
}

void do_Logout(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	CNetMsg::SP rmsg(new CNetMsg);
	do_ClientRestart(ch, rmsg);
}

void do_Exp(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	LONGLONG levelup_exp = GetLevelupExp(ch->m_level);

	if (vec.empty() == false)
	{
		LONGLONG newexp = ATOLL(vec[0].c_str());
		if (newexp >= levelup_exp)
		{
			newexp = levelup_exp - 1;
		}

		ch->m_exp = newexp;
		ch->m_bChangeStatus = true;
	}

	{
		std::string tstr = boost::str(boost::format(
										  "now exp : %1% / levelup exp : %2%") % ch->m_exp % levelup_exp);
		CNetMsg::SP rmsg(new CNetMsg);
		SayMsg(rmsg, MSG_CHAT_SAY, 0, "", "", tstr.c_str());
		SEND_Q(rmsg, ch->m_desc);
	}
}

void do_GoNextPC(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	static int previous_char_index = 0;

	std::vector<CPC*> cpc_vec;
	cpc_vec.reserve(1024);

	PCManager::map_t& playerMap	= PCManager::instance()->getPlayerMap();
	PCManager::map_t::iterator it		= playerMap.begin();
	PCManager::map_t::iterator endIt	= playerMap.end();
	for (; it != endIt; ++it)
	{
		CPC* pChar = (*it).pPlayer;
		// 명령어를 내린 캐릭터가 아니고
		// 같은 존에 있고
		// 이전에 찾아가지 않은 캐릭터이고
		// 찾아갈 캐릭터의 위치가 개인던전이 아니라면
		if (ch->m_index != pChar->m_index && ch->m_pZone == pChar->m_pZone
				&& pChar->m_index != previous_char_index && pChar->m_pZone->IsPersonalDungeon() == false)
		{
			cpc_vec.push_back(pChar);
		}
	}

	if (cpc_vec.empty())
	{
		previous_char_index = 0; // 현재 1명이 있고, 이전에 찾아간적이 있을 경우 - 다시 찾아가기 위해서

		CNetMsg::SP rmsg(new CNetMsg);
		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "Nobody is in this zone.");
		SEND_Q(rmsg, ch->m_desc);

		return;
	}

	int index = GetRandom(0, (int)(cpc_vec.size() - 1));
	previous_char_index = cpc_vec[index]->m_index;
	GMGoZone(ch, cpc_vec[index]);
}

extern void do_Reform(CPC *ch_, CNetMsg::SP& msg);

void do_VirtualReform(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
#ifndef LC_KOR
	if (gserver->barunsongames_flag == false)
		return;
#endif

	if (vec.size() != 4)
	{
		LOG_INFO("not enought argument count");
		return;
	}

	int weaponIndex = atoi(vec[0].c_str());
	int reformerIndex = atoi(vec[1].c_str());
	int magnifireCount = atoi(vec[2].c_str());
	int loopCount = atoi(vec[3].c_str());

	if (loopCount <= 0)
	{
		LOG_INFO("loopcount is zero or minus");
		return;
	}

	CItem* weaponItem = ch->m_inventory.FindByDBIndex(weaponIndex);
	if (weaponItem == NULL)
	{
		LOG_INFO("not found weapon item");
		return;
	}

	CItem* reformerItem = ch->m_inventory.FindByDBIndex(reformerIndex);
	if (reformerItem == NULL)
	{
		LOG_INFO("not found reformer item");
		return;
	}

	CItem* maginfireItem = ch->m_inventory.FindByDBIndex(1839); // 황금 돋보기
	if (maginfireItem == NULL)
	{
		LOG_INFO("not found maginifire item");
		return;
	}

	// NPC의 virtual index찾기
	int npcVirtualIndex = ch->m_pArea->findNpcVIndex(1520);
	if (npcVirtualIndex == -1)
	{
		LOG_INFO("not found ncp");
		return;
	}

	CNPC* npc = (CNPC*)ch->m_pArea->FindCharInCell(ch, npcVirtualIndex, MSG_CHAR_NPC, false);
	if (npc == NULL)
	{
		LOG_INFO("not found ncp");
		return;
	}

	for (int i = 0; i < loopCount; ++i)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		RequestClient::doReform* packet = reinterpret_cast<RequestClient::doReform*>(rmsg->m_buf);

		packet->npcIndex = npcVirtualIndex;

		reformerItem->setItemCount(reformerItem->getItemCount() + 1);
		{
			CNetMsg::SP rmsg(new CNetMsg);
			UpdateClient::makeUpdateItemCount(rmsg, reformerItem->tab(), reformerItem->getInvenIndex(), reformerItem->getItemCount());
			SEND_Q(rmsg, ch->m_desc);
		}
		packet->reformer_tab = reformerItem->tab();
		packet->reformer_invenIndex = reformerItem->getInvenIndex();

		maginfireItem->setItemCount(maginfireItem->getItemCount() + magnifireCount);
		{
			CNetMsg::SP rmsg(new CNetMsg);
			UpdateClient::makeUpdateItemCount(rmsg, maginfireItem->tab(), maginfireItem->getInvenIndex(), maginfireItem->getItemCount());
			SEND_Q(rmsg, ch->m_desc);
		}
		packet->magnifire_tab = maginfireItem->tab();
		packet->magnifire_invenIndex = maginfireItem->getInvenIndex();

		packet->reform_tab = weaponItem->tab();
		packet->reform_invenIndex = weaponItem->getInvenIndex();

		packet->grade = (reformerItem->getDBIndex() == 9005) ? 0 : 1;
		packet->count = magnifireCount;

		do_Reform(ch, rmsg);
	}
}

void do_RockPaperScissorsWin(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	ch->setRockPaperScissorsFlag();
}

void do_GMIncreaseRVRPoint(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	if (vec.size() != 1)
	{
		return;
	}

	LONGLONG point = atoi(vec[0].c_str());

	if(point > 100000000)
		point = 100000000;

	ch->m_syndicateManager.increaseSyndicatePoint(point);
}

void do_GMDecreaseRVRPoint(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	if (vec.size() != 1)
	{
		return;
	}

	LONGLONG point = atoi(vec[0].c_str());

	if(point > 100000000)
		point = 100000000;

	ch->m_syndicateManager.decreaseSyndicatePoint(point);
}

void do_GMTheosCheckTime(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	if (vec.size() != 1)
	{
		return;
	}

	int time = atoi(vec[0].c_str());

	gserver->m_DratanCheckTime = time;

	std::string str = boost::str(boost::format("Change Dratan Dungeon CheckTime Success. Time : %d") % gserver->m_DratanCheckTime );

	CNetMsg::SP rmsg(new CNetMsg);
	SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", str.c_str());
	SEND_Q(rmsg, ch->m_desc);
}

void do_GMTheosChangePollution(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	if (vec.size() != 2)
	{
		return;
	}

	int nRate = atoi(vec[0].c_str());
	int mode = atoi(vec[1].c_str());

	if(nRate > 100)
		nRate = 100;

	if( mode != 0 && mode != 1 )
		return ;
	
	CDratanCastle *pCastle = CDratanCastle::CreateInstance();
	pCastle->m_isNotChange = true;

	{
		// 헬퍼에 변경 요청 nRate
		pCastle->m_dvd.SetEnvRate(nRate);
		pCastle->m_dvd.setOwnerMode(mode);

		CNetMsg::SP rmsg(new CNetMsg);
		HelperDVDRateChangeMsg(rmsg, MSG_MANAGEMENT_ENV_CHANGE , pCastle->m_dvd.GetEnvRate(), pCastle->m_dvd.GetOwnerMode() );
		SEND_Q(rmsg, gserver->m_helper);
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		DVDManagementMsg( rmsg, MSG_MANAGEMENT_ENV_CHANGE );
		RefMsg(rmsg) << (unsigned char) MSG_DVD_ERROR_OK;
		SEND_Q( rmsg, ch->m_desc );
	}

	GAMELOG << init(" DRATAN DYNAMIC DUNGEON ENV RATE CHANGE " , ch ) << delim
			<< "CHNAGE RATE : " << nRate << delim
			<< "CURRENT RATE : " << pCastle->m_dvd.GetEnvRate() << delim << end;
}

void do_GMTheosChangeDifficulty(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	if (vec.size() != 2)
	{
		return;
	}

	int nRate = atoi(vec[0].c_str());
	int mode = atoi(vec[1].c_str());

	if(nRate > 100)
		nRate = 100;

	if( mode != 0 && mode != 1 )
		return ;

	CDratanCastle *pCastle = CDratanCastle::CreateInstance();
	pCastle->m_isNotChange = true;

	{
		// 헬퍼에 변경 요청 nRate
		pCastle->m_dvd.SetMobRate(nRate);
		pCastle->m_dvd.setOwnerMode(mode);

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
			<< "CURRENT RATE : " << pCastle->m_dvd.GetMobRate() << delim << end;
}

#ifdef DURABILITY
void do_GM_Durability(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	if (vec.size() != 4)
		return;

	int tab_no = atoi(vec[0].c_str());
	int invenIndex = atoi(vec[1].c_str());
	int now_dur = atoi(vec[2].c_str());
	int max_dur = atoi(vec[3].c_str());

	if (ch->m_inventory.isValidNormalInventory(tab_no, invenIndex) == false)
		return;

	CItem* item = ch->m_inventory.getItem(tab_no, invenIndex);

	if (item == NULL)
		return;

	if ((item->m_itemProto->getItemFlag() & ITEM_FLAG_DURABILITY) == 0)
		return;

	if (item->m_itemProto->getDurability() < max_dur)
		max_dur = item->m_itemProto->getDurability();

	if (max_dur < now_dur)
		now_dur = max_dur;

	item->setNowDurability(now_dur);
	item->setMaxDurability(max_dur);

	{
		CNetMsg::SP rmsg(new CNetMsg);
		UpdateClient::makeItemDurabilityForInventory(rmsg, item->tab(), item->getInvenIndex(),
				item->getNowDurability(), item->getMaxDurability());
		SEND_Q(rmsg, ch->m_desc);
	}
}
#endif

void do_GM_hpmp(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	if (vec.size() != 2)
		return;

	int hp = atoi(vec[0].c_str());
	int mp = atoi(vec[1].c_str());

	if (hp < 0)
	{
		ch->m_hp -= hp;
		if (ch->m_hp > ch->m_maxHP)
			ch->m_hp = ch->m_maxHP;
	}
	else
	{
		ch->m_hp = ch->m_maxHP * hp / 100;
	}

	if (mp < 0)
	{
		ch->m_mp -= mp;
		if (ch->m_mp > ch->m_maxMP)
			ch->m_mp = ch->m_maxMP;
	}
	else
	{
		ch->m_mp = ch->m_maxMP * mp / 100;
	}

	ch->CalcStatus(true);
}

void do_GM_hp(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	int hp = (vec.empty() == false) ? atoi(vec[0].c_str()) : ch->m_maxHP;

	if (hp < 0)
		return;

	if (hp > ch->m_maxHP)
		hp = ch->m_maxHP;

	ch->m_hp = hp;
	ch->CalcStatus(true);
}

void do_GM_mp(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	int mp = (vec.empty() == false) ? atoi(vec[0].c_str()) : ch->m_maxMP;

	if (mp < 0)
		return;

	if (mp > ch->m_maxMP)
		mp = ch->m_maxMP;

	ch->m_mp = mp;
	ch->CalcStatus(true);
}

void do_GM_guildPoint(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	if (vec.size() != 2)
		return;

	int charindex = atoi(vec[0].c_str());
	int guildpoint = atoi(vec[1].c_str());

	CPC* tcp = PCManager::instance()->getPlayerByCharIndex( charindex );
	if( tcp && tcp->m_admin == 0 )
	{
		if( tcp->m_guildInfo )
		{
			int guildindex = ch->m_guildInfo->guild()->index();
			gserver->m_guildlist.findguild( guildindex )->guildPoint( guildpoint );

			CNetMsg::SP rmsg(new CNetMsg);
			HelperSaveGuildPointMsg( rmsg, guildindex, guildpoint );
			SEND_Q(rmsg, gserver->m_helper);
		}
	}
}

void do_GM_eventCollect(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	if (gserver->isActiveEvent( A_EVENT_COLLECT_BUG) == false)
		return;

	if (vec.empty())
		return;

	if (vec[0] == "set")
	{
		if (vec.size() != 2)
			return;

		int pct = atoi(vec[1].c_str());
		if( pct < 0 || pct > 50 )
			return;

		gserver->m_CollectBugEventPercent = pct * 10;

		{
			std::string str = boost::str(boost::format("Bug Drop Percent set %d Percent") % pct);
			CNetMsg::SP rmsg(new CNetMsg);
			SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", str.c_str());
			SEND_Q(rmsg, ch->m_desc);
		}
	}
	else if (vec[0] == "show")
	{
		std::string str = boost::str(boost::format("Now, Bug Drop Percent is %d Percent") % (gserver->m_CollectBugEventPercent / 10));
		CNetMsg::SP rmsg(new CNetMsg);
		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", str.c_str());
		SEND_Q(rmsg, ch->m_desc);
	}
}

void do_GM_block(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	if (vec.size() != 1)
		return;

	CNetMsg::SP rmsg(new CNetMsg);
	rmsg->Init(MSG_EXTEND);
	RefMsg(rmsg) << MSG_EX_MESSENGER
				 << (unsigned char) MSG_EX_MESSENGER_BLOCK
				 << vec[0].c_str();

	do_Extend(ch, rmsg);
}

void do_GM_release(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	if (vec.size() != 1)
		return;

	CNetMsg::SP rmsg(new CNetMsg);
	rmsg->Init(MSG_EXTEND);
	RefMsg(rmsg) << MSG_EX_MESSENGER
				 << (unsigned char) MSG_EX_MESSENGER_RELEASE_BLOCK
				 << atoi(vec[0].c_str());

	do_Extend(ch, rmsg);
}

void do_GM_coupon(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
#ifdef EVENT_PACKAGE_ITEM
	if (vec.size() != 1)
		return;

	CNetMsg::SP rmsg(new CNetMsg);
	rmsg->Init(MSG_EVENT);
	RefMsg(rmsg) << (unsigned char) MSG_EVENT_PACKAGE_ITEM_GIVE
				 << vec[0].c_str();

	do_Event(ch, rmsg);
#endif
}

void do_GM_promotion(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	std::string query = boost::str(boost::format(
									   "delete from t_proSite where a_user_idx = %d ") % ch->m_desc->m_index);
	DBManager::instance()->pushQuery(ch->m_desc->m_index, query);
}

void do_GM_setWarTime(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
#if defined(LC_BILA)
	if( ch->m_admin != 10)
		return;
#endif

	if (vec.size() != 5)
		return;

	struct tm t;
	time_t t2;
	time(&t2);
	t = *localtime(&t2);
	t.tm_year = atoi(vec[0].c_str()) - 1900;

	if( t.tm_year < 0 )
	{
		return;
	}

	t.tm_mon = atoi(vec[1].c_str()) -1;
	t.tm_mday = atoi(vec[2].c_str());
	t.tm_hour = atoi(vec[3].c_str());
	t.tm_min = atoi(vec[4].c_str());
	t.tm_sec = 0;
	t.tm_isdst = -1;
	time_t t3 = mktime(&t);
	if (t3 < t2)
		return ;

	meracCastle.SetNextWarTime(mktime(&t));
	meracCastle.SetNoticeWarTime(true);
	meracCastle.SetNoticeRemain(9999);
}

void do_GM_setWarTime2(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
#if defined(LC_BILA)
	if( ch->m_admin != 10)
		return;
#endif

	if (vec.size() != 5)
		return;

	struct tm t;
	time_t t2;
	time(&t2);
	t = *localtime(&t2);
	t.tm_year = atoi(vec[0].c_str()) - 1900;

	if( t.tm_year < 0 )
	{
		return;
	}

	t.tm_mon = atoi(vec[1].c_str()) -1;
	t.tm_mday = atoi(vec[2].c_str());
	t.tm_hour = atoi(vec[3].c_str());
	t.tm_min = atoi(vec[4].c_str());
	t.tm_sec = 0;
	t.tm_isdst = -1;
	time_t t3 = mktime(&t);
	if (t3 < t2)
		return ;

	CDratanCastle * pCastle = CDratanCastle::CreateInstance();
	pCastle->SetNextWarTime(mktime(&t));
	pCastle->SetNoticeWarTime(true);
	pCastle->SetNoticeRemain(9999);
}

void do_GM_setWarTimeEnd(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	meracCastle.endWarCastleMerac();
}

void do_GM_setWarTime2End(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	CDratanCastle * pCastle = CDratanCastle::CreateInstance();
	pCastle->m_bLoop = false;
}

void do_GM_loadhp5(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	CWarCastle* castle = CWarCastle::GetCastleObject(CWarCastle::GetCurSubServerCastleZoneIndex());
	if(!castle)
		return ;

	if(ch->m_pZone->m_index != castle->GetZoneIndex())
		return ;

	CNPC* lord = castle->GetLordSymbol();
	if(lord)
		lord->m_hp = 5;
}

void do_GM_guildCubeStart(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
#ifdef EXTREME_CUBE
	if(gserver->m_extremeCube.IsGuildCubeTime())
		return;

	if (vec.size() != 6)
		return;

	struct tm t;
	time_t t2;
	time(&t2);
	t = *localtime(&t2);
	t.tm_year = atoi(vec[0].c_str()) - 1900;

	if( t.tm_year < 0 )
		return;

	t.tm_mon = atoi(vec[1].c_str()) -1;
	t.tm_mday = atoi(vec[2].c_str());
	t.tm_hour = atoi(vec[3].c_str());
	t.tm_min = atoi(vec[4].c_str());
	t.tm_sec = atoi(vec[5].c_str());
	t.tm_isdst = -1;
	time_t t3 = mktime(&t);
	if (t3 < t2)
		return ;

	gserver->m_extremeCube.m_nextGuildCubeTime = t3;
#endif
}

void do_GM_guildCubeEnd(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
#ifdef EXTREME_CUBE
	if(gserver->m_extremeCube.IsGuildCubeTime() == false)
		return;

	int min = (vec.empty()) ? 0 : atoi(vec[0].c_str());

	if( min > 0)
	{
		time_t curtime;
		time(&curtime);
		gserver->m_extremeCube.m_nextGuildCubeTime = curtime - GUILDCUBE_TIME + ( 60 * min) ;
	}
	else
	{
		gserver->m_extremeCube.EndGuildCube();
	}
#endif
}

void do_GM_guildCubeState(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
#ifdef EXTREME_CUBE
	if(gserver->m_extremeCube.IsExtremeCubeServer() == false)
		return;

	FILE* fpCube = fopen(".CubeState", "w");
	if(fpCube == NULL)
		return;

	int i;
	CCubeSpace* cube;
	char buf[1024] = "";
	struct tm ti = NOW();

	IntCat0(buf, ti.tm_year + 1900, 4, false);
	strcat(buf, "/");
	IntCat0(buf, ti.tm_mon + 1, 2, false);
	strcat(buf, "/");
	IntCat0(buf, ti.tm_mday, 2, false);
	strcat(buf, " ");
	IntCat0(buf, ti.tm_hour, 2, false);
	strcat(buf, ":");
	IntCat0(buf, ti.tm_min, 2, false);
	strcat(buf, ":");
	IntCat0(buf, ti.tm_sec, 2, false);

	fprintf(fpCube, "%s\n", buf );

	for(i = 0; i < MAX_CUBESPACE; ++i)
	{
		cube = gserver->m_extremeCube.GetExtremeCube(i);
		if(cube)
			fprintf(fpCube, "%d: %d\n", i, (int)cube->GetState());
	}

	fclose(fpCube);
	fpCube = NULL;

#endif
}

void do_GM_guildCubeTimeCheck(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
#ifdef EXTREME_CUBE
	char buf[1024] = "";
	struct tm* ti = localtime(&(gserver->m_extremeCube.m_nextGuildCubeTime));

	strcat(buf, "guildcubetime: ");
	IntCat0(buf, (*ti).tm_year + 1900, 4, false);
	strcat(buf, "/");
	IntCat0(buf, (*ti).tm_mon + 1, 2, false);
	strcat(buf, "/");
	IntCat0(buf, (*ti).tm_mday, 2, false);
	strcat(buf, " ");
	IntCat0(buf, (*ti).tm_hour, 2, false);
	strcat(buf, ":");
	IntCat0(buf, (*ti).tm_min, 2, false);
	strcat(buf, ":");
	IntCat0(buf, (*ti).tm_sec, 2, false);

	{
		CNetMsg::SP rmsg(new CNetMsg);
		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", buf);
		SEND_Q(rmsg, ch->m_desc);
	}
#endif
}

void do_GM_setOwner(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
#if defined(LC_BILA)
	if( ch->m_admin != 10)
		return;
#endif

#if defined(LC_RUS)
	if( ch->m_admin < 9)
		return;
#endif

	if (vec.size() != 1)
		return;

	CPC* pc = PCManager::instance()->getPlayerByName(vec[0].c_str(), false);
	if (pc)
	{
		if (pc->m_guildInfo)
		{
			if (pc->m_guildInfo->guild())
			{
				CWarCastle* castle = CWarCastle::GetCastleObject(pc->m_pZone->m_index);
				if (castle)
				{
					castle->SetOwner(pc->m_guildInfo->guild());
					CWarCastle::CheckJoinAll(pc);
				}
			}
		}
	}
}

void do_GM_lordChat(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	CNetMsg::SP rmsg(new CNetMsg);
	SayMsg(rmsg, MSG_CHAT_LORD, ch->m_index, ch->GetName() , "", arg);
	do_Chat(ch, rmsg);
}

void do_GM_createPet(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
#if defined(LC_BILA) || defined(LC_RUS)
	if( ch->m_admin != 10)
		return;
#endif

	if (vec.size() != 1)
		return;

	int nPetProtoIndex = atoi(vec[0].c_str());

	CNetMsg::SP rmsg(new CNetMsg);
	HelperAttackPetMsg( rmsg, MSG_HELPER_APET_CREATE_REQ, ch->m_index );
	RefMsg(rmsg) << nPetProtoIndex ;
	SEND_Q( rmsg, gserver->m_helper );
}

void do_GM_apetStat(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
#if defined(LC_BILA) || defined(LC_RUS)
	if( ch->m_admin != 10)
		return;
#endif

	if (vec.size() != 5)
		return;

	int remain = atoi(vec[0].c_str());
	int str = atoi(vec[1].c_str());
	int con = atoi(vec[2].c_str());
	int dex = atoi(vec[3].c_str());
	int Int = atoi(vec[4].c_str());

	CAPet* apet = ch->GetAPet();
	if( apet )
	{
		apet->m_nRemainStat = remain;
		apet->UpdateStatPlus( str, dex, Int, con );
		CNetMsg::SP rmsg(new CNetMsg);
		ExAPetStatusMsg( rmsg, apet );
		SEND_Q( rmsg, ch->m_desc );
	}
}

void do_GM_apetHPMP(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
#if defined(LC_BILA) || defined(LC_RUS)
	if( ch->m_admin != 10)
		return;
#endif

	if (vec.size() != 2)
		return;

	int hp = atoi(vec[0].c_str());
	int mp = atoi(vec[1].c_str());

	CAPet* apet = ch->GetAPet();
	if( apet )
	{
		apet->m_hp = apet->m_maxHP * hp / 100 + apet->GetOwner()->m_opJewelElementPetHPUP;
		apet->m_mp = apet->m_maxMP * mp / 100;
		CNetMsg::SP rmsg(new CNetMsg);
		ExAPetStatusMsg( rmsg, apet );
		SEND_Q( rmsg, ch->m_desc );
	}
}

void do_GM_apetExp(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
#if defined(LC_BILA) || defined(LC_RUS)
	if( ch->m_admin != 10)
		return;
#endif

	if (vec.size() != 1)
		return;

	int exp = atoi(vec[0].c_str());
	if( exp <0 || exp>100 )
		return;

	CAPet* apet = ch->GetAPet();
	if( apet )
	{
		apet->m_exp = apet->GetNeedExp() * exp / 100;
		CNetMsg::SP rmsg(new CNetMsg);
		ExAPetStatusMsg( rmsg, apet );
		SEND_Q( rmsg, ch->m_desc );
	}
}

void do_GM_apetLevelUp(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
#if defined(LC_BILA) || defined(LC_RUS)
	if( ch->m_admin != 10)
		return;
#endif

	if (vec.size() != 1)
		return;

	int level = atoi(vec[0].c_str());

	if( level > MAX_APET_LEVEL )
		level = MAX_APET_LEVEL;

	CAPet* apet = ch->GetAPet();
	if( apet )
	{
		apet->m_exp = 0;
		apet->m_level = level;
		apet->LevelUp(true);

		{
			CNetMsg::SP rmsg(new CNetMsg);
			ExAPetStatusMsg( rmsg, apet );
			SEND_Q( rmsg, ch->m_desc );
		}
	}
}

void do_GM_apetUse(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	CAPet* apet = ch->GetAPet();
	if (apet == NULL)
		return;

	CAPet* targetPet = ch->GetAPet(1417);
	if (targetPet == NULL)
		return;

	apet->UseAccExp( targetPet );
}

void do_GM_apetCoolTime(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	if (vec.size() != 1)
		return;

	CAPet* apet = ch->GetAPet();
	if (apet == NULL)
		return;

	int min = atoi(vec[0].c_str());

	if( min >= 0 )
	{
		apet->ResetCooltime();
		apet->SetCooltime( min );

		{
			CNetMsg::SP rmsg(new CNetMsg);
			ExAPetStatusMsg( rmsg, apet );
			SEND_Q( rmsg, ch->m_desc );
		}
	}
}

void do_GM_apetAccExp(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	if (vec.size() != 1)
		return;

	CAPet* apet = ch->GetAPet();
	if (apet == NULL)
		return;

	int percent = atoi(vec[0].c_str());
	if( percent < 0 || percent > 100 )
		return;

	const APET_EXP_INFO* info = apet->m_pProto->GetExpInfo();
	if (info == NULL)
	{
		LOG_INFO("%d apet is not exp apet.", apet->m_pProto->Index());
		return;
	}

	LONGLONG charLevelupExp = GetLevelupExp(ch->m_level);
	LONGLONG maxAccExp = (LONGLONG)( charLevelupExp * ( (double)info->nMaxAccExpParam1 + (double)( apet->m_level *  info->nMaxAccExpParam2 ) / 100 ) /100 );
	apet->SetAccExp( (LONGLONG)(maxAccExp * percent / 100) );

	{
		CNetMsg::SP rmsg(new CNetMsg);
		ExAPetStatusMsg( rmsg, apet );
		SEND_Q( rmsg, ch->m_desc );
	}
}

void do_GM_svrInfo(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
#ifndef LC_VER
#define LC_VER "개발자 PC"
#endif

	std::string str = boost::str(boost::format("LC-%s-Game-v %s ") % LC_LOCAL_STRING % LC_VER );
	CNetMsg::SP rmsg(new CNetMsg);
	SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", str.c_str());
	SEND_Q(rmsg, ch->m_desc);
}

void do_GM_ep(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	if (vec.size() != 1)
		return;

	int ep = atoi(vec[0].c_str());
	if( ep > 0 )
		ch->AddEP( ep );

	ch->CalcStatus(true);
}

void do_GM_rankRefresh0831(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	if( ch->m_admin != 10 )
		return;

	{
		CNetMsg::SP rmsg(new CNetMsg);
		SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", "ranking refresh start");
		SEND_Q(rmsg, ch->m_desc);
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		SubHelperRankingRefreshEx(rmsg, ch->m_index);
		SEND_Q(rmsg, gserver->m_subHelper);
	}
}

void do_GM_petLevelUp(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
#if defined(LC_BILA) || defined(LC_RUS)
	if( ch->m_admin != 10)
		return;
#endif

#ifdef LC_TLD
	if( ch->m_admin < 9  )
		return;
#endif

	if (vec.size() != 1)
		return;

	int level = atoi(vec[0].c_str());
	if (level < 1 || level > PET_MAX_LEVEL)
		return ;

	CPet* pet = ch->GetPet();
	if (pet == NULL)
		return;

	pet->m_exp = 0;
	pet->SetAbilityPoint(pet->GetAbilityPoint() + (level - pet->m_level));
	pet->m_level = level;

	pet->LevelUp();

	{
		CNetMsg::SP rmsg(new CNetMsg);
		ExPetStatusMsg(rmsg, pet);
		SEND_Q(rmsg, ch->m_desc);
	}
}

void do_GM_petDeadTime(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	if (vec.size() != 1)
		return;

	int remain = atoi(vec[0].c_str());
	if (remain < 1)
		return ;

	CItem* item = ch->m_inventory.getItem(INVENTORY_TAB_NORMAL, 0);
	if (item && item->IsPet())
	{
		CPet* pet = ch->GetPet(item->getPlus());
		if (pet)
		{
			int cur = pet->GetRemainRebirthTime();
			if (cur < remain)
				return ;

			pet->SetRemainRebirthTime(remain);

			{
				CNetMsg::SP rmsg(new CNetMsg);
				ExPetStatusMsg(rmsg, pet);
				SEND_Q(rmsg, ch->m_desc);
			}
		}
	}
}

void do_GM_petReset(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	CNetMsg::SP rmsg(new CNetMsg);
	rmsg->Init(MSG_EXTEND);
	RefMsg(rmsg) << MSG_EX_PET_RESET_SKILL;
	do_Extend(ch, rmsg);
}

void do_GM_petExpUp(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	if (vec.size() != 1)
		return;

	double exppercent = atof(vec[0].c_str());
	if (exppercent < 0 || exppercent > 100)
		return ;

	CPet* pPet = ch->GetPet();
	if (pPet)
	{
		LONGLONG exp = pPet->GetNeedExp();
		exp = (LONGLONG)(exp * exppercent / 100);
		pPet->m_exp = exp;

		{
			CNetMsg::SP rmsg(new CNetMsg);
			ExPetStatusMsg(rmsg, pPet);
			SEND_Q(rmsg, ch->m_desc);
		}
	}
}

void do_GM_petMountReset(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	CItem* pItem = ch->m_inventory.getItem(INVENTORY_TAB_NORMAL, 1);
	if (pItem == NULL)
		return;

	CNetMsg::SP rmsg(new CNetMsg);
	ResponseClient::ItemUseMsg(rmsg, INVENTORY_TAB_NORMAL, 0, 1519, pItem->getVIndex());
	do_Item(ch, rmsg);
}

void do_GM_petChangeName(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	if (vec.size() != 1)
		return;

	CNetMsg::SP rmsg(new CNetMsg);
	rmsg->Init( MSG_EXTEND );
	RefMsg(rmsg) << MSG_EX_PET_CHANGE_NAME
				 << vec[0].c_str();
	do_Extend( ch, rmsg );
}

void do_GM_pkCount(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	if (vec.size() != 1)
		return;

	int pkcount = atoi(vec[0].c_str());

	if (pkcount < -110)
		pkcount = -110;

	if (pkcount > 110)
		pkcount = 110;

	ch->m_pkCount = pkcount;

	{
		CNetMsg::SP rmsg(new CNetMsg);
		CharStatusMsg(rmsg, ch, 0);
		ch->m_pArea->SendToCell(rmsg, ch, false);
	}

	ch->m_bChangeStatus = true;
}

void do_GM_pkPenalty(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
#ifdef LC_RUS
	if(ch->m_admin < 8)
		return ;
#endif

	if (vec.size() != 1)
		return;

#ifdef REFORM_PK_PENALTY_201108 // 2011-08 PK 패널티 리폼
	int pkpenalty = atoi(vec[0].c_str());

	if (pkpenalty < PK_CHAOTIC_POINT_MAX)
		pkpenalty = PK_CHAOTIC_POINT_MAX;

	if (pkpenalty > PK_HUNTER_POINT_MAX)
		pkpenalty = PK_HUNTER_POINT_MAX;
#else
	int pkpenalty = atoi(vec[0].c_str());

	if (pkpenalty < -155)
		pkpenalty = -155;

	if (pkpenalty > 155)
		pkpenalty = 155;
#endif

	ch->m_pkPenalty = pkpenalty;

	{
		CNetMsg::SP rmsg(new CNetMsg);
		CharStatusMsg(rmsg, ch, 0);
		ch->m_pArea->SendToCell(rmsg, ch, false);
	}

	ch->m_bChangeStatus = true;
}

void do_GM_showHP(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	if (vec.size() != 1)
		return;

	char	cOn = 0;
	if (vec[0] == "on")
		cOn = 1;
	else if (vec[0] == "off")
		cOn = 0;
	else
		return;

	{
		CNetMsg::SP rmsg(new CNetMsg);
		rmsg->Init(MSG_PK);
		RefMsg(rmsg) << (unsigned char)MSG_PK_TARGET_SHOWHP
					 << (char)cOn ;
		SEND_Q( rmsg, ch->m_desc );
	}
}

void do_GM_goPOS(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
#if defined(LC_BILA)
	if( ch->m_admin != 10)
		return;
#endif

	if (vec.size() != 1)
		return;

	int pos = atoi(vec[0].c_str());

	{
		CNetMsg::SP rmsg(new CNetMsg);
		rmsg->Init(MSG_WARP);
		RefMsg(rmsg) << (unsigned char)MSG_WARP_TELEPORT
					 << pos;
		do_Warp(ch, rmsg);
	}
}

void do_GM_pdNPC(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	//MSG_NPC_REGEN,				// NPC 리젠 관련	: index(n) dbIndex(n) x(f) z(f) h(f) r(f) yLayer(int) entityindex(n)
	CNetMsg::SP rmsg(new CNetMsg);
	rmsg->Init(MSG_NPC_REGEN);
	RefMsg(rmsg) << -1
				 << 190
				 << (float) 178
				 << (float) 253
				 << (float) 153
				 << 0
				 << 1095;

	do_NPCRegen(ch, rmsg);
}

void do_GM_cashBalance(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	CNetMsg::SP rmsg(new CNetMsg);
	rmsg->Init(MSG_EXTEND);
	RefMsg(rmsg) << MSG_EX_CASHITEM
				 << (unsigned char) MSG_EX_CASHITEM_BALANCE_REQ;

	do_Extend(ch, rmsg);
}

void do_GM_cashPurchase(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	if (vec.empty())
		return;

	int count = atoi(vec[0].c_str());
	if (vec.size() != (count + 1))
		return;

	{
		CNetMsg::SP rmsg(new CNetMsg);
		rmsg->Init(MSG_EXTEND);
		RefMsg(rmsg) << MSG_EX_CASHITEM
					 << (unsigned char) MSG_EX_CASHITEM_PURCHASE_REQ
					 << ch->m_cashBalance
					 << count;

		for(int i = 0; i < count; i++)
		{
			RefMsg(rmsg) << vec[i + 1].c_str();
		}

		do_Extend(ch, rmsg);
	}
}

void do_GM_cashPurchaseList(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	CNetMsg::SP rmsg(new CNetMsg);
	rmsg->Init(MSG_EXTEND);
	RefMsg(rmsg) << MSG_EX_CASHITEM
				 << (unsigned char) MSG_EX_CASHITEM_PURCHASELIST_REQ;

	do_Extend(ch, rmsg);
}

void do_GM_cashPurchaseHistory(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	if (vec.size() != 3)
		return;

	int year = atoi(vec[0].c_str());
	char month = atoi(vec[1].c_str());
	char day = atoi(vec[2].c_str());

	{
		CNetMsg::SP rmsg(new CNetMsg);
		rmsg->Init(MSG_EXTEND);
		RefMsg(rmsg) << MSG_EX_CASHITEM
					 << (unsigned char) MSG_EX_CASHITEM_PURCHASEHISTORY_REQ
					 << year
					 << month
					 << day;

		do_Extend(ch, rmsg);
	}
}

void do_GMRVRApplySkill(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	////////
	int JewelPoint_K = SyndicateInfoDataManager::instance()->getJewelPoint(SYNDICATE::eSYNDICATE_KAILUX);
	int JewelPoint_D = SyndicateInfoDataManager::instance()->getJewelPoint(SYNDICATE::eSYNDICATE_DEALERMOON);

	if( JewelPoint_K > 100 )
	{
		BOOST_FOREACH(CPC* pc, gserver->m_kailuxUser)
		{
			bool bApply;
			CSkill* skill = gserver->m_skillProtoList.Create(1758, 1);
			ApplySkill(pc, pc, skill, -1, bApply);
		}
	}
	if ( JewelPoint_D > 100 )
	{
		BOOST_FOREACH(CPC* pc, gserver->m_dealermoonUser)
		{
			bool bApply;
			CSkill* skill = gserver->m_skillProtoList.Create(1758, 1);
			ApplySkill(pc, pc, skill, -1, bApply);
		}
	}
}

void do_GMRVRShowJewelPoint(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	int JewelPoint_K = SyndicateInfoDataManager::instance()->getJewelPoint(SYNDICATE::eSYNDICATE_KAILUX);
	int JewelPoint_D = SyndicateInfoDataManager::instance()->getJewelPoint(SYNDICATE::eSYNDICATE_DEALERMOON);

	char str[1024];
	sprintf(str, "Kailux Point : %d, DealerMoon Point : %d", JewelPoint_K, JewelPoint_D);
	CNetMsg::SP rmsg(new CNetMsg);
	SayMsg(rmsg, MSG_CHAT_GM, 0, "", "", str);
	SEND_Q(rmsg, ch->m_desc);
	return ;
}

void do_GMRVRSetUserCount(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	int type = atoi(vec[0].c_str());
	char count = atoi(vec[1].c_str());

	if(type == 1 || type == 2)
	{
		CNetMsg::SP joinMsg(new CNetMsg);
		UpdateServer::HelperServerRvRSetUserCount(joinMsg, type, count);
		SEND_Q(joinMsg, gserver->m_helper);
	}
	else
	{
		CNetMsg::SP rmsg(new CNetMsg);
		SayMsg(rmsg, MSG_CHAT_GM, 0, "", "", "Invalid Type Index. 1 : Kai, 2 : Dealar");
		SEND_Q(rmsg, ch->m_desc);
	}
}

void do_GMRVRShowUserCount(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	int UserCount_K = SyndicateInfoDataManager::instance()->getJoinCount(SYNDICATE::eSYNDICATE_KAILUX);
	int UserCount_D = SyndicateInfoDataManager::instance()->getJoinCount(SYNDICATE::eSYNDICATE_DEALERMOON);

	char str[1024];
	sprintf(str, "Kailux User : %d, DealerMoon User : %d", UserCount_K, UserCount_D);
	CNetMsg::SP rmsg(new CNetMsg);
	SayMsg(rmsg, MSG_CHAT_GM, 0, "", "", str);
	SEND_Q(rmsg, ch->m_desc);
	return ;
}

void do_GM_ItemUsed_Set(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	item_search_t vec_item;
	ch->m_inventory.searchAllItem(vec_item);

	item_search_t::iterator it = vec_item.begin();
	item_search_t::iterator it_end = vec_item.end();

	for(; it != it_end; it++)
	{
		it->pItem->setUsed(gserver->getNowSecond() + 60);
		it->pItem->setUsed_2(gserver->getNowSecond() + 60);
	}

	ch->m_inventory.sendInfoToClient();
}

void do_GM_GetExchangeItem(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	if (vec.empty())
		return;

	std::vector<itemExchangeProto::tag_sourceInfo> item_vec;
	itemExchangeProto::instance()->getExchangeItem(atoi(vec[0].c_str()), item_vec);
	if (item_vec.empty())
		return;

	BOOST_FOREACH(itemExchangeProto::tag_sourceInfo& data, item_vec)
	{
		CItem* pItem = gserver->m_itemProtoList.CreateItem(data.itemIndex, WEARING_NONE, 0, 0, data.itemCount);
		if (pItem == NULL)
			return;

		if (ch->m_inventory.addItem(pItem) == false)
		{
			delete pItem;
			return;
		}
	}
}

void do_GM_NoCoolBoxItem(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	ch->m_isNotCoolBoxItem = true;

	std::string str = "Special Item NoCoolTime Setting On";	
	CNetMsg::SP rmsg(new CNetMsg);
	SayMsg(rmsg, MSG_CHAT_GM, 0, "", "", str.c_str());
	SEND_Q(rmsg, ch->m_desc);
}

#ifdef PREMIUM_CHAR
void do_GM_PremiumChar_Reset(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	if (ch->m_premiumChar.isActive() == false)
	{
		LOG_INFO("premium char : is not running.");
		return;
	}

	// 로그를 남기는 작업이므로, 메모리 데이터를 초기화(reset()) 하기 이전에 호출할것
	LOG_INFO("PREMIUM_CHAR_RESET (by GM command) - charIndex : %d", ch->m_index);	// 향후 검색을 용이하게 하기 위한 작업
	ch->m_premiumChar.saveResetLog();

	ch->m_premiumChar.reset();
	ch->m_premiumChar.sendEnd();
}

void do_GM_PremiumChar_set_expire_time(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	if (ch->m_premiumChar.isActive() == false)
	{
		LOG_INFO("premium char : is not running.");
		return;
	}

	if (vec.size() != 1)
	{
		LOG_INFO("premium char : invalid argument");
		return;
	}

	int sec = atoi(vec[0].c_str());

	ch->m_premiumChar.setExpireTime(gserver->getNowSecond() + sec);
	ch->m_premiumChar.sendInfo();
}

void do_GM_PremiumChar_set_jump_count(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	if (ch->m_premiumChar.isActive() == false)
	{
		LOG_INFO("premium char : is not running.");
		return;
	}

	if (vec.size() != 1)
	{
		LOG_INFO("premium char : invalid argument");
		return;
	}

	int jump_count = atoi(vec[0].c_str());

	if (jump_count < 0 || jump_count > PREMIUM_CHAR_JUMP_COUNT_MAX)
	{
		LOG_INFO("premium char : invalid jump count(%d)", jump_count);
		return;
	}

	ch->m_premiumChar.setJumpCount(jump_count);
}

void do_GM_PremiumChar_set_type(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	if (ch->m_premiumChar.isActive() == false)
	{
		LOG_INFO("premium char : is not running.");
		return;
	}

	if (vec.size() != 1)
	{
		LOG_INFO("premium char : invalid argument");
		return;
	}

	int premium_type = atoi(vec[0].c_str());
	if (premium_type < PREMIUM_CHAR_TYPE_NONE || premium_type >= PREMIUM_CHAR_TYPE_MAX)
	{
		LOG_INFO("premium char : invalid type(%s)", premium_type);
		return;
	}

	if (premium_type == PREMIUM_CHAR_TYPE_NONE)
	{
		ch->m_premiumChar.reset();
	}
	else
	{
		ch->m_premiumChar.setPremiumType(premium_type);
	}

	ch->m_premiumChar.sendInfo();
}
#endif

void do_GM_Attendance_assure_set_expire_time(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	if(ch->m_attendanceManager.getUseAssure() == false)
	{
		return;
	}
	
	if (vec.size() != 1)
	{
		LOG_INFO("invalid argument");
		return;
	}

	//사용중이라면 지금 걸려있는 버프에서 출석 유지 보험 버프를 찾아서 남은 시간을 조정한다.
	int sec = atoi(vec[0].c_str());

	ch->m_assist.m_help.getHead();

	CAssistData *p = NULL;
	CAssistData *pNext = NULL;

	p = ch->m_assist.m_help.getHead();
	while((pNext = p))
	{
		p = p->m_next;

		if(pNext->m_index == 10421)
		{
			pNext->m_remain = sec * 10;
			break;
		}
	}

	{
		// assist
		CNetMsg::SP rmsg(new CNetMsg);
		AssistListMsg(rmsg, ch);
		SEND_Q(rmsg, ch->m_desc);
	}
}

void do_raid_changeMaxAreaCount(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	if (vec.size() != 2)
		return;

	int zone_index = atoi(vec[0].c_str());
	int maxCount = atoi(vec[1].c_str());

	CZone* zone = gserver->FindZone(zone_index);

	if(zone == NULL)
	{
		LOG_ERROR("Invalid Zone_Index. zone_index[%d]", zone_index);
		return;
	}

	if(zone->m_index == ZONE_ALTER_OF_DARK || zone->m_index == ZONE_CAPPELLA_1 || zone->m_index == ZONE_AKAN_TEMPLE
		|| zone->m_index == ZONE_TARIAN_DUNGEON)
	{
		if(maxCount < 1)
		{
			LOG_ERROR("Invalid MAX Count. maxCount[%d]", maxCount);
			return;
		}

		zone->m_countArea = maxCount;
		CNetMsg::SP rmsg(new CNetMsg);
		SayMsg(rmsg, MSG_CHAT_GM, 0, "", "", "Change AreaCount Success...");
		SEND_Q(rmsg, ch->m_desc);
	}
	else
	{
		LOG_ERROR("Invalid Zone_Index. zone_index[%d]", zone_index);
		CNetMsg::SP rmsg(new CNetMsg);
		SayMsg(rmsg, MSG_CHAT_GM, 0, "", "", "ZONE_ALTER_OF_DARK[35], ZONE_CAPPELLA_1[33], ZONE_AKAN_TEMPLE[36], ZONE_TARIAN_DUNGEON[41]");
		SEND_Q(rmsg, ch->m_desc);
		return;
	}
}

void do_popup_notice(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	char tmpBuf[MAX_MESSAGE_SIZE] = {0,};

	*tmpBuf = '\0';		// argument for arg
	if (!arg || !(*arg))		
		return ;
	
	//서버정보
	arg = AnyOneArg(arg, tmpBuf, true);
	int type;
	
	if (strcmp(tmpBuf, GM_POPUP_NOTICE_ALL) == 0)					
		type = 0;	//all
	else if (strcmp(tmpBuf, GM_POPUP_NOTICE_SERVER) == 0)			
		type = 1;	//Server
	else if (strcmp(tmpBuf, GM_POPUP_NOTICE_SUB_SERVER) == 0)		
		type = 2;	//Subserver

	if (strcmp(tmpBuf, "list") == 0)
	{
		//list 전달
		PopupNotice::instance()->SendAdminSystemMsg(ch);		
		return ;	
	}

	arg = AnyOneArg(arg, tmpBuf, true);

	if (strcmp(tmpBuf, "clear") == 0)
	{
		//clear msg 전달 
		CNetMsg::SP rmsg(new CNetMsg);

		switch (type)
		{
		case 0:
			MsgrPopupNoticeClearMsg(rmsg, -1, -1, -1, -1);
			break;
		case 1:
			MsgrPopupNoticeClearMsg(rmsg, -1, gserver->m_serverno, -1, -1);
			break;
		case 2:
			MsgrPopupNoticeClearMsg(rmsg, -1, gserver->m_serverno, gserver->m_subno, ch->m_pZone->m_index);
			break;
		}
		SEND_Q(rmsg, gserver->m_messenger);
		return ;	
	}

	if(vec.size() < 3 || vec.size() > 8)
		 return;

	//html 파일 정보 (넘버)
	int html_num = atoi(vec[1].c_str());

	//runtime 정보
	int runtime = atoi(vec[2].c_str());

	if(html_num == 0 || runtime == 0)
		return ;

	time_t start_time;

	if(vec.size() != 3 && vec.size() == 8)
	{
		//예약 시간 정보
		struct tm t;
		time_t t2;
		time(&t2);
		t = *localtime(&t2);
		t.tm_year = atoi(vec[3].c_str()) - 1900;

		if( t.tm_year < 0 )
		{
			return;
		}

		t.tm_mon = atoi(vec[4].c_str()) -1;
		t.tm_mday = atoi(vec[5].c_str());
		t.tm_hour = atoi(vec[6].c_str());
		t.tm_min = atoi(vec[7].c_str());
		t.tm_sec = 0;
		t.tm_isdst = -1;
		start_time = mktime(&t);
		if (start_time < t2)
			return ;
	}
	else if(vec.size() == 3)
	{
		start_time = 0;
	}
	else
		return;
	
	{
		CNetMsg::SP rmsg(new CNetMsg);
		switch (type)
		{
		case 0:
			MsgrPopupNoticeMsg(rmsg, -1, -1, -1, -1, html_num, runtime, start_time);
			break;
		case 1:
			MsgrPopupNoticeMsg(rmsg, -1, gserver->m_serverno, -1, -1, html_num, runtime, start_time);
			break;
		case 2:
			MsgrPopupNoticeMsg(rmsg, -1, gserver->m_serverno, gserver->m_subno, ch->m_pZone->m_index, html_num, runtime, start_time);
			break;
		}
		SEND_Q(rmsg, gserver->m_messenger);
	}
}

void do_popup_noticeoff(CPC* ch, const char* arg, std::vector<std::string>& vec)
{
	char tmpBuf[MAX_MESSAGE_SIZE] = {0,};

	*tmpBuf = '\0';		// argument for arg
	if (!arg || !(*arg))		
		return ;

	//서버정보
	arg = AnyOneArg(arg, tmpBuf, true);
	int type;

	if (strcmp(tmpBuf, GM_POPUP_NOTICE_ALL) == 0)					
		type = 0;	//all
	else if (strcmp(tmpBuf, GM_POPUP_NOTICE_SERVER) == 0)			
		type = 1;	//Server
	else if (strcmp(tmpBuf, GM_POPUP_NOTICE_SUB_SERVER) == 0)		
		type = 2;	//Subserver
	else 
		return ;

	//html 파일 정보 (넘버)
	int html_num = atoi(vec[1].c_str());
	
	CNetMsg::SP rmsg(new CNetMsg);
	switch (type)
	{
	case 0:
		MsgrPopupNoticeCloseMsg(rmsg, -1, -1, -1, -1, html_num);
		break;
	case 1:
		MsgrPopupNoticeCloseMsg(rmsg, -1, gserver->m_serverno, -1, -1, html_num);
		break;
	case 2:
		MsgrPopupNoticeCloseMsg(rmsg, -1, gserver->m_serverno, gserver->m_subno, ch->m_pZone->m_index, html_num);
		break;
	}
	SEND_Q(rmsg, gserver->m_messenger);
}

void do_test( CPC* ch, const char* arg, std::vector<std::string>& vec )
{
	/*CNetMsg::SP rmsg(new CNetMsg);
	do_GuildRecall(ch, rmsg);*/

	// 이동 가능 검사
	int canWarp = ch->CanWarp();
	if (canWarp != 0)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		SysMsg(rmsg, (MSG_SYS_TYPE)canWarp);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 10초후 이동하게 하기
	ch->m_reqWarpType_skill = IONCE_WARP_RETURN;
	ch->m_reqWarpData_skill = ZONE_GUILDROOM;
	ch->m_reqWarpTime_skill = PULSE_WARPDELAY;
	ch->SetPlayerState(PLAYER_STATE_WARP);

	{
		// 사용했음을 알리기
		CNetMsg::SP rmsg(new CNetMsg);
		WarpStartMsg(rmsg, ch);
		ch->m_pArea->SendToCell(rmsg, ch, true);
	}

	return ;
}
