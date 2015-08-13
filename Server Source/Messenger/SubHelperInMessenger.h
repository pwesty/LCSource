#ifndef __SUBHELPER_IN_MESSENGER_H__
#define __SUBHELPER_IN_MESSENGER_H__

#include <map>
#include "../ShareLib/rnsocketiohandler.h"
#include "../ShareLib/rnsocketioservice.h"

class SubHelperProcessInMessenger;

class SubHelperInMessenger : public rnSocketIOHandler
{
public:
	typedef std::map<int /* sever no */, SubHelperProcessInMessenger*> map_t;

public:
	SubHelperInMessenger() {}
	~SubHelperInMessenger() {}

	bool Init();
	void insert(int serverno, SubHelperProcessInMessenger* sub);
	void erase(int serverno);

	bool sendMsgServerGroup(int serverno, CNetMsg::SP& rmsg);
	void sendMsgToAll(CNetMsg::SP& rmsg);

	virtual void operate(rnSocketIOService* service);

public:
	static SubHelperInMessenger* instance();

private:
	map_t		subhelper_map_;			// SubHelper의 session정보를 관리
};

//////////////////////////////////////////////////////////////////////////

class SubHelperProcessInMessenger : public rnSocketIOHandler
{
public:
	explicit SubHelperProcessInMessenger(rnSocketIOService* service) : service_(service), server_no_(0) {}
	~SubHelperProcessInMessenger() {}

	void WriteToOutput(CNetMsg::SP& msg);

	virtual void onClose(rnSocketIOService* service);
	virtual void operate(rnSocketIOService* service);

private:
	rnSocketIOService* service_;
	int server_no_;
};

#endif
