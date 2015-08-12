#include "stdh.h"
#include <Engine/Math/Vector.h>
#include <Engine/Math/Plane.h>
#include <Engine/Math/AABBox.h>
#include <Engine/Math/Matrix12.h>
#include <Engine/Math/Clipping.inl>
#include <Engine/Math/Geometry.inl>
#include <Engine/Terrain/Terrain.h>
#include <Engine/Terrain/Terrain.hpp>
#include <Engine/Terrain/TRRayCast.hpp>
#include <Engine/Terrain/TRMisc.hpp>

#define RIGHT	  0
#define LEFT	  1
#define MIDDLE	2

static Matrix12 _mObjToAbs;
static Matrix12 _mInvObjToAbs;
static FLOATaabbox3D _bboxTerrain;

static CTerrainImp *_ptr;
static FLOAT3D _vHitBegin;
static FLOAT3D _vHitEnd;
static FLOAT   _fMinHeight;        // Min height that ray will pass through in tested quad
static FLOAT   _fMaxHeight;        // Max height that ray will pass through in tested quad
static FLOAT3D _vHitPoint;
static FLOATplane3D _plHitPlane;
static BOOL    _bHitInvisibleTriangles;

#if TR_DEBUG_RAYCASTING
CStaticStackArray<TerrainRay> _atrRays;
CStaticStackArray<SimpleTerrainTriangle> _asttRayTris;
#endif

