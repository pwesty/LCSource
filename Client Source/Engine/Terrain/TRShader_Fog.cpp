#include "stdh.h"
#include <Engine/Graphics/Shader.h>
#include <Engine/Terrain/Terrain.hpp>

static const INDEX ctVertexPrograms = 1;
static const INDEX ctPixelPrograms  = 1;
static const INDEX iBaseVP = 0;
static const INDEX iBasePP = 0;

SHADER_MAIN(TRShader_Fog)
{
//안태훈 수정 시작	//(Add Tagent-space Normal Map)(0.1)
	shaSetDefaultConstantRegisters();
//안태훈 수정 끝	//(Add Tagent-space Normal Map)(0.1)
	shaRender();
}

//안태훈 수정 시작	//(For Performance)(0.1)
SHADER_DESC(TRShader_Fog,ShaderDesc *&pshDesc)
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

SHADER_VCODE(TRShader_Fog, CTString &strVPCode, INDEX iVertexProgram)
{
	ASSERT(iVertexProgram==iBaseVP);
	strVPCode = "vs.1.1                         \n"
				"m4x4 oPos,   v0,   c0          \n"

				"dp3  r10.w,  v0.xyz,  c18.xyz  \n" // const FLOAT fD = vtx * _vFViewerObj;
				"dp3  r11.w,  v0.xyz,  c19.xyz  \n" // const FLOAT fH = vtx * _vHDirObj
				"add  r9.w,   r10.w,   c20.x    \n" // (fD+_fFogAddZ)
				"add  r8.w,   r11.w,   c20.y    \n" // (fH+_fFogAddH)
				"mul  oT0.x,  r9.w,    c20.z    \n" // tex.s = (fD+_fFogAddZ) * _fog_fMulZ;
				"mul  oT0.y,  r8.w,    c20.w    \n" // tex.t = (fH+_fFogAddH) * _fog_fMulH;

				;
}

SHADER_PCODE(TRShader_Fog, CTString &strPPCode, INDEX iPixelProgram, FOGTYPE eFogType)
{
	ASSERT(eFogType==FT_NONE);
	ASSERT(iPixelProgram==iBasePP);

/*	strPPCode = "tex    t0                     \n"
				"mul    t0,     t0,    c7      \n" // mul fog texture with fog color
				"mov    r0,     t0             \n"
				;*/
	strPPCode = "texld	r0,    t0              \n"
				"mul    r0,     r0,    c7      \n" // mul fog texture with fog color
				;
}
