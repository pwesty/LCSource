#include <boost/format.hpp>
#include "stdhdrs.h"

#include "Log.h"
#include "Exp.h"
#include "Server.h"
#include "CmdMsg.h"
#include "doFunc.h"
#include "WarCastle.h"
#include "../ShareLib/CryptMem.h"
#include "DBManager.h"

// 1초마다
void CServer::HeartBeat()
{
	CheckEventTime();

	gserver->m_RoyalRumble.CheckRoyalRumble();
	gserver->m_RoyalRumble.CheckRoyalRumbleArea();

	CheckRaidReset();

	// 공성 NPC 검사
	CheckWarCastle();

	// 세금 징수 및 분배
	ProcTax();

	ProcOXQuiz();

	ProcRecommendServerMoonstone();

	ProcGoldenBall();

	ProcMonsterCombo();

#ifdef NEW_DOUBLE_EVENT_AUTO
	NewDoubleEventAuto();
#endif // NEW_DOUBLE_EVENT_AUTO

#ifdef EXTREME_CUBE
	m_extremeCube.CheckCubeSpace();
#endif // EXTREME_CUBE

	ProcEndExped();			//원정대 해체 처리

	ProcEndParty();			//패티 해체 처리

	ProcTrigger();

	gserver->gecho.Print( );

	ItemDropAtTime();

	ProcLoopCount();
}

void CServer::ProcTax()
{
	// 1분 단위로 castle에 저장을 하다가
	// 매일 오전 8시에 helper로 요청하고
	// 헬퍼의 응답이 오면 castle에서 제거, 제거후 받을 대상을 갱신
	if (m_taxSavePulse + PULSE_REAL_MIN <= m_pulse)
	{
		mysql_ping(&m_dbcastle);

		m_taxSavePulse = m_pulse;
		SaveTax();

		if (gserver->isRunHelper())
		{
			struct tm tmCur = NOW();
			// 요일 검사
			if (m_taxDivGuild == -1 || m_taxDivGuild != tmCur.tm_wday)
			{
				// 월요일 0시에 세금 받을 길드 변경
				if (tmCur.tm_wday == 1 && tmCur.tm_hour >= 0)
					ChangeTaxGuild();

				// 8시 검사
				if (tmCur.tm_hour >= 8)
				{
					m_taxDivGuild = tmCur.tm_wday;
					DivideTax();
				}
			}
		}
	}
}
//#endif

