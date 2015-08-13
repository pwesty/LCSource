//안태훈 수정 시작	//(Add & Modify SSSE Effect)(0.1)
#ifdef PRAGMA_ONCE
  #pragma once
#endif

class CAnyProjection;
class CDrawPort;
class CTextureData;
class CTFileName;

extern CTextureObject g_toLightMap;

#include "CEffect.h"

ENGINE_API extern FLOAT g_fEffectDistance;
ENGINE_API extern void FinalizeShaders(void);
ENGINE_API extern void InitializeShaders(void);
//이펙트 관련 초기화 작업을 수행한다.
ENGINE_API extern BOOL Initialize_EffectSystem(const CTFileName *fnm = NULL);
//이펙트 관련 마무리 작업을 수행한다.
ENGINE_API extern BOOL Finalize_EffectSystem();
//이펙트 렌더링 관련 정보 세팅
ENGINE_API extern BOOL PrepareRender_EffectSystem(CTerrain *pTerrain, CWorld *pWorld);
//이펙트 렌더링 시작
ENGINE_API extern BOOL BeginRender_EffectSystem(EFFECT_TYPE effectType, CAnyProjection3D &apr, CDrawPort *pdp);
//이펙트 렌더링 끝
ENGINE_API extern BOOL EndRender_EffectSystem(EFFECT_TYPE effectType, BOOL bRestoreOrtho);
//이펙트 리셋
ENGINE_API extern void Reset_EffectSystem();
//이펙트 파일 로딩
ENGINE_API extern BOOL Open_EffectSystem(const CTFileName &fnm);
//이펙트 파일 저장
ENGINE_API extern BOOL Save_EffectSystem(const CTFileName &fnm);

CEffect *CreateFromType(EFFECT_TYPE et);

//안태훈 수정 끝	//(Add & Modify SSSE Effect)(0.1)