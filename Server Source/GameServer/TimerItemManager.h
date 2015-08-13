#ifndef __TIMER_ITEM_MANAGER_H__
#define __TIMER_ITEM_MANAGER_H__

#include <map>
#include <set>
#include "../ShareLib/rnsocketioservice.h"
#include "../ShareLib/packetType/ptype_timer_item.h"

class TimerItemManager
{
public:
	typedef std::map<int /*type*/, unsigned int /*unix time*/> map_t;
	typedef std::set<TimerItemManager*> set_t;

public:
	TimerItemManager();
	~TimerItemManager();

	bool insert(int charIndex, int type, unsigned int time);
	unsigned int appendByDay(int type, int day);
	unsigned int appendBySeccond(int type, int seconds);
	unsigned int getTime(int type);
	void setOwner(CPC* owner);
	bool isRun()
	{
		return (owner_ != NULL);
	}

	void sendListToClient();
	void getDataToQuery(std::vector<std::string>& vec);

	void checkExpireTime( unsigned int nowtime );

	//////////////////////////////////////////////////////////////////////////
	bool isRemoteExpressSystem();
	bool isCashInventory_1();
	bool isCashInventory_2()
	{
		return flag_[TIMERITEM_TYPE_CASH_INVENTORY_2];
	}
	bool isPetStash()
	{
		return flag_[TIMERITEM_TYPE_PET_STASH];
	}

	void setTime(int type, int sec);

private:
	void deleteDataInDB(int type);
	bool empty()
	{
		return map_.empty();
	}

private:
	CPC*				owner_;
	map_t				map_;

	bool				flag_[TIMERITEM_MAX]; // 인덱스에 해당하는 기간제 아이템이 있다면 true

	//////////////////////////////////////////////////////////////////////////
public:
	static void AllcheckExpireTime(unsigned int nowtime);

private:
	static set_t		static_set_; // 기간제 아이템을 가지고 있는 객체를 저장, 매초마다 조건 비교를 최소화하기 위한 조치
};

#endif
