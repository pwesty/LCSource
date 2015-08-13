#include "StdH.h"
#include "ExpressSystem.h"
#include <Engine/Ska/Render.h>
#include <Engine/Interface/UIManager.h>
#include <Engine/Contents/Base/UIExpressSystem.h>
#include <Engine/Network/CNetwork.h>
#include <Engine/Network/Server.h>
#include <Engine/Templates/StaticArray.h>
#include <Engine/Network/MobTarget.h>
#include <Engine/GameDataManager/GameDataManager.h>
#include <Engine/Contents/Base/Auction.h>
#include <Engine/Contents/Base/UIMysteriousBead.h>
#include <Engine/Interface/UIRadar.h>
#include <Engine/Contents/Base/ChattingUI.h>
#include <Engine/Object/ActorMgr.h>

#define NPC_INDEX 1374

ExpressSystem::ExpressSystem(void)
	: m_nNextPage(0)
	, m_nCount(0)
	, m_pExpressList(NULL)
	, m_nCurPage(0)
	, m_npcIndex(0)
	, m_fnpcPosX(0.0f)
	, m_fnpcPosZ(0.0f)
	, m_nNoticeFlag(0)
	, m_bPremiumChar(false)
{
	
}

ExpressSystem::~ExpressSystem(void)
{
	if (m_pExpressList)
		delete [] m_pExpressList;
}

