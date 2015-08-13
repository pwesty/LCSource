#include "stdh.h"

#include <Engine/Graphics/Raster.h>

#include <Engine/Base/ListIterator.inl>
#include <Engine/Graphics/DrawPort.h>
#include <Engine/Graphics/GfxLibrary.h>

/*
 *  Raster functions
 */

CRaster::CRaster(PIX ulWidth, PIX ulHeight, ULONG ulFlags) : ra_MainDrawPort()
{
  // remember width and height
  ra_Width  = ulWidth;
  ra_Height = ulHeight;

  // clear uninitialized fields
  ra_LockCount = 0;

  // set flags
  ra_Flags = ulFlags;

  // add main drawport to list
  ra_MainDrawPort.dp_Raster = this;
  ra_pvpViewPort = NULL;

	// when all is initialized, correct the drawport dimensions
  ra_MainDrawPort.dp_ScissorMinI = ra_MainDrawPort.dp_MinI = 0;
	ra_MainDrawPort.dp_ScissorMinJ = ra_MainDrawPort.dp_MinJ = 0;
  ra_MainDrawPort.dp_ScissorMaxI = ra_MainDrawPort.dp_MaxI = ra_Width;
	ra_MainDrawPort.dp_ScissorMaxJ = ra_MainDrawPort.dp_MaxJ = ra_Height;
}


CRaster::~CRaster(void)
{
  // raster must be unlocked before destroying it
	ASSERT(ra_LockCount==0);
}



/*
 * Lock for drawing.
 */
BOOL CRaster::Lock()
{
  ASSERT( this!=NULL);
  ASSERT( ra_LockCount>=0);
  ASSERT(ra_Width>0 && ra_Height>0);

  // if allready locked
  if( ra_LockCount>0) {
    // just increment counter
    ra_LockCount++;
    return TRUE;
  }
  // if not already locked
  else {
    // try to lock with driver
    BOOL bLocked = _pGfx->LockRaster(this);
    // if succeeded in locking
    if( bLocked) {
      // set the counter to 1
      ra_LockCount = 1;
      return TRUE;
    }
    // lock not ok
    return FALSE;
  }
}


/*
 * Unlock after drawing.
 */
void CRaster::Unlock()
{
  ASSERT( this!=NULL);
  // if not locked
  if (ra_LockCount<=0) {
    // do nothing
    return;
  }

  // decrement counter
  ra_LockCount--;
  // if reached zero
  if( ra_LockCount==0 ) {
    // unlock it with driver
    _pGfx->UnlockRaster(this);
  }
}


/*****
 * Change Raster size.
 */
void CRaster::Resize( PIX pixWidth, PIX pixHeight)
{
  ASSERT( pixWidth>0 && pixHeight>0);
  if( pixWidth <=0) pixWidth  = 1;
  if( pixHeight<=0) pixHeight = 1;
  ra_Width  = pixWidth;
  ra_Height = pixHeight;

  // correct the drawport dimensions
  ra_MainDrawPort.dp_ScissorMinI = ra_MainDrawPort.dp_MinI = 0;
	ra_MainDrawPort.dp_ScissorMinJ = ra_MainDrawPort.dp_MinJ = 0;
  ra_MainDrawPort.dp_ScissorMaxI = ra_MainDrawPort.dp_MaxI = ra_Width;
	ra_MainDrawPort.dp_ScissorMaxJ = ra_MainDrawPort.dp_MaxJ = ra_Height;
}
