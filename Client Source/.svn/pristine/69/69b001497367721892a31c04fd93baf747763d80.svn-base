#include "StdH.h"
#include <Engine/Base/Console.h>
#include <Engine/Math/Projection.h>
#include <Engine/Math/Float.h>
#include <Engine/Math/Vector.h>
#include <Engine/Math/Quaternion.h>
#include <Engine/Math/Geometry.inl>
#include <Engine/Math/Clipping.inl>
#include <Engine/Math/Matrix12.h>
#include <Engine/Ska/ModelInstance.h>
#include <Engine/Ska/Render.h>
#include <Engine/Ska/Mesh.h>
#include <Engine/Ska/Skeleton.h>
#include <Engine/Ska/AnimSet.h>
#include <Engine/Ska/StringTable.h>
#include <Engine/Graphics/Fog_internal.h>
#include <Engine/Templates/DynamicContainer.cpp>
#include <Engine/Graphics/Drawport.h>
#include <Engine/Base/Statistics_internal.h>
#include <Engine/Base/MemoryTracking.h>
#include <Engine/Base/ObjectRestore.h>

//안태훈 수정 시작	//(Effect Add & Modify for Close Beta)(0.1)
#include <Engine/Math/AdditionalFunction.h>
#include <Engine/Effect/CEffectGroupManager.h>
//안태훈 수정 끝	//(Effect Add & Modify for Close Beta)(0.1)
//안태훈 수정 시작	//(5th Closed beta)(0.2)
#include <Engine/Entities/Entity.h>
//안태훈 수정 끝	//(5th Closed beta)(0.2)

//강동민 수정 시작
// NOTE : 나중에 제거해야 할듯(CreateShadowMatrix() 때문에 필요함.)
#include <d3dx8.h>
#define SHADOWTEXTURESIZE (256)
//강동민 수정 끝


// asm shortcuts
#define O offset
#define Q qword ptr
#define D dword ptr
#define W  word ptr
#define B  byte ptr


static CAnyProjection3D _aprProjection;
static CDrawPort *_pdp = NULL;
static ULONG _ulDrawPortID = NONE; // for occlusion test

static enum FPUPrecisionType _fpuOldPrecision;

static FLOAT3D  _vLightDir            = FLOAT3D(0,0,0); // Light direction
static FLOAT3D  _vLightDirInObj       = FLOAT3D(0,0,0); // Light direction transformed to object space
static COLOR    _colAmbient           = 0x00000000;     // Ambient color
static COLOR    _colLight             = 0x00000000;     // Light color
static COLOR    _colModel             = 0x00000000;     // Root model color
static FLOAT    _fDistanceFactor      = 0.0f;           // Distance to object from viewer
static Matrix12 _mObjToAbs            = {0};            // object to absolute
static Matrix12 _mAbsToViewer         = {0};            // absolute to viewer
static Matrix12 _mObjToView           = {0};            // object to viewer
static Matrix12 _mObjToViewStretch    = {0};            // object to viewer, stretch by root model instance stretch factor
static Matrix12 _mTransObjToAbs       = {0};            // transformed object to absolute (if model does not have skeleton)
extern BOOL     _bTransAllToAbsolute  = FALSE;          // transform all vertices to absolute space
extern BOOL     _bUseHardwareShaders  = FALSE;          // use vertex program
static FLOATaabbox3D _bbRootAllFrames;                  // All frames bbox of root model instance 

static ULONG _ulFlags                 = RMF_SHOWTEXTURE;
static INDEX _iRenderingType          = 0;              // 0=none, 1=view, 2=mask
static FLOAT _fMipAdjustFactor        = 1.0f;           // mip metric multiplier
static ULONG _ulRenFlags              = 0;              // Render flags
static FLOAT _fCustomDistance         = -1;             // custom distance for lods
static INDEX _ctMaxWeightsPerVertex   = 4;              // max number of weights that effect one vertex

extern FLOAT ska_fLODMul;
extern FLOAT ska_fLODAdd;
extern INDEX ska_bAnimateMesh;

// mask shader (for rendering models' shadows to shadowmaps)
static CShader _shMaskShader;
//강동민 수정 시작
static CShader	*_pshShadowShader;
static CShader	*_pshNoShadowShader;
static CShader  *_pshNiceWaterShader;
static BOOL		_bLoadedShadowShader = FALSE;
//강동민 수정 시작 테스트 클라이언트 작업	06.17
//static COLOR	_crShadowColor = 0x05050580;			// Shadow Color
static COLOR	_crShadowColor = 0x050505A0;			// Shadow Color
//강동민 수정 끝 테스트 클라이언트 작업		06.17
//강동민 수정 끝

//강동민 수정 시작 Water 구현		04.20
//static CNiceWater* _pWaterInformation = NULL;
//강동민 수정 끝 Water 구현			04.20

// temporary rendering structures
// Tangent 관련 코드 추가를 하면서 Morph쪽은 처리하지 않았음.
// Morph는 현재 쓰이지 않음. ㅡ_ㅡ;
CStaticStackArray<struct RenModel> _aRenModels;
CStaticStackArray<struct RenMesh> _aRenMeshes;
CStaticStackArray<struct RenBone> _aRenBones;
CStaticStackArray<struct RenMorph> _aRenMorphs;
CStaticStackArray<struct RenWeight> _aRenWeights;
CStaticStackArray<struct MeshVertex> _aMorphedVertices;
CStaticStackArray<struct MeshNormal> _aMorphedNormals;
CStaticStackArray<struct MeshVertex> _aFinalVertices;
CStaticStackArray<struct MeshNormal> _aFinalNormals;
//안태훈 수정 시작	//(Add Tagent-space Normal Map)(0.1)
CStaticStackArray<struct MeshTangent> _aFinalTangents;
//안태훈 수정 끝	//(Add Tagent-space Normal Map)(0.1)
CStaticStackArray<struct RenMatrix> _aWeightMatrices;
CStaticStackArray<struct SurfaceMatrices> _aSurfacesMatrices;

// search tables 
static CStaticStackArray<SLONG> _aslRenBoneSearchTable;

// mesh source buffer pointers (valid only in prepare mesh for rendering and render mesh)
static MeshVertex *_pavMeshVertices = NULL; // mesh lod vertices (from vertex buffer)
static MeshNormal *_panMeshNormals  = NULL; // mesh lod normals  (from normal buffer)
//안태훈 수정 시작	//(Add Tagent-space Normal Map)(0.1)
static MeshTangent *_panMeshTangents  = NULL; // mesh lod normals  (from normal buffer)
//안태훈 수정 끝	//(Add Tagent-space Normal Map)(0.1)
static MeshVertexWeightInfo *_pawMeshWeights  = NULL; // mesh lod weights  (from weight buffer)
static CStaticStackArray<MeshTexCoord *> _patcMeshTexCoords;
extern INDEX _ctMeshVertices = 0;           // mesh lod vertex count
static BOOL _bMeshBuffersLocked = FALSE;    // Are buffers locked

extern MeshVertex *_pavWeightedVertices = NULL;  // final vertex arrays (in software this are morphed and weighted vertices, in hardware default model vertices)
extern MeshNormal *_panWeightedNormals  = NULL;  // final normal array  (in software this are morphed and weighted normals , in hardware default model normals)
//안태훈 수정 시작	//(Add Tagent-space Normal Map)(0.1)
extern MeshTangent *_panWeightedTangents  = NULL;  // final tangent array  (in software this are morphed and weighted normals , in hardware default model normals)
//안태훈 수정 끝	//(Add Tagent-space Normal Map)(0.1)
extern MeshVertex *_pavMorphedVertices  = NULL;  // morphed vertices (contains vertices only for surfaces that have morph maps - if no active morphmaps points to default model vertices)
extern MeshNormal *_panMorphedNormals   = NULL;  // morphed normals  (contains normals  only for surfaces that have morph maps - if no active morphmaps points to default model normals)
static INDEX _iMorphedBufferBindID      = -1;
static INDEX _iMorphedVertexBufferID    = -1;    // ID of dynamic vertex buffer for morphed vertices
static INDEX _iMorphedNormalBufferID    = -1;    // ID of dynamic normal buffer for morphed normals
extern BOOL  _bMeshUsesMorphs           = FALSE; // are morphs active in mesh
extern INDEX _ctMorphVertices           = 0;     // vertices count in dynamic surfaces


// Pointers for bone adjustment function
static void (*_pAdjustBonesCallback)(void *pData) = NULL;
static void *_pAdjustBonesData = NULL;
// Pointers for shader params adjustment function
static void (*_pAdjustShaderParams)(void *pData, INDEX iSurfaceID, CShader *pShader,ShaderParams &shParams) = NULL;
static void *_pAdjustShaderData = NULL;

inline static INDEX FindRenBoneIndex(const RenModel &rm,INDEX iBoneID);
inline extern void LockMeshReadBuffers(const MeshLOD &mlod);
inline extern void UnlockMeshReadBuffers(const MeshLOD &mlod);
inline static void SetSurfaceMatrices(const RenMesh &rmsh, const MeshSurface &msrf, const MeshLOD &mlod);
extern void PrepareMeshForRendering(RenMesh &rmsh, INDEX iSkeletonlod);
//안태훈 수정 시작	//(Add & Modify SSSE Effect)(0.1)
extern void CalculateRenderingData(CModelInstance &mi, BOOL bRenderToScreen);
//안태훈 수정 끝	//(Add & Modify SSSE Effect)(0.1)
extern void ClearRenArrays();


#define FastSlerp Slerp //FastSlerp_internal // or just "Slerp"


extern const INDEX *_piACosTable;
extern const FLOAT *_pfSinTable;
extern const FLOAT *_pf1oSinTable;


__forceinline FLOAT Fast1oSin( const INDEX iAngle1)
{
	INDEX iAngle2 = (iAngle1>>8) + Sgn(iAngle1);
	if( iAngle2<-255) iAngle2 += 256;
	else if( iAngle2>+255) iAngle2 -= 256;
	const FLOAT f1oSin1 = _pf1oSinTable[iAngle1>>8];
	const FLOAT f1oSin2 = _pf1oSinTable[iAngle2>>0];
	const FLOAT fFrac = (iAngle1 & 255) *(1.0f/256.0f);
	return (f1oSin1*(1.0f-fFrac) + f1oSin2*fFrac);
}


__forceinline FLOAT FastSin( const INDEX iAngle1)
{
	INDEX iAngle2 = (iAngle1>>8) + Sgn(iAngle1);
	if( iAngle2<-255) iAngle2 += 256;
	else if( iAngle2>+255) iAngle2 -= 256;
	const FLOAT fSin1 = _pfSinTable[iAngle1>>8];
	const FLOAT fSin2 = _pfSinTable[iAngle2>>0];
	const FLOAT fFrac = (iAngle1 & 255) *(1.0f/256.0f);
	return (fSin1*(1.0f-fFrac) + fSin2*fFrac);
}



// fast spherical linear interpolation using tables
__forceinline FLOATquat3D FastSlerp_internal( const FLOAT tT, const FLOATquat3D &q1, const FLOATquat3D &q2)
{
	FLOAT fCos = q1%q2;
	ASSERT( fCos>=-1.001 && fCos<=+1.001);
	
	FLOATquat3D qTemp;
	if( fCos<0) 
	{
		fCos = -fCos;
		qTemp = -q2;
	}
	else  
	{
		qTemp = q2;
	}
	
	if( fCos < 0.99f) 
	{
		// standard case (slerp)
		ASSERT( tT>=0 && tT<=1);
		const SLONG slT  = NormFloatToByte(tT);
		const INDEX iCos = FloatToInt(fCos*255);
		const INDEX iAngle = _piACosTable[iCos];
		const FLOAT f1oSin = Fast1oSin(iAngle<<8);
		const INDEX iAngle1 = ((255-slT)*iAngle);
		const INDEX iAngle2 = (     slT *iAngle);
		const FLOAT fF1 = FastSin(iAngle1) * f1oSin;
		const FLOAT fF2 = FastSin(iAngle2) * f1oSin;
		return q1*fF1 + qTemp*fF2;
	}
	else 
	{        
		// linear interpolation
		return q1 + tT*(qTemp-q1);
	}
}




// need this for occlusion query
extern void RenderBBox( const FLOAT3D &vMinVtx, const FLOAT3D &vMaxVtx, COLOR col=C_WHITE|CT_OPAQUE);


// convert 3x4 matrix to QVect 
__forceinline void Matrix12ToQVect(QVect &qv,const Matrix12 &m12)
{
	FLOATmatrix3D m;
	m(1,1) = m12[ 0]; m(1,2) = m12[ 1]; m(1,3) = m12[ 2]; 
	m(2,1) = m12[ 4]; m(2,2) = m12[ 5]; m(2,3) = m12[ 6]; 
	m(3,1) = m12[ 8]; m(3,2) = m12[ 9]; m(3,3) = m12[10]; 
	
	qv.qRot.FromMatrix(m);
	qv.vPos(1) = m12[3];
	qv.vPos(2) = m12[7];
	qv.vPos(3) = m12[11];
}


// covert QVect to matrix 3x4
__forceinline static void QVectToMatrix12(Matrix12 &m12, const QVect &qv)
{
	FLOATmatrix3D m;
	qv.qRot.ToMatrix(m);
	MatrixVectorToMatrix12(m12,m,qv.vPos);
}

__forceinline static void TransformVector_copy( FLOAT3 &vDst, FLOAT3 &vSrc, const Matrix12 &m)
{
	const FLOAT x = vSrc[0];
	const FLOAT y = vSrc[1];
	const FLOAT z = vSrc[2];
	vDst[0] = m[0]*x + m[1]*y + m[ 2]*z + m[ 3];
	vDst[1] = m[4]*x + m[5]*y + m[ 6]*z + m[ 7];
	vDst[2] = m[8]*x + m[9]*y + m[10]*z + m[11];
}

__forceinline static void TransformVector_copy( FLOAT3 &vDst, FLOAT3 &vSrc, const Matrix12 &m, FLOAT fWeight)
{
	const FLOAT x = vSrc[0];
	const FLOAT y = vSrc[1];
	const FLOAT z = vSrc[2];
	vDst[0] = (m[0]*x + m[1]*y + m[ 2]*z + m[ 3]) * fWeight;
	vDst[1] = (m[4]*x + m[5]*y + m[ 6]*z + m[ 7]) * fWeight;
	vDst[2] = (m[8]*x + m[9]*y + m[10]*z + m[11]) * fWeight;
}

__forceinline static void TransformVector_add( FLOAT3 &vDst, FLOAT3 &vSrc, const Matrix12 &m, FLOAT fWeight)
{
	const FLOAT x = vSrc[0];
	const FLOAT y = vSrc[1];
	const FLOAT z = vSrc[2];
	vDst[0] += (m[0]*x + m[1]*y + m[ 2]*z + m[ 3]) * fWeight;
	vDst[1] += (m[4]*x + m[5]*y + m[ 6]*z + m[ 7]) * fWeight;
	vDst[2] += (m[8]*x + m[9]*y + m[10]*z + m[11]) * fWeight;
}


//강동민 수정 시작
//__forceinline static void TransformVertex( GFXVertex &v, const Matrix12 &m)		// 원본
__forceinline void TransformVertex( GFXVertex &v, const Matrix12 &m)
//강동민 수정 끝
{
	const FLOAT x = v.x;
	const FLOAT y = v.y;
	const FLOAT z = v.z;
	v.x = m[0]*x + m[1]*y + m[ 2]*z + m[ 3];
	v.y = m[4]*x + m[5]*y + m[ 6]*z + m[ 7];
	v.z = m[8]*x + m[9]*y + m[10]*z + m[11];
}

__forceinline static void RotateVector_copy( FLOAT3 &vDst, FLOAT3 &vSrc, const Matrix12 &m)
{
	const FLOAT x = vSrc[0];
	const FLOAT y = vSrc[1];
	const FLOAT z = vSrc[2];
	vDst[0] = m[0]*x + m[1]*y + m[ 2]*z;
	vDst[1] = m[4]*x + m[5]*y + m[ 6]*z;
	vDst[2] = m[8]*x + m[9]*y + m[10]*z;
}

__forceinline static void RotateVector_copy( FLOAT3 &vDst, FLOAT3 &vSrc, const Matrix12 &m, FLOAT fWeight)
{
	const FLOAT x = vSrc[0];
	const FLOAT y = vSrc[1];
	const FLOAT z = vSrc[2];
	vDst[0] = (m[0]*x + m[1]*y + m[ 2]*z) * fWeight;
	vDst[1] = (m[4]*x + m[5]*y + m[ 6]*z) * fWeight;
	vDst[2] = (m[8]*x + m[9]*y + m[10]*z) * fWeight;
}

__forceinline static void RotateVector_add( FLOAT3 &vDst, FLOAT3 &vSrc, const Matrix12 &m, FLOAT fWeight)
{
	const FLOAT x = vSrc[0];
	const FLOAT y = vSrc[1];
	const FLOAT z = vSrc[2];
	vDst[0] += (m[0]*x + m[1]*y + m[ 2]*z) * fWeight;
	vDst[1] += (m[4]*x + m[5]*y + m[ 6]*z) * fWeight;
	vDst[2] += (m[8]*x + m[9]*y + m[10]*z) * fWeight;
}




// calculate fade factor of animation list in animqueue
__forceinline extern FLOAT CalculateFadeFactor(const AnimList &alList)
{
	if(alList.al_fFadeTime==0) 
	{
		return 1.0f;
	}
	
	const FLOAT fFadeFactor = (_pTimer->GetLerpedCurrentTick() - alList.al_fStartTime) / alList.al_fFadeTime;
	return Clamp(fFadeFactor,0.0f,1.0f);
}

// set view matrix
__forceinline extern void SetViewMatrix( Matrix12 &mView12)
{
	FLOAT mView16[16];
	mView16[ 0] = mView12[ 0];  mView16[ 1] = mView12[ 4];  mView16[ 2] = mView12[ 8];  mView16[ 3] = 0;
	mView16[ 4] = mView12[ 1];  mView16[ 5] = mView12[ 5];  mView16[ 6] = mView12[ 9];  mView16[ 7] = 0;
	mView16[ 8] = mView12[ 2];  mView16[ 9] = mView12[ 6];  mView16[10] = mView12[10];  mView16[11] = 0;
	mView16[12] = mView12[ 3];  mView16[13] = mView12[ 7];  mView16[14] = mView12[11];  mView16[15] = 1;
	gfxSetViewMatrix(mView16);
}


// LOD factor management
extern void RM_SetCurrentDistance(FLOAT fDistFactor)
{
	_fCustomDistance = fDistFactor;
}

// Calculate mip adjustment factor based on projection FOV(if it exists)
static FLOAT _fLastMipAdjustmentFactor = -1;
static FLOAT _fLastFOVForMipFactor = -1;
extern FLOAT RM_GetMipAdjustmentFactor(const CProjection3D *pprProjection)
{
	FLOAT fFOV;
	if(pprProjection!=NULL) 
	{
		fFOV = ((CPerspectiveProjection3D*)pprProjection)->FOVR();
	}
	else 
	{
		fFOV = 90.0f;
	}
	
	// if precalculated
	if(_fLastFOVForMipFactor==fFOV) 
	{
		// return precalculated value
		return _fLastMipAdjustmentFactor;
		// if not 
	}
	else 
	{
		// calculate new mip adjustment factor
		const FLOAT fMipAdjustFactor = Tan(fFOV/2.0f);
		_fLastMipAdjustmentFactor = fMipAdjustFactor;
		_fLastFOVForMipFactor = fFOV;
		return fMipAdjustFactor;
	}
}

extern FLOAT RM_GetAdjustedMipDistance(FLOAT fDistance, FLOAT fMipAdjustFactor)
{
	return fDistance * fMipAdjustFactor;
}

// fill given array with array of transformed vertices
extern void RM_GetModelVertices( CModelInstance &mi, CStaticStackArray<FLOAT3D> &avVertices,
								FLOATmatrix3D &mRotation, FLOAT3D &vPosition, FLOAT fNormalOffset, FLOAT fDistance)
{
	extern INDEX ska_iMaxWeightsPerVertex;
	CObjectRestore<BOOL> or(_bTransAllToAbsolute,TRUE);
	CObjectRestore<BOOL> or2(_bUseHardwareShaders,FALSE);
	CObjectRestore<INDEX> or3(ska_iMaxWeightsPerVertex,1);
	
	// only root model instances
	// ASSERT(mi.mi_iParentBoneID==-1);
	
	// remember parent bone ID and set parent bone ID as -1
	CObjectRestore<INDEX> or4(mi.mi_iParentBoneID, -1);
	
	RM_SetObjectPlacement(mRotation,vPosition);
	
	RM_SetCurrentDistance(fDistance);
	CalculateRenderingData(mi, FALSE);
	
	// for each ren model
	INDEX ctrmsh = _aRenModels.Count();
	for(int irmsh=1;irmsh<ctrmsh;irmsh++) 
	{
		RenModel &rm = _aRenModels[irmsh];
		INDEX ctmsh = rm.rm_iFirstMesh + rm.rm_ctMeshes;
		// for each mesh in renmodel
		for(int imsh=rm.rm_iFirstMesh;imsh<ctmsh;imsh++) 
		{
			// prepare mesh for rendering
			RenMesh &rmsh = _aRenMeshes[imsh];
			// Get mesh lod
			const MeshLOD &mlod = rmsh.rmsh_pMeshInst->mi_pMesh->msh_aMeshLODs[rmsh.rmsh_iMeshLODIndex];
			// Lock buffer arrays
			LockMeshReadBuffers(mlod);
			PrepareMeshForRendering(rmsh,rm.rm_iSkeletonLODIndex);
			const INDEX ctvtxGiven = avVertices.Count();
			const INDEX ctvtx = _ctMeshVertices;
			avVertices.Push(ctvtx);
			// for each surface
			const INDEX ctmsrf = mlod.mlod_aSurfaces.Count();
			for(INDEX imsrf=0;imsrf<ctmsrf;imsrf++) 
			{
				const MeshSurface &msrf = mlod.mlod_aSurfaces[imsrf];
				const INDEX ctmvx = msrf.msrf_ctVertices;
				const INDEX iFirstVertex = msrf.msrf_iFirstVertex;
				const BOOL bDynamicSurface = msrf.msrf_ulFlags&MS_DYNAMIC_SURFACE;
				MeshVertex *pavFinalVertices;
				MeshNormal *panFinalNormals;
				
				// if this is dynamic surface and morphs exists
				if(bDynamicSurface && _bMeshUsesMorphs) 
				{
					// use pointer to morphed vertices
					ASSERT(_pavMorphedVertices!=NULL && _panMorphedNormals!=NULL);
					pavFinalVertices = _pavMorphedVertices;
					panFinalNormals = _panMorphedNormals;
					// if software shaders is used all morphed and weighted vertices are in _pavWeightedVertices
				}
				else 
				{
					// use pointer to final vertices
					ASSERT(_pavWeightedVertices!=NULL && _panWeightedNormals!=NULL);
					pavFinalVertices = _pavWeightedVertices;
					panFinalNormals = _panWeightedNormals;
				}
				
				if(fNormalOffset==0.0f) 
				{
					for(INDEX imvx=0;imvx<ctmvx;imvx++) 
					{
						const INDEX imvxAbs = imvx+iFirstVertex;
						FLOAT3D &vx = avVertices[imvxAbs+ctvtxGiven];
						vx(1) = pavFinalVertices[imvxAbs].x;
						vx(2) = pavFinalVertices[imvxAbs].y;
						vx(3) = pavFinalVertices[imvxAbs].z;
					}
				}
				else 
				{
					for(INDEX imvx=0;imvx<ctmvx;imvx++) 
					{
						const INDEX imvxAbs = imvx+iFirstVertex;
						FLOAT3D vVtx( pavFinalVertices[imvxAbs].x, pavFinalVertices[imvxAbs].y, pavFinalVertices[imvxAbs].z);
						FLOAT3D vNor( panFinalNormals[imvxAbs].nx, panFinalNormals[imvxAbs].ny, panFinalNormals[imvxAbs].nz);
						avVertices[imvxAbs+ctvtxGiven] = vVtx+(vNor*fNormalOffset);
					}
				}
			}
			// Unlock buffer arrays
			UnlockMeshReadBuffers(mlod);
		}
	}
	ClearRenArrays();
}





//안태훈 수정 시작	//(Open beta)(2004-12-01)
#include <Engine/Effect/CSkaTag.h>
#include <algorithm>
class binfo
{
public:
	FLOAT bw;
	INDEX bi;
public:
	binfo() : bw(0), bi(0) {}

	bool operator < (binfo &other)	{ return bw < other.bw;		}
	bool operator <= (binfo &other)	{ return bw <= other.bw;	}
	bool operator > (binfo &other)	{ return bw > other.bw;		}
	bool operator >= (binfo &other)	{ return bw >= other.bw;	}
};
FLOAT RM_TestRayCastHit( CModelInstance &mi, FLOATmatrix3D &mRotation, FLOAT3D &vPosition,const FLOAT3D &vOrigin,
						const FLOAT3D &vTarget,FLOAT fOldDistance,INDEX *piBoneID, void *pSkatag)
