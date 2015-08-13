#include "stdh.h"

#include <Engine/Brushes/Brush.h>
#include <Engine/Base/ReplaceFile.h>
#include <Engine/Base/MemoryTracking.h>
#include <Engine/Brushes/BrushArchive.h>
#include <Engine/World/WorldEditingProfile.h>
#include <Engine/World/World.h>
#include <Engine/Math/Float.h>
#include <Engine/Base/Stream.h>
#include <Engine/Entities/Entity.h>
#include <Engine/Base/ListIterator.inl>

#include <Engine/Templates/BSP.h>
#include <Engine/Templates/BSP_internal.h>
#include <Engine/Templates/DynamicArray.cpp>
#include <Engine/Templates/StaticArray.cpp>
#include <Engine/Loading.h>
template CDynamicArray<CBrush3D>;

extern BOOL _bPortalSectorLinksPreLoaded = FALSE;
extern BOOL _bEntitySectorLinksPreLoaded = FALSE;

/*
 * Calculate bounding boxes in all brushes.
 */
void CBrushArchive::CalculateBoundingBoxes(void)
{
  _pfWorldEditingProfile.StartTimer(CWorldEditingProfile::PTI_CALCULATEBOUNDINGBOXES);
  // for each of the brushes
  FOREACHINDYNAMICARRAY(ba_abrBrushes, CBrush3D, itbr) {
    // if the brush has no entity
    if (itbr->br_penEntity==NULL) {
      // skip it
      continue;
    }
    // calculate its boxes
    itbr->CalculateBoundingBoxes();
  }
  _pfWorldEditingProfile.StopTimer(CWorldEditingProfile::PTI_CALCULATEBOUNDINGBOXES);
}

/* Make indices for all brush elements. */
void CBrushArchive::MakeIndices(void)
{
  // NOTE: Mips and brushes don't have indices, because it is not needed yet.
  // Polygon and sector indices are needed for loading/saving of portal-sector links.

  INDEX ctBrushes=0;
  INDEX ctMips=0;
  INDEX ctSectors=0;
  INDEX ctPolygons=0;
  // for each brush
  FOREACHINDYNAMICARRAY(ba_abrBrushes, CBrush3D, itbr) {
    // for each mip in the brush
    FOREACHINLIST(CBrushMip, bm_lnInBrush, itbr->br_lhBrushMips, itbm) {
      // for each sector in the brush mip
      FOREACHINDYNAMICARRAY(itbm->bm_abscSectors, CBrushSector, itbsc) {
        // for each polygon in the sector
        FOREACHINSTATICARRAY(itbsc->bsc_abpoPolygons, CBrushPolygon, itbpo) {
          itbpo->bpo_iInWorld = ctPolygons;
          ctPolygons++;
        }
        itbsc->bsc_iInWorld = ctSectors;
        ctSectors++;
      }
      ctMips++;
    }
    ctBrushes++;
  }

  // make arrays of pointers to sectors and polygons
  ba_apbpo.Clear();
  ba_apbpo.New(ctPolygons);
  ba_apbsc.Clear();
  ba_apbsc.New(ctSectors);
  {FOREACHINDYNAMICARRAY(ba_abrBrushes, CBrush3D, itbr) {
    FOREACHINLIST(CBrushMip, bm_lnInBrush, itbr->br_lhBrushMips, itbm) {
      FOREACHINDYNAMICARRAY(itbm->bm_abscSectors, CBrushSector, itbsc) {
        ba_apbsc[itbsc->bsc_iInWorld] = itbsc; // 섹터
        FOREACHINSTATICARRAY(itbsc->bsc_abpoPolygons, CBrushPolygon, itbpo) {
          ba_apbpo[itbpo->bpo_iInWorld] = itbpo; // 폴리곤
        }
      }
    }
  }}
}

#define DISTANCE_EPSILON 0.1f
/* Create links between portals and sectors on their other side. */
void CBrushArchive::LinkPortalsAndSectors(void)
{
  _pfWorldEditingProfile.StartTimer(CWorldEditingProfile::PTI_LINKPORTALSANDSECTORS);
  SetFPUPrecision(FPT_53BIT);

  // for each of the zoning brushes
  FOREACHINDYNAMICARRAY(ba_abrBrushes, CBrush3D, itbr1) {
    if (itbr1->br_penEntity==NULL || !(itbr1->br_penEntity->en_ulFlags&ENF_ZONING)) {
      continue;
    }
    // for each mip
    FOREACHINLIST(CBrushMip, bm_lnInBrush, itbr1->br_lhBrushMips, itbm1) {
      // for each sector in the brush mip
      FOREACHINDYNAMICARRAY(itbm1->bm_abscSectors, CBrushSector, itbsc1) {

        // for each of the zoning brushes
        FOREACHINDYNAMICARRAY(ba_abrBrushes, CBrush3D, itbr2) {
          if (itbr2->br_penEntity==NULL || !(itbr2->br_penEntity->en_ulFlags&ENF_ZONING)) {
            continue;
          }
          // for each mip that might have contact with the sector
          FOREACHINLIST(CBrushMip, bm_lnInBrush, itbr2->br_lhBrushMips, itbm2) {
            if (!itbm2->bm_boxBoundingBox.HasContactWith(itbsc1->bsc_boxBoundingBox, DISTANCE_EPSILON)) {
              continue;
            }
            // for each sector in the brush mip that might have contact, except current one
            FOREACHINDYNAMICARRAY(itbm2->bm_abscSectors, CBrushSector, itbsc2) {
              if (&*itbsc1==&*itbsc2) {
                continue;
              }
              if (!itbsc2->bsc_boxBoundingBox.HasContactWith(itbsc1->bsc_boxBoundingBox, DISTANCE_EPSILON)) {
                continue;
              }
              // for all portals in this sector that might have contact
              FOREACHINSTATICARRAY(itbsc2->bsc_abpoPolygons, CBrushPolygon, itbpo2) {
                if (!(itbpo2->bpo_ulFlags&(BPOF_PORTAL|BPOF_PASSABLE))) {
                  continue;
                }
                if (!itbpo2->bpo_boxBoundingBox.HasContactWith(itbsc1->bsc_boxBoundingBox, DISTANCE_EPSILON)) {
                  continue;
                }
                // create a BSP polygon from the brush polygon
                CBrushPolygon        &brpo2 = *itbpo2;
                BSPPolygon<DOUBLE, 3> bspo2;
                brpo2.CreateBSPPolygonNonPrecise(bspo2);
                // split the polygon with the BSP of the sector
                DOUBLEbspcutter3D bcCutter(bspo2, *itbsc1->bsc_bspBSPTree.bt_pbnRoot);
                // if anything remains on the border looking outside
                if (bcCutter.bc_abedInside.Count()>0
                  ||bcCutter.bc_abedBorderInside.Count()>0
                  ||bcCutter.bc_abedBorderOutside.Count()>0) {
                  // relate the sector to the portal
                  AddRelationPair(
                    itbpo2->bpo_rsOtherSideSectors,
                    itbsc1->bsc_rdOtherSidePortals);
                }
              }
            }
          }
        }
      }
    }
  }
  _pfWorldEditingProfile.StopTimer(CWorldEditingProfile::PTI_LINKPORTALSANDSECTORS);
}


// remove shadow layers without valid light source in all brushes
void CBrushArchive::RemoveDummyLayers(void)
{
  // for each brush
  FOREACHINDYNAMICARRAY(ba_abrBrushes, CBrush3D, itbr) { // for each mip
    if( itbr->br_penEntity==NULL) continue; // skip brush without entity
    FOREACHINLIST(CBrushMip, bm_lnInBrush, itbr->br_lhBrushMips, itbm) { // for each sector in the brush mip
      FOREACHINDYNAMICARRAY(itbm->bm_abscSectors, CBrushSector, itbsc) { // for each polygon in the sector
        FOREACHINSTATICARRAY(itbsc->bsc_abpoPolygons, CBrushPolygon, itbpo) {
          CBrushPolygon &bpo = *itbpo;
          bpo.bpo_smShadowMap.RemoveDummyLayers(); // remove shadow layers without valid light source
        }
      }
    }
  }
}


// cache all shadowmaps 
void CBrushArchive::CacheAllShadowmaps(void)
{
  // count all shadowmaps
  INDEX ctShadowMaps=0;
  {FOREACHINDYNAMICARRAY( ba_abrBrushes, CBrush3D, itbr) { // for each mip
    if( itbr->br_penEntity==NULL) continue; // skip brush without entity
    FOREACHINLIST( CBrushMip, bm_lnInBrush, itbr->br_lhBrushMips, itbm) { // for each sector in the brush mip
      FOREACHINDYNAMICARRAY( itbm->bm_abscSectors, CBrushSector, itbsc) { // for each polygon in the sector
        FOREACHINSTATICARRAY( itbsc->bsc_abpoPolygons, CBrushPolygon, itbpo) {
          if( !itbpo->bpo_smShadowMap.bsm_lhLayers.IsEmpty()) ctShadowMaps++; // count shadowmap if the one exist
        }
      }
    }
  }}

  try {
    CallProgressHook_t(0.0f);
    // for each brush
    INDEX iCurrentShadowMap=0;
    {FOREACHINDYNAMICARRAY( ba_abrBrushes, CBrush3D, itbr) { // for each mip
      if( itbr->br_penEntity==NULL) continue; // skip brush without entity
      FOREACHINLIST( CBrushMip, bm_lnInBrush, itbr->br_lhBrushMips, itbm) { // for each sector in the brush mip
        FOREACHINDYNAMICARRAY( itbm->bm_abscSectors, CBrushSector, itbsc) { // for each polygon in the sector
          FOREACHINSTATICARRAY( itbsc->bsc_abpoPolygons, CBrushPolygon, itbpo) {
            // cache shadowmap if the one exist
            CBrushShadowMap &bsm = itbpo->bpo_smShadowMap;
            if( bsm.bsm_lhLayers.IsEmpty()) continue;
            bsm.CheckLayersUpToDate();
            bsm.Prepare();
            bsm.SetAsCurrent();
            iCurrentShadowMap++;
            CallProgressHook_t( (FLOAT)iCurrentShadowMap/ctShadowMaps);
          }
        }
      }
    }}
    // all done
    CallProgressHook_t(1.0f);
  }
  catch( char*) { NOTHING; }
}


void CBrushArchive::ReadPortalSectorLinks_t( CTStream &strm)  // throw char *
{
  // links are not ok if they fail loading
  _bPortalSectorLinksPreLoaded = FALSE;

  _pfWorldEditingProfile.StartTimer(CWorldEditingProfile::PTI_READPORTALSECTORLINKS);
  // first make indices for all sectors and polygons
  MakeIndices();

  // if the chunk is not there
  if (!(strm.PeekID_t()==CChunkID("PSLS"))) {   // portal-sector links
    // do nothing;
    _pfWorldEditingProfile.StopTimer(CWorldEditingProfile::PTI_READPORTALSECTORLINKS);
    return;
  }

  // read the version
  strm.ExpectID_t("PSLS");   // portal-sector links
  INDEX iVersion;
  strm>>iVersion;
  ASSERT(iVersion==1);
  // read chunk size
  SLONG slChunkSizePos = strm.GetPos_t();
  SLONG slChunkSize;
  strm>>slChunkSize;

  // repeat
  FOREVER {
    // read sector index
    INDEX iSector;
    strm>>iSector;
    // if end marker
    if (iSector==-1) {
      // stop loading
      break;
    }
    // get the sector
    CBrushSector *pbsc = ba_apbsc[iSector];
    ASSERT(pbsc->bsc_iInWorld==iSector);
    // read number of links
    INDEX ctLinks;
    strm>>ctLinks;
    // for each link
    for(INDEX iLink=0; iLink<ctLinks; iLink++) {
      // read polygon index
      INDEX iPolygon;
      strm>>iPolygon;
      CBrushPolygon *pbpo = ba_apbpo[iPolygon];
      ASSERT(pbpo->bpo_iInWorld==iPolygon);
      // relate the sector to the portal
      AddRelationPair(
        pbpo->bpo_rsOtherSideSectors,
        pbsc->bsc_rdOtherSidePortals);
    }
    pbsc->bsc_ulTempFlags|=BSCTF_PRELOADEDLINKS;
  }

  // check chunk size
  ASSERT(strm.GetPos_t()-slChunkSizePos-sizeof(INDEX)==slChunkSize);
  // check end id
  strm.ExpectID_t("PSLE");   // portal-sector links end
  // mark that links are ok
  _bPortalSectorLinksPreLoaded = TRUE;
  _pfWorldEditingProfile.StopTimer(CWorldEditingProfile::PTI_READPORTALSECTORLINKS);
}

void CBrushArchive::ReadEntitySectorLinks_t( CTStream &strm)  // throw char *
{
  // links are not ok if they fail loading
  _bEntitySectorLinksPreLoaded = FALSE;

  _pfWorldEditingProfile.StartTimer(CWorldEditingProfile::PTI_READPORTALSECTORLINKS);
  // first make indices for all sectors and polygons
  MakeIndices();

  // if the chunk is not there
  if (!(strm.PeekID_t()==CChunkID("ESL2"))) {   // entity-sector links v2
    // do nothing;
    _pfWorldEditingProfile.StopTimer(CWorldEditingProfile::PTI_READPORTALSECTORLINKS);
    return;
  }

  // read the version
  strm.ExpectID_t("ESL2");   // entity-sector links v2
  INDEX iVersion;
  strm>>iVersion;
  ASSERT(iVersion==1);
  // read chunk size
  SLONG slChunkSizePos = strm.GetPos_t();
  SLONG slChunkSize;
  strm>>slChunkSize;

  // repeat
  FOREVER {
    // read sector index
    INDEX iSector;
    strm>>iSector;
    // if end marker
    if (iSector==-1) {
      // stop loading
      break;
    }
    // get the sector
    CBrushSector *pbsc = ba_apbsc[iSector];
    ASSERT(pbsc->bsc_iInWorld==iSector);
    // read number of links
    INDEX ctLinks;
    strm>>ctLinks;
    pbsc->bsc_rsEntities.Clear();
    // for each link
    for(INDEX iLink=0; iLink<ctLinks; iLink++) {
      // read entity index
      INDEX iEntity;
      strm>>iEntity;
      CEntity *pen = ba_pwoWorld->EntityFromID(iEntity);
      // relate the sector to the entity
      AddRelationPair(pbsc->bsc_rsEntities, pen->en_rdSectors);
    }
  }

  // check chunk size
  ASSERT(strm.GetPos_t()-slChunkSizePos-sizeof(INDEX)==slChunkSize);
  // check end id
  strm.ExpectID_t("ESLE");   // entity-sector links end

  // mark that links are ok
  _bEntitySectorLinksPreLoaded = TRUE;
  _pfWorldEditingProfile.StopTimer(CWorldEditingProfile::PTI_READPORTALSECTORLINKS);
}



