#include "stdhdrs.h"

#include "Log.h"
#include "Character.h"
#include "Server.h"
#include "CmdMsg.h"
#include "doFunc.h"
#include "hardcoding.h"
#include "../ShareLib/packetType/ptype_old_do_event.h"

void do_EventXmas2007( CPC* ch, CNetMsg::SP& msg );
void do_EventXmasPuzzle( CPC* ch, CNetMsg::SP& msg, unsigned char type );

#if defined( EVENT_SUMMER_VACATION_2007_RESULT )
void do_EventSummerVacation2007( CPC* ch, CNetMsg::SP& msg );
#endif// defined( EVENT_SUMMER_VACATION_2007_RESULT )

void do_EventGomdori2007(CPC* ch, CNetMsg::SP& msg);

void do_EventWhiteday2007(CPC* ch, CNetMsg::SP& msg);

void do_EventValentine2007(CPC* ch, CNetMsg::SP& msg);
bool BINGO(int plus, int* score, int* bingocount);

void do_EventChuseok2006(CPC* ch, CNetMsg::SP& msg);
void do_EventSuperstone(CPC* ch, CNetMsg::SP& msg);
#ifdef EVENT_TREASUREBOX
void do_EventTreasurebox(CPC* ch, CNetMsg::SP& msg);
#endif // #ifdef EVENT_TREASUREBOX

void do_EventLatto(CPC* ch, CNetMsg::SP& msg);
void do_EventChangeWeapon(CPC* ch, CNetMsg::SP& msg);
void do_EventChuseokUpgrade(CPC* ch, CNetMsg::SP& msg);
void do_EventChuseokExchange(CPC* ch, CNetMsg::SP& msg);
#ifdef EVENT_PACKAGE_ITEM
void do_EventPackageItemGive(CPC* ch, CNetMsg::SP& msg);
#endif

void do_EventWorldcup(CPC* ch, CNetMsg::SP& msg);
void do_EventGoldenball(CPC* ch, CNetMsg::SP& msg);

#if defined(EVENT_WORLDCUP_2010) || defined(EVENT_WORLDCUP_2010_TOTO) || defined(EVENT_WORLDCUP_2010_TOTO_STATUS) || defined(EVENT_WORLDCUP_2010_TOTO_GIFT) || defined(EVENT_WORLDCUP_2010_ATTENDANCE) || defined(EVENT_WORLDCUP_2010_KOREA)
void do_EventWorldcup2010(CPC* ch, CNetMsg::SP& msg);
void EventWordlcup2010Trade(CPC *ch, CNetMsg::SP& msg);
void EventWordlcup2010TOTO(CPC *ch, CNetMsg::SP& msg);
void EventWordlcup2010TOTOStatus(CPC *ch, CNetMsg::SP& msg);
void EventWordlcup2010TOTOGift(CPC *ch);
void EventWordlcup2010Attendance(CPC *ch);
void EventWordlcup2010Supportcard(CPC *ch, CNetMsg::SP& msg);
void EventWorldcup2010Korea(CPC *ch);
#endif // #if defined(EVENT_WORLDCUP_2010) || defined(EVENT_WORLDCUP_2010_TOTO_STATUS) || defined(EVENT_WORLDCUP_2010_TOTO_GIFT) || defined(EVENT_WORLDCUP_2010_ATTENDANCE) || defined(EVENT_WORLDCUP_2010_KOREA)

void do_EventRain2006(CPC* ch, CNetMsg::SP& msg);

void do_EventCollectBug(CPC* ch, CNetMsg::SP& msg);

void do_EventOpenAdultServer(CPC* ch, CNetMsg::SP& msg);

void do_EventHalloween2007(CPC* ch, CNetMsg::SP& msg);

void do_EventNewMoonstone(CPC* ch, CNetMsg::SP& msg);

#if defined (SAKURA_EVENT_2008_REWARD)
void do_EventSakura2008(CPC* ch, CNetMsg::SP& msg );
#endif //defined (SAKURA_EVENT_2008_REWARD)

void do_EventPhoenix( CPC* ch, CNetMsg::SP& msg );
void do_EventPhoenix_Success( CPC* ch, CNetMsg::SP& msg );

#ifdef DEV_EVENT_PROMOTION2
void do_EventPromotion2(CPC* pc, CNetMsg::SP& msg);
#endif

#ifdef EVENT_USER_COMEBACK
void do_EventUserComeback(CPC* ch, CNetMsg::SP& msg);
#endif

#ifdef EVENT_CHAR_BIRTHDAY
void do_EventCharBirthday(CPC* ch, CNetMsg::SP& msg);
#endif

#ifdef EVENT_EUR2_PROMOTION
void do_EventEUR2Promotion(CPC* ch, CNetMsg::SP& msg);
#endif

#ifdef DEV_EVENT_AUTO
void do_EventKBReward(CPC* ch, CNetMsg::SP& msg);
#endif

void do_Event(CPC* ch, CNetMsg::SP& msg)
{
	msg->MoveFirst();
	unsigned char subtype;
	RefMsg(msg) >> subtype;

	switch (subtype)
	{
	case MSG_EVENT_XMAS_2007 :
		do_EventXmas2007( ch, msg );
		break;

	case MSG_EVENT_HALLOWEEN_2007:
		do_EventHalloween2007( ch, msg );
		break;

#if defined( EVENT_SUMMER_VACATION_2007_RESULT )
	case MSG_EVENT_SUMMER_VACATION_2007:
		do_EventSummerVacation2007( ch, msg );
		break;
#endif// defined( EVENT_SUMMER_VACATION_2007_RESULT )

	case MSG_EVENT_GOMDORI_2007:
		do_EventGomdori2007(ch, msg);
		break;

	case MSG_EVENT_WHITEDAY_2007:
		do_EventWhiteday2007(ch, msg);
		break;

	case MSG_EVENT_VALENTINE_2007:
		do_EventValentine2007(ch, msg);

	case MSG_EVENT_CHUSEOK_2006:
		do_EventChuseok2006(ch, msg);
		break;

	case MSG_EVENT_COLLECT_BUG:
		do_EventCollectBug(ch, msg);
		break;

	case MSG_EVENT_RAIN_2006:
		do_EventRain2006(ch, msg);
		break;

	case MSG_EVENT_GOLDENBALL:
		do_EventGoldenball(ch, msg);
		break;

	case MSG_EVENT_WORLDCUP:
		do_EventWorldcup(ch, msg);
		break;

#ifdef EVENT_PACKAGE_ITEM
	case MSG_EVENT_PACKAGE_ITEM_GIVE:
		do_EventPackageItemGive(ch, msg);
		break;
#endif // EVENT_PACKAGE_ITEM

	case MSG_EVENT_CHANGEWEAPON:
		do_EventChangeWeapon(ch, msg);
		break;

	case MSG_EVENT_PREOPEN_GIFT:
		break;

	// 하드 코딩
	case MSG_EVENT_LATTO:
		do_EventLatto(ch, msg);
		break;
//0502 kwon
	case MSG_EVENT_MOONSTONE:
		break;

	case MSG_EVENT_TREASUREBOX:
#ifdef EVENT_TREASUREBOX
		do_EventTreasurebox(ch, msg);
#endif // EVENT_TREASUREBOX
		break;

	//0627
	case MSG_EVENT_SUPERSTONE:
		do_EventSuperstone(ch, msg);
		break;

	case MSG_EVENT_CHUSEOK_UPGRADE:
		do_EventChuseokUpgrade(ch, msg);
		break;

	case MSG_EVENT_CHUSEOK_EXCHANGE:
		do_EventChuseokExchange(ch, msg);
		break;

	case MSG_EVENT_OPEN_BETA_GIFT:
		break;

	case MSG_EVENT_OPEN_ADULT_SERVER:
		do_EventOpenAdultServer(ch, msg);
		break;

	case MSG_EVENT_NEW_MOONSTONE:
		do_EventNewMoonstone(ch, msg);
		break;

#if defined (SAKURA_EVENT_2008_REWARD)
	case MSG_EVENT_SAKURA_2008:
		do_EventSakura2008(ch, msg);
		break;
#endif // defined (SAKURA_EVENT_2008_REWARD)

	case MSG_EVENT_PHOENIX:
		do_EventPhoenix(ch, msg);
		break;

	case MSG_EVENT_PHOENIX_SUCCESS:
		do_EventPhoenix_Success(ch, msg);
		break;

#if defined(EVENT_WORLDCUP_2010) || defined(EVENT_WORLDCUP_2010_TOTO) || defined(EVENT_WORLDCUP_2010_TOTO_STATUS) || defined(EVENT_WORLDCUP_2010_TOTO_GIFT) || defined(EVENT_WORLDCUP_2010_ATTENDANCE)
	case MSG_EVENT_WORLDCUP_2010:
		do_EventWorldcup2010(ch, msg);
		break;
#endif

#ifdef DEV_EVENT_PROMOTION2
	case MSG_EVENT_PROMOTION2:
		do_EventPromotion2(ch, msg);
		break;
#endif

#ifdef EVENT_USER_COMEBACK
	case MSG_EVENT_USER_COMEBACK:
		do_EventUserComeback(ch, msg);
		break;
#endif

#ifdef EVENT_CHAR_BIRTHDAY
	case MSG_EVENT_CHAR_BIRTHDAY:
		do_EventCharBirthday(ch, msg);
		break;
#endif

#ifdef EVENT_EUR2_PROMOTION
	case MSG_EVENT_EUR2_PROMOTION:
		do_EventEUR2Promotion(ch, msg);
		break;
#endif

#ifdef DEV_EVENT_AUTO
	case MSG_EVENT_KB_REWARD:
		do_EventKBReward(ch, msg);
		break;
#endif
	default:
		break;
	}
}

void do_EventSuperstone(CPC* ch, CNetMsg::SP& msg)
{
	unsigned char type;
	int chaindex;

#ifdef NPC_CHECK
	int npcIndex;
	CNPC *npc;
	RefMsg(msg) >> type
				>> npcIndex
				>> chaindex;
	npc = ch->m_pArea->FindProtoNPCInCell(ch, npcIndex, false, 2);
	if(!npc)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		SysMsg(rmsg, MSG_SYS_NOT_EXIST_NPC);
		SEND_Q(rmsg, ch->m_desc);
		GAMELOG << init("EventSuperStone FAIL: NOT EXIST NPC (Dangerous User)", ch) << end;
		return ;
	}
	if( !npc->IsFlag(NPC_QUEST) )
	{
		GAMELOG << init("EventSuperStone FAIL: NOT EXIST NPC (Dangerous User)", ch) << end;
		return ;
	}
#else
	RefMsg(msg) >> type;
	RefMsg(msg) >> chaindex;
#endif

	if(type == MSG_EVENT_SUPERSTONE_REQ)
	{
		static const int superStoneTable[7][2] =
		{
			{0, 200},
			{1, 300},
			{2, 500},
			{3, 800},
			{4, 1200},
			{5, 1700},
			{6, 2300},
		};
		int i;
		for (i = 0; i < 7; i++)
		{
			if (ch->m_superstone == superStoneTable[i][0])
			{
				if (ch->m_fame < superStoneTable[i][1])
				{
					CNetMsg::SP rmsg(new CNetMsg);
					SysMsg(rmsg, MSG_SYS_FAME_NOT_ENOUGH);
					SEND_Q(rmsg, ch->m_desc);
					return;
				}
				break;
			}
		}
		if (i == 7)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			SysMsg(rmsg, MSG_SYS_NO_MORE_SUPERSTONE);
			SEND_Q(rmsg, ch->m_desc);
			return;
		}

		CItem* item = gserver->m_itemProtoList.CreateItem(779, -1, 0, 0, 1);//우선은 고급제련석.
		if (!item)
			return;

		if (ch->m_inventory.addItem(item) == false)
		{
			// 인벤토리 가득참
			CNetMsg::SP rmsg(new CNetMsg);
			SysMsg(rmsg, MSG_SYS_INVEN_NOT_ENOUGH);
			SEND_Q(rmsg, ch->m_desc);


			GAMELOG << init("CHANGE_SUPERSTONE", ch)
					<< itemlog(item)
					<< end;

			delete item;

			return;
		}

		{
			CNetMsg::SP rmsg(new CNetMsg);
			EventSuperStoneMsg(rmsg, MSG_EVENT_SUPERSTONE_REP);
			SEND_Q(rmsg, ch->m_desc);
		}

		ch->m_superstone++;

		{
			CNetMsg::SP rmsg(new CNetMsg);
			HelperTeacherSuperstoneRecieve(rmsg, ch->m_index);
			SEND_Q(rmsg, gserver->m_helper);
		}

		// Item LOG
		GAMELOG << init("CHANGE_SUPERSTONE", ch)
				<< itemlog(item)
				<< end;
	}
}

#ifdef EVENT_TREASUREBOX
void do_EventTreasurebox(CPC* ch, CNetMsg::SP& msg)
{
	CNetMsg::SP rMsg(new CNetMsg);

	unsigned char type;
	int chaindex;
	int level;

#ifdef NPC_CHECK
	int npcIndex;
	CNPC *npc;
	RefMsg(msg) >> type
				>> npcIndex
				>> chaindex;
	npc = ch->m_pArea->FindProtoNPCInCell(ch, npcIndex, false, 2);
	if(!npc)
	{
		GAMELOG << init("Event Treasurebox FAIL: NOT EXIST NPC (Dangerous User)", ch) << end;
		return ;
	}
	if( !npc->IsFlag(NPC_EVENT) )
	{
		GAMELOG << init("Event Treasurebox FAIL: NOT EXIST NPC (Dangerous User)", ch) << end;
		return ;
	}
#else
	RefMsg(msg) >> type;
	RefMsg(msg) >> chaindex;
#endif

	if(type == MSG_EVENT_TREASUREBOX_TRY_REQ)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		if (ch->m_inventory.FindByDBIndex(760/*보물상자 */, 0, 0))//10레벨
		{
			level = 10;
			if(ch->m_level >= level)
				EventTreasureBoxMsg(rmsg, MSG_EVENT_TREASUREBOX_TRY_REP, (int)MSG_EVENT_TREASUREBOX_10LEVEL);
			else
				EventTreasureBoxMsg(rmsg, MSG_EVENT_TREASUREBOX_TRY_REP, (int)MSG_EVENT_TREASUREBOX_ERROR_NOT_LEVEL);
		}
		else if(ch->m_inventory.FindByDBIndex(761/*보물상자 */, 0, 0))//14레벨
		{
			level = 14;
			if(ch->m_level >= level)
				EventTreasureBoxMsg(rmsg, MSG_EVENT_TREASUREBOX_TRY_REP, (int)MSG_EVENT_TREASUREBOX_14LEVEL);
			else
				EventTreasureBoxMsg(rmsg, MSG_EVENT_TREASUREBOX_TRY_REP, (int)MSG_EVENT_TREASUREBOX_ERROR_NOT_LEVEL);
		}
		else if(ch->m_inventory.FindByDBIndex(762/*보물상자 */, 0, 0))//18레벨
		{
			level = 18;
			if(ch->m_level >= level)
				EventTreasureBoxMsg(rmsg, MSG_EVENT_TREASUREBOX_TRY_REP, (int)MSG_EVENT_TREASUREBOX_18LEVEL);
			else
				EventTreasureBoxMsg(rmsg, MSG_EVENT_TREASUREBOX_TRY_REP, (int)MSG_EVENT_TREASUREBOX_ERROR_NOT_LEVEL);
		}
		else if(ch->m_inventory.FindByDBIndex(763/*보물상자 */, 0, 0))//22레벨
		{
			level = 22;
			if(ch->m_level >= level)
				EventTreasureBoxMsg(rmsg, MSG_EVENT_TREASUREBOX_TRY_REP, (int)MSG_EVENT_TREASUREBOX_22LEVEL);
			else
				EventTreasureBoxMsg(rmsg, MSG_EVENT_TREASUREBOX_TRY_REP, (int)MSG_EVENT_TREASUREBOX_ERROR_NOT_LEVEL);
		}
		else if(ch->m_inventory.FindByDBIndex(764/*보물상자 */, 0, 0))//26레벨
		{
			level = 26;
			if(ch->m_level >= level)
				EventTreasureBoxMsg(rmsg, MSG_EVENT_TREASUREBOX_TRY_REP, (int)MSG_EVENT_TREASUREBOX_26LEVEL);
			else
				EventTreasureBoxMsg(rmsg, MSG_EVENT_TREASUREBOX_TRY_REP, (int)MSG_EVENT_TREASUREBOX_ERROR_NOT_LEVEL);
		}
		else if(ch->m_inventory.FindByDBIndex(765/*보물상자 */, 0, 0))//30레벨
		{
			level = 30;
			if(ch->m_level >= level)
				EventTreasureBoxMsg(rmsg, MSG_EVENT_TREASUREBOX_TRY_REP, (int)MSG_EVENT_TREASUREBOX_30LEVEL);
			else
				EventTreasureBoxMsg(rmsg, MSG_EVENT_TREASUREBOX_TRY_REP, (int)MSG_EVENT_TREASUREBOX_ERROR_NOT_LEVEL);
		}
		else//보물상자가 없다.
		{
			level = 0;
			EventTreasureBoxMsg(rmsg, MSG_EVENT_TREASUREBOX_TRY_REP, (int)MSG_EVENT_TREASUREBOX_ERROR_NO_BOX);
		}

		SEND_Q(rmsg, ch->m_desc);

		return;
	}
	else if(type == MSG_EVENT_TREASUREBOX_OPEN_REQ)
	{
		int itemidx;
		int treasurelevel;

		int prob = GetRandom(1, 10000);
		CItem* item = NULL;
		CItem* gift = NULL;

		if ((item = ch->m_inventory.FindByDBIndex(760/*보물상자 */, 0, 0)))//10레벨
		{
			treasurelevel = 10;
			if (prob <= 2000)
			{
				// +4 13레벨 무기.

				switch (ch->m_job)
				{
				case JOB_TITAN:
					if(prob <= 1000)
						itemidx = 105;// 기간틱블레이드
					else
						itemidx = 603;// 러커버 액스
					break;
				case JOB_KNIGHT:
					if(prob <= 1000)
						itemidx = 107;// 세퍼소드
					else
						itemidx = 612;// 세퍼 듀얼소드
					break;

				case JOB_HEALER:
					if(prob <= 1000)
						itemidx = 106;// 테일보우
					else
						itemidx = 621;// 그웰리온 완드
					break;

				case JOB_MAGE:
#ifdef EX_MAGE
				case JOB_EX_MAGE:
#endif // EX_MAGE
					if(prob <= 1000)
						itemidx = 359;// 씨덕션완드
					else
						itemidx = 630;// 일레스 스테프
					break;

				case JOB_ROGUE:
#ifdef EX_ROGUE
				case JOB_EX_ROGUE:
#endif // EX_ROGUE
					if(prob <= 1000)
						itemidx = 533;// 푸기오
					else
						itemidx = 638;// 시즈보우 석궁
					break;

				case JOB_SORCERER:
					if(prob <= 1000)
						itemidx = 979;// 시온포일 사이드
					else
						itemidx = 991;// 파스토이런 폴암
					break;
				}
				gift = gserver->m_itemProtoList.CreateItem(itemidx, -1, 4, 0, 1);
				if (!gift)
					return;

				EventTreasureBoxMsg(rMsg, MSG_EVENT_TREASUREBOX_OPEN_REP, (int)MSG_EVENT_CHANGE_13LEVEL_WEAPON);
			}
			else if(prob <= 5000)//나스
			{
				itemidx = NAS_ITEM_DB_INDEX; //나스
				gift = gserver->m_itemProtoList.CreateItem(itemidx, -1, 0, 0, 30000);
				if (!gift)
					return;

				EventTreasureBoxMsg(rMsg, MSG_EVENT_TREASUREBOX_OPEN_REP, (int)MSG_EVENT_CHANGE_NAS);
			}
			else
			{
				itemidx = 556; //사탕
				gift = gserver->m_itemProtoList.CreateItem(itemidx, -1, 0, 0, 5);
				if (!gift)
					return;

				EventTreasureBoxMsg(rMsg, MSG_EVENT_TREASUREBOX_OPEN_REP, (int)MSG_EVENT_CHANGE_CANDY);
			}
		}
		else if((item = ch->m_inventory.FindByDBIndex(761/*보물상자 */, 0, 0)))//14레벨
		{
			treasurelevel = 14;

			if (prob <= 2000)
			{
				itemidx = 510; //공격력향상제
				gift = gserver->m_itemProtoList.CreateItem(itemidx, -1, 0, 0, 10);//10개
				if (!gift)
					return;

				EventTreasureBoxMsg(rMsg, MSG_EVENT_TREASUREBOX_OPEN_REP, (int)MSG_EVENT_CHANGE_ATTACK_POTION);
			}
			else if(prob <= 5000)//나스
			{
				itemidx = NAS_ITEM_DB_INDEX; //나스
				gift = gserver->m_itemProtoList.CreateItem(itemidx, -1, 0, 0, 50000);
				if (!gift)
					return;

				EventTreasureBoxMsg(rMsg, MSG_EVENT_TREASUREBOX_OPEN_REP, (int)MSG_EVENT_CHANGE_NAS);
			}
			else
			{
				itemidx = 556; //사탕
				gift = gserver->m_itemProtoList.CreateItem(itemidx, -1, 0, 0, 10);
				if (!gift)
					return;

				EventTreasureBoxMsg(rMsg, MSG_EVENT_TREASUREBOX_OPEN_REP, (int)MSG_EVENT_CHANGE_CANDY);
			}
		}
		else if((item = ch->m_inventory.FindByDBIndex(762/*보물상자 */, 0, 0)))//18레벨
		{
			treasurelevel = 18;

			if (prob <= 2000)
			{
				// +4 21레벨 무기.

				switch (ch->m_job)
				{
				case JOB_TITAN:
					if(prob <= 1000)
						itemidx = 307;// 메테오블레이드 대검
					else
						itemidx = 605;// 브로바 액스
					break;
				case JOB_KNIGHT:
					if(prob <= 1000)
						itemidx = 322;// 노바소드
					else
						itemidx = 614;// 노바 듀얼소드
					break;

				case JOB_HEALER:
					if(prob <= 1000)
						itemidx = 341;// 크로스보우
					else
						itemidx = 623;// 사일런트 완드
					break;

				case JOB_MAGE:
#ifdef EX_MAGE
				case JOB_EX_MAGE:
#endif // EX_MAGE
					if(prob <= 1000)
						itemidx = 361;// 아크완드
					else
						itemidx = 632;// 세이라 스테프
					break;

				case JOB_ROGUE:
#ifdef EX_ROGUE
				case JOB_EX_ROGUE:
#endif // EX_ROGUE
					if(prob <= 1000)
						itemidx = 535;// 보위나이프 단검
					else
						itemidx = 640;// 임퍼테스 보우
					break;

				case JOB_SORCERER:
					if(prob <= 1000)
						itemidx = 981;// 테너일러 사이드
					else
						itemidx = 993;// 블루마그마 폴암
					break;
				}
				gift = gserver->m_itemProtoList.CreateItem(itemidx, -1, 4, 0, 1);
				if (!gift)
					return;

				EventTreasureBoxMsg(rMsg, MSG_EVENT_TREASUREBOX_OPEN_REP, (int)MSG_EVENT_CHANGE_21LEVEL_WEAPON);
			}
			else if(prob <= 5000)//나스
			{
				itemidx = NAS_ITEM_DB_INDEX; //나스
				gift = gserver->m_itemProtoList.CreateItem(itemidx, -1, 0, 0, 80000);
				if (!gift)
					return;

				EventTreasureBoxMsg(rMsg, MSG_EVENT_TREASUREBOX_OPEN_REP, (int)MSG_EVENT_CHANGE_NAS);
			}
			else
			{
				itemidx = 556; //사탕
				gift = gserver->m_itemProtoList.CreateItem(itemidx, -1, 0, 0, 15);
				if (!gift)
					return;

				EventTreasureBoxMsg(rMsg, MSG_EVENT_TREASUREBOX_OPEN_REP, (int)MSG_EVENT_CHANGE_CANDY);
			}
		}
		else if((item = ch->m_inventory.FindByDBIndex(763/*보물상자 */, 0, 0)))//22레벨
		{
			treasurelevel = 22;

			if (prob <= 2000)
			{
				itemidx = 511; //방어력향상제
				gift = gserver->m_itemProtoList.CreateItem(itemidx, -1, 0, 0, 15);//15개
				if (!gift)
					return;

				EventTreasureBoxMsg(rMsg, MSG_EVENT_TREASUREBOX_OPEN_REP, (int)MSG_EVENT_CHANGE_DEFENSE_POTION);
			}
			else if(prob <= 5000)//나스
			{
				itemidx = NAS_ITEM_DB_INDEX; //나스
				gift = gserver->m_itemProtoList.CreateItem(itemidx, -1, 0, 0, 100000);
				if (!gift)
					return;

				EventTreasureBoxMsg(rMsg, MSG_EVENT_TREASUREBOX_OPEN_REP, (int)MSG_EVENT_CHANGE_NAS);
			}
			else
			{
				itemidx = 556; //사탕
				gift = gserver->m_itemProtoList.CreateItem(itemidx, -1, 0, 0, 20);
				if (!gift)
					return;

				EventTreasureBoxMsg(rMsg, MSG_EVENT_TREASUREBOX_OPEN_REP, (int)MSG_EVENT_CHANGE_CANDY);
			}
		}
		else if((item = ch->m_inventory.FindByDBIndex(764/*보물상자 */, 0, 0)))//26레벨
		{
			treasurelevel = 26;

			if (prob <= 2000)
			{
				// +4 29레벨 무기.

				switch (ch->m_job)
				{
				case JOB_TITAN:
					if(prob <= 1000)
						itemidx = 309;// 폴브레이크 대검
					else
						itemidx = 607;// 크레센트 액스
					break;
				case JOB_KNIGHT:
					if(prob <= 1000)
						itemidx = 324;// 소울크래셔
					else
						itemidx = 616;// 더블 소울크래셔
					break;

				case JOB_HEALER:
					if(prob <= 1000)
						itemidx = 343;// 글로우보우
					else
						itemidx = 625;// 페이스 완드
					break;

				case JOB_MAGE:
#ifdef EX_MAGE
				case JOB_EX_MAGE:
#endif
					if(prob <= 1000)
						itemidx = 363;// 커미셔완드
					else
						itemidx = 634;// 이터널 스테프
					break;

				case JOB_ROGUE:
#ifdef EX_ROGUE
				case JOB_EX_ROGUE:
#endif // EX_ROGUE
					if(prob <= 1000)
						itemidx = 537;// 푸아냐르 단검
					else
						itemidx = 642;// 드미트리 보우
					break;

				case JOB_SORCERER:
					if(prob <= 1000)
						itemidx = 983;// 파이언스톰 사이드
					else
						itemidx = 995;// 포세이던트 폴암
					break;
				}
				gift = gserver->m_itemProtoList.CreateItem(itemidx, -1, 4, 0, 1);
				if (!gift)
					return;

				EventTreasureBoxMsg(rMsg, MSG_EVENT_TREASUREBOX_OPEN_REP, (int)MSG_EVENT_CHANGE_29LEVEL_WEAPON);
			}
			else if(prob <= 5000)//나스
			{
				itemidx = NAS_ITEM_DB_INDEX; //나스
				gift = gserver->m_itemProtoList.CreateItem(itemidx, -1, 0, 0, 150000);
				if (!gift)
					return;

				EventTreasureBoxMsg(rMsg, MSG_EVENT_TREASUREBOX_OPEN_REP, (int)MSG_EVENT_CHANGE_NAS);
			}
			else
			{
				itemidx = 556; //사탕
				gift = gserver->m_itemProtoList.CreateItem(itemidx, -1, 0, 0, 30);
				if (!gift)
					return;

				EventTreasureBoxMsg(rMsg, MSG_EVENT_TREASUREBOX_OPEN_REP, (int)MSG_EVENT_CHANGE_CANDY);
			}
		}
		else if((item = ch->m_inventory.FindByDBIndex(765/*보물상자 */, 0, 0)))//30레벨
		{
			treasurelevel = 30;
			if (prob <= 2000)
			{
#ifdef LC_TLD
				itemidx = PET_BLUE_HORSE_EGG_INDEX; //믈루 판의 피리
				gift = gserver->m_itemProtoList.CreateItem(itemidx, -1, 0, 0, 1);//1개
				if (!gift)
					return;
				EventTreasureBoxMsg(rMsg, MSG_EVENT_TREASUREBOX_OPEN_REP, (int)MSG_EVENT_CHANGE_BLUE_HORSE);
#else
				itemidx = 510; //공격력향상제
				gift = gserver->m_itemProtoList.CreateItem(itemidx, -1, 0, 0, 20);//20개
				if (!gift)
					return;
				EventTreasureBoxMsg(rMsg, MSG_EVENT_TREASUREBOX_OPEN_REP, (int)MSG_EVENT_CHANGE_ATTACK_POTION);
#endif // LC_TLD
			}
			else if(prob <= 5000)//나스
			{
				itemidx = NAS_ITEM_DB_INDEX; //나스
				gift = gserver->m_itemProtoList.CreateItem(itemidx, -1, 0, 0, 200000);
				if (!gift)
					return;

				EventTreasureBoxMsg(rMsg, MSG_EVENT_TREASUREBOX_OPEN_REP, (int)MSG_EVENT_CHANGE_NAS);
			}
			else
			{
#ifdef LC_TLD
				itemidx = PET_PINK_DRAGON_EGG_INDEX; //핑크 드레이크알
				gift = gserver->m_itemProtoList.CreateItem(itemidx, -1, 0, 0, 1);//1개
				if (!gift)
					return;
				EventTreasureBoxMsg(rMsg, MSG_EVENT_TREASUREBOX_OPEN_REP, (int)MSG_EVENT_CHANGE_PINK_DRAGON);
#else
				itemidx = 556; //사탕
				gift = gserver->m_itemProtoList.CreateItem(itemidx, -1, 0, 0, 40);
				if (!gift)
					return;
				EventTreasureBoxMsg(rMsg, MSG_EVENT_TREASUREBOX_OPEN_REP, (int)MSG_EVENT_CHANGE_CANDY);
#endif // LC_TLD
			}
		}
		else//보물상자가 없다.
		{
			CNetMsg::SP rmsg(new CNetMsg);
			EventTreasureBoxMsg(rmsg, MSG_EVENT_TREASUREBOX_OPEN_REP, (int)MSG_EVENT_CHANGE_NO_TREASUREBOX);
			SEND_Q(rmsg, ch->m_desc);
			return;
		}

		//보물상자를 지운다.
		if(treasurelevel == 0)
			return;

// [101229: selo] 보상 아이템 드롭 수정
		if( !ch->CheckInvenForTresureBoxPrize(gift) )
		{
			return;
		}

		if (item == NULL || item->Count() < 1)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			EventTreasureBoxMsg(rmsg, MSG_EVENT_TREASUREBOX_OPEN_REP, (int)MSG_EVENT_CHANGE_NO_TREASUREBOX);
			SEND_Q(rmsg, ch->m_desc);
			return;
		}

		// 보물상자를 삭제.
		ch->m_inventory.decreaseItemCount(item, 1);

		bool nasFlag = (gift->getDBIndex() == NAS_ITEM_DB_INDEX) ? true : false;

		// 인벤에 넣기
		if (ch->m_inventory.addItem(gift) == false)
		{
			// 인벤토리 가득참
			gift = ch->m_pArea->DropItem(gift, ch);

			CNetMsg::SP rmsg(new CNetMsg);
			gift->m_preferenceIndex = ch->m_index;
			ItemDropMsg(rmsg, ch, gift);
			ch->m_pArea->SendToCell(rmsg, ch, true);
			SEND_Q(rmsg, ch->m_desc);
			return;
		}
		// 메시지 보내고
		SEND_Q(rMsg, ch->m_desc);

		if (nasFlag == false)
		{
			// Item LOG
			GAMELOG << init("CHANGE_TREASUREBOX", ch)
					<< itemlog(gift)
					<< end;
		}

