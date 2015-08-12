#include "stdh.h"
#include <Engine/Graphics/Shader.h>
#include <Engine/Terrain/Terrain.hpp>

static const INDEX ctVertexPrograms = 2;
static const INDEX ctPixelPrograms  = 1;
static const INDEX iBaseVP = 0;
static const INDEX iBasePP = 0;
static const INDEX iTopMapVP = 1;

SHADER_MAIN(TRShader_Base)
{
//안태훈 수정 시작	//(Add Tagent-space Normal Map)(0.1)
	shaSetDefaultConstantRegisters();
//안태훈 수정 끝	//(Add Tagent-space Normal Map)(0.1)
	shaRender();
}

//안태훈 수정 시작	//(For Performance)(0.1)
SHADER_DESC(TRShader_Base,ShaderDesc *&pshDesc)
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
		shDescMe.sd_ulStreamFlags[1] = SHA_POSITION_STREAM|SHA_NAKED_CODE;
	}
	pshDesc = &shDescMe;
//안태훈 수정 끝	//(For Performance)(0.1)
}

SHADER_VCODE(TRShader_Base, CTString &strVPCode, INDEX iVertexProgram)
{
	if(iVertexProgram==iBaseVP) {
		strVPCode = "vs.1.1                       \n"
					"m4x4 oPos,   v0,   c0        \n"
					"mov  r0.xy,  v0.xz           \n"
					"mov  r0.zw,  c7.xy           \n"
#if TER_SHADER_OPT
					"dp4  oT0.x,  r0,   c21       \n"
					"dp4  oT0.y,  r0,   c22       \n"
#else
					"m4x3 r1,     r0,   c21       \n"
					"mov  oT0.xy, r1              \n"
#endif
					;
	} else if(iVertexProgram==iTopMapVP) {
		strVPCode = "vs.1.1                       \n"
					"m4x4 oPos,   v0,   c0        \n"
					"mov  oT0.xy, v0.xz           \n"
					;
	} else {
		ASSERTALWAYS("Invalid vertex program index");
	}
}

SHADER_PCODE(TRShader_Base, CTString &strPPCode, INDEX iPixelProgram, FOGTYPE eFogType)
{
	ASSERT(eFogType==FT_NONE);
	ASSERT(iPixelProgram==iBasePP);

/*	strPPCode = "tex    t0                     \n"
				"mov    r0,      t0            \n"
				;*/
	strPPCode = "texld	r0,    t0                     \n"
				;
}
