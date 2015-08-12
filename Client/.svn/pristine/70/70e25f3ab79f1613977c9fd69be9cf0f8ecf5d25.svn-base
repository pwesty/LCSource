#include "StdH.h"
#include "ActorMgr.h"
#include <Engine/Object/ObjectBase.h>
#include <Engine/Util.h>
#include <Engine/Network/MobTarget.h>
#include <Engine/Network/CharacterTarget.h>
#include <Engine/Network/ItemTarget.h>
#include <Engine/Network/MultiTarget.h>
#include <Engine/Network/SlaveTarget.h>
#include <Engine/Network/PetTarget.h>
#include <Engine/Ska/ModelInstance.h>
#include <Engine/Network/CNetwork.h>
#include <Engine/Entities/QuestSystem.h>
#include <Engine/Help/Util_Help.h>
#include <Engine/Interface/UITextureManager.h>
#include <Engine/Interface/UIManager.h>
#include <Engine/Interface/UIRadar.h>
#include <Engine/Interface/UIOption.h>
#include <Engine/Contents/function/TargetInfoNewUI.h>
#include <Engine/Interface/UIPersonalShop.h>
#include <Engine/Interface/UINickName.h>
#include <Engine/Interface/UISiegeWarfareDoc.h>
#include <Engine/Interface/UIGuild.h>
#include <Engine/Interface/UIGuildBattle.h>
#include <Engine/Interface/UIImageArray.h>
#include <Engine/Contents/Base/Syndicate.h>
#include <Engine/Help/UISupport.h>
#include <Engine/Contents/function/gps.h>
#include <Engine/GameDataManager/GameDataManager.h>
#include <Engine/Contents/function/PremiumChar.h>
#include <Engine/Info/MyInfo.h>

#define RADAR_DEFENSE_COLOR 0x0000FFFF		// 수성측 Point 색
#define	RADAR_ATTACK_COLOR	0xFF0000FF		// 공성측 Point 색
#define DEF_MARK_WIDTH 30 // 결사대 마크 넓이
#define DEF_MARK_GPS_UPDATE		0.1f

enum eSYNDICATE_MARK_GRADE
{
	eMARK_KAY_NONE = 0,
	eMARK_KAY_SPECIAL,
	eMARK_KAY_KING,
	eMARK_DEAL_NONE,
	eMARK_DEAL_SPECIAL,
	eMARK_DEAL_KING,
	eMARK_GRADE_MAX
};

ActorMgr::ActorMgr()
	: m_pImgArrSyndiMark(NULL)
{
	init();
}

ActorMgr::~ActorMgr()
{
	release();
}

void ActorMgr::init()
{
	float fTexWidth, fTexHeight;

	m_ptdPopupTexture = _pTextureStock->Obtain_t( CTString( "Data\\Interface\\NamePopup.tex" ) );
	fTexWidth = m_ptdPopupTexture->GetPixWidth();
	fTexHeight = m_ptdPopupTexture->GetPixHeight();
	
	m_uvChatBG.SetUV(0, 240, 16, 256);

	m_rtNameL.SetUV( 0, 14, 2, 29, fTexWidth, fTexHeight );
	m_rtNameC.SetUV( 4, 14, 14, 29, fTexWidth, fTexHeight );
	m_rtNameR.SetUV( 30, 14, 32, 29, fTexWidth, fTexHeight );
	m_rtNameRPK.SetUV( 16, 14, 32, 29, fTexWidth, fTexHeight );
	//m_rtNameRDefPK1.SetUV( 0, 30, 16, 45, fTexWidth, fTexHeight );
	m_rtNameRDefPK2.SetUV( 18, 30, 34, 45, fTexWidth, fTexHeight );

	m_rtNickNameL.SetUV(35, 31, 37, 44, fTexWidth, fTexHeight );
	m_rtNickNameC.SetUV(39, 31, 49, 44, fTexWidth, fTexHeight );
	m_rtNickNameR.SetUV(65, 31, 67, 44, fTexWidth, fTexHeight );

#ifdef PREMIUM_CHAR
	m_rtPcNameL.SetUV( 238, 58, 240, 108, fTexWidth, fTexHeight );
	m_rtPcNameC.SetUV( 240, 58, 241, 108, fTexWidth, fTexHeight );
	m_rtPcNameR.SetUV( 241, 58, 304, 108, fTexWidth, fTexHeight );
	m_rtSelPcNameL.SetUV( 238, 5, 240, 55, fTexWidth, fTexHeight );
	m_rtSelPcNameC.SetUV( 240, 5, 241, 55, fTexWidth, fTexHeight );
	m_rtSelPcNameR.SetUV( 241, 5, 304, 55, fTexWidth, fTexHeight );
#endif	//	PREMIUM_CHAR

	m_rtShopPremLU.SetUV( 35, 0, 48, 13, fTexWidth, fTexHeight );
	m_rtShopPremUp.SetUV( 49, 0, 50, 13, fTexWidth, fTexHeight );
	m_rtShopPremRU.SetUV( 51, 0, 64, 13, fTexWidth, fTexHeight );
	m_rtShopPremL.SetUV( 35, 14, 48, 16, fTexWidth, fTexHeight );
	m_rtShopPremC.SetUV( 49, 14, 50, 16, fTexWidth, fTexHeight );
	m_rtShopPremR.SetUV( 51, 14, 64, 16, fTexWidth, fTexHeight );
	m_rtShopPremLL.SetUV( 35, 16, 48, 29, fTexWidth, fTexHeight );
	m_rtShopPremLo.SetUV( 49, 16, 50, 29, fTexWidth, fTexHeight );
	m_rtShopPremRL.SetUV( 51, 16, 64, 29, fTexWidth, fTexHeight );

	m_rtShopLU.SetUV( 15, 0, 20, 5, fTexWidth, fTexHeight );
	m_rtShopUp.SetUV( 21, 0, 22, 5, fTexWidth, fTexHeight );
	m_rtShopRU.SetUV( 23, 0, 28, 5, fTexWidth, fTexHeight );
	m_rtShopL.SetUV( 15, 6, 20, 7, fTexWidth, fTexHeight );
	m_rtShopC.SetUV( 21, 6, 22, 7, fTexWidth, fTexHeight );
	m_rtShopR.SetUV( 23, 6, 28, 7, fTexWidth, fTexHeight );
	m_rtShopLL.SetUV( 15, 8, 20, 13, fTexWidth, fTexHeight );
	m_rtShopLo.SetUV( 21, 8, 22, 13, fTexWidth, fTexHeight );
	m_rtShopRL.SetUV( 23, 8, 28, 13, fTexWidth, fTexHeight );

	m_rtChatLU.SetUV( 0, 0, 5, 5, fTexWidth, fTexHeight );
	m_rtChatUp.SetUV( 6, 0, 7, 5, fTexWidth, fTexHeight );
	m_rtChatRU.SetUV( 8, 0, 13, 5, fTexWidth, fTexHeight );
	m_rtChatL.SetUV( 0, 6, 5, 7, fTexWidth, fTexHeight );
	m_rtChatC.SetUV( 6, 6, 7, 7, fTexWidth, fTexHeight );
	m_rtChatR.SetUV( 8, 6, 13, 7, fTexWidth, fTexHeight );
	m_rtChatLL.SetUV( 0, 8, 5, 13, fTexWidth, fTexHeight );
	m_rtChatLo.SetUV( 6, 8, 7, 13, fTexWidth, fTexHeight );
	m_rtChatRL.SetUV( 8, 8, 13, 13, fTexWidth, fTexHeight );

	m_uvGPS.SetUV(87, 135, 131, 206, fTexWidth, fTexHeight);
	m_uvGPSBG.SetUV(153, 129, 242, 227, fTexWidth, fTexHeight);
	m_colGPS = DEF_UI_COLOR_WHITE;
	m_fDeltaGPS = 0.f;

	m_ptdExpeditionTexture = _pTextureStock->Obtain_t( CTString( "Data\\Interface\\Expedition.tex" ) );
	fTexWidth = m_ptdExpeditionTexture->GetPixWidth();
	fTexHeight = m_ptdExpeditionTexture->GetPixHeight();

	m_rtTargetLabel[6].SetUV( 355, 211, 397, 253, fTexWidth, fTexHeight);
	m_rtTargetLabel[5].SetUV( 403, 210, 445, 252, fTexWidth, fTexHeight);
	m_rtTargetLabel[4].SetUV( 459, 205, 501, 247, fTexWidth, fTexHeight);
	m_rtTargetLabel[3].SetUV( 355, 261, 397, 303, fTexWidth, fTexHeight);
	m_rtTargetLabel[2].SetUV( 401, 258, 443, 300, fTexWidth, fTexHeight);
	m_rtTargetLabel[1].SetUV( 459, 256, 501, 298, fTexWidth, fTexHeight);
	m_rtTargetLabel[0].SetUV( 358, 313, 400, 355, fTexWidth, fTexHeight);	
}

void ActorMgr::initSyndiMark()
{
	if (m_pImgArrSyndiMark != NULL)
		return;

	m_pImgArrSyndiMark = new CUIImageArray;

	m_pImgArrSyndiMark->setTexString("NamePopup.tex");
	m_pImgArrSyndiMark->InitPos(0, 0, 30, 34);
	m_pImgArrSyndiMark->SetDepthMode(TRUE);

	////////////// namePopup.tex //////////////////////////////
	/////////// Mark Image Idx ////////////////////////////////
	/////////// x 0 /// x 32 /// x 64 /////////////////////////
	// y 130//  왕 2,  공작 1,  일반 0 ////// 카이룩스 //////////
	// y 167//  왕 5,  공작 4,  일반 3 ////// 딜라문   //////////
	///////////////////////////////////////////////////////////

	int x, nGabX = 2;
	int y, nGabY = 3;

	UIRect rc(0, 0, 30, 34);
	UIRectUV uv;

	for (int i = 0; i < 6; i++)
	{
		x = (i / 3) * (30 + nGabX);
		y = 204 - ((i % 3) * (34 + nGabY));

		uv.SetUV(x, y, x + 30, y + 34);
		m_pImgArrSyndiMark->AddImage(rc, uv);
	}
}

void ActorMgr::release()
{
	STOCK_RELEASE(m_ptdPopupTexture);
	STOCK_RELEASE(m_ptdExpeditionTexture);
	SAFE_DELETE(m_pImgArrSyndiMark);
	RemoveAll();
}

//----------------------------------------------------------

void ActorMgr::AddObject( ObjectBase* pObject )
{
	if (pObject == NULL)
		return;

	if (pObject->m_eType >= eOBJ_MAX)
		return;

	_map[pObject->m_eType].insert( std::make_pair(pObject->m_nIdxServer, pObject) );
	ASSERT(pObject->m_nIdxClient >= 0);
	_mapClient[pObject->m_eType].insert( std::make_pair(pObject->m_nIdxClient, pObject->m_nIdxServer) );
	
	if (pObject->m_eType == eOBJ_MOB)
		_mapNPC.insert( std::make_pair(pObject->m_nType, pObject->m_nIdxServer) );
}

void ActorMgr::RemoveObject( eOBJ_TYPE eType, UINT index )
{
	if (eType >= eOBJ_MAX)
		return;

	mapObject_iter iter = _map[eType].find(index);

	if (iter != _map[eType].end())
	{
		RemoveCMap(eType, iter->second->m_nIdxClient);

		if (iter->second->m_eType == eOBJ_MOB)
			RemoveNMap(iter->second->m_nType);

		SAFE_DELETE(iter->second);
		_map[eType].erase(iter);
	}
}

void ActorMgr::RemoveAll()
{
	int		i;

	for (i = 0; i < eOBJ_MAX; ++i)
	{
		mapObject_iter	iter = _map[i].begin();
		mapObject_iter	eiter = _map[i].end();

		for ( ;iter != eiter; ++iter)
		{
			SAFE_DELETE(iter->second);
		}

		_map[i].clear();
	}

	for (i = 0; i < eOBJ_MAX; ++i)
		_mapClient[i].clear();

	_mapNPC.clear();
}

void ActorMgr::RemoveCMap( eOBJ_TYPE eType, UINT index )
{
	if (eType >= eOBJ_MAX)
		return;

	mapIndex_iter iter = _mapClient[eType].find(index);

	if (iter != _mapClient[eType].end())
	{
		_mapClient[eType].erase(iter);
	}
}

void ActorMgr::RemoveNMap( UINT index )
{
	mapIndex_iter iter = _mapNPC.find(index);

	if (iter != _mapNPC.end())
	{
		_mapNPC.erase(iter);
	}
}

ObjectBase* ActorMgr::GetObject( eOBJ_TYPE eType, UINT index )
{
	mapObject_iter iter = _map[eType].find(index);

	if (iter == _map[eType].end())
		return NULL;

	return iter->second;
}

ObjectBase* ActorMgr::GetObjectByCIndex( eOBJ_TYPE eType, UINT index )
{
	if (eType >= eOBJ_MAX)
		return NULL;

	mapIndex_iter iter = _mapClient[eType].find(index);

	if (iter == _mapClient[eType].end())
		return NULL;

	return GetObject(eType, iter->second);
}

ObjectBase* ActorMgr::GetObjectByNPCIdx( UINT index )
{
	mapIndex_iter iter = _mapNPC.find(index);

	if (iter == _mapNPC.end())
		return NULL;

	return GetObject(eOBJ_MOB, iter->second);
}

ObjectBase* ActorMgr::GetObjectByEntity( CEntity* pEntity )
{
	int		i;

	for (i = 0; i < eOBJ_MAX; ++i)
	{
		mapObject_iter	iter = _map[i].begin();
		mapObject_iter	eiter = _map[i].end();

		for ( ;iter != eiter; ++iter)
		{
			if (iter->second->GetEntity() == pEntity)
				return iter->second;
		}
	}

	return NULL;
}

ObjectBase* ActorMgr::GetObjectByName( const char* strName, eOBJ_TYPE eType )
{
	int		i;

	for (i = 0; i < eOBJ_MAX; ++i)
	{
		mapObject_iter	iter = _map[i].begin();
		mapObject_iter	eiter = _map[i].end();

		for ( ;iter != eiter; ++iter)
		{
			if (iter->second->m_eType == eType)
			{
				if (strcmp(iter->second->m_strName.c_str(), strName) == 0)
					return iter->second;
			}
		}
	}

	return NULL;
}

ObjectBase* ActorMgr::GetObjectByDistance( FLOAT3D vPos )
{
	ObjectBase* pOut = NULL;

	mapObject_iter	iter = _map[eOBJ_ITEM].begin();
	mapObject_iter	eiter = _map[eOBJ_ITEM].end();

	int myLayer = _pNetwork->MyCharacterInfo.yLayer;
		
	CItemTarget* pTarget;
	FLOAT	fNear = FLT_MAX;
	FLOAT	fTmpDistance = -1.f;

	for ( ;iter != eiter; ++iter)
	{
		if (iter->second->GetyLayer() == myLayer)
		{
			pTarget = static_cast< CItemTarget* >(iter->second);
			FLOAT3D vDelta = vPos - pTarget->item_place;
			vDelta(2) = 0.f; //0131 높이값 무시.
			fTmpDistance = vDelta.Length();

			if (fNear > fTmpDistance)
			{
				fNear = fTmpDistance;
				pOut = pTarget;
			}
		}
	}

	return pOut;
}

