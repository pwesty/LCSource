#include "stdh.h"

#include <Engine/Graphics/GfxLibrary.h>
#include <Engine/Base/MemoryTracking.h>

#include <Engine/Base/Statistics_internal.h>
#include <Engine/Math/Functions.h>

#include <Engine/Base/ListIterator.inl>


// asm shortcuts
#define O offset
#define Q qword ptr
#define D dword ptr
#define W  word ptr
#define B  byte ptr

static CTexParams *_tpCurrent;
static _D3DTEXTUREFILTERTYPE _eLastMipFilter;
static BOOL _abAnisotropySet[GFX_MAXTEXUNITS] = {1234};  // last state of anisotropy

// we need array for Direct3D mipmaps that are lower than N*1 or 1*N
extern ULONG _aulTempRow[4096];


// conversion from OpenGL's RGBA color format to one of D3D color formats
extern void SetInternalFormat_D3D( const D3DFORMAT d3dFormat);
extern void UploadMipmap_D3D( ULONG *pulSrc, LPDIRECT3DTEXTURE8 ptexDst, PIX pixWidth, PIX pixHeight, INDEX iMip);


// unpacks texture filtering from one INDEX to two GLenums (and eventually re-adjust input INDEX)
extern void UnpackFilter_D3D( INDEX iFilter, _D3DTEXTUREFILTERTYPE &eMagFilter,
                             _D3DTEXTUREFILTERTYPE &eMinFilter, _D3DTEXTUREFILTERTYPE &eMipFilter)
{
  switch( iFilter) {
  case 110:  case 10:  eMagFilter=D3DTEXF_POINT;  eMinFilter=D3DTEXF_POINT;  eMipFilter=D3DTEXF_NONE;   break;
  case 111:  case 11:  eMagFilter=D3DTEXF_POINT;  eMinFilter=D3DTEXF_POINT;  eMipFilter=D3DTEXF_POINT;  break;
  case 112:  case 12:  eMagFilter=D3DTEXF_POINT;  eMinFilter=D3DTEXF_POINT;  eMipFilter=D3DTEXF_LINEAR; break;
  case 220:  case 20:  eMagFilter=D3DTEXF_LINEAR; eMinFilter=D3DTEXF_LINEAR; eMipFilter=D3DTEXF_NONE;   break;
  case 221:  case 21:  eMagFilter=D3DTEXF_LINEAR; eMinFilter=D3DTEXF_LINEAR; eMipFilter=D3DTEXF_POINT;  break;
  case 222:  case 22:  eMagFilter=D3DTEXF_LINEAR; eMinFilter=D3DTEXF_LINEAR; eMipFilter=D3DTEXF_LINEAR; break;
  case 120:            eMagFilter=D3DTEXF_POINT;  eMinFilter=D3DTEXF_LINEAR; eMipFilter=D3DTEXF_NONE;   break;
  case 121:            eMagFilter=D3DTEXF_POINT;  eMinFilter=D3DTEXF_LINEAR; eMipFilter=D3DTEXF_POINT;  break;
  case 122:            eMagFilter=D3DTEXF_POINT;  eMinFilter=D3DTEXF_LINEAR; eMipFilter=D3DTEXF_LINEAR; break;
  case 210:            eMagFilter=D3DTEXF_LINEAR; eMinFilter=D3DTEXF_POINT;  eMipFilter=D3DTEXF_NONE;   break;
  case 211:            eMagFilter=D3DTEXF_LINEAR; eMinFilter=D3DTEXF_POINT;  eMipFilter=D3DTEXF_POINT;  break;
  case 212:            eMagFilter=D3DTEXF_LINEAR; eMinFilter=D3DTEXF_POINT;  eMipFilter=D3DTEXF_LINEAR; break;
  default: ASSERTALWAYS( "Illegal Direct3D texture filtering mode."); break;
  }
}



// returns bytes/pixels ratio for texture format
extern INDEX GetFormatPixRatio_D3D( D3DFORMAT d3dFormat)
{
  switch( d3dFormat) {
  case D3DFMT_A8R8G8B8:
  case D3DFMT_X8R8G8B8:
    return 4;
  case D3DFMT_R8G8B8:
    return 3;
  case D3DFMT_R5G6B5:
  case D3DFMT_X1R5G5B5:
  case D3DFMT_A1R5G5B5:
  case D3DFMT_A4R4G4B4:
  case D3DFMT_A8L8:
    return 2;
  // compressed formats and single-channel formats
  default:
    return 1;
  }
}



