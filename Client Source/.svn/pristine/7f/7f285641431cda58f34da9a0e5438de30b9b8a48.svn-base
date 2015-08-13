
// DIRECT3D "LOW-LEVEL" INTERFACE (COMMON)

#include "stdh.h"

#include <Engine/Base/Translation.h>
#include <Engine/Base/ErrorReporting.h>
#include <Engine/Base/Memory.h>
#include <Engine/Base/Console.h>
#include <Engine/Base/MemoryTracking.h>
#include <Engine/Math/Float.h>

#include <Engine/Graphics/GfxLibrary.h>
#include <Engine/Graphics/ViewPort.h>

#include <Engine/Templates/StaticStackArray.cpp>
#include <Engine/Templates/DynamicContainer.cpp>
#include <Engine/Templates/Stock_CTextureData.h>

//안태훈 수정 시작	//(Add & Modify SSSE Effect)(0.1)
#include <Engine/Effect/EffectCommon.h>
//안태훈 수정 끝	//(Add & Modify SSSE Effect)(0.1)

#undef new
#include <D3dx8core.h>


// asm shortcuts
#define O offset
#define Q qword ptr
#define D dword ptr
#define W  word ptr
#define B  byte ptr

#define ASMOPT 1


extern ULONG _fog_ulTexture;
extern ULONG _haze_ulTexture;

// occlusion queries
extern UINT *_puiOcclusionQueryIDs;

// state variables
extern BOOL GFX_bDepthTest;
extern BOOL GFX_bDepthWrite;
extern BOOL GFX_bAlphaTest;
extern BOOL GFX_bBlending;
extern BOOL GFX_bDithering;
extern BOOL GFX_bLighting;
extern BOOL GFX_bClipping;
extern BOOL GFX_bClipPlane;
extern BOOL GFX_bColorArray;
extern BOOL GFX_bFrontFace;
extern BOOL GFX_bTruform;
extern INDEX GFX_iActiveTexUnit;
extern FLOAT GFX_fMinDepthRange;
extern FLOAT GFX_fMaxDepthRange;
extern GfxBlend GFX_eBlendSrc;
extern GfxBlend GFX_eBlendDst;
extern GfxComp  GFX_eDepthFunc;
extern GfxFace  GFX_eCullFace;
extern INDEX GFX_iTexModulation[GFX_MAXTEXUNITS];
extern BOOL  GFX_abTexture[GFX_MAXTEXUNITS];
extern BOOL  GFX_abLights[GFX_MAXLIGHTS];

extern INDEX GFX_ctVertices;
extern ULONG GFX_ulCurrentColorMask;
extern BOOL  D3D_bUseColorArray = FALSE;
extern BOOL _bUsingDynamicBuffer;

extern BOOL  GFX_bUseVertexProgram;
extern BOOL  GFX_bUsePixelProgram;

// sehan
extern INDEX d3d_bDeviceChanged;
// sehan end

// internal vars
static INDEX _iVtxOffset = 0;
static INDEX _iVtxPos = 0;
static INDEX _iTexPass = 0;
static INDEX _iColPass = 0;
extern DWORD _dwCurrentVS = NONE;
extern DWORD _dwCurrentPS = NONE;
extern DWORD _dwLastVertexProgram = NONE;
extern ULONG _ulStreamsMask = NONE;
static ULONG _ulLastStreamsMask = NONE;
static BOOL  _bProjectiveMapping = FALSE;
static BOOL  _bLastProjectiveMapping = FALSE;
extern BOOL  _bGenerateTexCoord = FALSE;
static BOOL  _bLastGenerateTexCoord = FALSE;

// swap intervals tables
static UINT _auiSwapIntervals[] = {
	D3DPRESENT_INTERVAL_IMMEDIATE,
	D3DPRESENT_INTERVAL_ONE,
	D3DPRESENT_INTERVAL_TWO,
	D3DPRESENT_INTERVAL_THREE };

// shaders created so far
struct VertexShader {
	DWORD vs_dwHandle;
	ULONG vs_ulStreamMask;
};

static CStaticStackArray<VertexShader> _avsFixedShaders;

// device type (HAL is default, REF is only for debuging)
extern const D3DDEVTYPE d3dDevType = D3DDEVTYPE_HAL;

// identity matrix
extern const D3DMATRIX GFX_d3dIdentityMatrix = {
	1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
	0, 0, 0, 1 };


// size of one vertex
#define VTXSIZE (GFX_POSSIZE + GFX_NORSIZE + GFX_WGHSIZE + GFX_TX4SIZE \
							 + GFX_COLSIZE *  _pGfx->gl_ctColBuffers \
							 + GFX_TEXSIZE * (_pGfx->gl_ctTexBuffers-1))


// SHADER SETUP PARAMS

#define DECLTEXOFS (2*GFX_TEXIDX)

// NOTE: fixed function vs and programmable vs templates must have same streams and same register count

// template shader for fixed function vertex shader
DWORD _adwDeclTemplateFF[] = {
	D3DVSD_STREAM(0),
	D3DVSD_REG( D3DVSDE_POSITION,  D3DVSDT_FLOAT3),
	D3DVSD_STREAM(1),
	D3DVSD_REG( D3DVSDE_DIFFUSE,   D3DVSDT_D3DCOLOR),
	D3DVSD_STREAM(2),
	D3DVSD_REG( D3DVSDE_TEXCOORD0, D3DVSDT_FLOAT2),
	D3DVSD_STREAM(3),
	D3DVSD_REG( D3DVSDE_TEXCOORD1, D3DVSDT_FLOAT2),
	D3DVSD_STREAM(4),
	D3DVSD_REG( D3DVSDE_TEXCOORD2, D3DVSDT_FLOAT2),
	D3DVSD_STREAM(5),
	D3DVSD_REG( D3DVSDE_TEXCOORD3, D3DVSDT_FLOAT2),
	D3DVSD_STREAM(6),
	D3DVSD_REG( D3DVSDE_NORMAL,    D3DVSDT_FLOAT3),
	D3DVSD_STREAM(7), // not used in fixed-function vs but must be declared for stream reset
	D3DVSD_REG( NONE, NONE),
	D3DVSD_REG( NONE, NONE),
	D3DVSD_END()
};

// template shader for programmable vertex shader
DWORD _adwDeclTemplateVP[] = {
	D3DVSD_STREAM(0),
	D3DVSD_REG( 0,  D3DVSDT_FLOAT3),    // position
	D3DVSD_STREAM(1),
	D3DVSD_REG( 4,  D3DVSDT_D3DCOLOR),  // diffuse
	D3DVSD_STREAM(2),
	D3DVSD_REG( 5,  D3DVSDT_FLOAT2),    // texcoord0
	D3DVSD_STREAM(3),
	D3DVSD_REG( 6,  D3DVSDT_FLOAT2),    // texcoord1
	D3DVSD_STREAM(4),
	D3DVSD_REG( 7,  D3DVSDT_FLOAT2),    // texcoord2
	D3DVSD_STREAM(5),
	D3DVSD_REG( 8, D3DVSDT_FLOAT2),     // texcoord3
	D3DVSD_STREAM(6),
	D3DVSD_REG( 1,  D3DVSDT_FLOAT3),    // normal
	D3DVSD_STREAM(7),
	D3DVSD_REG( 3,  D3DVSDT_D3DCOLOR),  // blend indices
	D3DVSD_REG( 2,  D3DVSDT_D3DCOLOR),  // blend weights
	D3DVSD_END()
};

INDEX _aiStreamRegs[] = {
	1,// D3DVSDE_POSITION
	1,// D3DVSDE_DIFFUSE
	1,// D3DVSDE_TEXCOORD0
	1,// D3DVSDE_TEXCOORD1
	1,// D3DVSDE_TEXCOORD2
	1,// D3DVSDE_TEXCOORD3
	1,// D3DVSDE_NORMAL
	2,// D3DVSDE_BLENDINDICES & D3DVSDE_BLENDWEIGHT
};

// current shader
DWORD _adwCurrentDecl[MAX_SHADER_DECL_SIZE];



// check whether texture format is supported in D3D
static BOOL HasTextureFormat_D3D( D3DFORMAT d3dTextureFormat)
{
	// quickie?
	const D3DFORMAT d3dScreenFormat = _pGfx->gl_d3dColorFormat;
	if( d3dTextureFormat==D3DFMT_UNKNOWN || d3dScreenFormat==NONE) return TRUE;
	// checkie! :)
	HRESULT hr = _pGfx->gl_pD3D->CheckDeviceFormat( _pGfx->gl_iCurrentAdapter, d3dDevType, d3dScreenFormat,
																									0, D3DRTYPE_TEXTURE, d3dTextureFormat);
	return( hr==D3D_OK);
}


// returns number of vertices based on vertex size and required size in memory (KB)
extern INDEX VerticesFromSize_D3D( SLONG slSizeKB)
{
	const INDEX ctVertices = slSizeKB*1024 / VTXSIZE;
	ASSERT( ctVertices>0 && ctVertices<65536);
	return( ctVertices);
}

// returns size in memory based on number of vertices
extern SLONG SizeFromVertices_D3D( INDEX ctVertices)
{
	ASSERT( ctVertices>0 && ctVertices<65536);
	return( ctVertices * VTXSIZE);
}


// Get shader stream declaration from stream flags
extern void GetShaderDeclaration_D3D(ULONG *ulRetDecl, ULONG ulStreamFlags)
{
	// if using position stream
	if(ulStreamFlags&GFX_POSITION_STREAM) {
		*(  ulRetDecl) = D3DVSD_STREAM(0);
		*(++ulRetDecl) = D3DVSD_REG( 0,  D3DVSDT_FLOAT3);
		ulStreamFlags&=~GFX_POSITION_STREAM;
	}
	// if using color stream
	if(ulStreamFlags&GFX_COLOR_STREAM) {
		*(++ulRetDecl) = D3DVSD_STREAM(1);
		*(++ulRetDecl) = D3DVSD_REG( 4,  D3DVSDT_D3DCOLOR);
		ulStreamFlags&=~GFX_COLOR_STREAM;
	}
	// if using texture unit 1
	if(ulStreamFlags&GFX_TEXCOORD0) {
		*(++ulRetDecl) = D3DVSD_STREAM(2);
		*(++ulRetDecl) = D3DVSD_REG( 5,  D3DVSDT_FLOAT2);
		ulStreamFlags&=~GFX_TEXCOORD0;
	}
	// if using texture unit 2
	if(ulStreamFlags&GFX_TEXCOORD1) {
		*(++ulRetDecl) = D3DVSD_STREAM(3);
		*(++ulRetDecl) = D3DVSD_REG( 6,  D3DVSDT_FLOAT2);
		ulStreamFlags&=~GFX_TEXCOORD1;
	}
	// if using texture unit 3
	if(ulStreamFlags&GFX_TEXCOORD2) {
		*(++ulRetDecl) = D3DVSD_STREAM(4);
		*(++ulRetDecl) = D3DVSD_REG( 7,  D3DVSDT_FLOAT2);
		ulStreamFlags&=~GFX_TEXCOORD2;
	}
	// if using texture unit 4
	if(ulStreamFlags&GFX_TEXCOORD3) {
		*(++ulRetDecl) = D3DVSD_STREAM(5);
		*(++ulRetDecl) = D3DVSD_REG( 8,  D3DVSDT_FLOAT2);
		ulStreamFlags&=~GFX_TEXCOORD3;
	}
	if(ulStreamFlags&GFX_NORMAL_STREAM) {
		*(++ulRetDecl) = D3DVSD_STREAM(6);
		*(++ulRetDecl) = D3DVSD_REG( 1,  D3DVSDT_FLOAT3);
		ulStreamFlags&=~GFX_NORMAL_STREAM;
	}
	if(ulStreamFlags&GFX_WEIGHT_STREAM) {
		*(++ulRetDecl) = D3DVSD_STREAM(7);
		*(++ulRetDecl) = D3DVSD_REG( 3,  D3DVSDT_D3DCOLOR);
		*(++ulRetDecl) = D3DVSD_REG( 2,  D3DVSDT_D3DCOLOR);
		ulStreamFlags&=~GFX_WEIGHT_STREAM;
	}
//안태훈 수정 시작	//(Add Tagent-space Normal Map)(0.1)
	// if using tangent
	if(ulStreamFlags&GFX_TANGENT_STREAM) {
		//*(++ulRetDecl) = D3DVSD_STREAM(8);	//원본
		*(++ulRetDecl) = D3DVSD_STREAM(3);
		*(++ulRetDecl) = D3DVSD_REG( 9,  D3DVSDT_FLOAT4);
		ulStreamFlags&=~GFX_TANGENT_STREAM;
	}
//안태훈 수정 끝	//(Add Tagent-space Normal Map)(0.1)
	ASSERT(ulStreamFlags==0); // make sure stream flags were valid
	*(++ulRetDecl) = D3DVSD_END();
}

