#include "stdh.h"

#include <Engine/Entities/Entity.h>
#include <Engine/Entities/LastPositions.h>
#include <Engine/Entities/EntityProperties.h>
#include <Engine/Entities/EntityClass.h>
#include <Engine/Base/Translation.h>
#include <Engine/Base/Timer.h>
#include <Engine/Base/Console.h>

#include <Engine/Base/ListIterator.inl>
#include <Engine/Base/ErrorReporting.h>
#include <Engine/World/World.h>
#include <Engine/Math/Float.h>
#include <Engine/Math/Quaternion.h>

#include <Engine/Brushes/BrushArchive.h>
#include <Engine/Terrain/Terrain.h>
#include <Engine/Terrain/TerrainArchive.h>
#include <Engine/Light/LightSource.h>
#include <Engine/Entities/ShadingInfo.h>
#include <Engine/Models/ModelObject.h>
#include <Engine/Ska/ModelInstance.h>
#include <Engine/Sound/SoundObject.h>

#include <Engine/Templates/DynamicContainer.cpp>
#include <Engine/Templates/StaticArray.cpp>
#include <Engine/Templates/Selection.cpp>

#include <Engine/Network/EMsgBuffer.h>
#include <Engine/Network/Server.h>
#include <Engine/Network/CNetwork.h>

class CPointerRemapping {
public:
  CEntity *pr_penOriginal;
  CEntity *pr_penCopy;

  inline void Clear(void) {};
};

static CStaticArray<CPointerRemapping> _aprRemaps;
static BOOL _bRemapPointersToNULLs = TRUE;
extern BOOL _bReinitEntitiesWhileCopying = TRUE;
static BOOL _bMirrorAndStretch = FALSE;
static FLOAT _fStretch = 1.0f;
static enum WorldMirrorType _wmtMirror = WMT_NONE;

// mirror a placement of one entity
static void MirrorAndStretchPlacement(CPlacement3D &pl)
{
  ASSERT(_wmtMirror==WMT_X||_wmtMirror==WMT_Y||_wmtMirror==WMT_Z||_wmtMirror==WMT_NONE);

  // if there should be mirror
  if (_wmtMirror!=WMT_NONE) {
    // make rotation matrix for the placement
    FLOATmatrix3D m;
    MakeRotationMatrix(m, pl.pl_OrientationAngle);
    // get row vectors, with object x flipped
    FLOAT3D vX(-m(1,1),m(1,2),m(1,3));
    FLOAT3D vY(-m(2,1),m(2,2),m(2,3));
    FLOAT3D vZ(-m(3,1),m(3,2),m(3,3));

    // flip needed axis
    switch(_wmtMirror) {
    case WMT_X: 
      pl.pl_PositionVector(1) = -pl.pl_PositionVector(1);
      vX = -vX; 
      break;
    case WMT_Y: 
      pl.pl_PositionVector(2) = -pl.pl_PositionVector(2);
      vY = -vY; 
      break;
    case WMT_Z: 
      pl.pl_PositionVector(3) = -pl.pl_PositionVector(3);
      vZ = -vZ; 
      break;
    default: ASSERT(FALSE);
    }

    // compose matrix back from the vectors
    m(1,1) = vX(1); m(2,1) = vY(1); m(3,1) = vZ(1);
    m(1,2) = vX(2); m(2,2) = vY(2); m(3,2) = vZ(2);
    m(1,3) = vX(3); m(2,3) = vY(3); m(3,3) = vZ(3);
    // decompose matrix into angles
    DecomposeRotationMatrix(pl.pl_OrientationAngle, m);
  }

  pl.pl_PositionVector*=_fStretch;
}

CEntity *CEntity::FindRemappedEntityPointer(CEntity *penOriginal)
{
  // if original is null
  if (penOriginal==NULL) {
    // copy is null
    return NULL;
  }

  // try to find valid remap
  {FOREACHINSTATICARRAY(_aprRemaps, CPointerRemapping, itpr) {
    if (itpr->pr_penOriginal==penOriginal) {
      return itpr->pr_penCopy;
    }
  }}
  // if none found, copy is either null or original
  return _bRemapPointersToNULLs?NULL:penOriginal;
}

/*
 * Copy entity from another entity of same class.
 * NOTES:
 *  - Doesn't copy placement, it must be done on creation.
 *  - Entity must be initialized afterwards.
 */
