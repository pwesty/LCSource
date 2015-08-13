#include "stdh.h"

#define MESH_VERSION  16
//안태훈 수정 시작
//이 버전의 메쉬는 Tangent 정보와 SkaTag 정보를 갖고 있다.
#define MESH_NEW_VER  17
//안태훈 수정 끝
#define MESH_ID       "MESH"

#include <Engine/Ska/StringTable.h>
#include <Engine/Ska/Mesh.h>
#include <Engine/Base/Stream.h>
#include <Engine/Base/Console.h>
#include <Engine/Math/Projection.h>
#include <Engine/Graphics/DrawPort.h>
#include <Engine/Graphics/Shader.h>
#include <Engine/Templates/StaticArray.h>
#include <Engine/Templates/StaticArray.cpp>
#include <Engine/Templates/Stock_CShader.h>

#ifdef KALYDO
#include <Engine/Base/Console.h>
#include <Kalydo/KRFReadLib/Include/KRFReadLib.h>
CTString CMesh::strDefaultMeshPath = "data\\Defaults\\Axis.bm";
#endif

static INDEX AreVerticesDiferent(INDEX iCurrentIndex, INDEX iLastIndex);
static void FillVertexWeightInfos(MeshLOD &mlod);

//안태훈 수정 시작	//(Add Tagent-space Normal Map)(0.1)
static void CalcVertexTangent(MeshLOD &mlod, int texCoordForTangent);
static void FillWeightMapInfo(MeshLOD &mlod);
//안태훈 수정 끝	//(Add Tagent-space Normal Map)(0.1)

static void UnbindMeshLOD(MeshLOD &mlod)
{
	// Delete vertex buffers if buffer exists
	if(mlod.mlod_iVertexBufferID>=0) 
	{
		gfxDeleteVertexBuffer(mlod.mlod_iBufferBindID);
	}
	// Unbind buffer IDs
	mlod.mlod_iBufferBindID   = -1;
	mlod.mlod_iVertexBufferID = -1;
	mlod.mlod_iNormalBufferID = -1;
//안태훈 수정 시작	//(Add Tagent-space Normal Map)(0.1)
	mlod.mlod_iTangentBufferID = -1;
//안태훈 수정 끝	//(Add Tagent-space Normal Map)(0.1)
	mlod.mlod_iWeightBufferID = -1;
	
	// for each uvmap
	const INDEX ctmuvm = mlod.mlod_aUVMaps.Count();
	for(INDEX imuvm=0;imuvm<ctmuvm;imuvm++) 
	{
		// Unbind texcoord buffer id
		MeshUVMap &muvm = mlod.mlod_aUVMaps[imuvm];
		muvm.muvm_iTexCoordBufferID = -1;
	}
}

// Clear mesh lod
static void ClearMeshLOD(MeshLOD &mlod)
{
	// First unbind mesh lod
	UnbindMeshLOD(mlod);
	
	INDEX ctsrf = mlod.mlod_aSurfaces.Count();
	for (INDEX isrf=0;isrf<ctsrf;isrf++)
	{
		MeshSurface &msrf = mlod.mlod_aSurfaces[isrf];
		msrf.msrf_auwTriangles.Clear();
		// release shader form stock
		if(msrf.msrf_pShader!=NULL) 
		{
			_pShaderStock->Release(msrf.msrf_pShader);
		}
		msrf.msrf_pShader = NULL;
	}
	// clear the surfaces array
	mlod.mlod_aSurfaces.Clear();
	// for each uvmap, clear the texcord list
	INDEX ctmuvm = mlod.mlod_aUVMaps.Count();
	for (INDEX imuvm=0;imuvm<ctmuvm;imuvm++) 
	{
		MeshUVMap &muvm = mlod.mlod_aUVMaps[imuvm];
		muvm.muv_aTexCoords.Clear();
		muvm.muvm_iTexCoordBufferID = -1;
	}
	// clear the uvmaps array
	mlod.mlod_aUVMaps.Clear();
	// clear the vertices array
	mlod.mlod_aVertices.Clear();
	// clear the normals array
	mlod.mlod_aNormals.Clear();
//안태훈 수정 시작	//(Add Tagent-space Normal Map)(0.1)
	mlod.mlod_aTangents.Clear();
//안태훈 수정 끝	//(Add Tagent-space Normal Map)(0.1)
//안태훈 수정 시작	//(Add & Modify SSSE Effect)(0.1)
	mlod.mlod_aSkaTags.Clear();
//안태훈 수정 끝	//(Add & Modify SSSE Effect)(0.1)
}

// MeshLOD Constuctor / Destructor
MeshLOD::MeshLOD()
{
	mlod_fMaxDistance    = -1;
	mlod_ulFlags         =  0;
	mlod_iBufferBindID   = -1;
	mlod_iVertexBufferID = -1;
	mlod_iNormalBufferID = -1;
//안태훈 수정 시작	//(Add Tagent-space Normal Map)(0.1)
	mlod_iTangentBufferID = -1;
//안태훈 수정 끝	//(Add Tagent-space Normal Map)(0.1)
	mlod_iWeightBufferID = -1;
}

MeshLOD::~MeshLOD()
{
	ASSERT(this!=NULL);
	ClearMeshLOD(*this);
	// Each buffer must have been deleted
	ASSERT(mlod_iBufferBindID   == (-1));
	ASSERT(mlod_iVertexBufferID == (-1));
	ASSERT(mlod_iNormalBufferID == (-1));
//안태훈 수정 시작	//(Add Tagent-space Normal Map)(0.1)
	ASSERT(mlod_iTangentBufferID == (-1));
//안태훈 수정 끝	//(Add Tagent-space Normal Map)(0.1)
	ASSERT(mlod_iWeightBufferID == (-1));
	const INDEX ctmuvm = mlod_aUVMaps.Count();
	for(INDEX imuvm=0;imuvm<ctmuvm;imuvm++) 
	{
		const MeshUVMap &muvm = mlod_aUVMaps[imuvm];
		ASSERT(muvm.muvm_iTexCoordBufferID == (-1));
	}
}

void MeshLOD::operator=(const MeshLOD &mlodOther)
{
	const BOOL bHasContext = (_pGfx->gl_ctRealTextureUnits>0);
	const INDEX ctmuvm = mlodOther.mlod_aUVMaps.Count();
	
	// Copy mesh data
	mlod_ulFlags        = mlodOther.mlod_ulFlags;
	mlod_fMaxDistance   = mlodOther.mlod_fMaxDistance;
	mlod_fnSourceFile   = mlodOther.mlod_fnSourceFile;
	mlod_aVertices      = mlodOther.mlod_aVertices;
	mlod_aNormals       = mlodOther.mlod_aNormals;
//안태훈 수정 시작	//(Add Tagent-space Normal Map)(0.1)
	mlod_aTangents      = mlodOther.mlod_aTangents;
//안태훈 수정 끝	//(Add Tagent-space Normal Map)(0.1)
	mlod_aSurfaces      = mlodOther.mlod_aSurfaces;
	mlod_aWeightMaps    = mlodOther.mlod_aWeightMaps;
	mlod_aMorphMaps     = mlodOther.mlod_aMorphMaps;
	mlod_aVertexWeights = mlodOther.mlod_aVertexWeights;
//안태훈 수정 시작	//(Add & Modify SSSE Effect)(0.1)
	mlod_aSkaTags		= mlodOther.mlod_aSkaTags;
	//deep copy 해야함.
//안태훈 수정 끝	//(Add & Modify SSSE Effect)(0.1)
	
//안태훈 수정 시작	//(Add Tagent-space Normal Map)(0.1)
	BOOL bHasTangent = mlod_aTangents.Count()>0 ? TRUE : FALSE;
//안태훈 수정 끝	//(Add Tagent-space Normal Map)(0.1)

	if(ctmuvm>0) 
	{
		mlod_aUVMaps.New(ctmuvm);
	}
	
	for(INDEX imuvm=0;imuvm<ctmuvm;imuvm++) 
	{
		const MeshUVMap &muvmOther = mlodOther.mlod_aUVMaps[imuvm];
		MeshUVMap &muvm = mlod_aUVMaps[imuvm];
		muvm.muv_aTexCoords = muvmOther.muv_aTexCoords;
		muvm.muv_iID        = muvmOther.muv_iID;
	}
	
	// Copy vertex buffers
	if(bHasContext && mlodOther.mlod_iBufferBindID!=(-1)) 
	{
		INDEX ctUVMaps, imuvm;
		const INDEX ctmvx = gfxGetVertexBufferSize(mlodOther.mlod_iBufferBindID);
		const ULONG ulBufferFlags = gfxGetVertexBufferMask(mlodOther.mlod_iBufferBindID,ctUVMaps);
		const BOOL  bHasWeights = ulBufferFlags&GFX_VBM_WGH;
		ASSERT(ctmuvm==ctUVMaps);
		
		// create new buffers
		mlod_iBufferBindID = gfxCreateVertexBuffer(ctmvx,ulBufferFlags,ctmuvm,GFX_READWRITE);
		ASSERT(mlod_iBufferBindID>=0);
		// Get vertex buffer ids
		mlod_iVertexBufferID = gfxGetVertexSubBufferID(mlod_iBufferBindID,GFX_VBA_POS);
		mlod_iNormalBufferID = gfxGetVertexSubBufferID(mlod_iBufferBindID,GFX_VBA_NOR);
		ASSERT(mlod_iVertexBufferID>=0);
		ASSERT(mlod_iNormalBufferID>=0);
//안태훈 수정 시작	//(Add Tagent-space Normal Map)(0.1)
		if(bHasTangent)
		{
			mlod_iTangentBufferID = gfxGetVertexSubBufferID(mlod_iBufferBindID, GFX_VBA_TAN);
			ASSERT(mlod_iTangentBufferID>=0);
		}
//안태훈 수정 끝	//(Add Tagent-space Normal Map)(0.1)
		// Get weight buffer id if weight buffer exists
		if(bHasWeights) 
		{
			mlod_iWeightBufferID = gfxGetVertexSubBufferID(mlod_iBufferBindID,GFX_VBA_WGH);
			ASSERT(mlod_iWeightBufferID>=0);
		}
		for( imuvm = 0; imuvm < ctmuvm; imuvm++ )
		{
			MeshUVMap &muvm = mlod_aUVMaps[imuvm];
			muvm.muvm_iTexCoordBufferID = gfxGetVertexSubBufferID(mlod_iBufferBindID,GFX_VBA_TEX+imuvm);
			ASSERT(muvm.muvm_iTexCoordBufferID!=(-1));
		}
		
		// Duplicate vertices
		GFXVertex *pavVerticesSrc = (GFXVertex*)gfxLockVertexSubBuffer(mlodOther.mlod_iVertexBufferID,0,ctmvx,GFX_READ);
		GFXVertex *pavVerticesDst = (GFXVertex*)gfxLockVertexSubBuffer(mlod_iVertexBufferID,0,ctmvx,GFX_WRITE);
		memcpy(pavVerticesDst,pavVerticesSrc,sizeof(GFXVertex)*ctmvx);
		gfxUnlockVertexSubBuffer(mlodOther.mlod_iVertexBufferID, 0);
		gfxUnlockVertexSubBuffer(mlod_iVertexBufferID, 0);
		
		// Duplicate normals
		GFXNormal *panNormalsSrc  = (GFXNormal*)gfxLockNormalSubBuffer(mlodOther.mlod_iNormalBufferID,0,ctmvx,GFX_READ);
		GFXNormal *panNormalsDst  = (GFXNormal*)gfxLockNormalSubBuffer(mlod_iNormalBufferID,0,ctmvx,GFX_WRITE);
		memcpy(panNormalsDst,panNormalsSrc,sizeof(GFXNormal)*ctmvx);
		gfxUnlockNormalSubBuffer(mlodOther.mlod_iNormalBufferID, 0);
		gfxUnlockNormalSubBuffer(mlod_iNormalBufferID, 0);
		
//안태훈 수정 시작	//(Add Tagent-space Normal Map)(0.1)
		if(bHasTangent)
		{
			GFXTangent *panNormalsSrc  = (GFXTangent*)gfxLockTangentSubBuffer(mlodOther.mlod_iTangentBufferID,0,ctmvx,GFX_READ);
			GFXTangent *panNormalsDst  = (GFXTangent*)gfxLockTangentSubBuffer(mlod_iTangentBufferID,0,ctmvx,GFX_WRITE);
			memcpy(panNormalsDst,panNormalsSrc,sizeof(GFXTangent)*ctmvx);
			gfxUnlockNormalSubBuffer(mlodOther.mlod_iTangentBufferID, 0);
			gfxUnlockNormalSubBuffer(mlod_iTangentBufferID, 0);
		}
//안태훈 수정 끝	//(Add Tagent-space Normal Map)(0.1)
		
		// If weights exists
		if(bHasWeights) 
		{
			// Duplicate weights
			GFXWeight *pawWeightsSrc = (GFXWeight*)gfxLockWeightSubBuffer(mlodOther.mlod_iWeightBufferID,0,ctmvx,GFX_READ);
			GFXWeight *pawWeightsDst = (GFXWeight*)gfxLockWeightSubBuffer(mlod_iWeightBufferID,0,ctmvx,GFX_WRITE);
			memcpy(pawWeightsDst,pawWeightsSrc,sizeof(GFXWeight)*ctmvx);
			gfxUnlockWeightSubBuffer(mlodOther.mlod_iWeightBufferID, 0);
			gfxUnlockWeightSubBuffer(mlod_iWeightBufferID, 0);
		}
		
		// Duplicate each uvmap texcoord buffer
		for( imuvm = 0; imuvm < ctmuvm; imuvm++)
		{
			const MeshUVMap &muvm = mlod_aUVMaps[imuvm];
			const MeshUVMap &muvmOther = mlodOther.mlod_aUVMaps[imuvm];
			
			GFXTexCoord *patcTexCoordsSrc = (GFXTexCoord*)gfxLockTexCoordSubBuffer(muvmOther.muvm_iTexCoordBufferID,0,ctmvx,GFX_READ);
			GFXTexCoord *patcTexCoordsDst = (GFXTexCoord*)gfxLockTexCoordSubBuffer(muvm.muvm_iTexCoordBufferID,0,ctmvx,GFX_WRITE);
			memcpy(patcTexCoordsDst,patcTexCoordsSrc,sizeof(GFXTexCoord)*ctmvx);
			gfxUnlockTexCoordSubBuffer(muvmOther.muvm_iTexCoordBufferID, 0);
			gfxUnlockTexCoordSubBuffer(muvm.muvm_iTexCoordBufferID, 0);
		}
	}
}

MeshSurface::MeshSurface()
{
	msrf_iFirstVertex = 0;
	msrf_ctVertices = 0;
	msrf_iSurfaceID = -1;
	msrf_pShader = NULL;
	msrf_ulFlags = 0;
}

MeshSurface::~MeshSurface()
{
	if(msrf_pShader!=NULL) 
	{
		_pShaderStock->Release(msrf_pShader);
	}
	msrf_pShader = NULL;
}

void MeshSurface::operator=(const MeshSurface &msrfOther)
{
	msrf_iFirstVertex     = msrfOther.msrf_iFirstVertex;
	msrf_ctVertices       = msrfOther.msrf_ctVertices;
	msrf_iSurfaceID       = msrfOther.msrf_iSurfaceID;
	msrf_ShadingParams    = msrfOther.msrf_ShadingParams;
	msrf_ulFlags          = msrfOther.msrf_ulFlags;
	msrf_aubRelIndexTable = msrfOther.msrf_aubRelIndexTable;
	msrf_auwTriangles     = msrfOther.msrf_auwTriangles;
	
	// Obtain copy of shader
	if(msrfOther.msrf_pShader!=NULL) 
	{
		const CTFileName &fnShader = msrfOther.msrf_pShader->ser_FileName;
		ASSERT(fnShader!="");
		try 
		{
			msrf_pShader = _pShaderStock->Obtain_t(fnShader);
		}
		catch(char *) 
		{
			msrf_pShader = NULL;
		}
	}
	else 
	{
		msrf_pShader = NULL;
	}
}



// struct for vertex location in some map
// (ex. vl_iIndex is index of MeshVertexWeight and vl_iSubIndex is index of MeshVertexWeight)
struct VertexLocator
{
	INDEX vl_iIndex;
	INDEX vl_iSubIndex;
};

// struct for sorting mesh vertices
struct SortArray
{
	INDEX sa_iNewIndex;     // index of vertex in sorted array
	INDEX sa_iSurfaceIndex; // index of surface this vertex belongs to
	CStaticArray<struct VertexLocator> sa_aWeightMapList; // list of weightmaps this vertex belongs to
	CStaticArray<struct VertexLocator> sa_aMorphMapList;  // list of morphmaps this vertex belongs to
};

// struct for sorting veritces per weight count in vertex
struct SortVertexWeight
{
	UBYTE svw_aubIndices[MAX_BPV]; // array of absolute weight map indices in mesh lod
	FLOAT svw_afWeights[MAX_BPV];  // array of weight factors
	INDEX svw_ctWeights;           // weights count
};

CStaticArray<INDEX> _aiSortedIndex;
CStaticArray<INDEX> _aiOptimizedIndex;
CStaticArray<struct SortArray> _aSortArray;   // array of sorted vertices
CStaticArray<struct SortVertexWeight> _avwSortWeights;

MeshLOD *pMeshLOD = NULL;// curent mesh lod (for quick sort)
MeshLOD _mshOptimized;


CMesh::CMesh()
{
}


CMesh::~CMesh()
{
	Clear();
}


// release old shader and obtain new shader for mesh surface (expand ShaderParams if needed)
extern void ChangeSurfaceShader_t(MeshSurface &msrf,CTString fnNewShader)
{
	CShader *pShaderNew = _pShaderStock->Obtain_t(fnNewShader);
	ASSERT(pShaderNew!=NULL);
	if(msrf.msrf_pShader!=NULL) 
	{
		_pShaderStock->Release(msrf.msrf_pShader);
	}
	msrf.msrf_pShader = pShaderNew;
	// get new shader description
//안태훈 수정 시작	//(For Performance)(0.1)
	ShaderDesc *pshDesc;
	msrf.msrf_pShader->GetShaderDesc(pshDesc);
	ShaderDesc &shDesc = *pshDesc;
//안태훈 수정 끝	//(For Performance)(0.1)
	// if needed expand size of arrays for new shader
	// reset new values!!!!
	INDEX ctOldTextureIDs = msrf.msrf_ShadingParams.sp_aiTextureIDs.Count();
	INDEX ctNewTextureIDs = shDesc.sd_astrTextureNames.Count();
	INDEX ctOldUVMaps     = msrf.msrf_ShadingParams.sp_aiTexCoordsIndex.Count();
	INDEX ctNewUVMaps     = shDesc.sd_astrTexCoordNames.Count();
	INDEX ctOldColors     = msrf.msrf_ShadingParams.sp_acolColors.Count();
	INDEX ctNewColors     = shDesc.sd_astrColorNames.Count();
	INDEX ctOldFloats     = msrf.msrf_ShadingParams.sp_afFloats.Count();
	INDEX ctNewFloats     = shDesc.sd_astrFloatNames.Count();
#pragma message(">> copy vertex and pixel program pointers")
	
	if(ctOldTextureIDs<ctNewTextureIDs) 
	{
		// expand texture IDs array
		msrf.msrf_ShadingParams.sp_aiTextureIDs.Expand(ctNewTextureIDs);
		// set new texture IDs to 0
		for(INDEX itx=ctOldTextureIDs;itx<ctNewTextureIDs;itx++) 
		{
			msrf.msrf_ShadingParams.sp_aiTextureIDs[itx] = -1;
		}
	}
	// expand array of uvmaps if needed
	if(ctOldUVMaps<ctNewUVMaps) 
	{
		// expand uvmaps IDs array
		msrf.msrf_ShadingParams.sp_aiTexCoordsIndex.Expand(ctNewUVMaps);
		// set new uvmaps indices to 0
		for(INDEX itxc=ctOldUVMaps;itxc<ctNewUVMaps;itxc++) 
		{
			msrf.msrf_ShadingParams.sp_aiTexCoordsIndex[itxc] = 0;
		}
	}
	// expand array of colors if needed
	if(ctOldColors<ctNewColors) 
	{
		// expand color array
		msrf.msrf_ShadingParams.sp_acolColors.Expand(ctNewColors);
		// set new colors indices white
		for(INDEX icol=ctOldUVMaps;icol<ctNewColors;icol++) 
		{
			msrf.msrf_ShadingParams.sp_acolColors[icol] = 0xFFFFFFFF;
		}
	}
	// expand array of floats if needed
	if(ctOldFloats<ctNewFloats) 
	{
		// expand float array
		msrf.msrf_ShadingParams.sp_afFloats.Expand(ctNewFloats);
		// set new floats to 0
		for(INDEX ifl=ctOldFloats;ifl<ctNewFloats;ifl++) 
		{
			msrf.msrf_ShadingParams.sp_afFloats[ifl] = 1;
		}
	}
}

// Remap triangle indices base on given remap table
extern void RemapMeshIndices(MeshLOD &mlod, const INDEX *paiRemapTable, const INDEX ctVertices)
{
	ASSERT(FALSE);
	ASSERT(paiRemapTable!=NULL);
	ASSERT(mlod.mlod_aVertices.Count() == mlod.mlod_aNormals.Count() == ctVertices);
	
	// for each surface in mesh lod
	INDEX ctsrf = mlod.mlod_aSurfaces.Count();
	for(INDEX isrf=0;isrf<ctsrf;isrf++) 
	{
		MeshSurface &msrf = mlod.mlod_aSurfaces[isrf];
		UWORD *pauwIndices = &msrf.msrf_auwTriangles[0].mt_uwVertexIndex[0];
		
		// for each triangle index in mesh surface
		INDEX ctind = msrf.msrf_auwTriangles.Count() * 3;
		for(INDEX iind=0;iind<ctind;iind++) 
		{
			// remap triangle index
			UWORD &uwIndex = pauwIndices[iind];
			ASSERT(uwIndex<ctVertices);
			uwIndex = paiRemapTable[uwIndex];
		}
	}
}

