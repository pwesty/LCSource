#include "stdh.h"

#include <Engine/Entities/Entity.h>
#include <Engine/Entities/EntityClass.h>
#include <Engine/Entities/EntityProperties.h>
#include <Engine/Entities/EntityPointer.h>
#include <Engine/Entities/LastPositions.h>
#include <Engine/Entities/EntityCollision.h>
#include <Engine/Entities/Precaching.h>
#include <Engine/Entities/ShadingInfo.h>
#include <Engine/Light/LightSource.h>

#include <Engine/Math/Geometry.inl>
#include <Engine/Math/Clipping.inl>
#include <Engine/Math/Float.h>
#include <Engine/Math/OBBox.h>
#include <Engine/Math/Functions.h>
#include <Engine/Math/ExtensionMath.h>

#include <Engine/Base/CRC.h>
#include <Engine/Base/Console.h>
#include <Engine/Base/Statistics_Internal.h>
#include <Engine/Base/MemoryTracking.h>
#include <Engine/Network/CNetwork.h>
#include <Engine/Network/Server.h>
#include <Engine/Network/Common.h>
#include <Engine/Network/ClientInterface.h>
#include <Engine/Network/PlayerTarget.h>
#include <Engine/Network/SessionState.h>
#include <Engine/Brushes/Brush.h>
#include <Engine/Brushes/BrushTransformed.h>
#include <Engine/Brushes/BrushArchive.h>
#include <Engine/Terrain/TerrainArchive.h>
#include <Engine/World/World.h>
#include <Engine/World/WorldRayCasting.h>
#include <Engine/Base/ReplaceFile.h>
#include <Engine/Entities/InternalClasses.h>
#include <Engine/Models/ModelObject.h>
#include <Engine/Sound/SoundData.h>
#include <Engine/Sound/SoundObject.h>
#include <Engine/Graphics/Texture.h>
#include <Engine/Ska/Render.h>
#include <Engine/Math/Matrix12.h>
#include <Engine/Terrain/Terrain.h>
#include <Engine/Terrain/TRLayer.h>

#include <Engine/Base/ListIterator.inl>

#include <Engine/Templates/BSP.h>
#include <Engine/Templates/DynamicArray.cpp>
#include <Engine/Templates/DynamicContainer.cpp>
#include <Engine/Templates/StaticArray.cpp>
#include <Engine/Templates/StaticStackArray.cpp>

#include <Engine/Templates/Stock_CAnimData.h>
#include <Engine/Templates/Stock_CTextureData.h>
#include <Engine/Templates/Stock_CModelData.h>
#include <Engine/Templates/Stock_CSoundData.h>
#include <Engine/Templates/Stock_CModelInstance.h>

#include <Engine/Network/EntityHashing.h>
#include <Engine/World/WorldEntityHashing.h>

#include <Engine/Interface/UIManager.h>
#include <Engine/Entities/MobData.h>
#include <Engine/Entities/TargetInfo.h>
#include <Engine/GlobalDefinition.h>
#include <Engine/Contents/function/TargetInfoNewUI.h>
#include <Engine/Info/MyInfo.h>
#include <Engine/Object/ActorMgr.h>
//안태훈 수정 시작	//(For Performance)(0.2)
#include <vector>
//안태훈 수정 끝	//(For Performance)(0.2)

// a reference to a void event for use as default parameter
const EVoid _evVoid;
const CEntityEvent &_eeVoid = _evVoid;

// allocation step for state stack of a CRationalEntity
#define STATESTACK_ALLOCATIONSTEP 5
extern INDEX _ctEntities;
extern INDEX net_bReportServerTraffic;
extern INDEX net_bReportClientTraffic;
extern FLOAT ser_fPositionTreshold;
extern FLOAT ser_fOrientationTreshold;

extern CClientInterface cm_ciLocalClient;


// this is for sending ska animation and sound events over the network
UBYTE _abSkaData[ESKA_MAX_DATA_SIZE];
UBYTE _abSoundData[ESOUND_MAX_DATA_SIZE];


// all standard smart pointer functions are here
CEntity* CEntityID::GetEntity() 
{
	if (eid_ulEntityID == -1) {
		return NULL;
	} else {
		return _pNetwork->ga_World.EntityFromID(eid_ulEntityID);
	}
};
BOOL CEntityID::IsValid()
{
	if (eid_ulEntityID == -1) return TRUE;
	CEntity* penEntity;
	return _pNetwork->ga_World.EntityExists(eid_ulEntityID,penEntity);
};
CEntityID::CEntityID(void) : eid_ulEntityID(-1) {};
CEntityID::~CEntityID(void) { 
	if (eid_ulEntityID != -1) {
		GetEntity()->RemReference(); 
	}
};
CEntityID::CEntityID(const CEntityID &eidOther) : eid_ulEntityID(eidOther.eid_ulEntityID) {
	if (eid_ulEntityID != -1) {
		GetEntity()->AddReference(); 
	}
};
CEntityID::CEntityID(CEntity *pen) {
	if (pen == NULL) {
		eid_ulEntityID = -1;
		return;
	}
	eid_ulEntityID = pen->en_ulID;
	GetEntity()->AddReference(); 
};
const CEntityID &CEntityID::operator=(CEntity *pen) {
	if (pen == NULL) {
		eid_ulEntityID = -1;
		return *this;
	}
	pen->AddReference();    // must first add, then remove!
	GetEntity()->RemReference();
	eid_ulEntityID = pen->en_ulID;
	return *this;
}
const CEntityID &CEntityID::operator=(CEntityID &eidOther) {
	(eidOther.GetEntity())->AddReference();    // must first add, then remove!
	GetEntity()->RemReference();
	eid_ulEntityID = eidOther.eid_ulEntityID;
	return *this;
}

const CEntityID &CEntityID::operator=(ULONG ulEntityID) {
	if (ulEntityID != -1) {
		_pNetwork->ga_World.EntityFromID(ulEntityID)->AddReference();    // must first add, then remove!
	}
	GetEntity()->RemReference();
	eid_ulEntityID = ulEntityID;
	return *this;
}

CEntity* CEntityID::operator->(void){ return GetEntity(); }
CEntityID::operator CEntity*(void) const { return ((CEntityID *)this)->GetEntity(); }
CEntity& CEntityID::operator*(void){ return *GetEntity(); }


// check if entity is of given class
BOOL IsOfClass(CEntity *pen, const char *pstrClassName)
{
	if( pen==NULL || pstrClassName==NULL) {
		return FALSE;
	}
	return strcmp( pen->GetClass()->ec_pdecDLLClass->dec_strName, pstrClassName) == 0;
}


BOOL IsOfSameClass( CEntity *pen1, CEntity *pen2)
{
	if( pen1==NULL || pen2==NULL) {
		return FALSE;
	}
	return pen1->GetClass()->ec_pdecDLLClass == pen2->GetClass()->ec_pdecDLLClass;
}


// check if entity is of given class or derived from
BOOL IsDerivedFromClass(CEntity *pen, const char *pstrClassName)
{
	if (pen==NULL || pstrClassName==NULL) {
		return FALSE;
	}
	// for all classes in hierarchy of the entity
	for( CDLLEntityClass *pdecDLLClass = pen->GetClass()->ec_pdecDLLClass;
			 pdecDLLClass!=NULL;
			 pdecDLLClass = pdecDLLClass->dec_pdecBase) {
		// if it is the wanted class
		if( strcmp(pdecDLLClass->dec_strName, pstrClassName)==0) {
			// it is derived
			return TRUE;
		}
	}
	// otherwise, it is not derived
	return FALSE;
}


// check if entity is of given class or derived from
BOOL IsDerivedFromClass( CEntity *pen, class CDLLEntityClass *pdec)
{
	if( pen==NULL) {
		return FALSE;
	}
	// for all classes in hierarchy of the entity
	for( CDLLEntityClass *pdecDLLClass = pen->GetClass()->ec_pdecDLLClass;
			 pdecDLLClass!=NULL;
			 pdecDLLClass = pdecDLLClass->dec_pdecBase) {
		// if it is the wanted class
		if( pdecDLLClass==pdec) {
			// it is derived
			return TRUE;
		}
	}
	// otherwise, it is not derived
	return FALSE;
}


/////////////////////////////////////////////////////////////////////
// CEntity

/*
 * Default constructor.
 */
CEntity::CEntity(void)
{
	// EDIT : BS : BEGIN
	m_tickSendPDMove = 0;
	// EDIT : BS : END

	en_pbrBrush = NULL;
	en_psiShadingInfo = NULL;
	en_pciCollisionInfo = NULL;
	en_pecClass = NULL;
	en_ulFlags = 0;
	en_ulExtraFlags1	= 0;
	en_ulExtraFlags2	= 0;
	en_lNetworkID		= -1;
	en_sbNetworkType	= -1;
	en_ulSpawnFlags = 0xFFFFFFFFL;    // active always
	en_ulPhysicsFlags = 0;
	en_ulCollisionFlags = 0;
	en_ctReferences = 0;
	en_ulID = 0;
	en_RenderType = RT_NONE;
	en_EntityUseType = EU_NORMAL;
	en_fSpatialClassificationRadius = -1.0f;
	en_penParent = NULL;
	en_plpLastPositions = NULL;
	_ctEntities++;
	en_tmLastPlacementTime = -1.0f;

	en_pMobTarget		= NULL;
	en_pCharacterTarget	= NULL;
	en_pPetTarget		= NULL;
	en_pWildPetTarget	= NULL;
	
//안태훈 수정 시작	//(Add & Modify SSSE Effect)(0.1)
	m_plsLight = NULL;
	en_vDesiredDirection = FLOAT3D(0,0,0);

	en_FallingValue = 0.0125f;
/*
	CEntityTag *pTag = new CEntityTag;
	pTag->SetName("ET POS");
	pTag->SetEntity(this);
	pTag->AddNewTagInfo(0, FLOATquat3D(1,0,0,0), FLOAT3D(0,0,0), FLOAT3D(1,1,1));
	m_tmTagManager.Register(pTag);
*/
	//m_ptmTagManager = new CTagManager;
//안태훈 수정 끝	//(Add & Modify SSSE Effect)(0.1)

	SetFlagOn(ENF_PROPSCHANGED);
}

void CEntity::DeleteSelf(void)
{
	if (en_EntityUseType == EU_NORMAL)
	{
		if (!(en_ulFlags&ENF_DELETED)) {
			ASSERT(FALSE);
			CPrintF("!!!!!ERROR: Deleting undestroyed entity!\n");
			Destroy(FALSE);
		}
		delete this;
	}
}

/*
 * Destructor.
 */
CEntity::~CEntity(void)
{
//안태훈 수정 시작	//(Open beta)(2004-12-29)
	//delete m_ptmTagManager;
//안태훈 수정 끝	//(Open beta)(2004-12-29)
	if (en_EntityUseType == EU_DUMMY)
	{
		return;
	}

	ASSERT(en_ctReferences==0);
	ASSERT(en_ulID!=0);
	ASSERT(en_RenderType==RT_NONE);
	// remove it from container in its world
	ASSERT(!en_pwoWorld->wo_cenEntities.IsMember(this));
	if(en_pwoWorld == NULL) return;
	en_pwoWorld->wo_cenAllEntities.Remove(this);

	// if this is a movable entity, and this is the server
	if (en_ulPhysicsFlags&EPF_MOVABLE) {
		// leave the server hash table
		en_pwoWorld->wo_ehEntityHashContainer.RemoveEntity(this);
	}

	// leave the world hash table
	en_pwoWorld->wo_whWorldEntityContainer.RemoveEntity(this);

	// unset spatial clasification
	en_rdSectors.Clear();

	/*
	Models are always destructed on End(), but brushes and terrains are not, so
	if the pointer is not NULL, then it must be a brush or terrain.
	Both of them are derived from CBrushBase so GetBrushType() will return its real type 
	*/

	// if it is brush of terrain
	if(en_pbrBrush != NULL) {
		INDEX btType = en_pbrBrush->GetBrushType();
		// if this is brush3d
		if(btType==CBrushBase::BT_BRUSH3D) {
			// free the brush
			en_pwoWorld->wo_baBrushes.ba_abrBrushes.Delete(en_pbrBrush);
			en_pbrBrush = NULL;
		// if this is terrain
		} else if(btType==CBrushBase::BT_TERRAIN) {
			// free the brush
			en_pwoWorld->wo_taTerrains.ta_atrTerrains.Delete(en_ptrTerrain);
			en_pbrBrush = NULL;
		// unknown type
		} else {
			ASSERTALWAYS("Unsupported brush type");
		}
	}
	// clear entity type
	en_RenderType = RT_NONE;
	en_pecClass->RemReference();
	en_pecClass = NULL;

	en_fSpatialClassificationRadius = -1.0f;
	_ctEntities--;

}

/////////////////////////////////////////////////////////////////////
// Access functions
													 
/* Test if the entity is an empty brush. */
BOOL CEntity::IsEmptyBrush(void) const
{
	// if it is not brush
	if (en_RenderType != CEntity::RT_BRUSH && en_RenderType != RT_FIELDBRUSH) {
		// it is not empty brush
		return FALSE;
	// if it is brush
	} else {

		// get its brush
		CBrush3D &brBrush = *en_pbrBrush;
		// get the first mip of the brush
		CBrushMip *pbmMip = brBrush.GetFirstMip();

		// it is empty if it has zero sectors
		return pbmMip->bm_abscSectors.Count()==0;
	}
}

/* Return max Game Players */
INDEX CEntity::GetMaxPlayers(void) {
	return NET_MAXGAMEPLAYERS;
};

/* Return Player Entity */
CEntity *CEntity::GetPlayerEntity(INDEX iPlayer)
{
//강동민 수정 시작 싱글 던젼 작업	07.29
	//ASSERT(iPlayer>=0 && iPlayer<GetMaxPlayers());
//강동민 수정 끝 싱글 던젼 작업		07.29
	// if the player target is inactive, return NULL
	if (!_pNetwork->ga_srvServer.srv_apltPlayers[iPlayer].IsActive()) {
		return NULL;
	}
	// else return player entity
	return _pNetwork->ga_srvServer.srv_apltPlayers[iPlayer].plt_penPlayerEntity;
}

/* Get bounding box of this entity - for AI purposes only. */
void CEntity::GetBoundingBox(FLOATaabbox3D &box)
{
	if (en_pciCollisionInfo!=NULL) {
		box = en_pciCollisionInfo->ci_boxCurrent;
	} else {
		GetSize(box);
		box += GetPlacement().pl_PositionVector;
	}
}

/* Get size of this entity - for UI purposes only. */
void CEntity::GetSize(FLOATaabbox3D &box)
{
	if (en_RenderType==CEntity::RT_MODEL || en_RenderType==CEntity::RT_EDITORMODEL) {
		en_pmoModelObject->GetCurrentFrameBBox( box);
		box.StretchByVector(en_pmoModelObject->mo_Stretch);
	} else if(en_RenderType==CEntity::RT_SKAMODEL || en_RenderType==CEntity::RT_SKAEDITORMODEL) {
		GetModelInstance()->GetCurrentColisionBox( box);
		box.StretchByVector(GetModelInstance()->mi_vStretch);
	} else if (en_RenderType==CEntity::RT_TERRAIN) {
		const CTerrain *ptrTerrain = GetTerrain();
		box = TR_GetStretchedTerrainBBox(ptrTerrain);
	} else if (en_RenderType==CEntity::RT_BRUSH || en_RenderType==CEntity::RT_FIELDBRUSH) {
		CBrushMip *pbm = en_pbrBrush->GetFirstMip();
		if (pbm == NULL) {
			box = FLOATaabbox3D(FLOAT3D(0,0,0), FLOAT3D(0,0,0));
		} else {
			box = pbm->bm_boxBoundingBox;
			box += -GetPlacement().pl_PositionVector;
		}
	}
	else {
		box = FLOATaabbox3D(FLOAT3D(0,0,0), FLOAT3D(0,0,0));
	}
}

//0724 kwon 함수 추가.
//------------------------------------------------------------------------------
// CEntity::SetTargetInfo
// Explain:  
//------------------------------------------------------------------------------
bool CEntity::SetTargetInfo(float fMaxHealth, float fHealth, BOOL bMe, int level, int Pk,int pkstate, int legit,int dbIdx)
{
	CUIManager* pUIMgr = CUIManager::getSingleton();
	ObjInfo* pInfo = ObjInfo::getSingleton();
	CTargetInfoUI* pTargetUI = pUIMgr->GetTargetInfoUI();

	if (pTargetUI == NULL)
		return false;

	if( pInfo->GetTargetEntity(eTARGET) == this )
		return false;

	//전사의 축복일 경우 타겟 정보 저장하지 않음...
	if( GetName() == CMobData::getData(310)->GetName() ||
		GetName() == CMobData::getData(311)->GetName() ||
		GetName() == CMobData::getData(312)->GetName() ||
		GetName() == CMobData::getData(313)->GetName() )
	{
		pInfo->TargetClear(eTARGET);
		return false;
	}
	
	CTargetInfom TargetInfo;
	TargetInfo.fMaxHealth	= fMaxHealth;
	TargetInfo.fHealth		= fHealth;
	TargetInfo.bIsActive	= TRUE;
	TargetInfo.iLevel		= level;
	TargetInfo.pen_pEntity	= this;
	// NPC의 경우만 NPC의 flag 구분의 쓰기 위해
	TargetInfo.dbIdx		= dbIdx;

	if(Pk != -1)
	{
		TargetInfo.PkMode	= Pk;
		TargetInfo.PkState	= pkstate;
		TargetInfo.Legit	= legit;
	}

	if(bMe)
	{
		strcpy(TargetInfo.TargetName, _pNetwork->MyCharacterInfo.name);
	}
	else
	{
		// Modified by yjpark
		if( IsPet() )
		{
			if( en_pPetTarget )
			{
				if( en_pPetTarget->pet_strNameCard.Length() >0)
					strcpy( TargetInfo.TargetName, en_pPetTarget->pet_strNameCard);
				else
					strcpy( TargetInfo.TargetName, en_pPetTarget->m_strName.c_str() );
			}
		}
		else if( IsSlave() )
		{
			if( en_pSlaveTarget )
				strcpy( TargetInfo.TargetName, en_pSlaveTarget->m_strName.c_str() );
		}
		else if( IsWildPet())
		{
			if (en_pWildPetTarget != NULL)
			{
				strcpy( TargetInfo.TargetName, en_pWildPetTarget->m_strName.c_str() );
			}
		}
		else
			strcpy( TargetInfo.TargetName, GetName() );
		// Modified by yjpark
	}
	BOOL bShowBuff = FALSE;

	if(IsEnemy())
	{		
		if(this->IsFirstExtraFlagOn(ENF_EX1_NPC))
		{
			TargetInfo.TargetType = NPC;
		}
		else if(this->IsFirstExtraFlagOn(ENF_EX1_PRODUCTION))
		{
			TargetInfo.TargetType = PRODUCTION;
		}
		else
		{
			TargetInfo.TargetType = MOB;
		}
	}
	else if(IsPet())
	{
		TargetInfo.TargetType = P1PET;
	}
	else if(IsSlave())
	{
		TargetInfo.TargetType = SUMMON;
	}
	else if(IsCharacter() || bMe)
	{
		TargetInfo.TargetType = CHARACTER;
		bShowBuff = bMe;
	}
	else if(IsWildPet())
	{
		TargetInfo.TargetType = WILDPET;
	}
	
	int nNetworkID = -1;

	// 타겟대상이 캐릭터 일때만 보내준다.
	if (IsCharacter())
		nNetworkID = GetNetworkID();
		
	_pNetwork->SendClickObject(nNetworkID);
	pInfo->SetTargetInfo(TargetInfo);
	pTargetUI->SetTargetBuff( bShowBuff );
	return true;
}

void CEntity::SetTargetSyndicateInfo( int nType, int nGrade )
{
	INFO()->SetTargetSyndicate(nType, nGrade);
}

//------------------------------------------------------------------------------
// CEntity::UpdateTargetInfo
// Explain:  
//------------------------------------------------------------------------------
void CEntity::UpdateTargetInfo( float fMaxHealth, float fHealth, int Pk,int pkstate,int Legit)
{
	ObjInfo* pInfo = ObjInfo::getSingleton();
	
	pInfo->SetTargetHealth(fHealth, fMaxHealth);

	if(Pk != -1)
	{
		pInfo->SetTargetPKMode(Pk, pkstate, Legit);
	}
}

//------------------------------------------------------------------------------
// CEntity::DelTargetInfo
// Explain:  
//------------------------------------------------------------------------------
void CEntity::DelTargetInfo()
{
	INFO()->TargetClear(eTARGET);
}
//0824

//------------------------------------------------------------------------------
// CEntity::SetTargetInfoReal
// Explain: 
//------------------------------------------------------------------------------
// FIXME : 마우스 오버만 되면 호출되는 듯한테,
// FIXME : 타 캐릭터와 아이템만 정보 출력해주면 되는거 아닌가???
void CEntity::SetTargetInfoReal( float fMaxHealth, float fHealth, int level,int Pk,int pkstate,SQUAD llCount,int pkLegit,CCharacterTarget* ct, int nType )
{
	ObjInfo* pInfo = ObjInfo::getSingleton();

	// Modified by yjpark
	if( pInfo->GetTargetEntity(eTARGET_REAL) == this &&
		( IsPet() || IsSlave() || IsEnemy() || ( GetFlags() & ENF_ITEM ) || Pk == pInfo->GetTargetPKMode(eTARGET_REAL) ) )
		return;

	CTargetInfomReal targetInfo;
	targetInfo.fMaxHealth	= fMaxHealth;
	targetInfo.fHealth		= fHealth;
	targetInfo.bIsActive	= TRUE;
	targetInfo.iLevel		= level;
	targetInfo.pen_pEntity	= this;

	if(Pk != -1)
	{
		targetInfo.PkMode= Pk;
		targetInfo.PkState = pkstate;
		targetInfo.Legit = pkLegit;
	}

	if(GetFlags() & ENF_ITEM)
	{
		strcpy(targetInfo.TargetName,en_strItemName);
		targetInfo.llCount = llCount;
	}
	else if(IsPet())
	{
		if( en_pPetTarget )
		{
			if( en_pPetTarget->pet_strNameCard.Length() >0 )
				strcpy(targetInfo.TargetName, en_pPetTarget->pet_strNameCard);
			else
				// Modified by yjpark
				strcpy(targetInfo.TargetName, en_pPetTarget->m_strName.c_str());
				// Modified by yjpark
		}
	}
	else if(IsSlave())
	{
		if( en_pSlaveTarget )
			strcpy(targetInfo.TargetName, en_pSlaveTarget->m_strName.c_str());
	}
	else if( IsWildPet())
		{
			if (en_pWildPetTarget != NULL)
			{
				strcpy( targetInfo.TargetName, en_pWildPetTarget->m_strName.c_str() );
			}
		}
	else
	{
		ObjectBase* pObj = ACTORMGR()->GetObject(eOBJ_MOB, en_lNetworkID);

		if (pObj != NULL)
		{
			CMobData* pMD = CMobData::getData(pObj->m_nType);

			if (pMD != NULL && pMD->IsTotemItem())
				strcpy(targetInfo.TargetName, pObj->m_strName.c_str());
			else
				strcpy(targetInfo.TargetName,GetName());
		}
		else
		{
			strcpy(targetInfo.TargetName,GetName());
		}

		if( ct != NULL)
		{
			targetInfo.lGuildIndex	= ct->cha_lGuildIndex;
			targetInfo.strGuildName = ct->cha_strGuildName;
			// WSS_GUILDMASTER 070517
			// 캐릭터 클릭시 색깔지정 위해
			targetInfo.sbGuildRank = ct->cha_sbGuildRank;
			targetInfo.ubGuildNameColor = ct->cha_ubGuildNameColor;
			targetInfo.iNickIndex = ct->cha_NickType;
		}
	}

	if(IsEnemy())
	{
		if(this->IsFirstExtraFlagOn(ENF_EX1_NPC))
		{
			targetInfo.TargetType = NPC;
		}
		else if(this->IsFirstExtraFlagOn(ENF_EX1_PRODUCTION))
		{
			targetInfo.TargetType = PRODUCTION;
		}
		else
		{
			targetInfo.TargetType = MOB;
		}
	}
	else if(IsPet())
	{
		targetInfo.TargetType = P1PET;
	}
	else if(IsCharacter())
	{
		targetInfo.TargetType = CHARACTER;
	}
	else if(IsSlave())
	{
		targetInfo.TargetType = SUMMON;
	}
	else if(IsWildPet())
	{
		targetInfo.TargetType = WILDPET;
	}
	else if(GetFlags() & ENF_ITEM)
	{
		targetInfo.TargetType	= ITEM;
		targetInfo.dbIdx		= nType;		
	}

	pInfo->SetTargetRealInfo(targetInfo);
}

