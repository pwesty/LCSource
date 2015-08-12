#include <boost/format.hpp>
#include "stdhdrs.h"

#include "Descriptor.h"
#include "../ShareLib/DBCmd.h"

#include "Log.h"
#include "Character.h"
#include "Server.h"
#include "CmdMsg.h"
#include "doFunc.h"
#include "hardcoding.h"
#include "DBManager.h"

#define MAX_REPORT_DESC_SIZE 200

typedef enum _ReportSystemResponse {
	MSG_REPORT_OK=0,
	MSG_REPORT_BAD_NAME,
	MSG_REPORT_UNDEFINED,
	MSG_REPORT_CLIENT,
	MSG_REPORT_SERVER,
} MSG_REPORT_SYS_REPLY;

#pragma pack(push, 1)
//////////////////////////////////////////////////////////////////////////
namespace RequestClient
{
	struct ReportFromClient : public pTypeBase
	{
		int	 reason;
	    char reportedName[MAX_CHAR_NAME_LENGTH+1];
	    char reportMsg[MAX_REPORT_DESC_SIZE+1];
	};
}
//////////////////////////////////////////////////////////////////////////
#pragma pack(pop)

int flip_bytes(int value) {
	int ret = 0;

	char* n1 = (char *)&value;
	char* n2 = (char *)&ret;

	*(n2 + 0) =  *(n1 + 3);  // or n2[0] = n1[3];
	*(n2 + 1) =  *(n1 + 2);  // or n2[1] = n1[2];
	*(n2 + 2) =  *(n1 + 1);  // or n2[2] = n1[1]; 
	*(n2 + 3) =  *(n1 + 0);  // or n2[3] = n1[0];

	return ret;
}

void do_ReportForced(CPC* pc, CNetMsg::SP& msg) {
	if(pc == NULL)
		return;

	// Physical Packet Storage
	RequestClient::ReportFromClient* packet = reinterpret_cast<RequestClient::ReportFromClient*>(msg->m_buf);

	// Get PCManager this is to see if that player exists
	PCManager* mgr = PCManager::instance();
	
	// Our reply message
	CNetMsg::SP rmsg(new CNetMsg);

	int reason = -1;
	CLCString reportedName(MAX_CHAR_NAME_LENGTH);
	CLCString reportMsg(MAX_REPORT_DESC_SIZE);

	reason = packet->reason;
	reportedName = packet->reportedName;
	reportMsg = packet->reportMsg;

	// Correct byte order
	if(reason > 10)
		reason = flip_bytes(reason);

	GAMELOG << " Name Reported " << reportedName << end;

	CPC* reportedCPC = mgr->getPlayerByName(reportedName, true);

	// if the reported player does not exist return
	if(!reportedCPC || !pc) {
			rmsg->Init(MSG_SYS);
			RefMsg(rmsg) << (unsigned char)118
			<< (unsigned char)MSG_REPORT_BAD_NAME;
			SEND_Q(rmsg, pc->m_desc);
			return;
	}

	// Create string to hold our query
	std::string insertQuery;

	char* msg_clean = 0;

	// Allocate a buffer for a clean string
	if(reportMsg.Length() > 0) {
		msg_clean = new char[reportMsg.Length()];
		memset(msg_clean, 0, reportMsg.Length());

		// Clean off the report msg string to prevent SQL injections
		mysql_real_escape_string(&gserver->m_dbchar, msg_clean, reportMsg.getBuffer(), reportMsg.Length());
		
		reportMsg = msg_clean;
	}

	// Build our query
	int reporter = pc->m_index;
	int reported = reportedCPC->m_index;

	insertQuery = boost::str(boost::format("INSERT INTO t_reports (a_reporter, a_reported, a_reason, a_msg) VALUES (%1%, %2%, %3%, \"%4%\")")
		% reporter % reported % reason % reportMsg.getBuffer());

	GAMELOG << "Adding to query list" << end;
	// Set the query
	DBManager::instance()->pushQuery(0, insertQuery);

	GAMELOG << "report submitted!" << end;

	// After updating the DB inform the client we have finished our work
	rmsg->Init(MSG_SYS);
	RefMsg(rmsg) << (unsigned char)118
	<< (unsigned char)MSG_REPORT_OK;
	SEND_Q(rmsg, pc->m_desc);

	if(msg_clean) {
		delete[] msg_clean;
		msg_clean = 0;
	}
}