void CBrushArchive::WritePortalSectorLinks_t( CTStream &strm) // throw char *
{
  // first make indices for all sectors and polygons
  MakeIndices();

  // write chunk id and current version
  strm.WriteID_t("PSLS");   // portal-sector links
  strm<<INDEX(1);
  // leave room for chunk size
  SLONG slChunkSizePos = strm.GetPos_t();
  strm<<SLONG(0);

  // for each sector
  {FOREACHINDYNAMICARRAY(ba_abrBrushes, CBrush3D, itbr) {
    FOREACHINLIST(CBrushMip, bm_lnInBrush, itbr->br_lhBrushMips, itbm) {
      FOREACHINDYNAMICARRAY(itbm->bm_abscSectors, CBrushSector, itbsc) {
        CBrushSector *pbsc = itbsc;
        // get number of portal links that it has
        INDEX ctLinks = pbsc->bsc_rdOtherSidePortals.Count();
        // if it has no links
        if (ctLinks==0) {
          // skip it
          continue;
        }
        // write sector index and number of links
        strm<<pbsc->bsc_iInWorld<<ctLinks;
        // for each link
        {FOREACHSRCOFDST(pbsc->bsc_rdOtherSidePortals, CBrushPolygon, bpo_rsOtherSideSectors, pbpo)
          // write the polygon index
          strm<<pbpo->bpo_iInWorld;
        ENDFOR}
      }
    }
  }}
  // write sector index -1 as end marker
  strm<<INDEX(-1);

  // write back the chunk size
  SLONG slEndPos = strm.GetPos_t();
  strm.SetPos_t(slChunkSizePos);
  strm<<SLONG(slEndPos-slChunkSizePos-sizeof(INDEX));
  strm.SetPos_t(slEndPos);

  // write end id for checking
  strm.WriteID_t("PSLE");   // portal-sector links end
}

void CBrushArchive::WriteEntitySectorLinks_t( CTStream &strm) // throw char *
{
  // first make indices for all sectors and polygons
  MakeIndices();

  // write chunk id and current version
  strm.WriteID_t("ESL2");   // entity-sector links v2
  strm<<INDEX(1);
  // leave room for chunk size
  SLONG slChunkSizePos = strm.GetPos_t();
  strm<<SLONG(0);

  // for each sector
  {FOREACHINDYNAMICARRAY(ba_abrBrushes, CBrush3D, itbr) {
    FOREACHINLIST(CBrushMip, bm_lnInBrush, itbr->br_lhBrushMips, itbm) {
      FOREACHINDYNAMICARRAY(itbm->bm_abscSectors, CBrushSector, itbsc) {
        CBrushSector *pbsc = itbsc;
        // get number of entity links that it has
        INDEX ctLinks = pbsc->bsc_rsEntities.Count();
        // if it has no links
        if (ctLinks==0) {
          // skip it
          continue;
        }
        // write sector index and number of links
        strm<<pbsc->bsc_iInWorld<<ctLinks;
        // for each link
        {FOREACHDSTOFSRC(pbsc->bsc_rsEntities, CEntity, en_rdSectors, pen)
          // write the entity index
          strm<<pen->en_ulID;
        ENDFOR}
      }
    }
  }}
  // write sector index -1 as end marker
  strm<<INDEX(-1);

  // write back the chunk size
  SLONG slEndPos = strm.GetPos_t();
  strm.SetPos_t(slChunkSizePos);
  strm<<SLONG(slEndPos-slChunkSizePos-sizeof(INDEX));
  strm.SetPos_t(slEndPos);
  // write end id for checking
  strm.WriteID_t("ESLE");   // entity-sector links end
}


/*
 * Read from stream.
 */
void CBrushArchive::Read_t( CTStream *istrFile) // throw char *
{
  TRACKMEM(Brushes, "Brushes");

  istrFile->ExpectID_t("BRAR");   // brush archive

  INDEX ctBrushes;
  // read number of brushes
  (*istrFile)>>ctBrushes;

  // if there are some brushes
  if (ctBrushes!=0) {
    // create that much brushes
    CBrush3D *abrBrushes = ba_abrBrushes.New(ctBrushes);
    // for each of the new brushes
    for (INDEX iBrush=0; iBrush<ctBrushes; iBrush++) {
      // read it from stream
      CallProgressHook_t(FLOAT(iBrush)/ctBrushes);
      abrBrushes[iBrush].Read_t(istrFile);
    }
  }

  // read links if possible
  ReadPortalSectorLinks_t(*istrFile);

  istrFile->ExpectID_t("EOAR");   // end of archive
}

/*
 * Write to stream.
 */
void CBrushArchive::Write_t( CTStream *ostrFile) // throw char *
{
  ostrFile->WriteID_t("BRAR");   // brush archive

  // write the number of brushes
  (*ostrFile)<<ba_abrBrushes.Count();
  // for each of the brushes
  FOREACHINDYNAMICARRAY(ba_abrBrushes, CBrush3D, itbr) {
    // write it to stream
    itbr->Write_t(ostrFile);
  }

  // write links
  WritePortalSectorLinks_t(*ostrFile);
  ostrFile->WriteID_t("EOAR");   // end of archive
}

// yjpark |<--
#ifndef	FINALVERSION
static void BR_DrawMapAttrLine( int nSX, int nSY, int nEX, int nEY, UWORD* aTempAttrMap, int nWidth, UWORD ubAttr )
{
	if( nSX < 0 || nSY < 0 || nEX < 0 || nEY < 0 )		// 릴리즈에서 음수 값이 몇개 들어올때가 있음.. 언젠가 처리해야지..
		return;

	// Starting point of line
	int	x = nSX;
	int	y = nSY;

	// Direction of line
	int	dx = nEX - nSX;
	int	dy = nEY - nSY;

	// Increment or decrement depending on direction of line
	int	sx, sy;
	
	if( dx > 0 )
		sx = 1;
	else if( dx < 0 )
	{
		sx = -1;		dx = -dx;
	}
	else
		sx = 0;

	if( dy > 0 )
		sy = 1;
	else if( dy < 0 )
	{
		sy = -1;		dy = -dy;
	}
	else
		sy = 0;

	int	ax = 2 * dx;
	int	ay = 2 * dy;

	if( dy <= dx )
	{
		// Single step in x-direction
		for( int decy = ay - dx; ; x += sx, decy += ay )
		{
			aTempAttrMap[y*nWidth +x] = ubAttr;

			// Take bresenham stemp
			if( x == nEX )
				break;

			if( decy >= 0 )
			{
				decy -= ax;		y += sy;
			}
		}
	}
	else
	{
		// Single stemp in y-direction
		for( int decx = ax - dy; ; y += sy, decx += ax )
		{
			aTempAttrMap[y*nWidth +x] = ubAttr;

			// Take bresenham step
			if( y == nEY )
				break;

			if( decx >= 0 )
			{
				decx -= ay;		x += sx;
			}
		}
	}
}

static void BR_DrawAttributeMap( POINT ptStart[], POINT ptEnd[], INDEX ctPoints,
								UWORD *pubAttr, PIX pixWidth, PIX pixHeight, UWORD ubAttr )
{
	// Get min & max box
	POINT	ptMin = ptStart[0];
	POINT	ptMax = ptStart[0];
	INDEX	iPoint;
	for( iPoint = 1; iPoint < ctPoints; iPoint++ )
	{
		if( ptStart[iPoint].x < ptMin.x ) ptMin.x = ptStart[iPoint].x;
		else if( ptStart[iPoint].x > ptMax.x ) ptMax.x = ptStart[iPoint].x;
		if( ptStart[iPoint].y < ptMin.y ) ptMin.y = ptStart[iPoint].y;
		else if( ptStart[iPoint].y > ptMax.y ) ptMax.y = ptStart[iPoint].y;
		
		if( ptEnd[iPoint].x < ptMin.x ) ptMin.x = ptEnd[iPoint].x;
		else if( ptEnd[iPoint].x > ptMax.x ) ptMax.x = ptEnd[iPoint].x;
		if( ptEnd[iPoint].y < ptMin.y ) ptMin.y = ptEnd[iPoint].y;
		else if( ptEnd[iPoint].y > ptMax.y ) ptMax.y = ptEnd[iPoint].y;
	}

	// If the region is valid
	if( ptMin.x < 0 || ptMin.y < 0 || ptMax.x >= pixWidth || ptMax.y >= pixHeight )
		return ;

	// Get width/height of temp attribute map
	int	nWidth = ptMax.x - ptMin.x;
	int	nHeight = ptMax.y - ptMin.y;
	//UBYTE	aTempAttrMap[512][512];
	UWORD*	aTempAttrMap =new UWORD[(nWidth+1)*(nHeight+1)];
	memset( aTempAttrMap, 0, (nWidth+1)*(nHeight+1) * sizeof(UWORD));

	int	nX, nY;
	/*****
	for( nY = 0; nY <= nHeight; nY++ )
	{
		for( nX = 0; nX <= nWidth; nX++ )
			aTempAttrMap[nY][nX] = 0;
	}
	*****/

	// Draw map attribute line
	for( iPoint = 0; iPoint < ctPoints; iPoint++ )
	{
		ptStart[iPoint].x -= ptMin.x;		ptStart[iPoint].y -= ptMin.y;
		ptEnd[iPoint].x -= ptMin.x;			ptEnd[iPoint].y -= ptMin.y;
		BR_DrawMapAttrLine( ptStart[iPoint].x, ptStart[iPoint].y, ptEnd[iPoint].x, ptEnd[iPoint].y,
							aTempAttrMap, nWidth+1, ubAttr );
	}

	// Fill map attribute region
	int	nSX, nEX;
	for( nY = 0; nY <= nHeight; nY++ )
	{
		nSX = 0;	nEX = 0;
		// Find start point
		for( nX = 0; nX <= nWidth; nX++ )
		{
			if( aTempAttrMap[nY*(nWidth+1) +nX] == ubAttr )
			{
				nSX = nX;	break;
			}
		}

		// Find end point
		for( nX = nWidth; nX >= 0; nX-- )
		{
			if( aTempAttrMap[nY*(nWidth+1) +nX] == ubAttr )
			{
				nEX = nX;	break;
			}
		}

		// Fill attribute value from start point to end point
		if( nSX != nEX && nSX < nEX )
		{
			for( nX = nSX; nX <= nEX; nX++ )
				aTempAttrMap[nY*(nWidth+1) +nX] = ubAttr;
		}
	}

	// Copy temp attribute valuse to attribute map
	UWORD	*pTemp;
	for( nY = 0; nY <= nHeight; nY++ )
	{
		pTemp = &pubAttr[ptMin.x + ( ptMin.y + nY ) * pixWidth];
		for( nX = 0; nX <= nWidth; nX++ )
		{
			if( aTempAttrMap[nY*(nWidth+1) +nX] == ubAttr )
				*pTemp = aTempAttrMap[nY*(nWidth+1) +nX];
			pTemp++;
		}
	}

	delete[] aTempAttrMap;
}

#include <Engine/World/WorldRayCasting.h>
static BOOL BR_FindBelowPolygonByAllPolygon( CStaticArray<CBrushPolygon *> &apbpo,
												const FLOAT3D &rOrig, const FLOAT3D &rDir,
												INDEX iFloor, FLOAT fMaxDist )
{
	CBrushPolygon	*pBpo;
	UBYTE			ubPolygonAttr;
	FOREACHINSTATICARRAY( apbpo, CBrushPolygon *, itBpo )
	{
		pBpo = *itBpo;
		if( pBpo == NULL )
			continue;

		ubPolygonAttr = pBpo->bpo_ubPolygonAttribute;
		if( ubPolygonAttr == BPOA_NONE )
			continue;
		if( ubPolygonAttr > BPOA_WALKABLE5F && ubPolygonAttr < BPOA_STAIR1F2F )			// Unwalkable
			continue;
		if( ubPolygonAttr > BPOA_STAIR4F5F && ubPolygonAttr < BPOA_PEACEZONE1F )		// Stairwall
			continue;
		if( ubPolygonAttr < BPOA_UNWALKABLE1F && ubPolygonAttr != iFloor )				// Walkalbe but different floor
			continue;
		if( ubPolygonAttr > BPOA_STAIRWALL4F5F && ( ubPolygonAttr % 10 ) != iFloor )	// Peace, product zone but different floor
			continue;
		if( ubPolygonAttr > BPOA_UNWALKABLE5F && ubPolygonAttr < BPOA_STAIRWALL1F2F &&	// Stair but different floor
			ubPolygonAttr != iFloor + 19 && ubPolygonAttr != iFloor + 20 )
			continue;

		FLOAT3D	vTarget = rOrig + rDir * 10000.0f;
		FLOAT	fDistance0 = pBpo->bpo_pbplPlane->bpl_plAbsolute.PointDistance( rOrig );
		FLOAT	fDistance1 = pBpo->bpo_pbplPlane->bpl_plAbsolute.PointDistance( vTarget );

		if( fDistance0 < 0 || fDistance0 < fDistance1 || fDistance0 > fMaxDist )
			continue;

		//FLOAT	fFraction = fDistance0 / ( fDistance0 - fDistance1 );
		//FLOAT3D	vHitPoint = rOrig + rDir * fFraction;
		//if( !pBpo->bpo_boxBoundingBox.TouchesSphere( vHitPoint, 0.1f ) )
		//	continue;

		INDEX	iV0, iV1, iV2;
		INDEX	ctIndices = pBpo->bpo_aiTriangleElements.Count();
		for( INDEX iIndex = 0; iIndex < ctIndices; iIndex += 3 )
		{
			iV0 = pBpo->bpo_aiTriangleElements[iIndex];
			iV1 = pBpo->bpo_aiTriangleElements[iIndex + 1];
			iV2 = pBpo->bpo_aiTriangleElements[iIndex + 2];
			FLOAT3D	vP0 = pBpo->bpo_apbvxTriangleVertices[iV0]->bvx_vAbsolute;
			FLOAT3D	vP1 = pBpo->bpo_apbvxTriangleVertices[iV1]->bvx_vAbsolute;
			FLOAT3D	vP2 = pBpo->bpo_apbvxTriangleVertices[iV2]->bvx_vAbsolute;

			FLOAT	fT;
			if( RayHitsTriangle( rOrig, rDir, vP0, vP1, vP2, &fT ) )
				return TRUE;
		}
	}

	return FALSE;
}