// 다음 레벨 보물상자를 준다.
		switch(treasurelevel)
		{
		case 10:
			itemidx =761;
			break;
		case 14:
			itemidx =762;
			break;
		case 18:
			itemidx =763;
			break;
		case 22:
			itemidx =764;
			break;
		case 26:
			itemidx =765;
			break;
		case 30:
			return;
		}

		gift = gserver->m_itemProtoList.CreateItem(itemidx, -1, 0, 0, 1);
		if (gift == NULL)
			return;

		if (ch->m_inventory.addItem(gift) == false)
		{
			// 인벤토리 가득참
			gift = ch->m_pArea->DropItem(gift, ch);
			if (!gift)
			{
				return;
			}

			CNetMsg::SP rmsg(new CNetMsg);
			gift->m_preferenceIndex = ch->m_index;
			ItemDropMsg(rmsg, ch, gift);
			ch->m_pArea->SendToCell(rmsg, ch, true);
			SEND_Q(rmsg, ch->m_desc);
			return;
		}
		// Item LOG
		GAMELOG << init("GIVE_TREASUREBOX", ch)
				<< itemlog(gift)
				<< end;
	}
}
#endif // EVENT_TREASUREBOX

void do_EventLatto(CPC* ch, CNetMsg::SP& msg)
{
	return ;
}

