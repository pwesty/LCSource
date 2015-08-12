#include "stdh.h"

#include <Engine/Base/Types.h>
#include <Engine/Base/Console.h>
#include <Engine/Base/MemoryTracking.h>
#include <Engine/Base/Stream.h>
#include <Engine/Math/Float.h>
#include <Engine/World/World.h>
#include <Engine/World/WorldEditingProfile.h>
#include <Engine/Graphics/RenderScene.h>
#include <Engine/World/WorldSettings.h>
#include <Engine/Entities/EntityClass.h>
#include <Engine/Entities/Precaching.h>                    
#include <Engine/Entities/EntityProperties.h>
#include <Engine/Base/ListIterator.inl>
#include <Engine/Templates/DynamicContainer.cpp>
#include <Engine/Graphics/Color.h>
#include <Engine/Brushes/BrushArchive.h>
#include <Engine/Terrain/TerrainArchive.h>
#include <Engine/Entities/InternalClasses.h>
#include <Engine/Network/CNetwork.h>
#include <Engine/Network/SessionState.h>
#include <Engine/Templates/DynamicArray.cpp>
#include <Engine/Brushes/Brush.h>
#include <Engine/Light/LightSource.h>
#include <Engine/Base/CRCTable.h>
#include <Engine/Templates/StaticArray.cpp>
#include <Engine/Templates/Selection.cpp>
#include <Engine/Terrain/Terrain.h>
#include <Engine/Templates/BSP.h>

#include <Engine/World/WorldEntityHashing.h>
#undef TYPE

#include <Engine/Templates/Stock_CEntityClass.h>

#include <Engine/Network/EMsgBuffer.h>
#include <Engine/Network/EntityHashing.h>
#include <Engine/Network/CNetwork.h>
#include <Engine/Network/Server.h>
#include <Engine/Entities/ZoneData.h>
#include <Engine/Loading.h>

#define MAXMOBDATA 300
#define MAXSHOPDATA	300
#define MAXSKILLDATA 500
#define MAXSSKILLDATA 100
#define MAXACTIONDATA	50		// yjpark

extern SLONG g_slZone = -1;
// NOTE : 로그인 월드에서 게임 시작시 -1값으로 들어가 있어서,
// NOTE : 월드 이동이 제대로 되지 않아서 0으로 수정함.
//extern SLONG g_slZone = 0;

extern BOOL g_bFirstIntoWorld = TRUE;

//존 추가시 수정
CZoneInfo::CZoneInfo()
{
	//zone 관련
	//존이름
	m_nZoneCount					= MAX_ZONE_COUNT;
	m_pZoneInfo						= new sZoneInfo[MAX_ZONE_COUNT];

	m_pZoneInfo[33].bRaidDungeon = TRUE;	// 몬트 샤인 예배당
	m_pZoneInfo[35].bRaidDungeon = TRUE;	// 몬트 샤인 암흑의 제단
	m_pZoneInfo[36].bRaidDungeon = TRUE;	// 아칸 사원
	m_pZoneInfo[41].bRaidDungeon = TRUE;	// 람슬로 지하던전
}

CZoneInfo::~CZoneInfo()
{
	delete [] m_pZoneInfo;
}

int CZoneInfo::GetExtraCount(int zone) const
{
	ASSERT(zone >= 0 && zone < m_nZoneCount);
	return m_pZoneInfo[zone].iExtraCount;
}

eZone CZoneInfo::GetZoneType(int nZone) const
{
	ASSERT(nZone >= 0 && nZone < m_nZoneCount);	
	return m_pZoneInfo[nZone].eZoneType;
}

/*
UINT CZoneInfo::GetMinLevel(int nZone) const
{
	ASSERT(nZone >= 0 && nZone < m_nZoneCount);	
	return m_pZoneInfo[nZone].uiMinLevel;
}

UINT CZoneInfo::GetMaxLevel(int nZone) const
{
	ASSERT(nZone >= 0 && nZone < m_nZoneCount);	
	return m_pZoneInfo[nZone].uiMaxLevel;
}
*/

DWORD CZoneInfo::GetAccessJob(int nZone) const
{
	ASSERT(nZone >= 0 && nZone < m_nZoneCount);	
	return m_pZoneInfo[nZone].dwAccessJob;
}

CTString CZoneInfo::GetZoneName(int nZone)
{
	if (nZone < 0 || nZone >= MAX_ZONE_COUNT)
		return CTString("");

	return m_pZoneInfo[nZone].strZoneName;
}

CTString CZoneInfo::GetZoneWldFile(int nZone)
{
	ASSERT(nZone >= 0 && nZone < m_nZoneCount);
	return m_pZoneInfo[nZone].strZoneWldFile;
}

CTString CZoneInfo::GetExtraName(int nZone, int nExtra)
{
	ASSERT(nZone >= 0 && nZone < m_nZoneCount);

	if (nExtra < 0 || nExtra >= m_pZoneInfo[nZone].vecExtraName.size())
		return CTString("");

	return m_pZoneInfo[nZone].vecExtraName[nExtra];
}

float CZoneInfo::GetTerMul( int nZone )
{
	if (nZone < 0 || nZone >= MAX_ZONE_COUNT)
		return 1.f;	// 기본 배율 1배.

	return m_pZoneInfo[nZone].fTer_Lodmul;
}

void CZoneInfo::SetZoneName( int nZone, CTString &strZoneName )
{
	ASSERT( nZone >= 0 && nZone < m_nZoneCount );

	m_pZoneInfo[nZone].strZoneName = strZoneName;
}

void CZoneInfo::SetExtraName( int nZone, int nExtra, const char* szName )
{
	ASSERT( nZone >= 0 && nZone < m_nZoneCount );
	
	if (nExtra >= 0 && nExtra < m_pZoneInfo[nZone].vecExtraName.size())
		m_pZoneInfo[nZone].vecExtraName[nExtra] = CTString(szName);
	else
		m_pZoneInfo[nZone].vecExtraName.push_back(CTString(szName));
}

void CZoneInfo::SetZoneData( int nZone, int nType, int nExtraCnt, const char* szName, const char* szWldFile, float fLodMul )
{
	ASSERT( nZone >= 0 && nZone < m_nZoneCount );

	m_pZoneInfo[nZone].eZoneType   = (eZone)nType;
	m_pZoneInfo[nZone].iExtraCount	= nExtraCnt;
	m_pZoneInfo[nZone].strZoneName = szName;
	m_pZoneInfo[nZone].strZoneWldFile = szWldFile;
	m_pZoneInfo[nZone].fTer_Lodmul	= fLodMul;
}

BOOL CZoneInfo::GetRaidDungeon(int nZone)
{
	ASSERT( nZone >= 0 && nZone < m_nZoneCount );
	return m_pZoneInfo[nZone].bRaidDungeon;
}

CZoneInfo::ObjInZone* CZoneInfo::GetInZoneData(int iIndex)
{
	if (iIndex > 0)
	{
		for (int i=0; i<m_ObjInZones.Count(); ++i)
		{
			if (m_ObjInZones[i].iObjectID == iIndex)
			{
				return &m_ObjInZones[i];
			}
		}
	}

	return NULL;
}