/*
// template shader for programmable vertex shader
DWORD _adwDeclTemplateVP[] = {
	D3DVSD_STREAM(0),
	D3DVSD_REG( 0,  D3DVSDT_FLOAT3),    // position
	D3DVSD_STREAM(1),
	D3DVSD_REG( 4,  D3DVSDT_D3DCOLOR),  // diffuse
	D3DVSD_STREAM(2),
	D3DVSD_REG( 5,  D3DVSDT_FLOAT2),    // texcoord0
	D3DVSD_STREAM(3),
	D3DVSD_REG( 6,  D3DVSDT_FLOAT2),    // texcoord1
	D3DVSD_STREAM(4),
	D3DVSD_REG( 7,  D3DVSDT_FLOAT2),    // texcoord2
	D3DVSD_STREAM(5),
	D3DVSD_REG( 8, D3DVSDT_FLOAT2),     // texcoord3
	D3DVSD_STREAM(6),
	D3DVSD_REG( 1,  D3DVSDT_FLOAT3),    // normal
	D3DVSD_STREAM(7),
	D3DVSD_REG( 3,  D3DVSDT_D3DCOLOR),  // blend indices
	D3DVSD_REG( 2,  D3DVSDT_D3DCOLOR),  // blend weights
	D3DVSD_END()
};

*/
// Compile vertex program from given vertex program source code
//안태훈 수정 시작	//(DevPartner Bug Fix)(2005-01-13)
extern ULONG *CompileVertexProgram_D3D(const char *strVertexProgram, ID3DXBuffer **ppOut)
{
	ULONG ulFlags = 0;
	ID3DXBuffer *pdxbErrors = NULL;
#if _DEBUG
	ulFlags |= D3DXASM_DEBUG;
	HRESULT hr = D3DXAssembleShader(strVertexProgram,strlen(strVertexProgram),ulFlags,NULL,ppOut,&pdxbErrors);
	if(hr!=D3D_OK) {
		const char *pstrErrors = (const char *)pdxbErrors->GetBufferPointer();
		CPrintF(pstrErrors);
		ASSERTALWAYS("Shader syntax error\n");
		pdxbErrors->Release();
	}
#else 
	HRESULT hr = D3DXAssembleShader(strVertexProgram,strlen(strVertexProgram),ulFlags,NULL,ppOut,NULL);
#endif
	ASSERT(hr==D3D_OK);
	D3D_CHECKERROR(hr);
	return (ULONG*)(*ppOut)->GetBufferPointer();
}

extern ULONG *CompilePixelProgram_D3D(const char *strVertexProgram, ID3DXBuffer **ppOut)
{
	return CompileVertexProgram_D3D(strVertexProgram, ppOut);
}
//안태훈 수정 끝	//(DevPartner Bug Fix)(2005-01-13)

// construct vertex shader out of streams' bit-mask
extern DWORD SetupShader_D3D( ULONG ulStreamsMask)
{
	HRESULT hr;
	const LPDIRECT3DDEVICE8 pd3dDev = _pGfx->gl_pd3dDevice;
	const INDEX ctFixedShaders = _avsFixedShaders.Count();

	INDEX iVS;

	// delete all shaders?
	if( ulStreamsMask==NONE) {
		// first set default vertex shader
		hr = pd3dDev->SetVertexShader(D3DFVF_CTVERTEX);
		D3D_CHECKERROR(hr);
		gfxSetPixelProgram(NONE);

		for( iVS=0; iVS<ctFixedShaders; iVS++) {
			hr = pd3dDev->DeleteVertexShader(_avsFixedShaders[iVS].vs_dwHandle);
			D3D_CHECKERROR(hr);
		}

		// Delete all programable vertex and pixel programs
		extern void ClearVertexAndPixelPrograms(void);
		ClearVertexAndPixelPrograms();

		// free array
		_avsFixedShaders.PopAll();
		_dwCurrentVS = NONE;
		_dwCurrentPS = NONE;
		_dwLastVertexProgram = NONE;
		return NONE;
	}

	// see if required fixed function shader has already been created
	for( iVS=0; iVS<ctFixedShaders; iVS++) {
		if( _avsFixedShaders[iVS].vs_ulStreamMask==ulStreamsMask) return _avsFixedShaders[iVS].vs_dwHandle;
	}

	// darn, need to create shader :(

	// pre-adjustment for eventual projective mapping
	_adwDeclTemplateFF[DECLTEXOFS+1] = D3DVSD_REG( D3DVSDE_TEXCOORD0, (ulStreamsMask&0x1000) ? D3DVSDT_FLOAT4 : D3DVSDT_FLOAT2);

	ulStreamsMask &= ~0x1000;

	ULONG ulMask = ulStreamsMask;

	// process mask, bit by bit
	INDEX iSrcDecl=0, iDstDecl=0;
	INDEX iStream=0;
	while(_adwDeclTemplateFF[iSrcDecl]!=D3DVSD_END())
	{ // add declarator if used
		INDEX ctRegs = _aiStreamRegs[iStream] + 1; // n*D3DVSD_REG() + D3DVSD_STREAM()
		if( ulMask&1) {
			for(INDEX ireg=0;ireg<ctRegs;ireg++) {
				_adwCurrentDecl[iDstDecl+ireg] = _adwDeclTemplateFF[iSrcDecl+ireg];
			}
			iDstDecl+=ctRegs;
		}
		iSrcDecl+=ctRegs;
		ulMask >>= 1;
		iStream++;
		/*
		if( ulStreamsMask&1) {
			_adwCurrentDecl[iDstDecl*2+0] = _adwDeclTemplateFF[iSrcDecl*2+0];
			_adwCurrentDecl[iDstDecl*2+1] = _adwDeclTemplateFF[iSrcDecl*2+1];
			iDstDecl++;
		} // move to next declarator
		iSrcDecl++;
		ulStreamsMask >>= 1;
		*/
	}
	// mark end
	_adwCurrentDecl[iDstDecl] = D3DVSD_END();
	// ASSERT( iDstDecl < MAXSTREAMS);
	#pragma message(">> Fix ASSERT( iDstDecl < MAXSTREAMS)")
	ASSERT( _iTexPass>=0 && _iColPass>=0);
	ASSERT( _iVtxPos >=0 && _iVtxPos<65536);

	// create new vertex shader
	const DWORD dwFlags = (_pGfx->gl_ulFlags&GLF_D3D_USINGHWTNL) ? NONE : D3DUSAGE_SOFTWAREPROCESSING;

	VertexShader &vs = _avsFixedShaders.Push();
	vs.vs_ulStreamMask = ulStreamsMask;
	
	// create fixed shader
	hr = pd3dDev->CreateVertexShader( &_adwCurrentDecl[0], NULL, &vs.vs_dwHandle, dwFlags);
	D3D_CHECKERROR(hr);
	// reset current shader
	_pGfx->gl_dwVertexShader = NONE;
	return vs.vs_dwHandle;
}



// DIRECT3D "LOW-LEVEL" INTERFACE FOR PC

	 

extern INDEX gap_iTruformLevel;

static INDEX _iIdxOffset = 0;
static DWORD _dwVtxLockFlag;                  // for vertex and normal
static DWORD _dwColLockFlags[GFX_MAXLAYERS];  // for colors
static DWORD _dwTexLockFlags[GFX_MAXLAYERS];  // for texture coords
static LPDIRECT3DVERTEXBUFFER8 _pd3dLockedVtx = NULL;
static LPDIRECT3DVERTEXBUFFER8 _pd3dLockedNor = NULL;
//안태훈 수정 시작	//(Add Tagent-space Normal Map)(0.1)
static LPDIRECT3DVERTEXBUFFER8 _pd3dLockedTan = NULL;
//안태훈 수정 끝	//(Add Tagent-space Normal Map)(0.1)
static LPDIRECT3DVERTEXBUFFER8 _pd3dLockedWgh = NULL;
static LPDIRECT3DVERTEXBUFFER8 _pd3dLockedCol = NULL;
static LPDIRECT3DVERTEXBUFFER8 _pd3dLockedTex = NULL;

// system gamma table
extern UWORD _auwSystemGammaTable[256*3];
static D3DGAMMARAMP *pgrtSystemGamma = (D3DGAMMARAMP*)&_auwSystemGammaTable[0];



