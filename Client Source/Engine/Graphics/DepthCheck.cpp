#include "stdh.h"


#include <Engine/Base/Console.h>
#include <Engine/Graphics/GfxLibrary.h>
#include <Engine/Graphics/Raster.h>
#include <Engine/Graphics/ViewPort.h>

#include <Engine/Base/Statistics_internal.h>
#include <Engine/Templates/StaticArray.cpp>
#include <Engine/Templates/StaticStackArray.cpp>


extern INDEX gap_iOptimizeDepthReads;
extern INDEX gap_bUseOcclusionCulling;
extern COLOR UnpackColor_D3D( UBYTE *pd3dColor, D3DFORMAT d3dFormat, SLONG &slColorSize);

static INDEX _iCheckIteration = 0;
static CTimerValue _tvLast[8];  // 8 is max mirror recursion

#define KEEP_BEHIND 8

// info of one point for delayed depth buffer lookup
struct DepthInfo {
  INDEX di_iID;               // unique identifier
  PIX   di_pixI, di_pixJ;     // last requested coordinates
  FLOAT di_fOoK;              // last requested depth
  INDEX di_iSwapLastRequest;  // index of swap when last requested
  INDEX di_iMirrorLevel;      // level of mirror recursion in which flare is
  BOOL  di_bVisible;          // whether the point was visible
};
CStaticStackArray<DepthInfo> _adiDelayed;  // active delayed points
// don't ask, these are for D3D
CStaticStackArray<CTVERTEX> _avtxDelayed;  
CStaticStackArray<COLOR>    _acolDelayed;  

