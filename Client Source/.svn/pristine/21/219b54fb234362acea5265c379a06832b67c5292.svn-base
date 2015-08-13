#include "stdh.h"
#include <Engine/Base/Stream.h>
#include <Engine/Terrain/Terrain.h>
#include <Engine/Terrain/Terrain.hpp>
#include <Engine/Terrain/TRGenerate.hpp>
#include <Engine/Terrain/TRLayer.h>
#include <Engine/Terrain/TRLayer.hpp>
#include <Engine/Templates/Stock_CTextureData.h>
#include <Engine/Brushes/Brush.h>

extern TerrainInfo _tiTerrainInfo;
BOOL	_bLoadTerrain = FALSE;		// yjpark
template <class Type>
extern void TerrainMapCrop(Type *ptDst, SLONG slDstWidth, SLONG slDstHeight,
													 const Type *ptSrc, SLONG slSrcWidth, SLONG slSrcHeight, UBYTE ubFill);
#define TR_FNM_VERSION 20			// yjpark

// Constructor
CTerrain::CTerrain(void)
: tr_ptrTerrain(NULL)
{
	tr_ptrTerrain = new CTerrainImp;
	tr_iSelectedLayer = 0;
	tr_iSelectedShadowMap = 0;		// yjpark
	tr_ulTerrainFlags = TR_USES_TOPMAP; // Use top map by default
	tr_penEntity  = NULL;

	// Add terrain to list of loaded terrains
	_pGfx->gl_lhLoadedTerrains.AddTail(tr_lnLoadedTerrains);
}

// Destructor
CTerrain::~CTerrain(void)
{
	ASSERT(tr_lnLoadedTerrains.IsLinked());
	tr_lnLoadedTerrains.Remove();

	TR_ClearTerrain(this);
	if(tr_ptrTerrain!=NULL) {
		delete tr_ptrTerrain;
		tr_ptrTerrain = NULL;
	}
}

// yjpark |<--
static void ReadTerrainVersion17_t( CTerrain *ptrTerrain, CTStream *istrFile )
{
	ASSERT( ptrTerrain != NULL );
	ASSERT( ptrTerrain->tr_ptrTerrain != NULL );

	CTerrainImp	*ptr = ptrTerrain->tr_ptrTerrain;
	PIX			pixHeightMapWidth;
	PIX			pixHeightMapHeight;
	PIX			pixTopMapWidth;
	PIX			pixTopMapHeight;
	SLONG		slShadowMapSizeAspect;
	SLONG		slShadingMapSizeAspect;
	SLONG		slAttributeMapSizeAspect;
	INDEX		cttrl;
	INDEX		cttrs;
	INDEX		iFirstTopMapLOD;
	
	// Terrain global data
	istrFile->ExpectID_t( "TRGD" );
	(*istrFile) >> pixHeightMapWidth;
	(*istrFile) >> pixHeightMapHeight;
	(*istrFile) >> pixTopMapWidth;
	(*istrFile) >> pixTopMapHeight;
	(*istrFile) >> slShadowMapSizeAspect;
	(*istrFile) >> slShadingMapSizeAspect;
	if( ptr->tr_iTerrainVersion >= 19)
	{
		(*istrFile) >>slAttributeMapSizeAspect;		//[070626: Su-won] Terrain Ver.19
	}
	(*istrFile) >> iFirstTopMapLOD;
	(*istrFile) >> cttrl;
	(*istrFile) >> cttrs;
	(*istrFile) >> ptr->tr_fDistFactor;
	(*istrFile) >> ptr->tr_vStretch;
	(*istrFile) >> ptr->tr_vMetricSize;

	const PIX	pixEdgeMapWidth = pixHeightMapWidth - 1;
	const PIX	pixEdgeMapHeight = pixHeightMapHeight - 1;
	const PIX	pixShadowMapWidth = GetShadowMapWidth( pixHeightMapWidth, slShadowMapSizeAspect );
	const PIX	pixShadowMapHeight = GetShadowMapHeight( pixHeightMapHeight, slShadowMapSizeAspect );
	const SLONG	slHeightMapSize = pixHeightMapWidth * pixHeightMapHeight * sizeof(UWORD);
	const SLONG	slEdgeMapSize = pixEdgeMapWidth * pixEdgeMapHeight * sizeof(UBYTE);
	const SLONG	slShadowMapSize = pixShadowMapWidth * pixShadowMapHeight * sizeof(ULONG);
	const SLONG	slTopMapSize = pixTopMapWidth * pixTopMapHeight * sizeof(ULONG);

	ptr->tr_pixHeightMapWidth = pixHeightMapWidth;
	ptr->tr_pixHeightMapHeight = pixHeightMapHeight;
	ptr->tr_slShadowMapSizeAspect = slShadowMapSizeAspect;
	ptr->tr_slShadingMapSizeAspect = slShadingMapSizeAspect;
	//[070626: Su-won] Terrain Ver.19
	if( ptr->tr_iTerrainVersion >= 19)
	{
		ptr->tr_slAttributeMapSizeAspect = slAttributeMapSizeAspect;
	}

	extern BOOL	_bWorldEditorApp;
	if( _bWorldEditorApp )
	{
		ptr->tr_ptdEdgeMap = TR_CreateTexture();
		ptr->tr_ptdTopMap = TR_CreateTexture();

		ptr->tr_aptdShadowMaps.New( cttrs );
		ptr->tr_aulShadowTimes.New( cttrs + 1 );
		ptr->tr_afShadowBlurRadius.New( cttrs );
		ptr->tr_acolObjShadowColor.New( cttrs );

		ptr->tr_ptdShadowMap = TR_CreateTexture();
		TR_PrepareTexture( *ptr->tr_ptdShadowMap, pixShadowMapWidth, pixShadowMapHeight, PTF_STATIC );
	}
	else
	{
		ptr->tr_aptdShadowMaps.New( cttrs );
		ptr->tr_aulShadowTimes.New( cttrs + 1 );
		ptr->tr_afShadowBlurRadius.New( cttrs );
		ptr->tr_acolObjShadowColor.New( cttrs );
	}

	// Terrain height map
	ptr->tr_puwHeightMap = (UWORD *)AllocMemory( slHeightMapSize );
	UWORD	*puwHeightMap = ptr->tr_puwHeightMap;
	istrFile->ExpectID_t( "TRHM" );
	istrFile->Read_t( puwHeightMap, slHeightMapSize );

	// Set terrain size
	TR_SetTerrainSize( ptrTerrain, ptr->tr_vMetricSize );

	// Terrain edge map
	ptr->tr_pubEdgeMap = (UBYTE *)AllocMemory(slEdgeMapSize);
	UBYTE	*pubEdgeMap = ptr->tr_pubEdgeMap;
	istrFile->ExpectID_t( "TREM" );
	istrFile->Read_t( pubEdgeMap, slEdgeMapSize );

	// File name of edge map
	CTFileName	fnmEdgeMap;
	(*istrFile) >> fnmEdgeMap;

	// Upload edge map
	if( _bWorldEditorApp )
		TR_UploadEdgeMap( ptrTerrain );
	else
		ptr->tr_ptdEdgeMap = _pTextureStock->Obtain_t( fnmEdgeMap );

	istrFile->ExpectID_t( "TRSM" );
	ULONG	ulShadowTime;
	INDEX itrs;
	for( itrs = 0; itrs < cttrs; itrs++ )
	{
		// World time for current shadow map
		istrFile->Read_t( &ulShadowTime, sizeof(ULONG) );
		ptr->tr_aulShadowTimes[itrs] = ulShadowTime;

		// Blur factor for current shadow map
		FLOAT	fBlurRadius;
		istrFile->Read_t( &fBlurRadius, sizeof(FLOAT) );
		ptr->tr_afShadowBlurRadius[itrs] = fBlurRadius;

		// Color for current object shadow map
		GFXColor	colShadow;
		istrFile->Read_t( &colShadow, sizeof(GFXColor) );
		ptr->tr_acolObjShadowColor[itrs] = colShadow;

		// File name of current shadow map
		CTFileName	fnmShadowMap;
		(*istrFile) >> fnmShadowMap;
		
		// Shadow map
		ptr->tr_aptdShadowMaps[itrs] = _pTextureStock->Obtain_t( fnmShadowMap );
		if( _bWorldEditorApp )
		{
			ptr->tr_aptdShadowMaps[itrs]->Force( TEX_STATIC );
			ptr->tr_aptdShadowMaps[itrs]->Reload();
		}
	}
	istrFile->Read_t( &ulShadowTime, sizeof(ULONG) );
	ptr->tr_aulShadowTimes[itrs] = ulShadowTime;

	// Overbright strength of current shadow map
	FLOAT	fOverbright;
	istrFile->Read_t( &fOverbright, sizeof(FLOAT) );
	ptr->tr_fShadowOverbright = fOverbright;

	// Terrain top map
	ptr->tr_pixTopMapWidth = pixTopMapWidth;
	ptr->tr_pixTopMapHeight = pixTopMapHeight;
	ptr->tr_iFirstTopMapLOD = iFirstTopMapLOD;
	istrFile->ExpectID_t( "TRTM" );

	// File name of top map
	CTFileName	fnmTopMap;
	(*istrFile) >> fnmTopMap;

	// if terrain uses top map
	if( iFirstTopMapLOD >= 0 )
	{
		// Upload terrain top map
		ptr->tr_ptdTopMap = _pTextureStock->Obtain_t( fnmTopMap );
		if( _bWorldEditorApp )
		{
			ptr->tr_ptdTopMap->Force( TEX_STATIC );
			ptr->tr_ptdTopMap->Reload();
		}
		
		// Flag terrain that it can use top map
		ptrTerrain->tr_ulTerrainFlags |= TR_USES_TOPMAP;
	}
	else
	{
		// Flag terrain that if can't use top map
		ptrTerrain->tr_ulTerrainFlags &= ~TR_USES_TOPMAP;
	}

	// Terrain layer texture
	if( _bWorldEditorApp )
	{
		for( INDEX itrl = 0; itrl < cttrl; itrl++ )
		{
			CTFileName	fnmTexture;
			istrFile->ExpectID_t( "TRLT" );
			(*istrFile) >> fnmTexture;

			INDEX	iTerrainLayer = TRL_CreateLayer_t( ptrTerrain, fnmTexture );
			TRL_ReadLayerData_t( ptrTerrain, iTerrainLayer, istrFile );
		}
	}
	else
	{
		ptr->tr_atlLayers.New( cttrl );
		for( INDEX itrl = 0; itrl < cttrl; itrl++ )
		{
			CTFileName	fnmTexture;
			istrFile->ExpectID_t( "TRLT" );
			(*istrFile) >> fnmTexture;

			TRL_SetLayerTexture_t( ptrTerrain, itrl, fnmTexture );
			TRL_ApplyLayerMatrix( ptrTerrain, itrl );
			TRL_ReadLayerData_t( ptrTerrain, itrl, istrFile );
		}
	}

	//[070626: Su-won] Terrain Ver.19
	if (ptr->tr_iTerrainVersion >= 20)
	{
		istrFile->ExpectID_t( "TRAM" );
		const SLONG	slAttributeMapSize = ptr->tr_vMetricSize( 1 ) * ptr->tr_vMetricSize( 3 ) * sizeof(UWORD);
		ptr->tr_pubAttributeMap = (UWORD*)AllocMemory(slAttributeMapSize);
		UWORD	*pubAttributeMap = ptr->tr_pubAttributeMap;
		istrFile->Read_t(pubAttributeMap, slAttributeMapSize);
	}
	else if( ptr->tr_iTerrainVersion >= 19)
	{
		istrFile->ExpectID_t( "TRAM" );
		const SLONG	slAttributeMapSize = ptr->tr_vMetricSize( 1 ) * ptr->tr_vMetricSize( 3 );
		ptr->tr_pubAttributeMap = (UWORD*)AllocMemory(slAttributeMapSize * sizeof(UWORD));
		UBYTE	*pubAttributeMap = (UBYTE*)AllocMemory(slAttributeMapSize);
		istrFile->Read_t( pubAttributeMap, slAttributeMapSize );

		int		i;

		for (i = 0; i < slAttributeMapSize; ++i)
		{
			switch (pubAttributeMap[i])
			{
			case ATTC_WALKABLE:
				ptr->tr_pubAttributeMap[i] = (UWORD)MATT_WALKABLE;
				break;
			case ATTC_PEACE:
				ptr->tr_pubAttributeMap[i] = (UWORD)MATT_PEACE;
				break;
			case ATTC_PRODUCT_PUBLIC:
				ptr->tr_pubAttributeMap[i] = (UWORD)MATT_PRODUCT_PUBLIC;
				break;
			case ATTC_PRODUCT_PRIVATE:
				ptr->tr_pubAttributeMap[i] = (UWORD)MATT_PRODUCT_PRIVATE;
				break;
			case ATTC_STAIR_UP:
				ptr->tr_pubAttributeMap[i] = (UWORD)MATT_STAIR_UP;
				break;
			case ATTC_STAIR_DOWN:
				ptr->tr_pubAttributeMap[i] = (UWORD)MATT_STAIR_DOWN;
				break;
			case ATTC_WAR:
				ptr->tr_pubAttributeMap[i] = (UWORD)MATT_WAR;
				break;
			case ATTC_FREEPKZONE:
				ptr->tr_pubAttributeMap[i] = (UWORD)MATT_FREEPKZONE;
				break;
			case ATTC_UNWALKABLE:
				ptr->tr_pubAttributeMap[i] = (UWORD)MATT_UNWALKABLE;
				break;
			default:
				//ASSERT(0);
				ptr->tr_pubAttributeMap[i] = (UWORD)MATT_UNWALKABLE;
				break;
			}
		}
	}

	// Terrain data end
	istrFile->ExpectID_t( "TRDE" );
}