// Test ray agains one quad on terrain (if it's visible)
static FLOAT HitCheckQuad(const PIX pixX, const PIX pixZ)
{
	FLOAT fDistance = UpperLimit(0.0f);

	// if quad is outside terrain
	if(pixX<0 || pixZ<0 || pixX>= (_ptr->tr_pixHeightMapWidth-1) || pixZ >= (_ptr->tr_pixHeightMapHeight-1)) {
		return fDistance;
	}

	ASSERT(pixX>=0 && pixZ>=0);
	ASSERT(pixX<(_ptr->tr_pixHeightMapWidth-1) && pixZ<(_ptr->tr_pixHeightMapHeight-1));

	const PIX pixHeightMapWidth = _ptr->tr_pixHeightMapWidth;
	const PIX pixEdgeMapWidth   = _ptr->tr_pixHeightMapWidth-1;
	const PIX pix               = pixX + pixZ*pixHeightMapWidth;
	const PIX pixEdge           = pixX + pixZ*pixEdgeMapWidth;
	const UWORD *puwHeightMap   = &_ptr->tr_puwHeightMap[pix];
	const UBYTE *pubMask        = &_ptr->tr_pubEdgeMap[pixEdge];

	const FLOAT3D &vStretch = _ptr->tr_vStretch;
	// const FLOAT3D vStretch = FLOAT3D(1,1,1);
	const FLOAT3D avx[4] = {
		FLOAT3D((pixX+0)*vStretch(1), puwHeightMap[                  0]*vStretch(2),(pixZ+0)*vStretch(3)),
		FLOAT3D((pixX+1)*vStretch(1), puwHeightMap[                  1]*vStretch(2),(pixZ+0)*vStretch(3)),
		FLOAT3D((pixX+0)*vStretch(1), puwHeightMap[pixHeightMapWidth+0]*vStretch(2),(pixZ+1)*vStretch(3)),
		FLOAT3D((pixX+1)*vStretch(1), puwHeightMap[pixHeightMapWidth+1]*vStretch(2),(pixZ+1)*vStretch(3))
	};

	const FLOAT fMinHeight = _fMinHeight;// * vStretch(2);
	const FLOAT fMaxHeight = _fMaxHeight;// * vStretch(2);
	UBYTE ubMask = pubMask[0];
	if(_bHitInvisibleTriangles) {
		ubMask = 255;
	}

	SLONG slIndices[6];

	// Add one quad
	INDEX ctIndices=0;
	const BOOL bFacing = pix&1;
	if(bFacing) {
		// if at least one point of triangle is above min height and bellow max height of ray and traingle is visible
		if((avx[0](2)>=fMinHeight || avx[2](2)>=fMinHeight || avx[1](2)>=fMinHeight) &&
			(avx[0](2)<=fMaxHeight || avx[2](2)<=fMaxHeight || avx[1](2)<=fMaxHeight) &&
			(ubMask == 255)) {
			slIndices[ctIndices+0] = 0;
			slIndices[ctIndices+1] = 2;
			slIndices[ctIndices+2] = 1;
			ctIndices+=3;
		}
		// if at least one point of triangle is above min height and bellow max height of ray and traingle is visible
		if((avx[1](2)>=fMinHeight || avx[2](2)>=fMinHeight || avx[3](2)>=fMinHeight) &&
			(avx[1](2)<=fMaxHeight || avx[2](2)<=fMaxHeight || avx[3](2)<=fMaxHeight) &&
			(ubMask == 255)) {
			slIndices[ctIndices+0] = 1;
			slIndices[ctIndices+1] = 2;
			slIndices[ctIndices+2] = 3;
			ctIndices+=3;
		}
	} else {
		// if at least one point of triangle is above min height and bellow max height of ray and traingle is visible
		if((avx[2](2)>=fMinHeight || avx[3](2)>=fMinHeight || avx[0](2)>=fMinHeight) &&
			(avx[2](2)<=fMaxHeight || avx[3](2)<=fMaxHeight || avx[0](2)<=fMaxHeight) &&
			(ubMask == 255)) {
			slIndices[ctIndices+0] = 2;
			slIndices[ctIndices+1] = 3;
			slIndices[ctIndices+2] = 0;
			ctIndices+=3;
		}
		// if at least one point of triangle is above min height and bellow max height of ray and traingle is visible
		if((avx[0](2)>=fMinHeight || avx[3](2)>=fMinHeight || avx[1](2)>=fMinHeight) &&
			(avx[0](2)<=fMaxHeight || avx[3](2)<=fMaxHeight || avx[1](2)<=fMaxHeight) &&
			(ubMask == 255)) {
			slIndices[ctIndices+0] = 0;
			slIndices[ctIndices+1] = 3;
			slIndices[ctIndices+2] = 1;
			ctIndices+=3;
		}
	}

#if TR_DEBUG_RAYCASTING
	{
	extern INDEX ter_iRecordRayCasting;
	if(ter_iRecordRayCasting==1) {
		for(INDEX iind=0;iind<ctIndices;iind+=3) {
			const INDEX iind0 = slIndices[iind+0];
			const INDEX iind1 = slIndices[iind+1];
			const INDEX iind2 = slIndices[iind+2];
			SimpleTerrainTriangle &stt = _asttRayTris.Push();
			stt.stt_avx[0] = avx[iind0];
			stt.stt_avx[1] = avx[iind1];
			stt.stt_avx[2] = avx[iind2];
		}
	}
	}
#endif

	// none?
	if( ctIndices==0) return fDistance;

	// for each triangle
	for(INDEX iind=0;iind<ctIndices;iind+=3) {
		const INDEX iind0 = slIndices[iind+0];
		const INDEX iind1 = slIndices[iind+1];
		const INDEX iind2 = slIndices[iind+2];
		const FLOAT3D &v0 = avx[iind0];
		const FLOAT3D &v1 = avx[iind1];
		const FLOAT3D &v2 = avx[iind2];

		const FLOATplane3D plTriPlane(v0,v1,v2);
		const FLOAT fDistance0 = plTriPlane.PointDistance(_vHitBegin);
	  const FLOAT fDistance1 = plTriPlane.PointDistance(_vHitEnd);

		// if the ray hits the polygon plane
	  if(fDistance0>=0 && fDistance0>=fDistance1) {
		  // calculate fraction of line before intersection
		  FLOAT fFraction = 0.0f;
		  if ( fDistance0 != fDistance1 ) {
			fFraction = fDistance0/(fDistance0-fDistance1);
		  }
		  // calculate intersection coordinate
		  FLOAT3D vHitPoint = _vHitBegin+(_vHitEnd-_vHitBegin)*fFraction;
		  // calculate intersection distance
		  FLOAT fHitDistance = (vHitPoint-_vHitBegin).Length();
		  // if the hit point can not be new closest candidate
		  if (fHitDistance>fDistance) {
			  // skip this triangle
			  continue;
		  }

			// find major axes of the polygon plane
			INDEX iMajorAxis1, iMajorAxis2;
			GetMajorAxesForPlane(plTriPlane, iMajorAxis1, iMajorAxis2);

			// create an intersector
			CIntersector isIntersector(vHitPoint(iMajorAxis1), vHitPoint(iMajorAxis2));

			// check intersections for all three edges of the polygon
			isIntersector.AddEdge(
					v0(iMajorAxis1), v0(iMajorAxis2),
					v1(iMajorAxis1), v1(iMajorAxis2));
			isIntersector.AddEdge(
					v1(iMajorAxis1), v1(iMajorAxis2),
					v2(iMajorAxis1), v2(iMajorAxis2));
			isIntersector.AddEdge(
					v2(iMajorAxis1), v2(iMajorAxis2),
					v0(iMajorAxis1), v0(iMajorAxis2));

			// if the polygon is intersected by the ray, and it is the closest intersection so far
			if (isIntersector.IsIntersecting() && (fHitDistance < fDistance)) {
				// remember hit coordinates
				if(fHitDistance<fDistance) {
				  fDistance = fHitDistance;
					_vHitPoint = vHitPoint;
					/*
					_vHitPoint(1) *= _ptr->tr_vStretch(1);
					_vHitPoint(2) *= _ptr->tr_vStretch(2);
					_vHitPoint(3) *= _ptr->tr_vStretch(3);
					*/
					_plHitPlane = plTriPlane;
				}
			}
		}
	}
	return fDistance;
}