/* Get name of this entity. */
const CTString &CEntity::GetName(void) const
{
	static const CTString strDummyName("");
	return strDummyName;
}
const CTString &CEntity::GetDescription(void) const // name + some more verbose data
{
	static const CTString strDummyDescription("");
	return strDummyDescription;
}

/* Get first target of this entity. */
CEntity *CEntity::GetTarget(void) const
{
	return NULL;
}

/* Check if entity can be used as a target. */
BOOL CEntity::IsTargetable(void) const
{
	// cannot be targeted unless this function is overridden
	return FALSE;
}
/* Check if entity is server only (not updated through network) */
BOOL CEntity::IsSentOverNet(void) const 
{ 
	// it is allways updated, unless this function is overriden
	return TRUE; 
}
/* Check if entity is marker */
BOOL CEntity::IsMarker(void) const{
	// cannot be marker unless this function is overridden
	return FALSE;
}
//강동민 수정 시작 다중 공격 작업	08.27
BOOL CEntity::IsEnemy(void) const
{
	return FALSE;
}

BOOL CEntity::IsEnemySpawner(void) const
{
	return FALSE;
}

BOOL CEntity::IsPet(void) const
{
	return FALSE;
}

BOOL CEntity::IsSlave(void) const
{
	return FALSE;
}

BOOL CEntity::IsWildPet(void) const
{
	return FALSE;
}

SBYTE CEntity::GetNetworkType()	const
{
	return en_sbNetworkType;
}

BOOL	CEntity::IsAvailableRide()
{
	return FALSE;
}

//강동민 수정 끝 다중 공격 작업		08.27
/* Check if entity is important */
BOOL CEntity::IsImportant(void) const{
	// cannot be important unless this function is overridden
	return FALSE;
}
/* Check if entity is moved on a route set up by its targets. */
BOOL CEntity::MovesByTargetedRoute(CTString &strTargetProperty) const
{
	return FALSE;
}
/* Check if entity can drop marker for making linked route. */
BOOL CEntity::DropsMarker(CTFileName &fnmMarkerClass, CTString &strTargetProperty) const
{
	return FALSE;
}

/* Get light source information - return NULL if not a light source. */
//안태훈 수정 시작	//(For Performance)(0.2)
/*
CLightSource *CEntity::GetLightSource(void)
{
//안태훈 수정 시작	//(Add & Modify SSSE Effect)(0.1)
	return m_plsLight;
//안태훈 수정 끝	//(Add & Modify SSSE Effect)(0.1)
}
*/
//안태훈 수정 끝	//(For Performance)(0.2)
BOOL CEntity::GetRaidObject(void) const
{
	return FALSE;
}

INDEX CEntity::GetRaidEvent(void) const
{
	return INDEX(0);
}

BOOL CEntity::GetAlBackground(void) const
{
	return FALSE;
}

BOOL CEntity::IsTargetValid(SLONG slPropertyOffset, CEntity *penTarget)
{
	return TRUE;
}

/* Get anim data for given animation property - return NULL for none. */
CAnimData *CEntity::GetAnimData(SLONG slPropertyOffset)
{
	return NULL;
}

/* Get force type name, return empty string if not used. */
const CTString &CEntity::GetForceName(INDEX iForce)
{
	static const CTString strDummyName("");
	return strDummyName;
}

/* Get forces in given point. */
void CEntity::GetForce(INDEX iForce, const FLOAT3D &vPoint,
	CForceStrength &fsGravity, CForceStrength &fsField)
{
	// default gravity
	fsGravity.fs_vDirection = FLOAT3D(0,-1,0);
	fsGravity.fs_fAcceleration = 9.81f;
	fsGravity.fs_fVelocity = 0;

	// no force field
	fsField.fs_fAcceleration = 0;
}
/* Get entity that controls the force, used for change notification checking. */
CEntity *CEntity::GetForceController(INDEX iForce)
{
	return NULL;
}

/* Adjust model shading parameters if needed - return TRUE if needs model shadows. */
BOOL CEntity::AdjustShadingParameters(FLOAT3D &vLightDirection,
	COLOR &colLight, COLOR &colAmbient)
{
	return TRUE;
}
/* Adjust model mip factor if needed. */
void CEntity::AdjustMipFactor(FLOAT &fMipFactor)
{
	(void)fMipFactor;
	NOTHING;
}

// get a different model object for rendering - so entity can change its appearance dynamically
// NOTE: base model is always used for other things (physics, etc).
CModelObject *CEntity::GetModelForRendering(void)
{
	return en_pmoModelObject;
}

// get a different model instance for rendering - so entity can change its appearance dynamically
// NOTE: base model is always used for other things (physics, etc).
CModelInstance *CEntity::GetModelInstanceForRendering(void)
{
	return en_pmiModelInstance;
}

/* Get fog type name, return empty string if not used. */
const CTString &CEntity::GetFogName(INDEX iFog)
{
	static const CTString strDummyName("");
	return strDummyName;
}

/* Get fog, return FALSE for none. */
BOOL CEntity::GetFog(INDEX iFog, class CFogParameters &fpFog)
{
	return FALSE;
}

/* Get haze type name, return empty string if not used. */
const CTString &CEntity::GetHazeName(INDEX iHaze)
{
	static const CTString strDummyName("");
	return strDummyName;
}

/* Get haze, return FALSE for none. */
BOOL CEntity::GetHaze(INDEX iHaze, class CHazeParameters &hpHaze, FLOAT3D &vViewDir)
{
	return FALSE;
}

/* Get mirror type name, return empty string if not used. */
const CTString &CEntity::GetMirrorName(INDEX iMirror)
{
	static const CTString strDummyName("");
	return strDummyName;
}

/* Get mirror, return FALSE for none. */
BOOL CEntity::GetMirror(INDEX iMirror, class CMirrorParameters &mpMirror)
{
	return FALSE;
}

/* Get gradient type name, return empty string if not used. */
const CTString &CEntity::GetGradientName(INDEX iGradient)
{
	static const CTString strDummyName("");
	return strDummyName;
}

/* Get gradient, return FALSE for none. */
BOOL CEntity::GetGradient(INDEX iGradient, class CGradientParameters &gpGradient)
{
	return FALSE;
}

FLOAT3D CEntity::GetClassificationBoxStretch(void)
{
	return FLOAT3D( 1.0f, 1.0f, 1.0f);
}

/* Get field information - return NULL if not a field. */
CFieldSettings *CEntity::GetFieldSettings(void)
{
	return NULL;
}

/* Render particles made by this entity. */
void CEntity::RenderParticles(void)
{
	// signal no particles for renderer
	SetFlagOn(ENF_NOPARTICLES);
}

/* Get current collision box index for this entity. */
INDEX CEntity::GetCollisionBoxIndex(void)
{
	// by default, use only box 0
	return 0;
}
/* Get current collision box - override for custom collision boxes. */
void CEntity::GetCollisionBoxParameters(INDEX iBox, FLOATaabbox3D &box, INDEX &iEquality)
{
	// if this is ska model
	if(en_RenderType==RT_SKAMODEL || en_RenderType==RT_SKAEDITORMODEL) {
		box.minvect = GetModelInstance()->GetCollisionBoxMin(iBox);
		box.maxvect = GetModelInstance()->GetCollisionBoxMax(iBox);
		FLOATaabbox3D boxNS = box;
		box.StretchByVector(GetModelInstance()->mi_vStretch);
		iEquality = GetModelInstance()->GetCollisionBoxDimensionEquality(iBox);
	} else {
		box.minvect = en_pmoModelObject->GetCollisionBoxMin(iBox);
		box.maxvect = en_pmoModelObject->GetCollisionBoxMax(iBox);
		box.StretchByVector(en_pmoModelObject->mo_Stretch);
		iEquality = en_pmoModelObject->GetCollisionBoxDimensionEquality(iBox);
	}
}

/* Render game view */
void CEntity::RenderGameView(CDrawPort *pdp, void *pvUserData)
{
	NOTHING;
}
//0609 kwon 
/* Render game view */
void CEntity::RenderCursor(CDrawPort *pdp, void *pvUserData)
{
	NOTHING;
}
// apply mirror and stretch to the entity if supported
void CEntity::MirrorAndStretch(FLOAT fStretch, BOOL bMirrorX)
{
	NOTHING;
}
// get offset for depth-sorting of alpha models (in meters, positive is nearer)
FLOAT CEntity::GetDepthSortOffset(void)
{
	return 0.0f;
}
// get visibility tweaking bits
ULONG CEntity::GetVisTweaks(void)
{
	return 0;
}

// Get max tessellation level
FLOAT CEntity::GetMaxTessellationLevel(void)
{
	return 0.0f;
}


/* Called after creating and setting its properties. */
void CEntity::OnInitialize(const CEntityEvent &eeInput)
{


	// try to find a handler in start state
	pEventHandler pehHandler = HandlerForStateAndEvent(1, eeInput.ee_slEvent);
	// if there is a handler
	if (pehHandler!=NULL) {
		// call the function
		(this->*pehHandler)(eeInput);
	// if there is no handler
	} else {
		ASSERTALWAYS("All entities must have Main procedure!");
	}
}
/* Called before releasing entity. */
void CEntity::OnEnd(void)
{
}

// print stack to debug output
const char *CEntity::PrintStackDebug(void) 
{
	return "not CRationalEntity";
};

/*
 * Prepare entity (call after setting properties).
 */
void CEntity::Initialize(const CEntityEvent &eeInput,BOOL bNetwork)
{

	// make sure we are not deleted during intialization
	CEntityPointer penThis = this;

	// insert the entity initialize mesage into the server buffer
	if (_pNetwork->IsServer() && bNetwork) {// && IsSentOverNet()) {
		SLONG slSize = ((CEntityEvent&) eeInput).GetSizeOf() - sizeof(CEntityEvent);
		SLONG slEventCode = eeInput.ee_slEvent; 
		UWORD uwClassID,uwEventID;

		ASSERT (slSize>=0);

		uwEventID = slEventCode & 0x0000FFFF;
		uwClassID = (slEventCode & 0xFFFF0000) >> 16;

		if (net_bReportServerTraffic) {
			CPrintF("Initialized entity: ID: 0x%X, EntityClass: %d, EventID: %d, Size: %d\n",en_ulID,uwClassID,uwEventID,slSize);
		}
		extern CEntityMessage _emEntityMessage;
		_emEntityMessage.WriteEntityInitialize(en_ulID,(SLONG&)eeInput.ee_slEvent,((UBYTE*)&eeInput) + sizeof(CEntityEvent),slSize);
		_pNetwork->ga_srvServer.SendMessage(_emEntityMessage);
	}

	Initialize_internal(eeInput);
	// set spatial clasification
	FindSectorsAroundEntity();
	// precache all other things
	Precache();
	
//안태훈 수정 시작	//(For Performance)(0.2)
	if(GetLightSource()) 
	{
		//[080109: Su-won] WORLDEDITOR_BUG_FIX
		//Light entity의 상태가 변할 때마다 m_vectorLights에 같은 entity의 포인터를 계속 넣음. 
		//그래서 entity가 삭제될 때 그 중에 하나만 삭제되어, vector에서 삭제된 entity 포인터를 가져오면서 문제가 발생.
		//=>m_vectorLights에 존재하지 않는 entity만 넣음.
		std::vector<CEntity *>::iterator iter = std::find(en_pwoWorld->m_vectorLights.begin(), en_pwoWorld->m_vectorLights.end(), this);
		if( iter == this->en_pwoWorld->m_vectorLights.end() )
			this->en_pwoWorld->m_vectorLights.push_back(this); 
	}
//안태훈 수정 끝	//(For Performance)(0.2)
}

void CEntity::InitializeFromNet()
{
	Initialize();
}

void CEntity::Initialize_internal(const CEntityEvent &eeInput)
{
	//0507 kwon 삭제.
	/*
	#ifndef NDEBUG
		// clear settings for debugging
		en_RenderType = RT_ILLEGAL;
	#endif
*/
	// remember brush zoning flag
	BOOL bWasZoning = en_ulFlags&ENF_ZONING;

	// let derived class initialize according to the properties
	OnInitialize(eeInput);
	// derived class must set all properties
    ASSERT(en_RenderType != RT_ILLEGAL);

	// if this is a brush
	if (en_RenderType==RT_BRUSH || en_RenderType==RT_FIELDBRUSH) {
		// test if zoning
		BOOL bZoning = en_ulFlags&ENF_ZONING;
		// if switching from zoning to non-zoning
		if (bWasZoning && !bZoning) {
			// switch from zoning to non-zoning
			en_pbrBrush->SwitchToNonZoning();
			en_rdSectors.Clear();
		// if switching from non-zoning to zoning
		} else if (!bWasZoning && bZoning) {
			// switch from non-zoning to zoning
			en_pbrBrush->SwitchToZoning();
			en_rdSectors.Clear();
		}
	}

	// if it is a field brush
	CFieldSettings *pfs = GetFieldSettings();
	if (pfs!=NULL) {
		// remember its field settings
		ASSERT(en_RenderType == RT_FIELDBRUSH);
		en_pbrBrush->br_pfsFieldSettings = pfs;
	}
}

/*
 * Clean-up entity.
 */
void CEntity::End(void)
{
	/* NOTE: Must not remove from thinker/mover list here, or CServer::ProcessGameTick()
	 * might crash!
	 */
	End_internal();
}
void CEntity::End_internal(void)
{


	// let derived class clean-up after itself
	OnEnd();

	// clear last positions
	if (en_plpLastPositions!=NULL) {
		delete en_plpLastPositions;
		en_plpLastPositions = NULL;
	}

	// clear spatial classification
	en_fSpatialClassificationRadius = -1.0f;
	en_boxSpatialClassification = FLOATaabbox3D();

	// depending on entity type
	switch(en_RenderType) {
	// if it is brush
	case RT_BRUSH:
		break;
	// if it is field brush
	case RT_FIELDBRUSH:
		break;

	// if it is model
	case RT_MODEL:
	case RT_EDITORMODEL:
		// free its model object
		delete en_pmoModelObject;
		DiscardCollisionInfo();
		en_pmoModelObject = NULL;
		break;
	// if it is ska model
	case RT_SKAMODEL:
	case RT_SKAEDITORMODEL:
		DeleteModelInstance(en_pmiModelInstance);
		en_pmiModelInstance = NULL;
		break;
	case RT_TERRAIN:
		en_ptrTerrain->tr_ulTerrainFlags&=~TR_INITIALIZED;
		break;

	// if it is nothing
	case RT_NONE:
	case RT_VOID:
		// do nothing
		NOTHING;
		break;
	// if it is any other type
	default:
		ASSERTALWAYS("Unsupported entity type");
	}

	if(en_psiShadingInfo!=NULL) {
		delete en_psiShadingInfo;
		en_psiShadingInfo = NULL;
	}
	DiscardCollisionInfo();
	en_ulCollisionFlags = 0;

	// clear entity type
	en_RenderType = RT_NONE;
}

/*
 * Reinitialize the entity.
 */
void CEntity::Reinitialize(void)
{

	End_internal();
	Initialize_internal(_eeVoid);
}

// teleport this entity to a new location -- takes care of telefrag damage
void CEntity::Teleport(const CPlacement3D &plNew, BOOL bTelefrag /*=TRUE*/)
{

	ASSERT(en_fSpatialClassificationRadius>0);

	// if telefragging is on and the entity has collision box
	if (bTelefrag && en_pciCollisionInfo!=NULL) {

		// create the box of the entity at its new placement
		FLOATmatrix3D mRot;
		MakeRotationMatrixFast(mRot, plNew.pl_OrientationAngle);
		FLOAT3D vPos = plNew.pl_PositionVector;
		CMovingSphere &ms0 = en_pciCollisionInfo->ci_absSpheres[0];
		CMovingSphere &ms1 = en_pciCollisionInfo->ci_absSpheres[en_pciCollisionInfo->ci_absSpheres.Count()-1];
		FLOATaabbox3D box;
		box  = FLOATaabbox3D(vPos+ms0.ms_vCenter*mRot, ms0.ms_fR);
		box |= FLOATaabbox3D(vPos+ms1.ms_vCenter*mRot, ms1.ms_fR);

		// first inflict huge damage there in the entities box
		InflictBoxDamage(this, DMT_TELEPORT, 100000.0f, box);
	}

	// remember original orientation matrix
	FLOATmatrix3D mOld = en_mRotation;

	// now put the entity there
	SetPlacement(plNew);
	FallDownToFloor(); //0401 kwon
	// movable entity
	if (en_ulPhysicsFlags & EPF_MOVABLE) {
		((CMovableEntity*)this)->ClearTemporaryData();
		((CMovableEntity*)this)->en_plLastPlacement = en_plPlacement;
		// transform speeds
		FLOATmatrix3D mRel = en_mRotation*!mOld;
		((CMovableEntity*)this)->en_vCurrentTranslationAbsolute *= mRel;
		
		// clear reference
		((CMovableEntity*)this)->en_penReference = NULL;
		((CMovableEntity*)this)->en_pbpoStandOn = NULL;

		// notify that it was teleported
		SendEvent(ETeleport());
		((CMovableEntity*)this)->AddToMovers();

		extern INDEX ent_bReportSpawnInWall;
		if (ent_bReportSpawnInWall) {
			// if movable model
			if (en_RenderType==RT_MODEL || en_RenderType==RT_EDITORMODEL || 
				en_RenderType==RT_SKAMODEL || en_RenderType==RT_SKAEDITORMODEL) {
				// check if it was teleported inside a wall
				CMovableModelEntity *pmme = (CMovableModelEntity*)this;
				CEntity *ppenObstacleDummy;
				if (pmme->CheckForCollisionNow(pmme->en_iCollisionBox, &ppenObstacleDummy)) {
					CPrintF("Entity '%s' was teleported inside a wall at (%g,%g,%g)!\n", 
						GetName(), 
						en_plPlacement.pl_PositionVector(1),
						en_plPlacement.pl_PositionVector(2),
						en_plPlacement.pl_PositionVector(3));
				}
			}
		}
	}
/*	
	//0322 kwon
	if(en_plPlacement.pl_PositionVector(1)!=32000.0f)//기본 텔레포트가 아니라면,
	{	
	CNetworkMessage nmCharAdd(MSG_PLAYER_ADD); 		
	nmCharAdd<< en_plPlacement.pl_PositionVector(1)<<en_plPlacement.pl_PositionVector(2)<<en_plPlacement.pl_PositionVector(3);		
	nmCharAdd<< en_plPlacement.pl_OrientationAngle(1)<<en_plPlacement.pl_OrientationAngle(2)<<en_plPlacement.pl_OrientationAngle(3);		

	nmCharAdd<<en_pcCharacter.pc_iPlayerType;// (ULONG)1;
	
	_pNetwork->SendToServerNew(nmCharAdd);	
	}
	*/
}

/*
 * Set placement of this entity. 
 */
void CEntity::SetPlacement(const CPlacement3D &plNew)
{
	// check if orientation is changed
	BOOL bSameOrientation = (plNew.pl_OrientationAngle==en_plPlacement.pl_OrientationAngle);

	// kwon	
	extern BOOL _bWorldEditorApp;
	if(!_bWorldEditorApp)
	{		
		bSameOrientation = TRUE;
	}

	// if the orientation has not changed
	if (bSameOrientation) {
		// set the placement and do all needed recalculation
		SetPlacement_special(plNew, en_mRotation, 0 /* doesn't have to be near. */);

	// if the orientation has changed
	} else {
		// calculate new rotation matrix
		FLOATmatrix3D mRotation;
		MakeRotationMatrixFast(mRotation, plNew.pl_OrientationAngle);
		// set the placement and do all needed recalculation
		SetPlacement_special(plNew, mRotation, 0 /* doesn't have to be near. */);
	}
	
	// if this entity has parent
	if (en_penParent!=NULL) {
		// adjust relative placement
		en_plRelativeToParent = en_plPlacement;
		en_plRelativeToParent.AbsoluteToRelativeSmooth(en_penParent->en_plPlacement);
	}
}


/*
 * Fall down to floor. (use only in WEd!)
 */
void CEntity::FallDownToFloor( void)
{
	CEntity::RenderType rt = GetRenderType();
	// is this old model
	if(rt==CEntity::RT_MODEL || rt==CEntity::RT_EDITORMODEL) 
	{
		ASSERT(en_pmoModelObject != NULL);
	// is this ska model
	} 
	else if(rt==CEntity::RT_SKAMODEL || rt==CEntity::RT_SKAEDITORMODEL) 
	{
		ASSERT(GetModelInstance() != NULL);
	} 
	else 
	{
		return;
	}
	// if( rt!=CEntity::RT_MODEL && rt!=CEntity::RT_EDITORMODEL) return;
	// ASSERT(en_pmoModelObject != NULL);

	CPlacement3D plPlacement = GetPlacement();
	FLOAT3D vRay[4];
	// if it is movable entity
	if( en_ulPhysicsFlags & EPF_MOVABLE && en_sbNetworkType > 0/*MSG_CHAR_PC*/) 
	{
		INDEX iEq;
		FLOATaabbox3D box;
		GetCollisionBoxParameters(GetCollisionBoxIndex(), box, iEq);
		FLOAT3D vMin = box.Min();
		FLOAT3D vMax = box.Max();
		// all ray casts start from same height
		vRay[0](2) = vMax(2);
		vRay[1](2) = vMax(2);
		vRay[2](2) = vMax(2);
		vRay[3](2) = vMax(2);

		vRay[0](1) = vMin(1);
		vRay[0](3) = vMin(3);
		vRay[1](1) = vMin(1);
		vRay[1](3) = vMax(3);
		vRay[2](1) = vMax(1);
		vRay[2](3) = vMin(3);
		vRay[3](1) = vMax(1);
		vRay[3](3) = vMax(3);
	}
	else
	{
		FLOATaabbox3D box;
		if(rt==CEntity::RT_SKAMODEL || rt==CEntity::RT_SKAEDITORMODEL) {
			GetModelInstance()->GetCurrentColisionBox( box);
		} else {
			en_pmoModelObject->GetCurrentFrameBBox( box);
		}

		FLOAT3D vCenterUp = box.Center();
		vCenterUp(2) = box.Max()(2);
		vRay[0] = vCenterUp;
		vRay[1] = vCenterUp;
		vRay[2] = vCenterUp;
		vRay[3] = vCenterUp;
	}

	FLOAT fMaxY = -9999999.0f;
	BOOL bFloorHitted = FALSE;
	for( INDEX iRay=0; iRay<4; iRay++)
	{
		FLOAT3D vSource = plPlacement.pl_PositionVector+vRay[iRay];
		FLOAT3D vTarget = vSource;
		vTarget(2) -= 1000.0f;
		CCastRay crRay( this, vSource, vTarget);
		crRay.cr_ttHitModels = CCastRay::TT_NONE; // CCastRay::TT_FULLSEETHROUGH;
		crRay.cr_bHitTranslucentPortals = TRUE;
		crRay.cr_bPhysical = TRUE;
		GetWorld()->CastRay(crRay);
		if( (crRay.cr_penHit != NULL) && (crRay.cr_vHit(2) > fMaxY)) 
		{
			fMaxY = crRay.cr_vHit(2);
			bFloorHitted = TRUE;
		}
	}
	if( bFloorHitted)
	{
		if (IsPlayer() && (g_slZone == 26 || g_slZone == 27 || g_slZone == 28)) // 동굴 던전에서만
		{
			plPlacement.pl_PositionVector(2) += fMaxY-plPlacement.pl_PositionVector(2)+en_FallingValue;//.01f;
		}
		else
		{
			//plPlacement.pl_PositionVector(2) += fMaxY-plPlacement.pl_PositionVector(2)+0.01f;
			plPlacement.pl_PositionVector(2) += fMaxY-plPlacement.pl_PositionVector(2)+0.1f;
		}
	}
	SetPlacement( plPlacement);
}


extern CEntity *_penLightUpdating;
extern BOOL _bDontDiscardLinks = FALSE;

