#include "stdh.h"
#include <Engine/Base/Stream.h>
#include <Engine/Math/Projection.h>
#include <Engine/Math/Matrix12.h>
#include <Engine/Ska/Render.h>
#include <Engine/Graphics/Shader.h>
#include <Engine/Graphics/Texture.h>
#include <Engine/Graphics/Fog_internal.h>
#include <Engine/Base/Statistics_internal.h>
#include <Engine/Base/Console.h>
#include <Engine/Templates/Stock_CShader.h>
#include <Engine/Base/MemoryTracking.h>
#include <Engine/Base/ObjectRestore.h>
//안태훈 수정 시작	//(Add Tagent-space Normal Map)(0.1)
#include <Engine/Math/Geometry.h>
//안태훈 수정 끝	//(Add Tagent-space Normal Map)(0.1)

#pragma message(">> Remove include to d3d")
#undef new
#include "initguid.h"
#include <d3d8.h>
#include <D3dx8core.h>
#pragma comment(lib, "d3dx8.lib")

// asm shortcuts
#define O offset
#define Q qword ptr
#define D dword ptr
#define W  word ptr
#define B  byte ptr


#define ASMOPT 1



static INDEX _iFirstVertex = -1;
static INDEX _ctVertices   = -1;
static INDEX _ctIndices    = -1;
static INDEX _ctTextures   = -1;
static INDEX _ctUVMaps     = -1;
static INDEX _ctColors     = -1;
static INDEX _ctFloats     = -1;
static INDEX _ctLights     = -1;
static BOOL _bPrecachingShaders = FALSE;

static CAnyProjection3D *_paprProjection;  // current projection
static Matrix12 *_pmObjToView = NULL;
static Matrix12 *_pmObjToViewStr = NULL;
static Matrix12 *_pmObjToAbs  = NULL;

static CShader     *_pShader = NULL;   // current shader
static GFXTexCoord *_pCurrentUVMap = NULL; // current UVMap

static GFXVertex   *_paVertices = NULL;   // array of vertices
static GFXNormal   *_paNormals  = NULL;   // array of normals
static GFXWeight   *_paWeights  = NULL;   // array of weights
static GFXTexCoord **_paUVMaps  = NULL;   // array of uvmaps to chose from

//강동민 수정 시작 접속 시퀀스 작업	05.31
extern BOOL _bRenderProjectionShadow;
//강동민 수정 끝 접속 시퀀스 작업	05.31

static INDEX _iVertexBufferID   = -1;     // current vertex buffer ID
static INDEX _iNormalBufferID   = -1;     // current normal buffer ID
//안태훈 수정 시작	//(Add Tagent-space Normal Map)(0.1)
static INDEX _iTangentBufferID   = -1;     // current tangent buffer ID
//안태훈 수정 끝	//(Add Tagent-space Normal Map)(0.1)
static INDEX _iWeightBufferID   = -1;     // current weight buffer ID
static INDEX _iTexCoordBufferID = -1;     // current texcoord buffer ID
static INDEX *_paiUVMapBufferIDs = NULL;   // array of uvmap IDs to chose from

static GFXTexCoord *_paFogUVMap   = NULL;   // UVMap for fog pass
static GFXTexCoord *_paHazeUVMap  = NULL;   // UVMap for haze pass
static GFXColor    *_pacolVtxHaze = NULL;   // array of vertex colors for haze

static CTextureObject **_paTextures = NULL; // array of textures to chose from
static const UWORD *_puwIndices = NULL;     // current array of triangle indices

static COLOR    _colModel   = 0x000000FF;     // Model color
static GFXColor _colLight   = 0x000000FF;     // Light color
static GFXColor _colAmbient = 0x000000FF;     // Ambient color
static FLOAT3D  _vLightDir  = FLOAT3D(0,0,0); // Light direction

static COLOR _colConstant = NULL;  // current set color
static COLOR *_paColors   = NULL;  // array of colors to chose from
static FLOAT *_paFloats   = NULL;  // array of floats to chose from
static ULONG _ulFlags     = 0;     // Shading flags (full bright, double sided, etc)
static ULONG _ulRenFlags  = 0;     // Rendering flags (fog, haze, etc)

// Vertex and pixel programs
static BOOL  _bUseHWVertexProgram = FALSE;
static BOOL  _bUseHWPixelProgram  = FALSE;
static BOOL  _bUseHWVertexBuffers = FALSE;
static INDEX _ctWeightsPerVertex = 0;
static INDEX _iVertexProgram = -1;
static INDEX _iPixelProgram = -1;
static INDEX _iFogTexUnit = -1;
static BOOL  _bOpaqueSurface = TRUE;
// Used texture units
static ULONG _ulUsedTextureUnits = 0;

//강동민 수정 시작 Water 구현		04.21
extern Matrix16 _matWaterProj;
//강동민 수정 끝 Water 구현			04.21

//강동민 수정 시작 접속 시퀀스 작업	05.31
extern BOOL GFX_bClipping;
//강동민 수정 끝 접속 시퀀스 작업	05.31

//강동민 수정 시작 Water 구현		04.20
inline DWORD F2DW( FLOAT f ) { return *((DWORD*)&f); }
//강동민 수정 끝 Water 구현			04.20
//강동민 수정 시작 Water 구현		04.13
//static CNiceWater* _pWaterInformation = NULL;
//강동민 수정 끝 Water 구현			04.13
extern CNiceWater _WaterInformation;

// TEMP!!!!
MeshVertexWeightInfo *_pwiWeights = NULL;
// Matrix12 *_paMatrices = NULL;
static INDEX _ctBoneMatrices = 0;
static Matrix12 const *_pamBoneMatrices[MAX_WEIGHTS_PER_SURFACE];


// Vertex colors
static CStaticStackArray<GFXColor> _acolVtxColors;        // array of color values for each vertex
static CStaticStackArray<GFXColor> _acolVtxModifyColors;  // array of color modified values for each vertex
GFXColor *_pcolVtxColors = NULL;    // pointer to vertex color array (points to current array of vertex colors)

// vertex array that is returned if shader request vertices for modify
static CStaticStackArray<GFXVertex>   _vModifyVertices;
static CStaticStackArray<GFXTexCoord> _uvUVMapForModify;

// Fog and haze
static FLOAT3D _vViewer;
static FLOAT3D _vViewerObj;
static FLOAT3D _vObjPos;
static FLOAT3D _vFViewerObj, _vHDirObj;
static FLOAT   _fFogAddZ, _fFogAddH;
static FLOAT   _fHazeAdd;
static CStaticStackArray<struct GFXColor>    _acolFog;  // fog colors
static CStaticStackArray<struct GFXTexCoord> _atcFog;   // fog tex coords
static CStaticStackArray<struct GFXColor>    _acolHaze; // haze colors
static CStaticStackArray<struct GFXTexCoord> _atcHaze;  // haze tex coords
static CStaticStackArray<UBYTE> _aubshdHaze;
static CStaticStackArray<UBYTE> _aubshdFog;
//강동민 수정 시작 2차 작업			05.14
static D3DXPLANE	_plOld;
//강동민 수정 끝 2차 작업			05.14

static INDEX tempFileNameCount = 0;	// 쉐이더 코드 추출을 위한 변수

// Begin shader using
extern void shaBegin(CAnyProjection3D &aprProjection,CShader *pShader)
{
	// Chech that last shading ended with shaEnd
	ASSERT(_pShader==NULL);
	// Chech if shader exists
	ASSERT(pShader!=NULL);
	// Set current projection
	_paprProjection = &aprProjection;
	// Set pointer to shader
	_pShader = pShader;
}

// End shader using
extern void shaEnd(void)
{
	ASSERT(_pShader!=NULL);
	ASSERT(_pShader->ShaderFunc!=NULL);
//강동민 수정 시작 로그인 처리 작업	07.10
	extern INDEX gfx_bRenderReflection;
//강동민 수정 끝 로그인 처리 작업		07.10
//강동민 수정 시작 2차 작업			05.14	
	const BOOL bHardwareShader	= shaUseHWShaders();
	if(bHardwareShader)
	{
//강동민 수정 시작 접속 시퀀스 작업	05.31
		if(_bRenderProjectionShadow)
		{
			gfxDisableClipPlane();
		}
		else if(!_bRenderProjectionShadow)
//강동민 수정 끝 접속 시퀀스 작업	05.31
		{
//강동민 수정 시작 테스트 클라이언트 작업	06.16
			// if custom clip plane supported
			if((_pGfx->gl_ulFlags&GLF_D3D_CLIPPLANE) && gfx_bRenderReflection)
			{
//강동민 수정 끝 테스트 클라이언트 작업		06.16
			//gfxEnableClipPlane();
			// NOTE : 이전에 저장된 클리핑 평면의 방정식은 월드 공간상의 평면의 방정식이다.
			D3DXPLANE	plNew;
			HRESULT		hr;
			hr = _pGfx->gl_pd3dDevice->GetClipPlane( 0, (float*)&_plOld);			D3D_CHECKERROR(hr);
			
			D3DXMATRIX  matClip;
			D3DXMatrixIdentity(&matClip);
			D3DXMATRIX	matProj;
			D3DXMATRIX	matView;
			hr = _pGfx->gl_pd3dDevice->GetTransform( D3DTS_PROJECTION, &matProj);	D3D_CHECKERROR(hr);
			hr = _pGfx->gl_pd3dDevice->GetTransform( D3DTS_VIEW, &matView);			D3D_CHECKERROR(hr);
			D3DXMatrixMultiply(&matClip, &matView, &matProj);
			
			// Normal값을 변환함.
			D3DXMatrixInverse(&matClip, NULL, &matClip);
			D3DXMatrixTranspose(&matClip, &matClip);
			
			D3DXPlaneTransform(&plNew, &_plOld, &matClip);
			hr = _pGfx->gl_pd3dDevice->SetClipPlane( 0, &plNew[0]);					D3D_CHECKERROR(hr);
//강동민 수정 시작 테스트 클라이언트 작업	06.16
			}
//강동민 수정 끝 테스트 클라이언트 작업		06.16
		}
	}
//강동민 수정 끝 2차 작업			05.14

	// Call shader function
	_pShader->ShaderFunc();

//강동민 수정 시작 2차 작업			05.14
	//if(bClipping)
	//{
	if(bHardwareShader)
	{
//강동민 수정 시작 접속 시퀀스 작업	05.31
		/*
		if(_bRenderProjectionShadow)
		{
			gfxEnableClipPlane();
		}
		else 
		*/
			if(!_bRenderProjectionShadow)
//강동민 수정 끝 접속 시퀀스 작업	05.31
		{
			// NOTE : 이 루틴을 타기전에 이미 SetClipPlane()이 설정되어 있음.
			// if custom clip plane supported.
//강동민 수정 시작 테스트 클라이언트 작업	06.16
			if((_pGfx->gl_ulFlags&GLF_D3D_CLIPPLANE) && gfx_bRenderReflection)
			{
//강동민 수정 끝 테스트 클라이언트 작업		06.16
			HRESULT hr;
			hr = _pGfx->gl_pd3dDevice->SetClipPlane( 0, &_plOld[0]);				D3D_CHECKERROR(hr);
			//gfxDisableClipPlane();
//강동민 수정 시작 테스트 클라이언트 작업	06.16
			}
//강동민 수정 끝 테스트 클라이언트 작업		06.16
		}
	}
//강동민 수정 끝 2차 작업			05.14
	// No active shader
	_pShader = NULL;
}

//안태훈 수정 시작	//(Add Tagent-space Normal Map)(0.1)
/*안태훈 주석 : 필요없음.
static void SetConstantRegisters(void)
{
	#pragma message(">> Prepare projection*view matrix")

	D3DXMATRIX mat;
	D3DXMATRIX matView;
	D3DXMATRIX matProj;
	_pGfx->gl_pd3dDevice->GetTransform(D3DTS_VIEW,&matView);
	_pGfx->gl_pd3dDevice->GetTransform(D3DTS_PROJECTION,&matProj);
	D3DXMatrixMultiply( &mat, &matView, &matProj);
	D3DXMatrixTranspose( &mat, &mat);

	GFXColor colLight   = _colLight;
	GFXColor colAmbient = _colAmbient;
	colAmbient.r = ClampUp((SLONG)colAmbient.r, 127L);
	colAmbient.g = ClampUp((SLONG)colAmbient.g, 127L);
	colAmbient.b = ClampUp((SLONG)colAmbient.b, 127L);

	// if full bright
	if( shaGetFlags() & BASE_FULL_BRIGHT) {
		colLight.abgr = 0xFF000000;
		colAmbient.abgr = 0x7F7F7F7F;
	}

	const ShaderRegister srLightColor(colLight);
	const ShaderRegister srAmbientColor(colAmbient);
	const ShaderRegister srLightDir(_vLightDir(1), _vLightDir(2), _vLightDir(3), 765.01f);
	const ShaderRegister srConstants(0.0f, 1.0f, 2.0f, 0.5f);

	shaSetVertexProgramConst( 0, (Matrix16*)&mat, 4);
	shaSetVertexProgramConst( 4, &srLightDir,     1);
	shaSetVertexProgramConst( 5, &srLightColor,   1);
	shaSetVertexProgramConst( 6, &srAmbientColor, 1);
	shaSetVertexProgramConst( 7, &srConstants,    1);

	// set bone matrices that exists
	for(INDEX ibm=0;ibm<_ctBoneMatrices;ibm++)
	{
		const Matrix12 *pbm = _pamBoneMatrices[ibm];
		shaSetVertexProgramConst(21+ibm*3, pbm, 3);
	}
}
*/
//안태훈 수정 끝	//(Add Tagent-space Normal Map)(0.1)

//안태훈 수정 시작	//(Add Tagent-space Normal Map)(0.1)
extern void shaSetDefaultConstantRegisters(void)
{
	if( !( (_pGfx->gl_ulFlags & GLF_VERTEXPROGRAM)
		&& (_pGfx->gl_ulFlags & GLF_PIXELPROGRAM)
		&& (_pGfx->gl_ctTextureUnits >= 4) ) ) return;

	D3DXMATRIX mat;
	D3DXMATRIX matView;
	D3DXMATRIX matProj;
	_pGfx->gl_pd3dDevice->GetTransform(D3DTS_VIEW,&matView);
	_pGfx->gl_pd3dDevice->GetTransform(D3DTS_PROJECTION,&matProj);
	D3DXMatrixMultiply( &mat, &matView, &matProj);
	D3DXMatrixTranspose( &mat, &mat);

	GFXColor colLight   = _colLight;
	GFXColor colAmbient = _colAmbient;
	colAmbient.r = ClampUp((SLONG)colAmbient.r, 127L);
	colAmbient.g = ClampUp((SLONG)colAmbient.g, 127L);
	colAmbient.b = ClampUp((SLONG)colAmbient.b, 127L);

	// if full bright
	if( shaGetFlags() & BASE_FULL_BRIGHT) {
		colLight.abgr = 0xFF000000;
		colAmbient.abgr = 0xFF7F7F7F;
	}

	const ShaderRegister srLightColor(colLight);
	const ShaderRegister srAmbientColor(colAmbient);
	const ShaderRegister srLightDir(_vLightDir(1), _vLightDir(2), _vLightDir(3), 765.01f);
	const ShaderRegister srConstants(0.0f, 1.0f, 2.0f, 0.5f);

	shaSetVertexProgramConst( 0, (Matrix16*)&mat, 4);
	shaSetVertexProgramConst( 4, &srLightDir,     1);
	shaSetVertexProgramConst( 5, &srLightColor,   1);
	shaSetVertexProgramConst( 6, &srAmbientColor, 1);
	shaSetVertexProgramConst( 7, &srConstants,    1);

	// set bone matrices that exists
	for(INDEX ibm=0;ibm<_ctBoneMatrices;ibm++)
	{
		const Matrix12 *pbm = _pamBoneMatrices[ibm];
		shaSetVertexProgramConst(21+ibm*3, pbm, 3);
	}
}

