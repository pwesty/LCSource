#include "stdhdrs.h"

#include "../ShareLib/DBCmd.h"
#include "Server.h"
#include "Log.h"
#include "WarGround.h"
#include "CmdMsg.h"

CWGPlayer::CWGPlayer()
{
	m_nWarType = -1;
	m_nCharIndex = -1;
	m_nLevelType = -1;
	m_nSelected = -1;
	m_next = NULL;
}
CWGPlayer::CWGPlayer(int wartype, int charindex, int leveltype, int select)
{
	m_nWarType = wartype;
	m_nCharIndex = charindex;
	m_nLevelType = leveltype;
	m_nSelected = select;		//0:미분류, 1:선택, 2:탈락, 3:경기취소
	m_next = NULL;
}
CWGPlayer::~CWGPlayer()
{
	m_next = NULL;
}

void CWGPlayer::SetCharIndex(int index)
{
	m_nCharIndex = index;
}
void CWGPlayer::SetLevelType(int type)
{
	m_nLevelType = type;
}
void CWGPlayer::SetSelected(int select)
{
	m_nSelected = select;
}
void CWGPlayer::SetNext(CWGPlayer* node)
{
	m_next = node;
}
void CWGPlayer::SetWarType(int type)
{
	m_nWarType = type;
}
int CWGPlayer::GetCharIndex()
{
	return m_nCharIndex;
}
int CWGPlayer::GetLevelType()
{
	return m_nLevelType;
}
int CWGPlayer::GetSelected()
{
	return m_nSelected;
}
CWGPlayer* CWGPlayer::GetNext()
{
	return m_next;
}
int CWGPlayer::GetWarType()
{
	return m_nWarType;
}

CWGPlayerList::CWGPlayerList()
{
	m_head = NULL;
	m_nTotalCount = 0;
	m_nRookieCount = 0;
	m_nSeniorCount = 0;
	m_nMasterCount = 0;
}

CWGPlayerList::~CWGPlayerList()
{
	CWGPlayer* temp;

	while((temp = m_head))
	{
		m_head = m_head->GetNext();
		temp->SetNext(NULL);
		delete temp;
		temp = NULL;
	}
}

void CWGPlayerList::AddTotalCount(int val)
{
	m_nTotalCount += val;
	if(m_nTotalCount < 0)
		m_nTotalCount = 0;
}

void CWGPlayerList::AddRookieCount(int val)
{
	m_nRookieCount += val;
	if(m_nRookieCount < 0)
		m_nRookieCount = 0;
}

void CWGPlayerList::AddSeniorCount(int val)
{
	m_nSeniorCount += val;
	if(m_nSeniorCount < 0)
		m_nSeniorCount = 0;
}

void CWGPlayerList::AddMasterCount(int val)
{
	m_nMasterCount += val;
	if(m_nMasterCount < 0)
		m_nMasterCount = 0;
}

int CWGPlayerList::GetTotalCount()
{
	return m_nTotalCount;
}

int CWGPlayerList::GetRookieCount()
{
	return m_nRookieCount;
}

int CWGPlayerList::GetSeniorCount()
{
	return m_nSeniorCount;
}

int CWGPlayerList::GetMasterCount()
{
	return m_nMasterCount;
}

void CWGPlayerList::AddNode(CWGPlayer* node)
{
	if(node == NULL)
		return ;
	if(m_head == NULL)
		m_head = node;
	else
	{
		CWGPlayer* temp = m_head;
		while(temp->GetNext())
		{
			temp = temp->GetNext();
		}
		temp->SetNext(node);
	}
	AddTotalCount(1);
	switch(node->GetLevelType())
	{
	case LEVEL_TYPE_ROOKIE:
		{
			AddRookieCount(1);
		}
		break;
	case LEVEL_TYPE_SENIOR:
		{
			AddSeniorCount(1);
		}
		break;
	case LEVEL_TYPE_MASTER:
		{
			AddMasterCount(1);
		}
		break;
	default:
		break;
	}
}

void CWGPlayerList::DelNode(int charindex)
{
	CWGPlayer* temp = NULL;
	CWGPlayer* prev = NULL;
	if(m_head && m_head->GetCharIndex() == charindex)
	{
		temp = m_head;
		m_head = m_head->GetNext();

		AddTotalCount(-1);
		switch(temp->GetLevelType())
		{
		case LEVEL_TYPE_ROOKIE:
			{
				AddRookieCount(-1);
			}
			break;
		case LEVEL_TYPE_SENIOR:
			{
				AddSeniorCount(-1);
			}
			break;
		case LEVEL_TYPE_MASTER:
			{
				AddMasterCount(-1);
			}
			break;
		default:
			break;
		}

		delete temp;
		temp = NULL;
	}
	else if(m_head)
	{
		prev = m_head;
		temp = m_head->GetNext();
		while(temp)
		{
			if(temp->GetCharIndex() == charindex)
			{
				prev->SetNext(temp->GetNext());

				AddTotalCount(-1);
				switch(temp->GetLevelType())
				{
				case LEVEL_TYPE_ROOKIE:
					{
						AddRookieCount(-1);
					}
					break;
				case LEVEL_TYPE_SENIOR:
					{
						AddSeniorCount(-1);
					}
					break;
				case LEVEL_TYPE_MASTER:
					{
						AddMasterCount(-1);
					}
					break;
				default:
					break;
				}

				delete temp;
				temp = NULL;
				break;
			}
			prev = prev->GetNext();
			temp = temp->GetNext();
		}
	}
}

bool CWGPlayerList::FindNode(int charindex)
{
	CWGPlayer* temp = m_head;
	while(temp)
	{
		if(temp->GetCharIndex() == charindex)
			return true;
		temp = temp->GetNext();
	}
	return false;
}

CWGPlayer* CWGPlayerList::GetNode(int charindex)
{
	CWGPlayer* temp = m_head;
	while(temp)
	{
		if(temp->GetCharIndex() == charindex)
			return temp;
		temp = temp->GetNext();
	}
	return NULL;
}

CWGPlayer* CWGPlayerList::GetHead()
{
	return m_head;
}

bool CWGPlayerList::Select()
{
	CWGPlayer* temp = GetHead();
	int allselect[3] = {PLAYER_GREATER_THEN_MAX, }; // 0: 40명보다 크다, 1: 10명보다 작다, 2: 41명 보다 작다
	if(GetRookieCount() < gserver->m_RoyalRumble.getMinPlayerCount())
	{
		allselect[LEVEL_TYPE_ROOKIE] = PLAYER_SMALLER_THEN_MIN;
	}
	else if(GetRookieCount() <= gserver->m_RoyalRumble.getMaxPlayerCount())
	{
		allselect[LEVEL_TYPE_ROOKIE] = PLAYER_BETWEEN_MIN_AND_MAX;
	}
	if(GetSeniorCount() < gserver->m_RoyalRumble.getMinPlayerCount())
	{
		allselect[LEVEL_TYPE_SENIOR] = PLAYER_SMALLER_THEN_MIN;
	}
	else if(GetSeniorCount() <= gserver->m_RoyalRumble.getMaxPlayerCount())
	{
		allselect[LEVEL_TYPE_SENIOR] = PLAYER_BETWEEN_MIN_AND_MAX;
	}
	if(GetMasterCount() < gserver->m_RoyalRumble.getMinPlayerCount())
	{
		allselect[LEVEL_TYPE_MASTER] = PLAYER_SMALLER_THEN_MIN;
	}
	else if(GetMasterCount() <= gserver->m_RoyalRumble.getMaxPlayerCount())
	{
		allselect[LEVEL_TYPE_MASTER] = PLAYER_BETWEEN_MIN_AND_MAX;
	}
	while(temp)
	{
		if(allselect[LEVEL_TYPE_ROOKIE] == PLAYER_SMALLER_THEN_MIN && temp->GetLevelType() == LEVEL_TYPE_ROOKIE)				// 10명보다 작으면
			temp->SetSelected(PLAYER_DRAW);																					// select = draw

		else if(allselect[LEVEL_TYPE_ROOKIE] == PLAYER_BETWEEN_MIN_AND_MAX && temp->GetLevelType() == LEVEL_TYPE_ROOKIE)		// 10~40명이면
			temp->SetSelected(PLAYER_SELECTED);																				// select = 선발

		else if(allselect[LEVEL_TYPE_ROOKIE] == PLAYER_GREATER_THEN_MAX && temp->GetLevelType() == LEVEL_TYPE_ROOKIE)		// 40명보다 크면
			temp->SetSelected(PLAYER_DROP);																					// select = 일단 탈락

		if(allselect[LEVEL_TYPE_SENIOR] == PLAYER_SMALLER_THEN_MIN && temp->GetLevelType() == LEVEL_TYPE_SENIOR)
			temp->SetSelected(PLAYER_DRAW);

		else if(allselect[LEVEL_TYPE_SENIOR] == PLAYER_BETWEEN_MIN_AND_MAX && temp->GetLevelType() == LEVEL_TYPE_SENIOR)
			temp->SetSelected(PLAYER_SELECTED);

		else if(allselect[LEVEL_TYPE_SENIOR] == PLAYER_GREATER_THEN_MAX && temp->GetLevelType() == LEVEL_TYPE_SENIOR)
			temp->SetSelected(PLAYER_DROP);

		if(allselect[LEVEL_TYPE_MASTER] == PLAYER_SMALLER_THEN_MIN && temp->GetLevelType() == LEVEL_TYPE_MASTER)
			temp->SetSelected(PLAYER_DRAW);

		else if(allselect[LEVEL_TYPE_MASTER] == PLAYER_BETWEEN_MIN_AND_MAX && temp->GetLevelType() == LEVEL_TYPE_MASTER)
			temp->SetSelected(PLAYER_SELECTED);

		else if(allselect[LEVEL_TYPE_MASTER] == PLAYER_GREATER_THEN_MAX && temp->GetLevelType() == LEVEL_TYPE_MASTER)
			temp->SetSelected(PLAYER_DROP);

		temp = temp->GetNext();
	}

	if( allselect[LEVEL_TYPE_ROOKIE] == PLAYER_SMALLER_THEN_MIN
			&& allselect[LEVEL_TYPE_SENIOR] == PLAYER_SMALLER_THEN_MIN
			&& allselect[LEVEL_TYPE_MASTER] == PLAYER_SMALLER_THEN_MIN )
	{
		//모든 레벨이 10명보다 작다. 모든 경기 취소(draw)
		return false; //경기 취소
	}

	int loop = 0;
	int i = 0;
	for(i = 0; i < 3; i++)
	{
		loop = 0;
		if(allselect[i] == PLAYER_GREATER_THEN_MAX)
		{
			temp = GetHead();
			while(temp)
			{
				if(temp->GetLevelType() == i && temp->GetSelected() == PLAYER_DROP)
				{
					temp->SetSelected(PLAYER_SELECTED);
					loop++;
				}
				if(loop > 39)
					break;
				temp = temp->GetNext();
			}
		}
	}
	return true;
}