// Test all quads in ray direction and return exact hit location
static FLOAT GetExactHitLocation(CTerrainImp *ptr, const FLOAT3D &vHitBegin,
																 const FLOAT3D &vHitEnd, const FLOAT fOldDistance)
{
	_ptr = ptr;
	_vHitBegin = vHitBegin;
	_vHitEnd = vHitEnd;

	const FLOAT3D &vStretch = ptr->tr_vStretch;
	const FLOAT fX0 = vHitBegin(1) / vStretch(1);
	const FLOAT fY0 = vHitBegin(3) / vStretch(3);
	const FLOAT fH0 = vHitBegin(2) ;//* vStretch(2);
	const FLOAT fX1 = vHitEnd(1)   / vStretch(1);
	const FLOAT fY1 = vHitEnd(3)   / vStretch(3);
	const FLOAT fH1 = vHitEnd(2)   ;//* vStretch(2);

	const FLOAT fDeltaX = Abs(fX1-fX0);
	const FLOAT fDeltaY = Abs(fY1-fY0);
	FLOAT fIterator;

	if(fDeltaX>fDeltaY) {
		fIterator = fDeltaX;
	} else {
		fIterator = fDeltaY;
	}
	if(fIterator==0) {
		fIterator = 0.01f;
	}

	// const FLOAT fOneOverIter = 1.0f / fIterator;
	const FLOAT fStepX = (fX1-fX0) / fIterator;
	const FLOAT fStepY = (fY1-fY0) / fIterator;
	const FLOAT fStepH = (fH1-fH0) / fIterator;
	const FLOAT fEpsilonH = Abs(fStepH);

	FLOAT fX;
	FLOAT fY;
	FLOAT fH;

	// calculate prestep
	if(fDeltaX>fDeltaY) {
		if(fX0<fX1) {
			fX = ceil(fX0);
			fY = fY0 + (fX-fX0)*fStepY;
			fH = fH0 + (fX-fX0)*fStepH;
		} else {
			fX = floor(fX0);
			fY = fY0 + (fX0-fX)*fStepY;
			fH = fH0 + (fX0-fX)*fStepH;
		}
	} else {
		if(fY0<fY1) {
			fY = ceil(fY0);
			fX = fX0 + (fY-fY0)*fStepX;
			fH = fH0 + (fY-fY0)*fStepH;
		} else {
			fY = floor(fY0);
			fX = fX0 + (fY0-fY)*fStepX;
			fH = fH0 + (fY0-fY)*fStepH;
		}
	}

	// Chech quad where ray starts
	_fMinHeight = vHitBegin(2)-fEpsilonH;
	_fMaxHeight = vHitBegin(2)+fEpsilonH;
	const FLOAT fDistanceStart = HitCheckQuad(floor(fX0),floor(fY0));
	if(fDistanceStart<fOldDistance) {
		return fDistanceStart;
	}

	// for each iteration
//안태훈 수정 시작	//(For Performance)(0.2)
	INDEX ctit = ceil(fIterator);
	//loop unrolling, 효과는 미지수.
#	define INNER_LOOP \
	{\
		PIX pixX(fX);\
		PIX pixY(fY);\
		\
		FLOAT fDistance0;\
		FLOAT fDistance1;\
		/* Check first quad */\
		_fMinHeight = fH-fEpsilonH;\
		_fMaxHeight = fH+fEpsilonH;\
		fDistance0 = HitCheckQuad(pixX,pixY);\
		/* if iterating by x */\
		if(fDeltaX>fDeltaY) {\
			/* check left quad */\
			fDistance1 = HitCheckQuad(pixX-1,pixY);\
		} else {\
			/* check upper quad */\
			fDistance1 = HitCheckQuad(pixX,pixY-1);\
		}\
		/* find closer of two quads */\
		if(fDistance1<fDistance0) {\
			fDistance0 = fDistance1;\
		}\
		/* if distance is closer than old distance */\
		if(fDistance0<fOldDistance) {\
			/* return distance */\
			return fDistance0;\
		}\
		\
		fX+=fStepX;\
		fY+=fStepY;\
		fH+=fStepH;\
	}
/* define INNER_LOOP() end */

	INDEX ctit16 = ctit & 0xFFFFFFF0;
	INDEX	iit;
	for( iit = 0; iit < ctit16; iit+=16 )
	{
		INNER_LOOP; INNER_LOOP; INNER_LOOP; INNER_LOOP;
		INNER_LOOP; INNER_LOOP; INNER_LOOP; INNER_LOOP;
		INNER_LOOP; INNER_LOOP; INNER_LOOP; INNER_LOOP;
		INNER_LOOP; INNER_LOOP; INNER_LOOP; INNER_LOOP;
	}
	for( iit = ctit16; iit < ctit; ++iit )
	{
		INNER_LOOP;
	}
#undef INNER_LOOP
//안태훈 수정 끝	//(For Performance)(0.2)

	// Chech quad where ray ends
	_fMinHeight = vHitEnd(2)-fEpsilonH;
	_fMaxHeight = vHitEnd(2)+fEpsilonH;
	FLOAT fDistanceEnd = HitCheckQuad(floor(fX1),floor(fY1));
	if(fDistanceEnd<fOldDistance) {
		return fDistanceEnd;
	}

	// no hit
	return UpperLimit(0.0f);
}

