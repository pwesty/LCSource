#include "StdH.h"
//강동민 수정 시작 Water 구현		04.13
#include <Shaders/Common.h>

static const INDEX ctTextures		= 3; 
//강동민 수정 시작 Water 구현		04.21
//static const INDEX ctTextures		= 3; 
//강동민 수정 끝 Water 구현			04.21
static const INDEX ctUVMaps			= 1;
static const INDEX ctColors			= 1;
static const INDEX ctFloats			= 0;
static const INDEX ctFlags			= 0;
static const INDEX iBumpTexture		= 0;		// Bump Texture
//강동민 수정 시작 Water 구현		04.21
//static const INDEX iBumpTexture2		= 1;	// Bump Texture2
static const INDEX iBaseTexture		= 1;		// Base Texture
static const INDEX iWaterTexture	= 2;		// Water Texture
static const INDEX iBaseUVMap		= 0;

static const INDEX ctVertexPrograms = 1;
static const INDEX ctPixelPrograms	= 1;

static const INDEX iWaterHolderVP		= 0;
static const INDEX iWaterHolderPP		= 0;

static void SetRenderingState()
{
	// Set depth write
	//shaDisableDepthWrite();
	//shaEnableDepthTest();
	// Set blend mode
	shaDisableBlend();
	// Set alpha test
	shaDisableAlphaTest();
//강동민 수정 시작 Water 구현		04.21
	shaSetTextureWrapping( GFX_REPEAT, GFX_REPEAT, 0);			// 원본.	
	shaSetTextureWrapping( GFX_CLAMP, GFX_CLAMP, 1);
//강동민 수정 끝 Water 구현			04.21
	
//강동민 수정 시작 Water 구현		04.13
	//CTimerValue tvNow   = _pTimer->GetHighPrecisionTimer();
	//FLOAT r = 0.01f;

//강동민 수정 끝 Water 구현			04.13

//강동민 수정 시작 Water 구현		04.26
	// 원본.
	//_pGfx->gl_pd3dDevice->SetTextureStageState( 1, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT4|D3DTTFF_PROJECTED );
	_pGfx->gl_pd3dDevice->SetTextureStageState( 1, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_PROJECTED );
//강동민 수정 끝 Water 구현			04.26
}

static void InitShaderProcess()
{
	// NOTE : 애니메이션 텍스쳐가 적용될수 있도록 해야한다.
	// NOTE : 기존의 물 텍스쳐를 입힐 경우를 고려해서...
	//사용할 Texture지정
	shaSetTexture(iBumpTexture, 0);		// Bump Texture1
	//shaSetTexture(iBumpTexture2, 1);	// Bump Texture2
	shaSetTexture(iBaseTexture, 1);		// Reflection Texture
//강동민 수정 시작 접속 시퀀스 작업	05.31
	//	shaSetTexture(iWaterTexture, 2);	// Water Texture
//강동민 수정 끝 접속 시퀀스 작업	05.31
	//사용할 UV좌표셋
	shaSetUVMap(iBaseUVMap);
}

static void VertexShaderProcess(INDEX iVertexProgram)
{
	shaSetVertexProgram(iVertexProgram);

	// 광원의 위치와 카메라의 위치는 외부에서 설정해줄것...	
	shaSetVertexProgramConst_TransformMatrix();	//c0~c3
	shaSetVertexProgramConst_ProjectionMatrix();//c20~c23
	shaSetVertexProgramConst_Multiply();		//c7

//강동민 수정 시작 Water 구현		04.20
	// 범프매핑을 위한 설정.
	shaSetEMBM();
//강동민 수정 끝 Water 구현			04.20
	shaSetVertexProgramConst_WaterInformation();	
}

static void PixelShaderProcess(INDEX iPixelProgram)
{
	shaSetPixelProgramConst_WaterColor();
	shaSetPixelProgram(iPixelProgram);
}

static void Render()
{	 
	//_pGfx->gl_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_BUMPENVMAP );
	// Render surface
	shaRender();
}

