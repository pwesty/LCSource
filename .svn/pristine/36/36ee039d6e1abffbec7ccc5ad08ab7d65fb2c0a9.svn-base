#include "stdhdrs.h"

#include "Log.h"
#include "Character.h"
#include "Server.h"
#include "CmdMsg.h"
#include "doFunc.h"
#include "../ShareLib/packetType/ptype_old_do_friend.h"

void do_Teach(CPC* ch, CNetMsg::SP& msg)
{
#ifdef HARDCORE_SERVER
	if (gserver->m_hardcore_flag_in_gameserver)
	{
		LOG_ERROR("HACKING? : This server-group is hardcore. charIndex[%d]", ch->m_index);
		ch->m_desc->Close("This server-group is hardcore");
		return;
	}
#endif

	if (DEAD(ch))	return;

	unsigned char subtype;

	msg->MoveFirst();

	RefMsg(msg) >> subtype;

	switch (subtype)
	{
	case MSG_TEACH_TEACHER_LIST:
		do_TeachTeacherList(ch, msg);
		break;

	case MSG_TEACH_TEACHER_REQ:
		{
			unsigned char type;
			RefMsg(msg) >> type;

			switch (type)
			{
			case MSG_TEACH_TEACHER_REQ_REQ:
				do_TeachTeacherReqReq(ch, msg);
				break;

			case MSG_TEACH_TEACHER_REQ_REJECT:
				do_TeachTeacherReqReject(ch, msg);
				break;

			case MSG_TEACH_TEACHER_REQ_ACCEPT:
				do_TeachTeacherReqAccept(ch, msg);
				break;

			case MSG_TEACH_TEACHER_GIVEUP:
				do_TeachTeacherGiveUP(ch, msg);
				break;

			default:
				break;
			}
		}
		break;

	case MSG_TEACH_INFO:
		do_TeachInfo(ch, msg);
		break;

	case MSG_TEACH_RENEWER_TEACH_GIFT:
		do_TeacherGift(ch);
		break;

	default:
		break;
	}
}