// int ActorMgr::FindIndexClient( UINT indexServer )
// {
// 	mapObject_iter iter = _map.find(indexServer);
// 
// 	if (iter == _map.end())
// 		return -1;
// 
// 	return iter->second->m_nIdxClient;
// }

CEntity* ActorMgr::GetEntityByIndexServer( eOBJ_TYPE eType, UINT IndexServer )
{
	if (eType >= eOBJ_MAX)
		return NULL;

	mapObject_iter iter = _map[eType].find(IndexServer);

	if (iter == _map[eType].end())
		return NULL;

	return iter->second->m_pEntity;
}

void ActorMgr::Update( float fDeltaTime, float fElapsedTime )
{
	if (GAMEDATAMGR()->GetGPS()->IsPosition() == true)
	{
		m_fDeltaGPS += fDeltaTime;

		if (m_fDeltaGPS > DEF_MARK_GPS_UPDATE)
		{
			UINT8 alpha = m_colGPS & 0xFF;
			alpha += (int)(70.f * m_fDeltaGPS);
			m_colGPS &= 0xFFFFFF00;
			m_colGPS |= alpha;

			m_fDeltaGPS -= floor(m_fDeltaGPS / DEF_MARK_GPS_UPDATE) * DEF_MARK_GPS_UPDATE;
		}
	}
}

// ----------------------------------------------------------

void ActorMgr::RefreshQuestMark()
{
	//모든 npc의 quest mark 갱신.
	mapObject_iter	iter = _map[eOBJ_MOB].begin();
	mapObject_iter	eiter = _map[eOBJ_MOB].end();

	CMobTarget* pMT;

	for ( ;iter != eiter; ++iter)
	{
		pMT = static_cast<CMobTarget*>(iter->second);

		if (pMT->IsNPC() == TRUE)
			RefreshNPCQuestMark(pMT->m_nType);
	}	
}

void ActorMgr::RefreshNPCQuestMark( INDEX iNPCIndex )
{
	mapIndex_iter	iter  = _mapNPC.find(iNPCIndex);

	if (iter != _mapNPC.end())
	{
		ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_MOB, iter->second);

		if (pObject != NULL)
		{
			CMobTarget* pTarget = static_cast< CMobTarget* >(pObject);

			CEntity *penNPC = pTarget->GetEntity();
			
			if(penNPC && penNPC->GetModelInstance())
			{
				pTarget->mob_statusEffect.ChangeNPCQuestMark(
					&penNPC->GetModelInstance()->m_tmSkaTagManager
					, CQuestSystem::Instance().TestNPCForQuest(iNPCIndex));
			}
		}
	}
}

void ActorMgr::RefreshGuildMasterEffect()
{
	mapObject_iter	iter = _map[eOBJ_CHARACTER].begin();
	mapObject_iter	eiter = _map[eOBJ_CHARACTER].end();

	CCharacterTarget* pTarget;	

	for ( ;iter != eiter; ++iter)
	{
		pTarget = static_cast< CCharacterTarget* >(iter->second);

		if( pTarget->GetEntity() != NULL && 
			pTarget->GetEntity()->en_pmiModelInstance != NULL && 
			pTarget->GetEntity()->en_pmiModelInstance->GetName() != "" )
		{
			_pUISWDoc->StartGuildMasterEffect( pTarget->m_nIdxServer, pTarget->GetEntity(), 
				pTarget->cha_sbJoinFlagMerac, pTarget->cha_lGuildPosition, 
				pTarget->cha_lGuildPosition );
		}
	}	
}

void ActorMgr::StartGuildEffect()
{
	mapObject_iter	iter = _map[eOBJ_CHARACTER].begin();
	mapObject_iter	eiter = _map[eOBJ_CHARACTER].end();

	CCharacterTarget* pTarget;
	int bEnmeyGuild;

	CUIGuildBattle* pGBattle = CUIManager::getSingleton()->GetGuildBattle();

	for ( ;iter != eiter; ++iter)
	{
		pTarget = static_cast< CCharacterTarget* >(iter->second);

		if( pTarget->GetEntity() != NULL && 
			pTarget->GetEntity()->en_pmiModelInstance != NULL && 
			pTarget->GetEntity()->en_pmiModelInstance->GetName() != "" )
		{
			bEnmeyGuild = pGBattle->IsEnemyGuild( pTarget->cha_lGuildIndex );

			if (bEnmeyGuild == 1)
				pGBattle->StartGuildEffect ( pTarget->m_nIdxServer, pTarget->m_pEntity, TRUE );
			else if (bEnmeyGuild == -1)
				pGBattle->StartGuildEffect ( pTarget->m_nIdxServer, pTarget->m_pEntity, FALSE );
		}
	}	
}

void ActorMgr::RefreshSiegeWarfareEffect( bool bDratan )
{
	mapObject_iter	iter = _map[eOBJ_CHARACTER].begin();
	mapObject_iter	eiter = _map[eOBJ_CHARACTER].end();

	CCharacterTarget* pTarget;	

	for ( ;iter != eiter; ++iter)
	{
		pTarget = static_cast< CCharacterTarget* >(iter->second);

		if( pTarget->GetEntity() != NULL && 
			pTarget->GetEntity()->en_pmiModelInstance != NULL && 
			pTarget->GetEntity()->en_pmiModelInstance->GetName() != "" )
		{
			if (bDratan == true)
			{
				_pUISWDoc->StartEffect( pTarget->m_nIdxServer, pTarget->GetEntity(), 
					pTarget->cha_sbJoinFlagDratan, pTarget->cha_lGuildIndex );
			}
			else
			{
				_pUISWDoc->StartEffect( pTarget->m_nIdxServer, pTarget->GetEntity(), 
					pTarget->cha_sbJoinFlagMerac, pTarget->cha_lGuildIndex );
			}
		}		
	}
}

void ActorMgr::ChangeEffect( eOBJ_TYPE type, INDEX nKind, const char* EffectName, const char* DestName )
{
	if (type >= eOBJ_MAX)
		return;

	mapObject_iter	iter = _map[type].begin();
	mapObject_iter	eiter = _map[type].end();

	ObjectBase* pObject = NULL;

	for ( ;iter != eiter; ++iter)
	{
		pObject = iter->second;

		if (pObject->m_eType == type && pObject->GetType() == nKind)
		{
			CEffectGroup* pBaseEffect = CEffectGroupManager::Instance().GetEffectGroup( EffectName, pObject->GetEntity());

			if (pBaseEffect != NULL)
				DestroyEffectGroup(pBaseEffect);

			CEffectGroup* pApendCheckEffect = CEffectGroupManager::Instance().GetEffectGroup( DestName, pObject->GetEntity());

			if (pApendCheckEffect == NULL)
			{
				CMobTarget* pMT = static_cast< CMobTarget* >(pObject);
				pMT->mob_pNormalEffect = StartEffectGroup( DestName, 
					&pMT->GetEntity()->en_pmiModelInstance->m_tmSkaTagManager, 
					_pTimer->GetLerpedCurrentTick() );
			}
		}
	}	
}

bool ActorMgr::CheckNPCNotice( INDEX npcType, CTString& msg )
{
	mapObject_iter	iter = _map[eOBJ_MOB].begin();
	mapObject_iter	eiter = _map[eOBJ_MOB].end();

	CMobTarget* pMT;
	bool bRet = false;

	UtilHelp* pHelp = UtilHelp::getSingleton();

	for ( ;iter != eiter; ++iter)
	{
		pMT = static_cast<CMobTarget*>(iter->second);

		if (pMT->GetType() == npcType || npcType == 0)
		{
			if( pHelp->nearMyCharacter(pMT->m_pEntity->en_plPlacement.pl_PositionVector, CHATMSG_SQRDIST) )
			{
				bRet = true;
				pMT->ChatMsg.SetChatMsg( msg );
			}
		}
	}

	return bRet;
}

void ActorMgr::DrawMobRadar(int nCX, int nCY, float fRatio, CDrawPort* pDraw, UIRect& rc, UIRectUV& uv)
{
	if (pDraw == NULL)
		return;

	mapObject_iter	iter = _map[eOBJ_MOB].begin();
	mapObject_iter	eiter = _map[eOBJ_MOB].end();

	CMobTarget* pMT;
	SBYTE sbLayerDiff;
	FLOAT3D vPos;
	CEntity* pEntity;
	FLOAT fX, fZ;
	int	  nX, nY;
	FLOAT fXpc = _pNetwork->MyCharacterInfo.x;
	FLOAT fZpc = _pNetwork->MyCharacterInfo.z;
	SBYTE sbLayerpc = _pNetwork->MyCharacterInfo.yLayer;

	CUIRadar* pRadar = CUIManager::getSingleton()->GetRadar();

	for ( ;iter != eiter; ++iter)
	{
		pMT = static_cast<CMobTarget*>(iter->second);

		if (pMT->IsNPC() == TRUE)
			continue;

		if (pMT->GetType() == 1152 || pMT->GetType() == 1255)
			continue;

		sbLayerDiff = pMT->m_yLayer - sbLayerpc;
		if( sbLayerDiff < -1 || sbLayerDiff > 1 )
			continue;

		pEntity = pMT->GetEntity();

		if (pEntity == NULL)
			continue;

		vPos = pEntity->en_plPlacement.pl_PositionVector;

		// Test distance
		fX = vPos(1) - fXpc;
		fZ = vPos(3) - fZpc;

		fX *= fRatio;
		fZ *= fRatio;

		if( fX > 47.0f || fX < -47.0f || fZ > 47.0f || fZ < -47.0f )
			continue;

		nX = nCX + fX;
		nY = nCY + fZ;

		pRadar->AddIconName( nX + rc.Left,  nY + rc.Top,
			nX + rc.Right, nY + rc.Bottom, pMT->m_strName.c_str());

		pDraw->AddTexture( nX + rc.Left,  nY + rc.Top,
			nX + rc.Right, nY + rc.Bottom,
			uv.U0, uv.V0, uv.U1, uv.V1,
			DEF_UI_COLOR_WHITE );
	}
}

void ActorMgr::DrawCharRadar( int nCX, int nCY, float fRatio, CDrawPort* pDraw, 
					UIRect& rc, UIRectUV& uv, bool bGuild )
{
	if (pDraw == NULL)
		return;

	mapObject_iter	iter = _map[eOBJ_CHARACTER].begin();
	mapObject_iter	eiter = _map[eOBJ_CHARACTER].end();

	COLOR colRenderType = 0xFFFFFFFF;
	CCharacterTarget* pTarget = NULL;
	SBYTE sbLayerDiff;
	SBYTE sbLayerpc = _pNetwork->MyCharacterInfo.yLayer;
	FLOAT3D vPos;
	FLOAT fX, fZ;
	int	  nX, nY;
	FLOAT fXpc = _pNetwork->MyCharacterInfo.x;
	FLOAT fZpc = _pNetwork->MyCharacterInfo.z;

	CUIRadar* pRadar = CUIManager::getSingleton()->GetRadar();

	for ( ;iter != eiter; ++iter)
	{
		pTarget = static_cast< CCharacterTarget* >(iter->second);

		if (bGuild == true)
		{
			if (pTarget->cha_lGuildIndex == GUILD_MEMBER_NOMEMBER ||
				pTarget->cha_lGuildIndex != _pNetwork->MyCharacterInfo.lGuildIndex)
				continue;
		}
		else
		{
			switch( pTarget->cha_sbJoinFlagMerac )
			{
				// 참여하고 있지 않음
			case WCJF_NONE: 
				continue;
				break;

				// 수성
			case WCJF_OWNER:
			case WCJF_DEFENSE_GUILD:
			case WCJF_DEFENSE_CHAR:
				colRenderType = RADAR_DEFENSE_COLOR;
				break;

				// 공성측
			case WCJF_ATTACK_GUILD:
			case WCJF_ATTACK_CHAR:
				colRenderType = RADAR_ATTACK_COLOR;
				break;
			default :
				continue;
			}
		}

		sbLayerDiff = pTarget->cha_yLayer - sbLayerpc;
		if( sbLayerDiff < -1 || sbLayerDiff > 1 )
			continue;

		vPos = pTarget->GetEntity()->en_plPlacement.pl_PositionVector;

		// Test distance
		fX = vPos(1) - fXpc;
		fZ = vPos(3) - fZpc;

		fX *= fRatio;
		fZ *= fRatio;

		if( fX > 47.0f || fX < -47.0f || fZ > 47.0f || fZ < -47.0f )
			continue;

		nX = nCX + fX;
		nY = nCY + fZ;

		pRadar->AddIconName( nX + rc.Left,  nY + rc.Top,
			nX + rc.Right, nY + rc.Bottom, pTarget->m_strName.c_str());

		pDraw->AddTexture( nX + rc.Left,  nY + rc.Top,
			nX + rc.Right, nY + rc.Bottom,
			uv, colRenderType );

	}	
}

void ActorMgr::ClearLabel()
{
	{
		mapObject_iter	iter = _map[eOBJ_MOB].begin();
		mapObject_iter	eiter = _map[eOBJ_MOB].end();

		for ( ;iter != eiter; ++iter)
		{
			CMobTarget* pTarget = static_cast< CMobTarget* >(iter->second);
			pTarget->mob_Label = -1;
		}
	}

	{
		mapObject_iter	iter = _map[eOBJ_CHARACTER].begin();
		mapObject_iter	eiter = _map[eOBJ_CHARACTER].end();

		for ( ;iter != eiter; ++iter)
		{
			CCharacterTarget* pTarget = static_cast< CCharacterTarget* >(iter->second);
			pTarget->cha_Label = -1;
		}
	}
}

extern CUIFontTextureManager* _pUIFontTexMgr;
extern INDEX	g_iShowName;
extern INDEX	g_iShowNameItem;
extern INDEX	g_iCountry;
extern INDEX	_iNameType;