// Remap vertex order based on given remap table
extern void RemapMeshVertices(MeshLOD &mlod, const INDEX *paiRemapTable, const INDEX ctVertices)
{
	ASSERT(paiRemapTable!=NULL);
	ASSERT((mlod.mlod_aVertices.Count()==ctVertices) && (mlod.mlod_aNormals.Count()==ctVertices));
	
	CStaticArray<MeshVertex> avxRemapedVertices;
	CStaticArray<MeshNormal> anxRemapedNormals;
	CStaticArray<MeshTexCoord> atxRemapedTexCoords;
	CStaticArray<MeshVertexWeightInfo> avwiRemapedVertexWeights;
	
	const INDEX ctvwi = mlod.mlod_aVertexWeights.Count();
	
	avxRemapedVertices.New(ctVertices);
	anxRemapedNormals.New(ctVertices);
	atxRemapedTexCoords.New(ctVertices);
	if(ctvwi>0) 
	{
		avwiRemapedVertexWeights.New(ctvwi);
	}
	
//안태훈 수정 시작	//(Add Tagent-space Normal Map)(0.1)
	//Tangent 관련 처리
	CStaticArray<MeshTangent> anxRemapedTangents;
	const BOOL bHasTangent = (mlod.mlod_aTangents.Count() > 0);
	if(bHasTangent)
	{
		ASSERT(mlod.mlod_aTangents.Count() == mlod.mlod_aVertices.Count());
		anxRemapedTangents.New(mlod.mlod_aVertices.Count());
	}
//안태훈 수정 끝	//(Add Tagent-space Normal Map)(0.1)

	// remap vertices and normals
	INDEX	iVtxOld;
	for( iVtxOld = 0; iVtxOld < ctVertices; iVtxOld++ )
	{
		INDEX iVtxNew = paiRemapTable[iVtxOld];
		avxRemapedVertices[iVtxNew] = mlod.mlod_aVertices[iVtxOld];
		anxRemapedNormals[iVtxNew] = mlod.mlod_aNormals[iVtxOld];
//안태훈 수정 시작	//(Add Tagent-space Normal Map)(0.1)
		if(bHasTangent) anxRemapedTangents[iVtxNew] = mlod.mlod_aTangents[iVtxOld];
//안태훈 수정 끝	//(Add Tagent-space Normal Map)(0.1)
	}
	
	for( iVtxOld = 0; iVtxOld < ctvwi; iVtxOld++ )
	{
		INDEX iVtxNew = paiRemapTable[iVtxOld];
		avwiRemapedVertexWeights[iVtxNew] = mlod.mlod_aVertexWeights[iVtxOld];
	}
	
	// copy remaped vertices and normals back to mesh
	mlod.mlod_aVertices.CopyArray(avxRemapedVertices);
	mlod.mlod_aNormals.CopyArray(anxRemapedNormals);
//안태훈 수정 시작	//(Add Tagent-space Normal Map)(0.1)
	if(bHasTangent) mlod.mlod_aTangents.CopyArray(anxRemapedTangents);
//안태훈 수정 끝	//(Add Tagent-space Normal Map)(0.1)
	mlod.mlod_aVertexWeights.CopyArray(avwiRemapedVertexWeights);
	
	// for each uvmap in mesh lod
	INDEX ctuvm = mlod.mlod_aUVMaps.Count();
	for(INDEX imuvm=0;imuvm<ctuvm;imuvm++) 
	{
		// remap uvmap texcoords
		MeshUVMap &muvm = mlod.mlod_aUVMaps[imuvm];
		for(INDEX iVtxOld=0;iVtxOld<ctVertices;iVtxOld++) 
		{
			INDEX iVtxNew = paiRemapTable[iVtxOld];
			atxRemapedTexCoords[iVtxNew] = muvm.muv_aTexCoords[iVtxOld];
		}
		// copy remaped uvmap back to mesh
		muvm.muv_aTexCoords.CopyArray(atxRemapedTexCoords);
	}
	
	// for each weightmap
	INDEX ctmwm = mlod.mlod_aWeightMaps.Count();
	for(INDEX imwm=0;imwm<ctmwm;imwm++) 
	{
		MeshWeightMap &mwm = mlod.mlod_aWeightMaps[imwm];
		// for each vertex weight in weight map
		INDEX ctmvw = mwm.mwm_aVertexWeight.Count();
		for(INDEX imvw=0;imvw<ctmvw;imvw++) 
		{
			MeshVertexWeight &mvw = mwm.mwm_aVertexWeight[imvw];
			ASSERT(mvw.mww_iVertex<ctVertices);
			// remap vertex veight index 
			mvw.mww_iVertex = paiRemapTable[mvw.mww_iVertex];
		}
	}
	
	// for each morphmap
	INDEX ctmmm = mlod.mlod_aMorphMaps.Count();
	for(INDEX immm=0;immm<ctmmm;immm++) 
	{
		MeshMorphMap &mmm = mlod.mlod_aMorphMaps[immm];
		// for each vertex morph in morph map
		INDEX ctmvm = mmm.mmp_aMorphMap.Count();
		for(INDEX imvm=0;imvm<ctmvm;imvm++) 
		{
			MeshVertexMorph &mvm = mmm.mmp_aMorphMap[imvm];
			ASSERT(mvm.mwm_iVxIndex<ctVertices);
			// remap vertex morph index 
			mvm.mwm_iVxIndex = paiRemapTable[mvm.mwm_iVxIndex];
		}
	}
	
	// clear temp arrays
	avxRemapedVertices.Clear();
	anxRemapedNormals.Clear();
//안태훈 수정 시작	//(Add Tagent-space Normal Map)(0.1)
	if(bHasTangent) anxRemapedTangents.Clear();
//안태훈 수정 끝	//(Add Tagent-space Normal Map)(0.1)
	atxRemapedTexCoords.Clear();
	avwiRemapedVertexWeights.Clear();
}

// quck sort func for comparing vertices
static int qsort_CompareArray(const void *pVx1, const void *pVx2)
{
	INDEX *n1 = ((INDEX*)pVx1);
	INDEX *n2 = ((INDEX*)pVx2);
	return AreVerticesDiferent(*n1,*n2);
}


// clear array of sort vertices
static void ClearSortArray(INDEX ctOldVertices)
{
	for(int iv=0;iv<ctOldVertices;iv++)
	{
		_aSortArray[iv].sa_aWeightMapList.Clear();
		_aSortArray[iv].sa_aMorphMapList.Clear();
	}
	_aiOptimizedIndex.Clear();
	_aiSortedIndex.Clear();
	_aSortArray.Clear();
	_avwSortWeights.Clear();
}

// optimize mesh
void CMesh::Optimize(void)
{
	// for each mesh lod in mesh
	INDEX ctmlod = msh_aMeshLODs.Count();
	for(INDEX imlod=0;imlod<ctmlod;imlod++) 
	{
		MeshLOD &mlod = msh_aMeshLODs[imlod];
		// optimize each lod in mesh
		OptimizeLod(mlod);
	}
}

// returns surface relative weight map index if it exists, or adds new one to surface msrf_aubRelIndexTable array
//단 한군데서 호출되는 함수임.
static INDEX GetRelativeWeightIndex(MeshLOD &mlod, MeshSurface &msrf, INDEX iAbsWeightIndex)
{
	// search all weight maps maped as relative indices in given surface
	const INDEX ctrmwm = msrf.msrf_aubRelIndexTable.Count();
	for(INDEX irmwm=0;irmwm<ctrmwm;irmwm++) 
	{
		const INDEX imwm = msrf.msrf_aubRelIndexTable[irmwm];
		if(imwm == iAbsWeightIndex) 
		{
			return irmwm;
		}
	}
	/*
	if(msrf.msrf_aubRelIndexTable.Count()>MAX_WEIGHTS_PER_SURFACE) 
	{
	CPrintF("%s - Surface '%s' uses more than %d weights supported by hardware\n",(const char*)mlod.mlod_fnSourceFile, (const char*)ska_GetStringFromTable(msrf.msrf_iSurfaceID),MAX_WEIGHTS_PER_SURFACE);
	}
	*/
	
	const INDEX ctItems = msrf.msrf_aubRelIndexTable.Count();
	msrf.msrf_aubRelIndexTable.Expand(ctItems+1);
	UBYTE &ubNew = msrf.msrf_aubRelIndexTable[ctItems];
	ubNew = (UBYTE)iAbsWeightIndex;
	return ctItems;
	
	ASSERTALWAYS("Invalid weightmap index");
	CPrintF("Surface %s searching for invalid weightmap index %d\n",(const char*)ska_GetStringFromTable(msrf.msrf_iSurfaceID),iAbsWeightIndex);
	return 0;
}

struct MeshSubSurface
{
	CStaticStackArray<MeshTriangle> mssrf_auwTriangles; // array of new triangles for this subsurface
	CStaticArray<INDEX> mssrf_aiWeightMaps; // table of used mesh weight maps
	INDEX mssrf_ctmwm;                      // count of valid weight map entries in table
	INDEX mssrf_iID;
	ULONG mssrf_ulFlags;
	CTString mssrf_fnShader;
	ShaderParams msrf_ShadingParams;
};

struct TempWeightMap
{
	INDEX mwm_iID; // name of weightmap
	CStaticStackArray<struct MeshVertexWeight> mwm_aVertexWeight; // array of weight map vertices
};

struct TempMorphMap
{
	INDEX mwm_iID;
	BOOL  mwm_bRelative;
	CStaticStackArray<struct MeshVertexMorph> mmp_aMorphMap; // array of vertex morphs
};

static BOOL FindVertexInWeightMap(const INDEX iVertex, const MeshLOD &mlod, INDEX *pmwm, INDEX *pmvw, const INDEX imwmFirst)
{
	const INDEX ctmwm=mlod.mlod_aWeightMaps.Count();
	for(INDEX imwm=imwmFirst;imwm<ctmwm;imwm++) 
	{
		const MeshWeightMap &mwm = mlod.mlod_aWeightMaps[imwm];
		const INDEX ctmvw = mwm.mwm_aVertexWeight.Count();
		for(INDEX imvw=0;imvw<ctmvw;imvw++) 
		{
			const MeshVertexWeight &mvw = mwm.mwm_aVertexWeight[imvw];
			if(mvw.mww_iVertex == iVertex) 
			{
				*pmwm = imwm;
				*pmvw = imvw;
				return TRUE;
			}
		}
	}
	return FALSE;
}

static BOOL FindVertexInMorphMap(const INDEX iVertex, const MeshLOD &mlod, INDEX *pmmm, INDEX *pmvm, const INDEX immmFirst)
{
	const INDEX ctmmm = mlod.mlod_aMorphMaps.Count();
	for(INDEX immm=immmFirst;immm<ctmmm;immm++) 
	{
		const MeshMorphMap &mmm = mlod.mlod_aMorphMaps[immm];
		const INDEX ctmvm = mmm.mmp_aMorphMap.Count();
		for(INDEX imvm=0;imvm<ctmvm;imvm++) 
		{
			const MeshVertexMorph &mvm = mmm.mmp_aMorphMap[imvm];
			if(mvm.mwm_iVxIndex == iVertex) 
			{
				*pmmm = immm;
				*pmvm = imvm;
				return TRUE;
			}
		}
	}
	return FALSE;
}

static INDEX FindMeshVertex(const MeshLOD &mlod, const INDEX iFindVertex, const INDEX iFirstVertex)
{
	// ASSERT(iFindVertex<=iFirstVertex);
	const MeshVertex &mvxSearched = mlod.mlod_aVertices[iFindVertex];
	const INDEX ctmvx = mlod.mlod_aVertices.Count();
	for(INDEX imvx=iFirstVertex;imvx<ctmvx;imvx++) 
	{
		const MeshVertex &mvx = mlod.mlod_aVertices[imvx];
		if(mvx.x == mvxSearched.x && mvx.y == mvxSearched.y && mvx.z == mvxSearched.z) 
		{
			return imvx;
		}
	}
	return -1; // vertex does not exists
}

static void ValidateWeights(MeshLOD &mlod)
{
	const INDEX ctmvx=mlod.mlod_aVertices.Count();
	for(INDEX imvx=0;imvx<ctmvx;imvx++) 
	{
		const MeshVertex &mvx = mlod.mlod_aVertices[imvx];
		const MeshVertexWeightInfo &mvwi = mlod.mlod_aVertexWeights[imvx];
		
		INDEX ivxFound = 0;
		INDEX ctRepeats = 0;
		while(TRUE) 
		{
			ivxFound = FindMeshVertex(mlod,imvx,ivxFound);
			if(ivxFound==(-1)) 
			{
				break;
			}
			else 
			{
				const MeshVertex &mvxFound = mlod.mlod_aVertices[ivxFound];
				const MeshVertexWeightInfo &mvwiFound = mlod.mlod_aVertexWeights[ivxFound];
				for(INDEX i=0;i<4;i++) 
				{
					// ASSERT(mvwiFound.mvwi_aubIndices[i] == mvwi.mvwi_aubIndices[i]);
					ASSERT(mvwiFound.mvwi_aubWeights[i] == mvwi.mvwi_aubWeights[i]);
				}
				ivxFound++;
				ctRepeats++;
			}
		}
		// CPrintF("Vertex %d - %d repeates\n", imvx, ctRepeats);
	}
}

static void SortSurfacesByMorphs(MeshLOD &mlod)
{
	const INDEX ctmsrf = mlod.mlod_aSurfaces.Count();
	const INDEX ctmmm  = mlod.mlod_aMorphMaps.Count();
	if(ctmsrf==0) 
	{
		return;
	}
	if(ctmmm==0) 
	{
		return;
	}
	
	CStaticArray<struct MeshSurface> aSurfaces;
	INDEX iMeshSurface = 0;
	INDEX imsrf;
	aSurfaces.New(ctmsrf);
	
	// First copy dynamic surfaces
	for( imsrf = 0; imsrf < ctmsrf; imsrf++ )
	{
		MeshSurface &msrf = mlod.mlod_aSurfaces[imsrf];
		if(msrf.msrf_ulFlags&MS_DYNAMIC_SURFACE) 
		{
			aSurfaces[iMeshSurface] = mlod.mlod_aSurfaces[imsrf];
			iMeshSurface++;
		}
	}
	
	// Then copy other surfaces
	for( imsrf = 0; imsrf < ctmsrf; imsrf++ )
	{
		MeshSurface &msrf = mlod.mlod_aSurfaces[imsrf];
		if(!(msrf.msrf_ulFlags&MS_DYNAMIC_SURFACE)) 
		{
			aSurfaces[iMeshSurface] = mlod.mlod_aSurfaces[imsrf];
			iMeshSurface++;
		}
	}
	// Copy surfaces back to mesh lod
	mlod.mlod_aSurfaces = aSurfaces;
}

