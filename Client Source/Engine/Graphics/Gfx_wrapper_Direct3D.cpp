
#include <d3dx8.h>

// helper for keep user clip plane of D3D happy (i.e. updated as view matrix changes)
static void UpdateClipPlane_D3D(void)
{
	FLOAT afObjectClipPlane[4];

	// if view matrix is active
	if( GFX_bViewMatrix) {
		// need to back-transform user clip plane from view to object space
		FLOAT *pcp = &D3D_afClipPlane[0]; // (pl-v) * !m;
		FLOAT *pvm = &D3D_afViewMatrix[0];
		afObjectClipPlane[0] = pcp[0]*pvm[0] + pcp[1]*pvm[1] + pcp[2]*pvm[2];
		afObjectClipPlane[1] = pcp[0]*pvm[4] + pcp[1]*pvm[5] + pcp[2]*pvm[6];
		afObjectClipPlane[2] = pcp[0]*pvm[8] + pcp[1]*pvm[9] + pcp[2]*pvm[10];
		afObjectClipPlane[3] = pcp[3] + (pcp[0]*pvm[12] + pcp[1]*pvm[13] + pcp[2]*pvm[14]);
	} else {
		// just copy clip plane
		(ULONG&)afObjectClipPlane[0] = (ULONG&)D3D_afClipPlane[0];
		(ULONG&)afObjectClipPlane[1] = (ULONG&)D3D_afClipPlane[1];
		(ULONG&)afObjectClipPlane[2] = (ULONG&)D3D_afClipPlane[2];
		(ULONG&)afObjectClipPlane[3] = (ULONG&)D3D_afClipPlane[3];
	}
	// skip if the same as last time
	ULONG *pulThis = (ULONG*) afObjectClipPlane;
	ULONG *pulLast = (ULONG*)_afActiveClipPlane;
	if( pulLast[0]==pulThis[0] && pulLast[1]==pulThis[1]
	 && pulLast[2]==pulThis[2] && pulLast[3]==pulThis[3]) return;
	// update (if supported!)
	if( _pGfx->gl_ulFlags&GLF_D3D_CLIPPLANE) {
		HRESULT hr = _pGfx->gl_pd3dDevice->SetClipPlane( 0, &afObjectClipPlane[0]);
		D3D_CHECKERROR(hr);
	} 
	// keep it as current
	pulLast[0] = pulThis[0];
	pulLast[1] = pulThis[1];
	pulLast[2] = pulThis[2];
	pulLast[3] = pulThis[3];
}



// ENABLE/DISABLE FUNCTIONS


static void d3d_EnableTexture(void)
{
	// check consistency
	ASSERT( _pGfx->gl_eCurrentAPI==GAT_D3D);
	HRESULT hr;
#ifndef NDEBUG
	BOOL bRes; 
	hr = _pGfx->gl_pd3dDevice->GetTextureStageState( GFX_iActiveTexUnit, D3DTSS_COLOROP, (DWORD*)&bRes);
	if( bRes==D3DTOP_DISABLE) bRes = FALSE;
	D3D_CHECKERROR(hr);
	ASSERT( !bRes == !GFX_abTexture[GFX_iActiveTexUnit]);
#endif

	// cached?
	if( GFX_abTexture[GFX_iActiveTexUnit] && gap_bOptimizeStateChanges) return;
	GFX_abTexture[GFX_iActiveTexUnit] = TRUE;

	_sfStats.StartTimer(CStatForm::STI_GFXAPI);

	D3DTEXTUREOP d3dTexOp = (GFX_iTexModulation[GFX_iActiveTexUnit]==2) ? D3DTOP_MODULATE2X : D3DTOP_MODULATE;
	hr = _pGfx->gl_pd3dDevice->SetTextureStageState( GFX_iActiveTexUnit, D3DTSS_COLOROP, d3dTexOp);
	D3D_CHECKERROR(hr);

	_sfStats.StopTimer(CStatForm::STI_GFXAPI);
}



static void d3d_DisableTexture(void)
{
	// check consistency
	ASSERT( _pGfx->gl_eCurrentAPI==GAT_D3D);
	HRESULT hr;
#ifndef NDEBUG
	BOOL bRes; 
	hr = _pGfx->gl_pd3dDevice->GetTextureStageState( GFX_iActiveTexUnit, D3DTSS_COLOROP, (DWORD*)&bRes);
	if( bRes==D3DTOP_DISABLE) bRes = FALSE;
	D3D_CHECKERROR(hr);
	ASSERT( !bRes == !GFX_abTexture[GFX_iActiveTexUnit]);
#endif

	// cached?
	if( !GFX_abTexture[GFX_iActiveTexUnit] && gap_bOptimizeStateChanges) return;
	GFX_abTexture[GFX_iActiveTexUnit] = FALSE;

	_sfStats.StartTimer(CStatForm::STI_GFXAPI);

	hr = _pGfx->gl_pd3dDevice->SetTextureStageState( GFX_iActiveTexUnit, D3DTSS_COLOROP, D3DTOP_DISABLE);
	D3D_CHECKERROR(hr);

	_sfStats.StopTimer(CStatForm::STI_GFXAPI);
}



static void d3d_EnableDepthTest(void)
{
	// check consistency
	ASSERT( _pGfx->gl_eCurrentAPI==GAT_D3D);
	HRESULT hr;
#ifndef NDEBUG
	BOOL bRes; 
	hr = _pGfx->gl_pd3dDevice->GetRenderState( D3DRS_ZENABLE, (DWORD*)&bRes);
	D3D_CHECKERROR(hr);
	ASSERT( !bRes == !GFX_bDepthTest);
#endif
	// cached?
	if( GFX_bDepthTest && gap_bOptimizeStateChanges) return;
	GFX_bDepthTest = TRUE;

	_sfStats.StartTimer(CStatForm::STI_GFXAPI);

	hr = _pGfx->gl_pd3dDevice->SetRenderState( D3DRS_ZENABLE, D3DZB_TRUE);
	D3D_CHECKERROR(hr);

	_sfStats.StopTimer(CStatForm::STI_GFXAPI);
}



static void d3d_DisableDepthTest(void)
{
	// check consistency
	ASSERT( _pGfx->gl_eCurrentAPI==GAT_D3D);
	HRESULT hr;
#ifndef NDEBUG
	BOOL bRes; 
	hr = _pGfx->gl_pd3dDevice->GetRenderState( D3DRS_ZENABLE, (DWORD*)&bRes);
	D3D_CHECKERROR(hr);
	ASSERT( !bRes == !GFX_bDepthTest);
#endif

	// cached?
	if( !GFX_bDepthTest && gap_bOptimizeStateChanges) return;
	GFX_bDepthTest = FALSE;

	_sfStats.StartTimer(CStatForm::STI_GFXAPI);

	hr = _pGfx->gl_pd3dDevice->SetRenderState( D3DRS_ZENABLE, D3DZB_FALSE);
	D3D_CHECKERROR(hr);

	_sfStats.StopTimer(CStatForm::STI_GFXAPI);
}



static void d3d_EnableDepthBias(void)
{
	// only if supported
	ASSERT( _pGfx->gl_eCurrentAPI==GAT_D3D);
	if( !(_pGfx->gl_ulFlags&GLF_D3D_ZBIAS)) return;

	_sfStats.StartTimer(CStatForm::STI_GFXAPI);

	HRESULT hr = _pGfx->gl_pd3dDevice->SetRenderState( D3DRS_ZBIAS, 2);
	D3D_CHECKERROR(hr);

	_sfStats.StopTimer(CStatForm::STI_GFXAPI);
}



static void d3d_DisableDepthBias(void)
{
	// only if supported
	ASSERT( _pGfx->gl_eCurrentAPI==GAT_D3D);
	if( !(_pGfx->gl_ulFlags&GLF_D3D_ZBIAS)) return;

	_sfStats.StartTimer(CStatForm::STI_GFXAPI);

	HRESULT hr = _pGfx->gl_pd3dDevice->SetRenderState( D3DRS_ZBIAS, 0);
	D3D_CHECKERROR(hr);

	_sfStats.StopTimer(CStatForm::STI_GFXAPI);
}



static void d3d_EnableDepthWrite(void)
{
	// check consistency
	ASSERT( _pGfx->gl_eCurrentAPI==GAT_D3D);
	HRESULT hr;
#ifndef NDEBUG
	BOOL bRes; 
	hr = _pGfx->gl_pd3dDevice->GetRenderState( D3DRS_ZWRITEENABLE, (DWORD*)&bRes);
	D3D_CHECKERROR(hr);
	ASSERT( !bRes == !GFX_bDepthWrite);
#endif

	// cached?
	if( GFX_bDepthWrite && gap_bOptimizeStateChanges) return;
	GFX_bDepthWrite = TRUE;

	_sfStats.StartTimer(CStatForm::STI_GFXAPI);

	hr = _pGfx->gl_pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE);
	D3D_CHECKERROR(hr);

	_sfStats.StopTimer(CStatForm::STI_GFXAPI);
}



static void d3d_DisableDepthWrite(void)
{
	// check consistency
	ASSERT( _pGfx->gl_eCurrentAPI==GAT_D3D);
	HRESULT hr;
#ifndef NDEBUG
	BOOL bRes; 
	hr = _pGfx->gl_pd3dDevice->GetRenderState( D3DRS_ZWRITEENABLE, (DWORD*)&bRes);
	D3D_CHECKERROR(hr);
	ASSERT( !bRes == !GFX_bDepthWrite);
#endif

	// cached?
	if( !GFX_bDepthWrite && gap_bOptimizeStateChanges) return;
	GFX_bDepthWrite = FALSE;

	_sfStats.StartTimer(CStatForm::STI_GFXAPI);

	hr = _pGfx->gl_pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE, FALSE);
	D3D_CHECKERROR(hr);

	_sfStats.StopTimer(CStatForm::STI_GFXAPI);
}



static void d3d_EnableDither(void)
{
	// check consistency
	ASSERT( _pGfx->gl_eCurrentAPI==GAT_D3D);
	HRESULT hr;
#ifndef NDEBUG
	BOOL bRes; 
	hr = _pGfx->gl_pd3dDevice->GetRenderState( D3DRS_DITHERENABLE, (DWORD*)&bRes);
	D3D_CHECKERROR(hr);
	ASSERT( !bRes == !GFX_bDithering);
#endif

	// cached?
	if( GFX_bDithering && gap_bOptimizeStateChanges) return;
	GFX_bDithering = TRUE;

	_sfStats.StartTimer(CStatForm::STI_GFXAPI);

	hr = _pGfx->gl_pd3dDevice->SetRenderState( D3DRS_DITHERENABLE, TRUE);
	D3D_CHECKERROR(hr);
	
	_sfStats.StopTimer(CStatForm::STI_GFXAPI);
}



