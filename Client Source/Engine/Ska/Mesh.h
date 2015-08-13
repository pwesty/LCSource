#ifndef SE_INCL_MESH_H
#define SE_INCL_MESH_H
#ifdef PRAGMA_ONCE
	#pragma once
#endif

#include <Engine/Base/CTString.h>
#include <Engine/Base/Serial.h>
#include <Engine/Math/Vector.h>
#include <Engine/Math/Placement.h>
#include <Engine/Templates/DynamicArray.h>
#include <Engine/Templates/StaticArray.h>
#include <Engine/Graphics/Texture.h>
#include <Engine/Graphics/Shader.h>
//안태훈 수정 시작	//(Add & Modify SSSE Effect)(0.1)
#include <Engine/Effect/CSkaTag.h>
#include <Engine/Effect/CRefCountPtr.h>
//안태훈 수정 끝	//(Add & Modify SSSE Effect)(0.1)

#define MAX_BPV 4 // max bones per vertex
#define MAX_WEIGHTS_PER_SURFACE 25
#define MAX_UVMAPS_PER_MESH 8

#define ML_HALF_FACE_FORWARD         (1UL<<0)  // half face forward
#define ML_FULL_FACE_FORWARD         (1UL<<1)  // full face forward
#define ML_USE_VERTEX_PROGRAM        (1UL<<2)  // use hardware vertex shaders
#define ML_SURFACE_RELATIVE_INDICES  (1UL<<3)  // vertex indices in surfaces are relative to that surface
#define ML_NORMALIZED_WEIGHTS        (1UL<<4)  // are weights normalized

#define MS_MESH_SUBSURFACE           (1UL<<0)  // this is mesh subsurface
#define MS_DYNAMIC_SURFACE           (1UL<<1)  // this is dynamic surface (has morphs)

struct ENGINE_API MeshLOD
{
	// Constuctor/Destructor
	MeshLOD();
	~MeshLOD();

	// Copy operator
	void operator=(const MeshLOD &mlodOther);

	CStaticArray<struct MeshVertex>           mlod_aVertices;      // array of mesh vertices
	CStaticArray<struct MeshNormal>           mlod_aNormals;	     // array of mesh normals
//안태훈 수정 시작	//(Add Tagent-space Normal Map)(0.1)
	CStaticArray<struct MeshTangent>          mlod_aTangents;	     // array of mesh tangent
//안태훈 수정 끝	//(Add Tagent-space Normal Map)(0.1)
	CStaticArray<struct MeshUVMap>            mlod_aUVMaps;        // array of mesh UV maps
	CStaticArray<struct MeshSurface>          mlod_aSurfaces;      // array of mesh surfaces
	CStaticArray<struct MeshWeightMap>        mlod_aWeightMaps;    // array of mesh weight maps
	CStaticArray<struct MeshMorphMap>         mlod_aMorphMaps;     // array of mesh morph maps
	CStaticArray<struct MeshVertexWeightInfo> mlod_aVertexWeights; // array of weight indices and factors per vertex
//안태훈 수정 시작	//(Add & Modify SSSE Effect)(0.1)
	CStaticArray< ptr_tag >     mlod_aSkaTags;
//안태훈 수정 끝	//(Add & Modify SSSE Effect)(0.1)

	INDEX mlod_iBufferBindID;   // Bind   buffer ID
	INDEX mlod_iVertexBufferID; // Vertex buffer ID 
	INDEX mlod_iNormalBufferID; // Normal buffer ID
//안태훈 수정 시작	//(Add Tagent-space Normal Map)(0.1)
	INDEX mlod_iTangentBufferID; // tangent buffer ID
//안태훈 수정 끝	//(Add Tagent-space Normal Map)(0.1)
	INDEX mlod_iWeightBufferID; // Weight buffer ID

	ULONG mlod_ulFlags;         // mesh lod flags
	FLOAT mlod_fMaxDistance;    // max distance in witch this lod is visible
	CTString mlod_fnSourceFile; // file name of ascii am file, used in Ska studio
};