static BOOL BR_FindBelowPolygonBySector( CStaticArray<CBrushSector *> &apbsc,
											const FLOAT3D &rOrig, const FLOAT3D &rDir,
											INDEX iFloor, FLOAT fMaxDist )
{
	CBrushSector	*pBpsc;
	FOREACHINSTATICARRAY( apbsc, CBrushSector *, itBpsc )
	{
		pBpsc = *itBpsc;
		if( pBpsc == NULL )
			continue;
		if( pBpsc->bsc_ulFlags & BSCF_HIDDEN )
			continue;
	
		//////////////////////////////////////////////////////////////////////////////
		// [070711: Su-won] WORLDEDITOR_BUGFIX							  |---------->
		if( pBpsc->bsc_pbmBrushMip ==NULL)
			continue;

		if( pBpsc->bsc_pbmBrushMip->bm_pbrBrush ==NULL)
			continue;

		if( pBpsc->bsc_pbmBrushMip->bm_pbrBrush->br_penEntity==NULL)
			continue;
		// [070711: Su-won] WORLDEDITOR_BUGFIX							  <----------|
		//////////////////////////////////////////////////////////////////////////////

		if( pBpsc->bsc_pbmBrushMip->bm_pbrBrush->br_penEntity->en_ulFlags & ENF_HIDDEN )
			continue;

		UBYTE			ubPolygonAttr;
		FOREACHINSTATICARRAY( pBpsc->bsc_abpoPolygons, CBrushPolygon, itBpo )
		{
			CBrushPolygon	&rBpo = itBpo.Current();

			ubPolygonAttr = rBpo.bpo_ubPolygonAttribute;
			if( ubPolygonAttr == BPOA_NONE )
				continue;
			if( ubPolygonAttr > BPOA_WALKABLE5F && ubPolygonAttr < BPOA_STAIR1F2F )			// Unwalkable
				continue;
			if( ubPolygonAttr > BPOA_STAIR4F5F && ubPolygonAttr < BPOA_PEACEZONE1F )		// Stairwall
				continue;
			if( ubPolygonAttr < BPOA_UNWALKABLE1F && ubPolygonAttr != iFloor )				// Walkalbe but different floor
				continue;
			if( ubPolygonAttr > BPOA_STAIRWALL4F5F && ( ubPolygonAttr % 10 ) != iFloor )	// Peace, product zone but different floor
				continue;
			if( ubPolygonAttr > BPOA_UNWALKABLE5F && ubPolygonAttr < BPOA_STAIRWALL1F2F &&	// Stair but different floor
				ubPolygonAttr != iFloor + 19 && ubPolygonAttr != iFloor + 20 )
				continue;

			FLOAT3D	vTarget = rOrig + rDir * 10000.0f;
			FLOAT	fDistance0 = rBpo.bpo_pbplPlane->bpl_plAbsolute.PointDistance( rOrig );
			FLOAT	fDistance1 = rBpo.bpo_pbplPlane->bpl_plAbsolute.PointDistance( vTarget );

			if( fDistance0 < 0 || fDistance0 < fDistance1 || fDistance0 > fMaxDist )
				continue;

			//FLOAT	fFraction = fDistance0 / ( fDistance0 - fDistance1 );
			//FLOAT3D	vHitPoint = rOrig + rDir * fFraction;
			//if( !rBpo.bpo_boxBoundingBox.TouchesSphere( vHitPoint, 0.1f ) )
			//	continue;

			INDEX	iV0, iV1, iV2;
			INDEX	ctIndices = rBpo.bpo_aiTriangleElements.Count();
			for( INDEX iIndex = 0; iIndex < ctIndices; iIndex += 3 )
			{
				iV0 = rBpo.bpo_aiTriangleElements[iIndex];
				iV1 = rBpo.bpo_aiTriangleElements[iIndex + 1];
				iV2 = rBpo.bpo_aiTriangleElements[iIndex + 2];
				FLOAT3D	vP0 = rBpo.bpo_apbvxTriangleVertices[iV0]->bvx_vAbsolute;
				FLOAT3D	vP1 = rBpo.bpo_apbvxTriangleVertices[iV1]->bvx_vAbsolute;
				FLOAT3D	vP2 = rBpo.bpo_apbvxTriangleVertices[iV2]->bvx_vAbsolute;

				FLOAT	fT;
				if( RayHitsTriangle( rOrig, rDir, vP0, vP1, vP2, &fT ) )
					return TRUE;
			}
		}
	}

	return FALSE;
}

#include <Engine/World/World.h>
#include <Engine/Models/ModelData.h>
#include <Engine/Models/ModelObject.h>
#include <Engine/Ska/ModelInstance.h>
#include <Engine/Templates/DynamicContainer.cpp>
void CBrushArchive::MakeAttributeMap( INDEX iType, UWORD *pAttrBuffer, INDEX iFloor, PIX pixWidth, PIX pixHeight, CWorld *pWorld, BOOL bField )
{
	// 속성맵에 쓰는 순서~!!!
	//{ _T("Walkable"), _T("Unwalkable"), _T("Stair"), _T("Wall of stair"), _T("Peace"), 
	//_T("Product Public"), _T("Product Private"), _T("War Area"), _T("Free PK") };

	// NOTE : 속성맵에 쓰는 순서 주의.
	// FIXME : 맘에 안듬!!!!
	switch( iType )
	{
	case 0:	// Walkable
		MakeWalkableAttributeMap( pAttrBuffer, iFloor, pixWidth, pixHeight, pWorld, bField );
		break;
	case 1:	// Unwalkable
		MakeUnWalkableAttributeMap( pAttrBuffer, iFloor, pixWidth, pixHeight, pWorld, bField );
		break;
	case 2:	// Stair
		MakeStairAttributeMap( pAttrBuffer, iFloor, pixWidth, pixHeight, pWorld, bField );
		break;
	//case 3:	// Wall of stair
	case 4:	// Peace
		MakePeaceAttributeMap( pAttrBuffer, iFloor, pixWidth, pixHeight, pWorld, bField );
		break;
	case 5:	// Product Public
		MakeProductPublicAttributeMap( pAttrBuffer, iFloor, pixWidth, pixHeight, pWorld, bField );
		break;
	case 6:	// Product Private
		MakeProductPrivateAttributeMap( pAttrBuffer, iFloor, pixWidth, pixHeight, pWorld, bField );	
		break;
	case 7:	// War Area
		MakeWarAttributeMap( pAttrBuffer, iFloor, pixWidth, pixHeight, pWorld, bField );	
		break;
	case 8:	// Free PK
		MakeFreePKAttributeMap( pAttrBuffer, iFloor, pixWidth, pixHeight, pWorld, bField );	
		break;
	default:
		break;
	};
}