void do_EventChangeWeapon(CPC* ch, CNetMsg::SP& msg)
{
#ifdef EVENT_CHANGEWEAPON
	CItem* olditem = ch->m_wearInventory.wearItemInfo[WEARING_WEAPON];

	if (olditem == NULL)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		EventErrorMsg(rmsg, MSG_EVENT_ERROR_WRONG_PACKET);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if (!olditem->IsWeaponType())
	{
		CNetMsg::SP rmsg(new CNetMsg);
		EventErrorMsg(rmsg, MSG_EVENT_ERROR_WRONG_PACKET);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if (olditem->IsRareItem())
	{
		CNetMsg::SP rmsg(new CNetMsg);
		EventErrorMsg(rmsg, MSG_EVENT_ERROR_WRONG_PACKET);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if (olditem->IsOriginItem())
	{
		CNetMsg::SP rmsg(new CNetMsg);
		EventErrorMsg(rmsg, MSG_EVENT_ERROR_WRONG_PACKET);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	int newtype = -1;
	switch (olditem->m_itemProto->getItemSubTypeIdx())
	{
	case IWEAPON_NIGHT:
		newtype = IWEAPON_TWOSWORD;
		break;
	case IWEAPON_CROSSBOW:
		newtype = IWEAPON_SHORTGUM;
		break;
	case IWEAPON_STAFF:
		newtype = IWEAPON_SHORTSTAFF;
		break;
	case IWEAPON_BIGSWORD:
		newtype = IWEAPON_AXE;
		break;
	case IWEAPON_AXE:
		newtype = IWEAPON_BIGSWORD;
		break;
	case IWEAPON_SHORTSTAFF:
		newtype = IWEAPON_STAFF;
		break;
	case IWEAPON_BOW:
		newtype = IWEAPON_WAND;
		break;
	case IWEAPON_SHORTGUM:
		newtype = IWEAPON_CROSSBOW;
		break;
	case IWEAPON_TWOSWORD:
		newtype = IWEAPON_NIGHT;
		break;
	case IWEAPON_WAND:
		newtype = IWEAPON_BOW;
		break;
	case IWEAPON_SCYTHE:
		newtype = IWEAPON_POLEARM;
		break;
	case IWEAPON_POLEARM:
		newtype = IWEAPON_SCYTHE;
		break;
	case IWEAPON_SOUL:
		newtype = IWEAPON_SOUL;
		break;
	default:
		{
			CNetMsg::SP rmsg(new CNetMsg);
			EventErrorMsg(rmsg, MSG_EVENT_ERROR_WRONG_PACKET);
			SEND_Q(rmsg, ch->m_desc);
		}
		return ;
	}

	int newindex = -1;
	int i;
	for (i = 0; newindex == -1 && i < gserver->m_itemProtoList.m_nCount; i++)
	{
		if (gserver->m_itemProtoList.m_protoItems[i].m_typeIdx == ITYPE_WEAPON)
		{
			if (gserver->m_itemProtoList.m_protoItems[i].m_subtypeIdx == newtype)
			{
				if (gserver->m_itemProtoList.m_protoItems[i].m_level == olditem->m_itemProto->getItemLevel())
				{
					newindex = gserver->m_itemProtoList.m_protoItems[i].m_index;
					break;
				}
			}
		}
	}

	if (newindex == -1)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		EventErrorMsg(rmsg, MSG_EVENT_ERROR_NO_MATCH);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	CItem* newitem = gserver->m_itemProtoList.CreateItem(newindex, -1, olditem->getPlus(), olditem->getFlag(), 1);
	if (newitem == NULL)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		EventErrorMsg(rmsg, MSG_EVENT_ERROR_NO_MATCH);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}
	newitem->setUsed(olditem->getUsed());

	// 옵션 옮기기
	switch (olditem->m_itemProto->getItemSubTypeIdx())
	{
	case IWEAPON_NIGHT:
	case IWEAPON_STAFF:
	case IWEAPON_BIGSWORD:
	case IWEAPON_AXE:
	case IWEAPON_SHORTSTAFF:
	case IWEAPON_MINING:
	case IWEAPON_GATHERING:
	case IWEAPON_CHARGE:
	case IWEAPON_TWOSWORD:
	case IWEAPON_SCYTHE: // 할일 : 소서러 : 사이드 폴암의 옵션 변경
	case IWEAPON_POLEARM:
		newitem->m_nOption = 0;
		for (i = 0; i < olditem->m_nOption; i++)
		{
			if (!olditem->m_option[i].m_proto)
				continue ;
			newitem->m_option[newitem->m_nOption].m_proto	= olditem->m_option[i].m_proto;
			newitem->m_option[newitem->m_nOption].m_type	= olditem->m_option[i].m_type;
			newitem->m_option[newitem->m_nOption].m_level	= olditem->m_option[i].m_level;
			++newitem->m_nOption;
		}
		break;

	case IWEAPON_BOW:
		newitem->m_nOption = 0;
		for (i = 0; i < olditem->m_nOption; i++)
		{
			if (!olditem->m_option[i].m_proto)
				continue ;

			COptionProto*	tempProto = NULL;

			switch (olditem->m_option[i].m_proto->m_type)
			{
			case 6:		// 물리 공격 -> 지혜
				tempProto	= gserver->m_optionProtoList.FindProto(2);
				break;

			case 8:		// 원거리 공격 -> 마법 공격
				tempProto	= gserver->m_optionProtoList.FindProto(16);
				break;

			case 10:	// 원거리 명중 -> 마법 명중
				tempProto	= gserver->m_optionProtoList.FindProto(17);
				break;

			default:
				tempProto	= olditem->m_option[i].m_proto;
				break;
			}
			if (tempProto == NULL)
				continue;

			newitem->m_option[newitem->m_nOption].m_proto	= tempProto;
			newitem->m_option[newitem->m_nOption].m_type	= tempProto->m_type;
			newitem->m_option[newitem->m_nOption].m_level	= olditem->m_option[i].m_level;
			++newitem->m_nOption;
		}
		break;

	case IWEAPON_WAND:
		newitem->m_nOption = 0;
		for (i = 0; i < olditem->m_nOption; i++)
		{
			if (!olditem->m_option[i].m_proto)
				continue ;

			COptionProto*	tempProto = NULL;

			switch (olditem->m_option[i].m_proto->m_type)
			{
			case 2:		// 지혜 -> 물리 공격
				tempProto	= gserver->m_optionProtoList.FindProto(6);
				break;

			case 16:	// 마법 공격 -> 원거리 공격
				tempProto	= gserver->m_optionProtoList.FindProto(8);
				break;

			case 17:	// 마법 명중 -> 원거리 명중
				tempProto	= gserver->m_optionProtoList.FindProto(10);
				break;

			default:
				tempProto	= olditem->m_option[i].m_proto;
				break;
			}

			if (tempProto == NULL)
				continue;

			newitem->m_option[newitem->m_nOption].m_proto	= tempProto;
			newitem->m_option[newitem->m_nOption].m_type	= tempProto->m_type;
			newitem->m_option[newitem->m_nOption].m_level	= olditem->m_option[i].m_level;
			++newitem->m_nOption;
		}
		break;

	case IWEAPON_CROSSBOW:
		newitem->m_nOption = 0;
		for (i = 0; i < olditem->m_nOption; i++)
		{
			if (!olditem->m_option[i].m_proto)
				continue ;

			COptionProto*	tempProto = NULL;

			switch (olditem->m_option[i].m_proto->m_type)
			{
			case 8:		// 원거리 공격 -> 근접 공격
				tempProto	= gserver->m_optionProtoList.FindProto(7);
				break;

			case 10:	// 원거리 명중 -> 근접 명중
				tempProto	= gserver->m_optionProtoList.FindProto(9);
				break;

			default:
				tempProto	= olditem->m_option[i].m_proto;
				break;
			}

			if (tempProto == NULL)
				continue;

			newitem->m_option[newitem->m_nOption].m_proto	= tempProto;
			newitem->m_option[newitem->m_nOption].m_type	= tempProto->m_type;
			newitem->m_option[newitem->m_nOption].m_level	= olditem->m_option[i].m_level;
			++newitem->m_nOption;
		}
		break;

	case IWEAPON_SHORTGUM:
		newitem->m_nOption = 0;
		for (i = 0; i < olditem->m_nOption; i++)
		{
			if (!olditem->m_option[i].m_proto)
				continue ;

			COptionProto*	tempProto = NULL;

			switch (olditem->m_option[i].m_proto->m_type)
			{
			case 7:		// 근접 공격 -> 원거리 공격
				tempProto	= gserver->m_optionProtoList.FindProto(8);
				break;

			case 9:		// 근접 명중 -> 원거리 명중
				tempProto	= gserver->m_optionProtoList.FindProto(10);
				break;

			default:
				tempProto	= olditem->m_option[i].m_proto;
				break;
			}

			if (tempProto == NULL)
				continue;

			newitem->m_option[newitem->m_nOption].m_proto	= tempProto;
			newitem->m_option[newitem->m_nOption].m_type	= tempProto->m_type;
			newitem->m_option[newitem->m_nOption].m_level	= olditem->m_option[i].m_level;
			++newitem->m_nOption;
		}
		break;
	}

	if (olditem->m_nOption != newitem->m_nOption)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		EventErrorMsg(rmsg, MSG_EVENT_ERROR_CANT_OPTION);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	for (i = 0; i < newitem->m_nOption; i++)
	{
		if (!newitem->m_option[i].m_proto)
			continue ;
		newitem->m_option[i].m_value = newitem->m_option[i].m_proto->m_levelValue[newitem->m_option[i].m_level - 1];
		newitem->m_option[i].SetDBValue();
	}

	GAMELOG << init("EVENT CHANGE WEAPON", ch)
			<< "OLD ITEM" << delim
			<< itemlog(olditem) << delim;
}
GAMELOG << "NEW ITEM" << delim
		<< itemlog(newitem)
		<< end;

int olddb = olditem->m_itemProto->getItemIndex();
int newdb = newitem->m_itemProto->getItemIndex();

{
	CNetMsg::SP rmsg(new CNetMsg);
	ResponseClient::ItemWearMsg(rmsg, WEARING_WEAPON, NULL, olditem);
	SEND_Q(rmsg, ch->m_desc);
}

{
	CNetMsg::SP rmsg(new CNetMsg);
	WearingMsg(rmsg, ch, WEARING_WEAPON, -1, 0);
	ch->m_pArea->SendToCell(rmsg, ch, true);
}

ch->m_wearInventory.removeItem(olditem);
ch->m_invnentory.addItem(newitem);

ch->m_assist.CheckApplyConditions();
ch->CalcStatus(true);
#endif
}

void do_EventChuseokUpgrade(CPC* ch, CNetMsg::SP& msg)
{
#ifndef LC_TLD
	// 이벤트 인벤에 작물 있나? : 열매 제외
	CItem* crop = NULL;
	int grade = -1;

	if ((crop = ch->m_inventory.FindByDBIndex(876, 0, 0)))			grade = 0;	// 씨앗
	else if ((crop = ch->m_inventory.FindByDBIndex(877, 0, 0)))		grade = 1;	// 묘목
	else if ((crop = ch->m_inventory.FindByDBIndex(878, 0, 0)))		grade = 2;	// 분재
	else if ((crop = ch->m_inventory.FindByDBIndex(879, 0, 0)))		grade = 3;	// 나무
	else if ((crop = ch->m_inventory.FindByDBIndex(880, 0, 0)))		grade = 4;	// 열매

	int prob = 0;
	int newCropIndex = 0;
	switch (grade)
	{
	case 0:
		prob = 100;
		newCropIndex = 877;
		break;
	case 1:
		prob = 70;
		newCropIndex = 878;
		break;
	case 2:
		prob = 50;
		newCropIndex = 879;
		break;
	case 3:
		prob = 30;
		newCropIndex = 880;
		break;
	case 4:
		{
			CNetMsg::SP rmsg(new CNetMsg);
			EventErrorMsg(rmsg, MSG_EVENT_ERROR_CHUSEOK_ALREADY_UPGRADE);
			SEND_Q(rmsg, ch->m_desc);
		}
		return ;
	default:
		{
			CNetMsg::SP rmsg(new CNetMsg);
			EventErrorMsg(rmsg, MSG_EVENT_ERROR_CHUSEOK_NO_CROP);
			SEND_Q(rmsg, ch->m_desc);
		}
		return ;
	}

	// 배양토 검색
	CItem* soil = ch->m_inventory.FindByDBIndex(881, 0, 0);
	if (soil == NULL)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		EventErrorMsg(rmsg, MSG_EVENT_ERROR_CHUSEOK_NO_SOIL);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if (crop == NULL)
	{
		return;
	}

	CItem* newCrop = gserver->m_itemProtoList.CreateItem(newCropIndex, -1, 0, 0, 1);
	if (newCrop == NULL)
		return ;

	{
		// 배양토 감소
		ch->m_inventory.decreaseItemCount(soil, 1);
	}

	// 확률 적용
	if (GetRandom(1, 100) <= prob)
	{
		ch->m_inventory.decreaseItemCount(crop, 1);

		// 새 작물 추가
		bool bDrop = false;
		if (ch->m_inventory.addItem(newCrop) == false)
		{
			newCrop = ch->m_pArea->DropItem(newCrop, ch);
			if (newCrop == NULL)
				return;

			newCrop->m_preferenceIndex = ch->m_index;

			CNetMsg::SP rmsg(new CNetMsg);
			ItemDropMsg(rmsg, ch, newCrop);
			SEND_Q(rmsg, ch->m_desc);
			bDrop = true;
		}

		CNetMsg::SP rmsg(new CNetMsg);
		EventErrorMsg(rmsg, MSG_EVENT_ERROR_CHUSEOK_UPGRADE_OK);
		SEND_Q(rmsg, ch->m_desc);

		GAMELOG << init("EVENT CHUSEOK UPGRADE SUCCESS", ch)
				<< grade << delim
				<< (grade + 1)
				<< ((bDrop) ? "DROP" : "GET")
				<< end;
	}
	else
	{
		delete newCrop;

		CNetMsg::SP rmsg(new CNetMsg);
		EventErrorMsg(rmsg, MSG_EVENT_ERROR_CHUSEOK_UPGRADE_FAIL);
		SEND_Q(rmsg, ch->m_desc);

		GAMELOG << init("EVENT CHUSEOK UPGRADE FAIL", ch)
				<< grade
				<< end;
	}
#endif
}

void do_EventChuseokExchange(CPC* ch, CNetMsg::SP& msg)
{
#ifndef LC_TLD
	// 확률 테이블 : 누적확률%
	const int probTable[5][5] =
	{
		{ 70,  85,  95, 100, 100},		// 씨앗 : 70 15 10 5 0
		{ 35,  65,  85,  95, 100},		// 묘목 : 35 30 20 10 5
		{ 20,  45,  75,  90, 100},		// 분재 : 20 25 30 15 10
		{ 10,  25,  50,  80, 100},		// 나무 : 10 15 25 30 20
		{  0,  10,  40,  70, 100}		// 열매 : 0 10 30 30 30
	};

	// 작물 찾기
	CItem* crop = NULL;
	int grade = -1;
	if ((crop = ch->m_inventory.FindByDBIndex(876, 0, 0)))			grade = 0;	// 씨앗
	else if ((crop = ch->m_inventory.FindByDBIndex(877, 0, 0)))		grade = 1;	// 묘목
	else if ((crop = ch->m_inventory.FindByDBIndex(878, 0, 0)))		grade = 2;	// 분재
	else if ((crop = ch->m_inventory.FindByDBIndex(879, 0, 0)))		grade = 3;	// 나무
	else if ((crop = ch->m_inventory.FindByDBIndex(880, 0, 0)))		grade = 4;	// 열매
	else
	{
		CNetMsg::SP rmsg(new CNetMsg);
		EventErrorMsg(rmsg, MSG_EVENT_ERROR_CHUSEOK_NO_CROP);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if (crop == NULL)
		return ;

	// 확률 적용
	int prob = GetRandom(1, 100);
	int i = 0;
	for (i = 0; i < 5; i++)
	{
		if (prob <= probTable[grade][i])
			break;
	}

	int itemindex = 0;
	LONGLONG itemcount = 0;

	switch (i)
	{
	case 0:		// 10000 나스
		itemindex = NAS_ITEM_DB_INDEX;
		itemcount = 10000;
		break;
	case 1:		// 50000 나스
		itemindex = NAS_ITEM_DB_INDEX;
		itemcount = 50000;
		break;
	case 2:		// 200000 나스
		itemindex = NAS_ITEM_DB_INDEX;
		itemcount = 200000;
		break;
	case 3:		// 500000 나스
		itemindex = NAS_ITEM_DB_INDEX;
		itemcount = 500000;
		break;
	case 4:		// 고제 2 개
		itemindex = gserver->m_itemProtoList.m_specialRefineItem->getItemIndex();
		itemcount = 2;
		break;
	default:
		return ;
	}

	CItem* item = gserver->m_itemProtoList.CreateItem(itemindex, -1, 0, 0, itemcount);
	if (item == NULL)
		return ;

	// 작물 수거
	ch->m_inventory.decreaseItemCount(crop, 1);

	// 아이템 지급
	bool bDrop = false;
	if (ch->m_inventory.addItem(item) == false)
	{
		ch->m_pArea->DropItem(item, ch);
		item->m_preferenceIndex = ch->m_index;

		CNetMsg::SP rmsg(new CNetMsg);
		ItemDropMsg(rmsg, ch, item);
		SEND_Q(rmsg, ch->m_desc);

		bDrop = true;
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		EventErrorMsg(rmsg, MSG_EVENT_ERROR_CHUSEOK_EXCHANGE_OK);
		SEND_Q(rmsg, ch->m_desc);
	}

	GAMELOG << init("EVENT CHUSEOK EXCHANGE", ch)
			<< "GRADE" << delim
			<< grade << delim
			<< "ITEM" << delim
			<< itemindex << delim
			<< itemcount << delim
			<< ((bDrop) ? "DROP" : "GET")
			<< end;
#endif
}

#ifdef EVENT_PACKAGE_ITEM
void do_EventPackageItemGive(CPC* ch, CNetMsg::SP& msg)
{
	CLCString couponNo(21);

	couponNo = "";

	RefMsg(msg) >> couponNo;

	if( couponNo.Length() > 0 )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ConnCouponConfirm(rmsg, ch->m_index, couponNo);
#ifdef LC_GAMIGO
		RefMsg(rmsg) << ch->m_desc->m_index;
#endif // LC_GAMIGO

		SEND_Q(rmsg, gserver->m_connector);
	}
}
#endif

void do_EventWorldcup(CPC* ch, CNetMsg::SP& msg)
{
	// FIFA 컵 아이템 인덱스
	static const int nFIFACupIndex = 1484;

	// 국가 테이블 : 국가 번호, FIFA컵 개수, 국기 Index, 순위(1, 2, 3, 4)
#define TABLE_REF_COUNTRY		0
#define TABLE_REF_FIFACUP		1
#define TABLE_REF_FLAG			2
#define TABLE_REF_RANK			3
	static const int nCountOfCountry = 32;
	static const int tableCountry[nCountOfCountry][4] =
	{
		{ 0,	100,	1485,		4},		// 브라질
		{ 1,	100,	1486,		3},		// 독일
		{ 2,	100,	1487,		1},		// 이탈리아
		{ 3,	100,	1488,		4},		// 영국
		{ 4,	100,	1489,		4},		// 아르헨티나
		{ 5,	 75,	1490,		2},		// 프랑스
		{ 6,	 75,	1491,		4},		// 네덜란드
		{ 7,	 75,	1492,		4},		// 스페인
		{ 8,	 75,	1493,		4},		// 포르투갈
		{ 9,	 75,	1494,		4},		// 체코
		{10,	 75,	1495,		4},		// 스웨덴
		{11,	 75,	1496,		4},		// 멕시코
		{12,	 75,	1497,		4},		// 우크라이나
		{13,	 75,	1498,		4},		// 크로아티아
		{14,	 50,	1499,		4},		// 미국
		{15,	 50,	1500,		4},		// 스위스
		{16,	 50,	1501,		4},		// 코트디부아르
		{17,	 50,	1502,		4},		// 세르비아-몬테네그로
		{18,	 50,	1503,		4},		// 폴란드
		{19,	 50,	1504,		4},		// 파라과이
		{20,	 50,	1505,		4},		// 대한민국
		{21,	 50,	1506,		4},		// 호주
		{22,	 50,	1507,		4},		// 일본
		{23,	 30,	1508,		4},		// 가나
		{24,	 30,	1509,		4},		// 튀니지
		{25,	 30,	1510,		4},		// 에콰도르
		{26,	 30,	1511,		4},		// 토고
		{27,	 30,	1512,		4},		// 앙골라
		{28,	 30,	1513,		4},		// 이란
		{29,	 30,	1514,		4},		// 사우디아라비아
		{30,	 30,	1515,		4},		// 코스타리카
		{31,	 30,	1516,		4},		// 트리니다드 토바고
	};

	// 보상 테이블 : 보상 아이템 번호, 보상 아이템 수량
	static const int tableGift[4][4] =
	{
		{ 85,	5},			// 1 위		고제 5개
		{ 85,	2},			// 2 위		고제 2개
		{ 19,	300000},	// 3 위		나스 300,000
		{723,	5},			// 그외		문스톤 5개
	};

	int nSubType = -1;
	RefMsg(msg) >> nSubType;

	// 공용 변수
	int nCountryIndex = -1;

	// VOTE용
	int nCountry = 0;
	CItem* pItemCountry = NULL;
	CItem* pItemFIFACup = NULL;

	// GIFT용
	int nRank = 0;
	int nRowFlagItem, nColFlagItem;
	std::vector<int> listFlagItemIndex;
	int nGiftItemIndex = -1;
	LONGLONG nGiftItemCount = 0;
	CItem* pItemFlag = NULL;
	CItem* pItemGift = NULL;

	switch (nSubType)
	{
	// 응모 처리
	case MSG_EVENT_WORLDCUP_VOTE:
		{

			RefMsg(msg) >> nCountry;

			// 1. 응모 기간 검사
			// 2. 테이블에서 해당 국가 찾기
			// 3. 해당 국가 응모 검사 : 절대 같은 국기를 소유해선 안됨
			// 4. FIFA 컵 수량 검사
			// 5. 국기 생성
			// 6. 국기 지급
			// 7. FIFA 컵 회수
			// 8. 결과 알림
			// 9. 게임 로그

			// 1. 응모 기간 검사
			{
				CNetMsg::SP rmsg(new CNetMsg);
				EventWorldcupVoteMsg(rmsg, -1, MSG_EVENT_WORLDCUP_ERROR_VOTE_NOTCHANGE);
				SEND_Q(rmsg, ch->m_desc);
			}
			return ;

			// 2. 테이블에서 해당 국가 찾기
			for (nCountryIndex = 0; nCountryIndex < nCountOfCountry; nCountryIndex++)
			{
				if (tableCountry[nCountryIndex][TABLE_REF_COUNTRY] == nCountry)
					break;
			}
			if (nCountryIndex == nCountOfCountry)
				return ;

			// 3. 해당 국가 응모 검사 : 절대 같은 국기를 소유해선 안됨
			if (ch->m_inventory.FindByDBIndex(tableCountry[nCountryIndex][TABLE_REF_FLAG], 0, 0))
			{
				CNetMsg::SP rmsg(new CNetMsg);
				EventWorldcupVoteMsg(rmsg, -1, MSG_EVENT_WORLDCUP_ERROR_VOTE_ALREADY);
				SEND_Q(rmsg, ch->m_desc);
				return ;
			}

			// 4. FIFA 컵 수량 검사
			item_search_t vec;
			int sc = ch->m_inventory.searchItemByCondition(nFIFACupIndex, 0, 0, vec);
			if (sc == 0 || sc < tableCountry[nCountryIndex][TABLE_REF_FIFACUP])
			{
				CNetMsg::SP rmsg(new CNetMsg);
				EventWorldcupVoteMsg(rmsg, -1, MSG_EVENT_WORLDCUP_ERROR_VOTE_NOITEM);
				SEND_Q(rmsg, ch->m_desc);
				return ;
			}

			// 5. 국기 생성
			pItemCountry = gserver->m_itemProtoList.CreateItem(tableCountry[nCountryIndex][TABLE_REF_FLAG], -1, 0, 0, 1);
			if (!pItemCountry)
			{
				CNetMsg::SP rmsg(new CNetMsg);
				EventWorldcupVoteMsg(rmsg, -1, MSG_EVENT_WORLDCUP_ERROR_VOTE_NOITEM);
				SEND_Q(rmsg, ch->m_desc);
				return ;
			}

			// 6. 국기 지급
			if (ch->m_inventory.addItem(pItemCountry) == false)
			{
				delete pItemCountry;

				CNetMsg::SP rmsg(new CNetMsg);
				EventWorldcupVoteMsg(rmsg, -1, MSG_EVENT_WORLDCUP_ERROR_VOTE_FULLINVEN);
				SEND_Q(rmsg, ch->m_desc);
				return ;
			}

			// 7. FIFA 컵 회수
			ch->m_inventory.deleteItem(vec, sc);

			{
				// 8. 결과 알림
				CNetMsg::SP rmsg(new CNetMsg);
				EventWorldcupVoteMsg(rmsg, tableCountry[nCountryIndex][TABLE_REF_COUNTRY], MSG_EVENT_WORLDCUP_ERROR_VOTE_OK);
				SEND_Q(rmsg, ch->m_desc);
			}

			// 9. 게임 로그
			GAMELOG << init("EVENT WORLDCUP 2006 VOTE", ch)
					<< "COUNTRY" << delim
					<< tableCountry[nCountryIndex][TABLE_REF_COUNTRY] << delim
					<< "FIFA CUP" << delim
					<< tableCountry[nCountryIndex][TABLE_REF_FIFACUP] << delim
					<< "REMAIN" << delim;
			if (pItemFIFACup && pItemFIFACup->Count() > 0)
				GAMELOG << pItemFIFACup->Count();
			else
				GAMELOG << 0;
			GAMELOG << end;
		}
		break;

	// 보상 처리
	case MSG_EVENT_WORLDCUP_GIFT:
		{
			RefMsg(msg) >> nRank;

			// 1. 보상 기간 검사
			// 2. 순위권 국가 검사
			// 2.1.1. 순위권 국기 소유 검사
			// 2.1.2. 보상품 결정
			// 2.2.1. 비 순위권 국기 소유 리스트 작성
			// 2.2.2. 보상품 및 수량 결정
			// 3. 보상품 생성
			// 4. 보상품 지급
			// 5. 게임 로그 1
			// 6. 국기 회수
			// 7. 결과 알림
			// 8. 게임 로그 2

			// 1. 보상 기간 검사
			{
				CNetMsg::SP rmsg(new CNetMsg);
				EventWorldcupGiftMsg(rmsg, nRank, MSG_EVENT_WORLDCUP_ERROR_GIFT_NOTCHANGE);
				SEND_Q(rmsg, ch->m_desc);
			}
			return ;

			// 2. 순위권 국가 검사
			if (1 <= nRank && nRank <= 3)
			{
				// 2.1.1. 순위권 국기 소유 검사
				for (nCountryIndex = 0; nCountryIndex < nCountOfCountry; nCountryIndex++)
				{
					if (tableCountry[nCountryIndex][TABLE_REF_RANK] == nRank)
						break;
				}
				if (nCountryIndex == nCountOfCountry)
					return ;

				pItemFlag = ch->m_inventory.FindByDBIndex(tableCountry[nCountryIndex][TABLE_REF_FLAG], 0, 0);
				if (pItemFlag == NULL)
				{
					CNetMsg::SP rmsg(new CNetMsg);
					EventWorldcupGiftMsg(rmsg, nRank, MSG_EVENT_WORLDCUP_ERROR_GIFT_NOITEM);
					SEND_Q(rmsg, ch->m_desc);
					return ;
				}

				listFlagItemIndex.push_back(pItemFlag->getVIndex());
				pItemFlag = NULL;

				// 2.1.2. 보상품 결정
				nGiftItemIndex = tableGift[nRank - 1][0];
				nGiftItemCount = tableGift[nRank - 1][1];
			}
			else
			{
				nRank = 4;

				// 2.2.1. 비 순위권 국기 소유 리스트 작성
				// 32개국 국기 아이템 인덱스와 비교하여
				for (nCountryIndex = 0; nCountryIndex < nCountOfCountry; nCountryIndex++)
				{
					CItem* pItemFlag = ch->m_inventory.FindByDBIndex(tableCountry[nCountryIndex][TABLE_REF_FLAG], 0, 0);
					if (pItemFlag == NULL)
						continue;

					// 32개국 국기 중 하나면
					// 순위권 국가가 아닌지 검사
					if (tableCountry[nCountryIndex][TABLE_REF_RANK] <= 3)
						continue;

					listFlagItemIndex.push_back(pItemFlag->getVIndex());
				}

				if (listFlagItemIndex.empty())
				{
					CNetMsg::SP rmsg(new CNetMsg);
					EventWorldcupGiftMsg(rmsg, nRank, MSG_EVENT_WORLDCUP_ERROR_GIFT_NOITEM);
					SEND_Q(rmsg, ch->m_desc);
					return ;
				}

				// 2.2.2. 보상품 및 수량 결정
				nGiftItemIndex = tableGift[nRank - 1][0];
				nGiftItemCount = tableGift[nRank - 1][1] * listFlagItemIndex.size();
			}

			// 3. 보상품 생성
			pItemGift = gserver->m_itemProtoList.CreateItem(nGiftItemIndex, -1, 0, 0, nGiftItemCount);
			if (!pItemGift)
			{
				CNetMsg::SP rmsg(new CNetMsg);
				EventWorldcupGiftMsg(rmsg, nRank, MSG_EVENT_WORLDCUP_ERROR_GIFT_NOITEM);
				SEND_Q(rmsg, ch->m_desc);
				return ;
			}

			// 4. 보상품 지급
			if (ch->m_inventory.addItem(pItemGift) == false)
			{
				delete pItemGift;

				CNetMsg::SP rmsg(new CNetMsg);
				EventWorldcupGiftMsg(rmsg, nRank, MSG_EVENT_WORLDCUP_ERROR_GIFT_FULLINVEN);
				SEND_Q(rmsg, ch->m_desc);
				return ;
			}

			// 5. 게임 로그 1
			GAMELOG << init("EVENT WORLDCUP 2006 GIFT", ch)
					<< "RANK" << delim
					<< nRank << delim
					<< "FLAG" << delim;

			// 6. 국기 회수
			{
				std::vector<int>::iterator it = listFlagItemIndex.begin();
				std::vector<int>::iterator endit = listFlagItemIndex.end();
				for(; it != endit; ++it)
				{
					nRowFlagItem = *(it);
					pItemFlag = ch->m_inventory.FindByVirtualIndex(nRowFlagItem);
					if (pItemFlag)
					{
						GAMELOG << itemlog(pItemFlag) << delim;

						ch->m_inventory.deleteItemByItem(pItemFlag);
					}
				}
				listFlagItemIndex.clear();
			}

			{
				// 7. 결과 알림
				CNetMsg::SP rmsg(new CNetMsg);
				EventWorldcupGiftMsg(rmsg, nRank, MSG_EVENT_WORLDCUP_ERROR_GIFT_OK);
				SEND_Q(rmsg, ch->m_desc);
			}

			// 8. 게임 로그 2
			GAMELOG << "GIFT" << delim
					<< nGiftItemIndex << delim
					<< "COUNT" << delim
					<< nGiftItemCount << delim
					<< "TOTAL" << delim
					<< itemlog(pItemGift)
					<< end;
		}
		break;

	default:
		return ;
	}

#undef TABLE_REF_COUNTRY
#undef TABLE_REF_FIFACUP
#undef TABLE_REF_FLAG
#undef TABLE_REF_RANK
}

void do_EventGoldenball(CPC* ch, CNetMsg::SP& msg)
{
	// 필요 공 수
	static const int nNeedBallCount = 10;

	// 상품 테이블 : index, flag, count
	static const int tableGift[3][4] =
	{
		{ 85,   0,  5},			// 승리국/점수 적중			고제 5개
		{ 85,   0,  1},			// 승리국 적중				고제 1개
		{723,   0,  5},			// 꽝						문스톤 5개
	};

	int				nSubType;
	int				nTeam1Score;
	int				nTeam2Score;

	int				nRow;
	int				nCol;
	CItem*			pItemBall;
	CItem*			pItemCard;
	CItem*			pItemGift;
	int				nCardScore;
	int				nGiftType;		// 0 - 모두 적중, 1 - 승리국 적중, 2 - 꽝

	RefMsg(msg) >> nSubType;

	switch (nSubType)
	{
	case MSG_EVENT_GOLDENBALL_VOTE:
		{
			RefMsg(msg) >> nTeam1Score
						>> nTeam2Score;

			// 1. 스코어 검사
			// 2. 응모 기간 검사
			// 3. 응모 여부 검사
			// 4. 공 아이템 검사
			// 5. 카드 지급
			// 6. 결과 알림
			// 7. 게임로그

			// 1. 스코어 검사
			if (nTeam1Score < 0 || nTeam1Score > 10000 || nTeam2Score < 0 || nTeam2Score > 10000)
				return ;

			// 2. 응모 기간 검사
			if (gserver->m_clGoldenBall.GetStatus() != GOLDENBALL_STATUS_VOTE)
			{
				CNetMsg::SP rmsg(new CNetMsg);
				EventGoldenballVoteMsg(rmsg, nTeam1Score, nTeam2Score, MSG_EVENT_GOLDENBALL_ERROR_VOTE_TIME);
				SEND_Q(rmsg, ch->m_desc);
				return ;
			}

			// 3. 응모 여부 검사
			if (ch->m_inventory.FindByDBIndex(GOLDENBALL_CARD_INDEX))
			{
				CNetMsg::SP rmsg(new CNetMsg);
				EventGoldenballVoteMsg(rmsg, nTeam1Score, nTeam2Score, MSG_EVENT_GOLDENBALL_ERROR_VOTE_ALREADY);
				SEND_Q(rmsg, ch->m_desc);
				return ;
			}

			// 4. 공 아이템 검사
			item_search_t vec;
			int sc = ch->m_inventory.searchItemByCondition(GOLDENBALL_BALL_INDEX, 0, 0, vec);
			if (sc == 0 || sc < pItemBall->Count() < nNeedBallCount)
			{
				CNetMsg::SP rmsg(new CNetMsg);
				EventGoldenballVoteMsg(rmsg, nTeam1Score, nTeam2Score, MSG_EVENT_GOLDENBALL_ERROR_VOTE_NOITEM);
				SEND_Q(rmsg, ch->m_desc);
				return ;
			}

			// 5. 카드 지급 및 공 회수 : plus에 점수 설정
			nCardScore = ((nTeam1Score << 16) & 0xffff0000) | (nTeam2Score & 0x0000ffff);
			pItemCard = gserver->m_itemProtoList.CreateItem(GOLDENBALL_CARD_INDEX, -1, nCardScore, 0, 1);
			if (!pItemCard)
			{
				CNetMsg::SP rmsg(new CNetMsg);
				EventGoldenballVoteMsg(rmsg, nTeam1Score, nTeam2Score, MSG_EVENT_GOLDENBALL_ERROR_VOTE_NOITEM);
				SEND_Q(rmsg, ch->m_desc);
				return ;
			}

			if (ch->m_inventory.addItem(pItemCard) == false)
			{
				delete pItemCard;

				CNetMsg::SP rmsg(new CNetMsg);
				EventGoldenballVoteMsg(rmsg, nTeam1Score, nTeam2Score, MSG_EVENT_GOLDENBALL_ERROR_VOTE_NOITEM);
				SEND_Q(rmsg, ch->m_desc);
				return ;
			}

			ch->m_inventory.deleteItem(vec, sc);

			{
				// 6. 결과 알림
				CNetMsg::SP rmsg(new CNetMsg);
				EventGoldenballVoteMsg(rmsg, nTeam1Score, nTeam2Score, MSG_EVENT_GOLDENBALL_ERROR_VOTE_OK);
				SEND_Q(rmsg, ch->m_desc);
			}

			// 7. 게임로그
			GAMELOG << init("EVENT GOLDEN BALL VOTE", ch)
					<< "SCORE" << delim
					<< gserver->m_clGoldenBall.GetTeam1Name() << delim
					<< nTeam1Score << delim
					<< gserver->m_clGoldenBall.GetTeam2Name() << delim
					<< nTeam2Score << delim
					<< "CARD" << delim
					<< itemlog(pItemCard, true) << delim
					<< "BALL REMAIN" << delim;
			if (pItemBall)
				GAMELOG << pItemBall->Count();
			else
				GAMELOG << 0;
			GAMELOG << end;
		}

		break;

	case MSG_EVENT_GOLDENBALL_GIFT:
		{
			// 1. 보상 기간 검사
			// 2. 카드 찾기
			// 3. 점수 뽑기
			// 4. 적중 판단
			// 4.1. 점수가 정확히 맞으면 0
			// 4.2. 무승부가 아니면 승리국만 적중하는 경우 검사
			// 5. 아이템 생성
			// 6. 아이템 지급
			// 7. 게임로그
			// 8. 카드 회수
			// 9. 결과 알림

			// 1. 보상 기간 검사
			if (gserver->m_clGoldenBall.GetStatus() != GOLDENBALL_STATUS_GIFT)
			{
				CNetMsg::SP rmsg(new CNetMsg);
				EventGoldenballGiftMsg(rmsg, MSG_EVENT_GOLDENBALL_ERROR_GIFT_TIME);
				SEND_Q(rmsg, ch->m_desc);
				return ;
			}

			// 2. 카드 찾기
			pItemCard = ch->m_inventory.FindByDBIndex(GOLDENBALL_CARD_INDEX);
			if (pItemCard == NULL)
			{
				CNetMsg::SP rmsg(new CNetMsg);
				EventGoldenballGiftMsg(rmsg, MSG_EVENT_GOLDENBALL_ERROR_GIFT_NOITEM);
				SEND_Q(rmsg, ch->m_desc);
				return ;
			}

			// 3. 점수 뽑기
			nCardScore = pItemCard->getPlus();
			nTeam1Score = (nCardScore >> 16) & 0x0000ffff;
			nTeam2Score =  nCardScore        & 0x0000ffff;

			// 4. 적중 판단
			nGiftType = 2;
			// 4.1. 점수가 정확히 맞으면 0
			if (nTeam1Score == gserver->m_clGoldenBall.GetTeam1Score() && nTeam2Score == gserver->m_clGoldenBall.GetTeam2Score())
				nGiftType = 0;
			// 4.2. 무승부가 아니면 승리국만 적중하는 경우 검사
			else if ( (nTeam1Score != nTeam2Score) && (gserver->m_clGoldenBall.GetTeam1Score() != gserver->m_clGoldenBall.GetTeam2Score()) )
			{
				if ((nTeam1Score > nTeam2Score) == (gserver->m_clGoldenBall.GetTeam1Score() > gserver->m_clGoldenBall.GetTeam2Score()))
					nGiftType = 1;
			}

			// 5. 아이템 생성
			pItemGift = gserver->m_itemProtoList.CreateItem(tableGift[nGiftType][0], -1, 0, tableGift[nGiftType][1], tableGift[nGiftType][2]);
			if (!pItemGift)
			{
				CNetMsg::SP rmsg(new CNetMsg);
				EventGoldenballGiftMsg(rmsg, MSG_EVENT_GOLDENBALL_ERROR_GIFT_NOITEM);
				SEND_Q(rmsg, ch->m_desc);
				return ;
			}

			if (ch->m_inventory.addItem(pItemGift) == false)
			{
				delete pItemGift;

				CNetMsg::SP rmsg(new CNetMsg);
				EventGoldenballGiftMsg(rmsg, MSG_EVENT_GOLDENBALL_ERROR_GIFT_FULLINVEN);
				SEND_Q(rmsg, ch->m_desc);
				return ;
			}

			// 7. 게임로그
			GAMELOG << init("EVENT GOLDEN BALL GIFT", ch)
					<< "GIFT TYPE" << delim
					<< nGiftType << delim
					<< "GIFT" << delim
					<< tableGift[nGiftType][0] << delim
					<< pItemGift->m_itemProto->getItemName() << delim
					<< pItemGift->getPlus() << delim
					<< tableGift[nGiftType][1] << delim
					<< tableGift[nGiftType][2] << delim
					<< "CARD" << delim
					<< itemlog(pItemCard)
					<< end;

			// 8. 카드 회수
			ch->m_inventory.decreaseItemCount(pItemCard, 1);

			{
				// 9. 결과 알림
				CNetMsg::SP rmsg(new CNetMsg);
				switch (nGiftType)
				{
				case 0:
					EventGoldenballGiftMsg(rmsg, MSG_EVENT_GOLDENBALL_ERROR_GIFT_OK_EXACT);
					break;
				case 1:
					EventGoldenballGiftMsg(rmsg, MSG_EVENT_GOLDENBALL_ERROR_GIFT_OK_WINNER);
					break;
				case 2:
					EventGoldenballGiftMsg(rmsg, MSG_EVENT_GOLDENBALL_ERROR_GIFT_OK_NONE);
					break;
				}
				SEND_Q(rmsg, ch->m_desc);
			}
		}
		break;

	default:
		return ;
	}
}

void do_EventRain2006(CPC* ch, CNetMsg::SP& msg)
{
	if( !gserver->isActiveEvent( A_EVENT_RAIN_DROP ) )
		return ;
	// 아이템 인덱스
#ifdef LC_TLD
	// 태국 어머니날 이벤트로 변경
	static const int nRaindrop = 1705;
	static const int nRainBottle = 1704;
#else
	static const int nRaindrop = 1518;
	static const int nRainBottle = 1517;
#endif

	// 교환 필요 방울 수
	//static const int nNeedDrops = 10;

	// 물병당 방울 수 : nNeedDrops % nDropsPerBottle == 0 이어야 한다
	//static const int nDropsPerBottle = 5;

	// 보상 테이블 : 확률[1/10000], index1, flag1, count1, index2, flag2, count2

	int				nRow;
	int				nCol;
	CItem*			pItemRaindrop = NULL;
	CItem*			pItemRainBottle = NULL;
	int				nRainCount = 0;		// 빗방울 1, 빗물병 5 씩 증가
	int				i = 0;
	bool			bGiveItem = false;

	// 병부터 교환.
	{
		item_search_t vec;
		int sc = ch->m_inventory.searchItemByCondition(nRainBottle, 0, 0, vec);
		if (sc > 0)
		{
			nRainCount = sc / 2 ; // 병은 물방을 5개 즉 2개당 한번 지급

			if( nRainCount > 0 )
			{
				for (int i = 0; i < nRainCount; i++ )
				{
					if( gserver->m_rewardMgr.setRewardData( ch, 28 ) && gserver->m_rewardMgr.doReward( ch, 28 ) )
					{
						bGiveItem = true;
					}
					else
					{
						CNetMsg::SP rmsg(new CNetMsg);
						EventRain2006Msg(rmsg, MSG_EVENT_RAIN_2006_ERROR_FULLINVEN);
						SEND_Q(rmsg, ch->m_desc);
						//GAMELOG << init("RAINBOTTLE REWARD FAIL") << pItemRainBottle->Count() << delim;
						return;
					}
				}

				ch->m_inventory.deleteItem(vec, sc * 2);
			}
		}
	}

	// 물방울 교환
	{
		item_search_t vec;
		int sc = ch->m_inventory.searchItemByCondition(nRaindrop, 0, 0, vec);
		if (sc > 0)
		{
			nRainCount = sc / 10;

			if( nRainCount > 0 )
			{
				for (int i = 0; i < nRainCount; i++ )
				{
					if( gserver->m_rewardMgr.setRewardData( ch, 28 ) && gserver->m_rewardMgr.doReward( ch, 28 ) )
					{
						bGiveItem = true;
					}
					else
					{
						CNetMsg::SP rmsg(new CNetMsg);
						EventRain2006Msg(rmsg, MSG_EVENT_RAIN_2006_ERROR_FULLINVEN);
						SEND_Q(rmsg, ch->m_desc);
						//GAMELOG << init("RAINDROP REWARD FAIL") << pItemRaindrop->Count() << delim;
						return;
					}
				}

				ch->m_inventory.deleteItem(vec, sc * 10);
			}
		}
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		if(bGiveItem)
			EventRain2006Msg(rmsg, MSG_EVENT_RAIN_2006_ERROR_OK);
		else
			EventRain2006Msg(rmsg, MSG_EVENT_RAIN_2006_ERROR_NOITEM);

		SEND_Q(rmsg, ch->m_desc);
	}
}

void TO_BUGDATA(CItem* pItem, unsigned char ucBug[16])
{
	ucBug[ 0] = (unsigned char)((pItem->getPlus() >> 28) & 0x0f);
	ucBug[ 1] = (unsigned char)((pItem->getPlus() >> 24) & 0x0f);
	ucBug[ 2] = (unsigned char)((pItem->getPlus() >> 20) & 0x0f);
	ucBug[ 3] = (unsigned char)((pItem->getPlus() >> 16) & 0x0f);
	ucBug[ 4] = (unsigned char)((pItem->getPlus() >> 12) & 0x0f);
	ucBug[ 5] = (unsigned char)((pItem->getPlus() >>  8) & 0x0f);
	ucBug[ 6] = (unsigned char)((pItem->getPlus() >>  4) & 0x0f);
	ucBug[ 7] = (unsigned char)((pItem->getPlus()      ) & 0x0f);
	ucBug[ 8] = (unsigned char)((pItem->getUsed() >> 28) & 0x0f);
	ucBug[ 9] = (unsigned char)((pItem->getUsed() >> 24) & 0x0f);
	ucBug[10] = (unsigned char)((pItem->getUsed() >> 20) & 0x0f);
	ucBug[11] = (unsigned char)((pItem->getUsed() >> 16) & 0x0f);
	ucBug[12] = (unsigned char)((pItem->getUsed() >> 12) & 0x0f);
	ucBug[13] = (unsigned char)((pItem->getUsed() >>  8) & 0x0f);
	ucBug[14] = (unsigned char)((pItem->getUsed() >>  4) & 0x0f);
	ucBug[15] = (unsigned char)((pItem->getUsed()      ) & 0x0f);

	if( ucBug[0] == 11 )
		ucBug[0] = 8;

	if( ucBug[8] == 11 )
		ucBug[8] = 8;
}

void FROM_BUGDATA(unsigned char ucBug[16], CItem* pItem)
{
	if( ucBug[8] == 8 )
	{
		ucBug[8] = 11;
	}

	if( ucBug[0] == 8 )
	{
		ucBug[0] = 11;
	}

	pItem->setPlus( (((unsigned int)(ucBug[ 0]) << 28) & 0xf0000000)
					| (((unsigned int)(ucBug[ 1]) << 24) & 0x0f000000)
					| (((unsigned int)(ucBug[ 2]) << 20) & 0x00f00000)
					| (((unsigned int)(ucBug[ 3]) << 16) & 0x000f0000)
					| (((unsigned int)(ucBug[ 4]) << 12) & 0x0000f000)
					| (((unsigned int)(ucBug[ 5]) <<  8) & 0x00000f00)
					| (((unsigned int)(ucBug[ 6]) <<  4) & 0x000000f0)
					| (((unsigned int)(ucBug[ 7])      ) & 0x0000000f));

	pItem->setUsed( (((unsigned int)(ucBug[ 8]) << 28) & 0xf0000000)
					| (((unsigned int)(ucBug[ 9]) << 24) & 0x0f000000)
					| (((unsigned int)(ucBug[10]) << 20) & 0x00f00000)
					| (((unsigned int)(ucBug[11]) << 16) & 0x000f0000)
					| (((unsigned int)(ucBug[12]) << 12) & 0x0000f000)
					| (((unsigned int)(ucBug[13]) <<  8) & 0x00000f00)
					| (((unsigned int)(ucBug[14]) <<  4) & 0x000000f0)
					| (((unsigned int)(ucBug[15])      ) & 0x0000000f));
}

void do_EventCollectBug(CPC* ch, CNetMsg::SP& msg)
{
	if(!gserver->isActiveEvent( A_EVENT_COLLECT_BUG) )
		return;

	static const int	nNormalBoxIndex = 1577;		// 일반 상자 인덱스
	static const int	nGoldenBoxIndex = 1578;		// 골든 상자 인덱스
	static const int	nGoldenPoint = 30;			// 골든 상자 보너스 포인트

	static const int	nNeedMoney = 100;			// 상자 구매 금액
	static const int	nProbGoldenBox = 1;			// 골든 상자 확률

	int				nSubType = 0;
	CItem*			pItemBox = NULL;
	int				nRow, nCol;
	int				nSelectedBoxIndex = 0;
	int				nSelectedBoxPoint = 0;
	bool			bIsFull = true;
	int				nGift = 0;
	//int				nGiftIndex = 0;
	//int				nGiftCount = 0;
	//CItem*			pItemGift = NULL;
	unsigned char	ucBug[16];
	int				i;

	int				nProb = 0;

	RefMsg(msg) >> nSubType;

	switch (nSubType)
	{
	case MSG_EVENT_COLLECT_BUG_DROP:
		{
			RefMsg(msg) >> nSelectedBoxIndex;
			if (nSelectedBoxIndex < 0 || nSelectedBoxIndex >= 16)
				return ;

			// 1. 일반 상자 검색
			// 2. 골든 상자 검색
			// 3. 게임 로그 1
			// 4. 상자 내용 변경
			// 5. 게임 로그 2
			// 6. 결과 알림

			// 1. 일반 상자 검색
			pItemBox = ch->m_inventory.FindByDBIndex(nNormalBoxIndex);
			if (pItemBox)
			{
				TO_BUGDATA(pItemBox, ucBug);
			}

			// 2. 골든 상자 검색
			pItemBox = ch->m_inventory.FindByDBIndex(nGoldenBoxIndex);
			if (pItemBox)
			{
				TO_BUGDATA(pItemBox, ucBug);
			}

			if (pItemBox)
			{
				// 3. 게임 로그 1
				GAMELOG << init("EVENT COLLECT BUG DEL", ch)
						<< "POSITION" << delim
						<< nSelectedBoxIndex << delim
						<< "PREV" << delim
						<< itemlog(pItemBox) << delim
						<< "NEXT" << delim;

				// 4. 상자 내용 변경
				pItemBox->setFlag(pItemBox->getFlag() - ucBug[nSelectedBoxIndex]);
				ucBug[nSelectedBoxIndex] = 0;
				FROM_BUGDATA(ucBug, pItemBox);

				{
					CNetMsg::SP rmsg(new CNetMsg);
					UpdateClient::makeUpdateItemFlag(rmsg, pItemBox->tab(), pItemBox->getInvenIndex(), pItemBox->getFlag());
					SEND_Q(rmsg, ch->m_desc);
				}

				// 5. 게임 로그 2
				GAMELOG << itemlog(pItemBox)
						<< end;

				{
					// 6. 결과 알림
					CNetMsg::SP rmsg(new CNetMsg);
					EventCollectBugMsg(rmsg, MSG_EVENT_COLLECT_BUG_DROP, nSelectedBoxIndex);
					SEND_Q(rmsg, ch->m_desc);
				}
			}
		}
		break;

	case MSG_EVENT_COLLECT_BUG_GIFT_REQ:
		{
			// 1. 일반 상자 검색
			// 2. 골든 상자 검색
			// 3. 포인트에 따른 보상 선택
			// 4. 보상 지급
			// 5. 게임 로그 1
			// 6. 게임 로그 2
			// 7. 상자 회수
			// 8. 결과 알림

			// 1. 소유 상자 검색
			pItemBox = ch->m_inventory.FindByDBIndex(nNormalBoxIndex);
			if (pItemBox)
			{
				nSelectedBoxPoint = 0;
				TO_BUGDATA(pItemBox, ucBug);
				for (i = 0; i < 16; i++)
				{
					if (ucBug[i] == 0)
						break;
					nSelectedBoxPoint += ucBug[i];
				}
				if (i != 16)
				{
					bIsFull = false;
					pItemBox = NULL;
				}
			}

			// 2. 골든 상자 검색 (일반 상자에서 찾지 못했다면...)
			if (pItemBox == NULL)
			{
				pItemBox = ch->m_inventory.FindByDBIndex(nGoldenBoxIndex);
				if (pItemBox)
				{
					bIsFull = true;
					nSelectedBoxPoint = 0;
					TO_BUGDATA(pItemBox, ucBug);
					for (i = 0; i < 16; i++)
					{
						if (ucBug[i] == 0)
							break;
						nSelectedBoxPoint += ucBug[i];
					}
					if (i != 16)
					{
						bIsFull = false;
						pItemBox = NULL;
					}

					// 골든 상자 추가 포인트
					nSelectedBoxPoint += nGoldenPoint;
				}
			}

			if (pItemBox == NULL)
			{
				CNetMsg::SP rmsg(new CNetMsg);
				if (bIsFull)
					EventCollectBugMsg(rmsg, MSG_EVENT_COLLECT_BUG_GIFT_REP_ERROR_NOITEM, 0);
				else
					EventCollectBugMsg(rmsg, MSG_EVENT_COLLECT_BUG_GIFT_REP_ERROR_NOTFULL, 0);
				SEND_Q(rmsg, ch->m_desc);
				return ;
			}

			// 3. 포인트에 따른 보상 선택
			if      (nSelectedBoxPoint <  26)			nGift = 0;
			else if (nSelectedBoxPoint <  41)			nGift = 1;
			else if (nSelectedBoxPoint <  56)			nGift = 2;
			else if (nSelectedBoxPoint <  71)			nGift = 3;
			else if (nSelectedBoxPoint <  86)			nGift = 4;
			else if (nSelectedBoxPoint < 101)			nGift = 5;
			else if (nSelectedBoxPoint < 116)			nGift = 6;
			else if (nSelectedBoxPoint < 121)			nGift = 7;
			else if (nSelectedBoxPoint < 126)			nGift = 8;
			else if (nSelectedBoxPoint < 131)			nGift = 9;
			else if (nSelectedBoxPoint < 160)			nGift = 10;
			else                             			nGift = 11;

			{
				// int rewardIndex=7;
				if( !gserver->m_rewardMgr.setRewardData(ch, 7, -1, nGift)
						|| !gserver->m_rewardMgr.doReward(ch, 7 ) )
				{
					CNetMsg::SP rmsg(new CNetMsg);
					EventCollectBugMsg(rmsg, MSG_EVENT_COLLECT_BUG_GIFT_REP_ERROR_FULLINVEN, 0);
					SEND_Q(rmsg, ch->m_desc);
					return ;
				}
			}

			// 7. 상자 회수
			ch->m_inventory.decreaseItemCount(pItemBox, 1);

			{
				// 8. 결과 알림
				CNetMsg::SP rmsg(new CNetMsg);
				EventCollectBugMsg(rmsg, MSG_EVENT_COLLECT_BUG_GIFT_REP_OK, nGift);
				SEND_Q(rmsg, ch->m_desc);
			}
		}
		break;

	case MSG_EVENT_COLLECT_BUG_BUY_BOX_REQ:
		{
			// 1. 소지 금액 검사
			// 2. 소유 아이템 검사
			// 3. 확률 적용 : 1% 골든 상자
			// 4. 상자 지급
			// 5. 돈 회수
			// 6. 결과 알림
			// 7. 게임 로그

			// 1. 소지 금액 검사
			if (ch->m_inventory.getMoney() < nNeedMoney)
			{
				CNetMsg::SP rmsg(new CNetMsg);
				EventCollectBugMsg(rmsg, MSG_EVENT_COLLECT_BUG_BUY_BOX_REP_ERROR_MONEY, 0);
				SEND_Q(rmsg, ch->m_desc);
				return ;
			}

			// 2. 소유 아이템 검사
			if (ch->m_inventory.FindByDBIndex(nNormalBoxIndex) || ch->m_inventory.FindByDBIndex(nGoldenBoxIndex))
			{
				CNetMsg::SP rmsg(new CNetMsg);
				EventCollectBugMsg(rmsg, MSG_EVENT_COLLECT_BUG_BUY_BOX_REP_ERROR_DUPLICATE, 0);
				SEND_Q(rmsg, ch->m_desc);
				return ;
			}

			// 3. 확률 적용 : 1% 골든 상자
			nProb = GetRandom(1, 100);
			if (nProb == nProbGoldenBox)
			{
				nSelectedBoxIndex = nGoldenBoxIndex;
				nSelectedBoxPoint = nGoldenPoint;
			}
			else
			{
				nSelectedBoxIndex = nNormalBoxIndex;
				nSelectedBoxPoint = 0;
			}

			// 4. 상자 지급
			pItemBox = gserver->m_itemProtoList.CreateItem(nSelectedBoxIndex, -1, 0, nSelectedBoxPoint, 1);
			if (pItemBox == NULL)
			{
				CNetMsg::SP rmsg(new CNetMsg);
				EventCollectBugMsg(rmsg, MSG_EVENT_COLLECT_BUG_BUY_BOX_REP_ERROR_FULLINVEN, 0);
				SEND_Q(rmsg, ch->m_desc);
				return ;
			}

			pItemBox->setUsed(0);
			if (ch->m_inventory.addItem(pItemBox))
			{
				// 5. 돈 회수
				ch->m_inventory.decreaseMoney(nNeedMoney);

				{
					// 6. 결과 알림
					CNetMsg::SP rmsg(new CNetMsg);
					EventCollectBugMsg(rmsg, MSG_EVENT_COLLECT_BUG_BUY_BOX_REP_OK, (nSelectedBoxIndex == nGoldenBoxIndex) ? 1 : 0);
					SEND_Q(rmsg, ch->m_desc);
				}

				// 7. 게임 로그
				GAMELOG << init("EVENT COLLECT BUG BUY", ch)
						<< "BOX" << delim
						<< itemlog(pItemBox)
						<< end;
			}
			else
			{
				delete pItemBox;

				CNetMsg::SP rmsg(new CNetMsg);
				EventCollectBugMsg(rmsg, MSG_EVENT_COLLECT_BUG_BUY_BOX_REP_ERROR_FULLINVEN, 0);
				SEND_Q(rmsg, ch->m_desc);
				return ;
			}
		}
		break;
	}
}

void do_EventChuseok2006(CPC* ch, CNetMsg::SP& msg)
{
	if( !gserver->isActiveEvent( A_EVENT_CHUSUK ) )
		return;

	static const int		nIndexRice		= 1814;	// 쌀가루
	static const int		nIndexHoney		= 1815;	// 꿀
	static const int		nIndexPine		= 1816;	// 솔잎
	static const int		nIndexCake		= 1817;	// 송편
	static const int		nIndexRainbow	= 1818;	// 오색 송편

	int				nSubtype;
	int				nRow, nCol;
	CItem*			pItemNeed1		= NULL;
	CItem*			pItemNeed2		= NULL;
	CItem*			pItemNeed3		= NULL;
	//CItem*			pItemMade		= NULL;
	//bool			bSuccess		= false;

	RefMsg(msg) >> nSubtype;

	switch (nSubtype)
	{
	case MSG_EVENT_CHUSEOK_2006_MAKE_RICECAKE:			// 송편 제작
		{
			// 재료 1 : 쌀가루
			pItemNeed1 = ch->m_inventory.FindByDBIndex(nIndexRice, 0, 0);
			if (pItemNeed1 == NULL)
			{
				CNetMsg::SP rmsg(new CNetMsg);
				EventChuseok2006Msg(rmsg, MSG_EVENT_CHUSEOK_2006_MAKE_RICECAKE_NOITEM);
				SEND_Q(rmsg, ch->m_desc);
				return ;
			}

			// 재료 2 : 꿀
			pItemNeed2 = ch->m_inventory.FindByDBIndex(nIndexHoney, 0, 0);
			if (pItemNeed2 == NULL)
			{
				CNetMsg::SP rmsg(new CNetMsg);
				EventChuseok2006Msg(rmsg, MSG_EVENT_CHUSEOK_2006_MAKE_RICECAKE_NOITEM);
				SEND_Q(rmsg, ch->m_desc);
				return ;
			}

			// 재료 3 : 솔잎
			pItemNeed3 = ch->m_inventory.FindByDBIndex(nIndexPine, 0, 0);
			if (pItemNeed3 == NULL)
			{
				CNetMsg::SP rmsg(new CNetMsg);
				EventChuseok2006Msg(rmsg, MSG_EVENT_CHUSEOK_2006_MAKE_RICECAKE_NOITEM);
				SEND_Q(rmsg, ch->m_desc);
				return ;
			}

			GAMELOG << init("EVENT CHUSEOK 2006 MAKE CAKE", ch) ;

			if( !gserver->m_rewardMgr.setRewardData( ch, 30 ) || !gserver->m_rewardMgr.doReward(ch, 30) )
			{
				CNetMsg::SP rmsg(new CNetMsg);
				EventChuseok2006Msg(rmsg, MSG_EVENT_CHUSEOK_2006_MAKE_RICECAKE_FAIL);
				SEND_Q(rmsg, ch->m_desc);
				GAMELOG << "FAIL" << end;

				ch->m_inventory.decreaseItemCount(pItemNeed1, 1);
				ch->m_inventory.decreaseItemCount(pItemNeed2, 1);
				ch->m_inventory.decreaseItemCount(pItemNeed3, 1);

				return ;
			}

			GAMELOG << "SUCC" << end;

			ch->m_inventory.decreaseItemCount(pItemNeed1, 1);
			ch->m_inventory.decreaseItemCount(pItemNeed2, 1);
			ch->m_inventory.decreaseItemCount(pItemNeed3, 1);

			{
				CNetMsg::SP rmsg(new CNetMsg);
				EventChuseok2006Msg(rmsg, MSG_EVENT_CHUSEOK_2006_MAKE_RICECAKE_OK);
				SEND_Q(rmsg, ch->m_desc);
			}
		}
		break;

	case MSG_EVENT_CHUSEOK_2006_MAKE_RAINBOW_CAKE:		// 오색 송편 제작
		{
			item_search_t vec;
			int sc = ch->m_inventory.searchItemByCondition(nIndexCake, 0, 0, vec);
			if (sc == 0 || sc < 10)
			{
				CNetMsg::SP rmsg(new CNetMsg);
				EventChuseok2006Msg(rmsg, MSG_EVENT_CHUSEOK_2006_MAKE_RAINBOW_CAKE_NOITEM);
				SEND_Q(rmsg, ch->m_desc);
				return ;
			}

			GAMELOG << init("EVENT CHUSEOK 2006 MAKE RAINBOW CAKE", ch)
					<< "NEED"						 << delim
					<< itemlog(pItemNeed1, true)	 << delim
					<< "CAKE"						 << delim;

			if( !gserver->m_rewardMgr.setRewardData( ch, 31 ) || !gserver->m_rewardMgr.doReward(ch, 31) )
			{
				CNetMsg::SP rmsg(new CNetMsg);
				EventChuseok2006Msg(rmsg, MSG_EVENT_CHUSEOK_2006_MAKE_RAINBOW_CAKE_FAIL);
				SEND_Q(rmsg, ch->m_desc);

				ch->m_inventory.deleteItem(vec, 8);

				GAMELOG << "FAIL" << end;
				return;
			}

			GAMELOG << "SUC" << end;

			ch->m_inventory.deleteItem(vec, 10);

			{
				CNetMsg::SP rmsg(new CNetMsg);
				EventChuseok2006Msg(rmsg, MSG_EVENT_CHUSEOK_2006_MAKE_RAINBOW_CAKE_OK);
				SEND_Q(rmsg, ch->m_desc);
			}
		}
		break;

	case MSG_EVENT_CHUSEOK_2006_GIFT:	// 오색 송편 교환 요청
		{
			// 재료 : 오색 송편
			pItemNeed1 = ch->m_inventory.FindByDBIndex(nIndexRainbow, 0, 0);
			if (pItemNeed1 == NULL)
			{
				CNetMsg::SP rmsg(new CNetMsg);
				EventChuseok2006Msg(rmsg, MSG_EVENT_CHUSEOK_2006_GIFT_OK_NOITEM);
				SEND_Q(rmsg, ch->m_desc);
				return ;
			}

			GAMELOG << init("EVENT CHUSEOK 2006 GIFT", ch);

			if( !gserver->m_rewardMgr.setRewardData( ch, 29 ) || !gserver->m_rewardMgr.doReward(ch, 29) )
			{
				CNetMsg::SP rmsg(new CNetMsg);
				EventChuseok2006Msg(rmsg, MSG_EVENT_CHUSEOK_2006_GIFT_OK_FULLINVEN);
				SEND_Q(rmsg, ch->m_desc);
				GAMELOG << "FAIL" << end;
				return ;
			}

			GAMELOG << "SUC" << end;

			ch->m_inventory.decreaseItemCount(pItemNeed1, 1);

			{
				CNetMsg::SP rmsg(new CNetMsg);
				EventChuseok2006Msg(rmsg, MSG_EVENT_CHUSEOK_2006_GIFT_OK);
				SEND_Q(rmsg, ch->m_desc);
			}
		}
		break;

	default:
		break;
	}
}

void do_EventValentine2007(CPC* ch, CNetMsg::SP& msg)
{
	if( !gserver->isActiveEvent( A_EVENT_VALENTINE_DAY ) )
		return;

	int nSubType;

#ifdef NPC_CHECK
	int npcIndex;
	CNPC *npc;
	RefMsg(msg) >> nSubType
				>> npcIndex;
	npc = ch->m_pArea->FindProtoNPCInCell(ch, npcIndex, false, 2);
	if(!npc)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		EventValentine2007ErrorMsg(rmsg, MSG_EVENT_VALENTINE_2007_BINGO_ITEM_NOTFOUND);
		SEND_Q(rmsg, ch->m_desc);
		GAMELOG << init("EventValentine2007 FAIL: NOT EXIST NPC (Dangerous User)", ch) << end;
		return ;
	}
	if( !npc->IsFlag(NPC_EVENT) && !npc->IsFlag(NPC_QUEST) )
	{
		GAMELOG << init("EventValentine2007 FAIL: NOT EXIST NPC (Dangerous User)", ch) << end;
		return ;
	}
#else
	RefMsg(msg) >> nSubType;
#endif

	switch(nSubType)
	{
	case MSG_EVENT_VALENTINE_2007_BINGO_ITEM_ARRANGE:		// 빙고아이템 저장 요청
		{
			unsigned char itempos;
			int itemindex, bingoitemindex, row, col;
			int flag = 7;

			RefMsg(msg) >> itempos
						>> itemindex
						>> bingoitemindex;

			switch(itempos)
			{
			case 0:
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			case 7:
			case 8:
				break;
			default:
				return;
			}

			int const itemcount = 3;
			static int const itemtable[itemcount][2] =  		// {포장된 아이템, 아이템 비트값}
			{
				{ 2048, 1 },		// 카카오 30
				{ 2049, 2 },		// 카카오 60
				{ 2050, 4 },		// 카카오 90
			};

			CItem* pBingoBox = ch->m_inventory.FindByDBIndex(bingoitemindex);
			if (pBingoBox == NULL)
			{
				// 빙고상자 아이템이 없다.
				CNetMsg::SP rmsg(new CNetMsg);
				EventValentine2007ErrorMsg(rmsg, MSG_EVENT_VALENTINE_2007_BINGO_ITEM_NOTFOUND);
				SEND_Q(rmsg, ch->m_desc);
				return;
			}

			CItem* pPackagedItem = ch->m_inventory.FindByDBIndex(itemindex);
			if (pPackagedItem == NULL)
			{
				// 빙고상자에 저장할 아이템이 없다.
				LOG_ERROR("not found item. charIndex[%d] itemDbIndex[%d]", ch->m_index, itemindex);
				return;
			}

			int i = 0;
			for (i = 0 ; i < itemcount; ++i)
			{
				if(itemindex == itemtable[i][0])
				{
					int movebit = 3 * itempos;
					int nArrangeItem = itemtable[i][1] << movebit;

					if(( flag << movebit ) & pBingoBox->getPlus())
					{
						// 이미 배열 되어있는 곳이다.
						CNetMsg::SP rmsg(new CNetMsg);
						EventValentine2007ErrorMsg(rmsg, MSG_EVENT_VALENTINE_2007_BINGO_ITEM_ARRANGE_ALREADY);
						SEND_Q(rmsg, ch->m_desc);
						return;
					}

					pBingoBox->setPlus(pBingoBox->getPlus() | nArrangeItem);

					int score = 0;
					int bingocount = 0;
					int addscore = 0;

					if(!BINGO(pBingoBox->getPlus(), &score, &bingocount))	// 빙고 실패하면 원래대로 되돌린다.
					{
						pBingoBox->setPlus(pBingoBox->getPlus() & ~nArrangeItem);
						return ;
					}

					GAMELOG << init("EVENT VALENTINE 2007 ARRANGE", ch)
							<< "POSITION" << delim
							<< itempos << delim
							<< "ITEM" << delim
							<< itemlog(pPackagedItem) << delim
							<< end;

					addscore = score - pBingoBox->getUsed();
					pBingoBox->setUsed(score);

					ch->m_inventory.sendOneItemInfo(pBingoBox);

					// 저장된 아이템을 인벤에서 지운다.
					ch->m_inventory.decreaseItemCount(pPackagedItem, 1);

					{
						CNetMsg::SP rmsg(new CNetMsg);
						EventValentine2007Msg(rmsg, MSG_EVENT_VALENTINE_2007_BINGO_ITEM_ARRANGE_OK, addscore);
						SEND_Q(rmsg, ch->m_desc);
					}
					break;
				}
			}

			if(i == itemcount)
			{
				// 포장된 초코렛이 아닌 것은 배열할 수 없다.
				CNetMsg::SP rmsg(new CNetMsg);
				EventValentine2007ErrorMsg(rmsg, MSG_EVENT_VALENTINE_2007_BINGO_ITEM_ARRANGE_NOTPACKAGE);
				SEND_Q(rmsg, ch->m_desc);
				return;
			}
		}
		break;
	case MSG_EVENT_VALENTINE_2007_BINGO_GIFT:				// 보상요청
		{
			int bingoitemindex, row, col;

			RefMsg(msg) >> bingoitemindex;

			CItem* pBingoBox = ch->m_inventory.FindByDBIndex(bingoitemindex);
			if (pBingoBox)
			{
				int score = 0;
				int bingocount = 0;

				if(!BINGO(pBingoBox->getPlus(), &score, &bingocount))
					return;

				// 빙고가 없으면
				if(score == 0)
				{
					// 상품을 줄 수 없다고 한다.
					CNetMsg::SP rmsg(new CNetMsg);
					EventValentine2007ErrorMsg(rmsg, MSG_EVENT_VALENTINE_2007_BINGO_GIFT_NOITEM);
					SEND_Q(rmsg, ch->m_desc);
					return ;
				}

				int rewardIndex;
				if(score <= 23)			rewardIndex = 2; // valentine bingo score 1~23
				else if(score <= 39)	rewardIndex = 3; // valentine bingo score 24~39
				else if(score <= 55)	rewardIndex = 4; // valentine bingo score 40~55
				else if(score <= 71)	rewardIndex = 5; // valentine bingo score 56~71
				else					rewardIndex = 6; // valentine bingo score 72~

				int nProb =GetRandom(1, 10000);

				GAMELOG << init("EVENT VALENTINE REWARD START", ch) 	<< end;

				if( !gserver->m_rewardMgr.setRewardData(ch, rewardIndex, nProb)
						|| !gserver->m_rewardMgr.doReward( ch, rewardIndex) )
				{
					CNetMsg::SP rmsg(new CNetMsg);
					EventValentine2007ErrorMsg(rmsg, MSG_EVENT_VALENTINE_2007_BINGO_GIFT_NOITEM);
					SEND_Q(rmsg, ch->m_desc);
					return ;
				}

				GAMELOG << init("EVENT VALENTINE REWARD END", ch) 	<< end;

				// 빙고박스 아이템을 제거한다.
				ch->m_inventory.decreaseItemCount(pBingoBox, 1);

				{
					// 빙고개수도 같이 전해준다.
					CNetMsg::SP rmsg(new CNetMsg);
					EventValentine2007Msg(rmsg, MSG_EVENT_VALENTINE_2007_BINGO_GIFT_OK, bingocount);
					SEND_Q(rmsg, ch->m_desc);
				}
			}
			else
			{
				// 빙고판 아이템 없음
				CNetMsg::SP rmsg(new CNetMsg);
				EventValentine2007ErrorMsg(rmsg, MSG_EVENT_VALENTINE_2007_BINGO_ITEM_NOTFOUND);
				SEND_Q(rmsg, ch->m_desc);
			}
		}
		break;
	}
}