//안태훈 수정 끝	//(Open beta)(2004-12-01)
{
	FLOAT fDistance = 1E6f;
	extern INDEX ska_iMaxWeightsPerVertex;
	CObjectRestore<BOOL> or(_bTransAllToAbsolute,TRUE);
	CObjectRestore<BOOL> or2(_bUseHardwareShaders,FALSE);
	CObjectRestore<INDEX> or3(ska_iMaxWeightsPerVertex,1);
	
	RM_SetObjectPlacement(mRotation,vPosition);
	// Reset abs to viewer matrix
	// MakeIdentityMatrix(_mAbsToViewer);
	// allways use the first LOD
	RM_SetCurrentDistance(0);
	CalculateRenderingData(mi, FALSE);
	// for each ren model
	INDEX ctrm = _aRenModels.Count();
	for(int irmsh=1;irmsh<ctrm;irmsh++)
	{
		RenModel &rm = _aRenModels[irmsh];
		INDEX ctmsh = rm.rm_iFirstMesh + rm.rm_ctMeshes;
		// for each mesh in renmodel
		for(int imsh=rm.rm_iFirstMesh;imsh<ctmsh;imsh++)
		{
			// prepare mesh for rendering
			RenMesh &rmsh = _aRenMeshes[imsh];
			// Get mesh lod
//안태훈 수정 시작	//(Open beta)(2004-12-01)
			MeshLOD &mlod = rmsh.rmsh_pMeshInst->mi_pMesh->msh_aMeshLODs[rmsh.rmsh_iMeshLODIndex];
//안태훈 수정 끝	//(Open beta)(2004-12-01)
			// don't do raycast check if mesh wasn't reloaded
			if( mlod.mlod_iBufferBindID==(-1)) continue;
			
			// Lock buffer arrays
			LockMeshReadBuffers(mlod);
			PrepareMeshForRendering(rmsh,rm.rm_iSkeletonLODIndex);
			
			// for each surface in mesh
			INDEX ctmsrf = mlod.mlod_aSurfaces.Count();
			for(INDEX imsrf=0;imsrf<ctmsrf;imsrf++) 
			{
				const MeshSurface &msrf = mlod.mlod_aSurfaces[imsrf];
				INDEX cttris = msrf.msrf_auwTriangles.Count();
				// for each triangle in surface
				for(INDEX itri=0; itri<cttris;itri++) 
				{
					const MeshTriangle &msht = msrf.msrf_auwTriangles[itri];
					const MeshVertex &mshv1 = _pavWeightedVertices[msht.mt_uwVertexIndex[0] + msrf.msrf_iFirstVertex];
					const MeshVertex &mshv2 = _pavWeightedVertices[msht.mt_uwVertexIndex[1] + msrf.msrf_iFirstVertex];
					const MeshVertex &mshv3 = _pavWeightedVertices[msht.mt_uwVertexIndex[2] + msrf.msrf_iFirstVertex];
					
					FLOAT3D vVertex0( mshv1.x, mshv1.y, mshv1.z);
					FLOAT3D vVertex1( mshv2.x, mshv2.y, mshv2.z);
					FLOAT3D vVertex2( mshv3.x, mshv3.y, mshv3.z);
					
					Plane <float,3> plTriPlane(vVertex0,vVertex1,vVertex2);
					FLOAT fDistance0 = plTriPlane.PointDistance(vOrigin);
					FLOAT fDistance1 = plTriPlane.PointDistance(vTarget);
					
					// if the ray hits the polygon plane
					if (fDistance0>=0 && fDistance0>=fDistance1) 
					{
						// calculate fraction of line before intersection
						FLOAT fFraction = fDistance0/(fDistance0-fDistance1);
						// calculate intersection coordinate
						FLOAT3D vHitPoint = vOrigin+(vTarget-vOrigin)*fFraction;
						// calculate intersection distance
						FLOAT fHitDistance = (vHitPoint-vOrigin).Length();
						// if the hit point can not be new closest candidate
						if (fHitDistance>fOldDistance) 
						{
							// skip this triangle
							continue;
						}
						
						// find major axes of the polygon plane
						INDEX iMajorAxis1, iMajorAxis2;
						GetMajorAxesForPlane(plTriPlane, iMajorAxis1, iMajorAxis2);
						
						// create an intersector
						CIntersector isIntersector(vHitPoint(iMajorAxis1), vHitPoint(iMajorAxis2));
						
						
						// check intersections for all three edges of the polygon
						isIntersector.AddEdge(
							vVertex0(iMajorAxis1), vVertex0(iMajorAxis2),
							vVertex1(iMajorAxis1), vVertex1(iMajorAxis2));
						isIntersector.AddEdge(
							vVertex1(iMajorAxis1), vVertex1(iMajorAxis2),
							vVertex2(iMajorAxis1), vVertex2(iMajorAxis2));
						isIntersector.AddEdge(
							vVertex2(iMajorAxis1), vVertex2(iMajorAxis2),
							vVertex0(iMajorAxis1), vVertex0(iMajorAxis2));
						
						// if the polygon is intersected by the ray, and it is the closest intersection so far
						if (isIntersector.IsIntersecting() && (fHitDistance < fDistance)) 
						{
							// remember hit coordinates
							fDistance = fHitDistance;
							// do we neet to find the bone hit by the ray?
							if (piBoneID != NULL) 
							{
								INDEX iClosestVertex;
								// find the vertex closest to the intersection
								FLOAT fDist0 = (vHitPoint - vVertex0).Length();
								FLOAT fDist1 = (vHitPoint - vVertex1).Length();
								FLOAT fDist2 = (vHitPoint - vVertex2).Length();
//안태훈 수정 시작	//(Open beta)(2004-12-02)
								FLOAT3D vVertex;
								if (fDist0 < fDist1) 
								{
									if (fDist0 < fDist2) 
									{
										iClosestVertex = msrf.msrf_auwTriangles[itri].mt_uwVertexIndex[0] + msrf.msrf_iFirstVertex;
										vVertex = vVertex0;
									}
									else 
									{
										iClosestVertex = msrf.msrf_auwTriangles[itri].mt_uwVertexIndex[2] + msrf.msrf_iFirstVertex;
										vVertex = vVertex2;
									}
								}
								else 
								{
									if (fDist1 < fDist2) 
									{
										iClosestVertex = msrf.msrf_auwTriangles[itri].mt_uwVertexIndex[1] + msrf.msrf_iFirstVertex;
										vVertex = vVertex1;
									}
									else 
									{
										iClosestVertex = msrf.msrf_auwTriangles[itri].mt_uwVertexIndex[2] + msrf.msrf_iFirstVertex;
										vVertex = vVertex2;
									}
								}
								INDEX ibw = 0;
								binfo abi[4];
								
								// now find the weightmap with the largest weight for this vertex
								INDEX ctmwm = mlod.mlod_aWeightMaps.Count();
								FLOAT fMaxVertexWeight = 0.0f;
								INDEX iMaxWeightMap = -1;
								// for each weight map in surface
								for(INDEX imwm=0;imwm<ctmwm;imwm++) 
								{
									const MeshWeightMap &mwm = mlod.mlod_aWeightMaps[imwm];
									INDEX ctvtx = mwm.mwm_aVertexWeight.Count();
									// for each vertex in weight map
									for(INDEX ivtx=0;ivtx<ctvtx;ivtx++) 
									{
										if(mwm.mwm_aVertexWeight[ivtx].mww_iVertex == iClosestVertex)
										{
											abi[ibw].bw = mwm.mwm_aVertexWeight[ivtx].mww_fWeight;
											abi[ibw].bi = imwm;
											++ibw;
											if(mwm.mwm_aVertexWeight[ivtx].mww_fWeight > fMaxVertexWeight)
											{
												fMaxVertexWeight = mwm.mwm_aVertexWeight[ivtx].mww_fWeight;
												iMaxWeightMap = mwm.mwm_iID;
												//const CTString &strHitWeight = ska_GetStringFromTable(mwm.mwm_iID);
												break;
											}
										}
/*
										if ((mwm.mwm_aVertexWeight[ivtx].mww_iVertex == iClosestVertex)
											&& (mwm.mwm_aVertexWeight[ivtx].mww_fWeight > fMaxVertexWeight)) 
										{
											fMaxVertexWeight = mwm.mwm_aVertexWeight[ivtx].mww_fWeight;
											iMaxWeightMap = mwm.mwm_iID;
											const CTString &strHitWeight = ska_GetStringFromTable(mwm.mwm_iID);
											break;
										}
*/
									}
								}
								*piBoneID = iMaxWeightMap;
								//fill ska tag infos
								if(pSkatag != NULL)
								{
									std::sort(abi, abi+4);
									CSkaTag *ptrTag = (CSkaTag*)pSkatag;
									INDEX iVertex = iClosestVertex;
									ptrTag->SetBoneBlendValue(&mlod, ibw
										, abi[3].bi, abi[3].bw
										, abi[2].bi, abi[2].bw
										, abi[1].bi, abi[1].bw
										, abi[0].bi, abi[0].bw
									);
									ptrTag->SetOffsetPos( vVertex(1), vVertex(2), vVertex(3) );
/*
									FLOAT3D normal, axis(0,1,0);
									normal = ((vVertex2 - vVertex0) * (vVertex1 - vVertex0)).SafeNormalize();
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
*/
								}
//안태훈 수정 끝	//(Open beta)(2004-12-01)
							}
						}
					}
				}
			}
			// Unlock buffer arrays
			UnlockMeshReadBuffers(mlod);
		}
	}
	
	ClearRenArrays();
	return fDistance;
}

// yjpark |<--
BOOL RM_TestRayCastHitForTerrainShadow( CModelInstance &mi, FLOATmatrix3D &mRotation, FLOAT3D &vPosition,
											const FLOAT3D &vOrig, const FLOAT3D &vDir )
{
	FLOAT					fDistance = 1E6f;
	extern INDEX			ska_iMaxWeightsPerVertex;
	CObjectRestore<BOOL>	or( _bTransAllToAbsolute, TRUE );
	CObjectRestore<BOOL>	or2( _bUseHardwareShaders, FALSE );
	CObjectRestore<INDEX>	or3( ska_iMaxWeightsPerVertex, 1 );
	
	RM_SetObjectPlacement( mRotation, vPosition );

	// Allways use the first LOD
	RM_SetCurrentDistance( 0 );
	CalculateRenderingData( mi, FALSE );

	// For each ren model
	INDEX	ctrm = _aRenModels.Count();
	for( int irmsh = 1; irmsh < ctrm; irmsh++ )
	{
		RenModel &rm = _aRenModels[irmsh];
		INDEX ctmsh = rm.rm_iFirstMesh + rm.rm_ctMeshes;

		// For each mesh in renmodel
		for( int imsh = rm.rm_iFirstMesh; imsh < ctmsh; imsh++ )
		{
			// Prepare mesh for rendering
			RenMesh	&rmsh = _aRenMeshes[imsh];
			
			// Get mesh lod
			const MeshLOD	&mlod = rmsh.rmsh_pMeshInst->mi_pMesh->msh_aMeshLODs[rmsh.rmsh_iMeshLODIndex];
			
			// Don't do raycast check if mesh wasn't reloaded
			if( mlod.mlod_iBufferBindID == (-1) )
				continue;
			
			// Lock buffer arrays
			LockMeshReadBuffers( mlod );
			PrepareMeshForRendering( rmsh, rm.rm_iSkeletonLODIndex );
			
			// For each surface in mesh
			INDEX	ctmsrf = mlod.mlod_aSurfaces.Count();
			for( INDEX imsrf = 0; imsrf < ctmsrf; imsrf++ )
			{
				const MeshSurface	&msrf = mlod.mlod_aSurfaces[imsrf];
				INDEX	cttris = msrf.msrf_auwTriangles.Count();

				// For each triangle in surface
				for( INDEX itri = 0; itri < cttris; itri++ ) 
				{
					const MeshTriangle	&msht = msrf.msrf_auwTriangles[itri];
					const MeshVertex	&mshv1 = _pavWeightedVertices[msht.mt_uwVertexIndex[0] + msrf.msrf_iFirstVertex];
					const MeshVertex	&mshv2 = _pavWeightedVertices[msht.mt_uwVertexIndex[1] + msrf.msrf_iFirstVertex];
					const MeshVertex	&mshv3 = _pavWeightedVertices[msht.mt_uwVertexIndex[2] + msrf.msrf_iFirstVertex];
					
					FLOAT3D	vVertex0( mshv1.x, mshv1.y, mshv1.z );
					FLOAT3D	vVertex1( mshv2.x, mshv2.y, mshv2.z );
					FLOAT3D	vVertex2( mshv3.x, mshv3.y, mshv3.z );

					FLOAT	fT;
					if( RayHitsTriangle( vOrig, vDir, vVertex0, vVertex1, vVertex2, &fT ) )
					{
						if( fT <= 1.0f )
						{
							// Unlock buffer arrays
							UnlockMeshReadBuffers( mlod );

							ClearRenArrays();
							return TRUE;
						}
					}
				}
			}

			// Unlock buffer arrays
			UnlockMeshReadBuffers( mlod );
		}
	}
	
	ClearRenArrays();
	return FALSE;
}
// yjpark     -->|

// render simple model shadow
void RM_AddSimpleShadow_View(CModelInstance &mi, const FLOAT fIntensity, const FLOATplane3D &plShadowPlane)
{
	// only when rendering to viewport
	ASSERT( _iRenderingType==1);
	if(fIntensity<0.01f || !_aprProjection.IsPerspective()) return;
	
	// get viewer in absolute space
	FLOAT3D vViewerAbs = _aprProjection->ViewerPlacementR().pl_PositionVector;
	// if shadow destination plane is not visible, don't cast shadows
	if( plShadowPlane.PointDistance(vViewerAbs)<0.01f) return;
	
	// get shadow plane in object space
	FLOATmatrix3D mAbsToObj;
	FLOAT3D vAbsToObj;
	
	Matrix12ToMatrixVector(mAbsToObj,vAbsToObj,_mObjToAbs);
	FLOATplane3D plShadowPlaneObj = (plShadowPlane-vAbsToObj) * !mAbsToObj;
	
	// project object handle so we can calc how it is far away from viewer
	FLOAT3D vRef = plShadowPlaneObj.ProjectPoint(FLOAT3D(0,0,0));
	TransformVector(vRef.vector,_mObjToViewStretch);
	plShadowPlaneObj.pl_distance += ClampDn( -vRef(3)*0.005f, 0.01f); // move plane towards the viewer a bit to avoid z-fighting
	
	FLOATaabbox3D box;
	mi.GetCurrentColisionBox( box);
	box.StretchByVector(mi.mi_vStretch);
	// find points on plane nearest to bounding box edges
	FLOAT3D vMin = box.Min() * 1.25f;
	FLOAT3D vMax = box.Max() * 1.25f;
	if( _ulRenFlags & SRMF_SPECTATOR) { vMin*=2; vMax*=2; } // enlarge shadow for 1st person view
	FLOAT3D v00 = plShadowPlaneObj.ProjectPoint(FLOAT3D(vMin(1),vMin(2),vMin(3)));
	FLOAT3D v01 = plShadowPlaneObj.ProjectPoint(FLOAT3D(vMin(1),vMin(2),vMax(3)));
	FLOAT3D v10 = plShadowPlaneObj.ProjectPoint(FLOAT3D(vMax(1),vMin(2),vMin(3)));
	FLOAT3D v11 = plShadowPlaneObj.ProjectPoint(FLOAT3D(vMax(1),vMin(2),vMax(3)));
	TransformVector(v00.vector,_mObjToView);
	TransformVector(v01.vector,_mObjToView);
	TransformVector(v10.vector,_mObjToView);
	TransformVector(v11.vector,_mObjToView);
	
	// prepare color
	ASSERT( fIntensity>=0 && fIntensity<=1);
	ULONG ulAAAA = NormFloatToByte(fIntensity);
	ulAAAA |= (ulAAAA<<8) | (ulAAAA<<16); // alpha isn't needed
	
	// add to vertex arrays
	GFXVertex   *pvtx = _avtxCommon.Push(4);
	GFXColor    *pcol = _acolCommon.Push(4);
	GFXTexCoord *ptex = _atexCommon[0].Push(4);
	// vertices
	pvtx[0].x = v00(1);  pvtx[0].y = v00(2);  pvtx[0].z = v00(3);
	pvtx[2].x = v11(1);  pvtx[2].y = v11(2);  pvtx[2].z = v11(3);
	if( _ulRenFlags & SRMF_INVERTED) 
	{ // must re-adjust order for mirrored projection
		pvtx[1].x = v10(1);  pvtx[1].y = v10(2);  pvtx[1].z = v10(3);
		pvtx[3].x = v01(1);  pvtx[3].y = v01(2);  pvtx[3].z = v01(3);
	}
	else 
	{
		pvtx[1].x = v01(1);  pvtx[1].y = v01(2);  pvtx[1].z = v01(3);
		pvtx[3].x = v10(1);  pvtx[3].y = v10(2);  pvtx[3].z = v10(3);
	}
	// texture coords
	ptex[0].s = 0;  ptex[0].t = 0;
	ptex[1].s = 0;  ptex[1].t = 1;
	ptex[2].s = 1;  ptex[2].t = 1;
	ptex[3].s = 1;  ptex[3].t = 0;
	// colors
	pcol[0].abgr = ulAAAA;
	pcol[1].abgr = ulAAAA;
	pcol[2].abgr = ulAAAA;
	pcol[3].abgr = ulAAAA;
	
	// if this model has fog
	if(_ulRenFlags & SRMF_FOG)
	{ // for each vertex in shadow quad
		GFXTexCoord tex;
		for( INDEX i=0; i<4; i++) 
		{
			GFXVertex &vtx = pvtx[i];
			// get distance along viewer axis and fog axis and map to texture and attenuate shadow color
			const FLOAT fH = vtx.x*_fog_vHDirView(1) + vtx.y*_fog_vHDirView(2) + vtx.z*_fog_vHDirView(3);
			tex.s = -vtx.z *_fog_fMulZ;
			tex.t = (fH+_fog_fAddH) *_fog_fMulH;
			pcol[i].AttenuateRGB(GetFogAlpha(tex)^255);
		}
	}
	// if this model has haze
	if(_ulRenFlags & SRMF_HAZE)
	{ // for each vertex in shadow quad
		for( INDEX i=0; i<4; i++) 
		{
			// get distance along viewer axis map to texture  and attenuate shadow color
			const FLOAT fS = (_haze_fAdd-pvtx[i].z) *_haze_fMul;
			pcol[i].AttenuateRGB(GetHazeAlpha(fS)^255);
		}
	}
}

// set callback function for bone adjustment
void RM_SetBoneAdjustCallback(void (*pAdjustBones)(void *pData), void *pData)
{
	_pAdjustBonesCallback = pAdjustBones;
	_pAdjustBonesData = pData;
}

void RM_SetShaderParamsAdjustCallback(void (*pAdjustShaderParams)(void *pData, INDEX iSurfaceID,CShader *pShader,ShaderParams &spParams),void *pData)
{
	_pAdjustShaderParams = pAdjustShaderParams;
	_pAdjustShaderData = pData;
}

//강동민 수정 시작
// show gound for ska studio
void RM_RenderGround(CTextureObject &to, COLOR color)
{
	CTextureData *ptd = (CTextureData *)to.GetData();
	RM_RenderGround(*ptd, color);
}

void RM_RenderGround(CTextureData &td, COLOR color)
{	
	/*
	gfxEnableDepthTest();
	gfxEnableDepthWrite();
	gfxDisableAlphaTest();
	gfxDisableBlend();
	*/
	gfxCullFace(GFX_NONE);	
	td.SetAsCurrent();
	
	FLOAT3D vVtx = FLOAT3D(70,0,70);
	GFXVertex   vBoxVtxs[4];
	GFXTexCoord tcBoxTex[4];
	GFXColor    colArray[4];
	
	// set ground vertices
	vBoxVtxs[0].x =  vVtx(1); vBoxVtxs[0].y =  vVtx(2); vBoxVtxs[0].z = -vVtx(3);
	vBoxVtxs[1].x = -vVtx(1); vBoxVtxs[1].y =  vVtx(2); vBoxVtxs[1].z = -vVtx(3);
	vBoxVtxs[2].x = -vVtx(1); vBoxVtxs[2].y =  vVtx(2); vBoxVtxs[2].z =  vVtx(3);
	vBoxVtxs[3].x =  vVtx(1); vBoxVtxs[3].y =  vVtx(2); vBoxVtxs[3].z =  vVtx(3);
	
	// set ground color
	COLOR col = color;
	colArray[0] = col; colArray[1] = col; colArray[2] = col; colArray[3] = col;
	
	// set ground texcoords
	tcBoxTex[0].u =  vVtx(1); tcBoxTex[0].v =  0;
	tcBoxTex[1].u =        0; tcBoxTex[1].v =  0;
	tcBoxTex[2].u =        0; tcBoxTex[2].v =  vVtx(3);
	tcBoxTex[3].u =  vVtx(1); tcBoxTex[3].v =  vVtx(3);

	const UWORD auwIndices[6] = { 0,2,1, 0,3,2 };
	
	for( INDEX ivx=0; ivx<4; ivx++) TransformVertex( vBoxVtxs[ivx], _mAbsToViewer);
	
	gfxSetVertexArray( vBoxVtxs, 4);
	gfxSetTexCoordArray( tcBoxTex, FALSE);
	gfxSetColorArray(&colArray[0]);
	gfxDrawElements( 6, auwIndices);
}
//강동민 수정 끝

//강동민 수정 시작
// Function name	: RM_RenderQuad
// Description	    : 사각형을 그립니다.
// Return type		: void 
void RM_RenderQuad(CPlacement3D& plPosition, FLOAT fWidth)
{
	Matrix12 matObjToAbs;
	FLOATmatrix3D m;
	//MakeRotationMatrixFast(m, plPosition.pl_OrientationAngle);
	MakeRotationMatrixFast(m, FLOAT3D(0.0f, 0.0f, 0.0f));
	MatrixVectorToMatrix12(matObjToAbs, m, plPosition.pl_PositionVector);
	Matrix12 matObjToView;
	MatrixMultiply(matObjToView, _mAbsToViewer, matObjToAbs);
	gfxSetViewMatrix(NULL);

	FLOAT3D vVtx = FLOAT3D(fWidth, 0, fWidth);
	GFXVertex   vQuadVtxs[4];
	GFXTexCoord tcQuadTex[4];
	GFXColor    colArray[4];
	
	// set ground vertices
	vQuadVtxs[0].x =  vVtx(1); vQuadVtxs[0].y =  vVtx(2); vQuadVtxs[0].z = -vVtx(3);
	vQuadVtxs[1].x = -vVtx(1); vQuadVtxs[1].y =  vVtx(2); vQuadVtxs[1].z = -vVtx(3);
	vQuadVtxs[2].x = -vVtx(1); vQuadVtxs[2].y =  vVtx(2); vQuadVtxs[2].z =  vVtx(3);
	vQuadVtxs[3].x =  vVtx(1); vQuadVtxs[3].y =  vVtx(2); vQuadVtxs[3].z =  vVtx(3);

	// set ground color
	COLOR col = 0xFFFFFFFF;
	colArray[0] = col; colArray[1] = col; colArray[2] = col; colArray[3] = col;
	
	// set ground texcoords
	tcQuadTex[0].u =  1;			tcQuadTex[0].v =  0;
	tcQuadTex[1].u =  0;			tcQuadTex[1].v =  0;
	tcQuadTex[2].u =  0;			tcQuadTex[2].v =  1;
	tcQuadTex[3].u =  1;			tcQuadTex[3].v =  1;
	const UWORD auwIndices[6] = { 0,2,1, 0,3,2 };

	for( INDEX ivx=0; ivx<4; ivx++) TransformVertex( vQuadVtxs[ivx], matObjToView);
	
	gfxSetVertexArray( vQuadVtxs, 4);
	gfxSetTexCoordArray( tcQuadTex, FALSE);
	gfxSetColorArray(&colArray[0]);
	
	gfxDrawElements( 6, auwIndices);
}
//강동민 수정 끝


// render wirerame bounding box
static void RenderWireframeBox(FLOAT3D vMinVtx, FLOAT3D vMaxVtx, COLOR col)
{
	// prepare wireframe settings
	gfxDisableTexture();
	// fill vertex array so it represents bounding box
	FLOAT3D vBoxVtxs[8];
	vBoxVtxs[0] = FLOAT3D( vMinVtx(1), vMinVtx(2), vMinVtx(3));
	vBoxVtxs[1] = FLOAT3D( vMaxVtx(1), vMinVtx(2), vMinVtx(3));
	vBoxVtxs[2] = FLOAT3D( vMaxVtx(1), vMinVtx(2), vMaxVtx(3));
	vBoxVtxs[3] = FLOAT3D( vMinVtx(1), vMinVtx(2), vMaxVtx(3));
	vBoxVtxs[4] = FLOAT3D( vMinVtx(1), vMaxVtx(2), vMinVtx(3));
	vBoxVtxs[5] = FLOAT3D( vMaxVtx(1), vMaxVtx(2), vMinVtx(3));
	vBoxVtxs[6] = FLOAT3D( vMaxVtx(1), vMaxVtx(2), vMaxVtx(3));
	vBoxVtxs[7] = FLOAT3D( vMinVtx(1), vMaxVtx(2), vMaxVtx(3));
	
	for(INDEX iwx=0;iwx<8;iwx++) TransformVector(vBoxVtxs[iwx].vector,_mObjToViewStretch);
	
	// connect vertices into lines of bounding box
	INDEX iBoxLines[12][2];
	iBoxLines[ 0][0] = 0;  iBoxLines[ 0][1] = 1;  iBoxLines[ 1][0] = 1;  iBoxLines[ 1][1] = 2;
	iBoxLines[ 2][0] = 2;  iBoxLines[ 2][1] = 3;  iBoxLines[ 3][0] = 3;  iBoxLines[ 3][1] = 0;
	iBoxLines[ 4][0] = 0;  iBoxLines[ 4][1] = 4;  iBoxLines[ 5][0] = 1;  iBoxLines[ 5][1] = 5;
	iBoxLines[ 6][0] = 2;  iBoxLines[ 6][1] = 6;  iBoxLines[ 7][0] = 3;  iBoxLines[ 7][1] = 7;
	iBoxLines[ 8][0] = 4;  iBoxLines[ 8][1] = 5;  iBoxLines[ 9][0] = 5;  iBoxLines[ 9][1] = 6;
	iBoxLines[10][0] = 6;  iBoxLines[10][1] = 7;  iBoxLines[11][0] = 7;  iBoxLines[11][1] = 4;
	// for all vertices in bounding box
	for( INDEX i=0; i<12; i++) 
	{
		// get starting and ending vertices of one line
		FLOAT3D &v0 = vBoxVtxs[iBoxLines[i][0]];
		FLOAT3D &v1 = vBoxVtxs[iBoxLines[i][1]];
		_pdp->DrawLine3D(v0,v1,col);
	} 
}


// render bounding box
static void RenderBox(FLOAT3D vMinVtx, FLOAT3D vMaxVtx, COLOR col)
{
	// prepare settings
	gfxDisableTexture();
	gfxEnableBlend();
	gfxBlendFunc(GFX_SRC_ALPHA, GFX_INV_SRC_ALPHA);
	gfxCullFace(GFX_NONE);
	gfxDisableDepthWrite();
	
	GFXColor colArray[8];
	colArray[0] = col; colArray[1] = col; colArray[2] = col;
	colArray[3] = col; colArray[3] = col; colArray[4] = col;
	colArray[5] = col; colArray[6] = col; colArray[7] = col;
	// gfxSetConstantColor(col);
	// fill vertex array so it represents bounding box
	GFXVertex vBoxVtxs[8];
	vBoxVtxs[0].x = vMinVtx(1);  vBoxVtxs[0].y = vMaxVtx(2);  vBoxVtxs[0].z = vMinVtx(3);
	vBoxVtxs[1].x = vMinVtx(1);  vBoxVtxs[1].y = vMaxVtx(2);  vBoxVtxs[1].z = vMaxVtx(3);
	vBoxVtxs[2].x = vMaxVtx(1);  vBoxVtxs[2].y = vMaxVtx(2);  vBoxVtxs[2].z = vMinVtx(3);
	vBoxVtxs[3].x = vMaxVtx(1);  vBoxVtxs[3].y = vMaxVtx(2);  vBoxVtxs[3].z = vMaxVtx(3);
	vBoxVtxs[4].x = vMinVtx(1);  vBoxVtxs[4].y = vMinVtx(2);  vBoxVtxs[4].z = vMinVtx(3);
	vBoxVtxs[5].x = vMinVtx(1);  vBoxVtxs[5].y = vMinVtx(2);  vBoxVtxs[5].z = vMaxVtx(3);
	vBoxVtxs[6].x = vMaxVtx(1);  vBoxVtxs[6].y = vMinVtx(2);  vBoxVtxs[6].z = vMinVtx(3);
	vBoxVtxs[7].x = vMaxVtx(1);  vBoxVtxs[7].y = vMinVtx(2);  vBoxVtxs[7].z = vMaxVtx(3);
	
	const UWORD auwIndices[36] = 
	{
		0,3,1, 0,2,3, 5,1,3, 7,5,3,
			2,7,3, 6,7,2, 4,2,0, 4,6,2,
			5,0,1, 5,4,0, 4,5,7, 6,4,7 
	};
	
	for(INDEX iwx=0;iwx<8;iwx++) TransformVertex(vBoxVtxs[iwx],_mObjToViewStretch);
	
	gfxSetVertexArray( vBoxVtxs,8);
	gfxSetColorArray(&colArray[0]);
	gfxDrawElements( 36, auwIndices);
	
	gfxDisableBlend();
	gfxEnableDepthTest();
	
	RenderWireframeBox( vMinVtx, vMaxVtx, C_BLACK|CT_OPAQUE);
	gfxEnableDepthWrite();
	gfxDisableDepthBias();
}


// render bounding box on screen
void RM_RenderColisionBox(CModelInstance &mi,ColisionBox &cb, COLOR col)
{
	gfxSetViewMatrix(NULL);
	gfxDisableBlend();
	gfxDisableAlphaTest();
	gfxEnableDepthTest();
	gfxEnableDepthWrite();
	
	if(RM_GetFlags() & RMF_WIREFRAME) 
	{
		RenderWireframeBox(cb.Min(),cb.Max(),col|CT_OPAQUE);
	}
	else 
	{
		gfxEnableBlend();
		gfxBlendFunc(GFX_SRC_ALPHA, GFX_INV_SRC_ALPHA);
		RenderBox(cb.Min(),cb.Max(),col|0x7F);
	}
}


void RM_RenderCross(FLOAT3D &vPosition, COLOR col)
{
	gfxSetViewMatrix(NULL);
	gfxDisableBlend();
	gfxDisableAlphaTest();
	gfxDisableDepthTest();
	gfxDisableDepthWrite();
	
	FLOAT3D vPoint[6];
	const FLOAT fLen = 0.08f;
	vPoint[0] = vPosition+FLOAT3D(fLen,0,0);
	vPoint[1] = vPosition-FLOAT3D(fLen,0,0);
	vPoint[2] = vPosition+FLOAT3D(0,fLen,0);
	vPoint[3] = vPosition-FLOAT3D(0,fLen,0);
	vPoint[4] = vPosition+FLOAT3D(0,0,fLen);
	vPoint[5] = vPosition-FLOAT3D(0,0,fLen);
	for(INDEX ipt=0;ipt<6;ipt++) 
	{
		TransformVector(vPoint[ipt].vector,_mObjToView);
	}
	_pdp->DrawLine3D(vPoint[0],vPoint[1],col);
	_pdp->DrawLine3D(vPoint[2],vPoint[3],col);
	_pdp->DrawLine3D(vPoint[4],vPoint[5],col);
}