// read depth buffer and update visibility flag of depth points
static void UpdateDepthPointsVisibility( const CDrawPort *pdp, const INDEX iMirrorLevel,
                                         DepthInfo *pdi, const INDEX ctCount)
{
  const GfxAPIType eAPI = _pGfx->gl_eCurrentAPI;
  ASSERT( eAPI==GAT_OGL || eAPI==GAT_D3D || eAPI==GAT_NONE);
  ASSERT( pdp!=NULL && ctCount>0);
  const CRaster *pra = pdp->dp_Raster;

  // by default, see whether we have occlusion testing in disposal
  if( !_pGfx->HasOcclusionCulling()) gap_bUseOcclusionCulling = gap_bUseOcclusionCulling & 1;

  // fast check via occlusion testing?
  if( gap_bUseOcclusionCulling>=10)
  {
    // prepare rendering states
    gfxEnableDepthTest();
    gfxDisableDepthWrite();
    gfxDisableAlphaTest();
    gfxDisableTexture();
    gfxEnableBlend();
    gfxBlendFunc( GFX_SRC_ALPHA, GFX_INV_SRC_ALPHA);
    gfxCullFace(GFX_NONE);

    // for each stored point
    for( INDEX idi=0; idi<ctCount; idi++) {
      DepthInfo &di = pdi[idi];
      // skip if not in required mirror level or was already checked in this iteration
      if( iMirrorLevel!=di.di_iMirrorLevel || _iCheckIteration!=di.di_iSwapLastRequest) continue;
      // readout last occlusion query result
      SLONG slVisiblePixels = 0;
      gfxPullOcclusionQuery( di.di_iID, slVisiblePixels);  
      di.di_bVisible = (slVisiblePixels>0);

      // setup new occlusion test
      const BOOL bInTest = gfxBeginOcclusionTest( di.di_iID);
      if( !bInTest) continue; // this should not happen!

      // render tiny triangle
      const FLOAT fI = di.di_pixI - pdp->dp_MinI; // convert raster loc to drawport loc
      const FLOAT fJ = di.di_pixJ - pdp->dp_MinJ;
      const FLOAT afTriVtxs[3][3] = {
        { fI,   fJ-2, di.di_fOoK },
        { fI-2, fJ+2, di.di_fOoK },
        { fI+2, fJ,   di.di_fOoK } };
      #define FAINTCOLOR (0x08808080) // something that won't get into eyes (just to avoid switching the color buffer on/off)
      const ULONG aulTriCols[3] = { FAINTCOLOR, FAINTCOLOR, FAINTCOLOR };
      const UWORD auwTriIdxs[3] = { 0, 1, 2 };
      gfxSetVertexArray( (GFXVertex*)&afTriVtxs[0], 3);
      gfxSetColorArray(  (GFXColor*)&aulTriCols[0]);
      gfxDrawElements( 3, &auwTriIdxs[0]);
      gfxEndOcclusionTest();
    }
    // done
    return;
  }

  // OpenGL
  if( eAPI==GAT_OGL)
  { 
    _sfStats.StartTimer(CStatForm::STI_GFXAPI);
    FLOAT fPointOoK;
    // for each stored point
    for( INDEX idi=0; idi<ctCount; idi++) {
      DepthInfo &di = pdi[idi];
      // skip if not in required mirror level or was already checked in this iteration
      if( iMirrorLevel!=di.di_iMirrorLevel || _iCheckIteration!=di.di_iSwapLastRequest) continue;
      const PIX pixJ = pra->ra_Height-1 - di.di_pixJ; // OpenGL has Y-inversed buffer!
      pglReadPixels( di.di_pixI, pixJ, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &fPointOoK);
      OGL_CHECKERROR;
      // it is visible if there is nothing nearer in z-buffer already
      di.di_bVisible = (di.di_fOoK<fPointOoK);
    }
    // done
    _sfStats.StopTimer(CStatForm::STI_GFXAPI);
    return;
  }

  // Direct3D
  if( eAPI==GAT_D3D)
  {
    _sfStats.StartTimer(CStatForm::STI_GFXAPI);
    // ok, this will get really complicated ...
    // We'll have to do it thru back buffer because darn DX8 won't let us have values from z-buffer;
    // Anyway, we'll lock backbuffer, read color from the lens location and try to write little triangle there
    // with slightly modified color. Then we'll readout that color and see if triangle passes z-test. Voila! :)
    // P.S. To avoid lock-modify-lock, we need to batch all the locks in one. Uhhhh ... :(
    COLOR col;
    INDEX idi;
    SLONG slColSize;
    HRESULT hr;
    D3DLOCKED_RECT rectLocked;
    D3DSURFACE_DESC surfDesc;
    LPDIRECT3DSURFACE8 pBackBuffer;
    // fetch back buffer (different for full-screen and windowed mode)
    const BOOL bFullScreen = _pGfx->gl_ulFlags & GLF_FULLSCREEN;
    if(bFullScreen) {
      hr = _pGfx->gl_pd3dDevice->GetBackBuffer( 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer);
    } else {
      hr = pra->ra_pvpViewPort->vp_pSwapChain->GetBackBuffer( 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer);
    }
    // what, cannot get a back buffer?
    if( hr!=D3D_OK) { 
      // to hell with it all
      _sfStats.StopTimer(CStatForm::STI_GFXAPI);
      return;
    }
    // keep format of back-buffer
    pBackBuffer->GetDesc(&surfDesc);
    const D3DFORMAT d3dfBack = surfDesc.Format;
    
    // prepare array that'll back-buffer colors from depth point locations
    _acolDelayed.Push(ctCount);
    // store all colors
    for( idi=0; idi<ctCount; idi++) {
      DepthInfo &di = pdi[idi];
      // skip if not in required mirror level or was already checked in this iteration
      if( iMirrorLevel!=di.di_iMirrorLevel || _iCheckIteration!=di.di_iSwapLastRequest) continue;
      // fetch pixel
      _acolDelayed[idi] = 0;
      const RECT rectToLock = { di.di_pixI, di.di_pixJ, di.di_pixI+1, di.di_pixJ+1 };
      hr = pBackBuffer->LockRect( &rectLocked, &rectToLock, D3DLOCK_READONLY);
      if( hr!=D3D_OK) continue; // skip if lock didn't make it
      // read, convert and store original color
      _acolDelayed[idi] = UnpackColor_D3D( (UBYTE*)rectLocked.pBits, d3dfBack, slColSize) | CT_OPAQUE;
      pBackBuffer->UnlockRect();
    }

    // prepare to draw little triangles there with slightly adjusted colors
    _sfStats.StopTimer(CStatForm::STI_GFXAPI);
    gfxEnableDepthTest();
    gfxDisableDepthWrite();
    gfxDisableBlend();
    gfxDisableAlphaTest();
    gfxDisableTexture();
    gfxEnableColorArray();
    _sfStats.StartTimer(CStatForm::STI_GFXAPI);

    // prepare array and shader
    _avtxDelayed.Push(ctCount*3);
    d3dSetVertexShader(D3DFVF_CTVERTEX);

    // draw one trianle around each depth point
    INDEX ctVertex = 0;
    for( idi=0; idi<ctCount; idi++) {
      DepthInfo &di = pdi[idi];
      col = _acolDelayed[idi];
      // skip if not in required mirror level or was already checked in this iteration, or wasn't fetched at all
      if( iMirrorLevel!=di.di_iMirrorLevel || _iCheckIteration!=di.di_iSwapLastRequest || col==0) continue;
      const ULONG d3dCol = rgba2argb(col^0x20103000);
      const PIX pixI = di.di_pixI - pdp->dp_MinI; // convert raster loc to drawport loc
      const PIX pixJ = di.di_pixJ - pdp->dp_MinJ;
      // batch it and advance to next triangle
      CTVERTEX &vtx0 = _avtxDelayed[ctVertex++];
      CTVERTEX &vtx1 = _avtxDelayed[ctVertex++];
      CTVERTEX &vtx2 = _avtxDelayed[ctVertex++];
      vtx0.fX=pixI;   vtx0.fY=pixJ-2; vtx0.fZ=di.di_fOoK; vtx0.ulColor=d3dCol; vtx0.fU=vtx0.fV=0;
      vtx1.fX=pixI-2; vtx1.fY=pixJ+2; vtx1.fZ=di.di_fOoK; vtx1.ulColor=d3dCol; vtx1.fU=vtx0.fV=0;
      vtx2.fX=pixI+2; vtx2.fY=pixJ;   vtx2.fZ=di.di_fOoK; vtx2.ulColor=d3dCol; vtx2.fU=vtx0.fV=0;
    }
    const INDEX ctPrimitives = ctVertex/3;
    if(ctPrimitives>0) {
      // draw a bunch
      hr = _pGfx->gl_pd3dDevice->DrawPrimitiveUP( D3DPT_TRIANGLELIST, ctPrimitives, &_avtxDelayed[0], sizeof(CTVERTEX));
      D3D_CHECKERROR(hr);
    }

    // readout colors again and compare to old ones
    for( idi=0; idi<ctCount; idi++) {
      DepthInfo &di = pdi[idi];
      col = _acolDelayed[idi];
      // skip if not in required mirror level or was already checked in this iteration, or wasn't fetched at all
      if( iMirrorLevel!=di.di_iMirrorLevel || _iCheckIteration!=di.di_iSwapLastRequest || col==0) continue;
      // fetch pixel
      const RECT rectToLock = { di.di_pixI, di.di_pixJ, di.di_pixI+1, di.di_pixJ+1 };
      hr = pBackBuffer->LockRect( &rectLocked, &rectToLock, D3DLOCK_READONLY);
//안태훈 수정 시작	//(Open beta)(2005-01-04)
      if( hr!=D3D_OK || rectLocked.pBits == NULL) continue; // skip if lock didn't make it
//안태훈 수정 끝	//(Open beta)(2005-01-04)
      // read new color
      const COLOR colNew = UnpackColor_D3D( (UBYTE*)rectLocked.pBits, d3dfBack, slColSize) | CT_OPAQUE;
      pBackBuffer->UnlockRect();
      // if we managed to write adjusted color, point is visible!
      di.di_bVisible = (col!=colNew);
    }
    // phew, done! :)
    D3DRELEASE( pBackBuffer, TRUE);
    _acolDelayed.PopAll();
    _avtxDelayed.PopAll();
    _sfStats.StopTimer(CStatForm::STI_GFXAPI);
    return;
  }
}



