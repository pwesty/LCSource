#include "stdhdrs.h"

#include <map>
#include <vector>
#include <boost/format.hpp>
#include "../ShareLib/DBCmd.h"
#include "ItemCollection.h"
#include "Server.h"
#include "Character.h"

#include "../ShareLib/packetType/ptype_item_collection.h"

#define NEED_TYPE_ITEM		1
#define NEED_TYPE_THEME		2

enum
{
	RESULT_TYPE_NONE,
	RESULT_TYPE_ITEM,
	RESULT_TYPE_NAS,
	RESULT_TYPE_EXP,
	RESULT_TYPE_SP,
};

ItemCollectionData::ItemCollectionData(){}
ItemCollectionData::~ItemCollectionData(){}

ItemCollectionData* ItemCollectionData::instance()
{
	static ItemCollectionData _instance;
	return &_instance;
}

bool ItemCollectionData::load()
{
	std::string query = "SELECT * FROM t_item_collection WHERE a_enable = 1";

	CDBCmd cmd;
	cmd.Init(&gserver->m_dbdata);
	cmd.SetQuery(query);
	if( cmd.Open() == false )
	{
		LOG_ERROR("LOAD ITEM_COLLECTION DATA FAIL. ERROR[%s]", mysql_error(&gserver->m_dbdata));
		return false;
	}

	while(cmd.MoveNext() == true)
	{
		ST_COLLECTION_DATA* data = new ST_COLLECTION_DATA;
		int result_type;
		int result_index;
		int result_num;
		cmd.GetRec("a_category",		data->category_index);
		cmd.GetRec("a_theme",			data->theme_index);
		cmd.GetRec("a_need1_type",		data->need_data[0].need_type);
		cmd.GetRec("a_need1_index",		data->need_data[0].need_index);
		cmd.GetRec("a_need1_num",		data->need_data[0].need_num);
		cmd.GetRec("a_need2_type",		data->need_data[1].need_type);
		cmd.GetRec("a_need2_index",		data->need_data[1].need_index);
		cmd.GetRec("a_need2_num",		data->need_data[1].need_num);
		cmd.GetRec("a_need3_type",		data->need_data[2].need_type);
		cmd.GetRec("a_need3_index",		data->need_data[2].need_index);
		cmd.GetRec("a_need3_num",		data->need_data[2].need_num);
		cmd.GetRec("a_need4_type",		data->need_data[3].need_type);
		cmd.GetRec("a_need4_index",		data->need_data[3].need_index);
		cmd.GetRec("a_need4_num",		data->need_data[3].need_num);
		cmd.GetRec("a_need5_type",		data->need_data[4].need_type);
		cmd.GetRec("a_need5_index",		data->need_data[4].need_index);
		cmd.GetRec("a_need5_num",		data->need_data[4].need_num);
		cmd.GetRec("a_need6_type",		data->need_data[5].need_type);
		cmd.GetRec("a_need6_index",		data->need_data[5].need_index);
		cmd.GetRec("a_need6_num",		data->need_data[5].need_num);
		cmd.GetRec("a_result_type",		data->result_data.result_type);
		cmd.GetRec("a_result_index",	data->result_data.result_index);
		cmd.GetRec("a_result_num",		data->result_data.result_num);
				
		_multimap.insert(MULTIMAP_COLLECTION_DATA::value_type(data->category_index, data));

		for(int i = 0; i < 6; i++)
		{
			if(data->need_data[0].need_type == NEED_TYPE_THEME)
			{
				//필요조건이 테마일 경우에는 테마 맵 데이터에 데이터 추가.
				ST_NEED_DATA_TO_THEME st;
				st.category_index = data->category_index;
				st.theme_index = data->theme_index;
				st.col = i;
				
				Map_iter it = _map.find(data->need_data[i].need_index);
				if(it != _map.end())
				{
					it->second.push_back(st);
				}
				else
				{
					VEC_NEED_DATA_TO_THEME vec;
					vec.push_back(st);
					_map.insert(Map_iter::value_type(data->need_data[i].need_index, vec));
				}
			}
		}
	}

	return true;
}

