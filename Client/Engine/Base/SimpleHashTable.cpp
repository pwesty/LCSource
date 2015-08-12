#include "StdH.h"
#include <Engine/Base/CTString.h>
#include <Engine/Base/Console.h>
#include <Engine/Base/SimpleHashTable.h>
#include <Engine/Templates/StaticStackArray.cpp>

CSimpleHashTable::CSimpleHashTable( BOOL(*pCompareObjects)(INDEX iObject, const void *pObject),
                                    ULONG(*pCalculateHash)(const void *Object),
                                    void(*pRecreateObjectsInTable)(void), 
                                    INDEX iDefaultAllocationStep/*=1000*/)
{
  ASSERT(pCompareObjects!=NULL);
  ASSERT(pCalculateHash!=NULL);
  ASSERT(pRecreateObjectsInTable!=NULL);

  ht_pCompareObjects = pCompareObjects;
  ht_pCalculateHash = pCalculateHash;
  ht_pRecreateObjectsInTable = pRecreateObjectsInTable;
  // Initialize hash compartments
  SetAllocationStep(iDefaultAllocationStep);
  ExpandTable();
}

CSimpleHashTable::~CSimpleHashTable()
{
}

// Expand hash table
void CSimpleHashTable::ExpandTable(void)
{
  ASSERT(this!=NULL);
  ASSERT(GetCompartmentCount()>=0);
  ASSERT(sht_iAllocationStep>0);

#if _DEBUG
  CPrintF("Expanding hash table from %d to %d compartments, while %d slots are used\n",GetCompartmentCount(), GetCompartmentCount()+sht_iAllocationStep, GetUsedCount());
#endif

  // Get new size
  const INDEX ctCompartments = GetCompartmentCount() + sht_iAllocationStep;
  // Expand compartments array
  sht_ahcCompartments.Clear();
  sht_ahcCompartments.New(ctCompartments);
  sht_ctUsedCount = 0;
}

void CSimpleHashTable::SetAllocationStep(INDEX iNewStep)
{
  ASSERT(iNewStep>0);
  ASSERT(this!=NULL);
  sht_iAllocationStep = iNewStep;
}

INDEX CSimpleHashTable::GetCompartmentCount(void)
{
  ASSERT(this!=NULL);
  return sht_ahcCompartments.Count();
}

INDEX CSimpleHashTable::GetUsedCount(void)
{
  ASSERT(this!=NULL);
  return sht_ctUsedCount;
}

// Get first compartment with given hash
INDEX CSimpleHashTable::GetCompartment(ULONG ulHash)
{
  ASSERT(this!=NULL);
  ASSERT(sht_ahcCompartments.Count()>0);
  INDEX ctCompartments = sht_ahcCompartments.Count();
  return ulHash%GetCompartmentCount();
}

INDEX CSimpleHashTable::GetSlotValue(const SimpleHashSlot &shs)
{
  ASSERT(this!=NULL);
  // if invalid hash slot
  if(shs.shs_iCompartment==(-1) || shs.shs_iSlot==(-1)) {
    return -1;
  }

  const SimpleHashCompartment &shc = sht_ahcCompartments[shs.shs_iCompartment];
  return shc.shc_aiSlots[shs.shs_iSlot];
}

void CSimpleHashTable::AddObject(const void *pObject, INDEX iValue)
{
#if _DEBUG
  // make sure that we are not adding object that allready exists
  ASSERT(FindObject(pObject)==(-1));
#endif

  const INDEX ctshc = GetCompartmentCount();
  const ULONG ulHash = ht_pCalculateHash(pObject);
  const INDEX ishcFirst = GetCompartment(ulHash);
  const INDEX ctSlots = ctshc * SLOTS_COUNT;
  const INDEX ctUsedSlots = GetUsedCount();

  BOOL bHashTableFull = FALSE;
  if(((FLOAT)ctUsedSlots)/ctSlots>0.85f) {
    bHashTableFull = TRUE;
  }

  // if hash table isn't full
  if(!bHashTableFull) {
    // for each compartment in table starting from first with calculated hash
    for(INDEX ishc=0;ishc<ctshc;ishc++) {
      // Wrap it up in table
      INDEX iCompartment = (ishc+ishcFirst)%ctshc;
      SimpleHashCompartment &shc = sht_ahcCompartments[iCompartment];

      // for each slot in current compartment
      for(INDEX ics=0;ics<SLOTS_COUNT;ics++) {
        const INDEX iIndex = shc.shc_aiSlots[ics];
        // if slot is empty
        if(iIndex==(-1)) {
          // Set index in found slot
          shc.shc_aiSlots[ics] = iValue;
          sht_ctUsedCount++;
          return;
        }
      }
    }
  }

  // Expand hash table
  ExpandTable();
  // Recreate all objects in hash table
  ht_pRecreateObjectsInTable();
  // Finaly add new object to hash table
  AddObject(pObject,iValue);
}

