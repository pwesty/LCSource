#include "StdH.H"

#include <Engine/World/World.h>
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/World/WorldCollision.h>
#include <Engine/Entities/InternalClasses.h>
#include <Engine/Base/ListIterator.inl>
#include <Engine/Templates/DynamicContainer.cpp>
#include <Engine/Templates/DynamicArray.cpp>
#include <Engine/Brushes/Brush.h>
#include <Engine/Templates/StaticArray.cpp>
#include <Engine/Math/Clipping.inl>
#include <Engine/Math/Matrix12.h>
#include <Engine/Entities/EntityCollision.h>
#include <Engine/Base/ErrorReporting.h>
#include <Engine/Math/Geometry.inl>
#include <Engine/Templates/StaticStackArray.cpp>
#include <Engine/Terrain/Terrain.h>
#include <Engine/Network/CNetwork.h>
#include <Engine/Terrain/TerrainArchive.h>
#include <Engine/Terrain/Terrain.hpp>
#include <Engine/Terrain/TRMisc.hpp>
#include <Engine/LocalDefine.h>
#include <Engine/Interface/UIManager.h>
#include <Engine/Network/Server.h>
#include <Engine/Interface/UISiegeWarfareNew.h>
#include <Engine/Object/ActorMgr.h>

// these are used for making projections for converting from X space to Y space this way:
//  MatrixMulT(mY, mX, mXToY);
//  VectMulT(mY, vX-vY, vXToY);

// C=AtxB
__forceinline void MatrixMulT( const FLOATmatrix3D &mA, const FLOATmatrix3D &mB, FLOATmatrix3D &mC)
{
	mC(1,1) = mA(1,1)*mB(1,1)+mA(2,1)*mB(2,1)+mA(3,1)*mB(3,1);
	mC(1,2) = mA(1,1)*mB(1,2)+mA(2,1)*mB(2,2)+mA(3,1)*mB(3,2);
	mC(1,3) = mA(1,1)*mB(1,3)+mA(2,1)*mB(2,3)+mA(3,1)*mB(3,3);

	mC(2,1) = mA(1,2)*mB(1,1)+mA(2,2)*mB(2,1)+mA(3,2)*mB(3,1);
	mC(2,2) = mA(1,2)*mB(1,2)+mA(2,2)*mB(2,2)+mA(3,2)*mB(3,2);
	mC(2,3) = mA(1,2)*mB(1,3)+mA(2,2)*mB(2,3)+mA(3,2)*mB(3,3);

	mC(3,1) = mA(1,3)*mB(1,1)+mA(2,3)*mB(2,1)+mA(3,3)*mB(3,1);
	mC(3,2) = mA(1,3)*mB(1,2)+mA(2,3)*mB(2,2)+mA(3,3)*mB(3,2);
	mC(3,3) = mA(1,3)*mB(1,3)+mA(2,3)*mB(2,3)+mA(3,3)*mB(3,3);
}

// v2 = Mt*v1
__forceinline void VectMulT( const FLOATmatrix3D &mM, const FLOAT3D &vV1, FLOAT3D &vV2)
{
	vV2(1) = vV1(1)*mM(1,1)+vV1(2)*mM(2,1)+vV1(3)*mM(3,1);
	vV2(2) = vV1(1)*mM(1,2)+vV1(2)*mM(2,2)+vV1(3)*mM(3,2);
	vV2(3) = vV1(1)*mM(1,3)+vV1(2)*mM(2,3)+vV1(3)*mM(3,3);
}

/////////////////////////////////////////////////////////////////////
// CClipMove

// get start and end positions of an entity in this tick
inline void CClipMove::GetPositionsOfEntity(
	CEntity *pen, FLOAT3D &v0, FLOATmatrix3D &m0, FLOAT3D &v1, FLOATmatrix3D &m1)
{
	// start is where entity is now
	v0 = pen->en_plPlacement.pl_PositionVector;
	m0 = pen->en_mRotation;
	// if entity is movable
	if (pen->en_ulPhysicsFlags&EPF_MOVABLE) {
		// get end position from movable entity
		CMovableEntity *penMovable = (CMovableEntity*)pen;
		v1 = penMovable->en_vNextPosition;
		m1 = penMovable->en_mNextRotation;

		// NOTE: this prevents movable entities from hanging in the air when a brush moves
		// beneath their feet

		// if moving entity is reference of this entity
		if (penMovable->en_penReference == cm_penMoving)  {
			// add this entity to list of movers
			penMovable->AddToMoversDuringMoving();
		}

	// if entity is not movable
	} else {
		// end position is same as start
		v1 = v0;
		m1 = m0;
	}
}


/*
 * Constructor.
 */
CClipMove::CClipMove(CMovableEntity *penEntity)
{
	// clear last-hit statistics
	cm_penHit = NULL;
	cm_pbpoHit = NULL;
	cm_fMovementFraction = 2.0f;

	cm_penMoving = penEntity;
	// if the entity is deleted, or couldn't possible collide with anything
	if ((cm_penMoving->en_ulFlags&ENF_DELETED)
		||!(cm_penMoving->en_ulCollisionFlags&ECF_TESTMASK)
		||cm_penMoving->en_pciCollisionInfo==NULL) {
		// do nothing
		return;
	}

	// if entity is model
	if (penEntity->en_RenderType==CEntity::RT_MODEL ||
			penEntity->en_RenderType==CEntity::RT_EDITORMODEL || 
			penEntity->en_RenderType==CEntity::RT_SKAMODEL ||
			penEntity->en_RenderType==CEntity::RT_SKAEDITORMODEL ) {
		cm_bMovingBrush = FALSE;

		// remember entity and placements
		cm_penA = penEntity;
		GetPositionsOfEntity(cm_penA, cm_vA0, cm_mA0, cm_vA1, cm_mA1);

		// create spheres for the entity
		ASSERT(penEntity->en_pciCollisionInfo!=NULL);
		cm_pamsA = &penEntity->en_pciCollisionInfo->ci_absSpheres;

		// create aabbox for entire movement path
		FLOATaabbox3D box0, box1;
		penEntity->en_pciCollisionInfo->MakeBoxAtPlacement(cm_vA0, cm_mA0, box0);
		penEntity->en_pciCollisionInfo->MakeBoxAtPlacement(cm_vA1, cm_mA1, box1);
		cm_boxMovementPath  = box0;
		cm_boxMovementPath |= box1;

	// if entity is brush
	} else if (penEntity->en_RenderType==CEntity::RT_BRUSH) {
		cm_bMovingBrush = TRUE;

		// remember entity and placements
		cm_penB = penEntity;
		GetPositionsOfEntity(cm_penB, cm_vB0, cm_mB0, cm_vB1, cm_mB1);

		// create spheres for the entity
		ASSERT(penEntity->en_pciCollisionInfo!=NULL);
		// create aabbox for entire movement path
		FLOATaabbox3D box0, box1;
		penEntity->en_pciCollisionInfo->MakeBoxAtPlacement(cm_vB0, cm_mB0, box0);
		penEntity->en_pciCollisionInfo->MakeBoxAtPlacement(cm_vB1, cm_mB1, box1);
		cm_boxMovementPath  = box0;
		cm_boxMovementPath |= box1;

	} else {
		ASSERT(FALSE);
	}
}