void CWGPlayerList::ClearList()
{
	CWGPlayer* temp;

	while((temp = m_head))
	{
		m_head = m_head->GetNext();
		temp->SetNext(NULL);
		delete temp;
		temp = NULL;
	}
	m_nTotalCount = 0;
	m_nRookieCount = 0;
	m_nSeniorCount = 0;
	m_nMasterCount = 0;
	m_head = NULL;
}

void CWGPlayerList::SendResult()
{
	CWGPlayer* p = NULL;
	CWGPlayer* pn = NULL;

	pn = GetHead();
	while((p = pn))
	{
		pn = pn->GetNext();
		CPC* pPC = NULL;
		pPC = PCManager::instance()->getPlayerByCharIndex(p->GetCharIndex());
		if(pPC)
		{
			switch(p->GetSelected())
			{
			case PLAYER_NOT_SORT:
				{
					CNetMsg::SP rmsg(new CNetMsg);
					RoyalRumbleRegistResult(rmsg, PLAYER_NOT_SORT);
					SEND_Q(rmsg, pPC->m_desc);
					GAMELOG << init("ROYAL RUMBLE SELECT RESULT", pPC) << "NO STATUS" << end;
				}
				break;
			case PLAYER_SELECTED:
				{
					CNetMsg::SP rmsg(new CNetMsg);
					RoyalRumbleRegistResult(rmsg, PLAYER_SELECTED);
					SEND_Q(rmsg, pPC->m_desc);
					GAMELOG << init("ROYAL RUMBLE SELECT RESULT", pPC) << "SELECTED" << end;
				}
				break;
			case PLAYER_DROP:
				{
					CNetMsg::SP rmsg(new CNetMsg);
					RoyalRumbleRegistResult(rmsg, PLAYER_DROP);
					SEND_Q(rmsg, pPC->m_desc);
					GAMELOG << init("ROYAL RUMBLE SELECT RESULT", pPC) << "DROP" << end;
				}
				break;
			case PLAYER_DRAW:
				{
					CNetMsg::SP rmsg(new CNetMsg);
					RoyalRumbleRegistResult(rmsg, PLAYER_DRAW);
					SEND_Q(rmsg, pPC->m_desc);
					GAMELOG << init("ROYAL RUMBLE SELECT RESULT", pPC) << "GAME DRAW" << end;
				}
				break;
			default:
				break;
			}
		}
		else
		{
			switch(p->GetSelected())
			{
			case PLAYER_NOT_SORT:
				GAMELOG << init("ROYAL RUMBLE SELECT RESULT") << "CHAR INDEX" << delim << p->GetCharIndex() << "NO STATUS" << end;
				break;
			case PLAYER_SELECTED:
				GAMELOG << init("ROYAL RUMBLE SELECT RESULT") << "CHAR INDEX" << delim << p->GetCharIndex() << "SELECTED" << end;
				break;
			case PLAYER_DROP:
				GAMELOG << init("ROYAL RUMBLE SELECT RESULT") << "CHAR INDEX" << delim << p->GetCharIndex() << "DROP" << end;
				break;
			case PLAYER_DRAW:
				GAMELOG << init("ROYAL RUMBLE SELECT RESULT") << "CHAR INDEX" << delim << p->GetCharIndex() << "GAME DRAW" << end;
				break;
			default:
				break;
			}
		}
	}
}

CWaitPlayer::CWaitPlayer()
{
	m_bCheckIn = false;
	m_nWGType = -1;
	m_nLevelType = -1;
	m_cCharType = -1;
	m_nCharIndex = -1;
	m_bIsEntered = false;
	m_nTeam = -1;
	m_next = NULL;
	m_bHold = false;
}
CWaitPlayer::CWaitPlayer(int WGType, int leveltype, char chartype, int charindex, bool entered, int team)
{
	m_bCheckIn = false;
	m_nWGType = WGType;
	m_nLevelType = leveltype;
	m_cCharType = chartype;
	m_nCharIndex = charindex;
	m_bIsEntered = entered;
	m_nTeam = team;
	m_next = NULL;
	m_bHold = false;
}
CWaitPlayer::~CWaitPlayer()
{
	m_next = NULL;
}

bool CWaitPlayer::GetCheckIn()
{
	return m_bCheckIn;
}

int CWaitPlayer::GetWGType()
{
	return m_nWGType;
}

int CWaitPlayer::GetLevelType()
{
	return m_nLevelType;
}

char CWaitPlayer::GetCharType()
{
	return m_cCharType;
}

int CWaitPlayer::GetCharIndex()
{
	return m_nCharIndex;
}

bool CWaitPlayer::GetIsEntered()
{
	return m_bIsEntered;
}

int CWaitPlayer::GetTeam()
{
	return m_nTeam;
}

CWaitPlayer* CWaitPlayer::GetNext()
{
	return m_next;
}

void CWaitPlayer::SetCheckIn(bool checkin)
{
	m_bCheckIn = checkin;
}

void CWaitPlayer::SetWGType(int type)
{
	m_nWGType = type;
}

void CWaitPlayer::SetLevelType(int type)
{
	m_nLevelType = type;
}

void CWaitPlayer::SetCharType(char type)
{
	m_cCharType = type;
}

void CWaitPlayer::SetCharIndex(int index)
{
	m_nCharIndex = index;
}

void CWaitPlayer::SetEntered(bool boolean)
{
	m_bIsEntered = boolean;
}

void CWaitPlayer::SetTeam(int team)
{
	m_nTeam = team;
}

void CWaitPlayer::SetNext(CWaitPlayer* node)
{
	m_next = node;
}

void CWaitPlayer::SetHold(bool b)
{
	m_bHold = b;
}

bool CWaitPlayer::GetHold()
{
	return m_bHold;
}

CWaitPlayerList::CWaitPlayerList()
{
	m_head = NULL;
	m_nPlayerCount = 0;
	m_nRookieCount = 0;
	m_nSeniorCount = 0;
	m_nMasterCount = 0;
}
CWaitPlayerList::~CWaitPlayerList()
{
	CWaitPlayer* temp;

	while((temp = GetHead()))
	{
		m_head = m_head->GetNext();
		temp->SetNext(NULL);
		delete temp;
		temp = NULL;
	}
}

int CWaitPlayerList::GetPlayerCount()
{
	return m_nPlayerCount;
}

void CWaitPlayerList::AddNode(CWaitPlayer* node)
{
	CWaitPlayer* temp;
	if(node == NULL)
		return ;
	if(m_head == NULL)
		m_head = node;
	else
	{
		temp = m_head;
		while(temp->GetNext())
		{
			temp = temp->GetNext();
		}
		temp->SetNext(node);
	}
	m_nPlayerCount++;
	switch(node->GetLevelType())
	{
	case LEVEL_TYPE_ROOKIE:
		AddRookieCount(1);
		break;
	case LEVEL_TYPE_SENIOR:
		AddSeniorCount(1);
		break;
	case LEVEL_TYPE_MASTER:
		AddMasterCount(1);
		break;
	default:
		break;
	}
}

CWaitPlayer* CWaitPlayerList::GetNode(int charindex)
{
	CWaitPlayer* temp = m_head;
	while(temp)
	{
		if(temp->GetCharIndex() == charindex)
			return temp;
		temp = temp->GetNext();
	}
	return NULL;
}