//안태훈 수정 시작	//(Add Tagent-space Normal Map)(0.1)
#define EPSILONFORTANGENT 1.0e-7
#define DcVec2Add(r, a, b)           ((r)[0] = (a)[0] + (b)[0], (r)[1] = (a)[1] + (b)[1])
#define DcVec2AddTo(a, b)            ((a)[0] += (b)[0], (a)[1] += (b)[1])
#define DcVec2AddScalar(r, a, b)     ((r)[0] = (a)[0] + (b), (r)[1] = (a)[1] + (b))
#define DcVec2AddScalarTo(a, b)      ((a)[0] += (b), (a)[1] += (b))
#define DcVec2Copy(a, b)             ((b)[0] = (a)[0], (b)[1] += (a)[1])
#define DcVec2Divide(r, a, b)        ((r)[0] = (a)[0] / (b)[0], (r)[1] = (a)[1] / (b)[1])
#define DcVec2DivideBy(a, b)         ((a)[0] /= (b)[0], (a)[1] /= (b)[1])
#define DcVec2DivideByScalar(v, s)   ((v)[0] /= s, (v)[1] /= s)
#define DcVec2DivideScalar(r, v, s)  ((r)[0] = (v)[0] / s, (r)[1] = (v)[1] / s)
#define DcVec2MidPoint(c, a, b)      ((r)[0] = (b)[0] + ((a)[0] - (b)[0]) * 0.5f, (r)[1] = (b)[1] + ((a)[1] - (b)[1]) * 0.5f)
#define DcVec2Mult(a, b, c)          ((c)[0] = (a)[0] * (b)[0], (c)[1] = (a)[1] * (b)[1])
#define DcVec2MultBy(a, b)           ((a)[0] *= (b)[0], (a)[1] *= (b)[1])
#define DcVec2MultByScalar(v, s)     ((v)[0] *= s, (v)[1] *= s)
#define DcVec2MultScalar(r, v, s)    ((r)[0] = (v)[0] * s, (r)[1] = (v)[1] * s)
#define DcVec2Mad(r, a, b, c)        ((r)[0] = (a)[0] * (b)[0] + (c)[0], (r)[1] = (a)[1] * (b)[1] + (c)[1])
#define DcVec2Negate(a, b)           ((b)[0] = -(a)[0], (b)[1] = -(a)[1])
#define DcVec2Scale(r, v, s)         ((r)[0] = (v)[0] * s, (r)[1] = (v)[1] * s)
#define DcVec2ScaleBy(v, s)          ((v)[0] *= s, (v)[1] *= s)
#define DcVec2Set(v, x, y)           ((v)[0] = x, (v)[1] = y)
#define DcVec2Subtract(r, a, b)      ((r)[0] = (a)[0] - (b)[0], (r)[1] = (a)[1] - (b)[1])
#define DcVec2SubtractFrom(a, b)     ((a)[0] -= (b)[0], (a)[1] -= (b)[1])
#define DcVec2Magnitude(v)           ( sqrtf((v)[0] * (v)[0] + (v)[1] * (v)[1]))
#define DcVec2MagnitudeDouble(v)     ( sqrt((v)[0] * (v)[0] + (v)[1] * (v)[1]))
#define DcVec2NormalizeDouble(v)     { double __mag = 1.0 / DcVec2MagnitudeDouble(v); \
																			(v)[0] *= __mag; (v)[1] *= __mag; }
#define DcVec2Normalize(v)           { float __mag = 1.0f / (float)DcVec2Magnitude(v); \
																			(v)[0] *= __mag; (v)[1] *= __mag; }

