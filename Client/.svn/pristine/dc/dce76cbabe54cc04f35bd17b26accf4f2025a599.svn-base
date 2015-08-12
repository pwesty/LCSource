#include "stdh.h"

#include <Engine/Base/Stream.h>
#include <Engine/Base/Console.h>
#include <Engine/Math/Float.h>
#include <Engine/World/World.h>
#include <Engine/World/WorldEditingProfile.h>
#include <Engine/World/WorldSettings.h>
#include <Engine/Entities/EntityClass.h>
#include <Engine/Entities/Precaching.h>
#include <Engine/Ska/StringTable.h>
#include <Engine/Templates/StaticArray.cpp>
#include <Engine/Templates/DynamicContainer.cpp>
#include <Engine/Brushes/BrushArchive.h>
#include <Engine/Terrain/TerrainArchive.h>
#include <Engine/Network/CNetwork.h>
#include <Engine/Network/Server.h>
#include <Engine/Network/SessionState.h>
#include <Engine/Templates/StaticArray.cpp>
#include <Engine/Terrain/Terrain.h>
#include <Engine/World/WorldEntityHashing.h>
#include <Engine/Entities/InternalClasses.h>
#include <Engine/Entities/EntityClass.h>

#include <Engine/Ska/ModelInstance.h>
#include <Engine/Ska/Mesh.h>
#include <Engine/GameState.h>
#include <Engine/GlobalDefinition.h>
#include <Engine/LocalDefine.h>
//안태훈 수정 시작	//(Quest System)(0.1)
#include <Engine/Entities/QuestSystem.h>
#include <Engine/Effect/EffectCommon.h>
//안태훈 수정 끝	//(Quest System)(0.1)
#include <Engine/Loading.h>

#define WORLDSTATEVERSION_NOCLASSCONTAINER 9
#define WORLDSTATEVERSION_MULTITEXTURING 8
#define WORLDSTATEVERSION_SHADOWSPERMIP 7
#define WORLDSTATEVERSION_CURRENT WORLDSTATEVERSION_NOCLASSCONTAINER
extern CWorld *_pwoCurrentLoading = NULL;  // world that is currently loading
extern BOOL _bPortalSectorLinksPreLoaded;
extern BOOL _bEntitySectorLinksPreLoaded;
extern BOOL _bFileReplacingApplied;
extern BOOL _bReadEntitiesByID = FALSE;

extern BOOL g_bIsMalEng ;

static BOOL _bLoadingEffectData = FALSE;

#define PRELOAD_BY_EXTENSIONS 0

#if PRELOAD_BY_EXTENSIONS

#include <Engine/Templates/Stock_CTextureData.h>
#include <Engine/Templates/Stock_CModelData.h>
#include <Engine/Templates/Stock_CSoundData.h>
#include <Engine/Templates/Stock_CMesh.h>
#include <Engine/Templates/Stock_CSkeleton.h>
#include <Engine/Templates/Stock_CAnimSet.h>
#include <Engine/Templates/Stock_CEntityClass.h>
#include <Engine/Templates/Stock_CShader.h>
#include <Engine/Templates/Stock_CAnimData.h>

static int qsortCompareCTFileName(const void *pv0, const void *pv1)
{
	return stricmp(**(const CTFileName**)pv0, **(const CTFileName**)pv1);
}
static void DictionaryPreload_t(CTStream *istr, const CTString &strPreloadExt)
{
	INDEX ctFileNames = istr->strm_afnmDictionary.Count();

	CDynamicContainer<CTFileName> cfnmFiles;
	// add all filenames
	for(INDEX iFileName=0; iFileName<ctFileNames; iFileName++) {
		cfnmFiles.Add(&istr->strm_afnmDictionary[iFileName]);
	}
	// sort them
	if (ctFileNames>0) {
		qsort(cfnmFiles.sa_Array, ctFileNames, sizeof(CTFileName*), qsortCompareCTFileName);
	}

	//if(g_iCountry == RUSSIA)
	#if defined G_RUSSIA
		SetProgressDescription("행藎滄魏:" + strPreloadExt);
	//else
	#else
		SetProgressDescription("Loading:" + strPreloadExt);
	#endif
	// for each filename
	{for(INDEX iFileName=0; iFileName<ctFileNames; iFileName++) {
		// preload it
		CTFileName &fnm = cfnmFiles[iFileName];
		CTString strExt = fnm.FileExt();
		CallProgressHook_t(FLOAT(iFileName)/ctFileNames);
		try {
			if(TRUE) {//strPreloadExt == strExt || ) {
				if(strExt==".tex") {
					fnm.fnm_pserPreloaded = _pTextureStock->Obtain_t(fnm);
				} else if (strExt==".mdl") {
					fnm.fnm_pserPreloaded = _pModelStock->Obtain_t(fnm);
				} else if (strExt==".wav") {
					fnm.fnm_pserPreloaded = _pSoundStock->Obtain_t(fnm);
				} else if (strExt==".bm") {
					fnm.fnm_pserPreloaded = _pMeshStock->Obtain_t(fnm);
				} else if (strExt==".bs") {
					fnm.fnm_pserPreloaded = _pSkeletonStock->Obtain_t(fnm);
				} else if (strExt==".ba") {
					fnm.fnm_pserPreloaded = _pAnimSetStock->Obtain_t(fnm);
				} else if (strExt==".ecl") {
					fnm.fnm_pserPreloaded = _pEntityClassStock->Obtain_t(fnm);
				} else if (strExt==".sha") {
					fnm.fnm_pserPreloaded = _pShaderStock->Obtain_t(fnm);
				} else if (strExt==".ani") {
					fnm.fnm_pserPreloaded = _pAnimStock->Obtain_t(fnm);
				}
			}
		} catch (char *strError) {
			CPrintF( TRANS("Cannot preload %s: %s\n"), (CTString&)fnm, strError);
		}
	}}
}
#endif

/*
 * Save entire world (both brushes  current state).
 */
void CWorld::Save_t(const CTFileName &fnmWorld) // throw char *
{
	// create the file
	CTFileStream strmFile;
	strmFile.Create_t(fnmWorld);

	// save engine build
	_pNetwork->WriteVersion_t(strmFile);

	// write the world to the file
	Write_t(&strmFile);
}

/*
 * wld 파일을 구성 요소를 분리해서 저장하는 루틴
 * written by seo
 */
void CWorld::Save_t_ext(const CTFileName &fnmWorld)
{
	// 파일 생성
	CTFileName fnmWorldList, fnmBrush, fnmState;
	CTFileStream strmFileWls, strmFileWbr, strmFileSta;	

	fnmWorldList = fnmWorld.NoExt() + ".wls";
	fnmBrush = fnmWorld.NoExt() + ".wbr";
	fnmState = fnmWorld.NoExt() + ".wst";

	strmFileWls.Create_t(fnmWorldList);
	strmFileWbr.Create_t(fnmBrush);
	strmFileSta.Create_t(fnmState);

	// save engine build
	_pNetwork->WriteVersion_t(strmFileWbr);
	//_pNetwork->WriteVersion_t(strmFileSta);			// state stream의 버전 정보를 넣는 것은 추후 고려.

	Write_t_ext(&strmFileWbr, &strmFileSta);
}

/*
 * Load entire world (both brushes and current state).
 */