void CMesh::SplitSurfaces(MeshLOD &mlod)
{
	CStaticArray<MeshVertex>  aNewVertices; // new unoptimized vertices
	CStaticArray<MeshNormal>  aNewNormals;  // new unoptimized normals
	CStaticArray<MeshUVMap>   aNewUVMaps;   // new unoptimized uvmaps
	CStaticArray<TempWeightMap> aNewWeightMaps; // new unoptimized weightmaps
	CStaticArray<TempMorphMap>  aNewMorphMaps;  // new unoptimized morphmaps
	CStaticStackArray<MeshSurface> aNewSurfaces; // new unoptimized surfaces
	CStaticStackArray<INDEX> aiAddedSurfaces; // surfaces that has been 
	
	// Count weightmaps
	const INDEX ctmwm = mlod.mlod_aWeightMaps.Count();
	const INDEX ctmmm = mlod.mlod_aMorphMaps.Count();
	// if no weight maps no spliting
	if(ctmwm==0 && ctmmm==0) 
	{
		return;
	}
	
	// Expecting relative indices
	ASSERT(mlod.mlod_ulFlags&ML_SURFACE_RELATIVE_INDICES);
	
	FillVertexWeightInfos(mlod);
	
	// Count new vertices
	INDEX ctnvx = 0;
	INDEX invx = 0;
	INDEX imsrf;
	const INDEX ctmsrf = mlod.mlod_aSurfaces.Count();
	for( imsrf = 0; imsrf < ctmsrf; imsrf++ )
	{
		const MeshSurface &msrf = mlod.mlod_aSurfaces[imsrf];
		const INDEX its = msrf.msrf_auwTriangles.Count();
		ctnvx+=(its*3);
	}
	
	ASSERT(ctnvx>0);
	INDEX ctmuvm = mlod.mlod_aUVMaps.Count();
	if(ctmuvm>0) 
	{
		aNewUVMaps.New(ctmuvm);
	}
	for(INDEX imuvm=0;imuvm<ctmuvm;imuvm++) 
	{
		MeshUVMap &muvm = aNewUVMaps[imuvm];
		muvm.muv_aTexCoords.New(ctnvx);
		muvm.muv_iID = mlod.mlod_aUVMaps[imuvm].muv_iID; // Copy uvmap id
	}
	aNewVertices.New(ctnvx);
	aNewNormals.New(ctnvx);
	aNewWeightMaps.New(ctmwm);
	aNewMorphMaps.New(ctmmm);
	for(INDEX itwm=0;itwm<ctmwm;itwm++) 
	{
		TempWeightMap &twm = aNewWeightMaps[itwm];
		MeshWeightMap &mwm = mlod.mlod_aWeightMaps[itwm];
		twm.mwm_iID = mwm.mwm_iID;
	}
	for(INDEX itmm=0;itmm<ctmmm;itmm++) 
	{
		TempMorphMap &tmm = aNewMorphMaps[itmm];
		MeshMorphMap &mmm = mlod.mlod_aMorphMaps[itmm];
		tmm.mwm_iID = mmm.mmp_iID;
		tmm.mwm_bRelative = mmm.mmp_bRelative;
	}
	
	// for each surface in mesh
	for( imsrf = 0; imsrf < ctmsrf; imsrf++ )
	{
		// Get reference to mesh surface
		MeshSurface &msrf = mlod.mlod_aSurfaces[imsrf];
		INDEX ctts = msrf.msrf_auwTriangles.Count();
		
		CStaticStackArray<MeshSubSurface> aWeightSubSurfaces;// Array of sub surfaces that will be created from this surface
		CStaticStackArray<MeshSubSurface> aMorphSubSurfaces; // Array of sub surfaces that have morphs that will be created from this surface
		CStaticStackArray<INDEX> aiAllTriangleWeights;       // array of all weights that triangle uses
		CStaticStackArray<INDEX> aiTriangleWeights;          // array of weights that triangle uses(no duplicate weights)
		
		// for each triangle in surface
		for(INDEX its=0;its<ctts;its++) 
		{
			MeshTriangle &ts = msrf.msrf_auwTriangles[its];
#if 0 && _DEBUG
			ASSERT(ts.mt_uwVertexIndex[0] != 0xCDCD);
			ASSERT(ts.mt_uwVertexIndex[1] != 0xCDCD);
			ASSERT(ts.mt_uwVertexIndex[2] != 0xCDCD);
#endif
			const UWORD uwVx1 = ts.mt_uwVertexIndex[0] + msrf.msrf_iFirstVertex;
			const UWORD uwVx2 = ts.mt_uwVertexIndex[1] + msrf.msrf_iFirstVertex;
			const UWORD uwVx3 = ts.mt_uwVertexIndex[2] + msrf.msrf_iFirstVertex;
			const UWORD auwVx[3] = 
			{
				ts.mt_uwVertexIndex[0] + msrf.msrf_iFirstVertex,
					ts.mt_uwVertexIndex[1] + msrf.msrf_iFirstVertex,
					ts.mt_uwVertexIndex[2] + msrf.msrf_iFirstVertex
			};
			
			BOOL bHasWeights = ctmwm>0;
			BOOL bHasMorphs = FALSE;
			for(INDEX i=0;i<3;i++) 
			{
				INDEX immm,imvm;
				bHasMorphs |= FindVertexInMorphMap(auwVx[i],mlod,&immm,&imvm,0);
			}
			const BOOL bTriangleHasMorphs = bHasMorphs;
			
			// clear weights for next triangle
			aiAllTriangleWeights.PopAll();
			aiTriangleWeights.PopAll();
			
			if(bHasWeights) 
			{
				MeshVertexWeightInfo &mvwi1 = mlod.mlod_aVertexWeights[uwVx1];
				MeshVertexWeightInfo &mvwi2 = mlod.mlod_aVertexWeights[uwVx2];
				MeshVertexWeightInfo &mvwi3 = mlod.mlod_aVertexWeights[uwVx3];
				
				/* Fill array of weights used by triangle */
				// for each posible weight index
				for(INDEX iwmi=0;iwmi<MAX_BPV;iwmi++) 
				{
					// generate array of weight map indices
					const UBYTE aubTriWeights[3] = 
					{
						mvwi1.mvwi_aubIndices[iwmi],
							mvwi2.mvwi_aubIndices[iwmi],
							mvwi3.mvwi_aubIndices[iwmi]
					};
					const UBYTE aubTriFactors[3] = 
					{
						mvwi1.mvwi_aubWeights[iwmi],
							mvwi2.mvwi_aubWeights[iwmi],
							mvwi3.mvwi_aubWeights[iwmi]
					};
					// add each weight map to array of weights used by triangle
					for(INDEX i=0;i<3;i++) 
					{
						// if this weight index is zero and factor is zero
						if(aubTriWeights[i]==0 && aubTriFactors[i]==0) 
						{
							// this weight isnt used
							NOTHING;
							// else 
						}
						else 
						{
							// add triangle weight
							INDEX &itwi = aiAllTriangleWeights.Push();
							itwi = aubTriWeights[i];
						}
					}
				}
				
				// remove duplicate weights in triangle weights array 
				INDEX ctdtw = aiAllTriangleWeights.Count();
				for(INDEX idtw=0;idtw<ctdtw;idtw++) 
				{
					INDEX idtwi = aiAllTriangleWeights[idtw]; // triangle weight map index
					
					BOOL bWeightExists = FALSE;
					// for all weight indices in triangle
					INDEX cttw=aiTriangleWeights.Count();
					// check if weight is already added
					for(INDEX itw=0;itw<cttw;itw++) 
					{
						if(idtwi==aiTriangleWeights[itw]) 
						{
							bWeightExists = TRUE;
						}
					}
					// if weight is not in array of weights 
					if(!bWeightExists) 
					{
						// add it now
						INDEX &itw = aiTriangleWeights.Push();
						itw = idtwi;
					}
				}
			}
			
			BOOL bTriangleAdded = FALSE;
			// for each existing subsurface created from original mesh surface
			
			
			INDEX ctss;
			if(bTriangleHasMorphs) 
			{
				ctss = aMorphSubSurfaces.Count();
			}
			else 
			{
				ctss = aWeightSubSurfaces.Count();
			}
			
			for(INDEX iss=0;iss<ctss;iss++) 
			{
				MeshSubSurface *pmss;
				if(bTriangleHasMorphs) 
				{
					pmss = &aMorphSubSurfaces[iss];
				}
				else 
				{
					pmss = &aWeightSubSurfaces[iss];
				}
				
				// Count weight maps that are not in this subsurface
				INDEX cttwNew = 0;
				INDEX cttw=aiTriangleWeights.Count();
				for(INDEX itw=0;itw<cttw;itw++) 
				{
					const INDEX itwi = aiTriangleWeights[itw];
					// if weight map isn't used by surface
					if(pmss->mssrf_aiWeightMaps[itwi] == -1) 
					{
						// increment new weights in triangle
						cttwNew++;
					}
				}
				
				// This must never hapend
				if(cttwNew>=MAX_WEIGHTS_PER_SURFACE) 
				{
					CPrintF("Triangle %d in Surface '%s' has more than %d weight maps",its,ska_IDToString(msrf.msrf_iSurfaceID),MAX_WEIGHTS_PER_SURFACE-1);
					ASSERT(FALSE);
					return;
				}
				
				// if this triangle can fit into this subsurface
				if(pmss->mssrf_ctmwm+cttwNew<=MAX_WEIGHTS_PER_SURFACE) 
				{
					// Add this triangle to this surface
					bTriangleAdded = TRUE;
					
					// Add new triangle
					MeshTriangle &mt = pmss->mssrf_auwTriangles.Push();
					for(INDEX ivx=0;ivx<3;ivx++) 
					{
						const INDEX ivxi = auwVx[ivx];
						aNewVertices[invx+ivx] = mlod.mlod_aVertices[ivxi];
						aNewNormals [invx+ivx] = mlod.mlod_aNormals[ivxi];
						mt.mt_uwVertexIndex[ivx] = invx+ivx;
						// for each uvmap
						for(INDEX imuvm=0;imuvm<ctmuvm;imuvm++) 
						{
							const MeshUVMap &muvmOld = mlod.mlod_aUVMaps[imuvm];
							MeshUVMap &muvmNew = aNewUVMaps[imuvm];
							muvmNew.muv_aTexCoords[invx+ivx] = muvmOld.muv_aTexCoords[ivxi];
						}
						INDEX imvw = 0;
						INDEX imwm = 0;
						while(TRUE) 
						{
							// add this vertex from all weight maps to new weight maps
							if(FindVertexInWeightMap(ivxi,mlod,&imwm,&imvw,imwm)) 
							{
								const MeshWeightMap &mwm = mlod.mlod_aWeightMaps[imwm];
								const MeshVertexWeight &mvw = mwm.mwm_aVertexWeight[imvw];
								TempWeightMap &twm = aNewWeightMaps[imwm];
								MeshVertexWeight &mvwNew = twm.mwm_aVertexWeight.Push();
								mvwNew.mww_fWeight = mvw.mww_fWeight;
								mvwNew.mww_iVertex = invx+ivx;
								imwm++;
							}
							else 
							{
								break;
							}
						}
						INDEX immm = 0;
						INDEX imvm = 0;
						while(TRUE) 
						{
							// add this vertex from all morph maps to new morph maps
							if(FindVertexInMorphMap(ivxi,mlod,&immm,&imvm,immm)) 
							{
								const MeshMorphMap &mmm = mlod.mlod_aMorphMaps[immm];
								const MeshVertexMorph &mvm = mmm.mmp_aMorphMap[imvm];
								TempMorphMap &tmm = aNewMorphMaps[immm];
								MeshVertexMorph &mvmNew = tmm.mmp_aMorphMap.Push();
								mvmNew = mvm;
								mvmNew.mwm_iVxIndex = invx+ivx;
								immm++;
							}
							else 
							{
								break;
							}
						}
					}
					invx+=3;// increment vertex count
					
					// Mark new weights in surface
					for(INDEX itw=0;itw<cttw;itw++) 
					{
						INDEX itwi = aiTriangleWeights[itw];
						if(pmss->mssrf_aiWeightMaps[itwi]==(-1)) 
						{
							pmss->mssrf_aiWeightMaps[itwi] = 1;
							pmss->mssrf_ctmwm++;
						}
					}
				}
				// if triangle has been added 
				if(bTriangleAdded) 
				{
					// don't add it do another sub surface
					break;
				}
			}
			
			// if triangle hasn't been added in any of existing subsurfaces
			if(!bTriangleAdded) 
			{
				// Create new sub surface :(
				MeshSubSurface *pmssNew;
				
				BOOL bSurfaceHasBeenAdded = FALSE;
				INDEX ctAddedSurfaces = aiAddedSurfaces.Count();
				for(INDEX ias=0;ias<ctAddedSurfaces;ias++) 
				{
					if(aiAddedSurfaces[ias] == msrf.msrf_iSurfaceID) 
					{
						bSurfaceHasBeenAdded = TRUE;
					}
				}
				INDEX ctmssTotal;
				if(bTriangleHasMorphs) 
				{
					pmssNew = &aMorphSubSurfaces.Push();
					ctmssTotal = aMorphSubSurfaces.Count();
				}
				else 
				{
					pmssNew = &aWeightSubSurfaces.Push();
					ctmssTotal = aWeightSubSurfaces.Count();
				}
				
				// copy surface data
				// if this is subsurface hasn't been added then this is surface
				if(!bSurfaceHasBeenAdded) 
				{
					pmssNew->mssrf_iID = msrf.msrf_iSurfaceID;
					pmssNew->mssrf_ulFlags = msrf.msrf_ulFlags;
					// Add new surface to list of added surfaces
					INDEX &iNewSurfaceID = aiAddedSurfaces.Push();
					iNewSurfaceID = msrf.msrf_iSurfaceID;
					
					// else generate name for subsurface
				}
				else 
				{
					CTString strSurfaceName = ska_GetStringFromTable(msrf.msrf_iSurfaceID);
					CTString strSubSurfaceName = strSurfaceName + CTString(0,"_%d",ctmssTotal);
					pmssNew->mssrf_iID = ska_GetIDFromStringTable(strSubSurfaceName);
					// Flag new subsurface as subsurface
					pmssNew->mssrf_ulFlags = msrf.msrf_ulFlags|MS_MESH_SUBSURFACE;
				}
				// Copy shader properties
				pmssNew->msrf_ShadingParams = msrf.msrf_ShadingParams;
				if(msrf.msrf_pShader!=NULL) 
				{
					pmssNew->mssrf_fnShader = msrf.msrf_pShader->ser_FileName;
				}
				
				// Create table of used weight maps
				pmssNew->mssrf_aiWeightMaps.New(ctmwm);
				// Zero weight maps in this surface so far
				pmssNew->mssrf_ctmwm = 0;
				// Reset table of used weight maps
				for(INDEX imwm=0;imwm<ctmwm;imwm++) 
				{
					INDEX &imwmi = pmssNew->mssrf_aiWeightMaps[imwm];
					imwmi = -1;
				}
				
				// Add new triangle
				MeshTriangle &mt = pmssNew->mssrf_auwTriangles.Push();
				for(INDEX ivx=0;ivx<3;ivx++) 
				{
					const INDEX ivxi = auwVx[ivx];
					aNewVertices[invx+ivx] = mlod.mlod_aVertices[ivxi];
					aNewNormals [invx+ivx] = mlod.mlod_aNormals[ivxi];
					mt.mt_uwVertexIndex[ivx] = invx+ivx;
					// for each uvmap
					for(INDEX imuvm=0;imuvm<ctmuvm;imuvm++) 
					{
						const MeshUVMap &muvmOld = mlod.mlod_aUVMaps[imuvm];
						MeshUVMap &muvmNew = aNewUVMaps[imuvm];
						muvmNew.muv_aTexCoords[invx+ivx] = muvmOld.muv_aTexCoords[ivxi];
					}
					INDEX imvw = 0;
					INDEX imwm = 0;
					while(TRUE) 
					{
						// add this vertex from all weight maps to new weight maps
						if(FindVertexInWeightMap(ivxi,mlod,&imwm,&imvw,imwm)) 
						{
							const MeshWeightMap &mwm = mlod.mlod_aWeightMaps[imwm];
							const MeshVertexWeight &mvw = mwm.mwm_aVertexWeight[imvw];
							TempWeightMap &twm = aNewWeightMaps[imwm];
							MeshVertexWeight &mvwNew = twm.mwm_aVertexWeight.Push();
							mvwNew.mww_fWeight = mvw.mww_fWeight;
							mvwNew.mww_iVertex = invx+ivx;
							imwm++;
						}
						else 
						{
							break;
						}
					}
					INDEX immm = 0;
					INDEX imvm = 0;
					while(TRUE) 
					{
						// add this vertex from all morph maps to new morph maps
						if(FindVertexInMorphMap(ivxi,mlod,&immm,&imvm,immm)) 
						{
							const MeshMorphMap &mmm = mlod.mlod_aMorphMaps[immm];
							const MeshVertexMorph &mvm = mmm.mmp_aMorphMap[imvm];
							TempMorphMap &tmm = aNewMorphMaps[immm];
							MeshVertexMorph &mvmNew = tmm.mmp_aMorphMap.Push();
							mvmNew = mvm;
							mvmNew.mwm_iVxIndex = invx+ivx;
							immm++;
						}
						else 
						{
							break;
						}
					}
				}
				invx+=3;// increment vertex count
				
				// Mark new weights in surface
				INDEX cttw=aiTriangleWeights.Count();
				for(INDEX itw=0;itw<cttw;itw++) 
				{
					INDEX itwi = aiTriangleWeights[itw];
					if(pmssNew->mssrf_aiWeightMaps[itwi]==(-1)) 
					{
						pmssNew->mssrf_aiWeightMaps[itwi] = 1;
						pmssNew->mssrf_ctmwm++;
					}
				}
			}
		}
		
		/*
		CPrintF("%d Subsurfaces, %d Total weights\n",ctmss, ctmwm);
		
		  for(INDEX imss=0;imss<ctmss;imss++) 
		  {
		  MeshSubSurface &mss = aSubSurfaces[imss];
		  CPrintF("%d weights in %d. surface\n",mss.mssrf_ctmwm,imss);
		  }
		  */
		  
		  
		  // Copy morph and weight sub surfaces to mesh surfaces
		  for(INDEX i=0;i<2;i++) 
		  {
			  const BOOL bSurfacesWithMorphs = i==0;
			  INDEX ctmss;
			  if(bSurfacesWithMorphs) 
			  {
				  // copy all surfaces with morph maps
				  ctmss = aMorphSubSurfaces.Count();
			  }
			  else 
			  {
				  // copy all surfaces without morph maps
				  ctmss = aWeightSubSurfaces.Count();
			  }
			  
			  MeshSurface *pmsrf = NULL;
			  if(ctmss>0) 
			  {
				  pmsrf = aNewSurfaces.Push(ctmss);
			  }
			  
			  for(INDEX imss=0;imss<ctmss;imss++) 
			  {
				  MeshSurface &msrfNew = pmsrf[imss];
				  MeshSubSurface *pmss;
				  if(bSurfacesWithMorphs) 
				  {
					  pmss = &aMorphSubSurfaces[imss];
				  }
				  else 
				  {
					  pmss = &aWeightSubSurfaces[imss];
				  }
				  msrfNew.msrf_iSurfaceID = pmss->mssrf_iID;
				  msrfNew.msrf_ulFlags = pmss->mssrf_ulFlags;
				  msrfNew.msrf_ShadingParams = pmss->msrf_ShadingParams;
				  msrfNew.msrf_pShader = NULL;
				  if(pmss->mssrf_fnShader.Length()>0) 
				  {
					  try 
					  {
						  msrfNew.msrf_pShader = _pShaderStock->Obtain_t(pmss->mssrf_fnShader);
					  }
					  catch(char *) 
					  {
						  msrfNew.msrf_pShader = NULL;
					  }
				  }
				  const CTString strSurface = ska_IDToString(msrfNew.msrf_iSurfaceID);
				  if(bSurfacesWithMorphs) 
				  {
					  msrfNew.msrf_ulFlags |= MS_DYNAMIC_SURFACE;
				  }
				  msrfNew.msrf_iFirstVertex = 0;
				  msrfNew.msrf_ctVertices = ctnvx;
				  INDEX ctmt = pmss->mssrf_auwTriangles.Count();
				  msrfNew.msrf_auwTriangles.New(ctmt);
				  memcpy(&msrfNew.msrf_auwTriangles[0],&pmss->mssrf_auwTriangles[0],sizeof(MeshTriangle)*ctmt);
#if 0 && _DEBUG
				  INDEX ctst = pmss->mssrf_auwTriangles.Count();
				  for(INDEX ist=0;ist<ctst;ist++) 
				  {
					  ASSERT(msrfNew.msrf_auwTriangles[ist].mt_uwVertexIndex[0] != 0xCDCD);
					  ASSERT(msrfNew.msrf_auwTriangles[ist].mt_uwVertexIndex[1] != 0xCDCD);
					  ASSERT(msrfNew.msrf_auwTriangles[ist].mt_uwVertexIndex[2] != 0xCDCD);
				  }
#endif
			  }
		  }
	}
	// Copy new arrays to mesh lod
	mlod.mlod_aVertices = aNewVertices;
	mlod.mlod_aNormals = aNewNormals;
	mlod.mlod_aUVMaps = aNewUVMaps;
	mlod.mlod_aWeightMaps.Clear();
	mlod.mlod_aMorphMaps.Clear();
	mlod.mlod_aVertexWeights.Clear();
	// Copy new surfaces
	INDEX ctnmsrf = aNewSurfaces.Count();
	mlod.mlod_aSurfaces.Clear();
	mlod.mlod_aSurfaces.New(ctnmsrf);
	for( imsrf = 0; imsrf < ctnmsrf; imsrf++ )
	{
		MeshSurface &msrf = mlod.mlod_aSurfaces[imsrf];
		msrf = aNewSurfaces[imsrf];
	}
	
	if(ctmwm>0) 
	{
		mlod.mlod_aWeightMaps.New(ctmwm);
	}
	for(INDEX imwm=0;imwm<ctmwm;imwm++) 
	{
		MeshWeightMap &mwm = mlod.mlod_aWeightMaps[imwm];
		const TempWeightMap &twm = aNewWeightMaps[imwm];
		// Copy weight map id
		mwm.mwm_iID = twm.mwm_iID;
		// Copy vertex weights
		const INDEX ctmvw = twm.mwm_aVertexWeight.Count();
		if(ctmvw>0) 
		{
			mwm.mwm_aVertexWeight.New(ctmvw);
			memcpy(&mwm.mwm_aVertexWeight[0],&twm.mwm_aVertexWeight[0],sizeof(MeshVertexWeight)*ctmvw);
		}
	}
	
	if(ctmmm>0) 
	{
		mlod.mlod_aMorphMaps.New(ctmmm);
	}
	for(INDEX immm=0;immm<ctmmm;immm++) 
	{
		MeshMorphMap &mmm = mlod.mlod_aMorphMaps[immm];
		const TempMorphMap &tmm = aNewMorphMaps[immm];
		// Copy new morphs to mesh lod
		mmm.mmp_iID = tmm.mwm_iID;
		mmm.mmp_bRelative = tmm.mwm_bRelative;
		const INDEX ctmvm = tmm.mmp_aMorphMap.Count();
		if(ctmvm>0) 
		{
			mmm.mmp_aMorphMap.New(ctmvm);
			memcpy(&mmm.mmp_aMorphMap[0],&tmm.mmp_aMorphMap[0],sizeof(MeshVertexMorph)*ctmvm);
		}
	}
	
	// surface indices are absulute
	mlod.mlod_ulFlags&=~ML_SURFACE_RELATIVE_INDICES;
	
	// Morph surfaces must be first in array
	SortSurfacesByMorphs(mlod);
}

// optimize lod of mesh
void CMesh::OptimizeLod(MeshLOD &mlod)
{
	INDEX ctVertices   = mlod.mlod_aVertices.Count();
	INDEX ctSurfaces   = mlod.mlod_aSurfaces.Count();
	INDEX ctUVMaps     = mlod.mlod_aUVMaps.Count();
	INDEX ctWeightMaps = mlod.mlod_aWeightMaps.Count();
	INDEX ctMorphMaps  = mlod.mlod_aMorphMaps.Count();
	
//안태훈 수정 시작	//(Add Tagent-space Normal Map)(0.1)
	const BOOL bHasTangent = mlod.mlod_aTangents.Count()>0 ? TRUE : FALSE;
//안태훈 수정 끝	//(Add Tagent-space Normal Map)(0.1)
	
	if(ctVertices<=0) return;
	
	ASSERT(_aSortArray.Count()==0);
	ASSERT(_aiSortedIndex.Count()==0);
	ASSERT(_aiOptimizedIndex.Count()==0);
	ASSERT(_avwSortWeights.Count()==0);
	
	ASSERT(!(mlod.mlod_ulFlags&ML_SURFACE_RELATIVE_INDICES));
	
	// create array for sorting
	_aSortArray.New(ctVertices);
	_aiSortedIndex.New(ctVertices);
	_aiOptimizedIndex.New(ctVertices);
	// put original vertex indices in SortArray
	for(INDEX iv=0;iv<ctVertices;iv++) 
	{
		_aiSortedIndex[iv] = iv;
	}
	
	// loop each surface and remember surface indices per vertex
	INDEX	imsrf;
	for( imsrf = 0; imsrf < ctSurfaces; imsrf++ )
	{
		const MeshSurface &msrf = mlod.mlod_aSurfaces[imsrf];
		INDEX ctts=msrf.msrf_auwTriangles.Count();
		for(int its=0;its<ctts;its++) 
		{
			const MeshTriangle &mt = msrf.msrf_auwTriangles[its];
#if 0 && _DEBUG
			ASSERT(mt.mt_uwVertexIndex[0]!=0xCDCD);
			ASSERT(mt.mt_uwVertexIndex[1]!=0xCDCD);
			ASSERT(mt.mt_uwVertexIndex[2]!=0xCDCD);
#endif
			_aSortArray[mt.mt_uwVertexIndex[0]].sa_iSurfaceIndex = imsrf;
			_aSortArray[mt.mt_uwVertexIndex[1]].sa_iSurfaceIndex = imsrf;
			_aSortArray[mt.mt_uwVertexIndex[2]].sa_iSurfaceIndex = imsrf;
		}
	}
	
	// loop each weightmap and expand sa_aWeightMapList in SortArray
	for(INDEX imwm=0;imwm<ctWeightMaps;imwm++)   
	{
		MeshWeightMap &mwm = mlod.mlod_aWeightMaps[imwm];
		// loop each vertex weight array in weight map array
		INDEX ctwm = mwm.mwm_aVertexWeight.Count();
		for(INDEX imvw=0;imvw<ctwm;imvw++) 
		{
			MeshVertexWeight &mwwWeight = mwm.mwm_aVertexWeight[imvw];
			// get curent list num of weightmaps  
			INDEX ctWeightMapList = _aSortArray[mwwWeight.mww_iVertex].sa_aWeightMapList.Count();
			// expand array of sufrace lists for 1
			_aSortArray[mwwWeight.mww_iVertex].sa_aWeightMapList.Expand(ctWeightMapList+1);
			// set vl_iIndex to index of mesh weight map
			// set vl_iSubIndex to index in mesh vertex weight in weight map
			VertexLocator &vxLoc = _aSortArray[mwwWeight.mww_iVertex].sa_aWeightMapList[ctWeightMapList];
			vxLoc.vl_iIndex = imwm;
			vxLoc.vl_iSubIndex = imvw;
		}
	}
	
	// loop each morphmap and expand sa_aMorphMapList in SortArray
	for(INDEX im=0;im<ctMorphMaps;im++) 
	{
		// loop each morph map in array
		INDEX ctmm = mlod.mlod_aMorphMaps[im].mmp_aMorphMap.Count();
		for(INDEX imm=0;imm<ctmm;imm++) 
		{
			MeshVertexMorph &mwmMorph = mlod.mlod_aMorphMaps[im].mmp_aMorphMap[imm];
			// get curent list num of morphmaps  
			INDEX ctMorphMapList = _aSortArray[mwmMorph.mwm_iVxIndex].sa_aMorphMapList.Count();
			// expand array of sufrace lists for 1
			_aSortArray[mwmMorph.mwm_iVxIndex].sa_aMorphMapList.Expand(ctMorphMapList+1);
			// set vl_iIndex to index of mesh morph map
			// set vl_iSubIndex to index in mesh morph vertex
			VertexLocator &vxLoc = _aSortArray[mwmMorph.mwm_iVxIndex].sa_aMorphMapList[ctMorphMapList];
			vxLoc.vl_iIndex = im;
			vxLoc.vl_iSubIndex = imm;
		}
	}
	
	// set global pMeshLOD pointer used by quicksort
	pMeshLOD = &mlod;
	
	// sort array
	qsort(&_aiSortedIndex[0],ctVertices,sizeof(_aiSortedIndex[0]),qsort_CompareArray);
	
	// compare vertices
	INDEX iDiferentVertices = 1;
	INDEX iLastIndex = _aiSortedIndex[0];
	_aSortArray[iLastIndex].sa_iNewIndex = 0;
	_aiOptimizedIndex[0] = iLastIndex;
	
	for(INDEX isa=1;isa<ctVertices;isa++) 
	{
		INDEX iCurrentIndex = _aiSortedIndex[isa];
		// check if vertices are diferent
		if(AreVerticesDiferent(iLastIndex,iCurrentIndex)) 
		{
			// add Curent index to Optimized index array
			_aiOptimizedIndex[iDiferentVertices] = iCurrentIndex;
			iDiferentVertices++;
			iLastIndex = iCurrentIndex;
		}
		_aSortArray[iCurrentIndex].sa_iNewIndex = iDiferentVertices-1;
	}
	
	// create new mesh
	INDEX ctNewVertices = iDiferentVertices;
	_mshOptimized.mlod_aVertices.New(ctNewVertices);
	_mshOptimized.mlod_aNormals.New(ctNewVertices);
	_mshOptimized.mlod_aUVMaps.New(ctUVMaps);
//안태훈 수정 시작	//(Add Tagent-space Normal Map)(0.1)
	if(bHasTangent) _mshOptimized.mlod_aTangents.New(ctNewVertices);
//안태훈 수정 끝	//(Add Tagent-space Normal Map)(0.1)

	for(INDEX imuvm=0;imuvm<ctUVMaps;imuvm++) 
	{
		_mshOptimized.mlod_aUVMaps[imuvm].muv_aTexCoords.New(ctNewVertices);
	}
	
	// add new vertices and normals to _mshOptimized
	for(INDEX iNewVx=0;iNewVx<ctNewVertices;iNewVx++) 
	{
		_mshOptimized.mlod_aVertices[iNewVx] = mlod.mlod_aVertices[_aiOptimizedIndex[iNewVx]];
		_mshOptimized.mlod_aNormals[iNewVx] = mlod.mlod_aNormals[_aiOptimizedIndex[iNewVx]];
//안태훈 수정 시작	//(Add Tagent-space Normal Map)(0.1)
		if(bHasTangent) _mshOptimized.mlod_aTangents[iNewVx] = mlod.mlod_aTangents[_aiOptimizedIndex[iNewVx]];
//안태훈 수정 끝	//(Add Tagent-space Normal Map)(0.1)

		for(INDEX imuvm=0;imuvm<ctUVMaps;imuvm++) 
		{
			//???
			_mshOptimized.mlod_aUVMaps[imuvm].muv_iID = mlod.mlod_aUVMaps[imuvm].muv_iID;
			_mshOptimized.mlod_aUVMaps[imuvm].muv_aTexCoords[iNewVx] = mlod.mlod_aUVMaps[imuvm].muv_aTexCoords[_aiOptimizedIndex[iNewVx]];
		}
	}
	
	// remap surface triangles
	for( imsrf = 0; imsrf < ctSurfaces; imsrf++ )
	{
		MeshSurface &msrf = mlod.mlod_aSurfaces[imsrf];
		INDEX iMinIndex = ctNewVertices+1;
		INDEX iMaxIndex = -1;
		INDEX ctts = msrf.msrf_auwTriangles.Count();
		INDEX its;
		// for each triangle in this surface
		for( its = 0; its < ctts; its++ ) 
		{
			MeshTriangle &mtTriangle = msrf.msrf_auwTriangles[its];
			// for each vertex in triangle
			for(INDEX iv=0;iv<3;iv++)
			{
				mtTriangle.mt_uwVertexIndex[iv] = _aSortArray[mtTriangle.mt_uwVertexIndex[iv]].sa_iNewIndex;
				// find first index in this surface
				if(mtTriangle.mt_uwVertexIndex[iv]<iMinIndex) iMinIndex = mtTriangle.mt_uwVertexIndex[iv];
				// find last index in this surface
				if(mtTriangle.mt_uwVertexIndex[iv]>iMaxIndex) iMaxIndex = mtTriangle.mt_uwVertexIndex[iv];
			}
		}
		// remember first index in vertices array
		msrf.msrf_iFirstVertex = iMinIndex;
		// remember vertices count
		msrf.msrf_ctVertices = iMaxIndex-iMinIndex+1;
		
		// for each triangle in surface
		for( its = 0; its < ctts; its++ )
		{
			MeshTriangle &mtTriangle = msrf.msrf_auwTriangles[its];
			// for each vertex in triangle
			for(INDEX iv=0;iv<3;iv++) 
			{
				// substract vertex index in triangle with first vertex in surface
				mtTriangle.mt_uwVertexIndex[iv] -= msrf.msrf_iFirstVertex;
				ASSERT( mtTriangle.mt_uwVertexIndex[iv] < msrf.msrf_ctVertices);
			}
		}
	}
	
	// remap weightmaps
	_mshOptimized.mlod_aWeightMaps.New(ctWeightMaps);
	// expand wertex veights array for each vertex
	INDEX	ivx;
	for( ivx = 0; ivx < ctNewVertices; ivx++ )
	{
		INDEX ioptVx = _aiOptimizedIndex[ivx];
		for(INDEX iwl=0;iwl<_aSortArray[ioptVx].sa_aWeightMapList.Count();iwl++) 
		{
			VertexLocator &wml = _aSortArray[ioptVx].sa_aWeightMapList[iwl];
			INDEX wmIndex = wml.vl_iIndex;
			INDEX wwIndex = wml.vl_iSubIndex;
			INDEX ctww = _mshOptimized.mlod_aWeightMaps[wmIndex].mwm_aVertexWeight.Count();
			MeshWeightMap &mwm = _mshOptimized.mlod_aWeightMaps[wmIndex];
			MeshVertexWeight &mww = mlod.mlod_aWeightMaps[wmIndex].mwm_aVertexWeight[wwIndex];
			
			mwm.mwm_iID = mlod.mlod_aWeightMaps[wmIndex].mwm_iID;
			mwm.mwm_aVertexWeight.Expand(ctww+1);
			mwm.mwm_aVertexWeight[ctww].mww_fWeight = mww.mww_fWeight;
			mwm.mwm_aVertexWeight[ctww].mww_iVertex = ivx;
		}
	}
	
	// remap morphmaps
	_mshOptimized.mlod_aMorphMaps.New(ctMorphMaps);
	// expand morph maps array for each vertex
	for( ivx = 0; ivx < ctNewVertices; ivx++)
	{
		INDEX ioptVx = _aiOptimizedIndex[ivx];
		for(INDEX iml=0;iml<_aSortArray[ioptVx].sa_aMorphMapList.Count();iml++) 
		{
			VertexLocator &mml = _aSortArray[ioptVx].sa_aMorphMapList[iml];
			INDEX mmIndex = mml.vl_iIndex;
			INDEX mwmIndex = mml.vl_iSubIndex;
			INDEX ctmwm = _mshOptimized.mlod_aMorphMaps[mmIndex].mmp_aMorphMap.Count();
			MeshMorphMap &mmm = _mshOptimized.mlod_aMorphMaps[mmIndex];
			MeshVertexMorph &mwm = mlod.mlod_aMorphMaps[mmIndex].mmp_aMorphMap[mwmIndex];
			
			mmm.mmp_iID = mlod.mlod_aMorphMaps[mmIndex].mmp_iID;
			mmm.mmp_bRelative = mlod.mlod_aMorphMaps[mmIndex].mmp_bRelative;
			
			mmm.mmp_aMorphMap.Expand(ctmwm+1);
			mmm.mmp_aMorphMap[ctmwm].mwm_iVxIndex = ivx;
			mmm.mmp_aMorphMap[ctmwm].mwm_x = mwm.mwm_x;
			mmm.mmp_aMorphMap[ctmwm].mwm_y = mwm.mwm_y;
			mmm.mmp_aMorphMap[ctmwm].mwm_z = mwm.mwm_z;
			mmm.mmp_aMorphMap[ctmwm].mwm_nx = mwm.mwm_nx;
			mmm.mmp_aMorphMap[ctmwm].mwm_ny = mwm.mwm_ny;
			mmm.mmp_aMorphMap[ctmwm].mwm_nz = mwm.mwm_nz;
		}
	}
	
	// Copy arrays
	mlod.mlod_aVertices.CopyArray(_mshOptimized.mlod_aVertices);
	mlod.mlod_aNormals.CopyArray(_mshOptimized.mlod_aNormals);
//안태훈 수정 시작	//(Add Tagent-space Normal Map)(0.1)
	if(bHasTangent) mlod.mlod_aTangents.CopyArray(_mshOptimized.mlod_aTangents);
//안태훈 수정 끝	//(Add Tagent-space Normal Map)(0.1)
	mlod.mlod_aMorphMaps.CopyArray(_mshOptimized.mlod_aMorphMaps);
	mlod.mlod_aWeightMaps.CopyArray(_mshOptimized.mlod_aWeightMaps);
	mlod.mlod_aUVMaps.CopyArray(_mshOptimized.mlod_aUVMaps);
	
	// clear memory
	ClearSortArray(ctVertices);
	_mshOptimized.mlod_aVertices.Clear();
	_mshOptimized.mlod_aNormals.Clear();
//안태훈 수정 시작	//(Add Tagent-space Normal Map)(0.1)
	if(bHasTangent) _mshOptimized.mlod_aTangents.Clear();
//안태훈 수정 끝	//(Add Tagent-space Normal Map)(0.1)
	_mshOptimized.mlod_aWeightMaps.Clear();
	_mshOptimized.mlod_aMorphMaps.Clear();
	_mshOptimized.mlod_aUVMaps.Clear();
	_mshOptimized.mlod_aVertexWeights.Clear();
	_mshOptimized.mlod_aSurfaces.Clear();
	
	// Indices are surface relative
	mlod.mlod_ulFlags|=ML_SURFACE_RELATIVE_INDICES;
	
	// fill vertex weights
	FillVertexWeightInfos(mlod);
}

