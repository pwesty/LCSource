#ifndef __PARTY_RECALL_MANAGER_H__
#define __PARTY_RECALL_MANAGER_H__

class CParty_recall_manager
{
	/* member method */
public:
	virtual ~CParty_recall_manager();
	static CParty_recall_manager *Get_instance();
	static void Release_instance();
	void Insert_map(int Character_index, int Item_db_index);
	int Find_map(int Character_index);
	int Erase_map(int Character_index);

	/* ERROR ENUM */
public:
	enum FIND_RET_TYPE		{ FIND_NOTFOUND }; /* if find return item_db_index */
	enum ERASE_RET_TYPE		{ ERASE_SUCCESSED, ERASE_NOTFOUND };

	/* member method */
private:
	CParty_recall_manager();
	void Clear_map();

	/* member var */
private:
	std::map<int, int> Party_recall_map;

	static CParty_recall_manager *Instance;
};
#endif // __PARTY_RECALL_MANAGER_H__
//
