#include "stdh.h"
#include <Engine/Base/FileName.h>
#include <Engine/Terrain/Terrain.h>
#include <Engine/Terrain/Terrain.hpp>
#include <Engine/Terrain/TRDataArray.hpp>
#include <Engine/Terrain/TRMisc.hpp>
#include <Engine/Terrain/TerrainTile.hpp>
#include <Engine/Math/Matrix12.h>
#include <Engine/Entities/ShadingInfo.h>
#include <Engine/Entities/Entity.h>
#include <Engine/Brushes/Brush.h>

extern TerrainInfo _tiTerrainInfo;
static CStaticStackArray<FLOAT3D>	_avxExtractedVertices;
static CStaticStackArray<UBYTE>		_aubExtractedEdges;
static CStaticStackArray<UWORD>		_auwExtractedIndices;
static CStaticStackArray<GFXColor>	_acExtractedColors;			// yjpark

#if TR_DEBUG_EXTRACTPOLY
CStaticStackArray<FLOAT3D> _avxExtractedPolys;
CStaticStackArray<UWORD>   _auwExtractedPolys;
CStaticStackArray<CTRect> _arcExtractedPolys;
#endif

extern CTRect RelRectFromAbsBBox(const CTerrainImp *ptr, const FLOATaabbox3D &bbox, const Matrix12 &mTransf);

// Extract vertices and triangles from specified rectange on height map
extern void TR_ExtractVertices(CTerrainImp *ptr, CTRect rcSource,
															 CStaticArray<FLOAT3D> &avxVertices)
{
	ASSERT(ptr!=NULL);
	ASSERT(ptr->tr_puwHeightMap!=NULL);
	ASSERT(rcSource.rc_slLeft>=0);
	ASSERT(rcSource.rc_slTop>=0);
	ASSERT(rcSource.rc_slRight<=ptr->tr_pixHeightMapWidth);
	ASSERT(rcSource.rc_slBottom<=ptr->tr_pixHeightMapHeight);
	ASSERT(rcSource.GetWidth()>0);
	ASSERT(rcSource.GetHeight()>0);

	const UWORD *puwHeightMap      = ptr->tr_puwHeightMap;
	const PIX    pixHeightMapWidth = ptr->tr_pixHeightMapWidth;

	const SLONG slLeft   = rcSource.rc_slLeft;
	const SLONG slTop    = rcSource.rc_slTop;
	const SLONG slWidth  = rcSource.GetWidth();
	const SLONG slHeight = rcSource.GetHeight();

	const INDEX ctVertices = slWidth * slHeight;

	INDEX iVertex = 0;
	// for each pixel
	for(INDEX iZ=0;iZ<slHeight;iZ++) {
		for(INDEX iX=0;iX<slWidth;iX++) {
			// Get pixel
			const SLONG slX = iX+slLeft;
			const SLONG slZ = iZ+slTop;
			SLONG slPixel  = slX + slZ*pixHeightMapWidth;
			UWORD uwHeight = puwHeightMap[slPixel];
			// Fill vertex data
			FLOAT3D &vx = avxVertices[iVertex];
			vx(1) = slX;
			vx(2) = uwHeight;
			vx(3) = slZ;
			// next vertex
			iVertex++;
		}
	}
}

//강동민 수정 시작
//extern void TR_ExtractPoligonsInRect(const CTerrain *ptrTerrain, const CTRect &rcExtract, 
//																		FLOAT3D **pavRetVtx, INDEX &ctRetVertices, UWORD **puwRetInd, INDEX &ctRetIndices)
extern void TR_ExtractPoligonsInRect(const CTerrain *ptrTerrain, CTRect &rcExtract, 
									 FLOAT3D **pavRetVtx, INDEX &ctRetVertices, UWORD **puwRetInd, INDEX &ctRetIndices)
