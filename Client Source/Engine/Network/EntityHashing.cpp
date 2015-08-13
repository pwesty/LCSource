#include "stdh.h"

#include <Engine/Entities/InternalClasses.h>
#include <Engine/Entities/EntityClass.h>
#include <Engine/Network/CNetwork.h>
#include <Engine/Network/EntityHashing.h>

#define VALUE_TYPE ULONG
#define TYPE CEntityHashItem
#define CHashTableSlot_TYPE CEntityHashTableSlot
#define CHashTable_TYPE     CEntityHashTable
#include <Engine\Templates\HashTableTemplate.cpp>
#undef VALUE_TYPE
#undef TYPE
#undef CHashTableSlot_TYPE
#undef CHashTable_TYPE

#include <Engine\Network\EMsgBuffer.h>
#include <Engine\Base\Timer.h>

extern FLOAT ser_fPositionTreshold;
extern FLOAT ser_fOrientationTreshold;



CEntityHashItem::CEntityHashItem(CEntity* penEntity)
{
	ASSERT (penEntity != NULL);

	ehi_ulEntityID = penEntity->en_ulID;
};

BOOL CEntityHashItem::ClientNeedsUpdate(INDEX iClient,TIME tmNow,FLOAT3D &vBasePosition)
{
	ASSERT ((iClient>=0 && iClient<SERVER_CLIENTS) || (iClient==DEMO_CLIENT));

	// get the entry for specified client
	CClientEntry &ceEntry = ehi_ceClientEntries[iClient];
	// get the extrapolation factor
	FLOAT fLerpFactor = (tmNow - ceEntry.ce_tmLastUpdated) / _pTimer->TickQuantum;

	CEntity* penEntity = _pNetwork->ga_World.EntityFromID(ehi_ulEntityID);
	ASSERT(penEntity!=NULL);
	if (penEntity == NULL) {
		return FALSE;
	}

	// get the current real placement of the entity and the extraplated position from the last sent position
	CPlacement3D &plPlacement = penEntity->en_plPlacement;
	CPlacement3D plLastPlacement,plSpeed;
	plSpeed.pl_OrientationAngle = ceEntry.ce_plLastSentSpeed.pl_OrientationAngle * _pTimer->TickQuantum;
	plSpeed.pl_PositionVector = ceEntry.ce_plLastSentSpeed.pl_PositionVector  * _pTimer->TickQuantum;

	plLastPlacement.Extrapolate(ceEntry.ce_plLastSentPlacement,plSpeed,fLerpFactor);

	FLOAT fPositionDelta;
	FLOAT fOrientationDelta;
	
	// calculate the dead reckoning error
	fPositionDelta = (plLastPlacement.pl_PositionVector - plPlacement.pl_PositionVector).Length();
	fOrientationDelta = (plLastPlacement.pl_OrientationAngle - plPlacement.pl_OrientationAngle).ManhattanNorm();
	// calculate the distance factor - Log2 of distance, '+ 1' is there so Log2 would never return a negative result
	FLOAT fDistanceFactor = (plPlacement.pl_PositionVector - vBasePosition).Length() + 1;
	fDistanceFactor = Log2(fDistanceFactor);
	
	// 3*ser_fPositionTreshold is temporary until replaced with a value dependant on the distance from the player
	BOOL bSendNow = (fPositionDelta >= (fDistanceFactor*ser_fPositionTreshold)) || (fOrientationDelta >= (fDistanceFactor*ser_fOrientationTreshold));

	return bSendNow;
};


void CEntityHashItem::UpdateClient(INDEX iClient,CEntityMessage &emEMessage,FLOAT3D &vBasePosition)
{
	ASSERT ((iClient>=0 && iClient<SERVER_CLIENTS) || (iClient==DEMO_CLIENT));

	CMovableEntity* pmenEntity = (CMovableEntity*)_pNetwork->ga_World.EntityFromID(ehi_ulEntityID);
	ASSERT(pmenEntity!=NULL);
	if (pmenEntity == NULL) {
		return;
	}

	// get the entry for specified client
	CClientEntry &ceEntry = ehi_ceClientEntries[iClient];

	ceEntry.ce_tmLastUpdated = _pTimer->GetLerpedCurrentTick();

	ceEntry.ce_plLastSentPlacement = pmenEntity->en_plPlacement;
	ceEntry.ce_plLastSentSpeed.pl_PositionVector = pmenEntity->en_vCurrentTranslationAbsolute;
	ceEntry.ce_plLastSentSpeed.pl_OrientationAngle = pmenEntity->en_aCurrentRotationAbsolute;

	if (pmenEntity->en_penParent != NULL && pmenEntity->en_penParent->GetPhysicsFlags()&EPF_MOVABLE) {
		ceEntry.ce_plLastSentSpeed.pl_PositionVector += ((CMovableEntity*)(pmenEntity->en_penParent))->en_vCurrentTranslationAbsolute;
		ceEntry.ce_plLastSentSpeed.pl_OrientationAngle += ((CMovableEntity*)(pmenEntity->en_penParent))->en_aCurrentRotationAbsolute;
	}

	// if the speed is very low, clamp it to zero because very low speeds may cause the next placement change 
	// not to qualify for an update - the entity might keep sliding forever
	if (fabs(ceEntry.ce_plLastSentSpeed.pl_PositionVector(1))<ser_fPositionTreshold) ceEntry.ce_plLastSentSpeed.pl_PositionVector(1)= 0.0f;
	if (fabs(ceEntry.ce_plLastSentSpeed.pl_PositionVector(2))<ser_fPositionTreshold) ceEntry.ce_plLastSentSpeed.pl_PositionVector(2)= 0.0f;
	if (fabs(ceEntry.ce_plLastSentSpeed.pl_PositionVector(3))<ser_fPositionTreshold) ceEntry.ce_plLastSentSpeed.pl_PositionVector(3)= 0.0f;
	if (fabs(ceEntry.ce_plLastSentSpeed.pl_OrientationAngle(1))<ser_fPositionTreshold) ceEntry.ce_plLastSentSpeed.pl_OrientationAngle(1)= 0.0f;
	if (fabs(ceEntry.ce_plLastSentSpeed.pl_OrientationAngle(2))<ser_fPositionTreshold) ceEntry.ce_plLastSentSpeed.pl_OrientationAngle(2)= 0.0f;
	if (fabs(ceEntry.ce_plLastSentSpeed.pl_OrientationAngle(3))<ser_fPositionTreshold) ceEntry.ce_plLastSentSpeed.pl_OrientationAngle(3)= 0.0f;

	CPlacement3D plPlacement = ceEntry.ce_plLastSentPlacement;
	plPlacement.pl_PositionVector -= vBasePosition;

	emEMessage.WritePlacement(ehi_ulEntityID,plPlacement,ceEntry.ce_plLastSentSpeed);
};