#define CHECK(x,y)  if(((x)-(y))!=0) return ((x)-(y))
#define CHECKF(x,y) if(((x)-(y))!=0) return Sgn((x)-(y))

static INDEX AreVerticesDiferent(INDEX iCurrentIndex, INDEX iLastIndex)
{
	// check surfaces
	CHECK(_aSortArray[iCurrentIndex].sa_iSurfaceIndex,_aSortArray[iLastIndex].sa_iSurfaceIndex);
	
	// check vertices per weight
	// CHECK(_avwSortWeights[iCurrentIndex].svw_ctWeights,_avwSortWeights[iLastIndex].svw_ctWeights);
	
	// check vertices
	CHECKF(pMeshLOD->mlod_aVertices[iCurrentIndex].y,pMeshLOD->mlod_aVertices[iLastIndex].y);
	CHECKF(pMeshLOD->mlod_aVertices[iCurrentIndex].x,pMeshLOD->mlod_aVertices[iLastIndex].x);
	CHECKF(pMeshLOD->mlod_aVertices[iCurrentIndex].z,pMeshLOD->mlod_aVertices[iLastIndex].z);
	// check normals
	CHECKF(pMeshLOD->mlod_aNormals[iCurrentIndex].ny,pMeshLOD->mlod_aNormals[iLastIndex].ny);
	CHECKF(pMeshLOD->mlod_aNormals[iCurrentIndex].nx,pMeshLOD->mlod_aNormals[iLastIndex].nx);
	CHECKF(pMeshLOD->mlod_aNormals[iCurrentIndex].nz,pMeshLOD->mlod_aNormals[iLastIndex].nz);
//안태훈 수정 시작	//(Add Tagent-space Normal Map)(0.1)
	// check tangent
	if(pMeshLOD->mlod_aTangents.Count() > 0)
	{
		CHECKF(pMeshLOD->mlod_aTangents[iCurrentIndex].by,pMeshLOD->mlod_aTangents[iLastIndex].by);
		CHECKF(pMeshLOD->mlod_aTangents[iCurrentIndex].bx,pMeshLOD->mlod_aTangents[iLastIndex].bx);
		CHECKF(pMeshLOD->mlod_aTangents[iCurrentIndex].bz,pMeshLOD->mlod_aTangents[iLastIndex].bz);
		CHECKF(pMeshLOD->mlod_aTangents[iCurrentIndex].tdir,pMeshLOD->mlod_aTangents[iLastIndex].tdir);
	}
//안태훈 수정 끝	//(Add Tagent-space Normal Map)(0.1)
	
	// check uvmaps
	INDEX ctUVMaps = pMeshLOD->mlod_aUVMaps.Count();
	for(INDEX imuvm=0;imuvm<ctUVMaps;imuvm++) 
	{
		MeshUVMap &muvm = pMeshLOD->mlod_aUVMaps[imuvm];
		CHECKF(muvm.muv_aTexCoords[iCurrentIndex].u,muvm.muv_aTexCoords[iLastIndex].u);
		CHECKF(muvm.muv_aTexCoords[iCurrentIndex].v,muvm.muv_aTexCoords[iLastIndex].v);
	}
	
	// count weight and morph maps
	INDEX ctwmCurent  = _aSortArray[iCurrentIndex].sa_aWeightMapList.Count();
	INDEX ctwmLast    = _aSortArray[iLastIndex].sa_aWeightMapList.Count();
	INDEX ctmmCurent  = _aSortArray[iCurrentIndex].sa_aMorphMapList.Count();
	INDEX ctmmLast    = _aSortArray[iLastIndex].sa_aMorphMapList.Count();
	// check if vertices have same weight and morph maps count
	CHECK(ctwmCurent,ctwmLast);
	CHECK(ctmmCurent,ctmmLast);
	// check if vertices have same weight map factors
	for(INDEX iwm=0;iwm<ctwmCurent;iwm++) 
	{
		// get weight map indices
		INDEX iwmCurent = _aSortArray[iCurrentIndex].sa_aWeightMapList[iwm].vl_iIndex;
		INDEX iwmLast   = _aSortArray[iLastIndex].sa_aWeightMapList[iwm].vl_iIndex;
		// get wertex weight indices
		INDEX iwwCurent = _aSortArray[iCurrentIndex].sa_aWeightMapList[iwm].vl_iSubIndex;
		INDEX iwwLast   = _aSortArray[iLastIndex].sa_aWeightMapList[iwm].vl_iSubIndex;
		
		// if weight map factors are diferent
		CHECKF(pMeshLOD->mlod_aWeightMaps[iwmCurent].mwm_aVertexWeight[iwwCurent].mww_fWeight,pMeshLOD->mlod_aWeightMaps[iwmLast].mwm_aVertexWeight[iwwLast].mww_fWeight);
	}
	
	// check if vertices have same morph map factors
	for(INDEX imm=0;imm<ctmmCurent;imm++) 
	{
		// get morph map indices
		INDEX immCurent = _aSortArray[iCurrentIndex].sa_aMorphMapList[imm].vl_iIndex;
		INDEX immLast   = _aSortArray[iLastIndex].sa_aMorphMapList[imm].vl_iIndex;
		// get mesh vertex morph indices
		INDEX imwmCurent  = _aSortArray[iCurrentIndex].sa_aMorphMapList[imm].vl_iSubIndex;
		INDEX imwmLast    = _aSortArray[iLastIndex].sa_aMorphMapList[imm].vl_iSubIndex;
		
		// if mesh morph map params are diferent return
		CHECKF(pMeshLOD->mlod_aMorphMaps[immCurent].mmp_aMorphMap[imwmCurent].mwm_x,
			pMeshLOD->mlod_aMorphMaps[immLast].mmp_aMorphMap[imwmLast].mwm_x);
		CHECKF(pMeshLOD->mlod_aMorphMaps[immCurent].mmp_aMorphMap[imwmCurent].mwm_y,
			pMeshLOD->mlod_aMorphMaps[immLast].mmp_aMorphMap[imwmLast].mwm_y);
		CHECKF(pMeshLOD->mlod_aMorphMaps[immCurent].mmp_aMorphMap[imwmCurent].mwm_z,
			pMeshLOD->mlod_aMorphMaps[immLast].mmp_aMorphMap[imwmLast].mwm_z);
		CHECKF(pMeshLOD->mlod_aMorphMaps[immCurent].mmp_aMorphMap[imwmCurent].mwm_nx,
			pMeshLOD->mlod_aMorphMaps[immLast].mmp_aMorphMap[imwmLast].mwm_nx);
		CHECKF(pMeshLOD->mlod_aMorphMaps[immCurent].mmp_aMorphMap[imwmCurent].mwm_ny,
			pMeshLOD->mlod_aMorphMaps[immLast].mmp_aMorphMap[imwmLast].mwm_ny);
		CHECKF(pMeshLOD->mlod_aMorphMaps[immCurent].mmp_aMorphMap[imwmCurent].mwm_nz,
			pMeshLOD->mlod_aMorphMaps[immLast].mmp_aMorphMap[imwmLast].mwm_nz);
	}
	// vertices are same
	return 0;
}


// group vertices in 1-weight and non-1-weight groups and sort by indices inside groups
static int qsort_CompareWeights( const void *p1, const void *p2)
{
	ASSERT( p1!=NULL && p2!=NULL);
	MeshVertexWeight *pmvw1 = (MeshVertexWeight*)p1;
	MeshVertexWeight *pmvw2 = (MeshVertexWeight*)p2;
	
	// sort vertices only if one of them has weight factor 1.0f
	if( pmvw1->mww_fWeight<1.0f && pmvw2->mww_fWeight==1.0f) return +1;
	if( pmvw2->mww_fWeight<1.0f && pmvw1->mww_fWeight==1.0f) return -1;
	// sort by vertex order inside weight group
	CHECK( pmvw1->mww_iVertex, pmvw2->mww_iVertex);
	// same vertices
	return 0;
}

struct VertexWeights
{
	FLOAT vw_afWeights[4];
	INDEX vw_aiIndices[4];
};

static void NormalizeWeightsInfosInLod(MeshLOD &mlod)
{
	const BOOL bNormalizedWeights = mlod.mlod_ulFlags&ML_NORMALIZED_WEIGHTS;
	const INDEX ctmvx = mlod.mlod_aVertexWeights.Count();
	for(INDEX imvx=0;imvx<ctmvx;imvx++) 
	{
		MeshVertexWeightInfo &mvwi = mlod.mlod_aVertexWeights[imvx];
		INDEX ctWeightsTotal = 0;
		INDEX iLastWeight = -1;
		for(INDEX i=0;i<4;i++) 
		{
			if(mvwi.mvwi_aubWeights[i]>0) 
			{
				ctWeightsTotal += mvwi.mvwi_aubWeights[i];
				iLastWeight = i;
			}
		}
		if(iLastWeight>=0) 
		{
			INDEX iDiff = 255 - ctWeightsTotal;
			mvwi.mvwi_aubWeights[iLastWeight] += iDiff;
			ASSERT(mvwi.mvwi_aubWeights[0] + mvwi.mvwi_aubWeights[1] + 
				mvwi.mvwi_aubWeights[2] + mvwi.mvwi_aubWeights[3] == 255);
		}
		else 
		{
			// ASSERT(FALSE);
		}
	}
}

// Fill vertex weights infos for lod (absulute weight map indices and weight factors)
static void FillVertexWeightInfos(MeshLOD &mlod)
{
	// Fill vertex weight infos
	const INDEX ctmvx = mlod.mlod_aVertices.Count();
	const INDEX ctmwm = mlod.mlod_aWeightMaps.Count();
	CStaticArray<INDEX> actWeightsPerVertex;
	CStaticArray<VertexWeights> aVertexWeights;
	INDEX ctInvalidWeights = 0;
	
	ASSERT(ctmvx>=0);
	if(ctmwm<=0) 
	{
		mlod.mlod_ulFlags|=ML_USE_VERTEX_PROGRAM;
		return;
	}
	
	// Indices must be relative
	ASSERT(mlod.mlod_ulFlags&ML_SURFACE_RELATIVE_INDICES);
	
	actWeightsPerVertex.New(ctmvx);
	aVertexWeights.New(ctmvx);
	// Reset all weights to 0
	memset(&aVertexWeights[0],0,sizeof(VertexWeights)*ctmvx);
	// Reset all weight count to 0
	memset(&actWeightsPerVertex[0],0,sizeof(INDEX)*ctmvx);
	
	// for each weight map in mesh lod
	for(INDEX imwm=0;imwm<ctmwm;imwm++) 
	{
		const MeshWeightMap &mwm = mlod.mlod_aWeightMaps[imwm];
		// for each vertex weight in weight map
		const INDEX ctmvw = mwm.mwm_aVertexWeight.Count();
		for(INDEX imvw=0;imvw<ctmvw;imvw++) 
		{
			const MeshVertexWeight &mvw = mwm.mwm_aVertexWeight[imvw];
			const INDEX imvx = mvw.mww_iVertex;
			const FLOAT fvw  = mvw.mww_fWeight;
			
			VertexWeights &vw = aVertexWeights[imvx];
			
			if(fvw>vw.vw_afWeights[3]) 
			{
				if(vw.vw_afWeights[3]>0.0f) 
				{
					ctInvalidWeights++;
				}
				else 
				{
					actWeightsPerVertex[imvx]++;
				}
				ASSERT(actWeightsPerVertex[imvx]<=4);
				vw.vw_afWeights[3] = fvw;
				vw.vw_aiIndices[3] = imwm;
			}
			if(fvw>vw.vw_afWeights[2]) 
			{
				Swap(vw.vw_afWeights[3], vw.vw_afWeights[2]);
				Swap(vw.vw_aiIndices[3], vw.vw_aiIndices[2]);
			}
			if(fvw>vw.vw_afWeights[1]) 
			{
				Swap(vw.vw_afWeights[2], vw.vw_afWeights[1]);
				Swap(vw.vw_aiIndices[2], vw.vw_aiIndices[1]);
			}
			if(fvw>vw.vw_afWeights[0]) 
			{
				Swap(vw.vw_afWeights[1], vw.vw_afWeights[0]);
				Swap(vw.vw_aiIndices[1], vw.vw_aiIndices[0]);
			}
			ASSERT(vw.vw_afWeights[0]>=vw.vw_afWeights[1] &&
				vw.vw_afWeights[1]>=vw.vw_afWeights[2] &&
				vw.vw_afWeights[2]>=vw.vw_afWeights[3] && 
				vw.vw_afWeights[0]>=vw.vw_afWeights[3]);
		}
	}
	
	mlod.mlod_aVertexWeights.Clear();
	mlod.mlod_aVertexWeights.New(ctmvx);
	memset(&mlod.mlod_aVertexWeights[0],0,sizeof(MeshVertexWeightInfo)*ctmvx);
	
	INDEX ctSurfaceReadyForVP = 0;
	
	// for each surface in mesh lod
	INDEX ctmsrf = mlod.mlod_aSurfaces.Count();
	INDEX imsrf;
	for( imsrf = 0; imsrf < ctmsrf; imsrf++ )
	{
		MeshSurface &msrf = mlod.mlod_aSurfaces[imsrf];
		msrf.msrf_aubRelIndexTable.Clear();
		
		// for each vertex in surface weight map
		const INDEX iFirstVertex = msrf.msrf_iFirstVertex;
		const INDEX ctVtx = msrf.msrf_ctVertices;
		// prepare vertex weight infos for hardware vertex shaders
		for(INDEX imvx=0;imvx<ctVtx;imvx++) 
		{
			MeshVertexWeightInfo &mvwi = mlod.mlod_aVertexWeights[imvx + iFirstVertex];
			VertexWeights &vw = aVertexWeights[imvx + iFirstVertex];
			INDEX ctwpv = actWeightsPerVertex[imvx + iFirstVertex];
			ASSERT(ctwpv<=4);
			
			FLOAT fTotalWeights = 0;
			INDEX	iwpv;
			for( iwpv = 0; iwpv < ctwpv; iwpv++ )
			{
				fTotalWeights+=vw.vw_afWeights[iwpv];
			}
			
			for( iwpv = 0; iwpv < ctwpv; iwpv++)
			{
				ASSERT(vw.vw_aiIndices[iwpv]>0 || vw.vw_afWeights[iwpv]>0.0f);
				const INDEX iAbsWMapIndex = vw.vw_aiIndices[iwpv];
				const INDEX imwm = GetRelativeWeightIndex(mlod,msrf,iAbsWMapIndex);
				// ASSERT(msrf.msrf_aubRelIndexTable.Count()<=MAX_WEIGHTS_PER_SURFACE);
				// Normalize weights 
				FLOAT fmvw = vw.vw_afWeights[iwpv] / fTotalWeights;
				ASSERT(fmvw>=0.0f && fmvw<=1.0f);
				ASSERT(imwm<=255);
				// clamp vertex factor to 1.0f
				UBYTE ubWeight = NormFloatToByte(fmvw);
				mvwi.mvwi_aubIndices[iwpv] = imwm;
				mvwi.mvwi_aubWeights[iwpv] = ubWeight;
			}
#if _DEBUG
			UBYTE ubLast = 255;
			SLONG slTotal = 0;
			for( iwpv = 0; iwpv < ctwpv; iwpv++ )
			{
				if(mvwi.mvwi_aubWeights[iwpv]>0) 
				{
					ASSERT(mvwi.mvwi_aubWeights[iwpv]<=ubLast);
					ubLast = mvwi.mvwi_aubWeights[iwpv];
					slTotal+=ubLast;
				}
			}
#endif
		}
		if(msrf.msrf_aubRelIndexTable.Count()<=MAX_WEIGHTS_PER_SURFACE) 
		{
			ctSurfaceReadyForVP++;
		}
		else
		{
			//CPrintF("Bone count per surface is %d, this must be less or equal 25.", msrf.msrf_aubRelIndexTable.Count());
			CPrintF("표면당 본의 개수가 %d개입니다. 표면당 뼈개수는 25개 이하여야 합니다.", msrf.msrf_aubRelIndexTable.Count());
		}
	}
	
	NormalizeWeightsInfosInLod(mlod);
	mlod.mlod_ulFlags|=ML_NORMALIZED_WEIGHTS;
	
#if _DEBUG
	ValidateWeights(mlod);
#endif
	if(ctSurfaceReadyForVP==ctmsrf) 
	{
		mlod.mlod_ulFlags|=ML_USE_VERTEX_PROGRAM;
	}
}

// Add mesh lod to mesh
void CMesh::AddMeshLod(MeshLOD &mlod)
{
	INDEX ctmlod = msh_aMeshLODs.Count();
	msh_aMeshLODs.Expand(ctmlod+1);
	msh_aMeshLODs[ctmlod] = mlod;
}

// remove mesh lod from mesh
void CMesh::RemoveMeshLod(MeshLOD *pmlodRemove)
{
	INDEX ctmlod = msh_aMeshLODs.Count();
	// create temp space for skeleton lods
	CStaticArray<struct MeshLOD> aTempmlods;
	aTempmlods.New(ctmlod-1);
	INDEX iIndexSrc=0;
	
	// for each skeleton lod in skeleton
	for(INDEX imlod=0;imlod<ctmlod;imlod++)
	{
		MeshLOD *pmlod = &msh_aMeshLODs[imlod];
		// copy all skeleton lods except the selected one
		if(pmlod != pmlodRemove)
		{
			aTempmlods[iIndexSrc] = *pmlod;
			iIndexSrc++;
		}
	}
	// copy temp array of mesh lods back in skeleton
	msh_aMeshLODs.CopyArray(aTempmlods);
	// clear temp skleletons lods array
	aTempmlods.Clear();
}


static CShader _shDummyShader;            // dummy shader if shader is not found
static ShaderParams _shpDummyShaderParams;// dummy shader params if shader is not found

extern BOOL _bSkaStudioApp;

// Read mesh from stream
//안태훈 수정 시작	//(Encode Data)(0.1)
#define CODE_MESH_1 13
#define CODE_MESH_2 23
#define CODE_MESH_3 19
#define CODE_MESH_4 29
#define CODE_MESH_5 5

template<class Type>
inline Type EncodeSimpleMesh(Type val, UBYTE &ubChecker)
{
	if(sizeof(Type) == 4)
	{
		//ULONG ulChecker = ((ubChecker += (UBYTE)CODE_1) << 24)
						//| ((ubChecker += (UBYTE)CODE_2) << 16)
						//| ((ubChecker += (UBYTE)CODE_3) << 8 )
						//| ((ubChecker += (UBYTE)CODE_4) << 0 );
		ULONG ulChecker = 0;
		ubChecker += (UBYTE)CODE_MESH_1;
		ulChecker |= ULONG(ubChecker) << 24;
		ubChecker += (UBYTE)CODE_MESH_2;
		ulChecker |= ULONG(ubChecker) << 16;
		ubChecker += (UBYTE)CODE_MESH_3;
		ulChecker |= ULONG(ubChecker) << 8;
		ubChecker += (UBYTE)CODE_MESH_4;
		ulChecker |= ULONG(ubChecker) << 0;
		ubChecker += CODE_MESH_5;
		return Type( val ^ ulChecker);
	}
	else return val;
}

template<class Type>
inline Type DecodeSimpleMesh(Type val, UBYTE &ubChecker)
{
	if(sizeof(Type) == 4)
	{
		//ULONG ulChecker = ((ubChecker += (UBYTE)CODE_1) << 24)
						//| ((ubChecker += (UBYTE)CODE_2) << 16)
						//| ((ubChecker += (UBYTE)CODE_3) << 8 )
						//| ((ubChecker += (UBYTE)CODE_4) << 0 );
		ULONG ulChecker = 0;
/*
		ubChecker += 84;
		ulChecker |= ULONG(ubChecker) << 24;
		ulChecker |= ULONG(ubChecker) << 16;
		ulChecker |= ULONG(ubChecker) << 8;
		ulChecker |= ULONG(ubChecker) << 0;
*/
		//Error Versi0on
		
		ubChecker += (UBYTE)CODE_MESH_1;
		ulChecker |= ULONG(ubChecker) << 24;
		ubChecker += (UBYTE)CODE_MESH_2;
		ulChecker |= ULONG(ubChecker) << 16;
		ubChecker += (UBYTE)CODE_MESH_3;
		ulChecker |= ULONG(ubChecker) << 8;
		ubChecker += (UBYTE)CODE_MESH_4;
		ulChecker |= ULONG(ubChecker) << 0;
		ubChecker += CODE_MESH_5;
		return Type( val ^ ulChecker);
	}
	else return val;
}

