#include "stdhdrs.h"

#include <set>
#include <string>
#include <algorithm>
#include <boost/format.hpp>
#include <boost/foreach.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>
#include "../ShareLib/DBCmd.h"
#include "Server.h"
#include "itemExchangeProto.h"


itemExchangeProto::itemExchangeProto(void)
{
	for (int i = 0; i < ITEM_EXCHANGE_SOURCE_MAX; ++i)
	{
		itemIndex_str[i] = boost::str(boost::format("source_itemIndex%d") % i);
		itemCount_str[i] = boost::str(boost::format("source_itemCount%d") % i);
	}
}


itemExchangeProto::~itemExchangeProto(void)
{
}


itemExchangeProto* itemExchangeProto::instance()
{
	static itemExchangeProto __instance;
	return &__instance;
}

// 아이템 검사를 함으로 item 데이터를 모두 읽은 후 실행하도록 할것
bool itemExchangeProto::Load()
{
	CDBCmd cmd;
	cmd.Init(&gserver->m_dbdata);

	cmd.SetQuery("SELECT * FROM t_item_exchange WHERE a_enable > 0");
	if (cmd.Open() == false)
		return false;

	while (cmd.MoveNext())
	{
		tag_datainfo datainfo;

		cmd.GetRec("a_index", datainfo.a_index);
		cmd.GetRec("a_npc_index", datainfo.a_npc_index);
		cmd.GetRec("result_itemIndex", datainfo.result_itemIndex);
		cmd.GetRec("result_itemCount", datainfo.result_count);

		std::set<int> check_dup;
		for (int i = 0; i < ITEM_EXCHANGE_SOURCE_MAX; ++i)
		{
			tag_sourceInfo listinfo;

			cmd.GetRec(itemIndex_str[i].c_str(), listinfo.itemIndex);
			cmd.GetRec(itemCount_str[i].c_str(), listinfo.itemCount);

			if (listinfo.itemIndex == 0)
				continue;

			if (listinfo.itemCount <= 0)
			{
				LOG_ERROR("Item count is Zero. Item index is %d", listinfo.itemIndex);
				return false;
			}

			if (check_dup.insert(listinfo.itemIndex).second == false)
			{
				LOG_ERROR("Source item is duplicated. item Index is %d", listinfo.itemIndex);
				return false;
			}

			datainfo.source.push_back(listinfo);
		}

		// 재료 아이템을 itemindex에 맞게 소팅함
		std::sort(datainfo.source.begin(), datainfo.source.end(),
				  boost::lambda::bind(&tag_sourceInfo::itemIndex, boost::lambda::_1)
				  <
				  boost::lambda::bind(&tag_sourceInfo::itemIndex, boost::lambda::_2));

		map_.insert(map_t::value_type(datainfo.a_index, datainfo));
	}

	return this->checkAll();
}

bool itemExchangeProto::checkAll()
{
	map_t::iterator it = map_.begin();
	map_t::iterator endit = map_.end();
	for (; it != endit; ++it)
	{
		if (gserver->m_itemProtoList.FindIndex(it->second.result_itemIndex) == NULL)
		{
			LOG_ERROR("Not found result item[%d]", it->second.result_itemIndex);
			return false;
		}

		BOOST_FOREACH(tag_sourceInfo& source, it->second.source)
		{
			if (gserver->m_itemProtoList.FindIndex(source.itemIndex) == NULL)
			{
				LOG_ERROR("Not found source item[%d]", source.itemIndex);
				return false;
			}
		}
	}

	return true;
}

itemExchangeProto::tag_resultInfo itemExchangeProto::exchange( int index, std::vector<tag_sourceInfo>& vec )
{
	tag_resultInfo r;
	map_t::iterator it = map_.find(index);
	if (it == map_.end())
	{
		r.error_code = ERR_ITEM_EXCHANGE_NOT_FOUND_INDEX;
		return r;
	}

	tag_datainfo& dataInfo = it->second;

	if (dataInfo.source.size() != vec.size())
	{
		r.error_code = ERR_ITEM_EXCHANGE_NOT_EQUAL_LIST_COUNT;
		return r;
	}

	// 재료 아이템을 itemindex에 맞게 소팅함
	std::sort(vec.begin(), vec.end(),
			  boost::lambda::bind(&tag_sourceInfo::itemIndex, boost::lambda::_1)
			  <
			  boost::lambda::bind(&tag_sourceInfo::itemIndex, boost::lambda::_2));

	int listCount = dataInfo.source.size();
	for (int i = 0; i < listCount; ++i)
	{
		if (dataInfo.source[i].itemIndex != vec[i].itemIndex)
		{
			r.error_code = ERR_ITEM_EXCHANGE_NOT_EQUAL_ITEM_INDEX;
			return r;
		}

		if (dataInfo.source[i].itemCount != vec[i].itemCount)
		{
			r.error_code = ERR_ITEM_EXCHANGE_NOT_EQUAL_ITEM_COUNT;
			return r;
		}
	}

	// OK
	r.error_code = ERR_ITEM_EXCHANGE_OK;
	r.npc_db_index = dataInfo.a_npc_index;
	r.itemIndex = dataInfo.result_itemIndex;
	r.itemCount = dataInfo.result_count;

	return r;
}

void itemExchangeProto::getExchangeItem( int index, std::vector<tag_sourceInfo>& vec )
{
	map_t::iterator it = map_.find(index);
	if (it == map_.end())
		return;

	vec.assign(it->second.source.begin(), it->second.source.end());
}
