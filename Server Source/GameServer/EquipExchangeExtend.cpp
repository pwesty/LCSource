#include "stdhdrs.h"

#include <vector>
#include <map>
#include "Log.h"
#include "Server.h"
#include "EquipExchangeExtend.h"


CEquipExchangeExtend::CEquipExchangeExtend()	
{
}
CEquipExchangeExtend::~CEquipExchangeExtend()
{
	delete[] m_weapon;
	for(int i = 0; i < 5; i++)
	{
		delete[] m_wear[i];
	}
}

CEquipExchangeExtend* CEquipExchangeExtend::instance()
{
	static CEquipExchangeExtend __instance;
	return &__instance;
}

void CEquipExchangeExtend::load_()
{
	FILE* fp = NULL;

	CLCString strTemp(1024);

	strTemp.Format("%s%s", gserver->m_serverpath, FILENAME);

	fp = fopen(strTemp, "rb");

	if(fp == NULL)
	{
		LOG_ERROR("EquipExchangeExtend Data Load Fail.....");
		return;
	}


	for(int col = 0; col < EXCHANGE_DATA_END; col++)
	{
		fread(&array_size[col], sizeof(int), 1, fp);
		if(array_size[col] <= 0)
		{
			fclose(fp);
			break;
		}	

		switch(col)
		{
		case EXCHANGE_DATA_WEAPON:
			m_weapon = new ST_ExChangeData_Weapon[array_size[col]];
			fread(m_weapon, sizeof(ST_ExChangeData_Weapon) * array_size[col], 1, fp);			
			break;
		case EXCHANGE_DATA_HELM:
		case EXCHANGE_DATA_ARMOR:
		case EXCHANGE_DATA_PANTS:
		case EXCHANGE_DATA_GLOVE:
		case EXCHANGE_DATA_SHOES:
			m_wear[col - 1] = new ST_ExChangeData_Wear[array_size[col]];
			fread(m_wear[col - 1], sizeof(ST_ExChangeData_Wear) * array_size[col], 1, fp);
			break;
		}
	}
	fclose(fp);

	init_();
}

bool CEquipExchangeExtend::find_(CItem* item, int exchangeItemDBIndex, int itemType)
{
	//아이템 검색
	int _type;

	if(item->m_itemProto->getItemTypeIdx() == ITYPE_WEAPON)
	{
		_type = EXCHANGE_DATA_WEAPON;	
	}
	else if(item->m_itemProto->getItemTypeIdx() == ITYPE_WEAR)
	{
		switch(itemType)
		{		
		case IWEAR_HELMET:
			_type = EXCHANGE_DATA_HELM;
			break;
		case IWEAR_ARMOR:
			_type = EXCHANGE_DATA_ARMOR;
			break;
		case IWEAR_PANTS:
			_type = EXCHANGE_DATA_PANTS;
			break;
		case IWEAR_GLOVE:
			_type = EXCHANGE_DATA_GLOVE;
			break;
		case IWEAR_SHOES:
			_type = EXCHANGE_DATA_SHOES;
			break;		
		}
	}

	_MAP::iterator it = exchange_map.find(_type);
	if(it != exchange_map.end())
	{
		TARGET_ITEM_MAP::iterator it_map = it->second.find(item->getDBIndex());
		if(it_map != it->second.end())
		{
			TARGET_ITEMS::iterator it_vec = it_map->second.begin();
			TARGET_ITEMS::iterator it_vec_end = it_map->second.end();

			for(; it_vec != it_vec_end; it_vec++)
			{
				if(*it_vec == exchangeItemDBIndex)
				{
					CItem* exchangeItem = gserver->m_itemProtoList.CreateItem(exchangeItemDBIndex, item->getWearPos(), 0, 0, 1);
					return true;
				}
			}
		}
	}
	
	return false;	
}

void CEquipExchangeExtend::init_()
{
	for(int kind = 0; kind < EXCHANGE_DATA_END; kind++)
	{
		switch(kind)
		{
		case EXCHANGE_DATA_WEAPON:	
			{
				TARGET_ITEM_MAP _map;
				for(int i = 0; i < array_size[kind]; i++)
				{
					TARGET_ITEMS _vec;
					for(int j = 0; j < 12; j++)
					{
						_vec.push_back(m_weapon[i].itemIndex[j]);
					}					
					_map.insert(TARGET_ITEM_MAP::value_type(m_weapon[i].key, _vec));
				}
				exchange_map.insert(_MAP::value_type(kind, _map));
			}	
			break;
		case EXCHANGE_DATA_HELM:
		case EXCHANGE_DATA_ARMOR:
		case EXCHANGE_DATA_PANTS:
		case EXCHANGE_DATA_GLOVE:
		case EXCHANGE_DATA_SHOES:
			{
				TARGET_ITEM_MAP _map;
				for(int i = 0; i < array_size[kind]; i++)
				{
					TARGET_ITEMS _vec;
					for(int j = 0; j < 8; j++)
					{
						_vec.push_back(m_wear[kind-1][i].itemIndex[j]);
					}					
					_map.insert(TARGET_ITEM_MAP::value_type(m_wear[kind-1][i].key, _vec));
				}
				exchange_map.insert(_MAP::value_type(kind, _map));
			}	
			break;
		}		
	}
}

bool CEquipExchangeExtend::checkValid( CItem* oldItem, int changeType )
{
	if (oldItem->m_itemProto->getItemLevel() < 29)
		return false;

	if (oldItem->getWearPos() != WEARING_NONE)
		return false;

	// 무기 방어구 모두 교환 가능하게 수정 : 051219 : bs
	if (!oldItem->IsWeaponType() && (!oldItem->IsArmorType() || oldItem->m_itemProto->getItemSubTypeIdx() == IWEAR_SHIELD))
		return false;

	if ((oldItem->IsWeaponType() && oldItem->m_itemProto->getItemSubTypeIdx() == changeType)
		|| (oldItem->IsArmorType() && (oldItem->m_itemProto->getItemJobFlag() & (1 << changeType))))
		return false;
	
	// 대여 무기 검사
	if (oldItem->IsLent())
		return false;

	return true;
}

LONGLONG CEquipExchangeExtend::calNeedNas( CItem* oldItem, bool isRare )
{
	if(isRare == true)
		return 0;

	LONGLONG needmoney = (oldItem->IsWeaponType()) ? 5000 : 3000;
	needmoney = needmoney * (oldItem->getPlus() * 2 + 10) * (oldItem->GetItemLevel() - 28) / 10;

	return needmoney;
}
