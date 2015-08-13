#include "stdh.h"

#include <Engine/Ska/ModelInstance.h>
#include <Engine/Ska/SkaAssert.h>
#include <Engine/Ska/Skeleton.h>
#include <Engine/Ska/Render.h>
#include <Engine/Ska/StringTable.h>
#include <Engine/Ska/ParsingSmbs.h>

#include <Engine/Base/ErrorReporting.h>
#include <Engine/Base/Stream.h>
#include <Engine/Base/Console.h>
#include <Engine/Base/Statistics_internal.h>
#include <Engine/Base/ReplaceFile.h>
#include <Engine/Math/Quaternion.h>

#include <Engine/Templates/DynamicStackArray.cpp>
#include <Engine/Templates/DynamicContainer.cpp>
#include <Engine/Templates/Stock_CMesh.h>
#include <Engine/Templates/Stock_CSkeleton.h>
#include <Engine/Templates/Stock_CAnimSet.h>
#include <Engine/Templates/Stock_CTextureData.h>
#include <Engine/Templates/Stock_CShader.h>
#include <Engine/Templates/Stock_CModelInstance.h>

//안태훈 수정 시작	//(Zone Change System)(0.1)
#include <map>
#include <string>
#include <Engine/Effect/CGroupTag.h>
#include <Engine/Effect/CRefCountPtr.h>
//안태훈 수정 끝	//(Zone Change System)(0.1)

//안태훈 수정 시작	//(5th Closed beta)(0.2)
#include <Engine/Entities/Entity.h>
//안태훈 수정 끝	//(5th Closed beta)(0.2)
//..
// does parser remember smc source files?
extern BOOL _bRememberSourceFN = FALSE;
// Keep vertex arrays ?
extern BOOL _bKeepVertexArrays = FALSE;

// pointer to model instance for parser
CModelInstance *_yy_mi = NULL;

extern INDEX _ctExistingModelInstances = 0;
CModelInstance::CModelInstance()
{
	_ctExistingModelInstances++; // Track num of existing model instances
	mi_pmisSerial = NULL;
	mi_psklSkeleton = NULL;
	mi_iParentBoneID = -1;
	mi_iRideParentBoneID = -1;
	mi_colModelColor = 0;
	mi_vStretch = FLOAT3D(1,1,1);
	mi_colModelColor = 0xFFFFFFFF;
	mi_aqAnims.aq_Lists.SetAllocationStep(1);
	mi_cmiChildren.SetAllocationStep(1);
	memset(&mi_qvOffset,0,sizeof(QVect));
	memset(&mi_qvRideOffset,0,sizeof(QVect)); // Ride offset
	mi_qvOffset.qRot.q_w = 1;
	mi_qvRideOffset.qRot.q_w = 1; // Ride offset
	mi_iCurentBBox = -1;
	mi_bSkipOcclusionTest = FALSE;

	// set default all frames bbox
	mi_cbAllFramesBBox.SetMin(FLOAT3D(-0.5,0,-0.5));
	mi_cbAllFramesBBox.SetMax(FLOAT3D(0.5,2,0.5));

	// set default model instance name
	// mi_cbAllFramesBBox.SetName("All Frames Bounding box");
	SetName("");
	mi_cbAllFramesBBox.SetName("");
	mi_bRenderShadow		= FALSE;	// 03.02

	mi_iFaceMesh		= -1;	
	mi_iHairMesh		= -1;	

	mi_bDummyModel = FALSE;
	mi_ERType	   = ERS_NORMAL;
//안태훈 수정 시작	//(Remake Effect)(0.1)
	//CSkaTag tag;
	//tag.SetName("__ROOT");
	//m_tmSkaTagManager.Register(&tag);
//안태훈 수정 끝	//(Remake Effect)(0.1)
}


CModelInstance::~CModelInstance()
{
//0105
	Clear();
//..
	_ctExistingModelInstances--;
	if(_pModelInstanceStock!=NULL) {
		// Make sure model instance was deleted before destructor
		ASSERT(mi_psklSkeleton==NULL);
		ASSERT(mi_aMeshInst.Count()==0);
		ASSERT(mi_cbAABox.Count()==0);
		ASSERT(mi_aAnimSet.Count()==0);
		ASSERT(mi_cmiChildren.Count()==0);
	}
}


// copy constructor
CModelInstance::CModelInstance(CModelInstance &miOther)
{
	// Forbiden
	ASSERT(FALSE);
}


void CModelInstance::operator=(CModelInstance &miOther)
{
	// Forbiden
	ASSERT(FALSE);
}


void CModelInstance::GetAllFramesBBox(FLOATaabbox3D &aabbox)
{
	aabbox = FLOATaabbox3D(mi_cbAllFramesBBox.Min(),mi_cbAllFramesBBox.Max());
}


// fills curent colision box info
void CModelInstance::GetCurrentColisionBox(FLOATaabbox3D &aabbox)
{
	ColisionBox &cb = GetCurrentColisionBox();
	aabbox = FLOATaabbox3D(cb.Min(),cb.Max());
}

ColisionBox &CModelInstance::GetCurrentColisionBox()
{
	ASSERT(mi_iCurentBBox>=0);
	ASSERT(mi_iCurentBBox<mi_cbAABox.Count());
	ASSERT(mi_cbAABox.Count()>0);

	return mi_cbAABox[mi_iCurentBBox];
}


INDEX CModelInstance::GetColisionBoxIndex(INDEX iBoxID)
{
	INDEX ctcb = mi_cbAABox.Count();
	// for each existing box
	for(INT icb=0;icb<ctcb;icb++) {
		ColisionBox &cb = mi_cbAABox[icb];
		// if this is searched box
		if(cb.GetID() == iBoxID) {
			// return index of box
			return icb;
		}
	}
	// colision box was not found, return default (0)
//0105 1line 잠시 지우기
//  SKAASSERT(FALSE);
//..
	return 0;
}

ColisionBox &CModelInstance::GetColisionBox(INDEX icb)
{
	ASSERT(icb>=0);
	ASSERT(icb<mi_cbAABox.Count());
	return mi_cbAABox[icb];
}

FLOAT3D CModelInstance::GetCollisionBoxMin(INDEX iCollisionBox/*=0*/)
{
	INDEX iCollisionBoxClamped = Clamp(iCollisionBox, 0L, mi_cbAABox.Count()-1L);
	FLOAT3D vMin = mi_cbAABox[ iCollisionBoxClamped].Min();
	return vMin;
};

FLOAT3D CModelInstance::GetCollisionBoxMax(INDEX iCollisionBox/*=0*/)
{
	INDEX iCollisionBoxClamped = Clamp(iCollisionBox, 0L, mi_cbAABox.Count()-1L);
	FLOAT3D vMax = mi_cbAABox[ iCollisionBoxClamped].Max();
	return vMax;
};

// returns HEIGHT_EQ_WIDTH, LENGHT_EQ_WIDTH or LENGHT_EQ_HEIGHT
INDEX CModelInstance::GetCollisionBoxDimensionEquality(INDEX iCollisionBox/*=0*/)
{
	// if colision box does not exists
	if(iCollisionBox>=mi_cbAABox.Count()) {
		// give last colision box
		iCollisionBox = mi_cbAABox.Count()-1;
	}
	// check if error is fixed
	ASSERT(mi_cbAABox.Count()>iCollisionBox);

	ColisionBox &cb = this->mi_cbAABox[iCollisionBox];
	FLOAT fWidth = cb.Max()(1) - cb.Min()(1);
	FLOAT fHeight = cb.Max()(2) - cb.Min()(2);
	FLOAT fLength = cb.Max()(3) - cb.Min()(3);
	if(fLength == fHeight) {
		return SKA_LENGTH_EQ_HEIGHT;
	} else if(fHeight == fWidth) {
		return SKA_HEIGHT_EQ_WIDTH;
	// default fLength == fWidth
	} else {
		return SKA_LENGTH_EQ_WIDTH;
	}
};

// add colision box to model instance
void CModelInstance::AddColisionBox(CTString strName,FLOAT3D vMin,FLOAT3D vMax)
{
	INDEX ctcb = mi_cbAABox.Count();
	mi_cbAABox.Expand(ctcb+1);
	
	ColisionBox &cb = mi_cbAABox[ctcb];
	cb.SetName(strName);
	cb.SetMin(vMin);
	cb.SetMax(vMax);
	mi_iCurentBBox = 0;
}
// remove colision box from model instance
void CModelInstance::RemoveColisionBox(INDEX iIndex)
{
	INDEX ctcb = mi_cbAABox.Count();
	INDEX icbNew = 0;
	CStaticArray<struct ColisionBox> aColisionBoxesTemp;
	aColisionBoxesTemp.New(ctcb-1);
	for(INDEX icb=0;icb<ctcb;icb++) {
		if(iIndex != icb) { 
			aColisionBoxesTemp[icbNew] = mi_cbAABox[icb];
			icbNew++;
		}
	}
	mi_cbAABox = aColisionBoxesTemp;
}

	// Get index of current colision box
INDEX CModelInstance::GetCurrentColisionBoxIndex(void)
{
	return mi_iCurentBBox;
}

// Set index of current colision box
void CModelInstance::SetCurrentColisionBoxIndex(INDEX iNewIndex)
{
	INDEX ctcb = mi_cbAABox.Count();
	// if new index is invalid
	if(iNewIndex>=ctcb || iNewIndex<0) {
		CPrintF("Model: %s - invalid colision box index %d for model\n",(const char*)GetName(), iNewIndex);
		//ASSERTALWAYS("Invalid colision box index");
		// try with index 0
		iNewIndex = 0;
	}

	mi_iCurentBBox = iNewIndex;
}

// add child to modelinstance
void CModelInstance::AddChild(CModelInstance *pmi, INDEX iParentBoneID /* = -1 */)
{
	SKAASSERT(pmi!=NULL);
	if(pmi==NULL) return;
	mi_cmiChildren.Add(pmi);
	if (iParentBoneID>0) {
		pmi->SetParentBone(iParentBoneID);
	}
//안태훈 수정 시작	//(Remake Effect)(0.1)
	m_tmSkaTagManager.AddChild(&pmi->m_tmSkaTagManager);
//안태훈 수정 끝	//(Remake Effect)(0.1)
}

