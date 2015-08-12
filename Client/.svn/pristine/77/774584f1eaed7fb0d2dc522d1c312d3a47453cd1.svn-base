#include "StdH.h"
#include "World.h"
#include <Engine/Help/DefineHelp.h>
#include <Engine/Help/LoadString.h>
#include <Engine/Network/CNetwork.h>
#include <Engine/Interface/UIDefine.h>
#include <Engine/Interface/UIManager.h>
#include <Engine/Contents/function/ItemCollectionData.h>

extern INDEX g_iCountry;

void CWorld::LoadStringItem()
{
	StringLoader* pLoader = StringLoader::getSingleton();
	CUIManager* pUIManager = CUIManager::getSingleton();

	if (pLoader == NULL)
		return;

	pLoader->LoadString(eSTRING_ITEM, g_iCountry);

	int		nIdx;
	const char* strtmp = NULL;

	CItemData::_map::iterator	iter = CItemData::_mapdata.begin();
	CItemData::_map::iterator	eiter = CItemData::_mapdata.end();

	for (;iter != eiter; ++iter)
	{
		CItemData* pItem = (*iter).second;

		if (pItem == NULL)
			continue;

		nIdx = pItem->getindex();

		if (nIdx >= 0)
		{
			strtmp = pLoader->getData(eSTRING_ITEM, nIdx, 0);
			if (strtmp != NULL)
				pItem->SetName( strtmp );
			else
				pUIManager->SetNoTranslateFlag( TRANS_NAME, &pItem->GetTransFlag() );

			strtmp = pLoader->getData(eSTRING_ITEM, nIdx, 1);
			if (strtmp != NULL)
				pItem->SetDesc( strtmp );
			else
				pUIManager->SetNoTranslateFlag( TRANS_DESC, &pItem->GetTransFlag() );

			if( pItem->GetTransFlag() > 0 )	//[sora] 미번역 스트링 index 표시
				pUIManager->SetNoTranslationString( ITEM_STRING, pItem->GetItemIndex() );			
		}
	}

	pLoader->release(eSTRING_ITEM);
}

void CWorld::LoadStringItemSet()
{
	StringLoader* pLoader = StringLoader::getSingleton();
	CUIManager* pUIManager = CUIManager::getSingleton();

	if (pLoader == NULL)
		return;

	pLoader->LoadString(eSTRING_ITEM_SET, g_iCountry);

	int		nIdx;
	const char* strtmp = NULL;

	CSetItemData::_map::iterator	iter = CSetItemData::_mapdata.begin();
	CSetItemData::_map::iterator	eiter = CSetItemData::_mapdata.end();

	for (;iter != eiter; ++iter)
	{
		CSetItemData* pData = (*iter).second;

		if (pData == NULL)
			continue;

		nIdx = pData->getindex();

		if (nIdx >= 0)
		{
			strtmp = pLoader->getData(eSTRING_ITEM_SET, nIdx, 0);
			if (strtmp != NULL)
				pData->SetName( strtmp );
		}
	}

	pLoader->release(eSTRING_ITEM_SET);
}

void CWorld::LoadStringOption()
{
	StringLoader* pLoader = StringLoader::getSingleton();
	CUIManager* pUIManager = CUIManager::getSingleton();

	if (pLoader == NULL)
		return;

	pLoader->LoadString(eSTRING_OPTION, g_iCountry);

	int		nIdx;
	const char* strtmp = NULL;

	COptionData::_map::iterator	iter = COptionData::_mapdata.begin();
	COptionData::_map::iterator	eiter = COptionData::_mapdata.end();

	for (;iter != eiter; ++iter)
	{
		COptionData* pData = (*iter).second;

		if (pData == NULL)
			continue;

		nIdx = pData->GetIndex();

		if (nIdx >= 0)
		{
			strtmp = pLoader->getData(eSTRING_OPTION, nIdx, 0);
			if (strtmp != NULL)
				pData->SetName( strtmp );

			if (pData->GetName() == NULL)
				pUIManager->SetNoTranslationString(OPTION_STRING, nIdx);
		}
	}

	pLoader->release(eSTRING_OPTION);
}