void CServer::ProcMobItemControl()
{
	int i, j;
	int mobAIVec = 0;
	int mobAISeq = 0;
	int pulseProcHeartBeatPerSec;

	pulseProcHeartBeatPerSec = m_pulseProcHeartBeatPerSec;
	mobAISeq = m_mobAISeq;

	while (m_pulse - pulseProcHeartBeatPerSec >= PASSES_PER_SEC)
		pulseProcHeartBeatPerSec += PASSES_PER_SEC;

	for (i = mobAISeq; ; i++)
	{
		if (i >= PASSES_PER_SEC)
		{
			i = 0;
		}
		mobAIVec |= (1 << i);
		if (i == m_pulse - pulseProcHeartBeatPerSec)
			break;
	}

	// 몹 컨트롤, 바닥 아이템 조정
	for (i=0; i < this->m_numZone; i++)
	{
		if (m_zones[i].m_bRemote)
			continue;

		for (j=0; j< m_zones[i].m_countArea; j++)
		{
			if (m_zones[i].m_area[j].GetMobAISeq() == -1)
				continue;

			if (!(m_zones[i].m_area[j].GetMobAIVec() & mobAIVec))
				continue;

			if (m_zones[i].m_area[j].m_bEnable)
			{
				// 싱글 던전이 아닐경우만 옵 행동 루틴
				if (!(m_zones[i].IsPersonalDungeon() || m_zones[i].IsGuildRoom()))
				{
					if (m_pulse - m_zones[i].m_area[j].m_pulseNPCAI >= PULSE_NPC_AI)
					{
						m_zones[i].m_area[j].m_pulseNPCAI = m_pulse;
						//GAMELOG << init("_DEBUG") << "pulse" << delim << m_pulse << delim << "zones" << delim << i << delim << "area" << delim << j << end;
						CheckMobActivityAndAkanProc(m_zones[i], m_zones[i].m_area[j]);

						m_zones[i].m_area[j].Monster_synchronization( m_zones[i].m_area[j].m_nNPC_Killed_NPC );	// 몹 리스트 동기화
					}

					//암흑의 재단, 예배당은 몹 죽으면 리젠 하지 안음
					if(m_zones[i].m_index != ZONE_CAPPELLA_1
							&& m_zones[i].m_index != ZONE_CAPPELLA_1)
					{
						if (m_pulse - m_zones[i].m_area[j].m_pulseNPCRegen >= PULSE_NPC_REGEN)
						{
							m_zones[i].m_area[j].m_pulseNPCRegen = m_pulse;
							m_zones[i].m_area[j].MobRegen();			// 몹 리젠
						}
					}
				}
				m_zones[i].m_area[j].GroundItemControl();	// 떨어진 아이템 정리
			}
		}
	}

	m_mobAISeq = m_pulse - m_pulseProcHeartBeatPerSec;

	// 사라진 아이템 숫자 로그 출력 : 1분에 한번
	if (m_pulse - m_resetAdjustItem >= PULSE_REAL_MIN)
	{
		for (i=0; i < m_numZone; i++)
		{
			for (j=0; j < m_zones[i].m_countArea; j++)
			{
				if (m_zones[i].m_area[j].m_bEnable && m_zones[i].m_area[j].m_nJunkItems)
				{
					// ITEM_ADJUST Log
					GAMELOG << init("ITEM_ADJUST")
							<< m_zones[i].m_index << delim
							<< m_zones[i].m_area[j].m_index << delim
							<< m_zones[i].m_area[j].m_nJunkItems
							<< end;

					m_zones[i].m_area[j].m_nJunkItems = 0;
				}
			}
		}
		m_resetAdjustItem = m_pulse;
	}
}

void CServer::ChangeWeather()
{
	int i;

	// 날씨 변화 : 30분에 한번 씩
	if (m_pulse - m_resetWeather >= PULSE_REAL_HALF)
	{
		for (i=0; i < this->m_numZone; i++)
		{
			if (m_zones[i].m_bRemote)
				continue ;
			m_zones[i].ChangeWeather();
		}

		m_resetWeather = m_pulse;
	}
}

void CServer::ProcLoopCount()
{
	if (m_bLoop)
	{
		struct timeval tvCurrent, tvDiff;
		GetTimeofDay(&tvCurrent, NULL);
		TimeDiff(&tvDiff, &tvCurrent, &m_tvLoop);
		if (tvDiff.tv_sec >= 10)
		{
			if(m_bLoop == 1)
			{
				if (m_descLoop)
				{
					std::string message = boost::str(boost::format("Loop Count (10 sec): %d") % m_nLoop);

					CNetMsg::SP rmsg(new CNetMsg);
					SayMsg(rmsg, MSG_CHAT_NOTICE, 0, "", "", message.c_str());

					SEND_Q(rmsg, m_descLoop);
				}

				m_bLoop = 0;
				m_descLoop = NULL;
			}
#ifdef GMTOOL
			else if(m_bLoop == 2)
			{
				std::string message = boost::str(boost::format("Loop Count (10 sec): %d") % m_nLoop);

				{
					CNetMsg::SP rmsg(new CNetMsg);
					MsgLoopCountRep(rmsg, 1, 1, gserver->m_serverno, gserver->m_subno, message.c_str());
					SEND_Q(rmsg, gserver->m_messenger);
				}
				m_bLoop = 0;
			}
#endif // GMTOOL
		}
		else
			m_nLoop++;
	}
}

void CServer::ProcStatistics()
{
	GAMELOG << init("STATISTICS")
			<< "ITEM_BUY" << delim
			<< m_statisticsItemBuy << delim
			<< "ITEM_SELL" << delim
			<< m_statisticsItemSell << delim
			<< "PRODUCE" << delim
			<< m_statistics152 << delim
			<< m_statistics153 << delim
			<< m_statistics154 << delim
			<< m_statistics155 << delim
			<< m_statistics156 << delim
			<< m_statistics157 << delim
			<< m_statistics158 << delim
			<< m_statistics159 << delim
			<< m_statistics160 << delim
			<< m_statistics161 << delim
			<< m_statistics162 << delim
			<< m_statistics163 << delim
			<< m_statistics197 << delim
			<< m_statistics198 << delim
			<< m_statistics199
			<< end;
}