bool BINGO(int plus, int* score, int* bingocount)
{
	if (!score || !bingocount)
		return false;

	*score = 0;
	*bingocount = 0;

	int flag = 7;
	static const int scoretable[3] = {1, 2, 10};
	int BingoState[9];
	int i;
	for(i = 0; i < 9; ++i)
	{
		BingoState[i] = ( plus >> (i * 3) ) & flag;

		switch(BingoState[i])
		{
		case 0:
		case 1:
		case 2:
		case 4:
			break;
		default:
			return false;
		}
	}

	if(BingoState[0] > 0 && BingoState[0] == BingoState[1] && BingoState[1] == BingoState[2])
	{
		*score += scoretable[BingoState[0]/2];
		(*bingocount)++;
	}
	if(BingoState[3] > 0 && BingoState[3] == BingoState[4] && BingoState[4] == BingoState[5])
	{
		*score += scoretable[BingoState[3]/2];
		(*bingocount)++;
	}
	if(BingoState[6] > 0 && BingoState[6] == BingoState[7] && BingoState[7] == BingoState[8])
	{
		*score += scoretable[BingoState[6]/2];
		(*bingocount)++;
	}
	if(BingoState[0] > 0 && BingoState[0] == BingoState[3] && BingoState[3] == BingoState[6])
	{
		*score += scoretable[BingoState[0]/2];
		(*bingocount)++;
	}
	if(BingoState[1] > 0 && BingoState[1] == BingoState[4] && BingoState[4] == BingoState[7])
	{
		*score += scoretable[BingoState[1]/2];
		(*bingocount)++;
	}
	if(BingoState[2] > 0 && BingoState[2] == BingoState[5] && BingoState[5] == BingoState[8])
	{
		*score += scoretable[BingoState[2]/2];
		(*bingocount)++;
	}
	if(BingoState[0] > 0 && BingoState[0] == BingoState[4] && BingoState[4] == BingoState[8])
	{
		*score += scoretable[BingoState[0]/2];
		(*bingocount)++;
	}
	if(BingoState[2] > 0 && BingoState[2] == BingoState[4] && BingoState[4] == BingoState[6])
	{
		*score += scoretable[BingoState[2]/2];
		(*bingocount)++;
	}

	return true;
}

