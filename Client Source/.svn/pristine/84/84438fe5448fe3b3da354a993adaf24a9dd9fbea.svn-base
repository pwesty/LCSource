#pragma once


// enums for everything

enum GfxBlend
{
	GFX_ONE           = 21,
	GFX_ZERO          = 22,
	GFX_SRC_COLOR     = 23,
	GFX_INV_SRC_COLOR = 24,
	GFX_DST_COLOR     = 25,
	GFX_INV_DST_COLOR = 26,
	GFX_SRC_ALPHA     = 27,
	GFX_INV_SRC_ALPHA = 28,
	GFX_DST_ALPHA	  = 29,
	GFX_INV_DST_ALPHA = 30,
};

enum GfxComp
{
	GFX_NEVER         = 41,
	GFX_LESS          = 42,
	GFX_LESS_EQUAL    = 43,
	GFX_EQUAL         = 44,
	GFX_NOT_EQUAL     = 45,
	GFX_GREATER_EQUAL = 46,
	GFX_GREATER       = 47,
	GFX_ALWAYS        = 48,
};
	
enum GfxFace
{
	GFX_NONE  = 61,
	GFX_FRONT = 62,
	GFX_BACK  = 63,
	GFX_CW    = 64,
	GFX_CCW   = 65,
};

enum GfxMatrixType
{
	GFX_VIEW       = 71,
	GFX_PROJECTION = 72,
	GFX_TEXTURE    = 73,
};

enum GfxWrap
{
	GFX_REPEAT = 81,
	GFX_CLAMP  = 82,
};

enum GfxPolyMode
{
	GFX_FILL  = 91,
	GFX_LINE  = 92,
	GFX_POINT = 93,
};

#define GFX_POSITION_STREAM  (1UL<<0)
#define GFX_COLOR_STREAM     (1UL<<1)
#define GFX_TEXCOORD0        (1UL<<2)
#define GFX_TEXCOORD1        (1UL<<3)
#define GFX_TEXCOORD2        (1UL<<4)
#define GFX_TEXCOORD3        (1UL<<5)
#define GFX_NORMAL_STREAM    (1UL<<6)
#define GFX_WEIGHT_STREAM    (1UL<<7)
//안태훈 수정 시작	//(Add Tagent-space Normal Map)(0.1)
#define GFX_TANGENT_STREAM   (1UL<<8)
//안태훈 수정 끝	//(Add Tagent-space Normal Map)(0.1)



// vertex array lock type
enum GfxLockType
{
	GFX_READ  = 101,
	GFX_WRITE = 102,
	GFX_READWRITE = 103,
	GFX_WRITEONLY = 104,
	GFX_DYNAMIC   = 105, // implies write only
};



// streams' indices of vertex components
#define GFX_POSIDX (0)
#define GFX_COLIDX (1)
#define GFX_TEXIDX (2)
#define GFX_NORIDX (6)
#define GFX_WGHIDX (7)
//안태훈 수정 시작	//(Add Tagent-space Normal Map)(0.1)
#define GFX_TANIDX (8)
#define GFX_REAL_TANIDX (3)
//안태훈 수정 끝	//(Add Tagent-space Normal Map)(0.1)

// vertex array index and mask
#define GFX_VBA_POS 0
#define GFX_VBA_NOR 1
#define GFX_VBA_WGH 2
#define GFX_VBA_COL 3
#define GFX_VBA_TEX 4
//안태훈 수정 시작	//(Add Tagent-space Normal Map)(0.1)
#define GFX_VBA_TAN 12
#define GFX_MAX_VBA 13  // 8 texture coords
//안태훈 수정 끝	//(Add Tagent-space Normal Map)(0.1)

