//안태훈이 새로 추가한 파일임.
//안태훈 수정 시작	//(Add Tagent-space Normal Map)(0.1)

#include "StdH.h"
#include <Shaders/Common.h>
#include <d3d8.h>
#include <Engine/Math/AdditionalFunction.h>

#define DEFAULT_USE_SPECULAR		(1UL<<(BASE_FLAG_OFFSET+0)) // Use Specular Map for Default Shader
#define DEFAULT_ALWAYS_OPAQUE		(1UL<<(BASE_FLAG_OFFSET+1)) // Always opaque mode
#define DEFAULT_BILLBOARD			(1UL<<(BASE_FLAG_OFFSET+2))	// Use Billboard
#define DEFAULT_CYLINDER_BILLBOARD	(1UL<<(BASE_FLAG_OFFSET+3))	// Use Cylinder Billboard
#define DEFAULT_FLIP				(1UL<<(BASE_FLAG_OFFSET+4))	// Use Flip

static const INDEX ctTextures = 2; 
static const INDEX ctUVMaps   = 1;
static const INDEX ctColors   = 1;
static const INDEX ctFloats   = 1;
static const INDEX ctFlags    = 7;
static const INDEX ctVertexPrograms = 2;
static const INDEX ctPixelPrograms = 2;

static const INDEX iBaseTexture			= 0;
static const INDEX iSpecularTexture		= 1;
static const INDEX iBaseUVMap		= 0;
static const INDEX iDiffuseColor	= 0;
//static const INDEX iAmbientColor    = 1;
static const INDEX iSpecularPowerFloat	= 0;
static const INDEX iNonSpecularVP	= 0;
static const INDEX iUseSpecularVP	= 1;
static const INDEX iNonSpecularPP	= 0;
static const INDEX iUseSpecularPP	= 1;

static void SetRenderingState(BOOL bOpaque)
{
	if(bOpaque)
	{
		// Set depth test
		shaEnableDepthTest();
		shaDepthFunc(GFX_LESS_EQUAL);
		// Set depth write
		shaEnableDepthWrite();
		// Set blend mode
		shaDisableBlend();
		// Set alpha test
		shaEnableAlphaTest();
	}
	else
	{
		// Set depth test
		shaEnableDepthTest();
		shaDepthFunc(GFX_LESS_EQUAL);
		// Set depth write
		shaDisableDepthWrite();
		// Set blend mode
		shaEnableBlend();
		shaBlendFunc(GFX_SRC_ALPHA, GFX_INV_SRC_ALPHA);
		// Set alpha test
		shaDisableAlphaTest();
	}
}

