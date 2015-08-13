#ifndef SE_INCL_MODELINSTANCE_H
#define SE_INCL_MODELINSTANCE_H
#ifdef PRAGMA_ONCE
	#pragma once
#endif

#include <Engine/Ska/Skeleton.h>
#include <Engine/Ska/AnimSet.h>
#include <Engine/Ska/StringTable.h>
#include <Engine/Math/AABBox.h>
#include <Engine/Templates/StaticStackArray.h>
#include <Engine/Templates/StaticArray.h>
#include <Engine/Templates/DynamicContainer.h>
//안태훈 수정 시작	//(Add & Modify SSSE Effect)(0.1)
#include <Engine/Effect/CSkaTag.h>
#include <Engine/Effect/CTagManager.h>
//안태훈 수정 끝	//(Add & Modify SSSE Effect)(0.1)

//강동민 수정 시작 Water 구현		04.22
//#include <Engine/Effect/CNiceWater.h>
//강동민 수정 끝 Water 구현			04.22

// nubers are same as in models.h
#define SKA_HEIGHT_EQ_WIDTH 0
#define SKA_LENGTH_EQ_WIDTH 1
#define SKA_LENGTH_EQ_HEIGHT 2

struct ColisionBox
{
	ColisionBox() {};
	ColisionBox(FLOAT3D vMin,FLOAT3D vMax) {
		SetMin(vMin);
		SetMax(vMax);
		SetName("Default");
	};
	inline FLOAT3D &Min() {return cb_vMin;}
	inline FLOAT3D &Max() {return cb_vMax;}
	inline void SetMin(FLOAT3D &vMin) {cb_vMin = vMin;}
	inline void SetMax(FLOAT3D &vMax) {cb_vMax = vMax;}
	inline void SetName(CTString strName) {
		cb_strName = strName;
		if(cb_strName!="") {
			cb_iBoxID  = ska_GetIDFromStringTable(cb_strName);
		} else {
			cb_iBoxID = -1;
		}
	}
	inline const CTString &GetName() {return cb_strName;}
	inline const INDEX GetID() {return cb_iBoxID;}
private:
	FLOAT3D  cb_vMin;
	FLOAT3D  cb_vMax;
	CTString cb_strName;
	INDEX    cb_iBoxID;
};

struct MeshInstance
{
	MeshInstance() {
		mi_pMesh = NULL;
	}
	CMesh *mi_pMesh;
	CStaticArray<struct TextureInstance> mi_tiTextures;
};

struct TextureInstance
{
	// Constructor
	TextureInstance(void) {
		ti_iTextureID = -1;
	}

	// Copy operator
	TextureInstance &operator=(const TextureInstance &tiOther) {
		ti_iTextureID = tiOther.ti_iTextureID;
		TextureInstance &ti = (TextureInstance&)tiOther;

		CTString strTexName = ti.ti_toTexture.GetName();
		ti_toTexture.SetData_t(strTexName);
		ti.ti_toTexture.SetData(NULL);
		return *this;
	}

	INDEX GetID(void )               { return ti_iTextureID; }
	void SetName(CTString &strTexID) { ti_iTextureID = ska_GetIDFromStringTable(strTexID); }
	void SetName(INDEX iTexID)       { ti_iTextureID = iTexID; }

public:
	CTextureObject ti_toTexture;
private:
	INDEX ti_iTextureID;
};

struct AnimQueue
{
	CStaticStackArray<struct AnimList> aq_Lists; // Array of currently playing anim lists
};

struct AnimList
{
	FLOAT al_fStartTime; // Time when this list was created
	FLOAT al_fFadeTime;  // Time when this list will fade in
	CStaticStackArray<struct PlayedAnim> al_PlayedAnims;  // Array of currently playing anims in this list
};

struct PlayedAnim
{
	FLOAT pa_fStartTime; // Time when this animation was started
	FLOAT pa_fSpeedMul;  // Speed multiplier
	INDEX pa_iAnimID;    // Animation id
	ULONG pa_ulFlags;    // Animation flags
	FLOAT pa_fStrength;  // Animation strength
	INDEX pa_GroupID;    // Group ID
};

//안태훈 수정 시작	//(Effect Add & Modify for Close Beta)(0.1)
#include <vector>
#include <Engine/Effect/CEffectGroupManager.h>
#include <Engine/Effect/CEffectGroup.h>