#define GFX_VBM_POS (1L<<GFX_VBA_POS)
#define GFX_VBM_NOR (1L<<GFX_VBA_NOR)
#define GFX_VBM_WGH (1L<<GFX_VBA_WGH)
#define GFX_VBM_COL (1L<<GFX_VBA_COL)
#define GFX_VBM_TEX (1L<<GFX_VBA_TEX)
//안태훈 수정 시작	//(Add Tagent-space Normal Map)(0.1)
#define GFX_VBM_TAN (1L<<GFX_VBA_TAN)
#define GFX_MAX_VBM (1L<<6)
//안태훈 수정 끝	//(Add Tagent-space Normal Map)(0.1)

//안태훈 수정 시작	//(Add Tagent-space Normal Map)(0.1)
#define GFX_ALL_VBM ((GFX_MAX_VBM-1)|GFX_VBM_TAN)
//안태훈 수정 끝	//(Add Tagent-space Normal Map)(0.1)


// sizes
#define GFX_POSSIZE (3*sizeof(FLOAT))
#define GFX_NORSIZE (3*sizeof(FLOAT))
#define GFX_TEXSIZE (2*sizeof(FLOAT))
#define GFX_TX4SIZE (4*sizeof(FLOAT))
#define GFX_COLSIZE (4*sizeof(UBYTE))
#define GFX_WGHSIZE (4*sizeof(UBYTE)*2)
#define GFX_IDXSIZE (1*sizeof(UWORD))
//안태훈 수정 시작	//(Add Tagent-space Normal Map)(0.1)
#define GFX_TANSIZE (4*sizeof(FLOAT))
//안태훈 수정 끝	//(Add Tagent-space Normal Map)(0.1)

// functions initialization for OGL, D3D or NONE (dummy)
extern void GFX_SetFunctionPointers( INDEX iAPI);


// enable operations
extern void (*gfxEnableDepthWrite)(void);
extern void (*gfxEnableDepthBias)(void);
extern void (*gfxEnableDepthTest)(void);
extern void (*gfxEnableAlphaTest)(void);
extern void (*gfxEnableBlend)(void);
extern void (*gfxEnableDither)(void);
extern void (*gfxEnableTexture)(void);
extern void (*gfxEnableLighting)(void);
extern void (*gfxEnableClipping)(void);
extern void (*gfxEnableClipPlane)(void);

// disable operations
extern void (*gfxDisableDepthWrite)(void);
extern void (*gfxDisableDepthBias)(void);
extern void (*gfxDisableDepthTest)(void);
extern void (*gfxDisableAlphaTest)(void);
extern void (*gfxDisableBlend)(void);
extern void (*gfxDisableDither)(void);
extern void (*gfxDisableTexture)(void);
extern void (*gfxDisableLighting)(void);
extern void (*gfxDisableClipping)(void);
extern void (*gfxDisableClipPlane)(void);

// set blending operations
extern void (*gfxBlendFunc)( GfxBlend eSrc, GfxBlend eDst);

// set depth buffer compare mode
extern void (*gfxDepthFunc)( GfxComp eFunc);
		
// set depth buffer range
extern void (*gfxDepthRange)( FLOAT fMin, FLOAT fMax);

// color mask control (use CT_RMASK, CT_GMASK, CT_BMASK, CT_AMASK to enable specific channels)
extern void (*gfxSetColorMask)( ULONG ulColorMask);
extern ULONG gfxGetColorMask(void);



// PROJECTIONS


// set face culling
extern void (*gfxCullFace)(  GfxFace eFace);
extern void (*gfxFrontFace)( GfxFace eFace);

// set custom clip plane (if NULL, disable it)
extern void (*gfxClipPlane)( const DOUBLE *pdPlane);

// set view matrix 
extern void (*gfxSetViewMatrix)( const FLOAT *pfMatrix/*=NULL*/);
// set texture matrix
extern void (*gfxSetTextureMatrix)( const FLOAT *pfMatrix/*=NULL*/);
// set frustrum matrix
extern void (*gfxSetFrustum)( FLOAT fLeft, FLOAT fRight, FLOAT fTop, FLOAT fBottom, FLOAT fNear, FLOAT fFar);
// set orthographic matrix
extern void (*gfxSetOrtho)( FLOAT fLeft, FLOAT fRight, FLOAT fTop, FLOAT fBottom, FLOAT fNear, FLOAT fFar,
														const BOOL bSubPixelAdjust/*=FALSE*/);

