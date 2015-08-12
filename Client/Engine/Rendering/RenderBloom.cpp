#include "stdh.h"
#include <Engine/Rendering/Render.h>
#include <Engine/Rendering/Render_internal.h>

#include <Engine/Graphics/DrawPort.h>
#include <Engine/Graphics/GfxLibrary.h>
#include <Engine/GlobalDefinition.h>
// sehan
#include <assert.h>

#define FILTER_BLOOM_TEX			D3DTEXF_POINT
#define FILTER_BLOOM_TEX_TO_SCREEN	D3DTEXF_LINEAR

// FIXME : NULL로 초기화하지 않고 사용하고 있음.
CRenderTexture				*_prtFilterTarget[2];
LPDIRECT3DVERTEXBUFFER8		_pFilterVertexBuffer = NULL;
LPDIRECT3DVERTEXBUFFER8		_pBackVertexBuffer[MAX_BACKVERTEX_WIDTH][MAX_BACKVERTEX_HEIGHT]; // 1280 X 1024 까지만 지원되는걸로 가정한다.

extern INDEX d3d_bDeviceChanged	= TRUE;
extern BOOL _bFirst				= TRUE;

extern INDEX	g_iUseBloom;
//강동민 수정 시작 버그 사냥 작업	09.09
static DWORD	_dwAddFourPixelShader			= NULL;
static DWORD	_dwTexCoord4OffsetVertexShader	= NULL;
//강동민 수정 끝 버그 사냥 작업		09.09

extern void ReleaseBloomTexture()
{
	for( int i = 0; i < 2; i++ )
    {
		if (_prtFilterTarget[i]) 
		{
			delete _prtFilterTarget[i];
			_prtFilterTarget[i] = NULL;
		}
	}
	SAFE_RELEASE( _pFilterVertexBuffer );			// sets pointers to null after delete
	for (int height = 0; height < MAX_BACKVERTEX_HEIGHT; height++) 
	{
		for (int width = 0; width < MAX_BACKVERTEX_WIDTH; width++) 
		{
			SAFE_RELEASE( _pBackVertexBuffer[width][height] );			// sets pointers to null after delete
		}
	}
	
	HRESULT hr;	
	if(_dwTexCoord4OffsetVertexShader)
	{
		hr = _pGfx->gl_pd3dDevice->DeleteVertexShader(_dwTexCoord4OffsetVertexShader);
		_dwTexCoord4OffsetVertexShader = NULL;
	}
	if(_dwAddFourPixelShader)
	{
		hr = _pGfx->gl_pd3dDevice->DeletePixelShader(_dwAddFourPixelShader);
		_dwAddFourPixelShader = NULL;
	}
	_bFirst	= TRUE;
}

HRESULT CRenderer::InitBloom()
{
	//static bFirst = TRUE;
	if (_bFirst) 
	{
		// Init Device
		// 뷰, 투영등의 행렬값 계산하는 부분
		CalcFullCoverageMatrix(); // Vertex Shader에 넘겨줄 인수를 만든다. 한번만 해주면 된다.

		HRESULT hr = SetFlareLook();
		if (FAILED(hr)) 
		{
			return hr;
		}

		SAFE_RELEASE( _pFilterVertexBuffer );			// sets pointers to null after delete

		QuadVertex      *pBuff;

		hr = _pGfx->gl_pd3dDevice->CreateVertexBuffer( 4 * sizeof(QuadVertex), D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC,
											0, D3DPOOL_DEFAULT, &_pFilterVertexBuffer);
		//ASSERT_IF_FAILED(hr);
		if (FAILED(hr)) 
		{
			return hr;
		}

		if (_pFilterVertexBuffer)
		{
			hr = _pFilterVertexBuffer->Lock(0, 4 * sizeof(QuadVertex),(BYTE**)&pBuff, 0);
			//ASSERT_IF_FAILED(hr);
			if (FAILED(hr))
			{
				//FDebug("Couldn't lock buffer!");
				return(hr);
			}

			float uv_base;
			float uv_max;

			uv_base = 0.0f;
			uv_max  = 1.0f; 

			for (int i = 0; i < 4; ++i)
			{
				pBuff->Position = D3DXVECTOR3((i==0 || i==3) ? -1.0f : 1.0f,
											  (i<2)          ? -1.0f : 1.0f,
											  0.0f);
				pBuff->Tex      = D3DXVECTOR2((i==0 || i==3) ? uv_base : uv_max , 
											  (i<2)          ? uv_max : uv_base );
				pBuff++;
			}
			_pFilterVertexBuffer->Unlock();
		}

		// Restore Device
		// posyasi shaders
		std::vector<DWORD> Declaration;
		Declaration.push_back(D3DVSD_STREAM(0));
		Declaration.push_back(D3DVSD_REG( D3DVSDE_POSITION, D3DVSDT_FLOAT3));
		Declaration.push_back(D3DVSD_REG( D3DVSDE_TEXCOORD0, D3DVSDT_FLOAT2));
		Declaration.push_back(D3DVSD_END());

		if( FAILED( hr = CreateVShTexCoord4Offset( _pGfx->gl_pd3dDevice,
													 &Declaration[0] ) ) )
		{
			return hr;
		}

		if( FAILED( hr = CreatePShAddFour( )))
		{
			return hr;
		}
		//bFirst = FALSE;
		_bFirst = FALSE;
	}

	if (d3d_bDeviceChanged) 
	{
		d3d_bDeviceChanged = FALSE;
		HRESULT hr = CreateBackVertexBuffer();
		if (FAILED(hr)) 
		{
			return hr;
		}
	}
	return S_OK;
}