static void VertexShaderProcess(BOOL bBillboard, BOOL bCylBillboard, BOOL bFullBright, COLOR colModelColor, BOOL bUseSpecular, BOOL bDoubleSided)
{
	//if use billboard, register c0~c3 <- billboard matrix.
	if(bBillboard)
	{
		shaPrepareForBillboard(bCylBillboard, !bFullBright);
	}
	shaSetVertexProgramConst_TransformMatrix();	//c0~c3
	if(bUseSpecular)
	{
		FLOAT3D vLightDir = shaGetLightDirection();
		FLOAT len = vLightDir.Length();
		if(!IsNormalFloat(len)) vLightDir = FLOAT3D(0,1,0);
		const ShaderRegister srLightDir(vLightDir(1), vLightDir(2), vLightDir(3), 765.01f);
		shaSetVertexProgramConst( 4, &srLightDir,     1);
	}
	else shaSetVertexProgramConst_LightDir();		//c4
	shaSetVertexProgramConst_Multiply();		//c7
	shaSetVertexProgramConst_BoneMatrix();		//c21~

	//c5, c6
	GFXColor colLight   = shaGetLightColor();
	GFXColor colAmbient = shaGetAmbientColor();
	// if full bright
	if(bFullBright)
	{
		colLight.abgr = 0xFF000000;
		colAmbient.abgr = 0xFF7F7F7F;
	}
	else
	{
		if(bUseSpecular)
		{
			//c9 for specular
			INDEX r = colLight.r + colAmbient.r;
			INDEX g = colLight.g + colAmbient.g;
			INDEX b = colLight.b + colAmbient.b;
			#define MAX(a, b) ( ((a) > (b)) ? (a) : (b) )
			UBYTE temp = Clamp(MAX(r, MAX(g, b)), (INDEX)0x00, (INDEX)0x7F);
			#undef MAX
			const ShaderRegister srMaxSpeVal(0, 0, 0, NormByteToFloat(2*2*temp));
			shaSetVertexProgramConst(9, &srMaxSpeVal, 1);
		}
	}
	//c5, c6
	colLight.MultiplyRGBCopyA1( (GFXColor)colModelColor, colLight );
	colAmbient.MultiplyRGBCopyA1( (GFXColor)colModelColor, colAmbient );
	const ShaderRegister srLightColor(colLight);
	const ShaderRegister srAmbientColor(colAmbient);
	shaSetVertexProgramConst(5, &srLightColor,   1);
	shaSetVertexProgramConst(6, &srAmbientColor, 1);

	if(bUseSpecular)
	{
		//c8
		//viewer pos & specular power setting
		CAnyProjection3D &apr  = *shaGetProjection();
		FLOAT specularPower = shaGetFloat(iSpecularPowerFloat);
		FLOATmatrix3D &mViewer = apr->pr_ViewerRotationMatrix;
		FLOAT3D vViewerObj( mViewer(3,1), mViewer(3,2), mViewer(3,3) );
		FLOAT len = vViewerObj.Length();
		if(!IsNormalFloat(len)) vViewerObj = FLOAT3D(0,0,1);
		else
		{
			vViewerObj(1) /= len;
			vViewerObj(2) /= len;
			vViewerObj(3) /= len;
		}
		const ShaderRegister srSpecular(vViewerObj(1), vViewerObj(2), vViewerObj(3), specularPower);
		shaSetVertexProgramConst(8, &srSpecular,   1);
	}

	//c10
	const ShaderRegister srMinusMultiply(2.0f, -1.0f, -2.0f, -0.5f);
	shaSetVertexProgramConst(10, &srMinusMultiply, 1);
	
	//set vertex shader
	if(bUseSpecular) shaSetVertexProgram(iUseSpecularVP);
	else shaSetVertexProgram(iNonSpecularVP);
}

static void PixelShaderProcess(BOOL bUseSpecular)
{
	// Set texture wrapping
	shaSetTextureWrapping(GFX_REPEAT, GFX_REPEAT, 0);
	if(bUseSpecular) shaSetTextureWrapping(GFX_REPEAT, GFX_REPEAT, 1);
	//사용할 Texture지정
	shaSetTexture(iBaseTexture, 0);
	if(bUseSpecular) shaSetTexture(iSpecularTexture, 1);
	//사용할 UV좌표셋
	shaSetUVMap(iBaseUVMap);
	
	//set pixel shader
	if(bUseSpecular) shaSetPixelProgram(iUseSpecularPP);
	else shaSetPixelProgram(iNonSpecularPP);
}

static void FogProcess(BOOL bOpaque)
{
	//fog가 있을때 사용할 텍스쳐 스테이지의 지정
	shaSetFogTextureUnit(2, bOpaque);
	// Set texture wrapping
	shaSetTextureWrapping(GFX_CLAMP, GFX_CLAMP, 2);
	// prepare fog and haze
	shaPrepareFogAndHaze(bOpaque);
}

static void Render(BOOL bOpaque, BOOL bDoubleSided, BOOL bFlip)
{
	if (bOpaque)
	{
		// if double sided flag is on
		if (bDoubleSided) shaCullFace(GFX_NONE);	// no face culling
		else if(bFlip) shaCullFace(GFX_FRONT);		// front face culling
		else shaCullFace(GFX_BACK);					// back face culling
		// Render surface
		shaRender();
	}
	else
	{
		// if double sided flag is on
		if (bDoubleSided)
		{
			// Render back side
			if(bFlip) shaCullFace(GFX_BACK);		// front face culling
			else shaCullFace(GFX_FRONT);			// back face culling
			shaRender();
		}
		// Render front side
		if(bFlip) shaCullFace(GFX_FRONT);		// front face culling
		else shaCullFace(GFX_BACK);				// back face culling
		shaRender();
	}
}

