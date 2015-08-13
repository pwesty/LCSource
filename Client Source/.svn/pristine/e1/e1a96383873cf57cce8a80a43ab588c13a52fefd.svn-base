#include "stdh.h"
#include <Engine/Math/Placement.h>
#include <Engine/Math/AABBox.h>
#include <Engine/Math/Matrix12.h>
#include <Engine/Base/Rect.h>
#include <Engine/Graphics/ImageInfo.h>
#include <Engine/Base/ErrorReporting.h>
#include <Engine/Terrain/Terrain.h>
#include <Engine/Terrain/Terrain.hpp>
#include <Engine/Terrain/TRShadows.hpp>
#include <Engine/Terrain/TRGenerate.hpp>
#include <Engine/Templates/StaticStackArray.cpp>

#include <Engine/Light/LightSource.h>
#include <Engine/World/World.h>
#include <Engine/Entities/Entity.h>
#include <Engine/Templates/DynamicContainer.h>
#include <Engine/Templates/DynamicContainer.cpp>

#include <Engine/World/WorldRayCasting.h>		// yjpark
#include <Engine/Terrain/TRMisc.hpp>			// yjpark

static CStaticStackArray<TerrainPointLight> _atplLights;        // Array of point lights that will be in applied in shadow map
static CStaticStackArray<TerrainDirectionalLight> _atdlLights;  // Array of directional lights that will be in applied in shadow map
static Matrix12 _mObjToAbs           = {0};
static Matrix12 _mObjToAbsStretch    = {0};
static Matrix12 _mInvObjToAbs        = {0};
static Matrix12 _mInvObjToAbsStretch = {0};

static FLOATaabbox3D _bboxTerrain;
static CTRect        _rcUpdateRect;
static BOOL _bPlacementSet = FALSE;
static BOOL _bShadowMapChanged = FALSE;

static INDEX	_iCurShadowMap = 0;
static BOOL		_bSkaShadow = FALSE;
static BOOL		_bMdlShadow = FALSE;
static BOOL		_bBrushShadow = FALSE;
static BOOL		_bTerrainShadow = FALSE;

#if TR_DEBUG_SHADOWMAP
 CStaticStackArray<FLOATaabbox3D> _abbLightRegion;
 CStaticStackArray<FLOATaabbox3D> _abbUpdateShadowMap;
 CStaticStackArray<CTRect>        _arcLightRegion;
 CStaticStackArray<CTRect>        _arcUpdateShadowMap;
#endif
#if TR_DEBUG_NORMALS
 CStaticStackArray<FLOAT3D> _anxNormals;
 CStaticStackArray<FLOAT3D> _avxVertices;
#endif

static inline FLOAT3D TransVector(FLOAT3D &v,const Matrix12 &m) {
	TransformVector(v.vector,m); \
	return v;
}

static FLOATaabbox3D RelBBoxFromAbsBBox(const FLOATaabbox3D &bboxAbs)
{
	// Transform to terrain space
	FLOATaabbox3D bboxRel;
	bboxRel  = TransVector(FLOAT3D(bboxAbs.minvect(1),bboxAbs.minvect(2),bboxAbs.minvect(3)),_mInvObjToAbsStretch);
	bboxRel |= TransVector(FLOAT3D(bboxAbs.minvect(1),bboxAbs.minvect(2),bboxAbs.maxvect(3)),_mInvObjToAbsStretch);
	bboxRel |= TransVector(FLOAT3D(bboxAbs.maxvect(1),bboxAbs.minvect(2),bboxAbs.minvect(3)),_mInvObjToAbsStretch);
	bboxRel |= TransVector(FLOAT3D(bboxAbs.maxvect(1),bboxAbs.minvect(2),bboxAbs.maxvect(3)),_mInvObjToAbsStretch);
	bboxRel |= TransVector(FLOAT3D(bboxAbs.minvect(1),bboxAbs.maxvect(2),bboxAbs.minvect(3)),_mInvObjToAbsStretch);
	bboxRel |= TransVector(FLOAT3D(bboxAbs.minvect(1),bboxAbs.maxvect(2),bboxAbs.maxvect(3)),_mInvObjToAbsStretch);
	bboxRel |= TransVector(FLOAT3D(bboxAbs.maxvect(1),bboxAbs.maxvect(2),bboxAbs.minvect(3)),_mInvObjToAbsStretch);
	bboxRel |= TransVector(FLOAT3D(bboxAbs.maxvect(1),bboxAbs.maxvect(2),bboxAbs.maxvect(3)),_mInvObjToAbsStretch);

	return bboxRel;
}

extern CTRect RelRectFromAbsBBox(const CTerrainImp *ptr, const FLOATaabbox3D &bbox, const Matrix12 &mTransf)
{
	// Transform to terrain space
	FLOATaabbox3D bboxRel;
	bboxRel  = TransVector(FLOAT3D(bbox.minvect(1),bbox.minvect(2),bbox.minvect(3)),mTransf);
	bboxRel |= TransVector(FLOAT3D(bbox.minvect(1),bbox.minvect(2),bbox.maxvect(3)),mTransf);
	bboxRel |= TransVector(FLOAT3D(bbox.maxvect(1),bbox.minvect(2),bbox.minvect(3)),mTransf);
	bboxRel |= TransVector(FLOAT3D(bbox.maxvect(1),bbox.minvect(2),bbox.maxvect(3)),mTransf);
	bboxRel |= TransVector(FLOAT3D(bbox.minvect(1),bbox.maxvect(2),bbox.minvect(3)),mTransf);
	bboxRel |= TransVector(FLOAT3D(bbox.minvect(1),bbox.maxvect(2),bbox.maxvect(3)),mTransf);
	bboxRel |= TransVector(FLOAT3D(bbox.maxvect(1),bbox.maxvect(2),bbox.minvect(3)),mTransf);
	bboxRel |= TransVector(FLOAT3D(bbox.maxvect(1),bbox.maxvect(2),bbox.maxvect(3)),mTransf);

	FLOAT3D vMin = bboxRel.minvect;
	FLOAT3D vMax = bboxRel.maxvect;
	FLOAT3D vDiff = bboxRel.maxvect - bboxRel.minvect;

	// Stretch abs box
	const FLOAT3D vStretch = ptr->tr_vStretch;
	const FLOAT3D vInvStretch(1.0f/vStretch(1), 1.0f/vStretch(2), 1.0f/vStretch(3));
	vMin(1)  *= vInvStretch(1);
	vMin(2)  *= vInvStretch(2);
	vMin(3)  *= vInvStretch(3);
	vDiff(1) *= vInvStretch(1);
	vDiff(2) *= vInvStretch(2);
	vDiff(3) *= vInvStretch(3);
	FLOATaabbox3D bboxRetStretch(vMin,vMin+vDiff);


	// Convert to rect
	CTRect rcTerrain = CTRect(CTPoint(0,0),CTPoint(ptr->tr_pixHeightMapWidth-1,ptr->tr_pixHeightMapHeight-1));

	CTRect rcRel;
	rcRel.rc_slLeft   = floor(bboxRetStretch.minvect(1));
	rcRel.rc_slRight  = ceil (bboxRetStretch.maxvect(1));
	rcRel.rc_slTop    = floor(bboxRetStretch.minvect(3));
	rcRel.rc_slBottom = ceil (bboxRetStretch.maxvect(3));
	rcRel = ClampRect(rcRel,rcTerrain);
	return rcRel;
}