void ActorMgr::DrawObjectName(CDrawPort* pDraw, CProjection3D* pprProjection, COLOR colBlend, bool bHideName)
{
	if (g_iShowName <= 0 )
		return;

	int		i;

	for (i = 0; i < eOBJ_MAX; ++i)
	{
		mapObject_iter	iter = _map[i].begin();
		mapObject_iter	eiter = _map[i].end();

		for ( ;iter != eiter; ++iter)
		{
			if (iter->second->m_eType == eOBJ_MOB)
			{
				CMobTarget* pTarget = static_cast< CMobTarget* >(iter->second);
				DrawMob(pTarget, pDraw, pprProjection);
			}
			else if (iter->second->m_eType == eOBJ_CHARACTER)
			{
				CCharacterTarget* pTarget = static_cast< CCharacterTarget* >(iter->second);
				DrawCharacter(pTarget, pDraw, pprProjection, colBlend, bHideName);
			}
			else if (iter->second->m_eType == eOBJ_ITEM && g_iShowNameItem > 0)
			{
				CItemTarget* pTarget = static_cast< CItemTarget* >(iter->second);
				DrawItem(pTarget, pDraw, pprProjection);
			}
			else if (iter->second->m_eType == eOBJ_SLAVE)
			{
				CSlaveTarget* pTarget = static_cast< CSlaveTarget* >(iter->second);
				DrawSlave(pTarget, pDraw, pprProjection);
			}
			else if (iter->second->m_eType == eOBJ_PET)
			{
				CPetTarget* pTarget = static_cast< CPetTarget* >(iter->second);
				DrawPet(pTarget, pDraw, pprProjection);
			}
			else if (iter->second->m_eType == eOBJ_WILDPET)
			{
				CWildPetTarget* pTarget = static_cast< CWildPetTarget* >(iter->second);
				DrawWildPet(pTarget, pDraw, pprProjection);
			}
		}
	}
}

void ActorMgr::DrawMob(CMobTarget* pTarget, CDrawPort* pDraw, CProjection3D* pprProjection)
{
	int			nChatMsgLines;
	int			nBoxWidth = 0;
	int			nFontWidth = _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing();
	int			nFontHeight = _pUIFontTexMgr->GetLineHeight();
	int			nTextSX, nTextSY;
	int			nLevelDiff, nColIndex;
	CEntity*	penObject;
	FLOAT3D		vObjectPos, vViewPos, vPopupPos, vObjCenter;
	FLOAT		fRadius, fHeight, fX, fZ, fPopupZ, fSqrDist;	
	FLOAT		fSqrDistance = g_iShowName * OPTION_NAME_DISTANCE;
	fSqrDistance *= fSqrDistance;
	CModelInstance*	pmi = NULL;
	FLOATaabbox3D	boxModel;
	CTString		strName;
	int				nMySyndiType = _pNetwork->MyCharacterInfo.iSyndicateType;
	UIRect		rcName, rcChat;
	

	CUIManager* pUIMgr = CUIManager::getSingleton();
	UtilHelp* pHelp = UtilHelp::getSingleton();

	// Get target mob	
	CMobData* MD = CMobData::getData(pTarget->m_nType);
	nChatMsgLines = pTarget->ChatMsg.GetCount();

	// 공성 진행시 부활진지 이름이 변하므로...
	if (_pNetwork->MyCharacterInfo.sbAttributePos & MATT_WAR)
	{			
		if(pTarget->m_nType >=390 && pTarget->m_nType <=399)
			pTarget->m_strName = MD->GetName();
	}
	// ----------------------------------------------------<<
	// 콤보 바닥 이펙트 NPC 표시 안함
	if (pTarget->m_nType == 491) 
		return;

	if (pTarget->m_nType == 1152 || pTarget->m_nType == 1255) 
		return;

	penObject = pTarget->GetEntity();

	ASSERT( penObject != NULL );
	if( penObject == NULL )
		return;

	if (penObject->IsFlagOff(ENF_ALIVE))
	{
		pUIMgr->StopTargetEffect(pTarget->m_nIdxServer);
		return;
	}

	if (penObject->IsFlagOn(ENF_HIDDEN)&&
		(CEntity::GetPlayerEntity(0)->IsFlagOff(ENF_SHOWHIDDEN) ||
		(CEntity::GetPlayerEntity(0)->IsFlagOn(ENF_SHOWHIDDEN)&&!penObject->IsEnemy())))//ENF_SHOWHIDDEN이면 npc는 볼 수 있다.
		return;

	vObjectPos = penObject->GetLerpedPlacement().pl_PositionVector;

	// Test distance
	fX = vObjectPos(1) - _pNetwork->MyCharacterInfo.x;
	fZ = vObjectPos(3) - _pNetwork->MyCharacterInfo.z;
	fSqrDist = fX * fX + fZ * fZ;
	if( fSqrDist > fSqrDistance )
	{
		pUIMgr->StopTargetEffect(pTarget->m_nIdxServer);
		return;
	}

	// Get frame box
	pmi = penObject->GetModelInstance();
	ASSERT( pmi != NULL );
	if(pmi == NULL)
		return;

	ASSERT( pmi->GetName() != "" );
	if( pmi->GetName() == "" )
		return;
	pmi->GetAllFramesBBox( boxModel );
	boxModel.StretchByVector( pmi->mi_vStretch );
	fHeight = boxModel.maxvect(2) - boxModel.minvect(2);
	fRadius = fHeight * 0.5f;

	// Frustum test
	vObjCenter = vObjectPos;
	vObjCenter(2) += fRadius;
	pprProjection->PreClip( vObjCenter, vViewPos );
	if( pprProjection->TestSphereToFrustum( vViewPos, fRadius ) < 0 )
		return;

	// Object point to screen point
	vObjectPos(2) += fHeight;
	pprProjection->PreClip( vObjectPos, vViewPos );
	pprProjection->PostClip( vViewPos, vPopupPos );
	fPopupZ = ( 1 - pprProjection->pr_fDepthBufferFactor / vViewPos(3) )
		* pprProjection->pr_fDepthBufferMul + pprProjection->pr_fDepthBufferAdd;

	// Get box region
	// [2010/10/20 : Sora] 몬스터 용병 카드
	strName = pTarget->m_strName.c_str();
	if( pTarget->IsMercenary() )
	{
		strName = _S( 5151, "[용병]") + strName;
	}

	if (_iNameType == 0)
	{
		strName.PrintF( "%s I%d S%d L%d", pTarget->m_strName.c_str(), pTarget->GetType(), pTarget->m_nIdxServer, pTarget->m_yLayer );
	}
	//if(g_iCountry == THAILAND) 
#if defined G_THAI
	nBoxWidth = FindThaiLen(strName)+13; //wooss 051017
	//else
#else
	{
		//if(g_iCountry == RUSSIA)
#if defined G_RUSSIA
		nBoxWidth = pDraw->GetTextWidth(strName) + 13; 
		//else
#else
		nBoxWidth = strName.Length() * nFontWidth + 13;
#endif
	}
#endif
	rcName.Left = vPopupPos(1) - nBoxWidth / 2;
	rcName.Right = rcName.Left + nBoxWidth;
	rcName.Bottom = vPopupPos(2) - 7;
	rcName.Top = rcName.Bottom - 15;

	// Set popup texture
	pDraw->InitTextureData( m_ptdPopupTexture, FALSE, PBT_BLEND, TRUE );

	COLOR colPopup =0xFFFFFFFF;
	BOOL bTarget =FALSE;
	BOOL bTargetEffect = TRUE;

	switch (pTarget->m_nType)
	{
	case 484: case 486: // 크리스 마스 트리 
		{//타겟 이펙트가 필요 없다.
			bTargetEffect = FALSE;
		}
	}

	if (bTargetEffect)
	{
		//타겟으로 잡고 있는 NPC의 경우...
		if( pTarget->m_pEntity == INFO()->GetTargetEntity(eTARGET))
		{
			bTarget =TRUE;
			
			//NPC일 경우 테두리를 노란색으로...
			if( pTarget->mob_bNPC || pUIMgr->IsEnemy(pTarget, MOB) == FALSE)
			{
				colPopup =0xFFFF00FF;
				pUIMgr->StartTargetEffect(pTarget->m_nIdxServer, pTarget->m_pEntity, FALSE);
			}
			//몹일 경우 테두리를 빨간색으로...
			else
			{
				colPopup =0xFF0000FF;
				pUIMgr->StartTargetEffect(pTarget->m_nIdxServer, pTarget->m_pEntity, TRUE);
			}
		}
		else
			pUIMgr->StopTargetEffect(pTarget->m_nIdxServer);
	}

	if( bTarget )
	{
		int iAddFrame =3;
		// Add render regions
		pDraw->AddTexture( rcName.Left -iAddFrame, rcName.Top -iAddFrame, rcName.Left + 2, rcName.Bottom +iAddFrame,
			m_rtNameL.U0, m_rtNameL.V0, m_rtNameL.U1, m_rtNameL.V1, colPopup, fPopupZ );
		pDraw->AddTexture( rcName.Left + 2, rcName.Top -iAddFrame, rcName.Right - 2, rcName.Bottom +iAddFrame,
			m_rtNameC.U0, m_rtNameC.V0, m_rtNameC.U1, m_rtNameC.V1, colPopup, fPopupZ );
		pDraw->AddTexture( rcName.Right - 2, rcName.Top -iAddFrame, rcName.Right +iAddFrame, rcName.Bottom +iAddFrame,
			m_rtNameR.U0, m_rtNameR.V0, m_rtNameR.U1, m_rtNameR.V1, colPopup, fPopupZ );

		pDraw->AddTexture( rcName.Left -iAddFrame, rcName.Top -iAddFrame, rcName.Left + 2, rcName.Bottom +iAddFrame,
			m_rtNameL.U0, m_rtNameL.V0, m_rtNameL.U1, m_rtNameL.V1, colPopup, fPopupZ );
		pDraw->AddTexture( rcName.Left + 2, rcName.Top -iAddFrame, rcName.Right - 2, rcName.Bottom +iAddFrame,
			m_rtNameC.U0, m_rtNameC.V0, m_rtNameC.U1, m_rtNameC.V1, colPopup, fPopupZ );
		pDraw->AddTexture( rcName.Right - 2, rcName.Top -iAddFrame, rcName.Right +iAddFrame, rcName.Bottom +iAddFrame,
			m_rtNameR.U0, m_rtNameR.V0, m_rtNameR.U1, m_rtNameR.V1, colPopup, fPopupZ );
	}
	else
	{
		// Add render regions
		pDraw->AddTexture( rcName.Left, rcName.Top, rcName.Left + 2, rcName.Bottom,
			m_rtNameL.U0, m_rtNameL.V0, m_rtNameL.U1, m_rtNameL.V1, colPopup, fPopupZ );
		pDraw->AddTexture( rcName.Left + 2, rcName.Top, rcName.Right - 2, rcName.Bottom,
			m_rtNameC.U0, m_rtNameC.V0, m_rtNameC.U1, m_rtNameC.V1, colPopup, fPopupZ );
		pDraw->AddTexture( rcName.Right - 2, rcName.Top, rcName.Right, rcName.Bottom,
			m_rtNameR.U0, m_rtNameR.V0, m_rtNameR.U1, m_rtNameR.V1, colPopup, fPopupZ );
	}
	// [2011/03/08 : Sora] npc도 채팅 메시지 출력가능하게 수정
	if( nChatMsgLines > 0 )
	{

		if(g_iCountry == THAILAND) { //wooss 051017 이름창 길이 수정
			nBoxWidth = FindThaiLen(pTarget->ChatMsg.GetString(0)); }
		else 
			nBoxWidth = pTarget->ChatMsg.GetWidth();		

		rcChat.Left = vPopupPos(1) - nBoxWidth / 2 - 5;
		rcChat.Right = rcChat.Left + nBoxWidth + 5;
		rcChat.Bottom = rcName.Top - 10 + 5;
		rcChat.Top = rcChat.Bottom - nChatMsgLines * nFontHeight - 5;

		UISUPPORT()->Split9Plane(m_rsChatBG, m_ptdPopupTexture, rcChat, m_uvChatBG, 5, 5);

		m_rsChatBG.RenderRectSurfaceZ(pDraw, DEF_UI_COLOR_WHITE, fPopupZ);
	}
	// 내가 때렸을때 나오는 데미지???
	pUIMgr->ShowDamageList( vPopupPos, fPopupZ, pTarget->m_nIdxClient );

	// Render all elements
	pDraw->FlushRenderingQueue();

	// [sora] RAID_SYSTEM
	if (pTarget->mob_Label >= MSG_EXPED_SETLABEL_INDEX_1 && 
		pTarget->mob_Label <= MSG_EXPED_SETLABEL_INDEX_7) // 타겟 표식 render
	{
		pDraw->InitTextureData(m_ptdExpeditionTexture);

		int nPosX = ((rcName.Left + rcName.Right) / 2 ) - 21;
		pDraw->AddTexture(nPosX, rcName.Top - 42, nPosX + 42, rcName.Top,
			m_rtTargetLabel[pTarget->mob_Label].U0, m_rtTargetLabel[pTarget->mob_Label].V0,
			m_rtTargetLabel[pTarget->mob_Label].U1, m_rtTargetLabel[pTarget->mob_Label].V1,
			0xFFFFFFFF, fPopupZ);

		pDraw->FlushRenderingQueue();
	}

	// Text
	nTextSX = rcName.Left + 7;
	nTextSY = rcName.Top + 1;

	{
		if( pTarget->IsNPC() )
		{
			pDraw->PutTextCharEx( strName, 0, nTextSX, nTextSY,
				pHelp->GetTargetNameColor( 5 ), fPopupZ );
		}
		else if( pTarget->IsMercenary() )	// [2010/10/20 : Sora] 몬스터 용병 카드
		{
			pDraw->PutTextCharEx( strName, 0, nTextSX, nTextSY,
				pHelp->GetTargetNameColor( 12 ), fPopupZ );
		}
		else if( pTarget->IsTotem() || pTarget->IsTrap() || pTarget->IsParasite() )
		{
			pDraw->PutTextCharEx( strName, 0, nTextSX, nTextSY,
				pHelp->GetTargetNameColor( 13 ), fPopupZ );
		}
		else
		{
			nLevelDiff = pTarget->mob_iLevel - _pNetwork->MyCharacterInfo.level;
			if( nLevelDiff > 5 ) nColIndex = 0;
			else if( nLevelDiff > 2 ) nColIndex = 1;
			else if( nLevelDiff > -3 ) nColIndex = 2;
			else if( nLevelDiff > -6 ) nColIndex = 3;
			else nColIndex = 4;
			pDraw->PutTextCharEx( strName, 0, nTextSX, nTextSY,
				pHelp->GetTargetNameColor( nColIndex ), fPopupZ );
		}
	}

	int nMobType = pTarget->GetSyndiType();

	if ( _pNetwork->IsRvrZone() && nMySyndiType && nMySyndiType == nMobType )
	{
		int nIdx = CheckSyndiMark(nMobType, pTarget->GetSyndiGrade());
		DrawSyndiMark(nIdx, pDraw, rcName.Left, rcName.Top, fPopupZ);		
	}

	// [2011/03/08 : Sora] npc도 채팅 메시지 출력가능하게 수정
	if( nChatMsgLines > 0 )
	{
		pTarget->ChatMsg.CheckShowNPCTalk();
		nTextSX = rcChat.Left + 3;
		nTextSY = rcChat.Top + 3;
		for( int i = 0; i < nChatMsgLines; i++ )
		{
			pDraw->PutTextEx( pTarget->ChatMsg.GetString(i), nTextSX, nTextSY,
				pTarget->ChatMsg.GetColor(), fPopupZ );
			nTextSY += nFontHeight;
		}

		__int64	llCurTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();
		if( llCurTime - pTarget->ChatMsg.GetTime() > CHATMSG_TIME_DELAY || fSqrDist > CHATMSG_SQRDIST )
			pTarget->ChatMsg.Reset();
	}

	pDraw->EndTextEx(TRUE);
}