// change texture filtering mode if needed
extern void MimicTexParams_D3D( CTexParams &tpLocal)
{
  ASSERT( &tpLocal!=NULL);
  extern INDEX GFX_iActiveTexUnit;
  const  INDEX iTU = GFX_iActiveTexUnit; // current texture unit shortcut
  const LPDIRECT3DDEVICE8 pd3dDev = _pGfx->gl_pd3dDevice; 
  HRESULT hr;

  // update texture filtering mode if required
  if( tpLocal.tp_iFilter != _tpGlobal[0].tp_iFilter) tpLocal.tp_iFilter = _tpGlobal[0].tp_iFilter;

  // eventually adjust filtering for textures w/o mipmaps
  const INDEX iMipFilter = _tpGlobal[0].tp_iFilter % 10;  ASSERT( iMipFilter>=0 && iMipFilter<=2);
  if( (!tpLocal.tp_bSingleMipmap != !_tpGlobal[iTU].tp_bSingleMipmap) && iMipFilter!=0)
  { 
    _D3DTEXTUREFILTERTYPE eMipFilter;
    // no mipmaps?
    if( tpLocal.tp_bSingleMipmap) {
#ifndef NDEBUG
      // paranoid!
      hr = pd3dDev->GetTextureStageState( iTU, D3DTSS_MIPFILTER, (ULONG*)&eMipFilter);
      D3D_CHECKERROR(hr);
      ASSERT( eMipFilter==D3DTEXF_POINT || eMipFilter==D3DTEXF_LINEAR);
#endif // set it
      hr = pd3dDev->SetTextureStageState( iTU, D3DTSS_MIPFILTER, D3DTEXF_NONE);
    }
    // yes mipmaps?
    else {
      switch( iMipFilter) {
      case 0: eMipFilter = D3DTEXF_NONE;   break; 
      case 1: eMipFilter = D3DTEXF_POINT;  break; 
      case 2: eMipFilter = D3DTEXF_LINEAR; break; 
      default: ASSERTALWAYS( "Invalid mipmap filtering mode.");
      } // set it
      hr = pd3dDev->SetTextureStageState( iTU, D3DTSS_MIPFILTER, eMipFilter);
    } 
    // check and update mipmap state
    D3D_CHECKERROR(hr);
    _tpGlobal[iTU].tp_bSingleMipmap = tpLocal.tp_bSingleMipmap;
  }

  // update texture anisotropy degree
  extern INDEX gap_bAggressiveAnisotropy;
  if( gap_bAggressiveAnisotropy) gap_bAggressiveAnisotropy = 1;
  if( tpLocal.tp_iAnisotropy != _tpGlobal[0].tp_iAnisotropy) {
    tpLocal.tp_iAnisotropy = _tpGlobal[0].tp_iAnisotropy;
    for( INDEX iUnit=0; iUnit<_pGfx->gl_ctTextureUnits; iUnit++) _abAnisotropySet[iUnit] = 1234;
  }
  // eventually adjust anisotropy for non-mipmaped textures
  const BOOL bSetAnisotropy = tpLocal.tp_iAnisotropy>1 && (!tpLocal.tp_bSingleMipmap || gap_bAggressiveAnisotropy); // this state of anisotropy
  if( _abAnisotropySet[iTU] != bSetAnisotropy) {
    _abAnisotropySet[iTU] = bSetAnisotropy;
    if( bSetAnisotropy) {
      hr = pd3dDev->SetTextureStageState( iTU, D3DTSS_MAXANISOTROPY, tpLocal.tp_iAnisotropy);  D3D_CHECKERROR(hr);
      hr = pd3dDev->SetTextureStageState( iTU, D3DTSS_MAGFILTER, D3DTEXF_ANISOTROPIC);         D3D_CHECKERROR(hr);
      hr = pd3dDev->SetTextureStageState( iTU, D3DTSS_MINFILTER, D3DTEXF_ANISOTROPIC);         D3D_CHECKERROR(hr);
    } else {
      INDEX iMagFilter = _tpGlobal[0].tp_iFilter /100;     ASSERT( iMagFilter>=0 && iMagFilter<=2);
      INDEX iMinFilter = _tpGlobal[0].tp_iFilter /10 %10;  ASSERT( iMinFilter>=1 && iMinFilter<=2);
      if( iMagFilter==0) iMagFilter = iMinFilter; 
      _D3DTEXTUREFILTERTYPE eMagFilter = iMagFilter==1 ? D3DTEXF_POINT : D3DTEXF_LINEAR;
      _D3DTEXTUREFILTERTYPE eMinFilter = iMinFilter==1 ? D3DTEXF_POINT : D3DTEXF_LINEAR;
      hr = pd3dDev->SetTextureStageState( iTU, D3DTSS_MAXANISOTROPY, 1);       D3D_CHECKERROR(hr);
      hr = pd3dDev->SetTextureStageState( iTU, D3DTSS_MAGFILTER, iMagFilter);  D3D_CHECKERROR(hr);
      hr = pd3dDev->SetTextureStageState( iTU, D3DTSS_MINFILTER, iMinFilter);  D3D_CHECKERROR(hr);
    }
  }
  // update texture clamping modes if changed
  if( tpLocal.tp_eWrapU!=_tpGlobal[iTU].tp_eWrapU || tpLocal.tp_eWrapV!=_tpGlobal[iTU].tp_eWrapV) { 
    tpLocal.tp_eWrapU  = _tpGlobal[iTU].tp_eWrapU;
    tpLocal.tp_eWrapV  = _tpGlobal[iTU].tp_eWrapV;
  }

  // keep last texture params (for tex upload and stuff)
  _tpCurrent = &tpLocal;
}