static void CalculateLightUpdateBoxes(CTerrainImp *ptr)
{
	// for each directional light
	const INDEX ctdl = _atdlLights.Count();
	for(INDEX idl=0;idl<ctdl;idl++) {
		TerrainDirectionalLight &tdl = _atdlLights[idl];
		// Transform light orientation in terrain space
		FLOAT3D nxLight;
		AnglesToDirectionVector(tdl.tdl_plPlacement.pl_OrientationAngle,nxLight);
		RotateVector(nxLight.vector,_mInvObjToAbs);
		tdl.tdl_vLightNormal = -nxLight.Normalize();
		tdl.tdl_ulFlags |= TLF_TOUCHES_TERRAIN;
	}

	// for each point light
	const INDEX ctpl = _atplLights.Count();
	for(INDEX ipl=0;ipl<ctpl;ipl++) {
		TerrainPointLight &tpl = _atplLights[ipl];
		// Transform light placement in terrain space
		FLOAT3D vPosition = tpl.tpl_plPlacement.pl_PositionVector;

		// Get light box in terrain space
		FLOATaabbox3D bboxLightAbs = FLOATaabbox3D(vPosition,tpl.tpl_fFallOff);
		FLOATaabbox3D boxLight = RelBBoxFromAbsBBox(bboxLightAbs);

		tpl.tpl_bboxLight = boxLight;
		tpl.tpl_vLightPos = boxLight.Center();

		// if light box touches terrain box
		if(boxLight.HasContactWith(_bboxTerrain)) {
			// Convert light box to clamped light rect
			CTRect rcLight;
			rcLight.rc_slLeft   = floor(boxLight.minvect(1));
			rcLight.rc_slRight  = ceil(boxLight.maxvect(1));
			rcLight.rc_slTop    = floor(boxLight.minvect(3));
			rcLight.rc_slBottom = ceil(boxLight.maxvect(3));
			if(RectTouchesRect(rcLight,_rcUpdateRect)) {
				tpl.tpl_ulFlags  |= TLF_TOUCHES_TERRAIN;
				tpl.tpl_rcLight   = ClampRect(rcLight,_rcUpdateRect);
#if TR_DEBUG_SHADOWMAP
			_arcLightRegion.Push() = tpl.tpl_rcLight;
#endif
			} else {
				tpl.tpl_ulFlags &= ~TLF_TOUCHES_TERRAIN;
			}
		} else {
			int a=0;
		}
#if TR_DEBUG_SHADOWMAP
		// add it to show light region
		_abbLightRegion.Push() = boxLight;
#endif
	}
}

static void CalculateNormalFromPoint(CTerrainImp *ptr, const FLOAT fX, const FLOAT fZ, FLOAT3D &vxPos, FLOAT3D &nxNormal)
{
	const PIX pixX = (PIX)fX;
	const PIX pixZ = (PIX)fZ;
	const FLOAT fLerpX = fX - pixX;
	const FLOAT fLerpZ = fZ - pixZ;

	const PIX pixHeightMapWidth = ptr->tr_pixHeightMapWidth;
	const PIX pixheightMapHeight = ptr->tr_pixHeightMapHeight;
	const UWORD *puwHeightMap = &ptr->tr_puwHeightMap[0];
	const FLOAT3D &vStretch = ptr->tr_vStretch;
	// const FLOAT3D vStretch = FLOAT3D(1,1,1);

	const FLOAT fY0 = puwHeightMap[(pixX+0) + (pixZ+0)*pixHeightMapWidth];
	const FLOAT fY1 = puwHeightMap[(pixX+1) + (pixZ+0)*pixHeightMapWidth];
	const FLOAT fY2 = puwHeightMap[(pixX+0) + (pixZ+1)*pixHeightMapWidth];
	const FLOAT fY3 = puwHeightMap[(pixX+1) + (pixZ+1)*pixHeightMapWidth];

	const FLOAT3D avx[4] = {
		FLOAT3D((pixX+0)*vStretch(1), fY0*vStretch(2), (pixZ+0)*vStretch(3)),		// yjpark |<--
		FLOAT3D((pixX+1)*vStretch(1), fY1*vStretch(2), (pixZ+0)*vStretch(3)),
		FLOAT3D((pixX+0)*vStretch(1), fY2*vStretch(2), (pixZ+1)*vStretch(3)),
		FLOAT3D((pixX+1)*vStretch(1), fY3*vStretch(2), (pixZ+1)*vStretch(3))		// yjpark     -->|
	};

	const FLOAT fHDeltaX = Lerp(avx[1](2)-avx[0](2), avx[3](2)-avx[2](2), fLerpZ);
	const FLOAT fHDeltaZ = Lerp(avx[0](2)-avx[2](2), avx[1](2)-avx[3](2), fLerpX);
	const FLOAT fDeltaX  = avx[1](1) - avx[0](1);
	const FLOAT fDeltaZ  = avx[0](3) - avx[2](3);

	nxNormal(2) = sqrt(1 / (((fHDeltaX*fHDeltaX)/(fDeltaX*fDeltaX)) + ((fHDeltaZ*fHDeltaZ)/(fDeltaZ*fDeltaZ)) + 1));
	nxNormal(1) = sqrt(nxNormal(2)*nxNormal(2) * ((fHDeltaX*fHDeltaX) / (fDeltaX*fDeltaX)));
	nxNormal(3) = sqrt(nxNormal(2)*nxNormal(2) * ((fHDeltaZ*fHDeltaZ) / (fDeltaZ*fDeltaZ)));

	if (fHDeltaX>0) {
		nxNormal(1) = -nxNormal(1);
	}
	if (fHDeltaZ<0) {
		nxNormal(3) = -nxNormal(3);
	}
	ASSERT(Abs(nxNormal.Length()-1)<0.01);

	FLOAT fResX1 = Lerp(fY0,fY1,fLerpX);
	FLOAT fResX2 = Lerp(fY2,fY3,fLerpX);
	FLOAT fY     = Lerp(fResX1,fResX2,fLerpZ);

	vxPos(1) = fX*vStretch(1);
	vxPos(2) = fY*vStretch(2);
	vxPos(3) = fZ*vStretch(3);

#if TR_DEBUG_NORMALS
	_anxNormals.Push() = nxNormal;
	_avxVertices.Push() = vxPos;
#endif
}

static void ClearPartOfShadowMap(CTerrainImp *ptr, const CTRect &rcClear)
{
	const PIX pixHeightMapWidth = ptr->tr_pixHeightMapWidth;
	const PIX pixHeightMapHeight = ptr->tr_pixHeightMapHeight;
	const PIX pixShadowMapWidth = GetShadowMapWidth(pixHeightMapWidth,ptr->tr_slShadowMapSizeAspect);
	const PIX pixShadowMapHeight = GetShadowMapHeight(pixHeightMapHeight,ptr->tr_slShadowMapSizeAspect);
	const FLOAT fHeightShadowDiffX = (pixHeightMapWidth-1)/(FLOAT)pixShadowMapWidth;
	const FLOAT fHeightShadowDiffY = (pixHeightMapHeight-1)/(FLOAT)pixShadowMapHeight;
	
	const PIX pixTop = rcClear.rc_slTop / fHeightShadowDiffY;
	const PIX pixLeft = rcClear.rc_slLeft / fHeightShadowDiffX;
	const PIX pixRight = rcClear.rc_slRight / fHeightShadowDiffX;
	const PIX pixBottom = rcClear.rc_slBottom / fHeightShadowDiffY;

	const PIX pixFirstX = pixLeft;
	const PIX pixFirstY = pixTop;
	const PIX pixWidth  = pixRight - pixLeft;
	const PIX pixHeight = pixBottom - pixTop;

	ULONG *pulShadowMap = &ptr->tr_ptdShadowMap->td_pulFrames[0];

	// for each pixel of clear rect
	for(PIX pixY=0;pixY<pixHeight;pixY++) {
		for(PIX pixX=0;pixX<pixWidth;pixX++) {
			const PIX pixRealX = pixFirstX+pixX;
			const PIX pixRealY = pixFirstY+pixY;
			PIX pix = pixRealX + pixRealY*pixShadowMapWidth;
			// clear pixel
			ASSERT(pix<pixShadowMapWidth*pixShadowMapHeight);
			pulShadowMap[pix] = 0xFF000000;
			// pulShadowMap[pix] = 0xFF00FF00;
		}
	}
}