// prepare vertex arrays for drawing
extern void SetupVertexArrays_D3D( INDEX ctVertices)
{
	INDEX i;
	HRESULT hr;
	ASSERT( ctVertices>=0);

	// do nothing if buffer is sufficient
	ctVertices = ClampUp( ctVertices, 65535L); // need to clamp max vertices first
	if( ctVertices!=0 && ctVertices<=_pGfx->gl_ctVertices) return;
	const LPDIRECT3DDEVICE8 pd3dDev = _pGfx->gl_pd3dDevice;

	// deallocate if needed
	if( _pGfx->gl_pd3dVtx!=NULL)
	{
		// reset all streams
		for( i=0; i<_pGfx->gl_ctMaxStreams; i++) {
			hr = pd3dDev->SetStreamSource( i, NULL,0);
			D3D_CHECKERROR(hr);
		}
		// release vertex and eventual normal array
		D3DRELEASE( _pGfx->gl_pd3dVtx, TRUE);
		ASSERT( _pGfx->gl_pd3dNor!=NULL);
		D3DRELEASE( _pGfx->gl_pd3dNor, TRUE);
		ASSERT( _pGfx->gl_pd3dWgh!=NULL);
		D3DRELEASE( _pGfx->gl_pd3dWgh, TRUE);
		// color arrays
		for( i=0; i<_pGfx->gl_ctColBuffers; i++) {
			ASSERT( _pGfx->gl_pd3dCol[i]!=NULL);
			D3DRELEASE( _pGfx->gl_pd3dCol[i], TRUE);
		}
		// texcoord arrays
		for( i=0; i<_pGfx->gl_ctTexBuffers; i++) {
			ASSERT( _pGfx->gl_pd3dTex[i]!=NULL);
			D3DRELEASE( _pGfx->gl_pd3dTex[i], TRUE);
		}
	}

	// allocate if needed
	if( ctVertices>0)
	{
		// update max vertex count
		if( _pGfx->gl_ctVertices < ctVertices) _pGfx->gl_ctVertices = ctVertices;
		else ctVertices = _pGfx->gl_ctVertices;

		// determine SW or HW VP and NPatches usage (Truform) 
		DWORD dwFlags = D3DUSAGE_DYNAMIC|D3DUSAGE_WRITEONLY;
		if( !(_pGfx->gl_ulFlags&GLF_D3D_USINGHWTNL)) dwFlags |= D3DUSAGE_SOFTWAREPROCESSING;
		if( _pGfx->gl_iMaxTessellationLevel>0 && gap_iTruformLevel>0) dwFlags |= D3DUSAGE_NPATCHES;

		// vertices and normals...
		hr = pd3dDev->CreateVertexBuffer( ctVertices*GFX_POSSIZE, dwFlags, 0, D3DPOOL_DEFAULT, &_pGfx->gl_pd3dVtx);  D3D_CHECKERROR(hr);
		hr = pd3dDev->CreateVertexBuffer( ctVertices*GFX_NORSIZE, dwFlags, 0, D3DPOOL_DEFAULT, &_pGfx->gl_pd3dNor);  D3D_CHECKERROR(hr);
		hr = pd3dDev->CreateVertexBuffer( ctVertices*GFX_WGHSIZE, dwFlags, 0, D3DPOOL_DEFAULT, &_pGfx->gl_pd3dWgh);  D3D_CHECKERROR(hr);

		// all color buffers...
		for( i=0; i<_pGfx->gl_ctColBuffers; i++) {
			hr = pd3dDev->CreateVertexBuffer( ctVertices*GFX_COLSIZE, dwFlags, 0, D3DPOOL_DEFAULT, &_pGfx->gl_pd3dCol[i]);
			D3D_CHECKERROR(hr);
		} 
		// all texcoord buffers...
		for( i=0; i<_pGfx->gl_ctTexBuffers; i++) {
			const SLONG slSize = ctVertices * (i==0 ? GFX_TX4SIZE : GFX_TEXSIZE); // 1st texture buffer might have projective mapping
			hr = pd3dDev->CreateVertexBuffer( slSize, dwFlags, 0, D3DPOOL_DEFAULT, &_pGfx->gl_pd3dTex[i]);
			D3D_CHECKERROR(hr);
		}
	}
	// just switch it off if not needed any more (i.e. D3D is shutting down)
	else _pGfx->gl_ctVertices = 0;

	// reset and check
	_iVtxOffset = 0;
	_pGfx->gl_dwVertexShader = NONE;
	_pGfx->gl_dwPixelShader = NONE;
	_ulStreamsMask = NONE;
	_ulLastStreamsMask = NONE;
	_bProjectiveMapping = FALSE;
	_bLastProjectiveMapping = FALSE;
	_bGenerateTexCoord = FALSE;
	_bLastGenerateTexCoord = FALSE;
	_pd3dLockedVtx = _pd3dLockedNor = _pd3dLockedWgh = NULL;
	_pd3dLockedCol = _pd3dLockedTex = NULL;

	// reset to initial locking flags
	_dwVtxLockFlag = D3DLOCK_DISCARD;
	for( i=0; i<GFX_MAXLAYERS; i++) _dwColLockFlags[i] = _dwTexLockFlags[i] = D3DLOCK_DISCARD;
}



// prepare index arrays for drawing
extern void SetupIndexArray_D3D( INDEX ctIndices)
{
	HRESULT hr;
	ASSERT( ctIndices>=0);
	// clamp max indices
	// ctIndices = ClampUp( ctIndices, 65535L);

	// determine SW or HW VP
	DWORD dwFlags = D3DUSAGE_DYNAMIC|D3DUSAGE_WRITEONLY;
	const LPDIRECT3DDEVICE8 pd3dDev = _pGfx->gl_pd3dDevice;
	if( !(_pGfx->gl_ulFlags&GLF_D3D_USINGHWTNL)) dwFlags |= D3DUSAGE_SOFTWAREPROCESSING;
	if( _pGfx->gl_iMaxTessellationLevel>0 && gap_iTruformLevel>0) dwFlags |= D3DUSAGE_NPATCHES;
	
	// dealocate if needed
	if( _pGfx->gl_ctIndices>0) D3DRELEASE( _pGfx->gl_pd3dIdx, TRUE);

	// allocate if needed
	if( ctIndices>0)
	{
		// eventually update max index count
		if( _pGfx->gl_ctIndices < ctIndices) _pGfx->gl_ctIndices = ctIndices;
		else ctIndices = _pGfx->gl_ctIndices;
		// create buffer
		hr = pd3dDev->CreateIndexBuffer( ctIndices*GFX_IDXSIZE, dwFlags, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &_pGfx->gl_pd3dIdx);
		D3D_CHECKERROR(hr);
		// set it
		hr = _pGfx->gl_pd3dDevice->SetIndices( _pGfx->gl_pd3dIdx, 0);
		D3D_CHECKERROR(hr);
	}
	// just switch it off if not needed any more (i.e. D3D is shutting down)
	else _pGfx->gl_ctIndices = 0;
	
	// reset and check
	_iIdxOffset = 0;
	// ASSERT(_pGfx->gl_ctIndices<65536);
}



// initialize Direct3D driver
BOOL CGfxLibrary::InitDriver_D3D(void)
{
	// check for presence of DirectX 8
	gl_hiDriver = LoadLibrary( "D3D8.DLL");
	if( gl_hiDriver==NONE) {
		// not present - BUAHHAHAHAHAR :)
		CPrintF( "DX8 error: API not installed.\n");
		gl_gaAPI[GAT_D3D].ga_ctAdapters = 0;
		return FALSE; 
	}

	// query DX8 interface
	IDirect3D8* (WINAPI *pDirect3DCreate8)(UINT SDKVersion);
	pDirect3DCreate8 = (IDirect3D8* (WINAPI *)(UINT SDKVersion))GetProcAddress( gl_hiDriver, "Direct3DCreate8");
	if( pDirect3DCreate8==NULL) {
		// cannot init
		CPrintF( "DX8 error: Cannot get entry procedure address.\n");
		FreeLibrary(gl_hiDriver);
		gl_hiDriver = NONE;
		return FALSE; 
	}

	// init DX8
	gl_pD3D = pDirect3DCreate8(D3D_SDK_VERSION);
	if( gl_pD3D==NULL) {
		// cannot start
		CPrintF( "DX8 error: Cannot be initialized.\n");
		FreeLibrary(gl_hiDriver);
		gl_hiDriver = NONE;
		return FALSE;
	}

	// made it!
	return TRUE;
}


// initialize Direct3D driver
void CGfxLibrary::EndDriver_D3D(void)
{
	// free occlusion queries
	if( _puiOcclusionQueryIDs!=NULL) { 
		FreeMemory(_puiOcclusionQueryIDs);
	 _puiOcclusionQueryIDs = NULL;
	}

	// reset shader and vertices
	SetupShader_D3D(NONE); 
	SetupVertexArrays_D3D(0); 
	SetupIndexArray_D3D(0); 
	gl_d3dColorFormat = (D3DFORMAT)NONE;
	gl_d3dDepthFormat = (D3DFORMAT)NONE;

	// restore system gamma table
	if( gl_ulFlags & GLF_ADJUSTABLEGAMMA) {
		gl_pd3dDevice->SetGammaRamp( NONE, pgrtSystemGamma);
	}

	// shutdown device and d3d
	INDEX iRef;
	iRef = gl_pd3dDevice->Release();
	iRef = gl_pD3D->Release();
}


// prepare current viewport for rendering thru Direct3D
BOOL CGfxLibrary::SetCurrentViewport_D3D(CViewPort *pvp)
{
	// determine full screen mode
	CDisplayMode dm;
	RECT rectWindow;
	GetCurrentDisplayMode(dm);
	ASSERT( (dm.dm_pixSizeI==0 && dm.dm_pixSizeJ==0) || (dm.dm_pixSizeI!=0 && dm.dm_pixSizeJ!=0));
	GetClientRect( pvp->vp_hWnd, &rectWindow);
	const PIX pixWinSizeI = rectWindow.right  - rectWindow.left;
	const PIX pixWinSizeJ = rectWindow.bottom - rectWindow.top;

	// full screen allows only one window (main one, which has already been initialized)
	if( dm.dm_pixSizeI==pixWinSizeI && dm.dm_pixSizeJ==pixWinSizeJ) {
		gl_pvpActive = pvp;  // remember as current viewport (must do that BEFORE InitContext)
		if( gl_ulFlags & GLF_INITONNEXTWINDOW) InitContext_D3D();
		gl_ulFlags &= ~GLF_INITONNEXTWINDOW;
		return TRUE; 
	}

	// if must init entire D3D
	if( gl_ulFlags & GLF_INITONNEXTWINDOW) {
		gl_ulFlags &= ~GLF_INITONNEXTWINDOW;
		// additional swap chains have been destroyed
		pvp->vp_pSwapChain = NULL;
		pvp->vp_pSurfDepth = NULL;
		// reopen window
		pvp->CloseCanvas();
		pvp->OpenCanvas();
		gl_pvpActive = pvp;
		InitContext_D3D();
		pvp->vp_ctDisplayChanges = gl_ctDriverChanges;
		return TRUE;
	}

	// if window was not set for this driver
	if( pvp->vp_ctDisplayChanges<gl_ctDriverChanges) {
		// additional swap chains have been destroyed
		//pvp->vp_pSwapChain = NULL;
		//pvp->vp_pSurfDepth = NULL;
		// reopen window
		pvp->CloseCanvas();
		pvp->OpenCanvas();
		pvp->vp_ctDisplayChanges = gl_ctDriverChanges;
		gl_pvpActive = pvp;
		return TRUE;
	}

	// no need to set context if it is the same window as last time
	if( gl_pvpActive!=NULL && gl_pvpActive->vp_hWnd==pvp->vp_hWnd) return TRUE;

	// set rendering target
	HRESULT hr;
	LPDIRECT3DSURFACE8 pColorSurface =NULL;
	if( pvp->vp_pSwapChain != NULL) 
	{
		hr = pvp->vp_pSwapChain->GetBackBuffer( 0, D3DBACKBUFFER_TYPE_MONO, &pColorSurface);
		if( hr!=D3D_OK) return FALSE;
	}

	//////////////////////////////////////////////////////////////////////////////
	// [070711: Su-won] WORLDEDITOR_BUGFIX							  |---------->
	if(pvp->vp_pSurfDepth==NULL || pColorSurface ==NULL)
	{
		return FALSE;	
	}
	// [070711: Su-won] WORLDEDITOR_BUGFIX							  <----------|
	//////////////////////////////////////////////////////////////////////////////

	hr = gl_pd3dDevice->SetRenderTarget( pColorSurface, pvp->vp_pSurfDepth);
	D3DRELEASE( pColorSurface, TRUE);
	if( hr!=D3D_OK) return FALSE;

	// remember as current window
	gl_pvpActive = pvp;
	return TRUE;
}