void CServer::ProcOXQuiz()
{
	// OX 퀴즈
	if (m_bEventOX)
	{
		time_t timeCurrent;
		time(&timeCurrent);

		// OX 퀴즈 시작 알림
		if (!m_bOXQuizNoticeStart)
		{
			if (timeCurrent >= m_timeEventOXStart)
			{
				m_bOXQuizNoticeStart = true;
				CZone* pZone = gserver->FindZone(ZONE_OXQUIZROOM);
				if (pZone)
				{
					if (pZone->m_bRemote == false)
					{
						CNetMsg::SP rmsg(new CNetMsg);
						EventOXQuizMsg(rmsg, MSG_EVENT_OXQUIZ_START);
						pZone->m_area[0].SendToAllClient(rmsg);
					}
				}

				GAMELOG << init("OXEVENT START")
						<< end;
			}
		}

		// OX 퀴즈 정답 알림
		if (m_nOXQuizRealNo != -1)
		{
			if (timeCurrent >= m_timeOXQuizLimit)
			{
				map_listOXQuiz_t::iterator it = gserver->m_listOXQuiz.find(gserver->m_nOXQuizRealNo);
				if (it == gserver->m_listOXQuiz.end())
				{
					return;
				}
				COXQuizData* qdFind = it->second;

				CZone* pZone = gserver->FindZone(ZONE_OXQUIZROOM);
				if (pZone)
				{
					if (pZone->m_bRemote == false)
					{
						{
							CNetMsg::SP rmsg(new CNetMsg);
							EventOXQuizAnswerMsg(rmsg, m_nOXQuizSeqNo, qdFind->GetAnswer());
							pZone->m_area[0].SendToAllClient(rmsg);
						}

						// 상품 지급 결정
						int nItemIndex = -1;
						LONGLONG nItemCount = 0;
						switch (m_nOXQuizSeqNo)
						{
#ifdef LC_TLD
//						case  7:	nItemIndex = 723;	nItemCount = 5;		break;
//						case 10:	nItemIndex = 85;	nItemCount = 2;		break;
						case 7:
							nItemIndex = 723;
							nItemCount = 5;
							break;
						case 10:
							nItemIndex = 85;
							nItemCount = 2;
							break;
						case 13:
							nItemIndex = 1315;
							nItemCount = 10;
							break;
						case 15:
							nItemIndex = 1314;
							nItemCount = 10;
							break;
						case 17:
							nItemIndex = 85;
							nItemCount = 5;
							break;
						case 20:
							nItemIndex = 974;
							nItemCount = 1;
							break;
#elif defined(LC_KOR)
						// 2010년 04월 30일 보상 변경
						//	1단계 - 10문제까지 맞추면 경험치 증폭제 5개
						//	2단계 - 15문제까지 맞추면 고급제련석 3개
						//	3단계 - 18문제까지 맞추면 상급자용 아이리스의 상자 2개
						//	최종단계 - 20문제까지 모두 맞추면 나이트셰도우 생성카드 1개
						case 10:
							nItemIndex = 6094;
							nItemCount = 10;
							break;
						case 15:
							nItemIndex = 6652;
							nItemCount = 3;
							break;
						case 18:
							nItemIndex = 2548;
							nItemCount = 3;
							break;
						case 20:
							nItemIndex = 7209;
							nItemCount = 1;
							break;
							// 2010년 04월 30일 보상 변경
							//	1단계 - 10문제까지 맞추면 경험치 증폭제 5개
							//	2단계 - 15문제까지 맞추면 고급제련석 3개
							//	3단계 - 18문제까지 맞추면 상급자용 아이리스의 상자 2개
							//	최종단계 - 20문제까지 모두 맞추면 나이트셰도우 생성카드 1개
// 						case 10:	nItemIndex = 882;	nItemCount = 5;		break;
//						case 15:	nItemIndex = 85;	nItemCount = 3;		break;
//						case 18:	nItemIndex = 5078;	nItemCount = 2;		break;
//						case 20:	nItemIndex = 4914;	nItemCount = 1;		break;
							// 2007년 12월 4일 변경
							//	1단계 - 10문제까지 맞추면 최하급 문스톤 6개
							//	2단계 - 15문제까지 맞추면 하급 문스톤 4개
							//	3단계 - 18문제까지 맞추면 문스톤 3개
							//	최종단계 - 20문제까지 모두 맞추면 상급 문스톤 2개
//						case 10:	nItemIndex = 2545;	nItemCount = 6;		break;
//						case 15:	nItemIndex = 2546;	nItemCount = 4;		break;
//						case 18:	nItemIndex = 723;	nItemCount = 3;		break;
//						case 20:	nItemIndex = 2547;	nItemCount = 2;		break;
#elif defined(LC_GAMIGO)
						case 10:
							nItemIndex = 786;
							nItemCount = 10;
							break;
						case 15:
							nItemIndex = 723;
							nItemCount =  5;
							break;
						case 18:
							nItemIndex = 723;
							nItemCount = 10;
							break;
						case 20:
							nItemIndex = 85;
							nItemCount =  1;
							break;
#elif defined(LC_BILA)
						case 10:
							nItemIndex = 723;
							nItemCount = 10;
							break;  // 문스톤
						case 15:
							nItemIndex = 2843;
							nItemCount = 10;
							break;  // 카오스볼
						case 18:
							nItemIndex = 85;
							nItemCount = 5;
							break;  // 고제
						case 20:
							nItemIndex = 85;
							nItemCount = 10;
							break;	// 고제
#else
						case 10:
							nItemIndex = 786;
							nItemCount = 10;
							break;
						case 15:
							nItemIndex = 723;
							nItemCount = 10;
							break;
						case 18:
							nItemIndex = 85;
							nItemCount = 5;
							break;
						case 20:
							nItemIndex = 85;
							nItemCount = 10;
							break;
#endif // LC_TLD
						}

						// 오답자 내보내기
						int nExtra = (qdFind->GetAnswer()) ? 1 : 2;
						float left		= pZone->m_zonePos[nExtra][1] / 2.0;
						float top		= pZone->m_zonePos[nExtra][2] / 2.0;
						float right		= pZone->m_zonePos[nExtra][3] / 2.0;
						float bottom	= pZone->m_zonePos[nExtra][4] / 2.0;
						CZone* pStartZone = gserver->FindZone(ZONE_START);
						if (pStartZone == NULL)
							return;

// 먼저 이동시킬 유저를 선별후 이동 // 다른셀로 이동시 또는 이동중 삭제시 문제 발생
						std::vector<CPC*> m_MovePCList;

						int cx, cz;
						for (cx = 0; cx < pZone->m_area[0].m_size[0]; cx++)
						{
							for (cz = 0; cz < pZone->m_area[0].m_size[1]; cz++)
							{
								CCharacter* p;
								CCharacter* pNext = pZone->m_area[0].m_cell[cx][cz].m_listChar;
								while ((p = pNext))
								{
									pNext = pNext->m_pCellNext;
									if (IS_PC(p))
									{
										CPC* pPC = TO_PC(p);
										if (pPC->m_admin < 2)
										{
											// 앉아있으면 일으켜 새웁시다.
											pPC->ResetPlayerState(PLAYER_STATE_SITDOWN);
											pPC->CalcStatus(true);

											if (!(left < GET_X(pPC) && GET_X(pPC) < right && top < GET_Z(pPC) && GET_Z(pPC) < bottom))
											{
												m_MovePCList.push_back( pPC );		// 나가야 할사람 선별
											}
											else
											{
												GAMELOG << init("EVENT OX QUIZ CORRECT", pPC)
														<< "COUNT" << delim
														<< m_nOXQuizSeqNo << delim
														<< "QUIZ" << delim
														<< qdFind->GetQuizIndex() << delim
														<< qdFind->GetAnswer()
														<< end;
												if (nItemIndex != -1)
												{
													CItem* pItem = m_itemProtoList.CreateItem(nItemIndex, -1, 0, 0, nItemCount);
													if (pItem)
													{
														if (pPC->m_inventory.addItem(pItem) == false)
														{
															GAMELOG << init("EVENT OX QUIZ GIFT FAIL", pPC)
																	<< "COUNT" << delim
																	<< m_nOXQuizSeqNo << delim
																	<< "ITEM" << delim
																	<< itemlog(pItem)
																	<< end;
															delete pItem;
														}
													}
												}
											}
										}
									}
								}
							}
						}

						std::vector<CPC*>::iterator it = m_MovePCList.begin();
						std::vector<CPC*>::iterator endit = m_MovePCList.end();
						for(; it != endit; ++it)
						{
							CPC* movePc = *(it);
							// 내보내야할 사람 내보내기
							GAMELOG << init("EVENT OX QUIZ INCORRECT", movePc)
									<< "COUNT" << delim
									<< m_nOXQuizSeqNo << delim
									<< "QUIZ" << delim
									<< qdFind->GetQuizIndex() << delim
									<< qdFind->GetQuizString() << delim
									<< qdFind->GetAnswer()
									<< end;
							GoZoneForce(movePc, ZONE_START,
										pStartZone->m_zonePos[0][0],													// ylayer
										GetRandom(pStartZone->m_zonePos[0][1], pStartZone->m_zonePos[0][3]) / 2.0f,		// x
										GetRandom(pStartZone->m_zonePos[0][2], pStartZone->m_zonePos[0][4]) / 2.0f);	// z
						}
						m_MovePCList.clear();
					}
				}

				m_nOXQuizSeqNo++;
				m_nOXQuizGMCharIndex = -1;
				m_nOXQuizRealNo = -1;
			}
		}
	}
}