void do_TeachTeacherList(CPC* ch, CNetMsg::SP& msg)
{
	unsigned char subtype;
	RefMsg(msg) >> subtype;

	switch (subtype)
	{
	case MSG_TEACH_TEACHER_LIST_UP:

		// 레벨 조건 미달
		if (ch->m_level < TEACH_LEVEL_TEACHER)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			TeachErrMsg(rmsg, MSG_TEACH_ERR_TEACHER_LEVEL);
			SEND_Q(rmsg, ch->m_desc);
			return;
		}

		// 이미 선생리스트에 있음
		if (ch->m_bTeacher)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			TeachErrMsg(rmsg, MSG_TEACH_ERR_TEACHER_LIST_ALREADY);
			SEND_Q(rmsg, ch->m_desc);
			return;
		}

		ch->m_bTeacher = true;

		{
			CNetMsg::SP rmsg(new CNetMsg);
			TeachTeacherListMsg(rmsg, ch, MSG_TEACH_TEACHER_LIST_UP, NULL);
			SEND_Q(rmsg, ch->m_desc);
		}

		{
			unsigned char noticetime1, noticetime2;
			RefMsg(msg) >> noticetime1
						>> noticetime2;

			ch->m_teachTime[0] = noticetime1;
			ch->m_teachTime[1] = noticetime2;

			CNetMsg::SP rmsg(new CNetMsg);
			HelperTeachMsg(rmsg, ch->m_index, ch->m_bTeacher, noticetime1, noticetime2);
			SEND_Q(rmsg, gserver->m_helper);
		}
		break;

	case MSG_TEACH_TEACHER_LIST_DN:

		// 레벨 조건 미달
		if (ch->m_level < TEACH_LEVEL_TEACHER)
			return;

		// 선생리스트에 있지 않음
		if (!ch->m_bTeacher)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			TeachErrMsg(rmsg, MSG_TEACH_ERR_TEACHER_LIST_NOT);
			SEND_Q(rmsg, ch->m_desc);
			return;
		}

		ch->m_bTeacher = false;

		{
			CNetMsg::SP rmsg(new CNetMsg);
			TeachTeacherListMsg(rmsg, ch, MSG_TEACH_TEACHER_LIST_DN, NULL);
			SEND_Q(rmsg, ch->m_desc);
		}

		{
			CNetMsg::SP rmsg(new CNetMsg);
			HelperTeachMsg(rmsg, ch->m_index, ch->m_bTeacher);
			SEND_Q(rmsg, gserver->m_helper);
		}

		break;

	case MSG_TEACH_TEACHER_LIST_SHOW:
		{
			// 이미 신청중이다
			if (ch->m_teachWait != -1)
			{
				CNetMsg::SP rmsg(new CNetMsg);
				TeachErrMsg(rmsg, MSG_TEACH_ERR_TEACHER_LIST_NOT);
				SEND_Q(rmsg, ch->m_desc);
				return;
			}

			// 선생이 있으면 이 msg를 보낼 수 없다
			if (ch->m_teachIdx[0] != -1)
				return;

			// 선생이 있으면 이 msg를 보낼 수 없다
			if (ch->m_teachType == MSG_TEACH_STUDENT_TYPE)
				return;

			if (ch->m_teachType == MSG_TEACH_NO_STUDENT_TYPE)
			{
				CNetMsg::SP rmsg(new CNetMsg);
				TeachErrMsg(rmsg, MSG_TEACH_ERR_NOMORE_STUDENT);
				SEND_Q(rmsg, ch->m_desc);
				return;
			}

			// 제자 가능 레벨이 아님
			if (ch->m_level > TEACH_LEVEL_STUDENT)
			{
				CNetMsg::SP rmsg(new CNetMsg);
				TeachErrMsg(rmsg, MSG_TEACH_ERR_STUDENT_LEVEL);
				SEND_Q(rmsg, ch->m_desc);
				return;
			}

			int playerCount = PCManager::instance()->getPlayerCount();
			int* tlist = new int[PCManager::instance()->getPlayerCount()];
			memset(tlist, -1, sizeof(int) * playerCount);
			int cnt = 0;

			PCManager::map_t& playerMap = PCManager::instance()->getPlayerMap();
			PCManager::map_t::iterator iter		= playerMap.begin();
			PCManager::map_t::iterator endIter	= playerMap.end();
			for (; iter != endIter ; ++iter)
			{
				CPC* pc = (*iter).pPlayer;
				if (pc && pc->m_bTeacher)
				{
					tlist[cnt] = pc->m_index;
					cnt++;
				}
			}


			CPC* list[20];
			memset(list, 0, sizeof(CPC*) * 20);

			int idx;
			for (int i = 0; i < 20; i++)
			{
				idx = GetRandom(0, cnt - 1);
				int j, k;
				for (j = 0; j < cnt; j++)
				{
					k = (idx + j) % cnt;

					if (tlist[k] != -1)
					{
						list[i] = PCManager::instance()->getPlayerByCharIndex(tlist[k]);
						tlist[k] = -1;
						break;
					}
				}
				if (j == cnt)
					break;
			}

			{
				CNetMsg::SP rmsg(new CNetMsg);
				TeachTeacherListMsg(rmsg, ch, MSG_TEACH_TEACHER_LIST_SHOW, list);
				SEND_Q(rmsg, ch->m_desc);
			}

			delete [] tlist;
		}

		break;

	default:
		break;
	}
}

