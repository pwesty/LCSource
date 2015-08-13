#ifndef SE_INCL_TERRAIN_DATA_ARRAY_HPP
#define SE_INCL_TERRAIN_DATA_ARRAY_HPP
#ifdef PRAGMA_ONCE
  #pragma once
#endif

#define TDAF_USEVXBUFFS (1UL<<0) // array is actualy vertex buffer
#define TDAF_WRITEONLY  (1UL<<1) // array is create for writing only
#define TDAF_READWRITE  (1UL<<2) // array is created for reading and writing
#define TDAF_LOCK_READ  (1UL<<3) // array is locked for reading
#define TDAF_LOCK_WRITE (1UL<<4) // array is locked for writing

template<class Type>
class CTRDataArray {
public:
  // Constructor
  inline CTRDataArray(void) {
    tda_ulFlags     = 0;
    tda_ctObjects   = 0;
    tda_ctAllocated = 0;
    tda_slBufferID  = -1;
    tda_paObjects   = NULL;
    tda_slLastUsed  = 0;
  }
  // Destructor
  inline ~CTRDataArray(void) {
    Clear();
  }
  // Allocation of array
  inline void Allocate(INDEX ctObjects, ULONG ulFlags) {
    ASSERT(this!=NULL);
    ASSERT(tda_paObjects==NULL);        // must not be initialize
    ASSERT(ctObjects>0);                // must be valid
    ASSERT(!(ulFlags&TDAF_LOCK_READ));  // must not have lock flag
    ASSERT(!(ulFlags&TDAF_LOCK_WRITE)); // must not have lock flag

    // if array does uses vertex buffers
    if(ulFlags&TDAF_USEVXBUFFS) {
      ASSERTALWAYS("Undone");
    // if plain array
    } else {
      // allocate array
      tda_paObjects = new Type[ctObjects];
    }
    tda_ctObjects   = ctObjects;
    tda_ctAllocated = ctObjects;
    tda_ulFlags     = ulFlags;
  }

  // Lock array
  inline void Lock(ULONG ulFlags) {
    ASSERT(!(tda_ulFlags&TDAF_LOCK_READ));  // must not be locked
    ASSERT(!(tda_ulFlags&TDAF_LOCK_WRITE)); // must not be locked
    ASSERT(ulFlags&TDAF_LOCK_READ || ulFlags&TDAF_LOCK_WRITE); // validate lock flags

    if(tda_ulFlags&TDAF_USEVXBUFFS) {
      ASSERTALWAYS("Undone");
    } else {
      // no lock in software
      NOTHING;
    }
    tda_ulFlags|=ulFlags;
  }

  // Unlock array
  inline void Unlock(void) {
    ASSERT(tda_ulFlags&TDAF_LOCK_READ || tda_ulFlags&TDAF_LOCK_WRITE); // must be locked
    if(tda_ulFlags&TDAF_USEVXBUFFS) {
      ASSERTALWAYS("Undone");
    } else {
      // no unlock in software
      NOTHING;
    }
    tda_ulFlags&=~TDAF_LOCK_READ;
    tda_ulFlags&=~TDAF_LOCK_WRITE;
  }

  // swap two terrain data arrays
  inline void SwapArrays(CTRDataArray<Type> &tdaOther) {
    Swap(tda_ulFlags,    tdaOther.tda_ulFlags);
    Swap(tda_ctObjects,  tdaOther.tda_ctObjects);
    Swap(tda_ctAllocated,tdaOther.tda_ctAllocated);
    Swap(tda_slBufferID, tdaOther.tda_slBufferID);
    Swap(tda_paObjects,  tdaOther.tda_paObjects);
    Swap(tda_slLastUsed, tdaOther.tda_slLastUsed);
    ASSERT(sizeof(tdaOther)==24);
  }

  // Usage count
  inline INDEX Count(void) const{
    return tda_ctObjects;
  }

  inline void Clear(void) {
    ASSERT(!(tda_ulFlags&TDAF_LOCK_READ));  // must not be locked
    ASSERT(!(tda_ulFlags&TDAF_LOCK_WRITE)); // must not be locked

    if(tda_ulFlags&TDAF_USEVXBUFFS) {
      ASSERT(tda_paObjects==NULL); // must not be used
      ASSERTALWAYS("Undone");
    } else {
      if(tda_paObjects!=NULL) {
        delete[] tda_paObjects;
        tda_paObjects = NULL;
      }
    }
    tda_ulFlags     = 0;
    tda_ctObjects   = 0;
    tda_ctAllocated = 0;
  }

  // Random access operator
  inline Type &operator [](INDEX iObject) {
    ASSERT(this!=NULL);                          // must be valid
    ASSERT(tda_ulFlags&TDAF_LOCK_WRITE);         // must be locked for reading
    ASSERT(tda_ulFlags&TDAF_LOCK_READ);          // must be locked for writing
    ASSERT(tda_paObjects!=NULL);                 // must be valid
    ASSERT(iObject>=0 && iObject<tda_ctObjects); // must be in range
    return tda_paObjects[iObject];
  }
  inline const Type &operator[](INDEX iObject) const {
    ASSERT(this!=NULL);                          // must be valid
    ASSERT(tda_ulFlags&TDAF_LOCK_READ);          // must be locked for reading
    ASSERT(tda_paObjects!=NULL);                 // must be valid
    ASSERT(iObject>=0 && iObject<tda_ctObjects); // must be in range
    return tda_paObjects[iObject];
  }
public:
  ULONG  tda_ulFlags;     // array flags
  INDEX  tda_ctObjects;   // Objects count
  INDEX  tda_ctAllocated; // Allocated objects total count
  SLONG  tda_slBufferID;  // Vertex buffer ID
  Type  *tda_paObjects;   // Array of objects
  SLONG  tda_slLastUsed;  // Time of last usage
};

#endif