// prepares Direct3D drawing context
void CGfxLibrary::InitContext_D3D()
{
	// must have context
	ASSERT( gl_pvpActive!=NULL);

	// report header
	CPrintF( TRANS("\n* Direct3D context created: *----------------------------------\n"));
	CDisplayAdapter &da = gl_gaAPI[GAT_D3D].ga_adaAdapter[gl_iCurrentAdapter];
	CPrintF( "  (%s, %s, %s)\n\n", da.da_strVendor, da.da_strRenderer, da.da_strVersion);

	// <-- ErrorLog.txt에 디스플레이 정보를 기록하기 위한 부분
	extern CTString _strDisplayDriver;
	extern CTString _strDisplayDriverVersion;
	_strDisplayDriver = da.da_strRenderer;
	_strDisplayDriverVersion = da.da_strVersion;
	// -->
	
	HRESULT hr;

	// reset engine's internal Direct3D state variables
	GFX_bTruform   = FALSE;
	GFX_bClipping  = TRUE;
	GFX_bFrontFace = TRUE;
	GFX_bUseVertexProgram = FALSE;
	GFX_bUsePixelProgram  = FALSE;
	GFX_ulCurrentColorMask = 12345678; // force next call to this function to be efficient
	hr = gl_pd3dDevice->SetRenderState( D3DRS_ZENABLE, D3DZB_FALSE);     D3D_CHECKERROR(hr);  GFX_bDepthTest  = FALSE;
	hr = gl_pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE, FALSE);      D3D_CHECKERROR(hr);  GFX_bDepthWrite = FALSE;
	hr = gl_pd3dDevice->SetRenderState( D3DRS_ALPHATESTENABLE, FALSE);   D3D_CHECKERROR(hr);  GFX_bAlphaTest  = FALSE;
	hr = gl_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE);  D3D_CHECKERROR(hr);  GFX_bBlending   = FALSE;
	hr = gl_pd3dDevice->SetRenderState( D3DRS_DITHERENABLE, TRUE);       D3D_CHECKERROR(hr);  GFX_bDithering  = TRUE;
	hr = gl_pd3dDevice->SetRenderState( D3DRS_COLORVERTEX, FALSE);       D3D_CHECKERROR(hr);  GFX_bColorArray = FALSE;
	hr = gl_pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE);          D3D_CHECKERROR(hr);  GFX_bLighting   = FALSE;
	hr = gl_pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE);   D3D_CHECKERROR(hr);  GFX_eCullFace   = GFX_NONE;
	hr = gl_pd3dDevice->SetRenderState( D3DRS_ZFUNC, D3DCMP_LESSEQUAL);  D3D_CHECKERROR(hr);  GFX_eDepthFunc  = GFX_LESS_EQUAL;
	hr = gl_pd3dDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_ONE);  D3D_CHECKERROR(hr);  GFX_eBlendSrc   = GFX_ONE; 
	hr = gl_pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE);  D3D_CHECKERROR(hr);  GFX_eBlendDst   = GFX_ONE;
	hr = gl_pd3dDevice->SetRenderState( D3DRS_CLIPPLANEENABLE, FALSE);   D3D_CHECKERROR(hr);  GFX_bClipPlane  = FALSE;

	// set global ambient to black and disable all lights
	hr = gl_pd3dDevice->SetRenderState( D3DRS_AMBIENT, 0); D3D_CHECKERROR(hr); // or 0xFFFFFFFF !!!!
	for( INDEX iLight=0; iLight<GFX_MAXLIGHTS; iLight++) {
		GFX_abLights[iLight] = FALSE;
		gl_pd3dDevice->LightEnable( iLight, FALSE);
	}
		
	// (re)set some D3D defaults
	hr = gl_pd3dDevice->SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL); D3D_CHECKERROR(hr); 
	hr = gl_pd3dDevice->SetRenderState( D3DRS_ALPHAREF,  128);                 D3D_CHECKERROR(hr); 

	// constant color default setup
	D3DMATERIAL8 d3dMaterial;
	memset( &d3dMaterial, 0, sizeof(d3dMaterial));
	d3dMaterial.Diffuse.r = d3dMaterial.Ambient.r = 1.0f;
	d3dMaterial.Diffuse.g = d3dMaterial.Ambient.g = 1.0f;
	d3dMaterial.Diffuse.b = d3dMaterial.Ambient.b = 1.0f;
	d3dMaterial.Diffuse.a = d3dMaterial.Ambient.a = 1.0f;
	hr = gl_pd3dDevice->SetMaterial(&d3dMaterial);
	D3D_CHECKERROR(hr); 

	// set default texture unit and modulation mode
	GFX_iActiveTexUnit = 0;
	// reset frustum/ortho matrix
	extern BOOL  GFX_bViewMatrix;
	extern FLOAT GFX_fLastL, GFX_fLastR, GFX_fLastT, GFX_fLastB, GFX_fLastN, GFX_fLastF;
	GFX_fLastL = GFX_fLastR = GFX_fLastT = GFX_fLastB = GFX_fLastN = GFX_fLastF = 0;
	GFX_bViewMatrix = TRUE;

	// reset depth range
	GFX_fMinDepthRange = 0.0f;
	GFX_fMaxDepthRange = 1.0f;
	D3DVIEWPORT8 d3dViewPort = { 0,0, 8,8, 0,1 };
	hr = gl_pd3dDevice->SetViewport( &d3dViewPort);
	D3D_CHECKERROR(hr);
#ifndef NDEBUG
	hr = gl_pd3dDevice->GetViewport( &d3dViewPort);
	D3D_CHECKERROR(hr);
	ASSERT( d3dViewPort.MinZ==0 && d3dViewPort.MaxZ==1);
#endif

	// get capabilities
	D3DCAPS8 d3dCaps;
	hr = gl_pd3dDevice->GetDeviceCaps(&d3dCaps);
	D3D_CHECKERROR(hr);

	// if full screen and gamma adjustment is supported
	gl_ulFlags &= ~GLF_ADJUSTABLEGAMMA;
	if( gl_ulFlags & GLF_FULLSCREEN) {
		if( d3dCaps.Caps2 & D3DCAPS2_FULLSCREENGAMMA) {
			// store system gamma table
			gl_pd3dDevice->GetGammaRamp(pgrtSystemGamma);
			gl_ulFlags |= GLF_ADJUSTABLEGAMMA;
			for( INDEX i=0; i<256*3; i++) ((UWORD*)pgrtSystemGamma)[i] <<= 8;
		} else CPrintF( TRANS("\nWARNING: Gamma, brightness and contrast are not adjustable.\n\n"));
	}

	// determine rasterizer acceleration
	gl_ulFlags &= ~GLF_HASACCELERATION;
	if( (d3dCaps.DevCaps & D3DDEVCAPS_HWRASTERIZATION)
		|| d3dDevType==D3DDEVTYPE_REF) gl_ulFlags |= GLF_HASACCELERATION;

	// determine support for 32-bit textures
	gl_ulFlags &= ~GLF_32BITTEXTURES;
	if( HasTextureFormat_D3D(D3DFMT_X8R8G8B8)
	 || HasTextureFormat_D3D(D3DFMT_A8R8G8B8)) gl_ulFlags |= GLF_32BITTEXTURES;

	// determine support for compressed textures
	gl_ulFlags &= ~GLF_TEXTURECOMPRESSION;
	if( HasTextureFormat_D3D(D3DFMT_DXT1)) gl_ulFlags |= GLF_TEXTURECOMPRESSION;

	// determine max supported dimension of texture
	gl_pixMaxTextureDimension = d3dCaps.MaxTextureWidth;
	ASSERT( gl_pixMaxTextureDimension == d3dCaps.MaxTextureHeight); // perhaps not ?

	// determine max primitive count
	gl_ctMaxPrimitives = d3dCaps.MaxPrimitiveCount;

	// determine support for disabling of color buffer writes
	gl_ulFlags &= ~GLF_D3D_COLORWRITES;
	if( d3dCaps.PrimitiveMiscCaps & D3DPMISCCAPS_COLORWRITEENABLE) gl_ulFlags |= GLF_D3D_COLORWRITES;

	// determine support for custom clip planes
	gl_ulFlags &= ~GLF_D3D_CLIPPLANE;
	if( d3dCaps.MaxUserClipPlanes>0) gl_ulFlags |= GLF_D3D_CLIPPLANE;
	else CPrintF( TRANS("User clip plane not supported - mirrors will not work well.\n"));

	// determine support for texture LOD biasing
	gl_fMaxTextureLODBias = 0.0f;
	if( d3dCaps.RasterCaps & D3DPRASTERCAPS_MIPMAPLODBIAS) {
		gl_fMaxTextureLODBias = 4.0f;
	}

	// determine support for anisotropic filtering
	gl_iMaxTextureAnisotropy = 1;
	if( d3dCaps.RasterCaps & D3DPRASTERCAPS_ANISOTROPY) {
		gl_iMaxTextureAnisotropy = d3dCaps.MaxAnisotropy;
		ASSERT( gl_iMaxTextureAnisotropy>1); 
	}

	// determine support for z-biasing
	gl_ulFlags &= ~GLF_D3D_ZBIAS;
	if( d3dCaps.RasterCaps & D3DPRASTERCAPS_ZBIAS) gl_ulFlags |= GLF_D3D_ZBIAS;

	// check support for vsync swapping
	gl_ulFlags &= ~GLF_VSYNC;
	if( d3dCaps.PresentationIntervals & D3DPRESENT_INTERVAL_IMMEDIATE) {
		if( d3dCaps.PresentationIntervals & D3DPRESENT_INTERVAL_ONE) gl_ulFlags |= GLF_VSYNC;  
	} else CPrintF( TRANS("  Vertical syncronization cannot be disabled.\n"));

	// determine support for vertex shader (i.e. program)
	// Date : 2006-05-16(오후 4:48:55), By eons
	gl_ulFlags &= ~GLF_VERTEXPROGRAM;
	if( _pGfx->gl_pd3dCaps.MaxStreams>=8 && _pGfx->gl_pd3dCaps.VertexShaderVersion>=0x0101 && _pGfx->gl_pd3dCaps.MaxVertexShaderConst>=96 ) {
		gl_ulFlags |= GLF_VERTEXPROGRAM;
	}

	// determine support for pixel shader
	gl_ulFlags &= ~GLF_PIXELPROGRAM;
	if( d3dCaps.PixelShaderVersion>=0x0101 && d3dCaps.MaxPixelShaderValue>=1) {
		gl_ulFlags |= GLF_PIXELPROGRAM;
	}

	BOOL bPS14 = TRUE;

	if (d3dCaps.PixelShaderVersion < D3DPS_VERSION(1,4))
	{ // ps 1.4가 지원 되지 않는다.
		bPS14 = FALSE;
	}

	// determine support for N-Patches
	extern INDEX truform_iLevel;
	extern BOOL  truform_bLinear;
	truform_iLevel  = -1;
	truform_bLinear = FALSE;
	gl_iTessellationLevel    = 0;
	gl_iMaxTessellationLevel = 0;
	if( d3dCaps.DevCaps & D3DDEVCAPS_NPATCHES) {
		if( gl_ctMaxStreams>GFX_MINSTREAMS) {
			gl_iMaxTessellationLevel = 7;
			hr = gl_pd3dDevice->SetRenderState( D3DRS_PATCHEDGESTYLE, D3DPATCHEDGE_DISCRETE);
			D3D_CHECKERROR(hr);
		} else CPrintF( TRANS("Not enough streams - N-Patches cannot be used.\n"));
	}

	// determine support for multi-texturing (only if Modulate2X mode is supported!)
	gl_ctTextureUnits = 1;
	gl_ctRealTextureUnits = d3dCaps.MaxSimultaneousTextures;
	if( gl_ctRealTextureUnits>1) {
		// check everything that is required for multi-texturing
		if( !(d3dCaps.TextureOpCaps&D3DTOP_MODULATE2X)) CPrintF( TRANS("Texture operation MODULATE2X missing - multi-texturing cannot be used.\n"));
		else if( gl_ctMaxStreams<=GFX_MINSTREAMS)       CPrintF( TRANS("Not enough streams - multi-texturing cannot be used.\n"));
		else gl_ctTextureUnits = Min( GFX_USETEXUNITS, Min( gl_ctRealTextureUnits, 1+gl_ctMaxStreams-GFX_MINSTREAMS));
	}

	// disable texturing
	for( INDEX iUnit=0; iUnit<gl_ctRealTextureUnits; iUnit++) {
		GFX_abTexture[iUnit] = FALSE;
		GFX_iTexModulation[iUnit] = 1;
		hr = gl_pd3dDevice->SetTexture( iUnit, NULL);                                      D3D_CHECKERROR(hr);
		hr = gl_pd3dDevice->SetTextureStageState( iUnit, D3DTSS_COLOROP, D3DTOP_DISABLE);  D3D_CHECKERROR(hr); 
		hr = gl_pd3dDevice->SetTextureStageState( iUnit, D3DTSS_ALPHAOP, D3DTOP_MODULATE); D3D_CHECKERROR(hr); 
	}

	// setup fog and haze textures
	extern PIX _fog_pixSizeH;
	extern PIX _fog_pixSizeL;
	extern PIX _haze_pixSize;
	_fog_ulTexture  = NONE;
	_haze_ulTexture = NONE;
	_fog_pixSizeH = 0;
	_fog_pixSizeL = 0;
	_haze_pixSize = 0;

	// prepare pattern texture
	extern CTexParams _tpPattern;
	extern ULONG _ulPatternTexture;
	extern ULONG _ulLastUploadedPattern;
	_ulPatternTexture = NONE;
	_ulLastUploadedPattern = 0;
	_tpPattern.Clear();

	// determine number of color/texcoord buffers
	gl_ctColBuffers = 1;
	gl_ctTexBuffers = gl_ctTextureUnits;
	INDEX ctStreamsRemain = gl_ctMaxStreams - (GFX_MINSTREAMS-1+gl_ctTextureUnits); // -1 because of 1 texture unit inside MinStreams
	while( ctStreamsRemain>0) {
		if( gl_ctTexBuffers==GFX_MAXLAYERS && gl_ctColBuffers==GFX_MAXLAYERS) break;  // done if no need for more streams
		(gl_ctColBuffers<gl_ctTexBuffers) ? gl_ctColBuffers++ : gl_ctTexBuffers++;    // increase number of tex or color buffers
		ctStreamsRemain--;  // advance to next stream
	}

	// prepare vertex arrays
	INDEX i;
	gl_pd3dIdx = NULL;
	gl_pd3dVtx = NULL;
	gl_pd3dNor = NULL;
	gl_pd3dWgh = NULL;
	for( i=0; i<GFX_MAXLAYERS; i++) gl_pd3dCol[i] = gl_pd3dTex[i] = NULL;
	ASSERT( gl_ctTexBuffers>0 && gl_ctTexBuffers<=GFX_MAXLAYERS);
	ASSERT( gl_ctColBuffers>0 && gl_ctColBuffers<=GFX_MAXLAYERS);
	gl_ctVertices = 0;
	gl_ctIndices  = 0;
	extern INDEX d3d_iVertexBuffersSize;
	extern INDEX _iLastVertexBufferSize;
	d3d_iVertexBuffersSize = (d3d_iVertexBuffersSize+3) & (~3); // round to 4
	d3d_iVertexBuffersSize = Clamp( d3d_iVertexBuffersSize, 64L, 4096L);
	const INDEX ctVertices = VerticesFromSize_D3D(d3d_iVertexBuffersSize);
	_iLastVertexBufferSize = d3d_iVertexBuffersSize;
	SetupVertexArrays_D3D(ctVertices); 
	SetupIndexArray_D3D(2*ctVertices);
	// init vertex buffers
	extern void InitVertexBuffers(void);
	InitVertexBuffers();

	// reset texture filtering and some static vars
	for( i=0; i<GFX_MAXTEXUNITS; i++) _tpGlobal[i].Clear();
	_avsFixedShaders.Clear();
	_iVtxOffset = 0;
	_iIdxOffset = 0;
	_dwCurrentVS = NONE;
	_dwCurrentPS = NONE;
	_dwLastVertexProgram = NONE;
	GFX_ctVertices = 0;

	// set default texture filtering/biasing
	extern INDEX gap_iTextureFiltering;
	extern INDEX gap_iTextureAnisotropy;
	extern FLOAT gap_fTextureLODBias;
	gfxSetTextureFiltering( gap_iTextureFiltering, gap_iTextureAnisotropy);
	gfxSetTextureBiasing( gap_fTextureLODBias);

	// generate occlusion query IDs
	_puiOcclusionQueryIDs = (UINT*)AllocMemory( GFX_MAXOCCQUERIES*sizeof(UINT));
	for( INDEX iID=0; iID<GFX_MAXOCCQUERIES; iID++) _puiOcclusionQueryIDs[iID] = iID;

	// mark pretouching and probing
	extern BOOL _bNeedPretouch;
	_bNeedPretouch = TRUE;
	gl_bAllowProbing = FALSE;

	// update console system vars
	extern void UpdateGfxSysCVars(void);
	UpdateGfxSysCVars();

	// reload all loaded textures and eventually shadowmaps
	extern INDEX shd_bCacheAll;
	extern void ReloadTextures(void);
	extern void ReloadMeshes(void);
	extern void CacheShadows(void);
	ReloadTextures();
	ReloadMeshes();
	if( shd_bCacheAll) CacheShadows();

