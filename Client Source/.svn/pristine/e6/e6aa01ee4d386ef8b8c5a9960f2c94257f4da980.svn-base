#include "stdh.h"
#include <Engine/Base/Stream.h>
#include <Engine/Terrain/Terrain.h>
#include <Engine/Terrain/Terrain.hpp>
#include <Engine/Terrain/TRLayer.h>
#include <Engine/Terrain/TRLayer.hpp>
#include <Engine/Terrain/TRGenerate.hpp>
#include <Engine/Math/Matrix12.h>
#include <Engine/Templates/Stock_CTextureData.h>
#include <Engine/Graphics/ImageInfo.h>
#include <Engine/Base/ErrorReporting.h>

static void ClearTerrainLayer(CTerrainLayerImp *ptrl);
extern BOOL _bFullLayerCopy;
extern BOOL	_bLoadTerrain;		// yjpark

template <class Type>
extern void TerrainMapCrop(Type *ptDst, SLONG slDstWidth, SLONG slDstHeight,
							const Type *ptSrc, SLONG slSrcWidth, SLONG slSrcHeight, UBYTE ubFill);

#define TRL_FNM_VERSION 15		// yjpark

CTerrainLayerImp::CTerrainLayerImp(void)
{
	trl_iLayerVersion = 0;			// yjpark
	trl_pubLayerMask = NULL;
	trl_ptdTexture   = NULL;
	trl_ptdLayerMask = NULL;

	trl_ptoTexObj =NULL;
}

CTerrainLayerImp::~CTerrainLayerImp(void)
{
	ClearTerrainLayer(this);
}

CTerrainLayerImp::CTerrainLayerImp(const CTerrainLayerImp &trlOther)
{
	ASSERTALWAYS("Forbiden");
}

extern void TRL_CopyLayerImp(CTerrain *ptrTerrain, INDEX iTerrainLayer, const CTerrainLayerImp &trlSrc)
{
	ASSERT(ptrTerrain!=NULL);                // must be valid
	ASSERT(ptrTerrain->tr_ptrTerrain!=NULL); // must be valid
	CTerrainImp *ptr = ptrTerrain->tr_ptrTerrain;
	CTerrainLayer *ptlLayer = &ptr->tr_atlLayers[iTerrainLayer];
	CTerrainLayerImp *ptrlDst = ptlLayer->trl_trlLayerImp;

	// Clear current layer
	ClearTerrainLayer(ptrlDst);

	ASSERT(ptrlDst->trl_ptdTexture==NULL);
	ASSERT(ptrlDst->trl_pubLayerMask==NULL);
	ASSERT(ptrlDst->trl_ptdLayerMask==NULL);
	ASSERT(trlSrc.trl_pubLayerMask!=NULL);

	// Copy precached matrix
	MatrixCopy(ptrlDst->trl_mTransform,trlSrc.trl_mTransform);

	// Copy layer mask
	if(trlSrc.trl_pubLayerMask!=NULL) {
		const SLONG slMaskSize = MEM_Size(trlSrc.trl_pubLayerMask);
		ptrlDst->trl_pubLayerMask = (UBYTE*)AllocMemory(slMaskSize);
		memcpy(ptrlDst->trl_pubLayerMask,trlSrc.trl_pubLayerMask,slMaskSize);
	}

	// if other layer has valid texture
	if(trlSrc.trl_ptdTexture!=NULL) {
		// Add reference in stock
		ptrlDst->trl_ptdTexture = trlSrc.trl_ptdTexture;
		ptrlDst->trl_ptdTexture->MarkUsed();
	}

	// if doing a quick copy
	if(!_bFullLayerCopy) {
		// if other layer has valid layer mask texture
		if(trlSrc.trl_ptdLayerMask!=NULL) {
			// 'Copy' layer mask texture data
			ptrlDst->trl_ptdLayerMask = trlSrc.trl_ptdLayerMask;
			TR_MarkTextureUsed(ptrlDst->trl_ptdLayerMask);
		}
	// if doing a full copy
	} else {
		// if other layer has valid layer mask texture
		if(trlSrc.trl_ptdLayerMask!=NULL) {
			ptrlDst->trl_ptdLayerMask = TR_CreateTexture();
			TR_UploadLayerMask(ptrTerrain, iTerrainLayer);
		}
	}
}

CTerrainLayerImp &CTerrainLayerImp::operator=(const CTerrainLayerImp &trlOther)
{
	ASSERTALWAYS("Forbiden");

	return *this;
}


static void ReleaseLayerTexture(CTerrainLayerImp *ptrl)
{
	// if layer texture is valid
	if(ptrl->trl_ptdTexture!=NULL) {
		// release texture from stock
		_pTextureStock->Release(ptrl->trl_ptdTexture);
		ptrl->trl_ptdTexture = NULL;
	}

	if( ptrl->trl_ptoTexObj )
	{
		delete ptrl->trl_ptoTexObj;
		ptrl->trl_ptoTexObj =NULL;
	}
}

static void ObtainLayerTexture_t(CTerrainLayerImp *ptrl, const CTFileName &fnm)
{
	// first release old texture
	ReleaseLayerTexture(ptrl);
	ptrl->trl_ptdTexture = _pTextureStock->Obtain_t(fnm);
	
	// All layer textures must be static for top map gen in world editor
	extern BOOL _bWorldEditorApp;
	if(_bWorldEditorApp) {
		const BOOL bTexIsStatic = ptrl->trl_ptdTexture->GetFlags()&TEX_STATIC;
		if(!bTexIsStatic) {
			ptrl->trl_ptdTexture->Force(TEX_STATIC);
			ptrl->trl_ptdTexture->Reload();
		} else {
			ASSERT(ptrl->trl_ptdTexture->td_pulFrames!=NULL);
		}
	}

	ptrl->trl_ptoTexObj = new CTextureObject;
	ptrl->trl_ptoTexObj->SetData( ptrl->trl_ptdTexture );
}

static void ClearTerrainLayer(CTerrainLayerImp *ptrl)
{
	// free layer mask memory if exists
	if(ptrl->trl_pubLayerMask!=NULL) {
		FreeMemory(ptrl->trl_pubLayerMask);
		ptrl->trl_pubLayerMask = NULL;
	}

	// free uploaded layer mask if it exists
	if( ptrl->trl_ptdLayerMask != NULL )
	{
		extern BOOL		_bWorldEditorApp;					// yjpark |<--
		if( _bWorldEditorApp || ptrl->trl_iLayerVersion < 14 )
			TR_MarkTextureUnused( ptrl->trl_ptdLayerMask );
		else
			_pTextureStock->Release( ptrl->trl_ptdLayerMask );

		ptrl->trl_ptdLayerMask = NULL;						// yjpark     -->|
	}

	// Release layer texture
	ReleaseLayerTexture(ptrl);
}

