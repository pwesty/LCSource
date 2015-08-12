#include "stdh.h"

#include <Engine/Base/Console.h>
#include <Engine/Base/MemoryTracking.h>
#include <Engine/Math/Vector.h>
#include <Engine/Graphics/GfxLibrary.h>
#include <Engine/Graphics/ViewPort.h>

#include <Engine/Base/Statistics_internal.h>


// track texture memory allocated in main heap
#define TRACKVB_HEAP() TRACKMEM(mem, "Vtx buffers (heap)")
// track texture memory allocated in hardware
#define TRACKVB_HW() TRACKMEM(mem, "Vtx buffs (hardware)")


extern INDEX gap_bOptimizeStateChanges;
extern INDEX gap_iOptimizeClipping;
extern INDEX gap_bUseOcclusionCulling;
extern INDEX gap_iDithering;
								
						
// cached states
extern BOOL GFX_bDepthTest  = FALSE;
extern BOOL GFX_bDepthWrite = FALSE;
extern BOOL GFX_bAlphaTest  = FALSE;
extern BOOL GFX_bDithering  = TRUE;
extern BOOL GFX_bBlending   = TRUE;
extern BOOL GFX_bLighting   = FALSE;
extern BOOL GFX_bClipping   = TRUE;
extern BOOL GFX_bClipPlane  = FALSE;
extern BOOL GFX_bColorArray = FALSE;
extern BOOL GFX_bTruform    = FALSE;
extern BOOL GFX_bFrontFace  = TRUE;
extern BOOL GFX_bViewMatrix = TRUE;
extern INDEX GFX_iActiveTexUnit = 0;
extern FLOAT GFX_fMinDepthRange = 0.0f;
extern FLOAT GFX_fMaxDepthRange = 0.0f;

extern BOOL GFX_bUseVertexProgram = FALSE;
extern BOOL GFX_bUsePixelProgram  = FALSE;

extern GfxBlend GFX_eBlendSrc  = GFX_ONE;
extern GfxBlend GFX_eBlendDst  = GFX_ZERO;
extern GfxComp  GFX_eDepthFunc = GFX_LESS_EQUAL;
extern GfxFace  GFX_eCullFace  = GFX_NONE;
extern BOOL  GFX_abLights[GFX_MAXLIGHTS] = { FALSE };
extern BOOL  GFX_abTexture[GFX_MAXTEXUNITS] = { FALSE };
extern INDEX GFX_iTexModulation[GFX_MAXTEXUNITS] = { 0 };

// last ortho/frustum values (frustum has negative sign, because of orgho-frustum switching!)
extern FLOAT GFX_fLastL = 0;
extern FLOAT GFX_fLastR = 0;
extern FLOAT GFX_fLastT = 0;
extern FLOAT GFX_fLastB = 0;
extern FLOAT GFX_fLastN = 0;
extern FLOAT GFX_fLastF = 0;

// number of vertices currently in buffer
extern INDEX GFX_ctVertices = 0;

// vertex buffers control
static CStaticStackArray<VertexBuffer> _avbVertexBuffers;
extern BOOL _bUsingDynamicBuffer = TRUE;
			 
// current texture object/bind number and color mask (for Get... function)
extern ULONG *GFX_apulCurrentTexture[GFX_MAXTEXUNITS] = {0};
extern ULONG GFX_ulCurrentColorMask = (CT_RMASK|CT_GMASK|CT_BMASK|CT_AMASK);

// for D3D: mark need for clipping (when wants to be disable but cannot be because of user clip plane)
static BOOL _bWantsClipping = TRUE;
// locking state for OGL
static BOOL _bCVAReallyLocked = FALSE;

// clip plane and last view matrix for D3D
extern FLOAT D3D_afClipPlane[4]    = {0,0,0,0};
extern FLOAT D3D_afViewMatrix[16]  = {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0};
static FLOAT _afActiveClipPlane[4] = {0,0,0,0};

// Truform/N-Patches
extern INDEX truform_iLevel  = -1;
extern BOOL  truform_bLinear = FALSE;

// Texture transformation matrices
static ULONG _ulUsedTexMatrices = 0;

// functions' pointers
extern void (*gfxEnableDepthWrite)(void) = NULL;
extern void (*gfxEnableDepthBias)(void) = NULL;
extern void (*gfxEnableDepthTest)(void) = NULL;
extern void (*gfxEnableAlphaTest)(void) = NULL;
extern void (*gfxEnableBlend)(void) = NULL;
extern void (*gfxEnableDither)(void) = NULL;
extern void (*gfxEnableTexture)(void) = NULL;
extern void (*gfxEnableLight)( INDEX iLight) = NULL;
extern void (*gfxEnableLighting)(void) = NULL;
extern void (*gfxEnableClipping)(void) = NULL;
extern void (*gfxEnableClipPlane)(void) = NULL;
extern void (*gfxDisableDepthWrite)(void) = NULL;
extern void (*gfxDisableDepthBias)(void) = NULL;
extern void (*gfxDisableDepthTest)(void) = NULL;
extern void (*gfxDisableAlphaTest)(void) = NULL;
extern void (*gfxDisableBlend)(void) = NULL;
extern void (*gfxDisableDither)(void) = NULL;
extern void (*gfxDisableTexture)(void) = NULL;
extern void (*gfxDisableLight)( INDEX iLight) = NULL;
extern void (*gfxDisableLighting)(void) = NULL;
extern void (*gfxDisableClipping)(void) = NULL;
extern void (*gfxDisableClipPlane)(void) = NULL;
extern void (*gfxBlendFunc)( GfxBlend eSrc, GfxBlend eDst) = NULL;
extern void (*gfxDepthFunc)( GfxComp eFunc) = NULL;
extern void (*gfxDepthRange)( FLOAT fMin, FLOAT fMax) = NULL;
extern void (*gfxCullFace)(  GfxFace eFace) = NULL;
extern void (*gfxFrontFace)( GfxFace eFace) = NULL;
extern void (*gfxClipPlane)( const DOUBLE *pdPlane) = NULL;
extern void (*gfxSetOrtho)( const FLOAT fLeft, const FLOAT fRight, const FLOAT fTop,  const FLOAT fBottom, const FLOAT fNear, const FLOAT fFar, const BOOL bSubPixelAdjust) = NULL;
extern void (*gfxSetFrustum)( FLOAT fLeft, FLOAT fRight, FLOAT fTop, FLOAT fBottom, FLOAT fNear, FLOAT fFar) = NULL;
extern void (*gfxSetLightColor)( INDEX iLight, COLOR colLight, COLOR colAmbient, COLOR colSpecular/*=0*/) = NULL;
extern void (*gfxSetLightDirection)( INDEX iLight, const FLOAT3D &vLightDir) = NULL;
extern void (*gfxSetTextureMatrix)( const FLOAT *pfMatrix) = NULL;
extern void (*gfxSetViewMatrix)( const FLOAT *pfMatrix) = NULL;
extern void (*gfxPolygonMode)( GfxPolyMode ePolyMode) = NULL;
extern void (*gfxSetTextureWrapping)( enum GfxWrap eWrapU, enum GfxWrap eWrapV) = NULL;
extern void (*gfxSetTextureModulation)( INDEX iScale) = NULL;
extern void (*gfxGenerateTexture)( ULONG &ulTexObject) = NULL;
extern void (*gfxDeleteTexture)( ULONG &ulTexObject) = NULL;
extern void (*gfxSetVertexArray)( GFXVertex *pvtx, INDEX ctVtx) = NULL;
extern void (*gfxSetNormalArray)( GFXNormal *pnor) = NULL;
extern void (*gfxSetWeightArray)( GFXWeight *pwgh) = NULL;
extern void (*gfxSetTexCoordArray)( GFXTexCoord *ptex, BOOL bProjectiveMapping) = NULL;
extern void (*gfxSetColorArray)( GFXColor *pcol) = NULL;
extern void (*gfxDrawElements)( const INDEX ctIndices, const UWORD *puwIndices) = NULL;
extern void (*gfxSetConstantColor)(COLOR col) = NULL;
extern void (*gfxEnableColorArray)(void) = NULL;
extern void (*gfxDisableColorArray)(void) = NULL;
extern void (*gfxFinish)( BOOL bReadBack) = NULL;
extern void (*gfxLockArrays)(void) = NULL;
extern void (*gfxEnableTruform)( void) = NULL;
extern void (*gfxDisableTruform)(void) = NULL;
extern void (*gfxSetColorMask)( ULONG ulColorMask) = NULL; 
extern void* (*gfxLockSubBuffer)(   const INDEX iID, const INDEX i1stVertex, const INDEX ctVertices, const enum GfxLockType eLockType) = NULL;
extern void  (*gfxUnlockSubBuffer)( const INDEX iID, const INDEX ctVertices) = NULL;
extern void  (*gfxSetVertexSubBuffer)( const INDEX iID, const INDEX i1stVertex, const INDEX ctVertices) = NULL;
extern void  (*gfxSetSubBuffer)( const INDEX iID, const INDEX iUnit) = NULL;

extern void  (*gfxSetVertexProgram)( const ULONG ulHandle) = NULL;
extern void  (*gfxSetPixelProgram)(  const ULONG ulHandle) = NULL;
extern void  (*gfxDeleteVertexProgram)( ULONG ulHandle) = NULL;
extern void  (*gfxDeletePixelProgram)(  ULONG ulHandle) = NULL;
extern void  (*gfxSetVertexProgramConst)( INDEX iRegister, const void *pData, INDEX ctRegisters) = NULL;
extern void  (*gfxSetPixelProgramConst)(  INDEX iRegister, const void *pData, INDEX ctRegisters) = NULL;
extern ULONG (*gfxCreateVertexProgram)( const char *strVertexProgram, ULONG ulStreamFlags) = NULL;
extern ULONG (*gfxCreatePixelProgram)(  const char *strVertexProgram) = NULL;


// dummy functions (one size fits all:)
static void none_void(void)
{
	ASSERT( _pGfx->gl_eCurrentAPI==GAT_NONE);
}

static void none_int( INDEX iScale)
{
	ASSERT( _pGfx->gl_eCurrentAPI==GAT_NONE);
}