//안태훈 수정 시작	//(Add & Modify SSSE Effect)(0.1)
//	Initialize_EffectSystem();
//안태훈 수정 끝	//(Add & Modify SSSE Effect)(0.1)
}



// HELPER: returns number of bits for depth buffer format
static INDEX BitsFromDepthFormat_D3D( const D3DFORMAT d3df)
{
	switch(d3df) {
	case D3DFMT_D16:      return 16;
	case D3DFMT_D15S1:    return 15 | 0x80000000;
	case D3DFMT_D32:      return 32;
	case D3DFMT_D24X8:    return 24;
	case D3DFMT_D24S8:    return 24 | 0x80000000;
	case D3DFMT_D24X4S4:  return 24 | 0x80000000;
	case D3DFMT_D16_LOCKABLE:  return 16;
	default: return 0;
	}
}


// find depth buffer format (for specified color format) that closest matches required bit depth
// (returns new z-depth bits, and flag if stencil buffer is supported - highest bit in depth-bits!)
static D3DFORMAT FindDepthFormat_D3D( INDEX iAdapter, D3DFORMAT d3dfColor, INDEX iDepthBits)
{
	// adjust required Z-depth from color depth if needed
	if( iDepthBits==0) {
		if( d3dfColor==D3DFMT_X8R8G8B8 || d3dfColor==D3DFMT_A8R8G8B8) iDepthBits = 32;
		else iDepthBits = 16;
	}

	// tries' tables
	const  INDEX ctTries = 7;
	static D3DFORMAT ad3df16BitsTable[] = { D3DFMT_D16,   D3DFMT_D15S1, D3DFMT_D16_LOCKABLE, D3DFMT_D32,     D3DFMT_D24X8, D3DFMT_D24S8, D3DFMT_D24X4S4      };
	static D3DFORMAT ad3df24BitsTable[] = { D3DFMT_D24X8, D3DFMT_D24S8, D3DFMT_D24X4S4,      D3DFMT_D32,     D3DFMT_D16,   D3DFMT_D15S1, D3DFMT_D16_LOCKABLE };
	static D3DFORMAT ad3df32BitsTable[] = { D3DFMT_D32,   D3DFMT_D24X8, D3DFMT_D24S8,        D3DFMT_D24X4S4, D3DFMT_D16,   D3DFMT_D15S1, D3DFMT_D16_LOCKABLE };

	// find corresponding table based on depth bits
	D3DFORMAT *pd3dfDepthTable = &ad3df32BitsTable[0];
	if( iDepthBits<21) pd3dfDepthTable = &ad3df16BitsTable[0];
	else if( iDepthBits<28) pd3dfDepthTable = &ad3df24BitsTable[0];

//강동민 수정 시작
	/*
	// D3DFMT_D24S8로 설정함.
	D3DFORMAT d3dfDepth = pd3dfDepthTable[2];
	HRESULT hr;
	hr = _pGfx->gl_pD3D->CheckDeviceFormat( iAdapter, d3dDevType, d3dfColor, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, d3dfDepth);
	//D3D_CheckError(hr);

	hr = _pGfx->gl_pD3D->CheckDepthStencilMatch( iAdapter, d3dDevType, d3dfColor, d3dfColor, d3dfDepth);
	if( hr==D3D_OK) 
		return d3dfDepth; // done if found
		*/

	// 원본.
	// loop thru table
	for( INDEX i=0; i<ctTries; i++)
	{ // fetch format from table
		HRESULT hr;
		D3DFORMAT d3dfDepth = pd3dfDepthTable[i];
		hr = _pGfx->gl_pD3D->CheckDeviceFormat( iAdapter, d3dDevType, d3dfColor, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, d3dfDepth);
		if( hr!=D3D_OK) continue; // skip if format not supported at all
		hr = _pGfx->gl_pD3D->CheckDepthStencilMatch( iAdapter, d3dDevType, d3dfColor, d3dfColor, d3dfDepth);
		if( hr==D3D_OK) return d3dfDepth; // done if found
	}
//강동민 수정 끝

	// not found :(
	iDepthBits = 0;
	ASSERT( "FindDepthFormat_D3D: Not found?!" );
	return D3DFMT_UNKNOWN; 
}