void do_TeachTeacherReqReq(CPC* ch, CNetMsg::SP& msg)
{
	int charindex;

	RefMsg(msg) >> charindex;

	// 레벨 초과
	if (ch->m_level > TEACH_LEVEL_STUDENT)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		TeachErrMsg(rmsg, MSG_TEACH_ERR_STUDENT_LEVEL);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 이미 선생 있음
	if (ch->m_teachIdx[0] != -1)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		TeachErrMsg(rmsg, MSG_TEACH_ERR_HAVE_TEACHER);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 이미 신청대기중인 선생이 있음
	if (ch->m_teachWait != -1)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		TeachErrMsg(rmsg, MSG_TEACH_ERR_TEACHER_REQ_ALREADY);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	if (ch->m_teachType == MSG_TEACH_NO_STUDENT_TYPE) // 사제 관계 맺었다가 실패한 경우
	{
		CNetMsg::SP rmsg(new CNetMsg);
		TeachErrMsg(rmsg, MSG_TEACH_ERR_NOMORE_STUDENT);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 선생이 없음
	CPC* tch = PCManager::instance()->getPlayerByCharIndex(charindex);

	if (!tch)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		TeachErrMsg(rmsg, MSG_TEACH_ERR_TEACHER_NOT_CONNECT);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 선생이 선생리스트에 없음
	if (!tch->m_bTeacher)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		TeachErrMsg(rmsg, MSG_TEACH_ERR_TEACHER_LIST_NOT);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 이미 대기중인 학생이 있음
	if (tch->m_teachWait != -1)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		TeachErrMsg(rmsg, MSG_TEACH_ERR_TEACHER_REQ_ALREADY);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	bool bTeacher = false;

	int i;
	for (i=0; i < TEACH_MAX_STUDENTS; i++)
	{
		if (tch->m_teachIdx[i] == -1)
		{
			bTeacher = true;
			break;
		}
	}

	// 선생이 제자가 꽉 차 있음
	if (!bTeacher)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		TeachErrMsg(rmsg, MSG_TEACH_ERR_TEACHER_FULL);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 대기자 셋팅
	tch->m_teachWait = ch->m_index;
	ch->m_teachWait = tch->m_index;

	{
		// 선생에게 요청
		CNetMsg::SP rmsg(new CNetMsg);
		TeachTeacherReqReqMsg(rmsg, ch);
		SEND_Q(rmsg, tch->m_desc);
	}
}

void do_TeachTeacherReqReject(CPC* ch, CNetMsg::SP& msg)
{
	// 대기명단이 없음
	if (ch->m_teachWait == -1)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		TeachErrMsg(rmsg, MSG_TEACH_ERR_NOE_REQ);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	CPC* tch = PCManager::instance()->getPlayerByCharIndex(ch->m_teachWait);
	if (!tch)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		TeachErrMsg(rmsg, MSG_TEACH_ERR_SYSTEM);
		SEND_Q(rmsg, ch->m_desc);
		ch->m_teachWait = -1;
		return;
	}

	// 대기 명단이 없거나 같지 않거나
	if (tch->m_teachWait == -1 || tch->m_teachWait != ch->m_index)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		TeachErrMsg(rmsg, MSG_TEACH_ERR_NOE_REQ);
		SEND_Q(rmsg, ch->m_desc);
		ch->m_teachWait = -1;
		return;
	}

	ch->m_teachWait = -1;
	tch->m_teachWait = -1;

	CNetMsg::SP rmsg(new CNetMsg);
	TeachTeacherReqRejectMsg(rmsg, ch);
	SEND_Q(rmsg, ch->m_desc);
	SEND_Q(rmsg, tch->m_desc);
}

