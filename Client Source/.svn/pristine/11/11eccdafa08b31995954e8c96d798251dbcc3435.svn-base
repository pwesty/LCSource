#include "StdH.h"
#include "Syndicate.h"
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Network/CNetwork.h>
#include <Engine/Network/Server.h>
#include <Engine/Interface/UIManager.h>
#include <Engine/Contents/Base/ChattingUI.h>
#include <Engine/Contents/Base/UICharacterInfoNew.h>
#include <Engine/Contents/Base/UIMySyndicateInfo.h>
#include <Engine/Object/ActorMgr.h>


CSyndicate::CSyndicate()
	: m_pSyndicateInfo(NULL)
	, m_pJewelInfo(NULL)
	, m_pKingInfo(NULL)
	, m_llMaxAccPoint(0)
	, m_lSecessionNas(0)
	, m_bFirstJoin(FALSE)
{
}

CSyndicate::~CSyndicate()
{
	SAFE_DELETE(m_pSyndicateInfo);
	SAFE_DELETE(m_pJewelInfo);
	SAFE_DELETE(m_pKingInfo);

	m_HistoryData.clear();
}

void CSyndicate::RecvSyndicateMsg( CNetworkMessage* istr )
{
	pTypeBase* pPacket = reinterpret_cast<pTypeBase*>(istr->GetBuffer());

	switch(pPacket->subType)
	{
	case MSG_RVR_ERROR_REP:
		ErrMessage(istr);
		break;
	case MSG_RVR_SYNDICATE_INFO_USER_REP:
		RecvUserInfoRep(istr);
		break;
	case MSG_RVR_SYNDICATE_INFO_SERVER:
		RecvUpdateSyndicateInfo(istr);
		break;
	case MSG_UPDATE_RVR_SYNDICATE_TYPE:
		break;
	case MSG_UPDATE_RVR_SYNDICATE_GRADE:
		RecvUpdateGrade(istr);
		break;
	case MSG_UPDATE_RVR_CONTRIBUTIONS_POINT:
		RecvUpdateContributeionPoint(istr);
		break;
	case MSG_UPDATE_SYNDICATE_HISTORY_LOG:
		RecvUpdateHistoryLog(istr);
		break;
	case MSG_UPDATE_SYNDICATE_PERCENTAGE:
		RecvUpdateJewelPercent(istr);
		break;
	case MSG_UPDATE_KINGNAME:
		RecvUpdateKingInfo(istr);
		break;
	case SYSMSG_INCREASE_POINT:
		RecvUpdateIncreasePoint(istr);
		break;
	case MSG_UPDATE_OUT_NEEDNAS:
		RecvUpdateOutNeedNas(istr);
		break;
	}
}

void CSyndicate::RecvUserInfoRep( CNetworkMessage* istr )
{
	ResponseClient::RVRSyndicateInfoOfUser* pRecv = reinterpret_cast<ResponseClient::RVRSyndicateInfoOfUser*>(istr->GetBuffer());

	if (pRecv == NULL)
		return;

	CUIManager* pUimgr = CUIManager::getSingleton();
	CUICharacterInfo* pCharinfo = pUimgr->GetCharacterInfo();

	// 결사대 필수 정보는 MyCharacterInfo에, 나머지는 데이타는 맴버변수에 저장.
	_pNetwork->MyCharacterInfo.iSyndicateType = pRecv->syndicateType;
	_pNetwork->MyCharacterInfo.iSyndicateGrade = pRecv->grade;
	_pNetwork->MyCharacterInfo.lSyndicateAccPoint = pRecv->accumulateContributionPoint;

	m_llMaxAccPoint = pRecv->maxPoint;

	if (pCharinfo != NULL)
	{
		pCharinfo->UpdateSyndicateMark();
		pCharinfo->UpdateMySyndiPos();
	}
}

void CSyndicate::RecvUpdateContributeionPoint( CNetworkMessage* istr )
{
	UpdateClient::SyndicateContributePoint* pRecv = reinterpret_cast<UpdateClient::SyndicateContributePoint*>(istr->GetBuffer());

	if (pRecv == NULL)
		return;

	_pNetwork->MyCharacterInfo.iSyndicateType = pRecv->syndicateType;
	_pNetwork->MyCharacterInfo.lSyndicateAccPoint = pRecv->accumulateContributionPoint;
	m_bFirstJoin = pRecv->isFirstJoin;	// 최초 가입이냐.

	CUIManager* pUimgr = CUIManager::getSingleton();
	CUICharacterInfo* pCharinfo = pUimgr->GetCharacterInfo();

	if (pCharinfo != NULL)
		pCharinfo->UpdateMySyndiPos();
}

