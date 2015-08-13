#include "stdhdrs.h"
#include <boost/format.hpp>

#include "Character.h"
#include "PCList.h"
#include "Log.h"
#include "Notice.h"
#include "Server.h"
#include "PCList.h"
#include "CmdMsg.h"
#include "DBManager.h"

#include "../ShareLib/DBCmd.h"
#include "../ShareLib/packetType/ptype_notice.h"

bool Compare(NoticeStr* a, NoticeStr* b);

Notice::Notice()
{
}

Notice::~Notice()
{
	std::vector<NoticeStr*>::iterator it = _vec.begin();
	std::vector<NoticeStr*>::iterator it_end = _vec.end();

	for(; it != it_end; it++)
		delete *it;
}

void Notice::_insert( NoticeStr* data )
{
	_vec.push_back(data);

	sort(_vec.begin(), _vec.end(), Compare);
}

void Notice::_delete( int index )
{
	std::vector<NoticeStr*>::iterator it = _vec.begin();
	std::vector<NoticeStr*>::iterator it_end = _vec.end();

	for(; it != it_end; it++)
	{
		if((*it)->index == index)
		{
			_vec.erase(it);
			delete *it;
			return;
		}
	}
}

NoticeStr* Notice::_find(int index)
{
	std::vector<NoticeStr*>::iterator it = _vec.begin();
	std::vector<NoticeStr*>::iterator it_end = _vec.end();

	for(; it != it_end; it++)
	{
		if((*it)->index == index)
		{
			return (*it);
		}
	}
}

bool Notice::SendNoticeList( int charIndex, CNetMsg::SP& msg)
{
	CPC* pc = PCManager::instance()->getPlayerByCharIndex(charIndex);

	if(pc == NULL)
		return false;

	RequestClient::NoticeList* p = reinterpret_cast<RequestClient::NoticeList*>(msg->m_buf);
	int pageNum = p->pageNum;
	
	CNetMsg::SP rmsg(new CNetMsg);

	ResponseClient::NoticeList* packet = reinterpret_cast<ResponseClient::NoticeList*>(rmsg->m_buf);
	packet->type = MSG_EXTEND;
	packet->subType = MSG_EX_NOTICE;
	packet->thirdType = MSG_EX_NOTICE_LIST;
	packet->isNextPage = false;

	if(_vec.size() == 0)
		return false;

	if(_vec.size() > (10 * (pageNum + 1)))
	{
		packet->isNextPage = true;
		packet->count = 10;
	}
	else if( _vec.size() % (10 * (pageNum + 1)) == 0)
	{
		if( _vec.size() > (10 * (pageNum +1)) )
		{
			packet->isNextPage = true;
		}
		else
		{
			packet->isNextPage = false;
		}
		packet->count = 10;
	}
	else
	{
		packet->count = _vec.size() % 10;
	}

	int i = pageNum * 10;

	if( i > _vec.size() - 1 )
		return false;
	
	for(int j = 0; i < packet->count + (pageNum * 10); i++, j++)
	{
		packet->data[j].noticeRev = _vec[i]->index;
		strncpy(packet->data[j].noticeString, _vec[i]->notice_string, 107);
		packet->data[j].date = _vec[i]->date;
	}

	rmsg->setSize( sizeof(ResponseClient::NoticeList) + (sizeof(NoticeData) * packet->count));
	SEND_Q(rmsg, pc->m_desc);
}

void Notice::load()
{
	CDBCmd dbcmd;
	dbcmd.Init(&(gserver->m_dbdata));
	dbcmd.SetQuery("SELECT a_index, a_notice_string, UNIX_TIMESTAMP(a_date) as a_date FROM t_notice_all order by a_date desc");
	if (!dbcmd.Open())
	{
		LOG_INFO("Notice Date Load Error. mysql_error(%s)", mysql_error(dbcmd.m_dbconn));
		return ;
	}
	std::string notice_string;

	NoticeStr* data = NULL;
	
	while(dbcmd.MoveNext())
	{
		data = new NoticeStr();
		dbcmd.GetRec("a_index", data->index);
		strncpy(data->notice_string, dbcmd.GetRec("a_notice_string"), 100);
		dbcmd.GetRec("a_date", data->date);

		_vec.push_back(data);
	}

	if(data != NULL)
		m_lastRev = data->index;
}

Notice* Notice::instance()
{
	static Notice _instance;
	return &_instance;
}

int Notice::getLastRev()
{
	return m_lastRev;
}

void Notice::setLastRev( int revNo )
{
	m_lastRev = revNo;
}

int Notice::getNoticeCount()
{
	return this->_vec.size();
}

//PopupNotice Class

PopupNotice::PopupNotice()
{

}

PopupNotice::~PopupNotice()
{
	std::vector<PopupNoticeData*>::iterator it = _vec.begin();
	std::vector<PopupNoticeData*>::iterator it_end = _vec.end();

	for(; it != it_end; it++)
		delete *it;
}

PopupNotice* PopupNotice::instance()
{
	static PopupNotice _instance;
	return &_instance;
}

void PopupNotice::_insert( PopupNoticeData* data )
{
	std::vector<PopupNoticeData*>::iterator it = _vec.begin();
	std::vector<PopupNoticeData*>::iterator it_end = _vec.end();

	for(; it != it_end; it++)
	{
		if((*it)->html_num == data->html_num)
		{
			(*it)->start_time = data->start_time;
			(*it)->runtime = data->runtime;
			update_db(data);
			return ;
		}
	}
	_vec.push_back(data);
	save_db(data);
}