//안태훈 수정 시작	//(Effect Add & Modify for Close Beta)(0.1)
void RM_RenderSpline(SINT uiCount, FLOAT3D *pvPosition, SINT selIndex, COLOR col)
{
	if(uiCount < 2) return;
	if(pvPosition == NULL) return;

	gfxSetViewMatrix(NULL);
	gfxDisableBlend();
	gfxDisableAlphaTest();
	gfxDisableDepthTest();
	gfxDisableDepthWrite();
	
	FLOAT3D *pvPoint = new FLOAT3D[uiCount];

	pvPoint[0] = pvPosition[0];
	TransformVector(pvPoint[0].vector, _mObjToView);
	for(UINT i=1; i<uiCount; ++i) 
	{
		pvPoint[i] = pvPosition[i];
		TransformVector(pvPoint[i].vector, _mObjToView);
		_pdp->DrawLine3D(pvPoint[i-1], pvPoint[i], col);
	}
	delete[] pvPoint;

	if(selIndex < 0 || selIndex >= uiCount) return;

	FLOAT3D vPoint[6];
	const FLOAT fLen = 0.08f;
	vPoint[0] = pvPosition[selIndex] + FLOAT3D(fLen,0,0);
	vPoint[1] = pvPosition[selIndex] - FLOAT3D(fLen,0,0);
	vPoint[2] = pvPosition[selIndex] + FLOAT3D(0,fLen,0);
	vPoint[3] = pvPosition[selIndex] - FLOAT3D(0,fLen,0);
	vPoint[4] = pvPosition[selIndex] + FLOAT3D(0,0,fLen);
	vPoint[5] = pvPosition[selIndex] - FLOAT3D(0,0,fLen);
	for(INDEX ipt=0;ipt<6;ipt++) 
	{
		TransformVector(vPoint[ipt].vector, _mObjToView);
	}
	_pdp->DrawLine3D(vPoint[0],vPoint[1],col);
	_pdp->DrawLine3D(vPoint[2],vPoint[3],col);
	_pdp->DrawLine3D(vPoint[4],vPoint[5],col);
}
//안태훈 수정 끝	//(Effect Add & Modify for Close Beta)(0.1)

// render one renbone
static void RenderBone(RenBone &rb, COLOR col)
{
	FLOAT fSize = rb.rb_psbBone->sb_fBoneLength / 20;
	FLOAT3D vBoneStart = FLOAT3D(rb.rb_mBonePlacement[3],rb.rb_mBonePlacement[7],rb.rb_mBonePlacement[11]);
	FLOAT3D vBoneEnd = FLOAT3D(0,0,-rb.rb_psbBone->sb_fBoneLength);
	FLOAT3D vRingPt[4];
	INDEX	il;
	
	vRingPt[0] = FLOAT3D(-fSize,-fSize,-fSize*2);
	vRingPt[1] = FLOAT3D( fSize,-fSize,-fSize*2);
	vRingPt[2] = FLOAT3D( fSize, fSize,-fSize*2);
	vRingPt[3] = FLOAT3D(-fSize, fSize,-fSize*2);
	TransformVector(vBoneEnd.vector,rb.rb_mBonePlacement);
	TransformVector(vRingPt[0].vector,rb.rb_mBonePlacement);
	TransformVector(vRingPt[1].vector,rb.rb_mBonePlacement);
	TransformVector(vRingPt[2].vector,rb.rb_mBonePlacement);
	TransformVector(vRingPt[3].vector,rb.rb_mBonePlacement);
	
	// connect start point of bone with end point
	for( il=0;il<4;il++) 
	{
		_pdp->DrawLine3D(vBoneStart,vRingPt[il],col);
		_pdp->DrawLine3D(vBoneEnd,vRingPt[il],col);
	}
	
	// draw ring
	for( il = 0; il < 3; il++)
	{
		_pdp->DrawLine3D(vRingPt[il],vRingPt[il+1],col);
	}
	_pdp->DrawLine3D(vRingPt[0],vRingPt[3],col);
}

void RM_RenderBone(CModelInstance &mi,INDEX iBoneID)
{
	ASSERTALWAYS("Not implemented");
}

// render skeleton hierarchy
static void RenderSkeleton(void)
{
	SetViewMatrix(_mObjToView);
	// for each bone, except the dummy one
	for(int irb=1; irb<_aRenBones.Count(); irb++) 
	{
		RenBone &rb = _aRenBones[irb];
		RenderBone(rb,0x5A5ADCFF); // render in blue color
	}
}

static void RenderActiveBones(RenModel &rm)
{
	CModelInstance *pmi = rm.rm_pmiModel;
	if(pmi==NULL) return;
	// count animlists
	INDEX ctal = pmi->mi_aqAnims.aq_Lists.Count();
	// find newes animlist that has fully faded in
	INDEX iFirstAnimList = 0;
	// loop from newer to older
	INDEX ial;
	for( ial=ctal-1;ial>=0;ial--) 
	{
		AnimList &al = pmi->mi_aqAnims.aq_Lists[ial];
		// calculate fade factor
		FLOAT fFadeFactor = CalculateFadeFactor(al);
		if(fFadeFactor >= 1.0f) 
		{
			iFirstAnimList = ial;
			break;
		}
	}
	// for each anim list after iFirstAnimList
	for( ial = iFirstAnimList; ial < ctal; ial++) 
	{
		AnimList &al = pmi->mi_aqAnims.aq_Lists[ial];
		INDEX ctpa = al.al_PlayedAnims.Count();
		// for each played anim
		for(INDEX ipa=0;ipa<ctpa;ipa++) 
		{
			PlayedAnim &pa = al.al_PlayedAnims[ipa];
			INDEX iAnimSet,iAnimIndex;
			if(pmi->FindAnimationByID(pa.pa_iAnimID,&iAnimSet,&iAnimIndex)) 
			{
				CAnimSet &as = pmi->mi_aAnimSet[iAnimSet];
				Animation &an = as.as_Anims[iAnimIndex];
				INDEX ctbe = an.an_abeBones.Count();
				// for each bone envelope
				for(INDEX ibe=0;ibe<ctbe;ibe++) 
				{
					BoneEnvelope &be = an.an_abeBones[ibe];
					// try to find renbone for this bone envelope
					INDEX iBoneIndex = FindRenBoneIndex(rm,be.be_iBoneID);
					if(iBoneIndex!=(-1)) 
					{
						RenBone &rb = _aRenBones[iBoneIndex];
						// render bone
						RenderBone(rb,0x00FF00FF);
					}
				}
			}
		}
	}
}

static void RenderActiveBones(void)
{
	SetViewMatrix(_mObjToView);
	// for each renmodel
	INDEX ctrm = _aRenModels.Count();
	for(INT irm=0;irm<ctrm;irm++) 
	{
		RenModel &rm = _aRenModels[irm];
		RenderActiveBones(rm);
	}
}

// get render flags for model
ULONG &RM_GetRenderFlags()
{
	return _ulRenFlags;
}
// set new flag
void RM_SetFlags(ULONG ulNewFlags)
{
	_ulFlags = ulNewFlags;
}
// get curent flags
ULONG RM_GetFlags()
{
	return _ulFlags;
}
// add flag
void RM_AddFlag(ULONG ulFlag)
{
	_ulFlags |= ulFlag;
}
// remove flag
void RM_RemoveFlag(ULONG ulFlag)
{
	_ulFlags &= ~ulFlag;
}

// find texture data id 

static void FindTextureData(CTextureObject **ptoTextures, INDEX iTextureID, MeshInstance &mshi)
{
	// for each texture instances
	INDEX ctti=mshi.mi_tiTextures.Count();
	for(INDEX iti=0;iti<ctti;iti++)
	{
		TextureInstance &ti = mshi.mi_tiTextures[iti];
		if(ti.GetID() == iTextureID)
		{
			*ptoTextures = &ti.ti_toTexture;
			return;
		}
	}
	*ptoTextures = NULL;
}

#if NDEBUG
#define FindFrame FindFrameNew
#else 
#define FindFrame FindFrameAll
#endif


/*
static ULONG _ulnffRequest = 0;
static ULONG _ulnffTests = 0;
static ULONG _uloffRequest = 0;
static ULONG _uloffTests = 0;
*/

//  find frame (binary) index in compresed array of rotations, positions or opt_rotations
static inline INDEX FindFrameNew(UBYTE *pFirstMember, INDEX iFind, INDEX ctfn, UINT uiSize, UWORD iHighFrameNum, FLOAT fInvStep)
{
	const INDEX iFrameMid  = (INDEX)(iFind*fInvStep);                   // _ulnffRequest++;
	ASSERT(iFrameMid>=0 && iFrameMid<ctfn);
	const INDEX iFrameHit = *(UWORD*)(pFirstMember+(uiSize*iFrameMid)); // _ulnffTests++;
	INDEX iLastHit = iFrameMid;
	
	// if at first hit
	if(iFrameHit==iFind) 
	{
		// return frame
		return iFrameMid;
		// if found frame is larger than searched frame
	}
	else if(iFrameHit>iFind) 
	{
		// foop backward lineary
		for(INDEX ifr=iFrameMid-1;ifr>=0;ifr--) 
		{
			INDEX iNewHit = *(UWORD*)(pFirstMember+(uiSize*ifr));           // _ulnffTests++;
			if(iNewHit<=iFind) 
			{
				return ifr;
			}
		}
		// if found frame is smaller than searched frame
	}
	else 
	{
		// loop forward lineary
		for(INDEX ifr=iFrameMid+1;ifr<ctfn;ifr++) 
		{
			INDEX iNewHit = *(UWORD*)(pFirstMember+(uiSize*ifr));           // _ulnffTests++;
			if(iNewHit==iFind) 
			{
				return ifr;
			}
			if(iNewHit>iFind) 
			{
				return ifr-1;
			}
			iLastHit = iNewHit;
		}
	}
	
	ASSERT(FALSE); // should never get here
	return 0;
}

static inline INDEX FindFrameOld(UBYTE *pFirstMember, INDEX iFind, INDEX ctfn, UINT uiSize, UWORD iHighFrameNum, FLOAT fInvStep)
{
	INDEX iHigh = ctfn-1;                                               // _uloffRequest++;
	INDEX iLow = 0;
	INDEX iMid;
	
	if(iFind == iHighFrameNum) return iHigh;
	
	while(TRUE) 
	{
		iMid = (iHigh+iLow)>>1;
		const UBYTE *pubMidOffset = pFirstMember+(uiSize*iMid);           // _uloffTests++;
		const INDEX iMidFrameNum = *(UWORD*)pubMidOffset;
		const INDEX iMidFrameNumPlusOne = *(UWORD*)(pubMidOffset+uiSize); // _uloffTests++;
		if(iFind < iMidFrameNum) 
		{
			iHigh = iMid;
		}
		else if((iMid == iHigh) || (iMidFrameNumPlusOne > iFind)) 
		{
			return iMid;
		}
		else 
		{
			iLow = iMid;
		}
	}
}

static inline INDEX FindFrameAll(UBYTE *pFirstMember, INDEX iFind, INDEX ctfn, UINT uiSize, UWORD iHighFrameNum, FLOAT fInvStep)
{
	INDEX iOldFind = FindFrameOld(pFirstMember, iFind, ctfn, uiSize, iHighFrameNum, fInvStep);
	INDEX iNewFind = FindFrameNew(pFirstMember, iFind, ctfn, uiSize, iHighFrameNum, fInvStep);
	
	ASSERT(iOldFind==iNewFind);
	return iNewFind;
}

// Find renbone by renbone ID in array of renbones 
inline static INDEX FindRenBoneIndex(const RenModel &rm, INDEX iBoneID)
{
#if 1
// 	ASSERT(rm.rm_pmiModel!=NULL);
// 	ASSERT(rm.rm_pmiModel->mi_psklSkeleton!=NULL);
	
	const INDEX iSkeletonLOD = rm.rm_iSkeletonLODIndex;
	const CSkeleton *pskl = rm.rm_pmiModel->mi_psklSkeleton;
	const INDEX iBoneIndex = pskl->FindBoneInLOD(iBoneID,iSkeletonLOD);
	if(iBoneIndex!=(-1)) 
	{
		const INDEX iRenBoneIndex = rm.rm_iFirstBone + iBoneIndex;
		return iRenBoneIndex;
	}
	else 
	{
		return iBoneIndex;
	}
#endif
}

// find renmoph in given renmodel
static BOOL FindRenMorphIndex(RenModel &rm,int iMorphID,INDEX *piMorphIndex)
{
	// for each renmesh in given renmodel
	INDEX ctmsh = rm.rm_iFirstMesh + rm.rm_ctMeshes;
	for(INDEX irmsh=rm.rm_iFirstMesh;irmsh<ctmsh;irmsh++) 
	{
		// for each renmorph in this renmesh
		INDEX ctmm = _aRenMeshes[irmsh].rmsh_iFirstMorph + _aRenMeshes[irmsh].rmsh_ctMorphs;
		for(INDEX imm=_aRenMeshes[irmsh].rmsh_iFirstMorph;imm<ctmm;imm++) 
		{
			// if id's match
			if(iMorphID == _aRenMorphs[imm].rmp_pmmmMorphMap->mmp_iID) 
			{
				// return this renmorph
				*piMorphIndex = imm;
				return TRUE;
			}
		}
	}
	// renmorph was not found
	return FALSE;
}

// Find renbone in whole array on renbones
RenBone *RM_FindRenBone(INDEX iBoneID)
{
	INDEX ctrb=_aRenBones.Count();
	// for each renbone
	for(INDEX irb=1;irb<ctrb;irb++) 
	{
		RenBone &rb = _aRenBones[irb];
		// if bone id's match
		if(rb.rb_psbBone->sb_iID == iBoneID) 
		{
			// return this renbone
			return &rb;
		}
	}
	return NULL;
}

// Return array of renbones
RenBone *RM_GetRenBoneArray(INDEX &ctrb)
{
	ctrb = _aRenBones.Count();
	if(ctrb>0) 
	{
		return &_aRenBones[0];
	}
	else 
	{
		return NULL;
	}
}


static INDEX FindBone(INDEX iBoneID, const CModelInstance *pmi, INDEX iSkeletonLOD)
{
	if(pmi->mi_psklSkeleton==NULL || iSkeletonLOD==(-1)) 
	{
		return -1;
	}
	
	const INDEX iBoneIndex = pmi->mi_psklSkeleton->FindBoneInLOD(iBoneID,iSkeletonLOD);
	
#if 0
	if(iBoneIndex!=(-1)) 
	{
		return iBoneIndex;
	}
	else 
	{
		// for each child model instance
		INDEX ctmic = pmi->mi_cmiChildren.Count();
		for(INDEX imic=0;imic<ctmic;imic++) 
		{
			const CModelInstance *pmic = pmi->mi_cmiChildren[imic];
			// try to find bone in 
			const INDEX iBoneIndex2 = FindBone(iBoneID,pmic,iSkeletonLOD);
			if(iBoneIndex2!=(-1)) 
			{
				return iBoneIndex2;
			}
		}
	}
#else 
	return iBoneIndex;
#endif
	return -1;
}

// decompres axis for quaternion if animations are optimized
static void DecompressAxis(FLOAT3D &vNormal, UWORD ubH, UWORD ubP)
{
	ANGLE h = (ubH/65535.0f)*360.0f-180.0f;
	ANGLE p = (ubP/65535.0f)*360.0f-180.0f;
	
	FLOAT &x = vNormal(1);
	FLOAT &y = vNormal(2);
	FLOAT &z = vNormal(3);
	
	x = -Sin(h)*Cos(p);
	y = Sin(p);
	z = -Cos(h)*Cos(p);
}

ENGINE_API extern BOOL RM_AreHardwareShadersAvailable(void)
{
//강동민 수정 시작 접속 시퀀스 작업	06.01
	extern INDEX ska_bUseHardwareShaders;			// 원본.
	return ska_bUseHardwareShaders && 
		(_pGfx->gl_ulFlags&GLF_VERTEXPROGRAM) && 
		(_pGfx->gl_ulFlags&GLF_PIXELPROGRAM) && 
		(_pGfx->gl_ctTextureUnits>=4);
	// 강제로 HardwareShader 사용하지 않기.
	//return FALSE;
//강동민 수정 끝 접속 시퀀스 작업	06.01
}

// initialize batch model rendering
extern void RM_BeginRenderingView( CAnyProjection3D &apr, CDrawPort *pdp)
{
	CProjection3D *pprProjection = NULL;
	if(apr.IsPerspective()) 
	{
		pprProjection = (CProjection3D*)apr;
	}
	_fMipAdjustFactor = RM_GetMipAdjustmentFactor(pprProjection);
	
	
	// remember parameters
	_iRenderingType = 1;
	_pdp = pdp;
	_ulDrawPortID = pdp->GetID();
	// prepare and set the projection
	apr->ObjectPlacementL() = CPlacement3D(FLOAT3D(0,0,0), ANGLE3D(0,0,0));
	apr->Prepare();
	// in case of mirror projection, move mirror clip plane a bit father from the mirrored models,
	// so we have less clipping (for instance, player feet)
	if( apr->pr_bMirror) apr->pr_plMirrorView.pl_distance -= 0.06f; // -0.06 is because entire projection is offseted by +0.05

//강동민 수정 시작 Water 구현		04.13
	else if(apr->pr_bNiceWater)
		apr->pr_plNiceWaterView.pl_distance -= 0.06f;
//강동민 수정 끝 Water 구현			04.13

	_aprProjection = apr;
	_pdp->SetProjection( _aprProjection);
	
	// remember the abs to viewer transformation
	MatrixVectorToMatrix12(_mAbsToViewer,
		_aprProjection->pr_ViewerRotationMatrix, 
		-_aprProjection->pr_vViewerPosition*_aprProjection->pr_ViewerRotationMatrix);
	
	// make FPU precision low
	_fpuOldPrecision = GetFPUPrecision(); 
	SetFPUPrecision(FPT_24BIT);
	
	// decide if vertex and pixel programs will be used
	_bUseHardwareShaders = RM_AreHardwareShadersAvailable();
	
}

// cleanup after batch model rendering
extern void RM_EndRenderingView( BOOL bRestoreOrtho/*=TRUE*/)
{
	ASSERT( _iRenderingType==1 && _pdp!=NULL);
	// assure that FPU precision was low all the model rendering time, then revert to old FPU precision
	ASSERT( GetFPUPrecision()==FPT_24BIT);
	SetFPUPrecision(_fpuOldPrecision);
	
	// re-enable color mask (maybe occ test left it disabled)
	gfxSetColorMask(CT_RMASK|CT_GMASK|CT_BMASK|CT_AMASK);
	
	_fMipAdjustFactor = 1.0f;
	
	// back to 2D projection?
	if( bRestoreOrtho) _pdp->SetOrtho();
	_iRenderingType = 0;
	_pdp = NULL;
}



// for mark renderer
extern CAnyProjection3D _aprProjection;
extern UBYTE *_pubMask;
extern SLONG _slMaskWidth;
extern SLONG _slMaskHeight;

// begin/end model rendering to shadow mask
extern void RM_BeginModelRenderingMask( CAnyProjection3D &prProjection, UBYTE *pubMask, SLONG slMaskWidth, SLONG slMaskHeight)
{
	ASSERT( _iRenderingType==0);
	_iRenderingType = 2;
	_aprProjection  = prProjection;
	_pubMask      = pubMask;
	_slMaskWidth  = slMaskWidth; 
	_slMaskHeight = slMaskHeight; 
	
	// prepare and set the projection
	_aprProjection->ObjectPlacementL() = CPlacement3D(FLOAT3D(0,0,0), ANGLE3D(0,0,0));
	_aprProjection->Prepare();
	// remember the abs to viewer transformation
	MatrixVectorToMatrix12(_mAbsToViewer,
		_aprProjection->pr_ViewerRotationMatrix, 
		-_aprProjection->pr_vViewerPosition*_aprProjection->pr_ViewerRotationMatrix);
	
	// set mask shader
	extern void InternalShader_Mask(void);
//안태훈 수정 시작	//(For Performance)(0.1)
	extern void InternalShaderDesc_Mask(ShaderDesc *&pshDesc);
//안태훈 수정 끝	//(For Performance)(0.1)
	_shMaskShader.ShaderFunc    = InternalShader_Mask;
	_shMaskShader.GetShaderDesc = InternalShaderDesc_Mask;
}


extern void RM_EndModelRenderingMask(void)
{
	ASSERT( _iRenderingType==2);
	_iRenderingType = 0;
}






// setup light parameters
extern void RM_SetLightColor(COLOR colAmbient, COLOR colLight)
{
	_colAmbient = colAmbient;
	_colLight = colLight;
}
extern void RM_SetLightDirection(FLOAT3D &vLightDir)
{
	_vLightDir = vLightDir.Normalize() * (-1);
	// _vLightDir = vLightDir * (-1);
}


// calculate object matrices for givem model instance
extern void RM_SetObjectMatrices(CModelInstance &mi)
{
	const ULONG ulFlags = RM_GetRenderFlags();
	
	// adjust clipping to frustum
	if(ulFlags&SRMF_INSIDE) 
	{
		gfxDisableClipping();
	}
	else 
	{
		gfxEnableClipping();
	}
	
	// adjust clipping to mirror-plane (if any)
	extern INDEX gap_iOptimizeClipping;
	
	if((CProjection3D *)_aprProjection != NULL) 
	{
//강동민 수정 시작 Water 구현		04.21
		//if( gap_iOptimizeClipping>0 && (_aprProjection->pr_bMirror || _aprProjection->pr_bWarp))		// 원본.
		if( gap_iOptimizeClipping>0 && (_aprProjection->pr_bMirror || _aprProjection->pr_bWarp || _aprProjection->pr_bNiceWater))
//강동민 수정 끝 Water 구현			04.21
		{
			if( ulFlags & SRMF_INMIRROR) 
			{
//강동민 수정 시작 접속 시퀀스 작업	05.22
				//gfxDisableClipPlane();	// 원본.
				gfxEnableClipPlane();
//강동민 수정 끝 접속 시퀀스 작업	05.22
				gfxFrontFace( GFX_CCW);
			}
			else 
			{				
//강동민 수정 시작 Water 구현		04.21
				gfxEnableClipPlane();		// 원본.
				// NOTE :  Mirror에 대해서 ClipPlane()을 Enable할 경우,
				// NOTE : 문제가 발생함.
				// NOTE : Disable해도 Mirror된 이미지가 제대로 나오지 않는 문제가 있음.
				// NOTE : 고로, ClipPlane을 설정하는 부분이 문제가 되므로, 
				// NOTE : ClipPlane을 계산하는 부분을 수정하도록 할것.
				// NOTE : 또, 다른 한가지 문제점은 클리핑시에 오브젝트의 바운딩 박스를 가지고 하는데,
				// NOTE : 반사 평면이 바운딩 박스의 중간에 있을 경우가 문제가 된다.
				// NOTE : 이 경우에도 따로 처리를 해줘야 할듯.
				// NOTE : 궁금한점, 왜 BSP 기반의 물체들에 대해서는 잘 나오고 있는 것일까?
				// NOTE : 위의 문제를 해결하기 전까지는 Disable Clip함.
				//gfxDisableClipPlane();
//강동민 수정 끝 Water 구현			04.21
				gfxFrontFace( GFX_CW);
			}
		}
	}
	
	MatrixMultiply(_mObjToView,_mAbsToViewer, _mObjToAbs);
	
	Matrix12 mStretch;
	MakeStretchMatrix(mStretch, mi.mi_vStretch);
	MatrixMultiply(_mObjToViewStretch,_mObjToView,mStretch);
}


// setup object position
extern void RM_SetObjectPlacement(const CPlacement3D &pl)
{
	FLOATmatrix3D m;
	MakeRotationMatrixFast( m, pl.pl_OrientationAngle);
	MatrixVectorToMatrix12(_mObjToAbs,m, pl.pl_PositionVector);
}

extern void RM_SetObjectPlacement(const FLOATmatrix3D &m, const FLOAT3D &v)
{
	MatrixVectorToMatrix12(_mObjToAbs,m, v);
}

//강동민 수정 시작
extern void RM_SetAbsToView(const Matrix12 &matAbsToView)
{
	memcpy(&_mAbsToViewer, &matAbsToView, sizeof(Matrix12));
}

// NOTE : _iRenderingType가 여러곳에서 선언되어있음.  나중에 수정해야할 부분.
extern void RM_SetRenderingType(INDEX iRenderingType)
{
	_iRenderingType = iRenderingType;
}

/*
// NOTE : RenderModelShadow 및 CreateShadowMatrix, RenderOneSkaModelToTexture등에서 중복되는 코드가 많음.
// NOTE : 매 프레임마다 계산해야 하는 부분.  광원의 위치나 모델의 위치, 뷰어의 위치등이 변경될 수 있다.
extern void CreateShadowMatrix(CModelInstance *pMI, const CPlacement3D& plModel, Matrix16& matShadowUV, FLOAT fTextureSize, const CPlacement3D& plViewer)
{
	gfxSetViewMatrix(NULL);
	
	//////////////////////////////////////////////////////////////////////////	
	// matView 계산.
	// 카메라(뷰어)에서 원점을 바라보는 투영&뷰 행렬...(투영행렬은 필요없음... 단지, 뷰 행렬을 얻기 위한 부분.)
	Matrix12 matViewToAbs;						// Viewer -> Abs
	MakeIdentityMatrix(matViewToAbs);
	
	// 카메라에서 바라보는 뷰행렬...
	// World -> View(AbsToView)
	GetViewMatrix(plViewer, matViewToAbs);
	//////////////////////////////////////////////////////////////////////////
	// 임시코드
	FLOATaabbox3D FrameBBox;
	pMI->GetAllFramesBBox(FrameBBox);
	FLOAT3D vMin = FrameBBox.minvect;
	FLOAT3D vMax = FrameBBox.maxvect;
	FLOAT fHeight = fabs((vMax(2) - vMin(2))/2.0f);	

	//////////////////////////////////////////////////////////////////////////
	// 월드 행렬...
	Matrix12 matLocalToWorld;
	GetWorldMatrix(plModel, matLocalToWorld);
	
	//////////////////////////////////////////////////////////////////////////
	// matView의 역행렬 구하기...
	// Camera -> World(카메라에서 바라보는 뷰행렬의 역행렬.)
	Matrix12 matCameraToAbs;
	MatrixTranspose(matCameraToAbs, matViewToAbs);

	//////////////////////////////////////////////////////////////////////////
	// 텍스쳐투영 행렬 공식.
	// matShadowUV = matCameraToWorld * matWorldToLight * matShadowProj * (scale * offset)
	//////////////////////////////////////////////////////////////////////////	

	// NOTE : 아래 텍스쳐투영 행렬 계산하는 부분에서 D3D 함수들을 사용하고 있다.  수정할것.
	// matWorldToLight^T
	D3DXMATRIX matWorldToLight16;
	D3DXMatrixIdentity(&matWorldToLight16);
	memcpy(&matWorldToLight16, pMI->mi_matWorldToLight, sizeof(float)*12);
	D3DXMatrixTranspose(&matWorldToLight16, &matWorldToLight16);

	// 찍힌 그림자를 모델의 위치로 이동시킴...(World)
	D3DXMATRIX matTranslation;
	D3DXMatrixIdentity(&matTranslation);
	// NOTE : 왜 부호가 -일때만 되는것인가?
	D3DXMatrixTranslation(&matTranslation, -plModel.pl_PositionVector(1), -plModel.pl_PositionVector(2), -plModel.pl_PositionVector(3));
	
	D3DXMATRIX matModelTrans;
	D3DXMatrixIdentity(&matModelTrans);
	D3DXMatrixTranslation(&matModelTrans, 0.0f, -fHeight, 0.0f);

	// matCameraToLocal^T
	D3DXMATRIX matCameraToAbs16;
	D3DXMatrixIdentity(&matCameraToAbs16);
	memcpy(&matCameraToAbs16, matCameraToAbs, sizeof(float)*12);
	D3DXMatrixTranspose(&matCameraToAbs16, &matCameraToAbs16);

	D3DXMATRIX matCameraToLight;
	D3DXMatrixIdentity(&matCameraToLight);

	// Camera -> World -> Translation -> Light
	D3DXMatrixMultiply(&matCameraToLight, &matCameraToAbs16, &matModelTrans);
	D3DXMatrixMultiply(&matCameraToLight, &matCameraToLight, &matTranslation);
	D3DXMatrixMultiply(&matCameraToLight, &matCameraToLight, &matWorldToLight16);	

	// NOTE : 전치행렬의 사용에 대해서...
	// matTemp12 = (matWorldToLight * matCameraToAbs)^(T);
	//			 = matCameraToAbs^T * matWorldToLight^T;

	// 최종적인 텍스쳐 투영 행렬.
	// matCameraToWorld * matWorldToLight * matShadowProj
	Matrix16 matTemp16;
	memcpy(&matTemp16, &matCameraToLight, sizeof(float)*16);
	MatrixMultiply(matShadowUV, matTemp16, pMI->mi_matShadowProj);

	// 텍스쳐 좌표로 변환함.
	// -1~1 사이의 값을 [0, 1]의 값으로 변경합니다.
	Matrix16 matScaleOffset;
	memset(&matScaleOffset, 0, sizeof(float)*16);
	matScaleOffset[0] = 0.5f;
	matScaleOffset[5] = -0.5f;
	matScaleOffset[10] = 1.0f;

	matScaleOffset[12] = 0.5f;
	matScaleOffset[13] = 0.5f;
	matScaleOffset[14] = 0.0f;
	matScaleOffset[15] = 1.0f;
	
	MatrixMultiply(matShadowUV, matShadowUV, matScaleOffset);
}*/
//강동민 수정 시작		03.05
// NOTE : RenderModelShadow 및 CreateShadowMatrix, RenderOneSkaModelToTexture등에서 중복되는 코드가 많음.
// NOTE : 매 프레임마다 계산해야 하는 부분.  광원의 위치나 모델의 위치, 뷰어의 위치등이 변경될 수 있다.
//강동민 수정 시작		04.13
extern void CreateShadowMatrix(CModelInstance *pMI, const CPlacement3D& plModel, Matrix16& matShadowUV, 
							   const Matrix12& matWorldToLight,		// 광원에서 원점을 바라봄...
							   const Matrix16& matShadowProj,		// 광원에서 투영한 행렬.
							   const FLOAT fTextureSize, 
							   const CPlacement3D& plViewer)		// 카메라의 위치.
