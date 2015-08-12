#include "StdH.h"
#include <Shaders/Common.h>

void DoSpecularLayer(const INDEX iSpecularTexture, const INDEX iSpecularColor,const BOOL bOpaque)
{
	if(shaUseHWShaders()) 
	{
		return;
	}
	INDEX ivx;
	const INDEX ctVertices = shaGetVertexCount();
	const COLOR colLight = shaGetLightColor();
	const COLOR colAmbient = shaGetAmbientColor();
	const Matrix12 &mObjToAbs = *shaGetObjToAbsMatrix();
	const Matrix12 &mObjToView = *shaGetObjToViewMatrix();
	const GFXVertex *paVertices = shaGetVertexArray();
	GFXNormal *paNormals = shaGetNormalArray();
	GFXTexCoord *ptcUVMap = shaGetNewTexCoordArray();
	FLOAT3D vLightDir = -shaGetLightDirection();
	const FLOAT3D vLightObj = vLightDir.Normalize();
	
	shaCalculateLightForSpecular();
	
	// cache light intensities (-1 in case of overbrighting compensation)
	SLONG slLR = (colLight & CT_RMASK)>>(CT_RSHIFT);
	SLONG slLG = (colLight & CT_GMASK)>>(CT_GSHIFT);
	SLONG slLB = (colLight & CT_BMASK)>>(CT_BSHIFT);
	
	
	// for each vertex
	for( ivx = 0; ivx < ctVertices; ivx++ )
	{
		// reflect light vector around vertex normal in object space
		GFXNormal &nor = paNormals[ivx];
		const FLOAT fNL = nor.nx*vLightObj(1) + nor.ny*vLightObj(2) +	nor.nz*vLightObj(3);
		const FLOAT fRx = vLightObj(1) - 2*nor.nx*fNL;
		const FLOAT fRy = vLightObj(2) - 2*nor.ny*fNL;
		const FLOAT fRz = vLightObj(3) - 2*nor.nz*fNL;
		// transform the reflected vector to viewer space
		const FLOAT fRVx = fRx*mObjToView[ 0] + fRy*mObjToView[ 1] + fRz*mObjToView[ 2];
		const FLOAT fRVy = fRx*mObjToView[ 4] + fRy*mObjToView[ 5] + fRz*mObjToView[ 6];
		const FLOAT fRVz = fRx*mObjToView[ 8] + fRy*mObjToView[ 9] + fRz*mObjToView[10];
		// map reflected vector to texture
		const FLOAT f1oFM = 0.5f / sqrt(2+2*fRVz);  // was 2*sqrt(2+2*fRVz)
		ptcUVMap[ivx].s = fRVx*f1oFM +0.5f;
		ptcUVMap[ivx].t = fRVy*f1oFM +0.5f;
	}
	
	GFXColor colSrfSpec = shaGetColor(iSpecularColor);
	colSrfSpec.AttenuateRGB( (shaGetModelColor()&CT_AMASK)>>CT_ASHIFT);
	colSrfSpec.r = ClampUp( (colSrfSpec.r *slLR)>>8, 255L);
	colSrfSpec.g = ClampUp( (colSrfSpec.g *slLG)>>8, 255L);
	colSrfSpec.b = ClampUp( (colSrfSpec.b *slLB)>>8, 255L);
	
	GFXColor *pcolSpec = shaGetNewColorArray();
	GFXColor *pcolBase = shaGetColorArray();
	
	// for each vertex in the surface
	for( ivx=0; ivx < ctVertices; ivx++ )
	{
		// set specular color
		pcolSpec[ivx] = colSrfSpec;
		pcolSpec[ivx].a = pcolBase[ivx].a;
	}
	
	shaSetColorArray(pcolSpec);
	if(!bOpaque) 
	{
		// Adjust color for fog and haze
		shaAttenuateColorForFogAndHaze();
	}
	
	shaSetTexCoordArray(ptcUVMap);
	shaSetTexture(iSpecularTexture,0);
	shaBlendFunc( GFX_SRC_ALPHA, GFX_ONE);
	shaEnableBlend();
	shaCullFace(GFX_BACK);
	shaRender();
}