void ExpressSystem::RecvExpressMessage(CNetworkMessage* istr)
{
	CUIManager* pUimanager = CUIManager::getSingleton();

	CUIExpressSystem* pUI = pUimanager->GetLCESystem();

	pTypeBase* pBase = reinterpret_cast<pTypeBase*>(istr->GetBuffer());

	switch(pBase->subType)
	{
	case MSG_SUB_EXPRESS_EXIST:
		{
			ResponseClient::ExpressExist* pRecv = reinterpret_cast<ResponseClient::ExpressExist*>(istr->GetBuffer());
			// 레이더에 알림 실행
			m_nNoticeFlag = pRecv->flag;
			
			if (m_nNoticeFlag > 0)
			{
				pUimanager->GetRadar()->OnExpressNotice();
				ChangeMailNpc(MAIL_NPC_STATE_EXIST);
				
				switch (m_nNoticeFlag)
				{
				case EXPRESS_SEND_TYPE_TRADE_AGENT_NPC_BUY:
					GameDataManager::getSingleton()->GetAuction()->SendFavoriteListReq();
					break;

				case EXPRESS_SEND_TYPE_TRADE_AGENT_NPC_SELL:
				case EXPRESS_SEND_TYPE_TRADE_AGENT_NPC_CANCEL:
				case EXPRESS_SEND_TYPE_TRADE_AGENT_NPC_RETURN:
					GameDataManager::getSingleton()->GetAuction()->SendRegistListReq();
					break;
				}
			}
			else
			{
				pUimanager->GetRadar()->OffExpressNotice();
				ChangeMailNpc(MAIL_NPC_STATE_NONE);
			}
		}
		break;

	case MSG_SUB_EXPREES_LIST:
		{
			ResponseClient::expressList* pRecv = reinterpret_cast<ResponseClient::expressList*>(istr->GetBuffer());

			if (m_nCount != 0)	// 이미 받아놓은 리스트가 있다면 초기화
			{
				ClearList();
				if (pUI)
					pUI->ClearExpress();
			}

			m_nCurPage = pRecv->nowPage;
			m_nNextPage = pRecv->nextPage;
			m_nCount = pRecv->count;
			
			// 다음 페이지 처리			
			pUimanager->GetLCESystem()->SetItemCount( m_nCount );
			
			// 받은 아이템 UI 처리
			if (pRecv->count <= 0)
			{
				if (pRecv->nowPage > 0)
				{
					SubCurrnetPage();
					SendListReq();
					return;					
				}
				else
				{
					m_nNoticeFlag = 0;

					if (pUimanager->GetRadar())
						pUimanager->GetRadar()->OffExpressNotice();

					ChangeMailNpc(MAIL_NPC_STATE_NONE);
				}				
			}
			else
			{
				pUimanager->GetRadar()->OnExpressNotice();
				ChangeMailNpc(MAIL_NPC_STATE_EXIST);
				// 패킷에서 받은 리스트 복사
				m_pExpressList = new ExpressSystemItemInfo[m_nCount];
				memcpy( m_pExpressList, &pRecv->list[0], sizeof(ExpressSystemItemInfo) * m_nCount );
			}
						
			if (pUI)
			{
				pUI->SetLinkData(); // TO.DO 에러 출력

				if (pUI->IsVisible() == FALSE)
				{
					pUI->OpenExpressSystem();					
				}
				else
				{
					pUI->UpdateExpressItemList();
				}
			}
		}
		break;

	case MSG_SUB_EXPRESS_TAKE:
		{
			ResponseClient::expressTake* pRecv = reinterpret_cast<ResponseClient::expressTake*>(istr->GetBuffer());
			if (pRecv->result > 0)
			{
				CTString strMessage;

				// 에러 출력
				switch(pRecv->result)
				{
				case ERR_DB_ERROR:				// 디비 에러
				case ERR_NO_TAKE_ITEM:			// 찾을 물건이 없음
				case ERR_CAN_NOT_CREATE_ITEM:	// 아이템을 생성할 수 업음
				case ERR_INVALID_INFORMATION:	// 아이템(또는 돈)정보가 잘못된 정보
					break;
				case ERR_FULL_INVENTORY:		// 가방이 꽉참
					{
						strMessage.PrintF( _S( 265, "인벤토리 공간이 부족합니다.") );
						_pNetwork->ClientSystemMessage(strMessage, SYSMSG_ERROR);
					}
					break;
				case ERR_NOT_FOUND_NPC:
					{
						strMessage.PrintF( _S( 5913, "수행가능한 NPC가 주위에 존재하지 않습니다.") );
						_pNetwork->ClientSystemMessage(strMessage, SYSMSG_ERROR);
					}
					break;
				}
				// UI 리스트 업데이트
				if ( pUI )
					pUI->UpdateExpressItemList();
				return;
			}
			
			if ( pUI && pUI->IsVisible() )
			{
				SendListReq();
			}
		}
		break;

	case MSG_SUB_EXPRESS_DELETE:
		{
			ResponseClient::expressDelete* pRecv = reinterpret_cast<ResponseClient::expressDelete*>(istr->GetBuffer());
			if (pRecv->result > 0)
			{
				CTString strMessage;

				// 에러 출력
				switch(pRecv->result)
				{
				case ERR_DB_ERROR:				// 디비 에러
				case ERR_NO_TAKE_ITEM:			// 찾을 물건이 없음
				case ERR_CAN_NOT_CREATE_ITEM:	// 아이템을 생성할 수 업음
				case ERR_INVALID_INFORMATION:	// 아이템(또는 돈)정보가 잘못된 정보
				case ERR_FULL_INVENTORY:		// 가방이 꽉참
				case ERR_NOT_FOUND_NPC:
					{
						strMessage.PrintF( _S( 5913, "수행가능한 NPC가 주위에 존재하지 않습니다.") );
						_pNetwork->ClientSystemMessage(strMessage, SYSMSG_ERROR);
					}
					break;	
				}
				// UI 리스트 업데이트
				if ( pUI )
					pUI->UpdateExpressItemList();
				return;
			}

			if ( pUI && pUI->IsVisible() )
			{
				SendListReq();
			}
		}
		break;
	case MSG_SUB_EXPRESS_SEARCH_NICKNAME:
		{
			ResponseClient::express_unsearchable_stonestatus_searchName* pRecv = reinterpret_cast<ResponseClient::express_unsearchable_stonestatus_searchName*>(istr->GetBuffer());
			if( pRecv->result > 0 )
			{
				pUimanager->GetMysteryousBead()->ErrMessage(CUIMysteriousBead::eERR_INVALID_NAME);
				pUimanager->GetMysteryousBead()->SetFoundPlayer(FALSE);
			}
			else
				pUimanager->GetMysteryousBead()->SetFoundPlayer(TRUE);
		}
		break;
	case MSG_SUB_EXPRESS_SEND_ITEM_TO_NICKNAME:
		{
			ResponseClient::express_use_unsearchable_stonestatus* pRecv = reinterpret_cast<ResponseClient::express_use_unsearchable_stonestatus*>(istr->GetBuffer());
			if( pRecv->result > 0 )
			{
				CTString strMessage;
				switch(pRecv->result)
				{
				case ERR_NOT_SEND_ITEM:
						pUimanager->GetMysteryousBead()->ErrMessage(CUIMysteriousBead::eERR_CAN_NOT_REGIST);
					break;
				case ERR_NOT_ENOUGHT_COUNT:
						pUimanager->GetMysteryousBead()->ErrMessage(CUIMysteriousBead::eERR_ITEM_COUNT_OVER);
					break;
				case ERR_NOT_FOUND_NICKNAME_SYS_ERR:
						pUimanager->GetMysteryousBead()->ErrMessage(CUIMysteriousBead::eERR_INVALID_NAME);
					break;
				}
			}
			else
			{
				pUimanager->GetMysteryousBead()->Success();
			}
		}
		break;

	default:
		break;
	}
}
void ExpressSystem::SendListReq()
{
	CNetworkMessage nmList;
	RequestClient::expressList* packet = reinterpret_cast<RequestClient::expressList*>(nmList.nm_pubMessage);
	packet->type = MSG_EXPRESS_SYSTEM;
	packet->subType = MSG_SUB_EXPREES_LIST;
	packet->npcIndex = m_npcIndex;
	packet->pageIndex = m_nCurPage;
	nmList.setSize(sizeof(*packet));
	_pNetwork->SendToServerNew(nmList);
}