// upload context for current texture to accelerator's memory
// (returns format in which texture was really uploaded)
extern void UploadTexture_D3D( LPDIRECT3DTEXTURE8 *ppd3dTexture, ULONG *pulTexture,
                               PIX pixSizeU, PIX pixSizeV, D3DFORMAT eInternalFormat, BOOL bDiscard)
{
  // safeties
  ASSERT( pulTexture!=NULL);
  ASSERT( pixSizeU>0 && pixSizeV>0);
  _sfStats.StartTimer( CStatForm::STI_BINDTEXTURE);
  const BOOL bNoMipmaps = _tpCurrent->tp_bSingleMipmap;
  HRESULT hr;

  // recreate texture if needed
  if( bDiscard) {
    if( (*ppd3dTexture)!=NULL) {
      MEMTRACK_FREE( (void*)(((ULONG)(*ppd3dTexture))^0x80000000));
      D3DRELEASE( (*ppd3dTexture), TRUE);
    }
    const INDEX iSetupMipmaps = bNoMipmaps ? 1 : 0;
    hr = _pGfx->gl_pd3dDevice->CreateTexture( pixSizeU, pixSizeV, iSetupMipmaps, 0, eInternalFormat, D3DPOOL_MANAGED, ppd3dTexture);
    D3D_CHECKERROR(hr);
  }
  // D3D texture must be valid now
  LPDIRECT3DTEXTURE8 pd3dTex = (*ppd3dTexture);
  ASSERT( pd3dTex!=NULL);

  // prepare routine for conversion
  SetInternalFormat_D3D(eInternalFormat);
  
  // upload each mipmap
  INDEX iMip=0;
  PIX pixOffset=0;
  while( pixSizeU>0 && pixSizeV>0)
  { 
    // check that memory is readable and upload one mipmap
    ASSERT( pulTexture[pixOffset +pixSizeU*pixSizeV -1] != 0xDEADBEEF);
    UploadMipmap_D3D( pulTexture+pixOffset, pd3dTex, pixSizeU, pixSizeV, iMip);
    // advance to next mip-map
    pixOffset += pixSizeU*pixSizeV;
    pixSizeU >>=1;
    pixSizeV >>=1;
    iMip++;
    // end here if there is only one mip-map to upload
    if( bNoMipmaps) break;
  }

  // see if we need to generate and upload additional mipmaps (those under 1*N or N*1)
  if( !bNoMipmaps && pixSizeU!=pixSizeV)
  { // prepare variables
    PIX pixSize = Max(pixSizeU,pixSizeV);
    ASSERT(pixSize<=2048);
    ULONG *pulSrc = pulTexture+pixOffset-pixSize*2;
    ULONG *pulDst = _aulTempRow;
    // loop thru mipmaps
    while( pixSizeU>0 || pixSizeV>0)
    { // make next mipmap
      if( pixSizeU==0) pixSizeU=1;
      if( pixSizeV==0) pixSizeV=1;
      pixSize = pixSizeU*pixSizeV;
      MakeSubMipmap( pulSrc, pulDst, pixSize);
      // upload mipmap and advance
      UploadMipmap_D3D( pulDst, pd3dTex, pixSizeU, pixSizeV, iMip);
      pulSrc     = pulDst;
      pulDst    += pixSize;
      pixOffset += pixSize;
      pixSizeU >>=1;
      pixSizeV >>=1;
      iMip++;
    }
  }

#if MEMORY_TRACKING
  // signal to memory tracker  
  const SLONG slSize = pixOffset * GetFormatPixRatio_D3D(eInternalFormat);
  const ULONG ulObject = (ULONG)pd3dTex;
  MEMTRACK_ALLOC( (void*)(ulObject^0x80000000), (slSize+4095)/4096*4096, slSize);
#endif

  // all done
  _sfStats.IncrementCounter( CStatForm::SCI_TEXTUREUPLOADS, 1);
  _sfStats.IncrementCounter( CStatForm::SCI_TEXTUREUPLOADBYTES, pixOffset*4);
  _sfStats.StopTimer( CStatForm::STI_BINDTEXTURE);
}