void CWorld::Load_t(const CTFileName &fnmWorld) // throw char *
{
	// remember the file
	wo_fnmFileName = fnmWorld;
	// open the file
	CTFileStream strmFile;
	strmFile.Open_t(fnmWorld);

	// check engine build allowing reinit
	BOOL bNeedsReinit;
	_pNetwork->CheckVersion_t(strmFile, TRUE, bNeedsReinit);

	// read the world from the file
	Read_t(&strmFile);

	// close the file
	strmFile.Close();

//안태훈 수정 시작	//(For Performance)(0.2)
	//Light Source를 갖고 있는 녀석들만 Reinitialize한다.
	//ReinitializeEntities();	//임시.
	for(INDEX i=0; i<wo_cenEntities.Count(); ++i)
	{
		if(wo_cenEntities.Pointer(i+ 0)->GetLightSource() != NULL)
		{
			wo_cenEntities.Pointer(i+ 0)->Reinitialize();
		}
	}
//안태훈 수정 끝	//(For Performance)(0.2)

	// if reinit is needed
	if (bNeedsReinit) {
		// reinitialize
		CallProgressHook_t(0.0f);
		ReinitializeEntities();
		CallProgressHook_t(1.0f);
		// reinitialize
		CallProgressHook_t(0.0f);
		Save_t(fnmWorld);
		CallProgressHook_t(1.0f);
	}
}

/*
 * 분리된 wld 파일을 나누어서 읽는 부분.
 */
void CWorld::Load_t_ext(const CTFileName &fnmWorldWls) // throw char *
{
	// remember the file
	wo_fnmFileName = fnmWorldWls;

	CTFileName fnmBrush, fnmState;
	// open the file  
	CTFileStream strmFileWbr, strmFileSta;	

	fnmBrush = fnmWorldWls.NoExt() + ".wbr";
	fnmState = fnmWorldWls.NoExt() + ".wst";

	strmFileWbr.Open_t(fnmBrush);
	strmFileSta.Open_t(fnmState);	

	// check engine build allowing reinit
	BOOL bNeedsReinit;
	_pNetwork->CheckVersion_t(strmFileWbr, TRUE, bNeedsReinit);

	// read the world from the file
	Read_t_ext(&strmFileWbr, &strmFileSta);

	// close the file
	strmFileWbr.Close();
	strmFileSta.Close();

	// if reinit is needed
	// 아마도 bNeedsReinit가 호출될 가능성은 없을 듯. by seo.
	if (bNeedsReinit) {
		// reinitialize
		CallProgressHook_t(0.0f);
		ReinitializeEntities();
		CallProgressHook_t(1.0f);
		// reinitialize
		CallProgressHook_t(0.0f);
		Save_t_ext(fnmWorldWls);				// modified by seo
		CallProgressHook_t(1.0f);
	}
}

/*
 * Write entire world (both brushes and current state).
 */
void CWorld::Write_t(CTStream *postrm) // throw char *
{
	// need high FPU precision
	CSetFPUPrecision FPUPrecision(FPT_53BIT);

	// lock all arrays and containers
	LockAll();

	postrm->WriteID_t("WRLD"); // 'world'
	// write the world brushes to the file
	WriteBrushes_t(postrm);
	// write current world state to the file
	WriteState_t(postrm);
	postrm->WriteID_t("WEND"); // 'world end'

	// unlock all arrays and containers
	UnlockAll();
}

/*
 * Brush 와 state를 각기 다른 파일로 쓰기.
 */
void CWorld::Write_t_ext(CTStream *postrmBrsh, CTStream *postrmStat)
{
	CSetFPUPrecision FPUPrecision(FPT_53BIT);

	// lock all arrays and containers
		LockAll();

	postrmBrsh->WriteID_t("WRLD");	// 
	// brush의 내용을 화일에 쓰기.
	WriteBrushes_t(postrmBrsh);

	// 월드 상태 정보를 화일에 쓰기
	WriteState_t(postrmStat);
	postrmStat->WriteID_t("WEND");

	// unlock all arrays and containers
	UnlockAll();
}



/*
 * Read entire world (both brushes and current state).
 */
void CWorld::Read_t(CTStream *pistrm) // throw char *
{
	_pfWorldEditingProfile.IncrementAveragingCounter();
	_bFileReplacingApplied = FALSE;

	// need high FPU precision
	CSetFPUPrecision FPUPrecision(FPT_53BIT);

	// clear eventual old data in the world
	Clear();

	// lock all arrays and containers
	LockAll();

	pistrm->ExpectID_t("WRLD"); // 'world'
	// read the world brushes from the file
	ReadBrushes_t(pistrm);
	// read current world state from the file
	ReadState_t(pistrm);
	pistrm->ExpectID_t("WEND"); // 'world end'

	// unlock all arrays and containers
	UnlockAll();

	if( _bFileReplacingApplied)
		WarningMessage("Some of files needed to load world have been replaced while loading");
}

/*
 * 월드 파일을 브러쉬와 스테이트 화일을 나누어 읽는다. by seo
 */
void CWorld::Read_t_ext(CTStream *postrmBrsh, CTStream *postrmStat) // throw char *
{
	_pfWorldEditingProfile.IncrementAveragingCounter();
	_bFileReplacingApplied = FALSE;

	// need high FPU precision
	CSetFPUPrecision FPUPrecision(FPT_53BIT);

	// clear eventual old data in the world
	Clear();

	// lock all arrays and containers
	LockAll();

	postrmBrsh->ExpectID_t("WRLD"); // 'world'
	// read the world brushes from the file
	ReadBrushes_t(postrmBrsh);
	// read current world state from the file
	ReadState_t(postrmStat);
	postrmStat->ExpectID_t("WEND"); // 'world end'

	// unlock all arrays and containers
	UnlockAll();

	if( _bFileReplacingApplied)
		WarningMessage("Some of files needed to load world have been replaced while loading");
}

/*
 * Load just world brushes from a file with entire world information.
 */
void CWorld::LoadBrushes_t(const CTFileName &fnmWorld) // throw char *
{
	// remember the file
	wo_fnmFileName = fnmWorld;
	// open the file
	CTFileStream strmFile;
	strmFile.Open_t(fnmWorld);

	// check engine build disallowing reinit
	BOOL bNeedsReinit;
	_pNetwork->CheckVersion_t(strmFile, FALSE, bNeedsReinit);
	ASSERT(!bNeedsReinit);

	strmFile.ExpectID_t("WRLD"); // 'world'
	// read the world brushes from the file
	ReadBrushes_t(&strmFile);
}

/*
 * Read world brushes from stream.
 */
void CWorld::ReadBrushes_t( CTStream *istrm)// throw char *
{
	_pfWorldEditingProfile.StartTimer(CWorldEditingProfile::PTI_READBRUSHES);

	// must be in 53bit mode when managing brushes
	CSetFPUPrecision FPUPrecision(FPT_53BIT);
	
	ReadInfo_t(istrm, FALSE);

	CallProgressHook_t(0.0f);
	// read the brushes from the file
	_pwoCurrentLoading = this;
	istrm->DictionaryReadBegin_t();
	CallProgressHook_t(1.0f);

	CallProgressHook_t(0.0f);
	istrm->DictionaryPreload_t();
	CallProgressHook_t(1.0f);
	
	CallProgressHook_t(0.0f);
	wo_baBrushes.Read_t(istrm);
	CallProgressHook_t(1.0f);

	// if there are some terrais in world							// yjpark |<--
	if( istrm->PeekID_t() == CChunkID("TRAR") )			// 'terrain archive'
	{
		CallProgressHook_t(0.0f);
		wo_taTerrains.Read_t( istrm );
		CallProgressHook_t(1.0f);
	}
	else
	{
		CallProgressHook_t(0.0f);

		CTFileName	fnmFullPath = istrm->strm_strStreamDescription;	
		CTFileName	fnmTer = fnmFullPath.NoExt() + CTString( ".wtr" );

		CTFileStream	strmTer;
		strmTer.Open_t( fnmTer );
		wo_taTerrains.Read_t( &strmTer );
		strmTer.Close();

		CallProgressHook_t(1.0f);
	}																// yjpark     -->|

	istrm->DictionaryReadEnd_t();
	_pwoCurrentLoading = NULL;
	_pfWorldEditingProfile.StopTimer(CWorldEditingProfile::PTI_READBRUSHES);
}

