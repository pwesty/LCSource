#ifndef SE_INCL_SHADER_H
#define SE_INCL_SHADER_H
#ifdef PRAGMA_ONCE
	#pragma once
#endif

#include <Engine/Base/CTString.h>
#include <Engine/Base/Serial.h>
#include <Engine/Math/Vector.h>
#include <Engine/Graphics/Color.h>
#include <Engine/Graphics/GfxLibrary.h>

//강동민 수정 시작 Water 구현		04.13
#include <Engine/Effect/CNiceWater.h>
#include <Engine/SKA/ModelInstance.h>
//강동민 수정 끝 Water 구현			04.13

#include <Engine/Entities/SmcParser.h>

// Shader flags
#define BASE_DOUBLE_SIDED (1UL<<0) // Double sided
#define BASE_FULL_BRIGHT  (1UL<<1) // Full bright
//안태훈 수정 시작	//(Add Tagent-space Normal Map)(0.1)
#define BASE_FLAG_OFFSET		2
#define BASE_FLAG_DESC()	shDesc.sd_astrFlagNames[0] = "Double Sided";\
							shDesc.sd_astrFlagNames[1] = "Full Bright"
//안태훈 수정 끝	//(Add Tagent-space Normal Map)(0.1)

// Shader rendering flags
#define SHA_RMF_FOG  (1UL<<0) // model has fog
#define SHA_RMF_HAZE (1UL<<1) // model has haze

enum FOGTYPE {
	FT_NONE = 0,
	FT_OPAQUE,
	FT_NON_OPAQUE,
};

// Shader stream flags
#define SHA_POSITION_STREAM  GFX_POSITION_STREAM
#define SHA_COLOR_STREAM     GFX_COLOR_STREAM
#define SHA_TEXCOORD0        GFX_TEXCOORD0
#define SHA_TEXCOORD1        GFX_TEXCOORD1
#define SHA_TEXCOORD2        GFX_TEXCOORD2
#define SHA_TEXCOORD3        GFX_TEXCOORD3
#define SHA_NORMAL_STREAM    GFX_NORMAL_STREAM
#define SHA_WEIGHT_STREAM    GFX_WEIGHT_STREAM
//안태훈 수정 시작	//(Add Tagent-space Normal Map)(0.1)
#define SHA_TANGENT_STREAM    GFX_TANGENT_STREAM
//안태훈 수정 끝	//(Add Tagent-space Normal Map)(0.1)
#define SHA_NAKED_CODE       (1UL<<31)

struct ShaderDesc
{
	CStaticArray<class CTString> sd_astrTextureNames;
	CStaticArray<class CTString> sd_astrTexCoordNames;
	CStaticArray<class CTString> sd_astrColorNames;
	CStaticArray<class CTString> sd_astrFloatNames;
	CStaticArray<class CTString> sd_astrFlagNames;
	CStaticArray<ULONG> sd_ulStreamFlags;
	INDEX sd_ctVertexPrograms;
	INDEX sd_ctPixelPrograms;
	CTString sd_strShaderInfo;
	DWORD   VertexShaderVersion;
	DWORD   PixelShaderVersion;

	ShaderDesc()
	{
		VertexShaderVersion = D3DVS_VERSION(1,1);
		PixelShaderVersion	= D3DVS_VERSION(1,4);
	}
};

struct ShaderParams
{
	ShaderParams() {
		sp_ulFlags = 0;
	}
	~ShaderParams() {
		sp_aiTextureIDs.Clear();
		sp_aiTexCoordsIndex.Clear();
		sp_acolColors.Clear();
		sp_afFloats.Clear();
	}
	CStaticArray<INDEX> sp_aiTextureIDs;
	CStaticArray<INDEX> sp_aiTexCoordsIndex;
	CStaticArray<COLOR> sp_acolColors;
	CStaticArray<FLOAT> sp_afFloats;
	ULONG               sp_ulFlags;
};

#define MAX_VPH 5 // Max vertex program handles
struct VertexProgramHandle
{
	ULONG vph_ulHandles[MAX_VPH];
	ULONG vph_ulStreamFlags;
};