static void ReadTerrainVersion16_t( CTerrain *ptrTerrain, CTStream *istrFile )
{
	ASSERT( ptrTerrain != NULL );
	ASSERT( ptrTerrain->tr_ptrTerrain != NULL );

	CTerrainImp	*ptr = ptrTerrain->tr_ptrTerrain;
	PIX			pixHeightMapWidth;
	PIX			pixHeightMapHeight;
	PIX			pixTopMapWidth;
	PIX			pixTopMapHeight;
	SLONG		slShadowMapSizeAspect;
	SLONG		slShadingMapSizeAspect;
	INDEX		cttrl;
	INDEX		cttrs;
	INDEX		iFirstTopMapLOD;
	
	// Terrain global data
	istrFile->ExpectID_t( "TRGD" );
	(*istrFile) >> pixHeightMapWidth;
	(*istrFile) >> pixHeightMapHeight;
	(*istrFile) >> pixTopMapWidth;
	(*istrFile) >> pixTopMapHeight;
	(*istrFile) >> slShadowMapSizeAspect;
	(*istrFile) >> slShadingMapSizeAspect;
	(*istrFile) >> iFirstTopMapLOD;
	(*istrFile) >> cttrl;
	(*istrFile) >> cttrs;
	(*istrFile) >> ptr->tr_fDistFactor;
	(*istrFile) >> ptr->tr_vStretch;
	(*istrFile) >> ptr->tr_vMetricSize;

	const PIX	pixEdgeMapWidth = pixHeightMapWidth - 1;
	const PIX	pixEdgeMapHeight = pixHeightMapHeight - 1;
	const PIX	pixShadowMapWidth = GetShadowMapWidth( pixHeightMapWidth, slShadowMapSizeAspect );
	const PIX	pixShadowMapHeight = GetShadowMapHeight( pixHeightMapHeight, slShadowMapSizeAspect );
	const SLONG	slHeightMapSize = pixHeightMapWidth * pixHeightMapHeight * sizeof(UWORD);
	const SLONG	slEdgeMapSize = pixEdgeMapWidth * pixEdgeMapHeight * sizeof(UBYTE);
	const SLONG	slShadowMapSize = pixShadowMapWidth * pixShadowMapHeight * sizeof(ULONG);
	const SLONG	slTopMapSize = pixTopMapWidth * pixTopMapHeight * sizeof(ULONG);

	ptr->tr_pixHeightMapWidth = pixHeightMapWidth;
	ptr->tr_pixHeightMapHeight = pixHeightMapHeight;
	ptr->tr_slShadowMapSizeAspect = slShadowMapSizeAspect;
	ptr->tr_slShadingMapSizeAspect = slShadingMapSizeAspect;

	extern BOOL	_bWorldEditorApp;
	if( _bWorldEditorApp )
	{
		ptr->tr_ptdEdgeMap = TR_CreateTexture();
		ptr->tr_ptdTopMap = TR_CreateTexture();

		ptr->tr_aptdShadowMaps.New( cttrs );
		ptr->tr_aulShadowTimes.New( cttrs + 1 );
		ptr->tr_afShadowBlurRadius.New( cttrs );
		ptr->tr_acolObjShadowColor.New( cttrs );

		ptr->tr_ptdShadowMap = TR_CreateTexture();
		TR_PrepareTexture( *ptr->tr_ptdShadowMap, pixShadowMapWidth, pixShadowMapHeight, PTF_STATIC );
	}
	else
	{
		ptr->tr_aptdShadowMaps.New( cttrs );
		ptr->tr_aulShadowTimes.New( cttrs + 1 );
		ptr->tr_afShadowBlurRadius.New( cttrs );
		ptr->tr_acolObjShadowColor.New( cttrs );
	}

	// Terrain height map
	ptr->tr_puwHeightMap = (UWORD *)AllocMemory( slHeightMapSize );
	UWORD	*puwHeightMap = ptr->tr_puwHeightMap;
	istrFile->ExpectID_t( "TRHM" );
	istrFile->Read_t( puwHeightMap, slHeightMapSize );

	// Set terrain size
	TR_SetTerrainSize( ptrTerrain, ptr->tr_vMetricSize );

	// Terrain edge map
	ptr->tr_pubEdgeMap = (UBYTE *)AllocMemory(slEdgeMapSize);
	UBYTE	*pubEdgeMap = ptr->tr_pubEdgeMap;
	istrFile->ExpectID_t( "TREM" );
	istrFile->Read_t( pubEdgeMap, slEdgeMapSize );

	// File name of edge map
	CTFileName	fnmEdgeMap;
	(*istrFile) >> fnmEdgeMap;

	// Upload edge map
	if( _bWorldEditorApp )
		TR_UploadEdgeMap( ptrTerrain );
	else
		ptr->tr_ptdEdgeMap = _pTextureStock->Obtain_t( fnmEdgeMap );

	istrFile->ExpectID_t( "TRSM" );
	ULONG	ulShadowTime;
	INDEX itrs;
	for( itrs = 0; itrs < cttrs; itrs++ )
	{
		// World time for current shadow map
		istrFile->Read_t( &ulShadowTime, sizeof(ULONG) );
		ptr->tr_aulShadowTimes[itrs] = ulShadowTime;

		// Blur factor for current shadow map
		FLOAT	fBlurRadius;
		istrFile->Read_t( &fBlurRadius, sizeof(FLOAT) );
		ptr->tr_afShadowBlurRadius[itrs] = fBlurRadius;

		// Color for current object shadow map
		ptr->tr_acolObjShadowColor[itrs].abgr = 0xFF000000;

		// File name of current shadow map
		CTFileName	fnmShadowMap;
		(*istrFile) >> fnmShadowMap;
		
		// Shadow map
		ptr->tr_aptdShadowMaps[itrs] = _pTextureStock->Obtain_t( fnmShadowMap );
		if( _bWorldEditorApp )
		{
			ptr->tr_aptdShadowMaps[itrs]->Force( TEX_STATIC );
			ptr->tr_aptdShadowMaps[itrs]->Reload();
		}
	}
	istrFile->Read_t( &ulShadowTime, sizeof(ULONG) );
	ptr->tr_aulShadowTimes[itrs] = ulShadowTime;

	// Overbright strength of current shadow map
	FLOAT	fOverbright;
	istrFile->Read_t( &fOverbright, sizeof(FLOAT) );
	ptr->tr_fShadowOverbright = fOverbright;

	// Terrain top map
	ptr->tr_pixTopMapWidth = pixTopMapWidth;
	ptr->tr_pixTopMapHeight = pixTopMapHeight;
	ptr->tr_iFirstTopMapLOD = iFirstTopMapLOD;
	istrFile->ExpectID_t( "TRTM" );

	// File name of top map
	CTFileName	fnmTopMap;
	(*istrFile) >> fnmTopMap;

	// if terrain uses top map
	if( iFirstTopMapLOD >= 0 )
	{
		// Upload terrain top map
		if( _bWorldEditorApp )
		{
			TR_PrepareTexture( *ptr->tr_ptdTopMap, pixTopMapWidth, pixTopMapHeight, PTF_STATIC );
			CTFileName	fnmTopMap_ett = fnmTopMap.NoExt() + CTString( ".ett" );
			CTFileStream	strmTopMap_ett;
			strmTopMap_ett.Open_t( fnmTopMap_ett );
			strmTopMap_ett.Read_t( ptr->tr_ptdTopMap->td_pulFrames, slTopMapSize );
			strmTopMap_ett.Close();
			TR_UploadTexture( *ptr->tr_ptdTopMap, pixTopMapWidth, pixTopMapHeight, PTF_STATIC );
		}
		else
		{
			ptr->tr_ptdTopMap = _pTextureStock->Obtain_t( fnmTopMap );
		}
		
		// Flag terrain that it can use top map
		ptrTerrain->tr_ulTerrainFlags |= TR_USES_TOPMAP;
	}
	else
	{
		// Flag terrain that if can't use top map
		ptrTerrain->tr_ulTerrainFlags &= ~TR_USES_TOPMAP;
	}

	// Terrain layer texture
	if( _bWorldEditorApp )
	{
		for( INDEX itrl = 0; itrl < cttrl; itrl++ )
		{
			CTFileName	fnmTexture;
			istrFile->ExpectID_t( "TRLT" );
			(*istrFile) >> fnmTexture;

			INDEX	iTerrainLayer = TRL_CreateLayer_t( ptrTerrain, fnmTexture );
			TRL_ReadLayerData_t( ptrTerrain, iTerrainLayer, istrFile );
		}
	}
	else
	{
		ptr->tr_atlLayers.New( cttrl );
		for( INDEX itrl = 0; itrl < cttrl; itrl++ )
		{
			CTFileName	fnmTexture;
			istrFile->ExpectID_t( "TRLT" );
			(*istrFile) >> fnmTexture;

			TRL_SetLayerTexture_t( ptrTerrain, itrl, fnmTexture );
			TRL_ApplyLayerMatrix( ptrTerrain, itrl );
			TRL_ReadLayerData_t( ptrTerrain, itrl, istrFile );
		}
	}

	// Terrain data end
	istrFile->ExpectID_t( "TRDE" );
}

