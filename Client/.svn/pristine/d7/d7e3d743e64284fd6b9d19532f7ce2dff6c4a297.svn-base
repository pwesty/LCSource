#include "StdH.h"
#include <Engine/Ska/Mesh.h>
#include <Engine/Ska/Skeleton.h>
#include <Engine/Ska/AnimSet.h>
#include <Engine/Ska/ModelInstance.h>
#include <Engine/Ska/Render.h>
#include <Engine/Graphics/Shader.h>
#include <Engine/Graphics/Drawport.h>
#include <Engine/Base/ObjectRestore.h>

static GFXColor _aColors[] = { C_BLACK,
	C_GREEN,   C_BLUE,   C_RED,   C_YELLOW,   C_MAGENTA,   C_CYAN,   C_PINK,   C_ORANGE,   C_BROWN,  C_WHITE, C_GRAY,
	C_vlGREEN, C_vlBLUE, C_vlRED, C_vlYELLOW, C_vlMAGENTA, C_vlCYAN, C_vlPINK, C_vlORANGE, C_vlBROWN,
	C_vdGREEN, C_vdBLUE, C_vdRED, C_vdYELLOW, C_vdMAGENTA, C_vdCYAN, C_vdPINK, C_vdORANGE, C_vdBROWN,
};

extern CStaticStackArray<struct RenModel> _aRenModels;
extern CStaticStackArray<struct RenMesh> _aRenMeshes;
extern CStaticStackArray<struct RenBone> _aRenBones;
extern CStaticStackArray<struct RenMorph> _aRenMorphs;
extern CStaticStackArray<struct RenWeight> _aRenWeights;
extern CStaticStackArray<struct MeshVertex> _aMorphedVertices;
extern CStaticStackArray<struct MeshNormal> _aMorphedNormals;
//안태훈 수정 시작	//(Add Tagent-space Normal Map)(0.1)
extern CStaticStackArray<struct MeshTangent> _aMorphedTangents;
//안태훈 수정 끝	//(Add Tagent-space Normal Map)(0.1)
extern CStaticStackArray<struct MeshVertex> _aFinalVertices;
extern CStaticStackArray<struct MeshNormal> _aFinalNormals;
//안태훈 수정 시작	//(Add Tagent-space Normal Map)(0.1)
extern CStaticStackArray<struct MeshTangent> _aFinalTangents;
//안태훈 수정 끝	//(Add Tagent-space Normal Map)(0.1)
extern CStaticStackArray<struct RenMatrix> _aWeightMatrices;
extern CStaticStackArray<struct SurfaceMatrices> _aSurfacesMatrices;

//안태훈 수정 시작	//(Add & Modify SSSE Effect)(0.1)
extern void CalculateRenderingData(CModelInstance &mi, BOOL bRenderToScreen);
//안태훈 수정 끝	//(Add & Modify SSSE Effect)(0.1)
extern void UnlockMeshReadBuffers(const MeshLOD &mlod);
extern void LockMeshReadBuffers(const MeshLOD &mlod);
extern void PrepareMeshForRendering(RenMesh &rmsh, INDEX iSkeletonLOD);
extern void SetViewMatrix( Matrix12 &mView12);
extern void ClearRenArrays();

extern MeshVertex *_pavWeightedVertices; // final vertex arrays (in software this are morphed and weighted vertices, in hardware default model vertices)
extern MeshNormal *_panWeightedNormals;  // final normal array  (in software this are morphed and weighted normals , in hardware default model normals)
extern MeshVertex *_pavMorphedVertices;  // morphed vertices (contains vertices only for surfaces that have morph maps - if no active morphmaps points to default model vertices)
extern MeshNormal *_panMorphedNormals;   // morphed normals  (contains normals  only for surfaces that have morph maps - if no active morphmaps points to default model normals)
extern BOOL  _bMeshUsesMorphs;           // are morphs active in mesh
extern BOOL  _bTransAllToAbsolute;       // transform all vertices to absolute space
extern BOOL  _bUseHardwareShaders;       // use vertex program
extern INDEX _ctMeshVertices;            // mesh lod vertex count

static MeshVertex *_pavFinalVertices;
static MeshNormal *_panFinalNormals;
//안태훈 수정 시작	//(Add Tagent-space Normal Map)(0.1)
static MeshTangent *_panFinalTangents;
//안태훈 수정 끝	//(Add Tagent-space Normal Map)(0.1)

