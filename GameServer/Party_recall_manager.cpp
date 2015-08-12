#include "stdhdrs.h"

#include "Party_recall_manager.h"
#include "Server.h"
#include "Log.h"

CParty_recall_manager *CParty_recall_manager::Instance = NULL;

CParty_recall_manager::CParty_recall_manager()
{
}

CParty_recall_manager::~CParty_recall_manager()
{
	Clear_map();
	Release_instance();
}

CParty_recall_manager* CParty_recall_manager::Get_instance()
{
	if ( Instance == NULL )
		Instance = new CParty_recall_manager();

	return Instance;
}

void CParty_recall_manager::Release_instance()
{
	if ( Instance )
		delete Instance;

	Instance = NULL;
}

void CParty_recall_manager::Clear_map()
{
	Party_recall_map.clear();
}

/* insert map data */
void CParty_recall_manager::Insert_map(int Character_index, int Item_db_index)
{
	int loop = 0;
	while(loop != 3)
	{
		std::pair<std::map<int, int>::iterator, bool> ret = Party_recall_map.insert(std::pair<int, int>(Character_index, Item_db_index));

		if ( ret.second == false )
		{
			if ( Erase_map(Character_index) == ERASE_NOTFOUND )
			{
				GAMELOG << init("Error Party_recall_map Erase")
						<< "Character Index" << delim << Character_index << end;
			}
		}
		else
			break;
		loop++;
		if ( loop == 3 )
		{
			GAMELOG << init("Error Party_recall_map Insert")
					<< "Character Index" << delim << Character_index << delim
					<< "Item DB Index" << delim << Item_db_index << end;
		}
	}
}
/* find map data */
int CParty_recall_manager::Find_map(int Character_index)
{
	std::map<int, int>::iterator itr;

	itr = Party_recall_map.find(Character_index);
	if ( itr == Party_recall_map.end() )
		return CParty_recall_manager::FIND_NOTFOUND;

	return itr->second;
}

/* erase map data */
int CParty_recall_manager::Erase_map(int Character_index)
{
	if ( Find_map(Character_index) == CParty_recall_manager::FIND_NOTFOUND )
		return CParty_recall_manager::ERASE_NOTFOUND;

	Party_recall_map.erase(Character_index);
	return CParty_recall_manager::ERASE_SUCCESSED;
}
//