CZoneInfo::ObjInZone* CZoneInfo::GetInZoneData(CTString strName)
{
	if (strName.Length() > 0)
	{
		for (int i=0; i<m_ObjInZones.Count(); ++i)
		{
			if (m_ObjInZones[i].strName == strName)
			{
				return &m_ObjInZones[i];
			}
		}		
	}

	return NULL;
}

BOOL CZoneInfo::LoadObjInZoneData(void)
{
	if (g_slZone < 0 || !m_pZoneInfo[g_slZone].bRaidDungeon)
		return FALSE;

	const CTString strDataName = _fnmApplicationPath.FileDir() + "Data\\raidobjectlist.lod";

	FILE *fp;

	if ((fp = fopen(strDataName, "rb")) == NULL)
	{
		MessageBox(NULL, "File is not Exist.(Obj)", "Load Error!", MB_OK);
		return FALSE;
	}

	int iNumOfNPC	= 0;
	int iLength		= -1;
	int iReadBytes	= 0;
	int iLastIndex	= 0;

	iReadBytes = fread(&iLastIndex, sizeof(int), 1, fp);		// SHOP의 마지막 인덱스.
	m_ObjInZones.Clear();

	ASSERT(iLastIndex > 0 && "Invalid Objects Data");

	//////////////////////////////////////////////////////////////////////////	
	// MACRO DEFINITION
	//////////////////////////////////////////////////////////////////////////	
#define LOADINT(d)			iReadBytes = fread(&d, sizeof(int), 1, fp);
#define LOADSHORT(d)		iReadBytes = fread(&d, sizeof(short), 1, fp);
#define LOADCHAR(d)			iReadBytes = fread(&d, sizeof(char), 1, fp);
#define LOADFLOAT(d)		iReadBytes = fread(&d, sizeof(float), 1, fp);
#define LOADSTR(d)			{ int iLen; LOADINT(iLen); iReadBytes = fread(&d, iLen, 1, fp); }
	//////////////////////////////////////////////////////////////////////////	
	
	for (int i=0; i<iLastIndex; ++i)
	{
		INDEX iDbIndex;
		INDEX iAssignZone;
		INDEX iObjectType;
		INDEX iObjectIndex;
		char strObjectName[255];
		ZeroMemory(strObjectName, 255);

		LOADINT(iDbIndex);
		LOADINT(iAssignZone);
		LOADINT(iObjectType);
		LOADINT(iObjectIndex);
		LOADSTR(strObjectName);

		if (g_slZone == iAssignZone) // 현재 존의 오브젝트만 읽어 들인다.
		{
			m_ObjInZones.Add(ObjInZone((eObjects)iObjectType, iObjectIndex, CTString(strObjectName)));
		}
	}

	fclose(fp);

#undef LOADINT
#undef LOADCHAR
#undef LOADFLOAT
#undef LOADSTR

	return TRUE;
}

void CZoneInfo::AddLoadingInfo( float fStep, CTString strTex1, CTString strTex2)
{
	stLoadingInfo stInfo;

	stInfo.fStep = fStep;
	stInfo.strTex1 = strTex1;
	stInfo.strTex2 = strTex2;

	m_vecLoadingInfo.push_back(stInfo);
}

void CZoneInfo::GetLoadingInfo( int nZoneIndex, float& fStep, CTString& strTex1, CTString& strTex2)
{
	if (nZoneIndex < 0 || nZoneIndex >= m_vecLoadingInfo.size())
		return;

	fStep = m_vecLoadingInfo[nZoneIndex].fStep;
	strTex1 = m_vecLoadingInfo[nZoneIndex].strTex1;
	strTex2 = m_vecLoadingInfo[nZoneIndex].strTex2;
}

//안태훈 수정 끝	//(Zone Change System)(0.1)

template CDynamicContainer<CEntity>;
template CBrushPolygonSelection;
template CBrushSectorSelection;
template CEntitySelection;

extern BOOL _bPortalSectorLinksPreLoaded;
extern BOOL _bEntitySectorLinksPreLoaded;
extern INDEX net_bReportServerTraffic;

// calculate ray placement from origin and target positions (obsolete?)
static inline CPlacement3D CalculateRayPlacement(
												 const FLOAT3D &vOrigin, const FLOAT3D &vTarget)
{
	CPlacement3D plRay;
	// ray position is at origin
	plRay.pl_PositionVector = vOrigin;
	// calculate ray direction vector
	FLOAT3D vDirection = vTarget-vOrigin;
	// calculate ray orientation from the direction vector
	vDirection.Normalize();
	DirectionVectorToAngles(vDirection, plRay.pl_OrientationAngle);
	return plRay;
}

/* Constructor. */
CTextureTransformation::CTextureTransformation(void)
{
	tt_strName = "";
}

/* Constructor. */
CTextureBlending::CTextureBlending(void)
{
	tb_strName = "";
	tb_ubBlendingType = STXF_BLEND_OPAQUE;
	tb_colMultiply = C_WHITE|0xFF;
}

/*
* Constructor.
*/
CWorld::CWorld(void)
: wo_colBackground(C_BLACK)       // clear background color
, wo_pecWorldBaseClass(NULL)      // worldbase class must be obtained before using the world
, wo_bPortalLinksUpToDate(FALSE)  // portal-sector links must be updated
, wo_baBrushes(*new CBrushArchive)
, wo_taTerrains(*new CTerrainArchive)
, wo_ulSpawnFlags(0)
, m_bLoad(FALSE)
, m_bTradeItemLoad(false)
{
	wo_baBrushes.ba_pwoWorld = this;
	wo_taTerrains.ta_pwoWorld = this;
	
	// create empty texture movements
	wo_attTextureTransformations.New(256);
	wo_atbTextureBlendings.New(256);
	wo_astSurfaceTypes.New(256);
	wo_actContentTypes.New(256);
	wo_aetEnvironmentTypes.New(256);
	wo_aitIlluminationTypes.New(256);
	
	// initialize collision grid
	InitCollisionGrid();
	
	wo_slStateDictionaryOffset = 0;
	wo_strBackdropUp = "";
	wo_strBackdropFt = "";
	wo_strBackdropRt = "";
	wo_strBackdropObject = "";
	wo_fUpW = wo_fUpL = 1.0f; wo_fUpCX = wo_fUpCZ = 0.0f;
	wo_fFtW = wo_fFtH = 1.0f; wo_fFtCX = wo_fFtCY = 0.0f;
	wo_fRtL = wo_fRtH = 1.0f; wo_fRtCZ = wo_fRtCY = 0.0f;
	
	wo_ulNextEntityID = 1;
	
	// set default placement
	wo_plFocus = CPlacement3D( FLOAT3D(3.0f, 4.0f, 10.0f),
		ANGLE3D(AngleDeg( 20.0f), AngleDeg( -20.0f), 0));
	wo_fTargetDistance = 10.0f;
	
	// set default thumbnail placement
	wo_plThumbnailFocus = CPlacement3D( FLOAT3D(3.0f, 4.0f, 10.0f),
		ANGLE3D(AngleDeg( 20.0f), AngleDeg( -20.0f), 0));
	wo_fThumbnailTargetDistance = 10.0f;
	wo_whWorldEntityContainer.Clear();
	wo_whWorldEntityContainer.SetAllocationParameters(200,20,5);
	
	wo_cenEnemyHolders.Clear();
	wo_aSkillData.Clear();
	wo_aShopData.Clear();

	wo_iNumOfShop	= -1;
	
	wo_aCashShopData.Clear();
	
	m_pMousePointerEG = NULL;
}