static CModelInstance *_pmiCurrent = NULL;
static MeshLOD *_pmlodCurrent = NULL;
static MeshSurface *_pmsrfCurrent =NULL;

static CDrawPort *_pdp = NULL;
static MeshLOD *_pmlod = NULL;
static MeshSurface *_pmsrf = NULL;
static CModelInstance *_pmi = NULL;
static SkaPreviewFlags _psf;
static INDEX _iSelectionID = -1;
static INDEX _iModel = 0;
static INDEX _iMesh = 0;
static INDEX _iSurface = 0;
static INDEX _iSubSurface = 0;
static CStaticStackArray<struct GFXColor> _aMeshColors;
static CStaticStackArray<struct MeshVertex> _aStripVertices;
static CStaticStackArray<UWORD> _aStripIndices;
static CStaticStackArray<INDEX> _ctMapsPerVertex;

static void CleanPreview(void)
{
	_pmlod = NULL;
	_pmsrf = NULL;
	_aMeshColors.PopAll();
	_iModel = 0;
	_iMesh = 0;
	_iSurface = 0;
	_iSubSurface = 0;
	_pmiCurrent = NULL;
	_pmlodCurrent = NULL;
	_pmsrfCurrent = NULL;
	_pdp = NULL;
}

static GFXColor GetColor(INDEX iColor)
{
	const int ctColors = sizeof(_aColors)/sizeof(GFXColor);
	iColor = iColor%ctColors;


	_aColors[iColor].abgr |= 0xFF000000;
	return _aColors[iColor];
}

static void ColorizeSelection()
{
	for(INDEX imvx=0;imvx<_ctMeshVertices;imvx++) {
		_aMeshColors[imvx] = 0xcdcdcdcd;
	}
}

static void ColorizeModels()
{
	const GFXColor colModel = GetColor(_iModel);
	for(INDEX imvx=0;imvx<_ctMeshVertices;imvx++) {
		_aMeshColors[imvx] = colModel;
	}
}

static void ColorizeMeshes()
{
	const GFXColor colMesh = GetColor(_iMesh);
	for(INDEX imvx=0;imvx<_ctMeshVertices;imvx++) {
		_aMeshColors[imvx] = colMesh;
	}
}

static void ColorizeSurfaces()
{
	const GFXColor colSurface = GetColor(_iSurface);
	for(INDEX imvx=0;imvx<_ctMeshVertices;imvx++) {
		_aMeshColors[imvx] = colSurface;
	}
}

static void ColorizeSubSurfaces()
{
	const GFXColor colSubSurface = GetColor(_iSubSurface + _iSurface);
	for(INDEX imvx=0;imvx<_ctMeshVertices;imvx++) {
		_aMeshColors[imvx] = colSubSurface;
	}
}

static void ColorizeWeightsPerVertex()
{
	ASSERT(_pmlodCurrent!=NULL);
	ASSERT(_pmsrfCurrent!=NULL);
	const INDEX ctmvx = _pmsrfCurrent->msrf_ctVertices;
	const INDEX ctmlodvtx = _pmlodCurrent->mlod_aVertices.Count();
	const INDEX iFirstVertex = _pmsrfCurrent->msrf_iFirstVertex;
	_ctMapsPerVertex.PopAll();
	_ctMapsPerVertex.Push(ctmlodvtx);
	memset(&_ctMapsPerVertex[0],0,sizeof(INDEX)*ctmlodvtx);


	const INDEX ctmwm = _pmlodCurrent->mlod_aWeightMaps.Count();
	for(INDEX imwm=0;imwm<ctmwm;imwm++) {
		const MeshWeightMap &mwm = _pmlodCurrent->mlod_aWeightMaps[imwm];
		const INDEX ctmvw = mwm.mwm_aVertexWeight.Count();
		for(INDEX imvw=0;imvw<ctmvw;imvw++) {
			const MeshVertexWeight &mvw = mwm.mwm_aVertexWeight[imvw];
			_ctMapsPerVertex[mvw.mww_iVertex]++;
		}
	}

	for(INDEX imvx=0;imvx<ctmvx;imvx++) {
		const INDEX ctwpv = _ctMapsPerVertex[imvx+iFirstVertex];
		_aMeshColors[imvx+iFirstVertex] = GetColor(ctwpv);
	}
}