void CServer::ProcRecommendServerMoonstone()
{
	// 추천서버에서만
	if (!m_bRecommend)
	{
		m_pulseRecommendMoonstone = m_pulse;
		return ;
	}

	// 한시간에 한번
	if (m_pulse - m_pulseRecommendMoonstone >= PULSE_REAL_HOUR)
	{
		std::vector<int> listTargetPC;
		PCManager::map_t& playerMap			= PCManager::instance()->getPlayerMap();
		PCManager::map_t::iterator iter		= playerMap.begin();
		PCManager::map_t::iterator endIter	= playerMap.end();
		for (; iter != endIter; ++iter)
		{
			CPC* pc = (*iter).pPlayer;
			if (pc == NULL)
			{
				continue;
			}

			// 31레벨 이상 비피스존 유저대상

			if ( pc->m_level > RECOMMEND_SERVER_LEVEL && !pc->IsInPeaceZone(true)
					&& pc->m_admin < 2							// 운영자 제외
			   )
			{
				listTargetPC.push_back(pc->m_index);
			}
		}

		if (listTargetPC.empty() == false)
		{
			int ori_index = GetRandom(0, listTargetPC.size() - 1);
			int now_index = ori_index;
#if !defined (BILA_RECOMMEND_SERVER_REWARD_CHANGE)
			CItem* pItem = gserver->m_itemProtoList.CreateItem(723, -1, 0, 0, 10);
#else
			CItem* pItem = gserver->m_itemProtoList.CreateItem(2843, -1, 0, 0, 10);	// BILA는 추천 서버 보상 아이템이 카오스 볼
#endif // BILA_RECOMMEND_SERVER_REWARD_CHANGE
			if (pItem)
			{
				// pos에서 순차적으로 문스톤 10개를 지급할 수 있는 유저를 찾아 주고 알린다
				while(1)
				{
					if(pItem == NULL)
						break;

					CPC* pTargetPC = PCManager::instance()->getPlayerByCharIndex(listTargetPC[now_index]);
					if (pTargetPC && pTargetPC->m_inventory.addItem(pItem))
					{
						if (gserver->isRunMessenger())
						{
							CNetMsg::SP rmsg(new CNetMsg);
							MsgrRecommendMoonstoneMsg(rmsg, pTargetPC->m_index, pTargetPC->GetName());
							SEND_Q(rmsg, m_messenger);
						}
						else
						{
							CNetMsg::SP rmsg(new CNetMsg);
							EventRecommendMoonstoneMsg(rmsg, pTargetPC->m_index, pTargetPC->GetName());
							PCManager::instance()->sendToAll(rmsg);
						}

						GAMELOG << init("RECOMMEND SERVER MOONSTONE", pTargetPC)
								<< end;

						pItem = NULL;
						break;
					}
					else
					{
						if ((++now_index) >= listTargetPC.size())
						{
							now_index = 0;
						}

						if (now_index == ori_index)
						{
							GAMELOG << init("RECOMMEND SERVER MOONSTONE NOBODY")
									<< end;
							delete pItem;
							pItem = NULL;
							break;
						}
					} // else
				} // while
			} // if
		}

		m_pulseRecommendMoonstone = m_pulse;
	}
}