void CMesh::Read_t(CTStream *istrFile)
{
	// Clear old mesh if it exists
	Clear();
	// determine driver context presence (must have at least 1 texture unit!)
	const BOOL bHasContext = (_pGfx->gl_ctRealTextureUnits>0);
	
	INDEX iFileVersion;
	// read chunk id
	istrFile->ExpectID_t(CChunkID(MESH_ID));
	// check file version
	(*istrFile) >> iFileVersion;

	// if file version is invalid
	if(iFileVersion == MESH_VERSION) Read_t_old(istrFile, bHasContext);
	else if(iFileVersion == MESH_NEW_VER) Read_t_new(istrFile, bHasContext);
	else
	{
		ThrowF_t(TRANS("File '%s'.\nInvalid Mesh file version.\nExpected Ver \"%d or %d\" but found \"%d\"\n"),
			(const char*)istrFile->GetDescription(),MESH_VERSION, MESH_NEW_VER, iFileVersion);
	}
}

void CMesh::Read_t_new(CTStream *istrFile, BOOL bHasContext)
{
	UBYTE checker = MESH_NEW_VER;
	// Read size of mesh
	SLONG slMeshSize = 0;
	(*istrFile) >> slMeshSize;
	
	// if no driver available skip mesh loading
	if(!bHasContext) 
	{
		istrFile->Seek_t(slMeshSize,CTStream::SD_CUR);
		return;
	}
	
	INDEX ctmlod;
	// read count of mesh lods
	(*istrFile) >> ctmlod;		ctmlod = DecodeSimpleMesh(ctmlod, checker);
	if(ctmlod>0) 
	{
		// Create array of mesh lods
		msh_aMeshLODs.New(ctmlod);
	}
	
	// for each LOD in mesh
	for(INDEX imlod=0;imlod<ctmlod;imlod++) 
	{
		BOOL bHaveTangentStream = FALSE;
		// expand mlod count for one 
		MeshLOD &mlod = msh_aMeshLODs[imlod];
		INDEX ctmvx;
		INDEX ctmuvm;
		INDEX ctmsrf;
		INDEX ctmwm;
		INDEX ctmmm;
		INDEX ctmvwi;
		
		// read component count
		(*istrFile) >> ctmvwi;		ctmvwi = DecodeSimpleMesh(ctmvwi, checker);
		(*istrFile) >> ctmwm;		ctmwm = DecodeSimpleMesh(ctmwm, checker);
		(*istrFile) >> ctmuvm;		ctmuvm = DecodeSimpleMesh(ctmuvm, checker);
		(*istrFile) >> ctmvx;		ctmvx = DecodeSimpleMesh(ctmvx, checker);
		(*istrFile) >> ctmsrf;		ctmsrf = DecodeSimpleMesh(ctmsrf, checker);
		(*istrFile) >> ctmmm;		ctmmm = DecodeSimpleMesh(ctmmm, checker);
		
		const BOOL bHasWeights = ctmvwi>0;
		const BOOL bHasUVMaps  = ctmuvm>0;
		
		ULONG ulBufferFlags = GFX_VBM_POS|GFX_VBM_NOR;
		// Create weight buffer if mesh has vertex weight infos
		if(bHasWeights) 
		{
			ulBufferFlags|=GFX_VBM_WGH;
		}
		// Create texcoord buffer if mesh has tex coords
		if(bHasUVMaps) 
		{
			ulBufferFlags|=GFX_VBM_TEX;
		}
		
		// read source file name
		(*istrFile) >> mlod.mlod_fnSourceFile;
		// read max distance
		(*istrFile) >> mlod.mlod_fMaxDistance;
		// read flags
		(*istrFile) >> mlod.mlod_ulFlags;			mlod.mlod_ulFlags = DecodeSimpleMesh(mlod.mlod_ulFlags, checker);
		
		// create vertex and normal arrays
		mlod.mlod_aVertices.New(ctmvx);
		mlod.mlod_aNormals.New(ctmvx);
		
		// read vertices and normals
		istrFile->Read_t( &mlod.mlod_aVertices[0], sizeof(MeshVertex)*ctmvx);
		istrFile->Read_t( &mlod.mlod_aNormals[0],  sizeof(MeshNormal)*ctmvx);
		
		// Create array of uvmaps
		mlod.mlod_aUVMaps.New(ctmuvm);
		// read uvmaps
		INDEX	imuvm;
		for( imuvm = 0; imuvm < ctmuvm; imuvm++ )
		{
			MeshUVMap &muvm = mlod.mlod_aUVMaps[imuvm];
			// Read uvmap name
			CTString strUVMap;
			(*istrFile) >> strUVMap;
			muvm.muv_iID = ska_StringToID(strUVMap);
			// Reset texcoord buffer ID
			muvm.muvm_iTexCoordBufferID = -1;
			// Allocate array for uvmaps texcordinates
			muvm.muv_aTexCoords.New(ctmvx);
			// Read uvmap texcordinates
			istrFile->Read_t(&mlod.mlod_aUVMaps[imuvm].muv_aTexCoords[0],sizeof(MeshTexCoord)*ctmvx);
		}
		
		// Create array of surfaces
		mlod.mlod_aSurfaces.New(ctmsrf);
		// read surfaces
		for(INDEX imsrt=0;imsrt<ctmsrf;imsrt++) 
		{
			INDEX ctTris;
			MeshSurface &msrf = mlod.mlod_aSurfaces[imsrt];
			// read first vertex
			(*istrFile) >> msrf.msrf_iFirstVertex;		msrf.msrf_iFirstVertex = DecodeSimpleMesh(msrf.msrf_iFirstVertex, checker);
			// read vertices count
			(*istrFile) >> msrf.msrf_ctVertices;		msrf.msrf_ctVertices = DecodeSimpleMesh(msrf.msrf_ctVertices, checker);

			// read tris count
			(*istrFile) >> ctTris;		ctTris = DecodeSimpleMesh(ctTris, checker);
			// create triangles array
			msrf.msrf_auwTriangles.New(ctTris);
			// read surface triangles
			istrFile->Read_t( &msrf.msrf_auwTriangles[0], sizeof(MeshTriangle)*ctTris);

			// Read surface name
			CTString strSurface;
			(*istrFile) >> strSurface;
			msrf.msrf_iSurfaceID = ska_StringToID(strSurface);
			// read surface flags
			(*istrFile) >> msrf.msrf_ulFlags;		msrf.msrf_ulFlags = DecodeSimpleMesh(msrf.msrf_ulFlags, checker);
			
			// read table of surface relative weight map indices
			INDEX ctirt;
			(*istrFile) >> ctirt;		ctirt = DecodeSimpleMesh(ctirt, checker);
			if(ctirt>0) 
			{
				msrf.msrf_aubRelIndexTable.New(ctirt);
				istrFile->Read_t(&msrf.msrf_aubRelIndexTable[0], sizeof(UBYTE)*ctirt);
			}
			
			// Read bool that this surface has a shader
			INDEX bShaderExists;
			(*istrFile) >> bShaderExists;		bShaderExists = DecodeSimpleMesh(bShaderExists, checker);
			// if shader exists read its params
			if(bShaderExists) 
			{
				INDEX cttx,cttc,ctcol,ctfl;
				// read color count
				(*istrFile) >> ctcol;		ctcol = DecodeSimpleMesh(ctcol, checker);
				// read float count
				(*istrFile) >> ctfl;		ctfl = DecodeSimpleMesh(ctfl, checker);
				// read texture count
				(*istrFile) >> cttx;		cttx = DecodeSimpleMesh(cttx, checker);
				// read texture coords count
				(*istrFile) >> cttc;		cttc = DecodeSimpleMesh(cttc, checker);
				
				CShader *pshMeshShader = NULL;
				ShaderParams *pshpShaderParams = NULL;
				// read shader name
				CTString strShaderName;
				(*istrFile) >> strShaderName;
				// try to load shader
				try
				{
					msrf.msrf_pShader = _pShaderStock->Obtain_t(strShaderName);
					pshMeshShader = msrf.msrf_pShader;
					pshpShaderParams = &msrf.msrf_ShadingParams;
				}
				catch(char *strErr) 
				{
					CPrintF("Model: %s - %s\n", mlod.mlod_fnSourceFile, strErr);
					msrf.msrf_pShader = NULL;
					pshMeshShader = &_shDummyShader;
					pshpShaderParams = &_shpDummyShaderParams;
				}
				
				// if mesh shader exisits
				if(msrf.msrf_pShader!=NULL) 
				{
					// get shader description
//안태훈 수정 시작	//(For Performance)(0.1)
					ShaderDesc *pshDesc;
					msrf.msrf_pShader->GetShaderDesc(pshDesc);
					ShaderDesc &shDesc = *pshDesc;
//안태훈 수정 끝	//(For Performance)(0.1)
					BOOL bParamsFailed = FALSE;
					if(shDesc.sd_astrTextureNames.Count() != cttx) 
					{
						CPrintF("File '%s'\nWrong texture count %d",(const char*)GetName(),cttx);
						bParamsFailed = TRUE;
					}
					if(shDesc.sd_astrTexCoordNames.Count() != cttc) 
					{
						CPrintF("File '%s'\nWrong uvmaps count %d",(const char*)GetName(),cttc);
						bParamsFailed = TRUE;
					}
					if(shDesc.sd_astrColorNames.Count() != ctcol) 
					{
						CPrintF("File '%s'\nWrong colors count %d",(const char*)GetName(),ctcol);
						bParamsFailed = TRUE;
					}
					if(shDesc.sd_astrFloatNames.Count() != ctfl) 
					{
						CPrintF("File '%s'\nWrong floats count %d",(const char*)GetName(),ctfl);
						bParamsFailed = TRUE;
					}
					// if saved shader params do not match current shader params
					if(bParamsFailed) 
					{
						// Discard shader
						_pShaderStock->Release(msrf.msrf_pShader);
						msrf.msrf_pShader = NULL;
						// Use dummy shader for futher loading
						pshMeshShader = &_shDummyShader;
//안태훈 수정 시작	//(For Performance)(0.1)
						pshMeshShader->GetShaderDesc(pshDesc);
						ShaderDesc &shDesc = *pshDesc;
//안태훈 수정 끝	//(For Performance)(0.1)
						pshpShaderParams = &_shpDummyShaderParams;
					}
					for(INDEX i=0; i<shDesc.sd_ulStreamFlags.Count(); ++i)
						bHaveTangentStream = bHaveTangentStream || (shDesc.sd_ulStreamFlags[i] & GFX_TANGENT_STREAM);
					/* check if saved params count match shader params count
					if(shDesc.sd_astrTextureNames.Count() != cttx) ThrowF_t("File '%s'\nWrong texture count %d",(const char*)GetName(),cttx);
					if(shDesc.sd_astrTexCoordNames.Count() != cttc) ThrowF_t("File '%s'\nWrong uvmaps count %d",(const char*)GetName(),cttc);
					if(shDesc.sd_astrColorNames.Count() != ctcol) ThrowF_t("File '%s'\nWrong colors count %d",(const char*)GetName(),ctcol);
					if(shDesc.sd_astrFloatNames.Count() != ctfl) ThrowF_t("File '%s'\nWrong floats count %d",(const char*)GetName(),ctfl);
					*/
				}
				
				if(pshpShaderParams->sp_aiTextureIDs.Count()>0)     pshpShaderParams->sp_aiTextureIDs.Clear();
				if(pshpShaderParams->sp_aiTexCoordsIndex.Count()>0) pshpShaderParams->sp_aiTexCoordsIndex.Clear();
				if(pshpShaderParams->sp_acolColors.Count()>0)       pshpShaderParams->sp_acolColors.Clear();
				if(pshpShaderParams->sp_afFloats.Count()>0)         pshpShaderParams->sp_afFloats.Clear();
				// create arrays for shader params
				pshpShaderParams->sp_aiTextureIDs.New(cttx);
				pshpShaderParams->sp_aiTexCoordsIndex.New(cttc);
				pshpShaderParams->sp_acolColors.New(ctcol);
				pshpShaderParams->sp_afFloats.New(ctfl);
				
				// read shader texture IDs
				for(INDEX itx=0;itx<cttx;itx++) 
				{
					CTString strTexID;
					(*istrFile) >> strTexID;
					INDEX iTexID = -1;
					if(strTexID!="") 
					{
						iTexID = ska_GetIDFromStringTable(strTexID);
					}
					pshpShaderParams->sp_aiTextureIDs[itx] = iTexID;
				}
				// read shader flags
				ULONG ulFlags;
				(*istrFile) >> ulFlags;		ulFlags = DecodeSimpleMesh(ulFlags, checker);
				pshpShaderParams->sp_ulFlags = ulFlags;
				// read shader colors
				for(INDEX icol=0;icol<ctcol;icol++) 
				{
					COLOR colColor;
					(*istrFile) >> colColor;		colColor = DecodeSimpleMesh(colColor, checker);
					pshpShaderParams->sp_acolColors[icol] = colColor;
				}
				// read shader floats
				for(INDEX ifl=0;ifl<ctfl;ifl++) 
				{
					FLOAT fFloat;
					(*istrFile) >> fFloat;
					pshpShaderParams->sp_afFloats[ifl] = fFloat;
				}
				// read shader texture coords indices
				for(INDEX itc=0;itc<cttc;itc++) 
				{
					INDEX iTexCoorsIndex;
					(*istrFile) >> iTexCoorsIndex;		iTexCoorsIndex = DecodeSimpleMesh(iTexCoorsIndex, checker);
					pshpShaderParams->sp_aiTexCoordsIndex[itc] = iTexCoorsIndex;
				}
			}
			else 
			{
				// this surface does not have shader
				msrf.msrf_pShader=NULL;
			}
		}
		
		if(ctmwm) 
		{
			// create array of weightmaps
			mlod.mlod_aWeightMaps.New(ctmwm);
			// Read each weightmap
			for(INDEX imwm=0;imwm<ctmwm;imwm++) 
			{
				MeshWeightMap &mwm = mlod.mlod_aWeightMaps[imwm];
				// Read weightmap name
				CTString strWeightMap;
				(*istrFile) >> strWeightMap;
				mwm.mwm_iID = ska_StringToID(strWeightMap);
				
				// Read vertex weight count
				INDEX ctmvw;
				(*istrFile) >> ctmvw;		ctmvw = DecodeSimpleMesh(ctmvw, checker);
				// Read vertex weights
				if(ctmvw>0) 
				{
					SLONG slSize = sizeof(MeshVertexWeight)*ctmvw;
					// Create vertex weight array
					mwm.mwm_aVertexWeight.New(ctmvw);
					// Read vertex weights
					istrFile->Read_t(&mwm.mwm_aVertexWeight[0],slSize);
				}
			}
		}
		
		// Create morphmaps array
		if(ctmmm>0) 
		{
			mlod.mlod_aMorphMaps.New(ctmmm);
			// read morphmaps
			for(INDEX immm=0;immm<ctmmm;immm++) 
			{
				MeshMorphMap &mmm = mlod.mlod_aMorphMaps[immm];
				// read morphmap ID
				CTString strMorphMap;
				(*istrFile) >> strMorphMap;
				mmm.mmp_iID = ska_StringToID(strMorphMap);
				// read bRelative
				(*istrFile) >> mmm.mmp_bRelative;		mmm.mmp_bRelative = DecodeSimpleMesh(mmm.mmp_bRelative, checker);
				// read morph sets count
				INDEX ctmvm;
				(*istrFile) >> ctmvm;		ctmvm = DecodeSimpleMesh(ctmvm, checker);
				// create morps sets array
				mmm.mmp_aMorphMap.New(ctmvm);
				// read morph sets
				istrFile->Read_t(&mmm.mmp_aMorphMap[0],sizeof(MeshVertexMorph)*ctmvm);
			}
		}
		
		// Read vertex weight infos
		if(ctmvwi>0) 
		{
			mlod.mlod_aVertexWeights.New(ctmvwi);
			istrFile->Read_t(&mlod.mlod_aVertexWeights[0],sizeof(MeshVertexWeightInfo)*ctmvwi);
		}
		
		// If no context skip vertex buffers
		if(!bHasContext) 
		{
			continue;
		}
		
		//bm을 로딩할때 그 이름을 확인하여 _TAG_가 접두로 들어가 있으면
		//그 모든 vertex를 Ska Tag로 추가한다.
		mlod.mlod_aSkaTags.Clear();
		INDEX iSkaTag = 0;
		for(INDEX iSurface=0; iSurface<mlod.mlod_aSurfaces.Count(); ++iSurface) 
		{
			MeshSurface &surface = mlod.mlod_aSurfaces[iSurface];
			CTString strName = ska_IDToString(surface.msrf_iSurfaceID);
			VERTEX_TAG_CONVERT_TYPE vtct = VTCT_VERTEX;
			static CTString strTagIndicator(TAG_INDICATOR_STRING);
			static CTString strTagVtxIndicator(TAG_V_INDICATOR_STRING);
			static CTString strTagTriIndicator(TAG_T_INDICATOR_STRING);
			static CTString strTagGrpIndicator(TAG_G_INDICATOR_STRING);
			
			if( strName.RemovePrefix(strTagVtxIndicator) ) vtct = VTCT_VERTEX;
			else if( strName.RemovePrefix(strTagTriIndicator) ) vtct = VTCT_TRIANGLE;
			else if( strName.RemovePrefix(strTagGrpIndicator) ) vtct = VTCT_GROUP;
			else if( !strName.RemovePrefix(strTagIndicator) ) continue;	//기본값이 VTCT_VERTEX일때

			if(vtct == VTCT_GROUP)
			{
				static CTString strZero("0_");
				static CTString strOne("1_");
				static CTString strTwo("2_");
				static CTString strUBUB("__");
				if( strName.HasPrefix(strZero) || strName.HasPrefix(strOne) || strName.HasPrefix(strTwo) )
				{
					strName.InsertSubString(0, strUBUB);
				}
				else continue;
			}

			if(vtct == VTCT_VERTEX)
				mlod.mlod_aSkaTags.Expand( mlod.mlod_aSkaTags.Count() + surface.msrf_ctVertices );
			else if(vtct == VTCT_TRIANGLE || vtct == VTCT_GROUP)	//항상 하나의 Tag만 추가.
				mlod.mlod_aSkaTags.Expand( mlod.mlod_aSkaTags.Count() + 1 );
				//mlod.mlod_aSkaTags.Expand( mlod.mlod_aSkaTags.Count() + INDEX(floorf(surface.msrf_ctVertices/3.0f)) );

			INDEX checkTri = 1;	//하나만 만들게끔 보장.
			for(INDEX iVertex=surface.msrf_iFirstVertex; iVertex < surface.msrf_iFirstVertex+surface.msrf_ctVertices; ++iVertex)
			{
				if( (vtct == VTCT_TRIANGLE || vtct == VTCT_GROUP) && checkTri == 0) break;
				--checkTri;
				FLOAT3D		vTagPos;
				FLOATquat3D	qTagRot;
				MeshVertex &mv = mlod.mlod_aVertices[ iVertex ];
				CSkaTag *ptrTag = new CSkaTag;
				mlod.mlod_aSkaTags[ iSkaTag ] = ptr_tag(ptrTag);
				++iSkaTag;
				if(mlod.mlod_aVertexWeights.Count() > 0)
				{
					MeshVertexWeightInfo &mvwi = mlod.mlod_aVertexWeights[ iVertex ];
					INDEX index;
					for(index = 0; index < 4 && mvwi.mvwi_aubWeights[index] > 0; ++index) NULL;
					ptrTag->SetBoneBlendValue(&mlod, index
						, surface.msrf_aubRelIndexTable[ ( mlod.mlod_aVertexWeights[iVertex].mvwi_aubIndices[0] ) ]
						, NormByteToFloat(mlod.mlod_aVertexWeights[ iVertex ].mvwi_aubWeights[0])
						, surface.msrf_aubRelIndexTable[ ( mlod.mlod_aVertexWeights[iVertex].mvwi_aubIndices[1] ) ]
						, NormByteToFloat(mlod.mlod_aVertexWeights[ iVertex ].mvwi_aubWeights[1])
						, surface.msrf_aubRelIndexTable[ ( mlod.mlod_aVertexWeights[iVertex].mvwi_aubIndices[2] ) ]
						, NormByteToFloat(mlod.mlod_aVertexWeights[ iVertex ].mvwi_aubWeights[2])
						, surface.msrf_aubRelIndexTable[ ( mlod.mlod_aVertexWeights[iVertex].mvwi_aubIndices[3] ) ]
						, NormByteToFloat(mlod.mlod_aVertexWeights[ iVertex ].mvwi_aubWeights[3])
					);
				}
				else
				{
					ptrTag->SetBoneBlendValue(&mlod);
				}
				std::string strNewName(strName.str_String);
				char buf[16] = {0};
				if(vtct == VTCT_VERTEX) strNewName += itoa(iVertex, buf, 10);
				ptrTag->SetName(strNewName);
				ptrTag->SetOffsetPos( mlod.mlod_aVertices[iVertex].x, mlod.mlod_aVertices[iVertex].y, mlod.mlod_aVertices[iVertex].z );
				FLOAT3D normal, axis(0,1,0);
				if(mlod.mlod_aNormals.Count())
				{
					normal = FLOAT3D(mlod.mlod_aNormals[iVertex].nx, mlod.mlod_aNormals[iVertex].ny, mlod.mlod_aNormals[iVertex].nz);
					FLOAT cosVal = axis % normal;
					FLOAT angle = acos( cosVal );
					FLOAT sinVal = sin( angle );
					if(angle > 0.001f)
					{
						axis *= normal;
						axis.Normalize();
						FLOATquat3D quat;
						quat.FromAxisAngle(axis, angle);
						ptrTag->SetOffsetRot( quat );
					}
					else
					{
						ptrTag->SetOffsetRot( FLOATquat3D(1,0,0,0) );
					}
				}
				else
				{
					ptrTag->SetOffsetRot( FLOATquat3D(1,0,0,0) );
				}
			}
		}
		//Tagent를 생성한다.
		BOOL bHasTangent = FALSE;
		if(mlod.mlod_aUVMaps.Count() > 0 && (bHaveTangentStream || _bSkaStudioApp))
		{
			if( mlod.mlod_aVertices.Count() == mlod.mlod_aUVMaps[0].muv_aTexCoords.Count()
				&& mlod.mlod_aVertices.Count() == mlod.mlod_aNormals.Count())
			{
				bHasTangent = TRUE;
			}
			else
			{
				ASSERT(FALSE && "Tagent 계산을 위해서는 Vertex 위치와 노말, 텍스쳐 좌표[0]의 수는 일치해야 한다.");
			}
		}

		if(bHasTangent)
		{
			CalcVertexTangent(mlod, 0);
			ctmvx = mlod.mlod_aVertices.Count();
		}
		
		//-------------------- Loading은 끝, Vertex Buffer 생성은 시작 ---------------//
		// Disable d3d access in second thread
		
		// Create vertex buffers for meshlod
		INDEX &iBufferID = mlod.mlod_iBufferBindID;
		if(bHasTangent) ulBufferFlags |= GFX_VBM_TAN;
		iBufferID = gfxCreateVertexBuffer(ctmvx,ulBufferFlags,ctmuvm,GFX_READWRITE);
		ASSERT(iBufferID>=0);
		
		// Get vertex buffer ids
		mlod.mlod_iVertexBufferID = gfxGetVertexSubBufferID(iBufferID,GFX_VBA_POS);
		mlod.mlod_iNormalBufferID = gfxGetVertexSubBufferID(iBufferID,GFX_VBA_NOR);
		ASSERT(mlod.mlod_iVertexBufferID>=0);
		ASSERT(mlod.mlod_iNormalBufferID>=0);
		
		// Get weight buffer id if weight buffer exists
		if(bHasWeights) 
		{
			mlod.mlod_iWeightBufferID = gfxGetVertexSubBufferID(iBufferID,GFX_VBA_WGH);
			ASSERT(mlod.mlod_iWeightBufferID>=0);
		}
		
		// Get texcoord buffer ids
		for( imuvm = 0; imuvm < ctmuvm; imuvm++)
		{
			MeshUVMap &muvm = mlod.mlod_aUVMaps[imuvm];
			muvm.muvm_iTexCoordBufferID = gfxGetVertexSubBufferID(iBufferID,GFX_VBA_TEX+imuvm);
			ASSERT(muvm.muvm_iTexCoordBufferID>=0);
		}
		
		if(bHasTangent)
		{
			mlod.mlod_iTangentBufferID = gfxGetVertexSubBufferID(iBufferID, GFX_VBA_TAN);
			ASSERT(mlod.mlod_iTangentBufferID>=0);
		}

		ASSERT(sizeof(MeshVertex)==sizeof(GFXVertex));
		ASSERT(sizeof(MeshNormal)==sizeof(GFXNormal));
		ASSERT(sizeof(MeshTexCoord)==sizeof(GFXTexCoord));
		ASSERT(sizeof(MeshTangent)==sizeof(GFXTangent));
		
		// TEMP: Fill vertex buffers
		GFXVertex *pavVertices = (GFXVertex*)gfxLockVertexSubBuffer(mlod.mlod_iVertexBufferID,0,ctmvx,GFX_WRITE);
		memcpy(pavVertices,&mlod.mlod_aVertices[0],sizeof(GFXVertex)*ctmvx);
		gfxUnlockVertexSubBuffer(mlod.mlod_iVertexBufferID, 0);
		
		GFXNormal *panNormals  = (GFXNormal*)gfxLockNormalSubBuffer(mlod.mlod_iNormalBufferID,0,ctmvx,GFX_WRITE);
		memcpy(panNormals,&mlod.mlod_aNormals[0],sizeof(GFXNormal)*ctmvx);
		gfxUnlockNormalSubBuffer(mlod.mlod_iNormalBufferID, 0);
		
		if(bHasWeights) 
		{
			GFXWeight *pawWeights = (GFXWeight*)gfxLockWeightSubBuffer(mlod.mlod_iWeightBufferID,0,ctmvx,GFX_WRITE);
			memcpy(pawWeights,&mlod.mlod_aVertexWeights[0],sizeof(GFXWeight)*ctmvx);
			gfxUnlockWeightSubBuffer(mlod.mlod_iWeightBufferID, 0);
		}
		
		for( imuvm = 0; imuvm < ctmuvm; imuvm++)
		{
			const MeshUVMap &muvm = mlod.mlod_aUVMaps[imuvm];
			GFXTexCoord *patcTexCoords = (GFXTexCoord*)gfxLockTexCoordSubBuffer(muvm.muvm_iTexCoordBufferID,0,ctmvx,GFX_WRITE);
			memcpy(patcTexCoords,&muvm.muv_aTexCoords[0],sizeof(GFXTexCoord)*ctmvx);
			gfxUnlockTexCoordSubBuffer(muvm.muvm_iTexCoordBufferID, 0);
		}
		
		if(bHasTangent)
		{
			GFXTangent *panTangents  = (GFXTangent*)gfxLockTangentSubBuffer(mlod.mlod_iTangentBufferID, 0, ctmvx, GFX_WRITE);
			memcpy(panTangents, &mlod.mlod_aTangents[0], sizeof(GFXTangent)*ctmvx);
			gfxUnlockTangentSubBuffer(mlod.mlod_iTangentBufferID, 0);
		}

		extern BOOL _bKeepVertexArrays;
		// free vertex arrays if alowed
		if(!_bKeepVertexArrays) 
		{
			mlod.mlod_aVertices.Clear();
			mlod.mlod_aNormals.Clear();
			mlod.mlod_aTangents.Clear();
			mlod.mlod_aVertexWeights.Clear();
			const INDEX ctmuvm = mlod.mlod_aUVMaps.Count();
			for(INDEX imuvm=0;imuvm<ctmuvm;imuvm++) 
			{
				MeshUVMap &muvm = mlod.mlod_aUVMaps[imuvm];
				muvm.muv_aTexCoords.Clear();
			}
		}
		
	}
}