//강동민 수정 끝		04.13
{
	gfxSetViewMatrix(NULL);
	
	//////////////////////////////////////////////////////////////////////////	
	// matView 계산.
	// 카메라(뷰어)에서 원점을 바라보는 투영&뷰 행렬...(투영행렬은 필요없음... 단지, 뷰 행렬을 얻기 위한 부분.)
	Matrix12 matViewToAbs;						// Viewer -> Abs
	MakeIdentityMatrix(matViewToAbs);
	
	// 카메라에서 바라보는 뷰행렬...
	// World -> View(AbsToView)
	GetViewMatrix(plViewer, matViewToAbs);
	//////////////////////////////////////////////////////////////////////////
	// 임시코드
	FLOATaabbox3D FrameBBox;
	pMI->GetAllFramesBBox(FrameBBox);
//강동민 수정 시작		03.20
//강동민 수정 시작 테스트 클라이언트 작업	06.27
	// NOTE : 직교 투영으로 변환 후, Stretch에 문제가 생겨서 추가함.
	FrameBBox.StretchByVector(pMI->mi_vStretch);
//강동민 수정 끝 테스트 클라이언트 작업		06.27
//강동민 수정 끝		03.20
	FLOAT3D vMin = FrameBBox.minvect;
	FLOAT3D vMax = FrameBBox.maxvect;
	FLOAT fHeight = fabs((vMax(2) - vMin(2))/2.0f);	

	//////////////////////////////////////////////////////////////////////////
	// 월드 행렬...
//강동민 수정 시작		04.13
	//Matrix12 matLocalToWorld;
	//GetWorldMatrix(plModel, matLocalToWorld);
//강동민 수정 끝		04.13
	
	//////////////////////////////////////////////////////////////////////////
	// matView의 역행렬 구하기...
	// Camera -> World(카메라에서 바라보는 뷰행렬의 역행렬.)
	Matrix12 matCameraToAbs;
	MatrixTranspose(matCameraToAbs, matViewToAbs);

	//////////////////////////////////////////////////////////////////////////
	// 텍스쳐투영 행렬 공식.
	// matShadowUV = matCameraToWorld * matWorldToLight * matShadowProj * (scale * offset)
	//////////////////////////////////////////////////////////////////////////	

	// NOTE : 아래 텍스쳐투영 행렬 계산하는 부분에서 D3D 함수들을 사용하고 있다.  수정할것.
	// matWorldToLight^T
	D3DXMATRIX matWorldToLight16;
	D3DXMatrixIdentity(&matWorldToLight16);
	memcpy(&matWorldToLight16, matWorldToLight, sizeof(float)*12);
	D3DXMatrixTranspose(&matWorldToLight16, &matWorldToLight16);

	// 찍힌 그림자를 모델의 위치로 이동시킴...(World)
	D3DXMATRIX matTranslation;
	D3DXMatrixIdentity(&matTranslation);
	// NOTE : 왜 부호가 -일때만 되는것인가?
	D3DXMatrixTranslation(&matTranslation, -plModel.pl_PositionVector(1), -plModel.pl_PositionVector(2), -plModel.pl_PositionVector(3));
	
	D3DXMATRIX matModelTrans;
	D3DXMatrixIdentity(&matModelTrans);
	D3DXMatrixTranslation(&matModelTrans, 0.0f, -fHeight, 0.0f);

	// matCameraToLocal^T
	D3DXMATRIX matCameraToAbs16;
	D3DXMatrixIdentity(&matCameraToAbs16);
	memcpy(&matCameraToAbs16, matCameraToAbs, sizeof(float)*12);
	D3DXMatrixTranspose(&matCameraToAbs16, &matCameraToAbs16);

	D3DXMATRIX matCameraToLight;
	D3DXMatrixIdentity(&matCameraToLight);

	// Camera -> World -> Translation -> Light
	D3DXMatrixMultiply(&matCameraToLight, &matCameraToAbs16, &matModelTrans);
	D3DXMatrixMultiply(&matCameraToLight, &matCameraToLight, &matTranslation);
	D3DXMatrixMultiply(&matCameraToLight, &matCameraToLight, &matWorldToLight16);	

	// NOTE : 전치행렬의 사용에 대해서...
	// matTemp12 = (matWorldToLight * matCameraToAbs)^(T);
	//			 = matCameraToAbs^T * matWorldToLight^T;

	// 최종적인 텍스쳐 투영 행렬.
	// matCameraToWorld * matWorldToLight * matShadowProj
	Matrix16 matTemp16;
	memcpy(&matTemp16, &matCameraToLight, sizeof(float)*16);
	MatrixMultiply(matShadowUV, matTemp16, matShadowProj);

	// 텍스쳐 좌표로 변환함.
	// -1~1 사이의 값을 [0, 1]의 값으로 변경합니다.
	Matrix16 matScaleOffset;
	memset(&matScaleOffset, 0, sizeof(float)*16);
	matScaleOffset[0] = 0.5f;
	matScaleOffset[5] = -0.5f;
	matScaleOffset[10] = 1.0f;

	matScaleOffset[12] = 0.5f;
	matScaleOffset[13] = 0.5f;
	matScaleOffset[14] = 0.0f;
	matScaleOffset[15] = 1.0f;
	
	MatrixMultiply(matShadowUV, matShadowUV, matScaleOffset);
}
//강동민 수정 끝		03.05

extern void GetWorldMatrix(const CPlacement3D &plPosition, Matrix12& matObjToAbs)
{
	FLOATmatrix3D m;
	MakeRotationMatrixFast( m, plPosition.pl_OrientationAngle);
	MatrixVectorToMatrix12(matObjToAbs, m, plPosition.pl_PositionVector);
}

// 아래 함수는 Shadow Matrix를 구하기 위해서만 사용해야 합니다.
// NOTE : 나중에 CPerspectiveProjection3D에서 직접 투영행렬값을 얻어오도록 처리할것.
extern void GetViewMatrix(const CPlacement3D &plEye, Matrix12& matAbsToEye)
{
	// Eye에서 Abs를 바라보도록 설정.
	CPerspectiveProjection3D prEyeToAbs;	// Eye -> World	

	// 아랫부분은 단지 View Matrix를 얻기 위한 부분입니다
	// NOTE : 아랫부분에서 ViewerPlacement를 제외하고는 거의 디폴트 값이므로 설정해줄 필요가 없을거 같기도 하구...
	prEyeToAbs.FOVL() = AngleDeg(45);
	prEyeToAbs.ScreenBBoxL() = FLOATaabbox2D( FLOAT2D(0.0f, 0.0f), FLOAT2D(SHADOWTEXTURESIZE, SHADOWTEXTURESIZE));
	prEyeToAbs.AspectRatioL() = 1.0f;
	prEyeToAbs.FrontClipDistanceL() = 0.05f;
	prEyeToAbs.ViewerPlacementL() = plEye;
	prEyeToAbs.Prepare();

	// Abs -> Eye(AbsToEye(View))
	MatrixVectorToMatrix12(matAbsToEye,
		prEyeToAbs.pr_ViewerRotationMatrix,
		-prEyeToAbs.pr_vViewerPosition*prEyeToAbs.pr_ViewerRotationMatrix);
}

extern void FindBestFOV(const FLOATaabbox3D& aabbBox, const Matrix12& matLocalToLight, FLOAT& fX, FLOAT& fY)
{
	//-----------------------------------------------------------------------------
	// FOV를 자동으로 계산하기 위한 루틴.
	// NOTE : 어느 정도 계산을 되고 있으나, 모델 중심이 0, 0, 0 인데 모델의 크기의 절반만큼 아래로 이동시켜줘야 할듯.
	FLOAT3D vMin = aabbBox.minvect;
	FLOAT3D vMax = aabbBox.maxvect;

	// FrameBBox의 8점을 구하고, 그 점을 변환하여 FOV를 구합니다.
	FLOAT3D v[8];
	v[0] = vMin;v[1] = vMin;v[2] = vMin;v[3] = vMin;
	v[1](1) = vMax(1);
	v[2](2) = vMax(2);
	v[3](3) = vMax(3);

	v[4] = vMax;v[5] = vMax;v[6] = vMax;v[7] = vMax;
	v[5](1) = vMin(1);
	v[6](2) = vMin(2);
	v[7](3) = vMin(3);

	float fXMax = 0.0f;		//최대값을 구할것이므로 0으로 시작하면 된다.
	float fYMax = 0.0f;
	FLOAT3D vOut;
	
	for(int i = 0; i < 8; i++)
	{
		GFXVertex vVertex;
		memcpy(&vVertex, &v[i], sizeof(float)*3);
		
		TransformVertex(vVertex, matLocalToLight);
		
		memcpy(&vOut, &vVertex, sizeof(float)*3);
		if((float)fabs(vOut(1)/vOut(3)) > fXMax) fXMax = (float)fabs(vOut(1)/vOut(3));
		if((float)fabs(vOut(2)/vOut(3)) > fYMax) fYMax = (float)fabs(vOut(2)/vOut(3));
	}

	fX = fXMax;
	fY = fYMax;
}

extern HRESULT SetAdjustedProjectionMatrix( Matrix16& mat, FLOAT fFOV, FLOAT fAspect,
										   FLOAT fNearPlane, FLOAT fFarPlane,
										   FLOAT fPixDx, FLOAT fPixDy, FLOAT fVPWidth, 
										   FLOAT fVPHeight, BOOL bPerspective )
{
    if( fabs(fFarPlane-fNearPlane) < 0.01f )
        return E_INVALIDARG;
    if( fabs(sin(fFOV/2)) < 0.01f )
        return E_INVALIDARG;
	
    float h =   1.0f  * ( cosf(fFOV/2)/sinf(fFOV/2) );
	
	// 투영행렬의 Factor를 설정합니다.
    float fTop = fNearPlane/h;
    float fBottom = -fTop;
    float fRight = fTop * fAspect;
    float fLeft = -fRight;
	
    float fXWSize = fRight - fLeft;
    float fYWSize = fTop - fBottom;
	
    float fDx = -( fPixDx*fXWSize/fVPWidth );
    float fDy = -( fPixDy*fYWSize/fVPHeight );
    
	// 원근 투영행렬 생성.
	if(bPerspective)
	{
		SetFrustumMatrix( mat, fLeft+fDx, fRight+fDx, fTop+fDy, fBottom+fDy, 
			fNearPlane,
			fFarPlane );
	}
	// 직교 투영행렬 생성.
	else
	{
		SetOrthoMatrix( mat, fLeft+fDx, fRight+fDx, fTop+fDy, fBottom+fDy, 
			fNearPlane,
			fFarPlane );
	}
	
    return S_OK;
}

// 원근 투영 행렬을 구합니다.
extern void SetFrustumMatrix( Matrix16& mat, 
							 FLOAT fLeft, FLOAT fRight, FLOAT fTop,
							 FLOAT fBottom, FLOAT fNearPlane, FLOAT fFarPlane )
{
	// 직접 투영행렬을 계산하는 부분...
    float fT = fFarPlane / ( fFarPlane - fNearPlane );
	
    ZeroMemory( &mat, sizeof(Matrix16) );
	/*
	2*zn/(r-l)   0            0                0
	0            2*zn*(t-b)   0                0
	(l+r)/(r-l)  (t+b)/(t-b)  zf/(zn-zf)      -1
	0            0            zn*zf/(zn-zf)    0
	*/
	
    mat[0] = ( 2.0f*fNearPlane )/( fRight - fLeft );
    mat[5] = ( 2.0f*fNearPlane )/( fTop - fBottom );
    mat[8] = ( fRight + fLeft )/ (fRight - fLeft );
    mat[9] = ( fTop + fBottom )/ (fTop - fBottom );
    mat[10] = -fT;
    mat[11] = -1.0f;
    mat[14] = -fT*fNearPlane;
}

// 직교 투영 행렬을 구합니다.
extern void SetOrthoMatrix( Matrix16& mat, 
						   FLOAT fLeft, FLOAT fRight, FLOAT fTop, 
						   FLOAT fBottom, FLOAT fNearPlane, FLOAT fFarPlane )
{
	// 직접 투영행렬을 계산하는 부분...
    ZeroMemory( &mat, sizeof(Matrix16) );

	/*
	2/(r-l)      0            0           0
	0            2/(t-b)      0           0
	0            0            1/(zn-zf)   0
	(l+r)/(l-r)  (t+b)/(b-t)  zn/(zn-zf)  1
	*/
	mat[0] = 2.0f/( fRight - fLeft );
    mat[5] = 2.0f/( fTop - fBottom );
    mat[10] = 1.0f/(fNearPlane - fFarPlane);
	mat[12] = (fLeft + fRight)/(fLeft - fRight);
	mat[13] = (fTop + fBottom)/(fBottom - fTop);
    mat[14] = fNearPlane/(fNearPlane - fFarPlane);
	mat[15] = 1.0f;
}
//강동민 수정 끝

// Sets max weights that can effect one vertex
extern void RM_SetMaxWeightsPerVertex(const INDEX ctWeightsPerVertex)
{
	_ctMaxWeightsPerVertex = ctWeightsPerVertex;
}

// Returns index of skeleton lod at given distance
extern INDEX GetSkeletonLOD(CSkeleton &sk, FLOAT fDistance)
{
	FLOAT fMinDistance = UpperLimit(1.0f);
	INDEX iSkeletonLod = -1;
	
	// for each lod in skeleton
	INDEX ctslods = sk.skl_aSkeletonLODs.Count();
	for(INDEX islod=0;islod<ctslods;islod++) 
	{
		const SkeletonLOD &slod = sk.skl_aSkeletonLODs[islod];
		// adjust lod distance by custom settings
		const FLOAT fLodMaxDistance = slod.slod_fMaxDistance*ska_fLODMul+ska_fLODAdd;
		
		// check if this lod max distance is smaller than distance to object
		if(fDistance < fLodMaxDistance && fLodMaxDistance < fMinDistance) 
		{
			// remember this lod
			fMinDistance = fLodMaxDistance;
			iSkeletonLod = islod;
		}
	}
	return iSkeletonLod;
}

// Returns index of mesh lod at given distance
extern INDEX GetMeshLOD(CMesh &msh, FLOAT fDistance)
{
	FLOAT fMinDistance = UpperLimit(1.0f);
	INDEX iMeshLod = -1;
	
	// for each lod in mesh
	INDEX ctmlods = msh.msh_aMeshLODs.Count();
	for(INDEX imlod=0;imlod<ctmlods;imlod++) 
	{
		const MeshLOD &mlod = msh.msh_aMeshLODs[imlod];
		// adjust lod distance by custom settings
		const FLOAT fLodMaxDistance = mlod.mlod_fMaxDistance*ska_fLODMul+ska_fLODAdd;
		
		// check if this lod max distance is smaller than distance to object
		if(fDistance<fLodMaxDistance && fLodMaxDistance<fMinDistance) 
		{
			// remember this lod
			fMinDistance = fLodMaxDistance;
			iMeshLod = imlod;
		}
	}
	return iMeshLod;
}

__forceinline extern INDEX GetWeightsPerVertex(const MeshSurface &msrf, FLOAT fDistance)
{
	extern INDEX ska_iMaxWeightsPerVertex;
	return Clamp(_ctMaxWeightsPerVertex,1L,ska_iMaxWeightsPerVertex);
}

__forceinline RenBone &AddRenBone(INDEX iBoneID)
{
	// Add new renbone
	RenBone &rb = _aRenBones.Push();
	// Add one entry to renbone search table
	// SLONG &slBoneID = _aslRenBoneSearchTable.Push();
	// slBoneID = iBoneID;
	// return created renbone
	return rb;
}

// create first dummy model that serves as parent for the entire hierarchy
static void MakeRootModel(void)
{
	ASSERT(_aRenModels.Count()==0);
	ASSERT(_aRenBones.Count()==0);
	
	// create the model with one bone
	RenModel &rm = _aRenModels.Push();
	rm.rm_pmiModel = NULL;
	rm.rm_iFirstBone = 0;
	rm.rm_ctBones = 1;
	rm.rm_iParentBoneIndex = -1;
	rm.rm_iParentModelIndex = -1;
	rm.rm_iSkeletonLODIndex = -1;
	rm.rm_bInverted = FALSE;
	
	// add the default bone
	RenBone &rb = AddRenBone(-1);
	// RenBone &rb = _aRenBones.Push();
	rb.rb_iParentIndex = -1;
	rb.rb_psbBone = NULL;
	memset(&rb.rb_apPos,0,sizeof(AnimPos));
	memset(&rb.rb_arRot,0,sizeof(AnimRot));
}

// build model hierarchy
static INDEX BuildHierarchy(CModelInstance *pmiModel, INDEX irmParent)
{
	INDEX ctrm = _aRenModels.Count();
	INDEX iRenModel = _aRenModels.Count();
	// add one renmodel
	RenModel &rm = _aRenModels.Push();
	RenModel &rmParent = _aRenModels[irmParent];
	
	rm.rm_pmiModel = pmiModel;
	rm.rm_iParentModelIndex = irmParent;
	rm.rm_iNextSiblingModel = -1;
	rm.rm_iSkeletonLODIndex = -1;
	rm.rm_iFirstBone = _aRenBones.Count();
	rm.rm_ctBones = 0;
	
	// if this model is root model
	if(pmiModel->mi_iParentBoneID == (-1)) 
	{
		// if root model does not have skeleton
		if(rmParent.rm_ctBones>0) 
		{
			// set is parent bone index as first bone in parent model
			rm.rm_iParentBoneIndex = rmParent.rm_iFirstBone;
		}
		else 
		{
			// do not draw this model
			_aRenModels.Pop();
			return -1;
		}
		// model instance is attached to another model's bone 
	}
	else 
	{
		INDEX iParentBoneIndex = -1;
		// does parent model insntance has a skeleton
		if(ska_bAnimateMesh && rmParent.rm_pmiModel && rmParent.rm_pmiModel->mi_psklSkeleton!=NULL && rmParent.rm_iSkeletonLODIndex>=0)  
		{
			// get index of parent bone
			iParentBoneIndex = rmParent.rm_pmiModel->mi_psklSkeleton->FindBoneInLOD(pmiModel->mi_iParentBoneID,rmParent.rm_iSkeletonLODIndex);
			// model instance does not have skeleton
		}
		else 
		{
			// do not draw this model
			_aRenModels.Pop();
			return -1;
		}
		// if parent bone index was not found ( not visible in current lod)
		if(iParentBoneIndex == (-1)) 
		{
			// do not draw this model
			_aRenModels.Pop();
			return -1;
			// parent bone exists and its visible
		}
		else 
		{
			// set this model parent bone index in array of renbones
			rm.rm_iParentBoneIndex = iParentBoneIndex + rmParent.rm_iFirstBone;
		}
	}
	
	// Check if model is inverted
	const BOOL bXInverted = rm.rm_pmiModel->mi_vStretch(1) < 0;
	const BOOL bYInverted = rm.rm_pmiModel->mi_vStretch(2) < 0;
	const BOOL bZInverted = rm.rm_pmiModel->mi_vStretch(3) < 0;
	
	rm.rm_bInverted = (bXInverted != bYInverted != bZInverted != rmParent.rm_bInverted);
	
	// if this model instance has skeleton
	if( ska_bAnimateMesh && pmiModel->mi_psklSkeleton!=NULL)
	{
		// adjust mip factor in case of dynamic stretch factor
		FLOAT fDistFactor = _fDistanceFactor;
		const FLOAT3D &vStretch = pmiModel->mi_vStretch;
		// if model is stretched 
		if( vStretch != FLOAT3D(1,1,1)) 
		{
			// calculate new distance factor
			fDistFactor = fDistFactor / Max(vStretch(1),Max(vStretch(2),vStretch(3)));
		}
		// calulate its current skeleton lod
		rm.rm_iSkeletonLODIndex = GetSkeletonLOD(*pmiModel->mi_psklSkeleton,fDistFactor);
		// if current skeleton lod is valid and visible
		if(rm.rm_iSkeletonLODIndex > -1) 
		{
			SkeletonLOD &slod = pmiModel->mi_psklSkeleton->skl_aSkeletonLODs[rm.rm_iSkeletonLODIndex];
			// count all bones in this skeleton
			INDEX ctsb = slod.slod_aBones.Count();
			// for each bone in skeleton
			for(INDEX irb=0;irb<ctsb;irb++) 
			{
				SkeletonBone *psbone = &slod.slod_aBones[irb];
				// add one renbone
				RenBone &rb = AddRenBone(psbone->sb_iID);
				rb.rb_psbBone = psbone;
				rb.rb_iRenModelIndex = ctrm;
				rb.rb_fOffsetLen = psbone->sb_fOffSetLen;
				rm.rm_ctBones++;
				// add default bone position (used if no animations)
				rb.rb_apPos.ap_vPos = psbone->sb_qvRelPlacement.vPos;
				rb.rb_arRot.ar_qRot = psbone->sb_qvRelPlacement.qRot;
				rb.rb_apTempPos.ap_vPos = rb.rb_apPos.ap_vPos;
				rb.rb_arTempRot.ar_qRot = rb.rb_arRot.ar_qRot;
				
				// if this is root bone for this model instance
				if(psbone->sb_iParentID == (-1)) 
				{
					// set its parent bone index to be parent bone of this model instance
					rb.rb_iParentIndex = rm.rm_iParentBoneIndex;
					// this is child bone
				}
				else 
				{
					// get parent index in array of renbones
					INDEX rb_iParentIndex = pmiModel->mi_psklSkeleton->FindBoneInLOD(psbone->sb_iParentID,rm.rm_iSkeletonLODIndex);
					rb.rb_iParentIndex = rb_iParentIndex + rm.rm_iFirstBone;
				}
			}
		} // count skeletons
		_sfStats.IncrementCounter( CStatForm::SCI_SKELETONS);
	}
	
	rm.rm_iFirstMesh = _aRenMeshes.Count();
	rm.rm_ctMeshes = 0;
	
	INDEX ctm = pmiModel->mi_aMeshInst.Count();
	// for each mesh instance in this model instance
	for(INDEX im=0;im<ctm;im++) 
	{
		// adjust mip factor in case of dynamic stretch factor
		FLOAT fDistFactor = _fDistanceFactor;
		const FLOAT3D &vStretch = pmiModel->mi_vStretch;
		// if model is stretched 
		if( vStretch != FLOAT3D(1,1,1)) 
		{
			// calculate new distance factor
			fDistFactor = fDistFactor / Max(vStretch(1),Max(vStretch(2),vStretch(3)));
		}
		
		// calculate current mesh lod
		INDEX iMeshLodIndex = GetMeshLOD(*pmiModel->mi_aMeshInst[im].mi_pMesh,fDistFactor);
		// if mesh lod is visible
		if(iMeshLodIndex > -1) 
		{
			MeshInstance &mshi = pmiModel->mi_aMeshInst[im];
			MeshLOD &mlod = mshi.mi_pMesh->msh_aMeshLODs[iMeshLodIndex];
			if(_bTransAllToAbsolute&&(mlod.mlod_ulFlags&ML_FULL_FACE_FORWARD)) 
			{
				continue;
			}
			// add one ren mesh
			RenMesh &rmsh = _aRenMeshes.Push();
			rm.rm_ctMeshes++;
			rmsh.rmsh_iRenModelIndex = ctrm;
			rmsh.rmsh_pMeshInst = &mshi;
			rmsh.rmsh_iFirstMorph = _aRenMorphs.Count();
			rmsh.rmsh_iFirstWeight = _aRenWeights.Count();
			rmsh.rmsh_ctMorphs = 0;
			rmsh.rmsh_ctWeights = 0;
			// set mesh lod index for this ren mesh
			rmsh.rmsh_iMeshLODIndex = iMeshLodIndex;
			
			// skip processing of morph/weight maps if animation has been supressed
			if( !ska_bAnimateMesh) continue;
			
			// for each morph map in this mesh lod
			INDEX ctmm = mlod.mlod_aMorphMaps.Count();
			for(INDEX imm=0;imm<ctmm;imm++) 
			{
				// add this morph map in array of renmorphs
				MeshMorphMap &mmm = mlod.mlod_aMorphMaps[imm];
				RenMorph &rmp = _aRenMorphs.Push();
				rmsh.rmsh_ctMorphs++;
				rmp.rmp_pmmmMorphMap = &mmm;
				rmp.rmp_fFactor = 0;
				rmp.rmp_fTempFactor = 0;
			}
			
			// for each weight map in this mesh lod
			INDEX ctwm = mlod.mlod_aWeightMaps.Count();
			for(INDEX iwm=0;iwm<ctwm;iwm++) 
			{
				// add this weight map in array of renweights
				RenWeight &rw = _aRenWeights.Push();
				MeshWeightMap &mwm = mlod.mlod_aWeightMaps[iwm];
				rw.rw_pwmWeightMap = &mwm;
				rmsh.rmsh_ctWeights++;
				rw.rw_iBoneIndex = rm.rm_iFirstBone;
				const INDEX iBoneIndex = FindBone(mwm.mwm_iID,pmiModel,rm.rm_iSkeletonLODIndex);
				if(iBoneIndex!=(-1)) 
				{
					rw.rw_iBoneIndex += iBoneIndex;
				}
				else 
				{
					rw.rw_iBoneIndex = -1;
				}
				/*
				// find bone of this weight in current skeleton lod and get its index for this renweight
				if(!FindBone(mwm.mwm_iID,&rw.rw_iBoneIndex,pmiModel,rm.rm_iSkeletonLODIndex)) 
				{
				// if bone not found, set boneindex in renweight to -1
				rw.rw_iBoneIndex = -1;
				}
				*/
			}
		}
	}
	
	rm.rm_iFirstChildModel = -1;
	// for each child in this model instance
	INDEX ctmich = pmiModel->mi_cmiChildren.Count();
	for(INDEX imich=0;imich<ctmich;imich++) 
	{
		// build hierarchy for child model instance
		CModelInstance *pmiChild = &pmiModel->mi_cmiChildren[imich];
		ASSERT(pmiChild!=NULL);
		INDEX irmChildIndex = BuildHierarchy(pmiChild,ctrm);
		// if child is visible 
		if(irmChildIndex != (-1)) 
		{
			// set model sibling
			_aRenModels[irmChildIndex].rm_iNextSiblingModel = _aRenModels[iRenModel].rm_iFirstChildModel;
			_aRenModels[iRenModel].rm_iFirstChildModel = irmChildIndex;
		}
	}
	return ctrm;
}