HRESULT CRenderer::CreateBackVertexBuffer()
{
	QuadVertex      *pBuff;
	int width, height;

	for (height = 0; height < MAX_BACKVERTEX_HEIGHT; height++) 
	{
		for (width = 0; width < MAX_BACKVERTEX_WIDTH; width++) 
		{
			SAFE_RELEASE( _pBackVertexBuffer[width][height] );			// sets pointers to null after delete
		}
	}

	// Create vertexbuffer for backbuffer;
	int s_width		= re_pdpDrawPort->GetWidth();
	int s_height	= re_pdpDrawPort->GetHeight();
	int width_num	= (s_width-1) / m_nTexRes + 1;
	int height_num	= (s_height-1) / m_nTexRes + 1;

	if (width_num > MAX_BACKVERTEX_WIDTH && height_num > MAX_BACKVERTEX_HEIGHT) {
		//assert(false);
		return E_FAIL;
	}

	for (height = 0; height < height_num; height++) 
	{
		for (width = 0; width < width_num; width++) 
		{
			HRESULT hr = _pGfx->gl_pd3dDevice->CreateVertexBuffer( 4 * sizeof(QuadVertex), D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC,
				0, D3DPOOL_DEFAULT, &_pBackVertexBuffer[width][height]);
			//ASSERT_IF_FAILED(hr);
			if (FAILED(hr)) 
			{
				return hr;
			}
			
			if (_pBackVertexBuffer[width][height])
			{
				hr = _pBackVertexBuffer[width][height]->Lock(0, 4 * sizeof(QuadVertex),(BYTE**)&pBuff, 0);
				//ASSERT_IF_FAILED(hr);
				if (FAILED(hr))
				{
					//FDebug("Couldn't lock buffer!");
					return(hr);
				}
				
				float uv_base;
				float uv_max;
				
				uv_base = 0.0f;
				uv_max  = 1.0f;

				float x_min = (2.0f * m_nTexRes / s_width * width) - 1.0f;
				float x_max = (2.0f * m_nTexRes / s_width * (width + 1)) - 1.0f;
				float y_max = (-2.0f * m_nTexRes / s_height * height) + 1.0f;
				float y_min = (-2.0f * m_nTexRes / s_height * (height + 1)) + 1.0f;

				for (int i = 0; i < 4; ++i)
				{
					pBuff->Position = D3DXVECTOR3((i==0 || i==3) ? x_min : x_max,
												  (i<2)          ? y_min : y_max,
												  0.0f);
					pBuff->Tex      = D3DXVECTOR2((i==0 || i==3) ? uv_base : uv_max , 
												  (i<2)          ? uv_max : uv_base );
					pBuff++;
				}
				_pBackVertexBuffer[width][height]->Unlock();
			}
		}
	}

	return S_OK;
}

HRESULT CRenderer::RenderBloom()
{
	// main bloom render
	Get_SRS_Bloom();

	DoCreateFlareTexture_Separable();

	int iFilterNum = (g_iUseBloom > 3) ? 1 : 0;
	m_nGaussianSize[iFilterNum]	= g_iUseBloom*2;
	CreateGaussianBlur(iFilterNum);

	HRESULT hr = S_OK;

	if (TRUE)
	{
		/////////////////////////////////////////////////////
		// Render Flare texture added on top of the object

		_pGfx->gl_pd3dDevice->SetRenderState( D3DRS_ZENABLE,		D3DZB_FALSE );
		_pGfx->gl_pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE,	false );

		_pGfx->gl_pd3dDevice->SetRenderState( D3DRS_CULLMODE,		D3DCULL_NONE );
		_pGfx->gl_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,	TRUE );
		//_pGfx->gl_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,	FALSE );

		//_pGfx->gl_pd3dDevice->SetRenderState( D3DRS_SRCBLEND,		D3DBLEND_ONE );
		_pGfx->gl_pd3dDevice->SetRenderState( D3DRS_DESTBLEND,		D3DBLEND_ONE );
		//_pGfx->gl_pd3dDevice->SetRenderState( D3DRS_DESTBLEND,		D3DBLEND_INVSRCCOLOR );
		_pGfx->gl_pd3dDevice->SetRenderState( D3DRS_SRCBLEND,		D3DBLEND_SRCCOLOR );
		//_pGfx->gl_pd3dDevice->SetRenderState( D3DRS_SRCBLEND,		D3DBLEND_INVDESTCOLOR );
		//_pGfx->gl_pd3dDevice->SetRenderState( D3DRS_SRCBLEND,		D3DBLEND_SRCALPHA );
		//_pGfx->gl_pd3dDevice->SetRenderState( D3DRS_DESTBLEND,		D3DBLEND_INVSRCALPHA );
		//_pGfx->gl_pd3dDevice->SetRenderState( D3DRS_SRCBLEND,		D3DBLEND_SRCCOLOR );
		//_pGfx->gl_pd3dDevice->SetRenderState( D3DRS_DESTBLEND,		D3DBLEND_INVSRCCOLOR );

		hr = _pGfx->gl_pd3dDevice->SetTexture( 0, (IDirect3DBaseTexture8 *) _prtFilterTarget[0]->rt_tdTexture.td_ulObject);

		// turn off pixel shading
		_pGfx->gl_pd3dDevice->SetPixelShader(0);	

		_pGfx->gl_pd3dDevice->SetTextureStageState( 0, D3DTSS_MAGFILTER, FILTER_BLOOM_TEX_TO_SCREEN	);
		_pGfx->gl_pd3dDevice->SetTextureStageState( 0, D3DTSS_MINFILTER, FILTER_BLOOM_TEX_TO_SCREEN	);
		_pGfx->gl_pd3dDevice->SetTextureStageState( 0, D3DTSS_MIPFILTER, D3DTEXF_NONE	); 

		_pGfx->gl_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,		D3DTOP_MODULATE );
		_pGfx->gl_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1,	D3DTA_TEXTURE	);
		_pGfx->gl_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2,	D3DTA_TFACTOR	);

		_pGfx->gl_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,		D3DTOP_DISABLE );
		_pGfx->gl_pd3dDevice->SetTextureStageState( 1, D3DTSS_COLOROP,		D3DTOP_DISABLE );

		//////////////////////////////////////////////
		// Render textured quad to cover render target
		D3DXVECTOR4  commonconst( 0.0f, 0.5f, 1.0f, 2.0f );
		_pGfx->gl_pd3dDevice->SetVertexShaderConstant( 5, &commonconst,    1); //CV_CONSTS_1

		_pGfx->gl_pd3dDevice->SetVertexShaderConstant( 0, &m_matFullCoverage_WVP(0, 0), 4 ); //CV_WORLDVIEWPROJ_0

		// Use texture offsets set 0 -- ie No offsets
		// All offset values are zero
		_pGfx->gl_pd3dDevice->SetVertexShaderConstant( 10, & (m_Offsets[ 0 ].t[0] ), 4 ); //CV_T0_OFFSET

		_pGfx->gl_pd3dDevice->SetVertexShader( _dwTexCoord4OffsetVertexShader );

		_pGfx->gl_pd3dDevice->SetStreamSource(0, _pFilterVertexBuffer, sizeof(QuadVertex));

		// Draw the simple quad set above
		hr = _pGfx->gl_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 2);

		//_pGfx->gl_pd3dDevice->LightEnable(0, TRUE);
		//_pGfx->gl_pd3dDevice->LightEnable(1, TRUE);
		//_pGfx->gl_pd3dDevice->LightEnable(2, TRUE);
	}

	Restore_SRS_Bloom();

	return( hr );
}