#define REF_NOTHING			(0UL)
#define REF_SYNCANIMLENGTH	(1UL<<0)	//effect group의 길이를 anim가 플레이되는 시간보다 길지 않게 만든다.
#define REF_SYNCANIMLOOP	(1UL<<1)	//anim이 loop될때 effect도 같이 재시작함. 단 전의 effect는 stop함.
#define REF_SYNCANIMSPEED	(1UL<<2)	//anim의 speed와 effect의 길이의 비율을 유지한다.

#define ANIMEFFECT_VERSION 1
class ENGINE_API CAnimEffect
{
public:
	class CReservedEffectGroup
	{
	public:
		CReservedEffectGroup() : m_pPlayedEffectGroup(NULL) {}
		~CReservedEffectGroup()
		{
			if(m_pPlayedEffectGroup != NULL
				&& CEffectGroupManager::Instance().IsValidCreated(m_pPlayedEffectGroup))
			{
				DestroyEffectGroup(m_pPlayedEffectGroup);
			}
		}

		bool operator < (const CReservedEffectGroup &other)	const { return m_fStartTime < other.m_fStartTime;	}
		bool operator > (const CReservedEffectGroup &other)	const { return m_fStartTime > other.m_fStartTime;	}
		bool operator < (CReservedEffectGroup &other)	{ return m_fStartTime < other.m_fStartTime;	}
		bool operator > (CReservedEffectGroup &other)	{ return m_fStartTime > other.m_fStartTime;	}

		inline void Set(const CTString &egName, FLOAT startTime, ULONG flags = REF_NOTHING)
		{
			m_strEffectGroupName = egName;
			m_fStartTime = startTime;
			m_ulFlags = flags;
		}

		inline void SetEffectGroupName(const CTString &egName)	{ m_strEffectGroupName = egName;	}
		inline CTString GetEffectGroupName()					{ return m_strEffectGroupName;		}

		inline void SetStartTime(FLOAT time)	{ m_fStartTime = time;	}
		inline FLOAT GetStartTime()				{ return m_fStartTime;	}

		inline void SetStartedTime(FLOAT time)	{ m_fStartedTime = time;	}
		inline FLOAT GetStartedTime()			{ return m_fStartedTime;	}

		inline void SetPlayedEG(CEffectGroup *pEG)	{ m_pPlayedEffectGroup = pEG;	}
		inline CEffectGroup *GetPlayedEG()			{ return m_pPlayedEffectGroup;	}

		inline void SetFlags(ULONG ulFlag)		{ m_ulFlags = ulFlag;	}
		inline ULONG GetFlags()					{ return m_ulFlags;		}

	protected:
		//content
		CTString	m_strEffectGroupName;
		FLOAT		m_fStartTime;
		ULONG		m_ulFlags;
		//instance
		FLOAT		m_fStartedTime;
		CEffectGroup	*m_pPlayedEffectGroup;
	};
	typedef std::vector<CReservedEffectGroup> vector_reg;
public:
	CAnimEffect();
	CAnimEffect(const CTString &animName);
	~CAnimEffect();

	inline void SetAnimName(const CTString &animName)
	{
		m_strAnimName = animName;
		m_iAnimID = ska_StringToID(m_strAnimName);
	}
	inline const CTString &GetAnimName() const	{ return m_strAnimName;		}
	inline INDEX GetAnimID()	{ return m_iAnimID;	}

	inline INDEX GetReservedEGCount()	{ return m_vectorReservedEG.size();	}
	inline CReservedEffectGroup *GetReservedEG(INDEX index)
	{
		if(index < 0 || index >= m_vectorReservedEG.size()) return NULL;
		return &m_vectorReservedEG[index];
	}
	inline void AddReservedEG(const CTString &egName, FLOAT startTime, ULONG flags = REF_NOTHING)
	{
		CReservedEffectGroup reg;
		reg.Set(egName, startTime, flags);
		m_vectorReservedEG.push_back(reg);
	}
	inline BOOL DeleteReservedEG(INDEX index)
	{
		if(index < 0 || index >= m_vectorReservedEG.size()) return FALSE;

		m_vectorReservedEG.erase( m_vectorReservedEG.begin() + index );
		return TRUE;
	}

	inline void SetLoopCount(INDEX cnt)	{ m_iLoopCount = cnt;	}
	inline INDEX GetLoopCount()			{ return m_iLoopCount;	}
public:
	friend class CModelInstance;

protected:
	//content
	CTString	m_strAnimName;
	vector_reg	m_vectorReservedEG;
	//instance
	INDEX		m_iAnimID;
	INDEX		m_iLoopCount;