void CWorld::LoadStringOptionRare()
{
	StringLoader* pLoader = StringLoader::getSingleton();
	CUIManager* pUIManager = CUIManager::getSingleton();

	if (pLoader == NULL)
		return;

	pLoader->LoadString(eSTRING_OPTION_RARE, g_iCountry);

	int		nIdx;
	const char* strtmp = NULL;

	CItemRareOption::_map::iterator	iter = CItemRareOption::_mapdata.begin();
	CItemRareOption::_map::iterator	eiter = CItemRareOption::_mapdata.end();

	for (;iter != eiter; ++iter)
	{
		CItemRareOption* pData = (*iter).second;

		if (pData == NULL)
			continue;

		nIdx = pData->GetIndex();

		if (nIdx >= 0)
		{
			strtmp = pLoader->getData(eSTRING_OPTION_RARE, nIdx, 0);
			if (strtmp != NULL)
				pData->SetPrefix( strtmp );

			if (pData->GetPrefix() == NULL)
				pUIManager->SetNoTranslationString(RARE_OPTION_STRING, nIdx);
		}
	}

	pLoader->release(eSTRING_OPTION_RARE);
}

void CWorld::LoadStringNPCName()
{
	StringLoader* pLoader = StringLoader::getSingleton();
	CUIManager* pUIManager = CUIManager::getSingleton();

	if (pLoader == NULL)
		return;

	pLoader->LoadString(eSTRING_NPC_NAME, g_iCountry);

	int		nIdx;
	const char* strtmp = NULL;

	CMobData::_map::iterator	iter = CMobData::_mapdata.begin();
	CMobData::_map::iterator	eiter = CMobData::_mapdata.end();

	for (;iter != eiter; ++iter)
	{
		CMobData* pData = (*iter).second;

		if (pData == NULL)
			continue;

		nIdx = pData->GetMobIndex();

		if (nIdx >= 0)
		{
			strtmp = pLoader->getData(eSTRING_NPC_NAME, nIdx, 0);
			if (strtmp != NULL)
				pData->SetName( strtmp );
			else
				pUIManager->SetNoTranslateFlag( TRANS_NAME, &pData->GetTranslateFlag() );

			strtmp = pLoader->getData(eSTRING_NPC_NAME, nIdx, 1);
			if (strtmp != NULL)
				pData->SetDesc( strtmp );
			else
				pUIManager->SetNoTranslateFlag( TRANS_DESC, &pData->GetTranslateFlag() );

			if (pData->GetTranslateFlag() > 0 )	//[sora] 미번역 스트링 index 표시
				pUIManager->SetNoTranslationString( NPC_STRING, nIdx );
		}
	}

	pLoader->release(eSTRING_NPC_NAME);
}

void CWorld::LoadStringQuest()
{
	StringLoader* pLoader = StringLoader::getSingleton();
	CUIManager* pUIManager = CUIManager::getSingleton();

	if (pLoader == NULL)
		return;

	pLoader->LoadString(eSTRING_QUEST, g_iCountry);

	int		nIdx;
	const char* strtmp = NULL;

	CQuestStaticData::_map::iterator	iter = CQuestStaticData::_mapdata.begin();
	CQuestStaticData::_map::iterator	eiter = CQuestStaticData::_mapdata.end();

	for (;iter != eiter; ++iter)
	{
		CQuestStaticData* pData = (*iter).second;
	
		if (pData == NULL)
			continue;
		
		nIdx = pData->getindex();

		if (nIdx >= 0)
		{
			strtmp = pLoader->getData(eSTRING_QUEST, nIdx, 0);
			if (strtmp != NULL)
				pData->SetTitle(strtmp);
			
			strtmp = pLoader->getData(eSTRING_QUEST, nIdx, 1);
			if (strtmp != NULL)
				pData->SetDesc1(strtmp);

			strtmp = pLoader->getData(eSTRING_QUEST, nIdx, 2);
			if (strtmp != NULL)
				pData->SetDesc2(strtmp);

			strtmp = pLoader->getData(eSTRING_QUEST, nIdx, 3);
			if (strtmp != NULL)
				pData->SetDesc3(strtmp);			
		}
	}

	pLoader->release(eSTRING_QUEST);
}