static void d3d_DisableDither(void)
{
	// check consistency
	ASSERT( _pGfx->gl_eCurrentAPI==GAT_D3D);
	HRESULT hr;
#ifndef NDEBUG
	BOOL bRes; 
	hr = _pGfx->gl_pd3dDevice->GetRenderState( D3DRS_DITHERENABLE, (DWORD*)&bRes);
	D3D_CHECKERROR(hr);
	ASSERT( !bRes == !GFX_bDithering);
#endif

	// cached?
	if( !GFX_bDithering && gap_bOptimizeStateChanges) return;
	GFX_bDithering = FALSE;

	_sfStats.StartTimer(CStatForm::STI_GFXAPI);

	hr = _pGfx->gl_pd3dDevice->SetRenderState( D3DRS_DITHERENABLE, FALSE);
	D3D_CHECKERROR(hr);

	_sfStats.StopTimer(CStatForm::STI_GFXAPI);
}



static void d3d_EnableAlphaTest(void)
{
	// check consistency
	ASSERT( _pGfx->gl_eCurrentAPI==GAT_D3D);
	HRESULT hr;
#ifndef NDEBUG
	BOOL bRes; 
	hr = _pGfx->gl_pd3dDevice->GetRenderState( D3DRS_ALPHATESTENABLE, (DWORD*)&bRes);
	D3D_CHECKERROR(hr);
	ASSERT( !bRes == !GFX_bAlphaTest);
#endif

	// cached?
	if( GFX_bAlphaTest && gap_bOptimizeStateChanges) return;
	GFX_bAlphaTest = TRUE;

	_sfStats.StartTimer(CStatForm::STI_GFXAPI);

	hr = _pGfx->gl_pd3dDevice->SetRenderState( D3DRS_ALPHATESTENABLE, TRUE);
	D3D_CHECKERROR(hr);

	_sfStats.StopTimer(CStatForm::STI_GFXAPI);
}



static void d3d_DisableAlphaTest(void)
{
	// check consistency
	ASSERT( _pGfx->gl_eCurrentAPI==GAT_D3D);
	HRESULT hr;
#ifndef NDEBUG
	BOOL bRes; 
	hr = _pGfx->gl_pd3dDevice->GetRenderState( D3DRS_ALPHATESTENABLE, (DWORD*)&bRes);
	D3D_CHECKERROR(hr);
	ASSERT( !bRes == !GFX_bAlphaTest);
#endif

	// cached?
	if( !GFX_bAlphaTest && gap_bOptimizeStateChanges) return;
	GFX_bAlphaTest = FALSE;

	_sfStats.StartTimer(CStatForm::STI_GFXAPI);

	hr = _pGfx->gl_pd3dDevice->SetRenderState( D3DRS_ALPHATESTENABLE, FALSE);
	D3D_CHECKERROR(hr);
 
	_sfStats.StopTimer(CStatForm::STI_GFXAPI);
}



static void d3d_EnableBlend(void)
{
	// check consistency
	ASSERT( _pGfx->gl_eCurrentAPI==GAT_D3D);
	HRESULT hr;
#ifndef NDEBUG
	BOOL bRes; 
	hr = _pGfx->gl_pd3dDevice->GetRenderState( D3DRS_ALPHABLENDENABLE, (DWORD*)&bRes);
	D3D_CHECKERROR(hr);
	ASSERT( !bRes == !GFX_bBlending);
#endif
	// cached?
	if( GFX_bBlending && gap_bOptimizeStateChanges) return;
	GFX_bBlending = TRUE;

	_sfStats.StartTimer(CStatForm::STI_GFXAPI);

	hr = _pGfx->gl_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE);
	D3D_CHECKERROR(hr);

	_sfStats.StopTimer(CStatForm::STI_GFXAPI);

	// adjust dithering
	if( gap_iDithering==2) d3d_EnableDither();
	else d3d_DisableDither();
}



static void d3d_DisableBlend(void)
{
	// check consistency
	ASSERT( _pGfx->gl_eCurrentAPI==GAT_D3D);
	HRESULT hr;
#ifndef NDEBUG
	BOOL bRes; 
	hr = _pGfx->gl_pd3dDevice->GetRenderState( D3DRS_ALPHABLENDENABLE, (DWORD*)&bRes);
	D3D_CHECKERROR(hr);
	ASSERT( !bRes == !GFX_bBlending);
#endif

	// cached?
	if( !GFX_bBlending && gap_bOptimizeStateChanges) return;
	GFX_bBlending = FALSE;

	_sfStats.StartTimer(CStatForm::STI_GFXAPI);

	hr = _pGfx->gl_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE);
	D3D_CHECKERROR(hr);

	_sfStats.StopTimer(CStatForm::STI_GFXAPI);

	// adjust dithering
	if( gap_iDithering==0) d3d_DisableDither();
	else d3d_EnableDither();
}


static void d3d_SetVertexProgram(const ULONG ulHandle)
{
	ASSERT( _pGfx->gl_eCurrentAPI==GAT_D3D);
	ASSERT(GFX_bUseVertexProgram);

	extern void SetupShaderStreams(void);
	extern DWORD _dwCurrentVS;
	extern DWORD _dwLastVertexProgram;
	
	SetupShaderStreams();
	if(_pGfx->gl_dwVertexShader!=ulHandle) {
		HRESULT hr = _pGfx->gl_pd3dDevice->SetVertexShader(ulHandle);
		D3D_CHECKERROR(hr);
		_pGfx->gl_dwVertexShader = ulHandle;
		_dwCurrentVS = ulHandle;
	}
	_dwLastVertexProgram = ulHandle;
}


static void d3d_SetPixelProgram(const ULONG ulHandle)
{
	ASSERT( _pGfx->gl_eCurrentAPI==GAT_D3D);

	extern DWORD _dwCurrentPS;
	if(_pGfx->gl_dwPixelShader!=ulHandle) {
		HRESULT hr = _pGfx->gl_pd3dDevice->SetPixelShader(ulHandle);
		D3D_CHECKERROR(hr);
		_pGfx->gl_dwPixelShader = ulHandle;
		_dwCurrentPS = ulHandle;
	}
}


static void d3d_DeleteVertexProgram(ULONG ulHandle)
{
	ASSERT(_pGfx->gl_eCurrentAPI==GAT_D3D);
	ASSERT(ulHandle!=NONE);
	// first set default vertex shader
	d3dSetVertexShader(D3DFVF_CTVERTEX);
	// Delete given vertex shader
	HRESULT hr = _pGfx->gl_pd3dDevice->DeleteVertexShader(ulHandle);
	D3D_CHECKERROR(hr);
}


static void d3d_DeletePixelProgram(ULONG ulHandle)
{
	ASSERT(_pGfx->gl_eCurrentAPI==GAT_D3D);
	ASSERT(ulHandle!=NONE);
	HRESULT hr = _pGfx->gl_pd3dDevice->SetPixelShader(NONE);
	D3D_CHECKERROR(hr);
	hr = _pGfx->gl_pd3dDevice->DeletePixelShader(ulHandle);
	D3D_CHECKERROR(hr);
}


//안태훈 수정 시작	//(DevPartner Bug Fix)(2005-01-13)
static ULONG d3d_CreateVertexProgram(const char *strVertexProgram, ULONG ulStreamFlags)
{
	extern void GetShaderDeclaration_D3D(ULONG *ulRetDecl, ULONG ulStreamFlags);
	extern ULONG *CompileVertexProgram_D3D(const char *strVertexProgram, ID3DXBuffer **ppOut);

	ASSERT( _pGfx->gl_eCurrentAPI==GAT_D3D);
	ASSERT(strVertexProgram!=NULL);
	ASSERT(strlen(strVertexProgram)>0);

	ULONG ulProgramHandle;
	ULONG ulDeclaration[MAX_SHADER_DECL_SIZE];
	GetShaderDeclaration_D3D(ulDeclaration,ulStreamFlags);
	LPD3DXBUFFER pBuffer = NULL;
	ULONG *pulShaderTokens = CompileVertexProgram_D3D(strVertexProgram, &pBuffer);
	ASSERT(pBuffer->GetBufferPointer() == pulShaderTokens);

	// create new vertex shader
	const DWORD dwFlags = (_pGfx->gl_ulFlags&GLF_D3D_USINGHWTNL) ? NONE : D3DUSAGE_SOFTWAREPROCESSING;

	// create programmable vertex shader
	HRESULT hr = _pGfx->gl_pd3dDevice->CreateVertexShader( ulDeclaration, pulShaderTokens, &ulProgramHandle, dwFlags);
	pBuffer->Release();
	D3D_CHECKERROR(hr);
	return ulProgramHandle;
}


static ULONG d3d_CreatePixelProgram(const char *strPixelProgram)
{
	extern ULONG *CompilePixelProgram_D3D(const char *strVertexProgram, ID3DXBuffer **ppOut);

	ASSERT( _pGfx->gl_eCurrentAPI==GAT_D3D);
	ASSERT(strPixelProgram!=NULL);
	ASSERT(strlen(strPixelProgram)>0);

	ULONG ulProgramHandle;
	LPD3DXBUFFER pBuffer = NULL;
	ULONG *pulShaderTokens = CompilePixelProgram_D3D(strPixelProgram, &pBuffer);
	ASSERT(pBuffer->GetBufferPointer() == pulShaderTokens);

#define D3DPIXELSHADERDEF ULONG
	// create programmable pixel shader
	HRESULT hr = _pGfx->gl_pd3dDevice->CreatePixelShader( (D3DPIXELSHADERDEF*)pulShaderTokens, &ulProgramHandle);
	pBuffer->Release();
	D3D_CHECKERROR(hr);
	return ulProgramHandle;
}
//안태훈 수정 끝	//(DevPartner Bug Fix)(2005-01-13)


// Set value of constant register for vertex shader
static void d3d_SetVertexProgramConst(INDEX iRegister, const void *pData, INDEX ctRegisters)
{
	ASSERT( _pGfx->gl_eCurrentAPI==GAT_D3D);
	ASSERT(iRegister>=0 && iRegister<96);
	ASSERT(pData!=NULL);
	ASSERT(ctRegisters>0 && iRegister<=96);
	ASSERT(iRegister+ctRegisters<=96);
	ASSERT(GFX_bUseVertexProgram==TRUE);
	ASSERT(_pGfx->gl_dwVertexShader!=NONE);
	HRESULT hr = _pGfx->gl_pd3dDevice->SetVertexShaderConstant(iRegister, pData, ctRegisters);
	D3D_CHECKERROR(hr); 

#if _DEBUG
	FLOAT faData[96*4];
	hr = _pGfx->gl_pd3dDevice->GetVertexShaderConstant(iRegister, &faData, ctRegisters);
	D3D_CHECKERROR(hr);
	ASSERT(memcmp(&faData,pData,sizeof(FLOAT)*4*ctRegisters)==0);
#endif
}