void CWaitPlayerList::DelNode(int charindex)
{
	CWaitPlayer* temp = NULL;
	CWaitPlayer* prev = NULL;
	if(m_head && m_head->GetCharIndex() == charindex)
	{
		temp = m_head;
		m_head = m_head->GetNext();
		switch(temp->GetLevelType())
		{
		case 0:
			AddRookieCount(-1);
			break;
		case 1:
			AddSeniorCount(-1);
			break;
		case 2:
			AddMasterCount(-1);
			break;
		default:
			break;
		}
		delete temp;
		temp = NULL;
		m_nPlayerCount--;
	}
	else if(m_head)
	{
		prev = m_head;
		temp = m_head->GetNext();
		while(temp)
		{
			if(temp->GetCharIndex() == charindex)
			{
				prev->SetNext(temp->GetNext());
				switch(temp->GetLevelType())
				{
				case LEVEL_TYPE_ROOKIE:
					AddRookieCount(-1);
					break;
				case LEVEL_TYPE_SENIOR:
					AddSeniorCount(-1);
					break;
				case LEVEL_TYPE_MASTER:
					AddMasterCount(-1);
					break;
				default:
					break;
				}
				temp->SetNext(NULL);
				delete temp;
				temp = NULL;
				m_nPlayerCount--;
				break;
			}
			prev = prev->GetNext();
			temp = temp->GetNext();
		}
	}
}

bool CWaitPlayerList::FindNode(int charindex)
{
	CWaitPlayer* temp = m_head;
	while(temp)
	{
		if(temp->GetCharIndex() == charindex)
		{
			return true;
		}
		temp = temp->GetNext();
	}
	return false;
}

CWaitPlayer* CWaitPlayerList::GetHead()
{
	return m_head;
}

void CWaitPlayerList::ClearList()
{
	CWaitPlayer* temp;

	while((temp = GetHead()))
	{
		m_head = m_head->GetNext();
		temp->SetNext(NULL);
		delete temp;
		temp = NULL;
	}
	m_head = NULL;
	m_nPlayerCount = 0;
	m_nRookieCount = 0;
	m_nSeniorCount = 0;
	m_nMasterCount = 0;
}

void CWaitPlayerList::AddRookieCount(int n)
{
	m_nRookieCount += n;
	if(m_nRookieCount < 0)
		m_nRookieCount = 0;
}

void CWaitPlayerList::AddSeniorCount(int n)
{
	m_nSeniorCount += n;
	if(m_nSeniorCount < 0)
		m_nSeniorCount = 0;
}

void CWaitPlayerList::AddMasterCount(int n)
{
	m_nMasterCount += n;
	if(m_nMasterCount < 0)
		m_nMasterCount = 0;
}

int CWaitPlayerList::GetRookieCount()
{
	return m_nRookieCount;
}

int CWaitPlayerList::GetSeniorCount()
{
	return m_nSeniorCount;
}

int CWaitPlayerList::GetMasterCount()
{
	return m_nMasterCount;
}

void CWaitPlayerList::SendAllWaitPlayer(CNetMsg::SP& msg)
{
	CWaitPlayer* p = NULL;
	CWaitPlayer* pn = NULL;
	pn = GetHead();
	while((p = pn))
	{
		pn = pn->GetNext();
		if(p)
		{
			CPC* pPC = NULL;
			pPC = PCManager::instance()->getPlayerByCharIndex(p->GetCharIndex());
			if(pPC)
			{
				GAMELOG << init("ROYAL RUMBLE",pPC) << "SEND GO ZONE MSG" << end;
				SEND_Q(msg, pPC->m_desc);
			}
		}
	}
}
CRoyalRumble::CRoyalRumble():
	m_tRoyalRumbleTime(0),m_tRookieTime(0),m_tSeniorTime(0),m_tMasterTime(0),m_nRoyalRumbleProcess(0),m_bRoyalRumbleNotice(false),
	m_bIsRookiePlaying(false),m_bIsSeniorPlaying(false),m_bIsMasterPlaying(false),m_bRookieLastEnter(false),m_bSeniorLastEnter(false),m_bMasterLastEnter(false),
	m_nRookieWinnerCharIndex(0),m_nSeniorWinnerCharIndex(0),m_nMasterWinnerCharIndex(0),m_nWinnerIsRewarded(0),
	m_nMinPlayerCount(10),m_nMaxPlayerCount(50),
#ifdef LC_TLD
	m_nStartHour(21),
#elif LC_USA
	m_nStartHour(16),
#else
	m_nStartHour(22),
#endif
	m_bCondition(false),m_pRoyalRumbleZone(NULL),m_pRookieArea(NULL),m_pSeniorArea(NULL),m_pMasterArea(NULL)
{
	m_RoyalRumbleTime[0] = 0;
	m_RoyalRumbleTime[1] = 12;
	m_RoyalRumbleTime[2] = 18;
	m_RoyalRumbleTime[3] = 20;
	m_RoyalRumbleTime[4] = 22;
}
CRoyalRumble::~CRoyalRumble()
{
	m_pRoyalRumbleZone		= NULL;
	m_pRookieArea			= NULL;
	m_pSeniorArea			= NULL;
	m_pMasterArea			= NULL;
}

void CRoyalRumble::Init()
{
	m_pRoyalRumbleZone = gserver->FindZone(ZONE_ROYAL_RUMBLE);
	m_pRookieArea = m_pRoyalRumbleZone->m_area + LEVEL_TYPE_ROOKIE;
	m_pSeniorArea = m_pRoyalRumbleZone->m_area + LEVEL_TYPE_SENIOR;
	m_pMasterArea = m_pRoyalRumbleZone->m_area + LEVEL_TYPE_MASTER;
	loadRoyalRumbleSetting();
}