void CMesh::Read_t_old(CTStream *istrFile, BOOL bHasContext)
{
//안태훈 수정 끝	//(Encode Data)(0.1)
	// Read size of mesh
	SLONG slMeshSize = 0;
	(*istrFile)>>slMeshSize;
	
	// if no driver available skip mesh loading
	if(!bHasContext) 
	{
		istrFile->Seek_t(slMeshSize,CTStream::SD_CUR);
		return;
	}
	
	INDEX ctmlod;
	// read count of mesh lods
	(*istrFile)>>ctmlod;
	if(ctmlod>0) 
	{
		// Create array of mesh lods
		msh_aMeshLODs.New(ctmlod);
	}
	
	// for each LOD in mesh
	for(INDEX imlod=0;imlod<ctmlod;imlod++) 
	{
//안태훈 수정 시작	//(Add Tagent-space Normal Map)(0.1)
		BOOL bHaveTangentStream = FALSE;
//안태훈 수정 끝	//(Add Tagent-space Normal Map)(0.1)
		// expand mlod count for one 
		MeshLOD &mlod = msh_aMeshLODs[imlod];
		INDEX ctmvx;
		INDEX ctmuvm;
		INDEX ctmsrf;
		INDEX ctmwm;
		INDEX ctmmm;
		INDEX ctmvwi;
		
		// read component count
		(*istrFile)>>ctmvx;
		(*istrFile)>>ctmuvm;
		(*istrFile)>>ctmsrf;
		(*istrFile)>>ctmwm;
		(*istrFile)>>ctmmm;
		(*istrFile)>>ctmvwi;
		
		const BOOL bHasWeights = ctmvwi>0;
		const BOOL bHasUVMaps  = ctmuvm>0;
		
		ULONG ulBufferFlags = GFX_VBM_POS|GFX_VBM_NOR;
		// Create weight buffer if mesh has vertex weight infos
		if(bHasWeights) 
		{
			ulBufferFlags|=GFX_VBM_WGH;
		}
		// Create texcoord buffer if mesh has tex coords
		if(bHasUVMaps) 
		{
			ulBufferFlags|=GFX_VBM_TEX;
		}
		
		// read source file name
		(*istrFile)>>mlod.mlod_fnSourceFile;
		// read max distance
		(*istrFile)>>mlod.mlod_fMaxDistance;
		// read flags
		(*istrFile)>>mlod.mlod_ulFlags;
		
		// create vertex and normal arrays
		mlod.mlod_aVertices.New(ctmvx);
		mlod.mlod_aNormals.New(ctmvx);
		
		// read vertices and normals
		istrFile->Read_t( &mlod.mlod_aVertices[0], sizeof(MeshVertex)*ctmvx);
		istrFile->Read_t( &mlod.mlod_aNormals[0],  sizeof(MeshNormal)*ctmvx);
		
		// Create array of uvmaps
		mlod.mlod_aUVMaps.New(ctmuvm);
		// read uvmaps
		INDEX imuvm;
		for( imuvm = 0; imuvm < ctmuvm; imuvm++ )
		{
			MeshUVMap &muvm = mlod.mlod_aUVMaps[imuvm];
			// Read uvmap name
			CTString strUVMap;
			(*istrFile)>>strUVMap;
			muvm.muv_iID = ska_StringToID(strUVMap);
			// Reset texcoord buffer ID
			muvm.muvm_iTexCoordBufferID = -1;
			// Allocate array for uvmaps texcordinates
			muvm.muv_aTexCoords.New(ctmvx);
			// Read uvmap texcordinates
			istrFile->Read_t(&mlod.mlod_aUVMaps[imuvm].muv_aTexCoords[0],sizeof(MeshTexCoord)*ctmvx);
		}
		
		// Create array of surfaces
		mlod.mlod_aSurfaces.New(ctmsrf);
		// read surfaces
		for(INDEX imsrt=0;imsrt<ctmsrf;imsrt++) 
		{
			INDEX ctTris;
			MeshSurface &msrf = mlod.mlod_aSurfaces[imsrt];
			// Read surface name
			CTString strSurface;
			(*istrFile)>>strSurface;
			msrf.msrf_iSurfaceID = ska_StringToID(strSurface);
			// read surface flags
			(*istrFile)>>msrf.msrf_ulFlags;
			// read first vertex
			(*istrFile)>>msrf.msrf_iFirstVertex;
			// read vertices count
			(*istrFile)>>msrf.msrf_ctVertices;
			// read tris count
			(*istrFile)>>ctTris;
			// create triangles array
			msrf.msrf_auwTriangles.New(ctTris);
			
			// read surface triangles
			istrFile->Read_t( &msrf.msrf_auwTriangles[0], sizeof(MeshTriangle)*ctTris);
			
			// read table of surface relative weight map indices
			INDEX ctirt;
			(*istrFile)>>ctirt;
			if(ctirt>0) 
			{
				msrf.msrf_aubRelIndexTable.New(ctirt);
				istrFile->Read_t(&msrf.msrf_aubRelIndexTable[0], sizeof(UBYTE)*ctirt);
			}
			
			// Read bool that this surface has a shader
			INDEX bShaderExists;
			(*istrFile)>>bShaderExists;
			// if shader exists read its params
			if(bShaderExists) 
			{
				INDEX cttx,cttc,ctcol,ctfl;
				// read texture count
				(*istrFile)>>cttx;
				// read texture coords count
				(*istrFile)>>cttc;
				// read color count
				(*istrFile)>>ctcol;
				// read float count
				(*istrFile)>>ctfl;
				
				CShader *pshMeshShader = NULL;
				ShaderParams *pshpShaderParams = NULL;
				// read shader name
				CTString strShaderName;
				(*istrFile)>>strShaderName;
				// try to load shader
				try
				{
					msrf.msrf_pShader = _pShaderStock->Obtain_t(strShaderName);
					pshMeshShader = msrf.msrf_pShader;
					pshpShaderParams = &msrf.msrf_ShadingParams;
				}
				catch(char *strErr) 
				{
					CPrintF("Model: %s - %s\n", mlod.mlod_fnSourceFile, strErr);
					msrf.msrf_pShader = NULL;
					pshMeshShader = &_shDummyShader;
					pshpShaderParams = &_shpDummyShaderParams;
				}
				
				// if mesh shader exisits
				if(msrf.msrf_pShader!=NULL) 
				{
					// get shader description
//안태훈 수정 시작	//(For Performance)(0.1)
					ShaderDesc *pshDesc;
					msrf.msrf_pShader->GetShaderDesc(pshDesc);
					ShaderDesc &shDesc = *pshDesc;
//안태훈 수정 끝	//(For Performance)(0.1)
					BOOL bParamsFailed = FALSE;
					if(shDesc.sd_astrTextureNames.Count() != cttx) 
					{
						CPrintF("File '%s'\nWrong texture count %d",(const char*)GetName(),cttx);
						bParamsFailed = TRUE;
					}
					if(shDesc.sd_astrTexCoordNames.Count() != cttc) 
					{
						CPrintF("File '%s'\nWrong uvmaps count %d",(const char*)GetName(),cttc);
						bParamsFailed = TRUE;
					}
					if(shDesc.sd_astrColorNames.Count() != ctcol) 
					{
						CPrintF("File '%s'\nWrong colors count %d",(const char*)GetName(),ctcol);
						bParamsFailed = TRUE;
					}
					if(shDesc.sd_astrFloatNames.Count() != ctfl) 
					{
						CPrintF("File '%s'\nWrong floats count %d",(const char*)GetName(),ctfl);
						bParamsFailed = TRUE;
					}
					// if saved shader params do not match current shader params
//안태훈 수정 시작	//(Add Tagent-space Normal Map)(0.1)
					if(bParamsFailed) 
					{
						// Discard shader
						_pShaderStock->Release(msrf.msrf_pShader);
						msrf.msrf_pShader = NULL;
						// Use dummy shader for futher loading
						pshMeshShader = &_shDummyShader;
						pshpShaderParams = &_shpDummyShaderParams;
					}
					for(INDEX i=0; i<shDesc.sd_ulStreamFlags.Count(); ++i)
					{
						if(!bParamsFailed) bHaveTangentStream = bHaveTangentStream || (shDesc.sd_ulStreamFlags[i] & GFX_TANGENT_STREAM);
					}
//안태훈 수정 끝	//(Add Tagent-space Normal Map)(0.1)
					/* check if saved params count match shader params count
					if(shDesc.sd_astrTextureNames.Count() != cttx) ThrowF_t("File '%s'\nWrong texture count %d",(const char*)GetName(),cttx);
					if(shDesc.sd_astrTexCoordNames.Count() != cttc) ThrowF_t("File '%s'\nWrong uvmaps count %d",(const char*)GetName(),cttc);
					if(shDesc.sd_astrColorNames.Count() != ctcol) ThrowF_t("File '%s'\nWrong colors count %d",(const char*)GetName(),ctcol);
					if(shDesc.sd_astrFloatNames.Count() != ctfl) ThrowF_t("File '%s'\nWrong floats count %d",(const char*)GetName(),ctfl);
					*/
				}
				
				if(pshpShaderParams->sp_aiTextureIDs.Count()>0)     pshpShaderParams->sp_aiTextureIDs.Clear();
				if(pshpShaderParams->sp_aiTexCoordsIndex.Count()>0) pshpShaderParams->sp_aiTexCoordsIndex.Clear();
				if(pshpShaderParams->sp_acolColors.Count()>0)       pshpShaderParams->sp_acolColors.Clear();
				if(pshpShaderParams->sp_afFloats.Count()>0)         pshpShaderParams->sp_afFloats.Clear();
				// create arrays for shader params
				pshpShaderParams->sp_aiTextureIDs.New(cttx);
				pshpShaderParams->sp_aiTexCoordsIndex.New(cttc);
				pshpShaderParams->sp_acolColors.New(ctcol);
				pshpShaderParams->sp_afFloats.New(ctfl);
				
				// read shader texture IDs
				for(INDEX itx=0;itx<cttx;itx++) 
				{
					CTString strTexID;
					(*istrFile)>>strTexID;
					INDEX iTexID = -1;
					if(strTexID!="") 
					{
						iTexID = ska_GetIDFromStringTable(strTexID);
					}
					pshpShaderParams->sp_aiTextureIDs[itx] = iTexID;
				}
				// read shader texture coords indices
				for(INDEX itc=0;itc<cttc;itc++) 
				{
					INDEX iTexCoorsIndex;
					(*istrFile)>>iTexCoorsIndex;
					pshpShaderParams->sp_aiTexCoordsIndex[itc] = iTexCoorsIndex;
				}
				// read shader colors
				for(INDEX icol=0;icol<ctcol;icol++) 
				{
					COLOR colColor;
					(*istrFile)>>colColor;
					pshpShaderParams->sp_acolColors[icol] = colColor;
				}
				// read shader floats
				for(INDEX ifl=0;ifl<ctfl;ifl++) 
				{
					FLOAT fFloat;
					(*istrFile)>>fFloat;
					pshpShaderParams->sp_afFloats[ifl] = fFloat;
				}
				ULONG ulFlags;
				(*istrFile)>>ulFlags;
				pshpShaderParams->sp_ulFlags = ulFlags;
			}
			else 
			{
				// this surface does not have shader
				msrf.msrf_pShader=NULL;
			}
		}
		
		if(ctmwm) 
		{
			// create array of weightmaps
			mlod.mlod_aWeightMaps.New(ctmwm);
			// Read each weightmap
			for(INDEX imwm=0;imwm<ctmwm;imwm++) 
			{
				MeshWeightMap &mwm = mlod.mlod_aWeightMaps[imwm];
				// Read weightmap name
				CTString strWeightMap;
				(*istrFile)>>strWeightMap;
				mwm.mwm_iID = ska_StringToID(strWeightMap);
				
				// Read vertex weight count
				INDEX ctmvw;
				(*istrFile)>>ctmvw;
				// Read vertex weights
				if(ctmvw>0) 
				{
					SLONG slSize = sizeof(MeshVertexWeight)*ctmvw;
					// Create vertex weight array
					mwm.mwm_aVertexWeight.New(ctmvw);
					// Read vertex weights
					istrFile->Read_t(&mwm.mwm_aVertexWeight[0],slSize);
				}
			}
		}
		
		// Create morphmaps array
		if(ctmmm>0) 
		{
			mlod.mlod_aMorphMaps.New(ctmmm);
			// read morphmaps
			for(INDEX immm=0;immm<ctmmm;immm++) 
			{
				MeshMorphMap &mmm = mlod.mlod_aMorphMaps[immm];
				// read morphmap ID
				CTString strMorphMap;
				(*istrFile)>>strMorphMap;
				mmm.mmp_iID = ska_StringToID(strMorphMap);
				// read bRelative
				(*istrFile)>>mmm.mmp_bRelative;
				// read morph sets count
				INDEX ctmvm;
				(*istrFile)>>ctmvm;
				// create morps sets array
				mmm.mmp_aMorphMap.New(ctmvm);
				// read morph sets
				istrFile->Read_t(&mmm.mmp_aMorphMap[0],sizeof(MeshVertexMorph)*ctmvm);
			}
		}
		
		// Read vertex weight infos
		if(ctmvwi>0) 
		{
			mlod.mlod_aVertexWeights.New(ctmvwi);
			istrFile->Read_t(&mlod.mlod_aVertexWeights[0],sizeof(MeshVertexWeightInfo)*ctmvwi);
		}
		
		// If no context skip vertex buffers
		if(!bHasContext) 
		{
			continue;
		}
		
//안태훈 수정 시작	//(Add & Modify SSSE Effect)(0.1)
		//bm을 로딩할때 그 이름을 확인하여 _TAG_가 접두로 들어가 있으면
		//그 모든 vertex를 Ska Tag로 추가한다.
		mlod.mlod_aSkaTags.Clear();
		INDEX iSkaTag = 0;
		for(INDEX iSurface=0; iSurface<mlod.mlod_aSurfaces.Count(); ++iSurface) 
		{
			MeshSurface &surface = mlod.mlod_aSurfaces[iSurface];
			CTString strName = ska_IDToString(surface.msrf_iSurfaceID);
			VERTEX_TAG_CONVERT_TYPE vtct = VTCT_VERTEX;
//안태훈 수정 시작	//(Remake Effect)(0.1)
			static CTString strTagIndicator(TAG_INDICATOR_STRING);
			static CTString strTagVtxIndicator(TAG_V_INDICATOR_STRING);
			static CTString strTagTriIndicator(TAG_T_INDICATOR_STRING);
			static CTString strTagGrpIndicator(TAG_G_INDICATOR_STRING);
			
			if( strName.RemovePrefix(strTagVtxIndicator) ) vtct = VTCT_VERTEX;
			else if( strName.RemovePrefix(strTagTriIndicator) ) vtct = VTCT_TRIANGLE;
			else if( strName.RemovePrefix(strTagGrpIndicator) ) vtct = VTCT_GROUP;
			else if( !strName.RemovePrefix(strTagIndicator) ) continue;	//기본값이 VTCT_VERTEX일때

			if(vtct == VTCT_GROUP)
			{
				static CTString strZero("0_");
				static CTString strOne("1_");
				static CTString strTwo("2_");
				static CTString strUBUB("__");
				if( strName.HasPrefix(strZero) || strName.HasPrefix(strOne) || strName.HasPrefix(strTwo) )
				{
					strName.InsertSubString(0, strUBUB);
				}
				else continue;
			}

			if(vtct == VTCT_VERTEX)
				mlod.mlod_aSkaTags.Expand( mlod.mlod_aSkaTags.Count() + surface.msrf_ctVertices );
			else if(vtct == VTCT_TRIANGLE || vtct == VTCT_GROUP)	//항상 하나의 Tag만 추가.
				mlod.mlod_aSkaTags.Expand( mlod.mlod_aSkaTags.Count() + 1 );
				//mlod.mlod_aSkaTags.Expand( mlod.mlod_aSkaTags.Count() + INDEX(floorf(surface.msrf_ctVertices/3.0f)) );

			INDEX checkTri = 1;	//하나만 만들게끔 보장.
			for(INDEX iVertex=surface.msrf_iFirstVertex; iVertex < surface.msrf_iFirstVertex+surface.msrf_ctVertices; ++iVertex)
			{
				if( (vtct == VTCT_TRIANGLE || vtct == VTCT_GROUP) && checkTri == 0) break;
				--checkTri;
//안태훈 수정 끝	//(Remake Effect)(0.1)
				FLOAT3D		vTagPos;
				FLOATquat3D	qTagRot;
				MeshVertex &mv = mlod.mlod_aVertices[ iVertex ];
				CSkaTag *ptrTag = new CSkaTag;
				mlod.mlod_aSkaTags[ iSkaTag ] = ptr_tag(ptrTag);
				++iSkaTag;
				if(mlod.mlod_aVertexWeights.Count() > 0)
				{
					MeshVertexWeightInfo &mvwi = mlod.mlod_aVertexWeights[ iVertex ];
					INDEX index;
					for(index = 0; index < 4 && mvwi.mvwi_aubWeights[index] > 0; ++index) NULL;
					ptrTag->SetBoneBlendValue(&mlod, index
						, surface.msrf_aubRelIndexTable[ ( mlod.mlod_aVertexWeights[iVertex].mvwi_aubIndices[0] ) ]
						, NormByteToFloat(mlod.mlod_aVertexWeights[ iVertex ].mvwi_aubWeights[0])
						, surface.msrf_aubRelIndexTable[ ( mlod.mlod_aVertexWeights[iVertex].mvwi_aubIndices[1] ) ]
						, NormByteToFloat(mlod.mlod_aVertexWeights[ iVertex ].mvwi_aubWeights[1])
						, surface.msrf_aubRelIndexTable[ ( mlod.mlod_aVertexWeights[iVertex].mvwi_aubIndices[2] ) ]
						, NormByteToFloat(mlod.mlod_aVertexWeights[ iVertex ].mvwi_aubWeights[2])
						, surface.msrf_aubRelIndexTable[ ( mlod.mlod_aVertexWeights[iVertex].mvwi_aubIndices[3] ) ]
						, NormByteToFloat(mlod.mlod_aVertexWeights[ iVertex ].mvwi_aubWeights[3])
					);
				}
				else
				{
					ptrTag->SetBoneBlendValue(&mlod);
				}
				std::string strNewName(strName.str_String);
				char buf[16] = {0};
				if(vtct == VTCT_VERTEX) strNewName += itoa(iVertex, buf, 10);
				ptrTag->SetName(strNewName);
				ptrTag->SetOffsetPos( mlod.mlod_aVertices[iVertex].x, mlod.mlod_aVertices[iVertex].y, mlod.mlod_aVertices[iVertex].z );
				FLOAT3D normal, axis(0,1,0);
				if(mlod.mlod_aNormals.Count())
				{
					normal = FLOAT3D(mlod.mlod_aNormals[iVertex].nx, mlod.mlod_aNormals[iVertex].ny, mlod.mlod_aNormals[iVertex].nz);
					FLOAT cosVal = axis % normal;
					FLOAT angle = acos( cosVal );
					FLOAT sinVal = sin( angle );
					if(angle > 0.001f)
					{
						axis *= normal;
						axis.Normalize();
						FLOATquat3D quat;
						quat.FromAxisAngle(axis, angle);
						ptrTag->SetOffsetRot( quat );
					}
					else
					{
						ptrTag->SetOffsetRot( FLOATquat3D(1,0,0,0) );
					}
				}
				else
				{
					ptrTag->SetOffsetRot( FLOATquat3D(1,0,0,0) );
				}
			}
		}
//안태훈 수정 끝	//(Add & Modify SSSE Effect)(0.1)
//안태훈 수정 시작	//(Add Tagent-space Normal Map)(0.1)
		//Tagent를 생성한다.
		BOOL bHasTangent = FALSE;
		if(mlod.mlod_aUVMaps.Count() > 0 && (bHaveTangentStream || _bSkaStudioApp))
		{
			if( mlod.mlod_aVertices.Count() == mlod.mlod_aUVMaps[0].muv_aTexCoords.Count()
				&& mlod.mlod_aVertices.Count() == mlod.mlod_aNormals.Count())
			{
				bHasTangent = TRUE;
			}
			else
			{
				ASSERT(FALSE && "Tagent 계산을 위해서는 Vertex 위치와 노말, 텍스쳐 좌표[0]의 수는 일치해야 한다.");
			}
		}

		if(bHasTangent)
		{
			CalcVertexTangent(mlod, 0);
			ctmvx = mlod.mlod_aVertices.Count();
		}
//안태훈 수정 끝	//(Add Tagent-space Normal Map)(0.1)
		
		//-------------------- Loading은 끝, Vertex Buffer 생성은 시작 ---------------//
		
		INDEX &iBufferID = mlod.mlod_iBufferBindID;
//안태훈 수정 시작	//(Add Tagent-space Normal Map)(0.1)
		if(bHasTangent) ulBufferFlags |= GFX_VBM_TAN;
//안태훈 수정 끝	//(Add Tagent-space Normal Map)(0.1)
		iBufferID = gfxCreateVertexBuffer(ctmvx,ulBufferFlags,ctmuvm,GFX_READWRITE);
		ASSERT(iBufferID>=0);
		
		// Get vertex buffer ids
		mlod.mlod_iVertexBufferID = gfxGetVertexSubBufferID(iBufferID,GFX_VBA_POS);
		mlod.mlod_iNormalBufferID = gfxGetVertexSubBufferID(iBufferID,GFX_VBA_NOR);
		ASSERT(mlod.mlod_iVertexBufferID>=0);
		ASSERT(mlod.mlod_iNormalBufferID>=0);
		
		// Get weight buffer id if weight buffer exists
		if(bHasWeights) 
		{
			mlod.mlod_iWeightBufferID = gfxGetVertexSubBufferID(iBufferID,GFX_VBA_WGH);
			ASSERT(mlod.mlod_iWeightBufferID>=0);
		}
		
		// Get texcoord buffer ids
		for( imuvm = 0; imuvm < ctmuvm; imuvm++)
		{
			MeshUVMap &muvm = mlod.mlod_aUVMaps[imuvm];
			muvm.muvm_iTexCoordBufferID = gfxGetVertexSubBufferID(iBufferID,GFX_VBA_TEX+imuvm);
			ASSERT(muvm.muvm_iTexCoordBufferID>=0);
		}
		
//안태훈 수정 시작	//(Add Tagent-space Normal Map)(0.1)
		if(bHasTangent)
		{
			mlod.mlod_iTangentBufferID = gfxGetVertexSubBufferID(iBufferID, GFX_VBA_TAN);
			ASSERT(mlod.mlod_iTangentBufferID>=0);
		}
//안태훈 수정 끝	//(Add Tagent-space Normal Map)(0.1)

		ASSERT(sizeof(MeshVertex)==sizeof(GFXVertex));
		ASSERT(sizeof(MeshNormal)==sizeof(GFXNormal));
		ASSERT(sizeof(MeshTexCoord)==sizeof(GFXTexCoord));
//안태훈 수정 시작	//(Add Tagent-space Normal Map)(0.1)
		ASSERT(sizeof(MeshTangent)==sizeof(GFXTangent));
//안태훈 수정 끝	//(Add Tagent-space Normal Map)(0.1)
		
		// TEMP: Fill vertex buffers
		GFXVertex *pavVertices = (GFXVertex*)gfxLockVertexSubBuffer(mlod.mlod_iVertexBufferID,0,ctmvx,GFX_WRITE);
		memcpy(pavVertices,&mlod.mlod_aVertices[0],sizeof(GFXVertex)*ctmvx);
		gfxUnlockVertexSubBuffer(mlod.mlod_iVertexBufferID, 0);
		
		GFXNormal *panNormals  = (GFXNormal*)gfxLockNormalSubBuffer(mlod.mlod_iNormalBufferID,0,ctmvx,GFX_WRITE);
		memcpy(panNormals,&mlod.mlod_aNormals[0],sizeof(GFXNormal)*ctmvx);
		gfxUnlockNormalSubBuffer(mlod.mlod_iNormalBufferID, 0);
		
		if(bHasWeights) 
		{
			GFXWeight *pawWeights = (GFXWeight*)gfxLockWeightSubBuffer(mlod.mlod_iWeightBufferID,0,ctmvx,GFX_WRITE);
			memcpy(pawWeights,&mlod.mlod_aVertexWeights[0],sizeof(GFXWeight)*ctmvx);
			gfxUnlockWeightSubBuffer(mlod.mlod_iWeightBufferID, 0);
		}
		
		for( imuvm = 0; imuvm < ctmuvm; imuvm++)
		{
			const MeshUVMap &muvm = mlod.mlod_aUVMaps[imuvm];
			GFXTexCoord *patcTexCoords = (GFXTexCoord*)gfxLockTexCoordSubBuffer(muvm.muvm_iTexCoordBufferID,0,ctmvx,GFX_WRITE);
			memcpy(patcTexCoords,&muvm.muv_aTexCoords[0],sizeof(GFXTexCoord)*ctmvx);
			gfxUnlockTexCoordSubBuffer(muvm.muvm_iTexCoordBufferID, 0);
		}
		
//안태훈 수정 시작	//(Add Tagent-space Normal Map)(0.1)
		if(bHasTangent)
		{
			GFXTangent *panTangents  = (GFXTangent*)gfxLockTangentSubBuffer(mlod.mlod_iTangentBufferID, 0, ctmvx, GFX_WRITE);
			memcpy(panTangents, &mlod.mlod_aTangents[0], sizeof(GFXTangent)*ctmvx);
			gfxUnlockTangentSubBuffer(mlod.mlod_iTangentBufferID, 0);
		}
//안태훈 수정 끝	//(Add Tagent-space Normal Map)(0.1)

		extern BOOL _bKeepVertexArrays;
		// free vertex arrays if alowed
		if(!_bKeepVertexArrays) 
		{
			mlod.mlod_aVertices.Clear();
			mlod.mlod_aNormals.Clear();
//안태훈 수정 시작	//(Add Tagent-space Normal Map)(0.1)
			mlod.mlod_aTangents.Clear();
//안태훈 수정 끝	//(Add Tagent-space Normal Map)(0.1)
			mlod.mlod_aVertexWeights.Clear();
			const INDEX ctmuvm = mlod.mlod_aUVMaps.Count();
			for(INDEX imuvm=0;imuvm<ctmuvm;imuvm++) 
			{
				MeshUVMap &muvm = mlod.mlod_aUVMaps[imuvm];
				muvm.muv_aTexCoords.Clear();
			}
		}
		
	}
}