HRESULT CRenderer::DoCreateFlareTexture_Separable()
{
	// Create the Gaussian Flare in two step.  The first blurs
	//  in the u axis only.  The 2nd blurs in the v axis only.

	HRESULT hr = S_OK;

	int i, n;

	///////////////////////////////////////////////////////////////////////

	_pGfx->gl_pd3dDevice->SetPixelShader(0);

	// 큰 텍스춰를 작은 텍스춰에 다시 쓴다.
	if (TRUE) {
		IDirect3DSurface8       *pBackbufferColor; 
		_pGfx->gl_pd3dDevice->GetRenderTarget(        &pBackbufferColor );

		_prtFilterTarget[0]->Begin();
		_prtFilterTarget[0]->Clear(D3DCOLOR_XRGB( 0x0, 0x00, 0x00 ));

		_pGfx->gl_pd3dDevice->SetRenderState( D3DRS_ZENABLE,		D3DZB_FALSE );
		_pGfx->gl_pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE,	false );

		_pGfx->gl_pd3dDevice->SetRenderState( D3DRS_CULLMODE,		D3DCULL_NONE );
		//_pGfx->gl_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,	true );
		_pGfx->gl_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,	FALSE );

		//_pGfx->gl_pd3dDevice->SetRenderState( D3DRS_SRCBLEND,		D3DBLEND_ONE );
		_pGfx->gl_pd3dDevice->SetRenderState( D3DRS_DESTBLEND,		D3DBLEND_ONE );
		_pGfx->gl_pd3dDevice->SetRenderState( D3DRS_SRCBLEND,		D3DBLEND_SRCCOLOR );
		_pGfx->gl_pd3dDevice->SetRenderState( D3DRS_FOGENABLE,      FALSE );

		// turn off pixel shading
		_pGfx->gl_pd3dDevice->SetPixelShader(0);	

		_pGfx->gl_pd3dDevice->SetTextureStageState( 0, D3DTSS_MAGFILTER, FILTER_BLOOM_TEX_TO_SCREEN	);
		_pGfx->gl_pd3dDevice->SetTextureStageState( 0, D3DTSS_MINFILTER, FILTER_BLOOM_TEX_TO_SCREEN	);
		_pGfx->gl_pd3dDevice->SetTextureStageState( 0, D3DTSS_MIPFILTER, D3DTEXF_NONE	); 

		_pGfx->gl_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,		D3DTOP_MODULATE );
		_pGfx->gl_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1,	D3DTA_TEXTURE	);
		_pGfx->gl_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2,	D3DTA_TFACTOR	);

		_pGfx->gl_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,		D3DTOP_DISABLE );
		_pGfx->gl_pd3dDevice->SetTextureStageState( 1, D3DTSS_COLOROP,		D3DTOP_DISABLE );

		//////////////////////////////////////////////
		// Render textured quad to cover render target
		D3DXVECTOR4  commonconst( 0.0f, 0.5f, 1.0f, 2.0f );
		_pGfx->gl_pd3dDevice->SetVertexShaderConstant( 5, &commonconst,    1); //CV_CONSTS_1

		_pGfx->gl_pd3dDevice->SetVertexShaderConstant( 0, &m_matFullCoverage_WVP(0, 0), 4 ); //CV_WORLDVIEWPROJ_0

		// Use texture offsets set 0 -- ie No offsets
		// All offset values are zero
		_pGfx->gl_pd3dDevice->SetVertexShaderConstant( 10, & (m_Offsets[ 0 ].t[0] ), 4 ); //CV_T0_OFFSET

		_pGfx->gl_pd3dDevice->SetVertexShader( _dwTexCoord4OffsetVertexShader );

		// Draw the simple quad set above
		int s_width		= re_pdpDrawPort->GetWidth();
		int s_height	= re_pdpDrawPort->GetHeight();
		int width_num	= (s_width-1) / m_nTexRes + 1;
		int height_num	= (s_height-1) / m_nTexRes + 1;
		RECT rect;
		POINT p1;
		p1.x = 0;
		p1.y = 0;
		for (int height = 0; height < height_num; height++) 
		{
			for (int width = 0; width < width_num; width++) 
			{
				if (_pBackVertexBuffer[width][height]) 
				{
					_pGfx->gl_pd3dDevice->SetStreamSource(0, _pBackVertexBuffer[width][height], sizeof(QuadVertex));
					rect.top	= height * m_nTexRes;
					rect.bottom	= (height * m_nTexRes) + m_nTexRes;
					rect.left	= width * m_nTexRes;
					rect.right	= (width * m_nTexRes) + m_nTexRes;
					if (rect.bottom > s_height)
						rect.bottom = s_height;
					if (rect.right > s_width)
						rect.right = s_width;
					_pGfx->gl_pd3dDevice->CopyRects(pBackbufferColor, &rect, 1, _prtFilterTarget[1]->rt_pSurface, &p1);
					_pGfx->gl_pd3dDevice->SetTexture( 0, (IDirect3DBaseTexture8 *) _prtFilterTarget[1]->rt_tdTexture.td_ulObject );
					hr = _pGfx->gl_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 2);
				}
			}
		}

		//_pGfx->gl_pd3dDevice->LightEnable(0, TRUE);
		//_pGfx->gl_pd3dDevice->LightEnable(1, TRUE);
		//_pGfx->gl_pd3dDevice->LightEnable(2, TRUE);

		_prtFilterTarget[0]->End();

		pBackbufferColor->Release();
	}
	//return S_OK;

	///////////////////////////////////////////////////////////////
	// change render targets & blur out the glow source pixels rendered previously
	// Blur them in u (horizontal) axis only

	//return S_OK;
	//_pGfx->gl_pd3dDevice->SetRenderTarget( m_pFilterTarget[1], NULL );
	_prtFilterTarget[1]->Begin();
	_prtFilterTarget[1]->Clear(D3DCOLOR_XRGB( 0x0, 0x00, 0x00 ));

	for( i=0; i < 4; i++ )
	{
		_pGfx->gl_pd3dDevice->SetTextureStageState( i, D3DTSS_ADDRESSU,  D3DTADDRESS_CLAMP );
		_pGfx->gl_pd3dDevice->SetTextureStageState( i, D3DTSS_ADDRESSV,  D3DTADDRESS_CLAMP );

        hr = _pGfx->gl_pd3dDevice->SetTextureStageState( i, D3DTSS_MAGFILTER, FILTER_BLOOM_TEX		);
		//ASSERT_IF_FAILED(hr);
	    hr = _pGfx->gl_pd3dDevice->SetTextureStageState( i, D3DTSS_MINFILTER, FILTER_BLOOM_TEX		);
		//ASSERT_IF_FAILED(hr);
		hr = _pGfx->gl_pd3dDevice->SetTextureStageState( i, D3DTSS_MIPFILTER, FILTER_BLOOM_TEX		); 
		//ASSERT_IF_FAILED(hr);
	}


	_pGfx->gl_pd3dDevice->SetRenderState( D3DRS_CULLMODE,			D3DCULL_NONE );

	_pGfx->gl_pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE,		false );
	_pGfx->gl_pd3dDevice->SetRenderState( D3DRS_ZENABLE,			D3DZB_FALSE );


	/*_pGfx->gl_pd3dDevice->SetTexture( 0, m_pTextureFiltered[0] );
	_pGfx->gl_pd3dDevice->SetTexture( 1, m_pTextureFiltered[0] );
	_pGfx->gl_pd3dDevice->SetTexture( 2, m_pTextureFiltered[0] );
	_pGfx->gl_pd3dDevice->SetTexture( 3, m_pTextureFiltered[0] );*/
	_pGfx->gl_pd3dDevice->SetTexture( 0, (IDirect3DBaseTexture8 *) _prtFilterTarget[0]->rt_tdTexture.td_ulObject);
	_pGfx->gl_pd3dDevice->SetTexture( 1, (IDirect3DBaseTexture8 *) _prtFilterTarget[0]->rt_tdTexture.td_ulObject);
	_pGfx->gl_pd3dDevice->SetTexture( 2, (IDirect3DBaseTexture8 *) _prtFilterTarget[0]->rt_tdTexture.td_ulObject);
	_pGfx->gl_pd3dDevice->SetTexture( 3, (IDirect3DBaseTexture8 *) _prtFilterTarget[0]->rt_tdTexture.td_ulObject);

	D3DXVECTOR4  commonconst( 0.0f, 0.5f, 1.0f, 2.0f );
	_pGfx->gl_pd3dDevice->SetVertexShaderConstant( 5, &commonconst,    1); //CV_CONSTS_1
	_pGfx->gl_pd3dDevice->SetVertexShaderConstant( 0, & m_matFullCoverage_WVP(0, 0), 4); //CV_WORLDVIEWPROJ_0

	_pGfx->gl_pd3dDevice->SetPixelShader( _dwAddFourPixelShader );
	//hr = SetPixelShader( m_PSHI_AddFourSamples );	

	_pGfx->gl_pd3dDevice->SetVertexShader( _dwTexCoord4OffsetVertexShader );
	//SetVertexShader( m_SHI_4Offset_VS );
	_pGfx->gl_pd3dDevice->SetStreamSource(0, _pFilterVertexBuffer, sizeof(QuadVertex));

	float pix_mult[4][4];
	float woff, hoff;
	float centw, centh;

	centw = m_fPerTexelWidth * 0.5f;
	centh = m_fPerTexelHeight * 0.5f;

	centw = 0.0f;
	centh = 0.0f;

	//  1st pass is opaque
	_pGfx->gl_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, false );
	// Additive blending when alphablend is enabled

	_pGfx->gl_pd3dDevice->SetRenderState( D3DRS_SRCBLEND,		D3DBLEND_ONE );
	//_pGfx->gl_pd3dDevice->SetRenderState( D3DRS_SRCBLEND,		D3DBLEND_DESTCOLOR );
	//if ( m_bAddSmooth )
	{
		_pGfx->gl_pd3dDevice->SetRenderState( D3DRS_DESTBLEND,		D3DBLEND_INVSRCCOLOR );
		//_pGfx->gl_pd3dDevice->SetRenderState( D3DRS_DESTBLEND,		D3DBLEND_SRCCOLOR );
	}

	int iFilterNum;
	iFilterNum = (g_iUseBloom > 3) ? 1 : 0;