struct PixelProgramHandle
{
	ULONG pph_ulHandle;        // Pixel shader handle
};

struct ShaderRegister {
	ShaderRegister(FLOAT fx, FLOAT fy, FLOAT fz, FLOAT fw) {
		sr_fx = fx;
		sr_fy = fy;
		sr_fz = fz;
		sr_fw = fw;
	}
	ShaderRegister(FLOAT fx, FLOAT fy, FLOAT fz) {
		sr_fx = fx;
		sr_fy = fy;
		sr_fz = fz;
		sr_fw = 0.0f;
	}
	ShaderRegister(const FLOAT3D &v) {
		sr_fx = v(1);
		sr_fy = v(2);
		sr_fz = v(3);
		sr_fw = 0.0f;
	}
	ShaderRegister(const GFXColor col) {
		sr_fx = NormByteToFloat(col.r);
		sr_fy = NormByteToFloat(col.g);
		sr_fz = NormByteToFloat(col.b);
		sr_fw = NormByteToFloat(col.a);
	}

	FLOAT sr_fx;
	FLOAT sr_fy;
	FLOAT sr_fz;
	FLOAT sr_fw;
};

class ENGINE_API CShader : public CSerial
{
public:
	CShader(void) {
		hLibrary = NULL;
		ShaderFunc = NULL;
		GetShaderDesc = NULL;
		GetShaderVPCode = NULL;
		GetShaderPPCode = NULL;
	};
//안태훈 수정 시작	//(For Performance)(0.1)
	CShader(void (*pShaderFunc)(void), void (*pGetShaderDesc)(ShaderDesc *&pshDesc),
//안태훈 수정 끝	//(For Performance)(0.1)
					void (*pGetShaderVPCode)(CTString &strVPCode, INDEX iVertexProgram),
					void (*pGetShaderPPCode)(CTString &strPPCode, INDEX iPixelProgram, FOGTYPE eFogType)) {
		hLibrary = NULL;
		ShaderFunc = pShaderFunc;
		GetShaderDesc = pGetShaderDesc;
		GetShaderVPCode = pGetShaderVPCode;
		GetShaderPPCode = pGetShaderPPCode;
		ResetHandles();
	};

	~CShader(void) {
		Clear();
	};
	void DeleteHandles(void);
	void (*ShaderFunc)(void);
//안태훈 수정 시작	//(For Performance)(0.1)
	void (*GetShaderDesc)(ShaderDesc *&pshDesc);
//안태훈 수정 끝	//(For Performance)(0.1)
	void (*GetShaderVPCode)(CTString &strVPCode, INDEX iVertexProgram);
	void (*GetShaderPPCode)(CTString &strPPCode, INDEX iPixelProgram, FOGTYPE eFogType);

	void Read_t( CTStream *istrFile);  // throw char *
	void Write_t( CTStream *ostrFile); // throw char *
	// Reset vertex and pixel program handles
	void ResetHandles(void);
	// Clear shader 
	void Clear(void);
	// Get memory used by shader
	SLONG GetUsedMemory(void);

public:
	HINSTANCE hLibrary;
	CStaticArray<VertexProgramHandle> sh_aVPHandles;  // array of handles for vertex shaders 
	CStaticArray<PixelProgramHandle>  sh_aPPHandles;  // array of handles for pixel shaders
	INDEX sh_ctVertexPrograms;
	INDEX sh_ctPixelPrograms;
	CSmcParserList	m_TestShaderLog;
};

/*
 * Engine interface
 */
// Begin shader using
extern void shaBegin(CAnyProjection3D &aprProjection,CShader *pShader);
// End shader using
extern void shaEnd(void);
// Init fog and haze shared params by all shaders
extern void shaInitSharedFogAndHazeParams(ULONG ulRenFlags, CAnyProjection3D &apr, Matrix12 &mObjToView, Matrix12 &mObjToAbs);

/* Software vertex proccesing */
// Set array of vertices
ENGINE_API void shaSetVertexArray(GFXVertex *paVertices, INDEX iFirstVertex, INDEX ctVertices);
// Set array of indices
ENGINE_API void shaSetIndices(const UWORD *puwIndices, INDEX ctIndices);
// Set array of normals
ENGINE_API void shaSetNormalArray(GFXNormal *paNormals);
// Set array of weights
ENGINE_API void shaSetWeightArray(GFXWeight *paWeights);
// Set array of uv maps
ENGINE_API void shaSetSurfaceUVMaps(GFXTexCoord **paUVMaps, INDEX ctUVMaps);