// Remove model instance child (does not delete model instance)
void CModelInstance::RemoveChild(CModelInstance *pmi)
{
	ASSERT(pmi!=this);
	SKAASSERT(pmi!=NULL);
	// aditional check
	if(pmi==NULL) return;
	if(pmi==this) return;

	// Remove child from model instance
	mi_cmiChildren.Remove(pmi);
//안태훈 수정 시작	//(Remake Effect)(0.1)
	m_tmSkaTagManager.RemoveChild(&pmi->m_tmSkaTagManager);
//안태훈 수정 끝	//(Remake Effect)(0.1)
}

// Remove all child model instances (does not delete them)
void CModelInstance::RemoveAllChildren(BOOL bRecursive/*=FALSE*/)
{
	ASSERT(this!=NULL);
	INDEX ctcmi = mi_cmiChildren.Count();
	for(INDEX icmi=0;icmi<ctcmi;icmi++) {
		CModelInstance *pmi = &mi_cmiChildren[icmi];
		if(bRecursive) {
			pmi->RemoveAllChildren(bRecursive);
		}
		// Remove child
		RemoveChild(pmi);
	}
}

// Remove child model instance and delete it
void CModelInstance::DeleteChild(CModelInstance *pmi)
{
	ASSERT(pmi!=this);
	ASSERT(this!=NULL);
	SKAASSERT(pmi!=NULL);
	// aditional check
	if(pmi==NULL) return;
	if(pmi==this) return;

	// Remove child
	RemoveChild(pmi);
	// Delete child
	DeleteModelInstance(pmi);
	pmi = NULL;
}

// Remove all child model instances and deletes them
void CModelInstance::DeleteAllChildren(BOOL bRecursive/*=FALSE*/)
{
	ASSERT(this!=NULL);
	INDEX ctcmi = mi_cmiChildren.Count();
	for(INDEX icmi=0;icmi<ctcmi;icmi++) {
		CModelInstance *pmi = &mi_cmiChildren[icmi];
		if(bRecursive) {
			pmi->RemoveAllChildren(bRecursive);
		}
		// Remove child
		DeleteChild(pmi);
	}
}

// set new parent bone index
void CModelInstance::SetParentBone(INDEX iParentBoneID)
{
	mi_iParentBoneID = iParentBoneID;
}

// Model instance offsets from parent model
void CModelInstance::SetOffset(FLOAT fOffset[6])
{
	FLOAT3D fRot(fOffset[3],fOffset[4],fOffset[5]);
	mi_qvOffset.qRot.FromEuler(fRot);
	mi_qvOffset.vPos = FLOAT3D(fOffset[0],fOffset[1],fOffset[2]);
}

void CModelInstance::SetRideOffset(FLOAT fOffset[6])
{
	FLOAT3D fRot(fOffset[3],fOffset[4],fOffset[5]);
	mi_qvRideOffset.qRot.FromEuler(fRot);
	mi_qvRideOffset.vPos = FLOAT3D(fOffset[0],fOffset[1],fOffset[2]);
}

void CModelInstance::SetOffsetPos(FLOAT3D vPos)
{
	mi_qvOffset.vPos = vPos;
}

void CModelInstance::SetRideOffsetPos(FLOAT3D vPos)
{
	mi_qvRideOffset.vPos = vPos;
}

void CModelInstance::SetOffsetRot(ANGLE3D aRot)
{
	mi_qvOffset.qRot.FromEuler(aRot);
}

void CModelInstance::SetRideOffsetRot(ANGLE3D aRot)
{
	mi_qvRideOffset.qRot.FromEuler(aRot);
}

FLOAT3D CModelInstance::GetOffsetPos()
{
	return mi_qvOffset.vPos;
}

FLOAT3D CModelInstance::GetRideOffsetPos()
{
	return mi_qvRideOffset.vPos;
}

ANGLE3D CModelInstance::GetOffsetRot()
{
	ANGLE3D aRot;
	FLOATmatrix3D mat;
	mi_qvOffset.qRot.ToMatrix(mat);
	DecomposeRotationMatrix(aRot,mat);
	return aRot;
}

ANGLE3D CModelInstance::GetRideOffsetRot()
{
	ANGLE3D aRot;
	FLOATmatrix3D mat;
	mi_qvRideOffset.qRot.ToMatrix(mat);
	DecomposeRotationMatrix(aRot,mat);
	return aRot;
}

// Stretch model instance
void CModelInstance::StretchModel(FLOAT3D &vStretch)
{
	mi_vStretch = vStretch;
}


// Stretch model instance without attachments
void CModelInstance::StretchSingleModel(FLOAT3D &vStretch)
{
	// Note: (to self) No need to recursivly stretch each child, just children of root model instance
	mi_vStretch = vStretch;
	FLOAT3D vStretchInv = FLOAT3D(1/vStretch(1),1/vStretch(2),1/vStretch(3));

	// for each child of model instance
	INDEX ctmic = mi_cmiChildren.Count();
	for(INDEX imic=0;imic<ctmic;imic++) {
		// set new stretch of model instance
		CModelInstance &mic = mi_cmiChildren[imic];
		mic.StretchModel(vStretchInv);
	}
}

// Add mesh to ModelInstance
void CModelInstance::AddMesh_t(const CTFileName &fnMesh)
{
	int ctMeshInst = mi_aMeshInst.Count();
	mi_aMeshInst.Expand(ctMeshInst+1);
	memset(&mi_aMeshInst[ctMeshInst],0,sizeof(mi_aMeshInst[ctMeshInst]));
	mi_aMeshInst[ctMeshInst].mi_pMesh = _pMeshStock->Obtain_t(fnMesh);
//안태훈 수정 시작	//(Effect Add & Modify for Close Beta)(0.1)
	this->RefreshTagManager();
//안태훈 수정 끝	//(Effect Add & Modify for Close Beta)(0.1)
}

// Add skeleton to ModelInstance
void CModelInstance::AddSkeleton_t(const CTFileName &fnSkeleton)
{
	mi_psklSkeleton = _pSkeletonStock->Obtain_t(fnSkeleton);
}

// Add AnimSet to ModelInstance
void CModelInstance::AddAnimSet_t(const CTFileName &fnAnimSet)
{
	CAnimSet *Anim = _pAnimSetStock->Obtain_t(fnAnimSet);
	mi_aAnimSet.Add(Anim);
}

// Add texture to ModelInstance (if no mesh instance given, add texture to last mesh instance)
void CModelInstance::AddTexture_t(const CTFileName &fnTexture, INDEX iTexID, MeshInstance *pmshi)
{
	if(pmshi == NULL) {
		INDEX ctMeshInst = mi_aMeshInst.Count();
		if(ctMeshInst<=0) throw("Error adding texture\nMesh instance does not exists");
		pmshi = &mi_aMeshInst[ctMeshInst-1];
	}

	INDEX ctTextInst = pmshi->mi_tiTextures.Count();
	pmshi->mi_tiTextures.Expand(ctTextInst+1);
	pmshi->mi_tiTextures[ctTextInst].ti_toTexture.SetData_t(fnTexture);
	pmshi->mi_tiTextures[ctTextInst].SetName(iTexID);
}

// Add texture to ModelInstance (if no mesh instance given, add texture to last mesh instance)
void CModelInstance::AddTexture_t(const CTFileName &fnTexture, CTString strTexID,MeshInstance *pmshi)
{
	INDEX iTexID = ska_StringToID(strTexID);
	AddTexture_t(fnTexture,iTexID,pmshi);
}

// Remove one texture from model instance
void CModelInstance::RemoveTexture(TextureInstance *ptiRemove,MeshInstance *pmshi)
{
	ASSERT(pmshi!=NULL);
	CStaticArray<struct TextureInstance> atiTextures;
	INDEX ctti=pmshi->mi_tiTextures.Count();
	atiTextures.New(ctti-1);
	// for each texture instance in mesh instance
	INDEX iIndexSrc=0;
	for(INDEX iti=0;iti<ctti;iti++)
	{
		TextureInstance *pti = &pmshi->mi_tiTextures[iti];
		// if texture instance is different from selected one 
		if(pti != ptiRemove) {
			// copy it to new array of texture isntances
			atiTextures[iIndexSrc] = pmshi->mi_tiTextures[iti];
			iIndexSrc++;
		}
	}
	// copy new texture instances array in mesh instance
	pmshi->mi_tiTextures.CopyArray(atiTextures);
	// clear temp texture isntances array
	atiTextures.Clear();
}

MeshInstance *CModelInstance::FindMeshInstance(INDEX iMeshID)
{
	// for each mesh instance
	INDEX ctmshi = mi_aMeshInst.Count();
	for(INDEX imshi=0;imshi<ctmshi;imshi++) 
	{
		MeshInstance &mshi = mi_aMeshInst[imshi];
		if(mshi.mi_pMesh)
		{
			INDEX iID = ska_GetIDFromStringTable(mshi.mi_pMesh->GetName());
			if(iMeshID == iID)
			{
				return &mshi;
			}
		}
	}
	return NULL;
}

// Find texture instance in all mesh instances in model instance
TextureInstance *CModelInstance::FindTexureInstance(INDEX iTexID)
{
	// for each mesh instance
	INDEX ctmshi = mi_aMeshInst.Count();
	for(INDEX imshi=0;imshi<ctmshi;imshi++) {
		MeshInstance &mshi = mi_aMeshInst[imshi];
		// for each texture instance in meshinstance
		INDEX ctti = mshi.mi_tiTextures.Count();
		for(INDEX iti=0;iti<ctti;iti++) {
			TextureInstance &ti = mshi.mi_tiTextures[iti];
			// if this is texinstance that is beeing serched for
			if(ti.GetID() == iTexID) {
				// return it
				return &ti;
			}
		}
	}
	// texture instance wasn't found
	return NULL;
}