// Create new terrain layer
extern INDEX TRL_CreateLayer_t(CTerrain *ptrTerrain, const CTFileName &fnmTexture)
{
	ASSERT(ptrTerrain!=NULL);                // must be valid
	ASSERT(ptrTerrain->tr_ptrTerrain!=NULL); // must be valid

	CTerrainImp *ptr = ptrTerrain->tr_ptrTerrain;

	// Add new layer to terrain
	const INDEX ctLayers = ptr->tr_atlLayers.Count();
	const INDEX iLayerIndex = ctLayers;
	
	// Hack copy of static array
	CStaticArray<CTerrainLayer> atlOldLayers;
	atlOldLayers.sa_Array = ptr->tr_atlLayers.sa_Array;
	atlOldLayers.sa_Count = ptr->tr_atlLayers.sa_Count;
	ptr->tr_atlLayers.sa_Array = NULL;
	ptr->tr_atlLayers.sa_Count = 0;
	ptr->tr_atlLayers.New(ctLayers+1);

	for(INDEX itrl=0;itrl<ctLayers;itrl++) {
		const CTerrainLayer &tlSrc = atlOldLayers[itrl];
		ASSERT(tlSrc.trl_trlLayerImp->trl_pubLayerMask!=NULL);
		TRL_CopyLayer(ptrTerrain, itrl, tlSrc);
	}

	atlOldLayers.Clear();

	CTerrainLayer &tlLayer = ptr->tr_atlLayers[iLayerIndex];								// yjpark |<--
	CTerrainLayerImp *ptrl = tlLayer.trl_trlLayerImp;
	extern BOOL	_bWorldEditorApp;
	if( _bWorldEditorApp && !_bLoadTerrain )
	{
		// Allocate memory for layer mask
		const SLONG	slLayerMaskWidth = ( ptr->tr_pixHeightMapWidth - 1 ) << tlLayer.tl_slMaskStretch;
		const SLONG	slLayerMaskHeight = ( ptr->tr_pixHeightMapHeight - 1 ) << tlLayer.tl_slMaskStretch;
		const SLONG slLayerMaskSize = slLayerMaskWidth * slLayerMaskHeight * sizeof(UBYTE);
		ptrl->trl_pubLayerMask = (UBYTE *)AllocMemory( slLayerMaskSize );
	}																						// yjpark     -->|

	// Create layer mask texture
	if( _bWorldEditorApp || ptrl->trl_iLayerVersion < 14 )
		ptrl->trl_ptdLayerMask = TR_CreateTexture();

	// Set layer texture
	TRL_SetLayerTexture_t(ptrTerrain,iLayerIndex,fnmTexture);
	TRL_ApplyLayerMatrix(ptrTerrain,iLayerIndex);

	return iLayerIndex;
}

// Delete existing terrain layer
extern void TRL_DeleteLayer(CTerrain *ptrTerrain, INDEX iTerrainLayer, BOOL bDestroyingTerrain/*=FALSE*/)
{
	ASSERT(ptrTerrain!=NULL);                // must be valid
	ASSERT(ptrTerrain->tr_ptrTerrain!=NULL); // must be valid
	CTerrainImp *ptr = ptrTerrain->tr_ptrTerrain;

	const INDEX ctLayers = ptr->tr_atlLayers.Count();

	CStaticArray<CTerrainLayer> atlOldLayers;
	atlOldLayers.sa_Array = ptr->tr_atlLayers.sa_Array;
	atlOldLayers.sa_Count = ptr->tr_atlLayers.sa_Count;
	ptr->tr_atlLayers.sa_Array = NULL;
	ptr->tr_atlLayers.sa_Count = 0;
	ptr->tr_atlLayers.New(ctLayers-1);

	INDEX iNewLayersIndex = 0;
	for(INDEX itrl=0;itrl<ctLayers;itrl++) {
		if(itrl!=iTerrainLayer) {
			const CTerrainLayer &tlSrc = atlOldLayers[itrl];
			TRL_CopyLayer(ptrTerrain, iNewLayersIndex, tlSrc);
			iNewLayersIndex++;
		}
	}
	atlOldLayers.Clear();
	
	if(!bDestroyingTerrain) {
		extern void TR_GenerateTerrainTileOptimizationData(CTerrain *ptrTerrain);
		TR_GenerateTerrainTileOptimizationData(ptrTerrain);
	}
}

extern void TRL_SetLayerTexture_t(CTerrain *ptrTerrain, CTerrainLayer *ptlLayer, const CTFileName &fnmTexture)
{
	ASSERT(ptrTerrain!=NULL);                // must be valid
	ASSERT(ptrTerrain->tr_ptrTerrain!=NULL); // must be valid

	CTerrainLayerImp *ptrl = ptlLayer->trl_trlLayerImp;
	ObtainLayerTexture_t(ptrl,fnmTexture);
}

// Change layer texture
extern void TRL_SetLayerTexture_t(CTerrain *ptrTerrain, INDEX iTerrainLayer, const CTFileName &fnmTexture)
{
	ASSERT(ptrTerrain!=NULL);                // must be valid
	ASSERT(ptrTerrain->tr_ptrTerrain!=NULL); // must be valid

	CTerrainImp *ptr = ptrTerrain->tr_ptrTerrain;
	CTerrainLayer *ptlLayer = &ptr->tr_atlLayers[iTerrainLayer];
	TRL_SetLayerTexture_t(ptrTerrain,ptlLayer,fnmTexture);
}

extern void TRL_UpdateLayerSize(CTerrain *ptrTerrain, INDEX iTerrainLayer, PIX pixNewLayerMaskWidth, PIX pixNewLayerMaskHeight,
																PIX pixOldLayerMaskWidth, PIX pixOldLayerMaskHeight)
{
	ASSERT(ptrTerrain!=NULL);                // must be valid
	ASSERT(ptrTerrain->tr_ptrTerrain!=NULL); // must be valid
	CTerrainImp *ptr = ptrTerrain->tr_ptrTerrain;
	CTerrainLayer &tlLayer = ptr->tr_atlLayers[iTerrainLayer];
	CTerrainLayerImp *ptrl = tlLayer.trl_trlLayerImp;

	const PIX pixLayerMaskSize = pixNewLayerMaskWidth*pixNewLayerMaskHeight;
	const SLONG slLayerMaskSize = pixLayerMaskSize*sizeof(UBYTE);

	// Crop layer mask local data
	UBYTE *pubDst = (UBYTE*)AllocMemory(slLayerMaskSize);
	const UBYTE *pubSrc = ptrl->trl_pubLayerMask;
	TerrainMapCrop(pubDst,pixNewLayerMaskWidth, pixNewLayerMaskHeight, pubSrc, pixOldLayerMaskWidth, pixOldLayerMaskHeight,0xFF);

	// Set new map to terrain layer
	if(ptrl->trl_pubLayerMask!=NULL) {
		FreeMemory(ptrl->trl_pubLayerMask);
		ptrl->trl_pubLayerMask = NULL;
	}
	ptrl->trl_pubLayerMask = pubDst;
	
	// Apply it to texture
	TR_UploadLayerMask(ptrTerrain, iTerrainLayer, FALSE);
	ASSERT(_CrtCheckMemory());
}