/* Hardware vertex proccesing */
// Set vertex buffer ID
ENGINE_API void shaSetVertexBufferID(INDEX iVertexBufferID, INDEX iFirstVertex, INDEX ctVertices);
// Set normal buffer ID
ENGINE_API void shaSetNormalBufferID(INDEX iNormalBufferID);
//안태훈 수정 시작	//(Add Tagent-space Normal Map)(0.1)
ENGINE_API void shaSetTangentBufferID(INDEX iTagentBufferID);
//안태훈 수정 끝	//(Add Tagent-space Normal Map)(0.1)
// Set weight buffer ID
ENGINE_API void shaSetWeightBufferID(INDEX iWeightBufferID);
// Set array of texcoord buffer IDs
ENGINE_API void shaSetSurfaceUVMapsIDs(INDEX *paiTexCoordBufferIDs, INDEX ctUVMaps);
// Set constant registers values
ENGINE_API void shaSetBoneMatrix(Matrix12 const *paMatrix, INDEX iBone);
// Set number of weights per vertex to be used in vertex program
ENGINE_API void shaSetWeightsPerVertex(INDEX ctWeightsPerVertex);

/* Shared */
// Set array of texture objects for shader
ENGINE_API void shaSetSurfaceTextures(CTextureObject **paTextureObject, INDEX ctTextures);
// Set array of shader colors
ENGINE_API void shaSetSurfaceColors(COLOR *paColors, INDEX ctColors);
// Set array of floats for shader
ENGINE_API void shaSetSurfaceFloats(FLOAT *paFloats, INDEX ctFloats);
// Set shading flags
ENGINE_API void shaSetSurfaceFlags(ULONG ulFlags);
// Set base color of model 
ENGINE_API void shaSetModelColor(COLOR &colModel);
// Set light direction
ENGINE_API void shaSetLightDirection(const FLOAT3D &vLightDir);
// Set light color
ENGINE_API void shaSetLightColor(COLOR colAmbient, COLOR colLight);
// Set object to view matrix
ENGINE_API void shaSetObjToViewMatrix(Matrix12 &mat);
// Set stretched object to view matrix
ENGINE_API void shaSetObjToViewStrMatrix(Matrix12 &mat);
// Set object to abs matrix
ENGINE_API void shaSetObjToAbsMatrix(Matrix12 &mat);
// Set object rendering flags (fog,haze,etc)
ENGINE_API void shaSetRenFlags(ULONG ulRenFlags);
// Lock vertex array
ENGINE_API void shaLockArrays(void);
// Unlock vertex array
ENGINE_API void shaUnlockArrays(void);



/*
 * Shaders interface
 */

// Render given model
ENGINE_API void shaRender(void);
// Clear temp vars used by shader
ENGINE_API void shaClean(void);
// Prepare colors and texcoords for fog and haze
ENGINE_API void shaPrepareFogAndHaze(BOOL bOpaqueSurface);
// Select texture unit for fog and haze
ENGINE_API void shaSetFogTextureUnit(INDEX iFogTexUnit, BOOL bOpaque);
// Render aditional pass for fog and haze
ENGINE_API void shaDoFogAndHazePass(void);
// Adjust model color array for fog and haze
ENGINE_API void shaAttenuateColorForFogAndHaze(void);
// Adjust alpha in color array for fog and haze
ENGINE_API void shaAttenuateAlphaForFogAndHaze(void);
// Calculate lightning for given model
ENGINE_API void shaCalculateLight(void);
// Calculate lightning for given model (for specular shader)
ENGINE_API void shaCalculateLightForSpecular(void);
// Fill color buffer with given color
ENGINE_API void shaFillColorBuffer(COLOR col);