// Find texture instance in given mesh instance
TextureInstance *CModelInstance::FindTexureInstance(INDEX iTexID, MeshInstance &mshi)
{
	// for each texture instance in given mesh instance
	INDEX ctti = mshi.mi_tiTextures.Count();
	for(INDEX iti=0;iti<ctti;iti++) {
		TextureInstance &ti = mshi.mi_tiTextures[iti];
		// if this is texinstance that is beeing serched for
		if(ti.GetID() == iTexID) {
			// return it
			return &ti;
		}
	}
	// texture instance wasn't found
	return NULL;
}

// change parent of model instance
void CModelInstance::ChangeParent(CModelInstance *pmiOldParent, CModelInstance *pmiNewParent)
{
	SKAASSERT(pmiOldParent!=NULL);
	SKAASSERT(pmiNewParent!=NULL);

	if(pmiOldParent == NULL) {
		CPrintF("Model Instance doesn't have a parent\n");
		return;
	}
	if(pmiNewParent == NULL) {
		CPrintF("New parent of model instance is NULL\n");
		return;
	}
	pmiOldParent->mi_cmiChildren.Remove(this);
	pmiNewParent->mi_cmiChildren.Add(this);
}

// return parent of this model instance
// must suply first model instance in hierarchy cos model instance does not have its parent remembered
CModelInstance *CModelInstance::GetParent(CModelInstance *pmiStartFrom)
{
	ASSERT(pmiStartFrom!=NULL);
	// aditional check
	if(pmiStartFrom==NULL) return NULL;
	// if 'this' is member of pmiStartFrom return it
	if(pmiStartFrom->mi_cmiChildren.IsMember(this)) {
		return pmiStartFrom;
	}
	// count childrent of pmiStartFrom
	INDEX ctcmi = pmiStartFrom->mi_cmiChildren.Count();
	// for each child of pmiStartFrom
	for(INDEX icmi=0;icmi<ctcmi;icmi++) {
		// if some of children have 'this' as member return them as parent
		CModelInstance *pmiReturned = GetParent(&pmiStartFrom->mi_cmiChildren[icmi]);
		if(pmiReturned != NULL) {
			return pmiReturned;
		}
	}
	return NULL;
}

CModelInstance *CModelInstance::GetChild(INDEX iChildID, BOOL bRecursive/*=FALSE*/)
{
	INDEX ctcmi = mi_cmiChildren.Count();
	for(INDEX icmi=0;icmi<ctcmi;icmi++) {
		CModelInstance *pmi = &mi_cmiChildren[icmi];
		if(pmi->mi_iModelID == iChildID) {
			return pmi;
		}
		// if child has own children, go recursive
		if(bRecursive && pmi->mi_cmiChildren.Count()>0) {      
			pmi = pmi->GetChild(iChildID, TRUE);
			if (pmi!=NULL) return pmi;
		}    
	}
	return NULL;
}

// returns parent that is not included in his parents smc file
CModelInstance *CModelInstance::GetFirstNonReferencedParent(CModelInstance *pmiRoot)
{
	ASSERT(this!=NULL);
	ASSERT(pmiRoot!=NULL);
	CModelInstance *pmiParent = this->GetParent(pmiRoot);
	CModelInstance *pmiLast = this;
	while(pmiParent != NULL)
	{
		if(pmiParent->mi_fnSourceFile != mi_fnSourceFile)
		{
			return pmiLast;
		}
		pmiLast = pmiParent;
		pmiParent = pmiParent->GetParent(pmiRoot);
	}
	return NULL;//return pmiRoot
}

// add animation to ModelInstance
void CModelInstance::AddAnimation(INDEX iAnimID, ULONG ulFlags, FLOAT fStrength, INDEX iGroupID, FLOAT fSpeedMul/*=1.0f*/)
{
//안태훈 수정 시작	//(Effect Add & Modify for Close Beta)(0.1)
	if(iAnimID == -1) return;
//안태훈 수정 끝	//(Effect Add & Modify for Close Beta)(0.1)
#ifdef SKADEBUG
	INDEX iDummy1, iDummy2;
	// if animation does not exits in model instance
	if (!FindAnimationByID(iAnimID, &iDummy1, &iDummy2)) { 
		// assert
//		SKAASSERT(FALSE);
	}
#endif

	// if no restart flag was set
	if(ulFlags&AN_NORESTART) {
		// if given animtion is already playing
		if(IsAnimationPlaying(iAnimID)) {
			if(ulFlags&AN_LOOPING) {
				AddFlagsToPlayingAnim(iAnimID,AN_LOOPING);
			}
			// return without adding animtion
			return;
		}
	}

	// if flag for new cleared state is set
	if(ulFlags&AN_CLEAR) {
		// do new clear state with default length
		NewClearState(CLEAR_STATE_LENGTH);
	// if flag for new cloned state is set
	} else if(ulFlags&AN_CLONE) {
		// do new clear state with default length
		NewClonedState(CLONED_STATE_LENGTH);
	}

	// if anim queue is empty 
	INDEX ctal = mi_aqAnims.aq_Lists.Count();
	if(ctal == 0) {
		// add new clear state
		NewClearState(0);
	}

	ctal = mi_aqAnims.aq_Lists.Count();
	AnimList &alList = mi_aqAnims.aq_Lists[ctal-1];

	// if flag is set not to sort anims
	if(ulFlags&AN_NOGROUP_SORT) {
		// just add new animations to end of list
		PlayedAnim &plAnim = alList.al_PlayedAnims.Push();
		plAnim.pa_iAnimID = iAnimID;
		plAnim.pa_fSpeedMul = fSpeedMul;
		plAnim.pa_fStartTime = _pTimer->CurrentTick();
		plAnim.pa_fStrength = fStrength;
		plAnim.pa_ulFlags = ulFlags;
		plAnim.pa_GroupID = iGroupID;
//안태훈 수정 시작	//(5th Closed beta)(0.2)
		if(!m_vectorAnimEffect.empty())
			StartAnimEffect(plAnim.pa_iAnimID, plAnim.pa_fSpeedMul);
		//CPrintF("---%6.2f---StartAnimEffect : ani( %s )\n", _pTimer->GetLerpedCurrentTick(), ska_IDToString(plAnim.pa_iAnimID));
//안태훈 수정 끝	//(5th Closed beta)(0.2)
	// no flag set, sort animation by groupID
	} else {
		// add one animation to anim list
		alList.al_PlayedAnims.Push();
		INDEX ctpa = alList.al_PlayedAnims.Count();

		INDEX ipa=ctpa-1;
		if(ipa>0) {
			// for each old animation from last to first
			for(;ipa>0;ipa--) {
				PlayedAnim &pa = alList.al_PlayedAnims[ipa-1];
				PlayedAnim &paNext = alList.al_PlayedAnims[ipa];
				// if anim group id is larger than new group id
				if(pa.pa_GroupID>iGroupID) {
					// move animation in array to right
					paNext = pa;
				} else break;
			}
		}
		// set new animation as current index in anim list
		PlayedAnim &plAnim = alList.al_PlayedAnims[ipa];
		plAnim.pa_iAnimID = iAnimID;
		plAnim.pa_fSpeedMul = fSpeedMul;
		plAnim.pa_fStartTime = _pTimer->CurrentTick();
		plAnim.pa_fStrength = fStrength;
		plAnim.pa_ulFlags = ulFlags;
		plAnim.pa_GroupID = iGroupID;
//안태훈 수정 시작	//(5th Closed beta)(0.2)
		if(!m_vectorAnimEffect.empty())
			StartAnimEffect(plAnim.pa_iAnimID, plAnim.pa_fSpeedMul);
		//CPrintF("---%6.2f---StartAnimEffect : ani( %s )\n", _pTimer->GetLerpedCurrentTick(), ska_IDToString(plAnim.pa_iAnimID));
//안태훈 수정 끝	//(5th Closed beta)(0.2)
	}

	INDEX ctmi = mi_cmiChildren.Count();
	for(INDEX imi=0;imi<ctmi;imi++)
	{
		CModelInstance &cmi = mi_cmiChildren[imi];
		
		cmi.AddAnimation( iAnimID, ulFlags,fStrength,iGroupID, fSpeedMul);				
	}
}

void CModelInstance::AddAnimationChild(INDEX iAnimID, ULONG ulFlags, FLOAT fStrength, INDEX iGroupID, FLOAT fSpeedMul/*=1.0f*/)
{
#ifdef SKADEBUG	
	INDEX iDummy1, iDummy2;
	// if animation does not exits in model instance
	if (!FindAnimationByID(iAnimID, &iDummy1, &iDummy2)) { 
		// assert
		SKAASSERT(FALSE);
	}	
#endif

	fSpeedMul = 1/fSpeedMul;
	// if no restart flag wa

	INDEX ctmi = mi_cmiChildren.Count();
	for(INDEX imi=0;imi<ctmi;imi++)
	{
		CModelInstance &cmi = mi_cmiChildren[imi];
		
		cmi.AddAnimation( iAnimID, ulFlags,fStrength,iGroupID, fSpeedMul);				
	}
}

// remove played anim from stack
void CModelInstance::RemAnimation(INDEX iAnimID)
{
	INDEX ctal = mi_aqAnims.aq_Lists.Count();
	// if anim queue is empty
	if(ctal < 1) {
		SKAASSERT(FALSE);
		// no anim to remove
		return;
	}

	// get last anim list in queue
	AnimList &alList = mi_aqAnims.aq_Lists[ctal-1];
	// count played anims in anim list
	INDEX ctpa = alList.al_PlayedAnims.Count();
	// loop each played anim in anim list
	for(int ipa=0;ipa<ctpa;ipa++) {
		PlayedAnim &paAnim = alList.al_PlayedAnims[ipa];
		// remove if same ID
		if(paAnim.pa_iAnimID == iAnimID) {
			// copy all latter anims over this one
			for(int ira=ipa;ira<ctpa-1;ira++) {
				alList.al_PlayedAnims[ira] = alList.al_PlayedAnims[ira+1];
			}
			// decrease played anims count
			ctpa--;
			// remove last anim
			alList.al_PlayedAnims.Pop();
		}
	}
}

