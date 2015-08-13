#include "StdH.h"
#include "InvenData.h"
#include <Engine/LocalDefine.h>
#include <Engine/Util.h>
#include <Common/Packet/ptype_inventory.h>
#include <Engine/Entities/Items.h>

#define DEF_SLOT_MAX (450)

CInvenData::CInvenData()
{
	m_nEffectSpeed = 1000;
	m_nEffectPlayCount = -1;
	m_bRander = FALSE;
	m_nRenderTickCount = 0;

	m_vecItemData.clear();
	m_vecItemData.reserve(DEF_SLOT_MAX);
	m_vecItemData.resize(DEF_SLOT_MAX + 1);
}

CInvenData::~CInvenData()
{
	vecItemDataIter bIt = m_vecItemData.begin();
	vecItemDataIter eIt = m_vecItemData.end();

	for (; bIt != eIt; ++bIt)
	{
		if ((*bIt) == NULL)
			continue;

		SAFE_DELETE((*bIt));
	}

	m_vecItemData.clear();
}

void CInvenData::InitItemData( CItems* pItems )
{
	if (pItems == NULL)
		return;
	
	int nDataIndex = GetCalcIndex(pItems->Item_Tab, pItems->InvenIndex);

	if (nDataIndex < 0 || nDataIndex >= m_vecItemData.size())
		return;

	SAFE_DELETE(m_vecItemData[nDataIndex]);

	m_vecItemData[nDataIndex] = pItems;
}

int CInvenData::GetCalcIndex( int tab, int invenIndex )
{
	int nDataIndex = invenIndex;

	if (tab > INVENTORY_TAB_NORMAL)
		nDataIndex += ITEM_COUNT_IN_INVENTORY_NORMAL;

	if (tab > INVENTORY_TAB_CASH_1)
		nDataIndex += ITEM_COUNT_IN_INVENTORY_CASH_1;

	if (tab > INVENTORY_TAB_CASH_2)
		nDataIndex += ITEM_COUNT_IN_INVENTORY_CASH_2;

	if (tab > INVENTORY_TAB_STASH)
		nDataIndex += ITEM_COUNT_IN_INVENTORY_STASH;

	return nDataIndex;
}

CItems* CInvenData::GetItemInfo( int tab, int invenIndex )
{
	int nDataIndex = GetCalcIndex(tab, invenIndex);

	if (nDataIndex < 0 || nDataIndex >= m_vecItemData.size())
		return NULL;

	return m_vecItemData[nDataIndex];
}