// Set value of constant register for pixel shader
static void d3d_SetPixelProgramConst(INDEX iRegister, const void *pData, INDEX ctRegisters)
{
	ASSERT( _pGfx->gl_eCurrentAPI==GAT_D3D);
	ASSERT(iRegister>=0 && iRegister<8);
	ASSERT(pData!=NULL);
	ASSERT(ctRegisters>0 && iRegister<=8);
	ASSERT(GFX_bUsePixelProgram==TRUE);
//안태훈 수정 시작	//(Add Tagent-space Normal Map)(0.1)
	//원래 이것이 있어서 이 함수 호출전에 SetPixelProgram호출이 선행되야 했다.
	//그러나 그럴 필요가 없어서 뺐다. -_- 왠 이상한 규칙인가.
//	ASSERT(_pGfx->gl_dwPixelShader!=NONE);
//안태훈 수정 끝	//(Add Tagent-space Normal Map)(0.1)
	HRESULT hr = _pGfx->gl_pd3dDevice->SetPixelShaderConstant(iRegister, pData, ctRegisters);
	D3D_CHECKERROR(hr); 
}


static void d3d_EnableLighting(void)
{
	// check consistency
	ASSERT( _pGfx->gl_eCurrentAPI==GAT_D3D);
	HRESULT hr;
#ifndef NDEBUG
	BOOL bRes; 
	hr = _pGfx->gl_pd3dDevice->GetRenderState( D3DRS_LIGHTING, (DWORD*)&bRes);
	D3D_CHECKERROR(hr);
	ASSERT( !bRes == !GFX_bLighting);
#endif
	// cached?
	if( GFX_bLighting && gap_bOptimizeStateChanges) return;
	GFX_bLighting = TRUE;

	_sfStats.StartTimer(CStatForm::STI_GFXAPI);

	hr = _pGfx->gl_pd3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE);
	D3D_CHECKERROR(hr);

	_sfStats.StopTimer(CStatForm::STI_GFXAPI);
}


static void d3d_DisableLighting(void)
{
	// check consistency
	ASSERT( _pGfx->gl_eCurrentAPI==GAT_D3D);
	HRESULT hr;
#ifndef NDEBUG
	BOOL bRes; 
	hr = _pGfx->gl_pd3dDevice->GetRenderState( D3DRS_LIGHTING, (DWORD*)&bRes);
	D3D_CHECKERROR(hr);
	ASSERT( !bRes == !GFX_bLighting);
#endif
	// cached?
	if( !GFX_bLighting && gap_bOptimizeStateChanges) return;
	GFX_bLighting = FALSE;

	_sfStats.StartTimer(CStatForm::STI_GFXAPI);

	hr = _pGfx->gl_pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE);
	D3D_CHECKERROR(hr);

	_sfStats.StopTimer(CStatForm::STI_GFXAPI);
}



static void d3d_EnableClipping(void)
{
	// check consistency
	ASSERT( _pGfx->gl_eCurrentAPI==GAT_D3D);
	HRESULT hr;
#ifndef NDEBUG
	BOOL bRes = GFX_bClipping;
	hr = _pGfx->gl_pd3dDevice->GetRenderState( D3DRS_CLIPPING, (DWORD*)&bRes);
	D3D_CHECKERROR(hr);
	ASSERT( !bRes == !GFX_bClipping);
#endif

	// cached?
	if( GFX_bClipping && gap_bOptimizeStateChanges) return;
	GFX_bClipping = TRUE;

	_sfStats.StartTimer(CStatForm::STI_GFXAPI);

	_bWantsClipping = TRUE; // need to signal for custom clip plane
	hr = _pGfx->gl_pd3dDevice->SetRenderState( D3DRS_CLIPPING, TRUE);
	D3D_CHECKERROR(hr);

	_sfStats.StopTimer(CStatForm::STI_GFXAPI);
}



static void d3d_DisableClipping(void)
{
	// only if allowed
	if( gap_iOptimizeClipping<2) return;

	// check consistency
	ASSERT( _pGfx->gl_eCurrentAPI==GAT_D3D);
	HRESULT hr;
#ifndef NDEBUG
	BOOL bRes;
	hr = _pGfx->gl_pd3dDevice->GetRenderState( D3DRS_CLIPPING, (DWORD*)&bRes);
	D3D_CHECKERROR(hr);
	ASSERT( !bRes == !GFX_bClipping);
#endif

	// cached?
	if( !GFX_bClipping && gap_bOptimizeStateChanges) return;
	GFX_bClipping = FALSE;

	_sfStats.StartTimer(CStatForm::STI_GFXAPI);

	// cannot disable clipping if clip-plane is enabled!
	if( GFX_bClipPlane) {
		GFX_bClipping  = TRUE;
	 _bWantsClipping = FALSE;
	} else {
		hr = _pGfx->gl_pd3dDevice->SetRenderState( D3DRS_CLIPPING, FALSE);
		D3D_CHECKERROR(hr);
	}
	_sfStats.StopTimer(CStatForm::STI_GFXAPI);
}



static void d3d_EnableClipPlane(void)
{
	// only if supported
	if( !(_pGfx->gl_ulFlags&GLF_D3D_CLIPPLANE)) return;

	// check consistency
	ASSERT( _pGfx->gl_eCurrentAPI==GAT_D3D);
	HRESULT hr;
#ifndef NDEBUG
	BOOL bRes; 
	hr = _pGfx->gl_pd3dDevice->GetRenderState( D3DRS_CLIPPLANEENABLE, (DWORD*)&bRes);
	D3D_CHECKERROR(hr);
	ASSERT( !bRes == !GFX_bClipPlane);
#endif

	// cached?
	if( GFX_bClipPlane && gap_bOptimizeStateChanges) return;
	GFX_bClipPlane = TRUE;

	_sfStats.StartTimer(CStatForm::STI_GFXAPI);

	hr = _pGfx->gl_pd3dDevice->SetRenderState( D3DRS_CLIPPLANEENABLE, D3DCLIPPLANE0);
	D3D_CHECKERROR(hr);
	// (eventually) update clip plane, too
	UpdateClipPlane_D3D();
	// D3D needs to have clipping enabled for that matter
	if( !GFX_bClipping) {
		hr = _pGfx->gl_pd3dDevice->SetRenderState( D3DRS_CLIPPING, TRUE);
		D3D_CHECKERROR(hr);
		GFX_bClipping  = TRUE;
	 _bWantsClipping = FALSE;
	}
	_sfStats.StopTimer(CStatForm::STI_GFXAPI);
}



static void d3d_DisableClipPlane(void)
{
	// only if supported
	if( !(_pGfx->gl_ulFlags&GLF_D3D_CLIPPLANE)) return;

	// check consistency
	ASSERT( _pGfx->gl_eCurrentAPI==GAT_D3D);
	HRESULT hr;
#ifndef NDEBUG
	BOOL bRes = GFX_bClipPlane; 
	hr = _pGfx->gl_pd3dDevice->GetRenderState( D3DRS_CLIPPLANEENABLE, (DWORD*)&bRes);
	D3D_CHECKERROR(hr);
	ASSERT( !bRes == !GFX_bClipPlane);
#endif
	// cached?
	if( !GFX_bClipPlane && gap_bOptimizeStateChanges) return;
	GFX_bClipPlane = FALSE;

	_sfStats.StartTimer(CStatForm::STI_GFXAPI);

	hr = _pGfx->gl_pd3dDevice->SetRenderState( D3DRS_CLIPPLANEENABLE, FALSE);
	D3D_CHECKERROR(hr);
	// maybe we can disable clipping in general (if was kept enabled just beacuse of clip plane)
	if( !_bWantsClipping && GFX_bClipping) {
		GFX_bClipping = FALSE;
		hr = _pGfx->gl_pd3dDevice->SetRenderState( D3DRS_CLIPPING, FALSE);
		D3D_CHECKERROR(hr);
	}
	_sfStats.StopTimer(CStatForm::STI_GFXAPI);
}



extern void d3d_EnableColorArray(void)
{
	// check consistency
	ASSERT( _pGfx->gl_eCurrentAPI==GAT_D3D);
	HRESULT hr;
#ifndef NDEBUG
	BOOL bRes; 
	hr = _pGfx->gl_pd3dDevice->GetRenderState( D3DRS_COLORVERTEX, (DWORD*)&bRes);
	D3D_CHECKERROR(hr);
	ASSERT( !bRes == !GFX_bColorArray);
#endif
	// cached?
	if( GFX_bColorArray && gap_bOptimizeStateChanges) return;
	GFX_bColorArray = TRUE;

	const BOOL bTicking = _sfStats.CheckTimer(CStatForm::STI_GFXAPI);
	if( !bTicking) _sfStats.StartTimer(CStatForm::STI_GFXAPI);

	hr = _pGfx->gl_pd3dDevice->SetRenderState( D3DRS_COLORVERTEX, TRUE);
	D3D_CHECKERROR(hr);

	if( !bTicking) _sfStats.StopTimer(CStatForm::STI_GFXAPI);
}


static void d3d_DisableColorArray(void)
{
	// check consistency
	ASSERT( _pGfx->gl_eCurrentAPI==GAT_D3D);
	HRESULT hr;
#ifndef NDEBUG
	BOOL bRes; 
	hr = _pGfx->gl_pd3dDevice->GetRenderState( D3DRS_COLORVERTEX, (DWORD*)&bRes);
	D3D_CHECKERROR(hr);
	ASSERT( !bRes == !GFX_bColorArray);
#endif
	// cached?
	if( !GFX_bColorArray && gap_bOptimizeStateChanges) return;
	GFX_bColorArray = FALSE;

	const BOOL bTicking = _sfStats.CheckTimer(CStatForm::STI_GFXAPI);
	if( !bTicking) _sfStats.StartTimer(CStatForm::STI_GFXAPI);

	hr = _pGfx->gl_pd3dDevice->SetRenderState( D3DRS_COLORVERTEX, FALSE);
	D3D_CHECKERROR(hr);

	if( !bTicking) _sfStats.StopTimer(CStatForm::STI_GFXAPI);
}



static void d3d_EnableTruform(void)
{
	// skip if Truform isn't set
	if( truform_iLevel<1) return;

	// check consistency
	ASSERT( _pGfx->gl_eCurrentAPI==GAT_D3D);
	HRESULT hr;
	FLOAT fSegments;
#ifndef NDEBUG
	BOOL bRes; 
	hr = _pGfx->gl_pd3dDevice->GetRenderState( D3DRS_PATCHSEGMENTS, (DWORD*)&fSegments);
	D3D_CHECKERROR(hr);
	bRes = (fSegments>1) ? TRUE : FALSE;
	ASSERT( !bRes == !GFX_bTruform);
#endif

	if( GFX_bTruform && gap_bOptimizeStateChanges) return;
	GFX_bTruform = TRUE;

	_sfStats.StartTimer(CStatForm::STI_GFXAPI);

	fSegments = truform_iLevel+1;
	hr = _pGfx->gl_pd3dDevice->SetRenderState( D3DRS_PATCHSEGMENTS, *((DWORD*)&fSegments));
	D3D_CHECKERROR(hr);

	_sfStats.StopTimer(CStatForm::STI_GFXAPI);
}