// Remove all anims with GroupID
void CModelInstance::RemAnimsWithID(INDEX iGroupID)
{
	INDEX ctal = mi_aqAnims.aq_Lists.Count();
	// if anim queue is empty
	if(ctal < 1) {
		SKAASSERT(FALSE);
		// no anim to remove
		return;
	}

	// get last anim list in queue
	AnimList &alList = mi_aqAnims.aq_Lists[ctal-1];
	// count played anims in anim list
	INDEX ctpa = alList.al_PlayedAnims.Count();
	// loop each played anim in anim list
	for(int ipa=0;ipa<ctpa;ipa++) {
		PlayedAnim &paAnim = alList.al_PlayedAnims[ipa];
		// remove if same Group ID
		if(paAnim.pa_GroupID == iGroupID) {
			// copy all latter anims over this one
			for(int ira=ipa;ira<ctpa-1;ira++) {
				alList.al_PlayedAnims[ira] = alList.al_PlayedAnims[ira+1];
			}
			// decrease played anims count
			ctpa--;
			// remove last anim
			alList.al_PlayedAnims.Pop();
		}
	}
}

// remove unused anims from queue
void CModelInstance::RemovePassedAnimsFromQueue()
{
	INDEX	ial;
	// count AnimLists
	INDEX ctal = mi_aqAnims.aq_Lists.Count();
	// find newes animlist that has fully faded in
	INDEX iFirstAnimList = -1;
	// for each anim list from last to first
	for( ial=ctal-1;ial>=0;ial--) {
		AnimList &alList = mi_aqAnims.aq_Lists[ial];
		// calculate fade factor for this animlist
		FLOAT fFadeFactor = CalculateFadeFactor(alList);
		// if factor is 1 remove all animlists before this one
		if(fFadeFactor >= 1.0f) {
			iFirstAnimList = ial;
			break;
		}
	}
	if(iFirstAnimList <= 0) return;

	// move later anim lists to first pos
	for( ial = iFirstAnimList; ial < ctal; ial++) {
		mi_aqAnims.aq_Lists[ial-iFirstAnimList] = mi_aqAnims.aq_Lists[ial];
		mi_aqAnims.aq_Lists[ial].al_PlayedAnims.PopAll();
	}
	// remove all Anim list before iFirstAnimList
	mi_aqAnims.aq_Lists.PopUntil(ctal-iFirstAnimList-1);

	// for each anim list left
	ctal = mi_aqAnims.aq_Lists.Count();
	for( ial = 0; ial < ctal; ial++) {
		AnimList &al = mi_aqAnims.aq_Lists[ial];
		// for each played anim in anim list
		INDEX ctpa = al.al_PlayedAnims.Count();
		for(INDEX ipa=0;ipa<ctpa;ipa++) {
			PlayedAnim &pa = al.al_PlayedAnims[ipa];
			// if played anim has fade out flag
			if(pa.pa_ulFlags&AN_FADEOUT) {
				INDEX iAnimSetIndex, iAnimIndex;
				// find the animation in model instance
				if(FindAnimationByID(pa.pa_iAnimID,&iAnimSetIndex,&iAnimIndex)) {
					CAnimSet &as = mi_aAnimSet[iAnimSetIndex];
					Animation &an = as.as_Anims[iAnimIndex];
					FLOAT fAnimEndLength = _pTimer->GetLerpedCurrentTick() - (pa.pa_fStartTime + (an.an_iFrames * an.an_fSecPerFrame * pa.pa_fSpeedMul));
					FLOAT fAnimStrength = Clamp(pa.pa_fStrength-(fAnimEndLength/al.al_fFadeTime),0.0f,1.0f);
					// if anim has already faded out
					if(fAnimStrength==0) {
						INDEX ipaLeft = ctpa-ipa-1;
						// if there are some animations after this one
						if(ipaLeft>0) {
							// overwrite current animtion with later anims
							memcpy(&al.al_PlayedAnims[ipa],&al.al_PlayedAnims[ipa+1],sizeof(PlayedAnim)*ipaLeft);
						}
						// remove last animation from animlist
						al.al_PlayedAnims.Pop();
						ctpa--;
						ipa--;
					}
				} else {
					// this animation must exists
					SKAASSERT(FALSE);
				}
			}
		}
	}
}

// create new state, copy last state in it and give it a fade time
void CModelInstance::NewClonedState(FLOAT fFadeTime)
{
	RemovePassedAnimsFromQueue();
	INDEX ctal = mi_aqAnims.aq_Lists.Count();
	if(ctal == 0) 
	{
	// if anim queue is empty add new clear state
		NewClearState(fFadeTime);
		ctal = 1;
	}
	// create new Anim list
	AnimList &alNewList = mi_aqAnims.aq_Lists.Push();
	alNewList.al_PlayedAnims.SetAllocationStep(1);
	AnimList &alList = mi_aqAnims.aq_Lists[ctal-1];
	// copy anims to new List
	alNewList.al_PlayedAnims = alList.al_PlayedAnims;
	alNewList.al_fFadeTime = fFadeTime;
	alNewList.al_fStartTime = _pTimer->CurrentTick();
}

// create new cleared state and give it a fade time
void CModelInstance::NewClearState(FLOAT fFadeTime)
{
//안태훈 수정 시작	//(5th Closed beta)(0.2)
	if(!m_vectorAnimEffect.empty())
	{
		INDEX iAnimSetIndex, iAnimIndex;
		INDEX cntAL = mi_aqAnims.aq_Lists.Count();
		for(INDEX iAL=0; iAL<cntAL; ++iAL)
		{
			INDEX cntPA = mi_aqAnims.aq_Lists[iAL].al_PlayedAnims.Count();
			for(INDEX iPA=0; iPA<cntPA; ++iPA)
			{
				PlayedAnim &pa = mi_aqAnims.aq_Lists[iAL].al_PlayedAnims[iPA];
				if(FindAnimationByID(pa.pa_iAnimID,&iAnimSetIndex,&iAnimIndex))
				{
					const Animation &an = mi_aAnimSet[iAnimSetIndex].as_Anims[iAnimIndex];
					FLOAT fAnimLength = an.an_iFrames * an.an_fSecPerFrame * pa.pa_fSpeedMul;
					FLOAT delta = (INDEX((_pTimer->GetLerpedCurrentTick() - pa.pa_fStartTime) / fAnimLength) + 1) * fAnimLength;
					if(pa.pa_fStartTime + delta < _pTimer->GetLerpedCurrentTick()) pa.pa_fStartTime += delta;
					else if(pa.pa_fStartTime + delta - fAnimLength < _pTimer->GetLerpedCurrentTick()) pa.pa_fStartTime += delta - fAnimLength;
					StopAnimEffect(mi_aqAnims.aq_Lists[iAL].al_PlayedAnims[iPA].pa_iAnimID, fFadeTime*0.5f);

					//if(CEntity::GetPlayerEntity(0)->GetModelInstance() == this)
						//CPrintF("---%6.2f---StopAnimEffect : ani( %s )\n", _pTimer->GetLerpedCurrentTick(), ska_IDToString(mi_aqAnims.aq_Lists[iAL].al_PlayedAnims[iPA].pa_iAnimID));
				}
			}
		}
	}
//안태훈 수정 끝	//(5th Closed beta)(0.2)
	RemovePassedAnimsFromQueue();
	// add new empty list
	AnimList &alNewList = mi_aqAnims.aq_Lists.Push();
	alNewList.al_PlayedAnims.SetAllocationStep(1);
	alNewList.al_fFadeTime = fFadeTime;
	alNewList.al_fStartTime = _pTimer->CurrentTick();
	alNewList.al_PlayedAnims.PopAll();

	INDEX ctmi = mi_cmiChildren.Count();
	for(INDEX imi=0;imi<ctmi;imi++)
	{
		CModelInstance &cmi = mi_cmiChildren[imi];
		cmi.NewClearState(fFadeTime);
	}
}

// stop all animations in this model instance and its children
void CModelInstance::StopAllAnimations(FLOAT fFadeTime)
{
	INDEX ctmi = mi_cmiChildren.Count();
	for(INDEX imi=0;imi<ctmi;imi++)
	{
		CModelInstance &cmi = mi_cmiChildren[imi];
		cmi.StopAllAnimations(fFadeTime);
	}
	NewClearState(fFadeTime);
}

// Offset all animations in anim queue
void CModelInstance::OffSetAnimationQueue(TIME fOffsetTime)
{
	// for each anim list in anim queue
	INDEX ctal = mi_aqAnims.aq_Lists.Count();
	for(INDEX ial=0;ial<ctal;ial++) {
		AnimList &al = mi_aqAnims.aq_Lists[ial];
		// Modify anim list start time
		al.al_fStartTime -=fOffsetTime;
	}
}

// Find animation by ID
BOOL CModelInstance::FindAnimationByID(INDEX iAnimID,INDEX *piAnimSetIndex,INDEX *piAnimIndex)
{
	INDEX ctas = mi_aAnimSet.Count();
	if (ctas<=0) return FALSE;
	// for each animset
	for(int ias=ctas-1;ias>=0;ias--) {
		CAnimSet &asAnimSet = mi_aAnimSet[ias];
		INDEX ctan = asAnimSet.as_Anims.Count();
		// for each animation
		for(int ian=0;ian<ctan;ian++) {
			Animation &an = asAnimSet.as_Anims[ian];
			// if this is animation to find
			if(an.an_iID == iAnimID) {
				// set pointers of indices to animset and animation
				*piAnimSetIndex = ias;
				*piAnimIndex = ian;
				// retrun succesfully
				return TRUE;
			}
		}
	}
	// animation was't found
	return FALSE;
}

// Find animation by ID
INDEX CModelInstance::FindFirstAnimationID()
{
	INDEX ctas = mi_aAnimSet.Count();
	// for each animset
	for(int ias=0; ias<ctas; ias--) {
		CAnimSet &asAnimSet = mi_aAnimSet[ias];
		INDEX ctan = asAnimSet.as_Anims.Count();
		// for each animation
		for(int ian=0;ian<ctan;ian++) {
			Animation &an = asAnimSet.as_Anims[ian];
			return an.an_iID;      
		}
	}
//안태훈 수정 시작	//(Effect Add & Modify for Close Beta)(0.1)
	//SKAASSERT(FALSE);
//안태훈 수정 끝	//(Effect Add & Modify for Close Beta)(0.1)
	// never should get here
	return -1;
}

