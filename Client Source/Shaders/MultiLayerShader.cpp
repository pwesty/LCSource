#include "StdH.h"
#include <Shaders/Common.h>

static const INDEX ctTextures = 2;
static const INDEX ctUVMaps   = 2;
static const INDEX ctColors   = 2;
static const INDEX ctFloats   = 2;
static const INDEX ctFlags    = 2;
static const INDEX ctVertexPrograms = 1;
static const INDEX ctPixelPrograms = 1;

static const INDEX iTexture1 = 0;
static const INDEX iUVMap1   = 0;
static const INDEX iColor1   = 0;
static const INDEX iTiling1  = 0;

static const INDEX iTexture2 = 1;
static const INDEX iUVMap2   = 1;
static const INDEX iColor2   = 1;
static const INDEX iTiling2  = 1;

static const INDEX iBaseVP = 0;
static const INDEX iBasePP = 0;

void RenderMultilayer(void)
{
	const COLOR colModelColor = MulColors(shaGetModelColor(),shaGetCurrentColor());
	const BOOL bOpaque = (colModelColor&0xFF)==0xFF;
	const BOOL bHardwareShader = shaUseHWShaders();
	
	if(bHardwareShader) 
	{
		shaRender();
	}
	else 
	{
		const FLOAT fTiling1 = shaGetFloat(iTiling1);
		const FLOAT fTiling2 = shaGetFloat(iTiling2);
		// if layer is tiling 
		if(fTiling1!=1.0f) 
		{
			// calculate new tex coords
			const INDEX ctvx = shaGetVertexCount();
			const GFXTexCoord *ptcOld = shaGetUVMap(iUVMap1);
			GFXTexCoord *ptcNew = shaGetNewTexCoordArray();
			for(INDEX ivx=0;ivx<ctvx;ivx++) 
			{
				ptcNew[ivx].u = ptcOld[ivx].u * fTiling1;
				ptcNew[ivx].v = ptcOld[ivx].v * fTiling1;
			}
			shaSetTexCoordArray(ptcNew);
		}
		// render first layer
		shaRender();
		
		// enable blend
		shaEnableBlend();
		shaBlendFunc(GFX_SRC_ALPHA, GFX_INV_SRC_ALPHA);
		
		shaSetTexture(iTexture2,0);
		shaSetUVMap(iUVMap2);
		shaSetColor(iColor2);
		
		// calculate shading
		shaCalculateLight();
		// prepare fog and haze
		shaPrepareFogAndHaze(bOpaque);
		if(!bOpaque) 
		{
			shaAttenuateAlphaForFogAndHaze();
		}
		
		// if layer is tiling 
		if(fTiling2!=1.0f) 
		{
			// calculate new tex coords
			const INDEX ctvx = shaGetVertexCount();
			const GFXTexCoord *ptcOld = shaGetUVMap(iUVMap2);
			GFXTexCoord *ptcNew = shaGetNewTexCoordArray();
			for(INDEX ivx=0;ivx<ctvx;ivx++) 
			{
				ptcNew[ivx].u = ptcOld[ivx].u * fTiling2;
				ptcNew[ivx].v = ptcOld[ivx].v * fTiling2;
			}
			shaSetTexCoordArray(ptcNew);
		}
		// render second layer
		shaRender();
	}
}