void CRoyalRumble::CheckRoyalRumble()
{
	if(gserver->m_subno != WAR_GROUND_CHANNEL)
		return ;

	if(GetRoyalRumbleTime() == 0)
	{
		bool b = false;
		if( gserver->m_RoyalRumble.GetRookiePlaying() ||
				gserver->m_RoyalRumble.GetSeniorPlaying() ||
				gserver->m_RoyalRumble.GetMasterPlaying() )
		{
			b = true;
		}
		//else
		//{
		//	b = true;
		//}
		if(b)
			return ;
		//int i;
		//b = false;
		//for(i = 0; i < ROYAL_RUMBLE_SCHEDULE; i++)
		//{
		//	if(gserver->m_tRealSystemTime.tm_hour == m_RoyalRumbleTime[i])
		//		b = true;
		//}
		//if(!b)
		//	return ;
		if(gserver->m_tRealSystemTime.tm_hour == getStartHour() && gserver->m_tRealSystemTime.tm_min == 0)
		{
			m_bCondition = true;
		}

		if(m_bCondition == true)
		{
			m_WaitPlayerList.ClearList();

			{
				// 메신저 서버를 통해 전채널 공지
				CNetMsg::SP rmsg(new CNetMsg);
				MsgrRoyalRumbleStart(rmsg, -1, gserver->m_serverno, -1, -1, MSG_MSGR_WARGROUND_NOTICE, 0);
				SEND_Q(rmsg, gserver->m_messenger);
			}

			SetRoyalRumbleNextProcess();
			{
				CNetMsg::SP rmsg(new CNetMsg);
				SetRoyalRumbleTime(mktime(&gserver->m_tRealSystemTime) + (5 * 60));
				RoyalRumbleTimeNotice(rmsg, 0, GetRemainWaitTime());
				PCManager::instance()->sendToAll(rmsg);
			}
			/*SetTimeNoticeType(1);*/
			/*SetTimeNotice(mktime(&gserver->m_tRealSystemTime) + (20 * 60));*/
			//RoyalRumbleTimeNotice(msg, (20*60), (unsigned char)GetTimeNotice());
			SetRoyalRumbleNotice(true);
			GAMELOG << init("ROYAL RUMBLE NOTICE") << "Player regist remain 20 minutes."<< end;
			m_bCondition = false;
		}
	}
	else
	{
		if (mktime(&gserver->m_tRealSystemTime) >= GetRoyalRumbleTime())
		{
			switch(GetRoyalRumbleProcess())
			{
			case 1:
				{
					// 15분 남았음.
					SetRoyalRumbleTime(mktime(&gserver->m_tRealSystemTime) + (5 * 60));
					SetRoyalRumbleNextProcess();
					CNetMsg::SP rmsg(new CNetMsg);
					RoyalRumbleTimeNotice(rmsg, 0, GetRemainWaitTime());
					PCManager::instance()->sendToAll(rmsg);
				}
				break;
			case 2:
				{
					// 10분 남았음.
					SetRoyalRumbleTime(mktime(&gserver->m_tRealSystemTime) + (5 * 60));
					SetRoyalRumbleNextProcess();
					CNetMsg::SP rmsg(new CNetMsg);
					RoyalRumbleTimeNotice(rmsg, 1, GetRemainWaitTime());
					PCManager::instance()->sendToAll(rmsg);
				}
				break;
			case 3:
				{
					{
						// 신청시간 5분 남았습니다.
						// 전체널에 신청시간 5분 남았다고 보내기
						CNetMsg::SP rmsg(new CNetMsg);
						MsgrRoyalRumbleStart(rmsg, -1, gserver->m_serverno, -1, -1, MSG_MSGR_WARGROUND_NOTICE, 1);
						SEND_Q(rmsg, gserver->m_messenger);
					}
					//5분 남았음
					AddRoyalRumbleTime((5*60));
					SetRoyalRumbleNextProcess();
					GAMELOG << init("ROYAL RUMBLE NOTICE") << "Player regist remain 5 minutes."<< end;
					{
						CNetMsg::SP rmsg(new CNetMsg);
						RoyalRumbleTimeNotice(rmsg, 2, GetRemainWaitTime());
						PCManager::instance()->sendToAll(rmsg);
					}
				}
				break;
			case 4:
				{
					// 신청 끝. 경기 시작 10분 전
					// 전체널 마감 공지
					{
						CNetMsg::SP rmsg(new CNetMsg);
						MsgrRoyalRumbleStart(rmsg, -1, gserver->m_serverno, -1, -1, MSG_MSGR_WARGROUND_NOTICE, 2);
						SEND_Q(rmsg, gserver->m_messenger);
					}
					SetRoyalRumbleNotice(false);
					GAMELOG << init("ROYAL RUMBLE NOTICE") << "Player regist end."<< end;

					// 선발될 선수 선택
					GAMELOG << init("ROYAL RUMBLE Player Select Start") << end;
					m_WGPlayerList.ShuffleWGPlayer(m_WGPlayerList.GetTotalCount());
					bool bRoyalRumbleCanStart = true;
					bRoyalRumbleCanStart = m_WGPlayerList.Select();
					GAMELOG << init("ROYAL RUMBLE Player Select End") << end;

					// 전 선수에게 탈락, 합격, 경기 draw메시지 보내기
					m_WGPlayerList.SendResult();

					// 신청자표에서 선발 선수표로 이동
					MakeWaitPlayer();
					m_WaitPlayerList.ShuffleWaitPlayer(m_WaitPlayerList.GetPlayerCount());
					AddRoyalRumbleTime((9*60));
					/*SetTimeNotice(0);*/
					/*SetTimeNoticeType(2);*/
					//SetTimeNotice(mktime(&gserver->m_tRealSystemTime) + (10 * 60));
					//RoyalRumbleTimeNotice(msg, (10*60), (unsigned char)GetTimeNotice());
					{
						CNetMsg::SP rmsg(new CNetMsg);
						RoyalRumbleTimeNotice(rmsg, 3, 0);
						PCManager::instance()->sendToAll(rmsg);
						CWaitPlayer* p = NULL;
						CWaitPlayer* pn = NULL;
						pn = m_WaitPlayerList.GetHead();
						while((p = pn))
						{
							pn = pn->GetNext();
							CPC* pc = PCManager::instance()->getPlayerByCharIndex(p->GetCharIndex());
							if(pc)
							{
								SEND_Q(rmsg, pc->m_desc);
							}
						}
					}
					SetRoyalRumbleNextProcess();
					if(bRoyalRumbleCanStart == false)
					{
						// rookie, senior, master 모든 경기가 draw되었다. 다음단계를 시행할 필요가 없다.
						// 로얄럼블 진행 시간을 초기화 하고, 단계를 초기화 한다.
						// 프로세스는 여기서 끝난다.
						SetRoyalRumbleTime(0);
						SetInitRoyalRumbleProcess();
					}
				}
				break;
			case 5:
				{
					// 선발선수들에게 존이동 메시지 보내기.
					CNetMsg::SP rmsg(new CNetMsg);
					RoyalRumbleGoZone(rmsg);
					m_WaitPlayerList.SendAllWaitPlayer(rmsg);
					AddRoyalRumbleTime(60);
					SetRoyalRumbleNextProcess();
				}
				break;
			//case 4:
			//	{
			//		AddRoyalRumbleTime(30);
			//		SetRoyalRumbleNextProcess();
			//	}
			//	break;
			case 6:
				{
					/*SetTimeNoticeType(0);*/
					/*SetTimeNotice(0);*/
					// 홀드된 사람 홀드 풀기
					/*UnHold();*/

					// 경기 시작
					// CheckIn 된 사람 4명을 뽑아서 전장에 투입 시키기
					int nLoop = 0;
					for(nLoop = 0; nLoop < 4; nLoop++)
					{
						EnterTheWarGround(LEVEL_TYPE_ROOKIE);
						EnterTheWarGround(LEVEL_TYPE_SENIOR);
						EnterTheWarGround(LEVEL_TYPE_MASTER);
					}

					bool bRookie = false;
					bool bSenior = false;
					bool bMaster = false;

					CWaitPlayer* pWp = NULL;
					CWaitPlayer* pWpNext = NULL;
					pWpNext = m_WaitPlayerList.GetHead();

					while((pWp = pWpNext))
					{
						pWpNext = pWpNext->GetNext();
						if(pWp->GetLevelType() == LEVEL_TYPE_ROOKIE)
							bRookie = true;
						else if(pWp->GetLevelType() == LEVEL_TYPE_SENIOR)
							bSenior = true;
						else if(pWp->GetLevelType() == LEVEL_TYPE_MASTER)
							bMaster = true;
					}

					if(bRookie)
					{
						SetRookiePlaying(true);
						AddRookieTime((GetRoyalRumbleTime()+30));
					}
					if(bSenior)
					{
						SetSeniorPlaying(true);
						AddSeniorTime((GetRoyalRumbleTime()+30));
					}
					if(bMaster)
					{
						SetMasterPlaying(true);
						AddMasterTime((GetRoyalRumbleTime()+30));
					}
					SetRoyalRumbleTime(0);
					SetInitRoyalRumbleProcess();

					int leftcount[3] = {0,};
					CWaitPlayer* player = NULL;
					CWaitPlayer* playern = NULL;
					playern = gserver->m_RoyalRumble.m_WaitPlayerList.GetHead();
					while((player = playern))
					{
						playern = playern->GetNext();
						if( player->GetCheckIn() == true )
							leftcount[player->GetLevelType()]++;
					}
					leftcount[LEVEL_TYPE_ROOKIE] -= 1;
					leftcount[LEVEL_TYPE_SENIOR] -= 1;
					leftcount[LEVEL_TYPE_MASTER] -= 1;
					int nloop = 0;
					for(nloop = 0; nloop < 3; nloop++)
					{
						if(leftcount[nloop] < 0)
							leftcount[nloop] = 0;
					}

					{
						CNetMsg::SP rmsg(new CNetMsg);
						RoyalRumbleLeftCount(rmsg, leftcount[LEVEL_TYPE_ROOKIE]);
						if(m_pRookieArea && leftcount[LEVEL_TYPE_ROOKIE] > 1) m_pRookieArea->SendToAllClient(rmsg);
					}

					{
						CNetMsg::SP rmsg(new CNetMsg);
						RoyalRumbleLeftCount(rmsg, leftcount[LEVEL_TYPE_SENIOR]);
						if(m_pSeniorArea && leftcount[LEVEL_TYPE_SENIOR] > 1) m_pSeniorArea->SendToAllClient(rmsg);
					}

					{
						CNetMsg::SP rmsg(new CNetMsg);
						RoyalRumbleLeftCount(rmsg, leftcount[LEVEL_TYPE_MASTER]);
						if(m_pMasterArea && leftcount[LEVEL_TYPE_MASTER] > 1) m_pMasterArea->SendToAllClient(rmsg);
					}
				}
				break;
			default:
				break;
			}
		}
	}
}

void CRoyalRumble::SetRoyalRumbleTime(time_t time)
{
	if(time < 0)
		return ;
	m_tRoyalRumbleTime = time;
}

void CRoyalRumble::AddRoyalRumbleTime(time_t time)
{
	m_tRoyalRumbleTime += time;

	if(m_tRoyalRumbleTime < 0)
		m_tRoyalRumbleTime = 0;
}

time_t CRoyalRumble::GetRoyalRumbleTime()
{
	return m_tRoyalRumbleTime;
}

void CRoyalRumble::SetRoyalRumbleNextProcess()
{
	m_nRoyalRumbleProcess++;
}

void CRoyalRumble::SetInitRoyalRumbleProcess()
{
	m_nRoyalRumbleProcess = 0;
}

int CRoyalRumble::GetRoyalRumbleProcess()
{
	return m_nRoyalRumbleProcess;
}

void CRoyalRumble::SetRoyalRumbleNotice(bool b)
{
	m_bRoyalRumbleNotice = b;
}

bool CRoyalRumble::GetRoyalRumbleNotice()
{
	return m_bRoyalRumbleNotice;
}

void CRoyalRumble::SetRookiePlaying(bool b)
{
	m_bIsRookiePlaying = b;
}

void CRoyalRumble::SetSeniorPlaying(bool b)
{
	m_bIsSeniorPlaying = b;
}

void CRoyalRumble::SetMasterPlaying(bool b)
{
	m_bIsMasterPlaying = b;
}

bool CRoyalRumble::GetRookiePlaying()
{
	return m_bIsRookiePlaying;
}

