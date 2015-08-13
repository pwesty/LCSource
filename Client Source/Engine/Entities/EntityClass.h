#ifndef SE_INCL_ENTITYCLASS_H
#define SE_INCL_ENTITYCLASS_H
#ifdef PRAGMA_ONCE
  #pragma once
#endif

#include <Engine/Base/Serial.h>
#include <Engine/Entities/Entity.h>
#include <Engine/Entities/EntityProperties.h> /* rcg10042001 */


// this struct holds entity class information to be stored in the container
struct EntityClassEntry {
  INDEX ece_iClassID;
  CTFileName ece_fnmEntityClass;

  EntityClassEntry() {
    ece_iClassID = -1;
  };
};
// this container is filled with all used entity classes as they are used in the world
extern CDynamicContainer<EntityClassEntry> _cEntityClassContainer;

// class container manpulation functions
void ClearEntityClassContainer();
EntityClassEntry* FindClassInContainer(INDEX iID);

/*
 *  General structure of an entity class.
 */
class ENGINE_API CEntityClass : public CSerial {
public:
  /* Obtain all components from component table. */
  void ObtainComponents_t(void);  // throw char *
  /* Release all components from component table. */
  void ReleaseComponents(void);
public:
  CTFileName ec_fnmClassDLL;              // filename of the DLL with the class
  HINSTANCE ec_hiClassDLL;                // handle to the DLL with the class
  class CDLLEntityClass *ec_pdecDLLClass; // pointer to DLL class in the DLL

  /* Default constructor. */
  CEntityClass(void);
  /* Constructor for a fixed class. */
  CEntityClass(class CDLLEntityClass *pdecDLLClass);
  /* Destructor. */
  ~CEntityClass(void);
  /* Clear the object. */
  void Clear(void);

  // reference counting functions
  void AddReference(void);
  void RemReference(void);

  /* Check that all properties have been properly declared. */
  void CheckClassProperties(void);

  /* Construct a new member of the class. */
  class CEntity *New(void);

  /* Get pointer to entity property from its name. */
  class CEntityProperty *PropertyForName(const CTString &strPropertyName);
  /* Get pointer to entity property from its packed identifier. */
  class CEntityProperty *PropertyForTypeAndID(ULONG ulType, ULONG ulID);
  /* Get event handler for given state and event code. */
  CEntity::pEventHandler HandlerForStateAndEvent(SLONG slState, SLONG slEvent);
  /* Get pointer to component from its type and identifier. */
  class CEntityComponent *ComponentForTypeAndID(enum EntityComponentType ectType, SLONG slID);
  /* Get pointer to component from the component. */
  class CEntityComponent *ComponentForPointer(void *pv);

  // overrides from CSerial
  /* Read from stream. */
  virtual void Read_t( CTStream *istr);  // throw char *
  /* Write to stream. */
  virtual void Write_t( CTStream *ostr); // throw char *

  // get amount of memory used by this object
  SLONG GetUsedMemory(void);
  // check if this kind of objects is auto-freed
  BOOL IsAutoFreed(void);
  // gather the CRC of the file
  void AddToCRCTable(void);
};

// check if entity is of given class
BOOL ENGINE_API IsOfClass( CEntity *pen, const char *pstrClassName);

inline BOOL ENGINE_API IsOfClass(CEntity *pen, class CDLLEntityClass *pdec)
{
  return pen!=NULL && pen->en_pecClass->ec_pdecDLLClass==pdec;
};

BOOL ENGINE_API IsOfSameClass(CEntity *pen1, CEntity *pen2);
// check if entity is of given class or derived from
BOOL ENGINE_API IsDerivedFromClass( CEntity *pen, const char *pstrClassName);
BOOL ENGINE_API IsDerivedFromClass( CEntity *pen, class CDLLEntityClass *pdec);



struct PlayerControls 
{
	FLOAT fMoveForward;
	FLOAT fMoveBackward;
	FLOAT fMoveLeft;
	FLOAT fMoveRight;
	FLOAT fMoveUp;
	FLOAT fMoveDown;
	
	FLOAT fTurnLeft;
	FLOAT fTurnRight;
	FLOAT fTurnUp;
	FLOAT fTurnDown;
	FLOAT fTurnBankingLeft;
	FLOAT fTurnBankingRight;	
	
	FLOAT fLookLeft;
	FLOAT fLookRight;
	FLOAT fLookUp;
	FLOAT fLookDown;
	FLOAT fLookBankingLeft;
	FLOAT fLookBankingRight;
	
#define CTL_DIGITAL_FIRST bCenterView
	
	BOOL bCenterView;
	//BOOL bSelectWeapon[MAX_WEAPONS+1];
	//BOOL bWeaponNext;
	//BOOL bWeaponPrev;
	//BOOL bWeaponFlip;
	
	BOOL bWalk;
	BOOL bStrafe;
	BOOL bFire;	
	BOOL bUse;
	BOOL b3rdPersonView;	
};

#endif  /* include-once check. */

