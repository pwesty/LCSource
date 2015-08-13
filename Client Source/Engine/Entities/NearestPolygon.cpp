#include "stdh.h"

#include <Engine/Entities/Entity.h>
#include <Engine/Brushes/Brush.h>

#include <Engine/Templates/DynamicArray.cpp>
//강동민 수정 시작 테스트 클라이언트 작업	06.10
#include <Engine/Templates/DynamicContainer.cpp>
//강동민 수정 끝 테스트 클라이언트 작업		06.10
#include <Engine/Templates/StaticArray.cpp>
#include <Engine/Templates/StaticStackArray.cpp>
#include <Engine/Math/Geometry.inl>
#include <Engine/Math/Clipping.inl>

static CEntity *_pen;
static FLOAT3D _vHandle;
static CBrushPolygon *_pbpoNear;
static FLOAT _fNearDistance;
static FLOAT3D _vNearPoint;
static FLOATplane3D _plPlane;

class CActiveSector {
public:
	CBrushSector *as_pbsc;
	void Clear(void) {};
};

static CStaticStackArray<CActiveSector> _aas;

/* Add a sector if needed. */
static void AddSector(CBrushSector *pbsc)
{
	// if not already active and in first mip of its brush
	if ( pbsc->bsc_pbmBrushMip->IsFirstMip()
		&&!(pbsc->bsc_ulFlags&BSCF_NEARTESTED)) {
		// add it to active sectors
		_aas.Push().as_pbsc = pbsc;
		pbsc->bsc_ulFlags|=BSCF_NEARTESTED;
	}
}
/* Add all sectors of a brush. */
static void AddAllSectorsOfBrush(CBrush3D *pbr)
{
	// get first mip
	CBrushMip *pbmMip = pbr->GetFirstMip();
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

//강동민 수정 시작 접속 시퀀스 작업	06.01
void SearchThroughSectors(float fRadius, CDynamicContainer<CBrushPolygon> &dcBrushPolygon)
{
	// for each active sector (sectors are added during iteration!)
	// 활성화된 섹터에 대해서...
	for(INDEX ias=0; ias<_aas.Count(); ias++) 
	{
		CBrushSector *pbsc = _aas[ias].as_pbsc;
		//CBrushSector *pbsc = _pen->GetSectorFromPoint(_vHandle);
		if(pbsc == NULL)	return;

		//_pen->en_rdSectors;
		INDEX iCount = pbsc->bsc_abpoPolygons.Count();

		// for each polygon in the sector
		// 섹터내의 모든 폴리곤에 대해서...
		{FOREACHINSTATICARRAY(pbsc->bsc_abpoPolygons, CBrushPolygon, itbpo) 
		{
			CBrushPolygon &bpo = *itbpo;

			if(!bpo.bpo_pspoScreenPolygon)
				continue;

			// if it is not a wall
			if (bpo.bpo_ulFlags&BPOF_PORTAL) 
			{
				// skip it
				continue;
			}
			else if (bpo.bpo_ulFlags&BPOF_INVISIBLE)
			{
				continue;
			}
			else if (bpo.bpo_ulFlags&BPOF_OCCLUDER)
			{
				continue;
			}
			else if( bpo.bpo_ulFlags&BPOF_WATER)
			{
				continue;
			}

			ASSERT( bpo.bpo_pbplPlane != NULL && "Invalid Brush Polygon");
			if(!bpo.bpo_pbplPlane)
				continue;

			// 절대 공간상의 평면의 방정식을 얻음.
			const FLOATplane3D &plPolygon = bpo.bpo_pbplPlane->bpl_plAbsolute;

			// find distance of the polygon plane from the handle
			// 엔티티로부터 평면까지의 거리 계산.
			FLOAT fDistance = plPolygon.PointDistance(_vHandle);
			
			// 구의 범위를 벗어난다면, 제외.
			//if (fDistance<0.0f || fDistance>_fNearDistance) 
			if (fDistance<0.0f || fDistance>fRadius) 
			//if (fDistance>fRadius)
			{
				// skip it
				continue;
			}

			// find projection of handle to the polygon plane
			// 엔티티의 위치를 폴리곤 평면으로 투영함.
			FLOAT3D vOnPlane = plPolygon.ProjectPoint(_vHandle);

			// if it is not in the bounding box of polygon
			// 찾은 점이 폴리곤의 바운딩 박스내에 없다면...
			const FLOATaabbox3D &boxPolygon = bpo.bpo_boxBoundingBox;			
			const FLOAT EPSILON = 0.01f;
			if (
				(boxPolygon.Min()(1)-EPSILON>vOnPlane(1)) ||
				(boxPolygon.Max()(1)+EPSILON<vOnPlane(1)) ||
				(boxPolygon.Min()(2)-EPSILON>vOnPlane(2)) ||
				(boxPolygon.Max()(2)+EPSILON<vOnPlane(2)) ||
				(boxPolygon.Min()(3)-EPSILON>vOnPlane(3)) ||
				(boxPolygon.Max()(3)+EPSILON<vOnPlane(3))) 
			{
				// skip it
				continue;
			}
			dcBrushPolygon.Add(&bpo);
		}}

		// for each entity in the sector
		{FOREACHDSTOFSRC(pbsc->bsc_rsEntities, CEntity, en_rdSectors, pen)
			// if it is a brush
			if (pen->en_RenderType == CEntity::RT_BRUSH) 
			{
				// get its brush
				CBrush3D &brBrush = *pen->en_pbrBrush;
				// add all sectors in the brush
				AddAllSectorsOfBrush(&brBrush);
			}
		ENDFOR}
	}
}
//강동민 수정 끝 접속 시퀀스 작업	06.01

void SearchThroughSectors(void)
{
	// for each active sector (sectors are added during iteration!)
	for(INDEX ias=0; ias<_aas.Count(); ias++) {
		CBrushSector *pbsc = _aas[ias].as_pbsc;
		// for each polygon in the sector
		{FOREACHINSTATICARRAY(pbsc->bsc_abpoPolygons, CBrushPolygon, itbpo) {
			CBrushPolygon &bpo = *itbpo;
			// if it is not a wall
			if (bpo.bpo_ulFlags&BPOF_PORTAL) {
				// skip it
				continue;
			}

			ASSERT( bpo.bpo_pbplPlane != NULL && "Invalid Brush Polygon");
			if(!bpo.bpo_pbplPlane)
				continue;

			const FLOATplane3D &plPolygon = bpo.bpo_pbplPlane->bpl_plAbsolute;
			// find distance of the polygon plane from the handle
			FLOAT fDistance = plPolygon.PointDistance(_vHandle);
			// if it is behind the plane or further than nearest found
			if (fDistance<0.0f || fDistance>_fNearDistance) {
				// skip it
				continue;
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
				continue;
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
				continue;
			}

			// remember the polygon
			_pbpoNear = &bpo;
			_fNearDistance = fDistance;
			_vNearPoint = vOnPlane;
		}}

		// for each entity in the sector
		{FOREACHDSTOFSRC(pbsc->bsc_rsEntities, CEntity, en_rdSectors, pen)
			// if it is a brush
			if (pen->en_RenderType == CEntity::RT_BRUSH) {
				// get its brush
				CBrush3D &brBrush = *pen->en_pbrBrush;
				// add all sectors in the brush
				AddAllSectorsOfBrush(&brBrush);
			}
		ENDFOR}
	}
}

/* Get nearest position of nearest brush polygon to this entity if available. */
// use:
// ->bpo_pbscSector->bsc_pbmBrushMip->bm_pbrBrush->br_penEntity
// to get the entity
CBrushPolygon *CEntity::GetNearestPolygon(
	FLOAT3D &vPoint, FLOATplane3D &plPlane, FLOAT &fDistanceToEdge)
{
	_pen = this;
	// take reference point at handle of the model entity
	_vHandle = en_plPlacement.pl_PositionVector;

	// start infinitely far away
	_pbpoNear = NULL;
	_fNearDistance = UpperLimit(1.0f);

	// for each zoning sector that this entity is in
	{FOREACHSRCOFDST(en_rdSectors, CBrushSector, bsc_rsEntities, pbsc)
		// add the sector
		AddSector(pbsc);
	ENDFOR}

	// start the search
	SearchThroughSectors();

	// for each active sector
	for(INDEX ias=0; ias<_aas.Count(); ias++) {
		// mark it as inactive
		_aas[ias].as_pbsc->bsc_ulFlags&=~BSCF_NEARTESTED;
	}
	_aas.PopAll();

	// if there is some polygon found
	if( _pbpoNear!=NULL) {
		// return info
		plPlane = _pbpoNear->bpo_pbplPlane->bpl_plAbsolute;
		vPoint = _vNearPoint;
		fDistanceToEdge = _pbpoNear->GetDistanceFromEdges(_vNearPoint);
		return _pbpoNear;
	// if none is found
	} else {
		// return failure
		return NULL;
	}
}

//강동민 수정 시작 접속 시퀀스 작업	06.01
void CEntity::GetNearPolygonsInSphere(FLOAT fRadius, CDynamicContainer<CBrushPolygon> &dcBrushPolygon)
{
	_pen = this;
	// take reference point at handle of the model entity
	_vHandle = en_plPlacement.pl_PositionVector;	

	_pen->FindSectorsAroundEntity();
	//_pen->en_rdSectors;
	
	// for each zoning sector that this entity is in
	// 엔티티가 속한 섹터 찾기.
	{FOREACHSRCOFDST(en_rdSectors, CBrushSector, bsc_rsEntities, pbsc)
		if(pbsc != NULL)
			// add the sector
			AddSector(pbsc);
	ENDFOR}	

	SearchThroughSectors(fRadius, dcBrushPolygon);
	
	for(int ias=0; ias<_aas.Count(); ias++) 
	{
		// mark it as inactive
		// 비활성화상태인것처럼 플래그 설정.
		_aas[ias].as_pbsc->bsc_ulFlags&=~BSCF_NEARTESTED;
	}
	_aas.PopAll();	
}
//강동민 수정 끝 접속 시퀀스 작업	06.01