CEntityHash::CEntityHash()
{
	extern INDEX net_iHashCompartments;
	extern INDEX net_iHashSlotsPerComp;
	extern INDEX net_iHashStep;
	eh_ehtHashTable.Clear();
	eh_ehtHashTable.SetAllocationParameters(net_iHashCompartments,net_iHashSlotsPerComp,net_iHashStep);
	eh_ehtHashTable.SetCallbacks(GetItemKey,GetItemValue);
};


CEntityHash::~CEntityHash()
{
	RemoveAll();
	eh_ehtHashTable.Clear();
};


void CEntityHash::AddEntity(CEntity* penEntity)
{
	INDEX iEntityIndex = eh_ehtHashTable.FindIndex(penEntity->en_ulID);

	//ASSERT (iEntityIndex == -1);
	if (iEntityIndex == -1) {
		CEntityHashItem*  ehiNewItem = new CEntityHashItem((CEntity*)penEntity);
		eh_ehtHashTable.Add(ehiNewItem);
	}
};

void CEntityHash::RemoveEntity(CEntity* penEntity)
{
	CEntityHashItem* ehiRemovedItem = eh_ehtHashTable.Find(penEntity->en_ulID);

	//ASSERT (ehiRemovedItem != NULL);
	if (ehiRemovedItem != NULL) {
		eh_ehtHashTable.Remove(ehiRemovedItem);
		delete ehiRemovedItem;
	}
};


// does a client need an update for this entity? If so, create a seplacement message
BOOL CEntityHash::ClientNeedsUpdate(INDEX iClient,TIME tmNow,ULONG ulEntityID,CEntityMessage &emEMessage,FLOAT3D &vBasePosition)
{
	CEntityHashItem* ehiHashItem = eh_ehtHashTable.Find(ulEntityID);
	// if the entity has been destroyed , it is no longer in the hash table
	if (ehiHashItem==NULL) { 
		return FALSE; 
	}
	if (ehiHashItem->ClientNeedsUpdate(iClient,tmNow,vBasePosition)) {
		ehiHashItem->UpdateClient(iClient,emEMessage,vBasePosition);
		return TRUE;
	}
	return FALSE;
};


// update a client's dead reckoning info for a specific entity
BOOL CEntityHash::UpdateClient(INDEX iClient,ULONG ulEntityID,CEntityMessage &emEMessage,FLOAT3D &vBasePosition)
{
	CEntityHashItem* ehiHashItem = eh_ehtHashTable.Find(ulEntityID);
	// if the entity has been destroyed , it is no longer in the hash table
	if (ehiHashItem==NULL) { 
		return FALSE; 
	}
	ehiHashItem->UpdateClient(iClient,emEMessage,vBasePosition);
	return TRUE;
};


void CEntityHash::ReportEfficiency()
{
	eh_ehtHashTable.ReportEfficiency();
};

void CEntityHash::Clear()
{
	RemoveAll();
	eh_ehtHashTable.Clear();
};

void CEntityHash::RemoveAll()
{
 ASSERT(eh_ehtHashTable.ht_ctCompartments>0 && eh_ehtHashTable.ht_ctSlotsPerComp>0);

	for (INDEX iComp=0;iComp<eh_ehtHashTable.ht_ctCompartments;iComp++) {
		// for each slot in the compartment
		INDEX iSlot = iComp*eh_ehtHashTable.ht_ctSlotsPerComp;

		for(INDEX iSlotInComp=0; iSlotInComp<eh_ehtHashTable.ht_ctSlotsPerComp; iSlotInComp++, iSlot++) {
			// if it is not empty
			CEntityHashTableSlot &hts = eh_ehtHashTable.ht_ahtsSlots[iSlot];
			if (eh_ehtHashTable.ht_ahtsSlots[iSlot].hts_ptElement!=NULL) {
				delete eh_ehtHashTable.ht_ahtsSlots[iSlot].hts_ptElement;
				eh_ehtHashTable.ht_ahtsSlots[iSlot].hts_ptElement = NULL;        
			}
		}
	}
	return;
};

