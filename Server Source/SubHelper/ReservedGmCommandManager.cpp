#include <time.h>
#include <vector>
#include <boost/format.hpp>
#include <boost/foreach.hpp>
#include "stdhdrs.h"

#include "../ShareLib/bnf.h"
#include "Utils.h"
#include "../ShareLib/DBCmd.h"
#include "../ShareLib/Conf.h"
#include "Server.h"
#include "Log.h"
#include "ReservedGmCommandManager.h"

#define MAKE_YEAR_MONTH_KEY(x, y)				((x) << 16 | (y))

ReservedGmCommandManager* ReservedGmCommandManager::Instance()
{
	static ReservedGmCommandManager __instance;
	return &__instance;
}

bool ReservedGmCommandManager::Init()
{
	CDBCmd dbChar;
	dbChar.Init(&gserver.m_dbchar);

	// 시간이 경과된 데이터는 삭제한다.
	dbChar.SetQuery("DELETE FROM t_reserved_gm_command WHERE a_start_time <= now() AND a_end_time <= now()");
	if (dbChar.Update() == false)
		return false;

	// DB에 기록되어있는 기존 데이터를 읽어들인다.
	std::string qry = boost::str(boost::format(
									 "SELECT a_index, a_event_type, UNIX_TIMESTAMP(a_start_time) as a_start_time, a_start_string,"
									 "UNIX_TIMESTAMP(a_end_time) as a_end_time, a_end_string, a_image_type, a_image_x, a_image_y, a_title, a_start_title, a_end_title, a_descr"
									 " FROM t_reserved_gm_command WHERE a_server=%1% AND (a_start_time > now() OR a_end_time > now()) ORDER BY a_index")
								 % gserver.m_serverno);
	dbChar.SetQuery(qry.c_str());
	if (dbChar.Open() == false)
		return false;

	while (dbChar.MoveNext())
	{
		reservedGMCommandElement ele;

		dbChar.GetRec("a_index", ele.a_Index);
		dbChar.GetRec("a_event_type", ele.a_evnetType);
		dbChar.GetRec("a_start_time", ele.a_startTime);
		strncpy(ele.a_startString, dbChar.GetRec("a_start_string"), sizeof(ele.a_startString));
		dbChar.GetRec("a_end_time", ele.a_endTime);
		strncpy(ele.a_endString, dbChar.GetRec("a_end_string"), sizeof(ele.a_endString));
		dbChar.GetRec("a_image_type", ele.a_image_type);
		dbChar.GetRec("a_image_x", ele.a_image_x);
		dbChar.GetRec("a_image_y", ele.a_image_y);
		strncpy(ele.a_title, dbChar.GetRec("a_title"), sizeof(ele.a_title));
		strncpy(ele.a_start_title, dbChar.GetRec("a_start_title"), sizeof(ele.a_start_title));
		strncpy(ele.a_end_title, dbChar.GetRec("a_end_title"), sizeof(ele.a_end_title));
		strncpy(ele.a_desc, dbChar.GetRec("a_descr"), sizeof(ele.a_desc));

		this->makeMap(ele);
	}

	dbChar.Close();

	//////////////////////////////////////////////////////////////////////////
	// 매분마다 검사를 위한 타이머를 생성
	time_t nowtime = time(0);
	int remain_sec = 60 - ((int)nowtime % 60);

	bnf::instance()->CreateSecTimerPeriod(remain_sec, 60, this);

	return true;
}

