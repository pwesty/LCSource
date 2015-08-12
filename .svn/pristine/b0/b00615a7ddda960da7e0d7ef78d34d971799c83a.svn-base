#ifndef __ITEM_COLLECTION_DATA_H__
#define __ITEM_COLLECTION_DATA_H__

#include "mysql.h"

#define ITEM_COLLECTION_MAX_NEED_VALUE 6

enum
{
	ITEM_COLLECTION_NEED_TYPE_NONE,
	ITEM_COLLECTION_NEED_TYPE_ITEM,
	ITEM_COLLECTION_NEED_TYPE_THEME,
};

class CDBCmd;
enum
{
	ITEMCOLLECTION_RESULT_TYPE_ITEM = 1,
	ITEMCOLLECTION_RESULT_TYPE_NAS,
	ITEMCOLLECTION_RESULT_TYPE_EXP,
	ITEMCOLLECTION_RESULT_TYPE_SP,
};

//for server
struct ST_COLLECTION_NEED_DATA
{
	int need_type;
	int need_index;
	int need_num;
};

struct ST_COLLECTION_HEAD
{
	int category_index;
	int theme_index;
};

struct ST_COLLECTION_RESULT_DATA
{
	int result_type;
	int result_index;
	int result_num;
};

struct ST_COLLECTION_DATA : public ST_COLLECTION_HEAD
{
	bool enable;
	ST_COLLECTION_NEED_DATA need_data[6];
	ST_COLLECTION_RESULT_DATA result_data;
};

struct ST_NEED_DATA_TO_THEME : public ST_COLLECTION_HEAD
{
	int col;
};
//for server


//for user
struct ST_DOING_DATA : public ST_COLLECTION_HEAD
{
	int need_data[6];
};

struct ST_FINISH_DATA : public ST_COLLECTION_HEAD
{
	int end_time;
};
//for user


//Server Data
class ItemCollectionData
{
public:
	ItemCollectionData();
	~ItemCollectionData();
	static ItemCollectionData* instance();

private:
	bool checkCollectionData(int category_index, int theme_index, CPC* pc);

public:
	typedef std::multimap<int/*category index*/, ST_COLLECTION_DATA*> MULTIMAP_COLLECTION_DATA;
	typedef std::vector<ST_NEED_DATA_TO_THEME> VEC_NEED_DATA_TO_THEME;
	typedef std::map<int/*need item*/, VEC_NEED_DATA_TO_THEME> MAP_NEED_DATA_TO_THEME;
	typedef MAP_NEED_DATA_TO_THEME::iterator Map_iter;

	MULTIMAP_COLLECTION_DATA _multimap;
	MAP_NEED_DATA_TO_THEME _map;
	bool load();

	ST_COLLECTION_DATA* findCollectionData(int category_index, int theme_index);
	CItem* getResultItem(int category_index, int theme_index);
};
//Server Data

//User Data
class ItemCollectionManager
{
public:
	ItemCollectionManager(CPC* owner);
	~ItemCollectionManager();

	typedef std::multimap<int, ST_DOING_DATA> multimap_doing;
	typedef std::multimap<int, ST_FINISH_DATA> multimap_finish;

	typedef multimap_doing::iterator multimap_doing_iter;
	typedef multimap_finish::iterator multimap_finish_iter;

private:
	multimap_doing	_multimap_doing;
	multimap_finish	_multimap_finish;

	void CompleteDoingData(int category_index, int theme_index);

	void sendFinishData(int category_index, int theme_index, int end_time);
	void sendDoingData(int category_index, int theme_index, int* need_data);

public:
	CPC* _owner;

	bool checkCanComplete(int category_index, int theme_index);

	void sendFinishInfo();
	void sendDoingInfo();

	void load(MYSQL* conn);
	void save(std::vector<std::string>& vec);
		
	bool updateNeedData(int category_index, int theme_index, int* need);
	void complete(int category_index, int theme_index);
};
//User Data
#endif