// send pass if needed
__forceinline BOOL CClipMove::SendPassEvent(CEntity *penTested)
{
	BOOL bSent = FALSE;
	if (cm_ulPassMaskA & penTested->en_ulCollisionFlags) {

		EPass ePassA;
		ePassA.penOther = penTested;
		ePassA.bThisMoved = TRUE;
		cm_penMoving->SendEvent(ePassA);

		bSent = TRUE;
	}
	if (cm_ulPassMaskB & penTested->en_ulCollisionFlags) {

		EPass ePassB;
		ePassB.penOther = cm_penMoving;
		ePassB.bThisMoved = FALSE;
		penTested->SendEvent(ePassB);

		bSent = TRUE;
	}

	return bSent;
}

/*
 * Clip a moving point to a sphere, update collision data.
 */
__forceinline void CClipMove::ClipMovingPointToSphere(
	const FLOAT3D &vStart,
	const FLOAT3D &vEnd,
	const FLOAT3D &vSphereCenter,
	const FLOAT fSphereRadius)
{
	const FLOAT3D vSphereCenterToStart = vStart - vSphereCenter;
	const FLOAT3D vStartToEnd          = vEnd - vStart;

	// calculate discriminant for intersection parameters
	FLOAT fTemp = 100.0f;

	if ( vStartToEnd.Length() != 0.0f )
	{
		fTemp = 1.0f / (vStartToEnd%vStartToEnd);
	}

	const FLOAT f1oDotEnd = fTemp;
	const FLOAT fP = (vStartToEnd%vSphereCenterToStart) * f1oDotEnd;
	const FLOAT fQ = ((vSphereCenterToStart%vSphereCenterToStart) - (fSphereRadius*fSphereRadius)) * f1oDotEnd;

	const FLOAT fD = fP*fP-fQ;
	// if it is less than zero
	if (fD<0) {
		// no collision will occur
		return;
	}
	// calculate intersection parameters
	const FLOAT fSqrtD = sqrt(fD);
	const FLOAT fLambda1 = -fP+fSqrtD;
	const FLOAT fLambda2 = -fP-fSqrtD;
	// use lower one
	const FLOAT fMinLambda = Min(fLambda1, fLambda2);

	// if it is betwen zero and last collision found
	if (0.0f<=fMinLambda && fMinLambda<cm_fMovementFraction) {
		// if cannot pass
		if (!SendPassEvent(cm_penTested)) {
			// mark this as the new closest found collision point
			cm_fMovementFraction = fMinLambda;
			cm_vClippedLine = (vStartToEnd*(1.0f-fMinLambda))*cm_mBToAbsolute;
			ASSERT(cm_vClippedLine.Length()<100.0f);
			FLOAT3D vCollisionPoint = vStartToEnd*fMinLambda + vStart;
			FLOAT3D vCollisionNormal = vCollisionPoint - vSphereCenter;
			FLOATplane3D plClippedPlane(vCollisionNormal, vCollisionPoint);
			// project the collision plane from space B to absolute space
			cm_plClippedPlane = plClippedPlane*cm_mBToAbsolute+cm_vBToAbsolute;
			// remember hit entity
			cm_penHit = cm_penTested;
			cm_pbpoHit = cm_pbpoTested;
		}
	}
}

/*
 * Clip a moving point to a cylinder, update collision data.
 */
inline void CClipMove::ClipMovingPointToCylinder(
	const FLOAT3D &vStart,
	const FLOAT3D &vEnd,
	const FLOAT3D &vCylinderBottomCenter,
	const FLOAT3D &vCylinderTopCenter,
	const FLOAT fCylinderRadius)
{
	const FLOAT3D vStartToEnd            = vEnd - vStart;
	const FLOAT3D vCylinderBottomToStart = vStart - vCylinderBottomCenter;

	const FLOAT3D vCylinderBottomToTop       = vCylinderTopCenter - vCylinderBottomCenter;
	const FLOAT   fCylinderBottomToTopLength = vCylinderBottomToTop.Length();
	const FLOAT3D vCylinderDirection         = vCylinderBottomToTop/fCylinderBottomToTopLength;

	const FLOAT3D vB = vStartToEnd - vCylinderDirection*(vCylinderDirection%vStartToEnd);
	const FLOAT3D vC = vCylinderBottomToStart - vCylinderDirection*(vCylinderDirection%vCylinderBottomToStart);

	FLOAT fTemp = 100.0f;

	if ( vB.Length() != 0.0f ) 
	{
		fTemp = 1.0f / (vB%vB);
	}

	const FLOAT f1oDotBB = fTemp;
	const FLOAT fP = (vB%vC) * f1oDotBB;
	const FLOAT fQ = (vC%vC-fCylinderRadius*fCylinderRadius) * f1oDotBB;

	const FLOAT fD = fP*fP-fQ;
	// if it is less than zero
	if (fD<0) {
		// no collision will occur
		return;
	}
	// calculate intersection parameters
	const FLOAT fSqrtD = sqrt(fD);
	const FLOAT fLambda1 = -fP+fSqrtD;
	const FLOAT fLambda2 = -fP-fSqrtD;
	// use lower one
	const FLOAT fMinLambda = Min(fLambda1, fLambda2);

	// if it is betwen zero and last collision found
	if (0.0f<=fMinLambda && fMinLambda<cm_fMovementFraction) {
		// calculate the collision point
		FLOAT3D vCollisionPoint = vStartToEnd*fMinLambda + vStart;
		// create plane at cylinder bottom
		FLOATplane3D plCylinderBottom(vCylinderBottomToTop, vCylinderBottomCenter);
		// find distance of the collision point from the bottom plane
		FLOAT fCollisionToBottomPlaneDistance = plCylinderBottom.PointDistance(vCollisionPoint);
		// if the point is between bottom and top of cylinder
		if (0<=fCollisionToBottomPlaneDistance
			&&fCollisionToBottomPlaneDistance<fCylinderBottomToTopLength) {

			// if cannot pass
			if (!SendPassEvent(cm_penTested)) {
				// mark this as the new closest found collision point
				cm_fMovementFraction = fMinLambda;
				cm_vClippedLine = (vStartToEnd*(1.0f-fMinLambda))*cm_mBToAbsolute;
				ASSERT(cm_vClippedLine.Length()<100.0f);
				FLOAT3D vCollisionNormal = plCylinderBottom.ProjectPoint(vCollisionPoint)
					- vCylinderBottomCenter;
				FLOATplane3D plClippedPlane(vCollisionNormal, vCollisionPoint);
				// project the collision plane from space B to absolute space
				cm_plClippedPlane = plClippedPlane*cm_mBToAbsolute+cm_vBToAbsolute;
				// remember hit entity
				cm_penHit = cm_penTested;
				cm_pbpoHit = cm_pbpoTested;
			}
		}
	}
}

/*
 * Clip a moving sphere to a standing sphere, update collision data.
 */
void CClipMove::ClipMovingSphereToSphere(const CMovingSphere &msMoving,
	const CMovingSphere &msStanding)
{
	// use moving point to sphere collision with sum of sphere radii
	ClipMovingPointToSphere(
			msMoving.ms_vRelativeCenter0,       // start
			msMoving.ms_vRelativeCenter1,       // end
			msStanding.ms_vCenter,              // sphere center
			msMoving.ms_fR + msStanding.ms_fR   // sphere radius
		);
}
/*
 * Clip a moving sphere to a brush polygon, update collision data.
 */