// Fill layer with constant value
extern void TRL_FillLayerData(CTerrain *ptrTerrain, INDEX iTerrainLayer, UBYTE ubFill)
{
	ASSERT(ptrTerrain!=NULL);                // must be valid
	ASSERT(ptrTerrain->tr_ptrTerrain!=NULL); // must be valid

	CTerrainImp *ptr = ptrTerrain->tr_ptrTerrain;
	CTerrainLayer &tlLayer = ptr->tr_atlLayers[iTerrainLayer];
	CTerrainLayerImp *ptrl = tlLayer.trl_trlLayerImp;
	
	ASSERT(ptrl->trl_pubLayerMask!=NULL);

	const PIX pixLayerMaskWidth  = ( ptr->tr_pixHeightMapWidth - 1 ) << tlLayer.tl_slMaskStretch;			// yjpark
	const PIX pixLayerMaskHeight = ( ptr->tr_pixHeightMapHeight - 1 ) << tlLayer.tl_slMaskStretch;		// yjpark
	const PIX pixLayerMaskSize   = pixLayerMaskWidth*pixLayerMaskHeight;

	// Fill layer mask with constant value
	memset(ptrl->trl_pubLayerMask, ubFill, pixLayerMaskSize);
	// Upload layer mask texture
	TR_UploadLayerMask(ptrTerrain, iTerrainLayer);
}

// Precache layer matrix
extern void TRL_ApplyLayerMatrix(CTerrain *ptrTerrain, INDEX iTerrainLayer)
{
	ASSERT(ptrTerrain!=NULL);                // must be valid
	ASSERT(ptrTerrain->tr_ptrTerrain!=NULL); // must be valid

	CTerrainImp *ptr = ptrTerrain->tr_ptrTerrain;
	CTerrainLayer &tlLayer = ptr->tr_atlLayers[iTerrainLayer];
	CTerrainLayerImp *ptrl = tlLayer.trl_trlLayerImp;

	Matrix12 mRotate;
	Matrix12 mTranslate;
	Matrix12 mTransform;
	MakeIdentityMatrix(mRotate);
	MakeIdentityMatrix(mTranslate);

	mRotate[ 0] = CosFast(tlLayer.tl_fRotateX);
	mRotate[ 1] =-SinFast(tlLayer.tl_fRotateY);
	mRotate[ 4] = SinFast(tlLayer.tl_fRotateX);
	mRotate[ 5] = CosFast(tlLayer.tl_fRotateY);
	mTranslate[ 3] = tlLayer.tl_fOffsetX;
	mTranslate[ 7] = tlLayer.tl_fOffsetY;
	mTranslate[ 0] = 1/tlLayer.tl_fStretchX;
	mTranslate[ 5] = 1/tlLayer.tl_fStretchY;

	MatrixMultiply(mTransform,mRotate,mTranslate);
	MatrixCopy(ptrl->trl_mTransform,mTransform);

}

static INDEX GetLayerIndex(const CTerrainImp *ptr, const CTerrainLayer *ptlLayer)
{
	const INDEX cttrl = ptr->tr_atlLayers.Count();
	for(INDEX itrl=0;itrl<cttrl;itrl++) {
		const CTerrainLayer &tlLayer = ptr->tr_atlLayers[itrl];
		if(&tlLayer == ptlLayer) {
			return itrl;
		}
	}
	ASSERTALWAYS("Layer does not exists");
	return 0;
}

// yjpark |<--
static void ReadLayerVersion15_t( CTerrain *ptrTerrain, INDEX iTerrainLayer, CTStream *istrFile )
{
	ASSERT( ptrTerrain != NULL );					// must be valid
	ASSERT( ptrTerrain->tr_ptrTerrain != NULL );	// must be valid
	CTerrainImp			*ptr = ptrTerrain->tr_ptrTerrain;
	CTerrainLayer		*ptlLayer = &ptr->tr_atlLayers[iTerrainLayer];
	CTerrainLayerImp	*ptrl = ptlLayer->trl_trlLayerImp;

	// Terrain layer global data
	istrFile->ExpectID_t( "TRLG" );

	// Layer data
	INDEX iLayerType;
	(*istrFile) >> ptlLayer->tl_strName;
	(*istrFile) >> ptlLayer->tl_bVisible;
	(*istrFile) >> iLayerType;
	ptlLayer->tl_ltType = (LayerType)iLayerType;
	(*istrFile) >> ptlLayer->tl_colMultiply;
	(*istrFile) >> ptlLayer->tl_ulFlags;
	(*istrFile) >> ptlLayer->tl_slMaskStretch;
	(*istrFile) >> ptlLayer->tl_iSoundIndex;

	const PIX	pixLayerMaskWidth = ( ptr->tr_pixHeightMapWidth - 1 ) << ptlLayer->tl_slMaskStretch;
	const PIX	pixLayerMaskHeight = ( ptr->tr_pixHeightMapHeight - 1 ) << ptlLayer->tl_slMaskStretch;
	const SLONG	slLayerMaskSize = pixLayerMaskWidth * pixLayerMaskHeight * sizeof(UBYTE);

	// Layer transformation
	(*istrFile) >> ptlLayer->tl_fOffsetX;
	(*istrFile) >> ptlLayer->tl_fOffsetY;
	(*istrFile) >> ptlLayer->tl_fRotateX;
	(*istrFile) >> ptlLayer->tl_fRotateY;
	(*istrFile) >> ptlLayer->tl_fStretchX;
	(*istrFile) >> ptlLayer->tl_fStretchY;

	// Layer distribution
	(*istrFile) >> ptlLayer->tl_bAutoRegenerated;
	(*istrFile) >> ptlLayer->tl_fCoverage;
	(*istrFile) >> ptlLayer->tl_fCoverageNoise;
	(*istrFile) >> ptlLayer->tl_fCoverageRandom;
	(*istrFile) >> ptlLayer->tl_bApplyMinAltitude;
	(*istrFile) >> ptlLayer->tl_bApplyMaxAltitude;
	(*istrFile) >> ptlLayer->tl_fMinAltitude;
	(*istrFile) >> ptlLayer->tl_fMaxAltitude;
	(*istrFile) >> ptlLayer->tl_fMinAltitudeFade;
	(*istrFile) >> ptlLayer->tl_fMaxAltitudeFade;
	(*istrFile) >> ptlLayer->tl_fMinAltitudeNoise;
	(*istrFile) >> ptlLayer->tl_fMaxAltitudeNoise;
	(*istrFile) >> ptlLayer->tl_fMinAltitudeRandom;
	(*istrFile) >> ptlLayer->tl_fMaxAltitudeRandom;
	(*istrFile) >> ptlLayer->tl_bApplyMinSlope;
	(*istrFile) >> ptlLayer->tl_bApplyMaxSlope;
	(*istrFile) >> ptlLayer->tl_fMinSlope;
	(*istrFile) >> ptlLayer->tl_fMaxSlope;
	(*istrFile) >> ptlLayer->tl_fMinSlopeFade;
	(*istrFile) >> ptlLayer->tl_fMaxSlopeFade;
	(*istrFile) >> ptlLayer->tl_fMinSlopeNoise;
	(*istrFile) >> ptlLayer->tl_fMaxSlopeNoise;
	(*istrFile) >> ptlLayer->tl_fMinSlopeRandom;
	(*istrFile) >> ptlLayer->tl_fMaxSlopeRandom;

	TRL_ApplyLayerMatrix( ptrTerrain, iTerrainLayer );
	
	// Allocate memory for layer mask
	ptrl->trl_pubLayerMask = (UBYTE*)AllocMemory( slLayerMaskSize );
	UBYTE	*pubLayerMask = ptrl->trl_pubLayerMask;

	// Terrain layer implementation data
	istrFile->ExpectID_t( "TRLM" );
	istrFile->Read_t( pubLayerMask, slLayerMaskSize );

	// File name of current alpha texture
	CTFileName	fnmAlphaMask;
	(*istrFile) >> fnmAlphaMask;

	// Upload layer mask texture
	extern BOOL		_bWorldEditorApp;
	if( _bWorldEditorApp || ptrl->trl_iLayerVersion < 14 )
		TR_UploadLayerMask( ptrTerrain, iTerrainLayer, FALSE );
	else
		ptrl->trl_ptdLayerMask = _pTextureStock->Obtain_t( fnmAlphaMask );
}