static void ColorizeMorphsPerVertex()
{
	ASSERT(_pmlodCurrent!=NULL);
	ASSERT(_pmsrfCurrent!=NULL);
	const INDEX ctmvx = _pmsrfCurrent->msrf_ctVertices;
	const INDEX ctmlodvtx = _pmlodCurrent->mlod_aVertices.Count();
	const INDEX iFirstVertex = _pmsrfCurrent->msrf_iFirstVertex;
	_ctMapsPerVertex.PopAll();
	_ctMapsPerVertex.Push(ctmlodvtx);
	memset(&_ctMapsPerVertex[0],0,sizeof(INDEX)*ctmlodvtx);

	const INDEX ctmmm = _pmlodCurrent->mlod_aMorphMaps.Count();
	for(INDEX immm=0;immm<ctmmm;immm++) {
		const MeshMorphMap &mmm = _pmlodCurrent->mlod_aMorphMaps[immm];
		const INDEX ctmvm = mmm.mmp_aMorphMap.Count();
		for(INDEX imvm=0;imvm<ctmvm;imvm++) {
			const MeshVertexMorph &mvm = mmm.mmp_aMorphMap[imvm];
			_ctMapsPerVertex[mvm.mwm_iVxIndex]++;
		}
	}

	for(INDEX imvx=0;imvx<ctmvx;imvx++) {
		const INDEX ctwpv = _ctMapsPerVertex[imvx+iFirstVertex];
		_aMeshColors[imvx+iFirstVertex] = GetColor(ctwpv);
	}
}

static void ShowStrips(const MeshSurface &msrf)
{
	ASSERT(_pmlodCurrent!=NULL);
	ASSERT(_pmsrfCurrent!=NULL);
	const INDEX ctmvx = msrf.msrf_ctVertices;
	const INDEX ctind = msrf.msrf_auwTriangles.Count()*3;
	const INDEX ctvtxStrips = ctind;
	const INDEX ctindStrips = ctind;
	const INDEX iFirstVertex = _pmsrfCurrent->msrf_iFirstVertex;
	MeshVertex *pavVertices = &_pavFinalVertices[msrf.msrf_iFirstVertex];
	const UWORD *pauwIndices = &msrf.msrf_auwTriangles[0].mt_uwVertexIndex[0];

	ASSERT(ctmvx>0);
	ASSERT(ctind>2);


	_aStripVertices.PopAll();
	_aStripIndices.PopAll();
	_aMeshColors.PopAll();
	_aStripVertices.Push(ctvtxStrips);
	_aStripIndices.Push(ctindStrips);
	_aMeshColors.Push(ctvtxStrips);

	INDEX iVertex = 0;
	INDEX iIndex = 0;
	INDEX iColorIndex=1;
	for(INDEX iind=0;iind<ctind;iind++) {
		const UWORD uwIndex = pauwIndices[iind];
		_aStripVertices[iVertex] = pavVertices[uwIndex];
		_aStripIndices[iIndex] = iVertex;
		iVertex++;
		iIndex++;
	}

	INDEX iaLastInd[3] = {-1, -2, -3};

	GFXColor col = GetColor(iColorIndex);
	const INDEX ctts = ctind/3;
	for(INDEX its=0;its<ctts;its++) {
		INDEX iFirstIndex = its*3;
		INDEX ctSameVertices = 0;
		for(INDEX i=0;i<3;i++) {
			for(INDEX j=0;j<3;j++) {
				if(pauwIndices[iFirstIndex+i] == iaLastInd[j]) {
					ctSameVertices++;
				}
			}
		}
		ASSERT(ctSameVertices<3);
		// if last two vertices are not same
		if(ctSameVertices<2) {
			// get new tri color
			col = GetColor(++iColorIndex);
		}
		_aMeshColors[iFirstIndex+0] = col;
		_aMeshColors[iFirstIndex+1] = col;
		_aMeshColors[iFirstIndex+2] = col;
		iaLastInd[0] = pauwIndices[iFirstIndex+0];
		iaLastInd[1] = pauwIndices[iFirstIndex+1];
		iaLastInd[2] = pauwIndices[iFirstIndex+2];
	}

	shaSetVertexArray((GFXVertex*)&_aStripVertices[0],0,ctvtxStrips);
	shaSetIndices(&_aStripIndices[0],ctindStrips);
	shaSetColorArray(&_aMeshColors[0]);
	shaRender();
}