void CWorld::LoadStringSkill()
{
	StringLoader* pLoader = StringLoader::getSingleton();
	CUIManager* pUIManager = CUIManager::getSingleton();

	if (pLoader == NULL)
		return;

	pLoader->LoadString(eSTRING_SKILL, g_iCountry);

	CStaticArray<CSkill>&	cont_ = _pNetwork->ga_World.wo_aSkillData;

	int		i, nIdx;
	int		nMax = cont_.Count();	
	const char* strtmp;

	for (i = 0; i <= nMax; ++i)
	{
		nIdx = cont_[i].GetIndex();

		if (nIdx >= 0)
		{
			strtmp = pLoader->getData(eSTRING_SKILL, nIdx, 0);
			
			if (strtmp != NULL)
				cont_[i].SetName( strtmp );
			else
				pUIManager->SetNoTranslateFlag( TRANS_NAME, &cont_[i].GetTransFlag() );

			strtmp = pLoader->getData(eSTRING_SKILL, nIdx, 1);
			if (strtmp != NULL)
				cont_[i].SetDescription( strtmp );
			else
				pUIManager->SetNoTranslateFlag( TRANS_DESC, &cont_[i].GetTransFlag() );

			strtmp = pLoader->getData(eSTRING_SKILL, nIdx, 2);
			if (strtmp != NULL)
				cont_[i].SetTooltip( strtmp );
			else
				pUIManager->SetNoTranslateFlag( TRANS_DESC2, &cont_[i].GetTransFlag() );

			if( cont_[i].GetTransFlag() > 0 )	//[sora] 미번역 스트링 index 표시
				pUIManager->SetNoTranslationString( SKILL_STRING, nIdx );			
		}
	}

	pLoader->release(eSTRING_SKILL);
}

void CWorld::LoadStringSkillSpecial()
{
	StringLoader* pLoader = StringLoader::getSingleton();
	CUIManager* pUIManager = CUIManager::getSingleton();

	if (pLoader == NULL)
		return;

	pLoader->LoadString(eSTRING_SKILL_SPECIAL, g_iCountry);


	int		nIdx;
	const char* strtmp = NULL;

	CSpecialSkill::_map::iterator	iter = CSpecialSkill::_mapdata.begin();
	CSpecialSkill::_map::iterator	eiter = CSpecialSkill::_mapdata.end();

	for (;iter != eiter; ++iter)
	{
		CSpecialSkill* pData = (*iter).second;

		if (pData == NULL)
			continue;
		
		nIdx = pData->getindex();

		if (nIdx >= 0)
		{
			strtmp = pLoader->getData(eSTRING_SKILL_SPECIAL, nIdx, 0);
			if (strtmp != NULL)
				pData->SetName( strtmp );
			else
				pUIManager->SetNoTranslateFlag( TRANS_NAME, &pData->GetTransFlag() );

			strtmp = pLoader->getData(eSTRING_SKILL_SPECIAL, nIdx, 1);
			if (strtmp != NULL)
				pData->SetDesc( strtmp );
			else
				pUIManager->SetNoTranslateFlag( TRANS_DESC, &pData->GetTransFlag() );
			
			if( pData->GetTransFlag() > 0 )	//[sora] 미번역 스트링 index 표시
				pUIManager->SetNoTranslationString( SPECIAL_SKILL_STRING, nIdx );			
		}
	}

	pLoader->release(eSTRING_SKILL_SPECIAL);
}

void CWorld::LoadStringAction()
{
	StringLoader* pLoader = StringLoader::getSingleton();

	if (pLoader == NULL)
		return;

	pLoader->LoadString(eSTRING_ACTION, g_iCountry);

	int		nIdx;
	const char* strtmp = NULL;

	CAction::_map::iterator	iter = CAction::_mapdata.begin();
	CAction::_map::iterator	eiter = CAction::_mapdata.end();

	for (;iter != eiter; ++iter)
	{
		CAction* pData = (*iter).second;

		if (pData == NULL)
			continue;

		nIdx = pData->GetIndex();

		if (nIdx >= 0)
		{
			strtmp = pLoader->getData(eSTRING_ACTION, nIdx, 0);
			if (strtmp != NULL)
				pData->SetName( strtmp );

			strtmp = pLoader->getData(eSTRING_ACTION, nIdx, 1);
			if (strtmp != NULL)
				pData->SetDesc( strtmp );
		}
	}

	pLoader->release(eSTRING_ACTION);
}

