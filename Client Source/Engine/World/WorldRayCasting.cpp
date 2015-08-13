#include "stdh.h"

#include <Engine/Base/Console.h>
#include <Engine/World/World.h>
#include <Engine/Rendering/Render.h>
#include <Engine/World/WorldRayCasting.h>
#include <Engine/Base/ListIterator.inl>
#include <Engine/Templates/DynamicContainer.cpp>
#include <Engine/Templates/DynamicArray.cpp>
#include <Engine/Brushes/Brush.h>
#include <Engine/Templates/StaticArray.cpp>
#include <Engine/Models/ModelObject.h>
#include <Engine/Math/Clipping.inl>
#include <Engine/Entities/EntityCollision.h>
#include <Engine/Math/Geometry.inl>
#include <Engine/Network/CNetwork.h>
#include <Engine/Ska/Render.h>
#include <Engine/Terrain/Terrain.h>

#include <Engine/Base/Statistics_internal.h>
#include <Engine/Templates/StaticStackArray.cpp>

#define EPSILON (0.1f)

class CActiveSector {
public:
	CBrushSector *as_pbsc;
	void Clear(void) {};
};

static CStaticStackArray<CActiveSector> _aas;
CListHead _lhTestedTerrains; // list of tested terrains

// calculate origin position from ray placement
static inline FLOAT3D CalculateRayOrigin(const CPlacement3D &plRay)
{
	// origin is the position from the placement
	return plRay.pl_PositionVector;
}
// calculate target position from ray placement
static inline FLOAT3D CalculateRayTarget(const CPlacement3D &plRay)
{
	// calculate direction of the ray
	FLOAT3D vDirection;
	AnglesToDirectionVector(plRay.pl_OrientationAngle, vDirection);
	// make target be from the origin in that direction
	return plRay.pl_PositionVector+vDirection;
}

/*
 * Internal construction helper.
 */
void CCastRay::Init(CEntity *penOrigin, const FLOAT3D &vOrigin, const FLOAT3D &vTarget)
{
	ClearSectorList();
	cr_penOrigin = penOrigin;
	cr_vOrigin = vOrigin;
	cr_vTarget = vTarget;
	cr_bAllowOverHit = FALSE;
	cr_pbpoIgnore = NULL;
	cr_penIgnore = NULL;

	cr_bHitPortals = FALSE;
	cr_bHitTranslucentPortals = TRUE;
	cr_ttHitModels = TT_SIMPLE;
	cr_bHitFields = FALSE;
	cr_bPhysical = FALSE;
	cr_bHitBrushes = TRUE;
	cr_bHitTerrainInvisibleTris = FALSE;
	cr_fTestR = 0;

	cr_bFindBone = TRUE;
	cr_iBoneHit	 = -1;

	cl_plRay.pl_PositionVector = vOrigin;
//안태훈 수정 시작	//(For Performance)(0.2)
	DirectionVectorToAnglesNoSnap((vTarget-vOrigin).Normalize(), cl_plRay.pl_OrientationAngle);
	//DirectionVectorToAngles((vTarget-vOrigin).Normalize(), cl_plRay.pl_OrientationAngle);
//안태훈 수정 끝	//(For Performance)(0.2)
}

/*
 * Constructor.
 */
CCastRay::CCastRay(CEntity *penOrigin, const CPlacement3D &plOrigin)
{
	Init(penOrigin, CalculateRayOrigin(plOrigin), CalculateRayTarget(plOrigin));
	// mark last found hit point in infinity
	cr_fHitDistance = UpperLimit(0.0f);
}
CCastRay::CCastRay(CEntity *penOrigin, const CPlacement3D &plOrigin, FLOAT fMaxTestDistance)
{
	Init(penOrigin, CalculateRayOrigin(plOrigin), CalculateRayTarget(plOrigin));
	// mark last found hit point just as far away as we wan't to test
	cr_fHitDistance = fMaxTestDistance;
}
CCastRay::CCastRay(CEntity *penOrigin, const FLOAT3D &vOrigin, const FLOAT3D &vTarget)
{
	Init(penOrigin, vOrigin, vTarget);
	// mark last found hit point just a bit behind the target
	cr_fHitDistance = (cr_vTarget-cr_vOrigin).Length() + EPSILON;
}

CCastRay::~CCastRay(void)
{
	ClearSectorList();
}

void CCastRay::ClearSectorList(void)
{
//안태훈 수정 시작	//(For Performance)(0.2)
	// for each active sector
	//loop unrolling, 효과는 미지수.
	INDEX cnt = _aas.Count();
	INDEX cnt16 = cnt & 0xFFFFFFF0;
	INDEX ias;
#define INNER_LOOP {_aas[ias++].as_pbsc->bsc_ulFlags&=~BSCF_RAYTESTED;}
	// mark it as inactive
	for( ias=0; ias<cnt16;)
	{
		INNER_LOOP; INNER_LOOP; INNER_LOOP; INNER_LOOP;
		INNER_LOOP; INNER_LOOP; INNER_LOOP; INNER_LOOP;
		INNER_LOOP; INNER_LOOP; INNER_LOOP; INNER_LOOP;
		INNER_LOOP; INNER_LOOP; INNER_LOOP; INNER_LOOP;
	}
	for( ias = cnt16; ias < cnt; )
	{
		INNER_LOOP;
	}
#undef INNER_LOOP
	_aas.PopAll();
//안태훈 수정 끝	//(For Performance)(0.2)
}