SHADER_MAIN(Default)
{
	const COLOR colModelColor = MulColors(shaGetModelColor(), shaGetColor(iDiffuseColor));
	//완전 불투명이 아니면 반투명임, 혹은 불투명 플래그가 설정되어 있으면 불투명.
	const BOOL bOpaque = ((colModelColor&0xFF) == 0xFF)
						|| ((shaGetFlags() & DEFAULT_ALWAYS_OPAQUE)==DEFAULT_ALWAYS_OPAQUE);
	if((colModelColor&0xFF) == 0x00) return;	//완전 투명.
	const BOOL bDoubleSided = shaGetFlags() & BASE_DOUBLE_SIDED;
	const BOOL bFullBright = shaGetFlags() & BASE_FULL_BRIGHT;
	const BOOL bUseSpecular = shaGetFlags() & DEFAULT_USE_SPECULAR;
	const BOOL bBillboard = shaGetFlags() & DEFAULT_BILLBOARD;
	const BOOL bCylBillboard = shaGetFlags() & DEFAULT_CYLINDER_BILLBOARD;
	const BOOL bFlip = shaGetFlags() & DEFAULT_FLIP;
	const BOOL bHardwareShader = shaUseHWShaders();

	if(bHardwareShader)
	{
		SetRenderingState(bOpaque);
		FogProcess(bOpaque);
		VertexShaderProcess(bBillboard, bCylBillboard, bFullBright, colModelColor, bUseSpecular, bDoubleSided);
		PixelShaderProcess(bUseSpecular);
		Render(bOpaque, bDoubleSided, bFlip);
	}
	else	//USE VS(SOFT or HARD), NON-PS
	{
		// Set texture wrapping
		shaSetTextureWrapping( GFX_REPEAT, GFX_REPEAT);
		// Set depth test
		shaEnableDepthTest();
		shaEnableDepthWrite();
		shaDepthFunc(GFX_LESS_EQUAL);
		
		shaSetTexture(iBaseTexture,0);
		shaEnableOverBrightning();
		if(bUseSpecular)
		{
			//일단 SW모드에서 Specaular는 고려하지 않음.
			//shaSetTexture(iSpecularTexture, 1);
		}
		else
		{
		}
		shaSetUVMap(iBaseUVMap);
		shaSetColor(iDiffuseColor);
		
		shaSetLightColor(shaGetAmbientColor(), shaGetLightColor());
		
		if(bOpaque) 
		{
			// Set blend mode
			shaDisableBlend();
			// Set alpha test
			shaEnableAlphaTest();

			// enable overbrightning
			shaEnableOverBrightning();
			// calculate shading
			shaCalculateLight();
			// prepare fog and haze
			shaPrepareFogAndHaze(bOpaque);

			// if double sided flag is on
			// no face culling
			if(bDoubleSided) shaCullFace(GFX_NONE);
			else if(bFlip) shaCullFace(GFX_FRONT);		// front face culling
			else shaCullFace(GFX_BACK);					// back face culling
			// Render surface
			shaRender();
			
			shaDepthFunc(GFX_EQUAL);
			// render fog and haze pass for surface
			shaDoFogAndHazePass();
			shaDepthFunc(GFX_LESS_EQUAL);
			
			// disable overbrighning
			shaDisableOverBrightning();
		}
		else 
		{
			// Set blend mode
			shaBlendFunc(GFX_SRC_ALPHA, GFX_INV_SRC_ALPHA);
			shaEnableBlend();
			shaDisableDepthWrite();
			// Set alpha test
			shaDisableAlphaTest();

			// enable overbrightning
			shaEnableOverBrightning();
			// calculate shading
			shaCalculateLight();
			// prepare fog and haze
			shaPrepareFogAndHaze(bOpaque);

			// adjust alpha for fog and haze
			shaAttenuateAlphaForFogAndHaze();
			
			// if double sided flag is on
			if(bDoubleSided) 
			{
				if(bFlip) shaCullFace(GFX_BACK);		// front face culling
				else shaCullFace(GFX_FRONT);			// back face culling
				shaRender();
			}
			if(bFlip) shaCullFace(GFX_FRONT);		// front face culling
			else shaCullFace(GFX_BACK);				// back face culling
			shaRender();
			
			// disable overbrighning
			shaDisableOverBrightning();
		}
	}
}