//안태훈 수정 시작	//(Encode Data)(0.1)
// Write mesh to stream
void CMesh::Write_t(CTStream *ostrFile)
{
	UBYTE checker = MESH_NEW_VER;
	// write id
	ostrFile->WriteID_t(CChunkID(MESH_ID));
	// write version
	(*ostrFile)<<(INDEX)MESH_NEW_VER;
	
	// Remember position for mesh size in stream
	SLONG slSizePosition = ostrFile->GetPos_t();
	// Write invalid size of mesh
	SLONG slInvalidSize = 0;
	(*ostrFile) << slInvalidSize;
	
	INDEX ctmlod = msh_aMeshLODs.Count();
	// write mlod count
	(*ostrFile) << EncodeSimpleMesh(ctmlod, checker);
	// for each lod in mesh
	for(INDEX imlod=0;imlod<ctmlod;imlod++) 
	{
		
		const MeshLOD &mlod = msh_aMeshLODs[imlod];
		const INDEX ctmvx  = mlod.mlod_aVertices.Count();
		const INDEX ctmuvm = mlod.mlod_aUVMaps.Count();
		const INDEX ctmsrf = mlod.mlod_aSurfaces.Count();
		const INDEX ctmwm  = mlod.mlod_aWeightMaps.Count();
		const INDEX ctmmm  = mlod.mlod_aMorphMaps.Count();
		const INDEX ctmvwi = mlod.mlod_aVertexWeights.Count();
		
		// Write component count
		(*ostrFile) << EncodeSimpleMesh(ctmvwi, checker);
		(*ostrFile) << EncodeSimpleMesh(ctmwm, checker);
		(*ostrFile) << EncodeSimpleMesh(ctmuvm, checker);
		(*ostrFile) << EncodeSimpleMesh(ctmvx, checker);
		(*ostrFile) << EncodeSimpleMesh(ctmsrf, checker);
		(*ostrFile) << EncodeSimpleMesh(ctmmm, checker);
		
		// write source file name
		(*ostrFile) << mlod.mlod_fnSourceFile;
		// write max distance
		(*ostrFile) << mlod.mlod_fMaxDistance;
		// write flags
		(*ostrFile) << EncodeSimpleMesh(mlod.mlod_ulFlags, checker);
		
		// write vertices
		ostrFile->Write_t( &mlod.mlod_aVertices[0], sizeof(MeshVertex)*ctmvx);
		// write normals
		ostrFile->Write_t( &mlod.mlod_aNormals[0], sizeof(MeshNormal)*ctmvx);
		
		// write uvmaps
		for(int imuvm=0;imuvm<ctmuvm;imuvm++) 
		{
			// write uvmap ID
			const CTString strNameID = ska_GetStringFromTable(mlod.mlod_aUVMaps[imuvm].muv_iID);
			(*ostrFile) << strNameID;
			// write uvmaps texcordinates
			ostrFile->Write_t(&mlod.mlod_aUVMaps[imuvm].muv_aTexCoords[0],sizeof(MeshTexCoord)*ctmvx);
		}
		
		// write surfaces
		for(INDEX imsrt=0;imsrt<ctmsrf;imsrt++) 
		{
			const MeshSurface &msrf = mlod.mlod_aSurfaces[imsrt];
			const INDEX ctTris = msrf.msrf_auwTriangles.Count();
			const CTString strSurface = ska_IDToString(msrf.msrf_iSurfaceID);
			// write first vertex
			(*ostrFile) << EncodeSimpleMesh(msrf.msrf_iFirstVertex, checker);
			// write vertices count
			(*ostrFile) << EncodeSimpleMesh(msrf.msrf_ctVertices, checker);

			// write tris count
			(*ostrFile) << EncodeSimpleMesh(ctTris, checker);
			// write triangle indices
			ostrFile->Write_t( &mlod.mlod_aSurfaces[imsrt].msrf_auwTriangles[0], sizeof(MeshTriangle)*ctTris);

			// write surface Name
			(*ostrFile) << strSurface;
			// write surface flags
			(*ostrFile) << EncodeSimpleMesh(msrf.msrf_ulFlags, checker);

			// write relative to abs weight map index table
			const INDEX ctirt = msrf.msrf_aubRelIndexTable.Count();
			(*ostrFile) << EncodeSimpleMesh(ctirt, checker);
			if(ctirt>0) 
			{
				ostrFile->Write_t(&msrf.msrf_aubRelIndexTable[0],sizeof(UBYTE)*ctirt);
			}
			
			// write bool that this surface has a shader
			const INDEX bShaderExists = (msrf.msrf_pShader!=NULL);
			(*ostrFile) << EncodeSimpleMesh(bShaderExists, checker);
			if(bShaderExists) 
			{
				// get shader decription
//안태훈 수정 시작	//(For Performance)(0.1)
				ShaderDesc *pshDesc;
				msrf.msrf_pShader->GetShaderDesc(pshDesc);
				ShaderDesc &shDesc = *pshDesc;
//안태훈 수정 끝	//(For Performance)(0.1)
				INDEX cttx=shDesc.sd_astrTextureNames.Count();
				INDEX cttc=shDesc.sd_astrTexCoordNames.Count();
				INDEX ctcol=shDesc.sd_astrColorNames.Count();
				INDEX ctfl=shDesc.sd_astrFloatNames.Count();
				// data count must be at same as size defined in shader or higher
				ASSERT(cttx<=msrf.msrf_ShadingParams.sp_aiTextureIDs.Count());
				ASSERT(cttc<=msrf.msrf_ShadingParams.sp_aiTexCoordsIndex.Count());
				ASSERT(ctcol<=msrf.msrf_ShadingParams.sp_acolColors.Count());
				ASSERT(ctfl<=msrf.msrf_ShadingParams.sp_afFloats.Count());
				ASSERT(msrf.msrf_pShader->GetShaderDesc!=NULL);
				// write color count 
				(*ostrFile) << EncodeSimpleMesh(ctcol, checker);
				// write float count 
				(*ostrFile) << EncodeSimpleMesh(ctfl, checker);
				// write texture count 
				(*ostrFile) << EncodeSimpleMesh(cttx, checker);
				// write texture coords count 
				(*ostrFile) << EncodeSimpleMesh(cttc, checker);
				
				ASSERT(msrf.msrf_pShader!=NULL);
				// write shader name
				CTString strShaderName;
				strShaderName = msrf.msrf_pShader->GetName();
				(*ostrFile) << strShaderName;
				// write shader texture IDs
				for(INDEX itx=0;itx<cttx;itx++)
				{
					INDEX iTexID = msrf.msrf_ShadingParams.sp_aiTextureIDs[itx];
					CTString strTextID;
					if(iTexID!=(-1)) 
					{
						strTextID = ska_IDToString(iTexID);
					}
					(*ostrFile) << strTextID;
				}
				// write shader flags
				ULONG ulFlags = msrf.msrf_ShadingParams.sp_ulFlags;
				(*ostrFile) << EncodeSimpleMesh(ulFlags, checker);
				// write shader colors
				for(INDEX icol=0;icol<ctcol;icol++)
				{
					COLOR colColor = msrf.msrf_ShadingParams.sp_acolColors[icol];
					(*ostrFile) << EncodeSimpleMesh(colColor, checker);
				}
				// write shader floats
				for(INDEX ifl=0;ifl<ctfl;ifl++)
				{
					FLOAT fFloat = msrf.msrf_ShadingParams.sp_afFloats[ifl];
					(*ostrFile) << fFloat;
				}
				// write shader texture coords indices
				for(INDEX itc=0;itc<cttc;itc++)
				{
					INDEX iTexCoorsIndex = msrf.msrf_ShadingParams.sp_aiTexCoordsIndex[itc];
					(*ostrFile) << EncodeSimpleMesh(iTexCoorsIndex, checker);
				}
			}
		}
		
		// for each weightmap in array
		for(INDEX imwm=0;imwm<ctmwm;imwm++) 
		{
			const MeshWeightMap &mwm = mlod.mlod_aWeightMaps[imwm];
			const INDEX ctmvw = mwm.mwm_aVertexWeight.Count();
			// write wertex weight map ID
			CTString strWeightMap = ska_IDToString(mwm.mwm_iID);
			(*ostrFile) << strWeightMap;
			// write vertex weights count
			(*ostrFile) << EncodeSimpleMesh(ctmvw, checker);
			if(ctmvw>0) 
			{
				// write vertex weights
				ostrFile->Write_t(&mwm.mwm_aVertexWeight[0],sizeof(MeshVertexWeight)*ctmvw);
			}
		}
		
		for(INDEX immm=0;immm<ctmmm;immm++) 
		{
			const MeshMorphMap &mmm = mlod.mlod_aMorphMaps[immm];
			const INDEX ctmvm = mmm.mmp_aMorphMap.Count();
			// write morph map name
			CTString strMorphMap = ska_IDToString(mmm.mmp_iID);
			(*ostrFile) << strMorphMap;
			// write bRelative
			(*ostrFile) << EncodeSimpleMesh(mmm.mmp_bRelative, checker);
			// write morph count
			(*ostrFile) << EncodeSimpleMesh(ctmvm, checker);
			// write morph sets
			ostrFile->Write_t(&mmm.mmp_aMorphMap[0],sizeof(MeshVertexMorph)*ctmvm);
		}
		
		// write vertex weight infos if they exists
		if(ctmvwi>0) 
		{
			ostrFile->Write_t(&mlod.mlod_aVertexWeights[0],sizeof(MeshVertexWeightInfo)*ctmvwi);
		}
	}
	// Remember current position
	SLONG slCurrentPos = ostrFile->GetPos_t();
	// Get byte count saved for mesh
	SLONG slMeshSize = slCurrentPos - slSizePosition;
	// Update mesh size at begining of stream
	ostrFile->SetPos_t(slSizePosition);
	(*ostrFile) << slMeshSize;
	// Return file pointer to old position
	ostrFile->SetPos_t(slCurrentPos);
}

void CMesh::Write_t_old(CTStream *ostrFile)
//안태훈 수정 끝	//(Encode Data)(0.1)
{
	// write id
	ostrFile->WriteID_t(CChunkID(MESH_ID));
	// write version
	(*ostrFile)<<(INDEX)MESH_VERSION;
	
	// Remember position for mesh size in stream
	SLONG slSizePosition = ostrFile->GetPos_t();
	// Write invalid size of mesh
	SLONG slInvalidSize = 0;
	(*ostrFile)<<slInvalidSize;
	
	INDEX ctmlod = msh_aMeshLODs.Count();
	// write mlod count
	(*ostrFile)<<ctmlod;
	// for each lod in mesh
	for(INDEX imlod=0;imlod<ctmlod;imlod++) 
	{
		
		const MeshLOD &mlod = msh_aMeshLODs[imlod];
		const INDEX ctmvx  = mlod.mlod_aVertices.Count();
		const INDEX ctmuvm = mlod.mlod_aUVMaps.Count();
		const INDEX ctmsrf = mlod.mlod_aSurfaces.Count();
		const INDEX ctmwm  = mlod.mlod_aWeightMaps.Count();
		const INDEX ctmmm  = mlod.mlod_aMorphMaps.Count();
		const INDEX ctmvwi = mlod.mlod_aVertexWeights.Count();
		
		// Write component count
		(*ostrFile)<<ctmvx;
		(*ostrFile)<<ctmuvm;
		(*ostrFile)<<ctmsrf;
		(*ostrFile)<<ctmwm;
		(*ostrFile)<<ctmmm;
		(*ostrFile)<<ctmvwi;
		
		// write source file name
		(*ostrFile)<<mlod.mlod_fnSourceFile;
		// write max distance
		(*ostrFile)<<mlod.mlod_fMaxDistance;
		// write flags
		(*ostrFile)<<mlod.mlod_ulFlags;
		
		// write vertices
		ostrFile->Write_t( &mlod.mlod_aVertices[0], sizeof(MeshVertex)*ctmvx);
		// write normals
		ostrFile->Write_t( &mlod.mlod_aNormals[0], sizeof(MeshNormal)*ctmvx);
		
		// write uvmaps
		for(int imuvm=0;imuvm<ctmuvm;imuvm++) 
		{
			// write uvmap ID
			const CTString strNameID = ska_GetStringFromTable(mlod.mlod_aUVMaps[imuvm].muv_iID);
			(*ostrFile)<<strNameID;
			// write uvmaps texcordinates
			ostrFile->Write_t(&mlod.mlod_aUVMaps[imuvm].muv_aTexCoords[0],sizeof(MeshTexCoord)*ctmvx);
		}
		
		// write surfaces
		for(INDEX imsrt=0;imsrt<ctmsrf;imsrt++) 
		{
			const MeshSurface &msrf = mlod.mlod_aSurfaces[imsrt];
			const INDEX ctTris = msrf.msrf_auwTriangles.Count();
			const CTString strSurface = ska_IDToString(msrf.msrf_iSurfaceID);
			// write surface Name
			(*ostrFile)<<strSurface;
			// write surface flags
			(*ostrFile)<<msrf.msrf_ulFlags;
			// write first vertex
			(*ostrFile)<<msrf.msrf_iFirstVertex;
			// write vertices count
			(*ostrFile)<<msrf.msrf_ctVertices;
			// write tris count
			(*ostrFile)<<ctTris;
			// write triangle indices
			ostrFile->Write_t( &mlod.mlod_aSurfaces[imsrt].msrf_auwTriangles[0], sizeof(MeshTriangle)*ctTris);
			// write relative to abs weight map index table
			const INDEX ctirt = msrf.msrf_aubRelIndexTable.Count();
			(*ostrFile)<<ctirt;
			if(ctirt>0) 
			{
				ostrFile->Write_t(&msrf.msrf_aubRelIndexTable[0],sizeof(UBYTE)*ctirt);
			}
			
			// write bool that this surface has a shader
			const INDEX bShaderExists = (msrf.msrf_pShader!=NULL);
			(*ostrFile)<<bShaderExists;
			if(bShaderExists) 
			{
				// get shader decription
//안태훈 수정 시작	//(For Performance)(0.1)
				ShaderDesc *pshDesc;
				msrf.msrf_pShader->GetShaderDesc(pshDesc);
				ShaderDesc &shDesc = *pshDesc;
//안태훈 수정 끝	//(For Performance)(0.1)
				INDEX cttx=shDesc.sd_astrTextureNames.Count();
				INDEX cttc=shDesc.sd_astrTexCoordNames.Count();
				INDEX ctcol=shDesc.sd_astrColorNames.Count();
				INDEX ctfl=shDesc.sd_astrFloatNames.Count();
				// data count must be at same as size defined in shader or higher
				ASSERT(cttx<=msrf.msrf_ShadingParams.sp_aiTextureIDs.Count());
				ASSERT(cttc<=msrf.msrf_ShadingParams.sp_aiTexCoordsIndex.Count());
				ASSERT(ctcol<=msrf.msrf_ShadingParams.sp_acolColors.Count());
				ASSERT(ctfl<=msrf.msrf_ShadingParams.sp_afFloats.Count());
				ASSERT(msrf.msrf_pShader->GetShaderDesc!=NULL);
				// write texture count 
				(*ostrFile)<<cttx;
				// write texture coords count 
				(*ostrFile)<<cttc;
				// write color count 
				(*ostrFile)<<ctcol;
				// write float count 
				(*ostrFile)<<ctfl;
				
				ASSERT(msrf.msrf_pShader!=NULL);
				// write shader name
				CTString strShaderName;
				strShaderName = msrf.msrf_pShader->GetName();
				(*ostrFile)<<strShaderName;
				// write shader texture IDs
				for(INDEX itx=0;itx<cttx;itx++)
				{
					INDEX iTexID = msrf.msrf_ShadingParams.sp_aiTextureIDs[itx];
					CTString strTextID;
					if(iTexID!=(-1)) 
					{
						strTextID = ska_IDToString(iTexID);
					}
					(*ostrFile)<<strTextID;
				}
				// write shader texture coords indices
				for(INDEX itc=0;itc<cttc;itc++)
				{
					INDEX iTexCoorsIndex = msrf.msrf_ShadingParams.sp_aiTexCoordsIndex[itc];
					(*ostrFile)<<iTexCoorsIndex;
				}
				// write shader colors
				for(INDEX icol=0;icol<ctcol;icol++)
				{
					COLOR colColor = msrf.msrf_ShadingParams.sp_acolColors[icol];
					(*ostrFile)<<colColor;
				}
				// write shader floats
				for(INDEX ifl=0;ifl<ctfl;ifl++)
				{
					FLOAT fFloat = msrf.msrf_ShadingParams.sp_afFloats[ifl];
					(*ostrFile)<<fFloat;
				}
				// write shader flags
				ULONG ulFlags = msrf.msrf_ShadingParams.sp_ulFlags;
				(*ostrFile)<<ulFlags;
			}
		}
		
		// for each weightmap in array
		for(INDEX imwm=0;imwm<ctmwm;imwm++) 
		{
			const MeshWeightMap &mwm = mlod.mlod_aWeightMaps[imwm];
			const INDEX ctmvw = mwm.mwm_aVertexWeight.Count();
			// write wertex weight map ID
			CTString strWeightMap = ska_IDToString(mwm.mwm_iID);
			(*ostrFile)<<strWeightMap;
			// write vertex weights count
			(*ostrFile)<<ctmvw;
			if(ctmvw>0) 
			{
				// write vertex weights
				ostrFile->Write_t(&mwm.mwm_aVertexWeight[0],sizeof(MeshVertexWeight)*ctmvw);
			}
		}
		
		for(INDEX immm=0;immm<ctmmm;immm++) 
		{
			const MeshMorphMap &mmm = mlod.mlod_aMorphMaps[immm];
			const INDEX ctmvm = mmm.mmp_aMorphMap.Count();
			// write morph map name
			CTString strMorphMap = ska_IDToString(mmm.mmp_iID);
			(*ostrFile)<<strMorphMap;
			// write bRelative
			(*ostrFile)<<mmm.mmp_bRelative;
			// write morph count
			(*ostrFile)<<ctmvm;
			// write morph sets
			ostrFile->Write_t(&mmm.mmp_aMorphMap[0],sizeof(MeshVertexMorph)*ctmvm);
		}
		
		// write vertex weight infos if they exists
		if(ctmvwi>0) 
		{
			ostrFile->Write_t(&mlod.mlod_aVertexWeights[0],sizeof(MeshVertexWeightInfo)*ctmvwi);
		}
	}
	// Remember current position
	SLONG slCurrentPos = ostrFile->GetPos_t();
	// Get byte count saved for mesh
	SLONG slMeshSize = slCurrentPos - slSizePosition;
	// Update mesh size at begining of stream
	ostrFile->SetPos_t(slSizePosition);
	(*ostrFile)<<slMeshSize;
	// Return file pointer to old position
	ostrFile->SetPos_t(slCurrentPos);
}