// Set current texture index in texture unit
ENGINE_API void shaSetTexture(INDEX iTexture, INDEX iTextureUnit=-1);
ENGINE_API void shaSetTexture(CTextureData *ptd, INDEX iTextureUnit=-1);
ENGINE_API void shaDisableTexture(INDEX iTextureUnit=-1);
// Set current uvmap index
ENGINE_API void shaSetUVMap(INDEX iUVMap, INDEX iTextureUnit=0);
// Set current color index
ENGINE_API void shaSetColor(INDEX icolIndex);

// Set current vertex program
//안태훈 수정 시작	//(Add Tagent-space Normal Map)(0.1)
//ENGINE_API void shaSetVertexProgram(INDEX iVertexProgram);	//원본
enum NormalMapMeshType
{
	NMMT_NONE = 0,
	NMMT_OBJSPACE,
	NMMT_TANSPACE,
};
ENGINE_API void shaSetVertexProgram(INDEX iVertexProgram, NormalMapMeshType nmmType = NMMT_NONE);	//수정본
//원래는 없는 함수들의 추가 시작
ENGINE_API void shaSetDefaultConstantRegisters(void);
//Billboard를 쓰려면 다른 VertesShaderConstant를 설정하기전에 호출해 주어야함.
ENGINE_API void shaPrepareForBillboard(BOOL bCylinderType = FALSE, BOOL bNeedCalcLight = FALSE);
ENGINE_API void shaSetVertexProgramConst_TransformMatrix();
ENGINE_API void shaSetVertexProgramConst_LightDir();
ENGINE_API void shaSetVertexProgramConst_LightColor(GFXColor colDiffuseColor, GFXColor colAmbientColor, BOOL bFullBright = FALSE);
ENGINE_API void shaSetVertexProgramConst_Multiply();
//강동민 수정 시작 Water 구현		04.20
ENGINE_API void shaSetEMBM();
//강동민 수정 끝 Water 구현			04.20
ENGINE_API void shaSetVertexProgramConst_BoneMatrix();
//원래는 없는 함수들의 추가 끝
//안태훈 수정 끝	//(Add Tagent-space Normal Map)(0.1)

// Set current pixel program
ENGINE_API void shaSetPixelProgram(INDEX iPixelProgram);
// Set current texture unit
ENGINE_API void shaSetTextureUnit(INDEX iTexUnit);
// Set const vertex program register
ENGINE_API void shaSetVertexProgramConst(INDEX iRegister, const ShaderRegister *pData, INDEX ctRegisters);
ENGINE_API void shaSetVertexProgramConst(INDEX iRegister, const Matrix12 *pmData, INDEX ctRegisters);
ENGINE_API void shaSetVertexProgramConst(INDEX iRegister, const Matrix16 *pmData, INDEX ctRegisters);
// Set const pixel program register
ENGINE_API void shaSetPixelProgramConst(INDEX iRegister, const ShaderRegister *pData, INDEX ctRegisters);
//안태훈 수정 시작	//(Add Tagent-space Normal Map)(0.1)
// Set const vertex program register
ENGINE_API void shaGetVertexProgramConst(INDEX iRegister, ShaderRegister &pData);
ENGINE_API void shaGetVertexProgramConst(INDEX iRegister, Matrix12 &pmData);
ENGINE_API void shaGetVertexProgramConst(INDEX iRegister, Matrix16 &pmData);
// Set const pixel program register
ENGINE_API void shaGetPixelProgramConst(INDEX iRegister, ShaderRegister &pData);
//안태훈 수정 끝	//(Add Tagent-space Normal Map)(0.1)


//강동민 수정 시작 Water 구현		04.13
ENGINE_API void shaSetPixelProgramConst_WaterColor();
ENGINE_API void shaSetVertexProgramConst_ProjectionMatrix();
ENGINE_API void shaSetVertexProgramConst_PI();
ENGINE_API void shaSetVertexProgramConst_LightPos();
ENGINE_API void shaSetVertexProgramConst_EyePos();
ENGINE_API void shaSetVertexProgramConst_Sine();
ENGINE_API void shaSetVertexProgramConst_CoSine();
ENGINE_API void shaSetVertexProgramConst_Fixup();
ENGINE_API void shaSetVertexProgramConst_Timer();
ENGINE_API void shaSetVertexProgramConst_WaterInformation();
//강동민 수정 끝 Water 구현			04.13