void CEntity::Copy(CEntity &enOther, ULONG ulFlags)
{
  BOOL bRemapPointers = ulFlags & COPY_REMAP;

  // copy base class data
  en_RenderType     = enOther.en_RenderType;
  en_ulPhysicsFlags = enOther.en_ulPhysicsFlags;
  en_ulCollisionFlags = enOther.en_ulCollisionFlags;
  en_ulFlags        = enOther.en_ulFlags &
    ~(ENF_SELECTED|ENF_FOUNDINGRIDSEARCH|ENF_VALIDSHADINGINFO|ENF_INRENDERING);
  en_ulSpawnFlags   = enOther.en_ulSpawnFlags;

  // if this is a brush
  if ( enOther.en_RenderType == RT_BRUSH || en_RenderType == RT_FIELDBRUSH) {
    // there must be no existing brush
    ASSERT(en_pbrBrush == NULL);
    // create a new empty brush in the brush archive of current world
    en_pbrBrush = en_pwoWorld->wo_baBrushes.ba_abrBrushes.New();
    en_pbrBrush->br_penEntity = this;
    // copy the brush
    if (_bMirrorAndStretch) {
      en_pbrBrush->Copy(*enOther.en_pbrBrush, _fStretch, _wmtMirror!=WMT_NONE);
    } else {
      en_pbrBrush->Copy(*enOther.en_pbrBrush, 1.0f, FALSE);
    }
  // if this is a terrain
  } else if( enOther.en_RenderType == RT_TERRAIN) {
    ASSERT(en_ptrTerrain == NULL);
    // create a new empty terrain in the brush archive of current world
    en_ptrTerrain = en_pwoWorld->wo_taTerrains.ta_atrTerrains.New();
    en_ptrTerrain->tr_penEntity = this;
    // Copy terrain
    TR_CopyTerrain(en_ptrTerrain,enOther.en_ptrTerrain);
    // Update terrain shadowmap
    Matrix12 mTerrain;
    TR_GetMatrixFromEntity(mTerrain,en_ptrTerrain);
    TR_UpdateShadowMap(en_ptrTerrain,mTerrain,FLOATaabbox3D());
  // if this is a model
  } if ( enOther.en_RenderType == RT_MODEL || en_RenderType == RT_EDITORMODEL) {
    // if will not initialize
    if (!(ulFlags&COPY_REINIT)) {
      ASSERT(en_pmoModelObject==NULL);
      ASSERT(en_psiShadingInfo==NULL);
      if(en_pmoModelObject!=NULL) {
        delete en_pmoModelObject;
      }
      if(en_psiShadingInfo!=NULL) {
        delete en_psiShadingInfo;
      }
      // create a new model object
      en_pmoModelObject = new CModelObject;
      en_psiShadingInfo = new CShadingInfo;
      en_ulFlags &= ~ENF_VALIDSHADINGINFO;
      // copy it
      en_pmoModelObject->Copy(*enOther.en_pmoModelObject);
    }
  // if this is ska model
  } else if ( enOther.en_RenderType == RT_SKAMODEL || en_RenderType == RT_SKAEDITORMODEL) {
      ASSERT(en_psiShadingInfo==NULL);
      if(en_psiShadingInfo!=NULL) {
        delete en_psiShadingInfo;
      }
      en_psiShadingInfo = new CShadingInfo;
      en_ulFlags &= ~ENF_VALIDSHADINGINFO;
      ASSERT(en_pmiModelInstance==NULL);
      if(en_pmiModelInstance!=NULL) {
        DeleteModelInstance(en_pmiModelInstance);
      }
      en_pmiModelInstance = CreateModelInstance("");
      // copy it
      GetModelInstance()->Copy(*enOther.GetModelInstance());
  }

  // copy the parent pointer
  if (bRemapPointers) {
    en_penParent = FindRemappedEntityPointer(enOther.en_penParent);
  } else {
    en_penParent = enOther.en_penParent;
  }
  // if the entity has a parent
  if (en_penParent!=NULL) {
    // create relative offset
    en_plRelativeToParent = en_plPlacement;
    en_plRelativeToParent.AbsoluteToRelativeSmooth(en_penParent->en_plPlacement);
    // link to parent
    en_penParent->en_lhChildren.AddTail(en_lnInParent);
  }

  // copy the derived class properties
  CopyEntityProperties(enOther, ulFlags);

}