bool CRoyalRumble::GetSeniorPlaying()
{
	return m_bIsSeniorPlaying;
}

bool CRoyalRumble::GetMasterPlaying()
{
	return m_bIsMasterPlaying;
}

void CRoyalRumble::MakeWaitPlayer()
{
	// m_WGPlayerList에서 선발된 선수를 m_WaitPlayerList이동 시킨다.

	CWGPlayer* pTemp = NULL;
	CWGPlayer* pTempNext = m_WGPlayerList.GetHead();
	while((pTemp = pTempNext))
	{
		pTempNext = pTempNext->GetNext();
		if(pTemp->GetSelected() == PLAYER_SELECTED)
		{
			CWaitPlayer* pWait = NULL;
			pWait = new CWaitPlayer(pTemp->GetWarType(), pTemp->GetLevelType(), MSG_CHAR_PC, pTemp->GetCharIndex(), false, 0);
			m_WaitPlayerList.AddNode(pWait);
			GAMELOG << init("ROYAL RUMBLE Entry Player")
					<< "WAR TYPE" << delim << pWait->GetWGType() << delim
					<< "CHAR INDEX" << delim << pWait->GetCharIndex() << delim
					<< "LEVEL TYPE" << delim << pWait->GetLevelType() << delim << end;
		}
	}

	m_WGPlayerList.ClearList();
}

void CRoyalRumble::SetInitRookieTime()
{
	m_tRookieTime = 0;
}

void CRoyalRumble::SetInitSeniorTime()
{
	m_tSeniorTime = 0;
}

void CRoyalRumble::SetInitMasterTime()
{
	m_tMasterTime = 0;
}

void CRoyalRumble::AddRookieTime(time_t t)
{
	m_tRookieTime += t;
	if(m_tRookieTime < 0)
		m_tRookieTime = 0;
}

void CRoyalRumble::AddSeniorTime(time_t t)
{
	m_tSeniorTime += t;
	if(m_tSeniorTime < 0)
		m_tSeniorTime = 0;
}

void CRoyalRumble::AddMasterTime(time_t t)
{
	m_tMasterTime += t;
	if(m_tMasterTime < 0)
		m_tMasterTime = 0;
}

time_t CRoyalRumble::GetRookieTime()
{
	return m_tRookieTime;
}

time_t CRoyalRumble::GetSeniorTime()
{
	return m_tSeniorTime;
}

time_t CRoyalRumble::GetMasterTime()
{
	return m_tMasterTime;
}

void CRoyalRumble::CheckRoyalRumbleArea()
{
	time_t tGap = 0;
	if(GetRookiePlaying())
	{
		// Area에게 메시지 보내기
		tGap = (GetRookieTime() - mktime(&gserver->m_tRealSystemTime));
		if((tGap >= 0) && !GetRookieLastEnter())
		{
			//  tGap초 뒤 다음 선수가 입장합니다.
			CNetMsg::SP rmsg(new CNetMsg);
			RoyalRumbleEnterNextPlayer(rmsg, (int)tGap);
			m_pRookieArea->SendToAllClient(rmsg);
		}
		else if(tGap <= 0)
		{
			if(!GetRookieLastEnter())
			{
				// 선수입장, 선수가 없으면 WaitPlayerList에서 삭제
				int i;
				for(i = 0; i < 2; i++)				// 2명입장 시키기
				{
					EnterTheWarGround(LEVEL_TYPE_ROOKIE);
				}

				if(GetRemainPlayer(LEVEL_TYPE_ROOKIE))
					AddRookieTime(30);
				else
				{
					AddRookieTime((20 * 60));
					SetRookieLastEnter(true);
				}
			}
			else if(GetRookiePlaying())	// 다들어갔다. 20분도 지났다. 경기를 진행중이다. 디버프를 주자
			{
				//디버프
				SetDebuff(DEBUFF_TYPE_20MINUTE, LEVEL_TYPE_ROOKIE);
			}
		}
		// 게임 인원수가 많아지면 디버프를 준다.
		if(GetEnteredPlayer(LEVEL_TYPE_ROOKIE) > 7)
		{
			// 디버프
			SetDebuff(DEBUFF_TYPE_8PLAYER, LEVEL_TYPE_ROOKIE);
		}

		// 평화지역에 갔으면 디버프를 준다.
		// SetDebuff(DEBUFF_TYPE_PEACEZONE, LEVEL_TYPE_ROOKIE);

		// 1명 남았는지?
		if( GetRookieLastEnter() )
		{
			int nCount = 0;
			CWaitPlayer* p = NULL;
			CWaitPlayer* pn = NULL;
			pn = m_WaitPlayerList.GetHead();
			while((p = pn))
			{
				pn = pn->GetNext();
				if(p->GetCheckIn() && p->GetIsEntered() && p->GetLevelType() == LEVEL_TYPE_ROOKIE)
				{
					nCount++;
				}
			}
			if(nCount == 1)
			{
				p = NULL;
				pn = NULL;
				pn = m_WaitPlayerList.GetHead();
				while((p = pn))
				{
					pn = pn->GetNext();
					if(p->GetCheckIn() && p->GetIsEntered() && p->GetLevelType() == LEVEL_TYPE_ROOKIE)
					{
						// p가 rookie 우승자
						break;
					}
				}
				if(p)
				{
					CPC* pPC = NULL;
					pPC = PCManager::instance()->getPlayerByCharIndex(p->GetCharIndex());
					if(pPC)
					{
						if(pPC->m_assist.FindBySkillIndex(ROYAL_RUMBLE_DEBUFF_SKILL))
						{
							pPC->m_assist.CureBySkillIndex(ROYAL_RUMBLE_DEBUFF_SKILL);
						}
						// 전장 포인트 100포인트 지급
						pPC->AddWarGroundPoint(100);
						SetWinner(LEVEL_TYPE_ROOKIE, pPC);

						{
							// 서버군에 우승자 메시지 전송
							CNetMsg::SP rmsg(new CNetMsg);
							MsgrRoyalRumbleWinner(rmsg, -1, gserver->m_serverno, -1, -1, p->GetLevelType(), p->GetCharIndex(), pPC->GetName());
							SEND_Q(rmsg, gserver->m_messenger);
						}

						m_WaitPlayerList.DelNode(p->GetCharIndex());
						SetInitRookieTime();
						SetRookiePlaying(false);
						SetRookieLastEnter(false);
					}
				}
			}
		}
	}

	if(GetSeniorPlaying())
	{
		// Area에게 메시지 보내기
		tGap = (GetSeniorTime() - mktime(&gserver->m_tRealSystemTime));
		if(tGap >= 0  && !GetSeniorLastEnter())
		{
			// tGap초 뒤 다음 선수가 입장합니다.
			CNetMsg::SP rmsg(new CNetMsg);
			RoyalRumbleEnterNextPlayer(rmsg, (int)tGap);
			m_pSeniorArea->SendToAllClient(rmsg);
		}
		else if(tGap <= 0)
		{
			if(!GetSeniorLastEnter())
			{
				// 선수입장, 선수가 없으면 WaitPlayerList에서 삭제
				int i;
				for(i = 0; i < 2; i++)				// 2명입장 시키기
				{
					EnterTheWarGround(LEVEL_TYPE_SENIOR);
				}

				if(GetRemainPlayer(LEVEL_TYPE_SENIOR))
					AddSeniorTime(30);
				else
				{
					AddSeniorTime((20 * 60));
					SetSeniorLastEnter(true);
				}
			}
			else if(GetSeniorPlaying())	// 다들어갔다. 20분도 지났다. 디버프를 주자
			{
				// 디버프
				SetDebuff(DEBUFF_TYPE_20MINUTE, LEVEL_TYPE_SENIOR);
			}
		}
		// 게임 인원수가 많아지면 디버프를 준다.
		if(GetEnteredPlayer(LEVEL_TYPE_SENIOR) > 7)
		{
			// 디버프
			SetDebuff(DEBUFF_TYPE_8PLAYER, LEVEL_TYPE_SENIOR);
		}
		// 평화지역에 갔으면 디버프를 준다.
		// SetDebuff(DEBUFF_TYPE_PEACEZONE, LEVEL_TYPE_SENIOR);

		// 1명 남았는지?
		if( GetSeniorLastEnter() )
		{
			int nCount = 0;
			CWaitPlayer* p = NULL;
			CWaitPlayer* pn = NULL;
			pn = m_WaitPlayerList.GetHead();
			while((p = pn))
			{
				pn = pn->GetNext();
				if(p->GetCheckIn() && p->GetIsEntered() && p->GetLevelType() == LEVEL_TYPE_SENIOR)
				{
					nCount++;
				}
			}
			if(nCount == 1)
			{
				p = NULL;
				pn = NULL;
				pn = m_WaitPlayerList.GetHead();
				while((p = pn))
				{
					pn = pn->GetNext();
					if(p->GetCheckIn() && p->GetIsEntered() && p->GetLevelType() == LEVEL_TYPE_SENIOR)
					{
						// p가 rookie 우승자
						break;
					}
				}
				if(p)
				{
					CPC* pPC = NULL;
					pPC = PCManager::instance()->getPlayerByCharIndex(p->GetCharIndex());
					if(pPC)
					{
						if(pPC->m_assist.FindBySkillIndex(ROYAL_RUMBLE_DEBUFF_SKILL))
						{
							pPC->m_assist.CureBySkillIndex(ROYAL_RUMBLE_DEBUFF_SKILL);
						}
						// 전장 포인트 100포인트 지급
						pPC->AddWarGroundPoint(100);
						SetWinner(LEVEL_TYPE_SENIOR, pPC);

						{
							// 서버군에 우승자 메시지 전송
							CNetMsg::SP rmsg(new CNetMsg);
							MsgrRoyalRumbleWinner(rmsg, -1, gserver->m_serverno, -1, -1, p->GetLevelType(), p->GetCharIndex(), pPC->GetName());
							SEND_Q(rmsg, gserver->m_messenger);
						}

						m_WaitPlayerList.DelNode(p->GetCharIndex());
						SetInitSeniorTime();
						SetSeniorPlaying(false);
						SetSeniorLastEnter(false);
					}
				}
			}
		}
	}

	if(GetMasterPlaying())
	{
		// Area에게 메시지 보내기
		tGap = (GetMasterTime() - mktime(&gserver->m_tRealSystemTime));
		if(tGap >= 0  && !GetMasterLastEnter())
		{
			// tGap초 뒤 다음 선수가 입장합니다.
			CNetMsg::SP rmsg(new CNetMsg);
			RoyalRumbleEnterNextPlayer(rmsg, (int)tGap);
			m_pMasterArea->SendToAllClient(rmsg);
		}
		else if(tGap <= 0)
		{
			if(!GetMasterLastEnter())
			{
				// 선수입장, 선수가 없으면 WaitPlayerList에서 삭제
				int i;
				for(i = 0; i < 2; i++)				// 2명입장 시키기
				{
					EnterTheWarGround(LEVEL_TYPE_MASTER);
				}

				if(GetRemainPlayer(LEVEL_TYPE_MASTER))
					AddMasterTime(30);
				else
				{
					AddMasterTime((20 * 60));
					SetMasterLastEnter(true);
				}
			}
			else if(GetMasterPlaying())	// 다들어갔다. 20분도 지났다. 디버프를 주자
			{
				// 디버프
				SetDebuff(DEBUFF_TYPE_20MINUTE, LEVEL_TYPE_MASTER);
			}
		}
		// 게임 인원수가 많아지면 디버프를 준다.
		if(GetEnteredPlayer(LEVEL_TYPE_MASTER) > 7)
		{
			// 디버프
			SetDebuff(DEBUFF_TYPE_8PLAYER, LEVEL_TYPE_MASTER);
		}
		// 평화지역에 갔으면 디버프를 준다.
		// SetDebuff(DEBUFF_TYPE_PEACEZONE, LEVEL_TYPE_MASTER);

		// 1명 남았는지?
		if( GetMasterLastEnter() )
		{
			int nCount = 0;
			CWaitPlayer* p = NULL;
			CWaitPlayer* pn = NULL;
			pn = m_WaitPlayerList.GetHead();
			while((p = pn))
			{
				pn = pn->GetNext();
				if(p->GetCheckIn() && p->GetIsEntered() && p->GetLevelType() == LEVEL_TYPE_MASTER)
				{
					nCount++;
				}
			}
			if(nCount == 1)
			{
				p = NULL;
				pn = NULL;
				pn = m_WaitPlayerList.GetHead();
				while((p = pn))
				{
					pn = pn->GetNext();
					if(p->GetCheckIn() && p->GetIsEntered() && p->GetLevelType() == LEVEL_TYPE_MASTER)
					{
						// p가 rookie 우승자
						break;
					}
				}
				if(p)
				{
					CPC* pPC = NULL;
					pPC = PCManager::instance()->getPlayerByCharIndex(p->GetCharIndex());
					if(pPC)
					{
						if(pPC->m_assist.FindBySkillIndex(ROYAL_RUMBLE_DEBUFF_SKILL))
						{
							pPC->m_assist.CureBySkillIndex(ROYAL_RUMBLE_DEBUFF_SKILL);
						}
						// 전장 포인트 100포인트 지급
						pPC->AddWarGroundPoint(100);
						SetWinner(LEVEL_TYPE_MASTER, pPC);

						{
							// 서버군에 우승자 메시지 전송
							CNetMsg::SP rmsg(new CNetMsg);
							MsgrRoyalRumbleWinner(rmsg, -1, gserver->m_serverno, -1, -1, p->GetLevelType(), p->GetCharIndex(), pPC->GetName());
							SEND_Q(rmsg, gserver->m_messenger);
						}

						m_WaitPlayerList.DelNode(p->GetCharIndex());
						SetInitMasterTime();
						SetMasterPlaying(false);
						SetMasterLastEnter(false);
					}
				}
			}
		}
	}
}