static void CalculatePointLight(CTerrainImp *ptr, const TerrainPointLight &tpl, const CTRect rcUpdate)
{
	const PIX	pixHeightMapWidth = ptr->tr_pixHeightMapWidth;
	const PIX	pixHeightMapHeight = ptr->tr_pixHeightMapHeight;
	const PIX	pixShadowMapWidth = GetShadowMapWidth( pixHeightMapWidth, ptr->tr_slShadowMapSizeAspect );
	const PIX	pixShadowMapHeight = GetShadowMapHeight( pixHeightMapHeight, ptr->tr_slShadowMapSizeAspect );
	const FLOAT	fHeightShadowDiffX = ( pixHeightMapWidth - 1 ) / (FLOAT)pixShadowMapWidth;
	const FLOAT	fHeightShadowDiffY = ( pixHeightMapHeight - 1 ) / (FLOAT)pixShadowMapHeight;

	const PIX	pixTop = rcUpdate.rc_slTop / fHeightShadowDiffY;
	const PIX	pixLeft = rcUpdate.rc_slLeft / fHeightShadowDiffX;
	const PIX	pixRight = rcUpdate.rc_slRight / fHeightShadowDiffX;
	const PIX	pixBottom = rcUpdate.rc_slBottom / fHeightShadowDiffY;

	const PIX	pixFirstX = pixLeft;
	const PIX	pixFirstY = pixTop;
	const PIX	pixWidth = pixRight - pixLeft;
	const PIX	pixHeight = pixBottom - pixTop;

	const FLOAT3D	&vStretch = ptr->tr_vStretch;
	FLOAT3D	vLightPos = tpl.tpl_vLightPos;
	vLightPos(1) *= vStretch(1);
	vLightPos(2) *= vStretch(2);
	vLightPos(3) *= vStretch(3);

	const FLOAT fFallOff = tpl.tpl_fFallOff;
	const FLOAT fHotSpot = tpl.tpl_fHotSpot;
	//COLOR	colLight = tpl.tpl_colLight;
	GFXColor	colLight = ByteSwap( tpl.tpl_colLight );		// yjpark |<--
	ULONG	ulOverbrightStrength = 255 * ptr->tr_fShadowOverbright;
	colLight.AttenuateRGB( ulOverbrightStrength );				// yjpark     -->|

	ULONG *pulShadowMap = &ptr->tr_ptdShadowMap->td_pulFrames[0];

	for(PIX pixY = 0; pixY < pixHeight; pixY++ )
	{
		for(PIX pixX = 0; pixX < pixWidth; pixX++ )
		{
			const PIX	pixRealX = pixFirstX+pixX;
			const PIX	pixRealY = pixFirstY+pixY;
			const PIX	pix = pixRealX + pixRealY * pixShadowMapWidth;
			ASSERT( pix < pixShadowMapWidth * pixShadowMapHeight );

			FLOAT3D	vxPos;
			FLOAT3D	nxNormal;
			const FLOAT	fPosX = pixRealX * fHeightShadowDiffY;
			const FLOAT	fPosY = pixRealY * fHeightShadowDiffY;
			CalculateNormalFromPoint( ptr, fPosX, fPosY, vxPos, nxNormal );
			FLOAT3D	vDistance = vxPos - vLightPos;
			const FLOAT		fDistance = vDistance.Length();
			const FLOAT3D	vLightNormal = -vDistance.Normalize();

			FLOAT	fIntensity = 1.0f;
			if( fDistance > fFallOff )
				fIntensity = 0;
			else if( fDistance > fHotSpot )
				fIntensity = CalculateRatio( fDistance, fHotSpot, fFallOff, 0.0f, 1.0f );

			ULONG	ulIntensity = NormFloatToByte( fIntensity );
			//ulIntensity = ( ulIntensity << CT_RSHIFT ) | ( ulIntensity << CT_GSHIFT ) | ( ulIntensity << CT_BSHIFT );
			//GFXColor	col = MulColors( ByteSwap( colLight ), ulIntensity );
			GFXColor	col = colLight;			// yjpark
			col.AttenuateRGB( ulIntensity );	// yjpark
			const FLOAT	fDot = Clamp( nxNormal % vLightNormal, 0.0f, 1.0f );
			const SLONG	slDot = NormFloatToByte( fDot );

#if 1
			GFXColor	*pcol = (GFXColor *)&pulShadowMap[pix];
			pcol->r = ClampUp( pcol->r + ( ( col.r * slDot ) >> 8 ), 255L );
			pcol->g = ClampUp( pcol->g + ( ( col.g * slDot ) >> 8 ), 255L );
			pcol->b = ClampUp( pcol->b + ( ( col.b * slDot ) >> 8 ), 255L );
			pcol->a = 255;
#else
			UBYTE	ubr = NormFloatToByte( nxNormal(1) );
			UBYTE	ubg = NormFloatToByte( nxNormal(2) );
			UBYTE	ubb = NormFloatToByte( nxNormal(3) );
			pulShadowMap[pix] = 0xFF000000 | ( ubb << 16 ) | ( ubg << 8 ) | ( ubr << 0 );
#endif
		}
	}
}

static void CalculateDirectionalLight(CTerrainImp *ptr, const TerrainDirectionalLight &tdl, const CTRect rcUpdate)
{
	const PIX	pixHeightMapWidth = ptr->tr_pixHeightMapWidth;
	const PIX	pixHeightMapHeight = ptr->tr_pixHeightMapHeight;
	const PIX	pixShadowMapWidth = GetShadowMapWidth( pixHeightMapWidth, ptr->tr_slShadowMapSizeAspect );
	const PIX	pixShadowMapHeight = GetShadowMapHeight( pixHeightMapHeight, ptr->tr_slShadowMapSizeAspect );
	const FLOAT	fHeightShadowDiffX = ( pixHeightMapWidth - 1 ) / ( FLOAT )pixShadowMapWidth;
	const FLOAT	fHeightShadowDiffY = ( pixHeightMapHeight - 1 ) / ( FLOAT )pixShadowMapHeight;

	const PIX	pixTop = rcUpdate.rc_slTop / fHeightShadowDiffY;
	const PIX	pixLeft = rcUpdate.rc_slLeft / fHeightShadowDiffX;
	const PIX	pixRight = rcUpdate.rc_slRight / fHeightShadowDiffX;
	const PIX	pixBottom = rcUpdate.rc_slBottom / fHeightShadowDiffY;

	const PIX	pixFirstX = pixLeft;
	const PIX	pixFirstY = pixTop;
	const PIX	pixWidth  = pixRight - pixLeft;
	const PIX	pixHeight = pixBottom - pixTop;

	GFXColor	colLight = tdl.tdl_colLight;
	GFXColor	colAmbient = tdl.tdl_colAmbient;
	const FLOAT3D	nxLight = tdl.tdl_vLightNormal;
	
	ULONG	ulOverbrightStrength = 255 * ptr->tr_fShadowOverbright;				// yjpark |<--
	colLight.AttenuateRGB( ulOverbrightStrength );			
	colAmbient.AttenuateRGB( ulOverbrightStrength );							// yjpark     -->|

	const SLONG	slar = ClampUp( (SLONG)colAmbient.r, 127L );
	const SLONG	slag = ClampUp( (SLONG)colAmbient.g, 127L );
	const SLONG	slab = ClampUp( (SLONG)colAmbient.b, 127L );

	ULONG	*pulShadowMap = &ptr->tr_ptdShadowMap->td_pulFrames[0];

	for( PIX pixY = 0; pixY < pixHeight; pixY++ )
	{
		for( PIX pixX = 0; pixX < pixWidth; pixX++ )
		{
			const PIX	pixRealX = pixFirstX + pixX;
			const PIX	pixRealY = pixFirstY + pixY;
			const PIX	pix = pixRealX + pixRealY * pixShadowMapWidth;
			ASSERT( pix < pixShadowMapWidth * pixShadowMapHeight );

			FLOAT3D	vxPos;
			FLOAT3D	nxNormal;
			const FLOAT	fPosX = pixRealX * fHeightShadowDiffY;
			const FLOAT	fPosY = pixRealY * fHeightShadowDiffY;
			CalculateNormalFromPoint( ptr, fPosX, fPosY, vxPos, nxNormal );
			const FLOAT	fDot = Clamp( nxNormal % nxLight, 0.0f, 1.0f) ;
			const SLONG	slDot = NormFloatToByte( fDot );

			GFXColor	*pcol = (GFXColor *)&pulShadowMap[pix];
			pcol->r = ClampUp( pcol->r + slar + ( ( colLight.r * slDot ) >> 8 ), 255L );
			pcol->g = ClampUp( pcol->g + slag + ( ( colLight.g * slDot ) >> 8 ), 255L );
			pcol->b = ClampUp( pcol->b + slab + ( ( colLight.b * slDot ) >> 8 ), 255L );
			pcol->a = 255;
		}
	}
}

