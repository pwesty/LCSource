#ifndef SE_INCL_REUSABLECONTAINER_H
#define SE_INCL_REUSABLECONTAINER_H
#ifdef PRAGMA_ONCE
  #pragma once
#endif

#include <Engine/Templates/DynamicContainer.cpp>
template<class Type>
class CReusableContainer {
public:
  // Constructor (must specify size of container)
  CReusableContainer(INDEX ctMaxAllocated);
  // Destructor
  ~CReusableContainer(void);
  // Create new object (uses old object if any is free)
  Type *CreateObject(void);
  // Deletes object (marks object as free)
  void DeleteObject(Type *ptObject);
public:
  CDynamicContainer<Type> rc_atObjects;      // Array of allocated objects
  CStaticStackArray<INDEX> rc_aiFreeObjects; // Array of indices of free objects
  INDEX rc_iCreateCalls;                     // How many create calls were made (debug info)
  INDEX rc_iDeleteCalls;                     // How many delete calls were made (debug info)
  INDEX rc_ctOwerfows;                       // How many owerflow objects are allocated now (debug info)
  INDEX rc_ctMaxOwerflows;                   // Peek of owerflow objects (debug info)
  BOOL rc_bNowDeleting;                      // Flag when objects are deleted
private:
  INDEX rc_ctMaxAllocated;                   // Objects count that will not be deleted from array
};

#endif // #ifndef SE_INCL_REUSABLECONTAINER_H
