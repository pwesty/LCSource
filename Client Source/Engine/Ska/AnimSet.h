#ifndef SE_INCL_ANIMSET_H
#define SE_INCL_ANIMSET_H
#ifdef PRAGMA_ONCE
  #pragma once
#endif
#include <Engine/Templates/StaticArray.h>
#include <Engine/Base/Serial.h>
#include <Engine/Math/Vector.h>
#include <Engine/Math/Quaternion.h>

#define ANG_COMPRESIONMUL 182.0416666666667f

#define AN_LOOPING              (1UL<<0) // looping animation
#define AN_NORESTART            (1UL<<1) // dont restart anim
#define AN_PAUSED               (1UL<<2)
#define AN_CLEAR                (1UL<<3) // do new clear state before adding animation
#define AN_CLONE                (1UL<<4) // do new cloned state before adding animation
#define AN_NOGROUP_SORT         (1UL<<5) // dont sort animations by groups
#define AN_FREEZE_ON_BLEND      (1UL<<6) // freeze animations when blending 
#define AN_REMOVE_ON_END        (1UL<<7) // when animation is over disable it in queue
#define AN_FADEOUT              (1UL<<8) // flag is set after anim with AN_REMOVE_ON_END is finished (internal)

#define CLEAR_STATE_LENGTH   0.1f // default clear state length 
#define CLONED_STATE_LENGTH  0.2f // default clone state length

struct AnimPos
{
  UWORD ap_iFrameNum;    // Frame number
  FLOAT3D ap_vPos;       // Bone position
};

struct AnimRotOpt
{
  UWORD aro_iFrameNum;   // Frame number
  UWORD aro_ubH,aro_ubP; // Bone rotation
  ANGLE aro_aAngle;      // Bone rotation
};

struct AnimRot
{
  UWORD ar_iFrameNum;    // Frame number
  FLOATquat3D ar_qRot;   // Bone rotation
};

struct MorphEnvelope
{
  INDEX me_iMorphMapID;            // Morph set id that this envelope animates
  CStaticArray<FLOAT> me_aFactors; // Array of morph factors for morph set
};

struct BoneEnvelope
{
  INDEX be_iBoneID;          // Bone id that this envelope animates
  UWORD be_uwLastFramePos;   // Index of frame stored in last member of AnimPos array (precached)
  UWORD be_uwLastFrameRot;   // Index of frame stored in last member of AnimRot array (precached)
  FLOAT be_fInvFrameStepPos; // be_apPos.Count()-1 / be_uwLastFramePos (precached)
  FLOAT be_fInvFrameStepRot; // be_arRot.Count()-1 / be_uwLastFrameRot (precached)
  FLOAT be_OffSetLen;        // Length of bone position from parent bone position
  CStaticArray<struct AnimPos> be_apPos;       // Array of compresed bone positions
  CStaticArray<struct AnimRot> be_arRot;       // Array of compresed bone rotations
  CStaticArray<struct AnimRotOpt> be_arRotOpt; // Array of optimized compresed bone rotations
};

struct Animation
{
  INDEX an_iID;          // Animation id
  INDEX an_iFrames;      // Frames count
  FLOAT an_fSecPerFrame; // Animation speed
  FLOAT an_fThreshold;   // Optimization threshold
  BOOL  an_bCompresed;   // Are quaternions in animation compresed
  BOOL  an_bCustomSpeed; // Animation has custom speed set in animset list file, witch override speed from anim file
  CStaticArray<struct MorphEnvelope> an_ameMorphs; // Array of morph envelopes
  CStaticArray<struct BoneEnvelope> an_abeBones;   // Array of bone envelopes
  CTString an_fnSourceFile; // Name of ascii aa file, used in Ska studio
};

class ENGINE_API CAnimSet : public CSerial
{
#ifdef KALYDO
  static CTString strDefaultAnimSetPath;
#endif
public:
  // Constructor
  CAnimSet(void);
  // Destructor
  ~CAnimSet(void);
  // Animation optimization
  void Optimize(void);
  void OptimizeAnimation(Animation &an, FLOAT fTreshold);
  // Add new animation to anim set
  void AddAnimation(Animation *pan);
  // Remove animation from animset
  void RemoveAnimation(Animation *pan);
  // Read binary animset
  void Read_t( CTStream *istrFile); // throw char*
  // Write binary animset
  void Write_t( CTStream *ostrFile); // throw char*
  // Clear animset
  void Clear(void);
#ifdef KALYDO
  void Load_t(const CTFileName &fnFileName);
  void Load_Delay_t(const CTFileName &fnFileName);
#endif
  // Get memory used by animset
  SLONG GetUsedMemory(void);
  // Get the description of this object.
  CTString GetDescription(void);

public:
  CStaticArray<struct Animation> as_Anims; // Array of animations
};

// if rotations are compresed does loader also fills array of uncompresed rotations
ENGINE_API void RememberUnCompresedRotatations(BOOL bRemember);
#endif  /* include-once check. */