// yjpark |<--
static void CalculateObjectShadowOfDirectionalLight( CTerrain *ptrTerrain, const TerrainDirectionalLight &tdl,
														const CTRect rcUpdate )
{
	CTerrainImp	*ptr = ptrTerrain->tr_ptrTerrain;

	const PIX	pixHeightMapWidth = ptr->tr_pixHeightMapWidth;
	const PIX	pixHeightMapHeight = ptr->tr_pixHeightMapHeight;
	const PIX	pixShadowMapWidth = GetShadowMapWidth( pixHeightMapWidth, ptr->tr_slShadowMapSizeAspect );
	const PIX	pixShadowMapHeight = GetShadowMapHeight( pixHeightMapHeight, ptr->tr_slShadowMapSizeAspect );
	const FLOAT	fHeightShadowDiffX = ( pixHeightMapWidth - 1 ) / ( FLOAT )pixShadowMapWidth;
	const FLOAT	fHeightShadowDiffY = ( pixHeightMapHeight - 1 ) / ( FLOAT )pixShadowMapHeight;

	const PIX	pixTop = rcUpdate.rc_slTop / fHeightShadowDiffY;
	const PIX	pixLeft = rcUpdate.rc_slLeft / fHeightShadowDiffX;
	const PIX	pixRight = rcUpdate.rc_slRight / fHeightShadowDiffX;
	const PIX	pixBottom = rcUpdate.rc_slBottom / fHeightShadowDiffY;

	const PIX	pixFirstX = pixLeft;
	const PIX	pixFirstY = pixTop;
	const PIX	pixWidth  = pixRight - pixLeft;
	const PIX	pixHeight = pixBottom - pixTop;

	GFXColor	colObjShadow = ptr->tr_acolObjShadowColor[_iCurShadowMap];
	ULONG		ulOverbrightStrength = 255 * ptr->tr_fShadowOverbright;
	colObjShadow.AttenuateRGB( ulOverbrightStrength );

	ULONG	*pulShadowMap = &ptr->tr_ptdShadowMap->td_pulFrames[0];

	FLOAT	fX, fY, fZ;
	FLOAT	fXMul = ptr->tr_vMetricSize( 1 ) / (FLOAT)pixShadowMapWidth;
	FLOAT	fYMul = ptr->tr_vMetricSize( 3 ) / (FLOAT)pixShadowMapHeight;
	FLOAT	fXOffset = fXMul * 0.5f;
	FLOAT	fYOffset = fYMul * 0.5f;

	for( PIX pixY = 0; pixY < pixHeight; pixY++ )
	{
		const PIX	pixRealY = pixFirstY + pixY;
		fZ = pixRealY * fYMul + fYOffset;

		for( PIX pixX = 0; pixX < pixWidth; pixX++ )
		{
			const PIX	pixRealX = pixFirstX + pixX;
			const PIX	pix = pixRealX + pixRealY * pixShadowMapWidth;
			ASSERT( pix < pixShadowMapWidth * pixShadowMapHeight );

			fX = pixRealX * fXMul + fXOffset;
			fY = TR_GetHeight( ptr, fX, fZ );

			FLOAT3D	vOrigin = FLOAT3D( fX, fY, fZ );
			FLOAT3D	vDirection;
			ANGLE3D	aOrientationAngle = tdl.tdl_plPlacement.pl_OrientationAngle;
			if( aOrientationAngle(2) > 90.0f && aOrientationAngle(2) < 270.0f )
			{
				if( aOrientationAngle(2) < 180.0f )
					aOrientationAngle(2) = 180.0f;

				FLOAT	fRatio = ( 270.0f - aOrientationAngle(2) ) / 90.0f;
				aOrientationAngle(2) += 30.0f * fRatio;
			}
			else
			{
				if( aOrientationAngle(2) > 360.0f || aOrientationAngle(2) < 270.0f)
					aOrientationAngle(2) = 360.0f;

				FLOAT	fRatio = ( aOrientationAngle(2) - 270.0f ) / 90.0f;
				aOrientationAngle(2) -= 30.0f * fRatio;
			}

			AnglesToDirectionVector( aOrientationAngle, vDirection );
			vDirection *= -100000.0f;
			FLOAT3D		vTarget = vOrigin + vDirection;

			CCastRay	crObjShadow( NULL, vOrigin, vTarget );
			if( crObjShadow.CastForTerrainShadow( ptrTerrain->tr_penEntity->en_pwoWorld, _bSkaShadow,
													_bMdlShadow, _bBrushShadow, _bTerrainShadow ) )
			{
				GFXColor	*pcolShadow = (GFXColor *)&pulShadowMap[pix];
				if( pcolShadow->r > colObjShadow.r )
					pcolShadow->r = colObjShadow.r;
				if( pcolShadow->g > colObjShadow.g )
					pcolShadow->g = colObjShadow.g;
				if( pcolShadow->b > colObjShadow.b )
					pcolShadow->b = colObjShadow.b;
			}
		}
	}
}

static void CalculateObjectShadowOfPointLight( CTerrain *ptrTerrain, const TerrainPointLight &tpl, const CTRect rcUpdate )
{
	CTerrainImp	*ptr = ptrTerrain->tr_ptrTerrain;

	const PIX	pixHeightMapWidth = ptr->tr_pixHeightMapWidth;
	const PIX	pixHeightMapHeight = ptr->tr_pixHeightMapHeight;
	const PIX	pixShadowMapWidth = GetShadowMapWidth( pixHeightMapWidth, ptr->tr_slShadowMapSizeAspect );
	const PIX	pixShadowMapHeight = GetShadowMapHeight( pixHeightMapHeight, ptr->tr_slShadowMapSizeAspect );
	const FLOAT	fHeightShadowDiffX = ( pixHeightMapWidth - 1 ) / (FLOAT)pixShadowMapWidth;
	const FLOAT	fHeightShadowDiffY = ( pixHeightMapHeight - 1 ) / (FLOAT)pixShadowMapHeight;

	const PIX	pixTop = rcUpdate.rc_slTop / fHeightShadowDiffY;
	const PIX	pixLeft = rcUpdate.rc_slLeft / fHeightShadowDiffX;
	const PIX	pixRight = rcUpdate.rc_slRight / fHeightShadowDiffX;
	const PIX	pixBottom = rcUpdate.rc_slBottom / fHeightShadowDiffY;

	const PIX	pixFirstX = pixLeft;
	const PIX	pixFirstY = pixTop;
	const PIX	pixWidth = pixRight - pixLeft;
	const PIX	pixHeight = pixBottom - pixTop;

	const FLOAT3D	&vStretch = ptr->tr_vStretch;
	FLOAT3D	vLightPos = tpl.tpl_vLightPos;
	vLightPos(1) *= vStretch(1);
	vLightPos(2) *= vStretch(2);
	vLightPos(3) *= vStretch(3);

	const FLOAT	fFallOff = tpl.tpl_fFallOff;
	const FLOAT fHotSpot = tpl.tpl_fHotSpot;

	GFXColor	colObjShadow = ptr->tr_acolObjShadowColor[_iCurShadowMap];
	ULONG		ulOverbrightStrength = 255 * ptr->tr_fShadowOverbright;
	colObjShadow.AttenuateRGB( ulOverbrightStrength );
	
	ULONG	*pulShadowMap = &ptr->tr_ptdShadowMap->td_pulFrames[0];

	FLOAT	fX, fY, fZ;
	FLOAT	fXMul = ptr->tr_vMetricSize( 1 ) / (FLOAT)pixShadowMapWidth;
	FLOAT	fYMul = ptr->tr_vMetricSize( 3 ) / (FLOAT)pixShadowMapHeight;
	FLOAT	fXOffset = fXMul * 0.5f;
	FLOAT	fYOffset = fYMul * 0.5f;

	for(PIX pixY = 0; pixY < pixHeight; pixY++ )
	{
		const PIX	pixRealY = pixFirstY + pixY;
		fZ = pixRealY * fYMul + fYOffset;

		for(PIX pixX = 0; pixX < pixWidth; pixX++ )
		{
			const PIX	pixRealX = pixFirstX + pixX;
			const PIX	pix = pixRealX + pixRealY * pixShadowMapWidth;
			ASSERT( pix < pixShadowMapWidth * pixShadowMapHeight );

			fX = pixRealX * fXMul + fXOffset;
			fY = TR_GetHeight( ptr, fX, fZ );

			FLOAT3D	vOrigin = FLOAT3D( fX, fY, fZ );
			FLOAT3D	vDirection = vLightPos - vOrigin;
			FLOAT3D	vTarget = vOrigin + vDirection;

			const FLOAT		fDistance = vDirection.Length();
			if( fDistance > fFallOff )
				continue;

			CCastRay	crObjShadow( NULL, vOrigin, vTarget );
			if( crObjShadow.CastForTerrainShadow( ptrTerrain->tr_penEntity->en_pwoWorld, _bSkaShadow,
													_bMdlShadow, _bBrushShadow, _bTerrainShadow ) )
			{
				FLOAT	fIntensity = 1.0f;
				if( fDistance > fHotSpot )
					fIntensity = CalculateRatio( fDistance, fHotSpot, fFallOff, 0.0f, 1.0f );

				GFXColor	*pcolShadow = (GFXColor *)&pulShadowMap[pix];
				if( pcolShadow->r > colObjShadow.r )
					pcolShadow->r = pcolShadow->r + ( colObjShadow.r - pcolShadow->r ) * fIntensity;
				if( pcolShadow->g > colObjShadow.g )
					pcolShadow->g = pcolShadow->g + ( colObjShadow.g - pcolShadow->g ) * fIntensity;
				if( pcolShadow->b > colObjShadow.b )
					pcolShadow->b = pcolShadow->b + ( colObjShadow.b - pcolShadow->b ) * fIntensity;
			}
		}
	}
}
// yjpark     -->|