// get animation length
FLOAT CModelInstance::GetAnimLength(INDEX iAnimID, BOOL bLoopingAnim/*=FALSE*/)
{
	INDEX iAnimSetIndex,iAnimIndex;
	if(FindAnimationByID(iAnimID,&iAnimSetIndex,&iAnimIndex)) {
		CAnimSet &as = mi_aAnimSet[iAnimSetIndex];
		Animation &an = as.as_Anims[iAnimIndex];
		if(bLoopingAnim) {
			return an.an_fSecPerFrame * an.an_iFrames;
		} else {
			return an.an_fSecPerFrame * (an.an_iFrames-1);
		}
	} else {
		// Animation wasn't found
	//	SKAASSERT(FALSE);
		return 0.1f;
	};
}

// Get animation start time
FLOAT CModelInstance::GetAnimPlayingTime(INDEX iAnimID)
{
	// check last anim list if animation iAnimID exists in it
	INDEX ctal = mi_aqAnims.aq_Lists.Count();
	// if there are anim lists in animqueue
	if(ctal>0) {
		// check last one
		AnimList &al = mi_aqAnims.aq_Lists[ctal-1];
		INDEX ctpa = al.al_PlayedAnims.Count();
		for(INDEX ipa=0;ipa<ctpa;ipa++) {
			PlayedAnim &pa = al.al_PlayedAnims[ipa];
			if(pa.pa_iAnimID == iAnimID) {
				return _pTimer->CurrentTick()-al.al_fStartTime;
			}
		}
	}
	// this animation is currently not playing
	return 0.0f;
}

// Check if given animation is currently playing
BOOL CModelInstance::IsAnimationPlaying(INDEX iAnimID)
{
	// check last anim list if animation iAnimID exists in it
	INDEX ctal = mi_aqAnims.aq_Lists.Count();
	// if there are anim lists in animqueue
	if(ctal>0) {
		// check last one
		AnimList &al = mi_aqAnims.aq_Lists[ctal-1];
		INDEX ctpa = al.al_PlayedAnims.Count();
		for(INDEX ipa=0;ipa<ctpa;ipa++) {
			PlayedAnim &pa = al.al_PlayedAnims[ipa];
			if(pa.pa_iAnimID == iAnimID) {
				// found it
				return TRUE;
			}
		}
	}
	// this animation is currently not playing
	return FALSE;
}

// Add flags to animation playing in anim queue
BOOL CModelInstance::AddFlagsToPlayingAnim(INDEX iAnimID, ULONG ulFlags)
{
	// check last anim list if animation iAnimID exists in it
	INDEX ctal = mi_aqAnims.aq_Lists.Count();
	// if there are anim lists in animqueue
	if(ctal>0) {
		// check last one
		AnimList &al = mi_aqAnims.aq_Lists[ctal-1];
		INDEX ctpa = al.al_PlayedAnims.Count();
		for(INDEX ipa=0;ipa<ctpa;ipa++) {
			PlayedAnim &pa = al.al_PlayedAnims[ipa];
			if(pa.pa_iAnimID == iAnimID) {
				pa.pa_ulFlags |= ulFlags;
				// found it
				return TRUE;
			}
		}
	}
	// this animation is currently not playing
	return FALSE;
}

// Sets name of model instance
void CModelInstance::SetName(const CTString &strName)
{
	mi_strName = strName;
	if(strName.Length()==0) {
		mi_iModelID = -1;
	} else {
		mi_iModelID = ska_GetIDFromStringTable(strName);
	}
}
// Gets name of model instance
CTString CModelInstance::GetName()
{
	if(mi_iModelID==(-1)) {
		return "";
	} else {
		return ska_GetStringFromTable(mi_iModelID);
	}
}
// Gets id of model instance
const INDEX &CModelInstance::GetID()
{
	return mi_iModelID;
}

// Get vertex positions in absolute space
void CModelInstance::GetModelVertices( CStaticStackArray<FLOAT3D> &avVertices, FLOATmatrix3D &mRotation,
																		 FLOAT3D &vPosition, FLOAT fNormalOffset, FLOAT fDistance)
{
	RM_GetModelVertices(*this,avVertices, mRotation, vPosition, fNormalOffset, fDistance);
}

// Model color
COLOR CModelInstance::GetModelColor(void)
{
	return mi_colModelColor;
}

void CModelInstance::SetModelColor(COLOR colNewColor)
{
	mi_colModelColor = colNewColor;
	INDEX ctch = mi_cmiChildren.Count();
	// for each child
	for(INDEX ich=0;ich<ctch;ich++) {
		CModelInstance &cmi = mi_cmiChildren[ich];
		// set child model color 
		cmi.SetModelColor(colNewColor);
	}
}

// Test it the model has alpha blending
BOOL CModelInstance::HasAlpha(void)
{
	return (GetModelColor()&0xFF) != 0xFF;
}

BOOL CModelInstance::IsModelVisible(FLOAT fMipDistance)
{
	// for each mesh
	INDEX ctmshi = mi_aMeshInst.Count();
	for(INDEX imshi=0;imshi<ctmshi;imshi++) {
		ASSERT(mi_aMeshInst[imshi].mi_pMesh!=NULL);
		MeshInstance &mshi = mi_aMeshInst[imshi];
		// if some mesh lod is visible
		if(GetMeshLOD(*mshi.mi_pMesh,fMipDistance)>=0) {
			// mark as visible
			return TRUE;
		}
	}

	// if model has skeleton and its skeleton is visible
	if((mi_psklSkeleton!=NULL) && (GetSkeletonLOD(*mi_psklSkeleton,fMipDistance)>=0)) {
		return TRUE;
	}
	return FALSE;
}

BOOL CModelInstance::HasShadow(FLOAT fMipFactor)
{
	#pragma message(">> HasShadow")
	return TRUE;
}

// simple shadow rendering
void CModelInstance::AddSimpleShadow( const FLOAT fIntensity, const FLOATplane3D &plShadowPlane)
{
	// if shadows are not rendered for current mip, model is half/full face-forward,
	// intensitiy is too low or projection is not perspective - do nothing!
	// || (rm.rm_pmdModelData->md_Flags&(MF_FACE_FORWARD|MF_HALF_FACE_FORWARD))) return;
	ASSERT( fIntensity>0 && fIntensity<=1);
	_sfStats.IncrementCounter( CStatForm::SCI_MODELSHADOWS);
	// add one simple shadow to batch list
	RM_SetObjectMatrices(*this);
	RM_AddSimpleShadow_View(*this, fIntensity, plShadowPlane);
}

// Copy mesh instance for other model instance
void CModelInstance::CopyMeshInstance(CModelInstance &miOther)
{
	INDEX ctmshi = miOther.mi_aMeshInst.Count();
	// for each mesh insntace
	for(INDEX imshi=0;imshi<ctmshi;imshi++) {
		MeshInstance &mshiOther = miOther.mi_aMeshInst[imshi];
		// add its mesh
		AddMesh_t(mshiOther.mi_pMesh->GetName());
		MeshInstance *pmshi = &mi_aMeshInst[imshi];

		INDEX ctti = mshiOther.mi_tiTextures.Count();
		// for each texture in mesh instance
		for(INDEX iti=0;iti<ctti;iti++) {
			TextureInstance &tiOther = mshiOther.mi_tiTextures[iti];
			AddTexture_t(tiOther.ti_toTexture.GetName(),tiOther.GetID(),pmshi);
		}
	}
}

// copy from another object of same class
void CModelInstance::Copy(CModelInstance &miOther)
{
	// clear this instance - otherwise it won't work
	Clear();

	mi_aqAnims.aq_Lists = miOther.mi_aqAnims.aq_Lists;
	mi_iCurentBBox  = miOther.mi_iCurentBBox;
	mi_colModelColor = miOther.mi_colModelColor;
	mi_iParentBoneID = miOther.mi_iParentBoneID;
	mi_iRideParentBoneID = miOther.mi_iRideParentBoneID;
	mi_qvOffset = miOther.mi_qvOffset;
	mi_qvRideOffset = miOther.mi_qvRideOffset;
	mi_strName = miOther.mi_strName;
	mi_iModelID = miOther.mi_iModelID;
	mi_cbAABox = miOther.mi_cbAABox;
	mi_fnSourceFile = miOther.mi_fnSourceFile;
	mi_vStretch = miOther.mi_vStretch;
	mi_cbAllFramesBBox = miOther.mi_cbAllFramesBBox;

	mi_bRenderShadow	= miOther.mi_bRenderShadow;	// 03.02
	mi_iFaceMesh		= miOther.mi_iFaceMesh;
	mi_iHairMesh		= miOther.mi_iHairMesh;
	
	mi_bDummyModel		= miOther.mi_bDummyModel;
	mi_ERType			= miOther.mi_ERType;
	// if model instance was obtained from stock
	if(miOther.mi_pmisSerial!=NULL) {
		// Copy pointer to model instance serial
		mi_pmisSerial = miOther.mi_pmisSerial;
		// mark model instance used once more
		mi_pmisSerial->MarkUsed();
	}

	// copt mesh instance
	CopyMeshInstance(miOther);

	// if skeleton exists 
	if(miOther.mi_psklSkeleton!=NULL) {
		// copy skeleton
		AddSkeleton_t(miOther.mi_psklSkeleton->GetName());
	}

	// copy animsets
	INDEX ctas = miOther.mi_aAnimSet.Count();
	// for each animset
	for(INDEX ias=0;ias<ctas;ias++) {
		// add animset to this model instance
		CAnimSet &asOther = miOther.mi_aAnimSet[ias];
		AddAnimSet_t(asOther.GetName());
	}

	// copy children
	INDEX ctch = miOther.mi_cmiChildren.Count();
	// for each child in other model instance
	for(INDEX ich=0;ich<ctch;ich++) {
		CModelInstance &chmiOther = miOther.mi_cmiChildren[ich];
		CModelInstance *pchmi = CreateModelInstance("");
		pchmi->Copy(chmiOther);
		AddChild(pchmi);
	}

//안태훈 수정 시작	//(Effect Add & Modify for Close Beta)(0.1)
	m_vectorAnimEffect = miOther.m_vectorAnimEffect;
	m_fnmAnimEffectFile = miOther.m_fnmAnimEffectFile;
//안태훈 수정 끝	//(Effect Add & Modify for Close Beta)(0.1)

	this->RefreshTagManager();
	m_tmSkaTagManager.SetOwner(miOther.m_tmSkaTagManager.GetOwner());
}