// 골든볼 종료 처리
void CServer::ProcGoldenBall()
{
	if (m_clGoldenBall.GetStatus() != GOLDENBALL_STATUS_NOTHING)
	{
		time_t tCurrent;
		time(&tCurrent);
		if (m_clGoldenBall.GetEndTime() <= tCurrent)
		{
			switch (m_clGoldenBall.GetStatus())
			{
			case GOLDENBALL_STATUS_VOTE:
				{
					{
						CNetMsg::SP rmsg(new CNetMsg);
						EventGoldenballVoteEndMsg(rmsg, m_clGoldenBall.GetTeam1Name(), m_clGoldenBall.GetTeam2Name());
						PCManager::instance()->sendToAll(rmsg);
					}

					m_clGoldenBall.EndVote();

					GAMELOG << init("EVENT GOLDEN BALL STATE CHANGE : VOTE => VOTE END") << end;
				}
				break;
			case GOLDENBALL_STATUS_GIFT:
				{
					{
						CNetMsg::SP rmsg(new CNetMsg);
						EventGoldenballGiftEndMsg(rmsg, m_clGoldenBall.GetTeam1Name(), m_clGoldenBall.GetTeam2Name());
						PCManager::instance()->sendToAll(rmsg);
					}

					m_clGoldenBall.End();

					GAMELOG << init("EVENT GOLDEN BALL STATE CHANGE : GIFT => END") << end;

					{
						CNetMsg::SP rmsg(new CNetMsg);
						MsgrEventGoldenballEndMsg(rmsg, 0);
						SEND_Q(rmsg, m_messenger);
					}
				}
				break;
			}
		}
	}
}