/*
 * Write world brushes to stream.
 */
void CWorld::WriteBrushes_t( CTStream *ostrm) // throw char *
{
	WriteInfo_t(ostrm);

	// write the brushes to the file
	ostrm->DictionaryWriteBegin_t(CTString(""), 0);
	wo_baBrushes.Write_t(ostrm);

	// Terrain
	CTFileName	fnmFullPath = ostrm->strm_strStreamDescription;
	CTFileName	fnmTer = fnmFullPath.NoExt() + CTString( ".wtr" );
	CTFileStream	strmTer;
	strmTer.Create_t( fnmTer );
	wo_taTerrains.Write_t( &strmTer );
	strmTer.Close();
	
	ostrm->DictionaryWriteEnd_t();
}

/*
 * Read current world state from stream.
 */


void CWorld::ReadState_t( CTStream *istr) // throw char *
{
	_pfWorldEditingProfile.StartTimer(CWorldEditingProfile::PTI_READSTATE);

	CTmpPrecachingNow tpn;
	_bReadEntitiesByID = FALSE;

	CallProgressHook_t(0.0f);
	wo_slStateDictionaryOffset = istr->DictionaryReadBegin_t();
#if PRELOAD_BY_EXTENSIONS
	DictionaryPreload_t(istr,".tex");
	DictionaryPreload_t(istr,".wav");
	DictionaryPreload_t(istr,".bm");
	DictionaryPreload_t(istr,".bs");
	DictionaryPreload_t(istr,".ba");
	DictionaryPreload_t(istr,".ani");
	DictionaryPreload_t(istr,".mdl");
	DictionaryPreload_t(istr,".ecl");
	DictionaryPreload_t(istr,".sha");
#else
	istr->DictionaryPreload_t();
#endif
	CallProgressHook_t(1.0f);
	istr->ExpectID_t("WSTA"); // world state

	// read the version number
	INDEX iSavedVersion;
	(*istr)>>iSavedVersion;
	// if the version number is the newest
	if(iSavedVersion==WORLDSTATEVERSION_CURRENT) {
		// read current version
		ReadState_new_t(istr);

	// if the version number is not the newest
	} else {

		// if the version can be converted
		if(iSavedVersion==WORLDSTATEVERSION_CURRENT-1) {
			// show warning
//      WarningMessage(
//        "World state version was %d (old).\n"
//        "Auto-converting to version %d.",
//        iSavedVersion, WORLDSTATEVERSION_CURRENT);
			// read previous version
			ReadState_old_t(istr);
		// if the version can be converted
		} else if(iSavedVersion==WORLDSTATEVERSION_CURRENT-2) {
			// show warning
			WarningMessage(
				TRANS("World state version was %d (very old).\n"
				"Auto-converting to version %d."),
				iSavedVersion, WORLDSTATEVERSION_CURRENT);
			// read previous version
			ReadState_veryold_t(istr);
		} else {
			// report error
			ThrowF_t(
				TRANS("World state version is %d (unsupported).\n"
				"Current supported version is %d."),
				iSavedVersion, WORLDSTATEVERSION_CURRENT);
		}
	}
	istr->DictionaryReadEnd_t();

/*
	SetProgressDescription(TRANS("precaching"));
	CallProgressHook_t(0.0f);
	// precache data needed by entities
	if( gam_iPrecachePolicy>=PRECACHE_SMART) {
		PrecacheEntities_t();
	}
	CallProgressHook_t(1.0f);
*/

	_pfWorldEditingProfile.StopTimer(CWorldEditingProfile::PTI_READSTATE);
}

/*
 * Read current world state from stream -- preprevious version.
 */
void CWorld::ReadState_veryold_t( CTStream *istr) // throw char *
{
	// read the world description
	(*istr)>>wo_strDescription;
	// read the world background color
	(*istr)>>wo_colBackground;

	INDEX ienBackgroundViewer = -1;
	// if background viewer entity is saved here
	if (istr->PeekID_t() == CChunkID("BGVW")) {
		// read background viewer entity index
		istr->ExpectID_t("BGVW"); // background viewer
		(*istr)>>ienBackgroundViewer;
	} else {
		BOOL bUseBackgroundTexture;
		(*istr)>>(SLONG &)bUseBackgroundTexture;

		// read the world background texture name
		CTString strBackgroundTexture;
		(*istr)>>strBackgroundTexture;   // saved as string to bypass dependency catcher
		// skip the 6 dummy texture names used for dependencies
		CTFileName fnmDummy;
		(*istr)>>fnmDummy>>fnmDummy>>fnmDummy
					 >>fnmDummy>>fnmDummy>>fnmDummy;
	}

	// if backdrop image data is saved here
	if (istr->PeekID_t() == CChunkID("BRDP")) {
		// read backdrop image data
		istr->ExpectID_t("BRDP"); // backdrop
		(*istr)>>wo_strBackdropUp;
		(*istr)>>wo_strBackdropFt;
		(*istr)>>wo_strBackdropRt;
		(*istr)>>wo_fUpW>>wo_fUpL>>wo_fUpCX>>wo_fUpCZ;
		(*istr)>>wo_fFtW>>wo_fFtH>>wo_fFtCX>>wo_fFtCY;
		(*istr)>>wo_fRtL>>wo_fRtH>>wo_fRtCZ>>wo_fRtCY;
	}

	// if backdrop object name is saved here
	if (istr->PeekID_t() == CChunkID("BDRO")) {
		// read backdrop object name
		istr->ExpectID_t("BDRO"); // backdrop object
		(*istr)>>wo_strBackdropObject;
	}

	// if viewer position should be loaded
	if (istr->PeekID_t() == CChunkID("VWPS")) {
		istr->ExpectID_t("VWPS"); // viewer position
		(*istr)>>wo_plFocus;
		(*istr)>>wo_fTargetDistance;
	}

	istr->ExpectID_t("SHAN"); // shadow animations
	// for all anim objects
	{for(INDEX iao=0; iao<256; iao++) {
		// skip animation object
		CAnimObject ao;
		ao.Read_t(istr);
	}}

	istr->ExpectID_t("ECLs"); // entity classes
	// read number of entity classes
	INDEX ctEntityClasses;
	(*istr)>>ctEntityClasses;

	CStaticArray<CTFileName> cecClasses;
	cecClasses.New(ctEntityClasses);
	// for each entity class
	{for(INDEX iEntityClass=0; iEntityClass<ctEntityClasses; iEntityClass++) {
		// load filename
		(*istr)>>cecClasses[iEntityClass];
	}}

	/* NOTE: Entities must be loaded in two passes, since all entities must be created
	 * before any entity pointer properties can be loaded.
	 */
	istr->ExpectID_t("ENTs"); // entities
	// read number of entities
	INDEX ctEntities;
	(*istr)>>ctEntities;

	// for each entity
	{for(INDEX iEntity=0; iEntity<ctEntities; iEntity++) {
		INDEX iEntityClass;
		CPlacement3D plPlacement;
		// read entity class index and entity placement
		(*istr)>>iEntityClass>>plPlacement;
		// create an entity of that class
		CEntity *penNew = CreateEntity_t(plPlacement, cecClasses[iEntityClass]);
	}}

	// for each entity
	{for(INDEX iEntity=0; iEntity<ctEntities; iEntity++) {
		// deserialize entity from stream
		wo_cenAllEntities[iEntity].Read_t(istr,FALSE);
	}}

	// after all entities have been read, set the background viewer entity
	if (ienBackgroundViewer==-1) {
		SetBackgroundViewer(NULL);
	} else {
		SetBackgroundViewer(wo_cenAllEntities.Pointer(ienBackgroundViewer));
	}

	// for each entity
	{for(INDEX iEntity=0; iEntity<ctEntities; iEntity++) {
		CEntity &en = wo_cenAllEntities[iEntity];
		// if the entity is destroyed
		if (en.en_ulFlags&ENF_DELETED) {
			// remove the reference made by itself
			ASSERT(en.en_ctReferences>1); // must be referenced by someone else too
			en.RemReference();
			wo_cenEntities.Remove(&en);
		}
	}}

	// after all entities have been read and brushes are connected to entities,
	// calculate bounding boxes of all brushes
	wo_baBrushes.CalculateBoundingBoxes();
	// after all bounding boxes and BSP trees are created,
	// create links between portals and sectors on their other side
	wo_baBrushes.LinkPortalsAndSectors();
	wo_bPortalLinksUpToDate = TRUE;
	// create links between sectors and non-zoning entities in sectors
	LinkEntitiesToSectors();
}

