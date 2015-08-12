#ifndef __ATTENDANCE_MANAGER_H__
#define __ATTENDANCE_MANAGER_H__

enum
{
	NONE,
	ATTENDANCE_SYS_MSG_CASE1,		//출석 유지 보험 종료
	ATTENDANCE_SYS_MSG_CASE2,		//게임 접속 중인상태에서 아이템 기간이 만료 또는 출석 유지 보험의 기간이 만료된 당일 게임에 최초 접속 하였을 때
	ATTENDANCE_SYS_MSG_CASE3,		//게임에 접속 중이 아닐 때 만료되고 만료된 당일이 지난 후 게임에 접속 하였을 때
};

class CAttendanceManager
{
public:
	CAttendanceManager(CPC* pc);
	~CAttendanceManager();

	CPC* _owner;

private:
	int m_acc_count;
	int m_max_acc;
	int m_last_reward_point;

	bool m_isAttendance;
	bool m_isUseAssure;
	bool m_isFirst;

	int m_sysMsg;

public:
	void setAttendance(bool isAttendance);
	bool getAttendance();

	void setUseAssure(bool isUseAssure);
	bool getUseAssure();

	void setAccCount(int accCount);
	int getAccCount();

	void setMaxAcc(int maxAcc);
	int getMaxAcc();

	void setLastRewardPoint(int last_reward_point);
	int getLastRewardPoint();

	void setSysMsg(int sysMsg);
	int getSysMsg();

	void load(MYSQL* conn);

	void checkAttendance();
	void finish();

	void reward();

	void assureOk();

	void sendSysMsg();
};

#endif