static void ShowWireframe(void)
{
	const GFXColor colSubSurface = 0xFFFFFFFF;
	for(INDEX imvx=0;imvx<_ctMeshVertices;imvx++) {
		_aMeshColors[imvx] = colSubSurface;
	}
}

static void ShowNormals(const MeshSurface &msrf)
{
	const INDEX ctVertices = msrf.msrf_ctVertices;
	const INDEX iFirstVertex = msrf.msrf_iFirstVertex;

	for(INDEX ivx=0;ivx<ctVertices;ivx++) {
		const INDEX ivxAbs = ivx + iFirstVertex;
		FLOAT3D vNormal = FLOAT3D(_panFinalNormals[ivxAbs].nx,_panFinalNormals[ivxAbs].ny,_panFinalNormals[ivxAbs].nz);
		// vNormal.Normalize();
		FLOAT3D vVtx1 = FLOAT3D(_pavFinalVertices[ivxAbs].x,_pavFinalVertices[ivxAbs].y,_pavFinalVertices[ivxAbs].z);
		FLOAT3D vVtx2 = vVtx1 + (vNormal/5);
		_pdp->DrawLine3D(vVtx1,vVtx2,0xFFFFFFFF);
	}
}

static void ColorizeSelectedWeight(void)
{
	const GFXColor colSelected = 0xFF800000;

	ASSERT(_pmlodCurrent!=NULL);
	ASSERT(_pmsrfCurrent!=NULL);
	const INDEX ctmvx = _pmsrfCurrent->msrf_ctVertices;
	const INDEX ctmlodvtx = _pmlodCurrent->mlod_aVertices.Count();
	const INDEX iFirstVertex = _pmsrfCurrent->msrf_iFirstVertex;

	// for each weight map
	const INDEX ctmwm = _pmlodCurrent->mlod_aWeightMaps.Count();
	for(INDEX imwm=0;imwm<ctmwm;imwm++) {
		const MeshWeightMap &mwm = _pmlodCurrent->mlod_aWeightMaps[imwm];
		// for each vertex in weight map
		const INDEX ctmvw = mwm.mwm_aVertexWeight.Count();
		for(INDEX imvw=0;imvw<ctmvw;imvw++) {
			const MeshVertexWeight &mvw = mwm.mwm_aVertexWeight[imvw];
			if(mwm.mwm_iID == _iSelectionID) {
				GFXColor colVertex = colSelected;
				colVertex.a = NormFloatToByte(mvw.mww_fWeight);
				_aMeshColors[mvw.mww_iVertex] = colVertex;
			}
		}
	}
}

static void ColorizeSelectedMorph(void)
{
	const GFXColor colSelected = 0x0080FFFF;

	ASSERT(_pmlodCurrent!=NULL);
	ASSERT(_pmsrfCurrent!=NULL);
	const INDEX ctmvx = _pmsrfCurrent->msrf_ctVertices;
	const INDEX ctmlodvtx = _pmlodCurrent->mlod_aVertices.Count();
	const INDEX iFirstVertex = _pmsrfCurrent->msrf_iFirstVertex;

	// for each morph map
	const INDEX ctmmm = _pmlodCurrent->mlod_aMorphMaps.Count();
	for(INDEX immm=0;immm<ctmmm;immm++) {
		const MeshMorphMap &mmm = _pmlodCurrent->mlod_aMorphMaps[immm];
		// for each vertex in morph map
		const INDEX ctmvm = mmm.mmp_aMorphMap.Count();
		for(INDEX imvm=0;imvm<ctmvm;imvm++) {
			const MeshVertexMorph &mvm = mmm.mmp_aMorphMap[imvm];
			if(mmm.mmp_iID == _iSelectionID) {
				GFXColor colVertex = colSelected;
				_aMeshColors[mvm.mwm_iVxIndex] = colVertex;
			}
		}
	}
}

static void ColorizeSelectedSubSurface(void)
{
	ASSERT(_pmsrfCurrent!=NULL);
	const GFXColor colSelected = 0x00FF0080;
	const INDEX ctmvx = _pmsrfCurrent->msrf_ctVertices;
	const INDEX iFirstVertex = _pmsrfCurrent->msrf_iFirstVertex;

	if(_pmsrfCurrent->msrf_iSurfaceID==_iSelectionID) {
		for(INDEX imvx=0;imvx<ctmvx;imvx++) {
			_aMeshColors[imvx + iFirstVertex] = colSelected;
		}
	}
}