void CCastRay::TestModelSimple(CEntity *penModel, CModelObject &mo)
{
	// get model's bounding box for current frame
	FLOATaabbox3D boxModel;
	mo.GetCurrentFrameBBox(boxModel);
	boxModel.StretchByVector(mo.mo_Stretch);
	// get center and radius of the bounding sphere in absolute space
	FLOAT fSphereRadius = boxModel.Size().Length()/2.0f;
	FLOAT3D vSphereCenter = boxModel.Center();
	vSphereCenter*=penModel->en_mRotation;
	vSphereCenter+=penModel->en_plPlacement.pl_PositionVector;

	// if the ray doesn't hit the sphere
	FLOAT fSphereHitDistance;
	if (!RayHitsSphere(cr_vOrigin, cr_vTarget,
		vSphereCenter, fSphereRadius+cr_fTestR, fSphereHitDistance) ) {
		// ignore
		return;
	}

	// if the ray hits the sphere closer than closest found hit point yet
	if (fSphereHitDistance<cr_fHitDistance && fSphereHitDistance>0.0f) {
		// set the current entity as new hit target
		cr_fHitDistance=fSphereHitDistance;
		cr_penHit = penModel;
		cr_pbscBrushSector = NULL;
		cr_pbpoBrushPolygon = NULL;
	}
}

void CCastRay::TestModelCollisionBox(CEntity *penModel)
{
	// if no collision box
	CCollisionInfo *pci = penModel->en_pciCollisionInfo;
	if (pci==NULL) {
		// don't test
		return;
	}

	// get model's collision bounding box
	FLOATaabbox3D &boxModel = pci->ci_boxCurrent;
	FLOAT fSphereRadius = boxModel.Size().Length()/2.0f;
	FLOAT3D vSphereCenter = boxModel.Center();

	// if the ray doesn't hit the sphere
	FLOAT fSphereHitDistance;
	if (!RayHitsSphere(cr_vOrigin, cr_vTarget,
		vSphereCenter, fSphereRadius+cr_fTestR, fSphereHitDistance) ) {
		// ignore
		return;
	}

	// get entity collision spheres
	CStaticArray<CMovingSphere> &ams = pci->ci_absSpheres;
	// get entity position
	const FLOAT3D &vPosition = penModel->en_plPlacement.pl_PositionVector;
	const FLOATmatrix3D &mRotation = penModel->en_mRotation;

	// for each sphere
	FOREACHINSTATICARRAY(ams, CMovingSphere, itms) {
		// project its center to absolute space
		FLOAT3D vCenter = itms->ms_vCenter*mRotation + vPosition;
		// if the ray hits the sphere closer than closest found hit point yet
		FLOAT fOneSphereHitDistance;
		if (RayHitsSphere(cr_vOrigin, cr_vTarget,
			vCenter, itms->ms_fR+cr_fTestR, fOneSphereHitDistance) &&
			fOneSphereHitDistance<cr_fHitDistance && fOneSphereHitDistance>-cr_fTestR) {
			// set the current entity as new hit target
			cr_fHitDistance=fOneSphereHitDistance;
			cr_penHit = penModel;
			cr_pbscBrushSector = NULL;
			cr_pbpoBrushPolygon = NULL;
		}
	}
}

void CCastRay::TestModelFull(CEntity *penModel, CModelObject &mo)
{
	// NOTE: this contains an ugly hack to simulate good trivial rejection
	// for models that have attachments that extend far off the base entity.
	// it is used only in wed, so it should not be a big problem.

	// get model's bounding box for all frames and expand it a lot
	FLOATaabbox3D boxModel;
	mo.GetAllFramesBBox(boxModel);
	boxModel.StretchByVector(mo.mo_Stretch*5.0f);
	// get center and radius of the bounding sphere in absolute space
	FLOAT fSphereRadius = boxModel.Size().Length()/2.0f;
	FLOAT3D vSphereCenter = boxModel.Center();
	vSphereCenter*=penModel->en_mRotation;
	vSphereCenter+=penModel->en_plPlacement.pl_PositionVector;

	// if the ray doesn't hit the sphere
	FLOAT fSphereHitDistance;
	if (!RayHitsSphere(cr_vOrigin, cr_vTarget,
		vSphereCenter, fSphereRadius+cr_fTestR, fSphereHitDistance) ) {
		// ignore
		return;
	}

	FLOAT fHitDistance;
	// if the ray hits the model closer than closest found hit point yet
	if (mo.PolygonHit(cl_plRay, penModel->en_plPlacement, 0/*iCurrentMip*/,
		fHitDistance)!=NULL
		&& fHitDistance<cr_fHitDistance) {
		// set the current entity as new hit target
		cr_fHitDistance=fHitDistance;
		cr_penHit = penModel;
		cr_pbscBrushSector = NULL;
		cr_pbpoBrushPolygon = NULL;
	}
}

/*
 * Test against a model entity.
 */
