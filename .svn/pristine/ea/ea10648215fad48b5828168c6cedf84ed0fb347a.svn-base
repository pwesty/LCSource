#ifndef __MESSENGER_IN_GAME_H__
#define __MESSENGER_IN_GAME_H__

#include "../ShareLib/rnsocketiohandler.h"
#include "../ShareLib/rnsocketioservice.h"
#include "../ShareLib/packetType/ptype_express_system.h"

class MessengerInSubHelperConnectTimer : public rnSocketIOHandler
{
public:
	MessengerInSubHelperConnectTimer() {}
	~MessengerInSubHelperConnectTimer() {}

	virtual void operate(rnSocketIOService* service);

	static MessengerInSubHelperConnectTimer* instance();
};

//////////////////////////////////////////////////////////////////////////

class MessengerInSubHelper : public rnSocketIOHandler
{
public:
	MessengerInSubHelper() {}
	~MessengerInSubHelper() {}

	void connect();
	void connect(std::string host, std::string port);
	bool isRun()
	{
		return (service_ != NULL);
	}
	void WriteToOutput(CNetMsg::SP& msg);

public:
	virtual void operate(rnSocketIOService* service);
	virtual void onClose(rnSocketIOService* service);

	virtual void onConnect(rnSocketIOService* service);
	virtual void onConnectFail(rnSocketIOService* service);

	void process_InputItemToExpress(int charIndex, ExpressSystemItemInfo& item);

public:
	static MessengerInSubHelper* instance();

private:
	void process_ExpressSystem(CNetMsg::SP& msg);

private:
	rnSocketIOService*		service_;
	std::string				host_;
	int						port_;
};

#endif