static void d3d_DisableTruform(void)
{
	// skip if Truform isn't set
	if( truform_iLevel<1) return;

	// check consistency
	ASSERT( _pGfx->gl_eCurrentAPI==GAT_D3D);
	HRESULT hr;
	FLOAT fSegments;
#ifndef NDEBUG
	BOOL bRes; 
	hr = _pGfx->gl_pd3dDevice->GetRenderState( D3DRS_PATCHSEGMENTS, (DWORD*)&fSegments);
	D3D_CHECKERROR(hr);
	bRes = (fSegments>1) ? TRUE : FALSE;
	ASSERT( !bRes == !GFX_bTruform);
#endif

	if( !GFX_bTruform && gap_bOptimizeStateChanges) return;
	GFX_bTruform = FALSE;

	_sfStats.StartTimer(CStatForm::STI_GFXAPI);

	fSegments = 1.0f;
	hr = _pGfx->gl_pd3dDevice->SetRenderState( D3DRS_PATCHSEGMENTS, *((DWORD*)&fSegments));
	D3D_CHECKERROR(hr);

	_sfStats.StopTimer(CStatForm::STI_GFXAPI);
}




__forceinline _D3DBLEND BlendToD3D( GfxBlend eFunc) {
	switch( eFunc) {
	case GFX_ZERO:          return D3DBLEND_ZERO;
	case GFX_ONE:           return D3DBLEND_ONE;
	case GFX_SRC_COLOR:     return D3DBLEND_SRCCOLOR;
	case GFX_INV_SRC_COLOR: return D3DBLEND_INVSRCCOLOR;
	case GFX_DST_COLOR:     return D3DBLEND_DESTCOLOR;
	case GFX_INV_DST_COLOR: return D3DBLEND_INVDESTCOLOR;
	case GFX_SRC_ALPHA:     return D3DBLEND_SRCALPHA;
	case GFX_INV_SRC_ALPHA: return D3DBLEND_INVSRCALPHA;
	case GFX_DST_ALPHA:		return D3DBLEND_DESTALPHA;
	case GFX_INV_DST_ALPHA: return D3DBLEND_INVDESTALPHA;
	default: ASSERTALWAYS("Invalid GFX blending function!");
	} return D3DBLEND_ONE;
}

__forceinline GfxBlend BlendFromD3D( _D3DBLEND d3dbFunc) {
	switch( d3dbFunc) {
	case D3DBLEND_ZERO:         return GFX_ZERO;
	case D3DBLEND_ONE:          return GFX_ONE;          
	case D3DBLEND_SRCCOLOR:     return GFX_SRC_COLOR;    
	case D3DBLEND_INVSRCCOLOR:  return GFX_INV_SRC_COLOR;
	case D3DBLEND_DESTCOLOR:    return GFX_DST_COLOR;    
	case D3DBLEND_INVDESTCOLOR: return GFX_INV_DST_COLOR;
	case D3DBLEND_SRCALPHA:     return GFX_SRC_ALPHA;    
	case D3DBLEND_INVSRCALPHA:  return GFX_INV_SRC_ALPHA;
	case D3DBLEND_DESTALPHA:		return GFX_DST_ALPHA;
	case D3DBLEND_INVDESTALPHA:  return GFX_INV_DST_ALPHA;
	default: ASSERTALWAYS("Unsupported D3D blending function!");
	} return GFX_ONE;
}



// set blending operations
static void d3d_BlendFunc( GfxBlend eSrc, GfxBlend eDst)
{
	// check consistency
	ASSERT( _pGfx->gl_eCurrentAPI==GAT_D3D);
	HRESULT hr;
#ifndef NDEBUG
	GfxBlend  gfxSrc, gfxDst; 
	_D3DBLEND d3dSrc, d3dDst;
	hr = _pGfx->gl_pd3dDevice->GetRenderState( D3DRS_SRCBLEND,  (DWORD*)&d3dSrc);  D3D_CHECKERROR(hr);
	hr = _pGfx->gl_pd3dDevice->GetRenderState( D3DRS_DESTBLEND, (DWORD*)&d3dDst);  D3D_CHECKERROR(hr);
	gfxSrc = BlendFromD3D(d3dSrc);
	gfxDst = BlendFromD3D(d3dDst);
	ASSERT( gfxSrc==GFX_eBlendSrc && gfxDst==GFX_eBlendDst);
#endif
	// cached?
	if( eSrc==GFX_eBlendSrc && eDst==GFX_eBlendDst && gap_bOptimizeStateChanges) return;

	_sfStats.StartTimer(CStatForm::STI_GFXAPI);

	if( eSrc!=GFX_eBlendSrc) {
	 _D3DBLEND d3dSrc = BlendToD3D(eSrc);
		hr = _pGfx->gl_pd3dDevice->SetRenderState( D3DRS_SRCBLEND, d3dSrc);
		D3D_CHECKERROR(hr);
		GFX_eBlendSrc = eSrc;
	}
	if( eDst!=GFX_eBlendDst) {
	 _D3DBLEND d3dDst = BlendToD3D(eDst);
		hr = _pGfx->gl_pd3dDevice->SetRenderState( D3DRS_DESTBLEND, d3dDst);
		D3D_CHECKERROR(hr);
		GFX_eBlendDst = eDst;
	}
	_sfStats.StopTimer(CStatForm::STI_GFXAPI);
}



static void d3d_SetColorMask( ULONG ulColorMask)
{
	// skip if the same as last time
	if( GFX_ulCurrentColorMask==ulColorMask) return;
	GFX_ulCurrentColorMask = ulColorMask; // keep for Get...()

	// set only if supported
	ASSERT( _pGfx->gl_eCurrentAPI==GAT_D3D);
	if( !(_pGfx->gl_ulFlags&GLF_D3D_COLORWRITES))
	{ // emulate disabling of all channels
		if( ulColorMask==0) {
			d3d_EnableBlend();
			d3d_BlendFunc( GFX_ZERO, GFX_ONE);
		} // done
		return;
	}

	_sfStats.StartTimer(CStatForm::STI_GFXAPI);

	// no emulation
	ULONG ulBitMask = NONE;
	if( (ulColorMask&CT_RMASK) == CT_RMASK) ulBitMask |= D3DCOLORWRITEENABLE_RED;
	if( (ulColorMask&CT_GMASK) == CT_GMASK) ulBitMask |= D3DCOLORWRITEENABLE_GREEN;
	if( (ulColorMask&CT_BMASK) == CT_BMASK) ulBitMask |= D3DCOLORWRITEENABLE_BLUE;
	if( (ulColorMask&CT_AMASK) == CT_AMASK) ulBitMask |= D3DCOLORWRITEENABLE_ALPHA;
	HRESULT hr = _pGfx->gl_pd3dDevice->SetRenderState( D3DRS_COLORWRITEENABLE, ulBitMask);
	D3D_CHECKERROR(hr);

	_sfStats.StopTimer(CStatForm::STI_GFXAPI);
}



__forceinline _D3DCMPFUNC CompToD3D( GfxComp eFunc) {
	switch( eFunc) {
	case GFX_NEVER:         return D3DCMP_NEVER;
	case GFX_LESS:          return D3DCMP_LESS;
	case GFX_LESS_EQUAL:    return D3DCMP_LESSEQUAL;
	case GFX_EQUAL:         return D3DCMP_EQUAL;
	case GFX_NOT_EQUAL:     return D3DCMP_NOTEQUAL;
	case GFX_GREATER_EQUAL: return D3DCMP_GREATEREQUAL;
	case GFX_GREATER:       return D3DCMP_GREATER;
	case GFX_ALWAYS:        return D3DCMP_ALWAYS;
	default: ASSERTALWAYS("Invalid GFX compare function!");
	} return D3DCMP_ALWAYS;
}

__forceinline GfxComp CompFromD3D( _D3DCMPFUNC d3dcFunc) {
	switch( d3dcFunc) {
	case D3DCMP_NEVER:        return GFX_NEVER;
	case D3DCMP_LESS:         return GFX_LESS;
	case D3DCMP_LESSEQUAL:    return GFX_LESS_EQUAL;
	case D3DCMP_EQUAL:        return GFX_EQUAL;
	case D3DCMP_NOTEQUAL:     return GFX_NOT_EQUAL;
	case D3DCMP_GREATEREQUAL: return GFX_GREATER_EQUAL;
	case D3DCMP_GREATER:      return GFX_GREATER;
	case D3DCMP_ALWAYS:       return GFX_ALWAYS;
	default: ASSERTALWAYS("Invalid D3D compare function!");
	} return GFX_ALWAYS;
}




// set depth buffer compare mode
static void d3d_DepthFunc( GfxComp eFunc)
{
	// check consistency
	ASSERT( _pGfx->gl_eCurrentAPI==GAT_D3D);
	HRESULT hr;
 _D3DCMPFUNC d3dcFunc;
#ifndef NDEBUG
	GfxComp gfxFunc; 
	hr = _pGfx->gl_pd3dDevice->GetRenderState( D3DRS_ZFUNC, (DWORD*)&d3dcFunc);
	D3D_CHECKERROR(hr);
	gfxFunc = CompFromD3D( d3dcFunc);
	ASSERT( gfxFunc==GFX_eDepthFunc);
#endif
	// cached?
	if( eFunc==GFX_eDepthFunc && gap_bOptimizeStateChanges) return;

	_sfStats.StartTimer(CStatForm::STI_GFXAPI);

	d3dcFunc = CompToD3D(eFunc);
	hr = _pGfx->gl_pd3dDevice->SetRenderState( D3DRS_ZFUNC, d3dcFunc);
	D3D_CHECKERROR(hr);
	GFX_eDepthFunc = eFunc;

	_sfStats.StopTimer(CStatForm::STI_GFXAPI);
}


		

// set depth buffer range
static void d3d_DepthRange( FLOAT fMin, FLOAT fMax)
{
	// D3D doesn't allow 0 for max value (no comment!)
	if( fMax<0.001f) fMax = 0.001f; 

	// check consistency
	ASSERT( _pGfx->gl_eCurrentAPI==GAT_D3D);
	HRESULT hr;
	D3DVIEWPORT8 d3dViewport;
#ifndef NDEBUG
//강동민 수정 시작 접속 시퀀스 작업	05.31
//	hr = _pGfx->gl_pd3dDevice->GetViewport( &d3dViewport);										// 원본.
//	ASSERT( d3dViewport.MinZ==GFX_fMinDepthRange && d3dViewport.MaxZ==GFX_fMaxDepthRange);		// 원본.
//강동민 수정 끝 접속 시퀀스 작업	05.31
#endif

	// cached?
	if( GFX_fMinDepthRange==fMin && GFX_fMaxDepthRange==fMax && gap_bOptimizeStateChanges) return;
	GFX_fMinDepthRange = fMin;
	GFX_fMaxDepthRange = fMax;

	_sfStats.StartTimer(CStatForm::STI_GFXAPI);

	// get viewport
	hr = _pGfx->gl_pd3dDevice->GetViewport( &d3dViewport);
	D3D_CHECKERROR(hr);
	// update depth range and set the viewport back
	d3dViewport.MinZ = fMin;
	d3dViewport.MaxZ = fMax;
	hr = _pGfx->gl_pd3dDevice->SetViewport( &d3dViewport);
	D3D_CHECKERROR(hr);

	_sfStats.StopTimer(CStatForm::STI_GFXAPI);
}