#ifdef NEW_DOUBLE_EVENT_AUTO
void CServer::NewDoubleEventAuto()
{
	if(!m_bDoubleEventAuto)
	{
		m_bDoubleEventAutoOn = false;
		return ;
	}

	struct tm time;
	time = NOW();
#ifdef LC_KOR
	static bool bAuto = false;
	int min;
	min = time.tm_min;
	if(min%20 == 0)
	{
		if(bAuto == false)
		{
			if(m_bDoubleEventAutoOn)
			{
				m_bDoubleEventAutoOn = false;
				GAMELOG << init("ComboDoubleEvent Stop")
						<< end;
			}
			else
			{
				m_bDoubleEventAutoOn = true;
				GAMELOG << init("ComboDoubleEvent Start")
						<< end;
			}

			bAuto = true;
		}
	}
	else
	{
		bAuto = false;
	}
#else
	switch(time.tm_hour)		// 21시 ~ 24시 콤보존 더블이벤트
	{
	case 21:
	case 22:
	case 23:
		{
			if(m_bDoubleEventAutoOn == false)
			{
				GAMELOG << init("ComboDoubleEvent Start")
						<< end;
			}
			m_bDoubleEventAutoOn = true;
		}
		break;
	default:
		{
			if(m_bDoubleEventAutoOn == true)
			{
				GAMELOG << init("ComboDoubleEvent Stop")
						<< end;
			}
			m_bDoubleEventAutoOn = false;
		}
		break;
	}
#endif // LC_KOR
}
#endif // NEW_DOUBLE_EVENT_AUTO

