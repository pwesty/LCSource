#include "stdh.h"

#include <Engine/Base/Console.h>
#include <Engine/Base/Timer.h>
#include <Engine/Base/Priority.inl>
#include <Engine/Graphics/GfxLibrary.h>

#include <Engine/Graphics/ViewPort.h>
#include <Engine/Graphics/DrawPort.h>

#include <Engine/Templates/StaticArray.cpp>
#include <Engine/Templates/StaticStackArray.cpp>


static CViewPort *_pvp = NULL;
static CDrawPort *_pdp = NULL;
static PIX _pixSizeI;
static PIX _pixSizeJ;
static CTimerValue _tv;
static BOOL _bBlend = FALSE;
static BOOL _bVisible = FALSE;
static BOOL _bDepth = FALSE;
static INDEX _iTexture = 0; // how many texture units to use
static UBYTE *_pubTexture;
static ULONG _ulTexObject;
static ULONG _ulTexFormat;
static BOOL _bSubImage = FALSE;
static INDEX _ctR = 100;
static INDEX _ctC = 100;
static CTexParams _tpLocal;


static CStaticStackArray<GFXVertex>    _avtx;
static CStaticStackArray<GFXTexCoord>  _atex;
static CStaticStackArray<GFXColor>     _acol;
static CStaticStackArray<UWORD> _auwElements;



BOOL _bStarted = FALSE;
static __forceinline void StartTimer(void)
{
  ASSERT(!_bStarted);
  _tv = _pTimer->GetHighPrecisionTimer();
  _bStarted = TRUE;
}


static __forceinline DOUBLE StopTimer(void)
{
  ASSERT(_bStarted);
  _bStarted = FALSE;
  return (_pTimer->GetHighPrecisionTimer()-_tv).GetSeconds();
}


// fill rate benchmark
static DOUBLE FillRatePass(INDEX ct)
{
  _pdp->SetAsCurrent();
  _pdp->Fill(C_GRAY|255);
  _pdp->FillZBuffer(1.0f);

  StartTimer();

  GFXVertex avtx[4];
  avtx[0].x = 0;          avtx[0].y = 0;          avtx[0].z = 0.5f;  
  avtx[1].x = 0;          avtx[1].y = _pixSizeJ;  avtx[1].z = 0.5f;  
  avtx[2].x = _pixSizeI;  avtx[2].y = _pixSizeJ;  avtx[2].z = 0.5f;  
  avtx[3].x = _pixSizeI;  avtx[3].y = 0;          avtx[3].z = 0.5f;  
  GFXTexCoord atex[4] = { {0,0}, {0,1}, {1,1}, {1,0} };
  GFXColor    acol[4] = { 0xFF0000FF, 0xFF00FF00, 0xFFFF0000, 0xFFFF00FF };
  UWORD       aidx[6] = { 0,1,2, 0,2,3};
  gfxSetVertexArray( &avtx[0], 4);
  gfxSetColorArray(  &acol[0]);

  if(_iTexture) {
    gfxEnableTexture();
    gfxSetTexCoordArray( &_atex[0], FALSE);
    for( INDEX iTex=2; iTex<=_iTexture; iTex++) {
      gfxSetTextureUnit(iTex-1);
      gfxEnableTexture();
      gfxSetTexture( _ulTexObject, _tpLocal);
      gfxSetTexCoordArray( &atex[0], FALSE);
    } // set 0 tex unit as default
    if(_iTexture>1) gfxSetTextureUnit(0);
  } else {
    gfxDisableTexture();
  }

  if(_bBlend) {
    gfxEnableBlend();
    if(_iTexture) {
      gfxBlendFunc( GFX_SRC_ALPHA, GFX_INV_SRC_ALPHA); 
    } else {
      gfxBlendFunc( GFX_ONE, GFX_ONE);
    }
  } else {
    gfxDisableBlend();
  }

  if(_bDepth) {
    gfxEnableDepthTest();
    gfxEnableDepthWrite();
  } else {
    gfxDisableDepthTest();
    gfxDisableDepthWrite();
  }
  gfxDisableAlphaTest();

  // draw!
  for( INDEX i=0; i<ct; i++) gfxDrawElements( 6, &aidx[0]);

  // disable multi-texture units
  if(_iTexture>1) {
    for( INDEX iTex=2; iTex<=_iTexture; iTex++) {
      gfxSetTextureUnit(iTex-1);
      gfxDisableTexture();
    } // set 0 tex unit as default
    gfxSetTextureUnit(0);
  }

  // done
  gfxFinish(TRUE);
  _pvp->SwapBuffers();
  return StopTimer();
}