//강동민 수정 끝
{
	ASSERT(ptrTerrain!=NULL);
	ASSERT(ptrTerrain->tr_ptrTerrain!=NULL);
	ASSERT(ptrTerrain->tr_ptrTerrain->tr_puwHeightMap!=NULL);
	
//강동민 수정 시작
	// 원본
	//ASSERT(rcExtract.rc_slLeft>=0);
	//ASSERT(rcExtract.rc_slTop>=0);
	//ASSERT(rcExtract.rc_slRight<=ptrTerrain->tr_ptrTerrain->tr_pixHeightMapWidth);
	//ASSERT(rcExtract.rc_slBottom<=ptrTerrain->tr_ptrTerrain->tr_pixHeightMapHeight);

	// 수정
	if(rcExtract.rc_slLeft < 0)	rcExtract.rc_slLeft = 0;
	if(rcExtract.rc_slTop < 0)	rcExtract.rc_slTop = 0;

	if(rcExtract.rc_slRight >ptrTerrain->tr_ptrTerrain->tr_pixHeightMapWidth)
		rcExtract.rc_slRight = ptrTerrain->tr_ptrTerrain->tr_pixHeightMapWidth;

	if(rcExtract.rc_slBottom >ptrTerrain->tr_ptrTerrain->tr_pixHeightMapHeight)
		rcExtract.rc_slBottom = ptrTerrain->tr_ptrTerrain->tr_pixHeightMapHeight;	
//강동민 수정 끝
	const CTerrainImp *ptr = ptrTerrain->tr_ptrTerrain;

	const UWORD *puwHeightMap      = ptr->tr_puwHeightMap;
	const UBYTE *pubEdgeMap        = ptr->tr_pubEdgeMap;
	const PIX    pixHeightMapWidth = ptr->tr_pixHeightMapWidth;
	const PIX    pixHeightMapHeight= ptr->tr_pixHeightMapHeight;
	const PIX    pixEdgeMapWidth   = pixHeightMapWidth-1;
	const PIX    pixEdgeMapHeight  = pixHeightMapHeight-1;
	const SLONG  slHeightMapSize   = pixHeightMapWidth * pixHeightMapHeight;
	const SLONG  slEdgeMapSize     = pixEdgeMapWidth * pixEdgeMapHeight;
	const FLOAT3D vStretch         = ptr->tr_vStretch;

	const SLONG slLeft   = rcExtract.rc_slLeft;
	const SLONG slTop    = rcExtract.rc_slTop;
	const SLONG slWidth  = rcExtract.GetWidth();
	const SLONG slHeight = rcExtract.GetHeight();

	const INDEX ctqdsX = slWidth;
	const INDEX ctqdsZ = slHeight;
	const INDEX ctvtxX = slWidth +1;
	const INDEX ctvtxZ = slHeight+1;

	const INDEX ctvx  = ctvtxX * ctvtxZ;
	const INDEX ctind = ctqdsX * ctqdsZ * 6;
	if(ctind<=0 || ctvx<=0 || (ctqdsX < 0 && ctqdsZ < 0)) {
		*pavRetVtx = NULL;
		*puwRetInd = NULL;
		ctRetVertices = 0;
		ctRetIndices  = 0;
		return;
	}

	_avxExtractedVertices.PopAll();
	_aubExtractedEdges.PopAll();
	_avxExtractedVertices.Push(ctvx);
	_aubExtractedEdges.Push(ctvx);
	_auwExtractedIndices.PopAll();
	_auwExtractedIndices.Push(ctind);

	INDEX iVertex = 0;
	// for each vertex
	for(INDEX iZ=0;iZ<ctvtxZ;iZ++) 
	{
		for(INDEX iX=0;iX<ctvtxX;iX++) 
		{
			// Get pixel
//강동민 수정 시작
			// 원본
			/*
			const SLONG slX         = iX+slLeft;
			const SLONG slZ         = iZ+slTop;
			const SLONG slPixel     = slX + slZ*pixHeightMapWidth;
			const SLONG slEdgePixel = ClampUp(slX, pixEdgeMapWidth-1L) + ClampUp(slZ, pixEdgeMapHeight-1L)*pixEdgeMapWidth;
			*/
			SLONG slX         = iX+slLeft;			
			SLONG slZ         = iZ+slTop;
			SLONG slPixel     = slX + slZ*pixHeightMapWidth;
			SLONG slEdgePixel = ClampUp(slX, pixEdgeMapWidth-1L) + ClampUp(slZ, pixEdgeMapHeight-1L)*pixEdgeMapWidth;
//강동민 수정 끝
			
//강동민 수정 시작
			if(slEdgePixel < 0)					slEdgePixel = 0;
			if(slPixel < 0)						slPixel = 0;
			if(slEdgePixel >= slEdgeMapSize)	slEdgePixel = slEdgeMapSize;
			if(slPixel >= slHeightMapSize)		slPixel = slHeightMapSize;
			// 원본
			//ASSERT(slEdgePixel>=0 && slEdgePixel<slEdgeMapSize);
			//ASSERT(slPixel>=0 && slPixel<slHeightMapSize);
//강동민 수정 끝

			const UWORD uwHeight    = puwHeightMap[slPixel];
			const UBYTE ubEdgeVal   = pubEdgeMap[slEdgePixel];
			// Fill vertex data
			FLOAT3D &vx = _avxExtractedVertices[iVertex];
			vx(1) = slX      * vStretch(1);
			vx(2) = uwHeight * vStretch(2);
			vx(3) = slZ      * vStretch(3);
			// Fill shade data
			UBYTE   &ubMask = _aubExtractedEdges[iVertex];
			ubMask = ubEdgeVal;
			// next vertex
			iVertex++;
		}
	}

	const INDEX iFirstVertex = slLeft + slTop*pixHeightMapWidth;
	INDEX ivx=0;
	INDEX iFacing = iFirstVertex;
	INDEX iQuad = 0;
	INDEX ctIndices = 0;
	// for each quad
	for(INDEX iqdZ=0;iqdZ<ctqdsZ;iqdZ++) {
		for(INDEX iqdX=0;iqdX<ctqdsX;iqdX++) {
			const UBYTE ubMask = _aubExtractedEdges[ivx];
			if(iFacing&1) {
				if(ubMask==255) {
					_auwExtractedIndices[ctIndices+0] = ivx;
					_auwExtractedIndices[ctIndices+1] = ivx+ctvtxX;
					_auwExtractedIndices[ctIndices+2] = ivx+1;
					ctIndices+=3;

					_auwExtractedIndices[ctIndices+0] = ivx+1;
					_auwExtractedIndices[ctIndices+1] = ivx+ctvtxX;
					_auwExtractedIndices[ctIndices+2] = ivx+ctvtxX+1;
					ctIndices+=3;
				}
			} else {
				if(ubMask==255) {
					_auwExtractedIndices[ctIndices+0] = ivx+ctvtxX;
					_auwExtractedIndices[ctIndices+1] = ivx+ctvtxX+1;
					_auwExtractedIndices[ctIndices+2] = ivx;
					ctIndices+=3;

					_auwExtractedIndices[ctIndices+0] = ivx;
					_auwExtractedIndices[ctIndices+1] = ivx+ctvtxX+1;
					_auwExtractedIndices[ctIndices+2] = ivx+1;
					ctIndices+=3;
				}
			}
			iFacing++;
			ivx++;
		}
		if(ctvtxX&1) iFacing++;
		ivx++;
	}

	if(ctIndices<ctind) {
		_auwExtractedIndices.PopUntil(ctIndices);
	}

#if TR_DEBUG_EXTRACTPOLY
	{
	_arcExtractedPolys.Push() = rcExtract;
	INDEX ctFirstVertex = _avxExtractedPolys.Count();
	FLOAT3D *pvxFirst = _avxExtractedPolys.Push(ctvx);
	UWORD   *puwFirst = _auwExtractedPolys.Push(ctIndices);
	for(INDEX ivx=0;ivx<ctvx;ivx++) {
		pvxFirst[ivx] = _avxExtractedVertices[ivx];
	}
	for(INDEX iind=0;iind<ctIndices;iind++) {
		puwFirst[iind] = _auwExtractedIndices[iind] + ctFirstVertex;
	}
	}
#endif

	if(ctvx>0) {
		ctRetVertices = ctvx;
		*pavRetVtx = &_avxExtractedVertices[0];
	} else {
		ctRetVertices = 0;
		*pavRetVtx = NULL;
	}

	if(ctIndices>0) {
		ctRetIndices = ctIndices;
		*puwRetInd = &_auwExtractedIndices[0];
	} else {
		ctRetIndices = 0;
		*puwRetInd = NULL;
	}
}