// Synchronize with another model (copy animations/attachments positions etc from there)
void CModelInstance::Synchronize(CModelInstance &miOther)
{
	// Sync animations
	mi_aqAnims.aq_Lists = miOther.mi_aqAnims.aq_Lists;
	// Sync misc params
	mi_qvOffset      = miOther.mi_qvOffset;
	mi_qvRideOffset  = miOther.mi_qvRideOffset;
	mi_iParentBoneID = miOther.mi_iParentBoneID;
	mi_iRideParentBoneID = miOther.mi_iRideParentBoneID;
	mi_colModelColor = miOther.mi_colModelColor;
	mi_vStretch      = miOther.mi_vStretch;

	// for each child in model instance
	INDEX ctmic=mi_cmiChildren.Count();
	for(INDEX imic=0;imic<ctmic;imic++) {
		CModelInstance &mic = mi_cmiChildren[imic];
		INDEX iChildID = mic.GetID();
		CModelInstance *pmicOther = miOther.GetChild(iChildID,FALSE);
		// if both model instance have this child 
		if(pmicOther!=NULL) {
			// sync child
			mic.Synchronize(*pmicOther);
		}
	}
}


// clear model instance
void CModelInstance::Clear(void)
{
	// if model instance was obtained from stock
	if(mi_pmisSerial!=NULL) {
		// release it
		_pModelInstanceStock->Release(mi_pmisSerial);
		mi_pmisSerial = NULL; // Release it only once
	}

	// for each child of this model instance
	INDEX ctcmi = mi_cmiChildren.Count();
	for(INDEX icmi=0; icmi<ctcmi; icmi++) {
		// delete child
		CModelInstance *pcmi = &mi_cmiChildren[0];
		mi_cmiChildren.Remove(pcmi);
		DeleteModelInstance(pcmi);
	}

	// release all meshes in stock used by mi
	INDEX ctmshi = mi_aMeshInst.Count();
	for(INDEX imshi=0; imshi<ctmshi; imshi++) {
		MeshInstance &mshi = mi_aMeshInst[imshi];
		CMesh *pmesh = mshi.mi_pMesh;
		if(pmesh != NULL) {
			_pMeshStock->Release(pmesh);
		}
		// release all textures in stock used by mesh
		INDEX ctti = mshi.mi_tiTextures.Count();
		for(INDEX iti=0;iti<ctti;iti++) {
			TextureInstance &ti = mshi.mi_tiTextures[iti];
			ti.ti_toTexture.SetData(NULL);
		}
	}
	mi_aMeshInst.Clear();
	// release skeleton in stock used by mi(if it exist)
	if(mi_psklSkeleton != NULL) {
		_pSkeletonStock->Release(mi_psklSkeleton);
		mi_psklSkeleton = NULL;
	}

	// release all animsets in stock used by mi
	INDEX ctas = mi_aAnimSet.Count();
	for(INDEX ias=0;ias<ctas;ias++) {
		_pAnimSetStock->Release(&mi_aAnimSet[ias]);  
	}
	mi_aAnimSet.Clear();

	// clear all colision boxes 
	mi_cbAABox.Clear();
	// clear anim list
	mi_aqAnims.aq_Lists.Clear();
}

//0528 kwon
void CModelInstance::DeleteMesh(CTString strName)
{
	INDEX ctmshi = mi_aMeshInst.Count();
	
	//0520 kwon
	for(INDEX imshi=0;imshi<ctmshi;imshi++) {
		MeshInstance &mshi = mi_aMeshInst[imshi];
		if(mshi.mi_pMesh->GetName() == strName)
		{
			CMesh *pmesh = mshi.mi_pMesh;
			if(pmesh != NULL) {
				_pMeshStock->Release(pmesh);
			}

			// release all textures in stock used by mesh
			INDEX ctti = mshi.mi_tiTextures.Count();
			for(INDEX iti=0;iti<ctti;iti++) {
				TextureInstance &ti = mshi.mi_tiTextures[iti];
				ti.ti_toTexture.SetData(NULL);
			}
			
			//0519 kwon	
			// count mesh instances
			//	INDEX ctmshi=pmiSelected->mi_aMeshInst.Count();
			INDEX itmpmshi=0;
			CStaticArray<struct MeshInstance> atmpMeshInst;
			// create array for mesh instances 
			atmpMeshInst.New(ctmshi-1);
			// for each mesh instance in selected model instance 
			for(INDEX imshi=0;imshi<ctmshi;imshi++) {
				MeshInstance *ptmpmshi = &mi_aMeshInst[imshi];		
				// if mesh instance is diferent then selected mesh instance
				if(ptmpmshi != &mshi) {
					// copy it to temp array			
					atmpMeshInst[itmpmshi] = *ptmpmshi;
					itmpmshi++;
				}
			}
			// clear mesh instance
			mi_aMeshInst.Clear();
			mi_aMeshInst.CopyArray(atmpMeshInst);
			break;
		}
	}
//안태훈 수정 시작	//(Effect Add & Modify for Close Beta)(0.1)
	this->RefreshTagManager();
//안태훈 수정 끝	//(Effect Add & Modify for Close Beta)(0.1)
}

//0528 kwon
MeshInstance* CModelInstance::AddArmor(const CTFileName &fnMesh)
{
	int ctMeshInst = mi_aMeshInst.Count();
	mi_aMeshInst.Expand(ctMeshInst+1);
	memset(&mi_aMeshInst[ctMeshInst],0,sizeof(mi_aMeshInst[ctMeshInst]));
	mi_aMeshInst[ctMeshInst].mi_pMesh = _pMeshStock->Obtain_t(fnMesh);
	
	this->RefreshTagManager();

	return &mi_aMeshInst[ctMeshInst];
}

// Count used memory
SLONG CModelInstance::GetUsedMemory(void)
{
	SLONG slMemoryUsed = sizeof(*this);
	// Count mesh instances
	INDEX ctmshi = mi_aMeshInst.Count();
	for(INDEX imshi=0;imshi<ctmshi;imshi++) {
		MeshInstance &mshi = mi_aMeshInst[imshi];
		slMemoryUsed += mshi.mi_tiTextures.Count() * sizeof(TextureInstance);
	}
	slMemoryUsed += mi_aMeshInst.Count() * sizeof(MeshInstance);
	// Count bounding boxes
	slMemoryUsed += mi_cbAABox.Count() * sizeof(ColisionBox);
	// Cound child model instances
	INDEX ctcmi = mi_cmiChildren.Count();
	for(INDEX icmi=0;icmi<ctcmi;icmi++) {
		CModelInstance &cmi = mi_cmiChildren[icmi];
		slMemoryUsed += cmi.GetUsedMemory();
	}
	return slMemoryUsed;
}

// set parser to remember file names for modelinstances
void CModelInstance::EnableSrcRememberFN(BOOL bEnable)
{
	_bRememberSourceFN = bEnable;
}

void CModelInstance::EnableVertexArrayKeeping(BOOL bEnable)
{
	_bKeepVertexArrays = bEnable;
}

CModelInstanceSerial::CModelInstanceSerial() 
{
	mis_pmiModelInstance = NULL;
}

CModelInstanceSerial::~CModelInstanceSerial()
{
	// model instance exists
	if(mis_pmiModelInstance!=NULL) {
		// When deleting model instance do not release model instance serail (this)
		if(mis_pmiModelInstance->mi_pmisSerial!=NULL) {
			ASSERT(mis_pmiModelInstance->mi_pmisSerial->ser_ctUsed==0);
		}
		mis_pmiModelInstance->mi_pmisSerial = NULL;
		// delete model instance 
		DeleteModelInstance(mis_pmiModelInstance);
		// reset pointer to model instance
		mis_pmiModelInstance = NULL;
	}
}
#define SEARCH_STRING "Data\\Models\\Enemies\\Cucurbito\\Cucurbito.smc"
// Read model instance from stream
void CModelInstanceSerial::Read_t( CTStream *istrFile)
{
	if(ser_FileName==SEARCH_STRING) {
		int a=0;
	}
	extern INDEX ska_bReplaceSmcWithBmc;
	ASSERT(mis_pmiModelInstance==NULL);
	// if using bmcs instead of smcs
	if(ska_bReplaceSmcWithBmc) {
		mis_pmiModelInstance = CreateModelInstance("");
		ASSERT(mis_pmiModelInstance!=NULL);
		// Read it binary but dont mark it as used in stock
		ReadModelInstance_t(*istrFile,*mis_pmiModelInstance,FALSE);
	// else if this is smc file load it
	} else {
		const CTFileName fnmBmc = istrFile->GetDescription();
		const CTFileName fnmSmc = fnmBmc.NoExt()+".smc";
		mis_pmiModelInstance = ParseSmcFile_t(fnmSmc);
	}
	ASSERT(mis_pmiModelInstance!=NULL);
	ASSERT(mis_pmiModelInstance->mi_pmisSerial==NULL);
	mis_pmiModelInstance->mi_pmisSerial=this;
	ASSERT(mis_pmiModelInstance!=NULL);
	// Remeber pointer to model instance serial from model instance
}
// Write model instance in stream
void CModelInstanceSerial::Write_t( CTStream *ostrFile)
{
	ASSERTALWAYS("CModelInstanceSerial cannot be writen");
}