void ActorMgr::DrawCharacter( CCharacterTarget* pTarget, CDrawPort* pDraw, CProjection3D* pprProjection, 
	COLOR colBlend, bool bHideName )
{
	int			nChatMsgLines;
	int			nBoxWidth = 0;
	int			nFontWidth = _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing();
	int			nFontHeight = _pUIFontTexMgr->GetLineHeight();
	int			nTextSX, nTextSY;
	int			nColIndex;
	int			nPopupY = 0;
	int			nShopMsgLines;
	CEntity*	penObject;
	FLOAT3D		vObjectPos, vViewPos, vPopupPos, vObjCenter;
	FLOAT		fRadius, fHeight, fX, fZ, fPopupZ, fSqrDist;	
	FLOAT		fSqrDistance = g_iShowName * OPTION_NAME_DISTANCE;
	fSqrDistance *= fSqrDistance;
	CModelInstance*	pmi = NULL;
	FLOATaabbox3D	boxModel;
	CTString		strName;
	CTString		otherNick;
	int				nMySyndiType = _pNetwork->MyCharacterInfo.iSyndicateType;
	UIRect		rcName, rcChat, rcNickName;
	UIRect		rcGuildName, rcShop;

	CUIManager* pUIMgr = CUIManager::getSingleton();

	penObject = pTarget->m_pEntity;
	nChatMsgLines = pTarget->ChatMsg.GetCount();

	// Invisibility buff
	if( pTarget->cha_statusEffect.GetStatus() & ( 1L << EST_ASSIST_INVISIBLE ) )
		return;

	ASSERT( penObject != NULL );
	if( penObject == NULL )
		return;
	vObjectPos = penObject->GetLerpedPlacement().pl_PositionVector;

	// Test distance
	fX = vObjectPos(1) - _pNetwork->MyCharacterInfo.x;
	fZ = vObjectPos(3) - _pNetwork->MyCharacterInfo.z;
	fSqrDist = fX * fX + fZ * fZ;

	if( nChatMsgLines == 0 && pTarget->cha_sbShopType == PST_NOSHOP &&
		pTarget->GetPkState() == CHA_PVP_STATE_PEACE && fSqrDist > fSqrDistance )
	{
		pUIMgr->StopTargetEffect(pTarget->m_nIdxServer);
		return;
	}

	// Get frame box
	pmi = penObject->GetModelInstance();
	ASSERT( pmi != NULL );
	if(pmi == NULL)
		return;
	ASSERT( pmi->GetName() != "" );
	if( pmi->GetName() == "" )
		return;
	pmi->GetAllFramesBBox( boxModel );
	boxModel.StretchByVector( pmi->mi_vStretch );
	fHeight = boxModel.maxvect(2) - boxModel.minvect(2);
	fRadius = fHeight * 0.5f;

	// Frustum test
	vObjCenter = vObjectPos;
	vObjCenter(2) += fRadius;
	pprProjection->PreClip( vObjCenter, vViewPos );
	if( pprProjection->TestSphereToFrustum( vViewPos, fRadius ) < 0 )
		return;

	// Object point to screen point
	if (pTarget->cha_state & PLAYER_STATE_FLYING)
	{
		vObjectPos(2) += (fHeight * 1.5f);
	}
	else
	{
		vObjectPos(2) += fHeight;
	}

	pprProjection->PreClip( vObjectPos, vViewPos );
	pprProjection->PostClip( vViewPos, vPopupPos );
	fPopupZ = ( 1 - pprProjection->pr_fDepthBufferFactor / vViewPos(3) )
		* pprProjection->pr_fDepthBufferMul + pprProjection->pr_fDepthBufferAdd;

	strName = pTarget->m_strName.c_str();

	if (_iNameType == 0)
	{
		strName.PrintF( "%s S%d", pTarget->m_strName.c_str(), pTarget->m_nIdxServer );
	}

	// Get box region
	//if(g_iCountry == THAILAND) 
#if defined G_THAI
	{ //wooss 051017 캐릭터 이름창 길이 수정
		nBoxWidth = FindThaiLen(strName);
		if( pTarget->GetPkState() == CHA_PVP_STATE_PEACE && !pTarget->IsLegitimate()) 
			nBoxWidth += 13; 
		else 
			nBoxWidth += 27; 
	}
	//else 
#else
	if( pTarget->GetPkState() == CHA_PVP_STATE_PEACE && !pTarget->IsLegitimate())
	{
		//if(g_iCountry == RUSSIA)
#if defined G_RUSSIA
		nBoxWidth = pDraw->GetTextWidth(strName) + 13; 
		//else
#else
		nBoxWidth = strName.Length() * nFontWidth + 13;
#endif
	}
	else
	{
		//if(g_iCountry == RUSSIA)
#if defined G_RUSSIA
		nBoxWidth = pDraw->GetTextWidth(strName) + 27;
		//else
#else
		nBoxWidth = strName.Length() * nFontWidth + 27;
#endif
	}
#endif
	rcName.Left = vPopupPos(1) - nBoxWidth / 2;
	rcName.Right = rcName.Left + nBoxWidth;
	rcName.Bottom = vPopupPos(2) - 7;
	rcName.Top = rcName.Bottom - 15;
	nPopupY = rcName.Top;

	// Set popup texture
	pDraw->InitTextureData( m_ptdPopupTexture, FALSE, PBT_BLEND, TRUE );

	//이름 팝업 테두리색
	COLOR colPopup =0xFFFFFFFF;
	BOOL bTarget =FALSE;
	int iAddFrame =0;
	//타겟으로 잡고 있는 캐릭터의 경우...
	if( pTarget->m_pEntity== INFO()->GetTargetEntity(eTARGET))
	{
		bTarget = TRUE;
		iAddFrame =3;

		if( pUIMgr->IsEnemy(pTarget, CHARACTER) )	//캐릭터가 적이면 테두리를 빨간색으로...
		{
			colPopup =0xFF0000FF;
			pUIMgr->StartTargetEffect( pTarget->m_nIdxServer, pTarget->m_pEntity, TRUE);
		}
		else							//캐릭터가 적이 아니면 노란색으로...
		{
			colPopup =0xFFFF00FF;
			pUIMgr->StartTargetEffect( pTarget->m_nIdxServer, pTarget->m_pEntity, FALSE);
		}
	}
	else
		pUIMgr->StopTargetEffect(pTarget->m_nIdxServer);

#ifdef PREMIUM_CHAR
	if (pTarget->GetPremiumType() == PREMIUM_CHAR_TYPE_FIRST)
	{
		UIRect			rcPcName;
		rcPcName.Right = rcName.Right + 30;

		if (bTarget)
		{
			rcPcName.Left = rcName.Left - 3;
			rcPcName.Top = rcName.Top - 12;
			rcPcName.Bottom = rcName.Bottom + 18;

			if( pTarget->GetPkState() == CHA_PVP_STATE_RELEASE || pTarget->IsLegitimate())
				rcPcName.Right = rcName.Right + 31;

			pDraw->AddTexture( rcPcName.Left, rcPcName.Top, rcPcName.Left + 2, rcPcName.Bottom,
				m_rtSelPcNameL.U0, m_rtSelPcNameL.V0, m_rtSelPcNameL.U1, m_rtSelPcNameL.V1, 0xFFFFFFFF, fPopupZ );
			pDraw->AddTexture( rcPcName.Left + 2, rcPcName.Top, rcPcName.Right - 63, rcPcName.Bottom,
				m_rtSelPcNameC.U0, m_rtSelPcNameC.V0, m_rtSelPcNameC.U1, m_rtSelPcNameC.V1, 0xFFFFFFFF, fPopupZ );
			pDraw->AddTexture( rcPcName.Right - 63, rcPcName.Top, rcPcName.Right, rcPcName.Bottom,
				m_rtSelPcNameR.U0, m_rtSelPcNameR.V0, m_rtSelPcNameR.U1, m_rtSelPcNameR.V1, 0xFFFFFFFF, fPopupZ );
		}
		else
		{
			rcPcName.Left = rcName.Left - 2;
			rcPcName.Top = rcName.Top - 11;
			rcPcName.Bottom = rcName.Bottom + 24;

			pDraw->AddTexture( rcPcName.Left, rcPcName.Top, rcPcName.Left + 2, rcPcName.Bottom,
				m_rtPcNameL.U0, m_rtPcNameL.V0, m_rtPcNameL.U1, m_rtPcNameL.V1, 0xFFFFFFFF, fPopupZ );
			pDraw->AddTexture( rcPcName.Left + 2, rcPcName.Top, rcPcName.Right - 63, rcPcName.Bottom,
				m_rtPcNameC.U0, m_rtPcNameC.V0, m_rtPcNameC.U1, m_rtPcNameC.V1, 0xFFFFFFFF, fPopupZ );
			pDraw->AddTexture( rcPcName.Right - 63, rcPcName.Top, rcPcName.Right, rcPcName.Bottom,
				m_rtPcNameR.U0, m_rtPcNameR.V0, m_rtPcNameR.U1, m_rtPcNameR.V1, 0xFFFFFFFF, fPopupZ );
		}

		nPopupY = rcName.Top - 5;
	}
#endif	//	PREMIUM_CHAR

	// Add render regions
	if( pTarget->IsLegitimate() )
	{
		if( bTarget )
		{
			pDraw->AddTexture( rcName.Left -iAddFrame, rcName.Top -iAddFrame, rcName.Left + 2, rcName.Bottom +iAddFrame,
				m_rtNameL.U0, m_rtNameL.V0, m_rtNameL.U1, m_rtNameL.V1, colPopup, fPopupZ );
			pDraw->AddTexture( rcName.Left + 2, rcName.Top -iAddFrame, rcName.Right - 16, rcName.Bottom +iAddFrame,
				m_rtNameC.U0, m_rtNameC.V0, m_rtNameC.U1, m_rtNameC.V1, colPopup, fPopupZ );
			pDraw->AddTexture( rcName.Right - 16, rcName.Top -iAddFrame, rcName.Right +iAddFrame, rcName.Bottom +iAddFrame,
				m_rtNameRDefPK2.U0, m_rtNameRDefPK2.V0, m_rtNameRDefPK2.U1, m_rtNameRDefPK2.V1, colPopup, fPopupZ );
		}
		pDraw->AddTexture( rcName.Left -iAddFrame, rcName.Top -iAddFrame, rcName.Left + 2, rcName.Bottom +iAddFrame,
			m_rtNameL.U0, m_rtNameL.V0, m_rtNameL.U1, m_rtNameL.V1, colPopup, fPopupZ );
		pDraw->AddTexture( rcName.Left + 2, rcName.Top -iAddFrame, rcName.Right - 16, rcName.Bottom +iAddFrame,
			m_rtNameC.U0, m_rtNameC.V0, m_rtNameC.U1, m_rtNameC.V1, colPopup, fPopupZ );
		pDraw->AddTexture( rcName.Right - 16, rcName.Top -iAddFrame, rcName.Right +iAddFrame, rcName.Bottom +iAddFrame,
			m_rtNameRDefPK2.U0, m_rtNameRDefPK2.V0, m_rtNameRDefPK2.U1, m_rtNameRDefPK2.V1, colPopup, fPopupZ );
	}
	else if( pTarget->GetPkState() == CHA_PVP_STATE_PEACE )
	{
		if( bTarget )
		{
			pDraw->AddTexture( rcName.Left -iAddFrame, rcName.Top -iAddFrame, rcName.Left + 2, rcName.Bottom+iAddFrame,
				m_rtNameL.U0, m_rtNameL.V0, m_rtNameL.U1, m_rtNameL.V1, colPopup, fPopupZ );
			pDraw->AddTexture( rcName.Left + 2, rcName.Top -iAddFrame, rcName.Right - 2, rcName.Bottom +iAddFrame,
				m_rtNameC.U0, m_rtNameC.V0, m_rtNameC.U1, m_rtNameC.V1, colPopup, fPopupZ );
			pDraw->AddTexture( rcName.Right - 2, rcName.Top -iAddFrame, rcName.Right +iAddFrame, rcName.Bottom +iAddFrame,
				m_rtNameR.U0, m_rtNameR.V0, m_rtNameR.U1, m_rtNameR.V1, colPopup, fPopupZ );
		}
		pDraw->AddTexture( rcName.Left -iAddFrame, rcName.Top -iAddFrame, rcName.Left + 2, rcName.Bottom+iAddFrame,
			m_rtNameL.U0, m_rtNameL.V0, m_rtNameL.U1, m_rtNameL.V1, colPopup, fPopupZ );
		pDraw->AddTexture( rcName.Left + 2, rcName.Top -iAddFrame, rcName.Right - 2, rcName.Bottom +iAddFrame,
			m_rtNameC.U0, m_rtNameC.V0, m_rtNameC.U1, m_rtNameC.V1, colPopup, fPopupZ );
		pDraw->AddTexture( rcName.Right - 2, rcName.Top -iAddFrame, rcName.Right +iAddFrame, rcName.Bottom +iAddFrame,
			m_rtNameR.U0, m_rtNameR.V0, m_rtNameR.U1, m_rtNameR.V1, colPopup, fPopupZ );
	}
	else		// PK
	{
		if( pTarget->GetPkState() == CHA_PVP_STATE_RELEASE )
			colPopup= colBlend;

		if( bTarget )
		{
			pDraw->AddTexture( rcName.Left -iAddFrame, rcName.Top -iAddFrame, rcName.Left + 2, rcName.Bottom +iAddFrame,
				m_rtNameL.U0, m_rtNameL.V0, m_rtNameL.U1, m_rtNameL.V1, colPopup, fPopupZ );
			pDraw->AddTexture( rcName.Left + 2, rcName.Top -iAddFrame, rcName.Right - 16, rcName.Bottom +iAddFrame,
				m_rtNameC.U0, m_rtNameC.V0, m_rtNameC.U1, m_rtNameC.V1, colPopup, fPopupZ );
			pDraw->AddTexture( rcName.Right - 16, rcName.Top -iAddFrame, rcName.Right +iAddFrame, rcName.Bottom +iAddFrame,
				m_rtNameRPK.U0, m_rtNameRPK.V0, m_rtNameRPK.U1, m_rtNameRPK.V1, colPopup, fPopupZ );
		}

		pDraw->AddTexture( rcName.Left -iAddFrame, rcName.Top -iAddFrame, rcName.Left + 2, rcName.Bottom +iAddFrame,
			m_rtNameL.U0, m_rtNameL.V0, m_rtNameL.U1, m_rtNameL.V1, colPopup, fPopupZ );
		pDraw->AddTexture( rcName.Left + 2, rcName.Top -iAddFrame, rcName.Right - 16, rcName.Bottom +iAddFrame,
			m_rtNameC.U0, m_rtNameC.V0, m_rtNameC.U1, m_rtNameC.V1, colPopup, fPopupZ );
		pDraw->AddTexture( rcName.Right - 16, rcName.Top -iAddFrame, rcName.Right +iAddFrame, rcName.Bottom +iAddFrame,
			m_rtNameRPK.U0, m_rtNameRPK.V0, m_rtNameRPK.U1, m_rtNameRPK.V1, colPopup, fPopupZ );
	}

	// 호칭
	if( pTarget->cha_NickType > 0)
	{
		COLOR otherNickColor;
		INDEX iotherItemIndex;
		//  [3/25/2010 kiny8216] 호칭 이름을 아이템 lod에서 불러오도록 변경
		iotherItemIndex = CNickNameData::getData(pTarget->cha_NickType)->GetItemIndex();
		otherNick  = pUIMgr->GetNickName()->GetName(iotherItemIndex);
		COLOR tempColor = CNickNameData::getData(pTarget->cha_NickType)->GetBGColor();
		otherNickColor = (tempColor|255);

#if defined G_RUSSIA
		nBoxWidth = pDraw->GetTextWidth(otherNick) + 13; 
#else
		nBoxWidth = otherNick.Length() * nFontWidth + 13;
#endif		
		rcNickName.Left = vPopupPos(1) - nBoxWidth / 2;
		rcNickName.Right = rcNickName.Left + nBoxWidth;			
		rcNickName.Bottom = nPopupY - 5;
		rcNickName.Top = (rcNickName.Bottom - 1 * nFontHeight) - 4;
		nPopupY = rcNickName.Top;

		pDraw->AddTexture( rcNickName.Left, rcNickName.Top, rcNickName.Left + 2, rcNickName.Bottom,
			m_rtNickNameL.U0, m_rtNickNameL.V0, m_rtNickNameL.U1, m_rtNickNameL.V1, otherNickColor, fPopupZ );
		pDraw->AddTexture( rcNickName.Left + 2, rcNickName.Top, rcNickName.Right - 2, rcNickName.Bottom,
			m_rtNickNameC.U0, m_rtNickNameC.V0, m_rtNickNameC.U1, m_rtNickNameC.V1, otherNickColor, fPopupZ );
		pDraw->AddTexture( rcNickName.Right - 2, rcNickName.Top, rcNickName.Right, rcNickName.Bottom,
			m_rtNickNameR.U0, m_rtNickNameR.V0, m_rtNickNameR.U1, m_rtNickNameR.V1, otherNickColor, fPopupZ );
	}
	// Guild Name
	if( pTarget->cha_lGuildIndex > 0)
	{
		//if(g_iCountry == THAILAND) { //wooss 051017 캐릭터 이름창 길이 수정
#if defined G_THAI
		nBoxWidth = FindThaiLen(pTarget->cha_strGuildName)+13;
		//else 
#else
		{
			//if(g_iCountry == RUSSIA)
#if defined G_RUSSIA
			nBoxWidth = pDraw->GetTextWidth(pTarget->cha_strGuildName) + 13;
			//else
#else
			nBoxWidth = pTarget->cha_strGuildName.Length() * nFontWidth + 13;	
#endif
		}
#endif
		rcGuildName.Left = vPopupPos(1) - nBoxWidth / 2;
		rcGuildName.Right = rcGuildName.Left + nBoxWidth;			
		rcGuildName.Bottom = nPopupY - 5;
		rcGuildName.Top = (rcGuildName.Bottom - 1 * nFontHeight) - 4;
		nPopupY = rcGuildName.Top;

		pDraw->AddTexture( rcGuildName.Left, rcGuildName.Top, rcGuildName.Left + 2, rcGuildName.Bottom,
			m_rtNameL, 0xFFFFFFFF, fPopupZ );
		pDraw->AddTexture( rcGuildName.Left + 2, rcGuildName.Top, rcGuildName.Right - 2, rcGuildName.Bottom,
			m_rtNameC, 0xFFFFFFFF, fPopupZ );
		pDraw->AddTexture( rcGuildName.Right - 2, rcGuildName.Top, rcGuildName.Right, rcGuildName.Bottom,
			m_rtNameR, 0xFFFFFFFF, fPopupZ );

		// WSS_GUILDMASTER 070517 -------------------------->>	
		pUIMgr->DrawGuildRankBox(rcGuildName, pTarget->cha_sbGuildRank, fPopupZ);
		// -------------------------------------------------<<

	}

	// GPS 출력
	{
		if (GAMEDATAMGR()->GetGPS()->IsPosition() == true &&
			GAMEDATAMGR()->GetGPS()->getTargetID() == pTarget->GetSIndex())
		{
			UIRect	rc, rcBG;

			rc.Left = vPopupPos(1) - (44 / 2);
			rc.Right = rc.Left + 44;
			rc.Top = nPopupY - 71;
			rc.Bottom = rc.Top + 71;

			rcBG = rc;
			rcBG.Left -= 23;
			rcBG.Right = rcBG.Left + 88;
			rcBG.Top -= 6;
			rcBG.Bottom = rcBG.Top + 97;

			pDraw->AddTexture(rcBG.Left, rcBG.Top, rcBG.Right, rcBG.Bottom, m_uvGPSBG, m_colGPS, fPopupZ);
			pDraw->AddTexture(rc.Left, rc.Top, rc.Right, rc.Bottom, m_uvGPS, DEF_UI_COLOR_WHITE, fPopupZ);
		}
	}

	nShopMsgLines = 0;
	if( pTarget->cha_sbShopType != PST_NOSHOP )
	{
		nShopMsgLines = pTarget->ShopMsg.GetCount();
		//if(g_iCountry == THAILAND) { //wooss 051017 이름창 길이 수정
#if defined G_THAI
		nBoxWidth = FindThaiLen(pTarget->ShopMsg.GetString(0));
		//else 
#else
		{
			//if(g_iCountry == RUSSIA)
#if defined G_RUSSIA
			nBoxWidth = pDraw->GetTextWidth(pTarget->ShopMsg.GetString(0));
			//else
#else
			nBoxWidth = pTarget->ShopMsg.GetWidth();
#endif
		}
#endif
		rcShop.Left = vPopupPos(1) - nBoxWidth / 2;
		rcShop.Right = rcShop.Left + nBoxWidth;

		if( pTarget->cha_sbShopType & PST_PREMIUM )
		{
			rcShop.Bottom = nPopupY - 18;
			rcShop.Top = rcShop.Bottom - nShopMsgLines * nFontHeight;

			pDraw->AddTexture( rcShop.Left - 13, rcShop.Top - 13, rcShop.Left, rcShop.Top,
				m_rtShopPremLU.U0, m_rtShopPremLU.V0, m_rtShopPremLU.U1, m_rtShopPremLU.V1, 0xFFFFFFFF, fPopupZ );
			pDraw->AddTexture( rcShop.Left, rcShop.Top - 13, rcShop.Right, rcShop.Top,
				m_rtShopPremUp.U0, m_rtShopPremUp.V0, m_rtShopPremUp.U1, m_rtShopPremUp.V1, 0xFFFFFFFF, fPopupZ );
			pDraw->AddTexture( rcShop.Right, rcShop.Top - 13, rcShop.Right + 13, rcShop.Top,
				m_rtShopPremRU.U0, m_rtShopPremRU.V0, m_rtShopPremRU.U1, m_rtShopPremRU.V1, 0xFFFFFFFF, fPopupZ );
			pDraw->AddTexture( rcShop.Left - 13, rcShop.Top, rcShop.Left, rcShop.Bottom,
				m_rtShopPremL.U0, m_rtShopPremL.V0, m_rtShopPremL.U1, m_rtShopPremL.V1, 0xFFFFFFFF, fPopupZ );
			pDraw->AddTexture( rcShop.Left, rcShop.Top, rcShop.Right, rcShop.Bottom,
				m_rtShopPremC.U0, m_rtShopPremC.V0, m_rtShopPremC.U1, m_rtShopPremC.V1, 0xFFFFFFFF, fPopupZ );
			pDraw->AddTexture( rcShop.Right, rcShop.Top, rcShop.Right + 13, rcShop.Bottom,
				m_rtShopPremR.U0, m_rtShopPremR.V0, m_rtShopPremR.U1, m_rtShopPremR.V1, 0xFFFFFFFF, fPopupZ );
			pDraw->AddTexture( rcShop.Left - 13, rcShop.Bottom, rcShop.Left, rcShop.Bottom + 13,
				m_rtShopPremLL.U0, m_rtShopPremLL.V0, m_rtShopPremLL.U1, m_rtShopPremLL.V1, 0xFFFFFFFF, fPopupZ );
			pDraw->AddTexture( rcShop.Left, rcShop.Bottom, rcShop.Right, rcShop.Bottom + 13,
				m_rtShopPremLo.U0, m_rtShopPremLo.V0, m_rtShopPremLo.U1, m_rtShopPremLo.V1, 0xFFFFFFFF, fPopupZ );
			pDraw->AddTexture( rcShop.Right, rcShop.Bottom, rcShop.Right + 13, rcShop.Bottom + 13,
				m_rtShopPremRL.U0, m_rtShopPremRL.V0, m_rtShopPremRL.U1, m_rtShopPremRL.V1, 0xFFFFFFFF, fPopupZ );

			nPopupY = rcShop.Top - 8;
		}
		else
		{
			rcShop.Bottom = nPopupY - 10;
			rcShop.Top = rcShop.Bottom - nShopMsgLines * nFontHeight;

			pDraw->AddTexture( rcShop.Left - 5, rcShop.Top - 5, rcShop.Left, rcShop.Top,
				m_rtShopLU.U0, m_rtShopLU.V0, m_rtShopLU.U1, m_rtShopLU.V1, 0xFFFFFFFF, fPopupZ );
			pDraw->AddTexture( rcShop.Left, rcShop.Top - 5, rcShop.Right, rcShop.Top,
				m_rtShopUp.U0, m_rtShopUp.V0, m_rtShopUp.U1, m_rtShopUp.V1, 0xFFFFFFFF, fPopupZ );
			pDraw->AddTexture( rcShop.Right, rcShop.Top - 5, rcShop.Right + 5, rcShop.Top,
				m_rtShopRU.U0, m_rtShopRU.V0, m_rtShopRU.U1, m_rtShopRU.V1, 0xFFFFFFFF, fPopupZ );
			pDraw->AddTexture( rcShop.Left - 5, rcShop.Top, rcShop.Left, rcShop.Bottom,
				m_rtShopL.U0, m_rtShopL.V0, m_rtShopL.U1, m_rtShopL.V1, 0xFFFFFFFF, fPopupZ );
			pDraw->AddTexture( rcShop.Left, rcShop.Top, rcShop.Right, rcShop.Bottom,
				m_rtShopC.U0, m_rtShopC.V0, m_rtShopC.U1, m_rtShopC.V1, 0xFFFFFFFF, fPopupZ );
			pDraw->AddTexture( rcShop.Right, rcShop.Top, rcShop.Right + 5, rcShop.Bottom,
				m_rtShopR.U0, m_rtShopR.V0, m_rtShopR.U1, m_rtShopR.V1, 0xFFFFFFFF, fPopupZ );
			pDraw->AddTexture( rcShop.Left - 5, rcShop.Bottom, rcShop.Left, rcShop.Bottom + 5,
				m_rtShopLL.U0, m_rtShopLL.V0, m_rtShopLL.U1, m_rtShopLL.V1, 0xFFFFFFFF, fPopupZ );
			pDraw->AddTexture( rcShop.Left, rcShop.Bottom, rcShop.Right, rcShop.Bottom + 5,
				m_rtShopLo.U0, m_rtShopLo.V0, m_rtShopLo.U1, m_rtShopLo.V1, 0xFFFFFFFF, fPopupZ );
			pDraw->AddTexture( rcShop.Right, rcShop.Bottom, rcShop.Right + 5, rcShop.Bottom + 5,
				m_rtShopRL.U0, m_rtShopRL.V0, m_rtShopRL.U1, m_rtShopRL.V1, 0xFFFFFFFF, fPopupZ );

			nPopupY = rcShop.Top;
		}
	}

	if( nChatMsgLines > 0 )
	{
		//if(g_iCountry == THAILAND) { //wooss 051017 이름창 길이 수정
#if defined G_THAI
		nBoxWidth = FindThaiLen(pTarget->ChatMsg.GetString(0));
		//else 
#else
		{
			//if(g_iCountry == RUSSIA)
#if defined G_RUSSIA
			int nCurWidth = 0;
			int nMaxWidth = 0;
			for( int i=0; i<pTarget->ChatMsg.GetCount(); ++i )
			{
				nCurWidth = pDraw->GetTextWidth(pTarget->ChatMsg.GetString(i));
				if( nMaxWidth < nCurWidth )
				{
					nMaxWidth = nCurWidth;
				}
			}
			nBoxWidth = nMaxWidth;
			//else
#else
			nBoxWidth = pTarget->ChatMsg.GetWidth();
#endif
		}
#endif
		rcChat.Left = vPopupPos(1) - nBoxWidth / 2;
		rcChat.Right = rcChat.Left + nBoxWidth;
		rcChat.Bottom = nPopupY - 10;
		rcChat.Top = rcChat.Bottom - nChatMsgLines * nFontHeight;
		nPopupY = rcChat.Top;

		pDraw->AddTexture( rcChat.Left - 5, rcChat.Top - 5, rcChat.Left, rcChat.Top,
			m_rtChatLU.U0, m_rtChatLU.V0, m_rtChatLU.U1, m_rtChatLU.V1, 0xFFFFFFFF, fPopupZ );
		pDraw->AddTexture( rcChat.Left, rcChat.Top - 5, rcChat.Right, rcChat.Top,
			m_rtChatUp.U0, m_rtChatUp.V0, m_rtChatUp.U1, m_rtChatUp.V1, 0xFFFFFFFF, fPopupZ );
		pDraw->AddTexture( rcChat.Right, rcChat.Top - 5, rcChat.Right + 5, rcChat.Top,
			m_rtChatRU.U0, m_rtChatRU.V0, m_rtChatRU.U1, m_rtChatRU.V1, 0xFFFFFFFF, fPopupZ );
		pDraw->AddTexture( rcChat.Left - 5, rcChat.Top, rcChat.Left, rcChat.Bottom,
			m_rtChatL.U0, m_rtChatL.V0, m_rtChatL.U1, m_rtChatL.V1, 0xFFFFFFFF, fPopupZ );
		pDraw->AddTexture( rcChat.Left, rcChat.Top, rcChat.Right, rcChat.Bottom,
			m_rtChatC.U0, m_rtChatC.V0, m_rtChatC.U1, m_rtChatC.V1, 0xFFFFFFFF, fPopupZ );
		pDraw->AddTexture( rcChat.Right, rcChat.Top, rcChat.Right + 5, rcChat.Bottom,
			m_rtChatR.U0, m_rtChatR.V0, m_rtChatR.U1, m_rtChatR.V1, 0xFFFFFFFF, fPopupZ );
		pDraw->AddTexture( rcChat.Left - 5, rcChat.Bottom, rcChat.Left, rcChat.Bottom + 5,
			m_rtChatLL.U0, m_rtChatLL.V0, m_rtChatLL.U1, m_rtChatLL.V1, 0xFFFFFFFF, fPopupZ );
		pDraw->AddTexture( rcChat.Left, rcChat.Bottom, rcChat.Right, rcChat.Bottom + 5,
			m_rtChatLo.U0, m_rtChatLo.V0, m_rtChatLo.U1, m_rtChatLo.V1, 0xFFFFFFFF, fPopupZ );
		pDraw->AddTexture( rcChat.Right, rcChat.Bottom, rcChat.Right + 5, rcChat.Bottom + 5,
			m_rtChatRL.U0, m_rtChatRL.V0, m_rtChatRL.U1, m_rtChatRL.V1, 0xFFFFFFFF, fPopupZ );
	}

	// 내가 때렸을때 나오는 데미지???
	pUIMgr->ShowDamageList( vPopupPos, fPopupZ, pTarget->m_nIdxClient );

	// Render all elements
	pDraw->FlushRenderingQueue();

	// [sora] RAID_SYSTEM
	if(pTarget->cha_Label >= MSG_EXPED_SETLABEL_INDEX_1 && pTarget->cha_Label <= MSG_EXPED_SETLABEL_INDEX_7) // 타겟 표식 render
	{
		pDraw->InitTextureData(m_ptdExpeditionTexture);

		int nPosX = ((rcName.Left + rcName.Right) / 2 ) - 21;
		int nPosY = 0;

		if(nChatMsgLines > 0)
		{
			nPosY = rcChat.Top - 3;
		}
		else if(pTarget->cha_lGuildIndex > 0)	// 소속 길드가 있을경우 길드명 위에 표시
		{
			nPosY = rcGuildName.Top;
		}
		else if (pTarget->cha_NickType > 0)	// 호칭이 있는 경우 호칭 위에 표시
		{
			nPosY = rcNickName.Top;
		}
		else
		{
			nPosY = rcName.Top;
		}

		pDraw->AddTexture(nPosX, nPosY - 42, nPosX + 42, nPosY,
			m_rtTargetLabel[pTarget->cha_Label].U0, m_rtTargetLabel[pTarget->cha_Label].V0,
			m_rtTargetLabel[pTarget->cha_Label].U1, m_rtTargetLabel[pTarget->cha_Label].V1,
			0xFFFFFFFF, fPopupZ);

		pDraw->FlushRenderingQueue();
	}
	// [sora] GUILD_MARK
#ifdef GUILD_MARK
	if (pTarget->pGuildmark != NULL)
	{
		//pUIMgr->GetGuildMark()->RenderGuildMark( rcGuildName.Left - 17, rcGuildName.Top, 15, pTarget->guildMark, fPopupZ );
		pTarget->pGuildmark->SetPos(rcGuildName.Left - 17, rcGuildName.Top);
		pTarget->pGuildmark->setZ(fPopupZ);
		pTarget->pGuildmark->Render(pDraw);		
	}
#endif

	if (_pNetwork->IsRvrZone())
	{
		int nChaType = pTarget->GetSyndiType();
		int nChaGrade = pTarget->GetSyndiGrade();
		int nIdx = CheckSyndiMark( nChaType, nChaGrade);

		// 내가 결사대에 가입 되어 있고, 내 결사대 타입과 상대 결사대 타입이 같거나,
		// 상대방 결사대 타입이 none이 아닐때 마크를 뿌려준다.
		if ( nMySyndiType && nMySyndiType == nChaType || 
			(nIdx > eMARK_KAY_NONE && nIdx != eMARK_DEAL_NONE))
		{
			DrawSyndiMark(nIdx, pDraw, rcName.Left, rcName.Top, fPopupZ);
		}
	}


	// Text
	nTextSX = rcName.Left + 7;
	nTextSY = rcName.Top + 1;
	// Title
#ifdef NEW_CHAO_SYS
	//남의 것
	BOOL buseblindchao = TRUE;
	if(pTarget->IsBuffBySkill(1395))
		buseblindchao = FALSE;
	if (pTarget->cha_pkstate > 19000 && pTarget->cha_pkstate <= 32000 && buseblindchao)
		nColIndex = 14;
	else if(pTarget->cha_pkstate > 6000 && pTarget->cha_pkstate <= 19000 && buseblindchao)
		nColIndex = 15;
	else if(pTarget->cha_pkstate > 0 && pTarget->cha_pkstate <= 6000 && buseblindchao)
		nColIndex = 16;
	else if(pTarget->cha_pkstate >= -6000 && pTarget->cha_pkstate < 0 && buseblindchao)
		nColIndex = 17;
	else if(pTarget->cha_pkstate >= -19000 && pTarget->cha_pkstate < -6000 && buseblindchao)
		nColIndex = 18;
	else if(pTarget->cha_pkstate >= -32000 && pTarget->cha_pkstate < -19000 && buseblindchao)
		nColIndex = 19;
	else nColIndex = 9;
#else
	if( pTarget->cha_pkstate < -9 ) nColIndex = 11;
	else if( pTarget->cha_pkstate > 9 ) nColIndex = 7;
	else nColIndex = 9;
	// PK
	if( pTarget->GetPkState() != CHA_PVP_STATE_PEACE )
		nColIndex--;
#endif	
	if(pTarget->cha_sbPresscorps > 0)
	{
		pDraw->PutTextEx( strName, nTextSX, nTextSY,
			0x00C80FFF, fPopupZ );
	}
	else
	{
		pDraw->PutTextEx( strName, nTextSX, nTextSY,
			UTIL_HELP()->GetTargetNameColor( nColIndex ), fPopupZ );
	}

	// 호칭
	nTextSX = rcNickName.Left + 7;
	nTextSY = rcNickName.Top + 2;

	if( pTarget->cha_NickType > 0)
	{
		COLOR tmpColor1 = CNickNameData::getData(pTarget->cha_NickType)->GetColor();

		pDraw->PutTextEx(otherNick, nTextSX, nTextSY, tmpColor1, fPopupZ );
	}
	// Text
	nTextSX = rcGuildName.Left + 7;
	nTextSY = rcGuildName.Top + 2;

	// Guild Name
	if( pTarget->cha_lGuildIndex > 0)
	{
		// WSS_GUILDMASTER 070517 --------------------------->>
		//[071123: Su-won] DRATAN_SIEGE_DUNGEON
		//길드 이름 색 설정
		COLOR colGuildName =0xD6A4D6FF;			//일반 길드

		if( pTarget->cha_ubGuildNameColor == 1)	//메라크 성주 길드
			colGuildName = 0xFF4500FF;
		else if( pTarget->cha_ubGuildNameColor == 2)	//드라탄 성주 길드
			colGuildName = 0xFFD700FF;

		// --------------------------------------------------<<
		pDraw->PutTextEx( pTarget->cha_strGuildName, nTextSX, nTextSY,
			colGuildName, fPopupZ );
	}

	if( nShopMsgLines > 0 )
	{
		nTextSX = rcShop.Left + 1;
		nTextSY = rcShop.Top + 1;
		for( int i = 0; i < nShopMsgLines; i++ )
		{
			pDraw->PutTextEx( pTarget->ShopMsg.GetString( i ), nTextSX, nTextSY,
				pTarget->ShopMsg.GetColor(), fPopupZ );
			nTextSY += nFontHeight;
		}
	}

	if( nChatMsgLines > 0 )
	{
		nTextSX = rcChat.Left + 1;
		nTextSY = rcChat.Top + 1;
		for( int i = 0; i < nChatMsgLines; i++ )
		{
			pDraw->PutTextEx( pTarget->ChatMsg.GetString( i ), nTextSX, nTextSY,
				pTarget->ChatMsg.GetColor(), fPopupZ );
			nTextSY += nFontHeight;
		}

		__int64	llCurTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();
		if( llCurTime - pTarget->ChatMsg.GetTime() > CHATMSG_TIME_DELAY || fSqrDist > CHATMSG_SQRDIST )
			pTarget->ChatMsg.Reset();
	}

	// Flush all render text queue
	pDraw->EndTextEx( TRUE );
}