void PopupNotice::_delete( int html_num )
{
	std::vector<PopupNoticeData*>::iterator it = _vec.begin();
	std::vector<PopupNoticeData*>::iterator it_end = _vec.end();

	for(; it != it_end; it++)
	{
		if((*it)->html_num == html_num)
		{
			delete *it;
			_vec.erase(it);
			
			//DB에서 해당 데이터 삭제
			{
				std::string query = boost::str(boost::format("delete from t_popup_notice where a_html_index = %d and a_server = %d and a_subserver = %d") % html_num % gserver->m_serverno % gserver->m_subno);
				DBManager::instance()->pushQueryForDataDB(0, query);
			}
			return;
		}
	}
}

void PopupNotice::SendDataForLoop()
{
	std::vector<PopupNoticeData*>::iterator it = _vec.begin();
	std::vector<PopupNoticeData*>::iterator it_end = _vec.end();

	int expire_time = 0;

	for(; it != _vec.end();)
	{
		expire_time = (*it)->start_time + ((*it)->runtime * 60);

		if(expire_time > gserver->getNowSecond()
			&& ((*it)->start_time / 60) == (gserver->getNowSecond() /60)
			&& (*it)->isSend == false)
		{
			//전체 유저에게 전송
			CNetMsg::SP rmsg(new CNetMsg);
			CheckPopupNoticeMsg(rmsg, (*it)->html_num);
			PCManager::instance()->sendToAll(rmsg);
			(*it)->isSend = true;
			it++;
		}
		else if(expire_time - 60 < gserver->getNowSecond())
		{
			CNetMsg::SP rmsg(new CNetMsg);
			ClosePopupNoticeMsg(rmsg, (*it)->html_num);
			PCManager::instance()->sendToAll(rmsg);
			
			delete (*it);
			it = _vec.erase(it);
		}
		else
		{
			it++;
		}
	}
}

void PopupNotice::SendDataForUser(CPC* pc)
{
	std::vector<PopupNoticeData*>::iterator it = _vec.begin();
	std::vector<PopupNoticeData*>::iterator it_end = _vec.end();

	int expire_time = 0;

	for(; it != it_end; it++)
	{
		expire_time = (*it)->start_time + ((*it)->runtime * 60);

		if(expire_time > gserver->getNowSecond())
		{
			//전송
			CNetMsg::SP rmsg(new CNetMsg);
			CheckPopupNoticeMsg(rmsg, (*it)->html_num);
			SEND_Q(rmsg, pc->m_desc);
		}
	}
}

void PopupNotice::load()
{
	CDBCmd dbcmd;
	dbcmd.Init(&(gserver->m_dbdata));
	std::string query = boost::str(boost::format("select * from t_popup_notice where a_server = %d and a_subserver = %d and (a_start_time + (a_runtime * 60)) > %d") % gserver->m_serverno % gserver->m_subno % gserver->getNowSecond());
	dbcmd.SetQuery(query);
	if (!dbcmd.Open())
	{
		LOG_INFO("PopupNotice Date Load Error. mysql_error(%s)", mysql_error(dbcmd.m_dbconn));
		return ;
	}

	while(dbcmd.MoveNext())
	{
		PopupNoticeData* data = new PopupNoticeData();
		dbcmd.GetRec("a_html_index", data->html_num);
		dbcmd.GetRec("a_runtime", data->runtime);
		dbcmd.GetRec("a_start_time", data->start_time);
		_vec.push_back(data);
	}
}

void PopupNotice::save_db(PopupNoticeData* data)
{
	if(_vec.size() == 0)
		return ;

	std::string query = boost::str(boost::format("insert into t_popup_notice values (%d, %d, %d, %d, %d)") % data->html_num % data->runtime % data->start_time % gserver->m_serverno % gserver->m_subno);

	CDBCmd dbcmd;
	dbcmd.Init(&(gserver->m_dbdata));
	dbcmd.SetQuery(query);

	if (!dbcmd.Update())
	{
		LOG_INFO("PopupNotice Date Save Error. mysql_error(%s)", mysql_error(dbcmd.m_dbconn));
		return ;
	}
}

void PopupNotice::_clear()
{
	_vec.clear();
}

void PopupNotice::SendAdminSystemMsg(CPC* pc)
{
	std::vector<PopupNoticeData*>::iterator it = _vec.begin();
	std::vector<PopupNoticeData*>::iterator it_end = _vec.end();

	std::string str;

	for(; it != it_end; it++)
	{
		CNetMsg::SP rmsg(new CNetMsg);
		str = boost::str(boost::format("Popup_%d.htm %d") % (*it)->html_num % (*it)->runtime);
		SayMsg(rmsg, MSG_CHAT_GM, 0, "", "", str.c_str());
		SEND_Q(rmsg, pc->m_desc);
	}
}

void PopupNotice::update_db( PopupNoticeData* data )
{
	if(_vec.size() == 0)
		return ;

	std::string query = boost::str(boost::format("update t_popup_notice set a_runtime = %d, a_start_time = %d where a_html_index = %d and a_server = %d and a_subserver = %d") % data->runtime % data->start_time % data->html_num  % gserver->m_serverno % gserver->m_subno);

	CDBCmd dbcmd;
	dbcmd.Init(&(gserver->m_dbdata));
	dbcmd.SetQuery(query);

	if (!dbcmd.Update())
	{
		LOG_INFO("PopupNotice Date Save Error. mysql_error(%s)", mysql_error(dbcmd.m_dbconn));
		return ;
	}
}

bool Compare(NoticeStr* a, NoticeStr* b)
{
	return a->date > b->date;
}