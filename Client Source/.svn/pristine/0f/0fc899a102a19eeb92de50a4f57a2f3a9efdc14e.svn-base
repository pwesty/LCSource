#include "stdh.h"
#include <Engine/Ska/Skeleton.h>
#include <Engine/Ska/Render.h>
#include <Engine/Ska/StringTable.h>
#include <Engine/Ska/SkaAssert.h>

#include <Engine/Base/Stream.h>
#include <Engine/Math/Projection.h>
#include <Engine/Math/Matrix12.h>
#include <Engine/Graphics/DrawPort.h>
#include <Engine/Templates/DynamicContainer.cpp>

#ifdef KALYDO
#include <Kalydo/KRFReadLib/Include/KRFReadLib.h>
CTString CSkeleton::strDefaultSkeletonPath = "data\\Defaults\\test.bs";
#endif


#define SKELETON_VERSION  6
#define SKELETON_ID       "SKEL"

static inline INDEX FindBoneInHashTable(const SkeletonLOD &slod, INDEX iBoneID);
static void AddBoneToHashTable(SkeletonLOD &slod, INDEX iBone);
static INDEX FindBoneLineary(const CSkeleton &skl, INDEX iSkeletonLod, INDEX iBoneID);

static CStaticArray<struct SkeletonBone> _aSortArray; // Array used for skeleton sorting
static INDEX _ctSortBones = 0;

CSkeleton::CSkeleton()
{
}

CSkeleton::~CSkeleton()
{
}

// Find bone in skeleton lod
INDEX CSkeleton::FindBoneInLOD(INDEX iBoneID,INDEX iSkeletonLod) const
{
#if 1
//  ASSERT(this!=NULL);
  // safe code [11/21/2011 rumist]
  if( this == NULL )
	return -1;

  if(iSkeletonLod<0 || iSkeletonLod>=skl_aSkeletonLODs.Count()) {
    return -1;
  }
  const SkeletonLOD &slod = skl_aSkeletonLODs[iSkeletonLod];
  const INDEX iBoneIndex = FindBoneInHashTable(slod,iBoneID);
  return iBoneIndex;
#else
  ASSERT(iSkeletonLod>=0);
  INDEX ctslods = skl_aSkeletonLODs.Count();
  if(ctslods < 1) return -1;

  SkeletonLOD &slod = skl_aSkeletonLODs[iSkeletonLod];
  INDEX ctb = slod.slod_aBones.Count();
  // for each bone in skeleton
  for(INDEX isb=0;isb<ctb;isb++) {
    // if bone id match
    if(slod.slod_aBones[isb].sb_iID == iBoneID) {
      // return index in array of bones
      return isb;
    }
  }

  ASSERT("Bone in lod not found");
  return -1;
#endif

#if 0
  ASSERT(iSkeletonLod>=0);
  INDEX ctslods = skl_aSkeletonLODs.Count();
  if(ctslods < 1) return -1;

  SkeletonLOD &slod = skl_aSkeletonLODs[iSkeletonLod];
  INDEX ctb = slod.slod_aBones.Count();
  // for each bone in skeleton
  for(INDEX isb=0;isb<ctb;isb++) {
    // if bone id match
    if(slod.slod_aBones[isb].sb_iID == iBoneID) {
      // return index in array of bones
      return isb;
    }
  }

  ASSERT("Bone in lod not found");
  return -1;
#endif
}

// Sorts bones in skeleton so parent bones are allways before child bones in array
void CSkeleton::SortSkeleton(void)
{
  // sort each lod in skeleton
  INDEX ctslods = skl_aSkeletonLODs.Count();
  // for each lod in skeleton
  for(INDEX islod=0;islod<ctslods;islod++) {
    // create SkeletonBones array for sorting array
    _aSortArray.New(skl_aSkeletonLODs[islod].slod_aBones.Count());
    // start sort for parent bone
    SortSkeletonRecursive(-1,islod);
    // just copy sorted array
    skl_aSkeletonLODs[islod].slod_aBones.CopyArray(_aSortArray);
    // clear array
    _aSortArray.Clear();
    // calculate abs transforms for bones in this lod
    CalculateAbsoluteTransformations(islod);
  }
}