// polygon mode (point, line or fill)
extern void (*gfxPolygonMode)( GfxPolyMode ePolyMode);



// LIGHTS


// enables/disables usage of light in lighting calculations
extern void (*gfxEnableLight)(  INDEX iLight);
extern void (*gfxDisableLight)( INDEX iLight);

// set color components of a light
extern void (*gfxSetLightColor)( INDEX iLight, COLOR colLight, COLOR colAmbient, COLOR colSpecular/*=0*/);

// set light direction (only directional light are supported for now - they're the fastest!)
extern void (*gfxSetLightDirection)( INDEX iLight, const FLOAT3D &vLightDir);



// OCCLUSION


// begin/end occlusion query (returns whether query is available)
// NOTE: max 500 querys can be active at one time!
extern BOOL gfxBeginOcclusionTest( ULONG ulQueryID);
extern void gfxEndOcclusionTest(void);

// fetch result of occlusion query (returns whether occlusion query result is available if "don't wait" is specified)
// NOTE: the ID gets "freed" when this function is called!
extern BOOL gfxPullOcclusionQuery( ULONG ulQueryID, SLONG &slPixCount, BOOL bDontWait=FALSE);

// just mark occlusion query as free
extern void gfxFreeOcclusionQuery( ULONG ulQueryID);



// TEXTURES


// texture settings (holds current states of texture quality, size and such)
struct TextureSettings {
public:
	//quailties
	INDEX ts_iNormQualityO;    
	INDEX ts_iNormQualityA;
	INDEX ts_iAnimQualityO;
	INDEX ts_iAnimQualityA;
	// sizes/forcing
	PIX ts_pixNormSize;
	PIX ts_pixAnimSize;
	// texture formats (set by OGL or D3D)
	ULONG ts_tfRGB8, ts_tfRGBA8;               // true color
	ULONG ts_tfRGB5, ts_tfRGBA4, ts_tfRGB5A1;  // high color
	ULONG ts_tfLA8,  ts_tfL8,    ts_tfA8;      // grayscale
	ULONG ts_tfRGBc, ts_tfRGBcA, ts_tfRGBcAc;  // compressed formats
	// maximum texel-byte ratio for largest texture size
	INDEX ts_iMaxBytesPerTexel;
};
// singleton object for texture settings
extern struct TextureSettings TS;
// routine for updating texture settings from console variable
extern void UpdateTextureSettings(void);


// texture parameters for texture state changes
class CTexParams {
public:
	INDEX tp_iFilter;            // OpenGL texture mapping mode
	INDEX tp_iAnisotropy;        // texture degree of anisotropy (>=1.0f; 1.0=isotropic, default)
	BOOL  tp_bSingleMipmap;      // texture has only one mipmap
	GfxWrap tp_eWrapU, tp_eWrapV;  // wrapping states
	inline CTexParams(void) { Clear(); tp_bSingleMipmap = FALSE; };
	inline void Clear(void) { tp_iFilter = 00; tp_iAnisotropy = 0; tp_eWrapU = tp_eWrapV = (GfxWrap)NONE; };
	inline BOOL IsEqual( CTexParams tp) { return tp_iFilter==tp.tp_iFilter && tp_iAnisotropy==tp_iAnisotropy && 
																							 tp_eWrapU==tp.tp_eWrapU && tp_eWrapV==tp.tp_eWrapV; };
};
//안태훈 수정 시작	//(Remake Effect)(0.1)
enum GFX_TEXTURE_ARG
{
	GFX_TA_DIFFUSE	= D3DTA_DIFFUSE,
	GFX_TA_CURRENT	= D3DTA_CURRENT,
	GFX_TA_TEXTURE	= D3DTA_TEXTURE,
	GFX_TA_TFACTOR	= D3DTA_TFACTOR,
	GFX_TA_SPECULAR	= D3DTA_SPECULAR,
	GFX_TA_TEMP		= D3DTA_TEMP,
	//GFX_TA_COMPLEMENT = D3DTA_COMPLEMENT,
	//GFX_TA_ALPHAREPLICATE = D3DTA_ALPHAREPLICATE,
};