void CSyndicate::RecvUpdateHistoryLog( CNetworkMessage* istr )
{
	UpdateClient::SyndicateHistoryData* pRecv = reinterpret_cast<UpdateClient::SyndicateHistoryData*>(istr->GetBuffer());

	if (pRecv == NULL)
		return;

	CUIManager* pUimgr = CUIManager::getSingleton();
	CUICharacterInfo* pCharinfo = pUimgr->GetCharacterInfo();

	int i;

	m_HistoryData.clear();

	for (i = 0; i < pRecv->count; ++i)
	{
		m_HistoryData.push_front( pRecv->data[i] );
	}

	if (m_HistoryData.size() > 0)
		pCharinfo->UpdateSyndiHistory();
}

void CSyndicate::RecvUpdateSyndicateInfo( CNetworkMessage* istr )
{
	UpdateClient::RVRSyndicateInfoOfServer* pRecv = reinterpret_cast<UpdateClient::RVRSyndicateInfoOfServer*>(istr->GetBuffer());

	if (pRecv == NULL)
		return;

	if (m_pSyndicateInfo == NULL)
		m_pSyndicateInfo = new UpdateClient::RVRSyndicateInfoOfServer;	

	memcpy(m_pSyndicateInfo, pRecv, sizeof(UpdateClient::RVRSyndicateInfoOfServer));

	CUIManager* pUimgr = CUIManager::getSingleton();
	CUIMySyndicateInfo* pSyndiinfo = pUimgr->GetMySyndiInfo();

	if (pSyndiinfo != NULL)
		pSyndiinfo->UpdateMyInfo();
}

void CSyndicate::RecvUpdateGrade( CNetworkMessage* istr )
{	// 브로드 캐스팅 정보.
	UpdateClient::SyndicateKind* pRecv = reinterpret_cast<UpdateClient::SyndicateKind*>(istr->GetBuffer());

	if (pRecv == NULL)
		return;

	int nCharIndex = pRecv->charIndex;

	if (_pNetwork->MyCharacterInfo.index == nCharIndex)
	{
		_pNetwork->MyCharacterInfo.iSyndicateType = pRecv->syndicateType;
		_pNetwork->MyCharacterInfo.iSyndicateGrade = pRecv->grade;
		return;
	}

	ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_CHARACTER, nCharIndex);

	if (pObject != NULL)
	{
		CCharacterTarget* pTarget = static_cast< CCharacterTarget* >(pObject);

		pTarget->cha_iSyndicateType = pRecv->syndicateType;
		pTarget->cha_iSyndicateGrade = pRecv->grade;
	}
}

void CSyndicate::RecvUpdateJewelPercent( CNetworkMessage* istr )
{
	UpdateClient::SyndicatePercentageData* pRecv = reinterpret_cast<UpdateClient::SyndicatePercentageData*>(istr->GetBuffer());

	if (pRecv == NULL)
		return;

	if (m_pJewelInfo == NULL)
		m_pJewelInfo = new UpdateClient::SyndicatePercentageData;

	memcpy(m_pJewelInfo, pRecv, sizeof(UpdateClient::SyndicatePercentageData));

	CUIManager* pUimgr = CUIManager::getSingleton();
	CUICharacterInfo* pCharinfo = pUimgr->GetCharacterInfo();

	if (pCharinfo != NULL)
		pCharinfo->UpdateTotalJewel();
}

void CSyndicate::RecvUpdateKingInfo( CNetworkMessage* istr )
{
	UpdateClient::SyndicateKingNameUpdate* pRecv = reinterpret_cast<UpdateClient::SyndicateKingNameUpdate*>(istr->GetBuffer());

	if (pRecv == NULL)
		return;

	if (m_pKingInfo == NULL)
		m_pKingInfo = new UpdateClient::SyndicateKingNameUpdate;

	memcpy(m_pKingInfo, pRecv, sizeof(UpdateClient::SyndicateKingNameUpdate));
}

void CSyndicate::RecvUpdateIncreasePoint( CNetworkMessage* istr )
{
	UpdateClient::SyndicateIncreasePoint* pRecv = reinterpret_cast<UpdateClient::SyndicateIncreasePoint*>(istr->GetBuffer());

	if (pRecv == NULL)
		return;
	
	CTString strMessage;
	CTString strType = GetSyndicateName(pRecv->syndicateType);

	if (pRecv->point > 0)
		strMessage.PrintF(_S(6269, "%s 결사대의 기여도가 %d 상승하였습니다."), strType, pRecv->point);
	else if (pRecv->point < 0)
		strMessage.PrintF(_S(6270, "%s 결사대의 기여도가 %d 하락하였습니다."), strType, pRecv->point);
	
	if (strMessage.IsEmpty() == FALSE)
		CUIManager::getSingleton()->GetChattingUI()->AddSysMessage(strMessage, SYSMSG_NORMAL, DEF_UI_COLOR_WHITE);
}