void ExpressSystem::SendRecvReq( int nUniIndex )
{
	CNetworkMessage nmList;
	RequestClient::expressTake* packet = reinterpret_cast<RequestClient::expressTake*>(nmList.nm_pubMessage);
	packet->type = MSG_EXPRESS_SYSTEM;
	packet->subType = MSG_SUB_EXPRESS_TAKE;
	packet->npcIndex = m_npcIndex;
	packet->index = nUniIndex;
	nmList.setSize(sizeof(*packet));
	_pNetwork->SendToServerNew(nmList);
}

void ExpressSystem::SendDeleteReq( int nUniIndex )
{
	CNetworkMessage nmList;
	RequestClient::expressDelete* packet = reinterpret_cast<RequestClient::expressDelete*>(nmList.nm_pubMessage);
	packet->type = MSG_EXPRESS_SYSTEM;
	packet->subType = MSG_SUB_EXPRESS_DELETE;
	packet->npcIndex = m_npcIndex;
	packet->index = nUniIndex;
	nmList.setSize(sizeof(*packet));
	_pNetwork->SendToServerNew(nmList);
}

void ExpressSystem::SendPlayerSearchReq( CTString strName )
{
	CNetworkMessage nmList;
	RequestClient::express_unsearchable_stonestatue_searchName* packet = reinterpret_cast<RequestClient::express_unsearchable_stonestatue_searchName*>(nmList.nm_pubMessage);
	packet->type = MSG_EXPRESS_SYSTEM;
	packet->subType = MSG_SUB_EXPRESS_SEARCH_NICKNAME;
	strcpy(packet->nickName, strName.str_String);
	nmList.setSize(sizeof(*packet));
	_pNetwork->SendToServerNew(nmList);
}