void ActorMgr::DrawItem( CItemTarget* pTarget, CDrawPort* pDraw, CProjection3D* pprProjection )
{
	int			nBoxWidth = 0;
	int			nFontWidth = _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing();
	int			nFontHeight = _pUIFontTexMgr->GetLineHeight();
	int			nTextSX, nTextSY;
	CEntity*	penObject;
	FLOAT3D		vObjectPos, vViewPos, vPopupPos, vObjCenter;
	FLOAT		fRadius, fHeight, fX, fZ, fPopupZ, fSqrDist;	
	FLOAT		fSqrDistance = g_iShowName * OPTION_NAME_DISTANCE;
	fSqrDistance *= fSqrDistance;

	FLOAT			fSqrDistanceItem = g_iShowNameItem * OPTION_NAME_DISTANCE;
	fSqrDistanceItem *= fSqrDistanceItem;

	CModelInstance*	pmi = NULL;
	FLOATaabbox3D	boxModel;
	CTString		strName, strTemp;
	int				nMySyndiType = _pNetwork->MyCharacterInfo.iSyndicateType;
	UIRect		rcName, rcChat;


	CUIManager* pUIMgr = CUIManager::getSingleton();

	penObject = pTarget->GetEntity();

	if( penObject == NULL )
		return;

	vObjectPos = penObject->en_plPlacement.pl_PositionVector;

	// Test distance
	fX = vObjectPos(1) - _pNetwork->MyCharacterInfo.x;
	fZ = vObjectPos(3) - _pNetwork->MyCharacterInfo.z;
	fSqrDist = fX * fX + fZ * fZ;

	if( fSqrDist > fSqrDistanceItem )
		return;

	// Get frame box
	pmi = penObject->GetModelInstance();
	ASSERT( pmi != NULL );
	if(pmi == NULL)
		return;

	ASSERT( pmi->GetName() != "" );
	if( pmi->GetName() == "" )
		return;

	pmi->GetAllFramesBBox( boxModel );
	boxModel.StretchByVector( pmi->mi_vStretch );
	fHeight = boxModel.maxvect(2) - boxModel.minvect(2);
	fRadius = fHeight * 0.5f;

	// Frustum test
	vObjCenter = vObjectPos;
	vObjCenter(2) += fRadius;
	pprProjection->PreClip( vObjCenter, vViewPos );
	if( pprProjection->TestSphereToFrustum( vViewPos, fRadius ) < 0 )
		return;

	// Object point to screen point
	vObjectPos(2) += fHeight;
	pprProjection->PreClip( vObjectPos, vViewPos );
	pprProjection->PostClip( vViewPos, vPopupPos );
	fPopupZ = ( 1 - pprProjection->pr_fDepthBufferFactor / vViewPos(3) )
		* pprProjection->pr_fDepthBufferMul + pprProjection->pr_fDepthBufferAdd;

	COLOR colNas = UTIL_HELP()->GetTargetNameColor( 2 );

	// Get box region
	if( pTarget->item_llCount > 1 )
	{
		strTemp.PrintF( "%I64d", pTarget->item_llCount );
		strName.PrintF( "%s(%s)", pTarget->m_strName.c_str(), strTemp );
		pUIMgr->InsertCommaToString( strTemp );
		colNas = pUIMgr->GetNasColor(  pTarget->item_llCount );
	}
	else
	{
		strName = pTarget->m_strName.c_str();
	}

	if (_iNameType == 0)
	{
		if (pTarget->item_llCount > 1)
		{
			strTemp.PrintF( "%I64d", pTarget->item_llCount );
			pUIMgr->InsertCommaToString( strTemp );
			colNas = pUIMgr->GetNasColor( pTarget->item_llCount );
			strName.PrintF( "%s(%s) S%d", pTarget->m_strName.c_str(), strTemp, pTarget->m_nIdxServer );
		}
		else
		{
			strName.PrintF( "%s S%d", pTarget->m_strName.c_str(), pTarget->m_nIdxServer );
		}
	}

	//			if(g_iCountry == THAILAND) 
#if defined G_THAI
	nBoxWidth = FindThaiLen(strName)+13; //wooss 051017
	//else 
#else
	{
		//if(g_iCountry == RUSSIA)
#if defined G_RUSSIA
		nBoxWidth = pDraw->GetTextWidth(strName) + 13;
		//else
#else
		nBoxWidth = strName.Length() * nFontWidth + 13;
#endif
	}
#endif
	rcName.Left = vPopupPos(1) - nBoxWidth / 2;
	rcName.Right = rcName.Left + nBoxWidth;
	rcName.Bottom = vPopupPos(2) - 7;
	rcName.Top = rcName.Bottom - 15;

	// Set popup texture
	// 텍스춰 배경.
// 	pDraw->InitTextureData( m_ptdPopupTexture, FALSE, PBT_BLEND, TRUE );
// 
// 	// Add render regions
// 	pDraw->AddTexture( rcName.Left, rcName.Top, rcName.Left + 2, rcName.Bottom,
// 		m_rtNameL.U0, m_rtNameL.V0, m_rtNameL.U1, m_rtNameL.V1, 0xFFFFFFFF, fPopupZ );
// 	pDraw->AddTexture( rcName.Left + 2, rcName.Top, rcName.Right - 2, rcName.Bottom,
// 		m_rtNameC.U0, m_rtNameC.V0, m_rtNameC.U1, m_rtNameC.V1, 0xFFFFFFFF, fPopupZ );
// 	pDraw->AddTexture( rcName.Right - 2, rcName.Top, rcName.Right, rcName.Bottom,
// 		m_rtNameR.U0, m_rtNameR.V0, m_rtNameR.U1, m_rtNameR.V1, 0xFFFFFFFF, fPopupZ );
// 
// 	pDraw->FlushRenderingQueue();

	// 3d배경.
	pDraw->InitTexture( NULL, FALSE, PBT_BLEND, TRUE);
	pDraw->AddQuadrangle(rcName.Left, rcName.Top, rcName.Right, rcName.Bottom, UIMGR()->GetItemNameColor(eDROPITEM_NORMAL_BG), fPopupZ);
	// Render all elements
	pDraw->FlushRenderingQueue();

	// Text
	nTextSX = rcName.Left + 7;
	nTextSY = rcName.Top + 1;

	{
		CItemData* pData = _pNetwork->GetItemData(pTarget->GetType());

		if (pData != NULL && pData->IsFlag(ITEM_FLAG_RARE))
		{
			pDraw->PutTextEx( strName, nTextSX, nTextSY, UIMGR()->GetItemNameColor(eDROPITEM_NORMAL_NAME_RARE), fPopupZ );				
		}
		else if (pData != NULL && pData->IsFlag(ITEM_FLAG_ORIGIN))
		{
			pDraw->PutTextEx( strName, nTextSX, nTextSY, UIMGR()->GetItemNameColor(eDROPITEM_NORMAL_NAME_ORIGIN), fPopupZ );
		}
		else
		{
			pDraw->PutTextEx( strName, nTextSX, nTextSY, UIMGR()->GetItemNameColor(eDROPITEM_NORMAL_NAME), fPopupZ );
		}
	}

	// Flush all render text queue
	pDraw->EndTextEx( TRUE );
}