static void UpdateShadowMap( CTerrain *ptrTerrain, BOOL bUpdateObjectShadow )
{
	CTerrainImp	*ptr = ptrTerrain->tr_ptrTerrain;
	ASSERT(ptr->tr_ptdShadowMap!=NULL);
	ASSERT(ptr->tr_ptdShadowMap->td_pulFrames!=NULL);

#if TR_DEBUG_NORMALS
	_anxNormals.PopAll();
	_avxVertices.PopAll();
#endif

	// Calculate update box for each added light
	CalculateLightUpdateBoxes(ptr);
	
	// Update all directional lights
	CTRect rcUpdate = _rcUpdateRect;
	const INDEX cttdl = _atdlLights.Count();
	for(INDEX itdl=0;itdl<cttdl;itdl++) {
		TerrainDirectionalLight &tdl = _atdlLights[itdl];
		if(tdl.tdl_ulFlags&TLF_TOUCHES_TERRAIN) {
			_bShadowMapChanged = TRUE;
			CalculateDirectionalLight(ptr, tdl, rcUpdate);
		}
	}

	// Update object shadow affected by directional light
	if( bUpdateObjectShadow )		// yjpark |<--
	{
		const INDEX	cttdl = _atdlLights.Count();
		for( INDEX itdl = 0; itdl < cttdl; itdl++ )
		{
			TerrainDirectionalLight	&tdl = _atdlLights[itdl];
			if( tdl.tdl_ulFlags & TLF_TOUCHES_TERRAIN )
			{
				_bShadowMapChanged = TRUE;
				CalculateObjectShadowOfDirectionalLight( ptrTerrain, tdl, rcUpdate );
			}
		}
	}								// yjpark     -->|

	// Update all point lights
	const INDEX cttpl = _atplLights.Count();
	for(INDEX itpl=0;itpl<cttpl;itpl++) {
		TerrainPointLight &tpl = _atplLights[itpl];
		if(tpl.tpl_ulFlags&TLF_TOUCHES_TERRAIN) {
			_bShadowMapChanged = TRUE;
			CalculatePointLight(ptr, tpl, tpl.tpl_rcLight);
		}
	}

	// Update object shadow affected by point light
	if( bUpdateObjectShadow )		// yjpark |<--
	{
		const INDEX	cttpl = _atplLights.Count();
		for( INDEX itpl = 0; itpl < cttpl; itpl++ )
		{
			TerrainPointLight	&tpl = _atplLights[itpl];
			if( tpl.tpl_ulFlags & TLF_TOUCHES_TERRAIN )
			{
				_bShadowMapChanged = TRUE;
				CalculateObjectShadowOfPointLight( ptrTerrain, tpl, tpl.tpl_rcLight );
			}
		}
	}								// yjpark     -->|
}

// Add point light to shadow map calculation
extern void TRS_AddPointLight(FLOAT fFallOff, FLOAT fHotSpot, COLOR colLight, CPlacement3D plPlacement)
{
	TerrainPointLight &tpl = _atplLights.Push();
	tpl.tpl_fFallOff = fFallOff;
	tpl.tpl_fHotSpot = fHotSpot;
	tpl.tpl_colLight = ByteSwap(colLight);
	tpl.tpl_ulFlags  = 0;
	tpl.tpl_plPlacement = plPlacement;
}

// Add directional light to shadow map calculation
extern void TRS_AddDirectionalLight(COLOR colLight, COLOR colAmbient, CPlacement3D plPlacement)
{
	TerrainDirectionalLight &tdl = _atdlLights.Push();
	tdl.tdl_colLight = colLight;
	tdl.tdl_colAmbient = colAmbient;
	tdl.tdl_plPlacement = plPlacement;
}

// Add all lights in world
extern void TRS_ColectLightsFromWorld(CTerrain *ptrTerrain)
{
	ASSERT(ptrTerrain!=NULL);                // must be valid
	ASSERT(ptrTerrain->tr_ptrTerrain!=NULL); // must be valid
	ASSERT(ptrTerrain->tr_penEntity!=NULL);
	ASSERT(ptrTerrain->tr_penEntity->en_pwoWorld!=NULL);

	CEntity *pen = ptrTerrain->tr_penEntity;
	CWorld *pwld = pen->en_pwoWorld;
	CTerrainImp *ptr = ptrTerrain->tr_ptrTerrain;

	// for each entity in the world
	FOREACHINDYNAMICCONTAINER(pwld->wo_cenEntities, CEntity, iten) {
		// if it is light entity 
		const CLightSource *pls = iten->GetLightSource();
		if(pls!=NULL) {
			// if this isn't static light
			if(pls->ls_ulFlags&LSF_DYNAMIC || pls->ls_ulFlags&LSF_LENSFLAREONLY) {
				// skip this light
				continue;
			}
			// if light is directional
			if(pls->ls_ulFlags&LSF_DIRECTIONAL) {
				// Add one directional light
				const COLOR colLight = pls->GetLightColor();
				const COLOR colAmbient = pls->GetLightAmbient();
				const CPlacement3D &plLight = iten->en_plPlacement;
				TRS_AddDirectionalLight(colLight, colAmbient, plLight);
			} else {
				const FLOAT fFallOff = pls->ls_rFallOff;
				const FLOAT fHotSpot = pls->ls_rHotSpot;
				const COLOR colLight = pls->GetLightColor();
				const CPlacement3D &plLight = iten->en_plPlacement;
				TRS_AddPointLight(fFallOff,fHotSpot,colLight,plLight);
			}
		}
	}
}

// Calculate region of shadow map with colected lights
extern void TRS_UpdateShadowMap( CTerrain *ptrTerrain, FLOATaabbox3D bboxUpdate, BOOL bUpdateObjectShadow )		// yjpark
{
	ASSERT(ptrTerrain!=NULL);                // must be valid
	ASSERT(ptrTerrain->tr_ptrTerrain!=NULL); // must be valid
	ASSERTMSG(_bPlacementSet,"Terrain placement not set. Use TRS_SetTerrainPlacement when updation shadow map");
	
	CTerrainImp *ptr = ptrTerrain->tr_ptrTerrain;
	const FLOAT3D vStretch = ptr->tr_vStretch;
	const FLOAT3D vInvStretch(1.0f/vStretch(1), 1.0f/vStretch(2), 1.0f/vStretch(3));
	Matrix12 mStretch;
	Matrix12 mInvStretch;
	MakeStretchMatrix(mStretch, vStretch);
	MakeStretchMatrix(mInvStretch, vInvStretch);
	MatrixMultiply(_mObjToAbsStretch,_mObjToAbs,mStretch);
	MatrixTranspose(_mInvObjToAbs,_mObjToAbs);
	MatrixMultiply(_mInvObjToAbsStretch,mInvStretch,_mInvObjToAbs);

	_bboxTerrain = TR_GetTerrainBBox(ptrTerrain);

	// if update box wasn'tspecified
	if(bboxUpdate.IsEmpty()) {
		// update all terrain
		CTRect rcTerrain = CTRect(CTPoint(0,0),CTPoint(ptr->tr_pixHeightMapWidth-1,ptr->tr_pixHeightMapHeight-1));
		_rcUpdateRect = rcTerrain;
	} else {
		_rcUpdateRect = RelRectFromAbsBBox(ptr, bboxUpdate,_mInvObjToAbs);
	}

#if TR_DEBUG_SHADOWMAP
	_arcUpdateShadowMap.Push() = _rcUpdateRect;
	_abbUpdateShadowMap.Push() = bboxUpdate;
#endif 

	// Clear part of shadow map that will be updated
	ClearPartOfShadowMap(ptr, _rcUpdateRect);
	// Update shadowmap
#if 1
	UpdateShadowMap( ptrTerrain, bUpdateObjectShadow );			// yjpark
#else
	_bShadowMapChanged = TRUE;
#endif
}