ST_COLLECTION_DATA* ItemCollectionData::findCollectionData( int category_index, int theme_index )
{
	MULTIMAP_COLLECTION_DATA::iterator lower_it, upper_it;

	lower_it = _multimap.lower_bound(category_index);
	upper_it = _multimap.upper_bound(category_index);

	for( ; lower_it != upper_it; lower_it++ )
	{
		if(lower_it->second->theme_index == theme_index)
		{
			return lower_it->second;
		}
	}

	return NULL;
}

bool ItemCollectionData::checkCollectionData( int category_index, int theme_index, CPC* pc )
{
	//pc->m_itemCollection_data;
	return true;
}

CItem* ItemCollectionData::getResultItem( int category_index, int theme_index )
{
	MULTIMAP_COLLECTION_DATA::iterator lower_it, upper_it;

	lower_it = _multimap.lower_bound(category_index);
	upper_it = _multimap.upper_bound(category_index);

	for( ; lower_it != upper_it; lower_it++ )
	{
		if(lower_it->second->theme_index == theme_index)
		{
			int type = lower_it->second->result_data.result_type;
			if(type != RESULT_TYPE_ITEM)
				return NULL;

			int index = lower_it->second->result_data.result_index;
			int num = lower_it->second->result_data.result_num;

			CItem* item = gserver->m_itemProtoList.CreateItem(index, -1, 0, 0, num);
			return item;
		}
	}

	return NULL;
}

//-----------ItemCollectionManager Function------------//

ItemCollectionManager::ItemCollectionManager(CPC* owner)
{
	_owner = owner;
}

ItemCollectionManager::~ItemCollectionManager()
{
}

void ItemCollectionManager::CompleteDoingData(int category_index, int theme_index)
{
	//완료가 되면 해당 테마를 필요로 하는 다른 테마를 찾아서 doing 상태로 변경시켜준다.....
	ItemCollectionData::Map_iter it_map = ItemCollectionData::instance()->_map.find(theme_index);
	if(it_map != ItemCollectionData::instance()->_map.end())
	{
		ItemCollectionData::VEC_NEED_DATA_TO_THEME::iterator it_vec = it_map->second.begin();
		ItemCollectionData::VEC_NEED_DATA_TO_THEME::iterator it_vec_end = it_map->second.end();

		for(; it_vec != it_vec_end; it_vec++)
		{
			int need[6] = {0,};
			need[it_vec->col] = 1;
			updateNeedData(it_vec->category_index, it_vec->theme_index, need);
		}
	}

	ST_FINISH_DATA data;
	data.category_index = category_index;
	data.theme_index = theme_index;
	data.end_time = gserver->getNowSecond();
	_multimap_finish.insert(multimap_finish::value_type(data.category_index, data));
	
	sendFinishData(category_index, theme_index, data.end_time);
	return;
}

bool ItemCollectionManager::updateNeedData( int category_index, int theme_index, int* need )
{
	ST_COLLECTION_DATA* data = ItemCollectionData::instance()->findCollectionData(category_index, theme_index);

	if(data == NULL)
		return false;

	multimap_doing_iter lower_it = _multimap_doing.lower_bound(category_index);
	multimap_doing_iter upper_it = _multimap_doing.upper_bound(category_index);


	//기존에 있는 테마 데이터에 업데이트 하는 경우
	for(; lower_it != upper_it; lower_it++)
	{
		if(lower_it->second.theme_index == theme_index)
		{
			for(int i = 0; i < ITEM_COLLECTION_MAX_NEED_VALUE; i++)
			{
				lower_it->second.need_data[i] += need[i];

				if(lower_it->second.need_data[i] > data->need_data[i].need_num)
				{
					lower_it->second.need_data[i] = data->need_data[i].need_num;
					need[i] -= (lower_it->second.need_data[i] - data->need_data[i].need_num);
				}
			}
			sendDoingData(category_index, theme_index, lower_it->second.need_data);
			return true;
		}
	}

	//위에서 리턴하지 못하면 새로 추가되는 데이터로 판단.
	{
		ST_DOING_DATA st_d;
		st_d.category_index = category_index;
		st_d.theme_index = theme_index;

		for(int i = 0; i < ITEM_COLLECTION_MAX_NEED_VALUE; i++)
		{
			st_d.need_data[i] = need[i];
			if(st_d.need_data[i] > data->need_data[i].need_num)
			{
				st_d.need_data[i] = data->need_data[i].need_num;
			}
		}

		_multimap_doing.insert(multimap_doing::value_type(st_d.category_index, st_d));
		sendDoingData(category_index, theme_index, need);
	}
	return true;
}