CTString FillRateString(void)
{
  CTString str;
  CTString strTex = "no texture";
       if( _iTexture==0) strTex = "no texture";
  else if( _iTexture==1) strTex = "1 texture ";
  else if( _iTexture>=2) strTex.PrintF( "%d textures", _iTexture);
  str.PrintF( "%s, %s, %s ", strTex,
             _bBlend ? "blending"  : "no blending", 
             _bDepth ? "z-buffer:" : "no z-buffer:");
  return str;
}


static DOUBLE FillRate(void)
{
  DOUBLE dDelta = 0;
  for( INDEX i=0; i<10; i++) {
    dDelta += FillRatePass(3) - FillRatePass(2);
  }
  dDelta /= 10.0;
  return (_pixSizeI*_pixSizeJ)/dDelta;
}


static void InitTexture(void)
{
  const SLONG dwSize = 256*256 *4 *4/3 +16;
  _pubTexture = (UBYTE*)AllocMemory(dwSize);
  for( INDEX i=0; i<256; i++) {
    for( INDEX j=0; j<256; j++) {
      _pubTexture[(j*256+i)*4+0] = i;  
      _pubTexture[(j*256+i)*4+1] = j;  
      _pubTexture[(j*256+i)*4+2] = i+j;
      _pubTexture[(j*256+i)*4+3] = i-j;
    }
  }
  MakeMipmaps( (ULONG*)_pubTexture, 256,256);
  _tpLocal.tp_bSingleMipmap = FALSE;
  gfxGenerateTexture( _ulTexObject);
  gfxSetTexture( _ulTexObject, _tpLocal);
}


static void EndTexture(void)
{
  gfxDeleteTexture( _ulTexObject);
  FreeMemory(_pubTexture);
  _pubTexture = NULL;
}


static void InitTris(void)
{
  INDEX iR, iC;
  INDEX ctVx = _ctR*_ctC;
  _avtx.Push(ctVx);
  _atex.Push(ctVx);
  _acol.Push(ctVx);
  for( iR=0; iR<_ctR; iR++) {
    for( iC=0; iC<_ctC; iC++) {
      INDEX ivx = iR*_ctC+iC;
      _avtx[ivx].x =  FLOAT(iC) / _ctC*4 -2.0f;
      _avtx[ivx].y = -FLOAT(iR) / _ctR*4 +2.0f;
      _avtx[ivx].z = -1.0f;
      _atex[ivx].s = (iC+iR) % 2;
      _atex[ivx].t = (iR)    % 2;
      _acol[ivx].abgr = 0xFFFFFFFF;
    }
  }
  INDEX ctTri = (_ctR-1)*(_ctC-1)*2;
  _auwElements.Push(ctTri*3);
  for( iR=0; iR<_ctR-1; iR++) {
    for( iC=0; iC<_ctC-1; iC++) {
      INDEX iq = iR*(_ctC-1)+iC;
      _auwElements[iq*6+0] = (iR+1) * _ctC + (iC+0);
      _auwElements[iq*6+1] = (iR+1) * _ctC + (iC+1);
      _auwElements[iq*6+2] = (iR+0) * _ctC + (iC+0);
      _auwElements[iq*6+3] = (iR+0) * _ctC + (iC+0);
      _auwElements[iq*6+4] = (iR+1) * _ctC + (iC+1);
      _auwElements[iq*6+5] = (iR+0) * _ctC + (iC+1);
    }
  }
}


static void EndTris(void)
{
  _avtx.Clear();
  _atex.Clear();
  _acol.Clear();
  _auwElements.Clear();
}



