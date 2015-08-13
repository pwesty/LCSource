#ifndef __RESERVED_GM_COMMAND_MANAGER_H__
#define __RESERVED_GM_COMMAND_MANAGER_H__

#include <list>
#include <map>
#include "../ShareLib/rnsocketiohandler.h"
#include "../ShareLib/rnsocketioservice.h"
#include "../ShareLib/packetType/ptype_reserved_gm_command.h"

class ReservedGmCommandManager : public rnSocketIOHandler
{
public:
	struct dataInfo
	{
		bool start_active_flag_;			// false이면 모든 작동에서 제외를 시킨다.
		bool end_active_flag_;				// false이면 모든 작동에서 제외를 시킨다.
		int start_year;
		int start_month;
		int end_year;
		int end_month;
		reservedGMCommandElement ele;

		dataInfo() : start_active_flag_(true), end_active_flag_(true) {}
	};

	typedef std::map<int/*a_index*/, dataInfo> map_t;

public:
	ReservedGmCommandManager(void) {}
	~ReservedGmCommandManager(void) {}

	static ReservedGmCommandManager* Instance();

	bool Init();
	virtual void operate(rnSocketIOService* service);
	void sendListToClient(rnSocketIOService* service, int m_Index, int year, int month);

	void process(CNetMsg::SP& msg);

private:
	void Delete(int a_index);
	void Add(reservedGMCommandElement& ele);
	void makeMap(reservedGMCommandElement& ele);

private:
	map_t			map_;			// key = a_index
};

#endif