/*
 * Copy one entity property from property of another entity.
 */
void CEntity::CopyOneProperty( CEntityProperty &epPropertySrc, CEntityProperty &epPropertyDest,
                               CEntity &enOther, ULONG ulFlags)
{
// a helper macro
#define COPYPROPERTY(flag)                                            \
  (flag &)ENTITYPROPERTY(this, epPropertyDest.ep_slOffset, flag)      \
    = (flag &)ENTITYPROPERTY(&enOther, epPropertySrc.ep_slOffset, flag)

  // depending on the property type
  switch (epPropertySrc.ep_eptType) {
  // if it is BOOL
  case CEntityProperty::EPT_BOOL:
    // copy BOOL
    COPYPROPERTY(INDEX);
    break;
  // if it is INDEX
  case CEntityProperty::EPT_INDEX:
  case CEntityProperty::EPT_ENUM:
  case CEntityProperty::EPT_FLAGS:
  case CEntityProperty::EPT_ANIMATION:
  case CEntityProperty::EPT_ILLUMINATIONTYPE:
  case CEntityProperty::EPT_COLOR:
  case CEntityProperty::EPT_ANGLE:
    // copy INDEX
    COPYPROPERTY(INDEX);
    break;
  // if it is FLOAT
  case CEntityProperty::EPT_FLOAT:
  case CEntityProperty::EPT_RANGE:
    // copy FLOAT
    COPYPROPERTY(FLOAT);
    break;
  // if it is STRING
  case CEntityProperty::EPT_STRING:
  case CEntityProperty::EPT_STRINGTRANS:
    // copy STRING
    COPYPROPERTY(CTString);
    break;
  // if it is FILENAME
  case CEntityProperty::EPT_FILENAME:
    // copy FILENAME
    COPYPROPERTY(CTFileName);
    break;
  // if it is FILENAMENODEP
  case CEntityProperty::EPT_FILENAMENODEP:
    // copy FILENAMENODEP
    COPYPROPERTY(CTFileNameNoDep);
    break;
  // if it is FLOATAABBOX3D
  case CEntityProperty::EPT_FLOATAABBOX3D:
    // copy FLOATAABBOX3D
    COPYPROPERTY(FLOATaabbox3D);
    break;
  // if it is FLOATMATRIX3D
  case CEntityProperty::EPT_FLOATMATRIX3D:
    // copy FLOATMATRIX3D
    COPYPROPERTY(FLOATmatrix3D);
    break;
    
  // if it is FLOAT3D
  case CEntityProperty::EPT_FLOAT3D:
    // copy FLOAT3D
    COPYPROPERTY(FLOAT3D);
    break;
  // if it is ANGLE3D
  case CEntityProperty::EPT_ANGLE3D:
    // copy ANGLE3D
    COPYPROPERTY(ANGLE3D);
    break;
  // if it is QUATERNION3D
  case CEntityProperty::EPT_FLOATQUAT3D:
    // copy ANGLE3D
    COPYPROPERTY(FLOATquat3D);
    break;
  // if it is FLOATplane3D
  case CEntityProperty::EPT_FLOATplane3D:
    // copy FLOATplane3D
    COPYPROPERTY(FLOATplane3D);
    break;
  // if it is ENTITYPTR
  case CEntityProperty::EPT_ENTITYPTR:
    // remap and copy the pointer
    if (ulFlags & COPY_REMAP) {
      ENTITYPROPERTY(this, epPropertyDest.ep_slOffset, CEntityPointer) =
        FindRemappedEntityPointer(ENTITYPROPERTY(&enOther, epPropertySrc.ep_slOffset, CEntityPointer));
    // copy CEntityPointer
    } else {
      COPYPROPERTY(CEntityPointer);
    }
    break;
  // if it is MODELOBJECT
  case CEntityProperty::EPT_MODELOBJECT:
    // copy CModelObject
    ENTITYPROPERTY(this, epPropertyDest.ep_slOffset, CModelObject).
      Copy(ENTITYPROPERTY(&enOther, epPropertySrc.ep_slOffset, CModelObject));
    // model objects are not copied, but should be initialized in Main()
    break;
  // if it is MODELINSTANCE
  case CEntityProperty::EPT_MODELINSTANCE:
    // copy CModelInstance
    ENTITYPROPERTY(this, epPropertyDest.ep_slOffset, CModelInstance).
      Copy(ENTITYPROPERTY(&enOther, epPropertySrc.ep_slOffset, CModelInstance));
    // model objects are not copied, but should be initialized in Main()
    break;
  // if it is ANIMOBJECT
  case CEntityProperty::EPT_ANIMOBJECT:
    // copy CAnimObject
    ENTITYPROPERTY(this, epPropertyDest.ep_slOffset, CAnimObject).
      Copy(ENTITYPROPERTY(&enOther, epPropertySrc.ep_slOffset, CAnimObject));
    break;
  // if it is SOUNDOBJECT
  case CEntityProperty::EPT_SOUNDOBJECT:
    {
      // copy CSoundObject
      CSoundObject &so = ENTITYPROPERTY(this, epPropertyDest.ep_slOffset, CSoundObject);
      so.Copy(ENTITYPROPERTY(&enOther, epPropertySrc.ep_slOffset, CSoundObject));
      so.so_penEntity = this;
    }
    break;
  // if it is CPlacement3D
  case CEntityProperty::EPT_PLACEMENT3D:
    // copy CPlacement3D
    COPYPROPERTY(CPlacement3D);
    break;
  default:
    ASSERTALWAYS("Unknown property type");
  }
}