// FIXME : 아래 함수들의 코드 중복이 너무 심각함.
void CBrushArchive::MakeWalkableAttributeMap( UWORD *pAttrBuffer, INDEX iFloor, PIX pixWidth, PIX pixHeight, CWorld *pWorld, BOOL bField )
{
	FLOATaabbox3D	aabBox;
	FLOAT			fMinX, fMaxX, fMinZ, fMaxZ, fMaxY;
	INDEX			iX, iZ, iSX, iEX, iSZ, iEZ;
	FLOAT3D			vOrig, vHit;
	FLOAT3D			vDir = FLOAT3D( 0, -1, 0 );
	FLOAT3D			vVertex0, vVertex1;
	UBYTE			ubPolygonAttr;
	UWORD			usAttr;
	CBrushPolygon	*pBpo;

	usAttr = MATT_WALKABLE;		// Walkable
	FOREACHINSTATICARRAY( ba_apbpo, CBrushPolygon *, itBpo )
	{
		pBpo = *itBpo;
		if( pBpo == NULL )
			continue;

		ubPolygonAttr = pBpo->bpo_ubPolygonAttribute;
		if( ubPolygonAttr == BPOA_NONE )
			continue;
		if( ubPolygonAttr > BPOA_WALKABLE5F || ( ubPolygonAttr % 10 ) != iFloor )
			continue;

		aabBox = pBpo->bpo_boxBoundingBox;
		vOrig = FLOAT3D( ( aabBox.minvect( 1 ) + aabBox.maxvect( 1 ) ) / 2,
							aabBox.maxvect( 2 ) + 1,
							( aabBox.minvect( 3 ) + aabBox.maxvect( 3 ) ) / 2 );
		FLOAT3D	vTarget = vOrig + vDir * 10000.0f;
		FLOAT	fDistance0 = pBpo->bpo_pbplPlane->bpl_plAbsolute.PointDistance( vOrig );
		FLOAT	fDistance1 = pBpo->bpo_pbplPlane->bpl_plAbsolute.PointDistance( vTarget );

		if( fDistance0 < 0 || fDistance0 < fDistance1 )
			continue;

		INDEX	iV0, iV1, iV2;
		INDEX	ctIndices = pBpo->bpo_aiTriangleElements.Count();
		for( INDEX iIndex = 0; iIndex < ctIndices; iIndex += 3 )
		{
			iV0 = pBpo->bpo_aiTriangleElements[iIndex];
			iV1 = pBpo->bpo_aiTriangleElements[iIndex + 1];
			iV2 = pBpo->bpo_aiTriangleElements[iIndex + 2];
			FLOAT3D	vP0 = pBpo->bpo_apbvxTriangleVertices[iV0]->bvx_vAbsolute;
			FLOAT3D	vP1 = pBpo->bpo_apbvxTriangleVertices[iV1]->bvx_vAbsolute;
			FLOAT3D	vP2 = pBpo->bpo_apbvxTriangleVertices[iV2]->bvx_vAbsolute;

			fMinX = fMaxX = vP0( 1 );
			fMinZ = fMaxZ = vP0( 3 );
			fMaxY = vP0( 2 );
			
			if( fMinX > vP1( 1 ) ) fMinX = vP1( 1 );
			else if( fMaxX < vP1( 1 ) ) fMaxX = vP1( 1 );
			if( fMinZ > vP1( 3 ) ) fMinZ = vP1( 3 );
			else if( fMaxZ < vP1( 3 ) ) fMaxZ = vP1( 3 );
			if( fMaxY < vP1( 2 ) ) fMaxY = vP1( 2 );

			if( fMinX > vP2( 1 ) ) fMinX = vP2( 1 );
			else if( fMaxX < vP2( 1 ) ) fMaxX = vP2( 1 );
			if( fMinZ > vP2( 3 ) ) fMinZ = vP2( 3 );
			else if( fMaxZ < vP2( 3 ) ) fMaxZ = vP2( 3 );
			if( fMaxY < vP2( 2 ) ) fMaxY = vP2( 2 );

			fMaxY += 1.0f;
			iSX = (INDEX)fMinX;		iEX = (INDEX)( fMaxX + 0.99999f );
			iSZ = (INDEX)fMinZ;		iEZ = (INDEX)( fMaxZ + 0.99999f );

			if (iEX > pixWidth)
				iEX = pixWidth;

			if (iEZ > pixHeight)
				iEZ = pixHeight;

			FLOAT	fT;
			for( iZ = iSZ; iZ < iEZ; iZ++ )
			{
				for( iX = iSX; iX < iEX; iX++ )
				{
					vOrig = FLOAT3D( iX, fMaxY, iZ );
					if( RayHitsTriangle( vOrig, vDir, vP0, vP1, vP2, &fT ) )
					{
						pAttrBuffer[iX + iZ * pixWidth] |= usAttr;
						pAttrBuffer[iX + iZ * pixWidth] &= ~(MATT_UNWALKABLE);
					}
					else
					{
						vOrig = FLOAT3D( iX + 1.0f, fMaxY, iZ );
						if( RayHitsTriangle( vOrig, vDir, vP0, vP1, vP2, &fT ) )
						{
							pAttrBuffer[iX + iZ * pixWidth] |= usAttr;
							pAttrBuffer[iX + iZ * pixWidth] &= ~(MATT_UNWALKABLE);
						}
						else
						{
							vOrig = FLOAT3D( iX, fMaxY, iZ + 1.0f );
							if( RayHitsTriangle( vOrig, vDir, vP0, vP1, vP2, &fT ) )
							{
								pAttrBuffer[iX + iZ * pixWidth] |= usAttr;
								pAttrBuffer[iX + iZ * pixWidth] &= ~(MATT_UNWALKABLE);
							}
							else
							{
								vOrig = FLOAT3D( iX + 1.0f, fMaxY, iZ + 1.0f );
								if( RayHitsTriangle( vOrig, vDir, vP0, vP1, vP2, &fT ) )
								{
									pAttrBuffer[iX + iZ * pixWidth] |= usAttr;
									pAttrBuffer[iX + iZ * pixWidth] &= ~(MATT_UNWALKABLE);
								}
								else
								{
									vOrig = FLOAT3D( iX + 0.5f, fMaxY, iZ + 0.5f );
									if( RayHitsTriangle( vOrig, vDir, vP0, vP1, vP2, &fT ) )
									{
										pAttrBuffer[iX + iZ * pixWidth] |= usAttr;
										pAttrBuffer[iX + iZ * pixWidth] &= ~(MATT_UNWALKABLE);
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

void CBrushArchive::MakePeaceAttributeMap( UWORD *pAttrBuffer, INDEX iFloor, PIX pixWidth, PIX pixHeight, CWorld *pWorld, BOOL bField )
{
	FLOATaabbox3D	aabBox;
	FLOAT			fMinX, fMaxX, fMinZ, fMaxZ, fMaxY;
	INDEX			iX, iZ, iSX, iEX, iSZ, iEZ;
	FLOAT3D			vOrig, vHit;
	FLOAT3D			vDir = FLOAT3D( 0, -1, 0 );
	FLOAT3D			vVertex0, vVertex1;
	UBYTE			ubPolygonAttr;
	UWORD			usAttr;
	CBrushPolygon	*pBpo;

	usAttr = MATT_PEACE;		// Peace zone
	FOREACHINSTATICARRAY( ba_apbpo, CBrushPolygon *, itBpo1 )
	{
		pBpo = *itBpo1;
		if( pBpo == NULL )
			continue;

		ubPolygonAttr = pBpo->bpo_ubPolygonAttribute;
		if( ubPolygonAttr < BPOA_PEACEZONE1F || ubPolygonAttr > BPOA_PEACEZONE5F )
			continue;
		if( ( ubPolygonAttr % 10 ) != iFloor )
			continue;

		aabBox = pBpo->bpo_boxBoundingBox;
		vOrig = FLOAT3D( ( aabBox.minvect( 1 ) + aabBox.maxvect( 1 ) ) / 2,
							aabBox.maxvect( 2 ) + 1,
							( aabBox.minvect( 3 ) + aabBox.maxvect( 3 ) ) / 2 );
		FLOAT3D	vTarget = vOrig + vDir * 10000.0f;
		FLOAT	fDistance0 = pBpo->bpo_pbplPlane->bpl_plAbsolute.PointDistance( vOrig );
		FLOAT	fDistance1 = pBpo->bpo_pbplPlane->bpl_plAbsolute.PointDistance( vTarget );

		if( fDistance0 < 0 || fDistance0 < fDistance1 )
			continue;

		INDEX	iV0, iV1, iV2;
		INDEX	ctIndices = pBpo->bpo_aiTriangleElements.Count();
		for( INDEX iIndex = 0; iIndex < ctIndices; iIndex += 3 )
		{
			iV0 = pBpo->bpo_aiTriangleElements[iIndex];
			iV1 = pBpo->bpo_aiTriangleElements[iIndex + 1];
			iV2 = pBpo->bpo_aiTriangleElements[iIndex + 2];
			FLOAT3D	vP0 = pBpo->bpo_apbvxTriangleVertices[iV0]->bvx_vAbsolute;
			FLOAT3D	vP1 = pBpo->bpo_apbvxTriangleVertices[iV1]->bvx_vAbsolute;
			FLOAT3D	vP2 = pBpo->bpo_apbvxTriangleVertices[iV2]->bvx_vAbsolute;

			fMinX = fMaxX = vP0( 1 );
			fMinZ = fMaxZ = vP0( 3 );
			fMaxY = vP0( 2 );
			
			if( fMinX > vP1( 1 ) ) fMinX = vP1( 1 );
			else if( fMaxX < vP1( 1 ) ) fMaxX = vP1( 1 );
			if( fMinZ > vP1( 3 ) ) fMinZ = vP1( 3 );
			else if( fMaxZ < vP1( 3 ) ) fMaxZ = vP1( 3 );
			if( fMaxY < vP1( 2 ) ) fMaxY = vP1( 2 );

			if( fMinX > vP2( 1 ) ) fMinX = vP2( 1 );
			else if( fMaxX < vP2( 1 ) ) fMaxX = vP2( 1 );
			if( fMinZ > vP2( 3 ) ) fMinZ = vP2( 3 );
			else if( fMaxZ < vP2( 3 ) ) fMaxZ = vP2( 3 );
			if( fMaxY < vP2( 2 ) ) fMaxY = vP2( 2 );

			fMaxY += 1.0f;
			iSX = (INDEX)fMinX;		iEX = (INDEX)( fMaxX + 0.99999f );
			iSZ = (INDEX)fMinZ;		iEZ = (INDEX)( fMaxZ + 0.99999f );

			if (iEX > pixWidth)
				iEX = pixWidth;

			if (iEZ > pixHeight)
				iEZ = pixHeight;

			FLOAT	fT;
			for( iZ = iSZ; iZ < iEZ; iZ++ )
			{
				for( iX = iSX; iX < iEX; iX++ )
				{
					vOrig = FLOAT3D( iX, fMaxY, iZ );
					if( RayHitsTriangle( vOrig, vDir, vP0, vP1, vP2, &fT ) )
					{
						//if( !bField || pAttrBuffer[iX + iZ * pixWidth] != ATTC_UNWALKABLE )
							pAttrBuffer[iX + iZ * pixWidth] |= usAttr;
					}
					else
					{
						vOrig = FLOAT3D( iX + 1.0f, fMaxY, iZ );
						if( RayHitsTriangle( vOrig, vDir, vP0, vP1, vP2, &fT ) )
						{
							//if( !bField || pAttrBuffer[iX + iZ * pixWidth] != ATTC_UNWALKABLE )
								pAttrBuffer[iX + iZ * pixWidth] |= usAttr;
						}
						else
						{
							vOrig = FLOAT3D( iX, fMaxY, iZ + 1.0f );
							if( RayHitsTriangle( vOrig, vDir, vP0, vP1, vP2, &fT ) )
							{
								//if( !bField || pAttrBuffer[iX + iZ * pixWidth] != ATTC_UNWALKABLE )
									pAttrBuffer[iX + iZ * pixWidth] |= usAttr;
							}
							else
							{
								vOrig = FLOAT3D( iX + 1.0f, fMaxY, iZ + 1.0f );
								if( RayHitsTriangle( vOrig, vDir, vP0, vP1, vP2, &fT ) )
								{
									//if( !bField || pAttrBuffer[iX + iZ * pixWidth] != ATTC_UNWALKABLE )
										pAttrBuffer[iX + iZ * pixWidth] |= usAttr;
								}
								else
								{
									vOrig = FLOAT3D( iX + 0.5f, fMaxY, iZ + 0.5f );
									if( RayHitsTriangle( vOrig, vDir, vP0, vP1, vP2, &fT ) )
									{
										//if( !bField || !(pAttrBuffer[iX + iZ * pixWidth] & MATT_UNWALKABLE) )
											pAttrBuffer[iX + iZ * pixWidth] |= usAttr;
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

void CBrushArchive::MakeProductPublicAttributeMap( UWORD *pAttrBuffer, INDEX iFloor, PIX pixWidth, PIX pixHeight, CWorld *pWorld, BOOL bField )
{
	FLOATaabbox3D	aabBox;
	FLOAT			fMinX, fMaxX, fMinZ, fMaxZ, fMaxY;
	INDEX			iX, iZ, iSX, iEX, iSZ, iEZ;
	FLOAT3D			vOrig, vHit;
	FLOAT3D			vDir = FLOAT3D( 0, -1, 0 );
	FLOAT3D			vVertex0, vVertex1;
	UBYTE			ubPolygonAttr;
	UWORD			usAttr;
	CBrushPolygon	*pBpo;

	usAttr = MATT_PRODUCT_PUBLIC;		// Production zone - public
	FOREACHINSTATICARRAY( ba_apbpo, CBrushPolygon *, itBpo2 )
	{
		pBpo = *itBpo2;
		if( pBpo == NULL )
			continue;

		ubPolygonAttr = pBpo->bpo_ubPolygonAttribute;
		if( ubPolygonAttr < BPOA_PRODUCTZONE_PUBLIC1F || ubPolygonAttr > BPOA_PRODUCTZONE_PUBLIC5F )
			continue;
		if( ( ubPolygonAttr % 10 ) != iFloor )
			continue;

		aabBox = pBpo->bpo_boxBoundingBox;
		vOrig = FLOAT3D( ( aabBox.minvect( 1 ) + aabBox.maxvect( 1 ) ) / 2,
							aabBox.maxvect( 2 ) + 1,
							( aabBox.minvect( 3 ) + aabBox.maxvect( 3 ) ) / 2 );
		FLOAT3D	vTarget = vOrig + vDir * 10000.0f;
		FLOAT	fDistance0 = pBpo->bpo_pbplPlane->bpl_plAbsolute.PointDistance( vOrig );
		FLOAT	fDistance1 = pBpo->bpo_pbplPlane->bpl_plAbsolute.PointDistance( vTarget );

		if( fDistance0 < 0 || fDistance0 < fDistance1 )
			continue;

		INDEX	iV0, iV1, iV2;
		INDEX	ctIndices = pBpo->bpo_aiTriangleElements.Count();
		for( INDEX iIndex = 0; iIndex < ctIndices; iIndex += 3 )
		{
			iV0 = pBpo->bpo_aiTriangleElements[iIndex];
			iV1 = pBpo->bpo_aiTriangleElements[iIndex + 1];
			iV2 = pBpo->bpo_aiTriangleElements[iIndex + 2];
			FLOAT3D	vP0 = pBpo->bpo_apbvxTriangleVertices[iV0]->bvx_vAbsolute;
			FLOAT3D	vP1 = pBpo->bpo_apbvxTriangleVertices[iV1]->bvx_vAbsolute;
			FLOAT3D	vP2 = pBpo->bpo_apbvxTriangleVertices[iV2]->bvx_vAbsolute;

			fMinX = fMaxX = vP0( 1 );
			fMinZ = fMaxZ = vP0( 3 );
			fMaxY = vP0( 2 );
			
			if( fMinX > vP1( 1 ) ) fMinX = vP1( 1 );
			else if( fMaxX < vP1( 1 ) ) fMaxX = vP1( 1 );
			if( fMinZ > vP1( 3 ) ) fMinZ = vP1( 3 );
			else if( fMaxZ < vP1( 3 ) ) fMaxZ = vP1( 3 );
			if( fMaxY < vP1( 2 ) ) fMaxY = vP1( 2 );

			if( fMinX > vP2( 1 ) ) fMinX = vP2( 1 );
			else if( fMaxX < vP2( 1 ) ) fMaxX = vP2( 1 );
			if( fMinZ > vP2( 3 ) ) fMinZ = vP2( 3 );
			else if( fMaxZ < vP2( 3 ) ) fMaxZ = vP2( 3 );
			if( fMaxY < vP2( 2 ) ) fMaxY = vP2( 2 );

			fMaxY += 1.0f;
			iSX = (INDEX)fMinX;		iEX = (INDEX)( fMaxX + 0.99999f );
			iSZ = (INDEX)fMinZ;		iEZ = (INDEX)( fMaxZ + 0.99999f );

			FLOAT	fT;
			for( iZ = iSZ; iZ < iEZ; iZ++ )
			{
				for( iX = iSX; iX < iEX; iX++ )
				{
					vOrig = FLOAT3D( iX, fMaxY, iZ );
					if( RayHitsTriangle( vOrig, vDir, vP0, vP1, vP2, &fT ) )
					{
						//if( !bField || pAttrBuffer[iX + iZ * pixWidth] != ATTC_UNWALKABLE )
							pAttrBuffer[iX + iZ * pixWidth] |= usAttr;
					}
					else
					{
						vOrig = FLOAT3D( iX + 1.0f, fMaxY, iZ );
						if( RayHitsTriangle( vOrig, vDir, vP0, vP1, vP2, &fT ) )
						{
							//if( !bField || pAttrBuffer[iX + iZ * pixWidth] != ATTC_UNWALKABLE )
								pAttrBuffer[iX + iZ * pixWidth] |= usAttr;
						}
						else
						{
							vOrig = FLOAT3D( iX, fMaxY, iZ + 1.0f );
							if( RayHitsTriangle( vOrig, vDir, vP0, vP1, vP2, &fT ) )
							{
								//if( !bField || pAttrBuffer[iX + iZ * pixWidth] != ATTC_UNWALKABLE )
									pAttrBuffer[iX + iZ * pixWidth] |= usAttr;
							}
							else
							{
								vOrig = FLOAT3D( iX + 1.0f, fMaxY, iZ + 1.0f );
								if( RayHitsTriangle( vOrig, vDir, vP0, vP1, vP2, &fT ) )
								{
									//if( !bField || pAttrBuffer[iX + iZ * pixWidth] != ATTC_UNWALKABLE )
										pAttrBuffer[iX + iZ * pixWidth] |= usAttr;
								}
								else
								{
									vOrig = FLOAT3D( iX + 0.5f, fMaxY, iZ + 0.5f );
									if( RayHitsTriangle( vOrig, vDir, vP0, vP1, vP2, &fT ) )
									{
										//if( !bField || pAttrBuffer[iX + iZ * pixWidth] != ATTC_UNWALKABLE )
											pAttrBuffer[iX + iZ * pixWidth] |= usAttr;
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

void CBrushArchive::MakeProductPrivateAttributeMap( UWORD *pAttrBuffer, INDEX iFloor, PIX pixWidth, PIX pixHeight, CWorld *pWorld, BOOL bField )
{
	FLOATaabbox3D	aabBox;
	FLOAT			fMinX, fMaxX, fMinZ, fMaxZ, fMaxY;
	INDEX			iX, iZ, iSX, iEX, iSZ, iEZ;
	FLOAT3D			vOrig, vHit;
	FLOAT3D			vDir = FLOAT3D( 0, -1, 0 );
	FLOAT3D			vVertex0, vVertex1;
	UBYTE			ubPolygonAttr;
	UWORD			usAttr;
	CBrushPolygon	*pBpo;

	usAttr = MATT_PRODUCT_PRIVATE;		// Production zone - private
	FOREACHINSTATICARRAY( ba_apbpo, CBrushPolygon *, itBpo3 )
	{
		pBpo = *itBpo3;
		if( pBpo == NULL )
			continue;

		ubPolygonAttr = pBpo->bpo_ubPolygonAttribute;
		if (ubPolygonAttr < BPOA_PRODUCTZONE_PRIVATE1F || ubPolygonAttr > BPOA_PRODUCTZONE_PRIVATE5F)
			continue;
		if( ( ubPolygonAttr % 10 ) != iFloor )
			continue;

		aabBox = pBpo->bpo_boxBoundingBox;
		vOrig = FLOAT3D( ( aabBox.minvect( 1 ) + aabBox.maxvect( 1 ) ) / 2,
							aabBox.maxvect( 2 ) + 1,
							( aabBox.minvect( 3 ) + aabBox.maxvect( 3 ) ) / 2 );
		FLOAT3D	vTarget = vOrig + vDir * 10000.0f;
		FLOAT	fDistance0 = pBpo->bpo_pbplPlane->bpl_plAbsolute.PointDistance( vOrig );
		FLOAT	fDistance1 = pBpo->bpo_pbplPlane->bpl_plAbsolute.PointDistance( vTarget );

		if( fDistance0 < 0 || fDistance0 < fDistance1 )
			continue;

		INDEX	iV0, iV1, iV2;
		INDEX	ctIndices = pBpo->bpo_aiTriangleElements.Count();
		for( INDEX iIndex = 0; iIndex < ctIndices; iIndex += 3 )
		{
			iV0 = pBpo->bpo_aiTriangleElements[iIndex];
			iV1 = pBpo->bpo_aiTriangleElements[iIndex + 1];
			iV2 = pBpo->bpo_aiTriangleElements[iIndex + 2];
			FLOAT3D	vP0 = pBpo->bpo_apbvxTriangleVertices[iV0]->bvx_vAbsolute;
			FLOAT3D	vP1 = pBpo->bpo_apbvxTriangleVertices[iV1]->bvx_vAbsolute;
			FLOAT3D	vP2 = pBpo->bpo_apbvxTriangleVertices[iV2]->bvx_vAbsolute;

			fMinX = fMaxX = vP0( 1 );
			fMinZ = fMaxZ = vP0( 3 );
			fMaxY = vP0( 2 );
			
			if( fMinX > vP1( 1 ) ) fMinX = vP1( 1 );
			else if( fMaxX < vP1( 1 ) ) fMaxX = vP1( 1 );
			if( fMinZ > vP1( 3 ) ) fMinZ = vP1( 3 );
			else if( fMaxZ < vP1( 3 ) ) fMaxZ = vP1( 3 );
			if( fMaxY < vP1( 2 ) ) fMaxY = vP1( 2 );

			if( fMinX > vP2( 1 ) ) fMinX = vP2( 1 );
			else if( fMaxX < vP2( 1 ) ) fMaxX = vP2( 1 );
			if( fMinZ > vP2( 3 ) ) fMinZ = vP2( 3 );
			else if( fMaxZ < vP2( 3 ) ) fMaxZ = vP2( 3 );
			if( fMaxY < vP2( 2 ) ) fMaxY = vP2( 2 );

			fMaxY += 1.0f;
			iSX = (INDEX)fMinX;		iEX = (INDEX)( fMaxX + 0.99999f );
			iSZ = (INDEX)fMinZ;		iEZ = (INDEX)( fMaxZ + 0.99999f );

			FLOAT	fT;
			for( iZ = iSZ; iZ < iEZ; iZ++ )
			{
				for( iX = iSX; iX < iEX; iX++ )
				{
					vOrig = FLOAT3D( iX, fMaxY, iZ );
					if( RayHitsTriangle( vOrig, vDir, vP0, vP1, vP2, &fT ) )
					{
						//if( !bField || pAttrBuffer[iX + iZ * pixWidth] != ATTC_UNWALKABLE )
							pAttrBuffer[iX + iZ * pixWidth] |= usAttr;
					}
					else
					{
						vOrig = FLOAT3D( iX + 1.0f, fMaxY, iZ );
						if( RayHitsTriangle( vOrig, vDir, vP0, vP1, vP2, &fT ) )
						{
							//if( !bField || pAttrBuffer[iX + iZ * pixWidth] != ATTC_UNWALKABLE )
								pAttrBuffer[iX + iZ * pixWidth] |= usAttr;
						}
						else
						{
							vOrig = FLOAT3D( iX, fMaxY, iZ + 1.0f );
							if( RayHitsTriangle( vOrig, vDir, vP0, vP1, vP2, &fT ) )
							{
								//if( !bField || pAttrBuffer[iX + iZ * pixWidth] != ATTC_UNWALKABLE )
									pAttrBuffer[iX + iZ * pixWidth] |= usAttr;
							}
							else
							{
								vOrig = FLOAT3D( iX + 1.0f, fMaxY, iZ + 1.0f );
								if( RayHitsTriangle( vOrig, vDir, vP0, vP1, vP2, &fT ) )
								{
									//if( !bField || pAttrBuffer[iX + iZ * pixWidth] != ATTC_UNWALKABLE )
										pAttrBuffer[iX + iZ * pixWidth] |= usAttr;
								}
								else
								{
									vOrig = FLOAT3D( iX + 0.5f, fMaxY, iZ + 0.5f );
									if( RayHitsTriangle( vOrig, vDir, vP0, vP1, vP2, &fT ) )
									{
										//if( !bField || pAttrBuffer[iX + iZ * pixWidth] != ATTC_UNWALKABLE )
											pAttrBuffer[iX + iZ * pixWidth] |= usAttr;
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

void CBrushArchive::MakeWarAttributeMap( UWORD *pAttrBuffer, INDEX iFloor, PIX pixWidth, PIX pixHeight, CWorld *pWorld, BOOL bField )
{
	FLOATaabbox3D	aabBox;
	FLOAT			fMinX, fMaxX, fMinZ, fMaxZ, fMaxY;
	INDEX			iX, iZ, iSX, iEX, iSZ, iEZ;
	FLOAT3D			vOrig, vHit;
	FLOAT3D			vDir = FLOAT3D( 0, -1, 0 );
	FLOAT3D			vVertex0, vVertex1;
	UBYTE			ubPolygonAttr;
	UWORD			usAttr;
	CBrushPolygon	*pBpo;

	usAttr = MATT_WAR;		// War Zone
	FOREACHINSTATICARRAY( ba_apbpo, CBrushPolygon *, itBpo7 )
	{
		pBpo = *itBpo7;
		if( pBpo == NULL )
			continue;

		ubPolygonAttr = pBpo->bpo_ubPolygonAttribute;
		if( ubPolygonAttr < BPOA__WAR1F || ubPolygonAttr > BPOA__WAR5F )
			continue;
		if( ( ubPolygonAttr % 10 ) != iFloor )
			continue;

		aabBox = pBpo->bpo_boxBoundingBox;
		vOrig = FLOAT3D( ( aabBox.minvect( 1 ) + aabBox.maxvect( 1 ) ) / 2,
							aabBox.maxvect( 2 ) + 1,
							( aabBox.minvect( 3 ) + aabBox.maxvect( 3 ) ) / 2 );
		FLOAT3D	vTarget = vOrig + vDir * 10000.0f;
		FLOAT	fDistance0 = pBpo->bpo_pbplPlane->bpl_plAbsolute.PointDistance( vOrig );
		FLOAT	fDistance1 = pBpo->bpo_pbplPlane->bpl_plAbsolute.PointDistance( vTarget );

		if( fDistance0 < 0 || fDistance0 < fDistance1 )
			continue;

		INDEX	iV0, iV1, iV2;
		INDEX	ctIndices = pBpo->bpo_aiTriangleElements.Count();
		for( INDEX iIndex = 0; iIndex < ctIndices; iIndex += 3 )
		{
			iV0 = pBpo->bpo_aiTriangleElements[iIndex];
			iV1 = pBpo->bpo_aiTriangleElements[iIndex + 1];
			iV2 = pBpo->bpo_aiTriangleElements[iIndex + 2];
			FLOAT3D	vP0 = pBpo->bpo_apbvxTriangleVertices[iV0]->bvx_vAbsolute;
			FLOAT3D	vP1 = pBpo->bpo_apbvxTriangleVertices[iV1]->bvx_vAbsolute;
			FLOAT3D	vP2 = pBpo->bpo_apbvxTriangleVertices[iV2]->bvx_vAbsolute;

			fMinX = fMaxX = vP0( 1 );
			fMinZ = fMaxZ = vP0( 3 );
			fMaxY = vP0( 2 );
			
			if( fMinX > vP1( 1 ) ) fMinX = vP1( 1 );
			else if( fMaxX < vP1( 1 ) ) fMaxX = vP1( 1 );
			if( fMinZ > vP1( 3 ) ) fMinZ = vP1( 3 );
			else if( fMaxZ < vP1( 3 ) ) fMaxZ = vP1( 3 );
			if( fMaxY < vP1( 2 ) ) fMaxY = vP1( 2 );

			if( fMinX > vP2( 1 ) ) fMinX = vP2( 1 );
			else if( fMaxX < vP2( 1 ) ) fMaxX = vP2( 1 );
			if( fMinZ > vP2( 3 ) ) fMinZ = vP2( 3 );
			else if( fMaxZ < vP2( 3 ) ) fMaxZ = vP2( 3 );
			if( fMaxY < vP2( 2 ) ) fMaxY = vP2( 2 );

			fMaxY += 1.0f;
			iSX = (INDEX)fMinX;		iEX = (INDEX)( fMaxX + 0.99999f );
			iSZ = (INDEX)fMinZ;		iEZ = (INDEX)( fMaxZ + 0.99999f );

			FLOAT	fT;
			for( iZ = iSZ; iZ < iEZ; iZ++ )
			{
				for( iX = iSX; iX < iEX; iX++ )
				{
					vOrig = FLOAT3D( iX, fMaxY, iZ );
					if( RayHitsTriangle( vOrig, vDir, vP0, vP1, vP2, &fT ) )
					{
						//if( !bField || pAttrBuffer[iX + iZ * pixWidth] != ATTC_UNWALKABLE )
							pAttrBuffer[iX + iZ * pixWidth] |= usAttr;
					}
					else
					{
						vOrig = FLOAT3D( iX + 1.0f, fMaxY, iZ );
						if( RayHitsTriangle( vOrig, vDir, vP0, vP1, vP2, &fT ) )
						{
							//if( !bField || pAttrBuffer[iX + iZ * pixWidth] != ATTC_UNWALKABLE )
								pAttrBuffer[iX + iZ * pixWidth] |= usAttr;
						}
						else
						{
							vOrig = FLOAT3D( iX, fMaxY, iZ + 1.0f );
							if( RayHitsTriangle( vOrig, vDir, vP0, vP1, vP2, &fT ) )
							{
								//if( !bField || pAttrBuffer[iX + iZ * pixWidth] != ATTC_UNWALKABLE )
									pAttrBuffer[iX + iZ * pixWidth] |= usAttr;
							}
							else
							{
								vOrig = FLOAT3D( iX + 1.0f, fMaxY, iZ + 1.0f );
								if( RayHitsTriangle( vOrig, vDir, vP0, vP1, vP2, &fT ) )
								{
									//if( !bField || pAttrBuffer[iX + iZ * pixWidth] != ATTC_UNWALKABLE )
										pAttrBuffer[iX + iZ * pixWidth] |= usAttr;
								}
								else
								{
									vOrig = FLOAT3D( iX + 0.5f, fMaxY, iZ + 0.5f );
									if( RayHitsTriangle( vOrig, vDir, vP0, vP1, vP2, &fT ) )
									{
										//if( !bField || pAttrBuffer[iX + iZ * pixWidth] != ATTC_UNWALKABLE )
											pAttrBuffer[iX + iZ * pixWidth] |= usAttr;
									}
								}
							}
						}
					}
				}
			} 
		}
	}
}

void CBrushArchive::MakeFreePKAttributeMap( UWORD *pAttrBuffer, INDEX iFloor, PIX pixWidth, PIX pixHeight, CWorld *pWorld, BOOL bField )
{
	FLOATaabbox3D	aabBox;
	FLOAT			fMinX, fMaxX, fMinZ, fMaxZ, fMaxY;
	INDEX			iX, iZ, iSX, iEX, iSZ, iEZ;
	FLOAT3D			vOrig, vHit;
	FLOAT3D			vDir = FLOAT3D( 0, -1, 0 );
	FLOAT3D			vVertex0, vVertex1;	
	UBYTE			ubPolygonAttr;
	UWORD			usAttr;
	CBrushPolygon	*pBpo;
	// Free PK Zone
	usAttr = MATT_FREEPKZONE;		// Free PK Zone
	FOREACHINSTATICARRAY( ba_apbpo, CBrushPolygon *, itBpo8 )
	{
		pBpo = *itBpo8;
		if( pBpo == NULL )
			continue;

		ubPolygonAttr = pBpo->bpo_ubPolygonAttribute;
		if( ubPolygonAttr < BPOA__FREEPKZONE1F || ubPolygonAttr > BPOA__FREEPKZONE5F )
			continue;
		if( ( ubPolygonAttr % 10 ) != iFloor )
			continue;

		aabBox = pBpo->bpo_boxBoundingBox;
		vOrig = FLOAT3D( ( aabBox.minvect( 1 ) + aabBox.maxvect( 1 ) ) / 2,
							aabBox.maxvect( 2 ) + 1,
							( aabBox.minvect( 3 ) + aabBox.maxvect( 3 ) ) / 2 );
		FLOAT3D	vTarget = vOrig + vDir * 10000.0f;
		FLOAT	fDistance0 = pBpo->bpo_pbplPlane->bpl_plAbsolute.PointDistance( vOrig );
		FLOAT	fDistance1 = pBpo->bpo_pbplPlane->bpl_plAbsolute.PointDistance( vTarget );

		if( fDistance0 < 0 || fDistance0 < fDistance1 )
			continue;

		INDEX	iV0, iV1, iV2;
		INDEX	ctIndices = pBpo->bpo_aiTriangleElements.Count();
		for( INDEX iIndex = 0; iIndex < ctIndices; iIndex += 3 )
		{
			iV0 = pBpo->bpo_aiTriangleElements[iIndex];
			iV1 = pBpo->bpo_aiTriangleElements[iIndex + 1];
			iV2 = pBpo->bpo_aiTriangleElements[iIndex + 2];
			FLOAT3D	vP0 = pBpo->bpo_apbvxTriangleVertices[iV0]->bvx_vAbsolute;
			FLOAT3D	vP1 = pBpo->bpo_apbvxTriangleVertices[iV1]->bvx_vAbsolute;
			FLOAT3D	vP2 = pBpo->bpo_apbvxTriangleVertices[iV2]->bvx_vAbsolute;

			fMinX = fMaxX = vP0( 1 );
			fMinZ = fMaxZ = vP0( 3 );
			fMaxY = vP0( 2 );
			
			if( fMinX > vP1( 1 ) ) fMinX = vP1( 1 );
			else if( fMaxX < vP1( 1 ) ) fMaxX = vP1( 1 );
			if( fMinZ > vP1( 3 ) ) fMinZ = vP1( 3 );
			else if( fMaxZ < vP1( 3 ) ) fMaxZ = vP1( 3 );
			if( fMaxY < vP1( 2 ) ) fMaxY = vP1( 2 );

			if( fMinX > vP2( 1 ) ) fMinX = vP2( 1 );
			else if( fMaxX < vP2( 1 ) ) fMaxX = vP2( 1 );
			if( fMinZ > vP2( 3 ) ) fMinZ = vP2( 3 );
			else if( fMaxZ < vP2( 3 ) ) fMaxZ = vP2( 3 );
			if( fMaxY < vP2( 2 ) ) fMaxY = vP2( 2 );

			fMaxY += 1.0f;
			iSX = (INDEX)fMinX;		iEX = (INDEX)( fMaxX + 0.99999f );
			iSZ = (INDEX)fMinZ;		iEZ = (INDEX)( fMaxZ + 0.99999f );

			FLOAT	fT;
			for( iZ = iSZ; iZ < iEZ; iZ++ )
			{
				for( iX = iSX; iX < iEX; iX++ )
				{
					vOrig = FLOAT3D( iX, fMaxY, iZ );
					if( RayHitsTriangle( vOrig, vDir, vP0, vP1, vP2, &fT ) )
					{
						//if( !bField || pAttrBuffer[iX + iZ * pixWidth] != ATTC_UNWALKABLE )
							pAttrBuffer[iX + iZ * pixWidth] |= usAttr;
							pAttrBuffer[iX + iZ * pixWidth] &= ~MATT_PEACE;
					}
					else
					{
						vOrig = FLOAT3D( iX + 1.0f, fMaxY, iZ );
						if( RayHitsTriangle( vOrig, vDir, vP0, vP1, vP2, &fT ) )
						{
							//if( !bField || pAttrBuffer[iX + iZ * pixWidth] != ATTC_UNWALKABLE )
								pAttrBuffer[iX + iZ * pixWidth] |= usAttr;
								pAttrBuffer[iX + iZ * pixWidth] &= ~MATT_PEACE;
						}
						else
						{
							vOrig = FLOAT3D( iX, fMaxY, iZ + 1.0f );
							if( RayHitsTriangle( vOrig, vDir, vP0, vP1, vP2, &fT ) )
							{
								//if( !bField || pAttrBuffer[iX + iZ * pixWidth] != ATTC_UNWALKABLE )
									pAttrBuffer[iX + iZ * pixWidth] |= usAttr;
									pAttrBuffer[iX + iZ * pixWidth] &= ~MATT_PEACE;
							}
							else
							{
								vOrig = FLOAT3D( iX + 1.0f, fMaxY, iZ + 1.0f );
								if( RayHitsTriangle( vOrig, vDir, vP0, vP1, vP2, &fT ) )
								{
									//if( !bField || pAttrBuffer[iX + iZ * pixWidth] != ATTC_UNWALKABLE )
										pAttrBuffer[iX + iZ * pixWidth] |= usAttr;
										pAttrBuffer[iX + iZ * pixWidth] &= ~MATT_PEACE;
								}
								else
								{
									vOrig = FLOAT3D( iX + 0.5f, fMaxY, iZ + 0.5f );
									if( RayHitsTriangle( vOrig, vDir, vP0, vP1, vP2, &fT ) )
									{
										//if( !bField || pAttrBuffer[iX + iZ * pixWidth] != ATTC_UNWALKABLE )
											pAttrBuffer[iX + iZ * pixWidth] |= usAttr;
											pAttrBuffer[iX + iZ * pixWidth] &= ~MATT_PEACE;
									}
								}
							}
						}
					}
				}
			} 
		}
	}
}

void CBrushArchive::MakeStairAttributeMap( UWORD *pAttrBuffer, INDEX iFloor, PIX pixWidth, PIX pixHeight, CWorld *pWorld, BOOL bField )
{
	FLOATaabbox3D	aabBox;
	FLOAT			fMinX, fMaxX, fMinZ, fMaxZ, fMaxY;
	INDEX			iX, iZ, iSX, iEX, iSZ, iEZ;
	FLOAT3D			vOrig, vHit;
	FLOAT3D			vDir = FLOAT3D( 0, -1, 0 );
	FLOAT3D			vVertex0, vVertex1;
	UBYTE			ubPolygonAttr;
	UWORD			usAttr;
	CBrushPolygon	*pBpo;

	// Stair
	FOREACHINSTATICARRAY( ba_apbpo, CBrushPolygon *, itBpo4 )
	{
		pBpo = *itBpo4;
		if( pBpo == NULL )
			continue;

		ubPolygonAttr = pBpo->bpo_ubPolygonAttribute;
		if( ubPolygonAttr == BPOA_NONE )
			continue;
		if( ubPolygonAttr < BPOA_STAIR1F2F ||  ubPolygonAttr > BPOA_STAIR4F5F )
			continue;
		if( ubPolygonAttr != ( iFloor + 19 ) && ubPolygonAttr != ( iFloor + 20 ) )
			continue;

		if( ubPolygonAttr == ( iFloor + 19 ) )
			usAttr = MATT_STAIR_DOWN;		// Down stair
		else
			usAttr = MATT_STAIR_UP;			// Up stair

		aabBox = pBpo->bpo_boxBoundingBox;
		vOrig = FLOAT3D( ( aabBox.minvect( 1 ) + aabBox.maxvect( 1 ) ) / 2,
							aabBox.maxvect( 2 ) + 1,
							( aabBox.minvect( 3 ) + aabBox.maxvect( 3 ) ) / 2 );
		FLOAT3D	vTarget = vOrig + vDir * 10000.0f;
		FLOAT	fDistance0 = pBpo->bpo_pbplPlane->bpl_plAbsolute.PointDistance( vOrig );
		FLOAT	fDistance1 = pBpo->bpo_pbplPlane->bpl_plAbsolute.PointDistance( vTarget );

		if( fDistance0 < 0 || fDistance0 < fDistance1 )
			continue;

		INDEX	iV0, iV1, iV2;
		INDEX	ctIndices = pBpo->bpo_aiTriangleElements.Count();
		for( INDEX iIndex = 0; iIndex < ctIndices; iIndex += 3 )
		{
			iV0 = pBpo->bpo_aiTriangleElements[iIndex];
			iV1 = pBpo->bpo_aiTriangleElements[iIndex + 1];
			iV2 = pBpo->bpo_aiTriangleElements[iIndex + 2];
			FLOAT3D	vP0 = pBpo->bpo_apbvxTriangleVertices[iV0]->bvx_vAbsolute;
			FLOAT3D	vP1 = pBpo->bpo_apbvxTriangleVertices[iV1]->bvx_vAbsolute;
			FLOAT3D	vP2 = pBpo->bpo_apbvxTriangleVertices[iV2]->bvx_vAbsolute;

			fMinX = fMaxX = vP0( 1 );
			fMinZ = fMaxZ = vP0( 3 );
			fMaxY = vP0( 2 );
			
			if( fMinX > vP1( 1 ) ) fMinX = vP1( 1 );
			else if( fMaxX < vP1( 1 ) ) fMaxX = vP1( 1 );
			if( fMinZ > vP1( 3 ) ) fMinZ = vP1( 3 );
			else if( fMaxZ < vP1( 3 ) ) fMaxZ = vP1( 3 );
			if( fMaxY < vP1( 2 ) ) fMaxY = vP1( 2 );

			if( fMinX > vP2( 1 ) ) fMinX = vP2( 1 );
			else if( fMaxX < vP2( 1 ) ) fMaxX = vP2( 1 );
			if( fMinZ > vP2( 3 ) ) fMinZ = vP2( 3 );
			else if( fMaxZ < vP2( 3 ) ) fMaxZ = vP2( 3 );
			if( fMaxY < vP2( 2 ) ) fMaxY = vP2( 2 );

			fMaxY += 1.0f;
			iSX = (INDEX)fMinX;		iEX = (INDEX)( fMaxX + 0.99999f );
			iSZ = (INDEX)fMinZ;		iEZ = (INDEX)( fMaxZ + 0.99999f );

			FLOAT	fT;
			for( iZ = iSZ; iZ < iEZ; iZ++ )
			{
				for( iX = iSX; iX < iEX; iX++ )
				{
					vOrig = FLOAT3D( iX, fMaxY, iZ );
					if( RayHitsTriangle( vOrig, vDir, vP0, vP1, vP2, &fT ) )
					{
						//if( !bField || pAttrBuffer[iX + iZ * pixWidth] != ATTC_UNWALKABLE )
							pAttrBuffer[iX + iZ * pixWidth] |= usAttr;
					}
					else
					{
						vOrig = FLOAT3D( iX + 1.0f, fMaxY, iZ );
						if( RayHitsTriangle( vOrig, vDir, vP0, vP1, vP2, &fT ) )
						{
							//if( !bField || pAttrBuffer[iX + iZ * pixWidth] != ATTC_UNWALKABLE )
								pAttrBuffer[iX + iZ * pixWidth] |= usAttr;
						}
						else
						{
							vOrig = FLOAT3D( iX, fMaxY, iZ + 1.0f );
							if( RayHitsTriangle( vOrig, vDir, vP0, vP1, vP2, &fT ) )
							{
								//if( !bField || pAttrBuffer[iX + iZ * pixWidth] != ATTC_UNWALKABLE )
									pAttrBuffer[iX + iZ * pixWidth] |= usAttr;
							}
							else
							{
								vOrig = FLOAT3D( iX + 1.0f, fMaxY, iZ + 1.0f );
								if( RayHitsTriangle( vOrig, vDir, vP0, vP1, vP2, &fT ) )
								{
									//if( !bField || pAttrBuffer[iX + iZ * pixWidth] != ATTC_UNWALKABLE )
										pAttrBuffer[iX + iZ * pixWidth] |= usAttr;
								}
								else
								{
									vOrig = FLOAT3D( iX + 0.5f, fMaxY, iZ + 0.5f );
									if( RayHitsTriangle( vOrig, vDir, vP0, vP1, vP2, &fT ) )
									{
										if( !bField || pAttrBuffer[iX + iZ * pixWidth] != MATT_UNWALKABLE )
											pAttrBuffer[iX + iZ * pixWidth] = usAttr;
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

void CBrushArchive::MakeUnWalkableAttributeMap( UWORD *pAttrBuffer, INDEX iFloor, PIX pixWidth, PIX pixHeight, CWorld *pWorld, BOOL bField )
{
	FLOATaabbox3D	aabBox;
	FLOAT3D			vOrig, vHit;
	FLOAT3D			vDir = FLOAT3D( 0, -1, 0 );
	FLOAT3D			vVertex0, vVertex1;
	POINT			ptStart[255], ptEnd[255];
	INDEX			iPoints, iVert;
	UBYTE			ubPolygonAttr;
	UWORD			usAttr;
	CBrushPolygon	*pBpo;

	usAttr = MATT_UNWALKABLE;		// Unwalkable
	FOREACHINSTATICARRAY( ba_apbpo, CBrushPolygon *, itBpo5 )
	{
		pBpo = *itBpo5;
		if( pBpo == NULL )
			continue;

		ubPolygonAttr = pBpo->bpo_ubPolygonAttribute;
		if( ubPolygonAttr == BPOA_NONE )
			continue;
		if( ubPolygonAttr < BPOA_UNWALKABLE1F || ubPolygonAttr > BPOA_UNWALKABLE5F )
			continue;
		if( ( ubPolygonAttr % 10 ) != iFloor )
			continue;

		INDEX	ctTri = pBpo->bpo_aiTriangleElements.Count();
		for( INDEX iTri = 0; iTri < ctTri; iTri += 3 )
		{
			iPoints = 0;
			iVert = pBpo->bpo_aiTriangleElements[iTri];
			vVertex0 = pBpo->bpo_apbvxTriangleVertices[iVert]->bvx_vAbsolute;

			for( INDEX iTriEle = iTri + 1; iTriEle < iTri + 3; iTriEle++ )
			{
				iVert = pBpo->bpo_aiTriangleElements[iTriEle];
				vVertex1 = pBpo->bpo_apbvxTriangleVertices[iVert]->bvx_vAbsolute;

				ptStart[iPoints].x = vVertex0( 1 );
				ptStart[iPoints].y = vVertex0( 3 );
				ptEnd[iPoints].x = vVertex1( 1 );
				ptEnd[iPoints++].y = vVertex1( 3 );

				vVertex0 = vVertex1;				
			}

			iVert = pBpo->bpo_aiTriangleElements[iTri];
			vVertex1 = pBpo->bpo_apbvxTriangleVertices[iVert]->bvx_vAbsolute;

			ptStart[iPoints].x = vVertex0( 1 );
			ptStart[iPoints].y = vVertex0( 3 );
			ptEnd[iPoints].x = vVertex1( 1 );
			ptEnd[iPoints++].y = vVertex1( 3 );

			BR_DrawAttributeMap( ptStart, ptEnd, iPoints, pAttrBuffer, pixWidth, pixHeight, usAttr );
		}
	}

	usAttr = MATT_UNWALKABLE;		// Wall of stair
	FOREACHINSTATICARRAY( ba_apbpo, CBrushPolygon *, itBpo6 )
	{
		pBpo = *itBpo6;
		if( pBpo == NULL )
			continue;

		ubPolygonAttr = pBpo->bpo_ubPolygonAttribute;
		if( ubPolygonAttr == BPOA_NONE )
			continue;
		if( ubPolygonAttr < BPOA_STAIRWALL1F2F || ubPolygonAttr > BPOA_STAIRWALL4F5F )
			continue;
		if( ubPolygonAttr != ( iFloor + 29 ) && ubPolygonAttr != ( iFloor + 30 ) )
			continue;

		INDEX	ctTri = pBpo->bpo_aiTriangleElements.Count();
		for( INDEX iTri = 0; iTri < ctTri; iTri += 3 )
		{
			iPoints = 0;
			iVert = pBpo->bpo_aiTriangleElements[iTri];
			vVertex0 = pBpo->bpo_apbvxTriangleVertices[iVert]->bvx_vAbsolute;

			for( INDEX iTriEle = iTri + 1; iTriEle < iTri + 3; iTriEle++ )
			{
				iVert = pBpo->bpo_aiTriangleElements[iTriEle];
				vVertex1 = pBpo->bpo_apbvxTriangleVertices[iVert]->bvx_vAbsolute;

				ptStart[iPoints].x = vVertex0( 1 );
				ptStart[iPoints].y = vVertex0( 3 );
				ptEnd[iPoints].x = vVertex1( 1 );
				ptEnd[iPoints++].y = vVertex1( 3 );

				vVertex0 = vVertex1;				
			}

			iVert = pBpo->bpo_aiTriangleElements[iTri];
			vVertex1 = pBpo->bpo_apbvxTriangleVertices[iVert]->bvx_vAbsolute;

			ptStart[iPoints].x = vVertex0( 1 );
			ptStart[iPoints].y = vVertex0( 3 );
			ptEnd[iPoints].x = vVertex1( 1 );
			ptEnd[iPoints++].y = vVertex1( 3 );

			BR_DrawAttributeMap( ptStart, ptEnd, iPoints, pAttrBuffer, pixWidth, pixHeight, usAttr );
		}
	}

	// Each models...
	usAttr = MATT_UNWALKABLE;
	FOREACHINDYNAMICCONTAINER( pWorld->wo_cenAllEntities, CEntity, iten )
	{
		CEntity	*penEntity = iten;

		if( penEntity->en_RenderType != CEntity::RT_MODEL && penEntity->en_RenderType != CEntity::RT_SKAMODEL )
			continue;

		// Skip
		if( ( penEntity->en_ulFlags & ENF_BACKGROUND ) ||
			( penEntity->en_ulFlags & ENF_INVISIBLE ) ||
			( penEntity->en_ulCollisionFlags == 0 ) )
			continue;

		FLOAT3D	vMin, vMax, vStretch;
		FLOAT3D	vOrigin;
		// If the entity is a mdl model...
		if( penEntity->en_RenderType == CEntity::RT_MODEL )
		{
			CModelObject	*pmo = penEntity->en_pmoModelObject;
			vStretch = pmo->mo_Stretch;
			FOREACHINDYNAMICARRAY( pmo->GetData()->md_acbCollisionBox, CModelCollisionBox, itcb )
			{
				vMin = itcb->mcb_vCollisionBoxMin;
				vMax = itcb->mcb_vCollisionBoxMax;

				FLOATaabbox3D	boxModel = FLOATaabbox3D( vMin, vMax );
				boxModel.StretchByVector( pmo->mo_Stretch );
				vOrigin = boxModel.Center();
				break;
			}
		}
		// If the entity is a ska model...
		else
		{
			CModelInstance	*pmi = penEntity->en_pmiModelInstance;
			vStretch = pmi->mi_vStretch;
			vMin = pmi->GetCollisionBoxMin( 0 );
			vMax = pmi->GetCollisionBoxMax( 0 );

			FLOATaabbox3D	boxModel = FLOATaabbox3D( vMin, vMax );
			boxModel.StretchByVector( pmi->mi_vStretch );
			vOrigin = boxModel.Center();
		}

		// Get real size of bounding box
		vMin( 1 ) *= vStretch( 1 );
		vMin( 2 ) *= vStretch( 2 );
		vMin( 3 ) *= vStretch( 3 );
		vMax( 1 ) *= vStretch( 1 );
		vMax( 2 ) *= vStretch( 2 );
		vMax( 3 ) *= vStretch( 3 );

		// Translation & rotation
		FLOAT3D			vTranslation = penEntity->en_plPlacement.pl_PositionVector;
		FLOATmatrix3D	matRotation = penEntity->en_mRotation;
		FLOAT3D			vLB = FLOAT3D( vMin( 1 ), vMin( 2 ), vMin( 3 ) );
		FLOAT3D			vLT = FLOAT3D( vMin( 1 ), vMin( 2 ), vMax( 3 ) );
		FLOAT3D			vRT = FLOAT3D( vMax( 1 ), vMin( 2 ), vMax( 3 ) );
		FLOAT3D			vRB = FLOAT3D( vMax( 1 ), vMin( 2 ), vMin( 3 ) );
		vLB = vTranslation + vLB * matRotation;
		vLT = vTranslation + vLT * matRotation;
		vRT = vTranslation + vRT * matRotation;
		vRB = vTranslation + vRB * matRotation;

		// Get orgin of ray
		vOrigin *= penEntity->en_mRotation;
		vOrigin += penEntity->en_plPlacement.pl_PositionVector;
		FLOAT3D	vDir = FLOAT3D( 0, -1, 0 );

		// If valid polygon is found
		FLOAT	fMaxDist = ( vMax( 2 ) - vMin( 2 ) ) / 2.0f + 1.0f;
		if( ba_apbsc.Count() == 0 )
		{
			if( !BR_FindBelowPolygonByAllPolygon( ba_apbpo, vOrigin, vDir, iFloor, fMaxDist ) )
				continue;
		}
		else
		{
			if( !BR_FindBelowPolygonBySector( ba_apbsc, vOrigin, vDir, iFloor, fMaxDist ) )
				continue;
		}

		// Get points
		ptEnd[3].x = ptStart[0].x = vLB( 1 );
		ptEnd[3].y = ptStart[0].y = vLB( 3 );
		ptEnd[0].x = ptStart[1].x = vLT( 1 );
		ptEnd[0].y = ptStart[1].y = vLT( 3 );
		ptEnd[1].x = ptStart[2].x = vRT( 1 );
		ptEnd[1].y = ptStart[2].y = vRT( 3 );
		ptEnd[2].x = ptStart[3].x = vRB( 1 );
		ptEnd[2].y = ptStart[3].y = vRB( 3 );

		// Draw attribute map
		BR_DrawAttributeMap( ptStart, ptEnd, 4, pAttrBuffer, pixWidth, pixHeight, usAttr );
	}
}

void CBrushArchive::MakeSvrHeightMap( UWORD *pHeightBuffer, INDEX iFloor, PIX pixWidth, PIX pixHeight, BOOL bField )
{
	FLOATaabbox3D	aabBox;
	FLOAT			fMinX, fMaxX, fMinZ, fMaxZ, fMaxY;
	INDEX			iX, iZ, iSX, iEX, iSZ, iEZ;
	FLOAT3D			vOrig, vHit;
	FLOAT3D			vDir = FLOAT3D( 0, -1, 0 );
	UWORD			uwTempHeight, uwTempHeight2, uwHeight;

	// Walkable
	CBrushSector	*pBpsc;
	FOREACHINSTATICARRAY( ba_apbsc, CBrushSector *, itBpsc )
	{
		pBpsc = *itBpsc;
		if( pBpsc == NULL )
			continue;
		if( pBpsc->bsc_ulFlags & BSCF_HIDDEN )
			continue;
		if( pBpsc->bsc_pbmBrushMip->bm_pbrBrush->br_penEntity->en_ulFlags & ENF_HIDDEN )
			continue;

		UBYTE			ubPolygonAttr;
		FOREACHINSTATICARRAY( pBpsc->bsc_abpoPolygons, CBrushPolygon, itBpo )
		{
			CBrushPolygon	&rBpo = itBpo.Current();

			ubPolygonAttr = rBpo.bpo_ubPolygonAttribute;
			if( ubPolygonAttr == BPOA_NONE )
				continue;
			if( ubPolygonAttr > BPOA_WALKABLE5F || ( ubPolygonAttr % 10 ) != iFloor )
				continue;

			aabBox = rBpo.bpo_boxBoundingBox;
			vOrig = FLOAT3D( ( aabBox.minvect( 1 ) + aabBox.maxvect( 1 ) ) / 2,
								aabBox.maxvect( 2 ) + 1,
								( aabBox.minvect( 3 ) + aabBox.maxvect( 3 ) ) / 2 );
			FLOAT3D	vTarget = vOrig + vDir * 10000.0f;
			FLOAT	fDistance0 = rBpo.bpo_pbplPlane->bpl_plAbsolute.PointDistance( vOrig );
			FLOAT	fDistance1 = rBpo.bpo_pbplPlane->bpl_plAbsolute.PointDistance( vTarget );

			if( fDistance0 < 0 || fDistance0 < fDistance1 )
				continue;

			INDEX	iV0, iV1, iV2;
			INDEX	ctIndices = rBpo.bpo_aiTriangleElements.Count();
			for( INDEX iIndex = 0; iIndex < ctIndices; iIndex += 3 )
			{
				iV0 = rBpo.bpo_aiTriangleElements[iIndex];
				iV1 = rBpo.bpo_aiTriangleElements[iIndex + 1];
				iV2 = rBpo.bpo_aiTriangleElements[iIndex + 2];
				FLOAT3D	vP0 = rBpo.bpo_apbvxTriangleVertices[iV0]->bvx_vAbsolute;
				FLOAT3D	vP1 = rBpo.bpo_apbvxTriangleVertices[iV1]->bvx_vAbsolute;
				FLOAT3D	vP2 = rBpo.bpo_apbvxTriangleVertices[iV2]->bvx_vAbsolute;

				fMinX = fMaxX = vP0( 1 );
				fMinZ = fMaxZ = vP0( 3 );
				fMaxY = vP0( 2 );
				
				if( fMinX > vP1( 1 ) ) fMinX = vP1( 1 );
				else if( fMaxX < vP1( 1 ) ) fMaxX = vP1( 1 );
				if( fMinZ > vP1( 3 ) ) fMinZ = vP1( 3 );
				else if( fMaxZ < vP1( 3 ) ) fMaxZ = vP1( 3 );
				if( fMaxY < vP1( 2 ) ) fMaxY = vP1( 2 );

				if( fMinX > vP2( 1 ) ) fMinX = vP2( 1 );
				else if( fMaxX < vP2( 1 ) ) fMaxX = vP2( 1 );
				if( fMinZ > vP2( 3 ) ) fMinZ = vP2( 3 );
				else if( fMaxZ < vP2( 3 ) ) fMaxZ = vP2( 3 );
				if( fMaxY < vP2( 2 ) ) fMaxY = vP2( 2 );

				fMaxY += 1.0f;
				iSX = (INDEX)fMinX;		iEX = (INDEX)( fMaxX + 0.99999f );
				iSZ = (INDEX)fMinZ;		iEZ = (INDEX)( fMaxZ + 0.99999f );

				FLOAT	fT;
				for( iZ = iSZ; iZ < iEZ; iZ++ )
				{
					for( iX = iSX; iX < iEX; iX++ )
					{
						uwTempHeight = pHeightBuffer[iX + iZ * pixWidth];
						uwTempHeight = ( ( uwTempHeight & 0xFF00 ) >> 8 ) | ( ( uwTempHeight & 0x00FF ) << 8 );

						vOrig = FLOAT3D( iX, fMaxY, iZ );
						if( RayHitsTriangle( vOrig, vDir, vP0, vP1, vP2, &fT ) )
						{
							vHit = vDir * fT + vOrig;
							uwTempHeight2 = ( vHit( 2 ) + 0.009999f ) * 100.0f;
							if( uwTempHeight < uwTempHeight2 )
								uwTempHeight = uwTempHeight2;
						}
						vOrig = FLOAT3D( iX + 1.0f, fMaxY, iZ );
						if( RayHitsTriangle( vOrig, vDir, vP0, vP1, vP2, &fT ) )
						{
							vHit = vDir * fT + vOrig;
							uwTempHeight2 = ( vHit( 2 ) + 0.009999f ) * 100.0f;
							if( uwTempHeight < uwTempHeight2 )
								uwTempHeight = uwTempHeight2;
						}
						vOrig = FLOAT3D( iX, fMaxY, iZ + 1.0f );
						if( RayHitsTriangle( vOrig, vDir, vP0, vP1, vP2, &fT ) )
						{
							vHit = vDir * fT + vOrig;
							uwTempHeight2 = ( vHit( 2 ) + 0.009999f ) * 100.0f;
							if( uwTempHeight < uwTempHeight2 )
								uwTempHeight = uwTempHeight2;
						}
						vOrig = FLOAT3D( iX + 1.0f, fMaxY, iZ + 1.0f );
						if( RayHitsTriangle( vOrig, vDir, vP0, vP1, vP2, &fT ) )
						{
							vHit = vDir * fT + vOrig;
							uwTempHeight2 = ( vHit( 2 ) + 0.009999f ) * 100.0f;
							if( uwTempHeight < uwTempHeight2 )
								uwTempHeight = uwTempHeight2;
						}
						vOrig = FLOAT3D( iX + 0.5f, fMaxY, iZ + 0.5f );
						if( RayHitsTriangle( vOrig, vDir, vP0, vP1, vP2, &fT ) )
						{
							vHit = vDir * fT + vOrig;
							uwTempHeight2 = ( vHit( 2 ) + 0.009999f ) * 100.0f;
							if( uwTempHeight < uwTempHeight2 )
								uwTempHeight = uwTempHeight2;
						}

						if( uwTempHeight > 0 )
						{
							uwHeight = ( ( uwTempHeight & 0xFF00 ) >> 8 ) | ( ( uwTempHeight & 0x00FF ) << 8 );
							pHeightBuffer[iX + iZ * pixWidth] = uwHeight;
						}
					}
				}
			}
		}
	}

	// Peace zone
	if( !bField )
	{
		FOREACHINSTATICARRAY( ba_apbsc, CBrushSector *, itBpsc1 )
		{
			pBpsc = *itBpsc1;
			if( pBpsc == NULL )
				continue;
			if( pBpsc->bsc_ulFlags & BSCF_HIDDEN )
				continue;
			if( pBpsc->bsc_pbmBrushMip->bm_pbrBrush->br_penEntity->en_ulFlags & ENF_HIDDEN )
				continue;

			UBYTE			ubPolygonAttr;
			FOREACHINSTATICARRAY( pBpsc->bsc_abpoPolygons, CBrushPolygon, itBpo )
			{
				CBrushPolygon	&rBpo = itBpo.Current();

				ubPolygonAttr = rBpo.bpo_ubPolygonAttribute;
				if( ubPolygonAttr < BPOA_PEACEZONE1F || ubPolygonAttr > BPOA_PEACEZONE5F )
					continue;
				if( ( ubPolygonAttr % 10 ) != iFloor )
					continue;

				aabBox = rBpo.bpo_boxBoundingBox;
				vOrig = FLOAT3D( ( aabBox.minvect( 1 ) + aabBox.maxvect( 1 ) ) / 2,
									aabBox.maxvect( 2 ) + 1,
									( aabBox.minvect( 3 ) + aabBox.maxvect( 3 ) ) / 2 );
				FLOAT3D	vTarget = vOrig + vDir * 10000.0f;
				FLOAT	fDistance0 = rBpo.bpo_pbplPlane->bpl_plAbsolute.PointDistance( vOrig );
				FLOAT	fDistance1 = rBpo.bpo_pbplPlane->bpl_plAbsolute.PointDistance( vTarget );

				if( fDistance0 < 0 || fDistance0 < fDistance1 )
					continue;

				INDEX	iV0, iV1, iV2;
				INDEX	ctIndices = rBpo.bpo_aiTriangleElements.Count();
				for( INDEX iIndex = 0; iIndex < ctIndices; iIndex += 3 )
				{
					iV0 = rBpo.bpo_aiTriangleElements[iIndex];
					iV1 = rBpo.bpo_aiTriangleElements[iIndex + 1];
					iV2 = rBpo.bpo_aiTriangleElements[iIndex + 2];
					FLOAT3D	vP0 = rBpo.bpo_apbvxTriangleVertices[iV0]->bvx_vAbsolute;
					FLOAT3D	vP1 = rBpo.bpo_apbvxTriangleVertices[iV1]->bvx_vAbsolute;
					FLOAT3D	vP2 = rBpo.bpo_apbvxTriangleVertices[iV2]->bvx_vAbsolute;

					fMinX = fMaxX = vP0( 1 );
					fMinZ = fMaxZ = vP0( 3 );
					fMaxY = vP0( 2 );
					
					if( fMinX > vP1( 1 ) ) fMinX = vP1( 1 );
					else if( fMaxX < vP1( 1 ) ) fMaxX = vP1( 1 );
					if( fMinZ > vP1( 3 ) ) fMinZ = vP1( 3 );
					else if( fMaxZ < vP1( 3 ) ) fMaxZ = vP1( 3 );
					if( fMaxY < vP1( 2 ) ) fMaxY = vP1( 2 );

					if( fMinX > vP2( 1 ) ) fMinX = vP2( 1 );
					else if( fMaxX < vP2( 1 ) ) fMaxX = vP2( 1 );
					if( fMinZ > vP2( 3 ) ) fMinZ = vP2( 3 );
					else if( fMaxZ < vP2( 3 ) ) fMaxZ = vP2( 3 );
					if( fMaxY < vP2( 2 ) ) fMaxY = vP2( 2 );

					fMaxY += 1.0f;
					iSX = (INDEX)fMinX;		iEX = (INDEX)( fMaxX + 0.99999f );
					iSZ = (INDEX)fMinZ;		iEZ = (INDEX)( fMaxZ + 0.99999f );

					FLOAT	fT;
					for( iZ = iSZ; iZ < iEZ; iZ++ )
					{
						for( iX = iSX; iX < iEX; iX++ )
						{
							uwTempHeight = pHeightBuffer[iX + iZ * pixWidth];
							uwTempHeight = ( ( uwTempHeight & 0xFF00 ) >> 8 ) | ( ( uwTempHeight & 0x00FF ) << 8 );

							vOrig = FLOAT3D( iX, fMaxY, iZ );
							if( RayHitsTriangle( vOrig, vDir, vP0, vP1, vP2, &fT ) )
							{
								vHit = vDir * fT + vOrig;
								uwTempHeight2 = ( vHit( 2 ) + 0.009999f ) * 100.0f;
								if( uwTempHeight < uwTempHeight2 )
									uwTempHeight = uwTempHeight2;
							}
							vOrig = FLOAT3D( iX + 1.0f, fMaxY, iZ );
							if( RayHitsTriangle( vOrig, vDir, vP0, vP1, vP2, &fT ) )
							{
								vHit = vDir * fT + vOrig;
								uwTempHeight2 = ( vHit( 2 ) + 0.009999f ) * 100.0f;
								if( uwTempHeight < uwTempHeight2 )
									uwTempHeight = uwTempHeight2;
							}
							vOrig = FLOAT3D( iX, fMaxY, iZ + 1.0f );
							if( RayHitsTriangle( vOrig, vDir, vP0, vP1, vP2, &fT ) )
							{
								vHit = vDir * fT + vOrig;
								uwTempHeight2 = ( vHit( 2 ) + 0.009999f ) * 100.0f;
								if( uwTempHeight < uwTempHeight2 )
									uwTempHeight = uwTempHeight2;
							}
							vOrig = FLOAT3D( iX + 1.0f, fMaxY, iZ + 1.0f );
							if( RayHitsTriangle( vOrig, vDir, vP0, vP1, vP2, &fT ) )
							{
								vHit = vDir * fT + vOrig;
								uwTempHeight2 = ( vHit( 2 ) + 0.009999f ) * 100.0f;
								if( uwTempHeight < uwTempHeight2 )
									uwTempHeight = uwTempHeight2;
							}
							vOrig = FLOAT3D( iX + 0.5f, fMaxY, iZ + 0.5f );
							if( RayHitsTriangle( vOrig, vDir, vP0, vP1, vP2, &fT ) )
							{
								vHit = vDir * fT + vOrig;
								uwTempHeight2 = ( vHit( 2 ) + 0.009999f ) * 100.0f;
								if( uwTempHeight < uwTempHeight2 )
									uwTempHeight = uwTempHeight2;
							}

							if( uwTempHeight > 0 )
							{
								uwHeight = ( ( uwTempHeight & 0xFF00 ) >> 8 ) | ( ( uwTempHeight & 0x00FF ) << 8 );
								pHeightBuffer[iX + iZ * pixWidth] = uwHeight;
							}
						}
					}
				}
			}
		}
	}

	// Stair
	FOREACHINSTATICARRAY( ba_apbsc, CBrushSector *, itBpsc2 )
	{
		pBpsc = *itBpsc2;
		if( pBpsc == NULL )
			continue;
		if( pBpsc->bsc_ulFlags & BSCF_HIDDEN )
			continue;
		if( pBpsc->bsc_pbmBrushMip->bm_pbrBrush->br_penEntity->en_ulFlags & ENF_HIDDEN )
			continue;

		UBYTE			ubPolygonAttr;
		FOREACHINSTATICARRAY( pBpsc->bsc_abpoPolygons, CBrushPolygon, itBpo )
		{
			CBrushPolygon	&rBpo = itBpo.Current();

			ubPolygonAttr = rBpo.bpo_ubPolygonAttribute;
			if( ubPolygonAttr < BPOA_STAIR1F2F && ubPolygonAttr > BPOA_STAIR4F5F )
				continue;
			if( ubPolygonAttr != iFloor + 19 && ubPolygonAttr != iFloor + 20 )
				continue;

			aabBox = rBpo.bpo_boxBoundingBox;
			vOrig = FLOAT3D( ( aabBox.minvect( 1 ) + aabBox.maxvect( 1 ) ) / 2,
								aabBox.maxvect( 2 ) + 1,
								( aabBox.minvect( 3 ) + aabBox.maxvect( 3 ) ) / 2 );
			FLOAT3D	vTarget = vOrig + vDir * 10000.0f;
			FLOAT	fDistance0 = rBpo.bpo_pbplPlane->bpl_plAbsolute.PointDistance( vOrig );
			FLOAT	fDistance1 = rBpo.bpo_pbplPlane->bpl_plAbsolute.PointDistance( vTarget );

			if( fDistance0 < 0 || fDistance0 < fDistance1 )
				continue;

			INDEX	iV0, iV1, iV2;
			INDEX	ctIndices = rBpo.bpo_aiTriangleElements.Count();
			for( INDEX iIndex = 0; iIndex < ctIndices; iIndex += 3 )
			{
				iV0 = rBpo.bpo_aiTriangleElements[iIndex];
				iV1 = rBpo.bpo_aiTriangleElements[iIndex + 1];
				iV2 = rBpo.bpo_aiTriangleElements[iIndex + 2];
				FLOAT3D	vP0 = rBpo.bpo_apbvxTriangleVertices[iV0]->bvx_vAbsolute;
				FLOAT3D	vP1 = rBpo.bpo_apbvxTriangleVertices[iV1]->bvx_vAbsolute;
				FLOAT3D	vP2 = rBpo.bpo_apbvxTriangleVertices[iV2]->bvx_vAbsolute;

				fMinX = fMaxX = vP0( 1 );
				fMinZ = fMaxZ = vP0( 3 );
				fMaxY = vP0( 2 );
				
				if( fMinX > vP1( 1 ) ) fMinX = vP1( 1 );
				else if( fMaxX < vP1( 1 ) ) fMaxX = vP1( 1 );
				if( fMinZ > vP1( 3 ) ) fMinZ = vP1( 3 );
				else if( fMaxZ < vP1( 3 ) ) fMaxZ = vP1( 3 );
				if( fMaxY < vP1( 2 ) ) fMaxY = vP1( 2 );

				if( fMinX > vP2( 1 ) ) fMinX = vP2( 1 );
				else if( fMaxX < vP2( 1 ) ) fMaxX = vP2( 1 );
				if( fMinZ > vP2( 3 ) ) fMinZ = vP2( 3 );
				else if( fMaxZ < vP2( 3 ) ) fMaxZ = vP2( 3 );
				if( fMaxY < vP2( 2 ) ) fMaxY = vP2( 2 );

				fMaxY += 1.0f;
				iSX = (INDEX)fMinX;		iEX = (INDEX)( fMaxX + 0.99999f );
				iSZ = (INDEX)fMinZ;		iEZ = (INDEX)( fMaxZ + 0.99999f );

				FLOAT	fT;
				for( iZ = iSZ; iZ < iEZ; iZ++ )
				{
					for( iX = iSX; iX < iEX; iX++ )
					{
						uwTempHeight = pHeightBuffer[iX + iZ * pixWidth];
						uwTempHeight = ( ( uwTempHeight & 0xFF00 ) >> 8 ) | ( ( uwTempHeight & 0x00FF ) << 8 );

						vOrig = FLOAT3D( iX, fMaxY, iZ );
						if( RayHitsTriangle( vOrig, vDir, vP0, vP1, vP2, &fT ) )
						{
							vHit = vDir * fT + vOrig;
							uwTempHeight2 = ( vHit( 2 ) + 0.009999f ) * 100.0f;
							if( uwTempHeight < uwTempHeight2 )
								uwTempHeight = uwTempHeight2;
						}
						vOrig = FLOAT3D( iX + 1.0f, fMaxY, iZ );
						if( RayHitsTriangle( vOrig, vDir, vP0, vP1, vP2, &fT ) )
						{
							vHit = vDir * fT + vOrig;
							uwTempHeight2 = ( vHit( 2 ) + 0.009999f ) * 100.0f;
							if( uwTempHeight < uwTempHeight2 )
								uwTempHeight = uwTempHeight2;
						}
						vOrig = FLOAT3D( iX, fMaxY, iZ + 1.0f );
						if( RayHitsTriangle( vOrig, vDir, vP0, vP1, vP2, &fT ) )
						{
							vHit = vDir * fT + vOrig;
							uwTempHeight2 = ( vHit( 2 ) + 0.009999f ) * 100.0f;
							if( uwTempHeight < uwTempHeight2 )
								uwTempHeight = uwTempHeight2;
						}
						vOrig = FLOAT3D( iX + 1.0f, fMaxY, iZ + 1.0f );
						if( RayHitsTriangle( vOrig, vDir, vP0, vP1, vP2, &fT ) )
						{
							vHit = vDir * fT + vOrig;
							uwTempHeight2 = ( vHit( 2 ) + 0.009999f ) * 100.0f;
							if( uwTempHeight < uwTempHeight2 )
								uwTempHeight = uwTempHeight2;
						}
						vOrig = FLOAT3D( iX + 0.5f, fMaxY, iZ + 0.5f );
						if( RayHitsTriangle( vOrig, vDir, vP0, vP1, vP2, &fT ) )
						{
							vHit = vDir * fT + vOrig;
							uwTempHeight2 = ( vHit( 2 ) + 0.009999f ) * 100.0f;
							if( uwTempHeight < uwTempHeight2 )
								uwTempHeight = uwTempHeight2;
						}

						if( uwTempHeight > 0 )
						{
							uwHeight = ( ( uwTempHeight & 0xFF00 ) >> 8 ) | ( ( uwTempHeight & 0x00FF ) << 8 );
							pHeightBuffer[iX + iZ * pixWidth] = uwHeight;
						}
					}
				}
			}
		}
	}
}
#endif // FINALVERSION
// yjpark     -->|