void CSyndicate::RecvUpdateOutNeedNas( CNetworkMessage* istr )
{
	UpdateClient::SyndicateToNeedNasOfOut* pRecv = reinterpret_cast<UpdateClient::SyndicateToNeedNasOfOut*>(istr->GetBuffer());

	if (pRecv == NULL)
		return;

	m_lSecessionNas = pRecv->needNas;
}

void CSyndicate::SendGiveJewel( int nVNpcIndex, int nVIndexByJewel)
{
	CNetworkMessage nmList;
	RequestClient::RVRGiveJewel* packet = reinterpret_cast<RequestClient::RVRGiveJewel*>(nmList.nm_pubMessage);
	
	packet->type = MSG_RVR;
	packet->subType = MSG_RVR_GIVE_JEWEL_REQ;
	packet->npcIndex = nVNpcIndex;
	packet->vIndexByCreateJewel = nVIndexByJewel;

	nmList.setSize(sizeof(*packet));
	_pNetwork->SendToServerNew(nmList);
}

void CSyndicate::SendSecession( int nVNpcIndex, int nTab, int nInvenIndex )
{
	CNetworkMessage nmList;
	RequestClient::RVRRemoveRequest* packet = reinterpret_cast<RequestClient::RVRRemoveRequest*>(nmList.nm_pubMessage);

	packet->type = MSG_RVR;
	packet->subType = MSG_RVR_REMOVE_REQ;
	packet->tab = nTab;
	packet->invenIndex = nInvenIndex;
	packet->npcIndex = nVNpcIndex;

	nmList.setSize(sizeof(*packet));
	_pNetwork->SendToServerNew(nmList);
}

void CSyndicate::SendSyndicateInfoUser()
{
	CNetworkMessage nmList;
	pTypeBase* packet = reinterpret_cast<pTypeBase*>(nmList.nm_pubMessage);

	packet->type = MSG_RVR;
	packet->subType = MSG_RVR_SYNDICATE_INFO_USER_REQ;

	nmList.setSize(sizeof(*packet));
	_pNetwork->SendToServerNew(nmList);
}

void CSyndicate::SendSyndicateInfo()
{
	CNetworkMessage nmList;
	pTypeBase* packet = reinterpret_cast<pTypeBase*>(nmList.nm_pubMessage);

	packet->type = MSG_RVR;
	packet->subType = MSG_RVR_SYNDICATE_INFO_SERVER_REQ;

	nmList.setSize(sizeof(*packet));
	_pNetwork->SendToServerNew(nmList);
}