static void DummyCalculateBoneTransforms()
{
	Matrix12 mIdentity;
	
	MakeIdentityMatrix(mIdentity);
	
	// for each renbone after first dummy one
	for(int irb=0; irb<_aRenBones.Count(); irb++) 
	{
		RenBone &rb = _aRenBones[irb];
		MatrixCopy(rb.rb_mTransform,mIdentity);
		MatrixCopy(rb.rb_mStrTransform,mIdentity);
	}
	
	// for each renmodel after first dummy one
	for(int irm=1; irm<_aRenModels.Count(); irm++) 
	{
		RenModel &rm = _aRenModels[irm];
		MatrixCopy(rm.rm_mTransform,mIdentity);
		MatrixCopy(rm.rm_mStrTransform,mIdentity);
	}
}

// calculate transformations for all bones on already built hierarchy
//안태훈 수정 시작	//(Add & Modify SSSE Effect)(0.1)
void SetSkaTagInfo(CSkaTag *ptrTag, INDEX ctrb, RenModel &rm, MeshLOD &mlod)
{
	Matrix12 matrix1, matrix2, matrix3, matrix4;
	Matrix12 matrixStr1, matrixStr2, matrixStr3, matrixStr4;
	const Matrix12 &mObjToAbs = _mObjToAbs;
	//웨이트 정보가 없거나 현재 본이 없을때
	if(ptrTag->GetBoneCount() == 0 || ctrb == 1)
	{
		MatrixMultiply(matrix1, mObjToAbs, rm.rm_mTransform);
		MatrixMultiply(matrixStr1, mObjToAbs, rm.rm_mStrTransform);
		ptrTag->AddNewTagInfo(_pTimer->GetLerpedCurrentTick(), matrix1, matrixStr1);
	}
	else
	{
		switch(ptrTag->GetBoneCount())
		{
		case 1:
			{
				INDEX iBoneIndex = 0;
				iBoneIndex = FindBone(mlod.mlod_aWeightMaps[ptrTag->GetBoneID(0)].mwm_iID, rm.rm_pmiModel, rm.rm_iSkeletonLODIndex);
				RenBone &rb1 = _aRenBones[ iBoneIndex + rm.rm_iFirstBone ];
				MatrixMultiply(matrix1, mObjToAbs, rb1.rb_mTransform);
				MatrixMultiply(matrixStr1, mObjToAbs, rb1.rb_mStrTransform);
				ptrTag->AddNewTagInfo(_pTimer->GetLerpedCurrentTick(), matrix1, matrixStr1);
			} break;
		case 2:
			{
				INDEX iBoneIndex = 0;
				iBoneIndex = FindBone(mlod.mlod_aWeightMaps[ptrTag->GetBoneID(0)].mwm_iID, rm.rm_pmiModel, rm.rm_iSkeletonLODIndex);
				RenBone &rb1 = _aRenBones[ iBoneIndex + rm.rm_iFirstBone ];
				MatrixMultiply(matrix1, mObjToAbs, rb1.rb_mTransform);
				MatrixMultiply(matrixStr1, mObjToAbs, rb1.rb_mStrTransform);
				iBoneIndex = FindBone(mlod.mlod_aWeightMaps[ptrTag->GetBoneID(1)].mwm_iID, rm.rm_pmiModel, rm.rm_iSkeletonLODIndex);
				RenBone &rb2 = _aRenBones[ iBoneIndex + rm.rm_iFirstBone ];
				MatrixMultiply(matrix2, mObjToAbs, rb2.rb_mTransform);
				MatrixMultiply(matrixStr2, mObjToAbs, rb2.rb_mStrTransform);
				ptrTag->AddNewTagInfo(_pTimer->GetLerpedCurrentTick(), matrix1, matrixStr1, matrix2, matrixStr2);
			} break;
		case 3:
			{
				INDEX iBoneIndex = 0;
				iBoneIndex = FindBone(mlod.mlod_aWeightMaps[ptrTag->GetBoneID(0)].mwm_iID, rm.rm_pmiModel, rm.rm_iSkeletonLODIndex);
				RenBone &rb1 = _aRenBones[ iBoneIndex + rm.rm_iFirstBone ];
				MatrixMultiply(matrix1, mObjToAbs, rb1.rb_mTransform);
				MatrixMultiply(matrixStr1, mObjToAbs, rb1.rb_mStrTransform);
				iBoneIndex = FindBone(mlod.mlod_aWeightMaps[ptrTag->GetBoneID(1)].mwm_iID, rm.rm_pmiModel, rm.rm_iSkeletonLODIndex);
				RenBone &rb2 = _aRenBones[ iBoneIndex + rm.rm_iFirstBone ];
				MatrixMultiply(matrix2, mObjToAbs, rb2.rb_mTransform);
				MatrixMultiply(matrixStr2, mObjToAbs, rb2.rb_mStrTransform);
				iBoneIndex = FindBone(mlod.mlod_aWeightMaps[ptrTag->GetBoneID(2)].mwm_iID, rm.rm_pmiModel, rm.rm_iSkeletonLODIndex);
				RenBone &rb3 = _aRenBones[ iBoneIndex + rm.rm_iFirstBone ];
				MatrixMultiply(matrix3, mObjToAbs, rb3.rb_mTransform);
				MatrixMultiply(matrixStr3, mObjToAbs, rb3.rb_mStrTransform);
				ptrTag->AddNewTagInfo(_pTimer->GetLerpedCurrentTick(), matrix1, matrixStr1, matrix2, matrixStr2
														, matrix3, matrixStr3);
			} break;
		case 4:
			{
				INDEX iBoneIndex = 0;
				iBoneIndex = FindBone(mlod.mlod_aWeightMaps[ptrTag->GetBoneID(0)].mwm_iID, rm.rm_pmiModel, rm.rm_iSkeletonLODIndex);
				RenBone &rb1 = _aRenBones[ iBoneIndex + rm.rm_iFirstBone ];
				MatrixMultiply(matrix1, mObjToAbs, rb1.rb_mTransform);
				MatrixMultiply(matrixStr1, mObjToAbs, rb1.rb_mStrTransform);
				iBoneIndex = FindBone(mlod.mlod_aWeightMaps[ptrTag->GetBoneID(1)].mwm_iID, rm.rm_pmiModel, rm.rm_iSkeletonLODIndex);
				RenBone &rb2 = _aRenBones[ iBoneIndex + rm.rm_iFirstBone ];
				MatrixMultiply(matrix2, mObjToAbs, rb2.rb_mTransform);
				MatrixMultiply(matrixStr2, mObjToAbs, rb2.rb_mStrTransform);
				iBoneIndex = FindBone(mlod.mlod_aWeightMaps[ptrTag->GetBoneID(2)].mwm_iID, rm.rm_pmiModel, rm.rm_iSkeletonLODIndex);
				RenBone &rb3 = _aRenBones[ iBoneIndex + rm.rm_iFirstBone ];
				MatrixMultiply(matrix3, mObjToAbs, rb3.rb_mTransform);
				MatrixMultiply(matrixStr3, mObjToAbs, rb3.rb_mStrTransform);
				iBoneIndex = FindBone(mlod.mlod_aWeightMaps[ptrTag->GetBoneID(3)].mwm_iID, rm.rm_pmiModel, rm.rm_iSkeletonLODIndex);
				RenBone &rb4 = _aRenBones[ iBoneIndex + rm.rm_iFirstBone ];
				MatrixMultiply(matrix4, mObjToAbs, rb4.rb_mTransform);
				MatrixMultiply(matrixStr4, mObjToAbs, rb4.rb_mStrTransform);
				ptrTag->AddNewTagInfo(_pTimer->GetLerpedCurrentTick(), matrix1, matrixStr1, matrix2, matrixStr2
														, matrix3, matrixStr3, matrix4, matrixStr4);
			} break;
		default: ASSERTALWAYS("여기로 들어오면 안됨. 본의 개수 이상"); break;
		}
	}
}

static void CalculateBoneTransforms(BOOL bRenderToScreen)
//안태훈 수정 끝	//(Add & Modify SSSE Effect)(0.1)
{
	// put basic transformation in first dummy bone
	if(_bTransAllToAbsolute) 
	{
		MatrixCopy(_aRenBones[0].rb_mTransform, _mObjToAbs);
	}
	else 
	{
		MakeIdentityMatrix(_aRenBones[0].rb_mTransform);
	}
	MatrixCopy(_aRenBones[0].rb_mStrTransform, _aRenBones[0].rb_mTransform);

	extern INDEX ska_bAllowBonesAdjustment;
	// if callback function was specified
	if( ska_bAllowBonesAdjustment && _pAdjustBonesCallback!=NULL) 
	{
		// Call callback function
		_pAdjustBonesCallback(_pAdjustBonesData);
	}
	
	Matrix12 mStretch;
	const INDEX ctRenBones = _aRenBones.Count();
	_sfStats.IncrementCounter( CStatForm::SCI_BONES, ctRenBones-1);

	// for each renbone after first dummy one
	INDEX	irb;
	for( irb = 1; irb < ctRenBones; irb++ )
	{
		Matrix12 mRelPlacement;
		Matrix12 mOffset;
		RenBone &rb = _aRenBones[irb];
		const RenBone &rbParent = _aRenBones[rb.rb_iParentIndex];
		/*
		RenBone *prbNext = &_aRenBones[irb+1];
		const RenBone *prbParentNext = &_aRenBones[prbNext->rb_iParentIndex];
		__asm 
		{
		mov   esi,D [prbNext]
		mov   edi,D [prbParentNext]
		prefetchnta [esi]
		prefetchnta [edi]
		}
		*/
		// Convert QVect of placement to matrix12
		QVect qv;
		qv.vPos = rb.rb_apPos.ap_vPos;
		qv.qRot = rb.rb_arRot.ar_qRot;
		QVectToMatrix12(mRelPlacement,qv);
		
		// if this is root bone
		if(rb.rb_psbBone->sb_iParentID == (-1)) 
		{
			// stretch root bone
			RenModel &rmParent = _aRenModels[rb.rb_iRenModelIndex];
			MakeStretchMatrix(mStretch, rmParent.rm_pmiModel->mi_vStretch);
			QVectToMatrix12(mOffset,rmParent.rm_pmiModel->mi_qvOffset);
			// add offset to root bone
			MatrixMultiplyCP(mRelPlacement,mOffset,mRelPlacement);
			
			Matrix12 mStrParentBoneTrans;
			// Create stretch matrix with parent bone transformations
			MatrixMultiplyCP(mStrParentBoneTrans, rbParent.rb_mStrTransform,mStretch);
			// transform bone using stretch parent's transform, relative placement
			MatrixMultiply(rb.rb_mTransform,rbParent.rb_mTransform, mRelPlacement);
			MatrixMultiply(rb.rb_mStrTransform, mStrParentBoneTrans, mRelPlacement);
		}
		else 
		{
			// transform bone using parent's transform and relative placement
			MatrixMultiply(rb.rb_mTransform,rbParent.rb_mTransform,mRelPlacement);
			MatrixMultiply(rb.rb_mStrTransform, rbParent.rb_mStrTransform, mRelPlacement);
		}
		// remember tranform matrix of bone placement for bone rendering
		MatrixCopy(rb.rb_mBonePlacement,rb.rb_mStrTransform);
	}
	
	// for each renmodel after first dummy one
	const INDEX ctrm = _aRenModels.Count();
	for(INDEX irm=1;irm<ctrm;irm++) 
	{
		// remember transforms for bone-less models for every renmodel, except the dummy one
		Matrix12 mOffset;
		Matrix12 mStretch;
		RenModel &rm = _aRenModels[irm];
		
		QVectToMatrix12(mOffset,rm.rm_pmiModel->mi_qvOffset);
		MakeStretchMatrix(mStretch,rm.rm_pmiModel->mi_vStretch);
		
		MatrixMultiply(rm.rm_mTransform,_aRenBones[rm.rm_iParentBoneIndex].rb_mTransform,mOffset);
		MatrixMultiply(rm.rm_mStrTransform,_aRenBones[rm.rm_iParentBoneIndex].rb_mStrTransform,mOffset);
		MatrixMultiplyCP(rm.rm_mStrTransform,rm.rm_mStrTransform,mStretch);
	}
	
	Matrix12 mInvert;
	// for each renbone
	const INDEX ctrb = _aRenBones.Count();
	for( irb = 1; irb < ctrb; irb++) 
	{
		RenBone &rb = _aRenBones[irb];
		// multiply every transform with invert matrix of bone abs placement
		MatrixTranspose(mInvert,rb.rb_psbBone->sb_mAbsPlacement);
		// create two versions of transform matrices, stretch and normal for vertices and normals
		MatrixMultiplyCP(rb.rb_mTransform,rb.rb_mTransform,mInvert);
		MatrixMultiplyCP(rb.rb_mStrTransform,rb.rb_mStrTransform,mInvert);
	}
//안태훈 수정 시작	//(Add & Modify SSSE Effect)(0.1)
	if(!bRenderToScreen) return;

	//ska tag들을 bone과 연결한다.
	Matrix12 mObjToAbsStr;
	for(INDEX iRm=0; iRm < _aRenMeshes.Count(); ++iRm)
	{
		MeshLOD &mlod = _aRenMeshes[iRm].rmsh_pMeshInst->mi_pMesh->msh_aMeshLODs[_aRenMeshes[iRm].rmsh_iMeshLODIndex];
		RenModel &rm = _aRenModels[ _aRenMeshes[iRm].rmsh_iRenModelIndex ];
		CModelInstance &mi = *rm.rm_pmiModel;

		if(mi.m_tmSkaTagManager.Empty()) continue;

		const Matrix12 &mObjToAbs = _mObjToAbs;

		//root를 갱신한다.
		if(iRm == 0)
		{
			CSkaTag *pRootTag = (CSkaTag *)mi.m_tmSkaTagManager.Find("__ROOT");
			MakeStretchMatrix(mStretch, mi.mi_vStretch);
			MatrixMultiply(mObjToAbsStr, mObjToAbs, mStretch);
			if(pRootTag != NULL)
			{
				pRootTag->AddNewTagInfo(_pTimer->GetLerpedCurrentTick(), mObjToAbs, mObjToAbsStr);
			}
			CSkaTag *pCenterTag = (CSkaTag *)mi.m_tmSkaTagManager.Find("CENTER");
			if(pCenterTag != NULL && pCenterTag->GetBoneCount() == 0)
			{
				pCenterTag->AddNewTagInfo(_pTimer->GetLerpedCurrentTick(), mObjToAbs, mObjToAbsStr);
			}
			CSkaTag *pTopTag = (CSkaTag *)mi.m_tmSkaTagManager.Find("__TOP");
			if(pTopTag != NULL)
			{
				pTopTag->AddNewTagInfo(_pTimer->GetLerpedCurrentTick(), mObjToAbs, mObjToAbsStr);
			}
			//HITPOINT를 갱신한다.
			for(INDEX i=0; i<31; ++i)
			{
				if( !(mi.m_tmSkaTagManager.GetTempValue() & (0x00000001 << i)) ) continue;
				CTString strTemp;
				strTemp.PrintF("TEMP%x", 0x00000001 << i);
				CSkaTag *pHitTag = (CSkaTag *)mi.m_tmSkaTagManager.Find(strTemp.str_String);
				if(pHitTag != NULL && pHitTag->GetMeshLOD() != NULL && pHitTag->GetMeshLOD() == &mlod)
				{
					SetSkaTagInfo(pHitTag, ctrb, rm, mlod);
				}
			}
		}

		for(int iTag=0; iTag<mlod.mlod_aSkaTags.Count(); ++iTag)
		{
			//주의(안태훈) 역행 캐스팅이 있음. Type을 이럴때 쓰려고 만든거지만 참 -_-한 코드임.
			CTag *pNativeTag = mi.m_tmSkaTagManager.Find( mlod.mlod_aSkaTags[iTag]->GetName() );
			if(pNativeTag == NULL) continue;
			ASSERT(pNativeTag->GetType() == TT_SKA);
			CRefCountPtr<CSkaTag> ptrTag( (CSkaTag*)pNativeTag );
			if(!ptrTag->Active()) continue;

			SetSkaTagInfo(ptrTag.GetNative(), ctrb, rm, mlod);
		}
	}
//안태훈 수정 끝	//(Add & Modify SSSE Effect)(0.1)
}


// Match animations in anim queue for bones
static void MatchAnims(RenModel &rm, BOOL bRenderToScreen)
{
	const FLOAT fLerpedTick = _pTimer->GetLerpedCurrentTick();
	
	// return if no animsets
	const INDEX ctas = rm.rm_pmiModel->mi_aAnimSet.Count();
	if(ctas == 0) return;
	// count animlists
	const INDEX ctal = rm.rm_pmiModel->mi_aqAnims.aq_Lists.Count();
	// find newes animlist that has fully faded in
	INDEX iFirstAnimList = 0;
	// loop from newer to older
	INDEX	ial;
	for( ial = ctal-1; ial >= 0; ial-- )
	{
		AnimList &al = rm.rm_pmiModel->mi_aqAnims.aq_Lists[ial];
		// calculate fade factor
		const FLOAT fFadeFactor = CalculateFadeFactor(al);
		if(fFadeFactor >= 1.0f) 
		{
			iFirstAnimList = ial;
			break;
		}
	}

	// for each anim list after iFirstAnimList
	for( ial = iFirstAnimList; ial < ctal; ial++)
	{
//안태훈 수정 시작	//(For Performance)(0.2)
		AnimList &al = rm.rm_pmiModel->mi_aqAnims.aq_Lists[ial];
//안태훈 수정 끝	//(For Performance)(0.2)
		AnimList const *palListNext=NULL;
		if(ial+1<ctal) 
		{
			palListNext = &rm.rm_pmiModel->mi_aqAnims.aq_Lists[ial+1];
		}
		
		// calculate fade factor
		const FLOAT fFadeFactor = CalculateFadeFactor(al);
		
		// for each played anim in played anim list
		const INDEX ctpa = al.al_PlayedAnims.Count();
		for(INDEX ipa=0;ipa<ctpa;ipa++) 
		{
//안태훈 수정 시작	//(For Performance)(0.2)
			PlayedAnim &pa = al.al_PlayedAnims[ipa];
//안태훈 수정 끝	//(For Performance)(0.2)
			const BOOL bAnimLooping = pa.pa_ulFlags&AN_LOOPING;
			FLOAT fTime = fLerpedTick;
			
			INDEX iAnimSetIndex;
			INDEX iAnimIndex;
			FLOAT fAnimStrength = pa.pa_fStrength;
			// find anim by ID in all anim sets within this model
			if(rm.rm_pmiModel->FindAnimationByID(pa.pa_iAnimID,&iAnimSetIndex,&iAnimIndex)) 
			{
				// if found, animate bones
				const Animation &an = rm.rm_pmiModel->mi_aAnimSet[iAnimSetIndex].as_Anims[iAnimIndex];
				
				// calculate end time for this animation list
				const FLOAT fFadeInEndTime = al.al_fStartTime + al.al_fFadeTime;
				
				// if animation freeze is on and if there is a newer animation list
				if(pa.pa_ulFlags&AN_FREEZE_ON_BLEND && palListNext!=NULL) 
				{
					// freeze time of this one to never overlap with the newer list
					fTime = ClampUp(fTime, palListNext->al_fStartTime);
				}
				
				// calculate time passed since the animation started
				FLOAT fTimeOffset = fTime - pa.pa_fStartTime;
				fTimeOffset = ClampDn(fTimeOffset,0.0f);

				// if animation freeze is on and if this animation list is fading in
				if(pa.pa_ulFlags&AN_FREEZE_ON_BLEND && fLerpedTick<fFadeInEndTime) 
				{
					// offset the time so that it is paused at the end of fadein interval
					fTimeOffset += fFadeInEndTime - fLerpedTick;
				}
				
//안태훈 수정 시작	//(Effect Add & Modify for Close Beta)(0.1)
				FLOAT fAnimLength = an.an_iFrames * an.an_fSecPerFrame * pa.pa_fSpeedMul;
				INDEX iAnimLoopCount = INDEX(fTimeOffset / fAnimLength);
				FLOAT fOffsetFromAnimLoop = fTimeOffset - iAnimLoopCount * fAnimLength;
				BOOL bLoopAnimEffect = FALSE;
				CAnimEffect *pAnimEffect = rm.rm_pmiModel->GetAnimEffectByAnimIndex(pa.pa_iAnimID);
				if(pAnimEffect != NULL)// && ipa == (ctpa-1) && ial == (ctal-1))
				{
					if(pAnimEffect->GetLoopCount() < iAnimLoopCount)
					{
						bLoopAnimEffect = TRUE;
						pAnimEffect->SetLoopCount(iAnimLoopCount);
					}
				}
				//if(CEntity::GetPlayerEntity(0)->GetModelInstance() == rm.rm_pmiModel)
				//if(bLoopAnimEffect)
					//CPrintF("---%6.2f---AnimLoopCount : Offset&Len(%6.2f, %6.2f),Ani(%s),Cnt(%d),Loop(%d),%6.2f,%6.2f,%6.2f\n"
						//, _pTimer->GetLerpedCurrentTick()
						//, fTimeOffset, fAnimLength
						//, ska_IDToString(pa.pa_iAnimID)
						//, iAnimLoopCount, bLoopAnimEffect
						//, pa.pa_fStartTime, fTime, _pTimer->CurrentTick());
//안태훈 수정 끝	//(Effect Add & Modify for Close Beta)(0.1)
				
				// if fade out flag is set 
				if(pa.pa_ulFlags&AN_FADEOUT) 
				{
					// calculate current animation strenght
					const FLOAT fAnimEndLength = fTime - (pa.pa_fStartTime + (an.an_iFrames * an.an_fSecPerFrame * pa.pa_fSpeedMul));
					fAnimStrength = Clamp(pa.pa_fStrength-(fAnimEndLength/al.al_fFadeTime),0.0f,1.0f);
				}
				
				FLOAT f = fTimeOffset / (an.an_fSecPerFrame*pa.pa_fSpeedMul);
				INDEX iAnimFrame,iNextAnimFrame;
				
				// if animation looping flag is set
				if(bAnimLooping) 
				{
					// calculate current and next frame (wraped if anim has reached end)
					f = fmod(f,an.an_iFrames);
					const INDEX iCurentFrame = INDEX(f);
					const INDEX ctAnimFrames = an.an_iFrames;
					
//안태훈 수정 시작	//(Effect Add & Modify for Close Beta)(0.1)
					if(bLoopAnimEffect)
					{
						rm.rm_pmiModel->LoopAnimEffect(pa.pa_iAnimID, al.al_fFadeTime, fOffsetFromAnimLoop, pa.pa_fSpeedMul);
						//CPrintF("---%6.2f---LoopAnimEffect : ani( %s )\n", _pTimer->GetLerpedCurrentTick(), ska_IDToString(pa.pa_iAnimID));
					}
					// iAnimFrame = iCurentFrame % an.an_iFrames;
					iAnimFrame = iCurentFrame;
					if(iAnimFrame>=ctAnimFrames)
					{
						iAnimFrame -= ctAnimFrames;
					}
					
					// iNextAnimFrame = (iCurentFrame+1) % an.an_iFrames;
					iNextAnimFrame = (iCurentFrame+1);
					if(iNextAnimFrame>=ctAnimFrames)	//현재 ani위치는 한번 wrap된 상태임.
					{
						iNextAnimFrame -= ctAnimFrames;
					}
//안태훈 수정 끝	//(Effect Add & Modify for Close Beta)(0.1)
					
					ASSERT(iAnimFrame == (iCurentFrame%an.an_iFrames));
					ASSERT(iNextAnimFrame == ((iCurentFrame+1)%an.an_iFrames));
					
					ASSERT(iAnimFrame>=0 && iAnimFrame<an.an_iFrames);
					ASSERT(iNextAnimFrame>=0 && iNextAnimFrame<an.an_iFrames);
				}
				else 
				{
					// calculate current and next frame (last frame if anim has reached end)
					if(f>an.an_iFrames) 
					{
						f = an.an_iFrames-1;
						// if animation has flag to be removed on end
						if(pa.pa_ulFlags&AN_REMOVE_ON_END) 
						{
							// add internal fade out flag
							rm.rm_pmiModel->mi_aqAnims.aq_Lists[ial].al_PlayedAnims[ipa].pa_ulFlags|=AN_FADEOUT;
						}
					}
					const INDEX iCurentFrame = INDEX(f);
					iAnimFrame = ClampUp(iCurentFrame,an.an_iFrames-1L);
					iNextAnimFrame = ClampUp(iCurentFrame+1L,an.an_iFrames-1L);
					ASSERT(iAnimFrame>=0 && iAnimFrame<an.an_iFrames);
					ASSERT(iNextAnimFrame>=0 && iNextAnimFrame<an.an_iFrames);
				}
				// iAnimFrame = ClampDn(iAnimFrame,(INDEX)0); // by gogi
				
				// for each bone envelope
				const INDEX ctbe = an.an_abeBones.Count();
				for(INDEX ibe=0;ibe<ctbe;ibe++) 
				{
					// find its renbone in array of renbones
					INDEX iRenBoneIndex = FindRenBoneIndex(rm,an.an_abeBones[ibe].be_iBoneID);
					if(iRenBoneIndex!=(-1)) 
					{
						RenBone &rb = _aRenBones[iRenBoneIndex];
						const BoneEnvelope &be = an.an_abeBones[ibe];
						
						ASSERT(rb.rb_psbBone!=NULL);
						
						INDEX iNextRotFrameIndex;
						INDEX iRotFrameNum;
						INDEX iNextRotFrameNum;
						FLOAT fSlerpFactor;
						FLOATquat3D qRotCurrent;
						FLOATquat3D qRotNext;
						FLOATquat3D const *pqRotCurrent;
						FLOATquat3D const *pqRotNext;
						
						ASSERT(iAnimFrame>=0);
						// if animation is not compresed
						if(!an.an_bCompresed) 
						{
							const AnimRot *arFirst = &be.be_arRot[0];
							const INDEX ctfn = be.be_arRot.Count();
							// find index of closest frame
							const INDEX iRotFrameIndex = FindFrame((UBYTE*)arFirst, iAnimFrame, ctfn, sizeof(AnimRot), 
								be.be_uwLastFrameRot, be.be_fInvFrameStepRot);
							// get index of next frame
							if(bAnimLooping) 
							{
								iNextRotFrameIndex = (iRotFrameIndex+1);
								if(iNextRotFrameIndex>=ctfn) 
								{
									iNextRotFrameIndex -= ctfn;
								}
								ASSERT(iNextRotFrameIndex == ((iRotFrameIndex+1) % ctfn));
								// iNextRotFrameIndex = (iRotFrameIndex+1) % ctfn; // 68
							}
							else 
							{
								iNextRotFrameIndex = ClampUp(iRotFrameIndex+1L,ctfn - 1L);
							}
							
							iRotFrameNum = be.be_arRot[iRotFrameIndex].ar_iFrameNum;
							iNextRotFrameNum = be.be_arRot[iNextRotFrameIndex].ar_iFrameNum;
							pqRotCurrent = &be.be_arRot[iRotFrameIndex].ar_qRot;
							pqRotNext = &be.be_arRot[iNextRotFrameIndex].ar_qRot;
							// animation is compresed
						}
						else 
						{
							const AnimRotOpt *aroFirst = &be.be_arRotOpt[0];
							const INDEX ctfn = be.be_arRotOpt.Count();
							
							const INDEX iRotFrameIndex = FindFrame((UBYTE*)aroFirst, iAnimFrame, ctfn, sizeof(AnimRotOpt), 
								be.be_uwLastFrameRot, be.be_fInvFrameStepRot);
							// get index of next frame
							if(bAnimLooping) 
							{
								iNextRotFrameIndex = (iRotFrameIndex+1L) % ctfn;
							}
							else 
							{
								iNextRotFrameIndex = ClampUp(iRotFrameIndex+1L,ctfn - 1L);
							}
							
							const AnimRotOpt &aroRot = be.be_arRotOpt[iRotFrameIndex];
							const AnimRotOpt &aroRotNext = be.be_arRotOpt[iNextRotFrameIndex];
							iRotFrameNum = aroRot.aro_iFrameNum;
							iNextRotFrameNum = aroRotNext.aro_iFrameNum;
							FLOAT3D vAxis;
							ANGLE aAngle;
							
							// decompress angle
							aAngle = aroRot.aro_aAngle * (1.0f / ANG_COMPRESIONMUL);
							DecompressAxis(vAxis,aroRot.aro_ubH,aroRot.aro_ubP);
							qRotCurrent.FromAxisAngle(vAxis,aAngle);
							
							aAngle = aroRotNext.aro_aAngle * (1.0f / ANG_COMPRESIONMUL);
							DecompressAxis(vAxis,aroRotNext.aro_ubH,aroRotNext.aro_ubP);
							qRotNext.FromAxisAngle(vAxis,aAngle);
							pqRotCurrent = &qRotCurrent;
							pqRotNext = &qRotNext;
						}
						
						if(iNextRotFrameNum<=iRotFrameNum) 
						{
							// calculate slerp factor for rotations
							fSlerpFactor = (f-iRotFrameNum) / (an.an_iFrames-iRotFrameNum);
						}
						else 
						{
							// calculate slerp factor for rotations
							fSlerpFactor = (f-iRotFrameNum) / (iNextRotFrameNum-iRotFrameNum);
						}
						
						// calculate rotation for bone beetwen current and next frame in animation
						const FLOATquat3D qRot = FastSlerp( fSlerpFactor, *pqRotCurrent, *pqRotNext);
						// and currently playing animation 
						if( fAnimStrength>0.999) rb.rb_arTempRot.ar_qRot = qRot;
						else rb.rb_arTempRot.ar_qRot = FastSlerp( fAnimStrength, rb.rb_arTempRot.ar_qRot, qRot);
						
						const AnimPos *apFirst = &be.be_apPos[0];
						const INDEX ctfn = be.be_apPos.Count();
						const INDEX iPosFrameIndex = FindFrame((UBYTE*)apFirst, iAnimFrame, ctfn, sizeof(AnimPos), 
							be.be_uwLastFramePos, be.be_fInvFrameStepPos);
						
						INDEX iNextPosFrameIndex;
						// is animation looping
						if(bAnimLooping) 
						{
							iNextPosFrameIndex = (iPosFrameIndex+1);
							if(iNextPosFrameIndex>=ctfn) 
							{
								iNextPosFrameIndex -= ctfn;
							}
							ASSERT(iNextPosFrameIndex == ((iPosFrameIndex+1) % ctfn));
							// iNextRotFrameIndex = (iRotFrameIndex+1) % ctfn; // 68
							// iNextPosFrameIndex = (iPosFrameIndex+1) % ctfn; // 59
							// CPrintF("Notification: Position %d %d\n", iPosFrameIndex,iNextPosFrameIndex);
						}
						else 
						{
							iNextPosFrameIndex = ClampUp(iPosFrameIndex+1L,ctfn-1L);
						}
						
						const INDEX iPosFrameNum = be.be_apPos[iPosFrameIndex].ap_iFrameNum;
						const INDEX iNextPosFrameNum = be.be_apPos[iNextPosFrameIndex].ap_iFrameNum;
						
						FLOAT fLerpFactor;
						if(iNextPosFrameNum<=iPosFrameNum) fLerpFactor = (f-iPosFrameNum) / (an.an_iFrames-iPosFrameNum);
						else fLerpFactor = (f-iPosFrameNum) / (iNextPosFrameNum-iPosFrameNum);
						
						FLOAT3D vBonePosCurrent = be.be_apPos[iPosFrameIndex].ap_vPos;
						FLOAT3D vBonePosNext = be.be_apPos[iNextPosFrameIndex].ap_vPos;
						
						// if bone lengths are not same
						const FLOAT fBoneDiff = be.be_OffSetLen-rb.rb_fOffsetLen;
						if((fBoneDiff<-0.001f) || (fBoneDiff>=0.001f)) 
						{ // Abs(fBoneDiff)>0.001f
							// adjust bone spoistion so animations match
							if(rb.rb_fOffsetLen!=be.be_OffSetLen && be.be_OffSetLen!=0.0f) 
							{
								const FLOAT fLenDiff = rb.rb_fOffsetLen / be.be_OffSetLen;
								vBonePosCurrent *= (fLenDiff);
								vBonePosNext *= (fLenDiff);
							}
						}
						
						// calculate position for bone beetwen current and next frame in animation
						const FLOAT3D vPos = Lerp(vBonePosCurrent,vBonePosNext,fLerpFactor);
						// Mix all animations in temporay position and lerp with original position
						rb.rb_apTempPos.ap_vPos = Lerp(rb.rb_apTempPos.ap_vPos,vPos,fAnimStrength);
					}
				}
				
				// for each morphmap
				for(INDEX im=0;im<an.an_ameMorphs.Count();im++) 
				{
					INDEX iMorphIndex;
					// find it in renmorph
					if(FindRenMorphIndex(rm,an.an_ameMorphs[im].me_iMorphMapID,&iMorphIndex)) 
					{
						// lerp morphs
						const FLOAT fCurFactor = an.an_ameMorphs[im].me_aFactors[iAnimFrame];
						const FLOAT fLastFactor = an.an_ameMorphs[im].me_aFactors[iNextAnimFrame];
						const FLOAT fFactor = Lerp(fCurFactor,fLastFactor,f-iAnimFrame);
						
						_aRenMorphs[iMorphIndex].rmp_fTempFactor = Lerp(_aRenMorphs[iMorphIndex].rmp_fTempFactor,
							fFactor,
							fAnimStrength);
					}
				}
			}
		}
		
		// if not fading in 
		if(fFadeFactor>0.99999f) 
		{
			// for each ren bone in this renmodel
			for(INDEX irb=0;irb<rm.rm_ctBones;irb++) 
			{
				ASSERT(_aRenBones.Count() > irb + rm.rm_iFirstBone);
				if(_aRenBones.Count() <= irb + rm.rm_iFirstBone) return;
				RenBone &rb = _aRenBones[irb + rm.rm_iFirstBone];
				rb.rb_arRot.ar_qRot = rb.rb_arTempRot.ar_qRot;
				rb.rb_apPos.ap_vPos = rb.rb_apTempPos.ap_vPos;
				// Reset temp values
				ASSERT(rb.rb_psbBone!=NULL);
				rb.rb_arTempRot.ar_qRot = rb.rb_psbBone->sb_qvRelPlacement.qRot;
				rb.rb_apTempPos.ap_vPos = rb.rb_psbBone->sb_qvRelPlacement.vPos;
			}
			// else if fading in
		}
		else 
		{
			// for each ren bone in this renmodel
			for(INDEX irb=0;irb<rm.rm_ctBones;irb++) 
			{
				ASSERT(_aRenBones.Count() > irb + rm.rm_iFirstBone);
				if(_aRenBones.Count() <= irb + rm.rm_iFirstBone) return;
				RenBone &rb = _aRenBones[irb + rm.rm_iFirstBone];
				// blend rotations and position for this anim list
				rb.rb_arRot.ar_qRot = Slerp(fFadeFactor, rb.rb_arRot.ar_qRot, rb.rb_arTempRot.ar_qRot);
				rb.rb_apPos.ap_vPos = Lerp(rb.rb_apPos.ap_vPos,rb.rb_apTempPos.ap_vPos,fFadeFactor);
				// Reset temp values
				ASSERT(rb.rb_psbBone!=NULL);
				rb.rb_arTempRot.ar_qRot = rb.rb_psbBone->sb_qvRelPlacement.qRot;
				rb.rb_apTempPos.ap_vPos = rb.rb_psbBone->sb_qvRelPlacement.vPos;
			}
		}
		
		// for each renmesh in this renmodel
		const INDEX ctrmsh = rm.rm_ctMeshes;
		for(INDEX irmsh=0;irmsh<ctrmsh;irmsh++) 
		{
			RenMesh &rmsh = _aRenMeshes[irmsh + rm.rm_iFirstMesh];
			// for each renmorph in renmesh
			for(INDEX irmh=0;irmh<rmsh.rmsh_ctMorphs;irmh++) 
			{
				RenMorph &rmh = _aRenMorphs[irmh + rmsh.rmsh_iFirstMorph];
				// blend morphs for this anim list
				rmh.rmp_fFactor = Lerp(rmh.rmp_fFactor,rmh.rmp_fTempFactor, fFadeFactor);
			}
		}
	}
}