/*
* Destructor.
*/
CWorld::~CWorld()
{
	// clear all arrays
	Clear();
	// destroy collision grid
	DestroyCollisionGrid();
	
	delete &wo_baBrushes;
	delete &wo_taTerrains;

	wo_aSkillData.Clear();
	wo_cenEnemyHolders.Clear();
	wo_aShopData.Clear();
	wo_iNumOfShop	= -1;
}

//안태훈 수정 시작	//(Add & Modify SSSE Effect)(0.1)
#include <Engine/Effect/CParticleGroupManager.h>
#include <Engine/Effect/CEffectManager.h>
#include <Engine/Effect/CEffectGroupManager.h>
//안태훈 수정 끝	//(Add & Modify SSSE Effect)(0.1)
/*
* Clear all arrays.
*/
void CWorld::Clear(void)
{
//안태훈 수정 시작	//(Add & Modify SSSE Effect)(0.1)
	CEffectGroupManager::Instance().ClearCreated();
//안태훈 수정 끝	//(Add & Modify SSSE Effect)(0.1)
//안태훈 수정 시작	//(5th Closed beta)(0.2)
	m_pMousePointerEG = NULL;
//안태훈 수정 끝	//(5th Closed beta)(0.2)

	// force finish of rendering
	_pGfx->Flush();
	
	// detach worldbase class
	if (wo_pecWorldBaseClass!=NULL) 
	{
		if ( wo_pecWorldBaseClass->ec_pdecDLLClass!=NULL
			&&wo_pecWorldBaseClass->ec_pdecDLLClass->dec_OnWorldEnd!=NULL) 
		{
			wo_pecWorldBaseClass->ec_pdecDLLClass->dec_OnWorldEnd(this);
		}
		wo_pecWorldBaseClass=NULL;
	}
	
	// clear collision grid
	// (must clear it before entities in order to avoid expanding of allocation arrays' free spaces array!)
	ClearCollisionGrid();
	
	{
		// clear background viewer
		SetBackgroundViewer(NULL);
		// make a new container of entities
		CDynamicContainer<CEntity> cenToDestroy = wo_cenEntities;
		
		// for each of the entities
		{
			FOREACHINDYNAMICCONTAINER(cenToDestroy, CEntity, iten) 
			{
				// destroy it
				iten->Destroy();    
			}
		}
		
		extern void ClearSentEvents();
		ClearSentEvents();
		
		// clear the network entity hash table
		//wo_ehEntityHashContainer.RemoveAll();
		// clear the world entity hash table
		{
			MEMTRACK_SETFLAGS(mem,MTF_NOSTACKTRACE);
			wo_whWorldEntityContainer.Clear();
			wo_whWorldEntityContainer.SetAllocationParameters(200,20,5);
		}
		
		// the original container must be empty
		ASSERT(wo_cenEntities.Count()==0);
//강동민 수정 시작 2차 작업			05.18
		//ASSERT(wo_cenAllEntities.Count()==0);		// 원본.
//강동민 수정 끝 2차 작업			05.18
		wo_cenEntities.Clear();
		wo_cenAllEntities.Clear();
		wo_aulDestroyedEntities.Clear();
		cenToDestroy.Clear();
		
		wo_ulNextEntityID = 1;
	}

//강동민 수정 시작 시스템 싱글던젼 개선	10.07
	m_vectorTargetNPC.clear();
	m_vectorPreCreateNPC.clear();
//강동민 수정 끝 싱글던젼 개선		10.07
	
	// clear brushes
	wo_baBrushes.ba_abrBrushes.Clear();
	wo_baBrushes.ba_apbpo.Clear();
	wo_baBrushes.ba_apbsc.Clear();
	// clear terrains
	wo_taTerrains.ta_atrTerrains.Clear();
	
	extern void ClearMovableEntityCaches(void);
	ClearMovableEntityCaches();
	
	// This is allways happening and its ok so dont trace it
	MEMTRACK_SETFLAGS(mem,MTF_NOSTACKTRACE);
	wo_attTextureTransformations.Clear();
	wo_atbTextureBlendings.Clear();
	wo_astSurfaceTypes.Clear();
	wo_actContentTypes.Clear();
	wo_aetEnvironmentTypes.Clear();
	wo_aitIlluminationTypes.Clear();
	
	wo_attTextureTransformations.New(256);
	wo_atbTextureBlendings.New(256);
	wo_astSurfaceTypes.New(256);
	wo_actContentTypes.New(256);
	wo_aetEnvironmentTypes.New(256);
	wo_aitIlluminationTypes.New(256);
}


/*
* Create a new entity of given class.
*/
CEntity *CWorld::CreateEntity(const CPlacement3D &plPlacement, CEntityClass *pecClass,ULONG ulEntityID,BOOL bNetwork)
{
	// if the world base class is not yet remembered and this class is world base
	if (wo_pecWorldBaseClass==NULL
		&& stricmp(pecClass->ec_pdecDLLClass->dec_strName, "WorldBase")==0) 
	{
		// remember it
		wo_pecWorldBaseClass = pecClass;
		// execute the class attach function
		if (pecClass->ec_pdecDLLClass->dec_OnWorldInit!=NULL) 
		{
			pecClass->ec_pdecDLLClass->dec_OnWorldInit(this);
		}
	}
	// gather CRCs of that class
	pecClass->AddToCRCTable();
	
	// ask the class to instance a new member
	CEntity *penEntity;
	{ 
		TRACKMEM(Entities, "Entities");
		penEntity = pecClass->New(); 
	}
	// add the reference made by the entity itself
	penEntity->AddReference();
	
	// set the entity's world pointer to this world
	penEntity->en_pwoWorld = this;
	// add the new member to this world's entity container
	wo_cenEntities.Add(penEntity);
	wo_cenAllEntities.Add(penEntity);
	
	// set a new identifier
	if (ulEntityID == WLD_AUTO_ENTITY_ID) 
	{
		penEntity->en_ulID = wo_ulNextEntityID++;
	}
	else 
	{
		penEntity->en_ulID = ulEntityID;
	}
	
	// add it to the world entity hash table  
	wo_whWorldEntityContainer.AddEntity(penEntity);
	
	// set up the placement
	penEntity->en_plPlacement = plPlacement;
	// calculate rotation matrix
	MakeRotationMatrixFast(penEntity->en_mRotation, penEntity->en_plPlacement.pl_OrientationAngle);
	
	// insert the entity create mesage into the server buffer
	if (_pNetwork->IsServer() && bNetwork) 
	{
		UWORD iEClassID = (UWORD) (pecClass->ec_pdecDLLClass->dec_iID & 0x0000FFFF);
		UWORD uwDummy = 0;
		
		if (net_bReportServerTraffic) 
		{
			CPrintF("Created entity: ID: %X, Class: %s\n",penEntity->en_ulID,pecClass->GetName());
		}
		
		extern CEntityMessage _emEntityMessage;
		_emEntityMessage.WriteEntityCreate(penEntity->en_ulID,plPlacement,iEClassID);
		_pNetwork->ga_srvServer.SendMessage(_emEntityMessage);
	}
	
	// return it
	return penEntity;
}

