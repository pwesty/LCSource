#include "stdh.h"

#include <Engine/Entities/InternalClasses.h>

#include <Engine/Templates/StaticStackArray.cpp>
#include <Engine/World/WorldEntityHashing.h>

#define VALUE_TYPE ULONG
#define TYPE CWorldEntityHashItem
#define CHashTableSlot_TYPE CWorldEntityHashTableSlot
#define CHashTable_TYPE     CWorldEntityHashTable
#include <Engine/Templates/HashTableTemplate.cpp>
#undef VALUE_TYPE
#undef TYPE
#undef CHashTableSlot_TYPE
#undef CHashTable_TYPE

#include <Engine/Network/EMsgBuffer.h>
#include <Engine/Base/Timer.h>

extern FLOAT ser_fPositionTreshold;
extern FLOAT ser_fOrientationTreshold;

CWorldEntityHashItem::CWorldEntityHashItem(CEntity* penEntity)
{
	ASSERT (penEntity != NULL);

	whi_ulEntityID = penEntity->en_ulID;
	whi_penEntityPointer = penEntity;
};


CWorldEntityHash::CWorldEntityHash()
{
	wh_whtHashTable.Clear();
	wh_whtHashTable.SetCallbacks(GetItemKey,GetItemValue);

};

CWorldEntityHash::~CWorldEntityHash()
{
	wh_whtHashTable.Clear();

};

void CWorldEntityHash::SetAllocationParameters(INDEX ctCompartments, INDEX ctSlotsPerComp, INDEX ctSlotsPerCompStep)
{
	wh_whtHashTable.SetAllocationParameters(ctCompartments, ctSlotsPerComp, ctSlotsPerCompStep);
};

void CWorldEntityHash::AddEntity(CEntity* penEntity)
{
	INDEX iEntityIndex = wh_whtHashTable.FindIndex(penEntity->en_ulID);

	//ASSERT (iEntityIndex == -1);
	if (iEntityIndex == -1) {
		CWorldEntityHashItem*  whiNewItem = new CWorldEntityHashItem((CEntity*)penEntity);
		wh_whtHashTable.Add(whiNewItem);
	}
};

void CWorldEntityHash::RemoveEntity(CEntity* penEntity)
{
	CWorldEntityHashItem* whiRemovedItem = wh_whtHashTable.Find(penEntity->en_ulID);

	//ASSERT (ehiRemovedItem != NULL);
	if (whiRemovedItem != NULL) {
		wh_whtHashTable.Remove(whiRemovedItem);
		delete whiRemovedItem;
	}
};


void CWorldEntityHash::ReportEfficiency()
{
	wh_whtHashTable.ReportEfficiency();
};

void CWorldEntityHash::Clear()
{
//안태훈 수정 시작	//(DevPartner Bug Fix)(2005-01-10)
	this->RemoveAll();
//안태훈 수정 끝	//(DevPartner Bug Fix)(2005-01-10)
	wh_whtHashTable.Clear();
};

void CWorldEntityHash::RemoveAll()
{
	if (wh_whtHashTable.ht_ctCompartments<=0 || wh_whtHashTable.ht_ctSlotsPerComp<=0) {
		return;
	}
 
	for (INDEX iComp=0;iComp<wh_whtHashTable.ht_ctCompartments;iComp++) {
		// for each slot in the compartment
		INDEX iSlot = iComp*wh_whtHashTable.ht_ctSlotsPerComp;

		for(INDEX iSlotInComp=0; iSlotInComp<wh_whtHashTable.ht_ctSlotsPerComp; iSlotInComp++, iSlot++) {
			// if it is not empty
			CWorldEntityHashTableSlot &hts = wh_whtHashTable.ht_ahtsSlots[iSlot];
			if (wh_whtHashTable.ht_ahtsSlots[iSlot].hts_ptElement!=NULL) {
				wh_whtHashTable.ht_ahtsSlots[iSlot].hts_ptElement->whi_penEntityPointer = NULL;
				delete wh_whtHashTable.ht_ahtsSlots[iSlot].hts_ptElement;
				wh_whtHashTable.ht_ahtsSlots[iSlot].hts_ptElement = NULL;        
			}
		}
	}
	return;
};