/*
 * Copy entity properties from another entity of same class.
 */
void CEntity::CopyEntityProperties(CEntity &enOther, ULONG ulFlags)
{
  // other entity must have same class
  ASSERT(enOther.en_pecClass == en_pecClass);

  // for all classes in hierarchy of this entity
  for(CDLLEntityClass *pdecDLLClass = en_pecClass->ec_pdecDLLClass;
      pdecDLLClass!=NULL;
      pdecDLLClass = pdecDLLClass->dec_pdecBase) {

    // for all properties
    for(INDEX iProperty=0; iProperty<pdecDLLClass->dec_ctProperties; iProperty++) {
      CEntityProperty &epProperty = pdecDLLClass->dec_aepProperties[iProperty];
      CopyOneProperty( epProperty, epProperty, enOther, ulFlags);
    }
  }
}

/* Copy container of entities from another world to this one and select them. */
void CWorld::CopyEntities(CWorld &woOther, CDynamicContainer<CEntity> &cenToCopy,
  CEntitySelection &senCopied, const CPlacement3D &plOtherSystem)
{
  INDEX ctEntities = cenToCopy.Count();
  if (ctEntities<=0) {
    return;
  }


  ULONG ulCopyFlags = COPY_REMAP;
  if(_bReinitEntitiesWhileCopying) {
    ulCopyFlags|=COPY_REINIT;
  };

  // create array of pointer remaps
  _aprRemaps.Clear();
  _aprRemaps.New(ctEntities);


  // PASS 1: create entities

  // for each entity to copy
  INDEX iRemap = 0;
  {FOREACHINDYNAMICCONTAINER(cenToCopy, CEntity, itenToCopy) {
    CEntity &enToCopy = *itenToCopy;

    CEntity *penNew;
    CPlacement3D plEntity;
    // thansform the entity placement from the system of other world
    plEntity = enToCopy.en_plPlacement;
    plEntity.RelativeToAbsolute(plOtherSystem);

    // mirror and stretch placement if needed
    if (_bMirrorAndStretch) {
      MirrorAndStretchPlacement(plEntity);
    }

    /*
     * NOTE: We must use CreateEntity_t() overload with class name instead with class pointer
     * because the entity class must be obtained by the target world too!
     */
    // try to
    try {
      // create an entity of same class as the one to copy
      penNew = CreateEntity_t(plEntity, enToCopy.en_pecClass->GetName());
    // if not successfull
    } catch (char *strError) {
      (void)strError;
      ASSERT(FALSE);    // this should not happen
      FatalError(TRANS("Cannot CopyEntity():\n%s"), strError);
    }

    // remember its remap pointer
    _aprRemaps[iRemap].pr_penOriginal = &enToCopy;
    _aprRemaps[iRemap].pr_penCopy = penNew;
    iRemap++;
  }}

  // PASS 2: copy properties

  // for each of the created entities
  {FOREACHINSTATICARRAY(_aprRemaps, CPointerRemapping, itpr) {
    CEntity *penOriginal = itpr->pr_penOriginal;
    CEntity *penCopy = itpr->pr_penCopy;

    // copy the entity from its original
    penCopy->Copy(*penOriginal, ulCopyFlags);
    // if this is a brush
    if ( penOriginal->en_RenderType == CEntity::RT_BRUSH ||
         penOriginal->en_RenderType == CEntity::RT_FIELDBRUSH) {
      // update the bounding boxes of the brush
      penCopy->en_pbrBrush->CalculateBoundingBoxes();
    }
    if (_bMirrorAndStretch) {
      penCopy->MirrorAndStretch(_fStretch, _wmtMirror!=WMT_NONE);
    }
  }}

  // PASS 3: initialize

  // for each of the created entities
  {FOREACHINSTATICARRAY(_aprRemaps, CPointerRemapping, itpr) {
    CEntity *penOriginal = itpr->pr_penOriginal;
    CEntity *penCopy = itpr->pr_penCopy;
    if (_bReinitEntitiesWhileCopying) {
      // init the new copy
      penCopy->End();
      penCopy->Initialize();
    } else {
      penCopy->UpdateSpatialRange();
      penCopy->FindCollisionInfo();
      // set spatial clasification
      penCopy->FindSectorsAroundEntity();
    }
  }}

  // PASS 4: find shadows

  // for each of the created entities
  {FOREACHINSTATICARRAY(_aprRemaps, CPointerRemapping, itpr) {
    CEntity *penOriginal = itpr->pr_penOriginal;
    CEntity *penCopy = itpr->pr_penCopy;

    // if this is a brush
    if ( penCopy->en_RenderType == CEntity::RT_BRUSH ||
         penCopy->en_RenderType == CEntity::RT_FIELDBRUSH) {
      // find possible shadow layers near affected area
      FindShadowLayers(penCopy->en_pbrBrush->GetFirstMip()->bm_boxBoundingBox);
    }

    // if this is a light source
    {CLightSource *pls = penCopy->GetLightSource();
    if (pls!=NULL) {
      // find all shadow maps that should have layers from this light source
      pls->FindShadowLayers(FALSE);
      // update shadow map on terrains
      pls->UpdateTerrains();
    }}

    // select it
    senCopied.Select(*penCopy);
  }}

  // make sure someone doesn't reuse the remap array accidentially
  _aprRemaps.Clear();
}