//강동민 수정 시작		03.19
extern void shaSetPixelProgramConst_WaterColor()
{
		//GFXColor colWater = _pWaterInformation->m_colWater;		
		GFXColor colWater = shaGetModelColor();		
		// 반사 색상.
		ShaderRegister srWaterColor(colWater);
		shaSetPixelProgramConst( 3, &srWaterColor, 1);		// c3
}
//강동민 수정 끝		03.19

//Transform Matrix를 상수레지스터 c0~c3에 세팅한다.
extern void shaSetVertexProgramConst_TransformMatrix()
{
	D3DXMATRIX mat;
	D3DXMATRIX matView;
	D3DXMATRIX matProj;
	_pGfx->gl_pd3dDevice->GetTransform(D3DTS_VIEW,&matView);
	_pGfx->gl_pd3dDevice->GetTransform(D3DTS_PROJECTION,&matProj);
	D3DXMatrixMultiply( &mat, &matView, &matProj);
	D3DXMatrixTranspose( &mat, &mat);
	shaSetVertexProgramConst( 0, (Matrix16*)&mat, 4);
}

//Light의 방향과 bone을 위한 값을 상수레지스터 c4.xyz, c4.w에 세팅한다.
extern void shaSetVertexProgramConst_LightDir()
{
	const ShaderRegister srLightDir(_vLightDir(1), _vLightDir(2), _vLightDir(3), 765.01f);
	shaSetVertexProgramConst( 4, &srLightDir,     1);
}

//Light의 Diffuse Color와 Ambient Color를 상수레지스터 c5, c6에 세팅한다.
extern void shaSetVertexProgramConst_LightColor(GFXColor colDiffuseColor, GFXColor colAmbientColor, BOOL bFullBright /*= bFullBright*/)
{
	GFXColor colLight   = _colLight;
	GFXColor colAmbient = _colAmbient;

	// if full bright
	if(bFullBright)
	{
		colLight.abgr = 0xFF000000;
		colAmbient.abgr = 0xFF7F7F7F;
	}
	else
	{
		colLight.MultiplyRGBCopyA1( (GFXColor)colDiffuseColor, _colLight );
		colAmbient.MultiplyRGBCopyA1( (GFXColor)colAmbientColor, _colAmbient );
	}
	const ShaderRegister srLightColor(colLight);
	const ShaderRegister srAmbientColor(colAmbient);
	shaSetVertexProgramConst(5, &srLightColor,   1);
	shaSetVertexProgramConst(6, &srAmbientColor, 1);
}

//Shader에서 내부에서 곱셈에서 사용할 수를 세팅한다. 0, 1, 2, 0.5
extern void shaSetVertexProgramConst_Multiply()
{
	const ShaderRegister srConstants(0.0f, 1.0f, 2.0f, 0.5f);
	shaSetVertexProgramConst( 7, &srConstants,    1);
}


//강동민 수정 시작		03.17
// 텍스쳐 투영행렬... c20~c23
extern void shaSetVertexProgramConst_ProjectionMatrix()
{
//	if(_pWaterInformation)
//	{		
		D3DXMATRIX mat;
		D3DXMATRIX matView;
		D3DXMATRIX matProj;		
		D3DXMatrixIdentity(&mat);
		D3DXMatrixIdentity(&matView);
		D3DXMatrixIdentity(&matProj);			
		_pGfx->gl_pd3dDevice->GetTransform(D3DTS_VIEW, &matView);
//강동민 수정 시작 Water 구현		04.21
		//_pGfx->gl_pd3dDevice->GetTransform(D3DTS_PROJECTION, &matProj);		// 원본.
//강동민 수정 끝 Water 구현			04.21
		
		Matrix16 matScaleOffset;
		memset(&matScaleOffset, 0, sizeof(float)*16);
		// 0.5f, 0, 0,    0
		// 0,  -0.5, 0,   0
		// 0,    0, 1.0,  0
		// 0.5f, 0.5f, 0, 1
		matScaleOffset[0] = 0.5f;
		matScaleOffset[5] = -0.5f;
		matScaleOffset[10] = 1.0f;
		
		matScaleOffset[12] = 0.5f;
		matScaleOffset[13] = 0.5f;
		matScaleOffset[14] = 0.0f;
		matScaleOffset[15] = 1.0f;		

		//D3DXMatrixIdentity((D3DXMATRIX*)&_matWaterProj);
//강동민 수정 시작 Water 구현		04.21
		//D3DXMatrixMultiply(&matProj, (D3DXMATRIX*)&matProj, (D3DXMATRIX*)&matScaleOffset);		// 원본.
		D3DXMatrixMultiply(&matProj, (D3DXMATRIX*)&_matWaterProj, (D3DXMATRIX*)&matScaleOffset);
//강동민 수정 끝 Water 구현			04.21

		D3DXMatrixMultiply( &mat, &matView, (D3DXMATRIX*)&matProj);	// mat = matView * matProj;		
		//D3DXMatrixMultiply( &mat, &matView, (D3DXMATRIX*)&_matWaterProj);	// mat = matView * matProj;		
		D3DXMatrixTranspose( &mat, &mat);		

		// c21~24
		shaSetVertexProgramConst( 21, (Matrix16*)&mat, 4);
	//}
}
//강동민 수정 끝		03.17

//강동민 수정 시작		03.17
// Water 구현을 위해서...Shader에서 사용할 상수를 설정함.
extern void shaSetVertexProgramConst_WaterInformation()
{
//	if(_pWaterInformation)
//	{
		shaSetVertexProgramConst_Sine();			//c8
		shaSetVertexProgramConst_CoSine();			//c9	
		shaSetVertexProgramConst_EyePos();			//c19					// 카메라 위치.

		// c10 -> LightPos
		shaSetVertexProgramConst_Fixup();			//c11					// 테일러 급수 전개의 부정확성을 보정하기 위한 계수 c11		
		if(_WaterInformation.m_bWave)
		{
			shaSetVertexProgramConst_Timer();								//c17
		}
		else
		{
			const ShaderRegister srTimeSine(0.0f, 0.0f, 0.0f, 0.0f);			// 시간, Sin(시간)
			shaSetVertexProgramConst( 17, &srTimeSine,    1);			
		}
		shaSetVertexProgramConst_PI();						//c20
		// WaveHeight										c12
		// WaveOffset										c13
		// WaveSpeed										c14
		// WaveDirX											c15
		// WaveDirY											c16
		// 시간, sin(시간)									c17		
		// 텍스쳐 보정										c18		

		// 기반 텍스쳐 좌표 수정값.							c18	
//강동민 수정 시작 Water 구현		04.20
		//const ShaderRegister srBaseTexFix(_WaterInformation.m_fTexFlowX, _WaterInformation.m_fTexFlowY, 
//강동민 수정 끝 Water 구현			04.20
		//	-0.03f, 0.02f);			// BaseTexFix
//강동민 수정 시작 Water 구현		04.21
		//_pWaterInformation->m_fTexFlowX2, _pWaterInformation->m_fTexFlowY2);			// BaseTexFix
//강동민 수정 끝 Water 구현			04.21
		//shaSetVertexProgramConst( 18, &srBaseTexFix,    1);
		//_pWaterInformation = NULL;
	//}
}

extern void shaSetVertexProgramConst_PI()
{
	const ShaderRegister srPI(4.0f, 0.5f*PI, PI, 2.0f*PI);				// PI
	shaSetVertexProgramConst( 20, &srPI,    1);
}

//강동민 수정 끝		03.17
//강동민 수정 시작 Water 구현		04.20
extern void shaSetEMBM()
{
	//if(_pWaterInformation)
	//{
		D3DXMATRIX matBumpMat;
		D3DXMatrixIdentity(&matBumpMat);
		matBumpMat._11 = _WaterInformation.m_fBumpMat11;
		matBumpMat._12 = _WaterInformation.m_fBumpMat12;
		matBumpMat._21 = _WaterInformation.m_fBumpMat21;
		matBumpMat._22 = _WaterInformation.m_fBumpMat22;
		
		HRESULT hr;
		
		/*
		CTimerValue tvNow   = _pTimer->GetHighPrecisionTimer();
		FLOAT r = 0.01f;
		matBumpMat._11 =  r * cosf( tvNow.GetSeconds() * 4.0f );
		matBumpMat._12 = -r * sinf( tvNow.GetSeconds() * 4.0f );
		matBumpMat._21 =  r * sinf( tvNow.GetSeconds() * 4.0f );
		matBumpMat._22 =  r * cosf( tvNow.GetSeconds() * 4.0f );
		*/
		
		// EMBM 설정.
		hr = _pGfx->gl_pd3dDevice->SetTextureStageState( 1, D3DTSS_BUMPENVMAT00,   F2DW(matBumpMat._11));	D3D_CHECKERROR(hr);
		hr = _pGfx->gl_pd3dDevice->SetTextureStageState( 1, D3DTSS_BUMPENVMAT01,   F2DW(matBumpMat._12));	D3D_CHECKERROR(hr);
		hr = _pGfx->gl_pd3dDevice->SetTextureStageState( 1, D3DTSS_BUMPENVMAT10,   F2DW(matBumpMat._21));	D3D_CHECKERROR(hr);
		hr = _pGfx->gl_pd3dDevice->SetTextureStageState( 1, D3DTSS_BUMPENVMAT11,   F2DW(matBumpMat._22));	D3D_CHECKERROR(hr);
	//}
}
//강동민 수정 끝 Water 구현			04.20



extern void shaSetVertexProgramConst_Sine()
{
	const ShaderRegister srConstants(1.0f, -1.0f/6.0f, 1.0f/120.0f, -1.0f/5040.0f);
	shaSetVertexProgramConst( 8, &srConstants,    1);
}

extern void shaSetVertexProgramConst_Timer()
{
	TIME tmNow	= _pTimer->GetLerpedCurrentTick();
	tmNow		= fmod(tmNow, 600.0f);
	
	//TIME tmSin = Sin( tmNow*22)/40;
	//TIME tmCos = Cos( tmNow*26)/45;
	// FIXME : 임의적으로 설정한 값.
	// FIXME : 월드 에디터를 수정해서 처리할것.
	TIME tmSin = 0.0f;
	//TIME tmCos = 8192/1024.0f * tmNow * 0.001f;
	TIME tmCos = 8192/1024.0f * tmNow * _WaterInformation.m_fTexFlowX;
	const ShaderRegister srTimeSine(tmNow, tmSin, tmCos, 0.0f);			// 시간, Sin(시간)
	shaSetVertexProgramConst( 17, &srTimeSine,    1);
}

extern void shaSetVertexProgramConst_CoSine()
{
	const ShaderRegister srConstants(1.0f/2.0f, -1.0f/24.0f, 1.0f/720.0f, -1.0f/40320.0f);
	shaSetVertexProgramConst( 9, &srConstants,    1);
}

extern void shaSetVertexProgramConst_Fixup()
{
	const ShaderRegister srConstants(1.02f, 0.1f, 0.0f, 0.0f);
	shaSetVertexProgramConst( 11, &srConstants,    1);
}

// 광원의 위치.
extern void shaSetVertexProgramConst_LightPos()
{
	const ShaderRegister srLightPos(-10000.0f, -25000.0f, 2000.0f, 1.0f);
	shaSetVertexProgramConst( 10, &srLightPos,     1);
}

// 카메라 위치
extern void shaSetVertexProgramConst_EyePos()
{
	const ShaderRegister srEyePos(_vViewer(1), _vViewer(2), _vViewer(3), 1.0f);
	shaSetVertexProgramConst( 19, &srEyePos,     1);
}
//강동민 수정 끝 Water 구현		04.13

#define FLOAT_SAME_M6(f1, f2) (fabs((f1) - (f2)) < 1e-6f)
//bone blend를 위한 Matrix를 상수레지스터 c21~...에 세팅한다. c4.w에 765.01f가 세팅되어 있어야한다.
extern void shaSetVertexProgramConst_BoneMatrix()
{
#ifdef _DEBUG
	ShaderRegister srValue(0,0,0,0);
	shaGetVertexProgramConst(4, srValue);
	ASSERT( FLOAT_SAME_M6(srValue.sr_fw, 765.01f) );
#endif //_DEBUG
	//속도문제가 있을까봐 제거하였음.
//	ShaderRegister srValue(0,0,0,0);
//	shaGetVertexProgramConst(4, srValue);
//	srValue.sr_fw = 765.01f;
//	shaSetVertexProgramConst(4, &srValue, 1);

	// set bone matrices that exists
	for(INDEX ibm=0;ibm<_ctBoneMatrices;ibm++)
	{
		const Matrix12 *pbm = _pamBoneMatrices[ibm];
		shaSetVertexProgramConst(21+ibm*3, pbm, 3);
	}
}
#undef FLOAT_SAME_M6
#include <Engine/Math/AdditionalFunction.h>

