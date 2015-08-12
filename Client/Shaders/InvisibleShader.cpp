#include "StdH.h"

SHADER_MAIN(Invisible)
{
}

//안태훈 수정 시작	//(For Performance)(0.1)
SHADER_DESC(Invisible,ShaderDesc *&pshDesc)
{
	static bool bInit = false;
	static ShaderDesc shDescMe;
	if(!bInit)
	{
		bInit = true;
		shDescMe.sd_astrTextureNames.New(0);
		shDescMe.sd_astrTexCoordNames.New(0);
		shDescMe.sd_astrColorNames.New(0);
		shDescMe.sd_astrFloatNames.New(0);
		shDescMe.sd_astrFlagNames.New(0);
		
		shDescMe.sd_strShaderInfo = "Invisible shader";
		
		shDescMe.sd_ulStreamFlags.New(0);
		shDescMe.sd_ctVertexPrograms = 0;
		shDescMe.sd_ctPixelPrograms = 0;
	}
	pshDesc = &shDescMe;
//안태훈 수정 끝	//(For Performance)(0.1)
}

SHADER_VCODE(Invisible, CTString &strVPCode, INDEX iVertexProgram)
{
}

SHADER_PCODE(Invisible, CTString &strPPCode, INDEX iPixelProgram, FOGTYPE eFogType)
{
}