/*
static void ReadTerrainVersion15_t( CTerrain *ptrTerrain, CTStream *istrFile )
{
	ASSERT( ptrTerrain != NULL );
	ASSERT( ptrTerrain->tr_ptrTerrain != NULL );

	CTerrainImp	*ptr = ptrTerrain->tr_ptrTerrain;
	PIX			pixHeightMapWidth;
	PIX			pixHeightMapHeight;
	PIX			pixTopMapWidth;
	PIX			pixTopMapHeight;
	SLONG		slShadowMapSizeAspect;
	SLONG		slShadingMapSizeAspect;
	INDEX		cttrl;
	INDEX		cttrs;
	INDEX		iFirstTopMapLOD;
	
	// Terrain global data
	istrFile->ExpectID_t( "TRGD" );
	(*istrFile) >> pixHeightMapWidth;
	(*istrFile) >> pixHeightMapHeight;
	(*istrFile) >> pixTopMapWidth;
	(*istrFile) >> pixTopMapHeight;
	(*istrFile) >> slShadowMapSizeAspect;
	(*istrFile) >> slShadingMapSizeAspect;
	(*istrFile) >> iFirstTopMapLOD;
	(*istrFile) >> cttrl;
	(*istrFile) >> cttrs;
	(*istrFile) >> ptr->tr_fDistFactor;
	(*istrFile) >> ptr->tr_vStretch;
	(*istrFile) >> ptr->tr_vMetricSize;

	const PIX	pixEdgeMapWidth = pixHeightMapWidth - 1;
	const PIX	pixEdgeMapHeight = pixHeightMapHeight - 1;
	const PIX	pixShadowMapWidth = GetShadowMapWidth( pixHeightMapWidth, slShadowMapSizeAspect );
	const PIX	pixShadowMapHeight = GetShadowMapHeight( pixHeightMapHeight, slShadowMapSizeAspect );
	const SLONG	slHeightMapSize = pixHeightMapWidth * pixHeightMapHeight * sizeof(UWORD);
	const SLONG	slEdgeMapSize = pixEdgeMapWidth * pixEdgeMapHeight * sizeof(UBYTE);
	const SLONG	slShadowMapSize = pixShadowMapWidth * pixShadowMapHeight * sizeof(ULONG);
	const SLONG	slTopMapSize = pixTopMapWidth * pixTopMapHeight * sizeof(ULONG);

	ptr->tr_slShadowMapSizeAspect = slShadowMapSizeAspect;
	ptr->tr_slShadingMapSizeAspect = slShadingMapSizeAspect;
	ptr->tr_aptdShadowMaps.New( cttrs );
	ptr->tr_aulShadowTimes.New( cttrs + 1 );
	ptr->tr_afShadowBlurRadius.New( cttrs );
	ptr->tr_acolObjShadowColor.New( cttrs );
	for( INDEX itrs = 0; itrs < cttrs; itrs++ )
		ptr->tr_aptdShadowMaps[itrs] = TR_CreateTexture();

	extern BOOL	_bWorldEditorApp;
	if( _bWorldEditorApp )
		ptr->tr_ptdShadowMap = TR_CreateTexture();

	ptr->tr_ptdEdgeMap = TR_CreateTexture();
	ptr->tr_ptdTopMap = TR_CreateTexture();

	TR_SetHeightMapSize( ptrTerrain, pixHeightMapWidth, pixHeightMapHeight );

	UWORD	*puwHeightMap = ptr->tr_puwHeightMap;
	UBYTE	*pubEdgeMap = ptr->tr_pubEdgeMap;

	// Terrain height map
	istrFile->ExpectID_t( "TRHM" );
	istrFile->Read_t( puwHeightMap, slHeightMapSize );

	// Terrain edge map
	istrFile->ExpectID_t( "TREM" );
	istrFile->Read_t( pubEdgeMap, slEdgeMapSize );

	// Upload edge map
	TR_UploadEdgeMap( ptrTerrain );

	// Terrain shadow map
	istrFile->ExpectID_t( "TRSM" );
	for( itrs = 0; itrs < cttrs; itrs++ )
	{
		ULONG	ulShadowTime;
		istrFile->Read_t( &ulShadowTime, sizeof(ULONG) );
		ptr->tr_aulShadowTimes[itrs] = ulShadowTime;

		FLOAT	fBlurRadius;
		istrFile->Read_t( &fBlurRadius, sizeof(FLOAT) );
		ptr->tr_afShadowBlurRadius[itrs] = fBlurRadius;

		ptr->tr_acolObjShadowColor[itrs].abgr = 0xFF000000;

		istrFile->Seek_t( slShadowMapSize, CTStream::SD_CUR );
		istrFile->Seek_t( slShadowMapSize, CTStream::SD_CUR );
	}
	ULONG	ulLastShadowTime;
	istrFile->Read_t( &ulLastShadowTime, sizeof(ULONG) );
	ptr->tr_aulShadowTimes[itrs] = ulLastShadowTime;

	// Terrain top map
	ptr->tr_pixTopMapWidth = pixTopMapWidth;
	ptr->tr_pixTopMapHeight = pixTopMapHeight;
	ptr->tr_iFirstTopMapLOD = iFirstTopMapLOD;
	istrFile->ExpectID_t( "TRTM" );
	// if terrain uses top map
	if( iFirstTopMapLOD >= 0 )
	{
		// Upload terrain top map
		TR_PrepareTexture( *ptr->tr_ptdTopMap, pixTopMapWidth, pixTopMapHeight, PTF_STATIC );
		istrFile->Read_t( ptr->tr_ptdTopMap->td_pulFrames, slTopMapSize );
		TR_UploadTexture( *ptr->tr_ptdTopMap, pixTopMapWidth, pixTopMapHeight, PTF_STATIC );
		// Flag terrain that it can use top map
		ptrTerrain->tr_ulTerrainFlags |= TR_USES_TOPMAP;
	}
	else
	{
		// Flag terrain that if can't use top map
		ptrTerrain->tr_ulTerrainFlags &= ~TR_USES_TOPMAP;
	}

	// read terrain layers
	for( INDEX itrl = 0; itrl < cttrl; itrl++ )
	{
		// Terrain layer texture
		CTFileName	fnmTexture;
		istrFile->ExpectID_t( "TRLT" );
		(*istrFile) >> fnmTexture;

		INDEX			iTerrainLayer = TRL_CreateLayer_t( ptrTerrain, fnmTexture );
		CTerrainLayer	&tlLayer = ptr->tr_atlLayers[iTerrainLayer];
		// write layer data
		TRL_ReadLayerData_t( ptrTerrain, iTerrainLayer, istrFile );
	}

	// Terrain data end
	istrFile->ExpectID_t( "TRDE" );
}

static void ReadTerrainVersion14_t( CTerrain *ptrTerrain, CTStream *istrFile )
{
	ASSERT( ptrTerrain != NULL );
	ASSERT( ptrTerrain->tr_ptrTerrain != NULL );

	CTerrainImp	*ptr = ptrTerrain->tr_ptrTerrain;
	PIX			pixHeightMapWidth;
	PIX			pixHeightMapHeight;
	PIX			pixTopMapWidth;
	PIX			pixTopMapHeight;
	SLONG		slShadowMapSizeAspect;
	SLONG		slShadingMapSizeAspect;
	INDEX		cttrl;
	INDEX		cttrs;
	INDEX		iFirstTopMapLOD;
	
	// Terrain global data
	istrFile->ExpectID_t( "TRGD" );
	(*istrFile) >> pixHeightMapWidth;
	(*istrFile) >> pixHeightMapHeight;
	(*istrFile) >> pixTopMapWidth;
	(*istrFile) >> pixTopMapHeight;
	(*istrFile) >> slShadowMapSizeAspect;
	(*istrFile) >> slShadingMapSizeAspect;
	(*istrFile) >> iFirstTopMapLOD;
	(*istrFile) >> cttrl;
	(*istrFile) >> cttrs;
	(*istrFile) >> ptr->tr_fDistFactor;
	(*istrFile) >> ptr->tr_vStretch;
	(*istrFile) >> ptr->tr_vMetricSize;

	const PIX	pixEdgeMapWidth = pixHeightMapWidth - 1;
	const PIX	pixEdgeMapHeight = pixHeightMapHeight - 1;
	const PIX	pixShadowMapWidth = GetShadowMapWidth( pixHeightMapWidth, slShadowMapSizeAspect );
	const PIX	pixShadowMapHeight = GetShadowMapHeight( pixHeightMapHeight, slShadowMapSizeAspect );
	const SLONG	slHeightMapSize = pixHeightMapWidth * pixHeightMapHeight * sizeof(UWORD);
	const SLONG	slEdgeMapSize = pixEdgeMapWidth * pixEdgeMapHeight * sizeof(UBYTE);
	const SLONG	slShadowMapSize = pixShadowMapWidth * pixShadowMapHeight * sizeof(ULONG);
	const SLONG	slTopMapSize = pixTopMapWidth * pixTopMapHeight * sizeof(ULONG);

	ptr->tr_slShadowMapSizeAspect = slShadowMapSizeAspect;
	ptr->tr_slShadingMapSizeAspect = slShadingMapSizeAspect;
	ptr->tr_aptdShadowMaps.New( cttrs );
	ptr->tr_aulShadowTimes.New( cttrs + 1 );
	ptr->tr_afShadowBlurRadius.New( cttrs );
	ptr->tr_acolObjShadowColor.New( cttrs );
	for( INDEX itrs = 0; itrs < cttrs; itrs++ )
		ptr->tr_aptdShadowMaps[itrs] = TR_CreateTexture();

	extern BOOL	_bWorldEditorApp;
	if( _bWorldEditorApp )
		ptr->tr_ptdShadowMap = TR_CreateTexture();

	ptr->tr_ptdEdgeMap = TR_CreateTexture();
	ptr->tr_ptdTopMap = TR_CreateTexture();

	TR_SetHeightMapSize( ptrTerrain, pixHeightMapWidth, pixHeightMapHeight );

	UWORD	*puwHeightMap = ptr->tr_puwHeightMap;
	UBYTE	*pubEdgeMap = ptr->tr_pubEdgeMap;

	// Terrain height map
	istrFile->ExpectID_t( "TRHM" );
	istrFile->Read_t( puwHeightMap, slHeightMapSize );

	// Terrain edge map
	istrFile->ExpectID_t( "TREM" );
	istrFile->Read_t( pubEdgeMap, slEdgeMapSize );

	// Upload edge map
	TR_UploadEdgeMap( ptrTerrain );

	// Terrain shadow map
	istrFile->ExpectID_t( "TRSM" );
	istrFile->Seek_t( slShadowMapSize, CTStream::SD_CUR );
	for( itrs = 0; itrs < cttrs; itrs++ )
	{
		ULONG	ulShadowTime;
		istrFile->Read_t( &ulShadowTime, sizeof(ULONG) );
		ptr->tr_aulShadowTimes[itrs] = ulShadowTime;

		FLOAT	fBlurRadius;
		istrFile->Read_t( &fBlurRadius, sizeof(FLOAT) );
		ptr->tr_afShadowBlurRadius[itrs] = fBlurRadius;

		ptr->tr_acolObjShadowColor[itrs].abgr = 0xFF000000;

		istrFile->Seek_t( slShadowMapSize, CTStream::SD_CUR );
	}
	ULONG	ulLastShadowTime;
	istrFile->Read_t( &ulLastShadowTime, sizeof(ULONG) );
	ptr->tr_aulShadowTimes[itrs] = ulLastShadowTime;

	// Terrain top map
	ptr->tr_pixTopMapWidth = pixTopMapWidth;
	ptr->tr_pixTopMapHeight = pixTopMapHeight;
	ptr->tr_iFirstTopMapLOD = iFirstTopMapLOD;
	istrFile->ExpectID_t( "TRTM" );
	// if terrain uses top map
	if( iFirstTopMapLOD >= 0 )
	{
		// Upload terrain top map
		TR_PrepareTexture( *ptr->tr_ptdTopMap, pixTopMapWidth, pixTopMapHeight, PTF_STATIC );
		istrFile->Read_t( ptr->tr_ptdTopMap->td_pulFrames, slTopMapSize );
		TR_UploadTexture( *ptr->tr_ptdTopMap, pixTopMapWidth, pixTopMapHeight, PTF_STATIC );
		// Flag terrain that it can use top map
		ptrTerrain->tr_ulTerrainFlags |= TR_USES_TOPMAP;
	}
	else
	{
		// Flag terrain that if can't use top map
		ptrTerrain->tr_ulTerrainFlags &= ~TR_USES_TOPMAP;
	}

	// read terrain layers
	for( INDEX itrl = 0; itrl < cttrl; itrl++ )
	{
		// Terrain layer texture
		CTFileName	fnmTexture;
		istrFile->ExpectID_t( "TRLT" );
		(*istrFile) >> fnmTexture;

		INDEX			iTerrainLayer = TRL_CreateLayer_t( ptrTerrain, fnmTexture );
		CTerrainLayer	&tlLayer = ptr->tr_atlLayers[iTerrainLayer];
		// write layer data
		TRL_ReadLayerData_t( ptrTerrain, iTerrainLayer, istrFile );
	}

	// Terrain data end
	istrFile->ExpectID_t( "TRDE" );
}
// yjpark     -->|

static void ReadTerrainVersion13_t(CTerrain *ptrTerrain, CTStream *istrFile)
{
	ASSERT(ptrTerrain!=NULL);
	ASSERT(ptrTerrain->tr_ptrTerrain!=NULL);

	CTerrainImp *ptr = ptrTerrain->tr_ptrTerrain;
	PIX pixHeightMapWidth;
	PIX pixHeightMapHeight;
	PIX pixTopMapWidth;
	PIX pixTopMapHeight;
	SLONG slShadowMapSizeAspect;
	SLONG slShadingMapSizeAspect;
	INDEX cttrl;
	INDEX iFirstTopMapLOD;

	// Terrain global data
	istrFile->ExpectID_t("TRGD");
	(*istrFile)>>pixHeightMapWidth;
	(*istrFile)>>pixHeightMapHeight;
	(*istrFile)>>pixTopMapWidth;
	(*istrFile)>>pixTopMapHeight;
	(*istrFile)>>slShadowMapSizeAspect;
	(*istrFile)>>slShadingMapSizeAspect;
	(*istrFile)>>iFirstTopMapLOD;
	(*istrFile)>>cttrl;
	(*istrFile)>>ptr->tr_fDistFactor;
	(*istrFile)>>ptr->tr_vStretch;
	(*istrFile)>>ptr->tr_vMetricSize;

	const PIX pixEdgeMapWidth    = pixHeightMapWidth-1;
	const PIX pixEdgeMapHeight   = pixHeightMapHeight-1;
	const PIX pixShadowMapWidth  = GetShadowMapWidth(pixHeightMapWidth, slShadowMapSizeAspect);
	const PIX pixShadowMapHeight = GetShadowMapHeight(pixHeightMapHeight, slShadowMapSizeAspect);
	const SLONG slHeightMapSize  = pixHeightMapWidth*pixHeightMapHeight*sizeof(UWORD);
	const SLONG slEdgeMapSize    = pixEdgeMapWidth*pixEdgeMapHeight*sizeof(UBYTE);
	const SLONG slShadowMapSize  = pixShadowMapWidth*pixShadowMapHeight*sizeof(ULONG);
	const SLONG slTopMapSize     = pixTopMapWidth*pixTopMapHeight*sizeof(ULONG);

	ptr->tr_ptdEdgeMap = TR_CreateTexture();						// yjpark
	ptr->tr_ptdTopMap = TR_CreateTexture();							// yjpark
	ptr->tr_slShadowMapSizeAspect = slShadowMapSizeAspect;			// yjpark
	ptr->tr_slShadingMapSizeAspect = slShadingMapSizeAspect;		// yjpark
	TR_SetHeightMapSize(ptrTerrain, pixHeightMapWidth, pixHeightMapHeight);  

	UWORD *puwHeightMap = ptr->tr_puwHeightMap;
	UBYTE *pubEdgeMap   = ptr->tr_pubEdgeMap;
	ULONG *pulShadowMap = ptr->tr_ptdShadowMap->td_pulFrames;

	// Terrain height map
	istrFile->ExpectID_t("TRHM");
	istrFile->Read_t(puwHeightMap,slHeightMapSize);

	// Terrain edge map
	istrFile->ExpectID_t("TREM");
	istrFile->Read_t(pubEdgeMap,slEdgeMapSize);

	// Upload edge map
	TR_UploadEdgeMap(ptrTerrain);

	// Terrain shadow map
	TR_PrepareTexture(*ptr->tr_ptdShadowMap,pixShadowMapWidth,pixShadowMapHeight,PTF_STATIC);
	istrFile->ExpectID_t("TRSM");
	istrFile->Read_t(pulShadowMap,slShadowMapSize);
	TR_UploadTexture(*ptr->tr_ptdShadowMap,pixShadowMapWidth,pixShadowMapHeight,PTF_STATIC);

	// Terrain top map
	ptr->tr_pixTopMapWidth = pixTopMapWidth;
	ptr->tr_pixTopMapHeight = pixTopMapHeight;
	ptr->tr_iFirstTopMapLOD = iFirstTopMapLOD;
	istrFile->ExpectID_t("TRTM");
	// if terrain uses top map
	if(iFirstTopMapLOD>=0) {
		// Upload terrain top map
		TR_PrepareTexture(*ptr->tr_ptdTopMap,pixTopMapWidth,pixTopMapHeight,PTF_STATIC);
		istrFile->Read_t(ptr->tr_ptdTopMap->td_pulFrames,slTopMapSize);
		TR_UploadTexture(*ptr->tr_ptdTopMap,pixTopMapWidth,pixTopMapHeight,PTF_STATIC);
		// Flag terrain that it can use top map
		ptrTerrain->tr_ulTerrainFlags|=TR_USES_TOPMAP;
	} else {
		// Flag terrain that if can't use top map
		ptrTerrain->tr_ulTerrainFlags&=~TR_USES_TOPMAP;
	}


	// read terrain layers
	for(INDEX itrl=0;itrl<cttrl;itrl++) {

		// Terrain layer texture
		CTFileName fnmTexture;
		istrFile->ExpectID_t("TRLT");
		(*istrFile)>>fnmTexture;

		INDEX iTerrainLayer = TRL_CreateLayer_t(ptrTerrain, fnmTexture);
		CTerrainLayer &tlLayer = ptr->tr_atlLayers[iTerrainLayer];
		// write layer data
		TRL_ReadLayerData_t(ptrTerrain, iTerrainLayer, istrFile);
	}

	// Terrain data end
	istrFile->ExpectID_t("TRDE");
}

static void ReadTerrainVersion12_t(CTerrain *ptrTerrain, CTStream *istrFile)
{
	ASSERT(ptrTerrain!=NULL);
	ASSERT(ptrTerrain->tr_ptrTerrain!=NULL);

	CTerrainImp *ptr = ptrTerrain->tr_ptrTerrain;
	PIX pixHeightMapWidth;
	PIX pixHeightMapHeight;
	SLONG slShadowMapSizeAspect;
	SLONG slShadingMapSizeAspect;
	INDEX cttrl;

	// Terrain global data
	istrFile->ExpectID_t("TRGD");
	(*istrFile)>>pixHeightMapWidth;
	(*istrFile)>>pixHeightMapHeight;
	(*istrFile)>>slShadowMapSizeAspect;
	(*istrFile)>>slShadingMapSizeAspect;
	(*istrFile)>>cttrl;
	(*istrFile)>>ptr->tr_fDistFactor;
	(*istrFile)>>ptr->tr_vStretch;
	(*istrFile)>>ptr->tr_vMetricSize;

	const PIX pixEdgeMapWidth    = pixHeightMapWidth-1;
	const PIX pixEdgeMapHeight   = pixHeightMapHeight-1;
	const PIX pixShadowMapWidth  = GetShadowMapWidth(pixHeightMapWidth, slShadowMapSizeAspect);
	const PIX pixShadowMapHeight = GetShadowMapHeight(pixHeightMapHeight, slShadowMapSizeAspect);
	const SLONG slHeightMapSize  = pixHeightMapWidth*pixHeightMapHeight*sizeof(UWORD);
	const SLONG slEdgeMapSize    = pixEdgeMapWidth*pixEdgeMapHeight*sizeof(UBYTE);
	const SLONG slShadowMapSize  = pixShadowMapWidth*pixShadowMapHeight*sizeof(ULONG);

	ptr->tr_ptdEdgeMap = TR_CreateTexture();						// yjpark
	ptr->tr_ptdTopMap = TR_CreateTexture();							// yjpark
	ptr->tr_slShadowMapSizeAspect = slShadowMapSizeAspect;			// yjpark
	ptr->tr_slShadingMapSizeAspect = slShadingMapSizeAspect;		// yjpark
	TR_SetHeightMapSize(ptrTerrain, pixHeightMapWidth, pixHeightMapHeight);

	UWORD *puwHeightMap = ptr->tr_puwHeightMap;
	UBYTE *pubEdgeMap   = ptr->tr_pubEdgeMap;
	ULONG *pulShadowMap = ptr->tr_ptdShadowMap->td_pulFrames;

	// Terrain height map
	istrFile->ExpectID_t("TRHM");
	istrFile->Read_t(puwHeightMap,slHeightMapSize);

	// Terrain edge map
	istrFile->ExpectID_t("TREM");
	istrFile->Read_t(pubEdgeMap,slEdgeMapSize);

	// Upload edge map
	TR_UploadEdgeMap(ptrTerrain);

	// Terrain shadow map
	TR_PrepareTexture(*ptr->tr_ptdShadowMap,pixShadowMapWidth,pixShadowMapHeight,PTF_STATIC);
	istrFile->ExpectID_t("TRSM");
	istrFile->Read_t(pulShadowMap,slShadowMapSize);
	TR_UploadTexture(*ptr->tr_ptdShadowMap,pixShadowMapWidth,pixShadowMapHeight,PTF_STATIC);

	// read terrain layers
	for(INDEX itrl=0;itrl<cttrl;itrl++) {

		// Terrain layer texture
		CTFileName fnmTexture;
		istrFile->ExpectID_t("TRLT");
		(*istrFile)>>fnmTexture;

		INDEX iTerrainLayer = TRL_CreateLayer_t(ptrTerrain, fnmTexture);
		CTerrainLayer &tlLayer = ptr->tr_atlLayers[iTerrainLayer];
		// write layer data
		TRL_ReadLayerData_t(ptrTerrain, iTerrainLayer, istrFile);
	}

	// if this is world editor app
	extern BOOL _bWorldEditorApp;
	if(_bWorldEditorApp) {
		// Bake terrain top map
		TR_BakeTopMap(ptrTerrain);
		// Flag terrain that it can use top map
		ptrTerrain->tr_ulTerrainFlags|=TR_USES_TOPMAP;
	} else {
		// Flag terrain that if can't use top map
		ptrTerrain->tr_ulTerrainFlags&=~TR_USES_TOPMAP;
	}

	// Terrain data end
	istrFile->ExpectID_t("TRDE");
}
*/