extern void shaPrepareForBillboard(BOOL bCylinderType/* = FALSE*/, BOOL bNeedCalcLight/* = FALSE*/)
{
	D3DXMATRIX mat;
	D3DXMATRIX matView, matNewView, matInvViewRot, matViewRot;
	D3DXMATRIX matProj;
	
	//현재의 modelView행렬을 가져온다.
	_pGfx->gl_pd3dDevice->GetTransform(D3DTS_VIEW,&matView);
	//현재의 modelView행렬의 4번째줄을 기억한다.
	D3DXVECTOR3 viewRow4(matView._41, matView._42, matView._43);
	//현재의 modelView의 위치를 기억한다.
	D3DXVECTOR3 pos(matView._14, matView._24, matView._34);
	//modelView행렬의 회전만을 남긴다.
	matView._14 = matView._24 = matView._34 = 
	matView._41 = matView._42 = matView._43 = 0;
	matView._44 = 1;

	if(bCylinderType)
	{
		FLOAT3D vDir = (*_paprProjection)->pr_ViewerPlacement.pl_PositionVector;
		vDir = (-vDir).SafeNormalize();
		ANGLE3D a3dModel = GetEulerAngleFromDir(vDir);
 		a3dModel(2) = a3dModel(3) = 0; 
		FLOATmatrix3D matRot;
		MakeRotationMatrixFast(matRot, a3dModel);
/*
		//View의 회전을 갖고와서 cylinder billboard행렬을 계산한다.
		ANGLE3D angle(
			(*_paprProjection)->pr_ViewerPlacement.pl_OrientationAngle(1)
			, (*_paprProjection)->pr_ViewerPlacement.pl_OrientationAngle(2)
			, (*_paprProjection)->pr_ViewerPlacement.pl_OrientationAngle(3)
			);
		angle(1) = 0;
		//angle(2) = 0;
		//angle(3) = 0;
		FLOATmatrix3D matRot;
		MakeInverseRotationMatrixFast(matRot, angle);
		//modelView행렬에 계산한 행렬을 곱한다.
		matViewRot._11 = matRot(1,1); matViewRot._12 = matRot(1,2); matViewRot._13 = matRot(1,3); matViewRot._14 = 0;
		matViewRot._21 = matRot(2,1); matViewRot._22 = matRot(2,2); matViewRot._23 = matRot(2,3); matViewRot._24 = 0;
		matViewRot._31 = matRot(3,1); matViewRot._32 = matRot(3,2); matViewRot._33 = matRot(3,3); matViewRot._34 = 0;
		matViewRot._41 = 0;            matViewRot._42 = 0;           matViewRot._43 = 0;            matViewRot._44 = 1;
*/
		matViewRot._11 = matRot(1,1); matViewRot._12 = matRot(1,3); matViewRot._13 = matRot(1,2); matViewRot._14 = 0;
		matViewRot._21 = matRot(2,1); matViewRot._22 = matRot(2,3); matViewRot._23 = matRot(2,2); matViewRot._24 = 0;
		matViewRot._31 = matRot(3,1); matViewRot._32 = matRot(3,3); matViewRot._33 = matRot(3,2); matViewRot._34 = 0;
		matViewRot._41 = 0;           matViewRot._42 = 0;           matViewRot._43 = 0;           matViewRot._44 = 1;
		D3DXMatrixMultiply(&matNewView, &matView, &matViewRot);
		//matNewView = matViewRot; 
		//col 4와 row 4를 다시 세팅한다.
		matNewView._41 = viewRow4.x;
		matNewView._42 = viewRow4.y;
		matNewView._43 = viewRow4.z;
		matNewView._44 = 1;
		matNewView._14 = pos.x;
		matNewView._24 = pos.y;
		matNewView._34 = pos.z;
	}
	else
	{
		//View행렬의 회전을 갖고와서 billboard행렬을 계산한다.
		//memset(&matViewRot, 0, sizeof(matViewRot));
		FLOATmatrix3D &mView = (*_paprProjection)->pr_ViewerRotationMatrix;
		matViewRot._11 = -mView(1,1); matViewRot._12 = mView(1,2); matViewRot._13 = -mView(1,3);
		matViewRot._21 = -mView(2,1); matViewRot._22 = mView(2,2); matViewRot._23 = -mView(2,3);
		matViewRot._31 = -mView(3,1); matViewRot._32 = mView(3,2); matViewRot._33 = -mView(3,3);
		//modelView행렬에 계산한 행렬을 곱한다.
		D3DXMatrixMultiply(&matNewView, &matView, &matViewRot);
		//col 4와 row 4를 다시 세팅한다.
		matNewView._41 = viewRow4.x;
		matNewView._42 = viewRow4.y;
		matNewView._43 = viewRow4.z;
		matNewView._44 = 1;
		matNewView._14 = pos.x;
		matNewView._24 = pos.y;
		matNewView._34 = pos.z;
	}

	if(bNeedCalcLight)
	{
		//현재 modelView공간에 있는 light의 방향을 재조정한다.
		D3DXVECTOR3 lightDir(_vLightDir(1), _vLightDir(2), _vLightDir(3)), lightNewDir;
//		float det;	//이상하게도 역행렬은 필요없었음.-_-;
//		D3DXMATRIX *ret = D3DXMatrixInverse(&matInvViewRot, &det, &matViewRot);
		D3DXVec3TransformNormal(&lightNewDir, &lightDir, &matViewRot);
		_vLightDir.vector[0] = -lightNewDir.x;
		_vLightDir.vector[1] = lightNewDir.y;
		_vLightDir.vector[2] = lightNewDir.z;
	}

	//최종적으로 SetTransform한다.
	_pGfx->gl_pd3dDevice->SetTransform(D3DTS_VIEW, &matNewView);
}
//안태훈 수정 끝	//(Add Tagent-space Normal Map)(0.1)

// Set const vertex program register
extern void shaSetVertexProgramConst(INDEX iRegister, const ShaderRegister *srData, INDEX ctRegisters)
{
	gfxSetVertexProgramConst(iRegister, srData, ctRegisters);
}

extern void shaSetVertexProgramConst(INDEX iRegister, const Matrix12 *pmData, INDEX ctRegisters)
{
	ASSERT(ctRegisters==3);
	gfxSetVertexProgramConst(iRegister, pmData, ctRegisters);
}

extern void shaSetVertexProgramConst(INDEX iRegister, const Matrix16 *pmData, INDEX ctRegisters)
{
	ASSERT(ctRegisters==4);
	gfxSetVertexProgramConst(iRegister, pmData, ctRegisters);
}

//안태훈 수정 시작	//(Add Tagent-space Normal Map)(0.1)
// Set const vertex program register
extern void shaGetVertexProgramConst(INDEX iRegister, ShaderRegister &pData)
{
	_pGfx->gl_pd3dDevice->GetVertexShaderConstant(iRegister, &pData, 1);
}
extern void shaGetVertexProgramConst(INDEX iRegister, Matrix12 &pmData)
{
	_pGfx->gl_pd3dDevice->GetVertexShaderConstant(iRegister, pmData, 3);
}
extern void shaGetVertexProgramConst(INDEX iRegister, Matrix16 &pmData)
{
	_pGfx->gl_pd3dDevice->GetVertexShaderConstant(iRegister, pmData, 4);
}
// Set const pixel program register
extern void shaGetPixelProgramConst(INDEX iRegister, ShaderRegister &pData)
{
	_pGfx->gl_pd3dDevice->GetPixelShaderConstant(iRegister, &pData, 1);
}
//안태훈 수정 끝	//(Add Tagent-space Normal Map)(0.1)

// Set const pixel program register
extern void shaSetPixelProgramConst(INDEX iRegister, const ShaderRegister *pData, INDEX ctRegisters)
{
	gfxSetPixelProgramConst(iRegister, pData, ctRegisters);
}

//안태훈 수정 시작	//(Add Tagent-space Normal Map)(0.1)
//안태훈이 이 부분의 코드를 ShaderCode.h쪽으로 빼서 정리했음.
#include <Engine/Graphics/ShaderCode.h>
//안태훈 수정 끝	//(Add Tagent-space Normal Map)(0.1)

// Generate code for vertex program for given fog tex unit
static CTString GenerateVertexProgramFogCode(INDEX iFogTexUnit)
{
	ASSERT(iFogTexUnit>=0 && iFogTexUnit<_pGfx->gl_ctTextureUnits);
	CTString strFogTexUnit(0,"%d", iFogTexUnit);
	CTString strFogAddOutput =  "mul  oT" +  strFogTexUnit + ".x,  r9.w,    c20.z \n" + 
								"mul  oT" +  strFogTexUnit + ".y,  r8.w,    c20.w \n";
	return strFogVP + strFogAddOutput;
}

static CTString AssembleVertexProgram(const char *strAdditionalCode, ULONG ulStreamFlags)
{
	CTString strEndCode;
	extern INDEX sha_bColorizeShaders;	//현재는 콘솔에서 수정하지 않는 한은 항상 FALSE임.
	if( sha_bColorizeShaders) {
		strEndCode   = "mov  oD0,    r1                \n";
	}

	CTString strNormalize;
//안태훈 수정 시작	//(Add Shadow Util)(0.1)
	if(ulStreamFlags&GFX_NORMAL_STREAM)
	{
		extern INDEX dbg_bNormalizeShaderNormals;
		if( dbg_bNormalizeShaderNormals)
		{
			strNormalize =  "dp3  r8.w,   r1,      r1       \n" // r1%r1
							"rsq  r7.w,   r8.w              \n" // 1/Sqrt(r1%r1)
							"mul  r1,     r1,      r7.wwww  \n" // r1.x = r1.x / Sqrt(r1%r1) ...
							;
		}
		ASSERT(_ctWeightsPerVertex<5 && _ctWeightsPerVertex>=0);
		switch(_ctWeightsPerVertex)
		{
			case 0: return CTString(strNoWeights   ) + strNormalize + strAdditionalCode + strEndCode; break;
			case 1: return CTString(strOneWeight   ) + strNormalize + strAdditionalCode + strEndCode; break;
			case 2: return CTString(strTwoWeights  ) + strNormalize + strAdditionalCode + strEndCode; break;
			case 3: return CTString(strThreeWeights) + strNormalize + strAdditionalCode + strEndCode; break;
			case 4: return CTString(strFourWeights ) + strNormalize + strAdditionalCode + strEndCode; break;
			default:
				ASSERT(FALSE);
			break;
		}
	}
	//Normal을 쓰지 않는 경우의 처리.
	else
	{
		ASSERT(_ctWeightsPerVertex<5 && _ctWeightsPerVertex>=0);
		switch(_ctWeightsPerVertex)
		{
			case 0: return CTString(strNoWeights_OnlyPosition   ) + strAdditionalCode; break;
			case 1: return CTString(strOneWeight_OnlyPosition   ) + strAdditionalCode; break;
			case 2: return CTString(strTwoWeights_OnlyPosition  ) + strAdditionalCode; break;
			case 3: return CTString(strThreeWeights_OnlyPosition) + strAdditionalCode; break;
			case 4: return CTString(strFourWeights_OnlyPosition ) + strAdditionalCode; break;
			default:
				ASSERT(FALSE);
			break;
		}
	}
//안태훈 수정 끝	//(Add Shadow Util)(0.1)
	return "";
}

static CTString AssemblePixelProgram(CTString strAdditionalCode, DWORD PixelShaderVersion, INDEX iFogTexUnit=-1)
{
	if (PixelShaderVersion == D3DPS_VERSION(1,1))
		return strPixelProgramPrefix11 + strAdditionalCode;
	//else if (PixelShaderVersion == D3DPS_VERSION(1,4))

	return strPixelProgramPrefix14 + strAdditionalCode;
}


// Render given model
extern void shaRender(void)
{
	ASSERT(_ctVertices>0);
	ASSERT(_iFirstVertex>=0);
	ASSERT(_ctIndices>0);
	ASSERT(_puwIndices!=NULL);

	const BOOL bUseVertexBuffers = _bUseHWVertexBuffers;
	const BOOL bUseVertexProgram = _bUseHWVertexProgram;
	const BOOL bUseColorArrays   = _pcolVtxColors!=NULL;

	if(bUseColorArrays) {
		gfxSetColorArray(_pcolVtxColors);
	} else {
		gfxDisableColorArray();
	}

//안태훈 수정 시작	//(Add Tagent-space Normal Map)(0.1)
//	if(bUseVertexProgram) {
//		SetConstantRegisters();
//	}
//안태훈 수정 끝	//(Add Tagent-space Normal Map)(0.1)

	if(bUseVertexBuffers) {
		ASSERT(_iVertexBufferID>=0); // Vertices must be set 
//안태훈 수정 시작	//(Add Shadow Util)(0.1)
		//ASSERT(_iNormalBufferID>=0); // Normals must be set
//안태훈 수정 끝	//(Add Shadow Util)(0.1)
	} else {
		ASSERT(_paVertices!=NULL);   // Vertices must be set before rendering
	}

	gfxDrawElements( _ctIndices, _puwIndices);
}

// Precache all shaders ( needs to be called only once )
extern void shaPrecacheShaders(void)
{
#if 0
	CObjectRestore<BOOL> or(_bPrecachingShaders,TRUE);
	// for each shader in stock
	INDEX ctsh = _pShaderStock->st_ctObjects.Count();
	for(INDEX ish=0;ish<ctsh;ish++) {
		CShader &sh = _pShaderStock->st_ctObjects[ish];
		// for each vertex program
		const INDEX ctvp = sh.sh_ctVertexPrograms;
		for(INDEX ivp=0;ivp<ctvp;ivp++) {
			VertexProgramHandle &vph = sh.sh_aVPHandles[ivp];
			// first precache all normal shaders and than all in 
			// for each vertex program handle
			for(INDEX ivph=0;ivph<MAX_VPH;ivph++) {
				const ULONG ulHandle = vph.vph_ulHandles[ipvh];
				if(ulHandle==NONE) {
					// Fake real vertex setting
					shaSetWeightsPerVertex(ivph);
					shaSetVertexProgram(ivp);
				}
			}
		}
	}
#endif
}

// Init fog and haze shared params by all shaders
extern void shaInitSharedFogAndHazeParams(ULONG ulRenFlags, CAnyProjection3D &apr, 
										 Matrix12 &mObjToView, Matrix12 &mObjToAbs)
{
	ASSERT(_paFogUVMap==NULL);
	ASSERT(_paHazeUVMap==NULL);

	FLOAT3D vObjToView;
	Matrix12 mInvObjToAbs;
	FLOATmatrix3D mObjToViewRot;

	MatrixTranspose(mInvObjToAbs,mObjToAbs);
	Matrix12ToMatrixVector(mObjToViewRot,vObjToView,mObjToView);

	_vObjPos = FLOAT3D(mObjToAbs[ 3],mObjToAbs[ 7],mObjToAbs[11]);

	// calculate projection of viewer in absolute space
	FLOATmatrix3D &mViewer = apr->pr_ViewerRotationMatrix;
	_vViewer(1) = -mViewer(3,1);
	_vViewer(2) = -mViewer(3,2);
	_vViewer(3) = -mViewer(3,3);

	// calculate projection of viewer in object space
	_vViewerObj = _vViewer;
	RotateVector(_vViewerObj.vector,mInvObjToAbs);

	if(ulRenFlags&SHA_RMF_HAZE) {
		// Prepare haze
		_fHazeAdd  = -_haze_hp.hp_fNear;
		_fHazeAdd += _vViewer(1) * (_vObjPos(1) - apr->pr_vViewerPosition(1));
		_fHazeAdd += _vViewer(2) * (_vObjPos(2) - apr->pr_vViewerPosition(2));
		_fHazeAdd += _vViewer(3) * (_vObjPos(3) - apr->pr_vViewerPosition(3));
	}

	if(ulRenFlags&SHA_RMF_FOG) {
		// get viewer -z in object space
		_vFViewerObj = FLOAT3D(0,0,-1) * !mObjToViewRot;
		// get fog direction in object space
		_vHDirObj = _fog_vHDirAbs * !(!mViewer*mObjToViewRot);
		// get viewer offset
		_fFogAddZ  = _vViewer(1) * (_vObjPos(1) - apr->pr_vViewerPosition(1));
		_fFogAddZ += _vViewer(2) * (_vObjPos(2) - apr->pr_vViewerPosition(2));
		_fFogAddZ += _vViewer(3) * (_vObjPos(3) - apr->pr_vViewerPosition(3));
		// get fog offset
		_fFogAddH = (_fog_vHDirAbs % _vObjPos) + _fog_fp.fp_fH3;
	}
}


// check vertex against haze
static inline void GetHazeMapInVertex( GFXVertex &vtx, FLOAT &tx1)
{
	const FLOAT fD = vtx.x*_vViewerObj(1) + vtx.y*_vViewerObj(2) + vtx.z*_vViewerObj(3);
	tx1 = (fD+_fHazeAdd) * _haze_fMul;
}