// setup terrain position (must be set for shadow map update)
extern void TRS_SetTerrainPlacement(const CPlacement3D &pl)
{
	FLOATmatrix3D m;
	Matrix12 mObjToAbs;
	MakeRotationMatrixFast( m, pl.pl_OrientationAngle);
	MatrixVectorToMatrix12(mObjToAbs, m, pl.pl_PositionVector);
	TRS_SetTerrainPlacement(mObjToAbs);
}
extern void TRS_SetTerrainPlacement(const Matrix12 &mObjToAbs)
{
	MatrixCopy(_mObjToAbs,mObjToAbs);
	_bPlacementSet = TRUE;
}

extern void TRS_BeginShadowMapUpdate(CTerrain *ptrTerrain)
{
#if TR_DEBUG_SHADOWMAP
	_abbLightRegion.PopAll();
	_arcLightRegion.PopAll();
	_arcUpdateShadowMap.PopAll();
	_abbUpdateShadowMap.PopAll();
#endif 
	ASSERT(ptrTerrain!=NULL);                // must be valid
	ASSERT(ptrTerrain->tr_ptrTerrain!=NULL); // must be valid
	CTerrainImp *ptr = ptrTerrain->tr_ptrTerrain;

	ASSERTMSG(_atplLights.Count()==0,"please use TRS_BeginShadowMapUpdate and TRS_EndShadowMapUpdate");
	ASSERTMSG(_atdlLights.Count()==0,"please use TRS_BeginShadowMapUpdate and TRS_EndShadowMapUpdate");
	ASSERTMSG(!_bPlacementSet,"please use TRS_BeginShadowMapUpdate and TRS_EndShadowMapUpdate");
}
extern void TRS_EndShadowMapUpdate(CTerrain *ptrTerrain)
{
	ASSERT(ptrTerrain!=NULL);                // must be valid
	ASSERT(ptrTerrain->tr_ptrTerrain!=NULL); // must be valid
	CTerrainImp *ptr = ptrTerrain->tr_ptrTerrain;

	if(_bShadowMapChanged) {
		// Upload shadow map texture
		const PIX pixShadowMapWidth  = GetShadowMapWidth(ptr->tr_pixHeightMapWidth,ptr->tr_slShadowMapSizeAspect);
		const PIX pixShadowMapHeight = GetShadowMapHeight(ptr->tr_pixHeightMapHeight,ptr->tr_slShadowMapSizeAspect);
		TR_UploadTexture(*ptr->tr_ptdShadowMap,pixShadowMapWidth,pixShadowMapHeight,PTF_STATIC);
	}

	_bShadowMapChanged = FALSE;

	_atplLights.PopAll();
	_atdlLights.PopAll();
	_bPlacementSet = FALSE;

}

// yjpark |<--
static void TRS_ApplyBlur( FLOAT fBlurRadius, ULONG *pulSrc, ULONG *pulDst, PIX pixWidth, PIX pixHeight )
{
	// fBlurRadius : 0.0 ~ 10.0, deviation = 0.1
	DOUBLE	adGaussianFilter[21][21];
	DOUBLE	dDeviationSq = fBlurRadius / 2.0f;
	dDeviationSq *= dDeviationSq;
	INDEX	iKernelSize = (INDEX)( fBlurRadius + 0.9f ) * 2 + 1;
	INDEX	iOffset = iKernelSize / 2;
	DOUBLE	dTwoPi = atan( 1.0 ) * 8.0;
	PIX		i, j, x, y, x2, y2;

	for( j = 0; j < iKernelSize; j++ )
	{
		y = j - iOffset;
		for( i = 0; i < iKernelSize; i++ )
		{
			x = i - iOffset;
			adGaussianFilter[j][i] = ( 1.0 / ( dTwoPi * dDeviationSq ) ) *
										exp( -( y*y + x*x ) / ( 2.0 * dDeviationSq ) );
		}
	}

	for( y = 0; y < pixHeight; y++ )
	{
		for( x = 0; x < pixWidth; x++ )
		{
			PIX		pixDst = x + y * pixWidth;
			DOUBLE	dDivSum, dSumR, dSumG, dSumB;
			dDivSum = dSumR = dSumG = dSumB = 0.0;
		
			for( j = 0; j < iKernelSize; j++ )
			{
				y2 = Clamp( y + j - iOffset, (PIX)0, pixHeight );
				for( i = 0; i < iKernelSize; i++ )
				{
					DOUBLE	dWeight = adGaussianFilter[j][i];
					dDivSum += dWeight;
					x2 = Clamp( x + i - iOffset, (PIX)0, pixWidth );
					PIX		pixSrc = x2 + y2 * pixWidth;
				
					ULONG	ulColor = pulSrc[pixSrc];
					DOUBLE	dInfluenceR = ulColor & 0x000000FF;
					DOUBLE	dInfluenceG = ( ulColor & 0x0000FF00 ) >> 8;
					DOUBLE	dInfluenceB = ( ulColor & 0x00FF0000 ) >> 16;
				
					dSumR += dInfluenceR * dWeight;
					dSumG += dInfluenceG * dWeight;
					dSumB += dInfluenceB * dWeight;
				}
			}

			SINT	siR = Clamp( SINT( dSumR / dDivSum ), 0, 255 );
			SINT	siG = Clamp( SINT( dSumG / dDivSum ), 0, 255 );
			SINT	siB = Clamp( SINT( dSumB / dDivSum ), 0, 255 );
			pulDst[pixDst] = siR | ( siG << 8 ) | ( siB << 16 ) | 0xFF000000;
		}
	}
}

extern void TRS_UpdateRealShadowMap( CTerrain* ptrTerrain, INDEX iShadowMap,
										GFXColor colObjShadow, FLOAT fShadowBlur,
										BOOL bHitSka, BOOL bHitMdl, BOOL bHitBrush, BOOL bHitTerrain )
{
	ASSERT( ptrTerrain != NULL );
	ASSERT( ptrTerrain->tr_ptrTerrain != NULL );
	CTerrainImp*	ptr = ptrTerrain->tr_ptrTerrain;

	ptr->tr_acolObjShadowColor[iShadowMap] = colObjShadow;
	ptr->tr_afShadowBlurRadius[iShadowMap] = fShadowBlur;
	_iCurShadowMap = iShadowMap;
	_bSkaShadow = bHitSka;
	_bMdlShadow = bHitMdl;
	_bBrushShadow = bHitBrush;
	_bTerrainShadow = bHitTerrain;

	BOOL	bObjectShadow = FALSE;
	if( bHitSka || bHitMdl || bHitBrush || bHitTerrain )
		bObjectShadow = TRUE;

	Matrix12 mTerrain;
	TR_GetMatrixFromEntity( mTerrain, ptrTerrain);
	TR_UpdateShadowMap( ptrTerrain, mTerrain, FLOATaabbox3D(), TRUE, bObjectShadow );

	const SLONG	slShadowMapAspect = ptr->tr_slShadowMapSizeAspect;
	const PIX	pixHeightMapWidth = ptr->tr_pixHeightMapWidth;
	const PIX	pixHeightMapHeight = ptr->tr_pixHeightMapHeight;
	const PIX	pixShadowMapWidth = GetShadowMapWidth( pixHeightMapWidth, slShadowMapAspect );
	const PIX	pixShadowMapHeight = GetShadowMapHeight( pixHeightMapHeight, slShadowMapAspect );
	const PIX	pixShadowMapSize = pixShadowMapWidth * pixShadowMapHeight * sizeof(ULONG);

	CTextureData	*ptdShadowMap = ptr->tr_aptdShadowMaps[iShadowMap];
	ULONG			*pulShadowMap = ptdShadowMap->td_pulFrames;
	CTextureData	*ptdTempShadowMap = ptr->tr_ptdShadowMap;
	ULONG			*pulTempShadowMap = ptdTempShadowMap->td_pulFrames;

	if( fShadowBlur > 0.0f )
		TRS_ApplyBlur( fShadowBlur, pulTempShadowMap, pulShadowMap, pixShadowMapWidth, pixShadowMapHeight );
	else
		memcpy( pulShadowMap, pulTempShadowMap, pixShadowMapSize );

	TR_UploadTexture( *ptdShadowMap, pixShadowMapWidth, pixShadowMapHeight, PTF_STATIC );
}
// yjpark     -->|


// yjpark |<--
//-------------------------------------------------------------------------------------------------
// Desc : These contents aren't included in final release version
// ------------------------------------------------------------------------------------------------
#ifndef FINALVERSION