void DoReflectionLayer(const INDEX iReflectionTexture,const INDEX iReflectionColor, const BOOL bOpaque)
{
	if(shaUseHWShaders()) 
	{
		return;
	}
	INDEX ctVertices = shaGetVertexCount();
	GFXVertex  *paVertices = shaGetVertexArray();
	GFXNormal  *paNormals  = shaGetNormalArray();
	GFXTexCoord *ptcUVMap  = shaGetNewTexCoordArray();
	
	CAnyProjection3D &apr  = *shaGetProjection();
	Matrix12 &mObjToAbs    = *shaGetObjToAbsMatrix();
	
	BOOL bFullBright = shaGetFlags()&BASE_FULL_BRIGHT;
	// calculate projection of viewer in absolute space
	FLOATmatrix3D &mViewer = apr->pr_ViewerRotationMatrix;
	FLOAT3D vViewer(-mViewer(3,1),-mViewer(3,2),-mViewer(3,3));
	
	// for each vertex
	for(INDEX ivx=0;ivx<ctVertices;ivx++) 
	{
		// get normal in absolute space
		const GFXNormal &nor = paNormals[ivx];
		const FLOAT fNx = nor.nx*mObjToAbs[ 0] + nor.ny*mObjToAbs[ 1] + nor.nz*mObjToAbs[ 2];
		const FLOAT fNy = nor.nx*mObjToAbs[ 4] + nor.ny*mObjToAbs[ 5] + nor.nz*mObjToAbs[ 6];
		const FLOAT fNz = nor.nx*mObjToAbs[ 8] + nor.ny*mObjToAbs[ 9] + nor.nz*mObjToAbs[10];
		// reflect viewer around normal
		const FLOAT fNV  = fNx*vViewer(1) + fNy*vViewer(2) + fNz*vViewer(3);
		const FLOAT fRVx = vViewer(1) - 2*fNx*fNV;
		const FLOAT fRVy = vViewer(2) - 2*fNy*fNV;
		const FLOAT fRVz = vViewer(3) - 2*fNz*fNV;
		// map reflected vector to texture 
		// NOTE: using X and Z axes, so that singularity gets on -Y axis (where it will least probably be seen)
		const FLOAT f1oFM = 0.5f / sqrt(2+2*fRVy);  
		ptcUVMap[ivx].s = fRVx*f1oFM +0.5f;
		ptcUVMap[ivx].t = fRVz*f1oFM +0.5f;
	}
	
	GFXColor *pcolReflection = shaGetNewColorArray();
	
	// get model reflection color
	GFXColor colSrfRefl;
	colSrfRefl.abgr = ByteSwap(shaGetColor(iReflectionColor));
	colSrfRefl.AttenuateA((shaGetModelColor()&CT_AMASK)>>CT_ASHIFT);
	
	if(bFullBright) 
	{
		// just copy reflection color
		for( INDEX ivx=0;ivx<ctVertices;ivx++) 
		{
			pcolReflection[ivx] = colSrfRefl;
		}
	}
	else 
	{
		// set reflection color smooth
		GFXColor *pcolSrfBase = shaGetColorArray();
		for( INDEX ivx=0;ivx<ctVertices;ivx++) 
		{
			pcolReflection[ivx].MultiplyRGBCopyA1( colSrfRefl, pcolSrfBase[ivx]);
		}
	}
	
	shaSetTexCoordArray(ptcUVMap);
	shaSetColorArray(pcolReflection);
	
	if(!bOpaque) 
	{
		// Adjust alpha for fog and haze
		shaAttenuateAlphaForFogAndHaze();
	}
	
	shaSetTexture(iReflectionTexture,0);
	shaBlendFunc( GFX_SRC_ALPHA, GFX_INV_SRC_ALPHA);
	shaEnableBlend();
	
	BOOL bDoubleSided = shaGetFlags()&BASE_DOUBLE_SIDED;
	if(bDoubleSided) 
	{
		shaCullFace(GFX_FRONT);
		shaRender();
	}
	shaCullFace(GFX_BACK);
	shaRender();
}

