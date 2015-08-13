#include "stdhdrs.h"
#include "Log.h"
#include "Server.h"
#include "CmdMsg.h"

static int InitFDSET(socket_t s1, CDescriptor* slist, fd_set* infds, fd_set* outfds, fd_set* exfds);

//#if !defined ( LC_TLD  ) //|| !defined ( LC_USA )
#if !defined ( LC_USA  ) && !defined ( LC_TLD )
bool ReadCurrentInfo(int userindex, CUser& user, bool bForce);
bool ReadSectionInfo(const char* billguid, CSectionInfo& section);
bool PayBillGuid(CUser* user, const char* bguid);
#endif //#ifndef LC_TLD

void CServer::Run()
{
	struct timeval last_time, opt_time, process_time, temp_time;
	struct timeval before_sleep, now, timeout;
	CNetMsg inputmsg;
	CDescriptor* d;
	CDescriptor* dNext;
	int missed_pulses;
	int maxdesc;
	int count=0;

	// 시간 값 초기화
	opt_time.tv_usec = OPT_USEC;
	opt_time.tv_sec = 0;
	FD_ZERO(&m_null_set);
	GetTimeofDay(&last_time, (struct timezone *) 0);

	// 메인 루프
	while (!m_bshutdown)
	{
		// FD_SET 설정
		maxdesc = InitFDSET(m_ssock, m_desclist, &m_input_set, &m_output_set, &m_exc_set);

		// 이전 루프 처리 시간 구하기
		GetTimeofDay(&before_sleep, (struct timezone *) 0); /* current time */
		TimeDiff(&process_time, &before_sleep, &last_time);

		// 펄스 조정
		// 프로세스 시간이 1초에 10번(OPT_USEC) 수행되는 것을 기준으로 하는데
		// 오래 걸려서 놓친 pulse가 생기면 그 수를 계산하여 서버 펄스를 조정한다
		if (process_time.tv_sec == 0 && process_time.tv_usec < OPT_USEC)
		{
			missed_pulses = 0;
		}
		else
		{
			missed_pulses = process_time.tv_sec * PASSES_PER_SEC;
			missed_pulses += process_time.tv_usec / OPT_USEC;

			// 펄스 계산후 남은 처리 시간은 보존하여 최대한 오차를 줄여간다
			process_time.tv_sec = 0;
			process_time.tv_usec = process_time.tv_usec % OPT_USEC;
		}

		// 이번 루프에 소비될 시간을 구한다
		TimeDiff(&temp_time, &opt_time, &process_time);

		// 이전 처리 시간 + 이번 처리 시간을 구한다.
		TimeAdd(&last_time, &before_sleep, &temp_time);

		// 현재 시각을 구한다.
		GetTimeofDay(&now, (struct timezone *) 0);
		// 얼마나 Sleep 할 것인가를 정한다.
		TimeDiff(&timeout, &last_time, &now);

		// 시간 조정을 위한 딜레이
		do
		{
			if (timeout.tv_sec < 0)
			{
				GAMELOG << init("SYS_ERR")
						<< "Timeout value is negative"
						<< end;
				timeout.tv_sec = 0;

			}
			else if (timeout.tv_sec == 0 && timeout.tv_usec < 0)
			{
				GAMELOG << init("SYS_ERR")
						<< "Timeout value is negative"
						<< end;
				timeout.tv_usec = 0;
			}
			ServerSleep(&timeout);
			GetTimeofDay(&now, (struct timezone *) 0);
			TimeDiff(&timeout, &last_time, &now);
		} while (timeout.tv_usec || timeout.tv_sec);

		// 폴링 - 블럭 없이 select로 테스트
		a_NullTime.tv_sec = 0;
		a_NullTime.tv_usec = 0;
		if (select(maxdesc + 1, &m_input_set, &m_output_set, &m_exc_set, &a_NullTime) < 0)
		{
			GAMELOG << init("SYS_ERR") << "Select poll" << end;
			return;
		}

		// 새로운 접속 처리
		if (FD_ISSET(m_ssock, &m_input_set))
			NewDescriptor(m_ssock);

		// 연결되어 있는(입력을 받은) 접속 수
		count = 0;

		// 펄스를 하나 사용하므로 1 증가
		// 최소한 하나의 펄스를 사용
		missed_pulses++;
		if (missed_pulses <= 0) missed_pulses = 1;

		// 디스크립터 루프
		dNext = m_desclist;
		while ((d = dNext))
		{
			dNext = d->m_pNext;

			// 현재 상태가 연결이 끊어진 상태이면 스킵
			if (STATE(d) == CON_CLOSE || STATE(d) == CON_DISCONNECT) continue;

			// 디스크립터의 펄스 조정
			if (d->m_timestamp >=0) d->m_timestamp += missed_pulses;

			// 오류 발생 혹은 일정시간 입력이 없으면 연결 해제
			if (FD_ISSET(d->m_desc, &m_exc_set))
			{
				FD_CLR(d->m_desc, &m_input_set);
				FD_CLR(d->m_desc, &m_output_set);

				d->m_bclosed = true;
				CloseSocket(d);
				continue;
			}

			// 입력에서의 오류시 연결 해제
			if (FD_ISSET(d->m_desc, &m_input_set))
			{
				// 입력 쪽에서 접속이 끊어졌을 때..
				if (STATE(d) != CON_DISCONNECT && d->ProcessInput() < 0)
				{
					d->m_bclosed = true;
					CloseSocket(d);
					continue;
				}
			}

			count++;   // 정상적인 연결 수 증가

			// 입력 받은 메시지 처리
			if (!d->m_inQ.Get(inputmsg))
			{
				// 로그인 타임 아웃 검사
				if (STATE(d) == CON_GET_LOGIN)
				{
					d->m_connectreq ++;
					if (d->m_connectreq > 100)
					{
						/* m_input wait time 30 seconds */
						GAMELOG << init("BAD_CONNECTION", d->m_host) << end;

						d->m_bclosed = true;
						CloseSocket (d);
					}
				}
				continue;
			}

			do {

#ifdef BSTEST
				{
					GAMELOG << init("DUMP");
					int il;
					for (il = 0; il < inputmsg.m_size; il++)
					{
						char tmpstr[10];
						sprintf(tmpstr, " %02X", (unsigned char)(inputmsg.m_buf[il]));
						GAMELOG << tmpstr;
					}
					GAMELOG << end;
				}
#endif


				// 상태별 입력 처리
				if (STATE(d) == CON_GET_LOGIN)
				{
					if (!d->GetLogin(inputmsg))
					{
						inputmsg.MoveFirst();
						int version;
						inputmsg >> version;

						GAMELOG << init("BAD_VERSION", d->m_host) << version << end;

						d->m_bclosed = true;
						CloseSocket(d);
						break;
					}
				}
				else if (STATE(d) == CON_PLAYING)
				{
					CommandInterpreter(d, inputmsg);
				}

			} while (d->m_inQ.Get(inputmsg));

		} // 디스크립터 루프 끝

		if (m_bshutdown)
			break;

		// 패킷 보내기 디스크립터 루프
		dNext = m_desclist;
		while ((d = dNext))
		{
			dNext = d->m_pNext;

			// 연결 끊기
			if (d->m_bclosed || STATE(d) == CON_CLOSE || STATE(d) == CON_DISCONNECT)
			{
				CloseSocket(d);
				continue;
			}

			// 보내기
			if (d->m_outBuf.GetNextPoint() && FD_ISSET(d->m_desc, &m_output_set))
			{
				// 보내기 오류 처리
				if (!(d->m_bclosed) && d->ProcessOutput() < 0)
				{
					d->m_bclosed = true;
					CloseSocket(d);
					continue;
				}

			}

		} // 보내기 루프 끝

		// 펄스 적용
		m_pulse += missed_pulses;

		HeartBeat();

		// 연결 수 적용
		m_nDesc = count;
		// 펄스 소모 카운트 리셋
		missed_pulses = 0;
	}
}