#define DcVecAdd(r, a, b)            ((r)[0] = (a)[0] + (b)[0], (r)[1] = (a)[1] + (b)[1], (r)[2] = (a)[2] + (b)[2])
#define DcVecAddTo(a, b)             ((a)[0] += (b)[0], (a)[1] += (b)[1], (a)[2] += (b)[2])
#define DcVecAddScalar(r, a, b)      ((r)[0] = (a)[0] + (b), (r)[1] = (a)[1] + (b), (r)[2] = (a)[2] + (b))
#define DcVecAddScalarTo(a, b)       ((a)[0] += (b), (a)[1] += (b), (a)[2] += (b))
#define DcVecCopy(a, b)              ((b)[0] = (a)[0], (b)[1] += (a)[1], (b)[2] += (a)[2])
#define DcVecCross(r, a, b)          ((r)[0] = (a)[1] * (b)[2] - (a)[2] * (b)[1], (r)[1] = (a)[2] * (b)[0] - (a)[0] * (b)[2], (r)[2] = (a)[0] * (b)[1] - (a)[1] * (b)[0])
#define DcVecDivide(r, a, b)         ((r)[0] = (a)[0] / (b)[0], (r)[1] = (a)[1] / (b)[1], (r)[2] = (a)[2] / (b)[2])
#define DcVecDivideBy(a, b)          ((a)[0] /= (b)[0], (a)[1] /= (b)[1], (a)[2] /= (b)[2])
#define DcVecDivideByScalar(v, s)    ((v)[0] /= s, (v)[1] /= s, (v)[2] /= s)
#define DcVecDivideScalar(r, v, s)   ((r)[0] = (v)[0] / s, (r)[1] = (v)[1] / s, (r)[2] = v[2] / s)
#define DcVecMidPoint(c, a, b)       ((r)[0] = (b)[0] + ((a)[0] - (b)[0]) * 0.5f, (r)[1] = (b)[1] + ((a)[1] - (b)[1]) * 0.5f, (r)[2] = (b)[2] + ((a)[2] - (b)[2]) * 0.5f)
#define DcVecMult(a, b, c)           ((c)[0] = (a)[0] * (b)[0], (c)[1] = (a)[1] * (b)[1], (c)[2] = (a)[2] * (b)[2])
#define DcVecMultBy(a, b)            ((a)[0] *= (b)[0], (a)[1] *= (b)[1], (a)[2] *= (b)[2])
#define DcVecMultByScalar(v, s)      ((v)[0] *= s, (v)[1] *= s, (v)[2] *= s)
#define DcVecMultScalar(r, v, s)     ((r)[0] = (v)[0] * s, (r)[1] = (v)[1] * s, (r)[2] = v[2] * s)
#define DcVecMad(r, a, b, c)         ((r)[0] = (a)[0] * (b)[0] + (c)[0], (r)[1] = (a)[1] * (b)[1] + (c)[1], (r)[2] = (a)[2] * (b)[2] + (c)[2])
#define DcVecNegate(a)               ((a)[0] = -(a)[0], (a)[1] = -(a)[1], (a)[2] = -(a)[2])
#define DcVecScale(r, v, s)          ((r)[0] = (v)[0] * s, (r)[1] = (v)[1] * s, (r)[2] = v[2] * s)
#define DcVecScaleBy(v, s)           ((v)[0] *= s, (v)[1] *= s, (v)[2] *= s)
#define DcVecSet(v, x, y, z)         ((v)[0] = x, (v)[1] = y, (v)[2] = z)
#define DcVecSubtract(r, a, b)       ((r)[0] = (a)[0] - (b)[0], (r)[1] = (a)[1] - (b)[1], (r)[2] = (a)[2] - (b)[2])
#define DcVecSubtractFrom(a, b)      ((a)[0] -= (b)[0], (a)[1] -= (b)[1], (a)[2] -= (b)[2])
#define DcVecMagnitude(v)            ( sqrtf((v)[0] * (v)[0] + (v)[1] * (v)[1] + (v)[2] * (v)[2]))
#define DcVecMagnitudeDouble(v)      ( sqrt((v)[0] * (v)[0] + (v)[1] * (v)[1] + (v)[2] * (v)[2]))
#define DcVecNormalizeDouble(v,def)  { double __mag = 1.0f / DcVecMagnitudeDouble(v); \
																			 if (__mag < EPSILONFORTANGENT) {(v)[0] = (def)[0]; (v)[1] = (def)[1]; (v)[2] = (def)[2];} else \
																			 {(v)[0] *= __mag; (v)[1] *= __mag; (v)[2] *= __mag;} }
