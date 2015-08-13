#include "stdh.h"

#include <Engine/Graphics/Texture.h>

#include <Engine/Base/Stream.h>
#include <Engine/Base/Timer.h>
#include <Engine/Base/Console.h>
#include <Engine/Math/Functions.h>
#include <Engine/Graphics/GfxLibrary.h>
#include <Engine/Graphics/ImageInfo.h>
#include <Engine/Graphics/TextureEffects.h>

#include <Engine/Templates/DynamicArray.h>
#include <Engine/Templates/DynamicArray.cpp>
#include <Engine/Templates/Stock_CtextureData.h>
#include <Engine/Templates/StaticArray.cpp>

#include <Engine/Base/Statistics_internal.h>

//안태훈 수정 시작	//(Add CRenderTexture class for Render to Texture)(0.1)
#include <Engine/Base/MemoryTracking.h>
#include <d3d8.h>

// track texture memory allocated in main heap
#define TRACKTEX_HEAP() TRACKMEM(mem, "Textures (heap)")
// track texture memory allocated in hardware
#define TRACKTEX_HW() TRACKMEM(mem, "Textures (hardware)")

CRenderTexture::CRenderTexture()
{
	rt_pSurface = NULL;
	m_pDepthStencil = NULL;
}

CRenderTexture::~CRenderTexture()
{
	if(rt_pSurface) rt_pSurface->Release();
	if(m_pDepthStencil) m_pDepthStencil->Release();
}

// sehan D3DFORMAT fmt 추가 // sehan end
BOOL CRenderTexture::Init(INDEX width, INDEX height, ULONG flag, D3DFORMAT fmt)
{
	TRACKTEX_HEAP();
	
	// check maximum supported texture dimension
	if( width>MAX_MEX || height>MAX_MEX)
	{
		ASSERTALWAYS("너무 큰 텍스쳐 사이즈임. BY ANT");
		return FALSE;
	}

	// sehan D3DFMT_A8R8G8B8 를 fmt 로 바꿈. // sehan end
	rt_tdTexture.td_ulInternalFormat = fmt;
	
	// determine mip index from mex size and alpha channel presence
	rt_tdTexture.td_iFirstMipLevel = 0;
	rt_tdTexture.td_ulFlags = flag;
	rt_tdTexture.td_ulFlags |= TEX_ALPHACHANNEL;	//alhpa
	rt_tdTexture.td_ulFlags |= TEX_TRANSPARENT;		//alpha가 있으면 항상.
	rt_tdTexture.td_ulFlags |= TEX_32BIT;
	rt_tdTexture.td_ulFlags |= TEX_CONSTANT;
	rt_tdTexture.td_ulFlags &= ~TEX_STATIC;
	rt_tdTexture.td_ulFlags &= ~(TEX_COMPRESS|TEX_COMPRESSALPHA);	//not allowed compress in shadow

	// initialize general TextureData members
	rt_tdTexture.td_ctFrames  = 0;
	rt_tdTexture.td_mexWidth  = width << rt_tdTexture.td_iFirstMipLevel;
	rt_tdTexture.td_mexHeight = height << rt_tdTexture.td_iFirstMipLevel;
	
	// create all mip levels (either bilinear or downsampled)
	rt_tdTexture.td_ctFineMipLevels = 1;
	// get frame size (includes only one mip-map)
	rt_tdTexture.td_slFrameSize = 0;
	
	// allocate small ammount of memory just for Realloc sake
	rt_tdTexture.td_pulFrames = NULL;
	rt_tdTexture.td_ctFrames = 1;

	//IDirect3DTexture8의 생성.
	IDirect3DTexture8 *pTexture = NULL;
	// sehan D3DFMT_A8R8G8B8 를 fmt 로 바꿈. // sehan end
	HRESULT hr = _pGfx->gl_pd3dDevice->CreateTexture(width, height, 1, D3DUSAGE_RENDERTARGET, fmt, D3DPOOL_DEFAULT, &pTexture);
	if(hr == NOERROR)
	{
		rt_tdTexture.td_ulObject = (ULONG)pTexture;	//내가 작성했지만 싫다.  -_-;
		pTexture->GetSurfaceLevel(0, &rt_pSurface);
		//일단 Depth Stencil Surface는 쓰지 않지만 설정해야 된다.
		hr = _pGfx->gl_pd3dDevice->CreateDepthStencilSurface(width, height, D3DFMT_D16, D3DMULTISAMPLE_NONE, &m_pDepthStencil);
		if(hr == NOERROR)
		{
			return TRUE;
		}
	}
	else
	{
		rt_tdTexture.td_ulObject = NULL;
		return FALSE;
	}
	return FALSE;
}

void CRenderTexture::Begin()	// SetRenderTarget current
{
	IDirect3DDevice8 *pDev = _pGfx->gl_pd3dDevice;
	pDev->GetRenderTarget(&m_pOldRenderTarget);
	pDev->GetDepthStencilSurface(&m_pOldDepthStencil);
	pDev->SetRenderTarget(rt_pSurface, m_pDepthStencil);
}

void CRenderTexture::Clear(COLOR colClear, FLOAT fZVal)
{
	_pGfx->gl_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, colClear, fZVal, 0);
}

void CRenderTexture::End()		// SetRenderTarget old
{
	IDirect3DDevice8 *pDev = _pGfx->gl_pd3dDevice;
	pDev->SetRenderTarget(m_pOldRenderTarget, m_pOldDepthStencil);
	if(m_pOldRenderTarget)
	{
		m_pOldRenderTarget->Release();
		m_pOldRenderTarget = NULL;
	}

	if(m_pOldDepthStencil)
	{
		m_pOldDepthStencil->Release();
		m_pOldDepthStencil = NULL;
	}
}
//안태훈 수정 끝	//(Add CRenderTexture class for Render to Texture)(0.1)