	void Read(CTStream *pIS);
	void Write(CTStream *pOS);
};
#undef ANIMEFFECT_VERSION
//안태훈 수정 끝	//(Effect Add & Modify for Close Beta)(0.1)

class ENGINE_API CModelInstance
{
public:
	CModelInstance();
	~CModelInstance();
	CModelInstance(CModelInstance &miOther);
	void operator=(CModelInstance &miOther);

	// Add child model instance
	void AddChild(CModelInstance *pmi, INDEX iParentBoneID = -1);
	// Add new mesh to model instance
	void AddMesh_t(const CTFileName &fnMesh);
	// Add new skeleton to model instance
	void AddSkeleton_t(const CTFileName &fnSkeleton);
	// Add new animset to model instance
	void AddAnimSet_t(const CTFileName &fnAnimSet);
	// Add new texture to model instance
	void AddTexture_t(const CTFileName &fnTexture, CTString strTexID, MeshInstance *pmshi);
	// Add new texture to model instance
	void AddTexture_t(const CTFileName &fnTexture, INDEX iTexID, MeshInstance *pmshi);

	// Remove model instance child (does not delete model instance)
	void RemoveChild(CModelInstance *pmi);
	// Remove all child model instances (does not delete them)
	void RemoveAllChildren(BOOL bRecursive = FALSE);
	// Remove child model instance and delete it
	void DeleteChild(CModelInstance *pmi);
	// Remove all child model instances and deletes them
	void DeleteAllChildren(BOOL bRecursive = FALSE);
	// Remove one texture from model instance
	void RemoveTexture(TextureInstance *ptiRemove,MeshInstance *pmshi);

	//0528 kwon
	MeshInstance* AddArmor(const CTFileName &fnMesh);

	// Find mesh instance in model instance
	MeshInstance    *FindMeshInstance(INDEX iMeshID);
	// Find texture instance in all mesh instances in model instance
	TextureInstance *FindTexureInstance(INDEX iTexID);
	// Find texture instance in given mesh instance
	TextureInstance *FindTexureInstance(INDEX iTexID, MeshInstance &mshi);


	// Copy mesh instance for other model instance
	void CopyMeshInstance(CModelInstance &miOther);
	// Get child of model instance
	CModelInstance *GetChild(INDEX iChildID, BOOL bRecursive = FALSE);
	// Set parent bone of model instance
	void SetParentBone(INDEX iParentBoneID);
	// Get parent of model instance
	CModelInstance *GetParent(CModelInstance *pmiStartFrom);
	// Get first parent that does not reference its child model instance
	CModelInstance *GetFirstNonReferencedParent(CModelInstance *pmiRoot);
	// Change parent of model instance
	void ChangeParent(CModelInstance *pmiOldParent, CModelInstance *pmiNewParent);

	// Set model instance offset (position,rotation)
	void SetOffset(FLOAT fOffset[6]);
	// 
	void SetRideOffset(FLOAT fOffset[6]);
	// Set model instance position offset
	void SetOffsetPos(FLOAT3D vPos);
	void SetRideOffsetPos(FLOAT3D vPos);
	// Set model instance rotation offset
	void SetOffsetRot(ANGLE3D aRot);
	void SetRideOffsetRot(ANGLE3D aRot);
	// Get model instance position offset
	FLOAT3D GetOffsetPos();
	FLOAT3D GetRideOffsetPos();
	// Get model instance rotation offset
	ANGLE3D GetOffsetRot();
	ANGLE3D GetRideOffsetRot();

	// Stretch model instance
	void StretchModel(FLOAT3D &vStretch);
	// Stretch model instance without attachments
	void StretchSingleModel(FLOAT3D &vStretch);
	// Add new cloned anim state
	void NewClonedState(FLOAT fFadeTime);
	// Add new clear anim state
	void NewClearState(FLOAT fFadeTime);
	// Sets name of model instance
	void SetName(const CTString &strName);
	// Gets name of model instance
	CTString GetName(void);
	// Gets id of model instance
	const INDEX &GetID(void);