void CClipMove::ClipMovingSphereToBrushPolygon(const CMovingSphere &msMoving,
																							 CBrushPolygon *pbpoPolygon)
{
	cm_pbpoTested = pbpoPolygon;
	const FLOATplane3D &plPolygon = pbpoPolygon->bpo_pbplPlane->bpl_plRelative;
	// calculate point distances from polygon plane
	FLOAT fDistance0 = plPolygon.PointDistance(msMoving.ms_vRelativeCenter0)-msMoving.ms_fR;
	FLOAT fDistance1 = plPolygon.PointDistance(msMoving.ms_vRelativeCenter1)-msMoving.ms_fR;

	// if first point is in front and second point is behind
	if (fDistance0>=0 && fDistance1<0) {
		// calculate fraction of line before intersection
		FLOAT fFraction = fDistance0/(fDistance0-fDistance1);
		ASSERT(fFraction>=0.0f && fFraction<=1.0f);

		// if fraction is less than minimum found fraction
		if (fFraction<cm_fMovementFraction) {
			// calculate intersection coordinate, projected to the polygon plane
			FLOAT3D vPosMid = msMoving.ms_vRelativeCenter0+(msMoving.ms_vRelativeCenter1-msMoving.ms_vRelativeCenter0)*fFraction;
			FLOAT3D vHitPoint = plPolygon.ProjectPoint(vPosMid);
			// find major axes of the polygon plane
			INDEX iMajorAxis1, iMajorAxis2;
			GetMajorAxesForPlane(plPolygon, iMajorAxis1, iMajorAxis2);

			// create an intersector
			CIntersector isIntersector(vHitPoint(iMajorAxis1), vHitPoint(iMajorAxis2));
			// for all edges in the polygon
			FOREACHINSTATICARRAY(pbpoPolygon->bpo_abpePolygonEdges, CBrushPolygonEdge,
				itbpePolygonEdge) {
				// get edge vertices (edge direction is irrelevant here!)
				const FLOAT3D &vVertex0 = itbpePolygonEdge->bpe_pbedEdge->bed_pbvxVertex0->bvx_vRelative;
				const FLOAT3D &vVertex1 = itbpePolygonEdge->bpe_pbedEdge->bed_pbvxVertex1->bvx_vRelative;
				// pass the edge to the intersector
				isIntersector.AddEdge(
					vVertex0(iMajorAxis1), vVertex0(iMajorAxis2),
					vVertex1(iMajorAxis1), vVertex1(iMajorAxis2));
			}
			// if the polygon is intersected by the ray
			if (isIntersector.IsIntersecting()) {
				// if cannot pass
				if (!SendPassEvent(cm_penTested)) {
					// mark this as the new closest found collision point
					cm_fMovementFraction = fFraction;
					cm_vClippedLine = msMoving.ms_vRelativeCenter1 - vPosMid;
					ASSERT(cm_vClippedLine.Length()<100.0f);
					// project the collision plane from space B to absolute space
					// only the normal of the plane is correct, not the distance!!!!
					cm_plClippedPlane = plPolygon*cm_mBToAbsolute+cm_vBToAbsolute;
					// remember hit entity
					cm_penHit = cm_penTested;
					cm_pbpoHit = cm_pbpoTested;
				}
			}
		}
	}

	// for each edge in polygon
	FOREACHINSTATICARRAY(pbpoPolygon->bpo_abpePolygonEdges, CBrushPolygonEdge, itbpe) {
		// get edge vertices (edge direction is important here!)
		FLOAT3D vVertex0, vVertex1;
		itbpe->GetVertexCoordinatesRelative(vVertex0, vVertex1);

		// clip moving sphere to the edge (moving point to the edge cylinder)
		ClipMovingPointToCylinder(
			msMoving.ms_vRelativeCenter0, // start,
			msMoving.ms_vRelativeCenter1, // end,
			vVertex0,                     // cylinder bottom center,
			vVertex1,                     // cylinder top center,
			msMoving.ms_fR                // cylinder radius
		);
		// clip moving sphere to the first vertex
		// NOTE: use moving point to sphere collision
		ClipMovingPointToSphere(
				msMoving.ms_vRelativeCenter0,  // start
				msMoving.ms_vRelativeCenter1,  // end
				vVertex0,                      // sphere center
				msMoving.ms_fR                 // sphere radius
			);
	}
}

/* Clip a moving sphere to a terrain polygon, update collision data. */
void CClipMove::ClipMovingSphereToTerrainPolygon(
	const CMovingSphere &msMoving, const FLOAT3D &v0, const FLOAT3D &v1, const FLOAT3D &v2)
{
	cm_pbpoTested = NULL;
	const FLOATplane3D plPolygon = FLOATplane3D(v0,v1,v2);

	// calculate point distances from polygon plane
	FLOAT fDistance0 = plPolygon.PointDistance(msMoving.ms_vRelativeCenter0)-msMoving.ms_fR;
	FLOAT fDistance1 = plPolygon.PointDistance(msMoving.ms_vRelativeCenter1)-msMoving.ms_fR;

	// if first point is in front and second point is behind
	if (fDistance0>=0 && fDistance1<0) {
		// calculate fraction of line before intersection
		FLOAT fFraction = fDistance0/(fDistance0-fDistance1);
		ASSERT(fFraction>=0.0f && fFraction<=1.0f);

		// if fraction is less than minimum found fraction
		if (fFraction<cm_fMovementFraction) {
			// calculate intersection coordinate, projected to the polygon plane
			FLOAT3D vPosMid = msMoving.ms_vRelativeCenter0+(msMoving.ms_vRelativeCenter1-msMoving.ms_vRelativeCenter0)*fFraction;
			FLOAT3D vHitPoint = plPolygon.ProjectPoint(vPosMid);
			// find major axes of the polygon plane
			INDEX iMajorAxis1, iMajorAxis2;
			GetMajorAxesForPlane(plPolygon, iMajorAxis1, iMajorAxis2);

			// create an intersector
			CIntersector isIntersector(vHitPoint(iMajorAxis1), vHitPoint(iMajorAxis2));

			// for all edges in the polygon, pass the edge to the intersector
			isIntersector.AddEdge(v0(iMajorAxis1), v0(iMajorAxis2), v1(iMajorAxis1), v1(iMajorAxis2));
			isIntersector.AddEdge(v1(iMajorAxis1), v1(iMajorAxis2), v2(iMajorAxis1), v2(iMajorAxis2));
			isIntersector.AddEdge(v2(iMajorAxis1), v2(iMajorAxis2), v0(iMajorAxis1), v0(iMajorAxis2));

			// if the polygon is intersected by the ray
			if (isIntersector.IsIntersecting()) {
				// if cannot pass
				if (!SendPassEvent(cm_penTested)) {
					// mark this as the new closest found collision point
					cm_fMovementFraction = fFraction;
					cm_vClippedLine = msMoving.ms_vRelativeCenter1 - vPosMid;
					ASSERT(cm_vClippedLine.Length()<100.0f);
					// project the collision plane from space B to absolute space
					// only the normal of the plane is correct, not the distance!!!!
					cm_plClippedPlane = plPolygon*cm_mBToAbsolute+cm_vBToAbsolute;
					// remember hit entity
					cm_penHit = cm_penTested;
					cm_pbpoHit = cm_pbpoTested;
				}
			}
		}
	}

	// for all edges in the polygon, clip moving sphere to the edge (moving point to the edge cylinder)
	ClipMovingPointToCylinder(
		msMoving.ms_vRelativeCenter0, // start,
		msMoving.ms_vRelativeCenter1, // end,
		v0,                     // cylinder bottom center,
		v1,                     // cylinder top center,
		msMoving.ms_fR                // cylinder radius
	);
	ClipMovingPointToCylinder(
		msMoving.ms_vRelativeCenter0, // start,
		msMoving.ms_vRelativeCenter1, // end,
		v1,                     // cylinder bottom center,
		v2,                     // cylinder top center,
		msMoving.ms_fR                // cylinder radius
	);
	ClipMovingPointToCylinder(
		msMoving.ms_vRelativeCenter0, // start,
		msMoving.ms_vRelativeCenter1, // end,
		v2,                     // cylinder bottom center,
		v0,                     // cylinder top center,
		msMoving.ms_fR                // cylinder radius
	);

	// for each edge in polygon, clip moving sphere to the first vertex
	// NOTE: use moving point to sphere collision
	ClipMovingPointToSphere(
			msMoving.ms_vRelativeCenter0,  // start
			msMoving.ms_vRelativeCenter1,  // end
			v0,                      // sphere center
			msMoving.ms_fR                 // sphere radius
		);
	ClipMovingPointToSphere(
			msMoving.ms_vRelativeCenter0,  // start
			msMoving.ms_vRelativeCenter1,  // end
			v1,                     // sphere center
			msMoving.ms_fR                 // sphere radius
		);
	ClipMovingPointToSphere(
			msMoving.ms_vRelativeCenter0,  // start
			msMoving.ms_vRelativeCenter1,  // end
			v2,                      // sphere center
			msMoving.ms_fR                 // sphere radius
		);
}