void CModelInstanceSerial::Clear(void)
{
	ASSERTALWAYS("CModelInstanceSerial cannot be cleared");
}
// Count used memory
SLONG CModelInstanceSerial::GetUsedMemory(void)
{
	if (mis_pmiModelInstance==NULL) {
		return sizeof(*this);
	} else {
		return sizeof(*this)+mis_pmiModelInstance->GetUsedMemory();
	}
}
/* Get the description of this object. */
CTString CModelInstanceSerial::GetDescription(void)
{
	return CTString("");
}


// Create new empty model instance 
extern CModelInstance *CreateModelInstance(CTString strName)
{
	CModelInstance *pmi = new CModelInstance;
	pmi->SetName(strName);
	return pmi;
}

// Delete model instance
extern void DeleteModelInstance(CModelInstance *pmi)
{
	ASSERT(pmi!=NULL);

	if (_pModelInstanceStock!=NULL) {
		// if model instance is valid
		if(pmi!=NULL) {
			// Clear model instance
			pmi->Clear();
			// Delete model instance
			delete pmi;
			pmi = NULL;
		}
	}
}

//0601 kwon 함수 추가.
extern  CModelInstance* ObtainModelInstanceItem_t(CModelInstance &mi, const CTString &fnSmcFile)
{
	ASSERT(&mi!=NULL);
	// Obtain model instance from stock
	CModelInstanceSerial *pmis = _pModelInstanceStock->Obtain_t(fnSmcFile);
	ASSERT(pmis!=NULL && pmis->mis_pmiModelInstance!=NULL);
//0521 kwon
//	pmis->mis_pmiModelInstance->mi_aMeshInst
	// Make copy of model instance in stock (add new reference in stock)
	mi.Copy(*pmis->mis_pmiModelInstance);
	// Remove one reference from stock
	_pModelInstanceStock->Release(mi.mi_pmisSerial);

//안태훈 수정 시작	//(Remake Effect)(0.1)
	mi.RefreshTagManager();
	return 	&mi;
//	return 	pmis->mis_pmiModelInstance;	//원본.
//안태훈 수정 끝	//(Remake Effect)(0.1)
}


// Create model instance and obtain model instance from stock in it
extern void ObtainModelInstance_t(CModelInstance &mi, const CTString &fnSmcFile)
{
	ASSERT(&mi!=NULL);
	// Obtain model instance from stock
	CModelInstanceSerial *pmis = _pModelInstanceStock->Obtain_t(fnSmcFile);
	ASSERT(pmis!=NULL && pmis->mis_pmiModelInstance!=NULL);
	// Make copy of model instance in stock (add new reference in stock)
	mi.Copy(*pmis->mis_pmiModelInstance);
	// Remove one reference from stock
	_pModelInstanceStock->Release(mi.mi_pmisSerial);
//안태훈 수정 시작	//(Remake Effect)(0.1)
	mi.RefreshTagManager();
//안태훈 수정 끝	//(Remake Effect)(0.1)
}

// Obtain model instance from stock in existing model instance
extern CModelInstance *ObtainModelInstance_t(const CTString &fnSmcFile)
{
	CModelInstance *pmi = CreateModelInstance("");
	ObtainModelInstance_t(*pmi,fnSmcFile);
	return pmi;
}

// Create model instance and parse smc file in it
extern CModelInstance *ParseSmcFile_t(const CTString &fnSmcFile)
{
	_yy_mi = NULL;
	// Create new model instance for parser
	_yy_mi = CreateModelInstance("");
	try {
		// Parse given smc file
		ParseSmcFile_t(*_yy_mi,fnSmcFile);
	} catch(char*) {
		if(_yy_mi!=NULL) {
			_yy_mi->Clear();
			throw;
		}
	}

	return _yy_mi;
}

// Parse smc file in existing model instance
extern void ParseSmcFile_t(CModelInstance &mi, const CTString &fnSmcFile)
{
	// Clear given model instance before parsing
	mi.Clear();

	CTFileName fnFileName = fnSmcFile;
	try {
		fnFileName.RemoveApplicationPath_t();
	} catch (char *) {
	}

	CTString strIncludeFile;
	strIncludeFile.Load_t(fnFileName);

	_yy_mi = &mi;
	SMCPushBuffer(fnFileName, strIncludeFile, TRUE);
	syyparse();
}


//안태훈 수정 시작	//(Remake Effect)(0.1)
void CModelInstance::RefreshTagManager()
{
	//CPrintF("Call RefreshTagManager() \n");
	CSkaTag *pTag1 = (CSkaTag*)m_tmSkaTagManager.Find("__ROOT");
	CSkaTag *pTag2 = (CSkaTag*)m_tmSkaTagManager.Find("__TOP");
	if(pTag1 != NULL || pTag2 != NULL)
	{
		CRefCountPtr<CSkaTag> ptrTag1( pTag1 );
		CRefCountPtr<CSkaTag> ptrTag2( pTag2 );
		m_tmSkaTagManager.Clear();
		if(pTag1) m_tmSkaTagManager.RegisterNoCopy(pTag1);
		if(pTag2) m_tmSkaTagManager.RegisterNoCopy(pTag2);
	}
	else 
	{
		m_tmSkaTagManager.Clear();
	}
	for(INDEX i=0; i<mi_aMeshInst.Count(); ++i)
	{
		CMesh &mesh = *(mi_aMeshInst[i].mi_pMesh);
		for(INDEX j=0; j<mesh.msh_aMeshLODs.Count(); ++j)
		{
			MeshLOD &mlod = mesh.msh_aMeshLODs[j];
			for(INDEX k=0; k<mlod.mlod_aSkaTags.Count(); ++k)
			{
				CTag *ptrTag = mlod.mlod_aSkaTags[k].GetNative();
				std::string strNewName = ptrTag->GetName();
				if(!m_tmSkaTagManager.Register(ptrTag))
				{
				//	ASSERTALWAYS("태그 등록이 실패하였다. 이름 좀 잘짓자.");
				}
				else	//등록성공, GroupTag처리, __0_, __1_, __2_접두를 가진 녀석이 GroupTag가 된다.
				{
					ptr_tag pRegisteredTag( m_tmSkaTagManager.Find(strNewName) );
					ASSERT(pRegisteredTag.NotNull());
					if( strNewName.at(0) == '_' && strNewName.at(1) == '_' && strNewName.at(3) == '_'
					&& (strNewName.at(2) == '0' || strNewName.at(2) == '1' || strNewName.at(2) == '2') )	//접두어 확인.
					{
						CTag *pTag = m_tmSkaTagManager.Find(strNewName.substr(4, strNewName.length() - 4));
						CGroupTag *pGTag = NULL;
						if(pTag == NULL)	//아직 groupTag가 등록되지 않았으므로 등록이 필요.
						{
							CGroupTag gt;
							gt.Activate();
							gt.SetName(strNewName.substr(4, strNewName.length() - 4).c_str());
							m_tmSkaTagManager.Register(&gt);
							CTag *pTag = m_tmSkaTagManager.Find(strNewName.substr(4, strNewName.length() - 4));
							ASSERT(pTag->GetType() == TT_GROUP);
							pGTag = (CGroupTag*)pTag;
						}
						else
						{
							ASSERT(pTag->GetType() == TT_GROUP);
							pGTag = (CGroupTag*)pTag;
						}
						pGTag->AddTag( pRegisteredTag );
					}
				}
			}
		}
	}
}

//안태훈 수정 시작	//(Effect Add & Modify for Close Beta)(0.1)
#include <string>
void CModelInstance::ReadAnimEffectFile(const CTFileName &fnm)
{
	m_vectorAnimEffect.clear();
	m_fnmAnimEffectFile = fnm;

	BOOL bSuccess = TRUE;
	INDEX iAnimSetIndex, iAnimIndex;

	CTFileStream is;
	is.Open_t(fnm);
	
	is.ExpectID_t("AEFF");
	INDEX count;
	is >> count;

	if (count > 0)
	{
		m_vectorAnimEffect.reserve(count);
	}

	extern BOOL _bSkaStudioApp;

	CAnimEffect temp;
	std::vector<std::string> vectorLostAni;
	for(INDEX i=0; i<count; ++i)
	{
		temp.Read(&is);

		if (_bSkaStudioApp)
		{
			if(FindAnimationByID(temp.GetAnimID(), &iAnimSetIndex, &iAnimIndex))
				m_vectorAnimEffect.push_back(temp);
			else
			{
				bSuccess = FALSE;
				vectorLostAni.push_back(std::string(ska_IDToString(temp.GetAnimID()).str_String));
			}
		}
		else
		{
			m_vectorAnimEffect.push_back(temp);
		}
	}

	if(!bSuccess && _bSkaStudioApp)
	{
		std::string strMsg = "AnimEffect에 저장된 Ani중 없는 애니가 있습니다. "\
							"그 애니를 빼고 저장하시려면 그냥 저장하시고 "\
							"아니라면 저장하지 마세요. 없는 애니 : ";
		for(int i=0; i<vectorLostAni.size(); ++i)
		{
			strMsg += std::string(vectorLostAni[i]);
			if(i != vectorLostAni.size()-1) strMsg += std::string(", ");
		}
		MessageBox(NULL, strMsg.c_str(), "경고!", MB_OK | MB_ICONWARNING);
	}
}

void CModelInstance::WriteAnimEffectFile(const CTFileName &fnm)
{
	CTFileStream os;
	os.Create_t(fnm);

	os.WriteID_t("AEFF");
	os << (INDEX)m_vectorAnimEffect.size();
	for(INDEX i=0; i<m_vectorAnimEffect.size(); ++i)
	{
		m_vectorAnimEffect[i].Write(&os);
	}
	m_fnmAnimEffectFile = fnm;
}

void CModelInstance::AddAnimEffect(const CTString &strAnimName, const CTString &strEGName, FLOAT fStartTime, ULONG ulFlags)
{
	INDEX iCnt = m_vectorAnimEffect.size(), i=0;
	for(i=0; i<iCnt; ++i)
	{
		if(m_vectorAnimEffect[i].GetAnimName() == strAnimName)
		{
			m_vectorAnimEffect[i].AddReservedEG(strEGName, fStartTime, ulFlags);
			break;
		}
	}
	if(i == iCnt)
	{
		CAnimEffect ae;
		ae.SetAnimName(strAnimName);
		ae.AddReservedEG(strEGName, fStartTime, ulFlags);
		m_vectorAnimEffect.push_back(ae);
	}
}