void do_EventWhiteday2007(CPC* ch, CNetMsg::SP& msg)
{
	if( !gserver->isActiveEvent(A_EVENT_WHITE_DAY) )
		return;

	int nSubType;

	RefMsg(msg) >> nSubType;
	switch(nSubType)
	{
	case MSG_EVENT_WHITEDAY_2007_EXCHANGE_REQ : //
		{
			int nNeedItemCountSky =0;			// 교환에 필요한 하늘색 캔디 개수
			int nNeedItemCountPink=0;			// 교환에 필요한 분홍색 캔디 개수
			//int nRepaySkillIndex = 0;
			int rewardIndex = 0;
			int rewardSelectPos = -1;
			unsigned char exchangeType;
			RefMsg(msg) >> exchangeType;
			//////////////////////////////////////////////////////////////////////////
			switch( exchangeType )
			{
			case 0 :	// 하늘 색 5개  분홍색 5개 (보상 : 스킬 )
				nNeedItemCountSky	= 5;
				nNeedItemCountPink	= 5;
				rewardIndex = 12;
				break;
			case 1:		// 하늘색 20개 (보상 : 하늘색 편지지 )
				nNeedItemCountSky	= 20;
				nNeedItemCountPink	= 0;
				rewardIndex = 13;
				rewardSelectPos = 0;
				break;
			case 2:		// 분홍색 20개 (보상 : 분홍색 편지지  )
				nNeedItemCountSky	= 0;
				nNeedItemCountPink	= 20;
				rewardIndex = 13;
				rewardSelectPos = 1;
				break;
			}

			try
			{
				item_search_t vec_pItemSkyCandy;
				item_search_t vec_pItemPinkCandy;

				if (nNeedItemCountSky > 0 )		// 하늘색 사탕 갯수 검사
				{
					int sc = ch->m_inventory.searchItemByCondition(2133 , 0, 0, vec_pItemSkyCandy); // 하늘색 사탕 찾기
					if (sc == 0 || sc < nNeedItemCountSky)
						throw MSG_EVENT_WHITEDAY_2007_EXCHANGE_NOITEM;	// 아이템 없음
				}

				if ( nNeedItemCountPink > 0 )	// 분홍색 사탕 개수 검사
				{
					int sc = ch->m_inventory.searchItemByCondition(2134 , 0, 0, vec_pItemPinkCandy); // 분홍색 사탕 찾기
					if (sc == 0 || sc < nNeedItemCountPink)
						throw MSG_EVENT_WHITEDAY_2007_EXCHANGE_NOITEM;
				}

				if( !gserver->m_rewardMgr.setRewardData(ch, rewardIndex, -1, rewardSelectPos )
						|| !gserver->m_rewardMgr.doReward( ch, rewardIndex ) )
					throw MSG_EVENT_WHITEDAY_2007_EXCHANGE_FAILED;

				//////////////////////////////////////////////////////////////////////////
				{
					// 성공 메세지ㄹ
					CNetMsg::SP rmsg(new CNetMsg);
					EventWhiteday2007Msg(rmsg, MSG_EVENT_WHITEDAY_2007_EXCHANGE_OK );

					RefMsg(rmsg) << exchangeType;		// 0-스킬 보상, 1-하늘색 편지지 보상 , 2- 분홍색 편지지 보상
					//				if( exchangeType == 0 )
					//				{	RefMsg(rmsg) << nRepaySkill[nRepaySkillIndex][0];	}	// 스킬 보상일경우 스킬 인덱스
					SEND_Q(rmsg, ch->m_desc);
				}

				//////////////////////////////////////////////////////////////////////////
				// 인벤에서 아이템 제거
				if (nNeedItemCountSky > 0) // 하늘 색 사탕
				{
					ch->m_inventory.deleteItem(vec_pItemSkyCandy, nNeedItemCountSky);
				}

				if (nNeedItemCountPink > 0) // 분홍색 사탕
				{
					ch->m_inventory.deleteItem(vec_pItemPinkCandy, nNeedItemCountPink);
				}
			}
			catch (MSG_EVENT_WHITEDAY_2007_TYPE error)
			{
				CNetMsg::SP rmsg(new CNetMsg);
				EventWhiteday2007Msg(rmsg, error );
				SEND_Q(rmsg, ch->m_desc);
				return;
			}
		}
		break; // MSG_EVENT_WHITEDAY_2007_EXCHANGE_REQ

	case MSG_EVENT_WHITEDAY_2007_LETTER_REQ :	// Spred Message
		{
			int color;
			int nRow, nCol;
			int nItemIndex =0 ;
			CItem* pNeedItem = NULL;

			CLCString	msgStr(256);		//클라이언트 50자
			CLCString	rmsgStr(256);
			RefMsg(msg) >> nItemIndex;
			RefMsg(msg) >> color;
			RefMsg(msg) >> msgStr;

			pNeedItem = ch->m_inventory.FindByDBIndex(nItemIndex , 0, 0); // 하늘색 사탕 찾기
			if( pNeedItem != NULL )
			{
				{
					rmsgStr.Format( "%s> %s", ch->GetName(), (const char*)msgStr );
					CNetMsg::SP rmsg(new CNetMsg);
					EventWhiteday2007Msg(rmsg, MSG_EVENT_WHITEDAY_2007_LETTER_REP );
					RefMsg(rmsg) << color
								 << (const char*)rmsgStr;
					PCManager::instance()->sendToAll( rmsg );
				}

				GAMELOG << init("WHITE DAY EVENT ITEM [LETTER]", ch)
						<< "USE"			 << delim
						<< itemlog(pNeedItem, true);

				ch->m_inventory.decreaseItemCount(pNeedItem, 1);
			}
		}
		break; // MSG_EVENT_WHITEDAY_2007_LETTER_REQ
	}
}

void do_EventGomdori2007(CPC* ch, CNetMsg::SP& msg)
{
	if( !gserver->isActiveEvent( A_EVENT_BEAR_DOLL) )
		return;

	// 승패 확률 그외는 비김, 승리-0, 패배-1
	const static int nTableWinProb[8][2] =
	{
		{34, 67},		// 0승 -> 1승
		{34, 67},		// 1승 -> 2승
		{34, 67},		// 2승 -> 3승
		{30, 70},		// 3승 -> 4승
		{30, 70},		// 4승 -> 5승
		{30, 70},		// 5승 -> 6승
		{30, 65},		// 6승 -> 7승
		{30, 65},		// 7승 -> 8승
	};

	const static int nTableGomdoriItem[9] =
	{
		2335,
		2336,
		2337,
		2338,
		2339,
		2340,
		2341,
		2342,
		2343
	};

	const static int nTableGiftSize = 9;

	unsigned char			cSubType;
	unsigned char			cUserSel;
	unsigned char			cNPCSelWin;
	unsigned char			cNPCSelLose;
	int						i;
	int						nWinProb;

	RefMsg(msg) >> cSubType;

	switch (cSubType)
	{
	case MSG_EVENT_GOMDORI_2007_CHECKITEM:
		{
			for (i = 0; i < 9; i++)
			{
				if (ch->m_inventory.FindByDBIndex(nTableGomdoriItem[i], 0, 0) == NULL)
				{
					CNetMsg::SP rmsg(new CNetMsg);
					EventGomdori2007Msg(rmsg, MSG_EVENT_GOMDORI_2007_ERROR_NOITEM);
					SEND_Q(rmsg, ch->m_desc);
					return ;
				}
			}

			CNetMsg::SP rmsg(new CNetMsg);
			EventGomdori2007Msg(rmsg, MSG_EVENT_GOMDORI_2007_CHECKITEM);
			SEND_Q(rmsg, ch->m_desc);
		}
		break;

	case MSG_EVENT_GOMDORI_2007_START:
		{
			if (ch->m_bEventGomdori2007Start || ch->m_nEventGomdori2007Win > 0)
				return ;

			CItem*	pItems[9] = { NULL, };

			for (i = 0; i < 9; i++)
			{
				pItems[i] = ch->m_inventory.FindByDBIndex(nTableGomdoriItem[i], 0, 0);
				if (pItems[i] == NULL)
				{
					CNetMsg::SP rmsg(new CNetMsg);
					EventGomdori2007Msg(rmsg, MSG_EVENT_GOMDORI_2007_ERROR_NOITEM);
					SEND_Q(rmsg, ch->m_desc);
					return ;
				}
			}

			for (i = 0; i < 9; i++)
			{
				ch->m_inventory.decreaseItemCount(pItems[i], 1);
			}

			ch->m_bEventGomdori2007Start = true;
			ch->m_nEventGomdori2007Win = 0;
			ch->m_nEventGomdori2007Game = 1;
			ch->m_nEventGomdori2007Total++;
			ch->m_bEventGomdori2007CanSelect = true;

			{
				CNetMsg::SP rmsg(new CNetMsg);
				EventGomdori2007Msg(rmsg, MSG_EVENT_GOMDORI_2007_START);
				SEND_Q(rmsg, ch->m_desc);
			}

			{
				CNetMsg::SP rmsg(new CNetMsg);
				ConnEventGomdori2007CountMsg(rmsg);
				SEND_Q(rmsg, gserver->m_connector);
			}

			GAMELOG << init("EVENT GOMDORI GAME START", ch) << end;
		}
		break;

	case MSG_EVENT_GOMDORI_2007_SELECT:
		{
			if (!ch->m_bEventGomdori2007Start || ch->m_nEventGomdori2007Win >= nTableGiftSize - 1 || !ch->m_bEventGomdori2007CanSelect)
				return ;

			RefMsg(msg) >> cUserSel;
			switch (cUserSel)
			{
			case 0:
				cNPCSelWin  = 1;
				cNPCSelLose = 2;
				break;		// 가위
			case 1:
				cNPCSelWin  = 2;
				cNPCSelLose = 0;
				break;		// 바위
			case 2:
				cNPCSelWin  = 0;
				cNPCSelLose = 1;
				break;		// 보
			default:
				return ;
			}
			ch->m_bEventGomdori2007CanSelect = false;
			nWinProb = GetRandom(1, 100);
#ifdef _DEBUG_
			switch (cUserSel)
			{
			case 0:
				nWinProb = 20;
				break;		// 가위
			case 1:
				nWinProb = 60;
				break;		// 바위
			case 2:
				nWinProb = 80;
				break;		// 보
			}
#endif // _DEBUG
			if (ch->getRockPaperScissorsFlag() && ch->m_admin == 10)
			{
				nWinProb = 1;
			}

			if (nTableWinProb[(int)ch->m_nEventGomdori2007Win][0] >= nWinProb)
			{
				// 유저 승리
				ch->m_nEventGomdori2007Win++;
				GAMELOG << init("EVENT GOMDORI GAME RESULT WIN", ch)
						<< "USER SEL" << delim
						<< cUserSel << delim
						<< "WIN TOTAL" << delim
						<< ch->m_nEventGomdori2007Win
						<< end;
				CNetMsg::SP rmsg(new CNetMsg);
				EventGomdori2007ResultMsg(rmsg, MSG_EVENT_GOMDORI_2007_RESULT_WIN, ch->m_nEventGomdori2007Win, cUserSel, cNPCSelLose);
				SEND_Q(rmsg, ch->m_desc);
			}
			else if (nTableWinProb[(int)ch->m_nEventGomdori2007Win][1] >= nWinProb)
			{
				// NPC 승리
				ch->m_bEventGomdori2007Start = false;
				GAMELOG << init("EVENT GOMDORI GAME RESULT LOSE", ch)
						<< "USER SEL" << delim
						<< cUserSel << delim
						<< "WIN TOTAL" << delim
						<< ch->m_nEventGomdori2007Win
						<< end;
				CNetMsg::SP rmsg(new CNetMsg);
				EventGomdori2007ResultMsg(rmsg, MSG_EVENT_GOMDORI_2007_RESULT_LOSE, ch->m_nEventGomdori2007Win, cUserSel, cNPCSelWin);
				SEND_Q(rmsg, ch->m_desc);
			}
			else
			{
				// 비김
				GAMELOG << init("EVENT GOMDORI GAME RESULT DRAW", ch)
						<< "USER SEL" << delim
						<< cUserSel << delim
						<< "WIN TOTAL" << delim
						<< ch->m_nEventGomdori2007Win
						<< end;
				CNetMsg::SP rmsg(new CNetMsg);
				EventGomdori2007ResultMsg(rmsg, MSG_EVENT_GOMDORI_2007_RESULT_DRAW, ch->m_nEventGomdori2007Win, cUserSel, cUserSel);
				SEND_Q(rmsg, ch->m_desc);
			}
		}
		break;

	case MSG_EVENT_GOMDORI_2007_CONTINUE:
		{
			if (!ch->m_bEventGomdori2007Start)
				return ;

			CNetMsg::SP rmsg(new CNetMsg);
			EventGomdori2007Msg(rmsg, MSG_EVENT_GOMDORI_2007_CONTINUE);
			SEND_Q(rmsg, ch->m_desc);
			GAMELOG << init("EVENT GOMDORI GAME CONTINUE", ch) << end;
			ch->m_nEventGomdori2007Game++;
			ch->m_bEventGomdori2007CanSelect = true;
		}
		break;

	case MSG_EVENT_GOMDORI_2007_END:
		{
			if (ch->m_nEventGomdori2007Game < 1)
				return ;

			if (ch->m_nEventGomdori2007Win == 0 && ch->m_nEventGomdori2007FirstLose == 1)
			{
				if( gserver->m_rewardMgr.setRewardData(ch, 24) && gserver->m_rewardMgr.doReward(ch,24) )
				{
					CNetMsg::SP rmsg(new CNetMsg);
					EventGomdori2007EndMsg(rmsg, ch->m_nEventGomdori2007Win, 1, 0, 0);
					SEND_Q(rmsg, ch->m_desc);

					ch->m_nEventGomdori2007FirstLose = 0;
				}
			}
			else if (ch->m_nEventGomdori2007Win == 1 && ch->m_nEventGomdori2007FirstWin == 1)
			{
				// 엽기 무기 보상 묻기
				const std::vector<REWAD_DATA_INFO> * rewardDataList = NULL;

				gserver->m_rewardMgr.setRewardData(ch, 25);
				rewardDataList = gserver->m_rewardMgr.getRewardDataList();

				if( rewardDataList->size() == 0 )
					return;

				int select1 = (*rewardDataList)[0].index;

				gserver->m_rewardMgr.setRewardData(ch, 26);

				rewardDataList = gserver->m_rewardMgr.getRewardDataList();

				if( rewardDataList->size() == 0 )
					return;

				int select2 = (*rewardDataList)[0].index;

				CNetMsg::SP rmsg(new CNetMsg);
				EventGomdori2007EndMsg(rmsg, ch->m_nEventGomdori2007Win, 2, select1, select2);
				SEND_Q(rmsg, ch->m_desc);
				return ;
			}
			else
			{
				if( gserver->m_rewardMgr.setRewardData(ch, 27, -1, (int)ch->m_nEventGomdori2007Win ) )
				{
					const std::vector<REWAD_DATA_INFO> * rewardDataList = gserver->m_rewardMgr.getRewardDataList();
					int nGiftIndex = (*rewardDataList)[0].index;
					int nGiftCount = (*rewardDataList)[0].value3;
					if( gserver->m_rewardMgr.doReward(ch, 27) )
					{
						CNetMsg::SP rmsg(new CNetMsg);
						EventGomdori2007EndMsg(rmsg, ch->m_nEventGomdori2007Win, 0, nGiftIndex, nGiftCount);
						SEND_Q(rmsg, ch->m_desc);

						// 초기화
						ch->m_nEventGomdori2007Win = 0;
						ch->m_nEventGomdori2007Game = 0;
						ch->m_bEventGomdori2007Start = false;

						return;
					}
				}

				CNetMsg::SP rmsg(new CNetMsg);
				EventGomdori2007Msg(rmsg, MSG_EVENT_GOMDORI_2007_ERROR_FULLINVENTORY);
				SEND_Q(rmsg, ch->m_desc);
				return;
			}
		}
		break;

	case MSG_EVENT_GOMDORI_2007_SELECT_GIFT:
		{
			int	nSelItem;
			RefMsg(msg) >> nSelItem;
			int rewardIndex = -1;

			const std::vector<REWAD_DATA_INFO> * rewardDataList = NULL;

			gserver->m_rewardMgr.setRewardData(ch, 25);
			rewardDataList = gserver->m_rewardMgr.getRewardDataList();

			if( rewardDataList->size() == 0 )
				return;

			if( nSelItem == (*rewardDataList)[0].index )
				rewardIndex = 25;
			else
			{
				gserver->m_rewardMgr.setRewardData(ch, 26);
				rewardDataList = gserver->m_rewardMgr.getRewardDataList();

				if( rewardDataList->size() == 0 )
					return;

				if( nSelItem == (*rewardDataList)[0].index )
					rewardIndex = 26;
				else
					return;
			}
			if( gserver->m_rewardMgr.setRewardData(ch, rewardIndex ) && gserver->m_rewardMgr.doReward(ch, rewardIndex) )
			{
				CNetMsg::SP rmsg(new CNetMsg);
				EventGomdori2007SelectGiftMsg(rmsg, nSelItem);
				SEND_Q(rmsg, ch->m_desc);

				// 초기화
				ch->m_nEventGomdori2007Win = 0;
				ch->m_nEventGomdori2007Game = 0;
				ch->m_nEventGomdori2007FirstWin = 0;
				ch->m_bEventGomdori2007Start = false;
			}
			else
			{
				CNetMsg::SP rmsg(new CNetMsg);
				EventGomdori2007Msg(rmsg, MSG_EVENT_GOMDORI_2007_ERROR_FULLINVENTORY);
				SEND_Q(rmsg, ch->m_desc);
				return;
			}
		}
		break;

	case MSG_EVENT_GOMDORI_2007_VIEW_STATUS:
		{
			CNetMsg::SP rmsg(new CNetMsg);
			ConnEventGomdori2007StatusMsg(rmsg, ch->m_index);
			SEND_Q(rmsg, gserver->m_connector);
		}
		break;

	default:
		return ;
	}
}

#if defined( EVENT_SUMMER_VACATION_2007_RESULT )
void do_EventSummerVacation2007( CPC* ch, CNetMsg::SP& msg )
{
	int i=0;
	const static int nTableGiftSize = 10;
	const static int nTableGift[nTableGiftSize][3] =
	{
		//figuretype, item_index, count
		{1, 19, 500},
		{2, 47, 3 },
		{3, 724, 10},
		{4, 45, 10},
		{5, 723, 2},
		{6, 723, 5},
		{7, 723,10},
		{8, 85, 1},
		{9, 85, 2},
		{10, 85,3}
	};

	unsigned char cSubType;

//	int paperindex[4];
//	int papercount[4];

	CItem*	pItem;
	RefMsg(msg) >> cSubType;

	switch( cSubType )
	{
	case MSG_EVENT_SUMMER_VACATION_2007_PAPER_FIGURES:
		{
			int itemindex[3];
			int redpaper = 0;
			int greenpaper = 0;
			int silverpaper = 0;
			int goldpaper = 0;
			for( i = 0; i < 3; i++ )
			{
				RefMsg(msg) >> itemindex[i];
				if( itemindex[i] == GREEN_PAPER_ITEM )
					greenpaper++;
				else if( itemindex[i] == RED_PAPER_ITEM )
					redpaper++;
				else if( itemindex[i] == SILVER_PAPER_ITEM )
					silverpaper++;
				else if( itemindex[i] == GOLD_PAPER_ITEM )
					goldpaper++;
			}

			int MixSheet[10][5] =
			{
				// 골드, 실버, 레드, 그린, 피규어
				{0, 0, 0, 3, 1 },
				{0, 0, 1, 2, 2 },
				{0, 0, 2, 1, 3 },
				{0, 0, 3, 0, 4 },
				{0, 1, 2, 0, 5 },
				{0, 2, 1, 0, 6 },
				{0, 3, 0, 0, 7 },
				{1, 2, 0, 0, 8 },
				{2, 1, 0, 0, 9 },
				{3, 0, 0, 0, 10 },
			};
			int figuretype =0;

			for( i = 0; i < 10; i++ )
			{
				if( MixSheet[i][0] == goldpaper && MixSheet[i][1] == silverpaper &&
						MixSheet[i][2] == redpaper && MixSheet[i][3] == greenpaper )
				{
					figuretype = MixSheet[i][4];
					break;
				}
			}

			if( figuretype == 0 )
			{
				CNetMsg::SP rmsg(new CNetMsg);
				EventSummerVacation2007FigureMsg( rmsg, MSG_EVENT_SUMMER_VACATION_2007_ERROR_MAKE_FAIL, 0, 0, 0 );
				SEND_Q( rmsg, ch->m_desc );
				return;
			}

			item_search_t vec_redpaper;
			item_search_t vec_greenpaper;
			item_search_t vec_silverpapaer;
			item_search_t vec_goldpaper;

			if( redpaper != 0 )
			{
				int sc = ch->m_inventory.searchItemByCondition(RED_PAPER_ITEM, 0, 0, vec_redpaper);
				if (sc == 0 || sc < redpaper)
				{
					CNetMsg::SP rmsg(new CNetMsg);
					EventSummerVacation2007FigureMsg( rmsg, MSG_EVENT_SUMMER_VACATION_2007_ERROR_NOITEM, 0, 0, 0 );
					SEND_Q( rmsg, ch->m_desc );
					return;
				}
			}

			if( greenpaper != 0)
			{
				int sc = ch->m_inventory.searchItemByCondition(GREEN_PAPER_ITEM, 0, 0, vec_greenpaper);
				if (sc == 0 || sc < greenpaper)
				{
					CNetMsg::SP rmsg(new CNetMsg);
					EventSummerVacation2007FigureMsg( rmsg, MSG_EVENT_SUMMER_VACATION_2007_ERROR_NOITEM, 0, 0, 0 );
					SEND_Q( rmsg, ch->m_desc );
					return;
				}
			}

			if( silverpaper != 0 )
			{
				int sc = ch->m_inventory.searchItemByCondition(SILVER_PAPER_ITEM, 0, 0, vec_silverpapaer);
				if (sc == 0 || sc < silverpaper)
				{
					CNetMsg::SP rmsg(new CNetMsg);
					EventSummerVacation2007FigureMsg( rmsg, MSG_EVENT_SUMMER_VACATION_2007_ERROR_NOITEM, 0, 0, 0 );
					SEND_Q( rmsg, ch->m_desc );
					return;
				}
			}

			if( goldpaper != 0 )
			{
				int sc = ch->m_inventory.searchItemByCondition(GOLD_PAPER_ITEM, 0, 0, vec_goldpaper);
				if (sc == 0 || sc < goldpaper)
				{
					CNetMsg::SP rmsg(new CNetMsg);
					EventSummerVacation2007FigureMsg( rmsg, MSG_EVENT_SUMMER_VACATION_2007_ERROR_NOITEM, 0, 0, 0 );
					SEND_Q( rmsg, ch->m_desc );
					return;
				}
			}

			if( ch->m_inventory.getMoney() < 100 )
			{
				CNetMsg::SP rmsg(new CNetMsg);
				EventSummerVacation2007FigureMsg( rmsg, MSG_EVENT_SUMMER_VACATION_2007_ERROR_NOMONEY, 0, 0, 0 );
				SEND_Q( rmsg, ch->m_desc );
				return;
			}

			CItem* item = NULL;
			int giftitemindx = 0;
			int itemcount = 0;
			for( i = 0; i < 10 ; i++ )
			{
				if( nTableGift[i][0] == figuretype )
				{
					item = gserver->m_itemProtoList.CreateItem( nTableGift[i][1], -1, 0, 0, nTableGift[i][2] );
					giftitemindx  = nTableGift[i][1];
					itemcount = nTableGift[i][2];
					break;
				}
			}

			if( giftitemindx == 0 && figuretype == 0 )
			{
				CNetMsg::SP rmsg(new CNetMsg);
				EventSummerVacation2007FigureMsg( rmsg, MSG_EVENT_SUMMER_VACATION_2007_ERROR_MAKE_FAIL, figuretype, giftitemindx, itemcount );
				SEND_Q(rmsg, ch->m_desc );
				return;
			}

			if( item )
			{
				if (ch->m_inventory.addItem(item))
				{
					GAMELOG << init("EVENT SUMMER VACATION 2007 GIVE ITEM", ch )
							<< "GIFT" << delim
							<< itemlog( item )
							<< end;

					CNetMsg::SP rmsg(new CNetMsg);
					EventSummerVacation2007FigureMsg( rmsg, MSG_EVENT_SUMMER_VACATION_2007_ERROR_MAKE_OK, figuretype, giftitemindx, itemcount );
					SEND_Q(rmsg, ch->m_desc );
				}
				else
				{
					delete item;

					CNetMsg::SP rmsg(new CNetMsg);
					EventSummerVacation2007FigureMsg( rmsg, MSG_EVENT_SUMMER_VACATION_2007_ERROR_FULLINVENTORY, figuretype, giftitemindx, itemcount );
					SEND_Q( rmsg, ch->m_desc );

					return;
				}

				{
					// 나스 제거
					ch->m_inventory.decreaseMoney(100);
				}

				if( redpaper != 0 )
				{
					ch->m_inventory.deleteItem(vec_redpaper, redpaper);
				}

				if( greenpaper != 0)
				{
					ch->m_inventory.deleteItem(vec_greenpaper, greenpaper);
				}

				if( silverpaper != 0 )
				{
					ch->m_inventory.deleteItem(vec_silverpapaer, silverpaper);
				}

				if( goldpaper != 0 )
				{
					ch->m_inventory.deleteItem(vec_goldpaper, goldpaper);
				}
			}
			else
			{
				CNetMsg::SP rmsg(new CNetMsg);
				EventSummerVacation2007FigureMsg( rmsg, MSG_EVENT_SUMMER_VACATION_2007_ERROR_FULLINVENTORY, figuretype, giftitemindx, itemcount );
				SEND_Q( rmsg, ch->m_desc );
			}
		}
		break;

	case MSG_EVENT_SUMMER_VACATION_2007_INCHEN:
		{
			int paperindex;

			RefMsg(msg) >> paperindex;

			item_search_t vec;
			int sc = ch->m_inventory.searchItemByCondition(paperindex, 0, 0, vec);
			if (sc == 0 || sc < 5)
			{
				CNetMsg::SP rmsg(new CNetMsg);
				EventSummerVacationInchenMsg( rmsg, MSG_EVENT_SUMMER_VACATION_2007_ERROR_NOITEM, paperindex );
				SEND_Q(rmsg, ch->m_desc);
				return ;
			}

			const int GoldPaperInchenProb = 100;
			const int SilverPaperIncheProb = 400;
			const int RedPaperInchenProb = 900;
			int nProb = GetRandom( 1, 1000 );
			if( paperindex == GREEN_PAPER_ITEM )
			{
				if( nProb < RedPaperInchenProb )
				{
					// 성공 메세지 전송
					CItem* giveitem = gserver->m_itemProtoList.CreateItem( RED_PAPER_ITEM, -1, 0, 0, 1 );

					if( giveitem )
					{
						if (ch->m_inventory.addItem(giveitem))
						{
							CNetMsg::SP rmsg(new CNetMsg);
							EventSummerVacationInchenMsg( rmsg, MSG_EVENT_SUMMER_VACATION_2007_ERROR_INCHEN_OK, RED_PAPER_ITEM );
							SEND_Q(rmsg, ch->m_desc);

							// 인첸 된 아이템 삭제 - 5개
							ch->m_inventory.deleteItem(vec, 5);

							return;
						}
						else
						{
							delete giveitem;

							CNetMsg::SP rmsg(new CNetMsg);
							EventSummerVacationInchenMsg( rmsg, MSG_EVENT_SUMMER_VACATION_2007_ERROR_FULLINVENTORY, paperindex );
							SEND_Q( rmsg, ch->m_desc );
							return;
						}
					}
				}

				{
					CNetMsg::SP rmsg(new CNetMsg);
					EventSummerVacationInchenMsg( rmsg, MSG_EVENT_SUMMER_VACATION_2007_ERROR_INCHEN_FALIL, paperindex );
					SEND_Q(rmsg, ch->m_desc);
				}

				// 실패전송
				// 인첸 된 아이템 삭제 - 4개
				ch->m_inventory.deleteItem(vec, 4);

				return;
			}
			else if ( paperindex == RED_PAPER_ITEM )
			{
				if( nProb < SilverPaperIncheProb )
				{
					// 성공 메세지 전송
					CItem* giveitem = gserver->m_itemProtoList.CreateItem( SILVER_PAPER_ITEM, -1, 0, 0, 1 );

					if( giveitem )
					{
						if (ch->m_inventory.addItem(giveitem))
						{
							CNetMsg::SP rmsg(new CNetMsg);
							EventSummerVacationInchenMsg( rmsg, MSG_EVENT_SUMMER_VACATION_2007_ERROR_INCHEN_OK, SILVER_PAPER_ITEM );
							SEND_Q(rmsg, ch->m_desc);

							ch->m_inventory.deleteItem(vec, 5);

							return;
						}
						else
						{
							delete giveitem;

							CNetMsg::SP rmsg(new CNetMsg);
							EventSummerVacationInchenMsg( rmsg, MSG_EVENT_SUMMER_VACATION_2007_ERROR_FULLINVENTORY, paperindex );
							SEND_Q( rmsg, ch->m_desc );

							return;
						}
					}
				}

				{
					CNetMsg::SP rmsg(new CNetMsg);
					EventSummerVacationInchenMsg( rmsg, MSG_EVENT_SUMMER_VACATION_2007_ERROR_INCHEN_FALIL, paperindex );
					SEND_Q(rmsg, ch->m_desc);
				}

				// 실패전송
				// 인첸 된 아이템 삭제 - 4개
				ch->m_inventory.deleteItem(vec, 4);

				return;
			}
			else if( paperindex == SILVER_PAPER_ITEM )
			{
				if( nProb < GoldPaperInchenProb )
				{
					// 성공 메세지 전송
					CItem* giveitem = gserver->m_itemProtoList.CreateItem( GOLD_PAPER_ITEM, -1, 0, 0, 1 );

					if( giveitem )
					{
						if (ch->m_inventory.addItem(giveitem))
						{
							CNetMsg::SP rmsg(new CNetMsg);
							EventSummerVacationInchenMsg( rmsg, MSG_EVENT_SUMMER_VACATION_2007_ERROR_INCHEN_OK, GOLD_PAPER_ITEM );
							SEND_Q(rmsg, ch->m_desc);

							ch->m_inventory.deleteItem(vec, 5);

							return;
						}
						else
						{
							delete giveitem;

							CNetMsg::SP rmsg(new CNetMsg);
							EventSummerVacationInchenMsg( rmsg, MSG_EVENT_SUMMER_VACATION_2007_ERROR_FULLINVENTORY, paperindex );
							SEND_Q( rmsg, ch->m_desc );

							return;
						}
					}
				}

				{
					CNetMsg::SP rmsg(new CNetMsg);
					EventSummerVacationInchenMsg( rmsg, MSG_EVENT_SUMMER_VACATION_2007_ERROR_INCHEN_FALIL, paperindex );
					SEND_Q(rmsg, ch->m_desc);
				}

				// 실패전송
				// 인첸 된 아이템 삭제 - 4개
				ch->m_inventory.deleteItem(vec, 4);

				return;
			}
			else if( paperindex == GOLD_PAPER_ITEM )
			{
				CNetMsg::SP rmsg(new CNetMsg);
				EventSummerVacationInchenMsg( rmsg, MSG_EVENT_SUMMER_VACATION_2007_ERROR_INCHEN_FALIL, paperindex );
				SEND_Q(rmsg, ch->m_desc);
				return;
			}
		}
		break;
	default:
		return;
	}
}