/* Copy one entity from another world into this one. */
CEntity *CWorld::CopyOneEntity(CEntity &enToCopy, const CPlacement3D &plOtherSystem)
{
  // prepare container for copying
  CDynamicContainer<CEntity> cenToCopy;
  cenToCopy.Add(&enToCopy);
  // copy the entities in container
  CEntitySelection senCopied;
  CopyEntities(*enToCopy.en_pwoWorld, cenToCopy, senCopied, plOtherSystem);

  {FOREACHINDYNAMICCONTAINER(senCopied, CEntity, itenCopied) {
    return itenCopied;
  }}
  ASSERT(FALSE);
  return NULL;
}

/*
 * Copy all entities except one from another world to this one.
 */
void CWorld::CopyAllEntitiesExceptOne(CWorld &woOther, CEntity &enExcepted,
                          const CPlacement3D &plOtherSystem)
{
  // prepare container for copying, without excepted entity
  CDynamicContainer<CEntity> cenToCopy;
  cenToCopy = woOther.wo_cenEntities;
  cenToCopy.Remove(&enExcepted);
  // copy the entities in container and ignore the selection (we don't need it)
  CEntitySelection senCopied;
  CopyEntities(woOther, cenToCopy, senCopied, plOtherSystem);
  senCopied.Clear();
}