void ReservedGmCommandManager::operate( rnSocketIOService* service )
{
	std::vector<int /*a_index*/> delete_a_index;
	int nowtime = (int)time(0);
	map_t::iterator it = map_.begin();
	map_t::iterator endit = map_.end();
	for (; it != endit; ++it)
	{
		dataInfo& d = it->second;

		// start, end 시간이 모두 경과된 경우 삭제하도록 함
		if (d.ele.a_startTime < nowtime && d.ele.a_endTime < nowtime)
		{
			delete_a_index.push_back(d.ele.a_Index);
			continue;
		}

		if (d.ele.a_evnetType == eEVENT_TYPE_INFO) // 단순 정보 출력용이면 검사하지 않음
			continue;

		// 시작 시간 검사
		if ((d.start_active_flag_) && (d.ele.a_startTime <= nowtime))
		{
			d.start_active_flag_ = false;

			CNetMsg::SP rmsg(new CNetMsg);
			ResposeGameServer::makeAddReservedGMCommand(rmsg, d.ele.a_startString);
			gserver.SendToAllGameServer(rmsg);
		}

		// 끝 시간 검사
		if ((d.end_active_flag_) && (d.ele.a_endTime <= nowtime))
		{
			d.end_active_flag_ = false;

			CNetMsg::SP rmsg(new CNetMsg);
			ResposeGameServer::makeAddReservedGMCommand(rmsg, d.ele.a_endString);
			gserver.SendToAllGameServer(rmsg);
		}
	}

	// 삭데될 데이터 처리
	BOOST_FOREACH(int a_index, delete_a_index)
	{
		this->Delete(a_index);
	}
}

void ReservedGmCommandManager::Delete(int a_index)
{
	map_t::iterator it = map_.find(a_index);
	if (it == map_.end())
	{
		LOG_ERROR("ReservedGmCommand : Not found a_index[%d] in delete", a_index);
		return;
	}

	// DB에서 제거
	CDBCmd dbChar;
	dbChar.Init(&gserver.m_dbchar);
	std::string qry = boost::str(boost::format(
									 "DELETE FROM t_reserved_gm_command WHERE a_index=%1% AND a_server=%2%")
								 % a_index % gserver.m_serverno);
	dbChar.SetQuery(qry.c_str());
	dbChar.Update();

	map_.erase(a_index);
}

void ReservedGmCommandManager::sendListToClient( rnSocketIOService* service, int m_Index, int year, int month )
{
	time_t nowtime = time(0);
	struct tm* nowtm = localtime(&nowtime);

	CNetMsg::SP rmsg(new CNetMsg);
	ResponseClient::reservedGMCommandList* packet = reinterpret_cast<ResponseClient::reservedGMCommandList*>(rmsg->m_buf);
	rmsg->m_mtype = packet->type = MSG_RESERVED_GM_COMMAND;
	packet->subType = MSG_SUB_RESERVED_GM_COMMAND_LIST;
	packet->m_Index = m_Index;
	packet->nowTime = (int)nowtime;

	if (year == 0 && month == 0)	// 클라이언트가 처음 보낼때
	{
		packet->year = nowtm->tm_year + 1900;
		packet->month = nowtm->tm_mon + 1;
	}
	else
	{
		packet->year = year;
		packet->month = month;
	}

	int count = 0;
	map_t::iterator it = map_.begin();
	map_t::iterator endit = map_.end();
	for (; it != endit; ++it)
	{
		dataInfo& d = it->second;
		if ((d.start_year == packet->year && d.start_month == packet->month) ||
				(d.end_year == packet->year && d.end_month == packet->month))
		{
			memcpy(&packet->ele[count], &d.ele, sizeof(d.ele));

			// 클라이언트는 아래의 정보를 알 필요가 없으므로 공백 처리
			memset(packet->ele[count].a_startString, 0x00, sizeof(packet->ele[count].a_startString));
			memset(packet->ele[count].a_endString, 0x00, sizeof(packet->ele[count].a_endString));

			++count;
		}
	}
	packet->count = count;

	rmsg->setSize(sizeof(ResponseClient::reservedGMCommandList) + (count * sizeof(reservedGMCommandElement)));
	service->deliver(rmsg);
}