	// Add animation to last anim queue
	void AddAnimation(INDEX iAnimID, DWORD dwFlags, FLOAT fStrength, INDEX iGroupID, FLOAT fSpeedMul = 1.0f);	
	void AddAnimationChild(INDEX iAnimID, DWORD dwFlags, FLOAT fStrength, INDEX iGroupID, FLOAT fSpeedMul = 1.0f);//0409
	// Remove all animations before last animation that has fully faded in
	void RemovePassedAnimsFromQueue(void);
	// Remove animation from anim queue
	void RemAnimation(INDEX iAnimID);
	// Remove all animations from anim queue with same ID
	void RemAnimsWithID(INDEX iGroupID);
	// Stop all animations in anim queue
	void StopAllAnimations(FLOAT fFadeTime);
	// Offset all animations in anim queue
	void OffSetAnimationQueue(TIME fOffsetTime);
	// Find animation by ID
	BOOL FindAnimationByID(INDEX iAnimID, INDEX *piAnimSetIndex, INDEX *piAnimIndex);
	// Find first animation of all animations in ModelInstance (safety function)
	INDEX FindFirstAnimationID();
	// Get time that passed from animation start
	FLOAT GetAnimPlayingTime(INDEX iAnimID);
	// Get animation length
	FLOAT GetAnimLength(INDEX iAnimID, BOOL bLoopingAnim=FALSE);
	// Check if given animation is currently playing
	BOOL IsAnimationPlaying(INDEX iAnimID);
	// Add flags to animation playing in anim queue
	BOOL AddFlagsToPlayingAnim(INDEX iAnimID, ULONG ulFlags);

	// Get model instance color
	COLOR GetModelColor(void);
	// Set model instance color
	void SetModelColor(COLOR colNewColor);

	// Test it the model instance uses alpha blending
	BOOL HasAlpha(void);
	// Test it the model instance has shadow
	BOOL HasShadow(FLOAT fMipFactor);
	// Test it the model instance is visible at given distance
	BOOL IsModelVisible( FLOAT fMipDistance);


	void AddSimpleShadow( const FLOAT fIntensity, const FLOATplane3D &plShadowPlane);
	void GetModelVertices( CStaticStackArray<FLOAT3D> &avVertices, FLOATmatrix3D &mRotation,FLOAT3D &vPosition, FLOAT fNormalOffset, FLOAT fDistance);

	// Get model instance colision box
	ColisionBox &GetColisionBox(INDEX icb);
	// Get current model instance colision box
	ColisionBox &GetCurrentColisionBox();
	// Get current model instance colision box
	void GetCurrentColisionBox(FLOATaabbox3D &paabbox);
	// Get model instance all frames bounbing box
	void GetAllFramesBBox(FLOATaabbox3D &aabbox);
	// Get colision box min vector
	FLOAT3D GetCollisionBoxMin(INDEX iCollisionBox=0);
	// Get colision box max vector
	FLOAT3D GetCollisionBoxMax(INDEX iCollisionBox=0);
	// Get colision box dimension equality
	INDEX GetCollisionBoxDimensionEquality(INDEX iCollisionBox=0);
	// Get index of colision box from colision box name
	INDEX GetColisionBoxIndex(INDEX iBoxID);
	// Add new colision box to model instance
	void AddColisionBox(CTString strName,FLOAT3D vMin,FLOAT3D vMax);
	// Remove colision box from model instance
	void RemoveColisionBox(INDEX iIndex);
	// Get index of current colision box
	INDEX GetCurrentColisionBoxIndex(void);
	// Set index of current colision box
	void  SetCurrentColisionBoxIndex(INDEX iNewIndex);

	// Copy model instance data from other mi
	void Copy(CModelInstance &miOther);
	// Synchronize with another model (copy animations/attachments positions etc from there)
	void Synchronize(CModelInstance &miOther);

	// Clear model instance
	void Clear(void);
	// Flag for parser to remember source file names (used only in ska studio)
	static void EnableSrcRememberFN(BOOL bEnable);
	// Flag for mesh loader to keep vertices, normals and texcoords in arrays
	static void EnableVertexArrayKeeping(BOOL bEnable);
	// Count used memory
	SLONG GetUsedMemory(void);
//0220 kwon
	void DeleteMesh(CTString strName);
//안태훈 수정 시작	//(Remake Effect)(0.1)
	void ReadAnimEffectFile(const CTFileName &fnm);
	void WriteAnimEffectFile(const CTFileName &fnm);
	void AddAnimEffect(const CTString &strAnimName, const CTString &strEGName, FLOAT fStartTime, ULONG ulFlags = REF_NOTHING);
	void DeleteAnimEffect(const CTString &strAnimName);