void CCastRay::TestModel(CEntity *penModel)
{

	// if hidden model
	if (penModel->IsFlagOn(ENF_HIDDEN)&&
		(CEntity::GetPlayerEntity(0)->IsFlagOff(ENF_SHOWHIDDEN) ||
		(CEntity::GetPlayerEntity(0)->IsFlagOn(ENF_SHOWHIDDEN)&&!penModel->IsEnemy())))//ENF_SHOWHIDDEN이면 npc는 볼 수 있다.
		return;

	// get its model
	CModelObject *pmoModel;
	if (penModel->en_RenderType!=CEntity::RT_BRUSH
	 && penModel->en_RenderType != CEntity::RT_FIELDBRUSH) {
		pmoModel=penModel->en_pmoModelObject;
	} else {
		// empty brushes are also tested as models
		pmoModel=_wrpWorldRenderPrefs.GetEmptyBrushModel();
	}
	// if there is no valid model
	if (pmoModel==NULL) {
		// don't test it
		return;
	}
	CModelObject &mo = *pmoModel;

	// if simple testing, or no testing (used when testing empty brushes)
	if (cr_ttHitModels==TT_SIMPLE || cr_ttHitModels==TT_NONE) {
		TestModelSimple(penModel, mo);
	// if collision box testing
	} else if (cr_ttHitModels==TT_COLLISIONBOX) {
		TestModelCollisionBox(penModel);
	// if full testing
	} else if (cr_ttHitModels==TT_FULL || cr_ttHitModels==TT_FULLSEETHROUGH) {
		TestModelFull(penModel, mo);
	// must be no other testing
	} else {
		ASSERT(FALSE);
	}
}

/*
 * Test against a ska model
 */ 
void CCastRay::TestSkaModel(CEntity *penModel)
{
	extern ENGINE_API INDEX g_bSlaveNoTarget; // Target culling
	
	switch (g_bSlaveNoTarget)
	{
	case 2: // 캐릭터 
		{
			if (penModel->IsCharacter())
			{
				return;
			}
		}
	case 1: // 소환수 및 펫 (토템, 몬스터 용병 추가)
		{
			if (penModel->IsSlave() || penModel->IsPet() || penModel->IsWildPet() || penModel->GetFirstExFlags()&ENF_EX1_TOTEM || penModel->GetFirstExFlags()&ENF_EX1_MONSTER_MERCENARY)
			{
				return;
			}
		}
	default: // 모두 허가
		break;
	}

	// if hidden model
	if (penModel->IsFlagOn(ENF_HIDDEN)&&
		(CEntity::GetPlayerEntity(0)->IsFlagOff(ENF_SHOWHIDDEN) ||
		(CEntity::GetPlayerEntity(0)->IsFlagOn(ENF_SHOWHIDDEN)&&!penModel->IsEnemy())))//ENF_SHOWHIDDEN이면 npc는 볼 수 있다.
		return;

	if( penModel->GetFirstExFlags() &  (ENF_EX1_CURRENT_PET | ENF_EX1_CURRENT_SLAVE) )
	{
		return;
	}

	CModelInstance &mi = *penModel->GetModelInstance();
	// if simple testing, or no testing (used when testing empty brushes)
	if (cr_ttHitModels==TT_SIMPLE || cr_ttHitModels==TT_NONE) {
		TestSkaModelSimple(penModel, mi);
	// if collision box testing
	} else if (cr_ttHitModels==TT_COLLISIONBOX) {
		TestModelCollisionBox(penModel);
	// if full testing
	} else if (cr_ttHitModels==TT_FULL || cr_ttHitModels==TT_FULLSEETHROUGH) {
		// changed to box testing
		TestSkaModelFull(penModel, mi);
	// must be no other testing
	} else {
		ASSERT(FALSE);
	}
}

void CCastRay::TestSkaModelSimple(CEntity *penModel, CModelInstance &mi)
{
	FLOATaabbox3D boxModel;
	mi.GetCurrentColisionBox(boxModel);
	boxModel.StretchByVector(mi.mi_vStretch);
	// get center and radius of the bounding sphere in absolute space
	FLOAT fSphereRadius = boxModel.Size().Length()/2.0f;
	FLOAT3D vSphereCenter = boxModel.Center();
	vSphereCenter*=penModel->en_mRotation;
	vSphereCenter+=penModel->en_plPlacement.pl_PositionVector;

	// if the ray doesn't hit the sphere
	FLOAT fSphereHitDistance;
	if (!RayHitsSphere(cr_vOrigin, cr_vTarget,
		vSphereCenter, fSphereRadius+cr_fTestR, fSphereHitDistance) ) {
		// ignore
		return;
	}

	// if the ray hits the sphere closer than closest found hit point yet
	if (fSphereHitDistance<cr_fHitDistance && fSphereHitDistance>0.0f) {
		// set the current entity as new hit target
		cr_fHitDistance=fSphereHitDistance;
		cr_penHit = penModel;
		cr_pbscBrushSector = NULL;
		cr_pbpoBrushPolygon = NULL;
	}
}