extern void TR_ExtractPolygonsInBox(const CTerrain *ptrTerrain, const FLOATaabbox3D &bboxExtract,
																		const Matrix12 &mObjToAbs, FLOAT3D **pavVtx, INDEX &ctVtx,
																		UWORD **puwInd, INDEX &ctIndices)
{
	ASSERT(ptrTerrain!=NULL);
	ASSERT(ptrTerrain->tr_ptrTerrain!=NULL);
	const CTerrainImp *ptr = ptrTerrain->tr_ptrTerrain;

	Matrix12 mInvObjToAbs;
	MatrixTranspose(mInvObjToAbs,mObjToAbs);
	CTRect rcExtract = RelRectFromAbsBBox(ptr,bboxExtract,mInvObjToAbs);
	TR_ExtractPoligonsInRect(ptrTerrain,rcExtract,pavVtx,ctVtx,puwInd,ctIndices);
}

extern FLOAT2D TR_CalcShadingTC(const CTerrain *ptrTerrain, const FLOAT3D &vPointAbs, const Matrix12 &mObjToAbs)
{
	ASSERT(ptrTerrain!=NULL);
	ASSERT(ptrTerrain->tr_ptrTerrain!=NULL);
	const CTerrainImp *ptr = ptrTerrain->tr_ptrTerrain;

	const PIX pixHeightMapWidth = ptr->tr_pixHeightMapWidth;
	const PIX pixHeightMapHeight = ptr->tr_pixHeightMapHeight;
	const SLONG slShadowMapSizeAspect = ptr->tr_slShadowMapSizeAspect;
	const SLONG slShadingMapSizeAspect = ptr->tr_slShadingMapSizeAspect;
	const PIX pixShadowMapWidth  = GetShadowMapWidth(pixHeightMapWidth, slShadowMapSizeAspect);
	const PIX pixShadowMapHeight = GetShadowMapHeight(pixHeightMapHeight, slShadowMapSizeAspect);
	// const PIX pixShadingMapWidth = GetShadingMapWidth(pixHeightMapWidth, slShadowMapSizeAspect);
	// const PIX pixShadingMapHeight = GetShadingMapHeight(pixHeightMapHeight, slShadowMapSizeAspect);

	// Get relative hit point
	// const FLOAT3D vRelPoint = (vPointAbs-vPosition) * !mRotation;
	FLOAT3D vRelPoint = vPointAbs;
	Matrix12 mInvObjToAbs;
	MatrixTranspose(mInvObjToAbs,mObjToAbs);
	TransformVector(vRelPoint.vector,mInvObjToAbs);


	// Unstretch hit point and convert it to 2d point in shading map
	FLOAT fX = vRelPoint(1) / ptr->tr_vStretch(1);
	FLOAT fY = vRelPoint(3) / ptr->tr_vStretch(3);
	FLOAT fU = fX / ((FLOAT)(pixHeightMapWidth)  / pixShadowMapWidth);
	FLOAT fV = fY / ((FLOAT)(pixHeightMapHeight) / pixShadowMapHeight);
	
//강동민 수정 시작
	// 원본
	//ASSERT(fU>0.0f && fU<pixShadowMapWidth);
	//ASSERT(fV>0.0f && fV<pixShadowMapHeight);
	// 강제로 경계값을 맞춤.
	if(fU < 0.0f)					fU = 0.0f;
	if(fV < 0.0f)					fV = 0.0f;

	if(fU >= pixShadowMapWidth)	fU = pixShadowMapWidth;
	if(fV >= pixShadowMapHeight)	fV = pixShadowMapHeight;
//강동민 수정 끝
	return FLOAT2D(fU,fV);
	return FLOAT2D(0,0);
}