static void ReadLayerVersion14_t( CTerrain *ptrTerrain, INDEX iTerrainLayer, CTStream *istrFile )
{
	ASSERT( ptrTerrain != NULL );					// must be valid
	ASSERT( ptrTerrain->tr_ptrTerrain != NULL );	// must be valid
	CTerrainImp			*ptr = ptrTerrain->tr_ptrTerrain;
	CTerrainLayer		*ptlLayer = &ptr->tr_atlLayers[iTerrainLayer];
	CTerrainLayerImp	*ptrl = ptlLayer->trl_trlLayerImp;

	// Terrain layer global data
	istrFile->ExpectID_t( "TRLG" );

	// Layer data
	INDEX iLayerType;
	(*istrFile) >> ptlLayer->tl_strName;
	(*istrFile) >> ptlLayer->tl_bVisible;
	(*istrFile) >> iLayerType;
	ptlLayer->tl_ltType = (LayerType)iLayerType;
	(*istrFile) >> ptlLayer->tl_colMultiply;
	(*istrFile) >> ptlLayer->tl_ulFlags;
	(*istrFile) >> ptlLayer->tl_slMaskStretch;

	const PIX	pixLayerMaskWidth = ( ptr->tr_pixHeightMapWidth - 1 ) << ptlLayer->tl_slMaskStretch;
	const PIX	pixLayerMaskHeight = ( ptr->tr_pixHeightMapHeight - 1 ) << ptlLayer->tl_slMaskStretch;
	const SLONG	slLayerMaskSize = pixLayerMaskWidth * pixLayerMaskHeight * sizeof(UBYTE);

	// Layer transformation
	(*istrFile) >> ptlLayer->tl_fOffsetX;
	(*istrFile) >> ptlLayer->tl_fOffsetY;
	(*istrFile) >> ptlLayer->tl_fRotateX;
	(*istrFile) >> ptlLayer->tl_fRotateY;
	(*istrFile) >> ptlLayer->tl_fStretchX;
	(*istrFile) >> ptlLayer->tl_fStretchY;

	// Layer distribution
	(*istrFile) >> ptlLayer->tl_bAutoRegenerated;
	(*istrFile) >> ptlLayer->tl_fCoverage;
	(*istrFile) >> ptlLayer->tl_fCoverageNoise;
	(*istrFile) >> ptlLayer->tl_fCoverageRandom;
	(*istrFile) >> ptlLayer->tl_bApplyMinAltitude;
	(*istrFile) >> ptlLayer->tl_bApplyMaxAltitude;
	(*istrFile) >> ptlLayer->tl_fMinAltitude;
	(*istrFile) >> ptlLayer->tl_fMaxAltitude;
	(*istrFile) >> ptlLayer->tl_fMinAltitudeFade;
	(*istrFile) >> ptlLayer->tl_fMaxAltitudeFade;
	(*istrFile) >> ptlLayer->tl_fMinAltitudeNoise;
	(*istrFile) >> ptlLayer->tl_fMaxAltitudeNoise;
	(*istrFile) >> ptlLayer->tl_fMinAltitudeRandom;
	(*istrFile) >> ptlLayer->tl_fMaxAltitudeRandom;
	(*istrFile) >> ptlLayer->tl_bApplyMinSlope;
	(*istrFile) >> ptlLayer->tl_bApplyMaxSlope;
	(*istrFile) >> ptlLayer->tl_fMinSlope;
	(*istrFile) >> ptlLayer->tl_fMaxSlope;
	(*istrFile) >> ptlLayer->tl_fMinSlopeFade;
	(*istrFile) >> ptlLayer->tl_fMaxSlopeFade;
	(*istrFile) >> ptlLayer->tl_fMinSlopeNoise;
	(*istrFile) >> ptlLayer->tl_fMaxSlopeNoise;
	(*istrFile) >> ptlLayer->tl_fMinSlopeRandom;
	(*istrFile) >> ptlLayer->tl_fMaxSlopeRandom;

	TRL_ApplyLayerMatrix( ptrTerrain, iTerrainLayer );
	
	// Allocate memory for layer mask
	ptrl->trl_pubLayerMask = (UBYTE*)AllocMemory( slLayerMaskSize );
	UBYTE	*pubLayerMask = ptrl->trl_pubLayerMask;

	// Terrain layer implementation data
	istrFile->ExpectID_t( "TRLM" );
	istrFile->Read_t( pubLayerMask, slLayerMaskSize );

	// File name of current alpha texture
	CTFileName	fnmAlphaMask;
	(*istrFile) >> fnmAlphaMask;

	// Upload layer mask texture
	extern BOOL		_bWorldEditorApp;
	if( _bWorldEditorApp || ptrl->trl_iLayerVersion < 14 )
		TR_UploadLayerMask( ptrTerrain, iTerrainLayer, FALSE );
	else
		ptrl->trl_ptdLayerMask = _pTextureStock->Obtain_t( fnmAlphaMask );
}