static DOUBLE TrisTroughputPass(INDEX ct)
{
  _pdp->SetAsCurrent();

  StartTimer();

  gfxSetFrustum( -0.5f, +0.5f, -0.5f, +0.5f, 0.5f, 2.0f);
  gfxSetViewMatrix(NULL);
  gfxCullFace(GFX_NONE);

  _pdp->Fill(C_GRAY|255);
  _pdp->FillZBuffer(1.0f);

  if(_bBlend) {
    gfxEnableBlend();
    gfxBlendFunc( GFX_ONE, GFX_ONE);
  } else {
    gfxDisableBlend();
  }

  if(_bDepth) {
    gfxEnableDepthTest();
    gfxEnableDepthWrite();
  } else {
    gfxDisableDepthTest();
    gfxDisableDepthWrite();
  }
  gfxDisableAlphaTest();

  gfxSetVertexArray( &_avtx[0], _avtx.Count());
  gfxLockArrays();
  gfxSetColorArray( &_acol[0]);

  if(_iTexture) {
    gfxEnableTexture();
    gfxSetTexCoordArray( &_atex[0], FALSE);
    for( INDEX iTex=2; iTex<=_iTexture; iTex++) {
      gfxSetTextureUnit(iTex-1);
      gfxEnableTexture();
      gfxSetTexture( _ulTexObject, _tpLocal);
      gfxSetTexCoordArray( &_atex[0], FALSE);
    } // set 0 tex unit as default
    if(_iTexture>1) gfxSetTextureUnit(0);
  } else {
    gfxDisableTexture();
  }

  // draw!
  for( INDEX i=0; i<ct; i++) gfxDrawElements( _auwElements.Count(), &_auwElements[0]);

  gfxUnlockArrays();

  // disable multi-texture units
  if(_iTexture>1) {
    for( INDEX iTex=2; iTex<=_iTexture; iTex++) {
      gfxSetTextureUnit(iTex-1);
      gfxDisableTexture();
    } // set 0 tex unit as default
    gfxSetTextureUnit(0);
  }

  // done
  gfxFinish(TRUE);
  _pvp->SwapBuffers();
  return StopTimer();
}


static DOUBLE TrisTroughput(void)
{
  DOUBLE dDelta = 0;
  for( INDEX i=0; i<10; i++) {
    dDelta += TrisTroughputPass(3) - TrisTroughputPass(2);
  }
  dDelta /= 10.0;
  return ((_ctR-1)*(_ctC-1)*2)/dDelta;
}



static DOUBLE TextureUpload(void)
{
  StartTimer();
  gfxUploadTexture( (ULONG*)_pubTexture, 256, 256, _ulTexFormat, _bSubImage);
  const SLONG slTotal = 256*256*4 *4/3;
  return slTotal/StopTimer();
}



static DOUBLE _dX;
static DOUBLE _dD;
static void RunTest(DOUBLE (*pTest)(void), INDEX ct)
{
  DOUBLE dSum  = 0;
  DOUBLE dSum2 = 0;
  for(INDEX i=0; i<(ct+5); i++) {
    DOUBLE d = pTest();
    // must ignore 1st couple of passes due to API queue
    if( i>4) dSum  += d;
    if( i>4) dSum2 += d*d;
  }
  _dX = dSum/ct;
  _dD = Sqrt((dSum2-2*dSum*_dX+ct*_dX*_dX)/(ct-1));
}