//안태훈 수정 시작	//(For Performance)(0.1)
SHADER_DESC(Default, ShaderDesc *&pshDesc)
{
	static bool bInit = false;
	static ShaderDesc shDescMe;
	if(!bInit)
	{
		bInit = true;
		shDescMe.sd_astrTextureNames.New(ctTextures);
		shDescMe.sd_astrTexCoordNames.New(ctUVMaps);
		shDescMe.sd_astrColorNames.New(ctColors);
		shDescMe.sd_astrFloatNames.New(ctFloats);
		shDescMe.sd_astrFlagNames.New(ctFlags);

		shDescMe.sd_astrTextureNames[0] = "Diffuse map";
		shDescMe.sd_astrTextureNames[1] = "Specular map";
		shDescMe.sd_astrTexCoordNames[0] = "Base uvmap";
		shDescMe.sd_astrFloatNames[0] = "Specular Power";
		shDescMe.sd_astrColorNames[0] = "Mat.Diffuse";
		//shDesc.sd_astrColorNames[1] = "Mat.Ambient";
		shDescMe.sd_astrFlagNames[0] = "Double Sided";
		shDescMe.sd_astrFlagNames[1] = "Full Bright";
		shDescMe.sd_astrFlagNames[BASE_FLAG_OFFSET+0] = "Use Specular";
		shDescMe.sd_astrFlagNames[BASE_FLAG_OFFSET+1] = "Always Opaque";
		shDescMe.sd_astrFlagNames[BASE_FLAG_OFFSET+2] = "Billboard";
		shDescMe.sd_astrFlagNames[BASE_FLAG_OFFSET+3] = "Cyl.Billboard";
		shDescMe.sd_astrFlagNames[BASE_FLAG_OFFSET+4] = "Face Flip";
		shDescMe.sd_strShaderInfo = "Default shader";

		shDescMe.sd_ctVertexPrograms = ctVertexPrograms;
		shDescMe.sd_ctPixelPrograms = ctPixelPrograms;
		shDescMe.sd_ulStreamFlags.New(ctVertexPrograms);
		shDescMe.sd_ulStreamFlags[0] = GFX_POSITION_STREAM | GFX_TEXCOORD0 | GFX_NORMAL_STREAM;
		shDescMe.sd_ulStreamFlags[1] = GFX_POSITION_STREAM | GFX_TEXCOORD0 | GFX_NORMAL_STREAM;
	}
	pshDesc = &shDescMe;
//안태훈 수정 끝	//(For Performance)(0.1)
}