SHADER_MAIN(MultiLayer)
{
	// Set texture wrapping
	shaSetTextureWrapping( GFX_REPEAT, GFX_REPEAT);
	// Set depth test
	shaEnableDepthTest();
	shaDepthFunc(GFX_LESS_EQUAL); 
	// Set alpha test
	shaDisableAlphaTest();
	
	shaSetTexture(iTexture1,0);
	shaSetUVMap(iUVMap1);
	shaSetColor(iColor1);
	
	const COLOR colModelColor = MulColors(shaGetModelColor(),shaGetCurrentColor());
	const BOOL bDoubleSided = shaGetFlags()&BASE_DOUBLE_SIDED;
	const BOOL bOpaque = (colModelColor&0xFF)==0xFF;
	const BOOL bHardwareShader = shaUseHWShaders();
	
	if(bHardwareShader) 
	{
		GFXColor col1,col2;
		col1.MultiplyRGBA((GFXColor)shaGetModelColor(), (GFXColor)shaGetColor(iColor1));
		col2.MultiplyRGBA((GFXColor)shaGetModelColor(), (GFXColor)shaGetColor(iColor2));
		const FLOAT fTiling1 = shaGetFloat(iTiling1);
		const FLOAT fTiling2 = shaGetFloat(iTiling2);
		
		const ShaderRegister srMultyLayer(fTiling1, 1.0f, fTiling2, 1.0f);
		const ShaderRegister srColor1(col1);
		const ShaderRegister srColor2(col2);
		
		shaSetFogTextureUnit(2,bOpaque);
		shaSetVertexProgram(iBaseVP);
		shaSetPixelProgram(iBasePP);
		
		shaSetTextureUnit(1);
		shaSetTextureWrapping( GFX_REPEAT, GFX_REPEAT);
		shaSetTexture(iTexture2,1);
		shaSetUVMap(iUVMap2,1);
		
		shaSetVertexProgramConst(12,&srMultyLayer, 1);
		shaSetPixelProgramConst( 0, &srColor1,     1);
		shaSetPixelProgramConst( 1, &srColor2,     1);
		// prepare fog and haze
		shaPrepareFogAndHaze(bOpaque);
//안태훈 수정 시작	//(Add Tagent-space Normal Map)(0.1)
		shaSetDefaultConstantRegisters();
//안태훈 수정 끝	//(Add Tagent-space Normal Map)(0.1)
	}
	else 
	{
		// enable overbrightning
		shaEnableOverBrightning();
		// calculate shading
		shaCalculateLight();
		// prepare fog and haze
		shaPrepareFogAndHaze(bOpaque);
	}
	
	// if surface is opaque
	if(bOpaque) 
	{
		// disable blend
		shaDisableBlend();
		// enable depth write
		shaEnableDepthWrite();
		// if double sided flag is on
		if(bDoubleSided) 
		{
			// no face culling 
			shaCullFace(GFX_NONE);
		}
		else 
		{
			// back face culling
			shaCullFace(GFX_BACK);
		}
	}
	else 
	{ // else surface is translucent
		// Set blend mode
		shaEnableBlend();
		shaDisableDepthWrite();
		if(bHardwareShader) 
		{
			shaBlendFunc(GFX_ONE, GFX_SRC_ALPHA);
		}
		else 
		{
			// Adjust alpha for fog
			shaBlendFunc(GFX_SRC_ALPHA, GFX_INV_SRC_ALPHA);
			shaAttenuateAlphaForFogAndHaze();
		}
		
		// if double sided
		if(bDoubleSided) 
		{
			// render back side
			shaCullFace(GFX_FRONT);
			RenderMultilayer();
		}
		// back face culling
		shaCullFace(GFX_BACK);
		
	}
	// render front side
	RenderMultilayer();
	
	if(!bHardwareShader) 
	{
		// disable overbrighning
		shaDisableOverBrightning();
		// if surface is opaque
		if(bOpaque) 
		{
			// render fog and haze pass for surface
			shaDoFogAndHazePass();
		}
	}
}

//안태훈 수정 시작	//(For Performance)(0.1)
SHADER_DESC(MultiLayer, ShaderDesc *&pshDesc)
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
		
		shDescMe.sd_astrTextureNames[0] = "Texture 1";
		shDescMe.sd_astrTexCoordNames[0] = "UVMap 1";
		shDescMe.sd_astrColorNames[0] = "Color 1";
		shDescMe.sd_astrFloatNames[0] = "Tiling 1";
		shDescMe.sd_astrTextureNames[1] = "Texture 2";
		shDescMe.sd_astrTexCoordNames[1] = "UVMap 2";
		shDescMe.sd_astrColorNames[1] = "Color 2";
		shDescMe.sd_astrFloatNames[1] = "Tiling 2";
		shDescMe.sd_astrFlagNames[0] = "Double sided";
		shDescMe.sd_astrFlagNames[1] = "Full bright";
		shDescMe.sd_strShaderInfo = "Multi Layer shader";
		
		shDescMe.sd_ulStreamFlags.New(ctVertexPrograms);
		shDescMe.sd_ctVertexPrograms = ctVertexPrograms;
		shDescMe.sd_ctPixelPrograms = ctPixelPrograms;
		shDescMe.sd_ulStreamFlags[0] = GFX_POSITION_STREAM|GFX_TEXCOORD0|GFX_TEXCOORD1|GFX_NORMAL_STREAM;
	}
	pshDesc = &shDescMe;
//안태훈 수정 끝	//(For Performance)(0.1)
}

SHADER_VCODE(MultiLayer, CTString &strVPCode, INDEX iVertexProgram)
{
	ASSERT(iVertexProgram==iBaseVP);
	strVPCode = CALCULATE_SHADING_VP
				STANDARD_OUTPUT_VP
				"mul  oT0.xy, v5.xy, c12.xx  \n" // Calculate detail texcoords
				"mul  oT1.xy, v6.xy, c12.zz  \n" // Calculate detail texcoords
				;
}