void CServer::CheckMobActivityAndAkanProc(CZone& Zone, CArea& Area)
{
	if(Zone.m_index != ZONE_AKAN_TEMPLE)
	{
		Area.MobActivity();
		return ;
	}
	else
	{
		if(Area.m_nPauseMobActivity < gserver->getNowSecond())
		{
			Area.MobActivity();			// 몹 행동
			if(Zone.m_index == ZONE_AKAN_TEMPLE)
			{
				if(Area.m_nCountDown > 0 )
				{
					CNetMsg::SP rmsg(new CNetMsg);
					SendRaidSceneCountDown(rmsg, Area.m_nCountDown);
					Area.SendToAllClient(rmsg);
					Area.m_nCountDown--;
				}
				else if (Area.m_nCountDown == 0)
				{
					CPC* pPC = NULL;
					CNetMsg::SP rmsg(new CNetMsg);

					for(int ai = 0; ai < 8; ai++)
					{
						if(Area.m_AkanPCList[ai] != NULL)
						{
							pPC = Area.m_AkanPCList[ai];
							break;
						}
					}
					Area.m_nCountDown--;

					// COUNT DOWN TODO
					{
						CNetMsg::SP rmsg(new CNetMsg);
						RaidSceneMsg(rmsg, OBJECT_TYPE_TODO, COUNT_DOWN, Area.m_nObjectIndex, -1);
						do_ExRaidScene(pPC, rmsg);
					}

					Area.m_nObjectIndex = -1;
				}
			}
		}
		else if(Area.m_nPauseMobActivity == gserver->getNowSecond() && Zone.m_index == ZONE_AKAN_TEMPLE)
		{
			{
				CNetMsg::SP rmsg(new CNetMsg);
				SendOutputMsg(rmsg, PAUSE_AREA, Area.m_nPauseIndex, 0);
				Area.SendToAllClient(rmsg);
			}

			CNPC* npc = Area.m_pBossAppearNPC;
			if(npc != NULL && IS_NPC(npc))
			{
				int cx, cz;
				Area.PointToCellNum(GET_X(npc), GET_Z(npc), &cx, &cz);

				CNetMsg::SP rmsg(new CNetMsg);
				AppearMsg(rmsg, npc, true);
				if( npc->m_idNum != 1124
						&& npc->m_idNum != 1179
				  )
					Area.SendToCell(rmsg, GET_YLAYER(npc), cx, cz);
				Area.m_pBossAppearNPC = NULL;
			}
			Area.m_nPauseMobActivity = 0;
		}
		if(Area.m_nDelayOpenGate == gserver->getNowSecond() && Zone.m_index == ZONE_AKAN_TEMPLE)
		{
			Area.m_nDelayOpenGate = 0;
			int pcindex;
			for(pcindex = 0; pcindex < 8; pcindex++)
			{
				if(Area.m_AkanPCList[pcindex] != NULL)
					break;
			}
			if(pcindex >= 0 && pcindex <= 7)
			{
				CNetMsg::SP rmsg(new CNetMsg);
				RaidSceneMsg(rmsg, OBJECT_TYPE_TODO, NO_IN_AREA, 2, 1);
				do_ExRaidScene(Area.m_AkanPCList[pcindex], rmsg);
			}
		}
	}
}

void CServer::ItemDropAtTime()
{
	if(m_bApplySkillToNPC == false)
	{
		if(getNowSecond() > 0)
		{
			vec_listNpcDropItemAtTime_t::iterator it = m_listNpcDropItemAtTime.begin();
			vec_listNpcDropItemAtTime_t::iterator endit = m_listNpcDropItemAtTime.end();
			for(; it != endit; ++it)
			{
				CNPC* npc = *(it);
				CSkill* skill = gserver->m_skillProtoList.Create(1354);
				if(skill)
				{
					bool bApply;
					ApplySkill(npc, npc, skill, -1, bApply);
					delete skill;
					skill = NULL;
				}
			}
			m_listNpcDropItemAtTime.clear();
			m_bApplySkillToNPC = true;
		}
	}
}

void CServer::PingDB(void)
{
	mysql_ping(&m_dbcharingame);

	mysql_ping(&m_dbdata);

	DBManager::instance()->MySQLPing();
}