/*
 * Read current world state from stream -- previous version.
 */
void CWorld::ReadState_old_t( CTStream *istr) // throw char *
{
	// read the world description
	(*istr)>>wo_strDescription;
	// read the world background color
	(*istr)>>wo_colBackground;

	INDEX ienBackgroundViewer = -1;
	// read background viewer entity index
	istr->ExpectID_t("BGVW"); // background viewer
	(*istr)>>ienBackgroundViewer;

	// read backdrop image data
	istr->ExpectID_t("BRDP"); // backdrop
	(*istr)>>wo_strBackdropUp;
	(*istr)>>wo_strBackdropFt;
	(*istr)>>wo_strBackdropRt;
	(*istr)>>wo_fUpW>>wo_fUpL>>wo_fUpCX>>wo_fUpCZ;
	(*istr)>>wo_fFtW>>wo_fFtH>>wo_fFtCX>>wo_fFtCY;
	(*istr)>>wo_fRtL>>wo_fRtH>>wo_fRtCZ>>wo_fRtCY;

	// read backdrop object name
	istr->ExpectID_t("BDRO"); // backdrop object
	(*istr)>>wo_strBackdropObject;

	istr->ExpectID_t("VWPS"); // viewer position
	(*istr)>>wo_plFocus;
	(*istr)>>wo_fTargetDistance;

	// if thumbnail saving position should be loaded
	if (istr->PeekID_t() == CChunkID("TBPS")) {
		istr->ExpectID_t("TBPS"); // thumbnail position
		(*istr)>>wo_plThumbnailFocus;
		(*istr)>>wo_fThumbnailTargetDistance;
	}

	istr->ExpectID_t("SHAN"); // shadow animations
	// for all anim objects
	{for(INDEX iao=0; iao<256; iao++) {
		// skip animation object
		CAnimObject ao;
		ao.Read_t(istr);
	}}

	istr->ExpectID_t("ECLs"); // entity classes
	// read number of entity classes
	INDEX ctEntityClasses;
	(*istr)>>ctEntityClasses;

	CStaticArray<CTFileName> cecClasses;
	cecClasses.New(ctEntityClasses);
	// for each entity class
	{for(INDEX iEntityClass=0; iEntityClass<ctEntityClasses; iEntityClass++) {
		// load filename
		(*istr)>>cecClasses[iEntityClass];
	}}

	/* NOTE: Entities must be loaded in two passes, since all entities must be created
	 * before any entity pointer properties can be loaded.
	 */
	istr->ExpectID_t("ENTs"); // entities
	// read number of entities
	INDEX ctEntities;
	(*istr)>>ctEntities;

	// for each entity
	{for(INDEX iEntity=0; iEntity<ctEntities; iEntity++) {
		INDEX iEntityClass;
		CPlacement3D plPlacement;
		// read entity class index and entity placement
		(*istr)>>iEntityClass>>plPlacement;
		// create an entity of that class
		CEntity *penNew = CreateEntity_t(plPlacement, cecClasses[iEntityClass]);
	}}

	// for each entity
	{for(INDEX iEntity=0; iEntity<ctEntities; iEntity++) {
		// deserialize entity from stream
		wo_cenAllEntities[iEntity].Read_t(istr,FALSE);
	}}
	
	// after all entities have been read, set the background viewer entity
	if (ienBackgroundViewer==-1) {
		SetBackgroundViewer(NULL);
	} else {
		SetBackgroundViewer(wo_cenAllEntities.Pointer(ienBackgroundViewer));
	}

	// for each entity
	{for(INDEX iEntity=0; iEntity<ctEntities; iEntity++) {
		CEntity &en = wo_cenAllEntities[iEntity];
		// if the entity is destroyed
		if (en.en_ulFlags&ENF_DELETED) {
			// remove the reference made by itself
			ASSERT(en.en_ctReferences>1); // must be referenced by someone else too
			en.RemReference();
			wo_cenEntities.Remove(&en);
		}
	}}

	// after all entities have been read and brushes are connected to entities,
	// calculate bounding boxes of all brushes
	wo_baBrushes.CalculateBoundingBoxes();
	// after all bounding boxes and BSP trees are created,
	// create links between portals and sectors on their other side if needed
	if (!_bPortalSectorLinksPreLoaded) {
		wo_baBrushes.LinkPortalsAndSectors();
	}
	wo_bPortalLinksUpToDate = TRUE;
	_bPortalSectorLinksPreLoaded = FALSE;
	// create links between sectors and non-zoning entities in sectors
	LinkEntitiesToSectors();
}

/*
 * Read current world state from stream -- current version.
 */