void ActorMgr::DrawSlave( CSlaveTarget* pTarget, CDrawPort* pDraw, CProjection3D* pprProjection )
{
 	int			nBoxWidth = 0;
 	int			nFontWidth = _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing();
 	int			nTextSX, nTextSY;
 	CEntity*	penObject;
 	FLOAT3D		vObjectPos, vViewPos, vPopupPos, vObjCenter;
 	FLOAT		fRadius, fHeight, fX, fZ, fPopupZ, fSqrDist;	
 	FLOAT		fSqrDistance = g_iShowName * OPTION_NAME_DISTANCE;
 	fSqrDistance *= fSqrDistance;
 	CModelInstance*	pmi = NULL;
 	FLOATaabbox3D	boxModel;
 	CTString		strName;
 	UIRect			rcName;


	CUIManager* pUIMgr = CUIManager::getSingleton();

	penObject = pTarget->GetEntity();

	ASSERT( penObject != NULL );

	if( penObject == NULL )
		return;

#ifdef SORCERER_SUMMON_VILLAGE_VISIBLE_NA_20081008
	if (penObject->IsFlagOn(ENF_HIDDEN))
		return;
#endif
	vObjectPos = penObject->GetLerpedPlacement().pl_PositionVector;

	// Test distance
	fX = vObjectPos(1) - _pNetwork->MyCharacterInfo.x;
	fZ = vObjectPos(3) - _pNetwork->MyCharacterInfo.z;
	fSqrDist = fX * fX + fZ * fZ;
	if( fSqrDist > fSqrDistance )
	{
		pUIMgr->StopTargetEffect(pTarget->m_nIdxServer);
		return;
	}

	// Get frame box
	pmi = penObject->GetModelInstance();
	ASSERT( pmi != NULL );
	if(pmi == NULL)
		return;
	ASSERT( pmi->GetName() != "" );
	if( pmi->GetName() == "" )
		return;

	pmi->GetAllFramesBBox( boxModel );
	boxModel.StretchByVector( pmi->mi_vStretch );
	fHeight = boxModel.maxvect(2) - boxModel.minvect(2);
	fRadius = fHeight * 0.5f;

	// Frustum test
	vObjCenter = vObjectPos;
	vObjCenter(2) += fRadius;
	pprProjection->PreClip( vObjCenter, vViewPos );
	if( pprProjection->TestSphereToFrustum( vViewPos, fRadius ) < 0 )
		return;

	// Object point to screen point
	vObjectPos(2) += fHeight;
	pprProjection->PreClip( vObjectPos, vViewPos );
	pprProjection->PostClip( vViewPos, vPopupPos );
	fPopupZ = ( 1 - pprProjection->pr_fDepthBufferFactor / vViewPos(3) )
		* pprProjection->pr_fDepthBufferMul + pprProjection->pr_fDepthBufferAdd;

	strName = pTarget->m_strName.c_str();

	if (_iNameType == 0)
	{
		strName.PrintF( "%s S%d L%d", pTarget->m_strName.c_str(), pTarget->m_nIdxServer, pTarget->m_yLayer );
	}

	// Get box region
	//if(g_iCountry == RUSSIA)
#if defined G_RUSSIA
	{
		nBoxWidth = pDraw->GetTextWidth(strName) + 13; 
	}
#elif defined(G_THAI)
	{
		nBoxWidth = FindThaiLen( strName ) + 13;
	}
	//else
#else
	{
		nBoxWidth = strName.Length() * nFontWidth + 13;
	}
#endif
	rcName.Left = vPopupPos(1) - nBoxWidth / 2;
	rcName.Right = rcName.Left + nBoxWidth;
	rcName.Bottom = vPopupPos(2) - 7;
	rcName.Top = rcName.Bottom - 15;

	// Set popup texture
	pDraw->InitTextureData( m_ptdPopupTexture, FALSE, PBT_BLEND, TRUE );

	//이름 팝업 테두리색
	COLOR colPopup =0xFFFFFFFF;
	BOOL bTarget =FALSE;

	//타겟으로 잡고 있는 소환수의 경우...
	if( pTarget->m_pEntity== INFO()->GetTargetEntity(eTARGET))
	{
		bTarget =TRUE;

		if( pUIMgr->IsEnemy(pTarget, SUMMON) )		//소환수의 주인이 적이면 테두리를 빨간색으로...
		{
			colPopup =0xFF0000FF;
			pUIMgr->StartTargetEffect( pTarget->m_nIdxServer, pTarget->m_pEntity, TRUE);
		}
		else							//소환수의 주인이 적이 아니면 노란색으로...
		{
			colPopup =0xFFFF00FF;
			pUIMgr->StartTargetEffect( pTarget->m_nIdxServer, pTarget->m_pEntity, FALSE);
		}
	}
	else
		pUIMgr->StopTargetEffect(pTarget->m_nIdxServer);

	if( bTarget )
	{
		int iAddFrame =3;

		// Add render regions
		pDraw->AddTexture( rcName.Left -iAddFrame, rcName.Top -iAddFrame, rcName.Left + 2, rcName.Bottom +iAddFrame,
			m_rtNameL.U0, m_rtNameL.V0, m_rtNameL.U1, m_rtNameL.V1, colPopup, fPopupZ );
		pDraw->AddTexture( rcName.Left + 2, rcName.Top -iAddFrame, rcName.Right - 2, rcName.Bottom +iAddFrame,
			m_rtNameC.U0, m_rtNameC.V0, m_rtNameC.U1, m_rtNameC.V1, colPopup, fPopupZ );
		pDraw->AddTexture( rcName.Right - 2, rcName.Top -iAddFrame, rcName.Right +iAddFrame, rcName.Bottom +iAddFrame,
			m_rtNameR.U0, m_rtNameR.V0, m_rtNameR.U1, m_rtNameR.V1, colPopup, fPopupZ );

		pDraw->AddTexture( rcName.Left -iAddFrame, rcName.Top -iAddFrame, rcName.Left + 2, rcName.Bottom +iAddFrame,
			m_rtNameL.U0, m_rtNameL.V0, m_rtNameL.U1, m_rtNameL.V1, colPopup, fPopupZ );
		pDraw->AddTexture( rcName.Left + 2, rcName.Top -iAddFrame, rcName.Right - 2, rcName.Bottom +iAddFrame,
			m_rtNameC.U0, m_rtNameC.V0, m_rtNameC.U1, m_rtNameC.V1, colPopup, fPopupZ );
		pDraw->AddTexture( rcName.Right - 2, rcName.Top -iAddFrame, rcName.Right +iAddFrame, rcName.Bottom +iAddFrame,
			m_rtNameR.U0, m_rtNameR.V0, m_rtNameR.U1, m_rtNameR.V1, colPopup, fPopupZ );
	}
	else
	{
		// Add render regions
		pDraw->AddTexture( rcName.Left, rcName.Top, rcName.Left + 2, rcName.Bottom,
			m_rtNameL.U0, m_rtNameL.V0, m_rtNameL.U1, m_rtNameL.V1, colPopup, fPopupZ );
		pDraw->AddTexture( rcName.Left + 2, rcName.Top, rcName.Right - 2, rcName.Bottom,
			m_rtNameC.U0, m_rtNameC.V0, m_rtNameC.U1, m_rtNameC.V1, colPopup, fPopupZ );
		pDraw->AddTexture( rcName.Right - 2, rcName.Top, rcName.Right, rcName.Bottom,
			m_rtNameR.U0, m_rtNameR.V0, m_rtNameR.U1, m_rtNameR.V1, colPopup, fPopupZ );
	}

	// 내가 때렸을때 나오는 데미지???
	pUIMgr->ShowDamageList( vPopupPos, fPopupZ, pTarget->m_nIdxClient );

	// Render all elements
	pDraw->FlushRenderingQueue();

	// Text
	nTextSX = rcName.Left + 7;
	nTextSY = rcName.Top + 1;

	pDraw->PutTextCharEx( strName, 0, nTextSX, nTextSY,
		UTIL_HELP()->GetTargetNameColor( 5 ), fPopupZ );

	// Flush all render text queue
	pDraw->EndTextEx( TRUE );
}