extern void TR_GetMatrixFromEntity(Matrix12 &m, const CTerrain *ptrTerrain)
{
	ASSERT(ptrTerrain!=NULL);
	ASSERT(ptrTerrain->tr_penEntity!=NULL);

	const CEntity *pen = ptrTerrain->tr_penEntity;
	const FLOATmatrix3D &mRotation = pen->en_mRotation;
	const FLOAT3D &vTranslation = pen->en_plPlacement.pl_PositionVector;
	MatrixVectorToMatrix12(m, mRotation, vTranslation);
}

// yjpark |<--
//-------------------------------------------------------------------------------------------------
// Desc : These contents aren't included in final release version
// ------------------------------------------------------------------------------------------------
#ifndef FINALVERSION

// Get color for shading info
extern COLOR TR_GetShadeColor(const CTerrain *ptrTerrain, CShadingInfo *psi)
{
	ASSERT(psi!=NULL);
	ASSERT(ptrTerrain!=NULL);
	ASSERT(ptrTerrain->tr_ptrTerrain!=NULL);
	ASSERT(ptrTerrain->tr_ptrTerrain->tr_ptdShadowMap->td_pulFrames!=NULL);

	const CTerrainImp *ptr = ptrTerrain->tr_ptrTerrain;

	const PIX pixHeightMapWidth = ptr->tr_pixHeightMapWidth;
	const PIX pixHeightMapHeight = ptr->tr_pixHeightMapHeight;
	const SLONG slShadowMapSizeAspect = ptr->tr_slShadowMapSizeAspect;
	const SLONG slShadingMapSizeAspect = ptr->tr_slShadingMapSizeAspect;
	const PIX pixShadowMapWidth  = GetShadowMapWidth(pixHeightMapWidth, slShadowMapSizeAspect);
	const PIX pixShadowMapHeight = GetShadowMapHeight(pixHeightMapHeight, slShadowMapSizeAspect);
	// const PIX pixShadingMapWidth = GetShadingMapWidth(pixHeightMapWidth, slShadowMapSizeAspect);
	// const PIX pixShadingMapHeight = GetShadingMapHeight(pixHeightMapHeight, slShadowMapSizeAspect);

	const PIX pixShadowU = Clamp(psi->si_pixShadowU,0L,pixShadowMapWidth-2L);
	const PIX pixShadowV = Clamp(psi->si_pixShadowV,0L,pixShadowMapHeight-2L);
	const FLOAT fUDRatio = psi->si_fUDRatio;
	const FLOAT fLRRatio = psi->si_fLRRatio;
	
	const PIX pixShadow = pixShadowU + pixShadowV*pixShadowMapWidth;
	const ULONG *pulShadingData = ptr->tr_ptdShadowMap->td_pulFrames;

	const ULONG aulShade[4] = {
		pulShadingData[pixShadow+0],
		pulShadingData[pixShadow+1],
		pulShadingData[pixShadow+pixShadowMapWidth+0],
		pulShadingData[pixShadow+pixShadowMapWidth+1],
	};

	SLONG aslr[4], aslg[4], aslb[4];
	for(INDEX ish=0;ish<4;ish++) {
		aslr[ish] = (aulShade[ish]&0x000000FF)>> 0;
		aslg[ish] = (aulShade[ish]&0x0000FF00)>> 8;
		aslb[ish] = (aulShade[ish]&0x00FF0000)>>16;
	}

	SLONG slRed   = Lerp( Lerp(aslr[0], aslr[1], fLRRatio), Lerp(aslr[2], aslr[3], fLRRatio), fUDRatio);
	SLONG slGreen = Lerp( Lerp(aslg[0], aslg[1], fLRRatio), Lerp(aslg[2], aslg[3], fLRRatio), fUDRatio);
	SLONG slBlue  = Lerp( Lerp(aslb[0], aslb[1], fLRRatio), Lerp(aslb[2], aslb[3], fLRRatio), fUDRatio);

	ULONG ulPixel = ((slRed  <<24)&0xFF000000) |
									((slGreen<<16)&0x00FF0000) | 
									((slBlue << 8)&0x0000FF00) | 0xFF;

	return ulPixel;
}