void CCastRay::TestSkaModelFull(CEntity *penModel, CModelInstance &mi)
{
	FLOATaabbox3D boxModel;
	mi.GetAllFramesBBox(boxModel);
	boxModel.StretchByVector(mi.mi_vStretch);
	// get center and radius of the bounding sphere in absolute space
	FLOAT fSphereRadius = boxModel.Size().Length()/2.0f;
	FLOAT3D vSphereCenter = boxModel.Center();
	vSphereCenter*=penModel->en_mRotation;
	vSphereCenter+=penModel->en_plPlacement.pl_PositionVector;

	// if the ray doesn't hit the sphere
	FLOAT fSphereHitDistance;
	if (!RayHitsSphere(cr_vOrigin, cr_vTarget,
		vSphereCenter, fSphereRadius+cr_fTestR, fSphereHitDistance) ) {
		// ignore
		return;
	}

	INDEX iBoneID = -1;
	FLOAT fHitDistance = cr_fHitDistance;
	if (cr_bFindBone) {
	fHitDistance = RM_TestRayCastHit(mi,penModel->en_mRotation,penModel->en_plPlacement.pl_PositionVector,
																					 cr_vOrigin,cr_vTarget,fHitDistance,&iBoneID);
	}	else {
	fHitDistance = RM_TestRayCastHit(mi,penModel->en_mRotation,penModel->en_plPlacement.pl_PositionVector,
																					 cr_vOrigin,cr_vTarget,fHitDistance,NULL);
	}


	if (fHitDistance<cr_fHitDistance/* && fTriangleHitDistance>0.0f*/) {
		// set the current entity as new hit target
		cr_fHitDistance=fHitDistance;
		cr_penHit = penModel;
		cr_pbscBrushSector = NULL;
		cr_pbpoBrushPolygon = NULL;
		
		if (cr_bFindBone) {
			cr_iBoneHit = iBoneID;
		}
	}
}

void CCastRay::TestTerrain(CEntity *penTerrain)
{
	// if hidden model
	if( penTerrain->en_ulFlags&ENF_HIDDEN) {
		// don't test
		return;
	}

	CTerrain *ptrTerrain = penTerrain->GetTerrain();
	FLOAT fHitDistance = TR_TestRayCastHit(ptrTerrain, penTerrain->en_mRotation, penTerrain->en_plPlacement.pl_PositionVector,
																				 cr_vOrigin,cr_vTarget,cr_fHitDistance,cr_bHitTerrainInvisibleTris);

	if (fHitDistance<cr_fHitDistance && fHitDistance>0.0f) {
		// set the current entity as new hit target
		cr_fHitDistance=fHitDistance;
		cr_penHit = penTerrain;
		cr_pbscBrushSector = NULL;
		cr_pbpoBrushPolygon = NULL;
	}
}

/*
 * Test against a brush sector.
 */
void CCastRay::TestBrushSector(CBrushSector *pbscSector)
{
	// if entity is hidden
	if(pbscSector->bsc_pbmBrushMip->bm_pbrBrush->br_penEntity->en_ulFlags&ENF_HIDDEN)
	{
		// don't cast ray
		return;
	}
	// for each polygon in the sector
	FOREACHINSTATICARRAY(pbscSector->bsc_abpoPolygons, CBrushPolygon, itpoPolygon) {
		CBrushPolygon &bpoPolygon = itpoPolygon.Current();

		if (&bpoPolygon==cr_pbpoIgnore) {
			continue;
		}

		ULONG ulFlags = bpoPolygon.bpo_ulFlags;
		// if not testing recursively
		if (cr_penOrigin==NULL) {
			// if the polygon is portal
			if (ulFlags&BPOF_PORTAL) {
				extern BOOL _bShowPortalPolygon;

				// if it is translucent or selected
				if (ulFlags&(BPOF_TRANSLUCENT|BPOF_TRANSPARENT|BPOF_SELECTED)) {
					// if translucent portals should be passed through
					if (!cr_bHitTranslucentPortals) {
						// skip this polygon
						if( !_bShowPortalPolygon )
							continue;
					}
				// if it is not translucent
				} else {
					 // if portals should be passed through
					if (!cr_bHitPortals) {
						// skip this polygon
						if( !_bShowPortalPolygon )
							continue;
					}
				}
			}
			// if polygon is detail, and detail polygons are off
			extern INDEX wld_bRenderDetailPolygons;
			if ((ulFlags&BPOF_DETAILPOLYGON) && !wld_bRenderDetailPolygons) {
				// skip this polygon
				continue;
			}
		}
// 강동민 수정 시작		// 가끔 NULL로 들어오는듯...
// FIXME : 원인을 찾아서 수정할 것.
		if( !bpoPolygon.bpo_pbplPlane )
		{
			continue;
		}
// 강동민 수정 끝		// 가끔 NULL로 들어오는듯...
		// get distances of ray points from the polygon plane
		FLOAT fDistance0 = bpoPolygon.bpo_pbplPlane->bpl_plAbsolute.PointDistance(cr_vOrigin);
		FLOAT fDistance1 = bpoPolygon.bpo_pbplPlane->bpl_plAbsolute.PointDistance(cr_vTarget);

		// if the ray hits the polygon plane
		if (fDistance0>=0 && fDistance0>fDistance1) {
			// calculate fraction of line before intersection
			FLOAT fFraction = fDistance0/(fDistance0-fDistance1);
			// calculate intersection coordinate
			FLOAT3D vHitPoint = cr_vOrigin+(cr_vTarget-cr_vOrigin)*fFraction;
			// if not inside polygon's box
			if (!bpoPolygon.bpo_boxBoundingBox.TouchesSphere(vHitPoint, 0.1f)) {
				// skip this polygon
				continue;
			}

			// calculate intersection distance
			FLOAT fHitDistance = (vHitPoint-cr_vOrigin).Length();
			// if the hit point can not be new closest candidate
			if (fHitDistance>cr_fHitDistance) {
				// skip this polygon
				continue;
			}

			// find major axes of the polygon plane
			INDEX iMajorAxis1, iMajorAxis2;
			GetMajorAxesForPlane(itpoPolygon->bpo_pbplPlane->bpl_plAbsolute, iMajorAxis1, iMajorAxis2);

			// create an intersector
			CIntersector isIntersector(vHitPoint(iMajorAxis1), vHitPoint(iMajorAxis2));
			// for all edges in the polygon
			FOREACHINSTATICARRAY(bpoPolygon.bpo_abpePolygonEdges, CBrushPolygonEdge,
				itbpePolygonEdge) {
				// get edge vertices (edge direction is irrelevant here!)
				const FLOAT3D &vVertex0 = itbpePolygonEdge->bpe_pbedEdge->bed_pbvxVertex0->bvx_vAbsolute;
				const FLOAT3D &vVertex1 = itbpePolygonEdge->bpe_pbedEdge->bed_pbvxVertex1->bvx_vAbsolute;
				// pass the edge to the intersector
				isIntersector.AddEdge(
					vVertex0(iMajorAxis1), vVertex0(iMajorAxis2),
					vVertex1(iMajorAxis1), vVertex1(iMajorAxis2));
			}
			// if the polygon is intersected by the ray
			if (isIntersector.IsIntersecting()) {
				// if it is portal and testing recusively
				if ((ulFlags&cr_ulPassablePolygons) && (cr_penOrigin!=NULL)) {
					// for each sector on the other side
					{FOREACHDSTOFSRC(bpoPolygon.bpo_rsOtherSideSectors, CBrushSector, bsc_rdOtherSidePortals, pbsc)
						// add the sector
						AddSector(pbsc);
					ENDFOR}

					if( cr_bHitPortals && ulFlags&(BPOF_TRANSLUCENT|BPOF_TRANSPARENT) && !cr_bPhysical)
					{
						// remember hit coordinates
						cr_fHitDistance=fHitDistance;
						cr_penHit = pbscSector->bsc_pbmBrushMip->bm_pbrBrush->br_penEntity;
						cr_pbscBrushSector = pbscSector;
						cr_pbpoBrushPolygon = &bpoPolygon;
					}
				// if the ray just plainly hit it
				} else {
					// remember hit coordinates
					cr_fHitDistance=fHitDistance;
					cr_penHit = pbscSector->bsc_pbmBrushMip->bm_pbrBrush->br_penEntity;
					cr_pbscBrushSector = pbscSector;
					cr_pbpoBrushPolygon = &bpoPolygon;
				}
			}
		}
	}
}