static inline void GetFogMapInVertex( GFXVertex &vtx, GFXTexCoord &tex)
{
	const FLOAT fD = vtx.x*_vFViewerObj(1) + vtx.y*_vFViewerObj(2) + vtx.z*_vFViewerObj(3);
	const FLOAT fH = vtx.x*_vHDirObj(1)    + vtx.y*_vHDirObj(2)    + vtx.z*_vHDirObj(3);
	tex.s = (fD+_fFogAddZ) * _fog_fMulZ;
	tex.t = (fH+_fFogAddH) * _fog_fMulH;
}


// Select texture unit for fog and haze
extern void shaSetFogTextureUnit(INDEX iFogTexUnit, BOOL bOpaque)
{
	ASSERT(iFogTexUnit>=0 && iFogTexUnit<_pGfx->gl_ctTextureUnits);
	_iFogTexUnit = iFogTexUnit;
	_bOpaqueSurface = bOpaque;
}

// Prepare colors and texcoords for fog and haze
extern void shaPrepareFogAndHaze(BOOL bOpaqueSurface)
{
	const BOOL bHardwareShader = shaUseHWShaders();
	// if model is in haze
	if(_ulRenFlags&SHA_RMF_HAZE) {
		// unpack haze color
		GFXColor colHaze(_haze_hp.hp_colColor);

		// if using hardware shaders
		if(bHardwareShader) {
			ASSERT(_iFogTexUnit>=0 && _iFogTexUnit<_pGfx->gl_ctTextureUnits);
			// Set haze stuff for vertex and pixel shaders
			const ShaderRegister srHaze(_fHazeAdd, 0.0f, _haze_fMul, 0.0f);
			const ShaderRegister srDummy(0.0f, 0.0f, 0.0f, 0.0f);
			const ShaderRegister srViewerObj(_vViewerObj);
			const ShaderRegister srHazeColor(colHaze);


			shaSetTextureUnit(_iFogTexUnit);
			shaSetTextureWrapping( GFX_CLAMP, GFX_CLAMP);
			gfxSetTexture( _haze_ulTexture, _haze_tpLocal);
			shaSetVertexProgramConst(18,&srViewerObj,1);
			shaSetVertexProgramConst(19,&srDummy,1); // fill with zeros so same shader can be used for fog and haze
			shaSetVertexProgramConst(20,&srHaze,1);
			shaSetPixelProgramConst(7,&srHazeColor,1);
		// else prepare software
		} else {
			const INDEX ctvx = shaGetVertexCount();
			_atcHaze.PopAll();
			_acolHaze.PopAll();   
			_aubshdHaze.PopAll(); 
			_atcHaze.Push(ctvx);
			_acolHaze.Push(ctvx);
			_aubshdHaze.Push(ctvx);

			// if surface is opaque
			if(bOpaqueSurface) {
				for(INDEX ivx=0;ivx<ctvx;ivx++) {
					FLOAT &tx1 = _atcHaze[ivx].s;
					_atcHaze[ivx].t = 0;
					GetHazeMapInVertex(_paVertices[ivx], tx1);
					_acolHaze[ivx] = colHaze;
				}
			} else {
				for(INDEX ivx=0;ivx<ctvx;ivx++) {
					FLOAT &tx1 = _atcHaze[ivx].s;
					_atcHaze[ivx].t = 0;
					GetHazeMapInVertex(_paVertices[ivx], tx1);
					_aubshdHaze[ivx] = GetHazeAlpha(tx1)^255;
					_acolHaze[ivx] = colHaze;
				}
			}
		}
	}

	// if model is in fog
	if(_ulRenFlags&SHA_RMF_FOG) {
		// unpack fog color
		GFXColor colFog(_fog_fp.fp_colColor);

		// if using hardware shaders
		if(bHardwareShader) {
			ASSERT(_iFogTexUnit>=0 && _iFogTexUnit<_pGfx->gl_ctTextureUnits);

			const ShaderRegister srFog(_fFogAddZ, _fFogAddH, _fog_fMulZ, _fog_fMulH);
			const ShaderRegister srFViewerObj(_vFViewerObj);
			const ShaderRegister srHDirObj(_vHDirObj);
			const ShaderRegister srFogColor(colFog);

			// Set haze stuff for vertex and pixel shaders
			shaSetTextureUnit(_iFogTexUnit);
			shaSetTextureWrapping( GFX_CLAMP, GFX_CLAMP);
			gfxSetTexture( _fog_ulTexture, _fog_tpLocal);
			shaSetVertexProgramConst(18,&srFViewerObj,1);
			shaSetVertexProgramConst(19,&srHDirObj,1);
			shaSetVertexProgramConst(20,&srFog,1);
			shaSetPixelProgramConst(  7,&srFogColor,1);
		// else prepare software
		} else {
			const INDEX ctvx = shaGetVertexCount();
			_atcFog.PopAll();
			_acolFog.PopAll();
			_aubshdFog.PopAll();
			_atcFog.Push(ctvx);
			_acolFog.Push(ctvx);
			_aubshdFog.Push(ctvx);

			if(bOpaqueSurface) {
				for(INDEX ivx=0;ivx<ctvx;ivx++) {
					GFXTexCoord &tex = _atcFog[ivx];
					GetFogMapInVertex( _paVertices[ivx], tex);
					_acolFog[ivx] = colFog;
				}
			} else {
				for(INDEX ivx=0;ivx<ctvx;ivx++) {
					GFXTexCoord &tex = _atcFog[ivx];
					GetFogMapInVertex( _paVertices[ivx], tex);
					_aubshdFog[ivx] = GetFogAlpha(tex) ^255;
					_acolFog[ivx] = colFog;
				}
			}
		}
	}
}

// Adjust model color array for fog and haze
extern void shaAttenuateColorForFogAndHaze(void)
{
	const INDEX ctvx = shaGetVertexCount();
	GFXColor *pcolBase = shaGetColorArray();

	// if model has haze
	if(_ulRenFlags&SHA_RMF_HAZE) {
		// attenuate color on vertex colors
		for(INDEX ivx=0;ivx<ctvx;ivx++) {
			pcolBase[ivx].AttenuateRGB(_aubshdHaze[ivx]);
		}
	}
	// if model has fog
	if(_ulRenFlags&SHA_RMF_FOG) {
		// attenuate color on vertex colors
		for(INDEX ivx=0;ivx<ctvx;ivx++) {
			pcolBase[ivx].AttenuateRGB(_aubshdFog[ivx]);
		}
	}
}


// Adjust alpha in color array for fog and haze
extern void shaAttenuateAlphaForFogAndHaze(void)
{
	const INDEX ctvx = shaGetVertexCount();
	GFXColor *pcolBase = shaGetColorArray();

	// if model has haze
	if(_ulRenFlags&SHA_RMF_HAZE) {
		// attenuate alpha on vertex colors
		for(INDEX ivx=0;ivx<ctvx;ivx++) {
			pcolBase[ivx].AttenuateA(_aubshdHaze[ivx]);
		}
	}
	// if model has fog
	if(_ulRenFlags&SHA_RMF_FOG) {
		// attenuate alpha on vertex colors
		for(INDEX ivx=0;ivx<ctvx;ivx++) {
			pcolBase[ivx].AttenuateA(_aubshdFog[ivx]);
		}
	}
}


