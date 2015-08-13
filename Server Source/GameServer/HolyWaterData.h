#ifndef __HOLY_WATER_DATA_H__
#define __HOLY_WATER_DATA_H__

enum
{
	PHYSICAL_NPC = 0,
	RANGE_NPC,
	MAGIC_NPC,
	SKILL_NPC,
	PHYSICAL_PC,
	RANGE_PC,
	MAGIC_PC,
	SKILL_PC,
};

struct HOLY_WATER_DATA
{
	int type;
	std::vector<int> _data;
};

class HolyWaterData
{
public:
	HolyWaterData();
	~HolyWaterData();
	static HolyWaterData* instance();

private:
	typedef std::map<int, HOLY_WATER_DATA> HOLY_WATER_MAP;
	HOLY_WATER_MAP _map;

	
	int getToPcDamageType(CPC* opc, const CMagicProto* magic);
	int getToNpcDamageType(CPC* opc, const CMagicProto* magic);
public:
	int getDamage(CPC* op, CCharacter* df, const CMagicProto* magic, int damage);

	void load();
};

#endif