/*
* Create a new entity of given class.
*/
CEntity *CWorld::CreateEntity_t(const CPlacement3D &plPlacement,
								const CTFileName &fnmClass,ULONG ulEntityID,BOOL bNetwork) // throw char *
{
	// obtain a new entity class from global stock
	CEntityClass *pecClass = _pEntityClassStock->Obtain_t(fnmClass);
	// create entity with that class (obtains it once more)
	CEntity *penNew = CreateEntity(plPlacement, pecClass,ulEntityID,bNetwork);
	// release the class
	_pEntityClassStock->Release(pecClass);
	// return the entity
	return penNew;
}

/*
* Destroy one entities.
*/
void CWorld::DestroyOneEntity( CEntity *penToDestroy)
{
	// if the entity is targetable
	if (penToDestroy->IsTargetable()) 
	{
		// remove all eventual pointers to it
		UntargetEntity( penToDestroy);
	}
	// destroy it
	penToDestroy->Destroy();
}

/*
* Destroy a selection of entities.
*/
void CWorld::DestroyEntities(CEntitySelection &senToDestroy)
{
	
	// for each entity in selection
	FOREACHINDYNAMICCONTAINER(senToDestroy, CEntity, iten) 
	{
		// if the entity is targetable
		if (iten->IsTargetable()) 
		{
			// remove all eventual pointers to it
			UntargetEntity(iten);
		}
		// destroy it
		iten->Destroy();
	}
	// clear the selection on the container level
	/* NOTE: we must not clear the selection directly, since the entity objects
	contained there are already freed and deselecting them would make an access
	violation.
	*/
	senToDestroy.CDynamicContainer<CEntity>::Clear();
}

/*
* Clear all entity pointers that point to this entity.
*/
void CWorld::UntargetEntity(CEntity *penToUntarget)
{
	// for all entities in this world
	FOREACHINDYNAMICCONTAINER(wo_cenEntities, CEntity, itenInWorld)
	{
		// get the DLL class of this entity
		CDLLEntityClass *pdecDLLClass = itenInWorld->en_pecClass->ec_pdecDLLClass;
		
		// for all classes in hierarchy of this entity
		for(;pdecDLLClass!=NULL;pdecDLLClass = pdecDLLClass->dec_pdecBase) 
		{
			// for all properties
			for(INDEX iProperty=0; iProperty<pdecDLLClass->dec_ctProperties; iProperty++) 
			{
				CEntityProperty &epProperty = pdecDLLClass->dec_aepProperties[iProperty];
				
				// if the property type is entity pointer
				if (epProperty.ep_eptType == CEntityProperty::EPT_ENTITYPTR) 
				{
					// get the pointer
					CEntityPointer &penPointed = ENTITYPROPERTY(&*itenInWorld, epProperty.ep_slOffset, CEntityPointer);
					// if it points to the entity to be untargeted
					if (penPointed == penToUntarget) 
					{
						itenInWorld->End();
						// clear the pointer
						penPointed = NULL;
						itenInWorld->Initialize();
					}
				}
			}
		}
	}
	// if the entity is background viewer
	if (wo_penBackgroundViewer==penToUntarget) 
	{
		// reset background viewer
		SetBackgroundViewer(NULL);
	}
}

/*
* Clear all entity pointers that point to this entity.
*/
void CWorld::DumpReferenceInfo(CEntity *penEntity)
{
	// for all entities in this world
	FOREACHINDYNAMICCONTAINER(wo_cenAllEntities, CEntity, itenInWorld)
	{
		// get the DLL class of this entity
		CDLLEntityClass *pdecDLLClass = itenInWorld->en_pecClass->ec_pdecDLLClass;
		
		// for all classes in hierarchy of this entity
		for(;
		pdecDLLClass!=NULL;
		pdecDLLClass = pdecDLLClass->dec_pdecBase) 
		{
			// for all properties
			for(INDEX iProperty=0; iProperty<pdecDLLClass->dec_ctProperties; iProperty++) 
			{
				CEntityProperty &epProperty = pdecDLLClass->dec_aepProperties[iProperty];
				
				// if the property type is entity pointer
				if (epProperty.ep_eptType == CEntityProperty::EPT_ENTITYPTR) 
				{
					// get the pointer
					CEntityPointer &penPointed = ENTITYPROPERTY(&*itenInWorld, epProperty.ep_slOffset, CEntityPointer);
					// if it points to the entity to be untargeted
					if (penPointed == penEntity) 
					{
						CPrintF("   Referenced by ID: 0x%X    Property: %d\n",itenInWorld->en_ulID,iProperty);
					}
				}
			}
		}
	}
	
}


/*
* Find an entity with given character.
*/
CPlayerEntity *CWorld::FindEntityWithCharacter(CPlayerCharacter &pcCharacter)
{
	ASSERT(pcCharacter.pc_strName != "");
	
	// for each entity
	FOREACHINDYNAMICCONTAINER(wo_cenEntities, CEntity, iten) 
	{
		CEntity *pen = &*iten;
		// if it is player entity
		if( IsDerivedFromClass( pen, &CPlayerEntity_DLLClass)) 
		{
			CPlayerEntity *penPlayer = (CPlayerEntity*)pen;
			// if it has got that character
			if (penPlayer->en_pcCharacter.pc_iPlayerIndex == pcCharacter.pc_iPlayerIndex) 
			{
				// return its pointer
				return penPlayer;
			}
		}
	}
	// otherwise, none exists
	return NULL;
}

/*
* Add an entity to list of thinkers.
*/
void CWorld::AddTimer(CRationalEntity *penThinker)
{
	//  ASSERT(penThinker->en_timeTimer>_pTimer->CurrentTick());
	
	// if the entity is already in the list
	if (penThinker->en_lnInTimers.IsLinked()) 
	{
		// remove it
		penThinker->en_lnInTimers.Remove();
	}
	// for each entity in the thinker list
	FOREACHINLIST_IN(CRationalEntity, en_lnInTimers, wo_lhTimers, iten) 
	{
		// if the entity in list has greater or same think time than the one to add
		if (iten->en_timeTimer>=penThinker->en_timeTimer) 
		{
			// stop searching
			break;
		}
	}
	// add the new entity before current one
	iten.InsertBeforeCurrent(penThinker->en_lnInTimers);
}