void do_TeachTeacherReqAccept(CPC* ch, CNetMsg::SP& msg)
{
	// 대기 학생이 없음
	if (ch->m_teachWait == -1)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		TeachErrMsg(rmsg, MSG_TEACH_ERR_NOE_REQ);
		SEND_Q(rmsg, ch->m_desc);
		return;
	}

	// 선생리스트에 등록되어 있지 않음
	if (!ch->m_bTeacher)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		TeachErrMsg(rmsg, MSG_TEACH_ERR_TEACHER_LIST_NOT);
		SEND_Q(rmsg, ch->m_desc);
		ch->m_teachWait = -1;
		return;
	}

	CPC* tch = PCManager::instance()->getPlayerByCharIndex(ch->m_teachWait);
	if (!tch)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		TeachErrMsg(rmsg, MSG_TEACH_ERR_STUDENT_NOT_CONNECT);
		SEND_Q(rmsg, ch->m_desc);
		ch->m_teachWait = -1;
		return;
	}

	// 대기 선생이 없거나 같지 않거나
	if (tch->m_teachWait == -1 || tch->m_teachWait != ch->m_index)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		TeachErrMsg(rmsg, MSG_TEACH_ERR_NOE_REQ);
		SEND_Q(rmsg, ch->m_desc);
		ch->m_teachWait = -1;
		return;
	}

	if (tch->m_teachType == MSG_TEACH_NO_STUDENT_TYPE)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		TeachErrMsg(rmsg, MSG_TEACH_ERR_NOMORE_STUDENT);
		SEND_Q(rmsg, ch->m_desc);
		ch->m_teachWait = -1;
		tch->m_teachWait = -1;
		return;
	}

	bool bTeacher = false;

	int i;
	for (i=0; i < TEACH_MAX_STUDENTS; i++)
	{
		if (ch->m_teachIdx[i] == -1)
		{
			bTeacher = true;
			break;
		}
	}

	// 선생이 제자가 꽉 차 있음
	if (!bTeacher)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		TeachErrMsg(rmsg, MSG_TEACH_ERR_TEACHER_FULL);
		SEND_Q(rmsg, tch->m_desc);
		SEND_Q(rmsg, ch->m_desc);
		ch->m_teachWait = -1;
		tch->m_teachWait = -1;
		return;
	}

	ch->m_teachIdx[i] = tch->m_index;
	ch->m_teachJob[i] = tch->m_job;
	ch->m_teachJob2[i] = tch->m_job2;
	ch->m_teachLevel[i] = tch->m_level;
	strcpy(ch->m_teachName[i], tch->GetName() );
	tch->m_teachIdx[0] = ch->m_index;
	tch->m_teachJob[0] = ch->m_job;
	tch->m_teachJob2[0] = ch->m_job2;
	tch->m_teachLevel[0] = ch->m_level;
	strcpy(tch->m_teachName[0], ch->GetName());

	// 선생 리스트 검사해서 꽉 찾으면 후보에서 내림
	for (i=0; i < TEACH_MAX_STUDENTS; i++)
	{
		if (ch->m_teachIdx[i] == -1)
			break;
	}

	// 후보에서 제외
	if (i == TEACH_MAX_STUDENTS)
	{
		ch->m_bTeacher = false;
	}

	ch->m_teachWait = -1;
	ch->m_teachType = MSG_TEACH_TEACHER_TYPE;
	ch->m_cntTeachingStudent++;

	tch->m_teachWait = -1;
	tch->m_teachType = MSG_TEACH_STUDENT_TYPE;

	{
		// msg
		CNetMsg::SP rmsg(new CNetMsg);
		TeachTeacherReqAcceptMsg(rmsg, ch, tch);
		SEND_Q(rmsg, ch->m_desc);
		SEND_Q(rmsg, tch->m_desc);
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		TeachInfoMsg(rmsg, ch);
		SEND_Q(rmsg, ch->m_desc);
	}

	{
		CNetMsg::SP rmsg(new CNetMsg);
		TeachInfoMsg(rmsg, tch);
		SEND_Q(rmsg, tch->m_desc);
	}

	// 헬퍼에 등록 요청
	// teachidx bteacher studentidx
	if(gserver->isRunHelper())
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperTeachRegisterMsg(rmsg, ch->m_index, ch->m_bTeacher, tch->m_index);
		SEND_Q(rmsg, gserver->m_helper);
	}
	else
	{
		GAMELOG << init("TEACH_START_FAIL-not helper", tch)
				<< ch->m_name << delim
				<< ch->m_nick << delim
				<< ch->m_desc->m_idname << delim
				<< ch->m_fame << end;
		return;
	}

	GAMELOG << init("TEACH_START", tch)
			<< ch->m_name << delim
			<< ch->m_nick << delim
			<< ch->m_desc->m_idname << delim
			<< ch->m_fame << end;

	// 둘중에 한명이라도 친구 리스트가 꽉찼다면 친구 등록하지 말자.
	if(ch->m_Friend->GetFriendSum() < FRIEND_MAX_MEMBER && tch->m_Friend->GetFriendSum() < FRIEND_MAX_MEMBER )
	{
		// 메신저에 서로 등록이 되어 있는지 확인한다.
		CFriendMember* pFriendMem = ch->m_Friend->FindFriendMember(tch->m_index);
		if(pFriendMem == NULL)
		{
			CNetMsg::SP rmsg(new CNetMsg);
			UpdateServer::HelperFriendAddReqMsg(rmsg, ch->m_index, ch->GetName(), ch->m_job, tch->m_index, tch->GetName(), tch->m_job);
			SEND_Q(rmsg, gserver->m_helper);
		}
	}

	// 여기서 라카입문4 퀘스트 완료 해주자 (후견인과 관계(니가 말한 관계가 불륜관계는 아니겠지?)를 맺었다)
	CQuest* pQuest = NULL;
	CQuest* pQuestNext = tch->m_questList.GetNextQuest(NULL, QUEST_STATE_RUN);
	while(( pQuest = pQuestNext))
	{
		pQuestNext = tch->m_questList.GetNextQuest(pQuestNext, QUEST_STATE_RUN);
		if( pQuest->GetQuestProto()->m_index == 370 ) // 라카 입문4 퀘스트
		{
			pQuest->SetComplete1(true);
			pQuest->SetComplete2(true);

			CNetMsg::SP rmsg(new CNetMsg);
			QuestCompleteMsg(rmsg, pQuest);
			SEND_Q(rmsg, tch->m_desc);
		}
	}
}