// prepare display mode
BOOL CGfxLibrary::InitDisplay_D3D( INDEX iAdapter, PIX pixSizeI, PIX pixSizeJ, enum DisplayDepth eColorDepth)
{
	// reset
	HRESULT hr;
	D3DDISPLAYMODE d3dDisplayMode;
	D3DPRESENT_PARAMETERS d3dPresentParams;
	gl_pD3D->GetAdapterDisplayMode( iAdapter, &d3dDisplayMode);
	memset( &d3dPresentParams, 0, sizeof(d3dPresentParams));

	// readout device capabilities
	D3DCAPS8 d3dCaps;
	hr = gl_pD3D->GetDeviceCaps( iAdapter, d3dDevType, &d3dCaps);
	D3D_CHECKERROR(hr);

	// clamp depth/stencil values
	extern INDEX gap_iDepthBits;
			 if( gap_iDepthBits<12) gap_iDepthBits = 0;
	else if( gap_iDepthBits<21) gap_iDepthBits = 16;
	else if( gap_iDepthBits<28) gap_iDepthBits = 24;
	else                        gap_iDepthBits = 32;

	// prepare  
	INDEX iZDepth = gap_iDepthBits;
	D3DFORMAT d3dDepthFormat = D3DFMT_UNKNOWN;
	D3DFORMAT d3dColorFormat = d3dDisplayMode.Format;
	d3dPresentParams.BackBufferCount = 1;
	d3dPresentParams.MultiSampleType = D3DMULTISAMPLE_NONE; // !!!! TODO
	d3dPresentParams.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;
	d3dPresentParams.SwapEffect = D3DSWAPEFFECT_COPY;
	const BOOL bFullScreen = (pixSizeI>0 && pixSizeJ>0); 

	// setup for full screen
	if( bFullScreen) {
		// determine color and depth format
		if( eColorDepth==DISPD_16BIT) d3dColorFormat = D3DFMT_R5G6B5;
		if( eColorDepth==DISPD_32BIT) d3dColorFormat = D3DFMT_X8R8G8B8;
		d3dDepthFormat = FindDepthFormat_D3D( iAdapter, d3dColorFormat, iZDepth);
		iZDepth = BitsFromDepthFormat_D3D(d3dDepthFormat);

		// determine refresh rate and presentation interval
		extern INDEX gap_iRefreshRate;
		const UINT uiRefresh = gap_iRefreshRate>0 ? gap_iRefreshRate : D3DPRESENT_RATE_DEFAULT;

		// determine presentation interval
		extern INDEX gap_iSwapInterval;
		gap_iSwapInterval = Clamp( gap_iSwapInterval, 0L, 3L);
		// check if supported
		const UINT uiSupportedIntervals = d3dCaps.PresentationIntervals;
		if( gap_iSwapInterval==3 && !(uiSupportedIntervals & D3DPRESENT_INTERVAL_THREE))     gap_iSwapInterval = 2;
		if( gap_iSwapInterval==2 && !(uiSupportedIntervals & D3DPRESENT_INTERVAL_TWO))       gap_iSwapInterval = 1;
		if( gap_iSwapInterval==1 && !(uiSupportedIntervals & D3DPRESENT_INTERVAL_ONE))       gap_iSwapInterval = 0;
		if( gap_iSwapInterval==0 && !(uiSupportedIntervals & D3DPRESENT_INTERVAL_IMMEDIATE)) gap_iSwapInterval = 1;
		const UINT uiInterval = _auiSwapIntervals[gap_iSwapInterval];
		gl_iSwapInterval = gap_iSwapInterval;  // copy to gfx lib

		// set context directly to main window
		d3dPresentParams.Windowed = FALSE;
		d3dPresentParams.BackBufferWidth  = pixSizeI;
		d3dPresentParams.BackBufferHeight = pixSizeJ;
		d3dPresentParams.BackBufferFormat = d3dColorFormat;
		d3dPresentParams.EnableAutoDepthStencil = TRUE;
		d3dPresentParams.AutoDepthStencilFormat = d3dDepthFormat;
		d3dPresentParams.FullScreen_RefreshRateInHz = uiRefresh;
		d3dPresentParams.FullScreen_PresentationInterval = uiInterval;
	}
	// setup for windowed mode
	else {
		// create dummy Direct3D context
		d3dPresentParams.Windowed = TRUE;
		d3dPresentParams.BackBufferWidth  = 8;
		d3dPresentParams.BackBufferHeight = 8;
		d3dPresentParams.BackBufferFormat = d3dColorFormat;
//강동민 수정 시작
		/*
		d3dPresentParams.EnableAutoDepthStencil = TRUE;
		d3dDepthFormat = FindDepthFormat_D3D( iAdapter, d3dColorFormat, iZDepth);
		d3dPresentParams.AutoDepthStencilFormat = d3dDepthFormat;
		*/
		// 원본.
		d3dPresentParams.EnableAutoDepthStencil = FALSE;		
		d3dDepthFormat = FindDepthFormat_D3D( iAdapter, d3dColorFormat, iZDepth);		
//강동민 수정 끝
		iZDepth = BitsFromDepthFormat_D3D(d3dDepthFormat);
		gl_iSwapInterval = -1;
	}

	// determine HW or SW vertex processing
	DWORD dwVP = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	gl_ulFlags &= ~(GLF_D3D_HASHWTNL|GLF_D3D_USINGHWTNL); 
	gl_ctMaxStreams = 16; // software T&L has enough streams
	extern INDEX d3d_bUseHardwareTnL;

	// cannot have HW VP if not supported by HW, right?
	if( d3dCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) {
		gl_ulFlags |= GLF_D3D_HASHWTNL;
		gl_ctMaxStreams = d3dCaps.MaxStreams;
		if( gl_ctMaxStreams<GFX_MINSTREAMS) d3d_bUseHardwareTnL = 0; // cannot use HW T&L if not enough streams
		if( d3d_bUseHardwareTnL) {
			d3d_bUseHardwareTnL = 1; // clamp just in case
			dwVP = D3DCREATE_HARDWARE_VERTEXPROCESSING;
			gl_ulFlags |= GLF_D3D_USINGHWTNL;
		} // no HW T&L 
	} else d3d_bUseHardwareTnL = 0;

	// go for it ...
	extern HWND _hwndMain;
	extern const D3DDEVTYPE d3dDevType;
	hr = gl_pD3D->CreateDevice( iAdapter, d3dDevType, _hwndMain, dwVP, &d3dPresentParams, &gl_pd3dDevice);
//안태훈 수정 시작	//(5th Closed beta)(0.2)
	/*
	IUnknown *pD3DDev = NULL;
	if(S_OK == gl_pd3dDevice->QueryInterface(IID_IDirect3DDevice8, (void**)&pD3DDev))
	{
		if(pD3DDev != gl_pd3dDevice)
		{
			ASSERTALWAYS("D3D Device 생성시 문제 생김.");
			ExitProcess(1);
			return FALSE;
		}
	}
	*/
//안태훈 수정 끝	//(5th Closed beta)(0.2)
	if( hr!=D3D_OK) return FALSE;
	gl_d3dColorFormat = d3dColorFormat;
	gl_d3dDepthFormat = d3dDepthFormat;
	gl_ctDepthBits = iZDepth & 0x7FFFFFF; // clamp stencil presence flag
//강동민 수정 시작
	// 원본
	if( iZDepth & 0x8000000) 
		gl_ulFlags |= GLF_STENCILBUFFER; 
	else 
		gl_ulFlags &= ~GLF_STENCILBUFFER;
	/*
	gl_ulFlags |= GLF_STENCILBUFFER; 
	*/
//강동민 수정 끝

	// sehan
	d3d_bDeviceChanged = TRUE;
	// sehan end

	// done
	return TRUE;
}
		



// fallback D3D internal format
// (reverts to next format that closely matches requied one)
static D3DFORMAT FallbackFormat_D3D( D3DFORMAT eFormat, BOOL b2ndTry)
{
	switch( eFormat) {
	case D3DFMT_X8R8G8B8: return !b2ndTry ? D3DFMT_A8R8G8B8 : D3DFMT_R5G6B5;
	case D3DFMT_X1R5G5B5: return !b2ndTry ? D3DFMT_R5G6B5   : D3DFMT_A1R5G5B5;
	case D3DFMT_R5G6B5:   return !b2ndTry ? D3DFMT_X1R5G5B5 : D3DFMT_A1R5G5B5;
	case D3DFMT_L8:       return !b2ndTry ? D3DFMT_A8L8     : D3DFMT_X8R8G8B8;
	case D3DFMT_A8:       return !b2ndTry ? D3DFMT_A8L8     : D3DFMT_A8R8G8B8;
	case D3DFMT_A8L8:     return D3DFMT_A8R8G8B8;
	case D3DFMT_A1R5G5B5: return D3DFMT_A4R4G4B4;
	case D3DFMT_A8R8G8B8: return D3DFMT_A4R4G4B4;
	case D3DFMT_DXT1:     return D3DFMT_A1R5G5B5;
	case D3DFMT_DXT3:     return D3DFMT_A4R4G4B4;
	case D3DFMT_DXT5:     return D3DFMT_A4R4G4B4;
	case D3DFMT_A4R4G4B4: // must have this one!
	default: ASSERTALWAYS( "Can't fallback texture format.");
	} // missed!
	return D3DFMT_UNKNOWN;
}


// find closest 
extern D3DFORMAT FindClosestFormat_D3D( D3DFORMAT d3df)
{
	FOREVER {
		if( HasTextureFormat_D3D(d3df)) return d3df;
		D3DFORMAT d3df2 = FallbackFormat_D3D( d3df, FALSE);
		if( HasTextureFormat_D3D(d3df2)) return d3df2;
		d3df = FallbackFormat_D3D( d3df, TRUE);
	}
}



// VERTEX/INDEX BUFFERS SUPPORT THRU STREAMS


// DEBUG helper
static void CheckStreams(void)
{
	return;
	UINT uiRet, ui;
	INDEX iRef, iPass;
	DWORD dwVS;
	HRESULT hr;
	LPDIRECT3DVERTEXBUFFER8 pVBRet, pVB;
	const LPDIRECT3DDEVICE8 pd3dDev = _pGfx->gl_pd3dDevice;

	// check passes and buffer position
	ASSERT( _iTexPass>=0 && _iColPass>=0);
	ASSERT( _iVtxPos >=0 && _iVtxPos<65536);

	// check vertex positions
	ASSERT( _ulStreamsMask & (1<<GFX_POSIDX)); // must be in shader!
	hr = pd3dDev->GetStreamSource( GFX_POSIDX, &pVBRet, &uiRet);
	D3D_CHECKERROR(hr);
	ASSERT( pVBRet!=NULL);
	iRef = pVBRet->Release();
	ASSERT( iRef==1 && pVBRet==_pGfx->gl_pd3dVtx && uiRet==GFX_POSSIZE);

	// check normals
	pVB = NULL;
	ui  = GFX_NORSIZE;
	hr  = pd3dDev->GetStreamSource( GFX_NORIDX, &pVBRet, &uiRet);
	D3D_CHECKERROR(hr);
	if( pVBRet!=NULL) iRef = pVBRet->Release();
	if( _ulStreamsMask & (1<<GFX_NORIDX)) pVB = _pGfx->gl_pd3dNor;
	ASSERT( iRef==1 && pVBRet==pVB && (uiRet==ui || uiRet==0));

	// check weights
	pVB = NULL;
	ui  = GFX_WGHSIZE;
	hr  = pd3dDev->GetStreamSource( GFX_WGHIDX, &pVBRet, &uiRet);
	D3D_CHECKERROR(hr);
	if( pVBRet!=NULL) iRef = pVBRet->Release();
	if( _ulStreamsMask & (1<<GFX_WGHIDX)) pVB = _pGfx->gl_pd3dWgh;
	ASSERT( iRef==1 && pVBRet==pVB && (uiRet==ui || uiRet==0));

	// check colors
	pVB = NULL;
	ui  = GFX_COLSIZE;
	hr  = pd3dDev->GetStreamSource( GFX_COLIDX, &pVBRet, &uiRet);
	D3D_CHECKERROR(hr);
	if( pVBRet!=NULL) iRef = pVBRet->Release();
	if( _ulStreamsMask & (1<<GFX_COLIDX)) {
		iPass = (_iColPass-1) % _pGfx->gl_ctColBuffers;
		pVB = _pGfx->gl_pd3dCol[iPass];
	}
	if( !GFX_bColorArray) pVBRet = pVB; // force OK if disabled!
	ASSERT( iRef==1 && pVBRet==pVB && (uiRet==ui || uiRet==0));

	/* check 1st texture coords
	pVB = NULL;
	ui  = _bProjectiveMapping ? GFX_TX4SIZE : GFX_TEXSIZE;
	hr  = pd3dDev->GetStreamSource( TEXIDX, &pVBRet, &uiRet);
	D3D_CHECKERROR(hr);
	if( pVBRet!=NULL) iRef = pVBRet->Release();
	if( _ulStreamsMask & (1<<(TEXIDX))) {
		iPass = (_iTexPass-1) % _pGfx->gl_ctTexBuffers;
		pVB = _pGfx->gl_pd3dTex[iPass];
	}
	if( !GFX_bColorArray) pVBRet = pVB; // force OK if disabled!
	ASSERT( iRef==1 && pVBRet==pVB && (uiRet==ui || uiRet==0)); */

	// check indices
	LPDIRECT3DINDEXBUFFER8 pIBRet;
	hr = pd3dDev->GetIndices( &pIBRet, &uiRet);
	D3D_CHECKERROR(hr);
	ASSERT( pIBRet!=NULL);
	iRef = pIBRet->Release();
	ASSERT( iRef==1 && pIBRet==_pGfx->gl_pd3dIdx);

	// check shader
	hr = pd3dDev->GetVertexShader( &dwVS);
	D3D_CHECKERROR(hr);
	ASSERT( dwVS!=NONE && dwVS==_pGfx->gl_dwVertexShader);

#pragma message(">> Check pixel shader")
	/* check shader declaration (SEEMS LIKE THIS SHIT DOESN'T WORK!)
	const INDEX ctMaxDecls = 2*MAXSTREAMS+1;
	INDEX ctDecls = ctMaxDecls;
	DWORD adwDeclRet[ctMaxDecls];
	hr = pd3dDev->GetVertexShaderDeclaration( _pGfx->gl_dwVertexShader, (void*)&adwDeclRet[0], (DWORD*)&ctDecls);
	D3D_CHECKERROR(hr);
	ASSERT( ctDecls>0 && ctDecls<ctMaxDecls);
	INDEX iRet = memcmp( &adwDeclRet[0], &_adwCurrentDecl[0], ctDecls*sizeof(DWORD));
	ASSERT( iRet==0); */
}