//d3d의 SetTextureStageState를 wrapping한 것임. 태훈
extern void gfxSetTextureSelectArg(INDEX iStage, GFX_TEXTURE_ARG argColor, GFX_TEXTURE_ARG argAlpha);
//안태훈 수정 끝	//(Remake Effect)(0.1)

// get current texture filtering mode
extern void gfxGetTextureFiltering( INDEX &iFilterType, INDEX &iAnisotropyDegree);
// set texture filtering
extern void gfxSetTextureFiltering( INDEX &iFilterType, INDEX &iAnisotropyDegree);
// set texture LOD biasing
extern void gfxSetTextureBiasing( FLOAT &fLODBias);

// set texture wrapping mode
extern void (*gfxSetTextureWrapping)( enum GfxWrap eWrapU, enum GfxWrap eWrapV);

// set texture modulation mode (1X or 2X)
extern void (*gfxSetTextureModulation)( INDEX iScale);

// set texture unit as active
extern void gfxSetTextureUnit( INDEX iUnit);

// generate texture for API
extern void (*gfxGenerateTexture)( ULONG &ulTexObject);
// unbind texture from API
extern void (*gfxDeleteTexture)( ULONG &ulTexObject);


// set texture as current
//  - ulTexture = bind number for OGL, or *LPDIRECT3DTEXTURE8 for D3D (pointer to pointer!)
extern void gfxSetTexture( ULONG &ulTexObject, CTexParams &tpLocal);

// upload texture
// - ulTexture  = bind number for OGL, or LPDIRECT3DTEXTURE8 for D3D
// - pulTexture = pointer to texture in 32-bit R,G,B,A format (in that byte order)
// - ulFormat   = format in which the texture will be stored in accelerator's (or driver's) memory
// - bNoDiscard = no need to discard old texture (for OGL, this is like "use SubImage")
extern void gfxUploadTexture( ULONG *pulTexture, PIX pixWidth, PIX pixHeight, ULONG ulFormat, BOOL bNoDiscard);

// returns size of uploaded texture
extern SLONG gfxGetTextureSize( ULONG ulTexObject, BOOL bHasMipmaps=TRUE);

// returns bytes/pixels ratio for uploaded texture or texture format
extern INDEX gfxGetTexturePixRatio( ULONG ulTextureObject);
extern INDEX gfxGetFormatPixRatio(  ULONG ulTextureFormat);



// VERTEX AND PIXEL PROGRAM

extern void gfxDisableVertexProgram(void);
extern void gfxEnableVertexProgram(void);
extern void gfxEnablePixelProgram(void);
extern void gfxDisablePixelProgram(void);

extern void  (*gfxSetVertexProgram)( const ULONG ulHandle);
extern void  (*gfxSetPixelProgram)(  const ULONG ulHandle);
extern void  (*gfxDeleteVertexProgram)( ULONG ulHandle);
extern void  (*gfxDeletePixelProgram)(  ULONG ulHandle);
extern void  (*gfxSetVertexProgramConst)( INDEX iRegister, const void *pData, INDEX ctRegisters);
extern void  (*gfxSetPixelProgramConst)(  INDEX iRegister, const void *pData, INDEX ctRegisters);
extern ULONG (*gfxCreateVertexProgram)( const char *strVertexProgram, ULONG ulStreamMasks);
extern ULONG (*gfxCreatePixelProgram)(  const char *strPixelProgram);



// VERTEX ARRAYS

// prepare arrays for API
extern void (*gfxSetVertexArray)( GFXVertex *pvtx, INDEX ctVtx);
extern void (*gfxSetNormalArray)( GFXNormal *pnor);
extern void (*gfxSetWeightArray)( GFXWeight *pwgh);
extern void (*gfxSetTexCoordArray)( GFXTexCoord *ptex, BOOL bProjectiveMapping/*=FALSE*/);
extern void (*gfxSetColorArray)( GFXColor *pcol);