void ItemCollectionManager::save(std::vector<std::string>& vec)
{
	{
		std::string delete_end = boost::str(boost::format(
			"DELETE FROM t_item_collection_end WHERE a_char_index = %1%" ) % _owner->m_index);

		vec.push_back(delete_end);
	}
	
	{
		std::string delete_ing = boost::str(boost::format(
			"DELETE FROM t_item_collection_ing WHERE a_char_index = %1%" ) % _owner->m_index);

		vec.push_back(delete_ing);
	}

	if(_multimap_doing.size() > 0)
	{
		std::string insert_doing = "INSERT into t_item_collection_ing values ";

		multimap_doing_iter it = _multimap_doing.begin();
		multimap_doing_iter it_end = _multimap_doing.end();

		std::string buf1, buf2;
		ST_DOING_DATA st;
		for(; it != it_end; it++)
		{
			st = it->second;
			buf1 = boost::str(boost::format(
				"(%d, %d, %d") % st.category_index % st.theme_index % _owner->m_index);
			for(int i = 0; i < 6; i++)
			{
				buf2 = boost::str(boost::format(
					", %d") % st.need_data[i]);
				buf1 += buf2;
			}
			buf1 += "), ";
			insert_doing += buf1;
		}

		int pos = insert_doing.rfind(",");
		insert_doing.erase(pos);

		vec.push_back(insert_doing);
	}
	
	if(_multimap_finish.size() > 0)
	{
		std::string insert_end = "INSERT into t_item_collection_end values";

		multimap_finish_iter it = _multimap_finish.begin();
		multimap_finish_iter it_end = _multimap_finish.end();

		ST_FINISH_DATA st;
		std::string buf;
		for(; it != it_end; it++)
		{
			st = it->second;
			buf += boost::str(boost::format(
				"(%d, %d, %d, FROM_UNIXTIME(%d)), ") % st.category_index % st.theme_index % _owner->m_index % st.end_time);
		}

		insert_end += buf;
		int pos = insert_end.rfind(",");
		insert_end.erase(pos);

		vec.push_back(insert_end);
	}
}

void ItemCollectionManager::load(MYSQL* conn)
{
	CDBCmd dbChar;
	dbChar.Init(conn);

	std::string query = boost::str(boost::format("SELECT * from t_item_collection_ing where a_char_index = %d") % _owner->m_index);
	dbChar.SetQuery(query);
	dbChar.Open();
	ST_DOING_DATA st_d;
	while(dbChar.MoveNext())
	{
		dbChar.GetRec("a_category", st_d.category_index);
		dbChar.GetRec("a_theme", st_d.theme_index);
		std::string need;
		for(int i = 0; i < 6; i++)
		{
			need = boost::str(boost::format("a_need%d") % (i+1) );
			dbChar.GetRec(need.c_str(), st_d.need_data[i]);
		}

		_multimap_doing.insert(multimap_doing::value_type(st_d.category_index, st_d));
	}

	query = boost::str(boost::format("SELECT a_category, a_theme, UNIX_TIMESTAMP(a_end_time) as a_end_time from t_item_collection_end where a_char_index = %d") % _owner->m_index);
	dbChar.SetQuery(query);

	dbChar.Open();
	ST_FINISH_DATA st_f;
	while(dbChar.MoveNext())
	{
		dbChar.GetRec("a_category", st_f.category_index);
		dbChar.GetRec("a_theme", st_f.theme_index);
		dbChar.GetRec("a_end_time", st_f.end_time);

		_multimap_finish.insert(multimap_finish::value_type(st_f.category_index, st_f));
	}
}

bool ItemCollectionManager::checkCanComplete( int category_index, int theme_index )
{
	multimap_doing_iter lower_it = _multimap_doing.lower_bound(category_index);
	multimap_doing_iter upper_it = _multimap_doing.upper_bound(category_index);

	for(; lower_it != upper_it; lower_it++)
	{
		if(lower_it->second.theme_index == theme_index)
		{
			ST_COLLECTION_DATA* data = ItemCollectionData::instance()->findCollectionData(category_index, theme_index);

			for(int i = 0; i < 6; i++)
			{
				//하나라도 완료되지 않았으면
				if( data->need_data[i].need_num < lower_it->second.need_data[i] )
				{
					return false;
				}
			}
			return true;
		}
	}
	return false;
}

