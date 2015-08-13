#ifndef SE_INCL_RENDER_H
#define SE_INCL_RENDER_H
#ifdef PRAGMA_ONCE
  #pragma once
#endif
#include <Engine/Math/Vector.h>
#include <Engine/Math/Quaternion.h>
#include <Engine/Ska/ModelInstance.h>
#include <Engine/Ska/Skeleton.h>
#include <Engine/Ska/Mesh.h>
#include <Engine/World/WorldRayCasting.h>


#define RMF_WIREFRAME       (1UL<<0)  // set wireframe on
#define RMF_SHOWTEXTURE     (1UL<<1)  // show texture
#define RMF_SHOWNORMALS     (1UL<<2)  // show normalas  
#define RMF_SHOWSKELETON    (1UL<<3)  // show skeleton
#define RMF_SHOWACTIVEBONES (1UL<<4)  // show active bones
//강동민 수정 시작
#define RMF_SHOWSHADOW		(1UL<<5)  // show shadow
//강동민 수정 끝
//강동민 수정 시작 Water 구현		04.13
#define RMF_SHOWNICEWATER	(1UL<<6)
//강동민 수정 끝 Water 구현			04.13
                                      
// !!! THESE FLAGS MUST BE THE SAME AS RMF_ FLAGS FOR OLD MODELS !!!
#define SRMF_ATTACHMENT (1UL<<0)  // set for attachment render models
#define SRMF_FOG        (1UL<<1)  // render in fog
#define SRMF_HAZE       (1UL<<2)  // render in haze
#define SRMF_SPECTATOR  (1UL<<3)  // model will not be rendered but shadows might
#define SRMF_INVERTED   (1UL<<4)  // stretch is inverted
#define SRMF_BBOXSET    (1UL<<5)  // bounding box has been calculated
#define SRMF_MIRROR     (1UL<<6)  // mirrored/warped projection
#define SRMF_INSIDE     (1UL<<7)  // doesn't need clipping to frustum
#define SRMF_INMIRROR   (1UL<<8)  // doesn't need clipping to mirror/warp plane
#define SRMF_WEAPON     (1UL<<9)  // TEMP: weapon model is rendering so don't use ATI's Truform!

typedef FLOAT FLOAT3[3];

enum SkaPreviewFlags {
  SPF_NONE = 0,
  SPF_SELECTION,
  SPF_MODELS,
  SPF_MESH_LOD,
  SPF_SURFACES,
  SPF_SUBSURFACES,
  SPF_WEIGHTS_PER_VERTEX,
  SPF_MORPHS_PER_VERTEX,
  SPF_STRIPS,
  SPF_NORMALS,
  SPF_WEIGHTS,
  SPF_MORPHS,
  SPF_WIREFRAME,
  SPF_SKELETON,
  SPF_ACTIVE_BONES,
};

// Rendering structures
struct RenModel
{
  CModelInstance *rm_pmiModel;// pointer to model instance
  INDEX rm_iParentModelIndex; // index of parent renmodel
  INDEX rm_iParentBoneIndex;  // index of parent bone this model is attached to
  Matrix12 rm_mTransform;     // Tranform matrix for models without skeletons
  Matrix12 rm_mStrTransform;  // Stretch transform matrix for models without skeleton
  INDEX rm_iSkeletonLODIndex; // index of current skeleton lod
  
  INDEX rm_iFirstBone;        // index if first renbone
  INDEX rm_ctBones;           // renbones count for this renmodel
  INDEX rm_iFirstMesh;        // index of first renmesh
  INDEX rm_ctMeshes;          // meshes count for this renmodel
  INDEX rm_iFirstChildModel;
  INDEX rm_iNextSiblingModel;
  BOOL  rm_bInverted;          // is model inverted
};

struct RenBone
{
  SkeletonBone *rb_psbBone;   // pointer to skeleton bone
  INDEX rb_iParentIndex;      // index of parent renbone
  INDEX rb_iRenModelIndex;    // index of renmodel
  AnimPos rb_apPos;
  AnimRot rb_arRot;
  AnimPos rb_apTempPos;       // temporary position for anim blending
  AnimRot rb_arTempRot;       // temporaty rotations for anim blending
  FLOAT rb_fOffsetLen;        // offset length of bone (opt for cache miss)
  Matrix12 rb_mTransform;     // Transformation matrix for this ren bone
  Matrix12 rb_mStrTransform;  // Stretched transformation matrix for this ren bone
  Matrix12 rb_mBonePlacement; // Placement of bone in absolute space
};

