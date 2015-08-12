#ifndef __GM_TOOL_IN_MESSENGER_H__
#define __GM_TOOL_IN_MESSENGER_H__

#include "../ShareLib/rnsocketiohandler.h"
#include "../ShareLib/rnsocketioservice.h"

class GMToolInMessenger : public rnSocketIOHandler
{
public:
	GMToolInMessenger() : gmToolLogined_(false) {}
	~GMToolInMessenger() {}

	bool Init();

	void setFlag()
	{
		gmToolLogined_ = true;
	}
	void resetFlag()
	{
		gmToolLogined_ = false;
	}
	bool isLogined()
	{
		return gmToolLogined_;
	}

	virtual void operate(rnSocketIOService* service);

public:
	static GMToolInMessenger* instance();

private:
	bool	gmToolLogined_;				// GM TOOL이 접속하면 true, 즉 1개의 GM TOOL만이 접속 가능
};

//////////////////////////////////////////////////////////////////////////

class GMToolProcessInMessenger : public rnSocketIOHandler
{
public:
	explicit GMToolProcessInMessenger(rnSocketIOService* service) : service_(service) {}
	~GMToolProcessInMessenger() {}

	virtual void onClose(rnSocketIOService* service);
	virtual void operate(rnSocketIOService* service);

private:
	void process_alluser(CNetMsg::SP& msg);
	void process_server_group(CNetMsg::SP& msg);
	void process_zone(CNetMsg::SP& msg);
	void process_oneuser(CNetMsg::SP& msg);

	void process_reserved_gm_command_add(CNetMsg::SP& msg);
	void process_reserved_gm_command_delete(CNetMsg::SP& msg);
	bool isValidGmCommand(std::string tstr);

private:
	rnSocketIOService* service_;
};

#endif