/* Add a sector if needed. */
inline void CCastRay::AddSector(CBrushSector *pbsc)
{
	// if not already active and in first mip of its brush
	if ( pbsc->bsc_pbmBrushMip->IsFirstMip()
		&&!(pbsc->bsc_ulFlags&BSCF_RAYTESTED)) {
		// add it to active sectors
		_aas.Push().as_pbsc = pbsc;
		pbsc->bsc_ulFlags|=BSCF_RAYTESTED;
	}
}
/* Add all sectors of a brush. */
void CCastRay::AddAllSectorsOfBrush(CBrush3D *pbr)
{
	// get relevant mip as if in manual mip brushing mode
	CBrushMip *pbmMip = pbr->GetBrushMipByDistance(
		_wrpWorldRenderPrefs.GetManualMipBrushingFactor());

	// if it has no brush mip for that mip factor
	if (pbmMip==NULL) {
		// skip it
		return;
	}
	// for each sector in the brush mip
	FOREACHINDYNAMICARRAY(pbmMip->bm_abscSectors, CBrushSector, itbsc) {
		// add the sector
		AddSector(itbsc);
	}
}

/* Add all sectors around given entity. */
void CCastRay::AddSectorsAroundEntity(CEntity *pen)
{
	// for each zoning sector that this entity is in
	{FOREACHSRCOFDST(pen->en_rdSectors, CBrushSector, bsc_rsEntities, pbsc)
		// if part of zoning brush
		if (pbsc->bsc_pbmBrushMip->bm_pbrBrush->br_penEntity->GetRenderType()!=CEntity::RT_BRUSH) {
			// skip it
			continue;
		}
		// add the sector
		AddSector(pbsc);
	ENDFOR}
}