// Finds object in hash table (returns value stored in slot)
INDEX CSimpleHashTable::FindObject(const void *pObject)
{
  SimpleHashSlot shs = FindObjectSlot(pObject);
  return GetSlotValue(shs);
}

// Finds object in hash table (returns slot location)
SimpleHashSlot CSimpleHashTable::FindObjectSlot(const void *pObject)
{
  const INDEX ctshc = GetCompartmentCount();
  const ULONG ulHash = ht_pCalculateHash(pObject);
  const INDEX ishcFirst = GetCompartment(ulHash);

  // for each compartment in table starting from first with calculated hash
  for(INDEX ishc=0;ishc<ctshc;ishc++) {
    // Wrap it up in table
    INDEX iCompartment = (ishc+ishcFirst)%ctshc;
    const SimpleHashCompartment &shc = sht_ahcCompartments[iCompartment];

    // for each slot in current compartment
    for(INDEX iscs=0;iscs<SLOTS_COUNT;iscs++) {
      SimpleHashSlot shs;
      const INDEX iSlotValue = shc.shc_aiSlots[iscs];
      // if slot isn't used 
      if(iSlotValue==(-1)) {
        // object does not exists
        return SimpleHashSlot(-1,-1);
      }

      // Compare searched object with one in hash table
      shs.shs_iCompartment = iCompartment;
      shs.shs_iSlot = iscs;
      INDEX iObject = GetSlotValue(shs);
      if(ht_pCompareObjects(iObject,pObject)) {
        return shs;
      }
    }
  }

  // no luck
  return SimpleHashSlot(-1,-1);
}

void CSimpleHashTable::DumpHashTableUsage(void *paArray, INDEX iObjectSize, INDEX ctObjects)
{
  ASSERT(ctObjects==GetUsedCount());
  INDEX aiSlotsFill[SLOTS_COUNT+1] = {0};

  const INDEX cthc = GetCompartmentCount();
  const INDEX cths = cthc * SLOTS_COUNT;

  // for each compartment
  INDEX ihc;
  for(ihc=0;ihc<cthc;ihc++) {
    const SimpleHashCompartment &shc = sht_ahcCompartments[ihc];
    INDEX iSlotsUsed = 0;
    // for each slot
    for(INDEX ics=0;ics<SLOTS_COUNT;ics++) {
      if(shc.shc_aiSlots[ics]!=(-1)) {
        // Count used slots in this compartment
        iSlotsUsed++;
      }
    }
    aiSlotsFill[iSlotsUsed]++;
  }

  FLOAT fSlotsUsed = (((FLOAT)GetUsedCount())/cths) * 100.0f;
  CPrintF("\n\n");
  CPrintF("Compartments %d\n",GetCompartmentCount());
  CPrintF("Slots %d/%d (%g%%)\n",GetUsedCount(),cths,fSlotsUsed);
  for(INDEX is=0;is<SLOTS_COUNT+1;is++) {
    CPrintF("Compartments with %d used slots - %d\n",is,aiSlotsFill[is]);
  }

  CPrintF("\n");
  CPrintF("Dumping compartments usage:\n");

  UBYTE *paObjects = (UBYTE*)paArray;

  INDEX ctObjectsInValidCompartmet = 0;
  // for each compartment
  for(ihc=0;ihc<cthc;ihc++) {
    const SimpleHashCompartment &shc = sht_ahcCompartments[ihc];
    // for each slot in compartment
    CTString strSlots;
    strSlots.PrintF("%5d:",ihc);
    for(INDEX ics=0;ics<SLOTS_COUNT;ics++) {
      INDEX iValue = shc.shc_aiSlots[ics];
      if(iValue==(-1)) {
        strSlots += "-";
      } else {
        INDEX iObject = GetSlotValue(SimpleHashSlot(ihc,ics));
        void *pObject = &paObjects[iObject*iObjectSize];
        ULONG ulHash = ht_pCalculateHash(pObject);
        INDEX iCompartment = GetCompartment(ulHash);
        if(iCompartment==ihc) {
          strSlots += "+";
          ctObjectsInValidCompartmet++;
        } else {
          strSlots += "#";
        }
      }
    }
    CPrintF("%s\n",(const char*)strSlots);
  }
  FLOAT fValidComp = (((FLOAT)ctObjectsInValidCompartmet)/GetUsedCount() * 100.0f);
  CPrintF("%d/%d (%g%%) objects are in own compartment\n",ctObjectsInValidCompartmet,GetUsedCount(),fValidComp);
}