// prepare vertex array for D3D
extern inline void *LockVertexArray_D3D( const INDEX ctVertices)
{
	// make sure that we have enough space in vertex buffers
	ASSERT( ctVertices>0 && ctVertices<65536);
	GFX_ctVertices = ClampUp( ctVertices, 65535L);
	if( GFX_ctVertices>_pGfx->gl_ctVertices) SetupVertexArrays_D3D(GFX_ctVertices);
 _bUsingDynamicBuffer = TRUE;  // signal the rendering from dynamic buffer

	// determine lock type
	const BOOL bHWTnL = _pGfx->gl_ulFlags & GLF_D3D_USINGHWTNL;
	if( (_iVtxOffset+GFX_ctVertices)>=_pGfx->gl_ctVertices || !bHWTnL) {
		// reset position and flag (eventually)
		_iVtxOffset = 0; 
		_dwVtxLockFlag = D3DLOCK_DISCARD;
		// signal that to texcoord and color buffers
		for( INDEX i=0; i<GFX_MAXLAYERS; i++) _dwColLockFlags[i] = _dwTexLockFlags[i] = _dwVtxLockFlag;
	}
	// just proceed to next buffer portion
	else _dwVtxLockFlag = D3DLOCK_NOOVERWRITE;

	// keep current lock position
	_iVtxPos = _iVtxOffset;

	// reset array states
	_ulStreamsMask = NONE;
	_bProjectiveMapping = FALSE;
	_iTexPass = _iColPass = 0;
	ASSERT( _iVtxPos>=0 && _iVtxPos<65536);
	HRESULT hr;

	// update vertex base offset
	if( bHWTnL) {
		hr = _pGfx->gl_pd3dDevice->SetIndices( _pGfx->gl_pd3dIdx, _iVtxPos);
		D3D_CHECKERROR(hr);
	}

	// update streams mask and assign buffer
	_ulStreamsMask |= 1<<GFX_POSIDX;
	hr = _pGfx->gl_pd3dDevice->SetStreamSource( GFX_POSIDX, _pGfx->gl_pd3dVtx, GFX_POSSIZE);
	D3D_CHECKERROR(hr);

	// fetch and return D3D buffer
	void *pLockedBuffer;
	hr = _pGfx->gl_pd3dVtx->Lock( _iVtxPos*GFX_POSSIZE, GFX_ctVertices*GFX_POSSIZE, (UBYTE**)&pLockedBuffer, _dwVtxLockFlag);
	D3D_CHECKERROR(hr);

	// done
	_pd3dLockedVtx = _pGfx->gl_pd3dVtx;
	return pLockedBuffer;
}


// unlock array
extern inline void UnlockVertexArray_D3D(void)
{
	ASSERT( _pd3dLockedVtx!=NULL && _bUsingDynamicBuffer);
	HRESULT hr = _pd3dLockedVtx->Unlock();
	D3D_CHECKERROR(hr);
	_pd3dLockedVtx = NULL;
	// advance to next available lock position
	_iVtxOffset += GFX_ctVertices;
}


// prepare normal array for D3D
extern inline void *LockNormalArray_D3D(void)
{
	ASSERT( _bUsingDynamicBuffer);
	ASSERT( _iTexPass>=0 && _iColPass>=0);
	ASSERT( _iVtxPos >=0 && _iVtxPos <65536);
	ASSERT( _iTexPass <2 && _iColPass<2);  // normals must be set in 1st pass (completed or not)
	HRESULT hr;

	// update streams mask and assign buffer
	_ulStreamsMask |= 1<<GFX_NORIDX;
	hr = _pGfx->gl_pd3dDevice->SetStreamSource( GFX_NORIDX, _pGfx->gl_pd3dNor, GFX_NORSIZE);
	D3D_CHECKERROR(hr);

	// fetch and return D3D buffer
	void *pLockedBuffer;
	hr = _pGfx->gl_pd3dNor->Lock( _iVtxPos*GFX_NORSIZE, GFX_ctVertices*GFX_NORSIZE, (UBYTE**)&pLockedBuffer, _dwVtxLockFlag);
	D3D_CHECKERROR(hr);

	// done
	_pd3dLockedNor = _pGfx->gl_pd3dNor;
	return pLockedBuffer;
}

	
// unlock array
extern inline void UnlockNormalArray_D3D(void)
{
	ASSERT( _pd3dLockedNor!=NULL && _bUsingDynamicBuffer);
	HRESULT hr = _pd3dLockedNor->Unlock();
	D3D_CHECKERROR(hr);
 _pd3dLockedNor = NULL;
}

//안태훈 수정 시작	//(Add Tagent-space Normal Map)(0.1)
extern inline void *LockTangentArray_D3D(void)
{
  ASSERT( _bUsingDynamicBuffer);
  ASSERT( _iTexPass>=0 && _iColPass>=0);
  ASSERT( _iVtxPos >=0 && _iVtxPos <65536);
  ASSERT( _iTexPass <2 && _iColPass<2);  // normals must be set in 1st pass (completed or not)
  HRESULT hr;

  // update streams mask and assign buffer
  _ulStreamsMask |= 1<<GFX_TANIDX;
  hr = _pGfx->gl_pd3dDevice->SetStreamSource( GFX_REAL_TANIDX, _pGfx->gl_pd3dTan, GFX_TANSIZE);
  D3D_CHECKERROR(hr);

  // fetch and return D3D buffer
  void *pLockedBuffer;
  hr = _pGfx->gl_pd3dTan->Lock( _iVtxPos*GFX_TANSIZE, GFX_ctVertices*GFX_TANSIZE, (UBYTE**)&pLockedBuffer, _dwVtxLockFlag);
  D3D_CHECKERROR(hr);

  // done
  _pd3dLockedTan = _pGfx->gl_pd3dTan;
  return pLockedBuffer;
}

  
// unlock array
extern inline void UnlockTangentArray_D3D(void)
{
  ASSERT( _pd3dLockedTan!=NULL && _bUsingDynamicBuffer);
  HRESULT hr = _pd3dLockedTan->Unlock();
  D3D_CHECKERROR(hr);
 _pd3dLockedTan = NULL;
}
//안태훈 수정 끝	//(Add Tagent-space Normal Map)(0.1)


// prepare weight array for D3D
extern inline void *LockWeightArray_D3D(void)
{
	ASSERT( _bUsingDynamicBuffer);
	ASSERT( _iTexPass>=0 && _iColPass>=0);
	ASSERT( _iVtxPos >=0 && _iVtxPos <65536);
	ASSERT( _iTexPass <2 && _iColPass<2);  // weights must be set in 1st pass (completed or not)
	HRESULT hr;

	// update streams mask and assign buffer
	_ulStreamsMask |= 1<<GFX_WGHIDX;
	hr = _pGfx->gl_pd3dDevice->SetStreamSource( GFX_WGHIDX, _pGfx->gl_pd3dWgh, GFX_WGHSIZE);
	D3D_CHECKERROR(hr);

	// fetch and return D3D buffer
	void *pLockedBuffer;
	hr = _pGfx->gl_pd3dWgh->Lock( _iVtxPos*GFX_WGHSIZE, GFX_ctVertices*GFX_WGHSIZE, (UBYTE**)&pLockedBuffer, _dwVtxLockFlag);
	D3D_CHECKERROR(hr);

	// done
	_pd3dLockedWgh = _pGfx->gl_pd3dWgh;
	return pLockedBuffer;
}

	
// unlock array
extern inline void UnlockWeightArray_D3D(void)
{
	ASSERT( _pd3dLockedWgh!=NULL && _bUsingDynamicBuffer);
	HRESULT hr = _pd3dLockedWgh->Unlock();
	D3D_CHECKERROR(hr);
 _pd3dLockedWgh = NULL;
}



// prepare color array for D3D
extern inline void *LockColorArray_D3D(void)
{
	ASSERT( _bUsingDynamicBuffer);
	INDEX iThisPass = _iColPass;
	DWORD dwLockFlag = _dwColLockFlags[iThisPass];  // assume enough buffers by default
	// restart in case of too many passes
	if( iThisPass>=_pGfx->gl_ctColBuffers) {
		dwLockFlag = D3DLOCK_DISCARD;
		iThisPass %= _pGfx->gl_ctColBuffers;
	}
	// mark
	_dwColLockFlags[iThisPass] = D3DLOCK_NOOVERWRITE;
	ASSERT( iThisPass>=0 && iThisPass<_pGfx->gl_ctColBuffers);
	ASSERT( _iTexPass>=0 && _iColPass>=0);
	ASSERT( _iVtxPos >=0 && _iVtxPos<65536);
	_iColPass++; // advance to next color pass

	HRESULT hr;
	LPDIRECT3DVERTEXBUFFER8 pd3dVB = _pGfx->gl_pd3dCol[iThisPass];

	// update streams mask and assign buffer
	_ulStreamsMask |= 1<<GFX_COLIDX;
	hr = _pGfx->gl_pd3dDevice->SetStreamSource( GFX_COLIDX, pd3dVB, GFX_COLSIZE);
	D3D_CHECKERROR(hr);

	// fetch and return D3D buffer
	void *pLockedBuffer;
	hr = pd3dVB->Lock( _iVtxPos*GFX_COLSIZE, GFX_ctVertices*GFX_COLSIZE, (UBYTE**)&pLockedBuffer, dwLockFlag);
	D3D_CHECKERROR(hr);

	// done
	_pd3dLockedCol = pd3dVB;
	return pLockedBuffer;
}


// unlock array
extern inline void UnlockColorArray_D3D(void)
{
	ASSERT( _pd3dLockedCol!=NULL && _bUsingDynamicBuffer);
	HRESULT hr = _pd3dLockedCol->Unlock();
	D3D_CHECKERROR(hr);
 _pd3dLockedCol = NULL;
}