static void d3d_CullFace( GfxFace eFace)
{
	// check consistency and face
	ASSERT( eFace==GFX_FRONT || eFace==GFX_BACK || eFace==GFX_NONE);
	ASSERT( _pGfx->gl_eCurrentAPI==GAT_D3D);
	HRESULT hr;

	// must (re)assign faces for front-face emulation purposes
	//GfxFace eFrontFace, eBackFace;
	if( !GFX_bFrontFace && eFace!=GFX_NONE) {
		eFace = (eFace==GFX_FRONT) ? GFX_BACK : GFX_FRONT;
	}
	// cached?
	if( GFX_eCullFace==eFace && gap_bOptimizeStateChanges) return;
/*if( gap_bOptimizeStateChanges) {
		if( GFX_eCullFace==GFX_NONE  && eFace==GFX_NONE)   return;
		if( GFX_eCullFace==GFX_FRONT && eFace==eFrontFace) return;
		if( GFX_eCullFace==GFX_BACK  && eFace==eBackFace)  return;
	} */

	_sfStats.StartTimer(CStatForm::STI_GFXAPI);

			 if( eFace==GFX_FRONT) hr = _pGfx->gl_pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW);   
	else if( eFace==GFX_BACK)  hr = _pGfx->gl_pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CW);
	else                       hr = _pGfx->gl_pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE); 
	D3D_CHECKERROR(hr);
	GFX_eCullFace = eFace;

	_sfStats.StopTimer(CStatForm::STI_GFXAPI);
}



static void d3d_FrontFace( GfxFace eFace)
{
	// check consistency and face
	ASSERT( eFace==GFX_CW || eFace==GFX_CCW);
	ASSERT( _pGfx->gl_eCurrentAPI==GAT_D3D);
	// cached?
	BOOL bFrontFace = (eFace==GFX_CCW);
	if( !GFX_bFrontFace==!bFrontFace && gap_bOptimizeStateChanges) return;

	// must emulate this by toggling cull face
	GFX_bFrontFace = bFrontFace; 
	if( GFX_eCullFace!=GFX_NONE) d3d_CullFace(GFX_eCullFace);
}




static void d3d_ClipPlane( const DOUBLE *pdViewPlane)
{
	// check API and plane
	ASSERT( _pGfx->gl_eCurrentAPI==GAT_D3D && pdViewPlane!=NULL);

	_sfStats.StartTimer(CStatForm::STI_GFXAPI);

	// convert to floats, keep and update
	D3D_afClipPlane[0] = pdViewPlane[0];
	D3D_afClipPlane[1] = pdViewPlane[1];
	D3D_afClipPlane[2] = pdViewPlane[2];
	D3D_afClipPlane[3] = pdViewPlane[3];
	UpdateClipPlane_D3D();

	_sfStats.StopTimer(CStatForm::STI_GFXAPI);
}



static void d3d_SetTextureMatrix( const FLOAT *pfMatrix/*=NULL*/)
{
	// check API
	ASSERT( _pGfx->gl_eCurrentAPI==GAT_D3D);

	_sfStats.StartTimer(CStatForm::STI_GFXAPI);

	HRESULT hr;

	// Check if texture unit has allready enabled texture transform
	const ULONG ulMatrixMask = 1UL<<GFX_iActiveTexUnit;
	const BOOL bTexMatrixEnabled = _ulUsedTexMatrices&ulMatrixMask;

	// if matrix was specified
	if( pfMatrix!=NULL) {

		// if texture transform isn't enabled
		if(!bTexMatrixEnabled) {
			// Enable texture transform
			hr = _pGfx->gl_pd3dDevice->SetTextureStageState(GFX_iActiveTexUnit, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
			D3D_CHECKERROR(hr);
		}

		// Since texture is 2d @!#$ d3d uses translation in 3th row (3x3 matrix)!!!
		FLOAT afMatrix16[16];
		memcpy(&afMatrix16,pfMatrix, sizeof(FLOAT)*16);
		afMatrix16[ 8] = afMatrix16[12];
		afMatrix16[ 9] = afMatrix16[13];
		afMatrix16[ 3] = 0.0f;
		afMatrix16[ 7] = 0.0f;
		afMatrix16[10] = 0.0f;
		afMatrix16[11] = 0.0f;
		afMatrix16[12] = 0.0f;
		afMatrix16[13] = 0.0f;
		afMatrix16[14] = 0.0f;
		afMatrix16[15] = 0.0f;

		// Set current texture matrix
		D3DTRANSFORMSTATETYPE tsMatrixNo = (D3DTRANSFORMSTATETYPE)(D3DTS_TEXTURE0 + GFX_iActiveTexUnit);
		hr = _pGfx->gl_pd3dDevice->SetTransform( tsMatrixNo, (_D3DMATRIX*)&afMatrix16);
		D3D_CHECKERROR(hr);

		// Mark texture matrix active for current texture unit
		_ulUsedTexMatrices|=ulMatrixMask;

	} else {
		// if enabled
		if(bTexMatrixEnabled) {
			// disable texture transform
			hr = _pGfx->gl_pd3dDevice->SetTextureStageState(GFX_iActiveTexUnit, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
			D3D_CHECKERROR(hr);
			// Mark texture matrix inactive fur current texture unit
			_ulUsedTexMatrices&=~ulMatrixMask;
		}
	}
	_sfStats.StopTimer(CStatForm::STI_GFXAPI);
}




static void d3d_SetViewMatrix( const FLOAT *pfMatrix/*=NULL*/)
{
	// check API
	ASSERT( _pGfx->gl_eCurrentAPI==GAT_D3D);
	HRESULT hr;

	// cached? (only identity matrix)
	if( pfMatrix==NULL && GFX_bViewMatrix==NONE && gap_bOptimizeStateChanges) return;
	GFX_bViewMatrix = (pfMatrix!=NULL);

	_sfStats.StartTimer(CStatForm::STI_GFXAPI);

	if( pfMatrix!=NULL) {
		// need to keep it for clip plane
		CopyLongs( (ULONG*)pfMatrix, (ULONG*)D3D_afViewMatrix, 16);
		hr = _pGfx->gl_pd3dDevice->SetTransform( D3DTS_VIEW, (_D3DMATRIX*)D3D_afViewMatrix);
	} else {
		// load identity matrix
		extern const D3DMATRIX GFX_d3dIdentityMatrix;
		hr = _pGfx->gl_pd3dDevice->SetTransform( D3DTS_VIEW, &GFX_d3dIdentityMatrix);
	} // check
	D3D_CHECKERROR(hr);
	// update clip plane if enabled
	if( GFX_bClipPlane) UpdateClipPlane_D3D();

	_sfStats.StopTimer(CStatForm::STI_GFXAPI);
}



static void d3d_SetOrtho( FLOAT fLeft, FLOAT fRight, FLOAT fTop, FLOAT fBottom, FLOAT fNear, FLOAT fFar,
													const BOOL bSubPixelAdjust/*=FALSE*/)
{
	// check API and matrix type
	ASSERT( _pGfx->gl_eCurrentAPI==GAT_D3D);

	// cached?
	if( GFX_fLastL==fLeft  && GFX_fLastT==fTop    && GFX_fLastN==fNear
	 && GFX_fLastR==fRight && GFX_fLastB==fBottom && GFX_fLastF==fFar && gap_bOptimizeStateChanges) return;
	GFX_fLastL = fLeft;   GFX_fLastT = fTop;     GFX_fLastN = fNear;
	GFX_fLastR = fRight;  GFX_fLastB = fBottom;  GFX_fLastF = fFar;

	_sfStats.StartTimer(CStatForm::STI_GFXAPI);

	// adjust coords for sub-pixel precision
	// generate ortho matrix (+1 is because of sub-pixel adjustment!)
	FLOAT fAdj = bSubPixelAdjust ? 1 : 0;
	const FLOAT fRpL=fRight+fLeft+fAdj;  const FLOAT fRmL=fRight-fLeft;  //const FLOAT fFpN=fFar+fNear;
	const FLOAT fTpB=fTop+fBottom+fAdj;  const FLOAT fTmB=fTop-fBottom;  const FLOAT fFmN=fFar-fNear;
	const FLOAT afMatrix[16] = {	2/fRmL,				 0,				  0,		0,
										 0,			2/fTmB,				  0,		0,
										 0,				 0,			-1/fFmN,		0,
								-fRpL/fRmL,		-fTpB/fTmB,		-fNear/fFmN,		1 };
	HRESULT hr = _pGfx->gl_pd3dDevice->SetTransform( D3DTS_PROJECTION, (_D3DMATRIX*)afMatrix);
	D3D_CHECKERROR(hr);

	_sfStats.StopTimer(CStatForm::STI_GFXAPI);
}



static void d3d_SetFrustum( FLOAT fLeft, FLOAT fRight, FLOAT fTop, FLOAT fBottom, FLOAT fNear, FLOAT fFar)
{
	// check API
	ASSERT( _pGfx->gl_eCurrentAPI==GAT_D3D);

	// cached?
	if( GFX_fLastL==-fLeft  && GFX_fLastT==-fTop    && GFX_fLastN==-fNear
	 && GFX_fLastR==-fRight && GFX_fLastB==-fBottom && GFX_fLastF==-fFar && gap_bOptimizeStateChanges) return;
	GFX_fLastL = -fLeft;   GFX_fLastT = -fTop;     GFX_fLastN = -fNear;
	GFX_fLastR = -fRight;  GFX_fLastB = -fBottom;  GFX_fLastF = -fFar;

	_sfStats.StartTimer(CStatForm::STI_GFXAPI);

	// generate frustum matrix
	const FLOAT fRpL=fRight+fLeft;  const FLOAT fTpB=fTop+fBottom;  //const FLOAT fFpN=fFar+fNear;
	const FLOAT fRmL=fRight-fLeft;  const FLOAT fTmB=fTop-fBottom;  const FLOAT fFmN=fFar-fNear;
	const FLOAT afMatrix[4][4] = {
		{ 2*fNear/fRmL,            0,                0,  0 },
		{            0, 2*fNear/fTmB,                0,  0 },
		{    fRpL/fRmL,    fTpB/fTmB,       -fFar/fFmN, -1 },
		{            0,            0, -fFar*fNear/fFmN,  0 }
	}; // set it
	HRESULT hr = _pGfx->gl_pd3dDevice->SetTransform( D3DTS_PROJECTION, (const _D3DMATRIX*)&afMatrix);
	D3D_CHECKERROR(hr);

	_sfStats.StopTimer(CStatForm::STI_GFXAPI);
}




static void d3d_PolygonMode( GfxPolyMode ePolyMode)
{
	ASSERT( _pGfx->gl_eCurrentAPI==GAT_D3D);
	_sfStats.StartTimer(CStatForm::STI_GFXAPI);

	HRESULT hr;
	switch(ePolyMode) {
	case GFX_POINT:  hr = _pGfx->gl_pd3dDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_POINT);      break;
	case GFX_LINE:   hr = _pGfx->gl_pd3dDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME);  break;
	case GFX_FILL:   hr = _pGfx->gl_pd3dDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID);      break;
	default:  ASSERTALWAYS("Wrong polygon mode!");
	} // check
	D3D_CHECKERROR(hr);

	_sfStats.StopTimer(CStatForm::STI_GFXAPI);
}