// internal repositioning function
void CEntity::SetPlacement_special(const CPlacement3D &plNew, const FLOATmatrix3D &mRotation,
	 ULONG ulFlags)
{
	FLOAT fPositionDelta,fOrientationDelta;
	fPositionDelta =   fabs(plNew.pl_PositionVector(1) - en_plPlacement.pl_PositionVector(1))
									 + fabs(plNew.pl_PositionVector(2) - en_plPlacement.pl_PositionVector(2)) 
									 + fabs(plNew.pl_PositionVector(3) - en_plPlacement.pl_PositionVector(3));
	fOrientationDelta =   fabs(plNew.pl_OrientationAngle(1) - en_plPlacement.pl_OrientationAngle(1))
											+ fabs(plNew.pl_OrientationAngle(2) - en_plPlacement.pl_OrientationAngle(2))
											+ fabs(plNew.pl_OrientationAngle(3) - en_plPlacement.pl_OrientationAngle(3));

	BOOL bSendNow = (fPositionDelta >= ser_fPositionTreshold) || (fOrientationDelta > ser_fOrientationTreshold);
	if (bSendNow && _pNetwork->IsServer() && IsSentOverNet() && !IsPlayer()) {
		extern CEntityMessage _emEntityMessage;
		_emEntityMessage.WritePlacementNotify(en_ulID);
		_pNetwork->ga_srvServer.SendMessage(_emEntityMessage);
	}
	if (bSendNow) {
		SetFlagOn(ENF_PLACCHANGED);
	}

	// invalidate eventual cached info for still models
	en_ulFlags &= ~ENF_VALIDSHADINGINFO;

	// remembel old placement of the entity
	CPlacement3D plOld = en_plPlacement;
	
	// set new placement of the entity
	en_plPlacement = plNew;
	en_mRotation = mRotation;

	// if this is a brush entity
	if (en_RenderType==RT_BRUSH || en_RenderType==RT_FIELDBRUSH) {
		// recalculate all bounding boxes relative to new position
		_bDontDiscardLinks = TRUE;
		en_pbrBrush->CalculateBoundingBoxes();
		_bDontDiscardLinks = FALSE;

		BOOL bHasShadows=FALSE;
		// for all brush mips
		FOREACHINLIST(CBrushMip, bm_lnInBrush, en_pbrBrush->br_lhBrushMips, itbm) {
			// for all sectors in the mip
			{FOREACHINDYNAMICARRAY(itbm->bm_abscSectors, CBrushSector, itbsc) {
				// for all polygons in this sector
				{FOREACHINSTATICARRAY(itbsc->bsc_abpoPolygons, CBrushPolygon, itbpo) {
					// if the polygon has shadows
					if (!(itbpo->bpo_ulFlags & BPOF_FULLBRIGHT)) {
						// discard shadows
						itbpo->DiscardShadows();
						bHasShadows = TRUE;
					}
				}}
			}}
		}
		// find possible shadow layers near affected area
		if (bHasShadows) {
			if (en_ulFlags&ENF_DYNAMICSHADOWS) {
				_penLightUpdating = NULL;
			} else {
				_penLightUpdating = this;
			}
			en_pwoWorld->FindShadowLayers(en_pbrBrush->GetFirstMip()->bm_boxBoundingBox,
				FALSE, FALSE /* no directional */);
			_penLightUpdating = NULL;
		}

		// if it is zoning
		if (en_ulFlags&ENF_ZONING) {
			// FPU must be in 53-bit mode
			CSetFPUPrecision FPUPrecision(FPT_53BIT);

			// for all brush mips
			FOREACHINLIST(CBrushMip, bm_lnInBrush, en_pbrBrush->br_lhBrushMips, itbm) {
				// for all sectors in the mip
				{FOREACHINDYNAMICARRAY(itbm->bm_abscSectors, CBrushSector, itbsc) {
					// find entities in sector
					itbsc->FindEntitiesInSector();
				}}
			}
		}
	} else if(en_RenderType==RT_TERRAIN) {
		// Update terrain shadow map
		CTerrain *ptrTerrain = GetTerrain();
		Matrix12 mTerrain;
		TR_GetMatrixFromEntity(mTerrain,ptrTerrain);
		TR_UpdateShadowMap(ptrTerrain,mTerrain,FLOATaabbox3D());
	}

	// set spatial clasification
	if (ulFlags & SPIF_NEAR) {
		FindSectorsAroundEntityNear();
	} else {
		FindSectorsAroundEntity();
	}
	// if it is a light source
	{CLightSource *pls = GetLightSource();
	if (pls!=NULL) {
		// find all shadow maps that should have layers from this light source
		pls->FindShadowLayers(FALSE);
		// update shadow map on all terrains in world
		pls->UpdateTerrains(plOld,en_plPlacement);
	}}

	// move the entity to new position in collision grid
	if (en_pciCollisionInfo!=NULL) {
		FLOATaabbox3D boxNew;
		en_pciCollisionInfo->MakeBoxAtPlacement(
			en_plPlacement.pl_PositionVector, en_mRotation, boxNew);
		if (en_RenderType!=RT_BRUSH && en_RenderType!=RT_FIELDBRUSH) {
			en_pwoWorld->MoveEntityInCollisionGrid( this, en_pciCollisionInfo->ci_boxCurrent, boxNew);
		}
		en_pciCollisionInfo->ci_boxCurrent = boxNew;
	}

	// NOTE: this is outside profile because it uses recursion

	// for each nonmovable child of this entity
	{FOREACHINLIST(CEntity, en_lnInParent, en_lhChildren, itenChild) {
		if ((ulFlags&SPIF_SKIPMOVEABLECHILDREN) && (itenChild->en_ulPhysicsFlags&EPF_MOVABLE)) {
			continue;
		}
	
		if (_pNetwork->IsServer() || !(itenChild->en_ulPhysicsFlags&EPF_MOVABLE)) {
			CPlacement3D plNew = itenChild->en_plRelativeToParent;
			plNew.RelativeToAbsoluteSmooth(en_plPlacement);
			itenChild->SetPlacement(plNew);
		}
	}}
}
// this one is used in rendering - gets lerped placement between ticks
CPlacement3D CEntity::GetLerpedPlacement(void) const
{
	// if it has no parent
	if (en_penParent==NULL) {
		// no lerping
		return en_plPlacement;
	// if it has parent
	} else {
		// get lerped placement relative to parent
		CPlacement3D plParentLerped = en_penParent->GetLerpedPlacement();
		CPlacement3D plLerped = en_plRelativeToParent;
		plLerped.RelativeToAbsoluteSmooth(plParentLerped);
		return plLerped;
	}
}

void CEntity::SetFlags(ULONG ulFlags)
{
	en_ulFlags = ulFlags;
}

void CEntity::SetPhysicsFlags(ULONG ulFlags)
{
	// remember the new flags
	en_ulPhysicsFlags = ulFlags;

	// cache eventual collision info
	FindCollisionInfo();
}

void CEntity::SetCollisionFlags(ULONG ulFlags)
{
	// remember the new flags
	en_ulCollisionFlags = ulFlags;

	// cache eventual collision info
	FindCollisionInfo();
}

void CEntity::SetParent(CEntity *penNewParent)
{
	// if there is a parent already
	if (en_penParent!=NULL) {
		// remove from it
		en_penParent = NULL;
		en_lnInParent.Remove();
	}

	// if should set new parent
	if (penNewParent!=NULL) {
		// for each predecesor (parent) entity in the chain
		for (CEntity *penPred=penNewParent; penPred!=NULL; penPred=penPred->en_penParent) {
			// if self
			if (penPred==this) {
				// refuse to set parent
				return;
			}
		}

		// set new parent
		en_penParent = penNewParent;
		penNewParent->en_lhChildren.AddTail(en_lnInParent);
		// calculate relative placement
		en_plRelativeToParent = en_plPlacement;
		en_plRelativeToParent.AbsoluteToRelativeSmooth(en_penParent->en_plPlacement);
	}
}


// find first child of given class
CEntity *CEntity::GetChildOfClass(const char *strClass)
{
	// for each child of this entity
	{FOREACHINLIST(CEntity, en_lnInParent, en_lhChildren, itenChild) {
		// if it is of given class
		if (IsOfClass(itenChild, strClass)) {
			return itenChild;
		}
	}}
	// not found
	return NULL;
}  

/*
 * Destroy this entity (entity must not be targetable).
 */
void CEntity::Destroy(BOOL bNetwork)
{


	// if it is already destroyed
	if (en_ulFlags&ENF_DELETED) {
		// do nothing
		return;
	}	

	if (_pNetwork && _pNetwork->IsServer() && bNetwork && IsSentOverNet()) {
		extern CEntityMessage _emEntityMessage;
		_emEntityMessage.WriteEntityDestroy(en_ulID);
		_pNetwork->ga_srvServer.SendMessage(_emEntityMessage);
	}

	extern BOOL _bWorldEditorApp;
	if (en_ulFlags&ENF_MARKDESTROY && !_bWorldEditorApp && en_pwoWorld!=NULL) {
		ULONG &ulID = en_pwoWorld->wo_aulDestroyedEntities.Push();
		ulID = en_ulID;  
	}
	// if it is a light source
	{CLightSource *pls = GetLightSource();
	if (pls!=NULL) {
		// destroy all of its shadow layers
		pls->DiscardShadowLayers();
	}}
	// clean up the entity
	End();
	SetDefaultProperties(); // this effectively clears all entity pointers!

	// unlink parent-child links
	if (en_penParent != NULL) {
		en_penParent = NULL;
		en_lnInParent.Remove();
	}
	{FORDELETELIST( CEntity, en_lnInParent, en_lhChildren, itenChild) {
		itenChild->en_penParent = NULL;
		itenChild->en_lnInParent.Remove();
	}}

	if (net_bReportClientTraffic) {
//0206
//    CPrintF(TRANS("Entity destroyed:   EntityID: 0x%X\n"),en_ulID);
	}


	// set its flags to mark that it doesn't not exist anymore
	en_ulFlags|=ENF_DELETED;
	// make sure that no deleted entity can be alive
	en_ulFlags&=~ENF_ALIVE;
	// remove from all sectors
	en_rdSectors.Clear();
	// remove from active entities in the world
	if (en_pwoWorld!=NULL) 
	{
		en_pwoWorld->wo_cenEntities.Remove(this);
//안태훈 수정 시작	//(For Performance)(0.2)
		std::vector<CEntity *>::iterator iter = std::find(en_pwoWorld->m_vectorLights.begin(), en_pwoWorld->m_vectorLights.end(), this);
		if( iter != en_pwoWorld->m_vectorLights.end() )
		{
			en_pwoWorld->m_vectorLights.erase( iter );
		}
//안태훈 수정 끝	//(For Performance)(0.2)

		if(_pNetwork->m_bSingleMode)
		{
			en_pwoWorld->m_vectorTargetNPC.erase(
				std::remove(en_pwoWorld->m_vectorTargetNPC.begin(), en_pwoWorld->m_vectorTargetNPC.end(), this), 
				en_pwoWorld->m_vectorTargetNPC.end());

			if(en_pwoWorld->m_vectorTargetNPC.size() == 1)
			{
				_pNetwork->EndRescueNPC();
			}
		}
	}
	// remove the reference made by the entity itself (this can delete it!)
	RemReference();
}

FLOAT3D _vHandle;
CBrushPolygon *_pbpoNear;
CTerrain *_ptrTerrainNear;
FLOAT _fNearDistance;
FLOAT3D _vNearPoint;

static void CheckPolygonForShadingInfo(CBrushPolygon &bpo)
{
	// if it is not a wall
	if (bpo.bpo_ulFlags&(BPOF_INVISIBLE|BPOF_PORTAL) ) {
		// skip it
		return;
	}
	// if the polygon or it's entity are invisible
	if (bpo.bpo_pbscSector->bsc_pbmBrushMip->bm_pbrBrush->br_penEntity->en_ulFlags&ENF_HIDDEN) {
		// skip it
		return;
	}

	const FLOATplane3D &plPolygon = bpo.bpo_pbplPlane->bpl_plAbsolute;
	// find distance of the polygon plane from the handle
	FLOAT fDistance = plPolygon.PointDistance(_vHandle);
	// if it is behind the plane or further than nearest found
	if (fDistance<-0.5f || fabs(fDistance)>_fNearDistance) {
		// skip it
		return;
	}
	// find projection of handle to the polygon plane
	FLOAT3D vOnPlane = plPolygon.ProjectPoint(_vHandle);
	// if it is not in the bounding box of polygon
	const FLOATaabbox3D &boxPolygon = bpo.bpo_boxBoundingBox;
	const FLOAT EPSILON = 0.01f;
	if (
		(boxPolygon.Min()(1)-EPSILON>vOnPlane(1)) ||
		(boxPolygon.Max()(1)+EPSILON<vOnPlane(1)) ||
		(boxPolygon.Min()(2)-EPSILON>vOnPlane(2)) ||
		(boxPolygon.Max()(2)+EPSILON<vOnPlane(2)) ||
		(boxPolygon.Min()(3)-EPSILON>vOnPlane(3)) ||
		(boxPolygon.Max()(3)+EPSILON<vOnPlane(3))) {
		// skip it
		return;
	}

	// find major axes of the polygon plane
	INDEX iMajorAxis1, iMajorAxis2;
	GetMajorAxesForPlane(plPolygon, iMajorAxis1, iMajorAxis2);

	// create an intersector
	CIntersector isIntersector(_vHandle(iMajorAxis1), _vHandle(iMajorAxis2));
	// for all edges in the polygon
	FOREACHINSTATICARRAY(bpo.bpo_abpePolygonEdges, CBrushPolygonEdge, itbpePolygonEdge) {
		// get edge vertices (edge direction is irrelevant here!)
		const FLOAT3D &vVertex0 = itbpePolygonEdge->bpe_pbedEdge->bed_pbvxVertex0->bvx_vAbsolute;
		const FLOAT3D &vVertex1 = itbpePolygonEdge->bpe_pbedEdge->bed_pbvxVertex1->bvx_vAbsolute;
		// pass the edge to the intersector
		isIntersector.AddEdge(
			vVertex0(iMajorAxis1), vVertex0(iMajorAxis2),
			vVertex1(iMajorAxis1), vVertex1(iMajorAxis2));
	}

	// if the point is not inside polygon
	if (!isIntersector.IsIntersecting()) {
		// skip it
		return;
	}

	// remember the polygon
	_pbpoNear = &bpo;
	_fNearDistance = fabs(fDistance);
	_vNearPoint = vOnPlane;
}

static void CheckTerrainForShadingInfo(CTerrain *ptrTerrain)
{
	ASSERT(ptrTerrain!=NULL);
	ASSERT(ptrTerrain->tr_penEntity!=NULL);
	CEntity *pen = ptrTerrain->tr_penEntity;

	FLOAT3D vTerrainNormal;
	FLOAT3D vHitPoint;
	FLOATplane3D plHitPlane;
	vTerrainNormal = FLOAT3D(0,-1,0) * pen->en_mRotation;
	FLOAT fDistance = TR_TestRayCastHit(ptrTerrain, pen->en_mRotation, pen->en_plPlacement.pl_PositionVector,
																			_vHandle,_vHandle+vTerrainNormal,_fNearDistance,FALSE,plHitPlane,vHitPoint);
	if(fDistance<_fNearDistance) {
		_vNearPoint = vHitPoint;
		_fNearDistance = fDistance;
		_ptrTerrainNear = ptrTerrain;
	}
}

/* Find and remember shading info for this entity if invalid. */
void CEntity::FindShadingInfo(void)
{
	// if this entity can't even have shading info
	if (en_psiShadingInfo==NULL) {
		// do nothing
		return;
	}

	// if info is valid
	if (en_ulFlags & ENF_VALIDSHADINGINFO) {
		// !!! check if the polygon is still there !
		// do nothing
		return;
	}
	en_ulFlags |= ENF_VALIDSHADINGINFO;

	en_psiShadingInfo->si_penEntity = this;

	// clear shading info
	en_psiShadingInfo->si_pbpoPolygon = NULL;
	en_psiShadingInfo->si_ptrTerrain  = NULL;
	if (en_psiShadingInfo->si_lnInPolygon.IsLinked()) {
		en_psiShadingInfo->si_lnInPolygon.Remove();
	}

	// take reference point at handle of the model entity
	_vHandle = en_plPlacement.pl_PositionVector;
	// start infinitely far away
	_pbpoNear = NULL;
	_ptrTerrainNear = NULL;
	_fNearDistance = UpperLimit(1.0f);
	// if this is movable entity
	if (en_ulPhysicsFlags&EPF_MOVABLE) {
		// for each cached near polygon
		CStaticStackArray<CBrushPolygon *> &apbpo = ((CMovableEntity*)this)->en_apbpoNearPolygons;
		for(INDEX iPolygon=0; iPolygon<apbpo.Count(); iPolygon++) {
			CheckPolygonForShadingInfo(*apbpo[iPolygon]);
		}
	}

	// for each sector that this entity is in
	FOREACHSRCOFDST(en_rdSectors, CBrushSector, bsc_rsEntities, pbsc)
		// for each brush or terrain in this sector
		FOREACHDSTOFSRC(pbsc->bsc_rsEntities, CEntity, en_rdSectors, pen)
			if(pen->en_RenderType==CEntity::RT_TERRAIN) {
				CheckTerrainForShadingInfo(pen->GetTerrain());
			} else if(pen->en_RenderType!=CEntity::RT_BRUSH && pen->en_RenderType!=CEntity::RT_FIELDBRUSH) {
				break;
			}
		ENDFOR
	ENDFOR

	// if this is non-movable entity, or no polygon or terrain found so far
	if (_pbpoNear==NULL && _ptrTerrainNear==NULL) {
		// for each sector that this entity is in
		FOREACHSRCOFDST(en_rdSectors, CBrushSector, bsc_rsEntities, pbsc)
			// for each polygon in the sector
			{FOREACHINSTATICARRAY(pbsc->bsc_abpoPolygons, CBrushPolygon, itbpo) {
				CBrushPolygon &bpo = *itbpo;
				CheckPolygonForShadingInfo(bpo);
			}}
		ENDFOR
	}

	// if there is some polygon found
	if( _pbpoNear!=NULL) {
		// remember shading info
		en_psiShadingInfo->si_pbpoPolygon = _pbpoNear;
		_pbpoNear->bpo_lhShadingInfos.AddTail(en_psiShadingInfo->si_lnInPolygon);
		en_psiShadingInfo->si_vNearPoint = _vNearPoint;

		CEntity *penWithPolygon = _pbpoNear->bpo_pbscSector->bsc_pbmBrushMip->bm_pbrBrush->br_penEntity;
		ASSERT(penWithPolygon!=NULL);
		const FLOATmatrix3D &mPolygonRotation = penWithPolygon->en_mRotation;
		const FLOAT3D &vPolygonTranslation = penWithPolygon->GetPlacement().pl_PositionVector;

		_vNearPoint = (_vNearPoint-vPolygonTranslation)*!mPolygonRotation;

		MEX2D vmexShadow;
		_pbpoNear->bpo_mdShadow.GetTextureCoordinates(
			_pbpoNear->bpo_pbplPlane->bpl_pwplWorking->wpl_mvRelative,
			_vNearPoint, vmexShadow);
		CBrushShadowMap &bsm = _pbpoNear->bpo_smShadowMap;
		INDEX iMipLevel = bsm.sm_iFirstMipLevel;
		FLOAT fpixU = FLOAT(vmexShadow(1)+bsm.sm_mexOffsetX)*(1.0f/(1<<iMipLevel));
		FLOAT fpixV = FLOAT(vmexShadow(2)+bsm.sm_mexOffsetY)*(1.0f/(1<<iMipLevel));
		en_psiShadingInfo->si_pixShadowU = floor(fpixU);
		en_psiShadingInfo->si_pixShadowV = floor(fpixV);
		en_psiShadingInfo->si_fUDRatio = fpixU-en_psiShadingInfo->si_pixShadowU;
		en_psiShadingInfo->si_fLRRatio = fpixV-en_psiShadingInfo->si_pixShadowV;

	// else if there is some terrain found
	} else if(_ptrTerrainNear!=NULL) {
		// remember shading info
		en_psiShadingInfo->si_ptrTerrain = _ptrTerrainNear;
		en_psiShadingInfo->si_vNearPoint = _vNearPoint;
		
		const CEntity *penTerrain = _ptrTerrainNear->tr_penEntity;
		const FLOATmatrix3D &mRot = penTerrain->en_mRotation;
		const FLOAT3D &vPos = penTerrain->en_plPlacement.pl_PositionVector;
		Matrix12 mObjToAbs;
		MatrixVectorToMatrix12(mObjToAbs, mRot,vPos);
		FLOAT2D vTc = TR_CalcShadingTC(_ptrTerrainNear, _vNearPoint, mObjToAbs);
		en_psiShadingInfo->si_pixShadowU = floor(vTc(1));
		en_psiShadingInfo->si_pixShadowV = floor(vTc(2));
		en_psiShadingInfo->si_fLRRatio   = vTc(1) - en_psiShadingInfo->si_pixShadowU;
		en_psiShadingInfo->si_fUDRatio   = vTc(2) - en_psiShadingInfo->si_pixShadowV;

		_ptrTerrainNear->tr_lhShadingInfos.AddTail(en_psiShadingInfo->si_lnInPolygon);
	}
}

CBrushSector *CEntity::GetFirstSector(void)
{
	{FOREACHSRCOFDST(en_rdSectors, CBrushSector, bsc_rsEntities, pbsc)
		return pbsc;
	ENDFOR};
	return NULL;
}

CBrushSector *CEntity::GetFirstSectorWithName(void)
{
	CBrushSector *pbscResult = NULL;
	{FOREACHSRCOFDST(en_rdSectors, CBrushSector, bsc_rsEntities, pbsc)
		if (pbsc->bsc_strName!="") {
			pbscResult = pbsc;
			break;
		}
	ENDFOR};
	return pbscResult;
}

// max. distance between two spheres (as factor of radius of one sphere)
#define MIN_SPHEREDENSITY 1.0f

CCollisionInfo::CCollisionInfo(const CCollisionInfo &ciOrg)
{
	ci_absSpheres = ciOrg.ci_absSpheres ;
	ci_fMinHeight = ciOrg.ci_fMinHeight ;
	ci_fMaxHeight = ciOrg.ci_fMaxHeight ;
	ci_fHandleY   = ciOrg.ci_fHandleY   ;
	ci_fHandleR   = ciOrg.ci_fHandleR   ;
	ci_boxCurrent = ciOrg.ci_boxCurrent ;
	ci_ulFlags    = ciOrg.ci_ulFlags    ;
}
/* Create collision info for a model. */
void CCollisionInfo::FromModel(CEntity *penModel, INDEX iBox)
{


	// get collision box information from the model
	FLOATaabbox3D boxModel;
	INDEX iBoxType;
	penModel->GetCollisionBoxParameters(iBox, boxModel, iBoxType);
	FLOAT3D vBoxOffset = boxModel.Center();
	FLOAT3D vBoxSize = boxModel.Size();

//  ASSERT(iBoxType==LENGHT_EQ_WIDTH);
	ci_ulFlags = 0;

	INDEX iAxisMain; // in which direction are spheres set
	INDEX iAxis1, iAxis2; // other axis

	if (iBoxType==LENGTH_EQ_WIDTH) {
		iAxisMain = 2;
		iAxis1 = 1; iAxis2 = 3;
	} else if (iBoxType==HEIGHT_EQ_WIDTH) {
		iAxisMain = 3;
		iAxis1 = 2; iAxis2 = 1;
	} else if (iBoxType==LENGTH_EQ_HEIGHT) {
		iAxisMain = 1;
		iAxis1 = 2; iAxis2 = 3;
	} else {
		ASSERTALWAYS("Invalid collision box");
		iAxisMain = 2;
		iAxis1 = 1; iAxis2 = 3;
	}

	// calculate radius of one sphere
	FLOAT fSphereRadius = vBoxSize(iAxis1)/2.0f;
	// calculate length along which to set spheres
	FLOAT fSphereCentersSpan = vBoxSize(iAxisMain)-fSphereRadius*2;
	// calculate number of spheres to use
	INDEX ctSpheres = INDEX(ceil(fSphereCentersSpan/(fSphereRadius*MIN_SPHEREDENSITY)))+1;
	if (ctSpheres <=0) {
		ctSpheres=1;
	}
	// calculate how far from each other to set sphere centers
	FLOAT fSphereCentersDistance;
	if (ctSpheres==1) {
		fSphereCentersDistance = 0.0f;
	} else {
		fSphereCentersDistance = fSphereCentersSpan/(FLOAT)(ctSpheres-1);
	}

	// calculate coordinates for spreading sphere centers
	FLOAT fSphereCenterX = vBoxOffset(iAxis1);
	FLOAT fSphereCenterZ = vBoxOffset(iAxis2);
	FLOAT fSphereCenterY0 = vBoxOffset(iAxisMain)-(vBoxSize(iAxisMain)/2.0f)+fSphereRadius;
	FLOAT fSphereCenterKY = fSphereCentersDistance;

	ci_fMinHeight = boxModel.Min()(2);
	ci_fMaxHeight = boxModel.Max()(2);
	ci_fHandleY = UpperLimit(1.0f);
	// create needed number of spheres in the array
	ci_absSpheres.Clear();
	ci_absSpheres.New(ctSpheres);
	// for each sphere
	for(INDEX iSphere=0; iSphere<ctSpheres; iSphere++) {
		CMovingSphere &ms = ci_absSpheres[iSphere];
		// set its center and radius
		ms.ms_vCenter(iAxis1) = fSphereCenterX;
		ms.ms_vCenter(iAxis2) = fSphereCenterZ;
		ms.ms_vCenter(iAxisMain) = fSphereCenterY0+iSphere*fSphereCenterKY;
		ms.ms_fR = fSphereRadius;
		ci_fHandleY = Min(ci_fHandleY, ms.ms_vCenter(2));
	}

	// remember handle parameters
	if (ctSpheres==1 || iBoxType==LENGTH_EQ_WIDTH) {
		ci_ulFlags|=CIF_CANSTANDONHANDLE;
		ci_fHandleR = fSphereRadius;
	} else {
		ci_fHandleR = 0.0f;
	}

	// set optimization flags

	if (ctSpheres==1 &&
		ci_absSpheres[0].ms_vCenter(1)==0 &&
		ci_absSpheres[0].ms_vCenter(2)==0 &&
		ci_absSpheres[0].ms_vCenter(3)==0) {
		ci_ulFlags|=CIF_IGNOREROTATION;
	}

	if (iBoxType==LENGTH_EQ_WIDTH &&
		ci_absSpheres[0].ms_vCenter(1)==0 &&
		ci_absSpheres[0].ms_vCenter(3)==0) {
		ci_ulFlags|=CIF_IGNOREHEADING;
	}
}