/*	if (g_iCountry == JAPAN) // 일본 신 주노맵 밝기때문에 블롬효과 단계 늘림
	{
		iFilterNum = (g_iUseBloom > 3) ? 1 : 0;
	}else
	{
		iFilterNum = (g_iUseBloom > 0) ? (g_iUseBloom - 1) : 0;
	}*/
	
	for( i=0; i < m_vGaussian1D[iFilterNum].size(); i += 4 )
	{
		if( i == 4 )
		{
			// switch to additive blending if > 1 pass - ie if 
			//  gaussian has more than 4 elements.

			_pGfx->gl_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, true );
		}
		
		for( n=0; n < 4; n++ )
		{
			FilterKernelElement * pE;
			pE = & ( m_vGaussian1D[iFilterNum].at(i+n) );

			pix_mult[n][0] = pE->coef * m_fColorAtten[iFilterNum];
			pix_mult[n][1] = pE->coef * m_fColorAtten[iFilterNum];
			pix_mult[n][2] = pE->coef * m_fColorAtten[iFilterNum];
			pix_mult[n][3] = pE->coef * m_fColorAtten[iFilterNum];

			woff = m_fPerTexelWidth  * pE->du;
			hoff = m_fPerTexelHeight * pE->dv;

			m_Offsets[ 2 ].t[n] = D3DXVECTOR4( centw + woff, centh + hoff, 0.0f, 0.0f );
		}

		_pGfx->gl_pd3dDevice->SetPixelShaderConstant( 0, &pix_mult, 4 ); //PCN_MULTFACTOR_0
		_pGfx->gl_pd3dDevice->SetVertexShaderConstant( 10, & (m_Offsets[ 2 ].t[0] ), 4 ); //CV_T0_OFFSET

		// Draw the fan to cover the entire render target
		hr = _pGfx->gl_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 2);

		//ASSERT_IF_FAILED(hr);
	}
	_prtFilterTarget[1]->End();


	///////////////////////////////////////////////////////////////
	// Now flip from rendertarget 1 to rendertarget 0 and blur
	//  in the other axis.

	// set textures to NULL to a texture is never simultaneously an 
	//  active texture and a render target
	_pGfx->gl_pd3dDevice->SetTexture( 0, NULL );
	_pGfx->gl_pd3dDevice->SetTexture( 1, NULL );
	_pGfx->gl_pd3dDevice->SetTexture( 2, NULL );
	_pGfx->gl_pd3dDevice->SetTexture( 3, NULL );

	// Switch render targets
	_prtFilterTarget[0]->Begin();
	_prtFilterTarget[0]->Clear(D3DCOLOR_XRGB( 0x0, 0x00, 0x00 ));

	_pGfx->gl_pd3dDevice->SetTexture( 0, (IDirect3DBaseTexture8 *) _prtFilterTarget[1]->rt_tdTexture.td_ulObject);
	_pGfx->gl_pd3dDevice->SetTexture( 1, (IDirect3DBaseTexture8 *) _prtFilterTarget[1]->rt_tdTexture.td_ulObject);
	_pGfx->gl_pd3dDevice->SetTexture( 2, (IDirect3DBaseTexture8 *) _prtFilterTarget[1]->rt_tdTexture.td_ulObject);
	_pGfx->gl_pd3dDevice->SetTexture( 3, (IDirect3DBaseTexture8 *) _prtFilterTarget[1]->rt_tdTexture.td_ulObject);

	//  1st pass is opaque
	_pGfx->gl_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, false );

	for( i=0; i < m_vGaussian1D[iFilterNum].size(); i += 4 )
	{
		if( i == 4 )
		{
			// switch to additive blending if > 1 pass - ie if 
			//  gaussian has more than 4 elements.

			_pGfx->gl_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, true );
		}
		
		for( n=0; n < 4; n++ )
		{

			FilterKernelElement * pE;
			pE = & ( m_vGaussian1D[iFilterNum].at(i+n) );

			pix_mult[n][0] = pE->coef * m_fColorAtten[iFilterNum];
			pix_mult[n][1] = pE->coef * m_fColorAtten[iFilterNum];
			pix_mult[n][2] = pE->coef * m_fColorAtten[iFilterNum];
			pix_mult[n][3] = pE->coef * m_fColorAtten[iFilterNum];

			// Switch nature of u and v to blur along other axis. (dv should be 0.0f)
			woff = m_fPerTexelWidth  * pE->dv;
			hoff = m_fPerTexelHeight * pE->du;

			m_Offsets[ 2 ].t[n] = D3DXVECTOR4( centw + woff, centh + hoff, 0.0f, 0.0f );
		}

		_pGfx->gl_pd3dDevice->SetPixelShaderConstant( 0, &pix_mult, 4 ); //PCN_MULTFACTOR_0

		_pGfx->gl_pd3dDevice->SetVertexShaderConstant( 10, & (m_Offsets[ 2 ].t[0] ), 4 ); //CV_T0_OFFSET


		// Draw the fan to cover the entire render target
		hr = _pGfx->gl_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 2);

		//ASSERT_IF_FAILED(hr);
	}
	_prtFilterTarget[0]->End();

	return( hr );
}

