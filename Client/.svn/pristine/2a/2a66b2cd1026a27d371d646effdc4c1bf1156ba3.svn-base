#ifndef SE_INCL_WORLDENTITYHASHING_H
#define SE_INCL_WORLDENTITYHASHING_H
#ifdef PRAGMA_ONCE
  #pragma once
#endif

#include <Engine\Base\Types.h>
#include <Engine\Entities\EntityPointer.h>
#include <Engine\Entities\Entity.h>
#include <Engine\Math\Placement.h>
#include <Engine\Network\CommunicationInterface.h>
#include <Engine\Network\EMsgBuffer.h>


// this is an item in the hash table - one per each movable etity
class CWorldEntityHashItem {
// implementation
public:
  ULONG whi_ulEntityID;                         // id of the entity - also used as hash key
  CEntity* whi_penEntityPointer;           // pointer to the entity  - for fast access
  CWorldEntityHashItem() {whi_ulEntityID = -1; whi_penEntityPointer = NULL;} 
  CWorldEntityHashItem(CEntity* penEntity);

  ~CWorldEntityHashItem() {whi_ulEntityID = -1; whi_penEntityPointer = NULL;}; 
};


// defines neccessary to create an entity hash table from the template
#define VALUE_TYPE ULONG
#define TYPE CWorldEntityHashItem
#define CHashTableSlot_TYPE CWorldEntityHashTableSlot
#define CHashTable_TYPE     CWorldEntityHashTable
#include <Engine\Templates\HashTableTemplate.h>
#undef VALUE_TYPE
#undef TYPE
#undef CHashTableSlot_TYPE
#undef CHashTable_TYPE


// this is the interface for the hash table
class CWorldEntityHash {
// implementation
public:
  CWorldEntityHashTable wh_whtHashTable;

  CWorldEntityHash();  
  ~CWorldEntityHash();

  void SetAllocationParameters(INDEX ctCompartments, INDEX ctSlotsPerComp, INDEX ctSlotsPerCompStep);

  // callbacks for hashing key and value of the item
  static ULONG GetItemKey(ULONG &ulEntityID) {return ulEntityID;}
  static ULONG GetItemValue(CWorldEntityHashItem* whiItem) {return whiItem->whi_ulEntityID;}

// interface
public:
  // add an entity to the hash table
  void AddEntity(CEntity* penEntity);
  // remove an entity to the hash table
  void RemoveEntity(CEntity* penEntity);
  // get the pointer to the entity with a given id
  __forceinline CEntity* GetEntity(ULONG ulEntityID);

  // report hash table stats - number and size of slots, number of items, average number of item s per slot and
  // standard deviation of number of items per slot (to estimate quality of hashing key)
  void ReportEfficiency();
  // clear table
  void Clear();
  void RemoveAll();

};

// get the pointer to the entity with a given id
__forceinline CEntity* CWorldEntityHash::GetEntity(ULONG ulEntityID)
{
  CWorldEntityHashItem* whiItem = wh_whtHashTable.Find(ulEntityID);

  if (whiItem == NULL) {
    return NULL;
  }

  return (CEntity*) whiItem->whi_penEntityPointer;

};

#endif // include