extern CTString TR_GetTerrainInfo(const CTerrain *ptrTerrain)
{
	CTString strInfo;
	strInfo  = CTString(0, "Qtns - %d\n", _tiTerrainInfo.ti_ctVisibleNodes);
	strInfo += CTString(0, "Tris - %d\n", _tiTerrainInfo.ti_ctVisibleTriangles);
	strInfo += CTString(0, "Vtxs - %d\n", _tiTerrainInfo.tr_ctVisibleVertices);
	strInfo += CTString(0, "Tiles edge tested - %d\n", _tiTerrainInfo.ti_ctTileEdgeTested);
	strInfo += CTString(0, "Tiles edge rejected - %d\n", _tiTerrainInfo.ti_ctTileEdgeRejected);
	strInfo += CTString(0, "Tile layers skiped - %d\n", _tiTerrainInfo.ti_ctTileLayersSkiped);
	strInfo += CTString(0, "GeometryBuilds - %d\n", _tiTerrainInfo.ti_ctGeometryBuilds);
	strInfo += CTString(0, "Geometry(NB/TT) -  %d/%d - %g\n",_tiTerrainInfo.ti_ctNeighborsQueued, _tiTerrainInfo.ti_ctTileQueued, (FLOAT)_tiTerrainInfo.ti_ctNeighborsQueued/_tiTerrainInfo.ti_ctTileQueued);
	return strInfo;
}

extern CTPoint TR_RelFromAbsPoint(const CTerrain *ptrTerrain, const FLOAT3D &vPointAbs, const Matrix12 &mObjToAbs,
								  BufferType btType, SLONG slStretch)		// yjpark
{
	ASSERT(ptrTerrain!=NULL);
	ASSERT(ptrTerrain->tr_ptrTerrain!=NULL);
	const CTerrainImp *ptr = ptrTerrain->tr_ptrTerrain;

	// Get relative hit point
	FLOAT3D vRelPoint = vPointAbs;
	Matrix12 mInvObjToAbs;
	MatrixTranspose(mInvObjToAbs,mObjToAbs);
	TransformVector(vRelPoint.vector,mInvObjToAbs);

	CTPoint pt;
	if( btType == BT_LAYER_MASK || btType == BT_SHADOW_MAP )		// yjpark |<--
	{
		FLOAT	fStretch;
		if( slStretch < 0 )
			fStretch = (FLOAT)( 1 << (-slStretch) );
		else
			fStretch = 1.0f / (FLOAT)( 1 << slStretch );

		pt(1) = vRelPoint(1) / ( ptr->tr_vStretch(1) * fStretch );
		pt(2) = vRelPoint(3) / ( ptr->tr_vStretch(3) * fStretch );
	}
	else if( btType == BT_ATTRIBUTE_MAP )
	{
		pt(1) = vRelPoint(1);
		pt(2) = vRelPoint(3);
	}
	else
	{
		pt(1) = ceil(vRelPoint(1)/ptr->tr_vStretch(1) - 0.5f);
		pt(2) = ceil(vRelPoint(3)/ptr->tr_vStretch(3) - 0.5f);
	}																// yjpark     -->|
	return pt;
}

#include <d3dx8math.h>
extern FLOAT TR_GetHeight( CTerrainImp *ptr, FLOAT fX, FLOAT fZ )
{
	ASSERT( ptr != NULL );
	ASSERT( !( fX < 0 || fX > ptr->tr_vMetricSize( 1 ) ) );
	ASSERT( !( fZ < 0 || fZ > ptr->tr_vMetricSize( 3 ) ) );

	UWORD	*puwHeightMap = ptr->tr_puwHeightMap;
	PIX		pixHeightMapWidth = ptr->tr_pixHeightMapWidth;
	SLONG	slCellSize = ptr->tr_slAttributeMapSizeAspect;
	FLOAT	fHeightStretch = ptr->tr_vStretch( 2 );

	PIX		pixX = (PIX)( fX / slCellSize );
	PIX		pixZ = (PIX)( fZ / slCellSize );
	FLOAT	fRemainedX = fX - (FLOAT)( pixX * slCellSize );
	FLOAT	fRemainedZ = fZ - (FLOAT)( pixZ * slCellSize );

	BOOL bLRDiagonal = TRUE;		// Lower right diagonal shape
	if( ( pixX & 0x01 ) && !( pixZ & 0x01 ) || !( pixX & 0x01 ) && ( pixZ & 0x01 ) )
		bLRDiagonal = FALSE;

	D3DXPLANE	plTemp;
	D3DXVECTOR3	vP0, vP1, vP2;

	if( bLRDiagonal )
	{
		vP1 = D3DXVECTOR3( pixX * slCellSize,
							puwHeightMap[ pixX + pixZ * pixHeightMapWidth] * fHeightStretch,
							pixZ * slCellSize );
		vP2 = D3DXVECTOR3( ( pixX + 1 ) * slCellSize,
							puwHeightMap[pixX + 1 + ( pixZ + 1 ) * pixHeightMapWidth] * fHeightStretch,
							( pixZ + 1 ) * slCellSize );

		if( fRemainedX > fRemainedZ )		// Upper right triangle
		{
			vP0 = D3DXVECTOR3( ( pixX + 1 ) * slCellSize,
								puwHeightMap[pixX + 1 + pixZ * pixHeightMapWidth] * fHeightStretch,
								pixZ * slCellSize );
		}
		else								// Lower left triangle
		{
			vP0 = D3DXVECTOR3( pixX * slCellSize,
								puwHeightMap[pixX + ( pixZ + 1 ) * pixHeightMapWidth] * fHeightStretch,
								( pixZ + 1 ) * slCellSize );
		}
	}
	else
	{
		vP1 = D3DXVECTOR3( ( pixX + 1 ) * slCellSize,
							puwHeightMap[ pixX + 1 + pixZ * pixHeightMapWidth] * fHeightStretch,
							pixZ * slCellSize );
		vP2 = D3DXVECTOR3( pixX * slCellSize,
							puwHeightMap[pixX + ( pixZ + 1 ) * pixHeightMapWidth] * fHeightStretch,
							( pixZ + 1 ) * slCellSize );

		fRemainedX = slCellSize - fRemainedX;
		if( fRemainedX > fRemainedZ )		// Upper left triangle
		{
			vP0 = D3DXVECTOR3( pixX * slCellSize,
								puwHeightMap[pixX + pixZ * pixHeightMapWidth] * fHeightStretch,
								pixZ * slCellSize );
		}
		else								// Lower right triangle
		{
			vP0 = D3DXVECTOR3( ( pixX + 1 ) * slCellSize,
								puwHeightMap[pixX + 1 + ( pixZ + 1 ) * pixHeightMapWidth] * fHeightStretch,
								( pixZ + 1 ) * slCellSize );
		}
	}

	D3DXPlaneFromPoints( &plTemp, &vP0, &vP1, &vP2 );

	return -( plTemp.a * fX + plTemp.c * fZ + plTemp.d ) / plTemp.b;
}