SHADER_VCODE(Default, CTString &strVPCode, INDEX iVertexProgram)
{
	//상수등록 c0 ~ c20 사용가능, c0 ~ c7은 외부에서 입력됨.
	//vertex blending 쪽에서 최고 66 command slot까지 사용될 수 있음.
	//-------------------- Default Shader --------------------//
	//--------------- Input ---------------------//
	//--- v5     - tex coord                  ---//
	//--- r0     - vertex position            ---//
	//--- r1.xyz - vertex normal              ---//
	//--- c0     - mProjection*mView          ---//
	//--- c4.xyz - vLightDir                  ---//
	//--- c4.w   - 765.01f                    ---//
	//--- c5     - model*light rgb & model a  ---//
	//--- c6     - colAmbient                 ---//
	//--- c7     - (0.0f, 1.0f, 2.0f, 0.5f)   ---//여기까지가 외부에서 세팅
	//--- c8     - viewer position & Spe.Pow. ---//
	//--- c9.w   - Max Specular value         ---//
	//--- c10    - (-4.0f,-1.0f,-2.0f,-0.5f)  ---//
	//-------------- Output ---------------------//
	//--- oPos   - Position Output            ---//
	//--- oT0    - Tex Coord Output           ---//
	//--- oT1    - Tex Coord Output           ---//
	//--- oD0    - Model*Light rgb & model a  ---//
	//--- oD1    - Light Dir. & Specular val. ---//

	if(iVertexProgram == iNonSpecularVP)
	{
		//--- Over Burning, None Specular ---//
		strVPCode = "m4x4 oPos,   r0,       c0                  \n"
 					"mov  oT0.xy, v5.xy                         \n"	//diffuse map

					//diffuse light 관련 처리 부분 시작
					"dp3  r4.w,    r1.xyz,  c4.xyz              \n"	//빛의 방향 dot Normal
					"max  r4.w,    r4.w,    c7.x                \n"	//음수 값의 처리. 
					"mul  r5.xyz,  c5.xyz,  r4.www              \n"	//color값처리
					"add  r5.xyz,  r5.xyz,  c6.xyz              \n"	//diffuse + ambient
					//"mul  r5.xyz,  r5.xyz,  c7.www              \n"	// * 0.5
					"mov  r5.w,    c5.w                         \n"	//alpha는 입력을 그대로 출력으로
					"min  oD0,     r5,      c7.yyyy             \n"	//1초과 값 처리.
					//diffuse light 관련 처리 부분 끝
					;
	}
	else if(iVertexProgram == iUseSpecularVP)
	{
		//--- Over Burning, Use Specular ---//
		strVPCode = "m4x4 oPos,   r0,       c0                  \n"
 					"mov  oT0.xy, v5.xy                         \n"	//diffuse map
 					"mov  oT1.xy, v5.xy                         \n"	//specular map

					//diffuse light 관련 처리 부분 시작
					"dp3  r4.w,    r1.xyz,  c4.xyz              \n"	//빛의 방향 dot Normal
					"max  r4.w,    r4.w,    c7.x                \n"	//음수 값의 처리.
					"mul  r5.xyz,  c5.xyz,  r4.www              \n"	//color값처리
					"add  r5.xyz,  r5.xyz,  c6.xyz              \n"	//diffuse + ambient
					//"mul  r5.xyz,  r5.xyz,  c7.www              \n"	// * 0.5
					"mov  r5.w,    c5.w                         \n"	//alpha는 입력을 그대로 출력으로
					"min  oD0,     r5,      c7.yyyy             \n"	//1초과 값 처리.
					//diffuse light 관련 처리 부분 끝

					//specular 처리 관련 부분 시작
					"sub  r2.xyz,  c8.xyz,  r0.xyz              \n"	//make view direction, No need Normalize
					"dp3  r2.w,    r2.xyz,  r2.xyz              \n"	//normalize view direction
					"rsq  r2.w,    r2.w                         \n"
//					"mul  r2.xyz,  r2.xyz,  r2.www              \n"	//normalize 마무리
//					"add  r3.xyz,  r2.xyz,  c4.xyz              \n"	//half vector 만들기 //->mad로 최적화
					"mad  r3.xyz,  r2.xyz,  r2.www, c4.xyz      \n"	//make half vector
					"dp3  r3.w,    r3.xyz,  r3.xyz              \n"	//normalize half vector
					"rsq  r3.w,    r3.w                         \n"
					"mul  r3.xyz,  r3.xyz,  r3.www              \n"
					"dp3  r4.xyz,  r1.xyz,  r3.xyz              \n"	//Normal dot HalfVector
					"mov  r4.w,    c8.w                         \n"	//specular power for lit command
					"lit  r4.z,    r4                           \n"	//lighting 연산
					"mul  oD1,     r4.zzzz, c9.wwww             \n"	//oD1에 specular value 출력
					//specular 처리 관련 부분 끝
					;
	}
/*
	else if(iVertexProgram == iNonSpecularDSVP)
	{
		//--- Over Burning, None Specular ---//
		strVPCode = "m4x4 oPos,   r0,       c0                  \n"
 					"mov  oT0.xy, v5.xy                         \n"	//diffuse map

					//Double-Sided 관련 처리 부분 시작
					"sub  r2.xyz,  c8.xyz,  r0.xyz              \n"	//make view direction, No need Normalize
					"dp3  r2.w,    r2.xyz,  r1.xyz              \n"	//back face culling 여부의 판단.(normal dot viewerToVertex)
					"sge  r2.w,    r2.w,    c7.x                \n"	//if result < 0 then reverse normal
					"mad  r2.w,    r2.w,    c10.x,  c10.y       \n"
					"mul  r1.xyz,  r1.xyz,  r2.www              \n"
					//Double-Sided 관련 처리 부분 끝

					//diffuse light 관련 처리 부분 시작
					"dp3  r4.w,    r1.xyz,  c4.xyz              \n"	//빛의 방향 dot Normal
					"max  r4.w,    r4.w,    c7.x                \n"	//음수 값의 처리.
					"mul  r5.xyz,  c5.xyz,  r4.www              \n"	//color값처리
					"add  r5.xyz,  r5.xyz,  c6.xyz              \n"	//diffuse + ambient
					"mul  r5.xyz,  r5.xyz,  c7.www              \n"	// * 0.5
					"mov  r5.w,    c5.w                         \n"	//alpha는 입력을 그대로 출력으로
					"min  oD0,     r5,      c7.yyyy             \n"	//1초과 값 처리.
					//diffuse light 관련 처리 부분 끝
					;
	}
	else if(iVertexProgram == iUseSpecularDSVP)
	{
		//--- Over Burning, Use Specular ---//
		strVPCode = "m4x4 oPos,   r0,       c0                  \n"
 					"mov  oT0.xy, v5.xy                         \n"	//diffuse map
 					"mov  oT1.xy, v5.xy                         \n"	//specular map

					//Double-Sided 관련 처리 부분 시작
					"sub  r2.xyz,  c8.xyz,  r0.xyz              \n"	//make view direction, No need Normalize
					"dp3  r2.w,    r2.xyz,  r1.xyz              \n"	//back face culling 여부의 판단.(normal dot viewerToVertex)
					"sge  r2.w,    r2.w,    c7.x                \n"	//if result < 0 then reverse normal
					"mad  r2.w,    r2.w,    c10.x,  c10.y       \n"
					"mul  r1.xyz,  r1.xyz,  r2.www              \n"
					//Double-Sided 관련 처리 부분 끝

					//diffuse light 관련 처리 부분 시작
					"dp3  r4.w,    r1.xyz,  c4.xyz              \n"	//빛의 방향 dot Normal
					"max  r4.w,    r4.w,    c7.x                \n"	//음수 값의 처리.
					"mul  r5.xyz,  c5.xyz,  r4.www              \n"	//color값처리
					"add  r5.xyz,  r5.xyz,  c6.xyz              \n"	//diffuse + ambient
					"mul  r5.xyz,  r5.xyz,  c7.www              \n"	// * 0.5
					"mov  r5.w,    c5.w                         \n"	//alpha는 입력을 그대로 출력으로
					"min  oD0,     r5,      c7.yyyy             \n"	//1초과 값 처리.
					//diffuse light 관련 처리 부분 끝

					//specular 처리 관련 부분 시작
					"sub  r2.xyz,  c8.xyz,  r0.xyz              \n"	//make view direction
					"dp3  r2.w,    r2.xyz,  r2.xyz              \n"	//normalize view direction
					"rsq  r2.w,    r2.w                         \n"
//					"mul  r2.xyz,  r2.xyz,  r2.www              \n"	//normalize 마무리
//					"add  r3.xyz,  r2.xyz,  c4.xyz              \n"	//half vector 만들기 //->mad로 최적화
					"mad  r3.xyz,  r2.xyz,  r2.www, c4.xyz      \n"	//make half vector
					"dp3  r3.w,    r3.xyz,  r3.xyz              \n"	//normalize half vector
					"rsq  r3.w,    r3.w                         \n"
					"mul  r3.xyz,  r3.xyz,  r3.www              \n"
					"dp3  r4.xyz,  r1.xyz,  r3.xyz              \n"	//Normal dot HalfVector
					"mov  r4.w,    c8.w                         \n"	//specular power for lit command
					"lit  r4.z,    r4                           \n"	//lighting 연산
					"mul  oD1,     r4.zzzz, c9.wwww             \n"	//oD1에 specular value 출력
					//specular 처리 관련 부분 끝
					;
	}
*/
}