/* Benchmark current driver. */
void CGfxLibrary::Benchmark(CViewPort *pvp, CDrawPort *pdp)
{
  // remember drawport/viewport
  _pdp = pdp;
  _pvp = pvp;
  _pixSizeI = pdp->GetWidth();
  _pixSizeJ = pdp->GetHeight();

  // determine maximum number of texture units usable for this test
  const INDEX ctTextureUnits = ClampUp( Min( _pGfx->gl_ctRealTextureUnits, 1+_pGfx->gl_ctMaxStreams-GFX_MINSTREAMS), GFX_MAXTEXUNITS);

  // boost thread priority to gain more accurate results
  CSetPriority sp(REALTIME_PRIORITY_CLASS, THREAD_PRIORITY_TIME_CRITICAL);

  // off to the testing...
  CTString strAPI = "";
       if( _pGfx->gl_eCurrentAPI==GAT_OGL) strAPI = "OpenGL";
  else if( _pGfx->gl_eCurrentAPI==GAT_D3D) strAPI = "Direct3D";
  CPrintF("=====================================\n");
  CPrintF("%s performance testing ...\n", strAPI);

  InitTexture();
  InitTris();

  CPrintF("\n--- Texture upload\n");

  // texture uploading
  _ulTexFormat = TS.ts_tfRGBA8;
  _bSubImage = FALSE;
  RunTest(TextureUpload, 9);
  CPrintF("RGBA8  full: %5.01f +- %5.02f Mtex/s;" , _dX/1000/1000, _dD/1000/1000);
  _bSubImage = TRUE;
  RunTest(TextureUpload, 9);
  CPrintF(     "   sub: %5.01f +- %5.02f Mtex/s\n", _dX/1000/1000, _dD/1000/1000);

  _ulTexFormat = TS.ts_tfRGB8;
  _bSubImage = FALSE;
  RunTest(TextureUpload, 9);
  CPrintF("RGB8   full: %5.01f +- %5.02f Mtex/s;" , _dX/1000/1000, _dD/1000/1000);
  _bSubImage = TRUE;
  RunTest(TextureUpload, 9);
  CPrintF(     "   sub: %5.01f +- %5.02f Mtex/s\n", _dX/1000/1000, _dD/1000/1000);

  _ulTexFormat = TS.ts_tfRGBA4;
  _bSubImage = FALSE;
  RunTest(TextureUpload, 9);
  CPrintF("RGBA4  full: %5.01f +- %5.02f Mtex/s;" , _dX/1000/1000, _dD/1000/1000);
  _bSubImage = TRUE;
  RunTest(TextureUpload, 9);
  CPrintF(     "   sub: %5.01f +- %5.02f Mtex/s\n", _dX/1000/1000, _dD/1000/1000);

  _ulTexFormat = TS.ts_tfRGB5A1;
  _bSubImage = FALSE;
  RunTest(TextureUpload, 9);
  CPrintF("RGB5A1 full: %5.01f +- %5.02f Mtex/s;" , _dX/1000/1000, _dD/1000/1000);
  _bSubImage = TRUE;
  RunTest(TextureUpload, 9);
  CPrintF(     "   sub: %5.01f +- %5.02f Mtex/s\n", _dX/1000/1000, _dD/1000/1000);

  _ulTexFormat = TS.ts_tfRGB5;
  _bSubImage = FALSE;
  RunTest(TextureUpload, 9);
  CPrintF("RGB5   full: %5.01f +- %5.02f Mtex/s;" , _dX/1000/1000, _dD/1000/1000);
  _bSubImage = TRUE;
  RunTest(TextureUpload, 9);
  CPrintF(     "   sub: %5.01f +- %5.02f Mtex/s\n", _dX/1000/1000, _dD/1000/1000);

  _ulTexFormat = TS.ts_tfLA8;
  _bSubImage = FALSE;
  RunTest(TextureUpload, 9);
  CPrintF("LA8    full: %5.01f +- %5.02f Mtex/s;" , _dX/1000/1000, _dD/1000/1000);
  _bSubImage = TRUE;
  RunTest(TextureUpload, 9);
  CPrintF(     "   sub: %5.01f +- %5.02f Mtex/s\n", _dX/1000/1000, _dD/1000/1000);

  _ulTexFormat = TS.ts_tfL8;
  _bSubImage = FALSE;
  RunTest(TextureUpload, 9);
  CPrintF("L8     full: %5.01f +- %5.02f Mtex/s;" , _dX/1000/1000, _dD/1000/1000);
  _bSubImage = TRUE;
  RunTest(TextureUpload, 9);
  CPrintF(     "   sub: %5.01f +- %5.02f Mtex/s\n", _dX/1000/1000, _dD/1000/1000);

  // test compressed formats UL speed only if texture compression is supported
  if( _pGfx->gl_ulFlags & GLF_TEXTURECOMPRESSION)
  {  
    _ulTexFormat = TS.ts_tfRGBc;
    _bSubImage = FALSE;
    RunTest(TextureUpload, 3);
    CPrintF("RGB-C  full: %5.01f +- %5.02f Mtex/s\n" , _dX/1000/1000, _dD/1000/1000);

    _ulTexFormat = TS.ts_tfRGBcA;
    _bSubImage = FALSE;
    RunTest(TextureUpload, 3);
    CPrintF("RGBA-C full: %5.01f +- %5.02f Mtex/s\n" , _dX/1000/1000, _dD/1000/1000);

    if( TS.ts_tfRGBcA != TS.ts_tfRGBcAc) {
      _ulTexFormat = TS.ts_tfRGBcAc;
      _bSubImage = FALSE;
      RunTest(TextureUpload, 3);
      CPrintF("RGBA-I full: %5.01f +- %5.02f Mtex/s\n" , _dX/1000/1000, _dD/1000/1000);
    }
  }

  // revert to 16-bit texture for further tests
  _ulTexFormat = TS.ts_tfRGB5;
  _bSubImage = FALSE;
  RunTest(TextureUpload, 1);

  // fill rate
  CPrintF("\n--- Fill rate\n");
  _bBlend = 0; _bDepth = 0; _iTexture = 0;
  RunTest(FillRate, 9);
  CPrintF("%-38s %6.01f +- %5.02f Mpix/s\n", FillRateString(), _dX/1000/1000, _dD/1000/1000);
  _bBlend = 0; _bDepth = 0; _iTexture = 1;
  RunTest(FillRate, 9);
  CPrintF("%-38s %6.01f +- %5.02f Mpix/s\n", FillRateString(), _dX/1000/1000, _dD/1000/1000);
  _bBlend = 1; _bDepth = 1; _iTexture = 1;
  RunTest(FillRate, 9);
  CPrintF("%-38s %6.01f +- %5.02f Mpix/s\n", FillRateString(), _dX/1000/1000, _dD/1000/1000);
  _bBlend = 0; _bDepth = 1; _iTexture = 1;
  RunTest(FillRate, 9);
  CPrintF("%-38s %6.01f +- %5.02f Mpix/s\n", FillRateString(), _dX/1000/1000, _dD/1000/1000);

  // multi-texturing
  if( ctTextureUnits>1) {
    for( _iTexture=2; _iTexture<=ctTextureUnits; _iTexture++) {
      RunTest(FillRate, 9);
      CPrintF("%-38s %6.01f +- %5.02f Mpix/s\n", FillRateString(), _dX/1000/1000, _dD/1000/1000);
    } // reset current tex unit
    _iTexture = 1;
  }

  // triangle thruput
  _bBlend = 0; _bDepth = 0; _iTexture = 0;
  CPrintF("\n--- Geometry speed (%dpix tris)\n", (_pixSizeI/_ctR)*(_pixSizeI/_ctC)/2);
  _iTexture = 0;
  RunTest(TrisTroughput, 5);
  CPrintF("%-38s %5.02f +- %4.02f Mtri/s\n", FillRateString(), _dX/1000/1000, _dD/1000/1000);
  _iTexture = 1;
  RunTest(TrisTroughput, 5);
  CPrintF("%-38s %5.02f +- %4.02f Mtri/s\n", FillRateString(), _dX/1000/1000, _dD/1000/1000);

  if( ctTextureUnits>1) {
    for( _iTexture=2; _iTexture<=ctTextureUnits; _iTexture++) {
      RunTest(TrisTroughput, 5);
      CPrintF("%-38s %5.02f +- %4.02f Mtri/s\n", FillRateString(), _dX/1000/1000, _dD/1000/1000);
    }
  }

  // finito!
  CPrintF("\n");
  _pdp->Fill(C_BLACK|255);
  _pvp->SwapBuffers();
  _pdp->Fill(C_BLACK|255);
  _pvp->SwapBuffers();
  EndTris();
  EndTexture();
}