extern void TR_ExtractAttrPoligonsInRect( CTerrainImp *ptr, CTRect &rcExtract, 
											FLOAT3D **pavRetVtx, INDEX &ctRetVertices, GFXColor **pacolRetCol,
											UWORD **puwRetInd, INDEX &ctRetIndices, UWORD ubAttr )
{
	ASSERT( ptr != NULL );

	const UWORD			*pubAttr = ptr->tr_pubAttributeMap;
	const SLONG			slAttrAspect = ptr->tr_slAttributeMapSizeAspect;
	const SLONG			slLeft = rcExtract.rc_slLeft * slAttrAspect;
	const SLONG			slTop = rcExtract.rc_slTop * slAttrAspect;;
	const SLONG			slWidth = rcExtract.GetWidth() * slAttrAspect;;
	const SLONG			slHeight = rcExtract.GetHeight() * slAttrAspect;;
	const INDEX			ctqdsX = slWidth;
	const INDEX			ctqdsZ = slHeight;
	const INDEX			ctvtxX = slWidth + 1;
	const INDEX			ctvtxZ = slHeight + 1;
	const INDEX			ctvx  = ctvtxX * ctvtxZ;
	const INDEX			ctind = ctqdsX * ctqdsZ * 6;
	if( ctind <= 0 || ctvx <= 0 )
	{
		*pavRetVtx = NULL;
		*puwRetInd = NULL;
		ctRetVertices = 0;
		ctRetIndices = 0;
		return;
	}

	_avxExtractedVertices.PopAll();
	_acExtractedColors.PopAll();
	_avxExtractedVertices.Push( ctvx );
	_acExtractedColors.Push( ctvx );
	_auwExtractedIndices.PopAll();
	_auwExtractedIndices.Push( ctind );

	GFXColor	attrColor;
// 	switch( ubAttr )
// 	{
// 	case 255:
// 		attrColor.Set( 0xE42DDEAA );
// 		break;
// 	}

	if (ubAttr & MATT_UNWALKABLE)
		attrColor.Set(0xE42DDEAA);

	INDEX	iVertex = 0;
	// for each vertex
	for( INDEX iZ = 0; iZ < ctvtxZ; iZ++ )
	{
		SLONG	slZ = iZ + slTop;
		for( INDEX iX = 0; iX < ctvtxX; iX++ )
		{
			SLONG	slX = iX + slLeft;
			// Fill vertex data
			FLOAT3D	&vx = _avxExtractedVertices[iVertex];
			vx(1) = slX;
			vx(3) = slZ;
			vx(2) = TR_GetHeight( ptr, vx(1), vx(3) ) + 0.001f;

			// Fill color data
			_acExtractedColors[iVertex] = attrColor;

			// next vertex
			iVertex++;
		}
	}

	const PIX	pixAttrWidth = ptr->tr_vMetricSize( 1 );
	const INDEX	iFirstVertex = slLeft + slTop * pixAttrWidth;
	INDEX	ivx = 0;
	INDEX	iFacing = iFirstVertex;
	INDEX	iQuad = 0;
	INDEX	ctIndices = 0;
	// for each quad
	for( INDEX iqdZ = 0; iqdZ < ctqdsZ; iqdZ++ )
	{
		SLONG	slZ = iqdZ + slTop;
		
		for( INDEX iqdX = 0; iqdX < ctqdsX; iqdX++ )
		{
			SLONG	slX = iqdX + slLeft;
			if( pubAttr[slX + slZ * pixAttrWidth] == ubAttr )
			{
				if( iFacing & 1 )
				{
					_auwExtractedIndices[ctIndices+0] = ivx;
					_auwExtractedIndices[ctIndices+1] = ivx+ctvtxX;
					_auwExtractedIndices[ctIndices+2] = ivx+1;
					ctIndices+=3;

					_auwExtractedIndices[ctIndices+0] = ivx+1;
					_auwExtractedIndices[ctIndices+1] = ivx+ctvtxX;
					_auwExtractedIndices[ctIndices+2] = ivx+ctvtxX+1;
					ctIndices+=3;
				} else {
					_auwExtractedIndices[ctIndices+0] = ivx+ctvtxX;
					_auwExtractedIndices[ctIndices+1] = ivx+ctvtxX+1;
					_auwExtractedIndices[ctIndices+2] = ivx;
					ctIndices+=3;

					_auwExtractedIndices[ctIndices+0] = ivx;
					_auwExtractedIndices[ctIndices+1] = ivx+ctvtxX+1;
					_auwExtractedIndices[ctIndices+2] = ivx+1;
					ctIndices+=3;
				}
			}
			iFacing++;
			ivx++;
		}

		if( ctvtxX & 1) iFacing++;
		ivx++;
	}

	if( ctIndices < ctind )
		_auwExtractedIndices.PopUntil(ctIndices);

	if( ctvx > 0 )
	{
		ctRetVertices = ctvx;
		*pavRetVtx = &_avxExtractedVertices[0];
		*pacolRetCol = &_acExtractedColors[0];
	}
	else
	{
		ctRetVertices = 0;
		*pavRetVtx = NULL;
		*pacolRetCol = NULL;
	}

	if( ctIndices > 0 )
	{
		ctRetIndices = ctIndices;
		*puwRetInd = &_auwExtractedIndices[0];
	}
	else
	{
		ctRetIndices = 0;
		*puwRetInd = NULL;
	}
}