void CSyndicate::ErrMessage( CNetworkMessage* istr )
{
	ResponseClient::SyndicateError* pErr = reinterpret_cast<ResponseClient::SyndicateError*>(istr->GetBuffer());

	CTString strMessage;
	CTString strTitle;

	CTString name = GetSyndicateName(pErr->syndicateType);
	
	CUIManager* pUIManager = CUIManager::getSingleton();
	CUIMsgBox_Info	MsgBoxInfo;

	switch (pErr->result)
	{
	case ERROR_SYNDICATE_JOIN_SUCCESS:
		{	// 가입 성공
			strTitle.PrintF(_S(6119, "[%s] 가입 성공"), name);

			if (m_bFirstJoin == true)
			{	// 최초 가입이라면 기여도 추가 지급.
				strMessage.PrintF( _S( 6120, "최초 가입 선물로 기여도 [%I64d]포인트가 지급 되었습니다."), _pNetwork->MyCharacterInfo.lSyndicateAccPoint );
				MsgBoxInfo.AddString( strMessage );
			}

			strMessage.PrintF( _S( 6121, "[%s] 결사대에 가입하신 것을 축하 드리며 앞으로 더욱 더 활발한 활동 부탁 드립니다."), name );
		}
		break;
	case ERROR_SYNDICATE_REMOVE_SUCCESS:
		{	// 탈퇴 성공
			strTitle.PrintF(_S(6123, "결사대 탈퇴 요청"));
			strMessage.PrintF(_S(6128, "[%s] 결사대의 탈퇴가 완료되었습니다. 언제든 재 가입이 가능하니 다음에 다시 찾아주세요."), name);

			// RVR퀘스트 하드코딩. 연관퀘스트 타입이 없기 때문에 퀘스트 상태를 임의로 바꿔줘야함.
			CQuestSystem::Instance().SetQuestAllow(682, CQuestSystem::QAT_ALLOW);
			CQuestSystem::Instance().SetQuestAllow(683, CQuestSystem::QAT_ALLOW);
		}
		break;
	case ERROR_SYNICATE_GIVE_SUCCESS:
			strMessage.PrintF(_S(6116, "[%s] 결사대에 창조의 보석을 기부하였습니다."), name);
		break;
	case ERROR_SYNDICATE_REMOVE_FAIL_NOT_ENOUGH_MONEY:
			strMessage.PrintF( _S( 967, "나스가 부족 합니다.") );	// 나스 부족으로 인한 탈퇴 실패.
		break;
	case ERROR_SYNDICATE_REMOVE_FAIL_ALREADY_EQUIP:
		{
			strTitle.PrintF(_S(6123, "결사대 탈퇴 요청"));

			int index = IsRVRItem();

			if (index > 0)
			{// 결사대 아이템을 하나라도 착용중이라면 에러~
				CTString strWearItem;
				strWearItem = _pNetwork->GetItemName(index);
				strMessage.PrintF(_S(6129, "[%s] 착용하신 결사대 전용 장비를 해제하여 주시기 바랍니다."), strWearItem);
			}
		}
		break;
	case ERROR_SYNDICATE_BUSY:
		{
			// 서버에서 정보를 받을수 없는 상태일 경우 메시지 처리.
			strTitle.PrintF(_S(191, "확인"));
			strMessage.PrintF(_S(6271,"서버로부터 정보를 받을수 없습니다."));
		}
		break;
	}

	// 타이틀이 있다면 메시지 박스 그게 아니면 시스템 메시지로 출력.
	if (strTitle.IsEmpty() == FALSE)
	{
		MsgBoxInfo.SetMsgBoxInfo( strTitle, UMBS_OK, UI_NONE, MSGCMD_NULL);

		MsgBoxInfo.AddString( strMessage );
		pUIManager->CreateMessageBox( MsgBoxInfo );
	}
	else
		_pNetwork->ClientSystemMessage(strMessage, SYSMSG_ERROR);
}

UpdateClient::SyndicateHistoryData::historys* CSyndicate::GetHistoryData( int nIndex )
{
	if (nIndex < 0 || nIndex >= m_HistoryData.size())
		return NULL;

	list_history_iter bIt = m_HistoryData.begin();
	list_history_iter beIt = m_HistoryData.end();

	int i;
	for (i = 0; i < nIndex; ++i)
	{
		if (bIt != beIt)
			++bIt;
	}

	return &(*bIt);
}

CTString CSyndicate::GetGradeName(int nSyndiType, int nGrade)
{
	if (nSyndiType <= 0 || nSyndiType > eTYPE_DEALERMOON)	// 플로르 연맹 또는 무소속은 직위가 없음.
		return CTString("");

	if (nGrade < 1 || nGrade > eGRADE_MAX)
		return CTString("");

	if (nSyndiType == eTYPE_DEALERMOON)
		nGrade += eGRADE_MAX;

	CTString strName;

	strName.PrintF(_S(6135 + (nGrade - 1), "직위"));

	return strName;
}

CTString CSyndicate::GetSyndicateName( int nType )
{
	CTString strName;

	if (nType <= 0 || nType > eTYPE_FLOR)
		return CTString("");

	nType -= 1;	// 0번타입이 무소속이며 무소속은 결사대 이름이 없다.

	strName.PrintF(_S(6132 + nType, "결사대"));

	return strName;
}

int CSyndicate::IsRVRItem()
{
	int i;

	for (i = 0; i < WEAR_TOTAL; ++i)	// 내가 착용하고 있는 아이템중에 결사대 전용 아이템이 있는지 검사.
	{
		CItems* pItem = &_pNetwork->MyWearItem[i];

		if (pItem == NULL)
			continue;

		CItemData* pItemData = pItem->ItemData;

		if (pItemData == NULL)
			continue;

		if (pItemData->GetSyndicateType() > 0)
		{
			return pItemData->GetItemIndex();
		}
	}

	return -1;
}

void CSyndicate::ResetSyndiInfo()
{
	_pNetwork->MyCharacterInfo.iSyndicateType = eTYPE_NONE;
	_pNetwork->MyCharacterInfo.iSyndicateGrade = 0;
	_pNetwork->MyCharacterInfo.lSyndicateAccPoint = 0;

	SAFE_DELETE(m_pSyndicateInfo);
	SAFE_DELETE(m_pJewelInfo);
	SAFE_DELETE(m_pKingInfo);
	m_llMaxAccPoint = 0;
	m_bFirstJoin = FALSE;
	m_lSecessionNas = 0;

	m_HistoryData.clear();
}