// array of pointers to texure data for shader
static CStaticStackArray<class CTextureObject*> _patoTextures;
// array of pointers to uvmaps for shader
static CStaticStackArray<struct GFXTexCoord*> _paTexCoords;
// array of texcoord buffer pointers for shader
static CStaticStackArray<INDEX> _paiTexCoordBufferIDs;

extern CNiceWater _WaterInformation;

// draw mesh on screen
static void RenderMesh( RenMesh &rmsh, RenModel &rm, ULONG ulShaderRenFlags)
{
	// if in software
	if(!_bUseHardwareShaders) 
	{
		ASSERT(_bMeshBuffersLocked); // buffers must be locked
	}
	
	// render only if enabled
	extern INDEX ska_bRenderMesh;  // for profiling
	if( !ska_bRenderMesh) return;
	
	BOOL bInvertedProjection = FALSE;
	if((CProjection3D *)_aprProjection!=NULL) 
	{
		bInvertedProjection = _aprProjection->pr_bInverted != rm.rm_bInverted;
	}
	
	// set view matrix for this mesh
	SetViewMatrix( rmsh.rmsh_mStrObjToView);
	
	// if detailed statistics is displayed
	extern INDEX _iStatsMode;
	if( _iStatsMode>1) 
	{ 
		// count the triangles in 1st mip
		MeshLOD &mlod0 = rmsh.rmsh_pMeshInst->mi_pMesh->msh_aMeshLODs[0];
		INDEX ctsrf0 = mlod0.mlod_aSurfaces.Count();
		for( INDEX isrf=0; isrf<ctsrf0; isrf++) 
		{
			MeshSurface &msrf = mlod0.mlod_aSurfaces[isrf];
			_sfStats.IncrementCounter( CStatForm::SCI_SKATRIANGLES_FIRSTMIP, msrf.msrf_auwTriangles.Count());
		}
	}
	// profile rendering
	const BOOL bModelSetupTimer = _sfStats.CheckTimer(CStatForm::STI_MODELSETUP);
	if( bModelSetupTimer) _sfStats.StopTimer(CStatForm::STI_MODELSETUP);
	_sfStats.StartTimer(CStatForm::STI_MODELRENDERING);
	
	// get current lod and surfaces count
	MeshLOD &mlod  = rmsh.rmsh_pMeshInst->mi_pMesh->msh_aMeshLODs[rmsh.rmsh_iMeshLODIndex];
	
	INDEX ctmsrf = mlod.mlod_aSurfaces.Count();
	
	// for each surface
	for(INDEX imsrf=0; imsrf<ctmsrf; imsrf++) 
	{
		
		// count in the triangles
		MeshSurface &msrf = mlod.mlod_aSurfaces[imsrf];
		_sfStats.IncrementCounter( CStatForm::SCI_SKATRIANGLES_USEDMIP, msrf.msrf_auwTriangles.Count());

		extern INDEX gfx_bRenderReflection;
		if(gfx_bRenderReflection)
		{
			_sfStats.IncrementCounter( CStatForm::SCI_REFLECTION_SKATRI, msrf.msrf_auwTriangles.Count());
		}
		if(!_bLoadedShadowShader)
		{
			if(_pshShadowShader == NULL) _pshShadowShader = new CShader;
			_pshShadowShader->Load_t("Shaders\\Shadow.sha");
			if(_pshNoShadowShader == NULL) _pshNoShadowShader = new CShader;
			_pshNoShadowShader->Load_t("Shaders\\NoShadow.sha");
			if(_pshNiceWaterShader == NULL) _pshNiceWaterShader = new CShader;
			_pshNiceWaterShader->Load_t("Shaders\\WaterShader.sha");
			_bLoadedShadowShader = TRUE;
		}
		
		CShader *pShader = msrf.msrf_pShader;
		if( _iRenderingType==2) 
		{
			pShader = &_shMaskShader; // force mask shader for rendering to shadowmaps
		}

//강동민 수정 시작.
		// FIXME : 어느 순간부터 하드웨어 쉐이더가 지원이 안되는 곳에서 그림자가 안나오기 시작함.
		// FIXME : 원인이 파악될때까지는 하드웨어 쉐이더가 아닌 경우에는 자동 조절되도록 처리함.
		else if(RM_GetFlags() & RMF_SHOWSHADOW)
		{
			if(_bUseHardwareShaders)
			{
				pShader = _pshShadowShader;
				
				// 텍스쳐가 없는 모델의 Shader...
				// NOTE : 아랫부분이 중복되는 곳이 있으니, 알아서 처리할것...ㅡ.ㅡ
				ShaderParams *pShaderParams = &msrf.msrf_ShadingParams;
				INDEX cttx = pShaderParams->sp_aiTextureIDs.Count();
				if(cttx <= 0)
				{
					pShader = _pshNoShadowShader;
				}
			}
			// 하드웨어 쉐이더 지원 안되는 경우.
			else
			{
				extern INDEX g_iShadowDetail;
				if(g_iShadowDetail != 0 && g_iShadowDetail % 2 == 0)
				{
					g_iShadowDetail = g_iShadowDetail - 1;
				}
			}
		}
		else if((RM_GetFlags() & RMF_SHOWNICEWATER) && _bUseHardwareShaders)
		{
			// NOTE : Reflection 텍스쳐와 물로 사용할 텍스쳐 두개를 설정해줘야함.
			// NOTE : 추후에 Refraction 텍스쳐도 추가될 예정.
			pShader = _pshNiceWaterShader;
		}
//강동민 수정 끝
		
		const BOOL bDynamicSurface = msrf.msrf_ulFlags&MS_DYNAMIC_SURFACE;
		MeshVertex *pavFinalVertices;
		MeshNormal *panFinalNormals;
//안태훈 수정 시작	//(Add Tagent-space Normal Map)(0.2)
		MeshTangent *panFinalTangents;
//안태훈 수정 끝	//(Add Tagent-space Normal Map)(0.2)
		INDEX iFinalVertexBufferID;
		INDEX iFinalNormalBufferID;
//안태훈 수정 시작	//(Add Tagent-space Normal Map)(0.1)
		INDEX iFinalTangentBufferID;
//안태훈 수정 끝	//(Add Tagent-space Normal Map)(0.1)
		
		//이쪽은 morph관련 코드인듯
		// if this is dynamic surface and hardware shaders are used
		if(bDynamicSurface && _bMeshUsesMorphs) 
		{
			// use pointer to morphed vertices
			if(_bUseHardwareShaders) 
			{
				ASSERT(_iMorphedVertexBufferID!=(-1) && _iMorphedNormalBufferID!=(-1));
			}
			else 
			{
				ASSERT(_pavMorphedVertices!=NULL && _panMorphedNormals!=NULL);
			}
			pavFinalVertices = _pavMorphedVertices;
			panFinalNormals = _panMorphedNormals;
			iFinalVertexBufferID = _iMorphedVertexBufferID;
			iFinalNormalBufferID = _iMorphedNormalBufferID;
			// if software shaders is used all morphed and weighted vertices are in _pavWeightedVertices
		}
		else 
		{
			// use pointer to final vertices
			if(_bUseHardwareShaders) 
			{
				ASSERT(mlod.mlod_iVertexBufferID!=(-1) && mlod.mlod_iNormalBufferID!=(-1));
			}
			else 
			{
				ASSERT(_pavWeightedVertices!=NULL && _panWeightedNormals!=NULL);
			}
			pavFinalVertices = _pavWeightedVertices;
			panFinalNormals = _panWeightedNormals;
//안태훈 수정 시작	//(Add Tagent-space Normal Map)(0.2)
			panFinalTangents = _panWeightedTangents;
//안태훈 수정 끝	//(Add Tagent-space Normal Map)(0.2)
			iFinalVertexBufferID = mlod.mlod_iVertexBufferID;
			iFinalNormalBufferID = mlod.mlod_iNormalBufferID;
//안태훈 수정 시작	//(Add Tagent-space Normal Map)(0.1)
			iFinalTangentBufferID = mlod.mlod_iTangentBufferID;
//안태훈 수정 끝	//(Add Tagent-space Normal Map)(0.1)
		}
		
		// if this surface has valid shader and show texure flag is set
		if( pShader!=NULL && (RM_GetFlags() & RMF_SHOWTEXTURE)) 
		{
			// create copy of shading params
			ShaderParams *pShaderParams = &msrf.msrf_ShadingParams;
			// ShaderParams spForAdjustment;
			
#pragma message(">> Uncomment & optimize callback function params transfer")
			/* if callback function was specified
			if(_pAdjustShaderParams!=NULL) 
			{
			// Call callback function
			spForAdjustment = msrf.msrf_ShadingParams;
			_pAdjustShaderParams( _pAdjustShaderData, msrf.msrf_iSurfaceID, pShader, spForAdjustment);
			pShaderParams = &spForAdjustment;
			}
			*/
			
			// clamp surface texture count to max number of textrues in mesh
			INDEX cttx = pShaderParams->sp_aiTextureIDs.Count();
			INDEX cttxMax = rmsh.rmsh_pMeshInst->mi_tiTextures.Count();
			// cttx = ClampUp(cttx,cttxMax);
			
			_patoTextures.PopAll();
			extern CRenderTexture*	re_prtReflection;		// Reflection으로 사용할 텍스쳐.
			if((RM_GetFlags() & RMF_SHOWNICEWATER) && re_prtReflection && _bUseHardwareShaders)
			{
				_patoTextures.Push(3);			// 텍스쳐 3개.
				CTextureObject		toReflect;
				toReflect.SetData(&re_prtReflection->rt_tdTexture);				

				if(_WaterInformation.m_toBump.ao_AnimData)
				{
					_patoTextures[0] = &_WaterInformation.m_toBump;		// Bump	Texture
				}
//강동민 수정 시작 Water 구현		04.21
				//_patoTextures[1] = &_pWaterInformation->m_texBump2;		// Bump	Texture(미사용)
//강동민 수정 끝 Water 구현			04.21
				_patoTextures[1] = &toReflect;								// Reflection Texture
//강동민 수정 시작 Water 구현		04.23
				if(_WaterInformation.m_toWater.ao_AnimData)
				{
					_patoTextures[2] = &_WaterInformation.m_toWater;		// Water Texture
				}
//강동민 수정 끝 Water 구현			04.23
			}
			else
			{
//강동민 수정 끝 Water 구현			04.20
				if( cttx>0) _patoTextures.Push(cttx);
				// for each texture ID
				for( INDEX itx=0;itx<cttx;itx++) 
				{
					// find texture in mesh and get pointer to texture by texture ID
					FindTextureData( &_patoTextures[itx], pShaderParams->sp_aiTextureIDs[itx], *rmsh.rmsh_pMeshInst);
				}
//강동민 수정 시작		03.16
			}
//강동민 수정 끝		03.16			
			
			// if using hardware shaders
			if(_bUseHardwareShaders) 
			{
				// prepare array of texcoord buffer IDs for shader
				const INDEX ctmuvm = pShaderParams->sp_aiTexCoordsIndex.Count();
				_paiTexCoordBufferIDs.PopAll();
				if(ctmuvm>0) _paiTexCoordBufferIDs.Push(ctmuvm);
				// for each uvmap
				for(INDEX imuvm=0;imuvm<ctmuvm;imuvm++) 
				{
					// Get UVMap index
					INDEX imuvmIndex = pShaderParams->sp_aiTexCoordsIndex[imuvm];
					// if mesh lod has this uvmap
					if(imuvmIndex<mlod.mlod_aUVMaps.Count()) 
					{
						_paiTexCoordBufferIDs[imuvm] = mlod.mlod_aUVMaps[imuvmIndex].muvm_iTexCoordBufferID;
					}
					else 
					{
						_paiTexCoordBufferIDs[imuvm] = -1;
					}
				}
				
				// if using software shaders
			}
			else 
			{
				// if not prepare array of texcoords for shaders
				INDEX ctuvm = pShaderParams->sp_aiTexCoordsIndex.Count();
				_paTexCoords.PopAll();
				if(ctuvm>0) _paTexCoords.Push(ctuvm);
				// for each uvmap
				for( INDEX iuvm=0; iuvm<ctuvm; iuvm++) 
				{
					// set pointer of uvmap in array of uvmaps for shader
					INDEX iuvmIndex = pShaderParams->sp_aiTexCoordsIndex[iuvm];
					// if mesh lod has this uv map
					if(iuvmIndex<mlod.mlod_aUVMaps.Count()) 
					{
						// _paTexCoords[iuvm] = (GFXTexCoord*)&mlod.mlod_aUVMaps[iuvmIndex].muv_aTexCoords[msrf.msrf_iFirstVertex];
						GFXTexCoord *ptcTexCoords = (GFXTexCoord*)_patcMeshTexCoords[iuvmIndex];
						_paTexCoords[iuvm] = &ptcTexCoords[msrf.msrf_iFirstVertex];
						
					}
					else 
					{
						_paTexCoords[iuvm] = NULL;
					}
				}
			}
			const INDEX ctTextures = _patoTextures.Count();
			const INDEX ctColors = pShaderParams->sp_acolColors.Count();
			const INDEX ctFloats = pShaderParams->sp_afFloats.Count();
			
//안태훈 수정 시작	//(Add Tagent-space Normal Map)(0.1)
//안태훈 수정 시작	//(For Performance)(0.1)
			ShaderDesc *pshDesc;
			pShader->GetShaderDesc(pshDesc);
			ShaderDesc &shDesc = *pshDesc;
//안태훈 수정 끝	//(For Performance)(0.1)
			BOOL bHasNormal = FALSE;
			BOOL bHasTangent = FALSE;
			if(shDesc.sd_ulStreamFlags.Count() > 0)
			{
				if(shDesc.sd_ulStreamFlags[0] & GFX_NORMAL_STREAM) bHasNormal = TRUE;
				if(shDesc.sd_ulStreamFlags[0] & GFX_TANGENT_STREAM) bHasTangent = TRUE;
			}
//안태훈 수정 끝	//(Add Tagent-space Normal Map)(0.1)
			
			// begin model rendering
			shaBegin( _aprProjection, pShader);
			
			const BOOL bBlendVertices = _bUseHardwareShaders && (rmsh.rmsh_ctWeights>0) && (rm.rm_ctBones>0);
			
			// setup drawing direction
			if(bInvertedProjection) 
			{
				shaEnableInverted();
			}
			else 
			{
				shaDisableInverted();
			}
			
			const INDEX iFirstVertex = msrf.msrf_iFirstVertex;
			const INDEX ctVertices = msrf.msrf_ctVertices;
			const INDEX ctTriangles = msrf.msrf_auwTriangles.Count();
			const INDEX ctIndices = ctTriangles*3;
			
			// setup hardware shaders
			if(_bUseHardwareShaders) 
			{
				// Enable hardware shaders
				shaEnableVertexBuffers();
				shaEnableVertexProgram();
				shaEnablePixelProgram();
				// Set vertex buffer ID
				shaSetVertexBufferID(iFinalVertexBufferID, iFirstVertex, ctVertices);
				// Set normal buffer ID
//안태훈 수정 시작	//(Add Tagent-space Normal Map)(0.1)
				if(bHasNormal) shaSetNormalBufferID(iFinalNormalBufferID);
				if(bHasTangent) shaSetTangentBufferID(iFinalTangentBufferID);
//안태훈 수정 끝	//(Add Tagent-space Normal Map)(0.1)
				// Set array of texcoord buffer ID
				const INDEX ctTexCoords = _paiTexCoordBufferIDs.Count();
				if(ctTexCoords>0) shaSetSurfaceUVMapsIDs(&_paiTexCoordBufferIDs[0],ctTexCoords);
				// use software rendering
			}
			else 
			{
				// Set vertex array 
				shaSetVertexArray((GFXVertex*)&pavFinalVertices[iFirstVertex], 0, ctVertices);
//안태훈 수정 시작	//(Add Tagent-space Normal Map)(0.1)
				// set normal array
				if(bHasNormal) shaSetNormalArray((GFXNormal*)&panFinalNormals[iFirstVertex]);
				//if(bHasTangent) shaSetTangentArray((GFXTangent*)&panFinalNormals[iFirstVertex]);
//안태훈 수정 끝	//(Add Tagent-space Normal Map)(0.1)
				// set array of uvmaps
				const INDEX ctTexCoords = _paTexCoords.Count();
				if(ctTexCoords>0) shaSetSurfaceUVMaps(&_paTexCoords[0],ctTexCoords);
			}
			
			shaSetIndices( &msrf.msrf_auwTriangles[0].mt_uwVertexIndex[0], ctIndices);
			shaSetSurfaceFlags( msrf.msrf_ShadingParams.sp_ulFlags);
			
			if(bBlendVertices) 
			{
				if(mlod.mlod_iWeightBufferID!=(-1)) 
				{
					// Set vertex weights
					shaSetWeightBufferID(mlod.mlod_iWeightBufferID);
					
					// Set number fo weights per one vertex
					const INDEX ctWeightsPerVertex = GetWeightsPerVertex(msrf,_fDistanceFactor);
					shaSetWeightsPerVertex(ctWeightsPerVertex);
					
					// Set bone matrices used by vertices in this surface
					SetSurfaceMatrices(rmsh, msrf, mlod);
				}
			}
			
			// lock arrays
			shaLockArrays();
			// set render flags (fog, haze, etc)
			shaSetRenFlags(ulShaderRenFlags);
			// set matrices
			shaSetObjToAbsMatrix(_mTransObjToAbs);
			shaSetObjToViewMatrix(rmsh.rmsh_mObjToView);
			shaSetObjToViewStrMatrix(rmsh.rmsh_mStrObjToView);

			// set light parametars and model color
			shaSetLightColor( _colAmbient,_colLight);
			shaSetLightDirection( _vLightDirInObj);
//안태훈 수정 시작	//(Add Sun Moon Entity and etc)(0.1)
			//버그 수정, RenderMesh의 colCurrent계산 부분에서 mi_colModelColor가 두번 곱해짐.
			//COLOR colCurrent = MulColors( _colModel, rm.rm_pmiModel->mi_colModelColor);//원본
			COLOR colCurrent = rm.rm_pmiModel->mi_colModelColor;
//안태훈 수정 끝	//(Add Sun Moon Entity and etc)(0.1)
			shaSetModelColor( colCurrent);

			// NOTE : Surface의 색상과 광원의 정보는 그림자를 렌더링할 때 필요없음.
			// NOTE : 나중에 제거할것.
			
			// set arrays
			if(RM_GetFlags() & RMF_SHOWSHADOW)
			{
				shaSetSurfaceColors(&_crShadowColor, 1);
			}
			else if(RM_GetFlags() & RMF_SHOWTEXTURE)
			{
				if(ctColors>0)
					shaSetSurfaceColors(&pShaderParams->sp_acolColors[0],ctColors);
			}
			extern CRenderTexture*	re_prtReflection;		// Reflection으로 사용할 텍스쳐.
			if((RM_GetFlags() & RMF_SHOWNICEWATER) && re_prtReflection && _bUseHardwareShaders)
			{					
				shaSetSurfaceTextures(&_patoTextures[0], 3);		// 원본.
			}
			else
			{			
				if(ctTextures>0) 
					shaSetSurfaceTextures(&_patoTextures[0],ctTextures);
			}
			if(ctFloats>0)   
				shaSetSurfaceFloats(&pShaderParams->sp_afFloats[0],ctFloats);			
			
			// actual rendering
			shaEnd();
			// Clean used values
			shaClean();
			// done
			shaUnlockArrays();
			
			// if vertex programs were used
			if(_bUseHardwareShaders) 
			{
				// disable them
				shaDisableVertexBuffers();
				shaDisableVertexProgram();
				shaDisablePixelProgram();
			}
		}
		// surface has no shader or textures are turned off
		else 
		{
			COLOR colErrColor = 0xCDCDCDFF;
			// surface has no shader, just show vertices using custom simple shader 
			shaSetVertexArray( (GFXVertex*)&_pavWeightedVertices[msrf.msrf_iFirstVertex], 0, msrf.msrf_ctVertices);
			shaSetNormalArray( (GFXNormal*)&_panWeightedNormals[msrf.msrf_iFirstVertex]);
			shaSetIndices( &msrf.msrf_auwTriangles[0].mt_uwVertexIndex[0], msrf.msrf_auwTriangles.Count()*3);
			shaSetTexture(-1);
			shaSetSurfaceColors( &colErrColor, 1);
			
			shaSetLightColor(_colAmbient,_colLight);
			shaSetLightDirection(_vLightDirInObj);
			shaSetModelColor( _colModel);
			
			shaDisableBlend();
			shaEnableDepthTest();
			shaEnableDepthWrite();
			shaSetColor(0);
			shaCalculateLight();
			
			// actual rendering
			shaRender();
			// done
			shaClean();
		}
	}
	
	// back to profile setup
	_sfStats.StopTimer(CStatForm::STI_MODELRENDERING);
	if( bModelSetupTimer) _sfStats.StartTimer(CStatForm::STI_MODELSETUP);
}