SHADER_MAIN(WaterHolder)
{
	BOOL bHardwareShader = shaUseHWShaders();
	if(bHardwareShader)
	{
		SetRenderingState();
		InitShaderProcess();
		
		// NOTE : 지형부분에 Fog나 Haze가 적용되어 있으면, 제대로 투명하게 나오지 않음.
//강동민 수정 시작 접속 시퀀스 작업	05.23
		shaEnableBlend();
//강동민 수정 끝 접속 시퀀스 작업	05.23
		shaEnableDepthWrite();
		shaBlendFunc(
			GFX_SRC_ALPHA,
			GFX_INV_SRC_ALPHA);			
		// if double sided flag is on
		//shaCullFace(GFX_NONE);	// no face culling
		VertexShaderProcess(iWaterHolderVP);
		PixelShaderProcess(iWaterHolderPP);
		Render();
		_pGfx->gl_pd3dDevice->SetTextureStageState( 1, D3DTSS_TEXTURETRANSFORMFLAGS, 0);		
		shaDisableBlend();
	}
	else	//USE VS(SOFT or HARD), NON-PS
	{
		// 아래 코드 확인 안해봤음.
//강동민 수정 시작 접속 시퀀스 작업	05.31
		SetRenderingState();
		//사용할 Texture지정				
		shaSetTexture(iWaterTexture, 0);	// Water Texture
		//사용할 UV좌표셋
		shaSetUVMap(iBaseUVMap);		
//강동민 수정 끝 접속 시퀀스 작업	05.31
		Render();
	}
}

//안태훈 수정 시작	//(For Performance)(0.1)
SHADER_DESC(WaterHolder, ShaderDesc *&pshDesc)
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

		shDescMe.sd_astrTextureNames[0] = "Trans. map";
		shDescMe.sd_astrTexCoordNames[0] = "Base uvmap";
		shDescMe.sd_astrColorNames[0] = "Mat.Water";
		shDescMe.sd_strShaderInfo = "Water shader";

		shDescMe.sd_ctVertexPrograms = ctVertexPrograms;
		shDescMe.sd_ctPixelPrograms = ctPixelPrograms;
		shDescMe.sd_ulStreamFlags.New(ctVertexPrograms);
		shDescMe.sd_ulStreamFlags[0] = GFX_POSITION_STREAM | GFX_TEXCOORD0;
		shDescMe.PixelShaderVersion = D3DPS_VERSION(1,1); /// WaterHolder는 1.4에서 사용할 수 없는 texbem명령으로 인해 1.1을 사용.
			// 범프 맵 텍스처의 사용 방식을 바꾸는게 어렵기 때문에, 1.1사용
			// 추후 Water 구현에 대해 수정이 필요할 것으로 보임.
	}
	pshDesc = &shDescMe;
//안태훈 수정 끝	//(For Performance)(0.1)
}