// set overdue timers to be due in current time
void CWorld::AdjustLateTimers(TIME tmCurrentTime)
{
	
	// for each entity in the thinker list
	FOREACHINLIST(CRationalEntity, en_lnInTimers, wo_lhTimers, iten) 
	{
		CRationalEntity &en = *iten;
		// if the entity in list is overdue
		if (en.en_timeTimer<tmCurrentTime) 
		{
			// set it to current time
			en.en_timeTimer = tmCurrentTime;
		}
	}
}


/*
* Lock all arrays.
*/
void CWorld::LockAll(void)
{
	wo_cenEntities.Lock();
	wo_cenAllEntities.Lock();
	// lock the brush archive
	wo_baBrushes.ba_abrBrushes.Lock();
	// lock the terrain archive
	wo_taTerrains.ta_atrTerrains.Lock();
}

/*
* Unlock all arrays.
*/
void CWorld::UnlockAll(void)
{
	wo_cenEntities.Unlock();
	wo_cenAllEntities.Unlock();
	// unlock the brush archive
	wo_baBrushes.ba_abrBrushes.Unlock();
	// unlock the brush archive
	wo_taTerrains.ta_atrTerrains.Unlock();
}

/* Get background color for this world. */
COLOR CWorld::GetBackgroundColor(void)
{
	return wo_colBackground;
}

/* Set background color for this world. */
void CWorld::SetBackgroundColor(COLOR colBackground)
{
	wo_colBackground = colBackground;
}

/* Set background viewer entity for this world. */
void CWorld::SetBackgroundViewer(CEntity *penEntity)
{
	wo_penBackgroundViewer = penEntity;
}

/* Get background viewer entity for this world. */
CEntity *CWorld::GetBackgroundViewer(void)
{
	// if the background viewer entity is deleted
	if (wo_penBackgroundViewer!=NULL && wo_penBackgroundViewer->en_ulFlags&ENF_DELETED) 
	{
		// clear the pointer
		wo_penBackgroundViewer = NULL;
	}
	return wo_penBackgroundViewer;
}

/* Set description for this world. */
void CWorld::SetDescription(const CTString &strDescription)
{
	wo_strDescription = strDescription;
}
/* Get description for this world. */
const CTString &CWorld::GetDescription(void)
{
	return wo_strDescription;
}

// get/set name of the world
void CWorld::SetName(const CTString &strName)
{
	wo_strName = strName;
}
const CTString &CWorld::GetName(void)
{
	return wo_strName;
}

// get/set spawn flags for the world
void CWorld::SetSpawnFlags(ULONG ulFlags)
{
	wo_ulSpawnFlags = ulFlags;
}
ULONG CWorld::GetSpawnFlags(void)
{
	return wo_ulSpawnFlags;
}

/////////////////////////////////////////////////////////////////////
// Shadow manipulation functions

/*
* Recalculate all shadow maps that are not valid or of smaller precision.
*/
void CWorld::CalculateDirectionalShadows(void)
{
	extern INDEX _ctShadowLayers;
	extern INDEX _ctShadowClusters;
	CTimerValue tvStart;
	
	// clear shadow rendering stats
	tvStart = _pTimer->GetHighPrecisionTimer();
	_ctShadowLayers=0;
	_ctShadowClusters=0;
	
	// for each shadow map that is queued for calculation
	FORDELETELIST(CBrushShadowMap, bsm_lnInUncalculatedShadowMaps,
		wo_baBrushes.ba_lhUncalculatedShadowMaps, itbsm) 
	{
		// calculate shadows on it
		itbsm->GetBrushPolygon()->MakeShadowMap(this, TRUE);
	}
	
	// report shadow rendering stats
	CTimerValue tvStop = _pTimer->GetHighPrecisionTimer();
	CPrintF("Shadow calculation: total %d clusters in %d layers, %fs\n",
		_ctShadowClusters,
		_ctShadowLayers,
		(tvStop-tvStart).GetSeconds());
}

void CWorld::CalculateNonDirectionalShadows(void)
{
	// for each shadow map that is queued for calculation
	FORDELETELIST(CBrushShadowMap, bsm_lnInUncalculatedShadowMaps,
		wo_baBrushes.ba_lhUncalculatedShadowMaps, itbsm) 
	{
		// calculate shadows on it
		itbsm->GetBrushPolygon()->MakeShadowMap(this, FALSE);
	}
}


/* Find all shadow layers near a certain position. */
void CWorld::FindShadowLayers(
							  const FLOATaabbox3D &boxNear,
							  BOOL bSelectedOnly /*=FALSE*/,
							  BOOL bDirectional /*= TRUE*/)
{
	_pfWorldEditingProfile.StartTimer(CWorldEditingProfile::PTI_FINDSHADOWLAYERS);
	// for each entity in the world
	FOREACHINDYNAMICCONTAINER(wo_cenEntities, CEntity, iten) 
	{
		// if it is light entity and it influences the given range
		CLightSource *pls = iten->GetLightSource();
		if (pls!=NULL) 
		{
			FLOATaabbox3D boxLight(iten->en_plPlacement.pl_PositionVector, pls->ls_rFallOff);
			if ( bDirectional && (pls->ls_ulFlags &LSF_DIRECTIONAL)
				||boxLight.HasContactWith(boxNear)) 
			{
				// find layers for that light source
				pls->FindShadowLayers(bSelectedOnly);
			}
		}
	}
	_pfWorldEditingProfile.StopTimer(CWorldEditingProfile::PTI_FINDSHADOWLAYERS);
}
/* Discard shadows on all brush polygons in the world. */
void CWorld::DiscardAllShadows(void)
{
	FLOATaabbox3D box;
	// for each entity in the world
	FOREACHINDYNAMICCONTAINER(wo_cenEntities, CEntity, iten) 
	{
		// if it is brush entity
		if (iten->en_RenderType == CEntity::RT_BRUSH) 
		{
			// for each mip in its brush
			FOREACHINLIST(CBrushMip, bm_lnInBrush, iten->en_pbrBrush->br_lhBrushMips, itbm) 
			{
				box|=itbm->bm_boxBoundingBox;
				// for all sectors in this mip
				FOREACHINDYNAMICARRAY(itbm->bm_abscSectors, CBrushSector, itbsc) 
				{
					// for each polygon in the sector
					FOREACHINSTATICARRAY(itbsc->bsc_abpoPolygons, CBrushPolygon, itbpo) 
					{
						// discard its shadow map
						itbpo->DiscardShadows();
					}
				}
			}
		}
	}
	// find all shadow layers in the world
	FindShadowLayers(box);
}
/////////////////////////////////////////////////////////////////////
// Hide/Show functions

/*
* Hide entities contained in given selection.
*/
void CWorld::HideSelectedEntities(CEntitySelection &selenEntitiesToHide)
{
	// for all entities in the selection
	FOREACHINDYNAMICCONTAINER(selenEntitiesToHide, CEntity, iten) 
	{
		if( iten->IsSelected(ENF_SELECTED) &&
			!((iten->en_RenderType==CEntity::RT_BRUSH) && (iten->en_ulFlags&ENF_ZONING)) )
		{
			// hide the entity
			iten->en_ulFlags |= ENF_HIDDEN;
		}
	}
}