inline extern void LockMeshReadBuffers(const MeshLOD &mlod)
{
	ASSERT(mlod.mlod_iBufferBindID!=(-1));
	// Get mesh vertex count
	_ctMeshVertices = gfxGetVertexBufferSize(mlod.mlod_iBufferBindID);
	ASSERT(_ctMeshVertices>1);
	
	if(_bUseHardwareShaders) 
	{
		// return;
	}
	ASSERT(!_bMeshBuffersLocked); // buffers must not be locked
	ASSERT(mlod.mlod_iBufferBindID!=(-1));
	
	// Get mesh vertices and normals
	ASSERT(mlod.mlod_iVertexBufferID!=(-1));
	ASSERT(mlod.mlod_iNormalBufferID!=(-1));
	_pavMeshVertices = (MeshVertex*)gfxLockVertexSubBuffer(mlod.mlod_iVertexBufferID,0,_ctMeshVertices,GFX_READ);
	_panMeshNormals  = (MeshNormal*)gfxLockNormalSubBuffer(mlod.mlod_iNormalBufferID,0,_ctMeshVertices,GFX_READ);
	ASSERT(_pavMeshVertices!=NULL);
	ASSERT(_panMeshNormals!=NULL);
	
	// if mesh has weights 
	if(mlod.mlod_iWeightBufferID!=(-1)) 
	{
		// Get mesh vertex weights
		ASSERT(mlod.mlod_iWeightBufferID!=(-1));
		_pawMeshWeights  = (MeshVertexWeightInfo*)gfxLockWeightSubBuffer(mlod.mlod_iWeightBufferID,0,_ctMeshVertices,GFX_READ);
		ASSERT(_pawMeshWeights!=NULL);
	}
	
	ASSERT(_patcMeshTexCoords.Count()==0);
	
	// for each uvmap
	const INDEX ctmuvm = mlod.mlod_aUVMaps.Count();
	if(ctmuvm>0) 
	{
		_patcMeshTexCoords.Push(ctmuvm);
	}
	for(INDEX imuvm=0;imuvm<ctmuvm;imuvm++) 
	{
		// Get uvmap texcoords
		const MeshUVMap &muvm = mlod.mlod_aUVMaps[imuvm];
		ASSERT(muvm.muvm_iTexCoordBufferID!=(-1));
		_patcMeshTexCoords[imuvm] = (MeshTexCoord*)gfxLockTexCoordSubBuffer(muvm.muvm_iTexCoordBufferID,0,_ctMeshVertices,GFX_READ);
		ASSERT(_patcMeshTexCoords[imuvm]!=NULL);
	}
	_bMeshBuffersLocked = TRUE;
}

inline extern void UnlockMeshReadBuffers(const MeshLOD &mlod)
{
	if(_bUseHardwareShaders) 
	{
		// return;
	}
	
	ASSERT(_bMeshBuffersLocked); // buffers must be locked
	
	// Unlock buffer arrays
	ASSERT(mlod.mlod_iVertexBufferID!=(-1));
	ASSERT(mlod.mlod_iNormalBufferID!=(-1));
	gfxUnlockVertexSubBuffer(mlod.mlod_iVertexBufferID, 0);
	gfxUnlockNormalSubBuffer(mlod.mlod_iNormalBufferID, 0);
	if(mlod.mlod_iWeightBufferID!=(-1)) 
	{
		ASSERT(mlod.mlod_iWeightBufferID!=(-1));
		gfxUnlockWeightSubBuffer(mlod.mlod_iWeightBufferID, 0);
	}
	
	// for each uvmap
	const INDEX ctmuvm = mlod.mlod_aUVMaps.Count();
	for(INDEX imuvm=0;imuvm<ctmuvm;imuvm++) 
	{
		// Unlock uvmap texcoords
		const MeshUVMap &muvm = mlod.mlod_aUVMaps[imuvm];
		ASSERT(muvm.muvm_iTexCoordBufferID!=(-1));
		gfxUnlockTexCoordSubBuffer(muvm.muvm_iTexCoordBufferID, 0);
	}
	_patcMeshTexCoords.PopAll();
	
	// Reset vertex count
	_ctMeshVertices = 0;
	// Reset vertex, normal and weight pointers
	_pavMeshVertices=NULL;
	_panMeshNormals=NULL;
	_pawMeshWeights=NULL;
	_bMeshBuffersLocked = FALSE;
}



// Prepares transformation matrices for each weight in surface in given mesh lod
inline static void PrepareMeshMatrices(const RenMesh &rmsh, const MeshLOD &mlod)
{
	// if in software
	if(!_bUseHardwareShaders) 
	{
		ASSERT(_bMeshBuffersLocked); // buffers must be locked
	}
	
	if(!_bUseHardwareShaders) 
	{
		// for each surface
		INDEX ctmsrf = mlod.mlod_aSurfaces.Count();
		for(INDEX imsrf=0;imsrf<ctmsrf;imsrf++) 
		{
			const MeshSurface &msrf = mlod.mlod_aSurfaces[imsrf];
			const INDEX ctwm = msrf.msrf_aubRelIndexTable.Count();
			
			// Add new SurfaceMatrices
			SurfaceMatrices &sm = _aSurfacesMatrices.Push();
			sm.sm_iFirstMatrix = _aWeightMatrices.Count();
			sm.sm_iSurfaceID = msrf.msrf_iSurfaceID;
			sm.sm_ctMatrices = ctwm;
			
			// Add two matrices to array of model matrices
			RenMatrix *prmMatrices;
			if(ctwm>0) 
			{
				prmMatrices = _aWeightMatrices.Push(ctwm*2); // vertex + normal matrices
			}
			
			// set weight matrix order by order of weight indices in surface
			for(INDEX iwm=0;iwm<ctwm;iwm++) 
			{
				const INDEX imwm = msrf.msrf_aubRelIndexTable[iwm];
				const RenWeight &rw = _aRenWeights[rmsh.rmsh_iFirstWeight + imwm];
				// if no renbone for this renweight
				if(rw.rw_iBoneIndex==(-1)) 
				{
					// use models default transformation matrix
					const RenModel &rm = _aRenModels[rmsh.rmsh_iRenModelIndex];
					MatrixCopy(prmMatrices[iwm].rm_mTransform,rm.rm_mStrTransform);
					MatrixCopy(prmMatrices[iwm+ctwm].rm_mTransform,rm.rm_mTransform);
				}
				else 
				{
					// use renbones transformation matrix
					const RenBone &rb = _aRenBones[rw.rw_iBoneIndex];
					MatrixCopy(prmMatrices[iwm].rm_mTransform,rb.rb_mStrTransform);
					MatrixCopy(prmMatrices[iwm+ctwm].rm_mTransform,rb.rb_mTransform);
				}
			}
		}
	}
	
}

// Set transformation matrices for each weight in surface in given mesh lod
inline static void SetSurfaceMatrices(const RenMesh &rmsh, const MeshSurface &msrf, const MeshLOD &mlod)
{
	// Count weight maps used by surface
	const INDEX ctswm = msrf.msrf_aubRelIndexTable.Count();
	// set weight matrix order by order of weight indices in surface
	for(INDEX iswm=0;iswm<ctswm;iswm++) 
	{
		const INDEX imwm = msrf.msrf_aubRelIndexTable[iswm];
		const RenWeight &rw = _aRenWeights[rmsh.rmsh_iFirstWeight + imwm];
		// if no renbone for this renweight
		if(rw.rw_iBoneIndex==(-1)) 
		{
			// use models default transformation matrix
			const RenModel &rm = _aRenModels[rmsh.rmsh_iRenModelIndex];
			shaSetBoneMatrix(&rm.rm_mStrTransform,iswm);
			//MatrixCopy(prmMatrices[iwm].rm_mTransform,rm.rm_mStrTransform);
			//MatrixCopy(prmMatrices[iwm+ctwm].rm_mTransform,rm.rm_mTransform);
		}
		else 
		{
			// use renbones transformation matrix
			const RenBone &rb = _aRenBones[rw.rw_iBoneIndex];
			shaSetBoneMatrix(&rb.rb_mStrTransform,iswm);
			//MatrixCopy(prmMatrices[iwm].rm_mTransform,rb.rb_mStrTransform);
			//MatrixCopy(prmMatrices[iwm+ctwm].rm_mTransform,rb.rb_mTransform);
		}
	}
}

#define MORPHED_BUFFER_FLAGS GFX_VBM_POS|GFX_VBM_NOR
static void PrepareMorphedDynamicBuffer(INDEX ctMorphedVertices)
{
	TRACKMEM(Mem, "Ska morphed buffer");
	// if morphed buffer does not exists
	if(_iMorphedBufferBindID==(-1)) 
	{
		_iMorphedBufferBindID   = gfxCreateVertexBuffer(ctMorphedVertices,MORPHED_BUFFER_FLAGS,0,GFX_DYNAMIC);
		_iMorphedVertexBufferID = gfxGetVertexSubBufferID(_iMorphedBufferBindID,GFX_VBA_POS);
		_iMorphedNormalBufferID = gfxGetVertexSubBufferID(_iMorphedBufferBindID,GFX_VBA_NOR);
#if _DEBUG
		CPrintF("Notification: Creating morphed buffers for %d vertices\n",ctMorphedVertices);
#endif 
		// if already created
	}
	else 
	{
		SLONG slMorphedBufferSize = gfxGetVertexBufferSize(_iMorphedBufferBindID);
		// if morphed buffer needs to be resized
		if(slMorphedBufferSize<ctMorphedVertices) 
		{
#if _DEBUG
			CPrintF("Notification: Resizing morphed buffers from %d vertices to %d vertices\n",slMorphedBufferSize,ctMorphedVertices);
#endif 
			// Resize morphed buffer
			gfxDeleteVertexBuffer(_iMorphedBufferBindID);
			_iMorphedBufferBindID   = gfxCreateVertexBuffer(ctMorphedVertices,MORPHED_BUFFER_FLAGS,0,GFX_DYNAMIC);
			_iMorphedVertexBufferID = gfxGetVertexSubBufferID(_iMorphedBufferBindID,GFX_VBA_POS);
			_iMorphedNormalBufferID = gfxGetVertexSubBufferID(_iMorphedBufferBindID,GFX_VBA_NOR);
		}
	}
	ASSERT(_iMorphedBufferBindID!=(-1));
	ASSERT(_iMorphedVertexBufferID!=(-1));
	ASSERT(_iMorphedNormalBufferID!=(-1));
}

extern void DeleteMorphedDynamicBuffer(void)
{
	if(_iMorphedBufferBindID>=0) 
	{
		gfxDeleteVertexBuffer(_iMorphedBufferBindID);
		_iMorphedBufferBindID = -1;
		_iMorphedVertexBufferID = -1;
		_iMorphedNormalBufferID = -1;
	}
}

static FLOAT3D CalculateApsoluteModelStretch(const RenModel &rm)
{
	ASSERT(rm.rm_pmiModel!=NULL);
	// if root model
	const FLOAT3D vStretch = rm.rm_pmiModel->mi_vStretch;
	if(rm.rm_iParentModelIndex>0) 
	{ // 0 is dummy renmodel
		const RenModel &rmParent = _aRenModels[rm.rm_iParentModelIndex];
		const FLOAT3D vParentStretch = CalculateApsoluteModelStretch(rmParent);
		return FLOAT3D(vParentStretch(1)*vStretch(1),vParentStretch(2)*vStretch(2),vParentStretch(3)*vStretch(3));
	}
	else 
	{
		return vStretch;
	}
}

// Prepare ren mesh for rendering
extern void PrepareMeshForRendering(RenMesh &rmsh, INDEX iSkeletonLOD)
{
	// if in software
	if(!_bUseHardwareShaders) 
	{
		ASSERT(_bMeshBuffersLocked); // buffers must be locked
	}
	
	// reset obj to abs matrix and light direction
	MatrixCopy( _mTransObjToAbs, _mObjToAbs);
	_vLightDirInObj = _vLightDir;
	// clear vertices array
	_aMorphedVertices.PopAll();
	_aMorphedNormals.PopAll();
	_aFinalVertices.PopAll();
	_aFinalNormals.PopAll();
//안태훈 수정 시작	//(Add Tagent-space Normal Map)(0.1)
	_aFinalTangents.PopAll();
//안태훈 수정 끝	//(Add Tagent-space Normal Map)(0.1)
	_aWeightMatrices.PopAll();
	_aSurfacesMatrices.PopAll();
	_pavWeightedVertices = NULL;
	_panWeightedNormals  = NULL;
//안태훈 수정 시작	//(Add Tagent-space Normal Map)(0.2)
	_panWeightedTangents  = NULL;
//안태훈 수정 끝	//(Add Tagent-space Normal Map)(0.2)
	_pavMorphedVertices  = NULL;
	_panMorphedNormals   = NULL;
	_bMeshUsesMorphs     = FALSE;
	_ctMorphVertices     = 0;
	
	// get current mesh lod
	const MeshLOD &mlod = rmsh.rmsh_pMeshInst->mi_pMesh->msh_aMeshLODs[rmsh.rmsh_iMeshLODIndex];
	//안태훈 수정 시작	//(Add Tagent-space Normal Map)(0.1)
	const BOOL bHasTangent = mlod.mlod_aTangents.Count()>0 ? TRUE : FALSE;
	//안태훈 수정 끝	//(Add Tagent-space Normal Map)(0.1)
	
	// if using hardware shaders 
	if(_bUseHardwareShaders) 
	{
		// if mesh isn't prepared for hardware
		if(!(mlod.mlod_ulFlags&ML_USE_VERTEX_PROGRAM)) 
		{
			//ASSERTALWAYS("Mesh does not support hardware shaders");
			// drop to software for all models unil RM_BeginRenderingView is called (I dont care just dont trash gpu)
			_bUseHardwareShaders = FALSE;
		}
	}
	// Indices must be surface relative
	ASSERT(mlod.mlod_ulFlags&ML_SURFACE_RELATIVE_INDICES);
	
	// fetch number of render-weights and bones
	INDEX ctBones = 0;
	INDEX ctRenMorphs  = rmsh.rmsh_ctMorphs;
	INDEX ctRenWeights = rmsh.rmsh_ctWeights;
	const INDEX iLastRenWeight = rmsh.rmsh_iFirstWeight + rmsh.rmsh_ctWeights;
	CSkeleton *pskl = _aRenModels[rmsh.rmsh_iRenModelIndex].rm_pmiModel->mi_psklSkeleton;
	// if skeleton for this model exists and its currently visible
	if( pskl != NULL && iSkeletonLOD >= 0) 
	{
		// count bones in skeleton
		ctBones = pskl->skl_aSkeletonLODs[iSkeletonLOD].slod_aBones.Count();
	}
	
	// force skipping of transformations? (for profiling purposes only!)
	extern INDEX ska_bTransformMesh;
	if( !ska_bTransformMesh) ctBones = ctRenMorphs = ctRenWeights = 0;
	extern INDEX ska_bMorphMesh;
	
	// if ren mesh has some morph maps
	const INDEX iLastRenMorph = rmsh.rmsh_iFirstMorph + ctRenMorphs;
	if( ska_bMorphMesh && ctRenMorphs>0) 
	{
		for( INDEX irmrh=rmsh.rmsh_iFirstMorph; irmrh<iLastRenMorph; irmrh++) 
		{
			const RenMorph &rmph = _aRenMorphs[irmrh];
			if(_aRenMorphs[irmrh].rmp_fFactor >= 0.001f) 
			{
				_bMeshUsesMorphs = TRUE;
				break;
			}
		}
		// if some morphs are used
		if(_bMeshUsesMorphs) 
		{
			// Assign pointer to morphed vertices
			_pavMorphedVertices = _aMorphedVertices.Push(_ctMeshVertices);
			_panMorphedNormals  = _aMorphedNormals.Push(_ctMeshVertices);
			INDEX ctmsrf = mlod.mlod_aSurfaces.Count();
			for(INDEX imsrf=0;imsrf<ctmsrf;imsrf++) 
			{
				const MeshSurface &msrf = mlod.mlod_aSurfaces[imsrf];
				const BOOL bDynamicSurface = msrf.msrf_ulFlags&MS_DYNAMIC_SURFACE;
				// if this is dynamic surface(has morphs)
				if(bDynamicSurface) 
				{
					// Copy vertices for this surface
					const INDEX iFirstVertex = msrf.msrf_iFirstVertex;
					const INDEX ctVertices = msrf.msrf_ctVertices;
					ASSERT(_pavMeshVertices!=NULL);
					ASSERT(_panMeshNormals!=NULL);
					CopyLongs((ULONG*)&_pavMeshVertices[iFirstVertex],(ULONG*)&_aMorphedVertices[iFirstVertex], sizeof(MeshVertex)/sizeof(ULONG) *ctVertices);
					CopyLongs((ULONG*)&_panMeshNormals[iFirstVertex], (ULONG*)&_aMorphedNormals[iFirstVertex],  sizeof(MeshNormal)/sizeof(ULONG) *ctVertices);
					_ctMorphVertices += ctVertices;
				}
			}
		}
		else 
		{
			// Assign pointer to morphed vertices to default morphed vertices
			_pavMorphedVertices = _pavMeshVertices;
			_panMorphedNormals = _panMeshNormals;
		}
	}
	
	// if some morphs exists
	if(_bMeshUsesMorphs) 
	{
		// blend vertices and normals for each RenMorph 
		for( INDEX irmrh=rmsh.rmsh_iFirstMorph; irmrh<iLastRenMorph; irmrh++)
		{
			// blend only if factor is > 0
			RenMorph &rmh = _aRenMorphs[irmrh];
			if( rmh.rmp_fFactor < 0.001f) continue;
			INDEX ivx;
			
			// morphing type?
			if( rmh.rmp_pmmmMorphMap->mmp_bRelative) 
			{ // blend relative (new = cur + f*(dst-src))
				// vertices first...
				for(ivx=0; ivx<rmh.rmp_pmmmMorphMap->mmp_aMorphMap.Count(); ivx++) 
				{
					const INDEX vtx = rmh.rmp_pmmmMorphMap->mmp_aMorphMap[ivx].mwm_iVxIndex;
					const MeshVertexMorph &mvmDst = rmh.rmp_pmmmMorphMap->mmp_aMorphMap[ivx];
					const MeshVertex &mvSrc = _pavMeshVertices[vtx];
					const MeshNormal &mnSrc = _panMeshNormals[vtx];
					_pavMorphedVertices[vtx].x += rmh.rmp_fFactor * (mvmDst.mwm_x - mvSrc.x);
					_pavMorphedVertices[vtx].y += rmh.rmp_fFactor * (mvmDst.mwm_y - mvSrc.y);
					_pavMorphedVertices[vtx].z += rmh.rmp_fFactor * (mvmDst.mwm_z - mvSrc.z);
					_panMorphedNormals[vtx].nx += rmh.rmp_fFactor * (mvmDst.mwm_nx - mnSrc.nx);
					_panMorphedNormals[vtx].ny += rmh.rmp_fFactor * (mvmDst.mwm_ny - mnSrc.ny);
					_panMorphedNormals[vtx].nz += rmh.rmp_fFactor * (mvmDst.mwm_nz - mnSrc.nz);
				}
			}
			else 
			{ // blend absolute (1-f)*cur + f*dst (RARE CASE - no need for separate-array-optimization)
				// both vertices and normals in one go
				for( ivx=0; ivx<rmh.rmp_pmmmMorphMap->mmp_aMorphMap.Count(); ivx++) 
				{
					const INDEX vtx = rmh.rmp_pmmmMorphMap->mmp_aMorphMap[ivx].mwm_iVxIndex;
					const MeshVertexMorph &mvmDst = rmh.rmp_pmmmMorphMap->mmp_aMorphMap[ivx];
					_pavMorphedVertices[vtx].x = (1.0f-rmh.rmp_fFactor) * _pavMorphedVertices[vtx].x + rmh.rmp_fFactor * mvmDst.mwm_x;
					_pavMorphedVertices[vtx].y = (1.0f-rmh.rmp_fFactor) * _pavMorphedVertices[vtx].y + rmh.rmp_fFactor * mvmDst.mwm_y;
					_pavMorphedVertices[vtx].z = (1.0f-rmh.rmp_fFactor) * _pavMorphedVertices[vtx].z + rmh.rmp_fFactor * mvmDst.mwm_z;
					_panMorphedNormals[vtx].nx = (1.0f-rmh.rmp_fFactor) * _panMorphedNormals[vtx].nx + rmh.rmp_fFactor * mvmDst.mwm_nx;
					_panMorphedNormals[vtx].ny = (1.0f-rmh.rmp_fFactor) * _panMorphedNormals[vtx].ny + rmh.rmp_fFactor * mvmDst.mwm_ny;
					_panMorphedNormals[vtx].nz = (1.0f-rmh.rmp_fFactor) * _panMorphedNormals[vtx].nz + rmh.rmp_fFactor * mvmDst.mwm_nz;
				}
			}
		}
		
		// if using hardware shaders
		if(_bUseHardwareShaders) 
		{
			PrepareMorphedDynamicBuffer(_ctMorphVertices);
			
			ASSERT(_iMorphedBufferBindID!=(-1));
			ASSERT(_iMorphedVertexBufferID!=(-1));
			ASSERT(_iMorphedNormalBufferID!=(-1));
			
			// Copy morphed vertices to dynamic buffer
			GFXVertex *pavMorphedVertices = (GFXVertex*)gfxLockVertexSubBuffer(_iMorphedVertexBufferID,0,_ctMorphVertices,GFX_WRITE);
			memcpy(pavMorphedVertices,_pavMorphedVertices,sizeof(GFXVertex)*_ctMorphVertices);
			gfxUnlockVertexSubBuffer(_iMorphedVertexBufferID, 0);
			
			// Copy morphed normals to dynamic buffer
			GFXNormal *panMorphedNormals  = (GFXNormal*)gfxLockNormalSubBuffer(_iMorphedNormalBufferID,0,_ctMorphVertices,GFX_WRITE);
			memcpy(panMorphedNormals,_panMorphedNormals,sizeof(GFXNormal)*_ctMorphVertices);
			gfxUnlockNormalSubBuffer(_iMorphedNormalBufferID, 0);
		}
	}
	
	// if there is skeleton attached to this mesh
	if( ctBones>0 && ctRenWeights>0)
	{
		// Prepare transformation matrices for each weight in surface in given mesh lod
		PrepareMeshMatrices(rmsh,mlod);
		
		// if software vertex procesing is set
		if(!_bUseHardwareShaders) 
		{
			// assign final vertices and normals
			_pavWeightedVertices = _aFinalVertices.Push(_ctMeshVertices);
			_panWeightedNormals  = _aFinalNormals.Push(_ctMeshVertices);
			
			// for each surface
			const INDEX ctmsrf = mlod.mlod_aSurfaces.Count();
			for(INDEX imsrf=0;imsrf<ctmsrf;imsrf++) 
			{
				const MeshSurface &msrf = mlod.mlod_aSurfaces[imsrf];
				const SurfaceMatrices &sm = _aSurfacesMatrices[imsrf];
				const RenMatrix *prmFirst = &_aWeightMatrices[sm.sm_iFirstMatrix];
				
				ASSERT(sm.sm_ctMatrices == msrf.msrf_aubRelIndexTable.Count());
				const INDEX ctrm = sm.sm_ctMatrices;
				const INDEX ivxFirst = msrf.msrf_iFirstVertex;
				const INDEX ctvx = msrf.msrf_ctVertices;
				const INDEX ctWeightsPerVertex = GetWeightsPerVertex(msrf,_fDistanceFactor);
				const BOOL bDynamicSurface = msrf.msrf_ulFlags&MS_DYNAMIC_SURFACE;
				
				MeshVertex *pmvSrcVertices;
				MeshNormal *pmnSrcNormals;
				MeshVertex *pmvDstVertices;
				MeshNormal *pmnDstNormals;
				// if this is dynamic surface
				if(bDynamicSurface && _bMeshUsesMorphs) 
				{
					// use morphed vertices and normals (if no morphs active use mesh vertices and mormals)
					pmvSrcVertices = _pavMorphedVertices;
					pmnSrcNormals  = _panMorphedNormals;
					pmvDstVertices = _pavMorphedVertices;
					pmnDstNormals  = _panMorphedNormals;
					// must not write to source
					ASSERT(pmvDstVertices!=_pavMeshVertices);
					ASSERT(pmnDstNormals!=_panMeshNormals);
				}
				else 
				{
					pmvSrcVertices = _pavMeshVertices;
					pmnSrcNormals  = _panMeshNormals;
					pmvDstVertices = _pavWeightedVertices;
					pmnDstNormals  = _panWeightedNormals;
					// must not write to source
					ASSERT(pmvDstVertices!=_pavMeshVertices);
					ASSERT(pmnDstNormals!=_panMeshNormals);
				}
				
				// if using only one weight per vertex
				if(ctWeightsPerVertex==1) 
				{
					// for all vertices in surface
					for(INDEX ivx=ivxFirst;ivx<ctvx+ivxFirst;ivx++) 
					{
						const MeshVertexWeightInfo &mvwi = _pawMeshWeights[ivx];
						const INDEX iBoneIndex = mvwi.mvwi_aubIndices[0];
						const Matrix12 &mTransform = prmFirst[iBoneIndex].rm_mTransform;
						const Matrix12 &mRotate    = prmFirst[iBoneIndex+ctrm].rm_mTransform;
						
						// Transform vertices
						TransformVector_copy((FLOAT3&)pmvDstVertices[ivx], (FLOAT3&)pmvSrcVertices[ivx], mTransform);
						// Rotate normals
						RotateVector_copy((FLOAT3&)pmnDstNormals[ivx], (FLOAT3&)pmnSrcNormals[ivx], mRotate);
					}
					// if using two weight per vertex
				}
				else if(ctWeightsPerVertex==2) 
				{
					for(INDEX ivx=ivxFirst;ivx<ctvx+ivxFirst;ivx++) 
					{
						const MeshVertexWeightInfo &mvwi = _pawMeshWeights[ivx];
						const INDEX iBoneIndex1 = mvwi.mvwi_aubIndices[0];
						const INDEX iBoneIndex2 = mvwi.mvwi_aubIndices[1];
						const FLOAT fWeight1 = NormByteToFloat(mvwi.mvwi_aubWeights[0]);
						const FLOAT fWeight2 = 1.0f - fWeight1;
						const Matrix12 &mTransform1 = prmFirst[iBoneIndex1].rm_mTransform;
						const Matrix12 &mTransform2 = prmFirst[iBoneIndex2].rm_mTransform;
						const Matrix12 &mRotate1    = prmFirst[iBoneIndex1+ctrm].rm_mTransform;
						const Matrix12 &mRotate2    = prmFirst[iBoneIndex2+ctrm].rm_mTransform;
						MeshVertex vx;
						MeshNormal nx;
						// Transform vertices
						TransformVector_copy((FLOAT3&)vx,(FLOAT3&)pmvSrcVertices[ivx], mTransform1, fWeight1);
						TransformVector_add((FLOAT3&)vx, (FLOAT3&)pmvSrcVertices[ivx], mTransform2, fWeight2);
						pmvDstVertices[ivx] = vx;
						// Rotate normals
						RotateVector_copy((FLOAT3&)nx,(FLOAT3&)pmnSrcNormals[ivx], mRotate1, fWeight1);
						RotateVector_add((FLOAT3&)nx, (FLOAT3&)pmnSrcNormals[ivx], mRotate2, fWeight2);
						pmnDstNormals[ivx] = nx;
					}
					// if using three weight per vertex
				}
				else if(ctWeightsPerVertex==3) 
				{
					for(INDEX ivx=ivxFirst;ivx<ctvx+ivxFirst;ivx++) 
					{
						const MeshVertexWeightInfo &mvwi = _pawMeshWeights[ivx];
						const INDEX iBoneIndex1 = mvwi.mvwi_aubIndices[0];
						const INDEX iBoneIndex2 = mvwi.mvwi_aubIndices[1];
						const INDEX iBoneIndex3 = mvwi.mvwi_aubIndices[2];
						const FLOAT fWeight1 = NormByteToFloat(mvwi.mvwi_aubWeights[0]);
						const FLOAT fWeight2 = NormByteToFloat(mvwi.mvwi_aubWeights[1]);
						const FLOAT fWeight3 = 1.0f - (fWeight1+fWeight2);
						const Matrix12 &mTransform1 = prmFirst[iBoneIndex1].rm_mTransform;
						const Matrix12 &mTransform2 = prmFirst[iBoneIndex2].rm_mTransform;
						const Matrix12 &mTransform3 = prmFirst[iBoneIndex3].rm_mTransform;
						const Matrix12 &mRotate1    = prmFirst[iBoneIndex1+ctrm].rm_mTransform;
						const Matrix12 &mRotate2    = prmFirst[iBoneIndex2+ctrm].rm_mTransform;
						const Matrix12 &mRotate3    = prmFirst[iBoneIndex3+ctrm].rm_mTransform;
						
						MeshVertex vx;
						MeshNormal nx;
						// Transform vertices
						TransformVector_copy((FLOAT3&)vx, (FLOAT3&)pmvSrcVertices[ivx], mTransform1, fWeight1);
						TransformVector_add((FLOAT3&)vx, (FLOAT3&)pmvSrcVertices[ivx], mTransform2, fWeight2);
						TransformVector_add((FLOAT3&)vx, (FLOAT3&)pmvSrcVertices[ivx], mTransform3, fWeight3);
						pmvDstVertices[ivx] = vx;
						// Rotate normals
						RotateVector_copy((FLOAT3&)nx, (FLOAT3&)pmnSrcNormals[ivx], mRotate1, fWeight1);
						RotateVector_add((FLOAT3&)nx, (FLOAT3&)pmnSrcNormals[ivx], mRotate2, fWeight2);
						RotateVector_add((FLOAT3&)nx, (FLOAT3&)pmnSrcNormals[ivx], mRotate3, fWeight3);
						pmnDstNormals[ivx] = nx;
					}
					// if using four weight per vertex
				}
				else if(ctWeightsPerVertex==4) 
				{
					for(INDEX ivx=ivxFirst;ivx<ctvx+ivxFirst;ivx++) 
					{
						const MeshVertexWeightInfo &mvwi = _pawMeshWeights[ivx];
						const INDEX iBoneIndex1 = mvwi.mvwi_aubIndices[0];
						const INDEX iBoneIndex2 = mvwi.mvwi_aubIndices[1];
						const INDEX iBoneIndex3 = mvwi.mvwi_aubIndices[2];
						const INDEX iBoneIndex4 = mvwi.mvwi_aubIndices[3];
						const FLOAT fWeight1 = NormByteToFloat(mvwi.mvwi_aubWeights[0]);
						const FLOAT fWeight2 = NormByteToFloat(mvwi.mvwi_aubWeights[1]);
						const FLOAT fWeight3 = NormByteToFloat(mvwi.mvwi_aubWeights[2]);
						const FLOAT fWeight4 = 1.0f - (fWeight1+fWeight2+fWeight3);
						const Matrix12 &mTransform1 = prmFirst[iBoneIndex1].rm_mTransform;
						const Matrix12 &mTransform2 = prmFirst[iBoneIndex2].rm_mTransform;
						const Matrix12 &mTransform3 = prmFirst[iBoneIndex3].rm_mTransform;
						const Matrix12 &mTransform4 = prmFirst[iBoneIndex4].rm_mTransform;
						const Matrix12 &mRotate1    = prmFirst[iBoneIndex1+ctrm].rm_mTransform;
						const Matrix12 &mRotate2    = prmFirst[iBoneIndex2+ctrm].rm_mTransform;
						const Matrix12 &mRotate3    = prmFirst[iBoneIndex3+ctrm].rm_mTransform;
						const Matrix12 &mRotate4    = prmFirst[iBoneIndex4+ctrm].rm_mTransform;
						
						MeshVertex vx;
						MeshNormal nx;
						// Transform vertices
						TransformVector_copy((FLOAT3&)vx, (FLOAT3&)pmvSrcVertices[ivx], mTransform1, fWeight1);
						TransformVector_add((FLOAT3&)vx, (FLOAT3&)pmvSrcVertices[ivx], mTransform2, fWeight2);
						TransformVector_add((FLOAT3&)vx, (FLOAT3&)pmvSrcVertices[ivx], mTransform3, fWeight3);
						TransformVector_add((FLOAT3&)vx, (FLOAT3&)pmvSrcVertices[ivx], mTransform4, fWeight4);
						pmvDstVertices[ivx] = vx;
						// Rotate normals
						RotateVector_copy((FLOAT3&)nx, (FLOAT3&)pmnSrcNormals[ivx], mRotate1, fWeight1);
						RotateVector_add((FLOAT3&)nx, (FLOAT3&)pmnSrcNormals[ivx], mRotate2, fWeight2);
						RotateVector_add((FLOAT3&)nx, (FLOAT3&)pmnSrcNormals[ivx], mRotate3, fWeight3);
						RotateVector_add((FLOAT3&)nx, (FLOAT3&)pmnSrcNormals[ivx], mRotate4, fWeight4);
						pmnDstNormals[ivx] = nx;
					}
				}
			}
			// else in hardware, use mesh vertices and normals
		}
		else 
		{
			_pavWeightedVertices = _pavMeshVertices;
			_panWeightedNormals  = _panMeshNormals;
		}
		
		// set light in object space
		Matrix12 mInvObjToView;
		MatrixTranspose( mInvObjToView, _mObjToAbs);
		RotateVector( _vLightDirInObj.vector, mInvObjToView);
		MatrixCopy( rmsh.rmsh_mObjToView, _mObjToView);
		MatrixCopy( rmsh.rmsh_mStrObjToView, _mObjToView);
		
		// if no skeleton but need to transform all vertices to absolute space
	}
	else if( _bTransAllToAbsolute) 
	{
		// do not use vertex program
		CObjectRestore<BOOL> or(_bUseHardwareShaders,FALSE); // ????
		
		// prepare final vertices and normals
		_pavWeightedVertices = _aFinalVertices.Push(_ctMeshVertices);
		_panWeightedNormals  = _aFinalNormals.Push(_ctMeshVertices);
		const Matrix12 &mStrTransform = _aRenModels[rmsh.rmsh_iRenModelIndex].rm_mStrTransform;
		const Matrix12 &mTransform = _aRenModels[rmsh.rmsh_iRenModelIndex].rm_mTransform;
		// for each surface
		const INDEX ctmsrf=mlod.mlod_aSurfaces.Count();
		for(INDEX imsrf=0;imsrf<ctmsrf;imsrf++) 
		{
			const MeshSurface &msrf = mlod.mlod_aSurfaces[imsrf];
			const INDEX ctVertices = msrf.msrf_ctVertices;
			const INDEX iFirstVertex = msrf.msrf_iFirstVertex;
			const BOOL bDynamicSurface = msrf.msrf_ulFlags&MS_DYNAMIC_SURFACE;
			MeshVertex *pmvSrcVertices;
			MeshNormal *pmnSrcNormals;
			MeshVertex *pmvDstVertices = _pavWeightedVertices;
			MeshNormal *pmnDstNormals  = _panWeightedNormals;
			// must not write to source
			ASSERT(pmvDstVertices!=_pavMeshVertices);
			ASSERT(pmnDstNormals!=_panMeshNormals);
			
			if(bDynamicSurface && _bMeshUsesMorphs) 
			{
				pmvSrcVertices = _pavMorphedVertices;
				pmnSrcNormals  = _panMorphedNormals;
			}
			else 
			{
				pmvSrcVertices = _pavMeshVertices;
				pmnSrcNormals = _panMeshNormals;
			}
			
			// transform every vertex using default model transform matrix (for boneless models) but respect morphs
			for(INDEX imvx=iFirstVertex;imvx<iFirstVertex+ctVertices;imvx++) 
			{
				TransformVector_copy( (FLOAT3&)pmvDstVertices[imvx], (FLOAT3&)pmvSrcVertices[imvx], mStrTransform);
				RotateVector_copy(    (FLOAT3&)pmnDstNormals[imvx],  (FLOAT3&)pmnSrcNormals[imvx], mTransform); // don't stretch normals
			}
		}
		// remember view matrix for mesh
		// (no need to transform light since we need this only for physics purposes, not rendering)
		MatrixCopy( rmsh.rmsh_mObjToView, _mObjToView);
		MatrixCopy( rmsh.rmsh_mStrObjToView, _mObjToView);
		// no skeleton = no transform, so just modify view matrix
	}
	else 
	{
		RenModel &rm = _aRenModels[rmsh.rmsh_iRenModelIndex];
		// use mesh vertices and normals as final
		_pavWeightedVertices = _pavMeshVertices;
		_panWeightedNormals  = _panMeshNormals;
		
		// if this model is full face forward
		if( mlod.mlod_ulFlags & ML_FULL_FACE_FORWARD) 
		{
			FLOAT3D vAbsStretch = CalculateApsoluteModelStretch(rm);
			Matrix12 mView,mStretch,mViewStretch;
			MakeStretchMatrix( mStretch, vAbsStretch);        // create stretch matrix
			MatrixMultiply( mView, _mObjToView, rm.rm_mTransform);     // prepare ObjToView matrix for this model (without model stretch)
			MatrixMultiply( mViewStretch, _mObjToView, rm.rm_mStrTransform); // prepare ObjToView matrix for this model (with model stretch)
			RemoveRotationFromMatrix(mView);                           // remove rotations from view matrix
			mView[ 3] = mViewStretch[ 3];
			mView[ 7] = mViewStretch[ 7];
			mView[11] = mViewStretch[11];
			MatrixMultiply( rmsh.rmsh_mObjToView, mView, mStretch); // set ObjToView matrix
			MatrixCopy( rmsh.rmsh_mStrObjToView, rmsh.rmsh_mObjToView); // set stretch ObjToView matrix
			// 3D model
		}
		else 
		{
			// just set ObjToView matrix for this model
			MatrixMultiply( rmsh.rmsh_mObjToView, _mObjToView, rm.rm_mTransform);
			MatrixMultiply( rmsh.rmsh_mStrObjToView, _mObjToView, rm.rm_mStrTransform);
		}
		// set light in object space
		Matrix12 mInvObjToAbs;
		Matrix12 mInvTransform;
		Matrix12 mLight;
		MatrixTranspose( mInvObjToAbs, _mObjToAbs);
		MatrixTranspose( mInvTransform, rm.rm_mStrTransform);
		MatrixMultiply( mLight, mInvTransform, mInvObjToAbs);
		RotateVector( _vLightDirInObj.vector, mLight);
		// change obj to abs matrix for this mesh
		MatrixMultiply( _mTransObjToAbs, _mObjToAbs, rm.rm_mTransform);
	}
}