/* Create collision info for a ska model */
void CCollisionInfo::FromSkaModel(CEntity *penModel, INDEX iBox)
{
}
/* Create collision info for a brush. */
void CCollisionInfo::FromBrush(CBrush3D *pbrBrush)
{

	ci_absSpheres.Clear();
	ci_absSpheres.New(1);
	ci_ulFlags = CIF_BRUSH;
	// clear brush's relative box
	FLOATaabbox3D box;
	// get first brush mip
	CBrushMip *pbm = pbrBrush->GetFirstMip();
	// for each sector in the brush mip
	{FOREACHINDYNAMICARRAY(pbm->bm_abscSectors, CBrushSector, itbsc) {
		// for each vertex in the sector
		{FOREACHINSTATICARRAY(itbsc->bsc_abvxVertices, CBrushVertex, itbvx) {
			CBrushVertex &bvx = *itbvx;
			// add it to bounding box
			box |= DOUBLEtoFLOAT(bvx.bvx_vdPreciseRelative);
		}}
	}}

	// create a sphere from the relative box
	ci_absSpheres[0].ms_vCenter = box.Center();
	ci_absSpheres[0].ms_fR = box.Size().Length()/2;
	ci_fMinHeight = UpperLimit(1.0f);
	ci_fMaxHeight = LowerLimit(1.0f);
	ci_fHandleY   = 0.0f;
	ci_fHandleR   = 1.0f;
}

/* Calculate current bounding box in absolute space from position. */
void CCollisionInfo::MakeBoxAtPlacement(const FLOAT3D &vPosition, const FLOATmatrix3D &mRotation,
	FLOATaabbox3D &box)
{

	CMovingSphere &ms0 = ci_absSpheres[0];
	CMovingSphere &ms1 = ci_absSpheres[ci_absSpheres.Count()-1];
	box  = FLOATaabbox3D(vPosition+ms0.ms_vCenter*mRotation, ms0.ms_fR);
	box |= FLOATaabbox3D(vPosition+ms1.ms_vCenter*mRotation, ms1.ms_fR);
}

// get maximum radius of entity in xz plane (relative to entity handle)
FLOAT CCollisionInfo::GetMaxFloorRadius(void)
{


	// get first and last sphere
	CMovingSphere &ms0 = ci_absSpheres[0];
	CMovingSphere &ms1 = ci_absSpheres[ci_absSpheres.Count()-1];
	// get their positions in xz plane
	FLOAT3D vPosXZ0 = ms0.ms_vCenter;
	FLOAT3D vPosXZ1 = ms1.ms_vCenter;
	vPosXZ0(2) = 0.0f;
	vPosXZ1(2) = 0.0f;
	// return maximum distance from the handle in xz plane
	return Max(
		vPosXZ0.Length()+ms0.ms_fR, 
		vPosXZ1.Length()+ms1.ms_fR);
}


/* Find and remember collision info for this entity. */
void CEntity::FindCollisionInfo(void)
{


	// discard eventual collision info
	DiscardCollisionInfo();

	// if the entity is colliding
	if (en_ulCollisionFlags&ECF_TESTMASK) {
		// if it is a model
		if ((en_RenderType==RT_MODEL||en_RenderType==RT_EDITORMODEL)
		&&(en_pmoModelObject->GetData()!=NULL)) {
			// cache its new collision info
			en_pciCollisionInfo = new CCollisionInfo;
			en_pciCollisionInfo->FromModel(this, GetCollisionBoxIndex());
		} else if ((en_RenderType==RT_SKAMODEL||en_RenderType==RT_SKAEDITORMODEL)
			&&(GetModelInstance()!=NULL)) {
			// cache its new collision info
			en_pciCollisionInfo = new CCollisionInfo;
			en_pciCollisionInfo->FromModel(this, GetCollisionBoxIndex());
		// if it is a brush
		} else if (en_RenderType==RT_BRUSH) {
			// if it is zoning brush and non movable
			if ((en_ulFlags&ENF_ZONING) && !(en_ulPhysicsFlags&EPF_MOVABLE)) {
				// do nothing
				return;
			}
			// cache its new collision info
			en_pciCollisionInfo = new CCollisionInfo;
			en_pciCollisionInfo->FromBrush(en_pbrBrush);
		// if it is a field brush
		} else if (en_RenderType==RT_FIELDBRUSH) {
			// cache its new collision info
			en_pciCollisionInfo = new CCollisionInfo;
			en_pciCollisionInfo->FromBrush(en_pbrBrush);
			return;
		} else if (en_RenderType==RT_TERRAIN) {
			return;
		} else {
			return;
		}
		// add entity to collision grid
		FLOATaabbox3D boxNew;
		en_pciCollisionInfo->MakeBoxAtPlacement(
			en_plPlacement.pl_PositionVector, en_mRotation, boxNew);
		if (en_RenderType!=RT_BRUSH && en_RenderType!=RT_FIELDBRUSH) {
			en_pwoWorld->AddEntityToCollisionGrid(this, boxNew);
		}
		en_pciCollisionInfo->ci_boxCurrent = boxNew;
	}
}
// discard collision info for this entity
void CEntity::DiscardCollisionInfo(void)
{
	// if there was any collision info
	if (en_pciCollisionInfo!=NULL) {
		// remove entity from collision grid
		if (en_RenderType!=RT_BRUSH && en_RenderType!=RT_FIELDBRUSH) {
			en_pwoWorld->RemoveEntityFromCollisionGrid(this, en_pciCollisionInfo->ci_boxCurrent);
		}
		// free it
		delete en_pciCollisionInfo;
		en_pciCollisionInfo = NULL;
	}
	// movable entity
	if (en_ulPhysicsFlags & EPF_MOVABLE) {
		((CMovableEntity*)this)->ClearTemporaryData();
	}
}
// copy collision info from some other entity
void CEntity::CopyCollisionInfo(CEntity &enOrg)
{


	// if there is no collision info
	if (enOrg.en_pciCollisionInfo==NULL) {
		// do nothing
		en_pciCollisionInfo = NULL;
		return;
	}
	// create info and copy it
	en_pciCollisionInfo = new CCollisionInfo(*enOrg.en_pciCollisionInfo);
	// add entity to collision grid
	FLOATaabbox3D boxNew;
	en_pciCollisionInfo->MakeBoxAtPlacement(
		en_plPlacement.pl_PositionVector, en_mRotation, boxNew);
	if (en_RenderType!=RT_BRUSH && en_RenderType!=RT_FIELDBRUSH) {
		en_pwoWorld->AddEntityToCollisionGrid(this, boxNew);
	}
	en_pciCollisionInfo->ci_boxCurrent = boxNew;
}

/* Get box and sphere for spatial clasification. */
void CEntity::UpdateSpatialRange(void)
{

	en_fSpatialClassificationRadius = -1.0f;

	// if zoning
	if (en_ulFlags&ENF_ZONING) {
		// do nothing
		return;
	}

	FLOATaabbox3D box;
	FLOATaabbox3D boxStretched;
	// get bounding box of the entity
	// is this old model
	if (en_RenderType==CEntity::RT_MODEL
		||en_RenderType==CEntity::RT_EDITORMODEL) {
		en_pmoModelObject->GetAllFramesBBox(box);
		box.StretchByVector(en_pmoModelObject->mo_Stretch);
		FLOAT3D fClassificationStretch = GetClassificationBoxStretch();
		boxStretched = box;
		boxStretched .StretchByVector( fClassificationStretch);
		en_boxSpatialClassification = boxStretched;
	// is this ska model
	} else if (en_RenderType==CEntity::RT_SKAMODEL
		|| en_RenderType==RT_SKAEDITORMODEL) {
		GetModelInstance()->GetAllFramesBBox(box);
		box.StretchByVector(GetModelInstance()->mi_vStretch);
		FLOAT3D fClassificationStretch = GetClassificationBoxStretch();
		boxStretched = box;
		boxStretched.StretchByVector( fClassificationStretch);
		en_boxSpatialClassification = boxStretched;
	// is this brush
	} else if (en_RenderType==CEntity::RT_BRUSH || en_RenderType==RT_FIELDBRUSH) {
		box = en_pbrBrush->GetFirstMip()->bm_boxRelative;
		boxStretched = box;
		en_boxSpatialClassification = box;
	// is this terrain
	} else if (en_RenderType==CEntity::RT_TERRAIN) {
		const CTerrain *ptrTerrain = GetTerrain();
		box = TR_GetStretchedTerrainBBox(ptrTerrain);
		boxStretched = box;
		en_boxSpatialClassification = box;
	} else {
		return; // sound entities are not related to sectors !!!!
	}
	en_fSpatialClassificationRadius = Max( box.Min().Length(), box.Max().Length() );
	ASSERT(IsValidFloat(en_fSpatialClassificationRadius));
}

/* Find and remember all sectors that this entity is in. */
void CEntity::FindSectorsAroundEntity(void)
{
	CSetFPUPrecision sfp(FPT_53BIT);

	// if not in spatial clasification
	if (en_fSpatialClassificationRadius<0) {
		// do nothing
		return;
	}
	// get bounding sphere and box of entity
	FLOAT fSphereRadius = en_fSpatialClassificationRadius;
	const FLOAT3D &vSphereCenter = en_plPlacement.pl_PositionVector;
	// make oriented bounding box of the entity
	FLOATobbox3D boxEntity = FLOATobbox3D(en_boxSpatialClassification, 
		en_plPlacement.pl_PositionVector, en_mRotation);
	DOUBLEobbox3D boxdEntity = FLOATtoDOUBLE(boxEntity);

	// unset spatial clasification
	en_rdSectors.Clear();

	// for each brush in the world
	FOREACHINDYNAMICARRAY(en_pwoWorld->wo_baBrushes.ba_abrBrushes, CBrush3D, itbr) {
		CBrush3D &br=*itbr;
		// if the brush entity is not zoning
		if (itbr->br_penEntity==NULL || !(itbr->br_penEntity->en_ulFlags&ENF_ZONING)) {
			// skip it
			continue;
		}
		// for each mip in the brush
		FOREACHINLIST(CBrushMip, bm_lnInBrush, itbr->br_lhBrushMips, itbm) {
			// for each sector in the brush mip
			FOREACHINDYNAMICARRAY(itbm->bm_abscSectors, CBrushSector, itbsc) {
				// if the sector's bounding box has contact with the sphere 
				if(itbsc->bsc_boxBoundingBox.TouchesSphere(vSphereCenter, fSphereRadius)
					// and with the box
					&& boxEntity.HasContactWith(FLOATobbox3D(itbsc->bsc_boxBoundingBox))) {
					
					// if the sphere is inside the sector
					if (itbsc->bsc_bspBSPTree.TestSphere(
							FLOATtoDOUBLE(vSphereCenter), FLOATtoDOUBLE(fSphereRadius))>=0) {

						// if the box is inside the sector
						if (itbsc->bsc_bspBSPTree.TestBox(boxdEntity)>=0) {
							// relate the entity to the sector
							if (en_RenderType==RT_BRUSH
								||en_RenderType==RT_FIELDBRUSH
								||en_RenderType==RT_TERRAIN) {  // brushes first
								AddRelationPairHeadHead(itbsc->bsc_rsEntities, en_rdSectors);
							} else {
								AddRelationPairTailTail(itbsc->bsc_rsEntities, en_rdSectors);
							}
						}
					}
				}
			}
		}
	}
}

void CEntity::FindSectorsAroundEntityNear(void)
{


	// if not in spatial clasification
	if (en_fSpatialClassificationRadius<0) {
		// do nothing
		return;
	}
	// this may be called only for movable entities
//	ASSERT(en_ulPhysicsFlags&EPF_MOVABLE);
	CMovableEntity *pen = (CMovableEntity *)this;

	// get bounding sphere and box of entity
	FLOAT fSphereRadius = en_fSpatialClassificationRadius;
	const FLOAT3D &vSphereCenter = en_plPlacement.pl_PositionVector;
	FLOATaabbox3D boxEntity(vSphereCenter, fSphereRadius);
	// make oriented bounding box of the entity
	FLOATobbox3D oboxEntity = FLOATobbox3D(en_boxSpatialClassification, 
		en_plPlacement.pl_PositionVector, en_mRotation);
	DOUBLEobbox3D oboxdEntity = FLOATtoDOUBLE(oboxEntity);

	CListHead lhActive;
	// for each sector around this entity
	{FOREACHSRCOFDST(en_rdSectors, CBrushSector, bsc_rsEntities, pbsc)
		// remember its link
		pbsc->bsc_prlLink = pbsc_iter;
		// add it to list of active sectors
		lhActive.AddTail(pbsc->bsc_lnInActiveSectors);
	ENDFOR}

	CStaticStackArray<CBrushPolygon *> &apbpo = pen->en_apbpoNearPolygons;
	// for each cached polygon
	for(INDEX iPolygon=0; iPolygon<apbpo.Count(); iPolygon++) {
		CBrushSector *pbsc = apbpo[iPolygon]->bpo_pbscSector;
		// add its sector if not already added, and has BSP (is zoning)
		if (!pbsc->bsc_lnInActiveSectors.IsLinked() && pbsc->bsc_bspBSPTree.bt_pbnRoot!=NULL) {
			lhActive.AddTail(pbsc->bsc_lnInActiveSectors);
			pbsc->bsc_prlLink = NULL;
		}
	}

	// for each active sector
	FOREACHINLIST(CBrushSector, bsc_lnInActiveSectors, lhActive, itbsc) {
		CBrushSector *pbsc = itbsc;
		// test if entity is in sector
		BOOL bIn =
				// the sector's bounding box has contact with given bounding box,
				(pbsc->bsc_boxBoundingBox.HasContactWith(boxEntity))&&
				// the sphere is inside the sector
				(pbsc->bsc_bspBSPTree.TestSphere(
							FLOATtoDOUBLE(vSphereCenter), FLOATtoDOUBLE(fSphereRadius))>=0)&&
				// (use more detailed testing for moving brushes)
				(en_RenderType!=RT_BRUSH||
					// oriented box touches box of sector
					(oboxEntity.HasContactWith(FLOATobbox3D(pbsc->bsc_boxBoundingBox)))&&
					// oriented box is in bsp
					(pbsc->bsc_bspBSPTree.TestBox(oboxdEntity)>=0));
		// if it is not
		if (!bIn) {
			// if it has link
			if (pbsc->bsc_prlLink!=NULL) {
				// remove link to that sector
				delete pbsc->bsc_prlLink;
				pbsc->bsc_prlLink = NULL;
			}
		// if it is
		} else {
			// if it doesn't have link
			if (pbsc->bsc_prlLink==NULL) {
				// add the link
				if (en_RenderType==RT_BRUSH
					||en_RenderType==RT_FIELDBRUSH
					||en_RenderType==RT_TERRAIN) {  // brushes first
					AddRelationPairHeadHead(pbsc->bsc_rsEntities, en_rdSectors);
				} else {
					AddRelationPairTailTail(pbsc->bsc_rsEntities, en_rdSectors);
				}
			}
		}
	}

	// clear list of active sectors
	{FORDELETELIST(CBrushSector, bsc_lnInActiveSectors, lhActive, itbsc) {
		itbsc->bsc_prlLink = NULL;
		itbsc->bsc_lnInActiveSectors.Remove();
	}}
	ASSERT(lhActive.IsEmpty());

	// if there is no link found
	if (en_rdSectors.IsEmpty()) {
		// test with brute force algorithm
		FindSectorsAroundEntity();
	}
}

/*
 * Uncache shadows of each polygon that has given gradient index
 */
void CEntity::UncacheShadowsForGradient(INDEX iGradient)
{
	if(en_RenderType != CEntity::RT_BRUSH)
	{
		ASSERTALWAYS("Uncache shadows for gradient called on non-brush entity!");
		return;
	}

	// for all brush mips
	FOREACHINLIST(CBrushMip, bm_lnInBrush, en_pbrBrush->br_lhBrushMips, itbm)
	{
		// for all sectors in the mip
		{FOREACHINDYNAMICARRAY(itbm->bm_abscSectors, CBrushSector, itbsc)
		{
			// for all polygons in this sector
			{FOREACHINSTATICARRAY(itbsc->bsc_abpoPolygons, CBrushPolygon, itbpo)
			{
				// if the polygon has shadows
				if (itbpo->bpo_bppProperties.bpp_ubGradientType == iGradient)
				{
					// uncache shadows
					itbpo->bpo_smShadowMap.Uncache();
				}
			}}
		}}
	}
}

/*
 * Get state transition for given state and event code.
 */
CEntity::pEventHandler CEntity::HandlerForStateAndEvent(SLONG slState, SLONG slEvent)
{
	// find translation in the translation table of the DLL class
	return en_pecClass->HandlerForStateAndEvent(slState, slEvent);
}

/*
 * Handle an event - return false if event was not handled.
 */
BOOL CEntity::HandleEvent(const CEntityEvent &ee)
{
	/*
	 By default, base entities ignore all events.
	 Events are handled by classes derived from CRationalEntity using state stack.

	 Anyway, it is possible to override this in some class if some other way
	 of event handling is desired.
	 */

	return FALSE;
}

/////////////////////////////////////////////////////////////////////
// Event posting system

class CSentEvent {
public:
	CEntityPointer se_penEntity;
	CEntityEvent *se_peeEvent;
	inline void Clear(void) { se_penEntity = NULL; }
};

static CStaticStackArray<CSentEvent> _aseSentEvents;  // delayed events

void ClearSentEvents() 
{
	// for each event
	for(INDEX iee=0; iee<_aseSentEvents.Count(); iee++) {
		CSentEvent &se = _aseSentEvents[iee];
		// release the entity and destroy the event
		se.se_penEntity = NULL;
		delete se.se_peeEvent;
		se.se_peeEvent = NULL;
	}

	// flush all events
	_aseSentEvents.PopAll();
};

/* Send an event to this entity. */
void CEntity::SendEvent(const CEntityEvent &ee,BOOL bNetwork)
{
	if (this==NULL) {
		ASSERT(FALSE);
		return;
	}
	CSentEvent &se = _aseSentEvents.Push();

	if (!_pNetwork->IsServer() && !((CEntityEvent &)ee).CheckIDs()) {
		_aseSentEvents.Pop();
		extern INDEX net_bReportMiscErrors;
		if (net_bReportMiscErrors) {
			CPrintF(TRANS("WARNING: Bad entity ID in an event!\n"));
		}
		return;
	}
	se.se_penEntity = this;
	se.se_peeEvent = ((CEntityEvent&)ee).MakeCopy();  // discard const qualifier

	// send the entity to clients, if neccessary
	if (_pNetwork->IsServer() && bNetwork) {// && IsSentOverNet()) {
		SLONG slSize = ((CEntityEvent&) ee).GetSizeOf() - sizeof(CEntityEvent);
		SLONG slEventCode = ee.ee_slEvent; 
		UWORD uwClassID,uwEventID;

		ASSERT (slSize>=0);   

		uwEventID = slEventCode & 0x0000FFFF;
		uwClassID = (slEventCode & 0xFFFF0000) >> 16;

		if (net_bReportServerTraffic) {
			CPrintF("Sent EntityEvent: EntityID: 0x%X, EntityClass: %d, EventID: %d, Size: %d\n",en_ulID,uwClassID,uwEventID,slSize);
		}
		extern CEntityMessage _emEntityMessage;
		_emEntityMessage.WriteEntityEvent(en_ulID,(SLONG&)ee.ee_slEvent,((UBYTE*)&ee) + sizeof(CEntityEvent),slSize);
		_pNetwork->ga_srvServer.SendMessage(_emEntityMessage);
	}

}

// find entities in a box (box must be around this entity)
void CEntity::FindEntitiesInRange(
	const FLOATaabbox3D &boxRange, CDynamicContainer<CEntity> &cen, BOOL bCollidingOnly)
{


	// for each entity in the world of this entity
	FOREACHINDYNAMICCONTAINER(en_pwoWorld->wo_cenEntities, CEntity, iten) {
		// if it is zoning brush entity
		if (iten->en_RenderType == CEntity::RT_BRUSH && (iten->en_ulFlags&ENF_ZONING)) {
			// get first mip in its brush
			CBrushMip *pbm = iten->en_pbrBrush->GetFirstMip();
			// if the mip doesn't touch the box
			if (!pbm->bm_boxBoundingBox.HasContactWith(boxRange)) {
				// skip it
				continue;
			}

			// for all sectors in this mip
			FOREACHINDYNAMICARRAY(pbm->bm_abscSectors, CBrushSector, itbsc) {
				// if the sector doesn't touch the box
				if (!itbsc->bsc_boxBoundingBox.HasContactWith(boxRange)) {
					// skip it
					continue;
				}

				// for all entities in the sector
				{FOREACHDSTOFSRC(itbsc->bsc_rsEntities, CEntity, en_rdSectors, pen)
					// if the model entity touches the box
					if ((pen->en_RenderType==RT_MODEL || pen->en_RenderType==RT_EDITORMODEL)
						&& boxRange.HasContactWith(
						FLOATaabbox3D(pen->GetPlacement().pl_PositionVector, pen->en_fSpatialClassificationRadius))) {

						// if it has collision box
						if (pen->en_pciCollisionInfo!=NULL) {
							// for each sphere
							FOREACHINSTATICARRAY(pen->en_pciCollisionInfo->ci_absSpheres, CMovingSphere, itms) {
								// project it
								itms->ms_vRelativeCenter0 = itms->ms_vCenter*pen->en_mRotation+pen->en_plPlacement.pl_PositionVector;
								// if the sphere touches the range
								if (boxRange.HasContactWith(FLOATaabbox3D(itms->ms_vRelativeCenter0, itms->ms_fR))) {
									// add it to container
									if (!cen.IsMember(pen)) {
										cen.Add(pen);
									}
									goto next_entity;
								}
							}
						// if no collision box, but non-colliding are allowed
						} else if (!bCollidingOnly) {
							// add it to container
							if (!cen.IsMember(pen)) {
								cen.Add(pen);
							}
						}
					// if the brush entity touches the box
					} else if (pen->en_RenderType==RT_BRUSH && 
						boxRange.HasContactWith(
						FLOATaabbox3D(pen->GetPlacement().pl_PositionVector, pen->en_fSpatialClassificationRadius))) {
						// if the brush touches the box
						if (boxRange.HasContactWith(pen->en_pbrBrush->GetFirstMip()->bm_boxBoundingBox)) {
							// add it to container
							if (!cen.IsMember(pen)) {
								cen.Add(pen);
							}
						}
					} else if ((pen->en_RenderType==RT_SKAMODEL  || pen->en_RenderType==RT_SKAEDITORMODEL)
						&& boxRange.HasContactWith(
						FLOATaabbox3D(pen->GetPlacement().pl_PositionVector, pen->en_fSpatialClassificationRadius))) {
						// if it has collision box
						if (pen->en_pciCollisionInfo!=NULL) {
							// for each sphere
							FOREACHINSTATICARRAY(pen->en_pciCollisionInfo->ci_absSpheres, CMovingSphere, itms) {
								// project it
								itms->ms_vRelativeCenter0 = itms->ms_vCenter*pen->en_mRotation+pen->en_plPlacement.pl_PositionVector;
								// if the sphere touches the range
								if (boxRange.HasContactWith(FLOATaabbox3D(itms->ms_vRelativeCenter0, itms->ms_fR))) {
									// add it to container
									if (!cen.IsMember(pen)) {
										cen.Add(pen);
									}
									goto next_entity;
								}
							}
						// if no collision box, but non-colliding are allowed
						} else if (!bCollidingOnly) {
							// add it to container
							if (!cen.IsMember(pen)) {
								cen.Add(pen);
							}
						}
					}
					next_entity:;
				ENDFOR}
			}
		}
	}
}

