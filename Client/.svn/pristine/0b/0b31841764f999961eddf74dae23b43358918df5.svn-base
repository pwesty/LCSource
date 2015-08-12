//강동민 수정 시작 Water 구현		04.22
#include "StdH.h"

#include <Engine/Math/Functions.h>
#include <Engine/Base/Types.h>
#include <Engine/Graphics/Color.h>
#include "CNiceWater.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNiceWater::CNiceWater() 
: m_pWaterEntity(NULL)
{	
	m_bWave				= FALSE;
	m_bReflection		= FALSE;
	m_bRenderSky		= TRUE;
	m_bRenderModels		= TRUE;
	m_bWireFrame		= FALSE;

	m_fBumpMat11		= 0.0f;
	m_fBumpMat12		= 0.0f;
	m_fBumpMat21		= 0.0f;
	m_fBumpMat22		= 0.0f;

	m_fTexFlowX			= 0.0f;
	m_fTexFlowY			= 0.0f;	
}

CNiceWater::CNiceWater(CEntity *pEntity)
: m_pWaterEntity(pEntity)
{
	m_bWave				= FALSE;
	m_bReflection		= FALSE;
	m_bRenderSky		= TRUE;
	m_bRenderModels		= TRUE;
	m_bWireFrame		= FALSE;

	m_fBumpMat11		= 0.0f;
	m_fBumpMat12		= 0.0f;
	m_fBumpMat21		= 0.0f;
	m_fBumpMat22		= 0.0f;

	m_fTexFlowX			= 0.0f;
	m_fTexFlowY			= 0.0f;	
}
	
CNiceWater::~CNiceWater()
{
	//Clear();
}

void CNiceWater::Render()
{
	// NOTE : SKA 모델이라서 따로 렌더링할 필요가 없을듯...
}

void CNiceWater::Clear()
{
	m_pWaterEntity = NULL;
	m_toBump.SetData(NULL);
	m_toWater.SetData(NULL);
}
//강동민 수정 끝 Water 구현			04.22
