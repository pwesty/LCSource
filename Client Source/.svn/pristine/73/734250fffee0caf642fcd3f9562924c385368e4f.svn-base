#ifndef SE_INCL_SIMPLE_HASH_TABLE_H
#define SE_INCL_SIMPLE_HASH_TABLE_H
#ifdef PRAGMA_ONCE
  #pragma once
#endif

#define SLOTS_COUNT 5
#include <Engine/Templates/StaticArray.h>

struct SimpleHashSlot
{
  // Constructors
  SimpleHashSlot(void) {
    shs_iCompartment = -1;
    shs_iSlot = -1;
  };
  SimpleHashSlot(INDEX ihc, INDEX ics) {
    shs_iCompartment = ihc;
    shs_iSlot = ics;
  };

  INDEX shs_iCompartment;
  INDEX shs_iSlot;
};

struct SimpleHashCompartment
{
  // Constructor
  SimpleHashCompartment() {
    // Reset each slot
    for(INDEX is=0;is<SLOTS_COUNT;is++) {
      shc_aiSlots[is] = -1;
    }
  }

  INDEX shc_aiSlots[SLOTS_COUNT]; // Array of slots for hash compartment
};

class CSimpleHashTable
{
public:
  // Constructor for simple hash table
  CSimpleHashTable( BOOL(*pCompareObjects)(INDEX iObject, const void *pObject),
                    ULONG(*pCalculateHash)(const void *Object),
                    void(*pRecreateObjectsInTable)(void), 
                    INDEX iDefaultAllocationStep=1000);
  ~CSimpleHashTable(void);

  // Add new object to hash table
  void AddObject(const void *pObject, INDEX iValue);
  // Find object in hash table (returns value stored in slot) (-1 if object does not exists)
  INDEX FindObject(const void *pObject);
  // Find object in hash table (returns slot location)
  SimpleHashSlot FindObjectSlot(const void *pObject);
  // Get index writen in slot
  INDEX GetSlotValue(const SimpleHashSlot &shs);
  // Expand hash table for one allocation step
  void ExpandTable(void);

  void SetAllocationStep(INDEX iNewStep);
  INDEX GetCompartmentCount(void);
  INDEX GetUsedCount(void);
  INDEX GetCompartment(ULONG ulHash);
  void DumpHashTableUsage(void *paArray, INDEX iObjectSize, INDEX ctObjects);

  // Callback function for object comparing
  BOOL(*ht_pCompareObjects)(INDEX iObject, const void *pObject);
  // Callback function for hash calculation
  ULONG(*ht_pCalculateHash)(const void *pObject);
  // Callback function for table recreation
  void(*ht_pRecreateObjectsInTable)(void);

public:
  INDEX sht_ctUsedCount;      // count of used slots in table
  INDEX sht_iAllocationStep;  // allocation step
  CStaticArray<SimpleHashCompartment> sht_ahcCompartments; // array of compartments
};













#endif // #define SE_INCL_SIMPLE_HASH_TABLE_H