static void ColorizeSelectedSurface(void)
{
	ASSERT(_pmsrfCurrent!=NULL);
	const GFXColor colSelected = 0x00FF0080;
	const INDEX ctmvx = _pmsrfCurrent->msrf_ctVertices;
	const INDEX iFirstVertex = _pmsrfCurrent->msrf_iFirstVertex;

	BOOL bSelectedSurface = FALSE;
	if(_pmsrfCurrent->msrf_ulFlags&MS_MESH_SUBSURFACE) {
		CTString strSurface = ska_IDToString(_pmsrfCurrent->msrf_iSurfaceID);
		const char *pstrSurf =  (const char*)strSurface;
		char *pstrUnderline = strrchr(const_cast<char *>(pstrSurf),'_');
		if(pstrUnderline!=NULL) {
			INDEX iLength = pstrUnderline-pstrSurf;
			char *pstrSurfName = new char[iLength+1];
			memcpy(pstrSurfName,pstrSurf,iLength);
			pstrSurfName[iLength] = 0;
			// if this string exists in table (dont create new one with ska_StringToID)
			if(ska_FindID(pstrSurfName)>=0) {
				INDEX iSubSurfaceID = ska_StringToID(pstrSurfName);
				if(iSubSurfaceID==_iSelectionID) {
					bSelectedSurface = TRUE;
				}
			}
			delete [] pstrSurfName;
		}
	}
	
	if(_pmsrfCurrent->msrf_iSurfaceID==_iSelectionID) {
		bSelectedSurface = TRUE;
	}

	if(bSelectedSurface) {
		for(INDEX imvx=0;imvx<ctmvx;imvx++) {
			_aMeshColors[imvx + iFirstVertex] = colSelected;
		}
	}
}

static void RenderMesh_Preview(RenMesh &rmsh, RenModel &rm)
{
	SetViewMatrix( rmsh.rmsh_mStrObjToView);
	// get current lod and surfaces count
	MeshLOD &mlod  = rmsh.rmsh_pMeshInst->mi_pMesh->msh_aMeshLODs[rmsh.rmsh_iMeshLODIndex];
	
	const INDEX ctmsrf = mlod.mlod_aSurfaces.Count();
	for(INDEX imsrf=0;imsrf<ctmsrf;imsrf++) {
		MeshSurface &msrf = mlod.mlod_aSurfaces[imsrf];

		if(msrf.msrf_ulFlags&MS_MESH_SUBSURFACE) {
			_iSubSurface++;
		} else {
			_iSurface++;
		}
		_pmsrfCurrent = &msrf;

		const BOOL bDynamicSurface = msrf.msrf_ulFlags&MS_DYNAMIC_SURFACE;
		if(bDynamicSurface && _bMeshUsesMorphs) {
			ASSERT(_pavMorphedVertices!=NULL && _panMorphedNormals!=NULL);
			_pavFinalVertices = _pavMorphedVertices;
			_panFinalNormals = _panMorphedNormals;
		} else {
			ASSERT(_pavWeightedVertices!=NULL && _panWeightedNormals!=NULL);
			_pavFinalVertices = _pavWeightedVertices;
			_panFinalNormals = _panWeightedNormals;
		}

		BOOL bRenderMesh = TRUE;
		if(_psf==SPF_NORMALS || _psf==SPF_STRIPS) {
			bRenderMesh = FALSE;
		}

		if(bRenderMesh) {
			// begin model rendering
			shaSetVertexArray( (GFXVertex*)&_pavFinalVertices[msrf.msrf_iFirstVertex], 0, msrf.msrf_ctVertices);
			shaSetNormalArray( (GFXNormal*)&_panFinalNormals[msrf.msrf_iFirstVertex]);
			shaSetIndices( &msrf.msrf_auwTriangles[0].mt_uwVertexIndex[0], msrf.msrf_auwTriangles.Count()*3);
		}
		shaSetTexture(-1);
//안태훈 수정 시작	//(Add Tagent-space Normal Map)(0.1)
		//부분 선택시 초록색 반투명으로 나올 경우 처리관련.
		shaEnableDepthTest();
		shaEnableDepthWrite();
		//shaDisableDepthTest();
		//shaDisableDepthWrite();
//안태훈 수정 끝	//(Add Tagent-space Normal Map)(0.1)
		if(_iSelectionID>=0) {
			shaBlendFunc(GFX_SRC_ALPHA, GFX_INV_SRC_ALPHA);
			shaEnableBlend();
			// gfxEnableDepthBias(); // hardware and software dont match in depth buffer
		} else {
			shaDisableBlend();
		}

		if(_psf==SPF_WIREFRAME) {
			gfxPolygonMode(GFX_LINE);
			gfxEnableDepthBias();
			gfxDisableTexture();
		}

		gfxCullFace(GFX_NONE);
		_aMeshColors.PopAll();
		_aMeshColors.Push(_ctMeshVertices);
		memset(&_aMeshColors[0],0,_ctMeshVertices*sizeof(GFXColor));

		if(_iSelectionID>=0) {
			// Colorize part of mesh
			switch(_psf) {
				/*
				case SPF_MODELS:      ColorizeSelectedModel(); break;
				case SPF_MESH_LOD:    ColorizeSelectedMeshe(); break;
				*/
				case SPF_SURFACES:    ColorizeSelectedSurface(); break;
				case SPF_SUBSURFACES: ColorizeSelectedSubSurface(); break;
				case SPF_WEIGHTS:     ColorizeSelectedWeight(); break;
				case SPF_MORPHS:      ColorizeSelectedMorph(); break;
			}
		} else {
			// Colorize mesh
			switch(_psf) {
				case SPF_MODELS:             ColorizeModels(); break;
				case SPF_MESH_LOD:           ColorizeMeshes(); break;
				case SPF_SURFACES:           ColorizeSurfaces(); break;
				case SPF_SUBSURFACES:        ColorizeSubSurfaces(); break;
				case SPF_WEIGHTS_PER_VERTEX: ColorizeWeightsPerVertex(); break;
				case SPF_MORPHS_PER_VERTEX:  ColorizeMorphsPerVertex(); break;
				case SPF_STRIPS:             ShowStrips(msrf); break;
				case SPF_WIREFRAME:          ShowWireframe(); break;
				case SPF_NORMALS:            ShowNormals(msrf); break;
			}
		}


		if(bRenderMesh) {
			// Render mesh
			shaSetColorArray(&_aMeshColors[msrf.msrf_iFirstVertex]);
			shaRender();
		}

		if(_psf==SPF_WIREFRAME) {
			gfxPolygonMode(GFX_FILL);
		}

		gfxDisableDepthBias();
		shaClean();
	}
}