void do_TeachInfo(CPC* ch, CNetMsg::SP& msg)
{
	if (ch->m_teachType == MSG_TEACH_NO_TYPE)
		return;

	CNetMsg::SP rmsg(new CNetMsg);
	TeachInfoMsg(rmsg, ch);
	SEND_Q(rmsg, ch->m_desc);
}
void do_TeacherGift(CPC* ch)
{
	if( gserver->isRunHelper() )
	{
		CNetMsg::SP rmsg(new CNetMsg);
		HelperTeacherGiftReqMsg(rmsg, ch->m_index);
		SEND_Q(rmsg, gserver->m_helper);
	}
	else
	{
		CNetMsg::SP rmsg(new CNetMsg);
		TeachTeacherGiftMsg(rmsg, MSG_TEACH_GIFT_ERROR, ch);
		SEND_Q(rmsg, ch->m_desc);
	}
}

void do_TeachTeacherGiveUP(CPC* ch, CNetMsg::SP& msg)
{
	int charindex;
	RefMsg(msg) >> charindex;

	switch( ch->m_teachType )
	{
	case MSG_TEACH_NO_TYPE:
	case MSG_TEACH_NO_STUDENT_TYPE:
		{
			CNetMsg::SP rmsg(new CNetMsg);
			TeachErrMsg(rmsg, MSG_TEACH_ERR_SYSTEM);
			SEND_Q(rmsg, ch->m_desc);
		}
		return;

	case MSG_TEACH_TEACHER_TYPE: // 후견인이면
		{
			// 명성치 5이상 있어야 괜계를 끊을 수 있는 더러운 세상
			if( ch->m_fame < 5 )
			{
				CNetMsg::SP rmsg(new CNetMsg);
				TeachErrMsg(rmsg, MSG_TEACH_ERR_FAME);
				SEND_Q(rmsg, ch->m_desc);
				return;
			}

			// 이 학생이 내학생 맞냐?
			bool blist = false;
			int i=0;
			for(i=0; i<TEACH_MAX_STUDENTS; i++)
			{
				if( ch->m_teachIdx[i] == charindex)
					blist = true;
			}

			// 내 견습생이 아니당.
			if( !blist )
			{
				CNetMsg::SP rmsg(new CNetMsg);
				TeachErrMsg(rmsg, MSG_TEACH_ERR_SYSTEM);
				SEND_Q(rmsg, ch->m_desc);
				return;
			}

			CPC* tch = PCManager::instance()->getPlayerByCharIndex(charindex);

			// 견습생이 접속중이지 않음
			if (!tch || tch->m_bPlaying == false)
			{
				CNetMsg::SP rmsg(new CNetMsg);
				TeachErrMsg(rmsg, MSG_TEACH_ERR_STUDENT_NOT_CONNECT);
				SEND_Q(rmsg, ch->m_desc);
				return;
			}
			ch->m_cntFailStudent++;
			ch->m_cntTeachingStudent--;
			ch->m_fame -= 5;

			{
				// 후견인과 견습생 모두에게 메시지를 보낸당.
				CNetMsg::SP rmsg(new CNetMsg);
				TeachTeacherGiveUPMsg(rmsg, ch, tch, -1, ch->m_fame );
				SEND_Q(rmsg, ch->m_desc);
				SEND_Q(rmsg, tch->m_desc);
			}

			{
				CNetMsg::SP rmsg(new CNetMsg);
				StatusMsg(rmsg, ch);
				SEND_Q(rmsg, ch->m_desc);
			}

			//학생 리스트에서 삭제
			do_TeachSetCancel(ch, tch);

			if(gserver->isRunHelper())
			{
				CNetMsg::SP rmsg(new CNetMsg);
				HelperTeacherGiveup(rmsg, ch->m_index, tch->m_index, tch->m_teachType, tch->m_teachTime[0]);
				SEND_Q(rmsg, gserver->m_helper);
			}
			else
			{
				GAMELOG << init("TEACH_GIVEUP_TEACHER-not-helper", tch)
						<< ch->m_name << delim
						<< ch->m_nick << delim
						<< ch->m_desc->m_idname << delim
						<< ch->m_fame << delim
						<< tch->m_name << delim
						<< tch->m_nick << delim
						<< tch->m_desc->m_idname << delim
						<< tch->m_teachTime[0] << delim // 포기 횟수
						<< end;
				return;
			}

			GAMELOG << init("TEACH_GIVEUP_TEACHER", tch)
					<< ch->m_name << delim
					<< ch->m_nick << delim
					<< ch->m_desc->m_idname << delim
					<< ch->m_fame << delim
					<< tch->m_name << delim
					<< tch->m_nick << delim
					<< tch->m_desc->m_idname << delim
					<< tch->m_teachTime[0] << delim // 포기 횟수
					<< end;
		}
		break;
	case MSG_TEACH_STUDENT_TYPE: // 견습생이면
		{
			// 후견인이 없다.
			if (ch->m_teachIdx[0] == -1)
			{
				CNetMsg::SP rmsg(new CNetMsg);
				TeachErrMsg(rmsg, MSG_TEACH_ERR_SYSTEM);
				SEND_Q(rmsg, ch->m_desc);
				return;
			}

			// 후견인을 찾자
			CPC* tch = PCManager::instance()->getPlayerByCharIndex(charindex);

			// 후견인이 접속중이 아님
//			if (!tch)
//			{
//				TeachErrMsg(rmsg, MSG_TEACH_ERR_TEACHER_NOT_CONNECT);
//				SEND_Q(rmsg, ch->m_desc);
//				return;
//			}

			if( tch )
			{
				tch->m_cntFailStudent++;
				tch->m_cntTeachingStudent --;
				if( tch->m_cntTeachingStudent < 0)
					tch->m_cntTeachingStudent = 0;
			}

			int oldteachIndex = ch->m_teachIdx[0];
			// 셋팅해제
			do_TeachSetCancel(tch, ch);
			ch->m_teachIdx[0] = -1;
			ch->m_teachTime[0] ++;	// 후견인 관계를 3회까지 허용.
			if(ch->m_teachTime[0] >= TEACH_STUDENT_MAX_GIVEUP )
				ch->m_teachType = MSG_TEACH_NO_STUDENT_TYPE; // 더이상 견습생 못함
			else
				ch->m_teachType = MSG_TEACH_NO_TYPE;

			{
				CNetMsg::SP rmsg(new CNetMsg);
				if( tch )
					TeachTeacherGiveUPMsg(rmsg, tch, ch, ch->m_teachTime[0]);
				else
					TeachTeacherGiveUPMsg(rmsg, NULL, ch, ch->m_teachTime[0]);

				SEND_Q(rmsg, ch->m_desc);
			}

			if( tch )
			{
				CNetMsg::SP rmsg(new CNetMsg);
				TeachTeacherGiveUPMsg(rmsg, tch, ch, -1);
				SEND_Q(rmsg, tch->m_desc);
			}

			if(gserver->isRunHelper())
			{
				{
					CNetMsg::SP rmsg(new CNetMsg);
					HelperTeacherGiveup(rmsg, oldteachIndex, ch->m_index, ch->m_teachType, ch->m_teachTime[0]);
					SEND_Q(rmsg, gserver->m_helper);
				}

				if( !tch )
				{
					CNetMsg::SP rmsg(new CNetMsg);
					HelperStuentListRefresh( rmsg, charindex ); // 리스트를 갱신하도록 메시지를 보낸다
					SEND_Q(rmsg, gserver->m_helper);
				}
			}
			else
			{
				if( !tch )
				{
					GAMELOG << init("TEACH_GIVEUP_STUDENT-not-helper")
							<< ch->m_teachIdx[0] << delim
							<< ch->m_name << delim
							<< ch->m_nick << delim
							<< ch->m_desc->m_idname << delim
							<< ch->m_teachTime[0] << delim // 포기 횟수
							<< end;
				}
				else
				{
					GAMELOG << init("TEACH_GIVEUP_STUDENT-not-helper", tch)
							<< tch->m_name << delim
							<< tch->m_nick << delim
							<< tch->m_desc->m_idname << delim
							<< tch->m_fame << delim
							<< ch->m_name << delim
							<< ch->m_nick << delim
							<< ch->m_desc->m_idname << delim
							<< ch->m_teachTime[0] << delim // 포기 횟수
							<< end;
				}
				return;
			}

			if( !tch )
			{
				GAMELOG << init("TEACH_GIVEUP_STUDENT")
						<< ch->m_teachIdx[0] << delim
						<< ch->m_name << delim
						<< ch->m_nick << delim
						<< ch->m_desc->m_idname << delim
						<< ch->m_teachTime[0] << delim // 포기 횟수
						<< end;
			}
			else
			{
				GAMELOG << init("TEACH_GIVEUP_STUDENT", tch)
						<< tch->m_name << delim
						<< tch->m_nick << delim
						<< tch->m_desc->m_idname << delim
						<< tch->m_fame << delim
						<< ch->m_name << delim
						<< ch->m_nick << delim
						<< ch->m_desc->m_idname << delim
						<< ch->m_teachTime[0] << delim // 포기 횟수
						<< end;
			}
		}
		break;
	}
}