void CMesh::Unbind(void)
{
	const INDEX ctmlod = msh_aMeshLODs.Count();
	for(INDEX imlod=0;imlod<ctmlod;imlod++) 
	{
		MeshLOD &mlod = msh_aMeshLODs[imlod];
		UnbindMeshLOD(mlod);
	}
}

// Clear mesh
void CMesh::Clear(void)
{
	// for each LOD
	const INDEX ctmlod = msh_aMeshLODs.Count();
	for(INDEX imlod=0; imlod<ctmlod; imlod++) 
	{
		// for each surface, clear the triangles list
		MeshLOD &mlod = msh_aMeshLODs[imlod];
		ClearMeshLOD(mlod);
	}
	// in the end, clear all LODs
	msh_aMeshLODs.Clear();
}

#ifdef KALYDO
static void KCPMeshDownloaded(const char* fileName, TKResult result, void* id)
{
	switch (result)
	{
	case KR_OK:
		{
			SLS* pSLS = new SLS();
			pSLS->pTarget = reinterpret_cast<CSerial*>(id);
			pSLS->pTargetFilePath = fileName;
			g_deqLoadData.push_back( pSLS );
		}
		break;
	case KR_DOWNLOAD_FAILED:
	case KR_FILE_CORRUPT:
		krfRequestKCPFile(fileName, &KCPMeshDownloaded, id);
	//default:
		// unknown error!
	}
}

void CMesh::Load_t(const CTFileName &fnFileName)
{
  ASSERT(!IsUsed());
  // mark that you have changed
  MarkChanged();
  // 근데 이게 확실한가?? 호출 매커니즘의 정확한 해명이 필요할 거 같다.

  TKResult tkResult = krfRequestKCPFile( fnFileName, NULL, NULL );
  if( KR_OK == tkResult )
  {
	// open a stream
	CTFileStream istrFile;
	istrFile.Open_t(fnFileName);
	// read object from stream
	Read_t(&istrFile);
	// if still here (no exceptions raised)
	// remember filename
	ser_FileName = fnFileName;
  }
  else
  {
	CPrintF("Request file to kcp : %s\n", fnFileName );
	//?????????????
	CTFileStream istrFile;
	istrFile.Open_t( strDefaultMeshPath );
	Read_t(&istrFile);
	ser_FileName = fnFileName;
	if( KR_FILE_NOT_AVAILABLE == tkResult )
	{
		MarkUsed();
	}

	tkResult = krfRequestKCPFile(fnFileName, &KCPMeshDownloaded, this);
	if( KR_FILE_NOT_FOUND == tkResult )
	{
		CPrintF("[Load_t] Mesh File Not Found in kalydo...\n" );
	}
	else if( KR_IO_PENDING == tkResult )
	{
		CPrintF("[Load_t] Mesh File already request...\n" );
	}
	else
	{
		;
	}
  }  
}



void CMesh::Load_Delay_t(const CTFileName &fnFileName)
{
  // mark that you have changed
  MarkChanged();
  // 근데 이게 확실한가?? 호출 매커니즘의 정확한 해명이 필요할 거 같다.

  //if( kfileExists( fnFileName ) )
	// open a stream
	CTFileStream istrFile;
	istrFile.Open_t(fnFileName);
	// read object from stream
	Read_t(&istrFile);
	// if still here (no exceptions raised)
	// remember filename
	ser_FileName = fnFileName;
	MarkUnused();
}
#endif

// Count used memory
SLONG CMesh::GetUsedMemory(void)
{
	SLONG slMemoryUsed = sizeof(*this);
	INDEX ctmlod = msh_aMeshLODs.Count();
	for(INDEX imlod=0;imlod<ctmlod;imlod++) 
	{
		MeshLOD &mlod = msh_aMeshLODs[imlod];
		slMemoryUsed += sizeof(mlod);
		slMemoryUsed += mlod.mlod_aVertices.Count() * sizeof(MeshVertex);
		slMemoryUsed += mlod.mlod_aNormals.Count()  * sizeof(MeshNormal);
		slMemoryUsed += mlod.mlod_aVertexWeights.Count() * sizeof(MeshVertexWeightInfo);
		
		// for each uvmap
		INDEX ctuvmaps = mlod.mlod_aUVMaps.Count();
		for(INDEX imuvm=0;imuvm<ctuvmaps;imuvm++) 
		{
			MeshUVMap &uvmap = mlod.mlod_aUVMaps[imuvm];
			slMemoryUsed+=sizeof(uvmap);
			slMemoryUsed+=uvmap.muv_aTexCoords.Count() * sizeof(MeshTexCoord);
		}
		
		// for each surface
		INDEX ctmsrf = mlod.mlod_aSurfaces.Count();
		INDEX imsrf;
		for( imsrf = 0; imsrf < ctmsrf; imsrf++ ) 
		{
			MeshSurface &msrf = mlod.mlod_aSurfaces[imsrf];
			slMemoryUsed += sizeof(msrf);
			slMemoryUsed += sizeof(MeshTriangle) * msrf.msrf_auwTriangles.Count();
			slMemoryUsed += sizeof(ShaderParams);
			slMemoryUsed += sizeof(INDEX) * msrf.msrf_ShadingParams.sp_aiTextureIDs.Count();
			slMemoryUsed += sizeof(INDEX) * msrf.msrf_ShadingParams.sp_aiTexCoordsIndex.Count();
			slMemoryUsed += sizeof(COLOR) * msrf.msrf_ShadingParams.sp_acolColors.Count();
			slMemoryUsed += sizeof(FLOAT) * msrf.msrf_ShadingParams.sp_afFloats.Count();
		}
		// for each weight map
		INDEX ctwm = mlod.mlod_aWeightMaps.Count();
		for(INDEX imwm=0;imwm<ctwm;imwm++) 
		{
			MeshWeightMap &mwm = mlod.mlod_aWeightMaps[imwm];
			slMemoryUsed+=sizeof(mwm);
			slMemoryUsed+=mwm.mwm_aVertexWeight.Count() * sizeof(MeshVertexWeight);
		}
		// for each morphmap
		INDEX ctmm = mlod.mlod_aMorphMaps.Count();
		for(INDEX imm=0;imm<ctmm;imm++) 
		{
			MeshMorphMap &mmm = mlod.mlod_aMorphMaps[imm];
			slMemoryUsed+=sizeof(mmm);
			slMemoryUsed+=mmm.mmp_aMorphMap.Count() * sizeof(MeshVertexMorph);
		}
		
		// Get used memory by vertex buffers
		// INDEX ctmvx = gfxGetVertexBufferSize(mlod.mlod_iBufferBindID);
	}
	return slMemoryUsed;
}

/* Get the description of this object. */
CTString CMesh::GetDescription(void)
{
	return CTString(0, "%d LODs", msh_aMeshLODs.Count());
}


//안태훈 수정 시작	//(Add Tagent-space Normal Map)(0.1)

static void FillWeightMapInfo(MeshLOD &mlod)
{
	int boneCount = mlod.mlod_aWeightMaps.Count();
	int vertexCount = mlod.mlod_aVertexWeights.Count();
	int surfaceCount = mlod.mlod_aSurfaces.Count();
	for(int iBone=0; iBone<mlod.mlod_aWeightMaps.Count(); ++iBone)
	{
		mlod.mlod_aWeightMaps[iBone].mwm_aVertexWeight.Clear();	//일단 WeightMap의 정보를 지운다.
	}

	for(int iSurface=0; iSurface<surfaceCount; ++iSurface)
	{
		int vertexEnd = mlod.mlod_aSurfaces[iSurface].msrf_iFirstVertex + mlod.mlod_aSurfaces[iSurface].msrf_ctVertices;
		for(INDEX iVertex=mlod.mlod_aSurfaces[iSurface].msrf_iFirstVertex; iVertex<vertexEnd; ++iVertex)
		{
			for(int iBPV=0; iBPV<MAX_BPV; ++iBPV)
			{
				if(mlod.mlod_aVertexWeights[iVertex].mvwi_aubWeights[iBPV] < 1) continue;					//weight가 0인 bone은 의미가 없다.
				
				MeshVertexWeight insertVal;	//WeightMap에 추가할 정보
				insertVal.mww_iVertex = iVertex;
				insertVal.mww_fWeight = mlod.mlod_aVertexWeights[iVertex].mvwi_aubWeights[iBPV] / 255.0f;

				int relIndex = mlod.mlod_aVertexWeights[iVertex].mvwi_aubIndices[iBPV];				//vertex weight에서 쓰이는 본의 상대적인 인덱스
				int absIndex = mlod.mlod_aSurfaces[iSurface].msrf_aubRelIndexTable[relIndex];		//surface의 index table에 절대 인덱스가 들어있다.GetRelativeWeightIndex()참고

				int vertexWeightCount = mlod.mlod_aWeightMaps[absIndex].mwm_aVertexWeight.Count();
				mlod.mlod_aWeightMaps[absIndex].mwm_aVertexWeight.Expand( vertexWeightCount+1 );	//늘리고 추가.
				mlod.mlod_aWeightMaps[absIndex].mwm_aVertexWeight[ vertexWeightCount ] = insertVal;
			}
		}
	}
}

static MeshTangent MakeMeshTangent(double *normal, double *tangent, double *binormal)
{
	DcVecNormalizeD(tangent);
	DcVecNormalizeD(binormal);
	//transform for tangent space의 행렬식의 determinent를 이용해야 한다.
/*
	double det =  tangent[0] * binormal[1] * normal[2]
				+ tangent[1] * binormal[2] * normal[0]
				+ tangent[2] * binormal[0] * normal[1]
				- tangent[2] * binormal[1] * normal[0]
				- tangent[2] * binormal[1] * normal[0]
				- tangent[1] * binormal[0] * normal[2];
	det = (float)(det >= 0 ? +1 : -1);
	MeshTangent result( (float)tangent[0], (float)tangent[1], (float)tangent[2], det );
*///tangent 사용시

	//binormal 사용시
	MeshTangent result;
	result.bx = (float)binormal[0];
	result.by = (float)binormal[1];
	result.bz = (float)binormal[2];
	double crossResult[3];
	DcVecCross(crossResult, binormal, normal);
	result.tdir = DcVecDot3(crossResult, tangent) < 0 ? -1 : 1 ;
	return result;
}

#include <Engine/Ska/NmFileIO.h>
//적어도 한개의 삼각형이 존재하고 position, normal, 한개이상의 texcoord를 가져야 한다.
//texCoordForTangent의 값은 NormalMap을 어떤 좌표에 맞게 뽑았는지에 따라 달라질것이다.
//현재 0으로 fix해놓고 쓰나 다른 값을 넣어도 무방하다.(현재는 하드코딩으로 해야함.
static void CalcVertexTangent(MeshLOD &mlod, int texCoordForTangent)
{
	if(mlod.mlod_aVertices.Count() < 3 || mlod.mlod_aNormals.Count() < 3 || mlod.mlod_aUVMaps.Count() < 1)
	{
		ASSERTALWAYS("주어진 MeshLOD로는 TANGENT를 만들 수 없습니다. 입력인자가 부족합니다.");
	}
	
	int iSurf = 0;
	int cntVertInLOD = 0;
	int iUV;
	MeshLOD mlodNew;
	mlodNew.mlod_aUVMaps.New(mlod.mlod_aUVMaps.Count());
	mlodNew.mlod_aSurfaces.New(mlod.mlod_aSurfaces.Count());
	for(iSurf=0; iSurf<mlod.mlod_aSurfaces.Count(); ++iSurf)
	{
		mlodNew.mlod_aSurfaces[iSurf].msrf_auwTriangles.New( mlod.mlod_aSurfaces[iSurf].msrf_auwTriangles.Count() );
	}

	mlodNew.mlod_aVertices.New(0);
	mlodNew.mlod_aNormals.New(0);
	mlodNew.mlod_aTangents.New(0);
	mlodNew.mlod_aVertexWeights.New(0);
	for( iUV = 0; iUV < mlod.mlod_aUVMaps.Count(); ++iUV )
	{
		mlodNew.mlod_aUVMaps[iUV].muv_aTexCoords.New(0);
	}

	//Tagent Space 계산을 위해 데이터를 정리한다.
	for(iSurf=0; iSurf<mlod.mlod_aSurfaces.Count(); ++iSurf)
	{
		int cntTri = mlod.mlod_aSurfaces[iSurf].msrf_auwTriangles.Count();
		int numVertsRet = 0;
		NmTangentPointD *vertsRet = NULL;
		NmIndex *indicesRet = NULL;
		NmRawTriangle *trisData = new NmRawTriangle[cntTri];

		for(int trisIndex=0; trisIndex<cntTri; ++trisIndex)
		{
			int idx = 0;
			int iVtx[3];
			iVtx[0] = mlod.mlod_aSurfaces[iSurf].msrf_auwTriangles[trisIndex].mt_uwVertexIndex[0] + mlod.mlod_aSurfaces[iSurf].msrf_iFirstVertex;
			iVtx[1] = mlod.mlod_aSurfaces[iSurf].msrf_auwTriangles[trisIndex].mt_uwVertexIndex[1] + mlod.mlod_aSurfaces[iSurf].msrf_iFirstVertex;
			iVtx[2] = mlod.mlod_aSurfaces[iSurf].msrf_auwTriangles[trisIndex].mt_uwVertexIndex[2] + mlod.mlod_aSurfaces[iSurf].msrf_iFirstVertex;

			for(idx=0; idx < 3; ++idx)
			{
				//position
				trisData[trisIndex].vert[idx].x = mlod.mlod_aVertices[iVtx[idx]].x;
				trisData[trisIndex].vert[idx].y = mlod.mlod_aVertices[iVtx[idx]].y;
				trisData[trisIndex].vert[idx].z = mlod.mlod_aVertices[iVtx[idx]].z;
				//normal
				trisData[trisIndex].norm[idx].x = mlod.mlod_aNormals[iVtx[idx]].nx;
				trisData[trisIndex].norm[idx].y = mlod.mlod_aNormals[iVtx[idx]].ny;
				trisData[trisIndex].norm[idx].z = mlod.mlod_aNormals[iVtx[idx]].nz;
				//tex coord
				trisData[trisIndex].texCoord[idx] = new NmRawTexCoord[ mlod.mlod_aUVMaps.Count() ];
				for(int iUV=0; iUV<mlod.mlod_aUVMaps.Count(); ++iUV)
				{
					trisData[trisIndex].texCoord[idx][iUV].u = mlod.mlod_aUVMaps[iUV].muv_aTexCoords[iVtx[idx]].u;
					trisData[trisIndex].texCoord[idx][iUV].v = mlod.mlod_aUVMaps[iUV].muv_aTexCoords[iVtx[idx]].v;
				}
				//vertex weight
				if(mlod.mlod_aVertexWeights.Count() > 0)
				{
					trisData[trisIndex].weights[idx].use = TRUE;
					trisData[trisIndex].weights[idx].iBlend[0] = mlod.mlod_aVertexWeights[iVtx[idx]].mvwi_aubIndices[0];
					trisData[trisIndex].weights[idx].iBlend[1] = mlod.mlod_aVertexWeights[iVtx[idx]].mvwi_aubIndices[1];
					trisData[trisIndex].weights[idx].iBlend[2] = mlod.mlod_aVertexWeights[iVtx[idx]].mvwi_aubIndices[2];
					trisData[trisIndex].weights[idx].iBlend[3] = mlod.mlod_aVertexWeights[iVtx[idx]].mvwi_aubIndices[3];
					trisData[trisIndex].weights[idx].weight[0] = mlod.mlod_aVertexWeights[iVtx[idx]].mvwi_aubWeights[0];
					trisData[trisIndex].weights[idx].weight[1] = mlod.mlod_aVertexWeights[iVtx[idx]].mvwi_aubWeights[1];
					trisData[trisIndex].weights[idx].weight[2] = mlod.mlod_aVertexWeights[iVtx[idx]].mvwi_aubWeights[2];
					trisData[trisIndex].weights[idx].weight[3] = mlod.mlod_aVertexWeights[iVtx[idx]].mvwi_aubWeights[3];
				}
				else
				{
					trisData[trisIndex].weights[idx].use = FALSE;
				}
			}
		}
		//Tangent Space를 계산한다.
		NmCreateVertexBuffers(cntTri, trisData
			, &numVertsRet, &vertsRet, &indicesRet
			, mlod.mlod_aUVMaps.Count(), texCoordForTangent);

		//다시 MeshLOD쪽으로 계산된 결과를 저장한다.
		mlodNew.mlod_aVertices.Expand(cntVertInLOD + numVertsRet);
		mlodNew.mlod_aNormals.Expand(cntVertInLOD + numVertsRet);
		mlodNew.mlod_aTangents.Expand(cntVertInLOD + numVertsRet);
		mlodNew.mlod_aVertexWeights.Expand(cntVertInLOD + numVertsRet);
		for( iUV = 0; iUV < mlod.mlod_aUVMaps.Count(); ++iUV )
		{
			mlodNew.mlod_aUVMaps[iUV].muv_aTexCoords.Expand(cntVertInLOD + numVertsRet);
		}
		
		for(int k=0; k<numVertsRet; ++k)
		{
			mlodNew.mlod_aVertices[cntVertInLOD+k].x = (float)vertsRet[k].vertex[0];
			mlodNew.mlod_aVertices[cntVertInLOD+k].y = (float)vertsRet[k].vertex[1];
			mlodNew.mlod_aVertices[cntVertInLOD+k].z = (float)vertsRet[k].vertex[2];
			mlodNew.mlod_aNormals[cntVertInLOD+k].nx = (float)vertsRet[k].normal[0];
			mlodNew.mlod_aNormals[cntVertInLOD+k].ny = (float)vertsRet[k].normal[1];
			mlodNew.mlod_aNormals[cntVertInLOD+k].nz = (float)vertsRet[k].normal[2];
			mlodNew.mlod_aTangents[cntVertInLOD+k] = MakeMeshTangent(vertsRet[k].normal, vertsRet[k].tangent, vertsRet[k].binormal);
			for( iUV = 0; iUV < mlod.mlod_aUVMaps.Count(); ++iUV )
			{
				mlodNew.mlod_aUVMaps[iUV].muv_aTexCoords[cntVertInLOD+k].u = (float)vertsRet[k].u[iUV];
				mlodNew.mlod_aUVMaps[iUV].muv_aTexCoords[cntVertInLOD+k].v = (float)vertsRet[k].v[iUV];
			}
			if(mlod.mlod_aVertexWeights.Count() > 0)
			{
				mlodNew.mlod_aVertexWeights[cntVertInLOD+k].mvwi_aubIndices[0] = vertsRet[k].iBlend[0];
				mlodNew.mlod_aVertexWeights[cntVertInLOD+k].mvwi_aubIndices[1] = vertsRet[k].iBlend[1];
				mlodNew.mlod_aVertexWeights[cntVertInLOD+k].mvwi_aubIndices[2] = vertsRet[k].iBlend[2];
				mlodNew.mlod_aVertexWeights[cntVertInLOD+k].mvwi_aubIndices[3] = vertsRet[k].iBlend[3];
				mlodNew.mlod_aVertexWeights[cntVertInLOD+k].mvwi_aubWeights[0] = vertsRet[k].weights[0];
				mlodNew.mlod_aVertexWeights[cntVertInLOD+k].mvwi_aubWeights[1] = vertsRet[k].weights[1];
				mlodNew.mlod_aVertexWeights[cntVertInLOD+k].mvwi_aubWeights[2] = vertsRet[k].weights[2];
				mlodNew.mlod_aVertexWeights[cntVertInLOD+k].mvwi_aubWeights[3] = vertsRet[k].weights[3];
			}
		}
		//삼각형 인덱스 저장
		mlodNew.mlod_aSurfaces[iSurf].msrf_iFirstVertex = cntVertInLOD;
		mlodNew.mlod_aSurfaces[iSurf].msrf_ctVertices = numVertsRet;
		for(int j=0; j<mlodNew.mlod_aSurfaces[iSurf].msrf_auwTriangles.Count(); ++j)
		{
			mlodNew.mlod_aSurfaces[iSurf].msrf_auwTriangles[j].mt_uwVertexIndex[0] = indicesRet[j].idx[0];
			mlodNew.mlod_aSurfaces[iSurf].msrf_auwTriangles[j].mt_uwVertexIndex[1] = indicesRet[j].idx[1];
			mlodNew.mlod_aSurfaces[iSurf].msrf_auwTriangles[j].mt_uwVertexIndex[2] = indicesRet[j].idx[2];
		}

		cntVertInLOD += numVertsRet;
		delete[] trisData;
		delete[] vertsRet;
		delete[] indicesRet;
	}
	mlod.mlod_aVertices = mlodNew.mlod_aVertices;
	mlod.mlod_aNormals = mlodNew.mlod_aNormals;
	mlod.mlod_aTangents = mlodNew.mlod_aTangents;
	for( iUV = 0; iUV < mlod.mlod_aUVMaps.Count(); ++iUV )
	{
		mlod.mlod_aUVMaps[iUV].muv_aTexCoords = mlodNew.mlod_aUVMaps[iUV].muv_aTexCoords;
	}
	mlod.mlod_aVertexWeights = mlodNew.mlod_aVertexWeights;
	for(iSurf=0; iSurf<mlodNew.mlod_aSurfaces.Count(); ++iSurf)
	{
		mlod.mlod_aSurfaces[iSurf].msrf_iFirstVertex = mlodNew.mlod_aSurfaces[iSurf].msrf_iFirstVertex;
		mlod.mlod_aSurfaces[iSurf].msrf_ctVertices = mlodNew.mlod_aSurfaces[iSurf].msrf_ctVertices;
		mlod.mlod_aSurfaces[iSurf].msrf_auwTriangles = mlodNew.mlod_aSurfaces[iSurf].msrf_auwTriangles;
	}
	
	//스키닝 관련 정보의 저장
	if(mlod.mlod_aVertexWeights.Count() > 0 && mlod.mlod_aWeightMaps.Count() > 0)
	{
		FillWeightMapInfo(mlod);
	}
}
//안태훈 수정 끝	//(Add Tagent-space Normal Map)(0.1)