// Render aditional pass for fog and haze
extern void shaDoFogAndHazePass(void)
{
	ASSERT(_paFogUVMap==NULL);
	ASSERT(_paHazeUVMap==NULL);

	DWORD op, arg1;
	extern INDEX GFX_iActiveTexUnit;
	// if model is in haze
	if(_ulRenFlags&SHA_RMF_HAZE) {
		// set haze render states
		gfxSetTextureWrapping( GFX_CLAMP, GFX_CLAMP);
		gfxBlendFunc( GFX_SRC_ALPHA, GFX_INV_SRC_ALPHA);
		gfxEnableBlend();
		gfxDisableAlphaTest();
		gfxSetTexture( _haze_ulTexture, _haze_tpLocal);
		gfxSetTexCoordArray( &_atcHaze[0], FALSE);
		gfxSetColorArray(&_acolHaze[0]);
		_pGfx->gl_pd3dDevice->GetTextureStageState(GFX_iActiveTexUnit, D3DTSS_COLOROP, &op);
		_pGfx->gl_pd3dDevice->GetTextureStageState(GFX_iActiveTexUnit, D3DTSS_COLORARG1, &arg1);
		_pGfx->gl_pd3dDevice->SetTextureStageState(GFX_iActiveTexUnit, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
		_pGfx->gl_pd3dDevice->SetTextureStageState(GFX_iActiveTexUnit, D3DTSS_COLORARG1, D3DTA_CURRENT);
		// render haze pass
		gfxDrawElements( _ctIndices, _puwIndices);
		if(!(_ulRenFlags&SHA_RMF_FOG))
		{
			_pGfx->gl_pd3dDevice->SetTextureStageState(GFX_iActiveTexUnit, D3DTSS_COLOROP, op);
			_pGfx->gl_pd3dDevice->SetTextureStageState(GFX_iActiveTexUnit, D3DTSS_COLORARG1, arg1);
		}
	}

	// if model is in fog
	if(_ulRenFlags&SHA_RMF_FOG) {
		// set fog render states
		gfxSetTextureWrapping( GFX_CLAMP, GFX_CLAMP);
		gfxBlendFunc( GFX_SRC_ALPHA, GFX_INV_SRC_ALPHA);
		gfxEnableBlend();
		gfxDisableAlphaTest();
		gfxSetTexture( _fog_ulTexture, _fog_tpLocal);
		gfxSetTexCoordArray(&_atcFog[0], FALSE);
		gfxSetColorArray(&_acolFog[0]);
		if(!(_ulRenFlags&SHA_RMF_HAZE))
		{
			_pGfx->gl_pd3dDevice->GetTextureStageState(GFX_iActiveTexUnit, D3DTSS_COLOROP, &op);
			_pGfx->gl_pd3dDevice->GetTextureStageState(GFX_iActiveTexUnit, D3DTSS_COLORARG1, &arg1);
			_pGfx->gl_pd3dDevice->SetTextureStageState(GFX_iActiveTexUnit, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
			_pGfx->gl_pd3dDevice->SetTextureStageState(GFX_iActiveTexUnit, D3DTSS_COLORARG1, D3DTA_CURRENT);
		}
		// render fog pass
		gfxDrawElements( _ctIndices, _puwIndices);
		_pGfx->gl_pd3dDevice->SetTextureStageState(GFX_iActiveTexUnit, D3DTSS_COLOROP, op);
		_pGfx->gl_pd3dDevice->SetTextureStageState(GFX_iActiveTexUnit, D3DTSS_COLORARG1, arg1);
	}
}

extern void shaFillColorBuffer(COLOR col)
{
	// prepare and set current vertex color array 
	_acolVtxColors.PopAll();
	_acolVtxColors.Push(_ctVertices+1); // must make memory wall (for ASM)!
	_acolVtxColors.Pop();
	_pcolVtxColors = &_acolVtxColors[0];

	StoreLongs( col, (ULONG*)&_acolVtxColors[0], _ctVertices); 
	return;
}

// Calculate lightning for given model
extern void shaCalculateLight(void)
{
	if(_bUseHWVertexProgram) {
		return;
	}
	// SKA models must have overbrighting
	ASSERT( shaOverBrightningEnabled());  
	ASSERT(_paNormals!=NULL);

	// set surface color
	GFXColor colSurface;
	colSurface.MultiplyRGBA( (GFXColor)_colModel, (GFXColor)_colConstant); // mix surface with model color

	// if full bright
	if( shaGetFlags() & BASE_FULL_BRIGHT) {
		// just act as ambient is 127 (because of full-bright) and light is 0
		const ULONG col = (colSurface.abgr>>1) & 0x007F7F7F | (colSurface.a<<24);
		shaFillColorBuffer(col);
		return;
	}

	// prepare and set current vertex color array 
	_acolVtxColors.PopAll();
	_acolVtxColors.Push(_ctVertices+1); // must make memory wall (for ASM)!
	_acolVtxColors.Pop();
	_pcolVtxColors = &_acolVtxColors[0];


	// setup light and ambient colors
	GFXColor colLight, colAmbient;
	colLight  .MultiplyRGB( colSurface, (GFXColor)_colLight);    // mix light with surface 
	colAmbient.MultiplyRGB( colSurface, (GFXColor)_colAmbient);  // mix ambient with surface 
	const SLONG slLR = colLight.r;
	const SLONG slLG = colLight.g;
	const SLONG slLB = colLight.b;
	const SLONG slAR = ClampUp( (SLONG)colAmbient.r, 127L);
	const SLONG slAG = ClampUp( (SLONG)colAmbient.g, 127L);
	const SLONG slAB = ClampUp( (SLONG)colAmbient.b, 127L);

#if ASMOPT == 1

	#define VTX_PER_PASS 384 // looks like optimum for 32K L1 cache
	GFXNormal *pvNormals = &_paNormals[0];
	GFXColor  *pulColors = &_acolVtxColors[0];
	const FLOAT fLightX = _vLightDir(1)*127.0f; // boost for conversion to integer
	const FLOAT fLightY = _vLightDir(2)*127.0f;
	const FLOAT fLightZ = _vLightDir(3)*127.0f;
	const SLONG slSA = colSurface.a; 
	INDEX ctVtxTotal = _ctVertices;
	INDEX ctVtxPass = VTX_PER_PASS;

	__asm
	{
		push    ebx
		xor     ebx,ebx // pass offset

passLoop:
		mov     eax,D [ctVtxTotal]
		cmp     eax,VTX_PER_PASS 
		cmovge  eax,D [ctVtxPass]
		mov     D [ctVtxPass],eax

		// CALCULATE SHADES

		mov     ecx,D [ctVtxPass]
		lea     esi,[ebx*2+ebx] // *3
		lea     edi,[ebx+ecx*2] // +ecx*2 is for loc of temp array for shades (SWORDs)
		add     esi,D [pvNormals]
		add     edi,D [pulColors]

		shr     ecx,1
		jz      shadeRest
		// loop by 2

shadeLoop2:
		fld     D [esi]GFXNormal.nx
		fmul    D [fLightX]
		fld     D [esi]GFXNormal.ny
		fmul    D [fLightY]
		fld     D [esi]GFXNormal.nz
		fmul    D [fLightZ]

		fld     D [esi+3*4]GFXNormal.nx
		fmul    D [fLightX]
		fld     D [esi+3*4]GFXNormal.ny
		fmul    D [fLightY]             // ly*ny2, lx*nx2, lz*nz1, ly*ny1, lx*nx1
		fxch    st(4)                   // lx*nx1, lx*nx2, lz*nz1, ly*ny1, ly*ny2
		faddp   st(3),st(0)             // lx*nx2, lz*nz1, lx*nx1+ly*ny1, ly*ny2

		fld     D [esi+3*4]GFXNormal.nz
		fmul    D [fLightZ]             // lz*nz2, lx*nx2, lz*nz1, lx*nx1+ly*ny1, ly*ny2 

		fxch    st(2)                   // lz*nz1, lx*nx2, lz*nz2, lx*nx1+ly*ny1, ly*ny2 
		faddp   st(3),st(0)             // lx*nx2, lz*nz2, res1, ly*ny2 
		faddp   st(3),st(0)             // lz*nz2, res1, lx*nx2+ly*ny2 
		fxch    st(1)                   // res1, lz*nz2, lx*nx2+ly*ny2 
		fistp   D [edi]                 // lz*nz2, lx*nx2+ly*ny2 
		faddp   st(1),st(0)             // res2
		add     esi,2*12
		add     edi,2*2
		fistp   D [edi-1*2]
		dec     ecx
		jnz     shadeLoop2
shadeRest:
		// eventually do the last one
		test    D [ctVtxPass],1
		jz      shadeDone
		fld     D [esi]GFXNormal.nx
		fmul    D [fLightX]
		fld     D [esi]GFXNormal.ny
		fmul    D [fLightY]
		fld     D [esi]GFXNormal.nz
		fmul    D [fLightZ]
		fxch    st(2)
		faddp   st(1),st(0)
		faddp   st(1),st(0)
		fistp   D [edi]
shadeDone:

		// CALCULATE COLORS

		pxor    mm0,mm0
		// construct 64-bit RGBA light
		mov     eax,D [slLG]
		mov     edx,0x01FE0000
		shl     eax,16
		shl     edx,16
		or      eax,D [slLR]
		or      edx,D [slLB]
		movd    mm5,eax
		movd    mm7,edx
		psllq   mm7,32
		por     mm5,mm7
		psllw   mm5,1 // boost for multiply
		// construct 64-bit RGBA ambient
		mov     eax,D [slAG]
		mov     edx,D [slSA]
		shl     eax,16
		shl     edx,16
		or      eax,D [slAR]
		or      edx,D [slAB]
		movd    mm6,eax
		movd    mm7,edx
		psllq   mm7,32
		por     mm6,mm7

		// init
		mov     ecx,D [ctVtxPass]
		lea     edi,[ebx]
		add     edi,D [pulColors]
		lea     esi,[edi+ecx*2]   // temp SWORDs array of shades
		shr     ecx,2
		jz      colorRest2

		// 4-colors loop
colorLoop4:
		movq    mm1,Q [esi]
		packuswb mm1,mm0
		punpcklbw mm1,mm1
		movq    mm3,mm1
		punpcklwd mm1,mm1
		punpckhwd mm3,mm3
		movq    mm2,mm1
		movq    mm4,mm3
		punpckldq mm1,mm1
		punpckhdq mm2,mm2
		punpckldq mm3,mm3
		punpckhdq mm4,mm4
		pmulhw  mm1,mm5
		pmulhw  mm2,mm5
		pmulhw  mm3,mm5
		pmulhw  mm4,mm5
		paddsw  mm1,mm6
		paddsw  mm2,mm6
		paddsw  mm3,mm6
		paddsw  mm4,mm6
		packuswb mm1,mm2
		packuswb mm3,mm4
		movq    Q [edi+0],mm1
		movq    Q [edi+8],mm3
		add     esi,2*4
		add     edi,4*4
		dec     ecx
		jnz     colorLoop4

		// 2 colors rest
colorRest2:
		test    D [ctVtxPass],2
		jz      colorRest1
		movd    mm1,D [esi]
		packuswb mm1,mm0
		punpcklbw mm1,mm1
		punpcklwd mm1,mm1
		movq    mm2,mm1
		punpckldq mm1,mm1
		punpckhdq mm2,mm2
		pmulhw  mm1,mm5
		pmulhw  mm2,mm5
		paddsw  mm1,mm6
		paddsw  mm2,mm6
		packuswb mm1,mm2
		movq    Q [edi+0],mm1
		add     esi,1*4
		add     edi,2*4

		// 1 color rest
colorRest1:
		test    D [ctVtxPass],1
		jz      colorDone
		movd    mm1,D [esi]   // has wall
		packuswb mm1,mm0
		punpcklbw mm1,mm1
		punpcklwd mm1,mm1
		punpckldq mm1,mm1
		pmulhw  mm1,mm5
		paddsw  mm1,mm6
		packuswb mm1,mm0
		movd    D [edi],mm1
colorDone:

		// pass loop
		emms
		add     ebx,VTX_PER_PASS*4 // *4 because of dwords and 16 bytes normals alignment
		sub     D [ctVtxTotal],VTX_PER_PASS
		jg      passLoop

		// done
		pop     ebx
	}

#else

	// for each vertex color
	for(INDEX ivx=0;ivx<_ctVertices;ivx++) {
		// calculate vertex light
		FLOAT3D vNorm(_paNormals[ivx].nx,_paNormals[ivx].ny,_paNormals[ivx].nz);
		FLOAT fDot = vNorm % _vLightDir;
		fDot = Clamp(fDot,0.0f,1.0f);
		SLONG slDot = NormFloatToByte(fDot);
		_acolVtxColors[ivx].r = ClampUp( slAR + ((slLR*slDot)>>8), 255L);
		_acolVtxColors[ivx].g = ClampUp( slAG + ((slLG*slDot)>>8), 255L);
		_acolVtxColors[ivx].b = ClampUp( slAB + ((slLB*slDot)>>8), 255L);
		_acolVtxColors[ivx].a = colSurface.a;
	}

#endif // ASMOPT
}


// Calculate lightning for given model
extern void shaCalculateLightForSpecular(void)
{
	ASSERT(_paNormals!=NULL);
	_acolVtxColors.PopAll();
	_acolVtxColors.Push(_ctVertices);

	GFXColor  colModel   = (GFXColor)_colModel;     // Model color
	GFXColor &colAmbient = (GFXColor)_colAmbient;   // Ambient color
	GFXColor &colLight   = (GFXColor)_colLight;     // Light color
	GFXColor &colSurface = (GFXColor)_colConstant;  // shader color

	// colModel = MulColors(colModel.r,colSurface.abgr);
	colModel.MultiplyRGBA(colModel,colSurface);

	UBYTE ubColShift = 8;
	SLONG slar = colAmbient.r;
	SLONG slag = colAmbient.g;
	SLONG slab = colAmbient.b;

	if(shaOverBrightningEnabled()) {
		slar = ClampUp(slar,127L);
		slag = ClampUp(slag,127L);
		slab = ClampUp(slab,127L);
		ubColShift = 8;
	} else {
		slar*=2;
		slag*=2;
		slab*=2;
		ubColShift = 7;
	}

	// for each vertex color
	for(INDEX ivx=0;ivx<_ctVertices;ivx++) {
		// calculate vertex light
		FLOAT3D &vNorm = FLOAT3D(_paNormals[ivx].nx,_paNormals[ivx].ny,_paNormals[ivx].nz);
		FLOAT fDot = vNorm % _vLightDir;
		fDot = Clamp(fDot,0.0f,1.0f);
		SLONG slDot = NormFloatToByte(fDot);

		_acolVtxColors[ivx].r = ClampUp(colModel.r * (slar + ((colLight.r * slDot)>>ubColShift))>>8,255L);
		_acolVtxColors[ivx].g = ClampUp(colModel.g * (slag + ((colLight.g * slDot)>>ubColShift))>>8,255L);
		_acolVtxColors[ivx].b = ClampUp(colModel.b * (slab + ((colLight.b * slDot)>>ubColShift))>>8,255L);
		_acolVtxColors[ivx].a = slDot>0 ? 255 : 0;
	}
	// Set current wertex array 
	_pcolVtxColors = &_acolVtxColors[0];
}


// Clean vertex const registers
extern void CleanConstantRegisters(void)
{
	const ShaderRegister srNull(0.0f, 0.0f, 0.0f, 0.0f);
	const ShaderRegister asrNull[3] = {srNull,
																		 srNull,
																		 srNull};

	extern INDEX sha_iResetConstRegisters;
	// Clear vertex shader constant registers
	if(sha_iResetConstRegisters==1) {
		// Clean first 21 registers
		for(INDEX icr=0;icr<7;icr++) {
			shaSetVertexProgramConst(icr*3, asrNull, 3);
		}
	} else if(sha_iResetConstRegisters==2) {
		// Clean all 96 registers
		for(INDEX icr=0;icr<32;icr++) {
			shaSetVertexProgramConst(icr*3, asrNull, 3);
		}
	}
}

// Clean all values
extern void shaClean(void)
{
	_iFirstVertex = -1;
	_ctVertices   = -1;
	_ctIndices    = -1;
	_ctColors     = -1;
	_ctTextures   = -1;
	_ctUVMaps     = -1;
	_ctLights     = -1;

	_colConstant   = 0;
	_ulFlags       = 0;
	_ulRenFlags    = 0;
	_ctWeightsPerVertex = 0;

	_pShader        = NULL;
	_paVertices     = NULL;
	_paNormals      = NULL;
	_paWeights      = NULL;
	_puwIndices     = NULL;
	_paUVMaps       = NULL;
	_paTextures     = NULL;
	_paColors       = NULL;
	_paFloats       = NULL;
	_pCurrentUVMap  = NULL;
	_pcolVtxColors  = NULL;

	_iVertexBufferID   = -1;
	_iNormalBufferID   = -1;
	_iWeightBufferID   = -1;
	_iTexCoordBufferID = -1;
	_paiUVMapBufferIDs = NULL;

	_paFogUVMap     = NULL;
	_paHazeUVMap    = NULL;
	_pacolVtxHaze   = NULL;
	_pmObjToView    = NULL;
	_pmObjToViewStr = NULL;
	_pmObjToAbs     = NULL;
	_paprProjection = NULL;

	_iVertexProgram = -1;
	_iPixelProgram  = -1;
	_iFogTexUnit    = -1;
	_bOpaqueSurface = TRUE;
	
//  _bUseHWVertexBuffers = FALSE;
//  _bUseHWVertexProgram = FALSE;
//  _bUseHWPixelProgram  = FALSE;

#if _DEBUG
	for(INDEX ibm=0;ibm<_ctBoneMatrices;ibm++) {
		ASSERT(_pamBoneMatrices[ibm] != NULL);
		_pamBoneMatrices[ibm] = NULL;
	}
#endif

	extern INDEX sha_iResetConstRegisters;
	if(sha_iResetConstRegisters && _bUseHWVertexProgram) {
		// Clean constant registers
		CleanConstantRegisters();
	}

	// _paMatrices = NULL;
	_ctBoneMatrices = 0;

	_acolVtxColors.PopAll();
	_acolVtxModifyColors.PopAll();
	_vModifyVertices.PopAll();
	_uvUVMapForModify.PopAll();

	// for each available texture unit after first
	for(INDEX itu=1;itu<_pGfx->gl_ctTextureUnits;itu++) {
		// if texture unit was used
		if((_ulUsedTextureUnits>>itu)&1) {
			shaDisableTexture(itu);
		}
	}
	_ulUsedTextureUnits = 0;
	gfxSetTextureUnit(0);

	_pShader = NULL;
}

/*
 * Engine interface
 */

/* Software vertex proccesing */
// Set array of vertices
extern void shaSetVertexArray( GFXVertex *paVertices, INDEX iFirstVertex, INDEX ctVertices)
{
	ASSERT(paVertices!=NULL);
	ASSERT(ctVertices>0);
	ASSERT(iFirstVertex>=0 && iFirstVertex<ctVertices);

	// set pointer to new vertex array
	_paVertices = paVertices;
	_iFirstVertex = iFirstVertex;
	_ctVertices = ctVertices;

	// Set vertex array
	gfxSetVertexArray( &paVertices[_iFirstVertex], _ctVertices);
}

// Set array of indices
extern void shaSetIndices(const UWORD *puwIndices, INDEX ctIndices)
{
	ASSERT( puwIndices!=NULL && ctIndices>0);
	_puwIndices = puwIndices;
	_ctIndices  = ctIndices;
}

// set array of normals
extern void shaSetNormalArray( GFXNormal *paNormals)
{
	ASSERT(_paVertices!=NULL); // vertices must be set before normals
	ASSERT( paNormals!=NULL);
	// set pointer to new normals array
	_paNormals = paNormals;
	if(_bUseHWVertexProgram) {
		gfxSetNormalArray(&paNormals[_iFirstVertex]);
	}

	// set normals if lighting is enabled (!!!! must implement truform)
	extern BOOL GFX_bLighting;
	// if( GFX_bLighting) gfxSetNormalArray(_paNormals);
}

extern void shaSetWeightArray( GFXWeight *paWeights)
{
	ASSERT(_paVertices!=NULL); // weights must be set before normals
	ASSERT(paWeights!=NULL);
	ASSERT(_bUseHWVertexProgram);
	_paWeights = paWeights;
	gfxSetWeightArray(&paWeights[_iFirstVertex]);
}

// Set array of uv maps
extern void shaSetSurfaceUVMaps(GFXTexCoord **paUVMaps, INDEX ctUVMaps)
{
	ASSERT(paUVMaps!=NULL);
	ASSERT(ctUVMaps>0);

	_paUVMaps = paUVMaps;
	_ctUVMaps = ctUVMaps;
}

/* Hardware vertex proccesing */
extern void shaSetVertexBufferID(INDEX iVertexBufferID, INDEX iFirstVertex, INDEX ctVertices)
{
	ASSERT(_bUseHWVertexBuffers); // must use hardware shaders
	ASSERT(iVertexBufferID>=0);
	ASSERT(ctVertices>0);
	ASSERT(iFirstVertex>=0);
	_iVertexBufferID = iVertexBufferID;
	_iFirstVertex = iFirstVertex;
	_ctVertices = ctVertices;

	// Set vertex buffer ID
	gfxSetVertexSubBuffer(_iVertexBufferID,_iFirstVertex,ctVertices);
}

extern void shaSetNormalBufferID(INDEX iNormalBufferID)
{
	ASSERT(_bUseHWVertexBuffers); // must use hardware shaders
	ASSERT(_iVertexBufferID>=0); // vertex buffer must be set before normal buffer
	ASSERT(iNormalBufferID>=0);
	_iNormalBufferID = iNormalBufferID;

	// Set normal buffer ID
	gfxSetNormalSubBuffer(_iNormalBufferID, -1);
}

//안태훈 수정 시작	//(Add Tagent-space Normal Map)(0.1)
extern void shaSetTangentBufferID(INDEX iTagentBufferID)
{
	ASSERT(_bUseHWVertexBuffers); // must use hardware shaders
	ASSERT(_iVertexBufferID >= 0); // vertex buffer must be set before normal buffer
	ASSERT(iTagentBufferID >= 0);
	_iTangentBufferID = iTagentBufferID;

	// Set tangent buffer ID
	gfxSetTangentSubBuffer(_iTangentBufferID, -1);
}
//안태훈 수정 끝	//(Add Tagent-space Normal Map)(0.1)

extern void shaSetWeightBufferID(INDEX iWeightBufferID)
{
	ASSERT(_bUseHWVertexBuffers); // must use hardware shaders
	ASSERT(_iVertexBufferID>=0); // vertex buffer must be set before weight buffer
	ASSERT(iWeightBufferID>=0);
	_iWeightBufferID = iWeightBufferID;

	// Set weight buffer ID
	gfxSetWeightSubBuffer(_iWeightBufferID, -1);
}

extern void shaSetTexCoordBufferID(INDEX iTexCoordBufferID, INDEX iTextureUnit/*=0*/)
{
	ASSERT(iTexCoordBufferID>=0);
	ASSERT(iTextureUnit<4);
	_iTexCoordBufferID = iTexCoordBufferID;
	gfxSetTexCoordSubBuffer(iTexCoordBufferID,iTextureUnit);
}

extern void shaSetSurfaceUVMapsIDs(INDEX *paiUVMapBufferIDs, INDEX ctUVMaps)
{
	ASSERT(_bUseHWVertexBuffers); // must use hardware shaders
	ASSERT(paiUVMapBufferIDs!=NULL);
	ASSERT(ctUVMaps>0);

	_paiUVMapBufferIDs = paiUVMapBufferIDs;
	_ctUVMaps = ctUVMaps;
}

extern void shaSetBoneMatrix(Matrix12 const *paMatrix, INDEX iBone)
{
	ASSERT(paMatrix!=NULL);
	ASSERT(iBone>=0 && iBone<MAX_WEIGHTS_PER_SURFACE);
#if _DEBUG
	ASSERT(_pamBoneMatrices[iBone] == NULL);
#endif

	// Remember pointer to given matrix
	_pamBoneMatrices[iBone] = paMatrix;
	_ctBoneMatrices++;
	ASSERT(_ctBoneMatrices<=MAX_WEIGHTS_PER_SURFACE);
}

/*
// Set constant registers values
extern void shaSetBoneMatrices(Matrix12 *paMatrices, INDEX ctMatrices)
{
	ASSERT(paMatrices!=NULL);
	_paMatrices = paMatrices;
	_ctBoneMatrices = ctMatrices;
}
*/

// Set number of weights per vertex to be used in vertex program
extern void shaSetWeightsPerVertex(INDEX ctWeightsPerVertex)
{
	ASSERT(ctWeightsPerVertex>=0 && ctWeightsPerVertex<5);
	_ctWeightsPerVertex=ctWeightsPerVertex;
}


// Set array of texture objects for shader
extern void shaSetSurfaceTextures(CTextureObject **paTextureObject, INDEX ctTextures)
{
	_paTextures = paTextureObject;
	_ctTextures = ctTextures;
}

// Set array of shader colors
extern void shaSetSurfaceColors(COLOR *paColors, INDEX ctColors)
{
	ASSERT(paColors!=NULL);
	ASSERT(ctColors>0);

	_paColors = paColors;
	_ctColors = ctColors;
}

// Set array of floats for shader
extern void shaSetSurfaceFloats(FLOAT *paFloats, INDEX ctFloats)
{
	ASSERT(paFloats!=NULL);
	_paFloats = paFloats;
	_ctFloats = ctFloats;
}

// Set shading flags
extern void shaSetSurfaceFlags(ULONG ulFlags)
{
	_ulFlags = ulFlags;
}

// Set base color of model 
extern void shaSetModelColor(COLOR &colModel)
{
	_colModel = colModel;
}


// Set light direction
extern void shaSetLightDirection( const FLOAT3D &vLightDir)
{
	_vLightDir = vLightDir;
	_vLightDir.Normalize();

	// prepare HW light
	//gfxSetLightDirection( 0, vLightDir);
}


// Set light color
extern void shaSetLightColor( COLOR colAmbient, COLOR colLight)
{
	_colLight   = colLight;   // |0xFF ?
	_colAmbient = colAmbient; // |0xFF ?

	// prepare HW light
	//gfxSetLightColor( 0, colLight, colAmbient, 0);
}


// Set object to view matrix
extern void shaSetObjToViewMatrix(Matrix12 &mat)
{
	_pmObjToView = &mat;
}

extern void shaSetObjToViewStrMatrix(Matrix12 &mat)
{
	_pmObjToViewStr = &mat;
}


// Set object to abs matrix
extern void shaSetObjToAbsMatrix(Matrix12 &mat)
{
	_pmObjToAbs = &mat;
}

// Set object rendering flags (fog,haze,etc)
extern void shaSetRenFlags(ULONG ulRenFlags)
{
	_ulRenFlags = ulRenFlags;
}

// Lock vertex array
extern void shaLockArrays(void)
{
	extern BOOL CVA_bModels;
	if( CVA_bModels) gfxLockArrays();
}

// Unlock vertex array
extern void shaUnlockArrays(void)
{
	gfxUnlockArrays();
}




// Set current texture index
extern void shaSetTexture(INDEX iTextureIndex, INDEX iTextureUnit/*=-1*/)
{
	if(_paTextures==NULL || iTextureIndex<0 || iTextureIndex>=_ctTextures ||  _paTextures[iTextureIndex] == NULL) {
		shaDisableTexture(iTextureUnit);
		return;
	}
	ASSERT(iTextureIndex<_ctTextures);

	CTextureObject *pto = _paTextures[iTextureIndex];
	ASSERT(pto!=NULL);

	CTextureData *pTextureData = (CTextureData*)pto->GetData();
	const INDEX iFrameNo = pto->GetFrame();
	
	if(iTextureUnit!=(-1)) {
		shaSetTextureUnit(iTextureUnit);
	}
	pTextureData->SetAsCurrent(iFrameNo);
}

extern void shaSetTexture(CTextureData *ptd, INDEX iTextureUnit/*=-1*/)
{
	if(ptd==NULL) {
		shaDisableTexture(iTextureUnit);
		return;
	}
	if(iTextureUnit!=(-1)) {
		shaSetTextureUnit(iTextureUnit);
	}
	ptd->SetAsCurrent();
}

extern void shaDisableTexture(INDEX iTextureUnit/*=-1*/)
{
	if(iTextureUnit!=(-1)) {
		shaSetTextureUnit(iTextureUnit);
	}
	gfxDisableTexture();
}


// Set current uvmap index
extern void shaSetUVMap(INDEX iUVMapIndex, INDEX iTextureUnit/*=0*/)
{
	ASSERT(iUVMapIndex>=0);
	if(iUVMapIndex<=_ctUVMaps) {
		// if in software
		if(!_bUseHWVertexBuffers) {
			GFXTexCoord *patc = _paUVMaps[iUVMapIndex];
			if(patc!=NULL) {
				shaSetTexCoordArray(patc,iTextureUnit);
			}
		// if in hardware
		} else {
			INDEX iTexCoordBufferID = _paiUVMapBufferIDs[iUVMapIndex];
			// if buffer is valid 
			if(iTexCoordBufferID>=0) {
				// Set texcoord buffer ID
				shaSetTexCoordBufferID(iTexCoordBufferID,iTextureUnit);
			}
		}
	}
}

// Set current color index
extern void shaSetColor(INDEX icolIndex)
{
	ASSERT(icolIndex>=0);

	if(icolIndex>=_ctColors) {
		_colConstant = C_WHITE|CT_OPAQUE;
	} else {
		_colConstant = _paColors[icolIndex];
	}
}

// Set current texture unit
extern void shaSetTextureUnit(INDEX iTextureUnit)
{
	ASSERT(iTextureUnit>=0 && iTextureUnit<32 && iTextureUnit<_pGfx->gl_ctTextureUnits);

	// remember that this texture unit was used
	_ulUsedTextureUnits |= (1UL<<iTextureUnit);

	gfxSetTextureUnit(iTextureUnit);
}

//안태훈 수정 시작	//(Add Tagent-space Normal Map)(0.1)
static void shaSetVertexProgram_Default(INDEX iVertexProgram)
{
	ASSERT(_pShader!=NULL);
	ASSERT(iVertexProgram != (-1));
	ASSERT(_ctWeightsPerVertex>=0 && _ctWeightsPerVertex<5);
	ASSERT(iVertexProgram < _pShader->sh_aVPHandles.Count());
	// ASSERT(_iFogTexUnit>=0 && _iFogTexUnit<_pGfx->gl_ctTextureUnits);

	INDEX ivp = iVertexProgram;
	const BOOL bHasFog = (_ulRenFlags&SHA_RMF_HAZE || _ulRenFlags&SHA_RMF_FOG) && _iFogTexUnit>=0;
	// if in fog or haze
	if(bHasFog) {
		// use vertex program for fog
		ivp += _pShader->sh_ctVertexPrograms;
	}

	_iVertexProgram = ivp;

	// Get vertex program handle
	VertexProgramHandle &vph = _pShader->sh_aVPHandles[ivp];
	ULONG &ulHandle = vph.vph_ulHandles[_ctWeightsPerVertex];
	
	// if handle is invalid
	if(ulHandle==NONE) {
		TRACKMEM(mem,"Shaders");
		// All shaders are obtained at ssam startup and will be compiled only once
		MEMTRACK_SETFLAGS(mem,MTF_EOLIST|MTF_NOSTACKTRACE);
		// if handle is invalid and shaders arn't precaching now
		if(0 && !_bPrecachingShaders) {
//안태훈 수정 시작	//(For Performance)(0.1)
			ShaderDesc *pshd;
			_pShader->GetShaderDesc(pshd);
			CPrintF("Shader %s is not precached!\n",(const char*)pshd->sd_strShaderInfo);
//안태훈 수정 끝	//(For Performance)(0.1)
		}
		// Create new vertex program
		ULONG ulStreamFlags = vph.vph_ulStreamFlags;
		// if weights are enabled 
		if(_ctWeightsPerVertex>0) {
			// use weight stream
			ulStreamFlags|=GFX_WEIGHT_STREAM;
		}

		ASSERT(_pShader->GetShaderVPCode!=NULL);

		// Get vertex program code from shader
		CTString strUserProgram;
		CTString strFullProgram;

		const BOOL bNaked = ulStreamFlags&SHA_NAKED_CODE;	//Terrain쪽에서 쓰기 위해 만든 플래그인듯...
		// if naked code 
		if(bNaked) {
			// Get full program code
			_pShader->GetShaderVPCode(strFullProgram,iVertexProgram);
			ulStreamFlags&=~SHA_NAKED_CODE;
		} else {
			// Assamble full vertex program code
			_pShader->GetShaderVPCode(strUserProgram,iVertexProgram);
			strFullProgram = AssembleVertexProgram(strUserProgram,ulStreamFlags);
			// if this is vertex program for fog
			if(bHasFog) {
				// add fog code to vertex program
				strFullProgram+=GenerateVertexProgramFogCode(_iFogTexUnit);
			}
		}
/*		if (!_pShader->m_TestShaderLog.IsEmptyqueBuffer())
		{ // Vertex Shader만 변할때
			CTString tempFileName;
			tempFileName.PrintF(("ShdaerCodeExport_%d.txt"), tempFileNameCount);
			_pShader->m_TestShaderLog.WriteBin((const char*)tempFileName.str_String);
			tempFileNameCount++;
		}

		_pShader->m_TestShaderLog.AddString((const char*)strFullProgram.str_String);
*/
		// Compile vertex program with provided code
		// extern ULONG shaCompileVertexProgram(const char *pstrCode, ULONG ulStreamFlags);
		// ulHandle = shaCompileVertexProgram((const char*)strFullProgram,ulStreamFlags);
		ulHandle = gfxCreateVertexProgram((const char*)strFullProgram,ulStreamFlags);
		// Must not be null
		ASSERT(ulHandle!=NONE);
	}

	// if handle is still invalid
	if(ulHandle!=NONE && !_bPrecachingShaders) {
		// Set current vertex program
		gfxSetVertexProgram(ulHandle);
	}
}

static void shaSetVertexProgram_NMMObjSpace(INDEX iVertexProgram)
{
	ASSERT(FALSE && "아직 구현되지 않은 부분임");
}

static void shaSetVertexProgram_NMMTanSpace(INDEX iVertexProgram)
{
	ASSERT(_pShader!=NULL);
	ASSERT(iVertexProgram != (-1));
	ASSERT(_ctWeightsPerVertex>=0 && _ctWeightsPerVertex<5);
	ASSERT(iVertexProgram < _pShader->sh_aVPHandles.Count());

	//--- Block A Begin ---//
	INDEX ivp = iVertexProgram;
	//NotYet : 아직 포그와 연동되는 부분은 작성하지 않았음.
	const BOOL bHasFog = (_ulRenFlags & SHA_RMF_HAZE || _ulRenFlags & SHA_RMF_FOG) && _iFogTexUnit >= 0;
	// if in fog or haze
	if(bHasFog)
	{
		// use vertex program for fog
		ivp += _pShader->sh_ctVertexPrograms;
	}
	_iVertexProgram = ivp;
	// Get vertex program handle
	VertexProgramHandle &vph = _pShader->sh_aVPHandles[ivp];
	ULONG &ulHandle = vph.vph_ulHandles[_ctWeightsPerVertex];
	//--- Block A End ---//
	//--- Block A 해설 ---//
	//CShader에서 GetDesc로 Shader작성자가 설정한 VP의 개수를 받아 *2를 한후 sha_aVPHandles를 만든다.
	//그리고 포그나 헤이즈가 있으면 원래의 VP의 index에 개수를 더해 *2로 인해 추가된 부분의 VP를 사용한다.
	//그것은 이 뒷부분에 AssembleShaderProgram뒤에 처리된다.
	
	// if handle is invalid
	if(ulHandle==NONE) {
		TRACKMEM(mem,"Shaders");
		// All shaders are obtained at ssam startup and will be compiled only once
		MEMTRACK_SETFLAGS(mem,MTF_EOLIST|MTF_NOSTACKTRACE);
		// Create new vertex program
		ULONG ulStreamFlags = vph.vph_ulStreamFlags;
		// if weights are enabled 
		if(_ctWeightsPerVertex > 0)
		{
			// use weight stream
			ulStreamFlags|=GFX_WEIGHT_STREAM;
		}
		
		ASSERT(_pShader->GetShaderVPCode!=NULL);

		// Get vertex program code from shader
		CTString strUserProgram;
		CTString strFullProgram;

		// Assamble full vertex program code
		_pShader->GetShaderVPCode(strUserProgram,iVertexProgram);
		switch(_ctWeightsPerVertex) {
			case 0: strFullProgram = CTString(strNoWeights_NMTangentSpaceB   ) + strUserProgram; break;
			case 1: strFullProgram = CTString(strOneWeight_NMTangentSpaceB   ) + strUserProgram; break;
			case 2: strFullProgram = CTString(strTwoWeights_NMTangentSpaceB  ) + strUserProgram; break;
			case 3: strFullProgram = CTString(strThreeWeights_NMTangentSpaceB) + strUserProgram; break;
			case 4: strFullProgram = CTString(strFourWeights_NMTangentSpaceB ) + strUserProgram; break;
			default:
				ASSERTALWAYS("어랏? 버텍스당 뼈다귀의 수가 너무 많거나 코드에 버그가 있소. 주의하시오.");
			break;
		}
		
		// if this is vertex program for fog, add fog code to vertex program
		//TODO : 아직 포그와 연동되는 부분은 작성하지 않았음.
		if(bHasFog) strFullProgram+=GenerateVertexProgramFogCode(_iFogTexUnit);

		//_pShader->m_TestShaderLog.AddString((const char*)strFullProgram.str_String);

		// Compile vertex program with provided code
		// extern ULONG shaCompileVertexProgram(const char *pstrCode, ULONG ulStreamFlags);
		// ulHandle = shaCompileVertexProgram((const char*)strFullProgram,ulStreamFlags);
		ulHandle = gfxCreateVertexProgram((const char*)strFullProgram,ulStreamFlags);
		// Must not be null
		ASSERT(ulHandle!=NONE);
	}

	// if handle is still invalid
	if(ulHandle!=NONE && !_bPrecachingShaders)
	{
		// Set current vertex program
		gfxSetVertexProgram(ulHandle);
	}
}

extern void shaSetVertexProgram(INDEX iVertexProgram, NormalMapMeshType nmmType/* = NMMT_NONE*/)
{
	switch(nmmType)
	{
	case NMMT_NONE:
		{
			shaSetVertexProgram_Default(iVertexProgram);
		} break;
	case NMMT_OBJSPACE:
		{
			shaSetVertexProgram_NMMObjSpace(iVertexProgram);
		} break;
	case NMMT_TANSPACE:
		{
			shaSetVertexProgram_NMMTanSpace(iVertexProgram);
		} break;
	default:
		{ ASSERT(FALSE && "잘못된 Normal map Mesh Type입니다. 대략 즐..."); }
	}
}
//안태훈 수정 끝	//(Add Tagent-space Normal Map)(0.1)

extern void shaSetPixelProgram(INDEX iPixelProgram)
{
	ASSERT(_pShader!=NULL);
	ASSERT(iPixelProgram != (-1));
	// ASSERT(_iFogTexUnit>=0 && _iFogTexUnit<_pGfx->gl_ctTextureUnits);

	INDEX ipp = iPixelProgram;
	INDEX iFogTexUnit = -1;
	const BOOL bHasFog = (_ulRenFlags&SHA_RMF_HAZE||_ulRenFlags&SHA_RMF_FOG) && _iFogTexUnit>=0;
	FOGTYPE eFogType = FT_NONE;

	if(bHasFog) {
		if(_bOpaqueSurface) {
			ipp += _pShader->sh_ctPixelPrograms;
			eFogType = FT_OPAQUE;
		} else {
			ipp += _pShader->sh_ctPixelPrograms*2;
			eFogType = FT_NON_OPAQUE;
		}
		iFogTexUnit = _iFogTexUnit;
	}
	_iPixelProgram = iPixelProgram;
	

	PixelProgramHandle &pph = _pShader->sh_aPPHandles[ipp];
	ULONG &ulHandle = pph.pph_ulHandle;
	// if pixel program handle is invalid
	if(ulHandle==NONE) {
		TRACKMEM(mem,"Shaders");
		// All shaders are obtained at ssam startup and will be compiled only once
		MEMTRACK_SETFLAGS(mem,MTF_EOLIST|MTF_NOSTACKTRACE);

		ASSERT(_pShader->GetShaderPPCode!=NULL);

		// Get pixel program code from shader
		CTString strUserProgram;
		_pShader->GetShaderPPCode(strUserProgram,iPixelProgram,eFogType);
		ShaderDesc *pshDesc;
		_pShader->GetShaderDesc(pshDesc);
		// Assamble full pixel program code
		CTString strFullProgram = AssemblePixelProgram(strUserProgram,pshDesc->PixelShaderVersion,iFogTexUnit);
		// Compile pixel program with provided code
		ulHandle = gfxCreatePixelProgram((const char*)strFullProgram);
		// Must not be null
		ASSERT(ulHandle!=NONE);
/*		_pShader->m_TestShaderLog.AddString("\n");
		_pShader->m_TestShaderLog.AddString((const char*)strFullProgram.str_String);
		CTString tempFileName;
		tempFileName.PrintF(("ShdaerCodeExport_%d.txt"), tempFileNameCount);
		_pShader->m_TestShaderLog.WriteBin((const char*)tempFileName.str_String);
		tempFileNameCount++;*/
	}
	// if handle is still invalid
	if(ulHandle!=NONE) {
		// Set current pixel program
		gfxSetPixelProgram(ulHandle);
	}
}

// Set array of texcoords index
extern void shaSetTexCoordArray(GFXTexCoord *uvNewMap, INDEX iTextureUnit/*=0*/)
{
	_pCurrentUVMap = uvNewMap;
	shaSetTextureUnit(iTextureUnit);
	gfxSetTexCoordArray(_pCurrentUVMap, FALSE);
}

// Set array of vertex colors
extern void shaSetColorArray(GFXColor *paColors)
{
	_pcolVtxColors = paColors;
}

// Set constant color
extern void shaSetConstantColor(const COLOR colConstant)
{
	gfxSetConstantColor(colConstant);
}

// Set texture matrix
extern void shaSetTextureMatrix(const Matrix12 &mTexture, INDEX iTextureUnit/*=-1*/)
{
	if(iTextureUnit!=(-1)) {
		shaSetTextureUnit(iTextureUnit);
	}

	FLOAT mTex16[16];
	mTex16[ 0] = mTexture[ 0];  mTex16[ 1] = mTexture[ 4];  mTex16[ 2] = mTexture[ 8];  mTex16[ 3] = 0.0f;
	mTex16[ 4] = mTexture[ 1];  mTex16[ 5] = mTexture[ 5];  mTex16[ 6] = mTexture[ 9];  mTex16[ 7] = 0.0f;
	mTex16[ 8] = mTexture[ 2];  mTex16[ 9] = mTexture[ 6];  mTex16[10] = mTexture[10];  mTex16[11] = 0.0f;
	mTex16[12] = mTexture[ 3];  mTex16[13] = mTexture[ 7];  mTex16[14] = mTexture[11];  mTex16[15] = 1.0f;

	gfxSetTextureMatrix(mTex16);
}

extern void shaClearTextureMatrix(INDEX iTextureUnit/*=-1*/)
{
	if(iTextureUnit!=(-1)) {
		shaSetTextureUnit(iTextureUnit);
	}
	gfxSetTextureMatrix(NULL);
}

/*
 * Shader value getting
 */ 

// Get first vertex
extern INDEX shaGetFirstVertex(void)
{
	return _iFirstVertex;
}

// Get vertex count
extern INDEX shaGetVertexCount(void)
{
	return _ctVertices;
}

// Get index count
extern INDEX shaGetIndexCount(void)
{
	return _ctIndices;
}

// Get float from array of floats
extern FLOAT shaGetFloat(INDEX iFloatIndex)
{
	ASSERT(iFloatIndex>=0);
	ASSERT(iFloatIndex<_ctFloats);
	return _paFloats[iFloatIndex];
}

// Get texture from array of textures
extern CTextureObject *shaGetTexture( INDEX iTextureIndex)
{
	ASSERT( iTextureIndex>=0);
	if( _paTextures==NULL || iTextureIndex>=_ctTextures || _paTextures[iTextureIndex]==NULL) return NULL;
	else return _paTextures[iTextureIndex];
}

// Get color from color array
extern COLOR shaGetColor(INDEX iColorIndex)
{
	ASSERT(iColorIndex<_ctColors);
	return _paColors[iColorIndex];
}

// Get shading flags
extern ULONG &shaGetFlags()
{
	return _ulFlags;
}

// Get base color of model
extern COLOR shaGetModelColor(void)
{
	return _colModel;
}

// Get light direction
extern FLOAT3D shaGetLightDirection(void)
{
	return _vLightDir;
}

// Get current light color
extern COLOR shaGetLightColor(void)
{
	return ByteSwap(_colLight.abgr);
}

// Get current ambient volor
extern COLOR shaGetAmbientColor(void)
{
	return ByteSwap(_colAmbient.abgr);
}

// Get current set color
extern COLOR shaGetCurrentColor(void)
{
	return _colConstant;
}

// Get vertex array
extern GFXVertex *shaGetVertexArray(void)
{
	return _paVertices;
}

// Get index array
extern const UWORD *shaGetIndexArray(void)
{
	return _puwIndices;
}

// Get normal array
extern GFXNormal *shaGetNormalArray(void)
{
	return _paNormals;
}

extern GFXWeight *shaGetWeightArray(void)
{
	return _paWeights;
}
// Get uvmap array from array of uvmaps
extern GFXTexCoord *shaGetUVMap(INDEX iUVMapIndex)
{
	ASSERT(!_bUseHWVertexBuffers);
	ASSERT( iUVMapIndex>=0);
	if( iUVMapIndex>=_ctUVMaps) return NULL;
	else return _paUVMaps[iUVMapIndex];
}

// Get pointer to current color array
extern GFXColor *shaGetColorArray(void)
{
	return _pcolVtxColors;
}


// Get empty color array for modifying
extern GFXColor *shaGetNewColorArray(void)
{
	ASSERT(_ctVertices!=0);
	ASSERT(_iFirstVertex==0);
	_acolVtxModifyColors.PopAll();
	_acolVtxModifyColors.Push(_ctVertices);
	return &_acolVtxModifyColors[0];
}

// Get empty texcoords array for modifying
extern GFXTexCoord *shaGetNewTexCoordArray(void)
{
	ASSERT(_ctVertices!=0);
	ASSERT(_iFirstVertex==0);
	_uvUVMapForModify.PopAll();
	_uvUVMapForModify.Push(_ctVertices);
	return &_uvUVMapForModify[0];
}

// Get empty vertex array for modifying
extern GFXVertex *shaGetNewVertexArray(void)
{
	ASSERT(_ctVertices!=0);
	ASSERT(_iFirstVertex==0);
	_vModifyVertices.PopAll();
	_vModifyVertices.Push(_ctVertices);
	return &_vModifyVertices[0];
}

// Get current projection
extern CAnyProjection3D *shaGetProjection()
{
	return _paprProjection;
}

// Get object to view matrix
extern Matrix12 *shaGetObjToViewMatrix(void)
{
	ASSERT(_pmObjToView!=NULL);
	return _pmObjToView;
}

extern Matrix12 *shaGetObjToViewStrMatrix(void)
{
	ASSERT(_pmObjToViewStr!=NULL);
	return _pmObjToViewStr;
}
// Get object to abs matrix
extern Matrix12 *shaGetObjToAbsMatrix(void)
{
	ASSERT(_pmObjToAbs!=NULL);
	return _pmObjToAbs;
}
// Get object rendering flags (fog,haze,etc)
extern ULONG shaGetRenFlags(void)
{
	return _ulRenFlags;
}

//강동민 수정 시작		03.17
/*
extern void shaSetWaterInformation(CNiceWater* pInfo)
{
	ASSERT(pInfo != NULL);
	_pWaterInformation = pInfo;
}
*/
//강동민 수정 끝		03.17



/*
 * Shader states
 */

// Set face culling
extern void shaCullFace(GfxFace eFace)
{
	gfxCullFace(eFace);
}

// Set blending operations
extern void shaBlendFunc(GfxBlend eSrc, GfxBlend eDst)
{
	gfxBlendFunc(eSrc,eDst);
}

// Enable/Disable blening
extern void shaEnableBlend(void)
{
	gfxEnableBlend();
}
extern void shaDisableBlend(void)
{
	gfxDisableBlend();
}

// Enable/Disable alpha test
extern void shaEnableAlphaTest(void)
{
	gfxEnableAlphaTest();
}
extern void shaDisableAlphaTest(void)
{
	gfxDisableAlphaTest();
}

// Enable/Disable depth test
extern void shaEnableDepthTest(void)
{
	gfxEnableDepthTest();
}
extern void shaDisableDepthTest(void)
{
	gfxDisableDepthTest();
}

// Enable/Disable depth write
extern void shaEnableDepthWrite(void)
{
	gfxEnableDepthWrite();
}
extern void shaDisableDepthWrite(void)
{
	gfxDisableDepthWrite();
}

// Set depth buffer compare mode
extern void shaDepthFunc(GfxComp eComp)
{
	gfxDepthFunc(eComp);
}

// Set texture wrapping 
extern void shaSetTextureWrapping( enum GfxWrap eWrapU, enum GfxWrap eWrapV, INDEX iTextureUnit/*=-1*/)
{
	if(iTextureUnit!=(-1)) {
		shaSetTextureUnit(iTextureUnit);
	}
	gfxSetTextureWrapping(eWrapU,eWrapV);
}

// Enable/Disable overbrightning
extern void shaEnableOverBrightning(void)
{
	if(shaOverBrightningEnabled()) {
		gfxSetTextureModulation(2);
	}
}
extern void shaDisableOverBrightning(void)
{
	if(shaOverBrightningEnabled()) {
		gfxSetTextureModulation(1);
	}
}

// Enable/Disable vertex program
extern void shaEnableVertexBuffers(void) {
	_bUseHWVertexBuffers = TRUE;
}
extern void shaEnableVertexProgram(void) {
	_bUseHWVertexProgram = TRUE;
	gfxEnableVertexProgram();
}
extern void shaEnablePixelProgram(void) {
	_bUseHWPixelProgram = TRUE;
	gfxEnablePixelProgram();
}
extern void shaDisableVertexBuffers(void) {
	_bUseHWVertexBuffers = FALSE;
}
extern void shaDisableVertexProgram(void) {
	_bUseHWVertexProgram = FALSE;
	gfxDisableVertexProgram();
}
extern void shaDisablePixelProgram(void) {
	_bUseHWPixelProgram = FALSE;
	gfxDisablePixelProgram();
}
extern BOOL shaUseHWShaders(void) {
	return _bUseHWVertexProgram && _bUseHWPixelProgram;
}
extern BOOL shaUseVertexProgram(void) {
	return _bUseHWVertexProgram;
}
extern BOOL shaUsePixelProgram(void) {
	return _bUseHWPixelProgram;
}
extern BOOL shaUseVertexBuffers(void) {
	return _bUseHWVertexBuffers;
}

// Is bbox in fog (shaInitSharedFogAndHazeParams must be called)
extern BOOL shaHasFog(FLOATaabbox3D &bbox)
{
	GFXVertex vtx;
	GFXTexCoord tex;
	FLOAT3D &vMin = bbox.minvect;
	FLOAT3D &vMax = bbox.maxvect;
	vtx.x=vMin(1); vtx.y=vMin(2); vtx.z=vMin(3); GetFogMapInVertex(vtx,tex); if(InFog(tex.t)) return TRUE;
	vtx.x=vMin(1); vtx.y=vMin(2); vtx.z=vMax(3); GetFogMapInVertex(vtx,tex); if(InFog(tex.t)) return TRUE;
	vtx.x=vMin(1); vtx.y=vMax(2); vtx.z=vMin(3); GetFogMapInVertex(vtx,tex); if(InFog(tex.t)) return TRUE;
	vtx.x=vMin(1); vtx.y=vMax(2); vtx.z=vMax(3); GetFogMapInVertex(vtx,tex); if(InFog(tex.t)) return TRUE;
	vtx.x=vMax(1); vtx.y=vMin(2); vtx.z=vMin(3); GetFogMapInVertex(vtx,tex); if(InFog(tex.t)) return TRUE;
	vtx.x=vMax(1); vtx.y=vMin(2); vtx.z=vMax(3); GetFogMapInVertex(vtx,tex); if(InFog(tex.t)) return TRUE;
	vtx.x=vMax(1); vtx.y=vMax(2); vtx.z=vMin(3); GetFogMapInVertex(vtx,tex); if(InFog(tex.t)) return TRUE;
	vtx.x=vMax(1); vtx.y=vMax(2); vtx.z=vMax(3); GetFogMapInVertex(vtx,tex); if(InFog(tex.t)) return TRUE;
	return FALSE;
}

// Is bbox in haze (shaInitSharedFogAndHazeParams must be called)
extern BOOL shaHasHaze( FLOATaabbox3D &bbox)
{
	FLOAT fS;
	GFXVertex vtx;                                
	FLOAT3D &vMin = bbox.minvect;
	FLOAT3D &vMax = bbox.maxvect;
	vtx.x=vMin(1); vtx.y=vMin(2); vtx.z=vMin(3); GetHazeMapInVertex(vtx,fS); if(InHaze(fS)) return TRUE;
	vtx.x=vMin(1); vtx.y=vMin(2); vtx.z=vMax(3); GetHazeMapInVertex(vtx,fS); if(InHaze(fS)) return TRUE;
	vtx.x=vMin(1); vtx.y=vMax(2); vtx.z=vMin(3); GetHazeMapInVertex(vtx,fS); if(InHaze(fS)) return TRUE;
	vtx.x=vMin(1); vtx.y=vMax(2); vtx.z=vMax(3); GetHazeMapInVertex(vtx,fS); if(InHaze(fS)) return TRUE;
	vtx.x=vMax(1); vtx.y=vMin(2); vtx.z=vMin(3); GetHazeMapInVertex(vtx,fS); if(InHaze(fS)) return TRUE;
	vtx.x=vMax(1); vtx.y=vMin(2); vtx.z=vMax(3); GetHazeMapInVertex(vtx,fS); if(InHaze(fS)) return TRUE;
	vtx.x=vMax(1); vtx.y=vMax(2); vtx.z=vMin(3); GetHazeMapInVertex(vtx,fS); if(InHaze(fS)) return TRUE;
	vtx.x=vMax(1); vtx.y=vMax(2); vtx.z=vMax(3); GetHazeMapInVertex(vtx,fS); if(InHaze(fS)) return TRUE;
	return FALSE;
}


// Set uvmap for fog
extern void shaSetFogUVMap(GFXTexCoord *paFogUVMap)
{
	_paFogUVMap = paFogUVMap;
}

// Set uvmap for haze
extern void shaSetHazeUVMap(GFXTexCoord *paHazeUVMap)
{
	_paHazeUVMap = paHazeUVMap;
}

// Set array of vertex colors used in haze
extern void shaSetHazeColorArray(GFXColor *paHazeColors)
{
	_pacolVtxHaze = paHazeColors;
}

extern BOOL shaOverBrightningEnabled(void)
{
	// determine multitexturing capability for overbrighting purposes
	extern INDEX mdl_bAllowOverbright;
	return mdl_bAllowOverbright && _pGfx->gl_ctTextureUnits>1;
}

extern void shaEnableInverted(void)
{
	ASSERT(_paprProjection!=NULL);
	gfxFrontFace(GFX_CW);
}

extern void shaDisableInverted(void)
{
	ASSERT(_paprProjection!=NULL);
	gfxFrontFace( GFX_CCW);
}

/*
 * Shader handling
 */

void CShader::DeleteHandles(void)
{
	// for each vertex program in this shader
	INDEX ctvp = sh_aVPHandles.Count();
	for(INDEX ivp=0;ivp<ctvp;ivp++) {
		VertexProgramHandle &vp = sh_aVPHandles[ivp];
		// for each valid vertex program handle 
		for(INDEX ivph=0;ivph<MAX_VPH;ivph++) {
			ULONG &ulHandle = vp.vph_ulHandles[ivph];
			if(ulHandle!=NONE) {
				// delete vertex program
				gfxDeleteVertexProgram(ulHandle);
				ulHandle = NONE;
			}
		}
	}

	// for each pixel program in this shader
	INDEX ctpp = sh_aPPHandles.Count();
	for(INDEX ipp=0;ipp<ctpp;ipp++) {
		PixelProgramHandle &pp = sh_aPPHandles[ipp];
		// if pixel program handle is valid
		ULONG &ulHandle = pp.pph_ulHandle;
		if(ulHandle!=NONE) {
			// delete pixel program
			gfxDeletePixelProgram(ulHandle);
			ulHandle = NONE;
		}
	}
}

// Clear shader 
void CShader::Clear(void)
{
	// Delete precached vertex and pixel program handles
	DeleteHandles();

	ShaderFunc = NULL;
	GetShaderDesc = NULL;
	GetShaderVPCode = NULL;
	GetShaderPPCode = NULL;
	// release dll
	if(hLibrary!=NULL) FreeLibrary(hLibrary);
}

// Count used memory
SLONG CShader::GetUsedMemory(void)
{
	return sizeof(CShader);
}

// Write to stream
void CShader::Write_t(CTStream *ostrFile)
{
}

// Read from stream
void CShader::Read_t(CTStream *istrFile)
{
	// read the dll filename and class name from the stream
	CTFileName fnmDLL;
	CTString strShaderFunc;
	CTString strShaderInfo;
	CTString strShaderVPCode;
	CTString strShaderPPCode;

	fnmDLL.ReadFromText_t(*istrFile, "Package: ");
	strShaderFunc.ReadFromText_t(*istrFile, "Name: ");
	strShaderInfo.ReadFromText_t(*istrFile, "Info: ");
	strShaderVPCode.ReadFromText_t(*istrFile, "VPCode: ");
	strShaderPPCode.ReadFromText_t(*istrFile, "PPCode: ");

	// create name of dll
#ifndef NDEBUG
		//fnmDLL = fnmDLL.FileDir()+"Debug\\"+fnmDLL.FileName()+/*_strModExt+*/"D"+fnmDLL.FileExt();
#	if		defined(_MSC_VER) && (_MSC_VER >= 1600)
#		ifdef	WORLD_EDITOR
			fnmDLL = fnmDLL.FileName() + "D" + fnmDLL.FileExt();
#		else	// WORLD_EDITOR
			fnmDLL = fnmDLL.FileDir()+"Debug2010\\"+fnmDLL.FileName()+/*_strModExt+*/"D"+fnmDLL.FileExt();
#		endif	// WORLD_EDITOR

#	else
		fnmDLL = fnmDLL.FileDir()+"Debug\\"+fnmDLL.FileName()+/*_strModExt+*/"D"+fnmDLL.FileExt();
#	endif

#else
	fnmDLL = fnmDLL.FileDir()+fnmDLL.FileName()+/*_strModExt+*/fnmDLL.FileExt();
#endif

	CTFileName fnmExpanded;

#ifndef	WORLD_EDITOR
	ExpandFilePath(EFP_READ | EFP_NOZIPS,fnmDLL,fnmExpanded);
#else	// WORLD_EDITOR
	fnmExpanded = _fnmApplicationPath + _fnmApplicationExe.FileDir() + fnmDLL.FileName() + fnmDLL.FileExt();
#endif // WORLD_EDITOR
	
	// set new error mode
	UINT iOldErrorMode = SetErrorMode(SEM_NOOPENFILEERRORBOX|SEM_FAILCRITICALERRORS);
	// load dll
	hLibrary = LoadLibrary((const char*)fnmExpanded);
	// return last error mode
	SetErrorMode(iOldErrorMode);
	// check if library has loaded
	if(hLibrary==NULL) {
		// report error
		istrFile->Throw_t("Error loading '%s' library",(const char*)fnmExpanded);
		return;
	}
	// get pointer to shader render function
	ShaderFunc = (void(*)(void))GetProcAddress(hLibrary,(const char*)strShaderFunc);
	// if error accured
	if(ShaderFunc==NULL) {
		// report error
		istrFile->Throw_t("GetProcAddress 'ShaderFunc' Error");
	}
	// get pointer to shader info function
//안태훈 수정 시작	//(For Performance)(0.1)
	GetShaderDesc = (void(*)(ShaderDesc*&))GetProcAddress(hLibrary,(const char*)strShaderInfo);
//안태훈 수정 끝	//(For Performance)(0.1)
	// if error accured
	if(GetShaderDesc==NULL) {
		// report error
		istrFile->Throw_t("GetProcAddress 'ShaderDesc' Error");
	}
	// get pointer to shader vertex program code function
	GetShaderVPCode = (void(*)(CTString&,INDEX))GetProcAddress(hLibrary,(const char*)strShaderVPCode);
	// if error accured
	if(GetShaderVPCode==NULL) {
		// report error
		istrFile->Throw_t("GetProcAddress 'ShaderVPCode' Error");
	}
	// get pointer to shader pixel program code function
	GetShaderPPCode = (void(*)(CTString&,INDEX,FOGTYPE))GetProcAddress(hLibrary,(const char*)strShaderPPCode);
	// if error accured
	if(GetShaderPPCode==NULL) {
		// report error
		istrFile->Throw_t("GetProcAddress 'ShaderPPCode' Error");
	}

	// Initialize vertex program and pixel program handles
	ResetHandles();
}

void CShader::ResetHandles(void)
{
	ShaderDesc *psd;
	ASSERT(GetShaderDesc!=NULL);
	GetShaderDesc(psd);
	ShaderDesc &sd = *psd;
	const INDEX ctvp = sd.sd_ctVertexPrograms;
	const INDEX ctpp = sd.sd_ctPixelPrograms;
	sh_ctVertexPrograms = ctvp;
	sh_ctPixelPrograms = ctpp;

	// Alocate vertex and pixel program handles
	if(sh_aVPHandles.Count()==0) {
		// x2 cos of fog vertex program
		sh_aVPHandles.New(ctvp*2);
	}
	if(sh_aPPHandles.Count()==0) {
		// x3 cos of opaque and non opaque fog pixel programs
		sh_aPPHandles.New(ctpp*3);
	}

	// Clear vertex program handles
	for(INDEX ivp=0;ivp<ctvp;ivp++) {
		VertexProgramHandle &vph = sh_aVPHandles[ivp];
		memset(&vph,0,sizeof(VertexProgramHandle));
		vph.vph_ulStreamFlags = sd.sd_ulStreamFlags[ivp];
		ASSERT(vph.vph_ulStreamFlags!=0xCDCDCDCD);
	}
	// Clear vertex program handles for fog
	for(INDEX ivpf=ctvp;ivpf<ctvp*2;ivpf++) {
		VertexProgramHandle &vph = sh_aVPHandles[ivpf];
		memset(&vph,0,sizeof(VertexProgramHandle));
		vph.vph_ulStreamFlags = sd.sd_ulStreamFlags[ivpf-ctvp];
		ASSERT(vph.vph_ulStreamFlags!=0xCDCDCDCD);
	}

	// Clear pixel program handles (with opaque and nonopaque fog shaders)
	for(INDEX ipp=0;ipp<ctpp*3;ipp++) {
		PixelProgramHandle &pph = sh_aPPHandles[ipp];
		memset(&pph,0,sizeof(PixelProgramHandle));
	}
}


/////////// DEAD CODE

#if 0

		// for shaCalculateLight()

		//------------------------ SSE code

#define SELECT(s3,s2,s1,s0) (s3*64 + s2*16 + s1*4 + s0) // for SSE suffler

	__m128 xmmLightX, xmmLightY, xmmLightZ;

		// prepare light
		movss		xmm0,D [fLightX]			    
	  shufps	xmm0,xmm0,SELECT(0,0,0,0)
	  movaps	[xmmLightX],xmm0			
		movss		xmm0,D [fLightY]			    
	  shufps	xmm0,xmm0,SELECT(0,0,0,0)
	  movaps	[xmmLightY],xmm0			
		movss		xmm0,D [fLightZ]			    
	  shufps	xmm0,xmm0,SELECT(0,0,0,0)
	  movaps	[xmmLightZ],xmm0			

		// construct 64-bit RGBA light
		mov     eax,D [slLG]
		mov     edx,0x01FE0000
		shl     eax,16
		shl     edx,16
		or      eax,D [slLR]
		or      edx,D [slLB]
		movd    mm5,eax
		movd    mm7,edx
		psllq   mm7,32
		por     mm5,mm7
		psllw   mm5,1 // boost for multiply
		// construct 64-bit RGBA ambient
		mov     eax,D [slAG]
		mov     edx,D [slSA]
		shl     eax,16
		shl     edx,16
		or      eax,D [slAR]
		or      edx,D [slAB]
		movd    mm6,eax
		movd    mm7,edx
		psllq   mm7,32
		por     mm6,mm7

		// init 
		mov     esi,D [pvNormals]
		mov     edi,D [pulColors]
		mov     ecx,D [ctVtxTotal]
		shr     ecx,2
		jz      fullRest

fullLoop4:
		movups  xmm3,[esi +0*16]          // 0 | nz1 | ny1 | nx1             nx4 | nx3 | nx2 | nx1
		movups  xmm6,[esi +1*16]          // 0 | nz2 | ny2 | nx2   | -=> |   ny4 | ny3 | ny2 | ny1
		movups  xmm4,[esi +2*16]          // 0 | nz3 | ny3 | nx3             nz4 | nz3 | nz2 | nz1
		movups  xmm0,[esi +3*16]          // 0 | nz4 | ny4 | nx4                                  
		// transpose for SIMD
	  movaps	xmm2,xmm3                  // clone first input
	 unpcklps xmm2,xmm6                  // xmm2 = Ny2Ny1Nx2Nx1
	  movaps	xmm1,xmm2                  // clone xmm2
	 unpckhps xmm3,xmm6                  // xmm3 = ~~Nz2Nz1
	  movaps	xmm6,xmm4                  // clone third input
	 unpckhps xmm4,xmm0                  // xmm4 = ~~Nz4Nz3
	 unpcklps xmm6,xmm0			             // xmm6 = Ny4Ny3Nx4Nx3
	  movlhps	xmm3,xmm4		             	 // get Nz4Nz3Nz2Nz1 in xmm3 (shufps xmm3,xmm4,SELECT(1,0,1,0))
	  movlhps	xmm1,xmm6			             // get Nx4Nx3Nx2Nx1 in xmm1 (shufps xmm1,xmm6,SELECT(1,0,1,0))
	  shufps	xmm2,xmm6,SELECT(3,2,3,2)  // get Ny4Ny3Ny2Ny1 in xmm2

		mulps		xmm1,[xmmLightX]			// xmm1 = Nx*Lx
	  mulps		xmm2,[xmmLightY]			// xmm2 = Ny*Ly
	  mulps		xmm3,[xmmLightZ]			// xmm3 = Nz*Lz
	  addps		xmm1,xmm2
	  addps		xmm1,xmm3			      // cos4 | cos3 | cos2 | cos1
		xorps		xmm0,xmm0			
	  cmpps		xmm0,xmm1,1		      // compare cos to 0
	  andps		xmm1,xmm0			      // clamp
		movhlps xmm3,xmm1           // ?  | ? | cos4 | cos3

	cvttps2pi mm1,xmm1
	cvttps2pi mm3,xmm3
	 packssdw mm1,mm3
		psllw   mm1,8

		movq    mm3,mm1
		punpcklwd mm1,mm1
		punpckhwd mm3,mm3
		movq    mm2,mm1
		movq    mm4,mm3
		punpckldq mm1,mm1
		punpckhdq mm2,mm2
		punpckldq mm3,mm3
		punpckhdq mm4,mm4
		pmulhw  mm1,mm5
		pmulhw  mm2,mm5
		pmulhw  mm3,mm5
		pmulhw  mm4,mm5
		paddsw  mm1,mm6
		paddsw  mm2,mm6
		paddsw  mm3,mm6
		paddsw  mm4,mm6
		packuswb mm1,mm2
		packuswb mm3,mm4

		movq    Q [edi+0],mm1
		movq    Q [edi+8],mm3
		add     esi,4*16
		add     edi,4*4
		dec     ecx
		jnz     fullLoop4
fullRest:

		// done
		emms
		jmp     allDone


#endif
