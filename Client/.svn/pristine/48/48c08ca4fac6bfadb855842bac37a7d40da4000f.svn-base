#include "stdh.h"
#include <Engine/Graphics/Shader.h>
#include <Engine/Terrain/Terrain.hpp>

static const INDEX ctVertexPrograms = 1;
static const INDEX ctPixelPrograms = 1;
static const INDEX iBasePP = 0;
static const INDEX iBaseVP = 0;

SHADER_MAIN(TRShader_2TRL)
{
//안태훈 수정 시작	//(Add Tagent-space Normal Map)(0.1)
	shaSetDefaultConstantRegisters();
//안태훈 수정 끝	//(Add Tagent-space Normal Map)(0.1)
	shaRender();
}

//안태훈 수정 시작	//(For Performance)(0.1)
SHADER_DESC(TRShader_2TRL,ShaderDesc *&pshDesc)
{
	static bool bInit = false;
	static ShaderDesc shDescMe;
	if(!bInit)
	{
		bInit = true;
		shDescMe.sd_ctVertexPrograms = ctVertexPrograms;
		shDescMe.sd_ctPixelPrograms = ctPixelPrograms;

		shDescMe.sd_ulStreamFlags.New(ctVertexPrograms);
		shDescMe.sd_ulStreamFlags[0] = SHA_POSITION_STREAM|SHA_NAKED_CODE;
	}
	pshDesc = &shDescMe;
//안태훈 수정 끝	//(For Performance)(0.1)
}

SHADER_VCODE(TRShader_2TRL, CTString &strVPCode, INDEX iVertexProgram)
{
	ASSERT(iVertexProgram==iBaseVP);
	strVPCode = "vs.1.1                        \n"
				"m4x4 oPos,   v0,   c0         \n"
				"mov  r0.xy,  v0.xz            \n"
				"mov  r0.zw,  c7.xy            \n"
#if TER_SHADER_OPT
				"dp4  oT0.x,  r0,   c21        \n"
				"dp4  oT0.y,  r0,   c22        \n"
				"dp4  oT1.x,  r0,   c24        \n"
				"dp4  oT1.y,  r0,   c25        \n"
				"dp4  oT2.x,  r0,   c27        \n"
				"dp4  oT2.y,  r0,   c28        \n"
				"dp4  oT3.x,  r0,   c30        \n"
				"dp4  oT3.y,  r0,   c31        \n"
#else
				"m4x3 r1,   r0, c21            \n"
				"m4x3 r2,   r0, c24            \n"
				"m4x3 r3,   r0, c27            \n"
				"m4x3 r4,   r0, c30            \n"
				"mov  oT0.xy,  r1              \n"
				"mov  oT1.xy,  r2              \n"
				"mov  oT2.xy,  r3              \n"
				"mov  oT3.xy,  r4              \n"
#endif
				;
}

SHADER_PCODE(TRShader_2TRL, CTString &strPPCode, INDEX iPixelProgram, FOGTYPE eFogType)
{
	ASSERT(iPixelProgram==iBasePP);
	ASSERT(eFogType==FT_NONE);
/*	strPPCode = "tex    t0                              \n" // load layer 0 texture
				"tex    t1                              \n" // load layer 0 mask
				"tex    t2                              \n" // load layer 1 texture
				"tex    t3                              \n" // load layer 1 mask
				
				"mul    r0.a,    t0.a,   t1.a           \n" // multiply layer 0 texture with its alpha
				"mul    r1.rgb,  t0,     r0.a           \n" //  and alpha of layer 0 mask texture

				"+mul   r1.a,    t2.a,   t3.a           \n" // multiply layer 1 texture with its alpha
				"lrp    r0.rgb,  1-r1.a, r1,    t2      \n" //  and alpha of layer 1 mask texture

				"+mul   r0.a,    1-r0.a, 1-r1.a         \n" // output layer alpha
				;*/
	strPPCode = "texld    r0,    t0                     \n" // load layer 0 texture
				"texld    r1,    t1                     \n" // load layer 0 mask
				"texld    r2,    t2                     \n" // load layer 1 texture
				"texld    r3,    t3                     \n" // load layer 1 mask
				
				"mul    r0.a,    r0.a,   r1.a           \n" // multiply layer 0 texture with its alpha
				"mul    r1.rgb,  r0,     r0.a           \n" //  and alpha of layer 0 mask texture

				"+mul   r1.a,    r2.a,   r3.a           \n" // multiply layer 1 texture with its alpha
				"lrp    r0.rgb,  1-r1.a, r1,    r2      \n" //  and alpha of layer 1 mask texture

				"+mul   r0.a,    1-r0.a, 1-r1.a         \n" // output layer alpha
				;
}