void CRenderer::Get_SRS_Bloom()
{
	// Get Settings
	for(int i=0; i < 4; i++ )
	{
		_pGfx->gl_pd3dDevice->GetTextureStageState( i, D3DTSS_ADDRESSU,  &m_dwAddressU[i] );
		_pGfx->gl_pd3dDevice->GetTextureStageState( i, D3DTSS_ADDRESSV,  &m_dwAddressV[i] );

        _pGfx->gl_pd3dDevice->GetTextureStageState( i, D3DTSS_MAGFILTER, &m_dwMagfilter[i]		);
	    _pGfx->gl_pd3dDevice->GetTextureStageState( i, D3DTSS_MINFILTER, &m_dwMinfilter[i]		);
		_pGfx->gl_pd3dDevice->GetTextureStageState( i, D3DTSS_MIPFILTER, &m_dwMipfilter[i]		); 
	}

	_pGfx->gl_pd3dDevice->GetRenderState( D3DRS_ZENABLE,		&m_dwZenable );
	_pGfx->gl_pd3dDevice->GetRenderState( D3DRS_ZWRITEENABLE,	&m_dwZwriteenable );

	_pGfx->gl_pd3dDevice->GetRenderState( D3DRS_CULLMODE,		&m_dwCullmode );
	_pGfx->gl_pd3dDevice->GetRenderState( D3DRS_ALPHABLENDENABLE,	&m_dwAlphablendenable );

	_pGfx->gl_pd3dDevice->GetRenderState( D3DRS_SRCBLEND,		&m_dwSrcblend );
	_pGfx->gl_pd3dDevice->GetRenderState( D3DRS_DESTBLEND,		&m_dwDestblend );

	_pGfx->gl_pd3dDevice->GetRenderState( D3DRS_FOGENABLE,      &m_dwFogEnable );

	_pGfx->gl_pd3dDevice->GetTextureStageState( 0, D3DTSS_COLOROP,		&m_dwColorop );
	_pGfx->gl_pd3dDevice->GetTextureStageState( 0, D3DTSS_COLORARG1,	&m_dwColorarg1	);
	_pGfx->gl_pd3dDevice->GetTextureStageState( 0, D3DTSS_COLORARG2,	&m_dwColorarg2	);

	_pGfx->gl_pd3dDevice->GetTextureStageState( 0, D3DTSS_ALPHAOP,		&m_dwAlphaop );
	_pGfx->gl_pd3dDevice->GetTextureStageState( 1, D3DTSS_COLOROP,		&m_dwColorop1 );
	///
}

