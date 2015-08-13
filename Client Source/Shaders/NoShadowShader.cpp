//안태훈이 새로 추가한 파일임.
//안태훈 수정 시작	//(Add Shadow Util)(0.1)

#include "StdH.h"
#include <Shaders/Common.h>

static const INDEX ctTextures = 0; 
static const INDEX ctUVMaps   = 0;
static const INDEX ctColors   = 1;
static const INDEX ctFloats   = 0;
static const INDEX ctFlags    = 0;
static const INDEX ctVertexPrograms = 1;
static const INDEX ctPixelPrograms = 1;

static const INDEX iShadowColor	= 0;
static const INDEX iShadowVP	= 0;
static const INDEX iShadowPP	= 0;

static void SetRenderingState()
{
	// Set depth write
	shaDisableDepthWrite();
	// Set blend mode
	shaDisableBlend();
	// Set alpha test
	//shaEnableAlphaTest();
}

static void VertexShaderProcess()
{
	shaSetVertexProgramConst_TransformMatrix();	//c0~c3
	shaSetVertexProgramConst_Multiply();		//c7
	shaSetVertexProgramConst_LightDir();		//c4
	shaSetVertexProgramConst_BoneMatrix();		//c21~
	shaSetVertexProgram(iShadowVP);
}

static void PixelShaderProcess(COLOR colShadowColor)
{
	//Shadow color setting
	GFXColor colShadow = colShadowColor;
	ShaderRegister srShadowColor(colShadow);
	shaSetPixelProgramConst( 0, &srShadowColor, 1);
//강동민 수정 시작		03.22
	GFXColor colPlane = 0xFFFFFF80;
	ShaderRegister srPlaneColor(colPlane);
	shaSetPixelProgramConst( 1, &srPlaneColor, 1);	// c1
//강동민 수정 끝		03.22
	//set pixel shader
	shaSetPixelProgram(iShadowPP);
}

static void Render()
{
	// if double sided flag is on
	shaCullFace(GFX_NONE);	// no face culling
	// Render surface
	shaRender();
}

// 텍스쳐 없이 색상만 칠함.
SHADER_MAIN(NoShadow)
{
	const COLOR colShadowColor = shaGetColor(iShadowColor);
	const BOOL bHardwareShader = shaUseHWShaders();
//강동민 수정 시작		03.30	
	if(((shaGetModelColor()&CT_AMASK)>>CT_ASHIFT) >= 64)
	{
//강동민 수정 끝		03.30
		
		if(bHardwareShader)
		{
			SetRenderingState();
			VertexShaderProcess();
			//강동민 수정 시작
			PixelShaderProcess(colShadowColor);
			//강동민 수정 끝
			Render();
		}
		else	//USE VS(SOFT or HARD), NON-PS
		{
			//TODO : 안태훈
		}
	}
}

//안태훈 수정 시작	//(For Performance)(0.1)
SHADER_DESC(NoShadow, ShaderDesc *&pshDesc)
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

		shDescMe.sd_astrColorNames[0] = "Mat.Shadow";

		shDescMe.sd_strShaderInfo = "NoShadow shader";
		shDescMe.sd_ctVertexPrograms = ctVertexPrograms;
		shDescMe.sd_ctPixelPrograms = ctPixelPrograms;
		shDescMe.sd_ulStreamFlags.New(ctVertexPrograms);
		shDescMe.sd_ulStreamFlags[0] = GFX_POSITION_STREAM;
	}
	pshDesc = &shDescMe;
//안태훈 수정 끝	//(For Performance)(0.1)
}

SHADER_VCODE(NoShadow, CTString &strVPCode, INDEX iVertexProgram)
{
	//상수등록 c0 ~ c20 사용가능, c0 ~ c7은 외부에서 입력됨.
	//--------------- Input ---------------------//
	//--- r0     - vertex position            ---//
	//--- c0     - mProjection*mView          ---//
	//--- c7     - (0.0f, 1.0f, 2.0f, 0.5f)   ---//여기까지가 외부에서 세팅
	//--- c4.w   - 765.01f                    ---//
	//-------------- Output ---------------------//
	//--- oPos   - Position Output            ---//

	ASSERT(iVertexProgram == iShadowVP);
	if(iVertexProgram == iShadowVP)
	{
		strVPCode = "m4x4 oPos,   r0,       c0                  \n"
					;
	}
}

SHADER_PCODE(NoShadow, CTString &strPPCode, INDEX iPixelProgram, FOGTYPE eFogType)
{
	//상수등록 c0만 가능(PS가 없는 카드와의 호환성을 고려할 경우)
	//--------------- Input ---------------------//
	//--- v0     - diffuse color              ---//
	//--- c0     - Shadow color              ---//
	//-------------- Output ---------------------//
	//--- r0     - Output Pixel COLOR & Alpha ---//

	ASSERT(iPixelProgram == iShadowPP);
	//안개 고려하지 않음.
	if(iPixelProgram == iShadowPP)
	{
		strPPCode = "mov        r0,      c0                        \n"	//paint shadow color
//강동민 수정 시작		03.22
					"mul		r0, r0,  c1							\n"
//강동민 수정 끝		03.22
					;
	}
	return;
}
//안태훈 수정 끝	//(Add Shadow Util)(0.1)
