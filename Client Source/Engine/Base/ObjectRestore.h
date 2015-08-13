#ifndef SE_INCL_OBJECTRESTORE_H
#define SE_INCL_OBJECTRESTORE_H
#ifdef PRAGMA_ONCE
  #pragma once
#endif

// Sets new value for a objects and restore it at destruction
template<class Type>
class CObjectRestore {
public:
  CObjectRestore(Type &tObject, Type tNewValue) {
    or_ptObject = &tObject;
    or_tValue = tObject;
    tObject = tNewValue;
  };
  ~CObjectRestore() {
    if(or_ptObject!=NULL) {
      *or_ptObject = or_tValue;
    }
  };
public:
  Type *or_ptObject;
  Type or_tValue;
};

#endif //SE_INCL_OBJECTRESTORE_H
