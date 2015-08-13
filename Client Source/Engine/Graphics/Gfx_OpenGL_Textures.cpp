#include "stdh.h"

#include <Engine/Graphics/GfxLibrary.h>
#include <Engine/Base/MemoryTracking.h>

#include <Engine/Base/Statistics_internal.h>
#include <Engine/Math/Functions.h>
#include <Engine/Graphics/Color.h>
#include <Engine/Graphics/Texture.h>

#include <Engine/Base/ListIterator.inl>

// asm shortcuts
#define O offset
#define Q qword ptr
#define D dword ptr
#define W  word ptr
#define B  byte ptr

static CTexParams *_tpCurrent;
extern INDEX GFX_iActiveTexUnit;

// we need array for OpenGL mipmaps that are lower than N*1 or 1*N
extern ULONG _aulTempRow[4096];



// unpacks texture filering from one INDEX to two GLenums (and eventually re-adjust input INDEX)
static void UnpackFilter_OGL( INDEX iFilter, GLenum &eMagFilter, GLenum &eMinFilter)
{
  switch( iFilter) {
  case 110:  case 10:  eMagFilter=GL_NEAREST;  eMinFilter=GL_NEAREST;                 break;
  case 111:  case 11:  eMagFilter=GL_NEAREST;  eMinFilter=GL_NEAREST_MIPMAP_NEAREST;  break;
  case 112:  case 12:  eMagFilter=GL_NEAREST;  eMinFilter=GL_NEAREST_MIPMAP_LINEAR;   break;
  case 220:  case 20:  eMagFilter=GL_LINEAR;   eMinFilter=GL_LINEAR;                  break;
  case 221:  case 21:  eMagFilter=GL_LINEAR;   eMinFilter=GL_LINEAR_MIPMAP_NEAREST;   break;
  case 222:  case 22:  eMagFilter=GL_LINEAR;   eMinFilter=GL_LINEAR_MIPMAP_LINEAR;    break;
  case 120:            eMagFilter=GL_NEAREST;  eMinFilter=GL_LINEAR;                  break;
  case 121:            eMagFilter=GL_NEAREST;  eMinFilter=GL_LINEAR_MIPMAP_NEAREST;   break;
  case 122:            eMagFilter=GL_NEAREST;  eMinFilter=GL_LINEAR_MIPMAP_LINEAR;    break;
  case 210:            eMagFilter=GL_LINEAR;   eMinFilter=GL_NEAREST;                 break;
  case 211:            eMagFilter=GL_LINEAR;   eMinFilter=GL_NEAREST_MIPMAP_NEAREST;  break;
  case 212:            eMagFilter=GL_LINEAR;   eMinFilter=GL_NEAREST_MIPMAP_LINEAR;   break;
  default: ASSERTALWAYS( "Illegal OpenGL texture filtering mode."); break;
  }
}



// returns bytes/pixels ratio for uploaded texture
extern INDEX GetFormatPixRatio_OGL( GLenum eFormat)
{
  switch( eFormat) {
  case GL_RGBA:
  case GL_RGBA8:
    return 4;
  case GL_RGB:
  case GL_RGB8:
    return 3;
  case GL_RGB5:
  case GL_RGB5_A1:
  case GL_RGB4:
  case GL_RGBA4:
  case GL_LUMINANCE_ALPHA:
  case GL_LUMINANCE8_ALPHA8:
    return 2;
  // compressed formats and single-channel formats
  default:
    return 1;
  }
}