// render one ren model
static void RenderModel_View(RenModel &rm)
{
	ASSERT( _iRenderingType==1);
	const BOOL bShowNormals = RM_GetFlags() & RMF_SHOWNORMALS;
	
	ULONG ulRenFlags = RM_GetRenderFlags();
	// prepare shader flags
	ULONG ulShaderRenFlags = 0;
	if( ulRenFlags&SRMF_FOG)  ulShaderRenFlags |= SHA_RMF_FOG;
	if( ulRenFlags&SRMF_HAZE) ulShaderRenFlags |= SHA_RMF_HAZE;
	
	// if fog or haze exits
	if(ulShaderRenFlags>0) 
	{
		// Init fog and haze shared params by all surfaces
		shaInitSharedFogAndHazeParams(ulShaderRenFlags,_aprProjection,_mObjToView,_mObjToAbs);
		// check bounding box of model against fog
		if((ulShaderRenFlags&SHA_RMF_FOG) && !shaHasFog(_bbRootAllFrames)) 
		{
			// this model has no fog after all
			ulShaderRenFlags&=~SHA_RMF_FOG;
		}
		// check bounding box of model against haze
		if(ulRenFlags&SRMF_HAZE && !shaHasHaze(_bbRootAllFrames)) 
		{
			// this model has no haze after all
			ulShaderRenFlags&=~SHA_RMF_HAZE;
		}
	}
	
	// for each mesh in renmesh
	INDEX ctmsh = rm.rm_iFirstMesh + rm.rm_ctMeshes;
	for(INDEX imsh=rm.rm_iFirstMesh;imsh<ctmsh;imsh++) 
	{
		
		RenMesh &rmsh = _aRenMeshes[imsh];
		
		// Get mesh lod
		const MeshLOD &mlod = rmsh.rmsh_pMeshInst->mi_pMesh->msh_aMeshLODs[rmsh.rmsh_iMeshLODIndex];
		// Lock buffer arrays
		LockMeshReadBuffers(mlod);
		// prepare mesh for rendering
		PrepareMeshForRendering( rmsh, rm.rm_iSkeletonLODIndex);
		// render mesh
		if ( !strcmp(rm.rm_pmiModel->GetName(), "orc") )
			CTString str = rm.rm_pmiModel->GetName();

		RenderMesh( rmsh, rm,ulShaderRenFlags);
		// Unlock buffer arrays
		UnlockMeshReadBuffers(mlod);
		
		// Reset source pointers
#if _DEBUG
		_pavMeshVertices = NULL;
		_panMeshNormals  = NULL;
		_pawMeshWeights  = NULL;
#endif
		
		// show normals in required
		// if( bShowNormals) RenderNormals();
	}
}

// render one ren model to shadowmap
static void RenderModel_Mask(RenModel &rm)
{
	ASSERT( _iRenderingType==2);
	// flag to transform all vertices in view space
	RM_SetCurrentDistance(0);
	
	INDEX ctmsh = rm.rm_iFirstMesh + rm.rm_ctMeshes;
	// for each mesh in renmodel
	for(int imsh=rm.rm_iFirstMesh;imsh<ctmsh;imsh++) 
	{
		// render mesh
		RenMesh &rmsh = _aRenMeshes[imsh];
		
		const MeshLOD &mlod = rmsh.rmsh_pMeshInst->mi_pMesh->msh_aMeshLODs[rmsh.rmsh_iMeshLODIndex];
		// Lock buffer arrays
		LockMeshReadBuffers(mlod);
		PrepareMeshForRendering(rmsh,rm.rm_iSkeletonLODIndex);
		RenderMesh(rmsh,rm,0);
		// Unlock buffer arrays
		UnlockMeshReadBuffers(mlod);
	}
}

// Get bone abs position
BOOL RM_GetRenBoneAbs(CModelInstance &mi,INDEX iBoneID,RenBone &rb)
{
	// do not transform to view space
	MakeIdentityMatrix(_mAbsToViewer);
	CObjectRestore<BOOL> or(_bTransAllToAbsolute,TRUE);
	CalculateRenderingData(mi, FALSE);
	INDEX ctrb = _aRenBones.Count();
	// for each render bone after dummy one
	for(INDEX irb=1;irb<ctrb;irb++) 
	{
		RenBone &rbone = _aRenBones[irb];
		// check if this is serched bone
		if(rbone.rb_psbBone->sb_iID == iBoneID) 
		{
			rb = rbone;
			ClearRenArrays();
			return TRUE;
		}
	}
	// Clear ren arrays
	ClearRenArrays();
	return FALSE;
}


// Returns true if bone exists and sets two given vectors as start and end point of specified bone
BOOL RM_GetBoneRelPosition(CModelInstance &mi,INDEX iBoneID, FLOAT3D &vStartPoint, FLOAT3D &vEndPoint)
{
	// do not transform to view space
	MakeIdentityMatrix(_mAbsToViewer);
	CObjectRestore<BOOL> or(_bTransAllToAbsolute,TRUE);
	// use higher lod for bone finding
	RM_SetCurrentDistance(0);
	CalculateRenderingData(mi, FALSE);
	INDEX ctrb = _aRenBones.Count();
	// for each render bone after dummy one
	for(INDEX irb=1;irb<ctrb;irb++) 
	{
		RenBone &rb = _aRenBones[irb];
		// check if this is serched bone
		if(rb.rb_psbBone->sb_iID == iBoneID) 
		{
			vStartPoint = FLOAT3D(0,0,0);
			vEndPoint   = FLOAT3D(0,0,rb.rb_psbBone->sb_fBoneLength);
			TransformVector(vStartPoint.vector,rb.rb_mBonePlacement);
			TransformVector(vEndPoint.vector,rb.rb_mBonePlacement);
			ClearRenArrays();
			return TRUE;
		}
	}
	// Clear ren arrays
	ClearRenArrays();
	return FALSE;
}

extern BOOL RM_GetBoneAbsPosition(CModelInstance &mi,INDEX iBoneID, FLOAT3D &vStartPoint, FLOAT3D &vEndPoint)
{
	CObjectRestore<BOOL> or(_bTransAllToAbsolute,TRUE);
	BOOL bRet = RM_GetBoneRelPosition(mi,iBoneID, vStartPoint, vEndPoint);
	return bRet;
}

extern BOOL RM_GetBoneTransformations(CModelInstance &mi,INDEX iBoneID, Matrix12 *mTranform)
{
	// do not transform to view space
	MakeIdentityMatrix(_mAbsToViewer);
	CObjectRestore<BOOL> or(_bTransAllToAbsolute,TRUE);
	// use higher lod for bone finding
	RM_SetCurrentDistance(0);
	CalculateRenderingData(mi, FALSE);
	INDEX ctrb = _aRenBones.Count();
	// for each render bone after dummy one
	for(INDEX irb=1;irb<ctrb;irb++) 
	{
		RenBone &rb = _aRenBones[irb];
		// check if this is serched bone
		if(rb.rb_psbBone->sb_iID == iBoneID) 
		{
			MatrixCopy(*mTranform,rb.rb_mBonePlacement);
			ClearRenArrays();
			return TRUE;
		}
	}
	// Clear ren arrays
	ClearRenArrays();
	return FALSE;
}

// Calculate distance from model
static FLOAT GetModelDistance(CModelInstance &mi)
{
	FLOAT3D vDist = FLOAT3D(_mObjToView[ 3], _mObjToView[7], _mObjToView[11]);
	return vDist.Length();
}

// Calculate complete rendering data for model instance
//안태훈 수정 시작	//(Add & Modify SSSE Effect)(0.1)
extern void CalculateRenderingData(CModelInstance &mi, BOOL bRenderToScreen)
//안태훈 수정 끝	//(Add & Modify SSSE Effect)(0.1)
{
	RM_SetObjectMatrices(mi);
	// remeber all frames bbox 
	mi.GetAllFramesBBox(_bbRootAllFrames);
	
	// if custom model distance wasn't set
	if(_fCustomDistance==(-1)) 
	{
		// Caclulate one and adjust it to fov
		_fDistanceFactor = GetModelDistance(mi) * _fMipAdjustFactor;
	}
	else 
	{
		// Use custom distance (allready adjusted to current fov)
		_fDistanceFactor = _fCustomDistance;
	}
	
	// check
	// ASSERT(_fDistanceFactor>=(-1.0f));
	
	// create first dummy model that serves as parent for the entire hierarchy
	MakeRootModel();
	// build entire hierarchy with children
	BuildHierarchy(&mi, 0);
	
//안태훈 수정 시작	//(Effect Add & Modify for Close Beta)(0.1)

//안태훈 수정 끝	//(Effect Add & Modify for Close Beta)(0.1)
	
	INDEX ctrm = _aRenModels.Count();
	// for each renmodel 
	for(int irm=1;irm<ctrm;irm++) 
	{
		RenModel &rm = _aRenModels[irm];
		// match model animations
		MatchAnims(rm, bRenderToScreen);
	}
	extern INDEX ska_bCalcBoneTransf;
	if(ska_bCalcBoneTransf) 
	{
		// Calculate transformations for all bones on already built hierarchy
//안태훈 수정 시작	//(Add & Modify SSSE Effect)(0.1)
		CalculateBoneTransforms(bRenderToScreen);
//안태훈 수정 끝	//(Add & Modify SSSE Effect)(0.1)
	}
	else 
	{
		// fill transformations for all bones with identity matrices
		DummyCalculateBoneTransforms();
	}
//강동민 수정 시작 Water 구현		04.14
	/*
	if(mi.GetWater())
	{
		shaSetWaterInformation(mi.GetWater());
		_pWaterInformation = mi.GetWater();
	}
	*/
//강동민 수정 끝 Water 구현			04.14
}


// Render one SKA model with its children
void RM_RenderSKA( CModelInstance &mi, BOOL bRenderToScreen )
{
	TRACKMEM(Mem, "RenderSKA()");
	_colModel = mi.GetModelColor();
	// mask rendering?  
	if( _iRenderingType==2) 
	{
		CObjectRestore<BOOL> or(_bUseHardwareShaders,FALSE);
		// calculate all rendering data for this model instance
		CalculateRenderingData(mi, FALSE);
		// render each RenModel to mask
		for( INDEX irm=1; irm<_aRenModels.Count(); irm++) 
		{
			RenModel &rm = _aRenModels[irm];
			RM_SetObjectMatrices( *rm.rm_pmiModel);
			RenderModel_Mask(rm);
		} // done here
		ClearRenArrays();
		return;
	}
	
	// no cluster shadows - render to view
	//ASSERT( _iRenderingType==1);
	
	// determine whether model was occluded in previous frame
	extern INDEX gap_bUseOcclusionCulling;
	const BOOL bMirrored = (RM_GetRenderFlags() & SRMF_MIRROR); // mirrored models cannot have occlusion culling!
	const BOOL bDoOcclusionCheck = (gap_bUseOcclusionCulling&1) && _pGfx->HasOcclusionCulling() && !mi.mi_bSkipOcclusionTest;
	const ULONG ulQueryID = _ulDrawPortID ^ ((ULONG)&mi);
	
	BOOL bOccluded = FALSE;
	if( !bMirrored) 
	{ // don't test models in mirror
		if( bDoOcclusionCheck) 
		{ 
			SLONG slVisiblePixels = MAX_SLONG;
			gfxPullOcclusionQuery( ulQueryID, slVisiblePixels);
			bOccluded = (slVisiblePixels==0);
		} else gfxFreeOcclusionQuery( ulQueryID);
	}
	
	// setup new occlusion test if model has enough vertices (will auto-fail if disabled)
	BOOL bInTest = FALSE; 
	const BOOL bNeedTest = bDoOcclusionCheck && !bMirrored;
	if( bNeedTest) bInTest = gfxBeginOcclusionTest( ulQueryID);
	
	// just render and test bbox if model is occluded
	if( bOccluded && bInTest) 
	{
		gfxSetColorMask(NONE);
		gfxEnableDepthTest();
		FLOATaabbox3D aabbAllFrames;
		mi.GetAllFramesBBox( aabbAllFrames);
		RM_SetObjectMatrices(mi);
		SetViewMatrix(_mObjToViewStretch);
		RenderBBox( aabbAllFrames.Min(), aabbAllFrames.Max());
	}
	// or render whole model if not occluded
	else 
	{
		gfxSetColorMask(CT_RMASK|CT_GMASK|CT_BMASK|CT_AMASK);
		// calculate all rendering data for this model instance
		CalculateRenderingData(mi, bRenderToScreen);
		const INDEX ctrm = _aRenModels.Count();
		_sfStats.IncrementCounter( CStatForm::SCI_SKAMESHES_RENDERED, ctrm-1); // meshes rendered
		// render each RenModel to view
		for( INDEX irm=1; irm<ctrm; irm++) 
		{
			RenModel &rm = _aRenModels[irm];
			RM_SetObjectMatrices( *rm.rm_pmiModel);
			RenderModel_View(rm);
		}
	}
	// done with test
	if( bInTest) gfxEndOcclusionTest();
	mi.mi_bSkipOcclusionTest = FALSE;  // one-time flag
	_sfStats.IncrementCounter( CStatForm::SCI_SKAMODELS_RENDERED); // count models rendered (or occluded)
	
	// show occlusion box?
	extern INDEX mdl_bShowOccludees;
	if( mdl_bShowOccludees) 
	{
		COLOR col;
		if( bOccluded) col = C_mlRED|CT_OPAQUE;  // occluded
		else if( bNeedTest) col = C_GREEN|CT_OPAQUE;  // visible
		else                col = C_lBLUE|CT_OPAQUE;  // not tested
		gfxDisableDepthTest();
		gfxPolygonMode(GFX_LINE);
		gfxSetColorMask(CT_RMASK|CT_GMASK|CT_BMASK|CT_AMASK);
		FLOATaabbox3D aabbAllFrames;
		mi.GetAllFramesBBox( aabbAllFrames);
		RM_SetObjectMatrices(mi);
		SetViewMatrix(_mObjToViewStretch);
		RenderBBox( aabbAllFrames.Min(), aabbAllFrames.Max(), col);
		gfxPolygonMode(GFX_FILL);
	}
	
	// show skeleton
	extern INDEX ska_bShowSkeleton;
	if( ska_bShowSkeleton || RM_GetFlags() & RMF_SHOWSKELETON) 
	{
		gfxDisableTexture();
		gfxDisableDepthTest();
		RenderSkeleton();
		gfxEnableDepthTest();
	}
	
	extern INDEX ska_bShowActiveBones;
	if( ska_bShowActiveBones || RM_GetFlags() & RMF_SHOWACTIVEBONES) 
	{
		gfxDisableTexture();
		gfxDisableDepthTest();
		RenderActiveBones();
		gfxEnableDepthTest();
	}
	
	// show root model instance colision box
	extern INDEX ska_bShowCollision;
	if( ska_bShowCollision) 
	{
		RM_SetObjectMatrices(mi);
		if( mi.mi_cbAABox.Count()>0) 
		{
			ColisionBox &cb = mi.GetCurrentColisionBox();
			RM_RenderColisionBox(mi,cb,C_mlGREEN);
		}
	}
	
	extern INDEX ska_bShowAllFramesBox;
	if( ska_bShowAllFramesBox) 
	{
		RM_SetObjectMatrices(mi);
		ColisionBox &cbAllFrames = mi.mi_cbAllFramesBBox;
		RM_RenderColisionBox(mi,cbAllFrames,C_ORANGE);
	}
	
//안태훈 수정 시작	//(Add & Modify SSSE Effect)(0.1)
	extern INDEX ska_bShowSkaTag;
	//static value = FALSE;
	//ska_bShowSkaTag = value;		//임시로 만든 부분, 실제로는 SkaStudio에서만 TRUE로 직접 세팅함.
	CStaticArray<MeshLOD*> aMeshLOD;
	if(ska_bShowSkaTag)
	{
		aMeshLOD.New(_aRenMeshes.Count());
		for(INDEX irm=0; irm<_aRenMeshes.Count(); ++irm)
		{
			aMeshLOD[irm] = &(_aRenMeshes[irm].rmsh_pMeshInst->mi_pMesh->msh_aMeshLODs[_aRenMeshes[irm].rmsh_iMeshLODIndex]);
		}
	}
//안태훈 수정 끝	//(Add & Modify SSSE Effect)(0.1)

	// reset arrays
	ClearRenArrays();

//안태훈 수정 시작	//(Add & Modify SSSE Effect)(0.1)
	Matrix12 matOld;
	if(ska_bShowSkaTag)
	{
		//value = FALSE;
		MatrixCopy(matOld, _mObjToAbs);
		for(INDEX im=0; im<aMeshLOD.Count(); ++im)
		{
			MeshLOD &mlod = *aMeshLOD[im];
			for(INDEX iTag=0; iTag<mlod.mlod_aSkaTags.Count(); ++iTag)
			{
				mlod.mlod_aSkaTags[iTag]->Render();
			}
		}
		ClearRenArrays();
		MatrixCopy(_mObjToAbs, matOld);
		//value = TRUE;
	}
//안태훈 수정 끝	//(Add & Modify SSSE Effect)(0.1)
}


// clear all ren arrays
extern void ClearRenArrays()
{
	_pAdjustBonesCallback = NULL;
	_pAdjustBonesData = NULL;
	_pAdjustShaderParams = NULL;
	_pAdjustShaderData = NULL;
	
	// clear all arrays
	_aRenModels.PopAll();
	_aRenMeshes.PopAll();
	_aRenBones.PopAll();
	_aRenMorphs.PopAll();
	_aRenWeights.PopAll();
	_aslRenBoneSearchTable.PopAll();
	_fCustomDistance = -1;
	
	// Reset render flags
	ULONG &ulRenFlags = RM_GetRenderFlags();
	ulRenFlags = NONE;
}

//강동민 수정 시작
extern void RM_ClearShaders(void)
{
	delete _pshShadowShader;
	_pshShadowShader = NULL;
	delete _pshNoShadowShader;
	_pshNoShadowShader = NULL;
	delete _pshNiceWaterShader;
	_pshNiceWaterShader = NULL;
	_bLoadedShadowShader = FALSE;
}
//강동민 수정 끝