void CRoyalRumble::SetRookieLastEnter(bool b)
{
	m_bRookieLastEnter = b;
}

void CRoyalRumble::SetSeniorLastEnter(bool b)
{
	m_bSeniorLastEnter = b;
}

void CRoyalRumble::SetMasterLastEnter(bool b)
{
	m_bMasterLastEnter = b;
}

bool CRoyalRumble::GetRookieLastEnter()
{
	return m_bRookieLastEnter;
}

bool CRoyalRumble::GetSeniorLastEnter()
{
	return m_bSeniorLastEnter;
}

bool CRoyalRumble::GetMasterLastEnter()
{
	return m_bMasterLastEnter;
}

void CRoyalRumble::EnterTheWarGround(int leveltype)
{
	CWaitPlayer* p = NULL;
	CWaitPlayer* pn = NULL;

	p = NULL;
	pn = m_WaitPlayerList.GetHead();

	int nCount = 0;

	switch(leveltype)
	{
	case LEVEL_TYPE_ROOKIE:
		nCount = m_WaitPlayerList.GetRookieCount();
		break;
	case LEVEL_TYPE_SENIOR:
		nCount = m_WaitPlayerList.GetSeniorCount();
		break;
	case LEVEL_TYPE_MASTER:
		nCount = m_WaitPlayerList.GetMasterCount();
		break;
	default:
		nCount = 0;
		break;
	}

	if(nCount > 0)
	{
		while((p = pn))
		{
			pn = pn->GetNext();
			if(p->GetLevelType() == leveltype && p->GetCheckIn() == true && p->GetIsEntered() == false)
			{
				CPC* pc = PCManager::instance()->getPlayerByCharIndex(p->GetCharIndex());
				if(pc)
				{
					GAMELOG << init("ROYAL RUMBLE",pc) << "INTO THE PLAY GROUND";
					switch(leveltype)
					{
					case LEVEL_TYPE_ROOKIE:
						GAMELOG << "(ROOKIE)";
						break;
					case LEVEL_TYPE_SENIOR:
						GAMELOG << "(SENIOR)";
						break;
					case LEVEL_TYPE_MASTER:
						GAMELOG << "(MASTER)";
						break;
					default :
						break;
					}
					GAMELOG << end;
					p->SetEntered(true);
					GoZone(pc, m_pRoyalRumbleZone->m_index,
						   m_pRoyalRumbleZone->m_zonePos[1][0],
						   GetRandom(m_pRoyalRumbleZone->m_zonePos[1][1], m_pRoyalRumbleZone->m_zonePos[1][3]) / 2.0f,
						   GetRandom(m_pRoyalRumbleZone->m_zonePos[1][2], m_pRoyalRumbleZone->m_zonePos[1][4]) / 2.0f);

					break;
				}
				else
				{
					GAMELOG << init("ROYAL RUMBLE") << "PLAYER NOT FOUND";
					switch(leveltype)
					{
					case LEVEL_TYPE_ROOKIE:
						GAMELOG << "(ROOKIE)";
						break;
					case LEVEL_TYPE_SENIOR:
						GAMELOG << "(SENIOR)";
						break;
					case LEVEL_TYPE_MASTER:
						GAMELOG << "(MASTER)";
						break;
					default :
						break;
					}
					GAMELOG << delim << "CAHR INDEX" << delim << p->GetCharIndex() << end;
					m_WaitPlayerList.DelNode(p->GetCharIndex());
				}
			}
		}
	}
}

void CRoyalRumble::UnHold()
{
	CWaitPlayer* p = NULL;
	CWaitPlayer* pn = NULL;
	CPC* pc = NULL;

	pn = m_WaitPlayerList.GetHead();
	while((p = pn))
	{
		if(p->GetHold())
		{
			pc = PCManager::instance()->getPlayerByCharIndex(p->GetCharIndex());
			if(pc)
			{
				CNetMsg::SP rmsg(new CNetMsg);
				RoyalRumbleHold(rmsg, 0);
				SEND_Q(rmsg, pc->m_desc);
				p->SetHold(false);
			}
		}
	}
}