SHADER_PCODE(Default, CTString &strPPCode, INDEX iPixelProgram, FOGTYPE eFogType)
{
	//상수등록 c0만 가능(PS가 없는 카드와의 호환성을 고려할 경우)
	//--------------- Tagent Space Normal Map Pixel Shader --------------//
	//--------------- Input ---------------------//
	//--- t0     - Diffuse map                ---//
	//--- t1     - Specular map               ---// ->bUseSpecular가 있을때만 사용됨.
	//--- t2     - Fog map                    ---// ->Fog가 있을때만 사용됨.
	//--- v0     - diffuse color              ---//
	//--- c7     - Fog color                  ---// ->Fog가 있을때만 사용됨.
	//-------------- Output ---------------------//
	//--- r0     - Output Pixel COLOR & Alpha ---//

	//안개없음.
	if(eFogType == FT_NONE)
	{
		if(iPixelProgram == iNonSpecularPP)
		{
			//--- Over Burning, None Specular ---//
			//	Texture는 1장, Texture Stage는 1개까지 사용
/*			strPPCode = "tex        t0                                 \n"	//base(diffuse) map
						"mul_x2_sat r0.rgb,  t0,      v0               \n"	//(diffuse color + ambient color) * diffuse map
						"+mul_sat    r0.a,    t0.a,    v0.a            \n"	//diffuse map alpha * diffuse color alpha
						;*/
			strPPCode = "texld	r0,        t0                          \n"	//base(diffuse) map
						"mul_x2_sat r0.rgb,  r0,      v0               \n"	//(diffuse color + ambient color) * diffuse map
						"+mul_sat    r0.a,    r0.a,    v0.a            \n"	//diffuse map alpha * diffuse color alpha
						;
		}
		else if(iPixelProgram == iUseSpecularPP)
		{
			//--- Over Burning, Use Specular ---//
			//	Texture는 2장, Texture Stage는 2개까지 사용
/*			strPPCode = "tex        t0                                 \n"	//base(diffuse) map
						"tex        t1                                 \n"	//specular map
						"mul_x2_sat r0.rgb,  t0,      v0               \n"	//(diffuse color + ambient color) * diffuse map
						"+mul_sat    r0.a,    t0.a,    v0.a            \n"	//diffuse map alpha * diffuse color alpha
						"mad_sat    r0.rgb,  t1,      v1,      r0      \n"	//specular map * specular color + current
						;*/
			strPPCode = "texld	r0,        t0                          \n"	//base(diffuse) map
						"texld	r1,        t1                          \n"	//specular map
						"mul_x2_sat r0.rgb,  r0,      v0               \n"	//(diffuse color + ambient color) * diffuse map
						"+mul_sat    r0.a,    r0.a,    v0.a            \n"	//diffuse map alpha * diffuse color alpha
						"mad_sat    r0.rgb,  r1,      v1,      r0      \n"	//specular map * specular color + current
						;
		}
	}
	//표면이 불투명할때 안개 있음.
	else if(eFogType == FT_OPAQUE)
	{
		if(iPixelProgram == iNonSpecularPP)
		{
/*			strPPCode = "tex        t0                                 \n"	//base(diffuse) map
						"tex        t2                                 \n"	//fog map
						"mul_x2_sat r0.rgb,  t0,      v0               \n"	//(diffuse color + ambient color) * diffuse map
						"+mul_sat    r0.a,    t0.a,    v0.a            \n"	//diffuse map alpha * diffuse color alpha
						"mul        t2,      t2,      c7               \n"	// mul fog texture with fog color
						"lrp        r0.rgb,  t2.a,    t2,      r0      \n"	// Add fog
						;*/
			strPPCode = "texld	r0,      t0                                 \n"	//base(diffuse) map
						"texld  r2,      t2                                 \n"	//fog map
						"mul_x2_sat r0.rgb,  r0,      v0               \n"	//(diffuse color + ambient color) * diffuse map
						"+mul_sat    r0.a,    r0.a,    v0.a            \n"	//diffuse map alpha * diffuse color alpha
						"mul        r2,      r2,      c7               \n"	// mul fog texture with fog color
						"lrp        r0.rgb,  r2.a,    r2,      r0      \n"	// Add fog
						;
		}
		else if(iPixelProgram == iUseSpecularPP)
		{
/*			strPPCode = "tex        t0                                 \n"	//base(diffuse) map
						"tex        t1                                 \n"	//specular map
						"tex        t2                                 \n"	//fog map
						"mul_x2_sat r0.rgb,  t0,      v0               \n"	//(diffuse color + ambient color) * diffuse map
						"+mul_sat    r0.a,    t0.a,    v0.a            \n"	//diffuse map alpha * diffuse color alpha
						"mad_sat    r0.rgb,  t1,      v1,      r0      \n"	//specular map * specular color + current
						"mul        t2,      t2,      c7               \n"	// mul fog texture with fog color
						"lrp        r0.rgb,  t2.a,    t2,      r0      \n"	// Add fog
						;*/
			strPPCode = "texld	r0,        t0                                 \n"	//base(diffuse) map
						"texld	r1,        t1                                 \n"	//specular map
						"texld	r2,        t2                                 \n"	//fog map
						"mul_x2_sat r0.rgb,  r0,      v0               \n"	//(diffuse color + ambient color) * diffuse map
						"+mul_sat    r0.a,   r0.a,    v0.a            \n"	//diffuse map alpha * diffuse color alpha
						"mad_sat    r0.rgb,  r1,      v1,      r0      \n"	//specular map * specular color + current
						"mul        r2,      r2,      c7               \n"	// mul fog texture with fog color
						"lrp        r0.rgb,  r2.a,    r2,      r0      \n"	// Add fog
						;
		}
	}
	//표면이 반투명할때 안개 있음.
	else if(eFogType == FT_NON_OPAQUE)
	{
		if(iPixelProgram == iNonSpecularPP)
		{
/*			strPPCode = "tex        t0                                 \n"	//base(diffuse) map
						"tex        t2                                 \n"	//fog map
						"mul        t2,      t2,      c7               \n"	// mul fog texture with fog color
						//"mul        t0.a,    t0,      1-t2.a           \n"	// attenuate base tex with fog alpha
						"mul_x2_sat r0.rgb,  t0,      v0               \n"	//(diffuse color + ambient color) * diffuse map
						"+mul_sat    r0.a,    t0.a,    v0.a            \n"	//diffuse map alpha * diffuse color alpha

						"lrp        r0.rgb,  t2.a,    t2,      r0      \n" // Add fog
//						"+mul        r0.a,    r0.a,    1-t2.a          \n" // attenuate base tex with fog alpha
						;*/
			strPPCode = "texld	r0,        t0                                 \n"	//base(diffuse) map
						"texld  r2,      t2                                 \n"	//fog map
						"mul        r2,      r2,      c7               \n"	// mul fog texture with fog color
						//"mul        t0.a,    t0,      1-t2.a           \n"	// attenuate base tex with fog alpha
						"mul_x2_sat r0.rgb,  r0,      v0               \n"	//(diffuse color + ambient color) * diffuse map
						"+mul_sat    r0.a,    r0.a,    v0.a            \n"	//diffuse map alpha * diffuse color alpha

						"lrp        r0.rgb,  r2.a,    r2,      r0      \n" // Add fog
//						"+mul        r0.a,    r0.a,    1-t2.a          \n" // attenuate base tex with fog alpha
						;
		}
		else if(iPixelProgram == iUseSpecularPP)
		{
/*			strPPCode = "tex        t0                                 \n"	//base(diffuse) map
						"tex        t1                                 \n"	//specular map
						"tex        t2                                 \n"	//fog map
						"mul        t2,      t2,      c7               \n"	// mul fog texture with fog color
						//"mul        t0.a,    t0,      1-t2.a           \n"	// attenuate base tex with fog alpha
						"mul_x2_sat r0.rgb,  t0,      v0               \n"	//(diffuse color + ambient color) * diffuse map
						"+mul_sat    r0.a,    t0.a,    v0.a            \n"	//diffuse map alpha * diffuse color alpha
						"mad_sat    r0.rgb,  t1,      v1,      r0      \n"	//specular map * specular color + current

						"lrp        r0.rgb,  t2.a,    t2,      r0      \n" // Add fog
//						"+mul        r0.a,    r0.a,    1-t2.a          \n" // attenuate base tex with fog alpha
						;*/
			strPPCode = "texld		r0,        t0                                 \n"	//base(diffuse) map
						"texld		r1,        t1                                 \n"	//specular map
						"texld		r2,        t2                                 \n"	//fog map
						"mul        r2,      r2,      c7               \n"	// mul fog texture with fog color
						//"mul        t0.a,    t0,      1-t2.a           \n"	// attenuate base tex with fog alpha
						"mul_x2_sat r0.rgb,  r0,      v0               \n"	//(diffuse color + ambient color) * diffuse map
						"+mul_sat    r0.a,    r0.a,    v0.a            \n"	//diffuse map alpha * diffuse color alpha
						"mad_sat    r0.rgb,  r1,      v1,      r0      \n"	//specular map * specular color + current

						"lrp        r0.rgb,  r2.a,    r2,      r0      \n" // Add fog
//						"+mul        r0.a,    r0.a,    1-t2.a          \n" // attenuate base tex with fog alpha
						;
		}
	}

	return;
}
//안태훈 수정 끝	//(Add Tagent-space Normal Map)(0.1)