void ExpressSystem::SendItemTransReq(int tab, int invenIndex, int virtualIndex, int send_tab
	, int send_invenIndex, int send_virtualIndex, int send_count, CTString strName)
{
	CNetworkMessage nmList;
	RequestClient::express_use_unsearchable_stonestatue* packet = reinterpret_cast<RequestClient::express_use_unsearchable_stonestatue*>(nmList.nm_pubMessage);
	packet->type = MSG_EXPRESS_SYSTEM;
	packet->subType = MSG_SUB_EXPRESS_SEND_ITEM_TO_NICKNAME;
	packet->tab					= (unsigned short)tab;
	packet->invenIndex			= (unsigned short)invenIndex;
	packet->virualIndex			= virtualIndex;

	packet->send_tab			= (unsigned short)send_tab;
	packet->send_invenIndex		= (unsigned short)send_invenIndex;
	packet->send_virutalIndex	= send_virtualIndex;
	packet->itemCount			= send_count;
	strcpy(packet->toNickName, strName.str_String);
	nmList.setSize(sizeof(*packet));
	_pNetwork->SendToServerNew(nmList);
}

ExpressSystemItemInfo* ExpressSystem::getExpressListAt( int idx )
{
	if (idx < 0 || idx >= m_nCount)
		return NULL;

	return &m_pExpressList[idx];
}

void ExpressSystem::AddCurrnetPage()	
{
	if (m_nCurPage < 0)
	{
		m_nCurPage = 0;
	}
	m_nCurPage++;
}
void ExpressSystem::SubCurrnetPage()
{
	if (m_nCurPage < 0)
	{
		m_nCurPage = 0;
	}
	m_nCurPage--;
}

void ExpressSystem::ClearList()
{
	if (m_pExpressList)
	{
		delete [] m_pExpressList;
		m_pExpressList = NULL;
	}
	m_nNextPage = 0;
	m_nCount = 0;
}

void ExpressSystem::SetNPCInfo( UWORD npcIndex, FLOAT fX, FLOAT fZ )
{
	if (m_npcIndex == npcIndex)
		return;
	m_npcIndex	= npcIndex;
	m_fnpcPosX	= fX;
	m_fnpcPosZ	= fZ;
	m_nCurPage	= 0;
	ClearList();
	SendListReq();
}

void ExpressSystem::ClearNPCInfo()
{
	m_npcIndex	= 0;
	m_fnpcPosX	= 0;
	m_fnpcPosZ	= 0;
	m_nCurPage	= 0;
}

bool ExpressSystem::CheckDistance()
{
	if (_pUIBuff->GetMyTimerItemBuffExist(TIMETITEM_TYPE_REMOTE_EXPRESS_SYSTEM) == true || m_bPremiumChar == true)
		return false;

	// Check distance
	FLOAT	fDiffX = _pNetwork->MyCharacterInfo.x - m_fnpcPosX;
	FLOAT	fDiffZ = _pNetwork->MyCharacterInfo.z - m_fnpcPosZ;
	if( fDiffX * fDiffX + fDiffZ * fDiffZ > UI_VALID_SQRDIST )
	{
		return true;
	}
	return false;
}

void ExpressSystem::ChangeMailNpc(eMAIL_NPC_STATE eNpcState)
{
	CTString strBaseEffectName = CTString("cube_mail");
	CTString strChageEffectName = CTString("cube_mail2");

	switch(eNpcState)
	{
	case MAIL_NPC_STATE_NONE:
		{
			ACTORMGR()->ChangeEffect(eOBJ_MOB, NPC_INDEX, strChageEffectName.str_String, strBaseEffectName.str_String);
		}
		break;

	case MAIL_NPC_STATE_EXIST:
		{
			ACTORMGR()->ChangeEffect(eOBJ_MOB, NPC_INDEX, strBaseEffectName.str_String, strChageEffectName.str_String);
		}
		break;

	case MAIL_NPC_STATE_APPEAR:
		{
			if (m_nNoticeFlag > 0)
			{
				ACTORMGR()->ChangeEffect(eOBJ_MOB, NPC_INDEX, strBaseEffectName.str_String, strChageEffectName.str_String);
			}
		}
		break;
	}
}

void ExpressSystem::RemoteitemUse()
{
	ClearNPCInfo();
	ClearList();
	SendListReq();
}