void CWorld::LoadStringCombo()
{
	StringLoader* pLoader = StringLoader::getSingleton();

	if (pLoader == NULL)
		return;

	pLoader->LoadString(eSTRING_COMBO, g_iCountry);

	int		nIdx;
	const char* strtmp = NULL;

	CMissionCase::_map::iterator	iter = CMissionCase::_mapdata.begin();
	CMissionCase::_map::iterator	eiter = CMissionCase::_mapdata.end();

	for (;iter != eiter; ++iter)
	{
		CMissionCase* pData = (*iter).second;

		if (pData == NULL)
			continue;

		nIdx = pData->GetIndex();

		if (nIdx >= 0)
		{
			strtmp = pLoader->getData(eSTRING_COMBO, nIdx, 0);
			if (strtmp != NULL)
				pData->SetName( strtmp );
		}
	}

	pLoader->release(eSTRING_COMBO);
}

void CWorld::LoadStringAffinity()
{
	StringLoader* pLoader = StringLoader::getSingleton();

	if (pLoader == NULL)
		return;

	pLoader->LoadString(eSTRING_AFFINITY, g_iCountry);

	CAffinityData* pData = _pNetwork->GetAffinityData();
	const char* strtmp = NULL;

	CAffinityData::mapAffIter iter = pData->mapAffinity_Data.begin();
	CAffinityData::mapAffIter eiter = pData->mapAffinity_Data.end();

	for (; iter != eiter; ++iter)
	{
		if ((*iter).second != NULL)
		{
			strtmp = pLoader->getData(eSTRING_AFFINITY, (*iter).second->GetIndex(), 0);
			if (strtmp != NULL)
				(*iter).second->SetName( strtmp );
		}
	}

	pLoader->release(eSTRING_AFFINITY);
}

void CWorld::LoadStringLacarette()
{
	StringLoader* pLoader = StringLoader::getSingleton();

	if (pLoader == NULL)
		return;

	pLoader->LoadString(eSTRING_LACARETTE, g_iCountry);

	CStaticArray<CLacarette>&	cont_ = _pNetwork->wo_aLacaretteData;

	int		i, nIdx;
	int		nMax = cont_.Count();
	const char* strtmp = NULL;

	for (i = 0; i <= nMax; ++i)
	{
		nIdx = cont_[i].GetCourseIndex();

		if (nIdx >= 0)
		{
			strtmp = pLoader->getData(eSTRING_LACARETTE, nIdx, 0);
			if (strtmp != NULL)
				cont_[i].SetName( strtmp );
		}
	}

	pLoader->release(eSTRING_LACARETTE);
}

void CWorld::LoadStringItemCollection()
{
	StringLoader* pLoader = StringLoader::getSingleton();
	CUIManager* pUIManager = CUIManager::getSingleton();

	if (pLoader == NULL)
		return;

	pLoader->LoadString(eSTRING_ITEMCOLLECTION, g_iCountry);

	int		nIdx;
	const char* strtmp = NULL;

	CItemCollectionData::_map::iterator	iter = CItemCollectionData::_mapdata.begin();
	CItemCollectionData::_map::iterator	eiter = CItemCollectionData::_mapdata.end();

	for (;iter != eiter; ++iter)
	{
		CItemCollectionData* pItem = (*iter).second;

		if (pItem == NULL)
			continue;

		nIdx = pItem->getTheme();

		if (nIdx >= 0)
		{
			strtmp = pLoader->getData(eSTRING_ITEMCOLLECTION, nIdx, 0);

			if (strtmp != NULL)
				pItem->SetName( strtmp );
			else
				pUIManager->SetNoTranslateFlag( TRANS_NAME, &pItem->GetTransFlag() );

			strtmp = pLoader->getData(eSTRING_ITEMCOLLECTION, nIdx, 1);

			if (strtmp != NULL)
				pItem->SetDesc( strtmp );
			else
				pUIManager->SetNoTranslateFlag( TRANS_DESC, &pItem->GetTransFlag() );

			if( pItem->GetTransFlag() > 0 )	//미번역 스트링 index 표시
				pUIManager->SetNoTranslationString( ITEMCOLLECTION_STRING, pItem->getTheme() );			
		}
	}

	pLoader->release(eSTRING_ITEMCOLLECTION);
}