static void ReadTerrainVersion9_t(CTerrain *ptrTerrain, CTStream *istrFile)
{
	ASSERT(ptrTerrain!=NULL);
	ASSERT(ptrTerrain->tr_ptrTerrain!=NULL);

	CTerrainImp *ptr = ptrTerrain->tr_ptrTerrain;
	PIX pixHeightMapWidth;
	PIX pixHeightMapHeight;
	SLONG slShadowMapSizeAspect;
	SLONG slShadingMapSizeAspect;
	INDEX cttrl;
	INDEX iDummyIndex;
	FLOAT fDummyFloat;

	(*istrFile)>>pixHeightMapWidth;
	(*istrFile)>>pixHeightMapHeight;
	(*istrFile)>>ptr->tr_vStretch;
	(*istrFile)>>ptr->tr_fDistFactor;
	(*istrFile)>>ptr->tr_vMetricSize;
	
	istrFile->ExpectID_t("TRSM"); // 'Terrain shadowmap'

	(*istrFile)>>slShadowMapSizeAspect;
	(*istrFile)>>slShadingMapSizeAspect;

	ptr->tr_ptdEdgeMap = TR_CreateTexture();						// yjpark
	ptr->tr_ptdTopMap = TR_CreateTexture();							// yjpark
	ptr->tr_slShadowMapSizeAspect = slShadowMapSizeAspect;			// yjpark
	ptr->tr_slShadingMapSizeAspect = slShadingMapSizeAspect;		// yjpark
	TR_SetHeightMapSize(ptrTerrain, pixHeightMapWidth, pixHeightMapHeight);
	TR_FillEdgeMap(ptrTerrain, 0xFF);

	const SLONG pixOldEdgeMapWidth    = pixHeightMapWidth;
	const SLONG pixOldEdgeMapHeight   = pixHeightMapHeight;
	const SLONG pixNewEdgeMapWidth    = pixHeightMapWidth-1;
	const SLONG pixNewEdgeMapHeight   = pixHeightMapHeight-1;

	const SLONG pixOldLayerMaskWidth  = pixHeightMapWidth;
	const SLONG pixOldLayerMaskHeight = pixHeightMapHeight;
	const SLONG pixNewLayerMaskWidth  = pixHeightMapWidth-1;
	const SLONG pixNewLayerMaskHeight = pixHeightMapHeight-1;

	const PIX pixShadowMapWidth      = GetShadowMapWidth(pixHeightMapWidth,slShadowMapSizeAspect);
	const PIX pixShadowMapHeight     = GetShadowMapHeight(pixHeightMapWidth,slShadowMapSizeAspect);
	const PIX pixShadingMapWidth     = GetShadingMapWidth(pixShadowMapWidth,slShadingMapSizeAspect);
	const PIX pixShadingMapHeight    = GetShadingMapHeight(pixShadowMapHeight,slShadingMapSizeAspect);

	const SLONG slHeightMapSize      = pixHeightMapWidth * pixHeightMapHeight * sizeof(UWORD);
	const SLONG slShadowMapSize      = pixShadowMapWidth * pixShadowMapHeight * sizeof(ULONG);
	const SLONG slShadingMapSize     = pixShadingMapWidth * pixShadingMapHeight * sizeof(UWORD);
	const SLONG slOldEdgeMapSize     = pixOldEdgeMapWidth * pixOldEdgeMapHeight * sizeof(UBYTE);
	const SLONG slNewEdgeMapSize     = pixNewEdgeMapWidth * pixNewEdgeMapHeight * sizeof(UBYTE);
	const SLONG slOldLayerMaskSize   = pixOldLayerMaskWidth * pixOldLayerMaskHeight * sizeof(UBYTE);
	const SLONG slNewLayerMaskSize   = pixNewLayerMaskWidth * pixNewLayerMaskHeight * sizeof(UBYTE);

	UWORD *puwHeightMap              = ptr->tr_puwHeightMap;
	UBYTE *pubEdgeMap                = ptr->tr_pubEdgeMap;
	UBYTE *pubTempEdgeMap            = (UBYTE*)AllocMemory(slOldEdgeMapSize);
	UBYTE *pubTempLayerMask          = (UBYTE*)AllocMemory(slOldLayerMaskSize);
	ULONG *pulTempShadingMap         = (ULONG*)AllocMemory(slShadingMapSize);

	TR_PrepareTexture(*ptr->tr_ptdShadowMap,pixShadowMapWidth,pixShadowMapHeight,PTF_STATIC);
	ULONG *pulShadowMap = ptr->tr_ptdShadowMap->td_pulFrames;
	istrFile->Read_t(pulShadowMap,slShadowMapSize);
	TR_UploadTexture(*ptr->tr_ptdShadowMap,pixShadowMapWidth,pixShadowMapHeight,PTF_STATIC);

	istrFile->Read_t(pulTempShadingMap,slShadingMapSize);
	FreeMemory(pulTempShadingMap);
	istrFile->ExpectID_t("TSEN"); // 'Terrain shadowmap end'

	// if there is edge map saved
	if(istrFile->PeekID_t()==CChunkID("TREM")) { // 'Terrain edge map'
		// Read terrain edge map
		istrFile->ExpectID_t("TREM");
		istrFile->Read_t(pubTempEdgeMap,slOldEdgeMapSize);
		istrFile->ExpectID_t("TEEN");

		// Crop edge map to new size
		const UBYTE *pubSrc = pubTempEdgeMap;
		UBYTE *pubDst = pubEdgeMap;
		TerrainMapCrop(pubDst, pixNewEdgeMapWidth, pixNewEdgeMapHeight, pubSrc, pixOldEdgeMapWidth, pixOldEdgeMapHeight, 0xFF);

		// Upload edge map texture
		TR_UploadEdgeMap(ptrTerrain);
	}
	
	// Read height map
	istrFile->ExpectID_t("TRHM");  // 'Terrain heightmap'
	istrFile->Read_t(puwHeightMap,slHeightMapSize);
	istrFile->ExpectID_t("THEN");  // 'Terrain heightmap end'

	// Read Terrain layers
	istrFile->ExpectID_t("TRLR");  // 'Terrain layers'
	(*istrFile)>>cttrl;
	// for each terrain layer
	for(INDEX itrl=0;itrl<cttrl;itrl++) {
		// Read terrain layer texture
		CTFileName fnmTexture;
		istrFile->ExpectID_t("TLTX");  // 'Terrain layer texture'
		(*istrFile)>>fnmTexture;
		// Read terrain layer mask
		(*istrFile).ExpectID_t("TLMA"); // 'Terrain layer mask'
		(*istrFile)>>iDummyIndex;
		(*istrFile)>>iDummyIndex;

		// Create new terrain layer
		INDEX iTerrainLayer = TRL_CreateLayer_t(ptrTerrain, fnmTexture);
		TRL_FillLayerData(ptrTerrain, iTerrainLayer, 0xFF);
		CTerrainLayer *ptlLayer = &ptr->tr_atlLayers[iTerrainLayer];
		CTerrainLayerImp *ptrl = ptlLayer->trl_trlLayerImp;

		// Read old layer mask
		istrFile->Read_t(pubTempLayerMask, slOldLayerMaskSize);

		// Crop layer mask to new size
		const UBYTE *pubSrc = pubTempLayerMask;
		UBYTE *pubDst = ptrl->trl_pubLayerMask;
		TerrainMapCrop(pubDst, pixNewLayerMaskWidth, pixNewLayerMaskHeight, pubSrc, pixOldLayerMaskWidth, pixOldLayerMaskHeight, 0xFF);

		// upload layer mask
		TR_UploadLayerMask(ptrTerrain, iTerrainLayer);

		// Read terrain layer params
		(*istrFile).ExpectID_t("TLPR"); // 'Terrain layer params'

		(*istrFile)>>ptlLayer->tl_strName;
		(*istrFile)>>ptlLayer->tl_bVisible;

		(*istrFile)>>ptlLayer->tl_fRotateX;
		(*istrFile)>>ptlLayer->tl_fRotateY;
		(*istrFile)>>ptlLayer->tl_fStretchX;
		(*istrFile)>>ptlLayer->tl_fStretchY;
		(*istrFile)>>ptlLayer->tl_fOffsetX;
		(*istrFile)>>ptlLayer->tl_fOffsetY;

		(*istrFile)>>ptlLayer->tl_bAutoRegenerated;
		(*istrFile)>>ptlLayer->tl_fCoverage;
		(*istrFile)>>ptlLayer->tl_fCoverageNoise;
		(*istrFile)>>ptlLayer->tl_fCoverageRandom;

		(*istrFile)>>ptlLayer->tl_bApplyMinAltitude;
		(*istrFile)>>ptlLayer->tl_fMinAltitude;
		(*istrFile)>>ptlLayer->tl_fMinAltitudeFade;
		(*istrFile)>>ptlLayer->tl_fMinAltitudeNoise;
		(*istrFile)>>ptlLayer->tl_fMinAltitudeRandom;

		(*istrFile)>>ptlLayer->tl_bApplyMaxAltitude;
		(*istrFile)>>ptlLayer->tl_fMaxAltitude;
		(*istrFile)>>ptlLayer->tl_fMaxAltitudeFade;
		(*istrFile)>>ptlLayer->tl_fMaxAltitudeNoise;
		(*istrFile)>>ptlLayer->tl_fMaxAltitudeRandom;

		(*istrFile)>>ptlLayer->tl_bApplyMinSlope;
		(*istrFile)>>ptlLayer->tl_fMinSlope;
		(*istrFile)>>ptlLayer->tl_fMinSlopeFade;
		(*istrFile)>>ptlLayer->tl_fMinSlopeNoise;
		(*istrFile)>>ptlLayer->tl_fMinSlopeRandom;

		(*istrFile)>>ptlLayer->tl_bApplyMaxSlope;
		(*istrFile)>>ptlLayer->tl_fMaxSlope;
		(*istrFile)>>ptlLayer->tl_fMaxSlopeFade;
		(*istrFile)>>ptlLayer->tl_fMaxSlopeNoise;
		(*istrFile)>>ptlLayer->tl_fMaxSlopeRandom;

		(*istrFile)>>ptlLayer->tl_colMultiply;
		(*istrFile)>>fDummyFloat; // tl_fSmoothness
		(*istrFile)>>iDummyIndex; // tl_ltType

		// Tile layer properties
		(*istrFile)>>iDummyIndex; // tl_ctTilesInRow
		(*istrFile)>>iDummyIndex; // tl_ctTilesInCol;
		(*istrFile)>>iDummyIndex;
		(*istrFile)>>iDummyIndex; // tl_pixTileWidth;
		(*istrFile)>>iDummyIndex; // tl_pixTileHeight;
		(*istrFile)>>iDummyIndex; // tl_fTileU;
		(*istrFile)>>iDummyIndex; // tl_fTileV;

		TRL_ApplyLayerMatrix(ptrTerrain, iTerrainLayer);
	}

	FreeMemory(pubTempEdgeMap);
	FreeMemory(pubTempLayerMask);

	(*istrFile).ExpectID_t("TLEN");  // 'Terrain layers end'

	// if this is world editor app
	extern BOOL _bWorldEditorApp;
	if(_bWorldEditorApp) {
		// Bake terrain top map
		TR_BakeTopMap(ptrTerrain);
		// Flag terrain that it can use top map
		ptrTerrain->tr_ulTerrainFlags|=TR_USES_TOPMAP;
	} else {
		// Flag terrain that if can't use top map
		ptrTerrain->tr_ulTerrainFlags&=~TR_USES_TOPMAP;
	}

	// End of reading
	(*istrFile).ExpectID_t("TREN");  // 'terrain end'
}

