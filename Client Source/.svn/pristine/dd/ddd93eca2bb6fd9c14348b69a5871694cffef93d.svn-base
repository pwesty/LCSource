
#if !defined(TYPE) || !defined(CNameTableSlot_TYPE) || !defined(CNameTable_TYPE)
#error
#endif

#include <Engine/Templates/StaticArray.h>

class CNameTableSlot_TYPE {
public:
  ULONG nts_ulKey;      // hashing key
  TYPE *nts_ptElement;  // the element inhere
  CNameTableSlot_TYPE(void)  { nts_ptElement = NULL; };
  void Clear(void) { nts_ptElement = NULL; };
};

/*
 * Template class for storing pointers to objects for fast access by name.
 */
class CNameTable_TYPE {
// implementation:
public:
  INDEX nt_ctCompartments;    // number of compartments in table
  INDEX nt_ctSlotsPerComp;    // number of slots in one compartment
  INDEX nt_ctSlotsPerCompStep;    // allocation step for number of slots in one compartment
  CStaticArray<CNameTableSlot_TYPE > nt_antsSlots;  // all slots are here

  // internal finding, returns pointer to the the item
  CNameTableSlot_TYPE *FindSlot(ULONG ulKey, const CTString &strName);
  // internal finding, returns the index of the item in the nametable
  INDEX FindSlotIndex(ULONG ulKey, const CTString &strName);
  // get the name stored in the nametable by it's index
  const CTString GetNameFromIndex(INDEX iIndex);
  // expand the name table to next step
  void Expand(void);

// interface:
public:
  // default constructor
  CNameTable_TYPE(void);
  // destructor -- frees all memory
  ~CNameTable_TYPE(void);
  // remove all slots, and reset the nametable to initial (empty) state
  void Clear(void);

  /* Set allocation parameters. */
  void SetAllocationParameters(
    INDEX ctCompartments, INDEX ctSlotsPerComp, INDEX ctSlotsPerCompStep);

  // find an object by name, return a pointer to it
  TYPE *Find(const CTString &strName);
  // find an object by name, return it's index
  INDEX FindIndex(const CTString &strName);
  // add a new object
  void Add(TYPE *ptNew);
  // remove an object
  void Remove(TYPE *ptOld);

  // remove all objects but keep slots
  void Reset(void);

  // get estimated efficiency of the nametable
  CTString GetEfficiency(void);
};
