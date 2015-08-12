#ifndef SE_INCL_SKELETON_H
#define SE_INCL_SKELETON_H
#ifdef PRAGMA_ONCE
  #pragma once
#endif

#include <Engine/Math/Quaternion.h>
#include <Engine/Base/CTString.h>
#include <Engine/Math/Vector.h>
#include <Engine/Math/Placement.h>
#include <Engine/Graphics/Texture.h>
#include <Engine/Templates/DynamicArray.h>
#include <Engine/Templates/StaticArray.h>
#include <Engine/Base/Serial.h>


struct QVect
{
  FLOAT3D vPos;
  FLOATquat3D qRot;
};

#define BONE_HASHSLOTS 3

struct BoneHashSlot
{
  BoneHashSlot() {
    bhs_uwBoneID = -1;
    bhs_uwBoneIndex = -1;
  };

  UWORD bhs_uwBoneID;
  UWORD bhs_uwBoneIndex;
};

struct BoneHashCompartment
{
  BoneHashSlot bhc_bhsBoneHashSlot[BONE_HASHSLOTS];
};

struct ENGINE_API SkeletonLOD
{
  FLOAT slod_fMaxDistance;                        // distance in witch this lod is visible
  CStaticArray<struct SkeletonBone> slod_aBones;  // array of bones for this lod
  CStaticArray<struct BoneHashCompartment> slod_aBoneHashTable; // hash table of bones id
  CTString slod_fnSourceFile;                     // source filename of ascii skleton lod
};

struct ENGINE_API SkeletonBone
{
  INDEX sb_iID;               // ID of bone
  INDEX sb_iParentID;         // ID of parent bone
  Matrix12 sb_mAbsPlacement;  // default bone placement
  QVect sb_qvRelPlacement;    // default bone placement (same as mAbsPlacement)
  FLOAT sb_fOffSetLen;
  FLOAT sb_fBoneLength;       // length of bone
};

class ENGINE_API CSkeleton : public CSerial
{
#ifdef KALYDO
  static CTString strDefaultSkeletonPath;
#endif
public:
  // Constructor
  CSkeleton();
  // Destructor
  ~CSkeleton();

  // Find bone in skeleton lod
  INDEX FindBoneInLOD(INDEX iBoneID,INDEX iSkeletonLod) const;
  // Sorts bones in skeleton so parent bones are allways before child bones in array
  void SortSkeleton(void);
  void SortSkeletonRecursive(INDEX iParentID, INDEX iSkeletonLod);
  // Calculate absolute transformations for all bones in this lod
  void CalculateAbsoluteTransformations(INDEX iSkeletonLod);
  // Add skeleton lod to skeleton
  void AddSkletonLod(SkeletonLOD &slod);
  // Remove skleton lod form skeleton
  void RemoveSkeletonLod(SkeletonLOD *pslodRemove);
#ifdef KALYDO
  void Load_t(const CTFileName &fnFileName);
  void Load_Delay_t(const CTFileName &fnFileName);
//  void Reload();
#endif
  void Read_t( CTStream *istrFile); // throw char *
  void Write_t( CTStream *ostrFile); // throw char *
  void Clear(void);
  SLONG GetUsedMemory(void);
  // Get the description of this object.
  CTString GetDescription(void);

public:
  CStaticArray<struct SkeletonLOD>  skl_aSkeletonLODs; // array of skeleton lods
}; 




#endif  /* include-once check. */

