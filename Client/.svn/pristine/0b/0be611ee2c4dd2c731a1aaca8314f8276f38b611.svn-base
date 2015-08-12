#ifndef __PTYPE_ATTENDANCE_H__
#define __PTYPE_ATTENDANCE_H__

#include "ptype_base.h"

//subtype
enum
{
	//Requset
	MSG_SUB_ATTENDANCE_CHECK_REQ,
	MSG_SUB_ATTENDANCE_REWARD_REQ,
	MSG_SUB_ATTENDANCE_ASSURE_CANCEL_OK_REQ,	//보험 아이템 사용 종료 하겠다

	//Response
	MSG_SUB_ATTENDANCE_CHECK_RES,				//기존 RESPONSE 1번
	MSG_SUB_ATTENDANCE_REWARD_RES,				//기존 RESPONSE 3번

	//Update
	MSG_SUB_ATTENDANCE_FINISH,					//기존 RESPONSE 2번
	MSG_SUB_ATTENDANCE_INFO_UPD,				//기존 RESPONSE 0번
	MSG_SUB_ATTENDANCE_ASSURE_CANCEL_OK_UPD,	//보험 아이템 사용 해제 하겠니?
	MSG_SUB_ATTENDANCE_SYS_MSG,					//출석 시스템 메시지
};

//error code
enum
{
	ATTENDANCE_ERROR_SUCCESS,
	ATTENDANCE_ERROR_NO_REWARD,
	ATTENDANCE_ERROR_INVEN_FULL,
};

//system message
enum
{
	ATTENDANCE_SYS_MSG_TIME_END = 1,					//출석 유지 보험의 사용 기간이 만료되었습니다.
	ATTENDANCE_SYS_MSG_END_OK = 2,						//출석 유지 보험이 해제되었습니다. 현재 연속출석....
	ATTENDANCE_SYS_MSG_TIME_END_INIT_ATTENDANCE = 3,	//출석 유지 보험이 만료된 후 연속 출석을 하지 않아 출석 일 수가 초기화 되었습니다.
};

#pragma pack(push, 1)

namespace RequestClient
{
	struct AttendanceCheck : public pTypeThirdBase
	{
	};

	struct AttendanceReward : public pTypeThirdBase
	{
	};

	struct AttendanceAssureOk : public pTypeThirdBase
	{
	};
}

namespace ResponseClient
{
	struct AttendanceCheck : public pTypeThirdBase
	{
		int acc_count;
		int up_exp;
	};

	struct AttendanceReward : public pTypeThirdBase
	{
		int error_code;
	};

	struct AttendanceAssureOk : public pTypeThirdBase
	{
	};

#ifndef _CLIENT_
	inline void makeAttendanceCheckMsg(CNetMsg::SP& rmsg, int acc_count, int up_exp)
	{
		AttendanceCheck* packet = reinterpret_cast<AttendanceCheck*>(rmsg->m_buf);
		packet->type = MSG_EXTEND;
		packet->subType = MSG_EX_ATTENDANCE_EXP_SYSTEM;
		packet->thirdType = MSG_SUB_ATTENDANCE_CHECK_RES;
		packet->acc_count = acc_count;
		packet->up_exp = up_exp;
		rmsg->setSize(sizeof(AttendanceCheck));
	}

	inline void makeAttendanceRewardMsg(CNetMsg::SP& rmsg, int error_code)
	{
		AttendanceReward* packet = reinterpret_cast<AttendanceReward*>(rmsg->m_buf);
		packet->type = MSG_EXTEND;
		packet->subType = MSG_EX_ATTENDANCE_EXP_SYSTEM;
		packet->thirdType = MSG_SUB_ATTENDANCE_REWARD_RES;
		packet->error_code = error_code;
		rmsg->setSize(sizeof(AttendanceReward));
	}

	inline void makeAttendanceAssureCancelOkMsg(CNetMsg::SP& rmsg)
	{
		AttendanceAssureOk* packet = reinterpret_cast<AttendanceAssureOk*>(rmsg->m_buf);
		packet->type = MSG_EXTEND;
		packet->subType = MSG_EX_ATTENDANCE_EXP_SYSTEM;
		packet->thirdType = MSG_SUB_ATTENDANCE_ASSURE_CANCEL_OK_RES;
		rmsg->setSize(sizeof(AttendanceAssureOk));
	}
#endif
}

namespace UpdateClient
{
	struct AttendanceFinish : public pTypeThirdBase
	{
		int acc_count;
		int up_exp;
	};

	struct AttendanceInfo : public pTypeThirdBase
	{
		int acc_count;
		int up_exp;
	};

	struct AttendanceAssureOk : public pTypeThirdBase
	{
	};

	struct AttendanceTimeEnd : public pTypeThirdBase
	{
	};

	struct AttendanceSysMsg : public pTypeThirdBase
	{
		int sysMsg;
	};

#ifndef _CLIENT_
	inline void makeAttendanceFinish(CNetMsg::SP& rmsg, int acc_count, int up_exp)
	{
		AttendanceFinish* packet = reinterpret_cast<AttendanceFinish*>(rmsg->m_buf);
		packet->type = MSG_EXTEND;
		packet->subType = MSG_EX_ATTENDANCE_EXP_SYSTEM;
		packet->thirdType = MSG_SUB_ATTENDANCE_FINISH;
		packet->acc_count = acc_count;
		packet->up_exp = up_exp;
		rmsg->setSize(sizeof(AttendanceFinish));
	}

	inline void makeAttendanceInfo(CNetMsg::SP& rmsg, int acc_count, int up_exp)
	{
		AttendanceInfo* packet = reinterpret_cast<AttendanceInfo*>(rmsg->m_buf);
		packet->type = MSG_EXTEND;
		packet->subType = MSG_EX_ATTENDANCE_EXP_SYSTEM;
		packet->thirdType = MSG_SUB_ATTENDANCE_INFO_UPD;
		packet->acc_count = acc_count;
		packet->up_exp = up_exp;
		rmsg->setSize(sizeof(AttendanceInfo));
	}

	inline void makeAttendanceAssureCancelOkMsg(CNetMsg::SP& rmsg)
	{
		AttendanceAssureOk* packet = reinterpret_cast<AttendanceAssureOk*>(rmsg->m_buf);
		packet->type = MSG_EXTEND;
		packet->subType = MSG_EX_ATTENDANCE_EXP_SYSTEM;
		packet->thirdType = MSG_SUB_ATTENDANCE_ASSURE_CANCEL_OK_UPD;
		rmsg->setSize(sizeof(AttendanceAssureOk));
	}

	inline void makeAttendanceSysMsg(CNetMsg::SP& rmsg, int sysMsg)
	{
		AttendanceSysMsg* packet = reinterpret_cast<AttendanceSysMsg*>(rmsg->m_buf);
		packet->type = MSG_EXTEND;
		packet->subType = MSG_EX_ATTENDANCE_EXP_SYSTEM;
		packet->thirdType = MSG_SUB_ATTENDANCE_SYS_MSG;
		packet->sysMsg = sysMsg;
		rmsg->setSize(sizeof(AttendanceSysMsg));
	}
#endif
}

#pragma pack(pop)

#endif