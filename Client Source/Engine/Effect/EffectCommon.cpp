//안태훈 수정 시작	//(Add & Modify SSSE Effect)(0.1)
#include "stdH.h"
#include "EffectCommon.h"

#include "CSkaEffect.h"
#include "CMdlEffect.h"
#include "CParticleEffect.h"
#include "CEffectManager.h"
#include "CEffectGroupManager.h"
#include "CParticleGroupManager.h"
#include "CSkaTag.h"
#include "CWorldTag.h"
#include "CEntityTag.h"
#include "CTagManager.h"
#include "CLightEffect.h"
#include "CSoundEffect.h"
#include "CTraceEffect.h"
#include "CTerrainEffect.h"
#include "CSplineBillboardEffect.h"
#include "COrbitEffect.h"
#include "CShockWaveEffect.h"
#include "CSplinePathEffect.h"
#include "CCameraEffect.h"
#include "CEntityEffect.h"
#include <Engine/Ska/ModelInstance.h>
#include <Engine/Graphics/Texture.h>
#include <Engine/Base/Console.h>
#include <Engine/Base/Shell.h>
#include <vector>
#include <string>
#include <utility>
#include <Engine/Base/Memory.h>

CTextureObject g_toLightMap;
CTerrain *g_pTerrain = NULL;
CWorld *g_pWorld = NULL;

/*static const char *szEffectDataFileList[] = {
		"Data\\Effect\\Effect.dat"
		, "Data\\Effect\\Effect_SND.dat"
		, "Data\\Effect\\ItemEffect.dat"
		, "Data\\Effect\\Rogue.dat"
		, "Data\\Effect\\BGEffect.dat"
	};*/

BOOL g_bFirstInit = TRUE;

#define LIGHT_FILENAME "Data\\Effect\\TEXTURE\\tr_light.tex"

//effect 유효 처리 거리
FLOAT g_fEffectDistance = 100.0f;

struct ReservedEG
{
	std::string	m_strName;
	CTagManager	*m_pTagManager;
	FLOAT		m_fStartTime;
};
typedef std::vector<ReservedEG> ReservedEGVector;
ReservedEGVector g_vectorReservedEG;

extern BOOL RM_AreHardwareShadersAvailable(void);

extern void FinalizeShaders(void)
{
	CSplineBillboardEffect::FinalizeShaders();
	CTraceEffect::FinalizeShaders();
	CShockWaveEffect::FinalizeShaders();
}

extern void InitializeShaders(void)
{
	CTraceEffect::InitializeShaders();
	CSplineBillboardEffect::InitializeShaders();
	CShockWaveEffect::InitializeShaders();
}

//하드코딩된 이펙트 그룹 생성을 한다.
extern BOOL Initialize_EffectSystem(const CTFileName *fnm)
{
	if(g_bFirstInit)
	{
		//필요한 Symbol Declare
		_pShell->DeclareSymbol("user FLOAT g_fEffectDistance;", &g_fEffectDistance);
	}

	//Light Effect용 Texturedata 로드
	g_toLightMap.SetData_t(CTFILENAME(LIGHT_FILENAME));
	((CTextureData*)g_toLightMap.GetData())->Force(TEX_STATIC|TEX_CONSTANT);

	if(RM_AreHardwareShadersAvailable())
	{
		CTraceEffect::InitializeShaders();
		CSplineBillboardEffect::InitializeShaders();
		CShockWaveEffect::InitializeShaders();
	}

	if(g_bFirstInit)
	{
		Reset_EffectSystem();
		try
		{
			//INDEX cntEffectData = sizeof(szEffectDataFileList) / sizeof(char*);
			//for(INDEX i=0; i<cntEffectData; ++i)
			//{
			if (fnm == NULL)
				Open_EffectSystem("Data\\Effect\\Effect.dat");
			else
				Open_EffectSystem((*fnm));
			//}
		}
		catch(const char *szError)
		{
			ASSERTALWAYS("Something wrong!");
			CPrintF(szError);
			Reset_EffectSystem();
		}
		CEffectGroupManager::Instance().ClearCreated();
	}

	g_vectorReservedEG.clear();

	return TRUE;
}