/* Clip movement to a terrain polygon. */
void CClipMove::ClipMoveToTerrainPolygon(const FLOAT3D &v0, const FLOAT3D &v1, const FLOAT3D &v2)
{
	// for each sphere of entity A
	FOREACHINSTATICARRAY(*cm_pamsA, CMovingSphere, itmsMoving) {
		// clip moving sphere to the polygon
		ClipMovingSphereToTerrainPolygon(*itmsMoving, v0, v1, v2);
	}
}

/*
 * Clip movement to a brush polygon.
 */
void CClipMove::ClipMoveToBrushPolygon(CBrushPolygon *pbpoPolygon)
{
	// for each sphere of entity A
	FOREACHINSTATICARRAY(*cm_pamsA, CMovingSphere, itmsMoving) {
		// clip moving sphere to the polygon
		ClipMovingSphereToBrushPolygon(*itmsMoving, pbpoPolygon);
	}
}

/*
 * Project spheres of moving entity to standing entity space.
 */
void CClipMove::ProjectASpheresToB(void)
{
	// for each sphere
	FOREACHINSTATICARRAY(*cm_pamsA, CMovingSphere, itmsA) {
		// project it in start point
		itmsA->ms_vRelativeCenter0 = itmsA->ms_vCenter*cm_mAToB0+cm_vAToB0;
		// project it in end point
		itmsA->ms_vRelativeCenter1 = itmsA->ms_vCenter*cm_mAToB1+cm_vAToB1;
		// make bounding box
		itmsA->ms_boxMovement = FLOATaabbox3D(itmsA->ms_vRelativeCenter0, itmsA->ms_vRelativeCenter1);
		itmsA->ms_boxMovement.Expand(itmsA->ms_fR);
	}
}

/* Find movement box in absolute space for A entity. */
void CClipMove::FindAbsoluteMovementBoxForA(void)
{
	cm_boxMovementPathAbsoluteA = FLOATaabbox3D();
	// for each sphere
	FOREACHINSTATICARRAY(*cm_pamsA, CMovingSphere, itmsA) {
		// project it in start point
		FLOAT3D v0 = (itmsA->ms_vCenter*cm_mAToB0+cm_vAToB0)*cm_mB0+cm_vB0;
		// project it in end point
		FLOAT3D v1 = (itmsA->ms_vCenter*cm_mAToB1+cm_vAToB1)*cm_mB0+cm_vB0;
		// make bounding box
		FLOATaabbox3D box = FLOATaabbox3D(v0, v1);
		box.Expand(itmsA->ms_fR);
		cm_boxMovementPathAbsoluteA|=box;
	}
}

//Temp 시작
/*
 * Clip movement if B is a model.
 */
void CClipMove::ClipModelMoveToModel(void)
{
	// assumes that all spheres in one entity have same radius
	FLOAT fRB = (*cm_pamsB)[0].ms_fR;
	FLOAT fDiff;
	ULONG ulRes;

	// for each sphere in entity A
	FOREACHINSTATICARRAY(*cm_pamsA, CMovingSphere, itmsA) {
		CMovingSphere &msA = *itmsA;
		FLOATaabbox3D &boxMovingSphere = msA.ms_boxMovement;

		// for each sphere in entity B
		FOREACHINSTATICARRAY(*cm_pamsB, CMovingSphere, itmsB) {
			CMovingSphere &msB = *itmsB;
			// if the sphere is too far
			fDiff = (msB.ms_vCenter(1)+fRB) - boxMovingSphere.Min()(1);  ulRes  = (ULONG&)fDiff;
			fDiff = boxMovingSphere.Max()(1) - (msB.ms_vCenter(1)-fRB);  ulRes |= (ULONG&)fDiff;
			fDiff = (msB.ms_vCenter(2)+fRB) - boxMovingSphere.Min()(2);  ulRes |= (ULONG&)fDiff;
			fDiff = boxMovingSphere.Max()(2) - (msB.ms_vCenter(2)-fRB);  ulRes |= (ULONG&)fDiff;
			fDiff = (msB.ms_vCenter(3)+fRB) - boxMovingSphere.Min()(3);  ulRes |= (ULONG&)fDiff;
			fDiff = boxMovingSphere.Max()(3) - (msB.ms_vCenter(3)-fRB);  ulRes |= (ULONG&)fDiff;
			//(boxMovingSphere.Min()(1)>msB.ms_vCenter(1)+fRB) ||
				//(boxMovingSphere.Max()(1)<msB.ms_vCenter(1)-fRB) ||
				//(boxMovingSphere.Min()(2)>msB.ms_vCenter(2)+fRB) ||
				//(boxMovingSphere.Max()(2)<msB.ms_vCenter(2)-fRB) ||
				//(boxMovingSphere.Min()(3)>msB.ms_vCenter(3)+fRB) ||
				//(boxMovingSphere.Max()(3)<msB.ms_vCenter(3)-fRB)) { 
			if( ulRes&0x80000000) {
				// skip it
				continue;
			}
			// clip sphere A to sphere B
			ClipMovingSphereToSphere(msA, msB);
		}
	}
}