// yjpark |<--
extern void TRS_DeleteShadowMap( CTerrain* ptrTerrain, INDEX iShadowMap )
{
	ASSERT( ptrTerrain != NULL );
	ASSERT( ptrTerrain->tr_ptrTerrain != NULL );

	CTerrainImp*	ptr = ptrTerrain->tr_ptrTerrain;
	const INDEX	ctShadowMaps = ptr->tr_aptdShadowMaps.Count();

	CStaticArray<CTextureData*>	atsOldShadowMaps;
	atsOldShadowMaps.sa_Array = ptr->tr_aptdShadowMaps.sa_Array;
	atsOldShadowMaps.sa_Count = ptr->tr_aptdShadowMaps.sa_Count;
	ptr->tr_aptdShadowMaps.sa_Array = NULL;
	ptr->tr_aptdShadowMaps.sa_Count = 0;
	ptr->tr_aptdShadowMaps.New( ctShadowMaps - 1 );

	CStaticArray<ULONG>			aulOldShadowTimes;
	aulOldShadowTimes.sa_Array = ptr->tr_aulShadowTimes.sa_Array;
	aulOldShadowTimes.sa_Count = ptr->tr_aulShadowTimes.sa_Count;
	ptr->tr_aulShadowTimes.sa_Array = NULL;
	ptr->tr_aulShadowTimes.sa_Count = 0;
	ptr->tr_aulShadowTimes.New( ctShadowMaps  );

	CStaticArray<FLOAT>			afOldShadowBlurRadius;
	afOldShadowBlurRadius.sa_Array = ptr->tr_afShadowBlurRadius.sa_Array;
	afOldShadowBlurRadius.sa_Count = ptr->tr_afShadowBlurRadius.sa_Count;
	ptr->tr_afShadowBlurRadius.sa_Array = NULL;
	ptr->tr_afShadowBlurRadius.sa_Count = 0;
	ptr->tr_afShadowBlurRadius.New( ctShadowMaps - 1 );

	CStaticArray<GFXColor>		acolOldObjShadowColor;
	acolOldObjShadowColor.sa_Array = ptr->tr_acolObjShadowColor.sa_Array;
	acolOldObjShadowColor.sa_Count = ptr->tr_acolObjShadowColor.sa_Count;
	ptr->tr_acolObjShadowColor.sa_Array = NULL;
	ptr->tr_acolObjShadowColor.sa_Count = 0;
	ptr->tr_acolObjShadowColor.New( ctShadowMaps - 1 );

	const SLONG	slShadowMapAspect = ptr->tr_slShadowMapSizeAspect;
		const PIX	pixHeightMapWidth = ptr->tr_pixHeightMapWidth;
		const PIX	pixHeightMapHeight = ptr->tr_pixHeightMapHeight;
		const PIX	pixShadowMapWidth = GetShadowMapWidth( pixHeightMapWidth, slShadowMapAspect );
		const PIX	pixShadowMapHeight = GetShadowMapHeight( pixHeightMapHeight, slShadowMapAspect );
		const PIX	pixShadowMapSize = pixShadowMapWidth * pixShadowMapHeight * sizeof(ULONG);

	INDEX	iNewShadowMapsIndex = 0;
	INDEX	itrs;
	for( itrs = 0; itrs < ctShadowMaps; itrs++ )
	{
		if( itrs != iShadowMap )
		{
			ptr->tr_aptdShadowMaps[iNewShadowMapsIndex] = TR_CreateTexture();
			CTextureData*	ptdSrc = atsOldShadowMaps[itrs];
			CTextureData*	ptdDst = ptr->tr_aptdShadowMaps[iNewShadowMapsIndex];
			TR_PrepareTexture( *ptdDst, pixShadowMapWidth, pixShadowMapHeight, PTF_STATIC );
			memcpy( ptdDst->td_pulFrames, ptdSrc->td_pulFrames, pixShadowMapSize );
			TR_UploadTexture( *ptdDst, pixShadowMapWidth, pixShadowMapHeight, PTF_STATIC );

			ptr->tr_aulShadowTimes[iNewShadowMapsIndex] = aulOldShadowTimes[itrs];
			ptr->tr_afShadowBlurRadius[iNewShadowMapsIndex] = afOldShadowBlurRadius[itrs];
			ptr->tr_acolObjShadowColor[iNewShadowMapsIndex] = acolOldObjShadowColor[itrs];

			iNewShadowMapsIndex++;
		}
	}
	ptr->tr_aulShadowTimes[ctShadowMaps - 1] = aulOldShadowTimes[0];

	for( itrs = 0; itrs < atsOldShadowMaps.Count(); itrs++ )
	{
		TR_MarkTextureUnused( atsOldShadowMaps[itrs] );
		atsOldShadowMaps[itrs] = NULL;
	}
	atsOldShadowMaps.Clear();
	aulOldShadowTimes.Clear();
	afOldShadowBlurRadius.Clear();
}

extern void TRS_AddShadowMap( CTerrain* ptrTerrain )
{
	ASSERT( ptrTerrain != NULL );
	ASSERT( ptrTerrain->tr_ptrTerrain != NULL );

	CTerrainImp*	ptr = ptrTerrain->tr_ptrTerrain;

	const INDEX	ctShadowMaps = ptr->tr_aptdShadowMaps.Count();
	const INDEX	iShadowMapIndex = ctShadowMaps;
	
	CStaticArray<CTextureData*>	atsOldShadowMaps;
	atsOldShadowMaps.sa_Array = ptr->tr_aptdShadowMaps.sa_Array;
	atsOldShadowMaps.sa_Count = ptr->tr_aptdShadowMaps.sa_Count;
	ptr->tr_aptdShadowMaps.sa_Array = NULL;
	ptr->tr_aptdShadowMaps.sa_Count = 0;
	ptr->tr_aptdShadowMaps.New( ctShadowMaps + 1 );

	CStaticArray<ULONG>			aulOldShadowTimes;
	aulOldShadowTimes.sa_Array = ptr->tr_aulShadowTimes.sa_Array;
	aulOldShadowTimes.sa_Count = ptr->tr_aulShadowTimes.sa_Count;
	ptr->tr_aulShadowTimes.sa_Array = NULL;
	ptr->tr_aulShadowTimes.sa_Count = 0;
	ptr->tr_aulShadowTimes.New( ctShadowMaps + 2 );

	CStaticArray<FLOAT>			afOldShadowBlurRadius;
	afOldShadowBlurRadius.sa_Array = ptr->tr_afShadowBlurRadius.sa_Array;
	afOldShadowBlurRadius.sa_Count = ptr->tr_afShadowBlurRadius.sa_Count;
	ptr->tr_afShadowBlurRadius.sa_Array = NULL;
	ptr->tr_afShadowBlurRadius.sa_Count = 0;
	ptr->tr_afShadowBlurRadius.New( ctShadowMaps + 1 );

	CStaticArray<GFXColor>		acolOldObjShadowColor;
	acolOldObjShadowColor.sa_Array = ptr->tr_acolObjShadowColor.sa_Array;
	acolOldObjShadowColor.sa_Count = ptr->tr_acolObjShadowColor.sa_Count;
	ptr->tr_acolObjShadowColor.sa_Array = NULL;
	ptr->tr_acolObjShadowColor.sa_Count = 0;
	ptr->tr_acolObjShadowColor.New( ctShadowMaps + 1 );

	const SLONG	slShadowMapAspect = ptr->tr_slShadowMapSizeAspect;
	const PIX	pixHeightMapWidth = ptr->tr_pixHeightMapWidth;
	const PIX	pixHeightMapHeight = ptr->tr_pixHeightMapHeight;
	const PIX	pixShadowMapWidth = GetShadowMapWidth( pixHeightMapWidth, slShadowMapAspect );
	const PIX	pixShadowMapHeight = GetShadowMapHeight( pixHeightMapHeight, slShadowMapAspect );
	const PIX	pixShadowMapSize = pixShadowMapWidth * pixShadowMapHeight * sizeof(ULONG);

	INDEX itrs;
	for( itrs = 0; itrs < ctShadowMaps; itrs++ )
	{
		ptr->tr_aptdShadowMaps[itrs] = TR_CreateTexture();
		CTextureData*	ptdSrc = atsOldShadowMaps[itrs];
		CTextureData*	ptdDst = ptr->tr_aptdShadowMaps[itrs];
		TR_PrepareTexture( *ptdDst, pixShadowMapWidth, pixShadowMapHeight, PTF_STATIC );
		memcpy( ptdDst->td_pulFrames, ptdSrc->td_pulFrames, pixShadowMapSize );
		TR_UploadTexture( *ptdDst, pixShadowMapWidth, pixShadowMapHeight, PTF_STATIC );

		ptr->tr_aulShadowTimes[itrs] = aulOldShadowTimes[itrs];
		ptr->tr_afShadowBlurRadius[itrs] = afOldShadowBlurRadius[itrs];
		ptr->tr_acolObjShadowColor[itrs] = acolOldObjShadowColor[itrs];
	}

	for( itrs = 0; itrs < ctShadowMaps; itrs++ )
	{
		TR_MarkTextureUnused( atsOldShadowMaps[itrs] );
		atsOldShadowMaps[itrs] = NULL;
	}
	atsOldShadowMaps.Clear();
	aulOldShadowTimes.Clear();
	afOldShadowBlurRadius.Clear();
		
	ptr->tr_aptdShadowMaps[iShadowMapIndex] = TR_CreateTexture();
	CTextureData*	ptdShadow = ptr->tr_aptdShadowMaps[iShadowMapIndex];
	TR_PrepareTexture( *ptdShadow, pixShadowMapWidth, pixShadowMapHeight, PTF_STATIC | PTF_ZEROMEM );
	TR_UploadTexture( *ptdShadow, pixShadowMapWidth, pixShadowMapHeight, PTF_STATIC );

	ptr->tr_aulShadowTimes[iShadowMapIndex] = 0;
	ptr->tr_aulShadowTimes[iShadowMapIndex + 1] = ptr->tr_aulShadowTimes[0];
	ptr->tr_afShadowBlurRadius[iShadowMapIndex] = 0.0f;
	ptr->tr_acolObjShadowColor[iShadowMapIndex].abgr = 0xFF000000;
}