// Check if ray hits aabbox and return coords where ray enter and exit the box
static BOOL HitAABBox(const FLOAT3D &vOrigin, const FLOAT3D &vTarget, FLOAT3D &vHitBegin,
											FLOAT3D &vHitEnd, const FLOATaabbox3D &bbox) 
{
	const FLOAT3D vDir = (vTarget - vOrigin).Normalize();
	const FLOAT3D vMin = bbox.minvect;
	const FLOAT3D vMax = bbox.maxvect;
	FLOAT3D vBeginCandidatePlane;
	FLOAT3D vEndCandidatePlane;
	FLOAT3D vBeginTDistance;
	FLOAT3D vEndTDistance;
	INDEX   iOriginSide[3];
	BOOL    bOriginInside = TRUE;

	INDEX i;

	// Find candidate planes
	for(i=1;i<4;i++) {
		// Check begining of ray
		if(vOrigin(i) < vMin(i)) {
			vBeginCandidatePlane(i) = vMin(i);
			vEndCandidatePlane(i)   = vMax(i);
			bOriginInside = FALSE;
			iOriginSide[i-1] = LEFT;
		} else if(vOrigin(i) > vMax(i)) {
			vBeginCandidatePlane(i) = vMax(i);
			vEndCandidatePlane(i)   = vMin(i);
			bOriginInside = FALSE;
			iOriginSide[i-1] = RIGHT;
		} else {
			iOriginSide[i-1] = MIDDLE;
			if(vDir(i)>0.0f) {
				vEndCandidatePlane(i) = vMax(i);
			} else {
				vEndCandidatePlane(i) = vMin(i);
			}
		}
	}

	// Calculate T distances to candidate planes
	for(i=1;i<4;i++) {
		if(iOriginSide[i-1]!=MIDDLE && vDir(i)!=0.0f) {
			vBeginTDistance(i) = (vBeginCandidatePlane(i)-vOrigin(i)) / vDir(i);
		} else {
			vBeginTDistance(i) = -1.0f;
		}
		if(vDir(i)!=0.0f) {
			vEndTDistance(i) = (vEndCandidatePlane(i)-vOrigin(i)) / vDir(i);
		} else {
			vEndTDistance(i) = -1.0f;
		}
	}

	// Get largest of the T distances for final choice of intersection
	INDEX iBeginMaxT = 1;
	INDEX iEndMinT = 1;
	for(i=2;i<4;i++) {
		if(vBeginTDistance(i) > vBeginTDistance(iBeginMaxT)) {
			iBeginMaxT = i;
		}
		if(vEndTDistance(i)>=0.0f && (vEndTDistance(iEndMinT)<0.0f || vEndTDistance(i) < vEndTDistance(iEndMinT)) ) {
			iEndMinT = i;
		}
	}

	// if origin inside box
	if(bOriginInside) {
		// Begining of ray is origin point
		vHitBegin = vOrigin;
	// else 
	} else {
	  // Check final candidate actually inside box
		if(vBeginTDistance(iBeginMaxT)<0.0f) {
			return FALSE;
		}
		if(vEndTDistance(iEndMinT)<0.0f) {
			return FALSE;
		}

		// Calculate point where ray enter box
		for(i=1;i<4;i++) {
		  if(iBeginMaxT != i) {
			  vHitBegin(i) = vOrigin(i) + vBeginTDistance(iBeginMaxT) * vDir(i);
				if(vHitBegin(i) < vMin(i) || vHitBegin(i) > vMax(i)) {
					return FALSE;
				}
		  } else {
			  vHitBegin(i) = vBeginCandidatePlane(i);
		  }
		}
	}

	// Caclulate point where ray exit box
	for(i=1;i<4;i++) {
		if(iEndMinT != i) {
			vHitEnd(i) = vOrigin(i) + vEndTDistance(iEndMinT) * vDir(i);
			if(vHitEnd(i) < vMin(i) || vHitEnd(i) > vMax(i)) {
				// no ray exit point !?
				ASSERT(FALSE);
			}
		} else {
			vHitEnd(i) = vEndCandidatePlane(i);
		}
	}

	return TRUE;
}