/*
#include <Engine/Math/OBBox.h>
void CClipMove::ClipModelMoveToModel(void)
{
	//Sphere간 충돌에 필요한 데이터 준비. AABB전체를 포함하는 Sphere임.
	INDEX temp;
	FLOATaabbox3D abA, abB;
	cm_penMoving->GetCollisionBoxParameters(cm_penMoving->GetCollisionBoxIndex(), abA, temp);
	cm_penTested->GetCollisionBoxParameters(cm_penTested->GetCollisionBoxIndex(), abB, temp);
	FLOAT3D vA[2], vB[2];
	FLOATmatrix3D mA[2], mB[2];
	GetPositionsOfEntity(cm_penMoving, vA[0], mA[0], vA[1], mA[1]);
	GetPositionsOfEntity(cm_penTested, vB[0], mB[0], vB[1], mB[1]);
	vA[0] += abA.Size();
	vA[1] += abA.Size();
	vB[0] += abA.Size();
	vB[1] += abA.Size();

	FLOAT fRadiusA = abA.Size().Length() * 0.49f;//반지름 계산을 Entity쪽에 미루면 메모리는 더쓰지만 연산량은 적어짐.
	FLOAT fRadiusB = abB.Size().Length() * 0.49f;

	//sphere test
	static const INDEX iUseIndex = 1;
	FLOAT3D vDist = vA[iUseIndex] - vB[iUseIndex];
	FLOAT fDistance = vDist.Length();
	if(fDistance > fRadiusA + fRadiusB) return;

	//obb data prepare
	static const FLOAT fFractionValue = 0.00f;
	FLOATobbox3D obA(abA, vA[iUseIndex], mA[iUseIndex]);
	FLOATobbox3D obB(abB, vB[iUseIndex], mB[iUseIndex]);

	//obb test
 	if( !obA.HasContactWith(obB) ) return;

	//sliding을 위한 접촉면 추정, A, B사이 선분이 B의 obb의 어느면을 통과했나로 추정.
	//B(tested)의 obb의 세 축에 대한 dot값 계산.
	const FLOAT3D vStartToEnd = vA[1] - vA[0];
	FLOAT3D vAxisXB = mB[iUseIndex].GetColumn(1);
	FLOAT3D vAxisYB = mB[iUseIndex].GetColumn(2);
	FLOAT3D vAxisZB = mB[iUseIndex].GetColumn(3);
	FLOAT fAxisValB1 = ((vA[iUseIndex] - vB[iUseIndex]) % vAxisXB) / abB.Size()(1);
	FLOAT fAxisValB2 = ((vA[iUseIndex] - vB[iUseIndex]) % vAxisYB) / abB.Size()(2);
	FLOAT fAxisValB3 = ((vA[iUseIndex] - vB[iUseIndex]) % vAxisZB) / abB.Size()(3);
	FLOAT3D vAxisValB(fAxisValB1, fAxisValB2, fAxisValB3);
	fAxisValB1 = fabs(fAxisValB1);
	fAxisValB2 = fabs(fAxisValB2);
	fAxisValB3 = fabs(fAxisValB3);

	//B(tested)의 obb의 한 면의 normal을 Collision normal로 삼는다.
	//축의 값이 제일 크다는 것은 그 축이 방향의 obb면에서 충돌이 있었을 가능성이 크다는 뜻임.
	FLOAT3D vCollisionNormalTemp(0,0,-1);
	if( fAxisValB1 > fAxisValB2 && fAxisValB1 > fAxisValB3 )
	{
		if(fAxisValB1 > 0) vCollisionNormalTemp = -vAxisXB;
		else vCollisionNormalTemp = vAxisXB;
	}
	else if( fAxisValB2 > fAxisValB1 && fAxisValB2 > fAxisValB3 )
	{
		if(fAxisValB2 > 0) vCollisionNormalTemp = -vAxisYB;
		else vCollisionNormalTemp = vAxisYB;
	}
	else //if(fAxisValB3 > fAxisValB1 && fAxisValB3 > fAxisValB2)
	{
		if(fAxisValB3 > 0) vCollisionNormalTemp = -vAxisZB;
		else vCollisionNormalTemp = vAxisZB;
	}

	//CClipMove의 값을 세팅.
	cm_fMovementFraction = fFractionValue;
	cm_vClippedLine = (vStartToEnd * (1.0f - fFractionValue));
	ASSERT(cm_vClippedLine.Length() < 100.0f);	//sam에 있던거
	//FLOAT3D vCollisionPoint = vStartToEnd * fFractionValue + vA[0];
	FLOAT3D vCollisionPoint = vB[iUseIndex] + vCollisionNormalTemp;
	FLOAT3D vCollisionNormal = vCollisionNormalTemp;
	FLOATplane3D plClippedPlane(vCollisionNormal, vCollisionPoint);
	cm_plClippedPlane = plClippedPlane;
	cm_penHit = cm_penTested;
	cm_pbpoHit = cm_pbpoTested;
}
*/	//OBB 방식 충돌
//Temp 끝



/*
 * Clip movement if B is a brush.
 */
void CClipMove::ClipBrushMoveToModel(void)
{
	// get first mip of the brush
	CBrushMip *pbmMip = cm_penB->en_pbrBrush->GetFirstMip();
	// for each sector in the brush mip
	FOREACHINDYNAMICARRAY(pbmMip->bm_abscSectors, CBrushSector, itbsc) {
		// if the sector's bbox has no contact with bbox of movement path
		if ( !itbsc->bsc_boxBoundingBox.HasContactWith(cm_boxMovementPathAbsoluteA, 0.01f) ) {
			// skip it
			continue;
		}
		// for each polygon in the sector
		FOREACHINSTATICARRAY(itbsc->bsc_abpoPolygons, CBrushPolygon, itbpo) {
			// if it is passable or its bbox has no contact with bbox of movement path
			if ((itbpo->bpo_ulFlags&BPOF_PASSABLE)
					||!itbpo->bpo_boxBoundingBox.HasContactWith(cm_boxMovementPathAbsoluteA, 0.01f) ) {
				// skip it
				continue;
			}
			// clip movement to the polygon
			ClipMoveToBrushPolygon(itbpo);
		}
	}
}


/*
 * Prepare projections and spheres for movement clipping.
 */
void CClipMove::PrepareProjectionsAndSpheres(void)
{
 // Formula: C=AxB --> Cij=Sum(s=1..k)(Ais*Bsj)

	// make projections for converting from A space to B space
	MatrixMulT(cm_mB0, cm_mA0, cm_mAToB0);
	VectMulT(cm_mB0, cm_vA0-cm_vB0, cm_vAToB0);
	MatrixMulT(cm_mB1, cm_mA1, cm_mAToB1);
	VectMulT(cm_mB1, cm_vA1-cm_vB1, cm_vAToB1);

	// projection for converting from B space to absolute space
	cm_mBToAbsolute = cm_mB0;
	cm_vBToAbsolute = cm_vB0;

	// project spheres of entity A to space B
	ProjectASpheresToB();
}


/*
 * Clip movement to a model entity.
 */