// check point against depth buffer
extern BOOL CheckDepthPoint( const CDrawPort *pdp, PIX pixI, PIX pixJ, FLOAT fOoK, INDEX iID, INDEX iMirrorLevel/*=0*/)
{
  // no raster?
  const CRaster *pra = pdp->dp_Raster;
  if( pra==NULL) return FALSE;
  // almost out of raster?
  pixI += pdp->dp_MinI;
  pixJ += pdp->dp_MinJ;
  if( pixI<1 || pixJ<1 || pixI>pra->ra_Width-2 || pixJ>pra->ra_Height-2) return FALSE;

  // for each stored point
  DepthInfo *pdi = NULL;
  for( INDEX idi=0; idi<_adiDelayed.Count(); idi++) {
    // skip if different id
    if( _adiDelayed[idi].di_iID != iID) continue;
    // same id - remember new parameters
    pdi = &_adiDelayed[idi];
    pdi->di_pixI = pixI;
    pdi->di_pixJ = pixJ;
    pdi->di_fOoK = fOoK;
    pdi->di_iSwapLastRequest = _iCheckIteration;
    break;
  }
  // if not found
  if( pdi==NULL) {
    // create new one and remember parameters
    pdi = &_adiDelayed.Push();
    pdi->di_iID  = iID;
    pdi->di_pixI = pixI;
    pdi->di_pixJ = pixJ;
    pdi->di_fOoK = fOoK;
    pdi->di_iSwapLastRequest = _iCheckIteration;
    pdi->di_iMirrorLevel = iMirrorLevel;
    pdi->di_bVisible = FALSE; // not visible by default
  }

  // check immediately if allowed
  ASSERT( pdi!=NULL);
  if( gap_iOptimizeDepthReads==0) UpdateDepthPointsVisibility( pdp, iMirrorLevel, pdi, 1);
  return pdi->di_bVisible;
}