void ActorMgr::DrawPet( CPetTarget* pTarget, CDrawPort* pDraw, CProjection3D* pprProjection )
{
	int			nBoxWidth = 0;
	int			nFontWidth = _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing();
	int			nTextSX, nTextSY;
	CEntity*	penObject;
	FLOAT3D		vObjectPos, vViewPos, vPopupPos, vObjCenter;
	FLOAT		fRadius, fHeight, fX, fZ, fPopupZ, fSqrDist;	
	FLOAT		fSqrDistance = g_iShowName * OPTION_NAME_DISTANCE;
	fSqrDistance *= fSqrDistance;
	CModelInstance*	pmi = NULL;
	FLOATaabbox3D	boxModel;
	CTString		strName;
	UIRect			rcName;

	CUIManager* pUIMgr = CUIManager::getSingleton();

	penObject = pTarget->GetEntity();

	ASSERT( penObject != NULL );
	if( penObject == NULL )
		return;

	// Invisibility
	if( penObject->IsFlagOn(ENF_HIDDEN) )
		return;
	
	vObjectPos = penObject->GetLerpedPlacement().pl_PositionVector;

	// Test distance
	fX = vObjectPos(1) - _pNetwork->MyCharacterInfo.x;
	fZ = vObjectPos(3) - _pNetwork->MyCharacterInfo.z;
	fSqrDist = fX * fX + fZ * fZ;
	if( fSqrDist > fSqrDistance )
	{
		pUIMgr->StopTargetEffect(pTarget->GetSIndex());
		return;
	}

	// Get frame box
	pmi = penObject->GetModelInstance();
	ASSERT( pmi != NULL );
	if(pmi == NULL)
		return;

	ASSERT( pmi->GetName() != "" );
	if( pmi->GetName() == "" )
		return;

	pmi->GetAllFramesBBox( boxModel );
	boxModel.StretchByVector( pmi->mi_vStretch );
	fHeight = boxModel.maxvect(2) - boxModel.minvect(2);
	fRadius = fHeight * 0.5f;

	// Frustum test
	vObjCenter = vObjectPos;
	vObjCenter(2) += fRadius;
	pprProjection->PreClip( vObjCenter, vViewPos );
	if( pprProjection->TestSphereToFrustum( vViewPos, fRadius ) < 0 )
		return;

	// Object point to screen point
	vObjectPos(2) += fHeight;
	pprProjection->PreClip( vObjectPos, vViewPos );
	pprProjection->PostClip( vViewPos, vPopupPos );
	fPopupZ = ( 1 - pprProjection->pr_fDepthBufferFactor / vViewPos(3) )
		* pprProjection->pr_fDepthBufferMul + pprProjection->pr_fDepthBufferAdd;

	// Get box region
	if( pTarget->pet_strNameCard.Length() >0)
	{
		strName = pTarget->pet_strNameCard;

		if (_iNameType == 0)
			strName.PrintF( "%s S%d L%d", pTarget->pet_strNameCard, pTarget->m_nIdxServer, pTarget->m_yLayer );

		//if(g_iCountry == RUSSIA)
#if defined G_RUSSIA
		nBoxWidth = pDraw->GetTextWidth(strName) + 13;
		//else
#else
		nBoxWidth = strName.Length() * nFontWidth + 13;
#endif
	}
	else
	{
		strName = pTarget->m_strName.c_str();

		if (_iNameType == 0)
			strName.PrintF( "%s S%d L%d", pTarget->m_strName.c_str(), pTarget->m_nIdxServer, pTarget->m_yLayer );

		//if(g_iCountry == RUSSIA)
#if defined G_RUSSIA
		nBoxWidth = pDraw->GetTextWidth(strName) + 13; 
		//else
#else
		nBoxWidth = strName.Length() * nFontWidth + 13;
#endif
	}
	rcName.Left = vPopupPos(1) - nBoxWidth / 2;
	rcName.Right = rcName.Left + nBoxWidth;
	rcName.Bottom = vPopupPos(2) - 7;
	rcName.Top = rcName.Bottom - 15;

	// Set popup texture
	pDraw->InitTextureData( m_ptdPopupTexture, FALSE, PBT_BLEND, TRUE );

	//이름 팝업 테두리색
	COLOR colPopup =0xFFFFFFFF;
	BOOL bTarget =FALSE;

	//타겟으로 잡고 있는 펫의 경우...
	if( pTarget->GetEntity() == INFO()->GetTargetEntity(eTARGET) )
	{
		bTarget =TRUE;

		if( pUIMgr->IsEnemy(pTarget, P1PET) )			//펫의 주인이 적이면 테두리를 빨간색으로...
		{
			colPopup =0xFF0000FF;
			pUIMgr->StartTargetEffect(pTarget->GetSIndex(), pTarget->GetEntity(), TRUE);
		}
		else							//펫의 주인이 적이 아니면 노란색으로...
		{
			colPopup =0xFFFF00FF;
			pUIMgr->StartTargetEffect(pTarget->GetSIndex(), pTarget->GetEntity(), FALSE);
		}
	}
	else
		pUIMgr->StopTargetEffect(pTarget->GetSIndex());

	if( bTarget )
	{
		int iAddFrame =3;

		pDraw->AddTexture( rcName.Left -iAddFrame, rcName.Top -iAddFrame, rcName.Left + 2, rcName.Bottom +iAddFrame,
			m_rtNameL.U0, m_rtNameL.V0, m_rtNameL.U1, m_rtNameL.V1, colPopup, fPopupZ );
		pDraw->AddTexture( rcName.Left + 2, rcName.Top -iAddFrame, rcName.Right - 2, rcName.Bottom +iAddFrame,
			m_rtNameC.U0, m_rtNameC.V0, m_rtNameC.U1, m_rtNameC.V1, colPopup, fPopupZ );
		pDraw->AddTexture( rcName.Right - 2, rcName.Top -iAddFrame, rcName.Right +iAddFrame, rcName.Bottom +iAddFrame,
			m_rtNameR.U0, m_rtNameR.V0, m_rtNameR.U1, m_rtNameR.V1, colPopup, fPopupZ );

		pDraw->AddTexture( rcName.Left -iAddFrame, rcName.Top -iAddFrame, rcName.Left + 2, rcName.Bottom +iAddFrame,
			m_rtNameL.U0, m_rtNameL.V0, m_rtNameL.U1, m_rtNameL.V1, colPopup, fPopupZ );
		pDraw->AddTexture( rcName.Left + 2, rcName.Top -iAddFrame, rcName.Right - 2, rcName.Bottom +iAddFrame,
			m_rtNameC.U0, m_rtNameC.V0, m_rtNameC.U1, m_rtNameC.V1, colPopup, fPopupZ );
		pDraw->AddTexture( rcName.Right - 2, rcName.Top -iAddFrame, rcName.Right +iAddFrame, rcName.Bottom +iAddFrame,
			m_rtNameR.U0, m_rtNameR.V0, m_rtNameR.U1, m_rtNameR.V1, colPopup, fPopupZ );
	}
	else
	{
		// Add render regions
		pDraw->AddTexture( rcName.Left, rcName.Top, rcName.Left + 2, rcName.Bottom,
			m_rtNameL.U0, m_rtNameL.V0, m_rtNameL.U1, m_rtNameL.V1, colPopup, fPopupZ );
		pDraw->AddTexture( rcName.Left + 2, rcName.Top, rcName.Right - 2, rcName.Bottom,
			m_rtNameC.U0, m_rtNameC.V0, m_rtNameC.U1, m_rtNameC.V1, colPopup, fPopupZ );
		pDraw->AddTexture( rcName.Right - 2, rcName.Top, rcName.Right, rcName.Bottom,
			m_rtNameR.U0, m_rtNameR.V0, m_rtNameR.U1, m_rtNameR.V1, colPopup, fPopupZ );
	}

	pUIMgr->ShowDamageList( vPopupPos, fPopupZ, pTarget->m_nIdxClient );

	// Render all elements
	pDraw->FlushRenderingQueue();

	// Text
	nTextSX = rcName.Left + 7;
	nTextSY = rcName.Top + 1;

	pDraw->PutTextCharEx( strName, 0, nTextSX, nTextSY, UTIL_HELP()->GetTargetNameColor( 5 ), fPopupZ );

	// Flush all render text queue
	pDraw->EndTextEx( TRUE );
}