void CWorld::ReadState_new_t( CTStream *istr) // throw char *
{
	if (!_bLoadingEffectData)
	{
		_bLoadingEffectData = TRUE;
		Initialize_EffectSystem(); // 이펙트 데이터 로딩
	}
	// read the world info
	ReadInfo_t(istr, TRUE);

	// read the world background color
	(*istr)>>wo_colBackground;

	// read entity ID counter
	if (istr->PeekID_t()==CChunkID("NFID")) {
		istr->ExpectID_t("NFID");
		(*istr)>>wo_ulNextEntityID;
	} else {
		wo_ulNextEntityID = 1;
	}

	INDEX ienBackgroundViewer = -1;
	// read background viewer entity index
	istr->ExpectID_t("BGVW"); // background viewer
	(*istr)>>ienBackgroundViewer;

	// read backdrop image data
	istr->ExpectID_t("BRDP"); // backdrop
	(*istr)>>wo_strBackdropUp;
	(*istr)>>wo_strBackdropFt;
	(*istr)>>wo_strBackdropRt;
	(*istr)>>wo_fUpW>>wo_fUpL>>wo_fUpCX>>wo_fUpCZ;
	(*istr)>>wo_fFtW>>wo_fFtH>>wo_fFtCX>>wo_fFtCY;
	(*istr)>>wo_fRtL>>wo_fRtH>>wo_fRtCZ>>wo_fRtCY;

	// read backdrop object name
	istr->ExpectID_t("BDRO"); // backdrop object
	(*istr)>>wo_strBackdropObject;

	istr->ExpectID_t("VWPS"); // viewer position
	(*istr)>>wo_plFocus;
	(*istr)>>wo_fTargetDistance;

	// if thumbnail saving position should be loaded
	if (istr->PeekID_t() == CChunkID("TBPS")) {
		istr->ExpectID_t("TBPS"); // thumbnail position
		(*istr)>>wo_plThumbnailFocus;
		(*istr)>>wo_fThumbnailTargetDistance;
	}

	/* NOTE: Entities must be loaded in two passes, since all entities must be created
	 * before any entity pointer properties can be loaded.
	 */
	if (istr->PeekID_t()== CChunkID("ENTs")) {
		istr->ExpectID_t("ENTs"); // entities
	} else {
		istr->ExpectID_t("ENs2"); // entities v2
		_bReadEntitiesByID = TRUE;
	}
	// read number of entities
	INDEX ctEntities;
	(*istr)>>ctEntities;

	// allocate world entity hash table
	INDEX iNumCompartments = ctEntities/10+1;
	wo_whWorldEntityContainer.Clear();
	wo_whWorldEntityContainer.SetAllocationParameters(iNumCompartments,20,5);

	extern INDEX	g_iCountry;
	CallProgressHook_t(0.0f);
	// for each entity
	{for(INDEX iEntity=0; iEntity<ctEntities; iEntity++) {
		// read entity id if needed
		ULONG ulID = 0;
		if (_bReadEntitiesByID) {
			(*istr)>>ulID;
		}
		// read entity class and entity placement
		CTFileName fnmClass;
		CPlacement3D plPlacement;
		(*istr)>>fnmClass>>plPlacement;

		// create an entity of that class
		// adjust id if needed
		if (_bReadEntitiesByID) {
			CEntity *penNew = CreateEntity_t(plPlacement, fnmClass,ulID);
		} else {
			CEntity *penNew = CreateEntity_t(plPlacement, fnmClass);
		}
		CallProgressHook_t(FLOAT(iEntity)/ctEntities);
	}}
	CallProgressHook_t(1.0f);
		
	//if(g_iCountry == RUSSIA)
	CallProgressHook_t(0.0f);
	
	CallProgressHook_t(1.0f);
 //0507 kwon 추가.
	CallProgressHook_t(0.0f);
	CEntity* penEntity = NULL; 
	CPlacement3D plPlacement;
	plPlacement.pl_PositionVector = FLOAT3D(0,0,0);
	plPlacement.pl_OrientationAngle = ANGLE3D(0,0,0);

	LoadLOD();

	//if(g_iCountry == RUSSIA)
	CallProgressHook_t(0.0f);
	// for each entity
	{for(INDEX iEntity=0; iEntity<ctEntities; iEntity++) {
		// deserialize entity from stream
		wo_cenAllEntities[iEntity].Read_t(istr,FALSE);
		CallProgressHook_t(FLOAT(iEntity)/ctEntities);
	}}
	CallProgressHook_t(1.0f);

	// after all entities have been read, set the background viewer entity
	if (ienBackgroundViewer==-1) {
		SetBackgroundViewer(NULL);
	} else {
		CEntity *penBcg;
		if (_bReadEntitiesByID) {
			penBcg = EntityFromID(ienBackgroundViewer);
		} else {
			penBcg = wo_cenAllEntities.Pointer(ienBackgroundViewer);
		}
		SetBackgroundViewer(penBcg);
	}

	wo_cenEntities.Unlock();
	// for each entity
	{for(INDEX iEntity=0; iEntity<ctEntities; iEntity++) {
		CEntity &en = wo_cenAllEntities[iEntity];
		// if the entity is destroyed
		if (en.en_ulFlags&ENF_DELETED) {
			// remove the reference made by itself
			ASSERT(en.en_ctReferences>1); // must be referenced by someone else too
			en.RemReference();
			wo_cenEntities.Remove(&en);
		}
	}}
	wo_cenEntities.Lock();

	// if version with entity order
	if (istr->PeekID_t()==CChunkID("ENOR")) { // entity order
		istr->ExpectID_t(CChunkID("ENOR")); // entity order
		INDEX ctEntities;
		*istr>>ctEntities;
		wo_cenEntities.Clear();
		// for each non-deleted entity
		for(INDEX i=0; i<ctEntities; i++) {
			ULONG ulID;
			*istr>>ulID;
			wo_cenEntities.Add(EntityFromID(ulID));
		}
	}

	// some shadow layers might not have light sources, remove such to prevent crashes
	wo_baBrushes.RemoveDummyLayers();

	CallProgressHook_t(0.0f);
	// after all entities have been read and brushes are connected to entities,
	// calculate bounding boxes of all brushes
	wo_baBrushes.CalculateBoundingBoxes();
	CallProgressHook_t(0.3f);
	// after all bounding boxes and BSP trees are created,
	// create links between portals and sectors on their other side if needed
	if (!_bPortalSectorLinksPreLoaded) {
		wo_baBrushes.LinkPortalsAndSectors();
	}
	wo_bPortalLinksUpToDate = TRUE;
	// create links between sectors and non-zoning entities in sectors
	wo_baBrushes.ReadEntitySectorLinks_t(*istr);
	CallProgressHook_t(0.6f);
	LinkEntitiesToSectors();
	CallProgressHook_t(1.0f);
	_bPortalSectorLinksPreLoaded = FALSE;
	_bEntitySectorLinksPreLoaded = FALSE;

	m_bLoad = TRUE;
}

struct SEntityBasicData {
	ULONG ebd_ulEntityID;
	INDEX ebd_iClassID;
	CPlacement3D ebd_plPlacement;
	BOOL  bAlreadyExists;
};


//! 서버로 부터 받은 스트림으로부터 현재의 월드 스테이트를 읽는다.
/*
 * Read current world state from stream recieved from server -- current version.
 */