void CRenderer::Restore_SRS_Bloom()
{
	// Restore Settings...
	for(int i=0; i < 4; i++ )
	{
		_pGfx->gl_pd3dDevice->SetTextureStageState( i, D3DTSS_ADDRESSU,  m_dwAddressU[i] );
		_pGfx->gl_pd3dDevice->SetTextureStageState( i, D3DTSS_ADDRESSV,  m_dwAddressV[i] );

        _pGfx->gl_pd3dDevice->SetTextureStageState( i, D3DTSS_MAGFILTER, m_dwMagfilter[i]		);
	    _pGfx->gl_pd3dDevice->SetTextureStageState( i, D3DTSS_MINFILTER, m_dwMinfilter[i]		);
		_pGfx->gl_pd3dDevice->SetTextureStageState( i, D3DTSS_MIPFILTER, m_dwMipfilter[i]		);
	}

	_pGfx->gl_pd3dDevice->SetRenderState( D3DRS_ZENABLE,		m_dwZenable );
	_pGfx->gl_pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE,	m_dwZwriteenable );

	_pGfx->gl_pd3dDevice->SetRenderState( D3DRS_CULLMODE,		m_dwCullmode );
	_pGfx->gl_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,	m_dwAlphablendenable );

	_pGfx->gl_pd3dDevice->SetRenderState( D3DRS_SRCBLEND,		m_dwSrcblend );
	_pGfx->gl_pd3dDevice->SetRenderState( D3DRS_DESTBLEND,		m_dwDestblend );

	_pGfx->gl_pd3dDevice->SetRenderState( D3DRS_FOGENABLE,      m_dwFogEnable );

	_pGfx->gl_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,		m_dwColorop );
	_pGfx->gl_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1,	m_dwColorarg1	);
	_pGfx->gl_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2,	m_dwColorarg2	);

	_pGfx->gl_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,		m_dwAlphaop );
	_pGfx->gl_pd3dDevice->SetTextureStageState( 1, D3DTSS_COLOROP,		m_dwColorop1 );
	
//안태훈 수정 시작	//(Bug Fix)(0.1)
	d3dSetVertexShader(D3DFVF_CTVERTEX);
//안태훈 수정 끝	//(Bug Fix)(0.1)
}

HRESULT CRenderer::CreateTextureRenderTargets( int width, int height )
{
	int i;

	for( i=0; i < 2; i++ )
	{
		if (_prtFilterTarget[i]) 
		{
			delete _prtFilterTarget[i];
			_prtFilterTarget[i] = NULL;
		}
	}

	CreateUVOffsets( width, height );

    for( i = 0; i < 2; i++ )
    {
		IDirect3DSurface8       *pBackbufferColor;
		D3DSURFACE_DESC backDesc;
		_pGfx->gl_pd3dDevice->GetRenderTarget( &pBackbufferColor );
		pBackbufferColor->GetDesc(&backDesc);

		// 렌더 텍스쳐 생성 및 초기화
		_prtFilterTarget[i] = new CRenderTexture();
		if (!_prtFilterTarget[i] || !_prtFilterTarget[i]->Init(width, height, TEX_32BIT, backDesc.Format)) 
		{ 
			//D3DFMT_X8R8G8B8)) {
            return E_FAIL;
		}
		pBackbufferColor->Release();
    }

    return S_OK;
}

HRESULT	CRenderer::SetFlareLook()
{
	SAFE_ARRAY_DELETE( m_pGaussianProp );
	
	m_pGaussianProp = new GaussianProp[ 2 ];
	
	if( m_pGaussianProp == NULL )
	{
		return E_FAIL;
	}
	
	// really big kernel
	
	m_nNumGaussiansInSingleBlur = 2;
	
	m_pGaussianProp[0].radius_scale		= 0.18f;
	m_pGaussianProp[0].amp				= 0.24f;
	
	m_pGaussianProp[1].radius_scale		= 4.5f;
	m_pGaussianProp[1].amp				= 0.11f;
/*	
	//m_nGaussianSize	 = 8;//44;		// size of the Gaussian blur diameter in texels
	// Loop 2번 돈다.
	//m_nGaussianSize[0]	 = 10;//44;		// size of the Gaussian blur diameter in texels
	//m_fColorAtten[0]	= 0.46f;//0.20f;
	// Loop 3번 돈다.
	m_nGaussianSize[0]	= g_iUseBloom*2;//12;//44;		// size of the Gaussian blur diameter in texels
	m_fColorAtten[0]	= 0.40f;//0.20f;
	// Loop 4번 돈다.
	m_nGaussianSize[1]	= 12;//15;//44;		// size of the Gaussian blur diameter in texels
	m_fColorAtten[1]	= 0.35f;//0.20f;
*/
	//Bloom 효과 조절
/*	if(g_iCountry == JAPAN)
	{
		m_nGaussianSize[0]	= g_iUseBloom*2; // 옵션의 블럼 단계를 반영 // 신 주노맵의 Bloom 효과 단계 늘림
		m_nGaussianSize[1]	= 12;
	}else
	{
		m_nGaussianSize[0]	= 12;
		m_nGaussianSize[1]	= 15;
	}*/
	m_nGaussianSize[0]	= g_iUseBloom*2; // 옵션의 블럼 단계를 반영 // 신 주노맵의 Bloom 효과 단계 늘림
	m_nGaussianSize[1]	= 12;

	m_fColorAtten[0] = 0.40f;
	m_fColorAtten[1] = 0.35f;

	m_nTexRes			= 512;
	
	HRESULT hr = CreateTextureRenderTargets( m_nTexRes, m_nTexRes );
	
	if (FAILED(hr)) 
	{
		return hr;
	}
	
	CreateGaussianBlur(0);
	CreateGaussianBlur(1);
	
	return S_OK;
}