/*
* Hide all unselected entities.
*/
void CWorld::HideUnselectedEntities(void)
{
	// for each entity in the world
	FOREACHINDYNAMICCONTAINER(wo_cenEntities, CEntity, iten)
	{
		if( !iten->IsSelected(ENF_SELECTED) &&
			!((iten->en_RenderType==CEntity::RT_BRUSH)&&(iten->en_ulFlags&ENF_ZONING)) )
		{
			// hide it
			iten->en_ulFlags |= ENF_HIDDEN;
		}
	}
}

/*
* Show all entities.
*/
void CWorld::ShowAllEntities(void)
{
	// for each entity in the world
	FOREACHINDYNAMICCONTAINER(wo_cenEntities, CEntity, iten)
	{
		iten->en_ulFlags &= ~ENF_HIDDEN;
	}
}

/*
* Hide sectors contained in given selection.
*/
void CWorld::HideSelectedSectors(CBrushSectorSelection &selbscSectorsToHide)
{
	// for all sectors in the selection
	FOREACHINDYNAMICCONTAINER(selbscSectorsToHide, CBrushSector, itbsc) 
	{
		// hide the sector
		itbsc->bsc_ulFlags |= BSCF_HIDDEN;
	}
}

/*
* Hide all unselected sectors.
*/
void CWorld::HideUnselectedSectors(void)
{
	// for each entity in the world
	FOREACHINDYNAMICCONTAINER(wo_cenEntities, CEntity, iten) 
	{
		// if it is brush entity
		if (iten->en_RenderType == CEntity::RT_BRUSH) 
		{
			// for each mip in its brush
			FOREACHINLIST(CBrushMip, bm_lnInBrush, iten->en_pbrBrush->br_lhBrushMips, itbm) 
			{
				// for all sectors in this mip
				FOREACHINDYNAMICARRAY(itbm->bm_abscSectors, CBrushSector, itbsc) 
				{
					// if the sector is not selected
					if (!itbsc->IsSelected(BSCF_SELECTED)) 
					{
						// hide it
						itbsc->bsc_ulFlags |= BSCF_HIDDEN;
					}
				}
			}
		}
	}
}

/*
* Show all sectors.
*/
void CWorld::ShowAllSectors(void)
{
	// for each entity in the world
	FOREACHINDYNAMICCONTAINER(wo_cenEntities, CEntity, iten) 
	{
		// if it is brush entity
		if (iten->en_RenderType == CEntity::RT_BRUSH) 
		{
			// for each mip in its brush
			FOREACHINLIST(CBrushMip, bm_lnInBrush, iten->en_pbrBrush->br_lhBrushMips, itbm) 
			{
				// for all sectors in this mip
				FOREACHINDYNAMICARRAY(itbm->bm_abscSectors, CBrushSector, itbsc) 
				{
					// show the sector
					itbsc->bsc_ulFlags &= ~BSCF_HIDDEN;
				}
			}
		}
	}
}

/*
* Select all polygons in selected sectors with same texture.
*/
void CWorld::SelectByTextureInSelectedSectors(
											  CTFileName fnTexture, CBrushPolygonSelection &selbpoSimilar, INDEX iTexture)
{
	// for each entity in the world
	FOREACHINDYNAMICCONTAINER(wo_cenEntities, CEntity, iten) 
	{
		// if it is brush entity
		if (iten->en_RenderType == CEntity::RT_BRUSH) 
		{
			// for each mip in its brush
			FOREACHINLIST(CBrushMip, bm_lnInBrush, iten->en_pbrBrush->br_lhBrushMips, itbm) 
			{
				// for all sectors in this mip
				FOREACHINDYNAMICARRAY(itbm->bm_abscSectors, CBrushSector, itbsc) 
				{
					// if sector is selected
					if (itbsc->IsSelected(BSCF_SELECTED)) 
					{
						// for all polygons in sector
						FOREACHINSTATICARRAY(itbsc->bsc_abpoPolygons, CBrushPolygon, itbpo)
						{
							// if it is not portal and is not selected and has same texture
							if ( (!(itbpo->bpo_ulFlags&BPOF_PORTAL) || (itbpo->bpo_ulFlags&(BPOF_TRANSLUCENT|BPOF_TRANSPARENT))) &&
								!itbpo->IsSelected(BPOF_SELECTED) &&
								(itbpo->bpo_abptTextures[iTexture].bpt_toTexture.GetData() != NULL) &&
								(itbpo->bpo_abptTextures[iTexture].bpt_toTexture.GetData()->GetName()
								== fnTexture) )
								// select this polygon
								selbpoSimilar.Select(*itbpo);
						}
					}
				}
			}
		}
	}
}

/*
* Select all polygons in world with same texture.
*/
void CWorld::SelectByTextureInWorld(
									CTFileName fnTexture, CBrushPolygonSelection &selbpoSimilar, INDEX iTexture)
{
	// for each entity in the world
	FOREACHINDYNAMICCONTAINER(wo_cenEntities, CEntity, iten) 
	{
		// if it is brush entity
		if (iten->en_RenderType == CEntity::RT_BRUSH) 
		{
			// for each mip in its brush
			FOREACHINLIST(CBrushMip, bm_lnInBrush, iten->en_pbrBrush->br_lhBrushMips, itbm) 
			{
				// for all sectors in this mip
				FOREACHINDYNAMICARRAY(itbm->bm_abscSectors, CBrushSector, itbsc) 
				{
					// for all polygons in sector
					FOREACHINSTATICARRAY(itbsc->bsc_abpoPolygons, CBrushPolygon, itbpo)
					{
						// if it is not non translucent portal and is not selected and has same texture
						if ( (!(itbpo->bpo_ulFlags&BPOF_PORTAL) || (itbpo->bpo_ulFlags&(BPOF_TRANSLUCENT|BPOF_TRANSPARENT))) &&
							!itbpo->IsSelected(BPOF_SELECTED) &&
							(itbpo->bpo_abptTextures[iTexture].bpt_toTexture.GetData() != NULL) &&
							(itbpo->bpo_abptTextures[iTexture].bpt_toTexture.GetData()->GetName()
							== fnTexture) )
							// select this polygon
							selbpoSimilar.Select(*itbpo);
					}
				}
			}
		}
	}
}