static FLOAT TestRayCastHit(CTerrainImp *ptr, const FLOAT3D &vOrigin, const FLOAT3D &vTarget, const FLOAT fOldDistance)
{
	FLOAT3D vHitBegin;
	FLOAT3D vHitEnd;
	
	// Trash hit point 
	_vHitPoint = FLOAT3D(UpperLimit(0.0f),UpperLimit(0.0f),UpperLimit(0.0f));

	FLOAT fDistance = UpperLimit(0.0f);
	if(HitAABBox(vOrigin,vTarget,vHitBegin,vHitEnd,_bboxTerrain)) {
		// if begin and and points are same 
		if(vHitBegin==vHitEnd) {
			// fake raycasting so something can actualy be hit
			vHitBegin(2) += 0.01f;
		}
		Matrix12 mInvStretch;
		const FLOAT3D &vStretch = ptr->tr_vStretch;
		const FLOAT3D vInvStretch = FLOAT3D(1.0f/vStretch(1), 1.0f/vStretch(2), 1.0f/vStretch(3));
		MakeStretchMatrix(mInvStretch,vInvStretch);
		// find exact hit location on terrain
		fDistance = GetExactHitLocation(ptr,vHitBegin,vHitEnd,fOldDistance);
		fDistance += (vOrigin-vHitBegin).Length();

#if TR_DEBUG_RAYCASTING
		extern INDEX ter_iRecordRayCasting;
		if(ter_iRecordRayCasting==1) {
			if(fDistance!=UpperLimit(0.0f)) {
				_atrRays.Push() = TerrainRay(vHitBegin,vHitEnd,_vHitPoint);
			} else {
				_atrRays.Push() = TerrainRay(vHitBegin,vHitEnd);
			}
		}
#endif
	}
	return fDistance;
}