#define DcVecNormalize(v)            { float __mag = 1.0f / (float)DcVecMagnitude(v); \
																			 (v)[0] *= __mag; (v)[1] *= __mag; (v)[2] *= __mag; }
#define DcVecNormalizeD(v)           { double __mag = 1.0 / DcVecMagnitude(v); \
																			 (v)[0] *= __mag; (v)[1] *= __mag; (v)[2] *= __mag; }
#define DcVecLineLength(a, b)        ( sqrtf(((a)[0] - (b)[0]) * ((a)[0] - (b)[0]) + \
																						 ((a)[1] - (b)[1]) * ((a)[1] - (b)[1]) + \
																						 ((a)[2] - (b)[2]) * ((a)[2] - (b)[2])))
#define DcVecLerp(r, a, b, c)        { DcVecSubtract(r, b, a);  \
																			 DcVecMultByScalar(r, c);   \
																			 DcVecAddTo(r, a); } 

#define DcVecDotProduct(a, b)        ((a)[0] * (b)[0] + (a)[1] * (b)[1] + (a)[2] * (b)[2])
#define DcVecDot4(a, b)              ((a)[0] * (b)[0] + (a)[1] * (b)[1] + (a)[2] * (b)[2] + (a)[3] * (b)[3])
#define DcVecDot3(a, b)              ((a)[0] * (b)[0] + (a)[1] * (b)[1] + (a)[2] * (b)[2])

#define DC_INVALID_ARG false
#define DC_OK true


/*
struct ENGINE_API MeshTangent
{
	MeshTangent() : tx( 0 ), ty( 0 ), tz( 0 ), bdir( 0 ) {}
	MeshTangent(float x, float y, float z, float sign) : tx( x ), ty( y ), tz( z ), bdir( sign ) {}
	MeshTangent(float x, float y, float z) : tx( x ), ty( y ), tz( z ), bdir( 0 ) {}

	FLOAT tx, ty, tz;	//tangent vector
	FLOAT bdir;			//determinent for binormal
};
*/// tangent 사용시
struct ENGINE_API MeshTangent
{
	MeshTangent() : bx( 0 ), by( 0 ), bz( 0 ), tdir( 0 ) {}
	MeshTangent(float x, float y, float z, float sign) : bx( x ), by( y ), bz( z ), tdir( sign ) {}
	MeshTangent(float x, float y, float z) : bx( x ), by( y ), bz( z ), tdir( 0 ) {}

	FLOAT bx, by, bz;	//tangent vector
	FLOAT tdir;			//determinent for binormal
};
//안태훈 수정 끝	//(Add Tagent-space Normal Map)(0.1)

struct ENGINE_API MeshVertex
{
	FLOAT x, y, z;
};

struct ENGINE_API MeshNormal
{
	FLOAT nx, ny, nz;
};

struct ENGINE_API MeshVertexWeightInfo
{
	UBYTE mvwi_aubIndices[MAX_BPV]; // max of 4 weight indices per vertex
	UBYTE mvwi_aubWeights[MAX_BPV]; // max of 4 weight factors per vertex
};

struct ENGINE_API MeshUVMap
{
public:
	MeshUVMap() {
		muv_iID = -1;
		muvm_iTexCoordBufferID = -1;
	};

public:
	INDEX muv_iID;                // UVMap ID
	INDEX muvm_iTexCoordBufferID; // TexCoord buffer ID
	CStaticArray<struct MeshTexCoord> muv_aTexCoords; // Array of texcoords 
};

struct ENGINE_API MeshTexCoord
{
	FLOAT u, v;
};

