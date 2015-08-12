#ifndef SE_INCL_ENTITYHASHING_H
#define SE_INCL_ENTITYHASHING_H
#ifdef PRAGMA_ONCE
  #pragma once
#endif

#include <Engine\Base\Types.h>
#include <Engine\Entities\EntityPointer.h>
#include <Engine\Entities\Entity.h>
#include <Engine\Math\Placement.h>
#include <Engine\Network\CommunicationInterface.h>
#include <Engine\Network\EMsgBuffer.h>




// IMPORTANT!!! EntityHashing is for MovableEntities and derived classes ONLY!!!


// this struct is used to track info per entity, per client
struct CClientEntry {  
//implementation
  TIME  ce_tmLastUpdated;               // tick time of the last update sent
  CPlacement3D ce_plLastSentPlacement;  // last sent placement
  CPlacement3D ce_plLastSentSpeed;      // last sent speed

  // init
  CClientEntry() {
    ce_tmLastUpdated = -1.0f;
    ce_plLastSentPlacement.pl_PositionVector = FLOAT3D(0,0,0);
    ce_plLastSentPlacement.pl_OrientationAngle = ANGLE3D(0,0,0);
    ce_plLastSentSpeed.pl_PositionVector = FLOAT3D(0,0,0);
    ce_plLastSentSpeed.pl_OrientationAngle = ANGLE3D(0,0,0);
  }

};


// this is an item in the hash table - one per each movable etity
class CEntityHashItem {
// implementation
public:
  ULONG ehi_ulEntityID;                         // id of the entity - also used as hash key
  CClientEntry ehi_ceClientEntries[SERVER_CLIENTS+1]; // one bookkeeping entry per client

  CEntityHashItem() {ehi_ulEntityID = -1;} // entity pointer will initialize itself to NULL
  CEntityHashItem(CEntity* penEntity);

  ~CEntityHashItem() {}; // entity poiner will destroy itself and remove the reference
       
// interface
public:
  // does a client need an update for this entity? (due to dead reckoning error)
  BOOL ClientNeedsUpdate(INDEX iClient,TIME tmNow,FLOAT3D &vBasePosition);
  // create a setplacement message for the client
  void UpdateClient(INDEX iClient,CEntityMessage &emEMessage,FLOAT3D &vBasePosition);
};


// defines neccessary to create an entity hash table from the template
#define VALUE_TYPE ULONG
#define TYPE CEntityHashItem
#define CHashTableSlot_TYPE CEntityHashTableSlot
#define CHashTable_TYPE     CEntityHashTable
#include <Engine\Templates\HashTableTemplate.h>
#undef VALUE_TYPE
#undef TYPE
#undef CHashTableSlot_TYPE
#undef CHashTable_TYPE


// this is the interface for the hash table
class CEntityHash {
// implementation
public:
  CEntityHashTable eh_ehtHashTable;

  CEntityHash();
  ~CEntityHash();

  // callbacks for hashing key and value of the item
  static ULONG GetItemKey(ULONG &ulEntityID) {return ulEntityID;}
  static ULONG GetItemValue(CEntityHashItem* ehiItem) {return ehiItem->ehi_ulEntityID;}

// interface
public:
  // does a client need an update for this entity? If so, create a seplacement message
  BOOL ClientNeedsUpdate(INDEX iClient,TIME tmNow,ULONG ulEntityID,CEntityMessage &emEMessage,FLOAT3D &vBasePosition);
  // update a client's dead reckoning info for a specific entity
  BOOL UpdateClient(INDEX iClient,ULONG ulEntityID,CEntityMessage &emEMessage,FLOAT3D &vBasePosition);

  // add an entity to the hash table
  void AddEntity(CEntity* penEntity);
  // remove an entity to the hash table
  void RemoveEntity(CEntity* penEntity);

  // report hash table stats - number and size of slots, number of items, average number of item s per slot and
  // standard deviation of number of items per slot (to estimate quality of hashing key)
  void ReportEfficiency();
  // clear table
  void Clear();
  void RemoveAll();

};


#endif // include
