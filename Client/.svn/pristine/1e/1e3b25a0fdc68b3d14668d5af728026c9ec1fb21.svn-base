#include "StdH.h"
#include "ChangeWeapon.h"
#include <Engine/Util.h>
#include <Engine/Network/CNetwork.h>

mapChangeEquip CChangeEquipment::m_mapChangeWeapon; // 무기
mapChangeEquip CChangeEquipment::m_mapChangeArmor; // 방어구

ChangeWeapon::ChangeWeapon(void)
{
	Clear();
}

ChangeWeapon::~ChangeWeapon(void)
{
}

void ChangeWeapon::SendChangeEquipment(BOOL bCash)
{
	if (m_nTab < 0 || m_nInvenIdx < 0)
		return;

	int nCheckIdx = _pNetwork->MySlotItem[m_nTab][m_nInvenIdx].Item_UniIndex;

	if (nCheckIdx != m_nItemVirtualIdx)
		return;

	if(bCash)
		_pNetwork->UseChangeWeaponItem( m_nTab, m_nInvenIdx, m_nItemVirtualIdx, m_nSelectIdx, m_nTradeItemDBIdx );
	else
		_pNetwork->ChangeWeaponReq( m_nTab, m_nInvenIdx, m_nItemVirtualIdx,	m_nTradeItemDBIdx, m_nSelectIdx, m_nGoldTokenVirtualIdx, m_nGoldTokenCount);
}

void ChangeWeapon::Clear()
{
	m_nTab = -1;
	m_nInvenIdx = -1;
	m_nItemVirtualIdx = -1;
	m_nSelectIdx = -1;
	m_nGoldTokenVirtualIdx = -1;
	m_nTradeItemDBIdx = -1;
	m_nGoldTokenCount = -1;
}

CChangeEquipment::~CChangeEquipment()
{
	m_mapChangeArmor.clear();
	m_mapChangeWeapon.clear();
}