static void ReadLayerVersion13_t( CTerrain *ptrTerrain, INDEX iTerrainLayer, CTStream *istrFile )
{
	ASSERT( ptrTerrain != NULL );					// must be valid
	ASSERT( ptrTerrain->tr_ptrTerrain != NULL );	// must be valid
	CTerrainImp			*ptr = ptrTerrain->tr_ptrTerrain;
	CTerrainLayer		*ptlLayer = &ptr->tr_atlLayers[iTerrainLayer];
	CTerrainLayerImp	*ptrl = ptlLayer->trl_trlLayerImp;

	// Terrain layer global data
	istrFile->ExpectID_t( "TRLG" );

	// Layer data
	INDEX iLayerType;
	(*istrFile) >> ptlLayer->tl_strName;
	(*istrFile) >> ptlLayer->tl_bVisible;
	(*istrFile) >> iLayerType;
	ptlLayer->tl_ltType = (LayerType)iLayerType;
	(*istrFile) >> ptlLayer->tl_colMultiply;
	(*istrFile) >> ptlLayer->tl_ulFlags;
	(*istrFile) >> ptlLayer->tl_slMaskStretch;

	const PIX	pixLayerMaskWidth = ( ptr->tr_pixHeightMapWidth - 1 ) << ptlLayer->tl_slMaskStretch;
	const PIX	pixLayerMaskHeight = ( ptr->tr_pixHeightMapHeight - 1 ) << ptlLayer->tl_slMaskStretch;
	const PIX	pixLayerMaskSize = pixLayerMaskWidth * pixLayerMaskHeight;
	const SLONG	slLayerMaskSize = pixLayerMaskSize * sizeof(UBYTE);

	// Layer transformation
	(*istrFile) >> ptlLayer->tl_fOffsetX;
	(*istrFile) >> ptlLayer->tl_fOffsetY;
	(*istrFile) >> ptlLayer->tl_fRotateX;
	(*istrFile) >> ptlLayer->tl_fRotateY;
	(*istrFile) >> ptlLayer->tl_fStretchX;
	(*istrFile) >> ptlLayer->tl_fStretchY;

	// Layer distribution
	(*istrFile) >> ptlLayer->tl_bAutoRegenerated;
	(*istrFile) >> ptlLayer->tl_fCoverage;
	(*istrFile) >> ptlLayer->tl_fCoverageNoise;
	(*istrFile) >> ptlLayer->tl_fCoverageRandom;
	(*istrFile) >> ptlLayer->tl_bApplyMinAltitude;
	(*istrFile) >> ptlLayer->tl_bApplyMaxAltitude;
	(*istrFile) >> ptlLayer->tl_fMinAltitude;
	(*istrFile) >> ptlLayer->tl_fMaxAltitude;
	(*istrFile) >> ptlLayer->tl_fMinAltitudeFade;
	(*istrFile) >> ptlLayer->tl_fMaxAltitudeFade;
	(*istrFile) >> ptlLayer->tl_fMinAltitudeNoise;
	(*istrFile) >> ptlLayer->tl_fMaxAltitudeNoise;
	(*istrFile) >> ptlLayer->tl_fMinAltitudeRandom;
	(*istrFile) >> ptlLayer->tl_fMaxAltitudeRandom;
	(*istrFile) >> ptlLayer->tl_bApplyMinSlope;
	(*istrFile) >> ptlLayer->tl_bApplyMaxSlope;
	(*istrFile) >> ptlLayer->tl_fMinSlope;
	(*istrFile) >> ptlLayer->tl_fMaxSlope;
	(*istrFile) >> ptlLayer->tl_fMinSlopeFade;
	(*istrFile) >> ptlLayer->tl_fMaxSlopeFade;
	(*istrFile) >> ptlLayer->tl_fMinSlopeNoise;
	(*istrFile) >> ptlLayer->tl_fMaxSlopeNoise;
	(*istrFile) >> ptlLayer->tl_fMinSlopeRandom;
	(*istrFile) >> ptlLayer->tl_fMaxSlopeRandom;

	TRL_ApplyLayerMatrix( ptrTerrain, iTerrainLayer );

	// Allocate memory for layer mask
	ptrl->trl_pubLayerMask = (UBYTE*)AllocMemory( slLayerMaskSize );
	UBYTE	*pubLayerMask = ptrl->trl_pubLayerMask;

	// Terrain layer implementation data
	istrFile->ExpectID_t( "TRLM" );
	istrFile->Read_t( pubLayerMask, slLayerMaskSize );

	// Upload layer mask texture
	TR_UploadLayerMask( ptrTerrain, iTerrainLayer );
}
// yjpark     -->|