// change texture filtering mode if needed
extern void MimicTexParams_OGL( CTexParams &tpLocal)
{
  ASSERT( &tpLocal!=NULL);

  // set texture filtering mode if required
  if( tpLocal.tp_iFilter != _tpGlobal[0].tp_iFilter)
  { // update OpenGL texture filters
    GLenum eMagFilter, eMinFilter;
    UnpackFilter_OGL( _tpGlobal[0].tp_iFilter, eMagFilter, eMinFilter);
    // adjust minimize filter in case of a single mipmap
    if( tpLocal.tp_bSingleMipmap) {
           if( eMinFilter==GL_NEAREST_MIPMAP_NEAREST || eMinFilter==GL_NEAREST_MIPMAP_LINEAR) eMinFilter = GL_NEAREST;
      else if( eMinFilter==GL_LINEAR_MIPMAP_NEAREST  || eMinFilter==GL_LINEAR_MIPMAP_LINEAR)  eMinFilter = GL_LINEAR;
    }
    // update texture filter
    pglTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, eMagFilter);
    pglTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, eMinFilter);
    tpLocal.tp_iFilter = _tpGlobal[0].tp_iFilter;
    OGL_CHECKERROR;
  }

  // set texture anisotropy degree if required and supported
  extern INDEX gap_bAggressiveAnisotropy;
  static INDEX iLastAggressiveAnisotropy = 1234;
  if( tpLocal.tp_iAnisotropy    != _tpGlobal[0].tp_iAnisotropy
   || iLastAggressiveAnisotropy != gap_bAggressiveAnisotropy) { 
    // update anisotropy degree
    if( gap_bAggressiveAnisotropy) gap_bAggressiveAnisotropy = 1;
    tpLocal.tp_iAnisotropy    = _tpGlobal[0].tp_iAnisotropy;
    iLastAggressiveAnisotropy = gap_bAggressiveAnisotropy;
    // set anisotropy degree (only if allowed)
    if( _pGfx->gl_iMaxTextureAnisotropy>=2) {
      INDEX iAnisoDegree = tpLocal.tp_iAnisotropy;
      if( tpLocal.tp_bSingleMipmap && !gap_bAggressiveAnisotropy) iAnisoDegree = 1;
      pglTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, iAnisoDegree);
    }
  }

  // set texture clamping modes if changed
  if( tpLocal.tp_eWrapU!=_tpGlobal[GFX_iActiveTexUnit].tp_eWrapU
   || tpLocal.tp_eWrapV!=_tpGlobal[GFX_iActiveTexUnit].tp_eWrapV)
  { // prepare temp vars
    GLuint eWrapU = _tpGlobal[GFX_iActiveTexUnit].tp_eWrapU==GFX_REPEAT ? GL_REPEAT : GL_CLAMP;
    GLuint eWrapV = _tpGlobal[GFX_iActiveTexUnit].tp_eWrapV==GFX_REPEAT ? GL_REPEAT : GL_CLAMP;
    // eventually re-adjust clamping params in case of clamp_to_edge extension
    if( _pGfx->gl_ulFlags&GLF_EXT_EDGECLAMP) {
      if( eWrapU == GL_CLAMP) eWrapU = GL_CLAMP_TO_EDGE;
      if( eWrapV == GL_CLAMP) eWrapV = GL_CLAMP_TO_EDGE;
    } 
    // set clamping params and update local texture clamping modes
    pglTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, eWrapU);
    pglTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, eWrapV);
    tpLocal.tp_eWrapU = _tpGlobal[GFX_iActiveTexUnit].tp_eWrapU;
    tpLocal.tp_eWrapV = _tpGlobal[GFX_iActiveTexUnit].tp_eWrapV;
    OGL_CHECKERROR;
  }

  // keep last texture params (for tex upload and stuff)
  _tpCurrent = &tpLocal;
}



