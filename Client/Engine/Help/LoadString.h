#ifndef		_STRING_LOADER_H_
#define		_STRING_LOADER_H_

#include <Engine/DesignClasses/Singleton/SingletonBase.h>
#include <Engine/Help/Loader.h>

enum eSTRING
{
	eSTRING_STRING,
	eSTRING_HELP1,
	eSTRING_ITEM,
	eSTRING_ITEM_SET,
	eSTRING_OPTION,
	eSTRING_OPTION_RARE,
	eSTRING_NPC_NAME,
	eSTRING_NPC_HELP,
	eSTRING_NPC_SHOP,
	eSTRING_QUEST,
	eSTRING_SKILL,
	eSTRING_SKILL_SPECIAL,
	eSTRING_ACTION,
	eSTRING_COMBO,
	eSTRING_AFFINITY,
	eSTRING_LACARETTE,
	eSTRING_ITEMCOLLECTION,
	eSTRING_MAX
};

const char StringCount[eSTRING_MAX] = 
{
	1, // eSTRING_STRING,
	2, // eSTRING_HELP1,
	2, // eSTRING_ITEM,
	1, // eSTRING_ITEM_SET,
	1, // eSTRING_OPTION,
	1, // eSTRING_OPTION_RARE,
	2, // eSTRING_NPC_NAME,
	2, // eSTRING_NPC_HELP,
	2, // eSTRING_NPC_SHOP,
	4, // eSTRING_QUEST,
	3, // eSTRING_SKILL,
	2, // eSTRING_SKILL_SPECIAL,
	2, // eSTRING_ACTION,
	1, // eSTRING_COMBO,
	1, // eSTRING_AFFINITY,
	1, // eSTRING_LACARETTE
	2, // eSTRING_ITEMCOLLECTION
};


class StringLoader : public CSingletonBase< StringLoader >
{
public:
	bool LoadString(eSTRING eType, int nNation);
	void release(eSTRING eType);

	bool LoadData(eSTRING eType, const char* strPath);
	const char* getData(eSTRING eType, int index, int id);

	int getCount(eSTRING eType);
	

private:
	TableLoader		_tbl_string[eSTRING_MAX];
};


#endif		// _STRING_LOADER_H_