/* Send an event to all entities in a box (box must be around this entity). */
void CEntity::SendEventInRange(const CEntityEvent &ee, const FLOATaabbox3D &boxRange)
{
	// find entities in the range
	CDynamicContainer<CEntity> cenToReceive;
	FindEntitiesInRange(boxRange, cenToReceive, FALSE);

	// for each entity in container
	FOREACHINDYNAMICCONTAINER(cenToReceive, CEntity, iten) {
		// send the event to it
		iten->SendEvent(ee);
	}
}

/* Handle all sent events. */
void CEntity::HandleSentEvents(void)
{

	// while there are any unhandled events
	INDEX iFirstEvent = 0;
	while (iFirstEvent<_aseSentEvents.Count()) {
		CSentEvent &se = _aseSentEvents[iFirstEvent];
		// if the entity is not destroyed
		if (!(se.se_penEntity->en_ulFlags&ENF_DELETED)) {
			// handle the current event
			se.se_penEntity->HandleEvent(*se.se_peeEvent);
		}
		// go to next event
		iFirstEvent++;
	}

	// for each event
	for(INDEX iee=0; iee<_aseSentEvents.Count(); iee++) {
		CSentEvent &se = _aseSentEvents[iee];
		// release the entity and destroy the event
		se.se_penEntity = NULL;
		delete se.se_peeEvent;
		se.se_peeEvent = NULL;
	}

	// flush all events
	_aseSentEvents.PopAll();
}

/////////////////////////////////////////////////////////////////////
// DLL class interface

/* Initialize for being virtual entity that is not rendered. */
void CEntity::InitAsVoid(void)
{
	en_RenderType = RT_VOID;
	en_pbrBrush = NULL;
}
/*
 * Initialize for beeing a model object.
 */
void CEntity::InitAsModel(void)
{
	// set render type to model
	en_RenderType = RT_MODEL;
	// create a model object
	ASSERT(en_pmoModelObject==NULL);
	if (en_pmoModelObject!=NULL) {
		delete en_pmoModelObject;
	}
	en_pmoModelObject = new CModelObject;
	ASSERT(en_psiShadingInfo==NULL);
	if (en_psiShadingInfo!=NULL) {
		delete en_psiShadingInfo;
	}
	en_psiShadingInfo = new CShadingInfo;
	en_ulFlags &= ~ENF_VALIDSHADINGINFO;
}
/*
 * Initialize for beeing a ska model object.
 */
void CEntity::InitAsSkaModel(void)
{
	en_RenderType = RT_SKAMODEL;
	ASSERT(en_psiShadingInfo==NULL);
	if (en_psiShadingInfo!=NULL) {
		delete en_psiShadingInfo;
	}
	en_psiShadingInfo = new CShadingInfo;
	en_ulFlags &= ~ENF_VALIDSHADINGINFO;
}

/*
 * Initialize for beeing a terrain object.
 */
void CEntity::InitAsTerrain(void)
{
	en_RenderType = RT_TERRAIN;
	// if there is no existing terrain
	if(en_ptrTerrain == NULL) {
		// create a new empty terrain in the brush archive of current world
		en_ptrTerrain = en_pwoWorld->wo_taTerrains.ta_atrTerrains.New();
		en_ptrTerrain->tr_penEntity = this;


		// Create empty terrain
		const FLOAT3D vTerrainSize = FLOAT3D(256, 50, 256);
		TR_SetHeightMapSize(en_ptrTerrain, 257,257);
		TR_FillHeightMap(en_ptrTerrain, 0x0000);
		TR_SetTerrainSize(en_ptrTerrain, vTerrainSize);
		TR_FillEdgeMap(en_ptrTerrain, 0xFF);
		TR_SetTerrainSize(en_ptrTerrain,FLOAT3D(256,50,256));
		// Create default layer
		INDEX iLayerIndex = TRL_CreateLayer_t(en_ptrTerrain, CTFILENAME("Data\\Textures\\Editor\\Default.tex"));
		TRL_FillLayerData(en_ptrTerrain, iLayerIndex, 0xFF);
		// Update terrain shadowmap
		Matrix12 mTerrain;
		TR_GetMatrixFromEntity(mTerrain,en_ptrTerrain);
		TR_UpdateShadowMap(en_ptrTerrain,mTerrain,FLOATaabbox3D());
		// Update terrain top map
		TR_SetTopMapSize(en_ptrTerrain, 1024, 1024);
	}
	UpdateSpatialRange();
}

/*
 * Initialize for beeing a model object, for editor only.
 */
void CEntity::InitAsEditorModel(void)
{
	// set render type to model
	en_RenderType = RT_EDITORMODEL;
	// create a model object
	ASSERT(en_pmoModelObject==NULL);
	if (en_pmoModelObject!=NULL) {
		delete en_pmoModelObject;
	}
	en_pmoModelObject = new CModelObject;
	ASSERT(en_psiShadingInfo==NULL);
	if (en_psiShadingInfo!=NULL) {
		delete en_psiShadingInfo;
	}
	en_psiShadingInfo = new CShadingInfo;
	en_ulFlags &= ~ENF_VALIDSHADINGINFO;
}
/*
 * Initialize for beeing a ska model object, for editor only.
 */
void CEntity::InitAsSkaEditorModel(void)
{
	// set render type to model
	en_RenderType = RT_SKAEDITORMODEL;
	ASSERT(en_psiShadingInfo==NULL);
	if (en_psiShadingInfo!=NULL) {
		delete en_psiShadingInfo;
	}
	en_psiShadingInfo = new CShadingInfo;
	en_ulFlags &= ~ENF_VALIDSHADINGINFO;
}
/*
 * Initialize for beeing a brush object.
 */
void CEntity::InitAsBrush(void)
{


	// set render type to brush
	en_RenderType = RT_BRUSH;
	// if there is no existing brush
	if (en_pbrBrush == NULL) {
		// create a new empty brush in the brush archive of current world
		en_pbrBrush = en_pwoWorld->wo_baBrushes.ba_abrBrushes.New();
		en_pbrBrush->br_penEntity = this;
		// create a brush mip for it
		CBrushMip *pbmMip = new CBrushMip;
		// add it to list
		en_pbrBrush->br_lhBrushMips.AddTail(pbmMip->bm_lnInBrush);
		// set back-pointer to the brush
		pbmMip->bm_pbrBrush = en_pbrBrush;
		en_pbrBrush->CalculateBoundingBoxes();
	}
	UpdateSpatialRange();
}

/*
 * Initialize for beeing a field brush object.
 */
void CEntity::InitAsFieldBrush(void)
{


	// set render type to brush
	en_RenderType = RT_FIELDBRUSH;
	// if there is no existing brush
	if (en_pbrBrush == NULL) {
		// create a new empty brush in the brush archive of current world
		en_pbrBrush = en_pwoWorld->wo_baBrushes.ba_abrBrushes.New();
		en_pbrBrush->br_penEntity = this;
		// create a brush mip for it
		CBrushMip *pbmMip = new CBrushMip;
		// add it to list
		en_pbrBrush->br_lhBrushMips.AddTail(pbmMip->bm_lnInBrush);
		// set back-pointer to the brush
		pbmMip->bm_pbrBrush = en_pbrBrush;
		en_pbrBrush->CalculateBoundingBoxes();
	}
	UpdateSpatialRange();
}

/*
 *  Switch to Model/Editor model
 */
void CEntity::SwitchToModel(void)
{
	// change to editor model
	if(en_RenderType==RT_MODEL || en_RenderType==RT_EDITORMODEL) {
		en_RenderType = RT_MODEL;
	} else if( en_RenderType==RT_SKAMODEL || en_RenderType==RT_SKAEDITORMODEL) {
		en_RenderType = RT_SKAMODEL;
	} else {
		// it must be model (not brush)
		ASSERT(FALSE);
	}
}
void CEntity::SwitchToEditorModel(void)
{
	// change to editor model
	if(en_RenderType==RT_MODEL || en_RenderType==RT_EDITORMODEL) {
		en_RenderType = RT_EDITORMODEL;
	} else if( en_RenderType==RT_SKAMODEL || en_RenderType==RT_SKAEDITORMODEL) {
		en_RenderType = RT_SKAEDITORMODEL;
	} else {
		// it must be model (not brush)
		ASSERT(FALSE);
	}
}

/////////////////////////////////////////////////////////////////////
// Model manipulation
/* Set the model data for model entity. */
void CEntity::SetModel(const CTFileName &fnmModel)
{
	ASSERT(en_RenderType==RT_MODEL || en_RenderType==RT_EDITORMODEL);
	// try to
	try {
		// load the new model data
		en_pmoModelObject->SetData_t(fnmModel);
	// if failed
	} catch(char *strError) {
		(void)strError;
		DECLARE_CTFILENAME(fnmDefault, "Data\\Models\\Editor\\Axis.mdl");
		// try to
		try {
			// load the default model data
			en_pmoModelObject->SetData_t(fnmDefault);
		// if failed
		} catch(char *strErrorDefault) {
			FatalError(TRANS("Cannot load default model '%s':\n%s"),
				(CTString&)fnmDefault, strErrorDefault);
		}
	}
	UpdateSpatialRange();
	FindCollisionInfo();
}

void CEntity::SetModel(SLONG idModelComponent)
{
	ASSERT(en_RenderType==RT_MODEL || en_RenderType==RT_EDITORMODEL);
	CEntityComponent *pecModel = en_pecClass->ComponentForTypeAndID(
		ECT_MODEL, idModelComponent);
	en_pmoModelObject->SetData(pecModel->ec_pmdModel);
	UpdateSpatialRange();
	FindCollisionInfo();
}

void CEntity::SetSkaColisionInfo()
{
	ASSERT(en_RenderType==RT_SKAMODEL || en_RenderType==RT_SKAEDITORMODEL);
	// if there is no colision boxes for ska model 
	if(en_pmiModelInstance->mi_cbAABox.Count() == 0) {
		// add one default colision box
		en_pmiModelInstance->AddColisionBox("Default",FLOAT3D(-0.5,0,-0.5),FLOAT3D(0.5,2,0.5));
	}
	UpdateSpatialRange();
	FindCollisionInfo();
}

void CEntity::SetSkaModel_t(const CTString &fnmModel)
{
	ASSERT(en_RenderType==RT_SKAMODEL || en_RenderType==RT_SKAEDITORMODEL);
	try {
		// if no model instance exists for entity
		if(en_pmiModelInstance==NULL) {
			// Create him new one
			en_pmiModelInstance = CreateModelInstance("");
		}

		ASSERT(en_pmiModelInstance!=NULL);
		// Obtain copy of requested model instance
		ObtainModelInstance_t(*en_pmiModelInstance, fnmModel);
	} catch (char *strErrorDefault) {
		throw(strErrorDefault);
	}
	SetSkaColisionInfo();
}

void CEntity::DelSkaModel(const CTString &fnmModel)
{
	if (en_pmiModelInstance != NULL)
	{
		CTString strFullPath = _fnmApplicationPath.FileDir();
		strFullPath += fnmModel;

		CSmcParser tmpParser;
		tmpParser.LoadSmcParse(std::string(strFullPath.str_String));

		int iMeshCount = tmpParser.GetMeshInfoListSize();
		int i;

		for (i=0; i<iMeshCount; ++i)
		{
			CMeshInfo Mesh;
			Mesh = tmpParser.GetMeshInfo(i);

			en_pmiModelInstance->DeleteMesh(Mesh.GetMeshTFNM());
		}	
	}
}

void CEntity::AddSkaModel(const CTString &fnmModel)
{
	if (en_pmiModelInstance != NULL)
	{
		CTString strFullPath = _fnmApplicationPath.FileDir();
		strFullPath += fnmModel;		

		CSmcParser tmpParser;
		tmpParser.LoadSmcParse(std::string(strFullPath.str_String));

		int iMeshCount = tmpParser.GetMeshInfoListSize();
		int i,j;
		MeshInstance *mi;

		for (i=0; i<iMeshCount; ++i)
		{
			CMeshInfo Mesh;
			Mesh = tmpParser.GetMeshInfo(i);

			INDEX nID = ska_GetIDFromStringTable(Mesh.GetMeshTFNM());

			if (en_pmiModelInstance->FindMeshInstance(nID) != NULL)
				continue;

			mi = en_pmiModelInstance->AddArmor(Mesh.GetMeshTFNM());
			int iSize = Mesh.GetTexInfoSize();
			
			for (j=0; j<iSize; ++j)
			{
				CTFileName fnFileName = (CTString)Mesh.GetTexInfoTFNM(j);
				en_pmiModelInstance->AddTexture_t(fnFileName, fnFileName.FileName(), mi);
			}
		}
	}
}

bool CEntity::HasSkaModel( const CTString &fnmModel )
{
	if (en_pmiModelInstance != NULL)
	{
		CTString strFullPath = _fnmApplicationPath.FileDir();
		strFullPath += fnmModel;		

		CSmcParser tmpParser;
		tmpParser.LoadSmcParse(std::string(strFullPath.str_String));

		int iMeshCount = tmpParser.GetMeshInfoListSize();
		int i;

		for (i=0; i<iMeshCount; ++i)
		{
			CMeshInfo Mesh;
			Mesh = tmpParser.GetMeshInfo(i);

			INDEX nID = ska_GetIDFromStringTable(Mesh.GetMeshTFNM());

			if (en_pmiModelInstance->FindMeshInstance(nID) != NULL)
				return true;
		}
	}

	return false;
}

BOOL CEntity::SetSkaModel(const CTString &fnmModel)
{
	
	
	ASSERT(en_RenderType==RT_SKAMODEL || en_RenderType==RT_SKAEDITORMODEL);
	// try to
	try {
		SetSkaModel_t(fnmModel);
	// if failed
	} 
	catch(char *strError) 
	{
#if		_USE_DEFAULT_MODEL_
		WarningMessage("%s\n\rLoading default model.\n", (const char*)strError);
		DECLARE_CTFILENAME(fnmDefault, "Data\\Models\\Editor\\Ska\\Axis.smc");
		// try to
		try {
			// load the default model data
			SetSkaModel_t(fnmDefault);
		// if failed
		} catch(char *strErrorDefault) {
			FatalError(TRANS("Cannot load default model '%s':\n%s"),
				(const char*)fnmDefault, (const char*)strErrorDefault);
		}
#else	// _USE_DEFAULT_MODEL_
		FatalError(TRANS("Cannot load default model '%s':\n%s"),
			(const char*)fnmModel, (const char*)strError);
#endif	// _USE_DEFAULT_MODEL_
		// set colision info for default model
		SetSkaColisionInfo();
		return FALSE;
	}
	return TRUE;
}

//0601 kwon 함수 추가
// FIXME : 이거 왜 만든거야???
CModelInstance* CEntity::SetSkaModelItem_t(const CTString &fnmModel)
{
	ASSERT(en_RenderType==RT_SKAMODEL || en_RenderType==RT_SKAEDITORMODEL);

	CModelInstance *pmi;
	try {
		// if no model instance exists for entity
		if(en_pmiModelInstance==NULL) {
			// Create him new one
			en_pmiModelInstance = CreateModelInstance("");
		}

		ASSERT(en_pmiModelInstance!=NULL);
		// Obtain copy of requested model instance		
		pmi = ObtainModelInstanceItem_t(*en_pmiModelInstance, fnmModel);
	} catch (char *strErrorDefault) {
		throw(strErrorDefault);
	}
	SetSkaColisionInfo();

	return pmi;
}

// FIXME : 이거 왜 만든거야???
CModelInstance* CEntity::SetSkaModelItem(const CTString &fnmModel)
{
	ASSERT(en_RenderType==RT_SKAMODEL || en_RenderType==RT_SKAEDITORMODEL);

	CModelInstance *pmi;

	// try to
	try {		
		pmi = SetSkaModelItem_t(fnmModel);
	// if failed
	} catch(char *strError) {
		WarningMessage("%s\n\rLoading default model.\n", (const char*)strError);
		DECLARE_CTFILENAME(fnmDefault, "Data\\Models\\Editor\\Ska\\Axis.smc");
		// try to
		try {
			// load the default model data
			SetSkaModel_t(fnmDefault);
		// if failed
		} catch(char *strErrorDefault) {
			FatalError(TRANS("Cannot load default model '%s':\n%s"),
				(const char*)fnmDefault, (const char*)strErrorDefault);
		}
		// set colision info for default model
		SetSkaColisionInfo();
		return NULL;
	}
	return pmi;
}

void CEntity::SetSkaModel(SLONG idSkaModelComponent)
{
	ASSERT(en_RenderType==RT_SKAMODEL || en_RenderType==RT_SKAEDITORMODEL);
	CEntityComponent *pecSkaModel = en_pecClass->ComponentForTypeAndID(ECT_SKAMODEL, idSkaModelComponent);
	ASSERT(pecSkaModel!=NULL);
	ASSERT(pecSkaModel->ec_pmisModelInstance!=NULL);
	ASSERT(pecSkaModel->ec_pmisModelInstance->mis_pmiModelInstance!=NULL);
	CModelInstance *pmiComponent = pecSkaModel->ec_pmisModelInstance->mis_pmiModelInstance;
	// if entity does not have model instance
	if(en_pmiModelInstance==NULL) {
		// create new model instance
		en_pmiModelInstance = CreateModelInstance("");
	}
	// copy model instance from component to entity model instance 
	en_pmiModelInstance->Copy(*pmiComponent);
	SetSkaColisionInfo();
	UpdateSpatialRange();
	FindCollisionInfo();
}
// set/get model main blend color
void CEntity::SetModelColor( const COLOR colBlend)
{
//안태훈 수정 시작	//(Add & Modify SSSE Effect)(0.1)
	//ASSERT(en_RenderType==RT_MODEL || en_RenderType==RT_EDITORMODEL);	//원본
	//en_pmoModelObject->mo_colBlendColor = colBlend;					//원본
	if(en_RenderType==RT_MODEL || en_RenderType==RT_EDITORMODEL)
	{
		en_pmoModelObject->mo_colBlendColor = colBlend;
	}
	else if(en_RenderType==RT_SKAMODEL || en_RenderType==RT_SKAEDITORMODEL)
	{
		if(en_pmiModelInstance) en_pmiModelInstance->SetModelColor(colBlend);
	}
//안태훈 수정 끝	//(Add & Modify SSSE Effect)(0.1)
}

COLOR CEntity::GetModelColor(void) const
{
//안태훈 수정 시작	//(Add & Modify SSSE Effect)(0.1)
	//ASSERT(en_RenderType==RT_MODEL || en_RenderType==RT_EDITORMODEL);
	//return en_pmoModelObject->mo_colBlendColor;
	if(en_RenderType==RT_MODEL || en_RenderType==RT_EDITORMODEL)
	{
		return en_pmoModelObject->mo_colBlendColor;
	}
	else if(en_RenderType==RT_SKAMODEL || en_RenderType==RT_SKAEDITORMODEL)
	{
		if(en_pmiModelInstance) return en_pmiModelInstance->GetModelColor();
	}
	return C_WHITE|CT_OPAQUE;
//안태훈 수정 끝	//(Add & Modify SSSE Effect)(0.1)
}


/* Get the model data for model entity. */
const CTFileName &CEntity::GetModel(void)
{
	ASSERT(en_RenderType==RT_MODEL || en_RenderType==RT_EDITORMODEL);
	return ((CAnimData*)en_pmoModelObject->GetData())->GetName();
}

/* Start new animation for model entity. */
void CEntity::StartModelAnim(INDEX iNewModelAnim, ULONG ulFlags,BOOL bNetwork)
{
	ASSERT(en_RenderType==RT_MODEL || en_RenderType==RT_EDITORMODEL);

	INDEX iNew  = iNewModelAnim;
	if (iNew >= en_pmoModelObject->GetAnimsCt()) {
		iNew = 0;
	};
	BOOL bSend = !IsPlayer() || ((!(ulFlags&AOF_NORESTART) || (en_pmoModelObject->ao_iCurrentAnim!=iNew)) || (en_pmoModelObject->ao_ulFlags != (ulFlags&(AOF_LOOPING|AOF_PAUSED))));
	if (bSend && _pNetwork->IsServer() && bNetwork && IsSentOverNet()) {
		extern CEntityMessage _emEntityMessage;
		_emEntityMessage.WriteStartAnim(en_ulID,iNewModelAnim, ulFlags);
		_pNetwork->ga_srvServer.SendMessage(_emEntityMessage);
	}

	en_pmoModelObject->PlayAnim(iNewModelAnim, ulFlags);
}


/* Add a new animation to Ska amination stack */
void CEntity::AddAnimation(INDEX iAnimID, ULONG ulFlags, FLOAT fStrength, INDEX iGroupID, INDEX iModelInstanceID /*=master*/, FLOAT fSpeedMul /*=1.0f*/) 
{
	ASSERT (en_RenderType==RT_SKAMODEL || en_RenderType==RT_SKAEDITORMODEL);
	ASSERT (en_pmiModelInstance != NULL);
	if (en_pmiModelInstance == NULL) {
		extern INDEX net_bReportMiscErrors;
		if (net_bReportMiscErrors) {
			CPrintF(TRANS("Warning: invalid modelinstance for AddAnimation, entity id: 0x%X\n"),en_ulID);
		}
		return;
	}

	
	/*
	if (iModelInstanceID == ESKA_MASTER_MODEL_INSTANCE || iModelInstanceID == en_pmiModelInstance->GetID()) 
	{
		en_pmiModelInstance->AddAnimation(iAnimID,ulFlags,1,0);
	} else {
		CModelInstance* pmiChild = en_pmiModelInstance->GetChild(iModelInstanceID,TRUE);
		ASSERT (pmiChild != NULL);
		if (pmiChild == NULL) {
			extern INDEX net_bReportMiscErrors;
			if (net_bReportMiscErrors) {
				CPrintF(TRANS("Warning: invalid modelinstance child for AddAnimation, entity id: 0x%X child id: 0x%x\n"),en_ulID,iModelInstanceID);
			}
			return;
		}		
		pmiChild->AddAnimationChild(iAnimID,ulFlags,fStrength,iGroupID,fSpeedMul);
	}
	*/
	en_pmiModelInstance->AddAnimation(iAnimID,ulFlags,fStrength,iGroupID,fSpeedMul);

	if (_pNetwork->IsServer() && IsSentOverNet()) {
		SLONG slDataSize = sizeof(iAnimID) + sizeof(ulFlags) + sizeof(fStrength) + sizeof(iGroupID) + sizeof(iModelInstanceID) + sizeof(fSpeedMul);
		UBYTE* pubData = _abSkaData;
		*(INDEX*)pubData = iAnimID; pubData += sizeof(iAnimID);
		*(ULONG*)pubData = ulFlags; pubData += sizeof(ulFlags);
		*(FLOAT*)pubData = fStrength; pubData += sizeof(fStrength);
		*(INDEX*)pubData = iGroupID; pubData += sizeof(iGroupID);
		*(INDEX*)pubData = iModelInstanceID; pubData += sizeof(iModelInstanceID);
		*(FLOAT*)pubData = fSpeedMul; pubData += sizeof(fSpeedMul);  
		extern CEntityMessage _emEntityMessage;
		_emEntityMessage.WriteSka(en_ulID,EM_SKA_ADD,_abSkaData,(SLONG)slDataSize);
		_pNetwork->ga_srvServer.SendMessage(_emEntityMessage);
	}
}  

											
/* remove an animation from stack */
void CEntity::RemAnimation(INDEX iAnimID, INDEX iModelInstanceID /*=master*/)
{
	ASSERT (en_RenderType==RT_SKAMODEL || en_RenderType==RT_SKAEDITORMODEL);
	ASSERT (en_pmiModelInstance != NULL);
	if (en_pmiModelInstance == NULL) {
		extern INDEX net_bReportMiscErrors;
		if (net_bReportMiscErrors) {
			CPrintF(TRANS("Warning: invalid modelinstance for RemAnimation, entity id: 0x%X\n"),en_ulID);
		}
		return;
	}

	if (iModelInstanceID == ESKA_MASTER_MODEL_INSTANCE || iModelInstanceID == en_pmiModelInstance->GetID()) {
		en_pmiModelInstance->RemAnimation(iAnimID);
	} else {
		CModelInstance* pmiChild = en_pmiModelInstance->GetChild(iModelInstanceID,TRUE);
		ASSERT (pmiChild != NULL);    
		if (pmiChild == NULL) {
			extern INDEX net_bReportMiscErrors;
			if (net_bReportMiscErrors) {
				CPrintF(TRANS("Warning: invalid modelinstance child for RemAnimation, entity id: 0x%X child id: 0x%x\n"),en_ulID,iModelInstanceID);
			}
			return;
		}
		pmiChild->RemAnimation(iAnimID);
	}

	if (_pNetwork->IsServer() && IsSentOverNet()) {
		SLONG slDataSize = sizeof(iAnimID) + sizeof(iModelInstanceID);
		UBYTE* pubData = _abSkaData;
		*(INDEX*)pubData = iAnimID; pubData += sizeof(iAnimID);
		*(INDEX*)pubData = iModelInstanceID; pubData += sizeof(iModelInstanceID);
		extern CEntityMessage _emEntityMessage;
		_emEntityMessage.WriteSka(en_ulID,EM_SKA_REM,_abSkaData,slDataSize);
		_pNetwork->ga_srvServer.SendMessage(_emEntityMessage);
	}

}