/* Software vertex proccesing */
// Set array of texcoords index
ENGINE_API void shaSetTexCoordArray(GFXTexCoord *uvNewMap, INDEX iTextureUnit=0);
// Set array of vertex colors
ENGINE_API void shaSetColorArray(GFXColor *paColors);
// Set constant color
ENGINE_API void shaSetConstantColor(const COLOR colConstant);
// Set texture matrix
ENGINE_API void shaSetTextureMatrix(const Matrix12 &mTexture, INDEX iTextureUnit=-1);
ENGINE_API void shaClearTextureMatrix(INDEX iTextureUnit=-1);


/* HArdware vertex proccesing */
ENGINE_API void shaSetTexCoordBufferID(INDEX iTexCoordBufferID, INDEX iTextureUnit=0);


// Get first vertex
ENGINE_API INDEX shaGetFirstVertex(void);
// Get vertex count
ENGINE_API INDEX shaGetVertexCount(void);
// Get index count
ENGINE_API INDEX shaGetIndexCount(void);
// Get float from array of floats
ENGINE_API FLOAT shaGetFloat(INDEX iFloatIndex);
// Get texture from array of textures
ENGINE_API CTextureObject *shaGetTexture( INDEX iTextureIndex);
// Get base color from array of colors
ENGINE_API COLOR shaGetColor(INDEX iColorIndex);
// Get shading flags
ENGINE_API ULONG &shaGetFlags();
// Get base color of model
ENGINE_API COLOR shaGetModelColor(void);
// Get light direction
ENGINE_API FLOAT3D shaGetLightDirection(void);
// Get current light color
ENGINE_API COLOR shaGetLightColor(void);
// Get current ambient volor
ENGINE_API COLOR shaGetAmbientColor(void);
// Get current set color
ENGINE_API COLOR shaGetCurrentColor(void);
// Get vertex array
ENGINE_API GFXVertex *shaGetVertexArray(void);
// Get index array
ENGINE_API const UWORD *shaGetIndexArray(void);
// Get normal array
ENGINE_API GFXNormal *shaGetNormalArray(void);
// Get uvmap array from array of uvmaps
ENGINE_API GFXTexCoord *shaGetUVMap( INDEX iUVMapIndex);
// Get color array
ENGINE_API GFXColor *shaGetColorArray(void);

// Get empty color array for modifying
ENGINE_API GFXColor *shaGetNewColorArray(void);
// Get empty texcoords array for modifying
ENGINE_API GFXTexCoord *shaGetNewTexCoordArray(void);
// Get empty v array for modifying
ENGINE_API GFXVertex *shaGetNewVertexArray(void);

// Get current projection
ENGINE_API CAnyProjection3D *shaGetProjection(void);
// Get object to view matrix
ENGINE_API Matrix12 *shaGetObjToViewMatrix(void);
// Get stretched object to view matrix
ENGINE_API Matrix12 *shaGetObjToViewStrMatrix(void);
// Get object to abs matrix
ENGINE_API Matrix12 *shaGetObjToAbsMatrix(void);
// Get object rendering flags (fog,haze,etc)
ENGINE_API ULONG shaGetRenFlags(void);

//강동민 수정 시작		03.17
ENGINE_API void shaSetWaterInformation(CNiceWater *pInfo);
//강동민 수정 끝		03.17