void	CRenderer::CreateGaussianBlur(int filterNum)
{
	FilterKernelElement el;

	int u;
	float cent = (((float)m_nGaussianSize[filterNum]) - 1.0f ) / 2.0f;

	float coe;
	float radi;

	int remainder;

	///////////////////////////////////////////////////////
	// Create 1D Gaussian to use in creating 2D blur 

	m_vGaussian1D[filterNum].clear();

	int m;

	for( u=0; u < m_nGaussianSize[filterNum]; u++ )
	{

		el.du =  ((float)u) - cent + m_fTemp;

		el.dv =  0.0f;

		radi = el.du * el.du / (cent * cent);

		// do a regular gaussian type function

		el.coef = 0.0f;
		for( m=0; m < m_nNumGaussiansInSingleBlur; m++ )
		{
			coe = radi * m_pGaussianProp[m].radius_scale;
			coe = (float) ( 1.0f / exp( coe ) );
			el.coef += coe * m_pGaussianProp[m].amp;
		}

		// only add the element if it is a significant contribution - 
		//  greater than 0.01 -- or 1 percent

		if(  ( (el.coef > 0.01f ) && m_bElimSmallContributors ) ||
				! m_bElimSmallContributors								)
		{
			//FDebug("Gaussian element:  du: %f  dv: %f   coef: %f\n", el.du, el.dv, el.coef );
			m_vGaussian1D[filterNum].push_back( el );
		}

	}

	// Now make sure the array has a multiple of 4 elements
	el.du = 0.0f;
	el.dv = 0.0f;
	el.coef = 0.0f;

	remainder = m_vGaussian1D[filterNum].size() % 4;
	remainder = ( 4 - remainder ) % 4;

	for( u = 0; u < remainder; u++ )
	{
		m_vGaussian1D[filterNum].push_back( el );
	}

	//FDebug("Gaussian has %d elements\n", m_vGaussian1D.size() );

	if( m_vGaussian1D[filterNum].size() % 4 != 0 )
	{
		//FDebug("My logic is seriously messed up - m_vGaussian.size() should be a multiple of 4!!\n");
		assert( false );
	}
}

void CRenderer::CreateUVOffsets( int width, int height )
{
	// These offsets are added to each texture coordinate
	//  before it is iterated in rasterization.  Each texture
	//  stage can have a different offset value, so when one
	//  texture is selected into all four stages, the effect
	//  is to sample from various patterns of neighboring 
	//  texels.
	
	// Here are labels for neighboring texels for when the 
	//  pixel at 'e' is being rendered
    
    m_fPerTexelWidth  = 1.0f / (float)width;
    m_fPerTexelHeight = 1.0f / (float)height;

	float woff, hoff;
	woff =  m_fPerTexelWidth  / 2.0f;
	hoff =  m_fPerTexelHeight / 2.0f;

	//////////////////////////////////////////////////////////
	// Ofset set 0 : center texel sampling
	// No offsets
	// Because of the way D3D samples, we must use a half-texel
	//   offset to hit the center of each texel
	// This behavior is different in OpenGL and you will need different 
	//   values for woff and hoff if porting this sample to OpenGL.  The
	//   values for OpenGL may be 0.0f for both, but you should test your
	//   code to make sure you hit the exact texel centers
	
	m_Offsets[ 0 ].t[0] = D3DXVECTOR4( woff, hoff, 0.0f, 0.0f );
	m_Offsets[ 0 ].t[1] = D3DXVECTOR4( woff, hoff, 0.0f, 0.0f );
	m_Offsets[ 0 ].t[2] = D3DXVECTOR4( woff, hoff, 0.0f, 0.0f );
	m_Offsets[ 0 ].t[3] = D3DXVECTOR4( woff, hoff, 0.0f, 0.0f );

	//////////////////////////////////////////////////////////
	// Offset set 1:  Nearest neighbors -  b,d,f,h texels
	m_Offsets[ 1 ].t[0] = D3DXVECTOR4( - m_fPerTexelWidth	+ woff,  0.0f + hoff, 0.0f, 0.0f );
	m_Offsets[ 1 ].t[1] = D3DXVECTOR4(   m_fPerTexelWidth	+ woff,  0.0f + hoff, 0.0f, 0.0f );

	m_Offsets[ 1 ].t[2] = D3DXVECTOR4(   0.0f + woff,       m_fPerTexelHeight	+ hoff, 0.0f, 0.0f );
	m_Offsets[ 1 ].t[3] = D3DXVECTOR4(   0.0f + woff,     - m_fPerTexelHeight	+ hoff, 0.0f, 0.0f );

	// Offset set 2 has the effect of scrolling the texture as
	//   it is sampled.  This scrolling amount is animated in 
	//   the DoSingleTimeStep, so they are not set here
}