void CWorld::ReadState_net_t(CTStream *istr) // throw char *
{
	SetFPUPrecision(FPT_53BIT);
	INDEX ienBackgroundViewer = -1;
	// read background viewer entity index
	istr->ExpectID_t("BGVW"); // background viewer
	(*istr)>>ienBackgroundViewer;
	
	// receive SKA string table - conversion between string names of SKA components
	INDEX ctStrings;  

	//! 스카 스트링 테이블을 받는다.
	istr->ExpectID_t("SKST");
	(*istr)>>ctStrings;
	if (ctStrings>0) {
		//! 로컬 스카 아이디를 네트워크에서 받은 스카 아이디로 바꾼다.
		// make room for server id's in the convesion table
		_aiNetSkaIDConversion.PopAll();
		_aiNetSkaIDConversion.Push(ctStrings);
	}

	INDEX iClientID;
	CTString strSkaName;
//  UWORD strLen;

	for (INDEX iString=0;iString<ctStrings;iString++) {
		strSkaName.ReadFromStreamShort(istr);
		iClientID = ska_GetIDFromStringTable(strSkaName);
		_aiNetSkaIDConversion[iString] = iClientID;
	}

	//! 사용한 엔티티 클래스 이름과 아이디를 읽는다.
	// read used entity class names and id's
	INDEX ctClasses;
	istr->ExpectID_t("ECLT");
	// empty the existing container  
	ClearEntityClassContainer();
		
	(*istr)>>ctClasses; 
	for (INDEX iEntry=0;iEntry<ctClasses;iEntry++) {
		EntityClassEntry *peceEntry = new EntityClassEntry;
		(*istr) >> peceEntry->ece_iClassID;
		peceEntry->ece_fnmEntityClass.ReadFromStreamShort(istr);
		_cEntityClassContainer.Add(peceEntry);
	}



	istr->ExpectID_t("ENs2"); // entities v2
	_bReadEntitiesByID = TRUE;

	//! 엔티티의 갯수를 읽는다.
	// read number of entities
	INDEX ctEntities;
	(*istr)>>ctEntities;

	CStaticArray<SEntityBasicData> aebdReceivedEntityList;
	aebdReceivedEntityList.New(ctEntities);

	CallProgressHook_t(0.0f);
	//! 각 엔티티에 대하여 엔티티id,클래스 아이디,위치를 읽어들인다.
	// for each entity
	{for(INDEX iEntity=0; iEntity<ctEntities; iEntity++) {
		// read entity id if needed
		ULONG ulEntityID;
		// read entity class and entity placement
		INDEX iClassID;
		CPlacement3D plPlacement;

		(*istr)>>ulEntityID;
		(*istr)>>iClassID>>plPlacement;
		
		// put the entity into the array of received id's
		aebdReceivedEntityList[iEntity].ebd_ulEntityID  = ulEntityID;
		aebdReceivedEntityList[iEntity].ebd_iClassID    = iClassID;
		aebdReceivedEntityList[iEntity].ebd_plPlacement = plPlacement;
		aebdReceivedEntityList[iEntity].bAlreadyExists  = FALSE;

		CallProgressHook_t(FLOAT(iEntity)/ctEntities);
	}}
	CallProgressHook_t(1.0f);

	CallProgressHook_t(0.0f);

	//! 엔티티 업데이트.
	INDEX iEntity = 0;
	CStaticStackArray<ULONG> saToDelete;
	//! 모든 엔티티 중에서 id가 같은 엔티티를 찾아서 업데이트
	// for all existing entities - try to find them in the list of receieved entities
	FOREACHINDYNAMICCONTAINER(wo_cenAllEntities, CEntity, iten) {  
		CEntity* penEntity = iten;
		iEntity++;
		BOOL bFound = FALSE;
		FOREACHINSTATICARRAY(aebdReceivedEntityList,SEntityBasicData,itebd) {  
			if (iten->en_ulID == itebd->ebd_ulEntityID) {
//        penEntity = iten;
				// if found, adjust it's placement - we will update the rest of the data later
				// wrap angles, so we can compare them
				for (int i=1;i<=3;i++) {
					itebd->ebd_plPlacement.pl_OrientationAngle(i) = WrapAngle(itebd->ebd_plPlacement.pl_OrientationAngle(i));
					iten->en_plPlacement.pl_OrientationAngle(i) = WrapAngle(iten->en_plPlacement.pl_OrientationAngle(i));
				}
		//! 위치 갱신.
				// if the placement of this entity hasn't changed, don't update - it could mess up shadows
				if (!(iten->en_plPlacement == itebd->ebd_plPlacement)) {
					iten->SetPlacement(itebd->ebd_plPlacement);
				}
				if (iten->en_ulPhysicsFlags&EPF_MOVABLE) {
					CPlacement3D plSpeed;
					plSpeed.pl_PositionVector   = FLOAT3D(0,0,0);
					plSpeed.pl_OrientationAngle = ANGLE3D(0,0,0);
		  //! 데드렉커닝이 뭔지 모르겠다..ㅡㅡ
					((CMovableEntity*)((CEntity*)iten))->AdjustDeadReckoning(itebd->ebd_plPlacement,plSpeed,_pTimer->CurrentTick());
				}
				bFound = TRUE;
				itebd->bAlreadyExists = TRUE;
				break;      
			}
		}

		// if not found, mark it for deletions
		if (!bFound && iten->IsSentOverNet() && iten->IsFlagOff(ENF_NONETCONNECT)) {
			saToDelete.Push() = iten->en_ulID;
		}

		CallProgressHook_t(FLOAT(iEntity)/wo_cenAllEntities.Count());
	}
	//!업데이트 하려던 엔티티중에 id를 못찾아서 업데이트 못한 데이타 삭제하기.
	// now delete all marked entities
	for (INDEX iID=0;iID<saToDelete.Count();iID++) {
		CEntity* penToDelete;
		if (EntityExists(saToDelete[iID],penToDelete)) {
			// kill all pointers referencing this entity
			UntargetEntity(penToDelete);
			if (EntityExists(saToDelete[iID],penToDelete)) {
				// and destroy it
				penToDelete->Destroy();
			}
		}
	}
	saToDelete.PopAll();

	CallProgressHook_t(1.0f);

	CallProgressHook_t(0.0f);
	iEntity=0;
	//! 받은 엔티티중에 리스트에 존재하지 않는 엔티티를 생성한다.
	// all entites in the list of received entities that are not marked as already existing
	// are new, so create them
	{FOREACHINSTATICARRAY(aebdReceivedEntityList,SEntityBasicData,itebd) {  
		SEntityBasicData* sebd = itebd;
		if (!(itebd->bAlreadyExists)) {
			EntityClassEntry* peceEntry;
			CEntity* peEntity;

			peceEntry = FindClassInContainer(itebd->ebd_iClassID);
			ASSERT(peceEntry != NULL);

			peEntity = CreateEntity_t(itebd->ebd_plPlacement,peceEntry->ece_fnmEntityClass,itebd->ebd_ulEntityID);
			ASSERT(peEntity != NULL);
		}
		iEntity++;
		CallProgressHook_t(FLOAT(iEntity)/ctEntities/2);
	}}

	iEntity=0;
	//! 엔티티를 로딩한다.
	// now load all of their properties
	{FOREACHINSTATICARRAY(aebdReceivedEntityList,SEntityBasicData,itebd) {  
//    if (!(itebd->bAlreadyExists)) {
			CEntity* peEntity;

			peEntity = EntityFromID(itebd->ebd_ulEntityID);
			ASSERT(peEntity != NULL);
			peEntity->End();
			peEntity->Read_t(istr,TRUE);
			peEntity->en_plLastPlacementReceived = peEntity->en_plPlacement;
//    }                         
		iEntity++;
		CallProgressHook_t(FLOAT(iEntity+ctEntities)/ctEntities/2);
	}}

	CallProgressHook_t(1.0f);
	
 //! 생성된 모든 엔티티를 초기화 한다.
	// initialize all entities created during loading
	{FOREACHINSTATICARRAY(aebdReceivedEntityList,SEntityBasicData,itebd) {  
		if (!itebd->bAlreadyExists) {
			CEntity* peEntity = EntityFromID(itebd->ebd_ulEntityID);
			ASSERT(peEntity != NULL);
			peEntity->End();
			peEntity->InitializeFromNet();
		}
	}} 
	
	CallProgressHook_t(0.25f);
	//! 모든 엔티티를 읽은후에 백그라운드뷰어 엔티티를 세팅한다.
	// after all entities have been read, set the background viewer entity
	if (ienBackgroundViewer==-1) {
		SetBackgroundViewer(NULL);
	} else {
		CEntity *penBcg;
		penBcg = EntityFromID(ienBackgroundViewer);
		SetBackgroundViewer(penBcg);
	}

	CallProgressHook_t(0.5f);
	//! 파괴된 엔티티의 리스트를 읽어서 그들을 파괴한다.
	// read list of destroyed static entities and destroy them locally
	istr->ExpectID_t("EDST");
	ULONG ulDestroyed,ulEntityID;
	(*istr) >> ulDestroyed;
	for (iEntity=0;iEntity<ulDestroyed;iEntity++) {
		(*istr) >> ulEntityID;
		CEntity* penEntity;
		if (EntityExists(ulEntityID,penEntity)) {
			// kill all pointers referencing this entity
			UntargetEntity(penEntity);
			// and destroy it
			penEntity->Destroy();
		}  
	}
	CallProgressHook_t(0.75f);
	RebuildLinks();

	//! 각 플레이어 엔티티를 플레이어 타겟으로 연결하고, 그들의 애니메이션큐를 읽는다.
	// link player entities to player targets and read their anim queues
	istr->ExpectID_t("PLRS"); // players
	// for each player target
	FOREACHINSTATICARRAY(_pNetwork->ga_srvServer.srv_apltPlayers, CPlayerTarget, itclt) {
		// write it's ID
		(*istr) >> ulEntityID; 
		
		// 0xFFFFFFFF - no player entity
		// 0xFFFFFFFE - player entity not a ska model
		if ((ulEntityID != 0xFFFFFFFF) && (ulEntityID != 0xFFFFFFFE)) {
			itclt->plt_penPlayerEntity = (CPlayerEntity*) EntityFromID(ulEntityID);
			itclt->plt_penPlayerEntity->ReadSKAQueue(istr);
			itclt->plt_penPlayerEntity->ClientCacheNearPolygons();
		} else if (ulEntityID == 0xFFFFFFFF) {
			itclt->plt_penPlayerEntity = NULL;
		} // else if (ulEntityID == 0xFFFFFFFE) do nothing

	}



	CallProgressHook_t(1.0f);


}