#endif// defined( EVENT_SUMMER_VACATION_2007_RESULT )

void do_EventOpenAdultServer(CPC* ch, CNetMsg::SP& msg)
{
	unsigned char nSubType;
	RefMsg(msg) >> nSubType;

	try
	{
		switch(nSubType)
		{
		case MSG_EVENT_OPEN_ADULT_SERVER_MAGIC_REQ:
			{
				int nNeedItemCountSpade = 1;			// 교환에 필요한 스페이드 개수
				int nNeedItemCountHeart = 1;			// 교환에 필요한 하트 개수

				item_search_t vec_pItemSpade;
				item_search_t vec_pItemHeart;

				if(nNeedItemCountSpade > 0)
				{
					int sc = ch->m_inventory.searchItemByCondition(2383 , 0, 0, vec_pItemSpade); // 스페이드 A카드
					if (sc == 0 || sc < nNeedItemCountSpade)
					{
						throw MSG_EVENT_OPEN_ADULT_SERVER_MAGIC_NOITEM;	// 아이템 없음
					}
				}

				if(nNeedItemCountHeart > 0)
				{
					int sc = ch->m_inventory.searchItemByCondition(2384 , 0, 0, vec_pItemHeart); // 스페이드 A카드
					if (sc == 0 || sc < nNeedItemCountHeart)
					{
						throw MSG_EVENT_OPEN_ADULT_SERVER_MAGIC_NOITEM;	// 아이템 없음
					}
				}

				GAMELOG << init("EVENT MAGIC_CARD", ch) << "MAGIC CARD REWARD START" << end;

				if( !gserver->m_rewardMgr.setRewardData( ch, 14 )
						|| ! gserver->m_rewardMgr.doReward( ch, 14) )
					throw MSG_EVENT_OPEN_ADULT_SERVER_MAGIC_FAILED;

				GAMELOG << init("EVENT MAGIC_CARD", ch) << "MAGIC CARD REWARD END" << end;

				// 재료 지우기
				ch->m_inventory.deleteItem(vec_pItemSpade, nNeedItemCountSpade);

				ch->m_inventory.deleteItem(vec_pItemHeart, nNeedItemCountHeart);

				{
					CNetMsg::SP rmsg(new CNetMsg);
					EventOpenAdultServerMsg(rmsg, MSG_EVENT_OPEN_ADULT_SERVER_MAGIC_OK);
					SEND_Q(rmsg, ch->m_desc);
				}
			}
			break;
		}
	}
	catch(MSG_EVENT_OPEN_ADULT_SERVER_TYPE err)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		EventOpenAdultServerMsg(rmsg, err);
		SEND_Q(rmsg, ch->m_desc);
	}
}



void do_EventHalloween2007( CPC* ch, CNetMsg::SP& msg )
{
	if( !gserver->isActiveEvent( A_EVENT_HALLOWEEN ) )
		return ;

	unsigned char ucSubType;
#ifdef NPC_CHECK
	int npcIndex;
	CNPC *npc;
	RefMsg(msg) >> ucSubType
				>> npcIndex;
	npc = ch->m_pArea->FindProtoNPCInCell(ch, npcIndex, false, 2);
	if(!npc)
	{
		GAMELOG << init("EventHalloween FAIL: NOT EXIST NPC (Dangerous User)", ch) << end;
		return ;
	}
	if( !npc->IsFlag(NPC_EVENT) )
	{
		GAMELOG << init("EventHalloween FAIL: NOT EXIST NPC (Dangerous User)", ch) << end;
		return ;
	}
#else
	RefMsg(msg) >> ucSubType;
#endif

	int nTasteSealIndex[2] = { 2488, 2489 };
	int nCandyIndex[6] = { 2482, 2483, 2484, 2485, 2486, 2487 };

	switch( ucSubType )
	{
	case MSG_EVENT_TAKE_DEVILHEAIR :		// Helper 에 확인 요청
		{
			CNetMsg::SP rmsg(new CNetMsg);
			HelperHalloween2007Msg( rmsg, ch->m_index );
			SEND_Q( rmsg, gserver->m_helper );
		}
		break;

	case MSG_EVENT_CHANGE_CANDYBASKET :		// 사탕바구니 확인하고 렌덤으로 사탕주기
		{
			CItem* pCandyBasket = ch->m_inventory.FindByDBIndex(2490/*사탕바구니*/, 0, 0);
			if (pCandyBasket == NULL)
			{
				CNetMsg::SP rmsg(new CNetMsg);
				EventHalloween2007Msg( rmsg, MSG_EVENT_CHANGE_CANDYBASKET, 1 );		// 사탕 바구니 없음
				SEND_Q( rmsg, ch->m_desc );
				return;
			}

			// 사탕 지급
			if( !gserver->m_rewardMgr.setRewardData(ch, 11 )
					||!gserver->m_rewardMgr.doReward( ch, 11 ) )
			{
				CNetMsg::SP rmsg(new CNetMsg);
				EventHalloween2007Msg( rmsg, MSG_EVENT_FULL_INVEN ); // FULL INVEN
				SEND_Q( rmsg, ch->m_desc );
				return;
			}

			// 아이템 제거
			ch->m_inventory.decreaseItemCount(pCandyBasket, 1);

			{
				CNetMsg::SP rmsg(new CNetMsg);
				EventHalloween2007Msg( rmsg, MSG_EVENT_CHANGE_CANDYBASKET, 0 ); //성공
				SEND_Q( rmsg, ch->m_desc );
			}
		}
		break;

	case MSG_EVENT_CHANGE_PUMKINHEAD :		// 사탕 6개로 교환 받기
		{
			CItem* pCandy[6] = { NULL, };

			for (int i = 0; i < 6 ; i++ )
			{
				pCandy[i] = ch->m_inventory.FindByDBIndex(nCandyIndex[i]/*사탕*/, 0, 0);
				if (pCandy[i] == NULL)
				{
					CNetMsg::SP rmsg(new CNetMsg);
					EventHalloween2007Msg( rmsg, MSG_EVENT_CHANGE_PUMKINHEAD, 1 );		// 사탕 없음
					SEND_Q( rmsg, ch->m_desc );
					return;
				}
			}

			if( !gserver->m_rewardMgr.setRewardData(ch, 8 )
					|| !gserver->m_rewardMgr.doReward( ch, 8 ) )
			{
				CNetMsg::SP rmsg(new CNetMsg);
				EventHalloween2007Msg( rmsg, MSG_EVENT_FULL_INVEN );
				SEND_Q( rmsg, ch->m_desc );
				return;
			}

			// 아이템 제거
			for (int i = 0; i < 6 ; i++ )
			{
				ch->m_inventory.decreaseItemCount(pCandy[i], 1);
			}

			{
				CNetMsg::SP rmsg(new CNetMsg);
				EventHalloween2007Msg( rmsg, MSG_EVENT_CHANGE_PUMKINHEAD, 0 ); //성공
				SEND_Q( rmsg, ch->m_desc );
			}
		}
		break;

	case MSG_EVENT_CHANGE_WITCHHAT :		// 증표 2개(이벤트인벤) 확인 후 교환
		{
			CItem* pTasteSeal[2] = { NULL, };

			for(int i = 0; i < 2 ; i++ )
			{
				if ((pTasteSeal[i] = ch->m_inventory.FindByDBIndex(nTasteSealIndex[i]/*맛증표*/, 0, 0 )) == NULL)
				{
					CNetMsg::SP rmsg(new CNetMsg);
					EventHalloween2007Msg( rmsg, MSG_EVENT_CHANGE_WITCHHAT, 1 );		// 사탕 바구니 없음
					SEND_Q( rmsg, ch->m_desc );
					return;
				}
			}

			// 마녀 모자 지급
			if( !gserver->m_rewardMgr.setRewardData(ch,9)
					||!gserver->m_rewardMgr.doReward( ch, 9 ) )
			{
				CNetMsg::SP rmsg(new CNetMsg);
				EventHalloween2007Msg( rmsg, MSG_EVENT_FULL_INVEN );
				SEND_Q( rmsg, ch->m_desc );
				return;
			}

			// 아이템 제거
			for(int i = 0; i < 2 ; i++ )
			{
				ch->m_inventory.decreaseItemCount(pTasteSeal[i], 1);
			}

			{
				CNetMsg::SP rmsg(new CNetMsg);
				EventHalloween2007Msg( rmsg, MSG_EVENT_CHANGE_WITCHHAT, 0 ); //성공
				SEND_Q( rmsg, ch->m_desc );
			}
		}
		break;

	case MSG_EVENT_CHANGE_PUMKIN :			// 호박을 아이템으로
		{
			CItem* pPumkin = ch->m_inventory.FindByDBIndex(2491/*호박*/, 0, 0 );
			if (pPumkin == NULL)
			{
				CNetMsg::SP rmsg(new CNetMsg);
				EventHalloween2007Msg( rmsg, MSG_EVENT_CHANGE_PUMKIN, 1 );		// 사탕 바구니 없음
				SEND_Q( rmsg, ch->m_desc );
				return;
			}

			// 마녀 모자 지급
			if( !gserver->m_rewardMgr.setRewardData(ch, 10)
					|| !gserver->m_rewardMgr.doReward( ch, 10 ) )
			{
				CNetMsg::SP rmsg(new CNetMsg);
				EventHalloween2007Msg( rmsg, MSG_EVENT_FULL_INVEN );
				SEND_Q( rmsg, ch->m_desc );
				return;
			}

			// 아이템 제거
			ch->m_inventory.decreaseItemCount(pPumkin, 1);

			{
				addItemResult& invenResult = ch->m_inventory.getLastAddItemResult();

				//성공
				CNetMsg::SP rmsg(new CNetMsg);
				EventHalloween2007Msg( rmsg, MSG_EVENT_CHANGE_PUMKIN, 0 );
				RefMsg(rmsg) << invenResult.pItem->getDBIndex();
				SEND_Q( rmsg, ch->m_desc );
			}
		}
		break;
	}
}


void do_EventNewMoonstone(CPC* ch, CNetMsg::SP& msg)
{
	unsigned char subtype;
	RefMsg(msg) >> subtype;

	// 인벤 체크
	if (ch->m_inventory.getEmptyCount() < 1)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		EventNewMoonStoneMsg(rmsg, MSG_EVENT_NEW_MOONSTONE_FULLINVEN);
		SEND_Q( rmsg, ch->m_desc );
		return;
	}

	switch(subtype)
	{
	case MSG_EVENT_NEW_MOONSTONE_START_REQ:
		{
			CNetMsg::SP rmsg(new CNetMsg);
			EventNewMoonStoneMsg(rmsg, MSG_EVENT_NEW_MOONSTONE_START_REP);
			RefMsg(rmsg) << gserver->m_nMoonStoneNas;
			SEND_Q( rmsg, ch->m_desc );
		}
		break;
	case MSG_EVENT_NEW_MOONSTONE_TRY_REQ:
		{
			int nMoonStoneType;
			RefMsg(msg) >> nMoonStoneType;

			if(ch->m_nMSGiftIndex != 0 || ch->m_nMSIndex != 0)
			{
				ch->m_nMSIndex = 0;
				ch->m_nMSGiftIndex = 0;
				return ;
			}

			if (ch->m_inventory.FindByDBIndex(nMoonStoneType) == NULL)
			{
				ch->m_nMSIndex = 0;
				ch->m_nMSGiftIndex = 0;

				CNetMsg::SP rmsg(new CNetMsg);
				EventNewMoonStoneMsg(rmsg, MSG_EVENT_NEW_MOONSTONE_NO_MOONSTONE);
				RefMsg(rmsg) << nMoonStoneType;
				SEND_Q( rmsg, ch->m_desc );
				return ;
			}

			CMoonStoneRewardData* reward = gserver->m_moonstoneReward.Find(nMoonStoneType);
			if(!reward || reward->m_nTotalProb < 2 || reward->m_listReward.empty())
			{
				ch->m_nMSIndex = 0;
				ch->m_nMSGiftIndex = 0;
				return ;
			}

			int rand;
			int totalprob = 0;
			rand = GetRandom(1, reward->m_nTotalProb);

			CMoonStoneRewardData::map_reward_t::iterator it = reward->m_listReward.begin();
			CMoonStoneRewardData::map_reward_t::iterator endit = reward->m_listReward.end();
			for(; it != endit; ++it)
			{
				CMoonStoneRewardData::REWARD* rdata = it->second;

				totalprob += (int)(rdata->prob * 10000.0);
				if(rand <= totalprob)
				{
					ch->m_nMSGiftIndex = rdata->rewardindex;
					break;
				}
			}

			if(ch->m_nMSGiftIndex == 0)
			{
				ch->m_nMSGiftIndex = 0;
				ch->m_nMSIndex = 0;
				return ;
			}

			ch->m_nMSIndex = nMoonStoneType;

			const static int maxNas = 10000000;
			int sendnas = 0;
			if(gserver->m_nMoonStoneNas + 200 > maxNas)
			{
				sendnas = maxNas;
			}
			else
			{
				sendnas = gserver->m_nMoonStoneNas + 200;
			}

			{
				CNetMsg::SP rmsg(new CNetMsg);
				EventNewMoonStoneMsg(rmsg, MSG_EVENT_NEW_MOONSTONE_TRY_REP);
				RefMsg(rmsg) << ch->m_nMSGiftIndex
							 << sendnas;

				SEND_Q( rmsg, ch->m_desc );
			}
		}
		break;
	case MSG_EVENT_NEW_MOONSTONE_RESULT_REQ:
		{
			if(ch->m_nMSGiftIndex == 0 || ch->m_nMSIndex == 0)
			{
				ch->m_nMSGiftIndex = 0;
				ch->m_nMSIndex = 0;
				return ;
			}

			// 문스톤 확인
			CItem* moonstone = ch->m_inventory.FindByDBIndex(ch->m_nMSIndex);
			if (moonstone == NULL)
			{
				ch->m_nMSIndex = 0;
				ch->m_nMSGiftIndex = 0;

				CNetMsg::SP rmsg(new CNetMsg);
				EventNewMoonStoneMsg(rmsg, MSG_EVENT_NEW_MOONSTONE_NO_MOONSTONE);
				RefMsg(rmsg) << ch->m_nMSIndex;
				SEND_Q( rmsg, ch->m_desc );
				return ;
			}

			// 보상품 확인
			CMoonStoneRewardData* reward = gserver->m_moonstoneReward.Find(ch->m_nMSIndex);
			if(!reward)
			{
				ch->m_nMSGiftIndex = 0;
				ch->m_nMSIndex = 0;
				return ;
			}

			CMoonStoneRewardData::map_reward_t::iterator it = reward->m_listReward.find(ch->m_nMSGiftIndex);
			if(it == reward->m_listReward.end())
			{
				ch->m_nMSGiftIndex = 0;
				ch->m_nMSIndex = 0;
				return ;
			}
			CMoonStoneRewardData::REWARD* rdata = it->second;

			// 문스톤 아이템 감소
			ch->m_inventory.decreaseItemCount(moonstone, 1);

			{
				CNetMsg::SP rmsg(new CNetMsg);
				HelperEventMoonStoneUpdateReqMsg(rmsg);
				SEND_Q(rmsg, gserver->m_helper);
			}

			if(rdata->rewardindex == NAS_ITEM_DB_INDEX)		// 잭팟
			{
				{
					CNetMsg::SP rmsg(new CNetMsg);
					HelperEventMoonStoneJackPotReqMsg(rmsg, ch->m_index);
					SEND_Q(rmsg, gserver->m_helper);
				}

				ch->m_nMSIndex = 0;
				ch->m_nMSGiftIndex = 0;

				{
					CNetMsg::SP rmsg(new CNetMsg);
					EventNewMoonStoneMsg(rmsg, MSG_EVENT_NEW_MOONSTONE_JACK_POT);
					SEND_Q(rmsg, ch->m_desc);
				}
				return ;
			}

			CItemProto* itemproto = gserver->m_itemProtoList.FindIndex(rdata->rewardindex);
			if(!itemproto)
			{
				ch->m_nMSIndex = 0;
				ch->m_nMSGiftIndex = 0;
				return ;
			}

			int plus = 0;
			if(itemproto->getItemTypeIdx() == ITYPE_WEAPON) // 무기는 plus 0 ~ 4;
			{
				plus = GetRandom(0, 4);
			}

			CItem* gift = gserver->m_itemProtoList.CreateItem(rdata->rewardindex, -1, plus, rdata->rewardflag, rdata->rewardcount);
			if(!gift)
			{
				ch->m_nMSIndex = 0;
				ch->m_nMSGiftIndex = 0;
				return ;
			}

			if (ch->m_inventory.addItem(gift) == false)
			{
				gift = ch->m_pArea->DropItem(gift, ch);
				if (!gift)
				{
					ch->m_nMSGiftIndex = 0;
					ch->m_nMSIndex = 0;
					return;
				}

				CNetMsg::SP rmsg(new CNetMsg);
				gift->m_preferenceIndex = ch->m_index;
				ItemDropMsg(rmsg, ch, gift);
				ch->m_pArea->SendToCell(rmsg, ch, true);
			}

			ch->m_nMSGiftIndex = 0;
			ch->m_nMSIndex = 0;

			{
				CNetMsg::SP rmsg(new CNetMsg);
				EventNewMoonStoneMsg(rmsg, MSG_EVENT_NEW_MOONSTONE_RESULT_REP);
				RefMsg(rmsg) << rdata->rewardindex
							 << rdata->rewardcount;

				SEND_Q(rmsg, ch->m_desc);
			}
		}
		break;
	case MSG_EVENT_NEW_MOONSTONE_MIX_REQ:
		{
			const int MixGradeCount = 4;
			static const int MoonStoneMixGrade[MixGradeCount][3] =
			{
				{2545, 2546, 90},   // 문스톤 종류, 조합되는 문스톤, 확률
				{2546, 723, 60},
				{723, 2547, 30},
				{2547, 2548, 10},
			};

			const int MixBoxCount = 5;
			const int MixBoxes[MixBoxCount][4] =
			{
				{2552, 100, 100,	1},			// 조합상자종류, 증가확률, 소비되는 나스, 실패시 문스톤사라짐
				{2553, 200, 1000,	1},
				{2554, 300, 10000,	1},
				{2555, 400, 0,		1},
				{2556, 500, 0,		0},
			};

			int nMoonStoneIndex;
			int nMoonStoneMixBox;
			RefMsg(msg) >> nMoonStoneIndex
						>> nMoonStoneMixBox;

			unsigned char cSelMoonStone;
			unsigned char cSelMoonStoneBox;
			switch(nMoonStoneIndex)
			{
			case 2545:
				cSelMoonStone = 0;
				break;
			case 2546:
				cSelMoonStone = 1;
				break;
			case 723:
				cSelMoonStone = 2;
				break;
			case 2547:
				cSelMoonStone = 3;
				break;
			default:
				{
					CNetMsg::SP rmsg(new CNetMsg);
					EventNewMoonStoneMsg(rmsg, MSG_EVENT_NEW_MOONSTONE_MIX_MISS);
					SEND_Q( rmsg, ch->m_desc );
					return ;
				}
			}

			switch(nMoonStoneMixBox)
			{
			case 2552:
				cSelMoonStoneBox = 0;
				break;
			case 2553:
				cSelMoonStoneBox = 1;
				break;
			case 2554:
				cSelMoonStoneBox = 2;
				break;
			case 2555:
				cSelMoonStoneBox = 3;
				break;
			case 2556:
				cSelMoonStoneBox = 4;
				break;
			default:
				return ;
			}

			// 문스톤 있는지 확인
			item_search_t vec_moonstone;
			int sc = ch->m_inventory.searchItemByCondition(nMoonStoneIndex, 0, 0, vec_moonstone);
			if (sc == 0 || sc < 3)
			{
				CNetMsg::SP rmsg(new CNetMsg);
				EventNewMoonStoneMsg(rmsg, MSG_EVENT_NEW_MOONSTONE_NO_MOONSTONE);
				RefMsg(rmsg) << nMoonStoneIndex;
				SEND_Q( rmsg, ch->m_desc );
				return ;
			}

			// 문스톤 상자 있는지 확인
			CItem* moonstonebox = ch->m_inventory.FindByDBIndex(nMoonStoneMixBox);
			if (moonstonebox == NULL)
				return ;

			// 나스검사
			if(MixBoxes[cSelMoonStoneBox][2] > 0)
			{
				if(ch->m_inventory.getMoney() < MixBoxes[cSelMoonStoneBox][2])
				{
					CNetMsg::SP rmsg(new CNetMsg);
					SysMsg( rmsg, MSG_SYS_SHORT_MONEY);
					SEND_Q( rmsg, ch->m_desc );
					return;
				}
			}

			int rand = GetRandom(1, 100);
			if(rand <= MoonStoneMixGrade[cSelMoonStone][2] * MixBoxes[cSelMoonStoneBox][1]/100)
			{
				// 조합성공
				CItem* upgradeitem = gserver->m_itemProtoList.CreateItem(MoonStoneMixGrade[cSelMoonStone][1], -1, 0, 0, 1);
				if (upgradeitem == NULL)
					return ;

				if (ch->m_inventory.addItem(upgradeitem) == false)
				{
					upgradeitem = ch->m_pArea->DropItem(upgradeitem, ch);
					if (!upgradeitem)
						return;

					CNetMsg::SP rmsg(new CNetMsg);
					upgradeitem->m_preferenceIndex = ch->m_index;
					ItemDropMsg(rmsg, ch, upgradeitem);
					ch->m_pArea->SendToCell(rmsg, ch, true);
				}

				// 문스톤, 문스톤 박스 삭제
				ch->m_inventory.deleteItem(vec_moonstone, 3);

				ch->m_inventory.decreaseItemCount(moonstonebox, 1);

				{
					CNetMsg::SP rmsg(new CNetMsg);
					EventNewMoonStoneMsg(rmsg, MSG_EVENT_NEW_MOONSTONE_MIX_SUCCESS);
					RefMsg(rmsg) << upgradeitem->getDBIndex();
					SEND_Q(rmsg, ch->m_desc);
				}

				GAMELOG << init("MOONSTONE MIX SUCCESS", ch)
						<< "Char Index" << delim << ch->m_index << delim
						<< "NewMoonStone" << delim << upgradeitem->getDBIndex()
						<< end ;
			}
			else
			{
				// 조합실패
				if(MixBoxes[cSelMoonStoneBox][3])
				{
					// 문스톤 감소
					ch->m_inventory.deleteItem(vec_moonstone, 2);
				}

				ch->m_inventory.decreaseItemCount(moonstonebox, 1);

				{
					CNetMsg::SP rmsg(new CNetMsg);
					EventNewMoonStoneMsg(rmsg, MSG_EVENT_NEW_MOONSTONE_MIX_FAILED);
					SEND_Q(rmsg, ch->m_desc);
				}

				GAMELOG << init("MOONSTONE MIX FAILED", ch)
						<< end;
			}

			// 차감
			if(MixBoxes[cSelMoonStoneBox][2] > 0)
			{
				ch->m_inventory.decreaseMoney(MixBoxes[cSelMoonStoneBox][2]);
			}
		}
		break;
	default:
		break;
	}
}