struct ENGINE_API MeshSurface
{
public:
	// Constuctor/Destructor
	MeshSurface();
	~MeshSurface();
	// Copy operator
	void operator=(const MeshSurface &msrfOther);

public:
	INDEX msrf_iFirstVertex;        // Index of first vertex in mesh lod
	INDEX msrf_ctVertices;          // Vertices count in surface
	INDEX msrf_iSurfaceID;          // Name of surface
	CShader *msrf_pShader;          // Surface shader
	ShaderParams msrf_ShadingParams;// Shader params for shader
	ULONG msrf_ulFlags;             // Surface flags (dynamic, ...)
	CStaticArray<UBYTE> msrf_aubRelIndexTable;           // Table of surface relative mesh weight map indices
	CStaticArray<struct MeshTriangle> msrf_auwTriangles; // List of triangles for surface
};

struct ENGINE_API MeshTriangle
{
	UWORD mt_uwVertexIndex[3]; // triangle indices relative to surface
};

struct ENGINE_API MeshWeightMap
{
	INDEX mwm_iID; // name of weightmap
	CStaticArray<struct MeshVertexWeight> mwm_aVertexWeight; // array of weight map vertices
};

struct ENGINE_API MeshVertexWeight
{
	INDEX mww_iVertex; // absolute index of the vertex this weight refers to
	FLOAT mww_fWeight; // weight for this bone [0.0 - 1.0]
};

struct ENGINE_API MeshMorphMap
{
	INDEX mmp_iID;       // name of morph map
	BOOL  mmp_bRelative; // if this relabive or absolute morph
	CStaticArray<struct MeshVertexMorph> mmp_aMorphMap; // array of vertex morphs
};

struct ENGINE_API MeshVertexMorph
{
	INDEX mwm_iVxIndex; // absolute index of the vertex this morph refers to
	FLOAT mwm_x;
	FLOAT mwm_y;
	FLOAT mwm_z;
	FLOAT mwm_nx;
	FLOAT mwm_ny;
	FLOAT mwm_nz;
};

class ENGINE_API CMesh : public CSerial
{
#ifdef KALYDO
  static CTString strDefaultMeshPath;
#endif
public:
	CMesh();
	~CMesh();
	// Optimize mesh
	void Optimize(void);
	// Optimize mesh lod
	void OptimizeLod(MeshLOD &mLod);
	// Split surfaces so it can fit in hardware shaders
	void SplitSurfaces(MeshLOD &mlod);

	// Add mesh lod to mesh
	void AddMeshLod(MeshLOD &mlod);
	// Remove mesh lod
	void RemoveMeshLod(MeshLOD *pmlodRemove);
//안태훈 수정 시작	//(Encode Data)(0.1)
	// Read mesh from stream
	void Read_t( CTStream *istrFile);  // throw char *
	void Read_t_old( CTStream *istrFile, BOOL bHasContext);  // throw char *
	void Read_t_new( CTStream *istrFile, BOOL bHasContext);  // throw char *
	// Write mesh in stream
	void Write_t( CTStream *ostrFile); // throw char *
	void Write_t_old( CTStream *ostrFile); // throw char *
//안태훈 수정 끝	//(Encode Data)(0.1)
	// Unbind
	void Unbind(void);
	// Clear mesh
	void Clear(void);
#ifdef KALYDO
  void Load_t(const CTFileName &fnFileName);
  void Load_Delay_t(const CTFileName &fnFileName);
#endif
	// Count memory used by mesh
	SLONG GetUsedMemory(void);
	/* Get the description of this object. */
	CTString GetDescription(void);

public:
	CStaticArray<struct MeshLOD> msh_aMeshLODs; // array of mesh lods
};
// Remap triangle indices base on given remap table
ENGINE_API void RemapMeshIndices(MeshLOD &mlod, const INDEX *paiRemapTable, const INDEX ctVertices);
// Remap vertex order based on given remap table
ENGINE_API void RemapMeshVertices(MeshLOD &mlod, const INDEX *paiRemapTable, const INDEX ctVertices);
// Change surface shader
ENGINE_API void ChangeSurfaceShader_t(MeshSurface &msrf,CTString fnNewShader);


#endif  /* include-once check. */