int CRoyalRumble::GetRemainPlayer(int leveltype)
{
	int nCount = 0;
	CWaitPlayer* p = NULL;
	CWaitPlayer* pn = NULL;
	pn = m_WaitPlayerList.GetHead();
	switch(leveltype)
	{
	case LEVEL_TYPE_ROOKIE:
		{
			while((p = pn))
			{
				pn = pn->GetNext();
				if(p->GetCheckIn() && !p->GetIsEntered() && p->GetLevelType() == LEVEL_TYPE_ROOKIE)
					nCount++;
			}
			return nCount;
		}
		break;
	case LEVEL_TYPE_SENIOR:
		{
			while((p = pn))
			{
				pn = pn->GetNext();
				if(p->GetCheckIn() && !p->GetIsEntered() && p->GetLevelType() == LEVEL_TYPE_SENIOR)
					nCount++;
			}
			return nCount;
		}
		break;
	case LEVEL_TYPE_MASTER:
		{
			while((p = pn))
			{
				pn = pn->GetNext();
				if(p->GetCheckIn() && !p->GetIsEntered() && p->GetLevelType() == LEVEL_TYPE_MASTER)
					nCount++;
			}
			return nCount;
		}
		break;
	case LEVEL_TYPE_ALL:
		{
			while((p = pn))
			{
				pn = pn->GetNext();
				if(p->GetCheckIn() && !p->GetIsEntered())
					nCount++;
			}
			return nCount;
		}
		break;
	default:
		break;
	}
	return -1; // -1이 리턴 되면 에러
}

int CRoyalRumble::GetEnteredPlayer(int leveltype)
{
	int nCount = 0;
	CWaitPlayer* p = NULL;
	CWaitPlayer* pn = NULL;
	pn = m_WaitPlayerList.GetHead();
	switch(leveltype)
	{
	case LEVEL_TYPE_ROOKIE:
		{
			while((p = pn))
			{
				pn = pn->GetNext();
				if(p->GetCheckIn() && p->GetIsEntered() && p->GetLevelType() == LEVEL_TYPE_ROOKIE)
					nCount++;
			}
			return nCount;
		}
		break;
	case LEVEL_TYPE_SENIOR:
		{
			while((p = pn))
			{
				pn = pn->GetNext();
				if(p->GetCheckIn() && p->GetIsEntered() && p->GetLevelType() == LEVEL_TYPE_SENIOR)
					nCount++;
			}
			return nCount;
		}
		break;
	case LEVEL_TYPE_MASTER:
		{
			while((p = pn))
			{
				pn = pn->GetNext();
				if(p->GetCheckIn() && p->GetIsEntered() && p->GetLevelType() == LEVEL_TYPE_MASTER)
					nCount++;
			}
			return nCount;
		}
		break;
	case LEVEL_TYPE_ALL:
		{
			while((p = pn))
			{
				pn = pn->GetNext();
				if(p->GetCheckIn() && p->GetIsEntered())
					nCount++;
			}
			return nCount;
		}
		break;
	default:
		break;
	}
	return -1; // -1이 리턴 되면 에러
}

void CRoyalRumble::SetDebuff(int debufftype, int leveltype)
{
	switch(debufftype)
	{
	case DEBUFF_TYPE_8PLAYER:
	case DEBUFF_TYPE_20MINUTE:
		{
			CWaitPlayer* p = NULL;
			CWaitPlayer* pn = NULL;
			pn = m_WaitPlayerList.GetHead();
			while((p = pn))
			{
				pn = pn->GetNext();
				if( p->GetLevelType() == leveltype
						&& p->GetCheckIn() == true
						&& p->GetIsEntered() == true)
				{
					CPC* pc;
					pc = PCManager::instance()->getPlayerByCharIndex(p->GetCharIndex());
					if(pc)
					{
						if(!pc->m_assist.FindBySkillIndex(ROYAL_RUMBLE_DEBUFF_SKILL))
						{
							CSkill* pSkill = NULL;
							int skilllevel = 1;
							skilllevel += p->GetLevelType();
							pSkill = gserver->m_skillProtoList.Create(ROYAL_RUMBLE_DEBUFF_SKILL, skilllevel);
							bool bApply;
							ApplySkill(pc, pc, pSkill, -1, bApply);
							GAMELOG << init("ROYAL RUMBLE", pc) << "DEBUFF SKILL" << end;
							if(!bApply && pSkill)
							{
								delete pSkill;
								pSkill = NULL;
							}
						}
					}
				}
			}
		}
		break;

	case DEBUFF_TYPE_PEACEZONE:
		{
			CWaitPlayer* p = NULL;
			CWaitPlayer* pn = NULL;
			pn = m_WaitPlayerList.GetHead();
			while((p = pn))
			{
				pn = pn->GetNext();
				if( p->GetLevelType() == leveltype
						&& p->GetCheckIn() == true
						&& p->GetIsEntered() == true)
				{
					CPC* pc;
					pc = PCManager::instance()->getPlayerByCharIndex(p->GetCharIndex());
					if(pc && pc->IsInPeaceZone(true))
					{
						if(!pc->m_assist.FindBySkillIndex(ROYAL_RUMBLE_DEBUFF_SKILL))
						{
							CSkill* pSkill = NULL;
							int skilllevel = 1;
							skilllevel += p->GetLevelType();
							pSkill = gserver->m_skillProtoList.Create(ROYAL_RUMBLE_DEBUFF_SKILL, skilllevel);
							bool bApply;
							ApplySkill(pc, pc, pSkill, -1, bApply);
							GAMELOG << init("ROYAL RUMBLE", pc) << "DEBUFF SKILL" << end;
							if(!bApply && pSkill)
							{
								delete pSkill;
								pSkill = NULL;
							}
						}
					}
				}
			}
		}
		break;
	default:
		break;
	}
}

int CRoyalRumble::GetWinner(int LevelType)
{
	switch(LevelType)
	{
	case LEVEL_TYPE_ROOKIE:
		return m_nRookieWinnerCharIndex;
		break;
	case LEVEL_TYPE_SENIOR:
		return m_nSeniorWinnerCharIndex;
		break;
	case LEVEL_TYPE_MASTER:
		return m_nMasterWinnerCharIndex;
		break;
	default:
		return 0;
		break;
	}
	return 0;
}

void CRoyalRumble::SetWinner(int LevelType, CPC* pc)
{
	if(!pc)
		return ;
	switch(LevelType)
	{
	case LEVEL_TYPE_ROOKIE:
		m_nRookieWinnerCharIndex = pc->m_index;
		InitRewarded(LEVEL_TYPE_ROOKIE);
		GAMELOG << init("ROYAL RUMBLE WINNER(ROOKIE)", pc) << end;
		break;
	case LEVEL_TYPE_SENIOR:
		m_nSeniorWinnerCharIndex = pc->m_index;
		InitRewarded(LEVEL_TYPE_SENIOR);
		GAMELOG << init("ROYAL RUMBLE WINNER(SENIOR)", pc) << end;
		break;
	case LEVEL_TYPE_MASTER:
		m_nMasterWinnerCharIndex = pc->m_index;
		InitRewarded(LEVEL_TYPE_MASTER);
		GAMELOG << init("ROYAL RUMBLE WINNER(MASTER)", pc) << end;
		break;
	default:
		break;
	}

	{
		//HelperRoyalRumbleWinnerLogMsg(msg, pc, LevelType);
		CNetMsg::SP rmsg(new CNetMsg);
		rmsg->Init(MSG_HELPER_COMMAND);
		RefMsg(rmsg) << MSG_HELPER_ROYAL_RUMBLE_WINNER
					 << pc->m_index
					 << pc->m_name
					 << pc->m_nick
					 << LevelType;
		SEND_Q(rmsg, gserver->m_helper);
	}
}
void CRoyalRumble::SetRewarded(int LevelType, CPC* pc)
{
	if(!pc)
		return ;
	switch(LevelType)
	{
	case LEVEL_TYPE_ROOKIE:
		{
			if( m_nWinnerIsRewarded != 1
					&& m_nWinnerIsRewarded != 3
					&& m_nWinnerIsRewarded != 5
					&& m_nWinnerIsRewarded != 7
			  )
			{
				m_nWinnerIsRewarded += 1;
				GAMELOG << init("ROYAL RUMBLE REWARDED(ROOKIE)", pc) << end;
			}
		}
		break;
	case LEVEL_TYPE_SENIOR:
		{
			if( m_nWinnerIsRewarded != 2
					&& m_nWinnerIsRewarded != 3
					&& m_nWinnerIsRewarded != 6
					&& m_nWinnerIsRewarded != 7
			  )
			{
				m_nWinnerIsRewarded += 2;
				GAMELOG << init("ROYAL RUMBLE REWARDED(SENIOR)", pc) << end;
			}
		}
		break;
	case LEVEL_TYPE_MASTER:
		{
			if( m_nWinnerIsRewarded != 4
					&& m_nWinnerIsRewarded != 5
					&& m_nWinnerIsRewarded != 6
					&& m_nWinnerIsRewarded != 7
			  )
			{
				m_nWinnerIsRewarded += 4;
				GAMELOG << init("ROYAL RUMBLE REWARDED(MASTER)", pc) << end;
			}
		}
		break;
	default:
		break;
	}
}
bool CRoyalRumble::GetRewarded(int LevelType)
{
	switch(LevelType)
	{
	case LEVEL_TYPE_ROOKIE:
		{
			if( m_nWinnerIsRewarded != 1
					&& m_nWinnerIsRewarded != 3
					&& m_nWinnerIsRewarded != 5
					&& m_nWinnerIsRewarded != 7
			  )
			{
				return false;
			}
		}
		break;
	case LEVEL_TYPE_SENIOR:
		{
			if( m_nWinnerIsRewarded != 2
					&& m_nWinnerIsRewarded != 3
					&& m_nWinnerIsRewarded != 6
					&& m_nWinnerIsRewarded != 7
			  )
			{
				return false;
			}
		}
		break;
	case LEVEL_TYPE_MASTER:
		{
			if( m_nWinnerIsRewarded != 4
					&& m_nWinnerIsRewarded != 5
					&& m_nWinnerIsRewarded != 6
					&& m_nWinnerIsRewarded != 7
			  )
			{
				return false;
			}
		}
		break;
	default:
		return false;
		break;
	}
	return true;
}
void CRoyalRumble::InitRewarded(int LevelType)
{
	switch(LevelType)
	{
	case LEVEL_TYPE_ROOKIE:
		{
			if( m_nWinnerIsRewarded == 1
					|| m_nWinnerIsRewarded == 3
					|| m_nWinnerIsRewarded == 5
					|| m_nWinnerIsRewarded == 7
			  )
			{
				m_nWinnerIsRewarded -= 1;
			}
		}
		break;
	case LEVEL_TYPE_SENIOR:
		{
			if( m_nWinnerIsRewarded == 2
					|| m_nWinnerIsRewarded == 3
					|| m_nWinnerIsRewarded == 6
					|| m_nWinnerIsRewarded == 7
			  )
			{
				m_nWinnerIsRewarded -= 2;
			}
		}
		break;
	case LEVEL_TYPE_MASTER:
		{
			if( m_nWinnerIsRewarded == 4
					|| m_nWinnerIsRewarded == 5
					|| m_nWinnerIsRewarded == 6
					|| m_nWinnerIsRewarded == 7
			  )
			{
				m_nWinnerIsRewarded -= 4;
			}
		}
		break;
	default:
		break;
	}
}