// 셋팅 해제
void do_TeachSetCancel(CPC* teacher, CPC* student)
{
	if (!teacher || !student)
		return;

	// 학생이 있는가
	bool bStudent = false;
	int i;
	for (i=0; i < TEACH_MAX_STUDENTS; i++)
	{
		if (teacher->m_teachIdx[i] == -1)
			continue;

		if (teacher->m_teachIdx[i] == student->m_index)
		{
			bStudent = true;
			break;
		}
	}

	// 학생 목록에 있을때
	if (bStudent)
	{
		// 셋팅해제
		teacher->m_teachIdx[i] = -1;
		teacher->m_teachJob[i] = -1;
		teacher->m_teachJob2[i] = 0;
		teacher->m_teachLevel[i] = 0;
		teacher->m_teachName[i][0] = '\0';

		// 남은 학생 체크 여부
		bool bTeacher = true;
		for (i=0; i < TEACH_MAX_STUDENTS; i++)
		{
			if (teacher->m_teachIdx[i] != -1)
				bTeacher = false;
		}

		// 선생박탈
		if (bTeacher)
			teacher->m_teachType = MSG_TEACH_NO_TYPE;
	}

	student->m_teachIdx[0] = -1;
	student->m_teachJob[0] = -1;
	student->m_teachJob2[0] = 0;
	student->m_teachLevel[0] = 0;
	student->m_teachName[0][0] = '\0';

	student->m_teachType = MSG_TEACH_NO_TYPE;
}

void do_TeachSetCancel(CPC* teacher, int studentidx)
{
	if (!teacher || !studentidx)
		return;

	// 학생이 있는가
	bool bStudent = false;
	int i;
	for (i=0; i < TEACH_MAX_STUDENTS; i++)
	{
		if (teacher->m_teachIdx[i] == -1)
			continue;

		if (teacher->m_teachIdx[i] == studentidx)
		{
			bStudent = true;
			break;
		}
	}

	// 학생 목록에 있을때
	if (bStudent)
	{
		// 셋팅해제
		teacher->m_teachIdx[i] = -1;
		teacher->m_teachJob[i] = -1;
		teacher->m_teachJob2[i] = 0;
		teacher->m_teachLevel[i] = 0;
		teacher->m_teachName[i][0] = '\0';
		teacher->m_teachTime[i] = 0;

		// 남은 학생 체크 여부
		bool bTeacher = true;
		for (i=0; i < TEACH_MAX_STUDENTS; i++)
		{
			if (teacher->m_teachIdx[i] != -1)
				bTeacher = false;
		}

		// 선생박탈
		if (bTeacher)
			teacher->m_teachType = MSG_TEACH_NO_TYPE;
	}
}
//