// draw prepared arrays
extern void (*gfxDrawElements)( const INDEX ctIndices, const UWORD *puwIndices);

// set constant color for subsequent rendering (until 1st gfxSetColorArray() call!)
extern void (*gfxSetConstantColor)(COLOR col);
// color array usage control
extern void (*gfxEnableColorArray)(void);
extern void (*gfxDisableColorArray)(void);



// vertex buffers fast control

struct VertexBuffer {
public:
	INDEX vb_i1stLockedVertex[GFX_MAX_VBA];
	void *vb_paReadArray[GFX_MAX_VBA]; // pointers to read array(s) that exist in this buffer (non-exist = NULL)
	union {
		void *vb_paWriteArray[GFX_MAX_VBA]; // pointers to write array(s)...
		LPDIRECT3DVERTEXBUFFER8 vb_pavbWrite[GFX_MAX_VBA];
	};
	INDEX vb_ctLockedVertices[GFX_MAX_VBA];
	SLONG vb_slSize;            // total size of this vertex buffer (in bytes)
	INDEX vb_ctVertices;        // number of vertices
	ULONG vb_ulArrayMask;       // which array(s) exist in this buffer
	ULONG vb_ulLockMask;        // which array(s) from this buffer have been locked
	enum GfxLockType vb_eType;  // how is this buffer created (READ, WRITE or DYNAMIC)
};


// prepares vertex buffer (returns somekind of 'bind' number, zero if failed)
// ctTexCoordArrays = how many arrays to reserve for texture coords (-1 = 0 or 1, depends on mask)
// bRead==TRUE if reading from this buffer will be required
extern INDEX gfxCreateVertexBuffer( const INDEX ctVertices, ULONG ulTypeMask, INDEX ctTexCoordArrays=-1,
																		const enum GfxLockType eType=GFX_WRITEONLY);
extern void  gfxDeleteVertexBuffer( const INDEX iBindNo);

// returns ID number of a vertex buffer 'portion' (position, normal, texture coord, color or weight array)
extern INDEX gfxGetVertexSubBufferID( const INDEX iBindNo, const INDEX iType);

// returns number of vertices in a vertex buffer
extern INDEX gfxGetVertexBufferSize( const INDEX iBindNo);

// returns mask of what arrays are used in a vertex buffer and how many texture coord arrays
extern ULONG gfxGetVertexBufferMask( const INDEX iBindNo, INDEX &ctTexCoordArrays);

// lock/unlock one vertex buffer for reading or writing (if iID<16, lock dynamic vertex buffer)
#define gfxLockVertexSubBuffer   gfxLockSubBuffer
#define gfxLockNormalSubBuffer   gfxLockSubBuffer
//안태훈 수정 시작	//(Add Tagent-space Normal Map)(0.1)
#define gfxLockTangentSubBuffer   gfxLockSubBuffer
//안태훈 수정 끝	//(Add Tagent-space Normal Map)(0.1)
#define gfxLockWeightSubBuffer   gfxLockSubBuffer
#define gfxLockTexCoordSubBuffer gfxLockSubBuffer
extern void* (*gfxLockSubBuffer)( const INDEX iID, const INDEX i1stVertex, const INDEX ctVertices,
																	const enum GfxLockType eLockType/*=GFX_WRITE*/);
#define gfxUnlockVertexSubBuffer   gfxUnlockSubBuffer
#define gfxUnlockNormalSubBuffer   gfxUnlockSubBuffer
//안태훈 수정 시작	//(Add Tagent-space Normal Map)(0.1)
#define gfxUnlockTangentSubBuffer   gfxUnlockSubBuffer
//안태훈 수정 끝	//(Add Tagent-space Normal Map)(0.1)
#define gfxUnlockWeightSubBuffer   gfxUnlockSubBuffer
#define gfxUnlockTexCoordSubBuffer gfxUnlockSubBuffer
extern void (*gfxUnlockSubBuffer)( const INDEX iID, const INDEX ctVertices/*=0*/);