	BOOL StartAnimEffect(INDEX iAnimID, FLOAT fSpeedMul);
	void LoopAnimEffect(INDEX iAnimID, FLOAT fLeftAnimTime, FLOAT fOffsetTimeFromAnimLoop, FLOAT fSpeedMul);
	void StopAnimEffect(INDEX iAnimID, FLOAT fLeftAnimTime);
	void StopAllAnimEffect(FLOAT fLeftAnimTime);

	CAnimEffect *GetAnimEffectByAnimIndex(INDEX iAnimID);

	void RefreshTagManager();

	CTFileName m_fnmAnimEffectFile;
	std::vector<CAnimEffect> m_vectorAnimEffect;
//안태훈 수정 끝	//(Remake Effect)(0.1)

public:
	CSkeleton *mi_psklSkeleton;                     // pointer to skeleton object
	CStaticArray<struct MeshInstance> mi_aMeshInst; // array of mesh instances
	CStaticArray<struct ColisionBox>  mi_cbAABox;   // array of colision boxes
	CDynamicContainer<class CAnimSet> mi_aAnimSet;  // array of animsets
	CDynamicContainer<class CModelInstance> mi_cmiChildren; // array of child model instances
	class CModelInstanceSerial *mi_pmisSerial;
//안태훈 수정 시작	//(Add & Modify SSSE Effect)(0.1)
	CTagManager		m_tmSkaTagManager;
//안태훈 수정 끝	//(Add & Modify SSSE Effect)(0.1)

	AnimQueue mi_aqAnims;   // current animation queue for this model instance
	QVect mi_qvOffset;      // current offset from parent model instance
	QVect mi_qvRideOffset;	// pet rinding offset from child model instance(dummy)
	INDEX mi_iParentBoneID; // ID of parent bone in parent model instance
	INDEX mi_iRideParentBoneID; //ID of pet riding parent bone for child model instance
	INDEX mi_iCurentBBox;   // index of current colision box in colision box array
	COLOR mi_colModelColor; // color of this model instance
	FLOAT3D mi_vStretch;    // stretch of this model instance
	ColisionBox mi_cbAllFramesBBox; // all frames colision box
	CTFileName mi_fnSourceFile;     // source file name of this model instance (used only for ska studio)
	BOOL mi_bSkipOcclusionTest;     // skip occlussion culling test (for weapons and selectors)

	BOOL			mi_bRenderShadow;	// 03.02
	INDEX			mi_iFaceMesh;
	INDEX			mi_iHairMesh;
	
	BOOL			mi_bDummyModel;
	EFF_RENDER_SUB_TYPE			mi_ERType;
private:
	INDEX mi_iModelID; // ID of this model instance (this is ID for mi_strName)
	CTString mi_strName;    // name of this model instance
}; 


// class that holds model instance in stock
class ENGINE_API CModelInstanceSerial : public CSerial {
public:
	CModelInstanceSerial();
	~CModelInstanceSerial();
	// Read model instance from stream
	void Read_t( CTStream *istrFile);  // throw char *
	// Write model instance in stream
	void Write_t( CTStream *ostrFile); // throw char *
	// Clear model instance 
	void Clear(void);

	// Count used memory
	SLONG GetUsedMemory(void);
	/* Get the description of this object. */
	CTString GetDescription(void);
	// check if this kind of objects is auto-freed
	BOOL IsAutoFreed(void) { return FALSE; };
public:
	CModelInstance *mis_pmiModelInstance;
};


// Create new empty model instance 
ENGINE_API extern CModelInstance *CreateModelInstance(CTString strName);
// Delete model instance
ENGINE_API extern void DeleteModelInstance(CModelInstance *pmi);
// Create model instance and obtain model instance from stock in it
ENGINE_API extern void ObtainModelInstance_t(CModelInstance &mi, const CTString &fnSmcFile);
// Obtain model instance from stock in existing model instance
ENGINE_API extern CModelInstance *ObtainModelInstance_t(const CTString &fnSmcFile);
// Create model instance and parse smc file in it
ENGINE_API extern CModelInstance *ParseSmcFile_t(const CTString &fnSmcFile);
// Parse smc file in existing model instance
ENGINE_API extern void ParseSmcFile_t(CModelInstance &mi, const CTString &fnSmcFile);

// Calculate fading factor for animation list
ENGINE_API extern FLOAT CalculateFadeFactor(const AnimList &alList);

ENGINE_API extern  CModelInstance* ObtainModelInstanceItem_t(CModelInstance &mi, const CTString &fnSmcFile);

#endif  /* include-once check. */