void CRenderer::CalcFullCoverageMatrix()
{
	///////////////////////////////////////////////////////////////
	// Set matrix so that a single quad from -1,-1 to 1,1 exactly
	//  covers the entire render target

	D3DXMATRIX matWorld;
	D3DXMATRIX matView;
	D3DXMATRIX matProj;
	D3DXMATRIX matViewProj;
	D3DXMATRIX matWorldViewProj;

	D3DXVECTOR3 const vEyePt    = D3DXVECTOR3( 0.0f, 0.0f, -5.0f );
	D3DXVECTOR3 const vLookatPt = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	D3DXVECTOR3 const vUp       = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );

	// Set World, View, Projection, and combination matrices.
	D3DXMatrixLookAtLH(&matView, &vEyePt, &vLookatPt, &vUp);
	D3DXMatrixOrthoLH(&matProj, 4.0f, 4.0f, 0.2f, 20.0f);

    D3DXMatrixMultiply(&matViewProj, &matView, &matProj);

    // draw a single quad to texture:
    // the quad covers the whole render target exactly
	D3DXMatrixScaling(&matWorld, 2.0f, 2.0f, 1.0f);
	D3DXMatrixMultiply(&matWorldViewProj, &matWorld, &matViewProj);
    D3DXMatrixTranspose( & m_matFullCoverage_WVP, &matWorldViewProj);
}

HRESULT CRenderer::CreatePShAddFour( )
{
    //TCHAR        strPath[MAX_PATH];
	LPD3DXBUFFER pCode;
    HRESULT      hr;

/*	char *PshAddFourPixelShaderCode =
		"ps.1.1\n"
		"tex t0 \n"
		"tex t1 \n"
		"tex t2 \n"
		"tex t3 \n"
		"mul r0.xyz , c0 , t0 \n"
		//";+mul r1.w , c0.wwww , t0.wwww \n"
		"mad t0.xyz , c1 , t1 , r0 \n"
		//";+mad t0.w , c1.wwww , t1.wwww , r1 \n"
		//";mov t0.w , c5\n"
		//";mul r0 , c0 , t0 \n"
		//";mad t0 , c1 , t1 , r0 \n"
		"mad t0 , c2 , t2 , t0 \n"
		"mad r0 , c3 , t3 , t0 ";
*/
	char *PshAddFourPixelShaderCode =
		"ps.1.4\n"
		"texld r0, t0 \n"
		"texld r1, t1 \n"
		"texld r2, t2 \n"
		"texld r3, t3 \n"
		"mul r0.xyz , r0 , c0 \n"
		//";+mul r1.w , c0.wwww , t0.wwww \n"
		"mad r0.xyz , c1 , r1 , r0 \n"
		//";+mad t0.w , c1.wwww , t1.wwww , r1 \n"
		//";mov t0.w , c5\n"
		//";mul r0 , c0 , t0 \n"
		//";mad t0 , c1 , t1 , r0 \n"
		"mad r0 , c2 , r2 , r0 \n"
		"mad r0 , c3 , r3 , r0 ";


    // Get the path to the vertex shader file
    //DXUtil_FindMediaFile( strPath, strFilename );
	//strcpy(strPath, "D:/project/New/Shaders/");
	//strcat(strPath, strFilename);

    // Assemble the vertex shader file
    if( FAILED( hr = D3DXAssembleShader( PshAddFourPixelShaderCode, strlen(PshAddFourPixelShaderCode), 0, NULL, &pCode, NULL ) ) )
        return hr;

	// Delete old pixel shader
	if( _dwAddFourPixelShader )
	{
		_pGfx->gl_pd3dDevice->DeletePixelShader( _dwAddFourPixelShader );
		_dwAddFourPixelShader = NULL;
	}

	// Create new pixel shader
	hr = _pGfx->gl_pd3dDevice->CreatePixelShader( (DWORD*)pCode->GetBufferPointer(),
                                              &_dwAddFourPixelShader );

	pCode->Release();
    return S_OK;
}

HRESULT CRenderer::CreateVShTexCoord4Offset( LPDIRECT3DDEVICE8 pd3dDevice, 
                                    DWORD* pdwVertexDecl)
{
    LPD3DXBUFFER pCode;
    //TCHAR        strPath[MAX_PATH];
    HRESULT      hr;

	char *VshTexCoord4OffsetCode =
		"vs.1.1\n"
		"dp4 oPos.x , c0 , v0 \n"
		"dp4 oPos.y , c1 , v0 \n"
		"dp4 oPos.z , c2 , v0 \n"
		"dp4 oPos.w , c3 , v0 \n"
		"add oT0 , v7 , c10 \n"
		"add oT1 , v7 , c11 \n"
		"add oT2 , v7 , c12 \n"
		"add oT3 , v7 , c13 ";

    // Get the path to the vertex shader file
    //DXUtil_FindMediaFile( strPath, strFilename );
	//strcpy(strPath, "D:/project/New/Shaders/");
	//strcat(strPath, strFilename);
	//strcpy(strPath, strFilename);

    // Assemble the vertex shader file
    //if( FAILED( hr = D3DXAssembleShaderFromFile( strPath, 0, NULL, &pCode, NULL ) ) ) {
	if( FAILED( hr = D3DXAssembleShader( VshTexCoord4OffsetCode, strlen(VshTexCoord4OffsetCode), 0, NULL, &pCode, NULL ) ) ) 
	{
		if (hr == D3DERR_INVALIDCALL )
		{
			int aaa = 1;
		} 
		else if (hr == D3DXERR_INVALIDDATA ) 
		{
			int aaa = 2;
		} 
		else if (hr == E_OUTOFMEMORY ) 
		{
			int aaa = 3;
		}
        return hr;
	}

	// Delete old pixel shader
	if( _dwTexCoord4OffsetVertexShader )
	{
		_pGfx->gl_pd3dDevice->DeleteVertexShader( _dwTexCoord4OffsetVertexShader );
		_dwTexCoord4OffsetVertexShader = NULL;
	}

    // Create the vertex shader
    hr = pd3dDevice->CreateVertexShader( pdwVertexDecl, 
                                         (DWORD*)pCode->GetBufferPointer(),
                                         &_dwTexCoord4OffsetVertexShader, 0 );
    pCode->Release();
    return hr;
}

// sehan end