void CClipMove::ClipMoveToModel(CEntity *penModel)
{
	FLOAT fDiff;
	ULONG ulRes;
	ASSERT(penModel->en_pciCollisionInfo!=NULL);

	// if not possibly colliding
	const FLOATaabbox3D &boxModel = penModel->en_pciCollisionInfo->ci_boxCurrent;
	fDiff = boxModel.Max()(1) - cm_boxMovementPath.Min()(1);  ulRes  = (ULONG&)fDiff;
	fDiff = cm_boxMovementPath.Max()(1) - boxModel.Min()(1);  ulRes |= (ULONG&)fDiff;
	fDiff = boxModel.Max()(2) - cm_boxMovementPath.Min()(2);  ulRes |= (ULONG&)fDiff;
	fDiff = cm_boxMovementPath.Max()(2) - boxModel.Min()(2);  ulRes |= (ULONG&)fDiff;
	fDiff = boxModel.Max()(3) - cm_boxMovementPath.Min()(3);  ulRes |= (ULONG&)fDiff;
	fDiff = cm_boxMovementPath.Max()(3) - boxModel.Min()(3);  ulRes |= (ULONG&)fDiff;
	/*(cm_boxMovementPath.Min()(1)>boxModel.Max()(1)) ||
		(cm_boxMovementPath.Max()(1)<boxModel.Min()(1)) ||
		(cm_boxMovementPath.Min()(2)>boxModel.Max()(2)) ||
		(cm_boxMovementPath.Max()(2)<boxModel.Min()(2)) ||
		(cm_boxMovementPath.Min()(3)>boxModel.Max()(3)) ||
		(cm_boxMovementPath.Max()(3)<boxModel.Min()(3))) { */
	if( ulRes&0x80000000) {
		// do nothing
		return;
	}

	// remember tested entity
	cm_penTested = penModel;
	cm_pbpoTested = NULL;

	// if clipping a moving model
	if (!cm_bMovingBrush) {
		// moving model is A and other model is B
		cm_penB = penModel;
		GetPositionsOfEntity(cm_penB, cm_vB0, cm_mB0, cm_vB1, cm_mB1);
		// create bounding spheres for the model
		ASSERT(penModel->en_pciCollisionInfo!=NULL);
		cm_pamsB = &penModel->en_pciCollisionInfo->ci_absSpheres;

		// prepare new projections and spheres
		PrepareProjectionsAndSpheres();
		// clip model to model
		ClipModelMoveToModel();

	// if clipping a moving brush
	} else {
		// moving brush is B and still model is A
		cm_penA = penModel;
		GetPositionsOfEntity(cm_penA, cm_vA0, cm_mA0, cm_vA1, cm_mA1);
		// create bounding spheres for the model
		ASSERT(penModel->en_pciCollisionInfo!=NULL);
		cm_pamsA = &penModel->en_pciCollisionInfo->ci_absSpheres;

		// prepare new projections and spheres
		PrepareProjectionsAndSpheres();
		FindAbsoluteMovementBoxForA();
		// clip brush to model
		ClipBrushMoveToModel();
	}
}

/* Cache near polygons of movable entity. */
void CClipMove::CacheNearPolygons(void)
{
	// if movement box is still inside cached box
	if (cm_boxMovementPath<=cm_penMoving->en_boxNearCached) {
		// do nothing
		return;
	}

	FLOATaabbox3D &box = cm_penMoving->en_boxNearCached;
	CStaticStackArray<CBrushPolygon *> &apbpo = cm_penMoving->en_apbpoNearPolygons;

	// flush old cached polygons
	apbpo.PopAll();
	// set new box to union of movement box and future estimate
	box  = cm_boxMovementPath;
	box |= cm_penMoving->en_boxMovingEstimate;

	// for each zoning sector that this entity is in
	{FOREACHSRCOFDST(cm_penMoving->en_rdSectors, CBrushSector, bsc_rsEntities, pbsc)
		// add it to list of active sectors
		cm_lhActiveSectors.AddTail(pbsc->bsc_lnInActiveSectors);
	ENDFOR}

	// for each active sector
	FOREACHINLIST(CBrushSector, bsc_lnInActiveSectors, cm_lhActiveSectors, itbsc) {
		// for each polygon in the sector
		FOREACHINSTATICARRAY(itbsc->bsc_abpoPolygons, CBrushPolygon, itbpo) {
			CBrushPolygon *pbpo = itbpo;
			// if its bbox has no contact with bbox to cache
			if (!pbpo->bpo_boxBoundingBox.HasContactWith(box) ) {
				// skip it
				continue;
			}
			// add it to cache
			apbpo.Push() = pbpo;
			// if it is passable
			if (pbpo->bpo_ulFlags&BPOF_PASSABLE) {
				// for each sector related to the portal
				{FOREACHDSTOFSRC(pbpo->bpo_rsOtherSideSectors, CBrushSector, bsc_rdOtherSidePortals, pbscRelated)
					// if the sector is not active
					if (!pbscRelated->bsc_lnInActiveSectors.IsLinked()) {
						// add it to active list
						cm_lhActiveSectors.AddTail(pbscRelated->bsc_lnInActiveSectors);
					}
				ENDFOR}
			}
		}

		// for non-zoning non-movable brush entities in the sector
		{FOREACHDSTOFSRC(itbsc->bsc_rsEntities, CEntity, en_rdSectors, pen)
			if (pen->en_RenderType==CEntity::RT_TERRAIN) {
				continue;
			}
			if (pen->en_RenderType!=CEntity::RT_BRUSH&&
					pen->en_RenderType!=CEntity::RT_FIELDBRUSH) {
				break;  // brushes are sorted first in list
			}
			if(pen->en_ulPhysicsFlags&EPF_MOVABLE) {
				continue;
			}
			if(!MustTest(pen)) {
				continue;
			}

			// get first mip
			CBrushMip *pbm = pen->en_pbrBrush->GetFirstMip();
			// if brush mip exists for that mip factor
			if (pbm!=NULL) {
				// for each sector in the mip
				{FOREACHINDYNAMICARRAY(pbm->bm_abscSectors, CBrushSector, itbscNonZoning) {
					CBrushSector &bscNonZoning = *itbscNonZoning;
					// add it to list of active sectors
					if(!bscNonZoning.bsc_lnInActiveSectors.IsLinked()) {
						cm_lhActiveSectors.AddTail(bscNonZoning.bsc_lnInActiveSectors);
					}
				}}
			}
		ENDFOR}
	}

	// clear list of active sectors
	{FORDELETELIST(CBrushSector, bsc_lnInActiveSectors, cm_lhActiveSectors, itbsc) {
		itbsc->bsc_lnInActiveSectors.Remove();
	}}
}


void CClipMove::ClipToNonZoningSector(CBrushSector *pbsc)
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	// for each polygon in the sector
	FOREACHINSTATICARRAY(pbsc->bsc_abpoPolygons, CBrushPolygon, itbpo) {
		// if its bbox has no contact with bbox of movement path, or it is passable
		if (!itbpo->bpo_boxBoundingBox.HasContactWith(cm_boxMovementPath)
			||(itbpo->bpo_ulFlags&BPOF_PASSABLE)) {
			// skip it
			continue;
		}
		//WSS_DRATAN_SIEGEWARFARE 071009 -------------------------------------->>
		//드라탄 공성시 수성측의 경우 결계 충돌을 하지 않게 한다.
		BOOL tJoin = FALSE;
		if (pUIManager->GetSiegeWarfareNew()->GetWarState())
		{			
			if( cm_penB->en_ulID == 14230 ||
				cm_penB->en_ulID == 14229 ||
				cm_penB->en_ulID == 14228 )
			{	
				// 플레이어 캐릭터
				if( CEntity::GetPlayerEntity(0) == cm_penA )
				{
					if( _pNetwork->MyCharacterInfo.sbJoinFlagDratan == WCJF_OWNER || 
						_pNetwork->MyCharacterInfo.sbJoinFlagDratan == WCJF_DEFENSE_GUILD )
					{
						tJoin = TRUE;							
					}							
				}
				// 다른 캐릭터
				else 
				{
					ObjectBase* pObject = ACTORMGR()->GetObjectByEntity(cm_penA);

					if (pObject != NULL)
					{
						if (pObject->m_eType == eOBJ_CHARACTER)
						{
							CCharacterTarget* pTarget = static_cast< CCharacterTarget* >(pObject);

							if( pTarget->cha_sbJoinFlagDratan == WCJF_OWNER || 
								pTarget->cha_sbJoinFlagDratan == WCJF_DEFENSE_GUILD )
							{
								tJoin = TRUE;
							}
						}
					}
				}
			}
		}
		// --------------------------------------------------------------------<<
		if( !tJoin )
		{
			// clip movement to the polygon
			ClipMoveToBrushPolygon(itbpo);
		}
	}
}