void ItemCollectionManager::complete( int category_index, int theme_index )
{
	multimap_doing_iter lower_it = _multimap_doing.lower_bound(category_index);
	multimap_doing_iter upper_it = _multimap_doing.upper_bound(category_index);

	for(; lower_it != upper_it; lower_it++)
	{
		if(lower_it->second.theme_index == theme_index)
		{
			this->CompleteDoingData(lower_it->second.category_index, lower_it->second.theme_index);
			_multimap_doing.erase(lower_it);
			return ;
		}
	}
}

void ItemCollectionManager::sendFinishData(int category_index, int theme_index, int end_time)
{
	//완료된 항목 데이터 전달
	CNetMsg::SP rmsg(new CNetMsg);
	UpdateClient::makeItemCollectionEndDataMsg(rmsg, category_index, theme_index, end_time);
	SEND_Q(rmsg, _owner->m_desc);

}

void ItemCollectionManager::sendDoingData(int category_index, int theme_index, int* need_data)
{
	//진행중 항목 데이터 전달
	CNetMsg::SP rmsg(new CNetMsg);
	UpdateClient::makeItemCollectionDoingDataMsg(rmsg, category_index, theme_index, need_data);
	SEND_Q(rmsg, _owner->m_desc);
}

void ItemCollectionManager::sendFinishInfo()
{
	if(_multimap_finish.empty())
		return;

	//완료 리스트 데이터 패킷 전달
	int totalcount = _multimap_finish.size();
	multimap_finish_iter it = _multimap_finish.begin();
	multimap_finish_iter it_end = _multimap_finish.end();

	for (int k = 0; k < totalcount; )
	{
		CNetMsg::SP rmsg(new CNetMsg);

		UpdateClient::ItemCollection_EndList* packet = reinterpret_cast<UpdateClient::ItemCollection_EndList*>(rmsg->m_buf);
		packet->type = MSG_ITEM_COLLECTION;
		packet->subType = MSG_SUB_ITEM_COLLECTION_END_LIST_UPD;
		packet->count = 0;

		for(int i = 0; i < 100 && k < totalcount && it != it_end; it++, i++, k++)
		{
			packet->data[i].category_index = it->second.category_index;
			packet->data[i].theme_index = it->second.theme_index;
			packet->data[i].end_time = it->second.end_time;

			++packet->count;
		}

		rmsg->setSize(sizeof(UpdateClient::ItemCollection_EndList) +  sizeof(UpdateClient::ItemCollection) * packet->count);
		SEND_Q(rmsg, _owner->m_desc);
	}
}

void ItemCollectionManager::sendDoingInfo()
{
	if(_multimap_doing.empty())
		return;

	//진행중인 리스트 데이터 패킷 전달
	int totalcount = _multimap_doing.size();
	multimap_doing_iter it = _multimap_doing.begin();
	multimap_doing_iter it_end = _multimap_doing.end();

	for (int k = 0; k < totalcount; )
	{
		CNetMsg::SP rmsg(new CNetMsg);

		UpdateClient::ItemCollection_DoingList* packet = reinterpret_cast<UpdateClient::ItemCollection_DoingList*>(rmsg->m_buf);
		packet->type = MSG_ITEM_COLLECTION;
		packet->subType = MSG_SUB_ITEM_COLLECTION_DOING_LIST_UPD;
		packet->count = 0;

		for(int i = 0; i < 50 && k < totalcount && it != it_end; it++, i++, k++)
		{
			packet->data[i].category_index = it->second.category_index;
			packet->data[i].theme_index = it->second.theme_index;
			for(int j = 0; j < 6; j++)
			{
				packet->data[i].need_num[j] = it->second.need_data[j];
			}

			++packet->count;
		}

		rmsg->setSize(sizeof(UpdateClient::ItemCollection_DoingList) + sizeof(UpdateClient::ItemCollectionDoingData) * packet->count);
		SEND_Q(rmsg, _owner->m_desc);
	}
}