// LIGHTS MANAGEMENT


// enable one light
static void d3d_EnableLight( INDEX iLight)
{
	// check consistency
	ASSERT( _pGfx->gl_eCurrentAPI==GAT_D3D);
	ASSERT( iLight>=0 && iLight<GFX_MAXLIGHTS);
	HRESULT hr;
#ifndef NDEBUG
	BOOL bRes; 
	hr = _pGfx->gl_pd3dDevice->GetLightEnable( iLight, &bRes);
	D3D_CHECKERROR(hr);
	ASSERT( !bRes == !GFX_abLights[iLight]);
#endif

	// cached?
	if( GFX_abLights[iLight] && gap_bOptimizeStateChanges) return;
	GFX_abLights[iLight] = TRUE;

	_sfStats.StartTimer(CStatForm::STI_GFXAPI);

	hr = _pGfx->gl_pd3dDevice->LightEnable( iLight, TRUE);
	D3D_CHECKERROR(hr);

	_sfStats.StopTimer(CStatForm::STI_GFXAPI);
}


// disable one light
static void d3d_DisableLight( INDEX iLight)
{
	// check consistency
	ASSERT( _pGfx->gl_eCurrentAPI==GAT_D3D);
	ASSERT( iLight>=0 && iLight<GFX_MAXLIGHTS);
	HRESULT hr;
#ifndef NDEBUG
	BOOL bRes; 
	hr = _pGfx->gl_pd3dDevice->GetLightEnable( iLight, &bRes);
	D3D_CHECKERROR(hr);
	ASSERT( !bRes == !GFX_abLights[iLight]);
#endif

	// cached?
	if( !GFX_abLights[iLight] && gap_bOptimizeStateChanges) return;
	GFX_abLights[iLight] = FALSE;

	_sfStats.StartTimer(CStatForm::STI_GFXAPI);

	hr = _pGfx->gl_pd3dDevice->LightEnable( iLight, FALSE);
	D3D_CHECKERROR(hr);

	_sfStats.StopTimer(CStatForm::STI_GFXAPI);
}



// set color components of a light
static void d3d_SetLightColor( INDEX iLight, COLOR colLight, COLOR colAmbient, COLOR colSpecular/*=0*/)
{
	_sfStats.StartTimer(CStatForm::STI_GFXAPI);
	HRESULT hr;

	// get current light properties
	D3DLIGHT8 d3dLight;
	hr = _pGfx->gl_pd3dDevice->GetLight( iLight, &d3dLight);
	D3D_CHECKERROR(hr);

	// modify colors and set light
	d3dLight.Diffuse.r  = (colLight&CT_RMASK)>>CT_RSHIFT;
	d3dLight.Diffuse.g  = (colLight&CT_GMASK)>>CT_GSHIFT;
	d3dLight.Diffuse.b  = (colLight&CT_BMASK)>>CT_BSHIFT;
	d3dLight.Diffuse.a  = (colLight&CT_AMASK)>>CT_ASHIFT;
	d3dLight.Ambient.r  = (colAmbient&CT_RMASK)>>CT_RSHIFT;
	d3dLight.Ambient.g  = (colAmbient&CT_GMASK)>>CT_GSHIFT;
	d3dLight.Ambient.b  = (colAmbient&CT_BMASK)>>CT_BSHIFT;
	d3dLight.Ambient.a  = (colAmbient&CT_AMASK)>>CT_ASHIFT;
	d3dLight.Specular.r = (colSpecular&CT_RMASK)>>CT_RSHIFT;
	d3dLight.Specular.g = (colSpecular&CT_GMASK)>>CT_GSHIFT;
	d3dLight.Specular.b = (colSpecular&CT_BMASK)>>CT_BSHIFT;
	d3dLight.Specular.a = (colSpecular&CT_AMASK)>>CT_ASHIFT;
	d3dLight.Type = D3DLIGHT_DIRECTIONAL; // force directional light
	hr = _pGfx->gl_pd3dDevice->SetLight( iLight, &d3dLight);
	D3D_CHECKERROR(hr);

	_sfStats.StopTimer(CStatForm::STI_GFXAPI);
}


// set light direction (only directional light are supported for now - they're the fastest!)
static void d3d_SetLightDirection( INDEX iLight, const FLOAT3D &vLightDir)
{
	_sfStats.StartTimer(CStatForm::STI_GFXAPI);
	HRESULT hr;

	// get current light properties
	D3DLIGHT8 d3dLight;
	hr = _pGfx->gl_pd3dDevice->GetLight( iLight, &d3dLight);
	D3D_CHECKERROR(hr);

	// modify direction and set light
	d3dLight.Type = D3DLIGHT_DIRECTIONAL; // force directional light
	d3dLight.Direction = (D3DVECTOR&)vLightDir;
	hr = _pGfx->gl_pd3dDevice->SetLight( iLight, &d3dLight);
	D3D_CHECKERROR(hr);

	_sfStats.StopTimer(CStatForm::STI_GFXAPI);
}



// TEXTURE MANAGEMENT


static void d3d_SetTextureWrapping( enum GfxWrap eWrapU, enum GfxWrap eWrapV)
{
	ASSERT( _pGfx->gl_eCurrentAPI==GAT_D3D);
	HRESULT hr;
#ifndef NDEBUG
	// check wrapping consistency
	D3DTEXTUREADDRESS d3dtaU, d3dtaV;
	hr = _pGfx->gl_pd3dDevice->GetTextureStageState( GFX_iActiveTexUnit, D3DTSS_ADDRESSU, (ULONG*)&d3dtaU);  D3D_CHECKERROR(hr);
	hr = _pGfx->gl_pd3dDevice->GetTextureStageState( GFX_iActiveTexUnit, D3DTSS_ADDRESSV, (ULONG*)&d3dtaV);  D3D_CHECKERROR(hr);
	ASSERT( (d3dtaU==D3DTADDRESS_WRAP  && _tpGlobal[GFX_iActiveTexUnit].tp_eWrapU==GFX_REPEAT)
			 || (d3dtaU==D3DTADDRESS_CLAMP && _tpGlobal[GFX_iActiveTexUnit].tp_eWrapU==GFX_CLAMP)
			 || (_tpGlobal[GFX_iActiveTexUnit].tp_eWrapU==0));
	ASSERT( (d3dtaV==D3DTADDRESS_WRAP  && _tpGlobal[GFX_iActiveTexUnit].tp_eWrapV==GFX_REPEAT)
			 || (d3dtaV==D3DTADDRESS_CLAMP && _tpGlobal[GFX_iActiveTexUnit].tp_eWrapV==GFX_CLAMP)
			 || (_tpGlobal[GFX_iActiveTexUnit].tp_eWrapV==0));
#endif

	_sfStats.StartTimer(CStatForm::STI_GFXAPI);

	// adjust only those that differ
	if( _tpGlobal[GFX_iActiveTexUnit].tp_eWrapU!=eWrapU) {
		D3DTEXTUREADDRESS d3dta = (eWrapU==GFX_REPEAT) ? D3DTADDRESS_WRAP : D3DTADDRESS_CLAMP;
		hr = _pGfx->gl_pd3dDevice->SetTextureStageState( GFX_iActiveTexUnit, D3DTSS_ADDRESSU, d3dta);
		D3D_CHECKERROR(hr);
	 _tpGlobal[GFX_iActiveTexUnit].tp_eWrapU = eWrapU;
	}
	if( _tpGlobal[GFX_iActiveTexUnit].tp_eWrapV!=eWrapV) {
		D3DTEXTUREADDRESS d3dta = (eWrapV==GFX_REPEAT) ? D3DTADDRESS_WRAP : D3DTADDRESS_CLAMP;
		hr = _pGfx->gl_pd3dDevice->SetTextureStageState( GFX_iActiveTexUnit, D3DTSS_ADDRESSV, d3dta);
		D3D_CHECKERROR(hr);
	 _tpGlobal[GFX_iActiveTexUnit].tp_eWrapV = eWrapV;
	}

	_sfStats.StopTimer(CStatForm::STI_GFXAPI);
}



static void d3d_SetTextureModulation( INDEX iScale)
{
	// check consistency
	ASSERT( _pGfx->gl_eCurrentAPI==GAT_D3D);
	HRESULT hr;
#ifndef NDEBUG                 
/*
	INDEX iRet;
	hr = _pGfx->gl_pd3dDevice->GetTextureStageState( GFX_iActiveTexUnit, D3DTSS_COLOROP, (DWORD*)&iRet);
	D3D_CHECKERROR(hr);
		 if( iRet==D3DTOP_MODULATE2X) ASSERT( GFX_iTexModulation[GFX_iActiveTexUnit]==2);
	else if( iRet==D3DTOP_MODULATE)   ASSERT( GFX_iTexModulation[GFX_iActiveTexUnit]==1);
	else                              ASSERT( iRet==D3DTOP_DISABLE);
*/
#endif
	
	// cached?
	ASSERT( iScale==1 || iScale==2);
	if( GFX_iTexModulation[GFX_iActiveTexUnit]==iScale) return;
	GFX_iTexModulation[GFX_iActiveTexUnit] = iScale;

	_sfStats.StartTimer(CStatForm::STI_GFXAPI);

	// set only if texturing is enabled - will be auto-set at gfxEnableTexture
	if( GFX_abTexture[GFX_iActiveTexUnit]) {
		D3DTEXTUREOP d3dTexOp = (iScale==2) ? D3DTOP_MODULATE2X : D3DTOP_MODULATE;
		hr = _pGfx->gl_pd3dDevice->SetTextureStageState( GFX_iActiveTexUnit, D3DTSS_COLOROP, d3dTexOp);
		D3D_CHECKERROR(hr);
	}
	_sfStats.StopTimer(CStatForm::STI_GFXAPI);
}