//! 클라이언트로 보낼 월드 스테이트
/*
 * Write current world state to stream to be send to the client.
 */
void CWorld::WriteState_net_t( CTStream *ostr ) // throw char *
{
	SetFPUPrecision(FPT_53BIT);
	 // write background viewer entity if any
	ostr->WriteID_t("BGVW"); // background viewer
	CEntity *penBackgroundViewer = GetBackgroundViewer();
	if (penBackgroundViewer==NULL) {
		(*ostr)<<INDEX(-1);
	} else {
		(*ostr)<<penBackgroundViewer->en_ulID;
	}


	// send SKA string table - conversion between string names of SKA components
	INDEX ctStrings = _astrStringTable.Count();
	ostr->WriteID_t("SKST");
	(*ostr)<<ctStrings;
	for (INDEX iString=0;iString<ctStrings;iString++) {
		_astrStringTable[iString].WriteToStreamShort(ostr);
	}


	// write used entity class names and id's
	INDEX ctClasses = _cEntityClassContainer.Count();
	ostr->WriteID_t("ECLT");
	(*ostr)<<ctClasses;
	UBYTE* pubStart =  ostr->strm_pubCurrentPos;
	for (INDEX iEntry=0;iEntry<ctClasses;iEntry++) {
		EntityClassEntry &eceEntry = _cEntityClassContainer[iEntry];
		(*ostr) << eceEntry.ece_iClassID;
		eceEntry.ece_fnmEntityClass.WriteToStreamShort(ostr);
	}
	UBYTE* pubEnd =  ostr->strm_pubCurrentPos;




	/* NOTE: Entities must be saved in two passes, so that they can be loaded in two passes,
	 * since all entities must be created before any entity pointer properties can be loaded.
	 */
	ostr->WriteID_t("ENs2"); // entities
	INDEX ctEntities = wo_cenEntities.Count();
	// for each entity
	{FOREACHINDYNAMICCONTAINER(wo_cenEntities, CEntity, iten) {
		CEntity &en = *iten;
		if (!en.IsSentOverNet() || (iten->IsFlagOn(ENF_NONETCONNECT) && iten->IsFlagOff(ENF_PLACCHANGED) && iten->IsFlagOff(ENF_PROPSCHANGED))) {
			ctEntities--;
		}
	}}

	// write number of entities
	(*ostr)<< ctEntities;
	// for each entity
	{FOREACHINDYNAMICCONTAINER(wo_cenEntities, CEntity, iten) {
		CEntity &en = *iten;
		if (!en.IsSentOverNet() || (iten->IsFlagOn(ENF_NONETCONNECT) && iten->IsFlagOff(ENF_PLACCHANGED) && iten->IsFlagOff(ENF_PROPSCHANGED))) {
			continue;
		}

		// write the id, class and its placement
		(*ostr)<<en.en_ulID<<en.en_pecClass->ec_pdecDLLClass->dec_iID<<en.en_plPlacement;

	}}


	// for each entity
	{FOREACHINDYNAMICCONTAINER(wo_cenEntities, CEntity, iten) {
		CEntity &en = *iten;
		if (!en.IsSentOverNet() || (iten->IsFlagOn(ENF_NONETCONNECT) && iten->IsFlagOff(ENF_PLACCHANGED) && iten->IsFlagOff(ENF_PROPSCHANGED))) {
			continue;
		}

		// remember stream position
		SLONG slOffset = ostr->GetPos_t();
		// serialize entity into stream (with network option)
		iten->Write_t(ostr,TRUE);
		// save the size of data in start chunk, after chunkid and entity id
		SLONG slOffsetAfter = ostr->GetPos_t();
		ostr->SetPos_t(slOffset+2*sizeof(SLONG));
		*ostr<<SLONG(slOffsetAfter-slOffset-3*sizeof(SLONG));
		ostr->SetPos_t(slOffsetAfter);
	}}



	// write the list of destroyed static entities (those with ENF_MARKDESTROY set)
	ostr->WriteID_t("EDST");
	ULONG ulDestroyed = wo_aulDestroyedEntities.Count();
	(*ostr) << ulDestroyed;
	for (INDEX iEntity=0;iEntity<ulDestroyed;iEntity++) {
		(*ostr) << wo_aulDestroyedEntities[iEntity];
	}

	// now just write id's and anim queues of all player entities in order of their player targets
	ostr->WriteID_t("PLRS"); // players
	// for each player target
	FOREACHINSTATICARRAY(_pNetwork->ga_srvServer.srv_apltPlayers, CPlayerTarget, itclt) {
		// write it's ID and anim queue
		if (itclt->plt_penPlayerEntity != NULL && 
			 (itclt->plt_penPlayerEntity->en_RenderType == CEntity::RT_SKAMODEL || 
				itclt->plt_penPlayerEntity->en_RenderType == CEntity::RT_SKAEDITORMODEL)) 
		{
			(*ostr) << itclt->plt_penPlayerEntity->en_ulID;
			itclt->plt_penPlayerEntity->WriteSKAQueue(ostr);
		} else if (itclt->plt_penPlayerEntity != NULL && 
			 (itclt->plt_penPlayerEntity->en_RenderType == CEntity::RT_SKAMODEL || 
				itclt->plt_penPlayerEntity->en_RenderType == CEntity::RT_SKAEDITORMODEL)) 
		{
			(*ostr) << (ULONG)0xFFFFFFFE; // -2 for non ska
		} else {
			(*ostr) << (ULONG)0xFFFFFFFF; // -1 for NULLS
		}
	}
}


/*
 * Write current world state to stream.
 */
