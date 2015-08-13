#ifndef SE_INCL_ENTITYPOINTER_H
#define SE_INCL_ENTITYPOINTER_H
#ifdef PRAGMA_ONCE
  #pragma once
#endif

/*
 * Smart pointer to entity objects, does the book-keeping for reference counting.
 */
class CEntityPointer {
public:
  CEntity *ep_pen;  // the pointer itself
public:
  // all standard smart pointer functions are defined as inlines in Entity.h
  // (due to strange order of inclusion needed for events and enums)
  inline CEntityPointer(void);
  inline ~CEntityPointer(void);
  inline CEntityPointer(const CEntityPointer &penOther);
  inline CEntityPointer(CEntity *pen);
  inline const CEntityPointer &operator=(CEntity *pen);
  inline const CEntityPointer &operator=(const CEntityPointer &penOther);
  inline CEntity* operator->(void) const;
  inline operator CEntity*(void) const;
  inline CEntity& operator*(void) const;
};


/*
 * This class substitutes pointers to entities, keeping all the smart pointer functionality
 */
class ENGINE_API CEntityID {
public:
  ULONG eid_ulEntityID; //

  // definition of functions is in Entity.cpp
  CEntityID(void);
  ~CEntityID(void);
  CEntityID(const CEntityID &eidOther);
  CEntityID(CEntity *pen);
  const CEntityID &operator=(CEntity *pen);
  const CEntityID &operator=(CEntityID &eidOther);
  const CEntityID &operator=(ULONG ulEntityID);
  CEntity* operator->(void);
  operator CEntity*(void) const;
  CEntity& operator*(void);

  // do not call his function from outside this class, use predefined conversions instead
  CEntity* GetEntity();
  // tests if the entity ID is valid this world
  BOOL IsValid();
};
#endif  /* include-once check. */

