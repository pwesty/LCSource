#ifndef __PTYPE_OLD_DO_CHANGE_JOB_H__
#define __PTYPE_OLD_DO_CHANGE_JOB_H__

#include "ptype_base.h"

#pragma pack(push, 1)
//////////////////////////////////////////////////////////////////////////
namespace RequestClient
{
	struct changeJob : public pTypeBase 
	{
		int			npcIndex;	// memory index
		char		job;
	};

	struct changeJobReset : public pTypeBase
	{
		int			npcIndex;	// memory index
	};
}

namespace ResponseClient
{
	struct changeJobError : public pTypeBase 
	{
		char		errorCode;
	};

	struct changeJob : public pTypeBase 
	{
		int			nIndex;
		char		job1;
		char		job2;
	};

	struct changeJobReset : public pTypeBase 
	{
		int			charIndex;
		char		job;
	};

#ifndef _CLIENT_
	inline void makeChangeJobError(CNetMsg::SP& msg, int erroCode)
	{
		changeJobError* packet = reinterpret_cast<changeJobError*>(msg->m_buf);
		packet->type = MSG_CHANGEJOB;
		packet->subType = MSG_CHANGEJOB_ERROR;
		packet->errorCode = erroCode;
		msg->setSize(sizeof(changeJobError));
	}

	inline void makeChangeJob(CNetMsg::SP& msg, int nIndex, int job1, int job2)
	{
		changeJob* packet = reinterpret_cast<changeJob*>(msg->m_buf);
		packet->type = MSG_CHANGEJOB;
		packet->subType = MSG_CHANGEJOB_REP;
		packet->nIndex = nIndex;
		packet->job1 = job1;
		packet->job2 = job2;
		msg->setSize(sizeof(changeJob));
	}

	inline void makeChangeJobReset(CNetMsg::SP& msg, CPC* ch)
	{
		if(ch->m_job == JOB_NIGHTSHADOW)
		{
			msg->Init();
			return;
		}

		changeJobReset* packet = reinterpret_cast<changeJobReset*>(msg->m_buf);
		packet->type = MSG_CHANGEJOB;
		packet->subType = MSG_CHANGEJOB_RESET_REP;
		packet->charIndex = ch->m_index;
		packet->job = ch->m_job;
		msg->setSize(sizeof(changeJobReset));
	}
#endif
}
//////////////////////////////////////////////////////////////////////////
#pragma pack(pop)

#endif