// Read from stream
void CTerrain::Read_t(CTStream *istrFile)
{
	ASSERT(tr_ptrTerrain!=NULL);
	CTerrainImp *ptr = tr_ptrTerrain;

	_bLoadTerrain = TRUE;		// yjpark

	// Peek terrain version
	CChunkID cid = istrFile->PeekID_t();
	// if this is old terrain version
	if(cid==CChunkID("TERR")) {
		// Terrain version
		istrFile->ExpectID_t("TERR");
		(*istrFile) >> ptr->tr_iTerrainVersion;

		if(ptr->tr_iTerrainVersion==9) {
			ReadTerrainVersion9_t(this,istrFile);
			ASSERT(_CrtCheckMemory());
		} else {
			ThrowF_t( TRANS("The terrain version on disk is %d.\n"
				"Current supported version is %d."), ptr->tr_iTerrainVersion, TR_FNM_VERSION);
		}

	// else must be new one
	}
	else
	{
		// Terrain version
		istrFile->ExpectID_t("TRVR");
		(*istrFile) >> ptr->tr_iTerrainVersion;

		//[070626: Su-won] Terrain Ver.19
		if( ptr->tr_iTerrainVersion >= 19 )
		{
			ReadTerrainVersion17_t( this, istrFile );
			ASSERT( _CrtCheckMemory() );
		}
		else if( ptr->tr_iTerrainVersion == 18 )				// yjpark |<--
		{
			ReadTerrainVersion17_t( this, istrFile );
			ASSERT( _CrtCheckMemory() );
		}
		else if( ptr->tr_iTerrainVersion == 17 )
		{
			ReadTerrainVersion17_t( this, istrFile );
			ASSERT( _CrtCheckMemory() );
		}
		else if( ptr->tr_iTerrainVersion == 16 )
		{
			ReadTerrainVersion16_t( this, istrFile );
			ASSERT( _CrtCheckMemory() );
		}
		/*else if( ptr->tr_iTerrainVersion == 15 )
		{
			ReadTerrainVersion15_t( this, istrFile );
			ASSERT( _CrtCheckMemory() );
		}
		else if( ptr->tr_iTerrainVersion == 14 )
		{
			ReadTerrainVersion14_t( this, istrFile );
			ASSERT( _CrtCheckMemory() );
		}												// yjpark     -->|
		else if( ptr->tr_iTerrainVersion == 13 )
		{
			ReadTerrainVersion13_t( this, istrFile );
			ASSERT( _CrtCheckMemory() );
		}
		else if( ptr->tr_iTerrainVersion == 12 )
		{
			ReadTerrainVersion12_t( this, istrFile );
			ASSERT( _CrtCheckMemory() );
		}*/
		else
		{
			ThrowF_t( TRANS("The terrain version on disk is %d.\n"
				"Current supported version is %d."), ptr->tr_iTerrainVersion, TR_FNM_VERSION);
		}
	}

	TR_RegenerateAll(this);

	_bLoadTerrain = FALSE;		// yjpark
}

