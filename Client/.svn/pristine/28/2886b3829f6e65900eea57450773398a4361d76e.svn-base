#include "StdH.h"
#include "Durability.h"
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Network/CNetwork.h>
#include <Engine/Network/Server.h>
#include <Engine/Interface/UIManager.h>
#include <Engine/Contents/Base/UIDurability.h>


CDurability::CDurability()
{
}

CDurability::~CDurability()
{
}

void CDurability::RecvDurabilityMsg( CNetworkMessage* istr )
{
	pTypeBase* pPacket = reinterpret_cast<pTypeBase*>(istr->GetBuffer());

	switch(pPacket->subType)
	{
	case MSG_SUB_DURABILITY_NORMAL_REPAIR:		// 내구도 일반 수리
		RecvRepair(istr);
		break;
	case MSG_SUB_DURABILITY_SPECIAL_REPAIR:		// 내구도 특수 수리
		RecvRepairSpecial(istr);
		break;
	case MSG_SUB_DURABILITY_RECOVERY:			// 내구도 복구
		RecvRecovery(istr);
		break;
	}
}

void CDurability::RecvRepair( CNetworkMessage* istr )
{
#ifdef DURABILITY
	ResponseClient::repairDurability* pRecv = reinterpret_cast<ResponseClient::repairDurability*>(istr->GetBuffer());
	
	if (pRecv == NULL)
		return;

	CUIManager* pUiMgr = CUIManager::getSingleton();
	CUIDurability* pUIptr = pUiMgr->GetDurability();

	if (pUIptr == NULL)
		return;

	if (pRecv->result > ERR_NO_DURABILITY_OK)
	{
		if (pRecv->result == ERR_NO_DURABILITY_DO_NOT_REPAIR) // 수리할 필요가 없음
			pUIptr->ShowMsgBox(CUIDurability::eMSG_ERROR_NOT_DAMAGED_ITEM);
		else if (pRecv->result == ERR_NO_DURABILITY_NOT_ENOUGH_MONEY) // 나스 부족
			pUIptr->ShowMsgBox(CUIDurability::eMSG_ERROR_NOT_ENOUGH_MONEY);

		pUIptr->CheckBtnState(CUIDurability::eICON_ITEM);
	}
#endif	//	DURABILITY
}

void CDurability::RecvRepairSpecial( CNetworkMessage* istr )
{
#ifdef DURABILITY
	ResponseClient::specialRepairDurability* pRecv = reinterpret_cast<ResponseClient::specialRepairDurability*>(istr->GetBuffer());

	if (pRecv == NULL)
		return;

	CUIManager* pUiMgr = CUIManager::getSingleton();
	CUIDurability* pUIptr = pUiMgr->GetDurability();

	if (pUIptr == NULL)
		return;

	if (pRecv->result > ERR_NO_DURABILITY_OK)
	{
		if (pRecv->result == ERR_NO_DURABILITY_DO_NOT_REPAIR) // 수리할 필요가 없음
			pUIptr->ShowMsgBox(CUIDurability::eMSG_ERROR_NOT_DAMAGED_ITEM);
		else if (pRecv->result == ERR_NO_DURABILITY_NOT_ENOUGH_MONEY) // 나스 부족
			pUIptr->ShowMsgBox(CUIDurability::eMSG_ERROR_NOT_ENOUGH_MONEY);
		else if (pRecv->result == ERR_NO_DURABILITY_NOT_ENOUGH_ITEM_COUNT) // 숯돌 갯수가 부족함
			pUIptr->ShowMsgBox(CUIDurability::eMSG_ERROR_NOT_ENOUGH_ITEM);

		pUIptr->CheckBtnState(CUIDurability::eICON_ITEM);
	}
#endif	//	DURABILITY
}