static void ReadLayerVersion12_t(CTerrain *ptrTerrain, INDEX iTerrainLayer, CTStream *istrFile)
{
	ASSERT(ptrTerrain!=NULL);                // must be valid
	ASSERT(ptrTerrain->tr_ptrTerrain!=NULL); // must be valid
	CTerrainImp *ptr = ptrTerrain->tr_ptrTerrain;
	CTerrainLayer *ptlLayer = &ptr->tr_atlLayers[iTerrainLayer];
	CTerrainLayerImp *ptrl  = ptlLayer->trl_trlLayerImp;
	const PIX pixLayerMaskWidth   = ptr->tr_pixHeightMapWidth - 1;
	const PIX pixLayerMaskHeight  = ptr->tr_pixHeightMapHeight - 1;
	const PIX pixLayerMaskSize    = pixLayerMaskWidth*pixLayerMaskHeight;
	const SLONG slLayerMaskSize   = pixLayerMaskSize*sizeof(UBYTE);

	// Terrain layer global data
	istrFile->ExpectID_t("TRLG");

	// Layer data
	INDEX iLayerType;
	(*istrFile)>>ptlLayer->tl_strName;
	(*istrFile)>>ptlLayer->tl_bVisible;
	(*istrFile)>>iLayerType;
	ptlLayer->tl_ltType = (LayerType)iLayerType;
	(*istrFile)>>ptlLayer->tl_colMultiply;
	(*istrFile)>>ptlLayer->tl_ulFlags;

	// Layer transformation
	(*istrFile)>>ptlLayer->tl_fOffsetX;
	(*istrFile)>>ptlLayer->tl_fOffsetY;
	(*istrFile)>>ptlLayer->tl_fRotateX;
	(*istrFile)>>ptlLayer->tl_fRotateY;
	(*istrFile)>>ptlLayer->tl_fStretchX;
	(*istrFile)>>ptlLayer->tl_fStretchY;

	// Layer distribution
	(*istrFile)>>ptlLayer->tl_bAutoRegenerated;
	(*istrFile)>>ptlLayer->tl_fCoverage;
	(*istrFile)>>ptlLayer->tl_fCoverageNoise;
	(*istrFile)>>ptlLayer->tl_fCoverageRandom;
	(*istrFile)>>ptlLayer->tl_bApplyMinAltitude;
	(*istrFile)>>ptlLayer->tl_bApplyMaxAltitude;
	(*istrFile)>>ptlLayer->tl_fMinAltitude;
	(*istrFile)>>ptlLayer->tl_fMaxAltitude;
	(*istrFile)>>ptlLayer->tl_fMinAltitudeFade;
	(*istrFile)>>ptlLayer->tl_fMaxAltitudeFade;
	(*istrFile)>>ptlLayer->tl_fMinAltitudeNoise;
	(*istrFile)>>ptlLayer->tl_fMaxAltitudeNoise;
	(*istrFile)>>ptlLayer->tl_fMinAltitudeRandom;
	(*istrFile)>>ptlLayer->tl_fMaxAltitudeRandom;
	(*istrFile)>>ptlLayer->tl_bApplyMinSlope;
	(*istrFile)>>ptlLayer->tl_bApplyMaxSlope;
	(*istrFile)>>ptlLayer->tl_fMinSlope;
	(*istrFile)>>ptlLayer->tl_fMaxSlope;
	(*istrFile)>>ptlLayer->tl_fMinSlopeFade;
	(*istrFile)>>ptlLayer->tl_fMaxSlopeFade;
	(*istrFile)>>ptlLayer->tl_fMinSlopeNoise;
	(*istrFile)>>ptlLayer->tl_fMaxSlopeNoise;
	(*istrFile)>>ptlLayer->tl_fMinSlopeRandom;
	(*istrFile)>>ptlLayer->tl_fMaxSlopeRandom;

	TRL_ApplyLayerMatrix(ptrTerrain, iTerrainLayer);

	// Allocate memory for layer mask									// yjpark |<--
	ptrl->trl_pubLayerMask = (UBYTE*)AllocMemory( slLayerMaskSize );
	UBYTE *pubLayerMask = ptrl->trl_pubLayerMask;						// yjpark     -->|

	// Terrain layer implementation data
	istrFile->ExpectID_t("TRLM");
	istrFile->Read_t(pubLayerMask,slLayerMaskSize);

	// Upload layer mask texture
	TR_UploadLayerMask(ptrTerrain, iTerrainLayer);
}

// Read/Write layer
extern void TRL_ReadLayerData_t(CTerrain *ptrTerrain, INDEX iTerrainLayer, CTStream *istrFile)
{
	ASSERT(ptrTerrain!=NULL);                // must be valid
	ASSERT(ptrTerrain->tr_ptrTerrain!=NULL); // must be valid
	CTerrainImp			*ptr = ptrTerrain->tr_ptrTerrain;
	CTerrainLayer		*ptlLayer = &ptr->tr_atlLayers[iTerrainLayer];
	CTerrainLayerImp	*ptrl = ptlLayer->trl_trlLayerImp;

	// Terrain layer version
	istrFile->ExpectID_t("TRLV");
	(*istrFile) >> ptrl->trl_iLayerVersion;

	if( ptrl->trl_iLayerVersion == 15 )									// yjpark |<--
	{
		ReadLayerVersion15_t( ptrTerrain, iTerrainLayer, istrFile );
	}
	else if( ptrl->trl_iLayerVersion == 14 )
	{
		ReadLayerVersion14_t( ptrTerrain, iTerrainLayer, istrFile );
	}
	else if( ptrl->trl_iLayerVersion == 13 )
	{
		ReadLayerVersion13_t( ptrTerrain, iTerrainLayer, istrFile );
	}																	// yjpark     -->|
	else if( ptrl->trl_iLayerVersion == 12 )
	{
		ReadLayerVersion12_t( ptrTerrain, iTerrainLayer, istrFile );
	}
	else
	{
		ThrowF_t( TRANS("The terrain layer version on disk is %d.\n"
						"Current supported version is %d."), ptrl->trl_iLayerVersion, TRL_FNM_VERSION);
	}
}