// check all delayed depth points
extern void CheckDelayedDepthPoints( const CDrawPort *pdp, INDEX iMirrorLevel/*=0*/)
{
  // skip if not delayed or mirror level is to high
  gap_iOptimizeDepthReads = Clamp( gap_iOptimizeDepthReads, 0L, 2L);
  if( gap_iOptimizeDepthReads==0 || iMirrorLevel>7) return; 
  ASSERT( pdp!=NULL && iMirrorLevel>=0);

  // check only if time lapse allows
  const CTimerValue tvNow = _pTimer->GetHighPrecisionTimer();
  const TIME tmDelta = (tvNow-_tvLast[iMirrorLevel]).GetSeconds();
  ASSERT( tmDelta>=0);
  if( gap_iOptimizeDepthReads==2 && tmDelta<0.1f) return;

  // prepare
  _tvLast[iMirrorLevel] = tvNow;
  INDEX ctPoints = _adiDelayed.Count();
  if( ctPoints==0) return; // done if no points in queue

  // for each point
  INDEX iPoint = 0;
  while( iPoint<ctPoints) {
    DepthInfo &di = _adiDelayed[iPoint];
    // if the point is not active any more
    if( iMirrorLevel==di.di_iMirrorLevel && di.di_iSwapLastRequest<(_iCheckIteration-KEEP_BEHIND)) {
      // free occlusion query
      gfxFreeOcclusionQuery( di.di_iID);
      // delete it by moving the last one on its place
      di = _adiDelayed[ctPoints-1];
      ctPoints--;
    // if the point is still active
    } else {
      // go to next point
      iPoint++;
    }
  }

  // remove unused points at the end
  if( ctPoints==0) _adiDelayed.PopAll();
  else _adiDelayed.PopUntil(ctPoints-1);

  // ignore stalls
  if( tmDelta>1.0f) return;

  // check and upadete visibility of what has left
  ASSERT( ctPoints == _adiDelayed.Count());
  if( ctPoints>0) UpdateDepthPointsVisibility( pdp, iMirrorLevel, &_adiDelayed[0], ctPoints);
  // mark checking
  _iCheckIteration++;
}

