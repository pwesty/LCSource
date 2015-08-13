#include "stdh.h"
#include <Engine/Graphics/Shader.h>
#include <Engine/Terrain/Terrain.hpp>

static const INDEX ctVertexPrograms = 1;
static const INDEX ctPixelPrograms = 1;
static const INDEX iBasePP = 0;
static const INDEX iBaseVP = 0;

SHADER_MAIN(TRShader_1TRL)
{
//안태훈 수정 시작	//(Add Tagent-space Normal Map)(0.1)
	shaSetDefaultConstantRegisters();
//안태훈 수정 끝	//(Add Tagent-space Normal Map)(0.1)
	shaRender();
}

//안태훈 수정 시작	//(For Performance)(0.1)
SHADER_DESC(TRShader_1TRL,ShaderDesc *&pshDesc)
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

SHADER_VCODE(TRShader_1TRL, CTString &strVPCode, INDEX iVertexProgram)
{
	ASSERT(iVertexProgram==iBaseVP);
	strVPCode = "vs.1.1                       \n"
				"m4x4 oPos,   v0,   c0        \n"
				"mov  r0.xy,  v0.xz           \n"
				"mov  r0.zw,  c7.xy           \n"
#if TER_SHADER_OPT
				"dp4  oT0.x,  r0,   c21       \n"
				"dp4  oT0.y,  r0,   c22       \n"
				"dp4  oT1.x,  r0,   c24       \n"
				"dp4  oT1.y,  r0,   c25       \n"
#else
				"m4x3 r1,     r0,   c21       \n"
				"m4x3 r2,     r0,   c24       \n"
				"mov  oT0.xy, r1              \n"
				"mov  oT1.xy, r2              \n"
#endif
				;
}

#if 0
// SW
F  = Fc*Fa + D*1-Fa
// HW
F  = Fc*1.0f + D*Fa
Fc = Lc0 * (La0 * Ma0)
Fa = 1-(La0 * Ma0);


#endif

SHADER_PCODE(TRShader_1TRL, CTString &strPPCode, INDEX iPixelProgram, FOGTYPE eFogType)
{
	ASSERT(iPixelProgram==iBasePP);
	ASSERT(eFogType==FT_NONE);

/*	strPPCode = "tex    t0                     \n" // load layer 0 texture
				"tex    t1                     \n" // load layer 0 mask
				
				"mul    t1.a,    t0.a,   t1.a  \n" // multiply layer 0 texture with its alpha
				"mul    r0.rgb,  t0,     t1.a  \n" //  and alpha of layer 0 mask texture

				"+mov   r0.a,    1-t1.a        \n" // output layer alpha
				;*/
	strPPCode = "texld	r0,    t0                     \n" // load layer 0 texture
				"texld	r1,    t1                     \n" // load layer 0 mask
				
				"mul    r1.a,    r0.a,   r1.a  \n" // multiply layer 0 texture with its alpha
				"mul    r0.rgb,  r0,     r1.a  \n" //  and alpha of layer 0 mask texture

				"+mov   r0.a,    1-r1.a        \n" // output layer alpha
				;
}