static void d3d_GenerateTexture( ULONG &ulTexObject)
{
	ASSERT( _pGfx->gl_eCurrentAPI==GAT_D3D);
	_sfStats.StartTimer(CStatForm::STI_BINDTEXTURE);
	_sfStats.StartTimer(CStatForm::STI_GFXAPI);

	// generate one dummy texture that'll be entirely replaced upon 1st upload
	HRESULT hr = _pGfx->gl_pd3dDevice->CreateTexture( 1, 1, 0, 0, (D3DFORMAT)TS.ts_tfRGBA8, D3DPOOL_MANAGED, (LPDIRECT3DTEXTURE8*)&ulTexObject);
	D3D_CHECKERROR(hr);
	MEMTRACK_ALLOC( (void*)(ulTexObject^0x80000000), 4096, 4);
			
	_sfStats.StopTimer(CStatForm::STI_BINDTEXTURE);
	_sfStats.StopTimer(CStatForm::STI_GFXAPI);
}


// unbind texture from API
static void d3d_DeleteTexture( ULONG &ulTexObject)
{
	// skip if already unbound
	ASSERT( _pGfx->gl_eCurrentAPI==GAT_D3D);
	if( ulTexObject==NONE) return;

	_sfStats.StartTimer(CStatForm::STI_BINDTEXTURE);
	_sfStats.StartTimer(CStatForm::STI_GFXAPI);

	MEMTRACK_FREE( (void*)(ulTexObject^0x80000000));
	D3DRELEASE( (LPDIRECT3DTEXTURE8&)ulTexObject, TRUE);
	ulTexObject = NONE;

	_sfStats.StopTimer(CStatForm::STI_BINDTEXTURE);
	_sfStats.StopTimer(CStatForm::STI_GFXAPI);
}



// VERTEX BUFFERS & ARRAYS


extern void *LockVertexArray_D3D( const INDEX ctVertices);
extern void *LockNormalArray_D3D(void);
//안태훈 수정 시작	//(Add Tagent-space Normal Map)(0.1)
extern void *LockTangentArray_D3D(void);
//안태훈 수정 끝	//(Add Tagent-space Normal Map)(0.1)
extern void *LockWeightArray_D3D(void);
extern void *LockColorArray_D3D(void);
extern void *LockTexCoordArray_D3D( const BOOL bProjectiveMapping=FALSE);
extern void UnlockVertexArray_D3D(void);
extern void UnlockNormalArray_D3D(void);
//안태훈 수정 시작	//(Add Tagent-space Normal Map)(0.1)
extern void UnlockTangentArray_D3D(void);
//안태훈 수정 끝	//(Add Tagent-space Normal Map)(0.1)
extern void UnlockWeightArray_D3D(void);
extern void UnlockColorArray_D3D(void);
extern void UnlockTexCoordArray_D3D(void);



// lock one vertex buffer for reading or writing (if iID<16, lock dynamic vertex buffer)
static void *d3d_LockSubBuffer( const INDEX iID, const INDEX i1stVertex, const INDEX ctVertices,
																const enum GfxLockType eLockType/*=GFX_WRITE*/)
{
	// determine bind, type and mask
	const INDEX iBind = iID >> 4;
	const INDEX iType = iID & 15;
	const ULONG ulTypeMask = 1L << iType;
	// must be created and nothing already locked
	const INDEX ctBuffers = _avbVertexBuffers.Count();
	ASSERT( ctBuffers>0 && iBind<ctBuffers && iType<GFX_MAX_VBA);
	VertexBuffer &vb = _avbVertexBuffers[iBind];
	ASSERT( (vb.vb_ulLockMask&ulTypeMask)==0 && (vb.vb_ulArrayMask&ulTypeMask));
	void *pRet;

	_sfStats.StartTimer(CStatForm::STI_GFXAPI);

	// dynamic buffer?
	if( iBind==0) {
		ASSERT( eLockType==GFX_WRITE);  // dynamic vertex buffer can be locked only for writing
		switch( iType) {
		case GFX_VBA_POS:  pRet=LockVertexArray_D3D(ctVertices);  break;
		case GFX_VBA_NOR:  pRet=LockNormalArray_D3D();            break;
//안태훈 수정 시작	//(Add Tagent-space Normal Map)(0.1)
		case GFX_VBA_TAN:  pRet=LockTangentArray_D3D();            break;
//안태훈 수정 끝	//(Add Tagent-space Normal Map)(0.1)
		case GFX_VBA_WGH:  pRet=LockWeightArray_D3D();            break;
		case GFX_VBA_TEX:  pRet=LockTexCoordArray_D3D();          break;
		case GFX_VBA_COL: 
			d3d_EnableColorArray(); // obviously we're gonna need it
			pRet=LockColorArray_D3D();
			break;
		default: ASSERT(FALSE);
		}
	}

	// static buffer?
	else {
		// get size of requested vertex type
		const SLONG slTypeSize = GetVBTypeSize(iType);
		// for write access on PC, we'll actually lock the read buffer
		// (at unlock it'll get copied to write buffer if writing was required)
		if( eLockType==GFX_WRITE) {
			if( vb.vb_eType==GFX_READWRITE) {
				// we'll need to copy vetices at unlock
				vb.vb_i1stLockedVertex[iType] = i1stVertex;
				vb.vb_ctLockedVertices[iType] = ctVertices;
				pRet = (UBYTE*)vb.vb_paReadArray[iType] + slTypeSize*i1stVertex;
			} else {
				// for write-only access, we'll lock the write buffer right away (and mark it for unlock)
				vb.vb_i1stLockedVertex[iType] = -1;
				vb.vb_ctLockedVertices[iType] = -1;
				LPDIRECT3DVERTEXBUFFER8 pd3dVB = vb.vb_pavbWrite[iType];
				const DWORD dwLockFlag = (vb.vb_eType==GFX_DYNAMIC) ? D3DLOCK_DISCARD : NONE;
				HRESULT hr = pd3dVB->Lock( i1stVertex*slTypeSize, ctVertices*slTypeSize, (UBYTE**)&pRet, dwLockFlag);
				D3D_CHECKERROR(hr);
			}
		}
		// read access
		else {
			// we won't need later copying
			ASSERT( vb.vb_eType==GFX_READWRITE);
			vb.vb_i1stLockedVertex[iType] = 0;
			vb.vb_ctLockedVertices[iType] = 0;
			pRet = (UBYTE*)vb.vb_paReadArray[iType] + slTypeSize*i1stVertex;
		}
	}

	_sfStats.StopTimer(CStatForm::STI_GFXAPI);

	// mark that buffer has been locked
	vb.vb_ulLockMask |= ulTypeMask;
	return pRet;
}



// unlocks vertex buffer
static void d3d_UnlockSubBuffer( const INDEX iID, const INDEX ctVertices/*=0*/)
{
	// determine bind, type and mask
	const INDEX iBind = iID >> 4;
	const INDEX iType = iID & 15;
	const ULONG ulTypeMask = 1L << iType;
	// must be created and locked
	const INDEX ctBuffers = _avbVertexBuffers.Count();
	ASSERT( ctBuffers>0 && iBind<ctBuffers && iType<GFX_MAX_VBA);
	VertexBuffer &vb = _avbVertexBuffers[iBind];
	ASSERT( (vb.vb_ulLockMask&ulTypeMask)!=0 && (vb.vb_ulArrayMask&ulTypeMask));

	_sfStats.StartTimer(CStatForm::STI_GFXAPI);

	// dynamic buffer?
	if( iBind==0) {
		if( ctVertices>0) GFX_ctVertices = ctVertices;  // eventually adjust number of locked vertices
		switch( iType) {
		case GFX_VBA_POS:  UnlockVertexArray_D3D();   break;
		case GFX_VBA_NOR:  UnlockNormalArray_D3D();   break;
//안태훈 수정 시작	//(Add Tagent-space Normal Map)(0.1)
		case GFX_VBA_TAN:  UnlockTangentArray_D3D();  break;
//안태훈 수정 끝	//(Add Tagent-space Normal Map)(0.1)
		case GFX_VBA_WGH:  UnlockWeightArray_D3D();   break;
		case GFX_VBA_TEX:  UnlockTexCoordArray_D3D(); break;
		case GFX_VBA_COL:  UnlockColorArray_D3D();    break;
		default: ASSERT(FALSE);
		}
	}

	// static buffer?
	else {
		ASSERT( ctVertices==0); // cannot adjust size of static buffer
		// if read/write array was locked for writing, we must copy read to write buffer (i.e. synchronize)
		LPDIRECT3DVERTEXBUFFER8 pd3dVB = vb.vb_pavbWrite[iType];
		if( vb.vb_ctLockedVertices[iType]>0) {
			const SLONG slTypeSize = GetVBTypeSize(iType);
			const SLONG slOffset = vb.vb_i1stLockedVertex[iType] * slTypeSize;
			const SLONG slSize   = vb.vb_ctLockedVertices[iType] * slTypeSize;
			UBYTE *pubSrc = (UBYTE*)vb.vb_paReadArray[iType] + slOffset;
			UBYTE *pubDst;
			HRESULT hr = pd3dVB->Lock( slOffset, slSize, &pubDst, NONE);
			D3D_CHECKERROR(hr);
			CopyLongs( (ULONG*)pubSrc, (ULONG*)pubDst, slSize/sizeof(FLOAT));
		}
		// if write-only array was locked, just unlock it
		if( vb.vb_ctLockedVertices[iType]!=0) {
			vb.vb_i1stLockedVertex[iType] = 0;
			vb.vb_ctLockedVertices[iType] = 0;
			HRESULT hr = pd3dVB->Unlock();
			D3D_CHECKERROR(hr);
		}
	}

	_sfStats.StopTimer(CStatForm::STI_GFXAPI);

	// mark that buffer has been unlocked
	vb.vb_ulLockMask &= ~ulTypeMask;
}


// Array of stream indices for each vertex buffer type
//안태훈 수정 시작	//(Add Tagent-space Normal Map)(0.1)
static INDEX _aiStreamFromType[] = {
  GFX_POSIDX,
  GFX_NORIDX,
  GFX_WGHIDX,
  GFX_COLIDX,
  GFX_TEXIDX+0,
  GFX_TEXIDX+1,
  GFX_TEXIDX+2,
  GFX_TEXIDX+3,
  -1,
  -1,
  -1,
  -1,
  GFX_REAL_TANIDX,
};
//안태훈 수정 끝	//(Add Tagent-space Normal Map)(0.1)