/*
* Reinitialize entities from their properties. (use only in WEd!)
*/
void CWorld::ReinitializeEntities(void)
{
	_pfWorldEditingProfile.StartTimer(CWorldEditingProfile::PTI_REINITIALIZEENTITIES);
	
	CTmpPrecachingNow tpn;
	
	// for each entity in the world
	FOREACHINDYNAMICCONTAINER(wo_cenEntities, CEntity, iten) 
	{
		//0507 kwon 수정. 
		if(!(iten->en_ulFlags & ENF_ALIVE))
		{
			// reinitialize it
			iten->Reinitialize();
		}
	}
	
	_pfWorldEditingProfile.StopTimer(CWorldEditingProfile::PTI_REINITIALIZEENTITIES);
}
/* Precache data needed by entities. */
void CWorld::PrecacheEntities_t(void)
{
	// for each entity in the world
	INDEX ctEntities = wo_cenEntities.Count();
	INDEX iEntity = 0;
	FOREACHINDYNAMICCONTAINER(wo_cenEntities, CEntity, iten) 
	{
		// precache
		CallProgressHook_t((FLOAT)iEntity/ctEntities);
		iten->Precache();
		iEntity++;
	}
}
// delete all entities that don't fit given spawn flags
void CWorld::FilterEntitiesBySpawnFlags(ULONG ulFlags)
{
	
	BOOL bOldAllowRandom = _pNetwork->ga_sesSessionState.ses_bAllowRandom;
	_pNetwork->ga_sesSessionState.ses_bAllowRandom = TRUE;
	
	// create an empty selection of entities
	CEntitySelection senToDestroy;
	// for each entity in the world
	{
		FOREACHINDYNAMICCONTAINER(wo_cenEntities, CEntity, iten) 
		{
			// if brush
			if (iten->en_RenderType==CEntity::RT_BRUSH
				||iten->en_RenderType==CEntity::RT_FIELDBRUSH) 
			{
				// skip it (brushes must not be deleted on the fly)
				continue;
			}
			
			// if it shouldn't exist
			ULONG ulEntityFlags = iten->GetSpawnFlags();
			if (!(ulEntityFlags&ulFlags&SPF_MASK_DIFFICULTY)
				||!(ulEntityFlags&ulFlags&SPF_MASK_GAMEMODE)) 
			{
				// add it to the selection
				senToDestroy.Select(*iten);
			}
		}
	}
	// destroy all selected entities
	DestroyEntities(senToDestroy);
	_pNetwork->ga_sesSessionState.ses_bAllowRandom = bOldAllowRandom;
}

// create links between zoning-brush sectors and non-zoning entities in sectors
void CWorld::LinkEntitiesToSectors(void)
{
	_pfWorldEditingProfile.StartTimer(CWorldEditingProfile::PTI_LINKENTITIESTOSECTORS);
	// for each entity in the world
	FOREACHINDYNAMICCONTAINER(wo_cenEntities, CEntity, iten) 
	{
		CEntity &en = *iten;
		// cache eventual collision info
		en.FindCollisionInfo();
		en.UpdateSpatialRange();
		// link it
		if (!_bEntitySectorLinksPreLoaded) 
		{
			en.FindSectorsAroundEntity();
		}
	}
	// NOTE: this is here to force relinking for all moving zoning brushes after loading!
	// for each entity in the world
	{
		FOREACHINDYNAMICCONTAINER(wo_cenEntities, CEntity, iten) 
		{
			CEntity &en = *iten;
			if (en.en_RenderType==CEntity::RT_BRUSH && 
				(en.en_ulFlags&ENF_ZONING) && (en.en_ulPhysicsFlags&EPF_MOVABLE))
			{
				// recalculate all bounding boxes relative to new position
				extern BOOL _bDontDiscardLinks;
				_bDontDiscardLinks = TRUE;
				en.en_pbrBrush->CalculateBoundingBoxes();
				_bDontDiscardLinks = FALSE;
				// FPU must be in 53-bit mode
				CSetFPUPrecision FPUPrecision(FPT_53BIT);
				
				// for all brush mips
				FOREACHINLIST(CBrushMip, bm_lnInBrush, en.en_pbrBrush->br_lhBrushMips, itbm) 
				{
					// for all sectors in the mip
					{
						FOREACHINDYNAMICARRAY(itbm->bm_abscSectors, CBrushSector, itbsc) 
						{
							// find entities in sector
							itbsc->FindEntitiesInSector();
						}
					}
				}
			}
		}
	}
	_pfWorldEditingProfile.StopTimer(CWorldEditingProfile::PTI_LINKENTITIESTOSECTORS);
}

// rebuild all links in world
void CWorld::RebuildLinks(void)
{
	wo_baBrushes.LinkPortalsAndSectors();
	_bEntitySectorLinksPreLoaded = FALSE;
	LinkEntitiesToSectors();
}

/* Update sectors during brush vertex moving */
void CWorld::UpdateSectorsDuringVertexChange( CBrushVertexSelection &selVertex)
{
	// create container of sectors that will need to be updated
	CDynamicContainer<CBrushSector> cbscToUpdate;
	
	{
		FOREACHINDYNAMICCONTAINER( selVertex, CBrushVertex, itbvx)
		{
			// add the sector of that vertex to list for updating
			if (!cbscToUpdate.IsMember(itbvx->bvx_pbscSector)) 
			{
				cbscToUpdate.Add(itbvx->bvx_pbscSector);
			}
		}
	}
	
	// for each sector to be updated
	{
		FOREACHINDYNAMICCONTAINER( cbscToUpdate, CBrushSector, itbsc)
		{
			// recalculate planes for polygons from their vertices
			itbsc->MakePlanesFromVertices();
		}
	}
}

/* Update sectors after brush vertex moving */
void CWorld::UpdateSectorsAfterVertexChange( CBrushVertexSelection &selVertex)
{
	// create container of sectors that will need to be updated
	CDynamicContainer<CBrushSector> cbscToUpdate;
	
	{
		FOREACHINDYNAMICCONTAINER( selVertex, CBrushVertex, itbvx)
		{
			// add the sector of that vertex to list for updating
			if (!cbscToUpdate.IsMember(itbvx->bvx_pbscSector)) 
			{
				cbscToUpdate.Add(itbvx->bvx_pbscSector);
			}
		}
	}
	
	// for each sector to be updated
	{
		FOREACHINDYNAMICCONTAINER( cbscToUpdate, CBrushSector, itbsc)
		{
			// update it
			itbsc->UpdateVertexChanges();
		}
	}
}

/* Triangularize polygons that contain vertices from given selection */
void CWorld::TriangularizeForVertices( CBrushVertexSelection &selVertex)
{
	// create container of sectors that contain polygons that need to be triangularized
	CDynamicContainer<CBrushSector> cbscToTriangularize;
	
	{
		FOREACHINDYNAMICCONTAINER( selVertex, CBrushVertex, itbvx)
		{
			// add the sector of that vertex to list for triangularizing
			if (!cbscToTriangularize.IsMember(itbvx->bvx_pbscSector)) 
			{
				cbscToTriangularize.Add(itbvx->bvx_pbscSector);
			}
		}
	}
	
	// for each sector to be updated
	{
		FOREACHINDYNAMICCONTAINER( cbscToTriangularize, CBrushSector, itbsc)
		{
			// update it
			itbsc->TriangularizeForVertices(selVertex);
		}
	}
}

// get entity by its ID
CEntity *CWorld::EntityFromID(ULONG ulID)
{  
	CEntity* penEntity = wo_whWorldEntityContainer.GetEntity(ulID);
	
	ASSERT(penEntity != NULL);
	return penEntity;
}


// does entity with a given ID exist? (if it does, return it's pointer)
BOOL CWorld::EntityExists(ULONG ulID,CEntity* &penEntity)
{
	penEntity = wo_whWorldEntityContainer.GetEntity(ulID);
	if (penEntity != NULL) {return TRUE;}
	return FALSE;
}