struct RenMorph
{
  MeshMorphMap *rmp_pmmmMorphMap;
  FLOAT rmp_fFactor;
  FLOAT rmp_fTempFactor;      // temp factor for anim blending
};

struct RenWeight
{
  MeshWeightMap *rw_pwmWeightMap;
  INDEX rw_iBoneIndex;
};

struct RenMesh
{
  struct MeshInstance *rmsh_pMeshInst;// pointer to mesh instance
  INDEX rmsh_iRenModelIndex;          // index of renmodel that holds this ren mesh
  INDEX rmsh_iFirstWeight;            // first ren weight index
  INDEX rmsh_ctWeights;               // ren weight count used by ren mesh
  INDEX rmsh_iFirstMorph;             // first ren morph index
  INDEX rmsh_ctMorphs;                // ren morph count used by ren mesh
  INDEX rmsh_iMeshLODIndex;           // curent mesh LOD index
  Matrix12 rmsh_mObjToView;           // view matrix for this renmesh (used in shaders)
  Matrix12 rmsh_mStrObjToView;        // view matrix for this renmesh (used as view matrix)
};

struct RenMatrix
{
  Matrix12 rm_mTransform;
};

struct SurfaceMatrices
{
  INDEX sm_iSurfaceID;
  INDEX sm_iFirstMatrix;
  INDEX sm_ctMatrices;
};

// initialize batch model rendering
ENGINE_API void RM_BeginRenderingView(CAnyProjection3D &apr, CDrawPort *pdp);
ENGINE_API void RM_BeginModelRenderingMask( CAnyProjection3D &prProjection,
                                            UBYTE *pubMask, SLONG slMaskWidth, SLONG slMaskHeight);
// cleanup after batch model rendering
ENGINE_API void RM_EndRenderingView( BOOL bRestoreOrtho=TRUE);
ENGINE_API void RM_EndModelRenderingMask(void);

//강동민 수정 시작
//-----------------------------------------------------------------------------
// Name: 
// Desc: 
//-----------------------------------------------------------------------------
ENGINE_API void RM_SetRenderingType(INDEX iRenderingType);

//-----------------------------------------------------------------------------
// Name: CreateShadowMatrix()
// Desc: 텍스쳐 투영 행렬을 생성합니다.
//-----------------------------------------------------------------------------
//ENGINE_API void CreateShadowMatrix(CModelInstance *pMI, CPlacement3D &plModel, Matrix16& matShadowUV, FLOAT fTextureSize, CPlacement3D &plLight, CPlacement3D &plViewer);
//ENGINE_API void CreateShadowMatrix(CModelInstance *pMI, const CPlacement3D &plModel, Matrix16& matShadowUV, FLOAT fTextureSize, const CPlacement3D &plViewer);
//강동민 수정 시작		03.05
//ENGINE_API void CreateShadowMatrix(CModelInstance *pMI, const CPlacement3D &plModel, Matrix16& matShadowUV, Matrix12& matWorldToLight, Matrix16& matShadowProj,
//								   FLOAT fTextureSize, const CPlacement3D &plViewer);
//강동민 수정 끝		03.05
//강동민 수정 시작	04.13
ENGINE_API void CreateShadowMatrix(CModelInstance *pMI, const CPlacement3D &plModel, Matrix16& matShadowUV, const Matrix12& matWorldToLight, const Matrix16& matShadowProj,
								   const FLOAT fTextureSize, const CPlacement3D &plViewer);
//강동민 수정 끝	04.13

//-----------------------------------------------------------------------------
// Name: GetWorldMatrix()
// Desc: 주어진 위치값을 바탕으로 월드 행렬을 계산합니다.
//-----------------------------------------------------------------------------
ENGINE_API void GetWorldMatrix(const CPlacement3D &plPosition, Matrix12& matObjToAbs);

//-----------------------------------------------------------------------------
// Name: GetViewMatrix()
// Desc: 주어진 카메라의 위치를 바탕으로 뷰 행렬을 계산합니다.
//-----------------------------------------------------------------------------
ENGINE_API void GetViewMatrix(const CPlacement3D &plEye, Matrix12& matAbsToEye);