void CWorld::WriteState_t( CTStream *ostr, BOOL bImportDictionary /* = FALSE */) // throw char *
{
	if (bImportDictionary) {
		ostr->DictionaryWriteBegin_t(wo_fnmFileName, wo_slStateDictionaryOffset);
	} else {
		ostr->DictionaryWriteBegin_t(CTString(""), 0);
	}
	(*ostr).WriteID_t("WSTA"); // world state
	// write the world version
	(*ostr)<<INDEX(WORLDSTATEVERSION_CURRENT);

	// write the world description
	WriteInfo_t(ostr);

	// write the world background color
	(*ostr)<<wo_colBackground;

	// write entity ID counter
	ostr->WriteID_t("NFID");
	(*ostr)<<wo_ulNextEntityID;

	// write background viewer entity if any
	ostr->WriteID_t("BGVW"); // background viewer
	CEntity *penBackgroundViewer = GetBackgroundViewer();
	if (penBackgroundViewer==NULL) {
		(*ostr)<<INDEX(-1);
	} else {
		(*ostr)<<penBackgroundViewer->en_ulID;
	}

	// write backdrop image data
	ostr->WriteID_t("BRDP"); // backdrop
	(*ostr)<<wo_strBackdropUp;
	(*ostr)<<wo_strBackdropFt;
	(*ostr)<<wo_strBackdropRt;
	(*ostr)<<wo_fUpW<<wo_fUpL<<wo_fUpCX<<wo_fUpCZ;
	(*ostr)<<wo_fFtW<<wo_fFtH<<wo_fFtCX<<wo_fFtCY;
	(*ostr)<<wo_fRtL<<wo_fRtH<<wo_fRtCZ<<wo_fRtCY;

	// write backdrop object name
	ostr->WriteID_t("BDRO"); // backdrop object
	(*ostr)<<wo_strBackdropObject;

	// write viewer position
	ostr->WriteID_t("VWPS"); // viewer placement
	(*ostr)<<wo_plFocus;
	(*ostr)<<wo_fTargetDistance;
	// write thumbnail position
	ostr->WriteID_t("TBPS"); // thumbnail placement
	(*ostr)<<wo_plThumbnailFocus;
	(*ostr)<<wo_fThumbnailTargetDistance;

	/* NOTE: Entities must be saved in two passes, so that they can be loaded in two passes,
	 * since all entities must be created before any entity pointer properties can be loaded.
	 */
	ostr->WriteID_t("ENs2"); // entities
	// write number of entities
	(*ostr)<<wo_cenAllEntities.Count();
	// for each entity
	{FOREACHINDYNAMICCONTAINER(wo_cenAllEntities, CEntity, iten) {
		CEntity &en = *iten;
		// write the id, class and its placement
		(*ostr)<<en.en_ulID<<en.en_pecClass->GetName()<<en.en_plPlacement;
	}}
	// for each entity
	{FOREACHINDYNAMICCONTAINER(wo_cenAllEntities, CEntity, iten) {
		// remember stream position
		SLONG slOffset = ostr->GetPos_t();
		// serialize entity into stream
		iten->Write_t(ostr,FALSE);
		// save the size of data in start chunk, after chunkid and entity id
		SLONG slOffsetAfter = ostr->GetPos_t();
		ostr->SetPos_t(slOffset+2*sizeof(SLONG));
		*ostr<<SLONG(slOffsetAfter-slOffset-3*sizeof(SLONG));
		ostr->SetPos_t(slOffsetAfter);
	}}

	ostr->WriteID_t(CChunkID("ENOR")); // entity order
	*ostr<<wo_cenEntities.Count();
	// for each non-deleted entity
	{FOREACHINDYNAMICCONTAINER(wo_cenEntities, CEntity, iten) {
		// write its id
		*ostr<<iten->en_ulID;
	}}

	wo_baBrushes.WriteEntitySectorLinks_t(*ostr);

	ostr->DictionaryWriteEnd_t();
}





// read/write world information (description, name, flags...)
void CWorld::ReadInfo_t(CTStream *strm, BOOL bMaybeDescription) // throw char *
{
	// if version with world info
	if (strm->PeekID_t()==CChunkID("WLIF")) { // world info
		strm->ExpectID_t(CChunkID("WLIF")); // world info

		// skip eventual translation chunk
		if (strm->PeekID_t()==CChunkID("DTRS")) {
			strm->ExpectID_t("DTRS");
		}
		// read the name
		(*strm)>>wo_strName;
		// read the flags
		(*strm)>>wo_ulSpawnFlags;
		// read the world description
		(*strm)>>wo_strDescription;

	// if version with description only
	} else if (bMaybeDescription) {
		// read the world description
		(*strm)>>wo_strDescription;
	}
}

void CWorld::WriteInfo_t(CTStream *strm) // throw char *
{
	strm->WriteID_t(CChunkID("WLIF"));  // world info
	// write the name
	strm->WriteID_t(CChunkID("DTRS"));
	(*strm)<<wo_strName;
	// write the flags
	(*strm)<<wo_ulSpawnFlags;
	// write the world description
	(*strm)<<wo_strDescription;
}

// yjpark |<--
#ifndef	FINALVERSION
void CWorld::WriteAttributeMap( CTFileName &fnm, BOOL bField, INDEX iSelFloor, PIX pixWidth, PIX pixHeight, std::vector<int>& vectorProrityList )
{
	if( vectorProrityList.empty() )
		return;
	
	try
	{
		// Allocate memory
		PIX		pixAttMapSizeHalf = pixWidth * pixHeight;
		PIX		pixAttributeMapSize = pixAttMapSizeHalf * sizeof(UWORD);		
		UWORD*	pubAttrBuffer = (UWORD*)AllocMemory(pixAttributeMapSize);

		for (int i = 0; i < pixAttMapSizeHalf; ++i)
		{
			pubAttrBuffer[i] = MATT_UNWALKABLE;
		}

		UWORD*	pubTemp = pubAttrBuffer;
		
		if( bField && iSelFloor == 1 )
		{
			CTFileStream	strmFile;
			strmFile.Open_t( fnm );
			
			for( PIX pix = 0; pix < pixAttMapSizeHalf; pix++ )
			{
				strmFile >> *pubTemp;
				pubTemp++;
			}
			
			strmFile.Close();
		}
		
		// Make the attribute map of brushes
		std::vector<int>::iterator it;
		for( it = vectorProrityList.begin(); it != vectorProrityList.end(); ++it )
		{		
			wo_baBrushes.MakeAttributeMap( (*it), pubAttrBuffer, iSelFloor, pixWidth, pixHeight, this, bField );
		}	
		
		CTFileName	fnmAttributeMap;
		if( fnm.FindSubstr( CTString( "_" ) ) == -1 )
		{	
			CTString	strAttrMapInfo;
			strAttrMapInfo.PrintF( "_%d_%d_%d", pixWidth, pixHeight, iSelFloor - 1 );
			fnmAttributeMap = fnm.NoExt() + strAttrMapInfo + CTString( ".sat" );
		}
		else
			fnmAttributeMap = fnm;
		
		// Save the attribute map to the file
		CTFileStream	strmFile;
		strmFile.Create_t( fnmAttributeMap, CTStream::CM_BINARY );
		strmFile.Write_t( pubAttrBuffer, pixAttributeMapSize );
		strmFile.Close();
		
		// Free memory
		FreeMemory( pubAttrBuffer );
	}
	catch(char *strError)
	{
		ThrowF_t(strError);
	}
}

void CWorld::WriteSvrHeightMap( CTFileName &fnm, BOOL bField, INDEX iSelFloor, PIX pixWidth, PIX pixHeight )
{
	// Allocate memory
	PIX		pixHeightMapSize = pixWidth * pixHeight * sizeof(USHORT);
	UWORD	*puwHeightBuffer = (UWORD*)AllocMemory( pixHeightMapSize );
	memset( puwHeightBuffer, 0, pixHeightMapSize );
	UWORD	*puwTemp = puwHeightBuffer;
	
	if( bField && iSelFloor == 1 )
	{
		CTFileStream	strmFile;
		strmFile.Open_t( fnm );

		PIX	pixHMSize = pixWidth * pixHeight;
		for( PIX pix = 0; pix < pixHMSize; pix++ )
		{
			strmFile >> *puwTemp;
			puwTemp++;
		}

		strmFile.Close();
	}

	// Make the height map of brushes
	wo_baBrushes.MakeSvrHeightMap( puwHeightBuffer, iSelFloor, pixWidth, pixHeight, bField );

	CTFileName	fnmSvrHeightMap;
	if( fnm.FindSubstr( CTString( "_" ) ) == -1 )
	{
		CTString	strSvrHeightMapInfo;
		strSvrHeightMapInfo.PrintF( "_%d_%d_%d", pixWidth, pixHeight, iSelFloor - 1 );
		fnmSvrHeightMap = fnm.NoExt() + strSvrHeightMapInfo + CTString( ".sht" );
	}
	else
		fnmSvrHeightMap = fnm;

	// Save the attribute map to the file
	CTFileStream	strmFile;
	strmFile.Create_t( fnmSvrHeightMap, CTStream::CM_BINARY );
	strmFile.Write_t( puwHeightBuffer, pixHeightMapSize );
	strmFile.Close();

	// Free memory
	FreeMemory( puwHeightBuffer );
}
#endif // FINALVERSION
// yjpark     -->|