// [070705: Su-won] ATTRIBUTEMAP_BLOCK
extern void TR_ExtractAttrBlockPolygonsInRect( CTerrainImp *ptr, CTRect &rcExtract, 
											FLOAT3D **pavRetVtx, INDEX &ctRetVertices, 
											UWORD **puwRetInd, INDEX &ctRetIndices, float fHeight )
{
	ASSERT( ptr != NULL );

	const UWORD			*pubAttr = ptr->tr_pubAttributeMap;

	if( pubAttr == NULL )
		return;

	const SLONG			slHeightMapWidth = ptr->tr_pixHeightMapWidth;
	const SLONG			slHeightMapHeight = ptr->tr_pixHeightMapHeight;
	const SLONG			slAttrAspect = ptr->tr_slAttributeMapSizeAspect;

	rcExtract.rc_slLeft = rcExtract.rc_slLeft < 1 ? 1 : rcExtract.rc_slLeft;
	rcExtract.rc_slRight = rcExtract.rc_slRight > (slHeightMapWidth-1)*slAttrAspect -2 ? (slHeightMapWidth-1)*slAttrAspect -2 : rcExtract.rc_slRight;
	rcExtract.rc_slTop = rcExtract.rc_slTop < 1 ? 1 : rcExtract.rc_slTop;
	rcExtract.rc_slBottom = rcExtract.rc_slBottom > (slHeightMapWidth-1)*slAttrAspect -2 ? (slHeightMapWidth-1)*slAttrAspect -2 : rcExtract.rc_slBottom;

	const SLONG			slLeft = rcExtract.rc_slLeft;
	const SLONG			slTop = rcExtract.rc_slTop;
	const SLONG			slWidth = rcExtract.GetWidth();
	const SLONG			slHeight = rcExtract.GetHeight();
	const INDEX			ctqdsX = slWidth;
	const INDEX			ctqdsZ = slHeight;
	const INDEX			ctvtxX = slWidth + 1;
	const INDEX			ctvtxZ = slHeight + 1;
	const INDEX			ctvx  = ctvtxX * ctvtxZ *2;
//	const INDEX			ctind = ctqdsX * (ctqdsZ+1) *2 * 6;
	const INDEX			ctind = ctqdsX * (ctqdsZ+1) * 6 * 4 *2;

	if( ctind <= 0 || ctvx <= 0 )
	{
		*pavRetVtx = NULL;
		*puwRetInd = NULL;
		ctRetVertices = 0;
		ctRetIndices = 0;
		return;
	}

	_avxExtractedVertices.PopAll();
	_avxExtractedVertices.Push( ctvx );
	_auwExtractedIndices.PopAll();
	_auwExtractedIndices.Push( ctind );

	INDEX	iVertex = 0;
	// for each vertex
	for( INDEX iZ = 0; iZ < ctvtxZ; iZ++ )
	{
		SLONG	slZ = iZ + slTop;
		for( INDEX iX = 0; iX < ctvtxX; iX++ )
		{
			SLONG	slX = iX + slLeft;
			// Fill vertex data
			FLOAT3D	&vx = _avxExtractedVertices[iVertex];
			vx(1) = slX;
			vx(3) = slZ;
			vx(2) = TR_GetHeight( ptr, vx(1), vx(3) ) - fHeight;

			FLOAT3D	&vx2 = _avxExtractedVertices[iVertex +(ctvtxX * ctvtxZ)];
			vx2(1) = slX;
			vx2(3) = slZ;
			vx2(2) = TR_GetHeight( ptr, vx2(1), vx2(3) ) + fHeight;

			// next vertex
			iVertex++;
		}
	}

	const PIX	pixAttrWidth = ptr->tr_vMetricSize( 1 );
	const INDEX	iFirstVertex = slLeft + slTop * pixAttrWidth;
	INDEX	ivx = 0;
	INDEX	iQuad = 0;
	INDEX	ctIndices = 0;

	// for each quad
	for( INDEX iqdZ = 0; iqdZ < ctqdsZ; iqdZ++ )
	{
		SLONG	slZ = iqdZ + slTop;
		
		for( INDEX iqdX = 0; iqdX < ctqdsX; iqdX++ )
		{
			SLONG	slX = iqdX + slLeft;
			if (pubAttr[slX + slZ* pixAttrWidth] & MATT_UNWALKABLE)
			{
				//Top Edge
				//if( pubAttr[slX + (slZ-1) * pixAttrWidth] != 255)
				{
					_auwExtractedIndices[ctIndices+0] = ivx + (ctvtxX * ctvtxZ);
					_auwExtractedIndices[ctIndices+1] = ivx;
					_auwExtractedIndices[ctIndices+2] = ivx + (ctvtxX * ctvtxZ) +1;
					ctIndices+=3;

					_auwExtractedIndices[ctIndices+0] = ivx + (ctvtxX * ctvtxZ) +1;
					_auwExtractedIndices[ctIndices+1] = ivx;
					_auwExtractedIndices[ctIndices+2] = ivx+1;
					ctIndices+=3;
				}
				//Left Edge
				//if( pubAttr[slX-1 + slZ * pixAttrWidth] != 255)
				{
					_auwExtractedIndices[ctIndices+0] = ivx+ctvtxX +(ctvtxX * ctvtxZ);
					_auwExtractedIndices[ctIndices+1] = ivx+ctvtxX;
					_auwExtractedIndices[ctIndices+2] = ivx+(ctvtxX * ctvtxZ);
					ctIndices+=3;

					_auwExtractedIndices[ctIndices+0] = ivx+(ctvtxX * ctvtxZ);
					_auwExtractedIndices[ctIndices+1] = ivx+ctvtxX;
					_auwExtractedIndices[ctIndices+2] = ivx;
					ctIndices+=3;
				}

				// Right Edge
				//if( pubAttr[slX+1 + slZ * pixAttrWidth] != 255)
				{
					_auwExtractedIndices[ctIndices+0] = ivx+1 +(ctvtxX * ctvtxZ);
					_auwExtractedIndices[ctIndices+1] = ivx+1;
					_auwExtractedIndices[ctIndices+2] = ivx+1 +ctvtxX+(ctvtxX * ctvtxZ);
					ctIndices+=3;

					_auwExtractedIndices[ctIndices+0] = ivx+1 +ctvtxX+(ctvtxX * ctvtxZ);
					_auwExtractedIndices[ctIndices+1] = ivx+1;
					_auwExtractedIndices[ctIndices+2] = ivx+1+ctvtxX;
					ctIndices+=3;
				}

				// Bottom Edge
				//if( pubAttr[slX + (slZ-1) * pixAttrWidth] != 255)
				{
					_auwExtractedIndices[ctIndices+0] = ivx+1 +ctvtxX +(ctvtxX * ctvtxZ);
					_auwExtractedIndices[ctIndices+1] = ivx+1 +ctvtxX;
					_auwExtractedIndices[ctIndices+2] = ivx +ctvtxX +(ctvtxX * ctvtxZ);
					ctIndices+=3;

					_auwExtractedIndices[ctIndices+0] = ivx +ctvtxX +(ctvtxX * ctvtxZ);
					_auwExtractedIndices[ctIndices+1] = ivx+1 +ctvtxX;
					_auwExtractedIndices[ctIndices+2] = ivx+ctvtxX;
					ctIndices+=3;
				}
			}
			ivx++;
		}
		ivx++;
	}

	if( ctIndices < ctind )
		_auwExtractedIndices.PopUntil(ctIndices);

	if( ctvx > 0 )
	{
		ctRetVertices = ctvx;
		*pavRetVtx = &_avxExtractedVertices[0];
	}
	else
	{
		ctRetVertices = 0;
		*pavRetVtx = NULL;
	}

	if( ctIndices > 0 )
	{
		ctRetIndices = ctIndices;
		*puwRetInd = &_auwExtractedIndices[0];
	}
	else
	{
		ctRetIndices = 0;
		*puwRetInd = NULL;
	}
}










#endif // FINALVERSION
// yjpark     -->|