static void none_const_int( const INDEX iScale)
{
	ASSERT( _pGfx->gl_eCurrentAPI==GAT_NONE);
}

static void none_const_ulong( const ULONG ul)
{
	ASSERT( _pGfx->gl_eCurrentAPI==GAT_NONE);
}

static void none_ulong(ULONG ul)
{
	ASSERT( _pGfx->gl_eCurrentAPI==GAT_NONE);
}


// error checkers (this is for debug version only)


extern void OGL_CheckError(void)
{
#ifndef NDEBUG
	const GfxAPIType eAPI = _pGfx->gl_eCurrentAPI;
	if( eAPI==GAT_OGL) ASSERT( pglGetError()==GL_NO_ERROR);
	else ASSERT( eAPI==GAT_NONE);
#endif
}



extern void D3D_CheckError(HRESULT hr)
{
#ifndef NDEBUG
	const GfxAPIType eAPI = _pGfx->gl_eCurrentAPI;
	if( eAPI==GAT_D3D) ASSERT( hr==D3D_OK);
	else ASSERT( eAPI==GAT_NONE);
#endif
}



// TEXTURE MANAGEMENT

//안태훈 수정 시작	//(Remake Effect)(0.1)
//d3d의 SetTextureStageState를 wrapping한 것임. 태훈
extern void gfxSetTextureSelectArg(INDEX iStage, GFX_TEXTURE_ARG argColor, GFX_TEXTURE_ARG argAlpha)
{
	// check API
	const GfxAPIType eAPI = _pGfx->gl_eCurrentAPI;
	ASSERT( eAPI==GAT_D3D );
	
	//extern INDEX GFX_iActiveTexUnit;
	//extern INDEX GFX_iTexModulation[GFX_MAXTEXUNITS];
	GFX_iTexModulation[GFX_iActiveTexUnit] = 0;

	HRESULT hr;
	hr = _pGfx->gl_pd3dDevice->SetTextureStageState( iStage, D3DTSS_COLORARG1, argColor);
	hr = _pGfx->gl_pd3dDevice->SetTextureStageState( iStage, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	hr = _pGfx->gl_pd3dDevice->SetTextureStageState( iStage, D3DTSS_ALPHAARG1, argAlpha);
	hr = _pGfx->gl_pd3dDevice->SetTextureStageState( iStage, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	GFX_abTexture[GFX_iActiveTexUnit] = D3DTOP_SELECTARG1;
}
//안태훈 수정 끝	//(Remake Effect)(0.1)


// update texture LOD bias
extern FLOAT _fCurrentLODBias = 0;  // LOD bias adjuster
extern void UpdateLODBias( const FLOAT fLODBias)
{ 
	// check API
	const GfxAPIType eAPI = _pGfx->gl_eCurrentAPI;
	ASSERT( eAPI==GAT_OGL || eAPI==GAT_D3D || eAPI==GAT_NONE);
	// only if supported and needed
	if( _fCurrentLODBias==fLODBias && _pGfx->gl_fMaxTextureLODBias==0) return;
	_fCurrentLODBias = fLODBias;

	_sfStats.StartTimer(CStatForm::STI_GFXAPI);

	// OpenGL
	if( eAPI==GAT_OGL) 
	{ // if no multitexturing
		if( _pGfx->gl_ctTextureUnits<2) { 
			pglTexEnvf( GL_TEXTURE_FILTER_CONTROL_EXT, GL_TEXTURE_LOD_BIAS_EXT, fLODBias);
			OGL_CHECKERROR;
		} 
		// if multitexturing is active
		else {
			for( INDEX iUnit=0; iUnit<_pGfx->gl_ctTextureUnits; iUnit++) { // loop thru units
				pglActiveTexture(iUnit);  // select the unit
				pglTexEnvf( GL_TEXTURE_FILTER_CONTROL_EXT, GL_TEXTURE_LOD_BIAS_EXT, fLODBias);
				OGL_CHECKERROR;
			} // reselect the original unit
			pglActiveTexture(GFX_iActiveTexUnit);
			OGL_CHECKERROR;
		}
	}
	// Direct3D
	else if( eAPI==GAT_D3D)
	{ // just set it
		HRESULT hr;
		for( INDEX iUnit=0; iUnit<_pGfx->gl_ctTextureUnits; iUnit++) { // loop thru tex units
			hr = _pGfx->gl_pd3dDevice->SetTextureStageState( iUnit, D3DTSS_MIPMAPLODBIAS, *((DWORD*)&fLODBias));
			D3D_CHECKERROR(hr);
		}
	}
	_sfStats.StopTimer(CStatForm::STI_GFXAPI);
}



// get current texture filtering mode
extern void gfxGetTextureFiltering( INDEX &iFilterType, INDEX &iAnisotropyDegree)
{
	iFilterType = _tpGlobal[0].tp_iFilter;
	iAnisotropyDegree = _tpGlobal[0].tp_iAnisotropy;
}


// set texture filtering mode
extern void gfxSetTextureFiltering( INDEX &iFilterType, INDEX &iAnisotropyDegree)
{              
	// clamp vars
	INDEX iMagTex = iFilterType /100;     iMagTex = Clamp( iMagTex, 0L, 2L);  // 0=same as iMinTex, 1=nearest, 2=linear
	INDEX iMinTex = iFilterType /10 %10;  iMinTex = Clamp( iMinTex, 1L, 2L);  // 1=nearest, 2=linear
	INDEX iMinMip = iFilterType %10;      iMinMip = Clamp( iMinMip, 0L, 2L);  // 0=no mipmapping, 1=nearest, 2=linear
	iFilterType   = iMagTex*100 + iMinTex*10 + iMinMip;
	iAnisotropyDegree = Clamp( iAnisotropyDegree, 1L, _pGfx->gl_iMaxTextureAnisotropy);

	// skip if not changed
	if( _tpGlobal[0].tp_iFilter==iFilterType && _tpGlobal[0].tp_iAnisotropy==iAnisotropyDegree) return;
	_tpGlobal[0].tp_iFilter = iFilterType;
	_tpGlobal[0].tp_iAnisotropy = iAnisotropyDegree;

	// for OpenGL, that's about it
	if( _pGfx->gl_eCurrentAPI!=GAT_D3D) return;

	_sfStats.StartTimer(CStatForm::STI_GFXAPI);

	// for D3D, it's a stage state (not texture state), so change it!
	HRESULT hr;
 _D3DTEXTUREFILTERTYPE eMagFilter, eMinFilter, eMipFilter;
	const LPDIRECT3DDEVICE8 pd3dDev = _pGfx->gl_pd3dDevice; 
	extern void UnpackFilter_D3D( INDEX iFilter, _D3DTEXTUREFILTERTYPE &eMagFilter,
															 _D3DTEXTUREFILTERTYPE &eMinFilter, _D3DTEXTUREFILTERTYPE &eMipFilter);
	UnpackFilter_D3D( iFilterType, eMagFilter, eMinFilter, eMipFilter);
	if( iAnisotropyDegree>1) { // adjust filter for anisotropy
		eMagFilter = D3DTEXF_ANISOTROPIC;
		eMinFilter = D3DTEXF_ANISOTROPIC;
	}
	// set filtering and anisotropy degree
	for( INDEX iUnit=0; iUnit<_pGfx->gl_ctTextureUnits; iUnit++) { // must loop thru all usable texture units
		hr = pd3dDev->SetTextureStageState( iUnit, D3DTSS_MAXANISOTROPY, iAnisotropyDegree);  D3D_CHECKERROR(hr);
		hr = pd3dDev->SetTextureStageState( iUnit, D3DTSS_MAGFILTER, eMagFilter);  D3D_CHECKERROR(hr);
		hr = pd3dDev->SetTextureStageState( iUnit, D3DTSS_MINFILTER, eMinFilter);  D3D_CHECKERROR(hr);
		hr = pd3dDev->SetTextureStageState( iUnit, D3DTSS_MIPFILTER, eMipFilter);  D3D_CHECKERROR(hr);
	}
	// done
	_sfStats.StopTimer(CStatForm::STI_GFXAPI);
}


// set new texture LOD biasing
extern void gfxSetTextureBiasing( FLOAT &fLODBias)
{
	// adjust LOD biasing if needed
	fLODBias = Clamp( fLODBias, -_pGfx->gl_fMaxTextureLODBias, +_pGfx->gl_fMaxTextureLODBias); 
	if( _pGfx->gl_fTextureLODBias != fLODBias) {
		_pGfx->gl_fTextureLODBias = fLODBias;
		UpdateLODBias( fLODBias);
	}
}



// set texture unit as active
extern void gfxSetTextureUnit( INDEX iUnit)
{
	// check API
	const GfxAPIType eAPI = _pGfx->gl_eCurrentAPI;
	ASSERT( eAPI==GAT_OGL || eAPI==GAT_D3D || eAPI==GAT_NONE);
	ASSERT( iUnit>=0 && iUnit<GFX_MAXTEXUNITS); 

	// check consistency
#ifndef NDEBUG
	if( eAPI==GAT_OGL) {
		GLint gliRet;
		pglGetIntegerv( GL_ACTIVE_TEXTURE_ARB, &gliRet);
		ASSERT( GFX_iActiveTexUnit==(gliRet-GL_TEXTURE0_ARB));
		pglGetIntegerv( GL_CLIENT_ACTIVE_TEXTURE_ARB, &gliRet);
		ASSERT( GFX_iActiveTexUnit==(gliRet-GL_TEXTURE0_ARB));
	}
#endif

	// cached?
	if( GFX_iActiveTexUnit==iUnit) return;
	GFX_iActiveTexUnit = iUnit;

	// really set only for OpenGL
	if( eAPI!=GAT_OGL) return;

	_sfStats.StartTimer(CStatForm::STI_GFXAPI);
	pglActiveTexture(iUnit);
	_sfStats.StopTimer(CStatForm::STI_GFXAPI);
}



// set texture as current
extern void gfxSetTexture( ULONG &ulTexObject, CTexParams &tpLocal)
{
	// clamp texture filtering if needed
	static INDEX _iLastTextureFiltering = 0;
	if( _iLastTextureFiltering != _tpGlobal[0].tp_iFilter) {
		INDEX iMagTex = _tpGlobal[0].tp_iFilter /100;     iMagTex = Clamp( iMagTex, 0L, 2L);  // 0=same as iMinTex, 1=nearest, 2=linear
		INDEX iMinTex = _tpGlobal[0].tp_iFilter /10 %10;  iMinTex = Clamp( iMinTex, 1L, 2L);  // 1=nearest, 2=linear
		INDEX iMinMip = _tpGlobal[0].tp_iFilter %10;      iMinMip = Clamp( iMinMip, 0L, 2L);  // 0=no mipmapping, 1=nearest, 2=linear
		_tpGlobal[0].tp_iFilter = iMagTex*100 + iMinTex*10 + iMinMip;
		_iLastTextureFiltering  = _tpGlobal[0].tp_iFilter;
	}

	// determine API, enable texturing and keep current texture
	const GfxAPIType eAPI = _pGfx->gl_eCurrentAPI;
	ASSERT( eAPI==GAT_OGL || eAPI==GAT_D3D || eAPI==GAT_NONE);
	gfxEnableTexture();

	_sfStats.StartTimer(CStatForm::STI_BINDTEXTURE);
	_sfStats.StartTimer(CStatForm::STI_GFXAPI);

	if( eAPI==GAT_OGL) { // OpenGL
		GFX_apulCurrentTexture[GFX_iActiveTexUnit] = (ULONG*)ulTexObject;  // OpenGL tracks bind number
		extern void MimicTexParams_OGL( CTexParams &tpLocal);
		pglBindTexture( GL_TEXTURE_2D, ulTexObject);
		MimicTexParams_OGL(tpLocal);
	} 
	else if( eAPI==GAT_D3D) { // Direct3D
		GFX_apulCurrentTexture[GFX_iActiveTexUnit] = &ulTexObject;    // Direct3D track pointer to texture pointer
		extern void MimicTexParams_D3D( CTexParams &tpLocal);
		HRESULT hr = _pGfx->gl_pd3dDevice->SetTexture( GFX_iActiveTexUnit, (LPDIRECT3DTEXTURE8)ulTexObject);
		D3D_CHECKERROR(hr);
		MimicTexParams_D3D(tpLocal);
	}
	// done
	_sfStats.StopTimer(CStatForm::STI_BINDTEXTURE);
	_sfStats.StopTimer(CStatForm::STI_GFXAPI);
}



// upload texture
extern void gfxUploadTexture( ULONG *pulTexture, PIX pixWidth, PIX pixHeight, ULONG ulFormat, BOOL bNoDiscard)
{
	// determine API
	const GfxAPIType eAPI = _pGfx->gl_eCurrentAPI;
	ASSERT( eAPI==GAT_OGL || eAPI==GAT_D3D || eAPI==GAT_NONE);
	_sfStats.StartTimer(CStatForm::STI_GFXAPI);

	if( eAPI==GAT_OGL) { // OpenGL
		extern void UploadTexture_OGL( ULONG *pulTexture, PIX pixSizeU, PIX pixSizeV, GLenum eInternalFormat, BOOL bUseSubImage);
		UploadTexture_OGL( pulTexture, pixWidth, pixHeight, (GLenum)ulFormat, bNoDiscard);
	}
	else if( eAPI==GAT_D3D) { // Direct3D
		LPDIRECT3DTEXTURE8 *ppd3dCurrentTexture  = (LPDIRECT3DTEXTURE8*)GFX_apulCurrentTexture[GFX_iActiveTexUnit];
		const LPDIRECT3DTEXTURE8 pd3dLastTexture = *ppd3dCurrentTexture;
		extern void UploadTexture_D3D( LPDIRECT3DTEXTURE8 *ppd3dTexture, ULONG *pulTexture, PIX pixSizeU, PIX pixSizeV, D3DFORMAT eInternalFormat, BOOL bDiscard);
		UploadTexture_D3D( ppd3dCurrentTexture, pulTexture, pixWidth, pixHeight, (D3DFORMAT)ulFormat, !bNoDiscard);
		// in case that texture has been changed, must re-set it as current
		if( pd3dLastTexture != *ppd3dCurrentTexture) {
			HRESULT hr = _pGfx->gl_pd3dDevice->SetTexture( GFX_iActiveTexUnit, *ppd3dCurrentTexture);
			D3D_CHECKERROR(hr);
		}
	} 
	// done
	_sfStats.StopTimer(CStatForm::STI_GFXAPI);
}



// uploads compressed frames to compressed texture
// NOTE: number of mipmaps is determined from size of the frame and the size of each mipmap
extern BOOL gfxUploadCompressedTexture( UBYTE *pubMipmaps, PIX pixWidth, PIX pixHeight, SLONG slSize, ULONG ulFormat)
{
	// determine API
	const GfxAPIType eAPI = _pGfx->gl_eCurrentAPI;
	ASSERT( eAPI==GAT_OGL || eAPI==GAT_D3D || eAPI==GAT_NONE);
	_sfStats.StartTimer(CStatForm::STI_GFXAPI);
	BOOL bUploaded = FALSE;

	if( eAPI==GAT_OGL) { // OpenGL
		extern BOOL UploadCompressedTexture_OGL( UBYTE *pubTexture, PIX pixSizeU, PIX pixSizeV, SLONG slSize, GLenum eInternalFormat);
		bUploaded = UploadCompressedTexture_OGL( pubMipmaps, pixWidth, pixHeight, slSize, (GLenum)ulFormat);
	}
	else if( eAPI==GAT_D3D) { // Direct3D
		LPDIRECT3DTEXTURE8 *ppd3dCurrentTexture  = (LPDIRECT3DTEXTURE8*)GFX_apulCurrentTexture[GFX_iActiveTexUnit];
		const LPDIRECT3DTEXTURE8 pd3dLastTexture = *ppd3dCurrentTexture;
		extern BOOL UploadCompressedTexture_D3D( LPDIRECT3DTEXTURE8 *ppd3dTexture, UBYTE *pubTexture, PIX pixSizeU, PIX pixSizeV, SLONG slSize, D3DFORMAT eInternalFormat);
		bUploaded = UploadCompressedTexture_D3D( ppd3dCurrentTexture, pubMipmaps, pixWidth, pixHeight, slSize, (D3DFORMAT)ulFormat);
		// in case that texture has been changed, must re-set it as current
		if( pd3dLastTexture != *ppd3dCurrentTexture) {
			HRESULT hr = _pGfx->gl_pd3dDevice->SetTexture( GFX_iActiveTexUnit, *ppd3dCurrentTexture);
			D3D_CHECKERROR(hr);
		}
	}
	// done
	_sfStats.StopTimer(CStatForm::STI_GFXAPI);
	return bUploaded;
}



// returns size of uploaded texture
extern SLONG gfxGetTextureSize( ULONG ulTexObject, BOOL bHasMipmaps/*=TRUE*/)
{
	// nothing used if nothing uploaded
	if( ulTexObject==NULL) return 0;

	// determine API
	const GfxAPIType eAPI = _pGfx->gl_eCurrentAPI;
	ASSERT( eAPI==GAT_OGL || eAPI==GAT_D3D || eAPI==GAT_NONE);
	SLONG slMipSize;

	_sfStats.StartTimer(CStatForm::STI_GFXAPI);

	// OpenGL
	if( eAPI==GAT_OGL)
	{ 
		// was texture compressed?
		pglBindTexture( GL_TEXTURE_2D, ulTexObject); 
		BOOL bCompressed = FALSE;
		if( _pGfx->gl_ulFlags & GLF_EXTC_ARB) {
			pglGetTexLevelParameteriv( GL_TEXTURE_2D, 0, GL_TEXTURE_COMPRESSED_ARB, (BOOL*)&bCompressed);
			OGL_CHECKERROR;
		}
		// for compressed textures, determine size directly
		if( bCompressed) {
			pglGetTexLevelParameteriv( GL_TEXTURE_2D, 0, GL_TEXTURE_COMPRESSED_IMAGE_SIZE_ARB, (GLint*)&slMipSize);
			OGL_CHECKERROR;
		}
		// non-compressed textures goes thru determination of internal format
		else {
			PIX pixWidth, pixHeight;
			pglGetTexLevelParameteriv( GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH,  (GLint*)&pixWidth);
			pglGetTexLevelParameteriv( GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, (GLint*)&pixHeight);
			OGL_CHECKERROR;
			slMipSize = pixWidth*pixHeight * gfxGetTexturePixRatio(ulTexObject);
		}
	}
	// Direct3D
	else if( eAPI==GAT_D3D)
	{
		// we can determine exact size from texture surface (i.e. mipmap)
		D3DSURFACE_DESC d3dSurfDesc;
		HRESULT hr = ((LPDIRECT3DTEXTURE8)ulTexObject)->GetLevelDesc( 0, &d3dSurfDesc);
		D3D_CHECKERROR(hr);
		slMipSize = d3dSurfDesc.Size;
	}

	// eventually count in all the mipmaps (takes extra 33% of texture size)
	const SLONG slUploadSize = bHasMipmaps ? slMipSize*4/3 : slMipSize;

	_sfStats.StopTimer(CStatForm::STI_GFXAPI);
	return slUploadSize;
}



// returns bytes/pixels ratio for uploaded texture
extern INDEX gfxGetTexturePixRatio( ULONG ulTextureObject)
{
	// determine API
	const GfxAPIType eAPI = _pGfx->gl_eCurrentAPI;
	ASSERT( eAPI==GAT_OGL || eAPI==GAT_D3D || eAPI==GAT_NONE);
	if( eAPI==GAT_OGL) {
		extern INDEX GetTexturePixRatio_OGL( GLuint uiBindNo);
		return GetTexturePixRatio_OGL( (GLuint)ulTextureObject);
	}
	else if( eAPI==GAT_D3D) {
		extern INDEX GetTexturePixRatio_D3D( LPDIRECT3DTEXTURE8 pd3dTexture);
		return GetTexturePixRatio_D3D( (LPDIRECT3DTEXTURE8)ulTextureObject);
	}
	else return 0;
}


// returns bytes/pixels ratio for uploaded texture
extern INDEX gfxGetFormatPixRatio( ULONG ulTextureFormat)
{
	// determine API
	const GfxAPIType eAPI = _pGfx->gl_eCurrentAPI;
	ASSERT( eAPI==GAT_OGL || eAPI==GAT_D3D || eAPI==GAT_NONE);
	if( eAPI==GAT_OGL) {
		extern INDEX GetFormatPixRatio_OGL( GLenum eFormat);
		return GetFormatPixRatio_OGL( (GLenum)ulTextureFormat);
	}
	else if( eAPI==GAT_D3D) {
		extern INDEX GetFormatPixRatio_D3D( D3DFORMAT d3dFormat);
		return GetFormatPixRatio_D3D( (D3DFORMAT)ulTextureFormat);
	}
	else return 0;
}



// PATTERN TEXTURE FOR LINES

CTexParams _tpPattern;
extern ULONG _ulPatternTexture = NONE;
extern ULONG _ulLastUploadedPattern = 0;

// upload pattern to accelerator memory
extern void gfxSetPattern( ULONG ulPattern)
{
	// set pattern to be current texture
	_tpPattern.tp_bSingleMipmap = TRUE;
	gfxSetTextureWrapping( GFX_REPEAT, GFX_REPEAT);
	gfxSetTexture( _ulPatternTexture, _tpPattern);

	// if this pattern is currently uploaded, do nothing
	if( _ulLastUploadedPattern==ulPattern) return;

	// convert bits to ULONGs
	ULONG aulPattern[32];
	for( INDEX iBit=0; iBit<32; iBit++) {
		if( (0x80000000>>iBit) & ulPattern) aulPattern[iBit] = 0xFFFFFFFF;
		else aulPattern[iBit] = 0x00000000;
	}
	// remember new pattern and upload
	_ulLastUploadedPattern = ulPattern;
	gfxUploadTexture( &aulPattern[0], 32, 1, TS.ts_tfRGBA8, FALSE);
}



// VERTEX ARRAYS

extern void gfxUnlockArrays(void)
{
	// only if locked (OpenGL can lock 'em)
	if( !_bCVAReallyLocked) return;
	ASSERT( _pGfx->gl_eCurrentAPI==GAT_OGL);
#ifndef NDEBUG
	INDEX glctRet;
	pglGetIntegerv( GL_ARRAY_ELEMENT_LOCK_COUNT_EXT, (GLint*)&glctRet);
	ASSERT( glctRet==GFX_ctVertices);
#endif
	pglUnlockArraysEXT();
	OGL_CHECKERROR;
 _bCVAReallyLocked = FALSE;
}



// VERTEX BUFFERS FAST CONTROL


static const SLONG _aslVBTypeSizes[GFX_MAX_VBA] = {
	GFX_POSSIZE, GFX_NORSIZE, GFX_WGHSIZE, GFX_COLSIZE,
	GFX_TEXSIZE, GFX_TEXSIZE, GFX_TEXSIZE, GFX_TEXSIZE,
	GFX_TEXSIZE, GFX_TEXSIZE, GFX_TEXSIZE, GFX_TEXSIZE,
//안태훈 수정 시작	//(Add Tagent-space Normal Map)(0.1)
	GFX_TANSIZE,
//안태훈 수정 끝	//(Add Tagent-space Normal Map)(0.1)
	};


// helper: returns size of vertex buffer type
static inline SLONG GetVBTypeSize( const INDEX iType)
{
	ASSERT( iType>=0 && iType<GFX_MAX_VBM);
	return _aslVBTypeSizes[iType];
}


// re-inits all vertex buffers
extern void InitVertexBuffers(void)
{
	// determine API
	const GfxAPIType eAPI = _pGfx->gl_eCurrentAPI;
	ASSERT( eAPI==GAT_OGL || eAPI==GAT_D3D || eAPI==GAT_NONE);

	// vertex buffers should be deleted by now
	const INDEX ctBuffers = _avbVertexBuffers.Count();
	for( INDEX iBuf=1; iBuf<ctBuffers; iBuf++) {
		ASSERT(_avbVertexBuffers[iBuf].vb_ulArrayMask==NONE);
	}
	_avbVertexBuffers.Clear();  // clear 'em for good

	// set one for dynamic array
	VertexBuffer &vb  = _avbVertexBuffers.Push();
	vb.vb_ulLockMask  = NONE;
//안태훈 수정 시작	//(Add Tagent-space Normal Map)(0.1)
//	vb.vb_ulArrayMask = GFX_VBM_POS|GFX_VBM_NOR|GFX_VBM_WGH|GFX_VBM_COL|GFX_VBM_TEX; //원본
	vb.vb_ulArrayMask = GFX_VBM_POS|GFX_VBM_NOR|GFX_VBM_WGH|GFX_VBM_COL|GFX_VBM_TEX|GFX_VBM_TAN;
//안태훈 수정 끝	//(Add Tagent-space Normal Map)(0.1)
	vb.vb_eType       = GFX_DYNAMIC;
}


// prepare a vertex buffer (returns somekind of 'bind' number, zero if failed)
extern INDEX gfxCreateVertexBuffer( const INDEX ctVertices, ULONG ulTypeMask, INDEX ctTexCoordArrays/*=-1*/,
																		const enum GfxLockType eType/*=GFX_WRITEONLY*/)
{
	// determine API
	const GfxAPIType eAPI = _pGfx->gl_eCurrentAPI;
	ASSERT( eAPI==GAT_OGL || eAPI==GAT_D3D || eAPI==GAT_NONE);
	ASSERT(eType==GFX_READWRITE || eType==GFX_WRITEONLY || eType==GFX_DYNAMIC);

	// check number of tex coord arrays and reconstruct mask
	if( ctTexCoordArrays==-1) ctTexCoordArrays = (ulTypeMask&GFX_VBA_TEX) ? 1 : 0;
	ASSERT( ((ulTypeMask&GFX_VBM_TEX) && ctTexCoordArrays>=1)
			|| (!(ulTypeMask&GFX_VBM_TEX) && ctTexCoordArrays==0));
	for( INDEX iTCA=1; iTCA<ctTexCoordArrays; iTCA++) ulTypeMask |= (GFX_VBM_TEX<<iTCA);

	// isolate flags
	const BOOL bRead    = (eType==GFX_READWRITE);
	const BOOL bDynamic = (eType==GFX_DYNAMIC);

	ASSERT(_avbVertexBuffers.Count()>0); // Init must be called before create

	// find first available VB
	INDEX iVB, iVA;
	const INDEX ctBuffers = _avbVertexBuffers.Count();
	for( iVB=1; iVB<ctBuffers; iVB++) {
		if( _avbVertexBuffers[iVB].vb_ulArrayMask==NONE) break;
	}
	// add new if free one was not found
	if( iVB==ctBuffers) {
		TRACKMEM(mem,"Vtx buffs");
		_avbVertexBuffers.Push();
	}
	VertexBuffer &vb = _avbVertexBuffers[iVB];
	SLONG slTotalSize = 0;

	// OpenGL
	if( eAPI==GAT_OGL)
	{
		for( iVA=0; iVA<GFX_MAX_VBA; iVA++) {
			vb.vb_paReadArray[iVA] = vb.vb_paWriteArray[iVA] = NULL;
			if( !((ulTypeMask>>iVA)&1)) continue;
			const SLONG slSize = ctVertices*_aslVBTypeSizes[iVA];
			vb.vb_paWriteArray[iVA] = AllocMemory(slSize);
			if( bRead) vb.vb_paReadArray[iVA] = vb.vb_paWriteArray[iVA];
			slTotalSize += slSize;
		}
	}

	// Direct3D
	else if( eAPI==GAT_D3D)
	{
		// for PC
		DWORD dwFlags = D3DUSAGE_WRITEONLY;
		if( bDynamic) dwFlags |= D3DUSAGE_DYNAMIC;
		const LPDIRECT3DDEVICE8 pd3dDev = _pGfx->gl_pd3dDevice; 

		{TRACKVB_HEAP();

		for( iVA=0; iVA<GFX_MAX_VBA; iVA++) {
			vb.vb_paReadArray[iVA] = vb.vb_paWriteArray[iVA] = NULL;
			vb.vb_i1stLockedVertex[iVA] = vb.vb_ctLockedVertices[iVA] = 0;
			if( !((ulTypeMask>>iVA)&1)) continue;
			const SLONG slSize = ctVertices*_aslVBTypeSizes[iVA];
			if( bRead) vb.vb_paReadArray[iVA] = AllocMemory(slSize);
			HRESULT hr = pd3dDev->CreateVertexBuffer( slSize, dwFlags, 0, D3DPOOL_DEFAULT, &vb.vb_pavbWrite[iVA]);
			if( hr!=D3D_OK) break;  // out of memory (probably)
			slTotalSize += slSize;
		}

		// if we ran out of memory, must delete created vertex buffers so far
		if( iVA<GFX_MAX_VBA)
		{
//안태훈 수정 시작	//(DevPartner Bug Fix)(2005-01-10)
			for( iVA=0; iVA<GFX_MAX_VBA; iVA++)
			{
				if((ulTypeMask>>iVA)&1)
				{
					if(vb.vb_paReadArray[iVA]==NULL)
					{
						FreeMemory( vb.vb_paReadArray[iVA]);
						vb.vb_paReadArray[iVA] = NULL;
					}
				}
				if( !((ulTypeMask>>iVA)&1) || vb.vb_paWriteArray[iVA]==NULL) continue;
				D3DRELEASE( vb.vb_pavbWrite[iVA], TRUE);
				vb.vb_pavbWrite[iVA] = NULL;
			} // sorry, didn't make it
//안태훈 수정 끝	//(DevPartner Bug Fix)(2005-01-10)
			return FALSE;
		}
		}//TRACKVB_HEAP();

		{TRACKVB_HW();
	#if MEMORY_TRACKING
		// signal to memory tracker  
		const ULONG ulObject = (ULONG)(&vb.vb_pavbWrite[0]);
		MEMTRACK_ALLOC( (void*)(ulObject^0x80000000), (slTotalSize+4095)/4096*4096, slTotalSize);
	#endif
		}//TRACKVB_HW();

	}

	// phew, made it
	vb.vb_slSize      = slTotalSize;
	vb.vb_ctVertices  = ctVertices;
	vb.vb_ulArrayMask = ulTypeMask;
	vb.vb_ulLockMask  = NONE;
	vb.vb_eType       = eType;

	return iVB;
}


// delete a vertex buffer
extern void gfxDeleteVertexBuffer( const INDEX iBindNo)
{
	// determine API
	const GfxAPIType eAPI = _pGfx->gl_eCurrentAPI;
	ASSERT( eAPI==GAT_OGL || eAPI==GAT_D3D || eAPI==GAT_NONE);

	ASSERT( iBindNo>0 && iBindNo<_avbVertexBuffers.Count());
	VertexBuffer &vb = _avbVertexBuffers[iBindNo];
	INDEX iVA;
		
	// OpenGL
	if( eAPI==GAT_OGL)
	{
		for( iVA=0; iVA<GFX_MAX_VBA; iVA++) {
			if( vb.vb_paWriteArray[iVA] == NULL) continue;
			FreeMemory( vb.vb_paWriteArray[iVA]);
			vb.vb_paWriteArray[iVA] = NULL;
			vb.vb_paReadArray[iVA]  = NULL;
		}
	}

	// Direct3D
	else if( eAPI==GAT_D3D)
	{
		// Make sure that vertex buffer is not set as current
		extern void ClearStreams(void);
		ClearStreams();
		// for PC
/*
	#if MEMORY_TRACKING
		// signal to memory tracker  
		const ULONG ulObject = (ULONG)(&vb.vb_pavbWrite[0]);
		MEMTRACK_FREE( (void*)(ulObject^0x80000000));
	#endif
*/

		for( iVA=0; iVA<GFX_MAX_VBA; iVA++) {
			if( vb.vb_paWriteArray[iVA] == NULL) continue;
			D3DRELEASE( vb.vb_pavbWrite[iVA], TRUE);
			vb.vb_pavbWrite[iVA] = NULL;
			if( vb.vb_paReadArray[iVA] == NULL) continue;
			FreeMemory( vb.vb_paReadArray[iVA]);
			vb.vb_paReadArray[iVA] = NULL;
		}

	}

	// mark that this vertex buffer has been deleted
	vb.vb_ulArrayMask = NONE;
}



// returns ID number of a vertex buffer 'portion' (position, normal, texture coord, color or weight array)
extern INDEX gfxGetVertexSubBufferID( const INDEX iBindNo, const INDEX iType)
{
	// check
	ASSERT( iType<GFX_MAX_VBA);
	ASSERT( iBindNo>0 && iBindNo<_avbVertexBuffers.Count());
	// 16 should be enough for a nice 'alignment'
	return (iBindNo*16) + iType;
}


// returns number of vertices in a vertex buffer
extern INDEX gfxGetVertexBufferSize( const INDEX iBindNo)
{
	ASSERT( iBindNo>0 && iBindNo<_avbVertexBuffers.Count());
	return _avbVertexBuffers[iBindNo].vb_ctVertices;
}


// returns number of vertices in a vertex buffer
extern ULONG gfxGetVertexBufferMask( const INDEX iBindNo, INDEX &ctTexCoordArrays)
{
	ASSERT( iBindNo>0 && iBindNo<_avbVertexBuffers.Count());
	VertexBuffer &vb = _avbVertexBuffers[iBindNo];
//안태훈 수정 시작	//(Add Tagent-space Normal Map)(0.1)
	ULONG ulMask    = vb.vb_ulArrayMask & GFX_ALL_VBM;
	ULONG ulTexMask = ((vb.vb_ulArrayMask&(~(1<<GFX_VBA_TAN))) >> GFX_VBA_TEX);
//안태훈 수정 끝	//(Add Tagent-space Normal Map)(0.1)
	INDEX iTCA = 0;
	while( ulTexMask>0) {
		iTCA++;
		ulTexMask >>=1;
	}

	ctTexCoordArrays = iTCA;
	return ulMask;
}



// OpenGL workarounds


// initialization of commond quad elements array
extern void AddQuadElements( const INDEX ctQuads)
{
	const INDEX iStart = _auwCommonQuads.Count() /6*4;
	UWORD *puwQuads = _auwCommonQuads.Push(ctQuads*6); 
	for( INDEX i=0; i<ctQuads; i++) {
		puwQuads[i*6 +0] = iStart+ i*4 +1;
		puwQuads[i*6 +1] = iStart+ i*4 +2;
		puwQuads[i*6 +2] = iStart+ i*4 +0;
		puwQuads[i*6 +3] = iStart+ i*4 +0;
		puwQuads[i*6 +4] = iStart+ i*4 +2;
		puwQuads[i*6 +5] = iStart+ i*4 +3;
	}
}


// helper function for flushers
static void FlushArrays( UWORD *puwElements, INDEX ctElements)
{
	// check
	const INDEX ctVertices = _avtxCommon.Count();
	ASSERT( _atexCommon[0].Count()==ctVertices);
	ASSERT( _acolCommon.Count()==ctVertices);
	extern BOOL CVA_b2D;
	gfxSetVertexArray( &_avtxCommon[0], ctVertices);
	if(CVA_b2D) gfxLockArrays();
	gfxSetTexCoordArray( &_atexCommon[0][0], FALSE);
	gfxSetColorArray( &_acolCommon[0]);
	gfxDrawElements( ctElements, puwElements);
	gfxUnlockArrays();
}

// yjpark |<--
static void FlushTextArrays( UWORD *puwElements, INDEX ctElements, INDEX iBuffer )
{
	// check
	const INDEX ctVertices = _avtxText[iBuffer].Count();
	ASSERT( _atexText[iBuffer].Count() == ctVertices );
	ASSERT( _acolText[iBuffer].Count() == ctVertices );
	extern BOOL CVA_b2D;
	gfxSetVertexArray( &_avtxText[iBuffer][0], ctVertices );
	if( CVA_b2D ) gfxLockArrays();
	gfxSetTexCoordArray( &_atexText[iBuffer][0], FALSE );
	gfxSetColorArray( &_acolText[iBuffer][0] );
	gfxDrawElements( ctElements, puwElements );
	gfxUnlockArrays();
}

static void FlushBtnArrays( UWORD *puwElements, INDEX ctElements, INDEX iBuffer )
{
	// check
	const INDEX ctVertices = _avtxBtn[iBuffer].Count();
	ASSERT( _atexBtn[iBuffer].Count() == ctVertices );
	ASSERT( _acolBtn[iBuffer].Count() == ctVertices );
	extern BOOL CVA_b2D;
	gfxSetVertexArray( &_avtxBtn[iBuffer][0], ctVertices );
	if( CVA_b2D ) gfxLockArrays();
	gfxSetTexCoordArray( &_atexBtn[iBuffer][0], FALSE );
	gfxSetColorArray( &_acolBtn[iBuffer][0] );
	gfxDrawElements( ctElements, puwElements );
	gfxUnlockArrays();
}
// yjpark     -->|


// render quad elements to screen buffer
extern void gfxFlushQuads(void)
{
	// if there is something to draw
	const INDEX ctElements = _avtxCommon.Count()*6/4;
	if( ctElements<=0) return;

	// draw thru arrays or elements?
	extern INDEX ogl_bAllowQuadArrays;
	extern INDEX xbx_bAllowQuadArrays;
	if( (_pGfx->gl_eCurrentAPI==GAT_OGL && ogl_bAllowQuadArrays) 
		) {
		// OpenGL supports quads
		FlushArrays( NULL, _avtxCommon.Count());  
	} else {
		// make sure that enough quad elements has been initialized
		const INDEX ctQuads = _auwCommonQuads.Count();
		if( ctElements>ctQuads) AddQuadElements( ctElements-ctQuads); // yes, 4 times more!
		FlushArrays( &_auwCommonQuads[0], ctElements);
	}
}
 

// render elements to screen buffer
extern void gfxFlushElements(void)
{
	const INDEX ctElements = _auwCommonElements.Count();
	if( ctElements>0) FlushArrays( &_auwCommonElements[0], ctElements);
}

// yjpark |<--
extern void gfxFlushTextElements( INDEX ctElements, INDEX iBuffer )
{
	FlushTextArrays( &_auwText[iBuffer][0], ctElements, iBuffer );
}

extern void gfxFlushBtnElements( INDEX ctElements, INDEX iBuffer )
{
	FlushBtnArrays( &_auwBtn[iBuffer][0], ctElements, iBuffer );
}
// yjpark     -->|



// set truform parameters
extern void gfxSetTruform( INDEX iLevel, BOOL bLinearNormals)
{
	// skip if Truform isn't supported
	if( _pGfx->gl_iMaxTessellationLevel<1) {
		truform_iLevel  = 0;
		truform_bLinear = FALSE;
		return;
	}
	// skip if same as last time
	iLevel = Clamp( iLevel, 0L, _pGfx->gl_iMaxTessellationLevel);
	if( truform_iLevel==iLevel && !truform_bLinear==!bLinearNormals) return;

	// determine API
	const GfxAPIType eAPI = _pGfx->gl_eCurrentAPI;
	ASSERT( eAPI==GAT_OGL || eAPI==GAT_D3D || eAPI==GAT_NONE);

	_sfStats.StartTimer(CStatForm::STI_GFXAPI);

	// OpenGL needs truform set here
	if( eAPI==GAT_OGL) { 
		GLenum eTriMode = bLinearNormals ? GL_PN_TRIANGLES_NORMAL_MODE_LINEAR_ATI : GL_PN_TRIANGLES_NORMAL_MODE_QUADRATIC_ATI;
		pglPNTrianglesiATI( GL_PN_TRIANGLES_TESSELATION_LEVEL_ATI, iLevel);
		pglPNTrianglesiATI( GL_PN_TRIANGLES_NORMAL_MODE_ATI, eTriMode);
		OGL_CHECKERROR;
	}
	// if disabled, Direct3D will set tessellation level at "enable" call
	else if( eAPI==GAT_D3D && GFX_bTruform) { 
		FLOAT fSegments = iLevel+1;
		HRESULT hr = _pGfx->gl_pd3dDevice->SetRenderState( D3DRS_PATCHSEGMENTS, *((DWORD*)&fSegments));
		D3D_CHECKERROR(hr);
	}

	// keep current truform params
	truform_iLevel  = iLevel;
	truform_bLinear = bLinearNormals;

	_sfStats.StopTimer(CStatForm::STI_GFXAPI);
}



// readout current colormask
extern ULONG gfxGetColorMask(void)
{
	return GFX_ulCurrentColorMask;
}


extern void gfxEnableVertexProgram(void)
{
	GFX_bUseVertexProgram = TRUE;
}


extern void gfxDisableVertexProgram(void)
{
	GFX_bUseVertexProgram = FALSE;
}


extern void gfxEnablePixelProgram(void)
{
	GFX_bUsePixelProgram = TRUE;
}


extern void gfxDisablePixelProgram(void)
{
	GFX_bUsePixelProgram = FALSE;
}



// OCCLUSSION CULLING


extern UINT  *_puiOcclusionQueryIDs;
extern SLONG *_pslOcclusionQueryFrames;
extern ULONG *_pulOcclusionQueryOwners;
extern INDEX _ctUsedOcclusionQueries;    
static INDEX _iLastCleanupFrame = 0;
static INDEX _iCurrentOccQuery  = 0;


// initiate pixel counting
BOOL gfxBeginOcclusionTest( ULONG ulQueryID)
{
	// determine API
	const GfxAPIType eAPI = _pGfx->gl_eCurrentAPI;
	ASSERT( eAPI==GAT_OGL || eAPI==GAT_D3D || eAPI==GAT_NONE);

	// skip if not supported
		if( (eAPI==GAT_OGL && !(_pGfx->gl_ulFlags&GLF_EXT_OCCLUSIONQUERY))
		 || (eAPI==GAT_D3D)) return FALSE;

	// get last if available
	if( _ctUsedOcclusionQueries < GFX_MAXOCCQUERIES) {
		_iCurrentOccQuery = _ctUsedOcclusionQueries; 
		_ctUsedOcclusionQueries++;
	} // find space
	else {
		_iCurrentOccQuery = FindLong( 0, (ULONG*)_pslOcclusionQueryFrames, GFX_MAXOCCQUERIES);
		if( _iCurrentOccQuery==GFX_MAXOCCQUERIES) return FALSE;  // what, no free queries ? :(
	}

	_sfStats.StartTimer(CStatForm::STI_GFXAPI);

	// OpenGL
	if( eAPI==GAT_OGL) {
		const INDEX iBindID = _puiOcclusionQueryIDs[_iCurrentOccQuery];
		pglBeginOcclusionQueryNV(iBindID);
		OGL_CHECKERROR;
	}
	// Direct3D
	else if( eAPI==GAT_D3D) {
	}

	_sfStats.StopTimer(CStatForm::STI_GFXAPI);

	// mark as used
	_pslOcclusionQueryFrames[_iCurrentOccQuery] = _pGfx->gl_iFrameNumber;
	_pulOcclusionQueryOwners[_iCurrentOccQuery] = ulQueryID;
	return TRUE;
}


// stop pixel counting
void gfxEndOcclusionTest(void)
{
	// determine API
	const GfxAPIType eAPI = _pGfx->gl_eCurrentAPI;
	ASSERT( eAPI==GAT_OGL || eAPI==GAT_D3D || eAPI==GAT_NONE);

	// skip if not supported
		if( (eAPI==GAT_OGL && !(_pGfx->gl_ulFlags&GLF_EXT_OCCLUSIONQUERY))
		|| (eAPI==GAT_D3D)) return;

	ASSERT( _iCurrentOccQuery>=0 && _iCurrentOccQuery<GFX_MAXOCCQUERIES);
	_sfStats.StartTimer(CStatForm::STI_GFXAPI);

	// OpenGL
	if( eAPI==GAT_OGL) {
		pglEndOcclusionQueryNV();
		OGL_CHECKERROR;
	}
	// Direct3D
	else if( eAPI==GAT_D3D) {
	}

	_sfStats.StopTimer(CStatForm::STI_GFXAPI);
	_iCurrentOccQuery = -1;
}


// fetch pixel counter
BOOL gfxPullOcclusionQuery( ULONG ulQueryID, SLONG &slPixCount, BOOL bDontWait/*=FALSE*/)
{
	// determine API
	const GfxAPIType eAPI = _pGfx->gl_eCurrentAPI;
	ASSERT( eAPI==GAT_OGL || eAPI==GAT_D3D || eAPI==GAT_NONE);

	// skip if not supported
		if( (eAPI==GAT_OGL && !(_pGfx->gl_ulFlags&GLF_EXT_OCCLUSIONQUERY))
			|| (eAPI==GAT_D3D)) return TRUE;

	// skip if no queries are pending
	ASSERT( _ctUsedOcclusionQueries>=0 && _ctUsedOcclusionQueries<=GFX_MAXOCCQUERIES);
	if( _ctUsedOcclusionQueries<=0) {
		_ctUsedOcclusionQueries = 0;
		return TRUE;
	}

	// find owner (if not found or not used, just return - DON'T update visible pixel count)
	const INDEX iQueryID = FindLong( ulQueryID, _pulOcclusionQueryOwners, _ctUsedOcclusionQueries);
	if( iQueryID==_ctUsedOcclusionQueries) return TRUE;  
	const INDEX iBindID = _puiOcclusionQueryIDs[iQueryID];

	_sfStats.StartTimer(CStatForm::STI_GFXAPI);

	// OpenGL
	if( eAPI==GAT_OGL) {
		if( bDontWait) {
			GLint iRes;
			pglGetOcclusionQueryivNV( iBindID, GL_PIXEL_COUNT_AVAILABLE_NV, &iRes);
			OGL_CHECKERROR;
			if( iRes==0) {
				_sfStats.StopTimer(CStatForm::STI_GFXAPI);
				return FALSE;
			}
		} // readout!
		pglGetOcclusionQueryivNV( iBindID, GL_PIXEL_COUNT_NV, (GLint*)&slPixCount);
		OGL_CHECKERROR;
	}
	// Direct3D
	else if( eAPI==GAT_D3D) {
	}

	_sfStats.StopTimer(CStatForm::STI_GFXAPI);

	// if this wasn't the last query
	const INDEX iLastUsedQuery = _ctUsedOcclusionQueries-1;
	if( iQueryID < iLastUsedQuery) {
		// move the last to this place
		_pslOcclusionQueryFrames[iQueryID] = _pslOcclusionQueryFrames[iLastUsedQuery];
		_pulOcclusionQueryOwners[iQueryID] = _pulOcclusionQueryOwners[iLastUsedQuery];
		_pslOcclusionQueryFrames[iLastUsedQuery] = 0;
		_pulOcclusionQueryOwners[iLastUsedQuery] = NONE;
		// swap occlusion query bind numbers
		Swap( _puiOcclusionQueryIDs[iQueryID], _puiOcclusionQueryIDs[iLastUsedQuery]);
	} else {
		// just mark as free
		_pslOcclusionQueryFrames[iQueryID] = 0;
		_pulOcclusionQueryOwners[iQueryID] = NONE;
	}

	// decrease number of occlusion queries in use
	ASSERT(_ctUsedOcclusionQueries>0);
	do { _ctUsedOcclusionQueries--;
	} while( _pslOcclusionQueryFrames[_ctUsedOcclusionQueries-1]==0 && _ctUsedOcclusionQueries>0);
	return TRUE;
}


// free occlusion query
extern void gfxFreeOcclusionQuery( ULONG ulQueryID)
{ 
	// skip if memory has been freed or no queries are in use
	if( _pulOcclusionQueryOwners==NULL || _ctUsedOcclusionQueries==0) return; 

	// find owner
	ASSERT( _ctUsedOcclusionQueries>0 && _ctUsedOcclusionQueries<=GFX_MAXOCCQUERIES);
	const INDEX iQueryID = FindLong( ulQueryID, _pulOcclusionQueryOwners, _ctUsedOcclusionQueries);
	if( iQueryID==_ctUsedOcclusionQueries) return; // done here if not found

	// move the last to this place
	const INDEX iLastUsedQuery = _ctUsedOcclusionQueries-1;
	_pslOcclusionQueryFrames[iQueryID] = _pslOcclusionQueryFrames[iLastUsedQuery];
	_pulOcclusionQueryOwners[iQueryID] = _pulOcclusionQueryOwners[iLastUsedQuery];
	_pslOcclusionQueryFrames[iLastUsedQuery] = 0;
	_pulOcclusionQueryOwners[iLastUsedQuery] = NONE;
	// swap occlusion query bind numbers
	Swap( _puiOcclusionQueryIDs[iQueryID], _puiOcclusionQueryIDs[iLastUsedQuery]);

	// decrease number of occlusion queries in use
	do { _ctUsedOcclusionQueries--;
	} while( _pslOcclusionQueryFrames[_ctUsedOcclusionQueries-1]==0 && _ctUsedOcclusionQueries>0);
}


// remove unused occlusion queries
extern void RemoveOldOcclusionQueries(void)
{
	// only if occlusion culling is used
	if( gap_bUseOcclusionCulling==0) return;
	ASSERT( _pslOcclusionQueryFrames!=NULL);

	// no need for cleanup yet?
	SLONG slFramesElapsed = _pGfx->gl_iFrameNumber - _iLastCleanupFrame;
	if( slFramesElapsed<(GFX_MAXOCCQUERIES/10) && slFramesElapsed>0) return;

	// clear old queries
	ASSERT( _ctUsedOcclusionQueries>=0 && _ctUsedOcclusionQueries<=GFX_MAXOCCQUERIES);
	{for( INDEX iQuery=0; iQuery<_ctUsedOcclusionQueries; iQuery++) { 
		// skip unused query
		SLONG &slQueryFrame = _pslOcclusionQueryFrames[iQuery];
		ULONG &ulQueryOwner = _pulOcclusionQueryOwners[iQuery];
		if( slQueryFrame==0) {
			ASSERT( ulQueryOwner==NONE);
			continue; 
		}
		// get frame difference
		slFramesElapsed = _pGfx->gl_iFrameNumber - slQueryFrame;

		// free query if too old
		if( slFramesElapsed>(GFX_MAXOCCQUERIES/5) || slFramesElapsed<0) {
			slQueryFrame = 0;
			ulQueryOwner = NONE;
		} 
	}}

#ifndef NDEBUG
	// paranoia check (all queries after last used must be free)
	{for( INDEX iQuery=_ctUsedOcclusionQueries; iQuery<GFX_MAXOCCQUERIES; iQuery++) { 
		ASSERT( _pslOcclusionQueryFrames[iQuery]==0 && _pulOcclusionQueryOwners[iQuery]==0);
	}}
#endif

	// fill in empty queries with those from the end
	INDEX iStartQuery = 0; 
	INDEX iLastQuery = _ctUsedOcclusionQueries-1;
	_ctUsedOcclusionQueries = 0;   // assume no queries
	while( iStartQuery<=iLastQuery) {
		// if this query is unused
		if( _pslOcclusionQueryFrames[iStartQuery]==0) {
			// find last used query
			while( _pslOcclusionQueryFrames[iLastQuery]==0 && iLastQuery>0) iLastQuery--;
			if( iStartQuery>=iLastQuery) break; // reached the end of 'holes' ?
			// move last to empty
			_pslOcclusionQueryFrames[iStartQuery] = _pslOcclusionQueryFrames[iLastQuery];
			_pulOcclusionQueryOwners[iStartQuery] = _pulOcclusionQueryOwners[iLastQuery];
			_pslOcclusionQueryFrames[iLastQuery] = 0;
			_pulOcclusionQueryOwners[iLastQuery] = NONE;
			// swap occlusion query bind numbers if used
			Swap( _puiOcclusionQueryIDs[iStartQuery], _puiOcclusionQueryIDs[iLastQuery]);
		}
		// advance to next suposedly used query
		iStartQuery++;
	 _ctUsedOcclusionQueries = iStartQuery;
	}

	// done
	ASSERT( _ctUsedOcclusionQueries>=0 && _ctUsedOcclusionQueries<=GFX_MAXOCCQUERIES);
	_iLastCleanupFrame = _pGfx->gl_iFrameNumber;
}



// wrapper for blending types

extern void gfxSetBlendType( const enum PredefinedBlendType pbt)
{
	switch(pbt) {
	case PBT_OPAQUE:
		gfxEnableDepthWrite();
		gfxDisableAlphaTest();
		gfxDisableBlend();
		break;
	case PBT_TRANSPARENT:
		gfxEnableDepthWrite();
		gfxEnableAlphaTest();
		gfxDisableBlend();
		break;
	case PBT_BLEND:
		gfxDisableDepthWrite();
		gfxDisableAlphaTest();
		gfxEnableBlend();
		gfxBlendFunc( GFX_SRC_ALPHA, GFX_INV_SRC_ALPHA);
		break;
	case PBT_SHADE:
		gfxDisableDepthWrite();
		gfxDisableAlphaTest();
		gfxEnableBlend();
		gfxBlendFunc( GFX_DST_COLOR, GFX_SRC_COLOR);
		break;
	case PBT_ADD:
		gfxDisableDepthWrite();
		gfxDisableAlphaTest();
		gfxEnableBlend();
		gfxBlendFunc( GFX_ONE, GFX_ONE);
		break;
	case PBT_ADDALPHA:
		gfxDisableDepthWrite();
		gfxDisableAlphaTest();
		gfxEnableBlend();
		gfxBlendFunc( GFX_SRC_ALPHA, GFX_ONE);
		break;
	case PBT_MULTIPLY:
		gfxDisableDepthWrite();
		gfxDisableAlphaTest();
		gfxEnableBlend();
		gfxBlendFunc( GFX_DST_COLOR, GFX_ZERO);
		break;
	case PBT_INVMULTIPLY:
		gfxDisableDepthWrite();
		gfxDisableAlphaTest();
		gfxEnableBlend();
		gfxBlendFunc( GFX_ZERO, GFX_INV_SRC_COLOR);
		break;
	default:
		ASSERTALWAYS( "Unsupported predefined blending type!");
		break;
	}
}



#include "GFX_wrapper_OpenGL.cpp"

#include "GFX_wrapper_Direct3D.cpp"



// DUMMY FUNCTIONS FOR NONE API
static void none_BlendFunc( GfxBlend eSrc, GfxBlend eDst) { NOTHING; }
static void none_DepthFunc( GfxComp eFunc) { NOTHING; };
static void none_DepthRange( FLOAT fMin, FLOAT fMax) { NOTHING; };
static void none_CullFace( GfxFace eFace) { NOTHING; };
static void none_ClipPlane( const DOUBLE *pdViewPlane) { NOTHING; };
static void none_SetOrtho(   FLOAT fLeft, FLOAT fRight, FLOAT fTop, FLOAT fBottom, FLOAT fNear, FLOAT fFar, const BOOL bSubPixelAdjust) { NOTHING; };
static void none_SetFrustum( FLOAT fLeft, FLOAT fRight, FLOAT fTop, FLOAT fBottom, FLOAT fNear, FLOAT fFar) { NOTHING; };
static void none_SetMatrix( const FLOAT *pfMatrix) { NOTHING; };
static void none_PolygonMode( GfxPolyMode ePolyMode) { NOTHING; };
static void none_SetLightColor( INDEX iLight, COLOR colLight, COLOR colAmbient, COLOR colSpecular) { NOTHING; };
static void none_SetLightDirection( INDEX iLight, const FLOAT3D &vLightDir) { NOTHING; };
static void none_SetTextureWrapping( enum GfxWrap eWrapU, enum GfxWrap eWrapV) { NOTHING; };
static void none_GenDelTexture( ULONG &ulTexObject) { NOTHING; };
static void none_SetVertexArray( GFXVertex *pvtx, INDEX ctVtx) { NOTHING; };
static void none_SetNormalArray( GFXNormal *pnor) { NOTHING; };
static void none_SetWeightArray( GFXWeight *pwgh) { NOTHING; };
static void none_SetTexCoordArray( GFXTexCoord *ptex, BOOL bProjectiveMapping) { NOTHING; };
static void none_SetColorArray( GFXColor *pcol) { NOTHING; };
static void none_DrawElements( const INDEX ctIndices, const UWORD *puwIndices) { NOTHING; };
static void none_SetConstantColor( COLOR col) { NOTHING; };
static void none_SetColorMask( ULONG ulColorMask) { NOTHING; };
static void none_Finish( BOOL bReadBack) { NOTHING; };
static void* none_LockSubBuffer(   const INDEX iID, const INDEX i1stVertex, const INDEX ctVertices, const enum GfxLockType eLockType) { return NULL; };
static void  none_UnlockSubBuffer( const INDEX iID, const INDEX ctVertices) { NOTHING; };
static void  none_SetSubBuffer( const INDEX iID, const INDEX iUnit) { NOTHING; };
static void  none_SetVertexSubBuffer( const INDEX iID, const INDEX i1stVertex, const INDEX ctVertices) { NOTHING; };
static void  none_SetConstantRegister( INDEX iRegister, const void *pData, INDEX ctRegisters) { NOTHING; };
static ULONG none_CreateVertexProgram( const char *strVertexProgram, ULONG ulStreamFlags) { return NONE; };
static ULONG none_CreatePixelProgram(  const char *strPixelProgram) { return NONE; };
static void  none_SetProgramConst( INDEX iRegister, const void *pData, INDEX ctRegisters) { NOTHING; };


// functions initialization for OGL, D3D or NONE (dummy)
extern void GFX_SetFunctionPointers( INDEX iAPI)
{
	// OpenGL?
	if( iAPI==(INDEX)GAT_OGL)
	{
		gfxEnableDepthWrite     = &ogl_EnableDepthWrite;    
		gfxEnableDepthBias      = &ogl_EnableDepthBias;     
		gfxEnableDepthTest      = &ogl_EnableDepthTest;     
		gfxEnableAlphaTest      = &ogl_EnableAlphaTest;     
		gfxEnableBlend          = &ogl_EnableBlend;         
		gfxEnableDither         = &ogl_EnableDither;        
		gfxEnableTexture        = &ogl_EnableTexture;       
		gfxEnableLight          = &ogl_EnableLight;
		gfxEnableLighting       = &ogl_EnableLighting;      
		gfxEnableClipping       = &ogl_EnableClipping;      
		gfxEnableClipPlane      = &ogl_EnableClipPlane;     
		gfxEnableTruform        = &ogl_EnableTruform;       
		gfxDisableDepthWrite    = &ogl_DisableDepthWrite;   
		gfxDisableDepthBias     = &ogl_DisableDepthBias;    
		gfxDisableDepthTest     = &ogl_DisableDepthTest;    
		gfxDisableAlphaTest     = &ogl_DisableAlphaTest;    
		gfxDisableBlend         = &ogl_DisableBlend;        
		gfxDisableDither        = &ogl_DisableDither;       
		gfxDisableTexture       = &ogl_DisableTexture;      
		gfxDisableLight         = &ogl_DisableLight;
		gfxDisableLighting      = &ogl_DisableLighting;     
		gfxDisableClipping      = &ogl_DisableClipping;     
		gfxDisableClipPlane     = &ogl_DisableClipPlane;    
		gfxDisableTruform       = &ogl_DisableTruform;      
		gfxBlendFunc            = &ogl_BlendFunc;           
		gfxDepthFunc            = &ogl_DepthFunc;           
		gfxDepthRange           = &ogl_DepthRange;          
		gfxCullFace             = &ogl_CullFace;            
		gfxFrontFace            = &ogl_FrontFace;            
		gfxClipPlane            = &ogl_ClipPlane;           
		gfxSetOrtho             = &ogl_SetOrtho;            
		gfxSetFrustum           = &ogl_SetFrustum;          
		gfxSetTextureMatrix     = &ogl_SetTextureMatrix;       
		gfxSetViewMatrix        = &ogl_SetViewMatrix;       
		gfxPolygonMode          = &ogl_PolygonMode;         
		gfxSetLightColor        = &ogl_SetLightColor;
		gfxSetLightDirection    = &ogl_SetLightDirection;
		gfxSetTextureWrapping   = &ogl_SetTextureWrapping;  
		gfxSetTextureModulation = &ogl_SetTextureModulation;
		gfxGenerateTexture      = &ogl_GenerateTexture;     
		gfxDeleteTexture        = &ogl_DeleteTexture;       
		gfxSetVertexArray       = &ogl_SetVertexArray;      
		gfxSetNormalArray       = &ogl_SetNormalArray;      
		gfxSetWeightArray       = &ogl_SetWeightArray;      
		gfxSetTexCoordArray     = &ogl_SetTexCoordArray;    
		gfxSetColorArray        = &ogl_SetColorArray;       
		gfxDrawElements         = &ogl_DrawElements;        
		gfxSetConstantColor     = &ogl_SetConstantColor;    
		gfxEnableColorArray     = &ogl_EnableColorArray;    
		gfxDisableColorArray    = &ogl_DisableColorArray;   
		gfxFinish               = &ogl_Finish;              
		gfxLockArrays           = &ogl_LockArrays;          
		gfxSetColorMask         = &ogl_SetColorMask;
		gfxLockSubBuffer        = &ogl_LockSubBuffer;
		gfxUnlockSubBuffer      = &ogl_UnlockSubBuffer;
		gfxSetSubBuffer         = &ogl_SetSubBuffer;
		gfxSetVertexSubBuffer   = &ogl_SetVertexSubBuffer;

		gfxSetVertexProgram     = &ogl_SetVertexProgram;
		gfxSetPixelProgram      = &ogl_SetPixelProgram;
		gfxDeleteVertexProgram  = &ogl_DeleteVertexProgram;
		gfxDeletePixelProgram   = &ogl_DeletePixelProgram;
		gfxCreateVertexProgram  = &ogl_CreateVertexProgram;
		gfxCreatePixelProgram   = &ogl_CreatePixelProgram;
		gfxSetVertexProgramConst= &ogl_SetVertexProgramConst;
		gfxSetPixelProgramConst = &ogl_SetPixelProgramConst;

	}
	// Direct3D?
	else if( iAPI==(INDEX)GAT_D3D)
	{
		gfxEnableDepthWrite     = &d3d_EnableDepthWrite;
		gfxEnableDepthBias      = &d3d_EnableDepthBias;
		gfxEnableDepthTest      = &d3d_EnableDepthTest;
		gfxEnableAlphaTest      = &d3d_EnableAlphaTest;
		gfxEnableBlend          = &d3d_EnableBlend;
		gfxEnableDither         = &d3d_EnableDither;
		gfxEnableTexture        = &d3d_EnableTexture;
		gfxEnableLight          = &d3d_EnableLight;
		gfxEnableLighting       = &d3d_EnableLighting;
		gfxEnableClipping       = &d3d_EnableClipping;
		gfxEnableClipPlane      = &d3d_EnableClipPlane;
		gfxEnableTruform        = &d3d_EnableTruform;
		gfxDisableDepthWrite    = &d3d_DisableDepthWrite;
		gfxDisableDepthBias     = &d3d_DisableDepthBias;
		gfxDisableDepthTest     = &d3d_DisableDepthTest;
		gfxDisableAlphaTest     = &d3d_DisableAlphaTest;
		gfxDisableBlend         = &d3d_DisableBlend;
		gfxDisableDither        = &d3d_DisableDither;
		gfxDisableTexture       = &d3d_DisableTexture;
		gfxDisableLight         = &d3d_DisableLight;
		gfxDisableLighting      = &d3d_DisableLighting;
		gfxDisableClipping      = &d3d_DisableClipping;
		gfxDisableClipPlane     = &d3d_DisableClipPlane;
		gfxDisableTruform       = &d3d_DisableTruform;
		gfxBlendFunc            = &d3d_BlendFunc;
		gfxDepthFunc            = &d3d_DepthFunc;
		gfxDepthRange           = &d3d_DepthRange;
		gfxCullFace             = &d3d_CullFace;
		gfxFrontFace            = &d3d_FrontFace;            
		gfxClipPlane            = &d3d_ClipPlane;
		gfxSetOrtho             = &d3d_SetOrtho;
		gfxSetFrustum           = &d3d_SetFrustum;
		gfxSetTextureMatrix     = &d3d_SetTextureMatrix;       
		gfxSetViewMatrix        = &d3d_SetViewMatrix;
		gfxPolygonMode          = &d3d_PolygonMode;
		gfxSetLightColor        = &d3d_SetLightColor;
		gfxSetLightDirection    = &d3d_SetLightDirection;
		gfxSetTextureWrapping   = &d3d_SetTextureWrapping;
		gfxSetTextureModulation = &d3d_SetTextureModulation;
		gfxGenerateTexture      = &d3d_GenerateTexture;
		gfxDeleteTexture        = &d3d_DeleteTexture;   
		gfxSetVertexArray       = &d3d_SetVertexArray;  
		gfxSetNormalArray       = &d3d_SetNormalArray;  
		gfxSetWeightArray       = &d3d_SetWeightArray;  
		gfxSetTexCoordArray     = &d3d_SetTexCoordArray;
		gfxSetColorArray        = &d3d_SetColorArray;   
		gfxDrawElements         = &d3d_DrawElements;    
		gfxSetConstantColor     = &d3d_SetConstantColor;
		gfxEnableColorArray     = &d3d_EnableColorArray;
		gfxDisableColorArray    = &d3d_DisableColorArray;
		gfxFinish               = &d3d_Finish;
		gfxLockArrays           = &d3d_LockArrays;
		gfxSetColorMask         = &d3d_SetColorMask;
		gfxLockSubBuffer        = &d3d_LockSubBuffer;
		gfxUnlockSubBuffer      = &d3d_UnlockSubBuffer;
		gfxSetSubBuffer         = &d3d_SetSubBuffer;
		gfxSetVertexSubBuffer   = &d3d_SetVertexSubBuffer;
	
		gfxSetVertexProgram     = &d3d_SetVertexProgram;
		gfxSetPixelProgram      = &d3d_SetPixelProgram;
		gfxDeleteVertexProgram  = &d3d_DeleteVertexProgram;
		gfxDeletePixelProgram   = &d3d_DeletePixelProgram;
		gfxCreateVertexProgram  = &d3d_CreateVertexProgram;
		gfxCreatePixelProgram   = &d3d_CreatePixelProgram;
		gfxSetVertexProgramConst= &d3d_SetVertexProgramConst;
		gfxSetPixelProgramConst = &d3d_SetPixelProgramConst;

	}
	// NONE!
	else
	{
		gfxEnableDepthWrite     = &none_void;
		gfxEnableDepthBias      = &none_void;
		gfxEnableDepthTest      = &none_void;
		gfxEnableAlphaTest      = &none_void;
		gfxEnableBlend          = &none_void;
		gfxEnableDither         = &none_void;
		gfxEnableTexture        = &none_void;
		gfxEnableLight          = &none_int;
		gfxEnableLighting       = &none_void;
		gfxEnableClipping       = &none_void;
		gfxEnableClipPlane      = &none_void;
		gfxEnableTruform        = &none_void;
		gfxDisableDepthWrite    = &none_void;
		gfxDisableDepthBias     = &none_void;
		gfxDisableDepthTest     = &none_void;
		gfxDisableAlphaTest     = &none_void;
		gfxDisableBlend         = &none_void;
		gfxDisableDither        = &none_void;
		gfxDisableTexture       = &none_void;
		gfxDisableLight         = &none_int;
		gfxDisableLighting      = &none_void;
		gfxDisableClipping      = &none_void;
		gfxDisableClipPlane     = &none_void;
		gfxDisableTruform       = &none_void;
		gfxBlendFunc            = &none_BlendFunc;
		gfxDepthFunc            = &none_DepthFunc;
		gfxDepthRange           = &none_DepthRange;
		gfxCullFace             = &none_CullFace;
		gfxFrontFace            = &none_CullFace;
		gfxClipPlane            = &none_ClipPlane;
		gfxSetOrtho             = &none_SetOrtho;
		gfxSetFrustum           = &none_SetFrustum;
		gfxSetTextureMatrix     = &none_SetMatrix;
		gfxSetViewMatrix        = &none_SetMatrix;
		gfxPolygonMode          = &none_PolygonMode;
		gfxSetLightColor        = &none_SetLightColor;
		gfxSetLightDirection    = &none_SetLightDirection;
		gfxSetTextureWrapping   = &none_SetTextureWrapping;
		gfxSetTextureModulation = &none_int;
		gfxGenerateTexture      = &none_GenDelTexture;
		gfxDeleteTexture        = &none_GenDelTexture;   
		gfxSetVertexArray       = &none_SetVertexArray;  
		gfxSetNormalArray       = &none_SetNormalArray;  
		gfxSetWeightArray       = &none_SetWeightArray;  
		gfxSetTexCoordArray     = &none_SetTexCoordArray;
		gfxSetColorArray        = &none_SetColorArray;   
		gfxDrawElements         = &none_DrawElements;    
		gfxSetConstantColor     = &none_SetConstantColor;
		gfxEnableColorArray     = &none_void;
		gfxDisableColorArray    = &none_void;
		gfxFinish               = &none_Finish;
		gfxLockArrays           = &none_void;
		gfxSetColorMask         = &none_SetColorMask;
		gfxLockSubBuffer        = &none_LockSubBuffer;
		gfxUnlockSubBuffer      = &none_UnlockSubBuffer;
		gfxSetSubBuffer         = &none_SetSubBuffer;
		gfxSetVertexSubBuffer   = &none_SetVertexSubBuffer;
	
		gfxSetVertexProgram     = &none_const_ulong;
		gfxSetPixelProgram      = &none_const_ulong;
		gfxDeleteVertexProgram  = &none_ulong;
		gfxDeletePixelProgram   = &none_ulong;
		gfxCreateVertexProgram  = &none_CreateVertexProgram;
		gfxCreatePixelProgram   = &none_CreatePixelProgram;
		gfxSetVertexProgramConst= &none_SetProgramConst;
		gfxSetPixelProgramConst = &none_SetProgramConst;
	}
}