// set position sub-buffer to be the current one for rendering (if ctVertices==0 use all vertices)
extern void (*gfxSetVertexSubBuffer)( const INDEX iID, const INDEX i1stVertex/*=0*/,
																			const INDEX ctVertices/*=0*/);
// set normal, weight and color sub-buffers
// (when setting texture coord sub-buffer, iUnit = to which texture unit set this array; -1 = current unit)
#define gfxSetNormalSubBuffer   gfxSetSubBuffer
//안태훈 수정 시작	//(Add Tagent-space Normal Map)(0.1)
#define gfxSetTangentSubBuffer   gfxSetSubBuffer
//안태훈 수정 끝	//(Add Tagent-space Normal Map)(0.1)
#define gfxSetWeightSubBuffer   gfxSetSubBuffer
#define gfxSetTexCoordSubBuffer gfxSetSubBuffer
extern void (*gfxSetSubBuffer)( const INDEX iID, const INDEX iUnit/*=-1*/);



// MISC


// force finish of rendering queue
extern void (*gfxFinish)( BOOL bReadBack/*=FALSE*/);

// compiled vertex array control (for OpenGL only)
extern void (*gfxLockArrays)(void);
extern void gfxUnlockArrays(void);


// helper functions for drawing simple primitives thru drawelements

inline void gfxResetArrays(void)
{
	_avtxCommon.PopAll();
	_acolCommon.PopAll();
	for( INDEX i=0; i<GFX_USETEXUNITS; i++) _atexCommon[i].PopAll();
	_auwCommonElements.PopAll();
}
	
// render elements to screen buffer
extern void gfxFlushElements(void);
extern void gfxFlushQuads(void);
extern void gfxFlushTextElements( INDEX ctElements, INDEX iBuffer );		// yjpark
extern void gfxFlushBtnElements( INDEX ctElements, INDEX iBuffer );			// yjpark


// check GFX errors only in debug builds
#ifndef NDEBUG
	extern void OGL_CheckError(void);
	extern void D3D_CheckError(HRESULT hr);
	#define OGL_CHECKERROR     OGL_CheckError();
	#define D3D_CHECKERROR(hr) D3D_CheckError(hr);
#else
	#define OGL_CHECKERROR     (void)(0);
	#define D3D_CHECKERROR(hr) (void)(0);
#endif



// wrapper for blending types
// (ZW = z-writes, AT = alpha-test)
// WARNING: BPT_BLEND is implicitly set as default in some of drawport functions as 203 ULONG!

enum PredefinedBlendType
{
	PBT_OPAQUE      = 201,    // ZW
	PBT_TRANSPARENT = 202,    // ZW, AT
	PBT_BLEND       = 203,    // S*Sa + D*(1-Sa)    
	PBT_SHADE       = 204,    // S*D  + D*S   (mul x2)
	PBT_ADD         = 205,    // S    + D
	PBT_ADDALPHA    = 206,    // S*Sa + D
	PBT_MULTIPLY    = 207,    // S*D  + 0
	PBT_INVMULTIPLY = 208,    // 0    + D*(1-Sa)
};
// other names
#define PBT_TRANSLUCENT PBT_BLEND

// function
ENGINE_API extern void gfxSetBlendType( const enum PredefinedBlendType pbt);


	
// ATI's TRUFORM support


// set truform parameters
extern void gfxSetTruform( const INDEX iLevel, BOOL bLinearNormals);
extern void (*gfxEnableTruform)( void);
extern void (*gfxDisableTruform)(void);


// set D3D vertex shader only if different than last time
extern void d3dSetVertexShader(DWORD dwHandle);


// macro for releasing D3D objects


#define D3DRELEASE(object,check) \
{ \
	if( object != NULL ) \
	{ \
		INDEX ref; \
		do { \
			ref = (object)->Release(); \
			if(check) ASSERT(ref==0); \
		} while(ref>0);  \
		object = NONE; \
	} \
}