extern BOOL Finalize_EffectSystem()
{
	CSplineBillboardEffect::FinalizeShaders();
	CTraceEffect::FinalizeShaders();
	CShockWaveEffect::FinalizeShaders();

	Reset_EffectSystem();

	g_toLightMap.SetData(NULL);

	return TRUE;
}

extern CCameraEffect::CCameraValue g_cvCameraShake;
extern BOOL PrepareRender_EffectSystem(CTerrain *pTerrain, CWorld *pWorld)
{
	//ASSERT(pTerrain != NULL && pWorld != NULL);
	//ASSERT(g_toLightMap.GetData() != NULL);
	g_pTerrain = pTerrain;
	g_pWorld = pWorld;
	CLightEffect::SetLightTexture((CTextureData*)g_toLightMap.GetData());
	CLightEffect::SetTerrain(pTerrain);
	CLightEffect::SetWorld(pWorld);
	CTerrainEffect::SetTerrain(pTerrain);
	g_cvCameraShake.Reset();
	return TRUE;
}

//이펙트 리셋
extern void Reset_EffectSystem()
{
	CEffectGroupManager::Instance().Clear();
	CEffectManager::Instance().Clear();
	CParticleGroupManager::Instance().Clear();
}

#include <Engine/Base/Stream.h>

//이펙트 파일 로딩
extern BOOL Open_EffectSystem(const CTFileName &fnm)
{
	CTFileName fullPathName;
	ExpandFilePath(EFP_FILE, fnm, fullPathName);
	if(0xFFFFFFFF == GetFileAttributes(fullPathName.str_String)) return FALSE;
	CTFileStream fs;

	fs.Open_t(fnm);

	//effect begin
	fs.ExpectID_t("EFTB");

	//particle group mager
	fs.ExpectID_t("EPGM");
	CParticleGroupManager::Instance().Read(&fs);

	//effect mager
	fs.ExpectID_t("EEFM");
	CEffectManager::Instance().Read(&fs);

	//effect group mager
	fs.ExpectID_t("EEGM");
	CEffectGroupManager::Instance().Read(&fs);

	//effect end
	fs.ExpectID_t("EFTE");

	return TRUE;
}

//이펙트 파일 저장
extern BOOL Save_EffectSystem(const CTFileName &fnm)
{
	CTFileStream fs;

	fs.Create_t(fnm, CTStream::CM_BINARY);

//	CPrintF("%s\n", LIGHT_FILENAME);

	//effect begin
	fs.WriteID_t("EFTB");

	//particle group mager
	fs.WriteID_t("EPGM");
	CParticleGroupManager::Instance().Write(&fs);

	//effect mager
	fs.WriteID_t("EEFM");
	CEffectManager::Instance().Write(&fs);

	//effect group mager
	fs.WriteID_t("EEGM");
	CEffectGroupManager::Instance().Write(&fs);

	//effect end
	fs.WriteID_t("EFTE");
	
	return TRUE;
}

//effect추가시
extern CEffect *CreateFromType(EFFECT_TYPE et)
{
	switch(et)
	{
	case ET_TERRAIN:	return new CTerrainEffect;
	case ET_LIGHT:		return new CLightEffect;
	case ET_PARTICLE:	return new CParticleEffect;
	case ET_SKA:		return new CSkaEffect;
	case ET_MDL:		return new CMdlEffect;
	case ET_TRACE:		return new CTraceEffect;
	case ET_SOUND:		return new CSoundEffect;
	case ET_SPLINEBILLBOARD:
						return new CSplineBillboardEffect;
	case ET_ORBIT:		return new COrbitEffect;
	case ET_SHOCKWAVE:	return new CShockWaveEffect;
	case ET_SPLINEPATH:	return new CSplinePathEffect;
	case ET_CAMERA:		return new CCameraEffect;
	case ET_ENTITY:		return new CEntityEffect;
	default: ASSERTALWAYS("없는 이펙트 타입이거나 렌더링이 허용되지 않는 이펙트 타입입니다.");
	}
	return NULL;
}
//안태훈 수정 끝	//(Add & Modify SSSE Effect)(0.1)