/* Test entire world against ray. */
void CCastRay::TestWholeWorld(CWorld *pwoWorld)
{
	// for each entity in the world
	{FOREACHINDYNAMICCONTAINER(pwoWorld->wo_cenEntities, CEntity, itenInWorld) {
		// if it is the origin of the ray
		if (itenInWorld==cr_penOrigin || itenInWorld==cr_penIgnore) {
			// skip it
			continue;
		}

		// if it is a brush and testing against brushes is disabled
		if( (itenInWorld->en_RenderType == CEntity::RT_BRUSH ||
				 itenInWorld->en_RenderType == CEntity::RT_FIELDBRUSH) && 
				 !cr_bHitBrushes) {
			// skip it
			continue;
		}

		// if it is a model and testing against models is enabled
		if(((itenInWorld->en_RenderType == CEntity::RT_MODEL
			||(itenInWorld->en_RenderType == CEntity::RT_EDITORMODEL
				 && _wrpWorldRenderPrefs.IsEditorModelsOn()))
			&& cr_ttHitModels != TT_NONE)
		//  and if cast type is TT_FULL_SEETROUGH then model is not
		//  ENF_SEETROUGH
			&& !((cr_ttHitModels == TT_FULLSEETHROUGH || cr_ttHitModels == TT_COLLISIONBOX) &&
					 (itenInWorld->en_ulFlags&ENF_SEETHROUGH))) {
			// test it against the model entity
			TestModel(itenInWorld);
		// if it is a ska model
		} else if(((itenInWorld->en_RenderType == CEntity::RT_SKAMODEL
			||(itenInWorld->en_RenderType == CEntity::RT_SKAEDITORMODEL
				 && _wrpWorldRenderPrefs.IsEditorModelsOn()))
			&& cr_ttHitModels != TT_NONE)
		//  and if cast type is TT_FULL_SEETROUGH then model is not
		//  ENF_SEETROUGH
			&& !((cr_ttHitModels == TT_FULLSEETHROUGH || cr_ttHitModels == TT_COLLISIONBOX) &&
					 (itenInWorld->en_ulFlags&ENF_SEETHROUGH))) {
			TestSkaModel(itenInWorld);
		} else if (itenInWorld->en_RenderType == CEntity::RT_TERRAIN) {
			TestTerrain(itenInWorld);
		// if it is a brush
		} else if (itenInWorld->en_RenderType == CEntity::RT_BRUSH ||
			(itenInWorld->en_RenderType == CEntity::RT_FIELDBRUSH
			&&_wrpWorldRenderPrefs.IsFieldBrushesOn() && cr_bHitFields)) {
			// get its brush
			CBrush3D &brBrush = *itenInWorld->en_pbrBrush;

			// get relevant mip as if in manual mip brushing mode
			CBrushMip *pbmMip = brBrush.GetBrushMipByDistance(
				_wrpWorldRenderPrefs.GetManualMipBrushingFactor());

			// if it has no brush mip for that mip factor
			if (pbmMip==NULL) {
				// skip it
				continue;
			}

			// if it has zero sectors
			if (pbmMip->bm_abscSectors.Count()==0){
				// test it against the model entity
				TestModel(itenInWorld);

			// if it has some sectors
			} else {
				// for each sector in the brush mip
				FOREACHINDYNAMICARRAY(pbmMip->bm_abscSectors, CBrushSector, itbsc) {
					// if the sector is not hidden
					if (!(itbsc->bsc_ulFlags & BSCF_HIDDEN)) {
						// test the ray against the sector
						TestBrushSector(itbsc);
					}
				}
			}
		}
	}}
}

/* Test active sectors recusively. */
void CCastRay::TestThroughSectors(void)
{
	// for each active sector (sectors are added during iteration!)
	for(INDEX ias=0; ias<_aas.Count(); ias++) {
		CBrushSector *pbsc = _aas[ias].as_pbsc;
		// test the ray against the sector
		TestBrushSector(pbsc);
		// for each entity in the sector
		{FOREACHDSTOFSRC(pbsc->bsc_rsEntities, CEntity, en_rdSectors, pen)
			// if it is the origin of the ray
			if (pen==cr_penOrigin || pen==cr_penIgnore) {
				// skip it
				continue;
			}
			// if it is a model and testing against models is enabled
			if(((pen->en_RenderType == CEntity::RT_MODEL
				||(pen->en_RenderType == CEntity::RT_EDITORMODEL
					 && _wrpWorldRenderPrefs.IsEditorModelsOn()))
				&& cr_ttHitModels != TT_NONE)
			//  and if cast type is TT_FULL_SEETROUGH then model is not
			//  ENF_SEETROUGH
				&& !((cr_ttHitModels == TT_FULLSEETHROUGH || cr_ttHitModels == TT_COLLISIONBOX) &&
						 (pen->en_ulFlags&ENF_SEETHROUGH))) {
				// test it against the model entity
				TestModel(pen);
			// if is is a ska model
			} else if(((pen->en_RenderType == CEntity::RT_SKAMODEL
				||(pen->en_RenderType == CEntity::RT_SKAEDITORMODEL
					 && _wrpWorldRenderPrefs.IsEditorModelsOn()))
				&& cr_ttHitModels != TT_NONE)
			//  and if cast type is TT_FULL_SEETROUGH then model is not
			//  ENF_SEETROUGH
				&& !((cr_ttHitModels == TT_FULLSEETHROUGH || cr_ttHitModels == TT_COLLISIONBOX) &&
						 (pen->en_ulFlags&ENF_SEETHROUGH))) {
				// test it against the ska model entity
				TestSkaModel(pen);
			// if it is a terrain
			} else if( pen->en_RenderType == CEntity::RT_TERRAIN) {
				CTerrain *ptrTerrain = pen->GetTerrain();
				ASSERT(ptrTerrain!=NULL);
				// if terrain hasn't allready been tested
				if(!ptrTerrain->tr_lnInActiveTerrains.IsLinked()) {
					// test it now and add it to list of tested terrains
					TestTerrain(pen);
					_lhTestedTerrains.AddTail(ptrTerrain->tr_lnInActiveTerrains);
				}
			// if it is a non-hidden brush
			} else if ( (pen->en_RenderType == CEntity::RT_BRUSH) &&
									!(pen->en_ulFlags&ENF_HIDDEN) ) {
				extern INDEX phy_bNonZoningClipRay;
				if (phy_bNonZoningClipRay) {
					// get its brush
					CBrush3D &brBrush = *pen->en_pbrBrush;
					// add all sectors in the brush
					AddAllSectorsOfBrush(&brBrush);
				}
			}
		ENDFOR}
	}

	// for all tested terrains
	{FORDELETELIST(CTerrain, tr_lnInActiveTerrains, _lhTestedTerrains, ittr) {
		// remove it from list
		ittr->tr_lnInActiveTerrains.Remove();
	}}
	ASSERT(_lhTestedTerrains.IsEmpty());
}