// upload context for current texture to accelerator's memory
// (returns format in which texture was really uploaded)
extern void UploadTexture_OGL( ULONG *pulTexture, PIX pixSizeU, PIX pixSizeV,
                               GLenum eInternalFormat, BOOL bUseSubImage)
{
  // safeties
  ASSERT( pulTexture!=NULL);
  ASSERT( pixSizeU>0 && pixSizeV>0);
  _sfStats.StartTimer( CStatForm::STI_BINDTEXTURE);
  const BOOL bNoMipmaps = _tpCurrent->tp_bSingleMipmap;

  // upload each original mip-map
  INDEX iMip=0;
  PIX pixOffset=0;
  while( pixSizeU>0 && pixSizeV>0)
  { 
    // check that memory is readable
    ASSERT( pulTexture[pixOffset +pixSizeU*pixSizeV -1] != 0xDEADBEEF);
    // upload mipmap as fast as possible
    if( bUseSubImage) {
      pglTexSubImage2D( GL_TEXTURE_2D, iMip, 0, 0, pixSizeU, pixSizeV,
                        GL_RGBA, GL_UNSIGNED_BYTE, pulTexture+pixOffset);
    } else {
      pglTexImage2D( GL_TEXTURE_2D, iMip, eInternalFormat, pixSizeU, pixSizeV, 0,
                     GL_RGBA, GL_UNSIGNED_BYTE, pulTexture+pixOffset);
    } OGL_CHECKERROR;
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
    ASSERT( pixSize<=2048);
    ULONG *pulSrc = pulTexture+pixOffset-pixSize*2;
    ULONG *pulDst = _aulTempRow;
    // loop thru mipmaps
    while( pixSizeU>0 || pixSizeV>0)
    { // make next mipmap
      if( pixSizeU==0) pixSizeU=1;
      if( pixSizeV==0) pixSizeV=1;
      pixSize = pixSizeU*pixSizeV;
      MakeSubMipmap( pulSrc, pulDst, pixSize);
      // upload mipmap
      if( bUseSubImage) {
        pglTexSubImage2D( GL_TEXTURE_2D, iMip, 0, 0, pixSizeU, pixSizeV, GL_RGBA, GL_UNSIGNED_BYTE, pulDst);
      } else {
        pglTexImage2D( GL_TEXTURE_2D, iMip, eInternalFormat, pixSizeU, pixSizeV, 0, GL_RGBA, GL_UNSIGNED_BYTE, pulDst);
      } OGL_CHECKERROR;
      // advance to next mip-map
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
  extern ULONG *GFX_apulCurrentTexture[GFX_MAXTEXUNITS];
  const SLONG slSize = pixOffset * GetFormatPixRatio_OGL(eInternalFormat);
  MEMTRACK_ALLOC( (void*)((ULONG)(GFX_apulCurrentTexture[GFX_iActiveTexUnit])^0x80000000), (slSize+4095)/4096*4096, slSize);
#endif

  // all done
  _sfStats.IncrementCounter( CStatForm::SCI_TEXTUREUPLOADS, 1);
  _sfStats.IncrementCounter( CStatForm::SCI_TEXTUREUPLOADBYTES, pixOffset*4);
  _sfStats.StopTimer( CStatForm::STI_BINDTEXTURE);
}



// uploads compressed frames
extern BOOL UploadCompressedTexture_OGL( UBYTE *pubTexture, PIX pixSizeU, PIX pixSizeV,
                                         SLONG slSize, GLenum eInternalFormat)
{
  // safeties
  ASSERT( pubTexture!=NULL);
  ASSERT( pixSizeU>0 && pixSizeV>0 && slSize>0);
  _sfStats.StartTimer( CStatForm::STI_BINDTEXTURE);
  const BOOL bNoMipmaps = _tpCurrent->tp_bSingleMipmap;

  // must adjust internal format to match S3TC
  if( eInternalFormat==TS.ts_tfRGBc)   eInternalFormat = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
  if( eInternalFormat==TS.ts_tfRGBcA)  eInternalFormat = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
  if( eInternalFormat==TS.ts_tfRGBcAc) eInternalFormat = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;

  // upload each mipmap
  INDEX iMip = 0;
  SLONG slRetSize, slUploadSize = 0;
  while( slSize>slUploadSize) {
    // readout mip-size
    const SLONG slMipSize = *(SLONG*)pubTexture;
    pubTexture += 4;
    pglCompressedTexImageARB(  GL_TEXTURE_2D, iMip, eInternalFormat, pixSizeU, pixSizeV, 0, slMipSize, pubTexture);
    pglGetTexLevelParameteriv( GL_TEXTURE_2D, iMip, GL_TEXTURE_COMPRESSED_IMAGE_SIZE_ARB, (GLint*)&slRetSize); 
    OGL_CHECKERROR;
    ASSERT( slRetSize==slMipSize);
    if( slRetSize!=slMipSize) { // saved mip size and texture mip size must match!
      _sfStats.StopTimer( CStatForm::STI_BINDTEXTURE);
      return FALSE;
    } // advance to next mip
    pubTexture   += slMipSize;
    slUploadSize += slMipSize+4;
    pixSizeU = ClampDn( pixSizeU>>1, 1L);
    pixSizeV = ClampDn( pixSizeV>>1, 1L);
    iMip++;
  }

  // signal to memory tracker  
  extern ULONG *GFX_apulCurrentTexture[GFX_MAXTEXUNITS];
  MEMTRACK_ALLOC( (void*)((ULONG)(GFX_apulCurrentTexture[GFX_iActiveTexUnit])^0x80000000), (slSize+4095)/4096*4096, slSize);

  // all done
  _sfStats.IncrementCounter( CStatForm::SCI_TEXTUREUPLOADS, 1);
  _sfStats.IncrementCounter( CStatForm::SCI_TEXTUREUPLOADBYTES, slSize);
  _sfStats.StopTimer( CStatForm::STI_BINDTEXTURE);
  return TRUE;
}



// returns bytes/pixels ratio for uploaded texture
extern INDEX GetTexturePixRatio_OGL( GLuint uiBindNo)
{
  GLenum eInternalFormat;
  pglBindTexture( GL_TEXTURE_2D, uiBindNo);
  pglGetTexLevelParameteriv( GL_TEXTURE_2D, 0, GL_TEXTURE_INTERNAL_FORMAT, (GLint*)&eInternalFormat);
  OGL_CHECKERROR;
  return GetFormatPixRatio_OGL( eInternalFormat);
}


// return allowed dithering method
extern INDEX AdjustDitheringType_OGL( GLenum eFormat, INDEX iDitheringType)
{
  switch( eFormat) {
  // these formats don't need dithering
  case GL_RGB8:
  case GL_RGBA8:
  case GL_LUMINANCE8:
  case GL_LUMINANCE8_ALPHA8:
    return NONE;
  // these formats need reduced dithering
  case GL_RGB5:
  case GL_RGB5_A1:
    if( iDitheringType>7) return iDitheringType-3;
  // other formats need dithering as it is
  default:
    return iDitheringType;
  }
}