void do_EventXmas2007(CPC* ch, CNetMsg::SP& msg)
{
	if( !gserver->isActiveEvent( A_EVENT_XMAS ) )
		return;

	unsigned char subtype;
#ifdef NPC_CHECK
	int npcIndex;
	CNPC *npc;
	RefMsg(msg) >> subtype
				>> npcIndex;
	npc = ch->m_pArea->FindProtoNPCInCell(ch, npcIndex, false, 2);
	if(!npc)
	{
		GAMELOG << init("EventXmas FAIL: NOT EXIST NPC (Dangerous User)", ch) << end;
		return ;
	}
	if( !npc->IsFlag(NPC_EVENT) )
	{
		GAMELOG << init("EventXmas FAIL: NOT EXIST NPC (Dangerous User)", ch) << end;
		return ;
	}
#else
	RefMsg(msg) >> subtype;
#endif

	// KYD TO DO
	switch(subtype)
	{
	case MSG_EVENT_XMASTREE_ADD_POINT :		// 포인트 적립 커넥터로
		{
			// 장식 아이템
			const int ItemCount = 6;
			int nXmasItem[ItemCount] = { 2584, 2585, 2586, 2587, 2588, 2589 }; //양말,막대사탕 , 종, 리본, 구슬, 별

			unsigned int unPoint = 0;
			// 아이템 확인
			for( int i = 0; i < ItemCount; i++ )
			{
				item_search_t vec;
				int sc = ch->m_inventory.searchItemByCondition(nXmasItem[i], 0, 0, vec);
				if (sc > 0)
				{
					unPoint += sc;

					ch->m_inventory.deleteItem(vec, sc);
				}
			}

			{
				// unPoint가 0보다 크면 성공, 작으면 아이템 없음
				CNetMsg::SP rmsg(new CNetMsg);
				EventXmas2007Msg( rmsg, MSG_EVENT_XMASTREE_ADD_POINT );
				if( unPoint > 0 )
					RefMsg(rmsg) << (char) 0;
				else
					RefMsg(rmsg) << (char) 1;
				SEND_Q( rmsg, ch->m_desc );
			}

			if( unPoint > 0 )
			{
				CNetMsg::SP rmsg(new CNetMsg);
				ConnEventXmas2007Msg( rmsg, MSG_CONN_EVENT_XMASTREE_ADDPOINT );
				RefMsg(rmsg) << unPoint;
				SEND_Q( rmsg, gserver->m_connector );
			}
		}
		break;

	case MSG_EVENT_XMASTREE_GET_BLESS :		// 축복 받기
		{
			if( ch->m_assist.FindBySkillIndex( 489 ) == 0 )	// if( 현재 크리스 마스 디버프가 없다. )
			{
				unsigned int unLevel = gserver->m_unEventXmasTreePoint/1000;
				int	nSkillIndex=0;

				// nSkillIndex 스킬 주기
				CSkill* pSkill = NULL;
				bool bApply;

				if( unLevel < 1 )
				{
					nSkillIndex = 0;
				}
				else if ( unLevel >=1 && unLevel < 3 )
				{
					nSkillIndex = 485;
				}
				else if ( unLevel >=3 && unLevel < 15 )
				{
					nSkillIndex = 486;
				}
				else if ( unLevel >=15 && unLevel < 100 )
				{
					nSkillIndex = 487;
				}
				else if ( unLevel >= 100 )
				{
					nSkillIndex = 488;
				}

				for( int i=0; i<2 ; i++ )
				{
					bApply = false;

					pSkill = gserver->m_skillProtoList.Create( nSkillIndex , 1 );
					if( pSkill )
					{
						ApplySkill( ch, ch, pSkill, -1 , bApply );
						if( bApply ) // 스킬 성공
						{
							delete pSkill;
							pSkill = NULL;
							nSkillIndex = 489;	// 크리스 마스 디버프 스킬로 변경;
						}
						else // 스킬 실패
							break;
					}
					else	// 스킬 실패
						break;
				}

				{
					CNetMsg::SP rmsg(new CNetMsg);
					EventXmas2007Msg( rmsg, MSG_EVENT_XMASTREE_GET_BLESS );
					if(bApply)		// 스킬 2개 성공
					{
						RefMsg(rmsg) << (char) 0;
					}
					else			// 스킬 2개 모두는 성공 못했다.
					{
						RefMsg(rmsg) << (char) 1;
					}
					SEND_Q( rmsg, ch->m_desc );
				}
			}
			else
			{
				// 현재 이미 같은 스킬이 있음
				CNetMsg::SP rmsg(new CNetMsg);
				EventXmas2007Msg( rmsg, MSG_EVENT_XMASTREE_GET_BLESS );
				RefMsg(rmsg) << (char) 1;
				SEND_Q( rmsg, ch->m_desc );
			}
		}
		break;
	case MSG_EVENT_XMAS_PUZZLE_CHECKITEM_REQ:
	case MSG_EVENT_XMAS_PUZZLE_START_REQ:
	case MSG_EVENT_XMAS_PUZZLE_SELECT_REQ:
	case MSG_EVENT_XMAS_PUZZLE_CONTINUE_REQ:
	case MSG_EVENT_XMAS_PUZZLE_END_REQ:
		{
			do_EventXmasPuzzle(ch, msg, subtype);
		}
		break;
	}

	return;
}

void do_EventXmasPuzzle( CPC* ch, CNetMsg::SP& msg, unsigned char type )
{
	// 크리스마스 퍼즐 이벤트
	switch (type)
	{
	case MSG_EVENT_XMAS_PUZZLE_CHECKITEM_REQ:
		{
			CNetMsg::SP rmsg(new CNetMsg);
			bool bCanPlay = gserver->m_XmasPuzzleGame->CanPlay(ch);
			if (bCanPlay)
			{
				// 성공 메시지
				EventXmasPuzzleMsg(rmsg, MSG_EVENT_XMAS_PUZZLE_CHECKITEM_REP, MSG_EVENT_XMAS_PUZZLE_SUCCESS);
			}
			else
			{
				// 실패 메시지
				EventXmasPuzzleMsg(rmsg, MSG_EVENT_XMAS_PUZZLE_CHECKITEM_REP, MSG_EVENT_XMAS_PUZZLE_FAIL);
				GAMELOG << init("XMAS_EVENT_PUZZLE_GAME") << "CHECK ITEM ERROR : HAVE NOT PUZZLE ITEM" << end;
			}

			SEND_Q(rmsg, ch->m_desc);
		}
		break;
	case MSG_EVENT_XMAS_PUZZLE_START_REQ:
		{
			CNetMsg::SP rmsg(new CNetMsg);
			bool bStart = gserver->m_XmasPuzzleGame->Play(ch);
			if (bStart)
			{
				ch->StartRockPaperScissors();

				// 스타트 메시지
				EventXmasPuzzleMsg(rmsg, MSG_EVENT_XMAS_PUZZLE_START_REP, MSG_EVENT_XMAS_PUZZLE_SUCCESS);
			}
			else
			{
				// 실패 메시지
				EventXmasPuzzleMsg(rmsg, MSG_EVENT_XMAS_PUZZLE_START_REP, MSG_EVENT_XMAS_PUZZLE_FAIL);
				GAMELOG << init("XMAS_EVENT_PUZZLE_GAME") << "START GAME ERROR : HAVE NOT PUZZLE ITEM" << end;
			}

			SEND_Q(rmsg, ch->m_desc);
		}
		break;
	case MSG_EVENT_XMAS_PUZZLE_SELECT_REQ:
		{
			if (ch->CanRockPaperScissors())
			{
				// 가위바위보를 할 수 있는 상태이니,
				// 가위바위보를 해봅시다. result 0:유저 승리, 1:서버 승리, 2:비김
				unsigned char userChoice, serverChoice;
				RefMsg(msg) >> userChoice;
				int result = gserver->m_XmasRockPaperScissorsGame->Play(ch, userChoice, serverChoice);

				{
					CNetMsg::SP rmsg(new CNetMsg);
					EventXmasPuzzleRes(rmsg, result, ch->m_RockPaperScissorsInfo.GetWinCount(), userChoice, serverChoice);
					SEND_Q(rmsg, ch->m_desc);
				}

				GAMELOG << init("XMAS_EVENT_PUZZLE_GAME") << "SELECT SUCCESS" << delim
						<< ch->m_index << delim
						<< ch->GetName() << delim
						<< result << delim << userChoice << delim << serverChoice << end;
			}
			else
			{
				// 가위바위보를 할 수 없는 상태
				GAMELOG << init("XMAS_EVENT_PUZZLE_GAME") << "SELECT ERROR" << end;
			}
		}
		break;
	case MSG_EVENT_XMAS_PUZZLE_CONTINUE_REQ:
		{
			if ( !ch->m_RockPaperScissorsInfo.IsStart() )
				return;

			// 게임 판수 증가시키고, 선택 가능하게 해주자.
			ch->m_RockPaperScissorsInfo.IncreaseGameCount();
			ch->m_RockPaperScissorsInfo.SetCanPlay(true);

			{
				// continue 메시지 응답해주자.
				CNetMsg::SP rmsg(new CNetMsg);
				EventXmasPuzzleMsg(rmsg, MSG_EVENT_XMAS_PUZZLE_CONTINUE_REP);
				SEND_Q(rmsg, ch->m_desc);
			}

			GAMELOG << init("XMAS_EVENT_PUZZLE_GAME") << "CONTINUE GAME" << end;
		}
		break;
	case MSG_EVENT_XMAS_PUZZLE_END_REQ:
		{
			bool bResult = gserver->m_XmasRockPaperScissorsGame->GiveReward(ch);
			if (bResult)
			{
				const std::vector<REWAD_DATA_INFO> * rewardDataList = gserver->m_rewardMgr.getRewardDataList();
				int nGiftIndex = (*rewardDataList)[0].index;
				int nGiftCount = (*rewardDataList)[0].value3;

				{
					CNetMsg::SP rmsg(new CNetMsg);
					EventXmasPuzzleEnd(rmsg, ch->m_RockPaperScissorsInfo.GetWinCount(), nGiftIndex, nGiftCount);
					SEND_Q(rmsg, ch->m_desc);
				}

				ch->m_RockPaperScissorsInfo.SetWinCount(0);
				ch->m_RockPaperScissorsInfo.SetGameCount(0);
				ch->m_RockPaperScissorsInfo.SetStart(false);

				GAMELOG << init("XMAS_EVENT_PUZZLE_GAME") << "GIVE REWARD SUCCESS" << end;
			}
			else
			{
				CNetMsg::SP rmsg(new CNetMsg);
				EventXmasPuzzleEnd(rmsg, ch->m_RockPaperScissorsInfo.GetWinCount(), -1, -1);
				SEND_Q(rmsg, ch->m_desc);
				GAMELOG << init("XMAS_EVENT_PUZZLE_GAME") << "GIVE REWARD FAIL" << end;
			}
		}
		break;
	}
}

#if defined (SAKURA_EVENT_2008_REWARD)
void do_EventSakura2008( CPC* ch, CNetMsg::SP& msg )
{
	unsigned char subtype;
	RefMsg(msg) >> subtype;

	// KYD TO DO
	switch(subtype)
	{
	case MSG_EVNET_SAKURAEVENT_2008 :			// 퇴비(2345)를 회수하고 상자(2747)를 제공한다.
		{
			//////////////////////////////////////////////////////////////////////////
			item_search_t vec;
			int nSeedCount = ch->m_inventory.searchItemByCondition(2345/*퇴비*/, 0, 0, vec); // 퇴비
			if (nSeedCount == 0)
			{
				CNetMsg::SP rmsg(new CNetMsg);
				EventSakuraEvent2008Msg( rmsg, MSG_EVENT_SAKURAEVENT_2008_ERROR );
				RefMsg(rmsg) << (int) 1; // FAILED
				SEND_Q( rmsg, ch->m_desc );
				return;
			}


			// 벚꽃선물상자 지급
			CItem* pSakuraBox =  gserver->m_itemProtoList.CreateItem( 2747/*벚꽃선물상자*/ , -1, 0, 0, nSeedCount );
			if( !pSakuraBox )
			{
				CNetMsg::SP rmsg(new CNetMsg);
				EventSakuraEvent2008Msg( rmsg, MSG_EVENT_SAKURAEVENT_2008_ERROR );
				RefMsg(rmsg) << (int) 1; // FAILED
				SEND_Q( rmsg, ch->m_desc );
				return;
			}

			if (ch->m_inventory.addItem(pSakuraBox))
			{
				ch->m_inventory.deleteItem(vec, nSeedCount);
			}
			else //아이템 지급 실패
			{
				delete pSakuraBox;

				CNetMsg::SP rmsg(new CNetMsg);
				EventSakuraEvent2008Msg( rmsg, MSG_EVENT_SAKURAEVENT_2008_ERROR );
				RefMsg(rmsg) << (int) 1; // FAILED
				SEND_Q( rmsg, ch->m_desc );

				return;
			}

			{
				CNetMsg::SP rmsg(new CNetMsg);
				EventSakuraEvent2008Msg( rmsg, MSG_EVENT_SAKURAEVENT_2008_ERROR );
				RefMsg(rmsg) << (int) 0; // SUCESS
				GAMELOG << init("SAKURA EVENT CAHNGE SEED ", ch )
						<< "COUNT: " << nSeedCount << end;
				SEND_Q( rmsg, ch->m_desc );
			}
			//////////////////////////////////////////////////////////////////////////
		}
		break;
	}
}
#endif // defined (SAKURA_EVENT_2008_REWARD)

#ifdef CHAOSBALL
void do_ChaosBall(CPC * ch, CNetMsg::SP& msg )
{
	if(ch == NULL)
	{
		return;
	}

	// 0. 인벤토리에 빈 공간이 3개 남아야 한다.
	if(ch->m_inventory.getEmptyCount() < 3)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ChaosBallMsg(rmsg, ERROR_NOSPACE);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 1. 아이템 위치 받기
#ifdef NPC_CHECK
	int npcIndex;
	CNPC *npc;
	RefMsg(msg) >> npcIndex
				>> row >> col;
	npc = ch->m_pArea->FindProtoNPCInCell(ch, npcIndex, false, 2);
	if(!npc)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ChaosBallMsg(rmsg, ERROR_NOT_EXIST_NPC);
		SEND_Q(rmsg, ch->m_desc);
		GAMELOG << init("ChaosBall FAIL: NOT EXIST NPC (Dangerous User)", ch) << end;
		return ;
	}
	if( !npc->IsFlag(NPC_EVENT) )
	{
		GAMELOG << init("ChaosBall FAIL: NOT EXIST NPC (Dangerous User)", ch) << end;
		return ;
	}
#endif

	RequestClient::doChaosBall* packet = reinterpret_cast<RequestClient::doChaosBall*>(msg->m_buf);

	if (ch->m_inventory.isValidNormalInventory(packet->tab, packet->invenIndex) == false)
	{
		LOG_ERROR("HACKING? : invalid packet. char_index[%d] tab[%d] invenIndex[%d]",
				  ch->m_index, packet->tab, packet->invenIndex);
		ch->m_desc->Close("invalid packet");
		return;
	}

	// 2. 아이템 가져오기
	CItem * pItem = ch->m_inventory.getItem(packet->tab, packet->invenIndex);
	if (pItem == NULL)
	{
		LOG_ERROR("HACKING? : not found item. char_index[%d] tab[%d] invenIndex[%d]",
				  ch->m_index, packet->tab, packet->invenIndex);
		ch->m_desc->Close("not found item");
		return;
	}

	if (pItem->getDBIndex() != 2843)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ChaosBallMsg(rmsg, ERROR_ITEM_FAULT);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	if(pItem->Count() < 1)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ChaosBallMsg(rmsg, ERROR_ITEM_COUNT);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 보상 테이블
#if defined (LC_GAMIGO)
	const int item_count = 14;
	int reqord_item[item_count][7] =
	{
		{ 2844,  1,   0, 0,   0, 0,  20,  },	// 특급 제련석
		{ 974,   1,   0, 0,   0, 0,  50,  },	// 행운의 제련석
		{ 1416,  1,   0, 0,   0, 0,  290, },	// 플래티늄 아이리스의 축복
		{ 1288,  1,   0, 0,   0, 0,  290, },	// 아이리스의 축복
		{   85,  1,   0, 0,   0, 0,  500, },	// 고급 제련석
		{  971,  1, 973, 1, 972, 1,  700, },	// 경험의 스크롤, 노력의 스크롤, 행운의 스크롤
		{  882,  1, 883, 1, 884, 1,  800, },	// 경험치 증폭제, 숙련도 증폭제, 드롭율 증폭제
		{   45, 10,   0, 0,   0, 0, 1150, },	// 대형 체력회복 물약
		{  724, 10,   0, 0,   0, 0, 1000, },	// 대형 마나회복 물약
		{  498,  5, 500, 5,   0, 0,  900, },	// 12레벨 붉은케르, 12레벨 푸른케르
		{  499,  5, 501, 5,   0, 0,  900, },	// 12레벨 붉은 넨, 12레벨 푸른 넨
		{  786,  3,   0, 0,   0, 0,  900, },	// 부스터
		{  556, 10,   0, 0,   0, 0,  900, },	// 사탕
		{   19, 10000,0, 0,   0, 0, 1600, },	// 10,000나스
	};
#else
	const int item_count = 13;
	int reqord_item[item_count][7] =
	{
		{ 2844,  1,   0, 0,   0, 0,  50,  },	// 특급 제련석
		{ 1416,  1,   0, 0,   0, 0,  300, },	// 플래티늄 아이리스의 축복
		{ 1288,  1,   0, 0,   0, 0,  300, },	// 아이리스의 축복
		{   85,  1,   0, 0,   0, 0,  500, },	// 고급 제련석
		{  971,  1, 973, 1, 972, 1,  700, },	// 경험의 스크롤, 노력의 스크롤, 행운의 스크롤
		{  882,  1, 883, 1, 884, 1,  800, },	// 경험치 증폭제, 숙련도 증폭제, 드롭율 증폭제
		{   45, 10,   0, 0,   0, 0, 1150, },	// 대형 체력회복 물약
		{  724, 10,   0, 0,   0, 0, 1000, },	// 대형 마나회복 물약
		{  498,  5, 500, 5,   0, 0,  900, },	// 12레벨 붉은케르, 12레벨 푸른케르
		{  499,  5, 501, 5,   0, 0,  900, },	// 12레벨 붉은 넨, 12레벨 푸른 넨
		{  786,  3,   0, 0,   0, 0,  900, },	// 부스터
		{  556, 10,   0, 0,   0, 0,  900, },	// 사탕
		{   19, 10000,0, 0,   0, 0, 1600, },	// 10,000나스
	};
#endif // LC_GAMIGO

	int rate = GetRandom(0, 10000);
	int temp = 0, itemIdx = -1;
	int i;

	for(i = 0; i < item_count; i++)
	{
		temp += reqord_item[i][6];
		if(rate < temp)
		{
			itemIdx = i;
			break;
		}
	}

	if(itemIdx < 0)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		ChaosBallMsg(rmsg, ERROR_ITEM_COUNT);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	int flag = 0;	//
#if defined (LC_GAMIGO)
	if(i == 9 || i == 10)
	{
		// 붉은 케르와 푸른 넨 인 경우
		flag = 12;
	}
#else
	if(i == 8 || i == 9)
	{
		// 붉은 케르와 푸른 넨 인 경우
		flag = 12;
	}
#endif // LC_GAMIGO

	// 아이템 지급
	for(int j = 0; j < 6; j+=2)
	{
		if(reqord_item[itemIdx][j+1] != 0)
		{
			if(ch->GiveItem(reqord_item[i][j], 0, flag, reqord_item[i][j+1], false, false) == false)
			{
				{
					CNetMsg::SP rmsg(new CNetMsg);
					ChaosBallMsg(rmsg, ERROR_ITEM_FAULT);
					SEND_Q(rmsg, ch->m_desc);
				}

				// 롤백
				for( ; j > 0; j-=2)
				{
					CItem* item = ch->m_inventory.FindByVirtualIndex(reqord_item[i][j]);
					if(item)
					{
						ch->m_inventory.decreaseItemCount(item, reqord_item[i][j+1]);
					}
				}

				return;
			}

			GAMELOG << init("CHAOSBALL GIVE", ch) << reqord_item[i][j] << delim << reqord_item[i][j+1] << end;
		}
	}

	// 카오스 볼 제거
	ch->m_inventory.decreaseItemCount(pItem, 1);

	{
		// 카오스 볼 교환 성공 전송
		CNetMsg::SP rmsg(new CNetMsg);
		ChaosBallMsg(rmsg, ERROR_CHAOSBALL_SUCCESS);
		SEND_Q(rmsg, ch->m_desc);
	}
}
#endif

void do_EventPhoenix( CPC* ch, CNetMsg::SP& msg )
{
	int r, c;

	if (ch == NULL)
		return;

	CPC * tch = PCManager::instance()->getPlayerByCharIndex(ch->m_index);
	if ( tch == NULL )
		return;

#ifdef NPC_CHECK
	int npcIndex;
	CNPC *npc;
	RefMsg(msg) >> npcIndex;
	npc = ch->m_pArea->FindProtoNPCInCell(ch, npcIndex, false, 2);
	if(!npc)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		EventPhoenixMsg(rmsg, MSG_EVENT_PHOENIX_ERROR_ALREADY_APPLY);
		SEND_Q(rmsg, ch->m_desc);
		GAMELOG << init("Phoenix Event FAIL: NOT EXIST NPC (Dangerous User)", ch) << end;
		return ;
	}
	if( !npc->IsFlag(NPC_QUEST) && !npc->IsFlag(NPC_EVENT) )
	{
		GAMELOG << init("Phoenix Event FAIL: NOT EXIST NPC (Dangerous User)", ch) << end;
		return ;
	}
#endif

	// 이미 피닉스로 등록이 되어 있다.
	if ( ch->m_bPhoenix_Char == 1 )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		EventPhoenixMsg(rmsg, MSG_EVENT_PHOENIX_ERROR_ALREADY_APPLY);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 현재 캐릭터의 레벨이 31을 초과하여 피닉스 이벤트 자격이 안된다.
	else if ( ch->m_level >= 31 )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		EventPhoenixMsg(rmsg, MSG_EVENT_PHOENIX_ERROR_REQUIREMENT);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 인벤토리 가능 검사
	else if ( ch->m_inventory.getEmptyCount() < 12 )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		EventPhoenixMsg(rmsg, MSG_EVENT_PHOENIX_ERROR_FULL_INVENTORY);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}
	// 인벤토리에 피닉스 클럽 가입권이 존재하는 지 확인
	else if (ch->m_inventory.FindByDBIndex(3217, 0, 0) == NULL)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		EventPhoenixMsg(rmsg, MSG_EVENT_PHOENIX_ERROR_REQUIREMENT);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 모든 상황을 통과하였다면 캐릭터 DB로 검색해 보기 위해 HELPER SERVER로 패킷을 보낸다.
	else
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperEventPhoenixMsg( rmsg, ch->m_desc->m_index, ch->m_index );
		SEND_Q(rmsg, gserver->m_helper);
		return;
	}
}