// Fc = L0c*L0a*(1-L1) + L1*L1
// Fa = (1-L0)*(1-L1)
SHADER_PCODE(MultiLayer, CTString &strPPCode, INDEX iPixelProgram, FOGTYPE eFogType)
{
	ASSERT(iPixelProgram==iBasePP);
	
	if(eFogType==FT_NONE) 
	{
/*		strPPCode = "tex    t0                     \n" // load texture1
					"tex    t1                     \n" // load texture2
					"mul    t0,     t0,     c0     \n" // mul texture1 with color1
					"mul    t1,     t1,     c1     \n" // mul texture2 with color2
					"mul    r0.rgb, t0,     t0.a   \n" // L0c*L0a
					"lrp    r0.rgb, 1-t1.a, r0, t1 \n" // Fc = L0c*L0a*(1-L1a) + L1c*L1a
					"+mul   r0.a,   1-t0,   1-t1   \n" // Fa = (1-L0a)*(1-L1a)
					"mul_x2 r0.rgb, r0,     v0     \n" // Shade pixel
					;                             */
		strPPCode = "texld	r0,		t0			\n" // load texture1
					"texld  r1,		t1			\n" // load texture2
					"mul    r3,     r0,     c0     \n" // mul texture1 with color1
					"mul    r1,     r1,     c1     \n" // mul texture2 with color2
					"mul    r0.rgb, r3,     r3.a   \n" // L0c*L0a
					"lrp    r0.rgb, 1-r1.a, r0, r1 \n" // Fc = L0c*L0a*(1-L1a) + L1c*L1a
					"+mul   r0.a,   1-r3,   1-r1   \n" // Fa = (1-L0a)*(1-L1a)
					"mul_x2 r0.rgb, r0,     v0     \n" // Shade pixel
					;
	}
	else if(eFogType==FT_OPAQUE) 
	{
/*		strPPCode = "tex    t0                     \n" // load texture1
					"tex    t1                     \n" // load texture2
					"tex    t2                     \n" // load fog texture
					"mul    t0,     t0,     c0     \n" // mul texture1 with color1
					"mul    t1,     t1,     c1     \n" // mul texture2 with color2
					"mul    t2,     t2,     c7     \n" // mul fog texture with fog color
					"mul    r0.rgb, t0,     t0.a   \n" // L0c*L0a
					"lrp    r0.rgb, 1-t1.a, r0, t1 \n" // Fc = L0c*L0a*(1-L1a) + L1c*L1a
					"+mul   r0.a,   1-t0,   1-t1   \n" // Fa = (1-L0a)*(1-L1a)
					"mul_x2 r0.rgb, r0,     v0     \n" // Shade pixel
					"lrp    r0.rgb, t2.a,   t2,  r0\n" // Add fog
					;*/
		strPPCode = "texld	r0,    t0                     \n" // load texture1
					"texld	r1,    t1                     \n" // load texture2
					"texld	r2,    t2                     \n" // load fog texture
					"mul    r3,     r0,     c0     \n" // mul texture1 with color1
					"mul    r1,     r1,     c1     \n" // mul texture2 with color2
					"mul    r2,     r2,     c7     \n" // mul fog texture with fog color
					"mul    r0.rgb, r3,     r3.a   \n" // L0c*L0a
					"lrp    r0.rgb, 1-r1.a, r0, r1 \n" // Fc = L0c*L0a*(1-L1a) + L1c*L1a
					"+mul   r0.a,   1-r3,   1-r1   \n" // Fa = (1-L0a)*(1-L1a)
					"mul_x2 r0.rgb, r0,     v0     \n" // Shade pixel
					"lrp    r0.rgb, r2.a,   r2,  r0\n" // Add fog
					;
	}
	else if(eFogType==FT_NON_OPAQUE) 
	{
/*		strPPCode = "tex    t0                     \n" // load texture1
					"tex    t1                     \n" // load texture2
					"tex    t2                     \n" // load fog texture
					"mul    t0,     t0,     c0     \n" // mul texture1 with color1
					"mul    t1,     t1,     c1     \n" // mul texture2 with color2
					"mul    t2,     t2,     c7     \n" // mul fog texture with fog color
					"mul    t0.a,   t0.a,   1-t2.a \n" // attenuate texture1 alpha with fog alpha
					"mul    r0.rgb, t0,     t0.a   \n" // L0c*L0a
					"+mul   t1.a,   t1.a,   1-t2.a \n" // attenuate texture2 alpha with fog alpha
					"lrp    r0.rgb, 1-t1.a, r0, t1 \n" // Fc = L0c*L0a*(1-L1a) + L1c*L1a
					"+mul   r0.a,   1-t0,   1-t1   \n" // Fa = (1-L0a)*(1-L1a)
					"mul_x2 r0.rgb, r0,     v0     \n" // Shade pixel
					;*/
		strPPCode = "texld	r0,    t0                     \n" // load texture1
					"texld	r1,    t1                     \n" // load texture2
					"texld	r2,    t2                     \n" // load fog texture
					"mul    r3,     r0,     c0     \n" // mul texture1 with color1
					"mul    r1,     r1,     c1     \n" // mul texture2 with color2
					"mul    r2,     r2,     c7     \n" // mul fog texture with fog color
					"mul    r3.a,   r3.a,   1-r2.a \n" // attenuate texture1 alpha with fog alpha
					"mul    r0.rgb, r3,     r3.a   \n" // L0c*L0a
					"+mul   r1.a,   r1.a,   1-t2.a \n" // attenuate texture2 alpha with fog alpha
					"lrp    r0.rgb, 1-r1.a, r0, r1 \n" // Fc = L0c*L0a*(1-L1a) + L1c*L1a
					"+mul   r0.a,   1-r3,   1-r1   \n" // Fa = (1-L0a)*(1-L1a)
					"mul_x2 r0.rgb, r0,     v0     \n" // Shade pixel
					;
	} 
}