// Sort skeleton bones in lod so parent bones are before child bones in array
void CSkeleton::SortSkeletonRecursive(INDEX iParentID, INDEX iSkeletonLod)
{
  // reset global counter for sorted bones if this bone is parent bone
  if(iParentID == (-1)) {
    _ctSortBones = 0;
  }
  SkeletonLOD &slod = skl_aSkeletonLODs[iSkeletonLod];
  INDEX ctsb = slod.slod_aBones.Count();
  // for each bone in lod
  for(INDEX isb=0;isb<ctsb;isb++) {
    SkeletonBone &sb = slod.slod_aBones[isb];
    if(sb.sb_iParentID == iParentID) {
      // just copy data to _aSortArray
      _aSortArray[_ctSortBones].sb_iID = sb.sb_iID;
      _aSortArray[_ctSortBones].sb_iParentID = sb.sb_iParentID;
      _aSortArray[_ctSortBones].sb_fBoneLength = sb.sb_fBoneLength;
      memcpy(&_aSortArray[_ctSortBones].sb_mAbsPlacement,&sb.sb_mAbsPlacement,sizeof(float)*12);
      memcpy(&_aSortArray[_ctSortBones].sb_qvRelPlacement,&sb.sb_qvRelPlacement,sizeof(QVect));
      _aSortArray[_ctSortBones].sb_fOffSetLen = sb.sb_fOffSetLen;
      // increase couter for sorted bones
      _ctSortBones++;
    }
  }

  // sort children bones
  for(INDEX icsb=0;icsb<ctsb;icsb++) {
    SkeletonBone &sb = slod.slod_aBones[icsb];
    if(sb.sb_iParentID == iParentID) {
      SortSkeletonRecursive(sb.sb_iID,iSkeletonLod);
    }
  }
}

// Calculate absolute transformations for all bones in this lod
void CSkeleton::CalculateAbsoluteTransformations(INDEX iSkeletonLod)
{
  INDEX ctbones = skl_aSkeletonLODs[iSkeletonLod].slod_aBones.Count();
  SkeletonLOD &slod = skl_aSkeletonLODs[iSkeletonLod];
  // for each bone
  for(INDEX isb=0; isb<ctbones; isb++) {
    SkeletonBone &sb = slod.slod_aBones[isb];
    INDEX iParentID = sb.sb_iParentID;
    if(iParentID > (-1)) {
      INDEX iParentIndex = FindBoneLineary(*this, iSkeletonLod, iParentID); // hash table not created yet
      SkeletonBone &sbParent = slod.slod_aBones[iParentIndex];
      MatrixMultiplyCP(sb.sb_mAbsPlacement,sbParent.sb_mAbsPlacement,sb.sb_mAbsPlacement);
    }
  }
}

// Add skeleton lod to skeleton
void CSkeleton::AddSkletonLod(SkeletonLOD &slod)
{
  INDEX ctlods = skl_aSkeletonLODs.Count();
  skl_aSkeletonLODs.Expand(ctlods+1);
  skl_aSkeletonLODs[ctlods] = slod;
}

// Remove skleton lod form skeleton
void CSkeleton::RemoveSkeletonLod(SkeletonLOD *pslodRemove)
{
  INDEX ctslod = skl_aSkeletonLODs.Count();
  // create temp space for skeleton lods
  CStaticArray<struct SkeletonLOD> aTempSLODs;
  aTempSLODs.New(ctslod-1);
  INDEX iIndexSrc=0;

  // for each skeleton lod in skeleton
  for(INDEX islod=0;islod<ctslod;islod++) {
    SkeletonLOD *pslod = &skl_aSkeletonLODs[islod];
    // copy all skeleton lods except the selected one
    if(pslod != pslodRemove) {
      aTempSLODs[iIndexSrc] = *pslod;
      iIndexSrc++;
    }
  }
  // copy temp array of skeleton lods back in skeleton
  skl_aSkeletonLODs.CopyArray(aTempSLODs);
  // clear temp skleletons lods array
  aTempSLODs.Clear();
}