void do_EventPhoenix_Success( CPC* ch, CNetMsg::SP& msg )
{
	const static int phoenixItem_table[6][10] =
	{
		{ 3218, 3262, 3006, 3221, 3222, 3231, 3232, 3233, 3234, 3235 },		// 타이탄
		{ 3218, 3262, 3007, 3219, 3220, 3236, 3237, 3238, 3239, 3240 },		// 나이트
		{ 3218, 3262, 3008, 3223, 3224, 3242, 3243, 3244, 3245, 3246 },		// 힐러
		{ 3218, 3262, 3009, 3225, 3226, 3247, 3248, 3249, 3250, 3251 },		// 메이지
		{ 3218, 3262, 3010, 3227, 3228, 3252, 3253, 3254, 3255, 3256 },		// 로그
		{ 3218, 3262, 3011, 3229, 3230, 3257, 3258, 3259, 3260, 3261 }
	};	// 소서러

	if ( ch == NULL )
		return;

	CPC * tch = PCManager::instance()->getPlayerByCharIndex(ch->m_index);
	if ( tch == NULL )
		return;

	CItem* pitem3217 = NULL;

	// 인벤토리 가능 검사
	if ( ch->m_inventory.getEmptyCount() < 12 )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		EventPhoenixMsg(rmsg, MSG_EVENT_PHOENIX_ERROR_FULL_INVENTORY);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}
	// 인벤토리에 피닉스 클럽 가입권이 존재하는 지 확인
	else if ((pitem3217 = ch->m_inventory.FindByDBIndex(3217, 0, 0)) == NULL)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		EventPhoenixMsg(rmsg, MSG_EVENT_PHOENIX_ERROR_REQUIREMENT);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	//유저의 직업을 찾음, 피닉스 날개를 주고 피닉스 무기와 착용아이템을 주기 위해서
	int job_index = 0;

	switch( ch->m_job )
	{
	case JOB_TITAN:
		job_index = 0;
		break;

	case JOB_KNIGHT:
		job_index = 1;
		break;

	case JOB_HEALER:
		job_index = 2;
		break;

	case JOB_MAGE:
#ifdef EX_MAGE
	case JOB_EX_MAGE:
#endif // EX_MAGE
		job_index = 3;
		break;

	case JOB_ROGUE:
#ifdef EX_ROGUE
	case JOB_EX_ROGUE:
#endif // EX_ROGUE
		job_index = 4;
		break;

	case JOB_SORCERER:
		job_index = 5;
		break;

	default:
		return;
		break;
	}

	// 피닉스 아이템 지급
	for ( int i = 0; i < 10; i++ )
	{
		CItem* pItem;

		if ( i < 3 )
		{
			pItem = gserver->m_itemProtoList.CreateItem(phoenixItem_table[job_index][i], -1, 0, 0, 1);
		}
		else
		{
			pItem = gserver->m_itemProtoList.CreateItem(phoenixItem_table[job_index][i], -1, 4, 0, 1);
		}

		if ( pItem == NULL )
			goto ITEM_FAIL;

		if (ch->m_inventory.addItem(pItem) == false)
		{
			delete pItem;
			pItem = NULL;
			goto ITEM_FAIL;
		}
	}

	// 직업이 기사이면 방패도 지급
	if ( ch->m_job == JOB_KNIGHT )
	{
		// 피닉스 방패
		CItem* pItem_shield = gserver->m_itemProtoList.CreateItem(3241, -1, 4, 0, 1);

		if ( pItem_shield == NULL )
			goto ITEM_FAIL;

		if (ch->m_inventory.addItem(pItem_shield) == false)
		{
			delete pItem_shield;

			goto ITEM_FAIL;
		}
	}

	// 버프 효과 보이기
	CSkill* pSkill1;
	pSkill1 = gserver->m_skillProtoList.Create( 516, 1 );
	if(pSkill1 != NULL)
	{
		bool bApply = false;
		ApplySkill(ch, ch, pSkill1, -1 , bApply);
		if(bApply == false)
		{
			GAMELOG << init("EVENT_PHOENIX SKILL FAILED ") << ch->m_name << end;// 스킬 적용 실패
		}
	}

	//레벨 업
	int remain_level;
	remain_level = 35 - ch->m_level;

	// 35까지 남은 레벨 지급,  경험치는 0으로 조정
	ch->m_exp = 0;
	int remain;
	remain = remain_level;
	while(remain != 0)
	{
#ifdef GER_LOG
		GAMELOGGEM << init( 0, "CHAR_LEVEL_UP")
				   << LOG_VAL("account-id", ch->m_desc->m_idname ) << blank
				   << LOG_VAL("character-id", ch->m_name ) << blank
				   << LOG_VAL("level", ch->m_level+1 ) << blank
				   << endGer;
#endif // GER_LOG
		GAMELOG << init("LEVEL UP", ch)
				<< ch->m_level
				<< " ==> "
				<< ch->m_level + 1
				<< end;

		{
			CNetMsg::SP rmsg(new CNetMsg);
			SubHelperLevelup(rmsg, ch->m_index);
			SEND_Q(rmsg, gserver->m_subHelper);
		}

		ch->m_level++;
		remain--;
		ch->LevelUp(true);

		{
			CNetMsg::SP rmsg(new CNetMsg);
			HelperPartyMatchMemberChangeInfoMsg(rmsg, ch->m_index, MSG_HELPER_PARTY_MATCH_MEMBER_CHANGE_INFO_LEVEL, "", ch->m_level, 0);
			SEND_Q(rmsg, gserver->m_helper);
		}
	}

	// statpoint +30 지급에 35 까지 남은 레벨에 대한 statpoint 지급
	ch->m_statpt_remain = ch->m_statpt_remain + 30 + remain_level;
	{
		CNetMsg::SP rmsg(new CNetMsg);
		StatPointRemainMsg(rmsg, ch);
		SEND_Q(rmsg, ch->m_desc);
	}

	// 숙련도는 4000에서 레벨에 따라 차등 지급
	ch->m_skillPoint += 6000000 + (1000000 * remain_level);

	// 케릭터 스텟 리프레쉬
	ch->InitStat();
	ch->CalcStatus(true);
	ch->Restore();

	// 같은 파티 원에게도 레벨업 보여주기
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperPartyMatchMemberChangeInfoMsg(rmsg, ch->m_index, MSG_HELPER_PARTY_MATCH_MEMBER_CHANGE_INFO_LEVEL, "", ch->m_level, 0);
		SEND_Q(rmsg, gserver->m_helper);
	}

	// 피닉스 클럽 가입권을 삭제한다.
	ch->m_inventory.decreaseItemCount(pitem3217, 1);

	// 캐릭터를 피닉스로 업데이트
	ch->m_bPhoenix_Char = 1;

	{
		CNetMsg::SP rmsg(new CNetMsg);
		EventPhoenix_SuccessMsg(rmsg);
		SEND_Q(rmsg, ch->m_desc);
	}
	return;

ITEM_FAIL:

	for ( int j = 0; j < 10; j++ )
	{
		if ( j < 3 )
		{
			CItem* item = ch->m_inventory.FindByDBIndex(phoenixItem_table[job_index][j], 0, 0);
			if (item)
			{
				ch->m_inventory.decreaseItemCount(item, 1);
			}
		}
		else
		{
			CItem* item = ch->m_inventory.FindByDBIndex(phoenixItem_table[job_index][j], 4, 0);
			if (item)
			{
				ch->m_inventory.decreaseItemCount(item, 1);
			}
		}
	} // for
}

#if defined(EVENT_WORLDCUP_2010) || defined(EVENT_WORLDCUP_2010_TOTO) || defined(EVENT_WORLDCUP_2010_TOTO_STATUS) || defined(EVENT_WORLDCUP_2010_TOTO_GIFT) || defined(EVENT_WORLDCUP_2010_ATTENDANCE) || defined(EVENT_WORLDCUP_2010_KOREA)
void do_EventWorldcup2010(CPC* ch, CNetMsg::SP& msg)
{
	unsigned char nSubType;
	RefMsg(msg) >> nSubType;

	switch( nSubType )
	{
#ifdef EVENT_WORLDCUP_2010
	case MSG_EVENT_WORLDCUP2010_TRADE_REQ: // 축구공 교환
		EventWordlcup2010Trade(ch, msg);
		break;
#endif
#if defined(EVENT_WORLDCUP_2010_TOTO)
	case MSG_EVENT_WORLDCUP2010_TOTO_REQ: // 승리국가 맞추기 TOTO 응모
		EventWordlcup2010TOTO(ch, msg);
		break;
#endif
#if defined(EVENT_WORLDCUP_2010_TOTO_STATUS) || defined(EVENT_WORLDCUP_2010_TOTO) || defined(EVENT_WORLDCUP_2010_TOTO_GIFT)
	case MSG_EVENT_WORLDCUP2010_TOTO_STATUS_REQ: // TOTO 응모 현황
		EventWordlcup2010TOTOStatus(ch, msg);
		break;
#endif
#if defined(EVENT_WORLDCUP_2010_TOTO_GIFT)
	case MSG_EVENT_WORLDCUP2010_TOTO_GIFT_REQ: // TOTO 상품 요청
		EventWordlcup2010TOTOGift(ch);
		break;
#endif
#if defined(EVENT_WORLDCUP_2010_ATTENDANCE)
	case MSG_EVENT_WORLDCUP2010_ATTENDANCE_REQ: // 출석 체크
		EventWordlcup2010Attendance(ch);
		break;

	case MSG_EVENT_WORLDCUP2010_SUPPORTCARD_REQ: // 웅원 카드 교환
		EventWordlcup2010Supportcard(ch, msg);
		break;
#endif
#if defined(EVENT_WORLDCUP_2010_KOREA)
	case MSG_EVENT_WORLDCUP2010_KOREA_REQ:
		EventWorldcup2010Korea(ch);
		break;
#endif
	}
}

void EventWordlcup2010Trade(CPC *ch, CNetMsg::SP& msg)
{
	int nTradeType = -1;
	RefMsg(msg) >> nTradeType;

	int ballindex = -1; // 교환할 축구공 item index
	int needCount = -1; // 교환에 필요한 축구공 개수
	int giftIndex = -1; // 교환 해줄 item index

	if( nTradeType == 1)		// 축구공 10개 -> 황금 축구공
	{
		ballindex = 5345;
		needCount = 10;
		giftIndex = 5346;
	}
	else if( nTradeType == 2 )       // 황금 축구공 20개 -> 월드 축제 상자
	{
		ballindex = 5346;
		needCount = 50;
		giftIndex = 5347;
	}
	else // 쓰레기 값이 들어왔으면 교환 무효 처리
	{
		CNetMsg::SP rmsg(new CNetMsg);
		EventWorldcup2010ErrorMsg(rmsg, MSG_EVENT_WORLDCUP2010_TRADE_REP, MSG_EVENT_WORLDCUP2010_TRADE_ERROR_FAIL);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	CItem * pGiftItem = NULL;
	CItem * pBallItem = NULL;
	int r, c;

	// 축구공이 인벤에 있는지 확인
	item_search_t vec;
	int sc = ch->m_inventory.searchItemByDBIndex(ballindex, vec);
	if( sc == 0 || sc < needCount ) // 개수 부족
	{
		CNetMsg::SP rmsg(new CNetMsg);
		EventWorldcup2010TradeErrorMsg(rmsg, MSG_EVENT_WORLDCUP2010_TRADE_ERROR_BALL, nTradeType);
		SEND_Q( rmsg, ch->m_desc );
		return ;
	}

	// 교환 아이템 생성
	pGiftItem = gserver->m_itemProtoList.CreateItem( giftIndex, -1, 0, 0, 1);
	if( pGiftItem == NULL )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		EventWorldcup2010ErrorMsg(rmsg, MSG_EVENT_WORLDCUP2010_TRADE_REP, MSG_EVENT_WORLDCUP2010_TRADE_ERROR_FAIL);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}
	// 일반 슬롯 체크
	if( ch->m_inventory.getEmptyCount() < 1)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		EventWorldcup2010ErrorMsg(rmsg, MSG_EVENT_WORLDCUP2010_TOTO_GIFT_REP, MSG_EVENT_WORLDCUP2010_TOTO_GIFT_ERROR_INVEN);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 축구공을 삭제하자.
	ch->m_inventory.deleteItem(vec, needCount);
	ch->m_inventory.addItem(pGiftItem);
	
	GAMELOG << init("EVENT_WORLDCUP_2010 : BALL TRADE SUC", ch) << itemlog( pGiftItem ) << end;

	{
		CNetMsg::SP rmsg(new CNetMsg);
		EventWorldcup2010TradeErrorMsg(rmsg, MSG_EVENT_WORLDCUP2010_TRADE_ERROR_SUC, nTradeType);
		SEND_Q(rmsg, ch->m_desc);
	}
}

void EventWordlcup2010TOTO(CPC *ch, CNetMsg::SP& msg)
{
	static int national_flag[32] = {1485,1486,1487,1488,1489,1490,1491,1492,1493,1496,1498,1499,1500,1501,1505,1506,1507,1508,1510,1513,1515,5332,5333,5334,5338,5342,5343,5344,10297,10298,10299,10300};

	int Countryitemindex;
	RefMsg(msg) >> Countryitemindex;

	// 국기가 아니면
	for(int i = 0; i < 32; i++)
	{
		if (national_flag[i] == Countryitemindex)
			break;
		else if(i == 31)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			EventWorldcup2010ErrorMsg(rmsg, MSG_EVENT_WORLDCUP2010_TOTO_REP, MSG_EVENT_WORLDCUP2010_TOTO_ERROR_FAIL);
			SEND_Q(rmsg, ch->m_desc);
			return;
		}
	}
	
	int r, c;

	CItem * pItem = ch->m_inventory.FindByDBIndex(Countryitemindex);
	if( pItem == NULL )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		EventWorldcup2010ErrorMsg(rmsg, MSG_EVENT_WORLDCUP2010_TOTO_REP, MSG_EVENT_WORLDCUP2010_TOTO_ERROR_FAIL);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}
	GAMELOG << init("EVENT_WORLDCUP_2010 : TOTO SUC", ch) << itemlog( pItem ) << end;

	ch->m_inventory.decreaseItemCount(pItem, 1);

	{
		// 삭제가 되었다면
		// DB에 기록 하자.
		// 이벤트 기간엔 무조건 갱신된다. 받는데로 기록하고 성공 메시지를 날리장.
		// 이제 헬퍼랑 놀아야 하나 ㅡㅡ;;
		CNetMsg::SP rmsg(new CNetMsg);
		HelperWorldcup2010TOTORepMsg(rmsg, ch->m_index, Countryitemindex );
		SEND_Q(rmsg, gserver->m_helper);
	}
}

void EventWordlcup2010TOTOStatus(CPC *ch, CNetMsg::SP& msg)
{
	// 헬퍼랑 놀아보자.
	int resultType = -1;
	RefMsg(msg) >> resultType;
	CNetMsg::SP rmsg(new CNetMsg);
	HelperWorldcup2010TOTOStatusRepMsg(rmsg, ch->m_index, resultType);
	SEND_Q(rmsg, gserver->m_helper);
}

void EventWordlcup2010TOTOGift(CPC *ch)
{
	// 헬퍼가 무지 바쁘겠다.
	CNetMsg::SP rmsg(new CNetMsg);
	HelperWorldcup2010TOTOGiftRepMsg(rmsg, ch->m_index);
	SEND_Q(rmsg, gserver->m_helper);
}

void EventWordlcup2010Attendance(CPC *ch)
{
	// 헬퍼야 미안해.
	CNetMsg::SP rmsg(new CNetMsg);
	HelperWorldcup2010TOTOGiftRepMsg(rmsg, ch->m_index);
	SEND_Q(rmsg, gserver->m_helper);
}

void EventWordlcup2010Supportcard(CPC *ch, CNetMsg::SP& msg)
{
	int nTradeType = -1;
	RefMsg(msg) >> nTradeType;

	nTradeType --;

	if( nTradeType <0 || nTradeType > 3 )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		EventWorldcup2010ErrorMsg(rmsg, MSG_EVENT_WORLDCUP2010_SUPPORTCARD_REP, MSG_EVENT_WORLDCUP2010_SUPPORTCARD_ERROR_FAIL);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// giftItemIndex, needCount
	int supportInfo[4][2] ={
		{ 43,  7},	// 16강 기원
		{ 44, 14},	// 8강 기원
		{ 45, 21},	// 4강 기원
		{491, 28}	// 라카컵
	};

	int cardIndex = 19; // 응원카드 인덱스

	CItem * pGiftItem = NULL;
	CItem * pCardItem = NULL;
	int r, c;

	// 카드가 인벤에 있는지 확인
	pCardItem = ch->m_inventory.FindByDBIndex(cardIndex);
	if(pCardItem != NULL)
	{
		if( pCardItem->Count() < supportInfo[nTradeType][1] ) // 카드는 있지만 갯수가 부족하다.
		{
			CNetMsg::SP rmsg(new CNetMsg);
			EventWorldcup2010ErrorMsg(rmsg, MSG_EVENT_WORLDCUP2010_SUPPORTCARD_REP, MSG_EVENT_WORLDCUP2010_SUPPORTCARD_ERROR_CARD);
			SEND_Q( rmsg, ch->m_desc );
			return ;
		}
	}
	else // 카드가 없다.
	{
		CNetMsg::SP rmsg(new CNetMsg);
		EventWorldcup2010ErrorMsg(rmsg, MSG_EVENT_WORLDCUP2010_SUPPORTCARD_REP, MSG_EVENT_WORLDCUP2010_SUPPORTCARD_ERROR_CARD);
		SEND_Q( rmsg, ch->m_desc );
		return ;
	}

	// 교환 아이템 생성
	pGiftItem = gserver->m_itemProtoList.CreateItem( supportInfo[nTradeType][0], -1, 0, 0, 1);
	if( pGiftItem == NULL )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		EventWorldcup2010ErrorMsg(rmsg, MSG_EVENT_WORLDCUP2010_SUPPORTCARD_REP, MSG_EVENT_WORLDCUP2010_SUPPORTCARD_ERROR_FAIL);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	ch->m_inventory.decreaseItemCount(pCardItem, 1);
	ch->m_inventory.addItem(pGiftItem);

	{
		CNetMsg::SP rmsg(new CNetMsg);
		EventWorldcup2010ErrorMsg(rmsg, MSG_EVENT_WORLDCUP2010_SUPPORTCARD_REP, MSG_EVENT_WORLDCUP2010_SUPPORTCARD_ERROR_SUC);
		SEND_Q(rmsg, ch->m_desc);
	}
}

void EventWorldcup2010Korea(CPC *ch)
{
	CNetMsg::SP rmsg(new CNetMsg);
	HelperWorldcup2010KoreaRepMsg(rmsg, ch->m_index, 0);	// 받은거 확인
	SEND_Q(rmsg, gserver->m_helper);
}
#endif // #if defined(EVENT_WORLDCUP_2010) || defined(EVENT_WORLDCUP_2010_TOTO_STATUS) || defined(EVENT_WORLDCUP_2010_TOTO_GIFT) || defined(EVENT_WORLDCUP_2010_ATTENDANCE) || defined(EVENT_WORLDCUP_2010_KOREA)

#ifdef DEV_EVENT_PROMOTION2
void do_EventPromotion2(CPC* pc, CNetMsg::SP& msg)
{
	unsigned char subType = -1;
	RefMsg(msg) >> subType;

	switch( subType )
	{
	case MSG_EVENT_PROMOTION2_COUPON_USE_REQ:
		{
			CLCString couponNum(25);
			couponNum = "";
			RefMsg(msg) >> couponNum;

			if( couponNum.Length() < 6 )
			{
				CNetMsg::SP rmsg(new CNetMsg);
				EventPromotion2RepErrorMsg(rmsg, MSG_EVENT_PROMOTION2_REP_ERROR_KEY );
				SEND_Q(rmsg, pc->m_desc);
				return;
			}

			if( gserver->isRunConnector() )
			{
				CNetMsg::SP rmsg(new CNetMsg);
				ConnPromotion2CouponUseReqMsg(rmsg, pc->m_desc->m_index, pc->m_index , couponNum );
				SEND_Q(rmsg, gserver->m_connector);
			}
			else
			{
				CNetMsg::SP rmsg(new CNetMsg);
				EventPromotion2RepErrorMsg(rmsg, MSG_EVENT_PROMOTION2_REP_ERROR_FAIL );
				SEND_Q(rmsg, pc->m_desc);
			}
		}
		break;

	default:
		{
			CNetMsg::SP rmsg(new CNetMsg);
			EventPromotion2RepErrorMsg(rmsg, MSG_EVENT_PROMOTION2_REP_ERROR_FAIL );
			SEND_Q(rmsg, pc->m_desc);
		}
		break;
	}
}
#endif

#ifdef EVENT_USER_COMEBACK
void do_EventUserComeback(CPC* ch, CNetMsg::SP& msg)
{
#ifdef NPC_CHECK
	int npcIndex;
	CNPC *npc;
	RefMsg(msg) >> npcIndex;
	npc = ch->m_pArea->FindProtoNPCInCell(ch, npcIndex, false, 2);
	if(!npc)
	{
		GAMELOG << init("STASH_KEEP FAIL: NOT EXIST NPC (Dangerous User)", ch) << end;
		return ;
	}
	if( !npc->IsFlag(NPC_EVENT) )
	{
		GAMELOG << init("STASH_KEEP FAIL: NOT EXIST NPC (Dangerous User)", ch) << end;
		return ;
	}
#endif
	if(ch->m_inventory.getEmptyCount() < 1 )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		SysFullInventoryMsg(rmsg, 0);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		ConnUserComebackGiftReqMsg(msg, ch->m_desc->m_index, ch->m_index);
		SEND_Q(msg, gserver->m_connector);
		return;
	}

	GAMELOG << init("EVENT_USER_COMEBACK : gserver->isRunConnector() FAIL", ch) << end;

	{
		CNetMsg::SP rmsg(new CNetMsg);
		EventUserComebackErrorMsg(rmsg, MSG_EVENT_USER_COMEBACK_ERROR_NOT_USER );
		SEND_Q(rmsg, ch->m_desc);
	}
}
#endif

#ifdef EVENT_CHAR_BIRTHDAY
void do_EventCharBirthday(CPC* ch, CNetMsg::SP& msg)
{
	unsigned char  subtype;

#ifdef NPC_CHECK
	int npcIndex;
	CNPC *npc;
	RefMsg(msg) >> subtype
				>> npcIndex;
	npc = ch->m_pArea->FindProtoNPCInCell(ch, npcIndex, false, 2);
	if(!npc)
	{
		GAMELOG << init("EventCharBirthday FAIL: NOT EXIST NPC (Dangerous User)", ch) << end;
		return ;
	}
	if( !npc->IsFlag(NPC_EVENT) )
	{
		GAMELOG << init("EventCharBirthday FAIL: NOT EXIST NPC (Dangerous User)", ch) << end;
		return ;
	}
#else
	RefMsg(msg) >> subtype;
#endif

	switch( (int)subtype )
	{
	case MSG_EVENT_CHAR_BIRTHDAY_GIFT_REQ:
		{
			CNetMsg::SP rmsg(new CNetMsg);
			if(ch->m_inventory.getEmptyCount() < 1 )
			{
				SysFullInventoryMsg(rmsg, 0);
				SEND_Q(rmsg, ch->m_desc);
				return;
			}

			{
				HelperCharBirthdayReqMsg(rmsg, (MSG_EVENT_CHAR_BIRTHDAY_TYPE)subtype, ch->m_index);
				SEND_Q(rmsg, gserver->m_helper);
				return;
			}

			EventCharBirthdayErrorMsg(rmsg, MSG_EVENT_CHAR_BIRTHDAY_ERROR_FAIL );
			SEND_Q( rmsg, ch->m_desc);
		}
		break;

	case MSG_EVENT_CHAR_BIRTHDAY_BDAY_REQ:
		{
			{
				CNetMsg::SP rmsg(new CNetMsg);
				HelperCharBirthdayReqMsg(rmsg, (MSG_EVENT_CHAR_BIRTHDAY_TYPE)subtype, ch->m_index);
				SEND_Q(rmsg, gserver->m_helper);
				return;
			}

			{
				CNetMsg::SP rmsg(new CNetMsg);
				rmsg->Init(MSG_EVENT);
				RefMsg(rmsg) << (unsigned char) MSG_EVENT_CHAR_BIRTHDAY
							 << (unsigned char) MSG_EVENT_CHAR_BIRTHDAY_BDAY_REP
							 << 0;
				SEND_Q(rmsg, ch->m_desc);
			}
		}
		break;
	}
}
#endif

#ifdef EVENT_EUR2_PROMOTION
void do_EventEUR2Promotion(CPC* ch, CNetMsg::SP& msg)
{
	if(ch == NULL)
		return ;

	const int nItemList[7][5] =
	{
		{6531, 6532, 6533, 6534, 6535},		// 0: TITAN ITEM
		{6536, 6537, 6538, 6539, 6540},		// 1: KNIGHT ITEM
		{6541, 6542, 6543, 6544, 6545},		// 2: HEALER ITEM
		{6546, 6547, 6548, 6549, 6550},		// 3: MAGE ITEM
		{6551, 6552, 6553, 6554, 6555},		// 4: ROGUE ITEM
		{6556, 6557, 6558, 6559, 6560},		// 5: SORCERER ITEM
		{6561, 6562, 6563, 6564, 6565},		// 6: NIGHTSHADOW ITEM
	};
	CLCString key(21);
	unsigned char bInvenSpace = 1;	// true
	RefMsg(msg) >> key;
	// 위의 key값을 Connector로 보낸다. 근데 여기서 한방에 끝내자.

	if(key.Length() != 20)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		EventEUR2PromotionMsg(rmsg, (MSG_EVENT_EUR2_PROMOTION_ERROR_TYPE)MSG_EVENT_EUR2_PROMOTION_WRONG_KEY);
		SEND_Q(rmsg, ch->m_desc);
		return ;
	}

	if(ch->m_inventory.getEmptyCount() < 5)
	{
		bInvenSpace = 0;		// false 인벤 꽉차서 줄 수 없음.일단 구해놓기.
	}

	{
		// 이제 key 값과, charindex, userindex, inven공간을 메시징 해서 보낸다.
		CNetMsg::SP rmsg(new CNetMsg);
		ConnEUR2PromotionMsg(rmsg, ch->m_index, ch->m_desc->m_index, key, bInvenSpace, 1);
		SEND_Q(rmsg, gserver->m_connector);
	}
}
#endif

#ifdef DEV_EVENT_AUTO
void do_EventKBReward(CPC* ch, CNetMsg::SP& msg)
{
	if(!gserver->m_fathersDay.getEventStart())
		return ;
	if(!ch)
		return;
	unsigned char command;
	RefMsg(msg) >> command;
	CEventKingsBirthdayDropItemManager& itemManager = gserver->m_fathersDay.getDropItemManager();
	CEventKingsBirthdayRewardItem& rewardItem = gserver->m_fathersDay.getHeartRewardItem();
	CEventKingsBirthdayFlagReward& flagRewardItem = gserver->m_fathersDay.getFlagRewardItem();
	switch((int)command)
	{
	case 0:	// heart 교환 요청
		{
			if(ch->m_inventory.getEmptyCount() < 1)
			{
				CNetMsg::SP rmsg(new CNetMsg);
				SysFullInventoryMsg(rmsg, 0);
				SEND_Q(rmsg, ch->m_desc);
				return ;
			}

			int allPoint = itemManager.calcPoint(ch);
			if(allPoint == 0)
			{
				// TODO : 하트가 없습니다. 확인 후 다시 시도하여 주십시오.
				CNetMsg::SP rmsg(new CNetMsg);
				rmsg->Init(MSG_EVENT);
				RefMsg(rmsg) << (unsigned char)MSG_EVENT_ERROR
							 << (unsigned char)MSG_EVENT_ERROR_KB_NO_ITEM
							 << (unsigned char) 0;
				SEND_Q(rmsg, ch->m_desc);
				return ;
			}
			itemManager.deleteDropItem(ch);
			rewardItem.giveRewardItem(ch, allPoint);
		}
		break;
	case 1: // flag 교환 요청
		{
			if(ch->m_inventory.getEmptyCount() < 1)
			{
				CNetMsg::SP rmsg(new CNetMsg);
				SysFullInventoryMsg(rmsg, 0);
				SEND_Q(rmsg, ch->m_desc);
				return ;
			}
			int i;
			bool bFind = false;

			int dropItemCount = itemManager.getDropItemCount();

			for(i = 0; i < dropItemCount; i++)
			{
				if(itemManager.getDropItem(i) && strcmp(itemManager.getDropItem(i)->getType(), "Flag") == 0)
				{
					bFind = true;
					break;
				}
			}
			if(bFind == false)
				return ;

			if (ch->m_inventory.FindByDBIndex(itemManager.getDropItem(i)->getDropItemIndex(), 0, 0) == NULL)
			{
				// TODO : Father Flag 아이템이 없습니다. 확인 후 다시 시도하여 주십시오.
				CNetMsg::SP rmsg(new CNetMsg);
				rmsg->Init(MSG_EVENT);
				RefMsg(rmsg) << (unsigned char)MSG_EVENT_ERROR
							 << (unsigned char)MSG_EVENT_ERROR_KB_NO_ITEM
							 << (unsigned char) 1;
				SEND_Q(rmsg, ch->m_desc);
				return ;
			}
			itemManager.deleteFlagItem(ch);
			flagRewardItem.giveFlagRewardItem(ch);
		}
		break;
	default:
		break;
	}
}
#endif
//