void ActorMgr::DrawWildPet( CWildPetTarget* pTarget, CDrawPort* pDraw, CProjection3D* pprProjection )
{
	int			nBoxWidth = 0;
	int			nFontWidth = _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing();
	int			nTextSX, nTextSY;
	CEntity*	penObject;
	FLOAT3D		vObjectPos, vViewPos, vPopupPos, vObjCenter;
	FLOAT		fRadius, fHeight, fX, fZ, fPopupZ, fSqrDist;	
	FLOAT		fSqrDistance = g_iShowName * OPTION_NAME_DISTANCE;
	fSqrDistance *= fSqrDistance;
	CModelInstance*	pmi = NULL;
	FLOATaabbox3D	boxModel;
	CTString		strName;
	UIRect			rcName;

	CUIManager* pUIMgr = CUIManager::getSingleton();

	penObject = pTarget->GetEntity();

	ASSERT( penObject != NULL );

	if( penObject == NULL )
		return;

	// Invisibility
	if( penObject->IsFlagOn(ENF_HIDDEN) || penObject->IsFlagOff(ENF_ALIVE) || pTarget->bDeath)
		return;
	vObjectPos = penObject->GetLerpedPlacement().pl_PositionVector;

	// Test distance
	fX = vObjectPos(1) - _pNetwork->MyCharacterInfo.x;
	fZ = vObjectPos(3) - _pNetwork->MyCharacterInfo.z;
	fSqrDist = fX * fX + fZ * fZ;
	if( fSqrDist > fSqrDistance )
	{
		pUIMgr->StopTargetEffect(pTarget->GetSIndex());
		return;
	}

	// Get frame box
	pmi = penObject->GetModelInstance();
	ASSERT( pmi != NULL );
	if(pmi == NULL)
		return;
	ASSERT( pmi->GetName() != "" );
	if( pmi->GetName() == "" )
		return;
	pmi->GetAllFramesBBox( boxModel );
	boxModel.StretchByVector( pmi->mi_vStretch );
	fHeight = boxModel.maxvect(2) - boxModel.minvect(2);
	fRadius = fHeight * 0.5f;

	// Frustum test
	vObjCenter = vObjectPos;
	vObjCenter(2) += fRadius;
	pprProjection->PreClip( vObjCenter, vViewPos );
	if( pprProjection->TestSphereToFrustum( vViewPos, fRadius ) < 0 )
		return;

	// Object point to screen point
	vObjectPos(2) += fHeight;
	pprProjection->PreClip( vObjectPos, vViewPos );
	pprProjection->PostClip( vViewPos, vPopupPos );
	fPopupZ = ( 1 - pprProjection->pr_fDepthBufferFactor / vViewPos(3) )
		* pprProjection->pr_fDepthBufferMul + pprProjection->pr_fDepthBufferAdd;

	// Get box region
	strName = pTarget->m_strName.c_str();

	if (_iNameType == 0)
		strName.PrintF( "%s S%d", pTarget->m_strName.c_str(), pTarget->GetSIndex() );

	if (strName.Length() > 0)
	{
		//if(g_iCountry == RUSSIA)
#if defined G_RUSSIA
		{
			nBoxWidth = pDraw->GetTextWidth(strName) + 13; 
		}
#elif defined(G_THAI)
		{
			nBoxWidth = FindThaiLen( strName ) + 13;
		}
		//				else
#else
		{
			nBoxWidth = strName.Length() * nFontWidth + 13;
		}
#endif
	}

	rcName.Left = vPopupPos(1) - nBoxWidth / 2;
	rcName.Right = rcName.Left + nBoxWidth;
	rcName.Bottom = vPopupPos(2) - 7;
	rcName.Top = rcName.Bottom - 15;

	// Set popup texture
	pDraw->InitTextureData( m_ptdPopupTexture, FALSE, PBT_BLEND, TRUE );

	//이름 팝업 테두리색
	COLOR colPopup =0xFFFFFFFF;
	BOOL bTarget =FALSE;

	//타겟으로 잡고 있는 펫의 경우...
	if (pTarget->GetEntity() == INFO()->GetTargetEntity(eTARGET))
	{
		bTarget =TRUE;

		if( pUIMgr->IsEnemy(pTarget, WILDPET) )			//펫의 주인이 적이면 테두리를 빨간색으로...
		{
			colPopup =0xFF0000FF;
			pUIMgr->StartTargetEffect(pTarget->GetSIndex(), pTarget->GetEntity(), TRUE);
		}
		else							//펫의 주인이 적이 아니면 노란색으로...
		{
			colPopup =0xFFFF00FF;
			pUIMgr->StartTargetEffect(pTarget->GetSIndex(), pTarget->GetEntity(), FALSE);
		}
	}
	else
		pUIMgr->StopTargetEffect(pTarget->GetSIndex());

	if( bTarget )
	{
		int iAddFrame =3;

		pDraw->AddTexture( rcName.Left -iAddFrame, rcName.Top -iAddFrame, rcName.Left + 2, rcName.Bottom +iAddFrame,
			m_rtNameL.U0, m_rtNameL.V0, m_rtNameL.U1, m_rtNameL.V1, colPopup, fPopupZ );
		pDraw->AddTexture( rcName.Left + 2, rcName.Top -iAddFrame, rcName.Right - 2, rcName.Bottom +iAddFrame,
			m_rtNameC.U0, m_rtNameC.V0, m_rtNameC.U1, m_rtNameC.V1, colPopup, fPopupZ );
		pDraw->AddTexture( rcName.Right - 2, rcName.Top -iAddFrame, rcName.Right +iAddFrame, rcName.Bottom +iAddFrame,
			m_rtNameR.U0, m_rtNameR.V0, m_rtNameR.U1, m_rtNameR.V1, colPopup, fPopupZ );

		pDraw->AddTexture( rcName.Left -iAddFrame, rcName.Top -iAddFrame, rcName.Left + 2, rcName.Bottom +iAddFrame,
			m_rtNameL.U0, m_rtNameL.V0, m_rtNameL.U1, m_rtNameL.V1, colPopup, fPopupZ );
		pDraw->AddTexture( rcName.Left + 2, rcName.Top -iAddFrame, rcName.Right - 2, rcName.Bottom +iAddFrame,
			m_rtNameC.U0, m_rtNameC.V0, m_rtNameC.U1, m_rtNameC.V1, colPopup, fPopupZ );
		pDraw->AddTexture( rcName.Right - 2, rcName.Top -iAddFrame, rcName.Right +iAddFrame, rcName.Bottom +iAddFrame,
			m_rtNameR.U0, m_rtNameR.V0, m_rtNameR.U1, m_rtNameR.V1, colPopup, fPopupZ );
	}
	else
	{
		// Add render regions
		pDraw->AddTexture( rcName.Left, rcName.Top, rcName.Left + 2, rcName.Bottom,
			m_rtNameL.U0, m_rtNameL.V0, m_rtNameL.U1, m_rtNameL.V1, colPopup, fPopupZ );
		pDraw->AddTexture( rcName.Left + 2, rcName.Top, rcName.Right - 2, rcName.Bottom,
			m_rtNameC.U0, m_rtNameC.V0, m_rtNameC.U1, m_rtNameC.V1, colPopup, fPopupZ );
		pDraw->AddTexture( rcName.Right - 2, rcName.Top, rcName.Right, rcName.Bottom,
			m_rtNameR.U0, m_rtNameR.V0, m_rtNameR.U1, m_rtNameR.V1, colPopup, fPopupZ );
	}

	pUIMgr->ShowDamageList( vPopupPos, fPopupZ, pTarget->m_nIdxClient );

	// Render all elements
	pDraw->FlushRenderingQueue();

	// Text
	nTextSX = rcName.Left + 7;
	nTextSY = rcName.Top + 1;

	pDraw->PutTextCharEx(strName, 0, nTextSX, nTextSY,
		UTIL_HELP()->GetTargetNameColor( 5 ), fPopupZ );

	// Flush all render text queue
	pDraw->EndTextEx( TRUE );
}

void ActorMgr::SetJoinFlagMerac( SBYTE sbFlag )
{
	mapObject_iter	iter = _map[eOBJ_CHARACTER].begin();
	mapObject_iter	eiter = _map[eOBJ_CHARACTER].end();

	CCharacterTarget* pTarget;	

	for ( ;iter != eiter; ++iter)
	{
		pTarget = static_cast< CCharacterTarget* >(iter->second);
		pTarget->cha_sbJoinFlagMerac = sbFlag;
	}
}

void ActorMgr::ChangeGuildLevel( LONG lGuildIdx, LONG lGuildLv )
{
	mapObject_iter	iter = _map[eOBJ_CHARACTER].begin();
	mapObject_iter	eiter = _map[eOBJ_CHARACTER].end();

	CCharacterTarget* pTarget;	

	for ( ;iter != eiter; ++iter)
	{
		pTarget = static_cast< CCharacterTarget* >(iter->second);
			
		if (pTarget->cha_lGuildIndex == lGuildIdx)
		{
			pTarget->cha_lGuildLevel = lGuildLv;
		}
	}
}

void ActorMgr::ChangeGuildInfo( LONG lGuildIdx, LONG idx, LONG lv, LONG pos, const char* name )
{
	mapObject_iter	iter = _map[eOBJ_CHARACTER].begin();
	mapObject_iter	eiter = _map[eOBJ_CHARACTER].end();

	CCharacterTarget* pTarget;	

	for ( ;iter != eiter; ++iter)
	{
		pTarget = static_cast< CCharacterTarget* >(iter->second);

		if (pTarget->cha_lGuildIndex == lGuildIdx)
		{
			pTarget->cha_lGuildIndex	= idx;
			pTarget->cha_lGuildLevel	= lv;
			pTarget->cha_lGuildPosition	= pos;
			pTarget->cha_strGuildName	= name;
		}
	}
}

int ActorMgr::CheckSyndiMark( INDEX type, INDEX grade )
{
	if (type <= 0 || type >= CSyndicate::eTYPE_MAX)
		return -1;

	if (type == CSyndicate::eTYPE_KAILUX)
	{
		switch (grade)
		{
		case CSyndicate::eGRADE_10TH:
			return eMARK_KAY_KING;
		case CSyndicate::eGRADE_9TH:
			return eMARK_KAY_SPECIAL;
		default:
			return eMARK_KAY_NONE;
		}
	}
	else if (type == CSyndicate::eTYPE_DEALERMOON)
	{
		switch (grade)
		{
		case CSyndicate::eGRADE_7TH:
			return eMARK_DEAL_KING;
		case CSyndicate::eGRADE_6TH:
			return eMARK_DEAL_SPECIAL;
		default:
			return eMARK_DEAL_NONE;
		}
	}

	return -1;
}

void ActorMgr::DrawSyndiMark(int nDrawIdx, CDrawPort* pDraw, int x, int y, FLOAT fz)
{
	if (m_pImgArrSyndiMark == NULL)
		return;

	if (nDrawIdx < 0 || nDrawIdx >= eMARK_GRADE_MAX)
		return;

	m_pImgArrSyndiMark->SetRenderIdx(nDrawIdx);
	m_pImgArrSyndiMark->SetPos(x - DEF_MARK_WIDTH, y);
	m_pImgArrSyndiMark->SetDepth(fz);
	m_pImgArrSyndiMark->OnRender(pDraw);
}


void ActorMgr::SearchNearItem( CSelectedEntities* pSelectEntity, FLOAT3D vPos, float fDistance )
{
	if (pSelectEntity == NULL)
		return;

	mapObject_iter	iter = _map[eOBJ_ITEM].begin();
	mapObject_iter	eiter = _map[eOBJ_ITEM].end();

	int myLayer = _pNetwork->MyCharacterInfo.yLayer;
	ObjectBase* pOut = NULL;
	CItemTarget* pTarget;
	FLOAT	fNear = FLT_MAX;
	FLOAT	fTmpDistance = -1.f;

	for ( ;iter != eiter; ++iter)
	{
		if (iter->second->GetyLayer() == myLayer)
		{
			pTarget = static_cast< CItemTarget* >(iter->second);
			FLOAT3D vDelta = vPos - pTarget->item_place;
			vDelta(2) = 0.f; //0131 높이값 무시.
			fTmpDistance = vDelta.Length();

			if (fDistance >= fTmpDistance)
			{
				pSelectEntity->Add( pTarget->GetEntity() );
			}
		}
	}
}