void CClipMove::ClipToTerrain(CEntity *pen)
{
	CTerrain &tr = *pen->en_ptrTerrain;
	FLOAT3D *pavVertices;
	UWORD *puwIndices;
	INDEX ctVertices, ctIndices;
	FLOAT3D vMin = cm_boxMovementPath.Min();
	FLOAT3D vMax = cm_boxMovementPath.Max();

	/*
	FLOATaabbox3D boxMovementPath;
	#define TRANSPT(x) (x-pen->en_plPlacement.pl_PositionVector) * !pen->en_mRotation
	boxMovementPath  = TRANSPT(FLOAT3D(vMin(1),vMin(2),vMin(3)));
	boxMovementPath |= TRANSPT(FLOAT3D(vMin(1),vMin(2),vMax(3)));
	boxMovementPath |= TRANSPT(FLOAT3D(vMax(1),vMin(2),vMin(3)));
	boxMovementPath |= TRANSPT(FLOAT3D(vMax(1),vMin(2),vMax(3)));
	boxMovementPath |= TRANSPT(FLOAT3D(vMin(1),vMax(2),vMin(3)));
	boxMovementPath |= TRANSPT(FLOAT3D(vMin(1),vMax(2),vMax(3)));
	boxMovementPath |= TRANSPT(FLOAT3D(vMax(1),vMax(2),vMin(3)));
	boxMovementPath |= TRANSPT(FLOAT3D(vMax(1),vMax(2),vMax(3)));
	*/

	Matrix12 mObjToAbs;
	MatrixVectorToMatrix12(mObjToAbs, pen->en_mRotation, pen->en_plPlacement.pl_PositionVector);
	TR_ExtractPolygonsInBox( &tr, cm_boxMovementPath, mObjToAbs, &pavVertices, ctVertices, &puwIndices, ctIndices);
	
	// for each triangle
	for( INDEX iTri=0;iTri<ctIndices;iTri+=3) {
		const INDEX iind1 = puwIndices[iTri+0];
		const INDEX iind2 = puwIndices[iTri+1];
		const INDEX iind3 = puwIndices[iTri+2];
		const FLOAT3D &v0 = pavVertices[iind1];
		const FLOAT3D &v1 = pavVertices[iind2];
		const FLOAT3D &v2 = pavVertices[iind3];
		ClipMoveToTerrainPolygon(v0,v1,v2);
	}

	// [070705: Su-won] ATTRIBUTEMAP_BLOCK
	CTRect rcExtract;
	rcExtract.rc_slLeft = cm_boxMovementPath.minvect(1) -1.0f;
	rcExtract.rc_slRight = cm_boxMovementPath.maxvect(1) +1.0f;
	rcExtract.rc_slTop = cm_boxMovementPath.minvect(3) -1.0f;
	rcExtract.rc_slBottom = cm_boxMovementPath.maxvect(3) +1.0f;

	if(g_slZone != 21)
	{
		TR_ExtractAttrBlockPolygonsInRect( tr.tr_ptrTerrain, rcExtract, &pavVertices, ctVertices, &puwIndices, ctIndices, 10.0f);
		for( INDEX iTri = 0; iTri < ctIndices; iTri += 3)
		{
			const INDEX iind1 = puwIndices[iTri+0];
			const INDEX iind2 = puwIndices[iTri+1];
			const INDEX iind3 = puwIndices[iTri+2];
			const FLOAT3D &v0 = pavVertices[iind1];
			const FLOAT3D &v1 = pavVertices[iind2];
			const FLOAT3D &v2 = pavVertices[iind3];
			ClipMoveToTerrainPolygon(v0,v1,v2);
		}
	}
	
}


void CClipMove::ClipToZoningSector(CBrushSector *pbsc)
{
	CStaticStackArray<CBrushPolygon *> &apbpo = cm_penMoving->en_apbpoNearPolygons;

	// for each cached polygon
	for(INDEX iPolygon=0; iPolygon<apbpo.Count(); iPolygon++) {
		CBrushPolygon *pbpo = apbpo[iPolygon];
		// if it doesn't belong to the sector or its bbox has no contact with bbox of movement path
		if (pbpo->bpo_pbscSector != pbsc ||
			!pbpo->bpo_boxBoundingBox.HasContactWith(cm_boxMovementPath)) {
			// skip it
			continue;
		}
		// if it is not passable
		if (!(pbpo->bpo_ulFlags&BPOF_PASSABLE)) {
			// clip movement to the polygon
			ClipMoveToBrushPolygon(pbpo);
		// if it is passable
		} else {
			// for each sector related to the portal
			{FOREACHDSTOFSRC(pbpo->bpo_rsOtherSideSectors, CBrushSector, bsc_rdOtherSidePortals, pbscRelated)
				// if the sector is not active
				if (pbscRelated->bsc_pbmBrushMip->IsFirstMip() &&
					 !pbscRelated->bsc_lnInActiveSectors.IsLinked()) {
					// add it to active list
					cm_lhActiveSectors.AddTail(pbscRelated->bsc_lnInActiveSectors);
				}
			ENDFOR}
		}
	}
}