SHADER_VCODE(WaterHolder, CTString &strVPCode, INDEX iVertexProgram)
{
	if(iVertexProgram == iWaterHolderVP)
	{		
		strVPCode =
			// 정점을 변환해서 물결처럼 흔들리도록 하는 부분.
/*
			"mul r0, c15, v5.x							\n"	// 텍스쳐 좌표들을 주기적 사인파의 입력으로 사용.
			"mad r0, c16, v5.y, r0						\n"	// 텍스쳐 좌표들을 주기적 사인파의 입력으로 사용.
			
			"mov r1, c17.x								\n"	// 시간.
			"mad r0, r1, c14, r0						\n"	// 비례된 시간을 더해서 주파수에 따라 굴곡들을 이동.
			"add r0, r0, c13							\n"
			
			"frc r0.xy, r0								\n"	// 네 성분들의 소수부들을 취한다.
			"frc r1.xy, r0.zwzw							\n"
			"mov r0.zw, r1.xyxy							\n"

			"mul r0, r0, c11.x							\n"	// 보정 계수를 곱한다.
			"sub r0, r0, c7.w							\n"	// 0.5를 뺀다.
			"mul r0, r0, c20.w							\n"	// 텍스쳐 좌표들에 2*PI를 곱한다.
															// 이러면 텍스쳐 좌표들의 범위는 (-PI에서 PI)가 된다.

			"mul r5, r0, r0								\n"	// 사인과 코사인의 테일러 급수 전개를 계산한다.
			"mul r1, r5, r0								\n"
			"mul r6, r1, r0								\n"
			"mul r2, r6, r0								\n"
			"mul r7, r2, r0								\n"
			"mul r3, r7, r0								\n"
			"mul r8, r3, r0								\n"

			"mad r4, r1, c8.y, r0						\n"		// Sine
			"mad r4, r2, c8.z, r4						\n"
			"mad r4, r3, c8.w, r4						\n"

			"mov r0, c7.y								\n"		// 1
			"mad r5, r5, c9.x, r0						\n"		// Cosine
			"mad r5, r6, c9.y, r5						\n"
			"mad r5, r7, c9.z, r5						\n"
			"mad r5, r8, c9.w, r5						\n"

			"dp4 r0, r4, c12							\n"		// 파들에 파고 상수들을 곱한다.

			"mul r0.xyz, c7.xxyx, r0					\n"		// 이 정점에서의 파 크기에 법선을 곱한다.
			"add r0.xyz, r0, v0							\n"		// 위치에 더한다.
			"mov r0.w, c7.y								\n"		// 동차항
			"m4x4 oPos,   r0,       c0                  \n"		// 최종 위치 = 객체 공간 위치 * 세계:뷰:투영 행렬
			*/
			
			//------------------------------------------------
			// 두 장의 범프맵을 가로질러 흐르게 하는 부분.
			// c17 -> 시간
			// c18 -> 텍스쳐 흐르게 하는 값들.
			// v5 -> 텍스쳐 좌표.
			// c0 -> 정점 변환 행렬.
			// c21 -> View * Proj * Tex Matrix
			/*"mov r0, c17.x								\n"
			"mul r0, r0, c18.xyxy						\n"
			"frc r0.xy, r0								\n"
			"add r0, v5, r0								\n"
			"mov oT0, r0								\n"		// 범프맵 좌표 1
			*/
			"mov r0, v5									\n"
			"add r0, r0, c17.yz							\n"
			"mov oT0, r0								\n"		// 범프맵 좌표 1

/*
			"mov r0, c17.x								\n"
			"mul r0, r0, c18.zwzw						\n"
			"frc r0.xy, r0								\n"
			"add r0, v5, r0								\n"
			"mov oT1, r0.yxzw							\n"		// 범프맵 좌표 2
			*/
			
			"m4x4 oPos,   v0,       c0                  \n"		// 최종 위치 = 객체 공간 위치 * 세계:뷰:투영 행렬			
			
			// 아래에서 구해지는 4차원 값을 2차원으로 투영해야 함.
			// 
			"m4x4 oT1, v0, c21							\n"
								;
	}
}

SHADER_PCODE(WaterHolder, CTString &strPPCode, INDEX iPixelProgram, FOGTYPE eFogType)
{  // pixel shader 1.4사용 안함
	if(iPixelProgram == iWaterHolderPP)
	{
		// c2 -> WaterColor
		strPPCode =	"tex				t0		\n"
					"texbem t1,			t0		\n"		// 범프 매핑.
					"mov	r0.rgb,			t1	\n"
					"+mov   r0.a,    c3.a		\n"
					//"mov	r0,			t1		\n"
					//"add r0, r0, c3				\n"
					// calculate ocean color
					//lrp r1, v0, c[ CP_OCEAN_COLOR_DARK ], c[ CP_OCEAN_COLOR_LIGHT ]

					// combine ocean and env color
					//add r0, r0, r1
								;
	}
}
//강동민 수정 끝 Water 구현		04.13