/* Remove all anims with GroupID */
void CEntity::RemAnimsWithID(INDEX iGroupID, INDEX iModelInstanceID /*=master*/)
{
	ASSERT (en_RenderType==RT_SKAMODEL || en_RenderType==RT_SKAEDITORMODEL);
	ASSERT (en_pmiModelInstance != NULL);
	if (en_pmiModelInstance == NULL) {
		extern INDEX net_bReportMiscErrors;
		if (net_bReportMiscErrors) {
			CPrintF(TRANS("Warning: invalid modelinstance for RemAnimsWithID, entity id: 0x%X\n"),en_ulID);
		}
		return;
	}

	if (iModelInstanceID == ESKA_MASTER_MODEL_INSTANCE || iModelInstanceID == en_pmiModelInstance->GetID()) {
		en_pmiModelInstance->RemAnimsWithID(iGroupID);
	} else {
		CModelInstance* pmiChild = en_pmiModelInstance->GetChild(iModelInstanceID,TRUE);
		ASSERT (pmiChild != NULL);
		if (pmiChild == NULL) {
			extern INDEX net_bReportMiscErrors;
			if (net_bReportMiscErrors) {
				CPrintF(TRANS("Warning: invalid modelinstance child for RemAnimsWithID, entity id: 0x%X child id: 0x%x\n"),en_ulID,iModelInstanceID);
			}
			return;
		}
		pmiChild->RemAnimsWithID(iGroupID);
	}

	if (_pNetwork->IsServer() && IsSentOverNet()) {
		SLONG slDataSize = sizeof(iGroupID) + sizeof(iModelInstanceID);
		UBYTE* pubData = _abSkaData;
		*(INDEX*)pubData = iGroupID; pubData += sizeof(iGroupID);
		*(INDEX*)pubData = iModelInstanceID; pubData += sizeof(iModelInstanceID);
		extern CEntityMessage _emEntityMessage;
		_emEntityMessage.WriteSka(en_ulID,EM_SKA_REMID,_abSkaData,slDataSize);
		_pNetwork->ga_srvServer.SendMessage(_emEntityMessage);
	}

}

/* create new state, copy last state in it and give it a fade time */
void CEntity::NewClonedState(FLOAT fFadeTime, INDEX iModelInstanceID /*=master*/)
{
	ASSERT (en_RenderType==RT_SKAMODEL || en_RenderType==RT_SKAEDITORMODEL);
	ASSERT (en_pmiModelInstance != NULL);
	if (en_pmiModelInstance == NULL) {
		extern INDEX net_bReportMiscErrors;
		if (net_bReportMiscErrors) {
			CPrintF(TRANS("Warning: invalid modelinstance for NewClonedState, entity id: 0x%X\n"),en_ulID);
		}
		return;
	}

	if (iModelInstanceID == ESKA_MASTER_MODEL_INSTANCE || iModelInstanceID == en_pmiModelInstance->GetID()) {
		en_pmiModelInstance->NewClonedState(fFadeTime);
	} else {
		CModelInstance* pmiChild = en_pmiModelInstance->GetChild(iModelInstanceID,TRUE);
		ASSERT (pmiChild != NULL);
		if (pmiChild == NULL) {
			extern INDEX net_bReportMiscErrors;
			if (net_bReportMiscErrors) {
				CPrintF(TRANS("Warning: invalid modelinstance child for NewClonedState, entity id: 0x%X child id: 0x%x\n"),en_ulID,iModelInstanceID);
			}
			return;
		}
		pmiChild->NewClonedState(fFadeTime);
	}

	if (_pNetwork->IsServer() && IsSentOverNet()) {
		SLONG slDataSize = sizeof(fFadeTime) + sizeof(iModelInstanceID);
		UBYTE* pubData = _abSkaData;
		*(FLOAT*)pubData = fFadeTime; pubData += sizeof(fFadeTime);
		*(INDEX*)pubData = iModelInstanceID; pubData += sizeof(iModelInstanceID);
		extern CEntityMessage _emEntityMessage;
		_emEntityMessage.WriteSka(en_ulID,EM_SKA_NEWCLONED,_abSkaData,slDataSize);
		_pNetwork->ga_srvServer.SendMessage(_emEntityMessage);
	}

}

/* create new cleared state and give it a fade time */
void CEntity::NewClearState(FLOAT fFadeTime, INDEX iModelInstanceID /*=master*/)
{
	ASSERT (en_RenderType==RT_SKAMODEL || en_RenderType==RT_SKAEDITORMODEL);
	ASSERT (en_pmiModelInstance != NULL);
	if (en_pmiModelInstance == NULL) {
		extern INDEX net_bReportMiscErrors;
		if (net_bReportMiscErrors) {
			CPrintF(TRANS("Warning: invalid modelinstance for NewClearState, entity id: 0x%X\n"),en_ulID);
		}
		return;
	}

	if (iModelInstanceID == ESKA_MASTER_MODEL_INSTANCE || iModelInstanceID == en_pmiModelInstance->GetID()) {
		en_pmiModelInstance->NewClearState(fFadeTime);
	} else {
		CModelInstance* pmiChild = en_pmiModelInstance->GetChild(iModelInstanceID,TRUE);
		ASSERT (pmiChild != NULL);
		if (pmiChild == NULL) {
			extern INDEX net_bReportMiscErrors;
			if (net_bReportMiscErrors) {
				CPrintF(TRANS("Warning: invalid modelinstance child for NewClearState, entity id: 0x%X child id: 0x%x\n"),en_ulID,iModelInstanceID);
			}
			return;
		}
		pmiChild->NewClearState(fFadeTime);
	}
	
	if (_pNetwork->IsServer() && IsSentOverNet()) {
		SLONG slDataSize = sizeof(fFadeTime) + sizeof(iModelInstanceID);
		UBYTE* pubData = _abSkaData;
		*(FLOAT*)pubData = fFadeTime; pubData += sizeof(fFadeTime);
		*(INDEX*)pubData = iModelInstanceID; pubData += sizeof(iModelInstanceID);
		extern CEntityMessage _emEntityMessage;
		_emEntityMessage.WriteSka(en_ulID,EM_SKA_NEWCLEAR,_abSkaData,slDataSize);
		_pNetwork->ga_srvServer.SendMessage(_emEntityMessage);
	}

}


/* Check if given animation is currently playing */
BOOL CEntity::IsAnimationPlaying(INDEX iAnimID, INDEX iModelInstanceID /*=master*/)
{
	ASSERT (en_RenderType==RT_SKAMODEL || en_RenderType==RT_SKAEDITORMODEL);
	ASSERT (en_pmiModelInstance != NULL);
	if (en_pmiModelInstance == NULL) {
		extern INDEX net_bReportMiscErrors;
		if (net_bReportMiscErrors) {
			CPrintF(TRANS("Warning: invalid modelinstance for IsAnimationPlaying, entity id: 0x%X\n"),en_ulID);
		}
		return TRUE;
	}

	if (iModelInstanceID == ESKA_MASTER_MODEL_INSTANCE || iModelInstanceID == en_pmiModelInstance->GetID()) {
		return en_pmiModelInstance->IsAnimationPlaying(iAnimID);
	} else {
		CModelInstance* pmiChild = en_pmiModelInstance->GetChild(iModelInstanceID,TRUE);
		ASSERT (pmiChild != NULL);
		return pmiChild->IsAnimationPlaying(iAnimID);
	}

}


/* Stretch model instance */
void CEntity::StretchModel(FLOAT3D &vStretch, INDEX iModelInstanceID)
{
	ASSERT (en_RenderType==RT_SKAMODEL || en_RenderType==RT_SKAEDITORMODEL);
	ASSERT (en_pmiModelInstance != NULL);
	if (en_pmiModelInstance == NULL) {
		extern INDEX net_bReportMiscErrors;
		if (net_bReportMiscErrors) {
			CPrintF(TRANS("Warning: invalid modelinstance for StretchModel, entity id: 0x%X\n"),en_ulID);
		}
		return;
	}

	if (iModelInstanceID == ESKA_MASTER_MODEL_INSTANCE || iModelInstanceID == en_pmiModelInstance->GetID()) {
		en_pmiModelInstance->StretchModel(vStretch);
	} else {
		CModelInstance* pmiChild = en_pmiModelInstance->GetChild(iModelInstanceID,TRUE);
		ASSERT (pmiChild != NULL);
		pmiChild->StretchModel(vStretch);
	}

	if (_pNetwork->IsServer() && IsSentOverNet()) {
		SLONG slDataSize = 3*sizeof(FLOAT) + sizeof(iModelInstanceID);
		UBYTE* pubData = _abSkaData;
		*(FLOAT*)pubData = vStretch(1); pubData += sizeof(FLOAT);
		*(FLOAT*)pubData = vStretch(2); pubData += sizeof(FLOAT);
		*(FLOAT*)pubData = vStretch(3); pubData += sizeof(FLOAT);
		*(INDEX*)pubData = iModelInstanceID; pubData += sizeof(iModelInstanceID);
		extern CEntityMessage _emEntityMessage;
		_emEntityMessage.WriteSka(en_ulID,EM_SKA_STRETCH,_abSkaData,slDataSize);
		_pNetwork->ga_srvServer.SendMessage(_emEntityMessage);
	}

}

/* Stretch model instance without attachments */
void CEntity::StretchSingleModel(FLOAT3D &vStretch, INDEX iModelInstanceID)
{
	ASSERT (en_RenderType==RT_SKAMODEL || en_RenderType==RT_SKAEDITORMODEL);
	ASSERT (en_pmiModelInstance != NULL);
	if (en_pmiModelInstance == NULL) {
		extern INDEX net_bReportMiscErrors;
		if (net_bReportMiscErrors) {
			CPrintF(TRANS("Warning: invalid modelinstance for StretchSingleModel, entity id: 0x%X\n"),en_ulID);
		}
		return;
	}

	if (iModelInstanceID == ESKA_MASTER_MODEL_INSTANCE || iModelInstanceID == en_pmiModelInstance->GetID()) {
		en_pmiModelInstance->StretchSingleModel(vStretch);
	} else {
		CModelInstance* pmiChild = en_pmiModelInstance->GetChild(iModelInstanceID,TRUE);
		ASSERT (pmiChild != NULL);
		pmiChild->StretchSingleModel(vStretch);
	}

	if (_pNetwork->IsServer() && IsSentOverNet()) {
		SLONG slDataSize = 3*sizeof(FLOAT) + sizeof(iModelInstanceID);
		UBYTE* pubData = _abSkaData;
		*(FLOAT*)pubData = vStretch(1); pubData += sizeof(FLOAT);
		*(FLOAT*)pubData = vStretch(2); pubData += sizeof(FLOAT);
		*(FLOAT*)pubData = vStretch(3); pubData += sizeof(FLOAT);
		*(INDEX*)pubData = iModelInstanceID; pubData += sizeof(iModelInstanceID);
		extern CEntityMessage _emEntityMessage;
		_emEntityMessage.WriteSka(en_ulID,EM_SKA_STRETCHSINGLE,_abSkaData,slDataSize);
		_pNetwork->ga_srvServer.SendMessage(_emEntityMessage);
	}
}

/* Offset all animations in anim queue */
void CEntity::OffSetAnimationQueue(TIME fOffsetTime, INDEX iModelInstanceID /*=master*/)
{
	ASSERT (en_RenderType==RT_SKAMODEL || en_RenderType==RT_SKAEDITORMODEL);
	ASSERT (en_pmiModelInstance != NULL);
	if (en_pmiModelInstance == NULL) {
		extern INDEX net_bReportMiscErrors;
		if (net_bReportMiscErrors) {
			CPrintF(TRANS("Warning: invalid modelinstance for OffSetAnimationQueue, entity id: 0x%X\n"),en_ulID);
		}
		return;
	}

	if (iModelInstanceID == ESKA_MASTER_MODEL_INSTANCE || iModelInstanceID == en_pmiModelInstance->GetID()) {
		en_pmiModelInstance->OffSetAnimationQueue(fOffsetTime);
	} else {
		CModelInstance* pmiChild = en_pmiModelInstance->GetChild(iModelInstanceID,TRUE);
		ASSERT (pmiChild != NULL);
		pmiChild->OffSetAnimationQueue(fOffsetTime);
	}
}



void CEntity::ReadSKAQueue(CTStream *istr) {
	ASSERT (en_RenderType == RT_SKAMODEL || en_RenderType == RT_SKAEDITORMODEL);
	istr->ExpectID_t("SKAQ");
	INDEX ctAnimLists;
	(*istr) >> ctAnimLists;
	//_pmiModelInstance->StopAllAnimations(0.0f);
	en_pmiModelInstance->mi_aqAnims.aq_Lists.PopAll();
	for (INDEX iAnimList=0;iAnimList<ctAnimLists;iAnimList++) {
		AnimList& alAnimList = en_pmiModelInstance->mi_aqAnims.aq_Lists.Push();
		alAnimList.al_PlayedAnims.PopAll();
		(*istr) >> alAnimList.al_fFadeTime;
		(*istr) >> alAnimList.al_fStartTime;
		INDEX ctPlayedAnims;
		(*istr) >> ctPlayedAnims;
		for (INDEX iPlayedAnim=0;iPlayedAnim<ctPlayedAnims;iPlayedAnim++) {
			PlayedAnim &plAnim = alAnimList.al_PlayedAnims.Push();
			istr->Read_t(&plAnim,sizeof(PlayedAnim));
			extern CStaticStackArray<INDEX> _aiNetSkaIDConversion;
			plAnim.pa_iAnimID = _aiNetSkaIDConversion[plAnim.pa_iAnimID];
			alAnimList.al_PlayedAnims[iPlayedAnim] = plAnim;
		}
	}
}

void CEntity::ReadSKAQueue(CNetworkMessage& nmMessage) {
	ASSERT (en_RenderType == RT_SKAMODEL || en_RenderType == RT_SKAEDITORMODEL);
	INDEX ctAnimLists;
	nmMessage >> ctAnimLists;
	//_pmiModelInstance->StopAllAnimations(0.0f);
	en_pmiModelInstance->mi_aqAnims.aq_Lists.PopAll();
	for (INDEX iAnimList=0;iAnimList<ctAnimLists;iAnimList++) {
		AnimList& alAnimList = en_pmiModelInstance->mi_aqAnims.aq_Lists.Push();
		alAnimList.al_PlayedAnims.PopAll();
		nmMessage >> alAnimList.al_fFadeTime;
		nmMessage >> alAnimList.al_fStartTime;
		INDEX ctPlayedAnims;
		nmMessage >> ctPlayedAnims;
		for (INDEX iPlayedAnim=0;iPlayedAnim<ctPlayedAnims;iPlayedAnim++) {
			PlayedAnim &plAnim = alAnimList.al_PlayedAnims.Push();
			nmMessage.Read(&plAnim,sizeof(PlayedAnim));
			extern CStaticStackArray<INDEX> _aiNetSkaIDConversion;
			plAnim.pa_iAnimID = _aiNetSkaIDConversion[plAnim.pa_iAnimID];
			alAnimList.al_PlayedAnims[iPlayedAnim] = plAnim;
		}
	}
}


void CEntity::WriteSKAQueue(CTStream *ostr) {
	ASSERT (en_RenderType == RT_SKAMODEL || en_RenderType == RT_SKAEDITORMODEL);
	ostr->WriteID_t("SKAQ");
	INDEX ctAnimLists = en_pmiModelInstance->mi_aqAnims.aq_Lists.Count();
	(*ostr) << ctAnimLists;
	for (INDEX iAnimList=0;iAnimList<ctAnimLists;iAnimList++) {
		AnimList& alAnimList = en_pmiModelInstance->mi_aqAnims.aq_Lists[iAnimList];
		(*ostr) << alAnimList.al_fFadeTime;
		(*ostr) << alAnimList.al_fStartTime;
		INDEX ctPlayedAnims = alAnimList.al_PlayedAnims.Count();
		(*ostr) << ctPlayedAnims;
		for (INDEX iPlayedAnim=0;iPlayedAnim<ctPlayedAnims;iPlayedAnim++) {
			ostr->Write_t(&alAnimList.al_PlayedAnims[iPlayedAnim],sizeof(PlayedAnim));
		}
	}
}

void CEntity::WriteSKAQueue(CNetworkMessage& nmMessage) {
	ASSERT (en_RenderType == RT_SKAMODEL || en_RenderType == RT_SKAEDITORMODEL);  
	INDEX ctAnimLists = en_pmiModelInstance->mi_aqAnims.aq_Lists.Count();
	nmMessage << ctAnimLists;
	for (INDEX iAnimList=0;iAnimList<ctAnimLists;iAnimList++) {
		AnimList& alAnimList = en_pmiModelInstance->mi_aqAnims.aq_Lists[iAnimList];
		nmMessage << alAnimList.al_fFadeTime;
		nmMessage << alAnimList.al_fStartTime;
		INDEX ctPlayedAnims = alAnimList.al_PlayedAnims.Count();
		nmMessage << ctPlayedAnims;
		for (INDEX iPlayedAnim=0;iPlayedAnim<ctPlayedAnims;iPlayedAnim++) {
			nmMessage.Write(&alAnimList.al_PlayedAnims[iPlayedAnim],sizeof(PlayedAnim));
		}
	}
}



/* Set the main texture data for model entity. */
void CEntity::SetModelMainTexture(const CTFileName &fnmTexture)
{
	ASSERT(en_RenderType==RT_MODEL || en_RenderType==RT_EDITORMODEL);
	// try to
	try {
		// load the texture data
		en_pmoModelObject->mo_toTexture.SetData_t(fnmTexture);

	// if failed
	} catch(char *strError) {
		(void)strError;
		DECLARE_CTFILENAME(fnmDefault, "Data\\Textures\\Editor\\Default.tex");
		// try to
		try {
			// load the default model data
			en_pmoModelObject->mo_toTexture.SetData_t(fnmDefault);
		// if failed
		} catch(char *strErrorDefault) {
			FatalError(TRANS("Cannot load default texture '%s':\n%s"),
				(CTString&)fnmDefault, strErrorDefault);
		}
	}
}
void CEntity::SetModelMainTexture(SLONG idTextureComponent)
{
	ASSERT(en_RenderType==RT_MODEL || en_RenderType==RT_EDITORMODEL);
	CEntityComponent *pecTexture = en_pecClass->ComponentForTypeAndID(
		ECT_TEXTURE, idTextureComponent);
	en_pmoModelObject->mo_toTexture.SetData(pecTexture->ec_ptdTexture);
}
/* Get the main texture data for model entity. */
const CTFileName &CEntity::GetModelMainTexture(void)
{
	ASSERT(en_RenderType==RT_MODEL || en_RenderType==RT_EDITORMODEL);
	return en_pmoModelObject->mo_toTexture.GetData()->GetName();
}
/* Start new animation for main texture of model entity. */
void CEntity::StartModelMainTextureAnim(INDEX iNewTextureAnim)
{
	ASSERT(en_RenderType==RT_MODEL || en_RenderType==RT_EDITORMODEL);
	en_pmoModelObject->mo_toTexture.StartAnim(iNewTextureAnim);
}

/* Set the reflection texture data for model entity. */
void CEntity::SetModelReflectionTexture(SLONG idTextureComponent)
{
	ASSERT(en_RenderType==RT_MODEL || en_RenderType==RT_EDITORMODEL);
	CEntityComponent *pecTexture = en_pecClass->ComponentForTypeAndID(
		ECT_TEXTURE, idTextureComponent);
	en_pmoModelObject->mo_toReflection.SetData(pecTexture->ec_ptdTexture);
}
/* Set the specular texture data for model entity. */
void CEntity::SetModelSpecularTexture(SLONG idTextureComponent)
{
	ASSERT(en_RenderType==RT_MODEL || en_RenderType==RT_EDITORMODEL);
	CEntityComponent *pecTexture = en_pecClass->ComponentForTypeAndID(
		ECT_TEXTURE, idTextureComponent);
	en_pmoModelObject->mo_toSpecular.SetData(pecTexture->ec_ptdTexture);
}

/* Add attachment to model */
void CEntity::AddAttachment(INDEX iAttachment, ULONG ulIDModel, ULONG ulIDTexture)
{
	// add attachment
	CModelObject &mo = en_pmoModelObject->AddAttachmentModel(iAttachment)->amo_moModelObject;
	// update model data
	CEntityComponent *pecWeaponModel = ComponentForTypeAndID( ECT_MODEL, ulIDModel);
	mo.SetData(pecWeaponModel->ec_pmdModel);
	// update texture data if different
	CEntityComponent *pecWeaponTexture = ComponentForTypeAndID( ECT_TEXTURE, ulIDTexture);
	mo.SetTextureData(pecWeaponTexture->ec_ptdTexture);
}
void CEntity::AddAttachment(INDEX iAttachment, CTFileName fnModel, CTFileName fnTexture)
{
	if( fnModel == CTString("")) return;
	CModelObject *pmo = GetModelObject();
	ASSERT( pmo != NULL);
	if( pmo == NULL) return;

	CAttachmentModelObject *pamo = pmo->AddAttachmentModel( iAttachment);
	try
	{
		pamo->amo_moModelObject.SetData_t( fnModel);
	}
	catch(char *strError)
	{
		(void) strError;
		pmo->RemoveAttachmentModel( iAttachment);
		return;
	}

	try
	{
		pamo->amo_moModelObject.mo_toTexture.SetData_t( fnTexture);
	}
	catch(char *strError)
	{
		(void) strError;
	}
}
/* Set the reflection texture data for attachment model entity. */
void CEntity::SetModelAttachmentReflectionTexture(INDEX iAttachment, SLONG idTextureComponent)
{
	ASSERT(en_RenderType==RT_MODEL || en_RenderType==RT_EDITORMODEL);
	CModelObject &mo = en_pmoModelObject->GetAttachmentModel(iAttachment)->amo_moModelObject;
	CEntityComponent *pecTexture = en_pecClass->ComponentForTypeAndID(
		ECT_TEXTURE, idTextureComponent);
	mo.mo_toReflection.SetData(pecTexture->ec_ptdTexture);
}
/* Set the specular texture data for attachment model entity. */
void CEntity::SetModelAttachmentSpecularTexture(INDEX iAttachment, SLONG idTextureComponent)
{
	ASSERT(en_RenderType==RT_MODEL || en_RenderType==RT_EDITORMODEL);
	CModelObject &mo = en_pmoModelObject->GetAttachmentModel(iAttachment)->amo_moModelObject;
	CEntityComponent *pecTexture = en_pecClass->ComponentForTypeAndID(
		ECT_TEXTURE, idTextureComponent);
	mo.mo_toSpecular.SetData(pecTexture->ec_ptdTexture);
}

// Get all vertices of model entity in absolute space
void CEntity::GetModelVerticesAbsolute( CStaticStackArray<FLOAT3D> &avVertices, FLOAT fNormalOffset, FLOAT fMipFactor)
{
	TRACKMEM(Mem, "GetModelVertices");
	ASSERT(en_RenderType==RT_MODEL || en_RenderType==RT_EDITORMODEL ||
				 en_RenderType==RT_SKAMODEL || en_RenderType==RT_SKAEDITORMODEL);
	// get placement
	CPlacement3D plPlacement = GetLerpedPlacement();
	// calculate rotation matrix
	FLOATmatrix3D mRotation;
	MakeRotationMatrixFast(mRotation, plPlacement.pl_OrientationAngle);
	if(en_RenderType==RT_MODEL || en_RenderType==RT_EDITORMODEL) {
		en_pmoModelObject->GetModelVertices( avVertices, mRotation, plPlacement.pl_PositionVector, fNormalOffset, fMipFactor);
	} else {
		RM_SetBoneAdjustCallback(&EntityAdjustBonesCallback,this);
		GetModelInstance()->GetModelVertices( avVertices, mRotation, plPlacement.pl_PositionVector, fNormalOffset, fMipFactor);
	}
}

void EntityAdjustBonesCallback(void *pData)
{
	((CEntity*)pData)->AdjustBones();
}
void EntityAdjustShaderParamsCallback(void *pData,INDEX iSurfaceID,CShader *pShader,ShaderParams &spParams)
{
	((CEntity*)pData)->AdjustShaderParams(iSurfaceID,pShader,spParams);
}