// Set face culling
ENGINE_API void shaCullFace(GfxFace eFace);
// Set blending operations
ENGINE_API void shaBlendFunc(GfxBlend eSrc, GfxBlend eDst);
// Enable/Disable blening
ENGINE_API void shaEnableBlend(void);
ENGINE_API void shaDisableBlend(void);
// Enable/Disable alpha test
ENGINE_API void shaEnableAlphaTest(void);
ENGINE_API void shaDisableAlphaTest(void);
// Enable/Disable depth test
ENGINE_API void shaEnableDepthTest(void);
ENGINE_API void shaDisableDepthTest(void);
// Enable/Disable depth write
ENGINE_API void shaEnableDepthWrite(void);
ENGINE_API void shaDisableDepthWrite(void);
// Set depth buffer compare mode
ENGINE_API void shaDepthFunc(GfxComp eComp);
// Set texture wrapping 
ENGINE_API void shaSetTextureWrapping( enum GfxWrap eWrapU, enum GfxWrap eWrapV, INDEX iTextureUnit=-1);
// Enable/Disable overbrightning
ENGINE_API void shaEnableOverBrightning(void);
ENGINE_API void shaDisableOverBrightning(void);
// Enable/Disable vertex program
ENGINE_API void shaEnableVertexBuffers(void);
ENGINE_API void shaDisableVertexBuffers(void);
ENGINE_API void shaEnableVertexProgram(void);
ENGINE_API void shaEnablePixelProgram(void);
ENGINE_API void shaDisableVertexProgram(void);
ENGINE_API void shaDisablePixelProgram(void);

ENGINE_API BOOL shaUsePixelProgram(void);
ENGINE_API BOOL shaUseVertexProgram(void);
ENGINE_API BOOL shaUseHWShaders(void);
ENGINE_API BOOL shaUseVertexBuffers(void);

// Is bbox in fog
ENGINE_API BOOL shaHasFog(FLOATaabbox3D &bbox);
// Is bbox in haze
ENGINE_API BOOL shaHasHaze(FLOATaabbox3D &bbox);

// Set uvmap for fog
ENGINE_API void shaSetFogUVMap(GFXTexCoord *paFogUVMap);
// Set uvmap for haze
ENGINE_API void shaSetHazeUVMap(GFXTexCoord *paHazeUVMap);
// Set array of vertex colors used in haze
ENGINE_API void shaSetHazeColorArray(GFXColor *paHazeColors);

// Is overbrightning enabled
ENGINE_API BOOL shaOverBrightningEnabled(void);
ENGINE_API void shaEnableInverted(void);
ENGINE_API void shaDisableInverted(void);


/*
 * Things to add!
 */

#define shaSetColorMask gfxSetColorMask


#if (defined _MSC_VER)
 #define DECLSPEC_DLLEXPORT _declspec (dllexport)
#else
 #define DECLSPEC_DLLEXPORT
#endif

#define SHADER_MAIN(name) \
	extern "C" DECLSPEC_DLLEXPORT void Shader_##name (void);\
	SYMBOLLOCATOR(Shader_##name);\
	extern "C" DECLSPEC_DLLEXPORT void Shader_##name (void)

#define SHADER_DESC(name,x) \
	extern "C" DECLSPEC_DLLEXPORT void Shader_Desc_##name (x);\
	SYMBOLLOCATOR(Shader_Desc_##name);\
	extern "C" DECLSPEC_DLLEXPORT void Shader_Desc_##name (x)

#define SHADER_VCODE(name,code,index) \
	extern "C" DECLSPEC_DLLEXPORT void Shader_VPCode_##name (code,index);\
	SYMBOLLOCATOR(Shader_VPCode_##name);\
	extern "C" DECLSPEC_DLLEXPORT void Shader_VPCode_##name (code,index)

#define SHADER_PCODE(name,code,index,efog) \
	extern "C" DECLSPEC_DLLEXPORT void Shader_PPCode_##name (code,index,efog);\
	SYMBOLLOCATOR(Shader_PPCode_##name);\
	extern "C" DECLSPEC_DLLEXPORT void Shader_PPCode_##name (code,index,efog)


#define EXTERN_SHADER_MAIN(name) extern "C" void Shader_##name (void)
#define EXTERN_SHADER_DESC(name,x) extern "C" void Shader_Desc_##name (x)
#define EXTERN_SHADER_VCODE(name,code,index) extern "C" void Shader_VPCode_##name (code,index)
#define EXTERN_SHADER_PCODE(name,code,index,efog) extern "C" void Shader_PPCode_##name (code,index,efog)
#define FUNC_SHADER_MAIN(name) Shader_##name
#define FUNC_SHADER_DESC(name) Shader_Desc_##name
#define FUNC_SHADER_VCODE(name) Shader_VPCode_##name
#define FUNC_SHADER_PCODE(name) Shader_PPCode_##name

#endif  /* include-once check. */