/* Triangularize selected polygons. */
void CWorld::TriangularizePolygons(CDynamicContainer<CBrushPolygon> &dcPolygons)
{
	ClearMarkedForUseFlag();
	CDynamicContainer<CBrushSector> cbscToProcess;
	// for each polyon in selection
	FOREACHINDYNAMICCONTAINER(dcPolygons, CBrushPolygon, itbpo)
	{
		CBrushPolygon &bp=*itbpo;
		bp.bpo_ulFlags |= BPOF_MARKED_FOR_USE;
		CBrushSector *pbsc=bp.bpo_pbscSector;
		if( !cbscToProcess.IsMember( pbsc))
		{
			cbscToProcess.Add( pbsc);
		}
	}
	
	FOREACHINDYNAMICCONTAINER(cbscToProcess, CBrushSector, itbsc)
	{
		itbsc->TriangularizeMarkedPolygons();
		itbsc->UpdateVertexChanges();
	}
}

// Clear marked for use flag on all polygons in world
void CWorld::ClearMarkedForUseFlag(void)
{
	// for each entity in the world
	FOREACHINDYNAMICCONTAINER(wo_cenEntities, CEntity, iten) 
	{
		// if it is brush entity
		if (iten->en_RenderType == CEntity::RT_BRUSH) 
		{
			// for each mip in its brush
			FOREACHINLIST(CBrushMip, bm_lnInBrush, iten->en_pbrBrush->br_lhBrushMips, itbm) 
			{
				// for all sectors in this mip
				FOREACHINDYNAMICARRAY(itbm->bm_abscSectors, CBrushSector, itbsc) 
				{
					// for each polygon in the sector
					FOREACHINSTATICARRAY(itbsc->bsc_abpoPolygons, CBrushPolygon, itbpo) 
					{
						// discard marked for use flag
						itbpo->bpo_ulFlags &= ~BPOF_MARKED_FOR_USE;
					}
				}
			}
		}
	}
}

void CWorld::AddEntitiesInSector(CBrushSector *pbscSectorInside)
{
	if (pbscSectorInside == NULL || pbscSectorInside->bsc_bspBSPTree.bt_pbnRoot == NULL)
	{
		return;
	}

	CEntity* penPlEntity;
	penPlEntity = CEntity::GetPlayerEntity( 0 );

	{FOREACHDSTOFSRC(pbscSectorInside->bsc_rsEntities, CEntity, en_rdSectors, pen)
		if (pen->en_RenderType == CEntity::RT_SKAMODEL && pen->IsEnemy() && pen->GetFlags() & ENF_ALIVE && !(pen->GetFlags() & ENF_INVISIBLE)
			&& !(pen->GetFlags() & ENF_HIDDEN))
		{
			FLOAT3D vDelta = pen->GetPlacement().pl_PositionVector - penPlEntity->GetPlacement().pl_PositionVector;

			if (vDelta.Length() <= 30.0f)
			{
				// 리스트에 담는다.
				CEntityPointer penPointer = pen;
				m_vectorTabTargetEntities.push_back(penPointer);
			}
		}
	ENDFOR}
}

void CWorld::AddEntityZoningSector(CBrushSector *pbsc)
{
	CBrushMip *pbmBrushMip = pbsc->bsc_pbmBrushMip;
	CBrush3D *pbrBrush = pbmBrushMip->bm_pbrBrush;
	ASSERT(pbrBrush != NULL);
	CEntity *penBrush = pbrBrush->br_penEntity;
	ASSERT(penBrush != NULL);

	if (penBrush->en_RenderType == CEntity::RT_FIELDBRUSH)
	{
		return;
	}

	CBrushMip *pbmRelevant = pbrBrush->GetFirstMip();

	if (pbmRelevant == pbmBrushMip)
	{
		AddEntitiesInSector(pbsc);
	}
}

void CWorld::SearchSectorAroundEntity(CEntity* pen, const FLOAT3D &vEyesPos, CAnyProjection3D &prProjection)
{
	DOUBLE3D vdViewSphere = FLOATtoDOUBLE(pen->GetPlacement().pl_PositionVector);//FLOATtoDOUBLE(vEyesPos);
	DOUBLE dViewSphereR = 30.0f;//prProjection->NearClipDistanceR()*1.5f + 1.0f;
	//FLOATaabbox3D boxView(vEyesPos, FLOAT(dViewSphereR));
	FLOATaabbox3D boxView(pen->GetPlacement().pl_PositionVector, FLOAT(dViewSphereR));

	CListHead lhToAdd;
	
	{FOREACHSRCOFDST(pen->en_rdSectors, CBrushSector, bsc_rsEntities, pbsc)
		if (!pbsc->bsc_lnInActiveSectors.IsLinked())
		{
			lhToAdd.AddTail(pbsc->bsc_lnInActiveSectors);
		}
	ENDFOR}

	while (!lhToAdd.IsEmpty())
	{
		CBrushSector *pbsc = LIST_HEAD(lhToAdd, CBrushSector, bsc_lnInActiveSectors);
		pbsc->bsc_lnInActiveSectors.Remove();

		// 여기서 Sector안에 Entity를 list에 담는다.
		//
		AddEntityZoningSector(pbsc); // 여기가 중요

		if (!pbsc->bsc_lnInActiveSectors.IsLinked())
		{
			continue;
		}

		FOREACHINSTATICARRAY(pbsc->bsc_abpoPolygons, CBrushPolygon, itbpo) {
			CBrushPolygon *pbpo = itbpo;
			if (!(pbpo->bpo_ulFlags & BPOF_PORTAL) || !pbpo->bpo_boxBoundingBox.HasContactWith(boxView))
			{
				continue;
			}

			{FOREACHDSTOFSRC(pbpo->bpo_rsOtherSideSectors, CBrushSector, bsc_rdOtherSidePortals, pbscRelated)
				if (!pbscRelated->bsc_lnInActiveSectors.IsLinked())
				{
					if (pbscRelated->bsc_boxBoundingBox.HasContactWith(boxView) && 
						pbscRelated->bsc_bspBSPTree.TestSphere(vdViewSphere, dViewSphereR) >= 0)
					{
						lhToAdd.AddTail(pbscRelated->bsc_lnInActiveSectors);
					}
				}
			ENDFOR}
		}
	}
}

bool TargetSelectedEnemy_Comp(const CEntityPointer& l, const CEntityPointer& r)
{
	CEntity* tmpPlayer = CEntity::GetPlayerEntity(0); // 자기 중심

	FLOAT3D vDelta1 = l.ep_pen->GetPlacement().pl_PositionVector - tmpPlayer->GetPlacement().pl_PositionVector;
	FLOAT3D vDelta2 = r.ep_pen->GetPlacement().pl_PositionVector - tmpPlayer->GetPlacement().pl_PositionVector;

	return vDelta1.Length() < vDelta2.Length();
}

void CWorld::TargetSelectedEnemy_Sort(void)
{
	std::sort(m_vectorTabTargetEntities.begin(), m_vectorTabTargetEntities.end(), TargetSelectedEnemy_Comp);
}