/*
 * Do the ray casting.
 */
void CCastRay::Cast(CWorld *pwoWorld)
{
	// setup stat timers
	const BOOL bMainLoopTimer = _sfStats.CheckTimer(CStatForm::STI_MAINLOOP);
	if( bMainLoopTimer) _sfStats.StopTimer(CStatForm::STI_MAINLOOP);
	_sfStats.StartTimer(CStatForm::STI_RAYCAST);
	
	// initially no polygon is found
	cr_pbpoBrushPolygon= NULL;
	cr_pbscBrushSector = NULL;
	cr_penHit = NULL;
	if (cr_bPhysical) 
	{
//강동민 수정 시작 물 퍼포먼스 작업
		//cr_ulPassablePolygons = BPOF_PASSABLE|BPOF_SHOOTTHRU;
		cr_ulPassablePolygons = BPOF_PASSABLE;
//강동민 수정 끝 물 퍼포먼스 작업
	} 
	else 
	{
		cr_ulPassablePolygons = BPOF_PORTAL|BPOF_OCCLUDER;
	}
	
	// if origin entity is given
	if (cr_penOrigin!=NULL) {
		// if not continuing
		if (_aas.Count()==0) {
			// add all sectors around it
			AddSectorsAroundEntity(cr_penOrigin);
		}
		// test all sectors recursively
		TestThroughSectors();
		// if there is no origin entity
	} else {
		// test entire world against ray
		TestWholeWorld(pwoWorld);
	}
	
	// calculate the hit point from the hit distance
	cr_vHit = cr_vOrigin + (cr_vTarget-cr_vOrigin).Normalize()*cr_fHitDistance;
	
	// done with timing
	_sfStats.StopTimer(CStatForm::STI_RAYCAST);
	if( bMainLoopTimer) _sfStats.StartTimer(CStatForm::STI_MAINLOOP);
}


/*
 * Continue cast.
 */
void CCastRay::ContinueCast(CWorld *pwoWorld)
{
	cr_pbpoIgnore = cr_pbpoBrushPolygon;
	if( cr_penHit->GetRenderType()==CEntity::RT_MODEL
	 || cr_penHit->GetRenderType()==CEntity::RT_SKAMODEL) {
		cr_penIgnore = cr_penHit;
	}

	cr_vOrigin = cr_vHit;
	cl_plRay.pl_PositionVector = cr_vOrigin;
	cr_fHitDistance = (cr_vTarget-cr_vOrigin).Length() + EPSILON;
	Cast(pwoWorld);
}

/////////////////////////////////////////////////////////////////////
/*
 * Cast a ray and see what it hits.
 */
void CWorld::CastRay(CCastRay &crRay)
{
	crRay.Cast(this);
}
/*
 * Continue to cast already cast ray
 */
void CWorld::ContinueCast(CCastRay &crRay)
{
	crRay.ContinueCast(this);
}

// yjpark |<--
/*
 * Cast a ray for terrain shadow
 */