/* Clip movement to brush sectors near the entity. */
void CClipMove::ClipMoveToBrushes(void)
{
	// we never clip moving brush to a brush
	if (cm_bMovingBrush) {
		return;
	}
	if (cm_penMoving->en_ulCollisionFlags&ECF_IGNOREBRUSHES) {
		return;
	}

	// for each zoning sector that this entity is in
	{FOREACHSRCOFDST(cm_penMoving->en_rdSectors, CBrushSector, bsc_rsEntities, pbsc)
		// if it collides with this one
		if (pbsc->bsc_pbmBrushMip->IsFirstMip() &&
			pbsc->bsc_pbmBrushMip->bm_pbrBrush->br_pfsFieldSettings==NULL &&
			MustTest(pbsc->bsc_pbmBrushMip->bm_pbrBrush->br_penEntity)) {
			// add it to list of active sectors
			cm_lhActiveSectors.AddTail(pbsc->bsc_lnInActiveSectors);
		}
	ENDFOR}

	// for each active sector
	FOREACHINLIST(CBrushSector, bsc_lnInActiveSectors, cm_lhActiveSectors, itbsc) {
		// for non-zoning brush entities in the sector
		extern INDEX phy_bNonZoningClipMove;
		if (phy_bNonZoningClipMove) 
		{FOREACHDSTOFSRC(itbsc->bsc_rsEntities, CEntity, en_rdSectors, pen)
			if (pen->en_RenderType!=CEntity::RT_BRUSH&&
					pen->en_RenderType!=CEntity::RT_FIELDBRUSH&&
					pen->en_RenderType!=CEntity::RT_TERRAIN) {
				break;  // brushes are sorted first in list
			}
			if(!MustTest(pen)) {
				continue;
			}

			if (pen->en_RenderType==CEntity::RT_TERRAIN) {
				// remember currently tested entity
				cm_penTested = pen;
				// moving model is A and still terrain is B
				cm_penB = pen;
				GetPositionsOfEntity(cm_penB, cm_vB0, cm_mB0, cm_vB1, cm_mB1);

				// prepare new projections and spheres
				PrepareProjectionsAndSpheres();

				// clip movement to the terrain
				ClipToTerrain(pen);

				// don't process as brush
				continue;
			}

			// get first mip
			CBrushMip *pbm = pen->en_pbrBrush->GetFirstMip();
			// if brush mip exists for that mip factor
			if (pbm!=NULL) {
				// if the brush is either touching the movement path or it is moving
				if (pbm->bm_boxBoundingBox.HasContactWith(cm_boxMovementPath)
					|| (pen->en_ulPhysicsFlags&EPF_MOVABLE)) {
					// for each sector in the mip
					{FOREACHINDYNAMICARRAY(pbm->bm_abscSectors, CBrushSector, itbscNonZoning) {
						CBrushSector &bscNonZoning = *itbscNonZoning;
						// add it to list of active sectors
						if(!bscNonZoning.bsc_lnInActiveSectors.IsLinked()) {
							cm_lhActiveSectors.AddTail(bscNonZoning.bsc_lnInActiveSectors);
						}
					}}
				}
			}
		ENDFOR}

		// get the sector's brush mip, brush and entity
		CBrushMip *pbmBrushMip = itbsc->bsc_pbmBrushMip;
		CBrush3D *pbrBrush = pbmBrushMip->bm_pbrBrush;
		ASSERT(pbrBrush!=NULL);
		CEntity *penBrush = pbrBrush->br_penEntity;
		ASSERT(penBrush!=NULL);

		// remember currently tested entity
		cm_penTested = penBrush;
		// moving model is A and still brush is B
		cm_penB = penBrush;
		GetPositionsOfEntity(cm_penB, cm_vB0, cm_mB0, cm_vB1, cm_mB1);

		// prepare new projections and spheres
		PrepareProjectionsAndSpheres();

		// clip movement to the sector
		if (penBrush->en_ulFlags&ENF_ZONING) {
			ClipToZoningSector(itbsc);
		} else {
			ClipToNonZoningSector(itbsc);
		}
	}

	// clear list of active sectors
	{FORDELETELIST(CBrushSector, bsc_lnInActiveSectors, cm_lhActiveSectors, itbsc) {
		itbsc->bsc_lnInActiveSectors.Remove();
	}}
}


/* Clip movement to models near the entity. */
void CClipMove::ClipMoveToModels(void)
{
	if (cm_penMoving->en_ulCollisionFlags&ECF_IGNOREMODELS) {
		return;
	}

	// WSS_CHARACTER_COLLISION 070628 ------------->>
	// 자신의 캐릭터만 충돌 체크
	// $ : 드라탄 공성 처리 당시 수정된 사항으로 보임. 싱글 던전에서는 플레이어 외의 모델이 충돌처리를 해야 되서. 싱글던전은 예외로 한다.
	if(!cm_penMoving->IsPlayer() && !_pNetwork->m_bSingleMode)
	{
		return;	
	}
	// --------------------------------------------<<

	// create mask for skipping deleted entities
	ULONG ulSkipMask = ENF_DELETED;

	// find colliding entities near the box of movement path
	static CStaticStackArray<CEntity*> apenNearEntities;
	cm_pwoWorld->FindEntitiesNearBox(cm_boxMovementPath, apenNearEntities);

	CUIManager* pUIManager = CUIManager::getSingleton();

	// for each of the found entities
	{for(INDEX ienFound=0; ienFound<apenNearEntities.Count(); ienFound++) {
		CEntity &enToCollide = *apenNearEntities[ienFound];
		// if it is the one that is moving, or if it is skiped by the mask
		if (&enToCollide == cm_penMoving || (enToCollide.en_ulFlags&ulSkipMask)) {
			// skip it
			continue;
		}

		// 공성전에 관계 없이 모든 캐릭터, Player 는 충돌 검사 제외
		if( enToCollide.IsCharacter() || enToCollide.IsPlayer() )
		{
			continue; 
		}

		if( enToCollide.GetFirstExFlags() & ( ENF_EX1_CURRENT_SLAVE | ENF_EX1_CURRENT_PET ) )
		{
			continue;
		}

		if( cm_penMoving->GetFirstExFlags() & ( ENF_EX1_CURRENT_SLAVE | ENF_EX1_CURRENT_PET ) )
		{
			continue;
		}

		// if it can collide with this entity
		if (MustTest(&enToCollide)) {
			// if it is model entity
			if (enToCollide.en_RenderType == CEntity::RT_MODEL ||
					enToCollide.en_RenderType == CEntity::RT_EDITORMODEL ||
					enToCollide.en_RenderType == CEntity::RT_SKAMODEL ||
					enToCollide.en_RenderType == CEntity::RT_SKAEDITORMODEL) {
				// clip movement to the model
				ClipMoveToModel(&enToCollide);
			}
		}
	}}
	apenNearEntities.PopAll();
}


/*
 * Clip movement to the world.
 */
void CClipMove::ClipMoveToWorld(class CWorld *pwoWorld)
{
	// if there is no move or if the entity is deleted, or doesn't collide with anything
	// test if there is no movement !!!!
	if (/*!cm_bMovingBrush&&(cm_vA0 == cm_vA1 && cm_mA0 == cm_mA1)
		|| cm_bMovingBrush&&(cm_vB0 == cm_vB1 && cm_mB0 == cm_mB1)
		||*/(cm_penMoving->en_ulFlags&ENF_DELETED)
		||!(cm_penMoving->en_ulCollisionFlags&ECF_TESTMASK)) {
		// skip clipping
		return;
	}

	cm_pwoWorld = pwoWorld;

	// prepare flags masks for testing which entities collide with this
	cm_ulTestMask1 = ((cm_penMoving->en_ulCollisionFlags&ECF_TESTMASK)>>ECB_TEST)<<ECB_IS;
	cm_ulTestMask2 = ((cm_penMoving->en_ulCollisionFlags&ECF_ISMASK  )>>ECB_IS  )<<ECB_TEST;

	cm_ulPassMaskA = ((cm_penMoving->en_ulCollisionFlags&ECF_PASSMASK)>>ECB_PASS)<<ECB_IS;
	cm_ulPassMaskB = ((cm_penMoving->en_ulCollisionFlags&ECF_ISMASK  )>>ECB_IS  )<<ECB_PASS;

	// cache near polygons of zoning brushes
	CacheNearPolygons();

	// clip to brush sectors near the entity
	ClipMoveToBrushes();
	// clip to models near the entity
	ClipMoveToModels();
}


/*
 * Test if a movement is clipped by something and where.
 */
void CWorld::ClipMove(CClipMove &cmMove)
{
	cmMove.ClipMoveToWorld(this);
}
