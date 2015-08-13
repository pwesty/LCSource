#ifndef __EQUIP_EXCHANGE_EXTEND_H__
#define __EQUIP_EXCHANGE_EXTEND_H__

#define FILENAME "/data/change_item.bin"

class CItem;

enum
{
	EXCHANGE_DATA_WEAPON,
	EXCHANGE_DATA_HELM,
	EXCHANGE_DATA_ARMOR,
	EXCHANGE_DATA_PANTS,
	EXCHANGE_DATA_GLOVE,
	EXCHANGE_DATA_SHOES,
	EXCHANGE_DATA_END,
};

struct ST_ExChangeData_Weapon
{
	int key;
	int itemIndex[12];
};

struct ST_ExChangeData_Wear
{
	int key;
	int itemIndex[8];
};

class CEquipExchangeExtend
{
public:	
	CEquipExchangeExtend();
	~CEquipExchangeExtend();
	static CEquipExchangeExtend* instance();

private:
	ST_ExChangeData_Weapon* m_weapon;
	ST_ExChangeData_Wear* m_wear[5];

	typedef std::vector<int> TARGET_ITEMS;
	typedef std::map<int, TARGET_ITEMS> TARGET_ITEM_MAP;
	typedef std::map<int, TARGET_ITEM_MAP> _MAP;

	int array_size[EXCHANGE_DATA_END];

public:
	bool checkValid(CItem* oldItem, int changeType);
	LONGLONG calNeedNas( CItem* oldItem, bool isRare );
	void load_();
	bool find_(CItem* item, int exchangeItemDBIndex, int itemType);
	void init_();
	_MAP exchange_map;
};

#endif