// Returns true if bone exists and sets two given vectors as start and end point of specified bone
BOOL CEntity::GetBoneRelPosition(INDEX iBoneID, FLOAT3D &vStartPoint, FLOAT3D &vEndPoint)
{
	ASSERT(en_RenderType==RT_SKAMODEL || en_RenderType==RT_SKAEDITORMODEL);
	RM_SetObjectPlacement(CPlacement3D(FLOAT3D(0.0f,0.0f,0.0f),ANGLE3D(0.0f,0.0f,0.0f)));
	RM_SetBoneAdjustCallback(&EntityAdjustBonesCallback,this);
	return RM_GetBoneRelPosition(*GetModelInstance(),iBoneID,vStartPoint,vEndPoint);
}

// Returns true if bone exists and sets two given vectors as start and end point of specified bone
BOOL CEntity::GetBoneAbsPosition(INDEX iBoneID, FLOAT3D &vStartPoint, FLOAT3D &vEndPoint)
{
	ASSERT(en_RenderType==RT_SKAMODEL || en_RenderType==RT_SKAEDITORMODEL);
	RM_SetObjectPlacement(GetLerpedPlacement());
	RM_SetBoneAdjustCallback(&EntityAdjustBonesCallback,this);
	return RM_GetBoneAbsPosition(*GetModelInstance(),iBoneID,vStartPoint,vEndPoint);
}

// Returns true if bone exists and sets two given vectors as start and end point of specified bone
BOOL CEntity::GetBoneTransformations(INDEX iBoneID, FLOATmatrix3D &mRot, FLOAT3D &vTranslation)
{
	ASSERT(en_RenderType==RT_SKAMODEL || en_RenderType==RT_SKAEDITORMODEL);
	// RM_SetObjectPlacement(CPlacement3D(FLOAT3D(0.0f,0.0f,0.0f),ANGLE3D(0.0f,0.0f,0.0f)));
	RM_SetObjectPlacement(GetLerpedPlacement());
	RM_SetBoneAdjustCallback(&EntityAdjustBonesCallback,this);
	Matrix12 mTransform;
	if(RM_GetBoneTransformations(*GetModelInstance(), iBoneID, &mTransform)) {
		Matrix12ToMatrixVector(mRot,vTranslation,mTransform);
		return TRUE;
	} else {
		return FALSE;
	}
}

// Callback function for aditional bone adjustment
void CEntity::AdjustBones()
{
}

// Callback function for aditional shader params adjustment
void CEntity::AdjustShaderParams(INDEX iSurfaceID,CShader *pShader,ShaderParams &spParams)
{
}

// precache given component
void CEntity::PrecacheModel(SLONG slID)
{
	en_pecClass->ec_pdecDLLClass->PrecacheModel(slID);
}
void CEntity::PrecacheTexture(SLONG slID)
{
	en_pecClass->ec_pdecDLLClass->PrecacheTexture(slID);
}
void CEntity::PrecacheSound(SLONG slID)
{
	en_pecClass->ec_pdecDLLClass->PrecacheSound(slID);
}
void CEntity::PrecacheClass(SLONG slID, INDEX iUser /* = -1*/)
{
	en_pecClass->ec_pdecDLLClass->PrecacheClass(slID, iUser);
}
void CEntity::PrecacheSkaModel(SLONG slID)
{
	en_pecClass->ec_pdecDLLClass->PrecacheSkaModel(slID);
}
CAutoPrecacheSound::CAutoPrecacheSound()
{
	apc_psd = NULL;
}
CAutoPrecacheSound::~CAutoPrecacheSound()
{
	if (apc_psd!=NULL) {
		_pSoundStock->Release(apc_psd);
	}
}

void CAutoPrecacheSound::Precache(const CTFileName &fnm)
{
	CTFileName fnmActual;
	// very quick fix!!
	if(fnm!="" && fnm.FileExt()!=".wav") {
		// expand encoded music files
		ExpandFilePath(EFP_READ,fnm,fnmActual);
	} else {
		fnmActual = fnm;
	}

	if (apc_psd!=NULL) {
		_pSoundStock->Release(apc_psd);
	}
	try {
		if (fnmActual!="") {
			apc_psd = _pSoundStock->Obtain_t(fnmActual);
		}
	} catch (char *strError) {
		CPrintF("%s\n", strError);
	}
}

CAutoPrecacheModel::CAutoPrecacheModel()
{
	apc_pmd = NULL;
}
CAutoPrecacheModel::~CAutoPrecacheModel()
{
	if (apc_pmd!=NULL) {
		_pModelStock->Release(apc_pmd);
	}
}

void CAutoPrecacheModel::Precache(const CTFileName &fnm)
{
	if (apc_pmd!=NULL) {
		_pModelStock->Release(apc_pmd);
	}
	try {
		if (fnm!="") {
			apc_pmd = _pModelStock->Obtain_t(fnm);
		}
	} catch (char *strError) {
		CPrintF("%s\n", strError);
	}
}

CAutoPrecacheTexture::CAutoPrecacheTexture()
{
	apc_ptd = NULL;
}
CAutoPrecacheTexture::~CAutoPrecacheTexture()
{
	if (apc_ptd!=NULL) {
		_pTextureStock->Release(apc_ptd);
	}
}

void CAutoPrecacheTexture::Precache(const CTFileName &fnm)
{
	if (apc_ptd!=NULL) {
		_pTextureStock->Release(apc_ptd);
	}
	try {
		if (fnm!="") {
			apc_ptd = _pTextureStock->Obtain_t(fnm);
		}
	} catch (char *strError) {
		CPrintF("%s\n", strError);
	}
}

/* Get a filename for a component of given id. */
const CTFileName &CEntity::FileNameForComponent(SLONG slType, SLONG slID)
{
	// find the component
	CEntityComponent *pec = en_pecClass->ComponentForTypeAndID(
		(EntityComponentType)slType, slID);
	// the component must exist
	ASSERT(pec!=NULL);
	// get its name
	return pec->ec_fnmComponent;
}

// Get data for a texture component
CTextureData *CEntity::GetTextureDataForComponent(SLONG slID)
{
	CEntityComponent *pec = ComponentForTypeAndID( ECT_TEXTURE, slID);
	if (pec!=NULL) {
		return pec->ec_ptdTexture;
	} else {
		return NULL;
	}
}

// Get data for a model component
CModelData *CEntity::GetModelDataForComponent(SLONG slID)
{
	CEntityComponent *pec = ComponentForTypeAndID( ECT_MODEL, slID);
	if (pec!=NULL) {
		return pec->ec_pmdModel;
	} else {
		return NULL;
	}
}

CModelInstance *CEntity::GetModelInstanceForComponent(SLONG slID)
{
	CEntityComponent *pec = ComponentForTypeAndID( ECT_SKAMODEL, slID);
	ASSERT(pec!=NULL);
	ASSERT(pec->ec_pmisModelInstance!=NULL);
	ASSERT(pec->ec_pmisModelInstance->mis_pmiModelInstance!=NULL);
	if (pec!=NULL) {
		return pec->ec_pmisModelInstance->mis_pmiModelInstance;
	} else {
		return NULL;
	}
}

/* Remove attachment from model */
void CEntity::RemoveAttachment(INDEX iAttachment)
{
	// remove attachment
	en_pmoModelObject->RemoveAttachmentModel(iAttachment);
}

/* Initialize last positions structure for particles. */
CLastPositions *CEntity::GetLastPositions(INDEX ctPositions)
{
	TIME tmNow = _pTimer->CurrentTick();
	if (en_plpLastPositions==NULL) {
		en_plpLastPositions = new CLastPositions;
		en_plpLastPositions->lp_avPositions.New(ctPositions);
		en_plpLastPositions->lp_ctUsed = 0;
		en_plpLastPositions->lp_iLast = 0;
		en_plpLastPositions->lp_tmLastAdded = tmNow;
		const FLOAT3D &vNow = GetPlacement().pl_PositionVector;
		for(INDEX iPos = 0; iPos<ctPositions; iPos++) {
			en_plpLastPositions->lp_avPositions[iPos] = vNow;
		}
	}

	while(en_plpLastPositions->lp_tmLastAdded<tmNow) {
		en_plpLastPositions->AddPosition(en_plpLastPositions->GetPosition(0));
	}

	return en_plpLastPositions;
}


/* Get absolute position of point on entity given relative to its size. */
void CEntity::GetEntityPointRatio(const FLOAT3D &vRatio, FLOAT3D &vAbsPoint, BOOL bLerped)
{

	if (en_RenderType!=RT_MODEL && en_RenderType!=RT_EDITORMODEL &&
			en_RenderType!=RT_SKAMODEL && en_RenderType!=RT_SKAEDITORMODEL && 
			en_RenderType!=RT_BRUSH)  {
		//ASSERT(FALSE); //CrateBus caused this ASSERT when it blew up
		vAbsPoint = en_plPlacement.pl_PositionVector;
		return;
	}

	FLOAT3D vMin, vMax;

	if (en_RenderType==RT_BRUSH)
	{
		CBrushMip *pbmMip = en_pbrBrush->GetFirstMip();
		vMin = pbmMip->bm_boxBoundingBox.Min();
		vMax = pbmMip->bm_boxBoundingBox.Max();
		FLOAT3D vOff = vMax-vMin;
		vOff(1) *= vRatio(1);
		vOff(2) *= vRatio(2);
		vOff(3) *= vRatio(3);
		vAbsPoint = vMin+vOff;
	}
	else
	{
		INDEX iEq;
		FLOATaabbox3D box;
		GetCollisionBoxParameters(GetCollisionBoxIndex(), box, iEq);
		vMin = box.Min();
		vMax = box.Max();

		FLOAT3D vOff = vMax-vMin;
		vOff(1) *= vRatio(1);
		vOff(2) *= vRatio(2);
		vOff(3) *= vRatio(3);
		FLOAT3D vPos = vMin+vOff;
		if( bLerped)
		{
			CPlacement3D plLerped=GetLerpedPlacement();
			FLOATmatrix3D mRot;
			MakeRotationMatrixFast(mRot, plLerped.pl_OrientationAngle);
			vAbsPoint=plLerped.pl_PositionVector+vPos*mRot;
		}
		else
		{
			vAbsPoint = en_plPlacement.pl_PositionVector+vPos*en_mRotation;
		}
	}
}

/* Get absolute position of point on entity given in meters. */
void CEntity::GetEntityPointFixed(const FLOAT3D &vFixed, FLOAT3D &vAbsPoint)
{
	vAbsPoint = en_plPlacement.pl_PositionVector+vFixed*en_mRotation;
}
/* Get sector that given point is in - point must be inside this entity. */
CBrushSector *CEntity::GetSectorFromPoint(const FLOAT3D &vPointAbs)
{
	// for each sector around entity
	{FOREACHSRCOFDST(en_rdSectors, CBrushSector, bsc_rsEntities, pbsc)
		// if point is in this sector
		if( pbsc->bsc_bspBSPTree.TestSphere(FLOATtoDOUBLE(vPointAbs), 0.01)>=0) {
			// return that
			return pbsc;
		}
	ENDFOR;}
	return NULL;
}

/* Model change notify */
void CEntity::ModelChangeNotify(void)
{
	// if this is ska model
	if(en_RenderType == RT_SKAMODEL || en_RenderType == RT_SKAEDITORMODEL) {
		if(GetModelInstance()==NULL) {
			return;
		}

	// this is old model
	} else {
		if (en_pmoModelObject==NULL || en_pmoModelObject->GetData()==NULL) {
			return;
		}
	}

//안태훈 수정 시작	//(Add & Modify SSSE Effect)(0.1)
/*
	if(en_RenderType == RT_SKAMODEL || en_RenderType == RT_SKAEDITORMODEL)
	{
		if(GetModelInstance() != NULL)
		{
			GetModelInstance()->RefreshTagManager();
		}
	}
*/
//안태훈 수정 끝	//(Add & Modify SSSE Effect)(0.1)
	UpdateSpatialRange();
	FindCollisionInfo();
}

void CEntity::TerrainChangeNotify(void)
{
//  GetTerrain()->RemoveLayer(0,FALSE);
//  GetTerrain()->AddDefaultLayer_t();
	TR_RegenerateAll(GetTerrain());
	UpdateSpatialRange();
}

// map world polygon to/from indices
CBrushPolygon *CEntity::GetWorldPolygonPointer(INDEX ibpo)
{
	if (ibpo<0 || ibpo>=en_pwoWorld->wo_baBrushes.ba_apbpo.Count()) {
		return NULL;
	} else {
		return en_pwoWorld->wo_baBrushes.ba_apbpo[ibpo];
	}
}
INDEX CEntity::GetWorldPolygonIndex(CBrushPolygon *pbpo)
{
	if (pbpo==NULL) {
		return -1;
	} else {
		return pbpo->bpo_iInWorld;
	}
}

/////////////////////////////////////////////////////////////////////
// Sound functions
void CEntity::PlaySound(CSoundObject &so, SLONG idSoundComponent, SLONG slPlayType, BOOL bNetwork /* = TRUE */)
{
	CEntityComponent *pecSound = en_pecClass->ComponentForTypeAndID(ECT_SOUND, idSoundComponent);

	if (_pNetwork->IsServer() && bNetwork && IsSentOverNet()) { 
		ULONG ulComponentOffset = ((UBYTE*)&so) - ((UBYTE*)so.so_penEntity);
		UBYTE ubThis = 1;
		SLONG slDataSize = sizeof(ulComponentOffset) + sizeof(idSoundComponent) + sizeof(slPlayType) + sizeof(ubThis);
		UBYTE* pubData = _abSoundData;    

		if (this != so.so_penEntity) {
			slDataSize += sizeof(so.so_penEntity->en_ulID);
			ubThis = 0;
			*(UBYTE*)pubData = ubThis; pubData += sizeof(ubThis);  
			*(ULONG*)pubData = so.so_penEntity->en_ulID; pubData += sizeof(so.so_penEntity->en_ulID);  
		} else {
			*(UBYTE*)pubData = ubThis; pubData += sizeof(ubThis);
		}

		*(ULONG*)pubData = ulComponentOffset; pubData += sizeof(ulComponentOffset);
		*(SLONG*)pubData = idSoundComponent; pubData += sizeof(idSoundComponent);
		*(SLONG*)pubData = slPlayType; pubData += sizeof(slPlayType);

		extern CEntityMessage _emEntityMessage;
		_emEntityMessage.WriteSound(en_ulID,EM_SOUND_PLAY,_abSoundData,slDataSize);
		_pNetwork->ga_srvServer.SendMessage(_emEntityMessage);
	}

	//so.Stop();
	so.Play(pecSound->ec_psdSound, slPlayType);

}

double CEntity::GetSoundLength(SLONG idSoundComponent)
{
	CEntityComponent *pecSound = en_pecClass->ComponentForTypeAndID(ECT_SOUND, idSoundComponent);
	return pecSound->ec_psdSound->GetSecondsLength();
}

void CEntity::PlaySound(CSoundObject &so, const CTFileName &fnmSound, SLONG slPlayType)
{
	// try to
	try {
		// load the sound data
		//so.Stop();
		so.Play_t(fnmSound, slPlayType);
	// if failed
	} catch(char *strError) {
		(void)strError;
		DECLARE_CTFILENAME(fnmDefault, "Data\\Sounds\\Default.wav");
		// try to
		try {
			// load the default sound data
			so.Play_t(fnmDefault, slPlayType);
		// if failed
		} catch(char *strErrorDefault) {
			FatalError(TRANS("Cannot load default sound '%s':\n%s"),
				(CTString&)fnmDefault, strErrorDefault);
		}
	}
}

/////////////////////////////////////////////////////////////////////
/*
 * Apply some damage directly to one entity.
 */
void CEntity::InflictDirectDamage(CEntity *penToDamage, CEntity *penInflictor, enum DamageType dmtType,
	FLOAT fDamageAmmount, const FLOAT3D &vHitPoint, const FLOAT3D &vDirection)
{


	// if significant damage
	if (fDamageAmmount>0) {
		penToDamage->ReceiveDamage(penInflictor, dmtType, fDamageAmmount, vHitPoint, vDirection);
	}
}

// find intensity of current entity at given distance
static inline FLOAT IntensityAtDistance(
	FLOAT fDamageAmmount, FLOAT fHotSpotRange, FLOAT fFallOffRange, FLOAT fDistance)
{
	// if further than fall-off range
	if (fDistance>fFallOffRange) {
		// intensity is zero
		return 0;
	// if closer than hot-spot range
	} else if (fDistance<fHotSpotRange) {
		// intensity is maximum
		return fDamageAmmount;
	// if between fall-off and hot-spot range
	} else {
		// interpolate
		return fDamageAmmount*(fFallOffRange-fDistance)/(fFallOffRange-fHotSpotRange);
	}
}

//강동민 수정 시작 싱글 던젼 작업	07.29
// check if a range damage can hit given model entity
BOOL CEntity::CheckModelRangeDamage(
//강동민 수정 끝 싱글 던젼 작업		07.29
	CEntity &en, const FLOAT3D &vCenter, FLOAT &fMinD, FLOAT3D &vHitPos)
{
	CCollisionInfo *pci = en.en_pciCollisionInfo;
	if (pci==NULL) 
	{
		return FALSE;
	}

	// create 3 points along entity
	const FLOATmatrix3D &mR = en.en_mRotation;
	const FLOAT3D &vO = en.en_plPlacement.pl_PositionVector;

	FLOAT3D avPoints[3];

	// collision spheres of the model
	INDEX ctSpheres = pci->ci_absSpheres.Count();
	if (ctSpheres<1)
	{
		return FALSE;
	}

	// 상대적인 중점을 얻음.
	avPoints[1] = pci->ci_absSpheres[ctSpheres-1].ms_vCenter * mR + vO;
	avPoints[2] = pci->ci_absSpheres[0].ms_vCenter * mR + vO;

	// 두 Collision Sphere의 평균값을 얻은 후.
	avPoints[0] = (avPoints[1] + avPoints[2]) * 0.5f;

	// check if any point can be hit
	BOOL bCanHit = FALSE;
	for(INDEX i = 0; i < 3; i++) 
	{
		CCastRay crRay( &en, avPoints[i], vCenter);
		crRay.cr_ttHitModels = CCastRay::TT_NONE;     // only brushes block the damage
		crRay.cr_bHitTranslucentPortals = FALSE;
		crRay.cr_bPhysical = TRUE;
		en.en_pwoWorld->CastRay(crRay);
		if (crRay.cr_penHit==NULL) 
		{
			bCanHit = TRUE;
			break;
		}
	}

	// if none can be hit
	if (!bCanHit) 
	{
		// skip this entity
		return FALSE;
	}

	// find minimum distance
	fMinD = UpperLimit(0.0f);
	vHitPos = vO;

	// for each sphere
	// 각각의 Collision Sphere에 대해서...
	FOREACHINSTATICARRAY(pci->ci_absSpheres, CMovingSphere, itms) 
	{
		// project it
		// 상대적인 중점 좌표로 변환.
		itms->ms_vRelativeCenter0 = itms->ms_vCenter * en.en_mRotation + vO;

		// Get Distance
		FLOAT fD = (itms->ms_vRelativeCenter0-vCenter).Length()-itms->ms_fR;
		if (fD<fMinD) 
		{
			fMinD = Min(fD, fMinD);
			vHitPos = itms->ms_vRelativeCenter0;
		}
	}
	if (fMinD<0) 
	{
		fMinD = 0;
	}
	return TRUE;
}

// check if a range damage can hit given brush entity
static BOOL CheckBrushRangeDamage(
	CEntity &en, const FLOAT3D &vCenter, FLOAT &fMinD, FLOAT3D &vHitPos)
{
	// don't actually check for brushes, doesn't have to be too exact
	const FLOAT3D &vO = en.en_plPlacement.pl_PositionVector;
	fMinD = (vO-vCenter).Length();
	vHitPos = vO;
	return TRUE;
}

//강동민 수정 시작 싱글 던젼 작업	07.29
/* Get relative angles from direction angles. */
ANGLE CEntity::GetRelativeHeading(const FLOAT3D &vDirection) 
{
	// get front component of vector
	FLOAT fFront = 
		-vDirection(1)*en_mRotation(1,3)
		-vDirection(2)*en_mRotation(2,3)
		-vDirection(3)*en_mRotation(3,3);
	// get left component of vector
	FLOAT fLeft = 
		-vDirection(1)*en_mRotation(1,1)
		-vDirection(2)*en_mRotation(2,1)
		-vDirection(3)*en_mRotation(3,1);
	// relative heading is arctan of angle between front and left
	return ATan2(fLeft, fFront);
}
//강동민 수정 끝 싱글 던젼 작업		07.29

/* Apply some damage to all entities in some range. */
void CEntity::InflictRangeDamage(CEntity *penInflictor, enum DamageType dmtType,
	FLOAT fDamageAmmount, const FLOAT3D &vCenter, FLOAT fHotSpotRange, FLOAT fFallOffRange)
{
	// find entities in the range
	CDynamicContainer<CEntity> cenInRange;
	FindEntitiesInRange(FLOATaabbox3D(vCenter, fFallOffRange), cenInRange, TRUE);

	// for each entity in container
	FOREACHINDYNAMICCONTAINER(cenInRange, CEntity, iten) {
		CEntity &en = *iten;

		// if can be hit
		FLOAT3D vHitPos;
		FLOAT fMinD;
		if (
			(en.en_RenderType==RT_MODEL || en.en_RenderType==RT_EDITORMODEL || 
				en.en_RenderType==RT_SKAMODEL || en.en_RenderType==RT_SKAEDITORMODEL )&&
			 CheckModelRangeDamage(en, vCenter, fMinD, vHitPos) ||
			(en.en_RenderType==RT_BRUSH)&&
			 CheckBrushRangeDamage(en, vCenter, fMinD, vHitPos)) {

			// find damage ammount
			FLOAT fAmmount = IntensityAtDistance(fDamageAmmount, fHotSpotRange, fFallOffRange, fMinD);

			// if significant
			if (fAmmount>0) {
				// inflict damage to it
				en.ReceiveDamage(penInflictor, dmtType, fAmmount, vHitPos, (vHitPos-vCenter).Normalize());
			}
		}
	}
}

/* Apply some damage to all entities in a box (this doesn't test for obstacles). */
void CEntity::InflictBoxDamage(CEntity *penInflictor, enum DamageType dmtType,
	FLOAT fDamageAmmount, const FLOATaabbox3D &box)
{


	// find entities in the range
	CDynamicContainer<CEntity> cenInRange;
	FindEntitiesInRange(box, cenInRange, TRUE);

	// for each entity in container
	FOREACHINDYNAMICCONTAINER(cenInRange, CEntity, iten) {
		CEntity &en = *iten;
		//ASSERT(en.en_pciCollisionInfo!=NULL);  // assured by FindEntitiesInRange()
		if (en.en_pciCollisionInfo==NULL) {
			continue;
		}
		CCollisionInfo *pci = en.en_pciCollisionInfo;
		// if significant damage
		if (fDamageAmmount>0) {
			// inflict damage to it
			en.ReceiveDamage(penInflictor, dmtType,
				fDamageAmmount, box.Center(), 
				(box.Center()-en.GetPlacement().pl_PositionVector).Normalize());
		}
	}
}

// notify engine that gravity defined by this entity has changed
void CEntity::NotifyGravityChanged(void)
{
	// for each zoning brush in the world of this entity
	FOREACHINDYNAMICCONTAINER(en_pwoWorld->wo_cenEntities, CEntity, iten) {
		CEntity *penBrush = &*iten;
		if (iten->en_RenderType != CEntity::RT_BRUSH || !(iten->en_ulFlags&ENF_ZONING)) {
			continue;
		}
		CBrush3D *pbr = penBrush->en_pbrBrush;
		// get first brush mip
		CBrushMip *pbm = pbr->GetFirstMip();
		// for each sector in the brush mip
		{FOREACHINDYNAMICARRAY(pbm->bm_abscSectors, CBrushSector, itbsc) {
			// if controlled by this entity
			if ( penBrush->GetForceController(itbsc->GetForceType()) == this ) {
				// for each entity in the sector
				{FOREACHDSTOFSRC(itbsc->bsc_rsEntities, CEntity, en_rdSectors, pen) {
					// if movable
					if (pen->en_ulPhysicsFlags&EPF_MOVABLE) {
						CMovableEntity *pmen = (CMovableEntity*)pen;
						// add to movers
						pmen->AddToMovers();
					}
				ENDFOR}}
			}
		}}
	}
}