void CWGPlayerList::SetHead(CWGPlayer* node)
{
	m_head = node;
}

CWGPlayer* CWGPlayerList::GetTail()
{
	CWGPlayer* p = NULL;
	CWGPlayer* pn = NULL;
	pn = GetHead();
	if(pn == NULL)
		return NULL;

	while((p = pn))
	{
		pn = pn->GetNext();
		if(p->GetNext() == NULL)
			return p;
	}
	return NULL;
}

void CWGPlayerList::ShuffleWGPlayer(int nShuffleCount)
{
	int i = 0;
	int j = 0;
	int nCount = 0;
	int location = 0;
	CWGPlayer* p = NULL;
	CWGPlayer* move = NULL;
	CWGPlayer* tail = NULL;
	CWGPlayer* pn = NULL;

	if(!GetHead())
		return ;

	nCount = GetTotalCount();
	if(nCount < 2)
		return ;

	for(i = 0; i < nShuffleCount; i++)
	{
		location = GetRandom(0, nCount-1);
		move = GetHead();
		for(j = 0; j < location; j++)
		{
			move = move->GetNext();
		}

		if(move == GetHead())
		{
			tail = GetTail();
			SetHead(move->GetNext());
			move->SetNext(NULL);
			if(tail != NULL)
				tail->SetNext(move);
		}
		else
		{
			tail = GetTail();
			if(move != tail)
			{
				pn = GetHead();
				while((p = pn))
				{
					pn = pn->GetNext();
					if(p->GetNext() == move)
						break;
				}
				// p가 move의 prev
				p->SetNext(move->GetNext());
				move->SetNext(NULL);
				if(tail != NULL)
					tail->SetNext(move);
			}
		}
	}
}

void CWaitPlayerList::SetHead(CWaitPlayer* node)
{
	m_head = node;
}

CWaitPlayer* CWaitPlayerList::GetTail()
{
	CWaitPlayer* p = NULL;
	CWaitPlayer* pn = NULL;
	pn = GetHead();

	if(m_head == NULL)
	{
		return NULL;
	}

	while((p = pn))
	{
		pn = pn->GetNext();
		if(p->GetNext() == NULL)
		{
			return p;
		}
	}
	return NULL;
}

void CWaitPlayerList::ShuffleWaitPlayer(int nShuffleCount)
{
	int i = 0;
	int j = 0;
	int nCount = 0;
	int location = 0;
	CWaitPlayer* p = NULL;
	CWaitPlayer* move = NULL;
	CWaitPlayer* tail = NULL;
	CWaitPlayer* pn = NULL;

	if(!GetHead())
		return ;

	nCount = GetPlayerCount();
	if(nCount < 2)
		return ;

	for(i = 0; i < nShuffleCount; i++)
	{
		location = GetRandom(0, nCount-1);
		move = GetHead();
		for(j = 0; j < location; j++)
		{
			move = move->GetNext();
		}

		if(move == GetHead())
		{
			tail = GetTail();
			SetHead(move->GetNext());
			move->SetNext(NULL);
			if(tail != NULL)
				tail->SetNext(move);
		}
		else
		{
			tail = GetTail();
			if(move != tail)
			{
				pn = GetHead();
				while((p = pn))
				{
					pn = pn->GetNext();
					if(p->GetNext() == move)
						break;
				}
				// p가 move의 prev
				p->SetNext(move->GetNext());
				move->SetNext(NULL);
				if(tail != NULL)
					tail->SetNext(move);
			}
		}
	}
}
//void CRoyalRumble::SetTimeNoticeType(int type)
//{
//	m_nTimeNoticeType = type;
//}
//int CRoyalRumble::GetTimeNoticeType()
//{
//	return m_nTimeNoticeType;
//}
//void CRoyalRumble::SetTimeNotice(time_t t)
//{
//	m_tRoyalRumbleNoticeTime = t;
//}
//time_t CRoyalRumble::GetTimeNotice()
//{
//	return m_tRoyalRumbleNoticeTime;
//}

int	CRoyalRumble::GetRemainWaitTime()
{
	time_t remainTime = GetRoyalRumbleTime() - mktime(&gserver->m_tRealSystemTime);
	int process = GetRoyalRumbleProcess();
	int ret = 0;
	switch(process)
	{
	case 1:
		ret = (15 * 60) + (int)remainTime;
		break;
	case 2:
		ret = (10 * 60) + (int)remainTime;
		break;
	case 3:
		ret = (5 * 60) + (int)remainTime;
		break;
	case 4:
		ret = (int)remainTime;
		break;
	default:
		ret = 0;
		break;
	}
	return ret;
}

void CRoyalRumble::setMinPlayerCount(const int val)
{
	m_nMinPlayerCount = val;
}
void CRoyalRumble::setMaxPlayerCount(const int val)
{
	m_nMaxPlayerCount = val;
}
void CRoyalRumble::setStartHour(const int val)
{
	m_nStartHour = val;
}
int CRoyalRumble::getMinPlayerCount() const
{
	return m_nMinPlayerCount;
}
int CRoyalRumble::getMaxPlayerCount() const
{
	return m_nMaxPlayerCount;
}
int CRoyalRumble::getStartHour() const
{
	return m_nStartHour;
}
void CRoyalRumble::loadRoyalRumbleSetting()
{
	GAMELOG << init("SYSTEM") << "Loading Royal Rumble Setting..." << end;
	CDBCmd cmd;
	cmd.Init(&gserver->m_dbdata);
	cmd.SetQuery("SELECT * FROM t_royalrumble");
	if(!cmd.Open() || !cmd.MoveFirst())
	{
		GAMELOG << init("SYS_WARN") << "t_royalrumble is not exist. Set default value." << end;
		return ;
	}

	int min = 0, max = 0, startHour = 0;

	cmd.GetRec("a_minPlayer", min);
	cmd.GetRec("a_maxPlayer", max);
	cmd.GetRec("a_startHour", startHour);
	if(CRoyalRumble::checkSettingValue(min, max, startHour) != kValueIsValid)
	{
		GAMELOG << init("SYS_WARN") << "Invalid Setting Value" << delim << "min" << delim << min << delim << "max" << delim << max << delim << "startHour" << delim << startHour << end;
		return ;
	}

	setMinPlayerCount(min);
	setMaxPlayerCount(max);
	setStartHour(startHour);
}
int CRoyalRumble::checkSettingValue(const int &min, const int &max, const int &startHour)
{
	/*
	* 조건 1. min은 10보다 크고 50이하인 양수이어야 한다.
	* 조건 2. max는 min 보다 크거나 같고 50이하인 양수이어야 한다.
	* 조건 3. startHour는 0 ~ 23인 양수이어야 한다.
	*/
	if (gserver->barunsongames_flag)
	{
		if(min < 3 || min > 50)
			return kMinValueIsInvalid;
	}
	else
	{
		if(min < 10 || min > 50)
			return kMinValueIsInvalid;
	}

	if(max < min || max > 50)
		return kMaxValueIsInvalid;
	if(startHour < 0 || startHour > 23)
		return kStartHourValueIsInvalid;

	return kValueIsValid;
}