// Add skeleton bone to hash table
static void AddBoneToHashTable(SkeletonLOD &slod, INDEX iBone)
{
  const INDEX ctbhc = slod.slod_aBoneHashTable.Count();
  const SkeletonBone &sb = slod.slod_aBones[iBone];
  INDEX ibhcFirst = sb.sb_iID&(ctbhc-1); // Calculate hash compartment
  // for each compartment
  for(INDEX ibhc=0;ibhc<ctbhc;ibhc++) {
    INDEX iCompartment = (ibhc+ibhcFirst)&(ctbhc-1); // %ctbhc
    BoneHashCompartment &bhc = slod.slod_aBoneHashTable[iCompartment];
    // for each slot in compartment
    for(INDEX ibhs=0;ibhs<BONE_HASHSLOTS;ibhs++) {
      BoneHashSlot &bhs = bhc.bhc_bhsBoneHashSlot[ibhs];
      // if slot is empty
      if((SLONG)bhs.bhs_uwBoneIndex == 65535) { // (-1)
        bhs.bhs_uwBoneIndex = iBone;
        bhs.bhs_uwBoneID = sb.sb_iID;
        return;
      }
    }
  }
}

// Find skeleton bone in hash table
static inline INDEX FindBoneInHashTable(const SkeletonLOD &slod, INDEX iBoneID)
{
  const INDEX ctbhc = slod.slod_aBoneHashTable.Count();
  INDEX ibhcFirst = iBoneID&(ctbhc-1); // Calculate hash compartment

  // for each compartment
  for(INDEX ibhc=0;ibhc<ctbhc;ibhc++) {
    INDEX iCompartment = (ibhc+ibhcFirst)&(ctbhc-1);// %ctbhc
    const BoneHashCompartment &bhc = slod.slod_aBoneHashTable[iCompartment];
    // for each slot in compartment
    for(INDEX ibhs=0;ibhs<BONE_HASHSLOTS;ibhs++) {
      const BoneHashSlot &bhs = bhc.bhc_bhsBoneHashSlot[ibhs];
      // if bone was found
      if(bhs.bhs_uwBoneID == iBoneID) {
        return bhs.bhs_uwBoneIndex;
      }
      // if bone does not exists
      if((SLONG)bhs.bhs_uwBoneIndex == 65535) { // (-1)
        return -1;
      }
    }
  }

  ASSERT(FALSE);
  return -1; // should never get here
}

// Create skeleton hash table
static void CreateSkeletonLookupTable(SkeletonLOD &slod)
{
  ASSERT(slod.slod_aBoneHashTable.Count()==0); // must be empty
  // Create hash table (power of 2)
  const INDEX ctsb = slod.slod_aBones.Count();
  const INDEX iTablePow = FastMaxLog2(ctsb);
  const INDEX ctbhc = 1<<iTablePow;
  slod.slod_aBoneHashTable.New(ctbhc);

  // for each skeleton bone
  for(INDEX isb=0;isb<ctsb;isb++) {
    AddBoneToHashTable(slod,isb);
  }
}

static INDEX FindBoneLineary(const CSkeleton &skl, INDEX iSkeletonLod, INDEX iBoneID)
{
  ASSERT(iSkeletonLod>=0);
  INDEX ctslods = skl.skl_aSkeletonLODs.Count();
  if(ctslods < 1) return -1;

  const SkeletonLOD &slod = skl.skl_aSkeletonLODs[iSkeletonLod];
  INDEX ctb = slod.slod_aBones.Count();
  // for each bone in skeleton
  for(INDEX isb=0;isb<ctb;isb++) {
    // if bone id match
    if(slod.slod_aBones[isb].sb_iID == iBoneID) {
      // return index in array of bones
      return isb;
    }
  }

  SKAASSERT("Bone in lod not found");
  return -1;
}