void CDurability::RecvRecovery( CNetworkMessage* istr )
{
#ifdef	DURABILITY
	ResponseClient::recoveryDurability* pRecv = reinterpret_cast<ResponseClient::recoveryDurability*>(istr->GetBuffer());

	if (pRecv == NULL)
		return;

	CUIManager* pUiMgr = CUIManager::getSingleton();
	CUIDurability* pUIptr = pUiMgr->GetDurability();

	if (pUIptr == NULL)
		return;

	if (pRecv->result > ERR_NO_DURABILITY_OK)
	{
		if (pRecv->result == ERR_NO_DURABILITY_NOT_ENOUGH_ITEM_COUNT) // 망치 갯수가 부족함
			pUIptr->ShowMsgBox(CUIDurability::eMSG_ERROR_NOT_ENOUGH_ITEM);
		else if (pRecv->result == ERR_NO_DURABILITY_TOO_MUSH_ITEM_COUNT) // 망치 갯수가 너무 많습니다.
			pUIptr->ShowMsgBox(CUIDurability::eMSG_ERROR_OVER_RECOVER);
		else if (pRecv->result == ERR_NO_DURABILITY_DO_NOT_RECOVERY) // 복구할 필요가 없음
			pUIptr->ShowMsgBox(CUIDurability::eMSG_ERROR_NO_RECOVER_ITEM);

		pUIptr->CheckBtnState(CUIDurability::eICON_ITEM);
	}
#endif	//	DURABILITY
}

void CDurability::SendRepair( UWORD tab_no, UWORD invenIdx, int virualIdx, LONGLONG fee )
{
#ifdef DURABILITY
	CNetworkMessage nmList;
	RequestClient::repairDurability* packet = reinterpret_cast<RequestClient::repairDurability*>(nmList.nm_pubMessage);

	packet->type = MSG_DURABILITY;
	packet->subType = MSG_SUB_DURABILITY_NORMAL_REPAIR;
	packet->tab_no = tab_no;
	packet->invenIndex = invenIdx;
	packet->virualIndex = virualIdx;
	packet->fee = fee;

	nmList.setSize(sizeof(*packet));
	_pNetwork->SendToServerNew(nmList);
#endif	//	DURABILITY
}

void CDurability::SendRepairSpecial( UWORD tab_item, UWORD invenIdx_item, int virIdx_item, UWORD tab_material, UWORD invenIdx_material, int virIdx_material )
{
#ifdef DURABILITY
	CNetworkMessage nmList;
	RequestClient::specialRepairDurability* packet = reinterpret_cast<RequestClient::specialRepairDurability*>(nmList.nm_pubMessage);

	packet->type = MSG_DURABILITY;
	packet->subType = MSG_SUB_DURABILITY_SPECIAL_REPAIR;
	packet->tab_no = tab_item;
	packet->invenIndex = invenIdx_item;
	packet->virualIndex = virIdx_item;

	packet->material_tab_no = tab_material;
	packet->material_invenIndex = invenIdx_material;
	packet->material_virtualIndex = virIdx_material;

	nmList.setSize(sizeof(*packet));
	_pNetwork->SendToServerNew(nmList);
#endif	//	DURABILITY
}

void CDurability::SendRecovery( UWORD tab_item, UWORD invenIdx_item, int virIdx_item, UWORD tab_material, UWORD invenIdx_material, int virIdx_material, UWORD Count_material )
{
#ifdef DURABILITY
	CNetworkMessage nmList;
	RequestClient::recoveryDurability* packet = reinterpret_cast<RequestClient::recoveryDurability*>(nmList.nm_pubMessage);

	packet->type = MSG_DURABILITY;
	packet->subType = MSG_SUB_DURABILITY_RECOVERY;
	packet->tab_no = tab_item;
	packet->invenIndex = invenIdx_item;
	packet->virualIndex = virIdx_item;

	packet->material_tab_no = tab_material;
	packet->material_invenIndex = invenIdx_material;
	packet->material_virtualIndex = virIdx_material;
	packet->material_count = Count_material;

	nmList.setSize(sizeof(*packet));
	_pNetwork->SendToServerNew(nmList);
#endif	//	DURABILITY
}