// notify engine that collision of this entity was changed
void CEntity::NotifyCollisionChanged(void)
{
	if (en_pciCollisionInfo==NULL) {
		return;
	}

	// find colliding entities near this one
	static CStaticStackArray<CEntity*> apenNearEntities;
	en_pwoWorld->FindEntitiesNearBox(en_pciCollisionInfo->ci_boxCurrent, apenNearEntities);
	
	// for each of the found entities
	{for(INDEX ienFound=0; ienFound<apenNearEntities.Count(); ienFound++) {
		CEntity &enToNear = *apenNearEntities[ienFound];

		// if movable
		if (enToNear.en_ulPhysicsFlags&EPF_MOVABLE) {
			// add to movers
			((CMovableEntity*)&enToNear)->AddToMovers();
		}
	}}
	apenNearEntities.PopAll();
}

// apply some damage to the entity (see event EDamage for more info)
void CEntity::ReceiveDamage(CEntity *penInflictor, enum DamageType dmtType,
	FLOAT fDamageAmmount, const FLOAT3D &vHitPoint, const FLOAT3D &vDirection)
{
	CEntityPointer penThis = this;  // keep this entity alive during this function
	// just throw an event that you are damaged (base entities don't really have health)
	EDamage eDamage;
	eDamage.penInflictor = penInflictor;
	eDamage.vDirection   = vDirection;
	eDamage.vHitPoint    = vHitPoint;
	eDamage.fAmount      = fDamageAmmount;
	eDamage.dmtType      = dmtType;
	SendEvent(eDamage);
}

/* Receive item through event */
/*
BOOL CEntity::ReceiveItem(const CEntityEvent &ee)
{
	return FALSE;
}
*/

/* Get entity info */
void *CEntity::GetEntityInfo(void)
{
	return NULL;
};
/* Fill in entity statistics - for AI purposes only */
BOOL CEntity::FillEntityStatistics(struct EntityStats *pes)
{
	return FALSE;
}

/////////////////////////////////////////////////////////////////////
// Overrides from CSerial

/*
 * Read from stream.
 */
void CEntity::Read_t( CTStream *istr, BOOL bNetwork) // throw char *
{
//강동민 수정 시작 테스트 클라이언트 작업	06.30
	if(istr->PeekID_t()==CChunkID("ENT5")) { // entity v5
		istr->ExpectID_t("ENT5");
		ULONG ulID;
		SLONG slSize;
		(*istr)>>ulID>>slSize;    // skip id and size
		(*istr)>>(ULONG &)en_RenderType
					 >>en_ulPhysicsFlags
					 >>en_ulCollisionFlags
					 >>en_ulSpawnFlags
					 >>en_ulFlags
					 >>en_ulExtraFlags1
					 >>en_ulExtraFlags2;
		(*istr).Read_t(&en_mRotation, sizeof(en_mRotation));
	} else
//강동민 수정 끝 테스트 클라이언트 작업		06.30
	// read base class data from stream
	if (istr->PeekID_t()==CChunkID("ENT4")) { // entity v4
		istr->ExpectID_t("ENT4");
		ULONG ulID;
		SLONG slSize;
		(*istr)>>ulID>>slSize;    // skip id and size
		(*istr)>>(ULONG &)en_RenderType
					 >>en_ulPhysicsFlags
					 >>en_ulCollisionFlags
					 >>en_ulSpawnFlags
					 >>en_ulFlags;
		(*istr).Read_t(&en_mRotation, sizeof(en_mRotation));
	} else if (istr->PeekID_t()==CChunkID("ENT3")) { // entity v3
		istr->ExpectID_t("ENT3");
		(*istr)>>(ULONG &)en_RenderType
					 >>en_ulPhysicsFlags
					 >>en_ulCollisionFlags
					 >>en_ulSpawnFlags
					 >>en_ulFlags;
		(*istr).Read_t(&en_mRotation, sizeof(en_mRotation));
	} else if (istr->PeekID_t()==CChunkID("ENT2")) { // entity v2
		istr->ExpectID_t("ENT2");
		(*istr)>>(ULONG &)en_RenderType
					 >>en_ulPhysicsFlags
					 >>en_ulCollisionFlags
					 >>en_ulSpawnFlags
					 >>en_ulFlags;
	} else {
		(*istr)>>(ULONG &)en_RenderType
					 >>en_ulPhysicsFlags
					 >>en_ulCollisionFlags
					 >>en_ulFlags;
	}

	// clear flags for selection and caching info
	en_ulFlags &= ~(ENF_SELECTED|ENF_INRENDERING|ENF_VALIDSHADINGINFO);
	en_psiShadingInfo = NULL;
	en_pciCollisionInfo = NULL;

	// if this is a brush
	if ( en_RenderType == RT_BRUSH || en_RenderType == RT_FIELDBRUSH) {
		// read brush index in world's brush archive
		INDEX iBrush;
		(*istr)>>iBrush;
		en_pbrBrush = &en_pwoWorld->wo_baBrushes.ba_abrBrushes[iBrush];
		en_pbrBrush->br_penEntity = this;
		en_ulFlags &= ~ENF_INVISIBLE;  // this will be known at render time
	// if this is a terrain
	} else if (en_RenderType == RT_TERRAIN) {
		// read terrain index in world's terrain archive
		INDEX iTerrain;
		(*istr)>>iTerrain;
		en_ptrTerrain = &en_pwoWorld->wo_taTerrains.ta_atrTerrains[iTerrain];
		en_ptrTerrain->tr_penEntity = this;
		// force terrain regeneration (regenerate tiles on next render)
		extern BOOL _bWorldEditorApp;
		if( _bWorldEditorApp )
			TR_RegenerateAll(en_ptrTerrain);
	// if this is a model
	} else if ( en_RenderType == RT_MODEL || en_RenderType == RT_EDITORMODEL ) {
		// create a new model object
		en_pmoModelObject = new CModelObject;
		en_psiShadingInfo = new CShadingInfo;
		en_ulFlags &= ~ENF_VALIDSHADINGINFO;

		// read model
		ReadModelObject_t(*istr, *en_pmoModelObject);
	// if this is a ska model
	} else if( en_RenderType == RT_SKAMODEL || en_RenderType == RT_SKAEDITORMODEL) {
		en_pmiModelInstance = CreateModelInstance("");
		en_psiShadingInfo = new CShadingInfo;
		en_ulFlags &= ~ENF_VALIDSHADINGINFO;

		ReadModelInstance_t(*istr, *GetModelInstance());
	// if this is a void
	} else if (en_RenderType == RT_VOID) {
		en_pbrBrush = NULL;
	}

	// if the entity has a parent
	if (istr->PeekID_t()==CChunkID("PART")) { // parent
		// if it already has a parent, forget it
		if (en_lnInParent.IsLinked()) {
			en_lnInParent.Remove();
		}
		// read the parent pointer and relative offset
		istr->ExpectID_t("PART");
		INDEX iParent;
		*istr>>iParent;
		extern BOOL _bReadEntitiesByID;
		if (_bReadEntitiesByID) {
			en_penParent = en_pwoWorld->EntityFromID(iParent);
		} else {
			en_penParent = en_pwoWorld->wo_cenAllEntities.Pointer(iParent);
		}
		*istr>>en_plRelativeToParent;
		// link to parent
		en_penParent->en_lhChildren.AddTail(en_lnInParent);
	}

	// read the derived class properties from stream
	ReadProperties_t(*istr,bNetwork);

	// if it is a light source
	{CLightSource *pls = GetLightSource();
	if (pls!=NULL) {
		// read the light source layer list
		pls->ls_penEntity = this;
		pls->Read_t(istr,bNetwork);
	}}

	// if it is a field brush
	CFieldSettings *pfs = GetFieldSettings();
	if (pfs!=NULL) {
		// remember its field settings
		ASSERT(en_RenderType == RT_FIELDBRUSH);
		en_pbrBrush->br_pfsFieldSettings = pfs;
	}

	SetFlagOff(ENF_PLACCHANGED);  

//안태훈 수정 시작	//(For Performance)(0.2)
	if(GetLightSource()) this->en_pwoWorld->m_vectorLights.push_back(this);
//안태훈 수정 끝	//(For Performance)(0.2)
}

/*
 * Write to stream.
 */
void CEntity::Write_t( CTStream *ostr, BOOL bNetwork) // throw char *
{


//강동민 수정 시작 테스트 클라이언트 작업	06.30
	ostr->WriteID_t("ENT5");
	SLONG slSize = 0;
	(*ostr)<<en_ulID<<slSize;    // save id and keep space for size
	(*ostr)<<(ULONG &)en_RenderType
				 <<en_ulPhysicsFlags
				 <<en_ulCollisionFlags
				 <<en_ulSpawnFlags
				 <<en_ulFlags
				 <<en_ulExtraFlags1
				 <<en_ulExtraFlags2;
	// write base class data to stream
	// 원본.
	/*
	ostr->WriteID_t("ENT4");
	SLONG slSize = 0;
	(*ostr)<<en_ulID<<slSize;    // save id and keep space for size
	(*ostr)<<(ULONG &)en_RenderType
				 <<en_ulPhysicsFlags
				 <<en_ulCollisionFlags
				 <<en_ulSpawnFlags
				 <<en_ulFlags;
				 */
//강동민 수정 끝 테스트 클라이언트 작업		06.30
	(*ostr).Write_t(&en_mRotation, sizeof(en_mRotation));
	// if this is a brush
	if ( en_RenderType == RT_BRUSH || en_RenderType == RT_FIELDBRUSH) {
		// write brush index in world's brush archive
		(*ostr)<<en_pwoWorld->wo_baBrushes.ba_abrBrushes.Index(en_pbrBrush);
	// if this is a terrain
	} else if ( en_RenderType == RT_TERRAIN) {
		// write brush index in world's brush archive
		(*ostr)<<en_pwoWorld->wo_taTerrains.ta_atrTerrains.Index(en_ptrTerrain);
	// if this is a model
	} else if ( en_RenderType == RT_MODEL || en_RenderType == RT_EDITORMODEL) {
		// write model
		WriteModelObject_t(*ostr, *en_pmoModelObject);
	// if this is ska model
	} else if ( en_RenderType == RT_SKAMODEL || en_RenderType == RT_SKAEDITORMODEL) {
		// write ska model
		WriteModelInstance_t(*ostr, *GetModelInstance());
	// if this is a void
	} else if (en_RenderType == RT_VOID) {
		NOTHING;
	}

	// if the entity has a parent
	if (en_penParent!=NULL) {
		// write the parent pointer and relative offset
		ostr->WriteID_t("PART"); // parent
		INDEX iParent = en_penParent->en_ulID;
		*ostr<<iParent;
		*ostr<<en_plRelativeToParent;
	}

	// write the derived class properties to stream
	WriteProperties_t(*ostr,bNetwork);
	// if it is a light source
	{CLightSource *pls = GetLightSource();
	if (pls!=NULL) {
		// read the light source layer list
		pls->Write_t(ostr,bNetwork);
	}}
}


/* Precache components that might be needed. */
void CEntity::Precache(void)
{
	NOTHING;
}



// get a pseudo-random number (safe for network gaming)
ULONG CEntity::IRnd(void) 
{
	return ((_pNetwork->ga_sesSessionState.Rnd()>>(31-16))&0xFFFF);
}


FLOAT CEntity::FRnd(void)
{
	return ((_pNetwork->ga_sesSessionState.Rnd()>>(31-24))&0xFFFFFF)/FLOAT(0xFFFFFF);
}



// returns ammount of memory used by entity
SLONG CEntity::GetUsedMemory(void)
{
	// initial size
	SLONG slUsedMemory = sizeof(CEntity);

	// add relations
	slUsedMemory += en_rdSectors.Count() * sizeof(CRelationLnk);

	// add allocated memory for model type (if any)
	switch( en_RenderType) {
	case CEntity::RT_MODEL:
	case CEntity::RT_EDITORMODEL:
		slUsedMemory += en_pmoModelObject->GetUsedMemory();
		break;
	case CEntity::RT_SKAMODEL:
	case CEntity::RT_SKAEDITORMODEL:
		slUsedMemory += en_pmiModelInstance->GetUsedMemory();
	default:
		break;
	}

	// add shading info (if any)
	if( en_psiShadingInfo !=NULL) {
		slUsedMemory += sizeof(CShadingInfo);
	}
	// add collision info (if any)
	if( en_pciCollisionInfo!=NULL) {
		slUsedMemory += sizeof(CCollisionInfo) + (en_pciCollisionInfo->ci_absSpheres.sa_Count * sizeof(CMovingSphere));
	}
	// add last positions memory (if any)
	if( en_plpLastPositions!=NULL) {
		slUsedMemory += sizeof(CLastPositions) + (en_plpLastPositions->lp_avPositions.sa_Count * sizeof(FLOAT3D));
	}

	// done
	return slUsedMemory;
}



/* Get pointer to entity property from its packed identifier. */
class CEntityProperty *CEntity::PropertyForTypeAndID(ULONG ulType, ULONG ulID)
{
	return en_pecClass->PropertyForTypeAndID(ulType, ulID);
}

/* Get pointer to entity component from its packed identifier. */
class CEntityComponent *CEntity::ComponentForTypeAndID(ULONG ulType, ULONG ulID)
{
	return en_pecClass->ComponentForTypeAndID((enum EntityComponentType)ulType, ulID);
}

/* Get pointer to entity property from its name. */
class CEntityProperty *CEntity::PropertyForName(const CTString &strPropertyName)
{
	return en_pecClass->PropertyForName(strPropertyName);
}
 
/* Create a new entity of given class in this world. */
CEntity *CEntity::CreateEntity(const CPlacement3D &plPlacement, SLONG idClass, ULONG ulEntityID, BOOL bNetwork)
{
	CEntityComponent *pecClassComponent = en_pecClass->ComponentForTypeAndID(
		ECT_CLASS, idClass);
	return en_pwoWorld->CreateEntity(plPlacement, pecClassComponent->ec_pecEntityClass,ulEntityID,bNetwork);
}



/////////////////////////////////////////////////////////////////////
// CLiveEntity

/*
 * Constructor.
 */
CLiveEntity::CLiveEntity(void)
{
	en_fHealth = 0;
}

/* Copy entity from another entity of same class. */
void CLiveEntity::Copy(CEntity &enOther, ULONG ulFlags)
{
	CEntity::Copy(enOther, ulFlags);
	CLiveEntity *plenOther = (CLiveEntity *)(&enOther);
	en_fHealth = plenOther->en_fHealth;
}
/* Read from stream. */
void CLiveEntity::Read_t( CTStream *istr, BOOL bNetwork) // throw char *
{
	CEntity::Read_t(istr,bNetwork);
	(*istr)>>en_fHealth;
}
/* Write to stream. */
void CLiveEntity::Write_t( CTStream *ostr, BOOL bNetwork) // throw char *
{
	CEntity::Write_t(ostr,bNetwork);
	(*ostr)<<en_fHealth;
}

// apply some damage to the entity (see event EDamage for more info)
void CLiveEntity::ReceiveDamage(CEntity *penInflictor, enum DamageType dmtType,
	FLOAT fDamage, const FLOAT3D &vHitPoint, const FLOAT3D &vDirection)
{
	CEntityPointer penThis = this;  // keep this entity alive during this function

	// reduce your health
	en_fHealth-=fDamage;

	// throw an event that you are damaged
	EDamage eDamage;
	eDamage.penInflictor = penInflictor;
	eDamage.vDirection   = vDirection;
	eDamage.vHitPoint    = vHitPoint;
	eDamage.fAmount      = fDamage;
	eDamage.dmtType      = dmtType;
	SendEvent(eDamage);

	if(_pNetwork->m_bSingleMode)
	{
		// if health reached zero
		if (en_fHealth<=0) 
		{
			// throw an event that you have died
			EDeath eDeath;
			eDeath.eLastDamage = eDamage;
			SendEvent(eDeath);
		}
	}
}
/////////////////////////////////////////////////////////////////////
// CRationalEntity

/*
 * Constructor.
 */
CRationalEntity::CRationalEntity(void)
{
}

/* Calculate physics for moving. */
void CRationalEntity::ClearMovingTemp(void)
{
}
void CRationalEntity::PreMoving(void)
{
}
void CRationalEntity::DoMoving(void)
{
}
void CRationalEntity::PostMoving(void)
{
}
/* Copy entity from another entity of same class. */
void CRationalEntity::Copy(CEntity &enOther, ULONG ulFlags)
{
	CLiveEntity::Copy(enOther, ulFlags);
	if (!(ulFlags&COPY_REINIT)) {
		CRationalEntity *prenOther = (CRationalEntity *)(&enOther);
		en_timeTimer = prenOther->en_timeTimer;
		en_stslStateStack = prenOther->en_stslStateStack;
		if (prenOther->en_lnInTimers.IsLinked()) {
			en_pwoWorld->AddTimer(this);
		}
	}
}
/* Read from stream. */
void CRationalEntity::Read_t( CTStream *istr, BOOL bNetwork) // throw char *
{
	CLiveEntity::Read_t(istr,bNetwork);
	(*istr)>>en_timeTimer;
	// if waiting for thinking
	if (en_timeTimer!=THINKTIME_NEVER) {
		// add to list of thinkers
		en_pwoWorld->AddTimer(this);
	}
	// read the state stack
	en_stslStateStack.Clear();
	en_stslStateStack.SetAllocationStep(STATESTACK_ALLOCATIONSTEP);
	INDEX ctStates;
	(*istr)>>ctStates;
	for (INDEX iState=0; iState<ctStates; iState++) {
		(*istr)>>en_stslStateStack.Push();
	}
//강동민 수정 시작		03.02
	// PROJECTION SHADOW를 선택했을때, 그림자를 그리도록 합니다.
	// NOTE : 이 부분을 제거하면 그림자가 게임중에서 제대로 나오지 않음.
	if(GetFlags()&ENF_PROJECTIONSHADOWS)
	{
		if(en_RenderType == RT_SKAMODEL || en_RenderType == RT_SKAEDITORMODEL)
		{
			CModelInstance *pMI = GetModelInstance();
			if(pMI != NULL)
			{
				pMI->mi_bRenderShadow = TRUE;
			}
		}
	}
//강동민 수정 끝		03.02
}
/* Write to stream. */
void CRationalEntity::Write_t( CTStream *ostr, BOOL bNetwork) // throw char *
{
	CLiveEntity::Write_t(ostr,bNetwork);
	// if not currently waiting for thinking
	if (!en_lnInTimers.IsLinked()) {
		// set dummy thinking time as a flag for later loading
		en_timeTimer = THINKTIME_NEVER;
	}
	(*ostr)<<en_timeTimer;
	// write the state stack
	(*ostr)<<en_stslStateStack.Count();
	for(INDEX iState=0; iState<en_stslStateStack.Count(); iState++) {
		(*ostr)<<en_stslStateStack[iState];
	}
}

/*
 * Set next timer event to occur at given moment time.
 */
void CRationalEntity::SetTimerAt(TIME timeAbsolute)
{
	// must never set think back in time, except for special 'never' time
/*	ASSERTMSG(timeAbsolute>_pTimer->CurrentTick() ||
		timeAbsolute==THINKTIME_NEVER, "Do not SetThink() back in time!");*/ // eons 임시 주석 처리
	// set the timer
	en_timeTimer = timeAbsolute;

	// ehh. the entity should be allowed to enter the timers list if:
	// 1. The currently running app is the SeriousEditor
	// 2. or this is the server
	// 3. or this is the client and the entity is locally created (not received over the net)
	extern BOOL _bWorldEditorApp;
	if (_bWorldEditorApp || _pNetwork->IsServer() || (!_pNetwork->IsServer() && (!IsSentOverNet() || IsFlagOn(ENF_CLIENTHANDLING))))  {
		// add to world's list of timers if neccessary
		if (en_timeTimer != THINKTIME_NEVER) {
			en_pwoWorld->AddTimer(this);
		} else {
			if (en_lnInTimers.IsLinked()) {
				en_lnInTimers.Remove();
			}
		}
	}
}  

/*
 * Set next timer event to occur after given time has elapsed.
 */
void CRationalEntity::SetTimerAfter(TIME timeDelta)
{
	// set the execution for the moment that is that much ahead of the current tick
	SetTimerAt(_pTimer->CurrentTick()+timeDelta);
}

/* Cancel eventual pending timer. */
void CRationalEntity::UnsetTimer(void)
{
	en_timeTimer = THINKTIME_NEVER;
	if (en_lnInTimers.IsLinked()) {
		en_lnInTimers.Remove();
	}
}

/*
 * Unwind stack to a given state.
 */
void CRationalEntity::UnwindStack(SLONG slThisState)
{
	// for each state on the stack (from top to bottom)
	for(INDEX iStateInStack=en_stslStateStack.Count()-1; iStateInStack>=0; iStateInStack--) {
		// if it is the state
		if (en_stslStateStack[iStateInStack]==slThisState) {
			// unwind to it
			en_stslStateStack.PopUntil(iStateInStack);
			return;
		}
	}
	// the state must be on the stack
	ASSERTALWAYS("Unwinding to unexisting state!");
}

/*
 * Jump to a new state.
 */
void CRationalEntity::Jump(SLONG slThisState, SLONG slTargetState, BOOL bOverride, const CEntityEvent &eeInput)
{
	// unwind the stack to this state
	UnwindStack(slThisState);
	// set the new topmost state
	if (bOverride) {
		slTargetState = en_pecClass->ec_pdecDLLClass->GetOverridenState(slTargetState);
	}
	en_stslStateStack[en_stslStateStack.Count()-1] = slTargetState;
	// handle the given event in the new state
	HandleEvent(eeInput);
};
/*
 * Call a subautomaton.
 */
void CRationalEntity::Call(SLONG slThisState, SLONG slTargetState, BOOL bOverride, const CEntityEvent &eeInput)
{
	// unwind the stack to this state
	UnwindStack(slThisState);
	// push the new state to stack
	if (bOverride) {
		slTargetState = en_pecClass->ec_pdecDLLClass->GetOverridenState(slTargetState);
	}
	en_stslStateStack.Push() = slTargetState;
	// handle the given event in the new state
	HandleEvent(eeInput);
};
/*
 * Return from a subautomaton.
 */
void CRationalEntity::Return(SLONG slThisState, const CEntityEvent &eeReturn)
{
	// unwind the stack to this state
	UnwindStack(slThisState);
	// pop one state from the stack
	en_stslStateStack.PopUntil(en_stslStateStack.Count()-2);
	// handle the given event in the new topmost state
	HandleEvent(eeReturn);
};

// print stack to debug output
const char *CRationalEntity::PrintStackDebug(void)
{
	_RPT2(_CRT_WARN, "-- stack of '%s'@%gs\n", GetName(), _pTimer->CurrentTick());

	INDEX ctStates = en_stslStateStack.Count();
	for(INDEX iState=ctStates-1; iState>=0; iState--) {
		SLONG slState = en_stslStateStack[iState];
		_RPT2(_CRT_WARN, "0x%08x %s\n", slState, 
			en_pecClass->ec_pdecDLLClass->HandlerNameForState(slState));
	}
	_RPT0(_CRT_WARN, "----\n");
	return "ok";
}

/*
 * Handle an event - return false if event was not handled.
 */
BOOL CRationalEntity::HandleEvent(const CEntityEvent &ee)
{
	// for each state on the stack (from top to bottom)
	for(INDEX iStateInStack=en_stslStateStack.Count()-1; iStateInStack>=0; iStateInStack--) {
		// try to find a handler in that state
		pEventHandler pehHandler =
			HandlerForStateAndEvent(en_stslStateStack[iStateInStack], ee.ee_slEvent);
		// if there is a handler
		if (pehHandler!=NULL) {
			// call the function
			BOOL bHandled = (this->*pehHandler)(ee);
			// if the event was successfully handled
			if (bHandled) {
				// return that it was handled
				return TRUE;
			}
		}
	}

	// if no transition was found, the event was not handled
	return FALSE;
}

/*
 * Called after creating and setting its properties.
 */
void CRationalEntity::OnInitialize(const CEntityEvent &eeInput)
{
	// make sure entity doesn't destroy itself during intialization
	CEntityPointer penThis = this;

	// do not think
	en_timeTimer = THINKTIME_NEVER;
	if (en_lnInTimers.IsLinked()) {
		en_lnInTimers.Remove();
	}

	// initialize state stack
	en_stslStateStack.Clear();
	en_stslStateStack.SetAllocationStep(STATESTACK_ALLOCATIONSTEP);

	en_stslStateStack.Push() = 1;   // start state is always state with number 1

	// call the main function of the entity
	HandleEvent(eeInput);
}

/* Called before releasing entity. */
void CRationalEntity::OnEnd(void)
{
	// cancel eventual pending timer
	UnsetTimer();
}

void CEntity::SetNickNameDamageEffect(INDEX iNickIndex, NickNameEffectType iType)
{
	// Nothing
}

void CEntity::ReleaseChche( EntityComponentType eType, SLONG slID )
{
	en_pecClass->ec_pdecDLLClass->ReleaseCache(eType, slID);		
}