//-----------------------------------------------------------------------------
// Name: FindBestFOV()
// Desc: 물체의 전체가 다 보이는 최적의 FOV를 찾습니다.
//-----------------------------------------------------------------------------
ENGINE_API void FindBestFOV(const FLOATaabbox3D& aabbBox, const Matrix12& matLocalToLight, FLOAT& fX, FLOAT& fY);

//-----------------------------------------------------------------------------
// Name: 
// Desc: 
//-----------------------------------------------------------------------------
ENGINE_API HRESULT SetAdjustedProjectionMatrix( Matrix16& mat, FLOAT fFOV, FLOAT fAspect, 
											   FLOAT fNearPlane, FLOAT fFarPlane, 
											   FLOAT fPixDx, FLOAT fPixDy, FLOAT fVPWidth, 
											   FLOAT fVPHeight, BOOL bPerspective = TRUE);

//-----------------------------------------------------------------------------
// Name: SetFrustumMatrix()
// Desc: 절두체의 Dimension 인자를 바탕으로 투영행렬을 생성합니다.
//-----------------------------------------------------------------------------
ENGINE_API void SetFrustumMatrix( Matrix16& mat, 
								 FLOAT fLeft, FLOAT fRight, FLOAT fTop, 
								 FLOAT fBottom, FLOAT fNearPlane, FLOAT fFarPlane );

//-----------------------------------------------------------------------------
// Name: SetOrthoMatrix()
// Desc: 직교 투영행렬 생성.
//-----------------------------------------------------------------------------
ENGINE_API void SetOrthoMatrix( Matrix16& mat, 
							   FLOAT fLeft, FLOAT fRight, FLOAT fTop, 
							   FLOAT fBottom, FLOAT fNearPlane, FLOAT fFarPlane );

//-----------------------------------------------------------------------------
// Name: 
// Desc: 
//-----------------------------------------------------------------------------
ENGINE_API void RM_SetAbsToView(const Matrix12 &matAbsToView);
//강동민 수정 끝

// setup light parameters
ENGINE_API void RM_SetLightColor(COLOR colAmbient, COLOR colLight);
ENGINE_API void RM_SetLightDirection(FLOAT3D &vLightDir);
// LOD factor management
ENGINE_API void RM_SetCurrentDistance(FLOAT fDistFactor);
// Returns FOV based mip distance
ENGINE_API FLOAT RM_GetAdjustedMipDistance(FLOAT fDistance, FLOAT fMipAdjustFactor);
// Calculate mip adjustment factor based on projection FOV(if it exists)
ENGINE_API FLOAT RM_GetMipAdjustmentFactor(const CProjection3D *pprProjection);
// Sets number of weights per vertex
ENGINE_API void RM_SetMaxWeightsPerVertex(const INDEX ctWeightsPerVertex);
// setup object position
ENGINE_API void RM_SetObjectPlacement(const CPlacement3D &pl);
ENGINE_API void RM_SetObjectPlacement(const FLOATmatrix3D &m, const FLOAT3D &v);
ENGINE_API void RM_SetObjectMatrices(CModelInstance &mi);

// render one SKA model with its children
ENGINE_API void RM_RenderSKA(CModelInstance &mi, BOOL bRenderToScreen = FALSE);
ENGINE_API void RM_PreviewSKA(CModelInstance &mi, CDrawPort *pdp, SkaPreviewFlags spf, INDEX iSelectionID=-1);

// Rendering of ska stuff
ENGINE_API void RM_RenderBone(CModelInstance &mi,INDEX iBoneID);
ENGINE_API void RM_RenderColisionBox(CModelInstance &mi,ColisionBox &cb, COLOR col);
ENGINE_API void RM_RenderCross(FLOAT3D &vPosition, COLOR col);
//안태훈 수정 시작	//(Effect Add & Modify for Close Beta)(0.1)
ENGINE_API void RM_RenderSpline(SINT uiCount, FLOAT3D *pvPosition, SINT selIndex, COLOR col);
//안태훈 수정 끝	//(Effect Add & Modify for Close Beta)(0.1)
ENGINE_API void RM_RenderGround(CTextureObject &to, COLOR color);
//강동민 수정 시작
ENGINE_API void RM_RenderGround(CTextureData &td, COLOR color);
ENGINE_API void RM_RenderQuad(CPlacement3D& plPosition, FLOAT fWidth = 10.0f);
//강동민 수정 끝