static int InitFDSET(socket_t s1, CDescriptor* slist, fd_set* infds, fd_set* outfds, fd_set* exfds)
{
	FD_ZERO(infds);
	FD_ZERO(outfds);
	FD_ZERO(exfds);
	FD_SET(s1, infds);

	int maxdesc = s1;
	CDescriptor* d;
	CDescriptor* dNext = slist;

	while ((d = dNext))
	{
		dNext = d->m_pNext;

#ifndef CIRCLE_WINDOWS
		if (d->m_desc > maxdesc)
			maxdesc = d->m_desc;
#endif
		FD_SET(d->m_desc, infds);
		FD_SET(d->m_desc, outfds);
		FD_SET(d->m_desc, exfds);
	}

	return maxdesc;
}

void CServer::Close()
{
	CDescriptor* d;
	CDescriptor* dNext = m_desclist;

	GAMELOG << init("SYSTEM") << "Saving All User Data...." << end;

	while ((d = dNext))
	{
		dNext = d->m_pNext;
		CloseSocket(d);
	}

	GAMELOG << init("SYSTEM") << "Closing all sockets." << end;

	CLOSE_SOCKET(m_ssock);

	GAMELOG << init("SYSTEM") << "Normal termination of game." << end;
#ifdef CIRCLE_WINDOWS
	Sleep (30000);
#else
	sleep (30);
#endif
}