extern FLOAT TR_TestRayCastHit(CTerrain *ptrTerrain, const FLOATmatrix3D &mRotation, const FLOAT3D &vPosition, 
															 FLOAT3D vOrigin, FLOAT3D vTarget,FLOAT fOldDistance, 
															 BOOL bHitInvisibleTris)
{
	ASSERT(ptrTerrain!=NULL);                // must be valid
	ASSERT(ptrTerrain->tr_ptrTerrain!=NULL); // must be valid
	CTerrainImp *ptr = ptrTerrain->tr_ptrTerrain;
	_bHitInvisibleTriangles = bHitInvisibleTris;

	MatrixVectorToMatrix12(_mObjToAbs, mRotation, vPosition);
	MatrixTranspose(_mInvObjToAbs,_mObjToAbs);

	_bboxTerrain = TR_GetStretchedTerrainBBox(ptrTerrain);

	TransformVector(vOrigin.vector,_mInvObjToAbs);
	TransformVector(vTarget.vector,_mInvObjToAbs);
	FLOAT fDistance = TestRayCastHit(ptr, vOrigin, vTarget, fOldDistance);
	return fDistance;
}

extern FLOAT TR_TestRayCastHit(CTerrain *ptrTerrain, const FLOATmatrix3D &mRotation, const FLOAT3D &vPosition, 
															 FLOAT3D vOrigin, FLOAT3D vTarget,FLOAT fOldDistance, 
															 BOOL bHitInvisibleTris, FLOATplane3D &plHitPlane, FLOAT3D &vHitPoint)
{
	ASSERT(ptrTerrain!=NULL);                // must be valid
	ASSERT(ptrTerrain->tr_ptrTerrain!=NULL); // must be valid
	CTerrainImp *ptr = ptrTerrain->tr_ptrTerrain;
	_bHitInvisibleTriangles = bHitInvisibleTris;

	MatrixVectorToMatrix12(_mObjToAbs, mRotation, vPosition);
	MatrixTranspose(_mInvObjToAbs,_mObjToAbs);

	_bboxTerrain = TR_GetStretchedTerrainBBox(ptrTerrain);

	TransformVector(vOrigin.vector,_mInvObjToAbs);
	TransformVector(vTarget.vector,_mInvObjToAbs);
	FLOAT fDistance = TestRayCastHit(ptr, vOrigin, vTarget, fOldDistance);
	plHitPlane = _plHitPlane;
	vHitPoint  = _vHitPoint;
	TransformVector(vHitPoint.vector,_mObjToAbs);
	return fDistance;
}