void CModelInstance::DeleteAnimEffect(const CTString &strAnimName)
{
	INDEX iCnt = m_vectorAnimEffect.size(), i=0;
	for(i=0; i<iCnt; ++i)
	{
		if(m_vectorAnimEffect[i].GetAnimName() == strAnimName) break;
	}
	m_vectorAnimEffect[i] = m_vectorAnimEffect[iCnt-1];
	m_vectorAnimEffect.pop_back();
}

BOOL CModelInstance::StartAnimEffect(INDEX iAnimID, FLOAT fSpeedMul)
{
	CAnimEffect *pAnimEffect = GetAnimEffectByAnimIndex(iAnimID);
	if(pAnimEffect == NULL) return FALSE;
	CAnimEffect &animEffect = *pAnimEffect;

	animEffect.SetLoopCount(0);

	const INDEX cntREG = animEffect.GetReservedEGCount();
	BOOL bRet = FALSE;

	EFF_RENDER_TYPE tmpERType = ER_NORMAL;

	if (mi_bDummyModel)
	{
		tmpERType = ER_IN_UI;
	}

	for(INDEX i=0; i<cntREG; ++i)
	{
		CAnimEffect::CReservedEffectGroup *pREG = animEffect.GetReservedEG(i);
		pREG->SetStartedTime(_pTimer->GetLerpedCurrentTick() + pREG->GetStartTime());
		pREG->SetPlayedEG( StartEffectGroup(pREG->GetEffectGroupName().str_String
											, &m_tmSkaTagManager
											, _pTimer->GetLerpedCurrentTick() + pREG->GetStartTime(), tmpERType, mi_ERType) );
		//CPrintF("StartAnimEffect : %s \n", pREG->GetEffectGroupName().str_String);
		if(pREG->GetPlayedEG() == NULL) bRet = FALSE;
		else
		{
			//CPrintF("GetPlayedEG_Start : %s \n", pREG->GetPlayedEG()->GetName().c_str());
			pREG->GetPlayedEG()->SetSpeedMul(fSpeedMul);
			//if(CEntity::GetPlayerEntity(0)->GetModelInstance() == this)
				//CPrintF("---%6.2f---Start Effect : %s\n", _pTimer->GetLerpedCurrentTick(), pREG->GetEffectGroupName().str_String);
			bRet = bRet && TRUE;
		}
	}
	return bRet;
}

void CModelInstance::LoopAnimEffect(INDEX iAnimID, FLOAT fLeftAnimTime
									, FLOAT fOffsetTimeFromAnimLoop, FLOAT fSpeedMul)
{
	CAnimEffect *pAnimEffect = GetAnimEffectByAnimIndex(iAnimID);
	if(pAnimEffect == NULL) return;
	CAnimEffect &animEffect = *pAnimEffect;

	if(animEffect.GetLoopCount() == 0) return;

	EFF_RENDER_TYPE tmpERType = ER_NORMAL;

	if (mi_bDummyModel)
	{
		tmpERType = ER_IN_UI;
	}

	const INDEX cntREG = animEffect.GetReservedEGCount();
	for(INDEX i=0; i<cntREG; ++i)
	{
		CAnimEffect::CReservedEffectGroup *pREG = animEffect.GetReservedEG(i);
		if(!(pREG->GetFlags() & REF_SYNCANIMLOOP)) continue;
		if( pREG->GetPlayedEG() == NULL ) continue;
		FLOAT temp = _pTimer->GetLerpedCurrentTick() - fOffsetTimeFromAnimLoop + pREG->GetStartTime();
		if( pREG->GetPlayedEG() != NULL && CEffectGroupManager::Instance().IsValidCreated(pREG->GetPlayedEG()) )
		{
			if(pREG->GetFlags() & REF_SYNCANIMLENGTH) pREG->GetPlayedEG()->Stop(fLeftAnimTime * 0.5f);
			pREG->SetPlayedEG(NULL);
		}
		//안태훈 : TEST
		//if(fOffsetTimeFromAnimLoop > 0.19f) continue;
		pREG->SetPlayedEG( StartEffectGroup(pREG->GetEffectGroupName().str_String
											, &m_tmSkaTagManager
											, _pTimer->GetLerpedCurrentTick() - fOffsetTimeFromAnimLoop + pREG->GetStartTime(), tmpERType, mi_ERType) );
		pREG->SetStartedTime(_pTimer->GetLerpedCurrentTick() - fOffsetTimeFromAnimLoop + pREG->GetStartTime());
		if(pREG->GetPlayedEG() != NULL)
		{
			pREG->GetPlayedEG()->SetSpeedMul(fSpeedMul);
		}
		//if(CEntity::GetPlayerEntity(0)->GetModelInstance() == this)
			//CPrintF("---%6.2f---Loop Effect : %s\n", _pTimer->GetLerpedCurrentTick(), pREG->GetEffectGroupName().str_String);
	}
}

void CModelInstance::StopAnimEffect(INDEX iAnimID, FLOAT fLeftAnimTime)
{
	CAnimEffect *pAnimEffect = GetAnimEffectByAnimIndex(iAnimID);
	if(pAnimEffect == NULL) return;
	CAnimEffect &animEffect = *pAnimEffect;

	const INDEX cntREG = animEffect.GetReservedEGCount();
	animEffect.SetLoopCount(0);
	for(INDEX i=0; i<cntREG; ++i)
	{
		CAnimEffect::CReservedEffectGroup *pREG = animEffect.GetReservedEG(i);
		//CPrintF("StopAnimEffect : %s \n", pREG->GetEffectGroupName());

		if( pREG->GetPlayedEG() != NULL && CEffectGroupManager::Instance().IsValidCreated(pREG->GetPlayedEG()) )
		{
			CTString strPlayedEG = pREG->GetPlayedEG()->GetName().c_str();

			if (strPlayedEG == pREG->GetEffectGroupName())
			{
				if(pREG->GetFlags() & REF_SYNCANIMLENGTH) pREG->GetPlayedEG()->Stop(fLeftAnimTime * 0.5f);
				else pREG->GetPlayedEG()->Stop();
			}

			pREG->SetPlayedEG(NULL);
			//CPrintF("---%6.2f---Stop Effect : %s\n", _pTimer->GetLerpedCurrentTick(), pREG->GetEffectGroupName().str_String);
		}
		else pREG->SetPlayedEG(NULL);
	}
}

void CModelInstance::StopAllAnimEffect(FLOAT fLeftAnimTime)
{
	const INDEX cntAnimEffect = m_vectorAnimEffect.size();
	INDEX i=0;
	for(i=0; i<cntAnimEffect; ++i)
	{
		CAnimEffect &animEffect = m_vectorAnimEffect[i];
		const INDEX cntREG = animEffect.GetReservedEGCount();
		for(int j=0; j<cntREG; ++j)
		{
			animEffect.SetLoopCount(0);
			CAnimEffect::CReservedEffectGroup *pREG = animEffect.GetReservedEG(j);
			if( pREG != NULL && pREG->GetPlayedEG() != NULL && CEffectGroupManager::Instance().IsValidCreated(pREG->GetPlayedEG()) )
			{
				if(pREG->GetFlags() & REF_SYNCANIMLENGTH) pREG->GetPlayedEG()->Stop(fLeftAnimTime * 0.5f);
				else pREG->GetPlayedEG()->Stop(FLT_MAX);
				pREG->SetPlayedEG(NULL);
				//if(CEntity::GetPlayerEntity(0)->GetModelInstance() == this)
					//CPrintF("---%6.2f---Stop Effect : %s\n", _pTimer->GetLerpedCurrentTick(), pREG->GetEffectGroupName().str_String);
			}
		}
	}
}

CAnimEffect *CModelInstance::GetAnimEffectByAnimIndex(INDEX iAnimID)
{
	if (m_vectorAnimEffect.empty()) return NULL;

	const INDEX cntAnimEffect = m_vectorAnimEffect.size();
	INDEX i=0;
	for(i=0; i<cntAnimEffect; ++i)
	{
		if(m_vectorAnimEffect[i].GetAnimID() == iAnimID) break;
	}
	if(cntAnimEffect == i) return NULL;
	return &m_vectorAnimEffect[i];
}





#define ANIMEFFECT_VERSION 1

CAnimEffect::CAnimEffect()
: m_iAnimID(-1)
{
}

CAnimEffect::CAnimEffect(const CTString &animName)
: m_strAnimName(animName), m_iAnimID(-1)
{
}

CAnimEffect::~CAnimEffect()
{
}

void CAnimEffect::Read(CTStream *pIS)
{
	CTStream &is = *pIS;
	is.ExpectID_t("ANEF");

	UBYTE ubVer = 0;
	is >> ubVer;
	if(ubVer == ANIMEFFECT_VERSION)
	{
		is >> m_strAnimName;
		m_iAnimID = ska_StringToID(m_strAnimName);
		INDEX count;
		is >> count;
		m_vectorReservedEG.resize(count);
		for(INDEX i=0; i<count; ++i)
		{
			CTString strEGName;
			FLOAT fStartTime;
			ULONG flags;
			is >> strEGName;
			is >> fStartTime;
			is >> flags;
			m_vectorReservedEG[i].Set(strEGName, fStartTime, flags);
		}
	}
	else	//old
	{
		//NOTHING
	}
}
void CAnimEffect::Write(CTStream *pOS)
{
	CTStream &os = *pOS;
	os.WriteID_t("ANEF");

	os << (UBYTE)ANIMEFFECT_VERSION;

	os << m_strAnimName;
	os << (INDEX)m_vectorReservedEG.size();
	for(INDEX i=0; i<m_vectorReservedEG.size(); ++i)
	{
		os << m_vectorReservedEG[i].GetEffectGroupName();
		os << m_vectorReservedEG[i].GetStartTime();
		os << m_vectorReservedEG[i].GetFlags();
	}
}
#undef ANIMEFFECT_VERSION
//안태훈 수정 끝	//(Effect Add & Modify for Close Beta)(0.1)