void ReservedGmCommandManager::Add( reservedGMCommandElement& ele )
{
	map_t::iterator it = map_.find(ele.a_Index);
	if (it != map_.end())
	{
		LOG_ERROR("ReservedGmCommand : duplicate a_index[%d] in add", ele.a_Index);
		return;
	}

	char a_startString[RESERVED_GM_COMMAND_MAX_STRING * 2];
	char a_endString[RESERVED_GM_COMMAND_MAX_STRING * 2];
	mysql_escape_string(a_startString, ele.a_startString, strlen(ele.a_startString));
	mysql_escape_string(a_endString, ele.a_endString, strlen(ele.a_endString));

	char a_titleString[RESERVED_GM_COMMAND_TITLE_MAX_STRING * 2];
	char a_start_titleString[RESERVED_GM_COMMAND_TITLE_MAX_STRING * 2];
	char a_end_titleString[RESERVED_GM_COMMAND_TITLE_MAX_STRING * 2];
	mysql_escape_string(a_titleString, ele.a_title, strlen(ele.a_title));
	mysql_escape_string(a_start_titleString, ele.a_start_title, strlen(ele.a_start_title));
	mysql_escape_string(a_end_titleString, ele.a_end_title, strlen(ele.a_end_title));

	char a_descString[RESERVED_GM_COMMAND_DESC_MAX_STRING * 2];
	mysql_escape_string(a_descString, ele.a_desc, strlen(ele.a_desc));

	// DB에 추가
	CDBCmd dbChar;
	dbChar.Init(&gserver.m_dbchar);
	std::string qry = boost::str(boost::format(
									 "INSERT INTO t_reserved_gm_command(a_index, a_server, a_event_type,"
									 "a_start_time, a_start_string, a_end_time, a_end_string,"
									 "a_image_type, a_image_x, a_image_y, a_title, a_start_title, a_end_title, a_registe_time, a_descr) VALUES("
									 "%1%,%2%,%3%,"
									 "FROM_UNIXTIME(%4%),'%5%',FROM_UNIXTIME(%6%),'%7%',"
									 "%8%,%9%,%10%,'%11%','%12%','%13%',now(),'%14%')")
								 % ele.a_Index % gserver.m_serverno % (int)ele.a_evnetType
								 % ele.a_startTime % a_startString % ele.a_endTime % a_endString
								 % (int)ele.a_image_type % ele.a_image_x % ele.a_image_y
								 % a_titleString % a_start_titleString % a_end_titleString % a_descString);
	dbChar.SetQuery(qry.c_str());
	if (dbChar.Update() == false)
		return;

	this->makeMap(ele);
}

void ReservedGmCommandManager::process( CNetMsg::SP& msg )
{
	pTypeBase* pBase = reinterpret_cast<pTypeBase*>(msg->m_buf);
	switch (pBase->subType)
	{
	case MSG_SUB_ADD_RESERVED_GM_COMMAND:
		{
			RequestGMTool::addReservedGMCommand* packet = reinterpret_cast<RequestGMTool::addReservedGMCommand*>(msg->m_buf);
			this->Add(packet->element);
		}
		break;
	case MSG_SUB_DELETE_RESERVED_GM_COMMAND:
		{
			RequestGMTool::deleteReservedGMCommand* packet = reinterpret_cast<RequestGMTool::deleteReservedGMCommand*>(msg->m_buf);
			this->Delete(packet->a_Index);
		}
		break;
	default:
		break;
	}
}

void ReservedGmCommandManager::makeMap( reservedGMCommandElement& ele )
{
	struct tm starttm;
	struct tm endtm;

	time_t starttime = (time_t)ele.a_startTime;
	time_t endtime = (time_t)ele.a_endTime;

#ifdef WIN32
	localtime_s(&starttm, &starttime);
	localtime_s(&endtm, &endtime);
#else
	localtime_r((time_t*)&ele.a_startTime, &starttm);
	localtime_r((time_t*)&ele.a_endTime, &endtm);
#endif

	dataInfo d;
	d.start_year = starttm.tm_year + 1900;
	d.start_month = starttm.tm_mon + 1;
	d.end_year = endtm.tm_year + 1900;
	d.end_month = endtm.tm_mon + 1;
	d.ele = ele;

	map_.insert(map_t::value_type(ele.a_Index, d));
}