extern void TRS_SetShadowMapIndex( CTerrain* ptrTerrain, INDEX iShadowIndex, INDEX iNewShadowIndex )
{
	ASSERT( ptrTerrain != NULL );
	ASSERT( ptrTerrain->tr_ptrTerrain != NULL );

	CTerrainImp*	ptr = ptrTerrain->tr_ptrTerrain;
	
	const INDEX ctShadowMaps = ptr->tr_aptdShadowMaps.Count();

	if( iShadowIndex < 0 || iShadowIndex >= ctShadowMaps )
	{
		ASSERTALWAYS( "Invalid shadow map index" );
		return ;
	}

	if( iShadowIndex == iNewShadowIndex )
	{
		ASSERTALWAYS( "Old shadow map index is same as new one" );
		return ;
	}

	CStaticArray<CTextureData*>	atsOldShadowMaps;
	atsOldShadowMaps.sa_Array = ptr->tr_aptdShadowMaps.sa_Array;
	atsOldShadowMaps.sa_Count = ptr->tr_aptdShadowMaps.sa_Count;
	ptr->tr_aptdShadowMaps.sa_Array = NULL;
	ptr->tr_aptdShadowMaps.sa_Count = 0;
	ptr->tr_aptdShadowMaps.New( ctShadowMaps );

	CStaticArray<ULONG>			aulOldShadowTimes;
	aulOldShadowTimes.sa_Array = ptr->tr_aulShadowTimes.sa_Array;
	aulOldShadowTimes.sa_Count = ptr->tr_aulShadowTimes.sa_Count;
	ptr->tr_aulShadowTimes.sa_Array = NULL;
	ptr->tr_aulShadowTimes.sa_Count = 0;
	ptr->tr_aulShadowTimes.New( ctShadowMaps + 1 );

	CStaticArray<FLOAT>			afOldShadowBlurRadius;
	afOldShadowBlurRadius.sa_Array = ptr->tr_afShadowBlurRadius.sa_Array;
	afOldShadowBlurRadius.sa_Count = ptr->tr_afShadowBlurRadius.sa_Count;
	ptr->tr_afShadowBlurRadius.sa_Array = NULL;
	ptr->tr_afShadowBlurRadius.sa_Count = 0;
	ptr->tr_afShadowBlurRadius.New( ctShadowMaps );

	CStaticArray<GFXColor>		acolOldObjShadowColor;
	acolOldObjShadowColor.sa_Array = ptr->tr_acolObjShadowColor.sa_Array;
	acolOldObjShadowColor.sa_Count = ptr->tr_acolObjShadowColor.sa_Count;
	ptr->tr_acolObjShadowColor.sa_Array = NULL;
	ptr->tr_acolObjShadowColor.sa_Count = 0;
	ptr->tr_acolObjShadowColor.New( ctShadowMaps );

	INDEX	iOld = iShadowIndex;
	INDEX	iNew = iNewShadowIndex;
	
	const SLONG	slShadowMapAspect = ptr->tr_slShadowMapSizeAspect;
		const PIX	pixHeightMapWidth = ptr->tr_pixHeightMapWidth;
		const PIX	pixHeightMapHeight = ptr->tr_pixHeightMapHeight;
		const PIX	pixShadowMapWidth = GetShadowMapWidth( pixHeightMapWidth, slShadowMapAspect );
		const PIX	pixShadowMapHeight = GetShadowMapHeight( pixHeightMapHeight, slShadowMapAspect );
		const PIX	pixShadowMapSize = pixShadowMapWidth * pixShadowMapHeight * sizeof(ULONG);

	for( INDEX iFrom = 0; iFrom < ctShadowMaps; iFrom++ )
	{
		INDEX	iTo = -1;
		if( iNew == iOld )
			iTo = iFrom;

		if( ( iFrom < iOld && iFrom < iNew ) || ( iFrom > iOld && iFrom > iNew ) )
			iTo = iFrom;
		else if( iFrom == iOld )
			iTo = iNew;
		else
		{
			if( iNew > iOld )
				iTo = iFrom - 1;
			else
				iTo = iFrom + 1;
		}

		ptr->tr_aptdShadowMaps[iTo] = TR_CreateTexture();
		CTextureData*	ptdSrc = atsOldShadowMaps[iFrom];
		CTextureData*	ptdDst = ptr->tr_aptdShadowMaps[iTo];
		TR_PrepareTexture( *ptdDst, pixShadowMapWidth, pixShadowMapHeight, PTF_STATIC );
		memcpy( ptdDst->td_pulFrames, ptdSrc->td_pulFrames, pixShadowMapSize );
		TR_UploadTexture( *ptdDst, pixShadowMapWidth, pixShadowMapHeight, PTF_STATIC );

		ptr->tr_aulShadowTimes[iTo] = aulOldShadowTimes[iFrom];
		ptr->tr_afShadowBlurRadius[iTo] = afOldShadowBlurRadius[iFrom];
		ptr->tr_acolObjShadowColor[iTo] = acolOldObjShadowColor[iFrom];
	}
	ptr->tr_aulShadowTimes[ctShadowMaps] = aulOldShadowTimes[0];

	for( INDEX itrs = 0; itrs < ctShadowMaps; itrs++ )
	{
		TR_MarkTextureUnused( atsOldShadowMaps[itrs] );
		atsOldShadowMaps[itrs] = NULL;
	}
	atsOldShadowMaps.Clear();
	aulOldShadowTimes.Clear();
}

extern void TRS_ClearRealShadowMap( CTerrain* ptrTerrain, INDEX iShadowMap )
{
	ASSERT( ptrTerrain != NULL );
	ASSERT( ptrTerrain->tr_ptrTerrain != NULL );

	CTerrainImp*	ptr = ptrTerrain->tr_ptrTerrain;

	const SLONG	slShadowMapAspect = ptr->tr_slShadowMapSizeAspect;
		const PIX	pixHeightMapWidth = ptr->tr_pixHeightMapWidth;
		const PIX	pixHeightMapHeight = ptr->tr_pixHeightMapHeight;
	const PIX	pixShadowMapWidth = GetShadowMapWidth( pixHeightMapWidth, slShadowMapAspect );
		const PIX	pixShadowMapHeight = GetShadowMapHeight( pixHeightMapHeight, slShadowMapAspect );
	const PIX	pixShadowMapSize = pixShadowMapWidth * pixShadowMapHeight;

	CTextureData*	ptdShadowMap = ptr->tr_aptdShadowMaps[iShadowMap];
	TR_PrepareTexture( *ptdShadowMap, pixShadowMapWidth, pixShadowMapHeight, PTF_STATIC | PTF_ZEROMEM );
	TR_UploadTexture( *ptdShadowMap, pixShadowMapWidth, pixShadowMapHeight, PTF_STATIC );
}
// yjpark     -->|

#endif // FINALVERSION