// write to stream
void CSkeleton::Write_t(CTStream *ostrFile)
{
  INDEX ctslods = skl_aSkeletonLODs.Count();
  // write id
  ostrFile->WriteID_t(CChunkID(SKELETON_ID));
  // write version
  (*ostrFile)<<(INDEX)SKELETON_VERSION;
  // write lods count
  (*ostrFile)<<ctslods;
  // for each lod in skeleton
  for(INDEX islod=0;islod<ctslods;islod++) {
    SkeletonLOD &slod = skl_aSkeletonLODs[islod];
    // write source file name
    (*ostrFile)<<slod.slod_fnSourceFile;
    // write MaxDistance
    (*ostrFile)<<slod.slod_fMaxDistance;

    // write bone count
    INDEX ctb = slod.slod_aBones.Count();
    (*ostrFile)<<ctb;
    // write skeleton bones
    for(INDEX ib=0;ib<ctb;ib++) {

      const SkeletonBone &sb = slod.slod_aBones[ib];
      CTString strNameID = ska_IDToString(sb.sb_iID);
      CTString strParentID;
      // if parent bone is valid
      if(sb.sb_iParentID>=0) {
        strParentID = ska_IDToString(sb.sb_iParentID);
      }

      // write ID
      (*ostrFile)<<strNameID;
      // write Parent ID
      (*ostrFile)<<strParentID;
      //(*ostrFile)<<slod.slod_aBones[ib].sb_iParentIndex;
      // write AbsPlacement matrix
      ostrFile->Write_t(&sb.sb_mAbsPlacement,sizeof(FLOAT)*12);
      // write RelPlacement Qvect stuct
      ostrFile->Write_t(&sb.sb_qvRelPlacement,sizeof(QVect));
      // write offset len
      (*ostrFile)<<sb.sb_fOffSetLen;
      // write bone length
      (*ostrFile)<<sb.sb_fBoneLength;
    }
  }
}

#ifdef KALYDO
static void KCPSkeletonDownloaded(const char* fileName, TKResult result, void* id)
{
	switch (result)
	{
	case KR_OK:
		{
	// 		((CSerial*)id)->Clear();
	// 		((CSerial*)id)->Load_t( CTFileName( fileName ) );
			SLS* pSLS = new SLS();
			pSLS->pTarget = reinterpret_cast<CSerial*>(id);
			pSLS->pTargetFilePath = fileName;
			g_deqLoadData.push_back( pSLS );
		}
		break;
	case KR_DOWNLOAD_FAILED:
	case KR_FILE_CORRUPT:
		krfRequestKCPFile(fileName, &KCPSkeletonDownloaded, id);
	//default:
		// unknown error!
	}
}

void CSkeleton::Load_t(const CTFileName &fnFileName)
{
  ASSERT(!IsUsed());
  // mark that you have changed
  MarkChanged();
  // 근데 이게 확실한가?? 호출 매커니즘의 정확한 해명이 필요할 거 같다.
  TKResult tkResult = KR_OK;
  tkResult = krfRequestKCPFile( fnFileName, NULL, NULL );
  // if file exist in local disk.
  if( KR_OK == tkResult )
  {
	// open a stream
	CTFileStream istrFile;
	istrFile.Open_t(fnFileName);
	// read object from stream
	Read_t(&istrFile);
	// if still here (no exceptions raised)
	// remember filename
	ser_FileName = fnFileName;
  }
  else
  {
	CPrintF("Request file to kcp : %s\n", fnFileName );
	//?????????????
	CTFileStream istrFile;
	istrFile.Open_t( strDefaultSkeletonPath );
	Read_t(&istrFile);
	ser_FileName = fnFileName;
	if( tkResult == KR_FILE_NOT_AVAILABLE )
	{
		MarkUsed();
	}
	tkResult = krfRequestKCPFile(fnFileName, &KCPSkeletonDownloaded, this);
	if( KR_FILE_NOT_FOUND == tkResult )
	{
		CPrintF("[Load_t] Skeleton File Not Found in kalydo...\n" );
	}
	else if( KR_IO_PENDING == tkResult )
	{
		CPrintF("[Load_t] Skeleton File already request...\n" );
	}
	else
	{
		;
	}
  }  
}