BOOL CCastRay::CastForTerrainShadow( CWorld *pwoWorld, BOOL bHitSka, BOOL bHitMdl, BOOL bHitBrush, BOOL bHitTerrain )
{
	// Direction of ray
	FLOAT3D	vDir = cr_vTarget - cr_vOrigin;
	
	// For each entity in the world
	FOREACHINDYNAMICCONTAINER( pwoWorld->wo_cenEntities, CEntity, itenInWorld )
	{
		// Skip
		if( itenInWorld->en_RenderType == CEntity::RT_FIELDBRUSH ||
			itenInWorld->en_RenderType == CEntity::RT_EDITORMODEL ||
			itenInWorld->en_RenderType == CEntity::RT_SKAEDITORMODEL )
			continue;

		// If background, invisible or hidden
		if( ( itenInWorld->en_ulFlags & ENF_BACKGROUND ) ||
			( itenInWorld->en_ulFlags & ENF_INVISIBLE ) ||
			( itenInWorld->en_ulFlags & ENF_HIDDEN ) ||
			( itenInWorld->en_ulFlags & ENF_NICEWATER ) )
			continue;

		// If it is a model
		if( itenInWorld->en_RenderType == CEntity::RT_MODEL )
		{
			if( !bHitMdl )
				continue;

			if( !( itenInWorld->en_ulExtraFlags1 & ENF_EX1_TERRAINSHADOW ) )
				continue;

			//if( TestModelForTerrainShadow( itenInWorld ) )
			//	return TRUE;
		}
		// If it is a ska model
		else if( itenInWorld->en_RenderType == CEntity::RT_SKAMODEL )
		{
			if( !bHitSka )
				continue;

			if( !( itenInWorld->en_ulExtraFlags1 & ENF_EX1_TERRAINSHADOW ) )
				continue;

			FLOAT3D	vDist = cr_vOrigin - itenInWorld->en_plPlacement.pl_PositionVector;
			if( vDist.Magnitude() > 900.0f )
				continue;

			if( TestSkaModelForTerrainShadow( itenInWorld, vDir ) )
				return TRUE;
		}
		// If it is a brush
		else if( itenInWorld->en_RenderType == CEntity::RT_BRUSH )
		{
			if( !bHitBrush )
				continue;

			// Get its brush
			CBrush3D	&brBrush = *itenInWorld->en_pbrBrush;

			// Get first mip
			CBrushMip *pbmMip = brBrush.GetFirstMip();

			// If it has no brush mip
			if( pbmMip == NULL )
				continue;

			// If it has zero sectors
			if( pbmMip->bm_abscSectors.Count() == 0 )
				continue;
			// If it has some sectors
			else
			{
				// For each sector in the brush mip
				FOREACHINDYNAMICARRAY( pbmMip->bm_abscSectors, CBrushSector, itbsc )
				{
					// If the sector is not hidden
					if( !( itbsc->bsc_ulFlags & BSCF_HIDDEN ) )
					{
						// Test the ray against the sector
						if( TestBrushSectorForTerrainShadow( itbsc, vDir ) )
							return TRUE;
					}
				}
			}
		}
		// If it is a terrain
		else if( itenInWorld->en_RenderType == CEntity::RT_TERRAIN )
		{
			if( !bHitTerrain )
				continue;

			//if( TestTerrainForTerrainShadow( itenInWorld ) )
			//	return TRUE;
		}
	}

	return FALSE;
}

BOOL CCastRay::TestSkaModelForTerrainShadow( CEntity *penModel, FLOAT3D &rDir )
{
	CModelInstance	&mi = *penModel->GetModelInstance();

	FLOATaabbox3D	boxModel;
	mi.GetAllFramesBBox( boxModel );
	boxModel.StretchByVector( mi.mi_vStretch );
	
	// Get center and radius of the bounding sphere in absolute space
	FLOAT	fSphereRadius = boxModel.Size().Length() / 2.0f;
	FLOAT3D	vSphereCenter = boxModel.Center();
	vSphereCenter *= penModel->en_mRotation;
	vSphereCenter += penModel->en_plPlacement.pl_PositionVector;

	// If the ray doesn't hit the sphere
	FLOAT	fSphereHitDistance;
	if( !RayHitsSphere( cr_vOrigin, cr_vTarget,
						vSphereCenter, fSphereRadius + cr_fTestR, fSphereHitDistance ) )
		return FALSE;

	if( RM_TestRayCastHitForTerrainShadow( mi, penModel->en_mRotation, penModel->en_plPlacement.pl_PositionVector,
											//cr_vOrigin, rDir ) )
											cr_vTarget, -rDir ) )
		return TRUE;

	return FALSE;
}

BOOL CCastRay::TestBrushSectorForTerrainShadow( CBrushSector *pbscSector, FLOAT3D &rDir )
{
	FLOATaabbox3D	boxSector = pbscSector->bsc_boxBoundingBox;
	
	// Get center and radius of the bounding sphere in absolute space
	FLOAT	fSphereRadius = boxSector.Size().Length() / 2.0f;
	FLOAT3D	vSphereCenter = boxSector.Center();

	// If the ray doesn't hit the sphere
	FLOAT	fSphereHitDistance;
	if( !RayHitsSphere( cr_vOrigin, cr_vTarget,
						vSphereCenter, fSphereRadius + cr_fTestR, fSphereHitDistance ) )
		return FALSE;

	// For each polygon in the sector
	FOREACHINSTATICARRAY( pbscSector->bsc_abpoPolygons, CBrushPolygon, itpoPolygon )
	{
		CBrushPolygon	&bpoPolygon = itpoPolygon.Current();

		ULONG ulFlags = bpoPolygon.bpo_ulFlags;
		if( ( ulFlags & BPOF_INVISIBLE ) || ( ulFlags & BPOF_PORTAL ) )
			continue;

		INDEX	iV0, iV1, iV2;
		INDEX	ctIndices = bpoPolygon.bpo_aiTriangleElements.Count();
		for( INDEX iIndex = 0; iIndex < ctIndices; iIndex += 3 )
		{
			iV0 = bpoPolygon.bpo_aiTriangleElements[iIndex];
			iV1 = bpoPolygon.bpo_aiTriangleElements[iIndex + 1];
			iV2 = bpoPolygon.bpo_aiTriangleElements[iIndex + 2];
			FLOAT3D	vP0 = bpoPolygon.bpo_apbvxTriangleVertices[iV0]->bvx_vAbsolute;
			FLOAT3D	vP1 = bpoPolygon.bpo_apbvxTriangleVertices[iV1]->bvx_vAbsolute;
			FLOAT3D	vP2 = bpoPolygon.bpo_apbvxTriangleVertices[iV2]->bvx_vAbsolute;

			FLOAT	fT;
			if( RayHitsTriangle( cr_vOrigin, rDir, vP0, vP1, vP2, &fT ) )
				return TRUE;
		}
	}

	return FALSE;
}
// yjpark     -->|