// Returns specified renbone
ENGINE_API RenBone *RM_FindRenBone(INDEX iBoneID);
// Returns renbone array and sets renbone count
ENGINE_API RenBone *RM_GetRenBoneArray(INDEX &ctrb);
// Returns true if bone exists and sets two given vectors as start and end point of specified bone
ENGINE_API BOOL RM_GetBoneAbsPosition(CModelInstance &mi,INDEX iBoneID, FLOAT3D &vStartPoint, FLOAT3D &vEndPoint);
// Returns true if bone exists and sets two given matrix as transform matrix of bone
ENGINE_API BOOL RM_GetBoneTransformations(CModelInstance &mi,INDEX iBoneID, Matrix12 *mTranform);
// Returns true if bone exists and sets two given vectors as start and end point of specified bone
ENGINE_API BOOL RM_GetBoneRelPosition(CModelInstance &mi,INDEX iBoneID, FLOAT3D &vStartPoint, FLOAT3D &vEndPoint);
// Returns Renbone
ENGINE_API BOOL RM_GetRenBoneAbs(CModelInstance &mi,INDEX iBoneID,RenBone &rb);

ENGINE_API void RM_AddSimpleShadow_View(CModelInstance &mi, const FLOAT fIntensity, const FLOATplane3D &plShadowPlane);

ENGINE_API void RM_GetModelVertices( CModelInstance &mi, CStaticStackArray<FLOAT3D> &avVertices, FLOATmatrix3D &mRotation,
                                     FLOAT3D &vPosition, FLOAT fNormalOffset, FLOAT fDistance);

// test if the ray hit any of model instance's triangles and return 
ENGINE_API FLOAT RM_TestRayCastHit( CModelInstance &mi, FLOATmatrix3D &mRotation, FLOAT3D &vPosition,const FLOAT3D &vOrigin
								   , const FLOAT3D &vTarget,FLOAT fOldDistance,INDEX *piBoneID, void *pSkatag = NULL);
ENGINE_API BOOL RM_TestRayCastHitForTerrainShadow( CModelInstance &mi, FLOATmatrix3D &mRotation, FLOAT3D &vPosition,
													const FLOAT3D &vOrig, const FLOAT3D &vDir );		// yjpark

ENGINE_API void RM_SetBoneAdjustCallback(void (*pAdjustBones)(void *pData), void *pData);
ENGINE_API void RM_SetShaderParamsAdjustCallback(void (*pAdjustShaderParams)(void *pData, INDEX iSurfaceID, CShader *pShader,ShaderParams &shParams),void *pData);

ENGINE_API void Matrix12ToQVect(QVect &qv,const Matrix12 &m12);
//강동민 수정 시작
ENGINE_API void TransformVertex( GFXVertex &v, const Matrix12 &m);
//강동민 수정 끝
/*
// Matrix12 operations
ENGINE_API void MatrixVectorToMatrix12(Matrix12 &m12,const FLOATmatrix3D &m, const FLOAT3D &v);
ENGINE_API void Matrix12ToMatrixVector(FLOATmatrix3D &c, FLOAT3D &v, const Matrix12 &m12);


ENGINE_API void MatrixMultiply(Matrix12 &c,const Matrix12 &m, const Matrix12 &n);
extern void MatrixMultiplyCP(Matrix12 &c,const Matrix12 &m, const Matrix12 &n);
extern void MatrixTranspose(Matrix12 &r, const Matrix12 &m);
extern void MakeIdentityMatrix(Matrix12 &mat);
extern void RotateVector(FLOAT3 &v, const Matrix12 &m);
*/

// model flags
ENGINE_API void RM_SetFlags(ULONG ulNewFlags);
ENGINE_API ULONG RM_GetFlags();
ENGINE_API void RM_AddFlag(ULONG ulFlag);
ENGINE_API void RM_RemoveFlag(ULONG ulFlag);
ENGINE_API ULONG &RM_GetRenderFlags();

extern INDEX GetSkeletonLOD(CSkeleton &sk, FLOAT fDistance);
extern INDEX GetMeshLOD(CMesh &msh, FLOAT fDistance);

#endif  /* include-once check. */