void CSkeleton::Load_Delay_t(const CTFileName &fnFileName)
{
  // mark that you have changed
  MarkChanged();
  // 근데 이게 확실한가?? 호출 매커니즘의 정확한 해명이 필요할 거 같다.

  //if( kfileExists( fnFileName ) )
	// open a stream
	CTFileStream istrFile;
	istrFile.Open_t(fnFileName);
	// read object from stream
	Read_t(&istrFile);
	// if still here (no exceptions raised)
	// remember filename
	ser_FileName = fnFileName;
	MarkUnused();
}

#endif

// read from stream
void CSkeleton::Read_t(CTStream *istrFile)
{
  INDEX iFileVersion;
  INDEX ctslods;
  // read chunk id
  istrFile->ExpectID_t(CChunkID(SKELETON_ID));
  // check file version
  (*istrFile)>>iFileVersion;
  if(iFileVersion != SKELETON_VERSION) {
		ThrowF_t(TRANS("File '%s'.\nInvalid skeleton file version.\nExpected Ver \"%d\" but found \"%d\"\n"),
      (const char*)istrFile->GetDescription(),SKELETON_VERSION,iFileVersion);
  }
  // read skeleton lod count
  (*istrFile)>>ctslods;

  if(ctslods>0) {
    skl_aSkeletonLODs.New(ctslods);
  }
  CTString strNameID;
  CTString strParentID;
  // for each skeleton lod
  for(INDEX islod=0;islod<ctslods;islod++) {
    SkeletonLOD &slod = skl_aSkeletonLODs[islod];
    // read source file name
    (*istrFile)>>slod.slod_fnSourceFile;
    // read MaxDistance
    (*istrFile)>>slod.slod_fMaxDistance;
    // read bone count
    INDEX ctb;
    (*istrFile)>>ctb;
    // create bone array
    slod.slod_aBones.New(ctb);
    // read skeleton bones
    for(INDEX ib=0;ib<ctb;ib++) {
      SkeletonBone &sb = slod.slod_aBones[ib];
      // read ID
      (*istrFile)>>strNameID;
      // read Parent ID
      (*istrFile)>>strParentID;
      //(*istrFile)>>slod.slod_aBones[ib].sb_iParentIndex ;
      sb.sb_iID = ska_GetIDFromStringTable(strNameID);
      // if no parent bone
      if(strParentID=="") {
        // parent bone is invalid
        sb.sb_iParentID = -1;
      } else {
        // get parent bone id
        sb.sb_iParentID = ska_StringToID(strParentID);
      }
      
      // read AbsPlacement matrix
      istrFile->Read_t(&sb.sb_mAbsPlacement,sizeof(FLOAT)*12);
      // read RelPlacement Qvect stuct
      istrFile->Read_t(&sb.sb_qvRelPlacement,sizeof(QVect));
      // read offset len
      (*istrFile)>>sb.sb_fOffSetLen;
      // read bone length
      (*istrFile)>>sb.sb_fBoneLength;
    }

    // Create lookup table for quicker bone id searching
    CreateSkeletonLookupTable(slod);
  }
}

// Clear skeleton
void CSkeleton::Clear(void)
{
  // for each LOD
  for (INDEX islod=0; islod<skl_aSkeletonLODs.Count(); islod++) {
    // clear bones array
    skl_aSkeletonLODs[islod].slod_aBones.Clear();
  }
  // clear all lods
  skl_aSkeletonLODs.Clear();
}

// Count used memory
SLONG CSkeleton::GetUsedMemory(void)
{
  SLONG slMemoryUsed = sizeof(*this);
  INDEX ctslods = skl_aSkeletonLODs.Count();
  // for each skeleton lods
  for(INDEX islod=0;islod<ctslods;islod++) {
    SkeletonLOD &slod = skl_aSkeletonLODs[islod];
    slMemoryUsed += sizeof(slod);
    slMemoryUsed += slod.slod_aBones.Count() * sizeof(SkeletonBone);
  }
  return slMemoryUsed;
}

/* Get the description of this object. */
CTString CSkeleton::GetDescription(void)
{
  return CTString(0, "%d LODs", skl_aSkeletonLODs.Count());
}