void CServer::HeartBeat()
{
//#if !defined ( LC_TLD  ) //|| !defined ( LC_USA )
#if !defined ( LC_USA  ) && !defined ( LC_TLD )
	if (m_pulseRefresh + PULSE_BILL_REFRESH <= m_pulse)
	{
		m_pulseRefresh = m_pulse;

		ReadSectionList();

		CDescriptor* pDesc;
		for (pDesc = m_desclist; pDesc; pDesc = pDesc->m_pNext)
			pDesc->m_userlist.RefreshRemainTime();
	}
#endif //#ifndef LC_TLD

	if (m_pulseWarn + PULSE_BILL_WARN <= m_pulse)
	{
		PingDB();

		m_pulseWarn = m_pulse;

//#if !defined ( LC_TLD  ) //|| !defined ( LC_USA )
#if !defined ( LC_USA  ) && !defined ( LC_TLD )
		// 경고 검사
		CDescriptor* pDesc;
		for (pDesc = m_desclist; pDesc; pDesc = pDesc->m_pNext)
			pDesc->m_userlist.SendWarn(pDesc);
#endif
	}

}

//#if !defined ( LC_TLD  ) //|| !defined ( LC_USA )
#if !defined ( LC_USA  ) && !defined ( LC_TLD )
void CUserList::SendWarn(CDescriptor* desc)
{
	CNetMsg rmsg;
	CUser* p;
	CUser data;
	int remain;
	bool bSendTimeout;

	for (p = m_head; p; p = p->m_next)
	{
		if (p->billsection() >= BILL_SECTION_FREE)
			continue ;

		remain = p->RemainTime();
		bSendTimeout = p->bSendTimeOut();

		bool bSent1 = p->bSent1();
		bool bSent5 = p->bSent5();
		bool bSent10 = p->bSent10();
		bool bSent30 = p->bSent30();
		bool bSent60 = p->bSent60();
		bool bSent1Day = p->bSent1Day();
		bool bSent3Day = p->bSent3Day();

		if (	   (remain == 0 && !p->bSendTimeOut())
				|| (remain < 1 * 60 + BILL_REAL_SEC_WARN && !p->bSent1())
				|| (remain < 5 * 60 + BILL_REAL_SEC_WARN && !p->bSent5())
				|| (remain < 10 * 60 + BILL_REAL_SEC_WARN && !p->bSent10())
				|| (remain < 30 * 60 + BILL_REAL_SEC_WARN && !p->bSent30())
				|| (remain < 60 * 60 + BILL_REAL_SEC_WARN && !p->bSent60())
				|| (remain < 1 * 24 * 60 * 60 + BILL_REAL_SEC_WARN && !p->bSent1Day())
				|| (remain < 3 * 24 * 60 * 60 + BILL_REAL_SEC_WARN && !p->bSent3Day())
				)
		{
#ifdef BSTEST
	GAMELOG << init("DO", strrchr(__FILE__, '\\') + 1) << __LINE__ << end;
#endif
			ReadCurrentInfo(p->index(), *p, true);
		}

		remain = p->RemainTime();

		if (remain == 0)
		{
			p->bSent3Day(true);
			p->bSent1Day(true);
			p->bSent60(true);
			p->bSent30(true);
			p->bSent10(true);
			p->bSent5(true);
			p->bSent1(true);

			if (!bSendTimeout)
			{
				p->bSendTimeOut(true);

				// 시간제이거나 예약이 있으면 자동 결제
				char paybguid[LEN_BILL_GUID];

				if (p->reserve_found() || p->billsection() == BILL_SECTION_HOUR)
				{
					bool isreserved = p->reserve_found();
					if (isreserved)
						strcpy(paybguid, p->reserve_billguild());
					else if (p->billsection() == BILL_SECTION_HOUR)
						strcpy(paybguid, p->billguid());

					if (PayBillGuid(p, paybguid))
					{
						GAMELOG << init("AUTO PAY");
						if (isreserved)
						{
							GAMELOG << "RESERVED SECTION" << delim;
						}
						else
						{
							GAMELOG << "CURRENT SECTION" << delim;
						}
						GAMELOG << "USERINDEX" << delim
								<< p->index() << delim
								<< "BILL GUID" << delim
								<< paybguid
								<< end;
						PayRepMsg(rmsg, BM_PAY_ERROR_OK, p->subno(), p->index(), p, 1);
						SEND_Q(rmsg, desc);
					}
					else
					{
						GAMELOG << init("AUTO PAY FAIL");
						if (isreserved)
						{
							GAMELOG << "RESERVED SECTION" << delim;
						}
						else
						{
							GAMELOG << "CURRENT SECTION" << delim;
						}
						GAMELOG << "USERINDEX" << delim
								<< p->index() << delim
								<< "BILL GUID" << delim
								<< paybguid
								<< end;
						PayRepMsg(rmsg, BM_PAY_ERROR_NOPOINT, p->subno(), p->index(), p, 1);
						SEND_Q(rmsg, desc);
					}
				}
				else
				{
					GAMELOG << init("TIME OUT")
							<< "USERINDEX" << delim
							<< p->index() << delim
							<< "BILL GUID" << delim
							<< p->billguid()
							<< end;
					// 시간제도 아니고, 예약도 없으면 타임아웃
					UserInfoRepMsg(rmsg, BM_USERINFO_ERROR_OK, p->subno(), p->index(), p, 0, 1);
					SEND_Q(rmsg, desc);
				}
			}
		}
		else if (remain < 1 * 60 + BILL_REAL_SEC_WARN && !bSent1)
		{
			p->bSent3Day(true);
			p->bSent1Day(true);
			p->bSent60(true);
			p->bSent30(true);
			p->bSent10(true);
			p->bSent5(true);
			p->bSent1(true);
			GAMELOG << init("WARNING 1 MIN")
					<< "USERINDEX" << delim
					<< p->index()
					<< end;
			UserInfoRepMsg(rmsg, BM_USERINFO_ERROR_OK, p->subno(), p->index(), p, 1, 0);
			SEND_Q(rmsg, desc);
		}
		else if (remain < 5 * 60 + BILL_REAL_SEC_WARN && !bSent5)
		{
			p->bSent3Day(true);
			p->bSent1Day(true);
			p->bSent60(true);
			p->bSent30(true);
			p->bSent10(true);
			p->bSent5(true);
			GAMELOG << init("WARNING 5 MIN")
					<< "USERINDEX" << delim
					<< p->index()
					<< end;
			UserInfoRepMsg(rmsg, BM_USERINFO_ERROR_OK, p->subno(), p->index(), p, 1, 0);
			SEND_Q(rmsg, desc);
		}
		else if (remain < 10 * 60 + BILL_REAL_SEC_WARN && !bSent10)
		{
			p->bSent3Day(true);
			p->bSent1Day(true);
			p->bSent60(true);
			p->bSent30(true);
			p->bSent10(true);
			GAMELOG << init("WARNING 10 MIN")
					<< "USERINDEX" << delim
					<< p->index()
					<< end;
			UserInfoRepMsg(rmsg, BM_USERINFO_ERROR_OK, p->subno(), p->index(), p, 1, 0);
			SEND_Q(rmsg, desc);
		}
		else if (remain < 30 * 60 + BILL_REAL_SEC_WARN && !bSent30)
		{
			p->bSent3Day(true);
			p->bSent1Day(true);
			p->bSent60(true);
			p->bSent30(true);
			GAMELOG << init("WARNING 30 MIN")
					<< "USERINDEX" << delim
					<< p->index()
					<< end;
			UserInfoRepMsg(rmsg, BM_USERINFO_ERROR_OK, p->subno(), p->index(), p, 1, 0);
			SEND_Q(rmsg, desc);
		}
		else if (remain < 60 * 60 + BILL_REAL_SEC_WARN && !bSent60)
		{
			p->bSent3Day(true);
			p->bSent1Day(true);
			p->bSent60(true);
			GAMELOG << init("WARNING 60 MIN")
					<< "USERINDEX" << delim
					<< p->index()
					<< end;
			UserInfoRepMsg(rmsg, BM_USERINFO_ERROR_OK, p->subno(), p->index(), p, 1, 0);
			SEND_Q(rmsg, desc);
		}
		else if (remain < 1 * 24 * 60 * 60 + BILL_REAL_SEC_WARN && !bSent1Day)
		{
			p->bSent3Day(true);
			p->bSent1Day(true);
			GAMELOG << init("WARNING 1 DAY")
					<< "USERINDEX" << delim
					<< p->index()
					<< end;
			UserInfoRepMsg(rmsg, BM_USERINFO_ERROR_OK, p->subno(), p->index(), p, 1, 0);
			SEND_Q(rmsg, desc);
		}
		else if (remain < 3 * 24 * 60 * 60 + BILL_REAL_SEC_WARN && !bSent3Day)
		{
			p->bSent3Day(true);
			GAMELOG << init("WARNING 3 DAY")
					<< "USERINDEX" << delim
					<< p->index()
					<< end;
			UserInfoRepMsg(rmsg, BM_USERINFO_ERROR_OK, p->subno(), p->index(), p, 1, 0);
			SEND_Q(rmsg, desc);
		}
	}
}
#endif //#ifndef LC_TLD