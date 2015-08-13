#ifndef __PTYPE_ITEM_COLLECTION_H__
#define __PTYPE_ITEM_COLLECTION_H__

#include "ptype_base.h"

enum
{
	//request
	MSG_SUB_ITEM_COLLECTION_ITEM_UPDATE_REQ,	//아이템 등록 요청
	MSG_SUB_ITEM_COLLECTION_RESULT_REQ,			//보상받기 요청

	//response
	MSG_SUB_ITEM_COLLECTION_ERROR,

	//update
	MSG_SUB_ITEM_COLLECTION_DOING_LIST_UPD,		//진행중 전체 리스트 정보(로그인)
	MSG_SUB_ITEM_COLLECTION_END_LIST_UPD,		//완료된 전체 리스트 정보(로그인)
	MSG_SUB_ITEM_COLLECTION_DOING_DATA_UPD,		//진행중 테마 정보 업데이트
	MSG_SUB_ITEM_COLLECTION_END_DATA_UPD,		//업적 완료
};

//error code
enum
{
	ITEM_COLLECTION_ERROR_GIVE_SUCCESS,			//아이템 등록 성공
	ITEM_COLLECTION_ERROR_RESULT_SUCCESS,		//아이템 보상 성공

	ITEM_COLLECTION_ERROR_GIVE_FAIL,			//아이템 등록 실패
	ITEM_COLLECTION_ERROR_RESULT_FAIL,			//아이템 보상 실패

	ITEM_COLLECTION_NOT_ENOUGH_INVEN,			//인벤토리 공간 부족
};

#pragma pack(push, 1)

namespace RequestClient
{
	struct NEED_ITEM_DATA
	{
		int need_type;
		int need_index;
		int need_num;
	};


	struct ItemCollection_GiveItem : public pTypeBase
	{
		int category_index;
		int theme_index;
		NEED_ITEM_DATA data[6];
	};

	struct ItemCollection_Result : public pTypeBase
	{
		int category_index;
		int theme_index;
	};
}

namespace ResponseClient
{
	struct ItemCollection_Error : public pTypeBase
	{
		int errorCode;
	};

#ifndef _CLIENT_
	inline void makeItemCollectionError(CNetMsg::SP& rmsg, int errorCode)
	{
		ItemCollection_Error* packet = reinterpret_cast<ItemCollection_Error*>(rmsg->m_buf);
		packet->type = MSG_ITEM_COLLECTION;
		packet->subType = MSG_SUB_ITEM_COLLECTION_ERROR;
		packet->errorCode = errorCode;
		rmsg->setSize(sizeof(ItemCollection_Error));
	}
#endif
}

namespace UpdateClient
{
	struct ItemCollection
	{
		int category_index;
		int theme_index;
		int end_time;
	};
	struct ItemCollectionDoingData
	{
		int category_index;
		int theme_index;
		int need_num[6];
	};


	struct ItemCollection_DoingList : public pTypeBase
	{
		int count;
		ItemCollectionDoingData data[0];
	};

	struct ItemCollection_EndList : public pTypeBase
	{
		int count;
		ItemCollection data[0];
	};

	struct ItemCollection_DoingData : public pTypeBase
	{
		int category_index;
		int theme_index;
		int need_num[6];
	};

	struct ItemCollection_EndData : public pTypeBase
	{
		int category_index;
		int theme_index;
		int end_time;
	};

#ifndef _CLIENT_
	inline void makeItemCollectionDoingDataMsg(CNetMsg::SP& rmsg, int category_index, int theme_index, int* data)
	{
		ItemCollection_DoingData* packet = reinterpret_cast<ItemCollection_DoingData*>(rmsg->m_buf);
		packet->type = MSG_ITEM_COLLECTION;
		packet->subType = MSG_SUB_ITEM_COLLECTION_DOING_DATA_UPD;
		packet->category_index = category_index;
		packet->theme_index = theme_index;
		for(int i=0; i < 6; i++)
		{
			packet->need_num[i] = data[i];
		}
		rmsg->setSize(sizeof(ItemCollection_DoingData));
	}
	inline void makeItemCollectionEndDataMsg(CNetMsg::SP& rmsg, int category_index, int theme_index, int end_time)
	{
		ItemCollection_EndData* packet = reinterpret_cast<ItemCollection_EndData*>(rmsg->m_buf);
		packet->type = MSG_ITEM_COLLECTION;
		packet->subType = MSG_SUB_ITEM_COLLECTION_END_DATA_UPD;
		packet->category_index = category_index;
		packet->theme_index = theme_index;
		packet->end_time = end_time;
		rmsg->setSize(sizeof(ItemCollection_EndData));
	}
#endif
}

#pragma pack(pop)

#endif