// set vertex sub-buffer (i.e. array) to be the current one for rendering
static void d3d_SetVertexSubBuffer( const INDEX iID, const INDEX i1stVertex/*=0*/,
																		const INDEX ctVertices/*=0*/)
{
	// determine bind, type, stream, and mask
	const INDEX iBind = iID >> 4;
	const INDEX iType = iID & 15;
	const INDEX iStream = _aiStreamFromType[iType];

	ASSERT( iType==GFX_VBA_POS && iBind<_avbVertexBuffers.Count());
	VertexBuffer &vb = _avbVertexBuffers[iBind];
	HRESULT hr;

	_sfStats.StartTimer(CStatForm::STI_GFXAPI);

	if(iID!=0) {
		// assign stream 
		LPDIRECT3DVERTEXBUFFER8 pd3dVB = vb.vb_pavbWrite[iType];
		hr = _pGfx->gl_pd3dDevice->SetStreamSource( iStream, pd3dVB, GetVBTypeSize(iType));
	}

	// check and mark
	D3D_CHECKERROR(hr);
	_bUsingDynamicBuffer = FALSE;

	// set vertex count and offset
	GFX_ctVertices = (ctVertices>0) ? ctVertices : vb.vb_ctVertices;
	hr = _pGfx->gl_pd3dDevice->SetIndices( _pGfx->gl_pd3dIdx, i1stVertex);
	D3D_CHECKERROR(hr);

	// update streams mask
	extern ULONG _ulStreamsMask;
	_ulStreamsMask = NONE;
	_ulStreamsMask |= 1<<iStream;

	_sfStats.StopTimer(CStatForm::STI_GFXAPI);
}


// set a sub-buffer to be the current one for rendering
static void d3d_SetSubBuffer( const INDEX iID, const INDEX iUnit/*=-1*/)
{
	// determine bind, type, steam and mask
	const INDEX iBind = iID >> 4;
	const INDEX iType = iID & 15;
	INDEX iStream;
	// if texture unit is set
	if(iUnit!=(-1)) {
		ASSERT(iType>=GFX_VBA_TEX && iType<GFX_MAX_VBA);
		iStream = _aiStreamFromType[GFX_VBA_TEX + iUnit];
	} else {
		iStream = _aiStreamFromType[iType];
	}

	ASSERT( iType>GFX_VBA_POS && iType<GFX_MAX_VBA && iBind<_avbVertexBuffers.Count());
	HRESULT hr;

	_sfStats.StartTimer(CStatForm::STI_GFXAPI);

	if(iID!=0) {
		// assign stream 
		VertexBuffer &vb = _avbVertexBuffers[iBind];
		LPDIRECT3DVERTEXBUFFER8 pd3dVB = vb.vb_pavbWrite[iType];
		hr = _pGfx->gl_pd3dDevice->SetStreamSource( iStream, pd3dVB, GetVBTypeSize(iType));
	}
	// check and mark
	D3D_CHECKERROR(hr);
	_bUsingDynamicBuffer = FALSE;

	// update streams mask
	extern ULONG _ulStreamsMask;
	_ulStreamsMask |= 1<<iStream;

	_sfStats.StopTimer(CStatForm::STI_GFXAPI);
}



// array support wrapper

static void d3d_SetVertexArray( GFXVertex *pvtx, INDEX ctVtx)
{
	ASSERT( _pGfx->gl_eCurrentAPI==GAT_D3D);
	ASSERT( ctVtx>0 && pvtx!=NULL && GFX_iActiveTexUnit==0);
	_sfStats.StartTimer(CStatForm::STI_GFXAPI);
	ASSERT( ctVtx>0 && ctVtx<65536);
	ctVtx = ClampUp( ctVtx, 65535L);

	// lock dynamic vertex array
	void *pVA = LockVertexArray_D3D(ctVtx);
	// copy vertices there and unlock
	CopyLongs( (ULONG*)pvtx, (ULONG*)pVA, ctVtx*sizeof(GFXVertex)/sizeof(FLOAT));
	UnlockVertexArray_D3D();

	_sfStats.StopTimer(CStatForm::STI_GFXAPI);
}



static void d3d_SetNormalArray( GFXNormal *pnor)
{
	ASSERT( _pGfx->gl_eCurrentAPI==GAT_D3D);
	ASSERT( pnor!=NULL);
	_sfStats.StartTimer(CStatForm::STI_GFXAPI);

	// lock dynamic normal array
	void *pNA = LockNormalArray_D3D();
	// copy vertices there and unlock
	CopyLongs( (ULONG*)pnor, (ULONG*)pNA, GFX_ctVertices*sizeof(GFXNormal)/sizeof(FLOAT));
	UnlockNormalArray_D3D();

	_sfStats.StopTimer(CStatForm::STI_GFXAPI);
}


static void d3d_SetWeightArray( GFXWeight *pwgh)
{
	ASSERT( _pGfx->gl_eCurrentAPI==GAT_D3D);
	ASSERT( pwgh!=NULL);
	_sfStats.StartTimer(CStatForm::STI_GFXAPI);

	// lock dynamic weight array
	void *pWA = LockWeightArray_D3D();
	// copy vertices there and unlock
	CopyLongs( (ULONG*)pwgh, (ULONG*)pWA, GFX_ctVertices*sizeof(GFXWeight)/sizeof(ULONG));
	UnlockWeightArray_D3D();

	_sfStats.StopTimer(CStatForm::STI_GFXAPI);
}


static void d3d_SetColorArray( GFXColor *pcol)
{
	ASSERT( _pGfx->gl_eCurrentAPI==GAT_D3D);
	ASSERT( pcol!=NULL);
	d3d_EnableColorArray();
	_sfStats.StartTimer(CStatForm::STI_GFXAPI);
	
	// lock dynamic color array
	void *pCA = LockColorArray_D3D();
	// convert vertices there and unlock
	abgr2argb( (ULONG*)pcol, (ULONG*)pCA, GFX_ctVertices*sizeof(GFXColor)/sizeof(ULONG));
	UnlockColorArray_D3D();

	_sfStats.StopTimer(CStatForm::STI_GFXAPI);
}



static void d3d_SetTexCoordArray( GFXTexCoord *ptex, BOOL bProjectiveMapping/*=FALSE*/)
{
	ASSERT( _pGfx->gl_eCurrentAPI==GAT_D3D);
	ASSERT( ptex!=NULL);
	ASSERTMSG(bProjectiveMapping==FALSE, "Projective mapping isn't supported any more");
	_sfStats.StartTimer(CStatForm::STI_GFXAPI);

	// lock dynamic texture coords array
	void *pTA = LockTexCoordArray_D3D( bProjectiveMapping);
	// copy vertices there and unlock
	const SLONG slSize = bProjectiveMapping ? sizeof(GFXTexCoord4) : sizeof(GFXTexCoord);
	CopyLongs( (ULONG*)ptex, (ULONG*)pTA, GFX_ctVertices*slSize/sizeof(FLOAT));
	UnlockTexCoordArray_D3D();

	_sfStats.StopTimer(CStatForm::STI_GFXAPI);
}


static void d3d_SetConstantColor( COLOR col)
{
	ASSERT( _pGfx->gl_eCurrentAPI==GAT_D3D);
	d3d_DisableColorArray();
	_sfStats.StartTimer(CStatForm::STI_GFXAPI);

	const ULONG d3dColor = rgba2argb(col);
	HRESULT hr = _pGfx->gl_pd3dDevice->SetRenderState( D3DRS_AMBIENT, d3dColor);
	D3D_CHECKERROR(hr); 

	_sfStats.StopTimer(CStatForm::STI_GFXAPI);
}


static void d3d_DrawElements( const INDEX ctIndices, const UWORD *puwIndices)
{
	ASSERT( _pGfx->gl_eCurrentAPI==GAT_D3D);
#ifndef NDEBUG
	// check if all indices are inside lock count (or smaller than 65536)
	if( puwIndices!=NULL) for( INDEX i=0; i<ctIndices; i++) ASSERT( puwIndices[i] < GFX_ctVertices);
#endif

	_sfStats.IncrementCounter( CStatForm::SCI_GFXVERTICES, ctIndices);
//강동민 수정 시작 테스트 클라이언트 작업	06.29
	extern INDEX gfx_bRenderReflection;
	if(gfx_bRenderReflection)
	{
		_sfStats.IncrementCounter( CStatForm::SCI_REFLECTION_VERTICE, ctIndices);
	}
//강동민 수정 끝 테스트 클라이언트 작업		06.29
	_sfStats.StartTimer(CStatForm::STI_GFXAPI);
	_pGfx->gl_ctTotalElements += ctIndices;  // for profiling

		ASSERT(puwIndices!=NULL);

	extern void DrawElements_D3D( INDEX ctIndices, const UWORD *puwIndices);
	DrawElements_D3D( ctIndices, puwIndices);

	_sfStats.StopTimer(CStatForm::STI_GFXAPI);
}



// force finish of API rendering queue
static void d3d_Finish( BOOL bReadBack)
{
	ASSERT( _pGfx->gl_eCurrentAPI==GAT_D3D);
	_sfStats.StartTimer(CStatForm::STI_GFXAPI);


	// must "emulate" this by reading from back buffer :(
	LPDIRECT3DSURFACE8 pBackBuffer = NULL;
	HRESULT hr = D3DERR_NOTAVAILABLE;

	// fetch back buffer
	const BOOL bFullScreen = _pGfx->gl_ulFlags & GLF_FULLSCREEN;
	if( bFullScreen) {
		// directly in fullscreen
		hr = _pGfx->gl_pd3dDevice->GetBackBuffer( 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer);
	} else if( _pGfx->gl_pvpActive!=NULL) {
		// thru swapchain in window
		hr = _pGfx->gl_pvpActive->vp_pSwapChain->GetBackBuffer( 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer);
	}
	// lock one little rectangle (like I need it, right)
	if( hr==D3D_OK && pBackBuffer!=NULL) {
		const RECT rectToLock  = { 0,0, 1,1 };
		D3DLOCKED_RECT rectLocked;
		hr = pBackBuffer->LockRect( &rectLocked, &rectToLock, D3DLOCK_READONLY);
		if( hr==D3D_OK) pBackBuffer->UnlockRect();
		D3DRELEASE( pBackBuffer, TRUE);
	}

	_sfStats.StopTimer(CStatForm::STI_GFXAPI);
}



static void d3d_LockArrays(void)
{
	// only for OpenGL
	ASSERT( _pGfx->gl_eCurrentAPI==GAT_D3D);
	ASSERT( GFX_ctVertices>0 && !_bCVAReallyLocked);
}



// SPECIAL FUNCTIONS FOR Direct3D ONLY !


// set D3D vertex shader only if different than last time
extern void d3dSetVertexShader( DWORD dwHandle)
{
	ASSERT( _pGfx->gl_eCurrentAPI==GAT_D3D);
	if( _pGfx->gl_dwVertexShader==dwHandle) return;
	HRESULT hr = _pGfx->gl_pd3dDevice->SetVertexShader(dwHandle);
	D3D_CHECKERROR(hr);
	_pGfx->gl_dwVertexShader = dwHandle;

	gfxSetPixelProgram(NONE);
}
