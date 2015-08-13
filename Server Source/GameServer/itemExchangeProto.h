#ifndef __ITEM_EXCHANGE_PROTO_H__
#define __ITEM_EXCHANGE_PROTO_H__

#include <vector>
#include <map>

#define ITEM_EXCHANGE_SOURCE_MAX		(5)

enum
{
	ERR_ITEM_EXCHANGE_OK, // 성공

	ERR_ITEM_EXCHANGE_CANT_CREATE_RESULT_ITEM, // 결과 아이템을 만들지 못함
	ERR_ITEM_EXCHANGE_NOT_ENOUGH_SPACE, // 결과 아이템을 넣을 공간이 부족함

	// 여기서 부터는 서버용
	ERR_ITEM_EXCHANGE_NOT_FOUND_INDEX = 100,
	ERR_ITEM_EXCHANGE_NOT_EQUAL_LIST_COUNT,
	ERR_ITEM_EXCHANGE_NOT_EQUAL_ITEM_INDEX,
	ERR_ITEM_EXCHANGE_NOT_EQUAL_ITEM_COUNT,
};

class itemExchangeProto
{
public:
	struct tag_sourceInfo
	{
		int		itemIndex;
		int		itemCount;
	};

	struct tag_datainfo
	{
		int		a_index;
		int		a_npc_index;
		int		result_itemIndex;
		int		result_count;

		std::vector<tag_sourceInfo> source;
	};

	struct tag_resultInfo
	{
		int		error_code;
		int		npc_db_index;
		int		itemIndex;
		int		itemCount;

		tag_resultInfo() : error_code(0), itemIndex(0), itemCount(0) {}
	};

	typedef std::map<int, tag_datainfo> map_t;

public:
	itemExchangeProto(void);
	~itemExchangeProto(void);

	static itemExchangeProto* instance();

	bool Load();
	itemExchangeProto::tag_resultInfo exchange(int index, std::vector<tag_sourceInfo>& vec);
	void getExchangeItem(int index, std::vector<tag_sourceInfo>& vec);

private:
	bool checkAll();

private:
	map_t			map_;

	std::string		itemIndex_str[ITEM_EXCHANGE_SOURCE_MAX];
	std::string		itemCount_str[ITEM_EXCHANGE_SOURCE_MAX];
};

#endif