// prepare texture coordinates array for D3D
extern inline void *LockTexCoordArray_D3D( const BOOL bProjectiveMapping/*=FALSE*/)
{
	ASSERT( _bUsingDynamicBuffer);
	SLONG slStride;
	INDEX ctLockSize, iLockOffset;
	INDEX iThisPass = _iTexPass;
	DWORD dwLockFlag = _dwTexLockFlags[iThisPass];  // assume enough buffers by default
	// restart in case of too many passes
	if( iThisPass>=_pGfx->gl_ctTexBuffers) {
		dwLockFlag = D3DLOCK_DISCARD;
		iThisPass %= _pGfx->gl_ctTexBuffers;
	}
	// mark
	_dwTexLockFlags[iThisPass] = D3DLOCK_NOOVERWRITE;
	ASSERT( iThisPass>=0 && iThisPass<_pGfx->gl_ctTexBuffers);
	ASSERT( _iTexPass>=0 && _iColPass>=0);
	ASSERT( _iVtxPos >=0 && _iVtxPos<65536);
	_iTexPass++; // advance to next texture pass

	// determine stride, lock pos and size
	_bProjectiveMapping = bProjectiveMapping;
	const INDEX iStream = GFX_TEXIDX +GFX_iActiveTexUnit;  // must take into account tex-unit, because of multi-texturing!
	if( !bProjectiveMapping) {
		ctLockSize  = GFX_ctVertices*GFX_TEXSIZE; 
		iLockOffset = _iVtxPos*GFX_TEXSIZE; 
		slStride = GFX_TEXSIZE;
	} else {
		ctLockSize  = GFX_ctVertices*GFX_TX4SIZE; 
		iLockOffset = _iVtxPos*GFX_TX4SIZE; 
		slStride = GFX_TX4SIZE;
	}

	HRESULT hr;
	LPDIRECT3DVERTEXBUFFER8 pd3dVB = _pGfx->gl_pd3dTex[iThisPass];

	// update streams mask and assign buffer
	_ulStreamsMask |= 1<<iStream;
	hr = _pGfx->gl_pd3dDevice->SetStreamSource( iStream, pd3dVB, slStride);
	D3D_CHECKERROR(hr);

	// fetch and return D3D buffer
	void *pLockedBuffer;
	hr = pd3dVB->Lock( iLockOffset, ctLockSize, (UBYTE**)&pLockedBuffer, dwLockFlag);
	D3D_CHECKERROR(hr);

	// done
	_pd3dLockedTex = pd3dVB;
	return pLockedBuffer;
}


// unlock array
extern inline void UnlockTexCoordArray_D3D(void)
{
	ASSERT( _pd3dLockedTex!=NULL && _bUsingDynamicBuffer);
	HRESULT hr = _pd3dLockedTex->Unlock();
	D3D_CHECKERROR(hr);
 _pd3dLockedTex = NULL;
}


extern void SetupShaderStreams(void)
{
#if _DEBUG && 0
	const BOOL bPosStream = (_ulStreamsMask>>GFX_POSIDX&1);
	const BOOL bColStream = (_ulStreamsMask>>GFX_COLIDX&1);
	const BOOL bNorStream = (_ulStreamsMask>>GFX_NORIDX&1);
	const BOOL bWghStream = (_ulStreamsMask>>GFX_WGHIDX&1);
	const BOOL bTexStream1 = (_ulStreamsMask>>(GFX_TEXIDX+0)&1);
	const BOOL bTexStream2 = (_ulStreamsMask>>(GFX_TEXIDX+1)&1);
	const BOOL bTexStream3 = (_ulStreamsMask>>(GFX_TEXIDX+2)&1);
	const BOOL bTexStream4 = (_ulStreamsMask>>(GFX_TEXIDX+3)&1);

	const BOOL bLastPosStream = (_ulLastStreamsMask>>GFX_POSIDX&1);
	const BOOL bLastColStream = (_ulLastStreamsMask>>GFX_COLIDX&1);
	const BOOL bLastNorStream = (_ulLastStreamsMask>>GFX_NORIDX&1);
	const BOOL bLastWghStream = (_ulLastStreamsMask>>GFX_WGHIDX&1);
	const BOOL bLastTexStream1 = (_ulLastStreamsMask>>(GFX_TEXIDX+0)&1);
	const BOOL bLastTexStream2 = (_ulLastStreamsMask>>(GFX_TEXIDX+1)&1);
	const BOOL bLastTexStream3 = (_ulLastStreamsMask>>(GFX_TEXIDX+2)&1);
	const BOOL bLastTexStream4 = (_ulLastStreamsMask>>(GFX_TEXIDX+3)&1);

	CTString strStreams;
	CTString strLastStreams;
	strStreams.PrintF("%d,%d,%d,%d,%d,%d,%d,%d",bPosStream,bColStream,bNorStream,bWghStream,
										bTexStream1,bTexStream2,bTexStream3,bTexStream4);
	strLastStreams.PrintF("%d,%d,%d,%d,%d,%d,%d,%d",bLastPosStream,bLastColStream,bLastNorStream,bLastWghStream,
										bLastTexStream1,bLastTexStream2,bLastTexStream3,bLastTexStream4);
#endif

	const LPDIRECT3DDEVICE8 pd3dDev = _pGfx->gl_pd3dDevice;
	// eventually (re)construct vertex shader out of streams' bit-mask
	if( _ulLastStreamsMask != _ulStreamsMask)
	{ // reset streams that were used before
		ULONG ulThisMask = _ulStreamsMask;
		ULONG ulLastMask = _ulLastStreamsMask;
		for( INDEX iStream=0; iStream<MAXSTREAMS; iStream++) {
			if( (ulThisMask&1)==0 && (ulLastMask&1)!=0) {
				HRESULT hr = pd3dDev->SetStreamSource( iStream,NULL,0);
				D3D_CHECKERROR(hr);
			} // next stream
			ulThisMask >>= 1;
			ulLastMask >>= 1;
		}
	}
	// Setup fixed-function vertex shader if last vertex shader was programmable
	// or shader streams are different
	const BOOL bSetupFFShader = (_dwLastVertexProgram!=NONE || _ulLastStreamsMask!=_ulStreamsMask) && (!GFX_bUseVertexProgram);
	if(bSetupFFShader) {
		// Setup fixed function vertex shader
		_dwCurrentVS = SetupShader_D3D(_ulStreamsMask);
	}
	_ulLastStreamsMask = _ulStreamsMask;
}

// Reset all stream sources
extern void ClearStreams(void)
{
	// Make sure that morhed buffer was not last buffer that was used !
	const LPDIRECT3DDEVICE8 pd3dDev = _pGfx->gl_pd3dDevice;
	for( INDEX iStream=0; iStream<MAXSTREAMS; iStream++) {
		HRESULT hr = pd3dDev->SetStreamSource( iStream,NULL,0);
		D3D_CHECKERROR(hr);
	}
}

//안태훈 수정 시작	//(For Performance)(0.2)
#include <Engine/Base/Statistics_Internal.h>
//안태훈 수정 끝	//(For Performance)(0.2)
// prepare and draw arrays
extern void DrawElements_D3D( INDEX ctIndices, const UWORD *puwIndices)
{
	// paranoid & sunburnt (by Skunk Anansie:)
	ASSERT( _iTexPass>=0 && _iColPass>=0);
	ASSERT( _iVtxPos >=0 && _iVtxPos<65536);
	const LPDIRECT3DDEVICE8 pd3dDev = _pGfx->gl_pd3dDevice;

	// at least one triangle must be sent

	ASSERT( ctIndices>=3 && ((ctIndices/3)*3)==ctIndices);
	if( ctIndices<3) return;
	HRESULT hr;

	// clamp indices and eventually adjust size of index buffer
	ctIndices = ClampUp( ctIndices, _pGfx->gl_ctMaxPrimitives);
	if( ctIndices>_pGfx->gl_ctIndices) SetupIndexArray_D3D(ctIndices);

	// determine lock position and type
	DWORD dwLockFlag = D3DLOCK_NOOVERWRITE;
	if( (_iIdxOffset+ctIndices) >= _pGfx->gl_ctIndices) {
		_iIdxOffset = 0;
		dwLockFlag  = D3DLOCK_DISCARD;
	}

	// determine range span usage
	BOOL  bSetRange=FALSE;
	INDEX iVtxStart=0, ctVtxUsed=GFX_ctVertices;
	if( !(_pGfx->gl_ulFlags&GLF_D3D_USINGHWTNL)) {
		extern INDEX d3d_iVertexRangeTreshold;
		d3d_iVertexRangeTreshold = Clamp( d3d_iVertexRangeTreshold, 0L, 9999L);
		bSetRange = (GFX_ctVertices>d3d_iVertexRangeTreshold);
	}

	// copy indices to index buffer
	UWORD *puwLockedBuffer;
	hr = _pGfx->gl_pd3dIdx->Lock( _iIdxOffset*GFX_IDXSIZE, ctIndices*GFX_IDXSIZE, (UBYTE**)&puwLockedBuffer, dwLockFlag);
	D3D_CHECKERROR(hr);

#if ASMOPT == 1

	__asm {
		cld
		mov     esi,D [puwIndices]
		mov     edi,D [puwLockedBuffer]
		mov     ecx,D [ctIndices]
		shr     ecx,1 // will not be 0, since min indices is 3
		rep     movsd
		test    D [ctIndices],1
		jz      elemRange
		movzx   eax,W [esi]
		mov     W [edi],ax

elemRange:
		// find min/max index (if needed)
		cmp     D [bSetRange],0
		jz      elemEnd

		mov     edi,65536
		mov     edx,0
		mov     esi,D [puwIndices]
		mov     ecx,D [ctIndices]
elemTLoop:
		movzx   eax,W [esi]
		cmp     eax,edi
		cmovl   edi,eax
		cmp     eax,edx
		cmovg   edx,eax
		add     esi,2
		dec     ecx
		jnz     elemTLoop
		sub     edx,edi
		inc     edx
		mov     D [iVtxStart],edi
		mov     D [ctVtxUsed],edx
elemEnd:
	}

#else

	INDEX iMaxIndex = 0;
	INDEX iMinIndex = 65536;
	for( INDEX idx=0; idx<ctIndices; idx++) {
		const INDEX iIndex = puwIndices[idx];
		puwLockedBuffer[idx] = iIndex;
		if( !bSetRange) continue;
				 if( iMinIndex>iAdj) iMinIndex = iIndex;
		else if( iMaxIndex<iAdj) iMaxIndex = iIndex;
	}
	// set range?
	if( bSetRange) { 
		iVtxStart = iMinIndex;            
		ctVtxUsed = iMaxIndex-iMinIndex+1;
	}

#endif // ASMOPT

	// indices filled
	hr = _pGfx->gl_pd3dIdx->Unlock();
	D3D_CHECKERROR(hr);

	// no need to do all the vertex-shader and streams mumbo-jumbos if static buffer is used
	if(_bUsingDynamicBuffer) {
		// check whether to use color array or not
		if(GFX_bColorArray) {
			_ulStreamsMask |= (1<<GFX_COLIDX);
		} else {
			_ulStreamsMask &= ~(1<<GFX_COLIDX);
		}
	}

	// Set up shader streams
	SetupShaderStreams();

	if(_bUsingDynamicBuffer)
	{
		// (re)set vertex shader
		ASSERT(_dwCurrentVS!=NONE);
		if( _pGfx->gl_dwVertexShader!=_dwCurrentVS) {
			ASSERT(!GFX_bUseVertexProgram); // must be fixed-function vertex program
			_dwLastVertexProgram = NONE;    // set last programmable vertex shader as none
			ASSERT(_dwCurrentVS!=NONE);
			hr = _pGfx->gl_pd3dDevice->SetVertexShader(_dwCurrentVS);
			D3D_CHECKERROR(hr);
		 _pGfx->gl_dwVertexShader = _dwCurrentVS;
		}

#ifndef NDEBUG
		// Paranoid Android (by Radiohead:)
		CheckStreams();
#endif
	}

	// if not using pixel program
	if(!GFX_bUsePixelProgram) {
		// eventually clear current pixel program
		gfxSetPixelProgram(NONE);
	}

//안태훈 수정 시작	//(For Performance)(0.2)
	_sfStats.IncrementCounter(CStatForm::SCI_DPCOUNT);
//안태훈 수정 끝	//(For Performance)(0.2)
	// draw indices
	ASSERT( ctVtxUsed>0);
	hr = pd3dDev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, iVtxStart, ctVtxUsed, _iIdxOffset, ctIndices/3);
	D3D_CHECKERROR(hr);
	// move to next available lock position
	_iIdxOffset += ctIndices;
}                                                                             