// Write to stream
void CTerrain::Write_t(CTStream *ostrFile)
{
	ASSERT( tr_ptrTerrain != NULL );
	const CTerrainImp	*ptr = tr_ptrTerrain;

	const UWORD *puwHeightMap          = ptr->tr_puwHeightMap;
	const UBYTE *pubEdgeMap            = ptr->tr_pubEdgeMap;
	const ULONG *pulTopMap             = ptr->tr_ptdTopMap->td_pulFrames;
	const UWORD *pubAttributeMap       = ptr->tr_pubAttributeMap;	//[070626: Su-won] Terrain Ver.19
	const SLONG slShadowMapSizeAspect  = ptr->tr_slShadowMapSizeAspect;
	const SLONG slShadingMapSizeAspect = ptr->tr_slShadingMapSizeAspect;
	const SLONG slAttributeMapSizeAspect = ptr->tr_slAttributeMapSizeAspect;	//[070626: Su-won] Terrain Ver.19	
	const PIX pixHeightMapWidth        = ptr->tr_pixHeightMapWidth;
	const PIX pixHeightMapHeight       = ptr->tr_pixHeightMapHeight;
	const PIX pixEdgeMapWidth          = ptr->tr_pixHeightMapWidth-1;
	const PIX pixEdgeMapHeight         = ptr->tr_pixHeightMapHeight-1;
	const PIX pixShadowMapWidth        = GetShadowMapWidth(pixHeightMapWidth, slShadowMapSizeAspect);
	const PIX pixShadowMapHeight       = GetShadowMapHeight(pixHeightMapHeight, slShadowMapSizeAspect);
	const PIX pixTopMapWidth           = ptr->tr_pixTopMapWidth;
	const PIX pixTopMapHeight          = ptr->tr_pixTopMapHeight;
	const FLOAT fDistFactor            = ptr->tr_fDistFactor;
	const FLOAT3D vStretch             = ptr->tr_vStretch;
	const FLOAT3D vMetricSize          = ptr->tr_vMetricSize;
																		
	const SLONG slHeightMapSize        = pixHeightMapWidth*pixHeightMapHeight*sizeof(UWORD);
	const SLONG slEdgeMapSize          = pixEdgeMapWidth*pixEdgeMapHeight*sizeof(UBYTE);
	const SLONG slShadowMapSize        = pixShadowMapWidth*pixShadowMapHeight*sizeof(ULONG);
	const SLONG slTopMapSize           = pixTopMapWidth*pixTopMapHeight*sizeof(ULONG);
	const INDEX cttrl                  = ptr->tr_atlLayers.Count();
	const INDEX cttrs                  = ptr->tr_aptdShadowMaps.Count();		// yjpark
	const BOOL  bUseTopMap             = tr_ulTerrainFlags&TR_USES_TOPMAP;
	const INDEX iFirstTopMapLOD        = bUseTopMap ? ptr->tr_iFirstTopMapLOD : -1;

	// Terrain version
	ostrFile->WriteID_t("TRVR");
	(*ostrFile)<<TR_FNM_VERSION;

	// Terrain global data
	ostrFile->WriteID_t("TRGD");
	(*ostrFile)<<pixHeightMapWidth;
	(*ostrFile)<<pixHeightMapHeight;
	(*ostrFile)<<pixTopMapWidth;
	(*ostrFile)<<pixTopMapHeight;
	(*ostrFile)<<slShadowMapSizeAspect;
	(*ostrFile)<<slShadingMapSizeAspect;
	if( TR_FNM_VERSION>=19 )
		(*ostrFile)<<slAttributeMapSizeAspect;		//[070626: Su-won] Terrain Ver.19
	(*ostrFile)<<iFirstTopMapLOD;
	(*ostrFile)<<cttrl;
	(*ostrFile)<<cttrs;				// yjpark
	(*ostrFile)<<fDistFactor;
	(*ostrFile)<<vStretch;
	(*ostrFile)<<vMetricSize;

	// Directory desc
	CTFileName	fnmFullPath = ostrFile->strm_strStreamDescription;
	CTFileName	fnmDir = fnmFullPath.FileDir();
	CTFileName	fnmName = fnmFullPath.FileName();

	// Terrain height map
	ostrFile->WriteID_t("TRHM");
	ostrFile->Write_t(puwHeightMap,slHeightMapSize);

	// Terrain edge map
	ostrFile->WriteID_t("TREM");
	ostrFile->Write_t(pubEdgeMap,slEdgeMapSize);
	
	// File name of edge map
	CTFileName	fnmEdgeMap = fnmDir + CTString( "Texture\\" ) + fnmName + CTString( "_e.tex" );
	(*ostrFile) << fnmEdgeMap;

	// Write edge map texture
	ULONG	ulOldFlags = ptr->tr_ptdEdgeMap->td_ulFlags;
	ptr->tr_ptdEdgeMap->td_ulFlags &= ~( TEX_STATIC | TEX_CONSTANT );
	ptr->tr_ptdEdgeMap->td_ulFlags = TEX_COMPRESS | TEX_ALPHACHANNEL;
	ptr->tr_ptdEdgeMap->Save_t( fnmEdgeMap );
	ptr->tr_ptdEdgeMap->td_ulFlags = ulOldFlags;

	// Terrain shadow map
	ostrFile->WriteID_t("TRSM");
	ULONG	ulShadowTime;
	INDEX itrs;
	for( itrs = 0; itrs < cttrs; itrs++ )								// yjpark |<--
	{
		// World time for current shadow map
		ulShadowTime = ptr->tr_aulShadowTimes[itrs];
		ostrFile->Write_t( &ulShadowTime, sizeof(ULONG) );

		// Blur factor for current shadow map
		FLOAT	fBlurRadius = ptr->tr_afShadowBlurRadius[itrs];
		ostrFile->Write_t( &fBlurRadius, sizeof(FLOAT) );

		// Color for current object shadow map
		GFXColor	colShadow = ptr->tr_acolObjShadowColor[itrs];
		ostrFile->Write_t( &colShadow, sizeof(GFXColor) );

		// File name of current shadow map
		CTString	strShadowMap;
		strShadowMap.PrintF( "_s%d.tex", itrs );
		CTFileName	fnmShadowMap = fnmDir + CTString( "Texture\\" ) + fnmName + strShadowMap;
		(*ostrFile) << fnmShadowMap;

		// Write current shadow map texture - compressed
		ULONG	ulOldFlags = ptr->tr_aptdShadowMaps[itrs]->td_ulFlags;
		ptr->tr_aptdShadowMaps[itrs]->td_ulFlags &= ~( TEX_STATIC | TEX_CONSTANT );
		ptr->tr_aptdShadowMaps[itrs]->td_ulFlags |= TEX_COMPRESS;
		ptr->tr_aptdShadowMaps[itrs]->Save_t( fnmShadowMap );
		ptr->tr_aptdShadowMaps[itrs]->td_ulFlags = ulOldFlags;
	}
	ulShadowTime = ptr->tr_aulShadowTimes[itrs];
	ostrFile->Write_t( &ulShadowTime, sizeof(ULONG) );
	
	// Overbright strength for current shadow map
	FLOAT	fOverbright = ptr->tr_fShadowOverbright;
	ostrFile->Write_t( &fOverbright, sizeof(FLOAT) );						// yjpark     -->|

	// Terrain top map
	ostrFile->WriteID_t("TRTM");
	
	// File name of top map
	CTFileName	fnmTopMap = fnmDir + CTString( "Texture\\" ) + fnmName + CTString( "_t.tex" );
	(*ostrFile) << fnmTopMap;

	// if terrain uses top map
	if( iFirstTopMapLOD >= 0 )
	{
		// if top map must be synced
		if( tr_ulTerrainFlags & TR_SYNC_TOPMAP )
		{
			extern BOOL	_bWorldEditorApp;
			ASSERT( _bWorldEditorApp );
			TR_BakeTopMap( this );
		}

		// Write top map texture
		ULONG	ulOldFlags = ptr->tr_ptdTopMap->td_ulFlags;
		ptr->tr_ptdTopMap->td_ulFlags &= ~( TEX_STATIC | TEX_CONSTANT );
		ptr->tr_ptdTopMap->td_ulFlags = TEX_COMPRESS | TEX_ALPHACHANNEL;
		ptr->tr_ptdTopMap->Save_t( fnmTopMap );
		ptr->tr_ptdTopMap->td_ulFlags = ulOldFlags;
	}

	// write terrain layers
	for(INDEX itrl=0;itrl<cttrl;itrl++) {
		const CTerrainLayer &tlLayer = ptr->tr_atlLayers[itrl];
		const CTerrainLayerImp *ptrl = tlLayer.trl_trlLayerImp;
		const CTFileName &fnmTexture = ptrl->trl_ptdTexture->GetName();
		
		// Terrain layer texture
		ostrFile->WriteID_t("TRLT");
		(*ostrFile)<<fnmTexture;

		// write layer data
		TRL_WriteLayerData_t(this, itrl, ostrFile);
	}

	//[070626: Su-won] Terrain Ver.19
	//Write Attibute Map
	if (TR_FNM_VERSION >= 20)
	{
		const SLONG	slAttributeMapSize = ptr->tr_vMetricSize( 1 ) * ptr->tr_vMetricSize( 3 );
		ostrFile->WriteID_t("TRAM");
		ostrFile->Write_t(pubAttributeMap, slAttributeMapSize * sizeof(UWORD));
	}
	else if (TR_FNM_VERSION >= 19)
	{
		const SLONG	slAttributeMapSize = ptr->tr_vMetricSize( 1 ) * ptr->tr_vMetricSize( 3 );
		ostrFile->WriteID_t("TRAM");
		ostrFile->Write_t(pubAttributeMap, slAttributeMapSize);
	}

	// Terrain data end
	ostrFile->WriteID_t("TRDE");
}

// Clean terrain data
void CTerrain::Clear(void)
{
}


