// uploads compressed frames
extern BOOL UploadCompressedTexture_D3D( LPDIRECT3DTEXTURE8 *ppd3dTexture, UBYTE *pubTexture, PIX pixSizeU,
                                         PIX pixSizeV, SLONG slSize, D3DFORMAT eInternalFormat)
{
  // safeties
  ASSERT( pubTexture!=NULL);
  ASSERT( pixSizeU>0 && pixSizeV>0 && slSize>0);
  ASSERT( eInternalFormat==D3DFMT_DXT1 || eInternalFormat==D3DFMT_DXT3 || eInternalFormat==D3DFMT_DXT5);
  _sfStats.StartTimer( CStatForm::STI_BINDTEXTURE);
  const BOOL bNoMipmaps = _tpCurrent->tp_bSingleMipmap;
  const INDEX ctMaxMips = bNoMipmaps ? 1 : 15;
  HRESULT hr;

  // recreate texture
  if( (*ppd3dTexture)!=NULL) {
    MEMTRACK_FREE( (void*)(((ULONG)(*ppd3dTexture))^0x80000000));
    D3DRELEASE( (*ppd3dTexture), TRUE);
  }
  const INDEX iSetupMipmaps = bNoMipmaps ? 1 : 0;
  hr = _pGfx->gl_pd3dDevice->CreateTexture( pixSizeU, pixSizeV, iSetupMipmaps, 0, eInternalFormat, D3DPOOL_MANAGED, ppd3dTexture);
  D3D_CHECKERROR(hr);
  // D3D texture must be valid now
  LPDIRECT3DTEXTURE8 pd3dTex = (*ppd3dTexture);
  ASSERT( pd3dTex!=NULL);

  // upload each mipmap
  D3DLOCKED_RECT  rectLocked;
  D3DSURFACE_DESC d3dSurfDesc;
  INDEX iMip = 0;
  SLONG slUploadSize = 0;
  while( slSize>slUploadSize && iMip<ctMaxMips) {
    // readout mip-size
    const SLONG slMipSize = *(SLONG*)pubTexture;
    pubTexture += 4;
    // fetch texture
    hr = pd3dTex->GetLevelDesc( iMip, &d3dSurfDesc);         D3D_CHECKERROR(hr);
    hr = pd3dTex->LockRect( iMip, &rectLocked, NULL, NONE);  D3D_CHECKERROR(hr);
    ASSERT( d3dSurfDesc.Size==slMipSize);
    if( d3dSurfDesc.Size!=slMipSize) { // saved mip size and texture mip size must match!
      _sfStats.StopTimer( CStatForm::STI_BINDTEXTURE);
      return FALSE;
    } // copy mip to texture
    memcpy( rectLocked.pBits, pubTexture, slMipSize);
    hr = pd3dTex->UnlockRect(iMip);  D3D_CHECKERROR(hr);
    // advance to next mip
    pubTexture   += slMipSize;
    slUploadSize += slMipSize+4;
    iMip++;
  }

#if MEMORY_TRACKING
  // signal to memory tracker  
  const ULONG ulObject = (ULONG)pd3dTex;
  MEMTRACK_ALLOC( (void*)(ulObject^0x80000000), (slSize+4095)/4096*4096, slSize);
#endif

  // all done
  _sfStats.IncrementCounter( CStatForm::SCI_TEXTUREUPLOADS, 1);
  _sfStats.IncrementCounter( CStatForm::SCI_TEXTUREUPLOADBYTES, slSize);
  _sfStats.StopTimer( CStatForm::STI_BINDTEXTURE);
  return TRUE;
}



// returns bytes/pixels ratio for uploaded texture
extern INDEX GetTexturePixRatio_D3D( LPDIRECT3DTEXTURE8 pd3dTexture)
{
  D3DSURFACE_DESC d3dSurfDesc;
  HRESULT hr = pd3dTexture->GetLevelDesc( 0, &d3dSurfDesc);
  D3D_CHECKERROR(hr);
  return GetFormatPixRatio_D3D( d3dSurfDesc.Format);
}


// return allowed dithering method
extern INDEX AdjustDitheringType_D3D( D3DFORMAT eFormat, INDEX iDitheringType)
{
  switch( eFormat) {
  // these formats don't need dithering
  case D3DFMT_A8R8G8B8:
  case D3DFMT_X8R8G8B8:
  case D3DFMT_L8:
  case D3DFMT_A8L8:
    return NONE;
  // these formats need reduced dithering
  case D3DFMT_R5G6B5:
  case D3DFMT_X1R5G5B5:
  case D3DFMT_A1R5G5B5:
    if( iDitheringType>7) return iDitheringType-3;
  // other formats need dithering as it is
  default:
    return iDitheringType;
  }
}
