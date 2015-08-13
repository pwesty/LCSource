#ifndef		ACTOR_MANAGER_H_
#define		ACTOR_MANAGER_H_

#include <Engine/DesignClasses/Singleton/SingletonBase.h>
#include <map>
#include <Engine/Object/DefineObject.h>
#include <Engine/Interface/UICommon.h>
#include <Engine/Interface/UIDefine.h>

struct ObjectBase;
class CMobTarget;
class CCharacterTarget;
class CUIImageArray;
class CItemTarget;
class CSlaveTarget;
class CSelectedEntities;
class CPetTarget;
class CWildPetTarget;

class ENGINE_API ActorMgr : public CSingletonBase< ActorMgr >
{
public:
	ActorMgr();
	~ActorMgr();

	void init();
	void initSyndiMark();
	void release();

	//----------------------------------------------------------

	void AddObject(ObjectBase* pObject);
	void RemoveObject(eOBJ_TYPE eType, UINT index);
	void RemoveAll();

	ObjectBase* GetObject(eOBJ_TYPE eType, UINT index);
	ObjectBase* GetObjectByCIndex(eOBJ_TYPE eType, UINT index);
	ObjectBase* GetObjectByNPCIdx(UINT index);
	ObjectBase* GetObjectByEntity(CEntity* pEntity);
	ObjectBase* GetObjectByName(const char* strName, eOBJ_TYPE eType);
	ObjectBase* GetObjectByDistance(FLOAT3D vPos);

//	int FindIndexClient(UINT indexServer);

	CEntity* GetEntityByIndexServer(eOBJ_TYPE eType, UINT IndexServer);

	void Update(float fDeltaTime, float fElapsedTime);

	// ----------------------------------------------------------

	void RefreshQuestMark();
	void RefreshNPCQuestMark(INDEX iNPCIndex);
	void RefreshGuildMasterEffect();
	void StartGuildEffect();
	void RefreshSiegeWarfareEffect(bool bDratan);

	void ChangeEffect(eOBJ_TYPE type, INDEX nKind, const char* EffectName, const char* DestName);

	bool CheckNPCNotice(INDEX npcType, CTString& msg);

	void DrawMobRadar(int nCX, int nCY, float fRatio, CDrawPort* pDraw, UIRect& rc, UIRectUV& uv);
	void DrawCharRadar(int nCX, int nCY, float fRatio, CDrawPort* pDraw, UIRect& rc, UIRectUV& uv, bool bGuild = false);

	void ClearLabel();

	void DrawObjectName(CDrawPort* pDraw, CProjection3D* pprProjection, COLOR colBlend, bool bHideName);

	void SetJoinFlagMerac(SBYTE sbFlag);

	void ChangeGuildLevel(LONG lGuildIdx, LONG lGuildLv);
	void ChangeGuildInfo(LONG lGuildIdx, LONG idx, LONG lv, LONG pos, const char* name);

	int  CheckSyndiMark(INDEX type, INDEX grade);
	void DrawSyndiMark(int nDrawIdx, CDrawPort* pDraw, int x, int y, FLOAT fz);

	void SearchNearItem(CSelectedEntities* pSelectEntity, FLOAT3D vPos, float fDistance);

private:
	void RemoveCMap(eOBJ_TYPE eType, UINT index);
	void RemoveNMap(UINT index);

	void DrawMob(CMobTarget* pTarget, CDrawPort* pDraw, CProjection3D* pprProjection);

	void DrawCharacter(CCharacterTarget* pTarget, CDrawPort* pDraw, CProjection3D* pprProjection, 
		COLOR colBlend, bool bHideName);

	void DrawItem(CItemTarget* pTarget, CDrawPort* pDraw, CProjection3D* pprProjection);
	void DrawSlave(CSlaveTarget* pTarget, CDrawPort* pDraw, CProjection3D* pprProjection);
	void DrawPet(CPetTarget* pTarget, CDrawPort* pDraw, CProjection3D* pprProjection);
	void DrawWildPet(CWildPetTarget* pTarget, CDrawPort* pDraw, CProjection3D* pprProjection);

	typedef std::map< UINT, ObjectBase* >	mapObject;
	typedef mapObject::iterator				mapObject_iter;

	typedef	std::map< UINT, UINT >			mapIndex;
	typedef mapIndex::iterator				mapIndex_iter;

	mapObject	_map[eOBJ_MAX];
	mapIndex	_mapClient[eOBJ_MAX];
	mapIndex	_mapNPC;

	CTextureData*	m_ptdPopupTexture;
	CTextureData*	m_ptdExpeditionTexture;
	CTextureData*	m_ptdMap;
	//0 µµ³¢, 1 À¯·É, 2 ¹ß¹Ù´Ú, 3 Á¶°¢Ä®, 4 ¶Ë, 5 ¾Ç¸¶, 6 ÇØ°ñ 
	UIRectUV		m_rtTargetLabel[LABEL_INDEX_TOTAL];	
	CUIRectSurface	m_rsChatBG;
	UIRectUV		m_uvChatBG;

	UIRectUV		m_rtNameL;
	UIRectUV		m_rtNameC;
	UIRectUV		m_rtNameR;
	UIRectUV		m_rtNameRPK;
	UIRectUV		m_rtNameRDefPK1;
	UIRectUV		m_rtNameRDefPK2;
	UIRectUV		m_rtNickNameL;
	UIRectUV		m_rtNickNameC;
	UIRectUV		m_rtNickNameR;
	UIRectUV		m_rtPcNameL;					// UV of Premium char name effect
	UIRectUV		m_rtPcNameC;
	UIRectUV		m_rtPcNameR;
	UIRectUV		m_rtSelPcNameL;					// UV of Premium char name effect Select
	UIRectUV		m_rtSelPcNameC;
	UIRectUV		m_rtSelPcNameR;

	UIRectUV		m_rtShopPremLU;
	UIRectUV		m_rtShopPremUp;
	UIRectUV		m_rtShopPremRU;
	UIRectUV		m_rtShopPremL;
	UIRectUV		m_rtShopPremC;
	UIRectUV		m_rtShopPremR;
	UIRectUV		m_rtShopPremLL;
	UIRectUV		m_rtShopPremLo;
	UIRectUV		m_rtShopPremRL;

	UIRectUV		m_rtShopLU;
	UIRectUV		m_rtShopUp;
	UIRectUV		m_rtShopRU;
	UIRectUV		m_rtShopL;
	UIRectUV		m_rtShopC;
	UIRectUV		m_rtShopR;
	UIRectUV		m_rtShopLL;
	UIRectUV		m_rtShopLo;
	UIRectUV		m_rtShopRL;

	UIRectUV		m_rtChatLU;
	UIRectUV		m_rtChatUp;
	UIRectUV		m_rtChatRU;
	UIRectUV		m_rtChatL;
	UIRectUV		m_rtChatC;
	UIRectUV		m_rtChatR;
	UIRectUV		m_rtChatLL;
	UIRectUV		m_rtChatLo;
	UIRectUV		m_rtChatRL;

	CUIImageArray*  m_pImgArrSyndiMark;

	float			m_fDeltaGPS;
	UIRectUV		m_uvGPS;
	UIRectUV		m_uvGPSBG;
	COLOR			m_colGPS;
};

#define		ACTORMGR()		ActorMgr::getSingleton()


#endif		// ACTOR_MANAGER_H_