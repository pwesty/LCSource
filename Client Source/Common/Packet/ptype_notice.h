#ifndef __PTYPE_NOTICE_H__
#define __PTYPE_NOTICE_H__

#include <string.h>
#include "ptype_base.h"


enum MsgExNoticeThirdType
{
	MSG_EX_NOTICE_LIST,								// 공지 리스트 요청
};


#pragma pack(push, 1)
struct NoticeData
{
	int noticeRev;				//리비전 번호
	char noticeString[107];		//공지 스트링
	int date;					//시간
};

namespace RequestClient
{
	//공지 데이터 요청
	struct NoticeList : public pTypeThirdBase
	{
		int pageNum;
	};
}

namespace ResponseClient
{
	//공지 데이터 응답
	struct NoticeList : public pTypeThirdBase
	{
		bool isNextPage;
		int count;
		NoticeData data[0];
	};
}

namespace UpdateClient
{
	//공지 리비전 데이터 업데이트 (클라이언트가 받아서 체크 후 깜빡임 유무 판단)
	struct checkNotice : public pTypeThirdBase
	{
		int revNum;
	};

	struct checkPopupNotice : public pTypeThirdBase
	{
		int html_num;
	};

	struct closePopupNotice : public pTypeThirdBase
	{
		int html_num;
	};

	struct popupNoticeClear : public pTypeThirdBase
	{};
}
#pragma pack(pop)

#ifndef _CLIENT_
	inline void CheckNoticeMsg(CNetMsg::SP &rmsg, int revNum)
	{
		UpdateClient::checkNotice* packet = reinterpret_cast<UpdateClient::checkNotice*>(rmsg->m_buf);
		packet->type = MSG_UPDATE_DATA_FOR_CLIENT;
		packet->subType = MSG_SUB_UPDATE_CHECK_NOTICE;	//129
		packet->revNum = revNum;
		rmsg->setSize(sizeof(UpdateClient::checkNotice));
	}

	inline void CheckPopupNoticeMsg(CNetMsg::SP &rmsg, int html_num, int runtime)
	{
		UpdateClient::checkPopupNotice* packet = reinterpret_cast<UpdateClient::checkPopupNotice*>(rmsg->m_buf);
		packet->type = MSG_UPDATE_DATA_FOR_CLIENT;
		packet->subType = MSG_SUB_UPDATE_CHECK_POPUP_NOTICE;	//130
		packet->html_num = html_num;
		packet->runtime = runtime;
		rmsg->setSize(sizeof(UpdateClient::checkPopupNotice));
	}

	inline void ClosePopupNoticeMsg(CNetMsg::SP &rmsg, int html_num)
	{
		UpdateClient::closePopupNotice* packet = reinterpret_cast<UpdateClient::closePopupNotice*>(rmsg->m_buf);
		packet->type = MSG_UPDATE_DATA_FOR_CLIENT;
		packet->subType = MSG_SUB_UPDATE_CLOSE_POPUP_NOTICE;	//131
		packet->html_num = html_num;
		rmsg->setSize(sizeof(UpdateClient::closePopupNotice));
	}

	inline void PopupNoticeClearMsg(CNetMsg::SP &rmsg)
	{
		UpdateClient::popupNoticeClear* packet = reinterpret_cast<UpdateClient::popupNoticeClear*>(rmsg->m_buf);
		packet->type = MSG_UPDATE_DATA_FOR_CLIENT;
		packet->subType = MSG_SUB_UPDATE_CLEAR_POPUP_NOTICE;
		rmsg->setSize(sizeof(UpdateClient::popupNoticeClear));
	}
#endif

#endif