extern void TRL_WriteLayerData_t(CTerrain *ptrTerrain, INDEX iTerrainLayer, CTStream *ostrFile)
{
	ASSERT(ptrTerrain!=NULL);                // must be valid
	ASSERT(ptrTerrain->tr_ptrTerrain!=NULL); // must be valid
	const CTerrainImp *ptr = ptrTerrain->tr_ptrTerrain;
	const CTerrainLayer *ptlLayer = &ptr->tr_atlLayers[iTerrainLayer];
	const CTerrainLayerImp *ptrl  = ptlLayer->trl_trlLayerImp;
	const PIX pixLayerMaskWidth   = ( ptr->tr_pixHeightMapWidth - 1 ) << ptlLayer->tl_slMaskStretch;		// yjpark
	const PIX pixLayerMaskHeight  = ( ptr->tr_pixHeightMapHeight - 1 ) << ptlLayer->tl_slMaskStretch;		// yjpark
	const UBYTE *pubLayerMask     = ptrl->trl_pubLayerMask;
	const SLONG slLayerMaskSize   = pixLayerMaskWidth*pixLayerMaskHeight*sizeof(UBYTE);

	// Terrain layer version
	ostrFile->WriteID_t("TRLV");
	(*ostrFile)<<TRL_FNM_VERSION;

	// Terrain layer global data
	ostrFile->WriteID_t("TRLG");

	// Layer data
	(*ostrFile)<<ptlLayer->tl_strName;
	(*ostrFile)<<ptlLayer->tl_bVisible;
	(*ostrFile)<<ptlLayer->tl_ltType;
	(*ostrFile)<<ptlLayer->tl_colMultiply;
	(*ostrFile)<<ptlLayer->tl_ulFlags;
	(*ostrFile)<<ptlLayer->tl_slMaskStretch;			// yjpark
	(*ostrFile)<<ptlLayer->tl_iSoundIndex;				// yjpark

	// Layer transformation
	(*ostrFile)<<ptlLayer->tl_fOffsetX;
	(*ostrFile)<<ptlLayer->tl_fOffsetY;
	(*ostrFile)<<ptlLayer->tl_fRotateX;
	(*ostrFile)<<ptlLayer->tl_fRotateY;
	(*ostrFile)<<ptlLayer->tl_fStretchX;
	(*ostrFile)<<ptlLayer->tl_fStretchY;

	// Layer distribution
	(*ostrFile)<<ptlLayer->tl_bAutoRegenerated;
	(*ostrFile)<<ptlLayer->tl_fCoverage;
	(*ostrFile)<<ptlLayer->tl_fCoverageNoise;
	(*ostrFile)<<ptlLayer->tl_fCoverageRandom;
	(*ostrFile)<<ptlLayer->tl_bApplyMinAltitude;
	(*ostrFile)<<ptlLayer->tl_bApplyMaxAltitude;
	(*ostrFile)<<ptlLayer->tl_fMinAltitude;
	(*ostrFile)<<ptlLayer->tl_fMaxAltitude;
	(*ostrFile)<<ptlLayer->tl_fMinAltitudeFade;
	(*ostrFile)<<ptlLayer->tl_fMaxAltitudeFade;
	(*ostrFile)<<ptlLayer->tl_fMinAltitudeNoise;
	(*ostrFile)<<ptlLayer->tl_fMaxAltitudeNoise;
	(*ostrFile)<<ptlLayer->tl_fMinAltitudeRandom;
	(*ostrFile)<<ptlLayer->tl_fMaxAltitudeRandom;
	(*ostrFile)<<ptlLayer->tl_bApplyMinSlope;
	(*ostrFile)<<ptlLayer->tl_bApplyMaxSlope;
	(*ostrFile)<<ptlLayer->tl_fMinSlope;
	(*ostrFile)<<ptlLayer->tl_fMaxSlope;
	(*ostrFile)<<ptlLayer->tl_fMinSlopeFade;
	(*ostrFile)<<ptlLayer->tl_fMaxSlopeFade;
	(*ostrFile)<<ptlLayer->tl_fMinSlopeNoise;
	(*ostrFile)<<ptlLayer->tl_fMaxSlopeNoise;
	(*ostrFile)<<ptlLayer->tl_fMinSlopeRandom;
	(*ostrFile)<<ptlLayer->tl_fMaxSlopeRandom;

	// Terrain layer implementation data		// yjpark |<--
	ostrFile->WriteID_t("TRLM");
	ostrFile->Write_t(pubLayerMask,slLayerMaskSize);

	// File name of current alpha texture
	CTFileName	fnmFullPath = ostrFile->strm_strStreamDescription;
	CTFileName	fnmDir = fnmFullPath.FileDir();
	CTFileName	fnmName = fnmFullPath.FileName();
	CTString	strAlphaMask;
	strAlphaMask.PrintF( "_a%d.tex", iTerrainLayer );
	CTFileName	fnmAlphaMask = fnmDir + CTString( "Texture\\" ) + fnmName + strAlphaMask;
	(*ostrFile) << fnmAlphaMask;

	// Current alpha texture
	ULONG	ulOldFlags = ptrl->trl_ptdLayerMask->td_ulFlags;
	ptrl->trl_ptdLayerMask->td_ulFlags = TEX_COMPRESS | TEX_ALPHACHANNEL;
	ptrl->trl_ptdLayerMask->Save_t( fnmAlphaMask );
	ptrl->trl_ptdLayerMask->td_ulFlags = ulOldFlags;
}

// yjpark |<--
//-------------------------------------------------------------------------------------------------
// Desc : These contents aren't included in final release version
// ------------------------------------------------------------------------------------------------
#ifndef FINALVERSION

// Set layer position
extern INDEX TRL_SetLayerIndex(CTerrain *ptrTerrain, INDEX iTerrainLayer, INDEX iNewLayerIndex)
{
	ASSERT(ptrTerrain!=NULL);                // must be valid
	ASSERT(ptrTerrain->tr_ptrTerrain!=NULL); // must be valid

	CTerrainImp *ptr = ptrTerrain->tr_ptrTerrain;

	const INDEX ctLayers = ptr->tr_atlLayers.Count();

	if(iTerrainLayer<0 || iTerrainLayer>=ctLayers) {
		ASSERTALWAYS("Invalid layer index");
		return iTerrainLayer;
	}

	if(iTerrainLayer==0 && ctLayers==1) {
		ASSERTALWAYS("Can't move only layer");
		return iTerrainLayer;
	}

	if(iTerrainLayer==iNewLayerIndex) {
		ASSERTALWAYS("Old layer index is same as new one");
		return iTerrainLayer;
	}


	// Hack copy of static stack array
	CStaticArray<CTerrainLayer> atlOldLayers;
	atlOldLayers.sa_Array = ptr->tr_atlLayers.sa_Array;
	atlOldLayers.sa_Count = ptr->tr_atlLayers.sa_Count;
	ptr->tr_atlLayers.sa_Array = NULL;
	ptr->tr_atlLayers.sa_Count = 0;
	ptr->tr_atlLayers.New(ctLayers);

	CStaticArray<class CTerrainLayer> &atlFrom = atlOldLayers;
	CStaticArray<class CTerrainLayer> &atlTo = ptr->tr_atlLayers;

	INDEX iOld = iTerrainLayer;
	INDEX iNew = iNewLayerIndex;

	for(INDEX iFrom=0;iFrom<ctLayers;iFrom++) {
		INDEX iTo=-1;
		if (iNew==iOld) {
			iTo = iFrom;
		} if ((iFrom<iOld && iFrom<iNew) || (iFrom>iOld && iFrom>iNew)) {
			iTo = iFrom;
		} else if (iFrom==iOld) {
			iTo = iNew;
		} else {
			if (iNew>iOld) {
				iTo = iFrom-1;
			} else {
				iTo = iFrom+1;
			}
		}
		const CTerrainLayer &tlSrc = atlFrom[iFrom];
		TRL_CopyLayer(ptrTerrain, iTo, tlSrc);
	}

	// Clear old layers
	atlOldLayers.Clear();

	extern void TR_GenerateTerrainTileOptimizationData(CTerrain *ptrTerrain);
	TR_GenerateTerrainTileOptimizationData(ptrTerrain);

	return iNewLayerIndex;
}

// Get current layer texture
extern CTextureData *TRL_GetLayerTexture_t(CTerrain *ptrTerrain, INDEX iTerrainLayer)
{
	ASSERT(ptrTerrain!=NULL);                // must be valid
	ASSERT(ptrTerrain->tr_ptrTerrain!=NULL); // must be valid

	CTerrainImp *ptr = ptrTerrain->tr_ptrTerrain;
	CTerrainLayer &tlLayer = ptr->tr_atlLayers[iTerrainLayer];
	CTerrainLayerImp *ptrl = tlLayer.trl_trlLayerImp;
	return ptrl->trl_ptdTexture;
}