bool CChangeEquipment::loadEx( const char* FileName )
{
	FILE *fp		= NULL;

	if ((fp = fopen(FileName, "rb")) == NULL) 
	{
		MessageBox(NULL, "changeEquipment File is not Exist.", "error!", MB_OK);
		return false;
	}

	int nCount;
	int nHelmet, nTop, nPants, nGloves, nBoots;

	// 무기
	fread(&nCount, sizeof(int), 1, fp);

	if (nCount <= 0)
	{
		fclose(fp);
		return false;
	}

	stChangeWeapon* pdata = new stChangeWeapon[nCount];
	fread(pdata, sizeof(stChangeWeapon) * nCount, 1, fp);

	// 투구
	fread(&nHelmet, sizeof(int), 1, fp);

	if (nHelmet <= 0)
	{
		fclose(fp);
		return false;
	}

	stChangeArmor* pHelmet = new stChangeArmor[nHelmet];
	fread(pHelmet, sizeof(stChangeArmor) * nHelmet, 1, fp);
	
	// 상의
	fread(&nTop, sizeof(int), 1, fp);

	if (nTop <= 0)
	{
		fclose(fp);
		return false;
	}

	stChangeArmor* pTop = new stChangeArmor[nTop];
	fread(pTop, sizeof(stChangeArmor) * nTop, 1, fp);

	// 하의
	fread(&nPants, sizeof(int), 1, fp);

	if (nPants <= 0)
	{
		fclose(fp);
		return false;
	}

	stChangeArmor* pPants = new stChangeArmor[nPants];
	fread(pPants, sizeof(stChangeArmor) * nPants, 1, fp);

	// 장갑
	fread(&nGloves, sizeof(int), 1, fp);

	if (nGloves <= 0)
	{
		fclose(fp);
		return false;
	}

	stChangeArmor* pGloves = new stChangeArmor[nGloves];
	fread(pGloves, sizeof(stChangeArmor) * nGloves, 1, fp);

	// 부츠
	fread(&nBoots, sizeof(int), 1, fp);

	if (nBoots <= 0)
	{
		fclose(fp);
		return false;
	}

	stChangeArmor* pBoots = new stChangeArmor[nBoots];
	fread(pBoots, sizeof(stChangeArmor) * nBoots, 1, fp);

	fclose(fp);

	// 데이타 셋팅
	int		i;

	// 무기
	for (i = 0; i < nCount; i++)
	{
		int nKey = pdata[i].index[DEF_TRADE_CONDITION_INDEX];

		if (nKey <= 0)
			continue;

		std::vector<int> vecData;
		
		for (int j = 1; j < DEF_TRADE_WEAPON_MAX; j++)
		{
			vecData.push_back(pdata[i].index[j]);
		}	

		m_mapChangeWeapon.insert(std::make_pair(nKey, vecData));
	}

	// 헬멧
	for (i = 0; i < nHelmet; i++)
	{
		int nKey = pHelmet[i].index[DEF_TRADE_CONDITION_INDEX];

		if (nKey <= 0)
			continue;

		std::vector<int> vecData;

		for (int j = 1; j < TOTAL_JOB; j++)
		{
			vecData.push_back(pHelmet[i].index[j]);
		}	

		m_mapChangeArmor.insert(std::make_pair(nKey, vecData));
	}

	// 상의
	for (i = 0; i < nTop; i++)
	{
		int nKey = pTop[i].index[DEF_TRADE_CONDITION_INDEX];

		if (nKey <= 0)
			continue;

		std::vector<int> vecData;

		for (int j = 1; j < TOTAL_JOB; j++)
		{
			vecData.push_back(pTop[i].index[j]);
		}	

		m_mapChangeArmor.insert(std::make_pair(nKey, vecData));
	}

	// 하의
	for (i = 0; i < nPants; i++)
	{
		int nKey = pPants[i].index[DEF_TRADE_CONDITION_INDEX];

		if (nKey <= 0)
			continue;

		std::vector<int> vecData;

		for (int j = 1; j < TOTAL_JOB; j++)
		{
			vecData.push_back(pPants[i].index[j]);
		}	

		m_mapChangeArmor.insert(std::make_pair(nKey, vecData));
	}

	// 장갑
	for (i = 0; i < nGloves; i++)
	{
		int nKey = pGloves[i].index[DEF_TRADE_CONDITION_INDEX];

		if (nKey <= 0)
			continue;

		std::vector<int> vecData;

		for (int j = 1; j < TOTAL_JOB; j++)
		{
			vecData.push_back(pGloves[i].index[j]);
		}	

		m_mapChangeArmor.insert(std::make_pair(nKey, vecData));
	}

	// 부츠
	for (i = 0; i < nBoots; i++)
	{
		int nKey = pBoots[i].index[DEF_TRADE_CONDITION_INDEX];

		if (nKey <= 0)
			continue;

		std::vector<int> vecData;

		for (int j = 1; j < TOTAL_JOB; j++)
		{
			vecData.push_back(pBoots[i].index[j]);
		}	

		m_mapChangeArmor.insert(std::make_pair(nKey, vecData));
	}

	SAFE_ARRAY_DELETE(pdata);
	SAFE_ARRAY_DELETE(pHelmet);
	SAFE_ARRAY_DELETE(pTop);
	SAFE_ARRAY_DELETE(pPants);
	SAFE_ARRAY_DELETE(pGloves);
	SAFE_ARRAY_DELETE(pBoots);
	return true;
}

int CChangeEquipment::getTradeWeaponIndex( int nItemIdx, int nSelectIdx )
{
	if (nItemIdx <= 0)
		return 0;

	if (nSelectIdx < 0 || nSelectIdx >= DEF_TRADE_WEAPON_MAX)
		return 0;

	mapChangeEquip_iter iter = m_mapChangeWeapon.find(nItemIdx);

	if (iter != m_mapChangeWeapon.end())
	{
		if (iter->second.size() <= nSelectIdx)
			return 0;

		return iter->second[nSelectIdx];
	}

	return 0;
}

int CChangeEquipment::getTradeArmorIndex( int nItemIdx, int nSelectIdx )
{
	if (nItemIdx <= 0)
		return 0;

	if (nSelectIdx < 0 || nSelectIdx >= TOTAL_JOB)
		return 0;

	mapChangeEquip_iter iter = m_mapChangeArmor.find(nItemIdx);

	if (iter != m_mapChangeArmor.end())
	{
		if (iter->second.size() <= nSelectIdx)
			return 0;

		return iter->second[nSelectIdx];
	}

	return 0;
}

bool CChangeEquipment::IsValidWeapon( int nItemIdx )
{
	if (nItemIdx <= 0)
		return false;

	mapChangeEquip_iter iter = m_mapChangeWeapon.find(nItemIdx);

	if (iter != m_mapChangeWeapon.end())
	{
		return true;
	}

	return false;
}

bool CChangeEquipment::IsValidArmor( int nItemIdx )
{
	if (nItemIdx <= 0)
		return false;

	mapChangeEquip_iter iter = m_mapChangeArmor.find(nItemIdx);

	if (iter != m_mapChangeArmor.end())
	{
		return true;
	}

	return false;
}