static void RenderModel_Preview(RenModel &rm)
{
	const INDEX ctrmsh = rm.rm_ctMeshes;
	for(INDEX irmsh=0;irmsh<ctrmsh;irmsh++) {
		RenMesh &rmsh = _aRenMeshes[irmsh + rm.rm_iFirstMesh];
		_pmlodCurrent = &rmsh.rmsh_pMeshInst->mi_pMesh->msh_aMeshLODs[rmsh.rmsh_iMeshLODIndex];
		_iMesh++;

		// Get mesh lod
		const MeshLOD &mlod = rmsh.rmsh_pMeshInst->mi_pMesh->msh_aMeshLODs[rmsh.rmsh_iMeshLODIndex];
		// Lock buffer arrays
		LockMeshReadBuffers(mlod);
		// prepare mesh for rendering
		PrepareMeshForRendering(rmsh,rm.rm_iSkeletonLODIndex);
		// render mesh
		RenderMesh_Preview(rmsh,rm);
		// Unlock buffer arrays
		UnlockMeshReadBuffers(mlod);
	}
}

extern void RM_PreviewSKA(CModelInstance &mi, CDrawPort *pdp, SkaPreviewFlags spf, INDEX iSelectionID/*=-1*/)
{
	_pdp = pdp;
	CObjectRestore<BOOL> or(_bUseHardwareShaders,FALSE);

	_psf = spf;
	_iSelectionID = iSelectionID;
	
	CalculateRenderingData(mi, FALSE);
	const INDEX ctrm = _aRenModels.Count();
	for(INDEX irm=1;irm<ctrm;irm++) {
		RenModel &rm = _aRenModels[irm];
		_pmiCurrent = rm.rm_pmiModel;
		_iModel++;
		RM_SetObjectMatrices(*rm.rm_pmiModel);
		RenderModel_Preview(rm);
	}

	ClearRenArrays();
	CleanPreview();
}