// Import layer mask
extern void TRL_ImportLayerMask_t(CTerrain *ptrTerrain, CTerrainLayer *ptlLayer, const CTFileName &fnm)
{
	ASSERT(ptrTerrain!=NULL);                // must be valid
	ASSERT(ptrTerrain->tr_ptrTerrain!=NULL); // must be valid
	CTerrainImp *ptr = ptrTerrain->tr_ptrTerrain;
	CTerrainLayerImp *ptrl = ptlLayer->trl_trlLayerImp;

	ASSERT(ptr->tr_puwHeightMap!=NULL);
	ASSERT(ptrl->trl_pubLayerMask!=NULL);

	// Load layer mask map
	CImageInfo iiImage;
	iiImage.LoadAnyGfxFormat_t(fnm);

	const PIX pixMapWidth = iiImage.ii_Width;
	const PIX pixMapHeight = iiImage.ii_Height;
	const SLONG slLayerMaskSize = pixMapWidth*pixMapHeight;
	const INDEX iBpp = iiImage.ii_BitsPerPixel/8;

	if( pixMapWidth != ( ptr->tr_pixHeightMapWidth - 1 ) << ptlLayer->tl_slMaskStretch ) {			// yjpark |<--
		ThrowF_t( "Layer mask width must match ( terrain height map width - 1 ) * stretch of layer mask" );
	}
	if( pixMapHeight != ( ptr->tr_pixHeightMapHeight - 1 ) << ptlLayer->tl_slMaskStretch ) {
		ThrowF_t( "Layer mask height must match ( terrain height map height -1 ) * stretch of layer mask" );
	}																								// yjpark     -->|

	// Read height map from file
	const UBYTE *pubSrc = &iiImage.ii_Picture[0];
	UBYTE *pubDst = &ptrl->trl_pubLayerMask[0];

	// Create layer mask that will be in local memory
	for(PIX pix=0;pix<slLayerMaskSize;pix++) {
		*pubDst = *(UBYTE*)pubSrc;
		pubDst++;
		pubSrc+=iBpp;
	}

	INDEX iTerrainLayer = GetLayerIndex(ptr, ptlLayer);	
	TR_UploadLayerMask(ptrTerrain, iTerrainLayer);
}

extern void TRL_ImportLayerMask_t(CTerrain *ptrTerrain, INDEX iTerrainLayer, const CTFileName &fnm)
{
	ASSERT(ptrTerrain!=NULL);                // must be valid
	ASSERT(ptrTerrain->tr_ptrTerrain!=NULL); // must be valid
	CTerrainImp *ptr = ptrTerrain->tr_ptrTerrain;
	CTerrainLayer *ptlLayer = &ptr->tr_atlLayers[iTerrainLayer];
	TRL_ImportLayerMask_t(ptrTerrain,ptlLayer,fnm);
}

// Export layer mask
extern void TRL_ExportLayerMask_t(CTerrain *ptrTerrain, CTerrainLayer *ptlLayer, const CTFileName &fnm)
{
	ASSERT( ptrTerrain != NULL );
	ASSERT( ptrTerrain->tr_ptrTerrain != NULL );
	CTerrainImp*		ptr = ptrTerrain->tr_ptrTerrain;
	CTerrainLayerImp*	ptrl = ptlLayer->trl_trlLayerImp;

	ASSERT( ptrl->trl_pubLayerMask != NULL );

	// Get width & height of layer mask
	const PIX	pixLayerMaskWidth = ( ptr->tr_pixHeightMapWidth - 1 ) << ptlLayer->tl_slMaskStretch;
	const PIX	pixLayerMaskHeight = ( ptr->tr_pixHeightMapHeight - 1 ) << ptlLayer->tl_slMaskStretch;
	const PIX	pixLayerMaskSize = pixLayerMaskWidth * pixLayerMaskHeight;

	// Prepare for saving layer mask map
	CImageInfo	iiImage;
	iiImage.ii_BitsPerPixel = 32;
	iiImage.ii_Width = pixLayerMaskWidth;
	iiImage.ii_Height = pixLayerMaskHeight;
	iiImage.ii_Picture = (UBYTE*)AllocMemory( pixLayerMaskSize * sizeof(ULONG) );
	UBYTE*	pDst = iiImage.ii_Picture;
	UBYTE*	pSrc = ptrl->trl_pubLayerMask;

	for( PIX pix = 0; pix < pixLayerMaskSize; pix++ )
	{
		*pDst = *pSrc;
		*(pDst + 1 ) = 255;
		*(pDst + 2 ) = 255;
		*(pDst + 3 ) = 255;
		pDst += 4;
		pSrc++;
	}

	// Save layer mask map
	iiImage.SaveTGA_t( fnm );
}

extern void TRL_ExportLayerMask_t(CTerrain *ptrTerrain, INDEX iTerrainLayer, const CTFileName &fnmTexture)
{
	ASSERT( ptrTerrain != NULL );
	ASSERT( ptrTerrain->tr_ptrTerrain != NULL );
	CTerrainImp *ptr = ptrTerrain->tr_ptrTerrain;
	CTerrainLayer*	ptlLayer = &ptr->tr_atlLayers[iTerrainLayer];
	TRL_ExportLayerMask_t( ptrTerrain, ptlLayer, fnmTexture );
}

// Get layer thumbnail
extern CTextureData *TRL_GetLayerThumbnail(CTerrain *ptrTerrain, INDEX iTerrainLayer, PIX pixWidth, PIX pixHeight)
{
	ASSERT(ptrTerrain!=NULL);                // must be valid
	ASSERT(ptrTerrain->tr_ptrTerrain!=NULL); // must be valid
	CTerrainImp *ptr = ptrTerrain->tr_ptrTerrain;
	CTerrainLayer *ptlLayer = &ptr->tr_atlLayers[iTerrainLayer];
	CTerrainLayerImp *ptrl  = ptlLayer->trl_trlLayerImp;

	const PIX pixLayerMaskWidth  = ( ptr->tr_pixHeightMapWidth - 1 ) << ptlLayer->tl_slMaskStretch;			// yjpark
	const PIX pixLayerMaskHeight = ( ptr->tr_pixHeightMapHeight - 1 ) << ptlLayer->tl_slMaskStretch;		// yjpark
	const PIX pixLayerMaskSize   = pixLayerMaskWidth*pixLayerMaskHeight;

	TR_PrepareTexture(*ptlLayer->trl_ptdThumbnail, pixLayerMaskWidth, pixLayerMaskHeight,0);
	const UBYTE *pubSrc = ptrl->trl_pubLayerMask;
	ULONG *pulDst = ptlLayer->trl_ptdThumbnail->td_pulFrames;

	for(INDEX ipix=0;ipix<pixLayerMaskSize;ipix++) {
		pulDst[ipix] = 0x00FFFFFF;
		pulDst[ipix] |= ((ULONG)pubSrc[ipix])<<24;
	}

	TR_UploadTexture(*ptlLayer->trl_ptdThumbnail, pixLayerMaskWidth, pixLayerMaskHeight,0);
	return ptlLayer->trl_ptdThumbnail;
}

#endif	// FINALVERSION
// yjpark     -->|