/* Copy entity in world. */
CEntity *CWorld::CopyEntityInWorld(CEntity &enOriginal, const CPlacement3D &plOtherEntity,
  BOOL bWithDescendants /*= TRUE*/, ULONG ulTargetID /* = WLD_AUTO_ENTITY_ID */,BOOL bNetwork /* = TRUE*/)
{
  // new entity
  CEntity *penNew;

  /*
   * NOTE: We must use CreateEntity_t() overload with class name instead with class pointer
   * because the entity class must be obtained by the target world too!
   */
  // try to
  try {
    // create an entity of same class as the one to copy
    // ulTargetID is set to a value diferent than -1 (auto id) only when a client receives 
    // an order from the server to copy an entity to a specific ID
    penNew = CreateEntity_t(plOtherEntity, enOriginal.en_pecClass->GetName(),ulTargetID,FALSE);
  // if not successfull
  } catch (char *strError) {
    (void)strError;
    ASSERT(FALSE);    // this should not happen
    FatalError(TRANS("Cannot CopyEntity():\n%s"), strError);
  }


   // insert the entity create mesage into the server buffer
  if (_pNetwork->IsServer() && bNetwork) {
    UBYTE ubWithDescentants;
    
    if (bWithDescendants) {
      ubWithDescentants = 1;
    } else {
      ubWithDescentants=0;
    }

    extern INDEX net_bReportServerTraffic;
    if (net_bReportServerTraffic) {
      CPrintF("Copied entity: ID: 0x%X, Target: 0x%X Class: %s\n",enOriginal.en_ulID,penNew->en_ulID,enOriginal.en_pecClass->GetName());
    }

    extern CEntityMessage _emEntityMessage;
    _emEntityMessage.WriteEntityCopy(enOriginal.en_ulID,penNew->en_ulID,plOtherEntity,ubWithDescentants);
    _pNetwork->ga_srvServer.SendMessage(_emEntityMessage);
  }


  // copy the entity from its original
  penNew->Copy(enOriginal, COPY_REINIT);
  // if this is a brush
  if ( enOriginal.en_RenderType == CEntity::RT_BRUSH ||
       enOriginal.en_RenderType == CEntity::RT_FIELDBRUSH) {
    // update the bounding boxes of the brush
    penNew->en_pbrBrush->CalculateBoundingBoxes();
  }
  // init the new copy
  penNew->End();
  penNew->Initialize(EVoid(),FALSE);


  // if this is a brush
  if ( penNew->en_RenderType == CEntity::RT_BRUSH ||
       penNew->en_RenderType == CEntity::RT_FIELDBRUSH) {
    // find possible shadow layers near affected area
    FindShadowLayers(penNew->en_pbrBrush->GetFirstMip()->bm_boxBoundingBox);
  }

  // if this is a light source
  {CLightSource *pls = penNew->GetLightSource();
  if (pls!=NULL) {
    // find all shadow maps that should have layers from this light source
    pls->FindShadowLayers(FALSE);
  }}

  extern BOOL _bWorldEditorApp;
    // if descendants should be copied too
  if (bWithDescendants) {
    // for each child of this entity
    {FOREACHINLIST(CEntity, en_lnInParent, enOriginal.en_lhChildren, itenChild) {
      // copy it relatively to the new entity
      CPlacement3D plChild = itenChild->en_plRelativeToParent;
      plChild.RelativeToAbsoluteSmooth(penNew->en_plPlacement);
      CEntity *penNewChild;
      if (!bNetwork) {
        penNewChild = CopyEntityInWorld(*itenChild, plChild, TRUE,WLD_AUTO_ENTITY_ID,FALSE);
      } else {
        penNewChild = CopyEntityInWorld(*itenChild, plChild, TRUE);
      }
      // add new child to its new parent
      penNewChild->SetParent(penNew);
    }}
  }


  return penNew;
}

// mirror and stretch another world into this one
void CWorld::MirrorAndStretch(CWorld &woOriginal, FLOAT fStretch, enum WorldMirrorType wmt)
{
  _bMirrorAndStretch = TRUE;
  _fStretch = fStretch;
  _wmtMirror = wmt;

  // clear this world
  Clear();

  // make container for copying
  CDynamicContainer<CEntity> cenToCopy;
  cenToCopy = woOriginal.wo_cenEntities;
  // dummy selection for copied entities, we don't need that info
  CEntitySelection senCopied;
  // make mirroring placement
  CPlacement3D plOtherSystem;
  plOtherSystem = CPlacement3D(FLOAT3D(0,0,0), ANGLE3D(0,0,0));
  // copy entities with mirror and stretch
  CopyEntities(woOriginal, cenToCopy, senCopied, plOtherSystem);

  // update all links
  {
    CSetFPUPrecision FPUPrecision(FPT_53BIT);
    wo_baBrushes.LinkPortalsAndSectors();
  }

  _bMirrorAndStretch = FALSE;
}


